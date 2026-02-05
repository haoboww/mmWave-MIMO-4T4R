#include "main.h"
/*
	chip init 
	include bsp init ,hal init
*/
void Sys_Init(void)
{
		Set_CoreLDO_Voltage(2);
    LRC_DISABLE;   // Close LRC
    SET_WKUP_MASK0(0xFFFFFFFF);
    SET_WKUN_MASK0(0xFFFFFFFF);
    CLR_WKUP_MASK0(GPIO1_10_WKU_MSK);  //GPIO1.10 Wakeup Enable
    CLR_WKUN_MASK0(GPIO1_10_WKU_MSK);  //GPIO1.10 Wakeup Enable
    SET_WKUP_MASK1(0xFFFFFFFF);
    SET_WKUN_MASK1(0xFFFFFFFF);
    SET_WKUP_MASK2(0xFFFFFFFF);

    Set_Flash_WS(0xF);  //flash ws
		NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_3); //group4：抢占优先级0-15，响应优先级0
		BSP_UART_Init(); //systemClock/4

    BSP_GPIO_Init();
    BSP_SPI_Init();	
		stHRC.ibas=8 ;stHRC.cap=0 ;stHRC.div=1;
		stPLL.hxtal_freq=XTAL_40M;stPLL.prediv=0;stPLL.mdiv=4;stPLL.posdiv=0;//hxtal_freq,prediv,mdiv,posdiv   ((hxtal_freq / (1 + prediv)) * (8 + mdiv)) / (2 + posdiv * 2)

    HAL_SYS_CLK_Init(&stHRC,&stPLL);
		Set_Flash_WS(0x4);
		DMA_Config_Init();
		BSP_ADC_Init();
		WDT_Disable();
		HAL_MPU_Init();

    OSPI_MasterInit();	
    OSPI0->CON1 |=(4 << OSPI_CON1_DIV_Pos)          ; // clk_div		
		SysTick_Init();
//		printf("pclk %d\n",pclk);

}/*}}}*/

/*
	APP Init, include bb init,
*/

void App_Init(void){
	ParamConfigInit();
	Radar_Config_Init(&radarUserCfg);
}


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);
int main(void)
{
    Sys_Init();
    App_Init();
    //创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄 

     

   vTaskStartScheduler(); //
}/*}}}*/
//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
		xSemaphoreBB = xSemaphoreCreateBinary();
    CreateBBTask();   
    CreatePrintfTask();
		TMRB0_Init();
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}
/*
是FreeRTOS中的一个回调函数，
它允许您在系统处于空闲状态时执行自定义的应用代码。
这个回调函数在系统没有更多任务要运行时被调用，
通常用于执行低优先级的后台任务或系统监控
请注意，您应该确保执行的代码是非阻塞的，以允许其他任务在需要时运行
*/
void vApplicationIdleHook(void) {
//	CPUMonitor_IdleHandle();
//	BBMonitor_IdleHandle();
	WDT_CLEAR_COUNTER;
}
volatile uint8_t waveStartEn=0;
volatile uint32_t bbTimCnt=0;
volatile uint32_t CPU_RunTime;
volatile uint32_t bb_RunTime=0;
void userTimerB0CallBackHand(void){
	bbTimCnt++;
	if(bbTimCnt>=1){
		bbTimCnt=0;
		if(waveStartEn){
			RF_waveStart();
			RF_WriteRegSPI(CS1_PIN, ANA_CFG22_ADDR, 0x9000);
			waveStartEn = 0;
		}
		if(BB_AlgProcFinished){
			BB_Task_Trigger();				
		}
//		BaseType_t xBBTaskWoken = pdFALSE;
//		vTaskNotifyGiveFromISR(BBTask_Handler,&xBBTaskWoken);
//		portYIELD_FROM_ISR(xBBTaskWoken);
//		printf("tim\n");
//		bbTimCnt=0;
	}
	CPU_RunTime++;
}

