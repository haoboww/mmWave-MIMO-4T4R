/******************************************************************************
 * @Filename     : pmu.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 01/26/2021 9:12:34
 * @Modified by  : MXY
 *
 * @Brief        :
 *
 * Copyright (C) 2021 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"


volatile uint32_t WKU_FLAG_sta0 = 0;
volatile uint32_t WKU_FLAG_sta1 = 0;
volatile uint32_t WKU_FLAG_sta2 = 0;

/**
 * @Breif  : Deep_Sleep: Core and peripherals are all sleep, all clocks are gated
 */
void Deep_Sleep(void)
{
    AON_SYSC->SWITCH |= AON_DEEP_MODE_Msk;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}


/**
 * @Breif  : Power_Down: 1.8V logics are in power down state
 */
void Power_Down(void)
{
//    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
//    __WFI();
}

/**
 * @Breif  : CPU_Sleep: Core is sleep
 */
void CPU_Sleep(void)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}


void NMI_Handler (void)
{
    WKU_FLAG_sta0 = AON_WKU->WKU_FLAG0;
      WKU_FLAG_sta1 = AON_WKU->WKU_FLAG1;
        WKU_FLAG_sta2 = AON_WKU->WKU_FLAG2;

    AON_WKU->WKU_FLAG0 = WKU_FLAG_sta0;
    AON_WKU->WKU_FLAG1 = WKU_FLAG_sta1;
    AON_WKU->WKU_FLAG2 = WKU_FLAG_sta2;

      AON_WKU->WKUP_MASK0 = 0xFFFFFFFF;
      AON_WKU->WKUN_MASK0 = 0xFFFFFFFF;
        AON_WKU->WKUP_MASK1 = 0xFFFFFFFF;
      AON_WKU->WKUN_MASK1 = 0xFFFFFFFF;
        AON_WKU->WKUP_MASK2 = 0xFFFFFFFF;
      AON_WKU->WKUN_MASK2 = 0xFFFFFFFF;
}













