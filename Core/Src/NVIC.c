/*
 * NVIC.c
 *
 *  Created on: Jan 6, 2022
 *      Author: Ramadan
 */

#include "NVIC.h"

void Enable_NVIC(uint32 reg, uint32 bit)
{
	SETBIT(*NVIC_ISERx(reg), bit);
}
