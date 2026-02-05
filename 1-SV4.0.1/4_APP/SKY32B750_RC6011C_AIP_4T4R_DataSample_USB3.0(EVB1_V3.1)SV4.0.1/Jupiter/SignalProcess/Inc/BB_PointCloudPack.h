/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BB_POINTCLOUDPACK_H
#define _BB_POINTCLOUDPACK_H
#include <stdint.h>
#include "App_Def.h"


typedef struct{
	float dis;
	float vel;
	float angleAz;
	float angleEle;
	float amp;
	float noise;
	float snr;
}STRUCT_BB_POINTCLOUD;
extern volatile STRUCT_BB_POINTCLOUD  *stPointCloud;
uint32_t BB_PointCloudPack(STRUCT_FRAMEPOINT *frameRst);

#endif /*_BB_POINTCLOUDPACK_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























