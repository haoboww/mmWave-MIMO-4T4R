/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_SYSCLK_H
#define _HAL_SYSCLK_H

#include <stdio.h>
#include "SKY32B750.h"
#include "bsp_UART.h"
#define CLK_320M_EN
typedef struct{
	uint8_t ibas;
	uint8_t cap;
	uint8_t div;
}STRUCT_HRC;
typedef struct{
	uint32_t hxtal_freq;
	uint8_t prediv;
	uint8_t mdiv;
	uint8_t posdiv;
}STRUCT_PLL;
extern STRUCT_HRC stHRC;
extern STRUCT_PLL stPLL;
void SYS_CLK_PLL_Init(STRUCT_PLL *stPLL);
void SYS_CLK_HRC_Init(STRUCT_HRC *stHRC);
void HAL_SYS_CLK_Init(STRUCT_HRC *stHRC,STRUCT_PLL *stPLL);
#endif /*_HAL_SYSCLK_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























