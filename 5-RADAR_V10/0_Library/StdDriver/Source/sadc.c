/******************************************************************************
 * @Filename     : sadc.c
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 07/25/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : SADC driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/
#include "SKY32B750.h"

/**
 * @Breif  : Caliber_SADC_Ref : Calibrate sadc reference voltage and save the value into flash info area
 *
 * @Param  : u32InfoAddr: sould be flash info area address
 *
 * @Returns:
 */
uint32_t Caliber_SADC_Ref(uint32_t u32InfoAddr)
{
    uint32_t ref_val;
    uint8_t ref_buf[4] = {0, 0, 0, 0};
    if ((u32InfoAddr >= INFOFLASH_ADDR_BEGIN) && (u32InfoAddr < INFOFLASH_ADDR_END))
    {
        SADC_CLK_ENABLE;
        HW_Delay_us(10);
        SADC_IN2;
        SADC_TRIGGER;
        while (SADC_GET_EOC_FLG == 0);
        SADC_CLR_EOC_FLG;
        ref_val = (1023 * 1000) / SADC_GET_RESULT;
        SADC_CLK_DISABLE;
        if ((ref_val < SADC_REF_MIN) || (ref_val > SADC_REF_MAX))
        {
            return SADC_REF_ERR;
        }
        ref_buf[0] = GET_BYTE0(ref_val);
        ref_buf[1] = GET_BYTE1(ref_val);
        //write sadc ref valtage into flash
        if (EFC_WriteInside1Page(u32InfoAddr, 4, ref_buf) != OK)
        {
            return SADC_REF_ERR;
        }
        return ref_val;
    }
    else
    {
        return SADC_INVALID_PARA;
    }
}

/**
 * @Breif  : Get_SADC_Ref : get sadc reference voltage from flash info area
 *
 * @Param  : u32InfoAddr: should be flash info area address
 *
 * @Returns:
 */
uint32_t Get_SADC_Ref(uint32_t u32InfoAddr)
{
    if ((u32InfoAddr >= INFOFLASH_ADDR_BEGIN) && (u32InfoAddr < INFOFLASH_ADDR_END))
    {
        return Read_M32(u32InfoAddr);
    }
    else
    {
        return SADC_INVALID_PARA;
    }
}

/**
 * @Breif  : SADC_Get_Voltage : get voltage from sadc corresponding channel
 *
 * @Param  : u32Vref: sadc reference voltage, default 1600 (mV)
 *           u8Channel: sadc channel
 *           = VDD_CHANNEL
 *           = TEMP_CHANNEL
 *           = IN2_CHANNEL
 *           = IN3_CHANNEL
 *           = IN4_CHANNEL
 *           = IN5_CHANNEL
 *           = IN6_CHANNEL
 *
 * @Returns: voltage (mV)
 */
uint32_t SADC_Get_Voltage(uint32_t u32Vref, uint8_t u8Channel)
{
    uint32_t sadc_data;
    uint32_t voltage;
    SADC->CON |= (u8Channel << SADC_CON_MUX_Pos);
    SADC_TRIGGER;
    while (SADC_GET_EOC_FLG == 0); //wait sadc convert finish flg == 1
    SADC_CLR_EOC_FLG;              //clear sadc convert finish flg
    sadc_data = SADC->DAT;
    if (u8Channel == VDD_CHANNEL)
    {
        voltage = (sadc_data * u32Vref * 3) / 1023;
    }
    else
    {
        voltage = (sadc_data * u32Vref) / 1023;
    }
    return voltage;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
