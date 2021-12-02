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
 * $RCSfile: swdmx_Ogg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_Ogg.c
 *  Software demux for Ogg - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_ogg.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define AAC_FAKE_HDR_SIZE 7
#define AAC_ALLOC_SIZE   (256)

#define MIN_READ_SIZE     (512* 1024)
#define MIN_READ_SIZE_NO_VIDEO (192* 1024)
#define OGG_BUFFER_SIZE (235 * 7 * 1024)
#define OGG_TEN_SEC_DURATION (90000 * 10)
#define OGG_ONE_MB  1024*1024
#define SWDMX_OGG_AAC_ALLOC_SIZE   (256)
#define SWDMX_OGG_AAC_FAKE_HDR_SIZE (7)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define get_min(x, y) ((x < y) ? x : y)
//#define max(x, y) ((x > y) ? x : y)

#define get_abs(x)                  (((x) >= 0) ? (x) : -(x))

#define SWDMX_Ogg_VERIFY_NULL(ptr)                                    \
    do {                                              \
        if (ptr == NULL) { \
            LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
            return; }      \
    } while (0)

#define SWDMX_OGG_VERIFY_NULL_ERROR(ptr)                                    \
    do {                                              \
        if (ptr == NULL) { \
            LOG(3, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
            return FALSE; }      \
    } while (0)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static KEY_INFO stKeyinfo;
//static BOOL fgHaveSeek = FALSE;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static SWDMX_HDR_PRS_STATE_T _SWDMX_OggRequestData(SWDMX_INFO_T* prSwdmxInst);

static SWDMX_HDR_PRS_STATE_T _SWDMX_OggParsePage(  UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream,
        UINT32 u4AvailSize,
        UINT32* pu4ConsumeSize);

static SWDMX_HDR_PRS_STATE_T _SWDMX_OggParse(  UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverKey(SWDMX_INFO_T *prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T    _SWDMX_OggMoveKey(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream);


static SWDMX_HDR_PRS_STATE_T _SWDMX_OggMoveData(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream ,
        UINT8 u1PageSegment,
        UINT8 u1StreamType,
        UINT8 u1TrackIdx,
        BOOL fgFreshPacket,
        UINT64 u8GranulAverg);


static SWDMX_HDR_PRS_STATE_T _SWDMX_OggAACHandle(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UINT8 u1AudTrick, UINT32 u4SampleSize,
        UINT32 u4Pts);



static BOOL _SWDMX_OggInitDmx(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType,
    BOOL fgEos);

static void _SWDMX_OggCalculatePts(SWDMX_INFO_T *prSwdmxInst,
                                   UINT8 u1StreamType,
                                   UINT8 u1TrackIdx,
                                   UINT64 u4GranulPos,
                                   UINT32*  pu4Pts);

static BOOL _SWDMX_OggHandleSpeedChange(SWDMX_INFO_T *prSwdmxInst, SWDMX_CUR_CMD_T* prCmd, SWDMX_RANGE_LIST_T* prRangeList);
static BOOL _SWDMX_SendOggAVEos(SWDMX_INFO_T* prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList);

static BOOL _SWDMX_SendOggEos(SWDMX_INFO_T* prSwdmxInst,
                              UINT8 u1HdrType);

static BOOL _SWDMX_OggGetSomeBytes(SWDMX_INFO_T *prSwdmxInst, UCHAR *pucBitstream,
                                   UCHAR **pucStream, UCHAR *pucData, UINT8 u1ByteNum);

static BOOL _SWDMX_OggSeek(SWDMX_INFO_T* prSwdmxInst,
                           SWDMX_RANGE_LIST_T* prRangeList,
                           UCHAR *pucBitstream,
                           UINT64 u8SeekTime,
                           UINT32 u4AvailSize, UINT32 *pu4ConsumedSize);

/*static BOOL _SWDMX_OggFindHeader(SWDMX_INFO_T* prSwdmxInst,
    UCHAR *pucBitstream, UINT32* pu4ConsumedSize);*/


static BOOL _SWDMX_OggPageHdrPrs(SWDMX_INFO_T* prSwdmxInst,
                                 UCHAR *pucBitstream, UINT64 *pu8GranulePos,
                                 UINT8 *pu1StreamType, UINT8 *pu1Segment,
                                 UINT8 *pu1TrackIdx);



static BOOL _SWDMX_OggGetDataFromFeeder(SWDMX_INFO_T* prSwdmxInst,
                                        FEEDER_REQ_DATA_T *prFeederData);

/*static BOOL OggJudgeKey(SWDMX_INFO_T* prSwdmxInst,
                                                                  UCHAR *pucBitstream, UINT32 *pu4PageSize,
                                                                  UINT32 *pu4Pts);*/

static BOOL _SWDMX_OggCalculateRepeatFrame(SWDMX_INFO_T* prSwdmxInst, UINT32 u4Pts, UINT32 *pu4TimeCount);

static BOOL _SWDMX_OggResetFeeder(SWDMX_INFO_T* prSwdmxInst);

static void _SWDMX_OggVideoWakeUp(SWDMX_INFO_T* prSwdmxInst);

static BOOL _SWDMX_OggCheckHeader(UCHAR *pucBuf);

static BOOL _SWDMX_OggCheckOggType(SWDMX_INFO_T *prSwdmxInst ,
                                   UCHAR *pucBuf, UINT8 *pu1StreamType,
                                   UINT8 *pu1TrackIdx);

static void _SWDMX_OggConvertCharToUint(UCHAR *pucBuf, UINT32* pu4Data);

static UINT32 _SWDMX_OggSeekLen(SWDMX_INFO_T*prSwdmxInst, SEEK_DERECTION eSeekDerection);

static BOOL _SWDMX_OggFindHeader(SWDMX_INFO_T* prSwdmxInst,
                                 UCHAR *pucBitstream, UINT32* pu4ConsumedSize, UINT32 u4AvailSize);

static UINT32 _SWDMX_OggGetPageSizeAndFrameNum(SWDMX_INFO_T* prSwdmxInst, UCHAR *pucBitstream,
        UINT8 u1SegNum, UINT8 *u1FrameNum);

static BOOL _SWDMX_OggFindVideoPage(SWDMX_INFO_T* prSwdmxInst,
                                    UCHAR *pucBitstream, UINT32* pu4ConsumedSize, UINT32 u4AvailSize);


static BOOL _SWDMX_OggJudgeKey(SWDMX_INFO_T* prSwdmxInst,
                               UCHAR *pucBitstream, UINT8 u1SegNum, UINT8 *u1KeyNum,
                               BOOL fgFreshPage);

static BOOL _SWDMX_OggSeekKeyPage(SWDMX_INFO_T* prSwdmxInst,
                                  SWDMX_RANGE_LIST_T* prRangeList,
                                  UCHAR *pucBitstream, UINT32 u4AvailSize,
                                  UINT32 *pu4ConsumeSize, SEEK_DERECTION eSeekDerection,
                                  UINT32 u4SeekTime);

BOOL _SWDMX_OggSetDecoderInfo(UINT8 u1SrcId,
                              UINT8 u1Type,
                              UINT32 u4Para1,
                              UINT32 u4Para2,
                              UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_INFO_T *prSwdmxOggInfo;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&(prSwdmxInst->rFmtInfo.rSwdmxOggInfo);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    if (u1Type == eSWDMX_SET_VIDEOTYPE)
    {
#if defined(CC_Ogg_MPEG1_DISABLE) && defined(CC_Ogg_MPEG2_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
#endif
#if defined(CC_Ogg_MPEG4_DISABLE)
            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
            {
                return FALSE;
            }
            else
#endif
#if defined(CC_Ogg_DX311_DISABLE)
                if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
                {
                    return FALSE;
                }
                else
#endif
#if defined(CC_Ogg_H264_DISABLE)
                    if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
                    {
                        return FALSE;
                    }
                    else
#endif
#if defined(CC_Ogg_VC1_DISABLE)
                        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
                        {
                            return FALSE;
                        }
                        else
#endif
#if defined(CC_Ogg_MJPEG_DISABLE)
                            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
                            {
                                return FALSE;
                            }
                            else
#endif
                            {
                                prSwdmxOggInfo->u4VDecType =  u4Para1;
                                return TRUE;
                            }
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType = u4Para1;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudSampleRate = u4Para2;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudChannelNs = u4Para3;
        return TRUE;
    }

    return FALSE;
}

BOOL _SWDMX_OggGetInfo(UINT8 u1SrcId,
                       UINT8 u1Type,
                       UINT32* pu4Para1,
                       UINT32* pu4Para2,
                       UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "_SWDMX_OggGetInfo u1Type = %d\n", u1Type);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    if (u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_OggGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize, u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                              (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);

        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        *pu4Para1 = 1;
    }
    return FALSE;
}

BOOL _SWDMX_OggVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    SWDMX_OGG_INFO_T *prSwdmxOggInfo;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&(prSwdmxInst->rFmtInfo.rSwdmxOggInfo);

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    rPesInfo.u4PTS = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.ucEsId = prSwdmxInst->u1SwdmxId;
    rPesInfo.u8Offset =
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidLatestOff;

    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts == 0) &&
            (! rPesInfo.fgEos))
    {
        LOG(5, "this is not a frame\n");
        return TRUE;
    }
    if (prSwdmxOggInfo->u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    // Pts is valid all the time
    rPesInfo.fgDtsValid = TRUE;

#ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
#endif

    if (rPesInfo.fgEos)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 0)
        {
            rPesInfo.fgBackward = TRUE;
        }
        rPesInfo.u4FifoStart = 0;
        rPesInfo.u4FifoEnd = 0;
        rPesInfo.u4VldReadPtr = 0;
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == ENUM_SWDMX_VDEC_H264) &&
            !rPesInfo.fgEos)
    {
        u4Addr = prPes->u4FrameAddr + 3;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

        u4Addr = prPes->u4FrameAddr + 4;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        rPesInfo.u4VldReadPtr = u4Addr;

        if (( rPesInfo.ucPicType & 0x1f) == 5)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;

        }
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG4) &&
             !rPesInfo.fgEos)
    {
        UCHAR *pucBuf;

        u4Addr = prPes->u4FrameAddr + 3;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if (pucBuf[0] == 0xB6)
        {
            u4Addr = prPes->u4FrameAddr + 4;
            if (u4Addr >= prPes->u4BufEnd)
            {
                u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
            }
            pucBuf = (UCHAR*)VIRTUAL(u4Addr);
            switch (pucBuf[0] >> 6)
            {
                case 0:
                    rPesInfo.ucPicType  = I_TYPE;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;
                    break;
                case 1: case 3:
                    rPesInfo.ucPicType  = P_TYPE;
                    break;
                case 2:
                    rPesInfo.ucPicType = B_TYPE;
                    break;
                default:
                    rPesInfo.ucPicType = UNKNOWN_TYPE;
                    break;
            }
        }
        else if ((pucBuf[0] >= 0x20) &&
                 (pucBuf[0] <= 0x2f))
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsFirstFrame)
        {
            UNUSED(VDEC_SetTotalTime(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TotalTime));
        }
        switch (prPes->u4FrameType)
        {
            case PIC_TYPE_I:
                rPesInfo.ucPicType = MPV_I_TYPE;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;
                break;

            case PIC_TYPE_P:
                rPesInfo.ucPicType = MPV_P_TYPE;
                break;

            case PIC_TYPE_B:
                rPesInfo.ucPicType = MPV_B_TYPE;
                break;

            default:
#ifndef CC_VDEC_FMT_DETECT
                ASSERT(0);
#endif
                break;
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_H263)
    {
        UCHAR *pucBuf;

        u4Addr = prPes->u4FrameAddr + 4;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        switch (pucBuf[0] & 0x02)
        {
            case 0:
                rPesInfo.ucPicType = I_TYPE;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                break;
            case 2:
                rPesInfo.ucPicType = P_TYPE;
                break;
            default:
                rPesInfo.ucPicType = UNKNOWN_TYPE;
                break;
        }
    }
    else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType == ENUM_SWDMX_VDEC_DX311)
    {
        UCHAR *pucBuf;
        //UINT32 u4Temp;

        u4Addr = prPes->u4FrameAddr;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        rPesInfo.fgDtsValid = TRUE;

        if ((pucBuf[0] >> 6) & 0x1)
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
        else
        {
            rPesInfo.ucPicType = I_TYPE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts = rPesInfo.u4PTS;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
        }
    }

    rPesInfo.u8OffsetI = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8LastIPos;
    rPesInfo.rExtra.u8PTSI = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastIPts;
    VDEC_SendEs((void*)&rPesInfo);

    return TRUE;
}


BOOL _SWDMX_OggAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    DMX_AUDIO_PES_T rPes;

    SWDMX_INFO_T *prSwdmxInst = NULL;
    if (prPes != NULL)
    {
        prSwdmxInst = (SWDMX_INFO_T*)(prPes->pvInstanceTag);
        SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    }
    else
    {
        LOG(1, "_SWDMX_OggAudioCallback prPes NULL\n");
        return FALSE;
    }
    /* if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastAudPts == 0)&&
       (!prPes->fgEOS) )
     {
         return TRUE;
     }*/
    x_memset(&rPes, 0, sizeof(rPes));
    //LOG(1, "prPes->u4Pts %x\n", prPes->u4Pts);
    x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));

    fgRet = AUD_SendAudioPes(&rPes);
    return fgRet;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_OggInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    
    UNUSED(eType);
    
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);
    // Default some values
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4PrevAStrmID = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudSampleRate = 48000;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = 0xFFFFFFFF;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts = 0xFFFFFFFF;

    x_memset((void*)&stKeyinfo, 0, sizeof(KEY_INFO));
    
    //DDI_BUF_ALIGNMENT == 16 and
    //for _dmx_mm_movedata  Meet the alignment and packet size (188 bytes) requirements.
    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC &&
            prSwdmxInst->pucAudBuf == 0)
    {
        prSwdmxInst->pucAudBuf =
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE, 16));
        if (prSwdmxInst->pucAudBuf == 0)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            ASSERT(0);
            return FALSE;
        }
    }

    DMX_SetToDecoder(TRUE);


    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgRepeatRequest = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsFirstFrame = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid = 0xFF;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid  = 0xFF;

    
    return TRUE;
}

