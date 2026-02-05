/******************************************************************************
 * @Filename     : wwdg.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/23/2022 16:02:55
 * @Modified by  : CB
 *
 * @Brief        : WWDG driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __WWDG_H__
#define __WWDG_H__

#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define WWDG_INVALID_PARA       0x0701

#define WWDG_MAX_VAL            0x7F
#define WWDG_MIN_VAL            0x41

enum wwdg_timer_base {
    WWDG_TIMER_BASE_DIV1 = 0x00, /* 00: CK Counter Clock (PCLK div 4096) div 1 */
    WWDG_TIMER_BASE_DIV2 = 0x01, /* 01: CK Counter Clock (PCLK div 4096) div 2 */
    WWDG_TIMER_BASE_DIV4 = 0x02, /* 10: CK Counter Clock (PCLK div 4096) div 4 */
    WWDG_TIMER_BASE_DIV8 = 0x03, /* 11: CK Counter Clock (PCLK div 4096) div 8 */
    WWDG_TIMER_BASE_MAX  = 0x04
};

typedef struct
{
    enum wwdg_timer_base    div;
    uint8_t                 win_val;
    uint8_t                 counter;
    uint8_t                 int_en;             /*!< WWDG interrupt enable */
} wwdg_config_t;

uint16_t WWDG_Init(wwdg_config_t *config);
void WWDG_Seed(uint8_t counter);

#define WWDG_CLEAR_INT_FLAG         WWDG->SR = 0

#ifdef __cplusplus
}
#endif

#endif //__WWDG_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
