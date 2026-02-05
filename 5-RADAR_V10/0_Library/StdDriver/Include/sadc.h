/******************************************************************************
 * @Filename     : sadc.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 07/25/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : SADC driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __SADC_H__
#define __SADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define VDD_CHANNEL          0x0          //VDD33
#define TEMP_CHANNEL         0x1          //VTemp
#define IN2_CHANNEL          0x2          //GPIO0.10
#define IN3_CHANNEL          0x3          //GPIO0.11
#define IN4_CHANNEL          0x4          //GPIO0.12
#define IN5_CHANNEL          0x8          //GPIO0.13
#define IN6_CHANNEL          0xc          //GPIO0.14

#define SADC_REF_BASE_ADDR   0x40220
#define SADC_TEMP_BASE_ADDR  0x40224
#define SADC_REF_MAX         0x7F3
#define SADC_REF_MIN         0x681

//Error Code
#define SADC_INVALID_PARA    0xA101
#define SADC_REF_ERR         0xA102
#define SADC_TEMP_ERR        0xA103

#define SADC_VDD             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (VDD_CHANNEL << SADC_CON_MUX_Pos))  //VDD selected as sadc source
#define SADC_TEMP            (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (TEMP_CHANNEL << SADC_CON_MUX_Pos)) //TEMP selected as sadc source
#define SADC_IN2             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (IN2_CHANNEL << SADC_CON_MUX_Pos))  //GPIO0.10 selected as SADC_IN2 source
#define SADC_IN3             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (IN3_CHANNEL << SADC_CON_MUX_Pos))  //GPIO0.11 selected as SADC_IN3 source
#define SADC_IN4             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (IN4_CHANNEL << SADC_CON_MUX_Pos))  //GPIO0.12 selected as SADC_IN4 source
#define SADC_IN5             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (IN5_CHANNEL << SADC_CON_MUX_Pos))  //GPIO0.13 selected as SADC_IN5 source
#define SADC_IN6             (SADC->CON = (SADC->CON & (~SADC_CON_MUX_Msk)) | (IN6_CHANNEL << SADC_CON_MUX_Pos))  //GPIO0.14 selected as SADC_IN6 source
#define SADC_TRIGGER         (SADC->CON |= SADC_TRIG_Msk           )    //Trigger sadc sample once
#define SADC_GET_EOC_FLG     (SADC->CON & SADC_EOC_Msk ? 1 : 0     )    //Check end of conversion flag
#define SADC_CLR_EOC_FLG     (SADC->CON |= SADC_EOC_Msk            )    //Clear end of conversion flag
#define SADC_INT_ENABLE      (SADC->CON |= SADC_CON_IE_Msk         )    //SADC intr enable
#define SADC_INT_DISABLE     (SADC->CON &= ~SADC_CON_IE_Msk        )    //SADC intr disable
#define SADC_GET_RESULT      (SADC->DAT                            )    //Get SADC result

uint32_t Caliber_SADC_Ref(uint32_t u32InfoAddr);
uint32_t Get_SADC_Ref(uint32_t u32InfoAddr);
uint32_t SADC_Get_Voltage(uint32_t u32Vref, uint8_t u8Channel);

#ifdef __cplusplus
}
#endif

#endif // __SADC_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
