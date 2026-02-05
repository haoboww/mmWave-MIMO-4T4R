/******************************************************************************
 * @Filename     : dma.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/07/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : DMA driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

volatile dma_irq_flag_t dma_irq_sta;

/**
 * @Breif  : DMA_Init : Dma Init function
 *           This function is used to initialize the DMA controller. All
 *           interrupts are cleared and disabled; DMA channels are disabled; and
 *           the device instance structure is reset.
 *
 * @Returns: uint8_t        : DMA_INVALID_PARA
 *                          : OK
 */
uint8_t DMA_Init(void)
{
    DMA_Clear_All_Irq();
    DMA_Channel_Irq_Disable(DMA_CHANNEL0);
    DMA_Channel_Irq_Disable(DMA_CHANNEL1);
    DMA_Channel_Irq_Disable(DMA_CHANNEL2);
    DMA_Channel_Irq_Disable(DMA_CHANNEL3);
    DMA_Channel_Irq_Disable(DMA_CHANNEL4);
    DMA_Channel_Irq_Disable(DMA_CHANNEL5);
    DMA_Channel_Irq_Disable(DMA_CHANNEL6);
    DMA_Channel_Irq_Disable(DMA_CHANNEL7);
    DMA_Disable();
    return OK;
}

void DMA_Enable(void)
{
    DMA_EN->DmaCfg = DMA_DMACFG_ENABLE;
}

void DMA_Disable(void)
{
    DMA_EN->DmaCfg = DMA_DMACFG_DISABLE;
    DMA_EN->ChEn = DMA_CHEN_CH_DISABLE;
}

/**
 * @Breif  : DMA_Channel_Configure : Config DMA work function
 *
 * @Param  : ch_num         : DMA channel number
 * @Param  : *dma_config    : pointer to dma config struct
 * @Param  : *callback      : Register DMA finished callback function
 *
 * @Returns: uint8_t        : DMA_INVALID_PARA
 *                          : OK
 */
uint16_t DMA_Channel_Configure(enum dma_channel_number ch_num, dma_channel_config_t *dma_config, void (*callback)(void))
{
    uint32_t channel_ctl = NULL;
    DMA_CH_T *dma_ch = NULL;
    dma_ch = DMA_Num2Ch(ch_num);

    if ((dma_ch == NULL) || (dma_config == NULL) || (ch_num >= DMA_CHANNEL_MAX))
        return DMA_INVALID_PARA;

    dma_ch->SAR = dma_config->sar;
    dma_ch->DAR = dma_config->dar;

    channel_ctl = ((dma_config->ctl_dst_tr_width << DMA_CTL_DST_TR_WIDTH_Pos) & DMA_CTL_DST_TR_WIDTH_Msk) | \
                  ((dma_config->ctl_src_tr_width << DMA_CTL_SRC_TR_WIDTH_Pos) & DMA_CTL_SRC_TR_WIDTH_Msk) | \
                  ((dma_config->ctl_dinc << DMA_CTL_DINC_Pos) & DMA_CTL_DINC_Msk)                         | \
                  ((dma_config->ctl_sinc << DMA_CTL_SINC_Pos) & DMA_CTL_SINC_Msk)                         | \
                  ((dma_config->ctl_dst_msize << DMA_CTL_DEST_MSIZE_Pos) & DMA_CTL_DEST_MSIZE_Msk)        | \
                  ((dma_config->ctl_src_msize << DMA_CTL_SRC_MSIZE_Pos) & DMA_CTL_SRC_MSIZE_Msk)          | \
                  ((dma_config->ctl_tt_fc << DMA_CTL_TT_FC_Pos) & DMA_CTL_TT_FC_Msk)                      | \
                  ((dma_config->ctl_dms << DMA_CTL_DMS_Pos) & DMA_CTL_DMS_Msk)                            | \
                  ((dma_config->ctl_sms << DMA_CTL_SMS_Pos) & DMA_CTL_SMS_Msk);


    dma_ch->CTL = channel_ctl | ((dma_config->ctl_int_en << DMA_CTL_INT_EN_Pos) & DMA_CTL_INT_EN_Msk);
    channel_ctl = NULL;
    channel_ctl = ((dma_config->ctl_block_ts << DMA_HCTL_BLOCK_TS_Pos) & DMA_HCTL_BLOCK_TS_Msk);
    dma_ch->HCTL = channel_ctl;

    dma_ch->CFG = ((dma_config->cfg_ch_prior << DMA_CFG_CH_PRIOR_Pos) & DMA_CFG_CH_PRIOR_Msk)             | \
                  ((dma_config->cfg_reload_src << DMA_CFG_RELOAD_SRC_Pos) & DMA_CFG_RELOAD_SRC_Msk)          | \
                  ((dma_config->cfg_reload_dst << DMA_CFG_RELOAD_DST_Pos) & DMA_CFG_RELOAD_DST_Msk)          | \
                  ((dma_config->cfg_hs_sel_src << DMA_CFG_HS_SEL_SRC_Pos) & DMA_CFG_HS_SEL_SRC_Msk)          | \
                  ((dma_config->cfg_hs_sel_dst << DMA_CFG_HS_SEL_DST_Pos) & DMA_CFG_HS_SEL_DST_Msk)          | \
                  ((dma_config->cfg_src_hs_pol << DMA_CFG_SRC_HS_POL_Pos) & DMA_CFG_SRC_HS_POL_Msk)          | \
                  ((dma_config->cfg_dst_hs_pol << DMA_CFG_DST_HS_POL_Pos) & DMA_CFG_DST_HS_POL_Msk);

    dma_ch->HCFG = ((dma_config->cfg_src_per << DMA_HCFG_SRC_PER_Pos) & DMA_HCFG_SRC_PER_Msk)             | \
                   ((dma_config->cfg_dst_per << DMA_HCFG_DEST_PER_Pos) & DMA_HCFG_DEST_PER_Msk);

    return OK;
}

