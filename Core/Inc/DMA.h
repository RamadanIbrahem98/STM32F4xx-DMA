/*
 * DMA.h
 *
 *  Created on: Jan 2, 2022
 *      Author: Ramadan
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "CLK.h"
#include "MACROS.h"
#include "DMA_Registers.h"
#include "StandardTypes.h"

#define NULL ((void *) 0)

#define DMA1 ((DMA_Registers *)(0x40026000))
#define DMA2 ((DMA_Registers *)(0x40026400))

typedef struct {
	CR_Register CR;
	NDTR_Register NDTR;
	PAR_Register PAR;
	M0AR_Register M0AR;
	M1AR_Register M1AR;
	FCR_Register FCR;
}StreamX;

typedef struct{
	vuint32 ISR[2];
	vuint32 IFCR[2];
	StreamX S[8];
}DMA_Registers;

typedef enum {
	CHANNEL0,
	CHANNEL1,
	CHANNEL2,
	CHANNEL3,
	CHANNEL4,
	CHANNEL5,
	CHANNEL6,
	CHANNEL7,
}DMA_CHANNEL;

typedef enum {
	STREAM0,
	STREAM1,
	STREAM2,
	STREAM3,
	STREAM4,
	STREAM5,
	STREAM6,
	STREAM7,
}DMA_STREAM;

typedef enum {
	PERIPHERAL_MEMORY,
	MEMORY_PERIPHERAL,
	MEMORY_MEMORY,
}DMA_MODE;

typedef enum {
	LOW_PRIORITY,
	MEDIUM_PRIORITY,
	HIGH_PRIORITY,
	VERY_HIGH_PRIORITY,
}DMA_PRIORITY;

typedef enum {
	SINGLE,
	INCR4,
	INCR8,
	INCR16,
}DMA_BURST;

typedef enum {
	BYTE,
	HALF_WORD,
	WORD,
}DMA_DATA_SIZE;

typedef enum {
	ONE_FOURTH,
	HALF,
	THREE_FOURTH,
	FULL,
}DMA_FIFO_THRESHOLD;

typedef enum {
	FIXED,
	INCREMENT,
}DMA_INC_MODE;

typedef enum {
	DMA_FLOW_CTRLED,
	PERIPHERAL_FLOW_CTRLED,
}DMA_FLOW_CTRL;

typedef enum {
	DIRECT_MODE_DISABLED,
	DIRECT_MODE_ENABLED,
}DMA_DIRECT_MODE;

typedef enum {
	DISABLED,
	ENABLED,
}DMA_INTERRUPTS_STATES;

typedef enum {
	FIFO_ERROR = 1,
	DIRECT_MODE_ERROR = 4,
	TRANSFER_ERROR = 8,
	HALF_TRANSFER_COMPLETE = 16,
	TRANSFER_COMPLETE = 32,
}DMA_INTERRUPTS;

typedef enum {
	DMA1_STREAM0_IRQ=11,
	DMA1_STREAM1_IRQ,
	DMA1_STREAM2_IRQ,
	DMA1_STREAM3_IRQ,
	DMA1_STREAM4_IRQ,
	DMA1_STREAM5_IRQ,
	DMA1_STREAM6_IRQ,
	DMA1_STREAM7_IRQ=47,
	DMA2_STREAM0_IRQ=56,
	DMA2_STREAM1_IRQ,
	DMA2_STREAM2_IRQ,
	DMA2_STREAM3_IRQ,
	DMA2_STREAM4_IRQ,
	DMA2_STREAM5_IRQ=68,
	DMA2_STREAM6_IRQ,
	DMA2_STREAM7_IRQ,
}DMAx_STREAMx_IRQ;

typedef struct {
	DMA_STREAM stream;
	DMA_CHANNEL channel;
	uint16	n_of_transfers;
	uint32	peripheral_address;
	uint32	memory_address;
	DMA_PRIORITY priority;
	DMA_MODE direction;
	DMA_DIRECT_MODE direct_mode;
	DMA_FIFO_THRESHOLD fifo_threshold;
	DMA_DATA_SIZE peripheral_size;
	DMA_DATA_SIZE memory_size;
	DMA_INC_MODE peripheral_increment_mode;
	DMA_INC_MODE memory_increment_mode;
	DMA_INC_MODE peripheral_increment_offset;
	DMA_FLOW_CTRL flow_controller;
	DMA_BURST peripheral_transfer_type;
	DMA_BURST memory_transfer_type;
	DMA_INTERRUPTS_STATES transfer_complete_interrupt;
	DMA_INTERRUPTS_STATES half_transfer_interrupt;
	DMA_INTERRUPTS_STATES fifo_error_interrupt;
	DMA_INTERRUPTS_STATES transfer_error_interrupt;
	DMA_INTERRUPTS_STATES direct_mode_error_interrupt;
}DMA_InitializationObject;

/**
 * Function: DMA_EnableClock
 * -------------------------
 * Enables the RCC Line for the DMA Chosen
 *
 * DMA_Registers* LINE: the DMAx to be Enabled
 */
void DMA_EnableClock(DMA_Registers* LINE);
/**
 * Function: DMA_Config
 * --------------------
 * Configuration for DMAx Line
 *
 * DMA_Registers* LINE: the DMAx to be Enabled
 * DMA_InitializationObject* config_obj: the object containing all configurations
 */
void DMA_Config(DMA_Registers* LINE, DMA_InitializationObject* config_obj);
/**
 * Function: DMA_BeginTransport
 * ----------------------------
 * Starts the Transaction Process from source to destination
 *
 * DMA_Registers* LINE: the DMAx to be Enabled
 * DMA_InitializationObject* config_obj: the object containing all configurations
 */
void DMA_BeginTransport(DMA_Registers* LINE, DMA_InitializationObject* config_obj);
/**
 * Function: _Clear_Interrupts
 * -----------------------------
 * Private function to clear all flags of all interrupts for a specific stream
 *
 * DMA_Registers* LINE: the DMAx to be Enabled
 * DMA_InitializationObject* config_obj: the object containing all configurations
 */
void _Clear_Interrupts(DMA_Registers* LINE, const DMA_InitializationObject* config_obj);

#endif /* INC_DMA_H_ */


//#define DMAn_LISR(LINE) 		GET_REG(LINE, 0x00)
//#define DMAn_HISR(LINE) 		GET_REG(LINE, 0x04)
//#define DMAn_LIFCR(LINE) 		GET_REG(LINE, 0x08)
//#define DMAn_HIFCR(LINE) 		GET_REG(LINE, 0x0C)
//#define DMAn_SxCR(LINE, x) 	GET_REG(LINE, ((0x10)+(0x10 * x)))
//#define DMAn_SxNDTR(LINE, x) 	GET_REG(LINE, ((0x14)+(0x14 * x)))
//#define DMAn_SxPAR(LINE, x) 	GET_REG(LINE, ((0x18)+(0x18 * x)))
//#define DMAn_SxM0AR(LINE, x) 	GET_REG(LINE, ((0x1C)+(0x1C * x)))
//#define DMAn_SxM1AR(LINE, x) 	GET_REG(LINE, ((0x20)+(0x20 * x)))
//#define DMAn_SxFCR(LINE, x) 	GET_REG(LINE, ((0x24)+(0x24 * x)))
