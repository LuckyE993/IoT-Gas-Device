#ifndef __GAS_H_
#define __GAS_H_

#include "stdint.h"
#include "./usart/bsp_debug_usart.h"
#include "stm32f4xx.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"

#define GET_ALL 4
#define GET_O2 0x0000
#define GET_CO 0x0001
#define GET_H2S 0x0002
#define GET_CH4 0x0003

#define ASK_ADDR 0x01
#define ASK_FUNC 0x03
#define ASK_DATALEN 0x01

//#define DEBUG_GAS //ʹ�ô���1��ӡ�����Ϣ

// ������ѯ֡�ṹ��
typedef struct
{
    uint8_t address;       // ��ַ�룬1�ֽ�
    uint8_t function_code; // �����룬1�ֽ�
    uint16_t start_addr;   // �Ĵ�����ʼ��ַ��2�ֽ�
    uint16_t reg_length;   // �Ĵ������ȣ�2�ֽ�
    uint8_t crc_low;       // У�����λ��1�ֽ�
    uint8_t crc_high;      // У�����λ��1�ֽ�
} gas_data_tx;

// ����Ӧ��֡�ṹ��
typedef struct
{
    uint8_t address;       // ��ַ�룬1�ֽ�
    uint8_t function_code; // �����룬1�ֽ�
    uint8_t byte_count;    // ��Ч�ֽ�����1�ֽ�
    uint16_t data[1];      // ��������ʵ�ʴ�С���� byte_count ȷ����ÿ�������� 2�ֽ�
    uint8_t crc_low;       // У�����λ��1�ֽ�
    uint8_t crc_high;      // У�����λ��1�ֽ�
} gas_data_rx;

typedef struct
{
    uint16_t O2;
    uint16_t CO;
    uint16_t H2S;
    uint16_t CH4;
}gas_data_get;



extern gas_data_tx generated_gas_data_tx;
extern gas_data_rx generated_gas_data_rx;
extern gas_data_get generated_gas_data_get;

uint16_t calculate_crc(uint8_t *data, uint16_t length);
void init_gas_data_tx(gas_data_tx *frame,uint16_t func);
void parse_gas_data_rx(uint8_t *buffer, uint16_t length,uint16_t  func);
void send_gas_data_tx(USART_TypeDef *pUSARTx, gas_data_tx *frame);
void get_all_gas_info(void);

#endif /*__GAS_H_*/
