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
 * $Author: weibo.li $
 * $Date: 2013/01/19 $
 * $RCSfile: swdmx_mpg.c,v $
 * $Revision: #16 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_avi.c
 *  Software demux for avi - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#define SWDMX_MPG_SYNC_DTV_MODE

#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "aud_if.h"
#include "swdmx_drvif.h"
#include "swdmx_mpg.h"
#include "swdmx_mpg_dmx.h"
#include "swdmx_mpg_pvr.h"
#include "swdmx_mpg_psipsr_if.h"
#include "fbm_drvif.h"
#include "x_util.h"
#include "swdmx_debug.h"
#include "swdmx_mpg_idx.h"
#ifdef SWDMX_MPG_SYNC_DTV_MODE
#include "stc_drvif.h"
#endif

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SWDMX_MPG_DMX_20

//#define MPG_CHECK_FEEDER_REQ
#define PVR_START_OFFSET


#define SWDMX_FEEDER_ID_GAP (5000)
#define SWDMX_AUDIO_FULL_PTS_INTERVAL (90000*2)

#define IS_NETWORK_SOURCE(x)   ((x == MM_SRC_TYPE_NETWORK_VUDU) || (x == MM_SRC_TYPE_NETWORK_CMPB) \
                             || (x == MM_SRC_TYPE_NETWORK_DLNA) || (x == MM_SRC_TYPE_PUSH)   \
                             || (x == MM_SRC_TYPE_NETWORK_XUNLEI) /*|| (x == MM_SRC_TYPE_NETWORK_NETFLIX)*/)

#define IS_PVR(x)              ((x == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) \
                             || (x == SWDMX_FMT_MPEG2_TS_192))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static VDEC_SEQUENCE_DATA_T _arSeqInfo[SWDMX_SOURCE_MAX];
static UCHAR ucCount = 0;

//#ifdef CC_VUDU_SUPPORT
static DMX_VUDU_KEY_T _arVuduKey[SWDMX_SOURCE_MAX][MPG_BUF_NUM_MAX];
//#endif

#ifdef PVR_START_OFFSET
    static BOOL _fgPvrStartOffset = FALSE;
#endif

//#define SWDMX_MPG_DEBUG_FIFO
#ifdef SWDMX_MPG_DEBUG_FIFO
#define SWDMX_MPG_AFIFO_SIZE 0x200000
#define SWDMX_MPG_DMXFIFO_SIZE 0x500000
#ifdef SWDMX_MPG_AFIFO_SIZE
BOOL _fgAManual = TRUE;
BOOL _fgAFifo = FALSE;
UINT32 _u4AFifoStart = 0;
UINT32 _u4AFifoEnd = 0;
UINT32 _u4AFifoCur = 0;
UINT32 _u4AudioLastWriter = 0;
#endif
#ifdef SWDMX_MPG_DMXFIFO_SIZE
BOOL _fgDmxManual = TRUE;
BOOL _fgDmxFifo = FALSE;
UINT32 _u4DmxFifoStart = 0;
UINT32 _u4DmxFifoEnd = 0;
UINT32 _u4DmxFifoCur = 0;
#endif
VOID _SWDMX_COPY_RING_BUF(
    UINT32 u4TarStart,
    UINT32 u4TarEnd,
    UINT32* pu4TarCur,
    UINT32 u4SrcStart,
    UINT32 u4SrcEnd,
    UINT32 u4SrcCur,
    UINT32 u4Size);
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
VOID _SWDMX_MpgCreateFindSema(UINT8 u1SrcId);
VOID _SWDMX_MpgDeleteFindSema(UINT8 u1SrcId);
//INT32 _SWDMX_MpgSelectPSStream(DMX_NOTIFY_INFO_STREAM_ID_T *prInfo);
VOID _SWDMX_MpgFlushVFifo(UINT8 u1SrcId);
VOID _SWDMX_MpgFlushAFifo(UINT8 u1SrcId);
BOOL _SWDMX_MpgWatchVideoPTS(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_MpgWatchAudioPTS(UINT8 u1SrcId, const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_MpgAudioCallbackParse(UINT8 u1SrcId, const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_MpgVideoCallbackMpgParse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
#ifdef SWDMX_MPG_VC1_SUPPORT
BOOL _SWDMX_MpgVideoCallbackVc1Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
BOOL _SWDMX_MpgVideoCallbackH264Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
BOOL _SWDMX_MpgVideoCallbackMpeg4Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
#endif
#ifdef SWDMX_MPG_AVS_SUPPORT
BOOL _SWDMX_MpgVideoCallbackAVSParse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes);
#endif
VOID _SWDMX_MpgSeekFinNotify(UINT8 u1SrcId, UINT32 u4PTS, BOOL fgAgain);

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2VideoId);
BOOL _SWDMX_MpgPsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2AudioId, UINT16 u2AudioSubId);
BOOL _SWDMX_MpgPsFreeVideoId(UINT8 u1SrcId);
BOOL _SWDMX_MpgPsFreeAudioId(UINT8 u1SrcId);
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2VideoId, UINT8 u1KeyIndex, UINT32 u4DescMode, BOOL fgSecondVideo);
BOOL _SWDMX_MpgTsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2AudioId, UINT16 u2DeviceId, UINT8 u1KeyIndex, UINT32 u4DescMode);
BOOL _SWDMX_MpgTsSetPCRId(UINT8 u1SrcId, UINT16 u2PCRId);
BOOL _SWDMX_MpgTsFreeVideoId(UINT8 u1SrcId, BOOL fgSecondVideo);
BOOL _SWDMX_MpgTsFreeAudioId(UINT8 u1SrcId);
BOOL _SWDMX_MpgTsFreePCRId(UINT8 u1SrcId);
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx);
BOOL _SWDMX_MpgEsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx);
BOOL _SWDMX_MpgEsFreeVideoId(UINT8 u1SrcId);
BOOL _SWDMX_MpgEsFreeAudioId(UINT8 u1SrcId);
#endif

BOOL _SWDMX_MpgWatchDuration(UINT8 u1SrcId, UINT32 u4CurentTime);
UINT32 _SWDMX_MpgWatchLastPTS(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Pts);
VOID _SWDMX_MpgResetVariable(UINT8 u1SrcId);
VOID _SWDMX_MpgJumpToNormal(UINT8 u1SrcId, BOOL fgSeek, BOOL fgSeekOffset, BOOL fgJumpping, INT32 i4Speed);
VOID _SWDMX_MpgHandleChange(UINT8 u1SrcId, SWDMX_CUR_CMD_T* prCmd);
VOID _SWDMX_MpgHandleEOS(UINT8 u1SrcId, INT32 i4PlaySpeed);
BOOL _SWDMX_MpgResetFeeder(UINT8 u1SrcId);
VOID _SWDMX_MpgResetInputData(UINT8 u1SrcId);
VOID _SWDMX_MpgResetData(UINT8 u1SrcId);
INT32 _SWDMX_MpgHandleFifoFull(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgLast);
INT32 _SWDMX_MpgHandleMoveData(UINT8 u1SrcId, DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed);
BOOL _SWDMX_MpgHandleMoveDataSysB(UINT8 u1SrcId, DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed);
UINT32 _SWDMX_MpgHandleAudioIndex(UINT8 u1SrcId, UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, UINT32 *pu4OutReadPTS, INT32 i4PlaySpeed);
UINT32 _SWDMX_MpgHandleRing(UINT8 u1SrcId, UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, INT32 i4PlaySpeed);
INT32 _SWDMX_ReceiveFeederAck(UINT8 u1SrcId, VOID *pvData);
BOOL _SWDMX_MpgGetFirstSeqHdr(UINT8 u1SrcId);
VOID _SWDMX_MpgHandleSeek(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgSeek);
VOID _SWDMX_MpgHandleSeekAudio(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgSeek);
INT32 _SWDMX_MpgHandleFeeder(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed);
INT32 _SWDMX_MpgSendData(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList,
            SWDMX_CUR_CMD_T* prCmd);
BOOL _SWDMX_MpgSendDataBatch(UINT8 u1SrcId);
BOOL _SWDMX_MpgSendDataBatchForTotalTime(UINT8 u1SrcId);
BOOL _SWDMX_MpgBaseInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType);
BOOL _SWDMX_MpgOpen(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prOpenInfo);
BOOL _SWDMX_MpgBaseGetInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo);
BOOL _SWDMX_MpgGetSeqInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);
BOOL _SWDMX_MpgIsSpeedAvailable(UINT8 u1SrcId, BOOL fgForward);
INT32 _SWDMX_MpgHandleNoSignel(UINT8 u1SrcId, UINT64 *pu8SeekPos, INT32 i4PlaySpeed);
INT32 _SWDMX_MpgHandleEmpty(UINT8 u1SrcId, UINT64 *pu8SeekPos, INT32 i4PlaySpeed);
BOOL _SWDMX_MpgResetFeederReq(UINT8 u1SrcId);
#ifdef TIME_SHIFT_SUPPORT
UINT64 _SWDMX_MpgTSAlignDown(UINT8 u1SrcId, UINT64 u8Position);
UINT64 _SWDMX_MpgLBA2Pos(UINT8 u1SrcId, UINT32 u4LBA);
#endif

#ifdef SWDMX_MPG_DEBUG_FIFO
VOID _SWDMX_COPY_RING_BUF(
    UINT32 u4TarStart,
    UINT32 u4TarEnd,
    UINT32* pu4TarCur,
    UINT32 u4SrcStart,
    UINT32 u4SrcEnd,
    UINT32 u4SrcCur,
    UINT32 u4Size)
{
    UINT32 u4TmpSize;
    UINT32 u4TarCur;

    if(!pu4TarCur)
    {
        LOG(1, "pu4TarCur null\n");
        return;
    }

    u4TarCur = *pu4TarCur;
    while( u4Size > 0)
    {
        u4TmpSize = u4Size;
        u4TmpSize = (u4SrcEnd - u4SrcCur) > u4TmpSize ? u4TmpSize : (u4SrcEnd - u4SrcCur);
        u4TmpSize = (u4TarEnd - u4TarCur) > u4TmpSize ? u4TmpSize : (u4TarEnd - u4TarCur);
        if(u4TmpSize > 0)
        {
            x_memcpy((VOID*)u4TarCur, (VOID*)u4SrcCur, u4TmpSize);
            u4SrcCur += u4TmpSize;
            u4TarCur += u4TmpSize;
            u4Size -= u4TmpSize;
            if(u4SrcCur >= u4SrcEnd)
            {
                u4SrcCur = u4SrcStart;
            }
            if(u4TarCur >= u4TarEnd)
            {
                u4TarCur = u4TarStart;
            }
        }
    }

    *pu4TarCur = u4TarCur;
}
#endif


#ifdef SWDMX_STATISTICS
VOID _CalFeederTime(UINT8 u1SrcId)
{
    UINT32 u4Delta;
    HAL_TIME_T rDelta;
    HAL_TIME_T *rFeederReqStartTime;
    HAL_TIME_T *rFeederReqEndTime;
    UINT32 *u4FeederAvgTime;
    UINT32 *u4FeederMaxTime;
    UINT32 *u4FeederMinTime;
    UINT32 *u4FeederCount;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    rFeederReqStartTime=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rFeederReqStartTime;
    rFeederReqEndTime=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rFeederReqEndTime;
    u4FeederAvgTime=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederAvgTime;
    u4FeederMaxTime=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMaxTime;
    u4FeederMinTime=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMinTime;
    u4FeederCount=&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederCount;


    HAL_GetTime(rFeederReqEndTime);
    HAL_GetDeltaTime(&rDelta, rFeederReqStartTime, rFeederReqEndTime);

    u4Delta = rDelta.u4Micros;
    if (rDelta.u4Micros > 1000) // 1ms
    {
        *u4FeederAvgTime = ((*u4FeederAvgTime * (*u4FeederCount)) + u4Delta) / (*u4FeederCount + 1);

        (*u4FeederCount) ++;

        *u4FeederMaxTime = (*u4FeederMaxTime > u4Delta) ? *u4FeederMaxTime : u4Delta;
        *u4FeederMinTime = (*u4FeederMinTime < u4Delta) ? *u4FeederMinTime : u4Delta;
    }

    if ((*u4FeederCount % 1000) == (1000 - 1))
    {
        LOG(0, "Feeder Count(%d) Time(%d, %d, %d)\n",
            *u4FeederCount,
            *u4FeederMaxTime, *u4FeederAvgTime, *u4FeederMinTime);
    }
}
#endif

/**
*/
static VOID _SWDMX_MpgModifyPlayPos(UINT8 u1SrcId)
{
    UINT64 u8Remainder = 0;
    SWDMX_MPG_INFO_T* prMpgInst;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    prMpgInst = &(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo);
    
    if((prMpgInst->rVar.u4AlignBase <= 1) 
        || (prMpgInst->rVar.u4AlignBase > (prMpgInst->rVar.u8FileSize - prMpgInst->rVar.u8StartFileOffset))
        || (prMpgInst->rVar.u8PlayPos > prMpgInst->rVar.u8FileSize))
    {
        return;
    }
    if((prMpgInst->rVar.u8PlayPos < prMpgInst->rVar.u8StartFileOffset) 
        || ((prMpgInst->rVar.u8PlayPos - prMpgInst->rVar.u8StartFileOffset) < prMpgInst->rVar.u4AlignBase))
    {
        prMpgInst->rVar.u8PlayPos = prMpgInst->rVar.u8StartFileOffset;
    }
    else
    {
        UNUSED(_SWDMX_Div6464((prMpgInst->rVar.u8PlayPos - prMpgInst->rVar.u8StartFileOffset), 
            prMpgInst->rVar.u4AlignBase, &u8Remainder));
        prMpgInst->rVar.u8PlayPos -= u8Remainder;
    }    
}

static BOOL _SWDMX_DMX_Reset(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_Stop(u1SrcId))
        {
            LOG(3, "_SWDMX_DMX_Reset _SWDMX_PVR_Stop fail\n");
            return FALSE;
        }
        // Todo:
        return DMX_PVRPlay_RequestReset();
    }
    else
#endif
    {
        // because audio only is use CPU move data, we don't need reset it.
        // Todo: how about CPU reset?
        if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_UNKNOWN))
        {
            #ifdef SWDMX_MPG_DMX_20
            return DMX_MUL_RequestReset(prSwdmxInst->u1DmxId);
            #else
            UINT8 u1SrcIdSecond;
            SWDMX_INFO_T* prSwdmxInstSecond;
            u1SrcIdSecond = (u1SrcId+1)%SWDMX_SOURCE_MAX;
            prSwdmxInstSecond = _SWDMX_GetInst(u1SrcIdSecond);
            if(!prSwdmxInstSecond->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                return DMX_DDI_RequestReset();
            }
            #endif
        }
    }
    return TRUE;
}

#ifdef SWDMX_MPG_PS_GET_INFO
static VOID _SWDMX_FlushFindMsgQ(UINT8 u1SrcId)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(INT32);
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}

static VOID _SWDMX_TimerFindTimeout(UINT8 u1SrcId, HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4DecFin;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);
    u4DecFin = DETECT_TIMEOUT;
    VERIFY(x_msg_q_send(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ, (VOID *)&u4DecFin, sizeof(UINT32), 255) == OSR_OK);

    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
}
#endif

#ifdef SWDMX_MPG_PS_GET_INFO
static VOID _SWDMX_FlushDataMsgQ(UINT8 u1SrcId)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(INT32);
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}


static VOID _SWDMX_TimerDataTimeout(UINT8 u1SrcId, HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4DecFin;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    u4DecFin = DETECT_TIMEOUT;
    VERIFY(x_msg_q_send(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ, (VOID *)&u4DecFin, sizeof(UINT32), 255) == OSR_OK);

    UNUSED(pt_tm_handle);
}
#endif

#ifdef SWDMX_MPG_PS_GET_INFO
VOID _SWDMX_MpgCreateFindSema(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ)
    {
        VERIFY(x_msg_q_create(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ, "Find-Q", sizeof(UINT32), 2) == OSR_OK);
    }
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ)
    {
        VERIFY(x_msg_q_create(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ, "Data-Q", sizeof(UINT32), 2) == OSR_OK);
    }
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer)
    {
        VERIFY(x_timer_create(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer) == OSR_OK);
    }
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataTimer)
    {
        VERIFY(x_timer_create(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataTimer) == OSR_OK);
    }
}

VOID _SWDMX_MpgDeleteFindSema(UINT8 u1SrcId)
{

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ)
    {
        VERIFY(x_msg_q_delete(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ) == OSR_OK);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ = 0;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ)
    {
        VERIFY(x_msg_q_delete(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ) == OSR_OK);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataMsgQ = 0;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer)
    {
        VERIFY(x_timer_delete(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer) == OSR_OK);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer = 0;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataTimer)
    {
        VERIFY(x_timer_delete(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataTimer) == OSR_OK);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hDataTimer = 0;
    }
}
#endif

#if 0
INT32 _SWDMX_MpgSelectPSStream(DMX_NOTIFY_INFO_STREAM_ID_T *prInfo)
{
    UINT8 ucStreamId, ucSubStreamId;
    UINT32 i;
    BOOL fgVideoFound = FALSE;
    BOOL fgAudioFound = FALSE;
    UINT8 *pu1Buf;

    if(prInfo == NULL)
    {
        return FALSE;
    }

    if(prInfo->u4DataSize == 0)
    {
        return FALSE;
    }

    pu1Buf = (UINT8*)(prInfo->u4DataAddr);

    for(i=0;i<prInfo->u4DataSize;i+=2) // 2 for StreamId, 2 for SubStreamId
    {
        ucSubStreamId = (UINT8)(pu1Buf[i]);
        ucStreamId = (UINT8)(pu1Buf[i+1]);
        if((!fgVideoFound) &&
            (ucStreamId >= 0xE0) && (ucStreamId <= 0xEF))
        {
            fgVideoFound = TRUE;
            _ucPSVideoStreamId = ucStreamId;
            //_ucPSVideoSubStreamId = 0;
        }
        // mpg audio
        if((!fgAudioFound) &&
            (ucStreamId >= 0xC0) && (ucStreamId <= 0xDF))
        {
            fgAudioFound = TRUE;
            _ucPSAudioStreamId = ucStreamId;
            _ucPSAudioSubStreamId = 0;
        }
        // lpcm audio
        if((!fgAudioFound) &&
            (ucStreamId == 0xBD) &&
            (ucSubStreamId >= 0xA0) && (ucSubStreamId <= 0xA8))
        {
            fgAudioFound = TRUE;
            _ucPSAudioStreamId = ucStreamId;
            _ucPSAudioSubStreamId = ucSubStreamId;
        }
        // ac3 audio
        if((!fgAudioFound) &&
            (ucStreamId == 0xBD) &&
            (ucSubStreamId >= 0x80) && (ucSubStreamId <= 0x88))
        {
            fgAudioFound = TRUE;
            _ucPSAudioStreamId = ucStreamId;
            _ucPSAudioSubStreamId = ucSubStreamId;
        }
        if(fgVideoFound && fgAudioFound)
        {
            return TRUE;
        }
    }

    if(fgVideoFound || fgAudioFound)
    {
        return TRUE;
    }

    return FALSE;
}
#endif

VOID _SWDMX_MpgFlushVFifo(UINT8 u1SrcId)
{
    BOOL fgRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgFlushVFifo DMX_UpdateReadPointer Fail\n");
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr = 0;
        }
    }
}

VOID _SWDMX_MpgFlushVFifo2(UINT8 u1SrcId)
{
    BOOL fgRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 != 0xFF)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgFlushVFifo2 DMX_UpdateReadPointer Fail\n");
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr = 0;
        }
    }
}

VOID _SWDMX_MpgFlushAFifo(UINT8 u1SrcId)
{
    BOOL fgRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgFlushAFifo DMX_UpdateReadPointer Fail\n");
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
        }
    }
}

BOOL _SWDMX_MpgWatchDuration(UINT8 u1SrcId, UINT32 u4CurentTime)
{
    UINT32 u4FilteredTime;
    UINT32 u4NewTotalTime;
    UINT64 u8NewTotalTime;
    UINT64 u8Remainder;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    u4FilteredTime = _SWDMX_MpgWatchLastPTS(u1SrcId, 0xFF, (u4CurentTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS));
    u4FilteredTime -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
    // be careful with u4FilteredTime when it is closed to u4TotalTime
    // add 10 second time for monitor
    if((u4FilteredTime >
        ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset) + (10*90000))) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > 0))
    {
        u8NewTotalTime = _SWDMX_Div6464((UINT64)(u4FilteredTime+1), prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, &u8Remainder);
        u8NewTotalTime = u8NewTotalTime * (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset);
        u8NewTotalTime *= 90;
        u8NewTotalTime = _SWDMX_Div6464(u8NewTotalTime, (UINT64)100, &u8Remainder);
        if(u8NewTotalTime < (UINT64)0xFFFFFFFF)
        {
            u4NewTotalTime = (UINT32)u8NewTotalTime;
            if(u4NewTotalTime > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset = (u4NewTotalTime - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime);
                LOG(3, "_SWDMX_MpgWatchDuration u4TotalTimeOffset %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
            }
        }
    }

    return TRUE;
}


UINT32 _SWDMX_MpgWatchLastPTS(UINT8 u1SrcId, UINT32 u4Type, UINT32 u4Pts)
{
    UINT32 i;
    UINT32 u4Temp = 0;
    UINT32 u4Big = 0;
    UINT32 u4Small = 0;
    UINT32 u4RetPts = u4Pts;
    UINT64 u8Temp = 0;
    UINT64 u8Remainder;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
    // DTV00210035, There is two wrong PTS around 10 sec in this clip.
    // pre-parsing case and normal all use this filter
    {
        // Handle some unexpect PTS
        u8Temp = 0;
        u4Temp = 0;
        u4Big = 0;
        u4Small = 0xFFFFFFFF;
        for(i=0;i<MPG_PTS_CHECK_NUM;i++)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] != 0)
            {
                if(u4Big < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i])
                {
                    u4Big = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i];
                }
                if(u4Small > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i])
                {
                    u4Small = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i];
                }
            }
        }
        for(i=0;i<(MPG_PTS_CHECK_NUM-1);i++)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i+1];
            if(u4Big == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(u4Small == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] != 0)
            {
                u8Temp += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i];
                u4Temp++;
            }
        }
        // we don't record non a/v PTS.
        if(u4Type != 0xFF)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[MPG_PTS_CHECK_NUM-1] = u4Pts;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTSCnt++;
        }
        if(u4Temp > 0)
        {
            u8Temp = _SWDMX_Div6464(u8Temp, (UINT64)u4Temp, &u8Remainder);
            u4Temp = (UINT32)u8Temp;
            if(u4Pts > u4Temp)
            {
                if((u4Pts - u4Temp) > (5*90000*MPG_PTS_CHECK_NUM))
                {
                    LOG(1, "_SWDMX_MpgWatchLastPTS u4Type %d, u4PTS %d -> %d\n"
                         , u4Type, u4Pts, u4Temp);
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = u4Temp;
                    }
                    if(u4Type != 0xFF)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] = u4Temp;
                    }
                    u4RetPts = u4Temp;
                }
            }
            else
            {
                if((u4Temp - u4Pts) > (5*90000*MPG_PTS_CHECK_NUM))
                {
                    LOG(1, "_SWDMX_MpgWatchLastPTS u4Type %d, u4PTS %d -> %d\n"
                        , u4Type, u4Pts, u4Temp);
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = u4Temp;
                    }
                    if(u4Type != 0xFF)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] = u4Temp;
                    }
                    u4RetPts = u4Temp;
                }
            }
        }
    }
    return u4RetPts;
}

BOOL _SWDMX_MpgWatchVideoPTS(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    UINT32 u4Temp = 0;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgWatchVideoPTS prPes NULL\n");
        return FALSE;
    }

    if((prPes->fgPtsDts) && (prPes->u4Pts > 0))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = (prPes->u4Pts);

            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_PUSH))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
               (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
#endif
            LOG(5, "_SWDMX_MpgWatchVideoPTS u4FirstPTS 0x%x\n"
                , prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((prPes->u4Pts < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) &&
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS - prPes->u4Pts) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = prPes->u4Pts;
            }
        }

        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
            LOG(5, "_SWDMX_MpgWatchVideoPTS fgSetStc\n");

            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
            {
                //DTV00144863 When time shift for audio only file
                //no place to resume STC, so we can't stop stc
                LOG(3, "_SWDMX_MpgWatchVideoPTS STC_SetStcValue 0x%x\n",
                    ((prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));

                STC_StopStc(prSwdmxInst->u1StcId);
                // When there is video stream, stc value will set at vdec_flw.c
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)((prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                    if (u4Temp != (UINT32)((prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE))
                    {
                        LOG(3, "_SWDMX_MpgWatchVideoPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }

                // do on vdec_flw.c
                //STC_StartStc(prSwdmxInst->u1StcId);
            }
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            UNUSED(_SWDMX_MpgWatchLastPTS(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prPes->u4Pts));
        }

        #ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
        #endif
            // DTV00210035, two wrong audio PTS. We should not update duration
            //firstPTS=0x8ca9,LastPTS=0xcb139
            //audio PTS=0x3e4cc936
            //audio PTS=0x3e4cc936
            if(((prPes->u4Pts > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) && ((prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) < (600*90000)))||
               ((prPes->u4Pts < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) && ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prPes->u4Pts) < (600*90000))))
            {
                UNUSED(_SWDMX_MpgWatchDuration(u1SrcId, prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS));
            }
        #ifdef TIME_SHIFT_SUPPORT
        }
        #endif

        LOG(6, "video u4Pts 0x%x, pos 0x%x\n",
            ((prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE),
            (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = prPes->u4Pts;
       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS=prPes->u4Pts;  // DTV00139313  record last video pts
    }

    if(((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
        (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK)) &&
       (prPes->u1Channel == prSwdmxInst->u1Vdec2Id))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr = prPes->u4FrameAddr;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr = prPes->u4FrameAddr;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt++;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgPesFound = TRUE;
    #if 0
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt%150)
    {
        LOG(6, "_SWDMX_MpgWatchVideoPTS u4PicCnt 150\n");
    }
    #endif

    return TRUE;
}

BOOL _SWDMX_MpgWatchAudioPTS(UINT8 u1SrcId, const DMX_AUDIO_PES_T* prPes)
{
    UINT32 u4PTS;
    UINT32 u4Temp;
    UINT32 u4DmxAvailSize;
    UINT32 u4DmxAvailSizeAudio;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgWatchAudioPTS prPes NULL\n");
        return FALSE;
    }

    u4PTS = prPes->u4Pts;
    /*
    Can't hear the first second sound in Pandora.
    root cause:
    Playback skip audio data which pts is 0 in the begining of stream.
    Originaly playback does lip sync, so it waits valid (!= 0) audio pts.
    However this is not necessary for Pandora.

    Solution:
    Playback pass the whole audio data to audio driver.
    */
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)&&
       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)&&
       (u4PTS == 0)&&
       ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
         MM_SRC_TYPE_NETWORK_VUDU) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
         MM_SRC_TYPE_PUSH)))
    {
        u4PTS = (MPG_STC_BASE);
    }

    if(u4PTS > 0)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = (u4PTS);

            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_PUSH))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
               (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
#endif
            // in es file, we never get pts.
            // so assume we already found pts.
            if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
                LOG(5, "_SWDMX_MpgWatchAudioPTS u4FirstPTS 0x%x\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
            }

            LOG(5, "_SWDMX_MpgWatchAudioPTS u4FirstPTS %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((u4PTS < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) &&
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS - u4PTS) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = u4PTS;
            }
        }
		
		//DTV00093908
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetFirstAudPts)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetFirstAudPts = TRUE;
			//record the first audio pts after reset fgSetStc(for exp:do seek)
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS = u4PTS;

			LOG(5, "_SWDMX_MpgWatchAudioPTS fgSetStc GetFirstAudioPTS:0x%x\n",
			    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS);
        }

        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
			//record the first audio pts after reset fgSetStc(for exp:do seek)
			//prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS=u4PTS;

			//LOG(5, "_SWDMX_MpgWatchAudioPTS fgSetStc GetFirstAudioPTS:0x%x\n",
			//    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS);

            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
            {
                LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                    (((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE)));

                STC_StopStc(prSwdmxInst->u1StcId);
                // When there is video stream, stc value will set at vdec_flw.c
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                    if (u4Temp != (UINT32)((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE))
                    {
                        LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }

                // do on vdec_flw.c
                //STC_StartStc(prSwdmxInst->u1StcId);
            }
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            UNUSED(_SWDMX_MpgWatchLastPTS(u1SrcId,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, u4PTS));
        }

#ifdef SWDMX_MPG_SYNC_DTV_MODE
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
            //VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
            //UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));

            //LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 1 pts 0x%x pos 0x%llx\n",
            //    ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)),
            //    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
        }
#endif
		//DTV00093908 check 8 second if audio pts still < first video pts, we will start audio for some specail pvr file
		if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)&&
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)&&
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)&&
		  ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)>(90000*8)))
		{		
			  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
			  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
			  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = (MPG_STC_BASE);
	 #ifdef TIME_SHIFT_SUPPORT
			   if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
	 #endif
			 {
				 UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
			 }
			 if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
			   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
			   {
				   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
				   STC_StopStc(prSwdmxInst->u1StcId);
				   // When there is video stream, stc value will set at vdec_flw.c
				   if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
				   {
						u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
						STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
						u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
		
						LOG(3, "_SWDMX_MpgWatchAudioPTS u4DmxAvailSize STC_SetStcValue 0x%x\n",
							  ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
						if (u4Temp != ((UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE)))
						{
							LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
						}
				   }
			   }
			   if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
			   {
					  //Check in HandleFifoFull
					  //AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
					  //STC_StartStc(prSwdmxInst->u1StcId);
			   }
			   else
			   {
					  // if there is no audio PTS in till now,
					  // we use First video PTS as audio's PTS, but not the last one.
					  //VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
			  #ifdef TIME_SHIFT_SUPPORT
					  if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
			  #endif
					  {
						  VDP_TriggerAudReceive(MPG_STC_BASE);
						  // do on vdec_flw.c
						  //STC_StartStc(prSwdmxInst->u1StcId);
					  }
			   }
		
				LOG(0, "_SWDMX_MpgWatchAudioPTS VDP_TriggerAudReceive due to audio pts check fail\n");
		  }

        //DTV00146545, for audio only ts file, if we have get 8sec audio data but still can't get video data, we
        // should set fgGotVideo TRUE, and let audio start
		if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)&&
			(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)&&
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS>prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)&&
			((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)>(90000*8)))


		{
			VDEC_Notify(ES0,VDEC_DEC_DECODE_NO_DATA);
			//VDEC_Notify(ES0,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo=TRUE;
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS=prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS=prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;

			LOG(3,"_swdmx_mpgAudioCallback no video data and set fgGotVideo=TRUE \n");
		}

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio))
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X) &&
               // delay 0.3 second to output audio in time-shfit
               (((((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (u4PTS >= (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS + (30000)))) ||
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (u4PTS >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS)))
               &&
               // For 2x->1x, we must send audio data after the last PTS
               ((u4PTS >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS)
                ||
                // Error handle for we can't find target the PTS
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS - u4PTS) > (90000*10)))) ||
                (u4PTS + 90000 * 600 < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS)))

            // This is orginal code for trigger audio.
            //if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X) &&
            //    (u4PTS >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS))
            {
                BOOL fgOK = TRUE;
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo)
                {
                    // DTV00140198 [MM]Can't play the video file smoothly after select it to play
                    // when fifo full and video are playing, we have to check current playback time
                    fgOK = FALSE;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS == 0xFFFFFFFF)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = (STC_GetStcValue(prSwdmxInst->u1StcId) + 90000 + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
                        LOG(3, "_SWDMX_MpgWatchAudioPTS u4SecondVideoPTS 0x%x, Current time 0x%x\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS, STC_GetStcValue(prSwdmxInst->u1StcId));
                    }
                    if(u4PTS > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS)
                    {
                        fgOK = TRUE;
                    }
                    // DTV00141121, [DLNA] After repeatedly change audio stream, audio doesn't output.
                    // error handle, u4SecondVideoPTS is far from the u4Pts value
                    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS - u4PTS) > (UINT32)(2*90000))
                    {
                        fgOK = TRUE;
                        LOG(3, "_SWDMX_MpgWatchAudioPTS error handle, ignore u4SecondVideoPTS 0x%x\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS);
                    }
                }
                if(fgOK)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;

                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                        ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
                    #ifdef TIME_SHIFT_SUPPORT
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    #endif
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS != 0xFFFFFFFF)
                        {
                            VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                            UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
                        }                        

                        LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 1 pts 0x%x pos 0x%llx\n",
                            ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)),
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
                    }

                    /*
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                    */
               }
            }
        }
        else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                    ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                    //AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                    LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                        ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)));

                    //STC_StartStc(prSwdmxInst->u1StcId);
                    LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 2 pts 0x%x pos 0x%llx\n",
                        ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)), prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
                }
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            }
        }

        #ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
        #endif
            // DTV00210035, two wrong audio PTS. We should not update duration
            //firstPTS=0x8ca9,LastPTS=0xcb139
            //audio PTS=0x3e4cc936
            //audio PTS=0x3e4cc936
            if(((u4PTS > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) && ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) < (600*90000)))||
               ((u4PTS < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS) && ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - u4PTS) < (600*90000))))
            {
                UNUSED(_SWDMX_MpgWatchDuration(u1SrcId, u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS));
            }
        #ifdef TIME_SHIFT_SUPPORT
        }
        #endif

        #if 1
        {
            static UINT32 u4Cnt = 0;
            if(((u4Cnt++)%15) == 0)
            {
                LOG(6, "audio u4Pts 0x%x, pos 0x%x\n",
                    ((u4PTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE),
                    (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
            }
        }
        #else
        LOG(6, "_SWDMX_MpgWatchAudioPTS u4Pts 0x%x\n",
            (u4PTS-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS));
        #endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = u4PTS;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = u4PTS;
    }

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio))
    {
        // could be video fifo size

        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
        {
            u4DmxAvailSize = (1024*1024);
            // playbacking, check AV size
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                #ifdef SWDMX_MPG_DMX_20
                u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                    DMX_PID_TYPE_ES_VIDEO,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
                #else
#ifdef CC_DMX_PURE_AUDIO
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    u4DmxAvailSize = DMX_MM_GetBufEmptySize(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
               	}
                else
#endif
                {
                    u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
                }
                #endif
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
            {
                #ifdef SWDMX_MPG_DMX_20
                u4Temp = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                    DMX_PID_TYPE_ES_AUDIO,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                #else
#ifdef CC_DMX_PURE_AUDIO
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    u4Temp = DMX_MM_GetBufEmptySize(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
               	}
                else
#endif
                {
                    u4Temp = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                }
                #endif

                if(u4DmxAvailSize > u4Temp)
                {
                    u4DmxAvailSize = u4Temp;
                }
            }
            if(u4DmxAvailSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

                // if there is no audio PTS in till now,
                // we use First video PTS as audio's PTS, but not the last one.
                //AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                //    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS + (MPG_STC_BASE));
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = (MPG_STC_BASE);
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                }

                if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;

				    STC_StopStc(prSwdmxInst->u1StcId);
                    // When there is video stream, stc value will set at vdec_flw.c
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                    {
                        u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                        STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                        u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);

                        LOG(3, "_SWDMX_MpgWatchAudioPTS u4DmxAvailSize STC_SetStcValue 0x%x\n",
                            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                        if (u4Temp != ((UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE)))
                        {
                            LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        }
                    }
                }
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    //Check in HandleFifoFull
                    //AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                    //STC_StartStc(prSwdmxInst->u1StcId);
                }
                else
                {
                    // if there is no audio PTS in till now,
                    // we use First video PTS as audio's PTS, but not the last one.
                    //VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
                    #ifdef TIME_SHIFT_SUPPORT
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    #endif
                    {
                        VDP_TriggerAudReceive(MPG_STC_BASE);
                        // do on vdec_flw.c
                        //STC_StartStc(prSwdmxInst->u1StcId);
                    }
                }

                /*
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
                */
                //ASSERT(0);
                LOG(0, "_SWDMX_MpgWatchAudioPTS VDP_TriggerAudReceive due to fifo full");
            }
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
        {
            BOOL fgCheckAudFull=FALSE;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT)||                 // [DTV00211805]
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS)||                 // for PS, TS file
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
			   //|| (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))   //DTV00086567, the first second audio can't output

            {
                u4DmxAvailSize = (1024*1024);
                // playbacking, check AV size
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
                {
                    #ifdef SWDMX_MPG_DMX_20
                    u4DmxAvailSizeAudio = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                        DMX_PID_TYPE_ES_AUDIO,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                    #else
#ifdef CC_DMX_PURE_AUDIO
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                   	}
                    else
#endif
                    {
                        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
                    #endif
                    if(u4DmxAvailSize > u4DmxAvailSizeAudio)
                    {
                        u4DmxAvailSize = u4DmxAvailSizeAudio;
                    }
                }
                if(u4DmxAvailSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize)         //DTV00212016 check audio fifo is full or not
                {
                    fgCheckAudFull=TRUE;
                }
            }
            else
            {
                fgCheckAudFull=TRUE;          // DTV00140422 audio ES file no needed to check is audio fifo full or not
            }

            if(fgCheckAudFull == TRUE)         //
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;

                // if there is no audio PTS in till now,
                // we use First video PTS as audio's PTS, but not the last one.
                //AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                //    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS + (MPG_STC_BASE));
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = (MPG_STC_BASE);
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                    //AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                    LOG(3, "_SWDMX_MpgWatchAudioPTS no V AUD_SetStartPts pts 0x%x pos 0x%llx\n",
                           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)), prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
                    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                }

                if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;

                    //LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                        //(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE))));
                    LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);

                    STC_StopStc(prSwdmxInst->u1StcId);
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);

                    if (u4Temp != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS)
                    {
                        LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }
            }
            //STC_StartStc(prSwdmxInst->u1StcId);
        }
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgPesFound = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = prPes->u4Wp;

    #if 0
    {
        static UINT32 u4Cnt = 0;
        if(((u4Cnt++)%60) == 0)
        {
            LOG(6, "_SWDMX_MpgWatchAudioPTS u4Cnt 60\n");
        }
    }
    #endif

    return TRUE;
}


BOOL _SWDMX_MpgAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if((eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE) ||
       (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA))
    {
       if((eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE))
       {
           LOG(1, "audio is ready===========================\n");
       }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

        /*
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio || (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)))
        {
            LOG(5, "_SWDMX_MpgAudioCb  fgVideoPause VDEC_Play\n");
            if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                VDEC_Play(u1SrcId, VDEC_FMT_MPV);
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                VDEC_Play(u1SrcId, VDEC_FMT_H264);
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                VDEC_Play(u1SrcId, VDEC_FMT_WMV);
            }
            else
            {
                ASSERT(0);
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        }
        else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) // audio only
        {
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }*/

        #ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        #endif
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo))
            {
                // DTV00134760
                // In this case, because video fifo is full.
                // Video start to play without wait audio ready.
                // So after audio ready, notify aout in first audio PTS.                
                LOG(3, "trigger audio PTS = 0x%x, STC = 0x%x\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS, 
                    STC_GetStcValue(prSwdmxInst->u1StcId));
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS != 0xFFFFFFFF)
                {
                    VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
                }
            }
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio)
        {
            LOG(3, "re-enable audio\n");
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
        }
    }
    else
    {
        //impossible
        ASSERT(0);
    }
    return TRUE;
}

BOOL _SWDMX_MpgPcrCallBack(UINT8 u1SrcId, UINT32 u4PcrBase)
{
    // because we only have one stc, find the one
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
        STC_StopStc(prSwdmxInst->u1StcId);
        LOG(3, "_SWDMX_MpgPcrCallBack STC_SetStcValue 0x%x\n", (u4PcrBase));
        STC_SetStcValue(prSwdmxInst->u1StcId, (UINT32)(u4PcrBase));
        STC_StartStc(prSwdmxInst->u1StcId);
    }
    return TRUE;
}

BOOL _SWDMX_MpgAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    UINT8 u1SrcId;
    DMX_AUDIO_PES_T rPes;
    #ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    UINT32 u4Tmp;
    SWDMX_MPG_MP3_HDR rMp3Hdr;
    UCHAR *pRptr;
    UINT32 au4BufStart[2], au4BufEnd[2];
    #endif
    UINT32 u4Temp = 0;
    SWDMX_INFO_T *prSwdmxInst;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgAudioCallback prPes NULL\n");
        return FALSE;
    }

    prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    u1SrcId = prSwdmxInst->u1SwdmxId;

    x_memset(&rPes, 0, sizeof(rPes));

    #ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
       ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)) &&
       (prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_MPEG) &&
       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady)
      )
    {
#ifdef AUD_OLDFIFO_INTF
        VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                              &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
        VERIFY(AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr)
        {
            pRptr = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr;
        }
        else
        {
            pRptr = (UCHAR*)au4BufStart[0];
        }
        if((au4BufEnd[0] > ((UINT32)pRptr)) &&
           ((au4BufEnd[0] - ((UINT32)pRptr)) > 4))
        {
            u4Tmp = (*(UCHAR*)VIRTUAL((UINT32)(pRptr)));
            u4Tmp <<= 8;
            u4Tmp += (*(UCHAR*)VIRTUAL((UINT32)(pRptr+1)));
            u4Tmp <<= 8;
            u4Tmp += (*(UCHAR*)VIRTUAL((UINT32)(pRptr+2)));
            u4Tmp <<= 8;
            u4Tmp += (*(UCHAR*)VIRTUAL((UINT32)(pRptr+3)));
        }
        else
        {
            u4Tmp = 0;
        }
        rMp3Hdr.au4Reg[0] = u4Tmp;
        if((rMp3Hdr.rField.u4FrameSync == 0x7FF) &&
           (rMp3Hdr.rField.u4Version != 0x1) && // reserved
           (rMp3Hdr.rField.u4Layer == 0x1) &&// layer 3
           //(rMp3Hdr.rField.u4BitateIdx != 0xF) &&// bad
           (rMp3Hdr.rField.u4SampleRate != 0x3)// reserved
          )
        {
            //AUD_DEC_DECODE_FMT_CONFLICT
            LOG(3, "_SWDMX_MpgAudioCallback MP3 audio\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt ++;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt >= MPG_MP3_CHECK_CNT)
            {
                // notify audio driver
                #ifdef SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
                AUD_MM_Set_Dec_Fmt_Conflict(prPes->u1DeviceId, TRUE);
                #endif
            }
        }
    }
    #endif

    #if 0
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2))
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgAudioCallbackParse(prPes);
        }
    }
    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264))
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgAudioCallbackParse(prPes);
        }
    }
    #endif

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
#ifdef SWDMX_MPG_MPEG2_SUPPORT
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                // DTV00313499  before video decoder has start to decoder, we should stop STC,
                //for mw get the right time code at the first time.           
				STC_StopStc(prSwdmxInst->u1StcId);
                return _SWDMX_MpgAudioCallbackParse(u1SrcId, prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header

				STC_StopStc(prSwdmxInst->u1StcId);
                return _SWDMX_MpgAudioCallbackParse(u1SrcId, prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                
				STC_StopStc(prSwdmxInst->u1StcId);
                return _SWDMX_MpgAudioCallbackParse(u1SrcId, prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_AVS_SUPPORT
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                
				STC_StopStc(prSwdmxInst->u1StcId);
                return _SWDMX_MpgAudioCallbackParse(u1SrcId, prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                
				STC_StopStc(prSwdmxInst->u1StcId);
                return _SWDMX_MpgAudioCallbackParse(u1SrcId, prPes);
            }
        }
        else
#endif
        {
        }
    }

    x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr)
    {
        // for the first time, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0,
        // we use original write pointer.
        rPes.u4Wp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekaud)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeekAudPTS > (prPes->u4Pts - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
            return TRUE;
        }
        else
        {
            LOG(0,"1111111111111111111111111111111send first audo pts == %x,audiosee =%x\n",rPes.u4Pts,prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeekAudPTS);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekaud = FALSE;
        }
    }

    UNUSED(_SWDMX_MpgWatchAudioPTS(u1SrcId, prPes));

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec)
    {
        //_SWDMX_MpgFlushAFifo();
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
    {
        //_SWDMX_MpgFlushAFifo();
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        _SWDMX_MpgFlushVFifo(u1SrcId);
        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
            (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
        {
            _SWDMX_MpgFlushVFifo2(u1SrcId);
        }
        return TRUE;
    }

    // DTV00348760, add control, after enable audio, not using video to filter audio
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) && 
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio))
    {
        //_SWDMX_MpgFlushAFifo();
        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
        //{
        //    UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio));
        //}
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
    {
        //_SWDMX_MpgFlushAFifo();
        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
        //{
        //    UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio));
        //}
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    // On cleaning
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio)
    {
        return TRUE;
    }

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
    {
        return TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos)
    {
        return TRUE;
    }

    #ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPes.u4TickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
    }
    #endif

    if(rPes.u4Dts > 0)
    {
        rPes.u4Dts -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPes.u4Dts += (MPG_STC_BASE);
    }
    if(rPes.u4Pts > 0)
    {
        rPes.u4Pts -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPes.u4Pts += (MPG_STC_BASE);
    }
    else
    {
        // DTV00135223, audio driver will ignore zero pts now.
        // DTV00209856, audio PTS is zero sometimes. Send the last one.
        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS > 0)
        //{
        //    rPes.u4Pts = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS -
        //        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
        //}
        //LOG(6, "_SWDMX_MpgAudioCallback rPes.u4Pts = 0 -> %d\n", rPes.u4Pts);
    }
	rPes.fgForward = FALSE;
	
    if(prPes->fgEOS)
    {
        // DTV00065338, [Acceptance test] Playback Time length NOT equal in Playback Screen and File Information.
        // because audio driver does not get the lastest Wptr,
        // add this to set the last one.
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            DMX_AUDIO_PES_T rPes1Last;
            x_memset(&rPes1Last, 0, sizeof(rPes1Last));
            rPes1Last.u1DeviceId = prSwdmxInst->u1AdecId;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime)
            {
                rPes1Last.u4Dts =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + (MPG_STC_BASE);
                rPes1Last.u4Pts =  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + (MPG_STC_BASE);
            }
            rPes1Last.u4Wp =  rPes.u4Wp;
			
			if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >0)
			{
			   //rPes1Last.fgForward = TRUE;        //for trick mode 
			   rPes.fgForward =TRUE;
			}
			else
			{
				//rPes1Last.fgForward = FALSE;
				rPes.fgForward =FALSE;
			}
            UNUSED(AUD_SendAudioPes(&rPes1Last));
        }

        // for the first time, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = xx,
        // but we use original write pointer.
        LOG(3, "_SWDMX_MpgAudioCallback send EOS\n");
        rPes.u4Wp = prPes->u4Wp;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;
    }
#ifdef SWDMX_MPG_DEBUG_FIFO
#ifdef SWDMX_MPG_AFIFO_SIZE
    if((_fgAFifo) && (_fgAManual))
    {
        UINT32 u4Addr, u4Size;
        UINT32 u4AudioSize;
        UINT32 u4Tmp;

        if(_u4AudioLastWriter)
        {
            UNUSED(_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size));
            if(_u4AudioLastWriter > rPes.u4Wp)
            {
                u4AudioSize = ((rPes.u4Wp + u4Size) - _u4AudioLastWriter);
            }
            else
            {
                u4AudioSize = (rPes.u4Wp - _u4AudioLastWriter);
            }
            _SWDMX_COPY_RING_BUF(
                _u4AFifoStart,
                _u4AFifoEnd,
                &_u4AFifoCur,
                u4Addr,
                (u4Addr + u4Size),
                _u4AudioLastWriter,
                u4AudioSize);
        }
        _u4AudioLastWriter = rPes.u4Wp;
    }
#endif
#endif
if(ucCount)
{
    u4Temp = STC_GetStcValue(prSwdmxInst->u1StcId);
    LOG(1,"audio callback data,pts 0x%x,stc %x\n",rPes.u4Pts,u4Temp);
    ucCount--;
}
    fgRet = AUD_SendAudioPes(&rPes);
    if(prPes->fgEOS)
    {
        AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
    }
    return fgRet;
}

BOOL _SWDMX_MpgVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    UINT32 u4SwdmxPicType = SWDMX_PIC_TYPE_OTHER;
    UINT8 u1SrcId;

    SWDMX_INFO_T *prSwdmxInst;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallback prPes NULL\n");
        return FALSE;
    }

    prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    u1SrcId = prSwdmxInst->u1SwdmxId;

    if (u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_MpgVideoCallback (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return FALSE;
    }

#ifdef SWDMX_MPG_MPEG2_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackMpgParse(u1SrcId, prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackH264Parse(u1SrcId, prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
            return _SWDMX_MpgVideoCallbackMpeg4Parse(u1SrcId, prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_AVS_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
            return _SWDMX_MpgVideoCallbackAVSParse(u1SrcId, prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackVc1Parse(u1SrcId, prPes);
        }
    }
    else
#endif
    {
        LOG(0, "SwdmxMpg Not Support\n");
    }


    UNUSED(_SWDMX_MpgWatchVideoPTS(u1SrcId, prPes));

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec)
    {
        _SWDMX_MpgFlushAFifo(u1SrcId);
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
    {
        _SWDMX_MpgFlushAFifo(u1SrcId);
        _SWDMX_MpgFlushVFifo(u1SrcId);
        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
            (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
        {
            _SWDMX_MpgFlushVFifo2(u1SrcId);
        }
        return TRUE;
    }

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        return TRUE;
    }

    if(((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
        (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK)) &&
       (prPes->u1Channel == prSwdmxInst->u1Vdec2Id))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos)
        {
            return TRUE;
        }
    }
    else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos)
    {
        return TRUE;
    }

    // ilii, markout
    /*
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio || (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
        && prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady)
    {
        LOG(5, "_SWDMX_MpgVideoCallback fgVideoPause VDEC_Play\n");
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            VDEC_Play(u1SrcId, VDEC_FMT_MPV);
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            VDEC_Play(u1SrcId, VDEC_FMT_H264);
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            VDEC_Play(u1SrcId, VDEC_FMT_WMV);
        }
        else
        {
            ASSERT(0);
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
    }
    */

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    /*
    if((prSwdmxInst->i4PlaySpeed > 0)
        && (prSwdmxInst->i4PlaySpeed < 4000))
    {
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)
        {
            if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(prPes->u4FrameType != PIC_TYPE_I)
                {
                    _SWDMX_MpgFlushVFifo();
                    return TRUE;
                }
            }
        }
    }

    if((prSwdmxInst->i4PlaySpeed < 1000)
        || (prSwdmxInst->i4PlaySpeed >= 4000))
    {
        if(prPes->u4FrameType == PIC_TYPE_B)
        {
            return TRUE;
        }
    }   */

    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;
    //  This part of code has been move down ,  DTV00079400  there is a case  in the 00:00:55, pts=0, and fgPtsDts=true,
    // when jumping to the point , pts will wrong,
    //DTV00139582.. if (fgPtsDts) ,PTS=0 is also valid used as  first video PTS.
	#if 0
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)&&(prPes->u4Pts==0)&&(prPes->fgPtsDts))
    {
        rPesInfo.u4PTS= (MPG_STC_BASE);
    }
    #endif

    if(rPesInfo.u4PTS > 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        //LOG(5, "_SWDMX_MpgVideoCallback Org PTS %d\n", rPesInfo.u4PTS);
        rPesInfo.u4PTS -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPesInfo.u4PTS += (MPG_STC_BASE);

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS != (UINT32)0xFFFFFFFF)
        {
            if((prPes->u4Pts < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS) &&
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS - prPes->u4Pts) > (10000))) // 1 sec
            {
                // DTV00209729, first video pts is a fake one.
                // we would not send audio data with PTS small than this first fake video pts.

                // u4FirstVideoPTS is used to output audio after this video pts
                // even we assign a smaller PTS that actually is a fake one,
                // we would also not affect the time after we start to output audio
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = prPes->u4Pts;
            }
        }
    }
    if(rPesInfo.u4DTS > 0)
    {
        rPesInfo.u4DTS -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPesInfo.u4DTS += (MPG_STC_BASE);
    }

    //DTV00139582.. if (fgPtsDts) ,PTS=0 is also valid used as  first video PTS.
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)&&(prPes->u4Pts==0)&&(prPes->fgPtsDts))
    {
        if(prSwdmxInst->u4SeekTime >= 90000*2) // protect 2 seconds 
        {
            return TRUE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        rPesInfo.u4PTS = (MPG_STC_BASE);
		rPesInfo.u4DTS = (MPG_STC_BASE);

    }

    rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
    {
        rPesInfo.ucEsId        = (prPes->u1Channel);
    }
    rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    //rPesInfo.fgNotDisplay = u4Pts < u4SeekTime;
    rPesInfo.u4TotalTimeOffset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset;
    rPesInfo.u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize;
    rPesInfo.u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound;
    rPesInfo.u2DecodingOrder = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2OrderFromIPos;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    #ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPesInfo.u4TickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
    }
    #endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2OrderFromIPos++;

    // in es file, we never get pts.
    // so assume we already found pts.
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
            STC_StopStc(prSwdmxInst->u1StcId);
            // When there is video stream, stc value will set at vdec_flw.c

        }
    }

#ifdef SWDMX_MPG_MPEG2_SUPPORT
    if (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            rPesInfo.ucPicType = MPV_I_TYPE;
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
            //add pts info for DTV00332611
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx, now pts:0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos, prPes->u4Pts);
            break;

        case PIC_TYPE_P:
            rPesInfo.ucPicType = MPV_P_TYPE;
            u4SwdmxPicType = SWDMX_PIC_TYPE_P;
            break;

        case PIC_TYPE_B:
            rPesInfo.ucPicType = MPV_B_TYPE;
            break;

        case PIC_TYPE_SEQ_END:
            rPesInfo.ucPicType = MPV_I_TYPE;
            rPesInfo.fgSeqEnd = TRUE;
            break;

        default:
            rPesInfo.ucPicType = 0;
            #ifndef CC_VDEC_FMT_DETECT
            ASSERT(0);
            break;
            #else
            return TRUE;
            #endif
        }
        if((prPes->u4FrameType == PIC_TYPE_P) ||
           (prPes->u4FrameType == PIC_TYPE_B))
        {
            // Sample a key frame is there is no IDR
            // Currently, key is 3 second
            UINT32 u4Tmp;
            UINT64 u8Size;
            UINT64 u8Remainder;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime) > 90000)
            {
                u4Tmp = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime / 90000); // total frames
                //u4Tmp *= 3;  wrong -> u8Size *= 3;
                u8Size = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);
                u8Size *= 1;
            }
            else
            {
                u8Size = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
                u8Size *= 30;
            }
            
            if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)&&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
            {
                u8Size *= 3;
            }
            
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize
                - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos) > u8Size)
            {
                u4SwdmxPicType = SWDMX_PIC_TYPE_NO_I;
            }
        }
    }
    else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
    if (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid)
        {
            if(prPes->u4FrameType != VC1_PIC_TYPE_I)
            {
                return TRUE;
            }
        }

        switch (prPes->u4FrameType)
        {
        case VC1_PIC_TYPE_I:
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = FALSE;
            break;

        case VC1_PIC_TYPE_P:
            u4SwdmxPicType = SWDMX_PIC_TYPE_P;
            break;

        /*case PIC_TYPE_B:
            break;*/

        default:
            break;
        }
    }
    else
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
    {
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        u4Addr = prPes->u4FrameAddr + 4;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.u4VldReadPtr = u4Addr;

        if((rPesInfo.ucPicType & 0xF) == 0x5) // IDR_NALU
        {
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
        }
        else
        {
            // Sample a key frame is there is no IDR
            // Currently, key is 3 second
            UINT32 u4Tmp;
            UINT64 u8Size;
            UINT64 u8Remainder;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime) > 90000)
            {
                u4Tmp = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime / 90000); // total frames
                u4Tmp *= 1;
                u8Size = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);
            }
            else
            {
                u8Size = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
                u8Size *= 30;
            }
            
            if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)&&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_ENCRYPT_192))

            {
                u8Size *= 3;
            }

            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize
                - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos) > u8Size)
            {
                u4SwdmxPicType = SWDMX_PIC_TYPE_NO_I;
            }
        }

        LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos);
    }
    else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
        UCHAR *pucBuf;

        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if (pucBuf[0] == 0xB6)
        {
            u4Addr = prPes->u4FrameAddr + 4;
            if(u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            switch (pucBuf[0] >> 6)
            {
                case 0:
                    rPesInfo.ucPicType  = I_TYPE;
                    u4SwdmxPicType = SWDMX_PIC_TYPE_I;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = FALSE;
                    LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos);
                    break;

                case 1: case 3:
                    rPesInfo.ucPicType  = P_TYPE;
                    u4SwdmxPicType = SWDMX_PIC_TYPE_P;
                    break;

                case 2:
                    rPesInfo.ucPicType = B_TYPE;
                    break;

                default:
                    rPesInfo.ucPicType = UNKNOWN_TYPE;
                    break;
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid)
            {
                if(rPesInfo.ucPicType != VC1_PIC_TYPE_I)
                {
                    return TRUE;
                }
            }
        }
        else if ((pucBuf[0] & 0x20) == 0x20)
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }
    else
#endif
#ifdef SWDMX_MPG_AVS_SUPPORT
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
    {
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_AVS;
        u4Addr = prPes->u4FrameAddr + 3;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        if (rPesInfo.ucPicType == 0xB3)
        {
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos);
        }
        else if (rPesInfo.ucPicType == 0xB0)
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }
    else
#endif
    {
        LOG(1, "_SWDMX_MpgVideoCallback unknown video\n");
    }

    if((u4SwdmxPicType == SWDMX_PIC_TYPE_I)
       ||
       // in order to have u4FirstVideoPTS, reuse this code
       ((u4SwdmxPicType == SWDMX_PIC_TYPE_NO_I) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound && !prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos)
        {
            //if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
               (
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed <= 1500)
                )
                ||
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)&&
                 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 4000)
                )
               )
#else
               ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 4000))
#endif
              )
            {
                // In time-shift, we only pause vdp, but not vdec
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause)
                    {
                        UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE));
                        VDEC_Pause(prSwdmxInst->u1VdecId);
                        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                        {
                            VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                        }
                        LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                    }
                }
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                {
                    if(prPes->u4Pts==0)          //DTV00139313
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS=prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS;
                        LOG(5,"set FirstVideoPTS=u4LastPTS=0x%x STC(0x%x)\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS, STC_GetStcValue(prSwdmxInst->u1StcId));
                    }
                    else
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                        LOG(5,"set FirstVideoPTS= First I pic Pts=0x%x STC(0x%x)\n",
                            prPes->u4Pts, STC_GetStcValue(prSwdmxInst->u1StcId));
                    }
                }
                LOG(5, "_SWDMX_MpgVideoCallback fgGotVideo\n");
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime));
                UNUSED(VDEC_SetFileSize(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))
                {
                    UNUSED(VDEC_SetLastTime(prSwdmxInst->u1VdecId, MPG_STC_BASE));
                }
            }
            else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime));
                UNUSED(VDEC_SetFileSize(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))
                {
                    UNUSED(VDEC_SetLastTime(prSwdmxInst->u1VdecId, MPG_STC_BASE));
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2OrderFromIPos = 0;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPosWithPTSFound 0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound);

            #ifdef SWDMX_MPG_USE_INDEX
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx)
            {
                UNUSED(_SWDMX_MpgIdxUpdate(u1SrcId, rPesInfo.u4PTS,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound));
            }
            #endif
        }
    }
    else if(u4SwdmxPicType == SWDMX_PIC_TYPE_P)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound;
    }

    if(u4SwdmxPicType == SWDMX_PIC_TYPE_NO_I)
    {
        LOG(7, "_SWDMX_MpgVideoCallback No I case\n");
        rPesInfo.u8OffsetI = 0xFFFFFFFF;
        rPesInfo.u8OffsetI <<= 32;
        rPesInfo.u8OffsetI += 0xFFFFFFFF;
    }

    // For first video frame is not I frame clip,
    // we may send this pes to vdec. We have to pause vdec,
    // or we may start stc too earlier.
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos)
    {
        //if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
        // In time-shift, we only pause vdp, but not vdec
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) &&
               (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause) &&
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0)
                   && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 4000)))
            {
                VDEC_HDR_INFO_T rHdrInfo;
                x_memset(&rHdrInfo, 0, sizeof(rHdrInfo));
                UNUSED(VDEC_QueryInfo(prSwdmxInst->u1VdecId, &rHdrInfo));
                if(rHdrInfo.u4DisplayQPicCnt > 0)
                {
                    // DTV00136474 [MM]No audio ouput when play the special ts file.
                    // When all picture is P/B frame, we have to know this information from vdec
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed > 0)
                       &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 4000))
                    {
                        // In time-shift, we only pause vdp, but not vdec
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                        {
                            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause)
                            {
                                UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE));
                                VDEC_Pause(prSwdmxInst->u1VdecId);
                                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                                {
                                    VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                                }
                                LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                            }
                        }
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            if(prPes->u4Pts==0)          //DTV00139313
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS=prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS;
                                LOG(5,"set FirstVideoPTS=u4LastPTS=0x%x\n",
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS);

                            }
                            else
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                                LOG(5,"set FirstVideoPTS= First I pic Pts=0x%x\n",
                                    prPes->u4Pts);
                            }
                        }
                        LOG(5, "_SWDMX_MpgVideoCallback fgGotVideo\n");
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                        UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime));
                        UNUSED(VDEC_SetFileSize(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))
                        {
                            UNUSED(VDEC_SetLastTime(prSwdmxInst->u1VdecId, MPG_STC_BASE));
                        }
                    }
                    else //if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                        UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime));
                        UNUSED(VDEC_SetFileSize(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))
                        {
                            UNUSED(VDEC_SetLastTime(prSwdmxInst->u1VdecId, MPG_STC_BASE));
                        }
                    }
                }
                else
                {
                    UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE));
                    VDEC_Pause(prSwdmxInst->u1VdecId);
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        VDEC_Pause(prSwdmxInst->u1Vdec2Id);
                    }
                    LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                }
            }
        }
    }

    // in es file, we never get pts.
    // so assume we already found pts.
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) &&
         ((rPesInfo.ucPicType == 0xB6) || (rPesInfo.ucPicType == 0xB3))) ||
        ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) &&
         ((rPesInfo.ucPicType & 0x1F) <= 0x5))
#ifdef SWDMX_MPG_VC1_SUPPORT
        ||
        ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) &&
         (rPesInfo.ucPicType >= VC1_PIC_TYPE_I) &&
         (rPesInfo.ucPicType <= VC1_PIC_TYPE_SKIP))
#endif
      )
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4DTS = MPG_STC_BASE;
            rPesInfo.u4PTS = MPG_STC_BASE;

            LOG(5, "_SWDMX_MpgVideoCallback u4FirstPTS 0x%x\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping)
        {
            UINT32 u4PTS;
            UINT64 u8Tmp;
            UINT64 u8Remainder;

            // to make PTS increament, use file position to calculate a fake one
            u8Tmp = VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
            // 90000 / 30 fps = 3000
            u4PTS = (UINT32)_SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize * (UINT64)3000), u8Tmp, &u8Remainder);

            if(!rPesInfo.fgDtsValid)
            {
                rPesInfo.fgDtsValid = TRUE;
                rPesInfo.u4DTS = (u4PTS + MPG_STC_BASE);
                rPesInfo.u4PTS = rPesInfo.u4DTS;
            }
        }
    }
    else
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping)
        {
            if(!rPesInfo.fgDtsValid)
            {
                rPesInfo.fgDtsValid = TRUE;
                rPesInfo.u4DTS = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS)
                                  + (MPG_STC_BASE));
                rPesInfo.u4PTS = rPesInfo.u4DTS;
            }
        }
    }

    if(prPes->fgEOS)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0)
        {
            rPesInfo.fgBackward = TRUE;
        }
        
        if(((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
            (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK)) &&
           (prPes->u1Channel == prSwdmxInst->u1Vdec2Id))
        {
            LOG(3, "_SWDMX_MpgVideoCallback send 2 EOS\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE;
        }
        else //(prPes->u1Channel == ES0)
        {
            LOG(3, "_SWDMX_MpgVideoCallback send EOS\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
        }
    }
    rPesInfo.u4Timestap = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
    UNUSED(VDEC_SendEs((void*)&rPesInfo));

    /*
    if(prPes->fgEOS)
    {
        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
        {
            LOG(3, "_SWDMX_MpgVideoCallback send 2nd EOS\n");
            if(rPesInfo.ucEsId == prSwdmxInst->u1VdecId)
            {
                rPesInfo.ucEsId = prSwdmxInst->u1Vdec2Id;
            }
            else
            {
                rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
            }
            UNUSED(VDEC_SendEs((void*)&rPesInfo));
        }
    }
    */
    UNUSED(u4Addr);
    return TRUE;
}


BOOL _SWDMX_MpgAudioCallbackParse(UINT8 u1SrcId, const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgAudioCallbackParse prPes NULL\n");
        return FALSE;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgAudioCallbackParse DMX_UpdateReadPointer Fail\n");
        }
    }

    return TRUE;
}

BOOL _SWDMX_MpgVideoCallbackMpgParse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    //UCHAR ucByte0, ucByte1, ucByte2, ucByte3;
    //UINT16 u2QueueSize;
    //UINT16 u2MaxQueueSize;
    //UINT32 u4Tmp;
    VDEC_PES_INFO_T rPesInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallbackMpgParse prPes NULL\n");
        return FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeqHdrChk++;
    if( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeqHdrChk>256)         // define the check size is  "256"  more than  temporal reference
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;
        VDEC_Notify(prSwdmxInst->u1VdecId, VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
        LOG(3,"there is no sequence header has be find\n");

        return TRUE;
    }

    if(prPes->fgSeqHeader)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(prSwdmxInst->u1VdecId, VDEC_FMT_MPV, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
        // in getting sequence, we should not send pts to vdec
        // or we may get wrong start pts on vdec.
        rPesInfo.ucMpvId = VLD0;
        rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
        fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_MPV, &rPesInfo, &_arSeqInfo[u1SrcId]);
        if(fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackMpgParse send sequence\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
        }

        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        return TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackMpgParse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}


#ifdef SWDMX_MPG_VC1_SUPPORT
BOOL _SWDMX_MpgVideoCallbackVc1Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    //UCHAR ucByte0, ucByte1, ucByte2, ucByte3;
    VDEC_PES_INFO_T rPesInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallbackMpgParse prPes NULL\n");
        return FALSE;
    }

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    if(prPes->u4FrameType == VC1_PIC_TYPE_SEQ_START)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(prSwdmxInst->u1VdecId, VDEC_FMT_WMV, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        // in getting sequence, we should not send pts to vdec
        // or we may get wrong start pts on vdec.
        rPesInfo.ucMpvId = VLD0;
        rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = TRUE;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
        fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_WMV, &rPesInfo, &_arSeqInfo[u1SrcId]);
        if(fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse send sequence\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = TRUE;
        }

        //UNUSED(VDEC_SetGetSeqHdrOnly(prSwdmxInst->u1VdecId, FALSE));

        return TRUE;
    }
    else if(prPes->u4FrameType == VC1_PIC_TYPE_ENTRY)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 == TRUE)
        {
            //UNUSED(VDEC_SetGetSeqHdrOnly(prSwdmxInst->u1VdecId, TRUE));

            // in getting sequence, we should not send pts to vdec
            // or we may get wrong start pts on vdec.
            rPesInfo.ucMpvId = VLD0;
            rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
            rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
            rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
            rPesInfo.u4FifoStart = prPes->u4BufStart;
            rPesInfo.u4FifoEnd = prPes->u4BufEnd;
            rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
            rPesInfo.fgDtsValid = prPes->fgPtsDts;
            rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
            rPesInfo.fgGop = prPes->fgGop;
            x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
            fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_WMV, &rPesInfo, &_arSeqInfo[u1SrcId]);
            if(fgRet)
            {
                LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse send sequence\n");
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
            }

            UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

            return TRUE;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse DMX_UpdateReadPointer Fail\n");
        }
    }

    return TRUE;
}
#endif


#ifdef SWDMX_MPG_MPEG4_SUPPORT
BOOL _SWDMX_MpgVideoCallbackMpeg4Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UCHAR *pucBuf;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.ucMpvId = VLD0;
    rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }

    pucBuf = (UCHAR*)VIRTUAL(u4Addr);
    if ((pucBuf[0] & 0x20) == 0x20)
    {
        rPesInfo.fgSeqHdr = TRUE;
    }

    if (rPesInfo.fgSeqHdr)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(prSwdmxInst->u1VdecId, VDEC_FMT_MP4, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
        fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_MP4, &rPesInfo, &_arSeqInfo[u1SrcId]);
        if(fgRet)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
        }
        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
    }

    return TRUE;
}
#endif


#ifdef SWDMX_MPG_AVS_SUPPORT
BOOL _SWDMX_MpgVideoCallbackAVSParse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.ucMpvId = VLD0;
    rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }

    rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    if (rPesInfo.ucPicType == 0xB0) // seq header
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(prSwdmxInst->u1VdecId, VDEC_FMT_AVS, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
        fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_AVS, &rPesInfo, &_arSeqInfo[u1SrcId]);
        if(fgRet)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
        }
        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackAVSParse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}
#endif

BOOL _SWDMX_MpgVideoCallbackH264Parse(UINT8 u1SrcId, const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallbackH264Parse prPes NULL\n");
        return FALSE;
    }

    x_memset(&rPesInfo, 0, sizeof(rPesInfo));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rPesInfo.u4VldReadPtr = u4Addr;

    fgRet = TRUE;
    if((rPesInfo.ucPicType & 0xF) == 0x7) // SPS_NALU
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(prSwdmxInst->u1VdecId, VDEC_FMT_H264, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        rPesInfo.ucMpvId = VLD0;
        rPesInfo.ucEsId = prSwdmxInst->u1VdecId;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_arSeqInfo[u1SrcId], 0, sizeof(_arSeqInfo[u1SrcId]));
        fgRet = VDEC_PrsSeqHdr(prSwdmxInst->u1VdecId, VDEC_FMT_H264, &rPesInfo, &_arSeqInfo[u1SrcId]);
        if(fgRet)
        {
            //LOG(5, "_SWDMX_MpgVideoCallbackH264Parse rSeqInfo %dX%d\n",
            //    rSeqInfo.u2_width, rSeqInfo.u2_height);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
        }

        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        return TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackH264Parse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}

VOID _SWDMX_MpgSeekFinNotify(UINT8 u1SrcId, UINT32 u4PTS, BOOL fgAgain)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    LOG(6, "_SWDMX_MpgSeekFinNotify\n");
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);
    //VERIFY (x_sema_unlock(_hPlayMgrSemaFinSeek) == OSR_OK);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
    if(fgAgain)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount++;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
    }
    UNUSED(u4PTS);
}

#if 0
BOOL _SWDMX_MpgPsInfoNotify(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const VOID* pvNotifyTag)
{
    UINT32 u4DetectFin = 0;
    INT32 i4Ret;
    BOOL fgRet;

    DMX_NOTIFY_INFO_STREAM_ID_T *prInfo;

    if(eCode == DMX_NOTIFY_CODE_STREAM_ID)
    {
        prInfo = (DMX_NOTIFY_INFO_STREAM_ID_T*)u4Data;

        // parse stream id.
        fgRet = _SWDMX_MpgSelectPSStream(prInfo);
        // ~parse

        if(fgRet)
        {
            u4DetectFin = DETECT_OK;
        }
        else
        {
            u4DetectFin = DETECT_FAIL;
        }

        i4Ret = x_msg_q_send(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ, (VOID *)&u4DetectFin, sizeof(UINT32), 255);

        if(i4Ret != OSR_OK)
        {
            return FALSE;
        }
    }
    return TRUE;
}
#endif

BOOL _SWDMX_MpgResetFeeder(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    UINT32 u4ReqCnt;
    INT32 i4Ret;
    UINT32 u4AlignSize;
    FEEDER_TRANSMIT_COND rFeederCond;

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    #ifdef SWDMX_MPG_DEBUG_BUFFER
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes > 0))
    {
        LOG(7, "Mpg Fd Flh UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d UAdr %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    }
    #endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID += SWDMX_FEEDER_ID_GAP; // always a even value
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;

    for(u4ReqCnt = 0;u4ReqCnt < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum;u4ReqCnt++)
    {
        // make it invalid, not a even value
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt] = 1;
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqDup[u4ReqCnt] = 0;
#endif
    }

    #if 1//def CC_DLNA_SUPPORT
    FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
    FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
    FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, FALSE);
    #endif
    // Update read ptr to feeder
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else
    {
        if(prSwdmxInst->rFeederInfo.u4WriteAddr)
        {
            if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, prSwdmxInst->rFeederInfo.u4WriteAddr) != FEEDER_E_OK)
            {
                LOG(1, "_SWDMX_MpgResetFeeder FeederUpdateReadPointer fail\n");
            }
        }
    }
    // DDI is 4 byte alignment
    u4AlignSize = (prSwdmxInst->rFeederInfo.u4WriteAddr % 4);
    if(u4AlignSize > 0)
    {
        LOG(5, "Mpg Fd align u4WriteAddr %d\n",
            prSwdmxInst->rFeederInfo.u4WriteAddr);
        i4Ret = FeederSetRequest(prSwdmxInst->eFeederSrc,
                (4-u4AlignSize),
                (UINT64)(0),
                &rFeederCond,
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-25)); // 25 is a past requestID
        if(i4Ret != FEEDER_E_OK)
        {
            LOG(1, "Mpg Fd align FeederSetRequest Fail\n");
            /* not necessary
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
            {
                _SWDMX_MpgHandleEOS(i4PlaySpeed);
            }
            if(prRangeList)
            {
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            */
            return FALSE;
        }
    }
    return TRUE;
}

VOID _SWDMX_MpgResetInputData(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    _SWDMX_FlushReqMsgQ(u1SrcId);
    UNUSED(_SWDMX_MpgResetFeeder(u1SrcId));

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
}

VOID _SWDMX_MpgResetData(UINT8 u1SrcId)
{
    BOOL fgRet;
    UINT32 i;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    _SWDMX_MpgFlushAFifo(u1SrcId);
    _SWDMX_MpgFlushVFifo(u1SrcId);
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio));
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo));
    }
    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
        (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
    {
        _SWDMX_MpgFlushVFifo2(u1SrcId);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2));
        }
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    //if(prSwdmxInst->fgEnVideoDmx)
    //{
    //    SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //    SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //}
    //_u4BufStart = 0;
    _SWDMX_FlushReqMsgQ(u1SrcId);
    UNUSED(_SWDMX_MpgResetFeeder(u1SrcId));
    // This might put before _SWDMX_MpgFlushAFifo, DMX_MM_FlushBuffer
    // because write might meet read pointer, and DDI is in full status,
    // then _SWDMX_DMX_Reset will cause DMX in strange status.
    // so DMX_DDI_MoveData can't return when next call DMX_DDI_MoveData.
    if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        fgRet = _SWDMX_DMX_Reset(u1SrcId);//DMX_DDI_RequestReset();
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgResetData _SWDMX_DMX_Reset Fail\n");
        }
    }
    //#define DMXCMD_READ321(offset)            IO_READ32((IO_VIRT + 0x17000), ((offset) * 4))
    //LOG(3, "_SWDMX_MpgResetData DMXCMD 0x%x\n", DMXCMD_READ321(26));

    // For es file, DMX_DDI_RequestReset will reset the value set
    // by DMX_ES_SetFileLength. So set it again.
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
        {
            fgRet = DMX_ES_SetFileLength(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo,(UINT32)0xFFFFFFFF);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgResetData _DMX_ES_SetFileLength Fail\n");
            }
        }
    }


    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    // DTV00211818, mp3 audio es
    // pause and seek, then play
    // we will have fgSendEOF = 1, clean it.
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
    //_u4AvailSize = 0;

    // DTV00212560, [5387-FLIP][MM]Total time change after time search to the beginning with special file
    // when change position, we should clean LastPTS data.
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTSCnt = 0;
    for(i=0;i<MPG_PTS_CHECK_NUM;i++)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] = 0;
    }

#ifdef TIME_SHIFT_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4QueueTickNum = 0xFFFFFFFF;
    for(i=0;i<MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER;i++)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] = (0);
    }
#endif

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSystemB)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize = 0x100000;
    }

    //DTV00135555 [MM]TV stop at current video file after excute play and FF
    //DTV00210492 when do seek or trick after sended EOS, RangeStatus should set NOT_FINISHED
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

#ifdef SWDMX_MPG_DEBUG_FIFO
#ifdef SWDMX_MPG_AFIFO_SIZE
    _u4AFifoCur = _u4AFifoStart;
    _u4AudioLastWriter = 0;
#endif
#ifdef SWDMX_MPG_DMXFIFO_SIZE
    _u4DmxFifoCur = _u4DmxFifoStart;
#endif
#endif
}

VOID _SWDMX_MpgResetVariable(UINT8 u1SrcId)
{
    UINT32 i;
     SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
     SWDMX_MPG_VERIFY_NULL(prSwdmxInst);
     _SWDMX_MpgResetData(u1SrcId);
    //_SWDMX_FlushDataMsgQ();

    #if 0 // move to ResetData
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
    #endif
#if 1//def CC_DLNA_SUPPORT
    FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
    // for step lose audio
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTSCnt = 0;
    for(i=0;i<MPG_PTS_CHECK_NUM;i++)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.au4LastPTS[i] = 0;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS	= 0;    //DTV00093908	
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoPTS = 0;                          //DTV00139313
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
	prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetFirstAudPts = FALSE;    //DTV00093908
#ifdef TIME_SHIFT_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust = FALSE;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum = 0;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideoWptr = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastVideo2Wptr = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize = 0;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetKeep = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetIKeep = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4TempRefKeep = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2DecodingOrderKeep = 0;

    #if 1//def CC_DLNA_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgLastData = FALSE;
    #endif
}

VOID _SWDMX_MpgJumpToNormal(UINT8 u1SrcId, BOOL fgSeek, BOOL fgSeekOffset, BOOL fgJumpping, INT32 i4Speed)
{
    BOOL fgRet;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    VDP_PIC_INFO_T rPicInfo;
    BOOL fgNoI;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    x_memset(&rPicInfo, 0, sizeof(VDP_PIC_INFO_T));
    if(fgJumpping || fgSeek)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
            if(!fgSeek)
            {
                u8Offset = 0;
                u8OffsetI = 0;
                u2DecodingOrder = 0;
                i4TempRef = 0;
                fgNoI = FALSE;
                #ifdef TIME_SHIFT_SUPPORT
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                   ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE)))
                {
                    // Do not change corrent position when we are no signal/bad now.
                    u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                    u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                }
                else
                #endif
                {
                    // from now, we use file position of last displayed picture.
                    fgRet = (VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));

                    if(fgRet)
                    {
                        if(((UINT32)(u8OffsetI>>32) == (UINT32)0xFFFFFFFF)
                            && ((UINT32)(u8OffsetI) == (UINT32)0xFFFFFFFF))
                        {
                            u8OffsetI = u8Offset;
                            LOG(3, "_SWDMX_MpgJumpToNormal No I, start from 0x%llx\n",
                                u8OffsetI);
                            fgNoI = TRUE;
                        }
                    }
                    else
                    {
                        u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                        u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                        fgNoI = TRUE;
                        LOG(5, "_SWDMX_MpgJumpToNormal VDP_GetFilePosition fail\n");
                    }

                    if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
                    {
                        LOG(3, "_SWDMX_MpgJumpToNormal VDP_GetFilePosition u8OffsetI 0x%llx > u8FileSize 0x%llx\n",
                            u8OffsetI,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
                        u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                        u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                    }

                    fgRet = (VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rPicInfo));
                    if(fgRet)
                    {
                        #ifdef TIME_SHIFT_SUPPORT
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = rPicInfo.u4TickNum;
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
                        #endif                        
                        // DTV00352271, reset the pts if can get form vdp.
                        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed == 1)
                        {
                            STC_SetStcValue(prSwdmxInst->u1StcId, rPicInfo.u4Pts);
                            LOG(5, "the reset stc after step is 0x%x!\n", rPicInfo.u4Pts);
                        }
                    }
                    else
                    {
                        #ifdef TIME_SHIFT_SUPPORT
                        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = rPicInfo.u4TickNum;
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
                        #endif
                        LOG(5, "_SWDMX_MpgJumpToNormal VDP_GetPicInfo fail\n");
                    }
                }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = u8Offset;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = u8OffsetI;
                
                if(fgNoI)
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    }
                    LOG(5, "_SWDMX_MpgJumpToNormal VDEC_SetRenderFromFirstPic from 0x%llx\n", u8OffsetI);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                }
                else
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                    LOG(5, "_SWDMX_MpgJumpToNormal VDEC_SetRenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        u8OffsetI, u8Offset, rPicInfo.u4Pts, i4TempRef);
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = ((rPicInfo.u4Pts - (MPG_STC_BASE))+ prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                }
            }
            else if(fgSeekOffset)
            {
                LOG(5, "_SWDMX_MpgJumpToNormal fgSeek fgSeekOffset\n");
            }
            else
            {
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                }                
                LOG(5, "_SWDMX_MpgJumpToNormal fgSeek\n");
            }
            //STC_StopStc(prSwdmxInst->u1StcId);
            //STC_SetStcValue(prSwdmxInst->u1StcId, 0);
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
            UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId));
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, FALSE);
                UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id));
            }
            VDP_ResetPts();
            UNUSED(VDEC_RegTrickPtsCb(prSwdmxInst->u1VdecId,
                        NULL, u1SrcId));

			if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)||
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)) // set the flag for TS H.264 wait IDR frame
			{
			   VDEC_SetWaitPvrSps(prSwdmxInst->u1VdecId,1);
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
			        VDEC_SetWaitPvrSps(prSwdmxInst->u1Vdec2Id,1);
                }
			}
            /*
            if(!fgSeek)
            {
                if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
                }
                else
                {
                    ASSERT(0);
                }
            }*/
            // audio is stopped already.
            ////AUD_WaitDspFlush(prSwdmxInst->u1AdecId);
        }
        else
        {
            #ifdef TIME_SHIFT_SUPPORT
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                BOOL fgRet;
                u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                fgRet = _SWDMX_MpgGetPosByTickNum(u1SrcId,
                    TRUE,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum,
                    &u8Offset);
                if(fgRet)
                {
#if 1
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum =
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
#endif
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8Offset;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = u8Offset;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = u8Offset;
                }
                else
                {
                    LOG(5, "_SWDMX_MpgJumpToNormal: CurTickNum %d fail\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
                }
            }
            #endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;

        _SWDMX_MpgResetData(u1SrcId);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
        // Don't need to change it when chagne speed.
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;
        //_fgSetAudioStartTime = FALSE;

        LOG(5, "_SWDMX_MpgJumpToNormal Jump\n");
    }
    else
    {   
        //DTV00320791, when change from "step" to "normal play" we should set STC value agian.
	    fgRet = (VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rPicInfo));
		if((fgRet)&&
			((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)||
			 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
			 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
			 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES)||
			 (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)))
		{
            
			STC_SetStcValue(prSwdmxInst->u1StcId, rPicInfo.u4Pts);
			STC_StartStc(prSwdmxInst->u1StcId);

			LOG(5, "_SWDMX_MpgJumpToNormal set STC value (0x%x)\n",rPicInfo.u4Pts);
		}
        LOG(5, "_SWDMX_MpgJumpToNormal\n");
    }
    UNUSED(i4Speed);
}

// return TRUE if we found sequence
BOOL _SWDMX_MpgGetFirstSeqHdr(UINT8 u1SrcId)
{   // this is first run for one file, we have to get sequence header
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        ((prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2) ||
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264) ||
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1) ||
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4) ||
        (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)))
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 0;
            #if 1//def CC_DLNA_SUPPORT
            FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
            FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, FALSE);
            #endif
            _SWDMX_MpgResetData(u1SrcId);
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr to find sequence\n");
        }
        else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr)
        {
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr finding sequence\n");
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
#ifdef CC_SWDMX_RENDER_POSITION
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->fgSetRenderPosition))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->fgSetRenderPosition = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8StartPts;

                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize == (UINT64)-1)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset;
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    LOG(5, "FirstSeqHdr FromFirstPic I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef);
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    }
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u4Timestap;
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, TRUE,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u2DecodingOrder));
                    LOG(5, "FirstSeqHdr RenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef);
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, TRUE,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u2DecodingOrder));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    }
                }
            }
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            _SWDMX_MpgResetData(u1SrcId);
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr found sequence, move to 0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize);
            return TRUE;
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            _SWDMX_MpgResetData(u1SrcId);
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr reset position\n");
            return TRUE;
        }
    }
    return FALSE;
}


VOID _SWDMX_MpgHandleSeekAudio(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgSeek)
{
    UINT32 u4Speed;
    UINT64 u8Tmp;
    UINT64 u8Remainder;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(i4PlaySpeed >= 0)
    {
        LOG(3, "_SWDMX_MpgHandleSeekAudio i4PlaySpeed >= 0\n");
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
        return;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime == 0)
    {
        LOG(3, "_SWDMX_MpgHandleSeekAudio u4TotalTime == 0\n");
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
        return;
    }

    u4Speed = (i4PlaySpeed > 0) ? i4PlaySpeed : -i4PlaySpeed;
    u4Speed /= 1000;
    u8Tmp = _SWDMX_Div6464(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*MPG_AUDIO_ES_SKIP_TIME), (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
    u8Tmp *= u4Speed;
    UNUSED(u8Remainder);
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos > u8Tmp)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos -= u8Tmp;

        {
            UINT64 u8Remainder;
            UNUSED(_SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos -= u8Remainder;
        }

        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                UINT64 u8IdxBaseAddr;
                UINT64 u8IdxStartAddr;
                UINT64 u8IdxEndAddr;
                u8IdxBaseAddr =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                prTblEntry = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if((prTblEntry) && (prIdxTbl))
                {
                    if(prIdxTbl->ui4_number_of_entry > 0)
                    {
                        u8IdxEndAddr = u8IdxBaseAddr +
                            (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                            (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                        if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos) >=
                            (u8IdxEndAddr)
                           )
                           ||
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum >= prIdxTbl->ui4_number_of_entry)
                          )
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum = (prIdxTbl->ui4_number_of_entry-1);
                        }
                        while(1)
                        {
                            u8IdxEndAddr = u8IdxBaseAddr +
                                (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                                (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                            if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos) >=
                                (u8IdxBaseAddr +
                                 prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset
                                )
                               )
                               &&
                               ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos) <
                                (u8IdxEndAddr)
                               )
                              )
                            {
                                break;
                            }
                            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum == 0)
                            {
                                break;
                            }
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum--;
                        }
                        u8IdxStartAddr = u8IdxBaseAddr +
                            prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset;
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = u8IdxStartAddr;
                    }
                    else
                    {
                        LOG(3, "_SWDMX_MpgHandleSeekAudio ui4_number_of_entry 0\n");
                    }
                }
                else
                {
                    LOG(3, "_SWDMX_MpgHandleSeekAudio prTblEntry NULL\n");
                }
            }

        }

        LOG(5, "_SWDMX_MpgHandleSeekAudio new pos 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos);
    }
    else
    {
        // end seek
        //change for DTV00098584
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
        
        LOG(5, "_SWDMX_MpgHandleSeekAudio end seek\n");
    }

    // can't flush audio data, or we can't play audio
    //_SWDMX_MpgResetData(u1SrcId);
    _SWDMX_FlushReqMsgQ(u1SrcId);
    UNUSED(_SWDMX_MpgResetFeeder(u1SrcId));
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    _SWDMX_MpgModifyPlayPos(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
}

static UINT64 _SWDMX_MpgGetAvgBitRate(UINT8 u1SrcId)
{
    UINT64 u8AvgSize = 0x7800 * 30;      // Set default AvgSize
    SWDMX_INFO_T *prSwdmxInst = NULL;
    VDP_PIC_INFO_T rPicInfo;

    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        LOG(1, "Get Inst Failed. Inst(%d)\n", u1SrcId);
        goto EXIT_POINT;
    }
    
    UNUSED(rPicInfo);
    
#ifdef TIME_SHIFT_SUPPORT
    if (SWDMX_FMT_MPEG2_TS_TIME_SHIFT == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt)
    {
        UINT32 u4TickStart, u4TickEnd, u4Tmp, u4TickNum, u4Time;
        UINT64 u8StartPos, u8EndPos, u8Remainder;

        if (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
        {
            LOG(3, "_SWDMX_MpgGetAvgBitRate valid range is not set\n");
            goto EXIT_POINT;
        }
        
        _SWDMX_LockRangeSema(u1SrcId);
        
        // Calc current startTick
        {
            if (VDP_SET_OK == VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rPicInfo))
            {
                u4TickStart = rPicInfo.u4TickNum;
            }
            else
            {
                u4TickStart = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
            }
        }
        
        // Get the End Tick num, 4s
        if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase>0 &&
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase<1000)
        {
            u4TickEnd = u4TickStart + 4*(1000/prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase);
        }
        else
        {
            LOG(3, "Src[%d] Error u4TickBase(%d)... ...\n", u1SrcId, 
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase);
            _SWDMX_UnlockRangeSema(u1SrcId);
            goto EXIT_POINT;
        }

        // Modify Tick number to Safe range
        {
            if (u4TickEnd > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick)
            {
                u4Tmp = 0;
                u4Tmp = u4TickEnd - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick;
                u4TickEnd = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick;

                if (u4TickStart > u4Tmp)
                {
                    u4TickStart -= u4Tmp;
                }
                else
                {
                    u4TickStart = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick;
                }
            }
        } 
        
        if ( u4TickEnd<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick ||
             u4TickEnd>prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick )
        {
            _SWDMX_UnlockRangeSema(u1SrcId);
            goto EXIT_POINT;
        }
        
        if ( u4TickStart<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick ||
             u4TickStart>prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick )
        {
            _SWDMX_UnlockRangeSema(u1SrcId);
            goto EXIT_POINT;
        }

        if (0 == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick)
        {
            _SWDMX_UnlockRangeSema(u1SrcId);
            goto EXIT_POINT;
        }
        
        if (u4TickEnd <= u4TickStart)
        {
            LOG(3, "Src[%d] End<Start u8AvgSize(0x%llX)\n", u1SrcId, u8AvgSize);
            _SWDMX_UnlockRangeSema(u1SrcId);
            goto EXIT_POINT;
        }

        _SWDMX_UnlockRangeSema(u1SrcId);
        
        // Get position
        if (!_SWDMX_MpgGetPosByTickNum(u1SrcId, FALSE, u4TickStart, &u8StartPos))
        {
            LOG(3, "_SWDMX_MpgGetAvgBitRate Error, u4TickStart(%d) u8AvgSize(0x%llX)\n",
                   u4TickStart, u8AvgSize);
            goto EXIT_POINT;
        }

        if (!_SWDMX_MpgGetPosByTickNum(u1SrcId, FALSE, u4TickEnd, &u8EndPos))
        {
            LOG(3, "_SWDMX_MpgGetAvgBitRate Error, u4TickEnd(%d) u8AvgSize(0x%llX)\n",
                   u4TickEnd,  u8AvgSize);
            goto EXIT_POINT;
        }

        if (0 == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize)
        {
            LOG(3, "Src[%d] No Data info... ...\n", u1SrcId);
            goto EXIT_POINT;
        }
                
        u4TickNum = u4TickEnd - u4TickStart;
        // Calc AvgSize
        UNUSED(_SWDMX_Div6464((u8EndPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize - u8StartPos), 
                               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));
        u4Time = u4TickNum*10 / (1000/prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase);

        u8AvgSize = _SWDMX_Div6464(u8Remainder*10, u4Time, &u8Remainder);

        if (u8AvgSize < 0xE000)     // We assume the low bitrate (in Byte) should larger than 0xE000 and high bitrate should less than 0xA000000 
        {
            u8AvgSize = 0xE000;
        }
        else if (u8AvgSize > 0xA000000)
        {
            u8AvgSize = 0xA000000;
        }
        
        LOG(5, "S(%d) E(%d) Range(0x%llX-0x%llX) AS(0x%llX)\n",
                u4TickStart, u4TickEnd, u8EndPos, u8StartPos, u8AvgSize);
    }
    else
#endif
    {
        // u8AvgSize = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
        LOG(3, "Src[%d] _SWDMX_MpgGetAvgBitRate Only used for Timeshift... ...\n", u1SrcId);
        goto EXIT_POINT;
    }
    
EXIT_POINT:
    return u8AvgSize;
}

VOID _SWDMX_MpgHandleSeek(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgSeek)
{
    UINT32 u4Speed;
    UINT32 u4Tmp;
    UINT32 u4SkipTime;
    UINT64 u8Tmp;
    UINT64 u8Size;
    UINT64 u8SizeWithoutSkipTime;
    UINT64 u8Remainder;
    HAL_TIME_T rDelta;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    UINT64 u8SendSizeWOIdx; // withoud index
    VDP_PIC_INFO_T rPicInfo;
    BOOL fgNoI;
    UINT32 u8SizeCompare;
    BOOL fgTotalTimeOffsetFake=FALSE;
    UINT32 u4FrameRate = 30;
    UINT32 u4PreFrameTime = 3000;
    
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
    _SWDMX_MpgResetData(u1SrcId);

    u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;

    x_memset(&rPicInfo, 0, sizeof(rPicInfo));
    HAL_GetTime(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpEndTime);
    HAL_GetDeltaTime(&rDelta, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpStartTime, &prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpEndTime);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpStartTime.u4Micros = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpEndTime.u4Micros;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpStartTime.u4Seconds = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpEndTime.u4Seconds;
    if(rDelta.u4Seconds > 3)
    {
        rDelta.u4Seconds = 3;
    }
    u4SkipTime = ((rDelta.u4Seconds*1000000) + rDelta.u4Micros);

    // Get Frame Rate from VDEC
    {
        VDEC_HDR_INFO_T rHdrInfo;
                
        x_memset(&rHdrInfo, 0, sizeof(VDEC_HDR_INFO_T));
        if (VDEC_QueryInfo(prSwdmxInst->u1VdecId, &rHdrInfo))
        {
            u4FrameRate = (rHdrInfo.u2FrmRate + 99) / 100;
        }

        if ((0==u4FrameRate) || (300<u4FrameRate))
        {
            LOG(1, "Error Frame rate, set to 30fps.\n");
            u4FrameRate = 30;
        }

        ASSERT(u4FrameRate);
        u4PreFrameTime = 90000/u4FrameRate;
        ASSERT(u4PreFrameTime);
    }
    
    // from now, we use file position of last displayed picture.
    /*
    u8Offset = u8OffsetI = 0;
    u2DecodingOrder = 0;
    i4TempRef = 0;
    VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
    if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
    {
        LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI %d > u8FileSize %d\n",
            (UINT32)u8OffsetI, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
        u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
    }*/
    u8Offset = 0;
    u8OffsetI = 0;
    u2DecodingOrder = 0;
    i4TempRef = 0;
    fgNoI = FALSE;

#ifdef TIME_SHIFT_SUPPORT
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        u8Offset = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        u2DecodingOrder = 0;
        i4TempRef = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        LOG(5, "_SWDMX_MpgHandleSeek Scramble  u8SendedSize 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
    }
    else
#endif
    {
        UNUSED(VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
        UNUSED(VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rPicInfo));
        LOG(5, "HandleSeek Position i4TempRef %d u2DecodingOrder %d I:0x%llx 0x%llx\n",
                i4TempRef, u2DecodingOrder, u8OffsetI, u8Offset);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS =
            ((rPicInfo.u4Pts - (MPG_STC_BASE)) + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
    }

    if(((UINT32)(u8OffsetI>>32) == (UINT32)0xFFFFFFFF)
        && ((UINT32)(u8OffsetI) == (UINT32)0xFFFFFFFF))
    {
        u8OffsetI = u8Offset;
        LOG(3, "_SWDMX_MpgHandleSeek No I, start from 0x%llx\n", u8OffsetI);
        fgNoI = TRUE;
    }

    if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
    {
        LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI 0x%llx > u8FileSize 0x%llx\n",
                u8OffsetI, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
        u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
        u2DecodingOrder = 0;
        i4TempRef = 0;
    }

    /*
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS =
        ((rPicInfo.u4Pts - (MPG_STC_BASE)) + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);

#ifdef TIME_SHIFT_SUPPORT
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        // no need this
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    }
#endif
    */

    if(i4PlaySpeed == -1)
    {
        //u8Offset = u8OffsetI = 0;
        //u2DecodingOrder = 0;
        //i4TempRef = 0;
        // from now, we use file position of last displayed picture.
        /*VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
        if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
        {
            LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI %d > u8FileSize %d\n",
                (UINT32)u8OffsetI, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
            u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
            u2DecodingOrder = 0;
            i4TempRef = 0;
        }
        LOG(5, "_SWDMX_MpgHandleSeek VDP_GetFilePosition i4TempRef %d u2DecodingOrder %d u8Offset %d\n",
            i4TempRef, u2DecodingOrder, (UINT32)u8Offset, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
        */
        //if(u8OffsetI == u8Offset)
        {
            if((u8OffsetI == u8Offset) &&
               (u8Offset == 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount))
            {
                // end seek
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = TRUE; // FixMe, should be FALSE
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4Ret = _SWDMX_MpgHandleRing(
                            u1SrcId,
                            0, // begining of file. prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE),
                            &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                            
                    UNUSED(u4Ret);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                #endif
                {
                    LOG(5, "_SWDMX_MpgHandleSeek end seek\n");
                }
                
                u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, (UINT64)0));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    }
                }
            }
            else
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    // GOP boundry is there. Back a GOP.
                    // prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime unit is 90000
                    //u4Tmp = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime/90000)*30; // total frames
                    //u8Size = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize/u4Tmp); // avg size/frame
                    //u8Size *= (u4Speed/1000);
                    u4Tmp = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset) / u4PreFrameTime); // total frames
                    u8Size = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), 
                                            (UINT64)u4Tmp, &u8Remainder);

                    //DTV00212070  check  is prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset  normal ?
                    u8SizeCompare = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
                    if (u8SizeCompare>6*u8Size)
                    {
                         u8Size= u8SizeCompare;
                         fgTotalTimeOffsetFake=TRUE;
                         LOG(5,"SWDMX_HandleSeek TotalTimeOffset is Wrong\n");
                    }
                    u8Size *= u4FrameRate; // 1 sec
                    //ASSERT(u8Size < (UINT64)0xFFFFFFFF);
                    //u4Tmp = (UINT32)u8Size;
                    //u4Tmp = (u4Tmp%(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase));
                    //u8Size -= (UINT64)u4Tmp;
                    // error handleing for (u8Size >= (UINT64)0xFFFFFFFF)
                    UNUSED(_SWDMX_Div6464(  (u8Size - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), 
                                            (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase), &u8Remainder));
                    u8Size -= u8Remainder;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                    {
                        u8Size *= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount;
                    }
                    
                    if(u8OffsetI <= u8Size)
                    {
                        u8OffsetI = 0;
                    }
                    else
                    {
                        u8OffsetI -= u8Size;
                    }
                }

                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = u8OffsetI;
                u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = u8OffsetI;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                
                if(fgNoI)
                {
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    LOG(5, "HandleSeek FromFirstPic from 0x%llx\n", u8OffsetI);
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    }
                }
                else
                {
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                    UNUSED(VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rPicInfo));
                    LOG(5, "HandleSeek BackStep I:0x%llx, 0x%llx, PTS:%d, i4TempRef:%d, u2DecodingOrder:%d\n", u8OffsetI,
                        u8Offset, rPicInfo.u4Pts, i4TempRef, (UINT32)u2DecodingOrder);
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, (UINT64)0));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                    }
                }
                
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                }
            }
        }
    }
    else
    {
        u4Speed = (i4PlaySpeed < 0) ? ((UINT32)-i4PlaySpeed) : ((UINT32)i4PlaySpeed);

        // remove prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset
        // when totaltime is 0, we use average frame size from vdec.
        if (((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime) > u4PreFrameTime) && 
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > 0))
        {
            // prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime unit is 90000
            //u4Tmp = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime/90000)*30; // total frames
            //u8Size = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize/u4Tmp); // avg size/frame
            //u8Size *= (u4Speed/1000);
            u4Tmp = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset)
                     / u4PreFrameTime); // total frames
            if (u4Tmp > 0) // prevent div zero
            {
                u8Size = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), 
                                        (UINT64)u4Tmp, &u8Remainder);
            }
            else
            {
                u8Size = 1;
            }

            //DTV00212070  check  is prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset  normal ?
            u8SizeCompare=(UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
            if (u8SizeCompare>6*u8Size)
            {
                 u8Size = u8SizeCompare;
                 fgTotalTimeOffsetFake=TRUE;
                 LOG(5,"SWDMX_HandleSeek TotalTimeOffset is Wrong\n");
            }
            u8Tmp = u8Size;
            u8Size *= (u4Speed / 1000);
            //if(i4PlaySpeed < 0) //DTV00212070
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount);
                }
            }
            u8SizeWithoutSkipTime = u8Size;

            // This is the time we spent on the sending I frame
            if(u4SkipTime > 3000000) // 3 sec
            {
                LOG(5, "_SWDMX_MpgHandleSeek u4SkipTime %d\n", (UINT32)u4SkipTime);
                u4SkipTime = 3000000;
            }
            
            if(u4SkipTime > 10000) // 10 ms
            {
              if(fgTotalTimeOffsetFake==TRUE) //DTV00212070
              {

                u8Tmp = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
                u8Tmp *= u4FrameRate;
                u8Tmp *= u4SkipTime;
                u8Tmp = _SWDMX_Div6464(u8Tmp, (UINT64)1000000, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
               }
              else
              {
                // a = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize / u4TotalTime -> 90K base
                // b = (u4SkipTIme / 1000000) * (a/90K)
                // b = (u4SkipTIme / 1000000) * ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize / u4TotalTime)/90000)
                // b = (u4SkipTIme / 100) * ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize / u4TotalTime)/9)
                u8Tmp = (u4SkipTime/100);
                u8Tmp *= 9;
                u8Tmp = (u8Tmp * (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)); //1000000 / 90000 -> 100/9
                u8Tmp = _SWDMX_Div6464(u8Tmp,
                    ((UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset),
                    &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
              }
            }
        }
        else
        {
            UINT64 u8AvgSize = 0;

            if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                u8AvgSize = _SWDMX_MpgGetAvgBitRate(u1SrcId);
                u8AvgSize = _SWDMX_Div6464(u8AvgSize, u4FrameRate, &u8Remainder);
            }
            else
            {
                u8AvgSize = (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
            }

            u8Size = u8AvgSize; // (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
            u8Tmp = u8Size;
            u8Size *= (UINT64)(u4Speed / 1000);
            //if(i4PlaySpeed < 0)       //DTV00212070
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount);
                }
            }
            u8SizeWithoutSkipTime = u8Size;
            {
                UINT64 u8Remainder;
                UNUSED(_SWDMX_Div6464(u8SizeWithoutSkipTime, (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
                u8SizeWithoutSkipTime -= u8Remainder;
            }

            // This is the time we spent on the sending I frame
            if (u4SkipTime > 3000000) // 3 sec
            {
                LOG(5, "_SWDMX_MpgHandleSeek u4SkipTime %d\n", (UINT32)u4SkipTime);
                u4SkipTime = 3000000;
            }
            
            if(u4SkipTime > 10000) // 10 ms
            {
                u8Tmp = u8AvgSize;  // (UINT64)VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId);
                u8Tmp *= u4FrameRate;
                u8Tmp *= u4SkipTime;
                u8Tmp = _SWDMX_Div6464(u8Tmp, (UINT64)1000000, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
            }
        }

        //ASSERT(u8Size < (UINT64)0xFFFFFFFF);
        // error handling no need now.
        {
            UINT64 u8Remainder;
            UNUSED(_SWDMX_Div6464(u8Size, (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
            u8Size -= u8Remainder;
        }

        if (i4PlaySpeed < 0)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr > u8Size)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
                // DTV00136835 [MM]Video auto resume when time seek to end in fr mode.
                if(fgSeek)
                {
                    // When we are seek, make the first file position const.
                    // Not affected by skip time.
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr > (u8SizeWithoutSkipTime * 2))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize =
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr - (u8SizeWithoutSkipTime * 2));
                    }
                    else
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
                    }
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr - u8Size);
                }
                /*if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    u4Tmp = (UINT32)(u8OffsetI - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize -= (u4Tmp/2);
                }*/
                u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    u4SkipTime += (u4Speed * u4FrameRate); 
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum >=
                        (u4SkipTime / prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum -=
                            (u4SkipTime / prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period);
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4OutReadSize = 0;
                    u4Ret = _SWDMX_MpgHandleRing(u1SrcId,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE),
                        &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                    if(u4Ret == MPG_RING_END_MEET)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                    }
                    else if(u4Ret != MPG_RING_OK)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                    }
                    else
                    {
                        if(_SWDMX_MpgGetIPosByTickNum(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum),
                             &u8OutReadAddr))
                        {
                            // change I frame start position
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8OutReadAddr;
                        }
                    }
                }
                #endif
                #if 0 // move up
                u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                #endif
                #ifdef SWDMX_MPG_USE_INDEX
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx)
                {
                    UINT64 u8RetPos;
                    if(!_SWDMX_MpgIdxGetPosByPos(u1SrcId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, &u8RetPos))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgIdxGetPosByPos fail\n");
                    }
                    LOG(5, "_SWDMX_MpgHandleSeek ff GetPosByPos 0x%llx -> 0x%llx\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u8RetPos);
                    // if previous jump get a initial value in index table
                    // this time we might get a correct one, but that is a bigger one.
                    // avoid this case
                    if(u8RetPos <= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8RetPos;
                    }
                }
                #endif
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    }
                    if(fgSeek)
                    {
                        // alwasy render it.
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                        {
                            UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                        }
                    }
                    else
                    {
                        // only render it before u8Offset.
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, u8Offset));
                        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                        {
                            UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, u8Offset));
                        }
                    }
                }
            }
            else
            {
                // end seek
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = TRUE; // FixMe, should be FALSE
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4Ret = _SWDMX_MpgHandleRing(u1SrcId,
                                0, // begining of file. prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE),
                                &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                    UNUSED(u4Ret);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                #endif
                {
                    LOG(5, "_SWDMX_MpgHandleSeek end seek\n");
                }
                
                u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, (UINT64)0));
                        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    }
                }
            }
        }
        else // i4PlaySpeed > 0
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
            // DTV00136835 [MM]Video auto resume when time seek to end in fr mode.
            if(fgSeek)
            {
                // When we are seek, make the first file position const.
                // Not affected by skip time.
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize =
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr + (u8SizeWithoutSkipTime * 2));
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr + u8Size);
            }
            u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            #ifdef TIME_SHIFT_SUPPORT
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                UINT32 u4Ret;
                UINT32 u4OutReadSize;
                UINT64 u8OutReadAddr;

                u4SkipTime += (u4Speed * u4FrameRate); 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum +=
                    (u4SkipTime / prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period);
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

                u4Ret = _SWDMX_MpgHandleRing(u1SrcId,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE),
                    &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                if(u4Ret != MPG_RING_OK)
                {
                    // this is end of current range case
                    // do not move send position.
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr)
                    {
                        u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                        LOG(6, "_SWDMX_MpgHandleSeek u8SendedSize(0x%llx) >= u8ValidEndAddr(0x%llx)\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr);
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                {
                    if(_SWDMX_MpgGetIPosByTickNum(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                         (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum),
                         &u8OutReadAddr))
                    {
                        // change I frame start position
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8OutReadAddr;
                    }
                }
            }
            #endif
            #if 0 // move up
            u8SendSizeWOIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            #endif
            /*if(u8OffsetI > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
            {
                u4Tmp = (UINT32)(u8OffsetI - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += (u4Tmp/2);
            }*/
            #ifdef SWDMX_MPG_USE_INDEX
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx)
            {
                UINT64 u8RetPos;
                if(!_SWDMX_MpgIdxGetPosByPos(u1SrcId, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, &u8RetPos))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgIdxGetPosByPos fail\n");
                }
                LOG(5, "_SWDMX_MpgHandleSeek fb GetPosByPos 0x%llx -> 0x%llx\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u8RetPos);
                // if previous jump get a initial value in index table
                // this time we might get a correct one, but that is a small one.
                // avoid this case
                if(u8RetPos >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = u8RetPos;
                }
            }
            #endif
            
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                }
                if(fgSeek)
                {
                    // alwasy render it.
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    }
                }
                else
                {
                    // only render it after u8Offset + onetimesize.
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE,
                            (u8Offset + (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize))));
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE,
                                (u8Offset + (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize))));
                    }
                }
            }
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
        UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE));
        if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            eVdecFmt = VDEC_FMT_MPV;
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm VDEC_FMT_MPV fail\n");
            }
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            eVdecFmt = VDEC_FMT_H264;
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm VDEC_FMT_H264 fail\n");
            }
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            eVdecFmt = VDEC_FMT_WMV;
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_VC1 fail\n");
            }
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
        {
            eVdecFmt = VDEC_FMT_AVS;
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_AVS fail\n");
            }
        }
        else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
        {
            eVdecFmt = VDEC_FMT_MP4;
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_MPEG4 fail\n");
            }
        }
        else
        {
            ASSERT(0);
        }
        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
        {
            if(!VDEC_PlayOneFrm(prSwdmxInst->u1Vdec2Id, eVdecFmt))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm 2 VDEC_FMT_H264 fail\n");
            }
        }
    }

    LOG(6, "_SWDMX_MpgHandleSeek new addr 0x%llx (WO Idx 0x%llx)\n",
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u8SendSizeWOIdx);

    // reset jump
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8SendSizeWOIdx;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;
}

VOID _SWDMX_MpgHandleChange(UINT8 u1SrcId, SWDMX_CUR_CMD_T* prCmd)
{
    BOOL fgRet;
    BOOL fgSeek;
    UINT32 u4Count;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    INT32 i4PlaySpeed;
    UINT64 u8SeekPos;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(!prCmd)
    {
        LOG(1, "_SWDMX_MpgHandleChange prCmd null\n");
        return;
    }
    i4PlaySpeed = prCmd->i4PlaySpeed;
    u8SeekPos = prCmd->u8SeekPos;
    fgSeek = FALSE;
    if(((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
        || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF))
    {
        LOG(5, "_SWDMX_MpgHandleChange change pos: 0x%llx -> 0x%llx ================\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u8SeekPos);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = TRUE;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetFirstAudPts = FALSE;  //DTV00093908
#ifdef TIME_SHIFT_SUPPORT
   #if 0 // DTV00077914 remove it  when in no signal status ,FW to end and speed change to 1x, will send a wrong "  MM_SIGNAL_N_TO_Y" to mw
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL)
        {
            LOG(3, "_SWDMX_MpgHandleChange: MM_SIGNAL_N_TO_Y u4NoSignalStage 0\n");
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_SIGNAL_N_TO_Y,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE)
        {
            LOG(3, "_SWDMX_MpgHandleChange: MM_BAD_TICK_Y_TO_N u4NoSignalStage 0\n");
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_BAD_TICK_Y_TO_N,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
  #endif
//#if 1
#if 0
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = NULL;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = NULL;

        {
            BOOL fgRet;
            UINT32 u4Temp;
            fgRet = _SWDMX_MpgGetTickByPos(u1SrcId,
                TRUE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                i4PlaySpeed,
                &u4Temp);
            if(fgRet)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
                LOG(5, "_SWDMX_MpgHandleChange tick num: %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
            }
            else
            {
                // give a initial value
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
                LOG(5, "_SWDMX_MpgHandleChange, NG, use safe start tick num: %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
            }
        }
#endif
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        //_fgSetAudioStartTime = FALSE;
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;
        u4Count = (UINT32)u8SeekPos;
        UNUSED(u4Count);
        if(u8SeekPos < (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;            
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        }
        else
        {
#ifdef TIME_SHIFT_SUPPORT
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
                    _SWDMX_MpgTSAlignDown(u1SrcId, u8SeekPos);
            }
            else
#endif
            {
                {
                    //UINT64 u8Remainder;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = u8SeekPos;
                    //UNUSED(_SWDMX_Div6464((u8SeekPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
                    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos -= u8Remainder;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        _SWDMX_MpgModifyPlayPos(u1SrcId);
                    }                    
                }
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

        //  DTV00211793 only audio, no video ,do seek when have send EOF, need clear the flags
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;

#ifdef TIME_SHIFT_SUPPORT           //DTV00077914
		if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
		{
			UINT32 u4Temp;

			fgRet = _SWDMX_MpgGetTickByPos(u1SrcId,
                        FALSE,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                        i4PlaySpeed,
                        &u4Temp);
			if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex) &&
									   (fgRet))
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = u4Temp;
				//LOG(5, "Trace u4CurTickNum(%d) _u8SendedSize(0x%llx)\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum, _u8SendedSize);
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime =
				(UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime *=
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase;
			}
			else
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
			}
		}
#endif


        _SWDMX_MpgResetData(u1SrcId);
        // DTV00132905, should stop timer bar
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            STC_StopStc(prSwdmxInst->u1StcId);
        }
        fgSeek = TRUE;
    }
    else if((prCmd->fgSeek) &&
            (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
    {
        BOOL fgNoI = FALSE;
        LOG(5, "_SWDMX_MpgSendData fgPlayChangeOffset RenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
            prSwdmxInst->rSeekOffset.ui8_i_frame_position,
            prSwdmxInst->rSeekOffset.ui8_frame_position,
            prSwdmxInst->rSeekOffset.ui8_pts_info,
            prSwdmxInst->rSeekOffset.i4_temporal_reference);

        u8SeekPos = prCmd->rSeekOffset.ui8_frame_position;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;

#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        //_fgSetAudioStartTime = FALSE;
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        //_fgSetAudioStartTime = FALSE;
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;

        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);

        if((((UINT32)(prCmd->rSeekOffset.ui8_i_frame_position>>32) == (UINT32)0xFFFFFFFF)
            && ((UINT32)(prCmd->rSeekOffset.ui8_i_frame_position) == (UINT32)0xFFFFFFFF)))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize =
                prCmd->rSeekOffset.ui8_frame_position;
            fgNoI = TRUE;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize =
                prCmd->rSeekOffset.ui8_i_frame_position;
            fgNoI = FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS =
            prCmd->rSeekOffset.ui8_pts_info;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

        //  DTV00211793 only audio, no video ,do seek when have send EOF, need clear the flags
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#ifdef TIME_SHIFT_SUPPORT           //DTV00077914
		if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
		{
			UINT32 u4Temp;
			fgRet = _SWDMX_MpgGetTickByPos(u1SrcId,
								FALSE,
								prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
								i4PlaySpeed,
								&u4Temp);
			if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex) &&
							   (fgRet))
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = u4Temp;
				//LOG(5, "Trace u4CurTickNum(%d) _u8SendedSize(0x%llx)\n", prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum, _u8SendedSize);
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime =
				(UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime *=
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase;
			}
			else
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
			}
		}
#endif

        if(fgNoI)
        {
            UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
            UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
            UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
            LOG(5, "SEEK_OFFSET VDEC_SetRenderFromFirstPic from 0x%llx\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
            }
        }
        else
        {
            UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, TRUE,
                prCmd->rSeekOffset.ui8_frame_position,
                prCmd->rSeekOffset.i4_temporal_reference,
                prCmd->rSeekOffset.ui2_decoding_order));
            UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
            UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, TRUE,
                    prCmd->rSeekOffset.ui8_frame_position,
                    prCmd->rSeekOffset.i4_temporal_reference,
                    prCmd->rSeekOffset.ui2_decoding_order));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
            }
            LOG(5, "SEEK_OFFSET VDEC_SetRenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                prCmd->rSeekOffset.ui8_i_frame_position,
                prCmd->rSeekOffset.ui8_frame_position,
                prCmd->rSeekOffset.ui8_pts_info,
                prCmd->rSeekOffset.i4_temporal_reference);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS =
                ((prCmd->rSeekOffset.ui8_pts_info - (MPG_STC_BASE))+ prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);
        }

        _SWDMX_MpgResetData(u1SrcId);
        // DTV00132905, should stop timer bar
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            STC_StopStc(prSwdmxInst->u1StcId);
        }
        fgSeek = TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        LOG(5, "_SWDMX_MpgHandleChange change speed: %d ====================\n", i4PlaySpeed);
#ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(!_SWDMX_PVR_SetDataMode(u1SrcId, i4PlaySpeed))
            {
                LOG(3, "_SWDMX_MpgHandleChange _SWDMX_PVR_SetDataMode fail\n");
            }
        }
#endif
        if(i4PlaySpeed == 1000)
        {
            // always do start to play behavior
            //_SWDMX_MpgJumpToNormal(fgSeek,
            //    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping,
            //    i4PlaySpeed);
            //_SWDMX_MpgJumpToNormal(u1SrcId, fgSeek, TRUE,
            //    i4PlaySpeed);
            //DTV00078801  for H264 video ES  flush will cause H.264 decdode can't found PPS.
            //DTV00086507  for video es 2x-->1x no needed to do flush
            if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES)||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES)||
                (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264))&&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 1) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
                (((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed <= 1500) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)) ||
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed <= 2000) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)))
#else
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed <= 2000)
#endif
                )

            {
				_SWDMX_MpgJumpToNormal(u1SrcId, fgSeek,
				    ((prCmd->fgSeek) &&
				     (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)),
				      FALSE,
					  i4PlaySpeed);
            }
			else
			{
				_SWDMX_MpgJumpToNormal(u1SrcId, fgSeek,
				    ((prCmd->fgSeek) &&
				     (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)),
				     TRUE,
				     i4PlaySpeed);

			}


            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;

        }
        else if (
            ((i4PlaySpeed > 1000) && (i4PlaySpeed <= 1500))
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            ||
             // when play time-shift, usb performace is not good enough
             // use jump mode to play 2x speed.
            (((i4PlaySpeed > 1500) && (i4PlaySpeed < 4000))
             &&
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
#endif
            ||
            ((i4PlaySpeed >= 1) && (i4PlaySpeed < 1000)))
        {
            BOOL fgStartToPlay = FALSE;
            // DTV00211049 [MM Compatibility]No audio output when play after FF & FB with special file
            // If previous speed == step and now != step, do a start to play behavior
            if (((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed == 1) &&
                 (i4PlaySpeed != 1))
                ||
                // even we are in 1x speed,
                // vdec will fail to get a frame vdp are pause
                // so if we change to step, we will lose one frame to display
                // add the condition to avoid it
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed != 1) &&
                 (i4PlaySpeed == 1))
                ||
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping)
            {
                fgStartToPlay = TRUE;
            }

            //DTV00078801  for H264 video ES  flush will cause H.264 decdode can't found PPS.
		   if(( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed ==1000)&&
		   (i4PlaySpeed >= 1)&& (i4PlaySpeed < 1000)&&
		   ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)||
		   	(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES)||
			(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)))
		   {
			 fgStartToPlay = FALSE;
		   }

            _SWDMX_MpgJumpToNormal(u1SrcId, fgSeek,
                ((prCmd->fgSeek) &&
				 (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET)),
                fgStartToPlay,
                i4PlaySpeed);

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = TRUE;
        }
        else
        {
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
            UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId));
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, FALSE);
                UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id));
            }
            VDP_ResetPts();

            _SWDMX_MpgResetData(u1SrcId);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
            FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
            // Don't need to change it when chagne speed.
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
            //_fgSetAudioStartTime = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;
            // from now, we use file position of last displayed picture.
            u8OffsetI = 0;
            u8Offset = u8OffsetI;
            u2DecodingOrder = 0;
            i4TempRef = 0;
            if(!fgSeek)
            {
                // if seek and trick, we might get the older position before seek.
                // we have to ignore the older position when seek.
                fgRet = VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
                if(fgRet)
                {
                    if(i4PlaySpeed > 0)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8Offset;
                        LOG(5, "HandleChange Jumpping 1 u8SendedSize 0x%llx u8JumpStartAddr 0x%llx\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                    }
                    else
                    {
                        if((((UINT32)(u8OffsetI>>32) == (UINT32)0xFFFFFFFF)
                            && ((UINT32)(u8OffsetI) == (UINT32)0xFFFFFFFF)))
                        {
                            u8OffsetI = u8Offset;
                            LOG(3, "_SWDMX_MpgHandleChange No I, start from 0x%llx\n",
                                u8OffsetI);
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                        LOG(5, "HandleChange Jumpping 2 u8SendedSize 0x%llx u8JumpStartAddr 0x%llx\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                    }
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8Offset;
                    LOG(5, "_SWDMX_MpgHandleChange VDP_GetFilePosition fail\n");
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

            LOG(5, "_SWDMX_MpgHandleChange VDEC_GetAvgPicSize %d\n",
                VDEC_GetAvgPicSize(prSwdmxInst->u1VdecId));

            HAL_GetTime(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rJumpStartTime);
            UNUSED(VDEC_RegTrickPtsCb(prSwdmxInst->u1VdecId,
                _SWDMX_MpgSeekFinNotify, u1SrcId));
            #ifdef TIME_SHIFT_SUPPORT
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL)
                {
                    _SWDMX_MpgHandleSeek(u1SrcId, i4PlaySpeed, fgSeek);
                }
            }
            else
            #endif
            {
                _SWDMX_MpgHandleSeek(u1SrcId, i4PlaySpeed, fgSeek);
            }
        }
    }
    else if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
             // when RAW_AUDIO_ES, we support -?x speed in driver build
             (((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
               (i4PlaySpeed < 0))
              ||
              (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
             )
            )
            ||
            // we should reset variable in audio only stream
            // (might not set audio yet)
            (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
           )
    {
        LOG(5, "_SWDMX_MpgHandleChange no video change speed: %d ====================\n", i4PlaySpeed);

        _SWDMX_MpgResetData(u1SrcId);
        if(i4PlaySpeed < 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = TRUE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
#if 1//def CC_DLNA_SUPPORT
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif

		// when audio es change speed or change position,
		// we should send data begining at the position which pts>0(begining of each index entry)
		 // so there will be a jumpping, after do seek, or change speed  _u8SendedSize = _u8PlayPos;
		// reuse this code for tick mode -->normal mode, there is a jummping
#if 1
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
        {
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
			LOG(5,"_swdmx_mpgHandleChange set u8PlayPos >= u8StartFileOffset (0x%x)\n",
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset);
        }
		if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)&&
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)&&
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0)&&
			(i4PlaySpeed < 0))
		{
		    UINT32 u4Speed;
			UINT64 u8Tmp;
			UINT64 u8Remainder;

		   u4Speed = (i4PlaySpeed > 0) ? i4PlaySpeed : -i4PlaySpeed;
		   u4Speed /= 1000;
		   u8Tmp = _SWDMX_Div6464(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*MPG_AUDIO_ES_SKIP_TIME), (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
		   u8Tmp *= u4Speed;

           // change for DTV00098584
           if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos > u8Tmp + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos -= u8Tmp;
           else
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
           _SWDMX_MpgModifyPlayPos(u1SrcId);
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;;
           if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos > u8Tmp + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos - u8Tmp;
           else
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;

		}
		else
		{
		   if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
           {
               _SWDMX_MpgModifyPlayPos(u1SrcId);
           }      
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
		}

#else
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos;
#endif
        //_SWDMX_MpgHandleSeekAudio(i4PlaySpeed, fgSeek);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
         prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
            // Don't need to change it when chagne speed.
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt = 0;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    }
	//DTV00081416 trick mode to normal mode. we should start trigger audio again.
    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
               (((i4PlaySpeed == 1000)&&
                 (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed != 1000)&&
                 ((UINT32)(u8SeekPos>>32) == (UINT32)0xFFFFFFFF)&&((UINT32)(u8SeekPos) == (UINT32)0xFFFFFFFF))))
    {
           // DTV00091732 , when use  DMX_MM_GetBufEmptySize() to calculate play position is not so exactly,
        // so we do a error handle, check is playPos  < u8StartFileOffset
       if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
       {
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
           LOG(5,"_swdmx_mpgHandleChange set u8PlayPos >= u8StartFileOffset (0x%x)\n",
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset);
       }

       if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed > 0)
       {
           _SWDMX_MpgModifyPlayPos(u1SrcId);
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;        
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
       }
       else
       {
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
       }
    }
	//DTV00094813, when FR to the file start, u8SendedSize will be set to fileSize, so we need to flush Afifo, and set 
	// u8SendedSize and u8ReadedPos to u8PlayPos.
	else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
               (i4PlaySpeed >= 2000)&&(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0)&&
               prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF)
    {
        LOG(5, "_SWDMX_MpgHandleChange no video change speed from %d to %d====================\n", 
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed, i4PlaySpeed);
        _SWDMX_MpgResetData(u1SrcId);
        //DTV00314077, play pos can not be smaller than u8StartFileOffset
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
        } 
        _SWDMX_MpgModifyPlayPos(u1SrcId);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed = i4PlaySpeed;
}

VOID _SWDMX_MpgHandleEOS(UINT8 u1SrcId, INT32 i4PlaySpeed)
{
    UINT32 au4BufStart[2], au4BufEnd[2];

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    #if 1//def CC_DLNA_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
        MM_SRC_TYPE_HIGH_SPEED_STORAGE)
    {
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
    }
    #endif

    // if i4PlaySpeed = 1x or 2x, fgVideoPause = TRUE
    // then got EOS and VDec in pause.
    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF)
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if((((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady))
            ||
            (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)) &&
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause)))
        {
            ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
            UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE));
            if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_AVS fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                }
            }
            else
            {
                ASSERT(0);
            }
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                if(!VDEC_Play(prSwdmxInst->u1Vdec2Id, eVdecFmt))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play 2 eVdecFmt fail\n");
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
        }
    }

    // DTV00132905, seek to end without sending any a/v data.
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
        LOG(3, "_SWDMX_MpgHandleEOS STC_SetStcValue 0x%x\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
    }

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos))
    {
        DMX_AUDIO_PES_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
        {
            rPes.u4Wp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr;
        }
        else
        {
#ifdef AUD_OLDFIFO_INTF
            VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
            VERIFY(AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
            rPes.u4Wp = (au4BufStart[0] + 4); // Jessica.
        }
        UNUSED(AUD_SendAudioPes(&rPes)); // audio driver said they need this one.
        rPes.fgEOS = TRUE;
		
		if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >0)
		{
		   rPes.fgForward = TRUE;		  //for trick mode 
		}
		else
		{
			rPes.fgForward = FALSE;
		}

        //_SWDMX_MpgAudioCallback(&rPes);
        UNUSED(AUD_SendAudioPes(&rPes));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
            #ifdef TIME_SHIFT_SUPPORT
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            #endif
            {
                UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                //AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                /*
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                    STC_StopStc(prSwdmxInst->u1StcId);
                    STC_SetStcValue(prSwdmxInst->u1StcId, (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS));
                    LOG(3, "_SWDMX_MpgHandleEOS AUD_SetStartPts 4 pts 0x%x pos %d\n",
                        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS), (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
                }
                */
                //STC_StartStc(prSwdmxInst->u1StcId);
            }

            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
        }
        LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Adec\n");
    }

    if(((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK)) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos))
    {
        DMX_PES_MSG_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        rPes.u4FrameType = PIC_TYPE_I;
        rPes.fgSeqHeader = TRUE;
        rPes.fgGop = TRUE;
        rPes.u1Channel = prSwdmxInst->u1Vdec2Id;
        rPes.pvInstanceTag=(void*)prSwdmxInst;                 //surpport mult instance
        if(!_SWDMX_MpgVideoCallback(&rPes))
        {
            LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgVideoCallback fail\n");
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE;
        LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Vdec2\n");
    }
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos))
    {
        DMX_PES_MSG_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        rPes.u4FrameType = PIC_TYPE_I;
        rPes.fgSeqHeader = TRUE;
        rPes.fgGop = TRUE;
        rPes.pvInstanceTag=(void*)prSwdmxInst;                 //surpport mult instance
        if(!_SWDMX_MpgVideoCallback(&rPes))
        {
            LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgVideoCallback fail\n");
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
        // i4PlaySpeed = 4x or up.
        // if now in VDEC_PlayOneFrm, VDec might in pause status.
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((i4PlaySpeed < 1)
                || (i4PlaySpeed >= 4000)
                || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause))
            {
                ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
                UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE));
                if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_AVS fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                    }
                }
                else
                {
                    ASSERT(0);
                }
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    if(!VDEC_Play(prSwdmxInst->u1Vdec2Id, eVdecFmt))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play 2 eVdecFmt fail\n");
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
            }
        }
        LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Vdec\n");
    }
    if(prSwdmxInst->rCurRangeReq.prDmxRangeInfo &&
         IS_NETWORK_SOURCE(prSwdmxInst->rCurRangeReq.prDmxRangeInfo->e_mm_src_type))
    {
       prSwdmxInst->fgRcvEOS = TRUE;
    }
}

/*  for DTV00378494: It will not play until 3 seconds later
    for SWDMX_FMT_RAW_AUDIO_ES, each file has a birate, it is different from echo other,
    from small dirate, we must give it a small playback threshold
    (small birate will call AUdioCallback more times to reach the threshold)
*/
static inline UINT32 _SWDMX_MpgGetRawAudioEsStartThreshold(UINT8 u1SrcId)
{
    UINT64 u8Remainder = 0;
    UINT32 u4Rate = 0;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (SWDMX_FMT_RAW_AUDIO_ES == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt)
    {
        if (90000 <= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime )
        {
            u4Rate = _SWDMX_Div6464(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize, 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime/90000, &u8Remainder);
            u4Rate *= 15;
        }
        else
        {
            u4Rate = 100000;	
        }
        
        // Notes: the size must larger than FIFO_EMPTY_THRESHOLD  (UINT32)(3300)  or underflow may be occur 
        if (20480 > u4Rate)
        {
            u4Rate = 20480;
        }
        else if (100000 < u4Rate)
        {
            u4Rate = 100000;
        }
    }
    else
    {
        u4Rate = 0;
    }
    
    return u4Rate;
}

INT32 _SWDMX_MpgHandleFifoFull(UINT8 u1SrcId, INT32 i4PlaySpeed, BOOL fgLast)
{
    BOOL fgRet;
    BOOL fgFull;
    BOOL fgAudioSmaller;
    DMX_DDI_T rDDI;
    UINT32 u4DmxAvailSize;
    UINT32 u4DmxAvailSizeAudio;
    UINT16 u2QueueSize, u2MaxQueueSize;
    UINT16 u2DmxPictureQSize,u2DmxMaxQSize;
    ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    fgFull = FALSE;
    fgAudioSmaller = FALSE;

    x_memset(&rDDI, 0, sizeof(rDDI));
    fgRet = DMX_DDI_Get(DMX_DDI_FLAG_DATA_SIZE, &rDDI);
    rDDI.u4DataSize += (10*MPG_ALIGN_SIZE_TS_192); // max is 192
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgHandleFifoFull DMX_DDI_Get fail.\n");
        return MPG_SEND_FAIL;
    }

    u4DmxAvailSize = (1024*1024); // ToDo: 3M?
    u4DmxAvailSizeAudio = (1024*1024);
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
    {
        // playbacking, check AV size
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
            // DTV00139611, [MM]Only flash progress bar after select current video file to play
            // notify not support when this clip is ts file with scramble data
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) &&
               ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)))
            {
                if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                    {
                    }
                    else
                    {
                        VDEC_Notify(prSwdmxInst->u1VdecId,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
                    }
                    // VDEC_DEC_DECODE_NO_DATA
                    LOG(3,"video scramble\n");
                }
            }

            #ifdef SWDMX_MPG_DMX_20
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                DMX_PID_TYPE_ES_VIDEO,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
            #else
#ifdef CC_DMX_PURE_AUDIO
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
            {
                u4DmxAvailSize = DMX_MM_GetBufEmptySize(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
           	}
            else
#endif
            {
                u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
            }
            #endif

            if(u4DmxAvailSize <=
                (1024*1024))
            {
                u2QueueSize = 0;
                u2MaxQueueSize = 0;
                VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2QueueSize, &u2MaxQueueSize);
                if(u2QueueSize < 2)
                {
                    // we have two choices.
                    // 1. notfiy video not exist.
                    // 2. just flush fifo data
                    #if 0
                    // 1.
                    VDEC_Notify(prSwdmxInst->u1VdecId,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
                    // We don't know where to jump to.
                    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
                    //{
                    //    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = TRUE;
                    //}
                    LOG(3,"there is no video frame in queue, disable it\n");
                    #else
                    // 2.
                    _SWDMX_MpgFlushVFifo(u1SrcId);
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
                    {
                        UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo));
                    }
                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                        (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        _SWDMX_MpgFlushVFifo2(u1SrcId);
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 != 0xFF)
                        {
                            UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2));
                        }
                    }
                    LOG(3,"there is no video frame in queue, flush it\n");
                    #endif
                    return TRUE;
                }
            }
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
        {
            // DTV00139611, [MM]Only flash progress bar after select current video file to play
            // notify not support when this clip is ts file with scramble data
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio) &&
               ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)))
            {
                if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                    {
                    }
                    else
                    {
#ifdef SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
                        AUD_MM_Set_Dec_Fmt_Scramble(prSwdmxInst->u1AdecId, TRUE);
#endif
                    }
                    LOG(3,"audio scramble\n");
                }
            }


            #ifdef SWDMX_MPG_DMX_20
            u4DmxAvailSizeAudio = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                DMX_PID_TYPE_ES_AUDIO,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
            #else
#ifdef CC_DMX_PURE_AUDIO
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        	{
        	    u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
        	        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        	}
            else
#endif
        	{
                // FixMe
        	    u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
        	        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        	    //u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
        	}
            #endif
            if(u4DmxAvailSize > u4DmxAvailSizeAudio)
            {
                u4DmxAvailSize = u4DmxAvailSizeAudio;
                fgAudioSmaller = TRUE;
            }
        }
        /* need to complete audio2
        // DTV00139611, [MM]Only flash progress bar after select current video file to play
        // notify not support when this clip is ts file with scramble data
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio2)
        {
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
            {
                if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
#ifdef SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
                    AUD_MM_Set_Dec_Fmt_Scramble(prSwdmxInst->u1AdecId, TRUE);
#endif
                    LOG(3,"video scramble\n");
                }
            }

            u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
            if(u4DmxAvailSize > u4DmxAvailSizeAudio)
            {
                u4DmxAvailSize = u4DmxAvailSizeAudio;
                fgAudioSmaller = TRUE;
            }
        }
        */
        if (rDDI.u4DataSize >= u4DmxAvailSize)
        {
            u4DmxAvailSize = 0;
        }
        else
        {
            u4DmxAvailSize -= rDDI.u4DataSize;
        }
        if (MPG_DMX_RESERVED_SIZE >= u4DmxAvailSize)
        {
            u4DmxAvailSize = 0;
        }
        else
        {
            u4DmxAvailSize -= MPG_DMX_RESERVED_SIZE;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize)
    {
        static UINT32 u4BufferUnderRunCnt = 0;
        // parsing, check psi buffer size
        #ifdef SWDMX_MPG_DMX_20
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
            DMX_PID_TYPE_PSI, 0xFF);
        #else
        u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_PSI);
        #endif
        if (rDDI.u4DataSize >= u4DmxAvailSize)
        {
            u4DmxAvailSize = 0;
            u4BufferUnderRunCnt++;
            if(u4BufferUnderRunCnt > 90)
            {
                DMX_PID_T rPidStruct;
                UINT32 u4Idx;

                for(u4Idx = 0;u4Idx < DMX_NUM_PID_INDEX;u4Idx++)
                {
                    x_memset(&rPidStruct, 0, sizeof(rPidStruct));
                    DMX_GetPid(u4Idx, DMX_PID_FLAG_BUFFER, &rPidStruct);
                    if(rPidStruct.ePidType == DMX_PID_TYPE_PSI)
                    {
                        if(rPidStruct.u4BufSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize)
                        {
            LOG(0, "Please enlarge Pidx Buffer to 40K in sys_start.h\n");
            LOG(0, "#define PSI_ENG_PAT_BUFF_SIZE    ((SIZE_T) 40960)\n");
                            break;
                        }
                    }
                }

                u4BufferUnderRunCnt = 0;
            }
        }
        else
        {
            u4BufferUnderRunCnt = 0;
            u4DmxAvailSize -= rDDI.u4DataSize;
        }
    }

    if(u4DmxAvailSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize)
    {
        LOG(9, "_SWDMX_MpgHandleFifoFull SWDMX Demuxer Buffer is not enough.\n");
        fgFull = TRUE;
    }    
    
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo) && (!fgFull))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
            u2QueueSize = 0;
            u2MaxQueueSize = 0;
            u2DmxPictureQSize = 0;
            u2DmxMaxQSize = 0;
            
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2QueueSize, &u2MaxQueueSize);
            DMX_GetPicQSize(&u2DmxPictureQSize, &u2DmxMaxQSize);
            
            if((u2QueueSize > 0) && (u2MaxQueueSize > 150) &&
               (u2QueueSize > (u2MaxQueueSize - 150)))
            {
                fgFull = TRUE;
            }
            else if(((u2DmxPictureQSize > 0) && (u2DmxMaxQSize > 384)) &&
                (u2DmxPictureQSize > (u2DmxMaxQSize - 384)))
            {
                fgFull = TRUE;
            }
            else if((u2DmxPictureQSize + u2QueueSize) >= u2MaxQueueSize) 
            {
                 /*
                   protect ESMQ overflow by the interrupt from dmx picture Q
                   when os scheduled to dmx, it will dump all in hands picture Q to Vdec ESMQ
                 */
                fgFull = TRUE;
            }
            else if(
                    /*((u2QueueSize > 30) &&
                     (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_VUDU)) ||*/
                    // before output, check start condition
                    ((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo) &&
                     ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_VUDU) ||
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_PUSH) ||
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_CMPB)) &&
                     (((prSwdmxInst->ePlaybackBufType == eSWDMX_BUF_MODE_TIME) &&
                       (u2QueueSize >= (prSwdmxInst->u4PlaybackBufThreshold / 33))) // ms -> frame (fps = 30)
                      ||
                      ((prSwdmxInst->ePlaybackBufType == eSWDMX_BUF_MODE_BYTE) &&
                       ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize -
                         prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset) >=
                          prSwdmxInst->u4PlaybackBufThreshold)))
                    )
                    ||
                    ((u2QueueSize > 60) &&
                     ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_VUDU) ||
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_PUSH) ||
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_CMPB)))
                     ||                                 //DTV00093647 for dlna, the underflow threshold value is 32, so we set the full value is 60 
                     ((u2QueueSize > 60) &&
                     ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_DLNA)))
                     ||
                    ((u2QueueSize > 30) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_NETWORK_VUDU) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_PUSH) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_NETWORK_CMPB) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)&&
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_NETWORK_DLNA))
                   )
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                    (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo))
                {
                    // There are many frames we send to vdec,
                    // but there is still have no sync point.
                    // continue send to vdec.
                    fgFull = FALSE;
                }
                else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio &&
                    (!((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                    (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))))
                {
                    UINT32 u4Addr, u4Size;
                    if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
                    {
                        LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                        return FALSE;
                    }
                    // from aud_drvif.c
                    //#define FIFO_EMPTY_THRESHOLD    (UINT32)(3300)
                    // avoid audio underflow. CR-DTV00130988
                    //if(!AUD_IsFifoEmpty(prSwdmxInst->u1AdecId))
                    //[DTV00055801] [AMMP153] MM audio output not smoothly
                    // change from 10000 to 15000
                    if(((u4Size - u4DmxAvailSizeAudio) > (0x3C00))
                       &&
                       // in time-shfit, we might in pcr pause stage in begin play
                       // we have to send more data to let STC(pcr) increase.
                       // then video PTS will meat STC and start to play.
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                    {
                        // ToDo: check second audio driver buffer.
                        // maybe don't need this.
                        /*
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio2)
                        {
                            u4BufSize          = au4BufEnd[1] - au4BufStart[1];
                            if((u4BufSize - u4DmxAvailSizeAudio) > (5000))
                            {
                                fgFull = TRUE;
                            }
                        }
                        else
                        */
                        {
                            fgFull = TRUE;
                        }
                    }                    
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo)
                    {
                        if(IS_NETWORK_SOURCE(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type))
                        {
                            if(u2QueueSize > 150)
                            {
                                fgFull = TRUE;
                            }
                        }
                        else
                        {
                            if(u2QueueSize > 60)
                            {
                                fgFull = TRUE;
                            }
                        }                        
                    }
                    if(!(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio &&
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady))
                    {
                        if(fgFull) // not get audio yet
                        {
                            if(u4DmxAvailSize > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize)
                            {
                                fgFull = FALSE; // send more
                            }
                        }                        
                    }
                }                
                else if(!(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
                {
                    fgFull = TRUE;
                }
                LOG(9, "_SWDMX_MpgHandleFifoFull Vdec esm queue is not enough.\n");
            }
#ifdef TIME_SHIFT_SUPPORT
            // For Vestel Timeshift case, if vdec notdisplay, means load first picture. change data req follow
            else if ( (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                      (!MPV_IsDisplay(prSwdmxInst->u1VdecId)) ) 
            {
                if ( (prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264) && (u2QueueSize > 60) )
                {
                    fgFull = TRUE;
                }
                else if ( (prSwdmxInst->eVideoType != ENUM_SWDMX_VDEC_H264) && (u2QueueSize > 30) )
                {
                    fgFull = TRUE;
                }
                else
                {
                    // NULL
                }
            }
#endif
            else
            {
                // NULL
            }
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio // audio only
#ifdef TIME_SHIFT_SUPPORT
                //&&
                //(i4PlaySpeed == 1000)
                &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
               )
        {
            // start to play
            UINT32 u4Addr, u4Size, u4FifoFullSize;
            if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                return FALSE;
            }

            // for DTV00378494: It will not play until 3 seconds later, re_calc the start threshold
            if (SWDMX_FMT_RAW_AUDIO_ES == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt)
            {
                u4FifoFullSize = _SWDMX_MpgGetRawAudioEsStartThreshold(u1SrcId);
            }
            else
            {
                u4FifoFullSize = 100000;
            }
            u4FifoFullSize = (0==u4FifoFullSize) ? 100000 : u4FifoFullSize;
            
            // from aud_drvif.c
            //#define FIFO_EMPTY_THRESHOLD    (UINT32)(3300)
            // avoid audio underflow. CR-DTV00130988
            if((u4Size - u4DmxAvailSizeAudio) > u4FifoFullSize)
            {
                // ToDo: check second audio driver buffer.
                // maybe don't need this.
                /*
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio2)
                {
                    u4BufSize          = au4BufEnd[1] - au4BufStart[1];
                    if((u4BufSize - u4DmxAvailSizeAudio) > (100000))
                    {
                        fgFull = TRUE;
                    }
                }
                else
                */
                {
                    fgFull = TRUE;
                }
            }
            
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
            {
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio)
                {
                    UINT32 u4Diff = 0;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS >
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS)
                    {
                        u4Diff = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS
                            - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
                    }
                    else
                    {
                        u4Diff = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS -
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS;
                    }
                    if(u4Diff > SWDMX_AUDIO_FULL_PTS_INTERVAL)
                    {
                        LOG(5, "Audio only, start play with pts interval > (%d)\n",
                            SWDMX_AUDIO_FULL_PTS_INTERVAL);
                        fgFull = TRUE;
                    }
                }
            }
        }
        else
        {
            // TS MW parsing case, no A/V
        }
    }

    // Handle dmx might use old read/write point when we update this on
    // _SWDMX_MpgAudioCallback. So do it again.
    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio) ||
    // time-shift will set fgGotAudio but fgCleanAudio is on.
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio))
    {
        _SWDMX_MpgFlushAFifo(u1SrcId);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio));
        }
    }
    // ~handle dmx

#ifdef TIME_SHIFT_SUPPORT
    // To handle:
    // 1. pause vdp
    // 2. move data with block mode
    // 3. check display queue have video frame. If no, go to 2.
    // 4. keep move data with block mode
    // 5. if STC equals to first vdp's PTS,
    //    change move data method to tick mode
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause) &&
           (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo) &&
           (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
           (i4PlaySpeed >= 0) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
           (i4PlaySpeed <= 1500)
#else
           (i4PlaySpeed < 4000)
#endif
          )
        {
            // pause video
            //UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE));
            //STC_StopStc(prSwdmxInst->u1StcId);           //DTV00144863 When time shift for audio only file ,can't stop STC

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
            //LOG(3, "video pause  ==============\n");

            // 4x -> 1x, vdec might in pause status. play it.
            if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                eVdecFmt = VDEC_FMT_MPV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_MPV fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                eVdecFmt = VDEC_FMT_H264;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_H264 fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                eVdecFmt = VDEC_FMT_WMV;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_WMV fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
            {
                eVdecFmt = VDEC_FMT_AVS;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_AVS fail\n");
                }
            }
            else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
            {
                eVdecFmt = VDEC_FMT_MP4;
                if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                }
            }
            else
            {
                ASSERT(0);
            }
            if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
               (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
            {
                if(!VDEC_Play(prSwdmxInst->u1Vdec2Id, eVdecFmt))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play 2 eVdecFmt fail\n");
                }
            }

        }

        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo))
        {
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
            }
        }
    }
    // ~To handle
#endif

    // [DTV00132776] The video stopped and the position-duration indicator in the status bar is null after input the current playing time.
    // When seek to near file end, we might send to vdec then vdec_pause,
    // nobody will call vdec_play.
    // Check this status here.
    if((fgFull) ||
        //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize == (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize)))
        // DTV00211080 [MM Compatibility]Progress bar doesn't start from the beginning when play special file
        // we should check start to play after we have send almost all data,
        // but not read all data.
       (fgLast) ||
       // when rendering thumbnail, not wait fifo full to start playback.
       ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_vid_render_counts > 0)
       )
      )
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                /*if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause &&
                    (((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                      (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio &&
                       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady)) &&
                        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)))
                        */
                BOOL fgAudioReady;
                BOOL fgFeederReady;
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
                {
                    fgAudioReady = TRUE;
                }
                else
                {
                    fgAudioReady =
                          ((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio &&
                             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady));
                }

                fgFeederReady = FALSE;
                #ifdef CC_DLNA_SUPPORT
                if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_DLNA)
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr)
                    {
                        UINT32 u4FeederReadySize = (MPG_DLNA_FEEDER_READY_SIZE);
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE))
                            > (u4FeederReadySize))
                        {
                            FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize);
                        }
                        else
                        {
                            u4FeederReadySize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE));
                            FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize);
                        }
                        //FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNDERFLOW_SIZE);
                        FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize/8);
                        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, TRUE);
                    }
                    else
                    {
                        FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
                        FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
                        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, FALSE);
                    }
                    if(FeederIsReady(prSwdmxInst->eFeederSrc))
                    {
                        fgFeederReady = TRUE;
                    }
                }
                else
                #endif
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                {
                    fgFeederReady = TRUE;
                }
                else
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_NETWORK_VUDU)
                {
                    fgFeederReady = TRUE;
                }
                else
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_NETWORK_RHAPSODY)
                {
                    fgFeederReady = TRUE;
                }
                else
                //if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                {
                    // when audio buffer is 512K
                    // only could only have 1 buffer data (one is u4UsingBufSize)
                    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt >= 2)
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt >= 1)
                    {
                        fgFeederReady = TRUE;
                    }
                }

                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize ==
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos +
                     prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize +
                     prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize)) ||
                   // while vdec is pause then we got eof with vfifo full
                   // we will block here. Add this condition.
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof))
                {
                    // File end
                    fgFeederReady = TRUE;
                }

                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause) && (fgAudioReady)
                    && (fgFeederReady))
                {
                    // DTV00136461 [MM]Can't full display the special ts file.
                    // if there is no video PTS
                    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) &&
                       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
                        }
                        else
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = MPG_STC_BASE;
                        }
                        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc) &&
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                            STC_StopStc(prSwdmxInst->u1StcId);
                            // When there is video stream, stc value will set at vdec_flw.c
                        }
                    }


                    // if there is no video PTS
                    /*
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)
                    {
                        // DTV00135223
                        // we never found a video PTS
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS;
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
                        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                            STC_StopStc(prSwdmxInst->u1StcId);
                            // When there is video stream, stc value will set at vdec_flw.c
                        }
                    }
                    */

                    /*if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo)
                    {
                        // In this case, vdec ESQ is almost full.
                        // Although there is no sync point,
                        // we must start to play.
                        LOG(1, "_SWDMX_MpgHandleFifoFull NG fgGotVideo -> VDEC_Play\n");
                    }
                    else*/
                    {
                        LOG(5, "_SWDMX_MpgHandleFifoFull full -> VDEC_Play\n");
                    }
                    UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE));
                    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                    {
                        eVdecFmt = VDEC_FMT_MPV;
                        if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_MPV fail\n");
                        }
                    }
                    else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
                    {
                        eVdecFmt = VDEC_FMT_H264;
                        if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_H264 fail\n");
                        }
                    }
                    else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
                    {
                        eVdecFmt = VDEC_FMT_WMV;
                        if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_WMV fail\n");
                        }
                    }
                    else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
                    {
                        eVdecFmt = VDEC_FMT_AVS;
                        if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_AVS fail\n");
                        }
                    }
                    else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                    {
                        eVdecFmt = VDEC_FMT_MP4;
                        if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                        }
                    }
                    else
                    {
                        ASSERT(0);
                    }

                    if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                       (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                    {
                        if(!VDEC_Play(prSwdmxInst->u1Vdec2Id, eVdecFmt))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play 2 eVdecFmt fail\n");
                        }
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                    /* This part will be done when vdp display first video frame.
                    AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                    */
                }

                // DTV00136461 [MM]Can't full display the special ts file.
                // audio full without triger again
                if((fgFull) && (fgAudioSmaller))
                {
                    VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2QueueSize, &u2MaxQueueSize);
                    if(u2QueueSize == 0)
                    {
                        AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                        LOG(3, "_SWDMX_MpgHandleFifoFull AUD_MMAoutEnable video empty\n");
                    }

                }
            }
            else if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                    (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady))// audio only
            {
                #ifdef CC_DLNA_SUPPORT
                // DTV00212740, audio notify no data
                // audio should be pause when data underflow
                if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA)
                {

                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry))
                    {
                        // we can't generate audio index size, we will block in
                        // buf count is full but data is still not full enough
                        FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_INDEX_FEEDER_READY_SIZE);
                        FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_INDEX_FEEDER_UNDERFLOW_SIZE);
                    }
                    else
                    {
                        UINT32 u4FeederReadySize = (MPG_DLNA_FEEDER_READY_SIZE);
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE))
                            > (u4FeederReadySize))
                        {
                            FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize);
                        }
                        else
                        {
                            u4FeederReadySize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE));
                            FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize);
                        }
                        //FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNDERFLOW_SIZE);
                        FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, u4FeederReadySize/8);
                    }
                    FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, TRUE);
                }
                #endif

                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
            }
        }
        if(fgFull)
        {
            return MPG_SEND_FULL;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo &&
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio &&
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio &&
        (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS != 0xFFFFFFFF)
#ifdef TIME_SHIFT_SUPPORT
         && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
        )
    {
        VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
    }
    
    return MPG_SEND_OK;
}


BOOL _SWDMX_MpgHandleMoveDataSysB(UINT8 u1SrcId, DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed)
{
    BOOL fgRet = TRUE;
    UINT64 u8SkipStartpos;
    UINT64 u8SkipEndpos;
    UINT64 u8Remainder;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prDmxMMData->u4FrameSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize)
    {
        return _SWDMX_MoveData(u1SrcId, prDmxMMData);         
    }

    // confirm systemBPacketSize
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize == 0)
    {
        UINT64 u8Size = 0;
        UINT32 i = 0;
        UINT32 u4Addr = prDmxMMData->u4StartAddr;
        for(;i<14;i++)
        {
            u8Size <<= 8;
            u8Size +=  (UINT64)(*(CHAR*)u4Addr);
            u4Addr ++;
            if(u4Addr > prDmxMMData->u4BufEnd)
            {
                u4Addr -=
                    (prDmxMMData->u4BufEnd - prDmxMMData->u4BufEnd);
            }
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize != 
            u8Size)
        {
            if(u8Size < 0xFFFFFFFF)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize = 
                    u8Size;
            }
            LOG(3, "u4SystemBPacketSize(%d)\n", (UINT32)u8Size);
        }
    }

    u8SkipStartpos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize +
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize + 14) - 1;
    u8SkipStartpos = _SWDMX_Div6464(u8SkipStartpos,
        (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize + 14),
        &u8Remainder);
    u8SkipStartpos *= (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SystemBPacketSize + 14);

    u8SkipEndpos = u8SkipStartpos + 14;
    
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize +
        prDmxMMData->u4FrameSize) > u8SkipStartpos)
    {
        DMX_MM_DATA_T rDmxMMDataTmp;
        
        rDmxMMDataTmp = *prDmxMMData;
        rDmxMMDataTmp.u4FrameSize = 
            u8SkipStartpos - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize + prDmxMMData->u4FrameSize >
            u8SkipEndpos)
        {
            // will have second move
            rDmxMMDataTmp.fgEOS = FALSE;
        }
        if(rDmxMMDataTmp.u4FrameSize > 0)
        {
            fgRet = _SWDMX_MoveData(u1SrcId, &rDmxMMDataTmp);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgHandleMoveDataSysB 1 MoveData Fail.\n");
            }
            rDmxMMDataTmp.fgFrameHead = 0;
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize + prDmxMMData->u4FrameSize >
            u8SkipEndpos)
        {
            rDmxMMDataTmp.u4StartAddr +=
                (u8SkipEndpos - 
                 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            if(rDmxMMDataTmp.u4StartAddr > rDmxMMDataTmp.u4BufEnd)
            {
                rDmxMMDataTmp.u4StartAddr -=
                    (rDmxMMDataTmp.u4BufEnd - rDmxMMDataTmp.u4BufStart);
            }
            ASSERT(u8SkipEndpos > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            rDmxMMDataTmp.u4FrameSize = 
                prDmxMMData->u4FrameSize - 
                (u8SkipEndpos - 
                 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);                    
            rDmxMMDataTmp.fgEOS = prDmxMMData->fgEOS;
            fgRet = _SWDMX_MoveData(u1SrcId, &rDmxMMDataTmp);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgHandleMoveDataSysB 2 MoveData Fail.\n");
            }
        }
    }
    else
    {
        fgRet = _SWDMX_MoveData(u1SrcId, prDmxMMData);
    }
    return fgRet;
}
INT32 _SWDMX_MpgHandleMoveData(UINT8 u1SrcId, DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed)
{
    BOOL fgRet;
    INT32 i4Ret;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
#ifdef TIME_SHIFT_SUPPORT
    UINT32 i;
    SWDMX_PVR_SENDDATA_RET_T ePVRRet;
    UINT32 u4MinSize = 0;
    UINT32 u4Temp = 0;
#endif
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    if(!prDmxMMData)
    {
        LOG(1, "prDmxMMData null\n");
        return MPG_SEND_FAIL;
    }
#ifdef SWDMX_MPG_DEBUG_FIFO
#ifdef SWDMX_MPG_DMXFIFO_SIZE
    if((_fgDmxFifo) && (_fgDmxManual))
    {
        UINT32 u4Tmp;

        _SWDMX_COPY_RING_BUF(
            _u4DmxFifoStart,
            _u4DmxFifoEnd,
            &_u4DmxFifoCur,
            prDmxMMData->u4BufStart,
            prDmxMMData->u4BufEnd,
            prDmxMMData->u4StartAddr,
            prDmxMMData->u4FrameSize);
    }
#endif
#endif


    #if defined(CC_MT5365) || defined(CC_MT5395) || defined(VDEC_IS_POST_MT5368)
    if((prDmxMMData->u4StartAddr < prDmxMMData->u4BufEnd) &&
       ((prDmxMMData->u4StartAddr + prDmxMMData->u4FrameSize) >= prDmxMMData->u4BufEnd))
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prDmxMMData->u4StartAddr), (prDmxMMData->u4BufEnd - prDmxMMData->u4StartAddr));
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prDmxMMData->u4BufStart), (prDmxMMData->u4FrameSize - (prDmxMMData->u4BufEnd - prDmxMMData->u4StartAddr)));
    }
    else
    {
        HalFlushInvalidateDCacheMultipleLine(VIRTUAL(prDmxMMData->u4StartAddr), prDmxMMData->u4FrameSize);
    }
    #else
    HalFlushInvalidateDCache();
    #endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = FALSE;
    i4Ret = MPG_SEND_OK;
    UNUSED(fgRet);

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // ToDo: Handle 0x47 sync byte
        #if 1
        if((*(UCHAR*)VIRTUAL(prDmxMMData->u4StartAddr+4)) != 0x47)
        {
            LOG(3, "Move !0x47\n");
            ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize);
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize)
            {
                UINT64 u8Tmp;
                UINT64 u8Remainder;
                u8Tmp = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)192, &u8Remainder);
                UNUSED(_SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));

                LOG(3, "!0x47, FileOffset 0x%llx, LBA 0x%llx, FilePos 0x%llx\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset,
                    (UINT64)u8Tmp,
                    (UINT64)(u8Remainder + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)
                    );
                UNUSED(u8Tmp);
                UNUSED(u8Remainder);
            }
            //ASSERT(0);
        }
        #endif

        // [DTV00141987] After fast rewind,the fast forward to right boundary ,audio isn't smooth
        // We should not use fast mode when out of safe range
        // mw safe range is small, we might switch fast/normal during safe-valid range
        // enlarge safe range in driver to prevent this switch.
        /*
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum) &&
           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum) <=
             MPG_TIME_SHIFT_SAFE_RANGE_END_THRESHOLD))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange = TRUE;
        }
        */

        for(i=0;i<(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1);i++)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] == 0) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1] == 0))
            {
                break;
            }
            else
            {
                u4Temp = (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1] -
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i]);
                if(u4Temp <
                   (prDmxMMData->u4BufEnd - prDmxMMData->u4BufStart))
                {
                    u4MinSize += u4Temp;
                }
                else
                {
                    u4MinSize += (120*1024);
                }
            }
        }
        if(i < (MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1))
        {
            u4MinSize = (1024*1024);
        }

        // 1x senddata
        if((i4PlaySpeed == 1000)
           && // have at least one video or one audio
           // change to audio only not use fast mode
           // or swdmx speed is too fast
           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
            #if 0
           &&
            // can't use fast move when audio only, swdmx speed is too fast
           (
            // video with audio also have this problem
            //(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            //||
            // can't use fast move when out of safe range
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange)
             #if 1 // enable this to speed up Nx -> 1x speed
           )
             #endif
             #endif
          )
        {
            //LOG(3, "_SWDMX_PVR_SendData normal u8SendedSize(0x%llx)\n",
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            /*
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange = FALSE;
            }*/
            _SWDMX_LockMoveMutex(u1SrcId);
            ePVRRet = _SWDMX_PVR_SendData(u1SrcId, prDmxMMData, u4MinSize, FALSE);
            _SWDMX_UnlockMoveMutex(u1SrcId);
            if(ePVRRet == SWDMX_PVR_SENDDATA_FULL)
            {
                return MPG_SEND_FULL;
            }
            if(ePVRRet != SWDMX_PVR_SENDDATA_OK)
            {
                return MPG_SEND_FAIL;
            }
        }
        else
        {
            //LOG(3, "_SWDMX_PVR_SendData fast u8SendedSize(0x%llx)\n",
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            _SWDMX_LockMoveMutex(u1SrcId);
            ePVRRet = _SWDMX_PVR_SendData(u1SrcId, prDmxMMData, u4MinSize, TRUE);
            _SWDMX_UnlockMoveMutex(u1SrcId);
            if(ePVRRet != SWDMX_PVR_SENDDATA_OK)
            {
                return MPG_SEND_FAIL;
            }

        }
        // this is tick size < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4QueueTickNum !=
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4QueueTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
            for(i=0;i<(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1);i++)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1];
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8QueueStartAddr[(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1)] =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        }
    }
    else
#endif
    if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
        {
            #ifdef SWDMX_MPG_DMX_20
            prDmxMMData->u1Idx =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio;
            fgRet = _SWDMX_MoveData(u1SrcId, prDmxMMData);
            #else
            fgRet = DMX_MM_MoveData(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, prDmxMMData, 1000);
            #endif
            LOG(7, "_SWDMX_MpgHandleMoveData(%d) MoveData 0x%x, S=%d.\n",
                u1SrcId,
                prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgHandleMoveData DMX_MM_MoveData Fail.\n");
                i4Ret = MPG_SEND_FAIL;
            }
        }
        else//DTV00214170,  For ES audio only file swdmx can't send data before audio stream enable
        {
        	LOG(5, "_SWDMX_MpgHandleMoveData(%d) DMX_MM_MoveData Fail audio didn't enable.\n",
        	    u1SrcId);
        	i4Ret = MPG_SEND_FAIL;
        }
    }
    else
    {
        #ifdef SWDMX_MPG_DMX_20
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)))
        {
            prDmxMMData->u1Idx =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo;
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSystemB)
        {
            fgRet = _SWDMX_MpgHandleMoveDataSysB(u1SrcId, prDmxMMData, i4PlaySpeed);
        }
        else
        {
            fgRet = _SWDMX_MoveData(u1SrcId, prDmxMMData);
        }
        #else
    	fgRet = DMX_DDI_MoveData(prDmxMMData);
        #endif
        LOG(9, "_SWDMX_MpgHandleMoveData MoveData 0x%x, S=%d.\n",
            prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgHandleMoveData MoveData Fail.\n");
            i4Ret = MPG_SEND_FAIL;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio)
    {
        _SWDMX_MpgFlushAFifo(u1SrcId);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio));
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCleanAudio = FALSE;
    }

    UNUSED(i4PlaySpeed);
    return i4Ret;
}


//#define TickMethod
UINT32 _SWDMX_MpgHandleAudioIndex(UINT8 u1SrcId, UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, UINT32 *pu4OutReadPTS, INT32 i4PlaySpeed)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    *pu8OutStartAddr = u8StartAddr;
    *pu4OutReadSize = u4ReadSize;
    *pu4OutReadPTS = 0;

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        return MPG_AUDIO_INDEX_OK;
    }
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
      )
    {
        MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
        {
            UINT64 u8IdxBaseAddr;
            UINT64 u8IdxStartAddr;
            UINT64 u8IdxEndAddr;
            u8IdxBaseAddr =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
            prTblEntry = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
            if((prTblEntry) && (prIdxTbl))
            {
                UINT32 u4Cnt = 0;
                if(prIdxTbl->ui4_number_of_entry == 0)
                {
                    LOG(3, "_SWDMX_MpgHandleAudioIndex ui4_number_of_entry 0\n");
                    return MPG_AUDIO_INDEX_OK;
                }
                while(u4Cnt < prIdxTbl->ui4_number_of_entry)
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum >= prIdxTbl->ui4_number_of_entry)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum = 0;
                    }

                    u8IdxEndAddr = u8IdxBaseAddr +
                        (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                        (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                    if(((u8StartAddr) >=
                        (u8IdxBaseAddr +
                         prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset
                        )
                       )
                       &&
                       ((u8StartAddr) <
                        (u8IdxEndAddr)
                       )
                      )
                    {
                        break;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum++;
                    u4Cnt++;
                }
                if(u4Cnt >= prIdxTbl->ui4_number_of_entry)
                {
                    *pu4OutReadSize = 0;
                    LOG(3, "_SWDMX_MpgHandleAudioIndex Read Address(0x%x)out of index num (from %d)\n",
						u8StartAddr,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum);
                    return MPG_AUDIO_INDEX_OK;
                }
                u8IdxStartAddr = u8IdxBaseAddr +
                    (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset;

                u8IdxEndAddr = u8IdxStartAddr + (UINT64)prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                //if(u8IdxStartAddr == u8StartAddr)
                if(u8IdxStartAddr <= u8StartAddr)  //for handle jumpping to normal get  pts =0
                {
                    *pu4OutReadPTS = prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_pts;
                }
                if((u8StartAddr + (UINT64)u4ReadSize) > u8IdxEndAddr)
                {
                    *pu4OutReadSize = (UINT32)(u8IdxEndAddr - u8StartAddr);
                    if((u8IdxEndAddr - u8StartAddr) > 0xFFFFFFF)
                    {
                        LOG(3, "Too big, u8StartAddr %lld -- u8IdxEndAddr %lld\n",
                            u8StartAddr, u8IdxEndAddr);
                        *pu4OutReadSize = 0;
                    }
                    if(u8StartAddr > u8IdxEndAddr)
                    {
                        LOG(3, "u8StartAddr %lld > u8IdxEndAddr %lld\n",
                            u8StartAddr, u8IdxEndAddr);
                        *pu4OutReadSize = 0;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum++;
                }
            }
            else
            {
                //LOG(3, "_SWDMX_MpgHandleAudioIndex prTblEntry NULL\n");
            }
        }

    }
    else
    {
        UINT32 u4Time = 0;
        if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > 0))
        {
            UINT64 u8Tmp;
            UINT64 u8SendPos;
            UINT64 u8Remainder;
            // 90000 -> 90000
            u8Tmp = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset) * (UINT64)90000,
                              prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime,
                              &u8Remainder);
            if(u8Tmp > 0)
            {
                //DTV00141518, [MM]Can not display current time when play special .mp3 file.
                // This is call from handlefeeder, we should consider buffer status
                // Or the position is updated late.
                u8SendPos = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize + (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize
                    + (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize + (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize);
                if(u8SendPos >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
                {
                    u4Time = (UINT32)_SWDMX_Div6464((UINT64)(u8SendPos - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset) * (UINT64)90000,
                                               (UINT32)u8Tmp, &u8Remainder);
                }
            }
        }

        *pu8OutStartAddr = u8StartAddr;
        *pu4OutReadSize = u4ReadSize;
        *pu4OutReadPTS = u4Time;
    }
    return MPG_AUDIO_INDEX_OK;
}

//#define TickMethod
UINT32 _SWDMX_MpgHandleRing(UINT8 u1SrcId, UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, INT32 i4PlaySpeed)
{
    UINT32 u4Ret = MPG_RING_OK;
#ifdef TIME_SHIFT_SUPPORT
    BOOL fgRet;
    UINT64 u8Remainder;
#ifdef TickMethod
    UINT32 u4ReadTick;
#endif
    UINT64 u8Temp;
    UINT64 u8EndAddr;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if((pu8OutStartAddr == NULL) || (pu4OutReadSize == NULL))
    {
        return MPG_RING_FAIL;
    }

#ifdef TickMethod
    u4ReadTick = 0;
#endif
    fgRet = TRUE;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8OutStartAddr = u8StartAddr;
        *pu4OutReadSize = u4ReadSize;
        return MPG_RING_OK;
    }
    else//if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            LOG(9, "_SWDMX_MpgHandleRing valid range is not set\n");
            return MPG_RING_FAIL;
        }

        u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;

        ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            return MPG_RING_FAIL;
        }

        if(i4PlaySpeed >= 0)
        {
            fgRet = _SWDMX_MpgGetPosByTickNum(u1SrcId,
                FALSE,
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum),
                &u8Temp);

            if(fgRet)
            {
                if(u8EndAddr > u8Temp)
                {
                    u8EndAddr = _SWDMX_MpgTSAlignDown(u1SrcId, u8Temp);
                    if(u8EndAddr < u8StartAddr)
                    {
                        u8EndAddr = u8StartAddr;
                    }
                    else if((u8EndAddr - u8StartAddr) > u4ReadSize)
                    {
                        u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;
                    }
                    u4ReadSize = (UINT32)(u8EndAddr - u8StartAddr);
                    if(u4ReadSize == 0)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum++;
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum + 200) <
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum)
                        {
                            LOG(3, "u4ReadTickNum + 200 < u4CurTickNum\n");
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum += 100;
                        }
                    }
                }
            }
        }

#ifdef TickMethod
        fgRet = _SWDMX_MpgGetTickByPos(u1SrcId,
                    FALSE,
                    u8StartAddr,
                    i4PlaySpeed,
                    &u4ReadTick);
        if(fgRet)
        {
            if(i4PlaySpeed >= 0)
            {
                fgRet = _SWDMX_MpgGetPosByTickNum(
                    FALSE,
                    (u4ReadTick + 1),
                    &u8Temp);

                if(fgRet)
                {
                    if(u8EndAddr > u8Temp)
                    {
                        u8EndAddr = _SWDMX_MpgTSAlignDown(u1SrcId, u8Temp);
                        if(u8EndAddr < u8StartAddr)
                        {
                            u8EndAddr = u8StartAddr;
                        }
                        else if((u8EndAddr - u8StartAddr) > u4ReadSize)
                        {
                            u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;
                        }
                        u4ReadSize = (UINT32)(u8EndAddr - u8StartAddr);
                    }
                }
                /*
                else
                {
                    // out of range
                    *pu8OutStartAddr = u8StartAddr;
                    *pu4OutReadSize = 0;
                    return MPG_RING_END_MEET;
                }
                */
            }
        }
        //else
        {
            u4ReadTick = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
        }
#endif

        u8Temp = _SWDMX_Div6464((u8StartAddr - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder);
        u8EndAddr = _SWDMX_Div6464((u8EndAddr - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder);
        if(u8EndAddr == u8Temp)
        {
            u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;
        }
        else
        {
            u8EndAddr = (u8EndAddr * prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize) +
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset;
            u4ReadSize = (u8EndAddr - u8StartAddr);
        }

        if(prSwdmxInst)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA)
                &&
               // while we are change, mw will set new monitor addr
               // but we are still in old speed
               // we should make sure the speed information is synced.
               (!prSwdmxInst->fgPlayChange) // not changing speed
                &&
               (
                ((i4PlaySpeed >= 0) &&
                 (u8EndAddr >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
                ||
                ((i4PlaySpeed < 0) &&
                 (u8StartAddr < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
               )
              )
            {
                VDP_PIC_INFO_T rVdpPicInfo;
                UINT64 u8DispOffset = 0;
                x_memset(&rVdpPicInfo, 0, sizeof(VDP_PIC_INFO_T));
                if(i4PlaySpeed > 0)
                {
                    u8DispOffset = (UINT64)-1;
                }
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    if (VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rVdpPicInfo) == VDP_SET_OK)
                    {
                        u8DispOffset = rVdpPicInfo.u8OffsetDisp;
                    }
                }

                if(
                   (
                    ((i4PlaySpeed >= 0) &&
                     (u8DispOffset >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
                    ||
                    ((i4PlaySpeed < 0) &&
                     (u8DispOffset < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
                   )
                  )
                {
                    LOG(3, "fgMonitorLBA addr 0x%llx, start addr 0x%llx end addr 0x%llx, u8DispOffset 0x%llx\n",
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr,
                        u8StartAddr,
                        u8EndAddr,
                        u8DispOffset);
                    if((prSwdmxInst) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                        {
                            _SWDMX_RANGE_CALLBACK(
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_MONITOR_LBA_ARRIVAL,
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                (UINT32)u1SrcId);
                        }
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgHandleRing prSwdmxInst null\n");
        }
#ifdef TickMethod
        if((u4ReadTick + 1) >=
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick)
#else
        if(u8EndAddr >=
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr))
#endif
        {
            // allow this position
            //*pu8OutStartAddr = u8StartAddr;
            //*pu4OutReadSize = 0;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
               (i4PlaySpeed >= 0))
               //&&
               // codition is _u8SendedSize, not _u8ReadPos
               // Then we will notify on playback to this position.
               // -> rollback to original method
               //(_u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorSafeEnd tick + 1 %d safe end tick %d\n",
                    (u4ReadTick + 1),
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick);
#else
                LOG(3, "fgMonitorSafeEnd addr 0x%llx safe end addr 0x%llx\n",
                    u8EndAddr,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr);
#endif
                if((prSwdmxInst) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_END,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
            }
            // allow this position
            //if(i4PlaySpeed > 0)
            //{
            //    u4Ret = MPG_RING_FAIL;
            //}
        }
        else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
        }

#ifdef TickMethod
        if((u4ReadTick) <
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick)
#else
        if((u8StartAddr) <
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr))
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
               (i4PlaySpeed < 0))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorSafeStart tick %d safe end tick %d\n",
                    (u4ReadTick),
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick);
#else
                LOG(3, "fgMonitorSafeStart addr 0x%llx safe start addr 0x%llx\n",
                    u8StartAddr,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr);
#endif
                if((prSwdmxInst) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_START,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
            }
            if(i4PlaySpeed < 0)
            {
                u4Ret = MPG_RING_FAIL;
            }
        }
        else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
        }

#ifdef TickMethod
        if((u4ReadTick + 1) >=
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick)
#else
        if(u8EndAddr >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr)
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            u4ReadSize = 0;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
               (i4PlaySpeed >= 0))
               //&&
               // codition is _u8SendedSize, not _u8ReadPos
               // Then we will notify on playback to this position.
               //(_u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr))
               // we never out of valid range when we check with _u8SendedSize
               // -> rollback to original method
               //(_u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorValidEnd tick + 1 %d valid end tick %d\n",
                    (u4ReadTick + 1),
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick);
#else
                LOG(3, "fgMonitorValidEnd addr 0x%llx valid end addr 0x%llx\n",
                    u8EndAddr,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr);
#endif
                if((prSwdmxInst) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_END,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
            }

            u4Ret = MPG_RING_FAIL;

            // if we are doing backward
            //and there is no signal anymore (u8ValidEndAddr)
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
               (i4PlaySpeed < 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                u4Ret = MPG_RING_END_MEET;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
            }
        }

#ifdef TickMethod
        if((u4ReadTick) <
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick)
#else
        if(u8StartAddr < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr)
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
               (i4PlaySpeed < 0))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorValidStart tick %d valid start tick %d\n",
                    (u4ReadTick),
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick);
#else
                LOG(3, "fgMonitorValidStart addr 0xllx valid start addr 0x%llx\n",
                    u8StartAddr,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr);
#endif
                if((prSwdmxInst) &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_START,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
            }
            u4Ret = MPG_RING_FAIL;
        }

        if(u4ReadSize == 0)
        {
            //LOG(3, "_SWDMX_MpgHandleRing u4ReadSize 0\n");
            // out of range
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            return MPG_RING_FAIL;
        }

        ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            return MPG_RING_FAIL;
        }
        UNUSED(_SWDMX_Div6464((u8StartAddr - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));
        u8Temp = u8Remainder;

        *pu8OutStartAddr =
            u8Temp + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset;
        *pu4OutReadSize = u4ReadSize;

        if((u4Ret == MPG_RING_OK) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst) &&
           ((i4PlaySpeed >= 0) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            (i4PlaySpeed <= 1500)
#else
            (i4PlaySpeed < 4000)
#endif
           ))
        {
            LOG(5, "First u8OutStartAddr 0x%llx, u4OutReadSize 0x%x\n",
                *pu8OutStartAddr, *pu4OutReadSize);
        }
    }
#endif
    return u4Ret;
}

INT32 _SWDMX_MpgHandleFeeder(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed)
{
    INT32 fgRet;
    BOOL fgBlockReq;
    INT32 i4Ret;
    UINT32 u4Count;
    UINT32 u4ReadSize;
    UINT32 u4ReadPTS;
    UINT32 u4Ptr;
    UINT64 u8ReadAddr;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;
#ifdef SWDMX_MPG_ACCEPT_NON_SEQUENCE_ACK_ID
    UINT32 u4ReqCnt;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_MpgHandleFeeder (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return MPG_SEND_FAIL;
    }
    
    u8ReadAddr = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos+prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize+prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize);
    u4ReadSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize;
    u4ReadPTS = 0;
#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        UINT32 u4Ret;
        UINT32 u4OutReadSize;
        UINT64 u8OutReadAddr;
        u4Ret = _SWDMX_MpgHandleRing(u1SrcId, u8ReadAddr, u4ReadSize,
            &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
        if(u4Ret == MPG_RING_OK)
        {
            u8ReadAddr = u8OutReadAddr;
            u4ReadSize = u4OutReadSize;
        }
        else
        {
            if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes == 0) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
            {
                LOG(9, "Mpg Fd Ring Full.\n");
                return MPG_SEND_FULL;
            }
            else
            {
                u4ReadSize = 0;
            }
        }
    }
    else
#endif
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
        u8ReadAddr = 0;
        u4ReadSize = 1;
        u4ReadPTS = 0;
    }
    else
    {
        if((UINT64)u4ReadSize > ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize))))
        {
            u4ReadSize = ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize)));
            #if 1//def CC_DLNA_SUPPORT
            if((u4ReadSize == 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE))
            {
                FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
            }
            #endif
        }
        if((u4ReadSize > 0) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
        {
            UINT32 u4Ret;
            UINT32 u4OutReadSize;
            UINT32 u4OutReadPTS;
            UINT64 u8OutReadAddr;

            u4Ret = _SWDMX_MpgHandleAudioIndex(u1SrcId, u8ReadAddr, u4ReadSize,
                &u8OutReadAddr, &u4OutReadSize, &u4OutReadPTS, i4PlaySpeed);
            if(u4Ret == MPG_AUDIO_INDEX_OK)
            {
                u8ReadAddr = u8OutReadAddr;
                u4ReadSize = u4OutReadSize;
                u4ReadPTS = u4OutReadPTS;
            }
        }
    }

    fgBlockReq = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo) ||
        // for audio only file, we also could use multi-request
        ((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)) ||
        ((i4PlaySpeed < 0) ||
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
        ((i4PlaySpeed > 1500)
         &&
         (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        ||
        ((i4PlaySpeed >= 4000)
         &&
         (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))));
#else
        (i4PlaySpeed >= 4000))); // 4x use BlockReq
#endif

    //fgBlockReq = ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo) || (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr)); // 4x use multi request
    //fgBlockReq = TRUE; // request one and wait to got one.

#if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
   if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof)
   {
#endif
    if(fgBlockReq)
    {
        if ((u4ReadSize > 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes == 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
        {
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            UINT32 u4i;
            for(u4i=0;u4i<2;u4i++)
            {
            #endif

            //LOG(3, "Mpg Fd 1 FeederSetRequest size(%d)\n", u4ReadSize);
            i4Ret = FeederSetRequest(prSwdmxInst->eFeederSrc,
                    u4ReadSize,
                    (UINT64)u8ReadAddr,
                    &rFeederCond,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID);            
            if(i4Ret != FEEDER_E_OK)
            {
                #ifdef MPG_CHECK_FEEDER_REQ
                LOG(2, "*******feeder req id = %d failed!\n", (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID - 2));
                #endif
                if(i4Ret == FEEDER_E_BUFFER_FULL)
                {
                    // workaround for buffer full
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount++;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount <= (UINT32)MPG_REQ_BUFFULL_NUM)
                    {
                        LOG(1, "Mpg Fd FeederSetRequest Buffer Full\n");
                        return MPG_SEND_FULL;
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;

#ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_DATA_READ_ERROR,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                    LOG(1, "Mpg Fd FeederSetRequest Fail(%d)\n", (UINT32)i4Ret);
                    UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
                    return MPG_SEND_FAIL;
                }
#endif

                LOG(1, "Mpg Fd FeederSetRequest Fail\n");
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                {
                    _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
                }
                if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
                if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                   (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
                }
                return MPG_SEND_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID+=2;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadPTS;
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au8ReqFileOffset[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u8ReadAddr;
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-2);
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqDup[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] =
                  u4i;
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes++;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(7, "Mpg Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS 0x%llx RPTS %d Id %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
                u4ReadSize,
                u8ReadAddr,
                u4ReadPTS,
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-2));
            #endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx++;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
            }
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            }
            #endif
        }
        else
        {
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(9, "Mpg Fd Use UB %d BT %d RT %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
    else
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(1, "Mpg Fd input buffer error.\n");
            return FALSE;
        }
        if ((u4ReadSize > 0) &&
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes + 1) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum) &&
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes + 1 + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum) &&
            #else
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum) &&
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum) &&
            #endif
            //((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize +
            //  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize + u4ReadSize) <=
            // (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize*prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum)))
            // feeder now is 512 K in audio es case
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH) ||
            (prSwdmxInst->rFeederInfo.u4FreeSize >
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize +
              MPG_FEEDER_ALIGN))))
        {
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            UINT32 u4i;
            for(u4i=0;u4i<2;u4i++)
            {
            #endif
            //LOG(3, "Mpg Fd 2 FeederSetRequest size(%d)\n", u4ReadSize);
            i4Ret = FeederSetRequest(prSwdmxInst->eFeederSrc,
                    u4ReadSize,
                    (UINT64)u8ReadAddr,
                    &rFeederCond,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID);            
            if(i4Ret != FEEDER_E_OK)
            {
                if(i4Ret == FEEDER_E_BUFFER_FULL)
                {
                    // workaround for buffer full
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount++;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount <= (UINT32)MPG_REQ_BUFFULL_NUM)
                    {
                        LOG(1, "Mpg Fd FeederSetRequest Buffer Full\n");
                        return MPG_SEND_FULL;
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;

#ifdef TIME_SHIFT_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_DATA_READ_ERROR,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                    LOG(1, "Mpg Fd FeederSetRequest Fail(%d)\n", (UINT32)i4Ret);
                    UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
                    return MPG_SEND_FAIL;
                }
#endif

                LOG(1, "Mpg Fd FeederSetRequest Fail\n");
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                {
                    _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
                }
                if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
                if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                   (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
                }
                return MPG_SEND_FAIL;
            }
            #ifdef MPG_CHECK_FEEDER_REQ
            LOG(2, "*******feeder req id = %d req size = %d, sucess!\n", (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID - 2),
                u4ReadSize);
            #endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID+=2;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadPTS;
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au8ReqFileOffset[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u8ReadAddr;
#endif
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqDup[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] =
                  u4i;
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-2);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes++;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(7, "Mpg Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS 0x%llx RPTS %d Id %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
                u4ReadSize,
                u8ReadAddr,
                u4ReadPTS,
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-2));
            #endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx++;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
            }
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            }
            #endif
        }
        else
        {
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(9, "Mpg Fd Use UB %d BT %d RT %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
#if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
   }
#endif
    #ifdef SWDMX_STATISTICS
    HAL_GetTime(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rFeederReqStartTime);
    #endif

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0))
    {
        // when get eof from feeder, mean data all give to swdmx, no needer continue to ack
        // u4UsingBufSize = 0, u4BufAvailCnt = 0, fgGetEof = 1, will send eos in this function.
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof))
        {
            u4Count = (5 + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum); // the times try to remove previous feeder's ack
            while(u4Count--)
            {
                x_memset(&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0))
                //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
                {
                    fgRet = _SWDMX_ReceiveFeederAck(u1SrcId, (VOID*)&rFeederReqData);
                }
                else
                {
                    fgRet = TRUE;
                    i4Ret = _SWDMX_ReceiveFeederAckNoWait(u1SrcId, (VOID*)&rFeederReqData);
                    if(i4Ret == OSR_NO_MSG)
                    {
                        break; //return MPG_SEND_OK;
                    }
                    else if(i4Ret != OSR_OK)
                    {
                        LOG(5, "Mpg Fd AckNoWait i4Ret %d\n", i4Ret);
                        fgRet = FALSE;
                    }
                }
                // check rFeederReqData.u4Id
                if(rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
                {
                    LOG(2, "Mpg Fd FEEDER_SOURCE_INVALID, ack id = %d\n", rFeederReqData.u4Id);
                    // Feeder add error code for us.
                    if(fgRet==OSR_TIMEOUT)
                    {
                        // workaround for reset feeder request
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount++;
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount > (UINT32)MPG_REQ_TIMEOUT_NUM)
                        {
                            UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount = 0;
                            //reset request CR [DTV00211387]
                            LOG(1, "Mpg Fd Timeout count meet\n");
                        }
                    }
                    return MPG_SEND_FULL;
                }
                // Feeder add error code for us. u4WriteAddr = 0 is error
                if((rFeederReqData.u4WriteAddr == 0) && (!rFeederReqData.fgEof))
                {
                    LOG(1, "Mpg Fd u4WriteAddr = 0. Reset request.\n");
                    UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));         //reset request CR [DTV00211387]
                    return MPG_SEND_FULL;
                }
                
                // CR Review action item , write address invalid
                if( ((rFeederReqData.u4WriteAddr < prSwdmxInst->rFeederInfo.u4StartAddr) ||
                     (rFeederReqData.u4WriteAddr >= prSwdmxInst->rFeederInfo.u4EndAddr)) &&
                     (rFeederReqData.eFeederIBC != FEEDER_IBC_DMX_VUDU_KEY) )
                {
                    LOG(0,"!!!!!!!!prFeederData->u4WriteAddr = %x\n",rFeederReqData.u4WriteAddr);
                    UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
                    return MPG_SEND_FULL;
                }
                
                // Linux NetApp want to have eos
                #if 1//def CC_DLNA_SUPPORT                                 //for DLNA handle EOF
                if(rFeederReqData.fgEof)                                      // get EOF from feeder
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof=TRUE;
                    #if 1//def CC_DLNA_SUPPORT
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                    {
                        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
                    }
                    #endif
                    LOG(1,"get Eof from feeder\n ");
                }
                #endif
                if(rFeederReqData.u4WriteAddr == 0)
                {
                    LOG(1, "Warnning~ Mpg Fd u4WriteAddr = 0. (!rFeederReqData.fgEof).\n");
                    return MPG_SEND_FULL;
                }

                if((!fgRet)
            #if 1//ndef CC_DLNA_SUPPORT                                  //for DLNA handle EOF
              || ((rFeederReqData.u4ReadSize == 0) &&                        // "EOF" has been got  from feeder with nomore  other data
                  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE))
             #endif                                                                       // rFeederReqData.u4ReadSize =0
              )
                {
#ifdef TIME_SHIFT_SUPPORT
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    {
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                        {
                            _SWDMX_RANGE_CALLBACK(
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_DATA_READ_ERROR,
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF,
                                (UINT32)u1SrcId);
                        }
                        LOG(1, "Mpg Fd _SWDMX_ReceiveFeederAck Fail\n");
                        UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
                        return MPG_SEND_FAIL;
                    }
#endif
                    LOG(1, "Mpg Fd _SWDMX_ReceiveFeederAck Fail, handle eos!\n");
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                    {
                        _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
                    }
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                    }
                    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                       // DTV00211818
                       // when audio es playback.
                       // pause -> seek -> seek -> play
                       // we will send finish event. this is not correct.
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                    {
                        if(prRangeList)
                        {
                            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                        }
                    }
                    return MPG_SEND_FAIL;
                }
#ifdef SWDMX_MPG_ACCEPT_NON_SEQUENCE_ACK_ID
                for(u4ReqCnt = 0;u4ReqCnt < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum;u4ReqCnt++)
                {
                    if(rFeederReqData.u4Id == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt])
                    {
                        break;
                    }
                }
                if(u4ReqCnt < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum)
#else
                #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
                if(rFeederReqData.u4Id ==
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                #else
                if(rFeederReqData.u4Id ==
                    ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID -
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes*2))))
                #endif
#endif
                {
                    BOOL fgPut2Buf = TRUE;
                    #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
                    if(0 != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqDup[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                    {
                        // compare data
                        INT32 i4Ret;
                        UINT32 u4Size;
                        UINT32 u4SizeOrg;
                        UINT32 u4Size1;
                        UINT32 u4Size2;
                        UINT32 u4BufWriteIdx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx;
                        if(u4BufWriteIdx == 0)
                        {
                            u4BufWriteIdx =
                                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum - 1);
                        }
                        else
                        {
                            u4BufWriteIdx--;
                        }
                        u4Size = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[u4BufWriteIdx];
                        u4SizeOrg = u4Size;
                        u4Size1 = prSwdmxInst->rFeederInfo.u4EndAddr -
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]);
                        if(u4Size > u4Size1)
                        {
                            u4Size = u4Size1;
                        }
                        u4Size2 = prSwdmxInst->rFeederInfo.u4EndAddr -
                            (rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
                        if(u4Size > u4Size2)
                        {
                            u4Size = u4Size2;
                        }
                        // compare data before wrap around
                        i4Ret = x_memcmp(
                            (VOID*)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]),
                            (VOID*)(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset),
                            u4Size);
                        if((UINT32)i4Ret != (UINT32)0)
                        {
                            LOG(0, "Mpg Fd dup 1 compare fail\n");
                            ASSERT(0);
                        }
                        // compare data after wrap around
                        if(u4Size == u4Size1)
                        {
                            i4Ret = x_memcmp(
                                (VOID*)(prSwdmxInst->rFeederInfo.u4StartAddr),
                                (VOID*)(u4Size + rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset),
                                (u4SizeOrg - u4Size));
                            if((UINT32)i4Ret != (UINT32)0)
                            {
                                LOG(0, "Mpg Fd dup 2 compare fail\n");
                                ASSERT(0);
                            }
                        }
                        else if(u4Size == u4Size2)
                        {
                            i4Ret = x_memcmp(
                                (VOID*)(u4Size + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]),
                                (VOID*)(prSwdmxInst->rFeederInfo.u4StartAddr),
                                (u4SizeOrg - u4Size));
                            if((UINT32)i4Ret != (UINT32)0)
                            {
                                LOG(0, "Mpg Fd dup 3 compare fail\n");
                                ASSERT(0);
                            }
                        }

                        fgPut2Buf = FALSE;

                        // eat feeder data
                        // don't care part.
                        /*
                        u4Ptr = rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
                        if(u4Ptr >= prSwdmxInst->rFeederInfo.u4EndAddr)
                        {
                            u4Ptr -= (prSwdmxInst->rFeederInfo.u4EndAddr -
                            	             prSwdmxInst->rFeederInfo.u4StartAddr);
                        }
                        if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, u4Ptr) != FEEDER_E_OK)
                        {
                    		LOG(1, "Mpg Fd FeederUpdateReadPointer fail\n");
                    		//ASSERT(0);
                        }
                        */
                    }
                    #endif
                    #ifdef MPG_CHECK_FEEDER_REQ
                    LOG(2, "*******ack id = %d, size = %d sucess!\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize);
                    #endif
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes--;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
                    if(rFeederReqData.u4ReadSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                    {
                        LOG(1, "Mpg Fd read size too small %d -> %d\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx], rFeederReqData.u4ReadSize);
              #if 1//def CC_DLNA_SUPPORT        //for DLNA handle eof
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type !=
                            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx] = rFeederReqData.u4ReadSize;
                        }
              #endif
                        //ASSERT(0);
                        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx++;
                        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum)
                        //{
                        //    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
                        //}
                        //return MPG_SEND_FAIL;
                    }

                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx] = rFeederReqData.u4ReadSize;
                    }

                    if(fgPut2Buf)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx] =
                                rFeederReqData.u4WriteAddr +
                                rFeederReqData.u4AlignOffset;
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.afgBufFeederIBCType[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx] =
                            (UINT32)rFeederReqData.eFeederIBC;
//#ifdef CC_VUDU_SUPPORT
                        if(rFeederReqData.eFeederIBC == FEEDER_IBC_DMX_VUDU_KEY)
                        {
                            x_memcpy((VOID*)&_arVuduKey[u1SrcId][prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx],
                                (VOID*)rFeederReqData.u4WriteAddr,
                                sizeof(DMX_VUDU_KEY_T));
                            if(sizeof(DMX_VUDU_KEY_T) != rFeederReqData.u4ReadSize)
                            {
                                LOG(3, "(sizeof(DMX_VUDU_KEY_T)(%d) != rFeederReqData.u4ReadSize(%d))",
                                    sizeof(DMX_VUDU_KEY_T),
                                    rFeederReqData.u4ReadSize);
                            }
                        }
//#endif
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
                        #if 0
                        {
                            UCHAR *pucBuf = (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx]);
                            LOG(1, "FileOffset(0x%llx) size(0x%x) mem(0x%x) 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au8ReqFileOffset[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                                (UINT32)pucBuf,
                                *((UCHAR*)pucBuf + 0),
                                *((UCHAR*)pucBuf + 1),
                                *((UCHAR*)pucBuf + 2),
                                *((UCHAR*)pucBuf + 3),
                                *((UCHAR*)pucBuf + 4),
                                *((UCHAR*)pucBuf + 5),
                                *((UCHAR*)pucBuf + 6),
                                *((UCHAR*)pucBuf + 7));
                        }
                        #endif
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au8BufFileOffset[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au8ReqFileOffset[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
#endif
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx];

                        // though ack size is 0, should also u4BufAvailCnt++, or the req num may not match
                       //if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx])!=0)                        //for DLNA handle EOF
                       {
                           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt++;
                       }
                        #ifdef SWDMX_MPG_DEBUG_BUFFER
                        LOG(7, "Mpg Fd Get UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPTS %d FId %d FAdr %d FOfs %d\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            rFeederReqData.u4Id,
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4AlignOffset);
                        #endif
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx++;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
                    }
                    if(fgPut2Buf)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx++;
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum)
                        {
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
                        }
                    }

                    if(fgPut2Buf)
                    {
#ifdef TIME_SHIFT_SUPPORT
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                           ((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+4)) != 0x47))
                        {
                            UINT32 i;
                            LOG(3, "Mpg Fd !0x47\n");
                            ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize);
                            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize)
                            {
                                UINT64 u8Tmp;
                                UINT64 u8Remainder;
                                u8Tmp = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)192, &u8Remainder);
                                UNUSED(_SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));
                                LOG(1, "!0x47, FileOffset 0x%llx, u8SendedSize 0x%llx, LBA 0x%llx, FilePos 0x%llx\n",
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset,
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                                    (UINT64)u8Tmp,
                                    (UINT64)(u8Remainder + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)
                                    );
                            }
                            //ASSERT(0);
                            for(i=5;i<196;i++)
                            {
                                if((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+i)) == 0x47)
                                {
                                    _SWDMX_MpgResetData(u1SrcId);
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += (i - 4);
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                                    break;
                                }
                            }
                            return MPG_SEND_FULL;
                        }
#endif
// only enable this check on multiple instance
// in single instance, dmx could auto sync 0x47
#ifdef CC_FBM_TWO_FBP
                        if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192)) &&
                            ((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                            (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK)) &&
                           ((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+4)) != 0x47))
                        {
                            UINT32 i;
                            LOG(3, "Mpg Fd !0x47\n");
                            //ASSERT(0);
                            for(i=5;i<196;i++)
                            {
                                if((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+i)) == 0x47)
                                {
                                    //_SWDMX_MpgResetData(u1SrcId);
                                    //UNUSED(_SWDMX_MpgResetInputData);
                                    _SWDMX_MpgResetInputData(u1SrcId);
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += (i - 4);
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                                    break;
                                }
                            }
                            return MPG_SEND_FULL;
                        }
#endif
                    }

                    if(!fgPut2Buf)
                    {
                        return MPG_SEND_SKIP;
                    }

                    break;
                }
                else
                {
                    if(rFeederReqData.u4WriteAddr)
                    {
                        #ifdef SWDMX_MPG_DEBUG_BUFFER
                        LOG(7, "Mpg Fd Skp UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPTS %d FId %d FAdr %d FRS %d\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            rFeederReqData.u4Id,
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4ReadSize);
                        #endif
                        u4Ptr = rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
                        if(u4Ptr >= prSwdmxInst->rFeederInfo.u4EndAddr)
                        {
                            u4Ptr -= (prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr);
                        }
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                        {
                            u4Ptr = 0;
                        }
                        #if 0
                        if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, u4Ptr) != FEEDER_E_OK)
                        {
                            LOG(1, "Mpg Fd FeederUpdateReadPointer fail\n");
                            //ASSERT(0);
                        }
                        #else
                        {
                            UINT32 u4ReqID = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID;
                            UINT32 u4RegIDSize = 0;
                            if(u4ReqID >= rFeederReqData.u4Id)
                            {
                                u4RegIDSize = (u4ReqID - rFeederReqData.u4Id);
                            }
                            else
                            {
                                u4RegIDSize = ((0xFFFFFFFF - rFeederReqData.u4Id) + u4ReqID);
                            }
                            if(u4RegIDSize < (SWDMX_FEEDER_ID_GAP/2))
                            {
                                if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, u4Ptr) != FEEDER_E_OK)
                                {
                                    LOG(1, "Mpg Fd FeederUpdateReadPointer fail\n");
                                    //ASSERT ;
                                }
                                //LOG(3, "Mpg Fd FeederUpdateReadPointer %d\n", u4Ptr);
                            }
                            // else case means this updated address is belong to
                            // previous request. If we update this one, it might
                            // cause feeder internal buffer full and can't accept
                            // request anymore.
                        }
                        #endif
                    }
                    // DTV00140229, [DLNA] When play attached mp3 file,repeatedly Fast Backward, system hangs up.
                    // when we are skip the first ack from feeder, it should be a correct one.
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
                    {
                        LOG(3, "Mpg Fd Skp u4BufAvailCnt 0 u4UsingBufSize 0\n");
                        return MPG_SEND_FAIL;
                    }
                }
            }
            if(u4Count == 0)
            {
                LOG(0, "Mpg Fd _SWDMX_ReceiveFeederAck id is not correct\n");
                //ASSERT(u4Count > 0);
                return MPG_SEND_FAIL;
            }
        }
        // u4UsingBufSize = 0, u4BufAvailCnt = 0, fgGetEof = 1, will send eos in this function.
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0)
            {
                BOOL fgSkip = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufStartAddr[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize -= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt--;

//#ifdef CC_VUDU_SUPPORT
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.afgBufFeederIBCType[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx]
                    == (UINT32)FEEDER_IBC_DMX_VUDU_KEY)
                {
                    DMX_PID_T rPid;
                    x_memset(&rPid, 0, sizeof(DMX_PID_T));
                    rPid.eDescMode = ((DMX_VUDU_KEY_T *)(&_arVuduKey[u1SrcId][prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx]))->eMode;
                    
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
                    {

#if defined(CC_MT5365) || defined(CC_MT5395) || defined(SWDMX_IS_POST_MT5368)
                        if(!DMX_SetFilePid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo,
                                DMX_PID_FLAG_DESC_MODE, &rPid))
#else
                        if(!DMX_SetPid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo,
                                DMX_PID_FLAG_DESC_MODE, &rPid))
#endif
                        {
                            LOG(3, "MpgHandleFeeder DMX_SetPid V DMX_PID_FLAG_DESC_MODE fail\n");
                        }
                    }
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
                    {
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(SWDMX_IS_POST_MT5368)
                        if(!DMX_SetFilePid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio,
                                DMX_PID_FLAG_DESC_MODE, &rPid))
#else
                        if(!DMX_SetPid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio,
                                DMX_PID_FLAG_DESC_MODE, &rPid))
#endif
                        {
                            LOG(3, "MpgHandleFeeder DMX_SetPid A DMX_PID_FLAG_DESC_MODE fail\n");
                        }
                    }
    
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr = 0;
                    if(!DMX_VUDU_SetKey(&_arVuduKey[u1SrcId][prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx]))
                    {
                        LOG(3, "MpgHandleFeeder DMX_VUDU_SetKey fail\n");
                    }
                    LOG(5, "MpgHandleFeeder DMX_VUDU_SetKey OK\n");
                    fgSkip = TRUE;
                }
//#endif
                #ifdef SWDMX_MPG_DEBUG_BUFFER
                LOG(7, "Mpg Fd Eat UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d BRS %d BPTS %d UAdr %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufSize[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx],
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4BufPTS[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx],
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
                #endif
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx++;
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
                }
                if(fgSkip)
                {
                    return MPG_SEND_SKIP;
                }
                // sometimes, ack size may be 0
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
                    return MPG_SEND_FAIL;
            }
            else
            {
                LOG(3, "Mpg Fd u4BufAvailCnt 0 u4UsingBufSize 0\n");
                #if 1//def CC_DLNA_SUPPORT                                 //for DLNA handle EOF
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof)                                      // get EOF from feeder
                {
                     LOG(3, "Mpg Fd u4BufAvailCnt 0 u4UsingBufSize and send EOS 0\n");
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                    {
                        _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
                    }
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                    }
                    if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                       (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                    {
                        if(prRangeList)
                        {
                            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                        }
                    }
                    return MPG_SEND_FAIL;
                }
                #endif
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
                {
                    //DTV00334590 add for the file size is error
                    if(((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize >> 32) != (UINT32)0xFFFFFFFF) &&
                        ((UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize) != (UINT32)0xFFFFFFFF))
                    {
                        ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0);
                    }
                    else
                    {
                        LOG(1, "can not get file size, handle eos\n");
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                        if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                        {
                            _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
                        }
                        if(prRangeList)
                        {
                            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                        }
                        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                           (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                        {
                            if(prRangeList)
                            {
                                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                            }
                        }
                    }
                }
                return MPG_SEND_FAIL;
            }
        }
    }
    #if 1//def CC_DLNA_SUPPORT                           //for DLNA handle EOF
    if(( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgGetEof==TRUE) &&(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt==0))
    {
        LOG(7,"swdmx  will send  the Last data \n ");
        return MPG_SEND_LAST;
    }
    #endif
    #ifdef SWDMX_STATISTICS
    _CalFeederTime();
    #endif

    return MPG_SEND_OK;
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
/**
*/
INT32 _SWDMX_MpgSendData(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList,
            SWDMX_CUR_CMD_T* prCmd)
{
#ifdef TIME_SHIFT_SUPPORT
    BOOL fgRet;
    BOOL fgVideoScambled =FALSE;
	BOOL fgAudioScambled =FALSE;
#endif
    BOOL fgLast = FALSE;
    INT32 i4Ret;
    UINT32 u4SendSize;
    UINT32 u4Ptr;
    UINT32 u4WaitCnt;
    UINT64 u8SeekPos;
    INT32 i4PlaySpeed;

    DMX_MM_DATA_T rDmxMMData;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    u8SeekPos = prCmd->u8SeekPos;
    i4PlaySpeed = prCmd->i4PlaySpeed;
    UNUSED(u8SeekPos);
#ifdef CC_SWDMX_RENDER_POSITION
    if((prRangeList) &&
       (prRangeList->fgSetRenderPosition) &&
       (((UINT32)((prRangeList->u8Offset) >> 32) != (UINT32)0xFFFFFFFF) ||
       ((UINT32)(prRangeList->u8Offset) != (UINT32)0xFFFFFFFF)) &&
       (((UINT32)((prRangeList->u8Offset) >> 32) != (UINT32)0x0) ||
       ((UINT32)(prRangeList->u8Offset) != (UINT32)0x0)) &&
       // TS file, no sequence header need, decide position here
       // audio es file, should decide position here
       ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES)||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES)||
        (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)))
    {
        UINT64 u8FilePos = prRangeList->u8OffsetI;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = prRangeList->u4Timestap;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekaud = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SeekAudPTS = prRangeList->u8StartPts;
        if((((UINT32)(u8FilePos >> 32) == (UINT32)0xFFFFFFFF)
            && ((UINT32)(u8FilePos) == (UINT32)0xFFFFFFFF)) ||
            (prRangeList->u8OffsetI > prRangeList->u8Offset))
        {
            if(prRangeList->u8Offset > 0xA00000)
            {
                u8FilePos = (prRangeList->u8Offset - 0xA00000);
            }
            else
            {
                u8FilePos = 0x0;
            }
            if(u8FilePos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
            {
                u8FilePos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
            }
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            if(_SWDMX_MpgGetPosByTime(u1SrcId, TRUE, prRangeList->u8StartPts, &u8FilePos))
            {
            }
            else
            {
                u8FilePos = prRangeList->u8OffsetI;
            }
            STC_StopStc(prSwdmxInst->u1StcId);
            STC_SetStcValue(prSwdmxInst->u1StcId, prRangeList->u8StartPts);
        }

        /* DTV00325451, some video es file cannot be decoded if not from file start. If render pos < 10M,
                   send data from file start, just a temp solution. */
        if((u8FilePos < 0xA00000) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt >= SWDMX_FMT_VC1_ES) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt <= SWDMX_FMT_AVS_VIDEO_ES))
        {
            u8FilePos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
        }
        
        prRangeList->fgSetRenderPosition = FALSE;
        UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, TRUE,
            prRangeList->u8Offset,
            prRangeList->i4TempRef,
            prRangeList->u2DecodingOrder));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = u8FilePos;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = prRangeList->u8StartPts;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq)
        {
            LOG(5, "SendData Position RenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, prRangeList->u8Offset, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS, prRangeList->i4TempRef);
        }
        UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
        UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));

        _SWDMX_MpgResetData(u1SrcId);
        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
        if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
           (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
        {
            UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, TRUE,
                prRangeList->u8Offset,
                prRangeList->i4TempRef,
                prRangeList->u2DecodingOrder));
            UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
            UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
            MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, FALSE);
        }
    }
#endif

    // handle speed and position
    // must handle position before we parsing first sequence header.
    if(prCmd->fgChange)
    {
        _SWDMX_MpgHandleChange(u1SrcId, prCmd);
    }
    
#ifdef PVR_START_OFFSET
    while(_fgPvrStartOffset && 
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio || 
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS))
    {
        _fgPvrStartOffset = FALSE;
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->t_vid_start_render_pts))
        {
            UINT64 u4_seek_pos = 0;
            UINT64 u8_remainder = 0;
            UINT32 u4_seek_time = (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->t_vid_start_render_pts);
            LOG(5, "PVR start play from time(%d), total time(%d), filesize(0x%x)\n", 
                u4_seek_time/90000, 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime/90000,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
            if(u4_seek_time >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime)
                break;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0)
            {
                u4_seek_pos = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - 
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset) * (u4_seek_time),
                    (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime), &u8_remainder);
            }
            else
            {
                break;
            }
            u4_seek_pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
            
            LOG(5, "Pre Off(%d) (%d)\n", u4_seek_pos, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset);
            u4_seek_pos = _SWDMX_Div6464(u4_seek_pos, (UINT64)192,&u8_remainder)*((UINT64)192);            
            LOG(5, "Now Off(%d)\n", u4_seek_pos);  
            
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize = u4_seek_pos;
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));                    
                UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, TRUE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
            _SWDMX_MpgResetData(u1SrcId);
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
        }         
    }
#endif

    // this is first run for one file, we have to get sequence header
    if(_SWDMX_MpgGetFirstSeqHdr(u1SrcId))
    {
        u8SeekPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OrgSendedSize;
        //fgChange = TRUE;
        // I am not sure if we need this
        //_SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
    }

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        BOOL fgVideoNotSupport = FALSE;
        VDEC_HDR_INFO_T rHdrInfo;
		HAL_TIME_T NosigCheckTime;
        UNUSED(VDEC_QueryInfo(prSwdmxInst->u1VdecId, &rHdrInfo));
        /*
        if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED)
        {
            fgVideoNotSupport = TRUE;
        }
        else
        //VDEC_DEC_DECODE_HD_NOT_SUPPORT, etc.
        if((rHdrInfo.eNotifyStatus != VDEC_DEC_DECODE_UNKNOWN) &&
           (rHdrInfo.eNotifyStatus != VDEC_DEC_DECODE_NORMAL) &&
           (rHdrInfo.eNotifyStatus != VDEC_DEC_DECODE_NO_DATA))
        {
            fgVideoNotSupport = TRUE;
        }
        */
     if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
     {
	   if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED)
	   {
		   fgVideoScambled=TRUE;
	   }
     }
     if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
     {
       if(DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio) == DMX_SCRAMBLE_STATE_SCRAMBLED)
       {
	      fgAudioScambled=TRUE;
       }
     }
     //DTV00095614  video scramble, audio only, when set speed 1x, we should pause swdmx
     if((fgVideoScambled)&&(i4PlaySpeed==1))
	 {
	     SWDMX_Pause(u1SrcId);
		 if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
		 {
			 _SWDMX_RANGE_CALLBACK(
				    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_RANGE_SEEK_DONE,
					prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
					0xFFFFFFFF,
					(UINT32)u1SrcId);
	     }
	     else
	     {
			   LOG(3, "pfnSwdmxRangeCb null\n");
		 }
			  LOG(5, "video scramble : MM_RANGE_SEEK_DONE\n");
	}


	 if(((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)&&
		(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)&&fgVideoScambled)
		||		   
		((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)&&
		(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)&&fgAudioScambled)
		||
		((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)&&
		(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)&&fgAudioScambled&&fgVideoScambled))
		{
			 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgInScramble = TRUE;
			 LOG(5,"in scramble status\n");
		}
		else 
		{
		   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgInScramble = FALSE;
		}
		
        //DTV00333609  when unplug CA sim card, video always in sramble status, as swdmx didn't move data,
        // 
		
        //if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_NORMAL) ||
		//  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgInScramble==TRUE))
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_NORMAL)
        {
            // DTV00214748 when speed=1000, and in no signal status, we should calibrate prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime
            //as x_thread_delay() not very exactly
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust)&&(i4PlaySpeed==1000))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust=TRUE;
				HAL_GetTime(&NosigCheckTime);
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OrgtDeltaTime= (NosigCheckTime.u4Seconds*1000 + NosigCheckTime.u4Micros/1000)
					                                 - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime;
            }
            i4Ret = _SWDMX_MpgHandleNoSignel(u1SrcId, &u8SeekPos, i4PlaySpeed);
            //if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL)&&
			//  (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgInScramble!= TRUE))
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL)

            {
                // DTV00145526, The audio does NOT match the video after unplug/plug the signal cable.
                // Because MW does not close audio in this case,
                // we can't flush audio fifo.
                // Audio driver does not know we flush data if we didn't close
                // audio.

#if 1
                //_SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);
                UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    MPV_FlushEsmQ(prSwdmxInst->u1Vdec2Id, FALSE);
                    UNUSED(VDEC_ReleaseDispQ(prSwdmxInst->u1Vdec2Id));
                }
#else
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
                _SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
                //MPV_FlushEsmQ(ES0, FALSE);
                //UNUSED(VDEC_ReleaseDispQ(ES0));
#endif
                // TODO
                // Audio also need a flush
                // and resync action
                AUD_DSPCmdStop(prSwdmxInst->u1AdecId);
                AUD_DSPCmdPlay(prSwdmxInst->u1AdecId);
                LOG(0, "%u: Stop&Play audio(Tmp)\n", __LINE__);
            }
            return i4Ret;
        }
        else if(((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
                 ||
                 ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                  (fgVideoNotSupport)))
                &&
                // while we are parsing table, not handle this.
                // DTV00142030, Can't pause time shift playback mode in any audio channel.
                // when there is no video, we do not send data anymore
                // while beginin parsing table, i4PlaySpeed = 0;
                //(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize)
                //&&
                // both audio on and off have to check this.
                //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                (i4PlaySpeed != 1000) &&
                (i4PlaySpeed != 0))
        {
            i4Ret = _SWDMX_MpgHandleEmpty(u1SrcId, &u8SeekPos, i4PlaySpeed);
            return i4Ret;
        }
	   else
	   {
		   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust = FALSE;

	   }
       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal = 0;
    }
#endif

    LOG(9, "_SWDMX_MpgSendData pos 0x%llx\n",
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo)
        {
            LOG(5, "_SWDMX_MpgSendData fgChangeVideo\n");
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
            fgRet = _SWDMX_DMX_Reset(u1SrcId);//DMX_DDI_RequestReset();
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgSendData _SWDMX_DMX_Reset Fail\n");
            }
        }
    }
#endif


    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping && prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext)
    {
        // In worse case,
        // vdp will consume frame in display queue within 3 VSync (pull down).
        // With FPS 25, 1 VSync = 1000/(25*2) = 20;
        // We have to make sure vdp eat this frame.
        #if 0
        u4WaitCnt = 3;
        while(u4WaitCnt--)
        {
            if(_SWDMX_GetNumOfCmd() > 0)
            {
                // no wait display, when we have new command
                u4WaitCnt = 0;
            }
            else
            {
                x_thread_delay(20);
            }
        }

        #else
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount == 0)
        {
            // it's nice to have this. it could improve the trick speed.

            UINT64 u8Offset = 0;
            UINT64 u8OffsetI = 0;
            UINT16 u2DecodingOrder = 0;
            INT32 i4TempRef = 0;
            //static UINT64 u8OffsetKeep = 0;
            //static UINT64 u8OffsetIKeep = 0;
            //static UINT16 u2DecodingOrderKeep = 0;
            //static INT32 i4TempRefKeep = 0;
            u4WaitCnt = 3;
            while(u4WaitCnt--)
            {
                if(VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
                {
                }
                // to check vdp have change frame, so we could leave earlier.
                if(((u8Offset != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetKeep) &&
                    (u8OffsetI != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetIKeep) &&
                    (i4TempRef != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4TempRefKeep) &&
                    (u2DecodingOrder != prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2DecodingOrderKeep))
                   ||
                   (_SWDMX_GetNumOfCmd(u1SrcId) > 0))
                {
                    // no wait display, when we have new command
                    u4WaitCnt = 0;
                }
                else
                {
                    // at least we will delay 60 ms.
                    x_thread_delay(20);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetKeep = u8Offset;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8OffsetIKeep = u8OffsetI;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4TempRefKeep = i4TempRef;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2DecodingOrderKeep = u2DecodingOrder;
        }
        #endif

        if(((i4PlaySpeed == -1)
            ||
            (i4PlaySpeed == 1)
           )
           && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u2JumpAgainCount == 0))
        {
            // no need to pause
            //SWDMX_Pause();
        }
        else
        {
            _SWDMX_MpgHandleSeek(u1SrcId, i4PlaySpeed, FALSE);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
        }
    }
    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping)
            &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0)
        {
            UINT64 u8Tmp;
            UINT64 u8Remainder;
            u8Tmp = _SWDMX_Div6464(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*MPG_AUDIO_ES_SKIP_TIME), (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
            UNUSED(u8Remainder);
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr) >= u8Tmp)
            {
                {
                    UINT32 u4DmxAvailSizeAudio;
                    UINT32 u4Addr, u4Size;
                    UINT32 u4BufSize;
                    #ifdef SWDMX_MPG_DMX_20
                    u4DmxAvailSizeAudio = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                        DMX_PID_TYPE_ES_AUDIO,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                    #else
#ifdef CC_DMX_PURE_AUDIO
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                   	}
                    else
#endif
                    {
                        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
                    #endif
                    if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
                    {
                        LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                        return FALSE;
                    }
                    u4BufSize          = u4Size;
                    if(((u4BufSize - u4DmxAvailSizeAudio) < 100000)
                       &&
                       ((u4BufSize - u4DmxAvailSizeAudio) ==
                         prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize)
                      )
                    {
                        _SWDMX_MpgHandleSeekAudio(u1SrcId, i4PlaySpeed, FALSE);
                        return MPG_SEND_OK;
                    }
                    else
                    {
                        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady) &&
                           (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio))
                        {
                            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                            STC_StartStc(prSwdmxInst->u1StcId);
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                        }

                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize =
                            (u4BufSize - u4DmxAvailSizeAudio);
                        // wait audio consume
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos < u8Tmp*20)  // DTV00089946 for too low bitrate file, 
                        {                                                                  // last 20 second 
						    x_thread_delay(200);
                        }
						else
						{
                            x_thread_delay(100);
						}

                        return MPG_SEND_OK;
                    }
                }
            }
        }
    }

    // must put ahead _SWDMX_MpgHandleSeek
    // there is a case, after send EOS to decoder
    // and before decoder look EOS, decoder found PIC and notify
    // prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = TRUE
    // we must let _SWDMX_MpgHandleSeek call again to deliver EOS.
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF)
    {
    	//for small or low-bitrate file handling, update playpos when EOF. e.g. DTV00088728
		if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
		{
			if(!_SWDMX_MpgGetAudioCurrentPos(u1SrcId))
			{
				LOG(3, "_SWDMX_GetAudioFifo Fail\n");
				return FALSE;
			}
		}

        // audio still not aout after EOS
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady) &&
           (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio))
        {
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
        }

        // DTV00136518, The special .mpe video freezed after input time=00:00:20 and do time seek function.
        // Audio ready comes after eos.
        if((((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady))
            ||
            (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)) &&
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos) &&
             (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause)))
        {
            // i4PlaySpeed = 1x and audio ready later after video send eos
            // if now in VDEC_Pause, VDec might in pause status.
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                ENUM_VDEC_FMT_T eVdecFmt = VDEC_FMT_MPV;
                UNUSED(VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE));
                if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_MPV fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_H264 fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_AVS))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_AVS fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_WMV fail\n");
                    }
                }
                else if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                {
                    eVdecFmt = VDEC_FMT_MPV;
                    if(!VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                    }
                }
                else
                {
                    ASSERT(0);
                }
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    if(!VDEC_Play(prSwdmxInst->u1Vdec2Id, eVdecFmt))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play 2 eVdecFmt fail\n");
                    }
                }
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
                LOG(5, "_SWDMX_MpgSendData VDEC_Play after EOS\n");
            }
        }

        //DTV00091703 when audio eos has been sent to audio drv, but didn't notify to mw,  at this time change audio trick, we should
        // send audio eos again.
		if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) && (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos))
		{
             
			DMX_AUDIO_PES_T rPes;					 
		    UINT32 au4BufStart[2], au4BufEnd[2];
		    x_memset(&rPes, 0, sizeof(rPes));
			if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
			{
				rPes.u4Wp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr;
			}
			else
			{
#ifdef AUD_OLDFIFO_INTF
			    VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
						&au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
				VERIFY(AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
						 rPes.u4Wp = (au4BufStart[0] + 4); // Jessica.
			}
			UNUSED(AUD_SendAudioPes(&rPes)); // audio driver said they need this one.
			rPes.fgEOS = TRUE;
					 
			if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >0)
			{
				rPes.fgForward = TRUE;		   //for trick mode 
			}
			else
			{
				rPes.fgForward = FALSE;
			}			 
			 //_SWDMX_MpgAudioCallback(&rPes);
			UNUSED(AUD_SendAudioPes(&rPes));
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;
			if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio)
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
			 
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
				((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
#ifdef TIME_SHIFT_SUPPORT
			    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
				{
					UNUSED(AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));

				}
				//prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
				//prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
		    }
		    LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Adec\n");
		}		

        LOG(7, "_SWDMX_MpgSendData _fgSendDataEOS true.\n");
        return MPG_SEND_EOS;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst) // first send data
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(3, "_SWDMX_MpgSendData FeederGetBufferInfo Fail\n");
            return MPG_SEND_FAIL;
        }
    }


    // We have to request feeder data first.
    // Then check fifo status.
    //if((i4Ret = _SWDMX_MpgHandleFifoFull()) != MPG_SEND_OK)
    //{
    //    return i4Ret;
    //}

    u4SendSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize;

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
    #if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
    ||((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgLastData)&&(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize==0))
    #endif
        )        // have sended all the data
    {
#ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_INTERNAL_ERROR,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
            LOG(1, "_SWDMX_MpgSendData SWDMX send (EOF), impossible\n");
            //UNUSED(_SWDMX_MpgResetFeederReq(u1SrcId));
            //return MPG_SEND_FAIL;
        }
#endif
#if 0
		 //DTV00089460 for some pvr file size is too small, swdmx will send EOF and delete range before a/v stream has been enable
		 // and when mw enable stream, swdmx has no range to use, so if a/v stream hasn't been enable, we should not delete range
		 if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)||
		 	(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS)||
		 	(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192)||
		 	(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192))&&
		   (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
		   (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)) 	   
		 {
		 
			LOG(5,"_SWDMX_MpgSendData has send to end but a/v still not enale!\n");
			// DTV00093261 for some pvr file size is too small, before mw set PMT pid to dmx, swdmx has send all the data,
			// so for this case , we reset feeder and send data agin  from begining. 
			//DTV000310416 this solution is for all TS file not only pvr file
			//if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
			
			// 2010-11-16,Use new solotuon for this issue, mw wil change the flow to play ts file, so remove these code 
			//(1) parser PaT
			//(2) seek to file start , parser PMT
			//(3) seek to start play a/v 
			#if 0
			{

				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = 0;
				//_SWDMX_MpgResetData(u1SrcId);

				_SWDMX_FlushReqMsgQ(u1SrcId);
                UNUSED(_SWDMX_MpgResetFeeder(u1SrcId));

				LOG(5, "had not get stream send agian\n");

			}
			#endif 
			
			return MPG_SEND_FAIL;
		 } 
		 else
#endif
        {
			LOG(5, "_SWDMX_MpgSendData SWDMX send (EOF)\n");
			prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
		
			if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
			{
			   _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
			}
			if(prRangeList)
			{
			   prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
			}
			if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
			(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
			// DTV00211818
			// when audio es playback.
			// pause -> seek -> seek -> play
			// we will send finish event. this is not correct.
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
			{
			   if(prRangeList)
			   {
				   prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
			   }
			}
			return MPG_SEND_EOS;
		}

    }

    if((UINT64)u4SendSize > ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize)))
    {
        u4SendSize = (UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize));
    }


    if(u4SendSize == 0)
    {
        LOG(1, "_SWDMX_MpgSendData u4SendSize < 0 Fail\n");
        ASSERT(0);
    }

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        UINT32 u4Temp;
        fgRet = _SWDMX_MpgGetTickByPos(u1SrcId,
            TRUE,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
            prCmd->i4PlaySpeed,
            &u4Temp);
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex) &&
           (fgRet))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
            //LOG(5, "Trace u4CurTickNum(%d) u8SendedSize(0x%llx)\n",
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum,
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime =
                (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime *=
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase;

            LOG(9, "_SWDMX_MpgSendData u4CurTickNum 0x%x\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
            if(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
                PVR_TICK_INDEX_FLAG_EMPTY) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_NO_SIGNAL;
                LOG(3, "u4NoSignalStage 1.2.1, PREPARE_NO_SIGNAL tick %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
                return MPG_SEND_FULL;
            }
            else if(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
                PVR_TICK_INDEX_FLAG_BAD) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_BAD_STORAGE;
                LOG(3, "u4NoSignalStage 1.2.2, PREPARE_BAD_STORAGE tick %d\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
                return MPG_SEND_FULL;
            }
        }
        else
        {
            // DTV00060341 [Time Shift] Play directly and display "Retrieving Data..." by press PLAY/PAUSE after record more than 2 hrs.
            // Handle out of range case
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfRange) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr > 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr > 0))
            {
                UINT64 u8Temp;
                UINT32 u4TmpTick = 0, u4SafeStartTick, u4SafeEndTick;

                u4SafeStartTick = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick;
                u4SafeEndTick = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfRange = FALSE;
                
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0)
                {
                    u4TmpTick = ((u4SafeEndTick-2)>u4SafeStartTick) ? 
                                 (u4SafeEndTick-2):
                                  u4SafeEndTick;
                }
                else
                {
                    u4TmpTick = ((u4SafeStartTick+2)<u4SafeEndTick) ?
                                 (u4SafeStartTick+2) :
                                  u4SafeStartTick;
                }

                if (u4TmpTick<u4SafeStartTick || u4TmpTick>u4SafeEndTick)
                {
                    u4TmpTick = (u4SafeStartTick+u4SafeEndTick) / 2;
                }
                
                if (_SWDMX_MpgGetPosByTick(u1SrcId, TRUE, u4TmpTick, &u8Temp))
                {
                    u8Temp = _SWDMX_MpgTSAlignDown(u1SrcId, u8Temp);
                    if(prCmd->i4PlaySpeed >= 0)
                    {
                        u8Temp += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
                    }
                    LOG(3, "Change start position 0x%llx ==========\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
                    prCmd->fgSeek = FALSE;             // original method
                    prCmd->u8SeekPos = u8Temp;
                    _SWDMX_MpgHandleChange(u1SrcId, prCmd);
                    AUD_DSPCmdStop(prSwdmxInst->u1AdecId);
                    AUD_DSPCmdPlay(prSwdmxInst->u1AdecId);
                    LOG(0, "%u: Stop&Play audio(Tmp)\n", __LINE__);
                    return MPG_SEND_FULL;
                }
                else
                {
                     return MPG_SEND_FULL;
                }

                return MPG_SEND_FULL;
            }
        }
    }
#endif

    i4Ret = _SWDMX_MpgHandleFeeder(u1SrcId, prRangeList, prCmd->i4PlaySpeed);

    if(i4Ret != MPG_SEND_OK)
    {
        #if 1//def CC_DLNA_SUPPORT                       // for DLNA handle EOF
        if(i4Ret==MPG_SEND_LAST)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize<=u4SendSize)     // for DLAN handle  EOF
            {
                fgLast=TRUE;                                               // the last group of data will be send to dmx
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.fgLastData=TRUE;
                u4SendSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize;
            }
        }
        else
        #endif
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
                {
                    return i4Ret;
                }
            }
            else
            {
                return i4Ret;
            }
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
    {
#ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            LOG(5, "_SWDMX_MpgSendData u4UsingBufSize 0\n");
            return MPG_SEND_FULL;
        }
#endif
        LOG(0, "_SWDMX_MpgSendData u4UsingBufSize 0\n");
        ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize > 0);
        return MPG_SEND_FAIL;
    }



    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
       ||
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
       ||
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
       ||
       // Network application
       // when eof with request count > 0, but not receive feeder's EOF yet,
       // we don't know it is eof actually. (eof will receive at next ack)
       // And ack size are correct in most case (retry in mtmplayer).
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
       )
    {
        if(u4SendSize > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize)
        {
            u4SendSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        }
    }

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize + u4SendSize) >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
    {
        fgLast = TRUE;
    }

    // Move from above (before _SWDMX_MpgHandleFeeder)
	if((i4Ret = _SWDMX_MpgHandleFifoFull(u1SrcId, i4PlaySpeed, fgLast)) != MPG_SEND_OK)
    {
        prSwdmxInst->fgFifoFull = TRUE;
        return i4Ret;
    }


    x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));
    rDmxMMData.u4FrameSize = u4SendSize;
    rDmxMMData.u4BufStart = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxInst->rFeederInfo.u4EndAddr;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr >= rDmxMMData.u4BufEnd)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr -= (prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr);
    }
    rDmxMMData.u4StartAddr = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    if(rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
    {
        rDmxMMData.u4StartAddr -= (rDmxMMData.u4BufEnd - rDmxMMData.u4BufStart);
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst)
    {
        rDmxMMData.fgFrameHead = TRUE;
        LOG(7, "_SWDMX_MpgSendData fgFrameHead 0x%x-0x%x, 0x%x, S=%d.\n",
            rDmxMMData.u4BufStart, rDmxMMData.u4BufEnd,
            rDmxMMData.u4StartAddr, rDmxMMData.u4FrameSize);
    }
    if(fgLast)
    {
        rDmxMMData.fgEOS = TRUE;
    }

    //CR DTV00213030  for linux turkey because timing issue,
    //after seek ,at the time we are sending data to dmx but stream ID have not been enable yet
    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)&&(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio))
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS)
            {
                rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS;
                rDmxMMData.u4Dts = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS = 0;
            }
        }
        else
        {
            UINT64 u8Remainder;

            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS)
            {
                rDmxMMData.u4Pts = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS;
                rDmxMMData.u4Dts = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingPTS = 0;
            }
            // actually, below condition will not entry anymore.
            // because we have calculated it in _SWDMX_MpgHandleAudioIndex
            else
            if((rDmxMMData.fgFrameHead) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > 0))
            {
                rDmxMMData.u4Pts = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*((UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime),
                    (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset), &u8Remainder);
                rDmxMMData.u4Pts += MPG_STC_BASE;
                rDmxMMData.u4Dts = rDmxMMData.u4Pts;
            }
        }
    }

    if((i4Ret = _SWDMX_MpgHandleMoveData(u1SrcId, &rDmxMMData, i4PlaySpeed)) != MPG_SEND_OK)
    {
        return i4Ret;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

    // add for DTV00098584
    if(i4PlaySpeed < 0 
        && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize <= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
        && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
    {
        _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;

        if(prRangeList)
        {
             prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
        
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
            (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo))
        {
            if(prRangeList)
            {
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
            }
        }
        return MPG_SEND_OK;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize >= u4SendSize)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += u4SendSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr += u4SendSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize -= u4SendSize;
        //_u4AvailSize += u4SendSize;

        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH))
        {
            if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 0) != FEEDER_E_OK)
            {
                LOG(1, "_SWDMX_MpgSendData FeederUpdateReadPointer fail\n");
            }
        }
    }
    else
    {
        if(!fgLast)
        {
            LOG(1, "_SWDMX_MpgSendData u4UsingBufSize not correct\n");
            //ASSERT(0);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
            {
                 _SWDMX_MpgHandleEOS(u1SrcId, i4PlaySpeed);
            }
            if(prRangeList)
            {
                 prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
               (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
               {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
               }
                return MPG_SEND_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
        {
            if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 0) != FEEDER_E_OK)
            {
                LOG(1, "_SWDMX_MpgSendData FeederUpdateReadPointer fail\n");
            }
        }
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos)
    {
        LOG(1, "_SWDMX_MpgSendData u8SendedSize >  u8ReadedPos\n");
        ASSERT(0);
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
    	if(!_SWDMX_MpgGetAudioCurrentPos(u1SrcId))
		{
			LOG(3, "_SWDMX_GetAudioFifo Fail\n");
			return FALSE;
    	}
    }

    u4Ptr = rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize;
    if(u4Ptr >= prSwdmxInst->rFeederInfo.u4EndAddr)
    {
        u4Ptr -= (prSwdmxInst->rFeederInfo.u4EndAddr - prSwdmxInst->rFeederInfo.u4StartAddr);
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, u4Ptr) != FEEDER_E_OK)
    {
        LOG(1, "_SWDMX_MpgSendData FeederUpdateReadPointer fail\n");
        //ASSERT(0);
    }

#ifdef TIME_SHIFT_SUPPORT
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping) &&
       (DMX_GetScrambleState(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
    }
#endif

    // we use cpu to parse program stream.
    // when clip contains many garbage, we should delay a while to release CPU
    // to other threads.
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS)
    {
        BOOL fgRetFULL = FALSE;
        UINT64 u8Escape;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CheckedPos > 
           prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize)
        {
            u8Escape = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CheckedPos - 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        }
        else
        {
            u8Escape = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - 
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CheckedPos;
        }
        if(u8Escape > CHECK_NO_DATA_SIZE)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CheckedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgPesFound)
            {
                fgRetFULL = TRUE;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgPesFound = FALSE;
            if(fgRetFULL)
            {
                x_thread_delay(CHECK_NO_DATA_DELAY);
            }
        }
    }

    return MPG_SEND_OK;
}


BOOL _SWDMX_MpgSendDataBatch(UINT8 u1SrcId)
{
    INT32 i4Ret;
    SWDMX_CUR_CMD_T rCmd;
    // detect first
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    x_memset(&rCmd, 0, sizeof(SWDMX_CUR_CMD_T));
    while(1)
    {
        i4Ret = _SWDMX_MpgSendData(u1SrcId, NULL, &rCmd);
        if(i4Ret == MPG_SEND_OK)
        {
            continue;
        }
        else if(i4Ret == MPG_SEND_FULL)
        {
            x_thread_delay(1);
        }
        else if(i4Ret == MPG_SEND_EOS)
        {
            LOG(7, "_SWDMX_MpgSendDataBatch _SWDMX_MpgSendData Finish.\n");
            break;
        }
        else if(i4Ret == MPG_SEND_FAIL)
        {
            LOG(5, "_SWDMX_MpgSendDataBatch _SWDMX_MpgSendData Fail.\n");
            return FALSE;
        }
        else
        {
            LOG(5, "_SWDMX_MpgSendDataBatch _SWDMX_MpgSendData Unknow return.\n");
            return FALSE;
        }

        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_MpgResetData(u1SrcId);
        //}
    }
    return TRUE;
}

BOOL _SWDMX_MpgSendDataBatchForTotalTime(UINT8 u1SrcId)
{
    INT32 i4Ret;
    SWDMX_CUR_CMD_T rCmd;
    // detect first
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    x_memset(&rCmd, 0, sizeof(SWDMX_CUR_CMD_T));
    while(1)
    {
        i4Ret = _SWDMX_MpgSendData(u1SrcId, NULL, &rCmd);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS)
        {
            // there is some fake PTS in this file end.
            // please check program 4 and 8. (1 base).
            // [DTV00047346] [TS file playback] Can NOT display the TS, log will print fail info.
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTSCnt >= MPG_PTS_CHECK_NUM)
            {
                break;
            }
        }
        if(i4Ret == MPG_SEND_OK)
        {
            continue;
        }
        else if(i4Ret == MPG_SEND_FULL)
        {
            x_thread_delay(1);
        }
        else if(i4Ret == MPG_SEND_EOS)
        {
            LOG(7, "_SWDMX_MpgSendDataBatchForTotalTime _SWDMX_MpgSendData Finish.\n");
            break;
        }
        else if(i4Ret == MPG_SEND_FAIL)
        {
            LOG(5, "_SWDMX_MpgSendDataBatchForTotalTime _SWDMX_MpgSendData Fail.\n");
            return FALSE;
        }
        else
        {
            LOG(5, "_SWDMX_MpgSendDataBatchForTotalTime _SWDMX_MpgSendData Unknow return.\n");
            return FALSE;
        }

        //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_MpgResetData(u1SrcId);
        //}
    }
    return TRUE;
}

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFillBufferStart(UINT8 u1SrcId)
{
    BOOL fgRet;
  SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
  SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsFillBufferStart\n");

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE; // not parse sequence header
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE; // skip it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE; // skip it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it

    fgRet = _SWDMX_MpgSendDataBatch(u1SrcId);

    return fgRet;
}
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFillBufferEnd(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    LOG(5, "_SWDMX_MpgTsFillBufferEnd\n");

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;

    return TRUE;
}
#endif

/**
*/
BOOL _SWDMX_MpgBaseInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    LOG(5, "_SWDMX_MpgBaseInit(%d)\n", u1SrcId);

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef SWDMX_MPG_PS_GET_INFO
    _SWDMX_MpgCreateFindSema(u1SrcId);
#endif

    UNUSED(FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE,
        _SWDMX_FeederCallBack, (UINT32)&prSwdmxInst->rFeederInfo));

    //DMX_Init();
    //DMX_DDI_Init();
    _SWDMX_MpgResetVariable(u1SrcId);

    #ifdef SWDMX_MPG_USE_INDEX
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx = FALSE;
    #endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart = 4;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoPidIdx = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioPidIdx = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Audio2PidIdx = 0xFF;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PcrPidIdx = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1Audio2KeyIndex = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4VideoDescMode = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4AudioDescMode = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4Audio2DescMode = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekaud = 0;

#if 1//def CC_DLNA_SUPPORT
    // always check, some format feeder buffer is different
    //if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize == 0)
    {
        UINT32 u4FeederBufNum;
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(1, "Feeder input buffer error.\n");
            return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize = (prSwdmxInst->rFeederInfo.u4Size);
        u4FeederBufNum = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize / (MPG_BATCH_SIZE_PVR*MPG_BATCH_READ_SIZE));
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize > 0) &&
           (u4FeederBufNum > MPG_BUF_NUM_RESERVED)) // - 3 * (192*188*10)
        {
            if((u4FeederBufNum - MPG_BUF_NUM_RESERVED) > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum = (u4FeederBufNum - MPG_BUF_NUM_RESERVED);
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum = (u4FeederBufNum);
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum >= 2)
                {
                    // this is for remove buffer from u4BufReadIdx to u4UsingStartAddr
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum--;
                }
            }
        }
        else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize > 0) &&
                 (u4FeederBufNum > 0))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum = (u4FeederBufNum);
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum >= 2)
            {
                // this is for remove buffer from u4BufReadIdx to u4UsingStartAddr
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum--;
            }
        }
        else
        {
            ASSERT(0);
            LOG(1, "feeder buf size 0\n");
        }
        if(eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            // reserve two buffers for dmx pvr internal buffer.
            // one for swdmx_mpg_pvr internal buffer, SWDMX_PVR_MIN_BUFDATA.
            // one for current move data, MPG_BATCH_SIZE_TS_192.
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum >= 3)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum -= 2;
            }
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum > MPG_BUF_NUM_MAX)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum = MPG_BUF_NUM_MAX;
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum != MPG_BUF_NUM)
        {
            LOG(5, "feeder buf size 0x%x, buf num %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum);
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize > 0) &&
           (u4FeederBufNum > 0))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum = (u4FeederBufNum);
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum >= 2)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum--;
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum > MPG_REQ_NUM_MAX)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum = MPG_REQ_NUM_MAX;
            }
        }
        else
        {
            ASSERT(0);
            LOG(1, "feeder buf size 0\n");
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum;
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum != MPG_REQ_NUM)
        {
            LOG(5, "feeder buf size 0x%x, req num %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum);
        }
        #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum < 2)
        {
            LOG(0, "req num %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum);
            ASSERT(0);
        }
        #endif
    }
#endif

#ifdef SWDMX_MPG_DEBUG_FIFO
#ifdef SWDMX_MPG_AFIFO_SIZE
    if((!_fgAFifo) && (_fgAManual))
    {
        _u4AFifoStart = (UINT32)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_MPG_AFIFO_SIZE, 16));
        if(_u4AFifoStart)
        {
            _fgAFifo = TRUE;
        }
        _u4AFifoCur = _u4AFifoStart;
        _u4AFifoEnd = _u4AFifoStart + SWDMX_MPG_AFIFO_SIZE;
    }
#endif
#ifdef SWDMX_MPG_DMXFIFO_SIZE
    if((!_fgDmxFifo) && (_fgDmxManual))
    {
        _u4DmxFifoStart = (UINT32)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_MPG_DMXFIFO_SIZE, 16));
        if(_u4DmxFifoStart)
        {
            _fgDmxFifo = TRUE;
        }
        _u4DmxFifoCur = _u4DmxFifoStart;
        _u4DmxFifoEnd = _u4DmxFifoStart + SWDMX_MPG_DMXFIFO_SIZE;
    }
#endif
#endif

    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgEsInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
#ifndef SWDMX_MPG_DMX_20
    UINT8 u1SrcIdSecond;
    SWDMX_INFO_T* prSwdmxInstSecond;
#endif
#ifndef SWDMX_MPG_DMX_20
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

	LOG(5, "_SWDMX_MpgEsInit(%d)\n", u1SrcId);

    UNUSED(_SWDMX_MpgBaseInit(u1SrcId, eType));

    // Initialize dmx
    #ifndef SWDMX_MPG_DMX_20
    u1SrcIdSecond = (u1SrcId+1)%SWDMX_SOURCE_MAX;
    prSwdmxInstSecond = _SWDMX_GetInst(u1SrcIdSecond);
    UNUSED(prSwdmxInstSecond);

    if (eType == SWDMX_FMT_RAW_AUDIO_ES)
    {
#ifdef CC_DMX_PURE_AUDIO
        {
            fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_PURE_AUDIO);
        }
#else
        {
            fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        }
#endif
        if(!fgRet)
        {
            LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM/PURE_AUDIO\n");
            return fgRet;
        }
		// for audio es file , feeder has changed the buffer to 192kB,so we will change the request size.
		prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize*MPG_BATCH_READ_SIZE/2);

		
    }
    else
    {
        if(!prSwdmxInstSecond->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
            fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_ES);
            if(!fgRet)
            {
                LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_ES\n");
                return fgRet;
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize*MPG_BATCH_READ_SIZE);		
    }
    #endif
    UNUSED(fgRet);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt = eType;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE;

    DMX_SetToDecoder(TRUE);
#ifndef SWDMX_MPG_DMX_20
    // move to enable stream
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);
#endif

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgPsInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet = TRUE;
#ifndef SWDMX_MPG_DMX_20
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgPsInit\n");

    UNUSED(_SWDMX_MpgBaseInit(u1SrcId, eType));

    // Initialize dmx
    #ifndef SWDMX_MPG_DMX_20
    fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_PS);
    if(!fgRet)
    {
        LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_PS\n");
        return fgRet;
    }
    #endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt = eType;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize*MPG_BATCH_READ_SIZE);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE;

    if(eType == SWDMX_FMT_MPEG1_DAT)
    {
        fgRet = DMX_PS_SetDataFormat(prSwdmxInst->u1DmxId, DMX_DATA_FORMAT_DAT);
        if(!fgRet)
        {
            LOG(3, "DMX_PS_SetDataFormat DMX_DATA_FORMAT_RAW\n");
            return fgRet;
        }
    }
    else if(eType == SWDMX_FMT_MPEG2_PS)
    {
        fgRet = DMX_PS_SetDataFormat(prSwdmxInst->u1DmxId, DMX_DATA_FORMAT_RAW);
        if(!fgRet)
        {
            LOG(3, "DMX_PS_SetDataFormat DMX_DATA_FORMAT_DAT\n");
            return fgRet;
        }
    }
    else
    {
        LOG(3, "DMX_PS_SetDataFormat Unknown\n");
        return fgRet;
    }

    DMX_SetToDecoder(TRUE);
#ifndef SWDMX_MPG_DMX_20
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);
#endif

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;

    #ifdef SWDMX_MPG_USE_INDEX
    if (0 == (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize >> 31))
    {
        printf("2GB get the bigger buffer.\r\n");
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx = TRUE;
    }
    #endif

    return TRUE;
}

/**
*/
#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
#ifndef SWDMX_MPG_DMX_20
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsInit\n");

    UNUSED(fgRet);
    UNUSED(_SWDMX_MpgBaseInit(u1SrcId, eType));

#ifdef TIME_SHIFT_SUPPORT
    if(eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(DMX_GetInputType() != DMX_IN_BROADCAST_TS)
        {
            LOG(3, "Time-shift DMX mode not using DMX_IN_BROADCAST_TS\n");
            return FALSE;
        }

        if(!_SWDMX_PVR_Init(u1SrcId))
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_Init Fail\n");
            return FALSE;
        }
        if(!_SWDMX_PVR_OPEN(u1SrcId))
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_OPEN Fail\n");
            return FALSE;
        }

        // Make it single move in the begining
        if(!_SWDMX_PVR_SetDataMode(u1SrcId, 1))
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_SetDataMode fail\n");
        }

        UNUSED(DMX_SetTSMode(prSwdmxInst->u1DmxId, DMX_TSFMT_TIMESHIFT));

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE_PVR;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize*MPG_BATCH_READ_SIZE);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE_PVR;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfRange = FALSE;
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = NULL;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = NULL;
        x_memset(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo, 0, sizeof(MM_REC_BUF_RANGE_INFO_T));
        x_memset(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl, 0, sizeof(PVR_TICK_CTRL_BLK_T));
    }
    else
#endif
    {
#if 0//def TIME_SHIFT_SUPPORT
        if(eType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
        {
            // PVR background record, use DMX_IN_BROADCAST_TS, not DMX_IN_PLAYBACK_TS
            if(DMX_GetInputType() != DMX_IN_BROADCAST_TS)
            {
                LOG(3, "Time-shift DMX mode not using DMX_IN_BROADCAST_TS\n");
                return FALSE;
            }
        }
        else
#endif
        {
            // Initialize dmx
            #ifndef SWDMX_MPG_DMX_20
            fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_TS);
            if(!fgRet)
            {
                LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_TS\n");
                return fgRet;
            }
            #endif
        }

        UNUSED(DMX_SetTSMode(prSwdmxInst->u1DmxId, DMX_TSFMT_188));       // Default

        if((eType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (eType == SWDMX_FMT_MPEG2_TS_192))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE_TS_192;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE_TS_192;
//            UNUSED(DMX_DDI_SetPacketSize(192));
//            UNUSED(DMX_SetPacketSize(192));

            UNUSED(DMX_SetTSMode(prSwdmxInst->u1DmxId, DMX_TSFMT_192));
        }
#ifdef TIME_SHIFT_SUPPORT
        else if(eType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
        {
/*
            if(!_SWDMX_PVR_Init(u1SrcId))
            {
                LOG(3, "%s,%d: _SWDMX_PVR_Init Fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            if(!_SWDMX_PVR_OPEN(u1SrcId))
            {
                LOG(3, "%s,%d: _SWDMX_PVR_OPEN Fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
*/
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE_TS_192;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE_TS_192;

            UNUSED(DMX_SetTSMode(prSwdmxInst->u1DmxId, DMX_TSFMT_192_ENCRYPT));       // Default
        }
#endif
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize*MPG_BATCH_READ_SIZE);
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt = eType;

    DMX_SetToDecoder(TRUE);
#ifndef SWDMX_MPG_DMX_20
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);
#endif

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);

    SWDMX_DmxInit(u1SrcId);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq = NULL;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;

    return TRUE;
}
#endif


/**
*/
BOOL _SWDMX_MpgOpen(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet;
#ifndef SWDMX_MPG_DMX_20
    UINT8 u1SrcIdSecond;
    SWDMX_INFO_T* prSwdmxInstSecond;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prStreamInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgOpen prStreamInfo NULL\n");
        return FALSE;
    }

#ifndef SWDMX_MPG_DMX_20
    u1SrcIdSecond = (u1SrcId+1)%SWDMX_SOURCE_MAX;
    prSwdmxInstSecond = _SWDMX_GetInst(u1SrcIdSecond);
#endif

	LOG(5, "_SWDMX_MpgOpen(%d) eType %d\n", u1SrcId, eType);
    LOG(5, "    u8FileSize in 32 bits = 0x%llx\n", u8FileSize);

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);

    // assume our vdec id is same with swdmx id
    // (same with the one in playback)
    prSwdmxInst->u1VdecId = u1SrcId;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    //_u4AvailSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = u8FileSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;

    if(u8FileSize == 0)
    {
        LOG(1, "_SWDMX_MpgOpen u8FileSize Fail\n");
        return FALSE;
    }

    fgRet = FALSE;
    if((eType == SWDMX_FMT_MPEG1_DAT) ||
        (eType == SWDMX_FMT_MPEG2_PS) ||
        (eType == SWDMX_FMT_MPEG2_TS) ||
        (eType == SWDMX_FMT_MPEG2_TS_192) ||
        (eType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (eType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
        (eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
        (eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (eType == SWDMX_FMT_H264_VIDEO_ES) ||
        (eType == SWDMX_FMT_AVS_VIDEO_ES) ||
        (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (eType == SWDMX_FMT_VC1_ES) ||
        (eType == SWDMX_FMT_RAW_AUDIO_ES))
    {
        if((prStreamInfo->u2VideoNum > 0) && (u4TrackId < prStreamInfo->u2VideoNum))
        {
            if(u4TrackId >= SWDMX_MAX_VID_TRACK)
            {
                LOG(3, "_SWDMX_MpgOpen vid u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_VID_TRACK-1));
                u4TrackId = (SWDMX_MAX_VID_TRACK-1);
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_STRM_TYPE_VID, (UINT32)prStreamInfo->aeVideo[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetInfo V Fail\n");
                return FALSE;
            }

            fgRet = SWDMX_SetStrmID(u1SrcId, (UINT32)prStreamInfo->au2VideoId[u4TrackId],
                0, eSWDMX_STRM_TYPE_VID);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID V Fail\n");
                return FALSE;
            }

            // In playmgr, we assume swdmx source id equals to vdec id
            fgRet = SWDMX_SetInfo(u1SrcId,
                (UINT8)eSWDMX_SET_VDEC_ID,
                (UINT32)u1SrcId,
                0,
                0);
            if (!fgRet)
            {
                LOG(1, "_SWDMX_MpgOpen Fail to set vdec id\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM,
                (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen eSWDMX_ENABLE_STREAM V Fail\n");
                return FALSE;
            }
        }
        if((prStreamInfo->u2AudioNum > 0) && (u4TrackId < prStreamInfo->u2AudioNum)
#ifndef SWDMX_MPG_DMX_20
           &&
           // check if another instance is audio
           // we should not use audio track
           // or free pid index will flush audio, then audio will stop itself.
           (!prSwdmxInstSecond->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
#endif
           )
        {
            if(u4TrackId >= SWDMX_MAX_AUD_TRACK)
            {
                LOG(3, "_SWDMX_MpgOpen aud u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_AUD_TRACK-1));
                u4TrackId = (SWDMX_MAX_AUD_TRACK-1);
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_STRM_TYPE_AUD, (UINT32)prStreamInfo->aeAudio[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetInfo A Fail\n");
                return FALSE;
            }

            fgRet = SWDMX_SetStrmID(u1SrcId, (UINT32)prStreamInfo->au2AudioId[u4TrackId],
                prStreamInfo->au2AudioSubId[u4TrackId], eSWDMX_STRM_TYPE_AUD);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID A Fail\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM,
                (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen eSWDMX_ENABLE_STREAM A Fail\n");
                return FALSE;
            }
        }
        if((prStreamInfo->u2PcrNum > 0) && (u4TrackId < prStreamInfo->u2PcrNum))
        {
            if(u4TrackId >= SWDMX_MAX_PCR_TRACK)
            {
                LOG(3, "_SWDMX_MpgOpen aud u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_PCR_TRACK-1));
                u4TrackId = (SWDMX_MAX_PCR_TRACK-1);
            }

            fgRet = SWDMX_SetStrmID(u1SrcId, (UINT32)prStreamInfo->au2PcrId[u4TrackId],
                0, eSWDMX_STRM_TYPE_PCR);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID PCR Fail\n");
                return FALSE;
            }

            fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_ENABLE_STREAM,
                (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen eSWDMX_ENABLE_STREAM PCR Fail\n");
                return FALSE;
            }
        }
        if((prStreamInfo->u2VideoNum == 0) && (prStreamInfo->u2AudioNum == 0))
        {
            LOG(3, "_SWDMX_MpgOpen No A/V\n");
            return FALSE;
        }
    }
    else
    {
        LOG(3, "_SWDMX_MpgOpen Unknown eType\n");
    }

    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen FALSE\n");
        return FALSE;
    }

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;

    UNUSED(prStreamInfo);
    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgPlay(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust = FALSE;  //should clear the flag of needed adjust current time in no signal status
        return _SWDMX_PVR_Resume(u1SrcId);
    }
#endif

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgPause(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return _SWDMX_PVR_Pause(u1SrcId);
    }
#endif

    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgAbort(UINT8 u1SrcId)
{
    UNUSED(u1SrcId);
    return TRUE;
}


#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2VideoId)
{
    DMX_PS_T rDmxPS;
    UINT32 u4Flags;
    BOOL fgRet;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = (0 + (u1SrcId * 3));
    #endif

    fgRet = FALSE;

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_MpgPsSetVideoId u1PidxVideo != 0xFF\n");
        return FALSE;
    }
    if(!u2VideoId)
    {
        LOG(3, "_SWDMX_MpgPsSetVideoId u2VideoId == 0\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_MpgPsSetVideoId u2VideoId = %d\n",
        u2VideoId);

    //if(u2VideoId)
    {
        u4Flags = (UINT32)(DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_VALID
            | DMX_PID_FLAG_STREAM_ID | DMX_PID_FLAG_INSTANCE_TAG |
            DMX_PID_FLAG_DEVICE_ID);
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.u1StreamID = (UINT8)u2VideoId;
        {
            UINT32 u4Addr, u4Size;
            if(!_SWDMX_GetVideoFifo(u1SrcId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                return FALSE;
            }

            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = u4Addr;
            rDmxPS.u4BufSize          = u4Size;
        }
        rDmxPS.ePidType = DMX_PID_TYPE_ES_VIDEO;
        rDmxPS.fgEnable = TRUE;
        rDmxPS.u1DeviceId = VLD0;
        rDmxPS.u1ChannelId = (prSwdmxInst->u1VdecId);
    	rDmxPS.pvInstanceTag = (void*)prSwdmxInst;                //set instance id to dmx

        fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsSetVideoId DMX_PS_SetStream Fail\n");
            return FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = u1Pidx;
    }
    return TRUE;
}
#endif

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx,
        UINT16 u2AudioId, UINT16 u2AudioSubId)
{
    DMX_PS_T rDmxPS;
    UINT32 u4Flags;
    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = (1 + (u1SrcId * 3));
    #endif
    BOOL fgRet = FALSE;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        LOG(3, "_SWDMX_MpgPsSetAudioId u1PidxAudio != 0xFF\n");
        return FALSE;
    }
    if(!u2AudioId)
    {
        LOG(3, "_SWDMX_MpgPsSetAudioId u2AudioId == 0\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_MpgPsSetAudioId u2AudioId = %d, u2AudioSubId = %d\n",
        u2AudioId, u2AudioSubId);

    //if(u2AudioId)
    {
        // 1. Set stream first
        u4Flags = (UINT32)(DMX_PID_FLAG_BUFFER
            | DMX_PID_FLAG_STREAM_ID | DMX_PID_FLAG_INSTANCE_TAG |
            DMX_PID_FLAG_DEVICE_ID);
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.u1StreamID = (UINT8)u2AudioId;
        {
            UINT32 u4Addr, u4Size;
            if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                return FALSE;
            }

            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = u4Addr;
            rDmxPS.u4BufSize          = u4Size;
        }
        rDmxPS.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rDmxPS.fgEnable = TRUE;

        if(u2AudioSubId)
        {
            rDmxPS.u1SubstreamID = (UINT8)u2AudioSubId;
            rDmxPS.fgEnableSsidFilter = TRUE;
            u4Flags |= DMX_PID_FLAG_SUBSTREAM_ID;
        }

        rDmxPS.pvInstanceTag = (void*)prSwdmxInst;                //set instance id to dmx
        rDmxPS.u1DeviceId = (prSwdmxInst->u1AdecId);
        rDmxPS.u1ChannelId = (prSwdmxInst->u1AdecId);

        fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsSetAudioId DMX_PS_SetStream Fail\n");
            return FALSE;
        }

        // 2. Set stream format
        // use stream id.
        // because aud_mw.c might call DMX_PS_SetAudioType after
        // SetInfo(eSWDMX_SET_AUDIOTYPE)
        if(u2AudioId == 0xBD)
        {
            if(u2AudioSubId >= 0x80 && u2AudioSubId <= 0x88)
            {
                fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_AC3);
            }
            else if((u2AudioSubId & 0xF8) == 0x88)
            {
                fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_DTS);
            }
            else if(u2AudioSubId >= 0xA0 && u2AudioSubId <= 0xA8)
            {
                fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_LPCM);
            }
            else
            {
                LOG(3, "u2AudioId 0xBD u2AudioSubId 0x%x Unknown -> set MPEG\n", u2AudioSubId);
                fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
            }
        }
        else if(u2AudioId >= 0xC0 && u2AudioId <= 0xDF)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else
        {
            LOG(3, "u2AudioId 0x%x Unknown -> set MPEG\n", u2AudioId);
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        /*
        if(prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_AC3)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_AC3);
        }
        else if(prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_PCM)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_LPCM);
        }
        else if(prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_MPEG)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else if(prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_MP3)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else
        {
            LOG(3, "eAudioType Unknown\n");
            fgRet = FALSE;
        }
        */
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsSetAudioId DMX_PS_SetAudioType Fail\n");
            return FALSE;
        }

        // 3. Set stream enable
        u4Flags = (UINT32)(DMX_PID_FLAG_VALID);
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.u1StreamID = (UINT8)u2AudioId;
        {
            UINT32 u4Addr, u4Size;
            if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
            {
                LOG(3, "_SWDMX_GetAudioFifo Fail\n");
                return FALSE;
            }

            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = u4Addr;
            rDmxPS.u4BufSize          = u4Size;
        }
        rDmxPS.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rDmxPS.fgEnable = TRUE;

        if(u2AudioSubId)
        {
            rDmxPS.u1SubstreamID = (UINT8)u2AudioSubId;
            rDmxPS.fgEnableSsidFilter = TRUE;
            u4Flags |= DMX_PID_FLAG_SUBSTREAM_ID;
        }

        fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsSetAudioId DMX_PS_SetStream Fail\n");
            return FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = u1Pidx;
    }
    return TRUE;
}
#endif

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsFreeVideoId(UINT8 u1SrcId)
{
    BOOL fgRet;
    DMX_PS_T rDmxPS;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgPsFreeVideoId(%d)\n", u1SrcId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.fgEnable = FALSE;

        fgRet = DMX_PS_SetStream(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, DMX_PID_FLAG_VALID, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeVideoId DMX_PS_SetStream V Fail\n");
            return FALSE;
        }
        fgRet = DMX_PS_FreeStream(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeVideoId DMX_PS_FreeStream V Fail\n");
            return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgPsFreeVideoId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsFreeAudioId(UINT8 u1SrcId)
{
    BOOL fgRet;
    DMX_PS_T rDmxPS;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgPsFreeAudioId(%d)\n", u1SrcId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.fgEnable = FALSE;

        fgRet = DMX_PS_SetStream(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, DMX_PID_FLAG_VALID, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeAudioId DMX_PS_SetStream A Fail\n");
            //return FALSE;
        }
        fgRet = DMX_PS_FreeStream(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeAudioId DMX_PS_FreeStream A Fail\n");
            //return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgPsFreeAudioId Fail\n");
    return FALSE;
}
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx,
        UINT16 u2VideoId, UINT8 u1KeyIndex, UINT32 u4DescMode, BOOL fgSecondVideo)
{
    //BOOL fgRet;
    SWDMX_DMX_PES_T rPes;
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsSetVideoId(%d) u2VideoId = %d\n", u1SrcId, u2VideoId);

    if(fgSecondVideo)
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 != 0xFF)
        {
            LOG(3, "_SWDMX_MpgTsSetVideoId u1PidxVideo2 != 0xFF\n");
            return FALSE;
        }
    }
    else
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
        {
            LOG(3, "_SWDMX_MpgTsSetVideoId u1PidxVideo != 0xFF\n");
            return FALSE;
        }
    }
    if(!u2VideoId)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId u2VideoId != 0\n");
        return FALSE;
    }

#ifndef CC_TS_H264_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
    {
    }
    else
#endif
#ifndef CC_TS_MVC_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MVC)
    {
    }
    else
#endif
#ifndef CC_TS_MPEG2_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
    }
    else
#endif
#ifndef CC_TS_VC1_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
    }
    else
#endif
#ifndef CC_TS_AVS_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
    {
    }
    else
#endif
    {
        LOG(3, "TsSetVideoId Unknown eVideoType %d\n",
            prSwdmxInst->eVideoType);
        return FALSE;
    }

    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart + (u1SrcId * 3);
    #endif

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoPidIdx != 0xFF)
    {
        u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoPidIdx + (UINT16)fgSecondVideo;
    }

    x_memset(&rPes, 0, sizeof(rPes));
    rPes.u2Pid = u2VideoId;
    rPes.u2DeviceId = (UINT16)(VLD0);
    if(fgSecondVideo)
    {
        rPes.u2ChannelId = prSwdmxInst->u1Vdec2Id;
    }
    else
    {
        rPes.u2ChannelId = prSwdmxInst->u1VdecId;
    }
    rPes.u4BufferSize = TS_PES_VIDEO_FIFO_SIZE;
#ifdef TIME_SHIFT_SUPPORT
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
    {
        rPes.u1TsIndex = DMX_PVRPLAY_TS_INDEX;
        rPes.u1KeyIndex = u1KeyIndex;
        rPes.u4DescMode = u4DescMode;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPes.pfnScramble = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.pfnVScrambleNfy;
    }
#endif

    eRet = SWDMX_DmxPesAllocateBuffer(u1SrcId, &rPes, &u1Pidx, DMX_PID_TYPE_ES_VIDEO);

	
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId SWDMX_DmxPesAllocateBuffer V Fail\n");
        return FALSE;
    }
    eRet = SWDMX_DmxPesStart(u1Pidx);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId SWDMX_DmxPesStart V Fail\n");
        return FALSE;
    }

    if(fgSecondVideo)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 = u1Pidx;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = u1Pidx;
    }

    return TRUE;
}
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx,
            UINT16 u2AudioId, UINT16 u2DeviceId,
            UINT8 u1KeyIndex, UINT32 u4DescMode)
{
    //BOOL fgRet;
    SWDMX_DMX_PES_T rPes;
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsSetAudioId(%d) u2AudioId = %d\n", u1SrcId, u2AudioId);

    if((u2DeviceId == 0) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF))
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u1PidxAudio != 0xFF\n");
        return FALSE;
    }
    /* not support now
    if((u2DeviceId == 1) && (_u1PidxAudio2 != 0xFF))
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u1PidxAudio != 0xFF\n");
        return FALSE;
    }
    */
    if(!u2AudioId)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u2AudioId != 0\n");
        return FALSE;
    }
    if(u2DeviceId > INTERNAL_DAC_ID_4TH)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u2DeviceId %d > %d\n", u2DeviceId, INTERNAL_DAC_ID_4TH);
        return FALSE;
    }

    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = (UINT8)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart + 1) + u2DeviceId
        + (u1SrcId * 3));
    #endif

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioPidIdx != 0xFF)
    {
        if(u2DeviceId == 0)
        {
            u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioPidIdx;
        }
        /* not support now
        else if(u2DeviceId == 1)
        {
            u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Audio2PidIdx;
        }
        */
        else
        {
            LOG(0, "_SWDMX_MpgTsSetAudioId u2DeviceId(%d) Fail\n", u2DeviceId);
            return FALSE;
        }
    }

    x_memset(&rPes, 0, sizeof(rPes));
    rPes.u2Pid = u2AudioId;
    rPes.u2DeviceId = (u2DeviceId + prSwdmxInst->u1AdecId);
    rPes.u4BufferSize = TS_PES_AUDIO_FIFO_SIZE;
#ifdef TIME_SHIFT_SUPPORT
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
    {
        rPes.u1TsIndex = DMX_PVRPLAY_TS_INDEX;
        rPes.u1KeyIndex = u1KeyIndex;
        rPes.u4DescMode = u4DescMode;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPes.pfnScramble = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.pfnAScrambleNfy;
    }
#endif

    eRet = SWDMX_DmxPesAllocateBuffer(u1SrcId, &rPes, &u1Pidx, DMX_PID_TYPE_ES_AUDIO);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId SWDMX_DmxPesAllocateBuffer A Fail\n");
        return FALSE;
    }

    // Temp solution, wait for MW's new information.
    if(prSwdmxInst->eAudioType == ENUM_SWDMX_ADEC_PCM)
    {
        BOOL fgRet;
        fgRet = DMX_TS_SetAudioType(u1Pidx, DMX_AUDIO_LPCM);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgTsSetAudioId DMX_TS_CPU_SetAudioType Fail\n");
            return FALSE;
        }
    }

    eRet = SWDMX_DmxPesStart(u1Pidx);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId SWDMX_DmxPesStart A Fail\n");
        return FALSE;
    }
    if(u2DeviceId == 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = u1Pidx;
    }
    /* not support now
    else if(u2DeviceId == 1)
    {
        _u1PidxAudio2 = u1Pidx;
    }
    */
    else
    {
        LOG(0, "_SWDMX_MpgTsSetAudioId u2DeviceId(%d) Fail\n", u2DeviceId);
        return FALSE;
    }

    return TRUE;
}
#endif


#ifndef CC_TS_DISABLE
#ifdef SWDMX_MPG_SET_PCR_ID
BOOL _SWDMX_MpgTsSetPCRId(UINT8 u1SrcId, UINT16 u2PCRId)
{
    SWDMX_DMX_PES_T rPes;
    UINT8 u1Pidx;
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s(%d) u2PCRId = %d\n", __FUNCTION__, u1SrcId, u2PCRId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR != 0xFF)
    {
        LOG(3, "%s u1PidxPCR != 0xFF\n", __FUNCTION__);
        return FALSE;
    }

    u1Pidx = 0;

    x_memset(&rPes, 0, sizeof(rPes));
    rPes.u2Pid = u2PCRId;
    rPes.u2DeviceId = 0;
    rPes.u4BufferSize = 0;
#ifdef TIME_SHIFT_SUPPORT
    rPes.u1TsIndex = DMX_PVRPLAY_TS_INDEX;
#endif
    eRet = SWDMX_DmxPesAllocateBuffer(u1SrcId, &rPes, &u1Pidx, DMX_PID_TYPE_NONE);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId SWDMX_DmxPesAllocateBuffer V Fail\n");
        return FALSE;
    }
    eRet = SWDMX_DmxPesStart(u1Pidx);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId SWDMX_DmxPesStart V Fail\n");
        return FALSE;
    }

    if(!DMX_SetPcr(u1Pidx, TRUE, FALSE))
    {
        return FALSE;
    }

    if(DMX_SetStcSrc(u1Pidx, 0) != 0)
    {
        return FALSE;
    }

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR = u1Pidx;

    return TRUE;
}
#endif
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFreeVideoId(UINT8 u1SrcId, BOOL fgSecondVideo)
{
    UINT8 u1Pidx;
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsFreeVideoId(%d)\n", u1SrcId);

    if(fgSecondVideo)
    {
        u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2;
    }
    else
    {
        u1Pidx = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo;
    }
    if(u1Pidx != 0xFF)
    {
        eRet = SWDMX_DmxPesStop(u1Pidx);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeVideoId SWDMX_DmxPesStop V Fail\n");
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(u1SrcId, u1Pidx);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeVideoId SWDMX_DmxPesReleaseBuffer V Fail\n");
            //return FALSE;
        }
        if(fgSecondVideo)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 = 0xFF;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = 0xFF;
        }
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgTsFreeVideoId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFreeAudioId(UINT8 u1SrcId)
{
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgTsFreeAudioId(%d)\n", u1SrcId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        eRet = SWDMX_DmxPesStop(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeAudioId SWDMX_DmxPesStop A Fail\n");
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeAudioId SWDMX_DmxPesReleaseBuffer A Fail\n");
            //return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgTsFreeAudioId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_TS_DISABLE
#ifdef SWDMX_MPG_SET_PCR_ID
BOOL _SWDMX_MpgTsFreePCRId(UINT8 u1SrcId)
{
    SWDMX_ERR_CODE_T eRet;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s\n", __FUNCTION__);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR != 0xFF)
    {
        if(!DMX_SetPcr(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR, TRUE, FALSE))
        {
            return FALSE;
        }

        eRet = SWDMX_DmxPesStop(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "%s SWDMX_DmxPesStop Fail\n", __FUNCTION__);
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "%s SWDMX_DmxPesReleaseBuffer A Fail\n", __FUNCTION__);
            //return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR = 0xFF;
        return TRUE;
    }
    LOG(3, "%s Fail\n", __FUNCTION__);
    return FALSE;
}
#endif
#endif


#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetVideoId(UINT8 u1SrcId, UINT8 u1Pidx)
{
    DMX_ES_T rDmxES;
    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = (0 + (u1SrcId * 3));
    #endif
    UINT32 u4Flags;
    BOOL fgRet;
    UINT32 u4Addr, u4Size;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

	LOG(5, "_SWDMX_MpgEsSetVideoId(%d)\n", u1SrcId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_MpgEsSetVideoId u1PidxVideo != 0xFF\n");
        return FALSE;
    }

#ifndef CC_ES_H264_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_H264)
    {
    }
    else
#endif
#ifndef CC_ES_MPEG2_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
    }
    else
#endif
#ifndef CC_ES_MPEG4_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
    }
    else
#endif
#ifndef CC_ES_VC1_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
    }
    else
#endif
#ifndef CC_ES_AVS_DISABLE
    if(prSwdmxInst->eVideoType == ENUM_SWDMX_VDEC_AVS)
    {
    }
    else
#endif
    {
        LOG(3, "EsSetVideoId Unknown eVideoType %d\n",
            prSwdmxInst->eVideoType);
        return FALSE;
    }

    //set es pid for playback

    u4Flags = (UINT32)(DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_VALID
                | DMX_PID_FLAG_INSTANCE_TAG | DMX_PID_FLAG_DEVICE_ID);
    x_memset(&rDmxES, 0, sizeof(rDmxES));

    if(!_SWDMX_GetVideoFifo(u1SrcId, &u4Addr, &u4Size))
    {
        LOG(3, "_SWDMX_GetAudioFifo Fail\n");
        return FALSE;
    }

    rDmxES.fgAllocateBuffer   = FALSE;
    rDmxES.u4BufAddr          = u4Addr;
    rDmxES.u4BufSize          = u4Size;

    rDmxES.u1DeviceId         = (VLD0);
    rDmxES.u1ChannelId        = (prSwdmxInst->u1VdecId);
    rDmxES.fgEnable = TRUE;
    rDmxES.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rDmxES.pvInstanceTag = (void*)prSwdmxInst;                //set instance id to dmx

    fgRet = DMX_ES_SetPid(u1Pidx, u4Flags, &rDmxES);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgEsSetVideoId DMX_ES_SetPid Fail\n");
        return FALSE;
    }
    //set es filesize
    fgRet = DMX_ES_SetFileLength(u1Pidx,(UINT32)0xFFFFFFFF);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgEsSetVideoId _DMX_ES_SetFileLength Fail\n");
        return FALSE;
    }
    _SWDMX_MpgResetData(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = u1Pidx;

    return TRUE;
}
#endif


#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetAudioId(UINT8 u1SrcId, UINT8 u1Pidx)
{
    DMX_MM_T rDmxMMInfo;
    #ifndef SWDMX_MPG_DMX_20
    u1Pidx = (1 + (u1SrcId * 3));
    #endif
    UINT32 u4Flags;
    BOOL fgRet;
    UINT32 u4Addr, u4Size;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

	LOG(5, "_SWDMX_MpgEsSetAudioId(%d), eFeederSrc = %d\n",
	    prSwdmxInst->eFeederSrc);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        LOG(3, "_SWDMX_MpgEsSetAudioId u1PidxVideo != 0xFF\n");
        return FALSE;
    }

#if !defined(CC_ES_MP3_DISABLE) && !defined(CC_ES_MP2_DISABLE)
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
    }
    else
#endif
    {
        LOG(3, "EsSetAudioId Unknown eFmt %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt);
        return FALSE;
    }

    u4Flags = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
      DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG |
      DMX_MM_FLAG_DEVICE_ID);

    x_memset(&rDmxMMInfo, 0, sizeof(rDmxMMInfo));
    rDmxMMInfo.fgEnable = TRUE;

#ifdef CC_DMX_PURE_AUDIO
    {
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PURE_AUDIO;
    }
#else
    {
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
    }
#endif

    rDmxMMInfo.fgAllocBuf = FALSE;

    if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
    {
        LOG(3, "_SWDMX_MpgEsSetAudioId _SWDMX_GetAudioFifo Fail\n");
        return FALSE;
    }

    rDmxMMInfo.u4BufAddr = u4Addr;
    rDmxMMInfo.u4BufSize = u4Size;
    rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;                //set instance id to dmx
    rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
    rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;

    fgRet = DMX_MM_Set(u1Pidx, u4Flags, &rDmxMMInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgEsSetAudioId DMX_MM_Set Fail\n");
        return FALSE;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = u1Pidx;

    return TRUE;
}
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsFreeVideoId(UINT8 u1SrcId)
{
    BOOL fgRet;
    DMX_ES_T rDmxES;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
   	LOG(5, "_SWDMX_MpgEsFreeVideoId(%d)\n", u1SrcId);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxES, 0, sizeof(rDmxES));
        rDmxES.fgEnable = FALSE;
        fgRet = DMX_ES_SetPid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo, DMX_PID_FLAG_VALID, &rDmxES);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeVideoId DMX_ES_SetPid V Fail\n");
            //return FALSE;
        }
        fgRet = DMX_ES_FreePid(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeVideoId SWDMX_DmxPesReleaseBuffer V Fail\n");
            //return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgEsFreeVideoId Fail\n");
    return FALSE;
}
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsFreeAudioId(UINT8 u1SrcId)
{
    BOOL fgRet;
    DMX_MM_T rDmxMMInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    LOG(5, "_SWDMX_MpgEsFreeAudioId\n");

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio != 0xFF)
    {
        x_memset(&rDmxMMInfo, 0, sizeof(rDmxMMInfo));
        rDmxMMInfo.fgEnable = FALSE;
        fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio, DMX_PID_FLAG_VALID, &rDmxMMInfo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeAudioId DMX_MM_Set Fail\n");
            //return FALSE;
        }
        fgRet = DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeAudioId DMX_MM_Free Fail\n");
            //return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgEsFreeAudioId Fail\n");
    return FALSE;
}
#endif


/**
 To get first and last PTS
*/
BOOL _SWDMX_MpgBaseGetInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet = TRUE;
    UINT64 u8SendSize;
    UINT64 u8Tmp, u8Remainder;
    UINT32 u4LastPTS, u4FirstPTS;
    UINT32 u4PicCnt;
    UINT32 u4FindLastPtsTimes = 2;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prStreamInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgBaseGetInfo prStreamInfo NULL\n");
        return FALSE;
    }

    u8SendSize = 0;
    u4FirstPTS = 0;
    u4LastPTS = 0;

    LOG(5, "_SWDMX_MpgBaseGetInfo(%d)\n", u1SrcId);

    fgRet = _SWDMX_MpgOpen(u1SrcId, eType, 0, u8FileSize, prStreamInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgOpen Fail.\n");
        return FALSE;
    }

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE; // not parse sequence header
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE; // skip send eos it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it

    // first send
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = u8FileSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > FIRST_PTS_DETECT_SIZE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = FIRST_PTS_DETECT_SIZE;
    }
    ASSERT(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize < (UINT64)0xFFFFFFFF);
    u8SendSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;


    fgRet = _SWDMX_MpgSendDataBatchForTotalTime(u1SrcId);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgSendDataBatch First Fail.\n");
    }
    LOG(5, "First(%d): u4FirstPTS %d, u4PicCnt %d, u8SendSize 0x%llx\n",
        u1SrcId,
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS,
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt, u8SendSize);
    u4FirstPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
    u4LastPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS;
    u4PicCnt = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt;
    // ~First

    // Second
    while(u4FindLastPtsTimes < LAST_PTS_DETECT_TIME)
    {
        _SWDMX_MpgResetData(u1SrcId);
        _SWDMX_MpgResetVariable(u1SrcId);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE; // skip send eos it
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = u8FileSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize -=
            (LAST_PTS_DETECT_SIZE*(u4FindLastPtsTimes-2));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > (LAST_PTS_DETECT_SIZE*u4FindLastPtsTimes))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - (LAST_PTS_DETECT_SIZE*u4FindLastPtsTimes));
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;

        ASSERT((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize) < (UINT64)0xFFFFFFFF);
        u8SendSize += (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);

        fgRet = _SWDMX_MpgSendDataBatchForTotalTime(u1SrcId);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgSendDataBatch Second Fail.\n");
        }
        LOG(5, "Second(%d): Times %d, u4LastPTS %d, u4PicCnt %d, u8SendSize 0x%llx\n",
            u1SrcId,
            u4FindLastPtsTimes,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt, u8SendSize);
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS)
        {
            u4LastPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS;
            u4PicCnt += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PicCnt;
            break;
        }
        u4FindLastPtsTimes++;
        if((eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
           (eType == SWDMX_FMT_H264_VIDEO_ES) ||
           (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
           (eType == SWDMX_FMT_AVS_VIDEO_ES) ||
           (eType == SWDMX_FMT_VC1_ES))
        {
            // one shot for es
            break;
        }
    }
    if(u4FindLastPtsTimes >= LAST_PTS_DETECT_TIME)
    {
        u4LastPTS = 0;
        // calculate totaltime
    }
    // ~Second

    // Calculate TotalTime
    if((u4FirstPTS == 0) || (u4LastPTS == 0) ||
        (eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (eType == SWDMX_FMT_H264_VIDEO_ES) ||
        (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (eType == SWDMX_FMT_AVS_VIDEO_ES) ||
        (eType == SWDMX_FMT_VC1_ES))
    {
        if((u4PicCnt > 0) && (u8SendSize > 0))
        {
            u8Tmp = u8FileSize;
            u8Tmp *= u4PicCnt;
            // ToDo: To have frame rate, 30 frame rate
            //u8Tmp = _SWDMX_Div6464(u8Tmp, (UINT64)(u4SendSize*30), &u8Remainder);
            //u8Tmp *= 90000; // milli-seconds -> 90K unit
            u8Tmp = _SWDMX_Div6464(u8Tmp, (u8SendSize), &u8Remainder);
            u8Tmp *= (UINT64)3000; // milli-seconds -> 90K unit
            //ASSERT(u8Tmp < (UINT64)0xFFFFFFFF);
            if(u8Tmp > (UINT64)0xFFFFFFFF)
            {
                u8Tmp = (UINT64)0xFFFFFFFF;
            }
            prStreamInfo->u4TotalTime = (UINT32)u8Tmp;
        }
        else
        {
            prStreamInfo->u4TotalTime = 0;
        }
    }
    else
    {
        if(u4FirstPTS > u4LastPTS)
        {
            prStreamInfo->u4TotalTime = ((0xFFFFFFFF - u4FirstPTS) + u4LastPTS);

        }
        else
        {
            prStreamInfo->u4TotalTime = (u4LastPTS - u4FirstPTS);
        }
        if(prStreamInfo->u4TotalTime > 0)
        {
            u8Tmp = _SWDMX_Div6464(u8FileSize*90000, (UINT64)prStreamInfo->u4TotalTime, &u8Remainder);
            if(u8Tmp > (25*1024*1024)) // 200 Mbps
            {
                // this kind of data rate is impossilbe, total time must be wrong
                prStreamInfo->u4TotalTime = 0;
            }
        }
        prStreamInfo->u4FirstPts = u4FirstPTS;
        // Use 90K time unit now
        // To milli-seconds
        //prStreamInfo->rVar.u4TotalTime *= 1000;
        //prStreamInfo->rVar.u4TotalTime /= 90000;
    }
    // ~Calc

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif

    fgRet = _SWDMX_MpgCloseGetInfo(u1SrcId);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgCloseGetInfo Fail.\n");
        return FALSE;
    }
    UNUSED(u4InfoMask);
    return TRUE;
}


/**
 To get sequence header
*/
BOOL _SWDMX_MpgGetSeqInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet = TRUE;
    INT32 i4Ret;
    UINT32 i;
    UINT32 j;
    SWDMX_CUR_CMD_T rCmd;
    VDEC_SEQUENCE_DATA_T *prVdecSeqData;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgGetSeqInfo prStreamInfo NULL\n");
        return FALSE;
    }
    if (u1SrcId >= SWDMX_SOURCE_MAX)
    {
        LOG(1, "_SWDMX_MpgGetSeqInfo (u1SrcId >= SWDMX_SOURCE_MAX)\n");
        return FALSE;
    }

	LOG(5, "%s(%d)\n", __FUNCTION__, u1SrcId);

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    x_memset(&rCmd, 0, sizeof(SWDMX_CUR_CMD_T));
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif

    for(i=0;i<prPInfo->u4PgmNum;i++)
    {
        if(i >= SWDMX_MAX_PROGRAM)
        {
            LOG(3, "_SWDMX_MpgGetSeqInfo SWDMX_MAX_PROGRAM Fail.\n");
            break;
        }
        for(j=0;j<prPInfo->rPgm[i].u2VideoNum;j++)
        {
            //x_memset(_ucMpgSeqHdr, 0, MPG_MPEG2_SEQ_HDR_SIZE);
            prVdecSeqData = _arSeqInfo;
            prVdecSeqData += (UINT32)u1SrcId;
            x_memset((void*)prVdecSeqData, 0, sizeof(VDEC_SEQUENCE_DATA_T));
            if(j >= SWDMX_MAX_VID_TRACK)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo SWDMX_MAX_VID_TRACK Fail.\n");
                break;
            }
            //_eVideo = prPInfo->rPgm[i].aeVideo[j];
            fgRet = _SWDMX_MpgOpen(u1SrcId, eType, j, u8FileSize, &(prPInfo->rPgm[i]));
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgOpen Fail.\n");
                continue;
            }

            _SWDMX_MpgResetData(u1SrcId);
            _SWDMX_MpgResetVariable(u1SrcId);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE; // skip it
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it


            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = u8FileSize;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > SEQ_DETECT_SIZE)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = SEQ_DETECT_SIZE;
            }

            // send
            fgRet = TRUE;
            while(1)
            {
                i4Ret = _SWDMX_MpgSendData(u1SrcId, NULL, &rCmd);
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr)
                {
                    break;
                }
                if(i4Ret == MPG_SEND_OK)
                {
                    continue;
                }
                else if(i4Ret == MPG_SEND_FULL)
                {
                    x_thread_delay(1);
                }
                else if(i4Ret == MPG_SEND_EOS)
                {
                    LOG(7, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgSendData Finish.\n");
                    break;
                }
                else if(i4Ret == MPG_SEND_FAIL)
                {
                    LOG(5, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgSendData Fail.\n");
                    fgRet = FALSE;
                    break;
                }
                else
                {
                    LOG(5, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgSendData Unknow return.\n");
                    fgRet = FALSE;
                    break;
                }
            }
            // ~send

            // Parsing
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr)
            {
                prPInfo->rPgm[i].u4Width[j] = _arSeqInfo[u1SrcId].u2_width;
                prPInfo->rPgm[i].u4Height[j] = _arSeqInfo[u1SrcId].u2_height;
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif
            // ~Parsing

            fgRet = _SWDMX_MpgCloseGetInfo(u1SrcId);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgCloseGetInfo Fail.\n");
                //return FALSE;
            }
        }
    }

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif

    UNUSED(u4InfoMask);

    return TRUE;
}

/**
*/
#ifndef CC_PS_DISABLE
#ifdef SWDMX_MPG_PS_GET_INFO
BOOL _SWDMX_MpgPsGetStreamInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    DMX_PS_T rDmxPS;
    UINT16 u2MsgQIdx;
    UINT32 u4FindFin;
    SIZE_T zMsgSize;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    BOOL fgRet;
    UINT64 u8Tmp, u8Remainder;
    UINT32 u4Tmp;
	  SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgPsGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

    zMsgSize = sizeof(INT32);
    u1Pidx = 0;
    fgRet = FALSE;

    LOG(5, "_SWDMX_MpgPsGetStreamInfo\n");

    _SWDMX_MpgPsInit(u1SrcId, eType);

	_SWDMX_FlushFindMsgQ(u1SrcId);

    prPInfo->u4PgmNum = 0;

    u4Flags = (DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_STREAM_ID);
    x_memset(&rDmxPS, 0, sizeof(rDmxPS));
    {
        UINT32 u4Addr, u4Size;
        if(!_SWDMX_GetVideoFifo(u1SrcId, &u4Addr, &u4Size))
        {
            LOG(3, "_SWDMX_MpgEsSetAudioId _SWDMX_GetAudioFifo Fail\n");
            return FALSE;
        }

        rDmxPS.fgAllocateBuffer   = FALSE;
        rDmxPS.u4BufAddr          = u4Addr;
        rDmxPS.u4BufSize          = u4Size;
    }
    rDmxPS.ePidType = DMX_PID_TYPE_ES_VIDEO;

    fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
    if(!fgRet)
    {
        Printf("DMX_PS_SetStream Fail\n");
        return FALSE;
    }

    u4Flags = DMX_PID_FLAG_VALID;
    x_memset(&rDmxPS, 0, sizeof(rDmxPS));
    rDmxPS.fgEnable = TRUE;

    fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
    if(!fgRet)
    {
        Printf("DMX_PS_SetStream Fail\n");
        return FALSE;
    }

    // demux have limitation. I have to set smaller size than I will send
    u8Tmp = _SWDMX_Div6464(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize, (UINT64)(2048), &u8Remainder);
    UNUSED(_SWDMX_Div6464(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize, (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase), &u8Remainder));
    u4Tmp = u8Remainder;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase*2))
    {
        fgRet = DMX_PS_DetectStreamID(TRUE,
            (u8Tmp),
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - (u4Tmp) - (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase*2)));
    }
    else
    {
        u8Tmp = _SWDMX_Div6464(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize, (UINT64)(2048), &u8Remainder);
        fgRet = DMX_PS_DetectStreamID(TRUE,
            (u8Tmp),//(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize/2048),
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - (u4Tmp)));
    }

    if(!fgRet)
    {
        Printf("DMX_PS_DetectStreamID Fail\n");
        DMX_PS_FreeStream(u1Pidx);
        return FALSE;
    }

    u4Flags = DMX_PID_FLAG_CALLBACK;
    x_memset(&rDmxPS, 0, sizeof(rDmxPS));
    rDmxPS.pfnNotify = _SWDMX_MpgPsInfoNotify;
    rDmxPS.pvNotifyTag = 0;

    fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
    if(!fgRet)
    {
        Printf("DMX_PS_SetStream Fail\n");
        DMX_PS_FreeStream(u1Pidx);
        return FALSE;
    }

	_SWDMX_FlushDataMsgQ(u1SrcId);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    //_u4AvailSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE; // skip it
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it

    _SWDMX_MpgSendDataBatch(u1SrcId);

    #if 0
    //detect PS
    //dmx callback to swdmx tell streamid or pid for a/v
    VERIFY(x_timer_start(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer, OPEN_TIMEOUT, X_TIMER_FLAG_ONCE,
        _SWDMX_TimerFindTimeout, (VOID*)0) == OSR_OK);
    #endif
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4FindFin, &zMsgSize, &(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindMsgQ), 1,
    X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.hFindTimer) == OSR_OK);

    u4Flags = DMX_PID_FLAG_VALID;
    x_memset(&rDmxPS, 0, sizeof(rDmxPS));
    rDmxPS.fgEnable = FALSE;

    fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
    if(!fgRet)
    {
        Printf("DMX_PS_SetStream Fail\n");
        return FALSE;
    }

    fgRet = DMX_PS_FreeStream(u1Pidx);
    if(!fgRet)
    {
        Printf("DMX_PS_FreeStream Fail\n");
        return FALSE;
    }

    if((u4FindFin != DETECT_OK) ||
        ((_ucPSVideoStreamId == 0)&&(_ucPSAudioStreamId == 0)))
    {
        LOG(1, "PS open fail, findfin = %d, VideoStreamId = %d, AudioStreamId = %d",
            u4FindFin, (INT32)_ucPSVideoStreamId, (INT32)_ucPSAudioStreamId);
        return FALSE;
    }

    prPInfo->u4PgmNum = 1;
    if(_ucPSVideoStreamId)
    {
        prPInfo->rPgm[0].fgVideo = TRUE;
        prPInfo->rPgm[0].au2VideoId = _ucPSVideoStreamId;
        prPInfo->rPgm[0].aeVideo = ENUM_SWDMX_VDEC_MPEG1_MPEG2;
    }
    if(_ucPSAudioStreamId)
    {
        prPInfo->rPgm[0].fgStreamInfo = TRUE;
        prPInfo->rPgm[0].fgAudio = TRUE;
        prPInfo->rPgm[0].u2AudioId = _ucPSAudioStreamId;
        if(_ucPSAudioStreamId >= 0xC0 && _ucPSAudioStreamId <=0xDF)
        {
            prPInfo->rPgm[0].aeAudio = ENUM_SWDMX_ADEC_MPEG;
        }
        else if((_ucPSAudioStreamId == 0xBD) &&
            (_ucPSAudioSubStreamId >= 0xA0) && (_ucPSAudioSubStreamId <= 0xA8))
        {
            prPInfo->rPgm[0].aeAudio = ENUM_SWDMX_ADEC_PCM;
        }
        else if((_ucPSAudioStreamId == 0xBD) &&
            (_ucPSAudioSubStreamId >= 0x80) && (_ucPSAudioSubStreamId <= 0x88))
        {
            prPInfo->rPgm[0].aeAudio = ENUM_SWDMX_ADEC_AC3;
        }
        else
        {
            LOG(3, "prStreamInfo->aeAudio Unknown\n");
            prPInfo->rPgm[0].aeAudio = ENUM_SWDMX_ADEC_UNKNOWN;
        }
    }

    // in PS pre-parsing mode, we have to reset dmx to prevent infinite loop
    DMX_Init();
    _SWDMX_MpgPsInit(u1SrcId, eType);

    if(_ucPSVideoStreamId || _ucPSAudioStreamId)
    {
        // to get total duration
        return _SWDMX_MpgBaseGetInfo(u1SrcId, eType, prPInfo->rPgm[0]);
    }

    UNUSED(u4InfoMask);

    return FALSE;
}
#endif
#endif

/**
*/
#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsGetStreamInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    UINT32 u4Src, u4Num;
    UINT32 u4Timeout;
    UINT32 u4AudPid;
    UINT32 u4VidPid;
    //UINT32 u4VidPidSub;
    UINT32 u4APIDType;
    UINT32 u4VPIDType;
    UINT32 u4SubVPIDType;
    INT32 i4Ret;
    BOOL fgRet;
    UINT32 i, j;

	  SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgTsGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_GetInfo(u1SrcId, prPInfo))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
        /*
        // to get total duration
        fgRet = _SWDMX_MpgOpen(eType, 0, u8FileSize, prStreamInfo);
        //fgRet = _SWDMX_MpgBaseGetInfo(eType, prStreamInfo);
        if(fgRet)
        {
            LOG(5, "_SWDMX_MpgTsOpen return TRUE\n");
            return TRUE;
        }
        LOG(3, "_SWDMX_MpgTsOpen return FALSE\n");
        return FALSE;
        */
    }
#endif

    LOG(5, "_SWDMX_MpgTsGetStreamInfo\n");

    if(!_SWDMX_MpgTsInit(u1SrcId, eType))
    {
        LOG(1, "_SWDMX_MpgTsGetStreamInfo _SWDMX_MpgTsInit fail\n");
        return FALSE;
    }

    x_memset(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl, 0, sizeof(PSIPSR_TBL_PMT_T)*SWDMX_MAX_PMT_NUM);
    u4Src = 0;
    u4Num = 0;
    u4Timeout = OPEN_TIMEOUT; // timeout

    {
        prPInfo->u4PgmNum = 0;
        //detect TS
        i4Ret = SWDMX_PsiPsrParsePsi(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl, &u4Num, u4Timeout, u4Src);

        if (i4Ret==(INT32)E_PSIPSR_OK)
        {
            LOG(3, "PSI parsing done---------------------\n");
            if(u4Num >= SWDMX_MAX_PMT_NUM)
            {
                LOG(3, "u4Num %d > default %d\n", u4Num, SWDMX_MAX_PMT_NUM);
                //ASSERT(u4Num < SWDMX_MAX_PMT_NUM);
                u4Num = SWDMX_MAX_PMT_NUM;
            }
            for (i=0; i<u4Num; i++)
            {
                prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum = 0;
                prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum = 0;
                prPInfo->rPgm[prPInfo->u4PgmNum].u2SubpicNum = 0;
                prPInfo->rPgm[prPInfo->u4PgmNum].u2PcrNum = 0;
                LOG(5, "prog=0x%x, pcr=0x%x, v=0x%x, 0x%x(%d,%d), v2=0x%x(%d), a1=0x%x(%d), a2=0x%x(%d), a3=0x%x(%d)\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4ProgNum, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4PCR,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4SubVPID,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPIDType,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4SubVPIDType,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID2, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID2Type,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1Type,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID2, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID2Type,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID3, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID3Type);

                for (j=0; j<2 ;j++)
                {
                    u4VidPid = 0;
                    u4SubVPIDType = 0;
                    if((j == 0) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID > 0))
                    {
                        u4VidPid = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID;
                        //u4VidPidSub = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPIDSub;
                        u4VPIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPIDType;
                        u4SubVPIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4SubVPIDType;
                    }
                    else if((j == 1) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID2 > 0))
                    {
                        u4VidPid = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID2;
                        //u4VidPidSub = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPIDSub;
                        u4VPIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID2Type;
                        // only support on first video track
                        //u4SubVPIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4SubVPIDType;
                    }
                    if(u4VidPid > 0)
                    {
                        if(prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum < SWDMX_MAX_VID_TRACK)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].au2VideoId[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] =
                                (UINT16)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4VPID;
                            prPInfo->rPgm[prPInfo->u4PgmNum].au2SubVideoId[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] =
                                (UINT16)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4SubVPID;
                            x_memcpy((VOID*)&prPInfo->rPgm[prPInfo->u4PgmNum].aau1VideoHdr[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum][0],
                                (VOID*)&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].au1VData[0],
                                SWDMX_MAX_HDR_SIZE);
                            if(u4VPIDType == STREAM_TYPE_VIDEO)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_MPEG1_MPEG2;
                            }
                            else if(u4VPIDType == STREAM_TYPE_H264)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_H264;
                            }
                            else if(u4VPIDType == STREAM_TYPE_AVS)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_AVS;
                            }
                            else if(u4VPIDType == STREAM_TYPE_VC1)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_VC1;
                            }
                            else
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_UNKNOWN;
                                LOG(3, "_SWDMX_MpgTsOpen Unknown Video Type\n");
                            }
                            if(u4SubVPIDType == STREAM_TYPE_MVC)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeSubVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_MVC;
                                x_memcpy(
                                    (VOID*)&prPInfo->rPgm[prPInfo->u4PgmNum].aau1VideoHdr[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum][0],
                                    (VOID*)&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].au1VData[0],
                                    SWDMX_MVC_HDR_SIZE);
                            }
                            else if(u4SubVPIDType == STREAM_TYPE_MULTITRACK)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeSubVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum]
                                    = ENUM_SWDMX_VDEC_MULTITRACK;
                            }
                            if(prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum]!=ENUM_SWDMX_VDEC_UNKNOWN)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum++;
                            }
                        }
                        else
                        {
                            LOG(3, "_SWDMX_MpgTsOpen video num full\n");
                        }
                    }
                }
                for (j=0; j<3 ;j++)
                {
                    u4AudPid = 0;
                    if((j == 0) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1 > 0))
                    {
                        u4AudPid = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1;
                        u4APIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1Type;
                    }
                    else if((j == 1) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID2 > 0))
                    {
                        u4AudPid = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID2;
                        u4APIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID2Type;
                    }
                    else if((j == 2) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID3 > 0))
                    {
                        u4AudPid = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID3;
                        u4APIDType = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID3Type;
                    }
                    if(u4AudPid > 0)
                    {
                        if(prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum < SWDMX_MAX_AUD_TRACK)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].au2AudioId[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = (UINT16)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.arPMTTbl[i].u4APID1;
                            if((u4APIDType == STREAM_TYPE_MPEG1) ||
                                (u4APIDType == STREAM_TYPE_MPEG2))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_MPEG;
                            }
                            else if((u4APIDType == STREAM_TYPE_AC3) ||
                                (u4APIDType == STREAM_TYPE_AC3_EU) ||
                                (u4APIDType == STREAM_TYPE_EAC3))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_AC3;
                            }
                            else if((u4APIDType == STREAM_TYPE_AAC) ||
                                (u4APIDType == STREAM_TYPE_AAC_ISDB))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_AAC;
                            }
                            //Sample rate, frequence, etc.
                            else if(u4APIDType ==
                                    STREAM_TYPE_SWDMX_LPCM)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_PCM;
                            }
                            else
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_UNKNOWN;
                                LOG(3, "_SWDMX_MpgTsOpen Unknown Audio Type\n");
                            }
                            if(prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum]!=ENUM_SWDMX_ADEC_UNKNOWN)
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum++;
                            }
                        }
                        else
                        {
                            LOG(3, "_SWDMX_MpgTsOpen audio num full\n");
                        }
                    }
                }
                if((prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum > 0) || (prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum > 0))
                {
                    // to get total duration
                    fgRet = _SWDMX_MpgBaseGetInfo(u1SrcId, eType,
                                    u4InfoMask,
                                    u8FileSize,
                                    &(prPInfo->rPgm[prPInfo->u4PgmNum]));
                    if(!fgRet)
                    {
                        LOG(3, "_SWDMX_MpgTsOpen _SWDMX_MpgBaseGetInfo return FALSE\n");
                        continue;
                    }
                    LOG(5, "_SWDMX_MpgTsOpen _SWDMX_MpgBaseGetInfo return TRUE\n");

                    prPInfo->u4PgmNum++;
                }
            }
        }
        else
        {
            LOG(1, "PSI parse timeout\n");
            return FALSE;
        }

        LOG(5, "TS, prog cnt %d\n", u4Num);
        if(u4Num == 0)
        {
            LOG(3, "TS, No program Pid\n");
            return FALSE;
        }
    }

    //LOG(3, "_SWDMX_MpgTsOpen return FALSE\n");
    return TRUE;
}
#endif

/**
*/
#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsGetStreamInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet;

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgEsGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

	LOG(5, "%s(%d)\n", __FUNCTION__, u1SrcId);

    if(!_SWDMX_MpgEsInit(u1SrcId, eType))
    {
        LOG(1, "_SWDMX_MpgEsGetStreamInfo _SWDMX_MpgEsInit fail\n");
        return FALSE;
    }

    prPInfo->u4PgmNum = 0;
#ifndef CC_ES_H264_DISABLE
    if(eType == SWDMX_FMT_H264_VIDEO_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_H264;
    }
    else
#endif
#ifndef CC_ES_MPEG2_DISABLE
    if(eType == SWDMX_FMT_MPEG2_VIDEO_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_MPEG1_MPEG2;
    }
    else
#endif
#ifndef CC_ES_MPEG4_DISABLE
    if(eType == SWDMX_FMT_MPEG4_VIDEO_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_MPEG4;
    }
    else
#endif
#ifndef CC_ES_VC1_DISABLE
    if(eType == SWDMX_FMT_VC1_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_VC1;
    }
    else
#endif
#ifndef CC_ES_AVS_DISABLE
    if(eType == SWDMX_FMT_AVS_VIDEO_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_AVS;
    }
    else
#endif
    {
        LOG(3, "_SWDMX_MpgEsGetStreamInfo Unknown eType %d\n", eType);
        return FALSE;
    }
    prPInfo->u4PgmNum = 1;
    prPInfo->rPgm[0].u2VideoNum = 1;
    prPInfo->rPgm[0].u2AudioNum = 0;
    prPInfo->rPgm[0].u2SubpicNum = 0;
    prPInfo->rPgm[0].u2PcrNum = 0;
    prPInfo->rPgm[0].au2VideoId[0] = 0xE0;

    // to get total duration
    fgRet = _SWDMX_MpgBaseGetInfo(u1SrcId, eType,
                        u4InfoMask,
                        u8FileSize,
                        &(prPInfo->rPgm[0]));
    if(fgRet)
    {
        LOG(5, "_SWDMX_MpgEsGetStreamInfo return TRUE\n");
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgEsGetStreamInfo return FALSE\n");
    return FALSE;
}
#endif


/**
*/
BOOL _SWDMX_MpgGetStreamInfo(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet;
	  SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_MpgGetStreamInfo eType %d\n", eType);
    LOG(5, "    u8FileSize in 32 bits= 0x%llx\n", u8FileSize);

    if(u8FileSize == 0)
    {
        LOG(5, "_SWDMX_MpgGetStreamInfo u8FileSize Fail\n");
        return FALSE;
    }

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = u8FileSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;


    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_TRACK)
    {
        fgRet = FALSE;
#ifdef SWDMX_MPG_PS_GET_INFO
        if((eType == SWDMX_FMT_MPEG2_PS) ||
            (eType == SWDMX_FMT_MPEG1_DAT))
        {
            if(u8FileSize > PS_OPEN_FILE_SIZE)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = PS_OPEN_FILE_SIZE;
            }
            fgRet = _SWDMX_MpgPsGetStreamInfo(u1SrcId, eType, u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
#ifndef CC_TS_DISABLE
#ifdef TIME_SHIFT_SUPPORT
        if(eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            fgRet =  _SWDMX_MpgTsGetStreamInfo(u1SrcId, eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
        if((eType == SWDMX_FMT_MPEG2_TS) ||
            (eType == SWDMX_FMT_MPEG2_TS_192) ||
            (eType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (eType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
        {
            if(u8FileSize > TS_OPEN_FILE_SIZE)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = TS_OPEN_FILE_SIZE;
            }
            fgRet =  _SWDMX_MpgTsGetStreamInfo(u1SrcId, eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
#ifndef CC_ES_DISABLE
        if((eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (eType == SWDMX_FMT_H264_VIDEO_ES) ||
            (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (eType == SWDMX_FMT_AVS_VIDEO_ES) ||
            (eType == SWDMX_FMT_VC1_ES))
        {
            if(u8FileSize > ES_OPEN_FILE_SIZE)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = ES_OPEN_FILE_SIZE;
            }
            fgRet =  _SWDMX_MpgEsGetStreamInfo(u1SrcId, eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
        {
            LOG(3, "_SWDMX_MpgGetStreamInfo Unknown eType\n");
        }
        if(!fgRet)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo = FALSE;
            LOG(3, "_SWDMX_MpgGetStreamInfo TRACK FALSE\n");
            return FALSE;
        }
    }
    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_RESOLUTION)
    {
        fgRet =  _SWDMX_MpgGetSeqInfo(u1SrcId, eType, u4InfoMask, u8FileSize, prPInfo);
        if(!fgRet)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo = FALSE;
            LOG(3, "_SWDMX_MpgGetStreamInfo RESOLUTION FALSE\n");
            return FALSE;
        }
    }

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo = FALSE;

    LOG(5, "_SWDMX_MpgGetStreamInfo(%d) Result:\n", u1SrcId);
    LOG(5, "    u2VideoNum     = %d\n", prPInfo->rPgm[0].u2VideoNum);
    LOG(5, "    u2AudioNum     = %d\n", prPInfo->rPgm[0].u2AudioNum);
    LOG(5, "    u2VideoId[0]   = %d\n", prPInfo->rPgm[0].au2VideoId[0]);
    LOG(5, "    u2AudioId[0]   = %d\n", prPInfo->rPgm[0].au2AudioId[0]);
    LOG(5, "    u4TotalTime    = %d\n", prPInfo->rPgm[0].u4TotalTime);
    LOG(5, "    u8FileSize in 32 bits = 0x%llx\n", u8FileSize);
    LOG(5, "    eVideo[0]      = %d\n", prPInfo->rPgm[0].aeVideo[0]);
    LOG(5, "    eAudio[0]      = %d\n", prPInfo->rPgm[0].aeAudio[0]);

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgCloseGetInfo(UINT8 u1SrcId)
{
    BOOL fgRet;
    LOG(5, "_SWDMX_MpgCloseForOpen(%d)\n", u1SrcId);
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM,
        (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen eSWDMX_DISABLE_STREAM V Fail\n");
    }
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM,
        (UINT32)eSWDMX_STRM_TYPE_AUD, 0, 0);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen eSWDMX_DISABLE_STREAM A Fail\n");
    }
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM,
        (UINT32)eSWDMX_STRM_TYPE_AUD2, 0, 0);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen eSWDMX_DISABLE_STREAM A2 Fail\n");
    }
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM,
        (UINT32)eSWDMX_STRM_TYPE_SUBTITLE, 0, 0);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen eSWDMX_DISABLE_STREAM SUB Fail\n");
    }
    fgRet = SWDMX_SetInfo(u1SrcId, (UINT8)eSWDMX_DISABLE_STREAM,
        (UINT32)eSWDMX_STRM_TYPE_PCR, 0, 0);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgOpen eSWDMX_DISABLE_STREAM PCR Fail\n");
    }
    UNUSED(fgRet);
    return _SWDMX_MpgStop(u1SrcId);
}

/**
*/
BOOL _SWDMX_MpgClose(UINT8 u1SrcId)
{
    LOG(5, "_SWDMX_MpgClose\n");

    if(!_SWDMX_MpgStop(u1SrcId))
    {
        return FALSE;
    }

    return TRUE;
}


/**
*/
VOID _SWDMX_MpgSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    MM_RANGE_INFO_T* prRangeInfo = 0;
     SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    LOG(5, "_SWDMX_MpgSetRange(%d)\n", u1SrcId);

    if(prRangeList == NULL)
    {
        LOG(0, "Demuxer Range is NULL\n");
        return;
    }

    prRangeInfo = prRangeList->prDmxRangeInfo;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq = prRangeList;

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(//(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
           //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
           //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) /*&&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)*/
          )
        {
            //MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                //prTblEntry = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if(/*(prTblEntry) && */(prIdxTbl))
                {
                    if((prIdxTbl->ui4_number_of_entry > 0)
                        && prIdxTbl->u.pt_pb_idx_tbl_entry)
                    {
                        // with index, we don't need to check alignment
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = 1;
                    }
                    else if(prIdxTbl->ui4_align > 0)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = prIdxTbl->ui4_align;
                    }
                    else if(prIdxTbl->ui4_align == 0)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = 1;
                    }
                }
            }
        }
    }

    if (prRangeInfo != NULL)
    {
        _SWDMX_MpgResetData(u1SrcId);
        _SWDMX_MpgResetVariable(u1SrcId);

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = (prRangeInfo->z_range_sz + prRangeInfo->ui8_fileoffset);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = prRangeInfo->ui8_fileoffset;
        // for resume stop.
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
        {
            LOG(3, "u8StartFileOffset >= u8FileSize\n");
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > 0)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - 1);
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
            }
        }
        if(prSwdmxInst)
        {
            if((prSwdmxInst->fgPlayChange) &&
               (((UINT32)(prSwdmxInst->u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
                || ((UINT32)(prSwdmxInst->u8SeekPos) != (UINT32)0xFFFFFFFF)))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->u8SeekPos;
                prSwdmxInst->u8SeekPos = (UINT64)0xFFFFFFFF;
                prSwdmxInst->u8SeekPos <<= 32;
                prSwdmxInst->u8SeekPos += (UINT64)0xFFFFFFFF;
                prSwdmxInst->fgPlayChange = FALSE;
                UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1VdecId, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1VdecId, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                if((prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MVC) ||
                   (prSwdmxInst->eSubVideoType == ENUM_SWDMX_VDEC_MULTITRACK))
                {
                    UNUSED(VDEC_SetRenderFromPos(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderBackStep(prSwdmxInst->u1Vdec2Id, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(prSwdmxInst->u1Vdec2Id, FALSE, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize));
                }
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prRangeInfo->ui8_fileoffset;
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgSetRange prSwdmxInst null\n");
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->e_mm_src_type;
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU) ||
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_PUSH))
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
        }

#ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(prRangeInfo->pt_idxtbl_lst)
            {
                if(prRangeInfo->pt_idxtbl_lst->u.pt_ts_idx_tbl_entry)
                {
                    x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl,
                        &prRangeInfo->pt_idxtbl_lst->u.pt_ts_idx_tbl_entry->t_tick_ctrl_blk,
                        sizeof(PVR_TICK_CTRL_BLK_T));

                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address = (VOID*)VIRTUAL((UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address);
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address = (VOID*)VIRTUAL((UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address);

                    if((UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size != (UINT32)sizeof(SWDMX_MPG_PVR_TICK_INDEX_T))
                    {
                        LOG(0, "ui4_entry_size(%d) != sizeof(SWDMX_MPG_PVR_TICK_INDEX_T)(%d)\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size,
                            sizeof(SWDMX_MPG_PVR_TICK_INDEX_T));
                    }

                    if((UINT32)((UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -
                                (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address) !=
                        (UINT32)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num) *
                                 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size))
                    {
                        LOG(0, "pv_end_address(0x%x) - pv_start_address(0x%x) != ui4_entry_num(%d) * ui4_entry_size(%d)\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size);
                    }

                }
                else
                {
                    LOG(3, "pt_ts_idx_tbl_entry null\n");
                }
            }
            else
            {
                LOG(3, "pt_idxtbl_lst null\n");
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize = 0x0FFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize <<= 32;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize += 0xFFFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize =
                (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_lba_init *
                 (UINT64)MPG_LBA_BLOCK_SIZE ;

            {
                UINT64 u8Temp;
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num > 0)
                {
                    UINT64 u8Remainder;
                    u8Temp = _SWDMX_Div6464(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
                        (UINT32)((UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num *
                                 (UINT64)MPG_LBA_BLOCK_SIZE),
                        &u8Remainder);
                    u8Temp = u8Remainder;
                }
                else
                {
                    LOG(3, "ui4_fifo_pkt_num = 0\n");
                    u8Temp = 0;
                }

                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgFirst)
                {
                    LOG(5, "SetRange File Pos 0x%llx\n",
                        (u8Temp + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset));
                }
                UNUSED(u8Temp);
            }

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize +=
                 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset;
        }
#endif
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
        //_u4AvailSize = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        // move up
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime = (UINT32)prRangeInfo->ui8_pb_duration;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTimeOffset = 0;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS = ((UINT32)prRangeInfo->t_pts_offset);
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        LOG(5, "_SWDMX_MpgSetRange u8FileSize 0x%llx, u8SendedSize 0x%llx, u4TotalTime %d, u4FirstPTS %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
            (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime,
            (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS);

        // DTV00211081 [MM Compatibility]Video FB for a moment after FF then play with special file
        // For small file with 10 second up duration.
        // divide send data size to a small one.
        // Or when we send one time we will get 20 frames.
        // We can't identify the same one when we resend data.
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize >= MPG_BATCH_SIZE) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize < (1024*1024)) &&
           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > (90000*10)) ||
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime == 0)))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize /= (UINT32)MPG_BATCH_TIME_SMALL_FILE;
            LOG(3, "_SWDMX_MpgSetRange use small send size\n");
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize >= MPG_BATCH_SIZE) &&
           ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_RHAPSODY) ||
            ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_CMPB) &&
             (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo))))
        {
            //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize /= (UINT32)MPG_BATCH_TIME_RHAPSODY;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize /= (UINT32)MPG_BATCH_TIME_RHAPSODY;
            LOG(3, "_SWDMX_MpgSetRange use RHAPSODY send size\n");
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type ==
            MM_SRC_TYPE_NETWORK_CMPB) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_sz_max_acces_unt > 0))
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize > 0) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize >
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_sz_max_acces_unt))
            {
                UINT32 u4Tmp;
                u4Tmp
                    = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_sz_max_acces_unt;
                u4Tmp /= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize;
                u4Tmp = (u4Tmp == 0) ? 1 : u4Tmp;
                u4Tmp *= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = u4Tmp;
                LOG(3, "u4OneTimeReadSize(%d) use ui4_sz_max_acces_unt(%d)\n",
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_sz_max_acces_unt);
            }
        }
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize >
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize;
        }
        else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE) < 
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = 
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE);
        }

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase > 1)
                && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase < prRangeInfo->z_range_sz))
            {
                UINT32 u4Tmp = 0;
                u4Tmp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize;
                u4Tmp /= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
                if(u4Tmp == 0)
                {
                    LOG(1, "line:%d, the alignment size (0x%x) is too large!!\n", __LINE__,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase);
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase * u4Tmp;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE;
                }
            }
        }

        //_prRangeList = prRangeList;
        /*while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4AudStrmID)
            {
                //_prAudIdxTbl = pBuldIdxTbl;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO &&
                pBuldIdxTbl->t_strm_id.u.t_stm_avi_id == u4VidStrmID)
            {
                //_prVidIdxTbl = pBuldIdxTbl;
            }
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }*/

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        prRangeList->u4CurPTS =
            (UINT32)prRangeList->prDmxRangeInfo->t_vid_start_render_pts;

#ifdef PVR_START_OFFSET
        if(prRangeList->u4CurPTS > 0 && 
            IS_PVR(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt))
        {
            LOG(5, "start play time from %d\n", prRangeList->u4CurPTS/90000);
            _fgPvrStartOffset = TRUE;
        }
#endif

        #ifdef SWDMX_MPG_USE_INDEX
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx)
        {
            if(!_SWDMX_MpgIdxOpen(u1SrcId,
                (UINT32)(SWDMX_MPG_IDX_TYPE_POSITION | SWDMX_MPG_IDX_TYPE_TIME),
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize))
            {
                LOG(1, "_SWDMX_MpgIdxOpen fail\n");
            }
        }
        #endif
    }
    else
    {
        LOG(0, "Demuxer Range is NULL\n");
    }

    UNUSED(u4VidStrmID);
    UNUSED(u4AudStrmID);
    UNUSED(u4SubTitleID);
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_MpgHeaderParse(
    UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    INT32 i4Ret;
    //LOG(9, "_SWDMX_MpgHeaderParse\n");

    i4Ret = _SWDMX_MpgSendData(u1SrcId, prRangeList, prCmd);
    if(i4Ret == MPG_SEND_OK)
    {
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    else if(i4Ret == MPG_SEND_FULL)
    {
        x_thread_delay(10);
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
    else if(i4Ret == MPG_SEND_EOS)
    {
        x_thread_delay(1);
        LOG(7, "_SWDMX_MpgHeaderParse _SWDMX_MpgSendData Finish.\n");
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    else if(i4Ret == MPG_SEND_FAIL)
    {
        x_thread_delay(1);
        LOG(5, "_SWDMX_MpgHeaderParse _SWDMX_MpgSendData Fail.\n");
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
	else if(i4Ret == MPG_SEND_SKIP)
    {
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
    else
    {
        x_thread_delay(1);
        LOG(3, "_SWDMX_MpgHeaderParse i4Ret %d unknow.\n", i4Ret);
    }

    LOG(5, "_SWDMX_MpgHeaderParse _SWDMX_MpgSendData Unknow return.\n");
    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
}


/**
*/
BOOL _SWDMX_MpgStop(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
	LOG(5, "_SWDMX_MpgStop(%d)\n", u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef TIME_SHIFT_SUPPORT
    UNUSED(DMX_PVRPlay_SetUseDescramble(FALSE));
#endif

#ifdef TIME_SHIFT_SUPPORT
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_Stop(u1SrcId))
        {
            LOG(3, "_SWDMX_MpgStop _SWDMX_PVR_Stop fail\n");
        }

        if(!_SWDMX_PVR_Close(u1SrcId))
        {
            return FALSE;
        }
    }
#endif

    _SWDMX_MpgResetData(u1SrcId);
    _SWDMX_MpgResetVariable(u1SrcId);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed = 1000;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE; // not parse sequence header
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekvid = FALSE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSeekaud = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = 0;

    #ifdef SWDMX_MPG_USE_INDEX
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgUseIdx)
    {
        UNUSED(_SWDMX_MpgIdxClose(u1SrcId));
    }
    #endif

    UNUSED(VDEC_SetGetSeqHdrOnlyEnd(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff));
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgFlush(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    LOG(5, "_SWDMX_MpgFlush(%d)\n", u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0) && (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos)
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
            }
            _SWDMX_MpgModifyPlayPos(u1SrcId);
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos;        
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;   
        }
    }        
    _SWDMX_MpgResetData(u1SrcId);
    return TRUE;
}

BOOL _SWDMX_MpgSetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTickIndex;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

#ifdef TIME_SHIFT_SUPPORT
    if(u1Type == (UINT8)eSWDMX_SET_REC_BUF_RANGE)
    {
	    LOG(7, "_SWDMX_MpgSetInfo(%d) u1Type = %d, u4Para1 = %d\n",
	        u1SrcId, u1Type, u4Para1);
    }
    else
#endif
    {
	    LOG(5, "_SWDMX_MpgSetInfo(%d) u1Type = %d, u4Para1 = %d\n",
	        u1SrcId, u1Type, u4Para1);
    }

    if((u1Type == (UINT8)eSWDMX_SET_VIDEOTYPE) ||
       (u1Type == (UINT8)eSWDMX_SET_VIDEO2TYPE) ||
       (u1Type == (UINT8)eSWDMX_SET_SUBVIDEOTYPE))
    {
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUDIOTYPE)
    {
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_VIDEO_DECRYPT)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = (UINT16)u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4VideoDescMode = (UINT8)u4Para2;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUDIO_DECRYPT)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = (UINT16)u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4AudioDescMode = (UINT8)u4Para2;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_VID_PID_IDX)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoPidIdx = (UINT16)u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = (UINT8)u4Para2;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4VideoDescMode = u4Para3;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUD_PID_IDX)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioPidIdx = (UINT16)u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = (UINT8)u4Para2;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4AudioDescMode = u4Para3;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_PCR_PID_IDX)
    {
        //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PcrPidIdx = (UINT16)u4Para1;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE)
    {
        #ifndef SWDMX_MPG_USE_MTMPLAYER_PVR
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize = ((u4Para1 > 0) ? TRUE : FALSE);
        #endif
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_PID_INDEX_START)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart = (UINT16)(u4Para1);
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_VID_SCRAMBLE_NFY)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.pfnVScrambleNfy = (VOID*)(u4Para1);
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUD_SCRAMBLE_NFY)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.pfnAScrambleNfy = (VOID*)(u4Para1);
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_SYSTEM_B)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSystemB = (BOOL)(u4Para1);
        return TRUE;
    }
#ifdef TIME_SHIFT_SUPPORT
    else if(u1Type == (UINT8)eSWDMX_SET_REC_BUF_RANGE)
    {
        if(u4Para1 == 0)
        {
            LOG(3, "_SWDMX_MpgSetInfo eSWDMX_SET_REC_BUF_RANGE u4Para1 null\n");
            return FALSE;
        }

        _SWDMX_LockRangeSema(u1SrcId);

        x_memcpy(&prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo,
            (MM_REC_BUF_RANGE_INFO_T*)u4Para1, sizeof(MM_REC_BUF_RANGE_INFO_T));

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase == 0)
        {
            LOG(1, "_SWDMX_MpgSetInfo ui4_tick_period 0\n");
            //ASSERT(0);
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        // start addr
        prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
        if(prStartAddr == NULL)
        {
            LOG(3, "_SWDMX_MpgSetInfo pv_start_address null\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        // end addr
        // from the explain of mw pv_end_address is the end address of the tick_index_talbe;
        prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
        if(prEndAddr == NULL)
        {
            LOG(3, "_SWDMX_MpgSetInfo pv_end_address null\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        //  0  u8FifoOffset (u8FifoStart)   lba_init (Start lba)    u8FifoSize
        // (Max file size = u8FifoSize + u8FifoOffset)
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize =
            (((UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num)
            *(UINT64)MPG_LBA_BLOCK_SIZE);

        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            LOG(3, "_SWDMX_MpgSetInfo u8FifoSize 0\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        // safe start
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start);
        if((prTickIndex == NULL) || (prTickIndex > prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo safe start prTickIndex fail\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr =
            _SWDMX_MpgLBA2Pos(u1SrcId, ((prTickIndex)->ui4_lba));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick =
            prTickIndex->ui4_tick_num;

        // safe end
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
        if((prTickIndex == NULL) || (prTickIndex > prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo safe end prTickIndex fail\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr =
            _SWDMX_MpgLBA2Pos(u1SrcId, ((prTickIndex)->ui4_lba));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick =
            prTickIndex->ui4_tick_num;

        // valid start
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
        if((prTickIndex == NULL) || (prTickIndex > prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo valid start prTickIndex fail\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr =
            _SWDMX_MpgLBA2Pos(u1SrcId, ((prTickIndex)->ui4_lba));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick =
            prTickIndex->ui4_tick_num;

        // valid end
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
        if((prTickIndex == NULL) || (prTickIndex > prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo valid end prTickIndex fail\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr =
            _SWDMX_MpgLBA2Pos(u1SrcId, ((prTickIndex)->ui4_lba));
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick =
            prTickIndex->ui4_tick_num;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange = TRUE;

        if(prSwdmxInst)
        {
            if(prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PAUSE)
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos) <
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr))
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
                       (!prSwdmxInst->fgPlayChange) && // not changing speed
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                    {
                        LOG(3, "UpdateRange fgMonitorSafeStart addr 0x%llx safe start addr 0x%llx\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr);
                        if((prSwdmxInst) &&
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                        {
                            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                            {
                                _SWDMX_RANGE_CALLBACK(
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                    MM_OUT_OF_SAFE_RANGE_START,
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                    0xFFFFFFFF,
                                    (UINT32)u1SrcId);
                            }
                        }
                    }
                }

                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr)
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
                       (!prSwdmxInst->fgPlayChange) && // not changing speed
                       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                    {
                        LOG(3, "UpdateRange fgMonitorValidStart addr 0x%llx valid start addr 0x%llx\n",
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr);
                        if((prSwdmxInst) &&
                           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
                        {
                            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                            {
                                _SWDMX_RANGE_CALLBACK(
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                    MM_REACH_VALID_RANGE_START,
                                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                    0xFFFFFFFF,
                                    (UINT32)u1SrcId);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgSetInfo prSwdmxInst null\n");
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }

        _SWDMX_UnlockRangeSema(u1SrcId);

        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_PRINT_REC_BUF_STATUS)
    {
        _SWDMX_LockRangeSema(u1SrcId);
        LOG(0, "Monitor: Safe start(%d) end(%d), Valid start(%d) end(%d)\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd);
        LOG(0, "Range Addr: 0x%llx 0x%llx 0x%llx 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidStartAddr,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeStartAddr,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SafeEndAddr,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ValidEndAddr);
        LOG(0, "Range num (0x%x-0x%x): %d %d %d %d\n",
            (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address,
            (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
        LOG(0, "Range Tick (%d): %d %d %d %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick);
        LOG(0, "Fifo: Start 0x%llx, Size 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoSize);
        _SWDMX_UnlockRangeSema(u1SrcId);
    }
    else if(u1Type == (UINT8)eSWDMX_SET_MONITOF_LBA)
    {
        if(u4Para1 == (UINT32)0xFFFFFFFF)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr = 0;
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr =
                _SWDMX_MpgLBA2Pos(u1SrcId, (u4Para1));
        }
        return TRUE;
    }
#endif
    else if(u1Type == (UINT8)eSWDMX_PRINT_INTERNAL_STATUS)
    {
#ifdef TIME_SHIFT_SUPPORT
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            VDP_PIC_INFO_T rVdpPicInfo;
            UCHAR ucVdpId = 0; // prSwdmxInst->u1B2rId
            UINT32 u4TickAudio = 0;
            UINT32 u4TickVideo = 0;
            x_memset(&rVdpPicInfo, 0, sizeof(VDP_PIC_INFO_T));
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
               (VDP_GetPicInfo(ucVdpId, &rVdpPicInfo) == VDP_SET_OK))
            {
                u4TickVideo = rVdpPicInfo.u4TickNum;
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
            {
                UNUSED(AUD_MMFindTickInfo(prSwdmxInst->u1AdecId, &u4TickAudio));
            }

            LOG(0, "CurTickNum: Video(%d) Audio(%d) Swdmx(%d)\n",
                u4TickVideo, u4TickAudio, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
        }
#endif
        LOG(0, "Cur: u8SendedSize(0x%llx) u8ReadedPos(0x%llx)\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos);
        LOG(0, "rBuf: u4BufTotalSize(%d) u4ReqTotalSize(%d)\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
        #ifdef SWDMX_MPG_DMX_20
            LOG(0, "AvailableAFifo(%d) AvailableVFifo(%d)\n",
                DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                    DMX_PID_TYPE_ES_AUDIO,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio),
                DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                    DMX_PID_TYPE_ES_VIDEO,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo));
        #else
#ifdef CC_DMX_PURE_AUDIO
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            LOG(0, "AvailableAFifo(%d) AvailableVFifo(%d)\n",
                DMX_MM_GetBufEmptySize(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio),
                0);
       	}
        else
#endif
        {
            LOG(0, "AvailableAFifo(%d) AvailableVFifo(%d)\n",
                DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO),
                DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO));
        }
        #endif
        LOG(0, "Speed: %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed);
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AOUT_ENABLE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
        LOG(5, "Call AUD_MMAoutEnable from VDP\n");
    }

    UNUSED(u4Para2);
    UNUSED(u4Para3);
    return FALSE;
}


BOOL _SWDMX_MpgGetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    UINT64 u8Offset, u8OffsetI;
    UINT16 u2DecodingOrder;
    /*UINT16 u2QueueSize, u2MaxQueueSize;*/
    INT32 i4TempRef;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    LOG(6, "_SWDMX_MpgGetInfo u1Type = %d\n", u1Type);

    if(u1Type == (UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_GET_CURRENT_TIME)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }

        {
            UINT64 u8Tmp;
            UINT64 u8CurPos;
            UINT64 u8Remainder;

            *pu4Para1 = 0;

            u8CurPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                u8Offset = 0;
                u8OffsetI = 0;
                u2DecodingOrder = 0;
                i4TempRef = 0;
                if(VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
                {
                    if(u8Offset > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
                    {
                        LOG(7, "_SWDMX_MpgGetInfo VDP_GetFilePosition u8Offset %d > u8FileSize %d\n",
                            (UINT32)u8Offset, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
                        u8CurPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                    }
                }
            }
            else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
            {
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                   &&
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                   // do not need to consider changing
                   // because we could get our swdmx current position
                   // that's the correct one.
                   //(prSwdmxInst) &&
                   //(!prSwdmxInst->fgPlayChange) && // not changing speed
                {
                    u8CurPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                    // seek end
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                    {
                        u8CurPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                    }
                }
                else
                {
                    UINT32 u4DmxAvailSizeAudio = 0;
                    UINT32 u4Addr, u4Size;
                    UINT32 u4BufSize;
                    UINT32 u4DataSize;
                    #ifdef SWDMX_MPG_DMX_20
                    u4DmxAvailSizeAudio = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                        DMX_PID_TYPE_ES_AUDIO,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                    #else
#ifdef CC_DMX_PURE_AUDIO
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
                    }
                    else
#endif
                    {
                        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
                    #endif

                    if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
                    {
                        LOG(3, "_SWDMX_MpgEsSetAudioId _SWDMX_GetAudioFifo Fail\n");
                        return FALSE;
                    }

                    u4BufSize = u4Size;
                    u4DataSize = u4BufSize - u4DmxAvailSizeAudio;
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > u4DataSize)
                    {
                        u8CurPos = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - u4DataSize);
                    }
                    else
                    {
                        u8CurPos = 0;
                    }
                    #if 0
                    LOG(0, "---GetInfo %d %d %d (%d-%d)\n",
                        (UINT32)*pu8Tmp, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u4DataSize,
                        u4BufSize, u4DmxAvailSizeAudio);
                    #endif
                }
            }

            if ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize > 0))
            {
                //90000->90000
                u8Tmp = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*(UINT64)90000,
                                  (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime ,
                                  &u8Remainder);
                if(u8Tmp > 0)
                {
                    if(u8CurPos > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)
                    {
                        *pu4Para1 = (UINT32)_SWDMX_Div6464((u8CurPos-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*(UINT64)90000,
                                                   u8Tmp, &u8Remainder);
                    }
                    else
                    {
                        *pu4Para1 = 0;
                    }
                }
                UNUSED(u8Remainder);
            }
        }

        if(//(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                prTblEntry = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if((prTblEntry) && (prIdxTbl))
                {
                    //*pu4Para1 = prTblEntry[prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_pts;
                    *pu4Para1 = STC_GetStcValue(prSwdmxInst->u1StcId);
                }
                else
                {
                    LOG(3, "_SWDMX_MpgGetInfo prTblEntry NULL\n");
                }
            }
        }

        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)(VOID*)pu4Para1;

        *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
        {
			VDEC_HDR_INFO_T rHdrInfo;

            u8Offset = 0;
            u8OffsetI = 0;
            u2DecodingOrder = 0;
            i4TempRef = 0;
			
			//if( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo)
            UNUSED(VDEC_QueryInfo(prSwdmxInst->u1VdecId, &rHdrInfo)); //DTV00088421
            if(rHdrInfo.u4DisplayQPicCnt > 0)			
			{
              if(VDP_GetFilePosition(prSwdmxInst->u1B2rId, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
              {
                if(u8Offset > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize)
                {
                    LOG(7, "_SWDMX_MpgGetInfo VDP_GetFilePosition u8Offset 0x%llx > u8FileSize 0x%llx\n",
                        u8Offset, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize);
                    *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize;
                }
                else
                {
                    VDP_PIC_INFO_T rVdpPicInfo;
                    x_memset(&rVdpPicInfo, 0, sizeof(VDP_PIC_INFO_T));
                    if (VDP_GetPicInfo(prSwdmxInst->u1B2rId, &rVdpPicInfo) == VDP_SET_OK)
                    {
                        *pu8Tmp = rVdpPicInfo.u8OffsetDisp;
                    }
                }
              }
			  else
			  {
				  *pu8Tmp=0;
			  }			  
			}
			else    //video has not play
			{
				*pu8Tmp=0;
			}
        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
        {
            if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
               &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
               // do not need to consider changing
               // because we could get our swdmx current position
               // that's the correct one.
               //(prSwdmxInst) &&
               //(!prSwdmxInst->fgPlayChange) && // not changing speed
            {
                *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                // seek end
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    *pu8Tmp = 0;
                }
            }
            else
            {
                DMX_MM_T rDMXInfo;
                UINT32 u4DataSize = 0;
                UINT32 u4DmxAvailSizeAudio = 0;
                x_memset(&rDMXInfo, 0, sizeof(DMX_MM_T));
                if(DMX_MM_Get(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio,
                    DMX_MM_FLAG_BUF, &rDMXInfo))
                {
                    u4DataSize = (rDMXInfo.u4Wp < rDMXInfo.u4Rp) ?
                        (rDMXInfo.u4BufSize + rDMXInfo.u4Wp - rDMXInfo.u4Rp) : (rDMXInfo.u4Wp - rDMXInfo.u4Rp);
                }
                else
                {
                    return FALSE;
                }
                if(u4DataSize > u4DmxAvailSizeAudio)
                {
                    u4DataSize -= u4DmxAvailSizeAudio;
                }
                else
                {
                    u4DataSize = 0; 
                }
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > u4DataSize)
                {
                    *pu8Tmp = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - u4DataSize);
                }
                else
                {
                    *pu8Tmp = 0;
                }
                #if 1
                LOG(5, "---GetInfo %d %d %d %d\n",
                    (UINT32)*pu8Tmp, (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize, u4DataSize,
                    u4DmxAvailSizeAudio);
                #endif
            }
        }
        return TRUE;
    }
#ifdef TIME_SHIFT_SUPPORT
    else if(u1Type == (UINT8)eSWDMX_GET_CURRENT_TICK)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)&&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio)) //DTV0077970  for audio only program, when nosignal status, we use u4CurTickNum,
        {
            // pu4Para1 will be set in AUD_MMFindTickInfo
            if(!AUD_MMFindTickInfo(prSwdmxInst->u1AdecId, pu4Para1))
            {
                *pu4Para1 = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
            }
        }
        else
        {
            *pu4Para1 = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;
        }
        return TRUE;
    }

    else if(u1Type == (UINT8)eSWDMX_GET_CURRENT_LBA)
    {
        UINT64 u8Tmp;
        UINT64 u8Remainder;
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
		if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)
		{
          u8Tmp = _SWDMX_Div6464(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize -prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset,
			                   (UINT64)(MPG_LBA_BLOCK_SIZE), &u8Remainder);
         // Current MW use UINT32 to get LBA,
         // We might change to UINT64 in the future.
         *pu4Para1 = (UINT32)u8Tmp;
         //        x_memcpy((void*)pu4Para1, // UINT64
         //            &u8Tmp,
         //            sizeof(UINT64));
		}
		else
		{
            *pu4Para1=0;

		}
        return TRUE;
    }
#endif
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        BOOL fgForward = TRUE;
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        if(pu4Para2 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para2 = NULL\n");
            return FALSE;
        }
        
        if (pu4Para3== NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para3 = NULL\n");
            return FALSE;
        }
        
        if(*pu4Para2 == 0)
        {
            fgForward = FALSE;
        }      
        
        *pu4Para1 = _SWDMX_MpgIsSpeedAvailable(u1SrcId, fgForward);
        if((*pu4Para1 == 0) && (*pu4Para3 == 1))
        {
            *pu4Para1 = _SWDMX_MpgIsSpeedAvailable(u1SrcId, !fgForward);
        }


	#if 0 // audio driver already handle pause or other command before aout
	// we could remove this code now
#ifdef SWDMX_MPG_SYNC_DTV_MODE
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
        {
            // to protect trigger audio on right time
            if(((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio))
               /*
               ||
               ((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
                (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo) &&
                (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo))
               */
              )
            {
                *pu4Para1 = FALSE;
            }
            /********
            VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2QueueSize, &u2MaxQueueSize);			
		    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF)&&
				(u2QueueSize <= 1)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_VIDEO_ES)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_H264_VIDEO_ES)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_VC1_ES)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_AVS_VIDEO_ES)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG4_VIDEO_ES))  //EOS has send by swdmx, can't do speed change for a/v file
			{
                *pu4Para1 = FALSE;
				 LOG(5,"_SWDMX_MpgGetInfo EOS has been send, can't change speed\n");
			}
            ********/
        }
        #endif
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_STC_OFFSET)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FirstPTS;
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {

        UINT32 u4DmxAvailSizeAudio = 0;
        UINT32 u4Addr, u4Size;
        UINT32 u4BufSize;
        UINT32 u4DataSize;

        #ifdef SWDMX_MPG_DMX_20
        //DTV00214257  should add 1880 (10 pes) as  sub by dmx
        //u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
        // -> should query dram size
        u4DmxAvailSizeAudio = DMX_MUL_GetMinFreeDRAMBufferSize(prSwdmxInst->u1DmxId,
            DMX_PID_TYPE_ES_AUDIO,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        #else
#ifdef CC_DMX_PURE_AUDIO
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        }
        else
#endif
        {
            //u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO)
            //DTV00214257  should add 1880 (10 pes) as  sub by dmx
            u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(
                DMX_PID_TYPE_ES_AUDIO) + 1880;
            //u4DmxAvailSizeAudio = DMX_GetMinFreeDRAMBufferSize(DMX_PID_TYPE_ES_AUDIO);
        }
        #endif

        if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
        {
            LOG(3, "_SWDMX_MpgEsSetAudioId _SWDMX_GetAudioFifo Fail\n");
            return FALSE;
        }

        u4BufSize = u4Size;
        u4DataSize = u4BufSize - u4DmxAvailSizeAudio;
        *pu4Para1 = (UINT32)u4DataSize;
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_VIDEO_PIDX)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo;
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_AUDIO_PIDX)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio;
        return TRUE;
    }

    UNUSED(pu4Para2);
    UNUSED(pu4Para3);
    return FALSE;
}

BOOL _SWDMX_MpgSetStrmID(UINT8 u1SrcId, UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

	LOG(5, "_SWDMX_MpgSetStrmID(%d) u1HdrType = %d, u4StrmID = %d, u4StrmSubID = %d\n",
	    u1SrcId, u1HdrType, u4StrmID, u4StrmSubID);

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoId = (UINT16)u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_VID2:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Video2Id = (UINT16)u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioId = (UINT16)u4StrmID;
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioSubId = (UINT16)u4StrmSubID;
            ucCount = 2;
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Audio2Id = (UINT16)u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2SubTitleId = (UINT16)u4StrmID;
            break;
        #ifdef SWDMX_MPG_SET_PCR_ID
        case eSWDMX_STRM_TYPE_PCR:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PcrId = (UINT16)u4StrmID;
            break;
        #endif
        default:
            return FALSE;
    }

    return TRUE;
}

BOOL _SWDMX_MpgEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    BOOL fgRet = TRUE;
    UINT8 u1Pidx;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_MpgEnableStrm(%d), u1StrmType =%d, eFmt = %d\n",
        u1SrcId, u1StrmType, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt);

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdrVc1 = FALSE;
#endif

                #ifdef SWDMX_MPG_DMX_20
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    u1Pidx = (UINT8)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart + 4)
                        + (u1SrcId * 3));
                    if(!DMX_MUL_SetPidxInstance(prSwdmxInst->u1DmxId, u1Pidx))
                    {
                        LOG(3, "DMX_MUL_SetPidxInstance Fail\n");
                        return FALSE;
                    }
                }
                else
                {
                u1Pidx = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                if(u1Pidx >= DMX_NUM_PID_INDEX)
                {
                    LOG(3, "DMX_MUL_GetAvailablePidx Fail\n");
                    return FALSE;
                }
                }
                #endif

                // when current is not 1x
                // force to jump next position
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpping)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
                }
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                   ||
                   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                  )
                {
                    // We can't change file position on TS file
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendSeqHdr = TRUE;
                }

                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendVideo2Eos = FALSE;

#ifndef CC_PS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsSetVideoId(u1SrcId, u1Pidx, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetVideoId(u1SrcId, u1Pidx, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2VideoId,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1VideoKeyIndex,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4VideoDescMode, FALSE);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    fgRet = _SWDMX_MpgEsSetVideoId(u1SrcId, u1Pidx);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo = TRUE;
                }
            }
            break;

        case eSWDMX_STRM_TYPE_VID2:
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo2)
            {
                #ifdef SWDMX_MPG_DMX_20
                u1Pidx = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                if(u1Pidx >= DMX_NUM_PID_INDEX)
                {
                    LOG(3, "DMX_MUL_GetAvailablePidx Fail\n");
                    return FALSE;
                }
                #endif

#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetVideoId(u1SrcId, u1Pidx,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Video2Id,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1Video2KeyIndex,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4Video2DescMode, TRUE);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo2 = TRUE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X == 0)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                }
                // prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS might be old value (before seek)
                /*if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS == 0)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastPTS;
                }*/


                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif

                #ifdef SWDMX_MPG_DMX_20
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    u1Pidx = (UINT8)((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PidIdxStart + 1)
                        + (u1SrcId * 3));
                    if(!DMX_MUL_SetPidxInstance(prSwdmxInst->u1DmxId, u1Pidx))
                    {
                        LOG(3, "DMX_MUL_SetPidxInstance Fail\n");
                        return FALSE;
                    }
                }
                else
                {
                u1Pidx = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                if(u1Pidx >= DMX_NUM_PID_INDEX)
                {
                    LOG(3, "DMX_MUL_GetAvailablePidx Fail\n");
                    return FALSE;
                }
                }
                #endif

#ifndef CC_PS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsSetAudioId(u1SrcId, u1Pidx, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioId,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioSubId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetAudioId(u1SrcId, u1Pidx,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2AudioId, 0,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1AudioKeyIndex,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4AudioDescMode);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    fgRet = _SWDMX_MpgEsSetAudioId(u1SrcId, u1Pidx);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio = TRUE;
                }
				/* marked for DTV00089464, Low bitrate AAC file, fgSendEOF is coming too soon and _SWDMX_MpgHandleEOS is delivered twice here.
                // There is a case, MW will open audio stream after we deliver video EOS
                // Call handle EOS to send audio eos for this propose
                if(prSwdmxInst)
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF) &&
                       (prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PLAY))
                    {
                        x_thread_delay(1); // let handle eos finish
                        LOG(5, "_SWDMX_MpgEnableStrm _SWDMX_MpgHandleEOS\n");
                        _SWDMX_MpgHandleEOS(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed);
                    }
                }
                */
            }
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio2)
            {
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
                //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;

                #ifdef SWDMX_MPG_DMX_20
                u1Pidx = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                if(u1Pidx >= DMX_NUM_PID_INDEX)
                {
                    LOG(3, "DMX_MUL_GetAvailablePidx Fail\n");
                    return FALSE;
                }
                #endif

#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetAudioId(u1SrcId, u1Pidx,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2Audio2Id, 1,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u1Audio2KeyIndex,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u4Audio2DescMode);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio2 = TRUE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSubTitle)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSubTitle = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_PCR:
#ifndef CC_TS_DISABLE
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                /*
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotPcr = FALSE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGotPcrReady = FALSE;
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnablePcr = FALSE;
                    // Currently, MW set PCR via dmx_mw.c, but not dmx_mm_mw.c
                    // It's enough for us to know PCR is enable or not.

                    #ifdef SWDMX_MPG_SET_PCR_ID
                    fgRet = _SWDMX_MpgTsSetPCRId(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.u2PcrId);
                    if(fgRet)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr = TRUE;
                    }
                    #else
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr = TRUE;
                    #endif
                }
                */
            }
            break;
#endif
        default:
            break;
    }

    LOG(3, "_SWDMX_MpgEnableStrm(%d) %d\n", u1SrcId, fgRet);
    UNUSED(pfnCbFunc);
    return TRUE;
}


BOOL _SWDMX_MpgDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType)
{
    BOOL fgRet = TRUE;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

	LOG(5, "_SWDMX_MpgDisableStrm(%d) u1StrmType = %d\n", u1SrcId, u1StrmType);

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
            {
                UINT8 u1PidxVideo = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgChangeVideo = TRUE;

#ifndef CC_PS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsFreeVideoId(u1SrcId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsFreeVideoId(u1SrcId, FALSE);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    fgRet = _SWDMX_MpgEsFreeVideoId(u1SrcId);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo = FALSE;
                }
                #ifdef SWDMX_MPG_DMX_20
                fgRet = DMX_MUL_FreePidx(u1PidxVideo);
                if(!fgRet)
                {
                    LOG(3, "DMX_MUL_FreePidx Fail\n");
                    fgRet = FALSE;
                }
                #endif
            }
            break;

        case eSWDMX_STRM_TYPE_VID2:
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo2)
            {
                UINT8 u1PidxVideo = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2;
#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsFreeVideoId(u1SrcId, TRUE);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo2 = FALSE;
                }
                #ifdef SWDMX_MPG_DMX_20
                fgRet = DMX_MUL_FreePidx(u1PidxVideo);
                if(!fgRet)
                {
                    LOG(3, "DMX_MUL_FreePidx Fail\n");
                    fgRet = FALSE;
                }
                #endif
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio)
            {
                UINT8 u1PidxAudio = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio;
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;

#ifndef CC_PS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsFreeAudioId(u1SrcId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsFreeAudioId(u1SrcId);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    fgRet = _SWDMX_MpgEsFreeAudioId(u1SrcId);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }

                // flush audio data
                // same with
                // _SWDMX_MpgFlushVFifo();
                // UNUSED(DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo));
                {
                    if(u1PidxAudio != 0xFF)
                    {
                        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
                        {
                            BOOL fgRet1;
                            fgRet1 = DMX_UpdateReadPointer(u1PidxAudio, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr);
                            if(!fgRet1)
                            {
                                LOG(5, "_SWDMX_MpgDisableStrm DMX_UpdateReadPointer Fail\n");
                            }
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
                        }
                        UNUSED(DMX_MM_FlushBuffer(u1PidxAudio));
                    }
                }

                if(fgRet)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio = FALSE;
                }
                #ifdef SWDMX_MPG_DMX_20
                fgRet = DMX_MUL_FreePidx(u1PidxAudio);
                if(!fgRet)
                {
                    LOG(3, "DMX_MUL_FreePidx Fail\n");
                    fgRet = FALSE;
                }
                #endif
				
			   //DTV00091703 when play to the end of the file, audio EOS has been sent by swdmx, but audio drv has 
			   //not notify mw, at this time change audio trick, will cause playback hang (swmdx can't send eos again) 
			   if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos)
			   {			  
			      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
			   }
			   
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetFirstAudPts = FALSE;	//DTV00093908
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS = 0;     
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;			   
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgSubTitle = FALSE;
            break;
        case eSWDMX_STRM_TYPE_PCR:
#ifndef CC_TS_DISABLE
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                /*
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgEnablePcr = FALSE;
                    // Currently, MW set PCR via dmx_mw.c, but not dmx_mm_mw.c
                    // It's enough for us to know PCR is enable or not.
                    #ifdef SWDMX_MPG_SET_PCR_ID
                    fgRet = _SWDMX_MpgTsFreePCRId(u1SrcId);
                    if(fgRet)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr = FALSE;
                    }
                    #else
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgPcr = FALSE;
                    #endif
                }
                */
            }
            break;
#endif
        default:
            //fgRet = FALSE;
            break;
    }

    LOG(3, "_SWDMX_MpgDisableStrm(%d) do nothing\n", u1SrcId);
    return TRUE;
}


VOID _SWDMX_MpgSetSwdmxInfo(UINT8 u1SrcId, SWDMX_INFO_T* prSwdmxInfo)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    if(prSwdmxInfo == NULL)
    {
        return;
    }
    if(prSwdmxInst != prSwdmxInfo)
    {
        LOG(1, "_SWDMX_MpgSetSwdmxInfo prSwdmxInst != prSwdmxInfo\n");
    }
    UNUSED(prSwdmxInfo);
    UNUSED(prSwdmxInst);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxVideo2 = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio = 0xFF;

    #ifdef SWDMX_MPG_SET_PCR_ID
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxPCR = 0xFF;
    #endif
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize = MPG_BATCH_SIZE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeReadSize = (MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE);
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase = MPG_ALIGN_SIZE;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID = 1;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederBufNum = MPG_BUF_NUM;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum = MPG_REQ_NUM;
}


#ifdef SWDMX_STATISTICS
VOID _SWDMX_ResetTime(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederAvgTime = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederCount = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMaxTime = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMinTime = 0;
}

VOID _SWDMX_PrintTime(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL(prSwdmxInst);

    LOG(0, "Feeder Count(%d) Time(%d, %d, %d)\n",
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederCount,
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMaxTime, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederAvgTime, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederMinTime);
}

#endif


BOOL _SWDMX_MpgGetPosByTime(UINT8 u1SrcId, BOOL fgUpdate, UINT32 u4Time, UINT64 *pu8Pos)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        *pu8Pos = 0;
        return FALSE;
    }

    if(//(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
      )
    {
        MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
        {
            prIdxTbl = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
            prTblEntry = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
            if((prTblEntry) && (prIdxTbl))
            {
                UINT32 u4Cnt = 0;
                if(prIdxTbl->ui4_number_of_entry == 0)
                {
                    LOG(3, "_SWDMX_MpgGetInfo ui4_number_of_entry 0\n");
                    return FALSE;
                }
                while(u4Cnt < prIdxTbl->ui4_number_of_entry)
                {
                    if(prTblEntry[u4Cnt].ui4_pts >= u4Time)
                    {
                        break;
                    }
                    u4Cnt++;
                }
                if(u4Cnt < prIdxTbl->ui4_number_of_entry)
                {
                    *pu8Pos = prIdxTbl->ui8_base_offset +
                        prTblEntry[u4Cnt].ui4_relative_offset;
                    return TRUE;
                }

                LOG(3, "_SWDMX_MpgGetInfo out of last index time %d\n",
                    prTblEntry[prIdxTbl->ui4_number_of_entry-1].ui4_pts);
            }
            else
            {
                UINT64 u8Remainder;
                if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime > 0)
                {
                    *pu8Pos = _SWDMX_Div6464((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset)*(u4Time),
                                            (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
                    {
                        UNUSED(_SWDMX_Div6464(*pu8Pos, (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
                        *pu8Pos -= u8Remainder;
                    }
                    return TRUE;
                }
                else
                {
                    *pu8Pos = 0;
                }
                *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
            }
        }
    }

    UNUSED(fgUpdate);
    return FALSE;
}

BOOL _SWDMX_MpgGetPosByTick(UINT8 u1SrcId, BOOL fgUpdate, UINT32 u4Tick, UINT64 *pu8Pos)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetPosByTick valid range is not set\n");
        return FALSE;
    }
	// from the explain of mw pv_end_address is the end address of the tick_index_talbe;
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetPosByTick prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTick valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTick valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end >=
       prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start)
    {
        if((u4Tick > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end) ||
           (u4Tick < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start))
        {
            if(fgUpdate)
            {
                LOG(3, "1 _SWDMX_MpgGetPosByTick u4Tick(%d) not in %d-%d\n",
                    u4Tick,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
            }
            *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
            *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
            *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }
    }
    else
    {
        if((u4Tick > prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end) &&
           (u4Tick < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start))
        {
            if(fgUpdate)
            {
                LOG(3, "2 _SWDMX_MpgGetPosByTick u4Tick(%d) not in %d-%d\n",
                    u4Tick,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
            }
            *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
            *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
            *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
            _SWDMX_UnlockRangeSema(u1SrcId);
            return FALSE;
        }
    }

    prTargetAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + u4Tick);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetPosByTick prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
        *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
        *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    // DTV00142188, When ring buffer begins working,offset time jumps step by step to the biggest value.
    // MW change prEndAddr to not include the valid tick information
    // We have to change not to use prEndAddr for meta-data
    if(prTargetAddr > prEndAddr)
    {
        prTargetAddr -= ((prEndAddr +1)- prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
    *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);

    if(fgUpdate)
    {
        // When we start with seek to last, we only update u4CurTickNum
        // Read need many time to reach curticknum.
        // We should update read tick too.
        // swdmx must be in pause state now for this change read position.
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prTargetAddr->ui4_tick_num;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = prTargetAddr->ui4_tick_num;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetPosByTick u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return TRUE;
}

BOOL _SWDMX_MpgGetPosByTickNum(UINT8 u1SrcId, BOOL fgUpdate, UINT32 u4TickNum, UINT64 *pu8Pos)
{

    SWDMX_INFO_T* prSwdmxInst;
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif

    prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetPosByTickNum valid range is not set\n");
        return FALSE;
    }
	// from the explain of mw pv_end_address is the end address of the tick_index_talbe;
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        if(fgUpdate)
        {
            LOG(3, "_SWDMX_MpgGetPosByTickNum u4TickNum(%d) not in %d-%d\n",
                u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        }
        *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
        *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
        *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
        *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
        *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    if(prTargetAddr > prEndAddr)
    {
        prTargetAddr -= ((prEndAddr +1) - prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
    *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);

    if(fgUpdate)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetPosByTickNum u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return TRUE;
}

#ifdef LINUX_TURNKEY_SOLUTION
extern VOID rm_copy_from_user(VOID* pv_kernel,VOID *pv_user,UINT32 size);
#endif
BOOL _SWDMX_MpgGetIPosByTickNum(UINT8 u1SrcId, UINT64 u8Start, UINT32 u4TickNum, UINT64 *pu8Pos)
{
    BOOL fgRet;
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
    INT32 i4LBAOffset;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
    fgRet = FALSE;
#ifdef TIME_SHIFT_SUPPORT

    i4LBAOffset = 0;
    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetIPosByTickNum valid range is not set\n");
        return FALSE;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNoUseIFrmInfo)
    {
        return FALSE;
    }
	// from the explain of mw pv_end_address is the end address of the tick_index_talbe, not the start address of the last entry 
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        #if 0
        if(fgUpdate)
        {
            LOG(3, "_SWDMX_MpgGetIPosByTickNum u4TickNum(%d) not in %d-%d\n",
                u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        }
        #endif
        *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
        *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
        *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba));
        *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);
        *pu8Pos += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    if(prTargetAddr > prEndAddr)
    {
        prTargetAddr -= ((prEndAddr +1) - prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
    if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetAddr, PVR_TICK_INDEX_FLAG_HAS_I_FRAME))
    {
        #ifndef SWDMX_MPG_USE_MTMPLAYER_PVR
        if((prTargetAddr->pt_tick_detail) && (VIRTUAL((UINT32)prTargetAddr->pt_tick_detail)))
        {
            UINT32 u4Tmp;
            UINT32 u4PicType = 0x3;
            UINT32 u4Lba = 0;
            UINT32 u4EntryNum;

			PVR_TICK_DETAIL_T *prTick_detail;   //DTV00087636   ,prTargetAddr->pt_tick_detail is a user space address, we shuld convert to kernel

            #ifdef LINUX_TURNKEY_SOLUTION
			   prTick_detail = x_mem_alloc(sizeof(PVR_TICK_DETAIL_T));
			   rm_copy_from_user(prTick_detail,prTargetAddr->pt_tick_detail,sizeof(PVR_TICK_DETAIL_T));
			#else
			  prTick_detail = prTargetAddr->pt_tick_detail;
			#endif

            //UINT32 u4EntryNum = (UINT32)(PVR_TICK_DETAIL_GET_ENTRIES((PVR_TICK_DETAIL_T *)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)))));
            u4EntryNum = (UINT32)(PVR_TICK_DETAIL_GET_ENTRIES((PVR_TICK_DETAIL_T *)((UINT32)(VIRTUAL((UINT32)prTick_detail)))));
			u4Lba = prTargetAddr->ui4_lba;
            if(u4EntryNum > 100)
            {
                u4EntryNum = 0; // error handle
                LOG(3, "_SWDMX_MpgGetIPosByTickNum u4EntryNum > 100\n");
            }
            for(u4Tmp = 0 ; u4Tmp < u4EntryNum ; u4Tmp++)
            {
                //if(((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))) == 0)
                if(((UINT32)(VIRTUAL((UINT32)prTick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))) == 0)

				{
                    LOG(3, "_SWDMX_MpgGetIPosByTickNum GET_PIC_TYPE null\n");
                    break;
                }
                //if((!prTargetAddr->pt_tick_detail) || (!VIRTUAL(prTargetAddr->pt_tick_detail)))
                if((!prTick_detail) || (!VIRTUAL((UINT32)prTick_detail)))

                {
                    LOG(3, "_SWDMX_MpgGetIPosByTickNum pt_tick_detail null\n");
                    break;
                }
                //u4PicType = PVR_PIC_INFO_GET_PIC_TYPE((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))));
                u4PicType = PVR_PIC_INFO_GET_PIC_TYPE((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL((UINT32)prTick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))));

				if(u4PicType == 0) // I frame
                {
                    //PVR_PIC_INFO_GET_LBA_OFFSET(((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T)))), &i4LBAOffset);
                    PVR_PIC_INFO_GET_LBA_OFFSET(((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL((UINT32)prTick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T)))), &i4LBAOffset);

					if(i4LBAOffset < 0)
                    {
                        if(u4Lba < (-i4LBAOffset))
                        {
                            LOG(3, "_SWDMX_MpgGetIPosByTickNum (u4Lba < (-i4LBAOffset))\n");
                            //ASSERT(0);
                            break;
                        }
                    }
                    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                       (((UINT32)(u4Lba + i4LBAOffset) * MPG_LBA_BLOCK_SIZE) > u8Start))
                    {
                        if((((UINT32)(u4Lba + i4LBAOffset) * MPG_LBA_BLOCK_SIZE) - u8Start) > (10*1024*1024))
                        {
                            LOG(3, "_SWDMX_MpgGetIPosByTickNum (u4Lba + i4LBAOffset)(%d) too far)\n",
                                (u4Lba + i4LBAOffset));
                            //ASSERT(0);
                            // error handle
                            break;
                        }
                        fgRet = TRUE;
                        break;
                    }
                    else if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0) &&
                            (((UINT32)(u4Lba + i4LBAOffset) * MPG_LBA_BLOCK_SIZE) < u8Start))
                    {
                        if((((UINT32)(u4Lba + i4LBAOffset) * MPG_LBA_BLOCK_SIZE) + (10*1024*1024)) < u8Start)
                        {
                            LOG(3, "_SWDMX_MpgGetIPosByTickNum (u4Lba + i4LBAOffset)(%d) too far)\n",
                                (u4Lba + i4LBAOffset));
                            //ASSERT(0);
                            // error handle
                            break;
                        }
                        fgRet = TRUE;
                        break;
                    }
                }
            }
		    #ifdef LINUX_TURNKEY_SOLUTION
	         x_mem_free(prTick_detail);
            #endif
        }
        #endif
    }
    *pu8Pos += (i4LBAOffset*MPG_LBA_BLOCK_SIZE);
    *pu8Pos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8Pos);

    #if 0
    if(fgUpdate)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetIPosByTickNum u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    #endif
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return fgRet;
}

BOOL _SWDMX_MpgGetTickByPos(UINT8 u1SrcId, BOOL fgUpdate, UINT64 u8Pos, INT32 i4PlaySpeed, UINT32 *pu4TickNum)
{
#ifdef TIME_SHIFT_SUPPORT
    UINT32 u4Tmp;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetEndAddr;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    *pu4TickNum = 0;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetTickByPos valid range is not set\n");
        return FALSE;
    }

    if(u8Pos < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos (u8Pos < u8FifoOffset) fail\n");
    }
	// from the explain of mw pv_end_address is the end address of the tick_index_talbe;
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prSafeEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
    if((prSafeEnd == NULL) ||
       (prSafeEnd > prEndAddr) ||
       (prSafeEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos safe end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    // [DTV00141987] After fast rewind,the fast forward to right boundary ,audio isn't smooth
    // We should not use fast mode when out of safe range
    // = should be include. So we could have complete 2 tick between safe and valid
    if((u8Pos >= (((UINT64)prSafeEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                  prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
      )
    {
        //LOG(5, "_SWDMX_MpgGetTickByPos u8Pos(0x%llx) bigger than safe end 0x%llx\n",
        //    u8Pos,
        //    ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE),
        //    ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE));

        /*
        if(fgUpdate)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfSafeRange = TRUE;
        }
        */
    }

    if((u8Pos >= (((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) 
        + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset
        + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase)) ||
       (u8Pos < (((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE) + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Pos,
            _SWDMX_MpgLBA2Pos(u1SrcId, (prValidStart->ui4_lba)),
            _SWDMX_MpgLBA2Pos(u1SrcId, (prValidEnd->ui4_lba)));
        *pu4TickNum = prValidStart->ui4_tick_num;

        if(fgUpdate)
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgOutOfRange = TRUE;
        }

        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex == NULL)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex == NULL)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
    }

    if(prValidStart > prValidEnd)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
        }
    }
    else
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) ||
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) ||
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
        }
    }

    prTargetAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex;
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prTargetAddr null\n");
        *pu4TickNum = prValidStart->ui4_tick_num;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    prTargetEndAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex;
    if(prTargetEndAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prTargetEndAddr null\n");
        *pu4TickNum = prValidStart->ui4_tick_num;
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    for(u4Tmp=0;u4Tmp<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
    {
        if(i4PlaySpeed >= 0)
        {
            if(prTargetAddr > prEndAddr)
            {
                prTargetAddr = prStartAddr;
            }
            if(u8Pos <= (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                         prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
            {
                break;
            }
            if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetAddr,
                (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD)))
            {
                break;
            }
            prTargetAddr = prTargetAddr + 1;
        }
        else
        {
            if(prTargetAddr < prStartAddr)
            {
                prTargetAddr = prEndAddr;
            }
            if(u8Pos >= (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                         prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
            {
                break;
            }
            if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetAddr,
                (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD)))
            {
                break;
            }
            prTargetAddr = prTargetAddr - 1;
        }
    }


    for(u4Tmp=0;u4Tmp<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
    {
        if(i4PlaySpeed >= 0)
        {
            if(prTargetEndAddr > prEndAddr)
            {
                prTargetEndAddr = prStartAddr;
            }
            if((u8Pos + (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OneTimeSize * MPG_BATCH_READ_SIZE))
                < (((UINT64)prTargetEndAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
            {
                break;
            }
            if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
                (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD)))
            {
                break;
            }
            if(prTargetEndAddr->ui4_tick_num >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick)
            {
                break;
            }
            prTargetEndAddr = prTargetEndAddr + 1;
        }
    }

    if(fgUpdate)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetEndAddr;
        if((i4PlaySpeed >= 0) &&
           (SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
            (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetEndAddr;
        }
    }

    *pu4TickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
    LOG(8, "_SWDMX_MpgGetTickByPos u4TickNum = %d\n", *pu4TickNum);
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return TRUE;
}
#if 0
BOOL _SWDMX_MpgCheckNoSignal()
{
#ifdef TIME_SHIFT_SUPPORT
    UINT32 u4Tmp;
    UINT32 u4Num;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif
    u4Num = 0;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }

    _SWDMX_UnlockRangeSema(u1SrcId);

    if((u8Pos >= ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE)) ||
       (u8Pos < ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE)))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Pos,
            ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE),
            ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE));
        return FALSE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex == NULL)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex null\n");
        return FALSE;
    }
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex == NULL)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
    }

    if(prValidStart > prValidEnd)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex out of range 1\n");
            return FALSE;
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastEndTickIndex out of range 2\n");
            return FALSE;
        }
    }
    else
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) ||
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex out of range 3\n");
            return FALSE;
        }
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) ||
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastEndTickIndex out of range 4\n");
            return FALSE;
        }
    }

    prTargetAddr = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex;
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prTargetAddr null\n");
        return FALSE;
    }

    for(u4Tmp=0;u4Tmp<prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
    {
        if(prTargetAddr >= prEndAddr)
        {
            prTargetAddr = prStartAddr;
        }
        if(prTargetAddr == prValidEnd)
        {
            break;
        }
        if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetAddr,
            (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD)))
        {
            break;
        }
        prTargetAddr = prTargetAddr + 1;
    }

    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        if((i4PlaySpeed > 0) &&
           (SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
            (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetEndAddr;
        }
    }

    *pu4TickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
    LOG(8, "_SWDMX_MpgCheckNoSignal u4TickNum = %d\n", *pu4TickNum);
#endif
    return TRUE;
}
#endif

BOOL _SWDMX_MpgIsPositionAvailable(UINT8 u1SrcId, UINT64 u8Position)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeEnd;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return TRUE;
    }

#ifdef TIME_SHIFT_SUPPORT
    // from the explain of mw pv_end_address is the end address of the tick_index_talbe;
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable prStartAddr >= prEndAddr fail\n");
        return TRUE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prSafeStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start);
    if((prSafeStart == NULL) ||
       (prSafeStart > prEndAddr) ||
       (prSafeStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return TRUE;
    }

    prSafeEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
    if((prSafeEnd == NULL) ||
       (prSafeEnd > prEndAddr) ||
       (prSafeEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return TRUE;
    }   
    
    if(u8Position > (((UINT64)prSafeEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeStart->ui4_lba)),
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeEnd->ui4_lba)));

        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    if(u8Position < (((UINT64)prSafeStart->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeStart->ui4_lba)),
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeEnd->ui4_lba)));

        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return TRUE;
}


BOOL _SWDMX_MpgIsSpeedAvailable(UINT8 u1SrcId, BOOL fgForward)
{
#ifdef TIME_SHIFT_SUPPORT
    UINT64 u8Position;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeEnd;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return TRUE;
    }

#ifdef TIME_SHIFT_SUPPORT
    // handle no signa case
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_NORMAL)
    {
        return TRUE;
    }
    // handle empty case
    else if((!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo) &&
            // while we are parsing table, not handle this.
            (!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgCheckPSISize) &&
            // both audio on and off have to check this.
            //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgAudio) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed != 1000) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed != 0))
               // do not need to consider changing
               // because we could get our swdmx current position
               // that's the correct one.
               //(prSwdmxInst) &&
               //(!prSwdmxInst->fgPlayChange) && // not changing speed
    {
        return TRUE;
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
	// from the explain of mw pv_end_address is the end address of the tick_index_talbe;
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable prStartAddr >= prEndAddr fail\n");
        return TRUE;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prSafeStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start);
    if((prSafeStart == NULL) ||
       (prSafeStart > prEndAddr) ||
       (prSafeStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return TRUE;
    }

    prSafeEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
    if((prSafeEnd == NULL) ||
       (prSafeEnd > prEndAddr) ||
       (prSafeEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return TRUE;
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable (u8SendedSize < u8FifoOffset) fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return TRUE;
    }

    u8Position = (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
    // DTV00142192, After repeatedly press"Step Forward" key, video freezes and offset time adds.
    // use the new seek position
    if((prSwdmxInst) &&
       (prSwdmxInst->fgPlayChange) &&
       (prSwdmxInst->u8SeekPos != (UINT64)(-1)) &&
       (prSwdmxInst->eSwDmxStatus == eSWDMX_STATE_PAUSE))
    {
        // pause and change position
        u8Position = (prSwdmxInst->u8SeekPos);
    }

    if((u8Position > (((UINT64)prSafeEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)) &&
       (fgForward))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeStart->ui4_lba)),
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeEnd->ui4_lba)));

        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    if((u8Position < (((UINT64)prSafeStart->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset)) &&
       (!fgForward))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeStart->ui4_lba)),
            _SWDMX_MpgLBA2Pos(u1SrcId, (prSafeEnd->ui4_lba)));

        _SWDMX_UnlockRangeSema(u1SrcId);
        return FALSE;
    }
    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return TRUE;
}

INT32 _SWDMX_MpgHandleNoSignel(UINT8 u1SrcId, UINT64 *pu8SeekPos, INT32 i4PlaySpeed)
{
    INT32 i4Ret;
#ifdef TIME_SHIFT_SUPPORT
    static UINT32 u4LogCnt = 0;
    UINT32 u4TickNum;
    UINT64 u8TickNum;
    UINT64 u8Remainder;
	UINT32 u4CurDeltaTime;
	HAL_TIME_T CheckTime;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    i4Ret = MPG_SEND_OK;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return MPG_SEND_FAIL;
    }
#ifdef TIME_SHIFT_SUPPORT
    // Step
    if(i4PlaySpeed == 1)
    {
        SWDMX_Pause(u1SrcId);
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            _SWDMX_RANGE_CALLBACK(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                MM_RANGE_SEEK_DONE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                0xFFFFFFFF,
                (UINT32)u1SrcId);
        }
        else
        {
            LOG(3, "pfnSwdmxRangeCb null\n");
        }
        LOG(5, "NoSignel: MM_RANGE_SEEK_DONE\n");
    }
    else if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal >= 0xF0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal = 0;
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            _SWDMX_RANGE_CALLBACK(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                MM_RANGE_SEEK_DONE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                0xFFFFFFFF,
                (UINT32)u1SrcId);
        }
        else
        {
            LOG(3, "pfnSwdmxRangeCb null\n");
        }
        LOG(5, "NoSignel: MM_RANGE_SEEK_DONE\n");
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1NumNoSignal += 4;
    }

    // DTV00214748 we should calibrate prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime as x_thread_delay() is not evry exactly
	if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust==TRUE)&&(i4PlaySpeed==1000))
    {
	   HAL_GetTime(&CheckTime);
       u4CurDeltaTime= CheckTime.u4Seconds*1000+ CheckTime.u4Micros/1000 - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime;
	   if((u4CurDeltaTime-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OrgtDeltaTime)>300)
	   {
          prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime += (u4CurDeltaTime-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OrgtDeltaTime);
	      LOG(5,"_SWDMX_MpgHandleNoSignal adjust u8CurTime Delta:%d ms\n",
	        u4CurDeltaTime-prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4OrgtDeltaTime);
	   }
	}
	else
	{
	   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
	}

    x_thread_delay(1000/MPG_TIMER_DELAY_TIMES);
    if(i4PlaySpeed >= 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime +=
            i4PlaySpeed/MPG_TIMER_DELAY_TIMES;
    }
    else
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime >= ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime -=
                ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES);
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime = 0;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase > 0)
    {
        u8TickNum = _SWDMX_Div6464(
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase),
            &u8Remainder);
    }
    else
    {
        ASSERT(0);
        u8TickNum = _SWDMX_Div6464(
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(500),  // u8CurTime is milli-second unit
            &u8Remainder);
    }
    ASSERT(u8TickNum < (UINT32)0xFFFFFFFF);
    u4TickNum = (UINT32)u8TickNum;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

    // DTV00142071, After plug RF cable again, mini pane time displays incorrectly.
    // set current tick to vdp, so MW will get correct one after plug signal
    if (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.fgVideo)
    {
        VDP_SET_PIC_INFO_T rVdpSetPicInfo;
        x_memset(&rVdpSetPicInfo, 0, sizeof(rVdpSetPicInfo));
        rVdpSetPicInfo.u4TickNum = u4TickNum;
        if (VDP_SetPicInfo(prSwdmxInst->u1B2rId, rVdpSetPicInfo) != VDP_SET_OK)
        {
            LOG(3, "NoSignel: VDP_SetPicInfo u4TickNum(%d) fail\n", u4TickNum);
        }
    }

    u4LogCnt++;
    if(u4LogCnt >= MPG_TIMER_DELAY_TIMES)
    {
        LOG(5, "NoSignel: CurTickNum %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
        u4LogCnt = 0;
        UNUSED(u4LogCnt);
    }

    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL) ||
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE))
    {
        if(((i4PlaySpeed >= 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum >=
             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum))
           ||
           ((i4PlaySpeed < 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum <=
             prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum))
          )
        {
            LOG(3, "NoSignel: u4NoSignalStage 2, tick num %d\n",
                u4TickNum);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_SIGNAL_Y_TO_N,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                    else //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE)
                    {
                        _SWDMX_RANGE_CALLBACK(
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_BAD_TICK_N_TO_Y,
                            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF,
                            (UINT32)u1SrcId);
                    }
                }
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_NO_SIGNAL;
            }
            else //(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE)
            {
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_BAD_STORAGE;
            }
        }
    }

    //if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_NO_SIGNAL) &&
    //   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_BAD_STORAGE)&&
    //   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgInScramble!=TRUE))
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_NO_SIGNAL) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_BAD_STORAGE))

    {
        LOG(3, "NoSignel: u4NoSignalStage 1, waiting, cur tick %d, ng tick %d\n",
            u4TickNum,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
        return MPG_SEND_FULL;
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "NoSignel:  prStartAddr >= prEndAddr fail\n");
        return MPG_SEND_FAIL;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "NoSignel: valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "NoSignel: valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        LOG(3, "NoSignel: invalid u4TickNum %d, %d-%d\n",
            u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        if(i4PlaySpeed >= 0)
        {
            LOG(3, "NoSignel: invalid u4TickNum %d->%d\n",
                u4TickNum, prValidEnd->ui4_tick_num);
            u4TickNum = prValidEnd->ui4_tick_num;
        }
        else
        {
            LOG(3, "NoSignel: invalid u4TickNum %d->%d\n",
                u4TickNum, prValidStart->ui4_tick_num);
            u4TickNum = prValidStart->ui4_tick_num;
        }
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "NoSignel: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }
    if(prTargetAddr > prEndAddr)
    {
        prTargetAddr -= ((prEndAddr +1) - prStartAddr);
    }
    if(prTargetAddr == NULL)
    {
        LOG(3, "NoSignel: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;

    if((!(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_EMPTY))) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage 0, from have signal tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_SIGNAL_N_TO_Y,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
    }
    else if((!(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_BAD))) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage 0, from good storage tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_BAD_TICK_Y_TO_N,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
    }

    if(((SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_EMPTY))) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_NO_SIGNAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage UNDER_NO_SIGNAL, no signal tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_SIGNAL_Y_TO_N,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
    }
    else if(((SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_BAD))) &&
       (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_BAD_STORAGE;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(u1SrcId, *pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage UNDER_BAD_STORAGE, bad tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_BAD_TICK_N_TO_Y,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
    }

    // because here is empty, lba value is invalid
    /*
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA)
        &&
       (
        ((i4PlaySpeed >= 0) &&
         (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
        ||
        ((i4PlaySpeed < 0) &&
         (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
       )
      )
    {
        LOG(3, "NoSignel: fgMonitorLBA addr 0x%llx in start addr 0x%llx end addr 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr,
            u8StartAddr,
            u8EndAddr);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_MONITOR_LBA_ARRIVAL,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
    }
    */

    if(prTargetAddr->ui4_tick_num >=
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick))
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "NoSignel: fgMonitorSafeEnd tick %d safe end tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_OUT_OF_SAFE_RANGE_END,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
    }

    if((prTargetAddr->ui4_tick_num) <
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick))
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "NoSignel: fgMonitorSafeStart tick %d safe start tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_OUT_OF_SAFE_RANGE_START,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
    }

    if(prTargetAddr->ui4_tick_num >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "NoSignel: fgMonitorValidEnd tick %d valid end tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_REACH_VALID_RANGE_END,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    if(prTargetAddr->ui4_tick_num < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "NoSignel: fgMonitorValidStart tick %d valid start tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_REACH_VALID_RANGE_START,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return i4Ret;
}

INT32 _SWDMX_MpgHandleEmpty(UINT8 u1SrcId, UINT64 *pu8SeekPos, INT32 i4PlaySpeed)
{
    INT32 i4Ret;
#ifdef TIME_SHIFT_SUPPORT
    static UINT32 u4LogCnt = 0;
    UINT32 u4TickNum;
    UINT64 u8TickNum;
    UINT64 u8Remainder;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    i4Ret = MPG_SEND_OK;
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return MPG_SEND_FAIL;
    }
#ifdef TIME_SHIFT_SUPPORT
    // Step
    if(i4PlaySpeed == 1)
    {
        SWDMX_Pause(u1SrcId);
        if(/*(_SWDMX_RANGE_CALLBACK) &&*/
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            _SWDMX_RANGE_CALLBACK(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                MM_RANGE_SEEK_DONE,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                0xFFFFFFFF,
                (UINT32)u1SrcId);
        }
        else
        {
            LOG(3, "pfnSwdmxRangeCb null\n");
        }
        LOG(5, "Empty: MM_RANGE_SEEK_DONE\n");
    }

    x_thread_delay(1000/MPG_TIMER_DELAY_TIMES);
    if(i4PlaySpeed >= 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime +=
            i4PlaySpeed/MPG_TIMER_DELAY_TIMES;
    }
    else
    {
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime >= ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime -=
                ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES);
        }
        else
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime = 0;
        }
    }

    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase > 0)
    {
        u8TickNum = _SWDMX_Div6464(
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase),
            &u8Remainder);
    }
    else
    {
        ASSERT(0);
        u8TickNum = _SWDMX_Div6464(
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(45000),
            &u8Remainder);
    }
    ASSERT(u8TickNum < (UINT32)0xFFFFFFFF);
    u4TickNum = (UINT32)u8TickNum;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ReadTickNum = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum;

    // This function is audio only case. We don't need this.
    #if 0
    {
        VDP_SET_PIC_INFO_T rVdpSetPicInfo;
        x_memset(&rVdpSetPicInfo, 0, sizeof(rVdpSetPicInfo));
        rVdpSetPicInfo.u4TickNum = u4TickNum;
        if (VDP_SetPicInfo(prSwdmxInst->u1B2rId, &rVdpSetPicInfo) != VDP_SET_OK)
        {
            LOG(3, "Empty: VDP_SetPicInfo u4TickNum(%d) fail\n", u4TickNum);
        }
    }
    #endif

    u4LogCnt++;
    if(u4LogCnt >= MPG_TIMER_DELAY_TIMES)
    {
        LOG(5, "Empty: CurTickNum %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4CurTickNum);
        u4LogCnt = 0;
        UNUSED(u4LogCnt);
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -1;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr > prEndAddr)
    {
        LOG(3, "Empty:  prStartAddr >= prEndAddr fail\n");
        return MPG_SEND_FAIL;
    }

    _SWDMX_LockRangeSema(u1SrcId);

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart > prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "Empty:  valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd > prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "Empty: valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        LOG(3, "Empty: invalid u4TickNum %d, %d-%d\n",
            u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        if(i4PlaySpeed >= 0)
        {
            LOG(3, "Empty: invalid u4TickNum %d->%d\n",
                u4TickNum, prValidEnd->ui4_tick_num);
            u4TickNum = prValidEnd->ui4_tick_num;
        }
        else
        {
            LOG(3, "Empty: invalid u4TickNum %d->%d\n",
                u4TickNum, prValidStart->ui4_tick_num);
            u4TickNum = prValidStart->ui4_tick_num;
        }
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "Empty: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }
    if(prTargetAddr > prEndAddr)
    {
        prTargetAddr -= ((prEndAddr +1) - prStartAddr);
    }
    if(prTargetAddr == NULL)
    {
        LOG(3, "Empty: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FAIL;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = _SWDMX_MpgLBA2Pos(u1SrcId, (prTargetAddr->ui4_lba));
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize = _SWDMX_MpgTSAlignDown(u1SrcId, prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
    if(i4PlaySpeed >= 0)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize += prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase;
    }
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;
    _SWDMX_MpgResetData(u1SrcId);

    if((SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
        (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))) &&
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime =
            (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8CurTime *=
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4TickBase;

        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

        if(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
           (PVR_TICK_INDEX_FLAG_EMPTY)))
        {
            LOG(3, "Empty: u4NoSignalStage 1.2.1 audio only, PREPARE_NO_SIGNAL tick %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum);

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_NO_SIGNAL;
        }
        else //if(SWDMX_MPG_TICK_FLAG_CHECK(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prLastTickIndex,
             //   (PVR_TICK_INDEX_FLAG_BAD)))
        {
            LOG(3, "Empty: u4NoSignalStage 1.2.2 audio only, PREPARE_BAD_STORAGE tick %d\n",
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalTickNum);

            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_BAD_STORAGE;
        }
        _SWDMX_UnlockRangeSema(u1SrcId);
        return MPG_SEND_FULL;
    }


    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA)
        &&
       (
        ((i4PlaySpeed >= 0) &&
         (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
        ||
        ((i4PlaySpeed < 0) &&
         (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr))
       )
      )
    {
        LOG(3, "Empty: fgMonitorLBA addr 0x%llx in start addr 0x%llx end addr 0x%llx\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8MonitorAddr,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize);
        if((prSwdmxInst) &&
           (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _SWDMX_RANGE_CALLBACK(
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_MONITOR_LBA_ARRIVAL,
                    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF,
                    (UINT32)u1SrcId);
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
    }

    if(prTargetAddr->ui4_tick_num >=
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick))
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "Empty: fgMonitorSafeEnd tick %d safe end tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeEndTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_OUT_OF_SAFE_RANGE_END,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
    }

    if((prTargetAddr->ui4_tick_num) <
        (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick))
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "Empty: fgMonitorSafeStart tick %d safe start tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4SafeStartTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_OUT_OF_SAFE_RANGE_START,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        }
    }
    else if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
    {
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
    }

    if(prTargetAddr->ui4_tick_num >= prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "Empty: fgMonitorValidEnd tick %d valid end tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidEndTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_REACH_VALID_RANGE_END,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    if(prTargetAddr->ui4_tick_num < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick)
    {
        if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "Empty: fgMonitorValidStart tick %d valid start tick %d\n",
                prTargetAddr->ui4_tick_num,
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4ValidStartTick);
            if((prSwdmxInst) &&
               (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _SWDMX_RANGE_CALLBACK(
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_REACH_VALID_RANGE_START,
                        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF,
                        (UINT32)u1SrcId);
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    _SWDMX_UnlockRangeSema(u1SrcId);
#endif
    return i4Ret;
}

BOOL _SWDMX_MpgResetFeederReq(UINT8 u1SrcId)          // CR [DTV00211387]  reset feeder request when comes timeout error
{
    UINT32 u4ReqCnt;
    INT32 i4Ret;
    UINT32 u4AlignSize;
    FEEDER_TRANSMIT_COND rFeederCond;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }
LOG(5,"reset Feeder Req\n");          //bill
#ifdef SWDMX_MPG_DEBUG_BUFFER
    if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0) || (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes > 0))
    {
        LOG(7, "Mpg Fd Flh UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d UAdr %d\n",
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    }
#endif

    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID += 50; // always a even value
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufAvailCnt = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4BufTotalSize = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTimes = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
    prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    //prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8ReadedPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize;

    for(u4ReqCnt = 0;u4ReqCnt < prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqNum;u4ReqCnt++)
    {
        // make it invalid, not a even value
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt] = 1;
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rBuf.au4ReqDup[u4ReqCnt] = 0;
#endif
    }

#if 1//def CC_DLNA_SUPPORT
    FeederSetReadyThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
    FeederSetUnderFlowThreshold(prSwdmxInst->eFeederSrc, MPG_DLNA_FEEDER_UNREACH);
    FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_THRESHOLD, FALSE);
#endif
    // Update read ptr to feeder
    if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else if(prSwdmxInst->rFeederInfo.u4WriteAddr)
    {
        if(FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, prSwdmxInst->rFeederInfo.u4WriteAddr) != FEEDER_E_OK)
        {
            LOG(1, "_SWDMX_MpgResetFeeder FeederUpdateReadPointer fail\n");
        }
    }
    // DDI is 4 byte alignment
   // if ( !IS_PUSH_MODE(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.e_mm_src_type) )
    {
        u4AlignSize = (prSwdmxInst->rFeederInfo.u4WriteAddr % 4);
        if(u4AlignSize > 0)
        {
            LOG(5, "Mpg Fd align u4WriteAddr %d\n",
                prSwdmxInst->rFeederInfo.u4WriteAddr);
            i4Ret = FeederSetRequest(prSwdmxInst->eFeederSrc,
                    (4-u4AlignSize),
                    (UINT64)(0),
                    &rFeederCond,
                    (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4FeederReqID-25)); // 25 is a past requestID
            if(i4Ret != FEEDER_E_OK)
            {
                LOG(1, "Mpg Fd align FeederSetRequest Fail\n");
                /* not necessary
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                if(!prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.fgGetInfo)
                {
                    _SWDMX_MpgHandleEOS();
                }
                if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
                */
                return FALSE;
            }
        }
    }
    
    return TRUE;

}

#ifdef TIME_SHIFT_SUPPORT
UINT64 _SWDMX_MpgTSAlignDown(UINT8 u1SrcId, UINT64 u8Position)
{
    UINT64 u8Remainder;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    UNUSED(_SWDMX_Div6464((u8Position - prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u4AlignBase, &u8Remainder));
        u8Position -= u8Remainder;

    return u8Position;
}

UINT64 _SWDMX_MpgLBA2Pos(UINT8 u1SrcId, UINT32 u4LBA)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);
    return (((UINT64)(u4LBA) * MPG_LBA_BLOCK_SIZE) + prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FifoOffset);
}
#endif

BOOL _SWDMX_MpgGetAudioCurrentPos(UINT8 u1SrcId)
{
        UINT32 u4DmxAvailSizeAudio;
        UINT32 u4Addr, u4Size;
        UINT32 u4BufSize;
		 BOOL fgRet = TRUE;

        SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
        SWDMX_MPG_VERIFY_NULL_ERROR(prSwdmxInst);

        #ifdef SWDMX_MPG_DMX_20
        u4DmxAvailSizeAudio = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
            DMX_PID_TYPE_ES_AUDIO,
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
        #else
#ifdef CC_DMX_PURE_AUDIO
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u1PidxAudio);
       	}
        else
#endif
        {
            u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
        }
        #endif

		if(u4DmxAvailSizeAudio > 0)
		{
		   u4DmxAvailSizeAudio += (188*4 + 32);
		}

        if(!_SWDMX_GetAudioFifo(u1SrcId, &u4Addr, &u4Size))
        {
            LOG(3, "_SWDMX_GetAudioFifo Fail\n");
            fgRet = FALSE;
        }

        u4BufSize = u4Size;
        if(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize > (UINT64)(u4BufSize - u4DmxAvailSizeAudio))
        {
            prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize - (UINT64)(u4BufSize - u4DmxAvailSizeAudio);
        }
        else
        {
          #if 0
		   if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
			   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
			   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
			   (prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
			 )
           {//prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 0;
              prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
                prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
           }
           else
		  #endif   	
           {
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
           }
        }
		 // for FR to file  begiainig DTV00090810, delete for DTV00098584
		 /******************************************************
		if(( prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.i4CurSpeed < 0)&&
			(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8SendedSize == prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8FileSize))
		{
		  #if 0
		  if((prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq) &&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
				(prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
			)
			{//prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos = 0;
			   prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
				 prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
			}
			else
		   #endif 
		   
			{
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos =
				prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8StartFileOffset;
			}
		}
		********************************************************************/
        LOG(6,"_SWDMX_MpgGet play position =0x%x\n",prSwdmxInst->rFmtInfo.rSwdmxMpgInfo.rVar.u8PlayPos);
        return fgRet;
}


