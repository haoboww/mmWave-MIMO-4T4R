#include "hal_USCI_DMA.h"

uint8_t test_dma_src_buf[TEST_DMA_DATA_LENTH];
uint8_t test_dma_dst_buf[TEST_DMA_DATA_LENTH];
volatile uint32_t recCnt = 0;
volatile uint32_t lastCnt = 0;
volatile uint8_t recEnd = 0;
volatile uint8_t dma_flag = 0;

/**
 * @brief    USCI_UART发数据
 */
void Usci_Uart_DMA_Transmit(void)
{
	uint32_t i;
	enum dma_channel_number ch_num_1;
	dma_channel_config_t dma_config_1;
	
	/*准备传输的数据*/
	for(i=0; i< 16; i++)
	{
			 test_dma_src_buf[i] = i*3;
	}
	
	/*DMA初始化*/
	USCI4->CFG0 &= ~USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_UART TX Disable
	DMA_CLK_ENABLE;
	DMA_Disable();
	DMA_Clear_All_Irq();
	DMA_Init();
	
	/*配置DMA通道1*/
	ch_num_1 = DMA_CHANNEL1;
	dma_config_1.sar = (uint32_t)&test_dma_src_buf[0]; 	//源地址
	dma_config_1.dar = (uint32_t)&USCI4->TRX;						//目的地址
	dma_config_1.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_1.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_1.ctl_sinc = DMA_ADDR_INCREMENT;					//源地址递增
	dma_config_1.ctl_dinc = DMA_ADDR_NOCHANGE;					//目的地址不变
	dma_config_1.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_1.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_1.ctl_block_ts = 16;											//单次传输数据量
	dma_config_1.ctl_tt_fc = DMA_MEM2PRF_DMA;						//传输方向---内存到外设
	dma_config_1.ctl_sms = DMA_MASTER1;									//源地址为内存,属于层级1
	dma_config_1.ctl_dms = DMA_MASTER2;									//目的地址为USCI,属于层级2
	dma_config_1.cfg_hs_sel_dst = DMA_HS_HARDWARE;			//目的硬件握手
	dma_config_1.cfg_dst_per = DMA_USCI4_TX;						//选择目的硬件握手接口
	dma_config_1.ctl_int_en = 1;												//使能中断
	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_1, &dma_config_1, NULL);

	/*开始传输*/
	DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
	DMA_Mask_Irq(ch_num_1, DMA_IRQ_TFR);//使能传输结束中断
	DMA_Enable();
	NVIC_EnableIRQ(DMA_IRQn);
	DMA_Channel_Enable(ch_num_1);
	USCI4->CFG0 |= USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_UART TX Enable
}

void TMRB0_Init(void){
	TMRB_mTimeout(0,1); 
	NVIC_DisableIRQ(TMRB0_IRQn);
	NVIC_SetPriority (TMRB0_IRQn, TMRB0_PRIORITY_LV);
	NVIC_EnableIRQ(TMRB0_IRQn); //enable cpu quad timer0 interrupt
}

/**
 * @brief    USCI_UART收数据
 */
void Usci_Uart_DMA_Receive(void)
{
	enum dma_channel_number ch_num_2;
	ch_num_2 = DMA_CHANNEL2;	
	dma_channel_config_t dma_config_2;
	recCnt = 0; lastCnt = 0; recEnd = 0;
	
	/*DMA初始化*/
	USCI4->CFG0 &= ~USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_UART RX Disable
	DMA_Channel_Disable(ch_num_2);
	DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);	
	
	/*配置DMA通道2*/
	dma_config_2.sar = (uint32_t)&USCI4->TRX; 					//源地址
	dma_config_2.dar = (uint32_t)&test_dma_dst_buf[0];	//目的地址
	dma_config_2.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_2.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_2.ctl_sinc = DMA_ADDR_NOCHANGE;					//源地址不变
	dma_config_2.ctl_dinc = DMA_ADDR_INCREMENT;					//目的地址递增
	dma_config_2.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_2.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_2.ctl_block_ts = TEST_DMA_DATA_LENTH;		//单次传输数据量
	dma_config_2.ctl_tt_fc = DMA_PRF2MEM_DMA;						//传输方向---外设到内存
	dma_config_2.ctl_sms = DMA_MASTER2;									//源地址为USCI,属于层级2
	dma_config_2.ctl_dms = DMA_MASTER1;									//目的地址为内存,属于层级1
	dma_config_2.cfg_hs_sel_src = DMA_HS_HARDWARE;			//源硬件握手
	dma_config_2.cfg_src_per = DMA_USCI4_RX;						//选择源头硬件握手接口
	dma_config_2.ctl_int_en = 1;												//使能中断	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_2, &dma_config_2, NULL);

	TMRB0_Init();
	
	/*开始接收*/
	DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
	DMA_Mask_Irq(ch_num_2, DMA_IRQ_TFR);//使能传输结束中断
	DMA_Enable();
	NVIC_SetPriority(DMA_IRQn, DMA_PRIORITY_LV);
	NVIC_EnableIRQ(DMA_IRQn);
	DMA_Channel_Enable(ch_num_2);
	USCI4->CFG0 |= USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_UART RX Enable
