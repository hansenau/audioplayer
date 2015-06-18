/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*                 This application provides a handy framework to serve as a
*                 generic basis for any application. The MCU startup procedures
*                 are provided, as well as some basic means for I/O. The LCD is 
*                 initialized, board switches and their interrupt ISR stubs are
*                 defined, and a simple loop blinks an LED to indicate that the
*                 board is operating.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <mathf.h>
#include <machine.h>
#include "platform.h"
#include "sdcard.h"
//#include "r_switches.h"

#define TWO_PI 6.283185307179586476925286766559
#define PI	   3.1415926535897932384626433832795

#define AMPLITUDE	500.0
#define OFFSET		250.0
#define FREQUENCY_MIN	1024
#define FREQUENCY_MAX	24000

/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/
void led_display(uint16_t number);
void ADC_setup();
void TPU0_setup();
void TPU3_setup();

// Display line for LCD
uint8_t disp_line[13];

// LED ring status
uint16_t led_value;

// Wave frequency and phase offset
float frequency;

// Position in wave
float theta;

// PDM variables
float pdm_theta;
float pdm_error;
float pdm_temp;

// Data buffer
//static uint8_t data[4096];

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
void main(void)
{
  /* Used to pace toggling of the LEDs */
  led_value = 0;

  theta = 0.0;
  frequency = FREQUENCY_MIN;
  pdm_theta = 0;
  pdm_error = 0;

  /* Initialize LCD */
  lcd_initialize();

  /* Clear LCD */
  lcd_clear();

  lcd_display(LCD_LINE1, "whoops");

  // Initialize SD and display R1 on LCD
  //sprintf(disp_line, "resp = 0x%02X", sd_initialize());
  //lcd_display(LCD_LINE1, disp_line);

  //for(uint16_t i=0; i<4095; i++) data[i] = i/16;
  //sd_write(data, 0x1714F000>>9, 1);

  //for(uint16_t i=0; i<4095; i++) data[i] = 0;
  //sd_read(data, 0x00AA0000, 8);

  //while(1);

  /*uint32_t addr = 0;
  while(addr < 0x7f000000) {
	  sprintf(disp_line, "resp = 0x%02X", sd_read(data, addr, 1));
	  lcd_display(LCD_LINE2, disp_line);
	  addr+=0x200;
  }*/

  /* The three pushbuttons on the YRDK board are tied to interrupt lines,
     set them up here */
  //R_SWITCHES_Init();

  //ADC_setup();
  //TPU0_setup();
  //TPU3_setup();

  while (1) {
	  /*sprintf(disp_line, "VOL %3.0f", roundf(((AMPLITUDE*(S12AD.ADDR2/4096.0))+OFFSET)-500)/3.0);
	  lcd_display(LCD_LINE1, disp_line);
	  sprintf(disp_line, "FRQ %.2f", frequency);
	  lcd_display(LCD_LINE2, disp_line);*/
	  led_display(0xfff);
	  for(int i=0; i<0x400000; i++);
	  led_display(0x0000);
	  for(int i=0; i<0x400000; i++);
  }
}

void led_display(uint16_t number) {
  uint16_t value = number;
  LED4 = (value & 0x0001) > 0 ? LED_ON : LED_OFF;
  LED5 = (value & 0x0002) > 0 ? LED_ON : LED_OFF;
  LED6 = (value & 0x0004) > 0 ? LED_ON : LED_OFF;
  LED7 = (value & 0x0008) > 0 ? LED_ON : LED_OFF;
  LED8 = (value & 0x0010) > 0 ? LED_ON : LED_OFF;
  LED9 = (value & 0x0020) > 0 ? LED_ON : LED_OFF;
  LED10 = (value & 0x0040) > 0 ? LED_ON : LED_OFF;
  LED11 = (value & 0x0080) > 0 ? LED_ON : LED_OFF;
  LED12 = (value & 0x0100) > 0 ? LED_ON : LED_OFF;
  LED13 = (value & 0x0200) > 0 ? LED_ON : LED_OFF;
  LED14 = (value & 0x0400) > 0 ? LED_ON : LED_OFF;
  LED15 = (value & 0x0800) > 0 ? LED_ON : LED_OFF;
}

void ADC_setup() {

  SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */

  /* Power up the S12ADC */
  MSTP(S12AD) = 0;

  SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */

  // Enable continuous mode
  S12AD.ADCSR.BYTE = 0x40;

  // Select AN002 (potentiometer) for input
  S12AD.ADANS0.WORD = 0x0004;

  //Start ADC
  S12AD.ADCSR.BIT.ADST = 1;
}

