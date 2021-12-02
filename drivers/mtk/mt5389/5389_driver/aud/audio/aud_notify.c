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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: aud_notify.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_notify.c
 *  Brief of file aud_notify.c.
 *  Details of file aud_notify.c (optional).
 */

#define AUD_EXTERN_FILE

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "aud_if.h"
#include "drvcust_if.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "aud_drv.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#if 0 //ndef CC_MT5391_AUD_3_DECODER
#define NOTIFY_FUNC_NUM 5
#else
#define NOTIFY_FUNC_NUM 6
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _NOTIFY_FUNC_INFO_T
{
    BOOL         fgRegistered;
    BOOL         fgIgnoreMainDecNotify;
    BOOL         fgIgnoreAuxDecNotify;
#if 1 //def CC_MT5391_AUD_3_DECODER
    BOOL         fgIgnoreThirdDecNotify;
#endif
    UINT32       u4Handler;
    AudDecNfyFct pfNotifyFunc;
} NOTIFY_FUNC_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static NOTIFY_FUNC_INFO_T _arNotifyFuncInfo[NOTIFY_FUNC_NUM];
static HANDLE_T _rProtectSema = NULL_HANDLE;
#ifdef CC_AUD_SETUP_MELODY
static BOOL fgIgnoreSetupPlayNotify;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _fgIsListFull(void)
{
    INT32 i;
    for (i=0; i<NOTIFY_FUNC_NUM; i++)
    {
       if (!_arNotifyFuncInfo[i].fgRegistered )
       {
           return FALSE;
       }
    }

    return TRUE;
}

static BOOL _GetFreeList(UINT8 * pu1Index)
{
    INT32 i;
    if (pu1Index == NULL)
    {
        return FALSE;
    }

    *pu1Index = 0xff;

    for (i=0; i<NOTIFY_FUNC_NUM; i++)
    {
        if (!_arNotifyFuncInfo[i].fgRegistered )
        {
            *pu1Index = i;
            break;
        }
    }

    return (*pu1Index == 0xff) ? FALSE : TRUE;
}

