/******************************************************************************
 * @Filename     : retarget.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/18/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : HW retarget header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __RETARGET_H__
#define __RETARGET_H__
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

void hard_printf(USCI_T *usci, uint32_t uart_bps);
int getkey(void);

#ifdef __cplusplus
}
#endif

#endif

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
