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
 * $RCSfile: nandboot.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nandboot.c
 *  nandboot.c provides the low level register access function of nand device.
 */
 
#if defined(CC_NAND_BOOT) && defined(CC_MTK_PRELOADER)

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_assert.h"
#include "x_bim.h"
#include "nandboot.h"
#include "chip_txt.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define NAND_SUCC               (0)
#define NAND_FAIL               (1)
#define NAND_ECC_ERROR          (4)
#define NAND_CMD_ERROR          (8)
#define NAND_INVALID_ARG        (16)
#define POLLING_RETRY           (512*512)

//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
static BOOL _fgIsrEn = FALSE;
static BOOL _fgDmaEn = FALSE;
static NAND_ECC_TYPE_T _rECCType = NAND_ECC_SOFT;

static NAND_FLASH_DEV_T *_prFlashDev;
static UINT32 _u4BlkPgCount;
static UINT32 _u4FdmNum;
static UINT32 _u4SectorNum;
static UINT32 _u4PgAddrLen;
static UINT32 _u4TotalAddrLen;
static UINT32 _u4PageSize = 2048;
static UINT32 _u4OOBSize = 16;
static UINT32 _u4SectorSize = 1024;
static UINT32 _u4ECCBits = 0;
BOOL _fgNandInit = FALSE;

UINT8 pu1Spare[NAND_MAX_OOB_SIZE];
UINT8 pu1Data[(NAND_MAX_PAGE_SIZE + NAND_MAX_OOB_SIZE)/8];

#define ROMNANDTAG_0 0x2F4B544D
#define ROMNANDTAG_1 0x2F565444
#define ROMNANDTAG_2 0x434D4F52
#define ROMNANDTAG_3 0x2F45444F
#define ROMNANDTAG_4 0x444E414E
#define ROMNANDTAG_5 0x544F4F42
#define ROMNANDTAG_6 0x3130562F
#define ROMNANDTAG_7 0x0030302E

const static UINT32 _u4Signature[] __attribute__ ((section ("ROMNANDTAG"))) =
{
    ROMNANDTAG_0, ROMNANDTAG_1, ROMNANDTAG_2, ROMNANDTAG_3,     // MTK/DTV/ROMCODE/
    ROMNANDTAG_4, ROMNANDTAG_5, ROMNANDTAG_6, ROMNANDTAG_7,     // NANDBOOT/V01.00
    0,            0,            0,            0,
    0,            0,            0,            0,
    
#if defined(CC_NANDBOOT_SUPPORT_SMALL_BBM0)
    ROMNANDTAG_5, 4,            2,            0x200,
    0x10,         0,            4,            3,
    0x200,        0x10,         0x20,         0,
    0,            0,            0x195B2C7A,   0xEF81A6AF
#else
    ROMNANDTAG_7, 4,            2,            0x200,
    0x10,         0,            5,            3,
    0x800,        0x10,         0,            0,
    0,            0,            0x4D245937,   0x9B628D7C
#endif
};

//-----------------------------------------------------------------------------
// static functions
//-----------------------------------------------------------------------------
void CHIP_DisplayString(const CHAR *szString);
void CHIP_DisplayHex(UINT32);

//-----------------------------------------------------------------------------
/** CHIP_NandShowHex()
 */
//-----------------------------------------------------------------------------
VOID CHIP_NandShowHex(const CHAR *pu1Str, UINT32 u4Hex)
{
    CHIP_DisplayString(pu1Str);
    CHIP_DisplayHex(u4Hex);
    CHIP_DisplayString("\n");
}

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
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_RD_REMAIN;
    } while (u4Val < u4WaitNum);
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
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_STA);
        u4Val &= 0xFF000DFF;  // bit8=1: ready to busy
    } while (u4Val != 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_WaitBusy(void)
{
    UINT32 u4Val;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCIDLE);
    } while (u4Val == 0);
    
    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECIDLE);
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WaitSectorCnt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WaitSectorCnt(void)
{
    UINT32 u4Val, u4Cnt;
    
    do
    {
        u4Val = NAND_READ32(NAND_NFI_ADDRCNTR);
        u4Val &= 0xF000;
        u4Cnt = NAND_NFI_ADDRCNTR_SEC_CNTR(_u4SectorNum);
    } while (u4Val != u4Cnt);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Command(UINT32 u4Cmd)
{
    NAND_WRITE32(NAND_NFI_CMD, u4Cmd);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Addr(UINT32 u4RowAddr, UINT32 u4ColAddr, UINT32 u4ByteCount)
{
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
    
#if defined(CC_MT5365) || defined(CC_MT5395)
    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
    }
    else
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
    }

    if (_u4PageSize == 512)
    {
        u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
    }
    else if (_u4PageSize == 2048)
    {
        u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(16) | NAND_NFI_PAGEFMT_ECC_NUM(16);
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
        _u4FdmNum = 16;
    }
    else if (_u4PageSize == 4096)
    {
        if (_u4OOBSize == 16)
        {
            u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(16) | NAND_NFI_PAGEFMT_ECC_NUM(16);
            _u4FdmNum = 16;
        }
        else
        {
            u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);
        }
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
    }
    else if (_u4PageSize == 8192)
    {
        if (_u4OOBSize == 16)
        {
            u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(16) | NAND_NFI_PAGEFMT_ECC_NUM(16);
            _u4FdmNum = 16;
        }
        else
        {
            u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);
        }
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
    else
    {
        u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(16) | NAND_NFI_PAGEFMT_ECC_NUM(16);
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
        _u4FdmNum = 16;
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

    u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);
