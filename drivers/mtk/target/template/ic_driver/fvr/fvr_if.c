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
static FVR_STATE_T _eFVRState = FVR_STATE_STOP;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** FVR_Diag
 *  @retval INT32
 */
//-----------------------------------------------------------------------------
BOOL FVR_Diag()
{
    return _FVR_Diag();
}


//-----------------------------------------------------------------------------
/** FVR_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Init()
{
    _eFVRState = FVR_STATE_STOP;

    return _FVR_Init();
}


//-----------------------------------------------------------------------------
/** FVR_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Start()
{
    if(_eFVRState == FVR_STATE_RECORDING)
    {
        LOG(5, "Already recording...\n");
        return TRUE;
    }

    if(!_FVR_Start())
    {
        return FALSE;
    }

    _eFVRState = FVR_STATE_RECORDING;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** FVR_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_Stop()
{
    if(_eFVRState == FVR_STATE_STOP)    
    {
        LOG(5, "Already stop recording...\n");
        return TRUE;
    }

    if(!_FVR_Stop())
    {
        return FALSE;
    }

    _eFVRState = FVR_STATE_STOP;

    return TRUE;    
}


//-----------------------------------------------------------------------------
/** FVR_GetState
 *  @retval FVR_STATE_T
 */
//-----------------------------------------------------------------------------
FVR_STATE_T FVR_GetState(void)
{
    return _eFVRState;
}


//-----------------------------------------------------------------------------
/** FVR_SetGBL
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetGBL(UINT32 u4Flags, const FVR_GBL_T* prGBL)
{
    return _FVR_SetGBL(u4Flags, prGBL);
}


//-----------------------------------------------------------------------------
/** FVR_GetGBL
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetGBL(UINT32 u4Flags, FVR_GBL_T* prGBL)
{  
    return _FVR_GetGBL(u4Flags, prGBL);
}


//-----------------------------------------------------------------------------
/** FVR_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVR_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const FVR_PID_T* prPid)
{    
    return _FVR_SetPid(u1Pidx, u4Flags, prPid);
}


//-----------------------------------------------------------------------------
/** FVR_GetPid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetPid(UINT8 u1Pidx, UINT32 u4Flags, FVR_PID_T* prPid)
{
    return _FVR_GetPid(u1Pidx, u4Flags, prPid);
}


//-----------------------------------------------------------------------------
/** FVR_FreePid
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreePid(UINT8 u1Pidx)
{
    return _FVR_FreePid(u1Pidx);
}


//-----------------------------------------------------------------------------
/** FVR_SetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_SetTimestamp(UINT32 u4Flag, FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_SetTimestamp(u4Flag, prTimeStamp);
}


//-----------------------------------------------------------------------------
/** FVR_GetTimestamp
 *  @retval void
 */
//-----------------------------------------------------------------------------
void FVR_GetTimestamp(UINT32 u4Flags, FVR_TIMESTAMP_T* prTimeStamp)
{
    _FVR_GetTimestamp(u4Flags, prTimeStamp);
}


//-----------------------------------------------------------------------------
/** FVR_FlushBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FlushBuffer()
{
    return _FVR_FlushBuffer();
}


//-----------------------------------------------------------------------------
/** FVR_GetBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetBuffer(UINT32 u4Addr, UINT32 u4Size, UINT8 *pu1Buf, UINT32 *pu4RetSize)
{
    return _FVR_GetBuffer(u4Addr, u4Size, pu1Buf, pu4RetSize);
}


//-----------------------------------------------------------------------------
/** FVR_GetWritePointer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_GetWritePointer(UINT32 u4Addr, UINT32 u4Size, UINT32 *pu4RetAddr, UINT32 *pu4RetSize)
{
    return _FVR_GetWritePointer(u4Addr, u4Size, pu4RetAddr, pu4RetSize);
}


//-----------------------------------------------------------------------------
/** FVR_UnlockBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_UnlockBuffer(UINT32 u4Addr, UINT32 u4Size)
{
    return _FVR_UnlockBuffer(u4Addr, u4Size);
}


//-----------------------------------------------------------------------------
/** FVR_FreeBuffer
 *  @retval 
 */
//-----------------------------------------------------------------------------
BOOL FVR_FreeBuffer(void)
{
    return _FVR_FreeBuffer();
}


//-----------------------------------------------------------------------------
/** FVR_GetDataSize
 *  @retval 
 */
//-----------------------------------------------------------------------------
UINT32 FVR_GetDataSize(void)
{
    return _FVR_GetDataSize();
}


//-----------------------------------------------------------------------------
/** FVR_QueryStatus
 *  @retval 
 */
//-----------------------------------------------------------------------------
void FVR_QueryStatus(FVR_QUERY_INFO_T *prInfo)
{
    _FVR_QueryStatus(prInfo);
}


//-----------------------------------------------------------------------------
/** FVR_SetByPass
 *  @retval 
 */
//-----------------------------------------------------------------------------
void FVR_SetByPass(BOOL fgEnable, UINT8 u1Pidx, UINT16 u2CtrlBits)
{
    _FVR_SetByPass(fgEnable, u1Pidx, u2CtrlBits);
}

