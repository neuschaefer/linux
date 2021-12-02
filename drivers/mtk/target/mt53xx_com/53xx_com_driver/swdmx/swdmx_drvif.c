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
 * $RCSfile: swdmx_drvif.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_drvif.c
 *  Software program stream demux driver - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "swdmx_if.h"
#include "swdmx_drvif.h"
#include "swdmx_avi.h"
#include "swdmx_mpg.h"
#include "swdmx_wmv.h"
#include "swdmx_mp4.h"
#include "swdmx_flv.h"
#include "swdmx_mkv.h"
#include "swdmx_rm.h"
#include "swdmx_cmpb.h"
#include "feeder_if.h"
#include "fbm_drvif.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "vdec_drvif.h"
#include "x_util.h"
#include "x_rand.h"
#include "x_timer.h"
#ifdef CC_FLASH_TEST
#include "swdmx_flash.h"
#endif
#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif
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
#define DATA_EXHAUSTED_THRESHOLD 16
#define WAIT_INPUT_DATA_THRD 3000   // 3 sec
#define IS_NETWORK_SRC(x)   (x == MM_SRC_TYPE_NETWORK_VUDU)
//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static SWDMX_INFO_T _rSwdmxInfo = {0};

static SWDMX_RANGE_LIST_T _rCurRangeReq;
static SWDMX_RANGE_LIST_T _rQRangeReq;

static HANDLE_T _hSwDmxMainThread = 0;
static HANDLE_T _hRangeMutex = 0;
static HANDLE_T _hDmxFeederReqDataQ = 0;
static HANDLE_T _hDmxPlayCmdQ = 0;
static HANDLE_T _hFlushMutex = 0;
static HANDLE_T _hFlushSema = 0;

#ifdef CC_DLNA_SUPPORT
//static HANDLE_T _hFeederUnderFlowMutex = NULL;
static BOOL _fgFeederUnderFlow = FALSE;
#endif
//static SWDMX_DECODER_INFO_T _rDecoderInfo;

#ifdef CC_SWDMX_RENDER_PTS
static UINT32 _u4RenderPts = 0;
#endif

#if 0//def CC_SWDMX_RENDER_POSITION
static BOOL _fgSetRenderPosition = FALSE;
static INT32 _i4TempRef = 0;
static UINT16 _u2DecodingOrder = 0;
static UINT64 _u8Offset = 0;
static UINT64 _u8OffsetI = 0;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID _SwdmxWaitDataTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    LOG(2, "swdmx data exhausted=1, timeout\n");
    FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
}

static VOID _CheckDataExhausted(VOID)
{
    UINT16 u2VdecQueueSize = 0, u2VdecMaxQueueSize = 0;

    if(!MPV_IsDisplay(0))
    {
        return; //wait sequence header
    }
    else if(!_rSwdmxInfo.fgWaitDataTimerStart)
    {
        VERIFY(x_timer_start(_rSwdmxInfo.hWaitDataTimer, WAIT_INPUT_DATA_THRD, X_TIMER_FLAG_REPEAT,
            _SwdmxWaitDataTimeout, (void*)(UINT32)0) == OSR_OK); //10sec
        LOG(2, "swdmx data exhausted, start timer\n");
        _rSwdmxInfo.fgWaitDataTimerStart = TRUE;
    }

    VDEC_GetQueueInfo(0, &u2VdecQueueSize, &u2VdecMaxQueueSize);
    if ((u2VdecQueueSize < DATA_EXHAUSTED_THRESHOLD) &&
            (!_rSwdmxInfo.fgRcvEOS) && (!_rSwdmxInfo.fgDataExhausted))
    {
        if(u2VdecQueueSize < (DATA_EXHAUSTED_THRESHOLD>>1))
        {
            FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
        }
        LOG(2, "swdmx data exhausted=1(Q%d), start timer\n", u2VdecQueueSize);
        _rSwdmxInfo.fgDataExhausted = TRUE;
        VERIFY(x_timer_start(_rSwdmxInfo.hWaitDataTimer, WAIT_INPUT_DATA_THRD, X_TIMER_FLAG_REPEAT,
            _SwdmxWaitDataTimeout, (void*)(UINT32)0) == OSR_OK); //10sec
    }
    else if (u2VdecQueueSize > (DATA_EXHAUSTED_THRESHOLD<<1) && _rSwdmxInfo.fgDataExhausted)
    {
        LOG(2, "swdmx data exhausted=0, stop timer\n");
        _rSwdmxInfo.fgDataExhausted = FALSE;
        VERIFY(x_timer_stop(_rSwdmxInfo.hWaitDataTimer) == OSR_OK);
    }
}


