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

/** @file sd_if.c
 *  This C file implements the SD interface functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "sdc_func.h"
#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "fcihw_func.h"

#include "fci_type.h"
#include "fci_debug.h"
#include "fci_if.h"
#include "sdc_const.h"

#ifdef CC_BD_FCI
#include "drv_fci.h"
#endif

#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#if (CONFIG_DRV_FPGA_BOARD == 1)  // Don't do error handling in FPGA environment
#undef CC_FCI_ERROR_HANDLING
#endif

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

#ifdef CC_BD_FCI
extern FCI_DEV_T _rFciDev[FCI_MAX_CH_NUM * FCI_MAX_DEV_NUM];
#endif

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

#ifdef CC_BD_FCI

static INT32 _i4SDCStatus = (INT32)STORG_DEV_DETACH;
static FCI_CALLBACK _pfnSDCallback;

static STORG_DEV_T  *_prSdDev = NULL;
static INT32 _i4SdDevId = -1;    /* SD device id. */

#elif defined(CC_DTV_FCI)

static INT32 _i4SDCStatus = (INT32)STORG_DEV_ABSENT;

static UINT32 _u4SDCallbackTag;
static BLOCK_CALLBACK _pfnSDCallback;

static BlockFunc_T _rSDBlkFuncTbl =
{
    NULL, NULL, NULL, SD_GetSize, SD_GetBlockSize, SD_GetStatus,
    SD_GetModel, NULL, SD_Read, SD_Write, SD_SetHotswapCallback
};
#endif

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
/** SD_PIOModeAccessEnable
*  SD read method selection : PIO Mode / DMA mode
*  @param  fgEnable       TRUE : enable PIO mode access, FALSE : enable DMA access
*/
//-------------------------------------------------------------------------
VOID SD_PIOModeAccessEnable(BOOL fgEnable)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    prSDC->fgPIOMode = fgEnable;
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
    INT32 i4Ret = S_OK, i4Ret1;
    INT32 i4Retry = 0;

    UINT32 u4Lba, u4Offset, u4SecLen;

    UNUSED(i4Ret1);

#ifdef CC_DTV_FCI
    i4Ret1 = FCIHW_LockSemaphore();
#endif

    u4Offset = (UINT32)u8Offset;
    u4Lba = u4Offset >> SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(6, "SD_Read Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x", u4Lba, u4SecLen, u4MemPtr);
    UNUSED(u4Lba);
    FCIHW_ResetSlot(SDC_SLOT);

    while (u4MemLen > SD_FCI_MAX_DATA_SIZE)
    {
        while (i4Retry < SD_READ_RETRY)
        {
            i4Ret = SDC_ReadBlks(u4Offset, (UINT8 *)u4MemPtr, SD_FCI_MAX_BLOCK_NUM);
            
            if (i4Ret == S_OK)    // Read well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                SDC_ErrorHandling(++i4Retry);
#else
                break;
#endif                
            }
        }

        if (i4Ret != S_OK)
        {
            goto ErrorReturn;
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

ErrorReturn:
    LOG(6, " => i4Ret = 0x%x\n", i4Ret);

#ifdef CC_DTV_FCI
    i4Ret1 = FCIHW_ReleaseSemaphore();
#endif

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
    INT32 i4Ret = S_OK, i4Ret1;
    INT32 i4Retry = 0;

    UINT32 u4Lba, u4Offset, u4SecLen;

    UNUSED(i4Ret1);

#ifdef CC_DTV_FCI
    i4Ret1 = FCIHW_LockSemaphore();
#endif

    u4Offset = (UINT32)u8Offset;
    u4Lba = u4Offset >> SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(6, "SD_Write Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x", u4Lba, u4SecLen, u4MemPtr);
    UNUSED(u4Lba);
    FCIHW_ResetSlot(SDC_SLOT);

    while (u4MemLen > SD_FCI_MAX_DATA_SIZE)
    {
        while (i4Retry < SD_WRITE_RETRY)
        {
            i4Ret = SDC_WriteBlks(u4Offset, (UINT8 *)u4MemPtr, SD_FCI_MAX_BLOCK_NUM);
            
            if (i4Ret == S_OK)    // Write well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                SDC_ErrorHandling(++i4Retry);
#else
                break;
#endif
            }
        }

        if (i4Ret != S_OK)
        {            
            goto ErrorReturn;
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

ErrorReturn:
    LOG(6, " => i4Ret = 0x%x\n", i4Ret);

#ifdef CC_DTV_FCI
    i4Ret1 = FCIHW_ReleaseSemaphore();
#endif

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
#ifndef CC_CARD_DETECT_PIN
    SDC_T *prSDC;
#endif
    
#ifdef CC_DTV_FCI
    i4Ret = FCIHW_LockSemaphore();
#endif

#ifdef CC_CARD_DETECT_PIN
    // handle card detection.
    if (FCIHW_DetectCard())   // TRUE : Card remove, FALSE : Card insert
    {
        _fgSdCardRemove = TRUE; // Card not exist
        goto Exit;
    }
#endif  // #ifdef CC_CARD_DETECT_PIN

    if (fgPreSdCardRemove)
    {
#ifdef CC_FCI_POWER_CONTROL    
        // Turn Off the power then Turn on
        FCIHW_PowerSwitch(FALSE, 500);
        FCIHW_PowerSwitch(TRUE, 500);
#endif        
    }
    
    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

#ifndef CC_CARD_DETECT_PIN
    prSDC = FCI_GetSDCSlot();
#endif

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
            _u4SdDetectCardType = FCI_SD20_CARD;
            _fgSdCardRemove = FALSE;    // Card Exist ^^
            i4FindCard = TRUE;

            if(fgPreSdCardRemove != _fgSdCardRemove)
            {
                UTIL_Printf("SD-2.0 Card Exist ^^\n");
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
                    UTIL_Printf("SD-1.x Card Exist ^^\n");
                }
            }
        }

        // Check if MMC Card  insert    => use CMD 1
        if((_u4SdDetectCardType != FCI_SD20_CARD) &&    // Not SDHC or SD card
           (_u4SdDetectCardType != FCI_SD_CARD))           // Case : detect MMC last time, but can't initial mmc card
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
                    UTIL_Printf("MMC Card Exist ^^\n");
                }
            }
        }

        // Can't detect SDHC, SD, MMC card => Card is removed
        // This case happens when card insert but can't be initialized ok.
        if(!i4FindCard)
        {
            _u4SdDetectCardType = FCI_UNKNOWN_CARD;
            _fgSdCardRemove = TRUE;    // Card Not Exist
        }
    }
