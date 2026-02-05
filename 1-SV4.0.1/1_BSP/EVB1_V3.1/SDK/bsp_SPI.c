#include "main.h"
#include "bsp_SPI.h"
usci_handler_t SPI0;//RF 
usci_handler_t SPI1;//dma transmit fft mean

uint8_t snd_done = 0;

uint32_t snd_ff=0xff;

void BSP_SPI_Init(void){
      USCI3_CLK_ENABLE;
    //CS1 CS2 CS3
    // pull up
    PORT01_PIN_CTRL->UE |= 0xFFFF; //open input for analog 2.0-2.15
    GPIO_PullUp(GPIO0, SPI0_SCLK_PinMsk);
    GPIO_PullUp(GPIO0, SPI0_MOSI_PinMsk);
    GPIO_PullUp(GPIO0, SPI0_MISO_PinMsk);
    //CLK MOSI MISO
    PinMux_Set(SPI0_SCLK_Port, SPI0_SCLK_PinNum, IOMODE_USCI3_IO0);
    PinMux_Set(SPI0_MOSI_Port, SPI0_MOSI_PinNum, IOMODE_USCI3_IO1);
    PinMux_Set(SPI0_MISO_Port, SPI0_MISO_PinNum, IOMODE_USCI3_IO2);

    SPI0.instance = USCI3;
    SPI0.instance->CMD = USCI_CMD_CLK_ON;
    SPI0.s_init.spi_mode = USCI_SPI_WORK_MODE_MASTER;
    SPI0.s_init.spi_clkp = USCI_SPI_CLKP_IDLE_LOW;
    SPI0.s_init.spi_posedge = USCI_SPI_TRX_NEG_NEGEDGE;
    SPI0.s_init.spi_msb = USCI_SPI_LSB_MSB;
    SPI0.s_init.spi_data_len = USCI_SPI_DATA_LEN_8BIT;
    SPI0.usci_clock_div = 4;  //freq=pclk/2/(div+1) 4 ->16

    USCI_SPI_Init(&SPI0);
//    printf("spitest\n");
    #if 0
        RF_CHIPEN();
        RF_SPI_Init();
        while(1);
    #endif
}

void RF_SPI_CS(uint8_t csNum,uint8_t status)
{
	switch(csNum){
		case 0:
			SPI0_CS1_WritePin=status;
			break;
		case 1:
			SPI0_CS2_WritePin=status;
			break;
		case 2:		
			break;
		case 4:
			SPIms_CS5_WritePin=status;
		default:
      break;		
	}
}

