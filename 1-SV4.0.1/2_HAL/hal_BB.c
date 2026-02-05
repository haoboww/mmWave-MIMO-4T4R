#include "hal_BB.h"
#include "main.h"

STRUCT_BB_CFAR_CFG0 cfarCfg0_st;

/********************************************dataPrep*****************************************************/
void BB_PrepConfig(STRUCT_BB_PREP *bb_prep_st){
    /*write prep reg*/
    BB_REGB->PREP_CFG0 = ((uint64_t)bb_prep_st->cfg0.adc_clk_mode    << BB_BIT0_POS) |
                             ((uint64_t)bb_prep_st->cfg0.adc_dsize       << BB_BIT1_POS) |
                             ((uint64_t)bb_prep_st->cfg0.adc_addr_mode   << BB_BIT3_POS) |
                             ((uint64_t)bb_prep_st->cfg0.max_removal_dis << BB_BIT4_POS) |
                             ((uint64_t)bb_prep_st->cfg0.down_fac        << BB_BIT8_POS) |
                             ((uint64_t)bb_prep_st->cfg0.cic_sec         << BB_BIT12_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tim_rma         << BB_BIT16_POS)|
                             ((uint64_t)bb_prep_st->cfg0.cnt_edge        << BB_BIT26_POS)|
                             ((uint64_t)bb_prep_st->cfg0.samp_edge       << BB_BIT27_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tim_mode        << BB_BIT28_POS)|
                             ((uint64_t)bb_prep_st->cfg0.circ_en         << BB_BIT30_POS)|
                             ((uint64_t)bb_prep_st->cfg0.samp_pt         << BB_BIT40_POS)|
                             ((uint64_t)bb_prep_st->cfg0.chirp_num       << BB_BIT44_POS)|
                             ((uint64_t)bb_prep_st->cfg0.rx_num          << BB_BIT56_POS)|
                             ((uint64_t)bb_prep_st->cfg0.tx_num          << BB_BIT60_POS);
    BB_REGB->PREP_CFG1 = ((uint64_t)bb_prep_st->cfg1.rx_ant_seq0 << BB_BIT0_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq1 << BB_BIT4_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq2 << BB_BIT8_POS) |
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq3 << BB_BIT12_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq4 << BB_BIT16_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq5 << BB_BIT20_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq6 << BB_BIT24_POS)|
                             ((uint64_t)bb_prep_st->cfg1.rx_ant_seq7 << BB_BIT28_POS)|
                             ((uint64_t)bb_prep_st->cfg1.tx_ant_seq  << BB_BIT32_POS)|
                             ((uint64_t)bb_prep_st->cfg1.tx_ant_mode << BB_BIT36_POS);

    BB_REGB->PREP_CFG2 = ((uint64_t)bb_prep_st->cfg2.interInc << BB_BIT0_POS) |
                             ((uint64_t)bb_prep_st->cfg2.jumpInc  << BB_BIT32_POS) ;
    BB_REGB->PREP_CFG3 = (uint64_t)bb_prep_st->cfg3.prep_baddr << BB_BIT0_POS;
}
/****************************************change ********************************************************/
uint8_t BB_sampPT2reg(uint16_t sampPT){
	uint8_t rtn;
	switch(sampPT){
		case 2:
			rtn=NUMLOG_2;
			break;
		case 4:
			rtn=NUMLOG_4;
			break;
		case 8:
			rtn=NUMLOG_8;
			break;
		case 16:
			rtn=NUMLOG_16;
			break;
		case 32:
			rtn=NUMLOG_32;
			break;
		case 64:
			rtn=NUMLOG_64;
			break;
		case 128:
			rtn=NUMLOG_128;
			break;
		case 256:
			rtn=NUMLOG_256;
			break;
		case 512:
			rtn=NUMLOG_512;
			break;
		case 1024:
			rtn=NUMLOG_1024;
			break;
		case 2048:
			rtn=NUMLOG_2048;
			break;
		case 4096:
			rtn=NUMLOG_4096;
			break;																				
	}
	return rtn;
}
uint8_t BB_samprate2reg(float sSamprate){
	uint8_t rtn;
	if(sSamprate==20){
		rtn=PREP_CFG0_DOWN_FAC_1;
	}else if(sSamprate==10){
		rtn=PREP_CFG0_DOWN_FAC_2;
	}else if(sSamprate==6.66f){
		rtn=PREP_CFG0_DOWN_FAC_3;
	}else if(sSamprate==5){
		rtn=PREP_CFG0_DOWN_FAC_4;
	}else if(sSamprate==4){
		rtn=PREP_CFG0_DOWN_FAC_5;
	}else if(sSamprate==3.33f){
		rtn=PREP_CFG0_DOWN_FAC_6;
	}else if(sSamprate==2.85f){
		rtn=PREP_CFG0_DOWN_FAC_7;
	}else if(sSamprate==2.5f){
		rtn=PREP_CFG0_DOWN_FAC_8;
	}
	return rtn;
}
uint8_t BB_prepmode2reg(uint8_t sampBit){
	uint8_t rtn;
	switch(sampBit){
		case 8:
			rtn=PREP_CFG0_ADC_ADDR_MODE_8BIT;
			break;
		case 10:
		case 12:
		case 14:
			rtn=PREP_CFG0_ADC_ADDR_MODE_16BIT;
			break;
	}
	return rtn;
}
uint8_t BB_prepBit2reg(uint8_t sampBit){
	uint8_t rtn;
	switch(sampBit){
		case 8:
			rtn=PREP_CFG0_ADC_DSIZE_8BIT;
			break;
		case 10:
			rtn=PREP_CFG0_ADC_DSIZE_10BIT;
			break;			
		case 12:
			rtn=PREP_CFG0_ADC_DSIZE_12BIT;
			break;				
		case 14:
			rtn=PREP_CFG0_ADC_DSIZE_14BIT;
			break;
	}
	return rtn;
}
/**********************************write WBUF************************************************************/

