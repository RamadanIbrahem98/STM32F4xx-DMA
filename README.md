# DMA Driver

Complete Implementation for the DMA Controller of STM32f4xx

## API Components

At first, I tried writing the driver the same way as always with literals in some macro definitions
The problem is that the configuration lines are huge, it's never a convenient process to pass them to a function as a function parameters
I instead took the approach of pointer to a struct to access (read/write) (from/to) the memory

### Files

1. DMA_Registers.h

This file contains some of the registers used in the MCU with names of bits used in order not to write literals

2. DMA.h

This file contains functions declerations for the DMA API and the required variables and objects for them

3. DMA.c

This file contains all API functions definitions

### DMA_Registers.h

**_CR_Register_**, **_NDTR_Register_**, **_PAR_Register_**, **_M0AR_Register_**, **_M1AR_Register_**, **_FCR_Register_**

These are the registers that repeats for every stream

```c
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
```

### DMA.h

Defining objects (structs and enums) to be used in the functions. Here are the descriptions for each one of them

**_StreamX_** (struct): contains the 6 Registers for each stream with their types imported from the **_DMA_Registes.h_**

```c
typedef struct {
	CR_Register CR;
	NDTR_Register NDTR;
	PAR_Register PAR;
	M0AR_Register M0AR;
	M1AR_Register M1AR;
	FCR_Register FCR;
}StreamX;
```

**_DMA_Registers_** (struct): contains all Registers in order, for DMAx, this is used as the variable type to be used as a pointer to a DMA_Registers

```c
#define DMA1 ((DMA_Registers *)(0x40026000))
#define DMA2 ((DMA_Registers *)(0x40026400))
...
typedef struct{
	vuint32 ISR[2];
	vuint32 IFCR[2];
	StreamX S[8];
}DMA_Registers;
```

**_DMA_CHANNEL_** (enum): is the channel used by the stream

```c
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
```

**_DMA_STREAM_** (enum): is the stream used in by the DMA

```c
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
```

**_DMA_MODE_** (enum): is the data direction mode

```c
typedef enum {
	PERIPHERAL_MEMORY,
	MEMORY_PERIPHERAL,
	MEMORY_MEMORY,
}DMA_MODE;
```

**_DMA_PRIORITY_** (enum): is the transfer priority for the arbitrer

```c
typedef enum {
	LOW_PRIORITY,
	MEDIUM_PRIORITY,
	HIGH_PRIORITY,
	VERY_HIGH_PRIORITY,
}DMA_PRIORITY;
```

**_DMA_BURST_** (enum): is the type of transfer rate whether single or bursts of different sizes

```c
typedef enum {
	SINGLE,
	INCR4,
	INCR8,
	INCR16,
}DMA_BURST;
```

**_DMA_DATA_SIZE_** (enum): is the size of data for the peripheral or memory

```c
typedef enum {
	BYTE,
	HALF_WORD,
	WORD,
}DMA_DATA_SIZE;
```

**_DMA_FIFO_THRESHOLD_** (enum): is the fifo threshold for it to transfer data

```c
typedef enum {
	ONE_FOURTH,
	HALF,
	THREE_FOURTH,
	FULL,
}DMA_FIFO_THRESHOLD;
```

**_DMA_INC_MODE_** (enum): is the mode on which peripheral or address pointer gets increamented

```c
typedef enum {
	FIXED,
	INCREMENT,
}DMA_INC_MODE;
```

**_DMA_FLOW_CTRL_** (enum): is the flow controller whether it's a dma controller or peripheral controller

```c
typedef enum {
	DMA_FLOW_CTRLED,
	PERIPHERAL_FLOW_CTRLED,
}DMA_FLOW_CTRL;
```

**_DMA_DIRECT_MODE_** (enum): is whether to enable or disable the direct mode

```c
typedef enum {
	DIRECT_MODE_DISABLED,
	DIRECT_MODE_ENABLED,
}DMA_DIRECT_MODE;
```

**_DMA_INTERRUPTS_STATES_** (enum): is the logic for interrupts enable or disable

```c
typedef enum {
	DISABLED,
	ENABLED,
}DMA_INTERRUPTS_STATES;
```

