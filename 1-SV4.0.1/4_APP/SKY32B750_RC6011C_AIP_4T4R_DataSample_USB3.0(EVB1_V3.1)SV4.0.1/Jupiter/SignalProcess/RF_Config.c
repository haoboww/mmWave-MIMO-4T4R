#include "RF_Config.h"
#include "Radar_Config.h"
#include "hal_Delay.h"
#include "BB_Config.h"
STRUCT_RF_REG rf_reg;

void RF_CfgInit(RADAR_USER_CFG *usrCfg){
	uint32_t regVal;
	RF_WAVE_CFG waveVal[2];
	memset(&rf_reg,0,sizeof(STRUCT_RF_REG));
	rf_reg.anaCfg0=0x0C40;
	rf_reg.anaCfg1=0xC000;
	rf_reg.anaCfg2=0x0001;  
	rf_reg.anaCfg8=0x0C1F;
	// rf_reg.anaCfg9=0x000B;
	rf_reg.anaCfg9 = (usrCfg->lpf<<4) | (usrCfg->hpf2<<2) | (usrCfg->hpf1);
	rf_reg.anaCfg10 = (usrCfg->rxGain3 << 12) | (usrCfg->rxGain2 << 8) | (usrCfg->rxGain1 << 4) | usrCfg->rxGain0;
	rf_reg.anaCfg11=0x0000;
	rf_reg.anaCfg18=0x4007;
	rf_reg.anaCfg19=0x5138;
	rf_reg.anaCfg20=0x3703;
	rf_reg.anaCfg21=0x0000;
	rf_reg.anaCfg22=0x0000;
	rf_reg.rampSYNCWcfg0=0xFF00;
	rf_reg.rampSYNCWcfg1=0x0000;
	rf_reg.rampACCWcfg0=0x00FF;
	rf_reg.rampACCWcfg1=0x0000;
	rf_reg.rampWMcfg0=0x2000;
	rf_reg.rampWMcfg1=0x03 | ((usrCfg->sweepWaveNum-1) << 8);   //wave number
	regVal=RF_Freq_Start_Calc(radarUserCfg.freqStart);
	rf_reg.rampPLLcfg0=regVal>>16;
	rf_reg.rampPLLcfg1=regVal&0xFFFF;
	waveVal[0] = Hal_RF_WaveGen(usrCfg->Bandwidth, (usrCfg->sweepTimeA+usrCfg->sweepTimeB), 0, usrCfg->Bandwidth, usrCfg->sweepTimeA, 0, usrCfg->Bandwidth, usrCfg->sweepTimeB, 0, 0x5401, 0x0000); //a,b
	waveVal[1] = Hal_RF_WaveGen(usrCfg->Bandwidth, (usrCfg->sweepTimeC), 0, 0, 0, 0, 0, 0, 0, 0x5401, 0x0000); //c
	//wave0第一次是无效波形，wave1对应0x30000000
	//invalid
	rf_reg.wave[0] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].b2+1, 0, 0+1, 0, 0+1, 0x0001, 0x0000);
	rf_reg.wave[1] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].c2, 0, 0, 0, 0, 0x0001, 0x0000);
	rf_reg.wave[2] = Hal_RF_WaveConfig(waveVal[1].a1|0x8000, waveVal[1].a2, 0, 0, 0, 0, 0x0001, 0x0000);	
	//tx2
	rf_reg.wave[3] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].b2+1, 0, 0+1, 0, 0+1, 0x0001, 0x0000);
	rf_reg.wave[4] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].c2, 0, 0, 0, 0, 0x0001, 0x2000);
	rf_reg.wave[5] = Hal_RF_WaveConfig(waveVal[1].a1|0x8000, waveVal[1].a2, 0, 0, 0, 0, 0x0001, 0x0000);	
	//tx1
	rf_reg.wave[6] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].b2+1, 0, 0+1, 0, 0+1, 0x0001, 0x0000);
	rf_reg.wave[7] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].c2, 0, 0, 0, 0, 0x0001, 0x1000);
	rf_reg.wave[8] = Hal_RF_WaveConfig(waveVal[1].a1|0x8000, waveVal[1].a2, 0, 0, 0, 0, 0x0001, 0x0000);				
	//tx3
	rf_reg.wave[9] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].b2+1, 0, 0+1, 0, 0+1, 0x0001, 0x0000);
	rf_reg.wave[10] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].c2, 0, 0, 0, 0, 0x0001, 0x4000);
	rf_reg.wave[11] = Hal_RF_WaveConfig(waveVal[1].a1|0x8000, waveVal[1].a2, 0, 0, 0, 0, 0x0001, 0x0000);						
	//tx4
	rf_reg.wave[12] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].b2+1, 0, 0+1, 0, 0+1, 0x0001, 0x0000);
	rf_reg.wave[13] = Hal_RF_WaveConfig(waveVal[0].a1, waveVal[0].c2, 0, 0, 0, 0, 0x0001, 0x8000);
	rf_reg.wave[14] = Hal_RF_WaveConfig(waveVal[1].a1|0x8000, waveVal[1].a2, 0, 0, 0, 0, 0x0001, 0x0000);				
}

