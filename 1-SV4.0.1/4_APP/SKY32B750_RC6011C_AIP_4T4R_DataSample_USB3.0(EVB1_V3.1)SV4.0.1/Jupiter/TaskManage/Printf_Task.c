#include "Printf_Task.h"
#include "main.h"



#define   PRINTF_TASK_PRIO      (configMAX_PRIORITIES-1) 
#define   PRINTF_STK_SIZE    1024 //任务堆栈大小
TaskHandle_t PrintfTask_Handler; //任务句柄
void Printf_Task(void *pvParameters);



void CreatePrintfTask(void)
{
     //创建 基带 任务
     xTaskCreate((TaskFunction_t )Printf_Task, 
     (const char* )"Printf_Task", 
     (uint16_t )PRINTF_STK_SIZE, 
     (void* )NULL,
     (UBaseType_t )PRINTF_TASK_PRIO,
     (TaskHandle_t* )&PrintfTask_Handler); 
}
#define mainPRINT_BUFFER_SIZE 256
char pcWriteBuffer[mainPRINT_BUFFER_SIZE];
char pcStatusBuffer[mainPRINT_BUFFER_SIZE];

void PrintTaskList(void) {


    // 获取任务列表信息
    vTaskList(pcWriteBuffer);

    // 输出任务列表信息
    printf("Task List:\n%s", pcWriteBuffer);
		vTaskGetRunTimeStats(pcStatusBuffer);
		printf("CPU:\n%s", pcStatusBuffer);
}

void Printf_Task(void *pvParameters)
{
	while(1)
	{
		STRUCT_FRAMEPOINT *frameRst = (STRUCT_FRAMEPOINT *)algCfg.mem.frameRstD;			
		xSemaphoreTake(xSemaphoreBB, portMAX_DELAY);		
		getPointCloud(frameRst);		
	}
}

void Printf_Task_Trigger(void)
{
	BaseType_t xPrintfTaskWoken = pdTRUE;
	vTaskNotifyGiveFromISR(PrintfTask_Handler,&xPrintfTaskWoken);
	portYIELD_FROM_ISR(xPrintfTaskWoken);	
}

void CPUMonitor_IdleHandle(void){
	static uint32_t cnt=0;
	if(cnt>10000000){
		PrintTaskList();
		cnt=0;
	}
	cnt++;
}

