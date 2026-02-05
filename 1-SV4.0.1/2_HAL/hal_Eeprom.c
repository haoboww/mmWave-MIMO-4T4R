#include "hal_Eeprom.h"
#include "bsp_I2C.h"
#include "string.h"
#include <stdint.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "delay.h"



#define   TASK_RUN  1
#define   EEP_TASK_PRIO      (configMAX_PRIORITIES-1) 
#define   EEP_STK_SIZE    512 //任务堆栈大小

static TaskHandle_t EepromWrite_Task_Handler = NULL;


REMAIN_FIFO_Struct Remain_FIFO;
REMAIN_FIFO_Struct Remain_FIFO2;

INT32 EepromWriteData(UINT16 addr, UINT8 *data, UINT32 len, UINT8 task);
INT32 EepromWriteTaskForFIFO(void);
INT32 WriteDataToEepFifo(UINT16 memaddr,UINT8 *data, UINT32 length);

SemaphoreHandle_t Mutex_eeprom_flag;


/*****************************************************************************
 函 数 名  : Eeprom_Init
 功能描述  : EEPROM初始化，包括I2C总线初始化和FIFO初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
void Eeprom_Init(void)
{	
   	Remain_FIFO.ReadIndex  = 0;
	Remain_FIFO.WriteIndex = 1;

	for(UINT8 idata = 0;idata < EEP_MAX_FIFO_LEN;idata++)
	{
		Remain_FIFO.Remain_EEp[idata].valid = 0;
	}

    Mutex_eeprom_flag = xSemaphoreCreateMutex();

	if(pdPASS != xTaskCreate((TaskFunction_t )EepromWrite_Task,
					(const char*    )"EepromWrite",
					(UINT16       )EEP_STK_SIZE,
					(void*          )NULL,
					(UBaseType_t    )EEP_TASK_PRIO,
					(TaskHandle_t*  )&EepromWrite_Task_Handler))
	{
		printf("create EepromWrite task error\r\n");
	} 
}


/*****************************************************************************
 函 数 名  : EepromWrite_Task
 功能描述  : EEPROM 写任务，负责USD和参数设置的EEPROM写入管理
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 无
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
void EepromWrite_Task(void *pvParameters)
{
	INT32 udsWriteFlag = 0;

    while(1)
    {
		if(ulTaskNotifyTake(pdFALSE,portMAX_DELAY))
        {
			for(;;)
			{
				udsWriteFlag = EepromWriteTaskForFIFO();

				if(udsWriteFlag)
				{
					break;
				}
			}
		}
		else
		{
			taskYIELD();
		}
    }	
}



/*****************************************************************************
 函 数 名  : EepromWriteTaskForFIFOUds
 功能描述  : 读取USD FIFO数据 写入EEPROM
 输入参数  : void  
 输出参数  : 无
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 EepromWriteTaskForFIFO(void)
{
	UINT8 readIndex = 0;
	/*判断是否现在FIFO已经还没写入或者已经读完*/

	if((Remain_FIFO.WriteIndex - Remain_FIFO.ReadIndex == 1)  || ((Remain_FIFO.ReadIndex - Remain_FIFO.WriteIndex) == (EEP_MAX_FIFO_LEN - 1)))
	{
		readIndex = (Remain_FIFO.ReadIndex + 1) % EEP_MAX_FIFO_LEN;
        if(Remain_FIFO.Remain_EEp[readIndex].valid == 0)
        {
            return 1;
        }
	}

	/*读一次FIFO,偏移一次读指针*/
	Remain_FIFO.ReadIndex = (Remain_FIFO.ReadIndex + 1) % EEP_MAX_FIFO_LEN;

	/*对EEPROM写一次数据*/
	if(Remain_FIFO.Remain_EEp[Remain_FIFO.ReadIndex].valid == 1)
	{
		EepromWriteData(Remain_FIFO.Remain_EEp[Remain_FIFO.ReadIndex].Addr,  &Remain_FIFO.Remain_EEp[Remain_FIFO.ReadIndex].Data[0], Remain_FIFO.Remain_EEp[Remain_FIFO.ReadIndex].Len, TASK_RUN);
		Remain_FIFO.Remain_EEp[Remain_FIFO.ReadIndex].valid = 0;				
	}
	return 0;
}