#ifdef CC_DLNA_SUPPORT
VOID _SWDMX_FeederUnderFlowCb()
{
    //VERIFY(x_sema_lock(_hFeederUnderFlowMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
    if (_rSwdmxInfo.i4PlaySpeed == 1000 && !_fgFeederUnderFlow)
    {
        if (_rSwdmxInfo.pfnSwdmxRangeCb)
        {
            _rSwdmxInfo.pfnSwdmxRangeCb(_rCurRangeReq.prDmxRangeInfo->pv_tag,
                                        MM_RANGE_BUFF_UNDERFLOW,
                                        _rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF);
        }
        _fgFeederUnderFlow = TRUE;
        if (_rSwdmxInfo.fgEnVideoDmx)
        {
            VDEC_Pause(ES0);
            VDP_SetPauseMm(0, TRUE);
        }
        if (_rSwdmxInfo.fgEnAudioDmx)
        {
#ifdef CC_MT5363
            AUD_DSPCmdPause(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
            AUD_DSPCmdPause(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
            AUD_DSPCmdPause(INTERNAL_DAC_ID_MAIN);
#endif
        }
        LOG(0,"_SWDMX_FeederUnderFlowCb : A/V pause\n");
    }
    //VERIFY(x_sema_unlock(_hFeederUnderFlowMutex) == OSR_OK);
}

VOID _SWDMX_FeederStatusNfyCb(UINT32 u4ActionId, UINT32 u4Param1, UINT32 u4Param2)
{
    VDEC_HDR_INFO_T rVdecHdrInfo;

    if (u4ActionId == FEEDER_NFY_STATUS_READY)
    {
        //VERIFY(x_sema_lock(_hFeederUnderFlowMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
        if (((_rSwdmxInfo.i4PlaySpeed == 1000)|| (_rSwdmxInfo.i4PlaySpeed == 2000))
            && _fgFeederUnderFlow)
        {
            if (_rSwdmxInfo.pfnSwdmxRangeCb)
            {
                _rSwdmxInfo.pfnSwdmxRangeCb(_rCurRangeReq.prDmxRangeInfo->pv_tag,
                                            MM_RANGE_BUFF_READY,
                                            _rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                            0xFFFFFFFF);
            }
            _fgFeederUnderFlow = FALSE;
            VDEC_QueryInfo(ES0,&rVdecHdrInfo);
            if (_rSwdmxInfo.fgEnVideoDmx)
            {
                if (VDP_GetStepForward(VDP_1) != 0)
                {
                    VDP_SetFBNoTimeout(VDP_1, FALSE);
                    VDP_SetStepForward(VDP_1, VDP_PLAY_NORMAL);
                    VDP_SetPauseMm(0, FALSE);
                }
                if (VDP_GetSlowForward(VDP_1) != 0)
                {
                    VDP_SetFBNoTimeout(VDP_1, FALSE);
                    VDP_SetSlowForward(VDP_1, VDP_PLAY_NORMAL);
                }
                VDP_SetPauseMm(0, FALSE);
                VDEC_Play(ES0, rVdecHdrInfo.eCodecType);

                if(_rSwdmxInfo.i4PlaySpeed == 1000)
                {
                    VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_1X, 0, 0);
                    VDP_SetSpeed(0,STC_SPEED_TYPE_FORWARD_1X);
                    VDP_SetTrickMode(VDP_PLAY_NORMAL);
                }
                else
                {
                    VDEC_SetMMParam(ES0, VDEC_MM_PLAY_SPEED, STC_SPEED_TYPE_FORWARD_2X, 0, 0);
                    VDP_SetSpeed(0,STC_SPEED_TYPE_FORWARD_2X);
                    VDP_SetTrickMode(VDP_PLAY_NORMAL);
                }
            }
            if (_rSwdmxInfo.fgEnAudioDmx)
            {
#ifdef CC_MT5363
                AUD_DSPCmdResume(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdResume(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdResume(INTERNAL_DAC_ID_MAIN);
#endif
            }
            LOG(0,"_SWDMX_FeederStatusNfyCb : A/V resume\n");
        }
        //VERIFY(x_sema_unlock(_hFeederUnderFlowMutex) == OSR_OK);
    }
    else if (u4ActionId == FEEDER_NFY_STATUS_IGNORE_READY)
    {
        if (_fgFeederUnderFlow)
        {
             VDEC_QueryInfo(ES0,&rVdecHdrInfo);
            if (_rSwdmxInfo.fgEnVideoDmx)
            {
                VDP_SetPauseMm(0, FALSE);
                VDEC_Play(ES0, rVdecHdrInfo.eCodecType);
            }
            if (_rSwdmxInfo.fgEnAudioDmx)
            {
#ifdef CC_MT5363
                AUD_DSPCmdResume(AUD_D_DSP, INTERNAL_DAC_ID_MAIN);
                AUD_DSPCmdResume(AUD_A_DSP, INTERNAL_DAC_ID_MAIN);
#else
                AUD_DSPCmdResume(INTERNAL_DAC_ID_MAIN);
#endif
            }
            LOG(0,"_SWDMX_FeederStatusNfyCb : A/V resume\n");
            _fgFeederUnderFlow = FALSE;
       }
    }
}
#endif
/**
*/
#if 0
SWDMX_HDR_PRS_STATE_T _SWDMX_HdrParseFeederTest(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL   fgRet;
    UINT16 u2Cnt;
    INT32  i4Ret;
    UINT32 u4FeederReqID;
    UINT32 u4SendSize;
    UINT32 u4SendPos;
    UINT32 u4Count;
    UINT32 u4ReqCount;
    UINT32 u4TestCount;
    FEEDER_REQ_DATA_T rFeederReqData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_BUF_INFO_T rBuffInfo;
    UINT32 u4NewPtr;

    srand(1328198);

    while (1)
    {
        x_thread_delay(100);
        VERIFY(x_msg_q_num_msgs(_hDmxFeederReqDataQ, &u2Cnt) == OSR_OK);
        if (u2Cnt == 0)
        {
            break;
        }
        _SWDMX_FlushReqMsgQ();
        LOG(0, "Wait for flush\n");
    }

    u4FeederReqID = 0;
    u4SendPos = 0;
    u4SendSize = 1024;
    u4TestCount = 0;
    FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &rBuffInfo);
    while (u4TestCount < 100000)
    {
        _SWDMX_FlushReqMsgQ();
        u4ReqCount = 0;
        u4Count = (((UINT32)rand()) % 5) + 1; // random
        u4SendPos = (((UINT32)rand()) % ((UINT32)_rSwdmxInfo.u8FileSize)) + 1; // random
        u4SendSize = (((UINT32)rand()) % ((235*1024) - 1)) + 1; // random

        if (u4SendSize == 0)
        {
            LOG(1, "u4SendSize = 0\n");
            continue;
        }

        while (u4Count--)
        {
            if ( u4SendPos >= (UINT32)_rSwdmxInfo.u8FileSize)
            {
                u4SendPos = 0;
            }
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                                     u4SendSize,
                                     (UINT64)(u4SendPos),
                                     &rFeederCond,
                                     u4FeederReqID++);
            if (i4Ret != FEEDER_E_OK)
            {
                LOG(0, "FeederSetRequest Fail, pos %d size %d\n", (UINT32)u4SendPos, (UINT32)u4SendSize);
                ASSERT(0);
            }
            else
            {
                //handle other case, FEEDER_E_INVALID_PARAM
            }
            u4SendPos += u4SendSize;
            u4ReqCount ++;
        }

        u4Count = u4ReqCount; // the times try to remove previouw feeder's ack
        while (u4Count--)
        {
            fgRet = _SWDMX_ReceiveFeederAck((VOID*)&rFeederReqData);
            // check rFeederReqData.u4Id
            if ((!fgRet) || (rFeederReqData.u4ReadSize == 0))
            {
                LOG(0, "size = 0 or ack timeout\n");
                ASSERT(0);
            }
            if (rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
            {
                LOG(0, "FEEDER_SOURCE_INVALID.\n");
                ASSERT(0);
            }
            u4NewPtr = (rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize > rBuffInfo.u4EndAddr)?
                       rBuffInfo.u4StartAddr + rFeederReqData.u4ReadSize - (rBuffInfo.u4EndAddr - rFeederReqData.u4WriteAddr)
                       : rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
            i4Ret = FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4NewPtr);
            if (i4Ret != FEEDER_E_OK)
            {
                LOG(0, "FeederUpdateReadPointer fail pos %d\n",
                    rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize);
                ASSERT(0);
            }
        }
        if (rFeederReqData.u4Id != (u4FeederReqID-1))
        {
            LOG(0, "return ReqID %d not equal to request one %d\n", rFeederReqData.u4Id, (u4FeederReqID-1));
            ASSERT(0);
        }


        if ((u4TestCount % 500) == 0)
        {
            LOG(0, "TestCount %d\n", u4TestCount);
        }
        u4TestCount++;
    }
    LOG(0, "Feeder Test Finish\n");

    switch (_rSwdmxInfo.eCurFMT)
    {
    case SWDMX_FMT_AVI:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_AVIHdrParse;
        break;
    case SWDMX_FMT_VC1_WMV:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_WMVHeaderParse;
        break;
    case SWDMX_FMT_MPEG1_DAT:
    case SWDMX_FMT_MPEG2_PS:
    case SWDMX_FMT_MPEG2_TS:
    case SWDMX_FMT_MPEG2_TS_192:
    case SWDMX_FMT_MPEG2_TS_ZERO_192:
    case SWDMX_FMT_MPEG2_TS_ENCRYPT_192:
    case SWDMX_FMT_MPEG2_TS_TIME_SHIFT:
    case SWDMX_FMT_MPEG2_VIDEO_ES:
    case SWDMX_FMT_H264_VIDEO_ES:
    case SWDMX_FMT_VC1_ES:
    case SWDMX_FMT_MPEG4_VIDEO_ES:
    case SWDMX_FMT_RAW_AUDIO_ES:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        break;
    case SWDMX_FMT_MP4:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_Mp4HdrPrs;
        break;
    case SWDMX_FMT_MKV:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MKVHdrParse;
        break;

    default:
        LOG(0, "unknow format\n");
        break;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}
#endif

VOID _SWDMX_LockFlushMutex(VOID)
{
    if (_hFlushMutex == 0)
    {
        return;
    }
    VERIFY(x_sema_lock(_hFlushMutex, X_SEMA_OPTION_WAIT) == OSR_OK);
}

VOID _SWDMX_UnlockFlushMutex(VOID)
{
    if (_hFlushMutex == 0)
    {
        return;
    }
    VERIFY(x_sema_unlock(_hFlushMutex) == OSR_OK);
}

void _SWDMX_LockFlushSema(VOID)
{
    if (_hFlushSema == 0)
    {
        return;
    }
    VERIFY(x_sema_lock(_hFlushSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

void _SWDMX_UnlockFlushSema(VOID)
{
    if (_hFlushSema == 0)
    {
        return;
    }
    VERIFY(x_sema_unlock(_hFlushSema) == OSR_OK);
}

static BOOL _SWDMX_ReceivePlayCmd(VOID *pvData, UINT32 u4CurState)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    ENUM_SWDMX_PLAY_STATE_T eStatus;

    zMsgSize = sizeof(UINT32);
    //LOG(0,"current status : %d\n",u4CurState);
    if (u4CurState != eSWDMX_STATE_PLAY)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_hDmxPlayCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(_hDmxPlayCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    //u4Cmd = (UINT32*)(pvData);
    //LOG(0,"_SWDMX_ReceivePlayCmd : %d\n",*u4Cmd);
    if (i4Ret != OSR_OK)
    {
        return FALSE;
    }

    eStatus = (ENUM_SWDMX_PLAY_STATE_T)(*(UINT32*)pvData);
    if (eStatus == eSWDMX_STATE_MAX)
    {
        _SWDMX_LockFlushMutex();
        if (_rSwdmxInfo.fgFlush)
        {
            UNUSED(_SWDMX_DoFlush());
            _rSwdmxInfo.fgFlush = FALSE;
            _SWDMX_UnlockFlushSema();
        }
        _SWDMX_UnlockFlushMutex();
        return FALSE;
    }

    return TRUE;
}


// block function from now on.
static BOOL _SWDMX_SendPlayCmd(ENUM_SWDMX_PLAY_STATE_T ePlayState)
{
    INT32 i4Ret;
    UINT32 u4PlayCmd = ePlayState;
    SIZE_T zMsgSize = sizeof(UINT32);

    //LOG(0, "_SWDMX_SendPlayCmd %d\n", ePlayState);

    i4Ret = x_msg_q_send(_hDmxPlayCmdQ, &u4PlayCmd, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "SWDMX Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(_hDmxPlayCmdQ, &u4PlayCmd, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

#if 0
    // make sure status is changed.
    while (_rSwdmxInfo.eSwDmxStatus != ePlayState)
    {
        x_thread_delay(1);
    }
#endif

    return TRUE;

}


VOID _SWDMX_FlushReqMsgQ()
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    //UINT32 u4DecFin;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    FEEDER_REQ_DATA_T rData;

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);

    while (x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                           &(_hDmxFeederReqDataQ), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hDmxFeederReqDataQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
    // We have to have one dummy ack in hand.
    // So we could return to swdmx main loop after we do FlushReqMsgQ.
    rData.eDataType = FEEDER_SOURCE_INVALID;
    zMsgSize = sizeof(rData);
    i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &rData, zMsgSize, 255);
    VERIFY(i4Ret == OSR_OK);
}

UINT16 _SWDMX_GetFeederQueueSize()
{
    UINT16 u2QueueSize;

    VERIFY(x_msg_q_num_msgs(_hDmxFeederReqDataQ, &u2QueueSize) == OSR_OK);

    return u2QueueSize;
}

INT32 _SWDMX_ReceiveFeederAck(VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    FEEDER_REQ_DATA_T *prData;

    if (!pvData)
    {
        LOG(1, "_SWDMX_ReceiveFeederAck pvData = NULL\n");
        return FALSE;
    }

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    prData = pvData;

    i4Ret = x_msg_q_receive_timeout(&u2MsgQIdx, pvData, &zMsgSize,
                                    &(_hDmxFeederReqDataQ), 1, SWDMX_FEEDER_TIMEOUT);

    ASSERT(zMsgSize == sizeof(FEEDER_REQ_DATA_T));

    if (i4Ret == OSR_TIMEOUT)
    {
        prData->eDataType = FEEDER_SOURCE_INVALID;
        LOG(1, "_SWDMX_ReceiveFeederAck Timeout\n");
        // Restore original behavior. We would not reset request when timeout
        // Only reset request when u4WriteAddr = 0 (error case).
        //return OSR_TIMEOUT;
        return TRUE;
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(3, "_SWDMX_ReceiveFeederAck Fail %d\n", i4Ret);
        return FALSE;
    }

    return TRUE;
}


INT32 _SWDMX_ReceiveFeederAckNoWait(VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    //EDER_REQ_DATA_T *prData;

    if (!pvData)
    {
        LOG(1, "_SWDMX_ReceiveFeederAck pvData = NULL\n");
        return FALSE;
    }

    zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    //prData = pvData;
    i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                            &_hDmxFeederReqDataQ, 1, X_MSGQ_OPTION_NOWAIT);

    ASSERT(zMsgSize == sizeof(FEEDER_REQ_DATA_T));

    if (i4Ret == OSR_NO_MSG)
    {
        LOG(9, "_SWDMX_ReceiveFeederAckNoWait do nothing\n");
    }
    else if (i4Ret != OSR_OK)
    {
        LOG(3, "_SWDMX_ReceiveFeederAckNoWait Fail %d\n", i4Ret);
    }

    return i4Ret;
}

VOID _SWDMX_FeederCallBack(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param)
{
    // Send Msg to _hDmxFeederReqDataQ
    INT32 i4Ret;
    FEEDER_REQ_DATA_T rFeederReqData;
    SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);
    x_memcpy(&rFeederReqData, (FEEDER_REQ_DATA_T*)u4Param, sizeof(FEEDER_REQ_DATA_T));
    //LOG(0, "_SWDMX_FeederCallBack Id %d\n", rFeederReqData.u4Id);
    i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "_SWDMX_FeederCallBack Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);
}

UINT32 _u4SwdmxDelay = 33/*1*/;  // megaa 20100414
static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverData(VOID)
{
    SWDMX_HDR_PRS_STATE_T ePrsState;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl;
    UINT32 u4StartPTS, u4EndPTS;
    static UINT64 u8SeekPos;

    if (_rCurRangeReq.fgIsFull == TRUE)
    {
        _rCurRangeReq.fgInUse = TRUE;

        ASSERT(_rSwdmxInfo.pfnSwdmxHdrParse);

        if (_rSwdmxInfo.fgPlayChange)
        {
            if(!((_rSwdmxInfo.i4PlaySpeed == SWDMX_PLAY_SPEED_1X) ||
                (_rSwdmxInfo.i4PlaySpeed == SWDMX_PLAY_SPEED_2X)))
            {
                FeederIgnoreReady();
            }

            ePrsState = _rSwdmxInfo.pfnSwdmxHdrParse(
                            &_rCurRangeReq,
                            _rSwdmxInfo.i4PlaySpeed,
                            _rSwdmxInfo.u8SeekPos,
                            _rSwdmxInfo.fgPlayChange
                        );
            u8SeekPos = _rSwdmxInfo.u8SeekPos;
            _rSwdmxInfo.u8SeekPos = 0xFFFFFFFF;
            _rSwdmxInfo.u8SeekPos <<= 32;
            _rSwdmxInfo.u8SeekPos += 0xFFFFFFFF;
            _rSwdmxInfo.fgPlayChange = FALSE;
        }
        else
        {
            ePrsState = _rSwdmxInfo.pfnSwdmxHdrParse(
                            &_rCurRangeReq,
                            _rSwdmxInfo.i4PlaySpeed,
                            _rSwdmxInfo.u8SeekPos,
                            _rSwdmxInfo.fgPlayChange
                        );
        }

        if(_rCurRangeReq.prDmxRangeInfo &&
            IS_NETWORK_SRC(_rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
        {
            _CheckDataExhausted();
        }

        switch (ePrsState)
        {
        case eSWDMX_HDR_PRS_SUCCEED:
            if ((_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED) ||
                    (_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP) ||
                    (_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_PENDING))
            {
                MM_RANGE_INFO_T* pTempRangeInfo = _rCurRangeReq.prDmxRangeInfo;

                // Callback to the upper layer for notifying range complete
                if ((_rSwdmxInfo.pfnSwdmxRangeCb != 0) &&
                    (pTempRangeInfo))
                {
                    //mp4 seek case
                    if (_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP)
                    {
                        if (_rQRangeReq.fgIsFull == TRUE)
                        {
                            prIdxTbl = _rQRangeReq.prDmxRangeInfo->pt_idxtbl_lst;
                            //find video table
                            while (prIdxTbl != NULL)
                            {
                                if (prIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
                                {
                                    //query if Q range contain target seek PTS
                                    u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
                                    u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;

                                    LOG(1, "Seek: seek 0x%x, Q 0x%x--0x%x\n",
                                        (UINT32)u8SeekPos, u4StartPTS, u4EndPTS);
                                    break;
                                }
                                prIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prIdxTbl->pv_next_tbl;
                            }
                            _rSwdmxInfo.pfnSwdmxRangeCb(pTempRangeInfo->pv_tag,
                                                        MM_RANGE_ABORT,
                                                        pTempRangeInfo->ui4_range_id,
                                                        0xFFFFFFFF);
                        }
                        else
                        {
                            LOG(2, "Wait Q range indexing done.Request pts = 0x%08x\n",
                                _rCurRangeReq.u4CurPTS);
                            _rSwdmxInfo.pfnSwdmxRangeCb(pTempRangeInfo->pv_tag,
                                                        MM_RANGE_DONE,
                                                        pTempRangeInfo->ui4_range_id,
                                                        _rCurRangeReq.u4CurPTS);
                        }
                    }
                    else if (_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED)
                    {
                        _rSwdmxInfo.pfnSwdmxRangeCb(pTempRangeInfo->pv_tag,
                                                    MM_RANGE_DONE,
                                                    pTempRangeInfo->ui4_range_id,
                                                    0xFFFFFFFF);
                        LOG(5, "Range Finished Callback to Update Range ID = 0x%08x\n", pTempRangeInfo->ui4_range_id);
                        {
                            // delete range
                            _rCurRangeReq.fgInUse = FALSE;
                            SWDMX_DelRange(NULL, (UINT32)_rCurRangeReq.u4RangeId, FALSE);
                        }
                    }
                }

                if ((_rSwdmxInfo.eCurFMT == SWDMX_FMT_MP4) &&
                        ((_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_FINISHED) ||
                         (_rCurRangeReq.eRangeStats == eSWDMX_RANGE_STATE_SKIP)))
                {
                    // for speed change in the file end, we keep
                    _rCurRangeReq.fgInUse = FALSE;
                    SWDMX_DelRange(NULL, (UINT32)_rCurRangeReq.u4RangeId, FALSE);  //set new range from Qrange
                }
                else
                {
                    // we finish play, sleep a while
                    x_thread_delay(33);
                }
            }
            break;
        case eSWDMX_HDR_PRS_FED_MOVE_FAIL:
        case eSWDMX_HDR_PRS_FAIL:
            // Fail to move data
            // we finish play, sleep a while
#if 1  // megaa 20100414
            x_thread_delay(_u4SwdmxDelay);  // megaa 20100225: tmp solution for push mode
#else
            x_thread_delay(33);
#endif
            break;
        default:
            break;
        }
    }
    else
    {
        x_thread_delay(10);
    }

    return eSWDMX_HDR_PRS_EMPTY_RANGE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_MainLoop
 *  Software demuxer main loop
 *
 *  @retval VOID
 */
//-----------------------------------------------------------------------------
static VOID _SWDMX_MainLoop(VOID *pvArg)
{
    UINT32 u4PlayCmd;
    //SWDMX_HDR_PRS_STATE_T eStatus;
    UNUSED(pvArg);

    while (1)
    {
        // block if SwDmx status is not eSWDMX_STATE_PLAY
        u4PlayCmd = (UINT32)eSWDMX_STATE_MAX;
        if (_SWDMX_ReceivePlayCmd(&u4PlayCmd, _rSwdmxInfo.eSwDmxStatus) == TRUE)
        {
            if ((_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PLAY))
            {
                ASSERT(_rCurRangeReq.prDmxRangeInfo!=NULL);
                if (_rSwdmxInfo.eCurFMT != SWDMX_FMT_ES_WITH_PTS)
                {
                ASSERT(_rSwdmxInfo.pfnSwdmxSetRange != NULL);
                }

                _SWDMX_FlushReqMsgQ();

                if (_rSwdmxInfo.pfnSwdmxSetRange != NULL)
                {
                    _rSwdmxInfo.pfnSwdmxSetRange(&_rCurRangeReq, _rSwdmxInfo.u4VidStrmID,
                                                 _rSwdmxInfo.u4AudStrmID, _rSwdmxInfo.u4AudSubStrmID);
                }
            }

            if ((_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_PAUSE) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PLAY))
            {
                if (_rSwdmxInfo.pfnSwdmxPlay)
                {
                    _rSwdmxInfo.pfnSwdmxPlay();
                }
            }

            if ((_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_PLAY) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_PAUSE))
            {
                if (_rSwdmxInfo.pfnSwdmxPause)
                {
                    _rSwdmxInfo.pfnSwdmxPause();
                }
            }

            if ((_rSwdmxInfo.eSwDmxStatus != eSWDMX_STATE_STOP) &&
                    (u4PlayCmd == (UINT32)eSWDMX_STATE_STOP))
            {
                if(_rCurRangeReq.prDmxRangeInfo &&
                    IS_NETWORK_SRC(_rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
                {
                    _rSwdmxInfo.fgRcvEOS = FALSE;
                    _rSwdmxInfo.fgDataExhausted = TRUE;
                    _rSwdmxInfo.fgWaitDataTimerStart = FALSE;
                    VERIFY(x_timer_stop(_rSwdmxInfo.hWaitDataTimer) == OSR_OK);
                    LOG(2, "swdmx data exhausted, stop timer\n");
                }

                if (_rSwdmxInfo.pfnSwdmxStop)
                {
                    _rSwdmxInfo.pfnSwdmxStop();
                }

                _rSwdmxInfo.u8SeekPos = 0xFFFFFFFF;
                _rSwdmxInfo.u8SeekPos <<= 32;
                _rSwdmxInfo.u8SeekPos += 0xFFFFFFFF;
                _rSwdmxInfo.fgPlayChange = FALSE;
                _rSwdmxInfo.i4PlaySpeed = SWDMX_PLAY_SPEED_1X;

                if (_rQRangeReq.fgIsFull == TRUE && _rSwdmxInfo.pfnSwdmxRangeCb)
                {
                    LOG(5, "Callback for QRange Abort id = 0x%08x\n", _rQRangeReq.prDmxRangeInfo->ui4_range_id);
                    _rSwdmxInfo.pfnSwdmxRangeCb(_rQRangeReq.prDmxRangeInfo->pv_tag,
                                                MM_RANGE_ABORT,
                                                _rQRangeReq.prDmxRangeInfo->ui4_range_id,
                                                0xFFFFFFFF);
                }
                x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));

                if ((_rCurRangeReq.fgIsFull == TRUE) && _rSwdmxInfo.pfnSwdmxRangeCb && _rCurRangeReq.prDmxRangeInfo)
                {
                    LOG(5, "Callback for Cur Range Abort id = 0x%08x\n", _rCurRangeReq.prDmxRangeInfo->ui4_range_id);
                    _rSwdmxInfo.pfnSwdmxRangeCb(_rCurRangeReq.prDmxRangeInfo->pv_tag,
                                                MM_RANGE_ABORT,
                                                _rCurRangeReq.prDmxRangeInfo->ui4_range_id,
                                                0xFFFFFFFF);
                }
                x_memset((void*)&_rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));

                // clean Vdec thumbnail count
                VDEC_SetRenderPicCount(ES0, FALSE, 0);
                AUD_MM_Set_Dec_Fmt_Scramble(0, FALSE);
                AUD_MM_Set_Dec_Fmt_Conflict(0, FALSE);
                // DTV00144297, pvr file also have this problem
                // let all format reset pcr
                //if(_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    // DTV00139410, [MT5361B_EU]AV can not sync when do timeshift at HD channel
                    // dmx use the same micro code with DTV.
                    // so dmx would not call dmx_reset -> dmx_pcrinit
                    // STC might be stopped if our last control is pause.
                    // dmx_pcrinit will start stc
                    DMX_PcrInit();
                }
            }

            if ((u4PlayCmd == eSWDMX_STATE_STOP) ||
                    (u4PlayCmd == eSWDMX_STATE_PAUSE))
            {
                if (_rSwdmxInfo.pfSwdmxNfyFct)
                {
                    _rSwdmxInfo.pfSwdmxNfyFct(_rSwdmxInfo.pvSwdmxNfyTag, SWDMX_COND_CTRL_DONE, 0, 0);
                }
            }

            //stop command will send "feeder invalid" msg each time
            //no matter what current status is, so do flush while receiving stop.
            if (u4PlayCmd == (UINT32)eSWDMX_STATE_STOP)
            {
                _SWDMX_FlushReqMsgQ();
            }

            // update status if receive command successfully
            _rSwdmxInfo.eSwDmxPrevousStatus = _rSwdmxInfo.eSwDmxStatus;
            UNUSED(_rSwdmxInfo.eSwDmxPrevousStatus);
            _rSwdmxInfo.eSwDmxStatus = (ENUM_SWDMX_PLAY_STATE_T)u4PlayCmd;
            //LOG(0,"_rSwdmxInfo.eSwDmxStatus :%d\n",_rSwdmxInfo.eSwDmxStatus);
            continue;
        }

        if (_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_PLAY)
        {
            _SWDMX_DeliverData();
        }
    }
}


//-----------------------------------------------------------------------------
/** _SWDMX_Cfg
 *  Config some function pointers for different file format
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
static BOOL _SWDMX_Cfg(VOID)
{
    _rSwdmxInfo.pfnSwdmxInit = NULL;
    _rSwdmxInfo.pfnSwdmxPlay = NULL;
    _rSwdmxInfo.pfnSwdmxPause = NULL;
    _rSwdmxInfo.pfnSwdmxAbort = NULL;
    _rSwdmxInfo.pfnSwdmxGetStreamInfo = NULL;
    _rSwdmxInfo.pfnSwdmxSetStrmID = NULL;
    _rSwdmxInfo.pfnSwdmxEnableStrm = NULL;
    _rSwdmxInfo.pfnSwdmxDisableStrm = NULL;
    _rSwdmxInfo.pfnSwdmxSetRange = NULL;
    _rSwdmxInfo.pfnSwdmxHdrParse = NULL;
    _rSwdmxInfo.pfnSwdmxStop = NULL;
    _rSwdmxInfo.pfnSwdmxFlush = NULL;
    _rSwdmxInfo.pfnSwdmxSetInfo = NULL;
    _rSwdmxInfo.pfnSwdmxGetInfo = NULL;
    _rSwdmxInfo.pfnSwdmxRangeCb = NULL;
    _rSwdmxInfo.pfnSwdmxReadBuf = NULL;
    _rSwdmxInfo.pfnSwdmxReleaseBuf = NULL;
    _rSwdmxInfo.pfnSwdmxAudioCb = NULL;

    switch (_rSwdmxInfo.eCurFMT)
    {
#ifndef CC_FLV_DISABLE
    case SWDMX_FMT_FLV:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_FlvInit;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_FlvSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_FlvEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_FlvDisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_FlvSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_FlvHdrParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_FlvStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_FlvSetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_FlvGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_FlvAudioCb;
		_rSwdmxInfo.pfnSwdmxAbort=_SWDMX_FlvAbort;
        break;
#endif
#ifndef CC_AVI_DISABLE
    case SWDMX_FMT_AVI:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_AVIInit;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_AVISetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_AVIEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_AVIDisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_AVISetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_AVIHdrParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_AVIStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_AVISetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_AVIGetInfo;
        _rSwdmxInfo.pfnSwdmxReadBuf = _SWDMX_AviReadBuffer;
        _rSwdmxInfo.pfnSwdmxReleaseBuf = _SWDMX_AviReleaseBuffer;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_AviAudioCb;
        break;
#endif
#ifndef CC_WMV_DISABLE
    case SWDMX_FMT_VC1_WMV:
    case SWDMX_FMT_WMA_AUDIO_ES:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);

        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_WMVSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_WMVEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_WMVDisableStrm;

        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_WMVInit;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_WMVSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_WMVHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_WMVStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_WMVSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_WMVGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_WMVAudioCb;
        break;
#endif
#ifndef CC_PS_DISABLE
    case SWDMX_FMT_MPEG1_DAT:
    case SWDMX_FMT_MPEG2_PS:
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MpgPsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_MpgPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_MpgPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MpgAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MpgStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MpgFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        _SWDMX_MpgSetSwdmxInfo(&_rSwdmxInfo);
        break;
#endif
#ifndef CC_TS_DISABLE
    case SWDMX_FMT_MPEG2_TS:
    case SWDMX_FMT_MPEG2_TS_192:
    case SWDMX_FMT_MPEG2_TS_ZERO_192:
    case SWDMX_FMT_MPEG2_TS_ENCRYPT_192:
    case SWDMX_FMT_MPEG2_TS_TIME_SHIFT:
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MpgTsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_MpgPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_MpgPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MpgAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MpgStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MpgFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        _SWDMX_MpgSetSwdmxInfo(&_rSwdmxInfo);
        break;
#endif
#ifndef CC_ES_DISABLE
#ifndef CC_ES_MPEG2_DISABLE
    case SWDMX_FMT_MPEG2_VIDEO_ES:
#endif
#ifndef CC_ES_MPEG4_DISABLE
    case SWDMX_FMT_MPEG4_VIDEO_ES:
#endif
//#ifndef CC_ES_VC1_DISABLE          //for VC1 ES file, when mw setVideo type , we notify "codec not support " to mw DTV00148208
    case SWDMX_FMT_VC1_ES:
//#endif
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MpgEsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_MpgPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_MpgPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MpgAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MpgStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MpgFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        _SWDMX_MpgSetSwdmxInfo(&_rSwdmxInfo);
        break;
#ifdef CC_FLASH_TEST
    case SWDMX_FMT_ES_WITH_PTS:
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_FlashEsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_FlashPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_FlashPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_FlashAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_FlashSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_FlashEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_FlashDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_FlashGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = NULL;//_SWDMX_FlashSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_FlashHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_FlashStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_FlashFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_FlashSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_FlashGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_FlashAudioCb;
        _SWDMX_FlashSetSwdmxInfo(&_rSwdmxInfo);
        break;
#endif
#ifndef CC_ES_H264_DISABLE
    case SWDMX_FMT_H264_VIDEO_ES:
#endif
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MpgEsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_MpgPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_MpgPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MpgAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MpgStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MpgFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        _SWDMX_MpgSetSwdmxInfo(&_rSwdmxInfo);
        break;
    case SWDMX_FMT_RAW_AUDIO_ES:
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MpgEsInit;
        _rSwdmxInfo.pfnSwdmxPlay = _SWDMX_MpgPlay;
        _rSwdmxInfo.pfnSwdmxPause = _SWDMX_MpgPause;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MpgAbort;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MpgSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MpgEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MpgDisableStrm;
        _rSwdmxInfo.pfnSwdmxGetStreamInfo = _SWDMX_MpgGetStreamInfo;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MpgSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MpgHeaderParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MpgStop;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MpgFlush;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MpgSetInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MpgGetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MpgAudioCb;
        _SWDMX_MpgSetSwdmxInfo(&_rSwdmxInfo);
        break;
#endif
#ifndef CC_MP4_DISABLE
    case SWDMX_FMT_MP4:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_Mp4Init;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_Mp4SetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_Mp4EnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_Mp4DisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_Mp4SetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_Mp4HdrPrs;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_Mp4Stop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_Mp4SetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_Mp4GetInfo;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_Mp4AudioCb;
        break;
#endif
#ifndef CC_MKV_DISABLE
    case SWDMX_FMT_MKV:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_MKVInit;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_MKVSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_MKVEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_MKVDisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_MKVSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_MKVHdrParse;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_MKVStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_MKVSetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_MKVGetInfo;
        _rSwdmxInfo.pfnSwdmxFlush = _SWDMX_MKVFlush;
        _rSwdmxInfo.pfnSwdmxAbort = _SWDMX_MKVFlush;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_MkvAudioCb;
        break;
#endif
#ifndef CC_RM_DISABLE
    case SWDMX_FMT_RM:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_RmInit;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_RmSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_RmEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_RmDisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_RmSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_RmHdrPrs;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_RmStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_RmSetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_RmGetInfo;
        //_rSwdmxInfo.pfnSwdmxFlush = _SWDMX_RmFlush;
        //_rSwdmxInfo.pfnSwdmxAbort = _SWDMX_RmFlush;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_RmAudioCb;
        break;
#endif
#ifndef CC_CMPB_DISABLE
    case SWDMX_FMT_CMPB:
        DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        _rSwdmxInfo.pfnSwdmxInit = _SWDMX_CmpbInit;
        _rSwdmxInfo.pfnSwdmxSetStrmID = _SWDMX_CmpbSetStrmID;
        _rSwdmxInfo.pfnSwdmxEnableStrm = _SWDMX_CmpbEnableStrm;
        _rSwdmxInfo.pfnSwdmxDisableStrm = _SWDMX_CmpbDisableStrm;
        _rSwdmxInfo.pfnSwdmxSetRange = _SWDMX_CmpbSetRange;
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_CmpbHdrPrs;
        _rSwdmxInfo.pfnSwdmxStop = _SWDMX_CmpbStop;
        _rSwdmxInfo.pfnSwdmxSetInfo = _SWDMX_CmpbSetDecoderInfo;
        _rSwdmxInfo.pfnSwdmxGetInfo = _SWDMX_CmpbGetInfo;
        //_rSwdmxInfo.pfnSwdmxFlush = _SWDMX_CmpbFlush;
        //_rSwdmxInfo.pfnSwdmxAbort = _SWDMX_CmpbFlush;
        _rSwdmxInfo.pfnSwdmxAudioCb = _SWDMX_CmpbAudioCb;
        break;
#endif

    default:
        LOG(0, "_SWDMX_Cfg false\n");
        return FALSE;
    }
    VDEC_SetMMParam(ES0,VDEC_MM_CONTAINER_TYPE,(UINT32)(_rSwdmxInfo.eCurFMT),0,0);
    _rSwdmxInfo.pfnSwdmxInit(_rSwdmxInfo.eCurFMT);

    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _SWDMX_DmxReqDataNotify
 *  Callback from the demuxer for reqesting video data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DmxReqDataNotify(UINT8 u1Idx)
{
    INT32 i4Ret;
    UINT32 u4ReqType = u1Idx;
    SIZE_T zMsgSize = sizeof(UINT32);

    Printf("_SWDMX_DmxReqDataNotify\n");

    i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &u4ReqType, zMsgSize, 255);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        //LOG(0, "SWDMX Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &u4ReqType, zMsgSize, 255);
    }

    VERIFY(i4Ret == OSR_OK);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Init
 *  Initialize software program stream demux driver
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Init(VOID)
{
    if (_rSwdmxInfo.fgSwDmxInit == FALSE)
    {
        LOG(7, "_SWDMX_Init()\n");

        // create data queue
        VERIFY(x_msg_q_create(&_hDmxFeederReqDataQ, "SWDMX_FEEDER_DATAQ", sizeof(FEEDER_REQ_DATA_T),
                              SWDMX_DATA_Q_SIZE) == OSR_OK);

        // create play command queue
        VERIFY(x_msg_q_create(&_hDmxPlayCmdQ, "SWDMX_PLAYCMDQ", sizeof(UINT32),
                              SWDMX_DATA_Q_SIZE) == OSR_OK);


        // create software demux main loop thread for command receiving
        VERIFY(x_thread_create(&_hSwDmxMainThread,
                               SWDMX_THREAD_NAME, SWDMX_THREAD_STACK_SIZE, SWDMX_THREAD_PRIORITY,
                               (x_os_thread_main_fct)_SWDMX_MainLoop, 0, NULL) == OSR_OK);

        x_memset(&_rSwdmxInfo, 0, sizeof(SWDMX_INFO_T));

        VERIFY(x_sema_create(&_hRangeMutex, X_SEMA_TYPE_MUTEX,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hFlushMutex, X_SEMA_TYPE_MUTEX,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_sema_create(&_hFlushSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_LOCK) == OSR_OK);

        VERIFY(x_sema_create(&_rSwdmxInfo.hRangeSema, X_SEMA_TYPE_BINARY,
                             X_SEMA_STATE_UNLOCK) == OSR_OK);

        VERIFY(x_timer_create(&_rSwdmxInfo.hWaitDataTimer) == OSR_OK);

        //#ifdef CC_DLNA_SUPPORT
        //VERIFY(x_sema_create(&_hFeederUnderFlowMutex, X_SEMA_TYPE_MUTEX,
        //    X_SEMA_STATE_UNLOCK) == OSR_OK);
        //#endif
#ifdef CC_DLNA_SUPPORT
        FeederSetInfo(FEEDER_SWDMX_UNDERFLOW_CB,(UINT32)_SWDMX_FeederUnderFlowCb);
        FeederSetInfo(FEEDER_SWDMX_STATUS_CB,(UINT32)_SWDMX_FeederStatusNfyCb);
#endif
        _rSwdmxInfo.i4PlaySpeed = SWDMX_PLAY_SPEED_1X;
        _rSwdmxInfo.u8SeekPos = 0xFFFFFFFF;
        _rSwdmxInfo.u8SeekPos <<= 32;
        _rSwdmxInfo.u8SeekPos += 0xFFFFFFFF;
        _rSwdmxInfo.fgSwDmxInit = TRUE;
        _rSwdmxInfo.fgRcvEOS = FALSE;
        _rSwdmxInfo.fgDataExhausted = TRUE;
        _rSwdmxInfo.fgWaitDataTimerStart = FALSE;

        if (!_rSwdmxInfo.pucSwdmxInternalBuf)
        {
            _rSwdmxInfo.pucSwdmxInternalBuf = 
                (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_INTERNAL_BUF_SIZE,16));    
            if (!_rSwdmxInfo.pucSwdmxInternalBuf)
            {
                LOG(3, "%s Fail to allocate swdmx internal buffer\n", __FUNCTION__);
            }
        }
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_GetStreamInfo
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_GetStreamInfo(ENUM_SWDMX_FMT_T eType,
                          UINT32 u4InfoMask,
                          UINT64 u8FileSize,
                          SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet = FALSE;
    _rSwdmxInfo.eCurFMT = eType;
    _rSwdmxInfo.u8FileSize = u8FileSize;
    _SWDMX_Cfg();
    if (_rSwdmxInfo.pfnSwdmxGetStreamInfo)
    {
        fgRet = _rSwdmxInfo.pfnSwdmxGetStreamInfo(eType, u4InfoMask, u8FileSize, prPInfo);
        if (prPInfo->rPgm[0].u4TotalTime)
        {
            _rSwdmxInfo.u4TotalTime = prPInfo->rPgm[0].u4TotalTime;
        }
    }
    return fgRet;
}

//-----------------------------------------------------------------------------
/** _SWDMX_SetContainerType
 *  Set the container type to SwDmx
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetContainerType(ENUM_SWDMX_FMT_T eContainerType)
{
    _rSwdmxInfo.eCurFMT = eContainerType;
    //jack
    //_DMX_SelectInputType(DMX_IN_PLAYBACK_MM);

    return _SWDMX_Cfg();
}


//-----------------------------------------------------------------------------
/** _SWDMX_Set_Play_Speed
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetSpeed(INT32 i4Speed)
{
    if (i4Speed > 1024000 || i4Speed < -1024000)
    {
        LOG(3, "_SWDMX_SetSpeed not in 1024000 -1024000\n");
        return FALSE;
    }
    // swdmx have to know -1 speed again, or we don't know it's new backstep.
    if ((_rSwdmxInfo.i4PlaySpeed != i4Speed) ||
            (i4Speed == -1))
    {
        LOG(5, "_SWDMX_SetSpeed from %d to %d\n", _rSwdmxInfo.i4PlaySpeed, i4Speed);
        _rSwdmxInfo.i4PlaySpeed = i4Speed;
        _rSwdmxInfo.fgPlayChange = TRUE;
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekTime
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekTime(UINT32 u4SeekTime, UINT64* pu8FilePos)
{
    UINT64 u8Remainder;

    _rSwdmxInfo.fgPlayChange = TRUE;

    if(pu8FilePos == NULL)
    {
        LOG(3, "_SWDMX_SeekTime pu8FilePos NULL\n");
        _rSwdmxInfo.u8SeekPos = _rSwdmxInfo.u8FileSize;
        return FALSE;
    }

    // we don't have file size for time-shift
    if ((_rSwdmxInfo.eCurFMT != SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
        (u4SeekTime > _rSwdmxInfo.u4TotalTime))
    {
        LOG(3, "_SWDMX_SeekTime u4SeekTime > u4TotalTime\n");
        _rSwdmxInfo.u8SeekPos = _rSwdmxInfo.u8FileSize;
        return FALSE;
    }

    *pu8FilePos = 0;
    // to decide time -> pos.
    if ((_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG1_DAT) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_PS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_H264_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_ES_WITH_PTS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_VC1_ES))
    {
        if (_rSwdmxInfo.u4TotalTime > 0)
        {
            *pu8FilePos = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset)*(u4SeekTime),
                                    (UINT64)(_rSwdmxInfo.u4TotalTime), &u8Remainder);
        }
        else
        {
            *pu8FilePos = 0;
        }
        *pu8FilePos += _rSwdmxInfo.u8FileOffset;
        VDEC_SetRenderFromFirstPic(ES0, TRUE, _rSwdmxInfo.u8FileSize);// PTS
        _rSwdmxInfo.u8SeekPos = *pu8FilePos;

       if(!_rSwdmxInfo.fgEnVideoDmx) //for audio only file, when do seek after pause ,needed set STC Value to updata time code
	  {
	      STC_StopStc();
             STC_SetStcValue(u4SeekTime);
	  }
    }
    else if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // u4SeekTime is tick
        //if(_SWDMX_MpgGetPosByTick(FALSE, u4SeekTime, pu8FilePos))
        if(_SWDMX_MpgGetPosByTick(TRUE, u4SeekTime, pu8FilePos))
        {
            _rSwdmxInfo.u8SeekPos = *pu8FilePos;
        }
        else
        {
            LOG(3, "_SWDMX_SeekTime _SWDMX_MpgGetPosByTick fail, do nothing\n");
            _rSwdmxInfo.fgPlayChange = FALSE;
            return FALSE;
        }
    }
    else if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(_SWDMX_MpgGetPosByTime(TRUE, u4SeekTime, pu8FilePos))
        {
        }
        else
        {
            if (_rSwdmxInfo.u4TotalTime > 0)
            {
                *pu8FilePos = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset)*(u4SeekTime),
                                        (UINT64)(_rSwdmxInfo.u4TotalTime), &u8Remainder);
            }
            else
            {
                *pu8FilePos = 0;
            }
            *pu8FilePos += _rSwdmxInfo.u8FileOffset;
        }
        STC_StopStc();
        STC_SetStcValue(u4SeekTime);
        _rSwdmxInfo.u8SeekPos = *pu8FilePos;
    }
    else
    {
        //for audio only file, when do seek after pause ,needed set STC Value to updata time code
        if(!_rSwdmxInfo.fgEnVideoDmx)
        {
            STC_StopStc();
            STC_SetStcValue(u4SeekTime);
        }
        _rSwdmxInfo.u8SeekPos = u4SeekTime;
    }

    LOG(5, "_SWDMX_SeekTime u4SeekTime %d u8FilePos 0x%llx  \n", u4SeekTime, (*pu8FilePos));

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SeekPos
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekPos(UINT64 u8SeekPos, UINT64 u8SeekPosI)
{
    UINT32 u4SeekTime;
    UINT64 u8Remainder;

    _rSwdmxInfo.fgPlayChange = TRUE;

    if((u8SeekPosI > _rSwdmxInfo.u8FileSize) ||
        (u8SeekPos > _rSwdmxInfo.u8FileSize))
    {
        LOG(3, "_SWDMX_Seek u8SeekPosI or u8SeekPos > u8FileSize\n");
        _rSwdmxInfo.u8SeekPos = _rSwdmxInfo.u8FileSize;
        return FALSE;
    }

    if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        _rSwdmxInfo.u8SeekPos = (u8SeekPos*MPG_LBA_BLOCK_SIZE);
    }
    else if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)

    {
        u4SeekTime = _SWDMX_Div6464(u8SeekPos*(_rSwdmxInfo.u4TotalTime),
                               (UINT64)(_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset), &u8Remainder);
        STC_StopStc();
        STC_SetStcValue(u4SeekTime);
    }
    else
    {
        VDEC_SetRenderFromFirstPic(ES0, TRUE, _rSwdmxInfo.u8FileSize);// PTS
    }

    _rSwdmxInfo.u8SeekPos = u8SeekPosI;

    LOG(5, "_SWDMX_SeekPos PosI:0x%08X Pos:0x%08X\n", u8SeekPosI, u8SeekPos);

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_SeekPercent
 *  Set position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SeekPercent(UINT32 u4SeekPercent, UINT64* pu8FilePos)
{
    UINT32 u4SeekTime;
    UINT64 u8Remainder;

    _rSwdmxInfo.fgPlayChange = TRUE;

    if(pu8FilePos == NULL)
    {
        LOG(3, "_SWDMX_SeekPercent pu8FilePos NULL\n");
        _rSwdmxInfo.u8SeekPos = _rSwdmxInfo.u8FileSize;
        return FALSE;
    }

    if (u4SeekPercent > 100)
    {
        LOG(3, "_SWDMX_SeekPercent u4SeekPercent > 100, == 0\n");
        _rSwdmxInfo.u8SeekPos = _rSwdmxInfo.u8FileSize;
        return FALSE;
    }

    *pu8FilePos = 0;
    // to decide time -> pos.
    if ((_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG1_DAT) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_PS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_H264_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_ES_WITH_PTS) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (_rSwdmxInfo.eCurFMT == SWDMX_FMT_VC1_ES))
    {
        *pu8FilePos = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        *pu8FilePos += _rSwdmxInfo.u8FileOffset;
        VDEC_SetRenderFromFirstPic(ES0, TRUE, _rSwdmxInfo.u8FileSize);// PTS
    }
	else if(_rSwdmxInfo.eCurFMT == SWDMX_FMT_FLV)
	{
        *pu8FilePos = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        u4SeekTime=_SWDMX_Div6464(_rSwdmxInfo.u4TotalTime*u4SeekPercent,100,&u8Remainder);
	}
    else if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // Isaac
    }
    else if (_rSwdmxInfo.eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
    {
        *pu8FilePos = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset)*(u4SeekPercent),
                                (UINT64)(100), &u8Remainder);
        *pu8FilePos += _rSwdmxInfo.u8FileOffset;
        u4SeekTime = _SWDMX_Div6464((*pu8FilePos)*(_rSwdmxInfo.u4TotalTime),
                               (UINT64)(_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset), &u8Remainder);
        STC_StopStc();
        STC_SetStcValue(u4SeekTime);
    }
    else
    {
        // please decide your pts here.
        VDEC_SetRenderPts(ES0, 0);// PTS
    }

    LOG(5, "_SWDMX_SeekPercent %d u8FilePos -> (INT32)%d  \n", u4SeekPercent, (UINT32)(*pu8FilePos));

    _rSwdmxInfo.u8SeekPos = *pu8FilePos;
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_RenderFromPos
 *  Render from position
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_RenderFromPos(VOID *prPosition)
{
#ifdef CC_SWDMX_RENDER_POSITION
    START_POS_INFO_T *prPos = (START_POS_INFO_T*)prPosition;
    if(!prPos)
    {
        LOG(3, "_SWDMX_RenderFromPos prPosition null\n");
        return FALSE;
    }
    LOG(5, "_SWDMX_RenderFromPos Render Pos 0x%llx\n", prPos->ui8_frame_position);
    _rCurRangeReq.i4TempRef = prPos->i4_temporal_reference;
    _rCurRangeReq.u2DecodingOrder = prPos->ui2_decoding_order;
    _rCurRangeReq.u8Offset = prPos->ui8_frame_position;
    _rCurRangeReq.u8OffsetI = prPos->ui8_i_frame_position;
    _rCurRangeReq.u8StartPts = prPos->ui8_pts_info;
    _rCurRangeReq.fgSetRenderPosition = TRUE;
#endif
    return TRUE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Set_Video_StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetVideoStreamID(UINT32 u4VidStrmID)
{
    _rSwdmxInfo.u4VidStrmID = u4VidStrmID;

    if (_rSwdmxInfo.pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetVideoStreamID = %08x\n", u4VidStrmID);
        return _rSwdmxInfo.pfnSwdmxSetStrmID(eSWDMX_STRM_TYPE_VID, u4VidStrmID, 0);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** SWDMX_Set_Audio_StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetAudioStreamID(UINT32 u4AudStrmID, UINT32 u4AudStrmSubID)
{
    _rSwdmxInfo.u4AudStrmID = u4AudStrmID;
    _rSwdmxInfo.u4AudSubStrmID = u4AudStrmSubID;

    if (_rSwdmxInfo.pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetAudioStreamID = %08x, sub = %08x\n", u4AudStrmID, u4AudStrmSubID);
        return _rSwdmxInfo.pfnSwdmxSetStrmID(eSWDMX_STRM_TYPE_AUD, u4AudStrmID, u4AudStrmSubID);
    }

    return FALSE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_SetAudio2StreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetAudio2StreamID(UINT32 u4AudStrmID)
{
    _rSwdmxInfo.u4Aud2StrmID = u4AudStrmID;

    if (_rSwdmxInfo.pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetAudio2StreamID = %08x\n", u4AudStrmID);
        return _rSwdmxInfo.pfnSwdmxSetStrmID(eSWDMX_STRM_TYPE_AUD2, u4AudStrmID, 0);
    }

    return FALSE;
}
//-----------------------------------------------------------------------------
/** _SWDMX_SetSubTitleStreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetSubTitleStreamID(UINT32 u4SubTitleStrmID)
{
    /*_rSwdmxInfo.fgEnSubTitleDmx = TRUE;*/
    _rSwdmxInfo.u4SubTitleStrmID = u4SubTitleStrmID;
    if (_rSwdmxInfo.pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetSubtitleStreamID = %08x\n", u4SubTitleStrmID);
        return _rSwdmxInfo.pfnSwdmxSetStrmID(eSWDMX_STRM_TYPE_SUBTITLE, u4SubTitleStrmID, 0);
    }

    return TRUE;
}
//-----------------------------------------------------------------------------
/** _SWDMX_SetPcrStreamID
 *  Set the Container Type
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_SetPcrStreamID(UINT32 u4PcrStrmID)
{
    _rSwdmxInfo.u4PcrStrmID = u4PcrStrmID;
    if (_rSwdmxInfo.pfnSwdmxSetStrmID != 0)
    {
        LOG(7, "_SWDMX_SetPcrStreamID = %08x\n", u4PcrStrmID);
        return _rSwdmxInfo.pfnSwdmxSetStrmID(eSWDMX_STRM_TYPE_PCR, u4PcrStrmID, 0);
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Play
 *  Start to run software program stream demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Play(VOID)
{
    if (!_rSwdmxInfo.fgSwDmxInit)
    {
        LOG(1, "_SWDMX_Play fgSwDmxInit fail\n");
    }

    LOG(5, "_SWDMX_Play()\n");
    _SWDMX_SendPlayCmd(eSWDMX_STATE_PLAY);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Pause
 *  Pause software demux
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Pause(VOID)
{
    INT32 i4Ret;
    UINT16 u2QueueSize;
    FEEDER_REQ_DATA_T rFeederReqData;
    SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);

    if (_rSwdmxInfo.fgSwDmxInit == TRUE)
    {
        LOG(5, "_SWDMX_Pause()\n");

        _SWDMX_SendPlayCmd(eSWDMX_STATE_PAUSE);

        VERIFY(x_msg_q_num_msgs(_hDmxFeederReqDataQ, &u2QueueSize) == OSR_OK);

        if (u2QueueSize == 0)
        {
            rFeederReqData.eDataType = FEEDER_SOURCE_INVALID;
            i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
            VERIFY(i4Ret == OSR_OK);
        }

        if (_rSwdmxInfo.pfnSwdmxAbort)
        {
            _rSwdmxInfo.pfnSwdmxAbort();
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Stop
 *  Stop software PS parser
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Stop(VOID)
{
    if (_rSwdmxInfo.fgSwDmxInit == TRUE)
    {
        INT32 i4Ret;
        FEEDER_REQ_DATA_T rFeederReqData;
        SIZE_T zMsgSize = sizeof(FEEDER_REQ_DATA_T);

        LOG(5, "_SWDMX_Stop()\n");

        _SWDMX_SendPlayCmd(eSWDMX_STATE_STOP);

        // To prevent the hang problem in wait feeder's ack,
        // so I send a dummy msg to main loop

        //if(_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_PLAY)
        //{
        rFeederReqData.eDataType = FEEDER_SOURCE_INVALID;
        i4Ret = x_msg_q_send(_hDmxFeederReqDataQ, &rFeederReqData, zMsgSize, 255);
        VERIFY(i4Ret == OSR_OK);
        //}

#if 0
        AUD_Mute(0);
        AUD_MMAoutEnable(0, TRUE);
        AUD_UnMute(0);
#endif
#ifdef CC_DLNA_SUPPORT
        _fgFeederUnderFlow = FALSE;
#endif
        if (_rSwdmxInfo.pfnSwdmxAbort)
        {
            _rSwdmxInfo.pfnSwdmxAbort();
        }

#ifdef CC_SWDMX_RENDER_PTS
        _u4RenderPts = 0;
#endif
#if 0//def CC_SWDMX_RENDER_POSITION
        {
            INT32 i4TempRef;
            UINT16 u2DecodingOrder;
            UINT64 u8Offset;
            UINT64 u8OffsetI;
            UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
            LOG(3, "Temporal Reference = %d\n", i4TempRef);
            LOG(3, "Decoding Order = %d\n", u2DecodingOrder);
            LOG(3, "File Offset = %lld\n", u8Offset);
            LOG(3, "File Offset I = %lld\n", u8OffsetI);
            LOG(3, "( %d %d %lld %lld )\n", i4TempRef, u2DecodingOrder, u8Offset, u8OffsetI);
        }

        _fgSetRenderPosition = FALSE;
        _i4TempRef = 0;
        _u2DecodingOrder = 0;
        _u8Offset = 0;
        _u8OffsetI = 0;
#endif
    }
    else
    {
        x_memset((void*)&_rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        return FALSE;
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Flush
 *  Send cmd to Flush A/V data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_Flush(VOID)
{
    LOG(5, "_SWDMX_Flush()\n");
    _SWDMX_LockFlushMutex();
    _rSwdmxInfo.fgFlush = TRUE;
    _SWDMX_UnlockFlushMutex();

    _SWDMX_SendPlayCmd(eSWDMX_STATE_MAX);
    if (_rSwdmxInfo.pfnSwdmxAbort)
    {
        _rSwdmxInfo.pfnSwdmxAbort();
    }

    _SWDMX_LockFlushSema();
    LOG(5, "_SWDMX_Flush Done\n");
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_DoFlush
 *  Flush A/V data
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DoFlush(VOID)
{
    if (_rSwdmxInfo.fgEnVideoDmx)
    {
        // To not block decoder block by get frame buffer.
        VDEC_ReleaseDispQ(ES0);

        MPV_FlushEsmQ(ES0, FALSE);

        VDEC_ReleaseDispQ(ES0);
        VDP_ResetPts();
    }

    if (_rSwdmxInfo.fgEnAudioDmx)
    {
        AUD_WaitDspFlush(INTERNAL_DAC_ID_MAIN);;
    }

    // container part flush
    if (_rSwdmxInfo.pfnSwdmxFlush)
    {
        _rSwdmxInfo.pfnSwdmxFlush();
    }

    return TRUE;
}

ENUM_SWDMX_PLAY_STATE_T _SWDMX_QueryStatus(VOID)
{
    return _rSwdmxInfo.eSwDmxStatus;
}


BOOL _SWDMX_SetRangeCb(VOID* pfnCbFunc)
{
    _rSwdmxInfo.pfnSwdmxRangeCb = (x_mm_range_nfy_fct)pfnCbFunc;
    return TRUE;
}

//-----------------------------------------------------------------------------
/** _SWDMX_Add_Range
 *  Add the demux range
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_AddRange(MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID)
{
    LOG(11, "Enter _SWDMX_AddRange\n");

    VERIFY(x_sema_lock(_hRangeMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if (!prDmxRange)
    {
        LOG(3, "_SWDMX_AddRange Range is NULL\n");
        VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);
        return FALSE;
    }

    //set whole file total duration
    if (prDmxRange->ui8_pb_duration > 0)
    {
        _rSwdmxInfo.u4TotalTime = (UINT32)prDmxRange->ui8_pb_duration;
    }

    if (prDmxRange->z_range_sz > 0)
    {
        _rSwdmxInfo.u8FileSize = (prDmxRange->z_range_sz + prDmxRange->ui8_fileoffset);
    }

#ifdef CC_SWDMX_RENDER_PTS
    prDmxRange->t_vid_start_render_pts = _u4RenderPts;
#endif

    _rSwdmxInfo.u8FileOffset = prDmxRange->ui8_fileoffset;
    if(_rSwdmxInfo.u8FileOffset >= _rSwdmxInfo.u8FileSize)
    {
        if(_rSwdmxInfo.u8FileSize > 0)
        {
            _rSwdmxInfo.u8FileOffset = (_rSwdmxInfo.u8FileSize - 1);
        }
        else
        {
            _rSwdmxInfo.u8FileOffset = 0;
        }
        LOG(3, "ui8_fileoffset >= z_range_sz\n");
    }

    if ((_rCurRangeReq.fgIsFull) &&
            (_rQRangeReq.fgIsFull))
    {
        LOG(5, "_SWDMX_AddRange Range is full\n");
        VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);
        return FALSE;
    }
    else if (_rCurRangeReq.fgIsFull)
    {
        LOG(5, "_SWDMX_AddRange Add to Q Range, Tag = 0x%08x\n", u4TagID);
        x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        _rQRangeReq.u4RangeId = prDmxRange->ui4_range_id;
        _rQRangeReq.prDmxRangeInfo = prDmxRange;
        _rQRangeReq.fgIsFull = TRUE;
    }
    else
    {
        LOG(5, "_SWDMX_AddRange Add to Cur Range, Tag = 0x%08x\n", u4TagID);
        x_memset((void*)&_rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
        _rCurRangeReq.u4RangeId = prDmxRange->ui4_range_id;
        _rCurRangeReq.prDmxRangeInfo = prDmxRange;
        _rCurRangeReq.fgIsFull = TRUE;

#ifdef CC_SWDMX_RENDER_POSITION
        if((prDmxRange->t_start_pos.i4_temporal_reference > 0) ||
           (prDmxRange->t_start_pos.ui2_decoding_order > 0) ||
           (prDmxRange->t_start_pos.ui8_frame_position > 0) ||
           (prDmxRange->t_start_pos.ui8_i_frame_position > 0))
        {
            LOG(5, "_SWDMX_AddRange Render Pos\n", u4TagID);
            //_rCurRangeReq.fgSetRenderPosition = TRUE;
            _rCurRangeReq.i4TempRef = prDmxRange->t_start_pos.i4_temporal_reference;
            _rCurRangeReq.u2DecodingOrder = prDmxRange->t_start_pos.ui2_decoding_order;
            _rCurRangeReq.u8Offset = prDmxRange->t_start_pos.ui8_frame_position;
            _rCurRangeReq.u8OffsetI = prDmxRange->t_start_pos.ui8_i_frame_position;
            _rCurRangeReq.u8StartPts = prDmxRange->t_start_pos.ui8_pts_info;
            _rCurRangeReq.fgSetRenderPosition = TRUE;
        }
#endif

#if 0//def CC_SWDMX_RENDER_POSITION
        if (_fgSetRenderPosition)
        {
            _rCurRangeReq.fgSetRenderPosition = TRUE;
            _rCurRangeReq.i4TempRef = _i4TempRef;
            _rCurRangeReq.u2DecodingOrder = _u2DecodingOrder;
            _rCurRangeReq.u8Offset = _u8Offset;
            _rCurRangeReq.u8OffsetI = _u8OffsetI;
        }
#endif

        VDEC_SetMMParam(ES0,VDEC_MM_SRC_TYPE,(UINT32)(_rCurRangeReq.prDmxRangeInfo->e_mm_src_type),0,0);

//#ifdef CC_DLNA_SUPPORT
        if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA)
        {
            FeederSetInfo(FEEDER_SWDMX_SRC_TYPE,SWDMX_SRC_TYPE_NETWORK_DLNA);
        }
        else if(prDmxRange->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            FeederSetInfo(FEEDER_SWDMX_SRC_TYPE,SWDMX_SRC_TYPE_NETWORK_NETFLIX);
        }
        else
        {
            FeederSetInfo(FEEDER_SWDMX_SRC_TYPE,SWDMX_SRC_TYPE_HIGH_SPEED_STORAGE);
        }
//#endif

        if (prDmxRange->ui4_vid_render_counts)
        {
            VDEC_SetRenderPicCount(ES0, TRUE, prDmxRange->ui4_vid_render_counts);
        }

        if (_rSwdmxInfo.pfnSwdmxSetRange &&
             ((_rSwdmxInfo.eCurFMT==SWDMX_FMT_FLV)||
             (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MP4) ||
             (_rSwdmxInfo.eCurFMT == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)))
        {
            _rSwdmxInfo.pfnSwdmxSetRange(&_rCurRangeReq, _rSwdmxInfo.u4VidStrmID,
                                         _rSwdmxInfo.u4AudStrmID, _rSwdmxInfo.u4SubTitleStrmID);
        }
    }

    VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _SWDMX_Add_Range
 *  Add the demux range
 *
 *  @retval BOOL
 */
//-----------------------------------------------------------------------------
BOOL _SWDMX_DelRange(MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID, BOOL fgDelAllRange)
{
    VERIFY(x_sema_lock(_hRangeMutex, X_SEMA_OPTION_WAIT) == OSR_OK);

    if(fgDelAllRange)              //DTV00148327 before thumb nail stop, we should delete all the range info
    {
 
       LOG(5,"_SWDMX_DelRange delete all range\n");
	   x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
	   x_memset((void*)&_rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));	   
	   
	   VDEC_SetRenderPicCount(ES0, FALSE, 0);
	   
	   VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);
	   
       return TRUE;
    }

    if ((_rQRangeReq.fgIsFull) &&
            (((UINT32)_rQRangeReq.u4RangeId) == u4TagID))
    {
        x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
    }
    else if ((_rCurRangeReq.fgIsFull) &&
             (((UINT32)_rCurRangeReq.u4RangeId) == u4TagID))
    {
        if ((_rSwdmxInfo.eSwDmxStatus == eSWDMX_STATE_PLAY) &&
                (_rCurRangeReq.fgInUse))
        {
            LOG(5, "_SWDMX_DelRange Cur Range is in use, id = 0x%08x\n", u4TagID);
            VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);
            return FALSE;
        }
        else
        {
            LOG(5, "_SWDMX_DelRange Cur Range, id = 0x%08x\n", u4TagID);
            x_memset((void*)&_rCurRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
            // Swap Range
            if (_rQRangeReq.fgIsFull)
            {
                x_memcpy((void*)&_rCurRangeReq, (void*)&_rQRangeReq, sizeof(SWDMX_RANGE_LIST_T));
                x_memset((void*)&_rQRangeReq, 0, sizeof(SWDMX_RANGE_LIST_T));
            }
        }
    }
    if ((_rSwdmxInfo.pfnSwdmxSetRange) && (_rCurRangeReq.fgIsFull))
    {
        LOG(5, "Set Range tagID=0x%08x\n", _rCurRangeReq.u4RangeId);
        _rSwdmxInfo.pfnSwdmxSetRange(&_rCurRangeReq, _rSwdmxInfo.u4VidStrmID,
                                     _rSwdmxInfo.u4AudStrmID, _rSwdmxInfo.u4SubTitleStrmID);
    }
    else
    {
        LOG(5, "Set Range err full %d\n", _rCurRangeReq.fgIsFull);
    }

    VERIFY(x_sema_unlock(_hRangeMutex) == OSR_OK);
    return TRUE;
}

