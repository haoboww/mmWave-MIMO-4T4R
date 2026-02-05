/******************************************************************************
 * @Filename     : efc.c
 *
 * @Version      : V2.0
 * @Subversion   : $
 * @Last modified: 05/27/2022 8:55:08
 * @Modified by  : CB
 *
 * @Brief        : Embedded Flash Controller driver source file
 *
 * Copyright (C) 2022 SKYRELAY Technology co.,Ltd All rights reserved.
 *****************************************************************************/

#include "SKY32B750.h"

union {
    uint8_t flash_buf_8[EFLASH_PAGE_SIZE];
    uint32_t flash_buf_32[EFLASH_PAGE_SIZE / 4];
} flash_buf;

/**
 * @brief     EFC_Get_ActualAddress: Get the actual flash address to operate on
 * @DateTime  2022-05-27T14:28:26+0800
 * @param     u32Addr                   logic address to operate on
 * @return                              actual address to operate on
 *                                      EFC_PAGE_ADDR_INVALID
 */
uint32_t EFC_Get_ActualAddress(uint32_t u32Addr)
{
    uint32_t ret_addr = 0;
    // Check Input u32Addr
    if (!((u32Addr < CODEFLASH_ADDR_END) || ((u32Addr >= INFOFLASH_ADDR_BEGIN) && (u32Addr < INFOFLASH_ADDR_END))))
    {
        return EFC_PAGE_ADDR_INVALID; // u32Addr is beyond of physical address
    }
    else
    {
        if(EFC->REMAP_DIS == 0x01)  //In Boot
        {
            return u32Addr;
        }
        else                        //In App
        {
            if(u32Addr < CODEFLASH_ADDR_END)     //Code Addr
                ret_addr = (EFC->BOOT_NUM * EFLASH_PAGE_SIZE) + u32Addr;
            else                                 //Info Addr
                return EFC_PAGE_ADDR_INVALID;
            if(ret_addr >= CODEFLASH_ADDR_END)
                return EFC_PAGE_ADDR_INVALID;
        }
    }
    return ret_addr;
}

/**
 * @brief     EFC_Erase1Page : Erase one flash page
 * @DateTime  2022-05-04T15:34:34+0800
 * @param     u32PageAddr               page address
 */
uint32_t EFC_Erase1Page(uint32_t u32PageAddr)
{
    uint32_t act_addr = 0;
    act_addr = EFC_Get_ActualAddress(u32PageAddr);
    if(act_addr == EFC_PAGE_ADDR_INVALID)
        return EFC_PAGE_ADDR_INVALID;

    // erase 1 page
    EFC->PROTECT = 0xEDCB1234;
    EFC->PROTECT = 0xA9875678;
    EFC->PROTECT = 0x65439ABC;

    EFC->ADDR = (act_addr & EFLASH_PAGE_ADDR_MASK);
    EFC->CON = EFC_FCMD_ERASE;
    EFC->WDATA = 0xFFFFFFFF;
    while((EFC->STA & 0x00000001) != 0x00000001){};

    EFC->PROTECT = 0x00000000;
    return OK;
}

/**
 * @brief     EFC_Erase1Page_and_Check: Erase one flash page and check
 * @DateTime  2022-05-04T15:32:58+0800
 * @param     u32PageAddr               page address
 * @return                              check result of erase
 *                                      = NOK (1)   erase failure
 *                                      = OK  (0)   erase successfully
 */
uint32_t EFC_Erase1Page_and_Check(uint32_t u32PageAddr)
{
    uint32_t u32PageBaseAddr = 0;
    uint32_t i = 0;
    int32_t rt = OK;


    u32PageBaseAddr = (u32PageAddr & EFLASH_PAGE_ADDR_MASK);

    EFC_Erase1Page(u32PageAddr);
    // Check
    for (i = u32PageBaseAddr; i < (u32PageBaseAddr + EFLASH_PAGE_SIZE); i += 4)
    {
        if (Read_M32(i) != 0xffffffff)
        {
            rt = EFC_CHECK_ERROR; // Failed
        }
    }
    return rt;
}

