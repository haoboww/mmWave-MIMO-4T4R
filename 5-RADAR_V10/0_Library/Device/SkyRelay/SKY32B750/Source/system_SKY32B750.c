/******************************************************************************
 * @Filename     : system_SKY32B750.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/01/2022 9:12:34
 * @Modified by  : CB
 *
 * @Brief        : SKY32B750 Peripheral Access Layer System Source File.
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "SKY32B750.h"

//=====================================================================================//
// Global shared variables Definition                                                  //
//=====================================================================================//
volatile uint32_t sys_clk = (HRC_CLK);        //Default 93.3MHz
volatile uint32_t hclk    = (HRC_CLK / 8);    //Default 11.6MHz
volatile uint32_t pclk    = (HRC_CLK / 16);   //Default 5.8MHz
volatile uint32_t pll_clk = (PLL_CLK);        //Default 80MHz
volatile uint32_t lrc_clk = (LRC_CLK);        //Default 32.768KHz
volatile uint32_t hrc_clk = (HRC_CLK);        //Default 93.3MHz
volatile uint32_t can_clk = (HRC_CLK / 12);   //Default 7.7MHz
volatile uint32_t ms_step = (HRC_CLK / MS_CNT);

volatile uint32_t WKU_FLAG_sta0 = 0;
volatile uint32_t WKU_FLAG_sta1 = 0;
volatile uint32_t WKU_FLAG_sta2 = 0;

/**
 * @brief     Config PLL output frequency
 * @DateTime  2022-05-02T10:44:42+0800
 * @param     hxtal_freq                External high speed crystal frequency in Hz
 * @param     prediv                    pre div cfg, default 0, 0: 40MHz input;
 *                                                              1: 80MHz input;
 * @param     mdiv                      mdiv cfg, default 0000, 0000: 8;
 *                                                              1111: 23;
 * @param     posdiv                    post div cfg, default 1, 0: div2;
 *                                                               1: div4;
 * @return                              PLL output frequency in Hz
 *                                      = (((hxtal_freq / (1 + prediv)) * (8 + mdiv)) / (2 + posdiv * 2))
 */
uint32_t Set_PLL(uint32_t hxtal_freq, uint8_t prediv, uint8_t mdiv, uint8_t posdiv)
{
    AON_SYSC->ANA_CFG0 = ((AON_SYSC->ANA_CFG0 & (~ANA_CFG0_PREDIV_Msk) & (~ANA_CFG0_MDIV_CFG_Msk)) | \
                          (prediv << ANA_CFG0_PREDIV_Pos) |                                          \
                          (mdiv << ANA_CFG0_MDIV_CFG_Pos));
    AON_SYSC->ANA_CFG1 = ((AON_SYSC->ANA_CFG1 & ~ANA_CFG1_PLL_POST_DIV_Msk) |                        \
                          (posdiv << ANA_CFG1_PLL_POST_DIV_Pos));
    pll_clk = (((hxtal_freq / (1 + prediv)) * (8 + mdiv)) / (2 + posdiv * 2));
    return pll_clk;
}

/**
 * @brief     Config HRC frequency
 * @DateTime  2022-05-02T11:17:51+0800
 * @param     ibias                     ibias cfg, default 000, 000: min ibias;
 *                                                              xxx: mid ibias;
 *                                                              111: max ibias;
 * @param     cap                       cap cfg, default 0, 0: add cap;
 *                                                          1: cut cap;
 * @param     div                       div cfg, default 0, 0: div4;
 *                                                          1: div2;
 */
void Set_HRC(uint8_t ibias, uint8_t cap, uint8_t div)
{
    AON_SYSC->ANA_CFG0 = ((AON_SYSC->ANA_CFG0 & (~ANA_CFG0_HRC_IBIAS_CFG_Msk) &                    \
                           (~ANA_CFG0_HRC_CAP_Msk) & (~ANA_CFG0_HRC_DIV2_Msk)) |                   \
                          ((ibias << ANA_CFG0_HRC_IBIAS_CFG_Pos) & ANA_CFG0_HRC_IBIAS_CFG_Msk) |  \
                          ((cap << ANA_CFG0_HRC_CAP_Pos) & ANA_CFG0_HRC_CAP_Msk)       |          \
                          ((div << ANA_CFG0_HRC_DIV2_Pos) & ANA_CFG0_HRC_DIV2_Msk));
}