BOOL _SWDMX_OggSetStrmID(
    UINT8 u1SrcId,
    UINT8 u1HdrType,
    UINT32 u4StrmID,
    UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_INFO_T *prSwdmxOggInfo;
    
    UNUSED(u4StrmSubID);
    
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&(prSwdmxInst->rFmtInfo.rSwdmxOggInfo);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    switch (u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxOggInfo->u4VStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxOggInfo->u4AStrmID = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_OggEnableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType,
    VOID* pfnCbFunc)
{
    //BOOL fgFindATrack;
    UINT32 i = 0;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_INFO_T *prSwdmxOggInfo;
    
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&(prSwdmxInst->rFmtInfo.rSwdmxOggInfo);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    switch (u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxOggInfo->fgEnVideo = TRUE;
            if (prSwdmxOggInfo->fgEnVDmx == FALSE)
            {
                _SWDMX_OggInitDmx(prSwdmxInst, eSWDMX_MEDIA_VIDEO, FALSE);
                prSwdmxOggInfo->fgEnVDmx = TRUE;
            }
            if (prSwdmxOggInfo->fgEnADmx == FALSE)
            {
                _SWDMX_OggInitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO, FALSE);
                prSwdmxOggInfo->fgEnADmx = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            for ( i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (i < OGG_MAX_AUDIO_TRACK); i++)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[i])
                {
                    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_ogg_id ==
                            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AStrmID)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx = i;
                        LOG(5, "Cur Track Idx = %ld\n", i);
                    }
                }
            }

            if (prSwdmxOggInfo->fgEnADmx == FALSE)
            {
                _SWDMX_OggInitDmx(prSwdmxInst, eSWDMX_MEDIA_AUDIO, FALSE);
                prSwdmxOggInfo->fgEnADmx = TRUE;
            }

            if ((prSwdmxInst->i4PlaySpeed == 1000) &&
                    prSwdmxOggInfo->fgBeginToPlay)
            {
                prSwdmxOggInfo->fgChgATrack = TRUE;
            }
            prSwdmxOggInfo->u4PrevAStrmID = prSwdmxOggInfo->u4AStrmID;
            prSwdmxOggInfo->fgEnAudio = TRUE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_OggDisableStrm(
    UINT8 u1SrcId,
    UINT8 u1StrmType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);

    /*DMX_MM_T rPid;

    rPid.fgEnable = FALSE;*/

    switch (u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo = FALSE;

            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
            {
                _SWDMX_OggVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio = FALSE;
            DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = FALSE;

            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_OggInitDmx(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType,
    BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0;
    DMX_MM_T rDmxMMInfo;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, prSwdmxInst->u1SwdmxId);

    x_memset((void*)&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        if (!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVDmx)
        {
            UINT32 u4VidFifoAddr, u4VidFifoSize;

            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

#if 0
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if ((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }
#endif
            _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &u4VidFifoAddr, &u4VidFifoSize);

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4VidFifoAddr;
            rDmxMMInfo.u4BufSize = u4VidFifoSize;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
        }

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

        switch (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType)
        {
            case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_MJPEG: case ENUM_SWDMX_VDEC_H263:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            default:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
        }

        if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
        }

        fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid, u4Flag, &rDmxMMInfo);
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT8 u1DmxPid;
        UINT32 u4AudFifoAddr, u4AudFifoSize;
        UINT32 u4AudPreFifoAddr = 0;
        UINT32 u4AudPreFifoSize = 0;
        UINT32 u4AudTrickSize = 0;
        INT32 i;

        rDmxMMInfo.fgEnable = TRUE;


        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnADmx)
        {
            DMX_SetToDecoder(TRUE);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);

            u1DmxPid = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid;

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF | DMX_MM_FLAG_INSTANCE_TAG);

            rDmxMMInfo.fgAllocBuf = FALSE;

            rDmxMMInfo.u4BufAddr = u4AudFifoAddr;
            rDmxMMInfo.u4BufSize = u4AudFifoSize;
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;

            DMX_AudHandler_SetEnable(TRUE);

            fgRet = DMX_MM_Set(u1DmxPid, u4Flag, &rDmxMMInfo);
            if (!DMX_AudHandler_Set(u1DmxPid, TRUE))
            {
                LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
            }

            if (_SWDMX_GetAudioPreFifo(prSwdmxInst->u1SwdmxId, &u4AudPreFifoAddr, &u4AudPreFifoSize))
            {
                u4AudTrickSize = u4AudPreFifoSize / AVI_DIVX_MAX_AUD_TRACK;
            }
            else
            {
                LOG(1, "%s:%d, _SWDMX_GetAudioPreFifo fail\n", __FUNCTION__, __LINE__);
            }

            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                rDmxMMInfo.fgAllocBuf = FALSE;
                rDmxMMInfo.u4BufSize = u4AudTrickSize;
                rDmxMMInfo.u4BufAddr = u4AudPreFifoAddr;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i] =
                    DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

                DMX_AudHandler_SetQSize(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i], 480);
                fgRet = DMX_MM_Set( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i], u4Flag, &rDmxMMInfo);
                if (!DMX_AudHandler_Set( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i], FALSE))
                {
                    LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
                }
                u4AudPreFifoAddr = u4AudPreFifoAddr + u4AudTrickSize;
            }
            DMX_AudHandler_SetActivePidx( DMX_AUDIO_HANDLER_NULL_PIDX);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = FALSE;
        }
        else if (fgEos)
        {
            INT32 i;

            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            for (i = 0; i < AVI_DIVX_MAX_AUD_TRACK; i++)
            {
                fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i], u4Flag, &rDmxMMInfo);
            }
        }
    }

    return fgRet;
}

/**
*/
VOID _SWDMX_OggSetRange(UINT8 u1SrcId,
                        SWDMX_RANGE_LIST_T *prRangeList,
                        UINT32 u4VidStrmID,
                        UINT32 u4AudStrmID,
                        UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl;
    MM_RANGE_DECODER_T* pDecodrtInfo;
    
    SWDMX_Ogg_VERIFY_NULL(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, u1SrcId);
    if (prRangeList != NULL &&
            prRangeList->prDmxRangeInfo != NULL)
    {
        u1ATrack = 0;
        pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                    u1ATrack < OGG_MAX_AUDIO_TRACK)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_OggSetRange Audio Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_OggSetRange Video Index table = 0x%08x\n",
                    (UINT32)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl));
            }
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;

            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.ui8_data_offset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz  = prRangeList->prDmxRangeInfo->z_range_sz;
        }
        // Set the number of total audio tracks
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack = u1ATrack;
        if(prRangeList->prDmxRangeInfo->pt_aud_decoder_info&&prRangeList->prDmxRangeInfo->pt_aud_decoder_info->b_has_vorbis_info)
        {
            pDecodrtInfo = (MM_RANGE_DECODER_T*)prRangeList->prDmxRangeInfo->pt_aud_decoder_info;
            u1ATrack = 0;
            while (pDecodrtInfo != NULL)
            {
                if (u1ATrack < OGG_MAX_AUDIO_TRACK)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudVorbisOffset[u1ATrack] = pDecodrtInfo->t_audio_vorbis_offset;
                    u1ATrack++;
                }
                pDecodrtInfo = (MM_RANGE_DECODER_T*)pDecodrtInfo->pt_next;
            }
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TotalTime = prRangeList->prDmxRangeInfo->ui8_pb_duration;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
        LOG(1, "Demuxer Range is NULL\n");
    }
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_OggHdrPrs(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    if ((!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgBeginToPlay) &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info != 0))
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
                FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_start_pos.ui8_i_frame_position;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_start_pos.ui8_i_frame_position;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FileOffset =
            prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
        {
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info);

            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = TRUE;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts =
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_start_pos.ui8_pts_info;
            DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[0]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;
        }


        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prDmxRangeInfo->t_vid_start_render_pts - 45000);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgBeginToPlay = TRUE;
    }
    if ((!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgBeginToPlay) &&
            (prSwdmxInst->i4PlaySpeed == 1000))
    {
        if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo) &&
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio) &&
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl) &&
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < OGG_MAX_AUDIO_TRACK) &&
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx]))
        {
            AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);

            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = 0;
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = TRUE;
            DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[0]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;
        }
        // Video only
        else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo && prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = TRUE;
        }
        // Audio only
        else if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < OGG_MAX_AUDIO_TRACK) &&
                 (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx]))
        {
            AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);
            DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[0]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;
        }
        else
        {
            LOG(1, "SWDMX_Ogg Unknow state for starting playback\n");
            return eSWDMX_HDR_PRS_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgBeginToPlay = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed  = 1000;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgParseHeadPacket = FALSE;
    }
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort = FALSE;
    if (prSwdmxInst->fgPlayChange)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFindKey = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFirstFrame = FALSE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos = 0;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = 0xFFFFFFFF;
        _SWDMX_OggHandleSpeedChange(prSwdmxInst, prCmd, prRangeList);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed = prSwdmxInst->i4PlaySpeed;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek)
    {
        BOOL fgFindKey = FALSE;
        UINT32 u4SeekTime = 0;
        UINT32 u4ConsumedSize = 0;

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts != 0xFFFFFFFF)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts > 10*90000)
            {
                u4SeekTime = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts - 10 * 90000;
            }
        }
        fgFindKey = _SWDMX_OggSeekKeyPage(prSwdmxInst, prRangeList,
                                          (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr),
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder,
                                          &u4ConsumedSize, SEEK_KEY, u4SeekTime);

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid)
        {
            _SWDMX_OggResetFeeder(prSwdmxInst);
            u4ConsumedSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4FeederReqID++;
        }

        if (u4ConsumedSize != 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -=  u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
            if ((UINT32) prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                    ((UINT32) prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr -
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
            }
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < OGG_NORMAL_PAGE_SIZE)
        {
            if (_SWDMX_OggRequestData(prSwdmxInst) == eSWDMX_HDR_PRS_FED_MOVE_FAIL)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

        if (!fgFindKey && prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekDone)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NO_INDEX;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek ;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekError = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekDone = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek = FALSE;

            STC_StartStc(prSwdmxInst->u1StcId);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = 0;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts = 0xFFFFFFFF;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
            {
                AUD_WaitDspFlush(INTERNAL_DAC_ID_MAIN);
            }
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        else if (fgFindKey)
        {
            UINT32 u1Idx = 0;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVDmx)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, FALSE);

                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                VDP_ResetPts();
            }

            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
                AUD_WaitDspFlush(INTERNAL_DAC_ID_MAIN);
            }

            VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);

            for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < OGG_MAX_AUDIO_TRACK); u1Idx++)
            {
                DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx]);
            }
            if (prSwdmxInst->i4PlaySpeed > 0)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts + 2000);
                }
            }
            else
            {
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts = 0xFFFFFFFF;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekDone = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek = FALSE;
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        else
        {
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    if ((prSwdmxInst->i4PlaySpeed > 2000 || prSwdmxInst->i4PlaySpeed < 0 ) &&
         ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS) &&
           prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo))
    {
        return _SWDMX_DeliverKey(prSwdmxInst, prRangeList);
    }
    else
    {
        if (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_NO_INDEX)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        }
        return  _SWDMX_OggParse(u1SrcId, prRangeList);
    }
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverKey(SWDMX_INFO_T *prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL    fgFindKey = FALSE;
    UINT32  u4ConsumedSize = 0;

    if (_SWDMX_OggRequestData(prSwdmxInst) !=  eSWDMX_HDR_PRS_SUCCEED)
    {
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if ((prSwdmxInst->i4PlaySpeed > 2000) &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount == 0) &&
            !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos)
    {

        fgFindKey = _SWDMX_OggSeekKeyPage(prSwdmxInst, prRangeList,
                                          (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr),
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder,
                                          &u4ConsumedSize, SEEK_FORWARD, 0);

        if (u4ConsumedSize != 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr -
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            }
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
        {
            _SWDMX_SendOggAVEos(prSwdmxInst, prRangeList);
        }

        if (!fgFindKey)
        {
            //prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NO_INDEX;
            // prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
            //prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek ;
            //prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek = 0;
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

    }
    if ((prSwdmxInst->i4PlaySpeed  < 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount == 0) &&
            !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendOver)
        {
            _SWDMX_SendOggAVEos(prSwdmxInst, prRangeList);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFirstFrame = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendOver = FALSE;
        }

        fgFindKey = _SWDMX_OggSeekKeyPage(prSwdmxInst, prRangeList,
                                          (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr),
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder,
                                          &u4ConsumedSize, SEEK_BACKWARD, 0);
        if (u4ConsumedSize != 0)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr -
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            }
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));
        }

        if (!fgFindKey)
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
    }
    if (fgFindKey ||
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount > 0))
    {
        return  _SWDMX_OggMoveKey(prSwdmxInst, prRangeList,
                                  (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr) );
    }
    else
    {
        return eSWDMX_HDR_PRS_SUCCEED;
    }
}

