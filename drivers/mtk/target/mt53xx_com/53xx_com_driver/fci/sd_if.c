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
 * $RCSfile: sd_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sd_if.c
 *  This C file implements the SD interface functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "sdc_func.h"

LINT_EXT_HEADER_BEGIN

#include "fcihw_func.h"

#include "fci_type.h"
#include "fci_debug.h"
#include "fci_if.h"
#include "sdc_const.h"

#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/// FCI support multi-block transfer, max size is 64 KB.

#define SD_FCI_MAX_DATA_SIZE      0x10000

/// FCI support multi-block transfer, max block number is 128.
#define SD_FCI_MAX_BLOCK_NUM      128

#define SD_FCI_BLOCK_SIZE      (512)

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#undef  SECTOR_2N
#define SECTOR_2N               (9)

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

static INT32 _i4SDCStatus = (INT32)STORG_DEV_ABSENT;

static UINT32 _u4SDCallbackTag;
static BLOCK_CALLBACK _pfnSDCallback;

static BlockFunc_T _rSDBlkFuncTbl =
{
    NULL, NULL, NULL, SD_GetSize, SD_GetBlockSize, SD_GetStatus,
    SD_GetModel, NULL, SD_Read, SD_Write, SD_SetHotswapCallback
};

UINT32 _u4SdDetectCardType = FCI_UNKNOWN_CARD;
INT32 _fgSdCardRemove = TRUE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** SD_SingleBlockAccessEnable
*  SD read method selection : single block access / multiple block access
*  @param  fgEnable       TRUE : enable single block access, FALSE : enable multiple block access
*/
//-------------------------------------------------------------------------
VOID SD_SingleBlockAccessEnable(BOOL fgEnable)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    prSDC->fgSingleBlockAccess = fgEnable;
}

//-------------------------------------------------------------------------
/** SD_Read
*  SD card read function.
*  @param  u8Offset     read offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's read buffer address.
*  @param  u4MemLen      read length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SD_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = S_OK;

    UINT32 u4Lba, u4Offset, u4SecLen;

    u4Offset = (UINT32)u8Offset;
    u4Lba = u4Offset >> SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(9, "SD_Read Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x\n", u4Lba, u4SecLen, u4MemPtr);
    UNUSED(u4Lba);
    FCIHW_ResetSlot(SDC_SLOT);

    while (u4MemLen > SD_FCI_MAX_DATA_SIZE)
    {
        i4Ret = SDC_ReadBlks(u4Offset, (UINT8 *)u4MemPtr, SD_FCI_MAX_BLOCK_NUM);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }

        u4Offset += SD_FCI_MAX_DATA_SIZE;
        u4MemPtr += SD_FCI_MAX_DATA_SIZE;
        u4MemLen -= SD_FCI_MAX_DATA_SIZE;
    }

    if (u4MemLen)
    {
        u4SecLen = (u4MemLen >> SECTOR_2N);
        i4Ret = SDC_ReadBlks(u4Offset, (UINT8 *)u4MemPtr, u4SecLen);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SD_Write
*  SD card write function.
*  @param  u8Offset     write offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's write buffer address.
*  @param  u4MemLen      write length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SD_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret = S_OK;

    UINT32 u4Lba, u4Offset, u4SecLen;

    u4Offset = (UINT32)u8Offset;
    u4Lba = u4Offset >> SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(9, "SD_Write Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x\n", u4Lba, u4SecLen, u4MemPtr);
    UNUSED(u4Lba);
    FCIHW_ResetSlot(SDC_SLOT);

    while (u4MemLen > SD_FCI_MAX_DATA_SIZE)
    {
        i4Ret = SDC_WriteBlks(u4Offset, (UINT8 *)u4MemPtr, SD_FCI_MAX_BLOCK_NUM);

        if (i4Ret != S_OK)
        {
            return i4Ret;
        }

        u4Offset += SD_FCI_MAX_DATA_SIZE;
        u4MemPtr += SD_FCI_MAX_DATA_SIZE;
        u4MemLen -= SD_FCI_MAX_DATA_SIZE;
    }

    if (u4MemLen)
    {
        u4SecLen = (u4MemLen >> SECTOR_2N);
        i4Ret = SDC_WriteBlks(u4Offset, (UINT8 *)u4MemPtr, u4SecLen);
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SD_DetectCardRemove
 *  Detect SD slot card exist or not (SD-HC, SD, MMC card detection)
 *  @param  VOID
 *  @retval   S_OK : Card exist, E_FAIL : Not Exist.
 */
//-------------------------------------------------------------------------

