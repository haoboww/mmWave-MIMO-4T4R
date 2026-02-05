/******************************************************************************
 * @Filename     : SKY32B750.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/16/2022 9:25:01
 * @Modified by  : CB
 *
 * @Brief        : SKY32B750 series peripheral access layer header file.
 *                 This file contains all the peripheral register's definitions,
 *                 bits definitions and memory mapping for SKY32B750 series MCU.
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __SKY32B750_H__
#define __SKY32B750_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SKY32B750_Definitions SKY32B750 Definitions
  This file defines all structures and symbols for SKY32B750:
    - interrupt numbers
    - registers and bit fields
    - peripheral base address
    - peripheral ID
    - Peripheral definitions
  @{
*/

/******************************************************************************/
/*                Processor and Core Peripherals                              */
/******************************************************************************/
/** @addtogroup SKY32B750_CMSIS Device CMSIS Definitions
  Configuration of the Cortex-M4 Processor and Core Peripherals
  @{
*/

/**
 * @details  Interrupt Number Definition. The maximum of 32 Specific Interrupts are possible.
 */
typedef enum IRQn {
    /******  Cortex-M4 Processor Exceptions Numbers *****************************************/
    /* Auxiliary constants */
    NotAvail_IRQn                = -128,             /**< Not available device specific interrupt */

    /* Core interrupts */
    NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
    HardFault_IRQn               = -13,              /**< Cortex-M4 SV Hard Fault Interrupt */
    MemoryManagement_IRQn        = -12,              /**< Cortex-M4 Memory Management Interrupt */
    BusFault_IRQn                = -11,              /**< Cortex-M4 Bus Fault Interrupt */
    UsageFault_IRQn              = -10,              /**< Cortex-M4 Usage Fault Interrupt */
    SVCall_IRQn                  = -5,               /**< Cortex-M4 SV Call Interrupt */
    DebugMonitor_IRQn            = -4,               /**< Cortex-M4 Debug Monitor Interrupt */
    PendSV_IRQn                  = -2,               /**< Cortex-M4 Pend SV Interrupt */
    SysTick_IRQn                 = -1,               /**< Cortex-M4 System Tick Interrupt */

    /******  SKY32B750 specific Interrupt Numbers ***********************************************/
    RES_IRQn              = 0,      /* RES interrupt                                             */
    WDT_IRQn              = 1,      /* Watch Dog Timer interrupt                                 */
    RTC_IRQn              = 2,      /* RTC interrupt                                             */
    USCI0_IRQn            = 3,      /* USCI0 interrupt                                           */
    USCI1_IRQn            = 4,      /* USCI1 interrupt                                           */
    USCI2_IRQn            = 5,      /* USCI2 interrupt                                           */
    USCI3_IRQn            = 6,      /* USCI3 interrupt                                           */
    USCI4_IRQn            = 7,      /* USCI4 interrupt                                           */
    USCI5_IRQn            = 8,      /* USCI5 interrupt                                           */
    USCI6_IRQn            = 9,      /* USCI6 interrupt                                           */
    USCI7_IRQn            = 10,     /* USCI7 interrupt                                           */
    QSPI0_IRQn            = 11,     /* QSPI0 interrupt                                           */
    QSPI1_IRQn            = 12,     /* QSPI1 interrupt                                           */
    TMRA_IRQn             = 13,     /* TimerA interrupt                                          */
    TMRB0_IRQn            = 14,     /* TimerB 0 interrupt                                        */
    TMRB1_IRQn            = 15,     /* TimerB 1 interrupt                                        */
    TMRB2_IRQn            = 16,     /* TimerB 2 interrupt                                        */
    TMRB3_IRQn            = 17,     /* TimerB 3 interrupt                                        */
    WWDG_IRQn             = 18,     /* WWDG interrupt                                            */
    DMA_IRQn              = 19,     /* DMA interrupt                                             */
    BB_IRQn               = 20,     /* BB interrupt                                              */
    I2C_IRQn              = 21,     /* I2C interrupt                                             */
    GPIO0_IRQn            = 22,     /* GPIO0 interrupt                                           */
    GPIO0_E0_IRQn         = 23,     /* GPIO0 interrupt                                           */
    GPIO0_E1_IRQn         = 24,     /* GPIO0 interrupt                                           */
    GPIO1_IRQn            = 25,     /* GPIO1 interrupt                                           */
    GPIO1_E0_IRQn         = 26,     /* GPIO1 interrupt                                           */
    GPIO1_E1_IRQn         = 27,     /* GPIO1 interrupt                                           */
    GPIO2_IRQn            = 28,     /* GPIO2 interrupt                                           */
    GPIO2_E0_IRQn         = 29,     /* GPIO2 interrupt                                           */
    GPIO2_E1_IRQn         = 30,     /* GPIO2 interrupt                                           */
    GPIO3_IRQn            = 31,     /* GPIO3 interrupt                                           */
    GPIO3_E0_IRQn         = 32,     /* GPIO3 interrupt                                           */
    GPIO3_E1_IRQn         = 33,     /* GPIO3 interrupt                                           */
    CAN0_IRQn             = 34,     /* CAN0 interrupt                                            */
    CAN1_IRQn             = 35,     /* CAN1 interrupt                                            */
    BOR_IRQn              = 36,     /* BOR interrupt                                             */
    SAR_IRQn              = 37,     /* SAR interrupt                                             */
    OSPI0_IRQn            = 38,     /* OSPI0 interrupt                                           */
    OSPI1_IRQn            = 39,     /* OSPI1 interrupt                                           */
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M7 Processor and Core Peripherals */
#define __CM7_REV                      0x0001    /* Core revision r0p1   */
#define __MPU_PRESENT                  1         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               3         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */
#define __FPU_PRESENT                  1         /**< Defines if an FPU is present or not */
#define __ICACHE_PRESENT               1         /*!< Set to 1 if I-Cache is present                      */
#define __DCACHE_PRESENT               1         /*!< Set to 1 if D-Cache is present                      */

//#define __DTCM_PRESENT                 1         /**< Defines if an DTCM is present or not */

//#define __MPU_PRESENT           1       /*!< cm7ikmcu can support an MPU                         */
//#define __NVIC_PRIO_BITS        2       /*!< cm7ikmcu Supports 2 Bits for the Priority Levels    */
//#define __VTOR_PRESENT          1       /*!< cm7ikmcu can support the VTOR                       */
//#define __Vendor_SysTickConfig  0       /*!< Set to 1 if different SysTick Config is used        */
//#define __ICACHE_PRESENT        1       /*!< Set to 1 if I-Cache is present                      */
//#define __DCACHE_PRESENT        1       /*!< Set to 1 if D-Cache is present                      */
//#define __FPU_PRESENT           1       /*!< Set to 1 if FPU is present                          */

/*@}*/ /* end of group SKY32B750_CMSIS */
#include "core_cm7.h"                      /* Cortex-M4 processor and core peripherals           */
#include "system_SKY32B750.h"                /* SKY32B750 Series System include file                 */
#include <stdint.h>


/**
 * Initialize the system clock
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system
 *         Initialize the PLL and update the SystemFrequency variable
 */
extern void SystemInit (void);




/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/
//#if defined ( __CC_ARM  )
#pragma anon_unions
//#endif



/*--------------------- WDT ---------------------------------*/
//WDT Base 0x40084000
typedef struct {
    __IO uint32_t RES;                     /*!< Offset: 0x000 () */
    __IO uint32_t STR;                     /*!< Offset: 0x004 (R/W)  WDT State Register */
    __IO uint32_t CMP;                     /*!< Offset: 0x008 (R/W)  WDT Comparator Register */
    __IO uint32_t FCNT;                    /*!< Offset: 0x00C (R)    WDT Internal Free Counter Register */
} WDT_T;

//WDT_STR
#define WDT_CLR_Pos                        (0)
#define WDT_CLR_Msk                        (0x1ul << WDT_CLR_Pos)
#define WDT_INTF_Pos                       (1)
#define WDT_INTF_Msk                       (0x1ul << WDT_INTF_Pos)
#define WDT_EN_STA_Pos                     (3)
#define WDT_EN_STA_Msk                     (0x1u1 << WDT_EN_STA_Pos)

//WDT_CMP
#define WDT_CMP_Pos                        (0)
#define WDT_CMP_Msk                        (0xFFFFul << WDT_CMP_Pos)

//WDT_FCNT
#define WDT_CMP_Pos                        (0)
#define WDT_CMP_Msk                        (0xFFFFul << WDT_CMP_Pos)

/*---------------------- END ---------------------------------*/

/*--------------------- RTC ---------------------------------*/
//RTC Base 0x40085000
typedef struct {
    __IO uint32_t CLR;                     /*!< Offset: 0x000 (WP)  RTC Free Counter Clear Register */
    __IO uint32_t CNT0;                    /*!< Offset: 0x004 (RO)  RTC Free Counter [31 : 0] Register */
    __IO uint32_t CNT1;                    /*!< Offset: 0x008 (RO)  RTC Free Counter [47 : 32] Register */
    __IO uint32_t WKU_CMP;                 /*!< Offset: 0x00C (WR)  RTC Comparator Register */
    __IO uint32_t PUS_CFG;                 /*!< Offset: 0x010 (WR)  RTC Pulse Configuration Register */
    __IO uint32_t WKU_CFG;                 /*!< Offset: 0x014 (WR)  RTC wakeup Configuration Register */
    __IO uint32_t STA;                     /*!< Offset: 0x018 (RO)  RTC Status Register */
} RTC_T;

//RTC_CLR
#define RTC_CLR_Pos                        (0)
#define RTC_CLR_Msk                        (0x1ul << RTC_CLR_Pos)

//RTC_WKU_CMP
#define RTC_WKU_CMP_Pos                    (0)
#define RTC_WKU_CMP_Msk                    (0xFFFFul << RTC_WKU_CMP_Pos)

//RTC_PUS_CON
#define RTC_PUS_HIGH_Pos                   (0)
#define RTC_PUS_HIGH_Msk                   (0x3ul << RTC_PUS_HIGH_Pos)
#define RTC_PUS_LOW_Pos                    (2)
#define RTC_PUS_LOW_Msk                    (0xful << RTC_PUS_LOW_Pos)
#define RTC_PUS_INV_Pos                    (6)
#define RTC_PUS_INV_Msk                    (0x1ul << RTC_PUS_INV_Pos)
#define RTC_PUS_EN_Pos                     (7)
#define RTC_PUS_EN_Msk                     (0x1ul << RTC_PUS_EN_Pos)

//RTC_WKU_CFG
#define RTC_CLK_SEL_Pos                    (0)
#define RTC_CLK_SEL_Msk                    (0x1ul << RTC_CLK_SEL_Pos)   //0: 0.488ms per step; 1: 0.5s per step
#define RTC_WKU_MODE_Pos                   (1)
#define RTC_WKU_MODE_Msk                   (0x1ul << RTC_WKU_MODE_Pos)  //0: wakeup once; 1: wakeup repeat

//RTC_STA
#define RTC_EN_STA_Pos                     (0)
#define RTC_EN_STA_Msk                     (0x1ul << RTC_EN_STA_Pos)
#define RTC_XTAL_32K_STA_Pos               (1)
#define RTC_XTAL_32K_STA_Msk               (0x1ul << RTC_XTAL_32K_STA)
/*---------------------- END ---------------------------------*/

/*---------------------- AON_WKU ---------------------------------*/
typedef struct {
    __IO uint32_t WKUP_MASK0;        /*!< Offset: 0x000 (R/W)  Wakup mask Register 0 */
    __IO uint32_t WKUN_MASK0;        /*!< Offset: 0x004 (R/W)  Wakup mask Register 0 */
    __IO uint32_t WKU_FLAG0 ;        /*!< Offset: 0x008 (R/W)  Wakup flag Register 0 */
    __IO uint32_t WKUP_MASK1;        /*!< Offset: 0x00C (R/W)  Wakup mask Register 1 */
    __IO uint32_t WKUN_MASK1;        /*!< Offset: 0x010 (R/W)  Wakup mask Register 1 */
    __IO uint32_t WKU_FLAG1 ;        /*!< Offset: 0x014 (R/W)  Wakup flag Register 1 */
    __IO uint32_t WKUP_MASK2;        /*!< Offset: 0x00C (R/W)  Wakup mask Register 2 */
    __IO uint32_t RES0;              /*!< Offset: 0x010 (R/W)  Register */
    __IO uint32_t WKU_FLAG2 ;        /*!< Offset: 0x014 (R/W)  Wakup flag Register 2 */
} AON_WKU_T;

// WKUP_MASK0 & WKUP_MASK1
#define AON_WKU_IO_WKUP_MASK0_Pos              (0)
#define AON_WKU_IO_WKUP_MASK0_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK0_Pos)
#define AON_WKU_IO_WKUP_MASK1_Pos              (1)
#define AON_WKU_IO_WKUP_MASK1_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK1_Pos)
#define AON_WKU_IO_WKUP_MASK2_Pos              (2)
#define AON_WKU_IO_WKUP_MASK2_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK2_Pos)
#define AON_WKU_IO_WKUP_MASK3_Pos              (3)
#define AON_WKU_IO_WKUP_MASK3_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK3_Pos)
#define AON_WKU_IO_WKUP_MASK4_Pos              (4)
#define AON_WKU_IO_WKUP_MASK4_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK4_Pos)
#define AON_WKU_IO_WKUP_MASK5_Pos              (5)
#define AON_WKU_IO_WKUP_MASK5_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK5_Pos)
#define AON_WKU_IO_WKUP_MASK6_Pos              (6)
#define AON_WKU_IO_WKUP_MASK6_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK6_Pos)
#define AON_WKU_IO_WKUP_MASK7_Pos              (7)
#define AON_WKU_IO_WKUP_MASK7_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK7_Pos)
#define AON_WKU_IO_WKUP_MASK8_Pos              (8)
#define AON_WKU_IO_WKUP_MASK8_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK8_Pos)
#define AON_WKU_IO_WKUP_MASK9_Pos              (9)
#define AON_WKU_IO_WKUP_MASK9_Msk              (0x1ul << AON_WKU_IO_WKUP_MASK9_Pos)
#define AON_WKU_IO_WKUP_MASK10_Pos             (10)
#define AON_WKU_IO_WKUP_MASK10_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK10_Pos)
#define AON_WKU_IO_WKUP_MASK11_Pos             (11)
#define AON_WKU_IO_WKUP_MASK11_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK11_Pos)
#define AON_WKU_IO_WKUP_MASK12_Pos             (12)
#define AON_WKU_IO_WKUP_MASK12_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK12_Pos)
#define AON_WKU_IO_WKUP_MASK13_Pos             (13)
#define AON_WKU_IO_WKUP_MASK13_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK13_Pos)
#define AON_WKU_IO_WKUP_MASK14_Pos             (14)
#define AON_WKU_IO_WKUP_MASK14_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK14_Pos)
#define AON_WKU_IO_WKUP_MASK15_Pos             (15)
#define AON_WKU_IO_WKUP_MASK15_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK15_Pos)
#define AON_WKU_IO_WKUP_MASK16_Pos             (16)
#define AON_WKU_IO_WKUP_MASK16_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK16_Pos)
#define AON_WKU_IO_WKUP_MASK17_Pos             (17)
#define AON_WKU_IO_WKUP_MASK17_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK17_Pos)
#define AON_WKU_IO_WKUP_MASK18_Pos             (18)
#define AON_WKU_IO_WKUP_MASK18_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK18_Pos)
#define AON_WKU_IO_WKUP_MASK19_Pos             (19)
#define AON_WKU_IO_WKUP_MASK19_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK19_Pos)
#define AON_WKU_IO_WKUP_MASK20_Pos             (20)
#define AON_WKU_IO_WKUP_MASK20_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK20_Pos)
#define AON_WKU_IO_WKUP_MASK21_Pos             (21)
#define AON_WKU_IO_WKUP_MASK21_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK21_Pos)
#define AON_WKU_IO_WKUP_MASK22_Pos             (22)
#define AON_WKU_IO_WKUP_MASK22_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK22_Pos)
#define AON_WKU_IO_WKUP_MASK23_Pos             (23)
#define AON_WKU_IO_WKUP_MASK23_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK23_Pos)
#define AON_WKU_IO_WKUP_MASK24_Pos             (24)
#define AON_WKU_IO_WKUP_MASK24_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK24_Pos)
#define AON_WKU_IO_WKUP_MASK25_Pos             (25)
#define AON_WKU_IO_WKUP_MASK25_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK25_Pos)
#define AON_WKU_IO_WKUP_MASK26_Pos             (26)
#define AON_WKU_IO_WKUP_MASK26_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK26_Pos)
#define AON_WKU_IO_WKUP_MASK27_Pos             (27)
#define AON_WKU_IO_WKUP_MASK27_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK27_Pos)
#define AON_WKU_IO_WKUP_MASK28_Pos             (28)
#define AON_WKU_IO_WKUP_MASK28_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK28_Pos)
#define AON_WKU_IO_WKUP_MASK29_Pos             (29)
#define AON_WKU_IO_WKUP_MASK29_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK29_Pos)
#define AON_WKU_IO_WKUP_MASK30_Pos             (30)
#define AON_WKU_IO_WKUP_MASK30_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK30_Pos)
#define AON_WKU_IO_WKUP_MASK31_Pos             (31)
#define AON_WKU_IO_WKUP_MASK31_Msk             (0x1ul << AON_WKU_IO_WKUP_MASK31_Pos)

// WKUN_MASK0 & WKUN_MASK1
#define AON_WKU_IO_WKUN_MASK0_Pos              (0)
#define AON_WKU_IO_WKUN_MASK0_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK0_Pos)
#define AON_WKU_IO_WKUN_MASK1_Pos              (1)
#define AON_WKU_IO_WKUN_MASK1_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK1_Pos)
#define AON_WKU_IO_WKUN_MASK2_Pos              (2)
#define AON_WKU_IO_WKUN_MASK2_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK2_Pos)
#define AON_WKU_IO_WKUN_MASK3_Pos              (3)
#define AON_WKU_IO_WKUN_MASK3_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK3_Pos)
#define AON_WKU_IO_WKUN_MASK4_Pos              (4)
#define AON_WKU_IO_WKUN_MASK4_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK4_Pos)
#define AON_WKU_IO_WKUN_MASK5_Pos              (5)
#define AON_WKU_IO_WKUN_MASK5_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK5_Pos)
#define AON_WKU_IO_WKUN_MASK6_Pos              (6)
#define AON_WKU_IO_WKUN_MASK6_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK6_Pos)
#define AON_WKU_IO_WKUN_MASK7_Pos              (7)
#define AON_WKU_IO_WKUN_MASK7_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK7_Pos)
#define AON_WKU_IO_WKUN_MASK8_Pos              (8)
#define AON_WKU_IO_WKUN_MASK8_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK8_Pos)
#define AON_WKU_IO_WKUN_MASK9_Pos              (9)
#define AON_WKU_IO_WKUN_MASK9_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK9_Pos)
#define AON_WKU_IO_WKUN_MASK10_Pos             (10)
#define AON_WKU_IO_WKUN_MASK10_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK10_Pos)
#define AON_WKU_IO_WKUN_MASK11_Pos             (11)
#define AON_WKU_IO_WKUN_MASK11_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK11_Pos)
#define AON_WKU_IO_WKUN_MASK12_Pos             (12)
#define AON_WKU_IO_WKUN_MASK12_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK12_Pos)
#define AON_WKU_IO_WKUN_MASK13_Pos             (13)
#define AON_WKU_IO_WKUN_MASK13_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK13_Pos)
#define AON_WKU_IO_WKUN_MASK14_Pos             (14)
#define AON_WKU_IO_WKUN_MASK14_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK14_Pos)
#define AON_WKU_IO_WKUN_MASK15_Pos             (15)
#define AON_WKU_IO_WKUN_MASK15_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK15_Pos)
#define AON_WKU_IO_WKUN_MASK16_Pos             (16)
#define AON_WKU_IO_WKUN_MASK16_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK16_Pos)
#define AON_WKU_IO_WKUN_MASK17_Pos             (17)
#define AON_WKU_IO_WKUN_MASK17_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK17_Pos)
#define AON_WKU_IO_WKUN_MASK18_Pos             (18)
#define AON_WKU_IO_WKUN_MASK18_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK18_Pos)
#define AON_WKU_IO_WKUN_MASK19_Pos             (19)
#define AON_WKU_IO_WKUN_MASK19_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK19_Pos)
#define AON_WKU_IO_WKUN_MASK20_Pos             (20)
#define AON_WKU_IO_WKUN_MASK20_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK20_Pos)
#define AON_WKU_IO_WKUN_MASK21_Pos             (21)
#define AON_WKU_IO_WKUN_MASK21_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK21_Pos)
#define AON_WKU_IO_WKUN_MASK22_Pos             (22)
#define AON_WKU_IO_WKUN_MASK22_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK22_Pos)
#define AON_WKU_IO_WKUN_MASK23_Pos             (23)
#define AON_WKU_IO_WKUN_MASK23_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK23_Pos)
#define AON_WKU_IO_WKUN_MASK24_Pos             (24)
#define AON_WKU_IO_WKUN_MASK24_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK24_Pos)
#define AON_WKU_IO_WKUN_MASK25_Pos             (25)
#define AON_WKU_IO_WKUN_MASK25_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK25_Pos)
#define AON_WKU_IO_WKUN_MASK26_Pos             (26)
#define AON_WKU_IO_WKUN_MASK26_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK26_Pos)
#define AON_WKU_IO_WKUN_MASK27_Pos             (27)
#define AON_WKU_IO_WKUN_MASK27_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK27_Pos)
#define AON_WKU_IO_WKUN_MASK28_Pos             (28)
#define AON_WKU_IO_WKUN_MASK28_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK28_Pos)
#define AON_WKU_IO_WKUN_MASK29_Pos             (29)
#define AON_WKU_IO_WKUN_MASK29_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK29_Pos)
#define AON_WKU_IO_WKUN_MASK30_Pos             (30)
#define AON_WKU_IO_WKUN_MASK30_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK30_Pos)
#define AON_WKU_IO_WKUN_MASK31_Pos             (31)
#define AON_WKU_IO_WKUN_MASK31_Msk             (0x1ul << AON_WKU_IO_WKUN_MASK31_Pos)

// WKU_FLAG0 & WKU_FLAG1
#define AON_WKU_IO_WKU_FLAG0_Pos               (0)
#define AON_WKU_IO_WKU_FLAG0_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG0_Pos)
#define AON_WKU_IO_WKU_FLAG1_Pos               (1)
#define AON_WKU_IO_WKU_FLAG1_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG1_Pos)
#define AON_WKU_IO_WKU_FLAG2_Pos               (2)
#define AON_WKU_IO_WKU_FLAG2_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG2_Pos)
#define AON_WKU_IO_WKU_FLAG3_Pos               (3)
#define AON_WKU_IO_WKU_FLAG3_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG3_Pos)
#define AON_WKU_IO_WKU_FLAG4_Pos               (4)
#define AON_WKU_IO_WKU_FLAG4_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG4_Pos)
#define AON_WKU_IO_WKU_FLAG5_Pos               (5)
#define AON_WKU_IO_WKU_FLAG5_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG5_Pos)
#define AON_WKU_IO_WKU_FLAG6_Pos               (6)
#define AON_WKU_IO_WKU_FLAG6_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG6_Pos)
#define AON_WKU_IO_WKU_FLAG7_Pos               (7)
#define AON_WKU_IO_WKU_FLAG7_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG7_Pos)
#define AON_WKU_IO_WKU_FLAG8_Pos               (8)
#define AON_WKU_IO_WKU_FLAG8_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG8_Pos)
#define AON_WKU_IO_WKU_FLAG9_Pos               (9)
#define AON_WKU_IO_WKU_FLAG9_Msk               (0x1ul << AON_WKU_IO_WKU_FLAG9_Pos)
#define AON_WKU_IO_WKU_FLAG10Pos               (10)
#define AON_WKU_IO_WKU_FLAG10Msk               (0x1ul << AON_WKU_IO_WKU_FLAG10Pos)
#define AON_WKU_IO_WKU_FLAG11Pos               (11)
#define AON_WKU_IO_WKU_FLAG11Msk               (0x1ul << AON_WKU_IO_WKU_FLAG11Pos)
#define AON_WKU_IO_WKU_FLAG12Pos               (12)
#define AON_WKU_IO_WKU_FLAG12Msk               (0x1ul << AON_WKU_IO_WKU_FLAG12Pos)
#define AON_WKU_IO_WKU_FLAG13Pos               (13)
#define AON_WKU_IO_WKU_FLAG13Msk               (0x1ul << AON_WKU_IO_WKU_FLAG13Pos)
#define AON_WKU_IO_WKU_FLAG14Pos               (14)
#define AON_WKU_IO_WKU_FLAG14Msk               (0x1ul << AON_WKU_IO_WKU_FLAG14Pos)
#define AON_WKU_IO_WKU_FLAG15Pos               (15)
#define AON_WKU_IO_WKU_FLAG15Msk               (0x1ul << AON_WKU_IO_WKU_FLAG15Pos)
#define AON_WKU_IO_WKU_FLAG16Pos               (16)
#define AON_WKU_IO_WKU_FLAG16Msk               (0x1ul << AON_WKU_IO_WKU_FLAG16Pos)
#define AON_WKU_IO_WKU_FLAG17Pos               (17)
#define AON_WKU_IO_WKU_FLAG17Msk               (0x1ul << AON_WKU_IO_WKU_FLAG17Pos)
#define AON_WKU_IO_WKU_FLAG18_Pos              (18)
#define AON_WKU_IO_WKU_FLAG18_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK18_Pos)
#define AON_WKU_IO_WKU_FLAG19_Pos              (19)
#define AON_WKU_IO_WKU_FLAG19_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK19_Pos)
#define AON_WKU_IO_WKU_FLAG20_Pos              (20)
#define AON_WKU_IO_WKU_FLAG20_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK20_Pos)
#define AON_WKU_IO_WKU_FLAG21_Pos              (21)
#define AON_WKU_IO_WKU_FLAG21_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK21_Pos)
#define AON_WKU_IO_WKU_FLAG22_Pos              (22)
#define AON_WKU_IO_WKU_FLAG22_Msk              (0x1ul << AON_WKU_IO_WKUN_MASK22_Pos)
#define AON_WKU_IO_WKU_FLAG23_Pos              (23)
#define AON_WKU_IO_WKU_FLAG23_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG23_Pos)
#define AON_WKU_IO_WKU_FLAG24_Pos              (24)
#define AON_WKU_IO_WKU_FLAG24_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG24_Pos)
#define AON_WKU_IO_WKU_FLAG25_Pos              (25)
#define AON_WKU_IO_WKU_FLAG25_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG25_Pos)
#define AON_WKU_IO_WKU_FLAG26_Pos              (26)
#define AON_WKU_IO_WKU_FLAG26_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG26_Pos)
#define AON_WKU_IO_WKU_FLAG27_Pos              (27)
#define AON_WKU_IO_WKU_FLAG27_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG27_Pos)
#define AON_WKU_IO_WKU_FLAG28_Pos              (28)
#define AON_WKU_IO_WKU_FLAG28_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG28_Pos)
#define AON_WKU_IO_WKU_FLAG29_Pos              (29)
#define AON_WKU_IO_WKU_FLAG29_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG29_Pos)
#define AON_WKU_IO_WKU_FLAG30_Pos              (30)
#define AON_WKU_IO_WKU_FLAG30_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG30_Pos)
#define AON_WKU_IO_WKU_FLAG31_Pos              (31)
#define AON_WKU_IO_WKU_FLAG31_Msk              (0x1ul << AON_WKU_IO_WKU_FLAG31_Pos)

//WKUP_MASK2
#define AON_WKU_WDT_MASK_Pos                   (0)
#define AON_WKU_WDT_MASK_Msk                   (0x1ul << AON_WKU_WDT_MASK_Pos)
#define AON_WKU_RTC_MASK_Pos                   (1)
#define AON_WKU_RTC_MASK_Msk                   (0x1ul << AON_WKU_RTC_MASK_Pos)

//WKU_FLAG2
#define AON_WKU_WDT_FLAG2_Pos                  (0)
#define AON_WKU_WDT_FLAG2_Msk                  (0x1ul << AON_WKU_WDT_FLAG2_Pos)
#define AON_WKU_RTC_FLAG2_Pos                  (1)
#define AON_WKU_RTC_FLAG2_Msk                  (0x1ul << AON_WKU_RTC_FLAG2_Pos)

/*---------------------- END ---------------------------------*/

/*---------------------- AON_SYSC ---------------------------------*/
/** 0x40080000 */
typedef struct {
    __I  uint32_t RES0[1]         ;   /*!< Offset: 0x000 (R/W)  */
    __IO uint32_t RWRP            ;   /*!< Offset: 0x004 (R/W)  Register write protect Register 0x5A,0x26,0x88 */
    __IO uint32_t RES1[1]         ;   /*!< Offset: 0x008 (R/W)  */
    __IO uint32_t STA             ;   /*!< Offset: 0x00c (R/W)  Status Register por33,por_lp,io_rst,wdt_rst,write 1 clear*/
    __IO uint32_t IOLATCH         ;   /*!< Offset: 0x010 (R/W)  GPIO Latch Register */
    __IO uint32_t SWD_PEN         ;   /*!< Offset: 0x014 (R/W)  SWD Port Enable Register */
    __IO uint32_t ANA_CFG0        ;   /*!< Offset: 0x018 (R/W)  Analog Configure Register 0 */
    __IO uint32_t ANA_CFG1        ;   /*!< Offset: 0x01C (R/W)  Analog Configure Register 1 */
    __IO uint32_t PWR0            ;   /*!< Offset: 0x020 (R/W)  Always-on Power Register 0 */
    __IO uint32_t PWR1            ;   /*!< Offset: 0x024 (R/W)  Always-on Power Register 1 */
    __IO uint32_t RES2[6]         ;   /*!< Offset: 0x028--3C (R/W)   */
    __IO uint32_t LRC_EN          ;   /*!< Offset: 0x040 (R/W)  WDT and RTC Switch Pattern Configure Register */
} AON_SYSC_T;

//RWRP
#define AON_RWRP_STA_Pos                 (0)
#define AON_RWRP_STA_Msk                 (0x1ul << AON_RWRP_STA_Pos)
#define AON_RWRP_Pos                     (0)
#define AON_RWRP_Msk                     (0xFFul << AON_RWRP_Pos)

//STA
#define WDT_RST_STA_Pos                  (2)
#define WDT_RST_STA_Msk                  (0x1ul << WDT_RST_STA_Pos)
#define RST_N_STA_Pos                    (1)
#define RST_N_STA_Msk                    (0x1ul << RST_N_STA_Pos)
#define POR_LV_STA_Pos                   (0)
#define POR_LV_STA_Msk                   (0x1ul << POR_LV_STA_Pos)

//IOLATCH
#define GPIO_LATCH_Pos                   (0)
#define GPIO_LATCH_Msk                   (0x1ul << GPIO_LATCH_Pos)
#define GPIO_LATCH_SWD_Pos               (1)
#define GPIO_LATCH_SWD_Msk               (0x1ul << GPIO_LATCH_SWD_Pos)

//SWD_PEN
#define SWD_PEN_Pos                      (0)
#define SWD_PEN_Msk                      (0x1ul << SWD_PEN_Pos)

//ANA_CFG0
#define ANA_CFG0_MDIV_CFG_Pos            (0)
#define ANA_CFG0_MDIV_CFG_Msk            (0xFul << ANA_CFG0_MDIV_CFG_Pos)
#define ANA_CFG0_VCO_KVCO_TUNE_Pos       (4)
#define ANA_CFG0_VCO_KVCO_TUNE_Msk       (0x3ul << ANA_CFG0_VCO_KVCO_TUNE_Pos)
#define ANA_CFG0_VCO_BAND_CFG_Pos        (6)
#define ANA_CFG0_VCO_BAND_CFG_Msk        (0x1ul << ANA_CFG0_VCO_BAND_CFG_Pos)
#define ANA_CFG0_PLL_FAST_EN_Pos         (7)
#define ANA_CFG0_PLL_FAST_EN_Msk         (0x1ul << ANA_CFG0_PLL_FAST_EN_Pos)
#define ANA_CFG0_CORE_TEST_CFG_Pos       (8)
#define ANA_CFG0_CORE_TEST_CFG_Msk       (0x3ul << ANA_CFG0_CORE_TEST_CFG_Pos)
#define ANA_CFG0_PREDIV_Pos              (10)
#define ANA_CFG0_PREDIV_Msk              (0x1ul << ANA_CFG0_PREDIV_Pos)
#define ANA_CFG0_EN_PLL_CORE_TEST_Pos    (11)
#define ANA_CFG0_EN_PLL_CORE_TEST_Msk    (0x1ul << ANA_CFG0_EN_PLL_CORE_TEST_Pos)
#define ANA_CFG0_ICP_CFG_Pos             (12)
#define ANA_CFG0_ICP_CFG_Msk             (0x7ul << ANA_CFG0_ICP_CFG_Pos)
#define ANA_CFG0_PLL_LOCK_Pos            (15)
#define ANA_CFG0_PLL_LOCK_Msk            (0x1ul << ANA_CFG0_PLL_LOCK_Pos)
#define ANA_CFG0_PLL_TEST_CFG_Pos        (16)
#define ANA_CFG0_PLL_TEST_CFG_Msk        (0x3ul << ANA_CFG0_PLL_TEST_CFG_Pos)
#define ANA_CFG0_PLL_11V_CFG_Pos         (18)
#define ANA_CFG0_PLL_11V_CFG_Msk         (0x3ul << ANA_CFG0_PLL_11V_CFG_Pos)
#define ANA_CFG0_PLL_15V_CFG_Pos         (20)
#define ANA_CFG0_PLL_15V_CFG_Msk         (0x3ul << ANA_CFG0_PLL_15V_CFG_Pos)
#define ANA_CFG0_EN_PLL_PWR_TEST_Pos     (22)
#define ANA_CFG0_EN_PLL_PWR_TEST_Msk     (0x1ul << ANA_CFG0_EN_PLL_PWR_TEST_Pos)
#define ANA_CFG0_HRC_DIV2_Pos            (23)
#define ANA_CFG0_HRC_DIV2_Msk            (0x1ul << ANA_CFG0_HRC_DIV2_Pos)
#define ANA_CFG0_HRC_IBIAS_CFG_Pos       (24)
#define ANA_CFG0_HRC_IBIAS_CFG_Msk       (0x7ul << ANA_CFG0_HRC_IBIAS_CFG_Pos)
#define ANA_CFG0_HRC_CAP_Pos             (27)
#define ANA_CFG0_HRC_CAP_Msk             (0x1ul << ANA_CFG0_HRC_CAP_Pos)
#define ANA_CFG0_XTAL40M_CFG_Pos         (28)
#define ANA_CFG0_XTAL40M_CFG_Msk         (0x3ul << ANA_CFG0_XTAL40M_CFG_Pos)
#define ANA_CFG0_XTAL32K_CFG_Pos         (30)
#define ANA_CFG0_XTAL32K_CFG_Msk         (0x3ul << ANA_CFG0_XTAL32K_CFG_Pos)

//ANA_CFG1
#define ANA_CFG1_LDO_LP_OUTPUT_Pos       (0)
#define ANA_CFG1_LDO_LP_OUTPUT_Msk       (0x3ul << ANA_CFG1_LDO_LP_OUTPUT_Pos)
#define ANA_CFG1_LDO_CORE_OUTPUT_Pos     (2)
#define ANA_CFG1_LDO_CORE_OUTPUT_Msk     (0x3ul << ANA_CFG1_LDO_CORE_OUTPUT_Pos)
#define ANA_CFG1_BOR_THRESHOLD_Pos       (4)
#define ANA_CFG1_BOR_THRESHOLD_Msk       (0x7ul << ANA_CFG1_BOR_THRESHOLD_Pos)
#define ANA_CFG1_PLL_POST_DIV_Pos        (16)
#define ANA_CFG1_PLL_POST_DIV_Msk        (0x1ul << ANA_CFG1_PLL_POST_DIV_Pos)
#define ANA_CFG1_ADC_CLK_REV_Pos         (17)
#define ANA_CFG1_ADC_CLK_REV_Msk         (0x1ul << ANA_CFG1_ADC_CLK_REV_Pos)
#define ANA_CFG1_FADC_GRP0_LDO_Pos       (18)
#define ANA_CFG1_FADC_GRP0_LDO_Msk       (0x1ul << ANA_CFG1_FADC_GRP0_LDO_Pos)
#define ANA_CFG1_FADC_GRP1_LDO_Pos       (19)
#define ANA_CFG1_FADC_GRP1_LDO_Msk       (0x1ul << ANA_CFG1_FADC_GRP1_LDO_Pos)
#define ANA_CFG1_FADC_ALL_LDO_Pos        (18)
#define ANA_CFG1_FADC_ALL_LDO_Msk        (0x3ul << ANA_CFG1_FADC_ALL_LDO_Pos)
#define ANA_CFG1_SEL_R_SYNC_GEN_Pos      (21)
#define ANA_CFG1_SEL_R_SYNC_GEN_Msk      (0x1ul << ANA_CFG1_SEL_R_SYNC_GEN_Pos)
#define ANA_CFG1_SEL_PAD_GEN_SYNC_Pos    (22)
#define ANA_CFG1_SEL_PAD_GEN_SYNC_Msk    (0x1ul << ANA_CFG1_SEL_PAD_GEN_SYNC_Pos)
#define ANA_CFG1_R_GEN_SYNC_Pos          (23)
#define ANA_CFG1_R_GEN_SYNC_Msk          (0x1ul << ANA_CFG1_R_GEN_SYNC_Pos)

//LRC_EN
#define PD_LRC_PATTERN_Pos               (0)
#define PD_LRC_PATTERN_Msk               (0xFFFFFFFFul << PU_LRC_PATTERN_Pos)
#define PD_LRC_ALL_PATTERN               (0xECEB1314)
#define PD_LRC_WDT_PATTERN               (0x9A9C6563)
#define PD_LRC_RTC_PATTERN               (0xA9C95636)

//AON_PWR0
#define AON_DEEP_MODE_Pos                (0)
#define AON_DEEP_MODE_Msk                (0x1ul << AON_DEEP_MODE_Pos)
#define AON_PU_SRAM_LP_PWR_Pos           (1)
#define AON_PU_SRAM_LP_PWR_Msk           (0x1ul << AON_PU_SRAM_LP_PWR_Pos)
// #define AON_PU_PERI_PWR_Pos              (2)
// #define AON_PU_PERI_PWR_Msk              (0x1ul << AON_PU_PERI_PWR_Pos)
// #define AON_PU_FLASH_PWR_Pos             (3)
// #define AON_PU_FLASH_PWR_Msk             (0x1ul << AON_PU_FLASH_PWR_Pos)
#define AON_PU_SRAM3_PWR_Pos             (4)
#define AON_PU_SRAM3_PWR_Msk             (0x1ul << AON_PU_SRAM3_PWR_Pos)
#define AON_PU_SRAM2_PWR_Pos             (5)
#define AON_PU_SRAM2_PWR_Msk             (0x1ul << AON_PU_SRAM2_PWR_Pos)
#define AON_PU_SRAM1_PWR_Pos             (6)
#define AON_PU_SRAM1_PWR_Msk             (0x1ul << AON_PU_SRAM1_PWR_Pos)

//AON_PWR1
#define AON_PWR1_PU_XTAL_32K_Pos         (0)
#define AON_PWR1_PU_XTAL_32K_Msk         (0x1ul << AON_PWR1_PU_XTAL_32K_Pos)
#define AON_PWR1_PU_XTAL_40M_Pos         (1)
#define AON_PWR1_PU_XTAL_40M_Msk         (0x1ul << AON_PWR1_PU_XTAL_40M_Pos)
#define AON_PWR1_PU_PLL_Pos              (2)
#define AON_PWR1_PU_PLL_Msk              (0x1ul << AON_PWR1_PU_PLL_Pos)
#define AON_PWR1_PU_LRC_STA_Pos          (3)
#define AON_PWR1_PU_LRC_STA_Msk          (0x1ul << AON_PWR1_PU_LRC_STA_Pos)
/*---------------------- END ---------------------------------*/


/*---------------------- SYSC ---------------------------------*/
/** 0x50000000 */
typedef struct {
    __IO uint32_t CLK_EN;           /*!< Offset: 0x000 (R/W)  System Clock Enable Register */
    __IO uint32_t CLK_DIV;          /*!< Offset: 0x004 (R/W)  System Clock Divider Register */
    __IO uint32_t RST;              /*!< Offset: 0x008 (R/W)  System Software Reset Register */
    __IO uint32_t STA;              /*!< Offset: 0x00c (R/W)  System Clock Status Register */
    __IO uint32_t Res[28];
    __IO uint32_t CHIP_ID;          /*!< Offset: 0x080 (R) 0x88000000 Chip ID Register */
} SYSC_T;
/*---------------------- END ---------------------------------*/

//CLK_EN
#define DMA_EN_Pos                            (29)
#define SRAM1_EN_Pos                          (28)
#define SRAM2_EN_Pos                          (27)
#define SRAM3_EN_Pos                          (26)
#define GPIO_EN_Pos                           (25)
#define QSPI0_EN_Pos                          (24)
#define QSPI1_EN_Pos                          (23)
#define I2C_EN_Pos                            (22)
#define RADAR_BB_EN_Pos                       (21)
#define CAN0_EN_Pos                           (20)
#define CAN1_EN_Pos                           (19)
#define TRNG_EN_Pos                           (18)
#define WWDG_EN_Pos                           (17)
#define ALWAYS_ON_EN_Pos                      (16)
#define FADC_EN_Pos                           (15)
#define SADC_EN_Pos                           (14)
#define OSPI0_EN_Pos                          (13)
#define OSPI1_EN_Pos                          (12)
#define USCI7_EN_Pos                          (11)
#define USCI6_EN_Pos                          (10)
#define USCI5_EN_Pos                          (9)
#define USCI4_EN_Pos                          (8)
#define USCI3_EN_Pos                          (7)
#define USCI2_EN_Pos                          (6)
#define USCI1_EN_Pos                          (5)
#define USCI0_EN_Pos                          (4)
#define TMRA_EN_Pos                           (3)
#define TMRB_EN_CLK_Pos                       (2)
#define CRC_EN_Pos                            (1)
#define PMUX_EN_Pos                           (0)

#define DMA_EN_Msk                            (0x1ul << DMA_EN_Pos)
#define SRAM1_EN_Msk                          (0x1ul << SRAM1_EN_Pos)
#define SRAM2_EN_Msk                          (0x1ul << SRAM2_EN_Pos)
#define SRAM3_EN_Msk                          (0x1ul << SRAM3_EN_Pos)
#define GPIO_EN_Msk                           (0x1ul << GPIO_EN_Pos)
#define QSPI0_EN_Msk                          (0x1ul << QSPI0_EN_Pos)
#define QSPI1_EN_Msk                          (0x1ul << QSPI1_EN_Pos)
#define I2C_EN_Msk                            (0x1ul << I2C_EN_Pos)
#define RADAR_BB_EN_Msk                       (0x1ul << RADAR_BB_EN_Pos)
#define CAN0_EN_Msk                           (0x1ul << CAN0_EN_Pos)
#define CAN1_EN_Msk                           (0x1ul << CAN1_EN_Pos)
#define TRNG_EN_Msk                           (0x1ul << TRNG_EN_Pos)
#define WWDG_EN_Msk                           (0x1ul << WWDG_EN_Pos)
#define ALWAYS_ON_EN_Msk                      (0x1ul << ALWAYS_ON_EN_Pos)
#define FADC_EN_Msk                           (0x1ul << FADC_EN_Pos)
#define SADC_EN_Msk                           (0x1ul << SADC_EN_Pos)
#define OSPI0_EN_Msk                          (0x1ul << OSPI0_EN_Pos)
#define OSPI1_EN_Msk                          (0x1ul << OSPI1_EN_Pos)
#define USCI7_EN_Msk                          (0x1ul << USCI7_EN_Pos)
#define USCI6_EN_Msk                          (0x1ul << USCI6_EN_Pos)
#define USCI5_EN_Msk                          (0x1ul << USCI5_EN_Pos)
#define USCI4_EN_Msk                          (0x1ul << USCI4_EN_Pos)
#define USCI3_EN_Msk                          (0x1ul << USCI3_EN_Pos)
#define USCI2_EN_Msk                          (0x1ul << USCI2_EN_Pos)
#define USCI1_EN_Msk                          (0x1ul << USCI1_EN_Pos)
#define USCI0_EN_Msk                          (0x1ul << USCI0_EN_Pos)
#define TMRA_EN_Msk                           (0x1ul << TMRA_EN_Pos)
#define TMRB_EN_CLK_Msk                       (0x1ul << TMRB_EN_CLK_Pos)
#define CRC_EN_Msk                            (0x1ul << CRC_EN_Pos)
#define PMUX_EN_Msk                           (0x1ul << PMUX_EN_Pos)

//CLK_DIV
#define CAN_CLK_DIV_Pos                       (24)
#define FLASH_WS_Pos                          (16)
#define AHB_CLK_DIV_Pos                       (8)
#define APB_CLK_DIV_Pos                       (0)

#define CAN_CLK_DIV_Msk                       (0xFul << CAN_CLK_DIV_Pos)
#define FLASH_WS_Msk                          (0xFul << FLASH_WS_Pos  )
#define AHB_CLK_DIV_Msk                       (0xFFul << AHB_CLK_DIV_Pos)
#define APB_CLK_DIV_Msk                       (0xFFul << APB_CLK_DIV_Pos)

//RST
#define GPIO_RST_Pos                          (23)
#define I2C_RST_Pos                           (22)
#define RADAR_BB_RST_Pos                      (21)
#define CAN0_RST_Pos                          (20)
#define CAN1_RST_Pos                          (19)
#define FADC_RST_Pos                          (15)
#define SADC_RST_Pos                          (14)
#define FLASH_RST_Pos                         (12)
#define USCI7_RST_Pos                         (11)
#define USCI6_RST_Pos                         (10)
#define USCI5_RST_Pos                         (9)
#define USCI4_RST_Pos                         (8)
#define USCI3_RST_Pos                         (7)
#define USCI2_RST_Pos                         (6)
#define USCI1_RST_Pos                         (5)
#define USCI0_RST_Pos                         (4)
#define TMRA_RST_Pos                          (3)
#define TMRB_RST_Pos                          (2)
#define CRC_RST_Pos                           (1)
#define PMUX_RST_Pos                          (0)

#define GPIO_RST_Msk                          (0x1ul << GPIO_RST_Pos)
#define I2C_RST_Msk                           (0x1ul << I2C_RST_Pos)
#define RADAR_BB_RST_Msk                      (0x1ul << RADAR_BB_RST_Pos)
#define CAN0_RST_Msk                          (0x1ul << CAN0_RST_Pos)
#define CAN1_RST_Msk                          (0x1ul << CAN1_RST_Pos)
#define FADC_RST_Msk                          (0x1ul << FADC_RST_Pos)
#define SADC_RST_Msk                          (0x1ul << SADC_RST_Pos)
#define FLASH_RST_Msk                         (0x1ul << FLASH_RST_Pos)
#define USCI7_RST_Msk                         (0x1ul << USCI7_RST_Pos)
#define USCI6_RST_Msk                         (0x1ul << USCI6_RST_Pos)
#define USCI5_RST_Msk                         (0x1ul << USCI5_RST_Pos)
#define USCI4_RST_Msk                         (0x1ul << USCI4_RST_Pos)
#define USCI3_RST_Msk                         (0x1ul << USCI3_RST_Pos)
#define USCI2_RST_Msk                         (0x1ul << USCI2_RST_Pos)
#define USCI1_RST_Msk                         (0x1ul << USCI1_RST_Pos)
#define USCI0_RST_Msk                         (0x1ul << USCI0_RST_Pos)
#define TMRA_RST_Msk                          (0x1ul << TMRA_RST_Pos)
#define TMRB_RST_Msk                          (0x1ul << TMRB_RST_Pos)
#define CRC_RST_Msk                           (0x1ul << CRC_RST_Pos)
#define PMUX_RST_Msk                          (0x1ul << PMUX_RST_Pos)

//STA
#define PLL_RDY_Pos                           (0)
#define PLL_RDY_Msk                           (0x1ul << PLL_RDY_Pos)

/*---------------------- USCI ---------------------------------*/
/** 0x40000000 USCI0 */
/** 0x40004000 USCI1 */
/** 0x40008000 USCI2 */
/** 0x4000C000 USCI3 */
/** 0x40010000 USCI4 */
/** 0x40014000 USCI5 */
/** 0x40018000 USCI6 */
/** 0x4001C000 USCI7 */
typedef struct {
    __IO uint32_t STA;              /*!< Offset: 0x000 (R/W)  USCIx Status Register */
    __IO uint32_t TRX;              /*!< Offset: 0x004 (R/W)  USCIx TX/RX Buffer Register */
    __IO uint32_t CMD;              /*!< Offset: 0x008 (R/W)  USCIx Command Register */
    __IO uint32_t IMR;              /*!< Offset: 0x00C (R/W)  USCIx Interrup Mask Register */
    __IO uint32_t EMR;              /*!< Offset: 0x010 (R/W)  USCIx Error Interrup Mask Register */
    __IO uint32_t ISR;              /*!< Offset: 0x014 (R/W)  USCIx Interrupt Status Register */
    __IO uint32_t ESR;              /*!< Offset: 0x018 (R/W)  USCIx Error Status Register */
    __IO uint32_t CFG0;             /*!< Offset: 0x01C (R/W)  USCIx Configure Register0 */
    __IO uint32_t CFG1;             /*!< Offset: 0x020 (R/W)  USCIx Configure Register1 */
    __IO uint32_t BAUD;             /*!< Offset: 0x024 (R/W)  USCIx Baud Rate Configure Register */
    __IO uint32_t MOD;              /*!< Offset: 0x028 (R/W)  USCIx Baud Rate Mode Register */
    __IO uint32_t CLK_DIV;          /*!< Offset: 0x02C (R/W)  USCIx Clock Div Register */
} USCI_T;

//STA
#define USCI_TX_RUN_Pos                (0)
#define USCI_TX_RUN_Msk                (0x01ul << USCI_TX_RUN_Pos)
#define USCI_RX_RUN_Pos                (1)
#define USCI_RX_RUN_Msk                (0x01ul << USCI_RX_RUN_Pos)

//IMR
#define USCI_IM_TXE_Pos                (0)
#define USCI_IM_TXE_Msk                (0x01ul << USCI_IM_TXE_Pos)
#define USCI_IM_RXNE_Pos               (1)
#define USCI_IM_RXNE_Msk               (0x01ul << USCI_IM_RXNE_Pos)
#define USCI_IM_TE_Pos                 (2)
#define USCI_IM_TE_Msk                 (0x01ul << USCI_IM_TE_Pos)

//EMR
#define USCI_EM_ROVR_Pos               (0)
#define USCI_EM_ROVR_Msk               (0x01ul << USCI_EM_ROVR_Pos)
#define USCI_EM_RPAR_Pos               (1)
#define USCI_EM_RPAR_Msk               (0x01ul << USCI_EM_RPAR_Pos)

//ISR
#define USCI_IS_TXE_Pos                (0)
#define USCI_IS_TXE_Msk                (0x01ul << USCI_IS_TXE_Pos)
#define USCI_IS_RXNE_Pos               (1)
#define USCI_IS_RXNE_Msk               (0x01ul << USCI_IS_RXNE_Pos)
#define USCI_IS_TE_Pos                 (2)
#define USCI_IS_TE_Msk                 (0x01ul << USCI_IS_TE_Pos)
#define USCI_IS_ERR_Pos                (4)
#define USCI_IS_ERR_Msk                (0x1ul << USCI_IS_ERR_Pos)

//ESR
#define USCI_ES_ROVR_Pos               (0)
#define USCI_ES_ROVR_Msk               (0x01ul << USCI_EM_ROVR_Pos)
#define USCI_ES_RPAR_Pos               (1)
#define USCI_ES_RPAR_Msk               (0x01ul << USCI_EM_RPAR_Pos)

//CFG0
#define USCI_CFG0_WK_MODE_Pos          (0)
#define USCI_CFG0_WK_MODE_Msk          (0x03ul << USCI_CFG0_WK_MODE_Pos)
#define USCI_CFG0_DMA_TX_EN_Pos        (4)
#define USCI_CFG0_DMA_TX_EN_Msk        (0x01ul << USCI_CFG0_DMA_TX_EN_Pos)
#define USCI_CFG0_DMA_RX_EN_Pos        (5)
#define USCI_CFG0_DMA_RX_EN_Msk        (0x01ul << USCI_CFG0_DMA_RX_EN_Pos)

//CFG1
#define USCI_CFG1_TX_EN_Pos            (0)
#define USCI_CFG1_TX_EN_Msk            (0x01ul << USCI_CFG1_TX_EN_Pos)
#define USCI_CFG1_RX_EN_Pos            (1)
#define USCI_CFG1_RX_EN_Msk            (0x01ul << USCI_CFG1_RX_EN_Pos)
#define USCI_CFG1_HALF_DUPLEX_Pos      (2)
#define USCI_CFG1_HALF_DUPLEX_Msk      (0x01ul << USCI_CFG1_HALF_DUPLEX_Pos)
#define USCI_CFG1_DATA_LEN_Pos         (4)
#define USCI_CFG1_DATA_LEN_Msk         (0x03ul << USCI_CFG1_DATA_LEN_Pos)
#define USCI_CFG1_LSB_Pos              (6)
#define USCI_CFG1_LSB_Msk              (0x01ul << USCI_CFG1_LSB_Pos)
#define USCI_CFG1_PAR_TYP_Pos          (8)
#define USCI_CFG1_PAR_TYP_Msk          (0x01ul << USCI_CFG1_PAR_TYP_Pos)
#define USCI_CFG1_PAR_EN_Pos           (9)
#define USCI_CFG1_PAR_EN_Msk           (0x01ul << USCI_CFG1_PAR_EN_Pos)
#define USCI_CFG1_STOP_LEN_Pos         (10)
#define USCI_CFG1_STOP_LEN_Msk         (0x01ul << USCI_CFG1_STOP_LEN_Pos)
#define USCI_CFG1_BIT_INV_Pos          (12)
#define USCI_CFG1_BIT_INV_Msk          (0x01ul << USCI_CFG1_BIT_INV_Pos)
#define USCI_CFG1_TRX_NEG_Pos          (16)
#define USCI_CFG1_TRX_NEG_Msk          (0x01ul << USCI_CFG1_TRX_NEG_Pos)
#define USCI_CFG1_CLKP_Pos             (17)
#define USCI_CFG1_CLKP_Msk             (0x01ul << USCI_CFG1_CLKP_Pos)
#define USCI_CFG1_SLV_SEL_Pos          (18)
#define USCI_CFG1_SLV_SEL_Msk          (0x01ul << USCI_CFG1_SLV_SEL_Pos)
#define USCI_CFG1_CS_LVL_Pos           (19)
#define USCI_CFG1_CS_LVL_Msk           (0x01ul << USCI_CFG1_CS_LVL_Pos)
#define USCI_CFG1_CS_SEL_Pos           (20)
#define USCI_CFG1_CS_SEL_Msk           (0x1Ful << USCI_CFG1_CS_SEL_Pos)

//BAUD
#define USCI_BAUD_DIV_Pos              (0)
#define USCI_BAUD_DIV_Msk              (0xFFFFul << USCI_BAUD_DIV_Pos)

//MOD
#define USCI_BAUD_S16_Pos              (0)
#define USCI_BAUD_S16_Msk              (0x01ul << USCI_BAUD_S16_Pos)
#define USCI_BAUD_SDIV_Pos             (1)
#define USCI_BAUD_SDIV_Msk             (0x07ul << USCI_BAUD_SDIV_Pos)
#define USCI_BAUD_FDIV_Pos             (4)
#define USCI_BAUD_FDIV_Msk             (0x0Ful << USCI_BAUD_FDIV_Pos)

//CLK_DIV
#define USCI_CLK_DIV_Pos               (0)
#define USCI_CLK_DIV_Msk               (0x0Ful << USCI_CLK_DIV_Pos)

/*---------------------- END ---------------------------------*/


/*---------------------- DMA ---------------------------------*/
//DMA_CH

typedef struct
{
    __IO uint32_t SAR;            /*!< Offset: 0x000 (R/W) Source Address Register for Channel x[31:0] */
    __IO uint32_t HSAR;           /*!< Offset: 0x004 (R/W) Source Address Register for Channel x[63:32] */
    __IO uint32_t DAR;            /*!< Offset: 0x008 (R/W) Destination Address Register for Channel x[31:0] */
    __IO uint32_t HDAR;           /*!< Offset: 0x00c (R/W) Destination Address Register for Channel x[63:32] */
    __IO uint32_t LLP;            /*!< Offset: 0x010 (R/W) Linked List Pointer Register for Channel x[31:0] */
    __IO uint32_t HLLP;           /*!< Offset: 0x014 (R/W) Linked List Pointer Register for Channel x[63:32] */
    __IO uint32_t CTL;            /*!< Offset: 0x018 (R/W) Control Register for Channel x[31:0] */
    __IO uint32_t HCTL;           /*!< Offset: 0x01c (R/W) Control Register for Channel x[63:32] */
    __IO uint32_t SSTAT;          /*!< Offset: 0x020 (R/W) Source Status Register for Channel x[31:0] */
    __IO uint32_t HSSTAT;         /*!< Offset: 0x024 (R/W) Source Status Register for Channel x[63:32] */
    __IO uint32_t DSTAT;          /*!< Offset: 0x028 (R/W) Destination Status Register for Channel x[31:0] */
    __IO uint32_t HDSTAT;         /*!< Offset: 0x02c (R/W) Destination Status Register for Channel x[63:32] */
    __IO uint32_t SSTATAR;        /*!< Offset: 0x030 (R/W) Source Status Address Register for Channel x[31:0] */
    __IO uint32_t HSSTATAR;       /*!< Offset: 0x034 (R/W) Source Status Address Register for Channel x[63:32] */
    __IO uint32_t DSTATAR;        /*!< Offset: 0x038 (R/W) Destination Status Address Register for Channel x[31:0] */
    __IO uint32_t HDSTATAR;       /*!< Offset: 0x03c (R/W) Destination Status Address Register for Channel x[63:32] */
    __IO uint32_t CFG;            /*!< Offset: 0x040 (R/W) Configuration Register for Channel x[31:0] */
    __IO uint32_t HCFG;           /*!< Offset: 0x044 (R/W) Configuration Register for Channel x[63:32] */
    __IO uint32_t SGR;            /*!< Offset: 0x048 (R/W) Source Gather Register for Channel x[31:0] */
    __IO uint32_t HSGR;           /*!< Offset: 0x04c (R/W) Source Gather Register for Channel x[63:32] */
    __IO uint32_t DSR;            /*!< Offset: 0x050 (R/W) Destination Scatter Register for Channel x[31:0] */
    __IO uint32_t HDSR;           /*!< Offset: 0x054 (R/W) Destination Scatter Register for Channel x[63:32] */
} DMA_CH_T;

//CTL register
#define DMA_CTL_INT_EN_Pos                (0)
#define DMA_CTL_INT_EN_Msk                (0x01ul << DMA_CTL_INT_EN_Pos)
#define DMA_CTL_DST_TR_WIDTH_Pos          (1)
#define DMA_CTL_DST_TR_WIDTH_Msk          (0x07ul << DMA_CTL_DST_TR_WIDTH_Pos)
#define DMA_CTL_SRC_TR_WIDTH_Pos          (4)
#define DMA_CTL_SRC_TR_WIDTH_Msk          (0x07ul << DMA_CTL_SRC_TR_WIDTH_Pos)
#define DMA_CTL_DINC_Pos                  (7)
#define DMA_CTL_DINC_Msk                  (0x03ul << DMA_CTL_DINC_Pos)
#define DMA_CTL_SINC_Pos                  (9)
#define DMA_CTL_SINC_Msk                  (0x03ul << DMA_CTL_SINC_Pos)
#define DMA_CTL_DEST_MSIZE_Pos            (11)
#define DMA_CTL_DEST_MSIZE_Msk            (0x07ul << DMA_CTL_DEST_MSIZE_Pos)
#define DMA_CTL_SRC_MSIZE_Pos             (14)
#define DMA_CTL_SRC_MSIZE_Msk             (0x07ul << DMA_CTL_SRC_MSIZE_Pos)
#define DMA_CTL_SRC_GATHER_EN_Pos         (17)
#define DMA_CTL_SRC_GATHER_EN_Msk         (0x01ul << DMA_CTL_SRC_GATHER_EN_Pos)
#define DMA_CTL_DST_GATHER_EN_Pos         (18)
#define DMA_CTL_DST_GATHER_EN_Msk         (0x01ul << DMA_CTL_DST_GATHER_EN_Pos)
#define DMA_CTL_TT_FC_Pos                 (20)
#define DMA_CTL_TT_FC_Msk                 (0x07ul << DMA_CTL_TT_FC_Pos)
#define DMA_CTL_DMS_Pos                   (23)
#define DMA_CTL_DMS_Msk                   (0x03ul << DMA_CTL_DMS_Pos)
#define DMA_CTL_SMS_Pos                   (25)
#define DMA_CTL_SMS_Msk                   (0x03ul << DMA_CTL_SMS_Pos)
#define DMA_CTL_LLP_DST_EN_Pos            (27)
#define DMA_CTL_LLP_DST_EN_Msk            (0x01ul << DMA_CTL_LLP_DST_EN_Pos)
#define DMA_CTL_LLP_SRC_EN_Pos            (28)
#define DMA_CTL_LLP_SRC_EN_Msk            (0x01ul << DMA_CTL_LLP_SRC_EN_Pos)

//HCTL register
#define DMA_HCTL_BLOCK_TS_Pos             (0)
#define DMA_HCTL_BLOCK_TS_Msk             (0x0FFFul << DMA_HCTL_BLOCK_TS_Pos)
#define DMA_HCTL_DONE_Pos                 (12)
#define DMA_HCTL_DONE_Msk                 (0x01ul << DMA_HCTL_DONE_Pos)

//CFG register
#define DMA_CFG_CH_PRIOR_Pos              (5)
#define DMA_CFG_CH_PRIOR_Msk              (0x07ul << DMA_CFG_CH_PRIOR_Pos)
#define DMA_CFG_CH_SUSP_Pos               (8)
#define DMA_CFG_CH_SUSP_Msk               (0x01ul << DMA_CFG_CH_SUSP_Pos)
#define DMA_CFG_FIFO_EMPTY_Pos            (9)
#define DMA_CFG_FIFO_EMPTY_Msk            (0x01ul << DMA_CFG_FIFO_EMPTY_Pos)
#define DMA_CFG_HS_SEL_DST_Pos            (10)
#define DMA_CFG_HS_SEL_DST_Msk            (0x01ul << DMA_CFG_HS_SEL_DST_Pos)
#define DMA_CFG_HS_SEL_SRC_Pos            (11)
#define DMA_CFG_HS_SEL_SRC_Msk            (0x01ul << DMA_CFG_HS_SEL_SRC_Pos)
#define DMA_CFG_LOCK_CH_L_Pos             (12)
#define DMA_CFG_LOCK_CH_L_Msk             (0x03ul << DMA_CFG_LOCK_CH_L_Pos)
#define DMA_CFG_LOCK_B_L_Pos              (14)
#define DMA_CFG_LOCK_B_L_Msk              (0x03ul << DMA_CFG_LOCK_B_L_Pos)
#define DMA_CFG_LOCK_CH_Pos               (16)
#define DMA_CFG_LOCK_CH_Msk               (0x01ul << DMA_CFG_LOCK_CH_Pos)
#define DMA_CFG_LOCK_B_Pos                (17)
#define DMA_CFG_LOCK_B_Msk                (0x01ul << DMA_CFG_LOCK_B_Pos)
#define DMA_CFG_DST_HS_POL_Pos            (18)
#define DMA_CFG_DST_HS_POL_Msk            (0x01ul << DMA_CFG_DST_HS_POL_Pos)
#define DMA_CFG_SRC_HS_POL_Pos            (19)
#define DMA_CFG_SRC_HS_POL_Msk            (0x01ul << DMA_CFG_SRC_HS_POL_Pos)
#define DMA_CFG_MAX_ABRST_Pos             (20)
#define DMA_CFG_MAX_ABRST_Msk             (0x03FFul << DMA_CFG_MAX_ABRST_Pos)
#define DMA_CFG_RELOAD_SRC_Pos            (30)
#define DMA_CFG_RELOAD_SRC_Msk            (0x01ul << DMA_CFG_RELOAD_SRC_Pos)
#define DMA_CFG_RELOAD_DST_Pos            (31)
#define DMA_CFG_RELOAD_DST_Msk            (0x01ul << DMA_CFG_RELOAD_DST_Pos)

//HCFG
#define DMA_HCFG_FCMODE_Pos               (0)
#define DMA_HCFG_FCMODE_Msk               (0x01ul << DMA_HCFG_FCMODE_Pos)
#define DMA_HCFG_FIFO_MODE_Pos            (1)
#define DMA_HCFG_FIFO_MODE_Msk            (0x01ul << DMA_HCFG_FIFO_MODE_Pos)
#define DMA_HCFG_PROTCTL_Pos              (2)
#define DMA_HCFG_PROTCTL_Msk              (0x07ul << DMA_HCFG_PROTCTL_Pos)
#define DMA_HCFG_DS_UPD_EN_Pos            (5)
#define DMA_HCFG_DS_UPD_EN_Msk            (0x01ul << DMA_HCFG_DS_UPD_EN_Pos)
#define DMA_HCFG_SS_UPD_EN_Pos            (6)
#define DMA_HCFG_SS_UPD_EN_Msk            (0x01ul << DMA_HCFG_SS_UPD_EN_Pos)
#define DMA_HCFG_SRC_PER_Pos              (7)
#define DMA_HCFG_SRC_PER_Msk              (0x0Ful << DMA_HCFG_SRC_PER_Pos)
#define DMA_HCFG_DEST_PER_Pos             (11)
#define DMA_HCFG_DEST_PER_Msk             (0x0Ful << DMA_HCFG_DEST_PER_Pos)

//DMACFG_EN
#define DMA_DMACFG_EN_Pos                 (0)
#define DMA_DMACFG_EN_Msk                 (0x01ul << DMA_DMACFG_EN_Pos)

//CHEN
#define DMA_CHEN_CH_EN_Pos                (0)
#define DMA_CHEN_CH_EN_Msk                (0xFFul << DMA_CHEN_CH_EN_Pos)
#define DMA_CHEN_CH_EN_WE_Pos             (8)
#define DMA_CHEN_CH_EN_WE_Msk             (0xFFul << DMA_CHEN_CH_EN_WE_Pos)

typedef struct
{
    __IO uint32_t RawTfr;
    __IO uint32_t HRawTfr;
    __IO uint32_t RawBlock;
    __IO uint32_t HRawBlock;
    __IO uint32_t RawSrcTran;
    __IO uint32_t HRawSrcTran;
    __IO uint32_t RawDstTran;
    __IO uint32_t HRawDstTran;
    __IO uint32_t RawErr;
    __IO uint32_t HRawErr;
    __IO uint32_t StatusTfr;
    __IO uint32_t HStatusTfr;
    __IO uint32_t StatusBlock;
    __IO uint32_t HStatusBlock;
    __IO uint32_t StatusSrcTran;
    __IO uint32_t HStatusSrcTran;
    __IO uint32_t StatusDstTran;
    __IO uint32_t HStatusDstTran;
    __IO uint32_t StatusErr;
    __IO uint32_t HStatusErr;
    __IO uint32_t MaskTfr;
    __IO uint32_t HMaskTfr;
    __IO uint32_t MaskBlock;
    __IO uint32_t HMaskBlock;
    __IO uint32_t MaskSrcTran;
    __IO uint32_t HMaskSrcTran;
    __IO uint32_t MaskDstTran;
    __IO uint32_t HMaskDstTran;
    __IO uint32_t MaskErr;
    __IO uint32_t HMaskErr;
    __IO uint32_t ClearTfr;
    __IO uint32_t HClearTfr;
    __IO uint32_t ClearBlock;
    __IO uint32_t HClearBlock;
    __IO uint32_t ClearSrcTran;
    __IO uint32_t HClearSrcTran;
    __IO uint32_t ClearDstTran;
    __IO uint32_t HClearDstTran;
    __IO uint32_t ClearErr;
    __IO uint32_t HClearErr;
    __IO uint32_t StatusInt;
    __IO uint32_t HStatusInt;
} DMA_INT_T;

typedef struct
{
    __IO uint32_t ReqSrc;       //Source Software Transaction Request Register
    __IO uint32_t HReqSrc;
    __IO uint32_t ReqDst;       //Destination Software Transaction Request Register
    __IO uint32_t HReqDst;
    __IO uint32_t SglReqSrc;     //Single Source Transaction Request Register
    __IO uint32_t HSglReqSrc;
    __IO uint32_t SglReqDst;     ////Single Destination Transaction Request Register
    __IO uint32_t HSglReqDst;
    __IO uint32_t LstSrc;        //Last Source Transaction Request Register
    __IO uint32_t HLstSrc;
    __IO uint32_t LstDst;       //Last Destination Transaction Request Register
    __IO uint32_t HLstDst;
} DMA_SWHS_T;

typedef struct
{
    __IO uint32_t DmaCfg;         //DW_ahb_dmac Configuration Register
    __IO uint32_t RES0;
    __IO uint32_t ChEn;           //DW_ahb_dmac Channel Enable Register
    __IO uint32_t RES1;
    __IO uint32_t DmaId;
    __IO uint32_t HDmaId;
    __IO uint32_t DmaTest;
    __IO uint32_t HDmaTest;
} DMA_EN_T;
/*---------------------- END ---------------------------------*/


/*---------------------- QSPI ---------------------------------*/
//
typedef struct {
    __IO uint32_t CON;                      /*!< Offset: 0x000 (R/W)  QSPI Control Register */
    __IO uint32_t ST;                       /*!< Offset: 0x004 (R/W)  QSPI Status Register */
    union
    {
        __IO uint32_t DATA;                 /*!< Offset: 0x008 (R/W)  QSPI Data Register */
        __IO uint8_t DATA8[4];              /*!< Offset: 0x008 (R/W)  QSPI Data Register */
    };
    __IO uint32_t CS;                       /*!< Offset: 0x00c (R/W)  QSPI Chip Select Register */
} QSPI_T;

//QSPI_CON
#define QSPI_CON_IE_Pos                (0)
#define QSPI_CON_IE_Msk                (0x01ul << QSPI_CON_IE_Pos)
#define QSPI_CON_XIP_SEL_Pos           (1)
#define QSPI_CON_XIP_SEL_Msk           (0x01ul << QSPI_CON_XIP_SEL_Pos)
#define QSPI_CON_CPHA_Pos              (2)
#define QSPI_CON_CPHA_Msk              (0x01ul << QSPI_CON_CPHA_Pos)
#define QSPI_CON_CPOL_Pos              (3)
#define QSPI_CON_CPOL_Msk              (0x01ul << QSPI_CON_CPOL_Pos)
#define QSPI_CON_MSB_SEL_Pos           (4)
#define QSPI_CON_MSB_SEL_Msk           (0x01ul << QSPI_CON_MSB_SEL_Pos)
#define QSPI_CON_MODE_Pos              (5)
#define QSPI_CON_MODE_Msk              (0x07ul << QSPI_CON_MODE_Pos)
#define QSPI_CON_DIV_Pos               (8)
#define QSPI_CON_DIV_Msk               (0xFFul << QSPI_CON_DIV_Pos)
#define QSPI_CON_DUMMY_LEN_Pos         (16)
#define QSPI_CON_DUMMY_LEN_Msk         (0x0Ful << QSPI_CON_DUMMY_LEN_Pos)
#define QSPI_CON_TSHSL_Pos             (20)
#define QSPI_CON_TSHSL_Msk             (0x0Ful << QSPI_CON_TSHSL_Pos)
#define QSPI_CON_READ_CMD_Pos          (24)
#define QSPI_CON_READ_CMD_Msk          (0xFFul << QSPI_CON_READ_CMD_Pos)

//ST
#define QSPI_ST_BUSY_Pos                (0)
#define QSPI_ST_BUSY_Msk                (0x01ul << QSPI_ST_BUSY_Pos)
#define QSPI_ST_IRQ_Pos                 (1)
#define QSPI_ST_IRQ_Msk                 (0x01ul << QSPI_ST_IRQ_Pos)

//CS
#define QSPI_CS_CS_Pos                  (0)
#define QSPI_CS_CS_Msk                  (0x01ul << QSPI_CS_CS_Pos)
#define QSPI_CS_CMD_ON_Pos              (4)
#define QSPI_CS_CMD_ON_Msk              (0x01ul << QSPI_CS_CMD_ON_Pos)
#define QSPI_CS_DUMMY_BYTE_Pos          (8)
#define QSPI_CS_DUMMY_BYTE_Msk          (0xFFul << QSPI_CS_DUMMY_BYTE_Pos)
/*---------------------- END ---------------------------------*/

/*---------------------- OSPI ---------------------------------*/
//
typedef struct {
    __IO uint32_t CON0;                 /*!< Offset: 0x000 (R/W)  OSPI Control Register 0 */
    __IO uint32_t CON1;                 /*!< Offset: 0x004 (R/W)  OSPI Control Register 1 */
    __IO uint32_t CS;                   /*!< Offset: 0x008 (R/W)  OSPI Chip Select Register */
    __IO uint32_t INT_STA;              /*!< Offset: 0x00c (R/W)  OSPI Interrupt Status Register */
    __IO uint32_t FIFO_STA;             /*!< Offset: 0x010 (R/W)  OSPI FIFO Status Register */
    __IO uint32_t SDH;                  /*!< Offset: 0x014 (R/W)  OSPI Slave Mode Data High Wrod Register */
    __IO uint32_t SDL;                  /*!< Offset: 0x018 (R/W)  OSPI Slave Mode Data Low Wrod Register */
} OSPI_T;

//OSPI_CON0
#define OSPI_CON0_NEND_INT_EN_Pos               (0)
#define OSPI_CON0_NEND_INT_EN_Msk               (0x01ul << OSPI_CON0_NEND_INT_EN_Pos)
#define OSPI_CON0_SEND_INT_EN_Pos               (1)
#define OSPI_CON0_SEND_INT_EN_Msk               (0x01ul << OSPI_CON0_SEND_INT_EN_Pos)
#define OSPI_CON0_FIFO_INT_EN_Pos               (2)
#define OSPI_CON0_FIFO_INT_EN_Msk               (0x01ul << OSPI_CON0_FIFO_INT_EN_Pos)
#define OSPI_CON0_CPHA_Pos                      (3)
#define OSPI_CON0_CPHA_Msk                      (0x01ul << OSPI_CON0_CPHA_Pos)
#define OSPI_CON0_TRANS_MODE_Pos                (4)
#define OSPI_CON0_TRANS_MODE_Msk                (0x0Ful << OSPI_CON0_TRANS_MODE_Pos)
#define OSPI_CON0_DMA_EN_Pos                    (8)
#define OSPI_CON0_DMA_EN_Msk                    (0x01ul << OSPI_CON0_DMA_EN_Pos)
#define OSPI_CON0_SLAVE_Pos                     (9)
#define OSPI_CON0_SLAVE_Msk                     (0x01ul << OSPI_CON0_SLAVE_Pos)
#define OSPI_CON0_FIFO_WL_Pos                   (12)
#define OSPI_CON0_FIFO_WL_Msk                   (0x07ul << OSPI_CON0_FIFO_WL_Pos)

//OSPI_CON1
#define OSPI_CON1_XIP_SEL_Pos                   (0)
#define OSPI_CON1_XIP_SEL_Msk                   (0x01ul << OSPI_CON1_XIP_SEL_Pos)
#define OSPI_CON1_NORMAL_FIFO_Pos               (1)
#define OSPI_CON1_NORMAL_FIFO_Msk               (0x01ul << OSPI_CON1_NORMAL_FIFO_Pos)
#define OSPI_CON1_CMD_ON_Pos                    (2)
#define OSPI_CON1_CMD_ON_Msk                    (0x01ul << OSPI_CON1_CMD_ON_Pos)
#define OSPI_CON1_DIV_Pos                       (4)
#define OSPI_CON1_DIV_Msk                       (0x0Ful << OSPI_CON1_DIV_Pos)
#define OSPI_CON1_DUMMY_BYTE_Pos                (8)
#define OSPI_CON1_DUMMY_BYTE_Msk                (0xFFul << OSPI_CON1_DUMMY_BYTE_Pos)
#define OSPI_CON1_DUMMY_LEN_Pos                 (16)
#define OSPI_CON1_DUMMY_LEN_Msk                 (0x0Ful << OSPI_CON1_DUMMY_LEN_Pos)
#define OSPI_CON1_TSHSL_Pos                     (20)
#define OSPI_CON1_TSHSL_Msk                     (0x0Ful << OSPI_CON1_TSHSL_Pos)
#define OSPI_CON1_READ_CMD_Pos                  (24)
#define OSPI_CON1_READ_CMD_Msk                  (0xFFul << OSPI_CON1_READ_CMD_Pos)

//OSPI_CS
#define OSPI_CS_CS_Pos                          (0)
#define OSPI_CS_CS_Msk                          (0x01ul << OSPI_CS_CS_Pos)
#define OSPI_CS_SLV_RSTN_Pos                    (1)
#define OSPI_CS_SLV_RSTN_Msk                    (0x01ul << OSPI_CS_SLV_RSTN_Pos)

//OSPI_INT_STA
#define OSPI_NEND_IRQ_Pos                       (0)
#define OSPI_NEND_IRQ_Msk                       (0x01ul << OSPI_NEND_IRQ_Pos)
#define OSPI_SEND_IRQ_Pos                       (1)
#define OSPI_SEND_IRQ_Msk                       (0x01ul << OSPI_SEND_IRQ_Pos)
#define OSPI_FIFO_IRQ_Pos                       (2)
#define OSPI_FIFO_IRQ_Msk                       (0x01ul << OSPI_FIFO_IRQ_Pos)

//OSPI_FIFO_STA
#define OSPI_LEN_Pos                            (0)
#define OSPI_LEN_Msk                            (0x0Ful << OSPI_LEN_Pos)
#define OSPI_BCNT_Pos                           (8)
#define OSPI_BCNT_Msk                           (0x3Ful << OSPI_BCNT_Pos)
#define OSPI_FIFO_ERR_Pos                       (16)
#define OSPI_FIFO_ERR_Msk                       (0x01ul << OSPI_FIFO_ERR_Pos)
/*---------------------- END ---------------------------------*/

/*---------------------- PIN_CTRL ---------------------------------*/
/** 0x40090000 for Port0 and Port1 */
/** 0x40090040 for Port2 and Port3 */
typedef struct {
    __IO uint32_t AE;                   /*!< Offset: 0x000 (R/W)  Analog Function Enable Register */
    __IO uint32_t AT;                   /*!< Offset: 0x004 (R/W)  Analog Test Mode Register */
    __IO uint32_t UE;                   /*!< Offset: 0x008 (R/W)  Internal PullUp Enable Register */
    __IO uint32_t RE;                   /*!< Offset: 0x00c (R/W)  Internal Pull Resistor Enable Register */
    __IO uint32_t OD;                   /*!< Offset: 0x010 (R/W)  Open Drain Enable Register */
    __IO uint32_t VCM;                  /*!< Offset: 0x014 (R/W)  Supplies External Power Register */
} PIN_CTRL_T;
/*---------------------- END ---------------------------------*/

/*---------------------- PIN_MUX ---------------------------------*/
/** 0x40090018 for Port0 and Port1 */
/** 0x40090058 for Port2 and Port3 */
typedef struct {
    __IO uint32_t PORT_MUX0;            /*!< Offset: 0x000 (R/W)  Multiplexing Function Selection Register0 */
    __IO uint32_t PORT_MUX1;            /*!< Offset: 0x000 (R/W)  Multiplexing Function Selection Register1 */
    __IO uint32_t PORT_MUX2;            /*!< Offset: 0x000 (R/W)  Multiplexing Function Selection Register2 */
    __IO uint32_t PORT_MUX3;            /*!< Offset: 0x000 (R/W)  Multiplexing Function Selection Register3 */
} PINMUX_T;
/*---------------------- END ---------------------------------*/

/*---------------------- GPIO ---------------------------------*/
/** 0x50010000 for GPIO0 */
/** 0x50010040 for GPIO1 */
/** 0x50010080 for GPIO2 */
/** 0x500100C0 for GPIO3 */
typedef struct {
    __IO uint32_t PMODE;                /*!< Offset: 0x000 (R/W)  GPIO port mode Register */
    __IO uint32_t DOUT;                 /*!< Offset: 0x004 (R/W)  GPIO data out Register */
    __IO uint32_t DOUTMASK;             /*!< Offset: 0x008 (R/W)  GPIO data out write mask Register */
    __I  uint32_t PIN;                  /*!< Offset: 0x00c (R/W)  GPIO pin value Register */
    __IO uint32_t INTEN;                /*!< Offset: 0x010 (R/W)  GPIO interrupt enable Register */
    __IO uint32_t INTTYPE;              /*!< Offset: 0x014 (R/W)  GPIO interrupt type Register */
    __IO uint32_t INTPOL;               /*!< Offset: 0x018 (R/W)  GPIO interrupt polarity Register */
    __IO uint32_t INTSTR;               /*!< Offset: 0x01c (R/W)  GPIO interrupt status Register */
    __IO uint32_t DEBNEN;               /*!< Offset: 0x020 (R/W)  GPIO de-bounce enable Register */
    __IO uint32_t DEBNCON;              /*!< Offset: 0x024 (R/W)  GPIO de-bounce control Register */
} GPIO_T;
/*---------------------- END ---------------------------------*/

/*---------------------- Embedded Flash Controller ---------------------------*/
/** 0x50070000 */
typedef struct {
    __IO uint32_t RES0[8];           /*!< Offset: 0x000 ~ 0x01C (R/W) RFU Register */
    __IO uint32_t PROTECT;           /*!< Offset: 0x020 (R/W) Flash Operation Protect Register */
    __IO uint32_t RES1[7];           /*!< Offset: 0x024 ~ 0x03C (R/W) RFU Register */
    __IO uint32_t REMAP_DIS;         /*!< Offset: 0x040 (R/W) Flash Remap Disable Register */
    __IO uint32_t BOOT_NUM;          /*!< Offset: 0x044 (R/W) Number of Page for Boot Register */
    __IO uint32_t RO_NUM;            /*!< Offset: 0x048 (R/W) Number of Page for Read Only Register */
    __IO uint32_t CON;               /*!< Offset: 0x04C (R/W) Embeded Flash Controller Register */
    __IO uint32_t FPROG_LEN;         /*!< Offset: 0x050 (R/W) Length of Quick Write Register */
    __IO uint32_t ADDR;              /*!< Offset: 0x054 (R/W) Address of Flash Operation Register */
    __IO uint32_t WDATA;             /*!< Offset: 0x058 (R/W) Data of Flash Operation Register */
    __IO uint32_t TEIM00;            /*!< Offset: 0x05C (R) The Trim00 Data Register */
    __IO uint32_t TEIM01;            /*!< Offset: 0x060 (R) The Trim01 Data Register */
    __IO uint32_t TEIM02;            /*!< Offset: 0x064 (R) The Trim02 Data Register */
    __IO uint32_t TEIM03;            /*!< Offset: 0x068 (R) The Trim03 Data Register */
    __IO uint32_t TEIM10;            /*!< Offset: 0x06C (R) The Trim10 Data Register */
    __IO uint32_t TEIM11;            /*!< Offset: 0x070 (R) The Trim11 Data Register */
    __IO uint32_t TEIM12;            /*!< Offset: 0x074 (R) The Trim12 Data Register */
    __IO uint32_t TEIM13;            /*!< Offset: 0x078 (R) The Trim13 Data Register */
    __IO uint32_t RES2;              /*!< Offset: 0x07C (R/W) RFU Register */
    __IO uint32_t STA;               /*!< Offset: 0x080 (R/W) Status of The Flash Register */
} EFC_T;

//EFC_REMAP_DIS
#define EFC_REMAP_DIS_Pos                  (0)
#define EFC_REMAP_DIS_Msk                  (0x1ul  << EFC_REMAP_DIS_Pos)

//EFC_BOOT_NUM
#define EFC_BOOT_NUM_Pos                   (0)
#define EFC_BOOT_NUM_Msk                   (0x3ful << EFC_BOOT_NUM_Pos)

//EF_RO_NUM
#define EFC_RO_MAIN_Pos                    (0)
#define EFC_RO_MAIN_Msk                    (0x3ful << EFC_RO_MAIN_Pos)
#define EFC_RO_INFO_Pos                    (8)
#define EFC_RO_INFO_Msk                    (0x3ul << EFC_RO_INFO_Pos)

//EFC_CON
#define EFC_FLASH_CMD_Pos                  (0)
#define EFC_FLASH_CMD_Msk                  (0xfful << EFC_FLASH_CMD_Pos)

//EFC_FPROG_LEN
#define EFC_FPROG_LEN_Pos                  (0)
#define EFC_FPROG_LEN_Msk                  (0xfful << EFC_FPROG_LEN_Pos)

//EFC_STA
#define EFC_FLASH_OP_DONE_Pos              (0)
#define EFC_FLASH_OP_DONE_Msk              (0x1ul << EFC_FLASH_OP_DONE_Pos)
#define EFC_FLASH_PROTECT_Pos              (1)
#define EFC_FLASH_PROTECT_Msk              (0x1ul << EFC_FLASH_PROTECT_Pos)
#define EFC_PWR33_WARNING_Pos              (2)
#define EFC_PWR33_WARNING_Msk              (0x1ul << EFC_PWR33_WARNING_Pos)

/** 0x50070200 ~ 0x500703FF*/
typedef struct {
    __IO uint32_t BUF[128];
} EFC_BUF_T;
/*---------------------- END ---------------------------------*/





/*---------------------- TRNG ---------------------------------*/
typedef struct {
    __IO uint32_t RNG;             /*!< Offset: 0x000 (R/W)  True random number generate Register */
} TRNG_T;
/*---------------------- END ---------------------------------*/


/*---------------------- CRC ---------------------------------*/
typedef struct {
    __IO uint32_t CON     ;         /*!< Offset: 0x000 (R/W)  CRC16 configuration Register */
    __IO uint32_t INIT    ;         /*!< Offset: 0x004 (R/W)  CRC16 initial value Register */
    __IO uint32_t DIN     ;         /*!< Offset: 0x008 (R/W)  CRC16 input data Register */
    __IO uint32_t DOUT    ;         /*!< Offset: 0x00C (R/W)  CRC16 result Register */
} CRC_T;

//CRC_CON
#define CRC_CON_START_Pos                    (0)
#define CRC_CON_START_Msk                    (0x1ul << CRC_CON_START_Pos)
#define CRC_CON_DOUT_INV_Pos                 (1)
#define CRC_CON_DOUT_INV_Msk                 (0x1ul << CRC_CON_DOUT_INV_Pos)
/*---------------------- END ---------------------------------*/


/*--------------------- WWDG ---------------------------------*/
typedef struct {
    __IO uint32_t CFR;              /*!< Offset: 0x000 (R/W) WWDT Configuration Register */
    __IO uint32_t SR;               /*!< Offset: 0x004 (R/W) WWDT Status Register */
    __IO uint32_t CR;               /*!< Offset: 0x008 (R/W) WWDT Control Register */
} WWDG_T;

//WWDG_CFR
#define WWDG_EWI_Pos                      (9)
#define WWDG_EWI_Msk                      (0x1ul << WWDG_EWI_Pos)
#define WWDG_WDGTB_Pos                    (7)
#define WWDG_WDGTB_Msk                    (0x3ul << WWDG_WDGTB_Pos)
#define WWDG_WINDOW_VALUE_Pos             (0)
#define WWDG_WINDOW_VALUE_Msk             (0x7ful << WWDG_WINDOW_VALUE_Pos)

//WWDG_SR
#define WWDG_EWIF_Pos                     (0)
#define WWDG_EWIF_Msk                     (0x1ul << WWDG_EWIF_Pos)

//WWDG_CR
#define WWDG_WDGA_Pos                     (7)
#define WWDG_WDGA_Msk                     (0x1ul << WWDG_WDGA_Pos)
#define WWDG_T_Pos                        (0)
#define WWDG_T_Msk                        (0x7ful << WWDG_T_Pos)

/*---------------------- END ---------------------------------*/



/*--------------------- TIMERA ---------------------------------*/
typedef struct
{
    __IO uint32_t TAR  ;           //0x00 16bits timer
    __IO uint32_t TACTL;           //0x04 Control register
    __IO uint32_t TAIE ;           //0x08 Timer_A Interrupt enable
    __IO uint32_t TAI  ;           //0x0c Timer_A Interrupt
    __IO uint32_t TACCTL0;         //0x10 Capture/Compare Control Register
    __IO uint32_t TACCTL1;         //0x14 Capture/Compare Control Register
    __IO uint32_t TACCTL2;         //0x18 Capture/Compare Control Register
    __IO uint32_t TACCTL3;         //0x1C Capture/Compare Control Register
    __IO uint32_t TACCTL4;         //0x20 Capture/Compare Control Register
    __IO uint32_t TACCTL5;         //0x24 Capture/Compare Control Register
    __IO uint32_t TACCTL6;         //0x28 Capture/Compare Control Register
    __IO uint32_t TACCR0;          //0x2C Capture/Compare Register
    __IO uint32_t TACCR1;          //0x30 Capture/Compare Register
    __IO uint32_t TACCR2;          //0x34 Capture/Compare Register
    __IO uint32_t TACCR3;          //0x38 Capture/Compare Register
    __IO uint32_t TACCR4;          //0x3C Capture/Compare Register
    __IO uint32_t TACCR5;          //0x40 Capture/Compare Register
    __IO uint32_t TACCR6;          //0x44 Capture/Compare Register
} TMRA_T;

//TAR
#define TMRA_TAR_Pos                     (0)
#define TMRA_TAR_Msk                     (0x0000FFFFul)

//TACTL
#define TMRA_ID_Pos                      (6)
#define TMRA_ID_Msk                      (0x3ul << TMRA_ID_Pos)
#define TMRA_IDEX_Pos                    (3)
#define TMRA_IDEX_Msk                    (0x7ul << TMRA_IDEX_Pos)
#define TMRA_MC_Pos                      (1)
#define TMRA_MC_Msk                      (0x3ul << TMRA_MC_Pos)
#define TMRA_TACLR_Pos                   (0)
#define TMRA_TACLR_Msk                   (0x1ul << TMRA_TACLR_Pos)

//TAIE
#define TMRA_DMA0_Pos                    (8)
#define TMRA_DMA1_Pos                    (9)
#define TMRA_DMA2_Pos                    (10)
#define TMRA_DMA3_Pos                    (11)
#define TMRA_DMA4_Pos                    (12)
#define TMRA_DMA5_Pos                    (13)
#define TMRA_DMA6_Pos                    (14)
#define TMRA_DMA7_Pos                    (15)
#define TMRA_DMA0_Msk                    (0x1ul << TMRA_DMA0_Pos)
#define TMRA_DMA1_Msk                    (0x1ul << TMRA_DMA1_Pos)
#define TMRA_DMA2_Msk                    (0x1ul << TMRA_DMA2_Pos)
#define TMRA_DMA3_Msk                    (0x1ul << TMRA_DMA3_Pos)
#define TMRA_DMA4_Msk                    (0x1ul << TMRA_DMA4_Pos)
#define TMRA_DMA5_Msk                    (0x1ul << TMRA_DMA5_Pos)
#define TMRA_DMA6_Msk                    (0x1ul << TMRA_DMA6_Pos)
#define TMRA_DMA7_Msk                    (0x1ul << TMRA_DMA7_Pos)
#define TMRA_CCIE0_Pos                   (0)
#define TMRA_CCIE1_Pos                   (1)
#define TMRA_CCIE2_Pos                   (2)
#define TMRA_CCIE3_Pos                   (3)
#define TMRA_CCIE4_Pos                   (4)
#define TMRA_CCIE5_Pos                   (5)
#define TMRA_CCIE6_Pos                   (6)
#define TMRA_TAIE_Pos                    (7)
#define TMRA_CCIE0_Msk                   (0x1ul << TMRA_CCIE0_Pos)
#define TMRA_CCIE1_Msk                   (0x1ul << TMRA_CCIE1_Pos)
#define TMRA_CCIE2_Msk                   (0x1ul << TMRA_CCIE2_Pos)
#define TMRA_CCIE3_Msk                   (0x1ul << TMRA_CCIE3_Pos)
#define TMRA_CCIE4_Msk                   (0x1ul << TMRA_CCIE4_Pos)
#define TMRA_CCIE5_Msk                   (0x1ul << TMRA_CCIE5_Pos)
#define TMRA_CCIE6_Msk                   (0x1ul << TMRA_CCIE6_Pos)
#define TMRA_TAIE_Msk                    (0x1ul << TMRA_TAIE_Pos)

//TAI
#define TMRA_CCIFG0_Pos                  (0)
#define TMRA_CCIFG1_Pos                  (1)
#define TMRA_CCIFG2_Pos                  (2)
#define TMRA_CCIFG3_Pos                  (3)
#define TMRA_CCIFG4_Pos                  (4)
#define TMRA_CCIFG5_Pos                  (5)
#define TMRA_CCIFG6_Pos                  (6)
#define TMRA_TAIFG_Pos                   (7)
#define TMRA_CCIFG0_Msk                  (0x1ul << TMRA_CCIFG0_Pos)
#define TMRA_CCIFG1_Msk                  (0x1ul << TMRA_CCIFG1_Pos)
#define TMRA_CCIFG2_Msk                  (0x1ul << TMRA_CCIFG2_Pos)
#define TMRA_CCIFG3_Msk                  (0x1ul << TMRA_CCIFG3_Pos)
#define TMRA_CCIFG4_Msk                  (0x1ul << TMRA_CCIFG4_Pos)
#define TMRA_CCIFG5_Msk                  (0x1ul << TMRA_CCIFG5_Pos)
#define TMRA_CCIFG6_Msk                  (0x1ul << TMRA_CCIFG6_Pos)
#define TMRA_TAIFG_Msk                   (0x1ul << TMRA_TAIFG_Pos)

//TACCTLx
#define TMRA_P_CFG_Pos                   (18)
#define TMRA_P_CFG_Msk                   (0x3ul << TMRA_P_CFG_Pos)
#define TMRA_N_CFG_Pos                   (16)
#define TMRA_N_CFG_Msk                   (0x3ul << TMRA_N_CFG_Pos)
#define TMRA_CM_Pos                      (14)
#define TMRA_CM_Msk                      (0x3ul << TMRA_CM_Pos)
#define TMRA_CCIS_Pos                    (12)
#define TMRA_CCIS_Msk                    (0x3ul << TMRA_CCIS_Pos)
#define TMRA_SCCI_Pos                    (10)
#define TMRA_SCCI_Msk                    (0x1ul << TMRA_SCCI_Pos)
#define TMRA_CAP_Pos                     (9)
#define TMRA_CAP_Msk                     (0x1ul << TMRA_CAP_Pos)
#define TMRA_DEADE_Pos                   (8)
#define TMRA_DEADE_Msk                   (0x1ul << TMRA_DEADE_Pos)
#define TMRA_OUTMOD_Pos                  (5)
#define TMRA_OUTMOD_Msk                  (0x7ul << TMRA_OUTMOD_Pos)
#define TMRA_CCI_Pos                     (3)
#define TMRA_CCI_Msk                     (0x1ul << TMRA_CCI_Pos)
#define TMRA_OUT_Pos                     (2)
#define TMRA_OUT_Msk                     (0x1ul << TMRA_OUT_Pos)
#define TMRA_COV_Pos                     (1)
#define TMRA_COV_Msk                     (0x1ul << TMRA_COV_Pos)

//TACCRx
#define TMRA_TACCR_Pos                   (0)
#define TMRA_TACCR_Msk                   (0x0000FFFFul)

/*---------------------- END ---------------------------------*/

/*---------------------- TMRB ---------------------------------*/
typedef struct
{
    __IO uint32_t T0LC;            //0x00 value to be loaded into timer0
    __I  uint32_t T0CV;            //0x04 current value of timer0
    __IO uint32_t T0CTL;           //0x08 control register for timer0
    __I  uint32_t T0EOI;           //0x0c clear the interrupt form timer0
    __I  uint32_t T0ISR;           //0x10 contains the interrupt status for timer0
    __IO uint32_t T1LC;            //0x14 value to be loaded into timer1
    __I  uint32_t T1CV;            //0x18 current value of timer1
    __IO uint32_t T1CTL;           //0x1c control register for timer1
    __I  uint32_t T1EOI;           //0x20 clear the interrupt form timer1
    __I  uint32_t T1ISR;           //0x24 contains the interrupt status for timer1
    __IO uint32_t T2LC;            //0x28 value to be loaded into timer2
    __I  uint32_t T2CV;            //0x2c current value of timer2
    __IO uint32_t T2CTL;           //0x30 control register for timer2
    __I  uint32_t T2EOI;           //0x34 clear the interrupt form timer2
    __I  uint32_t T2ISR;           //0x38 contains the interrupt status for timer2
    __IO uint32_t T3LC;            //0x3c value to be loaded into timer3
    __I  uint32_t T3CV;            //0x40 current value of timer3
    __IO uint32_t T3CTL;           //0x44 control register for timer3
    __I  uint32_t T3EOI;           //0x48 clear the interrupt form timer3
    __I  uint32_t T3ISR;           //0x4c contains the interrupt status for timer3
} TMRB_T;

typedef struct
{
    __I uint32_t TSISR;
    __I uint32_t TSEOI;
    __I uint32_t TSRAWINT;
} TMRBS_T;

#define TMRB_LC_Pos                      (0)
#define TMRB_LC_Msk                      (0xfffffffful)
#define TMRB_CV_Pos                      (0)
#define TMRB_LC_Msk                      (0xfffffffful)
#define TMRB_EN_Pos                      (0)
#define TMRB_EN_Msk                      (0x1ul << TMRB_EN_Pos)
#define TMRB_MOD_Pos                     (1)
#define TMRB_MOD_Msk                     (0x1ul << TMRB_MOD_Pos)
#define TMRB_IMK_Pos                     (2)
#define TMRB_IMK_Msk                     (0x1ul << TMRB_IMK_Pos)
#define TMRB_EOI_Pos                     (0)
#define TMRB_EOI_Msk                     (0x1ul << TMRB_EOI_Pos)
#define TMRB_ISR_Pos                     (0)
#define TMRB_ISR_Msk                     (0x1ul << TMRB_ISR_Pos)

/*---------------------- CAN ---------------------------------*/
//0x50030000u

/** CAN - Size of Registers Arrays */
#define CAN_RAMn_COUNT                           128u
#define CAN_RXIMR_COUNT                          32u
#define CAN_WMB_COUNT                            4u

/** CAN - Register Layout Typedef */
typedef struct {
    __IO uint32_t MCR;                               /**< Module Configuration Register, offset: 0x0 */
    __IO uint32_t CTRL1;                             /**< Control 1 register, offset: 0x4 */
    __IO uint32_t TIMER;                             /**< Free Running Timer, offset: 0x8 */
    uint8_t  RESERVED_0[4];
    __IO uint32_t RXMGMASK;                          /**< Rx Mailboxes Global Mask Register, offset: 0x10 */
    __IO uint32_t RX14MASK;                          /**< Rx 14 Mask register, offset: 0x14 */
    __IO uint32_t RX15MASK;                          /**< Rx 15 Mask register, offset: 0x18 */
    __IO uint32_t ECR;                               /**< Error Counter, offset: 0x1C */
    __IO uint32_t ESR1;                              /**< Error and Status 1 register, offset: 0x20 */
    uint8_t  RESERVED_1[4];
    __IO uint32_t IMASK1;                            /**< Interrupt Masks 1 register, offset: 0x28 */
    uint8_t  RESERVED_2[4];
    __IO uint32_t IFLAG1;                            /**< Interrupt Flags 1 register, offset: 0x30 */
    __IO uint32_t CTRL2;                             /**< Control 2 register, offset: 0x34 */
    __I  uint32_t ESR2;                              /**< Error and Status 2 register, offset: 0x38 */
    uint8_t  RESERVED_3[8];
    __I  uint32_t CRCR;                              /**< CRC Register, offset: 0x44 */
    __IO uint32_t RXFGMASK;                          /**< Rx FIFO Global Mask register, offset: 0x48 */
    __I  uint32_t RXFIR;                             /**< Rx FIFO Information Register, offset: 0x4C */
    __IO uint32_t CBT;                               /**< CAN Bit Timing Register, offset: 0x50 */
    uint8_t  RESERVED_4[44];
    __IO uint32_t RAMn[CAN_RAMn_COUNT];              /**< Embedded RAM, array offset: 0x80, array step: 0x4 */
    uint8_t  RESERVED_5[1536];
    __IO uint32_t RXIMR[CAN_RXIMR_COUNT];            /**< Rx Individual Mask Registers, array offset: 0x880, array step: 0x4 */
    uint8_t  RESERVED_6[480];
    __IO uint32_t MECR;                              /* MECR  0xAE0   */
    uint8_t  RESERVED_66[28];
    __IO uint32_t CTRL1_PN;                          /**< Pretended Networking Control 1 Register, offset: 0xB00 */
    __IO uint32_t CTRL2_PN;                          /**< Pretended Networking Control 2 Register, offset: 0xB04 */
    __IO uint32_t WU_MTC;                            /**< Pretended Networking Wake Up Match Register, offset: 0xB08 */
    __IO uint32_t FLT_ID1;                           /**< Pretended Networking ID Filter 1 Register, offset: 0xB0C */
    __IO uint32_t FLT_DLC;                           /**< Pretended Networking DLC Filter Register, offset: 0xB10 */
    __IO uint32_t PL1_LO;                            /**< Pretended Networking Payload Low Filter 1 Register, offset: 0xB14 */
    __IO uint32_t PL1_HI;                            /**< Pretended Networking Payload High Filter 1 Register, offset: 0xB18 */
    __IO uint32_t FLT_ID2_IDMASK;                    /**< Pretended Networking ID Filter 2 Register / ID Mask Register, offset: 0xB1C */
    __IO uint32_t PL2_PLMASK_LO;                     /**< Pretended Networking Payload Low Filter 2 Register / Payload Low Mask Register, offset: 0xB20 */
    __IO uint32_t PL2_PLMASK_HI;                     /**< Pretended Networking Payload High Filter 2 low order bits / Payload High Mask Register, offset: 0xB24 */
    uint8_t  RESERVED_7[24];
    struct {                                         /* offset: 0xB40, array step: 0x10 */
        __I  uint32_t WMBn_CS;                           /**< Wake Up Message Buffer Register for C/S, array offset: 0xB40, array step: 0x10 */
        __I  uint32_t WMBn_ID;                           /**< Wake Up Message Buffer Register for ID, array offset: 0xB44, array step: 0x10 */
        __I  uint32_t WMBn_D03;                          /**< Wake Up Message Buffer Register for Data 0-3, array offset: 0xB48, array step: 0x10 */
        __I  uint32_t WMBn_D47;                          /**< Wake Up Message Buffer Register Data 4-7, array offset: 0xB4C, array step: 0x10 */
    } WMB[CAN_WMB_COUNT];
    uint8_t  RESERVED_8[128];
    __IO uint32_t FDCTRL;                            /**< CAN FD Control Register, offset: 0xC00 */
    __IO uint32_t FDCBT;                             /**< CAN FD Bit Timing Register, offset: 0xC04 */
    __I  uint32_t FDCRC;                             /**< CAN FD CRC Register, offset: 0xC08 */
} CAN_Type, *CAN_MemMapPtr;

/** Number of instances of the CAN module. */
#define CAN_INSTANCE_COUNT                       (2u)
/** Number of interrupt vector arrays for the CAN module. */
#define CAN_IRQS_ARR_COUNT                       (7u)
/** Number of interrupt channels for the Rx_Warning type of CAN module. */
#define CAN_Rx_Warning_IRQS_CH_COUNT             (1u)
/** Number of interrupt channels for the Tx_Warning type of CAN module. */
#define CAN_Tx_Warning_IRQS_CH_COUNT             (1u)
/** Number of interrupt channels for the Wake_Up type of CAN module. */
#define CAN_Wake_Up_IRQS_CH_COUNT                (1u)
/** Number of interrupt channels for the Error type of CAN module. */
#define CAN_Error_IRQS_CH_COUNT                  (1u)
/** Number of interrupt channels for the Bus_Off type of CAN module. */
#define CAN_Bus_Off_IRQS_CH_COUNT                (1u)
/** Number of interrupt channels for the ORed_0_15_MB type of CAN module. */
#define CAN_ORed_0_15_MB_IRQS_CH_COUNT           (1u)
/** Number of interrupt channels for the ORed_16_31_MB type of CAN module. */
#define CAN_ORed_16_31_MB_IRQS_CH_COUNT          (1u)

/** Interrupt vectors for the CAN peripheral type */
#define CAN_Rx_Warning_IRQS                      {CAN0_IRQn, CAN1_IRQn }
#define CAN_Tx_Warning_IRQS                      {CAN0_IRQn, CAN1_IRQn }
#define CAN_Wake_Up_IRQS                         {CAN0_IRQn, CAN1_IRQn }
#define CAN_Error_IRQS                           {CAN0_IRQn, CAN1_IRQn }
#define CAN_Bus_Off_IRQS                         {CAN0_IRQn, CAN1_IRQn }
#define CAN_ORed_0_15_MB_IRQS                    {CAN0_IRQn, CAN1_IRQn }
#define CAN_ORed_16_31_MB_IRQS                   {CAN0_IRQn, CAN1_IRQn }
/* ----------------------------------------------------------------------------
   -- CAN Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CAN_Register_Masks CAN Register Masks
 * @{
 */

/* MCR Bit Fields */
#define CAN_MCR_MAXMB_MASK                       0x7Fu
#define CAN_MCR_MAXMB_SHIFT                      0u
#define CAN_MCR_MAXMB_WIDTH                      7u
#define CAN_MCR_MAXMB(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_MCR_MAXMB_SHIFT))&CAN_MCR_MAXMB_MASK)
#define CAN_MCR_IDAM_MASK                        0x300u
#define CAN_MCR_IDAM_SHIFT                       8u
#define CAN_MCR_IDAM_WIDTH                       2u
#define CAN_MCR_IDAM(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_IDAM_SHIFT))&CAN_MCR_IDAM_MASK)
#define CAN_MCR_FDEN_MASK                        0x800u
#define CAN_MCR_FDEN_SHIFT                       11u
#define CAN_MCR_FDEN_WIDTH                       1u
#define CAN_MCR_FDEN(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FDEN_SHIFT))&CAN_MCR_FDEN_MASK)
#define CAN_MCR_AEN_MASK                         0x1000u
#define CAN_MCR_AEN_SHIFT                        12u
#define CAN_MCR_AEN_WIDTH                        1u
#define CAN_MCR_AEN(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_AEN_SHIFT))&CAN_MCR_AEN_MASK)
#define CAN_MCR_LPRIOEN_MASK                     0x2000u
#define CAN_MCR_LPRIOEN_SHIFT                    13u
#define CAN_MCR_LPRIOEN_WIDTH                    1u
#define CAN_MCR_LPRIOEN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_LPRIOEN_SHIFT))&CAN_MCR_LPRIOEN_MASK)
#define CAN_MCR_PNET_EN_MASK                     0x4000u
#define CAN_MCR_PNET_EN_SHIFT                    14u
#define CAN_MCR_PNET_EN_WIDTH                    1u
#define CAN_MCR_PNET_EN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_PNET_EN_SHIFT))&CAN_MCR_PNET_EN_MASK)
#define CAN_MCR_DMA_MASK                         0x8000u
#define CAN_MCR_DMA_SHIFT                        15u
#define CAN_MCR_DMA_WIDTH                        1u
#define CAN_MCR_DMA(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_DMA_SHIFT))&CAN_MCR_DMA_MASK)
#define CAN_MCR_IRMQ_MASK                        0x10000u
#define CAN_MCR_IRMQ_SHIFT                       16u
#define CAN_MCR_IRMQ_WIDTH                       1u
#define CAN_MCR_IRMQ(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_IRMQ_SHIFT))&CAN_MCR_IRMQ_MASK)
#define CAN_MCR_SRXDIS_MASK                      0x20000u
#define CAN_MCR_SRXDIS_SHIFT                     17u
#define CAN_MCR_SRXDIS_WIDTH                     1u
#define CAN_MCR_SRXDIS(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SRXDIS_SHIFT))&CAN_MCR_SRXDIS_MASK)
#define CAN_MCR_LPMACK_MASK                      0x100000u
#define CAN_MCR_LPMACK_SHIFT                     20u
#define CAN_MCR_LPMACK_WIDTH                     1u
#define CAN_MCR_LPMACK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_LPMACK_SHIFT))&CAN_MCR_LPMACK_MASK)
#define CAN_MCR_WRNEN_MASK                       0x200000u
#define CAN_MCR_WRNEN_SHIFT                      21u
#define CAN_MCR_WRNEN_WIDTH                      1u
#define CAN_MCR_WRNEN(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_MCR_WRNEN_SHIFT))&CAN_MCR_WRNEN_MASK)
//meng++
#define CAN_MCR_SLFWAK_MASK                      0x400000u
#define CAN_MCR_SLFWAK_SHIFT                     22u
#define CAN_MCR_SLFWAK_WIDTH                     1u
#define CAN_MCR_SLFWAK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SLFWAK_SHIFT))&CAN_MCR_SLFWAK_MASK)

#define CAN_MCR_SUPV_MASK                        0x800000u
#define CAN_MCR_SUPV_SHIFT                       23u
#define CAN_MCR_SUPV_WIDTH                       1u
#define CAN_MCR_SUPV(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SUPV_SHIFT))&CAN_MCR_SUPV_MASK)
#define CAN_MCR_FRZACK_MASK                      0x1000000u
#define CAN_MCR_FRZACK_SHIFT                     24u
#define CAN_MCR_FRZACK_WIDTH                     1u
#define CAN_MCR_FRZACK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FRZACK_SHIFT))&CAN_MCR_FRZACK_MASK)
#define CAN_MCR_SOFTRST_MASK                     0x2000000u
#define CAN_MCR_SOFTRST_SHIFT                    25u
#define CAN_MCR_SOFTRST_WIDTH                    1u
#define CAN_MCR_SOFTRST(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_MCR_SOFTRST_SHIFT))&CAN_MCR_SOFTRST_MASK)
//meng++
#define CAN_MCR_WAKMSK_MASK                      0x4000000u
#define CAN_MCR_WAKMSK_SHIFT                     26u
#define CAN_MCR_WAKMSK_WIDTH                     1u
#define CAN_MCR_WAKMSK(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_WAKMSK_SHIFT))&CAN_MCR_WAKMSK_MASK)

#define CAN_MCR_NOTRDY_MASK                      0x8000000u
#define CAN_MCR_NOTRDY_SHIFT                     27u
#define CAN_MCR_NOTRDY_WIDTH                     1u
#define CAN_MCR_NOTRDY(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_MCR_NOTRDY_SHIFT))&CAN_MCR_NOTRDY_MASK)
#define CAN_MCR_HALT_MASK                        0x10000000u
#define CAN_MCR_HALT_SHIFT                       28u
#define CAN_MCR_HALT_WIDTH                       1u
#define CAN_MCR_HALT(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_HALT_SHIFT))&CAN_MCR_HALT_MASK)
#define CAN_MCR_RFEN_MASK                        0x20000000u
#define CAN_MCR_RFEN_SHIFT                       29u
#define CAN_MCR_RFEN_WIDTH                       1u
#define CAN_MCR_RFEN(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_RFEN_SHIFT))&CAN_MCR_RFEN_MASK)
#define CAN_MCR_FRZ_MASK                         0x40000000u
#define CAN_MCR_FRZ_SHIFT                        30u
#define CAN_MCR_FRZ_WIDTH                        1u
#define CAN_MCR_FRZ(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_MCR_FRZ_SHIFT))&CAN_MCR_FRZ_MASK)
#define CAN_MCR_MDIS_MASK                        0x80000000u
#define CAN_MCR_MDIS_SHIFT                       31u
#define CAN_MCR_MDIS_WIDTH                       1u
#define CAN_MCR_MDIS(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_MCR_MDIS_SHIFT))&CAN_MCR_MDIS_MASK)
/* CTRL1 Bit Fields */
#define CAN_CTRL1_PROPSEG_MASK                   0x7u
#define CAN_CTRL1_PROPSEG_SHIFT                  0u
#define CAN_CTRL1_PROPSEG_WIDTH                  3u
#define CAN_CTRL1_PROPSEG(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PROPSEG_SHIFT))&CAN_CTRL1_PROPSEG_MASK)
#define CAN_CTRL1_LOM_MASK                       0x8u
#define CAN_CTRL1_LOM_SHIFT                      3u
#define CAN_CTRL1_LOM_WIDTH                      1u
#define CAN_CTRL1_LOM(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LOM_SHIFT))&CAN_CTRL1_LOM_MASK)
#define CAN_CTRL1_LBUF_MASK                      0x10u
#define CAN_CTRL1_LBUF_SHIFT                     4u
#define CAN_CTRL1_LBUF_WIDTH                     1u
#define CAN_CTRL1_LBUF(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LBUF_SHIFT))&CAN_CTRL1_LBUF_MASK)
#define CAN_CTRL1_TSYN_MASK                      0x20u
#define CAN_CTRL1_TSYN_SHIFT                     5u
#define CAN_CTRL1_TSYN_WIDTH                     1u
#define CAN_CTRL1_TSYN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_TSYN_SHIFT))&CAN_CTRL1_TSYN_MASK)
#define CAN_CTRL1_BOFFREC_MASK                   0x40u
#define CAN_CTRL1_BOFFREC_SHIFT                  6u
#define CAN_CTRL1_BOFFREC_WIDTH                  1u
#define CAN_CTRL1_BOFFREC(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_BOFFREC_SHIFT))&CAN_CTRL1_BOFFREC_MASK)
#define CAN_CTRL1_SMP_MASK                       0x80u
#define CAN_CTRL1_SMP_SHIFT                      7u
#define CAN_CTRL1_SMP_WIDTH                      1u
#define CAN_CTRL1_SMP(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_SMP_SHIFT))&CAN_CTRL1_SMP_MASK)
#define CAN_CTRL1_RWRNMSK_MASK                   0x400u
#define CAN_CTRL1_RWRNMSK_SHIFT                  10u
#define CAN_CTRL1_RWRNMSK_WIDTH                  1u
#define CAN_CTRL1_RWRNMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_RWRNMSK_SHIFT))&CAN_CTRL1_RWRNMSK_MASK)
#define CAN_CTRL1_TWRNMSK_MASK                   0x800u
#define CAN_CTRL1_TWRNMSK_SHIFT                  11u
#define CAN_CTRL1_TWRNMSK_WIDTH                  1u
#define CAN_CTRL1_TWRNMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_TWRNMSK_SHIFT))&CAN_CTRL1_TWRNMSK_MASK)
#define CAN_CTRL1_LPB_MASK                       0x1000u
#define CAN_CTRL1_LPB_SHIFT                      12u
#define CAN_CTRL1_LPB_WIDTH                      1u
#define CAN_CTRL1_LPB(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_LPB_SHIFT))&CAN_CTRL1_LPB_MASK)
#define CAN_CTRL1_CLKSRC_MASK                    0x2000u
#define CAN_CTRL1_CLKSRC_SHIFT                   13u
#define CAN_CTRL1_CLKSRC_WIDTH                   1u
#define CAN_CTRL1_CLKSRC(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_CLKSRC_SHIFT))&CAN_CTRL1_CLKSRC_MASK)
#define CAN_CTRL1_ERRMSK_MASK                    0x4000u
#define CAN_CTRL1_ERRMSK_SHIFT                   14u
#define CAN_CTRL1_ERRMSK_WIDTH                   1u
#define CAN_CTRL1_ERRMSK(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_ERRMSK_SHIFT))&CAN_CTRL1_ERRMSK_MASK)
#define CAN_CTRL1_BOFFMSK_MASK                   0x8000u
#define CAN_CTRL1_BOFFMSK_SHIFT                  15u
#define CAN_CTRL1_BOFFMSK_WIDTH                  1u
#define CAN_CTRL1_BOFFMSK(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_BOFFMSK_SHIFT))&CAN_CTRL1_BOFFMSK_MASK)
#define CAN_CTRL1_PSEG2_MASK                     0x70000u
#define CAN_CTRL1_PSEG2_SHIFT                    16u
#define CAN_CTRL1_PSEG2_WIDTH                    3u
#define CAN_CTRL1_PSEG2(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PSEG2_SHIFT))&CAN_CTRL1_PSEG2_MASK)
#define CAN_CTRL1_PSEG1_MASK                     0x380000u
#define CAN_CTRL1_PSEG1_SHIFT                    19u
#define CAN_CTRL1_PSEG1_WIDTH                    3u
#define CAN_CTRL1_PSEG1(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PSEG1_SHIFT))&CAN_CTRL1_PSEG1_MASK)
#define CAN_CTRL1_RJW_MASK                       0xC00000u
#define CAN_CTRL1_RJW_SHIFT                      22u
#define CAN_CTRL1_RJW_WIDTH                      2u
#define CAN_CTRL1_RJW(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_RJW_SHIFT))&CAN_CTRL1_RJW_MASK)
#define CAN_CTRL1_PRESDIV_MASK                   0xFF000000u
#define CAN_CTRL1_PRESDIV_SHIFT                  24u
#define CAN_CTRL1_PRESDIV_WIDTH                  8u
#define CAN_CTRL1_PRESDIV(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PRESDIV_SHIFT))&CAN_CTRL1_PRESDIV_MASK)
/* TIMER Bit Fields */
#define CAN_TIMER_TIMER_MASK                     0xFFFFu
#define CAN_TIMER_TIMER_SHIFT                    0u
#define CAN_TIMER_TIMER_WIDTH                    16u
#define CAN_TIMER_TIMER(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_TIMER_TIMER_SHIFT))&CAN_TIMER_TIMER_MASK)
/* RXMGMASK Bit Fields */
#define CAN_RXMGMASK_MG_MASK                     0xFFFFFFFFu
#define CAN_RXMGMASK_MG_SHIFT                    0u
#define CAN_RXMGMASK_MG_WIDTH                    32u
#define CAN_RXMGMASK_MG(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_RXMGMASK_MG_SHIFT))&CAN_RXMGMASK_MG_MASK)
/* RX14MASK Bit Fields */
#define CAN_RX14MASK_RX14M_MASK                  0xFFFFFFFFu
#define CAN_RX14MASK_RX14M_SHIFT                 0u
#define CAN_RX14MASK_RX14M_WIDTH                 32u
#define CAN_RX14MASK_RX14M(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_RX14MASK_RX14M_SHIFT))&CAN_RX14MASK_RX14M_MASK)
/* RX15MASK Bit Fields */
#define CAN_RX15MASK_RX15M_MASK                  0xFFFFFFFFu
#define CAN_RX15MASK_RX15M_SHIFT                 0u
#define CAN_RX15MASK_RX15M_WIDTH                 32u
#define CAN_RX15MASK_RX15M(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_RX15MASK_RX15M_SHIFT))&CAN_RX15MASK_RX15M_MASK)
/* ECR Bit Fields */
#define CAN_ECR_TXERRCNT_MASK                    0xFFu
#define CAN_ECR_TXERRCNT_SHIFT                   0u
#define CAN_ECR_TXERRCNT_WIDTH                   8u
#define CAN_ECR_TXERRCNT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ECR_TXERRCNT_SHIFT))&CAN_ECR_TXERRCNT_MASK)
#define CAN_ECR_RXERRCNT_MASK                    0xFF00u
#define CAN_ECR_RXERRCNT_SHIFT                   8u
#define CAN_ECR_RXERRCNT_WIDTH                   8u
#define CAN_ECR_RXERRCNT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ECR_RXERRCNT_SHIFT))&CAN_ECR_RXERRCNT_MASK)
#define CAN_ECR_TXERRCNT_FAST_MASK               0xFF0000u
#define CAN_ECR_TXERRCNT_FAST_SHIFT              16u
#define CAN_ECR_TXERRCNT_FAST_WIDTH              8u
#define CAN_ECR_TXERRCNT_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ECR_TXERRCNT_FAST_SHIFT))&CAN_ECR_TXERRCNT_FAST_MASK)
#define CAN_ECR_RXERRCNT_FAST_MASK               0xFF000000u
#define CAN_ECR_RXERRCNT_FAST_SHIFT              24u
#define CAN_ECR_RXERRCNT_FAST_WIDTH              8u
#define CAN_ECR_RXERRCNT_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ECR_RXERRCNT_FAST_SHIFT))&CAN_ECR_RXERRCNT_FAST_MASK)
/* ESR1 Bit Fields */
#define CAN_ESR1_WAKINT_MASK                     0x1u
#define CAN_ESR1_WAKINT_SHIFT                    0u
#define CAN_ESR1_WAKINT_WIDTH                    1u
#define CAN_ESR1_WAKINT(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_WAKINT_SHIFT))&CAN_ESR1_WAKINT_MASK)

#define CAN_ESR1_ERRINT_MASK                     0x2u
#define CAN_ESR1_ERRINT_SHIFT                    1u
#define CAN_ESR1_ERRINT_WIDTH                    1u
#define CAN_ESR1_ERRINT(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERRINT_SHIFT))&CAN_ESR1_ERRINT_MASK)
#define CAN_ESR1_BOFFINT_MASK                    0x4u
#define CAN_ESR1_BOFFINT_SHIFT                   2u
#define CAN_ESR1_BOFFINT_WIDTH                   1u
#define CAN_ESR1_BOFFINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BOFFINT_SHIFT))&CAN_ESR1_BOFFINT_MASK)
#define CAN_ESR1_RX_MASK                         0x8u
#define CAN_ESR1_RX_SHIFT                        3u
#define CAN_ESR1_RX_WIDTH                        1u
#define CAN_ESR1_RX(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RX_SHIFT))&CAN_ESR1_RX_MASK)
#define CAN_ESR1_FLTCONF_MASK                    0x30u
#define CAN_ESR1_FLTCONF_SHIFT                   4u
#define CAN_ESR1_FLTCONF_WIDTH                   2u
#define CAN_ESR1_FLTCONF(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FLTCONF_SHIFT))&CAN_ESR1_FLTCONF_MASK)
#define CAN_ESR1_TX_MASK                         0x40u
#define CAN_ESR1_TX_SHIFT                        6u
#define CAN_ESR1_TX_WIDTH                        1u
#define CAN_ESR1_TX(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TX_SHIFT))&CAN_ESR1_TX_MASK)
#define CAN_ESR1_IDLE_MASK                       0x80u
#define CAN_ESR1_IDLE_SHIFT                      7u
#define CAN_ESR1_IDLE_WIDTH                      1u
#define CAN_ESR1_IDLE(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_IDLE_SHIFT))&CAN_ESR1_IDLE_MASK)
#define CAN_ESR1_RXWRN_MASK                      0x100u
#define CAN_ESR1_RXWRN_SHIFT                     8u
#define CAN_ESR1_RXWRN_WIDTH                     1u
#define CAN_ESR1_RXWRN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RXWRN_SHIFT))&CAN_ESR1_RXWRN_MASK)
#define CAN_ESR1_TXWRN_MASK                      0x200u
#define CAN_ESR1_TXWRN_SHIFT                     9u
#define CAN_ESR1_TXWRN_WIDTH                     1u
#define CAN_ESR1_TXWRN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TXWRN_SHIFT))&CAN_ESR1_TXWRN_MASK)
#define CAN_ESR1_STFERR_MASK                     0x400u
#define CAN_ESR1_STFERR_SHIFT                    10u
#define CAN_ESR1_STFERR_WIDTH                    1u
#define CAN_ESR1_STFERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_STFERR_SHIFT))&CAN_ESR1_STFERR_MASK)
#define CAN_ESR1_FRMERR_MASK                     0x800u
#define CAN_ESR1_FRMERR_SHIFT                    11u
#define CAN_ESR1_FRMERR_WIDTH                    1u
#define CAN_ESR1_FRMERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FRMERR_SHIFT))&CAN_ESR1_FRMERR_MASK)
#define CAN_ESR1_CRCERR_MASK                     0x1000u
#define CAN_ESR1_CRCERR_SHIFT                    12u
#define CAN_ESR1_CRCERR_WIDTH                    1u
#define CAN_ESR1_CRCERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_CRCERR_SHIFT))&CAN_ESR1_CRCERR_MASK)
#define CAN_ESR1_ACKERR_MASK                     0x2000u
#define CAN_ESR1_ACKERR_SHIFT                    13u
#define CAN_ESR1_ACKERR_WIDTH                    1u
#define CAN_ESR1_ACKERR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ACKERR_SHIFT))&CAN_ESR1_ACKERR_MASK)
#define CAN_ESR1_BIT0ERR_MASK                    0x4000u
#define CAN_ESR1_BIT0ERR_SHIFT                   14u
#define CAN_ESR1_BIT0ERR_WIDTH                   1u
#define CAN_ESR1_BIT0ERR(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT0ERR_SHIFT))&CAN_ESR1_BIT0ERR_MASK)
#define CAN_ESR1_BIT1ERR_MASK                    0x8000u
#define CAN_ESR1_BIT1ERR_SHIFT                   15u
#define CAN_ESR1_BIT1ERR_WIDTH                   1u
#define CAN_ESR1_BIT1ERR(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT1ERR_SHIFT))&CAN_ESR1_BIT1ERR_MASK)
#define CAN_ESR1_RWRNINT_MASK                    0x10000u
#define CAN_ESR1_RWRNINT_SHIFT                   16u
#define CAN_ESR1_RWRNINT_WIDTH                   1u
#define CAN_ESR1_RWRNINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_RWRNINT_SHIFT))&CAN_ESR1_RWRNINT_MASK)
#define CAN_ESR1_TWRNINT_MASK                    0x20000u
#define CAN_ESR1_TWRNINT_SHIFT                   17u
#define CAN_ESR1_TWRNINT_WIDTH                   1u
#define CAN_ESR1_TWRNINT(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_TWRNINT_SHIFT))&CAN_ESR1_TWRNINT_MASK)
#define CAN_ESR1_SYNCH_MASK                      0x40000u
#define CAN_ESR1_SYNCH_SHIFT                     18u
#define CAN_ESR1_SYNCH_WIDTH                     1u
#define CAN_ESR1_SYNCH(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_SYNCH_SHIFT))&CAN_ESR1_SYNCH_MASK)
#define CAN_ESR1_BOFFDONEINT_MASK                0x80000u
#define CAN_ESR1_BOFFDONEINT_SHIFT               19u
#define CAN_ESR1_BOFFDONEINT_WIDTH               1u
#define CAN_ESR1_BOFFDONEINT(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BOFFDONEINT_SHIFT))&CAN_ESR1_BOFFDONEINT_MASK)
#define CAN_ESR1_ERRINT_FAST_MASK                0x100000u
#define CAN_ESR1_ERRINT_FAST_SHIFT               20u
#define CAN_ESR1_ERRINT_FAST_WIDTH               1u
#define CAN_ESR1_ERRINT_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERRINT_FAST_SHIFT))&CAN_ESR1_ERRINT_FAST_MASK)
#define CAN_ESR1_ERROVR_MASK                     0x200000u
#define CAN_ESR1_ERROVR_SHIFT                    21u
#define CAN_ESR1_ERROVR_WIDTH                    1u
#define CAN_ESR1_ERROVR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_ERROVR_SHIFT))&CAN_ESR1_ERROVR_MASK)
#define CAN_ESR1_STFERR_FAST_MASK                0x4000000u
#define CAN_ESR1_STFERR_FAST_SHIFT               26u
#define CAN_ESR1_STFERR_FAST_WIDTH               1u
#define CAN_ESR1_STFERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_STFERR_FAST_SHIFT))&CAN_ESR1_STFERR_FAST_MASK)
#define CAN_ESR1_FRMERR_FAST_MASK                0x8000000u
#define CAN_ESR1_FRMERR_FAST_SHIFT               27u
#define CAN_ESR1_FRMERR_FAST_WIDTH               1u
#define CAN_ESR1_FRMERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_FRMERR_FAST_SHIFT))&CAN_ESR1_FRMERR_FAST_MASK)
#define CAN_ESR1_CRCERR_FAST_MASK                0x10000000u
#define CAN_ESR1_CRCERR_FAST_SHIFT               28u
#define CAN_ESR1_CRCERR_FAST_WIDTH               1u
#define CAN_ESR1_CRCERR_FAST(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_CRCERR_FAST_SHIFT))&CAN_ESR1_CRCERR_FAST_MASK)
#define CAN_ESR1_BIT0ERR_FAST_MASK               0x40000000u
#define CAN_ESR1_BIT0ERR_FAST_SHIFT              30u
#define CAN_ESR1_BIT0ERR_FAST_WIDTH              1u
#define CAN_ESR1_BIT0ERR_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT0ERR_FAST_SHIFT))&CAN_ESR1_BIT0ERR_FAST_MASK)
#define CAN_ESR1_BIT1ERR_FAST_MASK               0x80000000u
#define CAN_ESR1_BIT1ERR_FAST_SHIFT              31u
#define CAN_ESR1_BIT1ERR_FAST_WIDTH              1u
#define CAN_ESR1_BIT1ERR_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_ESR1_BIT1ERR_FAST_SHIFT))&CAN_ESR1_BIT1ERR_FAST_MASK)
/* IMASK1 Bit Fields */
#define CAN_IMASK1_BUF31TO0M_MASK                0xFFFFFFFFu
#define CAN_IMASK1_BUF31TO0M_SHIFT               0u
#define CAN_IMASK1_BUF31TO0M_WIDTH               32u
#define CAN_IMASK1_BUF31TO0M(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_IMASK1_BUF31TO0M_SHIFT))&CAN_IMASK1_BUF31TO0M_MASK)
/* IFLAG1 Bit Fields */
#define CAN_IFLAG1_BUF0I_MASK                    0x1u
#define CAN_IFLAG1_BUF0I_SHIFT                   0u
#define CAN_IFLAG1_BUF0I_WIDTH                   1u
#define CAN_IFLAG1_BUF0I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF0I_SHIFT))&CAN_IFLAG1_BUF0I_MASK)
#define CAN_IFLAG1_BUF4TO1I_MASK                 0x1Eu
#define CAN_IFLAG1_BUF4TO1I_SHIFT                1u
#define CAN_IFLAG1_BUF4TO1I_WIDTH                4u
#define CAN_IFLAG1_BUF4TO1I(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF4TO1I_SHIFT))&CAN_IFLAG1_BUF4TO1I_MASK)
#define CAN_IFLAG1_BUF5I_MASK                    0x20u
#define CAN_IFLAG1_BUF5I_SHIFT                   5u
#define CAN_IFLAG1_BUF5I_WIDTH                   1u
#define CAN_IFLAG1_BUF5I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF5I_SHIFT))&CAN_IFLAG1_BUF5I_MASK)
#define CAN_IFLAG1_BUF6I_MASK                    0x40u
#define CAN_IFLAG1_BUF6I_SHIFT                   6u
#define CAN_IFLAG1_BUF6I_WIDTH                   1u
#define CAN_IFLAG1_BUF6I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF6I_SHIFT))&CAN_IFLAG1_BUF6I_MASK)
#define CAN_IFLAG1_BUF7I_MASK                    0x80u
#define CAN_IFLAG1_BUF7I_SHIFT                   7u
#define CAN_IFLAG1_BUF7I_WIDTH                   1u
#define CAN_IFLAG1_BUF7I(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF7I_SHIFT))&CAN_IFLAG1_BUF7I_MASK)
#define CAN_IFLAG1_BUF31TO8I_MASK                0xFFFFFF00u
#define CAN_IFLAG1_BUF31TO8I_SHIFT               8u
#define CAN_IFLAG1_BUF31TO8I_WIDTH               24u
#define CAN_IFLAG1_BUF31TO8I(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_IFLAG1_BUF31TO8I_SHIFT))&CAN_IFLAG1_BUF31TO8I_MASK)
/* CTRL2 Bit Fields */
#define CAN_CTRL2_EDFLTDIS_MASK                  0x800u
#define CAN_CTRL2_EDFLTDIS_SHIFT                 11u
#define CAN_CTRL2_EDFLTDIS_WIDTH                 1u
#define CAN_CTRL2_EDFLTDIS(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_EDFLTDIS_SHIFT))&CAN_CTRL2_EDFLTDIS_MASK)
#define CAN_CTRL2_ISOCANFDEN_MASK                0x1000u
#define CAN_CTRL2_ISOCANFDEN_SHIFT               12u
#define CAN_CTRL2_ISOCANFDEN_WIDTH               1u
#define CAN_CTRL2_ISOCANFDEN(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_ISOCANFDEN_SHIFT))&CAN_CTRL2_ISOCANFDEN_MASK)
#define CAN_CTRL2_PREXCEN_MASK                   0x4000u
#define CAN_CTRL2_PREXCEN_SHIFT                  14u
#define CAN_CTRL2_PREXCEN_WIDTH                  1u
#define CAN_CTRL2_PREXCEN(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_PREXCEN_SHIFT))&CAN_CTRL2_PREXCEN_MASK)
#define CAN_CTRL2_TIMER_SRC_MASK                 0x8000u
#define CAN_CTRL2_TIMER_SRC_SHIFT                15u
#define CAN_CTRL2_TIMER_SRC_WIDTH                1u
#define CAN_CTRL2_TIMER_SRC(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_TIMER_SRC_SHIFT))&CAN_CTRL2_TIMER_SRC_MASK)
#define CAN_CTRL2_EACEN_MASK                     0x10000u
#define CAN_CTRL2_EACEN_SHIFT                    16u
#define CAN_CTRL2_EACEN_WIDTH                    1u
#define CAN_CTRL2_EACEN(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_EACEN_SHIFT))&CAN_CTRL2_EACEN_MASK)
#define CAN_CTRL2_RRS_MASK                       0x20000u
#define CAN_CTRL2_RRS_SHIFT                      17u
#define CAN_CTRL2_RRS_WIDTH                      1u
#define CAN_CTRL2_RRS(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_RRS_SHIFT))&CAN_CTRL2_RRS_MASK)
#define CAN_CTRL2_MRP_MASK                       0x40000u
#define CAN_CTRL2_MRP_SHIFT                      18u
#define CAN_CTRL2_MRP_WIDTH                      1u
#define CAN_CTRL2_MRP(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_MRP_SHIFT))&CAN_CTRL2_MRP_MASK)
#define CAN_CTRL2_TASD_MASK                      0xF80000u
#define CAN_CTRL2_TASD_SHIFT                     19u
#define CAN_CTRL2_TASD_WIDTH                     5u
#define CAN_CTRL2_TASD(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_TASD_SHIFT))&CAN_CTRL2_TASD_MASK)
#define CAN_CTRL2_RFFN_MASK                      0xF000000u
#define CAN_CTRL2_RFFN_SHIFT                     24u
#define CAN_CTRL2_RFFN_WIDTH                     4u
#define CAN_CTRL2_RFFN(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_RFFN_SHIFT))&CAN_CTRL2_RFFN_MASK)
#define CAN_CTRL2_BOFFDONEMSK_MASK               0x40000000u
#define CAN_CTRL2_BOFFDONEMSK_SHIFT              30u
#define CAN_CTRL2_BOFFDONEMSK_WIDTH              1u
#define CAN_CTRL2_BOFFDONEMSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_BOFFDONEMSK_SHIFT))&CAN_CTRL2_BOFFDONEMSK_MASK)
#define CAN_CTRL2_ERRMSK_FAST_MASK               0x80000000u
#define CAN_CTRL2_ERRMSK_FAST_SHIFT              31u
#define CAN_CTRL2_ERRMSK_FAST_WIDTH              1u
#define CAN_CTRL2_ERRMSK_FAST(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_ERRMSK_FAST_SHIFT))&CAN_CTRL2_ERRMSK_FAST_MASK)
/* ESR2 Bit Fields */
#define CAN_ESR2_IMB_MASK                        0x2000u
#define CAN_ESR2_IMB_SHIFT                       13u
#define CAN_ESR2_IMB_WIDTH                       1u
#define CAN_ESR2_IMB(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_IMB_SHIFT))&CAN_ESR2_IMB_MASK)
#define CAN_ESR2_VPS_MASK                        0x4000u
#define CAN_ESR2_VPS_SHIFT                       14u
#define CAN_ESR2_VPS_WIDTH                       1u
#define CAN_ESR2_VPS(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_VPS_SHIFT))&CAN_ESR2_VPS_MASK)
#define CAN_ESR2_LPTM_MASK                       0x7F0000u
#define CAN_ESR2_LPTM_SHIFT                      16u
#define CAN_ESR2_LPTM_WIDTH                      7u
#define CAN_ESR2_LPTM(x)                         (((uint32_t)(((uint32_t)(x))<<CAN_ESR2_LPTM_SHIFT))&CAN_ESR2_LPTM_MASK)
/* CRCR Bit Fields */
#define CAN_CRCR_TXCRC_MASK                      0x7FFFu
#define CAN_CRCR_TXCRC_SHIFT                     0u
#define CAN_CRCR_TXCRC_WIDTH                     15u
#define CAN_CRCR_TXCRC(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CRCR_TXCRC_SHIFT))&CAN_CRCR_TXCRC_MASK)
#define CAN_CRCR_MBCRC_MASK                      0x7F0000u
#define CAN_CRCR_MBCRC_SHIFT                     16u
#define CAN_CRCR_MBCRC_WIDTH                     7u
#define CAN_CRCR_MBCRC(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CRCR_MBCRC_SHIFT))&CAN_CRCR_MBCRC_MASK)
/* RXFGMASK Bit Fields */
#define CAN_RXFGMASK_FGM_MASK                    0xFFFFFFFFu
#define CAN_RXFGMASK_FGM_SHIFT                   0u
#define CAN_RXFGMASK_FGM_WIDTH                   32u
#define CAN_RXFGMASK_FGM(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_RXFGMASK_FGM_SHIFT))&CAN_RXFGMASK_FGM_MASK)
/* RXFIR Bit Fields */
#define CAN_RXFIR_IDHIT_MASK                     0x1FFu
#define CAN_RXFIR_IDHIT_SHIFT                    0u
#define CAN_RXFIR_IDHIT_WIDTH                    9u
#define CAN_RXFIR_IDHIT(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_RXFIR_IDHIT_SHIFT))&CAN_RXFIR_IDHIT_MASK)
/* CBT Bit Fields */
#define CAN_CBT_EPSEG2_MASK                      0x1Fu
#define CAN_CBT_EPSEG2_SHIFT                     0u
#define CAN_CBT_EPSEG2_WIDTH                     5u
#define CAN_CBT_EPSEG2(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPSEG2_SHIFT))&CAN_CBT_EPSEG2_MASK)
#define CAN_CBT_EPSEG1_MASK                      0x3E0u
#define CAN_CBT_EPSEG1_SHIFT                     5u
#define CAN_CBT_EPSEG1_WIDTH                     5u
#define CAN_CBT_EPSEG1(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPSEG1_SHIFT))&CAN_CBT_EPSEG1_MASK)
#define CAN_CBT_EPROPSEG_MASK                    0xFC00u
#define CAN_CBT_EPROPSEG_SHIFT                   10u
#define CAN_CBT_EPROPSEG_WIDTH                   6u
#define CAN_CBT_EPROPSEG(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPROPSEG_SHIFT))&CAN_CBT_EPROPSEG_MASK)
#define CAN_CBT_ERJW_MASK                        0x1F0000u
#define CAN_CBT_ERJW_SHIFT                       16u
#define CAN_CBT_ERJW_WIDTH                       5u
#define CAN_CBT_ERJW(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_CBT_ERJW_SHIFT))&CAN_CBT_ERJW_MASK)
#define CAN_CBT_EPRESDIV_MASK                    0x7FE00000u
#define CAN_CBT_EPRESDIV_SHIFT                   21u
#define CAN_CBT_EPRESDIV_WIDTH                   10u
#define CAN_CBT_EPRESDIV(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CBT_EPRESDIV_SHIFT))&CAN_CBT_EPRESDIV_MASK)
#define CAN_CBT_BTF_MASK                         0x80000000u
#define CAN_CBT_BTF_SHIFT                        31u
#define CAN_CBT_BTF_WIDTH                        1u
#define CAN_CBT_BTF(x)                           (((uint32_t)(((uint32_t)(x))<<CAN_CBT_BTF_SHIFT))&CAN_CBT_BTF_MASK)
/* RAMn Bit Fields */
#define CAN_RAMn_DATA_BYTE_3_MASK                0xFFu
#define CAN_RAMn_DATA_BYTE_3_SHIFT               0u
#define CAN_RAMn_DATA_BYTE_3_WIDTH               8u
#define CAN_RAMn_DATA_BYTE_3(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_3_SHIFT))&CAN_RAMn_DATA_BYTE_3_MASK)
#define CAN_RAMn_DATA_BYTE_2_MASK                0xFF00u
#define CAN_RAMn_DATA_BYTE_2_SHIFT               8u
#define CAN_RAMn_DATA_BYTE_2_WIDTH               8u
#define CAN_RAMn_DATA_BYTE_2(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_2_SHIFT))&CAN_RAMn_DATA_BYTE_2_MASK)
#define CAN_RAMn_DATA_BYTE_1_MASK                0xFF0000u
#define CAN_RAMn_DATA_BYTE_1_SHIFT               16u
#define CAN_RAMn_DATA_BYTE_1_WIDTH               8u
#define CAN_RAMn_DATA_BYTE_1(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_1_SHIFT))&CAN_RAMn_DATA_BYTE_1_MASK)
#define CAN_RAMn_DATA_BYTE_0_MASK                0xFF000000u
#define CAN_RAMn_DATA_BYTE_0_SHIFT               24u
#define CAN_RAMn_DATA_BYTE_0_WIDTH               8u
#define CAN_RAMn_DATA_BYTE_0(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_RAMn_DATA_BYTE_0_SHIFT))&CAN_RAMn_DATA_BYTE_0_MASK)
/* RXIMR Bit Fields */
#define CAN_RXIMR_MI_MASK                        0xFFFFFFFFu
#define CAN_RXIMR_MI_SHIFT                       0u
#define CAN_RXIMR_MI_WIDTH                       32u
#define CAN_RXIMR_MI(x)                          (((uint32_t)(((uint32_t)(x))<<CAN_RXIMR_MI_SHIFT))&CAN_RXIMR_MI_MASK)
/* CTRL1_PN Bit Fields */
#define CAN_CTRL1_PN_FCS_MASK                    0x3u
#define CAN_CTRL1_PN_FCS_SHIFT                   0u
#define CAN_CTRL1_PN_FCS_WIDTH                   2u
#define CAN_CTRL1_PN_FCS(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_FCS_SHIFT))&CAN_CTRL1_PN_FCS_MASK)
#define CAN_CTRL1_PN_IDFS_MASK                   0xCu
#define CAN_CTRL1_PN_IDFS_SHIFT                  2u
#define CAN_CTRL1_PN_IDFS_WIDTH                  2u
#define CAN_CTRL1_PN_IDFS(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_IDFS_SHIFT))&CAN_CTRL1_PN_IDFS_MASK)
#define CAN_CTRL1_PN_PLFS_MASK                   0x30u
#define CAN_CTRL1_PN_PLFS_SHIFT                  4u
#define CAN_CTRL1_PN_PLFS_WIDTH                  2u
#define CAN_CTRL1_PN_PLFS(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_PLFS_SHIFT))&CAN_CTRL1_PN_PLFS_MASK)
#define CAN_CTRL1_PN_NMATCH_MASK                 0xFF00u
#define CAN_CTRL1_PN_NMATCH_SHIFT                8u
#define CAN_CTRL1_PN_NMATCH_WIDTH                8u
#define CAN_CTRL1_PN_NMATCH(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_NMATCH_SHIFT))&CAN_CTRL1_PN_NMATCH_MASK)
#define CAN_CTRL1_PN_WUMF_MSK_MASK               0x10000u
#define CAN_CTRL1_PN_WUMF_MSK_SHIFT              16u
#define CAN_CTRL1_PN_WUMF_MSK_WIDTH              1u
#define CAN_CTRL1_PN_WUMF_MSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_WUMF_MSK_SHIFT))&CAN_CTRL1_PN_WUMF_MSK_MASK)
#define CAN_CTRL1_PN_WTOF_MSK_MASK               0x20000u
#define CAN_CTRL1_PN_WTOF_MSK_SHIFT              17u
#define CAN_CTRL1_PN_WTOF_MSK_WIDTH              1u
#define CAN_CTRL1_PN_WTOF_MSK(x)                 (((uint32_t)(((uint32_t)(x))<<CAN_CTRL1_PN_WTOF_MSK_SHIFT))&CAN_CTRL1_PN_WTOF_MSK_MASK)
/* CTRL2_PN Bit Fields */
#define CAN_CTRL2_PN_MATCHTO_MASK                0xFFFFu
#define CAN_CTRL2_PN_MATCHTO_SHIFT               0u
#define CAN_CTRL2_PN_MATCHTO_WIDTH               16u
#define CAN_CTRL2_PN_MATCHTO(x)                  (((uint32_t)(((uint32_t)(x))<<CAN_CTRL2_PN_MATCHTO_SHIFT))&CAN_CTRL2_PN_MATCHTO_MASK)
/* WU_MTC Bit Fields */
#define CAN_WU_MTC_MCOUNTER_MASK                 0xFF00u
#define CAN_WU_MTC_MCOUNTER_SHIFT                8u
#define CAN_WU_MTC_MCOUNTER_WIDTH                8u
#define CAN_WU_MTC_MCOUNTER(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_MCOUNTER_SHIFT))&CAN_WU_MTC_MCOUNTER_MASK)
#define CAN_WU_MTC_WUMF_MASK                     0x10000u
#define CAN_WU_MTC_WUMF_SHIFT                    16u
#define CAN_WU_MTC_WUMF_WIDTH                    1u
#define CAN_WU_MTC_WUMF(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_WUMF_SHIFT))&CAN_WU_MTC_WUMF_MASK)
#define CAN_WU_MTC_WTOF_MASK                     0x20000u
#define CAN_WU_MTC_WTOF_SHIFT                    17u
#define CAN_WU_MTC_WTOF_WIDTH                    1u
#define CAN_WU_MTC_WTOF(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WU_MTC_WTOF_SHIFT))&CAN_WU_MTC_WTOF_MASK)
/* FLT_ID1 Bit Fields */
#define CAN_FLT_ID1_FLT_ID1_MASK                 0x1FFFFFFFu
#define CAN_FLT_ID1_FLT_ID1_SHIFT                0u
#define CAN_FLT_ID1_FLT_ID1_WIDTH                29u
#define CAN_FLT_ID1_FLT_ID1(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_ID1_SHIFT))&CAN_FLT_ID1_FLT_ID1_MASK)
#define CAN_FLT_ID1_FLT_RTR_MASK                 0x20000000u
#define CAN_FLT_ID1_FLT_RTR_SHIFT                29u
#define CAN_FLT_ID1_FLT_RTR_WIDTH                1u
#define CAN_FLT_ID1_FLT_RTR(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_RTR_SHIFT))&CAN_FLT_ID1_FLT_RTR_MASK)
#define CAN_FLT_ID1_FLT_IDE_MASK                 0x40000000u
#define CAN_FLT_ID1_FLT_IDE_SHIFT                30u
#define CAN_FLT_ID1_FLT_IDE_WIDTH                1u
#define CAN_FLT_ID1_FLT_IDE(x)                   (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID1_FLT_IDE_SHIFT))&CAN_FLT_ID1_FLT_IDE_MASK)
/* FLT_DLC Bit Fields */
#define CAN_FLT_DLC_FLT_DLC_HI_MASK              0xFu
#define CAN_FLT_DLC_FLT_DLC_HI_SHIFT             0u
#define CAN_FLT_DLC_FLT_DLC_HI_WIDTH             4u
#define CAN_FLT_DLC_FLT_DLC_HI(x)                (((uint32_t)(((uint32_t)(x))<<CAN_FLT_DLC_FLT_DLC_HI_SHIFT))&CAN_FLT_DLC_FLT_DLC_HI_MASK)
#define CAN_FLT_DLC_FLT_DLC_LO_MASK              0xF0000u
#define CAN_FLT_DLC_FLT_DLC_LO_SHIFT             16u
#define CAN_FLT_DLC_FLT_DLC_LO_WIDTH             4u
#define CAN_FLT_DLC_FLT_DLC_LO(x)                (((uint32_t)(((uint32_t)(x))<<CAN_FLT_DLC_FLT_DLC_LO_SHIFT))&CAN_FLT_DLC_FLT_DLC_LO_MASK)
/* PL1_LO Bit Fields */
#define CAN_PL1_LO_Data_byte_3_MASK              0xFFu
#define CAN_PL1_LO_Data_byte_3_SHIFT             0u
#define CAN_PL1_LO_Data_byte_3_WIDTH             8u
#define CAN_PL1_LO_Data_byte_3(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_3_SHIFT))&CAN_PL1_LO_Data_byte_3_MASK)
#define CAN_PL1_LO_Data_byte_2_MASK              0xFF00u
#define CAN_PL1_LO_Data_byte_2_SHIFT             8u
#define CAN_PL1_LO_Data_byte_2_WIDTH             8u
#define CAN_PL1_LO_Data_byte_2(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_2_SHIFT))&CAN_PL1_LO_Data_byte_2_MASK)
#define CAN_PL1_LO_Data_byte_1_MASK              0xFF0000u
#define CAN_PL1_LO_Data_byte_1_SHIFT             16u
#define CAN_PL1_LO_Data_byte_1_WIDTH             8u
#define CAN_PL1_LO_Data_byte_1(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_1_SHIFT))&CAN_PL1_LO_Data_byte_1_MASK)
#define CAN_PL1_LO_Data_byte_0_MASK              0xFF000000u
#define CAN_PL1_LO_Data_byte_0_SHIFT             24u
#define CAN_PL1_LO_Data_byte_0_WIDTH             8u
#define CAN_PL1_LO_Data_byte_0(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_LO_Data_byte_0_SHIFT))&CAN_PL1_LO_Data_byte_0_MASK)
/* PL1_HI Bit Fields */
#define CAN_PL1_HI_Data_byte_7_MASK              0xFFu
#define CAN_PL1_HI_Data_byte_7_SHIFT             0u
#define CAN_PL1_HI_Data_byte_7_WIDTH             8u
#define CAN_PL1_HI_Data_byte_7(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_7_SHIFT))&CAN_PL1_HI_Data_byte_7_MASK)
#define CAN_PL1_HI_Data_byte_6_MASK              0xFF00u
#define CAN_PL1_HI_Data_byte_6_SHIFT             8u
#define CAN_PL1_HI_Data_byte_6_WIDTH             8u
#define CAN_PL1_HI_Data_byte_6(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_6_SHIFT))&CAN_PL1_HI_Data_byte_6_MASK)
#define CAN_PL1_HI_Data_byte_5_MASK              0xFF0000u
#define CAN_PL1_HI_Data_byte_5_SHIFT             16u
#define CAN_PL1_HI_Data_byte_5_WIDTH             8u
#define CAN_PL1_HI_Data_byte_5(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_5_SHIFT))&CAN_PL1_HI_Data_byte_5_MASK)
#define CAN_PL1_HI_Data_byte_4_MASK              0xFF000000u
#define CAN_PL1_HI_Data_byte_4_SHIFT             24u
#define CAN_PL1_HI_Data_byte_4_WIDTH             8u
#define CAN_PL1_HI_Data_byte_4(x)                (((uint32_t)(((uint32_t)(x))<<CAN_PL1_HI_Data_byte_4_SHIFT))&CAN_PL1_HI_Data_byte_4_MASK)
/* FLT_ID2_IDMASK Bit Fields */
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK   0x1FFFFFFFu
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT  0u
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_WIDTH  29u
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(x)     (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT))&CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK)
#define CAN_FLT_ID2_IDMASK_RTR_MSK_MASK          0x20000000u
#define CAN_FLT_ID2_IDMASK_RTR_MSK_SHIFT         29u
#define CAN_FLT_ID2_IDMASK_RTR_MSK_WIDTH         1u
#define CAN_FLT_ID2_IDMASK_RTR_MSK(x)            (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_RTR_MSK_SHIFT))&CAN_FLT_ID2_IDMASK_RTR_MSK_MASK)
#define CAN_FLT_ID2_IDMASK_IDE_MSK_MASK          0x40000000u
#define CAN_FLT_ID2_IDMASK_IDE_MSK_SHIFT         30u
#define CAN_FLT_ID2_IDMASK_IDE_MSK_WIDTH         1u
#define CAN_FLT_ID2_IDMASK_IDE_MSK(x)            (((uint32_t)(((uint32_t)(x))<<CAN_FLT_ID2_IDMASK_IDE_MSK_SHIFT))&CAN_FLT_ID2_IDMASK_IDE_MSK_MASK)
/* PL2_PLMASK_LO Bit Fields */
#define CAN_PL2_PLMASK_LO_Data_byte_3_MASK       0xFFu
#define CAN_PL2_PLMASK_LO_Data_byte_3_SHIFT      0u
#define CAN_PL2_PLMASK_LO_Data_byte_3_WIDTH      8u
#define CAN_PL2_PLMASK_LO_Data_byte_3(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_3_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_3_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_2_MASK       0xFF00u
#define CAN_PL2_PLMASK_LO_Data_byte_2_SHIFT      8u
#define CAN_PL2_PLMASK_LO_Data_byte_2_WIDTH      8u
#define CAN_PL2_PLMASK_LO_Data_byte_2(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_2_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_2_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_1_MASK       0xFF0000u
#define CAN_PL2_PLMASK_LO_Data_byte_1_SHIFT      16u
#define CAN_PL2_PLMASK_LO_Data_byte_1_WIDTH      8u
#define CAN_PL2_PLMASK_LO_Data_byte_1(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_1_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_1_MASK)
#define CAN_PL2_PLMASK_LO_Data_byte_0_MASK       0xFF000000u
#define CAN_PL2_PLMASK_LO_Data_byte_0_SHIFT      24u
#define CAN_PL2_PLMASK_LO_Data_byte_0_WIDTH      8u
#define CAN_PL2_PLMASK_LO_Data_byte_0(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_LO_Data_byte_0_SHIFT))&CAN_PL2_PLMASK_LO_Data_byte_0_MASK)
/* PL2_PLMASK_HI Bit Fields */
#define CAN_PL2_PLMASK_HI_Data_byte_7_MASK       0xFFu
#define CAN_PL2_PLMASK_HI_Data_byte_7_SHIFT      0u
#define CAN_PL2_PLMASK_HI_Data_byte_7_WIDTH      8u
#define CAN_PL2_PLMASK_HI_Data_byte_7(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_7_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_7_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_6_MASK       0xFF00u
#define CAN_PL2_PLMASK_HI_Data_byte_6_SHIFT      8u
#define CAN_PL2_PLMASK_HI_Data_byte_6_WIDTH      8u
#define CAN_PL2_PLMASK_HI_Data_byte_6(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_6_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_6_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_5_MASK       0xFF0000u
#define CAN_PL2_PLMASK_HI_Data_byte_5_SHIFT      16u
#define CAN_PL2_PLMASK_HI_Data_byte_5_WIDTH      8u
#define CAN_PL2_PLMASK_HI_Data_byte_5(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_5_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_5_MASK)
#define CAN_PL2_PLMASK_HI_Data_byte_4_MASK       0xFF000000u
#define CAN_PL2_PLMASK_HI_Data_byte_4_SHIFT      24u
#define CAN_PL2_PLMASK_HI_Data_byte_4_WIDTH      8u
#define CAN_PL2_PLMASK_HI_Data_byte_4(x)         (((uint32_t)(((uint32_t)(x))<<CAN_PL2_PLMASK_HI_Data_byte_4_SHIFT))&CAN_PL2_PLMASK_HI_Data_byte_4_MASK)
/* WMBn_CS Bit Fields */
#define CAN_WMBn_CS_DLC_MASK                     0xF0000u
#define CAN_WMBn_CS_DLC_SHIFT                    16u
#define CAN_WMBn_CS_DLC_WIDTH                    4u
#define CAN_WMBn_CS_DLC(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_DLC_SHIFT))&CAN_WMBn_CS_DLC_MASK)
#define CAN_WMBn_CS_RTR_MASK                     0x100000u
#define CAN_WMBn_CS_RTR_SHIFT                    20u
#define CAN_WMBn_CS_RTR_WIDTH                    1u
#define CAN_WMBn_CS_RTR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_RTR_SHIFT))&CAN_WMBn_CS_RTR_MASK)
#define CAN_WMBn_CS_IDE_MASK                     0x200000u
#define CAN_WMBn_CS_IDE_SHIFT                    21u
#define CAN_WMBn_CS_IDE_WIDTH                    1u
#define CAN_WMBn_CS_IDE(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_IDE_SHIFT))&CAN_WMBn_CS_IDE_MASK)
#define CAN_WMBn_CS_SRR_MASK                     0x400000u
#define CAN_WMBn_CS_SRR_SHIFT                    22u
#define CAN_WMBn_CS_SRR_WIDTH                    1u
#define CAN_WMBn_CS_SRR(x)                       (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_CS_SRR_SHIFT))&CAN_WMBn_CS_SRR_MASK)
/* WMBn_ID Bit Fields */
#define CAN_WMBn_ID_ID_MASK                      0x1FFFFFFFu
#define CAN_WMBn_ID_ID_SHIFT                     0u
#define CAN_WMBn_ID_ID_WIDTH                     29u
#define CAN_WMBn_ID_ID(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_ID_ID_SHIFT))&CAN_WMBn_ID_ID_MASK)
/* WMBn_D03 Bit Fields */
#define CAN_WMBn_D03_Data_byte_3_MASK            0xFFu
#define CAN_WMBn_D03_Data_byte_3_SHIFT           0u
#define CAN_WMBn_D03_Data_byte_3_WIDTH           8u
#define CAN_WMBn_D03_Data_byte_3(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_3_SHIFT))&CAN_WMBn_D03_Data_byte_3_MASK)
#define CAN_WMBn_D03_Data_byte_2_MASK            0xFF00u
#define CAN_WMBn_D03_Data_byte_2_SHIFT           8u
#define CAN_WMBn_D03_Data_byte_2_WIDTH           8u
#define CAN_WMBn_D03_Data_byte_2(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_2_SHIFT))&CAN_WMBn_D03_Data_byte_2_MASK)
#define CAN_WMBn_D03_Data_byte_1_MASK            0xFF0000u
#define CAN_WMBn_D03_Data_byte_1_SHIFT           16u
#define CAN_WMBn_D03_Data_byte_1_WIDTH           8u
#define CAN_WMBn_D03_Data_byte_1(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_1_SHIFT))&CAN_WMBn_D03_Data_byte_1_MASK)
#define CAN_WMBn_D03_Data_byte_0_MASK            0xFF000000u
#define CAN_WMBn_D03_Data_byte_0_SHIFT           24u
#define CAN_WMBn_D03_Data_byte_0_WIDTH           8u
#define CAN_WMBn_D03_Data_byte_0(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D03_Data_byte_0_SHIFT))&CAN_WMBn_D03_Data_byte_0_MASK)
/* WMBn_D47 Bit Fields */
#define CAN_WMBn_D47_Data_byte_7_MASK            0xFFu
#define CAN_WMBn_D47_Data_byte_7_SHIFT           0u
#define CAN_WMBn_D47_Data_byte_7_WIDTH           8u
#define CAN_WMBn_D47_Data_byte_7(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_7_SHIFT))&CAN_WMBn_D47_Data_byte_7_MASK)
#define CAN_WMBn_D47_Data_byte_6_MASK            0xFF00u
#define CAN_WMBn_D47_Data_byte_6_SHIFT           8u
#define CAN_WMBn_D47_Data_byte_6_WIDTH           8u
#define CAN_WMBn_D47_Data_byte_6(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_6_SHIFT))&CAN_WMBn_D47_Data_byte_6_MASK)
#define CAN_WMBn_D47_Data_byte_5_MASK            0xFF0000u
#define CAN_WMBn_D47_Data_byte_5_SHIFT           16u
#define CAN_WMBn_D47_Data_byte_5_WIDTH           8u
#define CAN_WMBn_D47_Data_byte_5(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_5_SHIFT))&CAN_WMBn_D47_Data_byte_5_MASK)
#define CAN_WMBn_D47_Data_byte_4_MASK            0xFF000000u
#define CAN_WMBn_D47_Data_byte_4_SHIFT           24u
#define CAN_WMBn_D47_Data_byte_4_WIDTH           8u
#define CAN_WMBn_D47_Data_byte_4(x)              (((uint32_t)(((uint32_t)(x))<<CAN_WMBn_D47_Data_byte_4_SHIFT))&CAN_WMBn_D47_Data_byte_4_MASK)
/* FDCTRL Bit Fields */
#define CAN_FDCTRL_TDCVAL_MASK                   0x3Fu
#define CAN_FDCTRL_TDCVAL_SHIFT                  0u
#define CAN_FDCTRL_TDCVAL_WIDTH                  6u
#define CAN_FDCTRL_TDCVAL(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCVAL_SHIFT))&CAN_FDCTRL_TDCVAL_MASK)
#define CAN_FDCTRL_TDCOFF_MASK                   0x1F00u
#define CAN_FDCTRL_TDCOFF_SHIFT                  8u
#define CAN_FDCTRL_TDCOFF_WIDTH                  5u
#define CAN_FDCTRL_TDCOFF(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCOFF_SHIFT))&CAN_FDCTRL_TDCOFF_MASK)
#define CAN_FDCTRL_TDCFAIL_MASK                  0x4000u
#define CAN_FDCTRL_TDCFAIL_SHIFT                 14u
#define CAN_FDCTRL_TDCFAIL_WIDTH                 1u
#define CAN_FDCTRL_TDCFAIL(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCFAIL_SHIFT))&CAN_FDCTRL_TDCFAIL_MASK)
#define CAN_FDCTRL_TDCEN_MASK                    0x8000u
#define CAN_FDCTRL_TDCEN_SHIFT                   15u
#define CAN_FDCTRL_TDCEN_WIDTH                   1u
#define CAN_FDCTRL_TDCEN(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_TDCEN_SHIFT))&CAN_FDCTRL_TDCEN_MASK)
#define CAN_FDCTRL_MBDSR0_MASK                   0x30000u
#define CAN_FDCTRL_MBDSR0_SHIFT                  16u
#define CAN_FDCTRL_MBDSR0_WIDTH                  2u
#define CAN_FDCTRL_MBDSR0(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_MBDSR0_SHIFT))&CAN_FDCTRL_MBDSR0_MASK)
#define CAN_FDCTRL_FDRATE_MASK                   0x80000000u
#define CAN_FDCTRL_FDRATE_SHIFT                  31u
#define CAN_FDCTRL_FDRATE_WIDTH                  1u
#define CAN_FDCTRL_FDRATE(x)                     (((uint32_t)(((uint32_t)(x))<<CAN_FDCTRL_FDRATE_SHIFT))&CAN_FDCTRL_FDRATE_MASK)
/* FDCBT Bit Fields */
#define CAN_FDCBT_FPSEG2_MASK                    0x7u
#define CAN_FDCBT_FPSEG2_SHIFT                   0u
#define CAN_FDCBT_FPSEG2_WIDTH                   3u
#define CAN_FDCBT_FPSEG2(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPSEG2_SHIFT))&CAN_FDCBT_FPSEG2_MASK)
#define CAN_FDCBT_FPSEG1_MASK                    0xE0u
#define CAN_FDCBT_FPSEG1_SHIFT                   5u
#define CAN_FDCBT_FPSEG1_WIDTH                   3u
#define CAN_FDCBT_FPSEG1(x)                      (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPSEG1_SHIFT))&CAN_FDCBT_FPSEG1_MASK)
#define CAN_FDCBT_FPROPSEG_MASK                  0x7C00u
#define CAN_FDCBT_FPROPSEG_SHIFT                 10u
#define CAN_FDCBT_FPROPSEG_WIDTH                 5u
#define CAN_FDCBT_FPROPSEG(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPROPSEG_SHIFT))&CAN_FDCBT_FPROPSEG_MASK)
#define CAN_FDCBT_FRJW_MASK                      0x70000u
#define CAN_FDCBT_FRJW_SHIFT                     16u
#define CAN_FDCBT_FRJW_WIDTH                     3u
#define CAN_FDCBT_FRJW(x)                        (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FRJW_SHIFT))&CAN_FDCBT_FRJW_MASK)
#define CAN_FDCBT_FPRESDIV_MASK                  0x3FF00000u
#define CAN_FDCBT_FPRESDIV_SHIFT                 20u
#define CAN_FDCBT_FPRESDIV_WIDTH                 10u
#define CAN_FDCBT_FPRESDIV(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCBT_FPRESDIV_SHIFT))&CAN_FDCBT_FPRESDIV_MASK)
/* FDCRC Bit Fields */
#define CAN_FDCRC_FD_TXCRC_MASK                  0x1FFFFFu
#define CAN_FDCRC_FD_TXCRC_SHIFT                 0u
#define CAN_FDCRC_FD_TXCRC_WIDTH                 21u
#define CAN_FDCRC_FD_TXCRC(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCRC_FD_TXCRC_SHIFT))&CAN_FDCRC_FD_TXCRC_MASK)
#define CAN_FDCRC_FD_MBCRC_MASK                  0x7F000000u
#define CAN_FDCRC_FD_MBCRC_SHIFT                 24u
#define CAN_FDCRC_FD_MBCRC_WIDTH                 7u
#define CAN_FDCRC_FD_MBCRC(x)                    (((uint32_t)(((uint32_t)(x))<<CAN_FDCRC_FD_MBCRC_SHIFT))&CAN_FDCRC_FD_MBCRC_MASK)