#ifndef CC_CARD_DETECT_PIN
    // Card is attached => detect if card removed
    else
    {
        switch(_u4SdDetectCardType)
        {
            case FCI_SD20_CARD:
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
                                case FCI_SD20_CARD:
                                    UTIL_Printf("SD-2.0 Card Removed @@\n");
                                    break;
                                case FCI_SD_CARD:
                                    UTIL_Printf("SD-1.x Card Removed @@\n");
                                    break;
                                case FCI_MMC_CARD:
                                    UTIL_Printf("MMC Card Removed @@\n");
                                    break;
                                default:
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
#endif

#ifdef CC_CARD_DETECT_PIN
Exit:

    if (fgPreSdCardRemove != _fgSdCardRemove && 
                _fgSdCardRemove == TRUE)
    {
        switch(_u4SdDetectCardType)
        {
            case FCI_SD20_CARD:
                UTIL_Printf("SD-2.0 Card Removed @@\n");
                break;
            case FCI_SD_CARD:
                UTIL_Printf("SD-1.x Card Removed @@\n");
                break;
            case FCI_MMC_CARD:
                UTIL_Printf("MMC Card Removed @@\n");
                break;
        }
    }
#endif  // #ifdef CC_CARD_DETECT_PIN

    // Save for card removal flag
    fgPreSdCardRemove = _fgSdCardRemove;

#ifdef CC_DTV_FCI
    i4Ret = FCIHW_ReleaseSemaphore();
#endif

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

#ifdef CC_DTV_FCI
    i4Ret = FCIHW_LockSemaphore();
#endif

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    prSDC = FCI_GetSDCSlot();

    // Default DMA mode access    => Move to FCI_Init
    // prSDC->fgPIOMode = FALSE;
    
    // Default Multiple block access mode
    prSDC->fgSingleBlockAccess = FALSE;

    // Default Normal Speed mode
    prSDC->fgHighSpeedMode = FALSE;
            
    // Default non Test mode
    prSDC->fgFifoFullModeTest = FALSE;
    prSDC->fgDataTimeOutTest = FALSE;
    prSDC->fgCmdTimeOutTest = FALSE;
    
    FCIHW_ResetSlot(SDC_SLOT);

    // Reset SD high / normal clock setting   
    SDC_ResetClockSetting();

    if (!SDCHW_IsEmpty())
    { /* SD or MMC ? */
        i4Ret = SDC_Init(prSDC);
    }

#ifdef CC_DTV_FCI
    i4Ret = FCIHW_ReleaseSemaphore();
#endif

#ifdef CC_BD_FCI
    if (i4Ret)
    {
        _i4SDCStatus = ((SDCHW_IsEmpty()) ? (INT32)STORG_DEV_DETACH : (INT32)STORG_DEV_FAIL);
    }
    else
    {
        // notify block device manager.
        if ((_pfnSDCallback) && (_i4SDCStatus == (INT32)STORG_DEV_DETACH))
        {
            _pfnSDCallback((INT32)STORG_DEV_ATTACH);
        }

        _i4SDCStatus = (INT32)STORG_DEV_ATTACH;
    }
#elif defined(CC_DTV_FCI)

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

#endif

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
#ifdef CC_BD_FCI
    if ((_pfnSDCallback) && (_i4SDCStatus == (INT32)STORG_DEV_ATTACH))
    {
        _pfnSDCallback((INT32)STORG_DEV_DETACH);
    }

    _i4SDCStatus = (INT32)STORG_DEV_DETACH;
#elif defined(CC_DTV_FCI)
    if ((_pfnSDCallback) && (_i4SDCStatus == (INT32)STORG_DEV_READY))
    {
        _pfnSDCallback(_u4SDCallbackTag, (INT32)STORG_DEV_ABSENT);
    }

    _i4SDCStatus = (INT32)STORG_DEV_ABSENT;
#endif

    _fgSdCardRemove = TRUE;

#ifdef CC_FCI_SDIO_SUPPORT
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
/** SD_GetBusWidth
*  MS card get card type function.
*  @param  VOID.
*  @return  the card is MS card or MSpro card.
*/
//-------------------------------------------------------------------------
INT32 SD_GetBusWidth(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    return (INT32)(prSDC->fg4BitBus ? 4:1);
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
/** SD_IsPIOMode
*  SD card get Mode function.
*  @param  VOID.
*  @return  TRUE : PIO mode, FALSE : DMA mode
*/
//-------------------------------------------------------------------------
BOOL SD_IsPIOMode(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();

    return (prSDC->fgPIOMode);
}

//-------------------------------------------------------------------------
/** SD_IsHighSpeedMode
*  SD card get Mode function.
*  @param  VOID.
*  @return  TRUE : High Speed mode, FALSE : Default Speed mode
*/
//-------------------------------------------------------------------------
BOOL SD_IsHighSpeedMode(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    
    return (prSDC->fgHighSpeedMode);
}

//-------------------------------------------------------------------------
/** SD_IsFifoTestMode
*  SD card get Mode function.
*  @param  VOID.
*  @return  TRUE : Fifo Test mode, FALSE : Normal mode
*/
//-------------------------------------------------------------------------
BOOL SD_IsFifoTestMode(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    
    return (prSDC->fgFifoFullModeTest);
}

//-------------------------------------------------------------------------
/** SD_IsCmdTimeOutTestMode
*  SD card get Mode function.
*  @param  VOID.
*  @return  TRUE : Cmd Timeout Test mode, FALSE : Normal mode
*/
//-------------------------------------------------------------------------
BOOL SD_IsCmdTimeOutTestMode(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    
    return (prSDC->fgCmdTimeOutTest);
}
  
//-------------------------------------------------------------------------
/** SD_IsDataTimeOutTestMode
*  SD card get Mode function.
*  @param  VOID.
*  @return  TRUE : Data Timeout Test mode, FALSE : Normal mode
*/
//-------------------------------------------------------------------------
BOOL SD_IsDataTimeOutTestMode(VOID)
{
    SDC_T *prSDC;

    prSDC = FCI_GetSDCSlot();
    
    return (prSDC->fgDataTimeOutTest);
}

#ifdef CC_BD_FCI
//-------------------------------------------------------------------------
/** _i4StorgSdMemoryVariaty
 *  SD convert i4Ret to Storage return value
 *  @param  i4SdRet              SD Read / Write function return value
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

static BLKDEV_MEMCARD_VARIETY_T _i4StorgSdMemoryVariaty(void)
{
    switch(SD_GetCard())
    {
        case FCI_SD_CARD:
        case FCI_SDHC_CARD:
        case FCI_SDIO_COMBO_CARD:
            return BLKDEV_MEMCARD_SD;

        case FCI_MMC_CARD:
            return BLKDEV_MEMCARD_MMC;

        // should not happen
        default:
            return BLKDEV_MEMCARD_UNKNOWN;
    }
}

//-------------------------------------------------------------------------
/** i4StorgSdGetProperty
 *  SD get property function
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdGetProperty(UINT32 u4DevId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    LOG(5,"(%s) u4PropType, = 0x%x, pvData = 0x%x, u4DataSz = 0x%x\n",__FUNCTION__, u4PropType, pvData, u4DataSz);

    switch(u4PropType)
    {
        // required for block or character devices
        case STORG_PROP_TOTAL_SIZE:
            ASSERT(pvData != NULL);
            *(UINT64 *)pvData = SD_GetSize() * SD_GetBlockSize();
            break;

        case STORG_PROP_NUMBER_OF_UNIT:
            ASSERT(pvData != NULL);
            *(UINT64 *)pvData = SD_GetSize();
            break;

        case STORG_PROP_UNIT_SIZE:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = SD_GetBlockSize();
            break;

        case STORG_PROP_MAX_READ_TRANS_UNIT:
        case STORG_PROP_MAX_WRITE_TRANS_UNIT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = SD_FCI_MAX_BLOCK_NUM;
            break;

        case STORG_PROP_BEST_READ_TRANS_UNIT:
        case STORG_PROP_BEST_WRITE_TRANS_UNIT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = SD_FCI_MAX_BLOCK_NUM;
            break;

        case STORG_PROP_MIN_READ_TRANS_UNIT:
        case STORG_PROP_MIN_WRITE_TRANS_UNIT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = 1;
            break;

        // required for all I/O devices
        case STORG_PROP_MAX_IO_REQUEST:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = 1;
            break;

        case STORG_PROP_MEMORY_ALIGNMENT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = FCI_MEM_ALIGN;     // 4 byte alignment
            break;

        case STORG_PROP_DEVICE_STATUS:
            ASSERT(pvData != NULL);
            *(STORG_DEV_ST_TYPE_T *)pvData = _prSdDev->eDevStatus;
            break;

        // optional
        case STORG_PROP_MODEL_NAME:
            {
                CHAR *pszModel = SD_GetModel();
                
                ASSERT(pvData != NULL);

                if (pszModel != NULL)
                {
                    x_strncpy((CHAR *)pvData, pszModel, u4DataSz);
                    ((CHAR *)pvData)[u4DataSz - 1] = 0;
                }
                else
                {
                    return STORG_FAIL;
                }
            }                   
            break;

        case STORG_PROP_MEMCARD_VARIETY:
            ASSERT(pvData != NULL);
            *(BLKDEV_MEMCARD_VARIETY_T *)pvData = _i4StorgSdMemoryVariaty();
            break;

        default:
            return STORG_FAIL;
    }

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** i4StorgSdSetProperty
 *  SD set property function
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdSetProperty(UINT32 u4DevId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    return STORG_FAIL;
}

//-------------------------------------------------------------------------
/** i4StorgSdInit
 *  SD Initialization interface function .
 *  @param  pvSelfDev         device information
 *  @param  pvDevStNfyFct  callback function pointer for hot-plug feature
 *  @param  pvData             private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdInit(UINT32 u4DevId, void *pvSelfDev, void *pvDevStNfyFct, void *pvData)
{
    INT32 i4Ret = 0;

    LOG(1,"i4StorgSdInit\n");

    UNUSED(pvDevStNfyFct);

    ASSERT(pvSelfDev != NULL);
    _prSdDev = pvSelfDev;

    _prSdDev->u4DevType  = DRVT_MEM_CARD;
    _prSdDev->u4DevId    = u4DevId;
    // [Klocwork] ANTI.VULNERABILITY Analyze
    x_snprintf(_prSdDev->szDevName, STORG_MAX_DEV_NAME_LENGTH, "SD_%02x", u4DevId);
    _prSdDev->u4DevFlag  = STORG_FLAG_BLOCK_DEV;
    _prSdDev->u4PollingPeriod = 0;

    // Setup & initialize SD card
    i4Ret = SD_Insert();

    // Save the Device ID for check
    _i4SdDevId = u4DevId;

    if (i4Ret == S_OK)
    {
        _prSdDev->eDevStatus = STORG_DEV_ATTACH;
        LOG(1, "Inital SD/SDHC/MMC Card OK. Card Type = %d (%s).\n",SD_GetCard(), ((SD_GetCard() == FCI_SD_CARD) ? "SD" : ((SD_GetCard() == FCI_SDHC_CARD) ? "SDHC" : "MMC")));
        i4Ret = STORG_OK;
    }
    else
    {
        _prSdDev->eDevStatus = STORG_DEV_FAIL;
        LOG(1, "Inital SD/SDHC/MMC Card Failed. Card Type = %d.\n",SD_GetCard());
        i4Ret = STORG_FAIL;
    }

    // assign device info to parent device
    _rFciDev[u4DevId].pvDevData = (void*)_prSdDev;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4StorgSdDeinit
 *  SD De-Initialization interface function .
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdDeinit(UINT32 u4DevId)
{
    LOG(5,"%s\n",__FUNCTION__);

    ASSERT(_i4SdDevId == u4DevId);

    if (_i4SdDevId != u4DevId)
    {
        LOG(5, "%s(%d) %s _i4SdDevId != u4DevId error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    SD_Remove();

    // Device ID reset to default
    _i4SdDevId = -1;

    // assign parent device info to NULL
    _rFciDev[u4DevId].pvDevData = NULL;

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** _i4StorgSdConvertSdRet
 *  SD convert i4Ret to Storage return value
 *  @param  i4SdRet              SD Read / Write function return value
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

static INT32 _i4StorgSdConvertSdRet(INT32 i4SdRet)
{
    switch(i4SdRet)
    {
        case S_OK:
            return STORG_OK;

        default:
            return STORG_FAIL;
    }
}

//-------------------------------------------------------------------------
/** i4StorgSdRead
 *  SD read function
 *  @param  u8Offset            device information
 *  @param  u4MemPtr           callback function pointer for hot-plug feature
 *  @param  u4MemLen          private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdRead(UINT32 u4DevId, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    
    ASSERT(_i4SdDevId == u4DevId);
    ASSERT(u4MemPtr != (UINT32)NULL);
    ASSERT((u4MemPtr & 0x03) == 0);

    i4Ret = SD_Read((u8Offset << SECTOR_2N), u4MemPtr, u4MemLen);
    
    LOG(5,"(%s) u8Offset = 0x%x, u4MemPtr = 0x%x, u4MemLen = 0x%x, i4Ret = 0x%x\n",__FUNCTION__, (UINT32)u8Offset, u4MemPtr, u4MemLen, i4Ret);
    
    return _i4StorgSdConvertSdRet(i4Ret);
}

//-------------------------------------------------------------------------
/** i4StorgSdWrite
 *  SD write function
 *  @param  u8Offset            device information
 *  @param  u4MemPtr           callback function pointer for hot-plug feature
 *  @param  u4MemLen          private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgSdWrite(UINT32 u4DevId, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    
    ASSERT(_i4SdDevId == u4DevId);
    ASSERT(u4MemPtr != (UINT32)NULL);
    ASSERT((u4MemPtr & 0x03) == 0);

    i4Ret = SD_Write((u8Offset << SECTOR_2N), u4MemPtr, u4MemLen);
    
    LOG(5,"(%s) u8Offset = 0x%x, u4MemPtr = 0x%x, u4MemLen = 0x%x, i4Ret = 0x%x\n",__FUNCTION__, (UINT32)u8Offset, u4MemPtr, u4MemLen, i4Ret);

    return _i4StorgSdConvertSdRet(i4Ret);
}

STORG_FUNCTBL_T _rSdFuncTbl = {
    i4StorgSdGetProperty,   // STORG_GETPROP_FCT        pfnGetProperty;
    i4StorgSdSetProperty,   // STORG_SETPROP_FCT        pfnSetProperty;
    NULL,                            // STORG_CHKDEVST_FCT     pfnChkDevSt;
    i4StorgSdInit,                // STORG_INIT_FCT               pfnInit;
    i4StorgSdDeinit,            // STORG_DEINIT_FCT          pfnDeinit;
    i4StorgSdRead,             // STORG_READ_FCT            pfnRead;
    i4StorgSdWrite,            // STORG_WRITE_FCT          pfnWrite;
    NULL,                           // STORG_ERASE_FCT          pfnErase;
    NULL,                           // STORG_FLUSH_FCT          pfnFlush;
    NULL,                           // STORG_STDBYTIME_FCT   pfnStdbyTime;
    NULL,                           // STORG_IOCTRL_FCT         pfnIoctrl;
    NULL                            // STORG_ABORT_FCT          pfnAbort;
};
#elif defined(CC_DTV_FCI)
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
#endif  // #ifdef CC_BD_FCI
#endif  // #ifdef CC_FCI_ENABLE
