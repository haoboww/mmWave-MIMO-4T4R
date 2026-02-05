/******************************************************************************
 * @Filename     : system_SKY32B750.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/01/2022 9:26:12
 * @Modified by  : CB
 *
 * @Brief        : SKY32B750 Peripheral Access Layer System Header File.
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __SYSTEM_SKY32B750_H__
#define __SYSTEM_SKY32B750_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*----------------------------------------------------------------------------
  Define SYSCLK
 *----------------------------------------------------------------------------*/
#define XTAL_40M        40000000               //40MHZ
#define XTAL_50M        50000000               //50MHz
#define XTAL_80M        80000000               //80MHz
#define XTAL_32K        32768                  //32.768KHz

#define LRC_CLK         32768                  //32.768KHz
#define HRC_CLK         93312000               //93.312MHz

#define MAX_PLL_CLK     460000000              //460MHz
#define MIN_PLL_CLK     80000000               //80MHz
#define PLL_CLK         80000000               //80MHz

#define MS_CNT          6997

// XTAL Control
#define HXTAL_ENABLE            AON_SYSC->PWR1 |= AON_PWR1_PU_XTAL_40M_Msk
#define HXTAL_DISABLE           AON_SYSC->PWR1 &= ~AON_PWR1_PU_XTAL_40M_Msk
#define LXTAL_ENABLE            AON_SYSC->PWR1 |= AON_PWR1_PU_XTAL_32K_Msk
#define LXTAL_DISABLE           AON_SYSC->PWR1 &= ~AON_PWR1_PU_XTAL_32K_Msk
#define PLL_ENABLE              AON_SYSC->PWR1 |= AON_PWR1_PU_PLL_Msk
#define PLL_DISABLE             AON_SYSC->PWR1 &= ~AON_PWR1_PU_PLL_Msk

//LRC Control
#define LRC_ENABLE              AON_SYSC->LRC_EN = 0;
#define LRC_DISABLE             AON_SYSC->LRC_EN = PD_LRC_ALL_PATTERN;
#define WDT_DISABLE             AON_SYSC->LRC_EN = PD_LRC_WDT_PATTERN;
#define RTC_DISABLE             AON_SYSC->LRC_EN = PD_LRC_RTC_PATTERN;

// Flash WS Control
#define SET_FLASH_WS_MAX        SYSC->CLK_DIV  |= FLASH_WS_Msk;

// IO Latch Control
#define GPIO_LATCH_ENALBE       (AON_SYSC->IOLATCH |= GPIO_LATCH_Msk)
#define GPIO_LATCH_DISABLE      (AON_SYSC->IOLATCH &= ~GPIO_LATCH_Msk)
#define SWD_LATCH_ENABLE        (AON_SYSC->IOLATCH |= GPIO_LATCH_SWD_Msk)
#define SWD_LATCH_DISABLE       (AON_SYSC->IOLATCH &= ~GPIO_LATCH_SWD_Msk)
#define ALL_IOLATCH_ENABLE      (AON_SYSC->IOLATCH = 0x3)
#define ALL_IOLATCH_DISABLE     (AON_SYSC->IOLATCH = 0x0)