/**
 * @brief     EFC_Write1Page: Write one page
 * @DateTime  2022-05-04T14:19:04+0800
 * @param     u32Addr                   page address
 * @param     u32DataBuf                pointer to write data buffer
 * @return                              write result
 *                                      = EFC_PAGE_ADDR_INVALID: page address is not valid
 *                                      = EFC_HEAD_ADDR_INVALID: page address is not page head address
 *                                      = OK: write and check success
 */
uint32_t EFC_Write1Page(uint32_t u32Addr, uint32_t *u32DataBuf)
{
    uint32_t i = 0, row = 0;
    unsigned long adr_use;
    uint32_t *buf_use;
    uint32_t act_addr = 0;

    act_addr = EFC_Get_ActualAddress(u32Addr);
    if(act_addr == EFC_PAGE_ADDR_INVALID)
        return EFC_PAGE_ADDR_INVALID;

    if ( (act_addr & EFLASH_PAGE_OFFSET_MASK) != 0)
    {
        return EFC_HEAD_ADDR_INVALID; // act_addr is not flash page head address
    }

    adr_use = act_addr;
    buf_use = u32DataBuf;

    EFC->PROTECT = 0xEDCB1234;
    EFC->PROTECT = 0xA9875678;
    EFC->PROTECT = 0x65439ABC;

    for(row = 0; row < PAGE_ROW_NUM; row++)
    {
        for(i = 0; i < ROW_WORD_NUM; i++)
        {
            EFC_BUF->BUF[i] = *((uint32_t *) buf_use);
            buf_use += 1;
        }
        EFC->FPROG_LEN = i;
        EFC->CON = EFC_FCMD_FPROGRAM;
        EFC->ADDR = (adr_use + (row * ROW_WORD_NUM* 4));
        EFC->WDATA = 0xFFFFFFFF;
        while((EFC->STA & 0x00000001) != 0x00000001){};
    }

    EFC->PROTECT = 0x00000000;
    return OK;
}

/**
 * @brief     EFC_Write1Page_and_Check: Write one page and check
 * @DateTime  2022-05-04T15:31:45+0800
 * @param     u32Addr                   write address
 * @param     u32DataBuf                pointer to write data buffer
 * @return                              write result
 *                                      = EFC_PAGE_ADDR_INVALID: page address is not valid
 *                                      = EFC_HEAD_ADDR_INVALID: page address is not page head address
 *                                      = EFC_CHECK_ERROR: check error
 *                                      = OK: write and check success
 */
uint32_t EFC_Write1Page_and_Check(uint32_t u32Addr, uint32_t *u32DataBuf)
{
    uint32_t i = 0;

    // Check Addr
    if (!((u32Addr < CODEFLASH_ADDR_END) || ((u32Addr >= INFOFLASH_ADDR_BEGIN) && (u32Addr < INFOFLASH_ADDR_END))))
    {
        return EFC_PAGE_ADDR_INVALID; // u32Addr is beyond of physical address
    }
    if ( (u32Addr & EFLASH_PAGE_OFFSET_MASK) != 0)
    {
        return EFC_HEAD_ADDR_INVALID; // u32Addr is not flash page head address
    }

    // Write
    EFC_Write1Page(u32Addr, u32DataBuf);

    // Check
    for ( i = 0; i < EFLASH_PAGE_SIZE / 4; i++)
    {
        if (u32DataBuf[i] != Read_M32(u32Addr + (i << 2)))
        {
            return EFC_CHECK_ERROR;
        }
    }
    return OK;
}

/**
 * @brief     EFC_WriteInside1Page: Write n bytes in one page
 * @DateTime  2022-05-04T14:58:09+0800
 * @param     u32Addr                   write address
 * @param     u32Len                    the number of write data
 * @param     u8DataBuf                 pointer to write data buffer
 * @return                              write result
 *                                      = EFC_PAGE_ADDR_INVALID: page address is not valid
 *                                      = EFC_CHECK_ERROR: check error
 *                                      = OK: write and check success
 */
