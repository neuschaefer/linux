/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

//============================================================================
// NOTE:
//      In this file, all function cannot call sub-function!!!
//============================================================================
#if 0
#if defined(CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER)

#include "x_hal_5381.h"


#define NAND_WRITE32(offset, value)     IO_WRITE32(BIM_BASE, (offset), (value))
#define NAND_READ32(offset)             IO_READ32(BIM_BASE, (offset))


#define NAND_ACCESS_CTRL_REG            0x300
    #define NAND_ACSCTRL_WDH(x)         (((UINT32) x & 0x0f) << 20)
    #define NAND_ACSCTRL_C2R(x)         (((UINT32) x & 0x0f) << 16)
    #define NAND_ACSCTRL_W2R(x)         (((UINT32) x & 0x0f) << 12)
    #define NAND_ACSCTRL_WH(x)          (((UINT32) x & 0x0f) <<  8)
    #define NAND_ACSCTRL_WST(x)         (((UINT32) x & 0x0f) <<  4)
    #define NAND_ACSCTRL_RLT(x)         (((UINT32) x & 0x0f) <<  0)
#define NAND_PGFMT_CTRL_REG             0x304
    #define NAND_PGFMT_ECCBLK(x)        (((UINT32) x & 0x07) << 4)
    #define NAND_ECC_BLK_128B           0
    #define NAND_ECC_BLK_256B           1
    #define NAND_ECC_BLK_512B           2
    #define NAND_ECC_BLK_1024B          3
    #define NAND_ADRMODE                ((UINT32) 1 << 2)
    #define NAND_PSIZE(x)               (((UINT32) x & 0x03) << 0)
    #define NAND_PSIZE_528B             0
    #define NAND_PSIZE_2112B            1
#define NAND_OP_CTRL_REG                0x308
    #define NAND_BURST_RDMODE           ((UINT32) 1 << 0)
    #define NAND_BURST_WRMODE           ((UINT32) 1 << 1)
    #define NAND_ECC_REAL_MODE          ((UINT32) 1 << 4)
    #define NAND_BLK_ECC_GEN            ((UINT32) 1 << 5)
    #define NAND_SINGLE_RD_MODE         ((UINT32) 1 << 8)
    #define NAND_NOB(x)                 (((UINT32) x &0x03) << 12)
    #define NAND_NOB_4B                 0
    #define NAND_NOB_1B                 1
    #define NAND_NOB_2B                 2
    #define NAND_NOB_3B                 3
#define NAND_CMD_REG                    0x30c
    #define NAND_CMD_READ1_00           0x00
    #define NAND_CMD_READ1_01           0x01
    #define NAND_CMD_PROG_PAGE          0x10    /* WRITE 2 */
    #define NAND_CMD_READ2              0x50
    #define NAND_CMD_ERASE1_BLK         0x60
    #define NAND_CMD_STATUS             0x70
    #define NAND_CMD_INPUT_PAGE         0x80    /* WRITE 1 */
    #define NAND_CMD_READ_ID            0x90
    #define NAND_CMD_ERASE2_BLK         0xD0
    #define NAND_CMD_RESET              0xFF
#define NAND_ADDR_LEN_REG               0x310
#define NAND_ADDR_3_0_REG               0x314
#define NAND_ADDR_4_REG                 0x318
#define NAND_U4_WR_BUF_REG              0x31c
#define NAND_U1_WR_BUF_REG              0x320
#define NAND_U4_RD_BUF_REG              0x324
#define NAND_U1_RD_BUF_REG              0x328
#define NAND_STATUS_REG                 0x32c
    #define NAND_STATUS_BUSY            ((UINT32) 1 << 8)
    #define NAND_STATUS_DTWR            ((UINT32) 1 << 3)
    #define NAND_STATUS_DTRD            ((UINT32) 1 << 2)
    #define NAND_STATUS_ADDR            ((UINT32) 1 << 1)
    #define NAND_STATUS_CMD             ((UINT32) 1 << 0)
#define NAND_FIFO_CTRL_REG              0x330
    #define NAND_FIFO_WB_FULL           ((UINT32) 1 << 7)
    #define NAND_FIFO_RB_EMPTY          ((UINT32) 1 << 6)
    #define NAND_FIFO_RESET             ((UINT32) 1 << 5)
    #define NAND_FIFO_FLASH             ((UINT32) 1 << 4)
    #define NAND_FIFO_WR_FULL           ((UINT32) 1 << 3)
    #define NAND_FIFO_WR_EMPTY          ((UINT32) 1 << 2)
    #define NAND_FIFO_RD_FULL           ((UINT32) 1 << 1)
    #define NAND_FIFO_RD_EMPTY          ((UINT32) 1 << 0)
