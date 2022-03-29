################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../startup/startup_stm32f10x_md.S 

OBJS += \
./startup/startup_stm32f10x_md.o 

S_UPPER_DEPS += \
./startup/startup_stm32f10x_md.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F1 -DSTM32F103RBTx -DNUCLEO_F103RB -DDEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"C:/Users/Garritsen/Documents/Courses/STM32_Microcontrollers/STM32F103RB/Projects_Bare_Metal/Clock_Std/Utilities/STM32F1xx-Nucleo" -I"C:/Users/Garritsen/Documents/Courses/STM32_Microcontrollers/STM32F103RB/Projects_Bare_Metal/Clock_Std/StdPeriph_Driver/inc" -I"C:/Users/Garritsen/Documents/Courses/STM32_Microcontrollers/STM32F103RB/Projects_Bare_Metal/Clock_Std/inc" -I"C:/Users/Garritsen/Documents/Courses/STM32_Microcontrollers/STM32F103RB/Projects_Bare_Metal/Clock_Std/CMSIS/device" -I"C:/Users/Garritsen/Documents/Courses/STM32_Microcontrollers/STM32F103RB/Projects_Bare_Metal/Clock_Std/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


