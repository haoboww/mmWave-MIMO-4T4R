#include "BB_AlgProc.h"
#include "main.h"



//uint16_t frameTime = 0;

//const float steeringVec[NUM_ANGLE*algCfg.numRX]={
uint32_t head[2] = {0x04030201, 0x08070605};
//};
volatile uint8_t BB_AlgProcFinished = 0;
volatile uint32_t frameCnt = 0;
volatile uint8_t meanBufIdx = 0;

/*alg func*/
uint8_t fft1dCalc(void);
// sampData
uint8_t  sampDataHandle(void) {
	uint8_t rtn;
	BaseType_t xResult;
	rampCnt = 0;
	BB_isrRead = 0;
	BB_isrSet = 0;
	BB_sampOverFlag = 0;
	BB_firstChirp = 0;
	BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
	                     ((uint64_t)(algCfg.numSampByte * algCfg.numRX)  << BB_BIT32_POS); // jumpInc
	BB_IMR_CLEAR_ALL;
	BB_ISR_CLEAR_ALL;
	taskENTER_CRITICAL();           //进入临界区
	RF_Reg_Set();
	taskEXIT_CRITICAL();            //退出临界区
	BB_OPCLEAR(BB_CLEAR_PREP);
	BB_OPGATE_EN(BB_GATE_PREP);
	BB_ISR_CLEAR_ALL;
	BB_IMR_SET(BB_IMR_PREP_RAMP_END | BB_IMR_RAMP_PEDGE);
	ulTaskNotifyTake(pdTRUE, 0);
	waveStartEn = 1;
//	taskENTER_CRITICAL();           //进入临界区
//	RF_waveStart();
//	RF_WriteRegSPI(CS1_PIN, ANA_CFG22_ADDR, 0x9000);
//	taskEXIT_CRITICAL();            //退出临界区

	if (stFlashParam.projFunc == 0x02) {
		rtn = fft1dCalc();
	} else {
		BB_isrSet = BB_ISR_PREP_FRAME_END;
		BB_IMR_SET(BB_IMR_PREP_FRAME_END);
		xResult = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
		if (xResult == pdPASS && (BB_isrRead & BB_isrSet)) {
//			printf("BB_SUC:FRAME_END=%08X,%08X\n",BB_isrSet,bb_regb_str->ISR&BB_isrSet);
			rtn = TRUE;
		} else {
			// 处理错误或超时
			printf("BB_ERR:FRAME_END=%08X,%08X\n", BB_isrSet, BB_isrRead);
			rtn = FALSE;
		}
		RF_Reg_Reset();
	}
	BB_OPGATE_DIS(BB_GATE_PREP);
	BB_OPCLEAR(BB_CLEAR_PREP);
	BB_IMR_CLEAR_ALL;
	BB_ISR_CLEAR_ALL;
	ulTaskNotifyTake(pdTRUE, 0);
	return rtn;
}

// fft1d calc
uint8_t fft1dCalc(void) {
	uint8_t rtn;
	BaseType_t xResult;

	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, algCfg.numSamp - 1, DMA_CFG0_WMODE_ORI, algCfg.numByte - 1); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
	BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(0, BB_ZO_BASE); //dstIntraInc,dstBaseAddr

	BB_P1MUL_Cfg1(0, BB_FFT_BASE); // dstIntraInc,dstBaseAddr
	BB_FFT_Cfg0(0, 0, algCfg.fft1d_useA, algCfg.fft1d_useB, algCfg.numSamp - 1, algCfg.fft1dPT, FFT_UNLOAD_EN, FFT_MODE_FORWARD); //rsfOutput,rsfInput,useA,useB,inPt,fftPt,unloadEn,mode
	BB_FFT_ZP_Clear();
	BB_FFT_Cfg1(algCfg.mem.winRange, FFT_CFG1_OUTLSFEN_RIGHT, 0, FFT_CFG1_INLSFEN_RIGHT, 0, FFT_CFG1_WIN_SIZE_14BIT, FFT_CFG1_WIN_EN); //winBaseAddr,winSize,winEn
	BB_OPCLEAR(BB_CLEAR_DMA | BB_CLEAR_FFT);

	BB_OPGATE_EN(BB_GATE_P1MUL | BB_GATE_DMA | BB_GATE_FFT);
	for (uint32_t i = 0, chirpDiv = algCfg.numChirpDiv4; i < 4; i++) {
		BB_isrSet = BB_ISR_PREP_QUARTER_END;
		BB_ISR_CLEAR(BB_ISR_PREP_QUARTER_END);
		BB_IMR_SET(BB_IMR_PREP_QUARTER_END);
		if (BB_sampOverFlag) {
			rtn = TRUE;
		} else {
			xResult = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
			// TEST_WritePin ^= 1;
			if (xResult == pdPASS) {
				rtn = TRUE;
//				printf("BB_SUC:QUARTER%d_END=%08X\n",i+1,BB_isrSet);
			} else {
				// 处理错误或超时
				printf("BB_ERR:QUARTER%d_END=%08X\n", i + 1, BB_isrSet);
				return FALSE;
			}
		}
		BB_IMR_CLEAR(BB_IMR_PREP_QUARTER_END);
#if 1
		for (uint32_t antNow = 0; antNow < algCfg.numANT; antNow++) {
			BB_P1MUL_Cfg0(P1MUL_CFG0_NCOMODE_COS_SIN, nco_fcw[antNow], calib_ap[antNow]);
			for (uint32_t chirpNow = 0; chirpNow < chirpDiv; chirpNow++) {
				bb_regb_str->DMA_CFG2   = (uint64_t)algCfg.numByte << BB_BIT32_POS | (uint64_t)(algCfg.mem.adcSamp + antNow * algCfg.numSampByte + (chirpNow + i * chirpDiv) * algCfg.numSamp_ANT_Byte);
				bb_regb_str->FFT_CFG2   = (uint64_t)NUM_BYTE32 << BB_BIT32_POS | (uint64_t)(algCfg.mem.fft1d + antNow * algCfg.useRange32 + (chirpNow + i * chirpDiv) * algCfg.numSamp_ANT_Byte);
				BB_OPCLEAR(BB_CLEAR_P1MUL);
				BB_ZO_Cfg0(BB_ZO_ZLEN_6, stFlashParam.zoTh, BB_ZO_MODE_2, 0xFFFF); //zLen,mulFac,mode,thresSum
				BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
				BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
				BB_OPTRIG(BB_TRIG_DMA);
				BB_waitISR(BB_ISR_FFT_UNLOAD_END);
			}
		}
		BB_IMR_CLEAR(BB_IMR_FFT4_UNLOAD_END);
#endif
	}
	BB_isrSet = BB_ISR_PREP_FRAME_END;
	BB_IMR_SET(BB_IMR_PREP_FRAME_END);
	if (BB_sampOverFlag) {
		rtn = TRUE;
	} else {
		xResult = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(1000));
		if (xResult == pdPASS) {
//			printf("BB_SUC:QUARTER4_END=%08X\n",BB_isrSet);
			rtn = TRUE;
		} else {
			// 处理错误或超时
			printf("BB_ERR:FRAME_END=%08X\n", BB_isrSet);
			return FALSE;
		}
	}
	RF_Reg_Reset();
#if 0
//	UART_Transmit_Bytes(&UART0, (uint8_t*)&head[0], 8);
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1d, algCfg.useRange32 * algCfg.numANT * algCfg.numChirp);
	while (1);
#endif
	return rtn;
}


