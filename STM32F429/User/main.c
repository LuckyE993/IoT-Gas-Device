/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC-LCD��������
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32  F429������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./sdram/bsp_sdram.h"
#include "./lcd/bsp_lcd.h"
#include "./touch/bsp_touch_gtxx.h"
#include "./touch/palette.h"
#include "./gas/gas.h"
#include "./systick/bsp_SysTick.h"
#include "./wireless/wireless_ctl.h"
#include "./dht11/dht11.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	
	SysTick_Init();
	LED_GPIO_Config();	
  Debug_USART_Config();
	GAS_USART_Config();
	DHT11_GPIO_Config();
	printf("------------------Init Success--------------------\n");
	
	
////	LED_BLUE;
//	/* ��ʼ�������� */
//  GTP_Init_Panel(); 
//  
//  printf("\r\n��ʾ��\r\n"
//         "�����4.3��/5���RGB��Ļ��ʹ�õĴ���оƬ������GT1151QM��\r\n"
//         "�����������ʾ�쳣����ȷ��ʹ�õ���Ļ����оƬ�ͺţ�"
//         "�ڡ�bsp_i2c_touch.h���ļ��޸ĺ궨�� LCD_TOUCH_IC_GT1151QM��\r\n\r\n");
//  
//	/*��ʼ��Һ����*/
//  LCD_Init();
	//init display data
		initialize_display_data(&screen_display_data);
//  LCD_LayerInit();
//  LTDC_Cmd(ENABLE);
//	
//	/*�ѱ�����ˢ��ɫ*/
//  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
//	LCD_Clear(LCD_COLOR_BLACK);
//	
//  /*��ʼ����Ĭ��ʹ��ǰ����*/
//	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
//	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
//  LCD_SetTransparency(0xFF);
//	LCD_Clear(LCD_COLOR_BLACK);
//	
//	/*���û��庯��*/
//	Palette_Init();





		printf("------------------Wait for sensors--------------------\n");
		Delay_ms(1000);//��Ҫ��ʱ���ȴ��������ϵ硣
		get_humi_temp_info();
		printf("humi: %04X ��RH, temp: %04X�� \r\n",generated_dht11_get_data.humi,generated_dht11_get_data.temp);
		get_all_gas_info();
		printf("GAS O2 %04X\nGAS CO %04X\nGAS H2S %04X\nGAS CH4 %04X\n",generated_gas_data_get.O2,generated_gas_data_get.CO,
																																		generated_gas_data_get.H2S,generated_gas_data_get.CH4);
		printf("------------------Boot Success--------------------\n");
		set_display_data(&screen_display_data,generated_gas_data_get.O2,generated_gas_data_get.CO,
																					generated_gas_data_get.H2S,generated_gas_data_get.CH4,
																					generated_dht11_get_data.humi,generated_dht11_get_data.temp);
		
  while(1)
	{
		LED1_TOGGLE;
		get_all_gas_info();
		get_humi_temp_info();
		set_display_data(&screen_display_data,generated_gas_data_get.O2,generated_gas_data_get.CO,
																					generated_gas_data_get.H2S,generated_gas_data_get.CH4,
																					generated_dht11_get_data.humi,generated_dht11_get_data.temp);
		get_wireless_message();


		Delay_ms(200);
		
		
		
		
	}


}




/*********************************************END OF FILE**********************/

