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
 * $RCSfile: ms_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ms_if.c
 *  This C file implements the MS interface functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "msp_func.h"

LINT_EXT_HEADER_BEGIN

#include "fcihw_func.h"
#include "fci_debug.h"
#include "fci_if.h"

#include "x_os.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MS_FCI_BLOCK_SIZE      (512)

/// FCI support multi-block transfer (there is no limitation of max value, 128 is copy from SD)
#define MS_FCI_MAX_BLOCK_NUM      128

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

static INT32 _i4MSPStatus = (INT32)STORG_DEV_ABSENT;

static CHAR _szFCIMSPModel[STORG_DEV_MODELNAME_LEN];

static UINT32 _u4MSCallbackTag;
static BLOCK_CALLBACK _pfnMSCallback;

static BlockFunc_T _rMSBlkFuncTbl =
{
    NULL, NULL, NULL, MS_GetSize, MS_GetBlockSize, MS_GetStatus,
    MS_GetModel, NULL, MS_Read, MS_Write, MS_SetHotswapCallback
};

INT32 _fgMsCardRemove = TRUE;

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
/** MS_Read
*  MS card read function.
*  @param  u8Offset     read offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's read buffer address.
*  @param  u4MemLen      read length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MS_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    UINT32 u4Lba, u4SecLen;
    MSP_T *prMSP;

    u4Lba = (UINT32)u8Offset;
    u4Lba >>= SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(9, "MS_Read Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x\n", u4Lba, u4SecLen, u4MemPtr);

    FCIHW_ResetSlot(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        i4Ret = MSP_MsL2PRead(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
        if (i4Ret)
        {
            LOG(1, "MSP_MsL2PRead return %d\n", i4Ret);
        }
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        i4Ret = MSP_MsproReadBlks(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
        if (i4Ret)
        {
            LOG(1, "MSP_MsproReadBlks return %d\n", i4Ret);
        }
    }
    else
    {
        return E_FAIL;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** MS_Write
*  MS card write function.
*  @param  u8Offset     write offset from card, u8Offset should be multiple of page size (512 bytes).
*  @param  u4MemPtr      user's write buffer address.
*  @param  u4MemLen      write length, u4MemLen should be multiple of page size (512 bytes).
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MS_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    UINT32 u4Lba, u4SecLen;
    MSP_T *prMSP;

    u4Lba = (UINT32)u8Offset;
    u4Lba >>= SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(9, "MS_Write Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x\n", u4Lba, u4SecLen, u4MemPtr);

    FCIHW_ResetSlot(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        i4Ret = MSP_MsL2PWrite(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        i4Ret = MSP_MsproWriteBlks(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
    }
    else
    {
        return E_FAIL;
    }

    return i4Ret;
}


//-------------------------------------------------------------------------
/** MS_DetectCardRemove
 *  Detect MS slot card exist or not (MS / MSP card detection)
 *  @param  VOID
 *  @retval   S_OK : Card exist, E_FAIL : Not Exist.
 */
//-------------------------------------------------------------------------

