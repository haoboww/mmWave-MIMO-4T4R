/******************************************************************************
 * @Filename     : wwdg.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/13/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : WWDG driver source file
 *
 * Copyright (C) 2020 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

uint16_t WWDG_Init(wwdg_config_t *config)
{
    if ((config->div >= WWDG_TIMER_BASE_MAX) || (config->counter > WWDG_MAX_VAL) \
        || (config->counter < WWDG_MIN_VAL) || (config->win_val > WWDG_MAX_VAL))
        return WWDG_INVALID_PARA;

    if (config->int_en == 1)
        WWDG->CFR = (WWDG_EWI_Msk | (config->div << WWDG_WDGTB_Pos) | (config->win_val << WWDG_WINDOW_VALUE_Pos));
    else
        WWDG->CFR = ((config->div << WWDG_WDGTB_Pos) | (config->win_val << WWDG_WINDOW_VALUE_Pos));

    WWDG->CR = config->counter;

    return OK;
}

void WWDG_Seed(uint8_t counter)
{
    WWDG->CR = counter;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