//	printf("TS = %d\n", (DMA_Num2Ch(DMA_CHANNEL2)->HCTL & DMA_HCTL_BLOCK_TS_Msk) >> DMA_HCTL_BLOCK_TS_Pos);
}


/**
 * @brief     DMA_SPI发送
 * @param     u8Addr                    操作的寄存器地址
 * @param     u8Data                    写入寄存器的值
 * @param     len                       写入数据长度
 */
void Usci_Spi_DMA_Transmit(uint8_t u8Addr, uint16_t* u16Data, uint32_t len)
{
	uint32_t i;
	enum dma_channel_number ch_num_1;
	dma_channel_config_t dma_config_1;
	
	/*准备传输的数据*/
	test_dma_src_buf[0] = (u8Addr & 0x3F);     //SPI操作地址
	for(i=0; i< len; i++)
	{
		test_dma_src_buf[2*i+1] = u16Data[i] >> 8;
		test_dma_src_buf[2*i+2] = u16Data[i];
	}
		
	/*DMA初始化*/
	USCI3->CFG0 &= ~USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_SPI TX Disable
	DMA_CLK_ENABLE;
	DMA_Disable();
	DMA_Clear_All_Irq();
	DMA_Init();
	
	/*配置DMA通道1*/
	ch_num_1 = DMA_CHANNEL1;
	dma_config_1.sar = (uint32_t)&test_dma_src_buf[0]; 	//源地址
	dma_config_1.dar = (uint32_t)&USCI3->TRX;						//目的地址
	dma_config_1.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_1.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_1.ctl_sinc = DMA_ADDR_INCREMENT;					//源地址递增
	dma_config_1.ctl_dinc = DMA_ADDR_NOCHANGE;					//目的地址不变
	dma_config_1.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_1.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_1.ctl_block_ts = 2*len + 1;							//单次传输数据量
	dma_config_1.ctl_tt_fc = DMA_MEM2PRF_DMA;						//传输方向---内存到外设
	dma_config_1.ctl_sms = DMA_MASTER1;									//源地址为内存,属于层级1
	dma_config_1.ctl_dms = DMA_MASTER2;									//目的地址为USCI,属于层级2
	dma_config_1.cfg_hs_sel_dst = DMA_HS_HARDWARE;			//目的硬件握手
	dma_config_1.cfg_dst_per = DMA_USCI3_TX;						//选择目的硬件握手接口
	dma_config_1.ctl_int_en = 1;												//使能中断
	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_1, &dma_config_1, NULL);

	/*开始传输*/
	DMA_Clear_Irq(ch_num_1, DMA_IRQ_ALL);
	DMA_Mask_Irq(ch_num_1, DMA_IRQ_TFR);//使能传输结束中断
	
	SPI0_CS1_WritePin=0;//CSN拉低
	USCI3->ISR = USCI_IS_TE_Msk;	
	DMA_Enable();
	NVIC_EnableIRQ(DMA_IRQn);
	DMA_Channel_Enable(ch_num_1);
	USCI3->CFG0 |= USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_SPI TX Enable
}

/**
 * @brief     单通道实现DMA_SPI接收,CPU产生时钟Trig信号
 * @param     u8Addr                    读取的寄存器地址
 * @param     u8Data                    接收数组
 * @param     len                       接收数据长度
 */
