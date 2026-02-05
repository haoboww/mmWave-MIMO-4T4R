/******************************************************************************
 * @Filename     : delay.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 15:54:50
 * @Modified by  : CB
 *
 * @Brief        : delay function header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define MS_STEP     440

void HW_Delay_us(uint32_t u32Cnt);
void HW_Delay_ms(uint32_t u32Cnt);
uint32_t Calib_SW_ms(void);
void SW_Delay_us(uint32_t u32Cnt);
void SW_Delay_ms(uint32_t u32Cnt);

#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
