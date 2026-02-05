#ifndef __BB_TSAK_H
#define __BB_TSAK_H
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
extern void CreateBBTask(void);



extern TaskHandle_t BBTask_Handler; //ÈÎÎñ¾ä±ú
extern xSemaphoreHandle xSemaphoreBB;
void BB_Task_Trigger(void);


#endif