/**
 * @brief     Config AHB and APB clock divider
 * @DateTime  2022-05-02T11:22:00+0800
 * @param     ahb_div                   ahb div cfg, default 4, 0     : hclk = sys_clk,
 *                                                              others: hclk = sys_clk/(2*ahb_div)
 * @param     apb_div                   apb div cfg, default 1, 0     : pclk = hclk/2,
 *                                                              others: pclk = hclk/(2*apb_div)
 */
void Set_SysClk_Div(uint8_t ahb_div, uint8_t apb_div)
{
    SYSC->CLK_DIV = ((SYSC->CLK_DIV & (~AHB_CLK_DIV_Msk) & (~APB_CLK_DIV_Msk)) | \
                     (ahb_div << AHB_CLK_DIV_Pos) |                               \
                     (apb_div << APB_CLK_DIV_Pos));
}

/**
 * @brief
 * @DateTime  2022-05-03T14:58:52+0800
 * @param     div                       div, default 6, 0      : can_clk = sys_clk/2,
 *                                                      others : can_clk = sys_clk/(2*div)
 */
void Set_CanClk_Div(uint8_t div)
{
    SYSC->CLK_DIV = ((SYSC->CLK_DIV & (~CAN_CLK_DIV_Msk)) | (div << CAN_CLK_DIV_Pos));
}

/**
 * @brief     Use PLL as system clock and update sys_clk, hclk, pclk
 * @DateTime  2022-05-02T13:25:04+0800
 */
void PLL_System_Clock_Update(void)
{
    sys_clk = pll_clk;
    hclk    = ((SYSC->CLK_DIV & AHB_CLK_DIV_Msk) == 0) ? (hclk = sys_clk) :  \
              (sys_clk / (2 * ((SYSC->CLK_DIV & AHB_CLK_DIV_Msk) >> AHB_CLK_DIV_Pos)));
    pclk    = ((SYSC->CLK_DIV & APB_CLK_DIV_Msk) == 0) ? (pclk = hclk / 2) : \
              (hclk / (2 * ((SYSC->CLK_DIV & APB_CLK_DIV_Msk) >> APB_CLK_DIV_Pos)));
    can_clk = ((SYSC->CLK_DIV & CAN_CLK_DIV_Msk) == 0) ? (can_clk = sys_clk / 2) :  \
              (sys_clk / (2 * ((SYSC->CLK_DIV & CAN_CLK_DIV_Msk) >> CAN_CLK_DIV_Pos)));
    ms_step = hclk / MS_CNT;
}

/**
 * @brief     Use HRC as system clock and update sys_clk, hclk, pclk
 * @DateTime  2022-05-02T13:27:13+0800
 */
void HRC_System_Clock_Update(void)
{
    sys_clk = hrc_clk;
    hclk    = ((SYSC->CLK_DIV & AHB_CLK_DIV_Msk) == 0) ? (hclk = sys_clk) :  \
              (sys_clk / (2 * ((SYSC->CLK_DIV & AHB_CLK_DIV_Msk) >> AHB_CLK_DIV_Pos)));
    pclk    = ((SYSC->CLK_DIV & APB_CLK_DIV_Msk) == 0) ? (pclk = hclk / 2) : \
              (hclk / (2 * ((SYSC->CLK_DIV & APB_CLK_DIV_Msk) >> APB_CLK_DIV_Pos)));
    can_clk = ((SYSC->CLK_DIV & CAN_CLK_DIV_Msk) == 0) ? (can_clk = sys_clk / 2) :  \
              (sys_clk / (2 * ((SYSC->CLK_DIV & CAN_CLK_DIV_Msk) >> CAN_CLK_DIV_Pos)));
    ms_step = hclk / MS_CNT;
}

/**
 * @brief     Set wait time before access flash
 * @DateTime  2022-05-02T13:27:57+0800
 * @param     ws                        wait [ws] hclk, default 0000
 */
