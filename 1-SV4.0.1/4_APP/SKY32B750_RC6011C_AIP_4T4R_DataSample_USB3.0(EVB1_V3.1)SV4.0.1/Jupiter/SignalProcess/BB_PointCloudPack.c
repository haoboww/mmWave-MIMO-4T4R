#include "BB_PointCloudPack.h"
#include "main.h"
#define M_PI       (3.1415926f)
#define PHI_MULFAC  2.0f*(M_PI/180.0f) //单位：度
volatile STRUCT_BB_POINTCLOUD  *stPointCloud;
uint32_t BB_PointCloudPack(STRUCT_FRAMEPOINT *frameRst){
		uint32_t numPoint=0;
	#if 0

	float amp_dB,noise_dB; 
	for(uint32_t targetNow=0;targetNow<algCfg.frameTargetNum;targetNow++){
		if(frameRst[targetNow].d1IdxFine!=0xFFFF&&frameRst[targetNow].d2IdxFineRslt!=0x7FFF){
			stPointCloud[numPoint].dis=(float)frameRst[targetNow].d1IdxFine*0.0156f*radarUserCfg.rangeRes;
			stPointCloud[numPoint].vel=(float)frameRst[targetNow].d2IdxFineRslt*0.0156f*radarUserCfg.velRes;
			stPointCloud[numPoint].angleAz=(((float)frameRst[targetNow].d3IdxFine*0.0156f - 40) * PHI_MULFAC);
//			frameRst[targetNow].pow2dAbs=100000000;
//			frameRst[targetNow].noise=50000000;
			stPointCloud[numPoint].amp=(float)frameRst[targetNow].pow2dAbs;
			stPointCloud[numPoint].noise=(float)frameRst[targetNow].noise;
			amp_dB = 20.0f * log10f(stPointCloud[numPoint].amp);
			noise_dB = 20.0f * log10f(stPointCloud[numPoint].noise);
			stPointCloud[numPoint].snr = amp_dB - noise_dB + 7; // 7:为检测信噪比(cfar门限)
//			printf("amp_dB=%f,noise_dB=%f\n",amp_dB,noise_dB);
//			printf("frame:range=%f,vel=%f,azi=%f,pow2d=%f,noise=%f,snr=%f\n",stPointCloud[numPoint].dis,stPointCloud[numPoint].vel,stPointCloud[numPoint].angleAz,stPointCloud[numPoint].amp,stPointCloud[numPoint].noise,stPointCloud[numPoint].snr);
			numPoint++;		
		}
	}
	#endif
	return numPoint;
}
















