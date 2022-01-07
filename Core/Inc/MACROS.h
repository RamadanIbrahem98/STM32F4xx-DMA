/*
 * MACROS.h
 *
 *  Created on: Dec 18, 2021
 *      Author: Ramadan
 */

#ifndef INC_MACROS_H_
#define INC_MACROS_H_

#include "StandardTypes.h"

#define GET_REG(REG, OFFSET) ((vuint32 *)(REG + OFFSET))

#define SETBIT(R,B) ((R) |= (1<<(B)))
#define SETNBITS(R,B,NB,Data) ((R) |= (Data<<(NB * B)))
#define CLEARBIT(R,B) ((R) &=~ (1<<(B)))
#define CLEARNBITS(R,B,NB,Data) ((R) &=~ (Data<<(NB * B)))
#define TOGGLEBIT(R,B) ((R) ^= (1<<(B)))
#define IS_BIT_SET(R,B) ((R) & (1<<(B)) >> (B))
#define IS_BIT_CLEAR(R,B) (!IS_BIT_SET(R,B))

#endif /* INC_MACROS_H_ */
