#include "BB_Config.h"
#include "main.h"
BB_REGB_T *bb_regb_str =(BB_REGB_T  *)BB_REGB;

STRUCT_BB_PREP bb_prep;
STRUCT_BB_CFAR_CFG0 cfarCfg0_stR;
STRUCT_BB_CFAR_CFG0 cfarCfg0_stV; //rangeIdx:0-9
STRUCT_FFT2D_ZP fft2d_zp;
uint64_t prepCfg1_val1;
uint64_t prepCfg1_val2;
volatile uint32_t BB_isrSet=0;
volatile uint32_t BB_isrRead=0;
volatile uint8_t BB_sampOverFlag=0;
volatile uint8_t BB_firstChirp=0;
volatile uint32_t rampCnt=0;

void dataprep_RegSet(RADAR_USER_CFG *usrCfg){
		bb_prep.cfg0.tx_num = 1-1;
    bb_prep.cfg0.rx_num = usrCfg->numRX-1;
		bb_prep.cfg0.chirp_num = usrCfg->numChirp*usrCfg->numTX-1;
    bb_prep.cfg0.samp_pt = BB_sampPT2reg(usrCfg->sampPT);
    bb_prep.cfg0.circ_en = PREP_CFG0_CIRC_DIS;
    bb_prep.cfg0.tim_mode = PREP_CFG0_TIME_MODE_RAMPSYNC ; //PREP_CFG0_TIME_MODE_RAMPSYNC
    bb_prep.cfg0.samp_edge = PREP_CFG0_SAMP_EDGE_N;
    bb_prep.cfg0.cnt_edge = PREP_CFG0_CNT_EDGE_P;
    bb_prep.cfg0.tim_rma = usrCfg->removePT;
    bb_prep.cfg0.cic_sec = usrCfg->cicSec;
    bb_prep.cfg0.down_fac = usrCfg->downFac;
    bb_prep.cfg0.max_removal_dis = PREP_CFG0_MAX_REMOVAL_DIS_EN;
    bb_prep.cfg0.adc_addr_mode = BB_prepmode2reg(usrCfg->sampBit);
    bb_prep.cfg0.adc_dsize = BB_prepBit2reg(usrCfg->sampBit);
    bb_prep.cfg0.adc_clk_mode = PREP_CFG0_ADC_CLK_MODE_N;

    bb_prep.cfg1.tx_ant_mode = PREP_CFG1_TX_ANT_MODE_AUTO;//PREP_CFG1_TX_ANT_MODE_AUTO PREP_CFG1_TX_ANT_MODE_MANUAL
    bb_prep.cfg1.tx_ant_seq = 1; //0
    bb_prep.cfg1.rx_ant_seq7 = 7;
    bb_prep.cfg1.rx_ant_seq6 = 6;
    bb_prep.cfg1.rx_ant_seq5 = 5;
    bb_prep.cfg1.rx_ant_seq4 = 4;
    bb_prep.cfg1.rx_ant_seq3 = 0;
    bb_prep.cfg1.rx_ant_seq2 = 1;
    bb_prep.cfg1.rx_ant_seq1 = 2;
    bb_prep.cfg1.rx_ant_seq0 = 3;

    bb_prep.cfg2.interInc = algCfg.numSamp*algCfg.numByte;
		bb_prep.cfg2.jumpInc = bb_prep.cfg2.interInc*algCfg.numRX;

    #ifdef COMPARE_DATA
            bb_prep.cfg3.prep_baddr = BB_DBUF0_BASE; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
    #else
//				bb_prep.cfg3.prep_baddr = algCfg.mem.adcSamp-usrCfg->numRX*algCfg.numByte*usrCfg->sampPT; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
			bb_prep.cfg3.prep_baddr = algCfg.mem.adcSamp; //ADC_SAMPLE_ADDR BB_DBUF0_BASE
    #endif
		BB_PrepConfig(&bb_prep);		
}

void BB_IRQHandler(void){
	if(bb_regb_str->ISR&BB_ISR_PREP_RAMP_END){
		bb_regb_str->ISR = BB_ISR_PREP_RAMP_END;			//必须放前面	
		FADC_SPI_CLK_DISABLE;	
		if(BB_firstChirp==0){
			BB_OPCLEAR(BB_CLEAR_PREP);	
			BB_firstChirp=1;
		}
		rampCnt++;
	}
	if(bb_regb_str->ISR&BB_ISR_RAMP_PEDGE){
		bb_regb_str->ISR = BB_ISR_RAMP_PEDGE;			//必须放前面
		FADC_SPI_CLK_ENABLE;
	}
	if(bb_regb_str->ISR&BB_isrSet){
		BB_isrRead=bb_regb_str->ISR;
		if(bb_regb_str->ISR&BB_ISR_PREP_FRAME_END){
			BB_sampOverFlag=1;
			RF_waveStop();
		}
		bb_regb_str->ISR=BB_isrSet;
		BB_Task_Trigger();
	}
}
	
