#include "server.h"
EventGroupHandle_t xSendEventGroup;


static const char *TAG_SERVER = "SERVER";
static const char *TAG = "HTTP_CLIENT";

char json_string[1024] = "";
char ip_str[16] = "not connected";  // 全局变量，用于保存IP地址字符串
char time_str[128] = "not connected";

// 创建事件组句柄


extern const char howsmyssl_com_root_cert_pem_start[] asm("_binary_howsmyssl_com_root_cert_pem_start");
extern const char howsmyssl_com_root_cert_pem_end[]   asm("_binary_howsmyssl_com_root_cert_pem_end");

extern const char postman_root_cert_pem_start[] asm("_binary_postman_root_cert_pem_start");
extern const char postman_root_cert_pem_end[]   asm("_binary_postman_root_cert_pem_end");

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}

static void http_rest_with_url(void)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};
    /**
     * NOTE: All the configuration parameters for http_client must be spefied either in URL or as host and path parameters.
     * If host and path parameters are not set, query parameter will be ignored. In such cases,
     * query parameter should be specified in URL.
     *
     * If URL as well as host and path parameters are specified, values of host and path will be considered.
     */
    esp_http_client_config_t config = {
        .host = "httpbin.org",
        .path = "/get",
        .query = "esp",
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_err_t err;

    // POST
    const char *post_data = "{\"field1\":\"value1\"}";
    esp_http_client_set_url(client, "http://httpbin.org/post");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRIu64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
    }
    char url[500];
    esp_http_client_get_url(client,url,500);
    printf("ADDRSS: %s %s", url, config.path);
    printf("POST: %s\r\n", post_data);
    esp_http_client_cleanup(client);
}

void init_json_data(void) {
    // 创建根JSON对象
    cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        printf("Error creating root JSON object\n");
        
    }

    // 添加 node_num 属性
    cJSON_AddNumberToObject(root, "node_num", 1);

    // 创建 data 对象
    cJSON *data = cJSON_CreateObject();
    if (data == NULL) {
        printf("Error creating data JSON object\n");
        cJSON_Delete(root);

    }

    // 添加各个传感器数据
    cJSON_AddNumberToObject(data, "O2", 21);
    cJSON_AddNumberToObject(data, "CO", 5);
    cJSON_AddNumberToObject(data, "H2S", 3);
    cJSON_AddNumberToObject(data, "CH4", 10);
    cJSON_AddNumberToObject(data, "temp", 25);
    cJSON_AddNumberToObject(data, "humi", 50);

    // 将 data 对象添加到根对象
    cJSON_AddItemToObject(root, "data", data);

    // 添加 time 和 IP 属性
    cJSON_AddStringToObject(root, "time", "2024-07-23T12:34:56");
    cJSON_AddStringToObject(root, "IP", "192.168.1.1");

    // 将JSON对象转换为字符串
    char *json_string = cJSON_Print(root);
    if (json_string == NULL) {
        printf("Error printing JSON string\n");
        cJSON_Delete(root);

    }

    // 打印生成的JSON字符串
    printf("%s\n", json_string);

    // 释放内存
    cJSON_Delete(root);
    free(json_string);

}

void init_send_json_data(char *json_string, int node_num, 
                    int O2, int CO, int H2S, int CH4, 
                    int temp_int,int temp_dec,int humi_int,int humi_dec,
                    char *ip, char *time)
{
     cJSON *root = cJSON_CreateObject();
    if (root == NULL) {
        printf("Error creating root JSON object\n");
        
    }

    // 添加 node_num 属性
    cJSON_AddNumberToObject(root, "node_num", node_num);

    // 创建 data 对象
    cJSON *data = cJSON_CreateObject();
    if (data == NULL) {
        printf("Error creating data JSON object\n");
        cJSON_Delete(root);

    }

    // 添加各个传感器数据
    cJSON_AddNumberToObject(data, "O2", O2);
    cJSON_AddNumberToObject(data, "CO", CO);
    cJSON_AddNumberToObject(data, "H2S", H2S);
    cJSON_AddNumberToObject(data, "CH4", CH4);
    cJSON_AddNumberToObject(data, "temp_int", temp_int);
    cJSON_AddNumberToObject(data, "temp_dec", temp_dec);
    cJSON_AddNumberToObject(data, "humi_int", humi_int);
    cJSON_AddNumberToObject(data, "humi_dec", humi_dec);

    // 将 data 对象添加到根对象
    cJSON_AddItemToObject(root, "data", data);

    // 添加 time 和 IP 属性
    cJSON_AddStringToObject(root, "time", time);
    cJSON_AddStringToObject(root, "IP", ip);

    // 将JSON对象转换为字符串
    char *json_temp = cJSON_Print(root);
    if (json_temp == NULL) {
        printf("Error printing JSON string\n");
        cJSON_Delete(root);
        return;
    }

    // 确保传入的缓冲区大小足够
    if (strlen(json_temp) < 1024) {
        strcpy(json_string, json_temp);
    } else {
        printf("Buffer size is too small\n");
    }

    // 释放内存
    cJSON_Delete(root);
    free(json_temp);
}

void generate_and_send_json_data(void *pvParameters)
{
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    esp_http_client_config_t config = {
        #ifdef USE_URL
        .url = "http://192.168.239.51:5000/data",
        #else  
        .host = "192.168.239.51",
        .port = 5000,
        .path = "/data",

        #endif
        
        .method = HTTP_METHOD_POST,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,        // Pass address of local buffer to get response
        .disable_auto_redirect = true,
    };
    
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err;

    // POST
    //esp_http_client_set_url(client, "192.168.239.51:5000");
    esp_http_client_set_method(client, HTTP_METHOD_POST);
    esp_http_client_set_header(client, "Content-Type", "application/json");


    // 等待所有任务完成
    EventBits_t uxBits;
    while (1) 
    {
        printf("server task is running\n");
      
        uxBits = xEventGroupWaitBits(
        xSendEventGroup,          // 事件组句柄
        TASK1_WIFI_CONNECTED |      // 等待的事件位
        TASK2_SENSOR_COMPLETE,       
        false,               // 清除事件位
        true,               // 等待所有事件位
        portMAX_DELAY         // 等待时间
        );

        if (uxBits&TASK1_WIFI_CONNECTED)
        {
            printf("TASK1_WIFI_CONNECTED\n");
        }
        if (uxBits&TASK2_SENSOR_COMPLETE)
        {
            printf("TASK2_SENSOR_COMPLETE\n");
        }
        

        if((uxBits & TASK1_WIFI_CONNECTED) && (uxBits & TASK2_SENSOR_COMPLETE)) // 所有事件位都已完成
        {
            xEventGroupClearBits(xSendEventGroup, TASK2_SENSOR_COMPLETE);
            //进行数据发送
            ESP_LOGI(TAG_SERVER, "Ready to send data");

            esp_http_client_set_post_field(client, json_string, strlen(json_string));
            err = esp_http_client_perform(client);
            if (err == ESP_OK)
            {
                ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %"PRIu64,
                            esp_http_client_get_status_code(client),
                            esp_http_client_get_content_length(client));
                printf("Send data successfully\n");
                printf(" Content: %s\n", local_response_buffer);
            }
            else
            {
                ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    esp_http_client_cleanup(client);
    vTaskDelete(NULL);
}

void send_json_data_init(void)
{
    xSendEventGroup = xEventGroupCreate();
    xTaskCreate(generate_and_send_json_data, "generate_and_send_json_data", 8192, NULL, 5, NULL);
}
