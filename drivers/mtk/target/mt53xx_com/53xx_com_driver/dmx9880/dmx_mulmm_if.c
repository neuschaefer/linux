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
 * $RCSfile: dmx_multimm_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_multimm_if.c
 *  Demux multi-media driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END


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
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** DMX_MUL_EnableMultipleInst
 */
//-----------------------------------------------------------------------------
void DMX_MUL_EnableMultipleInst(BOOL fgEnable)
{
    _DMX_LockApi();
    _DMX_MUL_EnableMultipleInst(fgEnable);
    _DMX_UnlockApi();
}

//-----------------------------------------------------------------------------
/** DMX_MUL_GetAvailableInst
 */
//-----------------------------------------------------------------------------
UINT8 DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_T eConnType)
{
    UINT8 u1Inst;

    _DMX_LockApi();
    u1Inst = _DMX_MUL_GetAvailableInst(eConnType);
    _DMX_UnlockApi();

    return u1Inst;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_FreeInst
 */
//-----------------------------------------------------------------------------
BOOL DMX_MUL_FreeInst(UINT8 u1Inst)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_FreeInst(u1Inst);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_MUL_FreeInst
 */
//-----------------------------------------------------------------------------
UINT8 DMX_MUL_GetPidxInstId(UINT8 u1Pidx)
{
    UINT8 u1Inst;

    _DMX_LockApi();
    u1Inst = _DMX_MUL_GetPidxInstId(u1Pidx);
    _DMX_UnlockApi();

    return u1Inst;
}

//-----------------------------------------------------------------------------
/** DMX_MUL_SetInstType
 */
//-----------------------------------------------------------------------------
BOOL DMX_MUL_SetInstType(UINT8 u1Inst, DMX_INPUT_TYPE_T eInputType)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_SetInstType(u1Inst, eInputType);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_GetAvailablePidx
*/
//-----------------------------------------------------------------------------
UINT8 DMX_MUL_GetAvailablePidx(UINT8 u1Inst)
{
    UINT8 u1Pidx;

    _DMX_LockApi();
    u1Pidx = _DMX_MUL_GetAvailablePidx(u1Inst);
    _DMX_UnlockApi();

    return u1Pidx;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_SetSubtitlePidx
*/
//-----------------------------------------------------------------------------
UINT8 DMX_MUL_SetSubtitlePidx(UINT8 u1Inst, UINT8 u1Pidx)
{
    UINT8 u1Idx;

    _DMX_LockApi();
    u1Idx = _DMX_MUL_SetSubtitlePidx(u1Inst, u1Pidx);
    _DMX_UnlockApi();

    return u1Idx;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_GetSubtitlePidx
*/
//-----------------------------------------------------------------------------
UINT8 DMX_MUL_GetSubtitlePidx(UINT8 u1Inst)
{
    UINT8 u1Idx;

    _DMX_LockApi();
    u1Idx = _DMX_MUL_GetSubtitlePidx(u1Inst);
    _DMX_UnlockApi();

    return u1Idx;
}


//-----------------------------------------------------------------------------
/** _DMX_MUL_SetPidxInstance
 *  Temporary
*/
//-----------------------------------------------------------------------------
BOOL DMX_MUL_SetPidxInstance(UINT8 u1Inst, UINT8 u1Pidx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_SetPidxInstance(u1Inst, u1Pidx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_FreePidx
*/
//-----------------------------------------------------------------------------
BOOL DMX_MUL_FreePidx(UINT8 u1Pidx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_FreePidx(u1Pidx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_MoveData
 */
//-----------------------------------------------------------------------------
BOOL DMX_MUL_MoveData(UINT8 u1Inst, DMX_MM_DATA_T *prData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_MoveData(u1Inst, prData);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL DMX_MUL_RequestReset(UINT8 u1Inst)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_RequestReset(u1Inst);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_GetEmptySize
*/
//-----------------------------------------------------------------------------
UINT32 DMX_MUL_GetEmptySize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    return _DMX_MUL_GetEmptySize(u1Inst, eType, u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_GetMinFreeBufferSize
 */
//-----------------------------------------------------------------------------
UINT32 DMX_MUL_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx)
{
    return _DMX_MUL_GetMinFreeDRAMBufferSize(u1Inst, eType, u1Pidx);
}

//-----------------------------------------------------------------------------
/** DMX_MUL_SetDecoderCallbacks
 */
//-----------------------------------------------------------------------------
BOOL DMX_MUL_SetDecoderCallbacks(UINT8 u1Inst, const DMX_DECODER_CALLBACKS_T* prCallbacks)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_SetDecoderCallbacks(u1Inst, prCallbacks);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MUL_SetVideoType
 */
//-----------------------------------------------------------------------------
BOOL DMX_MUL_SetVideoType(UINT8 u1Inst, DMX_VIDEO_TYPE_T eVideoType)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_SetVideoType(u1Inst, eVideoType);
    _DMX_UnlockApi();

    return fgRet;
}