#define NAND_CTRL_REG                   0x334
    #define NAND_CTRL_MULTIPAGE         ((UINT32) 1 << 11)
    #define NAND_CTRL_READ              ((UINT32) 1 << 10)
    #define NAND_CTRL_PROGRAM           ((UINT32) 1 << 9)
    #define NAND_CTRL_ERASE             ((UINT32) 1 << 8)
    #define NAND_CTRL_DMAPAUSE          ((UINT32) 1 << 6)
    #define NAND_CTRL_PROGSPARE         ((UINT32) 1 << 5)
    #define NAND_CTRL_MULTI_RDBPG       ((UINT32) 1 << 4)
    #define NAND_CTRL_AUTOECC_ENCODE    ((UINT32) 1 << 3)
    #define NAND_CTRL_AUTOECC_DECODE    ((UINT32) 1 << 2)
    #define NAND_CTRL_DMA_WR            ((UINT32) 1 << 1)
    #define NAND_CTRL_DMA_RD            ((UINT32) 1 << 0)
#define NAND_INTR_STATUS_REG            0x338
#define NAND_READ_ACCESS_REG            0x3B4

//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitBusy(UINT32 u4Flag)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_STATUS_REG);
    } while ((u4Val & u4Flag) != 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Command(UINT32 u4Cmd)
{
    NAND_WRITE32(NAND_CMD_REG, u4Cmd);

    _NANDHW_WaitBusy(NAND_STATUS_CMD);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Addr(UINT32 u4Addr, UINT32 u4ByteCount)
{
    NAND_WRITE32(NAND_ADDR_3_0_REG, u4Addr);
    NAND_WRITE32(NAND_ADDR_LEN_REG, u4ByteCount);

    _NANDHW_WaitBusy(NAND_STATUS_ADDR);
}

#if !defined(CC_SMALL_NAND)
//-----------------------------------------------------------------------------
/** CHIP_NANDCopyToDRAM()
 */
//-----------------------------------------------------------------------------
void CHIP_NANDCopyToDRAM(UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len)
{
    UINT32 i, u4Val, u4NANDPgIdx;
    UINT32 u4ReadByte;
    // Clear interrupt
    NAND_READ32(NAND_INTR_STATUS_REG);

    // Change clock frequency
    NAND_WRITE32(NAND_ACCESS_CTRL_REG, 0x10277);
    NAND_WRITE32(NAND_READ_ACCESS_REG, 0x8);

    // Set arbitor
    NAND_WRITE32(0xC4, 0x3F);

    NAND_WRITE32(NAND_PGFMT_CTRL_REG, NAND_PSIZE(NAND_PSIZE_2112B) | NAND_ADRMODE |
                 NAND_PGFMT_ECCBLK(NAND_ECC_BLK_512B));

    NAND_WRITE32(NAND_OP_CTRL_REG, 0);
    NAND_WRITE32(NAND_FIFO_CTRL_REG, NAND_FIFO_RESET);
    NAND_WRITE32(NAND_CTRL_REG, 0x0);

    u4NANDPgIdx = (u4StartOffset/2048);
    do
    {
        _NANDHW_Command(NAND_CMD_READ1_00);
        _NANDHW_Addr((u4NANDPgIdx<<16)+(u4StartOffset&0x7FF), 4);
        _NANDHW_Command(0x30);
        NAND_WRITE32(NAND_OP_CTRL_REG, NAND_BURST_RDMODE);
        _NANDHW_WaitBusy(NAND_STATUS_BUSY | NAND_STATUS_DTRD);

        if (u4StartOffset&0x7ff)
        {
            u4ReadByte = (u4StartOffset&0x7ff);
        }
        else
        {
            u4ReadByte = 2048;
        }
        if (u4Len < u4ReadByte)
        {
            u4ReadByte = u4Len;
        }
        for(i = 0; i < u4ReadByte/4; i++)
        {
            do
            {
                u4Val = NAND_READ32(NAND_FIFO_CTRL_REG);
            } while (u4Val & NAND_FIFO_RD_EMPTY);
            *pu4DRAMBuf = NAND_READ32(NAND_U4_RD_BUF_REG);
            pu4DRAMBuf++;
        }
        NAND_WRITE32(NAND_OP_CTRL_REG, 0);
        u4NANDPgIdx++;
        u4Len -= u4ReadByte;
        u4StartOffset += u4ReadByte;
    }while(u4Len);
}

#else
void CHIP_NANDCopyToDRAM(UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len)
{
    UINT32 i, u4Val, u4NANDPgIdx;
    UINT32 u4ReadByte;

    // Clear interrupt
    NAND_READ32(NAND_INTR_STATUS_REG);

    // Change clock frequency
    NAND_WRITE32(NAND_ACCESS_CTRL_REG, 0x104AA);
    NAND_WRITE32(NAND_READ_ACCESS_REG, 0xB);

    // Set arbitor
    NAND_WRITE32(0xC4, 0x3F);

    NAND_WRITE32(NAND_PGFMT_CTRL_REG, NAND_PSIZE(NAND_PSIZE_528B)
                | NAND_PGFMT_ECCBLK(NAND_ECC_BLK_512B));

    NAND_WRITE32(NAND_OP_CTRL_REG, 0);
    NAND_WRITE32(NAND_FIFO_CTRL_REG, NAND_FIFO_RESET);
    NAND_WRITE32(NAND_CTRL_REG, 0x0);

    u4NANDPgIdx = u4StartOffset/512;

    do
    {
        _NANDHW_Command(NAND_CMD_READ1_00);
        _NANDHW_Addr((u4NANDPgIdx<<8)+(u4StartOffset&0x1FF), 4);
        NAND_WRITE32(NAND_OP_CTRL_REG, NAND_BURST_RDMODE);
        _NANDHW_WaitBusy(NAND_STATUS_BUSY | NAND_STATUS_DTRD);
        if (u4StartOffset&0x1ff)
        {
            u4ReadByte = (u4StartOffset&0x1ff);
        }
        else
        {
            u4ReadByte = 512;
        }
        if (u4Len < u4ReadByte)
        {
            u4ReadByte = u4Len;
        }
        for(i = 0; i < u4ReadByte/4; i++)
        {
            do
            {
                u4Val = NAND_READ32(NAND_FIFO_CTRL_REG);
            } while (u4Val & NAND_FIFO_RD_EMPTY);
            *pu4DRAMBuf = NAND_READ32(NAND_U4_RD_BUF_REG);
            pu4DRAMBuf++;
        }
        NAND_WRITE32(NAND_OP_CTRL_REG, 0);
        u4NANDPgIdx++;
        u4Len -= u4ReadByte;
        u4StartOffset += u4ReadByte;
    }while(u4Len);



}
#endif

//-----------------------------------------------------------------------------
/** CHIP_ITCMCopyToDRAM()
 */
//-----------------------------------------------------------------------------
void CHIP_ITCMCopyToDRAM()
{
    UINT32 *pITCM, *pDRAM;
    UINT32 i;

    pITCM = (UINT32*)0x0;
    pDRAM = (UINT32*)0x10000000;

    // Copy 8KBytes data from ITCM to DRAM
    for(i=0; i<(0x2000/4); i++)
    {
        pDRAM[i] = pITCM[i];
    }
}

#endif /* defined(CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER) */
#else
#include "x_typedef.h"
#include "x_assert.h"
#include "nandboot.h"

#if defined(CC_NAND_BOOT) || !defined(CC_MTK_PRELOADER)
//#if defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER)
#if defined(CC_MTK_PRELOADER)
void CHIP_DisplayString(const INT8 *szString);
void CHIP_DisplayHex(UINT32 u4Val);
#else
#define CHIP_DisplayString(x)
#define CHIP_DisplayHex(x)
#endif


typedef struct
{
    //CHAR acName[NAND_NAME_MAX_SIZE];
    UINT8  u1AddrCycle;
    UINT32 u4IDVal;
    UINT32 u4IDMask;
    UINT32 u4PageSize;
    //UINT32 u4OOBSize;
    UINT32 u4BlkSize;
    //UINT32 u4EraseSize;
    //UINT32 u4BlkPgCount;
    //UINT32 u4ChipSize;
    UINT32 u4Timing;
} NAND_FLASH_DEV_BOOT_T;

const NAND_FLASH_DEV_BOOT_T _arNand_BootInfo[] =
{
    { 3, 0x00007300, 0x0000FF00, 512,  0x4000/512, 0x104AA},  // Unknow 16 MB
    { 3, 0x00007500, 0x0000FF00, 512,  0x4000/512, 0x104AA},  // Unknow 32MB
    { 4, 0x00007000, 0x0000F000, 512,  0x4000/512, 0x104AA},  // Unknow >= 64MB 0x76
    { 4, 0x0000F100, 0x0000FF00, 2048, 0x20000/2048, 0x10277},   // Unknow 128MB
    { 5, 0x00000000, 0x00000000, 2048, 0x20000/2048, 0x10388},   // Unknow >= 128MB

//    { 4, 0x0000F100, 0x0000FF00, 2048, 0x20000/2048, 0x10277},   // Unknow 128MB
//    { 5, 0x00000000, 0x00000000, 2048, 0x20000/2048, 0x10277},   // Unknow >= 128MB
};
UINT32 _Nand_DevIdex;
NAND_FLASH_DEV_BOOT_T *_pNandInfo;
UINT8 _NAND_BlkInfo[4];
//UINT32 _NAND_PageBuf[512];
//#define _NAND_PageBuf NAND_DMX_SRAM_BUFF_BASE

VOID CHIP_NandShowHex(const CHAR *pu1Str, UINT32 u4Hex)
{
    UNUSED(pu1Str);
    UNUSED(u4Hex);
    CHIP_DisplayString((const INT8 *)pu1Str);
    CHIP_DisplayHex(u4Hex);
    CHIP_DisplayString("\n");
}
/*----------------------------------------------------------------------------
 * Function: CHIP_memcpy()
 *
 * Description:
 *      this API copies pv_from to pv_to up to z_l bytes. it performs
 *      1. parse argument and return pv_to on error.
 *      2. process to make the most UINT32 copies, then UINT16 copies, then
 *         UINT8 copies.
 *
 * Inputs:
 *      pv_to : memory block to be copied to.
 *      pv_from : memory block to be copied from.
 *      z_l : bytes to be copied
 *
 * Outputs: -
 *
 * Returns:
 *      pointer to pv_to
 *---------------------------------------------------------------------------*/
static void *CHIP_memcpy(void *pv_to, const void *pv_from, UINT32 z_l)
{
    UINT32 *pui4_t, *pui4_f;
    UINT32 i4_i, i4_j, i4_k;
    CHAR *ps_c, *ps_f;

    if ((pv_to == (CHAR *) NULL) || (pv_from == (CHAR *) NULL) || (z_l == 0))
    {
        return(pv_to);
    }

    i4_i = ((UINT32) pv_to) & 0x3;      /* off from INT32 boundary */
    i4_j = ((UINT32) pv_from) & 0x3;
    if (i4_i == i4_j)           /* check for INT32 assignment */
    {
        if (i4_i & 0x1)         /* off 1 or 3 bytes mean 3 or 1 leading bytes */
        {
            i4_i = 4 - i4_i;    /* leading bytes to next INT32 aligned */
        }
        i4_j = z_l - i4_i;      /* remaining bytes excl. leading bytes */
        i4_k = i4_j >> 2;       /* count of INT32 */
    }
    else
    {
        i4_k = 0;
    }

    if (i4_k > 0)               /* do INT32 assignment */
    {
        pui4_t = (UINT32 *) pv_to;
        pui4_f = (UINT32 *) pv_from;

        /* leading bytes */
        if (i4_i & 0x1)    /* 1 or 3 leading bytes  */
        {
            *((CHAR *) pui4_t) = *((CHAR *) pui4_f);
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 1);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 1);
        }
        if (i4_i & 0x2)    /* 2 or 3 leading bytes */
        {
            *((UINT16 *)((UINT32)pui4_t)) = *((UINT16 *) ((UINT32)pui4_f));
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 2);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 2);
        }

        do      /* do INT32 assignment */
        {
            *pui4_t++ = *pui4_f++;
        } while (--i4_k > 0);

        /* finish the remaining bytes */
        if (i4_j & 0x2)      /* do an INT16 assignment if possible */
        {
            *((UINT16 *) ((UINT32)pui4_t)) = *((UINT16 *) ((UINT32)pui4_f));
            pui4_t = (UINT32 *) (((UINT32) pui4_t) + (UINT32) 2);
            pui4_f = (UINT32 *) (((UINT32) pui4_f) + (UINT32) 2);
        }
        if (i4_j & 0x1)      /* do byte assignment */
        {
            *(CHAR *) pui4_t = *(CHAR *) pui4_f;
        }
        return(pv_to);
    }
    else
    {
        ps_c = (CHAR *) pv_to;
        ps_f = (CHAR *) pv_from;
        do
        {
            *ps_c++ = *ps_f++;
        } while (--z_l > 0);

        return(pv_to);
    }
}
//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_WaitBusy(void)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_STATUS_REG);
        u4Val &= 0x10F;
    }
    while ((u4Val) != 0);
    //while ((u4Val & u4Flag) != 0);
}
//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_WaitData(void)
{
UINT32 u4Val;
     do
     {
           u4Val = NAND_READ32(NAND_FIFO_CTRL_REG);
     } while (u4Val & NAND_FIFO_RD_EMPTY);
}
//-----------------------------------------------------------------------------
/** _NANDHW_ResetFifo()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_ResetFifo(void)
{
    NAND_WRITE32(NAND_OP_CTRL_REG, 0);
    NAND_WRITE32(NAND_FIFO_CTRL_REG, NAND_FIFO_RESET);
    NAND_WRITE32(NAND_CTRL_REG, 0x0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Entry()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_Entry(void)
{
UINT32 u4Temp;
    // Clear interrupt
    u4Temp = NAND_READ32(NAND_INTR_STATUS_REG);
    UNUSED(u4Temp);
   NAND_WRITE32(0x3B0,NAND_READ32(0x3B0)|0x4);
    // Set arbitor
    NAND_WRITE32(0xC4, 0x3F);
    CHIP_NANDHW_ResetFifo();
}
//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_Command(UINT32 u4Cmd)
{
    NAND_WRITE32(NAND_CMD_REG, u4Cmd);

    CHIP_NANDHW_WaitBusy();
}
//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_Addr(UINT32 u4Addr, UINT32 u4ByteCount)
{
    if(u4ByteCount == 5)
    {
      NAND_WRITE32(NAND_ADDR_3_0_REG, (UINT32)(u4Addr<<16));
      NAND_WRITE32(NAND_ADDR_4_REG, u4Addr>>16);
    }
    else
    {
      NAND_WRITE32(NAND_ADDR_3_0_REG, u4Addr);
    }
    NAND_WRITE32(NAND_ADDR_LEN_REG, u4ByteCount);
    CHIP_NANDHW_WaitBusy();
}
//-----------------------------------------------------------------------------
/** void NANDHW_ReadID(UINT32 *pu4ID)
 */