/* CAN module features */
/* @brief Frames available in Rx FIFO flag shift */
#define FEATURE_CAN_RXFIFO_FRAME_AVAILABLE  (5U)
/* @brief Rx FIFO warning flag shift */
#define FEATURE_CAN_RXFIFO_WARNING          (6U)
/* @brief Rx FIFO overflow flag shift */
#define FEATURE_CAN_RXFIFO_OVERFLOW         (7U)
/* @brief The list contains definitions of the FD feature support on all instances */
#define FEATURE_CAN_INSTANCES_HAS_FD        {true, true}
/* @brief Has Flexible Data Rate for CAN0 */
#define FEATURE_CAN0_HAS_FD                 (1)
/* @brief Maximum number of Message Buffers supported for payload size 8 for CAN0 */
#define FEATURE_CAN0_MAX_MB_NUM             (32U)
/* @brief Has PE clock source select (bit field CAN_CTRL1[CLKSRC]). */
#define FEATURE_CAN_HAS_PE_CLKSRC_SELECT    (1)
/* @brief Has DMA enable (bit field MCR[DMA]). */
#define FEATURE_CAN_HAS_DMA_Enable          (0)
/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the CAN instances */
#define FEATURE_CAN_MAX_MB_NUM              (32U)
/* @brief Maximum number of Message Buffers supported for payload size 8 for any of the CAN instances */
#define FEATURE_CAN_MAX_MB_NUM_ARRAY        { FEATURE_CAN0_MAX_MB_NUM, FEATURE_CAN0_MAX_MB_NUM}
/* @brief Has Pretending Networking mode */
#define FEATURE_CAN_HAS_PRETENDED_NETWORKING    (1)
/* @brief Has Stuff Bit Count Enable Bit */
#define FEATURE_CAN_HAS_STFCNTEN_ENABLE         (0)
/* @brief Has ISO CAN FD Enable Bit */
#define FEATURE_CAN_HAS_ISOCANFDEN_ENABLE       (1)
/* @brief Has Message Buffer Data Size Region 1 */
#define FEATURE_CAN_HAS_MBDSR1                  (0)
/* @brief Has Message Buffer Data Size Region 2 */
#define FEATURE_CAN_HAS_MBDSR2                  (0)
/* @brief DMA hardware requests for all FlexCAN instances */
//#define FEATURE_CAN_EDMA_REQUESTS              { EDMA_REQ_FLEXCAN0 }

