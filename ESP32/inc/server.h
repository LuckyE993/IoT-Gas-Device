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

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 1024



extern char json_string[1024];
extern char ip_str[16];
extern char time_str[128];

extern EventGroupHandle_t xSendEventGroup;


// 定义事件位
#define TASK1_WIFI_CONNECTED BIT3
#define TASK2_SENSOR_COMPLETE BIT4


void init_json_data(void);
void init_send_json_data(char *json_string, int node_num, 
                    int O2, int CO, int H2S, int CH4, 
                    int temp_int,int temp_dec,int humi_int,int humi_dec,
                    char *ip, char *time);
void generate_and_send_json_data(void *pvParameters);
void send_json_data_init(void);