#include "ParamConfig.h"
STRUCT_PARAM stFlashParam;




/*
	计算分辨率:
	RangeRes=3*10e8/(2*(采样点数/采样率)*(扫频带宽/扫频时间));
	VelRes=λ/(2*N*Tc);λ=c/freq;
	其中freq是初始频率
  c是光速
	λ为波长
  N为chirp个数
  Tc为chirp周期
*/
#if 0
static void ResolutionCount(void)
{
	float i,j;
	i=SAMPLE_LEN/ADC_SAMPLING_RATE;
	j=BANDWIDTH/SWEEPING_TIME;
	r2_str.RangeRes = 3.0e8f/(2*i*j);
	r2_str.VelRes = 3.0e8f/(FREQ_INIT*2*NUM_CHIRP*CHIRP_TIME);
}
#endif
/*
Read Parameters From Flash
 */
void Read_Param_From_Flash(void)
{
	stFlashParam.frameTime = Read_M32(FLASH_STANDBY_TIME_ADDR);
	if(stFlashParam.frameTime > AT_MAX_TIME)
	{
		stFlashParam.frameTime = 50;
	}
//	stFlashParam.projFunc    = Read_M32(FLASH_PROG_FUNC_ADDR);
	stFlashParam.projFunc    = 1;
	if((stFlashParam.projFunc < AT_MIN_PROG) || (stFlashParam.projFunc > AT_MAX_PROG))
	{
		stFlashParam.projFunc = 2;
	}
	stFlashParam.WorkMode     = 0;
	stFlashParam.cfarThVG     = Read_M32(FLASH_CFARTHVG_ADDR);
	if(stFlashParam.cfarThVG > AT_MAX_CFAR)
	{
		stFlashParam.cfarThVG = 8;
	}
	stFlashParam.cfarThVS     = Read_M32(FLASH_CFARTHVS_ADDR);
	if(stFlashParam.cfarThVS > AT_MAX_CFAR)
	{
		stFlashParam.cfarThVS = 8;
	}
	stFlashParam.cfarThV      = Read_M32(FLASH_CFARTHV_ADDR);
	if(stFlashParam.cfarThV > AT_MAX_CFAR)
	{
		stFlashParam.cfarThV = 8;
	}
	stFlashParam.cfarVmode      = Read_M32(FLASH_CFARVMODE_ADDR);
	if(stFlashParam.cfarVmode > 2)
	{
		stFlashParam.cfarVmode = 1; //0:GO,1:SO,2:CA
	}		
	stFlashParam.peakenV      = Read_M32(FLASH_PEAKENV_ADDR);
	if(!((stFlashParam.peakenV==0) || (stFlashParam.peakenV == 1)))
	{
		stFlashParam.peakenV = 0;
	}	
	stFlashParam.dpkThres     = Read_M32(FLASH_DPKTH_ADDR);
	if(stFlashParam.dpkThres > AT_MAX_DPK)
	{
		stFlashParam.dpkThres = 12;
	}
	stFlashParam.dpkTimes     = Read_M32(FLASH_DPKTIMES_ADDR);
	if(stFlashParam.dpkTimes > AT_MAX_DPKTIMES||stFlashParam.dpkTimes==0)
	{
		stFlashParam.dpkTimes = 2;
	}
	if((stFlashParam.numTLV < AT_MIN_NUMTLV) || (stFlashParam.numTLV > AT_MAX_NUMTLV))
	{
		stFlashParam.numTLV = 1;
	}
	stFlashParam.meanEn       = Read_M32(FLASH_MEAN_ADDR);
	if(stFlashParam.meanEn > 2)
	{
		stFlashParam.meanEn = 1;
	}	
	stFlashParam.zoTh       = Read_M32(FLASH_ZOTH_ADDR);
	if(stFlashParam.zoTh>=AT_MAX_ZO)
	{
		stFlashParam.zoTh = 10;
	}	
	stFlashParam.chirpGap = Read_M32(FLASH_CHIRPGAP_ADDR);
	if ((stFlashParam.chirpGap < AT_GAP_MIN) || (stFlashParam.chirpGap > AT_GAP_MAX))
	{
		stFlashParam.chirpGap = 80;
	}
	stFlashParam.bandWidth       = Read_M32(FLASH_BW_ADDR);
	if (!((stFlashParam.bandWidth == 0) || (stFlashParam.bandWidth == 1)))
	{
		stFlashParam.bandWidth = 0;
	}	
	stFlashParam.cfarThS      = Read_M32(FLASH_CFARTHS_ADDR);
	if(stFlashParam.cfarThS > AT_MAX_CFAR)
	{
		stFlashParam.cfarThS = 16;
	}	
	stFlashParam.frameGap = Read_M32(FLASH_FRAMEGAP_ADDR);
	if ((stFlashParam.frameGap < AT_FGAP_MIN) || (stFlashParam.frameGap > AT_FGAP_MAX))
	{
		stFlashParam.frameGap = 2;
	}
	stFlashParam.numSum = Read_M32(FLASH_NUMSUM_ADDR);
  if ((stFlashParam.numSum != 1) && (stFlashParam.numSum != 8) && (stFlashParam.numSum != 16))
	{
		stFlashParam.numSum = 16;
	}
}