uint32_t EFC_WriteInside1Page(uint32_t u32Addr, uint32_t u32Len, uint8_t *u8DataBuf)
{
    uint32_t flash_start_addr = 0;
    uint32_t flash_addr_offset = 0;
    uint32_t i = 0, row = 0;
    uint32_t u32LenInt = 0;
    uint32_t act_addr = 0;

    act_addr = EFC_Get_ActualAddress(u32Addr);
    if(act_addr == EFC_PAGE_ADDR_INVALID)
        return EFC_PAGE_ADDR_INVALID;

    flash_start_addr      = u32Addr & EFLASH_PAGE_ADDR_MASK;
    flash_addr_offset     = u32Addr & EFLASH_PAGE_OFFSET_MASK;

    // Read
    for (i = 0; i < (EFLASH_PAGE_SIZE/4); i++)
    {
       flash_buf.flash_buf_32[i] = Read_M32(flash_start_addr + i*4);
    }

    // Change
    u32LenInt = u32Len;
    if (flash_addr_offset + u32Len > EFLASH_PAGE_SIZE)
    {
        u32LenInt = EFLASH_PAGE_SIZE - flash_addr_offset;
    }
    for ( i = 0; i < u32LenInt; i++ )
    {
        flash_buf.flash_buf_8[i + flash_addr_offset] = u8DataBuf[i];
    }

    EFC->PROTECT = 0xEDCB1234;
    EFC->PROTECT = 0xA9875678;
    EFC->PROTECT = 0x65439ABC;

    //Erase
    EFC->ADDR = act_addr & EFLASH_PAGE_ADDR_MASK;
    EFC->CON = EFC_FCMD_ERASE;
    EFC->WDATA = 0xFFFFFFFF;
    while((EFC->STA & 0x00000001) != 0x00000001){};

    //Write Back
    for(row = 0; row < PAGE_ROW_NUM; row++)
    {
        for(i = 0; i < ROW_WORD_NUM; i++)
        {
             EFC_BUF->BUF[i] = *((uint32_t *)&flash_buf.flash_buf_32[i + ROW_WORD_NUM* row]);
        }
        EFC->FPROG_LEN = i;
        EFC->CON = EFC_FCMD_FPROGRAM;
        EFC->ADDR = ((act_addr & EFLASH_PAGE_ADDR_MASK) + (row * ROW_WORD_NUM* 4));
        EFC->WDATA = 0xFFFFFFFF;
        while((EFC->STA & 0x00000001) != 0x00000001){};
    }

    EFC->PROTECT = 0x00000000;

    //Check
    for (i = 0; i < u32LenInt; i++)
    {
        if (u8DataBuf[i] != Read_M8(u32Addr + i))
        {
            return EFC_CHECK_ERROR;
        }
    }
    return OK;
}

/**
 * @brief     EFC_ReWrite: Rewrite n bytes in flash area
 * @DateTime  2022-05-04T15:01:31+0800
 * @param     u32Addr                   write address
 * @param     u32Len                    the number of rewrite data
 * @param     u8DataBuf                 pointer to write data buffer
 * @return                              write result
 *                                      = EFC_PAGE_ADDR_INVALID: page address is not valid
 *                                      = EFC_CHECK_ERROR: check error
 *                                      = OK: write and check success
 */
uint32_t EFC_ReWrite(uint32_t u32Addr, uint32_t u32Len, uint8_t *u8DataBuf)
{
    uint32_t ll = 0;
    uint32_t paddr = 0;
    uint32_t poffset = 0;
    uint8_t *u8Buf;

    ll      = u32Len;
    u8Buf   = u8DataBuf;
    paddr   = u32Addr;
    poffset = u32Addr & EFLASH_PAGE_OFFSET_MASK;

    // Check Addr
    if (!(((u32Addr < CODEFLASH_ADDR_END)) || ((u32Addr >= INFOFLASH_ADDR_BEGIN) && (u32Addr < INFOFLASH_ADDR_END))))
    {
        return EFC_PAGE_ADDR_INVALID; // u32Addr is beyond of physical address
    }

    // Write
    while (ll)
    {
        if (EFC_WriteInside1Page(paddr, ll, u8Buf) != OK)
        {
            return EFC_CHECK_ERROR;
        }
        paddr &= EFLASH_PAGE_ADDR_MASK;
        if (poffset + ll > EFLASH_PAGE_SIZE)
        {
            ll -= (EFLASH_PAGE_SIZE - poffset);
            u8Buf += (EFLASH_PAGE_SIZE - poffset);
        }
        else
        {
            ll = 0;
        }
        poffset = 0;
        paddr += EFLASH_PAGE_SIZE;
    }
    return OK;
}

