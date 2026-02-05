/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _PARAMCONFIG_H
#define _PARAMCONFIG_H

#include <stdint.h>
#include <string.h>

#include "App_Def.h"
#include "bsp_UART.h"
#include "Usart_Protocol.h"
#include "Radar_Config.h"

//#include "stm32g4xx_ll_iwdg.h"
#define SAMPLE_LEN           (1024)     //采样点数，DownFac前面
#define ADC_SAMPLING_RATE    (20.0f)    //采样率 MHz
#define BANDWIDTH            (3510000000) //hz
#define SWEEPING_TIME        (60)       //us
#define CHIRP_TIME           ((10+SWEEPING_TIME+5)*NUM_TX/1e6)
#define FREQ_INIT            (75e9)





enum
{
    STANDBY_TIME = 0,
    PROG_FUNC,
    WORK_MODE,
    CFARTHVG,
    CFARTHVS,
    CFARTHV,
    CFARTHRG,
    CFARTHRS,
    CFARTHR,
		CREN,
		PEAKENR,
		PEAKENV,
		CFARRMODE,
		CFARVMODE,
    DPKTH,
		DPKTIMES,
    NUMTLV,
		MEAN,
		ZOTH,
    CHIRPGAP,
    BW,	
		CFARTHS,
		FRAMEGAP,
		NUMSUM,
};

#define FLASH_PARAM_HEAD_ADDR		(INFOFLASH_ADDR_BEGIN)  //0x80000
#define FLASH_STANDBY_TIME_ADDR		(FLASH_PARAM_HEAD_ADDR + STANDBY_TIME * 4)
#define FLASH_PROG_FUNC_ADDR		(FLASH_PARAM_HEAD_ADDR + PROG_FUNC * 4)
#define FLASH_WORK_MODE_ADDR        (FLASH_PARAM_HEAD_ADDR + WORK_MODE * 4)
#define FLASH_CFARTHVG_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHVG * 4)
#define FLASH_CFARTHVS_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHVS * 4)
#define FLASH_CFARTHV_ADDR 			(FLASH_PARAM_HEAD_ADDR + CFARTHV * 4)
#define FLASH_CFARTHRG_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHRG * 4)
#define FLASH_CFARTHRS_ADDR 		(FLASH_PARAM_HEAD_ADDR + CFARTHRS * 4)
#define FLASH_CFARTHR_ADDR 			(FLASH_PARAM_HEAD_ADDR + CFARTHR * 4)
#define FLASH_CREN_ADDR        (FLASH_PARAM_HEAD_ADDR + CREN * 4)
#define FLASH_CFARRMODE_ADDR        (FLASH_PARAM_HEAD_ADDR + CFARRMODE * 4)
#define FLASH_CFARVMODE_ADDR        (FLASH_PARAM_HEAD_ADDR + CFARVMODE * 4)
#define FLASH_PEAKENR_ADDR        (FLASH_PARAM_HEAD_ADDR + PEAKENR * 4)
#define FLASH_PEAKENV_ADDR        (FLASH_PARAM_HEAD_ADDR + PEAKENV * 4)
#define FLASH_DPKTH_ADDR 			(FLASH_PARAM_HEAD_ADDR + DPKTH * 4)
#define FLASH_DPKTIMES_ADDR 			(FLASH_PARAM_HEAD_ADDR + DPKTIMES * 4)
#define FLASH_NUMTLV_ADDR			(FLASH_PARAM_HEAD_ADDR + NUMTLV * 4)
#define FLASH_MEAN_ADDR       (FLASH_PARAM_HEAD_ADDR + MEAN * 4)
#define FLASH_ZOTH_ADDR       (FLASH_PARAM_HEAD_ADDR + ZOTH * 4)
#define FLASH_CHIRPGAP_ADDR         (FLASH_PARAM_HEAD_ADDR + CHIRPGAP * 4)
#define FLASH_BW_ADDR               (FLASH_PARAM_HEAD_ADDR + BW * 4)
#define FLASH_CFARTHS_ADDR          (FLASH_PARAM_HEAD_ADDR + CFARTHS * 4)
#define FLASH_FRAMEGAP_ADDR         (FLASH_PARAM_HEAD_ADDR + FRAMEGAP * 4)
#define FLASH_NUMSUM_ADDR           (FLASH_PARAM_HEAD_ADDR + NUMSUM * 4)

typedef struct
{
	uint8_t WorkMode;
	float RangeRes;
	float VelRes;

	uint32_t frameTime;
	uint32_t projFunc;
	uint32_t baudrate;
	/*app*/
	uint8_t cfarThVG;
	uint8_t cfarThVS;
  uint8_t cfarThV;
	uint8_t cfarThRG;
	uint8_t cfarThRS;
  uint8_t cfarThR;
	uint8_t cfarR_En;
	uint8_t cfarRmode;
	uint8_t cfarVmode;
	uint8_t peakenV;
	uint8_t numTLV;
	uint32_t dpkThres;
	uint8_t dpkTimes;
	uint8_t meanEn;
	uint8_t zoTh;
	uint8_t bandWidth;
	uint32_t chirpGap;
	uint8_t cfarThS;
	uint8_t frameGap;
	uint8_t numSum;
} STRUCT_PARAM;




extern STRUCT_PARAM stFlashParam;





void ParamConfigInit(void);
void Read_Param_From_Flash(void);







#endif /*_PARAMCONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
