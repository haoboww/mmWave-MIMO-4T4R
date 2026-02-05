/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _HAL_DMA_H
#define _HAL_DMA_H
#include "dma.h"

#define UART_PACK_SIZE 2047

void DMA_Config_Init(void);

void DMA_Start_TransferM2M_32(uint32_t *srcAddr,uint32_t *dstAddr,uint32_t Len,enum dma_channel_number channel);
void DMA_Start_TransferM2M_SRAM2K(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr,uint32_t Len,enum dma_channel_number channel);
void DMA_Start_TransferM2M_SRAM_Step1(void);
void DMA_Start_TransferM2M_SRAM2K_Step2(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr);
void DMA_Start_TransferM2M_SRAM4K_Step2(uint32_t srcAddr,uint32_t midAddr,uint32_t dstAddr);
void DMA_Start_TransferM2M_Flash2K_Step2(uint32_t srcAddr,uint32_t dstAddr,uint32_t times);
#endif /*_HAL_DMA_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























