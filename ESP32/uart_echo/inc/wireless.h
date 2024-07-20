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
#define ECHO_TASK_STACK_SIZE    2048
#define BUF_SIZE                (1024)

void uart_init(void);


#endif /* _WIRELESS_H_ */
