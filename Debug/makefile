################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
API_HCSR04.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
API_HCSR04.list \

OBJCOPY_BIN += \
API_HCSR04.bin \


# All Target
all: API_HCSR04.elf secondary-outputs

# Tool invocations
API_HCSR04.elf: $(OBJS) $(USER_OBJS) C:\Users\Italo\STM32CubeIDE\workspace_1.3.0\API_HCSR04\STM32F103RBTX_FLASH.ld
	arm-none-eabi-gcc -o "API_HCSR04.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\Italo\STM32CubeIDE\workspace_1.3.0\API_HCSR04\STM32F103RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="API_HCSR04.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

API_HCSR04.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "API_HCSR04.list"
	@echo 'Finished building: $@'
	@echo ' '

API_HCSR04.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "API_HCSR04.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
