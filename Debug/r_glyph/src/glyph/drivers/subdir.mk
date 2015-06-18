################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\r_glyph/src/glyph/drivers/st7579_lcd.c 

C_DEPS += \
./r_glyph/src/glyph/drivers/st7579_lcd.d 

OBJS += \
./r_glyph/src/glyph/drivers/st7579_lcd.obj 


# Each subdirectory must supply rules for building sources it contributes
r_glyph/src/glyph/drivers/%.obj: ../r_glyph/src/glyph/drivers/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/sdcard" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_bsp" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph/src" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph/src/glyph" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_rspi_rx600" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_rspi_rx600/src" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_switches" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_switches/src" -I"C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/src" -I"C:\PROGRA~2\Renesas\Hew\Tools\Renesas\RX\1_2_1/include" -D__RX   -D__RX600=1  -D__LIT=1 -D__RON=1 -D__UCHAR=1 -D__DBL4=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D_STDC_VERSION_=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x01020100 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/sdcard","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_bsp","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph/src","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_glyph/src/glyph","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_rspi_rx600","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_rspi_rx600/src","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_switches","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/r_switches/src","C:/Users/austin.hansen/e2_studio/workspace/AudioPlayer/src","C:\PROGRA~2\Renesas\Hew\Tools\Renesas\RX\1_2_1/include"  -debug -cpu=rx600 -optimize=0 -speed -alias=noansi -nologo -nologo  -define=__RX   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