/* @brief Maximum number of Message Buffers IRQs */
#define FEATURE_CAN_MB_IRQS_MAX_COUNT       (2U)
/* @brief Message Buffers IRQs */
#define FEATURE_CAN_MB_IRQS                 { CAN_ORed_0_15_MB_IRQS, \
                                              CAN_ORed_16_31_MB_IRQS }

/* @brief Has Wake Up Irq channels (CAN_Wake_Up_IRQS_CH_COUNT > 0u) */
#define FEATURE_CAN_HAS_WAKE_UP_IRQ         (1)
/* @brief Has Self Wake Up mode */
#define FEATURE_CAN_HAS_SELF_WAKE_UP        (0)
/* @brief Has Flexible Data Rate */
#define FEATURE_CAN_HAS_FD                  (1)
/* @brief Clock name for the PE oscillator clock source */
//#define FEATURE_CAN_PE_OSC_CLK_NAME         SOSC_CLK
/* @bried FlexCAN has Detection And Correction of Memory Errors */
#define FEATURE_CAN_HAS_MEM_ERR_DET             (1)

/*---------------------- CAN END ---------------------------------*/

/*---------------------- I2C ---------------------------------*/
typedef struct {
    __IO uint32_t CON;                              /* (0x00) I2C Control Register                                  */
    __IO uint32_t TAR;                              /* (0x04) I2C Target Address Register                           */
    __IO uint32_t SAR;                              /* (0x08) I2C Slave Address Register                            */
    __IO uint32_t HS_MADDR;                         /* (0x0C) I2C Master Mode Code Address Register                 */
    __IO uint32_t DATA_CMD;                         /* (0x10) I2C Data Buffer and Command Register                  */
    __IO uint32_t SS_SCL_HCNT;                      /* (0x14) I2C Standard Speed Clock SCL High Count Register      */
    __IO uint32_t SS_SCL_LCNT;                      /* (0x18) I2C Standard Speed Clock SCL Low Count Register       */
    __IO uint32_t FS_SCL_HCNT;                      /* (0x1C) I2C Fast Speed Clock SCL High Count Register          */
    __IO uint32_t FS_SCL_LCNT;                      /* (0x20) I2C Fast Speed Clock SCL Low Count Register           */
    __IO uint32_t HS_SCL_HCNT;                      /* (0x24) I2C High Speed Clock SCL High Count Register          */
    __IO uint32_t HS_SCL_LCNT;                      /* (0x28) I2C High Speed Clock SCL Low Count Register           */
    __IO uint32_t INTR_STAT;                        /* (0x2C) I2C Interrupt Status Register                         */
    __IO uint32_t INTR_MASK;                        /* (0x30) I2C Interrupt Mask Register                           */
    __IO uint32_t RAW_INTR_STAT;                    /* (0x34) I2C Raw Interrupt Status Register                     */
    __IO uint32_t RX_TL;                            /* (0x38) I2C Receive FIFO Threshold Register                   */
    __IO uint32_t TX_TL;                            /* (0x3C) I2C Transmit FIFO Threshold Register                  */
    __IO uint32_t CLR_INTR;                         /* (0x40) I2C Clear Combined and Individual Interrupt Register  */
    __IO uint32_t CLR_RX_UNDER;                     /* (0x44) I2C Clear RX_UNDER Interrupt Register                 */
    __IO uint32_t CLR_RX_OVER;                      /* (0x48) I2C Clear RX_OVER Interrupt Register                  */
    __IO uint32_t CLR_TX_OVER;                      /* (0x4C) I2C Clear TX_OVER Interrupt Register                  */
    __IO uint32_t CLR_RD_REQ;                       /* (0x50) I2C Clear RD_REQ Interrupt Register                   */
    __IO uint32_t CLR_TX_ABRT;                      /* (0x54) I2C Clear TX_ABRT Interrupt Register                  */
    __IO uint32_t CLR_RX_DONE;                      /* (0x58) I2C Clear RX_DONE Interrupt Register                  */
    __IO uint32_t CLR_ACTIVITY;                     /* (0x5C) I2C Clear ACTIVITY Interrupt Register                 */
    __IO uint32_t CLR_STOP_DET;                     /* (0x60) I2C Clear STOP_DET Interrupt Register                 */
    __IO uint32_t CLR_START_DET;                    /* (0x64) I2C Clear START_DET Interrupt Register                */
    __IO uint32_t CLR_GEN_CALL;                     /* (0x68) I2C Clear GEN_CALL Interrupt Register                 */
    __IO uint32_t I2C_ENABLE;                       /* (0x6C) I2C Enable Register                                   */
    __IO uint32_t STATUS;                           /* (0x70) I2C Status Register                                   */
    __IO uint32_t TXFLR;                            /* (0x74) I2C Transmit FIFO Level Register                      */
    __IO uint32_t RXFLR;                            /* (0x78) I2C Receive FIFO Level Register                       */
    __IO uint32_t RES0;                             /* (0x7C)                                                       */
    __IO uint32_t TX_ABRT_SOURCE;                   /* (0x80) I2C Transmit Abort Source Register                    */
    __IO uint32_t SLV_DATA_NACK_ONLY;               /* (0x84) I2C Generate SLV_DATA_NACK Register                   */
    __IO uint32_t RES1[3];                          /* (0x88-0x90) I2C Reserved Register                            */
    __IO uint32_t SDA_SETUP;                        /* (0x94) I2C SDA Setup Register                                */
    __IO uint32_t ACK_GENERAL_CALL;                 /* (0x98) I2C ACK General Call Register                         */
    __IO uint32_t ENBALE_STATUS;                    /* (0x9C) I2C Enable Status Register                            */
    __IO uint32_t RES2[21];                         /* (0xA0-0xF0) I2C Reserved Register                            */
    __IO uint32_t COMP_PARAM_1;                     /* (0xf4) I2C Component Parameter Register 1                    */
    __IO uint32_t COMP_VERSION;                     /* (0xf8) I2C Component Version Register                        */
    __IO uint32_t COMP_TYPE;                        /* (0xfc) I2C Component Type Register                           */
} I2C_T;

