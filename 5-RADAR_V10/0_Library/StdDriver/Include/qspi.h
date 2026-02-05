/******************************************************************************
 * @Filename     : qspi.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/16/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : QSPI driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __QSPI_H__
#define __QSPI_H__
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define QSPI_DUMMY_DATA                (0xFF)

//user config para
#define QSPI_CS_CS_LOW                 (0x00ul)
#define QSPI_CS_CS_HIGH                (0x01ul)

//Continous read mode. dual and quad
#define QSPI_CON_READ_MODE_FIRST_STEP  (0xA5)
#define QSPI_CON_READ_MODE_SECOND_STEP (0x5A)

enum qspi_status {
  QSPI_OK = 0,
  QSPI_BUSY,
  QSPI_ERROR,
  QSPI_PARA_ERROR
};

enum qspi_frame_mode {
  QSPI_STANDARD        = 0x00ul,     /* single */
  QSPI_DUAL_INPUT      = 0x01ul,     /* dual input */
  QSPI_DUAL_OUTPUT     = 0x02ul,     /* dual output */
  QSPI_QUAD_INPUT      = 0x03ul,     /* quad input */
  QSPI_QUAD_OUTPUT     = 0x04ul,     /* quad output */
  QSPI_OTHER           = QSPI_STANDARD
};

//QSPI API function
enum qspi_status QSPI_Init(QSPI_T *qspi, uint32_t ctrl_reg);
//QSPI_STANDARD RX and TX
enum qspi_status QSPI_Receive(QSPI_T *qspi, uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len);
enum qspi_status QSPI_Transmit(QSPI_T *qspi, uint8_t *cmd_buf, uint8_t cmd_len, uint8_t *tx_buf, uint32_t tx_len);
//DUAL or QUAD for RX and TX
enum qspi_status QSPI_Receive_Enhanced(QSPI_T *qspi, uint32_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len, enum qspi_frame_mode qspi_mode);
enum qspi_status QSPI_Transmit_Enhanced(QSPI_T *qspi, uint32_t *cmd_buf, uint8_t cmd_len, uint8_t *tx_buf, uint32_t tx_len, enum qspi_frame_mode qspi_mode);
//DUAL Fast or QUAD Fast for RX
enum qspi_status QSPI_Receive_QUAD_IO_Fast(QSPI_T *qspi, uint32_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len, enum qspi_frame_mode qspi_mode);
enum qspi_status QSPI_Receive_QUAD_IO_Fast_Continuous(QSPI_T *qspi, uint32_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len, enum qspi_frame_mode qspi_mode);
enum qspi_status QSPI_Receive_DUAL_IO_Fast(QSPI_T *qspi, uint32_t *cmd_buf, uint8_t cmd_len, uint8_t *rx_buf, uint32_t rx_len, enum qspi_frame_mode qspi_mode);
//XIP Mode
enum qspi_status QSPI_Enable_Xcode_Mode(QSPI_T *qspi, uint32_t ctrl_reg);
enum qspi_status QSPI_Disable_Xcode_Mode(QSPI_T *qspi);

#ifdef __cplusplus
}
#endif

#endif // __QSPI_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
