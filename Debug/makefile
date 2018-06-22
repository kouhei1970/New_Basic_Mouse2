################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif 
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif 
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif 
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif 
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
New_Basic_Mouse2.abs

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
New_Basic_Mouse2.lib

CONVERTOR_OUTPUT_OUTPUTS += \
New_Basic_Mouse2.mot \
New_Basic_Mouse2.x

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(CONVERTOR_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

New_Basic_Mouse2.mot: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Invoking: Converter'
	@echo 'Building target:'
	rlink "New_Basic_Mouse2.abs" -form=stype -output="New_Basic_Mouse2.mot" -byte_count=10 -nologo -nomessage
	@echo 'Finished building target:'
	@echo.


New_Basic_Mouse2.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	rlink -noprelink -subcommand="LinkerSubCommand.tmp"  -output="New_Basic_Mouse2.abs" -form=absolute -nomessage -list=New_Basic_Mouse2.map -nooptimize -rom=D=R,D_1=R_1,D_2=R_2 -nologo
	@echo 'Finished building:'
	@echo.


New_Basic_Mouse2.lib: 
	@echo 'Invoking: Library Generator'
	lbgrx -isa=rxv1 -nofpu -signed_char -lang=c -head=runtime,stdio,stdlib,string,new -output="New_Basic_Mouse2.lib" -nologo
	@echo 'Finished building:'
	@echo.

New_Basic_Mouse2.x: New_Basic_Mouse2.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe New_Basic_Mouse2.abs New_Basic_Mouse2.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS)
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.src src/*.jmp src/*.lst src/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets