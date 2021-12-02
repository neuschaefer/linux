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

/** @file ms_if.c
 *  This C file implements the MS interface functions.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "msp_func.h"
#ifdef CC_BD_FCI
#include "x_debug.h"
#endif

LINT_EXT_HEADER_BEGIN

#include "fcihw_func.h"
#include "fci_debug.h"
#include "fci_if.h"

#ifdef CC_BD_FCI
#include "drv_fci.h"
#endif

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
static INT32 _i4MSPStatus = (INT32)STORG_DEV_DETACH;
#elif defined(CC_DTV_FCI)
static INT32 _i4MSPStatus = (INT32)STORG_DEV_ABSENT;
#endif

static CHAR _szFCIMSPModel[STORG_DEV_MODELNAME_LEN];

#ifdef CC_BD_FCI
static FCI_CALLBACK _pfnMSCallback;

static STORG_DEV_T *_prMsDev = NULL;
static INT32 _i4MsDevId = -1; /* MS device id. */
#elif defined(CC_DTV_FCI)
static UINT32 _u4MSCallbackTag;
static BLOCK_CALLBACK _pfnMSCallback;

static BlockFunc_T _rMSBlkFuncTbl =
{
    NULL, NULL, NULL, MS_GetSize, MS_GetBlockSize, MS_GetStatus,
    MS_GetModel, NULL, MS_Read, MS_Write, MS_SetHotswapCallback
};
#endif


INT32 _fgMsCardRemove = TRUE;
static INT32 _fgPreMsCardRemove = TRUE;

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
/** MS_PIOModeAccessEnable
*  MS read method selection : PIO Mode / DMA mode
*  @param  fgEnable       TRUE : enable PIO mode access, FALSE : enable DMA access
*/
//-------------------------------------------------------------------------
VOID MS_PIOModeAccessEnable(BOOL fgEnable)
{
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    prMSP->fgPIOMode = fgEnable;
}

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
    INT32 i4Retry = 0;

    UINT32 u4Lba, u4SecLen;
    MSP_T *prMSP;

#ifdef CC_DTV_FCI
    FCIHW_LockSemaphore();
#endif

    u4Lba = (UINT32)u8Offset;
    u4Lba >>= SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(6, "MS_Read Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x", u4Lba, u4SecLen, u4MemPtr);

    FCIHW_ResetSlot(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        while (i4Retry < MS_READ_RETRY)
        {
            i4Ret = MSP_MsL2PRead(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
            
            if (i4Ret == S_OK)    // Read well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                MSP_ErrorHandling(++i4Retry);
#else
                break;
#endif                
            }
        }
        
        if (i4Ret)
        {
            LOG(1, "MSP_MsL2PRead return %d\n", i4Ret);
        }
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        while (i4Retry < MS_READ_RETRY)
        {
            i4Ret = MSP_MsproReadBlks(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
            
            if (i4Ret == S_OK)    // Read well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                MSP_ErrorHandling(++i4Retry);
#else
                break;
#endif                
            }
        }
        
        if (i4Ret)
        {
            LOG(1, "MSP_MsproReadBlks return %d\n", i4Ret);
        }
    }
    else
    {
        i4Ret = E_FAIL;
    }

    LOG(6, " => i4Ret = 0x%x\n", i4Ret);

#ifdef CC_DTV_FCI
    FCIHW_ReleaseSemaphore();
#endif

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
    INT32 i4Retry = 0;

    UINT32 u4Lba, u4SecLen;
    MSP_T *prMSP;

#ifdef CC_DTV_FCI
    FCIHW_LockSemaphore();
