#include "uart.h"

struct __FILE
{
	int dummy;
};
FILE __stdout;

int fputc(int _chr, FILE *f)
{
	usart_sendchar( _chr);
	return _chr;
}
void uart_Init(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart;
	// cau hinh clock
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_USART1 , ENABLE);
	// cau hinh TX - A9;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP ;
	gpio.GPIO_Pin = GPIO_Pin_9 ;
	gpio.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA , &gpio);
	// cau hinh RX - A10
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING ;
	gpio.GPIO_Pin = GPIO_Pin_10 ;
	gpio.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init (GPIOA , & gpio);
	// cau hinh usart 
	usart.USART_BaudRate = 9600;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx ;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_StopBits = USART_StopBits_1 ;
	usart.USART_WordLength = USART_WordLength_8b ;
	USART_Init (USART1 , &usart);
	// cau hinh ngat tong
	USART_ITConfig( USART1, USART_IT_RXNE , ENABLE);
	NVIC_EnableIRQ(USART1_IRQn );
	USART_Cmd ( USART1, ENABLE);
}

void usart_sendchar(char _chr)
{
	USART_SendData (USART1, _chr);
	while(USART_GetFlagStatus(USART1 ,USART_FLAG_TXE )== RESET);
}
void usart_sendstr(char *str)
{
	while(*str != NULL)
	{
		usart_sendchar( *str++ );
	}
}