/**
 * @brief     EFC_Read : read n bytes from flash
 * @DateTime  2022-05-04T15:03:33+0800
 * @param     u32Addr                   read address
 * @param     u32Len                    the number of read data
 * @param     u32DataBuf                pointer to read data buffer
 */
void EFC_Read(uint32_t u32Addr, uint32_t u32Len, uint32_t *u32DataBuf)
{
    uint32_t i = 0;
    // Read
    for (i = 0; i < (u32Len / 4 + ((u32Len & 3) != 0)); i++)
    {
        u32DataBuf[i] = Read_M32(u32Addr + (i << 2));
    }
}

/**
 * @Breif  : EFC_Set_Main_Ro_Addr: Set Read-Only address line
 *                                 The flash region below this address line will be changed into Read-Only
 * @DateTime  2022-05-04T15:21:30+0800
 * @param     u8PageNum                 page number (1 page number = 8192 Bytes address)
 * @return                              set result
 *                                      = EFC_PAGE_ADDR_INVALID: page number is not valid
 *                                      = OK: set success
 */
uint32_t EFC_Set_Main_Ro_Addr(uint8_t u8PageNum)
{
    if(u8PageNum > CODEFLASH_PAGE_END)
    {
        return EFC_PAGE_ADDR_INVALID;
    }
    else
    {
        EFC->PROTECT = 0xEDCB1234;
        EFC->PROTECT = 0xA9875678;
        EFC->PROTECT = 0x65439ABC;
        EFC->RO_NUM = ((EFC->RO_NUM & ~EFC_RO_MAIN_Msk) | (u8PageNum << EFC_RO_MAIN_Pos));
        EFC->PROTECT = 0x00000000;
    }
    return OK;
}

/**
 * @brief     EFC_Set_Info_Ro_Addr: Set Read-Only address line
 *                                  The flash region below this address line will be changed into Read-Only
 * @DateTime  2022-05-04T15:25:41+0800
 * @param     u8PageNum                 page number (1 page number = 8192 Bytes address)
 * @return                              set result
 *                                      = EFC_PAGE_ADDR_INVALID: page number is not valid
 *                                      = OK: set success
 */
uint32_t EFC_Set_Info_Ro_Addr(uint8_t u8PageNum)
{
    if(u8PageNum > INFOFLASH_PAGE_END)
    {
        return EFC_PAGE_ADDR_INVALID;
    }
    else
    {
        EFC->PROTECT = 0xEDCB1234;
        EFC->PROTECT = 0xA9875678;
        EFC->PROTECT = 0x65439ABC;
        EFC->RO_NUM = ((EFC->RO_NUM & ~EFC_RO_INFO_Msk) | (u8PageNum << EFC_RO_INFO_Pos));
        EFC->PROTECT = 0x00000000;
    }
    return OK;
}

/**
 * @brief     EFC_Set_Boot_Addr : Set the top address of bootloader area
 * @DateTime  2022-05-04T15:29:27+0800
 * @param     u8PageNum                 page number (1 page number = 8192 Bytes address)
 * @return                              set result
 *                                      = EFC_PAGE_ADDR_INVALID: page number is not valid
 *                                      = OK: set success
 */
uint32_t EFC_Set_Boot_Addr(uint8_t u8PageNum)
{
    if(u8PageNum > CODEFLASH_PAGE_END)
    {
        return EFC_PAGE_ADDR_INVALID;
    }
    else
    {
        EFC->PROTECT = 0xEDCB1234;
        EFC->PROTECT = 0xA9875678;
        EFC->PROTECT = 0x65439ABC;
        EFC->BOOT_NUM = u8PageNum;
        EFC->PROTECT = 0;
    }
    return OK;
}

/*** (C) COPYRIGHT 2022 SKYRELAY Technology co.,Ltd ***/
