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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nandhw_reg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nandhw_reg.c
 *  nandhw_reg.c provides the low level register access function of nand \n
 *  device.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifndef TOOLDRIVER  //TOOLDRIVER is for download tool used

#include "nand_debug.h"
#include "nandhw_io.h"
#include "nandhw_reg.h"
#include "nandhw_if.h"
#include "dmx_drm_if.h"
#include "x_lint.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "x_hal_5381.h"
#include "x_hal_arm.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_timer.h"
#include "x_printf.h"
#include "x_pinmux.h"
LINT_EXT_HEADER_END

#endif /* TOOLDRIVER */

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NAND_SCRAMBLE_EN            (0)
#define NAND_RANDOM_EN              (0)
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
   #if NAND_RANDOM_EN
   static  BOOL fgRandomizer = TRUE;
   static UINT16 RANDOM_SEED[256]=
   {
    //for page 0~127
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7,
   //for page 128~255
        0x576A, 0x05E8, 0x629D, 0x45A3, 0x649C, 0x4BF0, 0x2342, 0x272E,
        0x7358, 0x4FF3, 0x73EC, 0x5F70, 0x7A60, 0x1AD8, 0x3472, 0x3612,
        0x224F, 0x0454, 0x030E, 0x70A5, 0x7809, 0x2521, 0x484F, 0x5A2D,
        0x492A, 0x043D, 0x7F61, 0x3969, 0x517A, 0x3B42, 0x769D, 0x0647,
        0x7E2A, 0x1383, 0x49D9, 0x07B8, 0x2578, 0x4EEC, 0x4423, 0x352F,
        0x5B22, 0x72B9, 0x367B, 0x24B6, 0x7E8E, 0x2318, 0x6BD0, 0x5519,
        0x1783, 0x18A7, 0x7B6E, 0x7602, 0x4B7F, 0x3648, 0x2C53, 0x6B99,
        0x0C23, 0x67CF, 0x7E0E, 0x4D8C, 0x5079, 0x209D, 0x244A, 0x747B,
        0x350B, 0x0E4D, 0x7004, 0x6AC3, 0x7F3E, 0x21F5, 0x7A15, 0x2379,
        0x1517, 0x1ABA, 0x4E77, 0x15A1, 0x04FA, 0x2D61, 0x253A, 0x1302,
        0x1F63, 0x5AB3, 0x049A, 0x5AE8, 0x1CD7, 0x4A00, 0x30C8, 0x3247,
        0x729C, 0x5034, 0x2B0E, 0x57F2, 0x00E4, 0x575B, 0x6192, 0x38F8,
        0x2F6A, 0x0C14, 0x45FC, 0x41DF, 0x38DA, 0x7AE1, 0x7322, 0x62DF,
        0x5E39, 0x0E64, 0x6D85, 0x5951, 0x5937, 0x6281, 0x33A1, 0x6A32,
        0x3A5A, 0x2BAC, 0x743A, 0x5E74, 0x3B2E, 0x7EC7, 0x4FD2, 0x5D28,
        0x751F, 0x3EF8, 0x39B1, 0x4E49, 0x746B, 0x6EF6, 0x44BE, 0x6DB7
   };
   #endif   
#endif

#define NAND_SUCC                   (0)
#define NAND_FAIL                   (1)
#define NAND_ECC_ERROR              (4)
#define NAND_CMD_ERROR              (8)
#define NAND_INVALID_ARG            (16)
#define POLLING_RETRY               (512*512)

#define NAND_DMA_CACHE_LINE         (32)
#define NAND_DMA_READ_VERIFY        (0)
#define NAND_DMA_PATTERN            (0xFEDCBA98)
#define NAND_DMA_RETYR_DELAY        (2)
#define NAND_DMA_RETYR_CNT          (50)

//-----------------------------------------------------------------------------
// mtk partition information
//-----------------------------------------------------------------------------
mtk_part_info_t rMTKPartInfo[NANDPART_PARTNUM];

#if NAND_ENABLE_ISR
    #if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
    extern void mtd_nand_setisr(UINT32 u4Mask);
    extern void mtd_nand_waitisr(UINT32 u4Mask);
    #else
    static HANDLE_T _hNandIsrSema = (HANDLE_T)NULL;
    #endif
#endif

//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
static BOOL _fgIsrEn = FALSE;
static BOOL _fgDmaEn = FALSE;
static NAND_ECC_TYPE_T _rECCType = NAND_ECC_SOFT;

static BOOL _fgMLC = FALSE;
NAND_FLASH_DEV_T *_prFlashDev;
static UINT32 _u4BlkPgCount;
static UINT32 _u4FdmNum;
static UINT32 _u4SectorNum;
static UINT32 _u4PgAddrLen;
static UINT32 _u4TotalAddrLen;
static UINT32 _u4PageSize = 2048;
static UINT32 _u4OOBSize  = 16;
static UINT32 _u4SectorSize = 1024;
static UINT32 _u4ECCBits = 0;
static UINT32 _u4CurPage = 0xFFFFFFFF;
static BOOL _fgNandInit = FALSE;
static UINT32 _u4CurBitflip = 0;

#if NAND_MULTIPAGE_ACCESS
static UINT32 _u4MultiPage[NAND_NFI_MDMA_PAGENUM];
#endif
    
#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
    extern void MTDNANDGetDmaBuf(UINT8 **ppu1DataBuf, UINT8 **ppu1OobBuf);
#else
    #if NAND_MULTIPAGE_ACCESS
    static UINT32 _u4NandDataBuf[(NAND_MAX_PAGE_SIZE * NAND_NFI_MDMA_PAGENUM + 64)/4];
    static UINT32 _u4NandOobBuf[(NAND_MAX_OOB_SIZE * NAND_NFI_MDMA_PAGENUM)/4];
    #else
    static UINT32 _u4NandDataBuf[(NAND_MAX_PAGE_SIZE + 64)/4];
    static UINT32 _u4NandOobBuf[NAND_MAX_OOB_SIZE/4];
    #endif
#endif

static UINT8 *_pu1NandDataBuf = NULL;
static UINT8 *_pu1NandOobBuf  = NULL;

