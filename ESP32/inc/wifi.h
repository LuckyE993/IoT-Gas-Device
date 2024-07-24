#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_smartconfig.h"
#include "esp_wpa2.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "server.h"
#include <string.h>

#define MY_WIFI_SSID      "luckye"

#define MY_WIFI_PASSWD      "11223344"
#define EXAMPLE_ESP_MAXIMUM_RETRY  5
#define MY_WIFI_UPDATE  4092                /* 对数值进行修改表示更新NVS的WiFi名称和密码*/

#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK

#define ESP_WIFI_SAE_MODE WPA3_SAE_PWE_BOTH
#define EXAMPLE_H2E_IDENTIFIER ""

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
#define SMART_CONFIG_BIT    BIT2

void wifi_init_sta(void);
void wifi_config_init(void);