/* I2C Control Register*/
#define I2C_CON_MASTER_MODE_Pos          (0)
#define I2C_CON_MASTER_MODE_Msk          (0x01ul << I2C_CON_MASTER_MODE_Pos)
#define I2C_CON_SPEED_Pos                (1)
#define I2C_CON_SPEED_Msk                (0x03ul << I2C_CON_SPEED_Pos)
#define I2C_CON_10BITADDR_SLAVE_Pos      (3)
#define I2C_CON_10BITADDR_SLAVE_Msk      (0x01ul << I2C_CON_10BITADDR_SLAVE_Pos)
#define I2C_CON_10BITADDR_MASTER_Pos     (4)
#define I2C_CON_10BITADDR_MASTER_Msk     (0x01ul << I2C_CON_10BITADDR_MASTER_Pos)
#define I2C_CON_10BITADDR_MASTER_RD_ONLY (I2C_CON_10BITADDR_MASTER_Msk)
#define I2C_CON_RESTART_EN_Pos           (5)
#define I2C_CON_RESTART_EN_Msk           (0x01ul << I2C_CON_RESTART_EN_Pos)
#define I2C_CON_SLAVE_DISABLE_Pos        (6)
#define I2C_CON_SLAVE_DISABLE_Msk        (0x01ul << I2C_CON_SLAVE_DISABLE_Pos)

