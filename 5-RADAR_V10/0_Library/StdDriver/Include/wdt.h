/******************************************************************************
 * @Filename     : wdt.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 16:02:55
 * @Modified by  : CB
 *
 * @Brief        : WDT driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __WDT_H__
#define __WDT_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define WDT_INVALID_PARA       0x0B01

#define WDT_STEP               2         //2 step = 1ms

#define WDT_CLEAR_INT_FLAG     (WDT->STR = WDT_INTF_Msk)
#define WDT_GET_INT_FLAG       (WDT->STR &  WDT_INTF_Msk ? 1 : 0)
#define WDT_CLEAR_COUNTER      (WDT->STR = WDT_CLR_Msk)
#define WDT_GET_EN_STA         (WDT->STA & WDT_EN_STA_Msk ? 1 : 0)
#define WDT_GET_COUNTER        (WDT->FCNT)

void WDT_Enable(void);
void WDT_Disable(void);
void WDT_Reset(void);
void WDT_Open(uint16_t u16comparator);
void WDT_Set_ms(uint16_t u16Msec);

#ifdef __cplusplus
}
#endif

#endif //__WDT_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