void BB_CFAR_Vel_Init(void){
		cfarCfg0_stV.wrapDirA = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0_stV.wrapDirB = CFAR_CFG0_WRAPDIR_INC;
		cfarCfg0_stV.sumMode = CFAR_CFG0_SUMMODE_ABS;
		cfarCfg0_stV.interCnt = algCfg.useRange;
		cfarCfg0_stV.intraCnt = algCfg.numVel;
		cfarCfg0_stV.thresDiv = CFAR_CFG0_THRESDIV8;           // = searchSize num
		cfarCfg0_stV.divFac = CFAR_CFG0_DIVFAC1;
		cfarCfg0_stV.mulFac = stFlashParam.cfarThV;
		cfarCfg0_stV.searchSize = stFlashParam.cfarThVS;
		cfarCfg0_stV.guardSize = stFlashParam.cfarThVG;
		cfarCfg0_stV.totalSize = cfarCfg0_stV.searchSize+cfarCfg0_stV.guardSize;
		cfarCfg0_stV.pdEn = stFlashParam.peakenV; //CFAR_CFG0_PEAKDET_DIS CFAR_CFG0_PEAKDET_EN
		cfarCfg0_stV.cfarMode = stFlashParam.cfarVmode;
		BB_CFAR_CFG0(&cfarCfg0_stV); //STRUCT_CFAR_CFG0
		BB_CFAR_CFG1(algCfg.cfarMaxNum,NUM_BYTE32); //interInc
		BB_CFAR_CFG2(algCfg.useRange32, algCfg.mem.fft2dSumPf);//intraInc,intraAddr
		BB_CFAR_CFG3((algCfg.mem.fft2dSumPf+(algCfg.numVel-cfarCfg0_stV.totalSize)*algCfg.useRange32),algCfg.mem.fft2dSumPf);//wrapAddrA,wrapAddrB
		BB_CFAR_CFG4(algCfg.mem.cfarRsltV); //dstBaseAddr
}

void BB_FFT2D_ZP_Init(void){ //chirp
    uint32_t sCmd=algCfg.numChirp*algCfg.numTX;
    switch(sCmd){
        case 32:
            break;
        case 64:
            for(uint32_t txNow=0;txNow<algCfg.numTX;txNow++){
                fft2d_zp.txNum_zp0[txNow] = 0;
                for(uint32_t chirpNow=0;chirpNow<algCfg.numChirp;chirpNow++){
                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                }
            }
            break;
				case 128:
            for(uint32_t txNow=0;txNow<algCfg.numTX;txNow++){
                fft2d_zp.txNum_zp0[txNow] = 0;
                fft2d_zp.txNum_zp1[txNow] = 0;
                for(uint32_t chirpNow=0;chirpNow<algCfg.numChirpDiv2;chirpNow++){
                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                    fft2d_zp.txNum_zp1[txNow] = fft2d_zp.txNum_zp1[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                }
            }			
						break;						
        case 256:
            for(uint32_t txNow=0;txNow<algCfg.numTX;txNow++){
                fft2d_zp.txNum_zp0[txNow] = 0;
                fft2d_zp.txNum_zp1[txNow] = 0;
                fft2d_zp.txNum_zp2[txNow] = 0;
                fft2d_zp.txNum_zp3[txNow] = 0;
                for(uint32_t chirpNow=0;chirpNow<algCfg.numChirpDiv2;chirpNow++){
                    fft2d_zp.txNum_zp0[txNow] = fft2d_zp.txNum_zp0[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                    fft2d_zp.txNum_zp1[txNow] = fft2d_zp.txNum_zp1[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                    fft2d_zp.txNum_zp2[txNow] = fft2d_zp.txNum_zp2[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                    fft2d_zp.txNum_zp3[txNow] = fft2d_zp.txNum_zp3[txNow]|(uint64_t)(1<<txNow)<<(chirpNow*algCfg.numTX);
                }
            }
            break;
    }
}
