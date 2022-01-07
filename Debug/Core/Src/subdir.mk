################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DMA.c \
../Core/Src/EXTI.c \
../Core/Src/GPIO.c \
../Core/Src/NVIC.c \
../Core/Src/UART.c \
../Core/Src/main.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/DMA.o \
./Core/Src/EXTI.o \
./Core/Src/GPIO.o \
./Core/Src/NVIC.o \
./Core/Src/UART.o \
./Core/Src/main.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/DMA.d \
./Core/Src/EXTI.d \
./Core/Src/GPIO.d \
./Core/Src/NVIC.d \
./Core/Src/UART.d \
./Core/Src/main.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401xC -c -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DMA.d ./Core/Src/DMA.o ./Core/Src/EXTI.d ./Core/Src/EXTI.o ./Core/Src/GPIO.d ./Core/Src/GPIO.o ./Core/Src/NVIC.d ./Core/Src/NVIC.o ./Core/Src/UART.d ./Core/Src/UART.o ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o

.PHONY: clean-Core-2f-Src

