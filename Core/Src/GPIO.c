/*
 * GPIO.c
 *
 *  Created on: Dec 18, 2021
 *      Author: Ramadan
 */

#include "GPIO.h"

void GPIO_EnableClock(uint8 PortName)
{
	*RCC_AHB1ENR |= (0x01 << PortName);
}

void GPIO_Init(uint32 PortName, uint8 PinNum, uint8 PinMode,
		uint8 DefaultState)
{

  // Configure moder register for pin direction
	*GPIOx_MODER(PortName) &= ~(0x03 << 2 * PinNum);
	*GPIOx_MODER(PortName) |= PinMode << 2 * PinNum;

	*GPIOx_OTYPER(PortName) &= ~(1 << PinNum);
	*GPIOx_OTYPER(PortName) |= ((DefaultState & _OUTTYPE_MASK) >> _OUTTYPE_SHIFT) << PinNum;

	*GPIOx_PUPDR(PortName) &= ~(0x03 << 2 * PinNum);
	*GPIOx_PUPDR(PortName) |= ((DefaultState & _PUPD_MASK) >> _PUPD_SHIFT) << (2 * PinNum);
}

uint8 GPIO_WritePin(uint32 PortName, uint8 PinNum,
		uint8 Data)
{
	uint8 result;
  if (((*GPIOx_MODER(PortName) & (0x03 << 2 * PinNum)) >> (2 * PinNum)) == 1)
  {
    if (Data)
    {
      *GPIOx_ODR(PortName) |= (1 << PinNum);
    }
    else
    {
      *GPIOx_ODR(PortName) &= ~(1 << PinNum);
    }
    result = OK;
  }
  else
  {
    result = NOK;
  }

  return result;
}

uint8 GPIO_ReadPin(uint32 PortName, uint8 PinNum)
{
	uint8 data = 0;
	data = (*GPIOx_IDR(PortName) & (1 << PinNum)) >> PinNum;
	return data;
}

void GPIO_ToggleBit(uint32 PortName, uint8 PinNum)
{
	TOGGLEBIT(*GPIOx_ODR(PortName), PinNum);
}

void GPIO_AlternateConfig(uint32 PortName, uint8 PinNum)
{
	*GPIOx_AFRL(PortName)  |=(0x07<< PinNum * 4);
}
