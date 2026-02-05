#include "hal_CAN.h"
#include "bsp_CAN.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "peripherals_can_pal1.h"
#include "peripherals_can_pal0.h"

SemaphoreHandle_t mutex_canA_flag;
SemaphoreHandle_t mutex_canB_flag;


INT32 SendDataToCanAInTask(UINT32 canId, UINT8 *pucData, UINT8 len)
{
	INT32 result = 0;
	UINT8  index = 0;
    can_message_t can_message;
    can_message.id = canId;
	for(index = 0;index < len;index++)
	{
		can_message.data[index]  = pucData[index];
	}
    can_message.length = len;
    xSemaphoreTake(mutex_canA_flag, portMAX_DELAY);
    result = CAN_SendBlocking(&can_pal0_instance, TX_MAILBOX, &can_message, 20);
//    CAN_Send(&can_pal0_instance, TX_MAILBOX, &can_message);
//    while (CAN_GetTransferStatus(&can_pal0_instance, TX_MAILBOX) == STATUS_BUSY);
    xSemaphoreGive(mutex_canA_flag);
    return result;
}

INT32 SendDataToCanBInTask(UINT32 canId, UINT8 *pucData, UINT8 len)
{
	INT32 result = 0;
	UINT8  index = 0;
    can_message_t can_message;
    can_message.id = canId;
	for(index = 0;index < len;index++)
	{
		can_message.data[index]  = pucData[index];
	}
    can_message.length = len;
    xSemaphoreTake(mutex_canB_flag, portMAX_DELAY);
    result = CAN_SendBlocking(&can_pal1_instance, TX_MAILBOX, &can_message, 20);
    xSemaphoreGive(mutex_canB_flag);
    return result;
}




#if USR_CANA_FIFO

void CanAFifoInit(void)
{
    CanAMsgTxFifo.ReadIndex  = 0;
    CanAMsgTxFifo.WriteIndex = 1;

    for(UINT8 idata = 0; idata < CAN_MAX_FIFO_LEN;idata++)
    {
        CanAMsgTxFifo.CanMsg[idata].valid = 0;
    } 

    if(pdPASS != xTaskCreate((TaskFunction_t )CanASendData_Task,
                        (const char*    )"CanASendData",
                        (UINT16       )1024,
                        (void*          )NULL,
                        (UBaseType_t    )TSK_PRIOR_HI,
                        (TaskHandle_t*  )&CanASendData_Task_Handler))
        {
                EMBARC_PRINTF("create CanASendData task error\r\n");
        }

}


UINT8 CanAUdsSendTask(void)
{
//    INT32 result = E_OK;
    CAN_MSG canMsg = {0};
//    UINT8 i = 0;
    UINT8 readIndex = 0;
    //判断是否现在FIFO已经还没写入或者已经读完
    if((CanAMsgTxFifo.WriteIndex - CanAMsgTxFifo.ReadIndex == 1)  || ((CanAMsgTxFifo.ReadIndex - CanAMsgTxFifo.WriteIndex) == (CAN_MAX_FIFO_LEN - 1)))
    {
        readIndex  = (CanAMsgTxFifo.ReadIndex + 1) % CAN_MAX_FIFO_LEN;  
        if(CanAMsgTxFifo.CanMsg[readIndex].valid == 0)
        {
            return 1;
        }
    }

   // 读一次FIFO,偏移一次读指针
    CanAMsgTxFifo.ReadIndex = (CanAMsgTxFifo.ReadIndex + 1) % CAN_MAX_FIFO_LEN;

    if(CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.ReadIndex].valid == 1)
    {
        //CAN发送一次数据
        canMsg = CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.ReadIndex];
        can_send_data(CANA_DEVICE0, canMsg.CanId, (UINT32 *)canMsg.Data,  canMsg.Len);
        CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.ReadIndex].valid = 0;
    }

    return 0;
}




INT32 SendDataToCanA(UINT32 canId, UINT8 *pucData, UINT8 len)
{
    UINT8  idata = 0;

    if(CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.WriteIndex].valid == 0)
    {

        CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.WriteIndex].CanId= canId;
        CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.WriteIndex].Len = len;
        for(idata = 0; idata < len; idata++)
        {
            CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.WriteIndex].Data[idata] = pucData[idata];
        }

        CanAMsgTxFifo.CanMsg[CanAMsgTxFifo.WriteIndex].valid = 1;

        CanAMsgTxFifo.WriteIndex = (CanAMsgTxFifo.WriteIndex + 1) % CAN_MAX_FIFO_LEN;

        xTaskNotifyGive(CanASendData_Task_Handler);
        return 0;
    }
    else
    {
        return 1;
    }  
}


