#ifndef _HAL_USCI_DMA_H
#define _HAL_USCI_DMA_H

#include "SKY32B750.h"
#include "math.h"
#include "main.h"
#include "bsp_SPI.h"

#define TEST_DMA_DATA_LENTH 			128
#define DMA_PRIORITY_LV           10

void Usci_Uart_DMA_Transmit(void);
void Usci_Uart_DMA_Receive(void);
void Usci_Spi_DMA_Transmit(uint8_t u8Addr, uint16_t* u16Data, uint32_t len);
void Usci_Spi_DMA_Receive(uint8_t u8Addr, uint8_t* u8Data, uint32_t len);
void Usci_Spi_DMA_Receive_2CH(uint8_t u8Addr, uint8_t* u8Data, uint32_t len);
void DMA_IRQHandler(void);

#endif /*_HAL_USCI_DMA_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/
