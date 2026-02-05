
#ifndef _HAL_EEPROM_H
#define _HAL_EEPROM_H

#include "ColigenDataType.h"
#include "stdint.h"

#define  E_OK    (0)

#define EEP_PAGE_SIZE   			  64
#define EEP_MAX_PAGE              	  512

#define EEP_MAX_FIFO_SIZE             64

#define EEP_MAX_FIFO_LEN             	32

#define EEP_TEST_FLAG   0

#define EEP_SELF_CHECK_ADDR   		  0x7D00
enum EEPROM_PORT_TYPE
{
    HARD_DELAY_MODE = 0,        //0、直接写主函数使用
    UDS_FIFO_MODE,          //1、UDS写FIFO
	TASK_FIFO_MODE          //2、任务写FIFO
};


typedef struct
{
		UINT8  Data[EEP_MAX_FIFO_SIZE];
		UINT8  valid;
		UINT16 Len;
		UINT16 Addr;

}REMAIN_EEP_Struct;

typedef struct 
{
		REMAIN_EEP_Struct Remain_EEp[EEP_MAX_FIFO_LEN];             //最大缓存记录32
		UINT16 WriteIndex;
		UINT16 ReadIndex;
}REMAIN_FIFO_Struct;



extern void Eeprom_Init(void);//初始化函数 

extern INT32 WriteByte2Eep(UINT16 memaddr,UINT8 *pdata, UINT32 length, UINT8 type);//写EEPROM type：0、直接写主函数使用；1、UDS写FIFO；2、任务写FIFO
extern INT32 ReadByte2Eep(UINT16 memaddr,UINT8 *pdata, UINT32 length, UINT8 type);//读EEPROM type：0、直接写主函数使用；1、UDS写FIFO；2、任务写FIFO

extern void EepromWrite_Task(void *pvParameters);//eeprom读写任务，放在定时器中

extern UINT8 EepromSelfCheck(void);

#if EEP_TEST_FLAG
extern void MainWrEepromTest(void);
extern void TimeWrEepromTest(void);
extern void TaskWrEepromTest(void);
#endif

#endif 





