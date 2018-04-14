################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: EE2024Asmt1S2AY201718.axf

# Tool invocations
EE2024Asmt1S2AY201718.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\julia\Documents\LPCXpresso_6.1.4_194\workspace\Lib_CMSISv1p30_LPC17xx\Debug" -Xlinker --gc-sections -Xlinker -Map=EE2024Asmt1S2AY201718.map -mcpu=cortex-m3 -mthumb -T "EE2024Asmt1S2AY201718_Debug.ld" -o"EE2024Asmt1S2AY201718.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) EE2024Asmt1S2AY201718.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size EE2024Asmt1S2AY201718.axf; # arm-none-eabi-objcopy -O ihex EE2024Asmt1S2AY201718.axf EE2024Asmt1S2AY201718.hex ;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
