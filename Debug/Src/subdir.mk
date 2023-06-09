################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/lcd.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/ultrasonic.c 

OBJS += \
./Src/lcd.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/ultrasonic.o 

C_DEPS += \
./Src/lcd.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F401xE -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -c -I../Inc -I"D:/Workspace_stm32/Chip_Headers/CMSIS/Device/ST/STM32F4xx/Include" -I"D:/Workspace_stm32/Chip_Headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/lcd.d ./Src/lcd.o ./Src/lcd.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/ultrasonic.d ./Src/ultrasonic.o ./Src/ultrasonic.su

.PHONY: clean-Src