#endif

    u4Lba = (UINT32)u8Offset;
    u4Lba >>= SECTOR_2N;
    u4SecLen = (u4MemLen >> SECTOR_2N);
    LOG(6, "MS_Write Lba:0x%08x SecLen:0x%08x MemPtr:0x%08x", u4Lba, u4SecLen, u4MemPtr);

    FCIHW_ResetSlot(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();

    if (prMSP->u4CardType == FCI_MS_CARD)
    {
        while (i4Retry < MS_WRITE_RETRY)
        {
            i4Ret = MSP_MsL2PWrite(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
            
            if (i4Ret == S_OK)    // Write well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                MSP_ErrorHandling(++i4Retry);
#else
                break;
#endif                
            }
        }        
                
        if (i4Ret)
        {
            LOG(1, "MSP_MsL2PWrite return %d\n", i4Ret);
        }
    }
    else if (prMSP->u4CardType == FCI_MSPRO_CARD)
    {
        while (i4Retry < MS_WRITE_RETRY)
        {
            i4Ret = MSP_MsproWriteBlks(u4Lba, (UINT8 *)u4MemPtr, u4SecLen);
            
            if (i4Ret == S_OK)    // Write well done, break the loop
            {
                break;
            }
            else
            {
#ifdef CC_FCI_ERROR_HANDLING            
                MSP_ErrorHandling(++i4Retry);
#else
                break;
#endif                
            }
        }
                
        if (i4Ret)
        {
            LOG(1, "MSP_MsproWriteBlks return %d\n", i4Ret);
        }
    }
    else
    {
        i4Ret = E_FAIL;
    }

    LOG(6, " => i4Ret = 0x%x\n", i4Ret);

#ifdef CC_DTV_FCI
    FCIHW_ReleaseSemaphore();
#endif

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
    MSP_T *prMSP;

#ifdef CC_DTV_FCI
    FCIHW_LockSemaphore();
#endif

    // Reset clock before detect card
    MSP_ResetClockSetting();

#ifdef CC_CARD_DETECT_PIN    
    // handle card detection.
    if (FCIHW_DetectCard())   // TRUE : Card remove, FALSE : Card insert
    {        
        _fgMsCardRemove = TRUE; // Card not exist
        goto Exit;
    }
#endif  // #ifdef CC_CARD_DETECT_PIN    

    if (_fgPreMsCardRemove == TRUE)
    {
#ifdef CC_FCI_POWER_CONTROL
        // Turn Off the power then Turn on
        FCIHW_PowerSwitch(FALSE, 500);
        FCIHW_PowerSwitch(TRUE, 500);
#endif        
    }
     
#ifdef CC_CARD_DETECT_PIN    
    if (_fgPreMsCardRemove == TRUE)
#endif    
    {      
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

            if(_fgPreMsCardRemove != _fgMsCardRemove)
            {
                UTIL_Printf("MS/MSPro Card Detected ^^\n");
            }
        }
        else
        {
            _fgMsCardRemove = TRUE;   // Card Not Exist !!

            if(_fgPreMsCardRemove != _fgMsCardRemove)
            {
                UTIL_Printf("MS/MSPro Card Removed @@\n");
            }
        }
    }

#ifdef CC_CARD_DETECT_PIN    
Exit:

    if (_fgPreMsCardRemove != _fgMsCardRemove && 
        _fgMsCardRemove == TRUE)
    {
        UTIL_Printf("MS/MSPro Card Removed @@\n");
    }
#endif

    // Save for card removal flag
    _fgPreMsCardRemove = _fgMsCardRemove;

#ifdef CC_DTV_FCI
    FCIHW_ReleaseSemaphore();
#endif

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

#ifdef CC_DTV_FCI
    FCIHW_LockSemaphore();
#endif

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(MSP_SLOT);

    prMSP = FCI_GetMSPSlot();
    prMSP->u4CardType = FCI_UNKNOWN_CARD;
    prMSP->fg4BitEn = FALSE;
    FCIHW_ResetSlot(MSP_SLOT);

    // Reset MSP high / normal clock setting   
    MSP_ResetClockSetting();

    if (!MSPHW_IsEmpty())
    {
        // MS or MSPRO.
        i4Ret = MSP_Init(prMSP);
    }

#ifdef CC_DTV_FCI
    FCIHW_ReleaseSemaphore();
#endif

#ifdef CC_BD_FCI
    if (i4Ret)
    {
        _i4MSPStatus = ((MSPHW_IsEmpty()) ? (INT32)STORG_DEV_DETACH : (INT32)STORG_DEV_FAIL);
    }
    else
    {
        // notify block device manager.
        if ((_pfnMSCallback) && (_i4MSPStatus == (INT32)STORG_DEV_DETACH))
        {
            _pfnMSCallback((INT32)STORG_DEV_ATTACH);
        }

        _i4MSPStatus = (INT32)STORG_DEV_ATTACH;
    }
#elif defined(CC_DTV_FCI)
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
#endif

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
        //VERIFY(MSP_Stop() == 0);
        MSP_Stop();
    }

    prMSP->u4CardType = FCI_UNKNOWN_CARD;
    prMSP->fg4BitEn = 0;
    prMSP->fgSupportParallel = 0;
    // notify block device manager.

#ifdef CC_BD_FCI
    if ((_pfnMSCallback) && (_i4MSPStatus == (INT32)STORG_DEV_ATTACH))
    {
        _pfnMSCallback((INT32)STORG_DEV_DETACH);
    }

    _i4MSPStatus = (INT32)STORG_DEV_DETACH;
#elif defined(CC_DTV_FCI)
    if ((_pfnMSCallback) && (_i4MSPStatus == (INT32)STORG_DEV_READY))
    {
        _pfnMSCallback(_u4MSCallbackTag, (INT32)STORG_DEV_ABSENT);
    }

    _i4MSPStatus = (INT32)STORG_DEV_ABSENT;
