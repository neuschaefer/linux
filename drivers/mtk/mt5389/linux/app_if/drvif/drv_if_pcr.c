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
 * $RCSfile: drv_if_pcr.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pcr.c
 *  Driver interface internal use: pcr 
 */
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
#include "dmx_if.h"
#include "stc_drvif.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "vdo_common.h"
#include "aud_common.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DRV_PCR_MUTEX_INIT      x_sema_create(&_hPcrMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);
#define DRV_PCR_MUTEX_LOCK      x_sema_lock(_hPcrMutex, X_SEMA_OPTION_WAIT);
#define DRV_PCR_MUTEX_UNLOCK    x_sema_unlock(_hPcrMutex);

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static UINT8    _u1VideoIndex;
static BOOL     _fgVideoReady;
static BOOL     _fgAudioReady;
static BOOL     _fgSync;
static UINT8    _u1PcrPidx;
static UINT16   _u2PcrPid;
static HANDLE_T _hPcrMutex;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _GetVideoReady(void)
{
    return _fgVideoReady;
}

static BOOL _GetAudioReady(void)
{
    return _fgAudioReady;    
}

static DRV_ERR_CODE_T _Sync(UINT8 u1VideoIndex)
{
#ifndef __MODEL_slt__
    DRV_VDO_PARAM_T*        prVideoParam;
    DRV_AUD_PARAM_T*        prAudioParam;
    DRV_VDO_INTERNAL_T*     prVideoInternal;
    DRV_AUD_INTERNAL_T*     prAudioInternal;
    
    VERIFY(_DRV_VDO_GetParameter(u1VideoIndex, &prVideoParam) == DRV_E_OK);
    VERIFY(_DRV_AUD_GetParameter(&prAudioParam) == DRV_E_OK);
    VERIFY(_DRV_VDO_GetInternal(u1VideoIndex, &prVideoInternal) == DRV_E_OK);
    VERIFY(_DRV_AUD_GetInternal(&prAudioInternal) == DRV_E_OK);

    if(prVideoParam->u2PcrPid != prAudioParam->u2PcrPid)
    {
        return DRV_E_FAIL;
    }
    _u2PcrPid = prVideoParam->u2PcrPid;
    if (prVideoParam->u2Pid == prVideoParam->u2PcrPid)
    {
        VERIFY(DMX_SetPcr(prVideoInternal->u1Pidx, TRUE, 0));
        VERIFY(DMX_SetStcSrc(prVideoInternal->u1Pidx, 0) == STC_SET_OK);
    }
    else if(prAudioParam->u2Pid == prAudioParam->u2PcrPid)
    {
        VERIFY(DMX_SetPcr(prAudioInternal->u1Pidx, TRUE, 0));
        VERIFY(DMX_SetStcSrc(prAudioInternal->u1Pidx, 0) == STC_SET_OK);    
    }
    // The setting if PCR PID != AUDIO PID && PCR PID != VIDEO PID
    else
    {
        DMX_PID_T rPcrPid;
        if(!_DMX_AllocatePid(&_u1PcrPidx))
        {                          
            return DRV_E_FAIL;
        }
        rPcrPid.fgEnable =      TRUE;
        rPcrPid.u1TsIndex =     0;
        rPcrPid.u2Pid =         _u2PcrPid;
        rPcrPid.ePcrMode =      DMX_PCR_MODE_NEW;
        rPcrPid.ePidType =      DMX_PID_TYPE_NONE;
        VERIFY(DMX_SetPid(_u1PcrPidx, (DMX_PID_FLAG_VALID | DMX_PID_FLAG_TS_INDEX | DMX_PID_FLAG_PID | DMX_PID_FLAG_PCR), &rPcrPid));
        VERIFY(DMX_SetPcr(_u1PcrPidx, TRUE, 0));
        VERIFY(DMX_SetStcSrc(_u1PcrPidx, 0) == STC_SET_OK);
    }    
    return DRV_E_OK;
#endif    
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
DRV_ERR_CODE_T _DRV_PCR_TrySyncVideo(UINT8 u1VideoIndex)
{
    DRV_PCR_MUTEX_LOCK
        
    _u1VideoIndex = u1VideoIndex;
    if(!_fgSync && _GetAudioReady())
    {
        _Sync(u1VideoIndex);
        _fgSync = TRUE;         
    }
    _fgVideoReady = TRUE;

    DRV_PCR_MUTEX_UNLOCK
        
    return DRV_E_OK;        
}

DRV_ERR_CODE_T _DRV_PCR_TrySyncAudio(void)
{
    DRV_PCR_MUTEX_LOCK
    
    if(!_fgSync && _GetVideoReady())
    {
        _Sync(_u1VideoIndex);
        _fgSync = TRUE;
    }
    _fgAudioReady = TRUE;

    DRV_PCR_MUTEX_UNLOCK    

    return DRV_E_OK;
}

DRV_ERR_CODE_T _DRV_PCR_Unsync(void)
{
    DMX_PID_T               rPid;
    DRV_VDO_PARAM_T*        prVideoParam;
    DRV_AUD_PARAM_T*        prAudioParam;
    DRV_VDO_INTERNAL_T*     prVideoInternal;
    DRV_AUD_INTERNAL_T*     prAudioInternal;

    DRV_PCR_MUTEX_LOCK

    if(!_fgSync)
    {
        _fgVideoReady =     FALSE;
        _fgAudioReady =     FALSE;

        DRV_PCR_MUTEX_UNLOCK
        
        return DRV_E_OK;
    }
    
#ifndef __MODEL_slt__          
    _DRV_VDO_GetParameter(_u1VideoIndex, &prVideoParam);
    _DRV_AUD_GetParameter(&prAudioParam);
    _DRV_VDO_GetInternal(_u1VideoIndex, &prVideoInternal);
    _DRV_AUD_GetInternal(&prAudioInternal);    
        
    if (prVideoParam->u2PcrPid == prVideoParam->u2Pid)
    {
        VERIFY(DMX_SetStcSrc(prVideoInternal->u1Pidx, 2) == STC_SET_OK);
    }
    else if(prAudioParam->u2PcrPid == prAudioParam->u2Pid)
    {
        VERIFY(DMX_SetStcSrc(prAudioInternal->u1Pidx, 2) == STC_SET_OK);
    }
    else if(prAudioInternal->u1State == DRV_AUD_STATE_PLAY && prAudioParam->u2Pid != prAudioParam->u2PcrPid)
    {
        rPid.fgEnable = FALSE;
        VERIFY(DMX_SetPid(_u1PcrPidx, DMX_PID_FLAG_VALID, &rPid));
        VERIFY(DMX_SetStcSrc(_u1PcrPidx, 2) == STC_SET_OK);
        VERIFY(DMX_FreePid(_u1PcrPidx));
        _DMX_ReleasePid(_u1PcrPidx);        
    }    
#endif    
    
    _fgVideoReady = FALSE;
    _fgAudioReady = FALSE;
    _fgSync =       FALSE;

    DRV_PCR_MUTEX_UNLOCK
        
    return DRV_E_OK;        
}

DRV_ERR_CODE_T DRV_PCR_Init(void)
{
    DRV_PCR_MUTEX_INIT
        
    return DRV_E_OK;
}


