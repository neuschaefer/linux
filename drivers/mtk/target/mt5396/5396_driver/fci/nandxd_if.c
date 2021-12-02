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

/** @file .c
 *  This C file implements the SM/XD basic hardware protocol functions.  All calls
 *   are from nandxd_if.c.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "nandxd_if.h"
#include "nandhw_if.h"

LINT_EXT_HEADER_BEGIN

#include "fcihw_reg.h"
#include "fcihw_io.h"
#include "fcihw_func.h"
#include "fci_if.h"
#include "x_assert.h"

LINT_EXT_HEADER_END


/*
    lint inhibition(s)
        Info 826: Suspicious pointer-to-pointer conversion (area too small)
*/

/*lint -save -e826*/

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define NANDXD_CARD_SIZE_64MB     (64)
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------
#define XD_CELL_MASK    ((UINT8)0x0C)
// 1 bit/cell
#define XD_1BIT_CELL      ((UINT8)0x00)
#define XD_2BIT_CELL      ((UINT8)0x04)
#define XD_4BIT_CELL      ((UINT8)0x08)
#define XD_8BIT_CELL      ((UINT8)0x0C)

#define NANDXD_CMD_ID_READ_90             ((UINT32)0x90)
#define NANDXD_CMD_ID_READ_91             ((UINT32)0x91)
#define NANDXD_CMD_ID_READ_9A             ((UINT32)0x9A)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
/// device code and card size mapping table.
static const NANDXDDEVCODEMAP_T _arSmDevCodeMap [] =
{
    { 0x6E, 1 },
    { 0xE8, 1 },
    { 0xEC, 1 },
    { 0xEA, 2 },
    { 0x64, 2 },
    { 0x5D, 2 },
    { 0xE3, 4 },
    { 0x6B, 4 },
    { 0xE5, 4 },
    { 0xD5, 4 },
    { 0xE6, 8 },
    { 0xD6, 8 },
    { 0x73, 16 },
    { 0x57, 16 },
    { 0x75, 32 },
    { 0x58, 32 },
    { 0x76, 64 },
    { 0xD9, 64 },
    { 0x79, 128 },
    { 0xDA, 128 },
    { 0x71, 256 },
    { 0x5B, 256 }
};

static const NANDXDDEVCODEMAP_T _arXdDevCodeMap [] =
{
    { 0x73, 16 },
    { 0x75, 32 },
    { 0x76, 64 },
    { 0x79, 128 },
    { 0x71, 256 },
    { 0xDC, 512 },
    { 0xD3, 1024 },
    { 0xD5, 2048 },
    { 0xD6, 4096 },
    { 0xD7, 8192 }
};

/// card size, page size, pages/block, total blocks/card.
static const NANDXDMEMCFG_T _arSmXdMemCfg [] =
{
    /// use for SM/XD.
    { 1, 256, 16, 256 },     ///< SM_SIZE_1_MB,
    { 2, 256, 16, 512 },     ///< SM_SIZE_2_MB,
    { 4, 512, 16, 512 },     ///< SM_SIZE_4_MB,
    { 8, 512, 16, 1024 },    ///< SM_SIZE_8_MB,
    { 16, 512, 32, 1024 },   ///< SM_SIZE_16_MB,
    { 32, 512, 32, 2048 },   ///< SM_SIZE_32_MB,
    { 64, 512, 32, 4096 },   ///< SM_SIZE_64_MB,
    { 128, 512, 32, 8192 },  ///< SM_SIZE_128_MB,
    { 256, 512, 32, 16384 }, ///< SM_SIZE_256_MB
    /// use only for XD.
    { 512, 512, 32, 32768 },   ///< SM_SIZE_512_MB,
    { 1024, 512, 32, 65536 },  ///< SM_SIZE_1024_MB,
    { 2048, 512, 32, 131072 }, ///< SM_SIZE_2048_MB,
/// XD 4G, 8G TBD.
};

