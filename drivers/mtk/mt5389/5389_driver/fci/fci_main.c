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

/** @file fci_main.c
 *  This C file implements the FCI interface with strage driver
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef CC_FCI_ENABLE

#include "fci_if.h"

LINT_EXT_HEADER_BEGIN

#ifdef CC_BD_FCI
#include "x_hal_1176.h"
#include "x_hal_ic.h"
#endif
#include "x_typedef.h"
#include "x_assert.h"
#include "x_util.h"

#include "fci_type.h"
#include "fci_debug.h"
#include "fcihw_func.h"

#ifdef CC_BD_FCI
#include "drv_fci.h"
#endif

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

#ifdef CC_BD_FCI
extern STORG_FUNCTBL_T _rSdFuncTbl;
extern STORG_FUNCTBL_T _rMsFuncTbl;
#endif

extern INT32 _fgSdCardRemove;
extern INT32 _fgMsCardRemove;

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
static BOOL _fgFciHwInit = FALSE;
static FCI_CH_T _rFciCh;
FCI_DEV_T _rFciDev[FCI_MAX_CH_NUM * FCI_MAX_DEV_NUM];
#endif

BOOL _fgFCIPolling = TRUE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#ifdef CC_BD_FCI
//-------------------------------------------------------------------------
/** i4FciGetSysParm
 *  Get FCI system parameter function
 *  @param  rType              Type of request parameter
 *  @return  S_OK
 */
//-------------------------------------------------------------------------

INT32 i4FciGetSysParm(FCI_SYS_PARM_TYPE_T rType)
{
    switch(rType)
    {
        case FCI_PARM_MAX_DEV_NUM:
            return FCI_MAX_DEV_NUM;
    }

    return S_OK;
}

//-------------------------------------------------------------------------
/** i4FciInitHw
 *  FCI hardware Initialization
 *  @return  S_OK / E_FAIL
 */
//-------------------------------------------------------------------------

