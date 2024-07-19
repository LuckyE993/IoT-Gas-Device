#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "test.h"


//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART1
#define DEBUG_USART_CLK                         RCC_APB2Periph_USART1
#define DEBUG_USART_BAUDRATE                    115200  //串口波特率

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_10
#define DEBUG_USART_RX_AF                       GPIO_AF_USART1
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource10

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_9
#define DEBUG_USART_TX_AF                       GPIO_AF_USART1
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource9

#define DEBUG_USART_IRQHandler                  USART1_IRQHandler
#define DEBUG_USART_IRQ                 				USART1_IRQn
/*******************************************************/
#define GAS_USART                             USART3
#define GAS_USART_CLK                         RCC_APB1Periph_USART3
#define GAS_USART_BAUDRATE                    9600  //串口波特率

#define GAS_USART_RX_GPIO_PORT                GPIOB
#define GAS_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define GAS_USART_RX_PIN                      GPIO_Pin_11
#define GAS_USART_RX_AF                       GPIO_AF_USART3
#define GAS_USART_RX_SOURCE                   GPIO_PinSource11

#define GAS_USART_TX_GPIO_PORT                GPIOB
#define GAS_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define GAS_USART_TX_PIN                      GPIO_Pin_10
#define GAS_USART_TX_AF                       GPIO_AF_USART3
#define GAS_USART_TX_SOURCE                   GPIO_PinSource10

#define GAS_USART_IRQHandler                  USART3_IRQHandler
#define GAS_USART_IRQ                 				USART3_IRQn


/************************************************************/
#define data_size 256

typedef struct
{
	uint8_t data[data_size];
	uint16_t len;
	uint8_t flag;
	
}USART_RX_DATA;

extern USART_RX_DATA usart1_rx_data;
extern USART_RX_DATA usart3_rx_data;

void Debug_USART_Config(void);
void GAS_USART_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);



#endif /* __USART1_H */