void Set_Flash_WS(uint8_t ws)
{
    SYSC->CLK_DIV = ((SYSC->CLK_DIV & ~FLASH_WS_Msk) | (ws << FLASH_WS_Pos));
}

/**
 * @brief     Get all system configurations
 * @DateTime  2022-05-02T13:32:13+0800
 * @param     hxtal_freq                External high speed crystal frequency in Hz
 * @param     clk_cfg                   Pointer to sysclk_config_t
 */
void Get_SysClk_Config(uint32_t hxtal_freq, sysclk_config_t *clk_cfg)
{
    clk_cfg->hxtal_freq = hxtal_freq;
    clk_cfg->flash_ws   = GET_FLASH_WS_CFG;
    clk_cfg->prediv     = GET_PLL_PREDIV_CFG;
    clk_cfg->mdiv       = GET_PLL_MDIV_CFG;
    clk_cfg->posdiv     = GET_PLL_POSDIV_CFG;
    clk_cfg->ahbdiv     = GET_AHB_DIV_CFG;
    clk_cfg->candiv     = GET_CAN_DIV_CFG;
    clk_cfg->apbdiv     = GET_APB_DIV_CFG;
    clk_cfg->hxtal_sta  = GET_HXTAL_PU_STATE;
    clk_cfg->lxtal_sta  = GET_LXTAL_PU_STATE;
    clk_cfg->pll_sta    = GET_PLL_PU_STATE;
    clk_cfg->lrc_cfg    = AON_SYSC->LRC_EN;
    clk_cfg->wdt_cmp    = GET_WDT_CMP;
    clk_cfg->rtc_cmp    = GET_RTC_CMP;
}

/**
 * @brief     Set all system configurations
 * @DateTime  2022-05-02T13:36:11+0800
 * @param     clk_cfg                   Pointer to sysclk_config_t
 */
void Set_SysClk_Config(sysclk_config_t *clk_cfg)
{
    Set_Flash_WS(clk_cfg->flash_ws);
    Set_PLL(clk_cfg->hxtal_freq, clk_cfg->prediv, clk_cfg->mdiv, clk_cfg->posdiv);
    Set_CanClk_Div(clk_cfg->candiv);
    (clk_cfg->hxtal_sta == 1) ? (HXTAL_ENABLE) : (HXTAL_DISABLE);
    (clk_cfg->lxtal_sta == 1) ? (LXTAL_ENABLE) : (LXTAL_DISABLE);
    (clk_cfg->pll_sta   == 1) ? (PLL_ENABLE)   : (PLL_DISABLE);
    AON_SYSC->LRC_EN = clk_cfg->lrc_cfg;
    WDT->CMP = clk_cfg->wdt_cmp;
    RTC->WKU_CMP = clk_cfg->rtc_cmp;
    Set_SysClk_Div(clk_cfg->ahbdiv, clk_cfg->apbdiv);
}

/**
 * @brief     Calibrate LRC with high speed crystal
 * @DateTime  2022-05-02T15:04:12+0800
 * @param     hxtal_freq                External high speed crystal frequency in Hz
 * @param     lrc_step                  Number of LRC cycles in calibration
 * @return                              LRC frequency in Hz
 */
uint32_t HXTAL_Caliber_LRC(uint32_t hxtal_freq, uint32_t lrc_step)
{
    sysclk_config_t clk_cfg;
    uint32_t pll_freq = 0;
    uint32_t cnt_val = 0;
    uint32_t loop_cnt = 0;
    uint32_t ret_val = 0;
    uint64_t long_val;

    Get_SysClk_Config(hxtal_freq, &clk_cfg);
    Set_Flash_WS(0xF);
    if (hxtal_freq > XTAL_50M)
        pll_freq = Set_PLL(hxtal_freq, 1, 0, 1);
    else
        pll_freq = Set_PLL(hxtal_freq, 0, 0, 1);
    HXTAL_ENABLE;
    PLL_ENABLE;
    Set_SysClk_Div(0, 3);

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //Stop SysTick
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = 0;
    WDT_Open(lrc_step);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while (!WDT_GET_INT_FLAG)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        {
            loop_cnt++;
        }
    }
    cnt_val = SysTick->VAL;
    WDT_CLEAR_COUNTER;
    long_val = (uint64_t)pll_freq * 16 * lrc_step;
    ret_val = long_val / ((0xffffff - cnt_val) + 1 + loop_cnt * 0xffffff);
    Set_SysClk_Config(&clk_cfg);
    lrc_clk = ret_val;
    return ret_val;
}