//-----------------------------------------------------------------------------
/** _NANDHW_countbits()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_countbits(UINT32 byte)
{
    UINT32 res = 0;

    for (; byte; byte >>= 1)
    {
        res += (byte & 0x01);
    }
    
    return res;
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitRead()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitRead(UINT32 u4WaitNum)
{
#ifndef TOOLDRIVER
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_RD_REMAIN;
    } while (u4Val < u4WaitNum);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitWrite()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitWrite(void)
{
#ifndef TOOLDRIVER
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_WT_EMPTY;
    } while (u4Val == 0);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitEccDone() must wait done
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitEccDone(UINT32 u4SectIdx)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECDONE);
        u4Val = (u4Val >> u4SectIdx) & 0x1;
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitBusy(void)
{
#ifndef TOOLDRIVER
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_NFI_STA);
        u4Val &= 0xFF000DFF;    // bit8=1: ready to busy
    } while (u4Val != 0);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_WaitBusy(void)
{
#ifndef TOOLDRIVER
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCIDLE);
    } while (u4Val == 0);
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECIDLE);
    } while (u4Val == 0);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WaitSectorCnt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WaitSectorCnt(void)
{
#ifndef TOOLDRIVER
    UINT32 u4Val, u4Cnt;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_ADDRCNTR);
        u4Val &= 0xF000;
        u4Cnt = NAND_NFI_ADDRCNTR_SEC_CNTR(_u4SectorNum);
    } while (u4Val != u4Cnt);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
#if defined(CC_AC_DROP_GPIO_ISR) && defined(__KERNEL__)
extern UINT8 _bACDropISR;
#endif

static void _NANDHW_Command(UINT32 u4Cmd)
{
#if defined(CC_AC_DROP_GPIO_ISR) && defined(__KERNEL__)
    if (u4Cmd == NAND_CMD_INPUT_PAGE || u4Cmd == NAND_CMD_ERASE1_BLK || u4Cmd == NAND_CMD_READ1_00)
    {
        UINT32 u4Val = IO_READ32(0, REG_ACDROP_GPIO);
        
        if ((u4Val & REG_ACDROP_GPIOMASK) == 0)
        {
            printk(KERN_CRIT "Turn off nand access(mtk command:0x%x), detect AC drop GPIO!\n", u4Cmd);
            while (1);
        }
        
        if (_bACDropISR)
        {
            printk(KERN_CRIT "Turn off nand access(mtk command:0x%x), detect AC drop ISR!\n", u4Cmd);
            while (1);
        }
    }
#endif

    NAND_WRITE32(NAND_NFI_CMD, u4Cmd);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Addr(UINT32 u4RowAddr, UINT32 u4ColAddr, UINT32 u4ByteCount)
{
    if (_prFlashDev != NULL)
    {
       if (u4RowAddr >= (_prFlashDev->u4ChipSize/_prFlashDev->u4PageSize))
       {
          LOG(0, "page index = 0x%x exceeds nand flash page range,page range from 0 to 0x%x\n", u4RowAddr,
		  	     (_prFlashDev->u4ChipSize/_prFlashDev->u4PageSize)-1);
          ASSERT(0);
       }
    }
    NAND_WRITE32(NAND_NFI_COLADDR, u4ColAddr);
    NAND_WRITE32(NAND_NFI_ROWADDR, u4RowAddr);
    NAND_WRITE32(NAND_NFI_ADDRNOB, u4ByteCount);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Reset(void)
{
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_CON, NAND_NFI_CON_NFI_RST | NAND_NFI_CON_FIFO_FLUSH);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Reset(void)
{
    NAND_WRITE32(NAND_NFIECC_ENCON, 0);
    NAND_WRITE32(NAND_NFIECC_DECCON, 0);
    _NANDHW_NFIECC_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetPageFmt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetPageFmt(void)
{
    UINT32 u4Val = 0;
    _u4FdmNum = 8;

#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
        if (_u4PageSize == 512)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_512_16;
        }
        else if (_u4PageSize == 2048)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
        else if (_u4PageSize == 4096)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
        else if (_u4PageSize == 8192)
        {
            u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
            u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_32;
        }
    }
    else if (_u4OOBSize == 26)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_52;
    }
    else if (_u4OOBSize == 32)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_32;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_64;
    }
    else if (_u4OOBSize == 36)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_36;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_72;
    }
    else if (_u4OOBSize == 40)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_40;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_80;
    }
    else if (_u4OOBSize == 64)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_64;
        u4Val |= NAND_NFI_PAGEFMT_SECTOR_SIZE_1024_128;
    }

    if (_u4PageSize == 4096)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;            
    }
    else if (_u4PageSize == 8192)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
  
#else
    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
    }
    else
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
    }

    if (_u4PageSize == 4096)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
    }
    else if (_u4PageSize == 8192)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
    else
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
    }      
#endif
    u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);

    NAND_WRITE32(NAND_NFI_PAGEFMT, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Cfg(NAND_ECC_TYPE_T rECCType, BOOL fgDmaEn, UINT32 u4OpMode)
{
    UINT32 u4Val = 0;
    
    if (rECCType == NAND_ECC_HARD)
    {
        u4Val = NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_HW_ECC_EN;
    }
    else if (rECCType == NAND_ECC_SOFT)
    {
        u4Val = NAND_NFI_CNFG_HW_ECC_EN;
    }
    else
    {
        u4Val = 0;
    }

    if (fgDmaEn)
    {
        u4Val |= (NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_AHB_MODE);
    }
  
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_IDLE:
        NAND_WRITE32(NAND_NFI_CNFG, 0);
        return;
        
    case NAND_NFI_CNFG_OP_SINGLE:
    case NAND_NFI_CNFG_OP_READ:
        u4Val |= (NAND_NFI_CNFG_READ_MODE | u4OpMode);
        break;
        
    case NAND_NFI_CNFG_OP_PROGRAM:
    case NAND_NFI_CNFG_OP_ERASE:
    case NAND_NFI_CNFG_OP_RESET:
    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val |= u4OpMode;
        break;
        
    default:
        break;
    }
   
    if (_u4PageSize == 512)
    {
        u4Val |= NAND_NFI_CNFG_SEL_SEC_512BYTE;
    }

    NAND_WRITE32(NAND_NFI_CNFG, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Cfg(NAND_ECC_TYPE_T rECCType)
{
    UINT32 u4EncMsg = 0, u4DecMsg = 0;

    if (_u4OOBSize == 16)
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 4;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_4 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_4 | NAND_NFIECC_DECCNFG_DEC_CS_520_4;
        }
        else
        {
            _u4ECCBits = 12;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_12 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_12 | NAND_NFIECC_DECCNFG_DEC_CS_1032_12;
        }
    }
    else if(_u4OOBSize == 26)
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 10;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_10 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_10 | NAND_NFIECC_DECCNFG_DEC_CS_520_10;
        }
        else
        {
            _u4ECCBits = 24;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_24 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_24 | NAND_NFIECC_DECCNFG_DEC_CS_1032_24;
        }
    }
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    else if(_u4OOBSize == 32)
    {
         _u4ECCBits = 32;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_32 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_32 | NAND_NFIECC_DECCNFG_DEC_CS_1032_32;
    }
    else if(_u4OOBSize == 36)
    {
         _u4ECCBits = 36;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_36 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_36 | NAND_NFIECC_DECCNFG_DEC_CS_1032_36;
    }
    else if(_u4OOBSize == 40)
    {
         _u4ECCBits = 40;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_40 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_40 | NAND_NFIECC_DECCNFG_DEC_CS_1032_40;
    }
    else if(_u4OOBSize == 64)
    {
         _u4ECCBits = 60;
         u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_60 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
         u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_60 | NAND_NFIECC_DECCNFG_DEC_CS_1032_60;
    }
#endif
    u4EncMsg |= NAND_NFIECC_ENCCNFG_ENC_NFI_MODE;
    u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CS_EMPTY_EN | NAND_NFIECC_DECCNFG_DEC_NFI_MODE;

    if (rECCType == NAND_ECC_HARD)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_AUTO;
    }
    else if (rECCType == NAND_ECC_SOFT)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_SOFT;
    }
    else if (rECCType == NAND_ECC_NONE)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_NONE;
    }
    else
    {
        ASSERT(0);
    }
    
    NAND_WRITE32(NAND_NFIECC_ENCCNFG, u4EncMsg);
    NAND_WRITE32(NAND_NFIECC_DECCNFG, u4DecMsg);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Trig(UINT32 u4OpMode)
{
    UINT32 u4Val;
    
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_SINGLE:
        u4Val = NAND_NFI_CON_NOB(4) | NAND_NFI_CON_SRD;
        break;
        
    case NAND_NFI_CNFG_OP_READ:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BRD;
        break;
        
    case NAND_NFI_CNFG_OP_PROGRAM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        break;
        
    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        NAND_WRITE32(NAND_NFI_CON, u4Val);
        NAND_WRITE32(NAND_NFI_STRDATA, NAND_NFI_STRDATA_STRDATA);
        return;
        
    default:
        u4Val = 0;
        break;
    }
    
    NAND_WRITE32(NAND_NFI_CON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Trig(UINT32 u4OpMode)
{
    switch (u4OpMode)
    {
    case NAND_NFI_CNFG_OP_READ:
        NAND_WRITE32(NAND_NFIECC_DECCON, 1);
        break;

    case NAND_NFI_CNFG_OP_PROGRAM:
        NAND_WRITE32(NAND_NFIECC_ENCON, 1);
        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_CheckOpMode()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_CheckOpMode(UINT32 u4OpMode)
{
    UINT32 i, u4Val;
    
    for (i = 0; i < 3; i++)
    {
        switch (u4OpMode)
        {
        case NAND_NFI_CNFG_OP_READ:
            u4Val = NAND_READ32(NAND_NFI_STA);
            u4Val &= NAND_NFI_STA_NFI_FSM;
            if(u4Val == NAND_NFI_STA_NFI_FSM_READ_DATA)
            {
                return NAND_SUCC;
            }
            break;
            
        default:
            return NAND_INVALID_ARG;
        }
    }
    
    return NAND_CMD_ERROR;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_GetErrNum()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_GetErrNum(UINT32 u4SectIdx, UINT32 *pu4ErrNum)
{
    UINT32 u4Val;
    ASSERT(pu4ErrNum != NULL);
    
    if (u4SectIdx < 4)
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM0);
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x1F;
#endif
    }
    else
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM1);
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
        *pu4ErrNum = (u4Val >> ((u4SectIdx-4)*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> ((u4SectIdx-4)*8)) & 0x1F;
#endif
    }
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)    
    if (*pu4ErrNum == 0x3F)
    {
        return NAND_ECC_ERROR;
    }
#else
    if (*pu4ErrNum == 0x1F)
    {
        return NAND_ECC_ERROR;
    }
#endif
    else
    {
        return NAND_SUCC;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CorrectEmptySector()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CorrectEmptySector(UINT8 *buf, UINT8 *spare, UINT32 u4SectIdx)
{
    UINT32 i, u4Val, u4SectOobSz, u4bitflip = 0;
    UINT8 *pu1Data, *pu1Spare;
    UINT32 *pu4Data, *pu4Spare;
    
    u4Val = NAND_READ32(NAND_NFIECC_DECFER);
    if ((u4Val & (1 << u4SectIdx)) == 0)
    {
        return NAND_SUCC;
    }

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;

    pu1Data = buf + u4SectIdx * _u4SectorSize;
    pu1Spare = spare + u4SectIdx * u4SectOobSz;
    pu4Data = (UINT32 *)pu1Data;
    pu4Spare = (UINT32 *)pu1Spare;

    for (i = 0; i < _u4SectorSize/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Data[i]));
    }

    for (i = 0; i < _u4FdmNum/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Spare[i]));
    }

    if (u4bitflip <= _u4ECCBits)
    {
        x_memset(pu1Data, 0xFF, _u4SectorSize);
        x_memset(pu1Spare, 0xFF, _u4FdmNum);
        
        if (u4bitflip > (_u4ECCBits/2))
        {
            _u4CurBitflip += u4bitflip;
        }
        
        //LOG(3, "Correctable ECC found in empty page 0x%X, sector %d: %d bit-flip\n", 
        //    _u4CurPage, u4SectIdx, u4bitflip);
        return NAND_SUCC;
    }
    else
    {
#ifdef __KERNEL__
        printk(KERN_ERR "Un-Correctable ECC found in page 0x%X, sector %d, bitflip=%d\n", _u4CurPage, u4SectIdx, u4bitflip);
#else
        LOG(0, "Un-Correctable ECC found in page 0x%X, sector %d, bitflip=%d\n", _u4CurPage, u4SectIdx, u4bitflip);
#endif
        return NAND_ECC_ERROR;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CorrectEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CorrectEcc(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4SectIdx)
{
    INT32 i4Ret;
    UINT32 i, u4Val, u4SpareOff, u4SectOobSz;
    UINT32 u4ErrNum, u4ErrFound, u4ErrByte, u4ErrBit;
    
    ASSERT(pu1Buf != NULL);
    ASSERT(pu1Spare != NULL);
    
    _NANDHW_WaitEccDone(u4SectIdx); // must wait done

    if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
    {
        i4Ret = _NANDHW_NFIECC_CorrectEmptySector(pu1Buf, pu1Spare, u4SectIdx);
        return i4Ret;
    }
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    u4ErrFound = NAND_READ32(NAND_NFIECC_DECFER);
    if ((u4ErrFound >> u4SectIdx) & 0x01)
    {
        for (i = 0; i < u4ErrNum; i++)
        {
            u4Val = NAND_READ16(NAND_NFIECC_DECEL0 + (i*2));
            u4ErrByte = u4Val >> 3;
            u4ErrBit = u4Val & 0x7;

            if (u4ErrByte < _u4SectorSize)
            {
                // Data area bit error.
                u4ErrByte += u4SectIdx*_u4SectorSize;
                pu1Buf[u4ErrByte] = pu1Buf[u4ErrByte] ^ (((UINT32)1)<<u4ErrBit);
            }
            else if (u4ErrByte < (u4SectOobSz + _u4SectorSize))
            {
                // Spare area bit error.
                u4SpareOff = u4ErrByte - _u4SectorSize + u4SectOobSz * u4SectIdx;
                pu1Spare[u4SpareOff] = pu1Spare[u4SpareOff] ^ (((UINT32)1)<<u4ErrBit);
            }
            else
            {
#ifdef __KERNEL__
                printk(KERN_ERR "Un-Correctable ECC found in page = 0x%X, sector = 0x%X, loc = 0x%X, bitflip=%d\n", 
                    _u4CurPage, u4SectIdx, u4Val, u4ErrNum);
#else
                LOG(0, "Un-Correctable ECC found in page = 0x%X, sector = 0x%X, loc = 0x%X, bitflip=%d\n", 
                    _u4CurPage, u4SectIdx, u4Val, u4ErrNum);
#endif
                return NAND_ECC_ERROR;
            }
        }
    }
    
    if (u4ErrNum > (_u4ECCBits/2))
    {
        _u4CurBitflip += u4ErrNum;
    }
    
    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEccDone()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_CheckEccDone(void)
{
    UINT32 u4SectIdx;
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        _NANDHW_WaitEccDone(u4SectIdx);
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CheckEcc(UINT8 *buf, UINT8 *spare)
{
    UINT32 u4SectIdx, u4ErrNum;
    INT32 i4Ret = NAND_SUCC;
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
        {
            i4Ret |= _NANDHW_NFIECC_CorrectEmptySector(buf, spare, u4SectIdx);
        }
        else if(u4ErrNum != 0)
        {
            if (u4ErrNum > (_u4ECCBits/2))
            {              
                _u4CurBitflip += u4ErrNum;          
            }
            //LOG(3, "Correctable ECC found in page 0x%X, sector %d: %d bit-flip\n",  _u4CurPage, u4SectIdx, u4ErrNum);
        }
    }
    
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetRdDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetRdDma(UINT32 *pu4DataAddr)
{
    UINT32 u4PhyAddr;
    ASSERT(pu4DataAddr != NULL);

    u4PhyAddr = PHYSICAL((UINT32)pu4DataAddr);
    u4PhyAddr += NAND_DRAM_BASE;
    NAND_WRITE32(NAND_NFI_STRADDR, u4PhyAddr);
    NAND_WRITE32(NAND_NFIECC_FDMADDR, NAND_REG_BASE + NAND_NFI_FDM0L);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetWtDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetWtDma(UINT32 *pu4DataAddr, UINT32 *pu4SpareAddr)
{
    UINT32 i, j, u4SectOobSz, u4PhyAddr;
    UINT32 *pu4SectSpare;
    
    ASSERT(pu4SpareAddr != NULL);

    if (pu4DataAddr)
    {
        u4PhyAddr = PHYSICAL((UINT32)pu4DataAddr);
        u4PhyAddr += NAND_DRAM_BASE;
        NAND_WRITE32(NAND_NFI_STRADDR, u4PhyAddr);
    }
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    for (i = 0; i < _u4SectorNum; i++)
    {
        pu4SectSpare = (UINT32 *)((UINT32)pu4SpareAddr + u4SectOobSz * i);
        
        for (j = 0; j < _u4FdmNum/4; j++)
        {
            NAND_WRITE32((NAND_NFI_FDM0L + 4*j+ NAND_NFI_FDM_LEN * i), pu4SectSpare[j]);
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_GetRdDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_GetRdDma(UINT32 *pu4SpareAddr)
{
    UINT32 i, j, u4SectOobSz;
    UINT32 *pu4SectSpare;
    
    ASSERT(pu4SpareAddr != NULL);
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    for (i = 0; i < _u4SectorNum; i++)
    {
        pu4SectSpare = (UINT32 *)((UINT32)pu4SpareAddr + u4SectOobSz * i);
        
        for (j = 0; j < _u4FdmNum/4; j++)
        {
            pu4SectSpare[j] = NAND_READ32(NAND_NFI_FDM0L + 4*j + NAND_NFI_FDM_LEN * i);
        }
    }
}

#if NAND_MULTIPAGE_ACCESS
//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_Addr(UINT32 u4ByteCount)
{
    NAND_WRITE32(NAND_NFI_COLADDR, 0);
    NAND_WRITE32(NAND_NFI_ADDRNOB, u4ByteCount);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_SectNum(void)
{
    UINT32 u4Val;
    
    u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum);
    NAND_WRITE32(NAND_NFI_CON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_Cfg(UINT32 u4PgCnt, UINT32 u4OpMode)
{
     UINT32 u4Val;

     u4Val = NAND_NFI_MDMA_EN | NAND_NFI_MDMA_LEN(u4PgCnt - 1);

    if (u4OpMode == NAND_NFI_CNFG_OP_READ)
    {
        u4Val |= NAND_NFI_MDMA_READ;
    }
    else if (u4OpMode == NAND_NFI_CNFG_OP_PROGRAM)
    {
        u4Val |= NAND_NFI_MDMA_WRITE;
    }
    else if (u4OpMode == NAND_NFI_CNFG_OP_ERASE)
    {
        u4Val |= NAND_NFI_MDMA_ERASE;
    }
    else
    {
        ASSERT(0);
    }
    
    NAND_WRITE32(NAND_NFI_MDMACON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_Trig(void)
{
     UINT32 u4Val;
     
    u4Val = NAND_READ32(NAND_NFI_MDMACON);
    u4Val |= NAND_NFI_MDMA_TRIG;
    NAND_WRITE32(NAND_NFI_MDMACON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_SetRdSRAM()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_SetRdSRAM(UINT32 *pu4Buf, UINT32 u4PgCnt)
{
    UINT32 i, u4PhyAddr;
        
    for (i = 0; i < u4PgCnt; i++)
    {
        u4PhyAddr = (UINT32)pu4Buf + _u4PageSize * i;
        u4PhyAddr += NAND_DRAM_BASE;
        
        NAND_WRITE32(NAND_NFI_MDMAADDR, i);
        
        NAND_WRITE32(NAND_NFI_MDMADATA, u4PhyAddr);
        NAND_WRITE32(NAND_NFI_MDMADATA, _u4MultiPage[i]);
    }
    
    NAND_WRITE32(NAND_NFIECC_FDMADDR, NAND_REG_BASE + NAND_NFI_FDM0L);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_SetRdSRAM()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_SetWtSRAM(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgCnt)
{
    UINT32 i, j, k;
    UINT32 u4PhyAddr, u4PgOobSz, u4SectOobSz;
    UINT32 *pu4SectSpare;
    
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
        
    for (i = 0; i < u4PgCnt; i++)
    {
        u4PhyAddr = (UINT32)pu4Buf + _u4PageSize * i;
        u4PhyAddr += NAND_DRAM_BASE;
        
        NAND_WRITE32(NAND_NFI_MDMAADDR, i);
        
        NAND_WRITE32(NAND_NFI_MDMADATA, u4PhyAddr);
        NAND_WRITE32(NAND_NFI_MDMADATA, _u4MultiPage[i]);
        
        for (j = 0; j < _u4SectorNum; j++)
        {
            pu4SectSpare = (UINT32 *)((UINT32)pu4Spare + u4PgOobSz * i + u4SectOobSz * j);
            
            for (k = 0; k < _u4FdmNum/4; k++)
            {
                NAND_WRITE32(NAND_NFI_MDMADATA, pu4SectSpare[k]);
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_GetRdSRAM()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_GetRdSRAM(UINT32 *pu4Spare, UINT32 u4PgCnt)
{
    UINT32 i, j, k;
    UINT32 u4PgOobSz, u4SectOobSz;
    UINT32 *pu4SectSpare;
    
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;

    for (i = 0; i < u4PgCnt; i++)
    {
        NAND_WRITE32(NAND_NFI_MDMAADDR, i + NAND_NFI_MDMAADDR_FDM);
        
        for (j = 0; j < _u4SectorNum; j++)
        {
            pu4SectSpare = (UINT32 *)((UINT32)pu4Spare + u4PgOobSz * i + u4SectOobSz * j);
            
            for (k = 0; k < _u4FdmNum/4; k++)
            {
                pu4SectSpare[k] = NAND_READ32(NAND_NFI_MDMADATA);
            }
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_WaitEccDone() must wait done
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_WaitEccDone(UINT32 u4SectIdx, UINT32 u4PgIdx)
{
    UINT32 u4Val;
    
    do
    {
        NAND_WRITE32(NAND_NFI_MDMAADDR, u4PgIdx + NAND_NFI_MDMAADDR_DECDONE);
        
        u4Val = NAND_READ32(NAND_NFI_MDMADATA);
        u4Val = (u4Val >> u4SectIdx) & 0x1;
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_GetErrNum()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIMDMA_GetErrNum(UINT32 u4SectIdx, UINT32 *pu4ErrNum, UINT32 u4PgIdx)
{
    UINT32 u4Val;
    ASSERT(pu4ErrNum != NULL);
    
    if (u4SectIdx < 4)
    {
        NAND_WRITE32(NAND_NFI_MDMAADDR, u4PgIdx + NAND_NFI_MDMAADDR_DECENUM0);
        u4Val = NAND_READ32(NAND_NFI_MDMADATA);
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x1F;
#endif
    }
    else
    {
        NAND_WRITE32(NAND_NFI_MDMAADDR, u4PgIdx + NAND_NFI_MDMAADDR_DECENUM1);
        u4Val = NAND_READ32(NAND_NFI_MDMADATA);        
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
        *pu4ErrNum = (u4Val >> ((u4SectIdx-4)*8)) & 0x3F;
#else
        *pu4ErrNum = (u4Val >> ((u4SectIdx-4)*8)) & 0x1F;
#endif
    }
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)    
    if (*pu4ErrNum == 0x3F)
    {
        return NAND_ECC_ERROR;
    }
#else
    if (*pu4ErrNum == 0x1F)
    {
        return NAND_ECC_ERROR;
    }
#endif    
    else
    {
        return NAND_SUCC;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_CorrectEmptySector()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIMDMA_CorrectEmptySector(UINT8 *buf, UINT8 *spare, UINT32 u4SectIdx, UINT32 u4PgIdx)
{
    UINT32 i, u4Val, u4SectOobSz, u4bitflip = 0;
    UINT8 *pu1Data, *pu1Spare;
    UINT32 *pu4Data, *pu4Spare;
    
    NAND_WRITE32(NAND_NFI_MDMAADDR, u4PgIdx + NAND_NFI_MDMAADDR_DECFER);
    u4Val = NAND_READ32(NAND_NFI_MDMADATA);
    if ((u4Val & (1 << u4SectIdx)) == 0)
    {
        return NAND_SUCC;
    }

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    
    pu1Data = buf + u4SectIdx * _u4SectorSize;
    pu1Spare = spare + u4SectIdx * u4SectOobSz;
    pu4Data = (UINT32 *)pu1Data;
    pu4Spare = (UINT32 *)pu1Spare;

    for (i = 0; i < _u4SectorSize/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Data[i]));
    }

    for (i = 0; i < _u4FdmNum/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Spare[i]));
    }

    if (u4bitflip <= _u4ECCBits)
    {
        x_memset(pu1Data, 0xFF, _u4SectorSize);
        x_memset(pu1Spare, 0xFF, _u4FdmNum);
        
        if (u4bitflip > (_u4ECCBits/2))
        {
            _u4CurBitflip += u4bitflip;
        }
		
        //LOG(3, "Correctable ECC found in empty page 0x%X, sector %d: %d bit-flip\n", 
        //    _u4MultiPage[u4PgIdx], u4SectIdx, u4bitflip);
        return NAND_SUCC;
    }
    else
    {
        LOG(0, "Un-Correctable ECC found in page 0x%X, sector %d\n",
            _u4MultiPage[u4PgIdx], u4SectIdx);
        return NAND_ECC_ERROR;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_CheckEccDone()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIMDMA_CheckEccDone(UINT32 u4PgCnt)
{
    UINT32 u4PgIdx, u4SectIdx;

    for (u4PgIdx = 0; u4PgIdx < u4PgCnt; u4PgIdx++)
    {
        for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
        {
            _NANDHW_NFIMDMA_WaitEccDone(u4SectIdx, u4PgIdx);
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIMDMA_CheckEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIMDMA_CheckEcc(UINT8 *buf, UINT8 *spare, UINT32 u4PgCnt)
{
    UINT32 u4PgIdx, u4SectIdx, u4ErrNum;
    INT32 i4Ret = NAND_SUCC;

    for (u4PgIdx = 0; u4PgIdx < u4PgCnt; u4PgIdx++)
    {
        for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
        {
            if (_NANDHW_NFIMDMA_GetErrNum(u4SectIdx, &u4ErrNum, u4PgIdx))
            {
                i4Ret |= _NANDHW_NFIMDMA_CorrectEmptySector(buf, spare, u4SectIdx, u4PgIdx);
            }
            else if(u4ErrNum != 0)
            {
	            if (u4ErrNum > (_u4ECCBits/2))
		        {              
		            _u4CurBitflip += u4ErrNum;          
		        }
				
                //LOG(3, "Correctable ECC found in page 0x%X, sector %d: %d bit-flip\n", 
                //    _u4MultiPage[u4PgIdx], u4SectIdx, u4ErrNum);
            }
        }
    }
    
    return i4Ret;
}
#endif  // NAND_MULTIPAGE_ACCESS

#if NAND_ENABLE_ISR

#if !defined(__KERNEL__) || !defined(CC_NAND_ENABLE)
//-----------------------------------------------------------------------------
/** _NANDHW_Isr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Isr(UINT16 u2VctId)
{
    UINT32 u4NandIsrSt;
    
    UNUSED(u2VctId);
    
    u4NandIsrSt = NAND_READ32(NAND_NFI_INTR);
    u4NandIsrSt &= NAND_NFI_INTR_MASK;
    if (u4NandIsrSt == 0)
    {
        LOG(0, "curious _NANDHW_Isr!\n");
    }
    
    x_sema_unlock(_hNandIsrSema);
}
#endif

//-----------------------------------------------------------------------------
/** _NANDHW_RegIsr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_RegIsr(void)
{
// nand isr is registered in native driver in kernel.
#if !defined(__KERNEL__) || !defined(CC_NAND_ENABLE)
    void (* pfnOldIsr)(UINT16);
    
    NAND_READ32(NAND_NFI_INTR);      // Clear interrupt

    VERIFY(x_sema_create(&_hNandIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    VERIFY(x_reg_isr(VECTOR_NAND, _NANDHW_Isr, &pfnOldIsr) == OSR_OK);
    
    NAND_READ32(NAND_NFI_INTR);      // Clear interrupt
    VERIFY(BIM_DisableIrq(VECTOR_NAND));
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_SetIsr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetIsr(UINT32 u4Mask)
{
#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
    mtd_nand_setisr(u4Mask);
#else
    VERIFY(BIM_EnableIrq(VECTOR_NAND));
    
    NAND_READ32(NAND_NFI_INTR);      // Clear interrupt
    NAND_WRITE32(NAND_NFI_INTR_EN, u4Mask);
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitIsr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitIsr(UINT32 u4Mask)
{
#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
    mtd_nand_waitisr(u4Mask);
#else
    UNUSED(u4Mask);

    VERIFY(OSR_OK == x_sema_lock(_hNandIsrSema, X_SEMA_OPTION_WAIT));

    NAND_WRITE32(NAND_NFI_INTR_EN, 0);
    VERIFY(BIM_DisableIrq(VECTOR_NAND));
#endif
}
#endif

//-----------------------------------------------------------------------------
/** _NANDHW_SetTiming()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetTiming(void)
{
#ifndef TOOLDRIVER
   if (_prFlashDev)
   {
        NAND_WRITE32(NAND_NFI_ACCCON1, _prFlashDev->u4AccCon);
        NAND_WRITE32(NAND_NFI_ACCCON2, _prFlashDev->u4RdAcc);
   }
#endif
}

//-----------------------------------------------------------------------------
/** _NANDHW_SetSectorSize()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetSectorSize(void)
{
    if (_u4PageSize > 512)
    {
        _u4SectorSize = 1024;
    }
    else
    {
        _u4SectorSize = 512;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_DataExchange()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_DataExchange(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgOff, UINT32 u4Len)
{
    UINT32 u4Val, u4Addr;

    ASSERT(pu4Buf != NULL);
    ASSERT(pu4Spare != NULL);
    
    if (_u4OOBSize == 16)
    {
        if (_u4PageSize == 0x800)
        {
            u4Addr = 0x7E0;
        }
        else if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xFA0;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1F20;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 26)
    {
        if (_u4PageSize == 0x800)
        {
            u4Addr = 0x7CC;
        }
        else if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF64;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E94;
        }
        else
        {
            return NAND_SUCC;
        }
    }
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    else if (_u4OOBSize == 32)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF40;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E40;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 36)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF28;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E08;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 40)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xF10;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1DD0;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if (_u4OOBSize == 64)
    {
        if (_u4PageSize == 0x1000)
        {
            u4Addr = 0xE80;
        }
        else if (_u4PageSize == 0x2000)
        {
            u4Addr = 0x1C80;
        }
        else
        {
            return NAND_SUCC;
        }
    }
#endif
    else
    {
        return NAND_FAIL;
    }
    
    if ((u4Addr >= u4PgOff) && (u4Addr < (u4PgOff + u4Len)))
    {
        u4Val = pu4Buf[(u4Addr-u4PgOff)>>2];
        pu4Buf[(u4Addr-u4PgOff)>>2] = pu4Spare[0];
        pu4Spare[0] = u4Val;
    }
    
    return NAND_SUCC;
}
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
#if NAND_RANDOM_EN

//-----------------------------------------------------------------------------
/** NANDHW_Random_Mizer_Encode()
 */
