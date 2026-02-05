/******************************************************************************
 * @Filename     : timerA.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/09/2022 14:45:48
 * @Modified by  : CB
 *
 * @Brief        : TimerA function source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

void TMRA_Reset(void)
{
    Peripheral_Reset(TMRA_RST_Msk);
}

void TMRA_Clear(void)
{
    TMRA->TACTL |= TMRA_TACLR_Msk;
}

void TMRA_Timer_Cfg(tmra_config_t *config)
{
    TMRA->TACTL = (config->div | config->exdiv | config->mc);
    TMRA->TACCR0 = config->count;
    if (config->cci_en == 1) TMRA_CCI_ENABLE;
    else TMRA_CCI_DISABLE;
    if (config->tai_en == 1) TMRA_TAI_ENABLE;
    else TMRA_TAI_DISABLE;
    if (config->cci_dma_en == 1) TMRA_CCI0_DMA_ENABLE;
    else TMRA_CCI0_DMA_DISABLE;
    if (config->tai_dma_en == 1) TMRA_TAI_DMA_ENABLE;
    else TMRA_TAI_DMA_DISABLE;
}

uint16_t TMRA_PWM_AutoDead_Cfg(pwm_config_t *config)
{
    if (config->ch_num >= TMRA_CHANNEL_MAX)
        return TMRA_INVALID_PARA;

    TMRA->TACTL = (config->div | config->exdiv | TIM_UP_DOWN);
    TMRA->TACCR0 = config->pwm_ccr0;

    if ((config->ch_num == TMRA_CHANNEL1) || (config->ch_num == TMRA_CHANNEL4))
    {
        if (config->pwm_ccr4 > TMRA_MAX_DEADTIME)
            return TMRA_INVALID_PARA;
        TMRA->TACCR1 = config->pwm_ccr1;
        TMRA->TACCR4 = config->pwm_ccr4;
        TMRA->TACCTL1 = OUT_TOGGLE;
        TMRA->TACCTL4 = (TMRA_DEADE_Msk | config->pwm_p_cfg | config->pwm_n_cfg);
    }
    else if ((config->ch_num == TMRA_CHANNEL2) || (config->ch_num == TMRA_CHANNEL5))
    {
        if (config->pwm_ccr5 > TMRA_MAX_DEADTIME)
            return TMRA_INVALID_PARA;
        TMRA->TACCR2 = config->pwm_ccr2;
        TMRA->TACCR5 = config->pwm_ccr5;
        TMRA->TACCTL2 = OUT_TOGGLE;
        TMRA->TACCTL5 = (TMRA_DEADE_Msk | config->pwm_p_cfg | config->pwm_n_cfg);
    }
    else if (((config->ch_num == TMRA_CHANNEL3) || (config->ch_num == TMRA_CHANNEL6)))
    {
        if (config->pwm_ccr6 > TMRA_MAX_DEADTIME)
            return TMRA_INVALID_PARA;
        TMRA->TACCR3 = config->pwm_ccr3;
        TMRA->TACCR6 = config->pwm_ccr6;
        TMRA->TACCTL3 = OUT_TOGGLE;
        TMRA->TACCTL6 = (TMRA_DEADE_Msk | config->pwm_p_cfg | config->pwm_n_cfg);
    }
    else {}
    return OK;
}

uint16_t TMRA_PWM_Cfg(pwm_config_t *config)
{
    if (config->ch_num >= TMRA_CHANNEL_MAX)
        return TMRA_INVALID_PARA;

    TMRA->TACTL = (config->div | config->exdiv | config->mc);
    TMRA->TACCR0 = config->pwm_ccr0;

    if (config->ch_num == TMRA_CHANNEL0)
        TMRA->TACCTL0 = config->pwm_mod;
    else if (config->ch_num == TMRA_CHANNEL1)
    {
        TMRA->TACCTL1 = config->pwm_mod;
        TMRA->TACCR1 = config->pwm_ccr1;
    }
    else if (config->ch_num == TMRA_CHANNEL2)
    {
        TMRA->TACCTL2 = config->pwm_mod;
        TMRA->TACCR2 = config->pwm_ccr2;
    }
    else if (config->ch_num == TMRA_CHANNEL3)
    {
        TMRA->TACCTL3 = config->pwm_mod;
        TMRA->TACCR3 = config->pwm_ccr3;
    }
    else if (config->ch_num == TMRA_CHANNEL4)
    {
        TMRA->TACCTL4 = config->pwm_mod;
        TMRA->TACCR4 = config->pwm_ccr4;
    }
    else if (config->ch_num == TMRA_CHANNEL5)
    {
        TMRA->TACCTL5 = config->pwm_mod;
        TMRA->TACCR5 = config->pwm_ccr5;
    }
    else if (config->ch_num == TMRA_CHANNEL6)
    {
        TMRA->TACCTL6 = config->pwm_mod;
        TMRA->TACCR6 = config->pwm_ccr6;
    }
    else {}
    return OK;
}

uint16_t TMRA_Capture_Cfg(capture_config_t *config)
{
    if (config->ch_num >= TMRA_CHANNEL_MAX)
        return TMRA_INVALID_PARA;

    TMRA->TACTL = (config->div | config->exdiv | TIM_CONTINUE);

    if (config->ch_num == TMRA_CHANNEL0)
    {
        TMRA->TACCTL0 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI0_ENABLE;
        else TMRA_CCI0_DISABLE;
        if (config->dma_en == 1) TMRA_CCI0_DMA_ENABLE;
        else TMRA_CCI0_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL1)
    {
        TMRA->TACCTL1 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI1_ENABLE;
        else TMRA_CCI1_DISABLE;
        if (config->dma_en == 1) TMRA_CCI1_DMA_ENABLE;
        else TMRA_CCI1_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL2)
    {
        TMRA->TACCTL2 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI2_ENABLE;
        else TMRA_CCI2_DISABLE;
        if (config->dma_en == 1) TMRA_CCI2_DMA_ENABLE;
        else TMRA_CCI2_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL3)
    {
        TMRA->TACCTL3 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI3_ENABLE;
        else TMRA_CCI3_DISABLE;
        if (config->dma_en == 1) TMRA_CCI3_DMA_ENABLE;
        else TMRA_CCI3_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL4)
    {
        TMRA->TACCTL4 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI4_ENABLE;
        else TMRA_CCI4_DISABLE;
        if (config->dma_en == 1) TMRA_CCI4_DMA_ENABLE;
        else TMRA_CCI4_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL5)
    {
        TMRA->TACCTL5 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI5_ENABLE;
        else TMRA_CCI5_DISABLE;
        if (config->dma_en == 1) TMRA_CCI5_DMA_ENABLE;
        else TMRA_CCI5_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL6)
    {
        TMRA->TACCTL6 = (config->cap_input | config->cap_edge | CAPTURE_MODE);
        if (config->cci_en == 1) TMRA_CCI6_ENABLE;
        else TMRA_CCI6_DISABLE;
        if (config->dma_en == 1) TMRA_CCI6_DMA_ENABLE;
        else TMRA_CCI6_DMA_DISABLE;
    }
    else {}
    return OK;
}

uint16_t TMRA_Compare_Cfg(compare_config_t *config)
{
    if (config->ch_num >= TMRA_CHANNEL_MAX)
        return TMRA_INVALID_PARA;

    TMRA->TACTL = (config->div | config->exdiv | config->mc);
    TMRA->TACCR0 = config->cap_ccr0;

    if (config->ch_num == TMRA_CHANNEL0)
    {
        TMRA->TACCTL0 = (config->cap_input | COMPARE_MODE);
        if (config->cci_en == 1) TMRA_CCI0_ENABLE;
        else TMRA_CCI0_DISABLE;
        if (config->dma_en == 1) TMRA_CCI0_DMA_ENABLE;
        else TMRA_CCI0_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL1)
    {
        TMRA->TACCTL1 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR1 = config->cap_ccr1;
        if (config->cci_en == 1) TMRA_CCI1_ENABLE;
        else TMRA_CCI1_DISABLE;
        if (config->dma_en == 1) TMRA_CCI1_DMA_ENABLE;
        else TMRA_CCI1_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL2)
    {
        TMRA->TACCTL2 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR2 = config->cap_ccr2;
        if (config->cci_en == 1) TMRA_CCI2_ENABLE;
        else TMRA_CCI2_DISABLE;
        if (config->dma_en == 1) TMRA_CCI2_DMA_ENABLE;
        else TMRA_CCI2_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL3)
    {
        TMRA->TACCTL3 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR3 = config->cap_ccr3;
        if (config->cci_en == 1) TMRA_CCI3_ENABLE;
        else TMRA_CCI3_DISABLE;
        if (config->dma_en == 1) TMRA_CCI3_DMA_ENABLE;
        else TMRA_CCI3_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL4)
    {
        TMRA->TACCTL4 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR4 = config->cap_ccr4;
        if (config->cci_en == 1) TMRA_CCI4_ENABLE;
        else TMRA_CCI4_DISABLE;
        if (config->dma_en == 1) TMRA_CCI4_DMA_ENABLE;
        else TMRA_CCI4_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL5)
    {
        TMRA->TACCTL5 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR5 = config->cap_ccr5;
        if (config->cci_en == 1) TMRA_CCI5_ENABLE;
        else TMRA_CCI5_DISABLE;
        if (config->dma_en == 1) TMRA_CCI5_DMA_ENABLE;
        else TMRA_CCI5_DMA_DISABLE;
    }
    else if (config->ch_num == TMRA_CHANNEL6)
    {
        TMRA->TACCTL6 = (config->cap_input | COMPARE_MODE);
        TMRA->TACCR6 = config->cap_ccr6;
        if (config->cci_en == 1) TMRA_CCI6_ENABLE;
        else TMRA_CCI6_DISABLE;
        if (config->dma_en == 1) TMRA_CCI6_DMA_ENABLE;
        else TMRA_CCI6_DMA_DISABLE;
    }
    else {}
    return OK;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
