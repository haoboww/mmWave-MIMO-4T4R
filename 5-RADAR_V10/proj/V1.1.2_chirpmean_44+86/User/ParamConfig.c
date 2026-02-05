#include "ParamConfig.h"
R2_STRUCT r2_str;




/*
	����ֱ���:
	RangeRes=3*10e8/(2*(��������/������)*(ɨƵ����/ɨƵʱ��));
	VelRes=��/(2*N*Tc);��=c/freq;
	����freq�ǳ�ʼƵ��
  c�ǹ���
	��Ϊ����
  NΪchirp����
  TcΪchirp����
*/
static void ResolutionCount(void)
{
	float i,j;
	i=SAMPLE_LEN/ADC_SAMPLING_RATE;
	if (r2_str.bandWidth == 1)
	{
		j=BANDWIDTH_6G5/SWEEPING_TIME;
	}
	else
	{
		j=BANDWIDTH_3G2/SWEEPING_TIME;
	}
	r2_str.RangeRes = 3.0e8f/(2*i*j);
	r2_str.VelRes = 3.0e8f/(FREQ_INIT*2*NUM_CHIRP*CHIRP_TIME);
}

/*
Read Parameters From Flash
 */
void Read_Param_From_Flash(void)
{
	r2_str.Standby_Time = Read_M32(FLASH_STANDBY_TIME_ADDR);
//	r2_str.Standby_Time = 100;
	if(r2_str.Standby_Time > ProtocolTimeMax)
	{
		r2_str.Standby_Time = 100;
	}
	r2_str.Proj_Func    = Read_M32(FLASH_PROG_FUNC_ADDR);
	if((r2_str.Proj_Func < ProtocolProgFuncMin) || (r2_str.Proj_Func > ProtocolProgFuncMax))
	{
		r2_str.Proj_Func = 2;
	}
	r2_str.WorkMode     = 0;
	r2_str.cfarThVG     = Read_M32(FLASH_CFARTHVG_ADDR);
	if(r2_str.cfarThVG > ProtocolCFARTHMax)
	{
		r2_str.cfarThVG = 2;
	}
	r2_str.cfarThVS     = Read_M32(FLASH_CFARTHVS_ADDR);
	if(r2_str.cfarThVS > ProtocolCFARTHMax)
	{
		r2_str.cfarThVS = 8;
	}
	r2_str.cfarThV      = Read_M32(FLASH_CFARTHV_ADDR);
	if((r2_str.cfarThV < ProtocolCFARTHMin) || (r2_str.cfarThV > ProtocolCFARTHMax))
	{
		r2_str.cfarThV = 8;
	}
	r2_str.dpkThres     = Read_M32(FLASH_DPKTH_ADDR);
	if(r2_str.dpkThres > ProtocolDPKTHMax)
	{
		r2_str.dpkThres = 10;
	}
	r2_str.numTLV       = Read_M32(FLASH_NUMTLV_ADDR);
	if((r2_str.numTLV < ProtocolNumTlvsMin) || (r2_str.numTLV > ProtocolNumTlvsMax))
	{
		r2_str.numTLV = 1;
	}
	r2_str.meanEn       = Read_M32(FLASH_MEAN_ADDR);
	if(!((r2_str.meanEn==0) || (r2_str.meanEn == 1) || (r2_str.meanEn == 2)))
	{
		r2_str.meanEn = 2;
	}
	r2_str.chirpGap = Read_M32(FLASH_CHIRPGAP_ADDR);
	if((r2_str.chirpGap < ChirpGapMin) || (r2_str.chirpGap > ChirpGapMax))
	{
		r2_str.chirpGap = 80;
	}
	r2_str.bandWidth       = Read_M32(FLASH_BW_ADDR);
	if(!((r2_str.bandWidth==0) || (r2_str.bandWidth == 1)))
	{
		r2_str.bandWidth = 0;
	}
}

void Default_Param_Config(void)
{
	r2_str.Standby_Time = 60;
	r2_str.Proj_Func = 2;
	r2_str.WorkMode = 0;
	/*App*/
	#ifdef COMPARE_DATA
	r2_str.cfarThVG = 2;//3   2
	r2_str.cfarThVS = 6;//6   4
	r2_str.cfarThV = 6;//6    4
	r2_str.dpkThres = 8;//6   0
	#else
	r2_str.cfarThVG = 2;//3   2
	r2_str.cfarThVS = 6;//6   4
	r2_str.cfarThV = 6;//6    4
	r2_str.dpkThres = 8;//6   0
	#endif
	r2_str.numTLV = 1;
	r2_str.chirpGap = 80;
	r2_str.meanEn = 2;
	r2_str.dpkThres = 8;
	r2_str.bandWidth=1;
}

void Printf_Param(void){
	    printf("SoftVerison=SKY32B750_PeopleTrack_V2.2.8.14\r\n"); //C.X.Y.Z   C:chip  X:hw  Y:lib  Z:app
		printf("RangeRes=%fm\r\n",r2_str.RangeRes);
		printf("VelRes=%fm/s\r\n",r2_str.VelRes);
		/*Read Flash*/
		printf("TIME=%dms\r\n",r2_str.Standby_Time);
		printf("PROG=%02d\r\n",r2_str.Proj_Func);
		printf("cfarThVG=%02d\r\n", r2_str.cfarThVG);
		printf("cfarThVS=%02d\r\n", r2_str.cfarThVS);
		printf("cfarThV=%02d\r\n", r2_str.cfarThV);
		printf("dpkThres=%02d\r\n", r2_str.dpkThres);
		printf("numTLV=%02d\r\n", r2_str.numTLV);
		printf("meanEn=%02d\r\n", r2_str.meanEn);
	  printf("chirpGap=%dus\r\n", r2_str.chirpGap);
		printf("bandWidth=%d\r\n", r2_str.bandWidth);
}

/*
	������ʼ����
*/
void ParamConfigInit(void)
{
		/*��������ϵ��ִֻ��һ��*/
		memset(&r2_str,0,sizeof(R2_STRUCT)); //��� r2�ṹ�壬��ַ95K��size 1K
		
		
	  Read_Param_From_Flash();
	  Default_Param_Config();
		ResolutionCount();	//�������ֱ���	
	
		//printf("******************\r\n");
		Printf_Param();
}



























