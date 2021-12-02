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
 * $RCSfile: tshift_loopback.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fvr_app.c
 *  FVR demo application
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_926.h"

#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "fbm_drvif.h"
#include "mpv_drvif.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

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

// Loopback
static UINT32 _u4LoopbackDistance;
static UINT32 _au4Key[8];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _TSSetDmxRecPid
 */
//-----------------------------------------------------------------------------
static BOOL _TSSetDmxRecPid(UINT8 u1Pidx, UINT16 u2PidNum)
{
    DMX_PID_T rPid;
    UINT32 u4Flags;

    // DMX PID
    rPid.u1TsIndex = 0;
    rPid.u1DeviceId = 0;
	rPid.u1KeyIndex = 0;
	rPid.fgEnable = TRUE;
	rPid.u1SteerMode = DMX_STEER_TO_PVR;
	rPid.fgAllocateBuffer = FALSE;
	rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_NONE;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0;
    rPid.u2Pid = u2PidNum;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.fgPrimary = FALSE;
   
    u4Flags = DMX_PID_FLAG_ALL;
    if (!DMX_SetPid(u1Pidx,  u4Flags, &rPid))
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _TSSetDmxPlayPid
 */
//-----------------------------------------------------------------------------
static BOOL _TSSetDmxPlayPid(UINT8 u1Pidx, UINT16 u2PidNum, 
                             DMX_PID_TYPE_T eType, UINT8 u1KeyIndex)
{
    DMX_PID_T rPid;
    UINT32 u4Flags;
    FBM_POOL_T* prFbmPool;

    if(!DMX_SetKey(u1KeyIndex + 8, FALSE, TRUE, _au4Key))   // DES, Even key
    {
        return FALSE;
    }

    // DMX PID
    rPid.u1TsIndex = 2;     // PVR play
    rPid.u1DeviceId = 0;	
	rPid.fgEnable = TRUE;
	rPid.u1SteerMode = DMX_STEER_TO_FTUP;
	rPid.fgAllocateBuffer = FALSE;
	rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_NONE;
    
    rPid.eDescMode = DMX_DESC_MODE_DES_ECB;
    rPid.u1KeyIndex = u1KeyIndex + 8;
    
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = 0;
    rPid.u2Pid = u2PidNum;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.fgPrimary = FALSE;

    if(eType == DMX_PID_TYPE_ES_VIDEO)
    {
        rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;        

        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != NULL);

        // Setup buffer attributes
        rPid.fgAllocateBuffer = FALSE;

        rPid.u4BufAddr = prFbmPool->u4Addr;
        rPid.u4BufSize = prFbmPool->u4Size;

        rPid.rVideoType = DMX_VIDEO_MPEG;   // Fixme H.264 ?
    }
    else if(eType == DMX_PID_TYPE_ES_AUDIO)
    {
        ASSERT(0);      // Fixme
    }
    else if(eType == DMX_PID_TYPE_PSI)
    {
        ASSERT(0);      // Fixme
    }
    else
    {
        return FALSE;
    }
   
    u4Flags = DMX_PID_FLAG_ALL;
    if (!DMX_SetPid(u1Pidx,  u4Flags, &rPid))
    {
        return FALSE;
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _TSSetFVRPid
 */
//-----------------------------------------------------------------------------
static BOOL _TSSetFVRPid(UINT8 u1Pidx, UINT16 u2PidNum)
//                            BOOL fgPktSwap, BOOL fgPicSearch, BOOL fgEnc)
{
    FVR_PID_T rFVRPid;
    UINT32 u4Flags;

    // FVR PID
    u4Flags = FVR_PID_FLAG_ALL & (~FVR_PID_FLAG_KEY);

    rFVRPid.fgEnable = TRUE;
    rFVRPid.fgPktSwap = FALSE;
    rFVRPid.fgScramble = FALSE;
    rFVRPid.fgPicSearch = FALSE;
    rFVRPid.u2Pid = u2PidNum;
    rFVRPid.fgPicSearch = FALSE;
    rFVRPid.u1PicSearchMask = 0x0;
    rFVRPid.pfnPicSearchNotify = NULL;
    rFVRPid.fgPktSwap = FALSE;
    rFVRPid.pfnSwapNotify = NULL;    

    rFVRPid.fg3DES = FALSE;
    rFVRPid.fgCBCMode = FALSE;
    rFVRPid.rKeyType= FVR_KEY_EVEN;
    rFVRPid.fgScramble = TRUE;

    rFVRPid.u4EvenKey1Low = _au4Key[1];
    rFVRPid.u4EvenKey1High = _au4Key[0];

    u4Flags |= FVR_PID_FLAG_EVEN_KEY_1;
    
    if(!FVR_SetPid(u1Pidx, u4Flags, &rFVRPid))
    {
        return FALSE;
    }
    
    return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _TSMPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    MPV_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        ASSERT(0);
        break;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;

    return (MPV_CallBackEsm(&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
/** _TSSetFVRPid
 */
//-----------------------------------------------------------------------------
static BOOL _TSSetVideoDecoder(BOOL fgToDecoder)
{
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    
    DMX_SetToDecoder(fgToDecoder);
    rDecoderCallbacks.pfnSendVideoPes = _TSMPEGVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = MPV_UpdateVldWptr;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _TSLPRecNotify
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _TSLPRecNotify(UINT32 u4Addr, UINT32 u4Size, UINT32 u4PktCount)
{   
    UINT32 u4DataSize;
    
    Printf("_TSLPRecNotify, addr:0x%x size:0x%x\n", u4Addr, u4Size);

    u4DataSize = FVR_GetDataSize();

    if((_u4LoopbackDistance != 0) && (_u4LoopbackDistance > u4DataSize))
    {
        Printf("Skip update play WP\n");
        return;
    }
    _u4LoopbackDistance = 0x0;  // Only first time is effective

    DMX_PVRPlay_UpdateBuf(u4Addr, u4Size);     // Update play write pointer
}

//-----------------------------------------------------------------------------
/** _TSLPPlayNotify
 */
//-----------------------------------------------------------------------------
static BOOL _TSLPPlayNotify(UINT32 u4Addr, UINT32 u4Size)
{
    Printf("_TSLPPlayNotify: 0x%x\n", u4Size);

    FVR_UnlockBuffer(u4Addr, u4Size);   // Update capture read pointer
    
    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** TS_Loopback_Init
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL TS_Loopback_Init(UINT32 u4BufSize, UINT32 u4RecPktCount, UINT32 u4PlayThreshold)
{
    DMX_PVR_PLAY_T rPlay;
    FVR_GBL_T rGBL;
    FVR_TIMESTAMP_T rTimeStamp;
    UINT32 u4Flag;

    if((FVR_GetState() == FVR_STATE_RECORDING) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    if(!FVR_FreeBuffer())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_FreeBuf())
    {
        return FALSE;
    }

    // Capture init
    rGBL.fgAllocateBuffer = TRUE;
    rGBL.u2TargetPktCount = u4RecPktCount;
    rGBL.u4BufSize = u4BufSize;
    rGBL.pfnPktCountNotify = _TSLPRecNotify;
    
    if(!FVR_SetGBL(FVR_GBL_FLAG_TARGET_PKT | FVR_GBL_FLAG_BUFFER | FVR_GBL_FLAG_NOTIFY, &rGBL))
    {
        return FALSE;
    }

    if(!FVR_GetGBL(FVR_GBL_FLAG_BUFFER, &rGBL))
    {
        return FALSE;
    }
    
    // Play init
    rPlay.fgAllocBuf = FALSE;
    rPlay.u4BufStart = rGBL.u4BufStart;
    rPlay.u4BufSize = u4BufSize;
    rPlay.u4ThresholdSize = u4PlayThreshold;
    rPlay.pfnFillDataNotify = _TSLPPlayNotify;

    if(!DMX_PVRPlay_Set(PVRPLAY_FLAGS_THRESHOLD |PVRPLAY_FLAGS_BUFFER | PVRPLAY_FLAGS_CALLBACK, 
                    &rPlay))
    {
        return FALSE;
    }

    // Enabe Record timestamp
    rTimeStamp.fgEnable = TRUE;
    rTimeStamp.u2ClkDiv = 8;
    rTimeStamp.u4Mask = 0xFFFFFFFF;
    rTimeStamp.u4Counter= 0x0;
    FVR_SetTimestamp(FVR_TIMESTAMP_FLAG_ALL, &rTimeStamp);

    // Enable PLAY timestamp
    rPlay.fgContainTimeStamp = TRUE;
    rPlay.fgIgnoreTimeStamp = FALSE;
    rPlay.u2TimeStampFreqDiv = 8;    
    DMX_PVRPlay_Set(PVRPLAY_FLAGS_TIMESTAMP, &rPlay);

    // Set Key
    _au4Key[0] = 0x567abcd1;
    _au4Key[1] = 0xabcdef12;
    _au4Key[2] = 0x90143356;
    _au4Key[3] = 0x67acd123;
        
	return TRUE;
}


//-----------------------------------------------------------------------------
/** TS_Loopback_SetPid
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL TS_Loopback_SetPid(UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T eType)
{
    
    ASSERT(u1Pidx < 16);    // Reserved 8 PID for playback

    if(!_TSSetDmxRecPid(u1Pidx, u2PidNum))
    {
        LOG(3, "Create DMX record pid fail!\n");
        return FALSE;
    }

    if(!_TSSetFVRPid(u1Pidx, u2PidNum))
    {
        LOG(3, "Create FVR pid fail!\n");
        return FALSE;
    }

    if(!_TSSetDmxPlayPid(u1Pidx + 16, u2PidNum, eType, u1Pidx))
    {
        LOG(3, "Create DMX play pid fail!\n");
        return FALSE;
    }

    // Decoder
    if(!_TSSetVideoDecoder(TRUE))
    {
        LOG(3, "Enable Video deocer fail\n");
        return FALSE;
    }
   
    return TRUE;
}


//-----------------------------------------------------------------------------
/** TS_Loopback_Start
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL TS_Loopback_Start(UINT32 u4Distance)
{

    if((FVR_GetState() == FVR_STATE_RECORDING) || 
       (DMX_PVRPlay_GetState() == DMX_PVRPLAY_STATE_PLAYING))
    {
        LOG(3, "Please stop first\n");
        return FALSE;
    }

    _u4LoopbackDistance = u4Distance;

    if(!DMX_PVRPlay_Start())
    {
        return FALSE;
    }
    
    if(!FVR_Start())
    {
        return FALSE;
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** TS_Loopback_Stop
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL TS_Loopback_Stop()
{
    if(!FVR_Stop())
    {
        return FALSE;
    }

    if(!DMX_PVRPlay_Stop())
    {
        return FALSE;
    }

	return TRUE;
}


//-----------------------------------------------------------------------------
/** TS_Loopback_GetDataSize
 *  @retval UINT32
 */
//-----------------------------------------------------------------------------
UINT32 TS_Loopback_GetDataSize()
{
    DMX_PVR_PLAY_T rPlay;
    UINT32 u4EmptySize, u4DataSize;

    DMX_PVRPlay_Get(PVRPLAY_FLAGS_BUFFER, &rPlay);
    u4EmptySize = DMX_PVRPlay_GetBufSize();

    ASSERT(rPlay.u4BufSize >= u4EmptySize);

    u4DataSize = rPlay.u4BufSize - u4EmptySize;

    return u4DataSize;
}


#if 0
//-----------------------------------------------------------------------------
/** FVRAP_Loopback_FlushBuf
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL FVRAP_Loopback_FlushBuf()
{
	return TRUE;
}


//-----------------------------------------------------------------------------
static BOOL _LoopbackCaptureHandler(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    DMX_CAPTURE_INFO_T rCapture;

    Printf("_LoopbackCaptureHandler\n");

    if(!DMX_GetCaptureInfo(&rCapture))
    {
        return FALSE;
    }

    Printf("start:0x%x, end:0x%x, wp:0x%x, rp:0x%x\n", 
                  rCapture.u4BufStart, rCapture.u4BufEnd, rCapture.u4Wp, rCapture.u4Rp);

    // IDE send packet
    if (_DMX_IDE_IsEnabled())
    {
        _DMX_IDE_SendPacket(0, rCapture.u4BufStart, rCapture.u4BufEnd, 
                               rCapture.u4Rp, prInfo->u4Wp);
    }

    VERIFY(DMX_UpdateCaptureReadPointer(prInfo->u4Wp));

    return TRUE;
}

//-----------------------------------------------------------------------------
/** TS_Loopback_Test
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL TS_Loopback_Test(UINT32 u4BufSize)
{
    DMX_CAPTURE_T rCapture;

    // Capture
    rCapture.u4BytePassDev = DMX_CAPTURE_BYPASS_NONE;
    rCapture.u4BufSize = 0x100000;
    rCapture.u4Threshold = 0x40000;
    rCapture.pfnCaptureHandler = _LoopbackCaptureHandler;
    rCapture.pvCaptureTag = NULL;

    if (!DMX_SetCapture(TRUE, &rCapture))
    {
        return FALSE;
    }
    
    // Loopback init
    if(!TS_Loopback_Init(u4BufSize))
    {
        return FALSE;
    }

    // Loopback start
    if(!TS_Loopback_Start())
    {
        return FALSE;
    }
    
	return TRUE;
}
#endif