static SWDMX_HDR_PRS_STATE_T    _SWDMX_OggMoveKey(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream)
{
    BOOL fgFrechPage = FALSE;
    BOOL fgIsKeyFrame = FALSE;
    BOOL fgSendDone = FALSE;

    UCHAR *pucStream = pucBitstream;
    UCHAR *pucSegBits = NULL;
    UCHAR *pucBitNext = NULL;
    UCHAR ucData[4] = {0};

    UINT64 u8GranulePos = 0;

    UINT8 u1StreamType = 0;
    UINT8 u1PageSegment = 0;
    UINT8 u1TrackIdx = 0;
    UINT8 u1FrameCount = 0;
    UINT8 u1FrameKey = 0;

    UINT32 u4PayLoadSize = 0;
    UINT32 u4PageSize = 0;
    UINT32 u4Pts = 0;
    UINT32 u4RepeatCount = 0;
    UINT32 u4KeyFrameSize = 0;
    UINT32 u4AvailByte = 0;
    UINT32 u4ConsumeSize = 0;
    UINT32 u4DmxAvailSize = 0;

    DMX_MM_DATA_T rDmxMMData;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucStream, ucData, 4);

    if (!_SWDMX_OggCheckHeader(ucData) )
    {
        return eSWDMX_HDR_PRS_FAIL;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo &&
            (u1StreamType == eSWDMX_MEDIA_VIDEO))
    {
        UINT16 u2CurQNum = 0;
        UINT16 u2MaxQNum = 0;
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        if (u2CurQNum > (25*3))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
            {
                _SWDMX_OggVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            x_thread_delay(20);
		    prSwdmxInst->fgFifoFull = TRUE;
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    fgFrechPage =  _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,  &u8GranulePos,
                                        &u1StreamType, &u1PageSegment, &u1TrackIdx);
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;

    u4PageSize = OGG_HEADER_SIZE + u1PageSegment;
    pucStream  = pucBitstream + OGG_HEADER_SIZE;
    pucSegBits = pucStream + u1PageSegment;
    if ((UINT32)(pucSegBits) >=
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        pucSegBits =
            (UCHAR*)((UINT32)pucSegBits -
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
    }

    if (fgFrechPage)
    {
        if (*pucSegBits == 0x08)
        {
            fgIsKeyFrame = TRUE;
        }
    }

    while (u1PageSegment > 0)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);
        u1PageSegment--;
        if (((UINT8)ucData[0] == 0xff) && (u1PageSegment != 0))
        {
            u4PayLoadSize = u4PayLoadSize + (UINT8)ucData[0];
            u4PageSize += (UINT8)ucData[0];
        }
        else
        {
            u4PayLoadSize = u4PayLoadSize + (UINT8)ucData[0];
            u4PageSize += (UINT8)ucData[0];
            u1FrameCount ++;
            if (!fgIsKeyFrame )
            {
                pucSegBits += u4PayLoadSize;
                if ((UINT32)(pucSegBits) >=
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    pucSegBits =
                        (UCHAR*)((UINT32)pucSegBits -
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
                if (*pucSegBits == 0x08)
                {
                    u1FrameKey = u1FrameCount;
                    fgIsKeyFrame = TRUE;
                }
            }
            else
            {
                if (u4KeyFrameSize == 0)
                {
                    u4KeyFrameSize = u4PayLoadSize;
                }
            }

            u4PayLoadSize = 0;
        }
    }
    if (u4KeyFrameSize == 0)
    {
        u4KeyFrameSize = u4PayLoadSize;
    }

    if (((UINT8)ucData[0] == 0xff) && (u1PageSegment == 0))
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendKeyDone = FALSE;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendKeyDone = TRUE;
    }


    u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                          (DMX_PID_TYPE_T)eSWDMX_MEDIA_VIDEO,
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);


    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos > u1FrameCount)
    {
        u8GranulePos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos - u1FrameCount + u1FrameKey;
    }

    u8GranulePos++;
    _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, u8GranulePos, &u4Pts);

    if ( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount == 0)
    {
        _SWDMX_OggCalculateRepeatFrame(prSwdmxInst, u4Pts, &u4RepeatCount);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount = u4RepeatCount;
    }

    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));

    if (u4DmxAvailSize <= (u4KeyFrameSize + OGG_ONE_MB / 2))
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
        {
            _SWDMX_OggVideoWakeUp(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
        }
        x_thread_delay(20);
		prSwdmxInst->fgFifoFull = TRUE;
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }


    rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid;

    rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
    rDmxMMData.u4StartAddr = (UINT32)pucSegBits + 1;
    rDmxMMData.u4FrameSize = u4KeyFrameSize - 1;
    rDmxMMData.u4Pts = u4Pts;
    rDmxMMData.u4Dts = u4Pts;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts = u4Pts;

    LOG(7, "Type = %ld Size = 0x%08x pts = 0x%08x\n",
        u1StreamType, u4KeyFrameSize, u4Pts);
    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
    {
        LOG(0, "Demuxer fails to move data.\n");
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }
    u4AvailByte = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder;

    u4AvailByte -= u4PageSize;
    u4ConsumeSize += u4PageSize;
    u4KeyFrameSize = 0;

    if (!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendKeyDone )
    {
        pucBitNext = pucBitstream;

        while (TRUE)
        {
            pucBitNext += u4PageSize;
            if ((UINT32)(pucBitNext) >=
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                pucBitNext =
                    (UCHAR*)((UINT32)pucBitNext -
                             VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                             VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
            }
            pucStream = pucBitNext;
            if (u4AvailByte < OGG_HEADER_SIZE + 0xFF)
            {
                _SWDMX_OggRequestData(prSwdmxInst);
                u4AvailByte += MIN_READ_SIZE;
            }
            _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitNext, &pucStream, ucData, 4);
            if (!_SWDMX_OggCheckHeader(ucData) )
            {
                return eSWDMX_HDR_PRS_FAIL;
            }
            fgFrechPage =  _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,  &u8GranulePos,
                                                &u1StreamType, &u1PageSegment, &u1TrackIdx);


            u4PageSize = OGG_HEADER_SIZE + u1PageSegment;
            pucStream  = pucBitNext + OGG_HEADER_SIZE;
            if (u1StreamType != eSWDMX_MEDIA_VIDEO)
            {
                while (u1PageSegment > 0)
                {
                    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);
                    u4PageSize +=  (UINT32)(ucData[0]);
                    u1PageSegment --;
                }
                if (u4AvailByte < u4PageSize )
                {
                    _SWDMX_OggRequestData(prSwdmxInst);
                    u4AvailByte += MIN_READ_SIZE;
                }
                u4AvailByte -= u4PageSize;
                u4ConsumeSize += u4PageSize;

                continue;
            }
            else
            {
                if (fgFrechPage)
                {
                    break;
                }
                else
                {
                    pucStream  = pucBitNext + OGG_HEADER_SIZE;
                    pucSegBits = pucStream + u1PageSegment;
                    u4PageSize =  u1PageSegment + OGG_HEADER_SIZE;
                    if ((UINT32)(pucSegBits) >=
                            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucSegBits =
                            (UCHAR*)((UINT32)pucSegBits -
                                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    }

                    while (u1PageSegment > 0)
                    {
                        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);
                        u1PageSegment--;
                        if ((UINT8)ucData[0] == 0xff)
                        {
                            u4PayLoadSize = u4PayLoadSize + (UINT8)ucData[0];
                            u4PageSize += (UINT8)ucData[0];
                        }
                        else
                        {
                            u4PayLoadSize = u4PayLoadSize + (UINT8)ucData[0];
                            u4PageSize += (UINT8)ucData[0];
                            fgSendDone = TRUE;
                            if (u4KeyFrameSize == 0)
                            {
                                u4KeyFrameSize = u4PayLoadSize;
                            }
                            u4PayLoadSize = 0;
                        }
                    }
                    if (u4KeyFrameSize == 0)
                    {
                        u4KeyFrameSize = u4PayLoadSize;
                    }

                    if (u4AvailByte < u4PageSize )
                    {
                        _SWDMX_OggRequestData(prSwdmxInst);
                        u4AvailByte += MIN_READ_SIZE;
                    }

                    rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                    rDmxMMData.u4StartAddr = (UINT32)pucSegBits;
                    rDmxMMData.u4FrameSize = u4KeyFrameSize;
                    rDmxMMData.u4Pts = u4Pts;
                    rDmxMMData.u4Dts = u4Pts;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts =  0;

                    if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                    {
                        LOG(0, "Demuxer fails to move data.\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                }

                if (ucData[0] != 0xFF)
                {
                    fgSendDone = TRUE;
                }

                if (fgSendDone)
                {
                    break;
                }
                if (u4AvailByte < u4PageSize )
                {
                    _SWDMX_OggRequestData(prSwdmxInst);
                    u4AvailByte += MIN_READ_SIZE;
                }
                u4AvailByte -= u4PageSize;
                u4ConsumeSize += u4PageSize;
            }
        }
    }

    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount --;

    if ( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount == 0)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFirstFrame)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSendOver = TRUE;
        }

        if (pucBitNext != NULL)
        {
            pucBitNext = (UCHAR*)((UINT32)pucBitNext + u4PageSize);
        }
        else
        {
            pucBitNext = (UCHAR*)((UINT32)pucBitstream + u4PageSize);
        }
        if ((UINT32)(pucBitNext) >=
                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            pucBitNext =
                (UCHAR*)((UINT32)pucBitNext -
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
        }

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4ConsumeSize;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumeSize;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = (UINT32)pucBitNext;
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = (UINT32)pucBitstream;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}



static SWDMX_HDR_PRS_STATE_T _SWDMX_OggParse(  UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = 0;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos == 0)
    {
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, &prSwdmxInst->rFeederInfo) !=
                FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
            prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
            prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FileOffset =
            prRangeList->prDmxRangeInfo->ui8_fileoffset;
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos = 0;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos = 0;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FileOffset = 0;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >= prRangeList->prDmxRangeInfo->z_range_sz)
    {
        _SWDMX_SendOggAVEos(prSwdmxInst, prRangeList);
    }

    if (prRangeList->prDmxRangeInfo->z_range_sz > prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid)
        {
            _SWDMX_OggResetFeeder(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = FALSE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4FeederReqID++;
        }
        if (_SWDMX_OggRequestData(prSwdmxInst) == eSWDMX_HDR_PRS_FED_MOVE_FAIL)
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder > 0)
    {
        u4PrsState = _SWDMX_OggParsePage(u1SrcId, prRangeList,
                                         (UCHAR*)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr),
                                         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder, &u4ConsumedSize);

        switch (u4PrsState)
        {
            case eSWDMX_HDR_PRS_FAIL:
                if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                        (prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset)) ||
                        (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos > prRangeList->prDmxRangeInfo->ui8_fileoffset + OGG_ONE_MB))
                {
                    LOG(3, "No dts chunks was discovered\n");
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder = 0;
                    return eSWDMX_HDR_PRS_FAIL;
                }
                break;
            default:
                break;
        }
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
        //prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos += u4ConsumedSize;
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder = 0;
    }

    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >=
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
            VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr) +
            ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -
             VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
    }
    else
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_OggParsePage(  UINT8 u1SrcId,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream,
        UINT32 u4AvailSize,
        UINT32* pu4ConsumeSize)
{
    BOOL fgFindPage     = FALSE;
    BOOL fgLostPage     = FALSE;
    BOOL fgFreshPacket = FALSE;

    UINT8 u1TempSegment = 0;
    UINT8 u1PageSegment = 0;
    UINT8 u1StreamType = 0;
    UINT8 u1TrackIdx = 0;
    UINT8 u1PidIdx = 0;
    UINT8 u1PidType = 0;
    UINT8 u1FrameCount = 0;
    UINT16 u2CurQNum = 0;
    UINT16 u2MaxQNum = 0;

    UINT32 u4PageSize = 0;
    UINT32 i = 0;
    UINT32 u4PageSeqNum = 0;
    UINT32 u4PrsState = 0;
    UINT32 u4DmxAvailSize = 0;
    UINT64 u8GranulAverg = 0;

    UINT64 u8GranulePos = 0;

    UCHAR *pucStream = pucBitstream;
    UCHAR *pucSegBits = NULL;

    UCHAR ucData[4] = {0};
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    while (i < u4AvailSize)
    {
        pucStream = pucBitstream;

        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucStream, ucData, 4);

        if (_SWDMX_OggCheckHeader(ucData) == TRUE)
        {
            fgFindPage = TRUE;
            break;
        }

        i++;
        pucBitstream++;
        if ((UINT32)pucBitstream >=
                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            pucBitstream =
                (UCHAR*)((UINT32)pucBitstream -
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
        }
    }

    // Didn't find the Page
    if (!fgFindPage)
    {
        x_thread_delay(10);
        *pu4ConsumeSize = i;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if ((u4AvailSize -i) < OGG_HEADER_SIZE)
    {
        LOG(3, "left in Feeder byte number is small than header size ");
        *pu4ConsumeSize = i;
        return eSWDMX_HDR_PRS_SUCCEED;
    }
    fgFreshPacket = _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,  &u8GranulePos,
                                         &u1StreamType, &u1PageSegment, &u1TrackIdx);
    pucSegBits = pucBitstream + OGG_HEADER_SIZE;

    fgLostPage =
        (++prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4PageSeqNum)  == u4PageSeqNum ?  FALSE : TRUE ;

    if (fgLostPage)
    {
        //LOG(3, "there is a packet lost %d\n",(1+u4PageSeqNum - prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4PageSeqNum));
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo &&
            (u1StreamType == eSWDMX_MEDIA_VIDEO))
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        if (u2CurQNum > ( 25*3))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
            {
                _SWDMX_OggVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;
		    prSwdmxInst->fgFifoFull = TRUE;

            x_thread_delay(20);

            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }
    if (u4AvailSize < u1PageSegment)
    {
        LOG(3, "left in Feeder byte number is small than header size ");
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    u1TempSegment = u1PageSegment;

    pucStream = pucBitstream + OGG_HEADER_SIZE;

    if ((UINT32)pucStream >=
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        pucStream =
            (UCHAR*)((UINT32)pucStream -
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
    }

    while (u1TempSegment > 0)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);

        u4PageSize = u4PageSize +  (UINT8)ucData[0];
        if ((UINT8)ucData[0] != 0xFF)
        {
            u1FrameCount ++;
        }
        u1TempSegment --;
    }

    if (ucData[0] == 0xFF)
    {
        u1FrameCount ++;
    }

    u4PageSize = u4PageSize + u1PageSegment + OGG_HEADER_SIZE;

    if (u4AvailSize < u4PageSize)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos + u4AvailSize >=
                prRangeList->prDmxRangeInfo->z_range_sz)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                prRangeList->prDmxRangeInfo->z_range_sz;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder = 0;
        }
        LOG(3, "left in Feeder byte number is small than header size ");
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if ((eSWDMX_MEDIA_AUDIO == u1StreamType) ||
            (eSWDMX_MEDIA_SKIPPED_AUDIO == u1StreamType))
    {
        UINT32 u4Pts = 0;
        UINT64 u8remander = 0;

        if (u1FrameCount > 0)
        {
            // u8GranulAverg = (u8GranulePos - prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos)/u1FrameCount;
            if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
            {
                u8GranulAverg = u8Div6432((u8GranulePos - prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx]), u1FrameCount, &u8remander);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] += u8GranulAverg;
            }
        }
        else
        {
            if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] = u8GranulePos;
            }
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts != 0xFFFFFFFF)
        {
            if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
            {
                _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx], &u4Pts);
            }
            if ((u4Pts < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts) ||
                    (u1TrackIdx != prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastAudPts = u4Pts;
                u1StreamType = eSWDMX_MEDIA_SKIPPED;
                if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] = u8GranulePos;
                }
            }
            else
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
                {
                    AUD_DSPCmdPlay(0);
                }
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4Pts;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = 0xFFFFFFFF;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);

            }
        }
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgChgATrack && (eSWDMX_MEDIA_AUDIO == u1StreamType))
        {
            if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgChgATrack) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
            {
                BOOL fgRet = FALSE;
                UINT32 u4APts = 0;
                UINT32 u4CurPts = 0;
                if (u4Pts == 0)
                {
                    _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx], &u4Pts);
                }
                //  if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
                {
                    DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
                }
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgChgATrack = FALSE;

                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos = FALSE;
                    AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
                }
                //if (!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgRecSpeedChangeForAud)
                {
                    u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4CurPts + 27000;

                    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack)
                    {
                        fgRet = DMX_AudHandler_SeekAndUpdate(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx],
                                                             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts, &u4APts);
                    }

                    LOG(5, "Change Audio Track\n");

                    if (fgRet)
                    {
                        LOG(5, "Find audio in the pre-buffer\n");
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4APts;
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts);
                    }
                    else
                    {
                        LOG(5, "Can't find audio in the pre-buffer\n");
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts = u4Pts;
                    }
                    if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS))
                    {
                        DMX_AudHandler_SetActivePidxEx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx], 
                            (UINT8*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHeaderPtr[u1TrackIdx],
                            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHdrLength[u1TrackIdx]);
                    }
                    else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack)
                    {
                        DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx]);
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;
                    }
                    LOG(5, "prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = 0x%08x\n", prSwdmxInst->rFmtInfo.rSwdmxAviInfo.u4TriggerAudPts);
                }
            }
        }
        if ( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekError)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
            {
                AUD_DSPCmdPlay(0);
            }
            _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, u8GranulePos, &u4Pts);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4Pts;
            AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekError = FALSE;
        }
    }
    else
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;
        }
    }

    if (eSWDMX_MEDIA_SKIPPED == u1StreamType)
    {
        *pu4ConsumeSize = u4PageSize + i;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if ((u1StreamType == eSWDMX_MEDIA_VIDEO) ||
       (((u1StreamType == eSWDMX_MEDIA_AUDIO)) && 
             (prSwdmxInst->i4PlaySpeed  == SWDMX_PLAY_SPEED_1X)))
    {
        u1PidIdx = (u1StreamType == eSWDMX_MEDIA_VIDEO) ? (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid) :
                   (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
        u1PidType = (u1StreamType == eSWDMX_MEDIA_VIDEO) ? (DMX_PID_TYPE_ES_VIDEO) : (DMX_PID_TYPE_ES_AUDIO);

        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                              (DMX_PID_TYPE_T)u1PidType,
                                              u1PidIdx);

        if (u4DmxAvailSize <= u4PageSize)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
            {
                _SWDMX_OggVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
            }

            if (((u1StreamType == eSWDMX_MEDIA_AUDIO) || (u1StreamType == eSWDMX_MEDIA_SKIPPED_AUDIO)) &&
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgAudIsReady)
            {
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                STC_StartStc(prSwdmxInst->u1StcId);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgAudIsReady = FALSE;
            }

            LOG(3, "blocked u1StreamType = %ld, u4PageSize = %ld, u4DmxAvailSize = %ld\n",
                u1StreamType,
                u4PageSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = i;
            x_thread_delay(5);
			prSwdmxInst->fgFifoFull = TRUE;
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }
    }
    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS)&&
        ((u1StreamType == eSWDMX_MEDIA_AUDIO) || 
        (u1StreamType == eSWDMX_MEDIA_SKIPPED_AUDIO)))
    {
         DMX_MM_DATA_T rDmxMMData;
         if(!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
         {
             // save vorbis 0x1 identification header packet,this packe is in bos page,save ogg header
             if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgParseHeadPacket &&
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudVorbisOffset[u1TrackIdx].ui4_vorbis_header_offset < prRangeList->prDmxRangeInfo->ui8_fileoffset) ||
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos < prRangeList->prDmxRangeInfo->ui8_fileoffset))
             {
                 UINT8 *pucTgtBuf;
                 UINT32 u4Addr, u4Size, u4Align;
                 SWDMX_OGG_INFO_T *prSwdmxOggInfo = NULL;
                 SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
                 prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxOggInfo;
                 u4Align = 16;
                 u4Size = prRangeList->prDmxRangeInfo->ui8_fileoffset;
                 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgParseHeadPacket )
                {
                     if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
                         || u4Addr == 0)
                     {
                         LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
                         ASSERT(0);
                         return eSWDMX_HDR_PRS_FAIL;
                     }
                     prSwdmxOggInfo->u4AudHeaderPtr[u1TrackIdx] = u4Addr;
                 }
                 u4Size = u1PageSegment + OGG_HEADER_SIZE;
                 pucTgtBuf = (UINT8 *)prSwdmxOggInfo->u4AudHeaderPtr[u1TrackIdx] + prSwdmxOggInfo->u4AudHdrLength[u1TrackIdx];
                 x_memcpy((VOID *)pucTgtBuf, pucBitstream, u4Size);
                 //HalFlushDCacheMultipleLine(u4Addr, u4Size);
                 prSwdmxOggInfo->u4AudHdrLength[u1TrackIdx] += u4Size;
                 
             }
         }
         x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
         if(u1TrackIdx < OGG_MAX_AUDIO_TRACK)
         {
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1TrackIdx];
         }
         rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
         rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
         rDmxMMData.u4StartAddr = (UINT32)pucBitstream;
         rDmxMMData.u4FrameSize = u1PageSegment + OGG_HEADER_SIZE;
         rDmxMMData.u4Pts = 1;
         rDmxMMData.u4Dts = 1;
         if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
         {
             LOG(0, "Demuxer fails to move data.\n");
             return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
         }
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgParseHeadPacket = FALSE;
    }



    u4PrsState =  _SWDMX_OggMoveData(prSwdmxInst, prRangeList, pucSegBits, u1PageSegment, u1StreamType, u1TrackIdx, fgFreshPacket, u8GranulAverg);

    if (u4PrsState == eSWDMX_HDR_PRS_SUCCEED)
    {
        if (u1StreamType == eSWDMX_MEDIA_VIDEO)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;
        }
        else
        {
            if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] = u8GranulePos;
            }
        }
        *pu4ConsumeSize = u4PageSize + i;
    }
    else
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T    _SWDMX_OggMoveData(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UCHAR* pucBitstream,
        UINT8 u1PageSegment,
        UINT8 u1StreamType,
        UINT8 u1TrackIdx,
        BOOL fgFreshPacket,
        UINT64 u8GranulAverg)