/* I2C Target Address Register*/
#define I2C_TAR_TAR_Pos                  (0)
#define I2C_TAR_TAR_Msk                  (0x3FFul << I2C_TAR_TAR_Pos)
#define I2C_TAR_GC_OR_START_Pos          (10)
#define I2C_TAR_GC_OR_START_Msk          (0x01ul << I2C_TAR_GC_OR_START_Pos)
#define I2C_TAR_SPECIAL_Pos              (11)
#define I2C_TAR_SPECIAL_Msk              (0x01ul << I2C_TAR_SPECIAL_Pos)
#define I2C_TAR_10BITADDR_MASTER_Pos     (12)
#define I2C_TAR_10BITADDR_MASTER_Msk     (0x01ul << I2C_TAR_10BITADDR_MASTER_Pos)

/* I2C Slave Address Register*/
#define I2C_SAR_SAR_Pos                  (0)
#define I2C_SAR_SAR_Msk                  (0x3FFul << I2C_SAR_SAR_Pos)

/* I2C Master Mode Code Address Register*/
#define I2C_HS_MADDR_MAR_Pos             (0)
#define I2C_HS_MADDR_MAR_Msk             (0x07ul << I2C_HS_MADDR_MAR_Pos)

/* I2C Rx/Tx Data Buffer and Command Register*/
#define I2C_DATA_CMD_DAT_Pos             (0)
#define I2C_DATA_CMD_DAT_Msk             (0x0FFul << I2C_DATA_CMD_DAT_Pos)
#define I2C_DATA_CMD_CMD_Pos             (8)
#define I2C_DATA_CMD_CMD_Msk             (0x01ul << I2C_DATA_CMD_CMD_Pos)

