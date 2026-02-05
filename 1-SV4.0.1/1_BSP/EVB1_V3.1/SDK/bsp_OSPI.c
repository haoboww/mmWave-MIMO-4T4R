#include "bsp_OSPI.h"
#include "bsp_GPIO.h"
uint8_t ospi_sendover_flag = 0;
uint8_t ospi_receiveover_flag = 0;
void (*dma_callback[DMA_CHANNEL_MAX])(void);
volatile uint8_t irq_dma_flag1  = 0;
volatile uint8_t irq_dma_flag2  = 0;
void ospi0_dma_callback(void){
//	ospi_sendover_flag = 1;
	 __NOP();
}
void ospi1_dma_callback(void){
//	ospi_receiveover_flag = 1;
	__NOP();
}
void test_dma_callback2(void)
{
    __NOP();
}
volatile uint32_t int_sta = 0;
volatile uint8_t ospi0_flag = 0;
volatile uint8_t ospi1_flag = 0;
//Master
void OSPI0_IRQHandler (void)
{
    int_sta = OSPI0->INT_STA;
    OSPI0->INT_STA = int_sta;
	  if((int_sta&0x01) == 0x01)
        ospi_sendover_flag = 1;
}
//Slave
void OSPI1_IRQHandler (void)
{
    int_sta = OSPI1->INT_STA;
    OSPI1->INT_STA = int_sta;
	  if((int_sta&0x02) == 0x02)
        ospi_receiveover_flag = 1;
}

void test_dma_ch1_callback(void)
{
	irq_dma_flag1 = 1;
	if (dma_irq_sta.err_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_ERR);
		//printf("1.err_flag is 0x%08x\n", dma_irq_sta.err_flag);
		dma_irq_sta.err_flag &= ~BIT1;
	}
	if (dma_irq_sta.block_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_BLOCK);
		//printf("1.block_flag is 0x%08x\n", dma_irq_sta.block_flag);
		dma_irq_sta.block_flag &= ~BIT1;
	}
	if (dma_irq_sta.dstTran_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_DSTTRAN);
		//printf("1.dstTran_flag is 0x%08x\n", dma_irq_sta.dstTran_flag);
		dma_irq_sta.dstTran_flag &= ~BIT1;
	}
	if (dma_irq_sta.srcTran_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_SRCTRAN);
		//printf("1.srcTran_flag is 0x%08x\n", dma_irq_sta.srcTran_flag);
		dma_irq_sta.srcTran_flag &= ~BIT1;
	}
	if (dma_irq_sta.tfr_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL1, DMA_IRQ_TFR);
		//printf("1.tfr_flag is 0x%08x\n", dma_irq_sta.tfr_flag);
		dma_irq_sta.tfr_flag &= ~BIT1;
	}
}

void test_dma_ch2_callback(void)
{
	
	irq_dma_flag2 = 1;
	if (dma_irq_sta.err_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_ERR);
		//printf("2.err_flag is 0x%08x\n", dma_irq_sta.err_flag);
		dma_irq_sta.err_flag &= ~BIT2;
	}
	if (dma_irq_sta.block_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_BLOCK);
		//printf("2.block_flag is 0x%08x\n", dma_irq_sta.block_flag);
		dma_irq_sta.block_flag &= ~BIT2;
	}
	if (dma_irq_sta.dstTran_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_DSTTRAN);
		//printf("2.dstTran_flag is 0x%08x\n", dma_irq_sta.dstTran_flag);
		dma_irq_sta.dstTran_flag &= ~BIT2;
	}
	if (dma_irq_sta.srcTran_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_SRCTRAN);
		//printf("2.srcTran_flag is 0x%08x\n", dma_irq_sta.srcTran_flag);
		dma_irq_sta.srcTran_flag &= ~BIT2;
	}
	if (dma_irq_sta.tfr_flag)
	{
		DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_TFR);
		//printf("2.tfr_flag is 0x%08x\n", dma_irq_sta.tfr_flag);
		dma_irq_sta.tfr_flag &= ~BIT2;
	}
	
		Cnt_Delay_us(10);