//-----------------------------------------------------------------------------
static void CHIP_NANDHW_ReadID(UINT32 *pu4ID)
{
    ASSERT(pu4ID != NULL);
    CHIP_NANDHW_Command(NAND_CMD_READ_ID);
    CHIP_NANDHW_Addr(0x0, 1);

    NAND_WRITE32(NAND_OP_CTRL_REG, NAND_NOB(NAND_NOB_4B) | NAND_SINGLE_RD_MODE);
    CHIP_NANDHW_WaitBusy();
    *pu4ID = NAND_READ32(NAND_U4_RD_BUF_REG);
}
//-----------------------------------------------------------------------------
/** _ReadCtrlECCLarge()
 */
//-----------------------------------------------------------------------------
static void CHIP_ReadCtrlECCLarge(UINT32 *pu4Data)
{
#if 0
    if (NAND_READ32(0x3B0)& 0x80)
    {
        pu4Data[0] = ((NAND_READ32(NAND_ECC_PARITY0_REG) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 4) & 0xFFF) << 16));
        pu4Data[1] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 8) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 12) & 0xFFF) << 16));
        pu4Data[2] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 16) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 20) & 0xFFF) << 16));
        pu4Data[3] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 24) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 28) & 0xFFF) << 16));
    }
    else
    {
        pu4Data[0] = ~((NAND_READ32(NAND_ECC_PARITY0_REG) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 4) & 0xFFF) << 16));

        pu4Data[1] = ~((NAND_READ32(NAND_ECC_PARITY0_REG + 8) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 12) & 0xFFF) << 16));

        pu4Data[2] = ~((NAND_READ32(NAND_ECC_PARITY0_REG + 16) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 20) & 0xFFF) << 16));

        pu4Data[3] = ~((NAND_READ32(NAND_ECC_PARITY0_REG + 24) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 28) & 0xFFF) << 16));
    }
