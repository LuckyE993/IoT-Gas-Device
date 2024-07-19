#ifndef __DHT11_H_
#define __DHT11_H_

#include  "stm32f4xx.h"
#include "./systick/bsp_SysTick.h"


#define DHT11_HIGH  1
#define DHT11_LOW   0


/*---------------------------------------*/
#define DHT11_CLK     RCC_AHB1Periph_GPIOB
#define DHT11_PIN     GPIO_Pin_3     
#define DHT11_PORT    GPIOB


#define DHT11_DELAY_US(us)  Delay_us(us)
#define DHT11_DELAY_MS(ms)  Delay_ms(ms)

#define DHT11_DATA_OUT(a)	if (a)	\
                                   GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
                                   else		\
                                   GPIO_ResetBits(DHT11_PORT,DHT11_PIN)

#define  DHT11_DATA_IN()	  GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
}DHT11_Data_TypeDef;

typedef struct
{
	uint16_t  humi;		//湿度的整数部分
	uint16_t  temp;	 	//湿度的小数部分
}DHT11_HumiTemp_TypeDef;


void DHT11_GPIO_Config(void);

extern DHT11_Data_TypeDef dht11_get_data;
extern DHT11_HumiTemp_TypeDef generated_dht11_get_data;

uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
void get_humi_temp_info(void);
#endif //__DHT11_H_