/**
 * @brief     Calibrate HRC with LRC
 * @note      LRC must be calibrated first
 * @DateTime  2022-05-02T15:08:19+0800
 * @param     hxtal_freq                External high speed crystal frequency in Hz
 * @param     lrc_step                  Number of LRC cycles in calibration
 * @return                              HRC frequency in Hz
 */
uint32_t LRC_Caliber_HRC(uint32_t hxtal_freq, uint32_t lrc_step)
{
    sysclk_config_t clk_cfg;
    uint32_t cnt_val = 0;
    uint32_t loop_cnt = 0;
    uint32_t ret_val = 0;
    uint64_t long_val;

    Get_SysClk_Config(hxtal_freq, &clk_cfg);

    PLL_DISABLE;
    HXTAL_DISABLE;
    Set_SysClk_Div(0, 3);

    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //Stop SysTick
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = 0;
    WDT_Open(lrc_step);
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
    while (!WDT_GET_INT_FLAG)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        {
            loop_cnt++;
        }
    }
    cnt_val = SysTick->VAL;
    WDT_CLEAR_COUNTER;
    long_val = (uint64_t)lrc_clk * ((0xffffff - cnt_val) + 1 + loop_cnt * 0xffffff);

    ret_val = long_val / (16 * lrc_step);
    Set_SysClk_Config(&clk_cfg);
    hrc_clk = ret_val;
    return ret_val;
}

//uint32_t LXTAL_Caliber_HRC(uint32_t hxtal_freq, uint32_t lxtal_step)
//{
//}

/**
 * @brief     Enable the register writing protection function
 * @DateTime  2022-05-03T15:45:23+0800
 * @return                              0: Register Lock Successfully
 *                                      1: Register Lock Failure
 */
uint8_t Register_Lock(void)
{
    AON_SYSC->RWRP = 0x0;
    if (GET_REG_LOCK_STATE == 1) return OK; else return NOK;
}

/**
 * @brief     Disable the register writing protection function
 * @DateTime  2022-05-03T15:46:13+0800
 * @return                              0: Register UnLock Successfully
 *                                      1: Register UnLock Failure
 */
uint8_t Register_UnLock(void)
{
    AON_SYSC->RWRP = 0x5A;
    AON_SYSC->RWRP = 0x26;
    AON_SYSC->RWRP = 0x88;
    if (GET_REG_LOCK_STATE == 0) return OK; else return NOK;
}

/**
 * @brief     BOR threshold selection
 * @DateTime  2022-05-03T21:55:06+0800
 * @param     thres                     BOR Threshold, default 100:      about 2.3V
 *                                                             000:      min threshold about 1.9V
 *                                                             111:      max threshold about 2.6V
 */
void Set_BOR_Threshold(uint8_t thres)
{
    AON_SYSC->ANA_CFG1 = (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_BOR_THRESHOLD_Msk) | (thres << ANA_CFG1_BOR_THRESHOLD_Pos);
}

/**
 * @brief     LDO_CORE output control
 * @DateTime  2022-05-03T22:00:03+0800
 * @param     val                       Core LDO Voltage, default 10:   about 1.1V
 *                                                                00:   min voltage about 0.9V
 *                                                                11:   max voltage about 1.2V
 */
void Set_CoreLDO_Voltage(uint8_t val)
{
    AON_SYSC->ANA_CFG1 = (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_LDO_CORE_OUTPUT_Msk) | (val << ANA_CFG1_LDO_CORE_OUTPUT_Pos);
}

/**
 * @brief     LDO_LP output control
 * @DateTime  2022-05-03T22:03:25+0800
 * @param     val                       LP LDO Voltage, default 10:     about 0.9V
 *                                                              00:     max voltage about 1.0V
 *                                                              11:     min voltage about 0.84V
 */