#endif

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
    UINT32 u4EncMsg, u4DecMsg;

#if defined(CC_MT5365) || defined(CC_MT5395)
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
            _u4ECCBits = 8;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_8 | NAND_NFIECC_ENCCNFG_ENC_MS_1040;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_8 | NAND_NFIECC_DECCNFG_DEC_CS_1040_8;
        }
    }
    else if (_u4PageSize == 512)
    {
        _u4ECCBits = 10;
        u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_10 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
        u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_10 | NAND_NFIECC_DECCNFG_DEC_CS_520_10;
    }
    else if (_u4PageSize == 2048)
    {
        _u4ECCBits = 20;
        u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_20 | NAND_NFIECC_ENCCNFG_ENC_MS_1040;
        u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_20 | NAND_NFIECC_DECCNFG_DEC_CS_1040_20;
    }
    else
    {
        _u4ECCBits = 24;
        u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_24 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
        u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_24 | NAND_NFIECC_DECCNFG_DEC_CS_1032_24;
    }
#else
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
    else
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
    else
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_NONE;
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
            if (u4Val == NAND_NFI_STA_NFI_FSM_READ_DATA)
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
        *pu4ErrNum = (u4Val >> (u4SectIdx * 8)) & 0x1F;
    }
    else
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM1);
        *pu4ErrNum = (u4Val >> ((u4SectIdx - 4) * 8)) & 0x1F;
    }
    
    if (*pu4ErrNum == 0x1F)
    {
        return NAND_ECC_ERROR;
    }
    else
    {
        return NAND_SUCC;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CorrectEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CorrectEcc(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4SectIdx,
                                                             UINT32 u4PgOff, UINT32 u4Len)
{
    UINT32 i, u4Val, u4SpareOff, u4SectOobSz;
    UINT32 u4ErrNum, u4ErrFound, u4ErrByte, u4ErrBit;
    
    _NANDHW_WaitEccDone(u4SectIdx); // must wait done

    if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
    {
        return NAND_ECC_ERROR;
    }
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    u4ErrFound = NAND_READ32(NAND_NFIECC_DECFER);
    if ((u4ErrFound >> u4SectIdx) & 0x01)
    {
        for (i = 0; i < u4ErrNum; i++)
        {
            u4Val = NAND_READ16(NAND_NFIECC_DECEL0 + (i * 2));
            u4ErrByte = u4Val >> 3;
            u4ErrBit = u4Val & 0x7;

            if (u4ErrByte < _u4SectorSize)
            {
                // Data area bit error.
                u4ErrByte += u4SectIdx*_u4SectorSize;
                if (pu1Buf && (u4ErrByte >= u4PgOff) && (u4ErrByte < (u4PgOff + u4Len)))
                {
                    pu1Buf[u4ErrByte - u4PgOff] = pu1Buf[u4ErrByte - u4PgOff] ^ (((UINT32)1) << u4ErrBit);
                }
            }
            else if (u4ErrByte < (u4SectOobSz + _u4SectorSize))
            {
                // Spare area bit error.
                if (pu1Spare)
                {
                    u4SpareOff = u4ErrByte - _u4SectorSize + u4SectOobSz * u4SectIdx;
                    pu1Spare[u4SpareOff] = pu1Spare[u4SpareOff] ^ (((UINT32)1) << u4ErrBit);
                }
            }
            else
            {
                return NAND_ECC_ERROR;
            }
        }
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CheckEcc(void)
{
    UINT32 u4SectIdx, u4ErrNum;
    INT32 i4Ret = NAND_SUCC;
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        _NANDHW_WaitEccDone(u4SectIdx);
        i4Ret |= _NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum);
    }

    return i4Ret;
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
    else
    {
        return NAND_FAIL;
    }
    
    if ((u4Addr >= u4PgOff) && (u4Addr < (u4PgOff + u4Len)))
    {
        u4Val = pu4Buf[(u4Addr - u4PgOff) >> 2];
        pu4Buf[(u4Addr-u4PgOff) >> 2] = pu4Spare[0];
        pu4Spare[0] = u4Val;
    }

    return NAND_SUCC;
}

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
    
    if (_fgNandInit == FALSE)
    {
#if defined(CC_MT5365) || defined(CC_MT5395)
        u4Val = BIM_READ32(NAND_NFI_PERMISC);
        u4Val |= NAND_NFI_PERMISC_NFI_SEL;
        BIM_WRITE32(NAND_NFI_PERMISC, u4Val);
#endif
    } 
    
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
static INT32 _NANDHW_NFI_ReadFifo(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;
    UINT32 u4SectIdx, j, u4Val, u4LoopCnt;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    UINT32 *pu4Spare = (UINT32 *)pu1Spare;
    
    ASSERT(pu1Buf != NULL);
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        u4LoopCnt = _u4SectorSize>>2;
        u4Val = u4SectIdx * u4LoopCnt;
        for (j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            if (((u4Val << 2) >= u4PgOff) && ((u4Val << 2) < (u4PgOff + u4Len)))
            {
                pu4Buf[u4Val - (u4PgOff >> 2)] = NAND_READ32(NAND_NFI_DATAR);
            }
            else
            {
                NAND_READ32(NAND_NFI_DATAR);
            }
            u4Val++;
        }
        
        u4LoopCnt = (_u4OOBSize * _u4SectorSize) >> 11;
        u4Val = u4SectIdx * u4LoopCnt;
        for (j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            if (pu4Spare)
            {
                pu4Spare[u4Val] = NAND_READ32(NAND_NFI_DATAR);
            }
            else
            {
                NAND_READ32(NAND_NFI_DATAR);
            }
            u4Val ++;
        }

        if (_rECCType == NAND_ECC_SOFT)
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, pu1Spare, u4SectIdx, u4PgOff, u4Len))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }
        
        _NANDHW_WaitBusy();
    }
    
    _NANDHW_NFIECC_WaitBusy();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadPage()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ReadPage(UINT8 *pu1Buf, UINT8 *pu1Spare, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret = NAND_SUCC;

    ASSERT(pu1Buf != NULL);
    
    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    _NANDHW_NFI_Cfg(_rECCType, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(_rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    /* Write command. */
    _NANDHW_Command(NAND_CMD_READ1_00);
    _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
    if (_u4PageSize != 0x200)
    {
        _NANDHW_Command(NAND_CMD_READ_30);
    }

    _NANDHW_WaitBusy();
    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
        CHIP_NandShowHex("Read fail 1: page=", u4PgIdx);
        goto HandleExit;
    }
    
    /*Trigger Read . */
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
    _NANDHW_WaitBusy();

    i4Ret = _NANDHW_NFI_ReadFifo(pu1Buf, pu1Spare, u4PgOff, u4Len);
    if (i4Ret)
    {
        CHIP_NandShowHex("Read fail 2: page=", u4PgIdx);
        goto HandleExit;
    }

    _NANDHW_NFI_WaitSectorCnt();

    /* Check ECC */
    if (_rECCType != NAND_ECC_NONE)
    {
        i4Ret = _NANDHW_NFIECC_CheckEcc();
        if (i4Ret)
        {
            CHIP_NandShowHex("Read fail 3: page=", u4PgIdx);
            goto HandleExit;
        }
    }
    
HandleExit:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
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
    CHIP_NandShowHex("NID=", u4ID);
    
    u4DevCount = sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);
    for (i = 0; i < u4DevCount; i++)
    {
        prDev = (NAND_FLASH_DEV_T *)&_arNand_DevInfo[i];
        if ((u4ID & prDev->u4IDMask) == (prDev->u4IDVal & prDev->u4IDMask))
        {
            _prFlashDev = prDev;
            return NAND_SUCC;
        }
    }

    _prFlashDev = NULL;
    return NAND_FAIL;
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
    
    UNUSED(_fgIsrEn);
    UNUSED(_u4FdmNum);
    UNUSED(_u4ECCBits);
    
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

    NAND_WRITE32(NAND_NFI_ACCCON1, 0x00411888);
    NAND_WRITE32(NAND_NFI_ACCCON2, 0x00080008);

    for (i = 0; i < 3; i++)
    {
        // Reset NFI Controller
        _NANDHW_NFI_Reset();
        NANDHW_Reset();

        if (_NANDHW_ProbeDev() == NAND_SUCC)
        {
            LOG(1, "Detect %s NAND flash: %dMB\n", _prFlashDev->acName, (_prFlashDev->u4ChipSize / 0x100000));
            _u4PageSize = _prFlashDev->u4PageSize;
            _u4OOBSize =  _prFlashDev->u4OOBSize;
            
            _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
            _NANDHW_NFIECC_Cfg(NAND_ECC_NONE);
            
            if (_prFlashDev->u4PageSize == 512)
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 1);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
                _u4SectorSize = 512;
            }
            else
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 2);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
                _u4SectorSize = 1024;
            }
            _u4BlkPgCount = _prFlashDev->u4BlkPgCount;
            
            break;
        }
        
        LOG(0, "Not detect any flash\n");
        _u4PgAddrLen = 2;
        _u4TotalAddrLen = 5;
        _u4PageSize = 2048;
        _u4OOBSize = 16;
        _u4SectorSize = 1024;
    }
    
    _NANDHW_NFI_SetPageFmt();
    _u4SectorNum = _u4PageSize / _u4SectorSize;

    _fgNandInit = TRUE;
    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadSpare()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_ReadSpare(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i, u4Val = 0;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    INT32 i4Ret = NAND_SUCC;

    ASSERT(pu1Buf != NULL);

    for (i = 0; i < u4Len; i += 4)
    {
        _NANDHW_WaitRead(4);
        pu4Buf[u4Val] = NAND_READ32(NAND_NFI_DATAR);
        u4Val++;
    }
    
    if (_rECCType == NAND_ECC_SOFT)
    {
        if (u4Len == (_u4SectorSize + ((_u4OOBSize *_u4SectorSize)>>9)))
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, &pu1Buf[_u4SectorSize], 0, 0, u4Len))
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
        _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    else
    {
        _NANDHW_Command(NAND_CMD_READ1_00);
        _NANDHW_Addr(u4PgIdx, u4PgOff, (_u4PgAddrLen << 4)|(_u4TotalAddrLen - _u4PgAddrLen));
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
    UINT32 u4BitCnt, u4Offset, u4OobLastIdx, i;
    UINT32 u4EccSectorOobSize, u4EccPageOobSize, u4EccSectorSize;
    
    _rECCType = NAND_ECC_NONE;
    
    u4EccSectorOobSize =  (_u4OOBSize * _u4SectorSize)>>9;
    u4EccPageOobSize = (_u4OOBSize *_u4PageSize)>>9;
    u4EccSectorSize = _u4SectorSize + u4EccSectorOobSize;

    // read spare data without ecc
    u4Offset = _u4PageSize;
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT) == 1)
    {
        iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, _u4OOBSize);
    }
    else
    {
        iRet = _NANDHW_ReadSpare(pu1Spare, u4PgIdx, u4Offset, u4EccPageOobSize);
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
        u4OobLastIdx = u4EccPageOobSize - u4EccSectorOobSize;
        for (i = 0; i < 6; i++)
        {
           u4BitCnt += _NANDHW_countbits(pu1Spare[u4OobLastIdx + i]);
        }

        if(u4BitCnt > 62)
        {
            goto HandleGood;
        }
        
        if (u4BitCnt < 2)
        {
            goto HandleBad;
        }

        break;
    }
    
    _rECCType = NAND_ECC_SOFT;

    // read last sector data with ecc
    u4Offset = (_u4SectorNum - 1) * u4EccSectorSize;
    iRet = _NANDHW_ReadSpare(pu1Data, u4PgIdx, u4Offset, u4EccSectorSize);
    if (iRet)
    {
        goto HandleBad;
    }

    u4OobLastIdx = u4EccSectorSize - (u4EccSectorOobSize*_u4SectorNum);
    if ((pu1Data[u4OobLastIdx] != 0xFF) || (pu1Data[u4OobLastIdx + 1] != 0xFF))
    {
        goto HandleBad;
    }
      
