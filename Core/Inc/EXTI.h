/*
 * EXTI.h
 *
 *  Created on: Dec 25, 2021
 *      Author: Ramadan
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

#include "CLK.h"
#include "MACROS.h"
#include "StandardTypes.h"

#define RISING 	0x00
#define FALLING 0x01
#define BOTH	0x02

#define SYSCFG (0x40013800)
#define SYSCFG_EXTICRx(x) GET_REG(SYSCFG, (0x08 + (x * 0x04))) // SYSCFG_EXTICR1 &= (~(0x457 << 4 * 0)) for port A Interrupt 0

#define EXTI (0x40013C00)

#define EXTI_IMR 	GET_REG(EXTI, (0x00))
#define EXTI_RTSR 	GET_REG(EXTI, (0x08))
#define EXTI_FTSR 	GET_REG(EXTI, (0x0C))
#define EXTI_PR 	GET_REG(EXTI, (0x14))

#define NVIC_ISER		(0xE000E100)	// 1 means Interrupt Enabled OFFSET = 0x04 * x (x ranges [0, 7])
#define NVIC_ICER		(0XE000E180)
#define NVIC_ISERx(x) 	GET_REG(NVIC_ISER, (0x04*x))
#define NVIC_ICERx(x) 	GET_REG(NVIC_ICER, (0x04*x))

void Enable_ENT0(uint8 state);
void ENT0_ISR(void);

#endif /* INC_EXTI_H_ */