/**
 * @Breif  : dma_Channel_Eable: This function enables the specified DMA channel(s).
 *
 * @Param  : *dma           : pointer to Dma instance
 * @Param  : ch_num    : DMA channel number
 *
 * @Returns: uint16_t        : DMA_INVALID_PARA
 *                           : OK
 */
uint16_t DMA_Channel_Enable(enum dma_channel_number ch_num)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return DMA_INVALID_PARA;

//EN channel and WE
    DMA_EN->ChEn |= ((DMA_CHEN_CH_WE_ENABLE << ch_num) | DMA_CHEN_CH_ENABLE << ch_num);
    return OK;
}

/**
 * @brief     DMA_Channel_Disable: This function disables the specified DMA channel(s).
 * @DateTime  2022-05-25T15:28:35+0800
 * @param     ch_num               DMA channel number
 * @return    uint16_t                : DMA_INVALID_PARA
 *                                    : OK
 */
uint16_t DMA_Channel_Disable(enum dma_channel_number ch_num)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return DMA_INVALID_PARA;

    DMA_EN->ChEn &= ~((DMA_CHEN_CH_WE_ENABLE << ch_num) | (DMA_CHEN_CH_ENABLE << ch_num));
    return OK;
}

void DMA_Clear_All_Irq(void)
{
    DMA_INT->ClearErr     = 0xFF;
    DMA_INT->ClearBlock   = 0xFF;
    DMA_INT->ClearDstTran = 0xFF;
    DMA_INT->ClearSrcTran = 0xFF;
    DMA_INT->ClearTfr     = 0xFF;
}

uint16_t DMA_Channel_Irq_Enable(enum dma_channel_number ch_num)
{
    DMA_CH_T *dma_ch = NULL;
    dma_ch = DMA_Num2Ch(ch_num);
    if (dma_ch != NULL)
    {
        dma_ch->CTL |= DMA_CTL_INT_EN_Msk;
        return OK;
    }
    else
        return DMA_INVALID_PARA;
}

uint16_t DMA_Channel_Irq_Disable(enum dma_channel_number ch_num)
{
    DMA_CH_T *dma_ch = NULL;
    dma_ch = DMA_Num2Ch(ch_num);
    if (dma_ch != NULL)
    {
        dma_ch->CTL &= ~DMA_CTL_INT_EN_Msk;
        return OK;
    }
    else
        return DMA_INVALID_PARA;
}

