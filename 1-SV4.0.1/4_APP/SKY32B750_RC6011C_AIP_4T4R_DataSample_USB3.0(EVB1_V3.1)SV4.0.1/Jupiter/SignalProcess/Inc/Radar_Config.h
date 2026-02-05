#ifndef __RADAR_CONFIG_H
#define __RADAR_CONFIG_H


#include "hal_BB.h"
#include "hal_RF.h"
#include "bsp_GPIO.h"

//const max len
#define WIN_MAX_LEN      	      (2048)
#define CALIB_MAX_LEN     	    (16)
#define SINC_BUF_MAX_LEN  	    (128*4)
#define STEERING_VEC_MAX_LEN    (128*4*4)

#define PGA0_27DB								(0)
#define PGA0_23DB 							(3)
#define PGA1_0DB								(0)
#define PGA1_6DB	 							(1)
#define PGA1_12DB								(3)
#define PGA1_18DB	 							(7)
#define PGA2_0DB								(0)
#define PGA2_6DB	 							(1)
#define PGA2_12DB								(3)
#define PGA2_18DB	 							(7)
#define PGA3_0DB								(0)
#define PGA3_6DB	 							(1)
#define PGA3_12DB								(3)
#define PGA3_18DB	 							(7)

#define HPF1_200KHZ 						(0)
#define HPF1_300KHZ 						(1)
#define HPF1_400KHZ							(2)
#define HPF1_500KHZ 						(3)
#define HPF2_200KHZ 						(0)
#define HPF2_300KHZ 						(1)
#define HPF2_400KHZ							(2)
#define HPF2_600KHZ 						(3)
#define LPF_8MHZ								(0)
#define LPF_5MHZ								(1)
#define LPF_4MHZ								(2)
#define LPF_3_2MHZ							(3)
#define LPF_3_5MHZ							(4)
#define LPF_2_5MHZ							(5)
#define LPF_2MHZ							  (6)
#define LPF_1_5MHZ							(7)

#define MAX_FRAME_NUM           (7)   //最大存储帧数

// #define COMPARE_DATA
typedef struct __RADAR_USER_CFG__{
	//RF
	float freqStart;
	float Bandwidth;
	float sweepTimeA;
	float sweepTimeB;
	float sweepTimeC;	
	uint8_t sweepWaveNum;
	uint8_t rxGain0;
	uint8_t rxGain1;
	uint8_t rxGain2;
	uint8_t rxGain3;	
	uint8_t hpf1;
	uint8_t hpf2;
	uint8_t lpf;
	//Dataprep
	uint8_t cicSec;
	uint8_t downFac;
	uint16_t sampPT;
	uint16_t removePT;
	uint8_t sampBit;
	uint8_t numTX;
	uint8_t numRX;
	uint8_t cfarAnt;
	uint16_t numChirp;
	//param
	float rangeRes;
	float velRes;
  //alg
	uint8_t cfarThV;
	uint8_t cfarThVG;
	uint8_t cfarThVS;
	uint16_t cfarMaxNum;
	uint8_t peakenR;
	uint8_t peakenV;
}RADAR_USER_CFG;
typedef struct __MEM_ADDR_CONFIG__{
	uint32_t zeros;
	uint32_t winRange;
	uint32_t winVel;
	uint32_t winVel1;
	uint32_t winVel2;
	uint32_t winVel3;
	uint32_t winVel4;
	uint32_t steeringVec;
	uint32_t vDftArray;
	uint32_t a1DftArray;
	uint32_t a2DftArray;
	uint32_t a3DftArray;
	uint32_t aDftArray;
	uint32_t eDftArray;
	uint32_t sincBuf;
	uint32_t adcSamp;
	uint32_t fft1d;
	uint32_t fft1dMean;
	uint32_t fft1dMeanTmp;
	uint32_t fft1dMeanLast;
	uint32_t fft1dMean_0; //本帧FFT1D均值临时存放
	uint32_t fft1dWinMean_1;
	uint32_t fft1dMean0_1;
	uint32_t pfloatMaxVal;
	uint32_t fft2d;
	uint32_t fft2dSumAbs_1;
	uint32_t fft2dSumAbs_2;
	uint32_t fft2dSumPf;
	uint32_t fft1dMean0_1_SumAbs;
	uint32_t fft1dMean0_1_SumPf;
	uint32_t fft2dVel32;
	uint32_t cfarRsltV;
	uint32_t cfarRes;
	uint32_t cfarRsltTmp;
	uint32_t cfarRslt0_1;
	uint32_t cfarRslt0_2;
	uint32_t cfarRslt0_3;	
	uint32_t cfarRslt0_4;
	uint32_t cfarRslt0_5;	
	uint32_t cfarRslt0_6;	
	uint32_t cfarResPf;
	uint32_t cfarResAbs;
	uint32_t cfarRslt;
	uint32_t targetAllAnt_Step1;
	uint32_t targetAllAnt_Step2;
	uint32_t u2BaseComp;
	uint32_t m2RxDiffSum;
	uint32_t m2RxDiffSumDft;
	uint32_t d2IdxRslt;
	uint32_t fft3dPre;
	
	
	uint32_t fft3dRslt;
	uint32_t dpkDstFirst;
	uint32_t dpkDstMinus;
	uint32_t dpkSrcA;
	uint32_t dpkSrcB;
	uint32_t dpkRst;
	uint32_t d3Idx;
	uint32_t fft3dValid;
	uint32_t t23Comp;
	uint32_t fft4dPre;
	uint32_t fft4dRslt;
	uint32_t frameRstD;
	uint32_t frameRstS0_1;
	uint32_t protocolPack;
	uint32_t pointRst;
	//abs
	uint32_t fft2dSumPfAbs;
	//cpx
	uint32_t fft2dSumCpx;
	uint32_t m2RxDiffSumCpx;
	uint32_t m2RxDiffSumDftCpx;
	uint32_t fft3dRsltCpx;
}MEM_ADDR_CONFIG;

