#include "wireless.h"
#include <esp_log.h>
static const char *TAG = "UART TEST";

ResponseFrame response_frame;

void uart_init(void)
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));
}

void uart_cmd_send(uint8_t cmd)
{
    uint8_t cmd_data[4];
    InquiryFrame inquiry_frame;

    inquiry_frame.header = 0xFF;
    inquiry_frame.mode = MODE_INQUIRY;
    inquiry_frame.function = FUNCTION_DATA;
    inquiry_frame.tail = 0xFE;

    cmd_data[0] = inquiry_frame.header;
    cmd_data[1] = inquiry_frame.mode;
    cmd_data[2] = inquiry_frame.function;
    cmd_data[3] = inquiry_frame.tail;

    uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) cmd_data, 4);

}

void print_sensor_data(uint8_t *data,uint16_t len)
{
    int count;
    for (count = 0; count < len; count++) {
        printf("%.2X ", data[count]);
    }

    printf("\n");

}

void parse_inquiry_frame(uint8_t *data,ResponseFrame *frame)
{
    frame->header = data[0];
    frame->O2 = (data[1] << 8) | data[2];
    frame->CO = (data[3] << 8) | data[4];
    frame->H2S = (data[5] << 8) | data[6];
    frame->CH4 = (data[7] << 8) | data[8];
    frame->temperature_int = data[9];
    frame->temperature_dec = data[10];
    frame->humidity_int = data[11];
    frame->humidity_dec = data[12];
    frame->tail = data[13];
    
}
