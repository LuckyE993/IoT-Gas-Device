/**
  ******************************************************************************
  * @file    bsp_debug_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart/bsp_debug_usart.h"

USART_RX_DATA usart3_rx_data = { .len = 0, .flag = 0 };
USART_RX_DATA usart1_rx_data = { .len = 0, .flag = 0 };
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
	/* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = GAS_USART_IRQ;
  /* 抢断优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
}


 /**
  * @brief  DEBUG_USART GPIO 配置,工作模式配置。115200 8-N-1 ，中断接收模式
  * @param  无
  * @retval 无
  */
void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
  /* 嵌套向量中断控制器NVIC配置 */
	NVIC_Configuration();
  
	/* 使能串口接收中断 */
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(DEBUG_USART, USART_IT_IDLE, ENABLE);	
	
  /* 使能串口 */
  USART_Cmd(DEBUG_USART, ENABLE);
}

void GAS_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(GAS_USART_RX_GPIO_CLK|GAS_USART_TX_GPIO_CLK,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(GAS_USART_CLK, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GAS_USART_TX_PIN  ;  
  GPIO_Init(GAS_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GAS_USART_RX_PIN;
  GPIO_Init(GAS_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GAS_USART_RX_GPIO_PORT,GAS_USART_RX_SOURCE,GAS_USART_RX_AF);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GAS_USART_TX_GPIO_PORT,GAS_USART_TX_SOURCE,GAS_USART_TX_AF);
  
  /* 配置串GAS_USART 模式 */
  /* 波特率设置：GAS_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = GAS_USART_BAUDRATE;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(GAS_USART, &USART_InitStructure); 
	
//  /* 嵌套向量中断控制器NVIC配置 */
//	NVIC_Configuration();
  
	/* 使能串口接收中断 */
	USART_ITConfig(GAS_USART, USART_IT_RXNE, ENABLE);
	/* 使能串口空闲中断 */
	USART_ITConfig(GAS_USART, USART_IT_IDLE, ENABLE);	
	
  /* 使能串口 */
  USART_Cmd(GAS_USART, ENABLE);

}
	

/*****************  发送一个字符 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}

/************串口接收函数**************/

void DEBUG_USART_IRQHandler(void)
	
{
//	Usart_SendString(USART1,"USART1 Receive Success");
	if(USART_GetITStatus(DEBUG_USART,USART_IT_RXNE)!=RESET)
	{		
		usart1_rx_data.data[usart1_rx_data.len] = USART_ReceiveData(DEBUG_USART);
	
		if(++usart1_rx_data.len > data_size - 2) //让第 data_size - 1 位置一直是空终止位
		{
			usart1_rx_data.len = 0;                //演示从简处理，如果接收超过尺寸从头开始覆盖
			
		}  
	}	 
	

	if(USART_GetITStatus(DEBUG_USART,USART_IT_IDLE)!=RESET)
	{	

    USART_ReceiveData(DEBUG_USART);	        //查阅参考手册 软件序列清除标志位流程
		usart1_rx_data.data[usart1_rx_data.len] = '\0';     //以空闲中断认为接收完成 设置空终止位
		usart1_rx_data.flag = 1;			                  //赋值标志作2为程序判断是否完成
	}	 
}	

void GAS_USART_IRQHandler(void)
{
	if(USART_GetITStatus(GAS_USART,USART_IT_RXNE)!=RESET)
	{		
		usart3_rx_data.data[usart3_rx_data.len] = USART_ReceiveData(GAS_USART);
	
		if(++usart3_rx_data.len > data_size - 2) //让第 data_size - 1 位置一直是空终止位
		{
			usart3_rx_data.len = 0;                //演示从简处理，如果接收超过尺寸从头开始覆盖
			
		}	  
	}	 
	

	if(USART_GetITStatus(GAS_USART,USART_IT_IDLE)!=RESET)
	{	

    USART_ReceiveData(GAS_USART);	        //查阅参考手册 软件序列清除标志位流程
		usart3_rx_data.data[usart3_rx_data.len] = '\0';     //以空闲中断认为接收完成 设置空终止位
		usart3_rx_data.flag = 1;			                  //赋值标志作2为程序判断是否完成
	}	 
}	











/*********************************************END OF FILE**********************/
