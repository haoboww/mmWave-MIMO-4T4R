/******************************************************************************
 * @Filename     : dma.h
 *
 * @Version      : V1.0
 * @Subversion   : $
 * @Last modified: 05/07/2022 16:01:41
 * @Modified by  : CB
 *
 * @Brief        : DMA driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __DMA_H__
#define __DMA_H__

#include <stdbool.h>
#include "SKY32B750.h"

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define DMA_INVALID_PARA          0x0401

//CTL register value
#define INT_EN                    0x01ul

#define DMA_DMACFG_DISABLE        0x00
#define DMA_DMACFG_ENABLE         0x01

#define DMA_CHEN_CH_DISABLE       0x0000ul
#define DMA_CHEN_CH_ENABLE        0x0001ul
#define DMA_CHEN_CH_WE_DISABLE    0x0000ul
#define DMA_CHEN_CH_WE_ENABLE     0x0100ul

#define DMA_CLEARBLOCK            0xFFFF
#define DMA_MASKBLOCK             0xFFFF

enum dma_master_number {
    DMA_MASTER1 = 0x0,
    DMA_MASTER2 = 0x1,
};

enum dma_address_increment {
    DMA_ADDR_INCREMENT  = 0x0,
    DMA_ADDR_DECREMENT  = 0x1,
    DMA_ADDR_NOCHANGE   = 0x2
};

enum dma_hs_interface {
    DMA_USCI0_TX     = 0,
    DMA_USCI0_RX     = 1,
    DMA_USCI1_TX     = 2,
    DMA_USCI1_RX     = 3,
    DMA_USCI2_TX     = 4,
    DMA_USCI2_RX     = 5,
    DMA_USCI3_TX     = 6,
    DMA_USCI3_RX     = 7,
    DMA_USCI4_TX     = 8,
    DMA_USCI4_RX     = 9,
    DMA_USCI5_TX     = 10,
    DMA_USCI5_RX     = 11,
    DMA_USCI6_TX     = 12,
    DMA_USCI6_RX     = 13,
    DMA_USCI7_TX     = 14,
    DMA_USCI7_RX     = 15,
    DMA_OTHER_MAX    = 16,
    DMA_TIMERA0      = DMA_USCI4_TX,
    DMA_TIMERA1      = DMA_USCI4_RX,
    DMA_TIMERA2      = DMA_USCI5_TX,
    DMA_TIMERA3      = DMA_USCI5_RX,
    DMA_TIMERA4      = DMA_USCI6_TX,
    DMA_TIMERA5      = DMA_USCI6_RX,
    DMA_TIMERA6      = DMA_USCI7_TX,
    DMA_TIMERA7      = DMA_USCI7_RX,
    DMA_OSPI0        = DMA_USCI3_TX,
    DMA_OSPI1        = DMA_USCI3_RX,
};

enum dma_polarity_level {
    DMA_ACTIVE_HIGH  = 0x0,
    DMA_ACTIVE_LOW   = 0x1
};

enum dma_sw_hw_hs_select {
    DMA_HS_HARDWARE  = 0x0,
    DMA_HS_SOFTWARE  = 0x1
};

enum dma_prot_level {
    DMA_NONCACHE_NONBUFF_NONPRIV_OPCODE = 0x0,
    DMA_NONCACHE_NONBUFF_NONPRIV_DATA   = 0x1,
    DMA_NONCACHE_NONBUFF_PRIV_OPCODE    = 0x2,
    DMA_NONCACHE_NONBUFF_PRIV_DATA      = 0x3,
    DMA_NONCACHE_BUFF_NONPRIV_OPCODE    = 0x4,
    DMA_NONCACHE_BUFF_NONPRIV_DATA      = 0x5,
    DMA_NONCACHE_BUFF_PRIV_OPCODE       = 0x6,
    DMA_NONCACHE_BUFF_PRIV_DATA         = 0x7,
    DMA_CACHE_NONBUFF_NONPRIV_OPCODE    = 0x8,
    DMA_CACHE_NONBUFF_NONPRIV_DATA      = 0x9,
    DMA_CACHE_NONBUFF_PRIV_OPCODE       = 0xA,
    DMA_CACHE_NONBUFF_PRIV_DATA         = 0xB,
    DMA_CACHE_BUFF_NONPRIV_OPCODE       = 0xC,
    DMA_CACHE_BUFF_NONPRIV_DATA         = 0xD,
    DMA_CACHE_BUFF_PRIV_OPCODE          = 0xE,
    DMA_CACHE_BUFF_PRIV_DATA            = 0xF
};

enum dma_fifo_mode {
    DMA_FIFO_MODE_SINGLE = 0x0,
    DMA_FIFO_MODE_HALF   = 0x1
};

enum dma_flow_ctl_mode {
    DMA_DATA_PREFETCH_ENABLED   = 0x0,
    DMA_DATA_PREFETCH_DISABLED  = 0x1
};

enum dma_lock_level {
    DMA_LOCK_LEVEL_DMA_TRANSFER   = 0x0,
    DMA_LOCK_LEVEL_BLOCK_TRANSFER = 0x1,
    DMA_LOCK_LEVEL_TRANSACTION    = 0x2
};

enum dma_channel_priority {
    DMA_PRIORITY_0 = 0x0,
    DMA_PRIORITY_1 = 0x1,
    DMA_PRIORITY_2 = 0x2,
    DMA_PRIORITY_3 = 0x3,
    DMA_PRIORITY_4 = 0x4,
    DMA_PRIORITY_5 = 0x5,
    DMA_PRIORITY_6 = 0x6,
    DMA_PRIORITY_7 = 0x7
};

enum dma_channel_number {
    DMA_CHANNEL0    = 0,
    DMA_CHANNEL1    = 1,
    DMA_CHANNEL2    = 2,
    DMA_CHANNEL3    = 3,
    DMA_CHANNEL4    = 4,
    DMA_CHANNEL5    = 5,
    DMA_CHANNEL6    = 6,
    DMA_CHANNEL7    = 7,
    DMA_CHANNEL_MAX = 8
};

enum dma_irq {
    DMA_IRQ_NONE    = 0x00, /*!< no interrupts */
    DMA_IRQ_TFR     = 0x01, /*!< transfer complete */
    DMA_IRQ_BLOCK   = 0x02, /*!< block transfer complete */
    DMA_IRQ_SRCTRAN = 0x04, /*!< source transaction complete */
    DMA_IRQ_DSTTRAN = 0x08, /*!< destination transaction complete */
    DMA_IRQ_ERR     = 0x10, /*!< error */
    DMA_IRQ_ALL     = 0x1f  /*!< all interrupts */
};

