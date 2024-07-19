#include "wireless_ctl.h"

InquiryFrame inquiry_frame;
ResponseFrame response_frame;


void get_wireless_message(void)
{
	if(usart1_rx_data.flag==1)
    {
				if(parse_inquiry_frame(usart1_rx_data.data,&inquiry_frame))
				{
					switch(inquiry_frame.mode)
					{
						case WIRELESS_MODE_GET_INFO:
							generate_response_frame(&response_frame, generated_gas_data_get.O2, generated_gas_data_get.CO, 
																											generated_gas_data_get.H2S, generated_gas_data_get.CH4, generated_dht11_get_data.temp, generated_dht11_get_data.humi);
							send_response_frame(DEBUG_USART, &response_frame);
						break;
						default:
							break;
					}
				
				
				}
        usart1_rx_data.len = 0;
        usart1_rx_data.flag = 0;
    }
}

int parse_inquiry_frame(uint8_t *buffer, InquiryFrame *frame) 
	{
    if (buffer[0] == FRAME_HEADER && buffer[3] == FRAME_TAIL) {
			
        frame->header = buffer[0];
        frame->mode = buffer[1];
        frame->function = buffer[2];
        frame->tail = buffer[3];
				LED_Blink(LED_NUM_3,20);
        return 1; // 解析成功
    }
    return 0; // 解析失败
}

// 发送应答帧函数
void send_response_frame(USART_TypeDef *pUSARTx, ResponseFrame *frame) 
	{
    uint8_t buffer[14];
    buffer[0] = frame->header;
    buffer[1] = (frame->O2 >> 8) & 0xFF;
    buffer[2] = frame->O2 & 0xFF;
    buffer[3] = (frame->CO >> 8) & 0xFF;
    buffer[4] = frame->CO & 0xFF;
    buffer[5] = (frame->H2S >> 8) & 0xFF;
    buffer[6] = frame->H2S & 0xFF;
    buffer[7] = (frame->CH4 >> 8) & 0xFF;
    buffer[8] = frame->CH4 & 0xFF;
    buffer[9] = (frame->temperature >> 8) & 0xFF;
    buffer[10] = frame->temperature & 0xFF;
    buffer[11] = (frame->humidity >> 8) & 0xFF;
    buffer[12] = frame->humidity & 0xFF;
    buffer[13] = frame->tail;

    for (int i = 0; i < sizeof(buffer); i++) 
		{
        USART_SendData(pUSARTx, buffer[i]);
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
    }

    // 等待发送完成
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);
}

// 生成应答帧
void generate_response_frame(ResponseFrame *frame, uint16_t O2, uint16_t CO, uint16_t H2S, uint16_t CH4, uint16_t temperature, uint16_t humidity) {
    frame->header = FRAME_HEADER;
    frame->O2 = O2;
    frame->CO = CO;
    frame->H2S = H2S;
    frame->CH4 = CH4;
    frame->temperature = temperature;
    frame->humidity = humidity;
    frame->tail = FRAME_TAIL;
}