INT32 SD_DetectCardRemove()
{
    INT32 i4Ret;
    INT32 i4FindCard = FALSE;
    static INT32 fgPreSdCardRemove = TRUE;
    SDC_T *prSDC;

    // Turn Off the power then Turn on
    FCIHW_PowerSwitch(FALSE);
    x_thread_delay(20);
    FCIHW_PowerSwitch(TRUE);
    x_thread_delay(20);    
    
#ifndef CC_NO_CARD_DETECT_PIN
    // handle card detection.
    if (FCIHW_DetectCard())   // TRUE : Card remove, FALSE : Card insert
    {
        LOG(1, "(%d) %s No Card error.\n", __LINE__, __FUNCTION__);        
        return TRUE;    // Card not exist
    }
#endif  // #ifndef CC_NO_CARD_DETECT_PIN

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    prSDC = FCI_GetSDCSlot();
    FCIHW_ResetSlot(SDC_SLOT);

    // Card is not attached => detect if card insert
    if(SD_GetCard() == FCI_UNKNOWN_CARD)
    {
        // Initialize Hardware
        SDCHW_Init();

        /* 5 Reset to avoid initial clock. */
        i4Ret = _SDC_Reset();
        i4Ret = _SDC_Reset();
        i4Ret = _SDC_Reset();
        i4Ret = _SDC_Reset();
        i4Ret = _SDC_Reset();

        // Check if SD-HC card  insert => use CMD 8
        if (_SDC_SendIfCondCmd() == S_OK)
        {
            _u4SdDetectCardType = FCI_SDHC_CARD;
            _fgSdCardRemove = FALSE;    // Card Exist ^^
            i4FindCard = TRUE;

            if(fgPreSdCardRemove != _fgSdCardRemove)
            {
                LOG(1,"SD-HC Card Exist ^^\n");
            }
        }
        // Check if SD Card  insert    => use CMD 55 (fist CMD of ACMD41)
        else
        {
            // Reset SD Card
            i4Ret = _SDC_Reset();

            // For Standard SD card, CMD 8 is illegal CMD. It let SdCmd55 fail => ILLEGAL_CMD
            // Although, reception of valid command will clear ILLEGAL_CMD bit, but it
            // needs a delay of one command. So we add one Dummy Send RCA Cmd here.
            i4Ret = _SDC_SendCmd55(SDC_RCA_DEFAULT );
            i4Ret = _SDC_SendCmd55(SDC_RCA_DEFAULT);  // Second Cmd55

            // If CMD55 success => Card exist
            if (i4Ret == S_OK)
            {
                _u4SdDetectCardType = FCI_SD_CARD;
                _fgSdCardRemove = FALSE;    // Card Exist ^^
                i4FindCard = TRUE;

                if(fgPreSdCardRemove != _fgSdCardRemove)
                {
                    LOG(1,"SD Card Exist ^^\n");
                }
            }
        }

        // Check if MMC Card  insert    => use CMD 1
        if(_u4SdDetectCardType != FCI_SDHC_CARD &&    // Not SDHC or SD card
           _u4SdDetectCardType != FCI_SD_CARD)           // Case : detect MMC last time, but can't initial mmc card
        {
            // Reset MMC Card
            i4Ret = _SDC_Reset();

            i4Ret = SDCHW_SendCmd(SD_CMD_1, SDC_OCR_SD_DEFAULT);

            // If Cmd1 success => Card exist
            if (i4Ret == S_OK)
            {
                _u4SdDetectCardType = FCI_MMC_CARD;
                _fgSdCardRemove = FALSE;    // Card Exist ^^
                i4FindCard = TRUE;

                if(fgPreSdCardRemove != _fgSdCardRemove)
                {
                    LOG(1,"MMC Card Exist ^^\n");
                }
            }
        }

        // Can't detect SDHC, SD, MMC card => Card is removed
        // This case happens when card insert but can't be initialized ok.
        if(i4FindCard == FALSE)
        {
            _u4SdDetectCardType = FCI_UNKNOWN_CARD;
            _fgSdCardRemove = TRUE;    // Card Not Exist
        }
    }
    // Card is attached => detect if card removed
    else
    {
        switch(_u4SdDetectCardType)
        {
            case FCI_SDHC_CARD:
            case FCI_SD_CARD:
            case FCI_MMC_CARD:
                {
                    UINT32 u4Tmp;

                    i4Ret = SDC_GetStatus(prSDC->u2RCA, &u4Tmp);

                    if (i4Ret != S_OK)
                    {
                        _fgSdCardRemove = TRUE;

                        if(fgPreSdCardRemove != _fgSdCardRemove)
                        {
                            switch(_u4SdDetectCardType)
                            {
                                case FCI_SDHC_CARD:
                                    LOG(1,"SD-HC Card Removed @@\n");
                                    break;
                                case FCI_SD_CARD:
                                    LOG(1,"SD Card Removed @@\n");
                                    break;
                                case FCI_MMC_CARD:
                                    LOG(1,"MMC Card Removed @@\n");
                                    break;
                            }
                        }

                        _u4SdDetectCardType = FCI_UNKNOWN_CARD;
                    }

                    break;
                }

            default:
                  break;
        }
    }

    // Save for card removal flag
    fgPreSdCardRemove = _fgSdCardRemove;

    return _fgSdCardRemove;
}

