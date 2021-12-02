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
#include "dmx_ide.h"
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
#ifdef CC_DMX_PURE_AUDIO
    VERIFY(_DMX_PURE_Init());
#endif // CC_DMX_PURE_AUDIO

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_SetIgnorePESLen
 */
//-----------------------------------------------------------------------------
VOID DMX_SetIgnorePESLen(BOOL fgEnable)
{
    UNUSED(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_SetScrambleScheme
 */
//-----------------------------------------------------------------------------
VOID DMX_SetScrambleScheme(BOOL fgOrg)
{
    UNUSED(fgOrg);
}


//-----------------------------------------------------------------------------
/** DMX_EnablePower
 */
//-----------------------------------------------------------------------------
VOID DMX_EnablePower(BOOL fgEnable)
{
    UNUSED(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_SelectInputType
 */
//-----------------------------------------------------------------------------
BOOL DMX_SelectInputType(DMX_INPUT_TYPE_T rInputType)
{
    UNUSED(rInputType);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetInputType
 */
//-----------------------------------------------------------------------------
DMX_INPUT_TYPE_T DMX_GetInputType(void)
{
    return DMX_IN_BROADCAST_TS;
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
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMX_SetFrontEnd
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
    UNUSED(eFEType);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetFrontEnd
 *  Return front end type
 */
//-----------------------------------------------------------------------------
DMX_FRONTEND_T DMX_GetFrontEnd(void)
{
    return DMX_FE_ATSC_DEMOD;
}


//-----------------------------------------------------------------------------
/** DMX_ResetFramer
 */
//-----------------------------------------------------------------------------
void DMX_ResetFramer(void)
{
}


//-----------------------------------------------------------------------------
/** DMX_GetFramerIndex
 *  Get used framer index
 */
//-----------------------------------------------------------------------------
UINT8 DMX_GetFramerIndex(void)
{
    return 0;
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
    return TRUE;
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
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPid);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPid);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPid);    
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u2Pid);
    UNUSED(pu1PidIndex);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u1PidIndex);
    UNUSED(fgEnable);
    UNUSED(fgNewMode);
    return TRUE;
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
    UNUSED(u1FilterIndex);
    UNUSED(u1Bank);
    UNUSED(u4Flags);
    UNUSED(prFilter);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                               const DMX_FILTER_GENERIC_T* prFilter)
{
    UNUSED(u1FilterIndex);
    UNUSED(u4Flags);
    UNUSED(prFilter);
    return TRUE;
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
    UNUSED(u1FilterIndex);
    UNUSED(u1Bank);
    UNUSED(u4Flags);
    UNUSED(prFilter);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetGenFilter
 *  Get a generic section filter setting
 */
//-----------------------------------------------------------------------------
BOOL DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                DMX_FILTER_GENERIC_T* prFilter)
{
    UNUSED(u1FilterIndex);
    UNUSED(u4Flags);
    UNUSED(prFilter);
    return TRUE;
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
    UNUSED(eVDec);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetVideoType
 *  Get video type
 */
//-----------------------------------------------------------------------------
DMX_VIDEO_TYPE_T DMX_GetVideoType(void)
{
    return DMX_VIDEO_MPEG;
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
    UNUSED(u1Pidx);
    UNUSED(u4Size);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u1SerialNumber);
    UNUSED(u4Size);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u1SerialNumber);
    UNUSED(u4FrameAddr);
    UNUSED(u4SkipSize);
    UNUSED(u4CopySize);
    UNUSED(pu1Dest);
    return TRUE;
}


#ifdef DMX_MEASURE_PSI_TIME
//-----------------------------------------------------------------------------
/** DMX_PSI_GetMaxTime
 */
//-----------------------------------------------------------------------------
VOID DMX_PSI_GetMaxTime(HAL_TIME_T* prTime)
{
    UNUSED(prTime);
}


//-----------------------------------------------------------------------------
/** DMX_ResetPSIMaxTime
 */
//-----------------------------------------------------------------------------
VOID DMX_ResetPSIMaxTime(void)
{
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
    UNUSED(u1Pidx);
    UNUSED(u4Size);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u1SerialNumber);
    UNUSED(u4Size);
    UNUSED(u4FrameAddr);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u1SerialNumber);
    UNUSED(u4FrameAddr);
    UNUSED(u4SkipSize);
    UNUSED(u4CopySize);
    UNUSED(pu1Dest);
    return TRUE;
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
    UNUSED(u1Index);
    UNUSED(fg3Des);
    UNUSED(fgEven);
    UNUSED(au4Key);
    return TRUE;
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
    UNUSED(u4IvHi);
    UNUSED(u4IvLo);
}


//-----------------------------------------------------------------------------
/** DMX_SetAesKeyLen
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetAesKeyLen(UINT32 u4KeyLen)
{
    UNUSED(u4KeyLen);    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_SetAesKey
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8])
{
    UNUSED(u1Index);
    UNUSED(fgEven);
    UNUSED(au4Key);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_SetAesIV
 */
//-----------------------------------------------------------------------------
void DMX_SetAesIV(const UINT32 au4Iv[4])
{
    UNUSED(au4Iv);
}


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
    UNUSED(fgEnable);
    UNUSED(fgIsPs);
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    UNUSED(u4Threshold);
    return TRUE;
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
    UNUSED(u1TsIndex);
    UNUSED(u1PacketSize);
    return TRUE;
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
    UNUSED(fgEnable);
    UNUSED(prCapture);
    return TRUE;
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
    UNUSED(fgEnable);
    UNUSED(prCapture);
    return TRUE;
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
    UNUSED(u4NewRp);
    return TRUE;
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
    UNUSED(prInfo);
    return TRUE;
}