//Peripheral Clock Enable
#define DMA_CLK_ENABLE          (SYSC->CLK_EN |= DMA_EN_Msk      )
#define SRAM1_CLK_ENABLE        (SYSC->CLK_EN |= SRAM1_EN_Msk    )
#define SRAM2_CLK_ENABLE        (SYSC->CLK_EN |= SRAM2_EN_Msk    )
#define SRAM3_CLK_ENABLE        (SYSC->CLK_EN |= SRAM3_EN_Msk    )
#define GPIO_CLK_ENABLE         (SYSC->CLK_EN |= GPIO_EN_Msk     )
#define QSPI0_CLK_ENABLE        (SYSC->CLK_EN |= QSPI0_EN_Msk    )
#define QSPI1_CLK_ENABLE        (SYSC->CLK_EN |= QSPI1_EN_Msk    )
#define I2C_CLK_ENABLE          (SYSC->CLK_EN |= I2C_EN_Msk      )
#define RADAR_BB_CLK_ENABLE     (SYSC->CLK_EN |= RADAR_BB_EN_Msk )
#define CAN0_CLK_ENABLE         (SYSC->CLK_EN |= CAN0_EN_Msk     )
#define CAN1_CLK_ENABLE         (SYSC->CLK_EN |= CAN1_EN_Msk     )
#define TRNG_CLK_ENABLE         (SYSC->CLK_EN |= TRNG_EN_Msk     )
#define WWDG_CLK_ENABLE         (SYSC->CLK_EN |= WWDG_EN_Msk     )
#define ALWAYS_ON_CLK_ENABLE    (SYSC->CLK_EN |= ALWAYS_ON_EN_Msk)
#define FADC_CLK_ENABLE         (SYSC->CLK_EN |= FADC_EN_Msk     )
#define SADC_CLK_ENABLE         (SYSC->CLK_EN |= SADC_EN_Msk     )
#define OSPI0_CLK_ENABLE        (SYSC->CLK_EN |= OSPI0_EN_Msk    )
#define OSPI1_CLK_ENABLE        (SYSC->CLK_EN |= OSPI1_EN_Msk    )
#define USCI7_CLK_ENABLE        (SYSC->CLK_EN |= USCI7_EN_Msk    )
#define USCI6_CLK_ENABLE        (SYSC->CLK_EN |= USCI6_EN_Msk    )
#define USCI5_CLK_ENABLE        (SYSC->CLK_EN |= USCI5_EN_Msk    )
#define USCI4_CLK_ENABLE        (SYSC->CLK_EN |= USCI4_EN_Msk    )
#define USCI3_CLK_ENABLE        (SYSC->CLK_EN |= USCI3_EN_Msk    )
#define USCI2_CLK_ENABLE        (SYSC->CLK_EN |= USCI2_EN_Msk    )
#define USCI1_CLK_ENABLE        (SYSC->CLK_EN |= USCI1_EN_Msk    )
#define USCI0_CLK_ENABLE        (SYSC->CLK_EN |= USCI0_EN_Msk    )
#define TMRA_CLK_ENABLE         (SYSC->CLK_EN |= TMRA_EN_Msk     )
#define TMRB_CLK_ENABLE         (SYSC->CLK_EN |= TMRB_EN_CLK_Msk )
#define CRC_CLK_ENABLE          (SYSC->CLK_EN |= CRC_EN_Msk      )
#define PMUX_CLK_ENABLE         (SYSC->CLK_EN |= PMUX_EN_Msk     )