void BB_WBUF_W16(uint32_t WBUF_Addr,uint16_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M16(WBUF_Addr+2*i, Data[i]);
    }
}
void BB_WBUF_W32(uint32_t WBUF_Addr,uint32_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M32(WBUF_Addr+4*i, Data[i]);
    }
}
void BB_WBUF_W64(uint32_t WBUF_Addr,uint64_t *Data,uint32_t len){
    for(uint32_t i=0; i<len; i++){
        Write_M64(WBUF_Addr+8*i, Data[i]);
    }
}
/*********************************************************************************************/
void BB_FFT1D_ZP_Init(void){ //range

}
//void BB_FFT2D_ZP_Init(void){ //chirp
//    uint32_t sCmd=NUM_CHIRP*NUM_TX;
//    switch(sCmd){
//        case 32:
//            break;
//        case 64:
//            for(uint32_t txNow=0;txNow<NUM_TX;txNow++){
//                fft2d_zp.txNum_zp0[txNow] = 0;
//                for(uint32_t chirpNow=0;chirpNow<NUM_CHIRP;chirpNow++){
//                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
//                }
//            }
//            break;
//        case 256:
//            for(uint32_t txNow=0;txNow<NUM_TX;txNow++){
//                fft2d_zp.txNum_zp0[txNow] = 0;
//                fft2d_zp.txNum_zp1[txNow] = 0;
//                fft2d_zp.txNum_zp2[txNow] = 0;
//                fft2d_zp.txNum_zp3[txNow] = 0;
//                for(uint32_t chirpNow=0;chirpNow<NUM_CHIRP/2;chirpNow++){
//                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
//                    fft2d_zp.txNum_zp1[txNow] = fft2d_zp.txNum_zp1[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
//                    fft2d_zp.txNum_zp2[txNow] = fft2d_zp.txNum_zp2[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
//                    fft2d_zp.txNum_zp3[txNow] = fft2d_zp.txNum_zp3[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*NUM_TX);
//                }
//            }
//            break;
//    }
//}
void BB_FFT_ZP_Clear(void){
    BB_REGB->FFT_SRC_ZP00=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP01=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP02=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP03=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP04=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP05=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP06=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP07=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP08=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP09=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP10=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP11=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP12=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP13=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP14=0xFFFFFFFFFFFFFFFFULL;
    BB_REGB->FFT_SRC_ZP15=0xFFFFFFFFFFFFFFFFULL;
}
/**************************************************P2****************************************************************/

/**************************************************CFAR****************************************************************/
void BB_CFAR_CFG0(STRUCT_BB_CFAR_CFG0 *sCfarCfg0){
    BB_REGB->CFAR_CFG0 = (uint64_t)sCfarCfg0->wrapDirA  <<BB_BIT61_POS|
												 (uint64_t)sCfarCfg0->wrapDirB  <<BB_BIT60_POS|
												 (uint64_t)sCfarCfg0->sumMode   <<BB_BIT56_POS|
												 (uint64_t)sCfarCfg0->interCnt  <<BB_BIT44_POS|
												 (uint64_t)sCfarCfg0->intraCnt  <<BB_BIT32_POS|
												 (uint64_t)sCfarCfg0->thresDiv  <<BB_BIT24_POS|
												 (uint64_t)sCfarCfg0->divFac    <<BB_BIT20_POS|
												 (uint64_t)sCfarCfg0->mulFac    <<BB_BIT12_POS|
												 (uint64_t)sCfarCfg0->searchSize<<BB_BIT8_POS|
												 (uint64_t)sCfarCfg0->guardSize <<BB_BIT4_POS|
												 (uint64_t)sCfarCfg0->pdEn      <<BB_BIT2_POS|
												 (uint64_t)sCfarCfg0->cfarMode  <<BB_BIT0_POS;
}

