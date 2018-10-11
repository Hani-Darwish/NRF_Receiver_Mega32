################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ServiceLayer/Buffer.c \
../ServiceLayer/EF_Frame.c \
../ServiceLayer/Program_Handl.c \
../ServiceLayer/cwpack.c \
../ServiceLayer/json.c \
../ServiceLayer/umqtt.c 

OBJS += \
./ServiceLayer/Buffer.o \
./ServiceLayer/EF_Frame.o \
./ServiceLayer/Program_Handl.o \
./ServiceLayer/cwpack.o \
./ServiceLayer/json.o \
./ServiceLayer/umqtt.o 

C_DEPS += \
./ServiceLayer/Buffer.d \
./ServiceLayer/EF_Frame.d \
./ServiceLayer/Program_Handl.d \
./ServiceLayer/cwpack.d \
./ServiceLayer/json.d \
./ServiceLayer/umqtt.d 


# Each subdirectory must supply rules for building sources it contributes
ServiceLayer/%.o: ../ServiceLayer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


