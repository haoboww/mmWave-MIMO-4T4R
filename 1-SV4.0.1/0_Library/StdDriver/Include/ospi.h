/******************************************************************************
 * @Filename     : ospi.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 06/30/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : OSPI driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __OSPI_H__
#define __OSPI_H__

#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

#pragma anon_unions
typedef struct {
  union
  {
    __IO uint64_t DATA64[8];
    __IO uint32_t DATA32[16];
    __IO uint8_t  DATA8[64];
  };
} XOSPI_T;

enum ospi_frame_mode {
  OSPI_SING      = 0x00ul,         /* (cs,clk,out1,in1,NO,NO,NO,NO,NO,NO) */            //-----------
  OSPI_DSING     = 0x01ul,         /* (cs,clk,out1,in1,out1,in1,NO,NO,NO,NO) */
  OSPI_QSING     = 0x02ul,         /* (cs,clk,out1,in1,out1,in1,out1,in1,out1,in1) */
  OSPI_OSING     = 0x03ul,         /* (cs,clk,out1,in1,NO,NO,out1,in1,NO,NO) */            //---------
  OSPI_I_DUAL    = 0x04ul,         /* (cs,clk,in1,in2,NO,NO,NO,NO,NO,NO) */
  OSPI_O_DUAL    = 0x05ul,         /* (cs,clk,out1,out2,NO,NO,NO,NO,NO,NO) */
  OSPI_I_DDUAL   = 0x06ul,         /* (cs,clk,in1,in2,NO,NO,in1,in2,NO,NO) */
  OSPI_O_DDUAL   = 0x07ul,         /* (cs,clk,out1,out2,NO,NO,out1,out2,NO,NO) */
  OSPI_I_QDUAL   = 0x08ul,         /* (cs,clk,in1,in2,in1,in2,in1,in2,in1,in2) */
  OSPI_O_QDUAL   = 0x09ul,         /* (cs,clk,out1,out2,out1,out2,out1,out2,out1,out2) */
  OSPI_I_QUAD    = 0x0Aul,         /* (cs,clk,in1,in2,in3,in4,NO,NO,NO,NO) */
  OSPI_O_QUAD    = 0x0Bul,         /* (cs,clk,out1,out2,out3,out4,NO,NO,NO,NO) */
  OSPI_I_DQUAD   = 0x0Cul,         /* (cs,clk,in1,in2,in3,in4,in1,in2,in3,in4) */             //---------
  OSPI_O_DQUAD   = 0x0Dul,         /* (cs,clk,out1,out2,out3,out4,out1,out2,out3,out4) */      //---------
  OSPI_I_OCTU    = 0x0Eul,         /* (cs,clk,in1,in2,in3,in4,in5,in6,in7,in8) */
  OSPI_O_OCTU    = 0x0Ful,         /* (cs,clk,out1,out2,out3,out4,out5,out6,out7,out8) */
  OSPI_MAX       = OSPI_SING,
};

enum ospi_irq {
  OSPI_FIFO_IRQ = 0x4,
  OSPI_SEND_IRQ = 0x2,
  OSPI_NEND_IRQ = 0x1,
  OSPI_ALL_IRQ  = 0x7
};

typedef struct
{
  uint8_t fifo_wl;
  uint8_t slave_en;
  uint8_t dma_en;
  enum ospi_frame_mode trans_mode;
  uint8_t cpha;
  uint8_t fifo_int_en;
  uint8_t send_int_en;
  uint8_t nend_int_en;
  uint8_t read_cmd;
  uint8_t tshsl;
  uint8_t dummy_len;
  uint8_t dummy_byte;
  uint8_t div;
  uint8_t cmd_on;
  uint8_t fifo_en;
  uint8_t xip_en;
} ospi_config_t;

void OSPI_Init(OSPI_T *ospi, ospi_config_t config);
void OSPI_CS_HIGH(OSPI_T *ospi);
void OSPI_CS_LOW(OSPI_T *ospi);
void OSPI_Clear_Irq(OSPI_T *ospi, enum ospi_irq irq);
bool OSPI_Irq_IsActive(OSPI_T *ospi, enum ospi_irq irq);

#ifdef __cplusplus
}
#endif

#endif // __OSPI_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
