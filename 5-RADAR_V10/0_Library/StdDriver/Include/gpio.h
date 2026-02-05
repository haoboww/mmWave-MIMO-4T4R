/******************************************************************************
 * @Filename     : gpio.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/05/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : GPIO driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define GPIO_INVALID_PARA                  0x0501

/*---------------------- General Purpose I/O ---------------------------------*/
#define GPIO_PIN_0                         BIT0       /*!< Select pin 0 */
#define GPIO_PIN_1                         BIT1       /*!< Select pin 1 */
#define GPIO_PIN_2                         BIT2       /*!< Select pin 2 */
#define GPIO_PIN_3                         BIT3       /*!< Select pin 3 */
#define GPIO_PIN_4                         BIT4       /*!< Select pin 4 */
#define GPIO_PIN_5                         BIT5       /*!< Select pin 5 */
#define GPIO_PIN_6                         BIT6       /*!< Select pin 6 */
#define GPIO_PIN_7                         BIT7       /*!< Select pin 7 */
#define GPIO_PIN_8                         BIT8       /*!< Select pin 8 */
#define GPIO_PIN_9                         BIT9       /*!< Select pin 9 */
#define GPIO_PIN_10                        BIT10      /*!< Select pin 10 */
#define GPIO_PIN_11                        BIT11      /*!< Select pin 11 */
#define GPIO_PIN_12                        BIT12      /*!< Select pin 12 */
#define GPIO_PIN_13                        BIT13      /*!< Select pin 13 */
#define GPIO_PIN_14                        BIT14      /*!< Select pin 14 */
#define GPIO_PIN_15                        BIT15      /*!< Select pin 15 */

#define GPIO_DOUT_Pos                      (0)
#define GPIO_DOUT_Msk                      (0xfffffffful << GPIO_DOUT_Pos)

#define GPIO_DOUTMASK_Pos                  (0)
#define GPIO_DOUTMASK_Msk                  (0xfffffffful << GPIO_DOUTMASK_Pos)

#define GPIO_PIN_Pos                       (0)
#define GPIO_PIN_Msk                       (0xfffffffful << GPIO_PIN_Pos)

#define GPIO_INTEN_Pos                     (0)
#define GPIO_INTEN_Msk                     (0xfffffffful << GPIO_INTEN_Pos)

#define GPIO_INTMASK_Pos                   (0)
#define GPIO_INTMASK_Msk                   (0xfffffffful << GPIO_INTMASK_Pos)

#define GPIO_INTTYPE_Pos                   (0)
#define GPIO_INTTYPE_Msk                   (0xfffffffful << GPIO_INTTYPE_Pos)

#define GPIO_INTSTR_Pos                    (0)
#define GPIO_INTSTR_Msk                    (0xfffffffful << GPIO_INTSTR_Pos)

#define GPIO_PIN_MAX                        (16)                    // Specify Maximum Pins of Each GPIO Port

////////////////////////////////////////////////////////////////////////////////////////
//  Analog/Digital Pad Select Definitions                                                        //
////////////////////////////////////////////////////////////////////////////////////////
#define DIGITAL_MODE                0
#define ANALOG_MODE                 1

////////////////////////////////////////////////////////////////////////////////////////
//  Normal/Test Mode Select Definitions                                                        //
////////////////////////////////////////////////////////////////////////////////////////
#define NORMAL_MODE                 0
#define TEST_MODE                   1

////////////////////////////////////////////////////////////////////////////////////////
//  RE Control Definitions                                                        //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_RE_ON                  1
#define GPIO_RE_OFF                 0

////////////////////////////////////////////////////////////////////////////////////////
//  OD Control Definitions                                                        //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_OD_ON                  1
#define GPIO_OD_OFF                 0

////////////////////////////////////////////////////////////////////////////////////////
//  PMODE Constant Definitions                                                        //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_PMODE_INPUT            0x0UL           // Input Mode
#define GPIO_PMODE_OUTPUT           0x1UL           // Output Mode

////////////////////////////////////////////////////////////////////////////////////////
//  GPIO Interrupt Type Constant Definitions                                          //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_INTTYPE_EDGE           0x0UL           // INTTYPE Setting for Edge Trigger Mode
#define GPIO_INTTYPE_LEVEL          0x1UL           // INTTYPE Setting for Edge Level Mode

////////////////////////////////////////////////////////////////////////////////////////
//  GPIO Interrupt Polarity Constant Definitions                                      //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_INTPOL_RISING          0x0UL           // Interrupt enable by Input Rising Edge
#define GPIO_INTPOL_FALLING         0x1UL           // Interrupt enable by Input Falling Edge
#define GPIO_INTPOL_BOTH_EDGE       0x2UL           // Interrupt enable by both Rising Edge and Falling Edge
#define GPIO_INTPOL_LOW             0x0UL           // Interrupt enable by Level-Level
#define GPIO_INTPOL_HIGH            0x1UL           // Interrupt enable by Level-High

