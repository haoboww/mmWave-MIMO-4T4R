/******************************************************************************
 * @Filename     : i2c.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 08/24/2021 16:01:41
 * @Modified by  : Mxy
 *
 * @Brief        : I2C driver source file
 *
 * Copyright (C) 2020 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/**
 * @brief       Set i2c params
 *
 * @param[in]   i2c                 i2c number
 * @param[in]   speed               I2C_SPEED_SS / I2C_SPEED_FS / I2C_SPEED_HS
 * @param[in]   addr_width          address width 7bit or 10bit (7 / 10)
 * @param[in]   addr_tar            target address
 */
void I2C_Init(I2C_T *i2c, uint32_t speed, uint32_t div_fac, uint32_t addr_width, uint32_t addr_tar)
{
    int32_t divH, divL;
    I2C_CLK_ENABLE;
    i2c->I2C_ENABLE = 0;
    i2c->CON        =   I2C_CON_MASTER_MODE_Msk     |
                        I2C_CON_SLAVE_DISABLE_Msk   |
                        I2C_CON_RESTART_EN_Msk      |
                        (addr_width == 10 ? I2C_CON_10BITADDR_SLAVE_Msk : 0) |
                        (speed << I2C_CON_SPEED_Pos);
    divH = (div_fac / 2) - 8;   divH = (divH < 6) ? 6 : divH;
    divL = (div_fac / 2) - 1;   divL = (divL < 8) ? 8 : divL;

    switch (speed)
    {
    case I2C_SPEED_SS:
        i2c->SS_SCL_HCNT = divH & 0xffff;
        i2c->SS_SCL_LCNT = divL & 0xffff;
        break;
    case I2C_SPEED_FS:
        i2c->FS_SCL_HCNT = divH & 0xffff;
        i2c->FS_SCL_LCNT = divL & 0xffff;
        break;
    case I2C_SPEED_HS:
        i2c->HS_SCL_HCNT = divH & 0xffff;
        i2c->HS_SCL_LCNT = divL & 0xffff;
        break;
    }
    i2c->TAR        = addr_tar & I2C_TAR_TAR_Msk;
    i2c->I2C_ENABLE = I2C_ENABLE_ENABLE_Msk;
}

/**
 * @brief       I2c Transmit data
 *
 * @param[in]   i2c             i2c number
 * @param[in]   send_buf        Transmit data
 * @param[in]   send_buf_len    Transmit data length
 *
 * @return      result
 *     - 0      Success
 *     - Other  Fail
 */
int I2C_Transmit_Data(I2C_T *i2c, const uint8_t *send_buf, uint8_t send_buf_len)
{
    uint32_t fifo_len, index;

    i2c->CLR_STOP_DET;
    i2c->CLR_TX_ABRT;
    while (send_buf_len) {
        fifo_len = 8 - i2c->TXFLR;
        fifo_len = send_buf_len < fifo_len ? send_buf_len : fifo_len;
        for (index = 0; index < fifo_len; index++)
            i2c->DATA_CMD = *send_buf++;
        send_buf_len -= fifo_len;
    }

    while ((i2c->RAW_INTR_STAT & I2C_RAW_INTR_MASK_STOP_DET_Msk) == 0);
    if (i2c->TX_ABRT_SOURCE != 0)
        return NOK;
    else
        return OK;
}

/**
 * @brief       I2c receive data
 *
 * @param[in]   i2c                 i2c number
 * @param[in]   send_buf            send data address
 * @param[in]   send_buf_len        length of send buf
 * @param[in]   receive_buf         receive buf address
 * @param[in]   receive_buf_len     length of receive buf
 *
 * @return      result
 *     - 0      Success
 *     - Other  Fail
*/
int I2C_Receive_Data(I2C_T *i2c, const uint8_t *send_buf, uint8_t send_buf_len, uint8_t *receive_buf, uint8_t receive_buf_len)
{
    uint32_t fifo_len, index;
    uint32_t rx_len = receive_buf_len;

    i2c->CLR_STOP_DET;
    i2c->CLR_TX_ABRT;
    while (send_buf_len)
    {
        fifo_len = 8 - i2c->TXFLR;
        fifo_len = send_buf_len < fifo_len ? send_buf_len : fifo_len;
        for (index = 0; index < fifo_len; index++)
            i2c->DATA_CMD = *send_buf++;
        send_buf_len -= fifo_len;
    }
    while ((i2c->RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY_Msk) == 0);
    while (rx_len)
    {
        fifo_len = 8 - i2c->TXFLR;
        fifo_len = receive_buf_len < fifo_len ? receive_buf_len : fifo_len;
        for (index = 0; index < fifo_len; index++)
            i2c->DATA_CMD = 0x1FF;
        receive_buf_len -= fifo_len;

        fifo_len = i2c->RXFLR;
        for (index = 0; index < fifo_len; index++)
            *receive_buf++ = (uint8_t)i2c->DATA_CMD;
        rx_len -= fifo_len;
    }
    while ((i2c->RAW_INTR_STAT & I2C_RAW_INTR_MASK_STOP_DET_Msk) == 0);
    if (i2c->TX_ABRT_SOURCE != 0)
        return NOK;
    else
        return OK;
}