void CanASendData_Task(void *pvParameters)
{
    UINT8 idleFlag = 0;

    while(1)
    {
        if(ulTaskNotifyTake(pdFALSE,0))
        {

            idleFlag =  CanAUdsSendTask(); 
            if(idleFlag)
            {
                break;
            }

        }
	    else
	    {
		taskYIELD();
	    }
    }
}
#endif


#if USR_CANB_FIFO

void CanBFifoInit(void)
{
    CanBMsgTxFifo.ReadIndex  = 0;
    CanBMsgTxFifo.WriteIndex = 1;

    for(UINT8 idata = 0; idata < CAN_MAX_FIFO_LEN;idata++)
    {
        CanBMsgTxFifo.CanMsg[idata].valid = 0;
    } 

    if(pdPASS != xTaskCreate((TaskFunction_t )CanBSendData_Task,
                        (const char*    )"CanBSendData",
                        (UINT16       )1024,
                        (void*          )NULL,
                        (UBaseType_t    )TSK_PRIOR_HI,
                        (TaskHandle_t*  )&CanBSendData_Task_Handler))
        {
                EMBARC_PRINTF("create CanBSendData task error\r\n");
        }

}


UINT8 CanBUdsSendTask(void)
{
//    INT32 result = E_OK;
    CAN_MSG canMsg = {0};
//    UINT8 i = 0;
    UINT8 readIndex = 0;
    //判断是否现在FIFO已经还没写入或者已经读完
    if((CanBMsgTxFifo.WriteIndex - CanBMsgTxFifo.ReadIndex == 1)  || ((CanBMsgTxFifo.ReadIndex - CanBMsgTxFifo.WriteIndex) == (CAN_MAX_FIFO_LEN - 1)))
    {
        readIndex  = (CanBMsgTxFifo.ReadIndex + 1) % CAN_MAX_FIFO_LEN;  
        if(CanBMsgTxFifo.CanMsg[readIndex].valid == 0)
        {
            return 1;
        }
    }

   // 读一次FIFO,偏移一次读指针
    CanBMsgTxFifo.ReadIndex = (CanBMsgTxFifo.ReadIndex + 1) % CAN_MAX_FIFO_LEN;

    if(CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.ReadIndex].valid == 1)
    {
        //CAN发送一次数据
        canMsg = CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.ReadIndex];
        can_send_data(CANB_DEVICE1, canMsg.CanId, (UINT32 *)canMsg.Data,  canMsg.Len);
        CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.ReadIndex].valid = 0;
    }

    return 0;
}


INT32 SendDataToCanB(UINT32 canId, UINT8 *pucData, UINT8 len)
{
    UINT8  idata = 0;

    if(CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.WriteIndex].valid == 0)
    {

        CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.WriteIndex].CanId= canId;
        CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.WriteIndex].Len = len;
        for(idata = 0; idata < len; idata++)
        {
            CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.WriteIndex].Data[idata] = pucData[idata];
        }

        CanBMsgTxFifo.CanMsg[CanBMsgTxFifo.WriteIndex].valid = 1;

        CanBMsgTxFifo.WriteIndex = (CanBMsgTxFifo.WriteIndex + 1) % CAN_MAX_FIFO_LEN;

        xTaskNotifyGive(CanBSendData_Task_Handler);
        return 0;
    }
    else
    {
        return 1;
    }  
}


void CanBSendData_Task(void *pvParameters)
{
    UINT8 idleFlag = 0;

    while(1)
    {
        if(ulTaskNotifyTake(pdFALSE,0))
        {

            idleFlag =  CanBUdsSendTask(); 
            if(idleFlag)
            {
                break;
            }

        }
	else
	{
		taskYIELD();
	}
    }
}

#endif

#if TEST_CAN_SEND


UINT8  testCanData[8] = {12,2,3,4,5,6,7,8};

