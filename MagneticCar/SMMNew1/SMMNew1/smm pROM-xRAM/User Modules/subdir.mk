################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"$(APPCODE)/f8013.c" \
"$(APPCODE)/carsub.c" \
"$(APPCODE)/config.c" \
"$(APPCODE)/debug.c" \

C_SRCS += \
$(APPCODE)/f8013.c \
$(APPCODE)/carsub.c \
$(APPCODE)/config.c \
$(APPCODE)/debug.c \

OBJS += \
./User\ Modules/f8013_c.obj \
./User\ Modules/carsub_c.obj \
./User\ Modules/config_c.obj \
./User\ Modules/debug_c.obj \

OBJS_QUOTED += \
"./User Modules/f8013_c.obj" \
"./User Modules/carsub_c.obj" \
"./User Modules/config_c.obj" \
"./User Modules/debug_c.obj" \

C_DEPS += \
./User\ Modules/F8013_C.d \
./User\ Modules/carsub_C.d \
./User\ Modules/config_c.d \
./User\ Modules/debug_c.d \

C_DEPS_QUOTED += \
"./User Modules/F8013_C.d" \
"./User Modules/carsub_C.d" \
"./User Modules/config_c.d" \
"./User Modules/debug_c.d" \

OBJS_OS_FORMAT += \
./User\ Modules/f8013_c.obj \
./User\ Modules/carsub_c.obj \
./User\ Modules/config_c.obj \
./User\ Modules/debug_c.obj \


# Each subdirectory must supply rules for building sources it contributes
User\ Modules/f8013_c.obj: $(APPCODE)/f8013.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"User Modules/F8013.args" -o "User Modules/f8013_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

User\ Modules/F8013_C.d: $(APPCODE)/f8013.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

User\ Modules/carsub_c.obj: $(APPCODE)/carsub.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"User Modules/carsub.args" -o "User Modules/carsub_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

User\ Modules/carsub_C.d: $(APPCODE)/carsub.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

User\ Modules/config_c.obj: $(APPCODE)/config.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"User Modules/config.args" -o "User Modules/config_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

User\ Modules/config_c.d: $(APPCODE)/config.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

User\ Modules/debug_c.obj: $(APPCODE)/debug.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"User Modules/debug.args" -o "User Modules/debug_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

User\ Modules/debug_c.d: $(APPCODE)/debug.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


