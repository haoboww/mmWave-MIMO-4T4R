#include "hal_DMA.h"
#include "main.h"
dma_channel_config_t dmaConfigStr;

void DMA_Config_Init(void){
	  DMA_CLK_ENABLE;
		DMA_Disable();
    DMA_Clear_All_Irq();
    DMA_Init();
		NVIC_SetPriority (DMA_IRQn, DMA_PRIORITY_LV);	
		NVIC_EnableIRQ(DMA_IRQn);	
}

//2048Byte 的整数倍
void DMA_Start_TransferM2M_32(uint32_t *srcAddr,uint32_t *dstAddr,uint32_t Len,enum dma_channel_number channel){
		uint32_t sTimes=Len/2048;
		uint32_t sRemain=Len%2048;
    DMA_CLK_ENABLE;

    dmaConfigStr.sar = (uint32_t)&srcAddr[0];
    dmaConfigStr.dar = (uint32_t)&dstAddr[0];
    dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_sinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_dst_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_src_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_tt_fc = DMA_MEM2MEM_DMA;
    dmaConfigStr.ctl_block_ts = 512;		
    DMA_Disable();
    DMA_Channel_Configure(channel, &dmaConfigStr, NULL);
		DMA_Clear_Irq(channel, DMA_IRQ_ALL);	
    DMA_Enable();
		for(uint32_t i=0;i<sTimes;i++){
			DMA_Channel_Enable(channel);
			while((DMA_INT->RawBlock&(0x01<<channel))!= (0x01<<channel)){}
			DMA_INT->ClearBlock  = (0x01<<channel);		
		}
		if(sRemain){
			DMA_CH_T *dma_ch = DMA_Num2Ch(channel);
			DMA_Disable();
			dma_ch->HCTL = (sRemain << DMA_HCTL_BLOCK_TS_Pos) & DMA_HCTL_BLOCK_TS_Msk;
			DMA_Enable();			
			DMA_Channel_Enable(channel);
			while((DMA_INT->RawBlock&(0x01<<channel))!= (0x01<<channel)){}
			DMA_INT->ClearBlock  = (0x01<<channel);					
		}
}
DMA_CH_T *g_dma_ch;
DMA_CH_T *g_dma_ch0=DMA_CH0;
DMA_CH_T *g_dma_ch1=DMA_CH1;
DMA_CH_T *g_dma_ch2=DMA_CH2;
DMA_CH_T *g_dma_ch3=DMA_CH3;
DMA_CH_T *g_dma_ch4=DMA_CH4;
DMA_CH_T *g_dma_ch5=DMA_CH5;
DMA_CH_T *g_dma_ch6=DMA_CH6;
DMA_CH_T *g_dma_ch7=DMA_CH7;
void DMA_Start_TransferM2M_SRAM2K(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr,uint32_t Len,enum dma_channel_number channel){
		uint32_t sBlockMod=(Len/4)%512; //yushu
		uint32_t sTimes=Len/2048;
		uint32_t sAddrOffset=0;
    DMA_CLK_ENABLE;
		SRAM3_CLK_ENABLE;
//    dmaConfigStr.sar = srcAddr;
//    dmaConfigStr.dar = dstAddr;
    dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_32;
    dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_sinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_dst_msize = DMA_MSIZE_256;
    dmaConfigStr.ctl_src_msize = DMA_MSIZE_256;
		dmaConfigStr.ctl_sms             = DMA_MASTER1;
		dmaConfigStr.ctl_dms             = DMA_MASTER1;
    dmaConfigStr.ctl_tt_fc = DMA_MEM2MEM_DMA;
//		    dmaConfigStr.ctl_block_ts = Len/4;  //128 max
		dmaConfigStr.cfg_hs_sel_src      = DMA_HS_HARDWARE;
		dmaConfigStr.cfg_hs_sel_dst      = DMA_HS_HARDWARE;
		dmaConfigStr.ctl_int_en          = 0;
		
		
		DMA_Channel_Configure(channel, &dmaConfigStr, NULL);
		DMA_Clear_Irq(channel, DMA_IRQ_ALL);
//		DMA_Mask_Irq(channel, DMA_IRQ_TFR);
//		DMA_Unmask_Irq(channel, DMA_IRQ_TFR);
		g_dma_ch0->HCTL=512;
		for(uint32_t timNow=0;timNow<sTimes;timNow++){
			DMA_Disable();
			g_dma_ch0->SAR=srcAddr+sAddrOffset;
			g_dma_ch0->DAR=midAddr;
			DMA_Enable();
			DMA_Channel_Enable(channel);
			BB_timeoutCnt=0;
			while(((DMA_INT->RawTfr) & (1<<channel))==0){
				if(BB_timeoutCnt>1000){
					// 处理错误或超时
					printf("DMA_ERR:timeout=%08X\n",BB_isrSet);
					CHPIEN_WritePin=0;
					WDT_RstNow();
					while(1);			
				}			
			}
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);
			g_dma_ch0->SAR=midAddr;
			g_dma_ch0->DAR=dstAddr+sAddrOffset;
			DMA_Enable();
			DMA_Channel_Enable(channel);
			BB_timeoutCnt=0;
			while(((DMA_INT->RawTfr) & (1<<channel))==0){
				if(BB_timeoutCnt>1000){
					// 处理错误或超时
					printf("DMA_ERR:timeout=%08X\n",BB_isrSet);
					CHPIEN_WritePin=0;
					WDT_RstNow();
					while(1);			
				}			
			}
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);
			sAddrOffset=sAddrOffset+2048;			
		}		
		if(sBlockMod){
			DMA_Disable();
			g_dma_ch0->SAR=srcAddr+sAddrOffset;
			g_dma_ch0->DAR=dstAddr+sAddrOffset;
			g_dma_ch0->HCTL=sBlockMod;
			DMA_Enable();
			DMA_Channel_Enable(channel);	
			BB_timeoutCnt=0;
			while(((DMA_INT->RawTfr) & (1<<channel))==0){
				if(BB_timeoutCnt>1000){
					// 处理错误或超时
					printf("DMA_ERR:timeout=%08X\n",BB_isrSet);
					CHPIEN_WritePin=0;
					WDT_RstNow();
					while(1);			
				}			
			}
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);			
		}
}

