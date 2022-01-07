/*
 * DMA_Registers.h
 *
 *  Created on: Jan 5, 2022
 *      Author: Ramadan
 */

#ifndef SRC_DMA_REGISTERS_H_
#define SRC_DMA_REGISTERS_H_

#include "StandardTypes.h"

typedef union
{
		vuint32 Reg;
		struct{
				//Bit Fields
				vuint32 EN:1;
				vuint32 DMEIE:1;
				vuint32 TEIE:1;
				vuint32 HTIE:1;
				vuint32 TCIE:1;
				vuint32 PFCTRL:1;
				vuint32 DIR:2;
				vuint32 CIRC:1;
				vuint32 PINC:1;
				vuint32 MINC:1;
				vuint32 PSIZE:2;
				vuint32 MSIZE:2;
				vuint32 PINCOS:1;
				vuint32 PL:2;
				vuint32 DBM:1;
				vuint32 CT:1;
				vuint32 B20:1;
				vuint32 PBURST:2;
				vuint32 MBURST:2;
				vuint32 CHSEL:3;
				vuint32 B31t28:4;
			}Bits;
}CR_Register;

typedef union
{
		vuint32 Reg;
		struct{
				//Bit Fields
				vuint32 NDT:16;
				vuint32 B31tB16:16;
			}Bits;
}NDTR_Register;

typedef union
{
		vuint32 Reg;
}PAR_Register;

typedef union
{
		uint32 Reg;
}M0AR_Register;

typedef union
{
		uint32 Reg;
}M1AR_Register;

typedef union
{
		vuint32 Reg;
		struct{
				//Bit Fields
				vuint32 FTH:2;
				vuint32 DMDIS:1;
				vuint32 FS:3;
				vuint32 B06:1;
				vuint32 FEIE:1;
				vuint32 B31tB08:24;
			}Bits;
}FCR_Register;

#endif /* SRC_DMA_REGISTERS_H_ */
