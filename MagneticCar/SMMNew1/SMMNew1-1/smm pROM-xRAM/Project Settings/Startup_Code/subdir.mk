################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Project\ Settings/Startup_Code/56F80xx_init.asm \

ASM_SRCS_QUOTED += \
"../Project Settings/Startup_Code/56F80xx_init.asm" \

OBJS += \
./Project\ Settings/Startup_Code/56F80xx_init_asm.obj \

ASM_DEPS += \
./Project\ Settings/Startup_Code/56F80xx_init_asm.d \

OBJS_QUOTED += \
"./Project Settings/Startup_Code/56F80xx_init_asm.obj" \

ASM_DEPS_QUOTED += \
"./Project Settings/Startup_Code/56F80xx_init_asm.d" \

OBJS_OS_FORMAT += \
./Project\ Settings/Startup_Code/56F80xx_init_asm.obj \


# Each subdirectory must supply rules for building sources it contributes
Project\ Settings/Startup_Code/56F80xx_init_asm.obj: ../Project\ Settings/Startup_Code/56F80xx_init.asm
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: DSC Assembler'
	"$(DSC_ToolsDirEnv)/mwasm56800e" @@"Project Settings/Startup_Code/56F80xx_init.args" -o "Project Settings/Startup_Code/56F80xx_init_asm.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Project\ Settings/Startup_Code/56F80xx_init_asm.d: ../Project\ Settings/Startup_Code/56F80xx_init.asm
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


