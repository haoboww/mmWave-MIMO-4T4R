/******************************************************************************
 * @Filename     : i2c.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 07/25/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : I2C driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __I2C_H__
#define __I2C_H__
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum i2c_speed_mode_t {
    I2C_SPEED_SS = 1,
    I2C_SPEED_FS = 2,
    I2C_SPEED_HS = 3
};

typedef enum
{
    I2C_EV_START,
    I2C_EV_RESTART,
    I2C_EV_STOP
} i2c_event_t;

typedef struct
{
    void(*on_receive)(uint32_t data);
    uint32_t(*on_transmit)();
    void(*on_event)(i2c_event_t event);
} i2c_slave_handler_t;

#define I2C_CLR_INTR                        I2C->CLR_INTR
#define I2C_CLR_RX_UNDER                    I2C->CLR_RX_UNDER
#define I2C_CLR_RX_OVER                     I2C->CLR_RX_OVER
#define I2C_CLR_TX_OVER                     I2C->CLR_TX_OVER
#define I2C_CLR_RD_REQ                      I2C->CLR_RD_REQ
#define I2C_CLR_TX_ABART                    I2C->CLR_TX_ABART
#define I2C_CLR_RX_DONE                     I2C->CLR_RX_DONE
#define I2C_CLR_ACTIVITY                    I2C->CLR_ACTIVITY
#define I2C_CLR_STOP_DET                    I2C->CLR_STOP_DET
#define I2C_CLR_START_DE                    I2C->CLR_START_DE
#define I2C_CLR_GEN_CALL                    I2C->CLR_GEN_CALL

/**
 * @brief       Set i2c params
 *
 * @param[in]   i2c                 i2c number
 * @param[in]   speed               I2C_SPEED_SS / I2C_SPEED_FS / I2C_SPEED_HS
 * @param[in]   addr_width          address width 7bit or 10bit (7 / 10)
 * @param[in]   addr_tar            target address
 */
void I2C_Init(I2C_T *i2c, uint32_t speed, uint32_t div_fac, uint32_t addr_width, uint32_t addr_tar);

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
int I2C_Transmit_Data(I2C_T *i2c, const uint8_t *send_buf, uint8_t send_buf_len);

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
int I2C_Receive_Data(I2C_T *i2c, const uint8_t *send_buf, uint8_t send_buf_len, uint8_t *receive_buf, uint8_t receive_buf_len);

#ifdef __cplusplus
}
#endif

#endif // __I2C_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