**_DMA_INTERRUPTS_** (enum): is the interrupts with their respective values to select the desired interrupt in the ISR, and IFCR Registers

```c
typedef enum {
	FIFO_ERROR = 1,
	DIRECT_MODE_ERROR = 4,
	TRANSFER_ERROR = 8,
	HALF_TRANSFER_COMPLETE = 16,
	TRANSFER_COMPLETE = 32,
}DMA_INTERRUPTS;
```

**_DMAx_STREAMx_IRQ_** (enum): is extracted from the vector table

```c
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
```

and finally **_DMA_InitializationObject_** (struct): it uses all the previously defined custom data types to enable the API consumer from choosing the wanted configurations

```c
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
```

### DMA.c

The first function is a private function used by the Configuration function, in order to clear all the interrupts of a specific stream.

```c
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
```

The second function enables the RCC clock for DMAx

```c
void DMA_EnableClock(DMA_Registers* LINE)
{
	//	Enabling the RCC Line for the DMA
	(LINE == DMA1) ? SETBIT(*RCC_AHB1ENR, 21) : SETBIT(*RCC_AHB1ENR, 22);
}
```

The third function contains the configuration information with inline comments describing the steps
Here I didn't check for validating the user input, if they by mistake add a wrong configuration, then it's there problem XD

```c
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
```

The forth and final function starts the transfer process by enabling the streamx

```c
void DMA_BeginTransport(DMA_Registers* LINE, DMA_InitializationObject* config_obj)
{
	LINE->S[config_obj->stream].CR.Bits.EN = 1;
}
```

## Client Application

1. Create two arrays **_src_** and **_dest_** both of 100 elements, with initializing src[index] to 100, 200, ...

```c
uint32 src[100];
uint32 dest[100];

int main()
{
  ...
  for(int i = 1; i <= 100; i++)
  {
    src[i - 1] = i * 100;
  }
  ...
}
```

2. Enable the GPIOA PORT for adding flags to watch the transfer process

```c
int main()
{
  ...
  GPIO_EnableClock(PORTA);
  ...
  for (int i = 0; i < 6; i++)
  {
    GPIO_Init(GPIOA, i, OUTPUT, PUSH_PULL);
  }
  ...
}
```

3. Call the function that creates the DMA initialization object and enables the interrupts from the nvic line

```c
void initialize_dma()
{
	DMA_InitializationObject config_object;

	config_object.stream 						= STREAM0;
	config_object.channel 						= CHANNEL0;
	config_object.memory_address 				= (uint32)dest;
	config_object.peripheral_address 			= (uint32)src;
	config_object.direct_mode 					= DIRECT_MODE_DISABLED;
	config_object.direction 					= MEMORY_MEMORY;
	config_object.n_of_transfers 				= sizeof(src) / sizeof(uint32);
	config_object.memory_size 					= WORD;
	config_object.peripheral_size 				= WORD;
	config_object.memory_increment_mode 		= INCREMENT;
	config_object.peripheral_increment_mode 	= INCREMENT;
	config_object.memory_transfer_type 			= SINGLE;
	config_object.flow_controller 				= DMA_FLOW_CTRLED;
	config_object.peripheral_transfer_type 		= SINGLE;
	config_object.priority 						= VERY_HIGH_PRIORITY;
	config_object.fifo_threshold 				= FULL;
	config_object.transfer_complete_interrupt 	= ENABLED;
	config_object.transfer_error_interrupt 		= ENABLED;
	config_object.fifo_error_interrupt 			= ENABLED;
	config_object.direct_mode_error_interrupt 	= ENABLED;
	config_object.half_transfer_interrupt 		= DISABLED;

	Enable_NVIC(DMA2_STREAM0_IRQ/32, DMA2_STREAM0_IRQ%32);
	DMA_EnableClock(DMA2);
	DMA_Config(DMA2, &config_object);
	DMA_BeginTransport(DMA2, &config_object);
	transmission_started();
}

int main()
{
  ...
  initialize_dma();
  ...
}
```

4. Add 6 Methods for each of the different flags **_transmission_in_progress_**, **_transmission_completed_**, **_transmission_error_**, **_data_integrity_error_**, **_fifo_error_**, **_direction_error_**