bool DMA_Channel_Irq_IsEnable(enum dma_channel_number ch_num)
{
    DMA_CH_T *dma_ch = NULL;
    dma_ch = DMA_Num2Ch(ch_num);
    if (dma_ch != NULL)
    {
        if (dma_ch->CTL & DMA_CTL_INT_EN_Msk) return TRUE;
        else return FALSE;
    }
    else return FALSE;
}

bool DMA_RawIrq_IsActive(enum dma_channel_number ch_num, enum dma_irq ch_irq)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return FALSE;

    if (ch_irq == DMA_IRQ_NONE) return FALSE;
    else if (ch_irq == DMA_IRQ_TFR)
    {
        if (DMA_INT->RawTfr & (1 << ch_num)) return TRUE;
        else return FALSE;
    }
    else if (ch_irq == DMA_IRQ_BLOCK)
    {
        if (DMA_INT->RawBlock & (1 << ch_num)) return TRUE;
        else return FALSE;
    }
    else if (ch_irq == DMA_IRQ_SRCTRAN)
    {
        if (DMA_INT->RawSrcTran & (1 << ch_num)) return TRUE;
        else return FALSE;
    }
    else if (ch_irq == DMA_IRQ_DSTTRAN)
    {
        if (DMA_INT->RawDstTran & (1 << ch_num)) return TRUE;
        else return FALSE;
    }
    else if (ch_irq == DMA_IRQ_ERR)
    {
        if (DMA_INT->RawErr & (1 << ch_num)) return TRUE;
        else return FALSE;
    }
    else return FALSE;
}

/**
 * @brief
 * @DateTime  2022-05-25T15:45:01+0800
 * @param     ch_num                    Enumerated DMA channel number
 * @param     ch_irq                    Enumerated interrupt type
 * @return    uint16_t                : DMA_INVALID_PARA
 *                                    : OK
 */
uint16_t DMA_Clear_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return DMA_INVALID_PARA;

    if (ch_irq == DMA_IRQ_NONE) {}
    else if (ch_irq == DMA_IRQ_TFR)
        DMA_INT->ClearTfr = (1 << ch_num);
    else if (ch_irq == DMA_IRQ_BLOCK)
        DMA_INT->ClearBlock = (1 << ch_num);
    else if (ch_irq == DMA_IRQ_SRCTRAN)
        DMA_INT->ClearSrcTran = (1 << ch_num);
    else if (ch_irq == DMA_IRQ_DSTTRAN)
        DMA_INT->ClearDstTran = (1 << ch_num);
    else if (ch_irq == DMA_IRQ_ERR)
        DMA_INT->ClearErr = (1 << ch_num);
    else if (ch_irq == DMA_IRQ_ALL)
    {
        DMA_INT->ClearErr = (1 << ch_num);
        DMA_INT->ClearBlock = (1 << ch_num);
        DMA_INT->ClearDstTran = (1 << ch_num);
        DMA_INT->ClearSrcTran = (1 << ch_num);
        DMA_INT->ClearTfr = (1 << ch_num);
    }
    else {}
    return OK;
}

/**
 * @brief     DMA_Mask_Irq: This function masks the specified interrupt(s) on the specified channel(s).
 * @DateTime  2022-05-25T16:31:28+0800
 * @param     ch_num                    Enumerated DMA channel number
 * @param     ch_irq                    Enumerated interrupt type
 * @return    uint16_t                : DMA_INVALID_PARA
 *                                    : OK
 */
uint16_t DMA_Mask_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return DMA_INVALID_PARA;

    if (ch_irq == DMA_IRQ_NONE) {}
    else if (ch_irq == DMA_IRQ_TFR)
        DMA_INT->MaskTfr = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    else if (ch_irq == DMA_IRQ_BLOCK)
        DMA_INT->MaskBlock = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    else if (ch_irq == DMA_IRQ_SRCTRAN)
        DMA_INT->MaskSrcTran = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    else if (ch_irq == DMA_IRQ_DSTTRAN)
        DMA_INT->MaskDstTran = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    else if (ch_irq == DMA_IRQ_ERR)
        DMA_INT->MaskErr = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    else if (ch_irq == DMA_IRQ_ALL)
    {
        DMA_INT->MaskErr = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
        DMA_INT->MaskBlock = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
        DMA_INT->MaskDstTran = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
        DMA_INT->MaskSrcTran = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
        DMA_INT->MaskTfr = ((DMA_CHEN_CH_WE_ENABLE << ch_num) | 0xFF);
    }
    else {}
    return OK;
}