// TPU0 controls wave output
void TPU0_setup() {

  SYSTEM.PRCR.WORD = 0xA50B;
  MSTP(TPU0) = 0;
  SYSTEM.PRCR.WORD = 0xA500;

  // PCLK/1, Clear on CM w/TGRA, falling edge
  TPU0.TCR.BYTE = 0x20;

  // PWM mode 1
  TPU0.TMDR.BYTE = 0x02;

  // Set PWM compare matching (high on A, low on B)
  TPU0.TIORH.BYTE = 0x52;

  // Enable TPU0.TGRA interrupt
  TPU0.TIER.BYTE = 0x41;

  // Set refresh rate to 48 KHz
  TPU0.TGRA = 0x03E8; // == 1000d

  // Set 50% duty cycle (default)
  TPU0.TGRB = 500;

  IR (TPU0, TGI0A);     /* interrupt reset */
  IPR(TPU0, TGI0A) = 4; /* interrupt priority set */
  IEN(TPU0, TGI0A) = 1; /* interrupt enable */

  // Start TPU0
  TPUA.TSTR.BYTE |= 0x01;

}

// TPU3 is currently unused
void TPU3_setup() {

  SYSTEM.PRCR.WORD = 0xA50B;
  MSTP(TPU3) = 0;
  SYSTEM.PRCR.WORD = 0xA500;

  // PCLK/4096, Clear on CM w/TGRA, falling edge
  TPU3.TCR.BYTE = 0x20;

  // Normal operation
  TPU3.TMDR.BYTE = 0x00;

  // Enable TPU3.TGRA interrupt
  TPU3.TIER.BYTE = 0x01;

  // Set initial delay
  TPU3.TGRA = 0x0064;
    
  IR (TPU3, TGI3A);     /* interrupt reset */
  //ICU.FIR.WORD = 0x8086;		/* enable fast interrupt for CM on TGRA */
  IPR(TPU3, TGI3A) = 5; /* interrupt priority set */
  IEN(TPU3, TGI3A) = 1; /* interrupt enable */

  // Start TPU3
  TPUA.TSTR.BYTE |= 0x08;

}

#pragma interrupt (tpu3_isr(vect=134))
static void tpu3_isr(void) {

	pdm_temp = ((S12AD.ADDR2/8192.0)*0.5*sinf(pdm_theta) + 0.5);
	//pdm_temp = 0.1*pdm_theta/TWO_PI;
    PORT1.PODR.BIT.B7 = pdm_temp >= pdm_error ? 1 : 0 ;
    pdm_error = (PORT1.PODR.BIT.B7) - pdm_temp + pdm_error;
    pdm_theta = pdm_theta >= TWO_PI ? pdm_theta - TWO_PI : pdm_theta + (TWO_PI)/(48000.0/2048.0) ;
    frequency = frequency > FREQUENCY_MAX ? FREQUENCY_MIN : frequency*1.0000125 ;

}

#pragma interrupt (tpu0_isr(vect=126))
static void tpu0_isr(void) {

	//TPUA.TSTR.BYTE &= 0xFE;
	TPU0.TGRB = roundf( (AMPLITUDE*(1024.0/4096.0)) * sinf(theta) + OFFSET );
	//TPUA.TSTR.BYTE |= 0x01;
	theta 	  += (theta > TWO_PI) 			    ? -1*TWO_PI       : (TWO_PI)/(48000.0/frequency) ;
  //frequency = (frequency > FREQUENCY_MAX) ? FREQUENCY_MIN  : frequency*1.0000125 					 ;

}


/******************************************************************************
* Function name: sw1_callback
* Description  : Callback function that is executed when SW1 is pressed.
*                Called by sw1_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw1_callback(void)
{

} /* End of function sw1_callback() */


/******************************************************************************
* Function name: sw2_callback
* Description  : Callback function that is executed when SW2 is pressed.
*                Called by sw2_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw2_callback(void)
{

} /* End of function sw2_callback() */


/******************************************************************************
* Function name: sw3_callback
* Description  : Callback function that is executed when SW3 is pressed.
*                Called by sw3_isr in r_switches.c
* Arguments    : none
* Return value : none
******************************************************************************/
void sw3_callback(void)
{
    nop(); /* Add your code here. Note: this is executing inside an ISR. */
} /* End of function sw3_callback() */