enum dma_software_req {
    DMA_REQUEST        = 0x1, /* ReqSrcReq/ReqDstReq */
    DMA_SINGLE_REQUEST = 0x2, /* SglReqSrcReq/SglReqDstReq */
    DMA_LAST_REQUEST   = 0x4  /* LstReqSrcReq/LstReqDstReq */
};

enum dma_transfer_flow {
    DMA_MEM2MEM_DMA    = 0x0, /* mem to mem - DMAC   flow ctlr */
    DMA_MEM2PRF_DMA    = 0x1, /* mem to prf - DMAC   flow ctlr */
    DMA_PRF2MEM_DMA    = 0x2, /* prf to mem - DMAC   flow ctlr */
    DMA_PRF2PRF_DMA    = 0x3, /* prf to prf - DMAC   flow ctlr */
    DMA_PRF2MEM_PRF    = 0x4, /* prf to mem - periph flow ctlr */
    DMA_PRF2PRF_SRCPRF = 0x5, /* prf to prf - source flow ctlr */
    DMA_MEM2PRF_PRF    = 0x6, /* mem to prf - periph flow ctlr */
    DMA_PRF2PRF_DSTPRF = 0x7  /* prf to prf - dest   flow ctlr */
};

enum dma_burst_trans_length {
    DMA_MSIZE_1   = 0x0,
    DMA_MSIZE_4   = 0x1,
    DMA_MSIZE_8   = 0x2,
    DMA_MSIZE_16  = 0x3,
    DMA_MSIZE_32  = 0x4,
    DMA_MSIZE_64  = 0x5,
    DMA_MSIZE_128 = 0x6,
    DMA_MSIZE_256 = 0x7
};

