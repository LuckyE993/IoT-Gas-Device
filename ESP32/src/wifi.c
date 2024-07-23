#include "wifi.h"
static EventGroupHandle_t s_wifi_event_group;

static int s_retry_num = 0;


static const char *TAG_WIFI = "WIFI";
static const char *TAG_SMARTCONFIG = "SMARTCONFIG";
/* FreeRTOS event group to signal when we are connected*/
static void smartconfig_task(void);

static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT) 
    {
        if(event_id == WIFI_EVENT_STA_START)
        {
            esp_wifi_connect();
        }
        if(event_id == WIFI_EVENT_STA_DISCONNECTED)
        {
            if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) 
            {
                esp_wifi_connect();
                s_retry_num++;
                ESP_LOGI(TAG_WIFI, "retry to connect to the AP");
            } 
            else 
            {
                xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
            }
            ESP_LOGI(TAG_WIFI,"connect to the AP fail");
        }
    } 
    
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG_WIFI, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
    else if(event_base==SC_EVENT)
    {
        if(event_id==SC_EVENT_SCAN_DONE)
        {
            ESP_LOGI(TAG_SMARTCONFIG,"smartconfig scan done");
        }
        else if(event_id==SC_EVENT_FOUND_CHANNEL)
        {
            ESP_LOGI(TAG_SMARTCONFIG,"smartconfig found channel");
        }
        else if(event_id==SC_EVENT_GOT_SSID_PSWD)
        {
            ESP_LOGI(TAG_SMARTCONFIG,"smartconfig got ssid and pswd");
            smartconfig_event_got_ssid_pswd_t *evt = (smartconfig_event_got_ssid_pswd_t *)event_data;
            wifi_config_t wifi_config;
            uint8_t ssid[33] = { 0 };
            uint8_t password[65] = { 0 };
            uint8_t rvd_data[33] = { 0 };
            const char* ssid_str = (const char*) ssid;
            const char* password_str = (const char*) password;

            bzero(&wifi_config, sizeof(wifi_config_t));
            memcpy(wifi_config.sta.ssid, evt->ssid, sizeof(wifi_config.sta.ssid));
            memcpy(wifi_config.sta.password, evt->password, sizeof(wifi_config.sta.password));
            wifi_config.sta.bssid_set = evt->bssid_set;
            if (wifi_config.sta.bssid_set == true) {
                memcpy(wifi_config.sta.bssid, evt->bssid, sizeof(wifi_config.sta.bssid));
            }

            memcpy(ssid, evt->ssid, sizeof(evt->ssid));
            memcpy(password, evt->password, sizeof(evt->password));
            ESP_LOGI(TAG_SMARTCONFIG, "SSID_LENGTH:%d", sizeof(evt->ssid));
            ESP_LOGI(TAG_SMARTCONFIG, "SSID:%s", ssid);
            ESP_LOGI(TAG_SMARTCONFIG, "PASSWORD:%s", password);

            /* 将得到的WiFi名称和密码存入NVS*/
            nvs_handle wificfg_nvs_handler;
            ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_ssid",ssid_str) );
            ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_passwd",password_str) );
            ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
            nvs_close(wificfg_nvs_handler);                     /* 关闭 */ 
            printf("smartconfig save wifi_cfg to NVS .\n");

            if (evt->type == SC_TYPE_ESPTOUCH_V2) {
                ESP_ERROR_CHECK( esp_smartconfig_get_rvd_data(rvd_data, sizeof(rvd_data)) );
                ESP_LOGI(TAG_SMARTCONFIG, "RVD_DATA:");
                for (int i=0; i<33; i++) {
                    printf("%02x ", rvd_data[i]);
                }
                printf("\n");
            }

            ESP_ERROR_CHECK( esp_wifi_disconnect() );
            ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
            esp_wifi_connect();
        }
        else if(event_id==SC_EVENT_SEND_ACK_DONE)
        {
            ESP_LOGI(TAG_SMARTCONFIG,"smartconfig send ack done");
            xEventGroupSetBits(s_wifi_event_group, SMART_CONFIG_BIT);
        }

    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));
    ESP_ERROR_CHECK( esp_event_handler_register(SC_EVENT, 
                                                ESP_EVENT_ANY_ID, 
                                                &event_handler,
                                                NULL) );
    // Get NVS Wifi Config
    nvs_handle wificfg_nvs_handler; /* 定义一个NVS操作句柄 */

    char wifi_ssid[32] = { 0 };     /* 定义一个数组用来存储ssid*/
    char wifi_passwd[64] = { 0 };   /* 定义一个数组用来存储passwd */

    size_t len;

    /* 打开一个NVS命名空间 */
    ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );

    len = sizeof(wifi_ssid);    /* 从NVS中获取ssid */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_ssid",wifi_ssid,&len) );

    len = sizeof(wifi_passwd);      /* 从NVS中获取ssid */
    ESP_ERROR_CHECK( nvs_get_str(wificfg_nvs_handler,"wifi_passwd",wifi_passwd,&len) );
    
    ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
    nvs_close(wificfg_nvs_handler);                     /* 关闭 */
    //End Get NVS Wifi Config

    wifi_config_t wifi_config;
    bzero(&wifi_config, sizeof(wifi_config_t)); /* 将结构体数据清零 */
    memcpy(wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    memcpy(wifi_config.sta.password, wifi_passwd, sizeof(wifi_config.sta.password));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG_WIFI, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) 
    {
        

        ESP_LOGI(TAG_WIFI, "connected to ap SSID:%s password:%s",
                 wifi_ssid, wifi_passwd);
        
        // storage_wifi_config();
    } else if (bits & WIFI_FAIL_BIT) 
    {
        ESP_LOGI(TAG_WIFI, "Failed to connect to SSID:%s, password:%s",
                 wifi_ssid, wifi_passwd);

        smartconfig_task();

    } else {
        ESP_LOGE(TAG_WIFI, "UNEXPECTED EVENT");
        smartconfig_task();
    }
}
static void smartconfig_task(void)
{
    EventBits_t uxBits;
    ESP_ERROR_CHECK( esp_smartconfig_set_type(SC_TYPE_ESPTOUCH) );
    smartconfig_start_config_t cfg = SMARTCONFIG_START_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_smartconfig_start(&cfg) );
    while (1) {
        uxBits = xEventGroupWaitBits(s_wifi_event_group, WIFI_CONNECTED_BIT | SMART_CONFIG_BIT, true, false, portMAX_DELAY);
        if(uxBits & WIFI_CONNECTED_BIT) {
            ESP_LOGI(TAG_SMARTCONFIG, "WiFi Connected to ap");
            esp_smartconfig_stop();
        }
        if(uxBits & SMART_CONFIG_BIT) {
            ESP_LOGI(TAG_SMARTCONFIG, "smartconfig over");
            esp_smartconfig_stop();
            // vTaskDelete(NULL);
        }
    }
}
void wifi_config_init(void)
{
    nvs_handle wificfg_nvs_handler;
    /* 打开一个NVS命名空间 */
    ESP_ERROR_CHECK( nvs_open("WiFi_cfg", NVS_READWRITE, &wificfg_nvs_handler) );

    uint32_t wifi_update = 0;

    esp_err_t err = nvs_get_u32(wificfg_nvs_handler,"wifi_update",&wifi_update);

    if(MY_WIFI_UPDATE == wifi_update )
        printf("wifi_cfg needn't to update. \n");
    else
    {
        printf("wifi_cfg update now... \n");
        ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_ssid",MY_WIFI_SSID) );
        ESP_ERROR_CHECK( nvs_set_str(wificfg_nvs_handler,"wifi_passwd",MY_WIFI_PASSWD) ); 
        ESP_ERROR_CHECK( nvs_set_u32(wificfg_nvs_handler,"wifi_update",MY_WIFI_UPDATE) );
        printf("wifi_cfg update ok. \n");
    }
    ESP_ERROR_CHECK( nvs_commit(wificfg_nvs_handler) ); /* 提交 */
    nvs_close(wificfg_nvs_handler);                     /* 关闭 */

    printf("ESP_WIFI_MODE_STA \n");

    wifi_init_sta();
}



// static void storage_wifi_config(void)
// {
    
// }