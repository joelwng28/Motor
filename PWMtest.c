// PWMtest.c
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
   Program 6.8, section 6.3.2

   "Embedded Systems: Real-Time Operating Systems for ARM Cortex M Microcontrollers",
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2015
   Program 8.4, Section 8.3

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
#include "PWM.h"
#include "InputCapture.h"
#include "Tachometer.h"
#include "ST7735.h"
#include "SysTick.h"
#include "PID.h"
#include "Switch.h"

#define PF2                     (*((volatile uint32_t *)0x40025010))

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
char* speedText = "Speed:";
char* target = "Target:";
int counter_Done = 0;

int main(void){
	DisableInterrupts();
	PLL_Init(Bus80MHz);             // bus clock at 50 MHz
  Tach_Init();										// initialize the Tach measureing input capture
	
	PF2 = 0x00;
	
	SysTick_Init();
	Switches_Init();
	PWM0B_Init(40000, 35000); 			// 1000 Hz
	ST7735_InitR(INITR_REDTAB);
	Timer2_Init();
	Target_Speed = 400;
	ST7735_SetCursor(0,0);
	ST7735_OutString(speedText);
	ST7735_SetCursor(0,1);
	ST7735_OutString(target);
	EnableInterrupts();
  while(1){
  }
}