BOOL _SWDMX_GetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    switch (u1Type)
    {
    case eSWDMX_TOTAL_DURATION:
        if (pu4Para1 == NULL)
        {
            LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
            return FALSE;
        }
        *pu4Para1 = _rSwdmxInfo.u4TotalTime;
        return TRUE;

    case eSWDMX_GET_CURRENT_POSITION:
    case eSWDMX_GET_CURRENT_TIME:
    case eSWDMX_SET_CHK_PSI_BUF_SIZE:
    case eSWDMX_GET_IS_TRICK:
    case eSWDMX_GET_CURRENT_TICK:
    case eSWDMX_GET_CURRENT_LBA:
    case eSWDMX_GET_STC_OFFSET:
        if (_rSwdmxInfo.pfnSwdmxGetInfo) {
            return _rSwdmxInfo.pfnSwdmxGetInfo(u1Type,pu4Para1,pu4Para2,pu4Para3);
        }
        break;
    case eSWDMX_CONTAINER_TYPE:
        if (pu4Para1 == NULL)
        {
            LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)_rSwdmxInfo.eCurFMT;
        return TRUE;
    case eSWDMX_GET_INTERNAL_INFO:
        *pu4Para1 = (UINT32)&_rSwdmxInfo;
        return TRUE;

    case eSWDMX_GET_FILE_OFFSET:
        {
            UINT64 *pu8Tmp;
            if (pu4Para1 == NULL)
            {
                LOG(3, "_SWDMX_GetInfo pu4Para1 null\n");
                return FALSE;
            }
            pu8Tmp = (UINT64*)(VOID*)pu4Para1;
            *pu8Tmp = 0;
            if(_rSwdmxInfo.eCurFMT == SWDMX_FMT_RAW_AUDIO_ES)
            {
                *pu8Tmp = _rSwdmxInfo.u8FileOffset;
            }
        }
        return TRUE;

    case eSWDMX_GET_INTERNAL_BUF:
        if (_rSwdmxInfo.pucSwdmxInternalBuf)
        {
            *pu4Para1 = (UINT32)_rSwdmxInfo.pucSwdmxInternalBuf;
            return TRUE;
        }
        break;
    #if 0
    case eSWDMX_GET_CURRENT_TIME:
        {
            UINT64 u8Tmp;
            UINT64 u8CurPos;
            UINT64 u8Remainder;

            u8CurPos = 0;
            *pu4Para1 = 0;
            if (_rSwdmxInfo.pfnSwdmxGetInfo)
            {
                if(!_rSwdmxInfo.pfnSwdmxGetInfo(eSWDMX_GET_CURRENT_POSITION, (UINT32*)&u8CurPos, 0, 0))
                {
                    return FALSE;
                }
            }
            else
            {
                return FALSE;
            }
            if ((_rSwdmxInfo.u4TotalTime > 0) &&
                    (_rSwdmxInfo.u8FileSize > 0))
            {
                // 90000->90000
                u8Tmp = _SWDMX_Div6464((_rSwdmxInfo.u8FileSize - _rSwdmxInfo.u8FileOffset)*90000,
                                  _rSwdmxInfo.u4TotalTime,
                                  &u8Remainder);
                if(u8Tmp > 0)
                {
                    *pu4Para1 = (UINT32)_SWDMX_Div6464(u8CurPos*90000,
                                               (UINT32)u8Tmp, &u8Remainder);
                }
                UNUSED(u8Remainder);
            }
            return TRUE;
        }
    #endif

    default:
        break;
    }

    return FALSE;
}