//void BB_CFAR_Init(void){
//        cfarCfg0_st.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
//        cfarCfg0_st.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
//        cfarCfg0_st.sumMode = CFAR_CFG0_SUMMODE_ABS;
//        cfarCfg0_st.interCnt = USE_RANGE;
//        cfarCfg0_st.intraCnt = NUM_VEL;
//        cfarCfg0_st.thresDiv = CFAR_CFG0_THRESDIV4;           // = searchSize num
//        cfarCfg0_st.divFac = CFAR_CFG0_DIVFAC1;
//        cfarCfg0_st.mulFac = r2_str.cfarThV;
//        cfarCfg0_st.searchSize = r2_str.cfarThVS;
//        cfarCfg0_st.guardSize = r2_str.cfarThVG;
//        cfarCfg0_st.totalSize = cfarCfg0_st.searchSize+cfarCfg0_st.guardSize;
//        cfarCfg0_st.pdEn = CFAR_CFG0_PEAKDET_DIS; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
//        cfarCfg0_st.cfarMode = CFAR_CFG0_CFARMODE_SO;
//        BB_CFAR_CFG0(&cfarCfg0_st); //STRUCT_CFAR_CFG0
//        BB_CFAR_CFG1(CFAR_MAX_NUM,NUM_BYTE32); //interInc
//        BB_CFAR_CFG2(NUM_BIG_CHIRP_BYTES,FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32);//intraInc,intraAddr
//        BB_CFAR_CFG3((FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32+(NUM_VEL-cfarCfg0_st.totalSize)*NUM_BIG_CHIRP_BYTES),FFT2D_CACHE_ADDR+USE_RANGE*CFAR_ANT*NUM_BYTE32);//wrapAddrA,wrapAddrB
//        BB_CFAR_CFG4(CFAR_CACHE_ADDR);//dstBaseAddr
//}
/**************************************************DPK****************************************************************/

/**************************************************IRQ****************************************************************/
void BB_IRQ_Init(void){
		NVIC_SetPriority (BB_IRQn, BB_PRIORITY_LV);
    NVIC_EnableIRQ(BB_IRQn);
//    BB_IMR_Init(bb_imr_val);
}





void BB_waitISR(uint64_t val){
		BB_timeoutCnt=0;
    while( (BB_REGB->ISR & val)==0 ){
			if(BB_timeoutCnt>1000){
				// 处理错误或超时
				printf("BB_ERR:timeout=%08X\n",BB_isrSet);
				// CHPIEN_WritePin=0;
				WDT_RstNow();
				while(1);			
			}
		}
    BB_REGB->ISR = val;
}
void BB_waitISR_IRQ(uint32_t isrVal,uint32_t trigVal){
	BaseType_t xResult;
	ulTaskNotifyTake(pdTRUE, 0); 
	BB_isrSet=isrVal;
	BB_IMR_SET(isrVal);//equal to isr
	BB_OPTRIG(trigVal);
	xResult = ulTaskNotifyTake(pdTRUE,pdMS_TO_TICKS(1000));	
	if (xResult == pdFAIL){
			// 处理错误或超时
		printf("BB_ERR:waitISR=%08X\n",BB_isrSet);
		CHPIEN_WritePin=0;
		WDT_RstNow();
		while(1);
	}
	BB_IMR_CLEAR(isrVal);

}
void BB_waitESR(uint64_t val){
    while( (BB_REGB->ESR & val)==0 );
    BB_REGB->ESR     = val; //all clear
}


void BB_CpxFix2pf(uint32_t *In,uint32_t *out,uint32_t intraCnt){
    if(intraCnt<=4096){
        BB_OPCLEAR(BB_CLEAR_DMA);

        BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,0,intraCnt-1,DMA_CFG0_WMODE_FIX2PF,DMA_CFG0_DSIZE_64BIT);
        BB_DMA_Cfg1(0,0);//srcJumpInc,srcInterInc
        BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
        BB_DMA_Cfg3(0,0);//dstJumpInc,dstInterInc
        BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
        BB_OPGATE_EN(BB_GATE_DMA);
        BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
        BB_OPTRIG(BB_TRIG_DMA);
        BB_waitISR(BB_ISR_DMA_JUMP_END);
        BB_OPGATE_DIS(BB_GATE_DMA);
    }
}

