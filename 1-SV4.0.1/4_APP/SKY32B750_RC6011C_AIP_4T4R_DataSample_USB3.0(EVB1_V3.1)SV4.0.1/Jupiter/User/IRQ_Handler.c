#include "IRQ_Handler.h"

volatile uint32_t BB_timeoutCnt=0;
void TMRB0_IRQHandler(void)
{
    TMR0_CLR_INTR;
		BB_timeoutCnt++;
		userTimerB0CallBackHand();
}

uint32_t SysTick_Counter = 0;
extern void xPortSysTickHandler(void);
void SysTick_Handler(void){
	 SysTick_Counter++;
	 if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//
	 {
			xPortSysTickHandler();
	 }
//		printf("sys\n");
}

