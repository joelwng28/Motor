/* ******** Time.c ************** 
 * Samuel Zhang, Zi Zhou Wang
 * 2/7/2018
 * Handles time for clock
 * Lab 3
 * Saadallah
 * 2/6/2018
 */
 
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Tachometer.h"
#include "PID.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
#define PF2                     (*((volatile uint32_t *)0x40025010))

uint32_t x = 0;
int32_t speedY;
uint8_t current = 0;
uint32_t last9[9] = {0};
uint32_t speed;

void SysTick_Init(){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 8000000;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; //Priority 2
	NVIC_ST_CTRL_R = 7;
}

void SysTick_Handler(void){
		long sr = StartCritical();
	
		//PF2 = PF2^0x04;  // toggle PF2
		ST7735_SetCursor(8, 0);
		speed = median(last9);
		ST7735_OutUDec(speed/10);
		ST7735_SetCursor(10, 0);
		ST7735_OutChar('.');
		ST7735_OutUDec(speed%10);
		ST7735_SetCursor(8, 1);
		ST7735_OutUDec(Target_Speed/10);
		ST7735_OutString(".0");
		if(x == 0) {
			ST7735_InitR(INITR_REDTAB);
			ST7735_PlotClear(32, 159);
			ST7735_SetCursor(0,0);
			ST7735_OutString("Speed:");
			ST7735_SetCursor(0,1);
			ST7735_OutString("Target:");
		}
		speedY = (ST7735_TFTHEIGHT - speed / 4);
		ST7735_DrawPixel(x+1, speedY+1, ST7735_RED);
		ST7735_DrawPixel(x+1, speedY, ST7735_RED);
		ST7735_DrawPixel(x, speedY+1, ST7735_RED);
		ST7735_DrawPixel(x, speedY, ST7735_RED);
		x = (x + 1) % ST7735_TFTWIDTH;
		EndCritical(sr);
}