#endif
        ASSERT(pu4Data != NULL);
        pu4Data[0] = ((NAND_READ32(NAND_ECC_PARITY0_REG) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 4) & 0xFFF) << 16));
        pu4Data[1] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 8) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 12) & 0xFFF) << 16));
        pu4Data[2] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 16) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 20) & 0xFFF) << 16));
        pu4Data[3] = ((NAND_READ32(NAND_ECC_PARITY0_REG + 24) & 0xFFF) |
                  ((NAND_READ32(NAND_ECC_PARITY0_REG + 28) & 0xFFF) << 16));
        if ((NAND_READ32(0x3B0)& 0x80) == 0)
        {
           pu4Data[0] = ~pu4Data[0];
           pu4Data[1] = ~pu4Data[1];
           pu4Data[2] = ~pu4Data[2];
           pu4Data[3] = ~pu4Data[3];
        }
}
#if 0
//-----------------------------------------------------------------------------
/** void CHIP_ARMDtcmInit(UINT32 dAddr)
 */
//-----------------------------------------------------------------------------
void CHIP_ARMDtcmInit(UINT32 dAddr)
{
   asm{
          MOV     r1, dAddr
          MRC     p15, 0, r0, c9, c1, 1
          AND     r0, r0, #1
          CMP     r0, #0
          ORR     r0, r1, #0
          MCR     p15, 0, r0, c9, c1, 1
   }
}
#endif
//-----------------------------------------------------------------------------
/** void CHIP_NANDInit(void)
 */