HandleGood:
    _rECCType = NAND_ECC_SOFT;
    return FALSE;       // Good block
HandleBad:
    _rECCType = NAND_ECC_SOFT;
    return TRUE;        // bad block
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
/** NANDHW_ReadPageAttr()
 */
//-----------------------------------------------------------------------------
INT32 NANDHW_ReadPageAttr(UINT32 *pu4Buf, UINT32 *u4Spare, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len)
{
    INT32 i4Ret;
    
    i4Ret = _NANDHW_ReadPage((UINT8 *)pu4Buf, (UINT8 *)u4Spare, u4PgIdx, u4PgOff, u4Len);
    _NANDHW_DataExchange(pu4Buf, u4Spare, u4PgOff, u4Len);
    return i4Ret;
}

// Check for pre bad block of loader.
UINT32 NANDHW_CheckPreBadBlk(UINT32 u4ByteOffset)
{    
    UINT32 i, u4PgIdx, u4GoodBlk, u4Count, u4BlkSize;

    u4PgIdx = u4ByteOffset/_u4PageSize;
    u4BlkSize = _u4PageSize*_u4BlkPgCount;
    
    if ((u4PgIdx & (_u4BlkPgCount - 1)) == 0)
    {
        u4GoodBlk = u4ByteOffset/u4BlkSize;
    }
    else
    {
        u4GoodBlk = u4ByteOffset/u4BlkSize + 1;
    }

    for (i = 0, u4Count = 0; u4Count < u4GoodBlk; i++)
    {
        // The first block must be good.
        if ((i != 0) && NANDHW_IsBadBlk(i))
        {
            CHIP_NandShowHex("Pre  Bad Block:", i);
            
            u4ByteOffset += u4BlkSize;
            continue;
        }

        u4Count++;
    }

    return u4ByteOffset;
}

