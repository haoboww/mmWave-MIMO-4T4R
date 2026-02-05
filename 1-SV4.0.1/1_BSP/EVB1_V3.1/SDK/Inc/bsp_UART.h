/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "usci.h"
#include <stdio.h>
#include <string.h>


//USART
#define UART0_TX_Pin 2
#define UART0_TX_Port PORT01_PINMUX
#define UART0_RX_Pin 3
#define UART0_RX_Port PORT01_PINMUX


#define UART_REC_MAX_LEN 50
#define UART_MAX_TIMEOUT_NUM 2000

//
#define USART0_TX_Pin 10
#define USART0_RX_Pin 11


typedef struct
{
    uint8_t RecCount;
    uint32_t RecLen;
    uint8_t RecFinish;
    uint8_t RecData;
    uint8_t RecBuff[UART_REC_MAX_LEN];
    uint8_t timeoutStart;
    uint32_t timeoutCnt;
    uint32_t writeCnt;
}STRUCT_UART;

extern usci_handler_t  UART0;
extern uint8_t usci_uart_rx_it(usci_handler_t *usci);
extern STRUCT_UART strUart;












void BSP_UART_Init(void);
void UART_Transmit_Bytes(usci_handler_t *usci, uint8_t *tx_buf, int32_t size);
int sendchar(int ch);
#endif /*_BSP_UART_H */

/************************ (C) COPYRIGHT  *****END OF FILE****/























