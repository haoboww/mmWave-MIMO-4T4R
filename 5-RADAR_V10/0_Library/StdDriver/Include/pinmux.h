/******************************************************************************
 * @Filename     : pinmux.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/04/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : PINMUX driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __PINMUX_H__
#define __PINMUX_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define PINMUX_INVALID_PARA         0x0101

#define PINMUX_DATA_MSK             0x0Ful

#define IOMODE_FUNC0                0x0U        /*!< Selects pin function 0 */
#define IOMODE_FUNC1                0x1U        /*!< Selects pin function 1 */
#define IOMODE_FUNC2                0x2U        /*!< Selects pin function 2 */
#define IOMODE_FUNC3                0x3U        /*!< Selects pin function 3 */
#define IOMODE_FUNC4                0x4U        /*!< Selects pin function 4 */
#define IOMODE_FUNC5                0x5U        /*!< Selects pin function 5 */
#define IOMODE_FUNC6                0x6U        /*!< Selects pin function 6 */
#define IOMODE_FUNC7                0x7U        /*!< Selects pin function 7 */
#define IOMODE_FUNC8                0x8U        /*!< Selects pin function 8 */
#define IOMODE_FUNC_MAX             0x9U        /*!< Selects pin function max */

#define MUX0                        0x0U        /*!< Selects mux 0 */
#define MUX1                        0x1U        /*!< Selects mux 1 */
#define MUX2                        0x2U        /*!< Selects mux 2 */
#define MUX3                        0x3U        /*!< Selects mux 3 */
#define MUX_MAX                     0x4U        /*!< Selects mux max */

#define IOMODE_USCI0_IO0            IOMODE_FUNC1
#define IOMODE_USCI0_IO1            IOMODE_FUNC1
#define IOMODE_USCI0_IO2            IOMODE_FUNC1
#define IOMODE_USCI1_IO0            IOMODE_FUNC1
#define IOMODE_USCI1_IO1            IOMODE_FUNC1
#define IOMODE_USCI1_IO2            IOMODE_FUNC1
#define IOMODE_USCI2_IO0            IOMODE_FUNC1
#define IOMODE_USCI2_IO1            IOMODE_FUNC1
#define IOMODE_USCI2_IO2            IOMODE_FUNC1
#define IOMODE_USCI3_IO0            IOMODE_FUNC1
#define IOMODE_USCI3_IO1            IOMODE_FUNC1
#define IOMODE_USCI3_IO2            IOMODE_FUNC1
#define IOMODE_USCI4_IO0            IOMODE_FUNC1
#define IOMODE_USCI4_IO1            IOMODE_FUNC1
#define IOMODE_USCI4_IO2            IOMODE_FUNC1
#define IOMODE_USCI5_IO0            IOMODE_FUNC1
#define IOMODE_USCI5_IO1            IOMODE_FUNC1
#define IOMODE_USCI5_IO2            IOMODE_FUNC1
#define IOMODE_USCI6_IO0            IOMODE_FUNC1
#define IOMODE_USCI6_IO1            IOMODE_FUNC1
#define IOMODE_USCI6_IO2            IOMODE_FUNC1
#define IOMODE_USCI7_IO0            IOMODE_FUNC1
#define IOMODE_USCI7_IO1            IOMODE_FUNC1
#define IOMODE_USCI7_IO2            IOMODE_FUNC1

#define IOMODE_QSPI0_SS             IOMODE_FUNC2
#define IOMODE_QSPI0_CLK            IOMODE_FUNC2
#define IOMODE_QSPI0_DIO0           IOMODE_FUNC2
#define IOMODE_QSPI0_DIO1           IOMODE_FUNC2
#define IOMODE_QSPI0_DIO2           IOMODE_FUNC2
#define IOMODE_QSPI0_DIO3           IOMODE_FUNC2

#define IOMODE_QSPI1_SS             IOMODE_FUNC2
#define IOMODE_QSPI1_CLK            IOMODE_FUNC2
#define IOMODE_QSPI1_DIO0           IOMODE_FUNC2
#define IOMODE_QSPI1_DIO1           IOMODE_FUNC2
#define IOMODE_QSPI1_DIO2           IOMODE_FUNC2
#define IOMODE_QSPI1_DIO3           IOMODE_FUNC2

#define IOMODE_TIMERA_PWM_IO0       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO1       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO2       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO3       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO4       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO5       IOMODE_FUNC3
#define IOMODE_TIMERA_PWM_IO6       IOMODE_FUNC3

#define IOMODE_CAN0_RXD             IOMODE_FUNC4
#define IOMODE_CAN0_TXD             IOMODE_FUNC4
#define IOMODE_CAN1_RXD             IOMODE_FUNC4
#define IOMODE_CAN1_TXD             IOMODE_FUNC4

#define IOMODE_RTC_PULSE            IOMODE_FUNC5

#define IOMODE_OSPI0_SS             IOMODE_FUNC6
#define IOMODE_OSPI0_CLK            IOMODE_FUNC6
#define IOMODE_OSPI0_DIO0           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO1           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO2           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO3           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO4           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO5           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO6           IOMODE_FUNC6
#define IOMODE_OSPI0_DIO7           IOMODE_FUNC6

#define IOMODE_OSPI1_SS             IOMODE_FUNC6
#define IOMODE_OSPI1_CLK            IOMODE_FUNC6
#define IOMODE_OSPI1_DIO0           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO1           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO2           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO3           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO4           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO5           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO6           IOMODE_FUNC6
#define IOMODE_OSPI1_DIO7           IOMODE_FUNC6

#define IOMODE_I2C_CLK              IOMODE_FUNC7
#define IOMODE_I2C_SDA              IOMODE_FUNC7

// uint16_t PinMux_Set(PINMUX_T *pinmux, uint8_t Mux, uint8_t u8PinNum, uint32_t ModeFunc);
uint16_t PinMux_Set(GPIO_T *gpio, uint8_t u8PinNum, uint32_t ModeFunc);

#ifdef __cplusplus
}
#endif

#endif //__PINMUX_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
