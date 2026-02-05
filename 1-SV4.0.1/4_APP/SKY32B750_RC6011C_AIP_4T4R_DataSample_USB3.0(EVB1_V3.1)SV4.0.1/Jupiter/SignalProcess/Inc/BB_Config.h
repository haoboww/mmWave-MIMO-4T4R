/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BB_CONFIG_H
#define _BB_CONFIG_H

#include "BB_AlgProc.h"
#include "hal_BB.h"
#define MAX_ZP_LEN  10
typedef struct{
    uint64_t txNum_zp0[MAX_ZP_LEN];
    uint64_t txNum_zp1[MAX_ZP_LEN];
    uint64_t txNum_zp2[MAX_ZP_LEN];
    uint64_t txNum_zp3[MAX_ZP_LEN];
}STRUCT_FFT2D_ZP;
extern volatile uint32_t rampCnt;
extern BB_REGB_T *bb_regb_str;
extern volatile uint32_t BB_isrSet;
extern volatile uint32_t BB_isrRead;
extern volatile uint8_t BB_sampOverFlag;
extern volatile uint8_t BB_firstChirp;
extern STRUCT_BB_CFAR_CFG0 stCfarCfg0;
extern STRUCT_FFT2D_ZP fft2d_zp;
void dataprep_RegSet(RADAR_USER_CFG *usrCfg);
void BB_CFAR_Vel_Init(void);
void BB_FFT2D_ZP_Init(void);
//void BB_CFAR_Range_Init(void);
//uint16_t BB_CFAR_Vel_Calc(void);
//uint16_t BB_CFAR_Range_Calc(void);
//void cfarUnion(STRUCT_FRAMEPOINT *frameRst);
#endif /*_BB_CONFIG_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























