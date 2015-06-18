################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp src/*.src src/*.p src/*.pp *.bls *.libelf *.lbk *.abs *.fsy

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include sdcard/subdir.mk
-include r_switches/src/subdir.mk
-include r_rspi_rx600/src/subdir.mk
-include r_glyph/src/subdir.mk
-include r_glyph/src/glyph/subdir.mk
-include r_glyph/src/glyph/fonts/subdir.mk
-include r_glyph/src/glyph/drivers/subdir.mk
-include r_bsp/board/rdkrx63n/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CP_DEPS)),)
-include $(CP_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CP_UPPER_DEPS)),)
-include $(CP_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(OBJ_DEPS)),)
-include $(OBJ_DEPS)
endif
ifneq ($(strip $(ABS_DEPS)),)
-include $(ABS_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(O_DEPS)),)
-include $(O_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
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
CONVERTOR_OUTPUT_OUTPUTS += \
Demos.mot \

LINKER_OUTPUT_OUTPUTS += \
Demos.abs \

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
Demos.lib \

# All Target
all: Demos.mot
	@echo 'Build complete.'

# Tool invocations
Demos.mot: $(LINKER_OUTPUT_OUTPUTS)
	@echo 'Invoking: Converter'
	optlnk   "C:\Users\austin.hansen\e2_studio\workspace\AudioPlayer\Debug\Demos.abs" -form=stype  -output="C:\Users\austin.hansen\e2_studio\workspace\AudioPlayer\Debug\Demos.mot"
	@echo 'Finished building target:'
	@echo.
	$(MAKE) --no-print-directory post-build

Demos.abs: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	optlnk  -subcommand="LinkerSubCommand.tmp"  -library="C:\Users\AUSTIN~1.HAN\E2_STU~1\WORKSP~1\AUDIOP~1\Debug\Demos.lib" -noprelink -list="Demos.map" -nooptimize -debug -output="C:\Users\austin.hansen\e2_studio\workspace\AudioPlayer\Debug\Demos.abs" -rom=D=R,D_1=R_1,D_2=R_2 -vect=_undefined_interrupt_source_isr -nologo -nomessage
	@echo 'Finished building:'
	@echo.

Demos.lib:  
	@echo 'Invoking: Standard Library'
	lbgrx -lang=c99 -head=runtime,mathf,stdio,stdlib -cpu=rx600 -optimize=0 -speed -output="Demos.lib"
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
	-$(RM) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS) 
	-rm -rf "src"/*.obj "src"/*.d "src"/*.src "src"/*.p "src"/*.pp "src"/*.mrc "src"/*.prn
	-rm -rf "sdcard"/*.obj "sdcard"/*.d "sdcard"/*.src "sdcard"/*.p "sdcard"/*.pp "sdcard"/*.mrc "sdcard"/*.prn
	-rm -rf "r_switches/src"/*.obj "r_switches/src"/*.d "r_switches/src"/*.src "r_switches/src"/*.p "r_switches/src"/*.pp "r_switches/src"/*.mrc "r_switches/src"/*.prn
	-rm -rf "r_rspi_rx600/src"/*.obj "r_rspi_rx600/src"/*.d "r_rspi_rx600/src"/*.src "r_rspi_rx600/src"/*.p "r_rspi_rx600/src"/*.pp "r_rspi_rx600/src"/*.mrc "r_rspi_rx600/src"/*.prn
	-rm -rf "r_glyph/src"/*.obj "r_glyph/src"/*.d "r_glyph/src"/*.src "r_glyph/src"/*.p "r_glyph/src"/*.pp "r_glyph/src"/*.mrc "r_glyph/src"/*.prn
	-rm -rf "r_glyph/src/glyph"/*.obj "r_glyph/src/glyph"/*.d "r_glyph/src/glyph"/*.src "r_glyph/src/glyph"/*.p "r_glyph/src/glyph"/*.pp "r_glyph/src/glyph"/*.mrc "r_glyph/src/glyph"/*.prn
	-rm -rf "r_glyph/src/glyph/fonts"/*.obj "r_glyph/src/glyph/fonts"/*.d "r_glyph/src/glyph/fonts"/*.src "r_glyph/src/glyph/fonts"/*.p "r_glyph/src/glyph/fonts"/*.pp "r_glyph/src/glyph/fonts"/*.mrc "r_glyph/src/glyph/fonts"/*.prn
	-rm -rf "r_glyph/src/glyph/drivers"/*.obj "r_glyph/src/glyph/drivers"/*.d "r_glyph/src/glyph/drivers"/*.src "r_glyph/src/glyph/drivers"/*.p "r_glyph/src/glyph/drivers"/*.pp "r_glyph/src/glyph/drivers"/*.mrc "r_glyph/src/glyph/drivers"/*.prn
	-rm -rf "r_bsp/board/rdkrx63n"/*.obj "r_bsp/board/rdkrx63n"/*.d "r_bsp/board/rdkrx63n"/*.src "r_bsp/board/rdkrx63n"/*.p "r_bsp/board/rdkrx63n"/*.pp "r_bsp/board/rdkrx63n"/*.mrc "r_bsp/board/rdkrx63n"/*.prn
	@echo.
	@echo 'Clean complete.'

post-build:
	@echo.
	-C:\Renesas\E2_STU~3\DEBUGC~1\RX\RX_CON~1.EXE Demos.abs Demos.x
	@echo.

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
