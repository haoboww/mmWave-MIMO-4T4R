/******************************************************************************
 * @Filename     : gpio.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/05/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : GPIO driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/**
 * @brief       Set GPIO operation mode
 *
 * @param[in]   gpio        GPIO port. It could be
 *                          - GPIO0,
 *                          - GPIO1,
 *                          - GPIO2,
 *                          - GPIO3,
 * @param[in]   u32PinMask  The single or multiple pins of specified GPIO port.
 *                          - GPIO_PIN_0 ~ GPIO_PIN_15
 * @param[in]   u32Mode     Operation mode.
 *                          It could be:
 *                          - GPIO_PMODE_INPUT,
 *                          - GPIO_PMODE_OUTPUT,
 *
 * @return      None
 *
 * @details     This function is used to set specified GPIO operation mode.
 */
void GPIO_SetMode(GPIO_T *gpio, uint32_t u32PinMask, uint32_t u32Mode)
{
    if (u32Mode == GPIO_PMODE_OUTPUT) // output
    {
        gpio->PMODE |= (uint32_t) (u32PinMask);
    }
    else
    {
        gpio->PMODE &= (uint32_t) (~(u32PinMask));
    }
}

/**
 * @brief       Enable GPIO interrupt
 *
 * @param[in]   gpio            GPIO port. It could be
 *                              - GPIO0,
 *                              - GPIO1,
 *                              - GPIO2,
 *                              - GPIO3,
 * @param[in]   u32Pin          The pin of specified GPIO port. It could be
 *                              - 0 ~ 15
 * @param[in]   u32IntType      The interrupt Type of specified GPIO pin. It could be
 *                              - GPIO_INTTYPE_EDGE,
 *                              - GPIO_INTTYPE_LEVEL
 * @param[in]   u32IntPol       The interrupt Polarity of specified GPIO pin. It could be
 *                              - GPIO_INTPOL_RISING,
 *                              - GPIO_INTPOL_FALLING,
 *                              - GPIO_INTPOL_BOTH_EDGE,
 *                              - GPIO_INTPOL_HIGH,
 *                              - GPIO_INTPOL_LOW,
 *
 * @return      None
 *
 * @details     This function is used to enable specified GPIO pin interrupt.
 */
void GPIO_EnableInt(GPIO_T *gpio, uint32_t u32Pin, uint32_t u32IntType, uint32_t u32IntPol)
{
    gpio->INTTYPE |= (u32IntType << u32Pin);
    gpio->INTPOL  |= (u32IntPol  << (u32Pin << 1));
    gpio->INTEN   |= (1UL << u32Pin);
}

/**
 * @brief       Disable GPIO interrupt
 *
 * @param[in]   gpio        GPIO port. It could be
 *                          - GPIO0,
 *                          - GPIO1,
 *                          - GPIO2,
 *                          - GPIO3,
 * @param[in]   u32Pin      The pin of specified GPIO port. It could be
 *                          - 0 ~ 31
 *
 * @return      None
 *
 * @details     This function is used to enable specified GPIO pin interrupt.
 */
void GPIO_DisableInt(GPIO_T *gpio, uint32_t u32Pin)
{
    gpio->INTEN &= ~((1UL) << u32Pin);
}

/**
 * @Breif  : GPIO_AE_Ctrl: GPIO0 pad digital or analog mode select
 *
 * @param  : gpio: GPIO port: It could be
 *                 - GPIO0,
 *                 - GPIO1,
 *                 - GPIO2,
 *                 - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 * @Param  : u8Mode: GPIO0 pad mode
 *           - DIGITAL_MODE 0
 *           - ANALOG_MODE  1
 *
 * @return : set result
 *           - GPIO_INVALID_PARA: Parameter Error
 *           - OK: Set Successfully
 *
 * @note: must enable pwm first, and then config pwm register.
 */

