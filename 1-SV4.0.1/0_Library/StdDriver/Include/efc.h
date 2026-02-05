/******************************************************************************
 * @Filename     : efc.h
 *
 * @Version      : V2.0
 * @Subversion   : $
 * @Last modified: 05/27/2022 15:56:38
 * @Modified by  : CB
 *
 * @Brief        : Embedded Flash Controller driver header file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#ifndef __EFC_H__
#define __EFC_H__

#ifdef __cplusplus
extern "C"
{
#endif

//Error Code
#define EFC_PAGE_ADDR_INVALID    INFOFLASH_ADDR_END + 0x0301 //addr is beyond of physical address
#define EFC_HEAD_ADDR_INVALID    INFOFLASH_ADDR_END + 0x0302 //addr is not flash page head address
#define EFC_CHECK_ERROR          INFOFLASH_ADDR_END + 0x0303 //check error

/*---------------------------------------------------------------------------------------------------------*/
/*  Eflash constant definitions                                                                            */
/*---------------------------------------------------------------------------------------------------------*/
#define EFLASH_PAGE_SIZE         8192           // 8192 Bytes per page
#define CODEFLASH_PAGE_BEGIN     0
#define CODEFLASH_PAGE_END       32             // total 32 main page
#define INFOFLASH_PAGE_BEGIN     0
#define INFOFLASH_PAGE_END       2
#define EFLASH_PAGE_OFFSET_MASK  0x1FFF
#define EFLASH_PAGE_ADDR_MASK    0xFFFFE000
#define CODEFLASH_ADDR_BEGIN     0x00000
#define CODEFLASH_ADDR_END       0x40000
#define INFOFLASH_ADDR_BEGIN     0x80000
#define INFOFLASH_ADDR_END       0x84000
#define PAGE_ROW_NUM             16             // 16 rows per page
#define ROW_WORD_NUM             128            // 128 words per row
#define ROW_BYTE_NUM             512            // 512 bytes per row

/*---------------------------------------------------------------------------------------------------------*/
/*  FCMD constant definitions                                                                              */
/*---------------------------------------------------------------------------------------------------------*/
#define EFC_FCMD_PROGRAM      0x000000E1
#define EFC_FCMD_FPROGRAM     0x000000D2
#define EFC_FCMD_ERASE        0x000000A5
#define EFC_FCMD_MERASE       0x00000096
#define EFC_FCMD_VOID         0x00000000

uint32_t EFC_Get_ActualAddress(uint32_t u32Addr);
uint32_t EFC_Erase1Page(uint32_t u32PageAddr);
uint32_t EFC_Erase1Page_and_Check(uint32_t u32PageAddr);
uint32_t EFC_Write1Page(uint32_t u32Addr, uint32_t *u32DataBuf);
uint32_t EFC_Write1Page_and_Check(uint32_t u32Addr, uint32_t *u32DataBuf);
uint32_t EFC_Write_Words(uint32_t u32Addr, uint32_t u32Len, uint32_t *u32DataBuf);
uint32_t EFC_Write1Byte(uint32_t u32Addr, uint8_t *u8DataBuf);
uint32_t EFC_Write_Bytes(uint32_t u32Addr, uint32_t u32Len, uint8_t *u8DataBuf);
uint32_t EFC_WriteInside1Page(uint32_t u32Addr, uint32_t u32Len, uint8_t *u8DataBuf);
uint32_t EFC_ReWrite(uint32_t u32Addr, uint32_t u32Len, uint8_t *u8DataBuf);
void EFC_Read(uint32_t u32Addr, uint32_t u32Len, uint32_t *u32DataBuf);
uint32_t EFC_Set_Main_Ro_Addr(uint8_t u8PageNum);
uint32_t EFC_Set_Info_Ro_Addr(uint8_t u8PageNum);
uint32_t EFC_Set_Boot_Addr(uint8_t u8PageNum);
#ifdef __cplusplus
}
#endif

#endif // __EFC_H__

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