/*****************************************************************************
 函 数 名  : EepromWriteTaskForFIFONormal
 功能描述  : 读取参数设置 FIFO数据 写入EEPROM
 输入参数  : void  
 输出参数  : 无
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 EepromWriteTaskForFIFONormal(void)
{
	UINT8 readIndex = 0;
	/*判断是否现在FIFO已经还没写入或者已经读完*/
	if((Remain_FIFO2.WriteIndex - Remain_FIFO2.ReadIndex == 1)  || ((Remain_FIFO2.ReadIndex - Remain_FIFO2.WriteIndex) == (EEP_MAX_FIFO_LEN - 1)))
	{
		readIndex = (Remain_FIFO2.ReadIndex + 1) % EEP_MAX_FIFO_LEN;
        if(Remain_FIFO2.Remain_EEp[readIndex].valid == 0)
        {
            return 1;
        }
	}

	/*读一次FIFO,偏移一次读指针*/
	Remain_FIFO2.ReadIndex = (Remain_FIFO2.ReadIndex + 1) % EEP_MAX_FIFO_LEN;

	/*对EEPROM写一次数据*/
	if(Remain_FIFO2.Remain_EEp[Remain_FIFO2.ReadIndex].valid == 1)
	{
		EepromWriteData(Remain_FIFO2.Remain_EEp[Remain_FIFO2.ReadIndex].Addr, &Remain_FIFO2.Remain_EEp[Remain_FIFO2.ReadIndex].Data[0], Remain_FIFO2.Remain_EEp[Remain_FIFO2.ReadIndex].Len, TASK_RUN);
		Remain_FIFO2.Remain_EEp[Remain_FIFO2.ReadIndex].valid = 0;

	}
	return 0;
}



/*****************************************************************************
 函 数 名  : EepromWriteData
 功能描述  : EEPROM写入函数
 输入参数  :  UINT16 addr
			 UINT8 *data
			 UINT32 len
			 UINT8 task  
 输出参数  : 无
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 EepromWriteData(UINT16 addr, UINT8 *data, UINT32 len, UINT8 task)
{
	INT32 resault = E_OK;
	
	if(task)
	{
		/*在任务中写EEPROM */
		xSemaphoreTake(Mutex_eeprom_flag, portMAX_DELAY);
		i2c_write_data(addr, data, len);
		vTaskDelay(5);
		xSemaphoreGive(Mutex_eeprom_flag);
	}
	else
	{
		/*任务未开启前的主函数中写EEPROM */
		resault = i2c_write_data(addr, data, len);
		HW_Delay_ms(5);
	}	
    return resault;
}