uint16_t GPIO_AE_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8Mode)
{
    if(gpio == GPIO0)
    {
        if (u8Mode == ANALOG_MODE)
            PORT01_PIN_CTRL->AE |= u32PinMask;
        else if(u8Mode == DIGITAL_MODE)
            PORT01_PIN_CTRL->AE &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO1)
    {
        if (u8Mode == ANALOG_MODE)
            PORT01_PIN_CTRL->AE |= (u32PinMask << 16);
        else if(u8Mode == DIGITAL_MODE)
            PORT01_PIN_CTRL->AE &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO2)
    {
        if (u8Mode == ANALOG_MODE)
            PORT23_PIN_CTRL->AE |= u32PinMask;
        else if(u8Mode == DIGITAL_MODE)
            PORT23_PIN_CTRL->AE &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO3)
    {
        if (u8Mode == ANALOG_MODE)
            PORT23_PIN_CTRL->AE |= (u32PinMask << 16);
        else if(u8Mode == DIGITAL_MODE)
            PORT23_PIN_CTRL->AE &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else
        return GPIO_INVALID_PARA;
    return OK;
}

/**
 * @Breif  : GPIO_AT_Ctrl: GPIO analog test mode select
 *
 * @param  : gpio: GPIO port: It could be
 *                 - GPIO0,
 *                 - GPIO1,
 *                 - GPIO2,
 *                 - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 * @Param  : u8Mode: test mode enable
 *           - NORMAL_MODE 0
 *           - TEST_MODE   1
 *
 * @return : set result
 *           - GPIO_INVALID_PARA: Parameter Error
 *           - OK: Set Successfully
 *
 */

uint16_t GPIO_AT_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8Mode)
{
    if(gpio == GPIO0)
    {
        if (u8Mode == TEST_MODE)
            PORT01_PIN_CTRL->AT |= u32PinMask;
        else if(u8Mode == NORMAL_MODE)
            PORT01_PIN_CTRL->AT &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO1)
    {
        if (u8Mode == TEST_MODE)
            PORT01_PIN_CTRL->AT |= (u32PinMask << 16);
        else if(u8Mode == NORMAL_MODE)
            PORT01_PIN_CTRL->AT &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO2)
    {
        if (u8Mode == TEST_MODE)
            PORT23_PIN_CTRL->AT |= u32PinMask;
        else if(u8Mode == NORMAL_MODE)
            PORT23_PIN_CTRL->AT &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO3)
    {
        if (u8Mode == TEST_MODE)
            PORT23_PIN_CTRL->AT |= (u32PinMask << 16);
        else if(u8Mode == NORMAL_MODE)
            PORT23_PIN_CTRL->AT &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else
        return GPIO_INVALID_PARA;
    return OK;
}

/**
 * @Breif  : GPIO_RE_Ctrl : This function config special pin pull-up or pull-down resistor
 *
 * @param  : gpio      : GPIO port: It could be
 *                       - GPIO0,
 *                       - GPIO1,
 *                       - GPIO2,
 *                       - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 * @Param  : u8RE      : Resistor enable bit
 *                       - GPIO_RE_ON  (1): Resistor Enable
 *                       - GPIO_RE_OFF (0): Resistor Disable
 *
 * @return : uin16_t   : set result
 *                       - GPIO_INVALID_PARA: Parameter Error
 *                       - OK: Set Successfully
 *
 */
uint16_t GPIO_RE_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8RE)
{
    if(gpio == GPIO0)
    {
        if (u8RE == GPIO_RE_ON)
            PORT01_PIN_CTRL->RE |= u32PinMask;
        else if(u8RE == GPIO_RE_OFF)
            PORT01_PIN_CTRL->RE &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO1)
    {
        if (u8RE == GPIO_RE_ON)
            PORT01_PIN_CTRL->RE |= (u32PinMask << 16);
        else if(u8RE == GPIO_RE_OFF)
            PORT01_PIN_CTRL->RE &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO2)
    {
        if (u8RE == GPIO_RE_ON)
            PORT23_PIN_CTRL->RE |= u32PinMask;
        else if(u8RE == GPIO_RE_OFF)
            PORT23_PIN_CTRL->RE &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO3)
    {
        if (u8RE == GPIO_RE_ON)
            PORT23_PIN_CTRL->RE |= (u32PinMask << 16);
        else if(u8RE == GPIO_RE_OFF)
            PORT23_PIN_CTRL->RE &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else
        return GPIO_INVALID_PARA;
    return OK;
}

/**
 * @Breif  : GPIO_PullUp : This function config special pin pull-up
 *
 * @param  : gpio      : GPIO port: It could be
 *                       - GPIO0,
 *                       - GPIO1,
 *                       - GPIO2,
 *                       - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 *
 * @return : uin16_t   : set result
 *                       - GPIO_INVALID_PARA: Parameter Error
 *                       - OK: Set Successfully
 *
 */
uint16_t GPIO_PullUp(GPIO_T *gpio, uint32_t u32PinMask)
{
    if(gpio == GPIO0)
    {
        PORT01_PIN_CTRL->RE |= u32PinMask;
        PORT01_PIN_CTRL->UE |= u32PinMask;
    }
    else if(gpio == GPIO1)
    {
        PORT01_PIN_CTRL->RE |= (u32PinMask << 16);
        PORT01_PIN_CTRL->UE |= (u32PinMask << 16);
    }
    else if(gpio == GPIO2)
    {
        PORT23_PIN_CTRL->RE |= u32PinMask;
        PORT23_PIN_CTRL->UE |= u32PinMask;
    }
    else if(gpio == GPIO3)
    {
        PORT23_PIN_CTRL->RE |= (u32PinMask << 16);
        PORT23_PIN_CTRL->UE |= (u32PinMask << 16);
    }
    else
    {
        return GPIO_INVALID_PARA;
    }
    return OK;
}

/**
 * @Breif  : GPIO_PullDown : This function config special pin pull-down
 *
 * @param  : gpio      : GPIO port: It could be
 *                       - GPIO0,
 *                       - GPIO1,
 *                       - GPIO2,
 *                       - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 *
 * @return : uin16_t   : set result
 *                       - GPIO_INVALID_PARA: Parameter Error
 *                       - OK: Set Successfully
 *
 */
uint16_t GPIO_PullDown(GPIO_T *gpio, uint32_t u32PinMask)
{
    if(gpio == GPIO0)
    {
        PORT01_PIN_CTRL->RE |= u32PinMask;
        PORT01_PIN_CTRL->UE &= ~u32PinMask;
    }
    else if(gpio == GPIO1)
    {
        PORT01_PIN_CTRL->RE |= (u32PinMask << 16);
        PORT01_PIN_CTRL->UE &= ~(u32PinMask << 16);
    }
    else if(gpio == GPIO2)
    {
        PORT23_PIN_CTRL->RE |= u32PinMask;
        PORT23_PIN_CTRL->UE &= ~u32PinMask;
    }
    else if(gpio == GPIO3)
    {
        PORT23_PIN_CTRL->RE |= (u32PinMask << 16);
        PORT23_PIN_CTRL->UE &= ~(u32PinMask << 16);
    }
    else
        return GPIO_INVALID_PARA;
    return OK;
}

/**
 * @Breif  : GPIO_OD_Ctrl : This function control special pin open drain func
 *
* @param  : gpio       : GPIO port: It could be
 *                       - GPIO0,
 *                       - GPIO1,
 *                       - GPIO2,
 *                       - GPIO3,
 * @param  : u32PinMask: The single or multiple pins of specified GPIO port.
 *                       It could be
 *                       - GPIO_PIN_0 ~ GPIO_PIN_15
 * @Param  : u8RE      : Open Drain enable bit
 *                       - GPIO_OD_ON  (1): Open Drain Enable
 *                       - GPIO_OD_OFF (0): Open Drain Disable
 *
 * @return : uin16_t   : set result
 *                       - GPIO_INVALID_PARA: Parameter Error
 *                       - OK: Set Successfully
 *
 */
uint16_t GPIO_OD_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8OD)
{
    if(gpio == GPIO0)
    {
        if (u8OD == GPIO_OD_ON)
            PORT01_PIN_CTRL->OD |= u32PinMask;
        else if(u8OD == GPIO_OD_OFF)
            PORT01_PIN_CTRL->OD &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO1)
    {
        if (u8OD == GPIO_OD_ON)
            PORT01_PIN_CTRL->OD |= (u32PinMask << 16);
        else if(u8OD == GPIO_OD_OFF)
            PORT01_PIN_CTRL->OD &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO2)
    {
        if (u8OD == GPIO_OD_ON)
            PORT23_PIN_CTRL->OD |= u32PinMask;
        else if(u8OD == GPIO_OD_OFF)
            PORT23_PIN_CTRL->OD &= ~u32PinMask;
        else
            return GPIO_INVALID_PARA;
    }
    else if(gpio == GPIO3)
    {
        if (u8OD == GPIO_OD_ON)
            PORT23_PIN_CTRL->OD |= (u32PinMask << 16);
        else if(u8OD == GPIO_OD_OFF)
            PORT23_PIN_CTRL->OD &= ~(u32PinMask << 16);
        else
            return GPIO_INVALID_PARA;
    }
    else
        return GPIO_INVALID_PARA;
    return OK;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