//Peripheral Clock Disable
#define DMA_CLK_DISABLE         (SYSC->CLK_EN &= ~DMA_EN_Msk      )
#define SRAM1_CLK_DISABLE       (SYSC->CLK_EN &= ~SRAM1_EN_Msk    )
#define SRAM2_CLK_DISABLE       (SYSC->CLK_EN &= ~SRAM2_EN_Msk    )
#define SRAM3_CLK_DISABLE       (SYSC->CLK_EN &= ~SRAM3_EN_Msk    )
#define GPIO_CLK_DISABLE        (SYSC->CLK_EN &= ~GPIO_EN_Msk     )
#define QSPI0_CLK_DISABLE       (SYSC->CLK_EN &= ~QSPI0_EN_Msk    )
#define QSPI1_CLK_DISABLE       (SYSC->CLK_EN &= ~QSPI1_EN_Msk    )
#define I2C_CLK_DISABLE         (SYSC->CLK_EN &= ~I2C_EN_Msk      )
#define RADAR_BB_CLK_DISABLE    (SYSC->CLK_EN &= ~RADAR_BB_EN_Msk )
#define CAN0_CLK_DISABLE        (SYSC->CLK_EN &= ~CAN0_EN_Msk     )
#define CAN1_CLK_DISABLE        (SYSC->CLK_EN &= ~CAN1_EN_Msk     )
#define TRNG_CLK_DISABLE        (SYSC->CLK_EN &= ~TRNG_EN_Msk     )
#define WWDG_CLK_DISABLE        (SYSC->CLK_EN &= ~WWDG_EN_Msk     )
#define ALWAYS_ON_CLK_DISABLE   (SYSC->CLK_EN &= ~ALWAYS_ON_EN_Msk)
#define FADC_CLK_DISABLE        (SYSC->CLK_EN &= ~FADC_EN_Msk     )
#define SADC_CLK_DISABLE        (SYSC->CLK_EN &= ~SADC_EN_Msk     )
#define OSPI0_CLK_DISABLE       (SYSC->CLK_EN &= ~OSPI0_EN_Msk    )
#define OSPI1_CLK_DISABLE       (SYSC->CLK_EN &= ~OSPI1_EN_Msk    )
#define USCI7_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI7_EN_Msk    )
#define USCI6_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI6_EN_Msk    )
#define USCI5_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI5_EN_Msk    )
#define USCI4_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI4_EN_Msk    )
#define USCI3_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI3_EN_Msk    )
#define USCI2_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI2_EN_Msk    )
#define USCI1_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI1_EN_Msk    )
#define USCI0_CLK_DISABLE       (SYSC->CLK_EN &= ~USCI0_EN_Msk    )
#define TMRA_CLK_DISABLE        (SYSC->CLK_EN &= ~TMRA_EN_Msk     )
#define TMRB_CLK_DISABLE        (SYSC->CLK_EN &= ~TMRB_EN_CLK_Msk )
#define CRC_CLK_DISABLE         (SYSC->CLK_EN &= ~CRC_EN_Msk      )
#define PMUX_CLK_DISABLE        (SYSC->CLK_EN &= ~PMUX_EN_Msk     )

#define SWD_PORT_ENABLE         (SYSC->SWD_PEN |= SWD_PEN_Msk     )
#define SWD_PORT_DISABLE        (SYSC->SWD_PEN &= ~SWD_PEN_Msk    )

#define FADC_GRP0_LDO_ENABLE    (AON_SYSC->ANA_CFG1 |= ANA_CFG1_FADC_GRP0_LDO_Msk)
#define FADC_GRP1_LDO_ENABLE    (AON_SYSC->ANA_CFG1 |= ANA_CFG1_FADC_GRP1_LDO_Msk)
#define FADC_GRP0_LDO_DISABLE   (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_FADC_GRP0_LDO_Msk)
#define FADC_GRP1_LDO_DISABLE   (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_FADC_GRP1_LDO_Msk)
#define FADC_ALL_LDO_ENABLE     (AON_SYSC->ANA_CFG1 |= ANA_CFG1_FADC_ALL_LDO_Msk)
#define FADC_ALL_LDO_DISABLE    (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_FADC_ALL_LDO_Msk)