//		OSPI_CS_HIGH(OSPI0);
//    OSPI0_CS_WritePin=1;	
}

/*
说明：
    个人理解：
    OSPI有个FIFO，这个FIFO有8*64bit，还有个水线request，当DMA在m2f方式传输数据到FIFO时，要先有个request才能传输，这就产生了先有鸡还是先有蛋的问题
于是乎就要先给FIFO一个request，DMA才能以m2f方式传输，而给request有2种方法，一种是通过CPU写满8个数到FIFO，一种是DMA再开启一个通道，写8个数进FIFO
由于写数据进FIFO，FIFO会自动把数据发出去，所以要快速写入8个数进去，末过水线（4个数），当数据发送完之前，数据从8->0的过程中，经过水线4,才能起request，而request只有1拍就消失
所以要提前将DMA使能，等待request到来，才能正常工作。
		这里我们使用DMAchannel2作为填入8个数的方式，以m2m方式强行写入FIFO，以DMAchannel0的m2f方式作为正常写入方式，提前配置好所有的参数，可以同时开启这两个DMA，此时
DMAchannel2先工作，等写满8个后DMAchannel2停止工作，DMA1等到来request才正常工作。在DMA工作的过程中，如果OSPI速率最大，需要CPU进入idle状态，这样才能不会和DMA抢占总线，保证速率
DMA的src地址是可以自动增加的，如果不考虑OSPI最大速度，可以不用每次配置DMA地址，只开启DMA使能即可。

*/
void OSPI_MasterInit(void){

	enum dma_channel_number ch_num_1;
	enum dma_channel_number ch_num_2;
	dma_channel_config_t dma_config_1;
	dma_channel_config_t dma_config_2;
	
	ospi_config_t config;

	OSPI0_CLK_ENABLE;

	config.cmd_on = 0;
	config.cpha = 0;
	config.div = 0x4;
	config.dma_en = 1;
	config.dummy_byte = 0;
	config.dummy_len = 0;
	config.fifo_en = 1;
	config.fifo_int_en = 0;
	config.fifo_wl = 4;
	config.nend_int_en = 0;
	config.read_cmd = 0;
	config.send_int_en = 0;
	config.slave_en = 0;
	config.trans_mode = OSPI_O_OCTU;
	config.tshsl = 0;
	config.xip_en = 0;

  OSPI_Init(OSPI0, config);
//	OSPI_CS_HIGH(OSPI0);
	
//	PinMux_Set(GPIO0, 0, IOMODE_OSPI0_SS);
	PinMux_Set(GPIO0, 1, IOMODE_OSPI0_CLK);
	PinMux_Set(GPIO0, 2, IOMODE_OSPI0_DIO0);
	PinMux_Set(GPIO0, 3, IOMODE_OSPI0_DIO1);
	PinMux_Set(GPIO0, 4, IOMODE_OSPI0_DIO2);
	PinMux_Set(GPIO0, 5, IOMODE_OSPI0_DIO3);
	PinMux_Set(GPIO0, 6, IOMODE_OSPI0_DIO4);
	PinMux_Set(GPIO0, 7, IOMODE_OSPI0_DIO5);
	PinMux_Set(GPIO0, 8, IOMODE_OSPI0_DIO6);
	PinMux_Set(GPIO0, 9, IOMODE_OSPI0_DIO7);

/*
	OSPI0->CON0         =   (4<<12) |   // fifo_wl
													(0<<9)  |   // slv_en
													(0<<8)  |   // dma_en
													(0xf<<4)|   // mode
													(1<<3)  |   // samp_edge
													(0<<2)  |   // fifo_irq_en
													(0<<1)  |   // slv_end_irq_en
													(0<<0);     // master_tx_irq_en
	OSPI0->CON1     =   (0<<24) |   // read_cmd
													(0<<20) |   // tshsl
													(0<<16) |   // dummy_len
													(0<<8)  |   // dummy_byte
													(0x1<<4)|   // clk_div
													(0<<2)  |   // ins_on
													(1<<1)  |   // master_normal_fifo
													(0<<0);     // xip_mode_en
	OSPI0->CS = 0x01;
	OSPI0->INT_STA = 0x07;
//	OSPI0->CON0 &= (~OSPI_CON0_TRANS_MODE_Msk);
//	OSPI0->CON0 |= (OSPI_O_OCTU<<OSPI_CON0_TRANS_MODE_Pos);
*/
//	NVIC_EnableIRQ(OSPI0_IRQn);
    OSPI_Clear_Irq(OSPI0, OSPI_ALL_IRQ);
	
	
	DMA_CLK_ENABLE;
	DMA_Init();
	DMA_Disable();
//config channel 1
		ch_num_1 = DMA_CHANNEL1;
		dma_config_1.sar = (uint32_t)0x0;
		dma_config_1.dar = (uint32_t)&XOSPI0->DATA64[0];

		dma_config_1.ctl_src_tr_width = DMA_TRANS_WIDTH_64;
		dma_config_1.ctl_dst_tr_width = DMA_TRANS_WIDTH_64;

		dma_config_1.ctl_sinc = DMA_ADDR_INCREMENT;
		dma_config_1.ctl_dinc = DMA_ADDR_NOCHANGE;

		dma_config_1.ctl_dst_msize = DMA_MSIZE_1;
		dma_config_1.ctl_src_msize = DMA_MSIZE_1;

		dma_config_1.ctl_block_ts = 8;

		dma_config_1.ctl_tt_fc = DMA_MEM2MEM_DMA;

		dma_config_1.ctl_sms = DMA_MASTER1;
		dma_config_1.ctl_dms = DMA_MASTER1;

		dma_config_1.cfg_src_per = DMA_OSPI0;

		dma_config_1.cfg_hs_sel_src = DMA_HS_HARDWARE;
		dma_config_1.cfg_hs_sel_dst = DMA_HS_HARDWARE;

		dma_config_1.ctl_int_en = 0;

		DMA_Channel_Configure(ch_num_1, &dma_config_1, test_dma_ch1_callback);
		dma_callback[ch_num_1] = test_dma_ch1_callback;
		DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
		DMA_Mask_Irq(ch_num_1, DMA_IRQ_TFR);

//config channel 2

		ch_num_2 = DMA_CHANNEL2;
		dma_config_2.sar = (uint32_t)(0x0) + 64;
		dma_config_2.dar = (uint32_t)&XOSPI0->DATA64[0];

		dma_config_2.ctl_src_tr_width = DMA_TRANS_WIDTH_64;
		dma_config_2.ctl_dst_tr_width = DMA_TRANS_WIDTH_64;

		dma_config_2.ctl_sinc = DMA_ADDR_INCREMENT;
		dma_config_2.ctl_dinc = DMA_ADDR_NOCHANGE;

		dma_config_2.ctl_dst_msize = DMA_MSIZE_4;//DMA_MSIZE_1;
		dma_config_2.ctl_src_msize = DMA_MSIZE_1;

		dma_config_2.ctl_block_ts = 0x0;

		dma_config_2.ctl_tt_fc = DMA_MEM2PRF_DMA;

		dma_config_2.ctl_sms = DMA_MASTER1;
		dma_config_2.ctl_dms = DMA_MASTER1;

		dma_config_2.cfg_src_per = DMA_OSPI0;
		dma_config_2.cfg_dst_per = DMA_OSPI0;

		dma_config_2.cfg_hs_sel_src = DMA_HS_HARDWARE;
		dma_config_2.cfg_hs_sel_dst = DMA_HS_HARDWARE;

		dma_config_2.ctl_int_en = 1;

		DMA_Channel_Configure(ch_num_2, &dma_config_2, test_dma_ch2_callback);
		dma_callback[ch_num_2] = test_dma_ch2_callback;
		DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
		DMA_Mask_Irq(ch_num_2, DMA_IRQ_TFR);

		OSPI_Clear_Irq(OSPI0, OSPI_ALL_IRQ);
//		NVIC_EnableIRQ(OSPI0_IRQn);
		DMA_Enable();
		NVIC_EnableIRQ(DMA_IRQn);

}

