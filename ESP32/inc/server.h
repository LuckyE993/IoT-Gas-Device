#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "../inc/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <ctype.h>
#include "esp_netif.h"
#include "esp_tls.h"
#include "esp_http_client.h"
#include "esp_bit_defs.h"

#include <sys/time.h>
#include "esp_attr.h"
#include "esp_netif_sntp.h"
#include "lwip/ip_addr.h"
#include "esp_sntp.h"


#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 1024

#define USE_URL //def is use url, undef is use host and path 

#define HTTP_URL "http://192.168.239.51:5000/data"

extern char json_string[1024];
extern char ip_str[16];
extern char time_str[64];

extern EventGroupHandle_t xSendEventGroup;
#define IP6ADDR_STRLEN_MAX    46

// 定义事件位
#define TASK1_WIFI_CONNECTED BIT3
#define TASK2_SENSOR_COMPLETE BIT4
#define TASK3_TIME_SYNCED BIT5
#define SNTP_TIME_SERVER1 "ntp.aliyun.com"
#define SNTP_TIME_SERVER2 "ntp.ntsc.ac.cn"
#define SNTP_TIME_SERVER3 "ntp.tencent.com"
#define SNTP_TIME_SERVER4 "edu.ntp.org.cn"
#define SNTP_TIME_MAX_SERVERS    4



#define SNTP_TIME_ZONE "CST-8"
#define SNTP_SYNC_PERIOD 10 // minutes


void init_json_data(void);
void init_send_json_data(char *json_string, int node_num, 
                    int O2, int CO, int H2S, int CH4, 
                    int temp_int,int temp_dec,int humi_int,int humi_dec);
void generate_and_send_json_data(void *pvParameters);
void send_json_data_init(void);
void get_time(void);
void sync_time_task(void *pvParameter);