// Get System Clock Configurations
#define GET_HXTAL_PU_STATE      AON_SYSC->PWR1 & AON_PWR1_PU_XTAL_40M_Msk ? 1 : 0
#define GET_LXTAL_PU_STATE      AON_SYSC->PWR1 & AON_PWR1_PU_XTAL_32K_Msk ? 1 : 0
#define GET_PLL_PU_STATE        AON_SYSC->PWR1 & AON_PWR1_PU_PLL_Msk ? 1 : 0
#define GET_PLL_RDY_STATE       SYSC->STA & PLL_RDY_Msk ? 1 : 0
#define GET_LRC_PU_STATE        AON_SYSC->PWR1 & AON_PWR1_PU_LRC_STA_Msk ? 1 : 0
#define GET_WDT_EN_STATE        WDT->STR & WDT_EN_STA_Msk ? 1 : 0
#define GET_RTC_EN_STATE        RTC->STA & RTC_EN_STA_Msk ? 1 : 0
#define GET_WDT_CMP             (WDT->CMP & WDT_CMP_Msk) >> WDT_CMP_Pos
#define GET_RTC_CMP             (RTC->WKU_CMP & RTC_WKU_CMP_Msk) >> RTC_WKU_CMP_Pos
#define GET_FLASH_WS_CFG        ((SYSC->CLK_DIV & FLASH_WS_Msk) >> FLASH_WS_Pos)
#define GET_PLL_PREDIV_CFG      ((AON_SYSC->ANA_CFG0 & ANA_CFG0_PREDIV_Pos) >> ANA_CFG0_PREDIV_Pos)
#define GET_PLL_MDIV_CFG        ((AON_SYSC->ANA_CFG0 & ANA_CFG0_MDIV_CFG_Pos) >> ANA_CFG0_MDIV_CFG_Pos)
#define GET_PLL_POSDIV_CFG      ((AON_SYSC->ANA_CFG1 & ANA_CFG1_PLL_POST_DIV_Msk) >> ANA_CFG1_PLL_POST_DIV_Pos)
#define GET_AHB_DIV_CFG         ((SYSC->CLK_DIV & AHB_CLK_DIV_Msk) >> AHB_CLK_DIV_Pos)
#define GET_APB_DIV_CFG         ((SYSC->CLK_DIV & APB_CLK_DIV_Msk) >> APB_CLK_DIV_Pos)
#define GET_CAN_DIV_CFG         ((SYSC->CLK_DIV & CAN_CLK_DIV_Msk) >> CAN_CLK_DIV_Pos)

#define GET_CHIP_ID             (SYSC->CHIP_ID)
#define GET_REG_LOCK_STATE      (AON_SYSC->RWRP & AON_RWRP_STA_Msk ? 0 : 1)

#define GET_WDT_RST_STATE       (AON_SYSC->STA & WDT_RST_STA_Msk ? 1 : 0)
#define CLR_WDT_RST_STATE       (AON_SYSC->STA = WDT_RST_STA_Msk)
#define GET_PIN_RST_STATE       (AON_SYSC->STA & RST_N_STA_Msk ? 1 : 0)
#define CLR_PIN_RST_STATE       (AON_SYSC->STA = RST_N_STA_Msk)
#define GET_POR_RST_STATE       (AON_SYSC->STA & POR_LV_STA_Msk ? 1 : 0)
#define CLR_POR_RST_STATE       (AON_SYSC->STA = POR_LV_STA_Msk)

/** @brief  select register to be sync generator*/
#define SEL_REG_GEN_SYNC        (AON_SYSC->ANA_CFG1 |= ANA_CFG1_SEL_R_SYNC_GEN_Msk)

/** @brief  select pad to be sync generator*/
#define SEL_PAD_GEN_SYNC        (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_SEL_R_SYNC_GEN_Msk)

/** @brief  register generate high sync signal*/
#define REG_GEN_SYNC_HIGH       (AON_SYSC->ANA_CFG1 |= ANA_CFG1_R_GEN_SYNC_Msk)

/** @brief  register generate low sync signal*/
#define REG_GEN_SYNC_LOW        (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_R_GEN_SYNC_Msk)

/** @brief  select gpio1.0 to be sync generator*/
#define SEL_GPIO1_0_GEN_SYNC    (AON_SYSC->ANA_CFG1 |= ANA_CFG1_SEL_PAD_GEN_SYNC_Msk)

/** @brief  select gpio3.0 to be sync generator*/
#define SEL_GPIO3_0_GEN_SYNC    (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_SEL_PAD_GEN_SYNC_Msk)