BOOL _SWDMX_SetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
    BOOL fgRet;

    switch (u1Type)
    {
    case eSWDMX_SET_VIDEOTYPE:
    case eSWDMX_SET_AUDIOTYPE:
    case eSWDMX_SET_VIDSEQHEADER:
    case eSWDMX_SET_TPT_VIDSEQHEADER:
    case eSWDMX_SET_CHK_PSI_BUF_SIZE:
    case eSWDMX_SET_REC_BUF_RANGE:
    case eSWDMX_PRINT_REC_BUF_STATUS:
    case eSWDMX_PRINT_INTERNAL_STATUS:
    case eSWDMX_SET_MONITOF_LBA:
    case eSWDMX_SET_NETFLIX_STREAM_ID:
    case eSWDMX_SET_SYSTEMINFO:
    case eSWDMX_PID_INDEX_START:
    case eSWDMX_SET_VIDEO_DECRYPT:
    case eSWDMX_SET_AUDIO_DECRYPT:
    case eSWDMX_SET_VID_PID_IDX:
    case eSWDMX_SET_AUD_PID_IDX:
    case eSWDMX_SET_PCR_PID_IDX:
    case eSWDMX_SET_NETFLIX_EOS:
        if (_rSwdmxInfo.pfnSwdmxSetInfo) {
            return _rSwdmxInfo.pfnSwdmxSetInfo(u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
    case eSWDMX_ENABLE_STREAM:
        if (_rSwdmxInfo.pfnSwdmxEnableStrm != NULL)
        {
            if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
            {
                VDEC_SetMMParam(ES0,VDEC_MM_CONTAINER_TYPE,(UINT32)(_rSwdmxInfo.eCurFMT),0,0);
            }
            fgRet = _rSwdmxInfo.pfnSwdmxEnableStrm((UINT8)u4Para1, (void*)u4Para2);
            if (fgRet)
            {
                if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
                {
                    _rSwdmxInfo.fgEnVideoDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_AUD)
                {
                    _rSwdmxInfo.fgEnAudioDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_SUBTITLE)
                {
                    _rSwdmxInfo.fgEnSubTitleDmx = TRUE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_PCR)
                {
                    _rSwdmxInfo.fgEnPcrDmx = TRUE;
                }
            }
            return fgRet;
        }
        break;
    case eSWDMX_DISABLE_STREAM:
        if (_rSwdmxInfo.pfnSwdmxDisableStrm != NULL)
        {
            fgRet =  _rSwdmxInfo.pfnSwdmxDisableStrm((UINT8)u4Para1);
            if (fgRet)
            {
                if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_VID)
                {
                    _rSwdmxInfo.fgEnVideoDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_AUD)
                {
                    _rSwdmxInfo.fgEnAudioDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_SUBTITLE)
                {
                    _rSwdmxInfo.fgEnSubTitleDmx = FALSE;
                }
                else if (u4Para1 == (UINT32)eSWDMX_STRM_TYPE_PCR)
                {
                    _rSwdmxInfo.fgEnPcrDmx = FALSE;
                }
            }
            return fgRet;
        }
        break;
    case eSWDMX_SET_RANGECALLBACK:
        _SWDMX_SetRangeCb((VOID*)u4Para1);
        break;
    case eSWDMX_TOTAL_DURATION:
        _rSwdmxInfo.u4TotalTime = u4Para1;
        if (_rSwdmxInfo.pfnSwdmxSetInfo) {
            return _rSwdmxInfo.pfnSwdmxSetInfo(u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;
#if 0
    case eSWDMX_SET_FEEDER_TEST_FUNCTION:
        _rSwdmxInfo.pfnSwdmxHdrParse = _SWDMX_HdrParseFeederTest;
        break;
#endif
    case eSWDMX_SET_AOUT_ENABLE:
        AUD_MMAoutEnable(0, TRUE);
        if (_rSwdmxInfo.pfnSwdmxSetInfo) {
            return _rSwdmxInfo.pfnSwdmxSetInfo(u1Type,u4Para1,u4Para2,u4Para3);
        }
        break;

    case eSWDMX_SET_EOS:
        _rSwdmxInfo.fgRcvEOS = (BOOL)u4Para1;
        break;

    default:
        LOG(5, "unknow type %d\n", (UINT32)u1Type);
        break;
    }

    return FALSE;
}

VOID _SWDMX_QueryRangeInfo(SWDMX_RANGE_INFO_T* prRangeInfo)
{
    if (prRangeInfo != NULL)
    {
        x_memset(prRangeInfo, 0, sizeof(SWDMX_RANGE_INFO_T));
        if (_rCurRangeReq.fgIsFull)
        {
            prRangeInfo->u4CurRangeID = _rCurRangeReq.u4RangeId;
            prRangeInfo->u8CurRangePTS =
                (UINT64)_rCurRangeReq.prDmxRangeInfo->t_vid_start_pts;
        }
        if (_rQRangeReq.fgIsFull)
        {
            prRangeInfo->u4QRangeID = _rQRangeReq.u4RangeId;
            prRangeInfo->u8QRangePTS =
                (UINT64)_rQRangeReq.prDmxRangeInfo->t_vid_start_pts;
        }
    }
}

INT32 _SWDMX_SetDmxNfy(const SWDMX_NFY_INFO_T* prSwdmxNfyInfo)
{
    if (prSwdmxNfyInfo == NULL)
    {
        return (SWDMX_DRV_INV_SET_INFO);
    }

    _rSwdmxInfo.pvSwdmxNfyTag = prSwdmxNfyInfo->pvTag;
    _rSwdmxInfo.pfSwdmxNfyFct = prSwdmxNfyInfo->pfSwdmxNfy;

    return (SWDMX_DRV_OK);
}


BOOL _SWDMX_ReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    if (_rSwdmxInfo.pfnSwdmxReadBuf)
    {
        return _rSwdmxInfo.pfnSwdmxReadBuf(eStrmType, prReadInfo);
    }

    return FALSE;
}

BOOL _SWDMX_ReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo)
{
    if (_rSwdmxInfo.pfnSwdmxReleaseBuf)
    {
        return _rSwdmxInfo.pfnSwdmxReleaseBuf(eStrmType, prReadInfo);
    }

    return FALSE;
}