/**
 * @brief     DMA_Unmask_Irq: This function unmasks the specified interrupt(s) on the specified channel(s).
 * @DateTime  2022-05-25T16:32:51+0800
 * @param     ch_num                    Enumerated DMA channel number
 * @param     ch_irq                    Enumerated interrupt type
 * @return    uint16_t                : DMA_INVALID_PARA
 *                                    : OK
 */
uint16_t DMA_Unmask_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq)
{
    if (ch_num >= DMA_CHANNEL_MAX)
        return DMA_INVALID_PARA;

    if (ch_irq == DMA_IRQ_NONE) {}
    else if (ch_irq == DMA_IRQ_TFR)
        DMA_INT->MaskTfr = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    else if (ch_irq == DMA_IRQ_BLOCK)
        DMA_INT->MaskBlock = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    else if (ch_irq == DMA_IRQ_SRCTRAN)
        DMA_INT->MaskSrcTran = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    else if (ch_irq == DMA_IRQ_DSTTRAN)
        DMA_INT->MaskDstTran = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    else if (ch_irq == DMA_IRQ_ERR)
        DMA_INT->MaskErr = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    else if (ch_irq == DMA_IRQ_ALL)
    {
        DMA_INT->MaskErr = (DMA_CHEN_CH_WE_ENABLE << ch_num);
        DMA_INT->MaskBlock = (DMA_CHEN_CH_WE_ENABLE << ch_num);
        DMA_INT->MaskDstTran = (DMA_CHEN_CH_WE_ENABLE << ch_num);
        DMA_INT->MaskSrcTran = (DMA_CHEN_CH_WE_ENABLE << ch_num);
        DMA_INT->MaskTfr = (DMA_CHEN_CH_WE_ENABLE << ch_num);
    }
    else {}
    return OK;
}

/**
 * @Breif  : DMA_Num2Ch     : Choose the DMA instance from channel para
 *
 * @Param  : ch_num    : DMA channel number
 *
 * @Returns: DMA_CH_T       : pointer to Dma Channel instance
 *
 */
DMA_CH_T* DMA_Num2Ch(enum dma_channel_number ch_num)
{
    DMA_CH_T *dma_ch = NULL;

    if (ch_num == DMA_CHANNEL0) dma_ch = DMA_CH0;
    else if (ch_num == DMA_CHANNEL1) dma_ch = DMA_CH1;
    else if (ch_num == DMA_CHANNEL2) dma_ch = DMA_CH2;
    else if (ch_num == DMA_CHANNEL3) dma_ch = DMA_CH3;
    else if (ch_num == DMA_CHANNEL4) dma_ch = DMA_CH4;
    else if (ch_num == DMA_CHANNEL5) dma_ch = DMA_CH5;
    else if (ch_num == DMA_CHANNEL6) dma_ch = DMA_CH6;
    else if (ch_num == DMA_CHANNEL7) dma_ch = DMA_CH7;
    else ;
    return dma_ch;
}

/**
 * @brief  DMA_Get_FreeChannel: This function returns a DMA channel number (enumerated) that is
 *                              disabled. The function starts at channel 0 and increments up
 *                              through the channels until a free channel is found.
 * @DateTime  2022-05-26T16:03:19+0800
 * @return                              DMA channel number
 */
enum dma_channel_number DMA_Get_FreeChannel(void)
{
    uint8_t ch_sta;
    ch_sta = DMA_EN->ChEn;
    if ((ch_sta & BIT0) == 0) return DMA_CHANNEL0;
    else if ((ch_sta & BIT1) == 0) return DMA_CHANNEL1;
    else if ((ch_sta & BIT2) == 0) return DMA_CHANNEL2;
    else if ((ch_sta & BIT3) == 0) return DMA_CHANNEL3;
    else if ((ch_sta & BIT4) == 0) return DMA_CHANNEL4;
    else if ((ch_sta & BIT5) == 0) return DMA_CHANNEL5;
    else if ((ch_sta & BIT6) == 0) return DMA_CHANNEL6;
    else if ((ch_sta & BIT7) == 0) return DMA_CHANNEL7;
    else return DMA_CHANNEL_MAX;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
