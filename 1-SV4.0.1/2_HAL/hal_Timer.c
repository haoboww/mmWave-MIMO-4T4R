#include "hal_Timer.h"
#include "main.h"

void TMRB0_Init(void){
	TMRB_mTimeout(0,stFlashParam.frameTime); 
	NVIC_DisableIRQ(TMRB0_IRQn);
	NVIC_SetPriority (TMRB0_IRQn, TMRB0_PRIORITY_LV);
	NVIC_EnableIRQ(TMRB0_IRQn); //enable cpu quad timer0 interrupt
}

//1ms= SYS_CLK / 1000
void SysTick_Init(void){
    SysTick->LOAD = (uint32_t)hclk / 1000;
    SysTick->VAL  = 0;   // Clear counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		NVIC_SetPriority (SysTick_IRQn, SYSTICK_PRIORITY_LV);
    NVIC_EnableIRQ(SysTick_IRQn);
}
