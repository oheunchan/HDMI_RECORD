################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/66122_6350.c \
../Core/Src/OEM1_Main.c \
../Core/Src/csc.c \
../Core/Src/hdmitx_drv.c \
../Core/Src/hdmitx_hdcp.c \
../Core/Src/hdmitx_input.c \
../Core/Src/hdmitx_sys.c \
../Core/Src/main.c \
../Core/Src/sha1.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c 

OBJS += \
./Core/Src/66122_6350.o \
./Core/Src/OEM1_Main.o \
./Core/Src/csc.o \
./Core/Src/hdmitx_drv.o \
./Core/Src/hdmitx_hdcp.o \
./Core/Src/hdmitx_input.o \
./Core/Src/hdmitx_sys.o \
./Core/Src/main.o \
./Core/Src/sha1.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/66122_6350.d \
./Core/Src/OEM1_Main.d \
./Core/Src/csc.d \
./Core/Src/hdmitx_drv.d \
./Core/Src/hdmitx_hdcp.d \
./Core/Src/hdmitx_input.d \
./Core/Src/hdmitx_sys.d \
./Core/Src/main.d \
./Core/Src/sha1.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/66122_6350.cyclo ./Core/Src/66122_6350.d ./Core/Src/66122_6350.o ./Core/Src/66122_6350.su ./Core/Src/OEM1_Main.cyclo ./Core/Src/OEM1_Main.d ./Core/Src/OEM1_Main.o ./Core/Src/OEM1_Main.su ./Core/Src/csc.cyclo ./Core/Src/csc.d ./Core/Src/csc.o ./Core/Src/csc.su ./Core/Src/hdmitx_drv.cyclo ./Core/Src/hdmitx_drv.d ./Core/Src/hdmitx_drv.o ./Core/Src/hdmitx_drv.su ./Core/Src/hdmitx_hdcp.cyclo ./Core/Src/hdmitx_hdcp.d ./Core/Src/hdmitx_hdcp.o ./Core/Src/hdmitx_hdcp.su ./Core/Src/hdmitx_input.cyclo ./Core/Src/hdmitx_input.d ./Core/Src/hdmitx_input.o ./Core/Src/hdmitx_input.su ./Core/Src/hdmitx_sys.cyclo ./Core/Src/hdmitx_sys.d ./Core/Src/hdmitx_sys.o ./Core/Src/hdmitx_sys.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/sha1.cyclo ./Core/Src/sha1.d ./Core/Src/sha1.o ./Core/Src/sha1.su ./Core/Src/stm32f1xx_hal_msp.cyclo ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.cyclo ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.cyclo ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src

