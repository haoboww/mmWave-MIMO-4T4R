/******************************************************************************
 * @Filename     : crc.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/20/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : CRC driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include "SKY32B750.h"

/**
 * @brief
 * @DateTime  2022-05-20T13:23:15+0800
 * @param     crc_ini                   CRC init value
 * @param     crc_inv                   Invert the CRC result or not
 *                                      -- CRC_INV_ENABLE     1
 *                                      -- CRC_INV_DISABLE    0
 * @param     crc_din                   pointer to CRC Data in buffer
 * @param     size                      CRC Data in size (u8)
 * @return    uint16_t                  CRC Result
 */
uint16_t CRC_Calculate(uint16_t crc_ini, uint8_t crc_inv, uint8_t *crc_din, uint32_t size)
{
    Peripheral_Reset(CRC_RST_Msk);
    if (crc_inv == CRC_INV_ENABLE)
        CRC->CON  = 0x03;
    else
        CRC->CON  = 0x01;
    CRC->INIT = crc_ini;
    while (size--)
    {
        CRC->DIN = *crc_din++;
    }

    return ((uint16_t)CRC->DOUT);
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
