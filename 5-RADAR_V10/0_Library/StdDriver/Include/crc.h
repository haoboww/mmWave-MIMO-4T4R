/******************************************************************************
 * @Filename     : crc.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/18/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : CRC driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __CRC_H__
#define __CRC_H__

#include "SKY32B750.h"

#define CRC_CON_START                        (1)
#define CRC_INV_ENABLE                       (1)
#define CRC_INV_DISABLE                      (0)

/**
 * @Breif  : CRC_Calculate: CRC16 Calculate function
 *
 * @Param  : *crc      : pointer to CRC instance
 *           crc_ini   : CRC init value
 *           *crc_din  : pointer to CRC Data in buffer
 *           size      : CRC Data in size (u8)
 *
 * @Returns: uint16_t  : CRC Result
 */
uint16_t CRC_Calculate(uint16_t crc_ini, uint8_t crc_inv, uint8_t *crc_din, uint32_t size);

#endif

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
