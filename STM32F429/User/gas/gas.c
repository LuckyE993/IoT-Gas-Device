#include "gas.h"
#include <string.h>
// CRC16 校验函数 (示例实现, 实际需要根据具体协议实现)

gas_data_tx generated_gas_data_tx;
gas_data_rx generated_gas_data_rx;
gas_data_get generated_gas_data_get;

uint16_t calculate_crc(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF;
    for (uint16_t pos = 0; pos < length; pos++) {
        crc ^= (uint16_t)data[pos]; // XOR byte into least sig. byte of crc
        for (uint8_t i = 8; i != 0; i--) { // Loop over each bit
            if ((crc & 0x0001) != 0) { // If the LSB is set
                crc >>= 1; // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else {
                crc >>= 1; // Just shift right
            }
        }
    }
    return crc;
}
// 初始化问询帧

/**
  * @brief  Init Gas ask frame
  * @param  frame: communicate with gas sensor
  * @param  GAS_FUNC: GET_O2 or GET_CO or GET_H2S or GET_CH4
  * @retval None
  */
void init_gas_data_tx(gas_data_tx *frame,uint16_t GAS_FUNC)
{
    frame->address = ASK_ADDR;
    frame->function_code = ASK_FUNC;
    frame->start_addr = GAS_FUNC;
    frame->reg_length = ASK_DATALEN;
    uint8_t data[] = {frame->address, frame->function_code, frame->start_addr >> 8, frame->start_addr & 0xFF, frame->reg_length >> 8, frame->reg_length & 0xFF};
    uint16_t crc = calculate_crc(data, sizeof(data));
    frame->crc_low = crc & 0xFF;
    frame->crc_high = crc >> 8;
}

// 发送缓冲区数据
void Usart_SendBuffer(USART_TypeDef *pUSARTx, uint8_t *buffer, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        USART_SendData(pUSARTx, buffer[i]);
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
    }

    /* 等待发送完成 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

// 发送问询帧
void send_gas_data_tx(USART_TypeDef *pUSARTx, gas_data_tx *frame)
{
    uint8_t buffer[8];
    buffer[0] = frame->address;
    buffer[1] = frame->function_code;
    buffer[2] = (frame->start_addr >> 8) & 0xFF;
    buffer[3] = frame->start_addr & 0xFF;
    buffer[4] = (frame->reg_length >> 8) & 0xFF;
    buffer[5] = frame->reg_length & 0xFF;
    buffer[6] = frame->crc_low;
    buffer[7] = frame->crc_high;

    Usart_SendBuffer(pUSARTx, buffer, sizeof(buffer));
}

// 解析应答帧
void parse_gas_data_rx(uint8_t *buffer, uint16_t length,uint16_t  func)
{
//		for (int i = 0; i < 7; i++)
//			{
//					printf("Data[%d]: %04X   ", i, buffer[i]);

//			}
		gas_data_rx frame;
    frame.address = buffer[0];
    frame.function_code = buffer[1];
    frame.byte_count = buffer[2];
//	printf("frame.byte_count:%d \n",frame.byte_count);
    if (length < 5) // 最小帧长度
    {
			# ifdef DEBUG_GAS
				printf("Frame too short\n");
			# endif
        
        return;
    }
		
    if (length < (3 + frame.byte_count + 2)) // 确保缓冲区包含所有数据和 CRC
    {
        # ifdef DEBUG_GAS
					printf("Incomplete frame\n");
				# endif
				
        return;
    }

    for (int i = 0; i < frame.byte_count / 2; i++)
    {
        frame.data[i] = (buffer[3 + i * 2] << 8) | buffer[4 + i * 2];
    }

    frame.crc_low = buffer[length - 2];
    frame.crc_high = buffer[length - 1];

    uint16_t crc_calculated = calculate_crc(buffer, length - 2);
    uint16_t crc_received = (frame.crc_high << 8) | frame.crc_low;

    if (crc_received == crc_calculated)
    {
        
				switch(func)
				{
					case GET_O2:
						generated_gas_data_get.O2 = frame.data[0];
					break;
					case GET_CO:
						generated_gas_data_get.CO = frame.data[0];
					break;
					case GET_H2S:
						generated_gas_data_get.H2S = frame.data[0];
					break;
					case GET_CH4:
						generated_gas_data_get.CH4 = frame.data[0];
					break;
					
					
				
				}
				LED_Blink(LED_NUM_2,1);
				# ifdef DEBUG_GAS
					printf("%d CRC check passed\n",func);
				# endif
    }
    else
    {
				# ifdef DEBUG_GAS
					printf("CRC check failed\n");
				# endif
        
    }

}

void get_all_gas_info(void)
{
		init_gas_data_tx(&generated_gas_data_tx,GET_O2);
		send_gas_data_tx(GAS_USART,&generated_gas_data_tx);
    Delay_ms(30);
    if(usart3_rx_data.flag==1)
    {
        parse_gas_data_rx(usart3_rx_data.data, usart3_rx_data.len,GET_O2);
        usart3_rx_data.len = 0;
        usart3_rx_data.flag = 0;
    }
		
		
    init_gas_data_tx(&generated_gas_data_tx,GET_CO);
    send_gas_data_tx(GAS_USART,&generated_gas_data_tx);
		Delay_ms(30);
    if(usart3_rx_data.flag==1)
    {
        parse_gas_data_rx(usart3_rx_data.data, usart3_rx_data.len,GET_CO);
        usart3_rx_data.len = 0;
        usart3_rx_data.flag = 0;
    }
		
		
    init_gas_data_tx(&generated_gas_data_tx,GET_H2S);
    send_gas_data_tx(GAS_USART,&generated_gas_data_tx);
		Delay_ms(30);
    if(usart3_rx_data.flag==1)
    {
        parse_gas_data_rx(usart3_rx_data.data, usart3_rx_data.len,GET_H2S);
        usart3_rx_data.len = 0;
        usart3_rx_data.flag = 0;
    }
		
		
    init_gas_data_tx(&generated_gas_data_tx,GET_CH4);
    send_gas_data_tx(GAS_USART,&generated_gas_data_tx);
		Delay_ms(30);
    if(usart3_rx_data.flag==1)
    {
        parse_gas_data_rx(usart3_rx_data.data, usart3_rx_data.len,GET_CH4);
        usart3_rx_data.len = 0;
        usart3_rx_data.flag = 0;
    }
}