{
    UCHAR ucData[4] = {0};
    UCHAR*pucStream = pucBitstream;
    UCHAR *pucBitsSeg = NULL;
    UINT32 u4Pts = 0;
    UINT32 u4PacketSize = 0;
    UINT32 u4ConsumedSize    = 0;
    UINT32 u4LenBytes = 0;

    DMX_MM_DATA_T rDmxMMData;

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    pucBitsSeg = pucStream + u1PageSegment;

    if ((UINT32)pucBitsSeg >=
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        pucBitsSeg =
            (UCHAR*)((UINT32)pucBitsSeg -
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
    }

    while (u1PageSegment > 0)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);
        u1PageSegment--;
        if (((UINT8)ucData[0] == 0xff) && (u1PageSegment != 0))
        {
            u4PacketSize = u4PacketSize + (UINT8)ucData[0];
        }
        else
        {
            u4PacketSize = u4PacketSize + (UINT8)ucData[0];

            if (fgFreshPacket == FALSE)
            {
                pucBitsSeg = pucBitsSeg + u4ConsumedSize ;
            }
            else
            {
                pucBitsSeg = pucBitsSeg + u4ConsumedSize;

                if ((UINT32)pucBitsSeg >=
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    pucBitsSeg =
                        (UCHAR*)((UINT32)pucBitsSeg -
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
                u4LenBytes = (((*pucBitsSeg) & (0x3)) << 1) | (((*pucBitsSeg) & 0x40) >> 6);
                if (u4LenBytes != 0)
                {
                    LOG(7, "***********************%d**\n", u4LenBytes);
                }

                if(((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS)&&
                    (u1StreamType == eSWDMX_MEDIA_AUDIO)) ||
                    (u1StreamType == eSWDMX_MEDIA_VIDEO))
                {
                    pucBitsSeg = pucBitsSeg + u4LenBytes + 1;
                    if (u4PacketSize > (u4LenBytes + 1))
                    {
                        u4PacketSize = u4PacketSize - (u4LenBytes + 1);
                    }
                }
            }

            if (u1StreamType != eSWDMX_MEDIA_SKIPPED)
            {
                UINT32 u8GranulPos = 0;

                if ((u1StreamType == eSWDMX_MEDIA_AUDIO) ||
                        (u1StreamType == eSWDMX_MEDIA_SKIPPED_AUDIO))
                {
                    if (fgFreshPacket)
                    {
                        if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
                        {
                            u8GranulPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] ;
                            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] += u8GranulAverg;
                        }
                        _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, u8GranulPos, &u4Pts);
                        if(prSwdmxInst->fgPlayChange && prSwdmxInst->i4PlaySpeed == 1000)
                        {
                            AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);
                        }
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastAudPts = u4Pts;
                        if(((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS)&&
                            (u1StreamType == eSWDMX_MEDIA_AUDIO)) ||
                            (u1StreamType == eSWDMX_MEDIA_VIDEO))
                        {
                            if (u4PacketSize > 2)
                            {
                                pucBitsSeg = pucBitsSeg + 2;
                                u4PacketSize -= 2;
                            }
                        }
                    }
                    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC)
                    {
                        _SWDMX_OggAACHandle(prSwdmxInst, prRangeList, u1TrackIdx,
                                            u4PacketSize, u4Pts);
                    }
                    if (fgFreshPacket == FALSE)
                    {
                        u4Pts = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastAudPts;
                        if (u4PacketSize == 0)
                        {
                            fgFreshPacket = TRUE;
                            continue;
                        }
                    }

                    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts != 0)
                    {
                        if (!fgFreshPacket)
                        {
                            fgFreshPacket = TRUE;
                            u4ConsumedSize = u4PacketSize;
                            u4PacketSize = 0;
                            continue;
                        }
                        else
                        {
                            if (u4Pts > prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts)
                            {
                                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4Pts;
                                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);
                                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts = 0;
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                }
                else
                {
                    if (fgFreshPacket)
                    {
                        u8GranulPos = ++prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos;
                        _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, u1TrackIdx, u8GranulPos, &u4Pts);
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts = u4Pts;
                    }
                    if (fgFreshPacket == FALSE)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LastVidPts = 0;
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos++;
                        if (u4PacketSize == 0)
                        {
                            fgFreshPacket = TRUE;
                            continue;
                        }
                    }
                }

                if ((UINT32)pucBitsSeg >=
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    pucBitsSeg =
                        (UCHAR*)((UINT32)pucBitsSeg -
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
                if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange) &&
                        (fgFreshPacket == FALSE))
                {
                    fgFreshPacket = TRUE;
                    u4ConsumedSize = u4PacketSize;
                    u4PacketSize = 0;
                    continue;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange = FALSE;
                }

                if (u1StreamType == eSWDMX_MEDIA_VIDEO)
                {
                    if ((u4PacketSize < 0x20) &&
                            ( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos ==
                              prRangeList->prDmxRangeInfo->ui8_fileoffset))
                    {
                        u4ConsumedSize = u4PacketSize;
                        u4PacketSize = 0;
                        continue;
                    }
                }

                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange = FALSE;
                if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS) && 
                     (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudVorbisOffset[u1TrackIdx].ui4_setup_offset < prRangeList->prDmxRangeInfo->ui8_fileoffset) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudVorbisOffset[u1TrackIdx].ui4_comment_offset < prRangeList->prDmxRangeInfo->ui8_fileoffset) &&
                     (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos < prRangeList->prDmxRangeInfo->ui8_fileoffset))
                {
                    if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack == 1) &&
                        (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHeaderPtr[u1TrackIdx]!=0))
                    {
                        UINT8 *pucTgtBuf;
                        pucTgtBuf = (UINT8 *)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHeaderPtr[u1TrackIdx]; 
                        pucTgtBuf += prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHdrLength[u1TrackIdx];
                        x_memcpy((VOID *)pucTgtBuf, pucBitsSeg, u4PacketSize);
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHdrLength[u1TrackIdx] += u4PacketSize;

                    }
                }

                switch (u1StreamType)
                {
                    case eSWDMX_MEDIA_VIDEO:
                        rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid;
                        break;
                    case eSWDMX_MEDIA_AUDIO:
                    case eSWDMX_MEDIA_SKIPPED_AUDIO:
                        if (u1TrackIdx < OGG_MAX_AUDIO_TRACK)
                        {
                            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1TrackIdx];
                        }
                        break;
                    default:
                        LOG(3, "Wrong Pid\n");
                        break;
                }
                rDmxMMData.u4BufStart = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                rDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr));
                rDmxMMData.u4StartAddr = (UINT32)pucBitsSeg;
                rDmxMMData.u4FrameSize = u4PacketSize;
                rDmxMMData.u4Pts = u4Pts;
                rDmxMMData.u4Dts = u4Pts;

                if (u4PacketSize == 0)
                {
                    LOG(0, "meet error\n");
                    u4ConsumedSize = 0;
                    continue;
                }
                if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData)  != TRUE)
                {
                    LOG(0, "Demuxer fails to move data.\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                fgFreshPacket = TRUE;
                u4ConsumedSize = u4PacketSize;
                u4PacketSize = 0;
            }
            else
            {
                continue;
            }
        }
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}
static BOOL _SWDMX_OggGetSomeBytes(SWDMX_INFO_T *prSwdmxInst, UCHAR *pucBitstream,
                                   UCHAR **pucStream, UCHAR *pucData, UINT8 u1ByteNum)

{
    UINT32 u4Length = 0;

    if (((UINT32)(pucBitstream) + u1ByteNum) <
            prSwdmxInst->rFeederInfo.u4EndAddr)
    {
        x_memcpy(pucData, pucBitstream, u1ByteNum);
        *pucStream = (UCHAR*)((UINT32) * pucStream + u1ByteNum);
    }
    else
    {
        if ((UINT32)*pucStream >=
                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            *pucStream =
                (UCHAR*)((UINT32) * pucStream -
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
        }
        else
        {
            u4Length = prSwdmxInst->rFeederInfo.u4EndAddr - (UINT32)pucBitstream;
            x_memcpy(pucData, pucBitstream, u4Length);
            *pucStream = (UCHAR*)prSwdmxInst->rFeederInfo.u4StartAddr;
        }

        u4Length = u1ByteNum - u4Length;
        x_memcpy(pucData, pucStream, u4Length);
        *pucStream = (UCHAR*)((UINT32) * pucStream + u4Length);
    }
    return TRUE;
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_OggRequestData(SWDMX_INFO_T* prSwdmxInst)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4ReadSize = 0;
    UINT32 u4MaxDataByte = 0;

    if (!prSwdmxInst)
    {
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    x_memset(&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));

    if (prSwdmxInst->rFmtInfo.rSwdmxMp4Info.fgEnVideo)
    {
        u4ReadSize = MIN_READ_SIZE;
    }
    else
    {
        u4ReadSize =  MIN_READ_SIZE_NO_VIDEO;
    }

    u4MaxDataByte  =  prSwdmxInst->rFeederInfo.u4Size - u4ReadSize;

    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < u4MaxDataByte) &&
            ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos) < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz))
    {
        u4ReadSize = get_min((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz - prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos), u4ReadSize);

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes == 0)
        {
            if (FeederSetRequest(prSwdmxInst->eFeederSrc,
                                 u4ReadSize,
                                 (UINT64)(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos),
                                 &rFeederCond,
                                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4FeederReqID++) == FEEDER_E_BUFFER_FULL)
            {
                LOG(3, "Feeder buffer is not enough\n");
                return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes++;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes > 0)
        {
            if (!_SWDMX_OggGetDataFromFeeder(prSwdmxInst, &rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes--;
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgRepeatRequest)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AlignOffset = rFeederReqData.u4AlignOffset;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr > VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr  +
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr)   - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr);
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgRepeatRequest = FALSE;
        }

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos += rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset ;
    }
    if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek) &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos >= prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz))
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos = 0;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos = 0;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_OggCheckHeader(UCHAR *pucBuf)
{
    UINT32 u4CmpID = 0;

    _SWDMX_OggConvertCharToUint(pucBuf, &u4CmpID);
    if (u4CmpID != OGG_PAGE_HEADER)
    {
        return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_OggCheckOggType(SWDMX_INFO_T *prSwdmxInst ,
                                   UCHAR *pucBuf, UINT8 *pu1StreamType,
                                   UINT8 *pu1TrackIdx)
{
    UINT32 u4CmpID = 0;
    UINT32 i = 0;

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
    {
        _SWDMX_OggConvertCharToUint(pucBuf, &u4CmpID);
        if (u4CmpID == prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VStrmID)
        {
            *pu1StreamType = eSWDMX_MEDIA_VIDEO;
            return TRUE;
        }
    }

    for (i = 0; (i < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (i < OGG_MAX_AUDIO_TRACK); i++)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[i])
        {
            _SWDMX_OggConvertCharToUint(pucBuf, &u4CmpID);
            if (u4CmpID == prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[i]->t_strm_id.u.t_stm_ogg_id)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio && (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx == i))
                {
                    *pu1StreamType = eSWDMX_MEDIA_AUDIO;
                }
                else
                {
                    *pu1StreamType = eSWDMX_MEDIA_SKIPPED_AUDIO;
                }

                if (pu1TrackIdx)
                {
                    *pu1TrackIdx = i;
                }
                return TRUE;
            }
        }
    }
    *pu1StreamType = eSWDMX_MEDIA_SKIPPED;
    return TRUE;
}


