/*
 * EXTI.c
 *
 *  Created on: Dec 25, 2021
 *      Author: Ramadan
 */


#include "EXTI.h"

void Enable_ENT0(uint8 state)
{
	// 0 - Enable NVIC
	SETBIT(*NVIC_ISERx(0), 6);
	//	1 - Enable Clock
	SETBIT(*RCC_APB2ENR, 14);
	//	2 - Bit Selection
	CLEARBIT(*SYSCFG_EXTICRx(0), 0);
	CLEARBIT(*SYSCFG_EXTICRx(0), 1);
	CLEARBIT(*SYSCFG_EXTICRx(0), 2);
	CLEARBIT(*SYSCFG_EXTICRx(0), 3);
	// 3 - Demasking
	SETBIT(*EXTI_IMR,0);
	// 4 - Rising, falling, or Both
	switch(state)
	{
	case RISING:
		SETBIT(*EXTI_RTSR, 0);
		break;
	case FALLING:
		SETBIT(*EXTI_FTSR, 0);
		break;
	case BOTH:
		SETBIT(*EXTI_RTSR, 0);
		SETBIT(*EXTI_FTSR, 0);
		break;
	}
}
