################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Generated_Code/AD1.c" \
"../Generated_Code/AS1.c" \
"../Generated_Code/COUNTER1.c" \
"../Generated_Code/COUNTER2.c" \
"../Generated_Code/Cpu.c" \
"../Generated_Code/IFsh1.c" \
"../Generated_Code/LED.c" \
"../Generated_Code/PWMC1.c" \
"../Generated_Code/TI1.c" \

C_UPPER_SRCS += \
$(MAGNETICCAR)/SMMNew1/AppCode/F8013.C \

C_UPPER_SRCS_QUOTED += \
"$(MAGNETICCAR)/SMMNew1/AppCode/F8013.C" \

C_SRCS += \
../Generated_Code/AD1.c \
../Generated_Code/AS1.c \
../Generated_Code/COUNTER1.c \
../Generated_Code/COUNTER2.c \
../Generated_Code/Cpu.c \
../Generated_Code/IFsh1.c \
../Generated_Code/LED.c \
../Generated_Code/PWMC1.c \
../Generated_Code/TI1.c \

OBJS += \
./Generated_Code/AD1_c.obj \
./Generated_Code/AS1_c.obj \
./Generated_Code/COUNTER1_c.obj \
./Generated_Code/COUNTER2_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/F8013_C.obj \
./Generated_Code/IFsh1_c.obj \
./Generated_Code/LED_c.obj \
./Generated_Code/PWMC1_c.obj \
./Generated_Code/TI1_c.obj \

OBJS_QUOTED += \
"./Generated_Code/AD1_c.obj" \
"./Generated_Code/AS1_c.obj" \
"./Generated_Code/COUNTER1_c.obj" \
"./Generated_Code/COUNTER2_c.obj" \
"./Generated_Code/Cpu_c.obj" \
"./Generated_Code/F8013_C.obj" \
"./Generated_Code/IFsh1_c.obj" \
"./Generated_Code/LED_c.obj" \
"./Generated_Code/PWMC1_c.obj" \
"./Generated_Code/TI1_c.obj" \

C_DEPS += \
./Generated_Code/AD1_c.d \
./Generated_Code/AS1_c.d \
./Generated_Code/COUNTER1_c.d \
./Generated_Code/COUNTER2_c.d \
./Generated_Code/Cpu_c.d \
./Generated_Code/IFsh1_c.d \
./Generated_Code/LED_c.d \
./Generated_Code/PWMC1_c.d \
./Generated_Code/TI1_c.d \

C_UPPER_DEPS_QUOTED += \
"./Generated_Code/F8013_C.d" \

C_UPPER_DEPS += \
./Generated_Code/F8013_C.d \

C_DEPS_QUOTED += \
"./Generated_Code/AD1_c.d" \
"./Generated_Code/AS1_c.d" \
"./Generated_Code/COUNTER1_c.d" \
"./Generated_Code/COUNTER2_c.d" \
"./Generated_Code/Cpu_c.d" \
"./Generated_Code/IFsh1_c.d" \
"./Generated_Code/LED_c.d" \
"./Generated_Code/PWMC1_c.d" \
"./Generated_Code/TI1_c.d" \

OBJS_OS_FORMAT += \
./Generated_Code/AD1_c.obj \
./Generated_Code/AS1_c.obj \
./Generated_Code/COUNTER1_c.obj \
./Generated_Code/COUNTER2_c.obj \
./Generated_Code/Cpu_c.obj \
./Generated_Code/F8013_C.obj \
./Generated_Code/IFsh1_c.obj \
./Generated_Code/LED_c.obj \
./Generated_Code/PWMC1_c.obj \
./Generated_Code/TI1_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/AD1_c.obj: ../Generated_Code/AD1.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/AD1.args" -o "Generated_Code/AD1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/%.d: ../Generated_Code/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Generated_Code/AS1_c.obj: ../Generated_Code/AS1.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/AS1.args" -o "Generated_Code/AS1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/COUNTER1_c.obj: ../Generated_Code/COUNTER1.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/COUNTER1.args" -o "Generated_Code/COUNTER1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/COUNTER2_c.obj: ../Generated_Code/COUNTER2.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/COUNTER2.args" -o "Generated_Code/COUNTER2_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/Cpu_c.obj: ../Generated_Code/Cpu.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/Cpu.args" -o "Generated_Code/Cpu_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/F8013_C.obj: $(MAGNETICCAR)/SMMNew1/AppCode/F8013.C
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/F8013.args" -o "Generated_Code/F8013_C.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/F8013_C.d: $(MAGNETICCAR)/SMMNew1/AppCode/F8013.C
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Generated_Code/IFsh1_c.obj: ../Generated_Code/IFsh1.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/IFsh1.args" -o "Generated_Code/IFsh1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/LED_c.obj: ../Generated_Code/LED.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/LED.args" -o "Generated_Code/LED_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/PWMC1_c.obj: ../Generated_Code/PWMC1.c
	@echo 'Building file: $<'
	@echo 'Executing target #17 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/PWMC1.args" -o "Generated_Code/PWMC1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Generated_Code/TI1_c.obj: ../Generated_Code/TI1.c
	@echo 'Building file: $<'
	@echo 'Executing target #18 $<'
	@echo 'Invoking: DSC Compiler'
	"$(DSC_ToolsDirEnv)/mwcc56800e" -c @@"Generated_Code/TI1.args" -o "Generated_Code/TI1_c.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


