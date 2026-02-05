/******************************************************************************
 * @Filename     : usci.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : USCI driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __USCI_H__
#define __USCI_H__
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define USCI_INVALID_PARA              0x0201
#define USCI_TIMEOUT_ERR               0x0202

//usci work mode, uart/spi/u7816
#define USCI_WORK_MODE_SPI             0x00ul
#define USCI_WORK_MODE_UART            0x01ul
#define USCI_WORK_MODE_U7816           0x02ul

//uart config
#define USCI_UART_STOP_BIT_1           0x00ul
#define USCI_UART_STOP_BIT_2           0x01ul
#define USCI_UART_PAR_TYP_EVEN         0x00ul
#define USCI_UART_PAR_TYP_ODD          0x01ul
#define USCI_UART_PAR_EN_OFF           0x00ul
#define USCI_UART_PAR_EN_ON            0x01ul
#define USCI_UART_LSB_MSB              0x00ul
#define USCI_UART_LSB_LSB              0x01ul
#define USCI_UART_DATA_LEN_7BIT        0x00ul
#define USCI_UART_DATA_LEN_8BIT        0x01ul
#define USCI_UART_DATA_LEN_9BIT        0x02ul
#define USCI_UART_DATA_WIDTH           (16)

//spi config
#define USCI_SPI_WORK_MODE_MASTER      0x00ul
#define USCI_SPI_WORK_MODE_SLAVE       0x01ul
#define USCI_SPI_CLKP_IDLE_LOW         0x00ul
#define USCI_SPI_CLKP_IDLE_HIGH        0x01ul
#define USCI_SPI_TRX_NEG_POSEDGE       0x00ul
#define USCI_SPI_TRX_NEG_NEGEDGE       0x01ul
#define USCI_SPI_LSB_MSB               0x00ul
#define USCI_SPI_LSB_LSB               0x01ul
#define USCI_SPI_DATA_LEN_7BIT         0x00ul
#define USCI_SPI_DATA_LEN_8BIT         0x01ul
#define USCI_SPI_DATA_LEN_9BIT         0x02ul
#define USCI_SPI_SLAVE_CS_LEVEL_LOW    0x00ul
#define USCI_SPI_SLAVE_CS_LEVEL_HIGH   0x01ul
#define USCI_SPI_DUMMY_DATA            0xFF

//u7816 config
#define USCI_U7816_STOP_BIT_1          0x00ul
#define USCI_U7816_STOP_BIT_2          0x01ul
#define USCI_U7816_PAR_TYP_EVEN        0x00ul
#define USCI_U7816_PAR_TYP_ODD         0x01ul
#define USCI_U7816_PAR_EN_OFF          0x00ul
#define USCI_U7816_PAR_EN_ON           0x01ul
#define USCI_U7816_LSB_MSB             0x00ul
#define USCI_U7816_LSB_LSB             0x01ul
#define USCI_U7816_DATA_LEN_7BIT       0x00ul
#define USCI_U7816_DATA_LEN_8BIT       0x01ul
#define USCI_U7816_DATA_LEN_9BIT       0x02ul
#define USCI_U7816_BIT_INVERT_NO       0x00lu
#define USCI_U7816_BIT_INVERT          0x01ul

#define USCI_U7816_TRX_EN_MSK          0x00000003ul
#define USCI_U7816_BAUD_DEFAULT        (372*2 - 1)
#define USCI_U7816_BAUD_HIGH_SPEED     (32*2 -1)

#define USCI_U7816_CMD_CLK_ON          0x5A001AA5
#define USCI_U7816_CMD_CLK_OFF         0x5A0019A5

/**
  * @brief  USCI-UART Initial Structure definition
  */
typedef struct
{
  uint32_t uart_baud_rate;                /*!< UART communication baud rate @ref uart_baud_rate */
  uint32_t uart_stop_bit ;                /*!< Specifies the number of stop bits transmitted. */
  uint32_t uart_parity_typ;               /*!< Specifies the parity mode. */
  uint32_t uart_parity_en;                /*!< Parity enable. */
  uint32_t uart_lsb;                      /*!< LSB & MSB. */
  uint32_t uart_data_len;                 /*!< data lenth, 7,8,9 bit. */
} USCI_Uart_Init_t;

/**
  * @brief  USCI-SPI Initial structure definition
  */
typedef struct
{
  uint32_t spi_mode;                      /*!< Specifies the SPI mode (Master/Slave).*/
  uint32_t spi_clkp;
  uint32_t spi_posedge;
  uint32_t spi_msb;
  uint32_t spi_data_len;
  uint32_t spi_cs_level;
  uint32_t spi_cs_gpio;
} USCI_SPI_Init_t;

/**
  * @brief  USCI-u7816 Initial structure definition
  */
typedef struct
{
  uint32_t u7816_speed;
  uint32_t u7816_stop_bit ;                  /*!< Specifies the number of stop bits transmitted. */
  uint32_t u7816_parity_typ;                 /*!< Specifies the parity mode. */
  uint32_t u7816_parity_en;                  /*!< Parity enable. */
  uint32_t u7816_lsb;
  uint32_t u7816_data_len;
  uint32_t u7816_invert;
} USCI_U7816_Init_t;

/**
  * @brief USCI Transmit/Receive handler struct
  */
typedef struct
{
  USCI_T                     *instance;       /*!< USCI registers base address */
  union
  {
    USCI_Uart_Init_t       u_init;          /*!< UART communication parameters */
    USCI_SPI_Init_t        s_init;          /*!< SPI communication parameters */
    USCI_U7816_Init_t      c_init;          /*!< 7816 communication parameters */
  };
  uint32_t                   usci_clock_div;  /*!< usci divide parameters */
} usci_handler_t;

uint16_t USCI_Enable(USCI_T *usci);
uint16_t USCI_Disable(USCI_T *usci);
void USCI_Clear_ISR(usci_handler_t *usci);
void USCI_Clear_ESR(usci_handler_t *usci);

//UART API function
uint16_t USCI_UART_Init(usci_handler_t *usci);
uint16_t USCI_UART_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
uint16_t USCI_UART_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size);

//SPI API function
uint16_t USCI_SPI_Init(usci_handler_t *usci);
uint16_t USCI_SPI_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
uint16_t USCI_SPI_Transmit_Ext(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint32_t tout_ms);
uint16_t USCI_SPI_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size);
uint16_t USCI_SPI_TRx(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint8_t *rx_buf);
uint16_t USCI_SPI_TRx_Ext(usci_handler_t *usci, uint8_t *tx_buf, int32_t size, uint8_t *rx_buf, uint32_t tout_ms);
uint16_t USCI_SPI_Slave_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
uint16_t USCI_SPI_Slave_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size);

//U7816 API function
uint16_t USCI_U7816_Init(usci_handler_t *usci);
uint16_t USCI_U7816_Transmit(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
uint16_t USCI_U7816_Receive(usci_handler_t *usci, uint8_t *rx_buf, int32_t size);
uint16_t USCI_U7816_Cmd(usci_handler_t *usci, uint32_t cmd);

#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