//-----------------------------------------------------------------------------
void NANDHW_Random_Mizer_Encode(UINT32 u4PgIdx, BOOL fgRandom)
{
    UINT32 u4Val;

    if (fgRandom)
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
        u4Val = NAND_NFI_ENCODING_RANDON_SEED(RANDOM_SEED[u4PgIdx%_u4BlkPgCount]) | NAND_NFI_ENCODING_RANDON_EN;
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
    }
}
//-----------------------------------------------------------------------------
/** NANDHW_Random_Mizer_Decode()
 */
//-----------------------------------------------------------------------------
void NANDHW_Random_Mizer_Decode(UINT32 u4PgIdx, BOOL fgRandom)
{
    UINT32 u4Val;

    if (fgRandom)
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
        u4Val = NAND_NFI_DECODING_RANDON_SEED(RANDOM_SEED[u4PgIdx%_u4BlkPgCount]) | NAND_NFI_DECODING_RANDON_EN;
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_RANDOM_CFG, 0);
    }
}
#endif
#else
#if NAND_SCRAMBLE_EN
#define NAND_SCRAMBLE_PATTERN   0xAA

//-----------------------------------------------------------------------------
/** NANDHW_SetScramble()
 */
//-----------------------------------------------------------------------------
void NANDHW_SetScramble(BOOL fgScramble)
{
    UINT32 u4Val;

    if (fgScramble)
    {
        u4Val = NAND_NFI_SCRAMBLE_PAT(NAND_SCRAMBLE_PATTERN) | NAND_NFI_SCRAMBLE_EN | NAND_NFI_INTERLEAVE_EN;
        NAND_WRITE32(NAND_NFI_SCRAMBLE, u4Val);
    }
    else
    {
        NAND_WRITE32(NAND_NFI_SCRAMBLE, 0);
    }
}
#endif
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** NANDHW_ReadID()
 */