#endif
    _fgMsCardRemove = TRUE;
    _fgPreMsCardRemove = TRUE;

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
/** MS_GetBusWidth
*  MS card get card type function.
*  @param  VOID.
*  @return  the card is MS card or MSpro card.
*/
//-------------------------------------------------------------------------
INT32 MS_GetBusWidth(VOID)
{
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();
    
    return (INT32)(prMSP->fgSupportParallel ? 4:1);
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
/** MS_IsPIOMode
*  MS card get Mode function.
*  @param  VOID.
*  @return  TRUE : PIO mode, FALSE : DMA mode
*/
//-------------------------------------------------------------------------
BOOL MS_IsPIOMode(VOID)
{
    MSP_T *prMSP;

    prMSP = FCI_GetMSPSlot();

    return (prMSP->fgPIOMode);
}

#ifdef CC_DTV_FCI
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
#endif

#ifdef CC_BD_FCI
//-------------------------------------------------------------------------
/** _i4StorgMsMemoryVariaty
 *  MS convert i4Ret to Storage return value
 *  @param  i4MsRet              Ms Read / Write function return value
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

static BLKDEV_MEMCARD_VARIETY_T _i4StorgMsMemoryVariaty(void)
{
    switch(MS_GetCard())
    {
        case FCI_MS_CARD:
            return BLKDEV_MEMCARD_MS;

        case FCI_MSPRO_CARD:
            return BLKDEV_MEMCARD_MMC;

        // should not happen
        default:
            return BLKDEV_MEMCARD_UNKNOWN;
    }
}

//-------------------------------------------------------------------------
/** i4StorgMsGetProperty
 *  MS get property function
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsGetProperty(UINT32 u4DevId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    LOG(5,"(%s) u4PropType, = 0x%x, pvData = 0x%x, u4DataSz = 0x%x\n",__FUNCTION__, u4PropType, pvData, u4DataSz);
  
    switch(u4PropType)
    {
        // required for block or character devices
        case STORG_PROP_TOTAL_SIZE:
            ASSERT(pvData != NULL);
            *(UINT64 *)pvData = MS_GetSize() * MS_GetBlockSize();
            break;

        case STORG_PROP_NUMBER_OF_UNIT:
            ASSERT(pvData != NULL);
            *(UINT64 *)pvData = MS_GetSize();
            break;

        case STORG_PROP_UNIT_SIZE:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = MS_GetBlockSize();
            break;

        case STORG_PROP_MAX_READ_TRANS_UNIT:
        case STORG_PROP_MAX_WRITE_TRANS_UNIT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = MS_FCI_MAX_BLOCK_NUM;
            break;

        case STORG_PROP_BEST_READ_TRANS_UNIT:
        case STORG_PROP_BEST_WRITE_TRANS_UNIT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = MS_FCI_MAX_BLOCK_NUM;
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
            *(STORG_DEV_ST_TYPE_T *)pvData = _prMsDev->eDevStatus;
            break;

        // optional
        case STORG_PROP_MODEL_NAME:
            {
                CHAR *pszModel = MS_GetModel();
                
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
            *(BLKDEV_MEMCARD_VARIETY_T *)pvData = _i4StorgMsMemoryVariaty();
            break;

        default:
            return STORG_FAIL;
    }

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** i4StorgMsSetProperty
 *  MS set property function
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsSetProperty(UINT32 u4DevId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    return STORG_FAIL;
}

//-------------------------------------------------------------------------
/** i4StorgMsInit
 *  MS Initialization interface function .
 *  @param  pvSelfDev         device information
 *  @param  pvDevStNfyFct  callback function pointer for hot-plug feature
 *  @param  pvData             private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsInit(UINT32 u4DevId, void *pvSelfDev, void *pvDevStNfyFct, void *pvData)
{
    INT32 i4Ret = 0;

    LOG(5,"%s\n", __FUNCTION__);

    UNUSED(pvDevStNfyFct);

    ASSERT(pvSelfDev != NULL);
    _prMsDev = pvSelfDev;

    _prMsDev->u4DevType  = DRVT_MEM_CARD;
    _prMsDev->u4DevId    = u4DevId;
    // [Klocwork] ANTI.VULNERABILITY Analyze
    x_snprintf(_prMsDev->szDevName, STORG_MAX_DEV_NAME_LENGTH, "MS_%02x", u4DevId);
    _prMsDev->u4DevFlag  = STORG_FLAG_BLOCK_DEV;
    _prMsDev->u4PollingPeriod = 0;

    // Setup & initialize Ms card
    i4Ret = MS_Insert();

    // Save the Device ID for check
    _i4MsDevId = u4DevId;

    if (i4Ret == S_OK)
    {
        _prMsDev->eDevStatus = STORG_DEV_ATTACH;
        LOG(1, "Initial MS/MSPro OK. Card Type = %d (%s).\n", MS_GetCard(), ((MS_GetCard() == FCI_MSPRO_CARD) ? "MSP" : "MS"));
        i4Ret = STORG_OK;
    }
    else
    {
        _prMsDev->eDevStatus = STORG_DEV_FAIL;
        LOG(1, "Inital MS/MSPro Failed.\n");
        i4Ret = STORG_FAIL;
    }

    // assign device info to parent device
    _rFciDev[u4DevId].pvDevData = (void*)_prMsDev;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4StorgMsDeinit
 *  MS De-Initialization interface function .
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsDeinit(UINT32 u4DevId)
{
    LOG(5,"%s\n", __FUNCTION__);

    ASSERT(_i4MsDevId == u4DevId);

    if (_i4MsDevId != u4DevId)
    {
        LOG(5, "%s(%d) %s _i4SdDevId != u4DevId error.\n", __FILE__, __LINE__, __FUNCTION__);
    }

    MS_Remove();

    // Device ID reset to default
    _i4MsDevId = -1;

    // assign parent device info to NULL
    _rFciDev[u4DevId].pvDevData = NULL;

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** _i4StorgMsConvertMsRet
 *  MS convert i4Ret to Storage return value
 *  @param  i4MsRet              MS Read / Write function return value
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

static INT32 _i4StorgMsConvertMsRet(INT32 i4MsRet)
{
    switch(i4MsRet)
    {
        case S_OK:
            return STORG_OK;

        default:
            return STORG_FAIL;
    }
}

//-------------------------------------------------------------------------
/** i4StorgMsRead
 *  MS read function
 *  @param  u8Offset            device information
 *  @param  u4MemPtr           callback function pointer for hot-plug feature
 *  @param  u4MemLen          private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsRead(UINT32 u4DevId, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    
    ASSERT(_i4MsDevId == u4DevId);
    ASSERT(u4MemPtr != (UINT32)NULL);
    ASSERT((u4MemPtr & 0x03) == 0);

    i4Ret = MS_Read((u8Offset << SECTOR_2N), u4MemPtr, u4MemLen);

    LOG(5,"(%s) u8Offset = 0x%x, u4MemPtr = 0x%x, u4MemLen = 0x%x, i4Ret = 0x%x\n",__FUNCTION__, (UINT32)u8Offset, u4MemPtr, u4MemLen, i4Ret);

    return _i4StorgMsConvertMsRet(i4Ret);
}

//-------------------------------------------------------------------------
/** i4StorgMsWrite
 *  MS write function
 *  @param  u8Offset            device information
 *  @param  u4MemPtr           callback function pointer for hot-plug feature
 *  @param  u4MemLen          private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgMsWrite(UINT32 u4DevId, UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    
    ASSERT(_i4MsDevId == u4DevId);
    ASSERT(u4MemPtr != (UINT32)NULL);
    ASSERT((u4MemPtr & 0x03) == 0);

    i4Ret = MS_Write((u8Offset << SECTOR_2N), u4MemPtr, u4MemLen);

    LOG(5,"(%s) u8Offset = 0x%x, u4MemPtr = 0x%x, u4MemLen = 0x%x, i4Ret = 0x%x\n",__FUNCTION__, (UINT32)u8Offset, u4MemPtr, u4MemLen, i4Ret);

    return _i4StorgMsConvertMsRet(i4Ret);
}

STORG_FUNCTBL_T _rMsFuncTbl = {
    i4StorgMsGetProperty,    // STORG_GETPROP_FCT        pfnGetProperty;
    i4StorgMsSetProperty,    // STORG_SETPROP_FCT        pfnSetProperty;
    NULL,                             // STORG_CHKDEVST_FCT     pfnChkDevSt;
    i4StorgMsInit,                // STORG_INIT_FCT               pfnInit;
    i4StorgMsDeinit,            // STORG_DEINIT_FCT          pfnDeinit;
    i4StorgMsRead,             // STORG_READ_FCT            pfnRead;
    i4StorgMsWrite,            // STORG_WRITE_FCT          pfnWrite;
    NULL,                            // STORG_ERASE_FCT          pfnErase;
    NULL,                            // STORG_FLUSH_FCT          pfnFlush;
    NULL,                            // STORG_STDBYTIME_FCT   pfnStdbyTime;
    NULL,                            // STORG_IOCTRL_FCT         pfnIoctrl;
    NULL                             // STORG_ABORT_FCT          pfnAbort;
};

#elif defined(CC_DTV_FCI)

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

#endif  // #ifdef CC_BD_FCi
#endif  // #ifdef CC_FCI_ENABLE
