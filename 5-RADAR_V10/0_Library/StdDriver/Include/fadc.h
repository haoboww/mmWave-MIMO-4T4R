/******************************************************************************
 * @Filename     : fadc.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/09/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : FADC driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#ifndef __FADC_H__
#define __FADC_H__
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FADC_SPI_CLK_DIV(div)               FADC->SPI_DIV = (FADC->SPI_DIV & ~ADC_SPI_CLK_DIV_Msk) | (div << ADC_SPI_CLK_DIV_Pos)

#define FADC_SPI_CLK_ENABLE                 FADC->SPI_EN = 1
#define FADC_SPI_CLK_DISABLE                FADC->SPI_EN = 0

enum fadc_select {
    FADC_CH1  = 0xFE,
    FADC_CH2  = 0xFD,
    FADC_CH3  = 0xFB,
    FADC_CH4  = 0xF7,
    FADC_CH5  = 0xEF,
    FADC_CH6  = 0xDF,
    FADC_CH7  = 0xBF,
    FADC_CH8  = 0x7F,
    REG1_GRP1 = 0xFE,
    REG1_GRP2 = 0xEF,
    REG2_GRP1 = 0xFD,
    REG2_GRP2 = 0xDF,
    REG3_GRP1 = 0xFB,
    REG3_GRP2 = 0xBF,
    REG4_GRP1 = 0xF7,
    REG4_GRP2 = 0x7F,
    FADC_ALL  = 0x00,
};

void FADC_SPI_WriteSingle(uint8_t csn, uint8_t addr, uint8_t wdata);
uint8_t FADC_SPI_ReadSingle(uint8_t csn, uint8_t addr);
void FADC_WriteW4T(uint8_t csn, uint8_t wdata);
void FADC_WriteW5T(uint8_t csn, uint8_t wdata);
void FADC_WriteW6T(uint8_t csn, uint8_t wdata);
void FADC_WriteW7T(uint8_t csn, uint8_t wdata);
void FADC_WriteW8T(uint8_t csn, uint8_t wdata);
void FADC_WriteW9T(uint8_t csn, uint8_t wdata);
void FADC_WriteW10T(uint8_t csn, uint8_t wdata);
void FADC_WriteW11T(uint8_t csn, uint16_t wdata);

#ifdef __cplusplus
}
#endif

#endif // __FADC_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