/*****************************************************************************
 函 数 名  : ReadFromEepromTask
 功能描述  : EEPROM 在任务中读
 输入参数  : UINT16 memaddr 
			 UINT32 length 
 输出参数  : UINT8 *data
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 ReadFromEepromTask(UINT16 memaddr ,UINT8 *data, UINT32 length)
{
	INT32 result = E_OK;

	xSemaphoreTake(Mutex_eeprom_flag, portMAX_DELAY);

	result = i2c_read_data(memaddr, data, length);

	xSemaphoreGive(Mutex_eeprom_flag);
	
	return result;
}

/*****************************************************************************
 函 数 名  : ReadFromEepromMain
 功能描述  : EEPROM 在主函数中读
 输入参数  : UINT16 memaddr 
			 UINT32 length 
 输出参数  : UINT8 *data
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 ReadFromEepromMain(UINT16 memaddr ,UINT8 *data, UINT32 length)
{
	INT32 result = E_OK;

	result = i2c_read_data(memaddr, data, length);
	
	return result;
}


/*****************************************************************************
 函 数 名  : ReadByte2Eep
 功能描述  : EEPROM 读函数
 输入参数  : UINT16 memaddr 
			 UINT32 length 
			 UINT8 type
 输出参数  : UINT8 *pdata
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 ReadByte2Eep(UINT16 memaddr,UINT8 *pdata, UINT32 length, UINT8 type)
{
	INT32  result = E_OK;
	if(type == 0)
	{
		result = ReadFromEepromMain(memaddr,pdata,length);
	}
	else
	{
		result = ReadFromEepromTask(memaddr,pdata,length);
	}
	return result;
}


/*****************************************************************************
 函 数 名  : WriteByte2Eep
 功能描述  : EEPROM 写函数
 输入参数  : UINT16 memaddr 
			 UINT8 *pData
			 UINT32 length 
			 UINT8 type
 输出参数  : 无
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 WriteByte2Eep(UINT16 memaddr,UINT8 *pData, UINT32 length, UINT8 type)
{
 	UINT32 pageSize = 0;
    UINT32 iPage = 0;
    UINT16 writeAddr = 0;
    UINT16 writeSize = 0;
    INT32  status = 0;
    UINT16 firstPageSize = 0;
    
    UINT32 pageStartAddr = 0;
       
    pageStartAddr = memaddr % EEP_PAGE_SIZE;  /*起始地址求余*/
    firstPageSize = EEP_PAGE_SIZE - pageStartAddr; /*计算首页剩余字节数量*/
    
    if(length > firstPageSize)/*当写入数据长度跨页时*/
    {
        writeAddr = memaddr; 

		if(type == 0)
		{
			status = EepromWriteData(writeAddr,  &pData[0], firstPageSize, 0);/*写入首页*/
		}
		else
		{
			status = WriteDataToEepFifo(writeAddr, &pData[0], firstPageSize);/*写入首页*/
		}
        pageSize = (length - firstPageSize) / EEP_PAGE_SIZE; /*计算剩余整页数*/
        if(((length - firstPageSize) % EEP_PAGE_SIZE) > 0)/*判断最后一页是否有剩余*/
        {
            pageSize++;
        } 
        for(iPage = 0; iPage < pageSize; iPage++)
        {
            writeAddr = memaddr + firstPageSize + iPage*EEP_PAGE_SIZE;
            if((((length - firstPageSize) - iPage*EEP_PAGE_SIZE)/EEP_PAGE_SIZE) > 0)
            {
                writeSize = EEP_PAGE_SIZE;
            }
            else
            {
                writeSize = (length - firstPageSize) - iPage*EEP_PAGE_SIZE;
            }            
			if(type == 0)
			{
				status = EepromWriteData(writeAddr, &pData[iPage*EEP_PAGE_SIZE + firstPageSize], writeSize, 0);/*写入首页*/
			}
			else
			{
				status = WriteDataToEepFifo(writeAddr, &pData[iPage*EEP_PAGE_SIZE + firstPageSize], writeSize);/*写入首页*/
			}
		}        
    }
    else
    {
        pageSize = length / EEP_PAGE_SIZE; 
        if((length%EEP_PAGE_SIZE) > 0)
        {
            pageSize++;
        } 
        for(iPage = 0; iPage < pageSize; iPage++)
        {
            writeAddr = memaddr + iPage*EEP_PAGE_SIZE;
            if(((length  - iPage*EEP_PAGE_SIZE)/EEP_PAGE_SIZE) > 0)  
            {
                writeSize = EEP_PAGE_SIZE;
            }
            else
            {
                writeSize = length  - iPage*EEP_PAGE_SIZE;
            }

			if(type == 0)
			{
				status = EepromWriteData(writeAddr, &pData[iPage*EEP_PAGE_SIZE], writeSize, 0);/*写入首页*/
			}
			else
			{
				status = WriteDataToEepFifo(writeAddr, &pData[iPage*EEP_PAGE_SIZE], writeSize);/*写入首页*/
			}
        }         
    }

    return status;
}