void fft1dWinVelMeanCalc(uint32_t winAddr, uint32_t srcAddr, uint32_t dstAddr)
{
	BB_OPCLEAR(BB_CLEAR_P2);
	BB_OPGATE_EN(BB_GATE_P2);
	BB_P2_CFG0(algCfg.numRX - 1, algCfg.useRange - 1, algCfg.numChirp - 1, P2_CFG0_DIV1, P2_CFG0_SUBMODE_CPX_SUM_MUL, P2_CFG0_MODE_MAC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	BB_P2_CFG1(algCfg.useRange*NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG2(algCfg.numSamp_ANT_Byte, srcAddr); //src0IntraInc,src0BaseAddr
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG4(NUM_BYTE32, winAddr); //src1IntraInc,src1BaseAddr
	BB_P2_CFG5(algCfg.useRange*NUM_BYTE32, NUM_BYTE32); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, dstAddr); //dstIntraInc,dstBaseAddr
	BB_ISR_CLEAR(BB_ISR_P2_JUMP_END);
	BB_OPTRIG(BB_TRIG_P2);
	BB_waitISR(BB_ISR_P2_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_P2);			
}
//fft1d mean ,minus   return maxVal
void fft1dMeanCalc(uint8_t meanEn) {
	FFT1D_MEAN_BUF *pFFT1DMeanBuf = (FFT1D_MEAN_BUF *)algCfg.mem.fft1dWinMean_1;
	uint32_t *dstAddr;
	switch (meanEn) {
	case 0: //mean disable
		break;
	case 1: //mean en
		BB_OPCLEAR(BB_CLEAR_P2);
		BB_OPGATE_EN(BB_GATE_P2);
		BB_P2_CFG0(algCfg.numANT - 1, algCfg.useRange - 1, algCfg.numChirp - 1, P2_CFG0_DIV64, P2_CFG0_SUBMODE_CPX_SUM_MUL, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
		BB_P2_CFG1(algCfg.useRange * NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
		BB_P2_CFG2(algCfg.numSamp_ANT_Byte, algCfg.mem.fft1d); //src0IntraInc,src0BaseAddr
		BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
		BB_P2_CFG4(0, 0); //src1IntraInc,src1BaseAddr
		BB_P2_CFG5(algCfg.useRange * NUM_BYTE32, NUM_BYTE32); //dstJumpInc,dstInterInc
		BB_P2_CFG6(0, algCfg.mem.fft1dMean); //dstIntraInc,dstBaseAddr
		BB_ISR_CLEAR(BB_ISR_P2_JUMP_END);
		BB_OPTRIG(BB_TRIG_P2);
		BB_waitISR(BB_ISR_P2_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_P2);		
#if 0 //fft1d mean
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1dMean, algCfg.numSamp_ANT_Byte);
		while (1);
#endif
		if(frameCnt%algCfg.frameGap==0)
		{
			dstAddr = &pFFT1DMeanBuf->fft1dWinVel[meanBufIdx].mean[0];
			meanBufIdx = meanBufIdx == MAX_FRAME_NUM-1 ? 0 : meanBufIdx+1;	
			for (uint8_t txNum = 0; txNum < algCfg.numTX; txNum++)
			{
				fft1dWinVelMeanCalc(algCfg.mem.winVel1 + txNum * algCfg.numChirp*NUM_BYTE32, algCfg.mem.fft1d+txNum*algCfg.useRange32*algCfg.numRX, (uint32_t)dstAddr+txNum*algCfg.useRange32*algCfg.numRX);
			}
		}
#if 0 //fft1d winVel mean
		UART_Transmit_Bytes(&UART0, (uint8_t *)dstAddr, algCfg.numSamp_ANT_Byte);
		while (1);
#endif		
		break;		
	case 2: //mean en
		BB_OPCLEAR(BB_CLEAR_P2);
		BB_OPGATE_EN(BB_GATE_P2);
		BB_P2_CFG0(algCfg.numANT - 1, algCfg.useRange - 1, algCfg.numChirp - 1, P2_CFG0_DIV64, P2_CFG0_SUBMODE_CPX_SUM_MUL, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
		BB_P2_CFG1(algCfg.useRange * NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
		BB_P2_CFG2(algCfg.numSamp_ANT_Byte, algCfg.mem.fft1d); //src0IntraInc,src0BaseAddr
		BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
		BB_P2_CFG4(0, 0); //src1IntraInc,src1BaseAddr
		BB_P2_CFG5(algCfg.useRange * NUM_BYTE32, NUM_BYTE32); //dstJumpInc,dstInterInc
		BB_P2_CFG6(0, algCfg.mem.fft1dMeanTmp); //dstIntraInc,dstBaseAddr
		BB_ISR_CLEAR(BB_ISR_P2_JUMP_END);
		BB_OPTRIG(BB_TRIG_P2);
		BB_waitISR(BB_ISR_P2_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_P2);			
		DMA_Start_TransferM2M_32((uint32_t *)algCfg.mem.fft1dMeanLast, (uint32_t *)algCfg.mem.fft1dMean, 16*1024,DMA_CHANNEL0);
		DMA_Start_TransferM2M_32((uint32_t *)algCfg.mem.fft1dMeanTmp, (uint32_t *)algCfg.mem.fft1dMeanLast, 16*1024,DMA_CHANNEL0);	
	#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumAbs_1, algCfg.useRange * algCfg.numChirpDiv2 * NUM_BYTE64);
		while (1);
	#endif	
	
#if 0 //fft1d mean
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1dMean, algCfg.numSamp_ANT_Byte);
		while (1);
#endif
		if(frameCnt%algCfg.frameGap==0)
		{
			dstAddr = &pFFT1DMeanBuf->fft1dWinVel[meanBufIdx].mean[0];
			meanBufIdx = meanBufIdx == MAX_FRAME_NUM-1 ? 0 : meanBufIdx+1;	
			for (uint8_t txNum = 0; txNum < algCfg.numTX; txNum++)
			{
				fft1dWinVelMeanCalc(algCfg.mem.winVel1 + txNum * algCfg.numChirp*NUM_BYTE32, algCfg.mem.fft1d+txNum*algCfg.useRange32*algCfg.numRX, (uint32_t)dstAddr+txNum*algCfg.useRange32*algCfg.numRX);
			}
		}
#if 0 //fft1d winVel mean
		UART_Transmit_Bytes(&UART0, (uint8_t *)dstAddr, algCfg.numSamp_ANT_Byte);
		while (1);
#endif		
		break;		
	}
}

//fft2d
void fft2dCalc(void) {
	uint64_t sP2_Cfg2 = 0;
	uint64_t sFFT_Cfg2 = 0;
	uint64_t sP2_Cfg4 = 0;
	BUF_CFG0(BUF_CFG0_WR_FLT2PF_LSF_EN_RIGHT, 0, BUF_CFG0_CONVERT_CPX_MODE_FIX, BUF_CFG0_RD_POWMODE_ABS);
	BB_P2_CFG0(0, 0, algCfg.numChirp - 1, P2_CFG0_DIV1, P2_CFG0_SUBMODE_CPX_MINUS, P2_CFG0_MODE_ADD); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	BB_P2_CFG1(0, 0); //src0JumpInc,src0InterInc
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG5(0, 0); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, BB_FFT_BASE); //dstIntraInc,dstBaseAddr
	BB_FFT_Cfg0(0, 0, algCfg.fft2d_useA, algCfg.fft2d_useB, algCfg.numChirp - 1, algCfg.fft2dPT, FFT_UNLOAD_EN, FFT_MODE_FORWARD); //rsfOutput,rsfInput,useA,useB,inPt,fftPt,unloadEn,mode   //useA>useB
	BB_FFT_ZP_Clear();
	for (uint8_t txNum = 0; txNum < algCfg.numTX; txNum++)
	{
		BB_FFT_Cfg1(algCfg.mem.winVel1 + algCfg.numChirp * NUM_BYTE32 * txNum, FFT_CFG1_OUTLSFEN_RIGHT, 0, FFT_CFG1_INLSFEN_RIGHT, 0, FFT_CFG1_WIN_SIZE_32BIT, FFT_CFG1_WIN_EN);
		bb_regb_str->FFT_SRC_ZP00 = fft2d_zp.txNum_zp0[txNum];
		bb_regb_str->FFT_SRC_ZP01 = fft2d_zp.txNum_zp1[txNum];
		bb_regb_str->FFT_SRC_ZP02 = fft2d_zp.txNum_zp2[txNum];
		bb_regb_str->FFT_SRC_ZP03 = fft2d_zp.txNum_zp3[txNum];
		for (uint8_t rxNum = 0; rxNum < algCfg.numRX; rxNum++)
		{
			BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
			BB_OPCLEAR(BB_CLEAR_FFT | BB_CLEAR_P2);
			BB_OPGATE_EN(BB_GATE_P2 | BB_GATE_FFT);
			sP2_Cfg2 = (uint64_t)algCfg.numSamp_ANT_Byte << BB_BIT32_POS | (algCfg.mem.fft1d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32); //src0IntraInc,src0BaseAddr
			sFFT_Cfg2 = (uint64_t)algCfg.numSamp_ANT_Byte << BB_BIT32_POS | (algCfg.mem.fft2d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32);
			if (algCfg.meanEn == 0) {
				Write_M32(algCfg.mem.fft1dMean, 0);
				sP2_Cfg4 = (uint64_t)(algCfg.mem.fft1dMean); //src1IntraInc,src1BaseAddr
				for (uint32_t rangeNow = 1; rangeNow <= algCfg.useRange; rangeNow++) {
					bb_regb_str->P2_CFG2 = sP2_Cfg2;
					bb_regb_str->P2_CFG4 = sP2_Cfg4;
					bb_regb_str->FFT_CFG2 = sFFT_Cfg2;

					BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
					BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
					BB_OPTRIG(BB_TRIG_P2);
					sP2_Cfg2 = ((uint64_t)algCfg.numSamp_ANT_Byte) << BB_BIT32_POS | (algCfg.mem.fft1d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32 + rangeNow * NUM_BYTE32); //src0IntraInc,src0BaseAddr
					sFFT_Cfg2 = ((uint64_t)algCfg.numSamp_ANT_Byte) << BB_BIT32_POS | (algCfg.mem.fft2d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32 + rangeNow * NUM_BYTE32); //dstIntraInc,dstBaseAddr
					BB_waitISR(BB_ISR_FFT_UNLOAD_END);
				}
			} else {
				sP2_Cfg4 = (uint64_t)(algCfg.mem.fft1dMean + algCfg.useRange * NUM_BYTE32 * (txNum * algCfg.numRX + rxNum)); //src1IntraInc,src1BaseAddr
				for (uint32_t rangeNow = 1; rangeNow <= algCfg.useRange; rangeNow++) {
					bb_regb_str->P2_CFG2 = sP2_Cfg2;
					bb_regb_str->P2_CFG4 = sP2_Cfg4;
					bb_regb_str->FFT_CFG2 = sFFT_Cfg2;

					BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
					BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
					BB_OPTRIG(BB_TRIG_P2);
					sP2_Cfg2 = ((uint64_t)algCfg.numSamp_ANT_Byte) << BB_BIT32_POS | (algCfg.mem.fft1d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32 + rangeNow * NUM_BYTE32); //src0IntraInc,src0BaseAddr
					sP2_Cfg4 = (uint64_t)algCfg.mem.fft1dMean + algCfg.useRange * NUM_BYTE32 * (txNum * algCfg.numRX + rxNum) + rangeNow * NUM_BYTE32; //src1IntraInc,src1BaseAddr
					sFFT_Cfg2 = ((uint64_t)algCfg.numSamp_ANT_Byte) << BB_BIT32_POS | (algCfg.mem.fft2d + algCfg.useRange * (txNum * algCfg.numRX + rxNum) * NUM_BYTE32 + rangeNow * NUM_BYTE32); //dstIntraInc,dstBaseAddr
					BB_waitISR(BB_ISR_FFT_UNLOAD_END);
				}
			}
		}
	}
	BB_OPGATE_DIS(BB_GATE_P2 | BB_GATE_FFT);
#if 0 //fft2d
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2d, algCfg.useRange32 * algCfg.numANT * algCfg.numChirp);
	while (1);
#endif
}

void meanMinus(uint32_t src0Addr, uint32_t src1Addr, uint32_t dstAddr)
{
	BB_OPCLEAR(BB_CLEAR_P2);
	BB_OPGATE_EN(BB_GATE_P2);
	BB_P2_CFG0(0, algCfg.numANT - 1, algCfg.useRange - 1, P2_CFG0_DIV1, P2_CFG0_SUBMODE_CPX_MINUS, P2_CFG0_MODE_ADD); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	BB_P2_CFG1(0, algCfg.useRange*NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG2(NUM_BYTE32, src0Addr); //src0IntraInc,src0BaseAddr
	BB_P2_CFG3(0, algCfg.useRange*NUM_BYTE32); //src1JumpInc,src1InterInc
	BB_P2_CFG4(NUM_BYTE32, src1Addr); //src1IntraInc,src1BaseAddr
	BB_P2_CFG5(0, algCfg.useRange*NUM_BYTE32); //dstJumpInc,dstInterInc
	BB_P2_CFG6(NUM_BYTE32, dstAddr); //dstIntraInc,dstBaseAddr
	BB_ISR_CLEAR(BB_ISR_P2_JUMP_END);
	BB_OPTRIG(BB_TRIG_P2);
	BB_waitISR(BB_ISR_P2_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_P2);
}

void fft1dMeanMinus(void)
{
	uint8_t subtrahend, minuend;
	uint32_t *src0Addr, *src1Addr, *dstAddr;	
	FFT1D_MEANSUB_BUF *pFFT1DMeanSubBuf = (FFT1D_MEANSUB_BUF *)algCfg.mem.fft1dMean0_1;
	FFT1D_MEAN_BUF *pFFT1DMeanBuf = (FFT1D_MEAN_BUF *)algCfg.mem.fft1dWinMean_1;	
	subtrahend = meanBufIdx-1 < 0 ? (meanBufIdx-1+MAX_FRAME_NUM) : (meanBufIdx-1);
	src0Addr = &pFFT1DMeanBuf->fft1dWinVel[subtrahend].mean[0];
	//1. move src0 to ABUF2
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, algCfg.numANT - 1, algCfg.useRange - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, algCfg.useRange*NUM_BYTE32); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(NUM_BYTE32, (uint32_t)src0Addr); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(0, algCfg.useRange*NUM_BYTE32); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.fft1dMean_0); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);		
#if 0 //fft1d winVel mean
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1dMean_0, algCfg.numSamp_ANT_Byte);
		while (1);