//-----------------------------------------------------------------------------
void NANDHW_ReadID(UINT32 *pu4ID)
{
    ASSERT(pu4ID != NULL);

    _NANDHW_NFI_Reset();

    _NANDHW_Command(NAND_CMD_READ_ID);
    _NANDHW_Addr(0, 0, 1);
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_SINGLE);
    _NANDHW_WaitBusy();
    
    _NANDHW_WaitRead(4);
    *pu4ID = NAND_READ32(NAND_NFI_DATAR);
}

//-----------------------------------------------------------------------------
/** NANDHW_Reset()
 */
//-----------------------------------------------------------------------------
void NANDHW_Reset(void)
{
    _NANDHW_NFI_Reset();
    _NANDHW_Command(NAND_CMD_RESET);

    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** NANDHW_EnablePinmux()
 */
//-----------------------------------------------------------------------------
void NANDHW_EnablePinmux(BOOL fgEnable)
{
    UINT32 u4Val;
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    NAND_WRITE32(NAND_SELECT_REGISTER, 0x00000001);
    NAND_WRITE32(NAND_NFI_MLC_CFG, NAND_CHK_DRAM_WDLE_EN|NAND_CHK_DRAM_WDLE_EN2);
#endif

#if defined(CC_MT5880)
    NAND_WRITE32(NAND_DMMERGE_CFG, 0xFFFFFFFE);
#endif

    u4Val = NAND_READ32(NAND_NFI_MISC);
    if (fgEnable)
    {
        u4Val |= NAND_NFI_MISC_FLASH_PMUX;
    }
    else
    {
        u4Val &= (~NAND_NFI_MISC_FLASH_PMUX);
    }
    NAND_WRITE32(NAND_NFI_MISC, u4Val);

    if (fgEnable)
    {
        NANDHW_Reset();
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadFifo()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_ReadFifo(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    
    ASSERT(pu4Buf != NULL);
    
    for (i = 0; i < (u4Len>>2); i++)
    {
        _NANDHW_WaitRead(4);
        pu4Buf[i] = NAND_READ32(NAND_NFI_DATAR);
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ReadPage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 u4DmaRetryCnt = 0;
    UINT32 *pu4DmaBuf = (UINT32 *)pu1Buf;
    
    ASSERT(pu1Buf != NULL);
    ASSERT(pu1Spare != NULL);
    
    UNUSED(_u4CurPage);
    
    _u4CurPage = u4PgIdx;
    _u4CurBitflip = 0;

    if (_fgDmaEn)
    {
        pu4DmaBuf[_u4PageSize/4-1] = NAND_DMA_PATTERN;
        HalDmaBufferStart((void *)(&pu1Buf[_u4PageSize - NAND_DMA_CACHE_LINE]), NAND_DMA_CACHE_LINE, HAL_DMA_TO_DEVICE);
        HalDmaBufferDone((void *)(&pu1Buf[_u4PageSize - NAND_DMA_CACHE_LINE]), NAND_DMA_CACHE_LINE, HAL_DMA_TO_DEVICE);

        HalDmaBufferStart((void *)pu1Buf, _u4PageSize, HAL_DMA_FROM_DEVICE);
    }

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(_u4CurPage, TRUE);
    }
    #endif
#endif

    /* Set Dma. */
    if (_fgDmaEn)
    {
        _NANDHW_NFI_SetRdDma((UINT32 *)pu1Buf);
    }

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    /* Write command. */
    _NANDHW_Command(NAND_CMD_READ1_00);
    _NANDHW_Addr(u4PgIdx, 0,  (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    if (_u4PageSize != 0x200)
    {
#if NAND_ENABLE_ISR
        if (_fgIsrEn)
        {
            _NANDHW_SetIsr(NAND_NFI_INTR_EN_RD_DONE);
        }
#endif

        NAND_WRITE32(NAND_NFI_CMD, NAND_CMD_READ_30);

#if NAND_ENABLE_ISR
        if (_fgIsrEn)
        {
            _NANDHW_WaitIsr(NAND_NFI_INTR_RD_DONE);
        }
#endif
    }
    
    _NANDHW_WaitBusy();

    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
#ifdef __KERNEL__
        printk(KERN_ERR "_NANDHW_NFI_CheckOpMode return fail! Page:0x%08x\n", u4PgIdx);
#else
        LOG(0, "_NANDHW_NFI_CheckOpMode return fail! Page:0x%08x\n", u4PgIdx);
#endif
        goto HandleExit;
    }
    
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_AHB_DONE);
    }
#endif

    /*Trigger Read . */
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
 
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_AHB_DONE);
    }
#endif

    _NANDHW_WaitBusy();
    
    if (_fgDmaEn)
    {
        _NANDHW_NFIECC_WaitBusy();
        _NANDHW_NFI_GetRdDma((UINT32 *)pu1Spare);
    }
    else
    {
        UINT32 u4SectIdx, u4SectOobSz;
        u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
        
        for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
        {
            // read main data
            _NANDHW_NFI_ReadFifo(&pu1Buf[_u4SectorSize*u4SectIdx], _u4SectorSize);

            if (_rECCType == NAND_ECC_SOFT)
            {
                // read spare data
                _NANDHW_NFI_ReadFifo(&pu1Spare[u4SectOobSz*u4SectIdx], u4SectOobSz);

                /* ECC correction. */
                if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, pu1Spare, u4SectIdx))
                {
                    i4Ret |= NAND_ECC_ERROR;
                }
            }
            else if (_rECCType == NAND_ECC_HARD)
            {
                _NANDHW_WaitEccDone(u4SectIdx);
            }
            else if (_rECCType == NAND_ECC_NONE)
            {
                // read spare data
                _NANDHW_NFI_ReadFifo(&pu1Spare[u4SectOobSz*u4SectIdx], u4SectOobSz);
            }
            else
            {
                ASSERT(0);
            }
            
            _NANDHW_WaitBusy();
        }
        
        _NANDHW_NFIECC_WaitBusy();
        if (_rECCType == NAND_ECC_HARD)
        {
            _NANDHW_NFI_GetRdDma((UINT32 *)pu1Spare);
        }
        
        if (i4Ret)
        {
            goto HandleExit;
        }
    }
    
    _NANDHW_NFI_WaitSectorCnt();

    if (_rECCType == NAND_ECC_HARD)
    {
        // Check ECC done
        _NANDHW_NFIECC_CheckEccDone();
        
        // Cause nand dma read may send finish interrupt before write all data into dram.
        // So we add a pattern at last location to check if dma read is finished.
        while (_fgDmaEn)
        {
            HalDmaBufferDone((void *)pu1Buf, _u4PageSize, HAL_DMA_FROM_DEVICE);
            
            if (pu4DmaBuf[_u4PageSize/4-1] != NAND_DMA_PATTERN)
            {
                if (u4DmaRetryCnt > 0)
                {
                    LOG(0, "mtk nand driver: u4DmaTryCnt = %d, pageidx = 0x%08X\n", u4DmaRetryCnt, u4PgIdx);
                }
                break;
            }

            u4DmaRetryCnt++;
            if (u4DmaRetryCnt >= NAND_DMA_RETYR_CNT)
            {
                LOG(0, "mtk nand driver: dma pattern check time-out, pageidx = 0x%08X\n", u4PgIdx);
                break;
            }
            
            HAL_Delay_us(NAND_DMA_RETYR_DELAY);
        }
    
        // Cause pattern check and CPU prefetch will update cache.
        // Invalidate whole cache to ensure bit-flips writen into dram.
        if (_fgDmaEn)
        {
            HalDmaBufferDone((void *)pu1Buf, _u4PageSize, HAL_DMA_FROM_DEVICE);
        }
        
        i4Ret = _NANDHW_NFIECC_CheckEcc(pu1Buf, pu1Spare);
        if (i4Ret)
        {
            goto HandleExit;
        }
    }
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(_u4CurPage, FALSE);
    }
    #endif
