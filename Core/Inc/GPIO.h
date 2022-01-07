/*
 * GPIO.h
 *
 *  Created on: Dec 18, 2021
 *      Author: Ramadan
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "CLK.h"
#include "MACROS.h"

#define PORTA ((uint8)0x00)
#define PORTB ((uint8)0x01)

#define GPIOA (0x40020000)
#define GPIOB (0x40020400)

#define GPIOx_MODER(GPIOx) 		GET_REG(GPIOx, 0x00)
#define GPIOx_OTYPER(GPIOx) 	GET_REG(GPIOx, 0x04)
#define GPIOx_OSPEEDR(GPIOx) 	GET_REG(GPIOx, 0x08)
#define GPIOx_PUPDR(GPIOx) 		GET_REG(GPIOx, 0x0C)
#define GPIOx_IDR(GPIOx) 		GET_REG(GPIOx, 0x10)
#define GPIOx_ODR(GPIOx) 		GET_REG(GPIOx, 0x14)
#define GPIOx_BSRR(GPIOx) 		GET_REG(GPIOx, 0x18)
#define GPIOx_LCKR(GPIOx) 		GET_REG(GPIOx, 0x1C)
#define GPIOx_AFRL(GPIOx) 		GET_REG(GPIOx, 0x20)
#define GPIOx_AFRH(GPIOx) 		GET_REG(GPIOx, 0x24)

// Result Status
#define OK 				((uint8)0)
#define NOK 			((uint8)1)

#define HIGH 1
#define LOW 0

// Mode Types
#define INPUT 			((uint8)0x00)
#define OUTPUT 			((uint8)0x01)
#define ALTERNATE_FUN 	((uint8)0x02)
#define ANALOG 			((uint8)0x03)

// Output modes
#define PUSH_PULL 		((uint8)0x00)
#define OPEN_DRAIN 		((uint8)0x01)

// Resistor modes
#define NO_PULL_UP_DOWN ((uint8)0x00)
#define PULL_UP 		((uint8)0x02)
#define PULL_DOWN 		((uint8)0x04)

#define _OUTTYPE_MASK 	((uint8)0x01)
#define _OUTTYPE_SHIFT 0

#define _PUPD_MASK 		((uint8)0x06)
#define _PUPD_SHIFT 1

void GPIO_EnableClock(uint8 PortName);

void GPIO_Init(uint32 PortName, uint8 PinNum, uint8 PinDir,
               uint8 DefaultState);
uint8 GPIO_WritePin(uint32 Port, uint8 PinNum,
                            uint8 Data);

uint8 GPIO_ReadPin(uint32 PortName, uint8 PinNum);

void GPIO_ToggleBit(uint32 PortName, uint8 PinNum);

void GPIO_AlternateConfig(uint32 PortName, uint8 PinNum);

#endif /* INC_GPIO_H_ */