/* Standard Speed I2C Clock SCL High Count Register*/
#define I2C_SS_SCL_HCNT_COUNT_Pos        (0)
#define I2C_SS_SCL_HCNT_COUNT_Msk        (0xFFFFul << I2C_SS_SCL_HCNT_COUNT_Pos)

/* Standard Speed I2C Clock SCL Low Count Register*/
#define I2C_SS_SCL_LCNT_COUNT_Pos        (0)
#define I2C_SS_SCL_LCNT_COUNT_Msk        (0xFFFFul << I2C_SS_SCL_LCNT_COUNT_Pos)

/* Fast Speed I2C Clock SCL High Count Register*/
#define I2C_FS_SCL_HCNT_COUNT_Pos        (0)
#define I2C_FS_SCL_HCNT_COUNT_Msk        (0xFFFFul << I2C_FS_SCL_HCNT_COUNT_Pos)

/* Fast Speed I2C Clock SCL Low Count Register*/
#define I2C_FS_SCL_LCNT_COUNT_Pos        (0)
#define I2C_FS_SCL_LCNT_COUNT_Msk        (0xFFFFul << I2C_FS_SCL_LCNT_COUNT_Pos)

/* High Speed I2C Clock SCL High Count Register*/
#define I2C_HS_SCL_HCNT_COUNT_Pos        (0)
#define I2C_HS_SCL_HCNT_COUNT_Msk        (0xFFFFul << I2C_HS_SCL_HCNT_COUNT_Pos)

