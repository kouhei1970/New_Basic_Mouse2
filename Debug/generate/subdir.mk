################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/dbsct.c \
../generate/hwsetup.c \
../generate/intprg.c \
../generate/resetprg.c \
../generate/sbrk.c \
../generate/vecttbl.c 

COMPILER_OBJS += \
generate/dbsct.obj \
generate/hwsetup.obj \
generate/intprg.obj \
generate/resetprg.obj \
generate/sbrk.obj \
generate/vecttbl.obj 

C_DEPS += \
generate/dbsct.d \
generate/hwsetup.d \
generate/intprg.d \
generate/resetprg.d \
generate/sbrk.d \
generate/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
generate/%.obj: ../generate/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_8_0\include","C:\Users\kouhei\e2_studio\workspace\New_Basic_Mouse2\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -nofpu -include="C:\PROGRA~2\Renesas\RX\2_8_0\include","C:\Users\kouhei\e2_studio\workspace\New_Basic_Mouse2\generate" -lang=c99 -nomessage -output=obj -obj_path="generate" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