/** Power Management */
#define SRAM1_PWR_ON            (AON_SYSC->PWR0 |= AON_PU_SRAM1_PWR_Msk)
#define SRAM1_PWR_OFF           (AON_SYSC->PWR0 &= ~AON_PU_SRAM1_PWR_Msk)
#define SRAM2_PWR_ON            (AON_SYSC->PWR0 |= AON_PU_SRAM2_PWR_Msk)
#define SRAM2_PWR_OFF           (AON_SYSC->PWR0 &= ~AON_PU_SRAM2_PWR_Msk)
#define SRAM3_PWR_ON            (AON_SYSC->PWR0 |= AON_PU_SRAM3_PWR_Msk)
#define SRAM3_PWR_OFF           (AON_SYSC->PWR0 &= ~AON_PU_SRAM3_PWR_Msk)
#define SRAM_LP_PWR_ON          (AON_SYSC->PWR0 |= AON_PU_SRAM_LP_PWR_Msk)
#define SRAM_LP_PWR_OFF         (AON_SYSC->PWR0 &= ~AON_PU_SRAM_LP_PWR_Msk)
// Power Mode Control
#define DS_MODE_ENABLE          AON_SYSC->PWR0 |= AON_DEEP_MODE_Msk
#define PD_MODE_ENABLE          AON_SYSC->PWR0 &= ~AON_DEEP_MODE_Msk

/** Wakeup Source Management */
#define GPIO0_0_WKU_MSK         AON_WKU_IO_WKUP_MASK0_Msk
#define GPIO0_1_WKU_MSK         AON_WKU_IO_WKUP_MASK1_Msk
#define GPIO0_2_WKU_MSK         AON_WKU_IO_WKUP_MASK2_Msk
#define GPIO0_3_WKU_MSK         AON_WKU_IO_WKUP_MASK3_Msk
#define GPIO0_4_WKU_MSK         AON_WKU_IO_WKUP_MASK4_Msk
#define GPIO0_5_WKU_MSK         AON_WKU_IO_WKUP_MASK5_Msk
#define GPIO0_6_WKU_MSK         AON_WKU_IO_WKUP_MASK6_Msk
#define GPIO0_7_WKU_MSK         AON_WKU_IO_WKUP_MASK7_Msk
#define GPIO0_8_WKU_MSK         AON_WKU_IO_WKUP_MASK8_Msk
#define GPIO0_9_WKU_MSK         AON_WKU_IO_WKUP_MASK9_Msk
#define GPIO0_10_WKU_MSK        AON_WKU_IO_WKUP_MASK10_Msk
#define GPIO0_11_WKU_MSK        AON_WKU_IO_WKUP_MASK11_Msk
#define GPIO0_12_WKU_MSK        AON_WKU_IO_WKUP_MASK12_Msk
#define GPIO0_13_WKU_MSK        AON_WKU_IO_WKUP_MASK13_Msk
#define GPIO0_14_WKU_MSK        AON_WKU_IO_WKUP_MASK14_Msk
#define GPIO0_15_WKU_MSK        AON_WKU_IO_WKUP_MASK15_Msk
#define GPIO1_0_WKU_MSK         AON_WKU_IO_WKUP_MASK16_Msk
#define GPIO1_1_WKU_MSK         AON_WKU_IO_WKUP_MASK17_Msk
#define GPIO1_2_WKU_MSK         AON_WKU_IO_WKUP_MASK18_Msk
#define GPIO1_3_WKU_MSK         AON_WKU_IO_WKUP_MASK19_Msk
#define GPIO1_4_WKU_MSK         AON_WKU_IO_WKUP_MASK20_Msk
#define GPIO1_5_WKU_MSK         AON_WKU_IO_WKUP_MASK21_Msk
#define GPIO1_6_WKU_MSK         AON_WKU_IO_WKUP_MASK22_Msk
#define GPIO1_7_WKU_MSK         AON_WKU_IO_WKUP_MASK23_Msk
#define GPIO1_8_WKU_MSK         AON_WKU_IO_WKUP_MASK24_Msk
#define GPIO1_9_WKU_MSK         AON_WKU_IO_WKUP_MASK25_Msk
#define GPIO1_10_WKU_MSK        AON_WKU_IO_WKUP_MASK26_Msk
#define GPIO1_11_WKU_MSK        AON_WKU_IO_WKUP_MASK27_Msk
#define GPIO1_12_WKU_MSK        AON_WKU_IO_WKUP_MASK28_Msk
#define GPIO1_13_WKU_MSK        AON_WKU_IO_WKUP_MASK29_Msk
#define GPIO1_14_WKU_MSK        AON_WKU_IO_WKUP_MASK30_Msk
#define GPIO1_15_WKU_MSK        AON_WKU_IO_WKUP_MASK31_Msk

