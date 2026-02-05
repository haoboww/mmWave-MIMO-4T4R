/******************************************************************************
 * @Filename     : timerB.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/18/2022 15:59:57
 * @Modified by  : CB
 *
 * @Brief        : timerB driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __TMRB_H__
#define __TMRB_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define TMRB_CLK             pclk
#define USER_MODE            1
#define FREE_MODE            0
#define TMR0                 0
#define TMR1                 1
#define TMR2                 2
#define TMR3                 3
#define TMR01_MAX            0xffffffff
#define TMR23_MAX            0xffff

#define TMR0_ENABLE          (TMRB->T0CTL |= TMRB_EN_Msk)
#define TMR0_DISABLE         (TMRB->T0CTL &= (~TMRB_EN_Msk))
#define TMR0_USER_MODE       (TMRB->T0CTL |= TMRB_MOD_Msk)
#define TMR0_FREE_MODE       (TMRB->T0CTL &= (~TMRB_MOD_Msk))
#define TMR0_INTR_MASK       (TMRB->T0CTL |= TMRB_IMK_Msk)
#define TMR0_INTR_UNMASK     (TMRB->T0CTL &= (~TMRB_IMK_Msk))
#define TMR0_CLR_INTR        (TMRB->T0EOI)
#define TMR0_GET_CV          (TMRB->T0CV)
#define TMR0_GET_INT_FLAG    (TMRB->T0ISR)

#define TMR1_ENABLE          (TMRB->T1CTL |= TMRB_EN_Msk)
#define TMR1_DISABLE         (TMRB->T1CTL &= (~TMRB_EN_Msk))
#define TMR1_USER_MODE       (TMRB->T1CTL |= TMRB_MOD_Msk)
#define TMR1_FREE_MODE       (TMRB->T1CTL &= (~TMRB_MOD_Msk))
#define TMR1_INTR_MASK       (TMRB->T1CTL |= TMRB_IMK_Msk)
#define TMR1_INTR_UNMASK     (TMRB->T1CTL &= (~TMRB_IMK_Msk))
#define TMR1_CLR_INTR        (TMRB->T1EOI)
#define TMR1_GET_CV          (TMRB->T1CV)
#define TMR1_GET_INT_FLAG    (TMRB->T1ISR)

#define TMR2_ENABLE          (TMRB->T2CTL |= TMRB_EN_Msk)
#define TMR2_DISABLE         (TMRB->T2CTL &= (~TMRB_EN_Msk))
#define TMR2_USER_MODE       (TMRB->T2CTL |= TMRB_MOD_Msk)
#define TMR2_FREE_MODE       (TMRB->T2CTL &= (~TMRB_MOD_Msk))
#define TMR2_INTR_MASK       (TMRB->T2CTL |= TMRB_IMK_Msk)
#define TMR2_INTR_UNMASK     (TMRB->T2CTL &= (~TMRB_IMK_Msk))
#define TMR2_CLR_INTR        (TMRB->T2EOI)
#define TMR2_GET_CV          (TMRB->T2CV)
#define TMR2_GET_INT_FLAG    (TMRB->T2ISR)

#define TMR3_ENABLE          (TMRB->T3CTL |= TMRB_EN_Msk)
#define TMR3_DISABLE         (TMRB->T3CTL &= (~TMRB_EN_Msk))
#define TMR3_USER_MODE       (TMRB->T3CTL |= TMRB_MOD_Msk)
#define TMR3_FREE_MODE       (TMRB->T3CTL &= (~TMRB_MOD_Msk))
#define TMR3_INTR_MASK       (TMRB->T3CTL |= TMRB_IMK_Msk)
#define TMR3_INTR_UNMASK     (TMRB->T3CTL &= (~TMRB_IMK_Msk))
#define TMR3_CLR_INTR        (TMRB->T3EOI)
#define TMR3_GET_CV          (TMRB->T3CV)
#define TMR3_GET_INT_FLAG    (TMRB->T3ISR)

#define TMRS_CLR_INTR        (TMRBS->TSEOI)

uint32_t TMRB_Open(uint8_t u8TimerNum, uint8_t u8Mode, uint32_t u32Freq);
void TMRB_Close(uint8_t u8TimerNum);
uint8_t TMRB_mDelay(uint8_t u8TimerNum, uint32_t u32Msec);
uint8_t TMRB_uDelay(uint8_t u8TimerNum, uint32_t u32Usec);
uint8_t TMRB_mTimeout(uint8_t u8TimerNum, uint32_t u32Msec);
void TMRB_Reset(void);

#ifdef __cplusplus
}
#endif
#endif

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