void Default_Param_Config(void)
{
	/*App*/
	#ifdef COMPARE_DATA
	stFlashParam.projFunc = 2;
	stFlashParam.cfarThVG = 8;
	stFlashParam.cfarThVS = 8;
	stFlashParam.cfarThV = 4;
	stFlashParam.cfarThS = 4;
	stFlashParam.cfarVmode = 1;
	stFlashParam.meanEn = 1;
	stFlashParam.dpkThres = 12;
	stFlashParam.dpkTimes = 2;
	stFlashParam.zoTh = 10;
//	stFlashParam.numSum = 16;
	#endif
}

void Printf_Param(void){
	  printf("SoftVerison=SKY32B750_RC6011C_AIP_4T4R_DataSample_USB3.0(EVB1_V3.1)_SV3.9.2\r\n"); //SV.X.Y.Z   X:V2  Y:lib  Z:app
//		printf("RangeRes=%fm\r\n",stFlashParam.RangeRes);
//		printf("VelRes=%fm/s\r\n",stFlashParam.VelRes);
		/*Read Flash*/
		printf("TIME=%dms\r\n",stFlashParam.frameTime);
		printf("PROG=%02d\r\n",stFlashParam.projFunc);
		printf("cfarThVG=%02d\r\n", stFlashParam.cfarThVG);
		printf("cfarThVS=%02d\r\n", stFlashParam.cfarThVS);
		printf("cfarThV=%02d\r\n", stFlashParam.cfarThV);
		printf("cfarVmode=%02d\r\n",stFlashParam.cfarVmode);
		printf("peakenV=%02d\r\n",stFlashParam.peakenV);
		printf("dpkTh=%02d\r\n", stFlashParam.dpkThres);
		printf("dpkTimes=%02d\r\n", stFlashParam.dpkTimes);
		printf("numTLV=%02d\r\n", stFlashParam.numTLV);
		printf("meanEn=%02d\r\n", stFlashParam.meanEn);
//		printf("zoTh=%02d\r\n", stFlashParam.zoTh);
		printf("chirpGap=%dus\r\n", stFlashParam.chirpGap);
		printf("bandWidth=%d\r\n", stFlashParam.bandWidth);
		printf("cfarThS=%02d\r\n", stFlashParam.cfarThS);	
	  printf("frameGap=%d\r\n", stFlashParam.frameGap);
	  printf("numSum=%d\r\n", stFlashParam.numSum);
}

/*
	参数初始化：
*/
void ParamConfigInit(void)
{
		/*下面代码上电后只执行一次*/
		memset(&stFlashParam,0,sizeof(STRUCT_PARAM)); //清掉 r2结构体，地址95K，size 1K
//		ResolutionCount();	//计算距离分辨率
		Read_Param_From_Flash();
		Default_Param_Config();
		Printf_Param();
}
