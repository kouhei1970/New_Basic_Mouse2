################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/ADC.c \
../src/Battery.c \
../src/Buzzer.c \
../src/Clock.c \
../src/Control.c \
../src/E2Flash_RX220.c \
../src/Interrupt.c \
../src/Log.c \
../src/Map.c \
../src/Motor.c \
../src/My_Func.c \
../src/New_Basic_Mouse2.c \
../src/Run.c \
../src/SCI.c \
../src/UI.c \
../src/Wait.c \
../src/Wall_Sensor.c \
../src/dbsct.c \
../src/hardware_setup.c \
../src/interrupt_handlers.c \
../src/reset_program.c \
../src/sbrk.c \
../src/vector_table.c \
../src/xprintf.c 

COMPILER_OBJS += \
src/ADC.obj \
src/Battery.obj \
src/Buzzer.obj \
src/Clock.obj \
src/Control.obj \
src/E2Flash_RX220.obj \
src/Interrupt.obj \
src/Log.obj \
src/Map.obj \
src/Motor.obj \
src/My_Func.obj \
src/New_Basic_Mouse2.obj \
src/Run.obj \
src/SCI.obj \
src/UI.obj \
src/Wait.obj \
src/Wall_Sensor.obj \
src/dbsct.obj \
src/hardware_setup.obj \
src/interrupt_handlers.obj \
src/reset_program.obj \
src/sbrk.obj \
src/vector_table.obj \
src/xprintf.obj 

C_DEPS += \
src/ADC.d \
src/Battery.d \
src/Buzzer.d \
src/Clock.d \
src/Control.d \
src/E2Flash_RX220.d \
src/Interrupt.d \
src/Log.d \
src/Map.d \
src/Motor.d \
src/My_Func.d \
src/New_Basic_Mouse2.d \
src/Run.d \
src/SCI.d \
src/UI.d \
src/Wait.d \
src/Wall_Sensor.d \
src/dbsct.d \
src/hardware_setup.d \
src/interrupt_handlers.d \
src/reset_program.d \
src/sbrk.d \
src/vector_table.d \
src/xprintf.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -signed_char -include="C:\PROGRA~2\Renesas\RX\2_8_0\include","C:\Users\kouhei\e2_studio\workspace\New_Basic_Mouse2\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -signed_char -include="C:\PROGRA~2\Renesas\RX\2_8_0\include","C:\Users\kouhei\e2_studio\workspace\New_Basic_Mouse2\generate" -lang=c99 -nomessage -output=obj -obj_path="src" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

