/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAIN_H
#define _MAIN_H
/*Local*/
#include <stdio.h>
/*core*/
#include "SKY32B750.h"
/*Driver*/

/*hal*/
#include "hal_Timer.h"
#include "hal_MPU.h"
#include "hal_Delay.h"
#include "hal_DMA.h"
#include "hal_SysClk.h"
#include "hal_RTC.h"
#include "hal_OSPI.h"


/*bsp*/
#include "bsp_SPI.h"
#include "bsp_GPIO.h"
#include "bsp_UART.h"
#include "bsp_ADC.h"

/*User*/
#include "RF_Config.h"
#include "BB_Config.h"
#include "Usart_Protocol.h"
#include "ParamConfig.h"
#include "IRQ_Handler.h"
/*singalProcess*/
#include "Radar_Config.h"
#include "BB_AlgProc.h"
#include "BB_PointCloudPack.h"
#include "BB_Config.h"
/*FreeRTOS*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "BB_Task.h"
#include "Printf_Task.h"


/*interrupt priority level  0-15 */
#define BB_PRIORITY_LV        6
#define UART_PRIORITY_LV      6
#define DMA_PRIORITY_LV       5
#define TMRB0_PRIORITY_LV     6
#define SYSTICK_PRIORITY_LV   7

#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bits for pre-emption priority                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bits for pre-emption priority                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority                                                                1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority
                                                                 0 bits for subpriority */
extern volatile uint8_t waveStartEn;
extern volatile uint32_t bbTimCnt;
extern volatile uint32_t bb_RunTime;


void userTimerB0CallBackHand(void);
#endif /*_MAIN_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























