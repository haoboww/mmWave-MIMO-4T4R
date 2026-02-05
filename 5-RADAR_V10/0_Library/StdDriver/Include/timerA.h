/******************************************************************************
 * @Filename     : timerB.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/09/2022 15:59:57
 * @Modified by  : CB
 *
 * @Brief        : timerA driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __TMRA_H__
#define __TMRA_H__

#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define TMRA_INVALID_PARA       0x0601

#define TMRA_MAX_DEADTIME       0xFF

enum tmra_div {
    DIV_1   = (0x0ul << TMRA_ID_Pos),
    DIV_2   = (0x1ul << TMRA_ID_Pos),
    DIV_4   = (0x2ul << TMRA_ID_Pos),
    DIV_8   = (0x3ul << TMRA_ID_Pos)
};

enum tmra_exdiv {
    EXDIV_1  = (0x0ul << TMRA_IDEX_Pos),
    EXDIV_2  = (0x1ul << TMRA_IDEX_Pos),
    EXDIV_3  = (0x2ul << TMRA_IDEX_Pos),
    EXDIV_4  = (0x3ul << TMRA_IDEX_Pos),
    EXDIV_5  = (0x4ul << TMRA_IDEX_Pos),
    EXDIV_6  = (0x5ul << TMRA_IDEX_Pos),
    EXDIV_7  = (0x6ul << TMRA_IDEX_Pos),
    EXDIV_8  = (0x7ul << TMRA_IDEX_Pos),
};

enum tmra_mc {
    TIM_STOP     = (0x0ul << TMRA_MC_Pos), //Stop mode: timer is halted
    TIM_UP       = (0x1ul << TMRA_MC_Pos), //Up mode: timer counts up to TACCR0
    TIM_CONTINUE = (0x2ul << TMRA_MC_Pos), //Continuous mode: timer counts up to 0xFFFF
    TIM_UP_DOWN  = (0x3ul << TMRA_MC_Pos)  //Up/down mode: timer counts up to TACCR0 then down to 0x0000
};

typedef struct
{
    enum tmra_div    div;
    enum tmra_exdiv  exdiv;
    enum tmra_mc     mc;
    uint32_t         count;              /*!< Timer counter value, different working modes have different values */
    uint8_t          cci_en;             /*!< Capture/Compare interrupt enable */
    uint8_t          tai_en;             /*!< TimerA interrupt enable */
    uint8_t          cci_dma_en;
    uint8_t          tai_dma_en;
} tmra_config_t;

enum pwm_mod {
    OUT_BIT_VAL     = (0x0ul << TMRA_OUTMOD_Pos),
    OUT_SET         = (0x1ul << TMRA_OUTMOD_Pos),
    OUT_TOGGLE_RST  = (0x2ul << TMRA_OUTMOD_Pos),
    OUT_SET_RST     = (0x3ul << TMRA_OUTMOD_Pos),
    OUT_TOGGLE      = (0x4ul << TMRA_OUTMOD_Pos),
    OUT_RST         = (0x5ul << TMRA_OUTMOD_Pos),
    OUT_TOGGLE_SET  = (0x6ul << TMRA_OUTMOD_Pos),
    OUT_RST_SET     = (0x7ul << TMRA_OUTMOD_Pos)
};

enum tmra_channel_number {
    TMRA_CHANNEL0    = 0,
    TMRA_CHANNEL1    = 1,
    TMRA_CHANNEL2    = 2,
    TMRA_CHANNEL3    = 3,
    TMRA_CHANNEL4    = 4,
    TMRA_CHANNEL5    = 5,
    TMRA_CHANNEL6    = 6,
    TMRA_CHANNEL_MAX = 7
};