#define GPIO_PIN_ADDR(port, pin)    (*((volatile uint32_t *)((GPIO0_PIN_DATA_BASE+(0x40*(port))) + ((pin)<<2))))

#define GPIO0_0                      GPIO_PIN_ADDR(0, 0)
#define GPIO0_1                      GPIO_PIN_ADDR(0, 1)
#define GPIO0_2                      GPIO_PIN_ADDR(0, 2)
#define GPIO0_3                      GPIO_PIN_ADDR(0, 3)
#define GPIO0_4                      GPIO_PIN_ADDR(0, 4)
#define GPIO0_5                      GPIO_PIN_ADDR(0, 5)
#define GPIO0_6                      GPIO_PIN_ADDR(0, 6)
#define GPIO0_7                      GPIO_PIN_ADDR(0, 7)
#define GPIO0_8                      GPIO_PIN_ADDR(0, 8)
#define GPIO0_9                      GPIO_PIN_ADDR(0, 9)
#define GPIO0_10                     GPIO_PIN_ADDR(0, 10)
#define GPIO0_11                     GPIO_PIN_ADDR(0, 11)
#define GPIO0_12                     GPIO_PIN_ADDR(0, 12)
#define GPIO0_13                     GPIO_PIN_ADDR(0, 13)
#define GPIO0_14                     GPIO_PIN_ADDR(0, 14)
#define GPIO0_15                     GPIO_PIN_ADDR(0, 15)

#define GPIO1_0                      GPIO_PIN_ADDR(1, 0)
#define GPIO1_1                      GPIO_PIN_ADDR(1, 1)
#define GPIO1_2                      GPIO_PIN_ADDR(1, 2)
#define GPIO1_3                      GPIO_PIN_ADDR(1, 3)
#define GPIO1_4                      GPIO_PIN_ADDR(1, 4)
#define GPIO1_5                      GPIO_PIN_ADDR(1, 5)
#define GPIO1_6                      GPIO_PIN_ADDR(1, 6)
#define GPIO1_7                      GPIO_PIN_ADDR(1, 7)
#define GPIO1_8                      GPIO_PIN_ADDR(1, 8)
#define GPIO1_9                      GPIO_PIN_ADDR(1, 9)
#define GPIO1_10                     GPIO_PIN_ADDR(1,10)
#define GPIO1_11                     GPIO_PIN_ADDR(1,11)
#define GPIO1_12                     GPIO_PIN_ADDR(1,12)
#define GPIO1_13                     GPIO_PIN_ADDR(1,13)
#define GPIO1_14                     GPIO_PIN_ADDR(1,14)
#define GPIO1_15                     GPIO_PIN_ADDR(1,15)

#define GPIO2_0                      GPIO_PIN_ADDR(2, 0)
#define GPIO2_1                      GPIO_PIN_ADDR(2, 1)
#define GPIO2_2                      GPIO_PIN_ADDR(2, 2)
#define GPIO2_3                      GPIO_PIN_ADDR(2, 3)
#define GPIO2_4                      GPIO_PIN_ADDR(2, 4)
#define GPIO2_5                      GPIO_PIN_ADDR(2, 5)
#define GPIO2_6                      GPIO_PIN_ADDR(2, 6)
#define GPIO2_7                      GPIO_PIN_ADDR(2, 7)
#define GPIO2_8                      GPIO_PIN_ADDR(2, 8)
#define GPIO2_9                      GPIO_PIN_ADDR(2, 9)
#define GPIO2_10                     GPIO_PIN_ADDR(2,10)
#define GPIO2_11                     GPIO_PIN_ADDR(2,11)
#define GPIO2_12                     GPIO_PIN_ADDR(2,12)
#define GPIO2_13                     GPIO_PIN_ADDR(2,13)
#define GPIO2_14                     GPIO_PIN_ADDR(2,14)
#define GPIO2_15                     GPIO_PIN_ADDR(2,15)

#define GPIO3_0                      GPIO_PIN_ADDR(3, 0)
#define GPIO3_1                      GPIO_PIN_ADDR(3, 1)
#define GPIO3_2                      GPIO_PIN_ADDR(3, 2)
#define GPIO3_3                      GPIO_PIN_ADDR(3, 3)
#define GPIO3_4                      GPIO_PIN_ADDR(3, 4)
#define GPIO3_5                      GPIO_PIN_ADDR(3, 5)
#define GPIO3_6                      GPIO_PIN_ADDR(3, 6)
#define GPIO3_7                      GPIO_PIN_ADDR(3, 7)
#define GPIO3_8                      GPIO_PIN_ADDR(3, 8)
#define GPIO3_9                      GPIO_PIN_ADDR(3, 9)
#define GPIO3_10                     GPIO_PIN_ADDR(3,10)
#define GPIO3_11                     GPIO_PIN_ADDR(3,11)
#define GPIO3_12                     GPIO_PIN_ADDR(3,12)
#define GPIO3_13                     GPIO_PIN_ADDR(3,13)
#define GPIO3_14                     GPIO_PIN_ADDR(3,14)
#define GPIO3_15                     GPIO_PIN_ADDR(3,15)

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Clear GPIO Pin Interrupt Flag                                         //
//                                                                                    //
// @param[in]   gpio        GPIO port. It could be GPIO0, GPIO1                       //
// @param[in]   u32PinMask  The single or multiple pins of specified GPIO port        //
//                                                                                    //
// @return      None                                                                  //
//                                                                                    //
// @details     Clear the interrupt status of specified GPIO pin.                     //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_CLR_INT_FLAG(gpio, u32PinMask)         ((gpio)->INTSTR = u32PinMask)