enum dma_transfer_width {
    DMA_TRANS_WIDTH_8   = 0x0,
    DMA_TRANS_WIDTH_16  = 0x1,
    DMA_TRANS_WIDTH_32  = 0x2,
    DMA_TRANS_WIDTH_64  = 0x3,
    DMA_TRANS_WIDTH_128 = 0x4,
    DMA_TRANS_WIDTH_256 = 0x5,
};

enum dma_multiblk_transfer_type {
    CONTIGUOUS     = 0,
    RELOAD         = 1,
    SHADOWREGISTER = 2,
    LINKEDLIST     = 3
};

enum dma_multiblk_type {
    DMA_SRC_DST_CONTINUE              = 0,
    DMA_SRC_CONTINUE_DST_RELAOD       = 2,
    DMA_SRC_CONTINUE_DST_LINKEDLIST   = 3,
    DMA_SRC_RELOAD_DST_CONTINUE       = 4,
    DMA_SRC_RELOAD_DST_RELOAD         = 5,
    DMA_SRC_RELOAD_DST_LINKEDLIST     = 6,
    DMA_SRC_LINKEDLIST_DST_CONTINUE   = 7,
    DMA_SRC_LINKEDLIST_DST_RELOAD     = 8,
    DMA_SRC_LINKEDLIST_DST_LINKEDLIST = 9,
    DMA_SRC_SHADOWREG_DST_CONTINUE    = 10
};

typedef struct {
    uint32_t err_flag;
    uint32_t block_flag;
    uint32_t dstTran_flag;
    uint32_t srcTran_flag;
    uint32_t tfr_flag;
	  uint32_t stat_flag;
} dma_irq_flag_t;

