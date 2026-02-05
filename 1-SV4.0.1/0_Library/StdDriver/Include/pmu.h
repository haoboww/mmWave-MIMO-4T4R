/******************************************************************************
 * @Filename     : pmu.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 01/26/2021 9:12:34
 * @Modified by  : MXY
 *
 * @Brief        : Power Management Unit header file
 *
 * Copyright (C) 2021 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __PMU_H__
#define __PMU_H__

#ifdef __cplusplus
extern "C"
{
#endif




















void Deep_Sleep(void);
void Power_Down(void);
void CPU_Sleep(void);
void NMI_Handler (void);

#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2021 SKYRELAY Technology co.,Ltd ***/