void OSPI_SlaveInit(void){

	OSPI1_CLK_ENABLE;
	dma_channel_config_t dmaConfigStr={0};
	PinMux_Set(GPIO0, 10, IOMODE_OSPI1_SS);
	PinMux_Set(GPIO0, 11, IOMODE_OSPI1_CLK);
	PinMux_Set(GPIO0, 12, IOMODE_OSPI1_DIO0);
	PinMux_Set(GPIO0, 13, IOMODE_OSPI1_DIO1);
	PinMux_Set(GPIO0, 14, IOMODE_OSPI1_DIO2);
	PinMux_Set(GPIO0, 15, IOMODE_OSPI1_DIO3);
	PinMux_Set(GPIO1, 0, IOMODE_OSPI1_DIO4);
	PinMux_Set(GPIO1, 1, IOMODE_OSPI1_DIO5);
	PinMux_Set(GPIO1, 2, IOMODE_OSPI1_DIO6);
	PinMux_Set(GPIO1, 3, IOMODE_OSPI1_DIO7);
	OSPI1->CON0 = 0x00004302;
	OSPI1->CON1 =   0xEB06FF00;
	OSPI1->SDH = 0;
	OSPI1->SDL = 0;
	OSPI1->CON0 &= (~OSPI_CON0_TRANS_MODE_Msk);
	OSPI1->CON0 |= (OSPI_I_OCTU<<OSPI_CON0_TRANS_MODE_Pos);
	OSPI1->CS = 0x01;
//	NVIC_EnableIRQ(OSPI1_IRQn);
	OSPI1->INT_STA = 0x07;
	//config channel 1
	dmaConfigStr.sar = (uint32_t)&XOSPI1->DATA64[0];
	dmaConfigStr.dar = 	0;
	dmaConfigStr.ctl_src_tr_width = DMA_TRANS_WIDTH_64;
	dmaConfigStr.ctl_dst_tr_width = DMA_TRANS_WIDTH_64;
	dmaConfigStr.ctl_dinc =  DMA_ADDR_INCREMENT;
	dmaConfigStr.ctl_sinc =  DMA_ADDR_NOCHANGE;
	dmaConfigStr.ctl_dst_msize = DMA_MSIZE_4;
	dmaConfigStr.ctl_src_msize = DMA_MSIZE_4;
	dmaConfigStr.ctl_block_ts = 0;
	dmaConfigStr.ctl_tt_fc = DMA_PRF2MEM_PRF;
	dmaConfigStr.cfg_src_per = DMA_OSPI1;
	dmaConfigStr.ctl_int_en = 0;
	DMA_Disable();
	DMA_Channel_Configure(DMA_CHANNEL1, &dmaConfigStr, ospi1_dma_callback);
//	dma_Enable();
//	while((DMA_INT->RawBlock&(0x01<<DMA_CHANNEL1))!= (0x01<<DMA_CHANNEL1)){}
//	DMA_INT->ClearBlock  = (0x01<<DMA_CHANNEL1);
}
//len>8*64bit
void OSPI_TransDMA64(uint32_t *srcAddr,uint32_t len){
	DMA_CH0->SAR = (uint32_t)&srcAddr[0]+64;
	DMA_CH0->HCTL = (len-8)& DMA_HCTL_BLOCK_TS_Msk;
	DMA_CH2->SAR = (uint32_t)&srcAddr[0];
	OSPI0->CS = 0x00;
	DMA_Channel_Enable(DMA_CHANNEL0);
	DMA_Channel_Enable(DMA_CHANNEL2);
	while((OSPI0->INT_STA & 0x1)==0);
	OSPI0->INT_STA      = 0x1;
	OSPI0->CS = 0x01;
}
void OSPI_RecDMA64(uint32_t *dstAddr,uint32_t len){
	DMA_CH1->DAR = (uint32_t)&dstAddr[0];
	DMA_CH1->HCTL = len& DMA_HCTL_BLOCK_TS_Msk;
//	OSPI1->CS = 0x00;
	DMA_Channel_Enable(DMA_CHANNEL1);
//	OSPI1->CS = 0x01;
}
/*
len = bytes num
len%8=0
*/

