#include "hal_SADC.h"
#include "main.h"


FLOAT32 GetAdcValue(void)
{

	UINT32 adc_data = 0;
	UINT32 i;
	FLOAT32 sum_val = 0;
	FLOAT32 VGPIO;
	UINT32 adc_ref1;
	UINT32 adc_ref2;
	UINT32 sampTimes;
    //Enable SADC CLK
	SADC_CLK_ENABLE;
	//Enable SDAC Function
	SADC->CON |= SADC_CON_EN_Msk;  

////////////Chose ADC GPIO Source

//			SADC->CON |= 0x2;//GPIO0_10 Source
//	        SADC->CON |= 0x3;//GPIO0_11 Source
	SADC->CON |= 0x4;//GPIO0_12 Source
//			SADC->CON |= 0x8;//GPIO0_13 Source
//			SADC->CON |= 0xC;//GPIO0_14 Source


	sampTimes = 100;
	for (i = 0; i < sampTimes; )
	{
		SW_Delay_us(100);//Necessary Delay
		//Trigger Sampling
		SADC->TRIG = SADC_TRIG_Msk;
		//Wait for the completion of Sampling process
		while ((SADC->STA & SADC_EOC_Msk) != SADC_EOC_Msk);
		//Get Sampling Data
		adc_data = (uint16_t)SADC->DAT;
		if (adc_data)
		{
			sum_val += adc_data;
			i++;
		}
		//Clear The Status Register
		SADC->STA = SADC_EOC_Msk;
	}

	adc_data = sum_val / sampTimes;
	// printf("ADC data: 0x%04x --- %d \n", adc_data, adc_data);
	adc_ref1 = Read_M32(0x0008211C);
	// printf("adc_ref1 = 0x%x\n", adc_ref1);
	adc_ref2 = Read_M32(0x00082120);
	// printf("adc_ref2 = 0x%x\n", adc_ref2);

	VGPIO = (adc_data - (1.2 * adc_ref1 - 0.2 * adc_ref2)) / (adc_ref2 - adc_ref1);
	// printf("The Voltage of GPIO Source = %lfV\n", VGPIO);
	//Disable SDAC Function
	SADC->CON &= ~SADC_CON_EN_Msk;
    return VGPIO;

}