#endif

HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
    if(fgRandomizer == TRUE)
    {
        NANDHW_Random_Mizer_Decode(_u4CurPage, FALSE);
    }
    #endif
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ProbeDev()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ProbeDev(void)
{
    NAND_FLASH_DEV_T *prDev;
    UINT32 i, u4ID, u4DevCount;

    NANDHW_ReadID(&u4ID);
    LOG(1, "Detect NAND flash ID: 0x%X\n", u4ID);

    u4DevCount = NANDDev_GetDevCount();
    for (i = 0; i < u4DevCount; i++)
    {
        prDev = (NAND_FLASH_DEV_T *)NANDDev_GetDev(i);
        if ((u4ID & prDev->u4IDMask) == (prDev->u4IDVal & prDev->u4IDMask))
        {
            _prFlashDev = prDev;
            return NAND_SUCC;
        }
    }

    _prFlashDev = NULL;
    return NAND_FAIL;
}

static void part_enc_init(void)
{
    UINT32 i, u4Size = 0, u4Offset = 0, u4Encrypt = 0;
    mtk_part_info_t *p;

    for (i = 0; i < NANDPART_PARTNUM; i++)
    {
        p = &(rMTKPartInfo[i]);

        u4Offset = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartOffset0 + i));
        u4Size = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartSize0 + i));
        
#if defined(CC_MTD_ENCRYPT_SUPPORT)
        u4Encrypt = DRVCUST_InitGet((QUERY_TYPE_T)(eNANDFlashPartEncypt0 + i));
#endif
        
        p->u4PgStart = u4Offset/_u4PageSize;
        p->u4PgEnd = (u4Offset + u4Size)/_u4PageSize - 1;
        p->u4Encryped = u4Encrypt;
    }

#if defined(CC_MTD_ENCRYPT_SUPPORT) && !defined(CC_MTK_LOADER)
    {
        UINT8 u1Key[16] = {0};
        DMX_NAND_AES_INIT(u1Key, 128);
    }
#endif // CC_MTD_ENCRYPT_SUPPORT
}

//-----------------------------------------------------------------------------
/** _NANDHW_SetClock()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetClock(void)
{
#if defined(CC_MT5389)
    IO_WRITE32(0, 0xF000D260, 4);   // 120MHZ
#elif defined(CC_MT5368) || defined(CC_MT5396)
    IO_WRITE32(0, 0xF000D260, 6);   // 162MHZ
#else
    IO_WRITE32(0, 0xF000D260, 6);   // 216MHZ
#endif
}

//-----------------------------------------------------------------------------
/** NANDHW_Init()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_Init(void)
{
    UINT32 i;

    if (_fgNandInit)
    {
        return NAND_SUCC;
    }

    _fgIsrEn = FALSE;
    _fgDmaEn = FALSE;
    _rECCType = NAND_ECC_SOFT;
    _u4BlkPgCount = 64;
    _prFlashDev = NULL;
    _u4PgAddrLen  = 2;
    _u4TotalAddrLen = 5;
    _u4PageSize = 2048;
    _u4OOBSize = 16;
    _u4SectorSize = 1024;

    _NANDHW_SetClock();

    NAND_WRITE32(NAND_NFI_ACCCON1, 0x00411888);
    NAND_WRITE32(NAND_NFI_ACCCON2, 0x00080008);
    
    for (i = 0; i < 3; i++)
    {
        // Reset NFI Controller
        _NANDHW_NFI_Reset();
        NANDHW_Reset();

        _prFlashDev = NULL;
        
        if (_NANDHW_ProbeDev() == NAND_SUCC)
        {
            if ((_prFlashDev->u4IDVal & _prFlashDev->u4IDMask) & 0x000C0000)
            {
                _fgMLC = TRUE;
                LOG(1, "Detect %s NAND flash(MLC): %dMB\n", 
                    _prFlashDev->acName, (_prFlashDev->u4ChipSize/0x100000));
            }
            else
            {
                _fgMLC = FALSE;
                LOG(1, "Detect %s NAND flash(SLC): %dMB\n", 
                    _prFlashDev->acName, (_prFlashDev->u4ChipSize/0x100000));
            }
             
            _u4PageSize = _prFlashDev->u4PageSize;
            _u4OOBSize =  _prFlashDev->u4OOBSize;
            _NANDHW_SetSectorSize();
            _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
            _NANDHW_NFIECC_Cfg(NAND_ECC_NONE);
            
            if (_prFlashDev->u4PageSize == 512)
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 1);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
            }
            else
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 2);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
            }
            
            _u4BlkPgCount = _prFlashDev->u4BlkPgCount;
            break;
        }
        
        LOG(0, "Not detect any flash\n");
        _u4PgAddrLen = 2;
        _u4TotalAddrLen = 5;
        _u4PageSize = 2048;
        _u4OOBSize = 16;
    }
 
#if NAND_ENABLE_ISR
     _NANDHW_RegIsr();
#endif

    _NANDHW_SetTiming();
    _NANDHW_NFI_SetPageFmt();
    _u4SectorNum = _u4PageSize/_u4SectorSize;

    // Aligned the buffer for DMA used.
#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
    MTDNANDGetDmaBuf(&_pu1NandDataBuf, &_pu1NandOobBuf);
#else
    _pu1NandDataBuf = (UINT8 *)((((UINT32)_u4NandDataBuf >> 6) << 6) + 64);
    _pu1NandOobBuf  = (UINT8 *)_u4NandOobBuf;
#endif

#if !(defined(CC_MT5398) && defined(CC_NAND_60BIT))
#if NAND_SCRAMBLE_EN
    NANDHW_SetScramble(TRUE);
#endif
#endif

    part_enc_init();

    _fgNandInit = TRUE;
    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** NANDHW_SetControl()
 */
//-----------------------------------------------------------------------------
void NANDHW_SetControl(UINT32 u4Flag, NAND_CONTROL_T *prCtrl)
{
    if (u4Flag & NAND_CTRL_FLAGS_ECC)
    {
        _rECCType = prCtrl->rEccType;
    }

    if (u4Flag & NAND_CTRL_FLAGS_DMA)
    {
        _fgDmaEn = prCtrl->fgDMA;
    }

    if (u4Flag & NAND_CTRL_FLAGS_ISR)
    {
#if NAND_ENABLE_ISR
        _fgIsrEn = prCtrl->fgIsr;
#else
        _fgIsrEn = FALSE;
        UNUSED(_fgIsrEn);
#endif
    }
}

//-----------------------------------------------------------------------------
/** NANDHW_GetControl()
 */