////////////////////////////////////////////////////////////////////////////////////////
// @brief       Disable I/O DOUT mask                                                 //
//                                                                                    //
// @param[in]   gpio        GPIO port. It could be GPIO0, GPIO1                       //
// @param[in]   u32PinMask  The single or multiple pins of specified GPIO port        //
//                                                                                    //
// @return      None                                                                  //
//                                                                                    //
// @details     Disable I/O DOUT mask of specified GPIO pin.                          //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_DISABLE_DOUT_Msk(gpio, u32PinMask)    ((gpio)->DOUTMASK &= ~u32PinMask)

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Enable I/O DOUT mask                                                  //
//                                                                                    //
// @param[in]   gpio        GPIO port. It could be GPIO0, GPIO1                       //
// @param[in]   u32PinMask  The single or multiple pins of specified GPIO port        //
//                                                                                    //
// @return      None                                                                  //
//                                                                                    //
// @details     Enable I/O DOUT mask of specified GPIO pin.                           //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_ENABLE_DOUT_Msk(gpio, u32PinMask)     ((gpio)->DOUTMASK |= u32PinMask)

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Get GPIO Pin Interrupt Flag                                           //
//                                                                                    //
// @param[in]   gpio        GPIO port. It could be GPIO0, GPIO1                       //
// @param[in]   u32PinMask  The single or multiple pins of specified GPIO port        //
//                                                                                    //
// @retval      0           No interrupt at specified GPIO pin                        //
// @retval      1           The specified GPIO pin generate an interrupt              //
//                                                                                    //
// @details     Get the interrupt status of specified GPIO pin.                       //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_GET_INT_FLAG(gpio, u32PinMask)         ((gpio)->INTSTR & u32PinMask)

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Get GPIO Port IN Data                                                 //
//                                                                                    //
// @param[in]   gpio GPIO port. It could be GPIO0, GPIO1                              //
//                                                                                    //
// @retval      The specified port data                                               //
//                                                                                    //
// @details     Get the PIN register of specified GPIO port.                          //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_GET_IN_DATA(gpio)   ((gpio)->PIN)
#define GPIO_GET_IN_BIT(gpio, u32Pin)        (((gpio)->PIN & u32Pin) != 0 ? 1 : 0)

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Set GPIO Port OUT Data                                                //
//                                                                                    //
// @param[in]   gpio        GPIO port. It could be GPIO0, GPIO1                       //
// @param[in]   u32Data     GPIO port data                                            //
//                                                                                    //
// @retval      None                                                                  //
//                                                                                    //
// @details     Set the Data into specified GPIO port.                                //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_SET_OUT_DATA(gpio, u32Data)   ((gpio)->DOUT = (u32Data))

////////////////////////////////////////////////////////////////////////////////////////
// @brief       Toggle Specified GPIO pin                                             //
//                                                                                    //
// @param[in]   u32Pin  GPIOx_y x is {0,1}, y is {0,1,...,31}                         //
//                                                                                    //
// @retval      None                                                                  //
//                                                                                    //
// @details     Toggle the specified GPIO pint.                                       //
////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_TOGGLE(u32Pin)   ((u32Pin) ^= 1)

void GPIO_SetMode(GPIO_T *gpio, uint32_t u32PinMask, uint32_t u32Mode);
void GPIO_EnableInt(GPIO_T *gpio, uint32_t u32Pin, uint32_t u32IntType, uint32_t u32IntPol);
void GPIO_DisableInt(GPIO_T *gpio, uint32_t u32Pin);
uint16_t GPIO_AE_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8Mode);
uint16_t GPIO_AT_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8Mode);
uint16_t GPIO_RE_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8RE);
uint16_t GPIO_PullUp(GPIO_T *gpio, uint32_t u32PinMask);
uint16_t GPIO_PullDown(GPIO_T *gpio, uint32_t u32PinMask);
uint16_t GPIO_OD_Ctrl(GPIO_T *gpio, uint32_t u32PinMask, uint8_t u8OD);

#ifdef __cplusplus
}
#endif

#endif // __GPIO_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
