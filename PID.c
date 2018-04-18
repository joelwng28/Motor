#include <stdint.h>
#include "PWM.h"
#include "..//inc//tm4c123gh6pm.h"
#include "Tachometer.h"
#include "SysTick.h"

#define PF2                     (*((volatile uint32_t *)0x40025010))

uint32_t Target_Speed;
int32_t E;           // speed error in 0.1 rps
int32_t U;           // duty cycle 40 to 39960

void Timer2_Init(){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate timer2
  TIMER2_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = 800000;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable timer2A
}


void Timer2A_Handler(void){

	TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
	PF2 = PF2^0x04;  // toggle PF2
	E = Target_Speed-median(last9);          // 0.1 rps
  U = U+(60*E)/64;           // discrete integral
  if(U < 25000) U=25000;        // Constrain output
  if(U>39999) U=39999;      // 100 to 39900
  PWM0B_Duty(U);            // output

}
