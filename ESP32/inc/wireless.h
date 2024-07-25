#ifndef _WIRELESS_H_
#define _WIRELESS_H_
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define ECHO_TEST_TXD GPIO_NUM_4
#define ECHO_TEST_RXD GPIO_NUM_5
#define ECHO_TEST_RTS UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS UART_PIN_NO_CHANGE

#define ECHO_UART_PORT_NUM      UART_NUM_2
#define ECHO_UART_BAUD_RATE     115200
#define BUF_SIZE                (512)

// 模式位定义
#define MODE_INQUIRY    0x01
// 功能位定义
#define FUNCTION_DATA   0x01

// 定义应答帧结构体（STM32）
typedef struct {
    uint8_t header;             // 帧头，固定为0xFF
    uint16_t O2;                // O2浓度值
    uint16_t CO;                // CO浓度值
    uint16_t H2S;               // H2S浓度值
    uint16_t CH4;               // CH4浓度值
    uint8_t humidity_int;       // 湿度整数部分
    uint8_t humidity_dec;       // 湿度小数部分
    uint8_t temperature_int;    // 温度整数部分 
    uint8_t temperature_dec;    // 温度小数部分
    uint8_t tail;               // 帧尾，固定为0xFE
} ResponseFrame;

// 定义问询帧结构体（ESP32）
typedef struct {
    uint8_t header;   // 帧头，固定为0xFF
    uint8_t mode;     // 模式位
    uint8_t function; // 功能位
    uint8_t tail;     // 帧尾，固定为0xFE
} InquiryFrame;

extern ResponseFrame response_frame;

void uart_init(void);
void uart_cmd_send(uint8_t cmd);
void uart_receive_sensor_data(void);
void print_sensor_data(uint8_t *data,uint16_t len);
void parse_inquiry_frame(uint8_t *data,ResponseFrame *frame);
#endif /* _WIRELESS_H_ */