/*****************************************************************************
 函 数 名  : WriteDataToEepFifo
 功能描述  : Uds写EEPROM FIFO
 输入参数  : UINT16 memaddr 
			 const UINT8 *data
			 UINT32 length
 输出参数  : 无
 返 回 值  : INT32
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
INT32 WriteDataToEepFifo(UINT16 memaddr,UINT8 *data, UINT32 length)
{
	INT32  idata = 0;

	if(Remain_FIFO.Remain_EEp[Remain_FIFO.WriteIndex].valid == 0)
	{
		Remain_FIFO.Remain_EEp[Remain_FIFO.WriteIndex].Addr= memaddr;
		Remain_FIFO.Remain_EEp[Remain_FIFO.WriteIndex].Len = length;
		for(idata = 0; idata < length; idata ++)
		{
				Remain_FIFO.Remain_EEp[Remain_FIFO.WriteIndex].Data[idata] = data[idata];
		}
		Remain_FIFO.Remain_EEp[Remain_FIFO.WriteIndex].valid = 1;
		Remain_FIFO.WriteIndex = (Remain_FIFO.WriteIndex+1) % EEP_MAX_FIFO_LEN;

		xTaskNotifyGive(EepromWrite_Task_Handler);
		return 0;
	}
	else
	{
		return 1;
	}
}



/*****************************************************************************
 函 数 名  : EepromSelfCheck
 功能描述  : EEPROM 自检
 输入参数  : void 

 输出参数  : 无
 返 回 值  : UINT8
 
 修改历史      :
  1.日    期   : 2022年11月25日
    作    者   : YangJie
    修改内容   : 新生成函数

*****************************************************************************/
UINT8 EepromSelfCheck(void)
{
	UINT8 eWriteBuf[2] = {0x4f,0x5f};
	UINT8 eReadBuf[2];
	INT32 result = E_OK;
    result = WriteByte2Eep(EEP_SELF_CHECK_ADDR, eWriteBuf, sizeof(eWriteBuf),TASK_FIFO_MODE);
    vTaskDelay(20);
    result = ReadByte2Eep(EEP_SELF_CHECK_ADDR, eReadBuf, sizeof(eReadBuf),TASK_FIFO_MODE);
	if((eWriteBuf[0] != eReadBuf[0]) && (eWriteBuf[1] != eReadBuf[1]))
	{
		printf("Eeprom Test Fail!\r\n");
		result = 2;
	}
	else
	{
		printf("Eeprom Test OK!\r\n");
		result = 1;
	}
	return result;
}


#if EEP_TEST_FLAG

void MainWrEepromTest(void)
{
	
	UINT32 wrCnt = 0;
	UINT32 rdCnt = 0;
	UINT8 shift = 0;
	UINT8 wrData[256] = {0};
	UINT8 rdData[256] = {0};
	INT32 state = 0;
	for(wrCnt = 0; wrCnt < 256; wrCnt++)
	{
		wrData[wrCnt]   = wrCnt + 1;   
	} 

	state = WriteByte2Eep(0, wrData, 128, 0);

	memset(rdData,0,256);
	state = ReadByte2Eep(0, rdData, 128, 0);
	

	for(rdCnt = 0; rdCnt < 256; rdCnt++)
	{
		printf("M:%d\n",rdData[rdCnt]);
	}



}

void TimeWrEepromTest(void)
{
	UINT32 wrCnt = 0;
	UINT32 rdCnt = 0;
    static UINT8 sendCnt = 0;
	UINT8 wrData[72] = {0};
	UINT8 rdData[72] = {0};
	INT32 state = 0;
	char strData[12] = {0};
	int strLen = 0;
	for(wrCnt = 0; wrCnt < 72; wrCnt++)
	{
		wrData[wrCnt]   = wrCnt + 2;

	} 

	memset(rdData,0,72);
	
	state = ReadByte2Eep(0, rdData, 72,1);


	printf("Ir:%2d_%2d\n",sendCnt, rdData[sendCnt]);


    state = WriteByte2Eep(0, wrData, 72,1);

    sendCnt++;    
    if(sendCnt > 71)
    {
    	sendCnt = 0;
    } 


	printf("Iw:%2d_%2d\n",sendCnt, wrData[sendCnt]);
 
	
}


void TaskWrEepromTest(void)
{
	UINT8 wrCnt = 0;
	UINT8 rdCnt = 0;
    static UINT8 sendCnt = 0;
	UINT8 wrData[72] = {0};
	UINT8 rdData[72] = {0};
	INT32 state = 0;
	char strData[12] = {0};
	int strLen = 0;
	
	for(wrCnt = 0; wrCnt < 72; wrCnt++)
	{
		wrData[wrCnt]   = wrCnt + 3;   
	} 



	memset(rdData,0,72);

	state = ReadByte2Eep(128, rdData, 72, 2);
	
	
    print_usart0("Ar:%2d_%2d\n", sendCnt, rdData[sendCnt]);


	state = WriteByte2Eep(128, wrData, 72,2); 



    sendCnt++;    
    if(sendCnt > 71)
    {
    	sendCnt = 0;
    } 

	print_usart0("Aw:%2d_%2d\n", sendCnt, wrData[sendCnt]);

}	


#endif
