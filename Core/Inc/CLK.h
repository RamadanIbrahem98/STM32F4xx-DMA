/*
 * CLK.h
 *
 *  Created on: Dec 25, 2021
 *      Author: Ramadan
 */

#ifndef INC_CLK_H_
#define INC_CLK_H_

#include "MACROS.h"

#define RCC (0x40023800)

#define RCC_AHB1ENR GET_REG(RCC, (0x30))
#define RCC_APB1ENR GET_REG(RCC, (0x40))
#define RCC_APB2ENR GET_REG(RCC, (0x44))


#endif /* INC_CLK_H_ */
