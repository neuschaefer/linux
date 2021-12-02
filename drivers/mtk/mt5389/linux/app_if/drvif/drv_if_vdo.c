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
 * $RCSfile: drv_if_vdo.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo.c
 *  Driver interface: VDO part implementation
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_assert.h"
#include "x_os.h"
LINT_EXT_HEADER_END
#include "x_drv_if.h"
#include "vdo_common.h"
#include "vdo_ana.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static HANDLE_T _hVdoMutex;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define DRV_VDO_MUTEX_LOCK      x_sema_lock(_hVdoMutex, X_SEMA_OPTION_WAIT);
#define DRV_VDO_MUTEX_UNLOCK    x_sema_unlock(_hVdoMutex);
#define DRV_VDO_MUTEX_INIT      x_sema_create(&_hVdoMutex, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Exported functionss
//-----------------------------------------------------------------------------

DRV_ERR_CODE_T DRV_VDO_Init()
{
    DRV_ERR_CODE_T rRetCode;    

    DRV_VDO_MUTEX_INIT
 
    DRV_VDO_MUTEX_LOCK
        
    rRetCode = _DRV_VDO_Init();

    DRV_VDO_MUTEX_UNLOCK
    
    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_SetParameter(UINT8 u1VideoIndex,
    DRV_VDO_PARAM_T* prParam)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
        
    rRetCode = _DRV_VDO_SetParameter(u1VideoIndex, prParam);

    DRV_VDO_MUTEX_UNLOCK
    
    return rRetCode;
}

DRV_ERR_CODE_T DRV_VDO_Start(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Start(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK
    
    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_Stop(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Stop(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;
}
DRV_ERR_CODE_T DRV_VDO_Freeze(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Freeze(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_Resume(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Resume(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;        
}

DRV_ERR_CODE_T DRV_VDO_Mute(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Mute(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_Unmute(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_Unmute(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;        
}

DRV_ERR_CODE_T DRV_VDO_SetOutputWindow(UINT8 u1VideoIndex,
    DRV_VDO_WINDOW_T* prWindow)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetOutputWindow(u1VideoIndex, prWindow);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;        
}

DRV_ERR_CODE_T DRV_VDO_SetOverScanRatio(UINT8 u1VideoIndex,
    UINT32 u4X, UINT32 u4Y)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetOverScanRatio(u1VideoIndex, u4X, u4Y);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;        
}

DRV_ERR_CODE_T DRV_VDO_SetQuality(UINT8 u1VideoIndex, 
	UINT32 u4Mask, DRV_VDO_QUALITY_T* prQuality)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetQuality(u1VideoIndex, u4Mask, prQuality);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;       
}

DRV_ERR_CODE_T DRV_VDO_SetScart(UINT32 u4Mask, DRV_VDO_SCART_PARAM_T* prParam)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetScart(u4Mask, prParam);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;
}

DRV_ERR_CODE_T DRV_VDO_SetVga(UINT32 u4Mask, DRV_VDO_VGA_PARAM_T* prParam)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetVga(u4Mask, prParam);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;
}

DRV_ERR_CODE_T DRV_VDO_SetDispFmt(UINT8 u1VideoIndex, DRV_VDO_DISP_FMT_T rFmt)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
        
    rRetCode = _DRV_VDO_SetDispFmt(u1VideoIndex, rFmt);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_SetPtsNfy(UINT8 u1VideoIndex, UINT64 u8Pts)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_SetPtsNfy(u1VideoIndex, u8Pts);

    DRV_VDO_MUTEX_UNLOCK
    
    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_Auto(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
        
    rRetCode = _DRV_VDO_Auto(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_Autocolor(UINT8 u1VideoIndex)
{
    DRV_ERR_CODE_T rRetCode;    
 
    DRV_VDO_MUTEX_LOCK
        
    rRetCode = _DRV_VDO_Autocolor(u1VideoIndex);

    DRV_VDO_MUTEX_UNLOCK

    return rRetCode;    
}

DRV_ERR_CODE_T DRV_VDO_GetInfo(UINT8 u1VideoIndex,
    DRV_VDO_INFO_T* prInfo)
{
    return _DRV_VDO_GetInfo(u1VideoIndex, prInfo);
}

DRV_ERR_CODE_T DRV_VDO_GetVbiTtx(UINT8 u1VideoIndex, 
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag)
{    	
    return _DRV_VDO_GetVbiTtx(u1VideoIndex, prVbiTtxData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_UnlockVbiTtx(UINT8 u1VideoIndex, 
	DRV_VDO_VBITTX_DATA_T* prVbiTtxData, INT32 i4Tag)
{
    return _DRV_VDO_UnlockVbiTtx(u1VideoIndex, prVbiTtxData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetVbiCc(UINT8 u1VideoIndex, 
	DRV_VDO_VBICC_DATA_T* prVbiCcData, INT32 i4Tag)
{    	
    return _DRV_VDO_GetVbiCc(u1VideoIndex, prVbiCcData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetVbiVps(UINT8 u1VideoIndex, 
	DRV_VDO_VBIVPS_DATA_T* prVbiVpsData, INT32 i4Tag)
{
    return _DRV_VDO_GetVbiCc(u1VideoIndex, prVbiVpsData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetVbiWss(UINT8 u1VideoIndex, 
	DRV_VDO_VBIWSS_DATA_T* prVbiWssData, INT32 i4Tag)
{
    return _DRV_VDO_GetVbiCc(u1VideoIndex, prVbiWssData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetMpegCc(UINT8 u1VideoIndex, 
	DRV_VDO_MPEGCC_DATA_T* prMpegCcData, INT32 i4Tag)
{
    return _DRV_VDO_GetMpegCc(u1VideoIndex, prMpegCcData, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetScartStatus(UINT8 u1VideoIndex, 
	DRV_VDO_SCART_STATUS_T* prScartStatus)
{
    return _DRV_VDO_GetScartStatus(u1VideoIndex, prScartStatus);
}

DRV_ERR_CODE_T DRV_VDO_GetScartFs (UINT8 u1VideoIndex, 
    DRV_VDO_SCART_FS_T* prScartFs, INT32 i4Tag)
{
    return _DRV_VDO_GetScartFs(u1VideoIndex, prScartFs, i4Tag);
}

DRV_ERR_CODE_T DRV_VDO_GetMessage(INT32 i4Tag)
{
    DRV_ERR_CODE_T rRetCode;    
 
    //DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_GetMessage(i4Tag);

    //DRV_VDO_MUTEX_UNLOCK

    return rRetCode;
}

DRV_ERR_CODE_T DRV_VDO_GetStc(UINT8 u1VideoIndex, UINT64* pu8Stc)
{
    DRV_ERR_CODE_T rRetCode;    
 
    //DRV_VDO_MUTEX_LOCK
    	
    rRetCode = _DRV_VDO_GetStc(u1VideoIndex, pu8Stc);

    //DRV_VDO_MUTEX_UNLOCK

    return rRetCode;
}