//-------------------------------------------------------------------------
/** SD_Insert
*  SD card insert function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SD_Insert(VOID)
{
    INT32 i4Ret = 1;

    SDC_T *prSDC;

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    prSDC = FCI_GetSDCSlot();
    FCIHW_ResetSlot(SDC_SLOT);

    if (!SDCHW_IsEmpty())
    { /* SD or MMC ? */
        i4Ret = SDC_Init(prSDC);
    }

    if (i4Ret)
    {
        _i4SDCStatus = ((SDCHW_IsEmpty()) ? (INT32)STORG_DEV_ABSENT : (INT32)STORG_DEV_FAIL);
    }
    else
    {
        // notify block device manager.
        if ((_pfnSDCallback) && (_i4SDCStatus == (INT32)STORG_DEV_ABSENT))
        {
            _pfnSDCallback(_u4SDCallbackTag, (INT32)STORG_DEV_READY);
        }

        _i4SDCStatus = (INT32)STORG_DEV_READY;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** SD_Remove
*  SD card remove function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SD_Remove(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    prSDC->u4CardType = FCI_UNKNOWN_CARD;

    // notify block device manager.
    if ((_pfnSDCallback) && (_i4SDCStatus == (INT32)STORG_DEV_READY))
    {
        _pfnSDCallback(_u4SDCallbackTag, (INT32)STORG_DEV_ABSENT);
    }

    _i4SDCStatus = (INT32)STORG_DEV_ABSENT;

#ifdef FCI_SDIO_SUPPORT
    SDIO_DetachSDIOCard();
#endif

    return 0;
}

//-------------------------------------------------------------------------
/** SD_GetModel
*  SD card get model name function.
*  @param  VOID.
*  @return   SD model name.
*/
//-------------------------------------------------------------------------
CHAR *SD_GetModel(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    return (CHAR *)prSDC->u1PNM;
}

//-------------------------------------------------------------------------
/** SD_GetCard
*  SD card get card type function.
*  @param  VOID.
*  @return  the card is SM card or xD card.
*/
//-------------------------------------------------------------------------
INT32 SD_GetCard(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    return (INT32)(prSDC->u4CardType);
}

//-------------------------------------------------------------------------
/** SD_GetBlockSize
*  SD card get card block size function.
*  @param  VOID.
*  @return  the card block size in bytes.
*/
//-------------------------------------------------------------------------
INT32 SD_GetBlockSize(VOID)
{
    return (INT32)(SD_FCI_BLOCK_SIZE);
}

//-------------------------------------------------------------------------
/** SD_GetSize
*  SD card get card size function.
*  @param  VOID.
*  @return  the card size. Unit = sector = 512 bytes.
*/
//-------------------------------------------------------------------------
INT64 SD_GetSize(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    return (UINT64)(prSDC->u4CardSize);
}

//-------------------------------------------------------------------------
/** SD_GetStatus
*  SD card get card status.
*  @param  VOID.
*  @return  the card status in STORG_DEV_STATUS_T.
*/
//-------------------------------------------------------------------------
INT32 SD_GetStatus(VOID)
{
    return _i4SDCStatus;
}

//-------------------------------------------------------------------------
/** SD_SetHotswapCallback
* SD card register card call back function when card is removed, inserted.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at card inserted or removed.
*  @retval VOID.
*/
//-------------------------------------------------------------------------
void SD_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback)
{
    _u4SDCallbackTag = u4Tag;
    _pfnSDCallback = pfnCallback;
}

//-------------------------------------------------------------------------
/** SD_GetBlkFuncTbl
* Get SD block function table pointer.
*  @param  pFuncTable      define the query function that the block manager
*                                       can get a table of pointers to all IO functions.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 SD_GetBlkFuncTbl(BlockFunc_T *pFuncTable)
{
    x_memcpy(pFuncTable, &_rSDBlkFuncTbl, sizeof(BlockFunc_T));
    return 0;
}

#endif
