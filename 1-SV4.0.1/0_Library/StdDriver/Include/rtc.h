/******************************************************************************
 * @Filename     : rtc.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/17/2022 16:02:55
 * @Modified by  : CB
 *
 * @Brief        : RTC driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Error code
#define RTC_INVALID_PARA       0x0C01

#define ONCE_MODE              0
#define REPEAT_MODE            1
#define SOURCE_RC32K           0
#define SOURCE_OSC32K          1
#define RTC_SELECT_LRC32K      LXTAL_DISABLE
#define RTC_SELECT_OSC32K      LXTAL_ENABLE
#define RTC_CLEAR_COUNTER      (RTC->CLR = RTC_CLR_Msk)

uint16_t RTC_Enable(uint8_t u8ClkSrc);
void RTC_Disable(void);
void RTC_Get_Current_Value(uint8_t *pval);
void RTC_Reset(void);
void RTC_Wakeup_ms(uint32_t u32Msec, uint8_t u8Mode);
void RTC_Wakeup_sec(uint32_t u32sec, uint8_t u8Mode);
void RTC_Pulse_Config(uint8_t u8Inv, uint8_t u8LowTime, uint8_t u8HighTime);

#ifdef __cplusplus
}
#endif

#endif //__RTC_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