typedef struct {
    uint32_t                        sar;              //SAR: source address
    uint32_t                        dar;              //DAR: destiation address
    uint8_t                         ctl_llp_src_en;
    uint8_t                         ctl_llp_dst_en;
    enum dma_master_number          ctl_sms;          //CTL[26:25]: Source Master Select
    enum dma_master_number          ctl_dms;          //CTL[24:23]: Destination Master Select
    enum dma_burst_trans_length     ctl_src_msize;    //CTL[16:14]: Source Burst Transaction Length
    enum dma_burst_trans_length     ctl_dst_msize;    //CTL[13:11]: Destination Burst Transaction Length
    enum dma_address_increment      ctl_sinc;         //CTL[10:9]:  Source Address Increment
    enum dma_address_increment      ctl_dinc;         //CTL[8:7]:   Destination Address Increment
    enum dma_transfer_width         ctl_src_tr_width; //CTL[6:4]:   Source Transfer Width
    enum dma_transfer_width         ctl_dst_tr_width; //CTL[3:1]:   Destination Transfer Width
    uint32_t                        sstat;            //SSTAT:      Source Status
    uint32_t                        dstat;            //DSTAT:      Destination Status
    uint32_t                        sstatar;          //SSTATAR:    Source Status Address
    uint32_t                        dstatar;          //DSTATAR:    Destination Status Address
    enum dma_hs_interface           cfg_dst_per;      //HCFG:       Assigns a hardware handshaking interface
    enum dma_hs_interface           cfg_src_per;      //HCFG:       Assigns a hardware handshaking interface
    uint8_t                         cfg_ss_upd_en;    //HCFG[6]:    Source status update enable
    uint8_t                         cfg_ds_upd_en;    //HCFG[5]:    Destination status update enable
    uint8_t                         cfg_reload_src;   //CFG[30]:    Automatic souce reload
    uint8_t                         cfg_reload_dst;   //CFG[31]:    Automatic destination reload
    enum dma_polarity_level         cfg_src_hs_pol;   //CFG[19]:    Source handshaking interface polarity
    enum dma_polarity_level         cfg_dst_hs_pol;   //CFG[18]:    Destination handshaking interface polarity
    enum dma_sw_hw_hs_select        cfg_hs_sel_src;   //CFG[11]:    Source software or hardware handshaking select
    enum dma_sw_hw_hs_select        cfg_hs_sel_dst;   //CFG[10]:    Destination software or hardware handshaking select

    uint32_t                        llp_loc;          //LLP[31:2]:  Starting address in memory of next LLI
    enum dma_master_number          llp_lms;          //LLP[1:0]:   List master select
    uint8_t                         ctl_done;         //HCTL[12]:   Done bit
    uint32_t                        ctl_block_ts;     //HCLT[11:0]: Block transfer size
    enum dma_transfer_flow          ctl_tt_fc;        //CTL[22:20]: Transfer type and flow control
    uint8_t                         ctl_dst_scatter_en; //CTL[18]:  Destination scatter enable
    uint8_t                         ctl_src_gather_en;  //CTL[17]:  Source gather enable
    uint8_t                         ctl_int_en;       //CTL[0]:     Interrupt enable
    enum dma_prot_level             cfg_protctl;      //HCFG[4:2]:  protection control bits used to drive the AHB HPROT[3:1]
    enum dma_fifo_mode              cfg_fifo_mode;    //HCFG[1]:    FIFO mode select
    enum dma_flow_ctl_mode          cfg_fcmode;       //HCFG[0]:    Flow control mode
    uint32_t                        cfg_max_abrst;    //CFG[29:20]: Maximum AMBA burst length
    uint8_t                         cfg_lock_b;       //CFG[17]:    Bus lock bit
    uint8_t                         cfg_lock_ch;      //CFG[16]:    Channel lock bit
    enum dma_lock_level             cfg_lock_b_l;     //CFG[15:14]: Bus lock level
    enum dma_lock_level             cfg_lock_ch_l;    //CFG[13:12]: Channel lock level
    enum dma_channel_priority       cfg_ch_prior;     //CFG[7:5]:   Channel priority
    uint32_t                        sgr_sgc;          //SGR[]:      Source gather count
    uint32_t                        sgr_sgi;          //SGR[19:0]:  Source gather interval
    uint32_t                        dsr_dsc;          //DSR[]:      Destination scatter count
    uint32_t                        dsr_dsi;          //DSI[19:0]:  Destination scatter interval
} dma_channel_config_t;

extern volatile dma_irq_flag_t dma_irq_sta;
uint8_t DMA_Init(void);
void DMA_Enable(void);
void DMA_Disable(void);
uint16_t DMA_Channel_Configure(enum dma_channel_number ch_num, dma_channel_config_t *dma_config, void (*callback)(void));
uint16_t DMA_Channel_Enable(enum dma_channel_number ch_num);
uint16_t DMA_Channel_Irq_Disable(enum dma_channel_number ch_num);
uint16_t DMA_Channel_Irq_Enable(enum dma_channel_number ch_num);
bool DMA_Channel_Irq_IsEnable(enum dma_channel_number ch_num);
bool DMA_RawIrq_IsActive(enum dma_channel_number ch_num, enum dma_irq ch_irq);
void DMA_Clear_All_Irq(void);
uint16_t DMA_Clear_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq);
uint16_t DMA_Mask_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq);
uint16_t DMA_Unmask_Irq(enum dma_channel_number ch_num, enum dma_irq ch_irq);
DMA_CH_T* DMA_Num2Ch(enum dma_channel_number dma_channel);
enum dma_channel_number DMA_Get_FreeChannel(void);

#ifdef __cplusplus
}
#endif

#endif // __DMA_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
