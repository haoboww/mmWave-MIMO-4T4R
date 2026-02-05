#include "Usart_Protocol.h"
#include "main.h"

uint32_t frame_id = 0;
STRUCT_UDATA udata;

//static unsigned char HexToChar(unsigned char bChar)
//{
//	if((bChar>=0x30)&&(bChar<=0x39))
//	{
//		bChar -= 0x30;
//	}
//	else if((bChar>=0x41)&&(bChar<=0x46)) // Capital
//	{
//		bChar -= 0x37;
//	}
//	else if((bChar>=0x61)&&(bChar<=0x66)) //littlecase
//	{
//		bChar -= 0x57;
//	}
//	else
//	{
//		bChar = 0xff;
//	}
//	return bChar;
//}

char AsciiToDec(unsigned char * pAscii, unsigned char * pDec, int nLen)
{
	unsigned char Nibble[2] = {0};
	int i = 0;
	int j = 0;

	if (nLen % 2)
	{
		Nibble[0] = *pAscii ++;
		if (Nibble[0] >= '0' && Nibble[0] <= '9')
		{
			pDec[0] = Nibble[0] - '0';
		}
		else
			return 1;
		for (i = 0; i < (nLen - 1) / 2; i ++)
		{
			Nibble[0] = *pAscii ++;
			Nibble[1] = *pAscii ++;
			for (j = 0; j < 2; j ++)
			{
				if (Nibble[j] >= '0' && Nibble[j] <= '9')
					Nibble [j] = Nibble[j] - '0';
				else
					return 1;

			}
			pDec[i + 1] = Nibble[0] * 10;
			pDec[i + 1] += Nibble[1];
		}
	}
	else
	{
		for (i = 0; i < nLen / 2; i ++)
		{
			Nibble[0] = *pAscii ++;
			Nibble[1] = *pAscii ++;
			for (j = 0; j < 2; j ++)
			{
				if (Nibble[j] >= '0' && Nibble[j] <= '9')
					Nibble [j] = Nibble[j] - '0';
				else
					return 1;

			}
			pDec[i] = Nibble[0] * 10;
			pDec[i] += Nibble[1];
		}
	}
	return 0;
}