void BB_CpxFlt2pf_short(uint32_t *In,uint32_t *out,uint32_t intraCnt){
    if(intraCnt<=4096)
    {
        BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,0,intraCnt-1,DMA_CFG0_WMODE_F2PF,DMA_CFG0_DSIZE_64BIT);
        BB_DMA_Cfg1(0,0);//srcJumpInc,srcInterInc
        BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
        BB_DMA_Cfg3(0,0);//dstJumpInc,dstInterInc
        BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
        BB_OPCLEAR(BB_CLEAR_DMA);
        BB_OPGATE_EN(BB_GATE_DMA);
        BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
        BB_OPTRIG(BB_TRIG_DMA);
        BB_waitISR(BB_ISR_DMA_JUMP_END);
        BB_OPGATE_DIS(BB_GATE_DMA);
    }
}

void BB_CpxFlt2pf_long(uint32_t *In,uint32_t *out,uint32_t intraCnt,uint32_t interCnt){
    BB_DMA_Cfg0(DMA_CFG0_TWICEMODE_NORMAL,DMA_CFG0_MINMAX_MODE_ABS,DMA_CFG0_FLT2PF_LSF_EN_RIGHT,0,0,interCnt-1,intraCnt-1,DMA_CFG0_WMODE_F2PF,DMA_CFG0_DSIZE_64BIT);
    BB_DMA_Cfg1(0,intraCnt*NUM_BYTE64);//srcJumpInc,srcInterInc
    BB_DMA_Cfg2(NUM_BYTE64,&In[0]);//srcIntraInc,srcBaseAddr
    BB_DMA_Cfg3(0,intraCnt*NUM_BYTE32);//dstJumpInc,dstInterInc
    BB_DMA_Cfg4(NUM_BYTE32,&out[0]); //dstIntraInc,dstBaseAddr
    BB_OPCLEAR(BB_CLEAR_DMA);
    BB_OPGATE_EN(BB_GATE_DMA);
    BB_ISR_CLEAR(BB_ISR_DMA_JUMP_END);
    BB_OPTRIG(BB_TRIG_DMA);
    BB_waitISR(BB_ISR_DMA_JUMP_END);
    BB_OPGATE_DIS(BB_GATE_DMA);
}

uint16_t flt_max_search(uint16_t len,float *din){
    uint16_t maxIdx = 0;
    float maxVal=din[0];
    for (int i=1;i<len;i++){
        if(din[i]>maxVal){
            maxIdx=i;
            maxVal=din[i];
        }
    }
    return maxIdx;
}

uint8_t fftPTcalc(uint16_t useRange){
	uint8_t rtn;
	switch(useRange){
		case 16:
			rtn=FFTPT_16;
			break;
		case 32:
			rtn=FFTPT_32;
			break;
		case 64:
			rtn=FFTPT_64;
			break;
		case 128:
			rtn=FFTPT_128;
			break;
		case 256:
			rtn=FFTPT_256;
			break;
		case 512:
			rtn=FFTPT_512;
			break;
		case 1024:
			rtn=FFTPT_1024;
			break;
		case 2048:
			rtn=FFTPT_2048;
			break;
		case 4096:
			rtn=FFTPT_4096;
			break;
	}
	return rtn;
}

uint8_t fft1dMeanDivCalc(uint16_t numChirp){
	uint8_t rtn;
	switch(numChirp){
		case 1:
			rtn=P2_CFG0_DIV1;
			break;
		case 2:
			rtn=P2_CFG0_DIV2;
			break;
		case 4:
			rtn=P2_CFG0_DIV4;
			break;
		case 16:
			rtn=P2_CFG0_DIV16;
			break;
		case 32:
			rtn=P2_CFG0_DIV32;
			break;
		case 64:
			rtn=P2_CFG0_DIV64;
			break;
		case 128:
			rtn=P2_CFG0_DIV128;
			break;
	}
	return rtn;
}
uint8_t log2Data(uint16_t dataIn){
	uint8_t rtn;
	switch(dataIn){
		case 2:
			rtn=P2_CFG0_DIV2;
			break;
		case 3:
		case 4:
			rtn=P2_CFG0_DIV4;
			break;
		case 5:
		case 6:
		case 7:
		case 8:
			rtn=P2_CFG0_DIV8;
			break;
		case 16:
			rtn=P2_CFG0_DIV16;
			break;
		case 32:
			rtn=P2_CFG0_DIV32;
			break;
		case 64:
			rtn=P2_CFG0_DIV64;
			break;
		case 128:
			rtn=P2_CFG0_DIV128;
			break;
		default:
			rtn=0;
	}
	return rtn;	
}