//-----------------------------------------------------------------------------
void CHIP_NANDInit(void)
{
UINT32 u4ID;
    //CHIP_ARMDtcmInit(0x2b000000);
//    if(NAND_READ32(NAND_ACCESS_CTRL_REG)!=0x222222)
//    {
//         NandShowHex("NT=", NAND_READ32(NAND_ACCESS_CTRL_REG));
//         return;
//    }
    NAND_WRITE32(NAND_ACCESS_CTRL_REG, 0x2224AA);
    NAND_WRITE32(NAND_READ_ACCESS_REG, 0xB);
    CHIP_NANDHW_Entry();
    CHIP_NANDHW_ReadID(&u4ID);
    CHIP_NandShowHex("NID=", u4ID);
    for(_Nand_DevIdex = 0; _Nand_DevIdex < (sizeof(_arNand_BootInfo)/sizeof(NAND_FLASH_DEV_BOOT_T)); _Nand_DevIdex++)
    {
        if(_arNand_BootInfo[_Nand_DevIdex].u4IDVal == (u4ID & _arNand_BootInfo[_Nand_DevIdex].u4IDMask))
        {
            break;
        }
    }
    //ShowHex("NAND default=", u4ID);
    _pNandInfo = (NAND_FLASH_DEV_BOOT_T *)&_arNand_BootInfo[_Nand_DevIdex];
    NAND_WRITE32(NAND_ACCESS_CTRL_REG, _pNandInfo->u4Timing);
    if(_pNandInfo->u4PageSize == 512)
    {
      NAND_WRITE32(NAND_READ_ACCESS_REG, 0xB);
      NAND_WRITE32(NAND_PGFMT_CTRL_REG, NAND_PSIZE(NAND_PSIZE_528B)
                | NAND_PGFMT_ECCBLK(NAND_ECC_BLK_512B));
    }
    else
    {
      NAND_WRITE32(NAND_READ_ACCESS_REG, 0x8);
      NAND_WRITE32(NAND_PGFMT_CTRL_REG, NAND_PSIZE(NAND_PSIZE_2112B) | NAND_ADRMODE |
                    NAND_PGFMT_ECCBLK(NAND_ECC_BLK_512B));
    }
    CHIP_NandShowHex("NT=", NAND_READ32(NAND_ACCESS_CTRL_REG));
}
//-----------------------------------------------------------------------------
/** _NANDHW_CorrectECC()
 */
