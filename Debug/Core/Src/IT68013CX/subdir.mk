################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/IT68013CX/it68013CX_IO.c \
../Core/Src/IT68013CX/it68013CX_Utility.c \
../Core/Src/IT68013CX/it68013CX_it680X.c \
../Core/Src/IT68013CX/it68013CX_main.c 

OBJS += \
./Core/Src/IT68013CX/it68013CX_IO.o \
./Core/Src/IT68013CX/it68013CX_Utility.o \
./Core/Src/IT68013CX/it68013CX_it680X.o \
./Core/Src/IT68013CX/it68013CX_main.o 

C_DEPS += \
./Core/Src/IT68013CX/it68013CX_IO.d \
./Core/Src/IT68013CX/it68013CX_Utility.d \
./Core/Src/IT68013CX/it68013CX_it680X.d \
./Core/Src/IT68013CX/it68013CX_main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/IT68013CX/%.o Core/Src/IT68013CX/%.su Core/Src/IT68013CX/%.cyclo: ../Core/Src/IT68013CX/%.c Core/Src/IT68013CX/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-IT68013CX

clean-Core-2f-Src-2f-IT68013CX:
	-$(RM) ./Core/Src/IT68013CX/it68013CX_IO.cyclo ./Core/Src/IT68013CX/it68013CX_IO.d ./Core/Src/IT68013CX/it68013CX_IO.o ./Core/Src/IT68013CX/it68013CX_IO.su ./Core/Src/IT68013CX/it68013CX_Utility.cyclo ./Core/Src/IT68013CX/it68013CX_Utility.d ./Core/Src/IT68013CX/it68013CX_Utility.o ./Core/Src/IT68013CX/it68013CX_Utility.su ./Core/Src/IT68013CX/it68013CX_it680X.cyclo ./Core/Src/IT68013CX/it68013CX_it680X.d ./Core/Src/IT68013CX/it68013CX_it680X.o ./Core/Src/IT68013CX/it68013CX_it680X.su ./Core/Src/IT68013CX/it68013CX_main.cyclo ./Core/Src/IT68013CX/it68013CX_main.d ./Core/Src/IT68013CX/it68013CX_main.o ./Core/Src/IT68013CX/it68013CX_main.su

.PHONY: clean-Core-2f-Src-2f-IT68013CX

