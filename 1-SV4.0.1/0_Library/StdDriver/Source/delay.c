/******************************************************************************
 * @Filename     : delay.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 14:45:48
 * @Modified by  : MXY
 *
 * @Brief        : Delay function source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include "SKY32B750.h"

/**
 * @Breif  : HW_Delay_us : delay n microsecodes use systick
 *
 * @Param  : u32Cnt: delay time, max value 500000
 */
void HW_Delay_us(uint32_t u32Cnt)
{
    SysTick->LOAD = u32Cnt * (hclk / 1000000);
    SysTick->VAL  = 0;                          // Clear counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //disable systick
}

/**
 * @Breif  : HW_Delay_ms : delay n milliseconds use systick
 *
 * @Param  : u32Cnt: delay time, max value 500
 */
void HW_Delay_ms(uint32_t u32Cnt)
{
    uint32_t load_val;
    load_val = u32Cnt * (hclk / 1000);
    if (load_val <= 0xffffff)
        SysTick->LOAD = u32Cnt * (hclk / 1000);
    else
        SysTick->LOAD = 0xffffff;                // load max value
    SysTick->VAL  = 0;                           // Clear counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //disable systick
}

uint32_t Calib_SW_ms(void)
{
    uint32_t i = 0;
    SysTick->LOAD = 1* (hclk / 1000); //128ms
    SysTick->VAL  = 0;                          // Clear counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
    {
        i++;
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //disable systick
    ms_step = ((i * 5) >> 2);
    return ms_step;
}

/**
 * @Breif  : SW_Delay_us : delay n mircoseconds use software
 *
 * @Param  : u32Cnt: delay time
 */
void SW_Delay_us(uint32_t u32Cnt)
{
    uint32_t i, j;
    for (j = 0; j < u32Cnt; j++)
    {
        for(i = 0; i < 40; i++);
    }
}

/**
 * @Breif  : SW_Delay_ms : delay n milliseconds use software
 *
 * @Param  : u32Cnt: delay time
 */
void SW_Delay_ms(uint32_t u32Cnt)
{
    uint32_t i;
    uint32_t j;
    for (j = 0; j < u32Cnt; j++)
    {
        for (i = 0; i < ms_step; i++);
    }
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
