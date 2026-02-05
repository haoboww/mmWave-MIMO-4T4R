#include "hal_WDT.h"



void WDT_RstNow(void){

    WDT_Enable();
    WDT->STR = 0x3;                        // bit0:clear
    WDT->CMP = 0;	
    WDT_CLEAR_COUNTER;
}



void WDT_Init(uint32_t val)
{
    WDT_Enable();
    WDT->CMP = (uint16_t)val;
    WDT->STR = 0xF;                    // bit0:clear

}
void Close_Wdt(void)
{
    WDT_CLEAR_COUNTER;

//    NVIC_DisableIRQ(WDT_IRQn);
    WDT_Disable();
}







