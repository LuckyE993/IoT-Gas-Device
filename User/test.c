#include "test.h"
#include "./usart/bsp_debug_usart.h"
#include "stm32f4xx.h"
usart_data_typed test_data;


void do_process(usart_data_typed* udata)
{	
	//�Զ���Խ����ַ��������˴�ֻ�������շ���
	Usart_SendString(USART1,udata->data);	
	
	
	
	//������һ�����ݼǵ����ñ�־
	udata->flag = 0;
	udata->len = 0;                  
}
