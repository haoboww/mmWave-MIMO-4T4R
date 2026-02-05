/******************************************************************************
 * @Filename     : timerB.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/18/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : timerB driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/* =============================================================================*/
/**
 * @Breif  : TMRB_Open: This API is used to configure quad timer to operate in specified mode
 *           and frequency. a closest frequency will be chose and returned.
 *
 * @Param  : u8TimerNum: quad timer number (TMR0~TMR3)
 * @Param  : u8Mode: operation mode. Possible options are
 *                   -- USER_MODE
 *                   -- FREE_MODE
 * @Param  : u32Freq: Target generator frequency.
 *
 * @Returns: Nearest frequency clock, if return 0, the Frequency is out of valid range
 * @note:
 */
/* =============================================================================*/
uint32_t TMRB_Open(uint8_t u8TimerNum, uint8_t u8Mode, uint32_t u32Freq)
{
    uint32_t u32Cnt;
    if (u32Freq > TMRB_CLK)
        return FALSE;
    else
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        u32Cnt = TMRB_CLK / u32Freq;
        switch (u8TimerNum)
        {
        case 0:
        {
            TMR0_DISABLE;
            if (u8Mode)
                TMR0_USER_MODE;
            else
                TMR0_FREE_MODE;
            TMRB->T0LC = u32Cnt;
            TMR0_ENABLE;
            return u32Cnt;
        }
        case 1:
        {
            TMR1_DISABLE;
            if (u8Mode)
                TMR1_USER_MODE;
            else
                TMR1_FREE_MODE;
            TMRB->T1LC = u32Cnt;
            TMR1_ENABLE;
            return u32Cnt;
        }
        case 2:
        {
            if (u32Cnt > TMR23_MAX)
                return FALSE;
            TMR2_DISABLE;
            if (u8Mode)
                TMR2_USER_MODE;
            else
                TMR2_FREE_MODE;
            TMRB->T2LC = u32Cnt;
            TMR2_ENABLE;
            return u32Cnt;
        }
        case 3:
        {
            if (u32Cnt > TMR23_MAX)
                return FALSE;
            TMR3_DISABLE;
            if (u8Mode)
                TMR3_USER_MODE;
            else
                TMR3_FREE_MODE;
            TMRB->T3LC = u32Cnt;
            TMR3_ENABLE;
            return u32Cnt;
        }
        default: return FALSE;
        }
    }
}

/* =============================================================================*/
/**
 * @Breif  : TMRB_Close: This API stops quad timer counting
 *
 * @Param  : u8TimerNum: quad timer number (TMR0~TMR3)
 *
 * @Returns: NONE
 */
/* =============================================================================*/
void TMRB_Close(uint8_t u8TimerNum)
{
    switch (u8TimerNum)
    {
    case 0:
        TMR0_DISABLE;
        break;
    case 1:
        TMR1_DISABLE;
        break;
    case 2:
        TMR2_DISABLE;
        break;
    case 3:
        TMR3_DISABLE;
        break;
    }
}

/* =============================================================================*/
/**
 * @Breif  : TMRB_mDelay: This API is used to create a delay loop for u32Msec
 *                          milliseconds. Use free-running mode
 *
 * @Param  : u8TimerNum: quad timer number (TMR0~TMR3)
 * @Param  : u32Msec: delay time (1 millisecond every step)
 *
 * @Returns: NONE
 *
 * @Note   : qtimer 0 and 1 maximum duration 858993 ms
 *           qtimer 2 and 3 maximum duration 13 ms
 */
/* =============================================================================*/
uint8_t TMRB_mDelay(uint8_t u8TimerNum, uint32_t u32Msec)
{
    uint32_t u32Mstep;
    uint64_t u64Cnt;
    u32Mstep = TMRB_CLK / 1000;   //1ms count value
    u64Cnt = u32Msec * u32Mstep;
    if (u64Cnt > TMR01_MAX)
        return FALSE;
    switch (u8TimerNum)
    {
    case 0:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR0_DISABLE;
        TMR0_FREE_MODE;
        TMRB->T0LC = u64Cnt;
        TMR0_ENABLE;
        while (!TMRB->T0ISR);
        TMR0_DISABLE;
    }
    return u32Msec;
    case 1:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR1_DISABLE;
        TMR1_FREE_MODE;
        TMRB->T1LC = u64Cnt;
        TMR1_ENABLE;
        while (!TMRB->T1ISR);
        TMR1_DISABLE;
    }
    return u32Msec;
    case 2:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR2_DISABLE;
        TMR2_FREE_MODE;
        TMRB->T2LC = u64Cnt;
        TMR2_ENABLE;
        while (!TMRB->T2ISR);
        TMR2_DISABLE;
    }
    return u32Msec;
    case 3:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR3_DISABLE;
        TMR3_FREE_MODE;
        TMRB->T3LC = u64Cnt;
        TMR3_ENABLE;
        while (!TMRB->T3ISR);
        TMR3_DISABLE;
    }
    return u32Msec;
    default: return FALSE;
    }
}

/* =============================================================================*/
/**
 * @Breif  : TMRB_uDelay: This API is used to create a delay loop for u32Usec
 *                          micro seconds. Use free-running mode
 *
 * @Param  : u8TimerNum: quad timer number (TMR0~TMR3)
 * @Param  : u32Usec: delay time (1 micro second every step)
 *
 * @Returns: NONE
 */