#endif			
	//2. frame0-1, 0-2, ...
//	printf("meanBufIdx=%d, ", meanBufIdx);
//	printf("subtrahend=%d, ", subtrahend);
	for(uint8_t n = 1; n < 7; n++)
	{
		minuend = (subtrahend-n) < 0 ? (subtrahend-n+MAX_FRAME_NUM) : (subtrahend-n);
//		printf("minuend=%d, ", minuend);
		src1Addr = &pFFT1DMeanBuf->fft1dWinVel[minuend].mean[0];
		dstAddr  = &pFFT1DMeanSubBuf->fft1dWinVel[n-1].mean[0];
		meanMinus(algCfg.mem.fft1dMean_0, (uint32_t)src1Addr, (uint32_t)dstAddr);
	}
//	printf("\n");
#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1dMean0_1, algCfg.numSamp_ANT_Byte*6);
		while (1);	
#endif	
		//3. frame0-1, 0-2, ... sum abs
	//imag=0
	Write_M32(algCfg.mem.zeros, 0);	
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, MAX_FRAME_NUM - 2, algCfg.numANT - 1, algCfg.useRange - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(0, algCfg.mem.zeros); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(algCfg.useRange * algCfg.numANT * NUM_BYTE64, algCfg.useRange * NUM_BYTE64); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE64, algCfg.mem.fft1dMean0_1_SumAbs + NUM_BYTE32); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);	
	//sum abs
	BB_OPCLEAR(BB_CLEAR_P2);
	BB_OPGATE_EN(BB_GATE_P2);
	BB_P2_CFG0(MAX_FRAME_NUM - 2, algCfg.useRange - 1, algCfg.numANT - 1, P2_CFG0_DIV128, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	BB_P2_CFG1(algCfg.useRange * algCfg.numANT * NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG2(algCfg.useRange * NUM_BYTE32, algCfg.mem.fft1dMean0_1); //src0IntraInc,src0BaseAddr
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG4(0, 0); //src1IntraInc,src1BaseAddr
	BB_P2_CFG5(algCfg.useRange * NUM_BYTE64, NUM_BYTE64); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, algCfg.mem.fft1dMean0_1_SumAbs); //dstIntraInc,dstBaseAddr
	BB_ISR_CLEAR_ALL;
	BB_OPTRIG(BB_TRIG_P2);
	BB_waitISR(BB_ISR_P2_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_P2);	
	//4. abs to pf
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, MAX_FRAME_NUM - 2, algCfg.numANT-1, algCfg.useRange-1,DMA_CFG0_WMODE_FIX2PF, DMA_CFG0_DSIZE_64BIT);  //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(algCfg.useRange64*algCfg.numANT, algCfg.useRange64); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(NUM_BYTE64, algCfg.mem.fft1dMean0_1_SumAbs); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(algCfg.useRange32*algCfg.numANT, algCfg.useRange32); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.fft1dMean0_1_SumPf); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);		
#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft1dMean0_1_SumPf, algCfg.useRange32 *6);
		while (1);	