void OSPI_Trans64(uint64_t *srcAddr,uint32_t len){
	uint32_t cnt=0;
	OSPI0->INT_STA=7;
	while(cnt<len){
		OSPI0_CS_WritePin=1;
		XOSPI0->DATA64[0] = 0xFFFFFFFFFFFFFFFFULL;
//		XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;	
		while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0); 
		OSPI0->INT_STA = OSPI_NEND_IRQ_Msk;		
    Cnt_Delay_us(1);		
		OSPI0_CS_WritePin=0;	
		
		for(uint32_t i=0;i<128;i++){
			XOSPI0->DATA64[0] = srcAddr[0];
			XOSPI0->DATA64[1] = srcAddr[1];
			XOSPI0->DATA64[2] = srcAddr[2];
			XOSPI0->DATA64[3] = srcAddr[3];
			XOSPI0->DATA64[4] = srcAddr[4];
			XOSPI0->DATA64[5] = srcAddr[5];
			XOSPI0->DATA64[6] = srcAddr[6];
			XOSPI0->DATA64[7] = srcAddr[7];			
			srcAddr=srcAddr+8;			
			while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0);
			OSPI0->INT_STA      = OSPI_NEND_IRQ_Msk;					
		}	
		Cnt_Delay_us(1);
		OSPI0_CS_WritePin=1;
		XOSPI0->DATA64[0] = 0x9999999999999999ULL;
