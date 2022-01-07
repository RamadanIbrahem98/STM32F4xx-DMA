/*
 * UART.h
 *
 *  Created on: Dec 30, 2021
 *      Author: Ramadan
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "MACROS.h"
#include "CLK.h"
#include "StandardTypes.h"

#define USART6 (0x40011400)
#define USART1 (0x40011000)
#define USART2 (0x40004400)

#define USARTx_SR(BASE) 	GET_REG(BASE, 0x00)
#define USARTx_DR(BASE) 	GET_REG(BASE, 0x04)
#define USARTx_BRR(BASE) 	GET_REG(BASE, 0x08)
#define USARTx_CR1(BASE) 	GET_REG(BASE, 0x0C)
#define USARTx_CR2(BASE) 	GET_REG(BASE, 0x10)


void UART_EnableClock(void);
void UART_Init(uint32 USART);
void UART_Transmit(uint32 USART, const uint8* str);

#endif /* INC_UART_H_ */
