/*
 * DMA.c
 *
 *  Created on: Jan 2, 2022
 *      Author: Ramadan
 */

#include "DMA.h"
#include "GPIO.h"
#include <math.h>

uint32 _Get_Interrupt_Bits(DMA_STREAM stream)
{
	//	FIFO bits	 			0		6		16		22
	//	DIRECT bits				2		8		18		24
	//	TRANSFER ERR bits		3		9		19		25
	//	HALF TRANS bits			4		10		20		26
	//	TRANS COMPLETE bits		5		11		21		27

	// couldn't find a simpler way to access these bits directly, because of the scattered reserved bits
	uint32 interrupts = FIFO_ERROR | DIRECT_MODE_ERROR | TRANSFER_ERROR | HALF_TRANSFER_COMPLETE | TRANSFER_COMPLETE;

	for (int i = 0; i < (stream % 4); i++)
	{
		if (i == 1)
		{
			interrupts = interrupts << 10;
		}
		else
		{
			interrupts = interrupts << 6;
		}
	}

	return interrupts;
}

void Clear_One_Interrupt(DMA_Registers *LINE, const DMA_STREAM stream, DMA_INTERRUPTS interrupt_type)
{
	uint32 interrupt = log2(interrupt_type);
	switch (stream)
	{
	case STREAM0:
	case STREAM4:
		break;
	case STREAM1:
	case STREAM5:
		interrupt <<= 6;
		break;
	case STREAM2:
	case STREAM6:
		interrupt <<= 16;
		break;
	case STREAM3:
	case STREAM7:
		interrupt <<= 22;
		break;
	}
	LINE->IFCR[stream / 4] |= interrupt;
}

void _Clear_Interrupts(DMA_Registers *LINE, const DMA_InitializationObject *config_obj)
{
	uint32 interrupts = _Get_Interrupt_Bits(config_obj->stream);
	LINE->IFCR[config_obj->stream / 4] |= interrupts;
}

void DMA_EnableClock(DMA_Registers *LINE)
{
	//	Enabling the RCC Line for the DMA
	(LINE == DMA1) ? SETBIT(*RCC_AHB1ENR, 21) : SETBIT(*RCC_AHB1ENR, 22);
}

void DMA_Config(DMA_Registers *LINE, DMA_InitializationObject *config_obj)
{
	//	Making Sure the DMA is Disabled for that specific channel in order to unlock configurations
	LINE->S[config_obj->stream].CR.Bits.EN = 0;
	while (LINE->S[config_obj->stream].CR.Bits.EN)
		;
	//	Clearing all interrupts
	_Clear_Interrupts(LINE, config_obj);
	//	Channel Selection
	LINE->S[config_obj->stream].CR.Bits.CHSEL = config_obj->channel;
	//	Configure the number of data to be transfered
	LINE->S[config_obj->stream].NDTR.Bits.NDT = config_obj->n_of_transfers;
	// Set peripheral address (In case of Memory-Memory it carries source address)
	LINE->S[config_obj->stream].PAR.Reg = config_obj->peripheral_address;
	// Set Memory 0 address (In case of Memory-Memory it carries destination address)
	LINE->S[config_obj->stream].M0AR.Reg = config_obj->memory_address;
	//	Select the priority of the transfer
	LINE->S[config_obj->stream].CR.Bits.PL = config_obj->priority;
	// Set the data transfer direction
	LINE->S[config_obj->stream].CR.Bits.DIR = config_obj->direction;
	// Set Whether to use direct or FIFO or Direct Mode
	LINE->S[config_obj->stream].FCR.Bits.DMDIS = config_obj->direct_mode;
	// Set Flow Controller (When Memory-Memory it's set to 0 by hardware)
	LINE->S[config_obj->stream].CR.Bits.PFCTRL = config_obj->flow_controller;
	//	Set MSIZE and PSIZE, In Direct Mode they have to be equal
	if (config_obj->direct_mode == DIRECT_MODE_ENABLED)
	{
		if (config_obj->memory_size == config_obj->peripheral_size)
		{
			LINE->S[config_obj->stream].CR.Bits.MSIZE = config_obj->memory_size;
			LINE->S[config_obj->stream].CR.Bits.PSIZE = config_obj->peripheral_size;
		}
		else
		{
			//			Throw Error
		}
	}
	else
	{
		LINE->S[config_obj->stream].CR.Bits.MSIZE = config_obj->memory_size;
		LINE->S[config_obj->stream].CR.Bits.PSIZE = config_obj->peripheral_size;
		// If FIFO selected, Set FIFO Threshold
		LINE->S[config_obj->stream].FCR.Bits.FTH = config_obj->fifo_threshold;
	}
	// Set Peripheral and Memory Increment Mode
	LINE->S[config_obj->stream].CR.Bits.PINC = config_obj->peripheral_increment_mode;
	LINE->S[config_obj->stream].CR.Bits.MINC = config_obj->memory_increment_mode;
	LINE->S[config_obj->stream].CR.Bits.PINCOS = config_obj->peripheral_increment_offset;

	// Set Peripheral and Memory Burst Transfer Configuration
	LINE->S[config_obj->stream].CR.Bits.MBURST = config_obj->memory_transfer_type;
	LINE->S[config_obj->stream].CR.Bits.PBURST = config_obj->peripheral_transfer_type;

	//	Finally Enabling Interrupts
	LINE->S[config_obj->stream].CR.Bits.DMEIE = config_obj->direct_mode_error_interrupt;
	LINE->S[config_obj->stream].CR.Bits.TCIE = config_obj->transfer_complete_interrupt;
	LINE->S[config_obj->stream].CR.Bits.HTIE = config_obj->half_transfer_interrupt;
	LINE->S[config_obj->stream].CR.Bits.TEIE = config_obj->transfer_error_interrupt;
	LINE->S[config_obj->stream].FCR.Bits.FEIE = config_obj->fifo_error_interrupt;
}

void DMA_BeginTransport(DMA_Registers *LINE, DMA_InitializationObject *config_obj)
{
	LINE->S[config_obj->stream].CR.Bits.EN = 1;
}

DMA_Transfer_States *DMA_GET_Transfer_State(DMA_Registers *LINE, const DMA_STREAM stream)
{
	uint32 interrupts = _Get_Interrupt_Bits(stream) & LINE->ISR[stream / 4];
	switch (stream)
	{
	case STREAM0:
	case STREAM4:
		break;
	case STREAM1:
	case STREAM5:
		interrupts >>= 6;
		break;
	case STREAM2:
	case STREAM6:
		interrupts >>= 16;
		break;
	case STREAM3:
	case STREAM7:
		interrupts >>= 22;
		break;
	}

	interrupt_states.flags.FIFOError = (interrupts >> 0) & 0x01;
	interrupt_states.flags.DirectModeError = (interrupts >> 2) & 0x01;
	interrupt_states.flags.TransferError = (interrupts >> 3) & 0x01;
	interrupt_states.flags.HalfTransfer = (interrupts >> 4) & 0x01;
	interrupt_states.flags.TransferComplete = (interrupts >> 5) & 0x01;

	return &interrupt_states;
}

void DMA2_Stream0_IRQHandler(void)
{
	DMA_Interrupts_Callout_Notification();
}