//-----------------------------------------------------------------------------
static UINT32 CHIP_countbits(UINT32 byte)
{
	UINT32 res = 0;

	for (;byte; byte >>= 1)
	{
         res = res + (byte & 0x01);
	}
	return res;
}
//-----------------------------------------------------------------------------
/** CHIP_CorrectBit()
 */
//-----------------------------------------------------------------------------
static void CHIP_CorrectBit(UINT32 u4Addr, UINT32 *pu4Buf)
{
UINT32 u4DwrdIdx, u4BitIdx, u4Data;
      ASSERT(pu4Buf != NULL);
	u4DwrdIdx = (u4Addr & 0xFFF)>> 5 ;
	u4BitIdx = (u4Addr & 0x1F);
	u4Data = pu4Buf[u4DwrdIdx];
      if((u4Data & (1<<u4BitIdx)) == 0)
      {
          pu4Buf[u4DwrdIdx] |= (1<<u4BitIdx);
      }
      else
      {
         pu4Buf[u4DwrdIdx] &= (~(1U<<u4BitIdx));
      }
}

static UINT32 CHIP_CorrectECC(UINT32 u4CalECC, UINT32 u4ReadECC, UINT32* pbuf)
{
    UINT32 tmp1, tmp2, tmp11, tmp22, tmp3, tmp4;
//    UINT8 data;

    u4CalECC = ~u4CalECC;
    tmp1 = u4CalECC & 0xFFF;
    tmp2 = (u4CalECC >> 16) & 0xFFF;

    u4ReadECC = ~u4ReadECC;
    tmp11 = u4ReadECC & 0xFFF;
    tmp22 = (u4ReadECC >> 16) & 0xFFF;

    tmp3 = tmp1 ^ tmp11;
    tmp4 = tmp2 ^ tmp22;

    if((0xFFF & ((~tmp3) ^ tmp4)) != 0)
	{
		if (1 == (CHIP_countbits(tmp3) + CHIP_countbits(tmp4)))
		{
			// ECC code error
            CHIP_NandShowHex("1 bits err:", tmp4);
			return 0;
		}
		else {
			// error, not correctable
	        CHIP_DisplayString("m bits err\n");
			return 1;
        }
	}

    CHIP_CorrectBit(tmp4, (UINT32 *)pbuf);
    /*
    tmp1 = (tmp4 >> 3) & 0x1FF;     // addr offset
    tmp2 = tmp4 & 0x7;              // data offset
    data = pbuf[tmp1];
    if((data & (1<<tmp2)) == 0)
    {
        pbuf[tmp1] |= (1<<tmp2);
    }
    else
    {
        pbuf[tmp1] &= (~(1<<tmp2));
    }
    */
    return 0;
}