//-----------------------------------------------------------------------------
void NANDHW_GetControl(UINT32 u4Flag, NAND_CONTROL_T *prCtrl)
{
    if (u4Flag & NAND_CTRL_FLAGS_ECC)
    {
        prCtrl->rEccType = _rECCType;
    }

    if (u4Flag & NAND_CTRL_FLAGS_DMA)
    {
        prCtrl->fgDMA = _fgDmaEn;
    }

    if (u4Flag & NAND_CTRL_FLAGS_ISR)
    {
#if NAND_ENABLE_ISR
        prCtrl->fgIsr = _fgIsrEn;
#else
        prCtrl->fgIsr = FALSE;
#endif
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_GetEcc()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_NFIECC_GetEcc(UINT32 *pu4EccCode)
{
    UINT32 u4Val, i;
    UINT32 u4EccCodeBit, u4EccCodeDWord, u4EccModBit;

    // the redundant bit of parity bit must be 1 for rom code bug. -- 20110328 mtk40109
    u4EccCodeBit = _u4ECCBits * NAND_NFIECC_1BIT;
    u4EccModBit = u4EccCodeBit % 32;
    u4EccCodeDWord = u4EccModBit ? ((u4EccCodeBit>>5) + 1) : (u4EccCodeBit>>5);
    ASSERT(u4EccCodeDWord <= NAND_NFI_ENCPAR_NUM);

    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCSTA);
        u4Val &= NAND_NFIECC_ENCSTA_FSM_MASK;
    } while (u4Val != NAND_NFIECC_ENCSTA_FSM_PAROUT);

    for (i = 0; i < u4EccCodeDWord; i++)
    {
        pu4EccCode[i] = NAND_READ32(NAND_NFIECC_ENCPAR0 + (i<<2));
    }

    for (i = u4EccCodeDWord; i < NAND_NFI_ENCPAR_NUM; i++)
    {
        pu4EccCode[i] = 0xFFFFFFFF;
    }

    if (u4EccModBit)
    {
        pu4EccCode[u4EccCodeDWord - 1] |= (~(UINT32)((1<<u4EccModBit) - 1));
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** NANDHW_GetInfo()
 */
//-----------------------------------------------------------------------------
void NANDHW_GetInfo(NANDDEV_T *prNand)
{
    if (_prFlashDev)
    {
        prNand->fgMLC = _fgMLC;
        prNand->u4PgSz = _prFlashDev->u4PageSize;
        prNand->u4OOBSz = _prFlashDev->u4OOBSize;
        prNand->u4BlkSz = _prFlashDev->u4BlkSize;
        prNand->u4TotalSize = _prFlashDev->u4ChipSize;
        prNand->u4TotalBlk = _prFlashDev->u4ChipSize / _prFlashDev->u4BlkSize;
        prNand->u4PgPerBlk = _prFlashDev->u4BlkSize / _prFlashDev->u4PageSize;
        prNand->u4BBMode = _prFlashDev->u4BBMode;
    }
}

//-----------------------------------------------------------------------------
/** NANDHW_ReadStatus()
 */
//-----------------------------------------------------------------------------
UINT32 NANDHW_ReadStatus(void)
{
    UINT32 u4Val;

    _NANDHW_NFI_Reset();

    NAND_WRITE32(NAND_NFI_CNFG, NAND_NFI_CNFG_OP_SINGLE | NAND_NFI_CNFG_READ_MODE | NAND_NFI_CNFG_AHB_MODE);
    _NANDHW_Command(NAND_CMD_STATUS);
    NAND_WRITE32(NAND_NFI_CON, NAND_NFI_CON_NOB(4) | NAND_NFI_CON_SRD);
    _NANDHW_WaitBusy();
    
    _NANDHW_WaitRead(4);
    u4Val = NAND_READ32(NAND_NFI_DATAR);
    return u4Val;
}

//-----------------------------------------------------------------------------
/** NANDHW_EraseBlock()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_EraseBlock(UINT32 u4BlkIdx)
{
    INT32 iRet = NAND_SUCC;
    UINT32 u4Val;
    UINT32 u4PgIdx = u4BlkIdx * _u4BlkPgCount;

    _NANDHW_NFI_Reset();
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, FALSE, NAND_NFI_CNFG_OP_ERASE);
    
    _NANDHW_Command(NAND_CMD_ERASE1_BLK);
    _NANDHW_Addr(u4PgIdx, 0, _u4PgAddrLen<<4);
    
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_ERASE_DONE);
    }
#endif

    /*Trigger Erase. */
    NAND_WRITE32(NAND_NFI_CMD, NAND_CMD_ERASE2_BLK);

#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_ERASE_DONE);
    }
#endif

    _NANDHW_WaitBusy();
    
    while (1)
    {
        u4Val = NANDHW_ReadStatus();
        if (u4Val & STATUS_READY_BUSY)
        {
            break;
        }
    }

    if (u4Val & STATUS_PASS_FAIL)
    {
        LOG(0, "NANDHW_EraseBlock failed: u4BlkIdx = %d!\n", u4BlkIdx);
        iRet = NAND_FAIL;
    }

    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return iRet;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WriteFifo()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WriteFifo(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    
    ASSERT(pu4Buf != NULL);
    
    for (i = 0; i < (u4Len>>2); i++)
    {
        _NANDHW_WaitWrite();
        NAND_WRITE32(NAND_NFI_DATAW, pu4Buf[i]);
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_WritePage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_WritePage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx)
{
    UINT32 u4Val;
    UINT8 u1EccCode[NAND_NFI_ENCPAR_NUM<<2];
    INT32 i4Ret = NAND_SUCC;
    
    ASSERT(pu1Buf != NULL);
    ASSERT(pu1Spare != NULL);
    
    _u4CurPage = u4PgIdx;

    if (_fgDmaEn)
    {
        HalDmaBufferStart((void *)pu1Buf, _u4PageSize, HAL_DMA_TO_DEVICE);
    }

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
        NANDHW_Random_Mizer_Encode(_u4CurPage, TRUE);
    #endif
#endif

    //Set Dma
    if (_fgDmaEn)
    {
        _NANDHW_NFI_SetWtDma((UINT32 *)pu1Buf, (UINT32 *)pu1Spare);
    }
    else if (_rECCType == NAND_ECC_HARD)
    {
        _NANDHW_NFI_SetWtDma(NULL, (UINT32 *)pu1Spare);
    }

#ifdef TOOLDRIVER
    NAND_WRITE32(NAND_NFI_TCMCON, NAND_NFI_RS232_WR);
#endif

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_PROGRAM);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_PROGRAM);

    /*Write Command*/
    _NANDHW_Command(NAND_CMD_INPUT_PAGE);
    _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));

#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_AHB_DONE);
    }
#endif

    /*Trigger Write . */
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_PROGRAM);

#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_AHB_DONE);
    }
#endif

    _NANDHW_WaitBusy();
    
    if (_fgDmaEn)
    {
        _NANDHW_NFIECC_WaitBusy();
    }
    else
    {
        UINT32 u4SectIdx, u4SectOobSz;
        u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
        
        for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
        {
            // write main data
            _NANDHW_NFI_WriteFifo(&pu1Buf[u4SectIdx*_u4SectorSize], _u4SectorSize);
            
            if (_rECCType == NAND_ECC_SOFT)
            {
                // write fdm data
                _NANDHW_NFI_WriteFifo(&pu1Spare[u4SectIdx*u4SectOobSz], _u4FdmNum);
                
                /* ECC correction. */
                _NANDHW_WaitWrite();
                if (_NANDHW_NFIECC_GetEcc((UINT32 *)u1EccCode))
                {
                    i4Ret = NAND_ECC_ERROR;
                    goto HandleExit;
                }

                // write ecc data
                u4Val = u4SectOobSz - _u4FdmNum;
                _NANDHW_NFI_WriteFifo(u1EccCode, u4Val);
            }
            else if (_rECCType == NAND_ECC_NONE)
            {
                // write spare data
                _NANDHW_NFI_WriteFifo(&pu1Spare[u4SectIdx*u4SectOobSz], u4SectOobSz);
            }
            else if (_rECCType == NAND_ECC_HARD)
            {
                // HW ECC do nothing
            }
            else
            {
                ASSERT(0);
            }
        }
    }
    
    _NANDHW_NFI_WaitSectorCnt();
    _NANDHW_WaitBusy();

    /* Trigger NAND to program*/
    _NANDHW_Command(NAND_CMD_PROG_PAGE);

    // Read status back
    while (1)
    {
        u4Val = NANDHW_ReadStatus();
        if (u4Val & STATUS_READY_BUSY)
        {
            break;
        }
    }
    
    if (u4Val & STATUS_PASS_FAIL)
    {
        LOG(0, "Write page failed, pgidx:%d\n", u4PgIdx);
        i4Ret = NAND_FAIL;
        goto HandleExit;
    }
    
    if (_fgDmaEn)
    {
        HalDmaBufferDone((void *)pu1Buf, _u4PageSize, HAL_DMA_TO_DEVICE);
    }
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
        NANDHW_Random_Mizer_Encode(_u4CurPage, FALSE);
    #endif
#endif

HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
        NANDHW_Random_Mizer_Encode(_u4CurPage, FALSE);
    #endif
#endif  

#ifdef TOOLDRIVER
    NAND_WRITE32(NAND_NFI_TCMCON, 0);
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_Page_Enctyped()
 */
