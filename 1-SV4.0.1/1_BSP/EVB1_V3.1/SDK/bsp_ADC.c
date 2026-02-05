#include "bsp_ADC.h"
#include "hal_Delay.h"

void BSP_ADC_Init(void){

	HXTAL_DISABLE;
	FADC_GRP0_LDO_ENABLE;
	FADC_GRP1_LDO_ENABLE;
	Cnt_Delay_ms(1);
	FADC_CLK_ENABLE;
	HXTAL_ENABLE;
	SEL_PAD_GEN_SYNC;       //Select PAD Gen Sync
	SEL_GPIO1_0_GEN_SYNC;   //Select GPIO1.0 Gen Sync
	FADC_SPI_CLK_DIV(0x01);
	FADC_SPI_CLK_ENABLE;

	FADC_SPI_WriteSingle(0x00,0x16,0x00);
	FADC_SPI_WriteSingle(0x00,0x18,0x00);
	FADC_SPI_WriteSingle(0xF0,0x16,0x82); //0X82   manual:not care sync   trigpol: ~sync

	// PORT23_PIN_CTRL->AE = 0xFFFF; //open input for analog 2.0-2.15
	GPIO_AE_Ctrl(GPIO2, 0x00FF, ANALOG_MODE);
	FADC_SPI_WriteSingle(0xFE, 0x18, 0x40); //Set Vref 1.1V
	FADC_SPI_WriteSingle(0xFD,0x18,0xE0); //4channel input buf en  0-3

}
