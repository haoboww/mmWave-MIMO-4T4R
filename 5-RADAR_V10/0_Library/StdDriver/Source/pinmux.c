/******************************************************************************
 * @Filename     : pinmux.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/04/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : PINMUX driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/**
 * @brief     PinMux_Set : Set PINMUX function
 * @DateTime  2022-05-04T20:36:14+0800
 * @param     gpio                      GPIO port: It could be
 *                                      - GPIO0,
 *                                      - GPIO1,
 *                                      - GPIO2,
 *                                      - GPIO3,
 * @param     u8PinNum                  GPIO PIN Num, 0-15.
 * @param     ModeFunc                  PIN function mode, 0-8. ref pinmux excel
 * @return                              set result
 *                                      = PINMUX_INVALID_PARA: Parameter Error
 *                                      = OK: Set Successfully
 */
uint16_t PinMux_Set(GPIO_T *gpio, uint8_t u8PinNum, uint32_t ModeFunc)
{
    uint32_t reg_value = 0;

    if((gpio == NULL) || (u8PinNum >= GPIO_PIN_MAX) ||(ModeFunc >= IOMODE_FUNC_MAX))
        return PINMUX_INVALID_PARA;
    if(gpio == GPIO0)
    {
        if(u8PinNum < 8)
        {
            reg_value = PORT01_PINMUX->PORT_MUX0;
            reg_value &= ~(PINMUX_DATA_MSK << (u8PinNum * 4));
            reg_value |= (ModeFunc << (u8PinNum * 4));
            PORT01_PINMUX->PORT_MUX0 = reg_value;
        }
        else
        {
            reg_value = PORT01_PINMUX->PORT_MUX1;
            reg_value &= ~(PINMUX_DATA_MSK << ((u8PinNum - 8) * 4));
            reg_value |= (ModeFunc << ((u8PinNum - 8) * 4));
            PORT01_PINMUX->PORT_MUX1 = reg_value;
        }
    }
    else if(gpio == GPIO1)
    {
        if(u8PinNum < 8)
        {
            reg_value = PORT01_PINMUX->PORT_MUX2;
            reg_value &= ~(PINMUX_DATA_MSK << (u8PinNum * 4));
            reg_value |= (ModeFunc<<(u8PinNum * 4));
            PORT01_PINMUX->PORT_MUX2 = reg_value;
        }
        else
        {
            reg_value = PORT01_PINMUX->PORT_MUX3;
            reg_value &= ~(PINMUX_DATA_MSK << ((u8PinNum - 8) * 4));
            reg_value |= (ModeFunc << ((u8PinNum - 8) * 4));
            PORT01_PINMUX->PORT_MUX3 = reg_value;
        }
    }
    else if(gpio == GPIO2)
    {
        if(u8PinNum < 8)
        {
            reg_value = PORT23_PINMUX->PORT_MUX0;
            reg_value &= ~(PINMUX_DATA_MSK << (u8PinNum * 4));
            reg_value |= (ModeFunc << (u8PinNum * 4));
            PORT23_PINMUX->PORT_MUX0 = reg_value;
        }
        else
        {
            reg_value = PORT23_PINMUX->PORT_MUX1;
            reg_value &= ~(PINMUX_DATA_MSK << ((u8PinNum - 8) * 4));
            reg_value |= (ModeFunc << ((u8PinNum - 8) * 4));
            PORT23_PINMUX->PORT_MUX1 = reg_value;
        }
    }
    else if(gpio == GPIO3)
    {
        if(u8PinNum < 8)
        {
            reg_value = PORT23_PINMUX->PORT_MUX2;
            reg_value &= ~(PINMUX_DATA_MSK << (u8PinNum * 4));
            reg_value |= (ModeFunc<<(u8PinNum * 4));
            PORT23_PINMUX->PORT_MUX2 = reg_value;
        }
        else
        {
            reg_value = PORT23_PINMUX->PORT_MUX3;
            reg_value &= ~(PINMUX_DATA_MSK << ((u8PinNum - 8) * 4));
            reg_value |= (ModeFunc << ((u8PinNum - 8) * 4));
            PORT23_PINMUX->PORT_MUX3 = reg_value;
        }
    }
    else
        return PINMUX_INVALID_PARA;
    return OK;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