/// point to SM/xD data structure.
static SMXD_T *_prFCINANDXD;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** _NandXd_SetCardInfo
 *  Look up table and initialize _prFCINANDXD.
 *  @param  VOID.
 *  @retval S_OK	Success
 *  @retval Others	Fail
 */
//-------------------------------------------------------------------------
static INT32 _NandXd_SetCardInfo(UINT32 u4DevCode)
{
    UINT32 i;

    BOOL fgFound = FALSE;

    LOG(7, "%s Begin.\n", __FUNCTION__);

    // Search code table.
    if (_prFCINANDXD->u2CardType == FCI_SM_CARD)
    {
        for (i = 0; i < (sizeof(_arSmDevCodeMap) /sizeof(_arSmDevCodeMap[0])); i++)
        {
            if (_arSmDevCodeMap[i].u2DevCode == (UINT32)u4DevCode)
            {
                _prFCINANDXD->u2Size = _arSmDevCodeMap[i].u2Size;

                LOG(7, "%s u2Size: %d MB\n", __FUNCTION__, _prFCINANDXD->u2Size);
                fgFound = TRUE;
                break;
            }
        }
    }
    else if (_prFCINANDXD->u2CardType == FCI_XD_CARD)
    {
        for (i = 0; i < (sizeof(_arXdDevCodeMap) /sizeof(_arXdDevCodeMap[0])); i++)
        {
            if (_arXdDevCodeMap[i].u2DevCode == (UINT32)u4DevCode)
            {
                _prFCINANDXD->u2Size = _arXdDevCodeMap[i].u2Size;

                LOG(7, "%s u2Size: %d MB\n", __FUNCTION__, _prFCINANDXD->u2Size);
                fgFound = TRUE;
                break;
            }
        }
    }

    if (!fgFound)
    {
        LOG(1, "%s(%d) %s error.\n", __FILE__, __LINE__, __FUNCTION__);
        return E_FAIL;
    }

    // Search size table.
    for (i = 0; i < (sizeof(_arSmXdMemCfg) /sizeof(_arSmXdMemCfg[0])); i++)
    {
        if (_prFCINANDXD->u2Size == _arSmXdMemCfg[i].u2Size)
        {
            _prFCINANDXD->u2PageSz = _arSmXdMemCfg[i].u2PageSz;

            _prFCINANDXD->u2BlkSz = _arSmXdMemCfg[i].u2BlkSz;
            _prFCINANDXD->u4PhyBlkNs = _arSmXdMemCfg[i].u4BlkNs;
            _prFCINANDXD->u2TotalZone = _prFCINANDXD->u4PhyBlkNs / NANDXD_PHY_BLK_PER_ZONE;
            _prFCINANDXD->u4LogBlkNs = _prFCINANDXD->u2TotalZone * NANDXD_LOG_BLK_PER_ZONE;

            LOG(7, "%s u2Size = %d MB.\n", __FUNCTION__, _prFCINANDXD->u2Size);
            LOG(7, "%s u2PageSz = %d bytes.\n", __FUNCTION__, _prFCINANDXD->u2PageSz);
            LOG(7, "%s u2BlkSz = %d pages.\n", __FUNCTION__, _prFCINANDXD->u2BlkSz);
            LOG(7, "%s u2TotalZone = %d zones.\n", __FUNCTION__, _prFCINANDXD->u2TotalZone);
            LOG(7, "%s u4PhyBlkNs = %d blocks.\n", __FUNCTION__, _prFCINANDXD->u4PhyBlkNs);
            LOG(7, "%s u4LogBlkNs = %d blocks.\n", __FUNCTION__, _prFCINANDXD->u4LogBlkNs);

            return S_OK;
        }
    }

    return E_FAIL;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** NANDXDHW_EraseBlockfs
 * SM/xD erase block function.
 *  block erase command = 0x60, 0xD0.
 *  @param  u4Blk         indicated erase block number.
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------

INT32 NANDXDHW_EraseBlock(UINT32 u4Blk)
{
    INT32 i4Ret;

    UINT32 u4Sz = 0;

    if (!_prFCINANDXD->fgDetect)
    {
        LOG(1, "%s Error.\n", __FUNCTION__, u4Blk);
        return E_FAIL;
    }

    LOG(7, "%s Begin: block = %d.\n", __FUNCTION__, u4Blk);

    if (_prFCINANDXD->u2BlkSz == NANDXD_PAGE_PER_BLK)
    {
        u4Sz = u4Blk << 5;
    }
    else
    {
        u4Sz = u4Blk << 4;
    }

    i4Ret = NANDHW_EraseBlock(u4Sz);

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
        return i4Ret;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** NANDXDHW_SetStartPtr
 * SM/xD set data start pointer function.
 *  Caution: use Dummy read to set start pointer by comand 0x00.
 *  @param  VOID
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------f
INT32 NANDXDHW_SetStartPtr(VOID)
{
#if 0
    INT32 i4Ret;

    UINT32 u4Sz = 0;
    UINT32 u4Cmd = 0;
    UINT8 u1Buf[4]; // dummy read 4 bytes of data.

    if (!_prFCINANDXD->fgDetect)
    {
        LOG(1, "%s Error.\n", __FUNCTION__);
        return E_FAIL;
    }

    LOG(7, "%s.\n", __FUNCTION__);

    u4Sz = 0;
    // set SM address in SDAG.
    FCI_WRITE32(RW_SD_ARG_REG, u4Sz);

    // DMA is DW aligned.
    u4Sz = 4;

    // write one page and set DMA .
    FCIHW_DmaWriteDramEnable(u1Buf, u4Sz);

    // read one page, address cycle by card size.
    if (_prFCINANDXD->u2Size >= NANDXD_CARD_SIZE_64MB)
    {
        u4Cmd = SM_ADDR_CYCLE(4);
    }
    else
    {
        u4Cmd = SM_ADDR_CYCLE(3);
    }

    u4Cmd |= SM_CMD_READ | SM_CMD_READ_O | SM_DE | u4Sz;

    i4Ret = SMXDHW_SendCmd(u4Cmd);

    i4Ret = NANDHW_ReadPage((UINT32 *)pbBuf, u4Sz);

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
        return i4Ret;
    }

    // disable DMA.
    FCIHW_DmaDisable();

    LOG(7, "%s End.\n", __FUNCTION__);

#endif
    return S_OK;
}

//-------------------------------------------------------------------------
/** NANDXDHW_ReadPage
 * SM/xD read page function.
 *  read (1) = 0x00h.
 *  @param  u4Blk        block number.
 *  @param  u2Page	    page number.
 *  @param  pbBuf	    user's data buffer pointer.
 *  @param  u4PageSize	    page size = 512 or 528 bytes.
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------
INT32 NANDXDHW_ReadPage(UINT32 u4Blk, UINT16 u2Page, UINT8 *pbBuf, UINT32 u4PageSize)
{
    INT32 i4Ret;

    UINT32 u4Sz = 0;

    if (!_prFCINANDXD->fgDetect)
    {
        LOG(1, "%s Error.\n", __FUNCTION__);
        return E_FAIL;
    }

    LOG(7, "%s Begin: u4Blk =%d, u2Page=%d, pbBuf = 0x%08X%.\n", __FUNCTION__, u4Blk, u2Page,
        (UINT32)pbBuf);

    // check error.
    if ((_prFCINANDXD->u4PhyBlkNs < u4Blk) ||
        (_prFCINANDXD->u2BlkSz < u2Page) ||
        ((_prFCINANDXD->u2PageSz + NANDXD_RDNT_SIZE) < u4PageSize) ||
        (_prFCINANDXD->u2PageSz > u4PageSize))
    {
        // check parameter error.

#ifndef NDEBUG

        ASSERT(0);
#else

        return E_FAIL;
#endif

    }

    if (_prFCINANDXD->u2BlkSz == NANDXD_PAGE_PER_BLK)
    {
        u4Sz = (u4Blk << 5) | u2Page;
    }
    else
    {
        u4Sz = (u4Blk << 4) | u2Page;
    }

    i4Ret = NANDHW_ReadPage((UINT32 *)pbBuf, u4Sz);

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** NANDXDHW_ReadRdnt
 * SM/xD read redundancy function.
 *  read (3) = 0x50h.
 *  @param  u4Blk        block number.
 *  @param  u2Page	    page number.
 *  @param  pbBuf	    user's data buffer pointer.
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------
INT32 NANDXDHW_ReadRdnt(UINT32 u4Blk, UINT16 u2Page, UINT8 *pbBuf)
{
    INT32 i4Ret;

    UINT32 u4Sz = 0;

    if (!_prFCINANDXD->fgDetect)
    {
        LOG(1, "%s Error.\n", __FUNCTION__, u4Blk);
        return E_FAIL;
    }

    LOG(7, "%s Begin: u4Blk =%d, pbBuf = 0x%08X%.\n", __FUNCTION__, u4Blk, (UINT32)pbBuf);

    // check error.
    if (_prFCINANDXD->u4PhyBlkNs < u4Blk)
    {

#ifndef NDEBUG

        ASSERT(0);
#else

        return E_FAIL;
#endif

    }

    if (_prFCINANDXD->u2BlkSz == NANDXD_PAGE_PER_BLK)
    {
        u4Sz = (u4Blk << 5) | u2Page;
    }
    else
    {
        u4Sz = (u4Blk << 4) | u2Page;
    }

    // Jethro i4Ret = NANDHW_ReadAttr((UINT32 *)pbBuf, u4Sz);
    i4Ret = NANDHW_ReadAttr((UINT32 *)pbBuf, u4Sz, 0);

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** NANDXDHW_WritePage
 * SM/xD write page function.
 *  serial data input (1) = 0x80.
 *  true page program = 0x10.
 *  @param  u4Blk        block number.
 *  @param  u2Page	    page number.
 *  @param  pbBuf	    user's data buffer pointer.
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------
INT32 NANDXDHW_WritePage(UINT32 u4Blk, UINT16 u2Page, UINT8 *pbBuf)
{
    INT32 i4Ret;

    UINT32 u4Sz = 0;

    if (!_prFCINANDXD->fgDetect)
    {
        LOG(1, "%s Error.\n", __FUNCTION__, u4Blk);
        return E_FAIL;
    }

    LOG(7, "%s Begin: u4Blk =%d, u2Page=%d, pbBuf = 0x%08X%.\n", __FUNCTION__, u4Blk, u2Page,
        (UINT32)pbBuf);


    if (_prFCINANDXD->u2BlkSz == NANDXD_PAGE_PER_BLK)
    {
        u4Sz = (u4Blk << 5) | u2Page;
    }
    else
    {
        u4Sz = (u4Blk << 4) | u2Page;
    }

    i4Ret = NANDHW_WritePage((UINT32 *)pbBuf, u4Sz);

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** NANDXDHW_CheckCard
 * SM/xD check and initialize card function.
 *  @param  VOID.
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------
INT32 NANDXDHW_CheckCard(VOID)
{
    UINT32 u4Val = 0;
    INT32 i4Ret = 0;
    UINT32 u4DevCode;

    LOG(1, "%s Begin.\n", __FUNCTION__);

    _prFCINANDXD = FCI_GetSMXDSlot();

    ASSERT(_prFCINANDXD != NULL);

    // initialize nandxd data structure.
    VERIFY(NULL != x_memset(_prFCINANDXD, 0, sizeof(SMXD_T)));

    // check if this card is SM/xD card.
    i4Ret = NANDHW_ReadXdId(NANDXD_CMD_ID_READ_9A, &u4Val);
    if ((S_OK != i4Ret) ||(0 == u4Val))
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
        return E_FAIL;
    }

    // We go response from SM/xD slot.
    // start to initailze SM/xD data structure.
    _prFCINANDXD->fgDetect = TRUE;
    _prFCINANDXD->u2NowZone = NANDXD_BLANK_BYTE;

    if ((BYTE3(u4Val)) == XD_ID_CODE)
    {
        _prFCINANDXD->u2CardType = FCI_XD_CARD;
    }
    else
    {
        _prFCINANDXD->u2CardType = FCI_SM_CARD;
    }

    // get device code.
    i4Ret = NANDHW_ReadXdId(NANDXD_CMD_ID_READ_90, &u4Val);
    if ((S_OK != i4Ret) ||(0 == u4Val))
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
        return E_FAIL;
    }

    u4DevCode = BYTE2(u4Val);

    LOG(1, "%s bDevCode: 0x%X\n", __FUNCTION__, u4DevCode);
    LOG(1, "%s u191cmd: 0x%X\n", __FUNCTION__, BYTE4(u4Val));

    // check if support 91 cmd.
    if (BYTE4(u4Val) == XD_ID_WITH_91_CODE)
    {
        i4Ret = NANDHW_ReadXdId(NANDXD_CMD_ID_READ_91, &u4Val);

        // send 91 command and check if support X4 Multi-Block write.
        if ((S_OK != i4Ret) ||(0 == u4Val))
        {
            LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
            return E_FAIL;
        }

        LOG(1, "%s ID_READ_91: 0x%X\n", __FUNCTION__, u4Val);
        // check multiple block support.
        if (XD_ID_X4MULBLK_CODE == ((BYTE1(u4Val)) & 0x30))
        {
            LOG(1, "%s X4 Multi-Block is Supported.\n", __FUNCTION__);
            _prFCINANDXD->fgX4MulBlk = TRUE;
        }
        else
        {
            LOG(1, "%s X4 Multi-Block is not Supported.\n", __FUNCTION__);
        }

        // check cell type.
        if (XD_1BIT_CELL == ((BYTE1(u4Val)) & XD_CELL_MASK))
        {
            LOG(1, "%s 1BIT_CELL.\n", __FUNCTION__);
        }
        else if (XD_2BIT_CELL == ((BYTE1(u4Val)) & XD_CELL_MASK))
        {
            LOG(1, "%s 2BIT_CELL.\n", __FUNCTION__);
        }
        else if (XD_4BIT_CELL == ((BYTE1(u4Val)) & XD_CELL_MASK))
        {
            LOG(1, "%s 4BIT_CELL.\n", __FUNCTION__);
        }
        else if (XD_8BIT_CELL == ((BYTE1(u4Val)) & XD_CELL_MASK))
        {
            LOG(1, "%s 8BIT_CELL.\n", __FUNCTION__);
        }

    }

    // initialize SM/XD card data structure.
    if (S_OK != _NandXd_SetCardInfo(u4DevCode))
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
        return E_FAIL;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** NANDXDHW_Reset
 * SM/xD card reset function.
 *  reset = 0xFF..
 *  @retval S_OK        Success.
 *  @retval E_FAIL       Fail.
 */
//-------------------------------------------------------------------------
INT32 NANDXDHW_Reset(VOID)
{
    INT32 i4Ret = S_OK;
#if 0
    LOG(7, "%s Begin.\n", __FUNCTION__);

    i4Ret = NANDHW_Reset();

    if (S_OK != i4Ret)
    {
        LOG(1, "%s(%d) %s return error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    LOG(7, "%s End.\n", __FUNCTION__);
#endif
    return i4Ret;
}

#endif
