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

#if defined(CC_NAND_BOOT) || !defined(CC_5391_PRELOADER)

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

#endif /* defined(CC_NAND_BOOT) || !defined(CC_5391_PRELOADER) */

