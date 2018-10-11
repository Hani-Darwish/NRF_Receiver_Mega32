################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Utilities/subdir.mk
-include ServiceLayer/subdir.mk
-include MiddleWare/subdir.mk
-include MCAL/subdir.mk
-include HAL/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
nrf_Receiver.lss \

FLASH_IMAGE += \
nrf_Receiver.hex \

EEPROM_IMAGE += \
nrf_Receiver.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: nrf_Receiver.elf secondary-outputs

# Tool invocations
nrf_Receiver.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,nrf_Receiver.map -Wl,-gc-sections -mmcu=atmega32 -o "nrf_Receiver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

nrf_Receiver.lss: nrf_Receiver.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S nrf_Receiver.elf  >"nrf_Receiver.lss"
	@echo 'Finished building: $@'
	@echo ' '

nrf_Receiver.hex: nrf_Receiver.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex nrf_Receiver.elf  "nrf_Receiver.hex"
	@echo 'Finished building: $@'
	@echo ' '

nrf_Receiver.eep: nrf_Receiver.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex nrf_Receiver.elf  "nrf_Receiver.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: nrf_Receiver.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 nrf_Receiver.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) nrf_Receiver.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