void TestUdsCanASend(void)
{
    UINT8  iTest = 0;
    UINT32 testCanId = 0x100;

    UINT8  testCanLen = 0;

#if 1
    for(iTest = 0; iTest < CAN_MAX_FIFO_LEN; iTest++)
    {
        testCanId = iTest;
        testCanData[0] = iTest * 2 + 7;
        testCanData[1] = iTest * 2 + 6;
        testCanData[2] = iTest * 2 + 5;
        testCanData[3] = iTest * 2 + 4;
        testCanData[4] = iTest * 2 + 3;
        testCanData[5] = iTest * 2 + 2;
        testCanData[6] = iTest * 2 + 1;
        testCanData[7] = iTest * 2 + 0;
        testCanLen = iTest % 8 + 1;
        SendDataToCanAInTask(testCanId,testCanData,testCanLen);
//        SendDataToCanAInTask(0x10,testCanData,8);//testCanData 错误 发送的ID 数据错误
    }
#endif
//  SendDataToCanAInTask(testCanId,testCanData,8);
}


void TestUdsCanBSend(void)
{

    UINT8  iTest = 0;
    UINT32 testCanId = 0;
    UINT8  testCanLen = 0;

    for(iTest = 0; iTest < CAN_MAX_FIFO_LEN; iTest++)
    {
        testCanId = iTest;
        testCanData[0] = iTest * 2;
        testCanData[1] = iTest * 2 + 1;
        testCanData[2] = iTest * 2 + 2;
        testCanData[3] = iTest * 2 + 3;
        testCanData[4] = iTest * 2 + 4;
        testCanData[5] = iTest * 2 + 5;
        testCanData[6] = iTest * 2 + 6;
        testCanData[7] = iTest * 2 + 7;
        testCanLen = iTest % 8 + 1;
        SendDataToCanBInTask(testCanId,testCanData,testCanLen);
    }
}


void TestUdsCanARx(void)
{
    UINT8 i = 0; 
    if(IRQ_CAN0_RX == 1)
    {
        if((recvMsg_CAN0.id == RX_MSG_ID) && (recvMsg_CAN0.length > 0))
        {

            for (i = 0; i < recvMsg_CAN0.length; i++)
                print_usart0("0x%2X", recvMsg_CAN0.data[i]);
            print_usart0("\r\n");
            memset(&recvMsg_CAN0, 0, sizeof(recvMsg_CAN0));
//            CAN_Receive(&can_pal0_instance, RX_MAILBOX, &recvMsg_CAN0);  
        }
        IRQ_CAN0_RX = 0;
    }


}



void CAN_Tx_Test(void)
{
    uint32_t i = 0;
    CAN_Init(&can_pal0_instance, &can_pal0_Config0);

    can_buff_config_t T_buffCfg =  {
        .enableFD = false,
        .enableBRS = false,
        .fdPadding = 0U,
        .idType = CAN_MSG_ID_STD,
        .isRemote = false
    };
    print_usart0("CAN TX: ");
    if ((recvMsg_CAN0.id == RX_MSG_ID) && (recvMsg_CAN0.length > 0))
    {
        for (i = 0; i < recvMsg_CAN0.length; i++)
            print_usart0("0x%02X ", recvMsg_CAN0.data[i]);
        print_usart0("\n");
			
			CAN_ConfigTxBuff(&can_pal0_instance, TX_MAILBOX, &T_buffCfg);
			CAN_Send(&can_pal0_instance, TX_MAILBOX, &recvMsg_CAN0);
			while(CAN_GetTransferStatus(&can_pal0_instance, TX_MAILBOX) == STATUS_BUSY);
    }


}

void CAN_Rx_Test(void)
{
    uint32_t i = 0;

    CAN_Init(&can_pal0_instance, &can_pal0_Config0);

    can_buff_config_t R_buffCfg =  {
        .enableFD = false,
        .enableBRS = false,
        .fdPadding = 0U,
        .idType = CAN_MSG_ID_STD,
        .isRemote = false
    };
    print_usart0("CAN RX: ");

    CAN_ConfigRxBuff(&can_pal0_instance, RX_MAILBOX, &R_buffCfg, RX_MSG_ID);
    CAN_Receive(&can_pal0_instance, RX_MAILBOX, &recvMsg_CAN0);
    while (CAN_GetTransferStatus(&can_pal0_instance, RX_MAILBOX) == STATUS_BUSY);

    if ((recvMsg_CAN0.id == RX_MSG_ID) && (recvMsg_CAN0.length > 0))
    {
        for (i = 0; i < recvMsg_CAN0.length; i++)
            print_usart0("0x%02X ", recvMsg_CAN0.data[i]);
        print_usart0("\n");
    }
}

#endif
