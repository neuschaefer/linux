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
 * $RCSfile: dmx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_if.c
 *  Demux driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "dmx.h"
#include "dmx_mm.h"
#include "dmx_pcr.h"
#include "dmx_debug.h"

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_os.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

// Debug
#ifdef CC_MINI_DRIVER
#undef CC_MINI_DRIVER
#endif


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

/// API semaphore
static HANDLE_T _hApiSem = NULL_HANDLE;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Enter demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiLock(void)
{
    static BOOL _fgInit = FALSE;

    if (!_fgInit)
    {
        VERIFY(x_sema_create(&_hApiSem, X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) == OSR_OK);
        _fgInit = TRUE;
    }

    VERIFY(x_sema_lock(_hApiSem, X_SEMA_OPTION_WAIT) == OSR_OK);
}


//-----------------------------------------------------------------------------
/** Leave demux API critical section
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _ApiUnlock(void)
{
    VERIFY(x_sema_unlock(_hApiSem) == OSR_OK);
}



//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void _DMX_LockApi(void)
{
    _ApiLock();
}


void _DMX_UnlockApi(void)
{
    _ApiUnlock();
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//
// Initialization
//


//-----------------------------------------------------------------------------
/** DMX_Init
 *  Initialize demux driver
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Init(void)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_Init();
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetIgnorePESLen
 */