static BOOL _SWDMX_OggResetFeeder(SWDMX_INFO_T* prSwdmxInst)
{
    FEEDER_REQ_DATA_T rFeederReqData;
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    //SWDMX_OGG_INFO_T *prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxOggInfo;

    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder = 0;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgRepeatRequest = TRUE;
    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = prSwdmxInst->rFeederInfo.u4StartAddr;
    while (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes > 0)
    {
        _SWDMX_OggGetDataFromFeeder(prSwdmxInst, &rFeederReqData);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4ReqReadTimes--;
    }

    FeederSetRing(prSwdmxInst->eFeederSrc, TRUE);
    return TRUE;
}

static BOOL _SWDMX_SendOggAVEos(SWDMX_INFO_T* prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList)
{
    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo &&
            !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
        {
            _SWDMX_OggVideoWakeUp(prSwdmxInst);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
        }

        LOG(5, "_SWDMX_SendOggAVEos Deliver Video Eos\n");
        if (!_SWDMX_SendOggEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
        {
            LOG(3, "Fail to send Video Eos\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos = TRUE;
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio &&
            !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos)
    {
        LOG(5, "_SWDMX_SendOggAVEos Deliver Audio Eos\n");
        if (!_SWDMX_SendOggEos(prSwdmxInst, eSWDMX_MEDIA_AUDIO))
        {
            LOG(3, "Fail to send Audio Eos\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos = TRUE;
        AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
    }
    return TRUE;
}
static BOOL _SWDMX_SendOggEos(
    SWDMX_INFO_T* prSwdmxInst,
    UINT8 u1HdrType)
{
    UINT8 u1PidIdx = 0;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    LOG(5, "%s source id = %d\n", __FUNCTION__, prSwdmxInst->u1SwdmxId);
    LOG(5, "SWDMX Send Ogg Eos Hdr Type = %d\n", u1HdrType);

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxOggInfo.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
		    prSwdmxInst->fgFifoFull = TRUE;
            x_thread_delay(1);
            return FALSE;
        }
        u1PidIdx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid;
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_OggInitDmx(prSwdmxInst, u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = prSwdmxInst->rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = prSwdmxInst->rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;
    rDmxMMData.u1Idx = u1PidIdx;

    //return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
    if (u1HdrType != eSWDMX_MEDIA_AUDIO)
    {
        return _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData);
    }
    else
    {
        INT32 i;

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
        {
            if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < OGG_MAX_AUDIO_TRACK) &&
                    (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid == FALSE))
            {
                DMX_AudHandler_SetActivePidx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx]);
            }
        }
        for (i = 0; i < OGG_MAX_AUDIO_TRACK; i++)
        {
            rDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i];
            if (!_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rDmxMMData))
            {
                LOG(3, "Fail to send audio eos\n");
                return FALSE;
            }
        }
        return TRUE;
    }
}


/**
*/
BOOL _SWDMX_OggStop(UINT8 u1SrcId)
{

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    DMX_MM_T rPid;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    rPid.fgEnable = FALSE;

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVDmx)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVDmx = FALSE;
        LOG(1, "*****stop video\n");

        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
        DMX_MUL_FreePidx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnADmx)
    {
        INT32 i;
        LOG(1, "*****stop audio\n");

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
        DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid );

        for (i = 0; i < OGG_MAX_AUDIO_TRACK; i++)
        {
            VERIFY(DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i],
                              DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i]);
            DMX_MUL_FreePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i]);
            DMX_AudHandler_Free(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[i]);
        }
        DMX_AudHandler_SetEnable(FALSE);
    }
    x_memset(&prSwdmxInst->rFmtInfo.rSwdmxOggInfo, 0, sizeof(SWDMX_OGG_INFO_T));

    return TRUE;
}


static BOOL _SWDMX_OggSeek(SWDMX_INFO_T* prSwdmxInst,
                           SWDMX_RANGE_LIST_T* prRangeList,
                           UCHAR *pucBitstream,
                           UINT64 u8SeekTime,
                           UINT32 u4AvailSize, UINT32 *pu4ConsumedSize)

{
    BOOL fgSeekForward = FALSE;
    BOOL fgRequestData = FALSE;

    UINT32 u4CurPageTime = 0;

    UINT64 u8DiffPos = 0;
    UINT64 u8DiffGranule = 0;
    UINT64 u8AverageGran = 0;
    UINT64 u8AverageLen = 0;
    UINT64 u8DiffPts = 0;
    UINT64 u8TempValue = 0;
    UINT64 u8Remander = 0;

    if (!prSwdmxInst)
    {
        return FALSE;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl)
    {
        u8AverageGran = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_frame_num;
        u8AverageLen = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_packet_size;

        _SWDMX_OggCalculatePts(prSwdmxInst, eSWDMX_MEDIA_VIDEO, 0, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos, &u4CurPageTime);

        if ((u8SeekTime <= OGG_TEN_SEC_DURATION) ||
                (prRangeList->prDmxRangeInfo->z_range_sz < OGG_ONE_MB) )
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                prRangeList->prDmxRangeInfo->ui8_fileoffset;
            *pu4ConsumedSize = 0;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
            return TRUE;
        }

        if (u8SeekTime > u4CurPageTime)
        {
            u8DiffPts = u8SeekTime - (UINT64)u4CurPageTime;
            fgSeekForward = TRUE;
            if ((u8DiffPts < OGG_TEN_SEC_DURATION) ||
                    (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                     (prRangeList->prDmxRangeInfo->z_range_sz - OGG_ONE_MB)))
            {
                return TRUE;
            }
        }
        else
        {
            fgSeekForward = FALSE;
            u8DiffPts = (UINT64)u4CurPageTime - u8SeekTime;
            if (u8DiffPts < OGG_TEN_SEC_DURATION / 2)
            {
                u8DiffPts = OGG_TEN_SEC_DURATION / 2;
            }
        }
        u8TempValue = u8Div6432(u8DiffPts, 90, &u8Remander) * 10000;
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit > 0)
        {
            u8DiffGranule =  u8Div6432(u8TempValue, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit, &u8Remander);
        }
        if (u8AverageGran > 0)
        {
            u8DiffPos = u8Div6432(u8DiffGranule * 100, u8AverageGran, &u8Remander) * u8AverageLen;
        }

        if (fgSeekForward)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u8DiffPos;
            if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >
                    prRangeList->prDmxRangeInfo->z_range_sz) &&
                    (prRangeList->prDmxRangeInfo->z_range_sz > OGG_ONE_MB))
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                    prRangeList->prDmxRangeInfo->z_range_sz - OGG_ONE_MB;
            }

            if ((UINT32)u8DiffPos <  u4AvailSize)
            {
                u4AvailSize -= (UINT32)u8DiffPos;
                fgRequestData = FALSE;
                *pu4ConsumedSize += (UINT32)u8DiffPos;
            }
            else
            {
                fgRequestData = TRUE;
            }
        }
        else
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos < u8DiffPos)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                    prRangeList->prDmxRangeInfo->ui8_fileoffset;
            }
            else
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos -= u8DiffPos;
            }

            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos <=  prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos < u8DiffPos)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prRangeList->prDmxRangeInfo->ui8_fileoffset;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos - u8DiffPos;
                }
            }
            fgRequestData = TRUE;
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;

        if (fgRequestData)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
        }
    }
    return FALSE;
}




static BOOL _SWDMX_OggCalculateRepeatFrame(SWDMX_INFO_T* prSwdmxInst, UINT32 u4Pts, UINT32 *pu4TimeCount)
{
    //SWDMX_OGG_INFO_T *prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxOggInfo;

    UINT32 u4PtsDif = 0;
    UINT32 u4CurPts = 0;

    INT16  u1PreIdx = 0;

    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);
    u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);

    if (prSwdmxInst->i4PlaySpeed > 2000)
    {
        u1PreIdx = stKeyinfo.u1CurIdx  - 2;
        if (u1PreIdx < 0)
        {
            u1PreIdx = (u1PreIdx + KEY_LEN) % KEY_LEN ;
        }
        if (u1PreIdx < KEY_LEN)
        {
            if (stKeyinfo.stKeyData[u1PreIdx].u4KeyPts != 0)
            {
                u4CurPts = stKeyinfo.stKeyData[u1PreIdx].u4KeyPts;
            }
        }

        if (u4Pts > u4CurPts)
        {
            u4PtsDif = u4Pts - u4CurPts;
            *pu4TimeCount =  (UINT32)((u4PtsDif * 25) / (90 * prSwdmxInst->i4PlaySpeed));
        }
        if (*pu4TimeCount == 0)
        {
            *pu4TimeCount = 1;
        }

        if (*pu4TimeCount > MAX_REPEAT_TIME)
        {
            *pu4TimeCount = MAX_REPEAT_TIME;
        }
    }
    else if (prSwdmxInst->i4PlaySpeed < 0)
    {
        u1PreIdx = stKeyinfo.u1CurIdx  - 2;
        if (u1PreIdx < 0)
        {
            u1PreIdx = (u1PreIdx + KEY_LEN) % KEY_LEN ;
        }
        if (u1PreIdx < KEY_LEN)
        {
            if (stKeyinfo.stKeyData[u1PreIdx].u4KeyPts != 0)
            {
                u4CurPts = stKeyinfo.stKeyData[u1PreIdx].u4KeyPts;
            }
        }
        if (u4CurPts > u4Pts)
        {
            u4PtsDif = u4CurPts - u4Pts ;
            *pu4TimeCount =  (UINT32)((u4PtsDif * 25) / (90 * get_abs(prSwdmxInst->i4PlaySpeed) * 2));
        }
        if (*pu4TimeCount == 0)
        {
            *pu4TimeCount = 1;
        }

        if (*pu4TimeCount > MAX_REPEAT_TIME)
        {
            *pu4TimeCount = MAX_REPEAT_TIME;
        }
    }
    LOG(1, "pu4TimeCount = %d\n", *pu4TimeCount );
    return TRUE;
}
static BOOL _SWDMX_OggGetDataFromFeeder(SWDMX_INFO_T* prSwdmxInst,
                                        FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
    {
        return FALSE;
    }

    if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_OggGetDataFromFeeder FeederTimeOut!!!\n");
        return FALSE;
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "_SWDMX_OggGetDataFromFeeder invalid\n");
        return FALSE;
    }

    if ((prFeederData->u4Id + 1) != prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }
    return TRUE;
}

BOOL _SWDMX_OggAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_VERIFY_NULL_ERROR(prSwdmxInst);

    if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
        {
            LOG(5, "SWDMX_OggAudioCb Audio is ready\n");
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts != 0xFFFFFFFF)
            {
                VDP_TriggerAudReceive(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts);
                LOG(5, "SWDMX_OggAudioCb VDP Trigger PTS = 0x%08x\n",
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = 0xFFFFFFFF;
            }

            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
            {
                _SWDMX_OggVideoWakeUp(prSwdmxInst);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
            }

        }
        else if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS)
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_OggAudioCb\n");
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc(prSwdmxInst->u1StcId);
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgAudIsReady = TRUE;
    }
    else
    {
        // impossible
        LOG(7, "_SWDMX_OggAudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_OggVideoWakeUp(SWDMX_INFO_T* prSwdmxInst)
{
    SWDMX_OGG_INFO_T *prSwdmxOggInfo;

    SWDMX_Ogg_VERIFY_NULL(prSwdmxInst);
    prSwdmxOggInfo = (SWDMX_OGG_INFO_T*)&(prSwdmxInst->rFmtInfo.rSwdmxOggInfo);

    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    
    LOG(5, "_SWDMX_OggVideoWakeUp\n");

    switch (prSwdmxOggInfo->u4VDecType)
    {
        case ENUM_SWDMX_VDEC_VC1:
        case ENUM_SWDMX_VDEC_WMV7:
        case ENUM_SWDMX_VDEC_WMV8:
        case ENUM_SWDMX_VDEC_WMV9:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
            break;
        case ENUM_SWDMX_VDEC_DX311:
        case ENUM_SWDMX_VDEC_MPEG4:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MJPEG);
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
            break;
        default:
            LOG(1, "SWDMX_OggAudioCb Unknown Video Type\n");
            break;
    }
}

static void _SWDMX_OggConvertCharToUint(UCHAR *pucBuf, UINT32* pu4Data)
{
    *pu4Data = ((UINT32)(pucBuf[0])) |
               ((UINT32)(pucBuf[1]) << 8) |
               ((UINT32)(pucBuf[2]) << 16) |
               ((UINT32)(pucBuf[3]) << 24);

}

static SWDMX_HDR_PRS_STATE_T _SWDMX_OggAACHandle(  SWDMX_INFO_T *prSwdmxInst,
        SWDMX_RANGE_LIST_T* prRangeList,
        UINT8 u1AudTrick, UINT32 u4SampleSize,
        UINT32 u4Pts)
{

    {
        DMX_MM_DATA_T rTmpDmxMMData;

        UINT8 u1SampleRateIdx = 3;
        if (prSwdmxInst->pucAudBuf == 0)
        {
            LOG(1, "pucAudBuf should be allocated in init function");
            prSwdmxInst->pucAudBuf =
                (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(SWDMX_OGG_AAC_ALLOC_SIZE, 16));
            if (!prSwdmxInst->pucAudBuf)
            {
                LOG(1, "pucAudBuf allocated fail");
                ASSERT(0);
            }
        }

        if (prSwdmxInst->pucAudBuf)
        {
            /* 8b: syncword */
            prSwdmxInst->pucAudBuf[0] = 0xFF;
            /* 4b: syncword */
            /* 1b: mpeg id = 1 */
            /* 2b: layer = 0 */
            /* 1b: protection absent */
            prSwdmxInst->pucAudBuf[1] = 0xF9;
            /* 2b: profile */
            /* 4b: sampling_frequency_index */
            /* 1b: private = 0 */
            /* 1b: channel_configuration */
            switch (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudSampleRate)
            {
                case 48000:
                    u1SampleRateIdx = 3;
                    break;
                case 44100:
                    u1SampleRateIdx = 4;
                    break;
                case 32000:
                    u1SampleRateIdx = 5;
                    break;
                case 24000:
                    u1SampleRateIdx = 6;
                    break;
                case 22050:
                    u1SampleRateIdx = 7;
                    break;
                case 16000:
                    u1SampleRateIdx = 8;
                    break;
                case 12000:
                    u1SampleRateIdx = 9;
                    break;
                case 11025:
                    u1SampleRateIdx = 10;
                    break;
                case 8000:
                    u1SampleRateIdx = 11;
                    break;
                default:
                    u1SampleRateIdx = 4;
                    break;
            }

            prSwdmxInst->pucAudBuf[2] = (1 << 6) |
                                        ((u1SampleRateIdx << 2) & 0x3C) |
                                        ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudChannelNs >> 2) & 0x1);
            /* 2b: channel_configuration */
            /* 1b: original */
            /* 1b: home */
            /* 1b: copyright_id */
            /* 1b: copyright_id_start */
            /* 2b: aac_frame_length */
            prSwdmxInst->pucAudBuf[3] = ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudChannelNs & 0x3) << 6) |
                                        (((u4SampleSize + SWDMX_OGG_AAC_FAKE_HDR_SIZE) >> 11) & 0x3);
            /* 8b: aac_frame_length */
            prSwdmxInst->pucAudBuf[4] = (((u4SampleSize + SWDMX_OGG_AAC_FAKE_HDR_SIZE) >> 3) & 0xFF);
            /* 3b: aac_frame_length */
            /* 5b: adts_buffer_fullness */
            prSwdmxInst->pucAudBuf[5] = (((u4SampleSize + SWDMX_OGG_AAC_FAKE_HDR_SIZE) << 5) & 0xE0) |
                                        ((0x7FF >> 6) & 0x1F);
            /* 6b: adts_buffer_fullness */
            /* 2b: number_of_raw_data_blocks_in_frame */
            prSwdmxInst->pucAudBuf[6] = ((0x7FF << 2) & 0xFC);
            rTmpDmxMMData.u4BufStart = (UINT32)prSwdmxInst->pucAudBuf;
            rTmpDmxMMData.u4BufEnd = rTmpDmxMMData.u4BufStart + SWDMX_OGG_AAC_ALLOC_SIZE;
            rTmpDmxMMData.u4StartAddr = rTmpDmxMMData.u4BufStart;
            rTmpDmxMMData.u4FrameSize = 7;
            rTmpDmxMMData.u4Pts = u4Pts;
            rTmpDmxMMData.u1Idx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid;

            //if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
            if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, &rTmpDmxMMData)  != TRUE)
            {
                LOG(0, "Demuxer fails to move data.\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
    }
    return  eSWDMX_HDR_PRS_SUCCEED;
}