INT32 MS_DetectCardRemove()
{
    static INT32 fgPreMsCardRemove = TRUE;
    MSP_T *prMSP;

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
    FCIHW_HWConfigSetup(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();

    // Card is not exist => initial hardware before detection
    if(MS_GetCard() == FCI_UNKNOWN_CARD)
    {
        prMSP->fg4BitEn = FALSE;
        FCIHW_ResetSlot(MSP_SLOT);
        MSPHW_Init();
    }
    else
    {
        FCIHW_ResetSlot(MSP_SLOT);
    }

    // If Set R/W Reg success => Card exist
    if (_MSP_SetRWReg() == S_OK)
    {
        _fgMsCardRemove = FALSE;    // Card Exist ^^

        if(fgPreMsCardRemove != _fgMsCardRemove)
        {
            LOG(1,"MS/MSPro Card Detected ^^\n");
        }
    }
    else
    {
        _fgMsCardRemove = TRUE;   // Card Not Exist !!

        if(fgPreMsCardRemove != _fgMsCardRemove)
        {
            LOG(1,"MS/MSPro Card Removed @@\n");
        }
    }

    // Save for card removal flag
    fgPreMsCardRemove = _fgMsCardRemove;

    return _fgMsCardRemove;
}

//-------------------------------------------------------------------------
/** MS_Insert
*  MS card insert function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MS_Insert(VOID)
{
    INT32 i4Ret = 1;

    MSP_T *prMSP;

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();
    prMSP->u4CardType = FCI_UNKNOWN_CARD;
    prMSP->fg4BitEn = FALSE;
    FCIHW_ResetSlot(MSP_SLOT);

    if (!MSPHW_IsEmpty())
    {
        // MS or MSPRO.
        i4Ret = MSP_Init(prMSP);
    }

    if (i4Ret)
    {
        _i4MSPStatus = ((MSPHW_IsEmpty()) ? (INT32)STORG_DEV_ABSENT : (INT32)STORG_DEV_FAIL);
    }
    else
    {
        // notify block device manager.
        if ((_pfnMSCallback) && (_i4MSPStatus == (INT32)STORG_DEV_ABSENT))
        {
            _pfnMSCallback(_u4MSCallbackTag, (INT32)STORG_DEV_READY);
        }

        _i4MSPStatus = (INT32)STORG_DEV_READY;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** MS_Remove
*  MS card remove function.
*  @param  VOID.
*  @retval S_OK   Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MS_Remove(VOID)
{
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();

    if (!MSPHW_IsEmpty())
    {
        VERIFY(MSP_Stop() == 0);
    }

    prMSP->u4CardType = FCI_UNKNOWN_CARD;
    prMSP->fg4BitEn = 0;
    prMSP->fgSupportParallel = 0;
    // notify block device manager.
    if ((_pfnMSCallback) && (_i4MSPStatus == (INT32)STORG_DEV_READY))
    {
        _pfnMSCallback(_u4MSCallbackTag, (INT32)STORG_DEV_ABSENT);
    }

    _i4MSPStatus = (INT32)STORG_DEV_ABSENT;

    _fgMsCardRemove = TRUE;

    return S_OK;
}

//-------------------------------------------------------------------------
/** MS_GetModel
*  MS card get model name function.
*  @param  VOID.
*  @return   MS model name.
*/
//-------------------------------------------------------------------------
CHAR *MS_GetModel(VOID)
{
    MSP_T *prMSP;

    INT32 i;

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        x_memset((void *)_szFCIMSPModel, 0, sizeof(_szFCIMSPModel));

        VERIFY(x_strncpy((CHAR *)_szFCIMSPModel, (CHAR *)prMSP->uATR.rMsBootAtr.au1ModelName,
                         STORG_DEV_MODELNAME_LEN - 1) != NULL);
        _szFCIMSPModel[STORG_DEV_MODELNAME_LEN - 1] = 0;
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        x_memset((void *)_szFCIMSPModel, 0, sizeof(_szFCIMSPModel));

        VERIFY(x_strncpy((CHAR *)_szFCIMSPModel, (CHAR *)prMSP->uATR.rMsproAtr.pu1ModelName,
                         STORG_DEV_MODELNAME_LEN - 1) != NULL);
        _szFCIMSPModel[STORG_DEV_MODELNAME_LEN - 1] = 0;
    }
    else
    {
        return NULL;
    }

    for (i = (STORG_DEV_MODELNAME_LEN - 1); i > 0; i--)
    {
        if ((_szFCIMSPModel[i] == 0) || (_szFCIMSPModel[i] == ' '))
        {
            _szFCIMSPModel[i] = 0;
        }
        else
        {
            i = 0;
        }
    }

    return _szFCIMSPModel;
}

//-------------------------------------------------------------------------
/** MS_GetCard
*  MS card get card type function.
*  @param  VOID.
*  @return  the card is MS card or MSpro card.
*/
//-------------------------------------------------------------------------
INT32 MS_GetCard(VOID)
{
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    return (INT32)(prMSP->u4CardType);
}
//-------------------------------------------------------------------------
/** MS_GetBlockSize
*  MS card get card block size function.
*  @param  VOID.
*  @return  the card block size in bytes.
*/
//-------------------------------------------------------------------------
INT32 MS_GetBlockSize(VOID)
{
    return (INT32)(MS_FCI_BLOCK_SIZE);
}

//-------------------------------------------------------------------------
/** MS_GetSize
*  MS card get card size function.
*  @param  VOID.
*  @return  the card size. Unit = sector = 512 bytes.
*/
//-------------------------------------------------------------------------
INT64 MS_GetSize(VOID)
{
    MSP_T *prMSP;

    UINT64 u8Offset;

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        u8Offset = (UINT64)((UINT32)(MS_DEV_TOTALBYTE(prMSP->uATR.rMsBootAtr)) >> 9);
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        u8Offset = (UINT64)((UINT32)(MSPRO_DEV_TOTALBYTE(prMSP->uATR.rMsproAtr.prSysInfo)) >> 9);
    }
    else
    {
        u8Offset = 0;
    }

    return (INT64)u8Offset;
}

//-------------------------------------------------------------------------
/** MS_GetStatus
*  MS card get card size function.
*  @param  VOID.
*  @return  the card size.
*/
//-------------------------------------------------------------------------
INT32 MS_GetStatus(VOID)
{
    return _i4MSPStatus;
}

//-------------------------------------------------------------------------
/** MS_SetHotswapCallback
* MS card register card call back function when card is removed, inserted.
*  @param  u4Tag             user's tag in call back function.
*  @param  pfnCallback     user's call back function, it will be called at card inserted or removed.
*  @retval VOID.
*/
//-------------------------------------------------------------------------
void MS_SetHotswapCallback(UINT32 u4Tag, BLOCK_CALLBACK pfnCallback)
{
    _u4MSCallbackTag = u4Tag;
    _pfnMSCallback = pfnCallback;
}

//-------------------------------------------------------------------------
/** MS_GetBlkFuncTbl
* Get MS block function table pointer.
*  @param  pFuncTable      define the query function that the block manager
*                                       can get a table of pointers to all IO functions.
*  @retval 0         Success.
*  @retval others  Fail.
*/
//-------------------------------------------------------------------------
INT32 MS_GetBlkFuncTbl(BlockFunc_T *pFuncTable)
{
    x_memcpy(pFuncTable, &_rMSBlkFuncTbl, sizeof(BlockFunc_T));
    return 0;
}
#endif
