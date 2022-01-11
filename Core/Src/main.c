#include "MACROS.h"
#include "StandardTypes.h"
#include "GPIO.h"
#include "DMA.h"
#include "NVIC.h"

uint32 src[100];
uint32 dest[100];
uint8 transmiting = 1;

void transmission_started()
{
	while (transmiting)
	{
		GPIO_WritePin(GPIOA, 0, 1);
		for (int i = 0; i < 300000; i++)
			;
		GPIO_WritePin(GPIOA, 0, 0);
		for (int i = 0; i < 300000; i++)
			;
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
	for (int i = 0; i < 1000000; i++)
		;
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
	for (int i = 0; i < 1000000; i++)
		;
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
	for (int i = 0; i < 1000000; i++)
		;
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
	for (int i = 0; i < 1000000; i++)
		;
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
	for (int i = 0; i < 1000000; i++)
		;
	GPIO_WritePin(GPIOA, 5, 0);
}

void initialize_dma()
{
	DMA_InitializationObject config_object;

	config_object.stream = STREAM0;
	config_object.channel = CHANNEL0;
	config_object.memory_address = (uint32)dest;
	config_object.peripheral_address = (uint32)src;
	config_object.direct_mode = DIRECT_MODE_DISABLED;
	config_object.direction = MEMORY_MEMORY;
	config_object.n_of_transfers = sizeof(src) / sizeof(uint32);
	config_object.memory_size = WORD;
	config_object.peripheral_size = WORD;
	config_object.memory_increment_mode = INCREMENT;
	config_object.peripheral_increment_mode = INCREMENT;
	config_object.memory_transfer_type = SINGLE;
	config_object.flow_controller = DMA_FLOW_CTRLED;
	config_object.peripheral_transfer_type = SINGLE;
	config_object.priority = VERY_HIGH_PRIORITY;
	config_object.fifo_threshold = FULL;
	config_object.transfer_complete_interrupt = ENABLED;
	config_object.transfer_error_interrupt = ENABLED;
	config_object.fifo_error_interrupt = ENABLED;
	config_object.direct_mode_error_interrupt = ENABLED;
	config_object.half_transfer_interrupt = DISABLED;

	Enable_NVIC(DMA2_STREAM0_IRQ / 32, DMA2_STREAM0_IRQ % 32);
	DMA_EnableClock(DMA2);
	DMA_Config(DMA2, &config_object);
	DMA_BeginTransport(DMA2, &config_object);
	transmission_started();
}

int main()
{
	for (int i = 1; i <= 100; i++)
	{
		src[i - 1] = 100 * i;
	}

	GPIO_EnableClock(PORTA);

	for (int i = 0; i < 6; i++)
	{
		GPIO_Init(GPIOA, i, OUTPUT, PUSH_PULL);
	}

	initialize_dma();

	while (1)
		;

	return 0;
}

void DMA_Interrupts_Callout_Notification()
{
	transmiting = 0;
	DMA_Transfer_States *interrupts_states = DMA_GET_Transfer_State(DMA2, STREAM0);
	if (interrupts_states->flags.TransferComplete)
	{
		transmission_ended();
		//		Check data integrity
		for (int i = 0; i < 100; i++)
		{
			if (src[i] != dest[i])
			{
				data_integrity_error();
			}
			break;
		}
		Clear_One_Interrupt(DMA2, STREAM0, TRANSFER_COMPLETE);
	}
	else if (interrupts_states->flags.TransferError)
	{
		transmission_error();
		Clear_One_Interrupt(DMA2, STREAM0, TRANSFER_ERROR);
	}
	else if (interrupts_states->flags.FIFOError)
	{
		fifo_error();
		Clear_One_Interrupt(DMA2, STREAM0, FIFO_ERROR);
	}
	else if (interrupts_states->flags.DirectModeError)
	{
		direct_error();
		Clear_One_Interrupt(DMA2, STREAM0, DIRECT_MODE_ERROR);
	}
}