#define GPIO2_0_WKU_MSK         AON_WKU_IO_WKUP_MASK0_Msk
#define GPIO2_1_WKU_MSK         AON_WKU_IO_WKUP_MASK1_Msk
#define GPIO2_2_WKU_MSK         AON_WKU_IO_WKUP_MASK2_Msk
#define GPIO2_3_WKU_MSK         AON_WKU_IO_WKUP_MASK3_Msk
#define GPIO2_4_WKU_MSK         AON_WKU_IO_WKUP_MASK4_Msk
#define GPIO2_5_WKU_MSK         AON_WKU_IO_WKUP_MASK5_Msk
#define GPIO2_6_WKU_MSK         AON_WKU_IO_WKUP_MASK6_Msk
#define GPIO2_7_WKU_MSK         AON_WKU_IO_WKUP_MASK7_Msk
#define GPIO2_8_WKU_MSK         AON_WKU_IO_WKUP_MASK8_Msk
#define GPIO2_9_WKU_MSK         AON_WKU_IO_WKUP_MASK9_Msk
#define GPIO2_10_WKU_MSK        AON_WKU_IO_WKUP_MASK10_Msk
#define GPIO2_11_WKU_MSK        AON_WKU_IO_WKUP_MASK11_Msk
#define GPIO2_12_WKU_MSK        AON_WKU_IO_WKUP_MASK12_Msk
#define GPIO2_13_WKU_MSK        AON_WKU_IO_WKUP_MASK13_Msk
#define GPIO2_14_WKU_MSK        AON_WKU_IO_WKUP_MASK14_Msk
#define GPIO2_15_WKU_MSK        AON_WKU_IO_WKUP_MASK15_Msk
#define GPIO3_0_WKU_MSK         AON_WKU_IO_WKUP_MASK16_Msk
#define GPIO3_1_WKU_MSK         AON_WKU_IO_WKUP_MASK17_Msk
#define GPIO3_2_WKU_MSK         AON_WKU_IO_WKUP_MASK18_Msk
#define GPIO3_3_WKU_MSK         AON_WKU_IO_WKUP_MASK19_Msk
#define GPIO3_4_WKU_MSK         AON_WKU_IO_WKUP_MASK20_Msk
#define GPIO3_5_WKU_MSK         AON_WKU_IO_WKUP_MASK21_Msk
#define GPIO3_6_WKU_MSK         AON_WKU_IO_WKUP_MASK22_Msk
#define GPIO3_7_WKU_MSK         AON_WKU_IO_WKUP_MASK23_Msk
#define GPIO3_8_WKU_MSK         AON_WKU_IO_WKUP_MASK24_Msk
#define GPIO3_9_WKU_MSK         AON_WKU_IO_WKUP_MASK25_Msk
#define GPIO3_10_WKU_MSK        AON_WKU_IO_WKUP_MASK26_Msk
#define GPIO3_11_WKU_MSK        AON_WKU_IO_WKUP_MASK27_Msk
#define GPIO3_12_WKU_MSK        AON_WKU_IO_WKUP_MASK28_Msk
#define GPIO3_13_WKU_MSK        AON_WKU_IO_WKUP_MASK29_Msk
#define GPIO3_14_WKU_MSK        AON_WKU_IO_WKUP_MASK30_Msk
#define GPIO3_15_WKU_MSK        AON_WKU_IO_WKUP_MASK31_Msk

