/*
 * DMA.c
 *
 *  Created on: Jan 2, 2022
 *      Author: Ramadan
 */

#include "DMA.h"

void _Clear_Interrupts(DMA_Registers* LINE, const DMA_InitializationObject* config_obj)
{
//	FIFO bits	 			0		6		16		22
//	DIRECT bits				2		8		18		24
//	TRANSFER ERR bits		3		9		19		25
//	HALF TRANS bits			4		10		20		26
//	TRANS COMPLETE bits		5		11		21		27

// couldn't find a simpler way to access these bits directly, because of the scattered reserved bits
	int interrupts = FIFO_ERROR | DIRECT_MODE_ERROR | TRANSFER_ERROR | HALF_TRANSFER_COMPLETE | TRANSFER_COMPLETE;

	for (int i = 0; i <= (config_obj->stream % 4); i++)
	{
		if (i != 2)
		{
			interrupts = interrupts << 6;
		}
		else
		{
			interrupts = interrupts << 10;
		}
	}

	LINE->IFCR[config_obj->stream / 4] |= interrupts;
}


void DMA_EnableClock(DMA_Registers* LINE)
{
	//	Enabling the RCC Line for the DMA
	(LINE == DMA1) ? SETBIT(*RCC_AHB1ENR, 21) : SETBIT(*RCC_AHB1ENR, 22);
}

void DMA_Config(DMA_Registers* LINE, DMA_InitializationObject* config_obj)
{
	//	Making Sure the DMA is Disabled for that specific channel in order to unlock configurations
	LINE->S[config_obj->stream].CR.Bits.EN = 0;
	while(LINE->S[config_obj->stream].CR.Bits.EN);
	//	Clearing all interrupts
	_Disable_Interrupts(LINE, config_obj);
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
	} else {
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

void DMA_BeginTransport(DMA_Registers* LINE, DMA_InitializationObject* config_obj)
{
	LINE->S[config_obj->stream].CR.Bits.EN = 1;
}
