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
 * $RCSfile: fvr_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_fvr_if.c
 *  Demux driver - FVR Driver Interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fvr_debug.h"
#include "fvr.h"

#include "x_assert.h"

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
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter FVR API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_ApiLock(void)
{
    _DMX_LockApi();
}


//-----------------------------------------------------------------------------
/** Leave FVR API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void _FVR_ApiUnlock(void)
{
    _DMX_UnlockApi();
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Init()
{
    BOOL fgRet;
    
    _FVR_ApiLock();
    fgRet = _FVR_Init(); 
    _FVR_ApiUnlock();
    
    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Start(void)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_Start();
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Stop(void)
{
    BOOL fgRet = TRUE;

    // May call in HISR
    //_FVR_ApiLock();
    if(_FVR_IsRecording())
    {
        fgRet = _FVR_Stop();
    }
    //_FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetResidual
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetResidual(UINT32 *pu4Addr, UINT32 *pu4Size, UINT32 *pu4Index)
{
    BOOL fgRet;
    
    if((pu4Addr == NULL) || (pu4Size == NULL) || (pu4Index == NULL))
    {
        return FALSE;
    }

    _FVR_ApiLock();
    fgRet = _FVR_GetResidual(pu4Addr, pu4Size, pu4Index);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetScrambleScheme
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_SetScrambleScheme(BOOL fgOrg)
{
    _FVR_SetScrambleScheme(fgOrg);
}


//-----------------------------------------------------------------------------
/** FVR_IsRecording
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_IsRecording(void)
{
    BOOL fgRet;

    //_FVR_ApiLock();
    fgRet = _FVR_IsRecording();
    //_FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetGBL(u4Flags, prGBL);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetGBL
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetGBL(UINT32 u4Flags, FVR_GBL_T* prGBL)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_GetGBL(u4Flags, prGBL);
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_FreeGBL
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreeGBL(void)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreeGBL();
    _FVR_ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** FVR_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{    
    BOOL fgRet;

    _FVR_ApiLock();

    fgRet = _FVR_SetPid(u1Pidx, u4Flags, prPid);
    
    _FVR_ApiUnlock();

    return fgRet;    
}


//-----------------------------------------------------------------------------
/** FVR_GetPid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T* prPid)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_GetPid(u1Pidx, u4Flags, prPid);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_FreePid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreePid(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreePid(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_SetTimestamp(UINT32 u4Flag, const FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_ApiLock();
    _FVR_SetTimestamp(u4Flag, prTimeStamp);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_GetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_ApiLock();
    _FVR_GetTimestamp(u4Flags, prTimeStamp);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_FlushBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FlushBuffer(void)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FlushBuffer();
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetBuffer(UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetSize)
{
    BOOL fgRet;

    fgRet = _FVR_GetBuffer(u4Addr, u4Size, pu1Buf, pu4RetSize);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_UnlockBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_UnlockBuffer(UINT32 u4Addr, UINT32 u4Size)
{
    BOOL fgRet;

    fgRet = _FVR_UnlockBuffer(u4Addr, u4Size);

    return fgRet;    
}


//-----------------------------------------------------------------------------
/** FVR_FreeBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreeBuffer(void)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_FreeBuffer();
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetIsOverflow
 *  @retval 
 */
//-----------------------------------------------------------------------------
void FVR_SetIsOverflow(BOOL fgIsOverflow)
{
    _FVR_SetIsOverflow(fgIsOverflow);
}


//-----------------------------------------------------------------------------
/** FVR_QueryStatus
 *  @retval 
 */
//-----------------------------------------------------------------------------
void FVR_QueryStatus(FVR_QUERY_INFO_T *prInfo)
{
    _FVR_ApiLock();
    _FVR_QueryStatus(prInfo);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_SetByPass
 *  @retval 
 */
//-----------------------------------------------------------------------------
void FVR_SetByPass(UINT8 u1TsIndex, UINT32 u4Val, BOOL fgEnable)
{
    _FVR_ApiLock();
    _FVR_SetByPass(u1TsIndex, u4Val, fgEnable);
    _FVR_ApiUnlock();
}


//-----------------------------------------------------------------------------
/** FVR_SwapSetPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapSetPattern(UINT8 u1Pidx, UINT8* pu1Pattern, UINT16 u2Len)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapSetPattern(u1Pidx, pu1Pattern, u2Len);
    _FVR_ApiUnlock();

    return fgRet;    
}


//-----------------------------------------------------------------------------
/** FVR_SwapSetNullPattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapSetNullPattern(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapSetNullPattern(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SwapFreePattern
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SwapFreePattern(UINT8 u1Pidx)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SwapFreePattern(u1Pidx);
    _FVR_ApiUnlock();

    return fgRet;    
}


//-----------------------------------------------------------------------------
/** FVR_SetVideoType
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetVideoType(FVR_VIDEO_TYPE_T eVideoType)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetVideoType(eVideoType);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_GetVideoType
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
FVR_VIDEO_TYPE_T FVR_GetVideoType(void)
{
    FVR_VIDEO_TYPE_T eVideoType;

    //_FVR_ApiLock();
    eVideoType = _FVR_GetVideoType();
    //_FVR_ApiUnlock();

    return eVideoType;    
}


//-----------------------------------------------------------------------------
/** FVR_SetAesIV
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetAesIV(UINT32 *pu4IV)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetAesIV(pu4IV);
    _FVR_ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** FVR_SetAesKey
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetAesKey(UINT8 u1KeyIdx, BOOL fgEven, UINT8 *pu1Key)
{
    BOOL fgRet;

    _FVR_ApiLock();
    fgRet = _FVR_SetAesKey(u1KeyIdx, fgEven, pu1Key);
    _FVR_ApiUnlock();

    return fgRet;
}