void Set_LPLDO_Voltage(uint8_t val)
{
    AON_SYSC->ANA_CFG1 = (AON_SYSC->ANA_CFG1 &= ~ANA_CFG1_LDO_LP_OUTPUT_Msk) | (val << ANA_CFG1_LDO_LP_OUTPUT_Pos);
}

/**
 * @brief     Enable icache & dcache
 * @DateTime  2022-05-02T15:13:22+0800
 */
void Cache_Enable(void)
{
    SCB_EnableICache();
    SCB_EnableDCache();
}

/**
 * @brief     Disable icache & dcache
 * @DateTime  2022-05-02T15:14:00+0800
 */
void Cache_Disable(void)
{
    SCB_DisableICache();
    SCB_DisableDCache();
}

/**
 * @brief     Invalidate icache & dcache
 * @DateTime  2022-05-02T15:14:36+0800
 */
void Cache_Invalidate(void)
{
    SCB_InvalidateICache();
    SCB_InvalidateDCache();
}

/**
 * @brief
 * @DateTime  2022-05-02T19:57:28+0800
 * @return                              0: FPU Enable Successfully
 *                                      1: FPU Enable Failure
 */
uint8_t FPU_Enable(void)
{
    __DSB();
    __ISB();
    SCB->CPACR |= 0x00F00000;
    __DSB();
    __ISB();
    if ((SCB->CPACR & 0x00F00000) == 0x00F00000) return OK; else return NOK;
}

/**
 * @brief
 * @DateTime  2022-05-02T20:01:42+0800
 * @return                              0: FPU Disable Successfully
 *                                      1: FPU Disable Failure
 */
uint8_t FPU_Disable(void)
{
    __DSB();
    __ISB();
    SCB->CPACR &= ~0x00F00000;
    __DSB();
    __ISB();
    if ((SCB->CPACR & 0x00F00000) == 0) return OK; else return NOK;
}

/**
 * @brief
 * @DateTime  2022-05-02T20:18:15+0800
 * @param     peripheral                Peripheral_RST_Msk in SYSC_RST register
 */
void Peripheral_Reset(uint32_t peripheral)
{
    SYSC->RST &= ~peripheral;
    SYSC->RST |= peripheral;
}

/**
 * @brief     Setup the microcontroller system.
 *            Initialize the System.
 * @DateTime  2022-05-02T20:29:00+0800
 */
void SystemInit(void)
{
    /* ToDo: add code to initialize the system
             do not use global variables because this function is called before
             reaching pre-main. RW section maybe overwritten afterwards.          */
    LRC_DISABLE;
    ALL_IOLATCH_DISABLE;
    //Cache_Invalidate();
    //Cache_Disable();
    FPU_Enable();
    // SYSC->CLK_EN = 0xFFFFFFFF;
    //ARM_MPU_Disable();
}

/**
 * @brief     This function handles NMI exception
 * @DateTime  2022-05-03T10:32:12+0800
 */
void NMI_Handler(void)
{
    WKU_FLAG_sta0 |= AON_WKU->WKU_FLAG0;
    AON_WKU->WKU_FLAG0 = WKU_FLAG_sta0;
    WKU_FLAG_sta1 |= AON_WKU->WKU_FLAG1;
    AON_WKU->WKU_FLAG1 = WKU_FLAG_sta1;
    WKU_FLAG_sta2 |= AON_WKU->WKU_FLAG2;
    AON_WKU->WKU_FLAG2 = WKU_FLAG_sta2;
}

/**
 * @brief     Deep_Sleep: Core and peripherals are all sleep, all clocks are gated
 * @DateTime  2022-05-03T10:34:34+0800
 */
void Deep_Sleep(void)
{
    DS_MODE_ENABLE;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
}

/**
 * @brief     Power_Down: 1.2V logics are in power down state
 * @DateTime  2022-05-03T10:36:51+0800
 */
void Power_Down(void)
{
    PD_MODE_ENABLE;
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
}

/**
 * @brief     CPU_Sleep: Core is sleep
 * @DateTime  2022-05-03T10:37:18+0800
 */
void CPU_Sleep(void)
{
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    __WFI();
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
