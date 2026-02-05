/******************************************************************************
 * @Filename     : wdt.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 13:26:03
 * @Modified by  : CB
 *
 * @Brief        : WDT driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include "SKY32B750.h"

/**
 * @brief     WDT_Enable: Enable WDT Clock
 * @DateTime  2022-05-09T16:20:32+0800
 */
void WDT_Enable(void)
{
    //wdt and rtc disable
    if (AON_SYSC->LRC_EN == PD_LRC_ALL_PATTERN)
    {
        //wdt enable only
        RTC_DISABLE;
    }
    //wdt disable
    else if (AON_SYSC->LRC_EN == PD_LRC_WDT_PATTERN)
    {
        //wdt and rtc enable
        LRC_ENABLE;
    }
    //wdt enable
    else {}
}

/**
 * @brief     WDT_Disable: Disable WDT Clock
 * @DateTime  2022-05-09T16:21:46+0800
 */
void WDT_Disable(void)
{
    //rtc and wdt disable
    if (AON_SYSC->LRC_EN == PD_LRC_ALL_PATTERN) {}
    //rtc disable only
    else if (AON_SYSC->LRC_EN == PD_LRC_RTC_PATTERN)
    {
        //rtc and wdt disable
        AON_SYSC->LRC_EN = PD_LRC_ALL_PATTERN;
    }
    //rtc enable
    else
    {
        //wdt disable only
        AON_SYSC->LRC_EN = PD_LRC_WDT_PATTERN;
    }
}

/**
 * @brief     WDT_Reset: Set comparator counter to maximum and Disable WDT clock
 * @DateTime  2022-05-09T16:22:17+0800
 */
void WDT_Reset(void)
{
    WDT->STR = 0xF;
    WDT->CMP = 0xFFFF;
    WDT_Disable();
}

/**
 * @brief     WDT_Open: This function config WDT module working mode and count n*16 step
 * @DateTime  2022-05-09T16:23:20+0800
 * @param     u16comparator             comparator count
 */
void WDT_Open(uint16_t u16comparator)
{
    WDT_Enable();
    WDT->STR = 0x3;
    WDT->CMP = u16comparator;
    WDT_CLEAR_COUNTER;
}

/**
 * @Breif  : WDT_Set_ms    : This function config WDT module working mode and count n ms
 *
 * @Param  : u16Msec : count time (1 millisecond every step)
 */
void WDT_Set_ms(uint16_t u16Msec)
{
    uint32_t val;
    val = (u16Msec * lrc_clk) / 16000;
    WDT_Open((uint16_t)val);
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