static UINT32 CHIP_NANDHW_ReadPageToDRAM(UINT32 *pu4Buf, UINT32 u4PageIdx, UINT32 u4ByteCnt)
{
    UINT32 i, j;
    //UINT8 *pu1Buf;
    //UINT32 u4Size;
    UINT32 au4EccReg[4], au4EccRead[4]={0,0,0,0}, au4Spare[4];
    ASSERT(pu4Buf != NULL);
    UNUSED(u4ByteCnt);
    CHIP_NANDHW_WaitBusy();
    CHIP_NANDHW_ResetFifo();
    NAND_WRITE32(NAND_CTRL_REG, NAND_CTRL_PROGSPARE);
    CHIP_NANDHW_Command(NAND_CMD_READ1_00);
    //if(_pNandInfo->u1AddrCycle == 512)
    if(_pNandInfo->u4PageSize == 512)
    {
       CHIP_NANDHW_Addr( u4PageIdx<<8, _pNandInfo->u1AddrCycle);
    }
    else
    {
       if(_pNandInfo->u1AddrCycle == 4)
       {
          CHIP_NANDHW_Addr( u4PageIdx<<16, _pNandInfo->u1AddrCycle);
       }
       else
       {
          CHIP_NANDHW_Addr( u4PageIdx, _pNandInfo->u1AddrCycle);
       }
      CHIP_NANDHW_Command(NAND_CMD_READ1_30);

    }
    NAND_WRITE32(NAND_OP_CTRL_REG, NAND_BURST_RDMODE);
    CHIP_NANDHW_WaitBusy();
    for(i = 0; i < (_pNandInfo->u4PageSize); i+=4)
    {
        CHIP_NANDHW_WaitData();
        pu4Buf[i>>2] = NAND_READ32(NAND_U4_RD_BUF_REG);
    }

    CHIP_ReadCtrlECCLarge(au4EccReg);

    // Read spare area
    for (i = 0; i<(_pNandInfo->u4PageSize>>9); i++)
    {
	    for(j = 0; j< 4; j++)
	    {
              CHIP_NANDHW_WaitData();
	        au4Spare[j] = NAND_READ32(NAND_U4_RD_BUF_REG);
	        /*
	        if(j == 1)
	        {
	           au4EccRead[i] = NAND_READ32(NAND_U4_RD_BUF_REG)>>24;
	        }
	        else if(j == 2)
	        {
	          au4EccRead[i] = au4EccRead[i] | (NAND_READ32(NAND_U4_RD_BUF_REG)<<8);
	        }
	        else
	        {
	           u4Val = NAND_READ32(NAND_U4_RD_BUF_REG);
	        }
	        */
	    }
	    if(_pNandInfo->u4PageSize == 512)
	    {
	        _NAND_BlkInfo[i] = (UINT8)(au4Spare[1] >> 8);
	    }
	    else
	    {
             _NAND_BlkInfo[i] = (UINT8)(au4Spare[0]);
	    }
	    au4EccRead[i] = (au4Spare[1] >>24) | (au4Spare[2] <<8);
    }
    CHIP_NANDHW_WaitBusy();

    NAND_WRITE32(NAND_OP_CTRL_REG, 0);

    // Compare ECC
    for(i=0; i<(_pNandInfo->u4PageSize>>9); i++)
    {
            if(((0x0FFF0FFFU) & au4EccRead[i]) != ((0x0FFF0FFFU) & au4EccReg[i]) )
            {
                CHIP_NandShowHex(" Page", u4PageIdx);
                CHIP_NandShowHex("  HW ECC", au4EccReg[i]);
                CHIP_NandShowHex("  NAND ECC", au4EccRead[i]);
                if(CHIP_CorrectECC( ((0x0FFF0FFFU) & au4EccReg[i]), ((0x0FFF0FFFU) & au4EccRead[i]), (&pu4Buf[i*128])) != 0)
                {
                   return 1;
                }
            	}
    	}

    return 0;
}