static INT32 i4FciInitHw(void)
{
    INT32 i4Ret;
    INT32   i4Idx;

    LOG(5, "i4FciInitHw.\n");

    if (!_fgFciHwInit)
    {
        // Init FCI driver data structure
        for (i4Idx = 0; i4Idx < (FCI_MAX_CH_NUM * FCI_MAX_DEV_NUM); i4Idx ++)
        {
            _rFciDev[i4Idx].u4CardClass = FCI_UNKNOWN_CLASS;
            _rFciDev[i4Idx].pvDevData = NULL;
            _rFciDev[i4Idx].fgAttached = FALSE;
        }
    }

    i4Ret = FCI_RegInit();

    if (i4Ret != S_OK)
    {
        return E_FAIL;
    }

    // Setup multi-func, driving current & pull up resistor
    FCIHW_HWConfigSetup(SDC_SLOT);

    // Setup multi-func & driving current
    FCIHW_HWConfigSetup(MSP_SLOT);

    _fgFciHwInit = TRUE;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4FciDeInitHw
 *  FCI hardware De-Initialization
 *  @return  S_OK / E_FAIL
 */
//-------------------------------------------------------------------------

static INT32 i4FciDeInitHw(void)
{
    INT32 i4Ret;

    LOG(5, "i4FciDeInitHw.\n");

    i4Ret = FCI_RegDeInit();

    if (i4Ret != S_OK)
    {
        return E_FAIL;
    }

    _fgFciHwInit = FALSE;

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4FciDetectDev
 *  FCI card detection
 *  @return  S_OK / E_FAIL
 */
//-------------------------------------------------------------------------

static INT32 i4FciDetectDev(void)
{
    INT32 i4CardRemove = TRUE;
    
    LOG(5, "(%s, %d, %d)\n", __FUNCTION__, _fgSdCardRemove, _fgMsCardRemove);

#ifdef CC_FCI_3_IN_1_SOCKET
    if (_fgMsCardRemove)
#endif    
    {
        LOG(2, "=>Start to Detect SDSlot.\n");

        // Setup & Detect SDHC/SD/MMC card
        i4CardRemove = SD_DetectCardRemove();

        if (i4CardRemove == 0)   // Remove = False => Card exist
        {
            _rFciDev[SDC_SLOT].u4CardClass = FCI_SDMMC_CLASS;
            
    #ifdef CC_FCI_3_IN_1_SOCKET        // 3 in 1 socket, there is only one card inside at one time
            return i4CardRemove;
    #endif        
        }
        else
        {
            _rFciDev[SDC_SLOT].u4CardClass = FCI_UNKNOWN_CLASS;
        }
    }

#ifdef CC_FCI_3_IN_1_SOCKET
    if (_fgSdCardRemove)
#endif    
    {
        LOG(2, "=>Start to Detect MSSlot.\n");

        // Setup & Detect MS card
        i4CardRemove = MS_DetectCardRemove();

        if (i4CardRemove == 0)   // Remove = False => Card exist
        {
            _rFciDev[MSP_SLOT].u4CardClass = FCI_MSMSP_CLASS;
        }
        else
        {
            _rFciDev[MSP_SLOT].u4CardClass = FCI_UNKNOWN_CLASS;
        }
    }
    
    return i4CardRemove;
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** i4StorgFciGetProperty
 *  FCI Get hardware property
 *  @param  u4ChId             Channel ID
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgFciGetProperty(UINT32 u4ChId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    STORG_DEV_T *_prStorgChDev;

    _prStorgChDev = (STORG_DEV_T *)_rFciCh.pvChData;

    switch(u4PropType)
    {
        // required for all I/O devices
        case STORG_PROP_DEVICE_STATUS:
            ASSERT(pvData != NULL);
            *(STORG_DEV_ST_TYPE_T *)pvData = _prStorgChDev->eDevStatus;
            break;

        case STORG_PROP_MAX_IO_REQUEST:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = 1;
            break;

        case STORG_PROP_MEMORY_ALIGNMENT:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = FCI_MEM_ALIGN;     // 4 byte alignment
            break;

        // optional
        case STORG_PROP_MODEL_NAME:
            ASSERT(pvData != NULL);
            x_snprintf((CHAR *)pvData, u4DataSz, "FCI HARDWARE SET#%d", u4ChId);
            ((CHAR *)pvData)[u4DataSz - 1] = 0;
            break;

        // required for socket devices
        case STORG_PROP_NUMBER_OF_SOCKET:
            ASSERT(pvData != NULL);
            *(INT32 *)pvData = i4FciGetSysParm(FCI_PARM_MAX_DEV_NUM);
            break;

        case STORG_PROP_CHILD_DEV_INFO:
        {
            UINT32 u4DevId;
            STORG_DEV_T *_prStorgDevData;
            STORG_CHILD_DEV_INFO_T *prChildDevInfo  = NULL;

            ASSERT(pvData != NULL);
            prChildDevInfo = (STORG_CHILD_DEV_INFO_T *)pvData;

            u4DevId = prChildDevInfo->u4SocketNo;
            _prStorgDevData = (STORG_DEV_T *)_rFciDev[u4DevId].pvDevData;

            if ((_prStorgDevData != NULL) &&
                (_prStorgDevData->fgInit))
            {
                STORG_CHILD_SUP_DEV_T *prSupDev = NULL;
                prChildDevInfo->eChildDevSt = _prStorgDevData->eDevStatus;

                prSupDev = &((prChildDevInfo->u_info).rSupDev);
                prSupDev->u4ChildDevType  = _prStorgDevData->u4DevType;
                prSupDev->u4ChildDevId    = _prStorgDevData->u4DevId;
                prSupDev->u4ChildDevFlag  = _prStorgDevData->u4DevFlag;
            }
            else
            {
                STORG_CHILD_SUP_DEV_T *prSupDev = NULL;
                prChildDevInfo->eChildDevSt = STORG_DEV_DETACH;

                prSupDev = &((prChildDevInfo->u_info).rSupDev);
                prSupDev->u4ChildDevType  = DRVT_UNKNOWN;
                prSupDev->u4ChildDevId    = 0;
                prSupDev->u4ChildDevFlag  = 0;
            }
            break;
        }

        default:
            return STORG_ERR_NOT_SUPPORT;
    }

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** i4StorgFciGetProperty
 *  FCI Get hardware property
 *  @param  u4ChId             Channel ID
 *  @param  u4PropType       Peroperty type
 *  @param  pvData             Pointer to data structure
 *  @param  u4DataSz          Data size of data structure
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgFciSetProperty(UINT32 u4ChId, UINT32 u4PropType, void *pvData, UINT32 u4DataSz)
{
    return STORG_ERR_NOT_SUPPORT;
}

//-------------------------------------------------------------------------
/** i4StorgFciChkDevSt
 *  FCI Initialization interface function .
 *  @param  u4ChId             Channel ID
 *  @return  STORG_OK
 */
//-------------------------------------------------------------------------

INT32 i4StorgFciChkDevSt(UINT32 u4ChId)
{
    INT32   i4DevIdx;
    UINT32  u4DevId;

    if (!_fgFCIPolling)
    {
        return STORG_OK;
    }
    
    LOG(5, "i4StorgFciChkDevSt\n");

    i4FciDetectDev();

    for (i4DevIdx = 0; i4DevIdx < i4FciGetSysParm(FCI_PARM_MAX_DEV_NUM); i4DevIdx ++)
    {
        u4DevId = (u4ChId << 1) | i4DevIdx;

        if(_rFciCh.pvNfyFct != NULL)
        {
            // Find Device exist, but not Attached yet
            if (_rFciDev[u4DevId].u4CardClass != FCI_UNKNOWN_CLASS &&
                _rFciDev[u4DevId].fgAttached  == FALSE)
            {
#if !(CONFIG_SECTION_BUILD_LINUX_KO)
                STORG_DEV_ST_T            *prDevSt;
                STORG_DRV_DEV_ST_NFY_FCT  _pfnStorgDevStNfy;

                prDevSt = _prSTORGDevStAlloc();

                prDevSt->eDevStType = STORG_DEV_ATTACH;
                prDevSt->pvPareDev = _rFciCh.pvChData;
                prDevSt->u4PareSocketNo = i4DevIdx;
                prDevSt->u4ChildDevId = u4DevId;

                if (_rFciDev[u4DevId].u4CardClass == FCI_SDMMC_CLASS)
                {
                    prDevSt->prChildFuncTbl = &_rSdFuncTbl;
                }
                else if (_rFciDev[u4DevId].u4CardClass == FCI_MSMSP_CLASS)
                {
                    prDevSt->prChildFuncTbl = &_rMsFuncTbl;
                }

                prDevSt->pvData = (void *)&(_rFciDev[u4DevId].pvDevData);

                _pfnStorgDevStNfy = (STORG_DRV_DEV_ST_NFY_FCT)_rFciCh.pvNfyFct;
                _pfnStorgDevStNfy(prDevSt);

                // Device Attached flag
                _rFciDev[u4DevId].fgAttached = TRUE;
#endif  // #if !(CONFIG_SECTION_BUILD_LINUX_KO)
                LOG(1, "i4StorgFciChkDevSt - Device Attach u4DevId = %d\n",u4DevId);
            }
            // Device is removed, but not Detached yet
            else if(_rFciDev[u4DevId].u4CardClass == FCI_UNKNOWN_CLASS &&
                      _rFciDev[u4DevId].fgAttached  == TRUE)
            {
#if !(CONFIG_SECTION_BUILD_LINUX_KO)            
                STORG_DEV_ST_T *prDevSt = NULL;
                STORG_DRV_DEV_ST_NFY_FCT  _pfnStorgDevStNfy;

                prDevSt = _prSTORGDevStAlloc();

                prDevSt->eDevStType     = STORG_DEV_DETACH;
                prDevSt->pvPareDev      = _rFciCh.pvChData;
                prDevSt->u4PareSocketNo = i4DevIdx;
                prDevSt->u4ChildDevId   = u4DevId;

                if (_rFciDev[u4DevId].u4CardClass == FCI_SDMMC_CLASS)
                {
                    prDevSt->prChildFuncTbl = &_rSdFuncTbl;
                }
                else if (_rFciDev[u4DevId].u4CardClass == FCI_MSMSP_CLASS)
                {
                    prDevSt->prChildFuncTbl = &_rMsFuncTbl;
                }

                // prDevSt->pvData = (void *)&(_rFciDev[u4DevId].pvDevData);
                prDevSt->pvData = NULL;

                _pfnStorgDevStNfy = (STORG_DRV_DEV_ST_NFY_FCT)_rFciCh.pvNfyFct;
                _pfnStorgDevStNfy(prDevSt);

                // Device Detach
                _rFciDev[u4DevId].fgAttached = FALSE;
#endif  // #if !(CONFIG_SECTION_BUILD_LINUX_KO)
                LOG(1, "i4StorgFciChkDevSt - Device Detach u4DevId = %d\n",u4DevId);
            }
        }
    }

    return STORG_OK;
}

//-------------------------------------------------------------------------
/** i4StorgFciInit
 *  FCI Initialization interface function .
 *  @param  u4ChId             Channel ID
 *  @param  pvSelfDev         device information
 *  @param  pvDevStNfyFct  callback function pointer for hot-plug feature
 *  @param  pvData             private data passed from its parent device
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgFciInit(UINT32 u4ChId, void *pvSelfDev, void *pvDevStNfyFct, void *pvData)
{
    INT32       i4Ret;
    STORG_DEV_T *_prStorgDev;

    LOG(1, "i4StorgFciInit\n");

    ASSERT(pvSelfDev != NULL);

    // Init FCI channel data structure
    _rFciCh.pvChData  = pvSelfDev;
    _rFciCh.pvNfyFct  = pvDevStNfyFct;

    _prStorgDev = (STORG_DEV_T *)_rFciCh.pvChData;
    _prStorgDev->u4DevType  = DRVT_HW_FCI;
    _prStorgDev->u4DevId    = u4ChId;
    // [Klocwork] ANTI.VULNERABILITY Analyze
    x_snprintf(_prStorgDev->szDevName, STORG_MAX_DEV_NAME_LENGTH, "FCI_%02x", u4ChId);
    _prStorgDev->u4DevFlag  = STORG_FLAG_SOCKET_DEV |
                                          STORG_FLAG_HOTSWAP_POLLING |
                                          STORG_FLAG_POLLING_IN_IDLE;
    _prStorgDev->u4PollingPeriod = 1500;

    i4Ret = i4FciInitHw();

    if (i4Ret == S_OK)
    {
        _prStorgDev->eDevStatus = STORG_DEV_ATTACH;
        i4Ret = STORG_OK;
    }
    else
    {
        _prStorgDev->eDevStatus = STORG_DEV_FAIL;
        i4Ret = STORG_FAIL;
    }

    return i4Ret;
}

//-------------------------------------------------------------------------
/** i4StorgFciDeInit
 *  FCI De-Initialization interface function .
 *  @param  u4ChId             Channel ID
 *  @return  STORG_OK / STORG_FAIL
 */
//-------------------------------------------------------------------------

INT32 i4StorgFciDeInit(UINT32 u4ChId)
{
    INT32       i4Ret;

    // De-Init FCI channel data structure
    _rFciCh.pvChData  = NULL;
    _rFciCh.pvNfyFct  = NULL;

    i4Ret = i4FciDeInitHw();

    return i4Ret;
}

STORG_FUNCTBL_T _rFciFuncTbl = {
    i4StorgFciGetProperty,    // STORG_GETPROP_FCT        pfnGetProperty;
    i4StorgFciSetProperty,    // STORG_SETPROP_FCT        pfnSetProperty;
    i4StorgFciChkDevSt,       // STORG_CHKDEVST_FCT     pfnChkDevSt;
    i4StorgFciInit,                // STORG_INIT_FCT               pfnInit;
    i4StorgFciDeInit,            // STORG_DEINIT_FCT          pfnDeinit;
    NULL,          // STORG_READ_FCT            pfnRead;
    NULL,          // STORG_WRITE_FCT          pfnWrite;
    NULL,          // STORG_ERASE_FCT          pfnErase;
    NULL,          // STORG_FLUSH_FCT          pfnFlush;
    NULL,          // STORG_STDBYTIME_FCT   pfnStdbyTime;
    NULL,          // STORG_IOCTRL_FCT         pfnIoctrl;
    NULL           // STORG_ABORT_FCT          pfnAbort;
};

#else   // CC_DTV_FCI

static HANDLE_T _hFCICardDetThrd = NULL;

#define FCI_CARD_DET_STACK_SZ        (1024)
#define FCI_CARD_DET_PRIORITY         (40)
#define FCI_CARD_DET_PERIOD            (500)

static FCI_CALLBACK _pfnCardDetect = NULL;

//-------------------------------------------------------------------------
/** FCIHW_SetCardDetectFct
 *  Register FCI card insert/remove call back function.
 *  NOTICE: This function will be called in FCI HISR.
 *  @param  pfnCallback   call back funtion pointer.
 *  @return  VOID
 */
//-------------------------------------------------------------------------

VOID FCIHW_SetCardDetectFct(FCI_CALLBACK pfnCallback)
{
    _pfnCardDetect = pfnCallback;
}

//-------------------------------------------------------------------------
/** _FCICardDetThrd
 *  FCI Card detection thread
 */
//-------------------------------------------------------------------------
static VOID _FCICardDetThrd(VOID *pvArgv)
{
    INT32 i4CardRemove = TRUE;
    INT32 i4LastSDCardRemove = TRUE;
    INT32 i4LastMSCardRemove = TRUE;
    
    while(1)
    {    
        if (!_fgFCIPolling)
        {
            x_thread_delay(FCI_CARD_DET_PERIOD);
            continue;
        }
        
#ifdef CC_FCI_3_IN_1_SOCKET
        if (_fgMsCardRemove)
#endif    
        {
            // Setup & Detect SDHC/SD/MMC card
            i4CardRemove = SD_DetectCardRemove();

            if (i4CardRemove != i4LastSDCardRemove)   
            {
                if (i4CardRemove == 0)   // Remove = False => Card exist
                {
                    LOG(1, "SD/MMC Card Insert ^^\n");
                }
                else
                {
                    LOG(1, "SD/MMC Card Removed @@.\n");
                }
                
                if (_pfnCardDetect)
                {
                    _pfnCardDetect(i4CardRemove);
                }                
            }

            i4LastSDCardRemove = i4CardRemove;
        }

#ifdef CC_FCI_3_IN_1_SOCKET
        if (_fgSdCardRemove)
#endif    
        {
            // Setup & Detect MS card
            i4CardRemove = MS_DetectCardRemove();

            if (i4CardRemove != i4LastMSCardRemove)   
            {
                if (i4CardRemove == 0)   // Remove = False => Card exist
                {
                    LOG(1, "MSP Card Insert ^^\n");
                }
                else
                {
                    LOG(1, "MSP Card Removed @@.\n");
                }
                
                if (_pfnCardDetect)
                {
                    _pfnCardDetect(i4CardRemove);
                }
            }
            
            i4LastMSCardRemove = i4CardRemove;
        }

        x_thread_delay(FCI_CARD_DET_PERIOD);
    }
}

INT32 i4FciCardDetInit(VOID)
{
    INT32 i4Ret;

    if (_hFCICardDetThrd == NULL)
    {
        i4Ret = x_thread_create(&_hFCICardDetThrd, "FCI_DET",
                                FCI_CARD_DET_STACK_SZ, FCI_CARD_DET_PRIORITY,
                                _FCICardDetThrd, 0,
                                NULL);

        if (i4Ret != OSR_OK)
        {
            LOG(1, "Create FCI card detection thread failed.\n");
            return 1;
        }
        else
        {
            LOG(1, "Create FCI card detection thread Successfully !!.\n");
        }
    }

    return 0;
}        
#endif // #ifdef CC_BD_FCI

#endif  // #ifdef CC_FCI_ENABLE
