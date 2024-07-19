#ifndef __WIRELESS_CTL__
#define __WIRELESS_CTL__
#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./gas/gas.h"
#include "./systick/bsp_SysTick.h"
#include "./dht11/dht11.h"
#include <stdint.h>

#define FRAME_HEADER 0xFF
#define FRAME_TAIL 0xFE

#define WIRELESS_MODE_GET_INFO 0x01


// ����Ӧ��֡�ṹ�壨STM32��
typedef struct {
    uint8_t header;    // ֡ͷ���̶�Ϊ0xFF
    uint16_t O2;       // O2Ũ��ֵ
    uint16_t CO;       // COŨ��ֵ
    uint16_t H2S;      // H2SŨ��ֵ
    uint16_t CH4;      // CH4Ũ��ֵ
    uint16_t temperature; // �¶�
    uint16_t humidity;    // ʪ��
    uint8_t tail;      // ֡β���̶�Ϊ0xFE
} ResponseFrame;

// ������ѯ֡�ṹ�壨ESP32��
typedef struct {
    uint8_t header;   // ֡ͷ���̶�Ϊ0xFF
    uint8_t mode;     // ģʽλ
    uint8_t function; // ����λ
    uint8_t tail;     // ֡β���̶�Ϊ0xFE
} InquiryFrame;

extern InquiryFrame inquiry_frame;
extern ResponseFrame response_frame;

void get_wireless_message(void);
int parse_inquiry_frame(uint8_t *buffer, InquiryFrame *frame);
void send_response_frame(USART_TypeDef *pUSARTx, ResponseFrame *frame) ;
void generate_response_frame(ResponseFrame *frame, uint16_t O2, uint16_t CO, uint16_t H2S, uint16_t CH4, uint16_t temperature, uint16_t humidity);

#endif /*__WIRELESS_CTL__*/
