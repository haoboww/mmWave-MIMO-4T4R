#include "hal_OSPI.h"
#include "dma.h"
#include "ospi.h"
#include "bsp_GPIO.h"
#include "hal_Delay.h"

void (*dma_callback[DMA_CHANNEL_MAX])(void);
volatile uint8_t irq_dma_flag1  = 0;
volatile uint8_t irq_dma_flag2  = 0;
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
	DMA配置举例：(20240328)
	OSPI：要配2次DMA，一次是mem2mem,一次是mem2per,其中配置mem2mem时，需要CPU写入8个64bit才能启动cfg_src_per对应DMA_OSPI0的request，此时DMA才能继续工作，同时配置第二次DMA，来继续
	OSPI数据发送，这样就会一直有request，重点：第一次配置DMA，cfg_src_per=DMA_OSPI0，cfg_hs_sel_src = DMA_HS_HARDWARE，dst任意，第二次配置DMA,hs_sel的src和dst都配hardware，对应per也要配DMA_OSPI0
	USCI:配置1次DMA，原因是USCI会自动启动request一次，此时如果是mem2per，那么cfg_hs_sel_src配置成DMA_HS_SOFTWARE，cfg_src_per不用配，cfg_hs_sel_dst配DMA_HS_HARDWARE，cfg_dst_per配DMA_USCIX即可
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
	config.div = 0;
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
				while (!(DMA_INT->StatusTfr) & (1 << DMA_CHANNEL2)) {
				//轮询寄存器等待DMA2完成传输
				}
				DMA_Clear_Irq(DMA_CHANNEL2, DMA_IRQ_TFR);
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



/**
 * @Breif  : DMA_IRQHandler : DMA IRQ int function
 *
 * @Returns:
 */
void DMA_IRQHandler(void)
{
    enum dma_channel_number i;
    uint32_t irq_sta = 0;
    dma_irq_sta.err_flag |= DMA_INT->StatusErr;
    dma_irq_sta.block_flag |= DMA_INT->StatusBlock;
    dma_irq_sta.dstTran_flag |= DMA_INT->StatusDstTran;
    dma_irq_sta.srcTran_flag |= DMA_INT->StatusSrcTran;
    dma_irq_sta.tfr_flag |= DMA_INT->StatusTfr;
    dma_irq_sta.stat_flag |= DMA_INT->StatusInt;
    irq_sta = (DMA_INT->StatusErr | DMA_INT->StatusBlock | DMA_INT->StatusDstTran | \
                DMA_INT->StatusSrcTran | DMA_INT->StatusTfr);
    for (i = DMA_CHANNEL0; i < DMA_CHANNEL_MAX; i++)
    {
        if (irq_sta & (1 << i))
        {
            if(dma_callback[i] != NULL)
            {
                dma_callback[i]();
                break;
            }
            else
            {
                if((DMA_INT->StatusErr) & (1 << i)) DMA_Clear_Irq(i, DMA_IRQ_ERR);
                if((DMA_INT->StatusBlock) & (1 << i)) DMA_Clear_Irq(i, DMA_IRQ_BLOCK);
                if((DMA_INT->StatusDstTran) & (1 << i)) DMA_Clear_Irq(i, DMA_IRQ_DSTTRAN);
                if((DMA_INT->StatusSrcTran) & (1 << i)) DMA_Clear_Irq(i, DMA_IRQ_SRCTRAN);
                if((DMA_INT->StatusTfr) & (1 << i)) DMA_Clear_Irq(i, DMA_IRQ_TFR);
            }
        }
    }
}