void Usci_Spi_DMA_Receive(uint8_t u8Addr, uint8_t* u8Data, uint32_t len)
{
	uint32_t i;
	uint8_t SPI_write_data[100] = {0};
	enum dma_channel_number ch_num_2;
	dma_channel_config_t dma_config_2;

	/*初始化地址及DUMMY数据*/
	SPI_write_data[0] = (u8Addr & 0x3F) | 0x40;//SPI读取地址
	for(i=0; i< len; i++)
	{
		SPI_write_data[2*i+1] = 0xFF;
		SPI_write_data[2*i+2] = 0xFF;
	}
	
	/*DMA初始化*/
	USCI3->CFG0 &= ~USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_SPI RX Disable
	DMA_CLK_ENABLE;
	DMA_Disable();
	DMA_Clear_All_Irq();
	DMA_Init();
	
	/*配置DMA通道2*/
	ch_num_2 = DMA_CHANNEL2;
	dma_config_2.sar = (uint32_t)&USCI3->TRX; 					//源地址
	dma_config_2.dar = (uint32_t)&u8Data[0];						//目的地址
	dma_config_2.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_2.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_2.ctl_sinc = DMA_ADDR_NOCHANGE;					//源地址不变
	dma_config_2.ctl_dinc = DMA_ADDR_INCREMENT;					//目的地址递增
	dma_config_2.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输32Bit
	dma_config_2.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输32Bit
	dma_config_2.ctl_block_ts = len*2;									//单次传输数据量
	dma_config_2.ctl_tt_fc = DMA_PRF2MEM_DMA;						//传输方向---外设到内存
	dma_config_2.ctl_sms = DMA_MASTER2;									//源地址为USCI,属于层级2
	dma_config_2.ctl_dms = DMA_MASTER1;									//目的地址为内存,属于层级1
	dma_config_2.cfg_hs_sel_src = DMA_HS_HARDWARE;			//源硬件握手
	dma_config_2.cfg_src_per = DMA_USCI3_RX;						//选择硬件握手接口
	dma_config_2.ctl_int_en = 1;												//使能中断
	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_2, &dma_config_2, NULL);

	/*开启DMA接收之前，首先发送地址，否则第一个字节将接收到00*/
	SPI0_CS1_WritePin=0;//CSN拉低
	USCI3->ISR = USCI_IS_TE_Msk|USCI_IS_RXNE_Msk;
	USCI3->TRX = SPI_write_data[0];
	while((USCI3->ISR & USCI_IS_TXE_Msk)==0);

	/*开始接收*/
	DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
	DMA_Mask_Irq(ch_num_2, DMA_IRQ_TFR);//使能传输结束中断
	DMA_Enable();
	NVIC_SetPriority(DMA_IRQn, DMA_PRIORITY_LV);
	NVIC_EnableIRQ(DMA_IRQn);
	DMA_Channel_Enable(ch_num_2);
	USCI3->CFG0 |= USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_SPI RX Enable
	
	/*产生时钟,需要CPU参与*/
	for(i=1;i<2*len+1;i++)
	{
		USCI3->TRX = SPI_write_data[i];
		while((USCI3->ISR & USCI_IS_TXE_Msk)==0);
	}
}

/**
 * @brief     双通道实现DMA_SPI接收，一个通道负责接收，另一个通道负责产生时钟Trig信号
 * @param     u8Addr                    读取的寄存器地址
 * @param     u8Data                    接收数组
 * @param     len                       接收数据长度
 */
