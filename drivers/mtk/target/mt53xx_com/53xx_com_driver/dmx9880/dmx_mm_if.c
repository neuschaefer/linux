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
 * $RCSfile: dmx_mm_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_if.c
 *  Demux multi-media driver - public interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm.h"
#include "dmx_debug.h"
#include "dmx_drm_if.h"
#include "dmx_drm_drvif.h"
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
/** DMX_PVRPlay_SetDBMTestPid
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SetDBMTestPid(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_FreeDBMTestPid
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_FreeDBMTestPid(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Start
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Start(void)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Start();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Stop
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Stop(BOOL fgForce)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Stop(fgForce);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Pause
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Pause(DMX_PVRPLAY_PTR_T *prPtr)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Pause(prPtr);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Resume
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Resume(DMX_PVRPLAY_PTR_T *prPtr)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Resume(prPtr);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Reset
 */
//-----------------------------------------------------------------------------
VOID DMX_PVRPlay_Reset(void)
{
    _DMX_LockApi();
    _DMX_PVRPlay_Reset();
    _DMX_UnlockApi();
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetState
 */
//-----------------------------------------------------------------------------
DMX_PVR_PLAYSTATE_T DMX_PVRPlay_GetState(void)
{
    DMX_PVR_PLAYSTATE_T eState;

    _DMX_LockApi();
    eState = _DMX_PVRPlay_GetState();
    _DMX_UnlockApi();

    return eState;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Set(UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Set(u4Flags, prPVRPlay);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Get
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Get(UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_Get(u4Flags, prPVRPlay);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_UpdateWp
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_UpdateWp(UINT32 u4Addr, UINT32 u4Size)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_UpdateWp(u4Addr, u4Size);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_FreeBuf
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_FreeBuf()
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_FreeBuf();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_FlushBuf()
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_FlushBuf();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SingleMove(const DMX_MM_DATA_T* prData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_SingleMove(prData);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetBufPointer
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_GetBufPointer(DMX_PVRPLAY_BUFPTR_T *prPtr)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_GetBufPointer(prPtr);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SetBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SetBuffer(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Wp, UINT32 u4Rp)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_SetBuffer(u4BufStart, u4BufEnd, u4Wp, u4Rp);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_RequestReset(void)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PVRPlay_RequestReset(DMX_ALL_INSTINDEX);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SetUseDescramble
 *  For 192 + encryption format, 5365 NOT USE
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SetUseDescramble(BOOL fgEnable)
{
    UNUSED(fgEnable);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetCounter
*/
//-----------------------------------------------------------------------------
void DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter)
{
    _DMX_PVRPlay_GetCounter(prCounter);
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_ResetCounter
 */
//-----------------------------------------------------------------------------
void DMX_PVRPlay_ResetCounter(void)
{
    _DMX_PVRPlay_ResetCounter();
}


//-----------------------------------------------------------------------------
/** DMX_OpenTimeshift
 */
//-----------------------------------------------------------------------------
BOOL DMX_OpenTimeshift(BOOL fgOpen)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_OpenTimeshift(fgOpen);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Init
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Init(void)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_Init();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Set
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Set(UINT32 u4Flags, const DMX_DDI_T *prDDI)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_Set(u4Flags, prDDI);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_DDI_Set
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Get(UINT32 u4Flags, DMX_DDI_T *prDDI)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_Get(0, u4Flags, prDDI);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Free
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Free(void)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_Free();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_FlushBuf()
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_FlushBuf();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_GetFreeBufSize
*/
//-----------------------------------------------------------------------------
UINT32 DMX_DDI_GetFreeBufSize(UINT8 u1Inst)
{
    UINT32 u4FreeBufSize;

    _DMX_LockApi();
    u4FreeBufSize = _DMX_DDI_GetFreeBufSize(u1Inst);
    _DMX_UnlockApi();

    return u4FreeBufSize;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SetPacketSize
*   The hardware processes a certain amount of data at a time.  The amount is
*   called packet here, and this function sets the value.
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SetPacketSize(UINT8 u1PacketSize)
{
    // Legacy, should not use now
    UNUSED(u1PacketSize);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SetSyncOffset
*   Set the offset of the Sync byte of the padding data.
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SetSyncOffset(UINT8 u1Inst, UINT8 u1Offset)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_SetSyncOffset(u1Inst, u1Offset);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                                  UINT32 u4Addr, UINT32 u4Size)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_SingleMove(u4BufStart, u4BufEnd, u4Addr, u4Size);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_PowerDown
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_PowerDown(VOID)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_PowerDown();
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_RequestReset(VOID)
{
    DMX_INPUT_TYPE_T eInputType;
    BOOL fgRet = FALSE;

    _DMX_LockApi();
    eInputType = _DMX_GetInputType();
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        ASSERT(0);
    }
    if (eInputType == DMX_IN_PLAYBACK_PS)
    {
        fgRet = _DMX_PS_CPU_Reset(0);
    }
    else if(eInputType == DMX_IN_PLAYBACK_ES)
    {
        fgRet = _DMX_ES_Reset(0);
    }
    else
    {
        fgRet = _DMX_DDI_TSRequestReset(DMX_ALL_INSTINDEX);
    }
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_DDI_130byteEnable
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_130byteEnable(BOOL fgEnable, UINT8 u1PktSize)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DDI_130byteEnable(fgEnable,u1PktSize);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_DetectStreamID
 */
//-----------------------------------------------------------------------------
BOOL DMX_PS_DetectStreamID(BOOL fgDetect, UINT16 u2Pairs, UINT32 u4Len)
{
    UNUSED(fgDetect);
    UNUSED(u2Pairs);
    UNUSED(u4Len);

    return TRUE;
}



//-----------------------------------------------------------------------------
/** DMX_PS_SetStream
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_SetStream(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PS_T *prPs)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PS_CPU_SetStream(u1Pidx, u4Flags, prPs);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetStream
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_GetStream(UINT8 u1Pidx, UINT32 u4Flags, DMX_PS_T *prPs)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PS_CPU_GetStream(u1Pidx, u4Flags, prPs);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_FreeStream
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_FreeStream(UINT8 u1Pidx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PS_CPU_FreeStream(u1Pidx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_SetAudioType
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PS_CPU_SetAudioType(u1Pidx, eAudioType);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetAudioType
*/
//-----------------------------------------------------------------------------
DMX_AUDIO_TYPE_T DMX_PS_GetAudioType(UINT8 u1Pidx)
{
    DMX_AUDIO_TYPE_T eAudioType;

    _DMX_LockApi();
    eAudioType = _DMX_PS_CPU_GetAudioType(u1Pidx);
    _DMX_UnlockApi();

    return eAudioType;
}


//-----------------------------------------------------------------------------
/** DMX_PS_SetDataFormat
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_SetDataFormat(UINT8 u1Inst, DMX_DATA_FORMAT_T eDataFormat)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_PS_CPU_SetDataFormat(u1Inst, eDataFormat);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetDataFormat
*/
//-----------------------------------------------------------------------------
DMX_DATA_FORMAT_T DMX_PS_GetDataFormat(UINT8 u1Inst)
{
    DMX_DATA_FORMAT_T eDataFormat;

    _DMX_LockApi();
    eDataFormat = _DMX_PS_CPU_GetDataFormat(u1Inst);
    _DMX_UnlockApi();

    return eDataFormat;
}


//-----------------------------------------------------------------------------
/** DMX_ES_SetPid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prPs)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_ES_SetPid(u1Pidx, u4Flags, prPs);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_ES_GetPid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prPs)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_ES_GetPid(u1Pidx, u4Flags, prPs);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_ES_FreePid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_FreePid(UINT8 u1Pidx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_ES_FreePid(u1Pidx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_ES_SetFileLength
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_ES_SetFileLength(u1Pidx, u4Len);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_ES_SetMoveIndex
*/
//-----------------------------------------------------------------------------
void DMX_ES_SetMoveIndex(UINT8 u1Idx)
{
    _DMX_LockApi();
    _DMX_ES_SetMoveIndex(u1Idx);
    _DMX_UnlockApi();
}


//-----------------------------------------------------------------------------
/** DMX_MM_Init
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Init(void)
{
    if(!_DMX_MM_Init())
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMMStruct)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_Set(u1Idx, u4Flags, prMMStruct);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Get
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMMStruct)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_Get(u1Idx, u4Flags, prMMStruct);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Free
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Free(UINT8 u1Idx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_Free(u1Idx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_MoveData
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    BOOL fgRet;

    _DMX_LockApi();
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        ASSERT(0);
    }

    _DMX_DDI_SetPort(DMX_DDI_PORT_NO_FRAMER, FALSE);

    fgRet = _DMX_MM_MoveData(u1Idx, prData, u4TimeOut);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetBufEmptySize
 */
//-----------------------------------------------------------------------------
UINT32 DMX_MM_GetBufEmptySize(UINT8 u1Idx)
{
    UINT32 u4Size;

//    _DMX_LockApi();
    if(_DMX_MUL_GetMultipleInstEnabled())
    {
        ASSERT(0);
    }
    u4Size = _DMX_MM_GetBufEmptySize(u1Idx);
//    _DMX_UnlockApi();

    return u4Size;
}


//-----------------------------------------------------------------------------
/** DMX_MM_SetDRMMode
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_SetDRMMode(UINT8 u1Idx, DMX_DRM_MODE_T eMode)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_SetDRMMode(u1Idx, eMode);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetDRMMode
*/
//-----------------------------------------------------------------------------
DMX_DRM_MODE_T DMX_MM_GetDRMMode(UINT8 u1Idx)
{
    DMX_DRM_MODE_T eMode;

    _DMX_LockApi();
    eMode = _DMX_MM_GetDRMMode(u1Idx);
    _DMX_UnlockApi();

    return eMode;
}


//-----------------------------------------------------------------------------
/** DMX_MM_SetAES
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_SetAES(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_SetAES(u1Idx, u4Flags, prDRM);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_UnlockBuffer
*   Unlock subtitle buffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_UnlockBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    return _DMX_MM_UnlockBuffer(u1Idx, u1SerialNumber, u4Size, u4FrameAddr);
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetBuffer
*    Copy subtitle data to middleware
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_GetBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    return _DMX_MM_GetBuffer(u1Idx, u1SerialNumber, u4FrameAddr, u4SkipSize,
                             u4CopySize, pu1Dest);
}


//-----------------------------------------------------------------------------
/** DMX_MM_FlushBuffer
*
*   The caller of this function used to called it in ISR context.  This can
*   be a headache when we try to solve the noisy audio problem during the
*   the trick mode of a 24-bit PCM file (guitar solo).  ME team has agreed not
*   to call this function in ISR context, and to call it only when there is no
*   data transmission taking place.  (2009/05/15)
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_FlushBuffer(UINT8 u1Idx)
{
    BOOL fgRet = FALSE;
    DMX_INPUT_TYPE_T eInputType;

    _DMX_LockApi();
    eInputType = _DMX_GetPidInputType(u1Idx);
    if (eInputType == DMX_IN_PLAYBACK_PS)
    {
        fgRet = _DMX_PS_CPU_FlushBuffer(u1Idx);
    }
    else if((eInputType == DMX_IN_BROADCAST_TS) || (eInputType == DMX_IN_PLAYBACK_TS))
    {
        fgRet = _DMX_MM_TSFlushBuffer(u1Idx);
    }
    else if(eInputType == DMX_IN_PLAYBACK_ES)
    {
        fgRet = _DMX_ES_FlushBuffer(u1Idx);
    }
    else if(eInputType == DMX_IN_PLAYBACK_MM)
    {
        #ifdef CC_DMX_AUDIO_PREBUF
        _DMX_AudHandler_FlushQ(u1Idx);
        #endif
        fgRet = _DMX_MM_FlushBuffer(u1Idx);
    }
    else
    {
        fgRet = FALSE;
    }
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_AllocateBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_AllocateBuffer(UINT8 u1Idx, UINT32 u4Size)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_AllocateBuffer(u1Idx, u4Size);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_FreeBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_FreeBuffer(UINT8 u1Idx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_FreeBuffer(u1Idx);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_SendAudioChunk
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_SendAudioChunk(UINT8 u1Pidx, UINT32 u4Wp, UINT32 u4Pts, BOOL fgEOS)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_SendAudioChunk(u1Pidx, u4Wp, u4Pts, fgEOS);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetEnabledPidIndex
 */
//-----------------------------------------------------------------------------
UINT8 DMX_MM_GetEnabledPidIndex(DMX_INPUT_TYPE_T eInputType, DMX_PID_TYPE_T ePidType)
{
    UINT8 u1Idx;

    _DMX_LockApi();
    u1Idx = _DMX_MM_GetEnabledPidIndex(eInputType, ePidType);
    _DMX_UnlockApi();

    return u1Idx;
}

//-----------------------------------------------------------------------------
/** DMX_DTCP_SetFormat
 *  MT5363 use parser to do DTCP, so need not API lock to do sync
 */
//-----------------------------------------------------------------------------
void DMX_DTCP_SetFormat(DMX_INPUT_TYPE_T eInputType)
{
    UNUSED(eInputType);
}


//-----------------------------------------------------------------------------
/** DMX_DTCP_Set
 *  MT5363 use parser to do DTCP, so need not API lock to do sync
 */
//-----------------------------------------------------------------------------
BOOL DMX_DTCP_Set(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DTCP_SetAES
 *  MT5363 use parser to do DTCP, so need not API lock to do sync
*/
//-----------------------------------------------------------------------------
BOOL DMX_DTCP_SetAES(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DTCP_SetAES(u4Flags, prDRM);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DTCP_MoveData
 *  MT5363 use parser to do DTCP, so need not API lock to do sync
*/
//-----------------------------------------------------------------------------
BOOL DMX_DTCP_MoveData(const DMX_DTCP_DATA_T *prData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DTCP_MoveData(prData);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_DTCP_MoveBlockData
 *  MT5363 use parser to do DTCP, so need not API lock to do sync
*/
//-----------------------------------------------------------------------------
BOOL DMX_DTCP_MoveBlockData(const DMX_DTCP_BLKDATA_T *prData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_DTCP_MoveBlockData(prData);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_AES_SetDecryptInfo
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_SetDecryptInfo(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AES_SetDecryptInfo(u4Flags, prDRM);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_AES_MoveData
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_MoveData(const DMX_AES_DATA_T *prData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AES_MoveData(prData);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_AES_MoveBlockData
 */
//-----------------------------------------------------------------------------
BOOL DMX_AES_MoveBlockData(const DMX_AES_BLKDATA_T *prBlkData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AES_MoveBlockData(prBlkData);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AES_AllocEngine
*/
//-----------------------------------------------------------------------------
UINT8 DMX_AES_AllocEngine()
{
    UINT8 u1Idx;

    FUNC_ENTRY;

    _DMX_LockApi();
    u1Idx = _DMX_AES_AllocEngine();
    _DMX_UnlockApi();

    FUNC_EXIT;

    return u1Idx;
}

//-----------------------------------------------------------------------------
/** DMX_AES_FreeEngine
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_FreeEngine(UINT8 u1Idx)
{
    BOOL fgRet;

    FUNC_ENTRY;

    _DMX_LockApi();
    fgRet = _DMX_AES_FreeEngine(u1Idx);
    _DMX_UnlockApi();

    FUNC_EXIT;

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AES_SetDecryptInfoEx
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_SetDecryptInfoEx(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AES_SetDecryptInfoEx(u1Idx, u4Flags, prDRM);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AES_MoveBlockDataEx
 */
//-----------------------------------------------------------------------------
BOOL DMX_AES_MoveBlockDataEx(UINT8 u1Idx, const DMX_AES_BLKDATA_T *prBlkData)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AES_MoveBlockDataEx(u1Idx, prBlkData);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_Marlin_SetDecryptInfo
 *  Set AES by u4Idx
*/
//-----------------------------------------------------------------------------
BOOL DMX_Marlin_SetDecryptInfo(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    fgRet = _DMX_Marlin_SetDecryptInfo(u1Idx, u4Flags, prDRM);

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_Marlin_MoveData
 *  Decrypt data by AES u4Idx, this function will do AES decryption with SCTE-52
*/
//-----------------------------------------------------------------------------
BOOL DMX_Marlin_MoveData(UINT8 u1Idx, const DMX_DTCP_DATA_T *prData)
{
    BOOL fgRet;

    fgRet = _DMX_Marlin_MoveData(u1Idx, prData);

    return fgRet;
}


#ifdef DMX_SUPPORT_NDDRM
BOOL DMX_MM_SetNDDRM(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_MM_SetNDDRM(u1Idx, u4Flags, prDRM);
    _DMX_UnlockApi();

    return fgRet;
}


BOOL DMX_NDDRM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_NDDRM_MoveData(u1Idx, prData, u4TimeOut);
    _DMX_UnlockApi();

    return fgRet;
}
#endif  // DMX_SUPPORT_NDDRM

#ifdef CC_DMX_AUDIO_PREBUF
//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetActivePidx
*/
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SetActivePidx(UINT8 u1Idx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_SetActivePidx(u1Idx);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetActivePidxEx
*/
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SetActivePidxEx(UINT8 u1Idx, UINT8* pu1Buf, UINT32 u4Size)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_SetActivePidxEx(u1Idx, pu1Buf, u4Size);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_Set(u1Idx, fgToDec);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_Free
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_Free(UINT8 u1Idx)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_Free(u1Idx);
    _DMX_UnlockApi();

    return fgRet;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_SeekAndUpdate
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_SeekAndUpdate(u1Idx, u4Pts, pu4Pts);
    _DMX_UnlockApi();

    return fgRet;
}


//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetEnable
 */
//-----------------------------------------------------------------------------
void DMX_AudHandler_SetEnable(BOOL fgEnable)
{
    _DMX_LockApi();
    _DMX_AudHandler_SetEnable(fgEnable);
    _DMX_UnlockApi();
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetQSize
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size)
{
    BOOL fgRet;

    _DMX_LockApi();
    fgRet = _DMX_AudHandler_SetQSize(u1Idx, u2Size);
    _DMX_UnlockApi();

    return fgRet;
}

#endif  // CC_DMX_AUDIO_PREBUF