#endif		
}
	

//sum fft2d
void fft2dAbsSum(uint8_t numSum) {
	Write_M32(algCfg.mem.zeros, 0);
	//imag=0
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(0, algCfg.mem.zeros); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(0, algCfg.useRange * NUM_BYTE64); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE64, algCfg.mem.fft2dSumAbs_1 + NUM_BYTE32); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumAbs_1, algCfg.useRange * algCfg.numChirpDiv2 * NUM_BYTE64);
	while (1);
#endif
	//sum 1st ChirpDiv2
	BB_OPCLEAR(BB_CLEAR_P2);
	BB_OPGATE_EN(BB_GATE_P2);
	if(numSum == 1)
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 1 - 1, P2_CFG0_DIV8, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	}
	else if(numSum == 8)
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 8 - 1, P2_CFG0_DIV64, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode		
	}
	else
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 16 - 1, P2_CFG0_DIV128, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode		
	}
	BB_P2_CFG1(algCfg.useRange * algCfg.numANT * NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG2(algCfg.useRange * NUM_BYTE32, algCfg.mem.fft2d); //src0IntraInc,src0BaseAddr
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG4(0, 0); //src1IntraInc,src1BaseAddr
	BB_P2_CFG5(algCfg.useRange * NUM_BYTE64, NUM_BYTE64); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, algCfg.mem.fft2dSumAbs_1); //dstIntraInc,dstBaseAddr
	BB_ISR_CLEAR_ALL;
	BB_OPTRIG(BB_TRIG_P2);
	BB_waitISR(BB_ISR_P2_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_P2);
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumAbs_1, algCfg.useRange * NUM_BYTE64 * algCfg.numChirpDiv2);
	while (1);
#endif
	//abs to pf
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, algCfg.numChirpDiv2-1, algCfg.useRange-1,DMA_CFG0_WMODE_FIX2PF, DMA_CFG0_DSIZE_64BIT);  //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, algCfg.useRange64); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(NUM_BYTE64, algCfg.mem.fft2dSumAbs_1); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(0, algCfg.useRange32); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.fft2dSumPf); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);	
	
	//sum 2nd ChirpDiv2
	BB_OPCLEAR(BB_CLEAR_P2);
	BB_OPGATE_EN(BB_GATE_P2);
	if(numSum == 1)
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 1 - 1, P2_CFG0_DIV8, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	}
	else if(numSum == 8)
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 8 - 1, P2_CFG0_DIV64, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode		
	}
	else
	{
		BB_P2_CFG0(algCfg.numChirpDiv2 - 1, algCfg.useRange - 1, 16 - 1, P2_CFG0_DIV128, P2_CFG0_SUBMODE_ABS_SUM, P2_CFG0_MODE_ACC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode		
	}	
	BB_P2_CFG1(algCfg.useRange * algCfg.numANT * NUM_BYTE32, NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG2(algCfg.useRange * NUM_BYTE32, algCfg.mem.fft2d+algCfg.useRange32*algCfg.numANT*algCfg.numChirpDiv2); //src0IntraInc,src0BaseAddr
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG4(0, 0); //src1IntraInc,src1BaseAddr
	BB_P2_CFG5(algCfg.useRange * NUM_BYTE64, NUM_BYTE64); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, algCfg.mem.fft2dSumAbs_2); //dstIntraInc,dstBaseAddr
	BB_ISR_CLEAR_ALL;
	BB_OPTRIG(BB_TRIG_P2);
	BB_waitISR(BB_ISR_P2_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_P2);
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumAbs_1, algCfg.useRange * NUM_BYTE64 * algCfg.numChirpDiv2);
	while (1);
#endif
	//abs to pf
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, algCfg.numChirpDiv2-1, algCfg.useRange-1,DMA_CFG0_WMODE_FIX2PF, DMA_CFG0_DSIZE_64BIT);  //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, algCfg.useRange64); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(NUM_BYTE64, algCfg.mem.fft2dSumAbs_2); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(0, algCfg.useRange32); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.fft2dSumPf+algCfg.useRange32*algCfg.numChirpDiv2); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);	
#if 0
//            UART_Transmit_Bytes(&UART0, (uint8_t *)&head[0], 8);
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumPf, algCfg.useRange * NUM_BYTE32 * algCfg.numChirp);
	while (1);
#endif
}

void saveTargetAllAnt(void) {
	BB_CFARRST_T *pCfar = (BB_CFARRST_T *)algCfg.mem.cfarRsltV;
	uint32_t *pFFT2D = (uint32_t *)algCfg.mem.fft2d;
	uint32_t *pTargetAllAnt = (uint32_t *)algCfg.mem.targetAllAnt_Step1;
	uint16_t d1Idx, d2Idx;
	for (uint32_t cfarNow = 0; cfarNow < algCfg.cfarNumD; cfarNow++) {
		d1Idx = pCfar[cfarNow].interIdx;
		d2Idx = pCfar[cfarNow].intraIdx;
		for (uint32_t antNow = 0; antNow < algCfg.numANT; antNow++) {
			pTargetAllAnt[antNow + cfarNow * 16] = pFFT2D[d1Idx + d2Idx*algCfg.useRange*algCfg.numANT + antNow * algCfg.useRange];
		}
	}
	BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, algCfg.cfarNumD - 1, algCfg.numANT - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
	BB_DMA_Cfg1(0, algCfg.numANT*NUM_BYTE32); //srcJumpInc,srcInterInc
	BB_DMA_Cfg2(NUM_BYTE32, algCfg.mem.targetAllAnt_Step1); //srcIntraInc,srcBaseAddr
	BB_DMA_Cfg3(0, algCfg.numANT*NUM_BYTE32); //dstJumpInc,dstInterInc
	BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.targetAllAnt_Step2); //dstIntraInc,dstBaseAddr
	BB_OPCLEAR(BB_CLEAR_DMA);
	BB_ISR_CLEAR_ALL;
	BB_OPGATE_EN(BB_GATE_DMA);
	BB_OPTRIG(BB_TRIG_DMA);
	BB_waitISR(BB_ISR_DMA_JUMP_END);
	BB_OPGATE_DIS(BB_GATE_DMA);	
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.targetAllAnt_Step2, algCfg.cfarNumD * algCfg.numANT * NUM_BYTE32);
	while (1);
#endif
}

