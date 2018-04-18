/* ******** Switch.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/20/2018
 * switch init, debouncing, and function control
 * Lab 5
 * Saadallah
 * 2/20/2018
 */

#include "../inc/tm4c123gh6pm.h"
#include <stdint.h>
#include "PID.h"


#define PE3 (*((volatile uint32_t *)0x40024020))
#define PB3 (*((volatile uint32_t *)0x40005020))
	
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

//flags for timer to handle interrupts
static uint8_t PEIntFlag = 0;
static uint8_t PBIntFlag = 0;

//registers for holding previous state of ports to ensure no debouncing
volatile static unsigned long PELast;
volatile static unsigned long PBLast;


//*************Timer0Arm********************************************
//  Initializes one-shot 20ms timer for debouncing
//  Inputs: None
//  Output: none
static void Timer3Arm(void){
	SYSCTL_RCGCTIMER_R |= 0x08;
  TIMER3_CTL_R = 0x00000000;    // 1) disable TIMER3A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x0000001;    // 3) 1-SHOT mode
  TIMER3_TAILR_R = 320000;      // 4) 20ms reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
}

//*************GPIOArm********************************************
//  Reenables switch interrupts
//  Inputs: None
//  Output: none
void GPIOArm(void) {
	if(PEIntFlag) {
		PELast = PE3;					//switch state
		PEIntFlag = 0;
		GPIO_PORTE_ICR_R = 0x08;      //clear flag3
		GPIO_PORTE_IM_R |= 0x08;      //arm interrupt on PE3
		NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5

	}
	else if(PBIntFlag) {
		PBLast = PB3;					//switch state
		PBIntFlag = 0;
		GPIO_PORTB_ICR_R = 0x08;      //clear flag3
		GPIO_PORTB_IM_R |= 0x08;      //arm interrupt on PB3 

		NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
		
	}
}

//*************Timer0A_Handler********************************************
//  Disarms Timer0A and reeanbles appropriate switch interrupt
//  Inputs: None
//  Output: none
void Timer3A_Handler(void){
  TIMER3_IMR_R = 0x00000000;    	// disarm timeout interrupt
  GPIOArm();   										// start GPIO
}


//*************Switches_Init********************************************
//  Initialize "Up" "Down" and "Select" Switches
//  Inputs: None
//  Output: none
void Switches_Init(void) {
	SYSCTL_RCGCGPIO_R |= 0x16; 	//activate clock for Port E and Port C and Port B
	SYSCTL_RCGCGPIO_R |= 0x08; 	//activate clock for Port D
	//initialize PE3
	GPIO_PORTE_DIR_R &= ~0x08; 	//make PE3 in
	GPIO_PORTE_DEN_R |= 0x08;		//digital enable PE3
	GPIO_PORTE_AFSEL_R &= ~0x08; 	// disable alt funct on PE3
	GPIO_PORTE_AMSEL_R &= ~0x08;	// disable analog on PE3
	GPIO_PORTE_PCTL_R &= ~0x000F000; // configure PE3 as GPIO
	GPIO_PORTE_IS_R &= ~0x08;		//PE3 is edge sensitive
	GPIO_PORTE_IBE_R |= 0x08;	//PE3 is both edges
	GPIO_PORTE_IM_R |= 0x08; 		//arm interrupt on PE3
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00) | 0x000000A0;	//PortE priority = 5
	//initialize PB3
	GPIO_PORTB_DIR_R &= ~0x08; 	//make PB3 in
	GPIO_PORTB_DEN_R |= 0x08;		//digital enable PB3
	GPIO_PORTB_AFSEL_R &= ~0x08; 	// disable alt funct on PB3
	GPIO_PORTB_AMSEL_R &= ~0x08;	// disable analog on PB3
	GPIO_PORTB_PCTL_R &= ~0x000F000; // configure PB3 as GPIO
	GPIO_PORTB_IS_R &= ~0x08;		//PB3 is edge sensitive
	GPIO_PORTB_IBE_R |= 0x08;	//PB3 is both edges
	GPIO_PORTB_IM_R |= 0x08; 		//arm interrupt on PB3
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF00FF) | 0x0000A000; 	//PortB priority = 5
	
	NVIC_EN0_R = 16;				//Enable interrupts for PortE
	NVIC_EN0_R = 2;				//Enable interrupts for PortB

	PELast = PE3;					//Set initial state
	PBLast = PB3;					//Set initial state
}

long sr;	//for critical sections


void Up(void) {
	if(Target_Speed < 500) {
		Target_Speed += 50;
	}
}


void Down(void) {
	if(Target_Speed > 250) {
		Target_Speed -= 50;
	}		
}


//*************GPIOPortE_Handler********************************************
//  Completes Up Functionality
//  Inputs: None
//  Output: none
void GPIOPortE_Handler(void) {
	GPIO_PORTE_IM_R &= ~0x08;		//disarm intterupt on PE3
	PEIntFlag = 1;
	if(!PELast) {
		sr = StartCritical();						//start critical section
		Up();
		
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}


//*************GPIOPortB_Handler********************************************
//  Completes Select Functionality
//  Inputs: None
//  Output: none
void GPIOPortB_Handler(void) {
	GPIO_PORTB_IM_R &= ~0x08;		//disarm intterupt on PB3
	PBIntFlag = 1;
	if(!PBLast) {
		sr = StartCritical();						//start critical section
		Down();
		EndCritical(sr);						//end critical section
	}
	else {}
	
	Timer3Arm();								//start one shot
}








