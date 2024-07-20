/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   LTDC-LCD触摸画板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32  F429开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
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
  * @brief  主函数
  * @param  无
  * @retval 无
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
//	/* 初始化触摸屏 */
//  GTP_Init_Panel(); 
//  
//  printf("\r\n提示：\r\n"
//         "如果是4.3寸/5寸的RGB屏幕，使用的触摸芯片可能是GT1151QM。\r\n"
//         "如果触摸或显示异常，请确认使用的屏幕触摸芯片型号，"
//         "在“bsp_i2c_touch.h”文件修改宏定义 LCD_TOUCH_IC_GT1151QM。\r\n\r\n");
//  
//	/*初始化液晶屏*/
//  LCD_Init();
	//init display data
		initialize_display_data(&screen_display_data);
//  LCD_LayerInit();
//  LTDC_Cmd(ENABLE);
//	
//	/*把背景层刷黑色*/
//  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
//	LCD_Clear(LCD_COLOR_BLACK);
//	
//  /*初始化后默认使用前景层*/
//	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
//	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
//  LCD_SetTransparency(0xFF);
//	LCD_Clear(LCD_COLOR_BLACK);
//	
//	/*调用画板函数*/
//	Palette_Init();





		printf("------------------Wait for sensors--------------------\n");
		Delay_ms(1000);//必要延时，等待传感器上电。
		get_humi_temp_info();
		printf("humi: %04X ％RH, temp: %04X℃ \r\n",generated_dht11_get_data.humi,generated_dht11_get_data.temp);
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