void RF_Reg_Set(void){
	//RF
	#if 0
		while(1){
			uint16_t rtn=RF_ReadRegSPI(CS1_PIN,0);
			printf("RF:rtn=%04X\n",rtn);
			Cnt_Delay_ms(500);
		}
	#endif
		RF_WriteRegSPI(CS1_PIN,ANA_CFG0_ADDR,rf_reg.anaCfg0);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG1_ADDR,rf_reg.anaCfg1);
//		RF_WriteRegSPI(CS1_PIN,ANA_CFG2_ADDR,rf_reg.anaCfg2);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG8_ADDR,rf_reg.anaCfg8);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG9_ADDR,rf_reg.anaCfg9);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG10_ADDR,rf_reg.anaCfg10);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG11_ADDR,rf_reg.anaCfg11);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG18_ADDR,rf_reg.anaCfg18);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG19_ADDR,rf_reg.anaCfg19);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG20_ADDR,rf_reg.anaCfg20);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG21_ADDR,rf_reg.anaCfg21);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG22_ADDR,rf_reg.anaCfg22);
		RF_WriteRegSPI(CS1_PIN,RAMP_SYNCW_CFG0_ADDR,rf_reg.rampSYNCWcfg0); 
		RF_WriteRegSPI(CS1_PIN,RAMP_SYNCW_CFG1_ADDR,rf_reg.rampSYNCWcfg1);
		RF_WriteRegSPI(CS1_PIN,RAMP_ACCW_CFG0_ADDR,rf_reg.rampACCWcfg0);
		RF_WriteRegSPI(CS1_PIN,RAMP_ACCW_CFG1_ADDR,rf_reg.rampACCWcfg1);
		RF_WriteRegSPI(CS1_PIN,RAMP_WM_CFG0_ADDR,rf_reg.rampWMcfg0);
		RF_WriteRegSPI(CS1_PIN,RAMP_WM_CFG1_ADDR,rf_reg.rampWMcfg1); //0:1个  0100: 2个 0200:3个 波形
		RF_WriteRegSPI(CS1_PIN,RAMP_PLL_CFG0_ADDR,rf_reg.rampPLLcfg0);    //初始频率设置   75g
		RF_WriteRegSPI(CS1_PIN,RAMP_PLL_CFG1_ADDR,rf_reg.rampPLLcfg1);
		for(uint32_t i=0;i<radarUserCfg.sweepWaveNum;i++){
			RF_WaveSet(i,rf_reg.wave[i]);
		}
}
void RF_Reg_Reset(void){
    RF_waveStop();
		RF_WriteRegSPI(CS1_PIN,ANA_CFG0_ADDR,0);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG1_ADDR,0);
//		RF_WriteRegSPI(CS1_PIN,ANA_CFG2_ADDR,0);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG8_ADDR,0);//RX baseband channel
		RF_WriteRegSPI(CS1_PIN,ANA_CFG9_ADDR,0);//047F:1.5 50 200
		RF_WriteRegSPI(CS1_PIN,ANA_CFG10_ADDR,0); //0x1330
		RF_WriteRegSPI(CS1_PIN,ANA_CFG11_ADDR,0); //0x1330
		RF_WriteRegSPI(CS1_PIN,ANA_CFG18_ADDR,0);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG19_ADDR,0);
		RF_WriteRegSPI(CS1_PIN,ANA_CFG20_ADDR,0);
    RF_WriteRegSPI(CS1_PIN,ANA_CFG21_ADDR,0); //93F4
		RF_WriteRegSPI(CS1_PIN,ANA_CFG22_ADDR,0);
}

void RF_Dataprep_Switch(uint16_t rampCnt){

	#if 1
    switch (rampCnt%5){
    case 1:
        BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
                             ((uint64_t)(algCfg.numSampByte*1)  << BB_BIT32_POS);   // jumpInc
        break;                           
    case 2:                              
        BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
                             ((uint64_t)(algCfg.numSampByte*4)  << BB_BIT32_POS);   // jumpInc
        break;                           
    case 3:                              
        BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
                             ((uint64_t)(algCfg.numSampByte*4)  << BB_BIT32_POS);   // jumpInc
        break;                           
    case 4:                              
        BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
                             ((uint64_t)(algCfg.numSampByte*4)  << BB_BIT32_POS);   // jumpInc
        break;                           
    case 0:                              
        BB_REGB->PREP_CFG2 = ((uint64_t)(algCfg.numSampByte)    << BB_BIT0_POS) |   // interInc
                             ((uint64_t)(algCfg.numSampByte*4)  << BB_BIT32_POS);   // jumpInc
        break;                           
    }
		#endif
}