// cfar
void cfarCalc(void) {
	uint32_t targetNum = 0;
	uint32_t cfarNum = 0;
	uint16_t d1Idx, d2Idx;
	int16_t d1LastIdx, d1NextIdx;
	uint32_t d1LastMag, d1NextMag, d1CurrMag;	
	BB_CFARRST_T *bb_cfarRst_str = (BB_CFARRST_T *)algCfg.mem.cfarRsltTmp;
	BB_CFARRST_T *pCfar = (BB_CFARRST_T *)algCfg.mem.cfarRsltV;;	
	uint32_t *srcBuf = (uint32_t *)(algCfg.mem.fft2dSumPf + 0x400000);
	BUF_CFG0(BUF_CFG0_WR_FLT2PF_LSF_EN_RIGHT, 0, BUF_CFG0_CONVERT_CPX_MODE_FLOAT, BUF_CFG0_RD_POWMODE_ABS);
//	memcpy((uint8_t *)(BB_DBUF0_BASE), (uint8_t *)(algCfg.mem.fft2dSumPf), algCfg.useRange * NUM_BYTE32 * algCfg.numChirp);
	BB_CFAR_Vel_Init();
	BB_CFAR_CFG4(algCfg.mem.cfarRsltTmp); //dstBaseAddr	
	BB_OPCLEAR(BB_CLEAR_CFAR);
	BB_OPGATE_EN(BB_GATE_CFAR); //val,sw
	BB_ISR_CLEAR(BB_ISR_CFAR_END);
	BB_OPTRIG(BB_TRIG_CFAR); //val
	BB_waitISR(BB_ISR_CFAR_END);
	cfarNum = bb_regb_str->CFAR_STA0;
	// algCfg.cfarNumD = bb_regb_str->CFAR_STA0;	
	BB_OPGATE_DIS(BB_GATE_CFAR); //val,sw
	for(uint32_t i=0; i<cfarNum; i++)
	{
		d1Idx = bb_cfarRst_str[i].interIdx;
		d2Idx = bb_cfarRst_str[i].intraIdx;
		d1LastIdx = d1Idx - 1;
		d1NextIdx = d1Idx + 1;
		d1LastIdx = (d1LastIdx < 0) ? 1 : d1LastIdx;
		d1NextIdx = (d1NextIdx == algCfg.useRange) ? algCfg.useRange - 2 : d1NextIdx;			
		d1LastMag = srcBuf[d1LastIdx+d2Idx*algCfg.useRange];
		d1NextMag = srcBuf[d1NextIdx+d2Idx*algCfg.useRange];
		d1CurrMag = srcBuf[d1Idx+d2Idx*algCfg.useRange];			
		if (((d1CurrMag >= d1LastMag) && (d1CurrMag > d1NextMag)) || ((d1CurrMag > d1LastMag) && (d1CurrMag >= d1NextMag))) {
			if (d1LastMag >= d1CurrMag * algCfg.rangePeakRatioMul / algCfg.rangePeakRatioDiv)
			{
				pCfar[targetNum].interIdx = d1LastIdx;
				pCfar[targetNum].intraIdx = d2Idx;
				pCfar[targetNum].res = bb_cfarRst_str[i].res;
				targetNum++;					
			}
			pCfar[targetNum].interIdx = d1Idx;
			pCfar[targetNum].intraIdx = d2Idx;
			pCfar[targetNum].res = bb_cfarRst_str[i].res;
			targetNum++;
			if (d1NextMag >= d1CurrMag * algCfg.rangePeakRatioMul / algCfg.rangePeakRatioDiv)
			{
				pCfar[targetNum].interIdx = d1NextIdx;
				pCfar[targetNum].intraIdx = d2Idx;
				pCfar[targetNum].res = bb_cfarRst_str[i].res;
				targetNum++;								
			}
		}			
	}
	algCfg.cfarNumD = targetNum;
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.cfarRsltV, algCfg.cfarTargetNum * NUM_BYTE64);
	while(1);
#endif
	saveTargetAllAnt();
}

uint32_t cfarCalc2Col(uint32_t srcAddr, uint32_t refAddr, uint32_t rsltAddr)
{
		uint32_t targetNum = 0;
	  uint32_t cfarNum = 0;
	  uint16_t d1Idx, d2Idx;
	  int16_t d1LastIdx, d1NextIdx;
	  uint32_t d1LastMag, d1NextMag, d1CurrMag;
		STRUCT_BB_CFAR_CFG0 cfarCfg0;			
		BB_CFARRST_T *bb_cfarRst_str = (BB_CFARRST_T *)algCfg.mem.cfarRsltTmp;
		BB_CFARRST_T *pCfar = (BB_CFARRST_T *)rsltAddr;
		uint32_t *srcBuf = (uint32_t *)(srcAddr + 0x400000);
		cfarCfg0.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0.sumMode = CFAR_CFG0_SUMMODE_ABS;
		cfarCfg0.interCnt = algCfg.useRange;
		cfarCfg0.intraCnt = 1;
		cfarCfg0.thresDiv = CFAR_CFG0_THRESDIV1;
		cfarCfg0.divFac = CFAR_CFG0_DIVFAC1;
		cfarCfg0.mulFac = algCfg.cfarSMul;
		cfarCfg0.searchSize = 1;
		cfarCfg0.guardSize = 0;
		cfarCfg0.totalSize = cfarCfg0.searchSize+cfarCfg0.guardSize;
		cfarCfg0.pdEn = CFAR_CFG0_PEAKDET_DIS; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
		cfarCfg0.cfarMode = stFlashParam.cfarVmode;
		BB_CFAR_CFG0(&cfarCfg0); //STRUCT_CFAR_CFG0
		BB_CFAR_CFG1(algCfg.cfarMaxNum,NUM_BYTE32); //interInc
		BB_CFAR_CFG2(algCfg.useRange32, srcAddr);//intraInc,intraAddr
		BB_CFAR_CFG3(refAddr,refAddr);//wrapAddrA,wrapAddrB
		BB_CFAR_CFG4(algCfg.mem.cfarRsltTmp); //dstBaseAddr	
		BB_OPCLEAR(BB_CLEAR_CFAR);
		BB_OPGATE_EN(BB_GATE_CFAR); //val,sw
		BB_ISR_CLEAR(BB_ISR_CFAR_END);
		BB_OPTRIG(BB_TRIG_CFAR); //val
		BB_waitISR(BB_ISR_CFAR_END);
		cfarNum = bb_regb_str->CFAR_STA0;
		BB_OPGATE_DIS(BB_GATE_CFAR); //val,sw			
		for(uint32_t i=0; i<cfarNum; i++)
		{
			d1Idx = bb_cfarRst_str[i].interIdx;
			d2Idx = bb_cfarRst_str[i].intraIdx;
			d1LastIdx = d1Idx - 1;
			d1NextIdx = d1Idx + 1;
			d1LastIdx = (d1LastIdx < 0) ? 1 : d1LastIdx;
			d1NextIdx = (d1NextIdx == algCfg.useRange) ? algCfg.useRange - 2 : d1NextIdx;			
			d1LastMag = srcBuf[d1LastIdx];
			d1NextMag = srcBuf[d1NextIdx];
			d1CurrMag = srcBuf[d1Idx];			
			if (((d1CurrMag >= d1LastMag) && (d1CurrMag > d1NextMag)) || ((d1CurrMag > d1LastMag) && (d1CurrMag >= d1NextMag))) {
				if (d1LastMag >= d1CurrMag * algCfg.rangePeakRatioMul / algCfg.rangePeakRatioDiv)
				{
					pCfar[targetNum].interIdx = d1LastIdx;
					pCfar[targetNum].intraIdx = d2Idx;
					pCfar[targetNum].res = bb_cfarRst_str[i].res;
					targetNum++;					
				}
				pCfar[targetNum].interIdx = d1Idx;
				pCfar[targetNum].intraIdx = d2Idx;
				pCfar[targetNum].res = bb_cfarRst_str[i].res;
				targetNum++;
				if (d1NextMag >= d1CurrMag * algCfg.rangePeakRatioMul / algCfg.rangePeakRatioDiv)
				{
					pCfar[targetNum].interIdx = d1NextIdx;
					pCfar[targetNum].intraIdx = d2Idx;
					pCfar[targetNum].res = bb_cfarRst_str[i].res;
					targetNum++;								
				}
			}			
		}	
		return targetNum;
}

void cfarCalcS(void)
{
		uint32_t targetNum;
		Write_M32(algCfg.mem.zeros, 0xF0001FFF);
		//1. set 0 doppler max
		BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, algCfg.useRange - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg2(0, algCfg.mem.zeros); //srcIntraInc,srcBaseAddr
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.fft2dSumPf+(32)*algCfg.useRange32); //dstIntraInc,dstBaseAddr
		BB_OPCLEAR(BB_CLEAR_DMA);
		BB_ISR_CLEAR_ALL;
		BB_OPGATE_EN(BB_GATE_DMA);
		BB_OPTRIG(BB_TRIG_DMA);
		BB_waitISR(BB_ISR_DMA_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_DMA);		
#if 0
	UART_Transmit_Bytes(&UART0, (uint8_t *)algCfg.mem.fft2dSumPf, algCfg.useRange * NUM_BYTE32 * algCfg.numChirp);
	while (1);