void Usci_Spi_DMA_Receive_2CH(uint8_t u8Addr, uint8_t* u8Data, uint32_t len)
{
	uint32_t i;
	uint8_t SPI_write_data[100] = {0};
	enum dma_channel_number ch_num_2;
	enum dma_channel_number ch_num_3;
	dma_channel_config_t dma_config_2;
	dma_channel_config_t dma_config_3;
	
	/*DMA初始化*/
	USCI3->CFG0 &= ~USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_SPI RX Disable
	DMA_CLK_ENABLE;
	DMA_Disable();
	DMA_Clear_All_Irq();
	DMA_Init();
	
	/*配置DMA通道2*/
	ch_num_2 = DMA_CHANNEL2;
	dma_config_2.sar = (uint32_t)&USCI3->TRX; 					//源地址
	dma_config_2.dar = (uint32_t)&test_dma_dst_buf[0];	//目的地址
	dma_config_2.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_2.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_2.ctl_sinc = DMA_ADDR_NOCHANGE;					//源地址不变
	dma_config_2.ctl_dinc = DMA_ADDR_INCREMENT;					//目的地址递增
	dma_config_2.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_2.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_2.ctl_block_ts = len*2;									//单次传输数据量
	dma_config_2.ctl_tt_fc = DMA_PRF2MEM_DMA;						//传输方向---外设到内存
	dma_config_2.ctl_sms = DMA_MASTER2;									//源地址为USCI,属于层级2
	dma_config_2.ctl_dms = DMA_MASTER1;									//目的地址为内存,属于层级1
	dma_config_2.cfg_hs_sel_src = DMA_HS_HARDWARE;			//源硬件握手
	dma_config_2.cfg_src_per = DMA_USCI3_RX;						//选择硬件握手接口
	dma_config_2.ctl_int_en = 1;												//使能中断
	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_2, &dma_config_2, NULL);
	
	
	/*配置通道3*/
	/*初始化地址及DUMMY数据*/
	SPI_write_data[0] = (u8Addr & 0x3F) | 0x40;//SPI读取地址
	for(i=0; i< len; i++)
	{
		SPI_write_data[2*i+1] = 0xFF;
		SPI_write_data[2*i+2] = 0xFF;
	}
	/*DMA初始化*/
	USCI3->CFG0 &= ~USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_SPI TX Disable
	
	ch_num_3 = DMA_CHANNEL3;
	dma_config_3.sar = (uint32_t)&SPI_write_data[1]; 		//源地址
	dma_config_3.dar = (uint32_t)&USCI3->TRX;						//目的地址
	dma_config_3.ctl_src_tr_width = DMA_TRANS_WIDTH_8;	//源位宽
	dma_config_3.ctl_dst_tr_width = DMA_TRANS_WIDTH_8;	//目的位宽
	dma_config_3.ctl_sinc = DMA_ADDR_INCREMENT;					//源地址递增
	dma_config_3.ctl_dinc = DMA_ADDR_NOCHANGE;					//目的地址不变
	dma_config_3.ctl_dst_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_3.ctl_src_msize = DMA_MSIZE_1;						//单次突发传输1字节
	dma_config_3.ctl_block_ts = 2*len;									//单次传输数据量
	dma_config_3.ctl_tt_fc = DMA_MEM2PRF_DMA;						//传输方向---内存到外设
	dma_config_3.ctl_sms = DMA_MASTER1;									//源地址为内存,属于层级1
	dma_config_3.ctl_dms = DMA_MASTER2;									//目的地址为USCI,属于层级2
	dma_config_3.cfg_hs_sel_dst = DMA_HS_HARDWARE;			//目的硬件握手
	dma_config_3.cfg_dst_per = DMA_USCI3_TX;						//选择目的硬件握手接口
	dma_config_3.ctl_int_en = 0;												//禁用中断
	
	/*配置写入*/
	DMA_Channel_Configure(ch_num_3, &dma_config_3, NULL);
	
	
	/*开启DMA接收之前，首先发送地址，否则第一个字节将接收到00*/
	SPI0_CS1_WritePin=0;//CSN拉低
	USCI3->ISR = USCI_IS_TE_Msk|USCI_IS_RXNE_Msk;
	USCI3->TRX = SPI_write_data[0];
	while((USCI3->ISR & USCI_IS_TXE_Msk)==0);
	
	/*开始传输*/
	DMA_Clear_Irq(ch_num_2, DMA_IRQ_ALL);
	DMA_Mask_Irq(ch_num_2, DMA_IRQ_TFR);//使能传输结束中断
	DMA_Enable();
	NVIC_SetPriority(DMA_IRQn, DMA_PRIORITY_LV);
	NVIC_EnableIRQ(DMA_IRQn);
	DMA_Channel_Enable(ch_num_2);
	DMA_Channel_Enable(ch_num_3);
	USCI3->CFG0 |= USCI_CFG0_DMA_RX_EN_Msk;//DMA USIC_SPI RX Enable
	USCI3->CFG0 |= USCI_CFG0_DMA_TX_EN_Msk;//DMA USIC_SPI TX Enable 	
}
