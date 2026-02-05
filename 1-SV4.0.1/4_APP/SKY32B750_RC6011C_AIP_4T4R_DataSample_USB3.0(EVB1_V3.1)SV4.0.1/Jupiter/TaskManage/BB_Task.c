#include "BB_Task.h"
#include "main.h"
#define   BB_TASK_PRIO      (configMAX_PRIORITIES-2) 
#define   BB_STK_SIZE    1024 //任务堆栈大小
TaskHandle_t BBTask_Handler; //任务句柄
void BB_Task(void *pvParameters);

xSemaphoreHandle xSemaphoreBB;

void CreateBBTask(void)
{
     //创建 基带 任务
     xTaskCreate((TaskFunction_t )BB_Task, 
     (const char* )"BB_Task", 
     (uint16_t )BB_STK_SIZE, 
     (void* )NULL,
     (UBaseType_t )BB_TASK_PRIO,
     (TaskHandle_t* )&BBTask_Handler); 
}





void BB_AlgProc_Loop(void)
{
	uint8_t rtn;
    switch(stFlashParam.projFunc)
    {
        case 0x01:
//            printf("01\r\n");
            rtn=Func01_Handle(); //sample data
            break;
#if 0				
        case 0x02: //各种功能代码，需要宏定义切换
            rtn=Func02_Handle(); //sample data
            break;
#endif
        default:
            break;
    }
		if(rtn==FALSE){
			printf("FUNC Error\n");
			WDT_RstNow();
			while(1);
		}
}

void BB_Task(void *pvParameters)
{
	while(1)
	{
		switch(stFlashParam.WorkMode)
		{
				case 0:  //Config Mode
						RecProtocol_Loop();
						break;
				case 1:  //Work Mode				
						BB_AlgProc_Loop();
						break;
//				case 2:     //Import Mode
//						ImportData_Loop();

//						break;
				default:
//            printf("wm=%d\r\n",r2_str->WorkMode);
						break;
		}
	}
}

void BB_Task_Trigger(void)
{
	BaseType_t xBBTaskWoken = pdTRUE;
	vTaskNotifyGiveFromISR(BBTask_Handler,&xBBTaskWoken);
	portYIELD_FROM_ISR(xBBTaskWoken);	
}