//		XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;
//    Cnt_Delay_us(100);	
		while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0); 
		OSPI0->INT_STA = OSPI_NEND_IRQ_Msk; 			
		cnt=cnt+4096;
	}
}


void OSPI_DMA_Trans64(uint64_t *srcAddr, uint32_t len)
{
		enum dma_channel_number ch_num_1;
		enum dma_channel_number ch_num_2;
	
//		OSPI_CS_LOW(OSPI0);
	  OSPI0_CS_WritePin=0;	
	
//		XOSPI0->DATA8[0] = 0x00;
//		XOSPI0->DATA8[1] = 0x00;
//		while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE);
//		OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);

		DMA_Disable();
//config channel 1
		ch_num_1 = DMA_CHANNEL1;
	  DMA_CH1->SAR = (uint32_t)srcAddr;
		DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);

//config channel 2
		ch_num_2 = DMA_CHANNEL2;	
		DMA_CH2->SAR = (uint32_t)(srcAddr) + 64;
		DMA_CH2->HCTL = ((len / 8) - 8);
		DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
		
		DMA_Enable();
		DMA_Channel_Enable(ch_num_2);
		DMA_Channel_Enable(ch_num_1);
		
		while (irq_dma_flag1 == 0);
		irq_dma_flag1 = 0;
		while (irq_dma_flag2 == 0);
		irq_dma_flag2 = 0;
		
		Cnt_Delay_us(10);
//		OSPI_CS_HIGH(OSPI0);
		OSPI0_CS_WritePin=1;
}

#if 0
void OSPI_DMA_Trans64_IRQ(uint64_t *srcAddr, uint32_t len)
{
	  uint32_t cnt=0;
		enum dma_channel_number ch_num_1;
		enum dma_channel_number ch_num_2;
	
	
//		OSPI_CS_LOW(OSPI0);
//	  OSPI0_CS_WritePin=0;	
	
//		XOSPI0->DATA8[0] = 0x00;
//		XOSPI0->DATA8[1] = 0x00;
//		while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE);
//		OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);
	  OSPI0->INT_STA=7;
	  OSPI0_CS_WritePin=1;
//	  XOSPI0->DATA64[0] = 0x9999999999999999ULL;
			XOSPI0->DATA64[0] = 0x1111111111111111ULL;
			XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;
			XOSPI0->DATA64[2] = 0x2222222222222222ULL;
			XOSPI0->DATA64[3] = 0xEEEEEEEEEEEEEEEEULL;
			XOSPI0->DATA64[4] = 0x3333333333333333ULL;
			XOSPI0->DATA64[5] = 0xEEEEEEEEEEEEEEEEULL;
			XOSPI0->DATA64[6] = 0x4444444444444444ULL;
			XOSPI0->DATA64[7] = 0xEEEEEEEEEEEEEEEEULL;
			while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE){};
		  OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);
