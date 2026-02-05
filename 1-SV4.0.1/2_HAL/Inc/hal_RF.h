#ifndef __HAL_RF_H
#define __HAL_RF_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bsp_SPI.h"
#include "bsp_GPIO.h"
#include "App_Def.h"
#define SYS_STA0_ADDR             (0x00)
#define SYS_STA1_ADDR             (0x01)
#define SYS_STA2_ADDR             (0x02)
#define SYS_STA3_ADDR             (0x03)
#define RAMP_PLL_CFG0_ADDR        (0x04)
#define RAMP_PLL_CFG1_ADDR        (0x05)
#define RAMP_SYNCW_CFG0_ADDR      (0x06)
#define RAMP_SYNCW_CFG1_ADDR      (0x07)
#define RAMP_ACCW_CFG0_ADDR       (0x08)
#define RAMP_ACCW_CFG1_ADDR       (0x09)
#define RAMP_WM_CFG0_ADDR         (0x0A)
#define RAMP_WM_CFG1_ADDR         (0x0B)
#define ANA_CFG0_ADDR             (0x0C)
#define ANA_CFG1_ADDR             (0x0D)
#define ANA_CFG2_ADDR             (0x0E)
#define ANA_CFG3_ADDR             (0x0F)
#define ANA_CFG4_ADDR             (0x10)
#define ANA_CFG5_ADDR             (0x11)
#define ANA_CFG6_ADDR             (0x12)
#define ANA_CFG7_ADDR             (0x13)
#define ANA_CFG8_ADDR             (0x14)
#define ANA_CFG9_ADDR             (0x15)
#define ANA_CFG10_ADDR            (0x16)
#define ANA_CFG11_ADDR            (0x17)
#define ANA_CFG12_ADDR            (0x18)
#define ANA_CFG13_ADDR            (0x19)
#define ANA_CFG14_ADDR            (0x1A)
#define ANA_CFG15_ADDR            (0x1B)
#define ANA_CFG16_ADDR            (0x1C)
#define ANA_CFG17_ADDR            (0x1D)
#define ANA_CFG18_ADDR            (0x1E)
#define ANA_CFG19_ADDR            (0x1F)
#define ANA_CFG20_ADDR            (0x20)
#define ANA_CFG21_ADDR            (0x21)
#define ANA_CFG22_ADDR            (0x22)
#define RAMP_STA0_ADDR            (0x30)
#define RAMP_STA1_ADDR            (0x31)
#define RAMP_STA2_ADDR            (0x32)
#define RAMP_STA3_ADDR            (0x33)
#define RAMP_MEM_ADDR             (0x3E)

#define CMD_RAMP_START            (0x00)
#define CMD_RAMP_STOP             (0x02)
#define CMD_WAV_MEM_CLR           (0x03)

typedef struct __RF_WAVE_CFG__{
	uint16_t a1;
	uint16_t a2;
	uint16_t b1;
	uint16_t b2;
	uint16_t c1;
	uint16_t c2;
	uint16_t r;
	uint16_t v;
}RF_WAVE_CFG;
typedef struct __STRUCT_RF_REG__{
	uint16_t anaCfg0;
	uint16_t anaCfg1;
	uint16_t anaCfg2;
	uint16_t anaCfg3;	
	uint16_t anaCfg4;
	uint16_t anaCfg5;
	uint16_t anaCfg6;
	uint16_t anaCfg7;		
	uint16_t anaCfg8;
	uint16_t anaCfg9;
	uint16_t anaCfg10;
	uint16_t anaCfg11;	
	uint16_t anaCfg12;
	uint16_t anaCfg13;
	uint16_t anaCfg14;
	uint16_t anaCfg15;			
	uint16_t anaCfg16;
	uint16_t anaCfg17;
	uint16_t anaCfg18;
	uint16_t anaCfg19;	
	uint16_t anaCfg20;
	uint16_t anaCfg21;
	uint16_t anaCfg22;
	uint16_t rampPLLcfg0;
	uint16_t rampPLLcfg1;
	uint16_t rampSYNCWcfg0;
	uint16_t rampSYNCWcfg1;	
	uint16_t rampACCWcfg0;
	uint16_t rampACCWcfg1;
	uint16_t rampWMcfg0;
	uint16_t rampWMcfg1;
	RF_WAVE_CFG wave[20];
	
}STRUCT_RF_REG;
extern uint16_t RF_ReadRegSPI(uint8_t csNum,uint8_t u8Addr);
void RF_WriteRegSPI(uint8_t csNum,uint8_t u8Addr, uint16_t u16Data);
void RF_WritePaSPI_AM(uint8_t csNum,uint8_t u8Addr,uint16_t u16Data);
void RF_WaveSet(uint8_t Row_Addr,RF_WAVE_CFG sWave);
extern uint32_t RF_Freq_Start_Calc(float freqstart);
extern RF_WAVE_CFG Hal_RF_WaveCalc(float bandwidthA,float bandwidthB,float bandwidthC,float sweepTimA,float sweepTimB,float sweepTimC);
extern RF_WAVE_CFG Hal_RF_WaveGen(float bandwidthA, float sweepTimA, uint8_t syncwA, float bandwidthB, float sweepTimB, uint8_t syncwB, float bandwidthC, float sweepTimC, uint8_t syncwC, uint16_t r, uint16_t v);
extern RF_WAVE_CFG Hal_RF_WaveConfig(uint16_t a1,uint16_t a2,uint16_t b1,uint16_t b2,uint16_t c1,uint16_t c2,uint16_t r,uint16_t v);
void RF_Clock40M_En(void);
void RF_waveStart(void);
void RF_waveStop(void);
void RF_WriteReg40M(uint8_t csNum,uint8_t u8Addr, uint16_t u16Data);

#endif /*__HAL_RF_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/