/* =============================================================================*/
uint8_t TMRB_uDelay(uint8_t u8TimerNum, uint32_t u32Usec)
{
    uint32_t u32Ustep;
    uint64_t u64Cnt;
    u32Ustep = TMRB_CLK / 1000000;   //1us count value
    u64Cnt = u32Usec * u32Ustep;
    if (u64Cnt > TMR01_MAX)
        return FALSE;
    switch (u8TimerNum)
    {
    case 0:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR0_DISABLE;
        TMR0_FREE_MODE;
        TMRB->T0LC = u64Cnt;
        TMR0_ENABLE;
        while (!TMRB->T0ISR);
        TMR0_DISABLE;
    }
    return u32Usec;
    case 1:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR1_DISABLE;
        TMR1_FREE_MODE;
        TMRB->T1LC = u64Cnt;
        TMR1_ENABLE;
        while (!TMRB->T1ISR);
        TMR1_DISABLE;
    }
    return u32Usec;
    case 2:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR2_DISABLE;
        TMR2_FREE_MODE;
        TMRB->T2LC = u64Cnt;
        TMR2_ENABLE;
        while (!TMRB->T2ISR);
        TMR2_DISABLE;
    }
    return u32Usec;
    case 3:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR3_DISABLE;
        TMR3_FREE_MODE;
        TMRB->T3LC = u64Cnt;
        TMR3_ENABLE;
        while (!TMRB->T3ISR);
        TMR3_DISABLE;
    }
    return u32Usec;
    default: return FALSE;
    }
}

/**
 * @brief     TMRB_mTimeout: This API is used to set a timeout for n milliseconds
 * @DateTime  2022-05-18T17:35:33+0800
 * @param     u8TimerNum                timerB number (TMR0~TMR3)
 * @param     u32Msec                   timeout time (1 millisecond every step)
 * @return                              timeout time
 */
uint8_t TMRB_mTimeout(uint8_t u8TimerNum, uint32_t u32Msec)
{
    uint32_t u32Mstep;
    uint64_t u64Cnt;
    u32Mstep = TMRB_CLK / 1000;
    u64Cnt = u32Msec * u32Mstep;
    if (u64Cnt > TMR01_MAX)
        return FALSE;
    switch (u8TimerNum)
    {
    case 0:
    {
        TMRB_CLK_ENABLE;  //enable wqtimer clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB0_IRQn); //enable cpu quad timer0 interrupt
        TMR0_DISABLE;
        TMR0_USER_MODE;
        TMRB->T0LC = u64Cnt;
        TMR0_ENABLE;
    }
    return u32Msec;
    case 1:
    {
        TMRB_CLK_ENABLE;  //enable wqtimer clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB1_IRQn); //enable cpu quad timer1 interrupt
        TMR1_DISABLE;
        TMR1_USER_MODE;
        TMRB->T1LC = u64Cnt;
        TMR1_ENABLE;
    }
    return u32Msec;
    case 2:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable wqtimer clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB2_IRQn); //enable cpu quad timer2 interrupt
        TMR2_DISABLE;
        TMR2_USER_MODE;
        TMRB->T2LC = u64Cnt;
        TMR2_ENABLE;
    }
    return u32Msec;
    case 3:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_CLK_ENABLE;  //enable wqtimer clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB3_IRQn); //enable cpu quad timer3 interrupt
        TMR3_DISABLE;
        TMR3_USER_MODE;
        TMRB->T3LC = u64Cnt;
        TMR3_ENABLE;
    }
    return u32Msec;
    default: return FALSE;
    }
}

/**
 * @brief     TMRB_uTimeout: This API is used to set a timeout for n microseconds
 * @DateTime  2022-05-18T17:34:13+0800
 * @param     u8TimerNum                timerB number (TMR0~TRM3)
 * @param     u32Usec                   timeout time (1 microsecond every step)
 * @return                              timeout time
 */
uint8_t TMRB_uTimeout(uint8_t u8TimerNum, uint32_t u32Usec)
{
    uint32_t u32Ustep;
    uint64_t u64Cnt;
    u32Ustep = TMRB_CLK / 1000000;
    u64Cnt = u32Usec * u32Ustep;
    if (u64Cnt > TMR01_MAX)
        return FALSE;
    switch (u8TimerNum)
    {
    case 0:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB0_IRQn); //enable cpu quad timer0 interrupt
        TMR0_DISABLE;
        TMR0_USER_MODE;
        TMRB->T0LC = u64Cnt;
        TMR0_ENABLE;
    }
    return u32Usec;
    case 1:
    {
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB1_IRQn); //enable cpu quad timer0 interrupt
        TMR1_DISABLE;
        TMR1_USER_MODE;
        TMRB->T1LC = u64Cnt;
        TMR1_ENABLE;
    }
    return u32Usec;
    case 2:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB2_IRQn); //enable cpu quad timer0 interrupt
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR2_DISABLE;
        TMR2_USER_MODE;
        TMRB->T2LC = u64Cnt;
        TMR2_ENABLE;
    }
    return u32Usec;
    case 3:
    {
        if (u64Cnt > TMR23_MAX)
            return FALSE;
        TMRB_Reset();
        NVIC_EnableIRQ(TMRB3_IRQn); //enable cpu quad timer0 interrupt
        TMRB_CLK_ENABLE;  //enable timerB clock
        TMR3_DISABLE;
        TMR3_USER_MODE;
        TMRB->T3LC = u64Cnt;
        TMR3_ENABLE;
    }
    return u32Usec;
    default: return FALSE;
    }
}

/**
 * @brief     TMRB_Reset : reset timerB
 * @DateTime  2022-05-18T17:33:49+0800
 */
void TMRB_Reset(void)
{
    Peripheral_Reset(TMRB_RST_Msk);
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