static void _SWDMX_OggCalculatePts(SWDMX_INFO_T *prSwdmxInst,
                                   UINT8 u1StreamType,
                                   UINT8 u1TrackIdx,
                                   UINT64 u8GranulPos,
                                   UINT32*  pu4Pts)
{
    UINT64 ui8TempVal = 0;
    UINT64 u8Remender = 0;
    if (u1StreamType == eSWDMX_MEDIA_VIDEO)
    {
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl)
        {
            ui8TempVal = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit * u8GranulPos;
            *pu4Pts = (UINT32)u8Div6432(ui8TempVal, 10000, &u8Remender);
            *pu4Pts = *pu4Pts * 90;
        }
    }
    else if ((u1StreamType == eSWDMX_MEDIA_AUDIO || u1StreamType == eSWDMX_MEDIA_SKIPPED_AUDIO) &&
             (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK) && ((u1TrackIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack)))
    {
        if ( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[u1TrackIdx])
        {
            ui8TempVal = u8GranulPos * 1000;
            ui8TempVal = ui8TempVal * 90;
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[u1TrackIdx]->ui4_rate > 0)
            {
                *pu4Pts = (UINT32)u8Div6432(ui8TempVal, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prAudIdxTbl[u1TrackIdx]->ui4_rate, &u8Remender);
            }
        }
    }
}

static BOOL _SWDMX_OggHandleSpeedChange(SWDMX_INFO_T *prSwdmxInst, SWDMX_CUR_CMD_T* prCmd, SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT32 u1Idx = 0, u4SeekLen = 0;
    UINT32 u4CurPts = VDP_GetPts(prSwdmxInst->u1B2rId);
    UINT8   u1AudIdx = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx;

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos)
    {
        UINT32 u4Flag;
        DMX_MM_T rDmxMMInfo;

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
        u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

        switch (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VDecType)
        {
            case ENUM_SWDMX_VDEC_DX311: case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8: case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_MJPEG: case ENUM_SWDMX_VDEC_H263:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            default:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
        }
        if (!DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid, u4Flag, &rDmxMMInfo))
        {
            LOG(3, "Fail to reset demuxer\n");
        }

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos = FALSE;
    }
    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverAEos = FALSE;
        AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
    }

    if((prSwdmxInst->i4PlaySpeed >1000) &&
        ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS) &&
           !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo))
    {
        return TRUE;
    }
    if ((prSwdmxInst->i4PlaySpeed > 0) &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos != 0))
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos = 0;
    }

    if ((prCmd->fgSeek) &&
            (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
    {
        UINT8 u1Idx = 0;
        // UINT32 u4APts = 0;
        LOG(3, "seek offset\n");

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount = 0;
        /* for ( ;u1Idx < (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack)&&(u1Idx < AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
         {
             DMX_AudHandler_SeekAndUpdate(
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx],
             0xffffffff, &u4APts);
             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1Idx] = 0;
         }*/
        for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < OGG_MAX_AUDIO_TRACK); u1Idx++)
        {
            DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1Idx] = 0;
        }
        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
            prCmd->rSeekOffset.ui8_i_frame_position;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts =
            prCmd->rSeekOffset.ui8_pts_info;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts = prCmd->rSeekOffset.ui8_pts_info;
        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
        VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId,
                              prCmd->rSeekOffset.ui8_pts_info);
        stKeyinfo.stKeyData[0].u8KeyOffset =
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
        stKeyinfo.stKeyData[0].u4KeyPts = prCmd->rSeekOffset.ui8_i_pts_info;

        if ((u1AudIdx < AVI_DIVX_MAX_AUD_TRACK) && (u1AudIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) &&
                (prSwdmxInst->i4PlaySpeed == 1000))
        {
            DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1AudIdx]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;
        }
    }
    else  if (((UINT32)(prSwdmxInst->u8SeekPos >> 32) != 0xFFFFFFFF) || ((UINT32)(prSwdmxInst->u8SeekPos) != 0xFFFFFFFF))
    {
        UINT32 u1Idx = 0;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_HEADER;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = (UINT32)prSwdmxInst->u8SeekPos;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts = (UINT32)prSwdmxInst->u8SeekPos;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts = (UINT32)prSwdmxInst->u8SeekPos;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange = TRUE;
        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
        VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);

        VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);

        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);

        if (!prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek = FALSE;
        }

        for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < OGG_MAX_AUDIO_TRACK); u1Idx++)
        {
            DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1Idx] = 0;
        }

        if ((u1AudIdx < AVI_DIVX_MAX_AUD_TRACK) && (u1AudIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) &&
                (prSwdmxInst->i4PlaySpeed == 1000) && prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
        {
            DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1AudIdx]);
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;

        }

    }
    else if (((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed <= 2000) &&
              (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 0)) &&
             (prSwdmxInst->i4PlaySpeed <= 2000 && prSwdmxInst->i4PlaySpeed > 0))
    {
        UINT32 u4Pts = 0;
        BOOL fgFind      = FALSE;

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos = 0;

        if (prSwdmxInst->i4PlaySpeed == 1000 &&
            ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS) &&
           prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo))
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed == 1)
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
            }

            if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed == 1) ||
                    ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 1000) && (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 1)))
            {
                STC_StartStc(prSwdmxInst->u1StcId);
            }

            if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx < AVI_DIVX_MAX_AUD_TRACK))
            {

                fgFind = DMX_AudHandler_SeekAndUpdate(
                             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx],
                             (u4CurPts + 45000), &u4Pts);
            }
        }

        DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio &&
            (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS))
        {

            if (fgFind)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4Pts;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);
            }
            else
            {
                u4Pts = u4CurPts + 45000;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4TriggerAudPts = u4Pts;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, u4Pts);
            }
         }
         if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
        {
             if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS))
            {
                DMX_AudHandler_SetActivePidxEx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx], 
                    (UINT8*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHeaderPtr[u1AudIdx],
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHdrLength[u1AudIdx]);
            }
            else if ((u1AudIdx < AVI_DIVX_MAX_AUD_TRACK) && (u1AudIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack))
            {
                DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1AudIdx]);
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid  = TRUE;

            }
        }
    }
    else if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 2000 || prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 0) &&
             ((prSwdmxInst->i4PlaySpeed == 2000) || (prSwdmxInst->i4PlaySpeed <= 1000 && prSwdmxInst->i4PlaySpeed > 0)))
    {
        UINT32 i = 0;
        UINT8 u1Idx = 0;

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 2000 &&
            ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS) &&
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo))
        {
            for (i = 0; i < KEY_LEN; i++)
            {
                if (stKeyinfo.u1CurIdx < KEY_LEN)
                {
                    LOG(1, "u4KeyPts = %x\n", stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts);
                    if (stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts >= u4CurPts)
                    {
                        break;
                    }
                    stKeyinfo.u1CurIdx++;
                    if (stKeyinfo.u1CurIdx >= KEY_LEN)
                    {
                        stKeyinfo.u1CurIdx = stKeyinfo.u1CurIdx % KEY_LEN;
                    }
                }
            }

            /*    if(stKeyinfo.u1CurIdx > 0)
                {
                    stKeyinfo.u1CurIdx--;
                }
                else
                {
                   stKeyinfo.u1CurIdx = KEY_LEN - 1;
                }*/
        }
        else if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 0)
        {
            for (i = 0; i < KEY_LEN; i++)
            {
                stKeyinfo.u1CurIdx--;
                if (stKeyinfo.u1CurIdx <= 0)
                {
                    stKeyinfo.u1CurIdx = KEY_LEN - 1;
                }
                if (stKeyinfo.u1CurIdx < KEY_LEN)
                {
                    LOG(1, "u4KeyPts = %x\n", stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts);
                    if (stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts >= u4CurPts)
                    {
                        break;
                    }
                }
            }
            /*stKeyinfo.u1CurIdx++;
            if(stKeyinfo.u1CurIdx >= KEY_LEN)
            {
                stKeyinfo.u1CurIdx = 0;
            }*/
        }
        if ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType != ENUM_SWDMX_ADEC_VORBIS) &&
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
       {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSpeedChange = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount = 0;
            MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
            VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
            for ( ;u1Idx < (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < AVI_DIVX_MAX_AUD_TRACK); u1Idx++)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1Idx] = 0;
            }

            if ((stKeyinfo.u1CurIdx < KEY_LEN) && (i < KEY_LEN) )
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 0)
                {
                    u4SeekLen = _SWDMX_OggSeekLen(prSwdmxInst, SEEK_BACKWARD);
                }
                if ((u4SeekLen > prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos))
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prRangeList->prDmxRangeInfo->ui8_fileoffset;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8KeyOffset;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                }
            }
            _SWDMX_OggResetFeeder(prSwdmxInst);
        
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
            {
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
            }
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnAudio)
            {
                for (u1Idx = 0 ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < OGG_MAX_AUDIO_TRACK); u1Idx++)
                {
                    DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx]);
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1Idx] = 0;
                }
                DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
                if ((u1AudIdx < AVI_DIVX_MAX_AUD_TRACK) && (u1AudIdx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack))
                {
                    DMX_AudHandler_SetActivePidx( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1AudIdx]);
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSetActivePid = TRUE;

                }
            }
        }
        else
        {
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPid);
            if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ADecType == ENUM_SWDMX_ADEC_VORBIS))
            {
                DMX_AudHandler_SetActivePidxEx(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1CurATrackIdx], 
                    (UINT8*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHeaderPtr[u1AudIdx],
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AudHdrLength[u1AudIdx]);
            }
        }

        if (prSwdmxInst->i4PlaySpeed == 1000)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos ==
                    prRangeList->prDmxRangeInfo->ui8_fileoffset)
            {
                u4CurPts = 0;
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ASeekPts = u4CurPts;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4ChangeAudPts = u4CurPts;
        }

        if (i >= KEY_LEN)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_HEADER;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgIsSeek = TRUE;
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts = (UINT32)u4CurPts;
        }
        x_memset(&stKeyinfo, 0, sizeof(KEY_INFO));
        STC_SetStcValue(prSwdmxInst->u1StcId, u4CurPts);
        STC_StopStc(prSwdmxInst->u1StcId);
        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
    }
    else
    {
        if (((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 2000) &&
                (prSwdmxInst->i4PlaySpeed < 0)) ||
                ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed < 0) &&
                 (prSwdmxInst->i4PlaySpeed > 2000)))
        {
            x_memset(&stKeyinfo, 0, sizeof(KEY_INFO));
        }

        if ((prSwdmxInst->i4PlaySpeed < 0) &&
                (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos == 0))
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos =
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
        }

        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_SEEK_LEN;
        MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
        VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
        if (prSwdmxInst->i4PlaySpeed < 0)
        {
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);

        }
        else
        {
            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4CurPts);
        }

        for ( ;(u1Idx < prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1TotalATrack) && (u1Idx < OGG_MAX_AUDIO_TRACK); u1Idx++)
        {
            DMX_MM_FlushBuffer( prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1AudPreBufPid[u1Idx]);
        }

        if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo)
        {
            DMX_MM_FlushBuffer(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u1VidPid);
        }
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFeederInvalid = TRUE;
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4RepeatCount = 0;
    }
    //prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed = prSwdmxInst->i4PlaySpeed;
    return TRUE;
}