//-----------------------------------------------------------------------------
#ifdef CC_MTD_ENCRYPT_SUPPORT
static int _NANDHW_Page_Enctyped(UINT32 u4PgIdx, UINT32 *pu4Data)
{
    UINT32 i, j;
    mtk_part_info_t *p;
    
    for (i = 0; i < NANDPART_PARTNUM; i++)
    {
        p = &(rMTKPartInfo[i]);
        
        if (u4PgIdx >= p->u4PgStart && u4PgIdx <= p->u4PgEnd)
        {
            if (p->u4Encryped == 1)
            {
                for (j = 0; j < _u4PageSize/4; j++)
                {
                    if (pu4Data[j] != 0xFFFFFFFF)
                    {
                        return 1;
                    }
                }
            }
            
            break;
        }
    }

    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** _NANDHW_Page_Empty()
 */
//-----------------------------------------------------------------------------
static int _NANDHW_Page_Empty(UINT32 *pu4Buf, UINT32 *pu4Spare)
{
    // If all data is 0xff, do not write the page. mtk40109 - 2010/4/23
    UINT32 i, u4PgOobSz;
    
    for (i = 0; i < _u4PageSize/4; i++)
    {
        if (pu4Buf[i] != 0xFFFFFFFF)
        {
            return 0;
        }
    }
    //for scan bad block in Random mizer, the function is closed to a new nand flash.
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN
    if(!pu4Spare)
    {
        return 1;
    }
    #endif
#endif

    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    for (i = 0; i < u4PgOobSz/4; i++)
    {
        if (pu4Spare[i] != 0xFFFFFFFF)
        {
            return 0;
        }
    }

    return 1;
}

//-----------------------------------------------------------------------------
/** NANDHW_ReadPageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_ReadPageAttr(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgIdx)
{
    INT32 i4Ret;
    UINT32 u4PgOobSz;

    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    
    i4Ret = _NANDHW_ReadPage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx);
       
    _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, _u4PageSize);

#ifdef CC_MTD_ENCRYPT_SUPPORT
    if (_NANDHW_Page_Enctyped(u4PgIdx, (UINT32 *)_pu1NandDataBuf))
    { 
        DMX_NAND_AES_Decryption(PHYSICAL((UINT32)_pu1NandDataBuf), PHYSICAL((UINT32)_pu1NandDataBuf), _u4PageSize);
    }
#endif
        
    x_memcpy(pu4Buf, _pu1NandDataBuf, _u4PageSize);
    x_memcpy(pu4Spare, _pu1NandOobBuf, u4PgOobSz);
    
// use poi read to verify dma read
#if NAND_DMA_READ_VERIFY
    {
        UINT32 i;
        NAND_CONTROL_T rOrgCtl, rNewCtl;
        UINT32 *pu4Buf1 = (UINT32 *)pu4Buf;
        UINT32 *pu4Buf2 = (UINT32 *)_pu1NandDataBuf;
        
        memset(_pu1NandDataBuf, 0xA5, _u4PageSize);
        memset(_pu1NandOobBuf,  0x5A, u4PgOobSz);
        
        NANDHW_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtl);
        
        rNewCtl.fgIsr = FALSE;
        rNewCtl.fgDMA = FALSE;
        rNewCtl.rEccType = NAND_ECC_SOFT;
        NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rNewCtl);
        
        i4Ret = _NANDHW_ReadPage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx);
           
        _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, _u4PageSize);

    #ifdef CC_MTD_ENCRYPT_SUPPORT
        if (_NANDHW_Page_Enctyped(u4PgIdx, (UINT32 *)_pu1NandDataBuf))
        { 
            DMX_NAND_AES_Decryption(PHYSICAL((UINT32)_pu1NandDataBuf), PHYSICAL((UINT32)_pu1NandDataBuf), _u4PageSize);
        }
    #endif

        for (i = 0; i < _u4PageSize/4; i++)
        {
            if (pu4Buf1[i] != pu4Buf2[i])
            {
                LOG(0, "read verify fail(mtk): page=0x%X, offset=0x%X, dma=0x%08X, pio=0x%08X\n",
                	u4PgIdx, i*4, pu4Buf1[i], pu4Buf2[i]);
            }
        }
        
        NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtl);
    }
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NANDHW_WritePageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_WritePageAttr(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgIdx)
{
    INT32 i4Ret;
    UINT32 u4PgOobSz;

#ifndef TOOLDRIVER
    // The loader partition cannot ignore empty page cause rom code restrictition. 2010-12-07 mtk40109
    if (u4PgIdx > rMTKPartInfo[0].u4PgEnd)
    {
        if (_NANDHW_Page_Empty(pu4Buf, pu4Spare))
        {
            return NAND_SUCC;
        }
    }
#endif
    
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    x_memcpy(_pu1NandDataBuf, pu4Buf, _u4PageSize);
    x_memcpy(_pu1NandOobBuf, pu4Spare, u4PgOobSz);
    
#ifdef CC_MTD_ENCRYPT_SUPPORT
    if (_NANDHW_Page_Enctyped(u4PgIdx, (UINT32 *)_pu1NandDataBuf))
    {
        DMX_NAND_AES_Encryption(PHYSICAL((UINT32)_pu1NandDataBuf), PHYSICAL((UINT32)_pu1NandDataBuf), _u4PageSize);
    }
#endif
    
    _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, _u4PageSize);
    i4Ret = _NANDHW_WritePage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx);
    
    return i4Ret;
}

#if defined(CC_NAND_WRITE_VERIFY)
INT32 NANDHW_VerifyPageAttr(UINT32 u4PgIdx)
{
   INT32 i4Ret;

   i4Ret = _NANDHW_ReadPage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx);
   if (i4Ret)
   {  
      LOG(0, "NANDHW_VerifyPageAttr read error! page = 0x%x\n", u4PgIdx);
      return NAND_FAIL;
   }  

   if (_u4CurBitflip)
   {
      LOG(0, "NANDHW_VerifyPageAttr bitflip occur! page = 0x%x\n", u4PgIdx);
      return NAND_FAIL;
   }
   return 0;
}
#endif

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadSpare()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_ReadSpare(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i, u4Val = 0;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    INT32  i4Ret = NAND_SUCC;

    ASSERT(pu1Buf != NULL);

    for (i = 0; i < u4Len; i += 4)
    {
        _NANDHW_WaitRead(4);
        pu4Buf[u4Val] = NAND_READ32(NAND_NFI_DATAR);
        u4Val++;
    }
    
    if (_rECCType == NAND_ECC_SOFT)
    {
        if (u4Len == (_u4SectorSize + ((_u4OOBSize * _u4SectorSize)>>9)))
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, &pu1Buf[_u4SectorSize], 0))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }
    }

    _NANDHW_WaitBusy();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadSpare()
 */
//-----------------------------------------------------------------------------
INT32 _NANDHW_ReadSpare(UINT8 *pu1Buf, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;
    ASSERT(pu1Buf != NULL);
    
    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    /* Read command. */
    if ((u4PgOff == 0x200) && (_u4PageSize == 0x200))
    {
        _NANDHW_Command(NAND_CMD_READ2);
        _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    else
    {
        _NANDHW_Command(NAND_CMD_READ1_00);
        _NANDHW_Addr(u4PgIdx, u4PgOff, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    
    if (_u4PageSize != 0x200)
    {
        _NANDHW_Command(NAND_CMD_READ_30);
    }

    /*Trigger Read . */
    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
        goto HandleExit;
    }
    
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
    
    _NANDHW_WaitBusy();
    i4Ret = _NANDHW_NFI_ReadSpare(pu1Buf, u4Len);
    if (i4Ret)
    {
        goto HandleExit;
    }
    
HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_IsBadPage()
 */
//-----------------------------------------------------------------------------
static BOOL _NANDHW_IsBadPage(UINT32 u4PgIdx)
{
    INT32 iRet;
    BOOL isEmpty = TRUE;
    NAND_CONTROL_T rOrgCtl, rNewCtl;
    UINT32 u4BitCnt, u4Offset, u4OobLastIdx, i;
    UINT32 u4SectOobSz, u4PgOobSz, u4EccSectSz;
    
    UINT8 *pu1Data  = (UINT8 *)_pu1NandDataBuf;
    UINT8 *pu1Spare = (UINT8 *)_pu1NandOobBuf;
    
    _u4CurPage = u4PgIdx;
    
    NANDHW_GetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtl);
    
    rNewCtl.fgIsr = FALSE;
    rNewCtl.fgDMA = FALSE;
    rNewCtl.rEccType = NAND_ECC_NONE;
    NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rNewCtl);

    u4SectOobSz =  (_u4OOBSize * _u4SectorSize)>>9;
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    u4EccSectSz = _u4SectorSize + u4SectOobSz;

    // read spare data without ecc
    u4Offset = _u4PageSize;
    
    //for Random mizer, new flash block or erased block cannot be scan by Random mizer mode.
#if defined(CC_MT5398) && defined(CC_NAND_60BIT)
    #if NAND_RANDOM_EN    
    fgRandomizer = FALSE;
    iRet = NANDHW_ReadPageAttr((UINT32 *)pu1Data, (UINT32 *)pu1Spare, u4PgIdx);
    if (iRet)
    {
        fgRandomizer = TRUE;
        goto HandleBad;
    }
    else if(!_NANDHW_Page_Empty((UINT32 *)pu1Data, NULL))
    {   
        isEmpty = FALSE;
        fgRandomizer = TRUE;
        iRet = NANDHW_ReadPageAttr((UINT32 *)pu1Data, (UINT32 *)pu1Spare, u4PgIdx);
    }
    else
    {
        isEmpty = TRUE;
        fgRandomizer = TRUE;
    }
    #endif
#endif

    if(isEmpty == TRUE)
    {
        if ((_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT) == 1)
        {
           iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, _u4OOBSize);
        }
        else
        {
           iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, u4PgOobSz);
        }
    }
    if (iRet)
    {
        goto HandleBad;
    }
    
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_BBMARK) == 0x00000500)
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[5]) + _NANDHW_countbits(pu1Spare[6]);
    }
    else
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[0]) + _NANDHW_countbits(pu1Spare[1]);
    }
    
    switch (_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT)
    {
    case 0:
        ASSERT(0);
        break;
        
    case 1:
        if (u4BitCnt < 15)
        {
            goto HandleBad;
        }
        else
        {
            goto HandleGood;
        }
        
    default:
        u4OobLastIdx = u4PgOobSz - u4SectOobSz;
        for (i = 0; i < 6; i++)
        {
            u4BitCnt += _NANDHW_countbits(pu1Spare[u4OobLastIdx + i]);
        }
        
        if (u4BitCnt > 62)
        {
            goto HandleGood;
        }
        
        if (u4BitCnt < 2)
        {
            goto HandleBad;
        }
        
        break;
    }
    
    rNewCtl.rEccType = NAND_ECC_SOFT;
    NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rNewCtl);

    // read last sector data with ecc
    u4Offset = (_u4SectorNum - 1) * u4EccSectSz;
    iRet = _NANDHW_ReadSpare(pu1Data, u4PgIdx, u4Offset, u4EccSectSz);
    if (iRet)
    {
        goto HandleBad;
    }
    
    u4OobLastIdx = u4EccSectSz - (u4SectOobSz * _u4SectorNum);
    if ((pu1Data[u4OobLastIdx] != 0xFF) || (pu1Data[u4OobLastIdx + 1] != 0xFF))
    {
        goto HandleBad;
    }
    
HandleGood:
    NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtl);
    return FALSE;       // Good block
    
HandleBad:
    NANDHW_SetControl(NAND_CTRL_FLAGS_ALL, &rOrgCtl);
    return TRUE;       // bad block
}

//-----------------------------------------------------------------------------
/** NANDHW_IsBadBlk()
 */
//-----------------------------------------------------------------------------
BOOL NANDHW_IsBadBlk(UINT32 u4BlkIdx)
{
    UINT32 u4PgIdx = u4BlkIdx * _u4BlkPgCount;

    // check the 1st page of the block.
    if (_NANDHW_IsBadPage(u4PgIdx))
    {
        return TRUE;
    }
    
    // check the last page of the block if MLC.
    if (_u4OOBSize > 16)
    {
        if (_NANDHW_IsBadPage(u4PgIdx + _u4BlkPgCount -1))
        {
            return TRUE;
        }
    }
    
    return FALSE;
}

//-----------------------------------------------------------------------------
/** NANDHW_MarkBadPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_MarkBadPage(UINT32 u4PgIdx)
{
    _u4CurPage = u4PgIdx;
    
    x_memset(_pu1NandDataBuf, 0, _u4PageSize);
    x_memset(_pu1NandOobBuf, 0, (_u4OOBSize * _u4PageSize)>>9);
    
    return  (_NANDHW_WritePage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx));
}

//-----------------------------------------------------------------------------
/** NANDHW_MarkBadBlk()
 */
//-----------------------------------------------------------------------------
BOOL NANDHW_MarkBadBlk(UINT32 u4BlkIdx)
{
    BOOL bRet = TRUE;
    UINT32 u4PgIdx = u4BlkIdx * _u4BlkPgCount;
    
    LOG(0, "NANDHW_MarkBadBlk: %d\n", u4BlkIdx);
    
    // Erase the block before mark bad.
    NANDHW_EraseBlock(u4BlkIdx);
    
    if (_NANDHW_MarkBadPage(u4PgIdx))
    {
        bRet = FALSE;
    }

    // Mark the last page of the block if MLC.
    if (_u4OOBSize > 16)
    {    
        if (_NANDHW_MarkBadPage(u4PgIdx + _u4BlkPgCount - 1))
        {
            bRet = FALSE;
        }
    }
    
    return bRet;
}

