#include "hal_RTC.h"
#include "rtc.h"
#include "bsp_UART.h"
#include "hal_DELAY.h"
uint32_t pmod0,pmod2,sysClkVal;
void Pre_Config(void)
{
		pmod0=GPIO0->PMODE;
		pmod2=GPIO2->PMODE;
	  sysClkVal=SYSC->CLK_EN;
		GPIO0->PMODE=0;
//		GPIO1->PMODE=0;
		GPIO2->PMODE=0;
//		GPIO3->PMODE=0;
//    LXTAL_DISABLE;
		PLL_DISABLE;
		HXTAL_DISABLE;
    ALL_IOLATCH_ENABLE;
    SYSC->CLK_EN = 0x0;
    SYSC->CLK_EN |= BIT16;//always_on_pclk_en
}
void After_Config(void){
		SYSC->CLK_EN=sysClkVal;//1E218141
		ALL_IOLATCH_DISABLE;
		GPIO0->PMODE=pmod0;
		GPIO2->PMODE=pmod2;
//	printf("testWakeUp\n");
}
void DeepSleep_Config(void){
	Pre_Config();
	Deep_Sleep();
	After_Config();
//	Cnt_Delay_ms(100);
}
void HAL_RTC_Init(void){
	CLR_WKUP_MASK2(AON_WKU_RTC_MASK_Msk);
	RTC_Enable(SOURCE_RC32K);
	#if 0 //test RTC wakeup

	 Cnt_Delay_ms(5000);
	
//	Power_Down();
	while(1){
		DeepSleep_Config();
		printf("DeepSleep\n");
		Cnt_Delay_ms(1000);
	}
	#endif
}