/*static BOOL _SWDMX_OggFindHeader(SWDMX_INFO_T* prSwdmxInst,
    UCHAR *pucBitstream, UINT32* pu4ConsumedSize)
{
    UCHAR ucData[4] = {0};
    UCHAR *pucStream = NULL;
    UINT8 u1StreamType = 0;
    UINT8 u1TrackIdx = 0;
    UINT8 u1PageSegment = 0;
    UINT8 u1TimeCount = 0;
    UINT32 i = 0;
    UINT32 u4PageSize = 0;

     while(TRUE)
     {
         if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder <=  (OGG_HEADER_SIZE +0xFF))
         {
            if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos + *pu4ConsumedSize +
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder)>=
                                       prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
             {
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz;
                 return FALSE;
             }
         }
         else if((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos + *pu4ConsumedSize)>=
                                       prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
         {
             return FALSE;
         }

         if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder > (OGG_HEADER_SIZE +0xFF))
         {
             UINT64 u8GranulePos = 0;
             pucStream = pucBitstream;
             _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucStream, ucData,4);

             if (_SWDMX_OggCheckHeader(ucData) == TRUE)
             {
                  _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,
                                                                   &u8GranulePos,
                                                                   &u1StreamType, &u1PageSegment,&u1TrackIdx);

                 if(u1StreamType == eSWDMX_MEDIA_VIDEO)
                 {
                     prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;
                     prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr = (UINT32)pucBitstream;
                     FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL((UINT32)pucBitstream));
                     break;
                 }
                 else if (((eSWDMX_MEDIA_AUDIO == u1StreamType) ||
                            (eSWDMX_MEDIA_SKIPPED_AUDIO == u1StreamType))&&
                            (u1TrackIdx < AVI_DIVX_MAX_AUD_TRACK))
                 {
                     prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8AudGranulePos[u1TrackIdx] = u8GranulePos;
                 }
                 while(u1PageSegment > 0)
                 {
                    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData,1);
                     u4PageSize = u4PageSize +  (UINT8)ucData[0];
                     u1PageSegment --;
                 }
                 u4PageSize = u4PageSize + u1PageSegment + OGG_HEADER_SIZE;
                 pucBitstream =  pucBitstream + u4PageSize;
                 *pu4ConsumedSize += u4PageSize;
                 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < u4PageSize)
                 {
                     _SWDMX_OggRequestData(prSwdmxInst);
                 }
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4PageSize;
                 if((UINT32)pucBitstream >=
                                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                     pucBitstream =
                          (UCHAR*)((UINT32)pucBitstream -
                          VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                          VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
                 FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL((UINT32)pucBitstream));
                u1TimeCount ++;
                if(u1TimeCount >= 50)
                {
                     return FALSE;
                }
                 continue;
             }
             i++;
             (*pu4ConsumedSize)++;
             pucBitstream++;
             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder--;
             if((UINT32)pucBitstream >=
                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
             {
                  pucBitstream =
                       (UCHAR*)((UINT32)pucBitstream -
                       VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                       VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
             }
         }
         else
         {
            _SWDMX_OggRequestData(prSwdmxInst);
            if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
            {
                 return FALSE;
            }
            if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AlignOffset != 0)
            {
                pucBitstream = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4AlignOffset  = 0;
            }
         }
     }
     return TRUE;
}*/


/*static BOOL _SWDMX_OggSeekKeyPage(SWDMX_INFO_T* prSwdmxInst,
                                                                                         UCHAR *pucBitstream,
                                                                                         SEEK_DERECTION eSeekDerection, UINT32 u4SeekTime)
{
     BOOL     fgFrechPage = FALSE;
     BOOL     fgFindKey    = FALSE;

     UINT32 u4Pts = 0;
     UINT32 u4PageCount = 0;
     UINT32 u4PageSize = 0;

     if(eSeekDerection == SEEK_KEY)
     {
          while(TRUE)
          {
              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                                       prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
              {
                   prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz;
                   fgFindKey = FALSE;
                   break;
              }
              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder > (OGG_HEADER_SIZE + 0xFF))
              {
                  prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                fgFindKey =  OggJudgeKey (prSwdmxInst, (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr, &u4PageSize, &u4Pts);

                if(fgFindKey)
                {
                    break;
                }

                u4PageCount ++;
                if(u4PageCount > 2000)
                {
                    break;
                }
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4PageSize;
                 if((UINT32)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                 {
                      prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr- VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                          VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                 }
                 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < u4PageSize)
                 {
                     _SWDMX_OggRequestData(prSwdmxInst);
                 }
                  if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
                  {
                      return FALSE;
                  }
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4PageSize;
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4PageSize;

                 FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr ));
             }
             else
             {
                  _SWDMX_OggRequestData(prSwdmxInst);
             }

             if(u4Pts > u4SeekTime)
             {
                  fgFindKey = FALSE;
                  break;
             }
         }
          pucBitstream = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr;
         FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr));
         return fgFindKey;
     }
     else if(eSeekDerection == SEEK_FORWARD)
     {
         UINT32 u4SeekLenth = 0;
         UINT32 u4ConsumedSize = 0;

         UINT64 u8DiffGranule = 0;
         UINT64 u8AverageGran = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_frame_num;
         UINT64 u8AverageLen = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_packet_size;

         UINT64 u8TempValue = 0;
         UINT64 u8Remander = 0;
         UINT32 u4Pts = 0;

         u8TempValue = u8Div6432((UINT64)(OGG_FF_SEEK_TIME*90000), 90, &u8Remander)*10000;
         if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit > 0)
         {
             u8DiffGranule =  u8Div6432(u8TempValue, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit, &u8Remander);
         }

         if(u8AverageGran > 0)
         {
             u4SeekLenth = u8Div6432(u8DiffGranule*100, u8AverageGran, &u8Remander)*u8AverageLen;
         }
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4SeekLenth;
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
         _SWDMX_OggResetFeeder(prSwdmxInst);

         if(_SWDMX_OggRequestData(prSwdmxInst) ==  eSWDMX_HDR_PRS_FED_MOVE_FAIL)
         {
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
         }

         while(TRUE)
          {
              u4ConsumedSize = 0;
              u4PageSize = 0;

              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                                       prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
              {
                   prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz;
                   fgFindKey = FALSE;
                   break;
              }
              fgFrechPage = _SWDMX_OggFindHeader(prSwdmxInst, (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr, &u4ConsumedSize);

              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;

             if(fgFrechPage)
             {
                  fgFindKey =  OggJudgeKey (prSwdmxInst,
                                                                     (UCHAR* )prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr,
                                                                      &u4PageSize, &u4Pts);

                  if(fgFindKey)
                  {
                        if(stKeyinfo.u1CurIdx < KEY_LEN)
                        {
                            stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8KeyOffset =
                                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                            stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts = u4Pts;
                            stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8GranulPos =
                                         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos ;
                            stKeyinfo.u1CurIdx ++;
                            if(stKeyinfo.u1CurIdx >= KEY_LEN)
                            {
                                stKeyinfo.u1CurIdx =  stKeyinfo.u1CurIdx%KEY_LEN;
                                stKeyinfo.fgRing = TRUE;
                            }
                        }
                        break;
                  }

                   u4PageCount ++;
                   if(u4PageCount > 3000)
                   {
                       break;
                   }

                 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < u4PageSize)
                 {
                     _SWDMX_OggRequestData(prSwdmxInst);
                 }
                 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
                 {
                     return FALSE;
                 }
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4PageSize;
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4PageSize;
                 prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4PageSize;

                 if((UINT32)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                 {
                      prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                                                                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr-
                                                                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                                                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
                 }

                 pucBitstream = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr;
                 FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL((UINT32)pucBitstream));
             }
          }
         return fgFindKey;
     }
     else
     {
         UINT32 u4SeekLenth = 0;
         UINT32 u4ConsumedSize = 0;
         UINT32 u4DmxSize = 0;
         UINT32 u4Pts = 0;

         UINT64 u8DiffGranule = 0;
         UINT64 u8AverageGran = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_frame_num;
         UINT64 u8AverageLen = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_packet_size;

         UINT64 u8TempValue = 0;
         UINT64 u8Remander = 0;*/
/*  u8TempValue =(UINT64) (((OGG_FB_SEEK_TIME*90000)/90)*10000);
  u8DiffGranule = u8TempValue/ prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit;
  u4SeekLenth = ((u8DiffGranule*100)/ u8AverageGran)*u8AverageLen;*/
/* u8TempValue = u8Div6432((UINT64)(OGG_FF_SEEK_TIME*90000), 90, &u8Remander)*10000;

 if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit > 0)
 {
      u8DiffGranule =  u8Div6432(u8TempValue, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit, &u8Remander);
 }
 if(u8AverageGran > 0)
 {
     u4SeekLenth = u8Div6432(u8DiffGranule*100, u8AverageGran, &u8Remander)*u8AverageLen;
 }

   while(TRUE)
   {
       if(!fgHaveSeek )
       {
           if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos != 0)
           {
               prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos;
           }

           if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos <= u4SeekLenth)
           {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FileOffset;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFirstFrame = TRUE;
           }
           else
           {
               prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos -= u4SeekLenth;
           }

           prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos =
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos ;

            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                   prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            _SWDMX_OggResetFeeder(prSwdmxInst);
        }

        fgHaveSeek = TRUE;

        if(_SWDMX_OggRequestData(prSwdmxInst) ==  eSWDMX_HDR_PRS_FED_MOVE_FAIL)
        {
             if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
            {
                return FALSE;
            }
            continue;
        }

        u4DmxSize  = 0;
        while(u4DmxSize <u4SeekLenth )
         {
              fgHaveSeek = FALSE;
              u4ConsumedSize = 0;
              u4PageSize = 0;
              fgFrechPage = _SWDMX_OggFindHeader(prSwdmxInst, (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr, &u4ConsumedSize);
              u4DmxSize += u4ConsumedSize;
              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4ConsumedSize;

              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos >=
                                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8_range_sz)
              {
                   break;
              }

              fgFindKey =  OggJudgeKey (prSwdmxInst,
                                                                     (UCHAR* )prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr,
                                                                      &u4PageSize, &u4Pts);

              if(fgFindKey)
              {
                  if(stKeyinfo.u1CurIdx < KEY_LEN)
                  {
                     stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8KeyOffset =
                          prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                     stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts = u4Pts;
                     stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8GranulPos =
                                  prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos ;
                     stKeyinfo.u1CurIdx ++;
                     if(stKeyinfo.u1CurIdx >= KEY_LEN)
                     {
                         stKeyinfo.u1CurIdx =  stKeyinfo.u1CurIdx%KEY_LEN;
                         stKeyinfo.fgRing = TRUE;
                     }
                 }
                 break;
              }

              u4PageCount ++;
              if(u4PageCount > 3000)
              {
                  break;
              }
              u4DmxSize += u4PageSize;
              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < u4PageSize)
              {
                  _SWDMX_OggRequestData(prSwdmxInst);
              }
              if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
              {
                  return FALSE;
              }
              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr += u4PageSize;
              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder -= u4PageSize;
              prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4PageSize;
              if((UINT32)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr >=
              VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
              {
                   prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr =
                                                             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr-
                                                             VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                                             VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
              }

             pucBitstream = (UCHAR*)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr;
             FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL((UINT32)pucBitstream));
       }

        if(fgFindKey )
        {
            break;
        }
       prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                                  prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurFilePos;
   }
   return fgFindKey;
}
}*/

static BOOL _SWDMX_OggPageHdrPrs(SWDMX_INFO_T* prSwdmxInst,
                                 UCHAR *pucBitstream, UINT64 *pu8GranulePos,
                                 UINT8 *pu1StreamType, UINT8 *pu1Segment,
                                 UINT8 *pu1TrackIdx)
{
    BOOL fgFreshPacket = FALSE;
    BOOL fgBeginPage    = FALSE;
    BOOL fgEosPage       = FALSE;

    UCHAR *pucStream = pucBitstream;
    UCHAR ucData[4] = {0};

    UINT32 u4TempData = 0;
    UINT32 u4PageSeqNum = 0;
    UINT32 u4CrcCheck = 0;

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 2);

    fgFreshPacket = ucData[1] & 0x1 ? FALSE :  TRUE;
    fgBeginPage = ucData[1] & 0x2;
    fgEosPage = ucData[1] & 0x4;

    if (fgBeginPage)
    {
        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgParseHeadPacket = TRUE;
        LOG(5, "enter BOS page, maybe system\n ");
    }

    if (fgEosPage)
    {
        LOG(5, "enter EOS page\n");
    }

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);
    _SWDMX_OggConvertCharToUint(ucData, &u4TempData);
    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);
    _SWDMX_OggConvertCharToUint(ucData, (UINT32*)pu8GranulePos);
    *pu8GranulePos = (*pu8GranulePos << 32) | u4TempData ;

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);

    _SWDMX_OggCheckOggType(prSwdmxInst, ucData, pu1StreamType, pu1TrackIdx);

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);
    _SWDMX_OggConvertCharToUint(ucData, &u4PageSeqNum);

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);
    _SWDMX_OggConvertCharToUint(ucData, &u4CrcCheck);

    _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 1);
    *pu1Segment = (UINT8)ucData[0];

    if (fgEosPage)
    {
        LOG(5, "enter EOS page\n");
        if (*pu1StreamType == eSWDMX_MEDIA_VIDEO)
        {
            if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgEnVideo &&
                    !prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause)
                {
                    _SWDMX_OggVideoWakeUp(prSwdmxInst);
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgVDecIsPause = FALSE;
                }

                LOG(5, "_SWDMX_AviHdrPrsWithoutIdx Deliver Video Eos\n");
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.i4PrevSpeed > 0)
                {
                    if (!_SWDMX_SendOggEos(prSwdmxInst, eSWDMX_MEDIA_VIDEO))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgDeliverVEos = TRUE;
                }
            }
        }
    }
    return fgFreshPacket;
}