#endif	
	  //2. get ref noise
		STRUCT_BB_CFAR_CFG0 cfarCfg0;	
		cfarCfg0.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0.sumMode = CFAR_CFG0_SUMMODE_ABS;
		cfarCfg0.interCnt = algCfg.useRange;
		cfarCfg0.intraCnt = 1;
		cfarCfg0.thresDiv = CFAR_CFG0_THRESDIV8;
		cfarCfg0.divFac = CFAR_CFG0_DIVFAC8;
		cfarCfg0.mulFac = 1;
		cfarCfg0.searchSize = stFlashParam.cfarThVS;
		cfarCfg0.guardSize = 0;
		cfarCfg0.totalSize = cfarCfg0.searchSize+cfarCfg0.guardSize;
		cfarCfg0.pdEn = CFAR_CFG0_PEAKDET_DIS; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
		cfarCfg0.cfarMode = stFlashParam.cfarVmode;
		BB_CFAR_CFG0(&cfarCfg0); //STRUCT_CFAR_CFG0
		BB_CFAR_CFG1(algCfg.cfarMaxNum,NUM_BYTE32); //interInc
		BB_CFAR_CFG2(algCfg.useRange32, algCfg.mem.fft2dSumPf+(32)*algCfg.useRange32);//intraInc,intraAddr
		BB_CFAR_CFG3((algCfg.mem.fft2dSumPf+(41)*algCfg.useRange32),algCfg.mem.fft2dSumPf+(16)*algCfg.useRange32);//wrapAddrA,wrapAddrB
		BB_CFAR_CFG4(algCfg.mem.cfarRes); //dstBaseAddr	
		BB_OPCLEAR(BB_CLEAR_CFAR);
		BB_OPGATE_EN(BB_GATE_CFAR); //val,sw
		BB_ISR_CLEAR(BB_ISR_CFAR_END);
		BB_OPTRIG(BB_TRIG_CFAR); //val
		BB_waitISR(BB_ISR_CFAR_END);
		targetNum = bb_regb_str->CFAR_STA0;
		BB_OPGATE_DIS(BB_GATE_CFAR); //val,sw	
		//3. set img 0
		Write_M32(algCfg.mem.zeros, 0x0);
		BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, targetNum - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg2(0, algCfg.mem.zeros); //srcIntraInc,srcBaseAddr
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(NUM_BYTE64, algCfg.mem.cfarResAbs+NUM_BYTE32); //dstIntraInc,dstBaseAddr
		BB_OPCLEAR(BB_CLEAR_DMA);
		BB_ISR_CLEAR_ALL;
		BB_OPGATE_EN(BB_GATE_DMA);
		BB_OPTRIG(BB_TRIG_DMA);
		BB_waitISR(BB_ISR_DMA_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_DMA);	
		//4. move real part
		BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, targetNum - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg2(NUM_BYTE64, algCfg.mem.cfarRes+NUM_BYTE32); //srcIntraInc,srcBaseAddr
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(NUM_BYTE64, algCfg.mem.cfarResAbs); //dstIntraInc,dstBaseAddr
		BB_OPCLEAR(BB_CLEAR_DMA);
		BB_ISR_CLEAR_ALL;
		BB_OPGATE_EN(BB_GATE_DMA);
		BB_OPTRIG(BB_TRIG_DMA);
		BB_waitISR(BB_ISR_DMA_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_DMA);			
		//5. abs to pf
		BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, targetNum - 1,DMA_CFG0_WMODE_FIX2PF, DMA_CFG0_DSIZE_64BIT);  //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg2(NUM_BYTE64, algCfg.mem.cfarResAbs); //srcIntraInc,srcBaseAddr
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(NUM_BYTE32, algCfg.mem.cfarResPf); //dstIntraInc,dstBaseAddr
		BB_OPCLEAR(BB_CLEAR_DMA);
		BB_ISR_CLEAR_ALL;
		BB_OPGATE_EN(BB_GATE_DMA);
		BB_OPTRIG(BB_TRIG_DMA);
		BB_waitISR(BB_ISR_DMA_JUMP_END);
		BB_OPGATE_DIS(BB_GATE_DMA);			
#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.cfarResPf, algCfg.useRange*NUM_BYTE32);
		while(1);
#endif
		//6. cfar
		for(uint8_t n = 0; n < 6; n++)
		{
				algCfg.cfarNumS[n] = cfarCalc2Col(algCfg.mem.fft1dMean0_1_SumPf + n*algCfg.useRange32, algCfg.mem.cfarResPf, algCfg.mem.cfarRslt0_1 + n*algCfg.cfarMaxNum*NUM_BYTE64);
		}
}

uint32_t fft3dProc(uint32_t dataBuf, uint32_t cfarNum, BB_CFARRST_T *cfarRslt, STRUCT_FRAMEPOINT *frameRst, uint8_t isStaticMode)
{
	uint32_t frameTargetNum = 0;
	if(cfarNum)
	{
		uint64_t sDMA_Cfg2 = 0;
		uint32_t sDPK_Cfg0 = 0;
		BB_CFARRST_T *bb_cfarRst_str = cfarRslt;
		BB_DPKRST_T *bb_dpkRst_str = (BB_DPKRST_T *)(algCfg.mem.dpkDstFirst);	
		uint32_t *FFT3D_Buf = (uint32_t *)(algCfg.mem.fft3dRslt);
		/*FFT3D,DPK*/
		BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL, DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, algCfg.numAntA - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(0, BB_FFT_BASE); //dstIntraInc,dstBaseAddr
		BB_FFT_Cfg0(0, 0, algCfg.fft3d_useA, algCfg.fft3d_useB, algCfg.numAntA - 1, FFTPT_128, FFT_UNLOAD_EN, FFT_MODE_FORWARD); //rsfOutput,rsfInput,useA,useB,inPt,fftPt,unloadEn,mode   //useAuseB
		BB_FFT_ZP_Clear();
		bb_regb_str->FFT_SRC_ZP00 = (uint64_t)0xFF;
		bb_regb_str->FFT_SRC_ZP01 = (uint64_t)0;
		BB_FFT_Cfg1(algCfg.mem.winRange, FFT_CFG1_OUTLSFEN_RIGHT, 0, FFT_CFG1_INLSFEN_RIGHT, 0, FFT_CFG1_WIN_SIZE_14BIT, FFT_CFG1_WIN_DIS); //winBaseAddr,winSize,winEn
		BB_FFT_Cfg2(NUM_BYTE32, algCfg.mem.fft3dRslt); //dstIntraInc,dstBaseAddr
		BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);

		BB_OPCLEAR(BB_GATE_DMA | BB_GATE_FFT | BB_GATE_DPK);
		BB_OPGATE_EN(BB_GATE_DMA | BB_GATE_FFT | BB_GATE_DPK);
		if(isStaticMode)
			sDMA_Cfg2 = (uint64_t)(algCfg.useRange * NUM_BYTE32) << BB_BIT32_POS | (dataBuf + bb_cfarRst_str[0].interIdx * NUM_BYTE32 + algCfg.numAntA * algCfg.useRange * NUM_BYTE32);
		else
			sDMA_Cfg2 = (uint64_t)(NUM_BYTE32) << BB_BIT32_POS | (dataBuf + algCfg.numAntA * NUM_BYTE32);			
		sDPK_Cfg0 = (uint64_t)(algCfg.dpkTimes) << BB_BIT8_POS | (uint64_t)(algCfg.dpkLenLog) << BB_BIT4_POS | (uint64_t)DPK_CFG0_MODE_MINUS;
		for (uint32_t cfarNow = 0; cfarNow < cfarNum; cfarNow++) {
			bb_regb_str->DMA_CFG2 = sDMA_Cfg2;
			BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
			BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
			BB_OPTRIG(BB_TRIG_DMA);
			if(isStaticMode)
				sDMA_Cfg2 = (uint64_t)(algCfg.useRange * NUM_BYTE32) << BB_BIT32_POS | (dataBuf + bb_cfarRst_str[cfarNow + 1].interIdx * NUM_BYTE32 + algCfg.numAntA * algCfg.useRange * NUM_BYTE32);
			else
				sDMA_Cfg2 = (uint64_t)(NUM_BYTE32) << BB_BIT32_POS | (dataBuf + (cfarNow+1) * algCfg.numANT * NUM_BYTE32 + algCfg.numAntA * NUM_BYTE32);
			BB_waitISR(BB_ISR_FFT_UNLOAD_END);
	#if 0  //cpx
			UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.fft3dRslt, algCfg.numAngle * NUM_BYTE32);
	#endif
			/*DPK*/
	#if 1
			bb_dpkRst_str[0].cutPow = bb_regb_str->FFT_STA0 >> BB_BIT32_POS;
			bb_dpkRst_str[0].idx = bb_regb_str->FFT_STA0 & (uint64_t)0xFFF;
			bb_dpkRst_str[0].pFloat = FFT3D_Buf[BB_FFT_STA0_curMaxIdx];
			bb_regb_str->DPK_CFG0 = (uint64_t)(bb_dpkRst_str[0].pFloat) << BB_BIT32_POS | (uint64_t)(bb_dpkRst_str[0].idx ) << BB_BIT12_POS | sDPK_Cfg0;
			BB_OPCLEAR(BB_GATE_DPK);
			BB_ISR_CLEAR(BB_ISR_DPK_END);
			BB_OPTRIG(BB_TRIG_DPK);
			BB_waitISR(BB_ISR_DPK_END);
			for (uint8_t dpkNow = 0; dpkNow < algCfg.dpkTimes; dpkNow++) {
				if (bb_dpkRst_str[dpkNow].cutPow > bb_dpkRst_str[algCfg.dpkTimes].res * algCfg.dpkThres) {
					frameRst[frameTargetNum].d1Idx = bb_cfarRst_str[cfarNow].interIdx;
					frameRst[frameTargetNum].d2Idx = bb_cfarRst_str[cfarNow].intraIdx;
					frameRst[frameTargetNum].d3Idx = bb_dpkRst_str[dpkNow].idx;
					frameRst[frameTargetNum].cfarIdx = cfarNow;
					frameRst[frameTargetNum].pow3dAbs = bb_dpkRst_str[dpkNow].cutPow;
					frameTargetNum++;					
				}
	#if 0 //dpk output
				if (dpkNow == 0) {
					bb_dpkRst_str[dpkNow].real = 0;
					bb_dpkRst_str[dpkNow].imag = 0;
				}
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow].cutPow, 4);
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow].idx, 4);
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow].real, 4);
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow].imag, 4);
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow + 1].res, 4);
				UART_Transmit_Bytes(&UART0, (uint8_t*)&bb_dpkRst_str[dpkNow].pFloat, 4);

	#endif
			}

	#endif
		}
		BB_OPGATE_DIS(BB_GATE_DMA | BB_GATE_FFT | BB_GATE_DPK);	
	}
	return frameTargetNum;
}