enum pwm_pn_cfg {
    P_OUT_POSITIVE = (0x0ul << TMRA_P_CFG_Pos),
    P_OUT_LOW      = (0x1ul << TMRA_P_CFG_Pos),
    P_OUT_HIGHT    = (0x2ul << TMRA_P_CFG_Pos),
    P_OUT_NEGATIVE = (0x3ul << TMRA_P_CFG_Pos),
    N_OUT_POSITIVE = (0x0ul << TMRA_N_CFG_Pos),
    N_OUT_LOW      = (0x1ul << TMRA_N_CFG_Pos),
    N_OUT_HIGHT    = (0x2ul << TMRA_N_CFG_Pos),
    N_OUT_NEGATIVE = (0x3ul << TMRA_N_CFG_Pos)
};

typedef struct
{
    enum tmra_channel_number ch_num;
    enum tmra_div          div;
    enum tmra_exdiv        exdiv;
    enum tmra_mc           mc;
    uint16_t               pwm_ccr0;
    uint16_t               pwm_ccr1;
    uint16_t               pwm_ccr2;
    uint16_t               pwm_ccr3;
    uint16_t               pwm_ccr4;
    uint16_t               pwm_ccr5;
    uint16_t               pwm_ccr6;
    enum pwm_mod           pwm_mod;
    enum pwm_pn_cfg        pwm_p_cfg;
    enum pwm_pn_cfg        pwm_n_cfg;
} pwm_config_t;

/** Input capture edge type */
enum tmra_cap_edge {
    CAP_NO            = (0x0ul << TMRA_CM_Pos),
    CAP_EDGE_RISING   = (0x1ul << TMRA_CM_Pos), /*!< Rising edge is set as input capture edge */
    CAP_EDGE_FALLING  = (0x2ul << TMRA_CM_Pos), /*!< Falling edge is set as input capture edge */
    CAP_EDGE_BOTH     = (0x3ul << TMRA_CM_Pos)  /*!< Both rising and falling edge are set as input capture edge */
};

/** Input capture edge type */
enum tmra_cap_ccis {
    CCIS_GPIO = (0x0ul << TMRA_CCIS_Pos),
    CCIS_ICLK = (0x1ul << TMRA_CCIS_Pos),
    CCIS_LRC  = (0x2ul << TMRA_CCIS_Pos),
    CCIS_VCC  = (0x3ul << TMRA_CCIS_Pos)
};

enum tmra_cap_mode {
    COMPARE_MODE      = (0x0ul << TMRA_CAP_Pos),
    CAPTURE_MODE      = (0x1ul << TMRA_CAP_Pos),
};

typedef struct
{
    enum tmra_channel_number ch_num;
    enum tmra_div            div;
    enum tmra_exdiv          exdiv;
    enum tmra_cap_edge       cap_edge;      /*!< Input Capture Edge */
    enum tmra_cap_ccis       cap_input;     /*!< Capture/Compare input select */
    uint8_t                  cci_en;        /*!< Capture/Compare interrupt enable */
    uint8_t                  dma_en;
} capture_config_t;

typedef struct
{
    enum tmra_channel_number ch_num;
    enum tmra_div            div;
    enum tmra_exdiv          exdiv;
    enum tmra_mc             mc;
    enum tmra_cap_ccis       cap_input;     /*!< Capture/Compare input select */
    uint16_t                 cap_ccr0;
    uint16_t                 cap_ccr1;
    uint16_t                 cap_ccr2;
    uint16_t                 cap_ccr3;
    uint16_t                 cap_ccr4;
    uint16_t                 cap_ccr5;
    uint16_t                 cap_ccr6;
    uint8_t                  cci_en;
    uint8_t                  dma_en;
} compare_config_t;

extern volatile uint32_t cap_int_count;

#define TMRA_CCI_ENABLE        TMRA->TAIE |= TMRA_CCIFG0_Msk
#define TMRA_TAI_ENABLE        TMRA->TAIE |= TMRA_TAIFG_Msk
#define TMRA_CCI_DISABLE       TMRA->TAIE &= ~TMRA_CCIFG0_Msk
#define TMRA_TAI_DISABLE       TMRA->TAIE &= ~TMRA_TAIFG_Msk

