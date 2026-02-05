/******************************************************************************/
/* RETARGET.C: 'Retarget' layer for target-dependent low level functions      */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005 Keil Software. All rights reserved.                     */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/
#include <stdio.h>
#include "SKY32B750.h"

#pragma import(__use_no_semihosting_swi)

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

usci_handler_t  dbg_uart;

void hard_printf(USCI_T *usci, uint32_t uart_bps)
{
  USCI_Enable(usci);

  dbg_uart.instance = usci;
  dbg_uart.u_init.uart_data_len = USCI_UART_DATA_LEN_8BIT;   //8 bit
  dbg_uart.u_init.uart_parity_en = USCI_UART_PAR_EN_OFF;     //parity off
  dbg_uart.u_init.uart_lsb = USCI_UART_LSB_LSB;              //LSB
  dbg_uart.u_init.uart_baud_rate = uart_bps;
  dbg_uart.u_init.uart_stop_bit = USCI_UART_STOP_BIT_1;

  USCI_UART_Init(&dbg_uart);
}

int sendchar(int ch)
{
  if (ch == '\n' || ch == '\r')
  {
		dbg_uart.instance->ISR = USCI_IS_TE_Msk;		
    dbg_uart.instance->TRX = ch;
    while ((dbg_uart.instance->ISR & USCI_IS_TE_Msk) == 0);
  }
	else
	{
		dbg_uart.instance->TRX = ch;
		while ((dbg_uart.instance->ISR & USCI_IS_TXE_Msk) == 0);
	}
  return 0;
}

int getkey(void)
{
  while ((dbg_uart.instance->ISR & USCI_IS_RXNE_Msk) == 0);
  return (dbg_uart.instance->TRX & BYTEMsk);
}

int fputc(int ch, FILE *f)
{
  return (sendchar(ch));
}

int fgetc(FILE *f)
{
  return (sendchar(getkey()));
}

int ferror(FILE *f)
{
  /* Your implementation of ferror */
  return EOF;
}

void _ttywrch(int ch)
{
  sendchar(ch);
}

void _sys_exit(int return_code)
{
  while (1);    /* endless loop */
}