the transmission in progress is an infinite while loop until it's interrupted by any of the DMA flags, however, this flag never gets a chance to be noticed, as the transction barely takes time to complete.

```c
void transmission_started()
{
	while(transmiting)
	{
		GPIO_WritePin(GPIOA, 0, 1);
		for (int i = 0; i < 300000; i++);
		GPIO_WritePin(GPIOA, 0, 0);
		for (int i = 0; i < 300000; i++);
	}
}

void transmission_ended()
{
	GPIO_WritePin(GPIOA, 0, 0);
	GPIO_WritePin(GPIOA, 1, 1);
	GPIO_WritePin(GPIOA, 2, 0);
	GPIO_WritePin(GPIOA, 3, 0);
	GPIO_WritePin(GPIOA, 4, 0);
	GPIO_WritePin(GPIOA, 5, 0);
	for (int i = 0; i < 1000000; i++);
	GPIO_WritePin(GPIOA, 1, 0);
}

void data_integrity_error()
{
	GPIO_WritePin(GPIOA, 0, 0);
	GPIO_WritePin(GPIOA, 1, 0);
	GPIO_WritePin(GPIOA, 2, 0);
	GPIO_WritePin(GPIOA, 3, 1);
	GPIO_WritePin(GPIOA, 4, 0);
	GPIO_WritePin(GPIOA, 5, 0);
	for (int i = 0; i < 1000000; i++);
	GPIO_WritePin(GPIOA, 3, 0);
}

void transmission_error()
{
	GPIO_WritePin(GPIOA, 0, 0);
	GPIO_WritePin(GPIOA, 1, 0);
	GPIO_WritePin(GPIOA, 2, 1);
	GPIO_WritePin(GPIOA, 3, 0);
	GPIO_WritePin(GPIOA, 4, 0);
	GPIO_WritePin(GPIOA, 5, 0);
	for (int i = 0; i < 1000000; i++);
	GPIO_WritePin(GPIOA, 2, 0);
}

void fifo_error()
{
	GPIO_WritePin(GPIOA, 0, 0);
	GPIO_WritePin(GPIOA, 1, 0);
	GPIO_WritePin(GPIOA, 2, 0);
	GPIO_WritePin(GPIOA, 3, 0);
	GPIO_WritePin(GPIOA, 4, 1);
	GPIO_WritePin(GPIOA, 5, 0);
	for (int i = 0; i < 1000000; i++);
	GPIO_WritePin(GPIOA, 4, 0);
}

void direct_error()
{
	GPIO_WritePin(GPIOA, 0, 0);
	GPIO_WritePin(GPIOA, 1, 0);
	GPIO_WritePin(GPIOA, 2, 0);
	GPIO_WritePin(GPIOA, 3, 0);
	GPIO_WritePin(GPIOA, 4, 0);
	GPIO_WritePin(GPIOA, 5, 1);
	for (int i = 0; i < 1000000; i++);
	GPIO_WritePin(GPIOA, 5, 0);
}
```

5. The Interrupt Service Routine to handle the different DMA flags

The Direct Error is not needed here as we've disabled the direct mode, but I was debugging an issue so I added it

```c
void DMA2_Stream0_IRQHandler(void)
{
	transmiting = 0;
	if ((DMA2->ISR[0] & (0x01 << 5)) >> 5)
	{
		transmission_ended();
      // Check data integrity
		for (int i = 0; i < 100; i++)
		{
			if (src[i] != dest[i])
			{
				data_integrity_error();
			}
			break;
		}
		SETBIT(DMA2->IFCR[0], 5);
	}
	else if ((DMA2->ISR[0] & (0x01 << 3)) >> 3)
	{
		transmission_error();
		SETBIT(DMA2->IFCR[0], 3);
	}
	else if ((DMA2->ISR[0] & (0x01 << 0)) >> 0)
	{
		fifo_error();
		SETBIT(DMA2->IFCR[0], 0);
	}
	else if ((DMA2->ISR[0] & (0x01 << 2)) >> 2)
	{
		direct_error();
		SETBIT(DMA2->IFCR[0], 2);
	}
}
```