/* High Speed I2C Clock SCL Low Count Register*/
#define I2C_HS_SCL_LCNT_COUNT_Pos        (0)
#define I2C_HS_SCL_LCNT_COUNT_Msk        (0xFFFFul << I2C_HS_SCL_LCNT_COUNT_Pos)

/* I2C Interrupt Status Register*/
#define I2C_INTR_STAT_RX_UNDER_Pos       (0)
#define I2C_INTR_STAT_RX_UNDER_Msk       (0x01ul << I2C_INTR_STAT_RX_UNDER_Pos)
#define I2C_INTR_STAT_RX_OVER_Pos        (1)
#define I2C_INTR_STAT_RX_OVER_Msk        (0x01ul << I2C_INTR_STAT_RX_OVER_Pos)
#define I2C_INTR_STAT_RX_FULL_Pos        (2)
#define I2C_INTR_STAT_RX_FULL_Msk        (0x01ul << I2C_INTR_STAT_RX_FULL_Pos)
#define I2C_INTR_STAT_TX_OVER_Pos        (3)
#define I2C_INTR_STAT_TX_OVER_Msk        (0x01ul << I2C_INTR_STAT_TX_OVER_Pos)
#define I2C_INTR_STAT_TX_EMPTY_Pos       (4)
#define I2C_INTR_STAT_TX_EMPTY_Msk       (0x01ul << I2C_INTR_STAT_TX_EMPTY_Pos)
#define I2C_INTR_STAT_RD_REQ_Pos         (5)
#define I2C_INTR_STAT_RD_REQ_Msk         (0x01ul << I2C_INTR_STAT_RD_REQ_Pos)
#define I2C_INTR_STAT_TX_ABRT_Pos        (6)
#define I2C_INTR_STAT_TX_ABRT_Msk        (0x01ul << I2C_INTR_STAT_TX_ABRT_Pos)
#define I2C_INTR_STAT_RX_DONE_Pos        (7)
#define I2C_INTR_STAT_RX_DONE_Msk        (0x01ul << I2C_INTR_STAT_RX_DONE_Pos)
#define I2C_INTR_STAT_ACTIVITY_Pos       (8)
#define I2C_INTR_STAT_ACTIVITY_Msk       (0x01ul << I2C_INTR_STAT_ACTIVITY_Pos)
#define I2C_INTR_STAT_STOP_DET_Pos       (9)
#define I2C_INTR_STAT_STOP_DET_Msk       (0x01ul << I2C_INTR_STAT_STOP_DET_Pos)
#define I2C_INTR_STAT_START_DET_Pos      (10)
#define I2C_INTR_STAT_START_DET_Msk      (0x01ul << I2C_INTR_STAT_START_DET_Pos)
#define I2C_INTR_STAT_GEN_CALL_Pos       (11)
#define I2C_INTR_STAT_GEN_CALL_Msk       (0x01ul << I2C_INTR_STAT_GEN_CALL_Pos)

/* I2C Interrupt Mask Register*/
#define I2C_INTR_MASK_RX_UNDER_Pos       (0)
#define I2C_INTR_MASK_RX_UNDER_Msk       (0x01ul << I2C_INTR_MASK_RX_UNDER_Pos)
#define I2C_INTR_MASK_RX_OVER_Pos        (1)
#define I2C_INTR_MASK_RX_OVER_Msk        (0x01ul << I2C_INTR_MASK_RX_OVER_Pos)
#define I2C_INTR_MASK_RX_FULL_Pos        (2)
#define I2C_INTR_MASK_RX_FULL_Msk        (0x01ul << I2C_INTR_MASK_RX_FULL_Pos)
#define I2C_INTR_MASK_TX_OVER_Pos        (3)
#define I2C_INTR_MASK_TX_OVER_Msk        (0x01ul << I2C_INTR_MASK_TX_OVER_Pos)
#define I2C_INTR_MASK_TX_EMPTY_Pos       (4)
#define I2C_INTR_MASK_TX_EMPTY_Msk       (0x01ul << I2C_INTR_MASK_TX_EMPTY_Pos)
#define I2C_INTR_MASK_RD_REQ_Pos         (5)
#define I2C_INTR_MASK_RD_REQ_Msk         (0x01ul << I2C_INTR_MASK_RD_REQ_Pos)
#define I2C_INTR_MASK_TX_ABRT_Pos        (6)
#define I2C_INTR_MASK_TX_ABRT_Msk        (0x01ul << I2C_INTR_MASK_TX_ABRT_Pos)
#define I2C_INTR_MASK_RX_DONE_Pos        (7)
#define I2C_INTR_MASK_RX_DONE_Msk        (0x01ul << I2C_INTR_MASK_RX_DONE_Pos)
#define I2C_INTR_MASK_ACTIVITY_Pos       (8)
#define I2C_INTR_MASK_ACTIVITY_Msk       (0x01ul << I2C_INTR_MASK_ACTIVITY_Pos)
#define I2C_INTR_MASK_STOP_DET_Pos       (9)
#define I2C_INTR_MASK_STOP_DET_Msk       (0x01ul << I2C_INTR_MASK_STOP_DET_Pos)
#define I2C_INTR_MASK_START_DET_Pos      (10)
#define I2C_INTR_MASK_START_DET_Msk      (0x01ul << I2C_INTR_MASK_START_DET_Pos)
#define I2C_INTR_MASK_GEN_CALL_Pos       (11)
#define I2C_INTR_MASK_GEN_CALL_Msk       (0x01ul << I2C_INTR_MASK_GEN_CALL_Pos)

/* I2C Raw Interrupt Status Register*/
#define I2C_RAW_INTR_MASK_RX_UNDER_Pos     (0)
#define I2C_RAW_INTR_MASK_RX_UNDER_Msk     (0x01ul << I2C_RAW_INTR_MASK_RX_UNDER_Pos)
#define I2C_RAW_INTR_MASK_RX_OVER_Pos      (1)
#define I2C_RAW_INTR_MASK_RX_OVER_Msk      (0x01ul << I2C_RAW_INTR_MASK_RX_OVER_Pos)
#define I2C_RAW_INTR_MASK_RX_FULL_Pos      (2)
#define I2C_RAW_INTR_MASK_RX_FULL_Msk      (0x01ul << I2C_RAW_INTR_MASK_RX_FULL_Pos)
#define I2C_RAW_INTR_MASK_TX_OVER_Pos      (3)
#define I2C_RAW_INTR_MASK_TX_OVER_Msk      (0x01ul << I2C_RAW_INTR_MASK_TX_OVER_Pos)
#define I2C_RAW_INTR_MASK_TX_EMPTY_Pos     (4)
#define I2C_RAW_INTR_MASK_TX_EMPTY_Msk     (0x01ul << I2C_RAW_INTR_MASK_TX_EMPTY_Pos)
#define I2C_RAW_INTR_MASK_RD_REQ_Pos       (5)
#define I2C_RAW_INTR_MASK_RD_REQ_Msk       (0x01ul << I2C_RAW_INTR_MASK_RD_REQ_Pos)
#define I2C_RAW_INTR_MASK_TX_ABRT_Pos      (6)
#define I2C_RAW_INTR_MASK_TX_ABRT_Msk      (0x01ul << I2C_RAW_INTR_MASK_TX_ABRT_Pos)
#define I2C_RAW_INTR_MASK_RX_DONE_Pos      (7)
#define I2C_RAW_INTR_MASK_RX_DONE_Msk      (0x01ul << I2C_RAW_INTR_MASK_RX_DONE_Pos)
#define I2C_RAW_INTR_MASK_ACTIVITY_Pos     (8)
#define I2C_RAW_INTR_MASK_ACTIVITY_Msk     (0x01ul << I2C_RAW_INTR_MASK_ACTIVITY_Pos)
#define I2C_RAW_INTR_MASK_STOP_DET_Pos     (9)
#define I2C_RAW_INTR_MASK_STOP_DET_Msk     (0x01ul << I2C_RAW_INTR_MASK_STOP_DET_Pos)
#define I2C_RAW_INTR_MASK_START_DET_Pos    (10)
#define I2C_RAW_INTR_MASK_START_DET_Msk    (0x01ul << I2C_RAW_INTR_MASK_START_DET_Pos)
#define I2C_RAW_INTR_MASK_GEN_CALL_Pos     (11)
#define I2C_RAW_INTR_MASK_GEN_CALL_Msk     (0x01ul << I2C_RAW_INTR_MASK_GEN_CALL_Pos)

/* I2C Receive FIFO Threshold Register*/
#define I2C_RX_TL_VALUE_Pos                (0)
#define I2C_RX_TL_VALUE_Msk                (0xFFul << I2C_RX_TL_VALUE_Pos)

/* I2C Transmit FIFO Threshold Register*/
#define I2C_TX_TL_VALUE_Pos                (0)
#define I2C_TX_TL_VALUE_Msk                (0xFFul << I2C_TX_TL_VALUE_Pos)

/* Clear Combined and Individual Interrupt Register*/
#define I2C_CLR_INTR_CLR                        0x00000001U

/* Clear RX_UNDER Interrupt Register*/
#define I2C_CLR_RX_UNDER_CLR                    0x00000001U

/* Clear RX_OVER Interrupt Register*/
#define I2C_CLR_RX_OVER_CLR                     0x00000001U

/* Clear TX_OVER Interrupt Register*/
#define I2C_CLR_TX_OVER_CLR                     0x00000001U

/* Clear RD_REQ Interrupt Register*/
#define I2C_CLR_RD_REQ_CLR                      0x00000001U

/* Clear TX_ABRT Interrupt Register*/
#define I2C_CLR_TX_ABRT_CLR                     0x00000001U

/* Clear RX_DONE Interrupt Register*/
#define I2C_CLR_RX_DONE_CLR                     0x00000001U

/* Clear ACTIVITY Interrupt Register*/
#define I2C_CLR_ACTIVITY_CLR                    0x00000001U

/* Clear STOP_DET Interrupt Register*/
#define I2C_CLR_STOP_DET_CLR                    0x00000001U

/* Clear START_DET Interrupt Register*/
#define I2C_CLR_START_DET_CLR                   0x00000001U

/* Clear GEN_CALL Interrupt Register*/
#define I2C_CLR_GEN_CALL_CLR                    0x00000001U

/* I2C Enable Register*/
#define I2C_ENABLE_ENABLE_Pos         (0)
#define I2C_ENABLE_ENABLE_Msk         (0x01ul << I2C_ENABLE_ENABLE_Pos)

/* I2C Status Register*/
#define I2C_STATUS_ACTIVITY_Pos       (0)
#define I2C_STATUS_ACTIVITY_Msk       (0x01ul << I2C_STATUS_ACTIVITY_Pos)
#define I2C_STATUS_TFNF_Pos           (1)
#define I2C_STATUS_TFNF_Msk           (0x01ul << I2C_STATUS_TFNF_Pos)
#define I2C_STATUS_TFE_Pos            (2)
#define I2C_STATUS_TFE_Msk            (0x01ul << I2C_STATUS_TFE_Pos)
#define I2C_STATUS_RFNE_Pos           (3)
#define I2C_STATUS_RFNE_Msk           (0x01ul << I2C_STATUS_RFNE_Pos)
#define I2C_STATUS_RFF_Pos            (4)
#define I2C_STATUS_RFF_Msk            (0x01ul << I2C_STATUS_RFF_Pos)
#define I2C_STATUS_MST_ACTIVITY_Pos   (5)
#define I2C_STATUS_MST_ACTIVITY_Msk   (0x01ul << I2C_STATUS_MST_ACTIVITY_Pos)
#define I2C_STATUS_SLV_ACTIVITY_Pos   (6)
#define I2C_STATUS_SLV_ACTIVITY_Msk   (0x01ul << I2C_STATUS_SLV_ACTIVITY_Pos)

/* I2C Transmit FIFO Level Register*/
#define I2C_TXFLR_VALUE_Pos           (0)
#define I2C_TXFLR_VALUE_Msk           (0x07ul << I2C_TXFLR_VALUE_Pos)

/* I2C Receive FIFO Level Register*/
#define I2C_RXFLR_VALUE_Pos           (0)
#define I2C_RXFLR_VALUE_Msk           (0x07ul << I2C_RXFLR_VALUE_Pos)

/* I2C Transmit Abort Source Register*/
#define I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Pos       (0)
#define I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Msk       (0x01ul << I2C_TX_ABRT_SOURCE_7B_ADDR_NOACK_Pos)
#define I2C_TX_ABRT_SOURCE_10B_ADDR1_NOACK_Pos     (1)
#define I2C_TX_ABRT_SOURCE_10B_ADDR1_NOACK_Msk     (0x01ul << I2C_TX_ABRT_SOURCE_10B_ADDR1_NOACK_Pos)
#define I2C_TX_ABRT_SOURCE_10B_ADDR2_NOACK_Pos     (2)
#define I2C_TX_ABRT_SOURCE_10B_ADDR2_NOACK_Msk     (0x01ul << I2C_TX_ABRT_SOURCE_10B_ADDR2_NOACK_Pos)
#define I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Pos        (3)
#define I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Msk        (0x01ul << I2C_TX_ABRT_SOURCE_TXDATA_NOACK_Pos)
#define I2C_TX_ABRT_SOURCE_GCALL_NOACK_Pos         (4)
#define I2C_TX_ABRT_SOURCE_GCALL_NOACK_Msk         (0x01ul << I2C_TX_ABRT_SOURCE_GCALL_NOACK_Pos)
#define I2C_TX_ABRT_SOURCE_GCALL_READ_Pos          (5)
#define I2C_TX_ABRT_SOURCE_GCALL_READ_Msk          (0x01ul << I2C_TX_ABRT_SOURCE_GCALL_READ_Pos)
#define I2C_TX_ABRT_SOURCE_HS_ACKDET_Pos           (6)
#define I2C_TX_ABRT_SOURCE_HS_ACKDET_Msk           (0x01ul << I2C_TX_ABRT_SOURCE_HS_ACKDET_Pos)
#define I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Pos        (7)
#define I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Msk        (0x01ul << I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Pos)
#define I2C_TX_ABRT_SOURCE_HS_NORSTRT_Pos          (8)
#define I2C_TX_ABRT_SOURCE_HS_NORSTRT_Msk          (0x01ul << I2C_TX_ABRT_SOURCE_HS_NORSTRT_Pos)
#define I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Pos       (9)
#define I2C_TX_ABRT_SOURCE_SBYTE_NORSTRT_Msk       (0x01ul << I2C_TX_ABRT_SOURCE_SBYTE_ACKDET_Pos)
#define I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Pos      (10)
#define I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Msk      (0x01ul << I2C_TX_ABRT_SOURCE_10B_RD_NORSTRT_Pos)
#define I2C_TX_ABRT_SOURCE_MASTER_DIS_Pos          (11)
#define I2C_TX_ABRT_SOURCE_MASTER_DIS_Msk          (0x01ul << I2C_TX_ABRT_SOURCE_MASTER_DIS_Pos)
#define I2C_TX_ABRT_SOURCE_ARB_LOST_Pos            (12)
#define I2C_TX_ABRT_SOURCE_ARB_LOST_Msk            (0x01ul << I2C_TX_ABRT_SOURCE_ARB_LOST_Pos)
#define I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Pos     (13)
#define I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Msk     (0x01ul << I2C_TX_ABRT_SOURCE_SLVFLUSH_TXFIFO_Pos)
#define I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Pos         (14)
#define I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Msk         (0x01ul << I2C_TX_ABRT_SOURCE_SLV_ARBLOST_Pos)
#define I2C_TX_ABRT_SOURCE_SLVRD_INTX_Pos          (15)
#define I2C_TX_ABRT_SOURCE_SLVRD_INTX_Msk          (0x01ul << I2C_TX_ABRT_SOURCE_SLVRD_INTX_Pos)

/* I2C Generate SLV_DATA_NACK Register*/
#define I2C_SLV_DATA_NACK_ONLY_NACK_Pos            (0)
#define I2C_SLV_DATA_NACK_ONLY_NACK_Msk            (0x01ul << I2C_SLV_DATA_NACK_ONLY_NACK_Pos)

/* I2C SDA Setup Register*/
#define I2C_SDA_SETUP_VALUE_Pos                    (0)
#define I2C_SDA_SETUP_VALUE_Msk                    (0x0FFul << I2C_SDA_SETUP_VALUE_Pos)

/* I2C ACK General Call Register*/
#define I2C_ACK_GENERAL_CALL_ENABLE_Pos            (0)
#define I2C_ACK_GENERAL_CALL_ENABLE_Msk            (0x01ul << I2C_ACK_GENERAL_CALL_ENABLE_Pos)

/* I2C Enable Status Register*/
#define I2C_ENABLE_STATUS_IC_EN_Pos                     (0)
#define I2C_ENABLE_STATUS_IC_EN_Msk                     (0x01ul << I2C_ENABLE_STATUS_IC_EN_Pos)
#define I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos   (1)
#define I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Msk   (0x01ul << I2C_ENABLE_STATUS_SLV_DISABLED_WHILE_BUSY_Pos)
#define I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos          (2)
#define I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Msk          (0x01ul << I2C_ENABLE_STATUS_SLV_RX_DATA_LOST_Pos)

/* Component Parameter Register 1*/
#define I2C_COMP_PARAM1_APB_DATA_WIDTH_Pos              (0)
#define I2C_COMP_PARAM1_APB_DATA_WIDTH_Msk              (0x03ul << I2C_COMP_PARAM1_APB_DATA_WIDTH_Pos)
#define I2C_COMP_PARAM1_MAX_SPEED_MODE_Pos              (2)
#define I2C_COMP_PARAM1_MAX_SPEED_MODE_Msk              (0x03ul << I2C_COMP_PARAM1_MAX_SPEED_MODE_Pos)
#define I2C_COMP_PARAM1_HC_COUNT_VALUES_Pos             (4)
#define I2C_COMP_PARAM1_HC_COUNT_VALUES_Msk             (0x01ul << I2C_COMP_PARAM1_HC_COUNT_VALUES_Pos)
#define I2C_COMP_PARAM1_INTR_IO_Pos                     (5)
#define I2C_COMP_PARAM1_INTR_IO_Msk                     (0x01ul << I2C_COMP_PARAM1_INTR_IO_Pos)
#define I2C_COMP_PARAM1_HAS_DMA_Pos                     (6)
#define I2C_COMP_PARAM1_HAS_DMA_Msk                     (0x01ul << I2C_COMP_PARAM1_HAS_DMA_Pos)
#define I2C_COMP_PARAM1_ENCODED_PARAMS_Pos              (7)
#define I2C_COMP_PARAM1_ENCODED_PARAMS_Msk              (0x01ul << I2C_COMP_PARAM1_ENCODED_PARAMS_Pos)
#define I2C_COMP_PARAM1_RX_BUFFER_DEPTH_Pos             (8)
#define I2C_COMP_PARAM1_RX_BUFFER_DEPTH_Msk             (0xFFul << I2C_COMP_PARAM1_RX_BUFFER_DEPTH_Pos)
#define I2C_COMP_PARAM1_TX_BUFFER_DEPTH_Pos             (16)
#define I2C_COMP_PARAM1_TX_BUFFER_DEPTH_Msk             (0xFFul << I2C_COMP_PARAM1_TX_BUFFER_DEPTH_Pos)

/* I2C Component Version Register*/
#define I2C_COMP_VERSION_VALUE_Pos                      (0)
#define I2C_COMP_VERSION_VALUE_Msk                      (0xFFFFFFFFul << I2C_COMP_VERSION_VALUE_Pos)

/* I2C Component Type Register*/
#define I2C_COMP_TYPE_VALUE_Pos                         (0)
#define I2C_COMP_TYPE_VALUE_Msk                         (0xFFFFFFFFul << I2C_COMP_TYPE_VALUE_Pos)

/*---------------------- I2C END ---------------------------------*/