/*static BOOL OggJudgeKey(SWDMX_INFO_T* prSwdmxInst,
                                                                  UCHAR *pucBitstream, UINT32 *pu4PageSize,
                                                                  UINT32 *pu4Pts)
{
    BOOL fgFindKey = FALSE;
    BOOL fgFrechPage = FALSE;

    UCHAR *pucStream = pucBitstream;
    UCHAR ucData[4] = {0};
    UCHAR *pucPayLoadPos = NULL;

    UINT64  u8GranulePos = 0;

     UINT8    u1StreamType = 0;
     UINT8    u1Segment       = 0;
     UINT8    u1TempSeg     = 0;
     UINT8    u1TrackIdx      = 0;
     UINT8    u1PayDate = 0;
     UINT32 u4PayloadSize = 0;
     UINT32 u4FrameNum = 0;

      _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucStream, ucData,4);

     if (!_SWDMX_OggCheckHeader(ucData) )
     {
        LOG(3,"not a header\n");
        fgFindKey = FALSE;
     }
     fgFrechPage =  _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,  &u8GranulePos,
                                             &u1StreamType, &u1Segment, &u1TrackIdx);
     if(u1StreamType == eSWDMX_MEDIA_VIDEO)
     {
         _SWDMX_OggCalculatePts(prSwdmxInst,u1StreamType, u1TrackIdx, u8GranulePos,pu4Pts);
     }
     else
     {
         *pu4Pts = 0;
     }
     u1TempSeg = u1Segment;
     pucStream += OGG_HEADER_SIZE -4 ;

     if((UINT32)pucStream >=
     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
     {
          pucStream = pucStream - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
               VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
      }

     while(u1TempSeg> 0)
     {
         _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData,1);
         u4PayloadSize = u4PayloadSize +  (UINT8)ucData[0];
         if((UINT8)ucData[0] < 0xFF)
         {
             u4FrameNum ++;
         }
         u1TempSeg --;
     }
     *pu4PageSize = u4PayloadSize +u1Segment + OGG_HEADER_SIZE;
     pucPayLoadPos = pucBitstream+ u1Segment + OGG_HEADER_SIZE;
     pucStream = pucBitstream + OGG_HEADER_SIZE;
      if((UINT32)pucPayLoadPos >=
     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
     {
          pucPayLoadPos = pucPayLoadPos - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
               VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
      }

     if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4LeftBytesInFeeder < *pu4PageSize)
     {
         _SWDMX_OggRequestData(prSwdmxInst);
         if(prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgPlayAbort)
         {
             return FALSE;
         }
     }

     if(u1StreamType != eSWDMX_MEDIA_VIDEO)
     {
         pucStream += u4PayloadSize + u1Segment;
         if((UINT32)pucStream >=
         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
         {
              pucStream = pucStream - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                   VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
         }
         FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL((UINT32)pucBitstream));
         pucBitstream = pucStream;
         return FALSE;
     }

     if(u8GranulePos >= (UINT64)u4FrameNum)
     {
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos - u4FrameNum;
     }
     else
     {
         prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = 0;
     }

     if(fgFrechPage)
     {
         u1PayDate = (UINT8)*pucPayLoadPos;
         if(u1PayDate == 0x08)
         {
             fgFindKey = TRUE;
         }
     }

     u4PayloadSize = 0;

     while(u1Segment > 0)
     {
          _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData,1);
          u1Segment--;

          if((UINT8)ucData[0] == 0xff)
          {
              u4PayloadSize = u4PayloadSize + (UINT8)ucData[0];
          }
          else
          {
              u4PayloadSize = u4PayloadSize + (UINT8)ucData[0];
              pucPayLoadPos = pucPayLoadPos+ u4PayloadSize;
              u4PayloadSize = 0;

               if((UINT32)pucPayLoadPos >=
              VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
               {
                   pucPayLoadPos = pucPayLoadPos - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
               }

               u1PayDate = (UINT8)*pucPayLoadPos;
               if(u1PayDate == 0x08)
               {
                     fgFindKey = TRUE;
                     break;
               }
          }
     }
     return fgFindKey;
}*/

BOOL _SWDMX_OggAbort(UINT8 u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_OGG_INFO_T *prSwdmxOggInfo = &prSwdmxInst->rFmtInfo.rSwdmxOggInfo;
    if (prSwdmxOggInfo)
    {
        LOG(3, "FLV Play abort\n");
        prSwdmxOggInfo->fgPlayAbort = TRUE;
    }
    return TRUE;
}


static UINT32 _SWDMX_OggSeekLen(SWDMX_INFO_T*prSwdmxInst, SEEK_DERECTION eSeekDerection)
{
    UINT32 u4SeekLenth = 0;
    UINT64 u8DiffGranule = 0;
    UINT64 u8TempValue = 0;
    UINT64 u8AverageGran = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_frame_num;
    UINT64 u8AverageLen = prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui4_avg_packet_size;
    UINT64 u8Remander = 0;
    if (eSeekDerection == SEEK_FORWARD)
    {
        u8TempValue = u8Div6432((UINT64)(OGG_FF_SEEK_TIME * 90000), 90, &u8Remander) * 10000;
    }
    else if (eSeekDerection == SEEK_BACKWARD)
    {
        u8TempValue = u8Div6432((UINT64)(OGG_FB_SEEK_TIME * 90000), 90, &u8Remander) * 10000;
    }

    if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit > 0)
    {
        u8DiffGranule =  u8Div6432(u8TempValue, prSwdmxInst->rFmtInfo.rSwdmxOggInfo.prVidIdxTbl->ui8_timeunit, &u8Remander);
    }

    if (u8AverageGran > 0)
    {
        u4SeekLenth = u8Div6432(u8DiffGranule * 100, u8AverageGran, &u8Remander) * u8AverageLen;
    }

    return u4SeekLenth;
}

static BOOL _SWDMX_OggFindHeader(SWDMX_INFO_T* prSwdmxInst,
                                 UCHAR *pucBitstream, UINT32* pu4ConsumedSize, UINT32 u4AvailSize)
{
    UCHAR ucData[4] = {0};
    UCHAR *pucStream = NULL;

    while (u4AvailSize > (OGG_HEADER_SIZE + 0xFF))
    {
        pucStream = pucBitstream;
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucStream, ucData, 4);
        if (_SWDMX_OggCheckHeader(ucData) == TRUE)
        {
            return TRUE;
        }
        (*pu4ConsumedSize)++;
        pucBitstream++;
        u4AvailSize--;
        if ((UINT32)pucBitstream >=
                VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
        {
            pucBitstream =
                (UCHAR*)((UINT32)pucBitstream -
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                         VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
        }
    }
    return FALSE;
}

static UINT32 _SWDMX_OggGetPageSizeAndFrameNum(SWDMX_INFO_T* prSwdmxInst, UCHAR *pucBitstream,
        UINT8 u1SegNum, UINT8 *u1FrameNum)
{
    UCHAR ucData[4] = {0};
    UINT32 u4PayloadSize = u1SegNum;

    while (u1SegNum > 0)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucBitstream, ucData, 1);
        u4PayloadSize = u4PayloadSize +  (UINT8)ucData[0];
        if ((UINT8)ucData[0] < 0xFF)
        {
            (*u1FrameNum)++;
        }
        u1SegNum --;
    }
    u4PayloadSize = u4PayloadSize + OGG_HEADER_SIZE;
    return u4PayloadSize;
}

static BOOL _SWDMX_OggJudgeKey(SWDMX_INFO_T* prSwdmxInst,
                               UCHAR *pucBitstream, UINT8 u1SegNum, UINT8 *u1KeyNum,
                               BOOL fgFreshPage)
{
    BOOL fgFindKey = FALSE;
    UCHAR ucData[4] = {0};
    UINT8 u1PayDate = 0;
    UINT32 u4PayloadSize = 0;
    UCHAR *pucPayLoadPos = pucBitstream + u1SegNum;

    if ((UINT32)pucPayLoadPos >=
            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
    {
        pucPayLoadPos =
            (UCHAR*)((UINT32)prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4CurDmxFeederRPtr -
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
    }

    if (fgFreshPage)
    {
        u1PayDate = (UINT8) * pucPayLoadPos;
        if (u1PayDate == 0x08)
        {
            fgFindKey = TRUE;
        }
    }

    while (u1SegNum > 0)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucBitstream, &pucBitstream, ucData, 1);
        u1SegNum--;

        if ((UINT8)ucData[0] == 0xff)
        {
            u4PayloadSize = u4PayloadSize + (UINT8)ucData[0];
        }
        else
        {
            u4PayloadSize = u4PayloadSize + (UINT8)ucData[0];
            pucPayLoadPos = pucPayLoadPos + u4PayloadSize;
            u4PayloadSize = 0;
            (*u1KeyNum)++;
            if ((UINT32)pucPayLoadPos >=
                    VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
            {
                pucPayLoadPos = pucPayLoadPos - VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr);
            }

            u1PayDate = (UINT8) * pucPayLoadPos;
            if (u1PayDate == 0x08)
            {
                fgFindKey = TRUE;
                break;
            }
        }
    }
    return fgFindKey;
}

static BOOL _SWDMX_OggSeekKeyPage(SWDMX_INFO_T* prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList,
                                  UCHAR *pucBitstream, UINT32 u4AvailSize,
                                  UINT32 *pu4ConsumeSize, SEEK_DERECTION eSeekDerection,
                                  UINT32 u4SeekTime)
{
    BOOL    fgFindKey = FALSE;
    BOOL    fgPage = FALSE;
    BOOL    fgSeekDone = FALSE;

    switch (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus)
    {
        case eSWDMX_OGG_TIME_SEEK:
        {
            fgPage = _SWDMX_OggFindVideoPage(prSwdmxInst, pucBitstream, pu4ConsumeSize, u4AvailSize);

            if (fgPage)
            {
                u4AvailSize -= *pu4ConsumeSize;
                pucBitstream = (UCHAR*)((UINT32)pucBitstream + *pu4ConsumeSize);
                if ((UINT32)pucBitstream >=
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    pucBitstream =
                        (UCHAR*)((UINT32)pucBitstream -
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
                fgSeekDone = _SWDMX_OggSeek(prSwdmxInst, prRangeList, pucBitstream, u4SeekTime,
                                            u4AvailSize,
                                            pu4ConsumeSize);
                if (fgSeekDone)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_KEY;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_HEADER;
                }
            }
        }
        break;
        case eSWDMX_OGG_SEEK_LEN:
        {
            UINT32 u4SeekLen = 0;

            u4SeekLen = _SWDMX_OggSeekLen(prSwdmxInst, eSeekDerection);

            if (eSeekDerection == SEEK_FORWARD)
            {
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos += u4SeekLen;
                prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
            }
            else if (eSeekDerection == SEEK_BACKWARD)
            {
                if (prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos > u4SeekLen)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos - u4SeekLen;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8PreDmxPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos =
                        prRangeList->prDmxRangeInfo->ui8_fileoffset;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurMoviPos =
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgFirstFrame = TRUE;
                }
            }
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_HEADER;
            _SWDMX_OggResetFeeder(prSwdmxInst);
        }
        break;

        case eSWDMX_OGG_FIND_HEADER:
        {
            fgPage = _SWDMX_OggFindHeader(prSwdmxInst, (UCHAR*)pucBitstream,
                                          pu4ConsumeSize, u4AvailSize);
            if (fgPage)
            {
                if (eSeekDerection == SEEK_KEY)
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_TIME_SEEK;
                }
                else
                {
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_FIND_KEY;
                }
            }
        }
        break;

        case eSWDMX_OGG_FIND_KEY:
        {
            BOOL   fgFrechPage = FALSE;

            UINT8  u1StreamType = 0;
            UINT8  u1PageSegment = 0;
            UINT8  u1TrackIdx = 0;
            UINT8  u1FrameNum = 0;

            UINT32 u4PageSize = 0;
            UINT32 u4Pts = 0;

            UINT64 u8GranulePos = 0;

            while (u4AvailSize > OGG_HEADER_SIZE + 0xFF)
            {
                fgFrechPage = _SWDMX_OggPageHdrPrs(prSwdmxInst,
                                                   (pucBitstream + 4),
                                                   &u8GranulePos, &u1StreamType, &u1PageSegment, &u1TrackIdx);

                u4PageSize = _SWDMX_OggGetPageSizeAndFrameNum(prSwdmxInst,
                             (pucBitstream + OGG_HEADER_SIZE),
                             u1PageSegment, &u1FrameNum);


                if (u4AvailSize >= u4PageSize)
                {
                    if (u1StreamType == eSWDMX_MEDIA_VIDEO)
                    {
                        UINT8 u1KeyNum = 0;

                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;
                        _SWDMX_OggCalculatePts(prSwdmxInst, u1StreamType, 0, u8GranulePos, &u4Pts);

                        fgFindKey = _SWDMX_OggJudgeKey(prSwdmxInst,
                                                       (pucBitstream + 27),
                                                       u1PageSegment, &u1KeyNum, fgFrechPage);
                        if (fgFindKey)
                        {
                            if (stKeyinfo.u1CurIdx < KEY_LEN)
                            {
                                stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8KeyOffset =
                                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos;
                                stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u4KeyPts = u4Pts;
                                stKeyinfo.stKeyData[stKeyinfo.u1CurIdx].u8GranulPos =
                                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos ;
                                stKeyinfo.u1CurIdx ++;
                                if (stKeyinfo.u1CurIdx >= KEY_LEN)
                                {
                                    stKeyinfo.u1CurIdx =  stKeyinfo.u1CurIdx % KEY_LEN;
                                    stKeyinfo.fgRing = TRUE;
                                }
                            }
                            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_SEEK_LEN;
                            break;
                        }
                    }
                    prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4PageCount++;

                    if (u4Pts > prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u4VSeekPts)
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.fgSeekDone = TRUE;
                        break;
                    }

                    pucBitstream += u4PageSize;
                    u4AvailSize -= u4PageSize;
                    *pu4ConsumeSize += u4PageSize;
                    if ((UINT32)pucBitstream >=
                            VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                    {
                        pucBitstream =
                            (UCHAR*)((UINT32)pucBitstream -
                                     VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                     VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                    }

                    if ((eSeekDerection == SEEK_BACKWARD) &&
                            ((prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8CurDmxMoviPos + *pu4ConsumeSize) >
                             prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8FilePosBeforeSeek))
                    {
                        prSwdmxInst->rFmtInfo.rSwdmxOggInfo.eActStatus = eSWDMX_OGG_SEEK_LEN;
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        break;

        default:
            break;
    }
    return fgFindKey;
}

static BOOL _SWDMX_OggFindVideoPage(SWDMX_INFO_T* prSwdmxInst,
                                    UCHAR *pucBitstream, UINT32* pu4ConsumedSize, UINT32 u4AvailSize)
{
    UCHAR *pucStream = pucBitstream;
    UINT8 u1StreamType = 0;
    UINT8 u1PageSegment = 0;
    UINT8 u1TrackIdx = 0;
    UINT8 u1FrameNum = 0;

    UINT32 u4PageSize = 0;

    UINT64 u8GranulePos = 0;

    UCHAR ucData[4] = {0};



    while (u4AvailSize > OGG_HEADER_SIZE + 0xFF)
    {
        _SWDMX_OggGetSomeBytes(prSwdmxInst, pucStream, &pucStream, ucData, 4);

        if (_SWDMX_OggCheckHeader(ucData) != TRUE)
        {
            return FALSE;
        }
        _SWDMX_OggPageHdrPrs(prSwdmxInst, pucStream,  &u8GranulePos,
                             &u1StreamType, &u1PageSegment, &u1TrackIdx);

        if (u1StreamType == eSWDMX_MEDIA_VIDEO)
        {
            prSwdmxInst->rFmtInfo.rSwdmxOggInfo.u8VidGranulePos = u8GranulePos;
            return TRUE;
        }

        else
        {
            u4PageSize = _SWDMX_OggGetPageSizeAndFrameNum(prSwdmxInst,
                         (pucStream + (OGG_HEADER_SIZE - 4)),
                         u1PageSegment, &u1FrameNum);
            if (u4PageSize <= u4AvailSize)
            {
                u4AvailSize -= u4PageSize;
                *pu4ConsumedSize += u4PageSize;
                pucStream = (UCHAR*)((UINT32)pucStream + u4PageSize);
                if ((UINT32)pucStream >=
                        VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr))
                {
                    pucStream =
                        (UCHAR*)((UINT32)pucStream -
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4EndAddr) +
                                 VIRTUAL(prSwdmxInst->rFeederInfo.u4StartAddr));
                }
            }
            else
            {
                break;
            }
        }
    }
    return FALSE;
}