#define SET_WKUP_MASK0(msk)     AON_WKU->WKUP_MASK0 |= msk
#define CLR_WKUP_MASK0(msk)     AON_WKU->WKUP_MASK0 &= ~msk
#define SET_WKUN_MASK0(msk)     AON_WKU->WKUN_MASK0 |= msk
#define CLR_WKUN_MASK0(msk)     AON_WKU->WKUN_MASK0 &= ~msk

#define SET_WKUP_MASK1(msk)     AON_WKU->WKUP_MASK1 |= msk
#define CLR_WKUP_MASK1(msk)     AON_WKU->WKUP_MASK1 &= ~msk
#define SET_WKUN_MASK1(msk)     AON_WKU->WKUN_MASK1 |= msk
#define CLR_WKUN_MASK1(msk)     AON_WKU->WKUN_MASK1 &= ~msk

#define SET_WKUP_MASK2(msk)     AON_WKU->WKUP_MASK2 |= msk
#define CLR_WKUP_MASK2(msk)     AON_WKU->WKUP_MASK2 &= ~msk

#define GET_SYSTICK_STATE       ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) ? 1 : 0)
#define SYSTICK_ENABLE          SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk   //enable systick
#define SYSTICK_DISABLE         SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk  //disable systick

/**
 * @brief System Clock Configuration Structure Definition
 */
typedef struct
{
    uint32_t hxtal_freq;
    uint8_t flash_ws;
    uint8_t prediv;
    uint8_t mdiv;
    uint8_t posdiv;
    uint8_t ahbdiv;
    uint8_t apbdiv;
    uint8_t candiv;
    uint8_t hxtal_sta;
    uint8_t lxtal_sta;
    uint8_t pll_sta;
    uint32_t lrc_cfg;
    uint16_t wdt_cmp;
    uint16_t rtc_cmp;
} sysclk_config_t;

extern volatile uint32_t sys_clk;
extern volatile uint32_t hclk   ;
extern volatile uint32_t pclk   ;
extern volatile uint32_t pll_clk;
extern volatile uint32_t lrc_clk;
extern volatile uint32_t hrc_clk;
extern volatile uint32_t can_clk;
extern volatile uint32_t ms_step;

extern volatile uint32_t WKU_FLAG_sta0;
extern volatile uint32_t WKU_FLAG_sta1;
extern volatile uint32_t WKU_FLAG_sta2;

uint32_t Set_PLL(uint32_t hxtal_freq, uint8_t prediv, uint8_t mdiv, uint8_t posdiv);
void Set_HRC(uint8_t ibias, uint8_t cap, uint8_t div);
void Set_SysClk_Div(uint8_t ahb_div, uint8_t apb_div);
void Set_CanClk_Div(uint8_t div);
void PLL_System_Clock_Update(void);
void HRC_System_Clock_Update(void);
void Set_Flash_WS(uint8_t ws);
void Get_SysClk_Config(uint32_t hxtal_freq, sysclk_config_t *clk_cfg);
void Set_SysClk_Config(sysclk_config_t *clk_cfg);
uint32_t HXTAL_Caliber_LRC(uint32_t hxtal_freq, uint32_t lrc_step);
uint32_t LRC_Caliber_HRC(uint32_t hxtal_freq, uint32_t lrc_step);
uint8_t Register_Lock(void);
uint8_t Register_UnLock(void);
void Set_BOR_Threshold(uint8_t thres);
void Set_CoreLDO_Voltage(uint8_t val);
void Set_LPLDO_Voltage(uint8_t val);
void Cache_Enable(void);
void Cache_Disable(void);
void Cache_Invalidate(void);
uint8_t FPU_Enable(void);
uint8_t FPU_Disable(void);
void Peripheral_Reset(uint32_t peripheral);
void SystemInit (void);
void Deep_Sleep(void);
void Power_Down(void);
void CPU_Sleep(void);

#ifdef __cplusplus
}
#endif

#endif   //__SYSTEM_SKY32B750_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
