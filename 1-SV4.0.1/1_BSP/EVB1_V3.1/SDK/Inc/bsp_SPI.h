/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BSP_SPI_H
#define _BSP_SPI_H
#include "usci.h"
#include <stdio.h>
//SPI
#define SPI0_SCLK_PinMsk GPIO_PIN_13
#define SPI0_SCLK_PinNum 13
#define SPI0_SCLK_Port GPIO0

#define SPI0_MOSI_PinMsk GPIO_PIN_14
#define SPI0_MOSI_PinNum 14
#define SPI0_MOSI_Port GPIO0

#define SPI0_MISO_PinMsk GPIO_PIN_15
#define SPI0_MISO_PinNum 15
#define SPI0_MISO_Port GPIO0
//SPIms   master or slave
#define SPIms_SCLK_PinMsk GPIO_PIN_5
#define SPIms_SCLK_PinNum 5
#define SPIms_SCLK_Port GPIO0

#define SPIms_MOSI_PinMsk GPIO_PIN_6
#define SPIms_MOSI_PinNum 6
#define SPIms_MOSI_Port GPIO0

#define SPIms_MISO_PinMsk GPIO_PIN_7
#define SPIms_MISO_PinNum 7
#define SPIms_MISO_Port GPIO0



#define USCI_CMD_CLK_ON    USCI_U7816_CMD_CLK_ON
#define USCI_CMD_CLK_OFF   USCI_U7816_CMD_CLK_OFF




extern usci_handler_t SPI0;
extern usci_handler_t SPI1;
extern uint8_t snd_done ;

void BSP_SPI_Init(void);
void BSP_SPI_MasterOrSlave_Init(uint8_t run_mode);
void RF_SPI_CS(uint8_t csNum,uint8_t status);
void Usci_Spi_DMA_Receive_2CH_Ext(usci_handler_t *usci, uint32_t u32addr, uint32_t len);
void BSP_SPI_MasterOrSlave_User_Init(uint8_t run_mode);
#endif /*_BSP_SPI_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/
