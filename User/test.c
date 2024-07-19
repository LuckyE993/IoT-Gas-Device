#include "test.h"
#include "./usart/bsp_debug_usart.h"
#include "stm32f4xx.h"
usart_data_typed test_data;


void do_process(usart_data_typed* udata)
{	
	//自定义对接收字符串处理，此处只作将接收返回
	Usart_SendString(USART1,udata->data);	
	
	
	
	//处理完一次数据记得重置标志
	udata->flag = 0;
	udata->len = 0;                  
}