#define TMRA_CCI0_ENABLE       TMRA->TAIE |= TMRA_CCIFG0_Msk
#define TMRA_CCI1_ENABLE       TMRA->TAIE |= TMRA_CCIFG1_Msk
#define TMRA_CCI2_ENABLE       TMRA->TAIE |= TMRA_CCIFG2_Msk
#define TMRA_CCI3_ENABLE       TMRA->TAIE |= TMRA_CCIFG3_Msk
#define TMRA_CCI4_ENABLE       TMRA->TAIE |= TMRA_CCIFG4_Msk
#define TMRA_CCI5_ENABLE       TMRA->TAIE |= TMRA_CCIFG5_Msk
#define TMRA_CCI6_ENABLE       TMRA->TAIE |= TMRA_CCIFG6_Msk

#define TMRA_CCI0_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG0_Msk
#define TMRA_CCI1_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG1_Msk
#define TMRA_CCI2_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG2_Msk
#define TMRA_CCI3_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG3_Msk
#define TMRA_CCI4_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG4_Msk
#define TMRA_CCI5_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG5_Msk
#define TMRA_CCI6_DISABLE      TMRA->TAIE &= ~TMRA_CCIFG6_Msk

#define TMRA_CCI0_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA0_Msk
#define TMRA_CCI1_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA1_Msk
#define TMRA_CCI2_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA2_Msk
#define TMRA_CCI3_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA3_Msk
#define TMRA_CCI4_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA4_Msk
#define TMRA_CCI5_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA5_Msk
#define TMRA_CCI6_DMA_ENABLE   TMRA->TAIE |= TMRA_DMA6_Msk
#define TMRA_TAI_DMA_ENABLE    TMRA->TAIE |= TMRA_DMA7_Msk

#define TMRA_CCI0_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA0_Msk
#define TMRA_CCI1_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA1_Msk
#define TMRA_CCI2_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA2_Msk
#define TMRA_CCI3_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA3_Msk
#define TMRA_CCI4_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA4_Msk
#define TMRA_CCI5_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA5_Msk
#define TMRA_CCI6_DMA_DISABLE   TMRA->TAIE &= ~TMRA_DMA6_Msk
#define TMRA_TAI_DMA_DISABLE    TMRA->TAIE &= ~TMRA_DMA7_Msk

#define TMRA_GET_SCCI0         ((TMRA->TACCTL0 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI1         ((TMRA->TACCTL1 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI2         ((TMRA->TACCTL2 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI3         ((TMRA->TACCTL3 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI4         ((TMRA->TACCTL4 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI5         ((TMRA->TACCTL5 & TMRA_SCCI_Msk) ? 1 : 0)
#define TMRA_GET_SCCI6         ((TMRA->TACCTL6 & TMRA_SCCI_Msk) ? 1 : 0)

#define TMRA_GET_CCI0          ((TMRA->TACCTL0 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI1          ((TMRA->TACCTL1 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI2          ((TMRA->TACCTL2 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI3          ((TMRA->TACCTL3 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI4          ((TMRA->TACCTL4 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI5          ((TMRA->TACCTL5 & TMRA_CCI_Msk) ? 1 : 0)
#define TMRA_GET_CCI6          ((TMRA->TACCTL6 & TMRA_CCI_Msk) ? 1 : 0)

void TMRA_Reset(void);
void TMRA_Clear(void);
void TMRA_Timer_Cfg(tmra_config_t *config);
uint16_t TMRA_PWM_Cfg(pwm_config_t *config);
uint16_t TMRA_PWM_AutoDead_Cfg(pwm_config_t *config);
uint16_t TMRA_Capture_Cfg(capture_config_t *config);
uint16_t TMRA_Compare_Cfg(compare_config_t *config);

#ifdef __cplusplus
}
#endif

#endif // __TMRA_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
