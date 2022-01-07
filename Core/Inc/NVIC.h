/*
 * NVIC.h
 *
 *  Created on: Jan 6, 2022
 *      Author: Ramadan
 */

#ifndef INC_NVIC_H_
#define INC_NVIC_H_

#include "MACROS.h"
#include "StandardTypes.h"

#define NVIC_ISER		(0xE000E100)	// 1 means Interrupt Enabled OFFSET = 0x04 * x (x ranges [0, 7])
#define NVIC_ICER		(0XE000E180)
#define NVIC_ISERx(x) 	GET_REG(NVIC_ISER, (0x04*x))
#define NVIC_ICERx(x) 	GET_REG(NVIC_ICER, (0x04*x))

void Enable_NVIC(uint32 reg, uint32 bit);

#endif /* INC_NVIC_H_ */