typedef struct __RADAR_ALG_CFG__{	
	/*alg*/
	uint32_t isrSet;
	uint32_t isrRead;
	uint16_t numSamp;
	uint16_t numChirp;
	uint16_t useRange;
	uint16_t numVel;
	uint8_t meanEn;
	uint8_t numUnfoldDFT;
	uint8_t numByte;
	uint8_t numRX;
	uint8_t numRXT1;
	uint8_t rxIdxU2;
	uint8_t rxIdxT2;
	uint8_t rxIdxT3;
	uint8_t rxIdxT4;
	uint8_t rxIdxT5;
	
//	uint8_t numRXT12;
//	uint8_t numRXT21;
	uint8_t numTX;
	uint8_t numANT;
	uint8_t numAntA;
	uint8_t numAntE;
	uint8_t secNum;
	uint8_t antA_inc;
	uint16_t numAngle;
	uint8_t dpkTimes;
	uint8_t dpkLenLog;
	uint8_t dpkThres;
	uint8_t cfarDMul;   //动态点Cfar门限
	uint8_t cfarSMul;   //静态点Cfar门限
	uint8_t rangePeakRatioMul;
	uint8_t rangePeakRatioDiv;
	uint8_t frameGap;
	uint8_t cfarAnt;
	uint8_t fft1dPT;
	uint8_t fft2dPT;
//	uint8_t fft3dPT;
//	uint8_t fft4dPT;
	uint8_t fft1dMeanDiv;
	uint16_t fft1d_useA;
	uint16_t fft1d_useB;
	uint16_t fft2d_useA;
	uint16_t fft2d_useB;
	uint16_t cfarMaxNum;
	uint16_t frameMaxNum;
	uint16_t fft3d_useA;
	uint16_t fft3d_useB;
	uint16_t fft3dShift;
	uint16_t fft4d_useA;
	uint16_t fft4d_useB;
	uint16_t fft4dShift;

	uint16_t cfarNumD;
	uint32_t cfarNumS[6];		
	uint32_t targetNumD;
	uint32_t targetNumS[6];
	uint8_t numSum;
	/*mul*/
	uint16_t numSampByte;
	uint16_t numSamp_ANT;
	uint16_t numSamp_ANT_Div4;
	uint32_t numSamp_ANT_x2;
	uint32_t numSamp_ANT_Byte;
	uint32_t numSamp_Chirp_ANT_Byte;
	uint16_t useRange_ANT;
	uint32_t useRange32_ChirpDiv2;
	uint16_t useRange32_ChirpDiv4;
	uint16_t useRange32Div2;
	uint16_t useRange32_ANT;
	uint16_t useRange32_ANTx2;
	uint32_t useRange32_ANT_ChirpDiv4;
	uint16_t useRange32;
	uint16_t useRange32_x2;
	uint16_t useRange64;
	uint32_t useRange64_Chirp;
	uint16_t useRangeDiv2;
	uint16_t useRange_x2;
	uint16_t numChirp32;
	uint16_t numChirpDiv2;
	uint16_t numChirpDiv4;
	uint16_t numChirpDiv32;
	
	uint16_t numVelDiv2;
	uint16_t numRX32;
	uint16_t numRX64;
	uint16_t rxIdxU2_32;
	uint16_t rxIdxT2_32;
	uint16_t rxIdxT3_32;
	uint16_t rxIdxT4_32;
	uint16_t rxIdxT5_32;
	uint16_t numUnfoldDFT32;
	 
	
	uint16_t numANT32;
	uint16_t antA_inc32;
	uint8_t fft1dMeanDivDiv2;
	uint16_t numAntA_32;
	uint16_t numAntE32;
//	uint16_t numAngle_ANT_A_32;
	uint16_t numAngle32;
	uint16_t numAngle64;
	/*memAddr*/
	MEM_ADDR_CONFIG mem;	
}RADAR_ALG_CFG;

typedef struct __FFT1D_WINVEL_MEAN__
{
	uint32_t mean[4096];
} FFT1D_WINVEL_MEAN;

typedef struct __FFT1D_MEAN_BUF__
{
	FFT1D_WINVEL_MEAN fft1dWinVel[7];
} FFT1D_MEAN_BUF;

typedef struct __FFT1D_MEANSUB_BUF__
{
	FFT1D_WINVEL_MEAN fft1dWinVel[6];
} FFT1D_MEANSUB_BUF;

//const
extern const uint16_t winVelPre[];
extern const uint32_t sincBuf_Pre[];
extern const uint32_t nco_fcw[];
extern const uint32_t calib_ap[];

//struct
extern RADAR_ALG_CFG  algCfg;
extern RADAR_USER_CFG radarUserCfg;
void Radar_Config_Init(RADAR_USER_CFG *usrCfg);

#endif