//-----------------------------------------------------------------------------
VOID DMX_SetIgnorePESLen(BOOL fgEnable)
{
    _DMX_SetIgnorePESLen(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_SetScrambleScheme
 */
//-----------------------------------------------------------------------------
VOID DMX_SetScrambleScheme(BOOL fgOrg)
{
    _DMX_SetScrambleScheme(fgOrg);
}


//-----------------------------------------------------------------------------
/** _DMX_GetScrambleScheme
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetScrambleScheme(void)
{
    return _DMX_GetScrambleScheme();
}


//-----------------------------------------------------------------------------
/** DMX_EnablePower
 */
//-----------------------------------------------------------------------------
VOID DMX_EnablePower(BOOL fgEnable)
{
    _DMX_EnablePower(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_SelectInputType
 */
//-----------------------------------------------------------------------------
BOOL DMX_SelectInputType(DMX_INPUT_TYPE_T rInputType)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_SelectInputType(rInputType);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_GetInputType
 */
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T DMX_GetInputType(void)
{
    DMX_INPUT_TYPE_T eType;

    _ApiLock();
    eType = _DMX_GetInputType();
    _ApiUnlock();

    return eType;
}


//-----------------------------------------------------------------------------
/** DMX_Reset
 *  Reset demux driver
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Reset(void)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_Reset();
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetFrontEnd
 *  Select front end
 *
 *  @param  eFEType         Front end type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetFrontEnd(DMX_FRONTEND_T eFEType)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_SetFrontEndEx(0, eFEType);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_GetFrontEnd
 *  Return front end type
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T DMX_GetFrontEnd(void)
{
    return _DMX_GetFrontEnd(0);
}


//-----------------------------------------------------------------------------
/** DMX_GetFrontEnd
 *  Return front end type
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetFrontEndEx(UINT8 u1TsIdx, DMX_FRONTEND_T eInput)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_SetFrontEndEx(u1TsIdx, eInput);
    _ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_GetFrontEndEx
 *  Return front end type
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T DMX_GetFrontEndEx(UINT8 u1TsIdex)
{
    return _DMX_GetFrontEnd(u1TsIdex);
}

//-----------------------------------------------------------------------------
/** _DmxCustFrontEnd

 *	@retval TRUE			Succeed
 *	@retval FALSE			Fail

 */
//-----------------------------------------------------------------------------
BOOL DMX_CustFrontEnd(void)
{
	return _DmxCustFrontEnd();

}

//-----------------------------------------------------------------------------
/** DMX_ResetFramer
 */
//-----------------------------------------------------------------------------
void DMX_ResetFramer(void)
{
    _DMX_ResetFramer(_DMX_GetFramerIndex());
}


//-----------------------------------------------------------------------------
/** DMX_GetFramerIndex
 *  Get used framer index
 */
//-----------------------------------------------------------------------------
UINT8 DMX_GetFramerIndex(void)
{
    UINT8 u1TsIdx;

    _ApiLock();
    u1TsIdx = _DMX_GetFramerIndex();
    _ApiUnlock();

    return u1TsIdx;
}


//-----------------------------------------------------------------------------
/** DMX_Start
 *  Start demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Start(void)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_Start();
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_Stop
 *  Stop demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Stop(void)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_Stop();
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_Version
 *  Get DMX information
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
VOID DMX_Version(void)
{
    _DMX_Version();
}


//
// PES/PSI operations
//

//-----------------------------------------------------------------------------
/** DMX_SetPid
 *  Set a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid)
{
    BOOL fgRet;
    DMX_PID_T rPid;

    FUNC_ENTRY;

    // Transparent to Demo program/MW
    rPid = *prPid;
    rPid.eInputType = DMX_IN_BROADCAST_TS;
    u4Flags |= (DMX_PID_FLAG_INPUTTYPE);

    _ApiLock();
    fgRet = _DMX_SetPid(u1Pidx, u4Flags, &rPid, FALSE);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetPidEx
 *  Set a PID
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPidEx(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid, DMX_SOURCE_T eSource)
{
    BOOL fgRet;
    DMX_PID_T rPid;
    DMX_INPUT_TYPE_T eInputType;

    FUNC_ENTRY;

    if((eSource == DMX_SOURCE_DTV0) || (eSource == DMX_SOURCE_DTV1))
    {
        eInputType = DMX_IN_BROADCAST_TS;
    }
    else if((eSource == DMX_SOURCE_TSFILE0) || (eSource == DMX_SOURCE_TSFILE1))
    {
        eInputType = DMX_IN_PLAYBACK_TS;
    }
    else
    {
        return FALSE;
    }

    // Transparent to Demo program/MW
    rPid = *prPid;
    rPid.eInputType = eInputType;
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        rPid.u1TsIndex = _DMX_MUL_GetTsIdx(_DMX_MUL_GetPidxInstId(u1Pidx));
    }
    else
    {
        rPid.u1TsIndex = 0;
    }
    u4Flags |= (DMX_PID_FLAG_INPUTTYPE | DMX_PID_FLAG_TS_INDEX);

    _ApiLock();
    fgRet = _DMX_SetPid(u1Pidx, u4Flags, &rPid, FALSE);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetFilePid
 *  Set a PID for TS file playback
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetFilePid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPid)
{
    BOOL fgRet;
    DMX_PID_T rPid;

    FUNC_ENTRY;

    // Transparent to Demo program/MW
    rPid = *prPid;
    rPid.eInputType = DMX_IN_PLAYBACK_TS;
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        rPid.u1TsIndex = _DMX_MUL_GetTsIdx(_DMX_MUL_GetPidxInstId(u1Pidx));
    }
    else
    {
        rPid.u1TsIndex = 0;
    }
    u4Flags |= (DMX_PID_FLAG_INPUTTYPE | DMX_PID_FLAG_TS_INDEX);

    _ApiLock();
    fgRet = _DMX_SetPid(u1Pidx, u4Flags, &rPid, FALSE);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_TS_SetAudioType
 *
 *  @param  u1Pidx          PID index
 *  @param  eAudioType      Audio type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_TS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_TS_SetAudioType(u1Pidx, eAudioType);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** _DMX_TS_CPU_GetAudioType
 *
 *  @param  u1Pidx              PID index
 *
 *  @retval eAudioType          Audio type
 */
//-----------------------------------------------------------------------------
DMX_AUDIO_TYPE_T DMX_TS_GetAudioType(UINT8 u1Pidx)
{
    DMX_AUDIO_TYPE_T eAudioType;

    _ApiLock();
    eAudioType = _DMX_TS_GetAudioType(u1Pidx);
    _ApiUnlock();

    return eAudioType;
}


//-----------------------------------------------------------------------------
/** DMX_GetPid
 *  Get a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_GetPid(u1Pidx, u4Flags, prPid);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_CLIGetPid
 *  Get a PID, no API lock
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_CLIGetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid)
{
    return _DMX_GetPid(u1Pidx, u4Flags, prPid);
}


//-----------------------------------------------------------------------------
/** DMX_FreePid
 *  Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_FreePid(UINT8 u1Pidx)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_FreePid(u1Pidx);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_GetPidIndex
 *  Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param  pu1PidIndex [out]
 *                          The PID index if found
 *
 *  @retval TRUE            Found
 *  @retval FALSE           Not found
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_GetPidIndex(u2Pid, pu1PidIndex);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_FlushPidBuffer(UINT8 u1Pidx)
 *  Flush PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_FlushPidBuffer(UINT8 u1Pidx)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_FlushPidBuffer(u1Pidx);
    _ApiUnlock();

    return fgRet;
}


//
// PCR
//

//-----------------------------------------------------------------------------
/** DMX_SetPcr
 *  Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 *  @param  fgNewMode       Use the new PCR mode or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPcr(UINT8 u1PidIndex, BOOL fgEnable, BOOL fgNewMode)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_SetPcr(u1PidIndex, fgEnable, fgNewMode);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//
// PSI
//

//-----------------------------------------------------------------------------
/** DMX_SetFilter
 *  Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    const DMX_FILTER_T* prFilter)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_SetFilter(u1FilterIndex, u1Bank, u4Flags, prFilter);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                               const DMX_FILTER_GENERIC_T* prFilter)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_SetGenFilter(u1FilterIndex, u4Flags, prFilter);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_GetFilter
 *  Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    DMX_FILTER_T* prFilter)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_GetFilter(u1FilterIndex, u1Bank, u4Flags, prFilter);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_GetGenFilter
 *  Get a generic section filter setting
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                DMX_FILTER_GENERIC_T* prFilter)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_GetGenFilter(u1FilterIndex, u4Flags, prFilter);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetVideoType
 *  Set video type
 *
 *  @param  eVDec           VDec type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetVideoType(DMX_VIDEO_TYPE_T eVDec)
{
    BOOL fgRet;
    UINT8 i;

    _ApiLock();
    fgRet = TRUE;
    for(i=0; i<DMX_FRAMER_COUNT; i++)
    {
        if(!_DMX_SetVideoType(i, eVDec))
        {
            fgRet = FALSE;
            break;
        }
    }
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetVideoTypeByPidx
 *  Set video type by Pidx
 *
 *  @param  u1Pidx          PID index
 *  @param  eVideoType      VDec type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetVideoTypeByPidx(UINT8 u1Pidx, DMX_VIDEO_TYPE_T eVideoType)
{
    BOOL fgRet;
    _ApiLock();
    fgRet = _DMX_SetVideoTypeByPidx(u1Pidx, eVideoType);
    _ApiUnlock();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_PSI_AllocateBuffer
 *  Allocate section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PSI_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_PSI_AllocateBuffer(u1Pidx, u4Size);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PSI_FreeBuffer
 *  Free a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PSI_FreeBuffer(UINT8 u1Pidx)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_PSI_FreeBuffer(u1Pidx);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PSI_UnlockBuffer
 *  Unlock a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    return _DMX_PSI_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** DMX_PSI_FlushBuffer
 *  Flush a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PSI_FlushBuffer(UINT8 u1Pidx)
{
    return _DMX_PSI_FlushBuffer(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_PSI_GetBuffer
 *  Copy data from a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    return _DMX_PSI_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr, u4SkipSize,
        u4CopySize, pu1Dest);
}


#ifdef DMX_MEASURE_PSI_TIME
//-----------------------------------------------------------------------------
/** DMX_PSI_GetMaxTime
 */
//-----------------------------------------------------------------------------
VOID DMX_PSI_GetMaxTime(HAL_TIME_T* prTime)
{
    ASSERT(prTime != NULL);

    _DMX_PSI_GetMaxTime(prTime);
}


//-----------------------------------------------------------------------------
/** DMX_ResetPSIMaxTime
 */
//-----------------------------------------------------------------------------
VOID DMX_ResetPSIMaxTime(void)
{
    _DMX_PSI_ResetMaxTime();
}
#endif  // DMX_MEASURE_PSI_TIME

//-----------------------------------------------------------------------------
/** DMX_PES_AllocateBuffer
 *  Allocate PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PES_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_PES_AllocateBuffer(u1Pidx, u4Size);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PES_FreeBuffer
 *  Free a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PES_FreeBuffer(UINT8 u1Pidx)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_PES_FreeBuffer(u1Pidx);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PES_UnlockBuffer
 *  Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PES_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    return _DMX_PES_UnlockBuffer(u1Pidx, u1SerialNumber, u4Size, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** DMX_PES_FlushBuffer
 *  Flush a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PES_FlushBuffer(UINT8 u1Pidx)
{
    return _DMX_PES_FlushBuffer(u1Pidx);
}


//-----------------------------------------------------------------------------
/** _DMX_PES_GetBuffer
 *  Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param  pu1Dest         Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_PES_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    return _DMX_PES_GetBuffer(u1Pidx, u1SerialNumber, u4FrameAddr, u4SkipSize,
        u4CopySize, pu1Dest);
}


// Some functions exist only in non-mini driver
//
#ifndef CC_MINI_DRIVER


//
// Descrambler
//

//-----------------------------------------------------------------------------
/** DMX_SetDesKey
 *  Set a desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6])
{
    return _DMX_SetDesKey(u1Index, fg3Des, fgEven, au4Key);
}


//-----------------------------------------------------------------------------
/** DMX_SetDesIV
 *  Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo)
{
    _DMX_SetDesIVEx2(0, u4IvHi, u4IvLo, TRUE);
    _DMX_SetDesIVEx2(0, u4IvHi, u4IvLo, FALSE);
}

void DMX_SetDesIVEx(UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven)
{
    _DMX_SetDesIVEx2(0, u4IvHi, u4IvLo, fgEven);
}

void DMX_SetDesIVEx2(UINT8 u1Idx, UINT32 u4IvHi, UINT32 u4IvLo, BOOL fgEven)
{
    _DMX_SetDesIVEx2(u1Idx, u4IvHi, u4IvLo, fgEven);
}

//-----------------------------------------------------------------------------
/** DMX_SetDesRtbMode
 *  Set residual termination mode of DES
 *
 *  @param  DMX_DESC_RTB_MODE_T          CLEAR/CTS/SCTE-52
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
void DMX_SetDesRtbMode(DMX_DESC_RTB_MODE_T eMode)
{
    _DMX_SetDesRtbMode(0, eMode);
}


//-----------------------------------------------------------------------------
/** DMX_SetAesKeyLen
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetAesKeyLen(UINT32 u4KeyLen)
{
    return _DMX_SetAesKeyLen(0, u4KeyLen);
}


//-----------------------------------------------------------------------------
/** DMX_SetAesKey
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8])
{
    return _DMX_SetAesKey(u1Index, fgEven, au4Key);
}


//-----------------------------------------------------------------------------
/** DMX_SetAesIV
 */
//-----------------------------------------------------------------------------
void DMX_SetAesIV(const UINT32 au4Iv[4])
{
    _DMX_SetAesIVEx2(0, au4Iv, TRUE);
    _DMX_SetAesIVEx2(0, au4Iv, FALSE);
}

void DMX_SetAesIVEx(const UINT32 au4Iv[4], BOOL fgEven)
{
    _DMX_SetAesIVEx2(0, au4Iv, fgEven);
}

void DMX_SetAesIVEx2(UINT8 u1Idx, const UINT32 au4Iv[4], BOOL fgEven)
{
    _DMX_SetAesIVEx2(u1Idx, au4Iv, fgEven);
}


//-----------------------------------------------------------------------------
/** DMX_SetAesRtbMode
 *  Set residual termination mode of AES
 *
 *  @param  DMX_DESC_RTB_MODE_T          CLEAR/CTS/SCTE-52
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
void DMX_SetAesRtbMode(DMX_DESC_RTB_MODE_T eMode)
{
    _DMX_SetAesRtbMode(0, eMode);
}

//#ifdef CC_VUDU_SUPPORT
BOOL DMX_VUDU_SetKey(DMX_VUDU_KEY_T *prVuduKey)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _ApiLock();
    fgRet = _DMX_VUDU_SetKey(prVuduKey);
    _ApiUnlock();

    FUNC_EXIT;

    return fgRet;
}
//#endif //  CC_VUDU_SUPPORT


//-----------------------------------------------------------------------------
/** DMX_SetCaOutputBuffer
 *  Setup descrambler output buffer
 *
 *  @param  fgEnable        Enable or disable output buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetCaOutputBuffer(BOOL fgEnable, BOOL fgIsPs, UINT32 u4BufStart,
    UINT32 u4BufEnd, UINT32 u4Threshold)
{
    return _DMX_SetCaOutputBuffer(fgEnable, fgIsPs, u4BufStart, u4BufEnd,
        u4Threshold);
}


//-----------------------------------------------------------------------------
/** DMX_SetCaInputBuffer
 *  Setup descrambler input buffer
 *
 *  @param  fgEnable        Enable or disable input buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetCaInputBuffer(BOOL fgEnable, BOOL fgIsPs, UINT32 u4BufStart,
    UINT32 u4BufEnd, UINT32 u4Threshold)
{
    UNUSED(fgEnable);
    UNUSED(fgIsPs);
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    UNUSED(u4Threshold);
    return TRUE;
}


//
// Capture function
//

//-----------------------------------------------------------------------------
/** DMX_SetBypassMode
 *  Set demux (DBM) to bypass mode, mainly used for frontend capture function
 *
 *  @param  u1TsIndex       TS index (0 - 1)
 *  @param  u1PacketSize    TS packet size in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetBypassMode(UINT8 u1TsIndex, UINT8 u1PacketSize)
{
    return _DMX_SetBypassMode(u1TsIndex, u1PacketSize, TRUE, TRUE);
}


//-----------------------------------------------------------------------------
/** DMX_SetCapture
 *  Setup capture using CA output buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture)
{
    return _DMX_SetCapture(fgEnable, prCapture);
}


//-----------------------------------------------------------------------------
/** DMX_SetCapture2
 *  Setup capture using CA output buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetCapture2(BOOL fgEnable, const DMX_CAPTURE2_T* prCapture)
{
    return _DMX_SetCapture2(fgEnable, prCapture);
}


//-----------------------------------------------------------------------------
/** DMX_UpdateCaptureReadPointer
 *  Update read pointer of capture buffer
 *
 *  @param  u4NewRp         The new read pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_UpdateCaptureReadPointer(UINT32 u4NewRp)
{
    return _DMX_UpdateCaptureReadPointer(u4NewRp);
}


//-----------------------------------------------------------------------------
/** DMX_GetCaptureInfo
 *  Get capture info
 *
 *  @param  prInfo [output] The capture info
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetCaptureInfo(DMX_CAPTURE_INFO_T* prInfo)
{
    return _DMX_GetCaptureInfo(prInfo);
}


//-----------------------------------------------------------------------------
/** DMX_DemodDataCapture
 *  Setup capture using CA output buffer, not allocate buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  u4BufStart      Buffer Start
 *  @param  u4BufStart      Buffer Size
 *  @param  u4Threshold     Threshold
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_DemodDataCapture(BOOL fgEnable, const UINT32 u4BufStart, const UINT32 u4Size, const UINT32 u4Threshold)
{
    return _DMX_DemodDataCapture(fgEnable, u4BufStart, u4Size, u4Threshold);
}


//-----------------------------------------------------------------------------
/** DMX_Capture_ChannelChangeInit
 *  Setup capture buffer for channel change
 *
 *  @param  u4Size          Capture buffer size
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_Capture_ChannelChangeInit(UINT32 u4Size)
{
    return _DMX_Capture_ChannelChangeInit(u4Size);
}


//-----------------------------------------------------------------------------
/** DMX_SetChannelChange
 *  Inform channel change
 *
 *  @retval TRUE	        Succeed
 *  @retval FALSE	        Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_InformChannelChange(void)
{
    return _DMX_Capture_ChannelChangeStart();
}

#endif  // CC_MINI_DRIVER

//
// Handler
//

//-----------------------------------------------------------------------------
/** DMX_SetDecoderCallbacks
 *  Set decoder callback functions
 *
 *  @param  prCallbacks     The decoder callback functions
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_SetDecoderCallbacks(const DMX_DECODER_CALLBACKS_T* prCallbacks)
{
    _ApiLock();
    _DMX_SetDecoderCallbacks(prCallbacks);
    _ApiUnlock();
}

//-----------------------------------------------------------------------------
/** DMX_GetVideoChannel
 *  Get corresponding video channel of a given pid index
 *
 *  @param  u1Pid           PID index
 *  @param  pu1Channel [output]
 *                          The channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_GetVideoChannel(u1Pidx, pu1Channel);
    _ApiUnlock();

    return fgRet;
}


#ifdef CC_DMX_PES_EXT_BUFFER
//-----------------------------------------------------------------------------
/** DMX_UpdateExtBufferReadPointer
 *  Update the read pointer of the PES extension buffer.
 *
 *  @param  u1PIdx          PID index
 *  @param  u4NewRp         The new read pointer
 *  @param  u4DataAddr      The address of the current PES_extension_field data
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_UpdateExtBufferReadPointer(UINT8 u1Pidx, UINT32 u4NewRp, UINT32 u4DataAddr)
{
    return _DMX_UpdateExtBufferReadPointer(u1Pidx, u4NewRp, u4DataAddr);
}
#endif  // CC_DMX_PES_EXT_BUFFER


//-----------------------------------------------------------------------------
/** DMX_UpdateReadPointer
 *  Update PID read pointer (in MPEG mode)
 *
 *  @param  u1PIdx          PID index
 *  @param  u4Rp            The read pointer
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_UpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp, UINT32 u4FrameAddr)
{
    return _DMX_UpdateReadPointer(u1Pidx, u4Rp, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** DMX_UpdateVideoReadPointer
 *  Update read pointer of video PID buffer
 *
 *  @param  u1Channel       The channel
 *  @param  u4Rp            The new read pointer
 *  @param  u4FrameAddr     The frame (picture) address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_UpdateVideoReadPointer(UINT8 u1Channel, UINT32 u4Rp,
    UINT32 u4FrameAddr)
{
    return _DMX_UpdateVideoReadPointer(u1Channel, u4Rp, u4FrameAddr);
}


//
// Debug/statistics
//

//-----------------------------------------------------------------------------
/** DMX_GetPidCounters
 *  Get the counter structure of a given pid index
 *
 *  @param  u1Pidx          PID index
 *  @param  prCounters [output]
 *                          Pointer to the PID counter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetPidCounters(UINT8 u1Pidx, DMX_PID_COUNTERS_T* const prCounters)
{
    return _DMX_GetPidCounters(u1Pidx, prCounters);
}


//-----------------------------------------------------------------------------
/** DMX_ResetPidCounters
 *  Reset counters of a given pid index
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_ResetPidCounters(UINT8 u1Pidx)
{
    return _DMX_ResetPidCounters(u1Pidx);
}


//
// Utilities
//

//-----------------------------------------------------------------------------
/** DMX_Align
 *  Align a given address and an alignment
 *
 *  @param  u4Addr          The address to be aligned
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @retval The aligned address
 */
//-----------------------------------------------------------------------------
UINT32 DMX_Align(UINT32 u4Addr, UINT32 u4Alignment)
{
    return _DMX_Align(u4Addr, u4Alignment);
}


//-----------------------------------------------------------------------------
/** DMX_IsAligned
 *  Query if an address is aligned or not
 *
 *  @param  u4Addr          The address to be checked
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @retval TRUE            Aligned
 *  @retval FALSE           Not aligned
 */
//-----------------------------------------------------------------------------
BOOL DMX_IsAligned(UINT32 u4Addr, UINT32 u4Alignment)
{
    return _DMX_IsAligned(u4Addr, u4Alignment);
}


//
// Helpers
//

//-----------------------------------------------------------------------------
/** DMX_SetToDecoder
 *  Set data sink to real decoders
 *
 *  @param  fgToDecoder     TRUE: to decoders, FALSE: not to decoders
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_SetToDecoder(BOOL fgToDecoder)
{
    _ApiLock();
    _DMX_SetToDecoder(fgToDecoder);
    _ApiUnlock();
}


//-----------------------------------------------------------------------------
/** DMX_IsToDecoder
 *  Query if data sink is set to real decoders
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL DMX_IsToDecoder(void)
{
    return _DMX_IsToDecoder();
}


//-----------------------------------------------------------------------------
/** DMX_SetDebugPictureFinder
 *  Enable or disable debugging picture finder
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_SetDebugPictureFinder(BOOL fgEnable)
{
    UNUSED(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_IsDebugPictureFinder
 *  Query if picture finder debugging is enabled or not
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
BOOL DMX_IsDebugPictureFinder(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_SetFramerErrorHandlingTable
 *  Set error handling table of framers
 *
 *  @param  u4Value         New value of error handling table
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_SetFramerErrorHandlingTable(UINT32 u4Value)
{
    _DMX_SetFramerErrorHandlingTable(u4Value);
}


//-----------------------------------------------------------------------------
/** DMX_GetFramerErrorHandlingTable
 *  Get error handling table of framers
 *
 *  @retval The current value of error handling table
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetFramerErrorHandlingTable(void)
{
    return _DMX_GetFramerErrorHandlingTable();
}


//-----------------------------------------------------------------------------
/** DMX_GetScrambleState
 *  Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval The scramble state
 */
//-----------------------------------------------------------------------------
DMX_SCRAMBLE_STATE_T DMX_GetScrambleState(UINT8 u1Pidx)
{
    DMX_SCRAMBLE_STATE_T eState;

    _ApiLock();
    eState = _DMX_GetScrambleState(u1Pidx);
    _ApiUnlock();

    return eState;
}


//-----------------------------------------------------------------------------
/** DMX_SetInterruptThreshold
 *  Set interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  u4Bytes         Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32 u4Bytes)
{
    BOOL fgRet;

    _ApiLock();
    fgRet=_DMX_SetInterruptThreshold(ePidType, u4Bytes);
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_GetInterruptThreshold
 *  Get interrupt threshold for partial PES
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  pu4Bytes[output]
 *                          Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType, UINT32* pu4Bytes)
{
    BOOL fgRet;

    _ApiLock();
    fgRet=_DMX_GetInterruptThreshold(ePidType, pu4Bytes);
    _ApiUnlock();

    return fgRet;
}


// These functions exist only in SLT mode
#if defined(__MODEL_slt__) || defined(CC_DMX_EMULATION)


//-----------------------------------------------------------------------------
/** DMX_Diag
 *  Demux diagnostic function
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
INT32 DMX_Diag(void)
{
    return _DMX_Diag();
}


#endif  // __MODEL_slt__


//-----------------------------------------------------------------------------
/** DMX_GetLockState
 *  Get lock state
 *
 *  @retval TRUE            TS locked
 *  @retval FALSE           TS not lock
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetLockState(void)
{
    return _DMX_GetLockState();
}


//-----------------------------------------------------------------------------
/** DMX_GetLockStateEx
 *  Get lock state of framer
 *
 *  @retval TRUE            TS locked
 *  @retval FALSE           TS not lock
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetLockStateEx(UINT8 u1FramerIdx)
{
    return _DMX_GetLockStateEx(u1FramerIdx);
}


//-----------------------------------------------------------------------------
/** DMX_GetReceivedPacketNumber
 *  Get received TS packet number
 *
 *  @retval The received packet number
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetReceivedPacketNumber(void)
{
    return _DMX_GetReceivedPacketNumber();
}


//-----------------------------------------------------------------------------
/** DMX_GetDroppedPacketNumber
 *  Get dropped TS packet number
 *
 *  @retval The dropped packet number
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetDroppedPacketNumber(void)
{
    return _DMX_GetDroppedPacketNumber();
}

#ifdef CC_DMX_SUPPORT_MULTI2

//-----------------------------------------------------------------------------
/** DMX_SetMulti2Key
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetMulti2Key(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[2])
{
#ifndef CC_DMX_EMULATION
    if(!_DMX_Support_ARIB())
    {
        return FALSE;
    }
#endif  // CC_DMX_EMULATION
    return _DMX_SetMulti2Key(u1Index, fgEven, au4Key);
}

//-----------------------------------------------------------------------------
/** DMX_SetMulti2CbcIV
 */
//-----------------------------------------------------------------------------
void DMX_SetMulti2CbcIV(UINT8 u1Index, UINT32 u4IvHi, UINT32 u4IvLo)
{
#ifndef CC_DMX_EMULATION
    if(!_DMX_Support_ARIB())
    {
        return;
    }
#endif  // CC_DMX_EMULATION

    _DMX_SetMulti2CbcIV(u1Index, u4IvHi, u4IvLo);
}


//-----------------------------------------------------------------------------
/** DMX_SetMulti2OfbIV
 */
//-----------------------------------------------------------------------------
void DMX_SetMulti2OfbIV(UINT8 u1Index, UINT32 u4IvHi, UINT32 u4IvLo)
{
#ifndef CC_DMX_EMULATION
    if(!_DMX_Support_ARIB())
    {
        return;
    }
#endif  // CC_DMX_EMULATION

    _DMX_SetMulti2OfbIV(u1Index, u4IvHi, u4IvLo);
}


//-----------------------------------------------------------------------------
/** DMX_SetMulti2SysKey
 */
//-----------------------------------------------------------------------------
void DMX_SetMulti2SysKey(UINT8 u1Index, const UINT32 au4Key[8])
{
#ifndef CC_DMX_EMULATION
    if(!_DMX_Support_ARIB())
    {
        return;
    }
#endif  // CC_DMX_EMULATION

    _DMX_SetMulti2SysKey(u1Index, au4Key);
}


//-----------------------------------------------------------------------------
/** DMX_SetMulti2Config
 */
//-----------------------------------------------------------------------------
void DMX_SetMulti2Config(UINT8 u1Index, BOOL fgOfbFreeRun, UINT16 u2Iteration)
{
#ifndef CC_DMX_EMULATION
    if(!_DMX_Support_ARIB())
    {
        return;
    }
#endif  // CC_DMX_EMULATION

    _DMX_SetMulti2Config(u1Index, fgOfbFreeRun, u2Iteration);
}
#endif  // CC_DMX_SUPPORT_MULTI2


//-----------------------------------------------------------------------------
/** DMX_SetPacketSize
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPacketSize(UINT8 u1PacketSize)
{
    // Legacy, do not use now
    UNUSED(u1PacketSize);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_SetPacketSizeEx
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPacketSizeEx(UINT8 u1TsIndex, UINT16 u1PktSize)
{
    return _DMX_SetFramerPktSize(u1TsIndex, u1PktSize);
}

//-----------------------------------------------------------------------------
/** DMX_GetMinFreeBufferSize
 *  Get free buffer: dram buffer + 10 TS packets inside DMX
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetMinFreeBufferSize(DMX_PID_TYPE_T ePidType)
{
#ifdef ENABLE_MULTIMEDIA
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        ASSERT(0);
    }
#endif // ENABLE_MULTIMEDIA

    return _DMX_GetMinFreeBufferSize(0, ePidType);
}


//-----------------------------------------------------------------------------
/** DMX_GetMinFreeBufferSize
 *  Get free buffer: dram buffer
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetMinFreeDRAMBufferSize(DMX_PID_TYPE_T ePidType)
{
#ifdef ENABLE_MULTIMEDIA
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        ASSERT(0);
    }
#endif
    return _DMX_GetMinFreeDRAMBufferSize(0, ePidType);
}


//-----------------------------------------------------------------------------
/** DMX_GetMinFreeBufferSizeEx
 *  Get free buffer: dram buffer + 10 TS packets inside DMX
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetMinFreeBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType)
{
    return _DMX_GetMinFreeBufferSizeEx(u1TsIdx, ePidType);
}


//-----------------------------------------------------------------------------
/** DMX_GetMinFreeDRAMBufferSizeEx
 *  Get free buffer: dram buffer
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetMinFreeDRAMBufferSizeEx(UINT8 u1TsIdx, DMX_PID_TYPE_T ePidType)
{
    return _DMX_GetMinFreeDRAMBufferSizeEx(u1TsIdx, ePidType);
}


//-----------------------------------------------------------------------------
/** DMX_PcrInit
 */
//-----------------------------------------------------------------------------
void DMX_PcrInit(void)
{
    _ApiLock();
    _DMX_PcrInit();
    _ApiUnlock();
}


#ifdef CC_DMX_EMULATION
//-----------------------------------------------------------------------------
/** DMX_EnableLocalArbitor
 */
//-----------------------------------------------------------------------------
BOOL DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable)
{
    return _DMX_EnableLocalArbitor(eDevice, fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_DRAMMeasure
 */
//-----------------------------------------------------------------------------
BOOL DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs)
{
    return _DMX_DRAMMeasure(eDevice ,u4TimeMs);
}
#endif // CC_DMX_EMULATION

//-----------------------------------------------------------------------------
/** DMX_CheckAwSupport
 */
//-----------------------------------------------------------------------------
void DMX_CheckAwSupport(BOOL *pfgDriver, BOOL *pfgMicroCode)
{
    ASSERT((pfgDriver != NULL) && (pfgMicroCode != NULL));

    _ApiLock();
    *pfgDriver = _DMX_DriverHasAW();
    *pfgMicroCode = _DMX_uCodeHasAW();
    _ApiUnlock();
}


//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
void DMX_AwEraserCheck_Init(void)
{
    _DMX_AwEraserCheck_Init();
}

void DMX_AwEraserCheck_Free(void)
{
    _DMX_AwEraserCheck_Free();
}

void DMX_AwEraserCheck_Set(BOOL fgEnable)
{
    _DMX_AwEraserCheck_Set(fgEnable);
}

INT32 DMX_AwEraserCheck_Get(UINT32 u4Timeout)
{
    return _DMX_AwEraserCheck_Get(u4Timeout);
}
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------


#ifdef CC_DMX_TS_LOOPBACK
//-----------------------------------------------------------------------------
/** _DMX_Loopback_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_Loopback_Set(void)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_Loopback_Set();
    _ApiUnlock();

    return fgRet;

}


//-----------------------------------------------------------------------------
/** DMX_Loopback_Free
 */
//-----------------------------------------------------------------------------
BOOL DMX_Loopback_Free(void)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_Loopback_Free();
    _ApiUnlock();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_Loopback_Enable
 */
//-----------------------------------------------------------------------------
BOOL DMX_Loopback_Enable(BOOL fgEnable)
{
    BOOL fgRet;

    _ApiLock();
    fgRet = _DMX_Loopback_Enable(fgEnable);
    _ApiUnlock();

    return fgRet;
}
#endif  // CC_DMX_TS_LOOPBACK


//-----------------------------------------------------------------------------
/** DMX_SetErrLevel
 */
//-----------------------------------------------------------------------------
void DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel)
{
    _DMX_SetErrLevel(eLevel);
}


//-----------------------------------------------------------------------------
/** DMX_SetTSMode
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetTSMode(UINT8 u1Inst, DMX_TSFMT_T eFmt)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MUL_SetTSMode(u1Inst, eFmt);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_SetExternalFramer
 */
//-----------------------------------------------------------------------------
void DMX_SetExternalFramer(DMX_EXT_FRAMER_T eFramer)
{
    _DMX_LockApi();
    _DMX_SetExternalFramer(eFramer);
    _DMX_UnlockApi();
}

//----------------------------------------------------------------------------
/** for vdec buffer control            
*/
//----------------------------------------------------------------------------

void DMX_GetPicQSize(UINT16* u2DmxPictureQSize, UINT16* u2DmxMaxQSize)
{
    _DMX_GetPicQSize(u2DmxPictureQSize,u2DmxMaxQSize);
}