//
// IDE
//

//-----------------------------------------------------------------------------
/** DMX_IDE_Enable
 *  Enable or disable IDE test port
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
void DMX_IDE_Enable(BOOL fgEnable)
{
    UNUSED(fgEnable);
}


//-----------------------------------------------------------------------------
/** DMX_IDE_IsEnabled
 *  Query if IDE is enabled
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
BOOL DMX_IDE_IsEnabled(void)
{
    return TRUE;
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
#ifdef CC_DMX_PURE_AUDIO   
    _DMX_SetDecoderCallbacks(prCallbacks);
#endif // CC_DMX_PURE_AUDIO
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
    UNUSED(u1Pidx);
    UNUSED(pu1Channel);
    return TRUE;
}


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
    UNUSED(u1Pidx);
    UNUSED(u4Rp);
    UNUSED(u4FrameAddr);

#ifdef CC_DMX_PURE_AUDIO
    if (_DMX_PURE_IsPureAudio(u1Pidx))
    {
        return _DMX_PURE_UpdateReadPointer(u1Pidx, u4Rp, u4FrameAddr);
    }
#endif  // CC_DMX_PURE_AUDIO
    
    return TRUE;
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
    UNUSED(u1Channel);
    UNUSED(u4Rp);
    UNUSED(u4FrameAddr);
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(prCounters);
    return TRUE;
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
    UNUSED(u1Pidx);
    return TRUE;
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
    UNUSED(u4Addr);
    UNUSED(u4Alignment);
    return 0;
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
    UNUSED(u4Addr);
    UNUSED(u4Alignment);
    return TRUE;
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
    UNUSED(fgToDecoder);
#ifdef CC_DMX_PURE_AUDIO    
    _DMX_SetToDecoder(fgToDecoder);
#endif // CC_DMX_PURE_AUDIO
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
    return TRUE;
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
    UNUSED(u4Value);
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
    return 0;
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
    UNUSED(u1Pidx);
    return DMX_SCRAMBLE_STATE_CLEAR;
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
    UNUSED(ePidType);
    UNUSED(u4Bytes);
    return TRUE;
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
    UNUSED(ePidType);
    UNUSED(pu4Bytes);
    return TRUE;
}


// These functions exist only in SLT mode
#ifdef __MODEL_slt__


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
    return 0;
}


//-----------------------------------------------------------------------------
/** DMX_JChipDiag
 *  Verify the J-chip functionality.
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code
 */
//-----------------------------------------------------------------------------
INT32 DMX_JChipDiag(UINT32 u1TsIndex)
{
    UNUSED(u1TsIndex);
    return 0;
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
    return TRUE;
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
    UNUSED(u1FramerIdx);
    return TRUE;
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
    return 0;
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
    return 0;
}


//-----------------------------------------------------------------------------
/** DMX_SetPacketSize
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetPacketSize(UINT8 u1PacketSize)
{
    UNUSED(u1PacketSize);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetMinFreeBufferSize
 *
 */
//-----------------------------------------------------------------------------
UINT32 DMX_GetMinFreeBufferSize(DMX_PID_TYPE_T ePidType)
{
    UNUSED(ePidType);
    return 0;
}


//-----------------------------------------------------------------------------
/** DMX_SetTsIndex
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
BOOL DMX_SetTsIndex(UINT8 u1Idx)
{
    UNUSED(u1Idx);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_GetMemoryUsage
 */
//-----------------------------------------------------------------------------
void DMX_GetMemoryUsage(DMX_MEM_TRACKER_T *pMemTracker)
{
    UNUSED(pMemTracker);
}


//-----------------------------------------------------------------------------
/** DMX_PcrInit
 */
//-----------------------------------------------------------------------------
void DMX_PcrInit(void)
{
}


#ifdef CC_EMULATION
//-----------------------------------------------------------------------------
/** DMX_EnableLocalArbitor
 */
//-----------------------------------------------------------------------------
BOOL DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable)
{
    UNUSED(eDevice);
    UNUSED(fgEnable);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DRAMMeasure
 */
//-----------------------------------------------------------------------------
BOOL DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs)
{
    UNUSED(eDevice);
    UNUSED(u4TimeMs);
    return TRUE;
}
#endif // CC_EMULATION

//-----------------------------------------------------------------------------
/** DMX_CheckAwSupport
 */
//-----------------------------------------------------------------------------
void DMX_CheckAwSupport(BOOL *pfgDriver, BOOL *pfgMicroCode)
{
    UNUSED(pfgDriver);
    UNUSED(pfgMicroCode);
}


//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
void DMX_AwEraserCheck_Init(void)
{
}

void DMX_AwEraserCheck_Free(void)
{
}

void DMX_AwEraserCheck_Set(BOOL fgEnable)
{
    UNUSED(fgEnable);
}

INT32 DMX_AwEraserCheck_Get(UINT32 u4Timeout)
{
    UNUSED(u4Timeout);
    return 0;
}
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
/** DMX_SetErrLevel
 */
//-----------------------------------------------------------------------------
void DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel)
{
    UNUSED(eLevel);
}