#if NAND_MULTIPAGE_ACCESS
//-----------------------------------------------------------------------------
/** _NANDHW_ReadMultiPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ReadMultiPage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx, UINT32 u4PgCnt)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 u4DmaRetryCnt = 0;
    UINT32 *pu4DmaBuf = (UINT32 *)pu1Buf;
    
    ASSERT(pu1Buf != NULL);
    ASSERT(pu1Spare != NULL);

    pu4DmaBuf[_u4PageSize*u4PgCnt/4-1] = NAND_DMA_PATTERN;
    HalDmaBufferStart((void *)(&pu1Buf[_u4PageSize*u4PgCnt - NAND_DMA_CACHE_LINE]), NAND_DMA_CACHE_LINE, HAL_DMA_TO_DEVICE);
    HalDmaBufferDone((void *)(&pu1Buf[_u4PageSize*u4PgCnt - NAND_DMA_CACHE_LINE]), NAND_DMA_CACHE_LINE, HAL_DMA_TO_DEVICE);
    HalDmaBufferStart((void *)pu1Buf, _u4PageSize*u4PgCnt, HAL_DMA_FROM_DEVICE);

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();
    
    /* Set DMA & SRAM. */
    _NANDHW_NFIMDMA_SetRdSRAM((UINT32 *)pu1Buf, u4PgCnt);
    _NANDHW_NFIMDMA_Cfg(u4PgCnt, NAND_NFI_CNFG_OP_READ);

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    
    /* Write address & sector number. */
    _NANDHW_NFIMDMA_Addr((_u4PgAddrLen<<4) | (_u4TotalAddrLen - _u4PgAddrLen));
    _NANDHW_NFIMDMA_SectNum();
    
    /*Trigger MDMA Read. */
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_MDMA_DONE);
    }
    else
#endif
    {
        NAND_READ32(NAND_NFI_INTR);      // Clear interrupt
        NAND_WRITE32(NAND_NFI_INTR_EN, NAND_NFI_INTR_EN_MDMA_DONE);
    }

    _NANDHW_NFIMDMA_Trig();

#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_MDMA_DONE);
    }
    else
#endif
    {
        while (1)
        {
            if (NAND_READ32(NAND_NFI_INTR) & NAND_NFI_INTR_MDMA_DONE)
            {
                break;
            }
        }
        
        NAND_WRITE32(NAND_NFI_INTR_EN, 0);
    }

    _NANDHW_WaitBusy();
    _NANDHW_NFIECC_WaitBusy();

    // Get FDM data.
    _NANDHW_NFIMDMA_GetRdSRAM((UINT32 *)pu1Spare, u4PgCnt);

    // Check ECC done
    _NANDHW_NFIMDMA_CheckEccDone();
    
    // Cause nand dma read may send finish interrupt before write all data into dram.
    // So we add a pattern at last location to check if dma read is finished.
    while (1)
    {
        HalDmaBufferDone((void *)pu1Buf, _u4PageSize*u4PgCnt, HAL_DMA_FROM_DEVICE);
        
        if (pu4DmaBuf[_u4PageSize*u4PgCnt/4-1] != NAND_DMA_PATTERN)
        {
            if (u4DmaRetryCnt > 0)
            {
                LOG(0, "mtk nand driver: u4DmaTryCnt = %d, pageidx = 0x%08X\n", u4DmaRetryCnt, u4PgIdx);
            }
            break;
        }

        u4DmaRetryCnt++;
        if (u4DmaRetryCnt >= NAND_DMA_RETYR_CNT)
        {
            LOG(0, "mtk nand driver: dma pattern check time-out, pageidx = 0x%08X\n", u4PgIdx);
            break;
        }
        
        HAL_Delay_us(NAND_DMA_RETYR_DELAY);
    }
            
    // Cause pattern check and CPU prefetch will update cache.
    // Invalidate whole cache to ensure bit-flips writen into dram.
    HalDmaBufferDone((void *)pu1Buf, _u4PageSize*u4PgCnt, HAL_DMA_FROM_DEVICE);
    
    i4Ret = _NANDHW_NFIMDMA_CheckEcc(pu1Buf, pu1Spare, u4PgCnt);
    if (i4Ret)
    {
        goto HandleExit;
    }
    
HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_MDMACON, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NANDHW_ReadMultiPageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_ReadMultiPageAttr(UINT32 *pu4Buf , UINT32 *pu4Spare, UINT32 u4PgIdx, UINT32 u4PgCnt)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 i, u4PgOobSz;
    UINT32 *pu4DestPgBuf, *pu4RdPgBuf, *pu4RdOobBuf;
    
    for (i = 0; i < u4PgCnt; i++)
    {
        _u4MultiPage[i] = u4PgIdx + i;
    }
    
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    i4Ret = _NANDHW_ReadMultiPage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx, u4PgCnt);
    
    for (i = 0; i < u4PgCnt; i++)
    {
        pu4DestPgBuf = (UINT32 *)((UINT32)pu4Buf + _u4PageSize * i);
        pu4RdPgBuf = (UINT32 *)((UINT32)_pu1NandDataBuf + _u4PageSize * i);
        pu4RdOobBuf = (UINT32 *)((UINT32)_pu1NandOobBuf + u4PgOobSz * i);
        
        x_memcpy(pu4Spare, pu4RdOobBuf, u4PgOobSz);
        _NANDHW_DataExchange(pu4RdPgBuf, pu4Spare, 0, _u4PageSize);
        x_memcpy(pu4DestPgBuf, pu4RdPgBuf, _u4PageSize);
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_WriteMultiPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_WriteMultiPage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx, UINT32 u4PgCnt)
{
    UINT32 u4Val;
    INT32 i4Ret = NAND_SUCC;
    
    ASSERT(pu1Buf != NULL);
    ASSERT(pu1Spare != NULL);

	HalDmaBufferStart((void *)pu1Buf, _u4PageSize * u4PgCnt, HAL_DMA_TO_DEVICE);

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    /* Set DMA & SRAM. */
    _NANDHW_NFIMDMA_SetWtSRAM((UINT32 *)pu1Buf, (UINT32 *)pu1Spare, u4PgCnt);
    _NANDHW_NFIMDMA_Cfg(u4PgCnt, NAND_NFI_CNFG_OP_PROGRAM);

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_PROGRAM);
    _NANDHW_NFIECC_Cfg(_rECCType);

    /* Write address & sector number. */
    _NANDHW_NFIMDMA_Addr((_u4PgAddrLen<<4) | (_u4TotalAddrLen - _u4PgAddrLen));
    _NANDHW_NFIMDMA_SectNum();
    
    /*Trigger MDMA Write. */
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_MDMA_DONE);
    }
    else
#endif
    {
        NAND_READ32(NAND_NFI_INTR);      // Clear interrupt
        NAND_WRITE32(NAND_NFI_INTR_EN, NAND_NFI_INTR_EN_MDMA_DONE);
    }

    _NANDHW_NFIMDMA_Trig();

#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_MDMA_DONE);
    }
    else
#endif
    {
        while (1)
        {
            if (NAND_READ32(NAND_NFI_INTR) & NAND_NFI_INTR_MDMA_DONE)
            {
                break;
            }
        }
        
        NAND_WRITE32(NAND_NFI_INTR_EN, 0);
    }

    _NANDHW_WaitBusy();
    _NANDHW_NFIECC_WaitBusy();
        
    _NANDHW_NFI_WaitSectorCnt();
    _NANDHW_WaitBusy();

    // Read status back
    while (1)
    {
        u4Val = NANDHW_ReadStatus();
        if (u4Val & STATUS_READY_BUSY)
        {
            break;
        }
    }
    
    if (u4Val & STATUS_PASS_FAIL)
    {
        LOG(0, "_NANDHW_WriteMultiPage failed\n");
        i4Ret = NAND_FAIL;
        goto HandleExit;
    }

	HalDmaBufferDone((void *)pu1Buf, _u4PageSize * u4PgCnt, HAL_DMA_TO_DEVICE);

HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_MDMACON, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NANDHW_WriteMultiPageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_WriteMultiPageAttr(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgIdx, UINT32 u4PgCnt)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 i, u4PgOobSz, u4WtPgCnt = 0;
    UINT32 *pu4SrcPgBuf, *pu4WtPgBuf, *pu4WtOobBuf;

    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    
    for (i = 0; i < u4PgCnt; i++)
    {
        pu4SrcPgBuf = (UINT32 *)((UINT32)pu4Buf + _u4PageSize * i);
        pu4WtPgBuf = (UINT32 *)((UINT32)_pu1NandDataBuf + _u4PageSize * u4WtPgCnt);
        pu4WtOobBuf = (UINT32 *)((UINT32)_pu1NandOobBuf + u4PgOobSz * u4WtPgCnt);
        
        if (_NANDHW_Page_Empty(pu4SrcPgBuf, pu4Spare))
        {
            continue;
        }
        
        x_memcpy(pu4WtPgBuf, pu4SrcPgBuf, _u4PageSize);
        x_memcpy(pu4WtOobBuf, pu4Spare, u4PgOobSz);
        _NANDHW_DataExchange(pu4WtPgBuf, pu4WtOobBuf, 0, _u4PageSize);
        
        _u4MultiPage[u4WtPgCnt] = u4PgIdx + i;
        u4WtPgCnt++;
    }

    if (u4WtPgCnt)
    {
        i4Ret = _NANDHW_WriteMultiPage(_pu1NandDataBuf, _pu1NandOobBuf, u4PgIdx, u4WtPgCnt);
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** NANDHW_EraseMultiBlock()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_EraseMultiBlock(UINT32 u4BlkIdx, UINT32 u4BlkCnt)
{
    INT32 iRet = NAND_SUCC;
    UINT32 i, u4Val;
    
    _NANDHW_NFI_Reset();
    
    for (i = 0; i < u4BlkCnt; i++)
    {
        _u4MultiPage[i] = (u4BlkIdx + i) * _u4BlkPgCount;
        
        NAND_WRITE32(NAND_NFI_MDMAADDR, i+NAND_NFI_MDMAADDR_ROWADDR);
        NAND_WRITE32(NAND_NFI_MDMADATA, _u4MultiPage[i]);
    }
    
    _NANDHW_NFIMDMA_Cfg(u4BlkCnt, NAND_NFI_CNFG_OP_ERASE);
    
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, FALSE, NAND_NFI_CNFG_OP_ERASE);
    _NANDHW_NFIMDMA_Addr(_u4PgAddrLen<<4);
    
    /*Trigger MDMA Erase. */
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_SetIsr(NAND_NFI_INTR_EN_MDMA_DONE);
    }
    else
#endif
    {
        NAND_READ32(NAND_NFI_INTR);      // Clear interrupt
        NAND_WRITE32(NAND_NFI_INTR_EN, NAND_NFI_INTR_EN_MDMA_DONE);
    }

    _NANDHW_NFIMDMA_Trig();
    
#if NAND_ENABLE_ISR
    if (_fgIsrEn)
    {
        _NANDHW_WaitIsr(NAND_NFI_INTR_MDMA_DONE);
    }
    else
#endif
    {
        while (1)
        {
            if (NAND_READ32(NAND_NFI_INTR) & NAND_NFI_INTR_MDMA_DONE)
            {
                break;
            }
        }
        
        NAND_WRITE32(NAND_NFI_INTR_EN, 0);
    }

    _NANDHW_WaitBusy();
    
    while (1)
    {
        u4Val = NANDHW_ReadStatus();
        if (u4Val & STATUS_READY_BUSY)
        {
            break;
        }
    }

    if (u4Val & STATUS_PASS_FAIL)
    {
        LOG(0, "NANDHW_EraseMultiBlock failed: u4BlkIdx = %d, u4BlkCnt = %d!\n", u4BlkIdx, u4BlkCnt);
        iRet = NAND_FAIL;
    }

    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_MDMACON, 0);
    return iRet;
}
#endif  // NAND_MULTIPAGE_ACCESS

