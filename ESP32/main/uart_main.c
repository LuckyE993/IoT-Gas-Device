/* UART Echo Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "wireless.h"
/**
 * This is an example which echos any data it receives on configured UART back to the sender,
 * with hardware flow control turned off. It does not use UART driver event queue.
 *
 * - Port: configured UART
 * - Receive (Rx) buffer: on
 * - Transmit (Tx) buffer: off
 * - Flow control: off
 * - Event queue: off
 * - Pin assignment: see defines below (See Kconfig)
 */

#define ECHO_TEST_TXD GPIO_NUM_4
#define ECHO_TEST_RXD GPIO_NUM_5
#define ECHO_TEST_RTS UART_PIN_NO_CHANGE
#define ECHO_TEST_CTS UART_PIN_NO_CHANGE

#define ECHO_UART_PORT_NUM      UART_NUM_2
#define ECHO_UART_BAUD_RATE     115200
#define ECHO_TASK_STACK_SIZE    2048

static const char *TAG = "UART TEST";



static void echo_task(void *arg)
{
    uart_init();

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        
        // Read data from the UART
        // int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        // uart_flush(ECHO_UART_PORT_NUM);
        // Write data back to the UART
        uint16_t len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
        while(!len)
        {
            uart_cmd_send(MODE_INQUIRY);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        
        
        if (len == 14) 
        {
            if(data[0]==0xFF && data[13]==0xFE)
            {
               print_sensor_data(data,len);
               parse_inquiry_frame(data,&response_frame);
               printf("O2:%d CO:%d H2S:%d CH4:%d temperature:%u.%01u humidity:%u.%01u\n",response_frame.O2,response_frame.CO,response_frame.H2S,response_frame.CH4,
                                                                              response_frame.temperature_int,response_frame.temperature_dec,
                                                                              response_frame.humidity_int,response_frame.humidity_dec);
            }
            //uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, len);
            
        }
        else
        {
            ESP_LOGI(TAG, "Received Sensor Data is Not 14 Bytes, is %d", len);
        }

        uart_flush(ECHO_UART_PORT_NUM);             
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
}
