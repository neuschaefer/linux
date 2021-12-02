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
#include "dmx_ide.h"
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
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Stop
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Stop(BOOL fgForce)
{
    UNUSED(fgForce);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Pause
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Pause(DMX_PVRPLAY_PTR_T *prPtr)
{
    UNUSED(prPtr);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Resume
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Resume(DMX_PVRPLAY_PTR_T *prPtr)
{
    UNUSED(prPtr);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Reset
 */
//-----------------------------------------------------------------------------
VOID DMX_PVRPlay_Reset(void)
{
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetState
 */
//-----------------------------------------------------------------------------
DMX_PVR_PLAYSTATE_T DMX_PVRPlay_GetState(void)
{

    return DMX_PVRPLAY_STATE_STOP;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Set(UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay)
{
    UNUSED(u4Flags);
    UNUSED(prPVRPlay);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_Get
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_Get(UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay)
{
    UNUSED(u4Flags);
    UNUSED(prPVRPlay);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_UpdateWp
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_UpdateWp(UINT32 u4Addr, UINT32 u4Size)
{
    UNUSED(u4Addr);
    UNUSED(u4Size);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_FreeBuf
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_FreeBuf()
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_FlushBuf()
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SingleMove(const DMX_MM_DATA_T* prData)
{
    UNUSED(prData);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetBufPointer
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_GetBufPointer(DMX_PVRPLAY_BUFPTR_T *prPtr)
{
    UNUSED(prPtr);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SetBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SetBuffer(UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Wp, UINT32 u4Rp)
{
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    UNUSED(u4Wp);
    UNUSED(u4Rp);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_RequestReset(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_EnableTSOut
*/
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_EnableTSOut(BOOL fgEnable)
{
    UNUSED(fgEnable);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_SetUseDescramble
 *  For 192 + encryption format
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_SetUseDescramble(BOOL fgEnable)
{
    UNUSED(fgEnable);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetUseDescramble
 *  Setup to use scramble
 */
//-----------------------------------------------------------------------------
BOOL DMX_PVRPlay_GetUseDescramble(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_GetCounter
*/
//-----------------------------------------------------------------------------
void DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter)
{
    UNUSED(prCounter);
}


//-----------------------------------------------------------------------------
/** DMX_PVRPlay_ResetCounter
 */
//-----------------------------------------------------------------------------
void DMX_PVRPlay_ResetCounter(void)
{
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Init
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Init(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Set
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Set(UINT32 u4Flags, const DMX_DDI_T *prDDI)
{
    UNUSED(u4Flags);
    UNUSED(prDDI);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Set
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Get(UINT32 u4Flags, DMX_DDI_T *prDDI)
{
    UNUSED(u4Flags);
    UNUSED(prDDI);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SetPort
*/
//-----------------------------------------------------------------------------
void DMX_DDI_SetPort(DMX_DDI_PORT_T ePort)
{
    UNUSED(ePort);
}


//-----------------------------------------------------------------------------
/** DMX_DDI_GetPort
*/
//-----------------------------------------------------------------------------
DMX_DDI_PORT_T DMX_DDI_GetPort(void)
{
    return DMX_DDI_PORT_FRAMER;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_Free
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_Free(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_StreamStart
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_StreamStart()
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_StreamStop
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_StreamStop()
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_FlushBuf
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_FlushBuf()
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_GetFreeBufSize
*/
//-----------------------------------------------------------------------------
UINT32 DMX_DDI_GetFreeBufSize(void)
{
    return 0;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_FillBuf
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_FillBuf(UINT32 u4DataAddr, UINT32 u4DataSize, UINT32 *pu4FilledSize)
{
    UNUSED(u4DataAddr);
    UNUSED(u4DataSize);
    UNUSED(pu4FilledSize);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_GetStreamState
*/
//-----------------------------------------------------------------------------
DMX_DDI_STATE_T DMX_DDI_GetStreamState()
{
    return DMX_DDI_STOP;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SetPacketSize
*   The hardware processes a certain amount of data at a time.  The amount is
*   called packet here, and this function sets the value.
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SetPacketSize(UINT8 u1PacketSize)
{
    UNUSED(u1PacketSize);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SetSyncOffset
*   Set the offset of the Sync byte of the padding data.
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SetSyncOffset(UINT8 u1Offset)
{
    UNUSED(u1Offset);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_SingleMove
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                                  UINT32 u4Addr, UINT32 u4Size)
{
    UNUSED(u4BufStart);
    UNUSED(u4BufEnd);
    UNUSED(u4Addr);
    UNUSED(u4Size);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_MoveData
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_MoveData(const DMX_MM_DATA_T *prData)
{
    UNUSED(prData);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_PowerDown
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_PowerDown(VOID)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_DDI_RequestReset
*/
//-----------------------------------------------------------------------------
BOOL DMX_DDI_RequestReset(VOID)
{
    return TRUE;
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
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPs);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetStream
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_GetStream(UINT8 u1Pidx, UINT32 u4Flags, DMX_PS_T *prPid)
{
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPid);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PS_FreeStream
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_FreeStream(UINT8 u1Pidx)
{
    UNUSED(u1Pidx);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PS_SetAudioType
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType)
{
    UNUSED(u1Pidx);
    UNUSED(eAudioType);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetAudioType
*/
//-----------------------------------------------------------------------------
DMX_AUDIO_TYPE_T DMX_PS_GetAudioType(void)
{
    return DMX_AUDIO_MPEG;
}


//-----------------------------------------------------------------------------
/** DMX_PS_SetDataFormat
*/
//-----------------------------------------------------------------------------
BOOL DMX_PS_SetDataFormat(DMX_DATA_FORMAT_T eDataFormat)
{
    UNUSED(eDataFormat);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_PS_GetDataFormat
*/
//-----------------------------------------------------------------------------
DMX_DATA_FORMAT_T DMX_PS_GetDataFormat(void)
{
    return DMX_DATA_FORMAT_RAW;
}


//-----------------------------------------------------------------------------
/** DMX_ES_SetPid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prPs)
{
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPs);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_ES_GetPid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prPs)
{
    UNUSED(u1Pidx);
    UNUSED(u4Flags);
    UNUSED(prPs);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_ES_FreePid
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_FreePid(UINT8 u1Pidx)
{
    UNUSED(u1Pidx);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_ES_SetFileLength
*/
//-----------------------------------------------------------------------------
BOOL DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len)
{
    UNUSED(u1Pidx);
    UNUSED(u4Len);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Init
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Init(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMMStruct)
{
    UNUSED(u1Idx);
    UNUSED(u4Flags);
    UNUSED(prMMStruct);

#ifdef CC_DMX_PURE_AUDIO
    if (prMMStruct->ePidType == DMX_PID_TYPE_PURE_AUDIO)
    {
        return _DMX_PURE_Set(u1Idx, u4Flags, prMMStruct);
    }
#endif  // CC_DMX_PURE_AUDIO
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Get
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMMStruct)
{
    UNUSED(u1Idx);
    UNUSED(u4Flags);
    UNUSED(prMMStruct);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_Free
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_Free(UINT8 u1Idx)
{
    UNUSED(u1Idx);

#ifdef CC_DMX_PURE_AUDIO
    if (_DMX_PURE_IsPureAudio(u1Idx))
    {
        return _DMX_PURE_Free(u1Idx);
    }
#endif  // CC_DMX_PURE_AUDIO
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_MoveData
 */
//-----------------------------------------------------------------------------
BOOL DMX_MM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    UNUSED(u1Idx);
    UNUSED(prData);
    UNUSED(u4TimeOut);

#ifdef CC_DMX_PURE_AUDIO
    if (_DMX_PURE_IsPureAudio(u1Idx))
    {
        return _DMX_PURE_MoveData(u1Idx, prData);
    }
#endif  // CC_DMX_PURE_AUDIO
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetBufEmptySize
 */
//-----------------------------------------------------------------------------
UINT32 DMX_MM_GetBufEmptySize(UINT8 u1Idx)
{
    UNUSED(u1Idx);

#ifdef CC_DMX_PURE_AUDIO
    if (_DMX_PURE_IsPureAudio(u1Idx))
    {
        return _DMX_PURE_GetEmptyBufferSize(u1Idx);
    }
#endif  // CC_DMX_PURE_AUDIO   
    
    return 0;
}


//-----------------------------------------------------------------------------
/** DMX_MM_SetDRMMode
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_SetDRMMode(UINT8 u1Idx, DMX_DRM_MODE_T eMode)
{
    UNUSED(u1Idx);
    UNUSED(eMode);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetDRMMode
*/
//-----------------------------------------------------------------------------
DMX_DRM_MODE_T DMX_MM_GetDRMMode(UINT8 u1Idx)
{
    UNUSED(u1Idx);
    return DMX_DRM_MODE_BYPASS;
}


//-----------------------------------------------------------------------------
/** DMX_MM_SetAES
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_SetAES(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    UNUSED(u1Idx);
    UNUSED(u4Flags);
    UNUSED(prDRM);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_UnlockBuffer
*   Unlock subtitle buffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_UnlockBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4Size,
    UINT32 u4FrameAddr)
{
    UNUSED(u1Idx);
    UNUSED(u1SerialNumber);
    UNUSED(u4Size);
    UNUSED(u4FrameAddr);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_GetBuffer
*    Copy subtitle data to middleware
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_GetBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest)
{
    UNUSED(u1Idx);
    UNUSED(u1SerialNumber);
    UNUSED(u4FrameAddr);
    UNUSED(u4SkipSize);
    UNUSED(u4CopySize);
    UNUSED(pu1Dest);
    return TRUE;
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
    UNUSED(u1Idx);

#ifdef CC_DMX_PURE_AUDIO
    if (_DMX_PURE_IsPureAudio(u1Idx))
    {
        return _DMX_PURE_FlushBuffer(u1Idx);
    }
#endif  // CC_DMX_PURE_AUDIO  
    
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_AllocateBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_AllocateBuffer(UINT8 u1Idx, UINT32 u4Size)
{
    UNUSED(u1Idx);
    UNUSED(u4Size);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_MM_FreeBuffer
*/
//-----------------------------------------------------------------------------
BOOL DMX_MM_FreeBuffer(UINT8 u1Idx)
{
    UNUSED(u1Idx);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_AES_SetDecryptInfo
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_SetDecryptInfo(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM)
{
    UNUSED(u4Flags);
    UNUSED(prDRM);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_AES_MoveData
*/
//-----------------------------------------------------------------------------
BOOL DMX_AES_MoveData(const DMX_AES_DATA_T *prData)
{
    UNUSED(prData);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_AES_MoveBlockData
 */
//-----------------------------------------------------------------------------
BOOL DMX_AES_MoveBlockData(const DMX_AES_BLKDATA_T *prBlkData)
{
    UNUSED(prBlkData);

    return TRUE;
}

#ifdef CC_DMX_AUDIO_PREBUF
//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetActivePidx
*/
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SetActivePidx(UINT8 u1Idx)
{
    UNUSED(u1Idx);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_Set
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec)
{
    UNUSED(u1Idx);
    UNUSED(fgToDec);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_Free
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_Free(UINT8 u1Idx)
{
    UNUSED(u1Idx);
    return TRUE;
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_SeekAndUpdate
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts)
{
    UNUSED(u1Idx);
    UNUSED(u4Pts);
    UNUSED(pu4Pts);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetEnable
 */
//-----------------------------------------------------------------------------
void DMX_AudHandler_SetEnable(BOOL fgEnable)
{
    UNUSED(fgEnable);
}

//-----------------------------------------------------------------------------
/** DMX_AudHandler_SetQSize
 */
//-----------------------------------------------------------------------------
BOOL DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size)
{
    UNUSED(u1Idx);
    UNUSED(u2Size);
    return TRUE;
}

#endif  // CC_DMX_AUDIO_PREBUF

