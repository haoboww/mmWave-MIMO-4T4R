/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_CAN_H
#define _HAL_CAN_H
#include "SKY32B750.h"
#include "ColigenDataType.h"



extern INT32 SendDataToCanAInTask(UINT32 canId, UINT8 *pucData, UINT8 len);
extern INT32 SendDataToCanBInTask(UINT32 canId, UINT8 *pucData, UINT8 len);






#endif 
