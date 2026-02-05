/******************************************************************************
 * @Filename     : ospi.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/30/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : OSPI driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

void OSPI_Init(OSPI_T *ospi, ospi_config_t config)
{
    ospi->CON0 = (config.fifo_wl << OSPI_CON0_FIFO_WL_Pos)         | // fifo_wl
                 (config.slave_en << OSPI_CON0_SLAVE_Pos)          | // slv_en
                 (config.dma_en << OSPI_CON0_DMA_EN_Pos)           | // dma_en
                 (config.trans_mode << OSPI_CON0_TRANS_MODE_Pos)   | // mode
                 (config.cpha << OSPI_CON0_CPHA_Pos)               | // samp_edge
                 (config.fifo_int_en << OSPI_CON0_FIFO_INT_EN_Pos) | // fifo_irq_en
                 (config.send_int_en << OSPI_CON0_SEND_INT_EN_Pos) | // slv_end_irq_en
                 (config.nend_int_en << OSPI_CON0_NEND_INT_EN_Pos);  // master_tx_irq_en
    ospi->CON1 = (config.read_cmd << OSPI_CON1_READ_CMD_Pos)       | // read_cmd
                 (config.tshsl << OSPI_CON1_TSHSL_Pos)             | // tshsl
                 (config.dummy_len << OSPI_CON1_DUMMY_LEN_Pos)     | // dummy_len
                 (config.dummy_byte << OSPI_CON1_DUMMY_BYTE_Pos)   | // dummy_byte
                 (config.div << OSPI_CON1_DIV_Pos)                 | // clk_div
                 (config.cmd_on << OSPI_CON1_CMD_ON_Pos)           | // ins_on
                 (config.fifo_en << OSPI_CON1_NORMAL_FIFO_Pos)     | // master_normal_fifo
                 (config.xip_en << OSPI_CON1_XIP_SEL_Pos);           // xip_mode_en
}

void OSPI_CS_HIGH(OSPI_T *ospi)
{
    ospi->CS = 1;
}

void OSPI_CS_LOW(OSPI_T *ospi)
{
    ospi->CS = 0;
}

void OSPI_Clear_Irq(OSPI_T *ospi, enum ospi_irq irq)
{
    ospi->INT_STA = irq;
}

bool OSPI_Irq_IsActive(OSPI_T *ospi, enum ospi_irq irq)
{
    if (ospi->INT_STA & irq)
        return TRUE;
    else
        return FALSE;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
