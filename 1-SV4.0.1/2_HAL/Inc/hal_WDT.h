/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_WDT_H
#define _HAL_WDT_H
#include <stdint.h>

#include <stdio.h>

#include "SKY32B750.h"

void WDT_RstNow(void);
void WDT_Init(uint32_t val);
void Close_Wdt(void);

#define WDT_FEED_DOGS()           (WDT->STR = 0x1)                        // bit0:clear












#endif /*_HAL_WDT_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