static void _AudNotifyInit(void)
{
    UINT32 i;
    static BOOL fgInit = FALSE;
    
    // Init at once
    if (!fgInit)
    {       
        // Create protect semaphore
        VERIFY (x_sema_create(&_rProtectSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);
        
        // Notify information initialization
        for (i=0; i<NOTIFY_FUNC_NUM; i++)
        {
            x_memset((VOID *)VIRTUAL((UINT32)&_arNotifyFuncInfo[i]), 0, sizeof(NOTIFY_FUNC_INFO_T));
            _arNotifyFuncInfo[i].fgRegistered = FALSE;
            _arNotifyFuncInfo[i].fgIgnoreMainDecNotify = FALSE;
            _arNotifyFuncInfo[i].fgIgnoreAuxDecNotify = FALSE;
#if 1 //def CC_MT5391_AUD_3_DECODER
            _arNotifyFuncInfo[i].fgIgnoreThirdDecNotify = FALSE;
#endif
            _arNotifyFuncInfo[i].u4Handler = i + 1000;
            _arNotifyFuncInfo[i].pfNotifyFunc = NULL;
#ifdef CC_AUD_SETUP_MELODY
            fgIgnoreSetupPlayNotify = FALSE;
#endif
        }

        fgInit = TRUE;
    }    
}

//-----------------------------------------------------------------------------
//  _AudDrvNotifyFunc
//
/** Brief of _AudDrvNotifyFunc.
 *  Note that notify function cannot be in ISR context
 */
//-----------------------------------------------------------------------------
static void _AudDrvNotifyFunc(void *          pvNfyTag,
                              UINT8           u1DecId,
                              AUD_COND_NFY_T  eNfyCond,
                              UINT32          u4Data1,
                              UINT32          u4Data2)
{
    INT32 i;
    UINT32 u4NfyTag = 0;
                    
    _AudNotifyInit();
    VERIFY(x_sema_lock(_rProtectSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    u4NfyTag = ((UINT32)pvNfyTag & 0xF);

    // Call registered notify function
    for (i=0; i<NOTIFY_FUNC_NUM; i++)
    {
        if ((_arNotifyFuncInfo[i].fgRegistered ) && (_arNotifyFuncInfo[i].pfNotifyFunc != NULL))
        {
            // There are some dual decoder special cases that need to ignore notify function
            if ((eNfyCond == AUD_COND_CTRL_DONE) && ((AUD_NFY_TAG_T)u4NfyTag == AUD_NFY_STREAM))
            {
#if 0 //ndef CC_MT5391_AUD_3_DECODER            
                if (((u1DecId == AUD_DEC_MAIN) && !_arNotifyFuncInfo[i].fgIgnoreMainDecNotify) ||
                ((u1DecId == AUD_DEC_AUX) && !_arNotifyFuncInfo[i].fgIgnoreAuxDecNotify))
#else
                if (((u1DecId == AUD_DEC_MAIN) && !_arNotifyFuncInfo[i].fgIgnoreMainDecNotify) ||
                ((u1DecId == AUD_DEC_AUX) && !_arNotifyFuncInfo[i].fgIgnoreAuxDecNotify) ||
                ((u1DecId == AUD_DEC_THIRD) && !_arNotifyFuncInfo[i].fgIgnoreThirdDecNotify))
#endif
                {
#ifdef CC_AUD_SETUP_MELODY
                    if (!fgIgnoreSetupPlayNotify)
#endif			
                    _arNotifyFuncInfo[i].pfNotifyFunc(pvNfyTag, u1DecId,eNfyCond, u4Data1, u4Data2);    
                }    
            } // Handle AUD_INFO_CHG notification in dual decoder mode

#if 0 //ndef CC_MT5391_AUD_3_DECODER            
            else if (((u1DecId == AUD_DEC_MAIN) && !_arNotifyFuncInfo[i].fgIgnoreMainDecNotify) ||
                ((u1DecId == AUD_DEC_AUX) && !_arNotifyFuncInfo[i].fgIgnoreAuxDecNotify))
#else
            else if (((u1DecId == AUD_DEC_MAIN) && !_arNotifyFuncInfo[i].fgIgnoreMainDecNotify) ||
                ((u1DecId == AUD_DEC_AUX) && !_arNotifyFuncInfo[i].fgIgnoreAuxDecNotify) || 
                ((u1DecId == AUD_DEC_THIRD) && !_arNotifyFuncInfo[i].fgIgnoreThirdDecNotify))
#endif
            {
#ifdef CC_AUD_SETUP_MELODY
                if (!fgIgnoreSetupPlayNotify)
#endif			
                _arNotifyFuncInfo[i].pfNotifyFunc(pvNfyTag, u1DecId,eNfyCond, u4Data1, u4Data2);

            }    
        }
    }
#ifdef CC_AUD_SETUP_MELODY
    if ((eNfyCond == AUD_COND_AUD_CLIP_DONE) && ((AUD_NFY_TAG_T)u4NfyTag == AUD_NFY_STREAM))
    {
        fgIgnoreSetupPlayNotify = FALSE;
        AUD_SetSetupPlayStatue(TRUE);
    }
#endif	

    VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
}

//-----------------------------------------------------------------------------
//  AUD_GetNotifyFunc
//
/** Brief of AUD_GetNotifyFunc.
 *  Details of AUD_GetNotifyFunc (optional).
 *
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
BOOL _AUD_GetNotifyFunc(AudDecNfyFct * pfNfyFunc)
{
    if (pfNfyFunc)
    {
        *pfNfyFunc = _AudDrvNotifyFunc;        
    }
    return TRUE;
}

//-----------------------------------------------------------------------------
//  AUD_DeRegNotifyFunc
//
/** Brief of AUD_DeRegNotifyFunc.
 *  Details of AUD_DeRegNotifyFunc (optional).
 *
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
BOOL _AUD_DeRegNotifyFunc(UINT32 u4Handler)
{
    INT32 i;
    BOOL fgRet = FALSE;
    
    _AudNotifyInit();
    VERIFY(x_sema_lock(_rProtectSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    for (i=0; i<NOTIFY_FUNC_NUM; i++)
    {
        if (_arNotifyFuncInfo[i].u4Handler == u4Handler)
        {
            _arNotifyFuncInfo[i].fgRegistered = FALSE;
            _arNotifyFuncInfo[i].fgIgnoreMainDecNotify = FALSE;
            _arNotifyFuncInfo[i].fgIgnoreAuxDecNotify = FALSE;   
#if 1 //def CC_MT5391_AUD_3_DECODER
            _arNotifyFuncInfo[i].fgIgnoreThirdDecNotify = FALSE;
#endif
            _arNotifyFuncInfo[i].pfNotifyFunc = NULL;
            fgRet = TRUE;
        }   
    }

    VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
    return fgRet;
}

//-----------------------------------------------------------------------------
//  AUD_RegNotifyFunc
//
/** Brief of AUD_RegNotifyFunc.
 *  Details of AUD_RegNotifyFunc (optional).
 *
 *  @retval TRUE    Success
 *  @retval FALSE   Fail
 */
//-----------------------------------------------------------------------------
BOOL _AUD_RegNotifyFunc(UINT32 * pu4Handler, AudDecNfyFct pfNfyFunc)
{
    UINT8 u1Indx = 0;
    
    _AudNotifyInit();
    VERIFY(x_sema_lock(_rProtectSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    // Check if list is full or parameters invalid
    if (_fgIsListFull() || (pu4Handler == NULL) || (pfNfyFunc == NULL))
    {
        VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
        return FALSE;
    }
    else 
    {   // Find free entry and save registry information
        if (_GetFreeList(&u1Indx))
        {
            VERIFY(u1Indx < NOTIFY_FUNC_NUM);
            if (u1Indx < NOTIFY_FUNC_NUM)
            {
                _arNotifyFuncInfo[u1Indx].fgRegistered = TRUE;
                _arNotifyFuncInfo[u1Indx].fgIgnoreMainDecNotify = FALSE;
                _arNotifyFuncInfo[u1Indx].fgIgnoreAuxDecNotify = FALSE;  
#if 1 //def CC_MT5391_AUD_3_DECODER
                _arNotifyFuncInfo[u1Indx].fgIgnoreThirdDecNotify = FALSE;
#endif                
                _arNotifyFuncInfo[u1Indx].pfNotifyFunc = pfNfyFunc;    
                *pu4Handler = _arNotifyFuncInfo[u1Indx].u4Handler;
            }
        }
    }    
    
    VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
    return TRUE;
}

#ifdef CC_AUD_SETUP_MELODY
void _AUD_IgnoreSetupPlayNotify(BOOL fgIgnore)
{
    fgIgnoreSetupPlayNotify = fgIgnore;
}
#endif

BOOL _AUD_IgnoreDecNotify(UINT8 u1DecId, BOOL fgIgnore)
{
    INT32 i;
    _AudNotifyInit();
    VERIFY(x_sema_lock(_rProtectSema, X_SEMA_OPTION_WAIT) == OSR_OK);

    for (i=0; i<NOTIFY_FUNC_NUM; i++)
    {
        if (_arNotifyFuncInfo[i].fgRegistered)
        {
            if (u1DecId == AUD_DEC_MAIN)
            {
                _arNotifyFuncInfo[i].fgIgnoreMainDecNotify = fgIgnore;
            }
            else if (u1DecId == AUD_DEC_AUX)
            {
                _arNotifyFuncInfo[i].fgIgnoreAuxDecNotify = fgIgnore;           
            }   
#if 1 //def CC_MT5391_AUD_3_DECODER
            if (u1DecId == AUD_DEC_THIRD)
            {
                _arNotifyFuncInfo[i].fgIgnoreThirdDecNotify = fgIgnore;
            }
#endif
        }   
    }

    VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
    return TRUE;
}

void AUD_NotifySuspend(void)
{
    if (_rProtectSema != NULL_HANDLE)
    {
        VERIFY(x_sema_unlock(_rProtectSema) == OSR_OK);
    }
}