void fft4dProc(uint32_t dataBuf, uint32_t targetNum, STRUCT_FRAMEPOINT *frameRst, uint8_t isStaticMode)
{
#if 1
	BB_P2_CFG0(0, algCfg.numAntE - 1, algCfg.numRX - 1, P2_CFG0_DIV1, P2_CFG0_SUBMODE_CPX_SUM_MUL, P2_CFG0_MODE_MAC); //jumpCnt,interCnt,intraCnt,rsfBit,sub_mode,mode
	if (isStaticMode)
		BB_P2_CFG1(0, algCfg.numRX * algCfg.useRange * NUM_BYTE32); //src0JumpInc,src0InterInc
	else
		BB_P2_CFG1(0, algCfg.numRX * NUM_BYTE32); //src0JumpInc,src0InterInc
	BB_P2_CFG3(0, 0); //src1JumpInc,src1InterInc
	BB_P2_CFG5(0, 0); //dstJumpInc,dstInterInc
	BB_P2_CFG6(0, BB_FFT_BASE); //dstIntraInc,dstBaseAddr
	BB_P2_CFG7(P2_CFG7_SRC0CONJ_DIS, P2_CFG7_SRC0LSF_RIGHT, 0,
	           P2_CFG7_SRC1CONJ_DIS, P2_CFG7_SRC1LSF_RIGHT, 0,
	           P2_CFG7_DSTCONJ_DIS, P2_CFG7_DSTLSF_RIGHT, 0);
	BB_FFT_Cfg0(0, 0, algCfg.fft4d_useA, algCfg.fft4d_useB, algCfg.numAntE - 1, FFTPT_128, FFT_UNLOAD_EN, FFT_MODE_FORWARD); //rsfOutput,rsfInput,useA,useB,inPt,fftPt,unloadEn,mode   //useA>useB
	BB_FFT_Cfg1(algCfg.mem.winRange, FFT_CFG1_OUTLSFEN_RIGHT, 0, FFT_CFG1_INLSFEN_RIGHT, 0, FFT_CFG1_WIN_SIZE_14BIT, FFT_CFG1_WIN_DIS); //winBaseAddr,winSize,winEn
	BB_FFT_Cfg2(NUM_BYTE32, algCfg.mem.fft4dRslt); //dstIntraInc,dstBaseAddr
	BB_FFT_ZP_Clear();
	bb_regb_str->FFT_SRC_ZP00 = (uint64_t)0x07;
	bb_regb_str->FFT_SRC_ZP01 = (uint64_t)0;
	BB_OPGATE_EN(BB_GATE_P2 | BB_GATE_FFT);
	for (uint32_t targetNow = 0; targetNow < targetNum; targetNow++) {
		if (isStaticMode)
			BB_P2_CFG2(algCfg.useRange * NUM_BYTE32, dataBuf + frameRst[targetNow].d1Idx * NUM_BYTE32); //src0IntraInc,src0BaseAddr
		else
			BB_P2_CFG2(NUM_BYTE32, dataBuf + frameRst[targetNow].cfarIdx * algCfg.numANT * NUM_BYTE32); //src0IntraInc,src0BaseAddr			
		BB_P2_CFG4(algCfg.numAngle * NUM_BYTE32, algCfg.mem.steeringVec + frameRst[targetNow].d3Idx * NUM_BYTE32); //src1IntraInc,src1BaseAddr
#if 1
		BB_OPCLEAR(BB_CLEAR_P2);
		BB_ISR_CLEAR(BB_ISR_P2_JUMP_END);
		BB_OPTRIG(BB_TRIG_P2);
		BB_waitISR(BB_ISR_P2_JUMP_END);
#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.fft4dPre, algCfg.numAntE * NUM_BYTE32);
#endif
#if 0
		BB_DMA_Cfg0(DMA_CFG0_MINMAX_MODE_ABS, DMA_CFG0_FLT2PF_LSF_EN_RIGHT, 0, 0, 0, algCfg.numAntE - 1, DMA_CFG0_WMODE_ORI, DMA_CFG0_DSIZE_32BIT); //jumpCnt,interCnt,intraCnt,fix2pf_sel,dsize
		BB_DMA_Cfg1(0, 0); //srcJumpInc,srcInterInc
		BB_DMA_Cfg2(NUM_BYTE32, FFT4D_PRE_ADDR); //srcIntraInc,srcBaseAddr
		BB_DMA_Cfg3(0, 0); //dstJumpInc,dstInterInc
		BB_DMA_Cfg4(0, BB_FFT_BASE); //dstIntraInc,dstBaseAddr
		BB_FFT_Cfg0(0, 0, FFT4D_USE_A, FFT4D_USE_B, algCfg.numAntE - 1, FFTPT_128, FFT_UNLOAD_EN, FFT_MODE_FORWARD); //rsfOutput,rsfInput,useA,useB,inPt,fftPt,unloadEn,mode   //useAuseB
		BB_FFT_Cfg1(WIN_RANGE_ADDR, FFT_CFG1_OUTLSFEN_RIGHT, 0, FFT_CFG1_INLSFEN_RIGHT, 0, FFT_CFG1_WIN_SIZE_14BIT, FFT_CFG1_WIN_DIS); //winBaseAddr,winSize,winEn
		BB_FFT_Cfg2(NUM_BYTE32, FFT4D_CACHE_ADDR); //dstIntraInc,dstBaseAddr
		BB_FFT_ZP_Clear();
		bb_regb_str->FFT_SRC_ZP00 = (uint64_t)0x7;
		bb_regb_str->FFT_SRC_ZP01 = (uint64_t)0;
		//              bb_regb_str->FFT_SRC_ZP02 = (uint64_t)0;
		//              bb_regb_str->FFT_SRC_ZP03 = (uint64_t)0;
		BB_OPCLEAR(BB_CLEAR_FFT | BB_CLEAR_DMA);
		BB_OPGATE_EN(BB_GATE_DMA | BB_GATE_FFT);
		BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
		BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
		BB_OPTRIG(BB_TRIG_DMA);
		BB_waitISR(BB_ISR_FFT_UNLOAD_END);
#endif
#endif
		//FFT4D
		BB_OPCLEAR(BB_CLEAR_FFT | BB_CLEAR_P2);
		BB_FFT_CMD0(FFT_CMD0_CLEAR_IBUF);
		BB_ISR_CLEAR(BB_ISR_FFT_UNLOAD_END);
		BB_OPTRIG(BB_TRIG_P2);
		BB_waitISR(BB_ISR_FFT_UNLOAD_END);

#if 0
		UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.fft4dRslt, algCfg.numAngle * NUM_BYTE32);
#endif
		frameRst[targetNow].d4Idx = BB_FFT_STA0_curMaxIdx;
	}
	BB_OPGATE_DIS(BB_GATE_P2 | BB_GATE_FFT);