/*--------------------- FADC ---------------------------------*/
// FADC Base 0x4002C000
typedef struct {
    __IO uint32_t SPI_DIV;                     /*!< Offset: 0x000  */
    __IO uint32_t SPI_EN;                      /*!< Offset: 0x004   */
    __IO uint32_t SPI_CS;                      /*!< Offset: 0x008  */
    __IO uint32_t SPI_DI;                      /*!< Offset: 0x00C  */
    __IO uint32_t SPI_DO;                      /*!< Offset: 0x010  */
    __IO uint32_t SPI_STA;                     /*!< Offset: 0x014 (RO) */
} FADC_T;

//SPI_DIV
#define ADC_SPI_CLK_DIV_Pos                        (0)
#define ADC_SPI_CLK_DIV_Msk                        (0xFFul << ADC_SPI_CLK_DIV_Pos)
#define ADC_SPI_SCK_PRE_Pos                        (8)
#define ADC_SPI_SCK_PRE_Msk                        (0x0Ful << ADC_SPI_SCK_PRE_Pos)

//SPI_EN
#define ADC_SPI_CLK_EN_Pos                         (0)
#define ADC_SPI_CLK_EN_Msk                         (0x1ul << ADC_SPI_CLK_EN_Pos)

//SPI_STA
#define ADC_SPI_ST_DATA_VALID_Pos                  (0)
#define ADC_SPI_ST_DATA_VALID_Msk                  (0x1ul << ADC_SPI_ST_DATA_VALID_Pos)
#define ADC_SPI_ST_SHIF_DONE_Pos                   (1)
#define ADC_SPI_ST_SHIF_DONE_Msk                   (0x1ul << ADC_SPI_ST_SHIF_DONE_Pos)
/*---------------------- END ---------------------------------*/

/*--------------------- SAR ADC ---------------------------------*/
//ADC Base
typedef struct {
    __IO uint32_t CON;                         //!< Offset: 0x000 (R/W)  ADC control Register
    __IO uint32_t TRIG;                        //!< Offset: 0x004 (R/W)  ADC trigger Register
    __IO uint32_t DAT;                         //!< Offset: 0x008 (R/W)  ADC data Register
    __IO uint32_t STA;                         //!< Offset: 0x00C (R/W)  ADC state Register
} SADC_T;

//SADC_CON
#define SADC_CON_MUX_Pos                       (0)
#define SADC_CON_MUX_Msk                       (0x0Ful << SADC_CON_MUX_Pos)
#define SADC_CON_EN_Pos                        (4)
#define SADC_CON_EN_Msk                        (0x01ul << SADC_CON_EN_Pos)
#define SADC_CON_IE_Pos                        (7)
#define SADC_CON_IE_Msk                        (0x01ul << SADC_CON_IE_Pos)
#define SADC_CON_CLK_DIV_Pos                   (8)
#define SADC_CON_CLK_DIV_Msk                   (0xFFul << SADC_CON_CLK_DIV_Pos)

//SADC_TRIG
#define SADC_TRIG_Pos                          (0)
#define SADC_TRIG_Msk                          (0x01ul << SADC_TRIG_Pos)

//SADC_DAT
#define SADC_DAT_Pos                           (0)
#define SADC_DAT_Msk                           (0x3FFul << SADC_DAT_Pos)

//SADC_STA
#define SADC_EOC_Pos                           (0)
#define SADC_EOC_Msk                           (0x01ul << SADC_EOC_Pos)

/*---------------------- END ---------------------------------*/

/** @addtogroup SKY32B750_PERIPHERAL_MEM_MAP SKY32B750 Peripheral Memory Map
  Memory Mapped Structure for SKY32B750 Series Peripheral
  @{
 */

// FLASH & SRAM Memory Base
#define FLASH_BASE                         ((uint32_t)0x00000000)          //// 0x00000000 Flash base address
#define FLASH_CFG_BASE                     ((uint32_t)0x00080000)          //// 0x00080000 Flash CFG base address
#define DMA_SAON_RAM_BASE                  ((uint32_t)0x20000000)          //// 0x20000000 DMA SRAM base address
#define SAON_RAM_BASE                      ((uint32_t)0x20000000)          //// 0x20000000 SRAM base address
#define SRAM0_BASE                         ((uint32_t)0x20000000)          //// 0x20000000 SRAM0 base address
#define SRAM1_BASE                         ((uint32_t)0x20004000)          //// 0x20004000 SRAM1 base address
#define SRAM2_BASE                         ((uint32_t)0x20008000)          //// 0x20008000 SRAM2 base address
#define SRAM3_BASE                         ((uint32_t)0x2000C000)          //// 0x2000C000 SRAM3 base address

// AHB Peripherals Base
#define AHB_PERIPH_BASE                    ((uint32_t)0x50000000)          //// 0x50000000 AHB base address
#define SYSC_BASE                          (AHB_PERIPH_BASE  + 0x00000)    //// 0x50000000 system control base address
#define GPIO0_BASE                         (AHB_PERIPH_BASE  + 0x10000)    //// 0x50010000 GPIO Port 0 base address
#define GPIO1_BASE                         (AHB_PERIPH_BASE  + 0x10040)    //// 0x50010040 GPIO Port 1 base address
#define GPIO2_BASE                         (AHB_PERIPH_BASE  + 0x10080)    //// 0x50010080 GPIO Port 2 base address
#define GPIO3_BASE                         (AHB_PERIPH_BASE  + 0x100C0)    //// 0x500100C0 GPIO Port 3 base address
#define GPIO0_PIN_DATA_BASE                (AHB_PERIPH_BASE  + 0x10400)    //// 0x50010400 GPIO Port 0 Data Bit Operation base address
#define GPIO1_PIN_DATA_BASE                (AHB_PERIPH_BASE  + 0x10440)    //// 0x50010440 GPIO Port 1 Data Bit Operation base address
#define GPIO2_PIN_DATA_BASE                (AHB_PERIPH_BASE  + 0x10480)    //// 0x50010480 GPIO Port 2 Data Bit Operation base address
#define GPIO3_PIN_DATA_BASE                (AHB_PERIPH_BASE  + 0x104C0)    //// 0x500104C0 GPIO Port 3 Data Bit Operation base address
#define CAN0_BASE                          (AHB_PERIPH_BASE  + 0x20000)    //// 0x50020000 Flex CAN Controller0 base address
#define CAN1_BASE                          (AHB_PERIPH_BASE  + 0x30000)    //// 0x50030000 Flex CAN Controller1 base address
#define QSPI0_BASE                         (AHB_PERIPH_BASE  + 0x40000)    //// 0x50040000 QSPI Controller0 base address
#define QSPI1_BASE                         (AHB_PERIPH_BASE  + 0x50000)    //// 0x50050000 QSPI Controller1 base address
#define DMAC_BASE                          (AHB_PERIPH_BASE  + 0x60000)    //// 0x50060000 DMA Controller base address
#define EFC_BASE                           (AHB_PERIPH_BASE  + 0x70000)    //// 0x50070000 Embedded Flash Controller base address
#define OSPI0_BASE                         (AHB_PERIPH_BASE  + 0x80000)    //// 0x50080000 OSPI Controller0 base address
#define OSPI1_BASE                         (AHB_PERIPH_BASE  + 0x90000)    //// 0x50090000 OSPI Controller1 base address

// AXI2AHB Peripherals Base
#define BB_BASE                            ((uint32_t)0x30000000)          // 0x3000_0000-0x3011_FFFF, 1152KB
#define BB_ABUF_BASE                       ((uint32_t)0x30000000)          // 0x3000_0000-0x3003_FFFF,  256KB
#define BB_ABUF0_BASE                      ((uint32_t)0x30000000)          // 0x3000_0000-0x3003_FFFF,  256KB
#define BB_ABUF1_BASE                      ((uint32_t)0x30040000)          // 0x3004_0000-0x3007_FFFF,  256KB
#define BB_ABUF2_BASE                      ((uint32_t)0x30080000)          // 0x3008_0000-0x300B_FFFF,  256KB
#define BB_ABUF3_BASE                      ((uint32_t)0x300C0000)          // 0x300C_0000-0x300F_FFFF,  256KB
#define BB_ABUF4_BASE                      ((uint32_t)0x30100000)          // 0x3010_0000-0x3011_FFFF,  128KB
#define BB_DBUF_BASE                       ((uint32_t)0x30300000)          // 0x3030_0000-0x3031_FFFF,  128KB
#define BB_DBUF0_BASE                      ((uint32_t)0x30300000)          // 0x3030_0000-0x3030_FFFF,   64KB
#define BB_DBUF1_BASE                      ((uint32_t)0x30310000)          // 0x3031_0000-0x3031_FFFF,   64KB
#define BB_WBUF_BASE                       ((uint32_t)0x30380000)          // 0x3038_0000-0x3038_1FFF,    8KB
#define BB_FBUF_BASE                       ((uint32_t)0x303C0000)          // 0x303C_0000-0x303C_3FFF,   16KB
#define BB_REGB_BASE                       ((uint32_t)0x31C00000)
#define BB_P1MUL_BASE                      ((uint32_t)0x31000000)
#define BB_FFT_BASE                        ((uint32_t)0x31000010)
#define BB_ZO_BASE                         ((uint32_t)0x31000020)

#define QSPI0_XIP_BASE                     ((uint32_t)0x60000000)           //// 0x60000000 QSPI0 XIP base address
#define QSPI1_XIP_BASE                     ((uint32_t)0x61000000)           //// 0x61000000 QSPI1 XIP base address
#define OSPI0_XIP_BASE                     ((uint32_t)0x63000000)           //// 0x63000000 OSPI0 XIP base address
#define OSPI1_XIP_BASE                     ((uint32_t)0x64000000)           //// 0x64000000 OSPI1 XIP base address

// APB Peripherals Base
#define APB_PERIPH_BASE                    ((uint32_t)0x40000000)           //// 0x40000000 APB base address
#define USCI0_BASE                         (APB_PERIPH_BASE  + 0x00000)     //// 0x40000000 USCI0   register base address
#define USCI1_BASE                         (APB_PERIPH_BASE  + 0x04000)     //// 0x40004000 USCI1   register base address
#define USCI2_BASE                         (APB_PERIPH_BASE  + 0x08000)     //// 0x40008000 USCI2   register base address
#define USCI3_BASE                         (APB_PERIPH_BASE  + 0x0C000)     //// 0x4000C000 USCI3   register base address
#define USCI4_BASE                         (APB_PERIPH_BASE  + 0x10000)     //// 0x40010000 USCI4   register base address
#define USCI5_BASE                         (APB_PERIPH_BASE  + 0x14000)     //// 0x40014000 USCI5   register base address
#define USCI6_BASE                         (APB_PERIPH_BASE  + 0x18000)     //// 0x40018000 USCI6   register base address
#define USCI7_BASE                         (APB_PERIPH_BASE  + 0x1C000)     //// 0x4001C000 USCI7   register base address
#define TMRA_BASE                          (APB_PERIPH_BASE  + 0x20000)     //// 0x40020000 TimerA  register base address
#define TMRB_BASE                          (APB_PERIPH_BASE  + 0x24000)     //// 0x40024000 TimerB  register base address
#define WWDG_BASE                          (APB_PERIPH_BASE  + 0x28000)     //// 0x40028000 WWDG    register base address
#define FADC_BASE                          (APB_PERIPH_BASE  + 0x2C000)     //// 0x40030000 Fast ADC Controller base address
#define TRNG_BASE                          (APB_PERIPH_BASE  + 0x30000)     //// 0x40030000 TRNG    register base address
#define CRC_BASE                           (APB_PERIPH_BASE  + 0x34000)     //// 0x40034000 CRC    register base address
#define SADC_BASE                          (APB_PERIPH_BASE  + 0x70000)     //// 0x40070000 Slow ADC Controller base address

#define AON_BASE                           (APB_PERIPH_BASE  + 0x80000)     //// 0x40080000 AON base address
#define AON_SYSC_BASE                      (AON_BASE         + 0x00000)     //// 0x40080000 Always-on system control base address
#define AON_RAM_BASE                       (AON_BASE         + 0x01000)     //// 0x40081000 Always-on SRAM (2KB) base address
#define AON_WKU_BASE                       (AON_BASE         + 0x03000)     //// 0x40083000 Always-on Wakeup Controller base address
#define AON_WDT_BASE                       (AON_BASE         + 0x04000)     //// 0x40084000 Always-on WDT register base address
#define AON_RTC_BASE                       (AON_BASE         + 0x05000)     //// 0x40085000 Always-on RTC register base address

#define PINMUX_BASE                        (APB_PERIPH_BASE  + 0x90000)     //// 0x40090000 PIN MUX register base address
#define I2C_BASE                           (APB_PERIPH_BASE  + 0xA0000)     //// 0x400A0000 I2C register base address
/** Memory Map End */

#define SADC                               ((SADC_T *) SADC_BASE)              //// 0x40070000 Pointer to SADC register structurel
#define FADC                               ((FADC_T *) FADC_BASE)              //// 0x4002C000 Pointer to FADC register structurel
#define WDT                                ((WDT_T *) AON_WDT_BASE)            //// 0x40084000 Pointer to WDT register structure
#define RTC                                ((RTC_T *) AON_RTC_BASE)            //// 0x40085000 Pointer to RTC register structurel
#define EFC_BUF                            ((EFC_BUF_T *) (EFC_BASE + 0x200))  //// 0x50070200 Pointer to EFC Buffer register structurel

#define USCI0                              ((USCI_T *) USCI0_BASE)             //// 0x40000000 Pointer to USCI0 register structure
#define USCI1                              ((USCI_T *) USCI1_BASE)             //// 0x40004000 Pointer to USCI1 register structure
#define USCI2                              ((USCI_T *) USCI2_BASE)             //// 0x40008000 Pointer to USCI2 register structure
#define USCI3                              ((USCI_T *) USCI3_BASE)             //// 0x4000C000 Pointer to USCI3 register structure
#define USCI4                              ((USCI_T *) USCI4_BASE)             //// 0x40010000 Pointer to USCI4 register structure
#define USCI5                              ((USCI_T *) USCI5_BASE)             //// 0x40014000 Pointer to USCI5 register structure
#define USCI6                              ((USCI_T *) USCI6_BASE)             //// 0x40018000 Pointer to USCI6 register structure
#define USCI7                              ((USCI_T *) USCI7_BASE)             //// 0x4001C000 Pointer to USCI7 register structure
#define QSPI0                              ((QSPI_T *) QSPI0_BASE)
#define QSPI1                              ((QSPI_T *) QSPI1_BASE)

#define OSPI0                              ((OSPI_T *) OSPI0_BASE)             //// 0x50080000 Pointer to OSPI0 register structure
#define OSPI1                              ((OSPI_T *) OSPI1_BASE)             //// 0x50090000 Pointer to OSPI1 register structure
#define XOSPI0                             ((XOSPI_T *) OSPI0_XIP_BASE)        //// 0x63000000 Pointer to OSPI register structure
#define XOSPI1                             ((XOSPI_T *) OSPI1_XIP_BASE)        //// 0x64000000 Pointer to OSPI register structure

#define TRNG                               ((TRNG_T *)  TRNG_BASE)                    //// Pointer to NFC  register structure
#define CRC                                ((CRC_T *)  CRC_BASE)                      //// Pointer to NFC  register structure
#define WWDG                               ((WWDG_T *)  WWDG_BASE)
#define TMRA                               ((TMRA_T  *)   TMRA_BASE)                  //// Pointer to TMRA  register structure

#define TMRB                               ((TMRB_T  *)   TMRB_BASE)                  //// Pointer to TMRB  register structure
#define TMRBS                              ((TMRBS_T *)  (TMRB_BASE + 0x000A0))       //// Pointer to TMRBS register structure

#define PORT01_MUX_BASE                    (PINMUX_BASE  + 0x00018)                   //// 0x40090018 PIN_MUX01 register base address
#define PORT23_MUX_BASE                    (PINMUX_BASE  + 0x00058)                   //// 0x40090058 PIN_MUX23 register base address

#define GPIO0                              ((GPIO_T *) GPIO0_BASE)                    //// 0x50010000 Pointer to GPIO0 register structure
#define GPIO1                              ((GPIO_T *) GPIO1_BASE)                    //// 0x50010040 Pointer to GPIO1 register structure
#define GPIO2                              ((GPIO_T *) GPIO2_BASE)                    //// 0x50010080 Pointer to GPIO2 register structure
#define GPIO3                              ((GPIO_T *) GPIO3_BASE)                    //// 0x500100C0 Pointer to GPIO3 register structure
#define EFC                                ((EFC_T *) EFC_BASE)                       //// 0x50070000 Pointer to EFC register structure
#define PORT01_PINMUX                      ((PINMUX_T *) PORT01_MUX_BASE)             //// 0x40090018 Pointer to PIN_MUX01 register structure
#define PORT23_PINMUX                      ((PINMUX_T *) PORT23_MUX_BASE)             //// 0x40090058 Pointer to PIN_MUX23 register structure
#define PORT01_PIN_CTRL                    ((PIN_CTRL_T *) PINMUX_BASE)               //// 0x40090000 Pointer to AON_PIN_CTRL register structure
#define PORT23_PIN_CTRL                    ((PIN_CTRL_T *) (PINMUX_BASE+0x40))        //// 0x40090040 Pointer to AON_PIN_CTRL register structure

#define AON_SYSC                           ((AON_SYSC_T *)  AON_SYSC_BASE)            //// Pointer to NFC  register structure
#define AON_WKU                            ((AON_WKU_T  *)  AON_WKU_BASE )            //// Pointer to NFC  register structure
#define SYSC                               ((SYSC_T *)  SYSC_BASE)                    //// Pointer to NFC  register structure

//DMA
#define DMA_CH0                            ((DMA_CH_T  *)   (DMAC_BASE))
#define DMA_CH1                            ((DMA_CH_T  *)   (DMAC_BASE+0x058))
#define DMA_CH2                            ((DMA_CH_T  *)   (DMAC_BASE+0x0b0))
#define DMA_CH3                            ((DMA_CH_T  *)   (DMAC_BASE+0x108))
#define DMA_CH4                            ((DMA_CH_T  *)   (DMAC_BASE+0x160))
#define DMA_CH5                            ((DMA_CH_T  *)   (DMAC_BASE+0x1b8))
#define DMA_CH6                            ((DMA_CH_T  *)   (DMAC_BASE+0x210))
#define DMA_CH7                            ((DMA_CH_T  *)   (DMAC_BASE+0x268))

#define DMA_INT                            ((DMA_INT_T   *)   (DMAC_BASE+0x2C0))
#define DMA_SWHS                           ((DMA_SWHS_T  *)   (DMAC_BASE+0x368))
#define DMA_EN                             ((DMA_EN_T    *)   (DMAC_BASE+0x398))

#define CAN0                               ((CAN_Type *)CAN0_BASE)
#define CAN1                               ((CAN_Type *)CAN1_BASE)
#define CAN_BASE_PTRS                      {CAN0, CAN1}

#define I2C                                ((I2C_T  *)(I2C_BASE))

/*@}*/ /* end of group SKY32B750_PERIPHERAL_DECLARATION */

/*@}*/ /* end of group SKY32B750_Peripherals */

/** @addtogroup SKY32B750_IO_ROUTINE SKY32B750 I/O Routines
  The Declaration of SKY32B750 I/O Routines
  @{
 */

typedef volatile unsigned char  vu8;        ///< Define 8-bit unsigned volatile data type
typedef volatile unsigned short vu16;       ///< Define 16-bit unsigned volatile data type
typedef volatile unsigned int   vu32;       ///< Define 32-bit unsigned volatile data type
typedef volatile unsigned long long  vu64;  ///< Define 64-bit unsigned volatile data type

/**
  * @brief Get a 8-bit unsigned value from specified address
  * @param[in] addr Address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified address
  */
#define Read_M8(addr)  (*((vu8  *) (addr)))

/**
  * @brief Get a 16-bit unsigned value from specified address
  * @param[in] addr Address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified address
  * @note The input address must be 16-bit aligned
  */
#define Read_M16(addr) (*((vu16 *) (addr)))

/**
  * @brief Get a 32-bit unsigned value from specified address
  * @param[in] addr Address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified address
  * @note The input address must be 32-bit aligned
  */
#define Read_M32(addr) (*((vu32 *) (addr)))

/**
  * @brief Get a 64-bit unsigned value from specified address
  * @param[in] addr Address to get 32-bit data from
  * @return  64-bit unsigned value stored in specified address
  * @note The input address must be 64-bit aligned
  */
#define Read_M64(addr) (*((vu64 *) (addr)))

/**
  * @brief Write a 32-bit unsigned value to specified address
  * @param[in] addr Address to put 32-bit data to
  * @param[in] addr Data to be put 32-bit
  * @return  none
  * @note The input address must be 32-bit aligned
  */
#define Write_M64(addr, value) (*((vu64 *) (addr)) = (uint64_t)value)
#define Write_M32(addr, value) (*((vu32 *) (addr)) = value)
#define Write_M16(addr, value)  (*((vu16 *) (addr)) = (uint16_t)value)
#define Write_M8(addr, value)  (*((vu8 *) (addr)) = (uint8_t)value)
/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outpw(port,value)     *((volatile unsigned int *)(port)) = value

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inpw(port)            (*((volatile unsigned int *)(port)))

/**
  * @brief Set a 16-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 16-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 16-bit aligned
  */
#define outps(port,value)     *((volatile unsigned short *)(port)) = value

/**
  * @brief Get a 16-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified I/O port
  * @note The input port must be 16-bit aligned
  */
#define inps(port)            (*((volatile unsigned short *)(port)))

/**
  * @brief Set a 8-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 8-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  */
#define outpb(port,value)     *((volatile unsigned char *)(port)) = value

/**
  * @brief Get a 8-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified I/O port
  */
#define inpb(port)            (*((volatile unsigned char *)(port)))

/**
  * @brief Set a 32-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 32-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 32-bit aligned
  */
#define outp32(port,value)    *((volatile unsigned int *)(port)) = value

/**
  * @brief Get a 32-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 32-bit data from
  * @return  32-bit unsigned value stored in specified I/O port
  * @note The input port must be 32-bit aligned
  */
#define inp32(port)           (*((volatile unsigned int *)(port)))

/**
  * @brief Set a 16-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 16-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  * @note The output port must be 16-bit aligned
  */
#define outp16(port,value)    *((volatile unsigned short *)(port)) = value

/**
  * @brief Get a 16-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 16-bit data from
  * @return  16-bit unsigned value stored in specified I/O port
  * @note The input port must be 16-bit aligned
  */
#define inp16(port)           (*((volatile unsigned short *)(port)))

/**
  * @brief Set a 8-bit unsigned value to specified I/O port
  * @param[in] port Port address to set 8-bit data
  * @param[in] value Value to write to I/O port
  * @return  None
  */
#define outp8(port,value)     *((volatile unsigned char *)(port)) = value

/**
  * @brief Get a 8-bit unsigned value from specified I/O port
  * @param[in] port Port address to get 8-bit data from
  * @return  8-bit unsigned value stored in specified I/O port
  */
#define inp8(port)            (*((volatile unsigned char *)(port)))

/*@}*/ /* end of group SKY32B750_IO_ROUTINE */

/******************************************************************************/
/*                Legacy Constants                                            */
/******************************************************************************/
/** @addtogroup SKY32B750_legacy_Constants SKY32B750 Legacy Constants
  SKY32B750 Legacy Constants
  @{
*/

#ifndef NULL
#define NULL           (0)      ///< NULL pointer
#endif

#define TRUE           (1)      ///< Boolean true, define to use in API parameters or return value
#define FALSE          (0)      ///< Boolean false, define to use in API parameters or return value

#define ENABLE         (1)      ///< Enable, define to use in API parameters
#define DISABLE        (0)      ///< Disable, define to use in API parameters

/* Define one bit mask */
#define BIT0     (0x00000001)       ///< Bit 0 mask of an 32 bit integer
#define BIT1     (0x00000002)       ///< Bit 1 mask of an 32 bit integer
#define BIT2     (0x00000004)       ///< Bit 2 mask of an 32 bit integer
#define BIT3     (0x00000008)       ///< Bit 3 mask of an 32 bit integer
#define BIT4     (0x00000010)       ///< Bit 4 mask of an 32 bit integer
#define BIT5     (0x00000020)       ///< Bit 5 mask of an 32 bit integer
#define BIT6     (0x00000040)       ///< Bit 6 mask of an 32 bit integer
#define BIT7     (0x00000080)       ///< Bit 7 mask of an 32 bit integer
#define BIT8     (0x00000100)       ///< Bit 8 mask of an 32 bit integer
#define BIT9     (0x00000200)       ///< Bit 9 mask of an 32 bit integer
#define BIT10    (0x00000400)       ///< Bit 10 mask of an 32 bit integer
#define BIT11    (0x00000800)       ///< Bit 11 mask of an 32 bit integer
#define BIT12    (0x00001000)       ///< Bit 12 mask of an 32 bit integer
#define BIT13    (0x00002000)       ///< Bit 13 mask of an 32 bit integer
#define BIT14    (0x00004000)       ///< Bit 14 mask of an 32 bit integer
#define BIT15    (0x00008000)       ///< Bit 15 mask of an 32 bit integer
#define BIT16    (0x00010000)       ///< Bit 16 mask of an 32 bit integer
#define BIT17    (0x00020000)       ///< Bit 17 mask of an 32 bit integer
#define BIT18    (0x00040000)       ///< Bit 18 mask of an 32 bit integer
#define BIT19    (0x00080000)       ///< Bit 19 mask of an 32 bit integer
#define BIT20    (0x00100000)       ///< Bit 20 mask of an 32 bit integer
#define BIT21    (0x00200000)       ///< Bit 21 mask of an 32 bit integer
#define BIT22    (0x00400000)       ///< Bit 22 mask of an 32 bit integer
#define BIT23    (0x00800000)       ///< Bit 23 mask of an 32 bit integer
#define BIT24    (0x01000000)       ///< Bit 24 mask of an 32 bit integer
#define BIT25    (0x02000000)       ///< Bit 25 mask of an 32 bit integer
#define BIT26    (0x04000000)       ///< Bit 26 mask of an 32 bit integer
#define BIT27    (0x08000000)       ///< Bit 27 mask of an 32 bit integer
#define BIT28    (0x10000000)       ///< Bit 28 mask of an 32 bit integer
#define BIT29    (0x20000000)       ///< Bit 29 mask of an 32 bit integer
#define BIT30    (0x40000000)       ///< Bit 30 mask of an 32 bit integer
#define BIT31    (0x80000000)       ///< Bit 31 mask of an 32 bit integer

/* Byte Mask Definitions */
#define BYTEMsk                (0x000000FF)         ///< Mask to get bit0~bit7 from a 32 bit integer
#define BYTE1_Msk              (0x0000FF00)         ///< Mask to get bit8~bit15 from a 32 bit integer
#define BYTE2_Msk              (0x00FF0000)         ///< Mask to get bit16~bit23 from a 32 bit integer
#define BYTE3_Msk              (0xFF000000)         ///< Mask to get bit24~bit31 from a 32 bit integer

#define GET_BYTE0(u32Param)    ((u32Param & BYTEMsk)      )    /*!< Extract Byte 0 (Bit  0~ 7) from parameter u32Param */
#define GET_BYTE1(u32Param)    ((u32Param & BYTE1_Msk) >>  8)  /*!< Extract Byte 1 (Bit  8~15) from parameter u32Param */
#define GET_BYTE2(u32Param)    ((u32Param & BYTE2_Msk) >> 16)  /*!< Extract Byte 2 (Bit 16~23) from parameter u32Param */
#define GET_BYTE3(u32Param)    ((u32Param & BYTE3_Msk) >> 24)  /*!< Extract Byte 3 (Bit 24~31) from parameter u32Param */

#define OK                     0x0
#define NOK                    0x1

/*@}*/ /* end of group SKY32B750_legacy_Constants */

/*@}*/ /* end of group SKY32B750_Definitions */

#ifdef __cplusplus
}
#endif

/******************************************************************************/
/*                         Peripheral header files                            */
/******************************************************************************/
#include "system_SKY32B750.h"
#include "dma.h"
#include "usci.h"
#include "pinmux.h"
#include "qspi.h"
#include "wdt.h"
#include "timerA.h"
#include "timerB.h"
#include "gpio.h"
#include "efc.h"
#include "mpu.h"
#include "ospi.h"
#include "i2c.h"
#include "fadc.h"
#include "sadc.h"
#include "rtc.h"
#include "crc.h"
#include "wwdg.h"

#include "delay.h"
#include "retarget.h"
#endif  // __SKY32B750_H__
