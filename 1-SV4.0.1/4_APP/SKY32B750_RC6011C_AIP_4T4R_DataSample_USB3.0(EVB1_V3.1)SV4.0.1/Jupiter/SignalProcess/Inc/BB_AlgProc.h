/* Define to prevent recursive inclusion -------------------------------------*/
#include "App_Def.h"

#ifndef _BB_ALGPROC_H
#define _BB_ALGPROC_H




/*Local*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
/*Driver*/


/*User*/

#include "ParamConfig.h"





extern volatile uint8_t BB_AlgProcFinished;

extern uint8_t Func01_Handle(void);
extern uint8_t Func02_Handle(void);
extern void getPointCloud(STRUCT_FRAMEPOINT *frameRst);
#endif