//		Cnt_Delay_us(10);
	  OSPI0_CS_WritePin=0;
	
	while(cnt<len){
				

//	  OSPI0_CS_WritePin=0;
		DMA_Disable();
//config channel 1
		ch_num_1 = DMA_CHANNEL1;
	  DMA_CH1->SAR = (uint32_t)srcAddr + cnt;
		DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);

//config channel 2
		ch_num_2 = DMA_CHANNEL2;	
		DMA_CH2->SAR = (uint32_t)(srcAddr) + cnt + 64;
		DMA_CH2->HCTL = 16384/8 - 8;//((len / 8) - 8);
		DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
		
		
		
		DMA_Enable();
		DMA_Channel_Enable(ch_num_2);
		DMA_Channel_Enable(ch_num_1);
		
//		__WFI;
		
		while(irq_dma_flag2 == 0);
		irq_dma_flag2 = 0;
		
		while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0);		
		OSPI0->INT_STA = OSPI_NEND_IRQ_Msk; 		
		cnt=cnt+16384;
//		OSPI0_CS_WritePin=1;
//		Cnt_Delay_us(10);   	
//	  XOSPI0->DATA64[0] = 0x9999999999999999ULL;


	}
	
		OSPI0_CS_WritePin=1;
//		Cnt_Delay_us(10);   	
//	  XOSPI0->DATA64[0] = 0x9999999999999999ULL;
}
#endif
void OSPI_DMA_Trans64_IRQ(uint64_t *srcAddr, uint32_t len){
    uint32_t cnt=0;
    enum dma_channel_number ch_num_1;
    enum dma_channel_number ch_num_2;

    OSPI0->INT_STA=7;
    OSPI0_CS_WritePin=1;
    XOSPI0->DATA64[0] = 0x1111111111111111ULL;
    XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[2] = 0x2222222222222222ULL;
    XOSPI0->DATA64[3] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[4] = 0x3333333333333333ULL;
    XOSPI0->DATA64[5] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[6] = 0x4444444444444444ULL;
    XOSPI0->DATA64[7] = 0xEEEEEEEEEEEEEEEEULL;
    while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE){};
    OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);

    OSPI0_CS_WritePin=0;
	
    while(cnt<len){			
        DMA_Disable();
//config channel 1
        ch_num_1 = DMA_CHANNEL1;
        DMA_CH1->SAR = (uint32_t)srcAddr + cnt;
        DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
//config channel 2
        ch_num_2 = DMA_CHANNEL2;	
        DMA_CH2->SAR = (uint32_t)(srcAddr) + cnt + 64;
        DMA_CH2->HCTL = 16384/8 - 8;//((len / 8) - 8);
        DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
        
        DMA_Enable();
        DMA_Channel_Enable(ch_num_2);
        DMA_Channel_Enable(ch_num_1);
        
        while(irq_dma_flag2 == 0);
        irq_dma_flag2 = 0;
        
        while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0);		
        OSPI0->INT_STA = OSPI_NEND_IRQ_Msk; 		
        cnt=cnt+16384;
	}	
    OSPI0_CS_WritePin=1;
}
void OSPI_DMA_Trans64_IRQ_8kB(uint64_t *srcAddr, uint32_t len){
    uint32_t cnt=0;
    enum dma_channel_number ch_num_1;
    enum dma_channel_number ch_num_2;

    OSPI0->INT_STA=7;
    OSPI0_CS_WritePin=1;
    XOSPI0->DATA64[0] = 0x1111111111111111ULL;
    XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[2] = 0x2222222222222222ULL;
    XOSPI0->DATA64[3] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[4] = 0x3333333333333333ULL;
    XOSPI0->DATA64[5] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[6] = 0x4444444444444444ULL;
    XOSPI0->DATA64[7] = 0xEEEEEEEEEEEEEEEEULL;
    while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE){};
    OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);

    OSPI0_CS_WritePin=0;
	
    while(cnt<len){			
        DMA_Disable();
//config channel 1
        ch_num_1 = DMA_CHANNEL1;
        DMA_CH1->SAR = (uint32_t)srcAddr + cnt;
        DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
//config channel 2
        ch_num_2 = DMA_CHANNEL2;	
        DMA_CH2->SAR = (uint32_t)(srcAddr) + cnt + 64;
        DMA_CH2->HCTL = 8192/8 - 8;//((len / 8) - 8);
        DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
        
        DMA_Enable();
        DMA_Channel_Enable(ch_num_2);
        DMA_Channel_Enable(ch_num_1);
        
        while(irq_dma_flag2 == 0);
        irq_dma_flag2 = 0;
        
        while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0);		
        OSPI0->INT_STA = OSPI_NEND_IRQ_Msk; 		
        cnt=cnt+8192;
	}	
    OSPI0_CS_WritePin=1;
}
void OSPI_DMA_Trans64_IRQ_16kB(uint64_t *srcAddr, uint32_t len){
    uint32_t cnt=0;
    enum dma_channel_number ch_num_1;
    enum dma_channel_number ch_num_2;
    OSPI0->INT_STA=7;
    OSPI0_CS_WritePin=1;
    XOSPI0->DATA64[0] = 0x1111111111111111ULL;
    XOSPI0->DATA64[1] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[2] = 0x2222222222222222ULL;
    XOSPI0->DATA64[3] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[4] = 0x3333333333333333ULL;
    XOSPI0->DATA64[5] = 0xEEEEEEEEEEEEEEEEULL;
    XOSPI0->DATA64[6] = 0x4444444444444444ULL;
    XOSPI0->DATA64[7] = 0xEEEEEEEEEEEEEEEEULL;
    while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE){};
    OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);
        
    OSPI0_CS_WritePin=0;
    Cnt_Delay_us(10);
	
	while(cnt<len){		
        OSPI0_CS_WritePin=0;		
        DMA_Disable();
    //config channel 1
        ch_num_1 = DMA_CHANNEL1;
        DMA_CH1->SAR = (uint32_t)srcAddr + cnt;
        DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
    //config channel 2
        ch_num_2 = DMA_CHANNEL2;	
        DMA_CH2->SAR = (uint32_t)(srcAddr) + cnt + 64;
        DMA_CH2->HCTL = 16384/8 - 8;//((len / 8) - 8);
        DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
            
        DMA_Enable();
        DMA_Channel_Enable(ch_num_2);
        DMA_Channel_Enable(ch_num_1);
            
        while(irq_dma_flag2 == 0);
        irq_dma_flag2 = 0;
            
        while((OSPI0->INT_STA & OSPI_NEND_IRQ_Msk)==0);		
        OSPI0->INT_STA = OSPI_NEND_IRQ_Msk; 		
        cnt=cnt+16384;
        OSPI0_CS_WritePin=1;
        Cnt_Delay_us(20);   	
        XOSPI0->DATA64[0] = 0x9999999999999999ULL;
        XOSPI0->DATA64[1] = 0x9999999999999999ULL;
        XOSPI0->DATA64[2] = 0x9999999999999999ULL;
        XOSPI0->DATA64[3] = 0x9999999999999999ULL;
        XOSPI0->DATA64[4] = 0x9999999999999999ULL;
        XOSPI0->DATA64[5] = 0x9999999999999999ULL;
        XOSPI0->DATA64[6] = 0x9999999999999999ULL;
        XOSPI0->DATA64[7] = 0x9999999999999999ULL;
        while (OSPI_Irq_IsActive(OSPI0, OSPI_NEND_IRQ) == FALSE){};
        OSPI_Clear_Irq(OSPI0, OSPI_NEND_IRQ);
	}
    OSPI0_CS_WritePin=1;
}