static void CHIP_NANDHW_IsBadBlock(UINT32 *u4PageIdx, UINT32 *u4PageBuf)
{
//UINT32 *u4PageBuf=(UINT32 *)_NAND_PageBuf;
//UINT32 u4OrgIdx = *u4PageIdx;
UINT32 i;
   ASSERT(u4PageIdx != NULL);
   for(i = 0 ; i < 0x200; i++)
   {
      if(CHIP_NANDHW_ReadPageToDRAM(u4PageBuf, *u4PageIdx, _pNandInfo->u4PageSize))
      {
      };
      if(CHIP_countbits(_NAND_BlkInfo[0]) <7)
      {
         CHIP_NandShowHex("Bad Block:", *u4PageIdx);
         *u4PageIdx = (*u4PageIdx) + _pNandInfo->u4BlkSize;
      }
      else
      	{
		return;
      }
   }
}

void CHIP_NANDCopyToDRAMLZHS(UINT32 *pu4DstBuf, UINT32 u4ByteOffset , UINT32 u4Len)
{
	UINT32 u4PageIdxLog;
      UINT32 u4PageIdxPhy;
	UINT32 u4PageSize;
	UINT32 uPageMask;
	UINT32 u4PageOff;
	UINT32 u4ReadByte;
      //UINT32 *u4PageBuf=(UINT32 *)_NAND_PageBuf;
      UINT32 u4PageBuf[512];
      UINT32 i;
      CHIP_NANDInit();
	u4PageSize = _pNandInfo->u4PageSize;
	if(u4PageSize == 512)
	{
	    u4PageIdxLog = u4ByteOffset>>9;
	}
	else
	{
	    u4PageIdxLog = u4ByteOffset>>11;
	}

	u4PageIdxPhy = u4PageIdxLog;
      uPageMask = u4PageSize -1;
      u4ReadByte = u4PageSize;
      u4PageOff = uPageMask & u4ByteOffset;
      if(u4PageOff)
      	{
          u4ReadByte = u4PageSize - u4PageOff;
      }
      for(i = 0; i < u4Len; )
      	{
          if((u4ReadByte + i) > u4Len)
          {
              u4ReadByte = u4Len - i;
          }
          if((u4PageIdxPhy&(_pNandInfo->u4BlkSize-1)) == 0)
          {
             CHIP_NANDHW_IsBadBlock(&u4PageIdxPhy, u4PageBuf);
          }
          //_NANDHW_ReadPageToDRAM((UINT32 *)(u4PageBuf), u4PageIdxPhy, u4PageOff, u4ReadByte);
          //_NANDHW_ReadPageToDRAM((UINT32 *)(u4PageBuf), u4PageIdxPhy, u4ReadByte);
          if(CHIP_NANDHW_ReadPageToDRAM((UINT32 *)(u4PageBuf), u4PageIdxPhy, u4PageSize))
          {
          };
          //_NANDHW_MemCpy((void *)(pu4DstBuf + i), (void *)(u4PageBuf + u4PageOff), u4ReadByte);
          if(CHIP_memcpy((void *)((UINT32)pu4DstBuf + i), (void *)((UINT32)u4PageBuf + (UINT32)u4PageOff), u4ReadByte))
          {
          }
          i+=u4ReadByte;
          u4ReadByte = u4PageSize;
          u4PageOff = 0;
          u4PageIdxPhy++;
      	}
	return;
}

void CHIP_NANDCopyToDRAM(UINT32 *pu4DRAMBuf, UINT32 u4StartOffset, UINT32 u4Len)
{
      //CHIP_NANDInit();
	CHIP_NANDCopyToDRAMLZHS(pu4DRAMBuf, u4StartOffset, u4Len);
}
#endif
/*
static UINT32 au4NandArray[4];

UINT32 *CHIP_NandArray()
{
    return au4NandArray;
}
*/
#endif