//-----------------------------------------------------------------------------
/** CHIP_NANDCopyToDRAM
 */
//-----------------------------------------------------------------------------
INT32 CHIP_NANDCopyToDRAM(UINT32 *pu4DstBuf, UINT32 u4ByteOffset, UINT32 u4Len)
{
    INT32  i4Ret = 0;
    UINT32 u4Spare[28*4];
    UINT32 i, u4PgIdx, u4PageOff, u4ReadByte;

    u4ByteOffset = NANDHW_CheckPreBadBlk(u4ByteOffset);
        
    u4PgIdx = u4ByteOffset/_u4PageSize;
    u4PageOff = u4ByteOffset&(_u4PageSize - 1);
    
    if (u4PageOff)
    {
        u4ReadByte = _u4PageSize - u4PageOff;
    }
    else
    {
        u4ReadByte = _u4PageSize;
    }
    
    for (i = 0; i < u4Len; )
    {
        // Check for post bad block of loader.
        if ((u4PgIdx & (_u4BlkPgCount - 1)) == 0)
        {
            if (NANDHW_IsBadBlk(u4PgIdx/_u4BlkPgCount))
            {
                CHIP_NandShowHex("Post Bad Block:", u4PgIdx/_u4BlkPgCount);
                
                u4PgIdx += _u4BlkPgCount;
                continue;
            }
        }
        
        if ((u4ReadByte + i) > u4Len)
        {
            u4ReadByte = u4Len - i;
        }
        
        i4Ret |= NANDHW_ReadPageAttr((UINT32 *)((UINT32)pu4DstBuf+i), u4Spare, u4PgIdx, u4PageOff, u4ReadByte);

        i += u4ReadByte;
        u4PageOff = 0;
        u4ReadByte = _u4PageSize;
        u4PgIdx++;
    }
    
    return i4Ret;
}

// This function is just protect _u4Signature/ROMCODETAG not optimized by compiler.
UINT32 NandDummy(UINT32 i)
{
    return _u4Signature[i];
}

void CHIP_NANDCopyToDRAMLZHS(UINT32 *pu4DstBuf, UINT32 u4ByteOffset, UINT32 u4Len)
{
    NandDummy(0);
    
    NANDHW_EnablePinmux(TRUE);
    NANDHW_Init();
    
    CHIP_NANDCopyToDRAM((UINT32 *)pu4DstBuf, u4ByteOffset, u4Len);
    
    NANDHW_EnablePinmux(FALSE);
}

#endif

