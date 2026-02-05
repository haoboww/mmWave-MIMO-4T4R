/******************************************************************************
 * @Filename     : rtc.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/20/2022 13:26:03
 * @Modified by  : CB
 *
 * @Brief        : RTC driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include "SKY32B750.h"

uint32_t rtc_clk = XTAL_32K;

/**
 * @brief     RTC_Enable: Enable RTC
 * @DateTime  2022-06-20T09:45:29+0800
 * @param     u8ClkSrc                  RTC clock source select: SOURCE_RC32K or SOURCE_OSC32K
 * @return                              RTC_INVALID_PARA
 *                                      OK
 */
uint16_t RTC_Enable(uint8_t u8ClkSrc)
{
    if (u8ClkSrc == SOURCE_RC32K)
    {
        RTC_SELECT_LRC32K;
        //wdt and rtc disable
        if (AON_SYSC->LRC_EN == PD_LRC_ALL_PATTERN)
        {
            //rtc enable only
            WDT_DISABLE;
        }
        //rtc disable
        else if (AON_SYSC->LRC_EN == PD_LRC_RTC_PATTERN)
        {
            //wdt and rtc enable
            LRC_ENABLE;
        }
        //rtc enable
        else {}
        rtc_clk = lrc_clk;
        return OK;
    }
    else if (u8ClkSrc == SOURCE_OSC32K)
    {
        PORT01_PIN_CTRL->AE |= BIT18;    //Port1.2
        PORT01_PIN_CTRL->AE |= BIT19;    //Port1.3
        RTC_SELECT_OSC32K;
        rtc_clk = XTAL_32K;
        return OK;
    }
    else
    {
        return RTC_INVALID_PARA;
    }
}

/**
 * @brief     RTC_Disable：Disable RTC
 * @DateTime  2022-06-20T09:45:06+0800
 */
void RTC_Disable(void)
{
    RTC_SELECT_LRC32K;
    //rtc and wdt disable
    if (AON_SYSC->LRC_EN == PD_LRC_ALL_PATTERN) {}
    //wdt disable
    else if (AON_SYSC->LRC_EN == PD_LRC_WDT_PATTERN)
    {
        //rtc and wdt disable
        AON_SYSC->LRC_EN = PD_LRC_ALL_PATTERN;
    }
    //wdt enable
    else
    {
        //rtc disable only
        AON_SYSC->LRC_EN = PD_LRC_RTC_PATTERN;
    }
}

/**
 * @brief     RTC_Get_Current_Value: Get RTC current count value
 * @DateTime  2022-06-20T09:43:29+0800
 * @param     pval                      RTC current count value
 */
void RTC_Get_Current_Value(uint8_t *pval)
{
    pval[0] = RTC->CNT0;
    pval[1] = RTC->CNT1;
}

/**
 * @brief     RTC_Reset: Reset and disable RTC
 * @DateTime  2022-06-20T09:42:58+0800
 */
void RTC_Reset(void)
{
    RTC->WKU_CMP = 0xFFFF;
    RTC->PUS_CFG = 0x00;
    RTC->WKU_CFG = 0x00;
    RTC_CLEAR_COUNTER;
    RTC_Disable();
}

/**
 * @brief     RTC_Wakeup_ms: Set RTC wakeup time
 * @DateTime  2022-06-20T09:40:30+0800
 * @param     u32Msec                   RTC wakeup time (units are milliseconds)
 * @param     u8Mode                    RTC wkaeup mode: ONCE_MODE or REPEAT_MODE
 */
void RTC_Wakeup_ms(uint32_t u32Msec, uint8_t u8Mode)
{
    uint32_t val;
    val = (u32Msec * rtc_clk) / 16000;
    RTC->WKU_CMP = (uint16_t)val;
    RTC->WKU_CFG &= (~RTC_CLK_SEL_Msk);

    if (u8Mode == REPEAT_MODE)
    {
        RTC->WKU_CFG |= RTC_WKU_MODE_Msk;
    }
    else
    {
        RTC->WKU_CFG &= (~RTC_WKU_MODE_Msk);
    }
}

/**
 * @brief     RTC_Wakeup_sec: Set RTC wakeup time
 * @DateTime  2022-06-20T09:38:08+0800
 * @param     u32sec                    RTC wakeup time (units are seconds)
 * @param     u8Mode                    RTC wkaeup mode: ONCE_MODE or REPEAT_MODE
 */
void RTC_Wakeup_sec(uint32_t u32sec, uint8_t u8Mode)
{
    uint32_t val;
    val = (((u32sec * rtc_clk * 1000) / 16000 ) / 1024);
    RTC->WKU_CMP = (uint16_t)val;
    RTC->WKU_CFG |= RTC_CLK_SEL_Msk;
    if (u8Mode == REPEAT_MODE)
    {
        RTC->WKU_CFG |= RTC_WKU_MODE_Msk;
    }
    else
    {
        RTC->WKU_CFG &= (~RTC_WKU_MODE_Msk);
    }
}

/**
 * @brief     RTC_Pulse_Config: Configuration RTC pulse waveform
 * @DateTime  2022-06-20T09:28:34+0800
 * @param     u8Inv                     RTC pulse level invert: 0 normal, 1 invert
 * @param     u8LowTime                 RTC pulse low time
 * @param     u8HighTime                RTC pulse high time
 */
void RTC_Pulse_Config(uint8_t u8Inv, uint8_t u8LowTime, uint8_t u8HighTime)
{
    RTC->PUS_CFG = RTC_PUS_EN_Msk | ((u8Inv & 0x1) << RTC_PUS_INV_Pos) | \
                   ((u8LowTime & 0xF) << RTC_PUS_LOW_Pos) | \
                   ((u8HighTime & 0x3) << RTC_PUS_HIGH_Pos);
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