#endif	
}
void AngleCalcD(void)
{
	STRUCT_FRAMEPOINT *frameRst = (STRUCT_FRAMEPOINT *)algCfg.mem.frameRstD;
	algCfg.targetNumD = fft3dProc(algCfg.mem.targetAllAnt_Step2, algCfg.cfarNumD, (BB_CFARRST_T *)algCfg.mem.cfarRsltV, frameRst, 0);
  fft4dProc(algCfg.mem.targetAllAnt_Step2, algCfg.targetNumD, frameRst, 0);
#if 0	
	printf("targetFrameD\n");	
	for(uint32_t targetNow = 0; targetNow < algCfg.targetNumD; targetNow++)
	{
		printf("d1=%d,d2=%d,d3=%d,d4=%d\n",frameRst[targetNow].d1Idx+1,frameRst[targetNow].d2Idx+1,frameRst[targetNow].d3Idx+1,frameRst[targetNow].d4Idx+1);
	}	
#endif	
}

void angleCalcS(void)
{
	STRUCT_FRAMEPOINT *frameRst;
	BB_CFARRST_T *cfarRst;
	uint32_t dataBuf;
	for(uint8_t n = 0; n < 6; n++)
	{
		frameRst = (STRUCT_FRAMEPOINT *)(algCfg.mem.frameRstS0_1+algCfg.cfarMaxNum*algCfg.dpkTimes*sizeof(STRUCT_FRAMEPOINT)*n);
		cfarRst = (BB_CFARRST_T *)(algCfg.mem.cfarRslt0_1+algCfg.cfarMaxNum*NUM_BYTE64*n);
		dataBuf = algCfg.mem.fft1dMean0_1 + algCfg.useRange32*algCfg.numANT*n;
		algCfg.targetNumS[n] = fft3dProc(dataBuf, algCfg.cfarNumS[n], cfarRst, frameRst, 1);
		fft4dProc(dataBuf, algCfg.targetNumS[n], frameRst, 1);
#if 0		
		printf("targetFrame0_%d\n", n+1);
		for(uint32_t targetNow=0;targetNow<algCfg.targetNumS[n];targetNow++)
		{
			printf("d1=%d,d2=%d,d3=%d,d4=%d\n",frameRst[targetNow].d1Idx+1,frameRst[targetNow].d2Idx+32+1,frameRst[targetNow].d3Idx+1,frameRst[targetNow].d4Idx+1);
		}
#endif		
	}
}

#define M_PI       (3.1415926f)
#define PHI_MULFAC  2.0f*(M_PI/180.0f) //单位：度
void getPointCloud(STRUCT_FRAMEPOINT *frameRst) {
	// printf("target=%d\n",algCfg.frameTargetNum);
#if 1 //print frameRst
	uint32_t ProtocolSendLen = ProtocolSendPack(algCfg.targetNumD, frameRst, (uint8_t *)algCfg.mem.protocolPack, bb_RunTime);
	UART_Transmit_Bytes(&UART0, (uint8_t*)algCfg.mem.protocolPack, ProtocolSendLen);
#endif
}

/*alg Loop*/

uint8_t Func01_Handle(void)
{
//	memset((uint8_t *)BB_ABUF0_BASE,0,1048576+128*1024);
//	memset((uint8_t *)BB_DBUF0_BASE,0,128*1024);
	if (sampDataHandle() == TRUE) {
#if 1
//		UART_Transmit_Bytes(&UART0,(uint8_t*)&head[0],8);
//		UART_Transmit_Bytes(&UART0,(uint8_t *)BB_ABUF0_BASE,algCfg.numSamp*algCfg.numByte*algCfg.numChirp*algCfg.numANT);
//		for(uint32_t chirpNow=0; chirpNow<algCfg.numChirp; chirpNow++)
//		{
//			UART_Transmit_Bytes(&UART0,(uint8_t *)BB_ABUF0_BASE+16*1024*chirpNow,algCfg.numSamp*algCfg.numByte);			
//		}
		taskENTER_CRITICAL();           //进入临界区
		OSPI_DMA_Trans64_IRQ_16kB((uint64_t *)algCfg.mem.adcSamp, 1048576);
		taskEXIT_CRITICAL();            //退出临界区
//		vTaskDelay(pdMS_TO_TICKS(50));
#endif
#if 0
		OSPI_DMA_Trans64_IRQ((uint64_t *)BB_ABUF0_BASE, 1048576);
		//		printf("test\n");
		Cnt_Delay_ms(100);
#endif
	} else {
		return FALSE;
	}
	return TRUE;
}

/**********************************FUNC*****************************************/
uint8_t Func02_Handle(void)
{
//	STRUCT_FRAMEPOINT *frameRst = (STRUCT_FRAMEPOINT *)algCfg.mem.frameRstD;	
	uint8_t rtn = FALSE;
	BB_AlgProcFinished = 0;
	algCfg.targetNumD = 0;
	for(uint8_t n = 0; n < 6; n++)
	{
		algCfg.targetNumS[n]=0;
	}
//	TEST_WritePin = 1;
	/*samp+fft1d*/
	rtn=sampDataHandle();	
	/*alg3: fft1d mean*/
	fft1dMeanCalc(algCfg.meanEn);
	/*alg4: fft2d*/
	fft2dCalc();
	/*alg5:sum fft2d*/
	fft2dAbsSum(algCfg.numSum);
	/*alg6:cfar*/
	cfarCalc();
	/*alg10: AngleCalc*/
	AngleCalcD();
	/*alg11: fft1d Mean Minus*/
  fft1dMeanMinus();
	/*alg12: cfar*/
	cfarCalcS();
	/*alg13: AngleCalcS*/
	angleCalcS();
	frameCnt++;
//	printf("targetNumD=%d, ", algCfg.targetNumD);
//	for(uint8_t n=0;n<6;n++)
//	{
//		printf("targetNumS0_%d=%d, ", n, algCfg.targetNumS[n]);
//	}
//	printf("\n");
	/*alg11:generate pointCloud*/
//	getPointCloud(frameRst);
//	TEST_WritePin = 0;
	xSemaphoreGive(xSemaphoreBB);
	/*delay*/
	vTaskDelay(pdMS_TO_TICKS(2));
	BB_AlgProcFinished = 1;
	if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(10000)) == pdFAIL) {
		// 处理错误或超时
		printf("BB_ERR:DELAY\n");
//		WDT_RstNow();
		while (1);
	}

	/*print*/
#if 0
//	while(1);
	/*deep sleep*/
//	RF_WriteRegSPI(CS1_PIN,ANA_CFG21_ADDR,0);
//	DeepSleep_Config();
	Deep_Sleep();
//	RF_Clock40M_En();
//	HAL_SYS_CLK_Init(&stHRC,&stPLL);
	BSP_ADC_Init();
#endif
	return rtn;
}