#if 1
void DMA_Start_TransferM2M_SRAM_Step1(void){
    DMA_CLK_ENABLE;
		SRAM3_CLK_ENABLE;
		DMA_Disable();
//    dmaConfigStr.sar = srcAddr;
//    dmaConfigStr.dar = dstAddr;
    dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_64;
    dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_64;
    dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_sinc =  DMA_ADDR_INCREMENT;
    dmaConfigStr.ctl_dst_msize = DMA_MSIZE_1;
    dmaConfigStr.ctl_src_msize = DMA_MSIZE_1;
		dmaConfigStr.ctl_sms             = DMA_MASTER1;
		dmaConfigStr.ctl_dms             = DMA_MASTER1;
    dmaConfigStr.ctl_tt_fc = DMA_MEM2MEM_DMA;
		dmaConfigStr.ctl_block_ts = 256;  //128 max
		dmaConfigStr.cfg_hs_sel_src      = DMA_HS_HARDWARE;
		dmaConfigStr.cfg_hs_sel_dst      = DMA_HS_HARDWARE;
		dmaConfigStr.ctl_int_en          = 0;
		DMA_Channel_Configure(DMA_CHANNEL0, &dmaConfigStr, NULL);
		DMA_Clear_Irq(DMA_CHANNEL0, DMA_IRQ_ALL);		
		DMA_Channel_Configure(DMA_CHANNEL1, &dmaConfigStr, NULL);
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_ALL);		
}
void DMA_Start_TransferM2M_SRAM2K_Step2(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr){
		DMA_Disable();
		g_dma_ch0->SAR=srcAddr;
		g_dma_ch0->DAR=midAddr;
		g_dma_ch1->SAR=midAddr;		
		g_dma_ch1->DAR=dstAddr;
		DMA_Enable();
		for (enum dma_channel_number channel=DMA_CHANNEL0;channel<DMA_CHANNEL2;channel++){
			DMA_Channel_Enable(channel);
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);		
		}		
		DMA_Disable();
}
void DMA_Start_TransferM2M_SRAM4K_Step2(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr){
		DMA_Disable();
		g_dma_ch0->SAR=srcAddr;
		g_dma_ch0->DAR=midAddr;
		g_dma_ch1->SAR=midAddr;		
		g_dma_ch1->DAR=dstAddr;
		DMA_Enable();
		for (enum dma_channel_number channel=DMA_CHANNEL0;channel<DMA_CHANNEL2;channel++){
			DMA_Channel_Enable(channel);
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);		
		}		
		DMA_Disable();
		g_dma_ch0->DAR=midAddr;
		g_dma_ch1->SAR=midAddr;		
		DMA_Enable();
		for (enum dma_channel_number channel=DMA_CHANNEL0;channel<DMA_CHANNEL2;channel++){
			DMA_Channel_Enable(channel);
			while(((DMA_INT->RawTfr) & (1<<channel))==0);
			DMA_Clear_Irq(channel, DMA_IRQ_TFR);		
		}				
}
//2K one times
void DMA_Start_TransferM2M_Flash2K_Step2(uint32_t srcAddr,uint32_t dstAddr,uint32_t times){
		DMA_Disable();
		g_dma_ch0->SAR=srcAddr;
		g_dma_ch0->DAR=dstAddr;
		DMA_Enable();
		DMA_Clear_Irq(DMA_CHANNEL0, DMA_IRQ_TFR);
		for(uint32_t i=0;i<times;i++){
			DMA_Channel_Enable(DMA_CHANNEL0);
			while(((DMA_INT->RawTfr) & (1<<DMA_CHANNEL0))==0);
			DMA_Clear_Irq(DMA_CHANNEL0, DMA_IRQ_TFR);		
		}
		DMA_Disable();
}
#endif