BOOL _SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if (_rSwdmxInfo.pfnSwdmxAudioCb != NULL)
    {
        return _rSwdmxInfo.pfnSwdmxAudioCb(eAudioNotifyType);
    }
    return TRUE;
}

BOOL _SWDMX_PcrCallBack(UINT32 u4PcrBase)
{
    return _SWDMX_MpgPcrCallBack(u4PcrBase);
}

UINT16 _SWDMX_GetNumOfCmd(VOID)
{
    UINT16 u2Count = 0;

    VERIFY(x_msg_q_num_msgs(_hDmxPlayCmdQ, &u2Count) == OSR_OK);

    return u2Count;
}

void _SWDMX_LockRangeSema(VOID)
{
    if(_rSwdmxInfo.hRangeSema)
    {
        VERIFY(x_sema_lock(_rSwdmxInfo.hRangeSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
}

void _SWDMX_UnlockRangeSema(VOID)
{
    if(_rSwdmxInfo.hRangeSema)
    {
        VERIFY(x_sema_unlock(_rSwdmxInfo.hRangeSema) == OSR_OK);
    }
}

#ifdef CC_SWDMX_RENDER_PTS
EXTERN void _SWDMX_SetRenderPts(UINT32 u4RenderPts)
{
    _u4RenderPts = u4RenderPts;
}
#endif

#if 0//def CC_SWDMX_RENDER_POSITION
EXTERN VOID _SWDMX_GetRenderPosition(INT32 *pi4TempRef, UINT16 *pu2DecodingOrder, UINT64 *pu8Offset, UINT64 *pu8OffsetI)
{
    *pi4TempRef = _i4TempRef;
    *pu2DecodingOrder = _u2DecodingOrder;
    *pu8Offset = _u8Offset;
    *pu8OffsetI = _u8OffsetI;
}


EXTERN VOID _SWDMX_SetRenderPosition(INT32 i4TempRef, UINT16 u2DecodingOrder, UINT64 u8Offset, UINT64 u8OffsetI)
{
    _fgSetRenderPosition = TRUE;
    _i4TempRef = i4TempRef;
    _u2DecodingOrder = u2DecodingOrder;
    _u8Offset = u8Offset;
    _u8OffsetI = u8OffsetI;
}
#endif

UINT64 _SWDMX_Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder)
{
    if(u8Dividend == 0)
    {
        *pu8Remainder = 0;
        return 0;
    }
    if (u8Divider >> 32)
    {
        return u8Div6464(u8Dividend, u8Divider, pu8Remainder);
    }
    return u8Div6432(u8Dividend, u8Divider, pu8Remainder);
}

