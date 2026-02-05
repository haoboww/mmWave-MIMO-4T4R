#include "bsp_GPIO.h"
#include "hal_Delay.h"
void BSP_GPIO_Init(void){
	GPIO_CLK_ENABLE;
	GPIO_SetMode(CHIPEN_GPIO_Port, CHIPEN_Pin, GPIO_PMODE_OUTPUT);
	GPIO_SetMode(SPI0_CS1_GPIO_Port, SPI0_CS1_Pin, GPIO_PMODE_OUTPUT);
	GPIO_SetMode(OSPI0_CS_GPIO_Port, OSPI0_CS_Pin, GPIO_PMODE_OUTPUT);
	CHPIEN_WritePin = 0;
	SPI0_CS1_WritePin = 1;
	OSPI0_CS_WritePin = 1;
}

void BSP_ChipEn(void){
	CHPIEN_WritePin=1;
	Cnt_Delay_ms(1);
	SPI0_CS1_WritePin=1;
}

void BSP_ChipDisable(void){
	CHPIEN_WritePin=0;
	SPI0_CS1_WritePin=0;
}
















