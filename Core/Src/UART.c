/*
 * UART.c
 *
 *  Created on: Dec 30, 2021
 *      Author: Ramadan
 */

#include "UART.h"

void UART_EnableClock(void)
{
	SETBIT(*RCC_APB1ENR, 17);
}

void UART_Init(uint32 USART)
{
//	Enable USART
	SETBIT(*USARTx_CR1(USART), 13);
//	SET Baudrate to 9600
	*USARTx_BRR(USART) = 0x683;
// 	Enable Transmitter
	SETBIT(*USARTx_CR1(USART), 3);
// 	Enable Transmitter
	CLEARBIT(*USARTx_CR1(USART), 2);
//	SET BUFFER Size to 8 bit
	CLEARBIT(*USARTx_CR1(USART), 12);
//	SET Over-sampling to 16 bit
	CLEARBIT(*USARTx_CR1(USART), 15);
//	Set one Stop Bit
	*USARTx_CR2(USART) &= ~(0x03 << 12);
}

void UART_Transmit(uint32 USART, const uint8* str)
{
	while(*str != '\0')
	{
		*USARTx_DR(USART) = *str;
		while (!((*USARTx_SR(USART) >> 6) & 1));
		str++;
	}
}