void Word2Byte(uint32_t w_val, uint8_t* b_val)
{
	b_val[0] = w_val & 0xFF;
	b_val[1] = (w_val >> 8) & 0xFF;
	b_val[2] = (w_val >> 16) & 0xFF;
	b_val[3] = (w_val >> 24) & 0xFF;
}
#if 1
static char Protocol_Analysis_Handle(void)
{
	uint8_t AtoH[10] = {0};
	uint8_t wtbuf[16] = {0};
	uint8_t systick_flag = 0;
	if(strUart.RecFinish==1)
	{
		if(GET_SYSTICK_STATE == 1)
		{
			systick_flag = 1;
			SYSTICK_DISABLE;
		}
		memset(&udata,0,sizeof(udata));
		strUart.RecLen--;     
		//RF
		if(memcmp(strUart.RecBuff,"AT",AT_LEN)==0&&(strUart.RecLen==AT_LEN)){
			printf("AT+OK\n");
		}
		else if(memcmp(strUart.RecBuff,"AT+START",AT_START_LEN)==0&&(strUart.RecLen==AT_START_LEN)){
			stFlashParam.WorkMode = 1;
			/*RTC*/
			bbTimCnt=0;
		}
		else if(memcmp(strUart.RecBuff,"AT+RESET",AT_RESET_LEN)==0&&(strUart.RecLen==AT_RESET_LEN)){
			CHPIEN_WritePin=0;
			WDT_RstNow();
			while(1);
		}
		else if(memcmp(strUart.RecBuff,"AT+IMPORT",AT_IMPORT_LEN)==0){
			printf("AT+OK\n");
			strUart.writeCnt = 0;
			stFlashParam.WorkMode = 2;
		}
		else if(memcmp(strUart.RecBuff, "AT+TIME=", 8) == 0 && (strUart.RecLen - 8) > 0 && (strUart.RecLen - 8) < 6)
		{
			if (AsciiToDec(strUart.RecBuff + 8, AtoH, strUart.RecLen - 8) == 0)
			{
				if ((strUart.RecLen - 8) == 1 || (strUart.RecLen - 8) == 2)  stFlashParam.frameTime = AtoH[0];
				if ((strUart.RecLen - 8) == 3 || (strUart.RecLen - 8) == 4)  stFlashParam.frameTime = AtoH[0] * 100 + AtoH[1];		//100-9999
				if ((strUart.RecLen - 8) == 5) 	stFlashParam.frameTime = AtoH[0] * 10000 + AtoH[1] * 100 + AtoH[2];	//10000-99999
				if (stFlashParam.frameTime <= AT_MAX_TIME)
				{
					Word2Byte(stFlashParam.frameTime, wtbuf);
					if(EFC_ReWrite(FLASH_STANDBY_TIME_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.frameTime = Read_M32(FLASH_STANDBY_TIME_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.frameTime);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+PROG=", 8) == 0 && (strUart.RecLen - 8) == 2)
		{
			if (AsciiToDec(strUart.RecBuff + 8, AtoH, strUart.RecLen - 8) == 0)
			{
				stFlashParam.projFunc = AtoH[0];
				if (stFlashParam.projFunc >= AT_MIN_PROG && stFlashParam.projFunc <= AT_MAX_PROG)
				{
					Word2Byte(stFlashParam.projFunc, wtbuf);
					if(EFC_ReWrite(FLASH_PROG_FUNC_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.projFunc = Read_M32(FLASH_PROG_FUNC_ADDR);
						printf("AT+OK=%02d\r\n", stFlashParam.projFunc);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+CFARTHV=", 11) == 0 && (strUart.RecLen - 11) > 0 && (strUart.RecLen - 11) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 11, AtoH, strUart.RecLen - 11) == 0)
			{
				stFlashParam.cfarThV = AtoH[0];
				if(stFlashParam.cfarThV < AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.cfarThV, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHV_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.cfarThV = Read_M32(FLASH_CFARTHV_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.cfarThV);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+CFARTHVG=", 12) == 0 && (strUart.RecLen - 12) > 0 && (strUart.RecLen - 12) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 12, AtoH, strUart.RecLen - 12) == 0)
			{
				stFlashParam.cfarThVG = AtoH[0];
				if (stFlashParam.cfarThVG < AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.cfarThVG, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHVG_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.cfarThVG = Read_M32(FLASH_CFARTHVG_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.cfarThVG);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+CFARTHVS=", 12) == 0 && (strUart.RecLen - 12) > 0 && (strUart.RecLen - 12) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 12, AtoH, strUart.RecLen - 12) == 0)
			{
				stFlashParam.cfarThVS = AtoH[0];
				if (stFlashParam.cfarThVS < AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.cfarThVS, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHVS_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.cfarThVS = Read_M32(FLASH_CFARTHVS_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.cfarThVS);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+CFARVMODE=", 13) == 0 && (strUart.RecLen - 13) > 0 && (strUart.RecLen - 13) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 13, AtoH, strUart.RecLen - 13) == 0)
			{
				stFlashParam.cfarVmode = AtoH[0];
				if(stFlashParam.cfarVmode < AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.cfarVmode, wtbuf);
					if(EFC_ReWrite(FLASH_CFARVMODE_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.cfarVmode = Read_M32(FLASH_CFARVMODE_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.cfarVmode);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}	
		else if (memcmp(strUart.RecBuff, "AT+PEAKENV=", 11) == 0 && (strUart.RecLen - 11) == 2)
		{
			if (AsciiToDec(strUart.RecBuff + 11, AtoH, strUart.RecLen - 11) == 0)
			{
				stFlashParam.peakenV = AtoH[0];
				if (stFlashParam.peakenV >= 0 && stFlashParam.peakenV <= 1)
				{
					Word2Byte(stFlashParam.peakenV, wtbuf);
					if(EFC_ReWrite(FLASH_PEAKENV_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.peakenV = Read_M32(FLASH_PEAKENV_ADDR);
						printf("AT+OK=%02d\r\n", stFlashParam.peakenV);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+DPKTH=", 9) == 0 && (strUart.RecLen - 9) > 0 && (strUart.RecLen - 9) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 9, AtoH, strUart.RecLen - 9) == 0)
			{
				stFlashParam.dpkThres = AtoH[0];
				if (stFlashParam.dpkThres <= AT_MAX_DPK)
				{
					Word2Byte(stFlashParam.dpkThres, wtbuf);
					if(EFC_ReWrite(FLASH_DPKTH_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.dpkThres = Read_M32(FLASH_DPKTH_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.dpkThres);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+DPKTIMES=", 12) == 0 && (strUart.RecLen - 12) > 0 && (strUart.RecLen - 12) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 12, AtoH, strUart.RecLen - 12) == 0)
			{
				stFlashParam.dpkTimes = AtoH[0];
				if (stFlashParam.dpkTimes <= AT_MAX_DPKTIMES)
				{
					Word2Byte(stFlashParam.dpkTimes, wtbuf);
					if(EFC_ReWrite(FLASH_DPKTIMES_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.dpkThres = Read_M32(FLASH_DPKTIMES_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.dpkTimes);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
//		else if (memcmp(strUart.RecBuff, "AT+NUMTLV=", 10) == 0 && (strUart.RecLen - 10) > 0 && (strUart.RecLen - 10) < 3)
//		{
//			if (AsciiToDec(strUart.RecBuff + 10, AtoH, strUart.RecLen - 10) == 0)
//			{
//				r2_str.numTLV = AtoH[0];
//				if (r2_str.numTLV >= ProtocolNumTlvsMin && r2_str.numTLV <= ProtocolNumTlvsMax)
//				{
//					Word2Byte(r2_str.numTLV, wtbuf);
//					if(EFC_ReWrite(FLASH_NUMTLV_ADDR, 4, wtbuf) == OK)
//					{
//						r2_str.numTLV = Read_M32(FLASH_NUMTLV_ADDR);
//						printf("AT+OK=%d\r\n", r2_str.numTLV);
//					}
//					else
//						printf("Save Para Fail\r\n");
//				}
//			}
//		}
		else if (memcmp(strUart.RecBuff, "AT+MEAN=", 8) == 0 && (strUart.RecLen - 8) == 2)
		{
			if (AsciiToDec(strUart.RecBuff + 8, AtoH, strUart.RecLen - 8) == 0)
			{
				stFlashParam.meanEn = AtoH[0];
				if (stFlashParam.meanEn >= 0 && stFlashParam.meanEn <= 2)
				{
					Word2Byte(stFlashParam.meanEn, wtbuf);
					if(EFC_ReWrite(FLASH_MEAN_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.meanEn = Read_M32(FLASH_MEAN_ADDR);
						printf("AT+OK=%02d\r\n", stFlashParam.meanEn);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}	
		else if (memcmp(strUart.RecBuff, "AT+ZOTH=", 8) == 0 && (strUart.RecLen - 8) > 0 && (strUart.RecLen - 8) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 8, AtoH, strUart.RecLen - 8) == 0)
			{
				stFlashParam.zoTh = AtoH[0];
				if (stFlashParam.zoTh < AT_MAX_ZO)
				{
					Word2Byte(stFlashParam.zoTh, wtbuf);
					if(EFC_ReWrite(FLASH_ZOTH_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.zoTh = Read_M32(FLASH_ZOTH_ADDR);
						printf("AT+OK=%02d\r\n", stFlashParam.zoTh);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+GAP=", 7) == 0 && (strUart.RecLen - 7) > 0 && (strUart.RecLen - 7) < 6)
		{
			if (AsciiToDec(strUart.RecBuff + 7, AtoH, strUart.RecLen - 7) == 0)
			{
				if ((strUart.RecLen - 7) == 1 || (strUart.RecLen - 7) == 2)  stFlashParam.chirpGap = AtoH[0];
				if ((strUart.RecLen - 7) == 3 || (strUart.RecLen - 7) == 4)  stFlashParam.chirpGap = AtoH[0] * 100 + AtoH[1];		//100-9999
				if ((strUart.RecLen - 7) == 5) 	stFlashParam.chirpGap = AtoH[0] * 10000 + AtoH[1] * 100 + AtoH[2];	//10000-99999
				if ((stFlashParam.chirpGap >= AT_GAP_MIN) && (stFlashParam.chirpGap <= AT_GAP_MAX))
				{
					Word2Byte(stFlashParam.chirpGap, wtbuf);
					if (EFC_ReWrite(FLASH_CHIRPGAP_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.chirpGap = Read_M32(FLASH_CHIRPGAP_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.chirpGap);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+BW=", 6) == 0 && (strUart.RecLen - 6) > 0 && (strUart.RecLen - 6) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 6, AtoH, strUart.RecLen - 6) == 0)
			{
				stFlashParam.bandWidth = AtoH[0];
				if (stFlashParam.bandWidth >= 0 && stFlashParam.bandWidth <= 10)
				{
					Word2Byte(stFlashParam.bandWidth, wtbuf);
					if (EFC_ReWrite(FLASH_BW_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.bandWidth = Read_M32(FLASH_BW_ADDR);
						printf("AT+OK=%02d\r\n", stFlashParam.bandWidth);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+CFARTHS=", 11) == 0 && (strUart.RecLen - 11) > 0 && (strUart.RecLen - 11) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 11, AtoH, strUart.RecLen - 11) == 0)
			{
				stFlashParam.cfarThS = AtoH[0];
				if(stFlashParam.cfarThS < AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.cfarThS, wtbuf);
					if(EFC_ReWrite(FLASH_CFARTHS_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.cfarThS = Read_M32(FLASH_CFARTHS_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.cfarThS);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}
		else if (memcmp(strUart.RecBuff, "AT+FGAP=", 8) == 0 && (strUart.RecLen - 8) > 0 && (strUart.RecLen - 8) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 8, AtoH, strUart.RecLen - 8) == 0)
			{
				stFlashParam.frameGap = AtoH[0];
				if (stFlashParam.frameGap <= AT_MAX_CFAR)
				{
					Word2Byte(stFlashParam.frameGap, wtbuf);
					if (EFC_ReWrite(FLASH_FRAMEGAP_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.frameGap = Read_M32(FLASH_FRAMEGAP_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.frameGap);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}		
		else if (memcmp(strUart.RecBuff, "AT+NUMSUM=", 10) == 0 && (strUart.RecLen - 10) > 0 && (strUart.RecLen - 10) < 3)
		{
			if (AsciiToDec(strUart.RecBuff + 10, AtoH, strUart.RecLen - 10) == 0)
			{
				stFlashParam.numSum = AtoH[0];
				if ((stFlashParam.numSum == 1) || (stFlashParam.numSum == 8) || (stFlashParam.numSum == 16))
				{
					Word2Byte(stFlashParam.numSum, wtbuf);
					if (EFC_ReWrite(FLASH_NUMSUM_ADDR, 4, wtbuf) == OK)
					{
						stFlashParam.numSum = Read_M32(FLASH_NUMSUM_ADDR);
						printf("AT+OK=%d\r\n", stFlashParam.numSum);
					}
					else
						printf("Save Para Fail\r\n");
				}
			}
		}		
		else{ //false
			UART_Transmit_Bytes(&UART0,strUart.RecBuff,strUart.RecLen);
			printf("\nAT+ERR\n");
		}
		strUart.RecLen = 0;
		strUart.RecFinish = 0;
		if(systick_flag == 1)
		{
			SYSTICK_ENABLE;
		}
	}
	return 0;
}
#endif

void RecProtocol_Loop(void)
{
	vTaskDelay(pdMS_TO_TICKS(50));
	if(strUart.timeoutStart)
	{
		vTaskDelay(pdMS_TO_TICKS(50));
		strUart.RecFinish=1;
		Protocol_Analysis_Handle();
		strUart.timeoutStart=0;
	}
}




/*UsartD-¨°¨¦:
Head:(2B)
		55AA
Length:(4B)
		²»°üº¬HeadºÍLength
Time:(2B)
    ms
numTLVs:(1B)
		01£ºpoint
		02£ºtrack
		03£ºpoint+track
type:(1B)
		01:pointD
		02:pointS0_1
		03:pointS0_2
		04:pointS0_3
		05:pointS0_4
		06:pointS0_5
		07:pointS0_6
TargetNum:(2B)
TLV:(8B/16B)
	type=01:d1Idx(2B)+d2Idx(1B)+d3Idx(1B)+d4Idx(1B)+pow3Abs(1B)
	type=02/03/04/05/06/07:d1Idx(2B)+d2Idx(1B)+d3Idx(1B)+d4Idx(1B)+pow3Abs(1B)
*/
#if 1
uint32_t ProtocolSendPack(uint32_t targetNum,STRUCT_FRAMEPOINT *sPtRst,uint8_t *sBuff,uint16_t sTime){
	STRUCT_FRAMEPOINT *frameRstS;
	uint32_t sCnt = 0;
	uint32_t sLen = 0;
	bb_RunTime = 0;
	//Head(2B)
	sBuff[0]=0x55;
	sBuff[1]=0xAA;
	sCnt=sCnt+2;
	//Length(4B)
	sLen = targetNum*6 + 6;  //6=numTLV(1B)+type(1B)+targetnum(2B)+timeInterval(2B)  6=d1Idx(2B)+d2Idx(1B)+d3Idx(1B)+d4Idx(1B)+powABS(1B)
	sBuff[sCnt]=sLen&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>8)&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>16)&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>24)&0xFF;
	sCnt++;
  //Time(2B)
	sBuff[sCnt]= sTime&0xFF;
	sBuff[sCnt+1]= (sTime>>8)&0xFF;
	sCnt=sCnt+2; //frame interval time
	//numTLVs(1B)
	sBuff[sCnt] = 1;
	sCnt=sCnt+1;
	//type(1B)
	sBuff[sCnt] = 1; //type=1
	sCnt=sCnt+1;
  //TargetNum(2B)
	sBuff[sCnt]=(targetNum)&0xFF;
	sCnt++;
	sBuff[sCnt]=(targetNum>>8)&0xFF;
	sCnt++;
	//TLV
	for (uint32_t i = 0; i < targetNum; i++) {
		sBuff[sCnt] = (sPtRst[i].d1Idx) & 0xFF;
		sCnt++;
		sBuff[sCnt] = (sPtRst[i].d1Idx >> 8) & 0xFF;
		sCnt++;
		sBuff[sCnt] = sPtRst[i].d2Idx;
		sCnt++;
		sBuff[sCnt] = sPtRst[i].d3Idx;
		sCnt++;
		sBuff[sCnt] = sPtRst[i].d4Idx;
		sCnt++;
		sBuff[sCnt] = (sPtRst[i].pow3dAbs >> 12) & 0xFF;
		sCnt++;
	}
	for(uint8_t n = 0; n < MAX_FRAME_NUM-1; n++)
	{
		sLen = sLen + algCfg.targetNumS[n]*6 + 3;
		frameRstS = (STRUCT_FRAMEPOINT *)(algCfg.mem.frameRstS0_1+algCfg.cfarMaxNum*algCfg.dpkTimes*sizeof(STRUCT_FRAMEPOINT)*n);		
		//type(1B)
		sBuff[sCnt] = n+2; //type=1
		sCnt=sCnt+1;
		//TargetNum(2B)
		sBuff[sCnt]=(algCfg.targetNumS[n])&0xFF;
		sCnt++;
		sBuff[sCnt]=(algCfg.targetNumS[n]>>8)&0xFF;
		sCnt++;
		//TLV
		for (uint32_t i = 0; i < algCfg.targetNumS[n]; i++) {
			sBuff[sCnt] = (frameRstS[i].d1Idx) & 0xFF;
			sCnt++;
			sBuff[sCnt] = (frameRstS[i].d1Idx >> 8) & 0xFF;
			sCnt++;
			sBuff[sCnt] = frameRstS[i].d2Idx+32;
			sCnt++;
			sBuff[sCnt] = frameRstS[i].d3Idx;
			sCnt++;
			sBuff[sCnt] = frameRstS[i].d4Idx;
			sCnt++;
			sBuff[sCnt] = (frameRstS[i].pow3dAbs >> 12) & 0xFF;
			sCnt++;
		}		
	}
	//updata sLen
	sCnt=2;
	sBuff[sCnt]=sLen&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>8)&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>16)&0xFF;
	sCnt++;
	sBuff[sCnt]=(sLen>>24)&0xFF;
	sCnt++;	
	sLen = sLen+6; //6=Head(2B)+Len(4B)	
	return sLen;
}

#endif
void ImportData_Loop(void){
	if(    strUart.writeCnt>=algCfg.numSamp_Chirp_ANT_Byte){
			strUart.writeCnt = 0;
			stFlashParam.WorkMode = 0;
			printf("AT+OK\n");
//                USART_Transmit_Bytes(&USART0, (uint8_t*)IMPORT_ADDR,IMPORT_MAX_LEN);
	}
}
