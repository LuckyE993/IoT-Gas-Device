#ifndef __TEST_H
#define	__TEST_H

#define data_size2 100

typedef struct
{
	unsigned int flag;
	unsigned int len;
	char data[data_size2];

}usart_data_typed;



extern usart_data_typed test_data;


void do_process(usart_data_typed* udata);	



#endif
