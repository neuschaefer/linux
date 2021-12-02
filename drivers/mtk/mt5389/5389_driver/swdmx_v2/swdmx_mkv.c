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
 * $Author: songjun.tang $
 * $Date: 2012/05/14 $
 * $RCSfile: swdmx_mkv.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_mkv.c
 *  Software demux for mkv - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_mkv.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"

#include "b2r_drvif.h"
#include "b2r_if.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define AUD_PREBUF_EN

//#define MKV_TRICK_REFINE // TODO: 264 need to wait 2nd callback before send 1st pes


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


#define MKV_BUFFER_SIZE (235 * 7 * 1024)
#define MIN_READ_SIZE (235 * 1024)
#ifdef LINUX_TURNKEY_SOLUTION
#define FDR_ALIGN_SIZE (3 * 1024)
#else
#define FDR_ALIGN_SIZE (1 * 1024)
#endif
#define RING_BUF_RSV 64
#define MIN_REPEAT_FRM 2    // visible repeat period
#define SKIP_STRM 0xFFFF
#define TRICK_2_NORMAL 1
#define NORMAL_2_TRICK 2
#define MAX_KEY_FRM 5
#define SEND_EOS_THRD 3
#define MKV_KEY_TABLE_THRESHOLD 3
#define TIME_CODE_SCALE 100000


#ifdef MKV_CONTENT_ENCODING_SUPPORT
// content compression algorithm
#define MKV_CONTENT_ALGO_ZLIB ((UINT32)0)
//#define MKV_CONTENT_ALGO_BZLIB ((UINT32)1) abandoned
//#define MKV_CONTENT_ALGO_LZO1X ((UINT32)2) abandoned
#define MKV_CONTENT_ALGO_HEADER_STRIPPING ((UINT32)3)

// content encoding scope
#define MKV_CONTENT_SCOPE_ALL ((UINT32)1 << 0)
#define MKV_CONTENT_SCOPE_CODEC_PRIVATE ((UINT32)1 << 1)
#define MKV_CONTENT_SCOPE_NEXT ((UINT32)1 << 2)

// content encoding type
#define MKV_CONTENT_COMPRESSION ((UINT32)0)
#define MKV_CONTENT_ENCRYPTION ((UINT32)1)
#endif


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define LE_LONG32(a) ((a >> 24) | ((a & 0x00FF0000) >> 8) | \
           ((a & 0x0000FF00) << 8) | (a << 24))
#define cal_min(x, y) ((x < y) ? x : y)
#define cal_abs(x)                  (((x) >= 0) ? (x) : -(x))
#define SEI 6
#define SPS 7
#define PPS 8
#define IDR_PIC 5
#define IS_1ST_SLICE(x) (((x & 0x80) > 0))
#define IS_IDR_NALU(x) (((x & 0x1f) == IDR_PIC))
#define IS_FRM_NALU(x) (((x & 0x1f) <= IDR_PIC))
#define IS_HDR_NALU(x) (((x & 0x1f) == SEI) || ((x & 0x1f) == SPS) || ((x & 0x1f) == PPS))


#define AVC_I_FRM   2
#define AVC_B_FRM   1
#define AVC_P_FRM   0
#define AVC_UNKNOWN_FRM   5

#define SWDMX_MKV_VERIFY_NULL_ERROR(ptr, ret)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "MKV SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return ret; }      \
          } while (0)

#define SWDMX_MKV_VERIFY_NULL_ERROR_VOID(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { \
                LOG(3, "MKV SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                return; }      \
          } while (0)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static BOOL _SWDMXInitDmx(UINT8 u1SrcId, UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos);

static BOOL _SWDMXResetFeeder(UINT8 u1SrcId);

static BOOL _SWDMXMkvPrsDrmInfo(UINT8 u1SrcId, UCHAR* pucBitstream, UINT32 u4BufLength);

static BOOL _SWDMXMkvDecDrmData(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo, UINT32 u4BlkSz);

static BOOL _SWDMXMkvSwitchTrack(BOOL fgIsTPT, SWDMX_MKV_INFO_T *prSwdmxMkvInfo);

static BOOL _vMKVDeliverData(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo,
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf);

static UINT32 _vMKVGetNaluSize(UCHAR* pucFifo, UINT8 u1AvcNaluLen);

static BOOL _vMKVContentDecode(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo,
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf);

#ifdef MKV_TRICK_REFINE
static BOOL _IsSwHdrNalu(UINT32 u4Addr)
{
    UINT8 *pu1Addr = (UINT8 *)VIRTUAL(u4Addr);
    if ((pu1Addr[0] == 0)
        && (pu1Addr[1] == 0)
        && (pu1Addr[2] == 1)
        && IS_HDR_NALU(pu1Addr[3]))
    {
        return TRUE;
    }
    if ((pu1Addr[0] == 0)
        && (pu1Addr[1] == 0)
        && (pu1Addr[2] == 0)
        && (pu1Addr[3] == 1)
        && IS_HDR_NALU(pu1Addr[4]))
    {
        return TRUE;
    }
    return FALSE;
}
#endif


BOOL _SWDMX_MkvAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    DMX_AUDIO_PES_T rPes;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo = (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    // set PTS by start pts of sending data only for AAC , end pts for other codec
    if (!prPes->fgEOS
        && prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID] != ENUM_SWDMX_ADEC_AAC)
    {
        x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));
        rPes.u4Wp = rPes.u4PesRp;
        prPes = &rPes;
    }
    //LOG(5,"send audio pts = %d\n",prPes->u4Pts/90);
    LOG(7,"send audio pts = %x\n",prPes->u4Pts);

    return AUD_SendAudioPes(prPes);
}


BOOL _SWDMX_MkvVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo = (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    if (prPes->u4Pts)
    {
        prSwdmxMkvInfo->u4LastCbVidPts = prPes->u4Pts; // keep latest non-zero video pts
    }

    rPesInfo.u4PTS = prSwdmxMkvInfo->u4LastCbVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = prSwdmxMkvInfo->u8VidOffset;
    rPesInfo.ucEsId = prPes->u1Channel;
    if (prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    if (rPesInfo.fgEos)
    {
        LOG(5, "VEos CB\n");
        rPesInfo.fgBackward= (prSwdmxMkvInfo->i4PrevSpeed < 0) ? TRUE : FALSE;
        rPesInfo.u4VldReadPtr =0;
        rPesInfo.u4FifoStart =0;
        rPesInfo.u4FifoEnd =0;
    }
    else
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }

    if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        rPesInfo.fgGop = prPes->fgGop;
        // resolve AB repeat response slow, don't send pes until find I frame.
        if (prSwdmxMkvInfo->fgSeekOffset)
        {
            if (PIC_TYPE_I != prPes->u4FrameType)
            {
                if (!DMX_UpdateVideoReadPointer(rPesInfo.ucEsId, rPesInfo.u4VldReadPtr, rPesInfo.u4VldReadPtr))
                {
                    LOG(1, "Codec MPEG: ES(%d) DMX_UpdVRPtr Fail\n", rPesInfo.ucEsId);
                }
                return TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgSeekOffset =FALSE;
            }
        }
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
            rPesInfo.ucPicType = MPV_I_TYPE;
            break;

        case PIC_TYPE_P:
            rPesInfo.ucPicType = MPV_P_TYPE;
            break;

        case PIC_TYPE_B:
            rPesInfo.ucPicType = MPV_B_TYPE;
            break;

        default:
            break;
        }
    }
    else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
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
        if (IS_IDR_NALU(rPesInfo.ucPicType))
        {
            prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
        }
        //H.264 will use fgWaitIFrm flag to filter non-ref frame in _vMKVDeliverData, so here we do nothing for AB repeat.
        /*if (prSwdmxMkvInfo->fgSeekOffset)
        {
            if (!IS_IDR_NALU(rPesInfo.ucPicType))
            {
                return TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgSeekOffset =FALSE;
            }
        }*/
        //calculate PTS by decoder if CodecID is V_MS/VFW/FOURCC avi type
        if (prSwdmxMkvInfo->u4MainVidHeaderPtr == 0)
        {
            if (IS_IDR_NALU(rPesInfo.ucPicType))
           {
                prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
                rPesInfo.fgDtsValid = TRUE;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }
        }
    }
    else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
    {
        UCHAR *pucBuf;

        rPesInfo.fgDtsValid = TRUE;
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
            // resolve AB repeat response slow, don't send pes until find I frame.
            if (prSwdmxMkvInfo->fgSeekOffset)
            {
                if (0 != (((UINT32)(*pucBuf)) >> 6))
                {
                    if (!DMX_UpdateVideoReadPointer(rPesInfo.ucEsId, rPesInfo.u4VldReadPtr, rPesInfo.u4VldReadPtr))
                    {
                        LOG(1, "Codec MPEG4: ES(%d) DMX_UpdVRPtr Fail\n", rPesInfo.ucEsId);
                    }
                    return TRUE;
                }
                else
                {
                    prSwdmxMkvInfo->fgSeekOffset =FALSE;
                }
            }

            switch (pucBuf[0] >> 6)
            {
                case 0:
                    prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
                    rPesInfo.ucPicType  = I_TYPE;
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
    }
    else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_DX311)
    {
        UCHAR *pucBuf;

        rPesInfo.fgDtsValid = TRUE;
        u4Addr = prPes->u4FrameAddr;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        // resolve AB repeat response slow, don't send pes until find I frame.
        if (prSwdmxMkvInfo->fgSeekOffset)
        {
            if (I_TYPE != ((pucBuf[0] >> 6) & 0x1))
            {
                if (!DMX_UpdateVideoReadPointer(rPesInfo.ucEsId, rPesInfo.u4VldReadPtr, rPesInfo.u4VldReadPtr))
                {
                    LOG(1, "Codec DX311: ES(%d) DMX_UpdVRPtr Fail\n", rPesInfo.ucEsId);
                }
                return TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgSeekOffset =FALSE;
            }
        }
        if ((pucBuf[0] >> 6) & 0x1)
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
        else
        {
            prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
            rPesInfo.ucPicType = I_TYPE;
        }
    }
    else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_VP8)
    {
        UCHAR ucBuf;
        if (rPesInfo.u4DTS==1)
        {
           return TRUE;
        }
        rPesInfo.fgDtsValid = TRUE;
        u4Addr = prPes->u4FrameAddr;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        ucBuf = *(UCHAR*)VIRTUAL(u4Addr);
        // resolve AB repeat response slow, don't send pes until find I frame.
        if (prSwdmxMkvInfo->fgSeekOffset)
        {
            if (!((ucBuf&0x1)==0))
            {
                if (!DMX_UpdateVideoReadPointer(rPesInfo.ucEsId, rPesInfo.u4VldReadPtr, rPesInfo.u4VldReadPtr))
                {
                    LOG(1, "Codec VP8: ES(%d) DMX_UpdVRPtr Fail\n", rPesInfo.ucEsId);
                }
                return TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgSeekOffset =FALSE;
            }
        }

        if ((ucBuf&0x1)==0)
        {
            prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
            rPesInfo.ucPicType = I_TYPE;
        }
        else
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
    }
    else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
        prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
    }
    else
    {
        rPesInfo.fgDtsValid = TRUE;
        prSwdmxMkvInfo->u8IClusterPos = prSwdmxMkvInfo->u8CurClusterPos;
        switch(prSwdmxMkvInfo->u4VDecType)
        {
            case ENUM_SWDMX_VDEC_VC1:
                rPesInfo.u4VideoType = DMX_VIDEO_VC1;
                rPesInfo.fgDtsValid = TRUE;
                if (prSwdmxMkvInfo->fgSeekOffset ||prSwdmxMkvInfo->fgTtickSeekToNormal || (prSwdmxMkvInfo->i4Speed > 2000)||(prSwdmxMkvInfo->i4Speed < 0))
                {
                    rPesInfo.fgDtsValid = TRUE;
                    if (prSwdmxMkvInfo->fgTtickSeekToNormal)
                    {
                        if (prSwdmxMkvInfo->i4TtickSeekToNormalCount == 0)
                        {
                            prSwdmxMkvInfo->fgTtickSeekToNormal = FALSE;
                            prSwdmxMkvInfo->i4TtickSeekToNormalCount = 3;
                        }
                        else
                        {
                            prSwdmxMkvInfo->i4TtickSeekToNormalCount--;
                        }
                    }
                    else if (prSwdmxMkvInfo->fgSeekOffset)
                    {
                        prSwdmxMkvInfo->fgSeekOffset = FALSE;
                        rPesInfo.u4PTS = prSwdmxMkvInfo->u8VideoOnlyAdjustPts;
                        prSwdmxMkvInfo->u8VideoOnlyAdjustPts = 0;
                    }
                }
            break;
            case ENUM_SWDMX_VDEC_WMV7:
                rPesInfo.u4VideoType = DMX_VIDEO_WMV7;
            break;
            case ENUM_SWDMX_VDEC_WMV8:
                rPesInfo.u4VideoType = DMX_VIDEO_WMV8;
            break;
            case ENUM_SWDMX_VDEC_WMV9:
                rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
            break;
            default:
                rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
            break;
        }
    }
    rPesInfo.u8Offset = prSwdmxMkvInfo->u8CurDmxMoviPos;
    rPesInfo.u8OffsetI = prSwdmxMkvInfo->u8IClusterPos;
    rPesInfo.rExtra.u8PTSI = rPesInfo.u4PTS;
    if ((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(3, "MMCallback Addr 0!\n");
    }
    else
    {
#ifndef MKV_TRICK_REFINE
        VDEC_SendEs((void*)&rPesInfo);
#else
        if (prPes->fgEOS)
        {
            VDEC_SendEs((void*)&rPesInfo);
        }
        else if (prSwdmxMkvInfo->fgIsJump)
        {
            if (prSwdmxMkvInfo->u4VidRepeatFrame == 0)
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
            else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                if (_IsSwHdrNalu(prPes->u4FrameAddr))
                {
                    VDEC_SendEs((void*)&rPesInfo);
                }
                else
                {
                    INT32 i;
                    rPesInfo.fgNotUpdateDmxRp = TRUE;
                    for (i = cal_min(30, prSwdmxMkvInfo->u4VidRepeatFrame) - 1; i; i--)
                    {
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    rPesInfo.fgNotUpdateDmxRp = FALSE;
                    VDEC_SendEs((void*)&rPesInfo);

                    prSwdmxMkvInfo->u4VidRepeatFrame = 0;
                }
            }
            else if ((prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
                || (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_DX311))
            {
                if (rPesInfo.ucPicType != I_TYPE)
                {
                    VDEC_SendEs((void*)&rPesInfo);
                }
                else
                {
                    INT32 i;
                    rPesInfo.fgNotUpdateDmxRp = TRUE;
                    for (i = cal_min(30, prSwdmxMkvInfo->u4VidRepeatFrame) - 1; i; i--)
                    {
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    rPesInfo.fgNotUpdateDmxRp = FALSE;
                    VDEC_SendEs((void*)&rPesInfo);

                    prSwdmxMkvInfo->u4VidRepeatFrame = 0;
                }
            }
            else if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if (rPesInfo.ucPicType != MPV_I_TYPE)
                {
                    VDEC_SendEs((void*)&rPesInfo);
                }
                else
                {
                    INT32 i;
                    rPesInfo.fgNotUpdateDmxRp = TRUE;
                    for (i = cal_min(30, prSwdmxMkvInfo->u4VidRepeatFrame) - 1; i; i--)
                    {
                        VDEC_SendEs((void*)&rPesInfo);
                    }
                    rPesInfo.fgNotUpdateDmxRp = FALSE;
                    VDEC_SendEs((void*)&rPesInfo);

                    prSwdmxMkvInfo->u4VidRepeatFrame = 0;
                }
            }
            else
            {
                INT32 i;
                rPesInfo.fgNotUpdateDmxRp = TRUE;
                for (i = cal_min(30, prSwdmxMkvInfo->u4VidRepeatFrame) - 1; i; i--)
                {
                    VDEC_SendEs((void*)&rPesInfo);
                }
                rPesInfo.fgNotUpdateDmxRp = FALSE;
                VDEC_SendEs((void*)&rPesInfo);

                prSwdmxMkvInfo->u4VidRepeatFrame = 0;
            }
        }
        else
        {
            VDEC_SendEs((void*)&rPesInfo);
        }
#endif
    }

    return TRUE;
}


static INT64 _VsintSubtr(UINT8 u1Byte)
{
    INT64 i8RetVal = 0;

    switch(u1Byte)
    {
        case 1:
          i8RetVal = 0x3F;
        break;
        case 2:
          i8RetVal = 0x1FFF;
        break;
        case 3:
          i8RetVal = 0x0FFFFF;
        break;
        case 4:
          i8RetVal = 0x07FFFFFF;
        break;
        case 5:
          i8RetVal = 0x03FFFFFFF;
        break;
        case 6:
          i8RetVal = 0xFFFFFFFF;//0x01FFFFFFFFFF;
        break;
        case 7:
          i8RetVal = 0xFFFFFFFF;//0x00FFFFFFFFFFFFFF;
        break;
        case 8:
          i8RetVal = 0xFFFFFFFF;//0x007FFFFFFFFFFFFF;
        break;
        default:
          i8RetVal = 0;
        break;
    }

    return i8RetVal;
}

UINT32 _EbmlGetSz(UINT32 u4ReadData)
{
    UINT32 i, u4Mask;

    for (i = 1, u4Mask = 0x80; u4Mask > 0; i++, u4Mask >>= 1)
    {
        if (u4ReadData & u4Mask)
        {
            return i;
        }
    }
    return 0;
}

UINT32 _EbmlGetValueLess4(UCHAR* pucData)
{
    UINT32 i, u4Mask;
    UINT32 u4ReadData = (UINT32)(*pucData);
    UINT32 u4ReadValue = 0;
    UINT32 u4ReturnValue;

    LOADB_1DWRD(pucData, u4ReadValue);

    for (i = 1, u4Mask = 0x80; u4Mask > 0; i++, u4Mask >>= 1)
    {
        if (u4ReadData & u4Mask)
        {
            break;
        }
    }

    switch (i)
    {
        case 1:
          u4ReturnValue = EBML1BYTE(u4ReadValue);
        break;
        case 2:
          u4ReturnValue = EBML2BYTE(u4ReadValue);
          break;
        case 3:
          u4ReturnValue = EBML3BYTE(u4ReadValue);
          break;
        case 4:
          u4ReturnValue = EBML4BYTE(u4ReadValue);
          break;
     #if 0
         case 5:
         case 6:
         case 7:
         case 8:
         case 0:
     #endif
        default:
          // i = 0, return 0
          u4ReturnValue = 0;// MKV_EBML_INVALID; // Not Handle
          break;
    }

    return u4ReturnValue;
}

UINT32 _EbmlGetDataValueLess4(UCHAR* pucData, UINT32 u4Size)
{
    UINT32 u4ReadValue = 0;
    UINT32 u4ReturnValue;

    LOADB_1DWRD(pucData, u4ReadValue);

    switch (u4Size)
    {
      case 1:
        u4ReturnValue = EBML1BYTE(u4ReadValue);
        break;
      case 2:
        u4ReturnValue = EBML2BYTE(u4ReadValue);
        break;
      case 3:
        u4ReturnValue = EBML3BYTE(u4ReadValue);
        break;
      case 4:
        u4ReturnValue = EBML4BYTE(u4ReadValue);
        break;
      default:
        u4ReturnValue = MKV_EBML_INVALID; // Not Handle
        break;
    }

    return u4ReturnValue;
}

UINT32 _EbmlGetValueOver4(UCHAR* pucData)
{
    UINT32 u4ReturnValue = 0;

    LOADB_1DWRD(pucData, u4ReturnValue);

    return u4ReturnValue;
}

void _MkvSetElementRange(UINT8 u1SrcId, UINT32 u4Level, UINT32 u4Sa, UINT32 u4Sz)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);

    SWDMX_MKV_VERIFY_NULL_ERROR_VOID(prSwdmxInst);
    prSwdmxMkvInfo = (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (u4Level < MKV_MAX_LEVEL)
    {
        if ((UINT32)u4Sa >= prFeederMkvInfo->u4EndAddr)
        {
            u4Sa = (u4Sa - prFeederMkvInfo->u4EndAddr) + prFeederMkvInfo->u4StartAddr;
            LOG(3, "Chg SetRangeSa 0x%x\n", u4Sa);
        }

        prSwdmxMkvInfo->au4MkvElementSa[u4Level] = u4Sa;
        prSwdmxMkvInfo->au4MkvElementSz[u4Level] = u4Sz;
        prSwdmxMkvInfo->au4MkvElementEa[u4Level] = u4Sa + u4Sz;
        if (prSwdmxMkvInfo->au4MkvElementEa[u4Level] > prFeederMkvInfo->u4EndAddr)
        {
            prSwdmxMkvInfo->au4MkvElementEa[u4Level] =
                (prSwdmxMkvInfo->au4MkvElementEa[u4Level] - prFeederMkvInfo->u4EndAddr) +
                prFeederMkvInfo->u4StartAddr;
        }

    }
}

static UINT32 _MkvGetElementEa(UINT8 u1SrcId, UINT32 u4Level)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, 0);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    return (u4Level < MKV_MAX_LEVEL) ? prSwdmxMkvInfo->au4MkvElementEa[u4Level] : 0;
}

static UINT32 _MkvGetElementSz(UINT8 u1SrcId, UINT32 u4Level)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, 0);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    return (u4Level < MKV_MAX_LEVEL) ? prSwdmxMkvInfo->au4MkvElementSz[u4Level] : 0;
}

static BOOL _IsOutOfElmtRange(UINT8 u1SrcId, UINT32 u4Level, UINT32 u4Addr)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if ((UINT32)u4Addr >= prFeederMkvInfo->u4EndAddr)
    {
        u4Addr = (u4Addr - prFeederMkvInfo->u4EndAddr) + prFeederMkvInfo->u4StartAddr;
        LOG(3, "Chg OutRangeCheck 0x%x\n", u4Addr);
    }

    if (prSwdmxMkvInfo->au4MkvElementEa[u4Level] > prSwdmxMkvInfo->au4MkvElementSa[u4Level])
    {
        if ((u4Addr >= prSwdmxMkvInfo->au4MkvElementSa[u4Level]) &&
            (u4Addr < prSwdmxMkvInfo->au4MkvElementEa[u4Level]))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        if ((u4Addr >= prSwdmxMkvInfo->au4MkvElementSa[u4Level]) &&
            (u4Addr <= prFeederMkvInfo->u4EndAddr))
        {
            return FALSE;
        }
        else if ((u4Addr >= (prFeederMkvInfo->u4StartAddr - RING_BUF_RSV)) &&
            (u4Addr < prSwdmxMkvInfo->au4MkvElementEa[u4Level]))
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
}

static UINT32 _CalDataSize(UINT8 u1SrcId, UINT32 u4StartAddr, UINT32 u4EndAddr)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, 0);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (u4EndAddr > u4StartAddr)
    {
        return (u4EndAddr - u4StartAddr);
    }
    else
    {
        return ((prFeederMkvInfo->u4EndAddr - u4StartAddr) +
            (u4EndAddr - prFeederMkvInfo->u4StartAddr));
    }
}

UINT8 _AACSampFreqIdx(UINT32 u4SampFreq)
{
    switch(u4SampFreq)
    {
      case 96000:
        return(0x0);
      case 88200:
        return(0x1);
      case 64000:
        return(0x2);
      case 48000:
        return(0x3);
      case 44100:
        return(0x4);
      case 32000:
        return(0x5);
      case 24000:
        return(0x6);
      case 22050:
        return(0x7);
      case 16000:
        return(0x8);
      case 12000:
        return(0x9);
      case 11025:
        return(0xa);
      case 8000:
        return(0xb);
      case 7350:
        return(0xc);
      default:
        return(0xFF);
    }
}

UINT32 _ADecTypeCast(ASH_AUDIO_FMT_T  e_fmt)
{
    switch(e_fmt)
    {
    case ASH_AUDIO_FMT_MPEG:
        return ENUM_SWDMX_ADEC_MPEG;
    case ASH_AUDIO_FMT_AC3:
        return ENUM_SWDMX_ADEC_AC3;
    case ASH_AUDIO_FMT_PCM:
        return ENUM_SWDMX_ADEC_PCM;
    case ASH_AUDIO_FMT_MP3:
        return ENUM_SWDMX_ADEC_MP3;
    case ASH_AUDIO_FMT_AAC:
        return ENUM_SWDMX_ADEC_AAC;
    case ASH_AUDIO_FMT_DTS:
        return ENUM_SWDMX_ADEC_DTS;
    case ASH_AUDIO_FMT_WMA:
        return ENUM_SWDMX_ADEC_WMA;
    case ASH_AUDIO_FMT_RA:
        return ENUM_SWDMX_ADEC_RA;
    case ASH_AUDIO_FMT_HDCD:
        return ENUM_SWDMX_ADEC_HDCD;
    case ASH_AUDIO_FMT_MLP:
        return ENUM_SWDMX_ADEC_MLP;
    case ASH_AUDIO_FMT_MTS:
        return ENUM_SWDMX_ADEC_MTS;
    case ASH_AUDIO_FMT_VORBIS:
        return ENUM_SWDMX_ADEC_VORBIS;
    default:
        LOG(0, "Unknown fmt %d\n", e_fmt);
        return ENUM_SWDMX_ADEC_UNKNOWN;
    }
}

void _GenAACHdr(UINT8 u1SrcId, UCHAR *pucBuf, UINT32 u4FrameLen)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR_VOID(prSwdmxInst);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    /* 8b: syncword */
    pucBuf[0] = 0xFF;
    /* 4b: syncword */
    /* 1b: mpeg id = 1 */
    /* 2b: layer = 0 */
    /* 1b: protection absent */
    pucBuf[1] = 0xF9;
    /* 2b: profile */
    /* 4b: sampling_frequency_index */
    /* 1b: private = 0 */
    /* 1b: channel_configuration */
    pucBuf[2] = (UCHAR)((1 << 6) |
               ((prSwdmxMkvInfo->u4AudSampleRate[prSwdmxMkvInfo->u4CurAStrmID] << 2) & 0x3C) |
               ((prSwdmxMkvInfo->u4AudChannelNs[prSwdmxMkvInfo->u4CurAStrmID] >> 2) & 0x1));
    /* 2b: channel_configuration */
    /* 1b: original */
    /* 1b: home */
    /* 1b: copyright_id */
    /* 1b: copyright_id_start */
    /* 2b: aac_frame_length */
    pucBuf[3] = (UCHAR)(((prSwdmxMkvInfo->u4AudChannelNs[prSwdmxMkvInfo->u4CurAStrmID] & 0x3) << 6) |
               ((u4FrameLen >> 11) & 0x3));
    /* 8b: aac_frame_length */
    pucBuf[4] = (UCHAR)(((u4FrameLen >> 3) & 0xFF));
    /* 3b: aac_frame_length */
    /* 5b: adts_buffer_fullness */
    pucBuf[5] = (UCHAR)(((u4FrameLen << 5) & 0xE0) |
               ((0x7FF >> 6) & 0x1F));
    /* 6b: adts_buffer_fullness */
    /* 2b: number_of_raw_data_blocks_in_frame */
    pucBuf[6] = (UCHAR)(((0x7FF << 2) & 0xFC));

    HalFlushInvalidateDCacheMultipleLine((UINT32)pucBuf, AAC_HDR_LEN);
}


UINT32 _GenOggHdr(UINT8 u1SrcId, UCHAR *pucBuf, UINT32 u4FrameLen)
{
    UINT32 u4PageSeg, u4OggPage;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, 0);
    prSwdmxMkvInfo = (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    // Page no
    u4OggPage = prSwdmxMkvInfo->u4OggPage[prSwdmxMkvInfo->u4CurAStrmID];
    pucBuf[18] = (UINT8)((u4OggPage >>  0) & 0xFF);
    pucBuf[19] = (UINT8)((u4OggPage >>  8) & 0xFF);
    pucBuf[20] = (UINT8)((u4OggPage >> 16) & 0xFF);
    pucBuf[21] = (UINT8)((u4OggPage >> 24) & 0xFF);
    prSwdmxMkvInfo->u4OggPage[prSwdmxMkvInfo->u4CurAStrmID]++;
    //CRC=4 bytes
    x_memset(&pucBuf[22], 0, 4);

    for (u4PageSeg = 1; u4FrameLen > 0xFF; u4PageSeg++)
    {
        u4FrameLen -= 0xFF;
        pucBuf[26 + u4PageSeg] = 0xFF;
    }
    // Last segment of "Segment Table"
    pucBuf[26 + u4PageSeg] = u4FrameLen; //Update last segment table.
    // "Segments Number"=1
    pucBuf[26] = u4PageSeg;

    HalFlushDCacheMultipleLine((UINT32)&pucBuf[18], 9 + u4PageSeg);

    return 27 + u4PageSeg; // OGG header size
}


BOOL _GenVorbisTbl(UINT8 u1SrcId, MM_RANGE_DECODER_T *ptDecoderInfo)
{
    UINT64 u8BufLen;
    UINT8 *pu1SrcBuf;
    UINT8 *pucTgtBuf;
    UINT32 i;
    UINT32 u4Addr, u4Size, u4Align;
    UINT32 u4Frm[3];
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo = (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (NULL == ptDecoderInfo)
    {
        ASSERT(0);
        return FALSE;
    }

    u8BufLen = ptDecoderInfo->ui8_codec_private_len;
    pu1SrcBuf = ptDecoderInfo->pui1_codec_private;
    if (2 != pu1SrcBuf[0])
    {
        ASSERT(0);
        return FALSE;
    }
    u4Frm[0] = pu1SrcBuf[1];
    u4Frm[1] = pu1SrcBuf[2];
    if ((u4Frm[0] + u4Frm[1] + 3) >= u8BufLen)
    {
        ASSERT(0);
        return FALSE;
    }
    u4Frm[2] = u8BufLen - u4Frm[0] - u4Frm[1] - 3;

    u4Addr = 0;
    u4Size = u8BufLen + 27 * 3 + u4Frm[0] / 0xff + u4Frm[1] / 0xff + u4Frm[2] / 0xff;
    u4Align = 16;
    if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
        || u4Addr == 0)
    {
        LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return FALSE;
    }

    pucTgtBuf = (UINT8 *)u4Addr;
    pu1SrcBuf += 3;
    for (i = 0; i < 3; i++)
    {
        UINT32 u4OggHdrSz = _GenOggHdr(u1SrcId, (UCHAR*)prSwdmxMkvInfo->aucOggHdr, u4Frm[i]);
        if (!u4OggHdrSz)
        {
            ASSERT(0);
            return FALSE;
        }
        x_memcpy((VOID *)pucTgtBuf, (VOID *)prSwdmxMkvInfo->aucOggHdr, u4OggHdrSz);
        pucTgtBuf += u4OggHdrSz;
        x_memcpy((VOID *)pucTgtBuf, (VOID *)pu1SrcBuf, u4Frm[i]);
        pucTgtBuf += u4Frm[i];
        pu1SrcBuf += u4Frm[i];
    }

    HalFlushDCacheMultipleLine(u4Addr, u4Size);

    prSwdmxMkvInfo->u4AudHeaderPtr[ptDecoderInfo->ui4_strm_id] = u4Addr;
    prSwdmxMkvInfo->u4AudHdrLength[ptDecoderInfo->ui4_strm_id] = u4Size;

    return TRUE;
}


BOOL _ChkWMVSC(const UCHAR *pucBuf)
{
    if (((pucBuf[0] == 0) && (pucBuf[1] == 0)) && (pucBuf[2] == 0x1))
    {
        if ((pucBuf[3] >= 0xC) && (pucBuf[3] <= 0xF))
        {
            return TRUE;
        }
    }
    return FALSE;
}


static BOOL _ChkAACSW(const UCHAR *pucBuf)
{
    // check AAC sync word
    return ((pucBuf[0] == 0xFF) && ((pucBuf[1] & 0xF0) == 0xF0));
}


UINT8 _ChkAVCFrmType(const UCHAR *pucBuf)
{
    //bit0  : first_mb_in_slice, ue code 1(1 bit)
    //bit1~: slice_type, ue code decoding
    UINT8 u1Byte = (*pucBuf) & 0x7F;
    UINT8 u1ShiftBit = 6, u1BitCnt = 0, u1SliceType = 1;
    UINT8 u1Info = 0;

    if (!IS_1ST_SLICE(*pucBuf))    //not first slice in frame
    {
        return AVC_UNKNOWN_FRM;
    }
    while(((u1Byte >> u1ShiftBit) & 0x1) == 0)
    {
        u1BitCnt++;
        u1SliceType *= 2;
        u1ShiftBit--;
        if (u1ShiftBit == 0)
        {
            break;
        }
    }
    u1Info = (UINT8)(((u1Byte << (u1BitCnt + 2)) & 0xFF) >> (8 - u1BitCnt));
    u1SliceType += u1Info;
    u1SliceType -= 1;

    u1SliceType = (u1SliceType >= 5) ? (u1SliceType - 5) : u1SliceType;

    if (u1SliceType > AVC_I_FRM)
    {
        LOG(3, "AVCFrmType err!%d\n", u1SliceType);
    }
    return u1SliceType;
}


static VOID _vMkvClearSpInfo(UINT8 u1SrcId)
{
    UINT32 u4Idx;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR_VOID(prSwdmxInst);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    for(u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
    {
        prSwdmxMkvInfo->u4SBSkipAudPts[u4Idx]   = 0x0;
        prSwdmxMkvInfo->u4BKSBSkipAudPts[u4Idx] = 0x0;
        prSwdmxMkvInfo->fgIsAudSkipDone[u4Idx]  = TRUE;
    }
    for(u4Idx = 0; u4Idx < MKV_MAX_SB_TRACK; u4Idx++)
    {
        prSwdmxMkvInfo->u8SBClusterPos[u4Idx]   = 0x0;
        prSwdmxMkvInfo->u4SubPts[u4Idx]   = 0x0;
    }
    prSwdmxMkvInfo->fgIsVidSkipDone = TRUE;
    prSwdmxMkvInfo->u4SBSkipVidPts = 0x0;
}


static BOOL _vMkvInfoInit(UINT8 u1SrcId)
{
    UINT32 u4Idx;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    // Default Speed
    prSwdmxMkvInfo->i4PrevSpeed = 1000;
    prSwdmxMkvInfo->fgIsRewindToStart = FALSE;
    prSwdmxMkvInfo->i4TtickSeekToNormalCount = 3;

    // Default Seek parameter
    prSwdmxMkvInfo->fgIsSeek = FALSE;

    prSwdmxMkvInfo->fgBeginToPlay = TRUE;

    prSwdmxMkvInfo->u4CurRepeatFrameIdx = 0xFFFFFFFF;
    prSwdmxMkvInfo->u4ModeChangPts = 0xFFFFFFFF;
    prSwdmxMkvInfo->u4LastCbVidPts = 0;

    prSwdmxMkvInfo->u8CurClusterPos = 0;
    prSwdmxMkvInfo->u1AudPid = 0xFF;

    prSwdmxMkvInfo->u8SeekPts = 0xFFFFFFFF;
    prSwdmxMkvInfo->u8SeekPts <<= 32;
    prSwdmxMkvInfo->u8SeekPts += 0xFFFFFFFF;
    if (prSwdmxMkvInfo->pAVADmaAddr == NULL)
    {
        UINT32 u4Addr = 0;
        UINT32 u4Size = AVC_HDR_LEN + VC1_HDR_LEN + AAC_HDR_LEN + OGG_HDR_LEN;
        UINT32 u4Align = 1024;
        if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align))
        {
            LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
            ASSERT(0);
            return FALSE;
        }

        prSwdmxMkvInfo->pAVADmaAddr = (UINT8 *)u4Addr;
        if (prSwdmxMkvInfo->pAVADmaAddr==NULL)
        {
            LOG(3, "Failed to allocate AVC,VC1,AAC buffer!\n");
            return FALSE;
        }
        else
        {
            prSwdmxMkvInfo->aucAVCHdr = prSwdmxMkvInfo->pAVADmaAddr;
            prSwdmxMkvInfo->aucVC1Hdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN;
            prSwdmxMkvInfo->aucAACHdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN + VC1_HDR_LEN;
            prSwdmxMkvInfo->aucOggHdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN + VC1_HDR_LEN + AAC_HDR_LEN;
        }
    }
    else
    {
        prSwdmxMkvInfo->aucAVCHdr = prSwdmxMkvInfo->pAVADmaAddr;
        prSwdmxMkvInfo->aucVC1Hdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN;
        prSwdmxMkvInfo->aucAACHdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN + VC1_HDR_LEN;
        prSwdmxMkvInfo->aucOggHdr = prSwdmxMkvInfo->pAVADmaAddr + AVC_HDR_LEN + VC1_HDR_LEN + AAC_HDR_LEN;
    }



    if (prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale == 0)
    {
        prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale = 90 * TIME_CODE_SCALE;
    }
    prSwdmxMkvInfo->aucAVCHdr[0] = 0;
    prSwdmxMkvInfo->aucAVCHdr[1] = 0;
    prSwdmxMkvInfo->aucAVCHdr[2] = 0;
    prSwdmxMkvInfo->aucAVCHdr[3] = 1;
    prSwdmxMkvInfo->aucVC1Hdr[0] = 0;
    prSwdmxMkvInfo->aucVC1Hdr[1] = 0;
    prSwdmxMkvInfo->aucVC1Hdr[2] = 0x01;
    prSwdmxMkvInfo->aucVC1Hdr[3] = 0x0d;
    HalFlushDCacheMultipleLine((UINT32)prSwdmxMkvInfo->pAVADmaAddr, 8);

    // ### OGG HEADER FOR VORBIS ###
    //"OggS"
    prSwdmxMkvInfo->aucOggHdr[0] = 0x4F;
    prSwdmxMkvInfo->aucOggHdr[1] = 0x67;
    prSwdmxMkvInfo->aucOggHdr[2] = 0x67;
    prSwdmxMkvInfo->aucOggHdr[3] = 0x53;
    //"Version" must be 0x00
    prSwdmxMkvInfo->aucOggHdr[4] = 0x00;
    //"Type": =1 continue page, =2 BOS, =4 EOS.
    prSwdmxMkvInfo->aucOggHdr[5] = 0x00;
    //"Granule=8, S/N=4" bytes
    x_memset(&prSwdmxMkvInfo->aucOggHdr[6], 0, 12);
    HalFlushDCacheMultipleLine((UINT32)prSwdmxMkvInfo->aucOggHdr, 18);

    prSwdmxMkvInfo->i4BlkDuration = 0;
    for(u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
    {
        prSwdmxMkvInfo->u4AudPts[u4Idx] = 0xFFFFFFFF;
    }
    prSwdmxMkvInfo->fgAudioNnotifyDecodeDone = FALSE;

    prSwdmxMkvInfo->fgChangeSBDone = TRUE;
    _vMkvClearSpInfo(u1SrcId);

    return TRUE;
}


static void _vMkvVidHdrParsing(UINT8 u1SrcId, UINT32 u4Hdr, UINT32 u4HdrSz)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    UCHAR* pucFifo;
    UINT32 u4MkvPSNum, u4PSLen;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR_VOID(prSwdmxInst);
    prSwdmxMkvInfo =
      (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;


    if (u4Hdr == 0)
    {
        return;     // no video header
    }
    //video header parsing
    pucFifo = (UCHAR*)u4Hdr;
    if ((ENUM_SWDMX_VDEC_T)prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
    {
        prSwdmxMkvInfo->u1MkvAvcNaluLen = (UINT8)((pucFifo[4] & 0x03) + 1);
        pucFifo += 5;
        u4MkvPSNum = (*pucFifo++) & 0x1F; //SPS
        while(u4MkvPSNum > 0)
        {
            u4PSLen = 0;
            LOADB_1WORD(pucFifo, u4PSLen);
            pucFifo += 2;
            _vMKVContentDecode(u1SrcId, (UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4PSLen, TRUE, FALSE);   //insert start code
            u4MkvPSNum--;
            pucFifo += u4PSLen;
        }
        u4MkvPSNum = *pucFifo++;  //PPS
        while(u4MkvPSNum > 0)
        {
            u4PSLen = 0;
            LOADB_1WORD(pucFifo, u4PSLen);
            pucFifo += 2;
            _vMKVContentDecode(u1SrcId, (UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4PSLen, TRUE, FALSE);   //insert start code
            u4MkvPSNum--;
            pucFifo += u4PSLen;
        }
    }
    else// mpeg4, vc1
    {
        _vMKVContentDecode(u1SrcId, (UINT8)eSWDMX_MEDIA_VIDEO, pucFifo, u4HdrSz, FALSE, FALSE);
    }
}


static BOOL _vMKVDeliverData(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo,
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf)  //p transfer
{
    DMX_MM_DATA_T rDmxMMData;
    UINT16 u2CurQNum, u2MaxQNum;
    UINT32 u4FifoAvailSize, u4SliceTypeAddr, u4NaluTypeAddr;
    UINT32 u4QNumThrd = 30; //general fps
    UINT8 u1PidType, u1PidIdx;

    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (u4DataLen == 0)
    {
        return TRUE;
    }
    if(prSwdmxMkvInfo->fgHDRDELIVER == FALSE)
    {
        //check if A/V fifo full
        if(u1HdrType == eSWDMX_MEDIA_SUBTITLE)
        { //check if A/V fifo full
            u1PidIdx = prSwdmxMkvInfo->u1SubPid;

            u1PidType = DMX_PID_TYPE_PES_TIME;
            
            u4FifoAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)u1PidType, u1PidIdx);
        }
        else 
        {
            u1PidIdx = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (prSwdmxMkvInfo->u1VidPid):(prSwdmxMkvInfo->u1AudPid);

            u1PidType = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (DMX_PID_TYPE_ES_VIDEO):(DMX_PID_TYPE_ES_AUDIO);

            u4FifoAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)u1PidType, u1PidIdx);
        }
        
        if (u4FifoAvailSize < (u4DataLen + OGG_HDR_LEN))
        {
            LOG(2, "Type %d Fifo Full\n", u1HdrType);
            if (((u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO) &&
                prSwdmxMkvInfo->fgIsAudWaitAout) &&
                (!MPV_IsDisplay(prSwdmxInst->u1VdecId)))
            {
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                prSwdmxMkvInfo->fgIsAudWaitAout = FALSE;
            }
            //timeout, vfifo full and audio data is not parsed yet
            if ((u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO) &&
                prSwdmxMkvInfo->fgAudTrigVidDec)
            {
                LOG(3, "vfifo full and audio data is not parsed yet, force play video\n");
                _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
            }
            x_thread_delay(10);
            prSwdmxInst->fgFifoFull = TRUE;
            return FALSE;
        }
    }

    x_memset((void*)&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));
    if (u1HdrType == eSWDMX_MEDIA_VIDEO && prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_VP8)
    {
        DMX_MM_T rDMXInfo;
        x_memset(&rDMXInfo, 0, sizeof(DMX_MM_T));
        DMX_MM_Get(prSwdmxMkvInfo->u1VidPid,DMX_MM_FLAG_BUF,&rDMXInfo);
        if (rDMXInfo.u4Wp+u4DataLen+4>=rDMXInfo.u4BufAddr+rDMXInfo.u4BufSize)
        {
           rDmxMMData.u4StartAddr = (UINT32)(pucFifo);
           rDmxMMData.u4FrameSize = rDMXInfo.u4BufAddr+rDMXInfo.u4BufSize-rDMXInfo.u4Wp;
           rDmxMMData.u4Pts=prSwdmxMkvInfo->u4CurBlockPts;
           rDmxMMData.u4Dts=1;
           rDmxMMData.u1Idx=prSwdmxMkvInfo->u1VidPid;
           if (!bIsFdrBuf)
           {
               rDmxMMData.u4BufStart = rDmxMMData.u4StartAddr & 0xFFFFFFF0;
               rDmxMMData.u4BufEnd = (((rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize) + 0xf) & 0xFFFFFFF0) + 192;
           }
           else
           {
               rDmxMMData.u4BufStart = prFeederMkvInfo->u4StartAddr;
               rDmxMMData.u4BufEnd = prFeederMkvInfo->u4EndAddr;
               if (rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
               {
                   rDmxMMData.u4BufStart -= RING_BUF_RSV;
               }
               else if (rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
               {
                   rDmxMMData.u4StartAddr =(rDmxMMData.u4StartAddr - rDmxMMData.u4BufEnd) + rDmxMMData.u4BufStart;
               }
           }

           LOG(5, "VP8 Send data,Swrap dmx WP to start\n");
           if (_SWDMX_MoveData(u1SrcId, &rDmxMMData) != TRUE)
           {
               LOG(3, "Demuxer fails to move vp8 data.\n");
           }
           return FALSE;
         }
    }

    rDmxMMData.u4FrameSize = u4DataLen;
    rDmxMMData.u4Pts = prSwdmxMkvInfo->u4CurBlockPts;
    rDmxMMData.u4StartAddr = (UINT32)(pucFifo);

    //check vfifo Q full
    if ((UINT8)eSWDMX_MEDIA_VIDEO == u1HdrType)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        //mkv test
        UNUSED(u2MaxQNum);
        if (prSwdmxMkvInfo->u4VidFps > 0)
        {
            u4QNumThrd = prSwdmxMkvInfo->u4VidFps;
        }
        if (prSwdmxMkvInfo->u4AStrmID >= MKV_MAX_AUD_TRACK)
        {
            LOG(3, "Audio StrmID error!\n");
            return FALSE;
        }
        if (((prSwdmxMkvInfo->u4AudPts[prSwdmxMkvInfo->u4AStrmID] < prSwdmxMkvInfo->u4CurVidPts) &&
            ((prSwdmxMkvInfo->u4CurVidPts - prSwdmxMkvInfo->u4AudPts[prSwdmxMkvInfo->u4AStrmID]) > 45000)&&prSwdmxMkvInfo->fgEnAudio) ||
            (prSwdmxMkvInfo->fgAudTrigVidDec))
        {
            u4QNumThrd = u2MaxQNum>>1; //audio data behind video, don't block data sending here
        }

        if(prSwdmxMkvInfo->fgHDRDELIVER == TRUE)
        {
            if (u2CurQNum + 2 > (UINT16)u4QNumThrd)
            {
                if (prSwdmxMkvInfo->fgAudTrigVidDec)
                {
                    LOG(3, "vesq full and audio data is not parsed yet, force play video\n");
                    _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
                }
                x_thread_delay(10);
                prSwdmxMkvInfo->fgHDRDELIVER = false;
                prSwdmxInst->fgFifoFull = TRUE;
                return FALSE;
            }
        }
        else
        {
            if (u2CurQNum > (UINT16)u4QNumThrd)
            {
                if (prSwdmxMkvInfo->fgAudTrigVidDec)
                {
                    LOG(3, "vesq full and audio data is not parsed yet, force play video\n");
                    _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
                }
                x_thread_delay(10);
                prSwdmxInst->fgFifoFull = TRUE;
                return FALSE;
            }
        }


        if (bIsFdrBuf &&
            (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_VC1))
        {
            if (!_ChkWMVSC(pucFifo))
            {
                if (!_vMKVDeliverData(u1SrcId, u1HdrType, (UCHAR*)prSwdmxMkvInfo->aucVC1Hdr, VC1_HDR_LEN, FALSE, FALSE))
                {
                    return FALSE;
                }
            }
        }
        if (((prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
            (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
            (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV9))
        {
            rDmxMMData.fgFrameHead = TRUE;
        }
        if ((prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME) &&
            (rDmxMMData.u4Pts > prSwdmxMkvInfo->u4KeyPTS) &&
            (!prSwdmxMkvInfo->fgWaitIFrm))
        {
            prSwdmxMkvInfo->fgKeyDelivered = TRUE;
            return TRUE;
        }
        if (bIsFdrBuf && prSwdmxMkvInfo->fgWaitIFrm)
        {
            if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                u4NaluTypeAddr = rDmxMMData.u4StartAddr;
                //AVI type remove start code
                if (!bInsertSC)
                {
                    prSwdmxMkvInfo->fgWaitIFrm = FALSE;
                }
                else
                {
                    u4SliceTypeAddr = u4NaluTypeAddr + 1;
                    if (u4SliceTypeAddr == prFeederMkvInfo->u4EndAddr)
                    {
                        u4SliceTypeAddr = prFeederMkvInfo->u4StartAddr;
                    }
                    if (IS_1ST_SLICE(*(UINT8*)VIRTUAL(u4SliceTypeAddr)) &&    //first slice in frame
                        (IS_IDR_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)) ||
                        IS_HDR_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)) ||
                        (IS_FRM_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)) &&
                        (_ChkAVCFrmType((UCHAR*)VIRTUAL(u4SliceTypeAddr)) == AVC_I_FRM))))
                    {
                        LOG(5, "WaitI StartPTS 0x%x\n", rDmxMMData.u4Pts);
                        if (!IS_IDR_NALU(*(UINT8*)VIRTUAL(u4NaluTypeAddr)))
                        {
                            LOG(3, "Key!=IDR\n");
                        }
                        prSwdmxMkvInfo->fgWaitIFrm = FALSE;
                        if (prSwdmxMkvInfo->u8AudSeekPts < rDmxMMData.u4Pts)
                        {
                            prSwdmxMkvInfo->u8AudSeekPts = rDmxMMData.u4Pts;
                        }
                        if (!prSwdmxMkvInfo->fgKeyDelivered)
                        {
                            if (prSwdmxMkvInfo->u4KeyPTS < rDmxMMData.u4Pts)
                            {
                                prSwdmxMkvInfo->u4KeyPTS = rDmxMMData.u4Pts;
                                prSwdmxMkvInfo->u4KeyDeliveredPTS = rDmxMMData.u4Pts;
                            }
                        }
                    }
                    else
                    {
                        return TRUE;
                    }
                }
            }
            else
            {
                prSwdmxMkvInfo->fgWaitIFrm = FALSE;
            }
        }
    }
    else if (bIsFdrBuf && ((UINT8)eSWDMX_MEDIA_AUDIO == u1HdrType))
    {
        if (ENUM_SWDMX_ADEC_AAC == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4CurAStrmID]
        && (!_ChkAACSW(pucFifo)))
        {
             // insert ADTS header if no sync word
            _GenAACHdr(u1SrcId, (UCHAR*)prSwdmxMkvInfo->aucAACHdr, u4DataLen + AAC_HDR_LEN);
            if (!_vMKVDeliverData(u1SrcId, u1HdrType, (UCHAR*)prSwdmxMkvInfo->aucAACHdr, AAC_HDR_LEN, FALSE, FALSE))
            {
                return FALSE;
            }
        }
        else if (ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4CurAStrmID])
        {
            UINT32 u4OggHdrSz = _GenOggHdr(u1SrcId, (UCHAR*)prSwdmxMkvInfo->aucOggHdr, u4DataLen);
            if (!u4OggHdrSz)
            {
                return FALSE;
            }
            if (!_vMKVDeliverData(u1SrcId, u1HdrType, (UCHAR*)prSwdmxMkvInfo->aucOggHdr, u4OggHdrSz, FALSE, FALSE))
            {
                return FALSE;
            }
        }
    }

    if (bInsertSC)
    {
        if (!_vMKVDeliverData(u1SrcId, u1HdrType, (UCHAR*)prSwdmxMkvInfo->aucAVCHdr, AVC_HDR_LEN, FALSE, FALSE))
        {
            return FALSE;
        }
    }
    //check if data is in feader buffer
    if (!bIsFdrBuf)
    {
        rDmxMMData.u4BufStart = rDmxMMData.u4StartAddr & 0xFFFFFFF0;
        rDmxMMData.u4BufEnd = (((rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize) + 0xf) & 0xFFFFFFF0) + 192;
    }
    else
    {
        rDmxMMData.u4BufStart = prFeederMkvInfo->u4StartAddr;
        rDmxMMData.u4BufEnd = prFeederMkvInfo->u4EndAddr;
        if (rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
        {
            // in feeder RING_BUF_RSV region
            rDmxMMData.u4BufStart -= RING_BUF_RSV;
        }
        else if (rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
        {
            rDmxMMData.u4StartAddr =
                (rDmxMMData.u4StartAddr - rDmxMMData.u4BufEnd) + rDmxMMData.u4BufStart;
        }
    }
    if (u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMData.u1Idx = prSwdmxMkvInfo->u1VidPid;
    }
    else if (u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO)
    {
        if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
        {
            rDmxMMData.u1Idx = prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4CurAStrmID];
        }
        else
        {
            rDmxMMData.u1Idx = prSwdmxMkvInfo->u1AudPid;
        }
    }
    else if (u1HdrType == (UINT8)eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMData.u1Idx = prSwdmxMkvInfo->u1SubPid;
    }
    else
    {
        rDmxMMData.u1Idx = u1HdrType;
    }

    ASSERT((rDmxMMData.u4StartAddr < rDmxMMData.u4BufEnd) &&
        (rDmxMMData.u4StartAddr >= rDmxMMData.u4BufStart));

    if (u1HdrType == (UINT8)eSWDMX_MEDIA_SUBTITLE)
    {
        prSwdmxMkvInfo->fgPendingSbMoveData = TRUE;
        x_memcpy((void*)&prSwdmxMkvInfo->rSbDmxMMData, (void*)&rDmxMMData, sizeof(rDmxMMData));
    }
    else
    {
        if (u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO)
        {
            if (bIsFdrBuf)   //audio data
            {
                LOG(7, "AUDIO PTS=%d\n",rDmxMMData.u4Pts);
                if (_SWDMX_MoveData(u1SrcId, &rDmxMMData) != TRUE)
                {
                    LOG(3, "Demuxer fails to move data.\n");
                    return FALSE;
                }
            }
            else    // aac/ogg header part
            {
                rDmxMMData.u4Pts = 0;//invalid PTS, prSwdmxMkvInfo->rAudDmxMMData.u4Pts;
                if (_SWDMX_MoveData(u1SrcId, &rDmxMMData) != TRUE)
                {
                    LOG(3, "Demuxer fails to move data.\n");
                    return FALSE;
                }
            }
        }
        else if (u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO)
        {
            if (_SWDMX_MoveData(u1SrcId, &rDmxMMData) != TRUE)
            {
                LOG(3, "Demuxer fails to move data.\n");
                return FALSE;
            }
        }
    }
    return TRUE;
}

static BOOL _vMKVBlockParsing(UINT8 u1SrcId, UINT8* pucBuf, UINT32 u4BlockSz, UINT32* pu4ConsumedSz)
{
    UINT8* pucFifo = pucBuf;//(BYTE *) _pbHdrBuf;
    UINT8* pucSz = NULL;
    UINT32 u4VintIDSz = _EbmlGetSz((UINT32)*pucFifo);
    UINT32 u4HeaderSz = u4VintIDSz + 3; // timecode: 2 bytes and flag: 1 byte
    UINT32 u4MkvTrackNo, u4CurDispPts, u4APts;
    INT32 i4BlockTimeCode;
    INT16 i2MkvTimeCode = 0;
    UINT8 u1MkvFlag = 0;
    BOOL fgSkipBlock = TRUE;
    UINT64 u8Remainder;
    INT32 i4Subtraction;
    INT32 i4StcNow;
    UINT8 i = 0;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    u4MkvTrackNo = _EbmlGetValueLess4(pucFifo);

    if (u4MkvTrackNo >= MKV_MAX_AUD_TRACK)
    {
        LOG(3, "%s(%d): u4MkvTrackNo(%d) is too large\n", __FUNCTION__, __LINE__, u4MkvTrackNo);
        ASSERT(u4MkvTrackNo < MKV_MAX_AUD_TRACK);
        prSwdmxMkvInfo->u1StrmType = 0xFF;
        prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
        prSwdmxMkvInfo->fgDrmEnable = FALSE;
        *pu4ConsumedSz += u4BlockSz;
        return TRUE;
    }

    LOADB_1WORD((pucFifo + u4VintIDSz), i2MkvTimeCode);
    LOAD_1BYTE((pucFifo + u4VintIDSz + 2), u1MkvFlag);

    i4BlockTimeCode = (INT32)i2MkvTimeCode + (INT32)prSwdmxMkvInfo->u4ClusterTimeCode;

    // get block PTS
    #if 0
    if (prSwdmxMkvInfo->u4KeyFrmPTS > 0)  //trick mode
    {
        prSwdmxMkvInfo->u4CurBlockPts = prSwdmxMkvInfo->u4KeyFrmPTS;
    }
    else
    #endif
    {
        prSwdmxMkvInfo->u4CurBlockPts =
            (UINT32)u8Div6432(((UINT64)i4BlockTimeCode *
            prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale), TIME_CODE_SCALE, &u8Remainder);
    }

   if ((!prSwdmxMkvInfo->fgEnVideo)&& (u4MkvTrackNo < MKV_MAX_AUD_TRACK) &&     //is audio track and format support
        prSwdmxMkvInfo->fgIsAudTrack[u4MkvTrackNo])
    {
        i4StcNow = STC_GetStcValue(prSwdmxInst->u1StcId);
           i4Subtraction = (INT32)(prSwdmxMkvInfo->u4CurBlockPts) - i4StcNow;
        if (i4Subtraction > 450000)
        {
               return FALSE;
        }
        LOG(6,"prSwdmxMkvInfo->u4CurBlockPts=%x\n",prSwdmxMkvInfo->u4CurBlockPts);
    }

    // ignore block until seek done
    if (prSwdmxMkvInfo->fgIsSeek)
    {
        if ((prSwdmxMkvInfo->u4CurBlockPts < (UINT32)prSwdmxMkvInfo->u8SeekPts) &&
            (u4MkvTrackNo == prSwdmxMkvInfo->u4VStrmID))
        {
            u4MkvTrackNo = SKIP_STRM;
        }
        else if ((u4MkvTrackNo == prSwdmxMkvInfo->u4VStrmID) || (!prSwdmxMkvInfo->fgEnVideo))
        {
            LOG(5, "Seek done, BlockPTS 0x%x, Type (%d, A%d, V%d)\n",
                prSwdmxMkvInfo->u4CurBlockPts, u4MkvTrackNo,
                prSwdmxMkvInfo->u4AStrmID,
                prSwdmxMkvInfo->u4VStrmID);
            prSwdmxMkvInfo->u8SeekPts = 0xFFFFFFFF;
            prSwdmxMkvInfo->u8SeekPts <<= 32;
            prSwdmxMkvInfo->u8SeekPts += 0xFFFFFFFF;
            prSwdmxMkvInfo->fgIsSeek = FALSE;
        }
    }
    // set audio/video stream
    if ((u4MkvTrackNo == prSwdmxMkvInfo->u4VStrmID)/* && prSwdmxMkvInfo->fgEnVideo*/)
    {
        if (prSwdmxMkvInfo->fgEnVideo)
        {
            prSwdmxMkvInfo->u1StrmType = (UINT8)eSWDMX_MEDIA_VIDEO;
            prSwdmxMkvInfo->u4CurVidPts = prSwdmxMkvInfo->u4CurBlockPts;
            fgSkipBlock = FALSE;
            LOG(6,"prSwdmxMkvInfo->u4CurBlockPts=%x\n",prSwdmxMkvInfo->u4CurBlockPts);
        }
    }
    else if ((u4MkvTrackNo < MKV_MAX_AUD_TRACK) &&     //is audio track and format support
        prSwdmxMkvInfo->fgIsAudTrack[u4MkvTrackNo])
    {
        ASSERT(u4MkvTrackNo < MKV_MAX_AUD_TRACK);
        if ((u4MkvTrackNo == prSwdmxMkvInfo->u4AStrmID) &&
            prSwdmxMkvInfo->fgEnAudio)                                             //is current audio track
        {
            if ((prSwdmxMkvInfo->u4CurBlockPts >= (UINT32)prSwdmxMkvInfo->u8AudSeekPts) &&
                (!prSwdmxMkvInfo->fgWaitIFrm))   //the flag will be true if key!=I frame
            {
                if (prSwdmxMkvInfo->fgChgATrack
                    || (prSwdmxMkvInfo->u8AudSeekPts > 0)
#ifdef AUD_PREBUF_EN
                    || ((prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF) && prSwdmxMkvInfo->fgActivePidxIsUnKnown)
#endif
                    )
                {
                    u4APts = prSwdmxMkvInfo->u4CurBlockPts;
                    if (prSwdmxMkvInfo->fgEnVideo)
                    {
                        u4CurDispPts = VDP_GetPts(0);
                    }
                    else
                    {
                        u4CurDispPts =  STC_GetStcValue(prSwdmxInst->u1StcId);
                    }
                    #ifdef AUD_PREBUF_EN
                    if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
                    {
                        if (((prSwdmxMkvInfo->i4LastSpeed <= FF_2X)&&(prSwdmxMkvInfo->i4LastSpeed >= STEP_FW)) || prSwdmxMkvInfo->fgChgATrack)
                        {
                            prSwdmxMkvInfo->u8AudSeekPts = u4CurDispPts + 45000;
                        }
                        if (((prSwdmxMkvInfo->i4LastSpeed <= FF_2X)&&(prSwdmxMkvInfo->i4LastSpeed >= STEP_FW)) || prSwdmxMkvInfo->fgChgATrack)
                        {
                            if (!DMX_AudHandler_SeekAndUpdate(prSwdmxMkvInfo->auAudPidx[u4MkvTrackNo],
                            prSwdmxMkvInfo->u8AudSeekPts, &u4APts))
                            {
                                LOG(2, "Mkv DMX_AudHandler_SeekAndUpdate error!\n");
                                u4APts = prSwdmxMkvInfo->u4CurBlockPts;
                            }
                            prSwdmxMkvInfo->i4LastSpeed = 0xFF;
                        }
                        AUD_SetStartPts(prSwdmxInst->u1AdecId, u4APts);
                        if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[u4MkvTrackNo])
                        {
                            DMX_AudHandler_SetActivePidxEx(prSwdmxMkvInfo->auAudPidx[u4MkvTrackNo],
                                (UINT8*)prSwdmxMkvInfo->u4AudHeaderPtr[u4MkvTrackNo],
                                prSwdmxMkvInfo->u4AudHdrLength[u4MkvTrackNo]);
                        }
                        else
                        {
                            DMX_AudHandler_SetActivePidx(prSwdmxMkvInfo->auAudPidx[u4MkvTrackNo]);
                        }
                        prSwdmxMkvInfo->fgActivePidxIsUnKnown = FALSE;
                    }
                    #endif

                    LOG(3, "ChgAtrk %d, AudStartPts 0x%x, CurDisp 0x%x, Diff %ds\n", u4MkvTrackNo,
                        u4APts, u4CurDispPts,
                        (u4APts - u4CurDispPts) / 90000);
                    UNUSED(u4CurDispPts);
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, u4APts);
                    prSwdmxMkvInfo->fgChgATrack = FALSE;
                    prSwdmxMkvInfo->u8AudSeekPts = 0;
                    prSwdmxMkvInfo->u4AudTrigVidDecPTS = u4APts;
                    if (!prSwdmxMkvInfo->fgAudTrigVidDec)
                    {
                        VDP_TriggerAudReceive(u4APts);
                    }
                }
                else if (prSwdmxMkvInfo->u4AudPts[u4MkvTrackNo] == 0xFFFFFFFF)    //for begin playing
                {
                    AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxMkvInfo->u4CurBlockPts);
                }
                prSwdmxMkvInfo->u1StrmType = (UINT8)eSWDMX_MEDIA_AUDIO;
                prSwdmxMkvInfo->u4CurAStrmID = u4MkvTrackNo;
                prSwdmxMkvInfo->u4SBSkipAudPts[u4MkvTrackNo] = prSwdmxMkvInfo->u4CurBlockPts; //for divx subtitle change
                fgSkipBlock = FALSE;
            }
        }
        else if ((prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF) &&             //for pre-buffer
            ((prSwdmxMkvInfo->i4PrevSpeed <= FF_2X) &&
            (prSwdmxMkvInfo->i4PrevSpeed > 0)))
        {
            prSwdmxMkvInfo->u1StrmType = (UINT8)eSWDMX_MEDIA_AUDIO;
            prSwdmxMkvInfo->u4CurAStrmID = u4MkvTrackNo;
            prSwdmxMkvInfo->u4SBSkipAudPts[u4MkvTrackNo] = prSwdmxMkvInfo->u4CurBlockPts; //for divx subtitle change
            fgSkipBlock = FALSE;
        }

        prSwdmxMkvInfo->u4AudPts[u4MkvTrackNo] = prSwdmxMkvInfo->u4CurBlockPts;
    }
    else if ((u4MkvTrackNo == prSwdmxMkvInfo->u4SpStrmID) && prSwdmxMkvInfo->fgEnSubTitle)
    {
        if (prSwdmxMkvInfo->u4CurBlockPts >= (UINT32)prSwdmxMkvInfo->u8AudSeekPts)
        {
            prSwdmxMkvInfo->u1StrmType = (UINT8)eSWDMX_MEDIA_SUBTITLE;
            if (u4MkvTrackNo < MKV_MAX_SB_TRACK)
            {
                prSwdmxMkvInfo->u8SBClusterPos[u4MkvTrackNo] = prSwdmxMkvInfo->u8CurClusterPos;
                prSwdmxMkvInfo->u4SubPts[u4MkvTrackNo]   = prSwdmxMkvInfo->u4CurBlockPts;
            }
            fgSkipBlock = FALSE;
        }
    }
    else if (u4MkvTrackNo < MKV_MAX_SB_TRACK && prSwdmxMkvInfo->fgIsSBTrack[u4MkvTrackNo])
    {
        prSwdmxMkvInfo->u8SBClusterPos[u4MkvTrackNo] = prSwdmxMkvInfo->u8CurClusterPos;
        prSwdmxMkvInfo->u4SubPts[u4MkvTrackNo]   = prSwdmxMkvInfo->u4CurBlockPts;
    }
    if (!prSwdmxMkvInfo->fgChangeSBDone)
    {
        if ((u4MkvTrackNo == prSwdmxMkvInfo->u4VStrmID) && (!prSwdmxMkvInfo->fgIsVidSkipDone) && prSwdmxMkvInfo->fgEnVideo)
        {
            if (prSwdmxMkvInfo->u4SBSkipVidPts != prSwdmxMkvInfo->u4CurBlockPts)
            {
                fgSkipBlock = TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgIsVidSkipDone = TRUE;
                fgSkipBlock = TRUE;
                LOG(1,"skipd=0x%x\n",prSwdmxMkvInfo->u4SBSkipVidPts);
                LOG(5,"video reparsing done\n");
            }
        }
        else if (u4MkvTrackNo < MKV_MAX_AUD_TRACK
            && prSwdmxMkvInfo->fgIsAudTrack[u4MkvTrackNo]
            && (!prSwdmxMkvInfo->fgIsAudSkipDone[u4MkvTrackNo]))
        {
            if (prSwdmxMkvInfo->u4BKSBSkipAudPts[u4MkvTrackNo] >= prSwdmxMkvInfo->u4CurBlockPts)
            {
                fgSkipBlock = TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgIsAudSkipDone[u4MkvTrackNo] = TRUE;
                LOG(5,"audio track =%x, remove data done\n",u4MkvTrackNo);
            }
        }

        if (prSwdmxMkvInfo->fgIsVidSkipDone)
        {
            for (i = 0; i < MKV_MAX_AUD_TRACK; i++)
            {
                if (!prSwdmxMkvInfo->fgIsAudSkipDone[i])
                {
                    break;
                }
            }

            if (i >= MKV_MAX_AUD_TRACK)
            {
                prSwdmxMkvInfo->fgChangeSBDone = TRUE;
                LOG(5,"change subtitle remove data done\n");
            }
        }
    }

    if (fgSkipBlock)
    {
        prSwdmxMkvInfo->u1StrmType = 0xFF;
        prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
        prSwdmxMkvInfo->fgDrmEnable = FALSE;
        *pu4ConsumedSz += u4BlockSz;
        return TRUE;
    }
    else if (prSwdmxMkvInfo->prDmxRangeInfo->b_drm)
    {
        if (!_SWDMXMkvDecDrmData(u1SrcId, prSwdmxMkvInfo->u1StrmType, pucBuf + u4HeaderSz, u4BlockSz-u4HeaderSz))
        {
            LOG(2, "DecDrmData err!\n");
        }
        prSwdmxMkvInfo->fgDrmEnable = FALSE; //for video
    }

    prSwdmxMkvInfo->u4MkvBHeaderSa = (UINT32)pucBuf;//_dMkvFileOfst;
    prSwdmxMkvInfo->u4MkvBHeaderSz = u4HeaderSz;

    if (fgMkvIsNoLacing(u1MkvFlag))
    {
        //mkv test
        if ((prSwdmxMkvInfo->u1StrmType == (UINT8)eSWDMX_MEDIA_VIDEO) &&
            ((prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264) &&
            prSwdmxMkvInfo->u1MkvAvcNaluLen > 0))    // video type V_MPEG4/ISO/AVC
        {
            prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_AVC;
            *pu4ConsumedSz += prSwdmxMkvInfo->u4MkvBHeaderSz;
#ifdef MKV_CONTENT_ENCODING_SUPPORT
            prSwdmxMkvInfo->fg1stAvc = TRUE;
#endif
        }
        else
        {
            //prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_NO_LACING; // ToDo: Change
            if (_vMKVContentDecode(u1SrcId, prSwdmxMkvInfo->u1StrmType,
                pucBuf + u4HeaderSz,
                u4BlockSz - u4HeaderSz,
                FALSE, TRUE))
            {
                *pu4ConsumedSz += u4BlockSz;
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
            }
            else
            {
                return FALSE;
            }
        }
    }
    else
    {
        LOAD_1BYTE((pucFifo + u4VintIDSz + 3), prSwdmxMkvInfo->u4MkvFrameCount);
        prSwdmxMkvInfo->u4MkvFrameCount++; // From Spec, real frame count is one more
        if (prSwdmxMkvInfo->u4MkvFrameCount > MKV_MAX_FRAME_COUNT)
        {
            // impossible case
            LOG(1, "Error frame count: %d\n");
            ASSERT(prSwdmxMkvInfo->u4MkvFrameCount <= MKV_MAX_FRAME_COUNT);
            prSwdmxMkvInfo->u4MkvFrameCount = MKV_MAX_FRAME_COUNT;
        }
        prSwdmxMkvInfo->u4MkvBHeaderSz++; // size of _dMkvFrameCount

        prSwdmxMkvInfo->u4MkvFrmCntPrs = 0;
        prSwdmxMkvInfo->u4MkvFrmAccSz = 0;
        x_memset((void*)prSwdmxMkvInfo->au4MkvFrmCntPrsSz, 0,
            sizeof(prSwdmxMkvInfo->au4MkvFrmCntPrsSz));

        if (fgMkvIsXiphLacing(u1MkvFlag))
        {
            prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_XIPH_LACING_HDR;
            *pu4ConsumedSz += prSwdmxMkvInfo->u4MkvBHeaderSz;
        }
        else if (fgMkvIsFixedSizeLacing(u1MkvFlag))
        {
            INT32 i;
            prSwdmxMkvInfo->au4MkvFrmCntPrsSz[0] =
                (u4BlockSz - prSwdmxMkvInfo->u4MkvBHeaderSz)/prSwdmxMkvInfo->u4MkvFrameCount;

            i = prSwdmxMkvInfo->u4MkvFrameCount - 1;
            if (i > 0)
            {
                prSwdmxMkvInfo->au4MkvFrmCntPrsSz[i] =
                    (u4BlockSz - prSwdmxMkvInfo->u4MkvBHeaderSz) - (prSwdmxMkvInfo->au4MkvFrmCntPrsSz[0] * i);
            }

            for (i = prSwdmxMkvInfo->u4MkvFrameCount - 2; i > 0; --i)
            {
                prSwdmxMkvInfo->au4MkvFrmCntPrsSz[i] = prSwdmxMkvInfo->au4MkvFrmCntPrsSz[0];
            }

            prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_LACING;
            *pu4ConsumedSz += prSwdmxMkvInfo->u4MkvBHeaderSz;
        }
        else if (fgMkvIsEBMLLacing(u1MkvFlag))
        {
            prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_EBML_LACING_HDR;
            *pu4ConsumedSz += prSwdmxMkvInfo->u4MkvBHeaderSz;

            pucSz = pucFifo + prSwdmxMkvInfo->u4MkvBHeaderSz;
            prSwdmxMkvInfo->au4MkvFrmCntPrsSz[0] =
                _EbmlGetValueLess4(pucSz);
            prSwdmxMkvInfo->u4MkvFrmAccSz = prSwdmxMkvInfo->au4MkvFrmCntPrsSz[0];
            *pu4ConsumedSz += _EbmlGetSz((UINT32)*pucSz);
        }
    }

    return TRUE;
}


static BOOL _vMKVClusterParsing(
        UINT8 u1SrcId,
        UCHAR* pucBuf,
        SWDMX_RANGE_LIST_T* prRangeList,
        UINT32 u4AvalSz,
        UINT32* pu4ConsumedSz)
{
    UCHAR* pucFifo = pucBuf;
    UINT32 u4VintIDSz = 0, u4HdrSz = 0, u4VintSzLen = 0, u4VintDataSz = 0;
    INT64 i8Val = 0;
    UINT32 u4ValueLess4 = 0, u4ElmtSz = 0, u4Value = 0;
    UINT32 u4Len = 0, u4Level;
    UINT16 u2Val = 0;
    UINT8 u1RsvLen, u1Val = 0, u1AvcNaluLen;
    INT8 i1Val = 0;
    INT16 i2Val = 0;
    UINT64 u8Remainder;
    int i, j, i1;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    *pu4ConsumedSz = 0;
    while((u4AvalSz > (*pu4ConsumedSz + MKV_UNCERTAIN_SZ)) ||
        (prSwdmxMkvInfo->fgReadEOS && (u4AvalSz > (*pu4ConsumedSz))))
    {
        //already find key frame in trick mode
        if ((prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME))
        {
            if (prSwdmxMkvInfo->fgKeyDelivered && !prSwdmxMkvInfo->fgIsTPT)
            {
                break;
            }
        }
        pucFifo = pucBuf + *pu4ConsumedSz;

        //check ring buffer, last data block transfer
        if ((UINT32)pucFifo >= prFeederMkvInfo->u4EndAddr)
        {
            pucFifo = (pucFifo - prFeederMkvInfo->u4EndAddr) + prFeederMkvInfo->u4StartAddr;
        }
        else if ((UINT32)pucFifo >= (prFeederMkvInfo->u4EndAddr - MKV_UNCERTAIN_SZ))
        {
            u1RsvLen = (UINT8)(prFeederMkvInfo->u4EndAddr - (UINT32)pucFifo);
            x_memcpy((void*)(prFeederMkvInfo->u4StartAddr - u1RsvLen), (void*)pucFifo, u1RsvLen);
            HalFlushInvalidateDCacheMultipleLine((prFeederMkvInfo->u4StartAddr - (UINT32)u1RsvLen), (UINT32)u1RsvLen);
            pucFifo = (UINT8*)(prFeederMkvInfo->u4StartAddr - u1RsvLen);

            LOG(3, "Use rsv feederBuf len 0x%x\n", u1RsvLen);
            //handle reserve length with level element address
            for(u4Level = 0; u4Level < MKV_MAX_LEVEL; u4Level++)
            {
                if (prSwdmxMkvInfo->au4MkvElementEa[u4Level] >
                    (prFeederMkvInfo->u4EndAddr - MKV_UNCERTAIN_SZ))
                {
                    prSwdmxMkvInfo->au4MkvElementEa[u4Level] =
                        prFeederMkvInfo->u4StartAddr -
                        (prFeederMkvInfo->u4EndAddr - prSwdmxMkvInfo->au4MkvElementEa[u4Level]);
                }
            }
        }

        //embl parsing
        u4VintIDSz = _EbmlGetSz((UINT32)*pucFifo);
        u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + u4VintIDSz));
        u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + u4VintIDSz) :
            _EbmlGetValueOver4(pucFifo + u4VintIDSz + 4);
        u4HdrSz = u4VintIDSz + u4VintSzLen;
        u4ValueLess4 = _EbmlGetDataValueLess4(pucFifo + u4HdrSz, u4VintDataSz);
        u4ElmtSz = u4HdrSz + u4VintDataSz;
        UNUSED(u4ValueLess4);

        //LOG(4, "State 0x%x\n", prSwdmxMkvInfo->u4PrsState);

        switch(( MKV_ELMT_ID_STATE)prSwdmxMkvInfo->u4PrsState)
        {
        case MKVPRS_G_SEGMENT_ELEMENT_ID: // P.12~13 Table 2
            if (fgMkvIsSegmentClusterID(pucFifo))
            {
                prSwdmxMkvInfo->u8CurClusterPos = prSwdmxMkvInfo->u8CurDmxMoviPos;
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID;
                prSwdmxMkvInfo->u8CurClusterPos = prSwdmxMkvInfo->u8CurDmxMoviPos+*pu4ConsumedSz;
                for(i=0;i<10;i++)
                {
                    if (prSwdmxMkvInfo->au8PostionxTime[i][0]==0)
                    {
                        prSwdmxMkvInfo->au8PostionxTime[i][0]=prSwdmxMkvInfo->u8CurClusterPos;
                        break;
                    }
                }
                if (i==10)
                {
                    for(j=0;j<9;j++)
                    {
                        prSwdmxMkvInfo->au8PostionxTime[j][0]=prSwdmxMkvInfo->au8PostionxTime[j+1][0];
                        prSwdmxMkvInfo->au8PostionxTime[j][1]=prSwdmxMkvInfo->au8PostionxTime[j+1][1];
                    }
                    prSwdmxMkvInfo->au8PostionxTime[9][0]=prSwdmxMkvInfo->u8CurClusterPos;
                }
                prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position=prSwdmxMkvInfo->u8CurDmxMoviPos+*pu4ConsumedSz;
                *pu4ConsumedSz += u4HdrSz;
            }
            else if ((UINT32)pucFifo >= _MkvGetElementEa(u1SrcId, 0))
            {
                LOG(3, "File end?\n");
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
                return TRUE;
            }
            break;
        // 5.5 Segment.Cluster
        case MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID:
            if (fgMkvIsCRC32(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsClusterTimeCodeID(pucFifo))
            {
                u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + 1));
                u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + 1) : _EbmlGetValueOver4(pucFifo + 1 + 4);
                switch (u4VintDataSz)
                {
                    case 1:
                        LOAD_1BYTE(pucFifo + 1 + u4VintSzLen, u1Val);
                        prSwdmxMkvInfo->u4ClusterTimeCode = u1Val;
                        break;
                    case 2:
                        LOADB_1WORD(pucFifo + 1 + u4VintSzLen, u2Val);
                        prSwdmxMkvInfo->u4ClusterTimeCode = u2Val;
                        break;
                    case 3:
                        LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, prSwdmxMkvInfo->u4ClusterTimeCode);
                        prSwdmxMkvInfo->u4ClusterTimeCode >>= 8;
                        break;
                    case 4:
                        LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, prSwdmxMkvInfo->u4ClusterTimeCode);
                        break;
                    default:    // length > 4
                        LOG(3, "Cluster timecode length>4\n");
                        prSwdmxMkvInfo->u4ClusterTimeCode = 0;
                        break;
                }

                for(i1=1;i1<10;i1++)
                {
                    if (prSwdmxMkvInfo->au8PostionxTime[i1][0]==0)
                    {
                        prSwdmxMkvInfo->au8PostionxTime[i1-1][1]=prSwdmxMkvInfo->u4ClusterTimeCode * 90;
                        break;
                    }
                }
                if (i1==10)
                {
                    prSwdmxMkvInfo->au8PostionxTime[9][1]=prSwdmxMkvInfo->u4ClusterTimeCode * 90;
                }
                LOG(3, "cluster time code 0x%x\n",
                    (UINT32)u8Div6432(
                    ((UINT64)prSwdmxMkvInfo->u4ClusterTimeCode * prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale),
                    TIME_CODE_SCALE, &u8Remainder));
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsClusterPositionID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsClusterPrevSizeID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsClusterBlockGroupID(pucFifo))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
                _MkvSetElementRange(u1SrcId, 2, (UINT32)pucFifo, u4ElmtSz);
                *pu4ConsumedSz += u4HdrSz;
            }
            else if (fgMkvIsClusterSimpleBlockID(pucFifo))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_BLOCK_DATA;
                *pu4ConsumedSz += u4HdrSz;
                _MkvSetElementRange(u1SrcId, 2, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
                _MkvSetElementRange(u1SrcId, 4, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
            }
            else if ((UINT32)pucFifo >= _MkvGetElementEa(u1SrcId, 1))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
            }
            else
            {
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
            }
            break;

        case MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID:
            _MkvSetElementRange(u1SrcId, 3, (UINT32)pucFifo, u4ElmtSz);
            if (fgMkvIsBGBlockID(pucFifo))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_BLOCK_DATA;
                *pu4ConsumedSz += u4HdrSz;
                _MkvSetElementRange(u1SrcId, 4, (UINT32)pucFifo + u4HdrSz, u4VintDataSz);
                prSwdmxMkvInfo->u4KeyFrmSz = u4VintDataSz + u4HdrSz;
            }
            else if (fgMkvIsBGReferenceBlockID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsBGBlockDurationID(pucFifo))
            {
                u4VintSzLen = _EbmlGetSz((UINT32)*(pucFifo + 1));
                u4VintDataSz = (u4VintSzLen <= 4) ? _EbmlGetValueLess4(pucFifo + 1) : _EbmlGetValueOver4(pucFifo + 1 + 4);
                switch (u4VintDataSz)
                {
                    case 1:
                        LOAD_1BYTE(pucFifo + 1 + u4VintSzLen, i1Val);
                        prSwdmxMkvInfo->i4BlkDuration = (INT32)i1Val;
                        break;
                    case 2:
                        LOADB_1WORD(pucFifo + 1 + u4VintSzLen, i2Val);
                        prSwdmxMkvInfo->i4BlkDuration = (INT32)i2Val;
                        break;
                    case 3:
                        LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, prSwdmxMkvInfo->i4BlkDuration);
                        prSwdmxMkvInfo->i4BlkDuration >>= 8;
                        break;
                    case 4:
                        LOADB_1DWRD(pucFifo + 1 + u4VintSzLen, prSwdmxMkvInfo->i4BlkDuration);
                        break;
                    default:    // length > 4
                        LOG(3, "BlockDuration length>4\n");
                        prSwdmxMkvInfo->i4BlkDuration = 0;
                        break;
                }
                prSwdmxMkvInfo->i4BlkDuration =
                    (INT32)u8Div6432(((UINT64)prSwdmxMkvInfo->i4BlkDuration * prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale)
                    , TIME_CODE_SCALE, &u8Remainder);
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsBGDrmBlockID(pucFifo))
            {
                if (u4ElmtSz > u4HdrSz)
                {
                    UNUSED(_SWDMXMkvPrsDrmInfo(u1SrcId, (pucFifo+u4HdrSz), (u4ElmtSz-u4HdrSz)));
                }

                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (_IsOutOfElmtRange(u1SrcId, 2, (UINT32)pucFifo))
            {   // Return to Parent
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID;

                // for subtitle, block duration may be parsed after block data
                if (prSwdmxMkvInfo->fgPendingSbMoveData)
                {
                    prSwdmxMkvInfo->rSbDmxMMData.u4Dts = (UINT32)prSwdmxMkvInfo->i4BlkDuration;
                    if (_SWDMX_MoveData(u1SrcId,
                            &prSwdmxMkvInfo->rSbDmxMMData) != TRUE)
                    {
                        LOG(3, "Demuxer fails to move data.\n");
                    }
                    prSwdmxMkvInfo->fgPendingSbMoveData = FALSE;
                }
            }
            else // Unknow ID, skip this element
            {
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
            }
            break;

        case MKVPRS_G_BLOCK_DATA:
            if ((u4AvalSz < ((*pu4ConsumedSz/* + MKV_UNCERTAIN_SZ*/) + _MkvGetElementSz(u1SrcId, 4)))/* &&
                !prSwdmxMkvInfo->fgReadEOS*/)
            {
                prSwdmxMkvInfo->fgIsUnderFlw = TRUE;
                LOG(6, "Data !enough\n");
                return TRUE;
            }

            if (_SWDMX_GetNumOfCmd(u1SrcId))
            {
                if (prSwdmxMkvInfo->i4Speed >= FF_2X || IsBackward(prSwdmxMkvInfo->i4Speed))
                {
                    prSwdmxMkvInfo->fgKeyDelivered = TRUE;
                }
                return TRUE;
            }

            if (!_vMKVBlockParsing(u1SrcId, pucFifo, _MkvGetElementSz(u1SrcId, 4), pu4ConsumedSz))
            {
                return TRUE;
            }
            break;

        case MKVPRS_G_DATA_AVC:
            u1AvcNaluLen = prSwdmxMkvInfo->u1MkvAvcNaluLen;

#ifdef MKV_CONTENT_ENCODING_SUPPORT
            if (prSwdmxMkvInfo->fg1stAvc && prSwdmxMkvInfo->tVidContentEncodings.fgValid)
            {
                if (prSwdmxMkvInfo->tVidContentEncodings.ptContentEncoding->u4Type == MKV_CONTENT_COMPRESSION)
                {
                    SWDMX_MKV_CONTENT_COMPRESSION_T *ptCompression =
                        &prSwdmxMkvInfo->tVidContentEncodings.ptContentEncoding->tCompression;

                    if (ptCompression->u4CompAlgo == MKV_CONTENT_ALGO_HEADER_STRIPPING)
                    {
                        if (u1AvcNaluLen > ptCompression->u4CompSettingsLen)
                        {
                            u1AvcNaluLen -= ptCompression->u4CompSettingsLen;
                        }
                        else
                        {
                            LOG(3, "unexpected len: %d\n", ptCompression->u4CompSettingsLen);
                            ASSERT(0);
                        }
                    }
                }
            }
#endif

            u4Len = _vMKVGetNaluSize(pucFifo, u1AvcNaluLen);

            if (_IsOutOfElmtRange(u1SrcId, 4, (UINT32)pucFifo))
            { // Return to Parent
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
            }
            else
            //transfer data to A/V fifo by hw dmx
            {
                if ((u4AvalSz < (*pu4ConsumedSz + (u1AvcNaluLen + u4Len))) &&
                    !prSwdmxMkvInfo->fgReadEOS)
                {
                    prSwdmxMkvInfo->fgIsUnderFlw = TRUE;
                    LOG(3, "Data !enough\n");
                    return TRUE;
                }
                if (_vMKVContentDecode(u1SrcId, (UINT8)eSWDMX_MEDIA_VIDEO,
                    pucFifo + u1AvcNaluLen,
                    u4Len,
                    TRUE,
                    TRUE))  //mkv test
                {
                    *pu4ConsumedSz += (u1AvcNaluLen + u4Len);
#ifdef MKV_CONTENT_ENCODING_SUPPORT
                    prSwdmxMkvInfo->fg1stAvc = FALSE;
#endif
                }
                else
                {
                    return TRUE;
                }
            }
            break;

        case MKVPRS_G_DATA_XIPH_LACING_HDR:
            if (prSwdmxMkvInfo->u4MkvFrmCntPrs <
                (prSwdmxMkvInfo->u4MkvFrameCount - 1))
            {
                do{
                    LOAD_1BYTE(pucFifo, u4Value);
                    *pu4ConsumedSz += 1;
                    pucFifo += 1;
                    prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] += u4Value;
                }while (u4Value == 0xFF);

                prSwdmxMkvInfo->u4MkvFrmAccSz +=
                    prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] ;
                prSwdmxMkvInfo->u4MkvFrmCntPrs++;
            }
            else if (prSwdmxMkvInfo->u4MkvFrmCntPrs ==
                (prSwdmxMkvInfo->u4MkvFrameCount - 1)) //last frame
            {
                prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] =
                    _CalDataSize(u1SrcId, (UINT32)pucFifo, _MkvGetElementEa(u1SrcId, 4)) -
                    prSwdmxMkvInfo->u4MkvFrmAccSz;

                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_LACING;
                prSwdmxMkvInfo->u4MkvFrmCntPrs = 0;
                prSwdmxMkvInfo->u4MkvFrmAccSz = 0;
            }
            break;

        case MKVPRS_G_DATA_EBML_LACING_HDR:
            prSwdmxMkvInfo->u4MkvFrmCntPrs++;
            if (prSwdmxMkvInfo->u4MkvFrmCntPrs <
                (prSwdmxMkvInfo->u4MkvFrameCount - 1))
            {
                i8Val = (INT64)_EbmlGetValueLess4(pucFifo) - _VsintSubtr((UINT8)u4VintIDSz);
                i8Val += (INT64)prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs - 1];
                i8Val = (i8Val > 0) ? i8Val : 0;
                prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] = (UINT32)i8Val;
                prSwdmxMkvInfo->u4MkvFrmAccSz +=
                    prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] ;
                *pu4ConsumedSz += u4VintIDSz;
            }
            else if (prSwdmxMkvInfo->u4MkvFrmCntPrs ==
                (prSwdmxMkvInfo->u4MkvFrameCount - 1)) //last frame
            {
                prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs] =
                    _CalDataSize(u1SrcId, (UINT32)pucFifo, _MkvGetElementEa(u1SrcId, 4)) -
                    prSwdmxMkvInfo->u4MkvFrmAccSz;

                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_DATA_LACING;
                prSwdmxMkvInfo->u4MkvFrmCntPrs = 0;
                prSwdmxMkvInfo->u4MkvFrmAccSz = 0;
            }
            break;

        case MKVPRS_G_DATA_LACING:
            if (prSwdmxMkvInfo->u4MkvFrmCntPrs <
                prSwdmxMkvInfo->u4MkvFrameCount)
            {
                if ((u4AvalSz < ((*pu4ConsumedSz + MKV_UNCERTAIN_SZ) +
                    prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs])) &&
                    !prSwdmxMkvInfo->fgReadEOS)
                {
                    prSwdmxMkvInfo->fgIsUnderFlw = TRUE;
                    LOG(3, "Data !enough\n");
                    return TRUE;
                }
                if (_vMKVContentDecode(u1SrcId, prSwdmxMkvInfo->u1StrmType,
                        pucFifo,
                        prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs],
                        FALSE, TRUE))
                {
                    *pu4ConsumedSz +=
                        prSwdmxMkvInfo->au4MkvFrmCntPrsSz[prSwdmxMkvInfo->u4MkvFrmCntPrs];
                    prSwdmxMkvInfo->u4MkvFrmCntPrs++;
                }
                else
                {
                    return TRUE;
                }
            }
            else
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID;
            }
            break;

            // 5.6 Segment.Cues.CuePoont
        case MKVPRS_G_SEGMENT_CP_ID:
            _MkvSetElementRange(u1SrcId, 2, (UINT32)pucFifo, u4ElmtSz);
            if (fgMkvIsCRC32(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsCueCuePointID(pucFifo))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CP_ELEMENT_ID;
                *pu4ConsumedSz += u4ElmtSz;
            }
            /*else if (_dMkvFileOfst >= dMkvGetElementEa(1))
            { // Return to Parent
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
                vMkvPrsNextG(0);
            }*/
            else // Unknow ID, skip this element
            {
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
            }
            break;

        case MKVPRS_G_SEGMENT_CP_ELEMENT_ID:
            _MkvSetElementRange(u1SrcId, 3, (UINT32)pucFifo, u4ElmtSz);
            if (fgMkvIsCPTimeID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsCPTrackPositionID(pucFifo))
            {
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CP_TP_ELEMENT_ID;
                *pu4ConsumedSz += u4HdrSz;
            }
            /*else if (_dMkvFileOfst >= dMkvGetElementEa(2))
            { // Return to Parent
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CP_ID;
                vMkvPrsNextG(0);
            }*/
            else // Unknow ID, skip this element
            {
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
            }
            break;
        case MKVPRS_G_SEGMENT_CP_TP_ELEMENT_ID:
            _MkvSetElementRange(u1SrcId, 4, (UINT32)pucFifo, u4ElmtSz);
            if (fgMkvIsTPTrackID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsTPClusterPositionID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            else if (fgMkvIsTPBlockNumberID(pucFifo))
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            /*else if (_dMkvFileOfst >= dMkvGetElementEa(3))
            { // Return to Parent
                prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_CP_ELEMENT_ID;
                vMkvPrsNextG(0);
            }*/
            else // Unknow ID, skip this element
            {
                //error handling
                if (u4ElmtSz == 0)
                {
                    *pu4ConsumedSz = u4AvalSz;
                    return TRUE;
                }
                else
                {
                    *pu4ConsumedSz += u4ElmtSz;
                }
            }
            break;

        default:
            //error handling
            if (u4ElmtSz == 0)
            {
                *pu4ConsumedSz = u4AvalSz;
                return TRUE;
            }
            else
            {
                *pu4ConsumedSz += u4ElmtSz;
            }
            break;
        }
    }

    return TRUE;
}


static BOOL _bReqDataFromFeeder(UINT8 u1SrcId, UINT64 u8FileOffset, UINT64 u8ReadSize)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    UINT32 u4QrySize;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    x_memset((void*)&rFeederCond, 0, sizeof(FEEDER_TRANSMIT_COND));
    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prFeederMkvInfo) !=
        FEEDER_E_OK)
    {
        LOG(2, "Feeder input buffer error.\n");
        return FALSE;
    }

    u4QrySize = (UINT32)cal_min(u8ReadSize, MIN_READ_SIZE);

    if (FeederSetRequest(prSwdmxInst->eFeederSrc,
        u4QrySize,
        u8FileOffset,
        &rFeederCond,
        prSwdmxMkvInfo->u4FeederReqID) != FEEDER_E_BUFFER_FULL)
    {
        prSwdmxMkvInfo->u4LastAlignOffset = 0;
        prSwdmxMkvInfo->i4ReqReadTimes++;
        prSwdmxMkvInfo->u4FeederReqID++;
        LOG(6, "Req wptr 0x%x\n", prFeederMkvInfo->u4WriteAddr);
    }
    else
    {
        LOG(3, "Feeder Buffer Full\n");
        //return FALSE;
    }
    return TRUE;
}


static BOOL _bGetDataFromFeederAck(UINT8 u1SrcId, FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (!_SWDMX_ReceiveFeederAck(u1SrcId, (VOID*)prFeederData))
    {
        prSwdmxMkvInfo->fgFeederInvalid = TRUE;
        LOG(3, "GetDataFromFeeder FeederTimeOut!!!\n");
        return FALSE;
    }
    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "GetDataType=FEEDER_SOURCE_INVALID, Q\n"/*, _SWDMX_GetFeederQueueSize()*/);
        return FALSE;
    }

    if ((prSwdmxMkvInfo->i4ReqReadTimes == 0) ||
        (prFeederData->u4Id != (prSwdmxMkvInfo->u4FeederReqID - prSwdmxMkvInfo->i4ReqReadTimes)))
    {
        LOG(3, "ReqTimesErr\n");
        return FALSE;
    }
     prSwdmxMkvInfo->u4LastAlignOffset = prFeederData->u4AlignOffset;
    prSwdmxMkvInfo->u8CurMoviPos += prFeederData->u4ReadSize -prSwdmxMkvInfo->u4LastAlignOffset;
    prSwdmxMkvInfo->u4LeftBytesInFeeder += prFeederData->u4ReadSize -prSwdmxMkvInfo->u4LastAlignOffset;
    prSwdmxMkvInfo->i4ReqReadTimes--;
    if (prFeederData->u4ReadSize > (MIN_READ_SIZE + FDR_ALIGN_SIZE))    //error handling
    {
        LOG(3, "ReqDataErr\n");
        return FALSE;
    }
    return TRUE;
}


static BOOL _bCalculateRepeatFrame(UINT8 u1SrcId, INT32 i4PlaySpeed)
{
    UINT32 u4CurKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl;
    UINT32 u4CurRptPTS = 0, u4CurSentPTS = 0, u4EntryCnt;
    UINT32 u4PtsDif = 0, u4MaxRepeatFrame;

    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (prSwdmxMkvInfo->u4VidRepeatFrame != 0)
    {
        prSwdmxMkvInfo->u4VidRepeatFrame--;
        return TRUE;
    }

    u4CurKey = prSwdmxMkvInfo->u4CurSentKey;
    u4EntryCnt = 0;
    prKeyIdxTbl = prSwdmxMkvInfo->prVidKeyIdxTbl;
    while(prKeyIdxTbl->pt_next_partial &&
        (u4CurKey >= (u4EntryCnt + prKeyIdxTbl->ui4_number_of_entry)))
    {
        u4EntryCnt += prKeyIdxTbl->ui4_number_of_entry;
        prKeyIdxTbl = prKeyIdxTbl->pt_next_partial;
    }
    u4CurSentPTS = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey - u4EntryCnt].ui4_pts;

    u4MaxRepeatFrame = (prSwdmxMkvInfo->u4VidFps > 10) ? prSwdmxMkvInfo->u4VidFps : 30;

    if (prSwdmxMkvInfo->u4CurRepeatFrameIdx != u4CurKey &&
        (prSwdmxMkvInfo->u4CurRepeatFrameIdx < prSwdmxMkvInfo->u4NumOfKeyEntry))
    {
        //if key frame is not I picture, key table will be updated in _MKVCuePointParsing
        u4CurRptPTS = prSwdmxMkvInfo->u4CurRptPTS;

        if (i4PlaySpeed > FF_2X)
        {
            if (u4CurKey < prSwdmxMkvInfo->u4NumOfKeyEntry)
            {
                u4PtsDif = (u4CurSentPTS > u4CurRptPTS) ? (u4CurSentPTS - u4CurRptPTS) : 0;
#ifdef MKV_TRICK_REFINE
                prSwdmxMkvInfo->u4VidRepeatFrame =
                    (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * i4PlaySpeed) << 0));
#else
                prSwdmxMkvInfo->u4VidRepeatFrame =
                    (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * i4PlaySpeed) << 1));
#endif
            }
            else
            {
                prSwdmxMkvInfo->u4VidRepeatFrame = (u4MaxRepeatFrame * 1000)/cal_abs(i4PlaySpeed);
            }
        }
        else if (IsBackward(i4PlaySpeed))
        {
            u4PtsDif = (u4CurRptPTS > u4CurSentPTS) ? (u4CurRptPTS - u4CurSentPTS) : 0;

#ifdef MKV_TRICK_REFINE
            prSwdmxMkvInfo->u4VidRepeatFrame =
                    (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * cal_abs(i4PlaySpeed)) << 0));
#else
            prSwdmxMkvInfo->u4VidRepeatFrame =
                    (UINT32)((u4PtsDif * u4MaxRepeatFrame)/((90 * cal_abs(i4PlaySpeed)) << 1));
#endif
        }

        prSwdmxMkvInfo->u4VidRepeatFrame = (u4MaxRepeatFrame > prSwdmxMkvInfo->u4VidRepeatFrame) ?
            (prSwdmxMkvInfo->u4VidRepeatFrame) : u4MaxRepeatFrame;

        LOG(3, "u4PtsDif = %ld CurKeyIdx = %ld, RepeatIdx = %d, Repeat frame = %ld\n",
            u4PtsDif, u4CurKey, prSwdmxMkvInfo->u4CurRepeatFrameIdx, prSwdmxMkvInfo->u4VidRepeatFrame);

        if ((u4CurKey == prSwdmxMkvInfo->u4NumOfKeyEntry - 1) ||
            (u4CurKey == 0))    // must display
        {
            prSwdmxMkvInfo->u4VidRepeatFrame = u4MaxRepeatFrame << 1;
                /*(UINT32)((prSwdmxMkvInfo->u4VidRepeatFrame < MIN_REPEAT_FRM) ?
                MIN_REPEAT_FRM : prSwdmxMkvInfo->u4VidRepeatFrame);*/
        }
        else if (prSwdmxMkvInfo->u4VidRepeatFrame < MIN_REPEAT_FRM)
        {
            prSwdmxMkvInfo->u4VidRepeatFrame = 0;
            return FALSE;
        }

        prSwdmxMkvInfo->u4CurRepeatFrameIdx = u4CurKey;
        prSwdmxMkvInfo->u4CurRptPTS = u4CurSentPTS;
    }
    else
    {
        if (prSwdmxMkvInfo->u4CurRepeatFrameIdx == (UINT32)0xFFFFFFFF)    // first key frame
        {
            prSwdmxMkvInfo->u4VidRepeatFrame = (u4MaxRepeatFrame * 1000)/cal_abs(i4PlaySpeed);
            prSwdmxMkvInfo->u4CurRepeatFrameIdx = u4CurKey;
            prSwdmxMkvInfo->u4CurRptPTS = u4CurSentPTS;
        }
        else
        {
            return FALSE;
        }
    }

    return TRUE;
}

static BOOL _SWDMX_MkvSeekCurPos(UINT8 u1SrcId)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    VDP_POS_INTO_T rVdpPosInfo;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =(SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    if (!prSwdmxMkvInfo->fgEnVDmx)
    {
       return FALSE;
    }

    _SWDMXResetFeeder(u1SrcId);
    if (VDP_GetPosInfo(prSwdmxInst->u1B2rId,&rVdpPosInfo))
    {
        prSwdmxMkvInfo->u8CurDmxMoviPos=rVdpPosInfo.u8OffsetI;
        prSwdmxMkvInfo->u4CurVidPts=(UINT32)rVdpPosInfo.u8Pts;
        prSwdmxMkvInfo->u4CurRptPTS = prSwdmxMkvInfo->u4CurVidPts;
        prSwdmxMkvInfo->u8CurMoviPos = prSwdmxMkvInfo->u8CurDmxMoviPos;
        prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;
        prSwdmxMkvInfo->u8AudSeekPts=prSwdmxMkvInfo->u4CurVidPts;
        prSwdmxMkvInfo->fgWaitIFrm = TRUE;//new trick mode also need to drop the frame before IDR in cluster
        return TRUE;
    }
    else
    {
        prSwdmxMkvInfo->u8CurDmxMoviPos = 0;
        prSwdmxMkvInfo->u8CurMoviPos = 0;
        return FALSE;
    }
}

static BOOL _bSeekTime(UINT8 u1SrcId, UINT32 u4SeekTime, SWDMX_RANGE_LIST_T* prRangeList)
{
    INT32 i;
    UINT32 u4PrevIdx = 0;
    BOOL fgFindKey = FALSE;

    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = NULL;
    MM_RANGE_ELMT_IDXTBL_T *prPrevKeyIdxTbl = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    prVidKeyIdxTbl = prSwdmxMkvInfo->prVidKeyIdxTbl;
    prPrevKeyIdxTbl = prVidKeyIdxTbl;

    LOG(7, "MkvSeek u4SeekTime = 0x%x\n", u4SeekTime);

    _SWDMXResetFeeder(u1SrcId);
    prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;

    if (prVidKeyIdxTbl)
    {
        if (prVidKeyIdxTbl->ui4_number_of_entry == 0)
        {
            LOG(5, "MkvSeek doesn't have the key table\n");
            return FALSE;
        }
        prSwdmxMkvInfo->u4CurSentKey = 0;

        //check if target pts is less than first key entry
        if (u4SeekTime < prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts)
        {
            LOG(3, "TargetPTS < key entry0 0x%x, set to file begin\n",
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
            prSwdmxMkvInfo->u8CurDmxMoviPos = 0;
            prSwdmxMkvInfo->u4CurVidPts = 0;
        }
        else
        {
            do
            {
                //check if there's next key table
                for (i = 0; i < (INT32)prVidKeyIdxTbl->ui4_number_of_entry; i++)
                {
                    if (prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u4SeekTime)
                    {
                        fgFindKey = TRUE;
                        break;
                    }
                }
                prSwdmxMkvInfo->u4CurSentKey += (UINT32)((i > 0) ? (i - 1) : i);
                if (!fgFindKey)
                {
                    if (prVidKeyIdxTbl->pt_next_partial)
                    {
                        u4PrevIdx = prSwdmxMkvInfo->u4CurSentKey;
                        prPrevKeyIdxTbl = prVidKeyIdxTbl;
                        prVidKeyIdxTbl = prVidKeyIdxTbl->pt_next_partial;
                    }
                    else
                    {
                        LOG(3, "keyTbl missing\n");
                        break;
                    }
                }
                else if (i == 0) //prev table
                {
                    i = u4PrevIdx;
                    prVidKeyIdxTbl = prPrevKeyIdxTbl;
                }
            }while(!fgFindKey);

            /*if (IsBackward(prSwdmxMkvInfo->i4PrevSpeed) &&
                (prSwdmxMkvInfo->u4CurSentKey < (prVidKeyIdxTbl->ui4_number_of_entry - 1)))
            {
                prSwdmxMkvInfo->u4CurSentKey++;  // for render PTS
            }*/

            prSwdmxMkvInfo->u8CurDmxMoviPos =
                (prVidKeyIdxTbl->ui8_base_offset +
                (UINT64)prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[((i > 0) ? (i - 1) : i)].ui4_size) -
                prSwdmxMkvInfo->u8FileOffset;
            prSwdmxMkvInfo->u4CurVidPts =
                prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[((i > 0) ? (i - 1) : i)].ui4_pts;
        }

        prSwdmxMkvInfo->u4CurRepeatFrameIdx = prSwdmxMkvInfo->u4CurSentKey;
        prSwdmxMkvInfo->u4CurRptPTS = prSwdmxMkvInfo->u4CurVidPts;

        LOG(5, "Mkv Seek key idx = %d Cluster offset = %lld\n",
            prSwdmxMkvInfo->u4CurSentKey,
            prSwdmxMkvInfo->u8CurDmxMoviPos);

        LOG(5, "Mkv Seek key pts = 0x%08x, diff target %ds\n",
            prSwdmxMkvInfo->u4CurVidPts,
            (u4SeekTime - prSwdmxMkvInfo->u4CurVidPts) / 90000);

        prSwdmxMkvInfo->u8CurMoviPos = prSwdmxMkvInfo->u8CurDmxMoviPos;

        return TRUE;
    }
    else
    {
        prSwdmxMkvInfo->u8CurDmxMoviPos = 0;
        prSwdmxMkvInfo->u8CurMoviPos = 0;
        return FALSE;
    }
}


static SWDMX_HDR_PRS_STATE_T _MKVCuePointParsing
    (UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed)
{
    MM_RANGE_ELMT_IDXTBL_T* prKeyTbl;
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4CurIdx;
    UINT32 u4ConsumedSize = 0, u4ConsumedSzAcc = 0, u4AvalSz = 0;
    UINT64 u8KeyClusterOffst = 0;
    UINT8* pucBuf = NULL;
    UINT16 u2CurQNum, u2MaxQNum;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, eSWDMX_HDR_PRS_EMPTY_RANGE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    prKeyTbl = prSwdmxMkvInfo->prVidKeyIdxTbl;
    u4CurIdx = prSwdmxMkvInfo->u4CurSentKey;
    #if 0
    UINT32 u4ReqSz = 0;
    #endif

    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    if (prKeyTbl == NULL)
    {
        LOG(3, "No key table\n");
        x_thread_delay(10);
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }
    //=== Key table parsing
    if (u4CurIdx < prSwdmxMkvInfo->u4NumOfKeyEntry)
    {
        // check Q size, flow control
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        if (u2CurQNum > MAX_KEY_FRM)
        {
            x_thread_delay(10);
            prSwdmxInst->fgFifoFull = TRUE;
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        // calculate display period (repeat frame)
        if (!_bCalculateRepeatFrame(u1SrcId, i4PlaySpeed))
        {
            if (i4PlaySpeed > FF_2X && prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_IFRAME)
            {
                prSwdmxMkvInfo->u4CurSentKey++;
            }
            else if (IsBackward(i4PlaySpeed))
            {
                if (prSwdmxMkvInfo->u4CurSentKey > 0)
                {
                    prSwdmxMkvInfo->u4CurSentKey--;
                }
                else if (prSwdmxMkvInfo->u4CurSentKey == 0)
                {
                    prSwdmxMkvInfo->u8CurDmxMoviPos = prSwdmxMkvInfo->u8RangeSz;
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                    prSwdmxMkvInfo->fgIsRewindToStart = TRUE;
                }
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        //find key position
        while(u4CurIdx >= prKeyTbl->ui4_number_of_entry)
        {
            u4CurIdx -= prKeyTbl->ui4_number_of_entry;
            if (prKeyTbl->pt_next_partial)
            {
                prKeyTbl = prKeyTbl->pt_next_partial;
            }
            else
            {
                LOG(3, "keyTbl missing\n");
                break;
            }
        }

        if (prKeyTbl->u.pt_pb_idx_tbl_entry == NULL)
        {
            LOG(3, "prKeyTbl->u.pt_pb_idx_tbl_entry should not be NULL!!!\n");
            ASSERT(prKeyTbl->u.pt_pb_idx_tbl_entry);
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        u8KeyClusterOffst = (prKeyTbl->ui8_base_offset +
        (UINT64)prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size) -
        prSwdmxMkvInfo->u8FileOffset;

        LOG(3, "Idx %d - Blk Ofst, Cluster Ofst %lld, Pts 0x%x\n", u4CurIdx,
        /*prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_relative_offset,*/
        prKeyTbl->ui8_base_offset +
        (UINT64)prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size,
        prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts);

        prSwdmxMkvInfo->u8CurDmxMoviPos = u8KeyClusterOffst;
        prSwdmxMkvInfo->u8CurMoviPos = u8KeyClusterOffst;

        //=== Prepare data
        prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
        prSwdmxMkvInfo->u4LastAlignOffset = 0;
        prSwdmxMkvInfo->fgIsSeek = TRUE;
        prSwdmxMkvInfo->u4LastCbVidPts = 0;
        prSwdmxMkvInfo->u8SeekPts = prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts;
        prSwdmxMkvInfo->u4KeyDeliveredPTS = (UINT32)prSwdmxMkvInfo->u8SeekPts;
        prSwdmxMkvInfo->fgKeyDelivered = FALSE;
        if (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
        {
            prSwdmxMkvInfo->fgWaitIFrm = TRUE;
        }
        prSwdmxMkvInfo->u4KeyPTS = prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts;
        if ((prSwdmxMkvInfo->u1DispStruct == DISP_INTERLACE) &&
            (prSwdmxMkvInfo->u4VidFps >= 50))
        {
            prSwdmxMkvInfo->u4KeyPTS += ((90000/prSwdmxMkvInfo->u4VidFps) + 100);
        }

        //=== Deliver data
        do
        {
            if ((u4AvalSz - u4ConsumedSzAcc) < (MKV_BUFFER_SIZE - MIN_READ_SIZE))
            {
                //=== Request data
                if (prSwdmxMkvInfo->i4ReqReadTimes == 0)
                {
                    if (!_bReqDataFromFeeder(u1SrcId,
                        prSwdmxMkvInfo->u8CurMoviPos + prSwdmxMkvInfo->u8FileOffset,
                        prSwdmxMkvInfo->u8RangeSz - prSwdmxMkvInfo->u8CurMoviPos))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                }
                //=== Wait feeder ack
                if (prSwdmxMkvInfo->i4ReqReadTimes > 0)
                {
                    if (!_bGetDataFromFeederAck(u1SrcId, &rFeederReqData))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                    else
                    {
                        LOG(6, "CueAck ptr 0x%x--0x%x, ofst 0x%x\n",
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize,
                            prSwdmxMkvInfo->u4LastAlignOffset);
                        u4AvalSz += (rFeederReqData.u4ReadSize - prSwdmxMkvInfo->u4LastAlignOffset);
                    }
                }
            }
            if (pucBuf == NULL)
            {
                pucBuf = (UCHAR*)(VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset));
                prSwdmxMkvInfo->u4CurDmxFeederRPtr = (UINT32)pucBuf;
                //error handling
                if ((pucBuf == 0) ||
                    (((UINT32)pucBuf > VIRTUAL(prFeederMkvInfo->u4EndAddr)) ||
                    ((UINT32)pucBuf < VIRTUAL(prFeederMkvInfo->u4StartAddr))))
                {
                    LOG(3, "CuePt DataReq Err!\n");
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
            }

            //set cluster end addr
            _vMKVClusterParsing(u1SrcId,
                (UCHAR*)prSwdmxMkvInfo->u4CurDmxFeederRPtr,
                prRangeList,
                u4AvalSz - u4ConsumedSzAcc,
                &u4ConsumedSize);
            LOG(6, "CuePrs 0x%x--0x%x\n", prSwdmxMkvInfo->u4CurDmxFeederRPtr,
                prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize);
            u4ConsumedSzAcc += u4ConsumedSize;
            prSwdmxMkvInfo->u8CurDmxMoviPos += u4ConsumedSize;
            if (u4ConsumedSzAcc > u4AvalSz)
            {
                _SWDMXResetFeeder(u1SrcId);
                prSwdmxMkvInfo->u8CurMoviPos += u4ConsumedSzAcc - u4AvalSz;
                u4ConsumedSzAcc = u4AvalSz;
                pucBuf = NULL;
            }
            else
            {
                if ((prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >=
                    prFeederMkvInfo->u4EndAddr)
                {
                    prSwdmxMkvInfo->u4CurDmxFeederRPtr =  prFeederMkvInfo->u4StartAddr +
                        ((prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -prFeederMkvInfo->u4EndAddr);
                }
                else
                {
                    prSwdmxMkvInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
                }

                FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, prSwdmxMkvInfo->u4CurDmxFeederRPtr);
            }
        }while((!prSwdmxMkvInfo->fgKeyDelivered) &&
        (prSwdmxMkvInfo->u8CurDmxMoviPos < prSwdmxMkvInfo->u8RangeSz));

        if (prSwdmxMkvInfo->fgKeyDelivered &&
            (prSwdmxMkvInfo->u4KeyDeliveredPTS !=
            prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts))
        {
            prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_pts = prSwdmxMkvInfo->u4KeyDeliveredPTS;
            LOG(5, "Upd Key(Idx,PTS)=(%d, 0x%x)\n", u4CurIdx, prSwdmxMkvInfo->u4KeyDeliveredPTS);
            if (prSwdmxMkvInfo->u8CurClusterPos != u8KeyClusterOffst)
            {
                prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size =
                    (u8KeyClusterOffst + prSwdmxMkvInfo->u8FileOffset) -
                    prKeyTbl->ui8_base_offset;
                LOG(5, "Upd Key(Idx,Ofs)=(%d, 0x%x)\n", u4CurIdx,
                    prKeyTbl->u.pt_pb_idx_tbl_entry[u4CurIdx].ui4_size);
            }
        }
    }
    else
    {
        prSwdmxMkvInfo->u8CurDmxMoviPos = prSwdmxMkvInfo->u8RangeSz;
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
    }



    //=== Reset feeder
    /*if (!_SWDMXResetFeeder(u1SrcId))
    {
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }*/

    return eSWDMX_HDR_PRS_SUCCEED;
}


static BOOL _SWDMXInitDmx(UINT8 u1SrcId, UINT8 u1HdrType, VOID* pfnCbFunc, BOOL fgEos)
{
    BOOL fgRet = TRUE;
    UINT32 u4Flag = 0, u4FifoAddr, u4FifoSize;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    UINT32 u4AudPreFifoAddr,u4AudPreFifoSize,u4AudTrackNum,u4AudTrickSize;
    #endif
    DMX_MM_T rDmxMMInfo;

    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    x_memset((void*)&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!prSwdmxMkvInfo->fgEnVDmx)
        {
            DMX_SetToDecoder(TRUE);
            prSwdmxMkvInfo->u1VidPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
            _SWDMX_GetVideoFifo(u1SrcId, &u4FifoAddr, &u4FifoSize);

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
              DMX_MM_FLAG_INSTANCE_TAG |
              DMX_MM_FLAG_DEVICE_ID);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4FifoAddr;
            rDmxMMInfo.u4BufSize = u4FifoSize;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            if (prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u4VDecType ==
                (UINT32)ENUM_SWDMX_VDEC_MJPEG)
            {
                rDmxMMInfo.u1DeviceId = VLD1; // video format all use VLD expect jpeg
            }
            else
            {
                rDmxMMInfo.u1DeviceId = VLD0; // video format all use VLD expect jpeg
            }

            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxMkvInfo->u4VDecType)
            {
            case ENUM_SWDMX_VDEC_DX311:
            case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8:
            case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_VP8:
            case ENUM_SWDMX_VDEC_MJPEG:
                LOG(7, "%s SearchStartCode = FALSE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            default:
                LOG(7, "%s SearchStartCode = TRUE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            }
            fgRet = DMX_MM_Set(prSwdmxMkvInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxMkvInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!prSwdmxMkvInfo->fgEnADmx)
        {
            DMX_SetToDecoder(TRUE);
            prSwdmxMkvInfo->u1AudPid = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF |
              DMX_MM_FLAG_INSTANCE_TAG |
              DMX_MM_FLAG_DEVICE_ID);

            _SWDMX_GetAudioFifo(u1SrcId, &u4FifoAddr, &u4FifoSize);
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = u4FifoAddr;
            rDmxMMInfo.u4BufSize = u4FifoSize;
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1AdecId;
            rDmxMMInfo.u1DeviceId = prSwdmxInst->u1AdecId;
            fgRet = DMX_MM_Set(prSwdmxMkvInfo->u1AudPid, u4Flag, &rDmxMMInfo);

            #ifdef AUD_PREBUF_EN
            u4AudTrackNum = 0;
            for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
            {
                if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
                {
                    u4AudTrackNum++;
                }
            }
            if (_SWDMX_GetAudioPreFifo(prSwdmxInst->u1SwdmxId, &u4AudPreFifoAddr, &u4AudPreFifoSize))
            {
                u4AudTrickSize = u4AudPreFifoSize/u4AudTrackNum;
                if (u4AudTrickSize > u4FifoSize)
                {
                    u4AudTrickSize = u4FifoSize;
                }
            }
            else
            {
                LOG(1, "%s:%d, _SWDMX_GetAudioPreFifo fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            //audio prebuffer setting
            if (prSwdmxMkvInfo->prDmxRangeInfo)   //execute after range added
            {
                if (!DMX_AudHandler_Set(prSwdmxMkvInfo->u1AudPid, TRUE))
                {
                    LOG(1, "MKV DMX_AudHandler_Set fail\n");
                }
                else
                {
                    for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                    {
                        rDmxMMInfo.fgAllocBuf = FALSE;
                        rDmxMMInfo.u4BufSize = u4AudTrickSize;
                        rDmxMMInfo.u4BufAddr = u4AudPreFifoAddr;
                        if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
                        {

                            prSwdmxMkvInfo->auAudPidx[u4Idx] = DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);
                            if (DMX_MM_Set(prSwdmxMkvInfo->auAudPidx[u4Idx], u4Flag, &rDmxMMInfo))
                            {
                                if (!DMX_AudHandler_Set(prSwdmxMkvInfo->auAudPidx[u4Idx], FALSE))
                                {
                                    LOG(1, "MKV DMX_AudHandler_Set fail\n");
                                    break;
                                }
                                prSwdmxMkvInfo->fgAudPreBuf[u4Idx] = TRUE;
                            }
                            else
                            {
                                LOG(1, "MKV DMX_MM_Set pre-buffer fail\n");
                                break;
                            }

                            u4AudPreFifoAddr += u4AudTrickSize;
                        }
                    }
                    if (u4Idx == MKV_MAX_AUD_TRACK)  // all track set successfully
                    {
                        fgRet = DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
                        prSwdmxMkvInfo->fgActivePidxIsUnKnown = TRUE;
                        DMX_AudHandler_SetEnable(TRUE);
                        prSwdmxMkvInfo->u1FF2xTo1xMode = AUD_PRE_BUF;
                    }
                }
            }
            #endif
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxMkvInfo->u1AudPid, u4Flag, &rDmxMMInfo);

            #ifdef AUD_PREBUF_EN
            //audio prebuffer setting
            if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                {
                    if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
                    {
                        fgRet = DMX_MM_Set(prSwdmxMkvInfo->auAudPidx[u4Idx], u4Flag, &rDmxMMInfo);
                    }
                }
            }
            #endif
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SUBTITLE)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_PES;//DMX_PID_TYPE_PES_TIME;
        prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u1SubPid =
            DMX_MUL_GetSubtitlePidx(prSwdmxInst->u1DmxId);

        if (pfnCbFunc)
        {
            u4Flag = (DMX_MM_FLAG_TYPE |
                DMX_MM_FLAG_SEARCH_START_CODE |
                DMX_MM_FLAG_BUF |
              DMX_MM_FLAG_VALID);

            u4Flag |= DMX_MM_FLAG_NOTIFY;
            rDmxMMInfo.pfnCallback = (PFN_DMX_NOTIFY)pfnCbFunc;

            rDmxMMInfo.fgSearchStartCode = FALSE;
            rDmxMMInfo.fgAllocBuf = TRUE;
            rDmxMMInfo.u4BufSize = MKV_SUBTITLE_BUFSIZE;

            fgRet = DMX_MM_Set(prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u1SubPid, u4Flag, &rDmxMMInfo);
        }
    }
    return fgRet;
}


static BOOL _SWDMXResetFeeder(UINT8 u1SrcId)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prFeederMkvInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(6, "RstFdr Cur Fed's WPtr = 0x%08x\n", prFeederMkvInfo->u4WriteAddr);
    prSwdmxMkvInfo->u4CurDmxFeederRPtr = prFeederMkvInfo->u4WriteAddr;

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, prSwdmxMkvInfo->u4CurDmxFeederRPtr);

    _SWDMX_FlushReqMsgQ(u1SrcId);
    prSwdmxMkvInfo->i4ReqReadTimes = 0;  //invalid flush msg

    return TRUE;
}


static BOOL _SWDMX_SendMkvEos(UINT8 u1SrcId, UINT8 u1HdrType)
{
    UINT16 u2MaxQNum, u2CurQNum;
    UINT32 u4DummyData[4];
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    #endif
    BOOL fgRet = FALSE;
    DMX_MM_DATA_T rDmxMMData;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        if (u2CurQNum > SEND_EOS_THRD)
        {
            x_thread_delay(10);
            prSwdmxInst->fgFifoFull = TRUE;
            return FALSE;
        }
    }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        if (_SWDMXInitDmx(u1SrcId, u1HdrType, NULL, TRUE) == FALSE)
        {
            return FALSE;
        }
    }

    rDmxMMData.u4BufStart = (UINT32)(&(u4DummyData[0]))&0xFFFFFFF0;
    rDmxMMData.u4BufEnd = ((UINT32)(&(u4DummyData[2])) + 0x10+188)&0xFFFFFFF0;
    rDmxMMData.u4StartAddr = (UINT32)&(u4DummyData[1]);
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;
    if (((prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV7) ||
    (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV8)) ||
    (prSwdmxMkvInfo->u4VDecType == ENUM_SWDMX_VDEC_WMV9))
    {
        rDmxMMData.fgFrameHead = TRUE;
    }
    if (u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMData.u1Idx = prSwdmxMkvInfo->u1VidPid;
    }
    else if (u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO)
    {
        rDmxMMData.u1Idx = prSwdmxMkvInfo->u1AudPid;
    }
    else
    {
        rDmxMMData.u1Idx = u1HdrType;
    }

    #ifdef AUD_PREBUF_EN
    //audio prebuffer setting
    if ((u1HdrType == eSWDMX_MEDIA_AUDIO) &&
        (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF))
    {
        for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
        {
            if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
            {
                rDmxMMData.u1Idx = prSwdmxMkvInfo->auAudPidx[u4Idx];
                fgRet |= _SWDMX_MoveData(u1SrcId, &rDmxMMData);
            }
        }
    }
    else
    {
        fgRet = _SWDMX_MoveData(u1SrcId, &rDmxMMData);
    }
    #else
    fgRet = _SWDMX_MoveData(u1SrcId, &rDmxMMData);
    #endif

    return fgRet;
}


//DivX Plus Trick play track feature
static BOOL _SWDMXMkvSwitchTrack(BOOL fgIsTPT, SWDMX_MKV_INFO_T *prSwdmxMkvInfo)
{
    if (fgIsTPT)
    {
        #ifdef DIVX_PLUS_CER
        if (prSwdmxMkvInfo->prTPTVidKeyIdxTbl &&
            (prSwdmxMkvInfo->u4TPTNumOfKeyEntry >= MKV_KEY_TABLE_THRESHOLD))
        {
            prSwdmxMkvInfo->u8FileOffset = prSwdmxMkvInfo->u8TPTFileOffset;
            prSwdmxMkvInfo->u8RangeSz = prSwdmxMkvInfo->u8TPTRangeSz;
            prSwdmxMkvInfo->prVidKeyIdxTbl = prSwdmxMkvInfo->prTPTVidKeyIdxTbl;
            prSwdmxMkvInfo->u4NumOfKeyEntry = prSwdmxMkvInfo->u4TPTNumOfKeyEntry;
            prSwdmxMkvInfo->u4KeyPTS = 0xFFFFFFFF;
            return TRUE;
        }
        #endif
    }
    else
    {
        prSwdmxMkvInfo->u8FileOffset = prSwdmxMkvInfo->u8MainFileOffset;
        prSwdmxMkvInfo->u8RangeSz = prSwdmxMkvInfo->u8MainRangeSz;
        prSwdmxMkvInfo->prVidKeyIdxTbl = prSwdmxMkvInfo->prMainVidKeyIdxTbl;
        prSwdmxMkvInfo->u4NumOfKeyEntry = prSwdmxMkvInfo->u4MainNumOfKeyEntry;
    }
    return FALSE;
}


static UINT32 _vMKVGetNaluSize(UCHAR* pucFifo, UINT8 u1AvcNaluLen)
{
    UINT32 u4NaluLen4 = 0, u4Len = 0;
    UINT16 u2NaluLen2 = 0;
    UINT8 u1NaluLen1 = 0;

    if (u1AvcNaluLen == 4)
    {
        LOADB_1DWRD(pucFifo, u4NaluLen4);
        u4Len = u4NaluLen4;
    }
    else if (u1AvcNaluLen == 3)
    {
        LOADB_1DWRD(pucFifo, u4NaluLen4);
        u4NaluLen4 >>= 8;
        u4Len = u4NaluLen4;
    }
    else if (u1AvcNaluLen == 2)
    {
        LOADB_1WORD(pucFifo, u2NaluLen2);
        u4Len = u2NaluLen2;
    }
    else if (u1AvcNaluLen == 1)
    {
        LOAD_1BYTE(pucFifo, u1NaluLen1);
        u4Len = u1NaluLen1;
    }
    else
    {
        LOG(3, "AVCNaluLen %d error\n", u1AvcNaluLen);
        u4Len = 0;
    }

    return u4Len;
}


static BOOL _vMKVContentDecode(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo,
    UINT32 u4DataLen, BOOL bInsertSC, BOOL bIsFdrBuf)
{
#ifdef MKV_CONTENT_ENCODING_SUPPORT
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
     UINT32 u4FifoAvailSize = 0;
    UINT8 u1PidType = 0, u1PidIdx = 0;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO || u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        SWDMX_MKV_CONTENT_ENCODINGS_T *ptContentEncodings =
            (u1HdrType == eSWDMX_MEDIA_VIDEO) ?
            &prSwdmxMkvInfo->tVidContentEncodings :
            &prSwdmxMkvInfo->tAudContentEncodings[prSwdmxMkvInfo->u4CurAStrmID];

        if (ptContentEncodings->fgValid)
        {
            SWDMX_MKV_CONTENT_ENCODING_T *ptContentEncoding =
                ptContentEncodings->ptContentEncoding;
            SWDMX_MKV_CONTENT_COMPRESSION_T *ptCompression =
                &ptContentEncoding->tCompression;
            if(ptContentEncoding)
            {
                if(u1HdrType == eSWDMX_MEDIA_SUBTITLE)
                { //check if A/V fifo full
                    u1PidIdx = prSwdmxMkvInfo->u1SubPid;
                
                    u1PidType = DMX_PID_TYPE_PES_TIME;
                    
                    u4FifoAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)u1PidType, u1PidIdx);
                }
                else 
                {
                    u1PidIdx = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (prSwdmxMkvInfo->u1VidPid):(prSwdmxMkvInfo->u1AudPid);
                
                    u1PidType = (u1HdrType == eSWDMX_MEDIA_VIDEO) ? (DMX_PID_TYPE_ES_VIDEO):(DMX_PID_TYPE_ES_AUDIO);
                
                    u4FifoAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, (DMX_PID_TYPE_T)u1PidType, u1PidIdx);
                }
                if (u4FifoAvailSize < (u4DataLen + ptCompression->u4CompSettingsLen + OGG_HDR_LEN ))
                {
                    LOG(2, "Type %d Fifo Full 1\n", u1HdrType);
                    if (((u1HdrType == (UINT8)eSWDMX_MEDIA_AUDIO) &&
                        prSwdmxMkvInfo->fgIsAudWaitAout) &&
                        (!MPV_IsDisplay(prSwdmxInst->u1VdecId)))
                    {
                        AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                        prSwdmxMkvInfo->fgIsAudWaitAout = FALSE;
                    }
                    //timeout, vfifo full and audio data is not parsed yet
                    if ((u1HdrType == (UINT8)eSWDMX_MEDIA_VIDEO) &&
                        prSwdmxMkvInfo->fgAudTrigVidDec)
                    {
                        LOG(3, "vfifo full and audio data is not parsed yet, force play video\n");
                        _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
                    }
                    x_thread_delay(10);
                    prSwdmxInst->fgFifoFull = TRUE;
                    return FALSE;
                }
            }
            while (ptContentEncoding)
            {
                if (ptContentEncoding->u4Type == MKV_CONTENT_COMPRESSION)
                {
                    if (ptCompression->u4CompAlgo == MKV_CONTENT_ALGO_HEADER_STRIPPING)
                    {
                        prSwdmxMkvInfo->fgHDRDELIVER = TRUE;
                        if ((MKV_ELMT_ID_STATE)prSwdmxMkvInfo->u4PrsState == MKVPRS_G_DATA_AVC)
                        {
                            // donothing
                            ASSERT(*ptCompression->pucCompSettings == 0);
                        }
                        else if (!_vMKVDeliverData(u1SrcId, u1HdrType, ptCompression->pucCompSettings, ptCompression->u4CompSettingsLen, FALSE, FALSE))
                        {
                            prSwdmxMkvInfo->fgHDRDELIVER = FALSE;
                            return FALSE;
                        }
                        prSwdmxMkvInfo->fgHDRDELIVER = FALSE;
                    }
                    else
#if 0 // not support yet
                    if (ptCompression->u4CompAlgo == MKV_CONTENT_ALGO_ZLIB)
                    {
                        LOG(3, "We don't support ZLIB now!!!\n");
                        ASSERT(0);
                    }
                    else
#endif
                    {
                        LOG(3, "unknown compression algorithm: %d\n", ptCompression->u4CompAlgo);
                        ASSERT(0);
                    }
                }
                ptContentEncoding = ptContentEncoding->ptNext;
            }
        }
    }
#endif


    return _vMKVDeliverData(u1SrcId, u1HdrType, pucFifo, u4DataLen, bInsertSC, bIsFdrBuf);
}

#ifdef MKV_CONTENT_ENCODING_SUPPORT
static BOOL _SWDMXMkvAddContentEncoding(
    UINT8 u1SrcId,
    MINFO_MKV_CONTENT_ENCODING_T *ptCntEncoding,
    SWDMX_MKV_CONTENT_ENCODINGS_T *ptContentEncodings)
{
    SWDMX_MKV_CONTENT_ENCODING_T *ptContentEncoding;
    SWDMX_MKV_CONTENT_COMPRESSION_T *ptCompression;
    UINT32 u4Addr, u4Size, u4Align;

    if (ptCntEncoding->ui8_type!= MKV_CONTENT_COMPRESSION)
    {
        LOG(3, "Unsupported type %d\n", ptCntEncoding->ui8_type);
        return FALSE;
    }

    if (ptCntEncoding->ui8_scope != MKV_CONTENT_SCOPE_ALL)
    {
        LOG(3, "Unsupported scope %d\n", ptCntEncoding->ui8_scope);
        return FALSE;
    }

    if (ptCntEncoding->ui8_compress_algo != MKV_CONTENT_ALGO_HEADER_STRIPPING)
    {
        LOG(3, "Unsupported algo %d\n", ptCntEncoding->ui8_compress_algo);
        return FALSE;
    }

    if (ptCntEncoding->ui8_comp_settings_length > 1024)
    {
        LOG(3, "Comp Len (%d) is too long\n", ptCntEncoding->ui8_comp_settings_length);
        return FALSE;
    }

    u4Addr = 0;
    u4Size = sizeof(SWDMX_MKV_CONTENT_ENCODING_T);
    u4Align = 16;
    if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
        || u4Addr == 0)
    {
        LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return FALSE;
    }

    // copy Content Encoding data from MW to driver
    ptContentEncoding = (SWDMX_MKV_CONTENT_ENCODING_T *)u4Addr;
    ptCompression = &ptContentEncoding->tCompression;

    ptCompression->u4CompSettingsLen = ptCntEncoding->ui8_comp_settings_length;
    ptCompression->u4CompAlgo = ptCntEncoding->ui8_compress_algo;

    u4Addr = 0;
    u4Size = ptCompression->u4CompSettingsLen;
    u4Align = 16;
    if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
        || u4Addr == 0)
    {
        LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return FALSE;
    }

    x_memcpy((VOID *)u4Addr,
        (VOID *)ptCntEncoding->pui1_comp_settings_data,
        ptCompression->u4CompSettingsLen);
    ptCompression->pucCompSettings = (UCHAR *)u4Addr;

    ptContentEncoding->u4Order = ptCntEncoding->ui8_order;
    ptContentEncoding->u4Scope = ptCntEncoding->ui8_scope;
    ptContentEncoding->u4Type = ptCntEncoding->ui8_type;
    ptContentEncoding->ptNext = NULL;

    if (ptContentEncodings->ptContentEncoding == NULL)
    {
        ptContentEncodings->ptContentEncoding = ptContentEncoding;
    }
    else
    {
        SWDMX_MKV_CONTENT_ENCODING_T *ptContentEncodingCur =
            ptContentEncodings->ptContentEncoding;

        if (ptContentEncoding->u4Order > ptContentEncodingCur->u4Order)
        {
            ptContentEncodings->ptContentEncoding = ptContentEncoding;
            ptContentEncoding->ptNext = ptContentEncodingCur;
        }
        else
        {
            SWDMX_MKV_CONTENT_ENCODING_T *ptContentEncodingPre;
            do {
                ptContentEncodingPre = ptContentEncodingCur;
                ptContentEncodingCur = ptContentEncodingPre->ptNext;
                if (ptContentEncodingCur == NULL)
                {
                    ptContentEncodingPre->ptNext = ptContentEncoding;
                    break;
                }
                else if (ptContentEncoding->u4Order > ptContentEncodingCur->u4Order)
                {
                    ptContentEncodingPre->ptNext = ptContentEncoding;
                    ptContentEncoding->ptNext = ptContentEncodingCur;
                    break;
                }
            } while (ptContentEncodingCur);
        }
    }

    return TRUE;
}
#endif


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_MKVInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, TRUE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(prSwdmxInst->eFeederSrc);
    //*********************************************************
    //FeederSetBufferSize(prSwdmxInst->eFeederSrc, 5*10*1024);
    //FeederStart(prSwdmxInst->eFeederSrc);
    x_memset((void*)prSwdmxMkvInfo, 0, sizeof(SWDMX_MKV_INFO_T));
    x_memset((void*)prFeederMkvInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    #if 0
    FeederInstallCallback(prSwdmxInst->eFeederSrc, FEEDER_NOTIFY_DATA_REQ_DONE,
    _SWDMX_FeederCallBack, (UINT32)prFeederMkvInfo);
    //*********************************************************

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _MMVideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;

    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);
    #endif

    if (!_vMkvInfoInit(u1SrcId))
    {
        return FALSE;
    }
    prSwdmxMkvInfo->u1FF2xTo1xMode = KEEP_VFIFO;

    return TRUE;

}


void _SWDMX_MKVSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList,
    UINT32 u4VidStrmID,
    UINT32 u4AudStrmID,
    UINT32 u4SubTitleID)
{
    MM_RANGE_ELMT_IDXTBL_T *prKeyTbl;
    MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl;
    UINT32 u4StrmID;
    UINT32 u4TotalTime;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR_VOID(prSwdmxInst);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (prRangeList != NULL &&
         prRangeList->prDmxRangeInfo != NULL)
    {
        pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;
        prSwdmxMkvInfo->prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        u4TotalTime =
            (UINT32)u8Div6432((UINT64)((UINT64)(prSwdmxMkvInfo->prDmxRangeInfo->ui8_timecode_scale)*
            (UINT64)(prSwdmxMkvInfo->prDmxRangeInfo->ui8_pb_duration)), 1000000, NULL);
        VDEC_SetTotalTime(prSwdmxInst->u1VdecId,u4TotalTime );

        while (pBuldIdxTbl != NULL)
        {
            u4StrmID = pBuldIdxTbl->t_strm_id.u.t_stm_mkv_id;
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                prSwdmxMkvInfo->prMainVidKeyIdxTbl = pBuldIdxTbl;
                if (prSwdmxMkvInfo->prMainVidKeyIdxTbl->ui4_scale > 0)
                {
                    prSwdmxMkvInfo->u4VidFps =
                        (UINT32)(prSwdmxMkvInfo->prMainVidKeyIdxTbl->ui4_rate/
                       prSwdmxMkvInfo->prMainVidKeyIdxTbl->ui4_scale);
                }
                prKeyTbl = pBuldIdxTbl;
               prSwdmxMkvInfo->u4MainNumOfKeyEntry = prKeyTbl->ui4_number_of_entry;
                while(prKeyTbl->pt_next_partial)
                {
                    prKeyTbl = prKeyTbl->pt_next_partial;
                   prSwdmxMkvInfo->u4MainNumOfKeyEntry += prKeyTbl->ui4_number_of_entry;
                }
            }
            else if ((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO) &&
                (u4StrmID < MKV_MAX_AUD_TRACK))
            {
                prSwdmxMkvInfo->fgIsAudTrack[u4StrmID] = TRUE;
                if (pBuldIdxTbl->t_aud_spec_info.e_fmt < 0xFF)
                {
                    prSwdmxMkvInfo->u4ADecType[u4StrmID] =
                        _ADecTypeCast(pBuldIdxTbl->t_aud_spec_info.e_fmt);
                }
                if (prSwdmxMkvInfo->u4ADecType[u4StrmID] == ENUM_SWDMX_ADEC_AAC)
                {
                    prSwdmxMkvInfo->u4AudSampleRate[u4StrmID] =
                        _AACSampFreqIdx(pBuldIdxTbl->t_aud_spec_info.u.t_aac_info.ui4_sample_rate);
                    prSwdmxMkvInfo->u4AudChannelNs[u4StrmID] =
                        pBuldIdxTbl->t_aud_spec_info.u.t_aac_info.ui4_num_ch;
                }
            }
            else if ((pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_SP) &&
                (u4StrmID < MKV_MAX_SB_TRACK))
            {
                prSwdmxMkvInfo->fgIsSBTrack[u4StrmID] = TRUE;
            }
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            //set segment time scale
            prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale = (UINT64)prRangeList->prDmxRangeInfo->ui8_timecode_scale * 9;

            LOG(3, "SetSegScale %d\n", (UINT32)prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale);

        }

        prSwdmxMkvInfo->u8MainFileOffset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxMkvInfo->u8MainRangeSz = prRangeList->prDmxRangeInfo->z_range_sz;
        prSwdmxMkvInfo->fgIsTPT = _SWDMXMkvSwitchTrack(FALSE, prSwdmxMkvInfo);
        if (prSwdmxMkvInfo->fgIsTPT)
        {
            VDEC_SetTPTFlag(u1SrcId,prSwdmxMkvInfo->fgIsTPT);
        }

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        //prRangeList->u4CurPTS =
        //prRangeList->prDmxRangeInfo->t_vid_start_render_pts;

        //for trick play track
        #ifdef DIVX_PLUS_CER
        if (prRangeList->prDmxRangeInfo->pt_next_scale)
        {
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_next_scale->pt_idxtbl_lst;
            while (pBuldIdxTbl != NULL)
            {
                if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
                {
                    prSwdmxMkvInfo->prTPTVidKeyIdxTbl = pBuldIdxTbl;
                    prKeyTbl = pBuldIdxTbl;
                    prSwdmxMkvInfo->u4TPTNumOfKeyEntry = prKeyTbl->ui4_number_of_entry;
                    while(prKeyTbl->pt_next_partial)
                    {
                        prKeyTbl = prKeyTbl->pt_next_partial;
                        prSwdmxMkvInfo->u4TPTNumOfKeyEntry += prKeyTbl->ui4_number_of_entry;
                    }
                    break;
                }
                pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            }
            prSwdmxMkvInfo->u8TPTFileOffset =
                prRangeList->prDmxRangeInfo->pt_next_scale->ui8_fileoffset;
            prSwdmxMkvInfo->u8TPTRangeSz =
                prRangeList->prDmxRangeInfo->pt_next_scale->z_range_sz;
        }
        #endif

        // for content encoding
        if (prRangeList->prDmxRangeInfo->pt_vid_decoder_info)
        {
            MM_RANGE_DECODER_T *ptDecoderInfo =
                prRangeList->prDmxRangeInfo->pt_vid_decoder_info;

            while (ptDecoderInfo)
            {
                if (ptDecoderInfo->ui4_strm_id != u4VidStrmID)
                {
                    LOG(3, "Unexpected Vid Stream ID: %d\n", ptDecoderInfo->ui4_strm_id);
                    ptDecoderInfo = ptDecoderInfo->pt_next;
                    continue;
                }

#ifdef MKV_CONTENT_ENCODING_SUPPORT
                if (ptDecoderInfo->b_has_encoding)
                {
                    UINT32 u4Idx, u4Total;
                    MINFO_MKV_CONTENT_ENCODING_T *ptCntEncoding;
                    SWDMX_MKV_CONTENT_ENCODINGS_T *ptContentEncodings =
                        &prSwdmxMkvInfo->tVidContentEncodings;

                    u4Total = ptDecoderInfo->ui4_encoding_num;
                    ptCntEncoding = (MINFO_MKV_CONTENT_ENCODING_T *)ptDecoderInfo->pv_encoding_string;
                    for (u4Idx = 0; ptCntEncoding && u4Idx < u4Total; u4Idx++, ptCntEncoding = ptCntEncoding->pt_next)
                    {
                        if (!_SWDMXMkvAddContentEncoding(u1SrcId, ptCntEncoding, ptContentEncodings))
                        {
                            break;
                        }
                    }

                    if (ptCntEncoding && u4Idx < u4Total)
                    {
                        LOG(3, "Add vid content encoding NG\n");
                        break;
                    }
                    else
                    {
                        ptContentEncodings->fgValid = TRUE;
                    }
                }
#endif

                if (ptDecoderInfo->pui1_codec_private && ptDecoderInfo->ui8_codec_private_len)
                {
#if 1
                    LOG(0, "Video codec private is not supported yet\n");
                    ASSERT(0);
#else
                    UINT32 u4Addr, u4Size, u4Align;
                    u4Addr = 0;
                    u4Size = ptDecoderInfo->ui8_codec_private_len;
                    u4Align = 16;
                    if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
                        || u4Addr == 0)
                    {
                        LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
                        ASSERT(0);
                        return;
                    }

                    x_memcpy((VOID *)u4Addr, ptDecoderInfo->pui1_codec_private, ptDecoderInfo->ui8_codec_private_len);
                    prSwdmxMkvInfo->u4VidHeaderPtr = u4Addr;
                    prSwdmxMkvInfo->u4VidHdrLength = ptDecoderInfo->ui8_codec_private_len;
#endif
                }

                ptDecoderInfo = ptDecoderInfo->pt_next;
            }
        }

        if (prRangeList->prDmxRangeInfo->pt_aud_decoder_info)
        {
            MM_RANGE_DECODER_T *ptDecoderInfo =
                prRangeList->prDmxRangeInfo->pt_aud_decoder_info;

            while (ptDecoderInfo)
            {
                u4StrmID = ptDecoderInfo->ui4_strm_id;
                if (!prSwdmxMkvInfo->fgIsAudTrack[u4StrmID])
                {
                    LOG(3, "Unexpected Aud Stream ID: %d\n", u4StrmID);
                    ptDecoderInfo = ptDecoderInfo->pt_next;
                    continue;
                }

#ifdef MKV_CONTENT_ENCODING_SUPPORT
                if (ptDecoderInfo->b_has_encoding)
                {
                    UINT32 u4Idx, u4Total;
                    MINFO_MKV_CONTENT_ENCODING_T *ptCntEncoding;
                    SWDMX_MKV_CONTENT_ENCODINGS_T *ptContentEncodings =
                        &prSwdmxMkvInfo->tAudContentEncodings[u4StrmID];
                    u4Total = ptDecoderInfo->ui4_encoding_num;
                    ptCntEncoding = (MINFO_MKV_CONTENT_ENCODING_T *)ptDecoderInfo->pv_encoding_string;
                    for (u4Idx = 0; ptCntEncoding && u4Idx < u4Total; u4Idx++, ptCntEncoding = ptCntEncoding->pt_next)
                    {
                        if (!_SWDMXMkvAddContentEncoding(u1SrcId, ptCntEncoding, ptContentEncodings))
                        {
                            break;
                        }
                    }

                    if (ptCntEncoding && u4Idx < u4Total)
                    {
                        LOG(3, "Add vid content encoding NG\n");
                        break;
                    }
                    else
                    {
                        ptContentEncodings->fgValid = TRUE;
                    }
                }
#endif

                if (ptDecoderInfo->pui1_codec_private && ptDecoderInfo->ui8_codec_private_len)
                {
                    if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[u4StrmID])
                    {
                        if (!_GenVorbisTbl(u1SrcId, ptDecoderInfo))
                        {
                            LOG(0, "%s(%d) call _GenVorbisTbl(%d, %p) fail\n", __FUNCTION__, __LINE__, u1SrcId, ptDecoderInfo);
                            ASSERT(0);
                            break;
                        }
                    }
                    else
                    {
                        UINT32 u4Addr, u4Size, u4Align;
                        u4Addr = 0;
                        u4Size = ptDecoderInfo->ui8_codec_private_len;
                        u4Align = 16;
                        if (!SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &u4Addr, &u4Size, &u4Align)
                            || u4Addr == 0)
                        {
                            LOG(0, "%s(%d): eSWDMX_GET_INTERNAL_BUF NG\n", __FUNCTION__, __LINE__);
                            ASSERT(0);
                            break;
                        }

                        x_memcpy((VOID *)u4Addr, ptDecoderInfo->pui1_codec_private, ptDecoderInfo->ui8_codec_private_len);
                        prSwdmxMkvInfo->u4AudHeaderPtr[u4StrmID] = u4Addr;
                        prSwdmxMkvInfo->u4AudHdrLength[u4StrmID] = ptDecoderInfo->ui8_codec_private_len;
                    }
                }

                ptDecoderInfo = ptDecoderInfo->pt_next;
            }
        }
    }
    else
    {
        LOG(0, "Demuxer Range is NULL\n");
    }
}


BOOL _SWDMX_MKVSetDecoderInfo(UINT8 u1SrcId, UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    if (u1Type == eSWDMX_SET_VIDEOTYPE)
    {
        #if defined(CC_MKV_MPEG1_DISABLE) && defined(CC_MKV_MPEG2_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_MPEG4_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_DX311_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_H264_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_VC1_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_MJPEG_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MKV_VP8_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VP8)
        {
            return FALSE;
        }
        else
        #endif
        {
            prSwdmxMkvInfo->u4VDecType =  u4Para1;
            return TRUE;
        }
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
        prSwdmxMkvInfo->u4MainVidHdrLength = u4Para2;
        #ifndef LINUX_TURNKEY_SOLUTION
        prSwdmxMkvInfo->u4MainVidHeaderPtr = u4Para1;
        #else
        if (u4Para1 && u4Para2)
        {
            UINT32 u4Align = 16;
            _SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &prSwdmxMkvInfo->u4MainVidHeaderPtr, &u4Para2, &u4Align);

            if (prSwdmxMkvInfo->u4MainVidHeaderPtr)
            {
                  x_memcpy((VOID*)VIRTUAL(prSwdmxMkvInfo->u4MainVidHeaderPtr),
                 (VOID*)VIRTUAL(u4Para1),
                 prSwdmxMkvInfo->u4MainVidHdrLength);
            }
            else
            {
                LOG(0,"Header Seq Buffer alloc fail\n");
                ASSERT(0);
                return FALSE;
            }
        }
        else
        {
            prSwdmxMkvInfo->u4MainVidHeaderPtr = 0;
        }
        #endif
        //prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale = (u4Para3 * 9) / 100000;
        //LOG(1, "SetSegScale %d\n", prSwdmxMkvInfo->u8MkvSegmentInfoTimeCodeScale);
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_TPT_VIDSEQHEADER)
    {
        prSwdmxMkvInfo->u4TPTVidHdrLength = u4Para2;
        #ifndef LINUX_TURNKEY_SOLUTION
        prSwdmxMkvInfo->u4TPTVidHeaderPtr = u4Para1;
        #else
        if (u4Para1 && u4Para2)
        {
            UINT32 u4Align = 16;
            _SWDMX_GetInfo(u1SrcId, eSWDMX_GET_INTERNAL_BUF, &prSwdmxMkvInfo->u4TPTVidHeaderPtr, &u4Para2, &u4Align);

            if (prSwdmxMkvInfo->u4TPTVidHeaderPtr)
            {
                  x_memcpy((VOID*)VIRTUAL(prSwdmxMkvInfo->u4TPTVidHeaderPtr),
                 (VOID*)VIRTUAL(u4Para1),
                 prSwdmxMkvInfo->u4TPTVidHdrLength);
            }
            else
            {
                LOG(0,"Header Seq Buffer alloc fail\n");
                ASSERT(0);
                return FALSE;
            }
        }
        else
        {
            prSwdmxMkvInfo->u4TPTVidHeaderPtr = 0;
        }
        #endif
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
#if 0 // should be set at _SWDMX_MKVSetRange
        //mkv test
        if (u4Para1 < 0xFF)
        {
            prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID] = u4Para1;
        }
        if (u4Para2 > 0)
        {
            prSwdmxMkvInfo->u4AudSampleRate[prSwdmxMkvInfo->u4AStrmID] =
                _AACSampFreqIdx(u4Para2);
        }
        if (u4Para3 > 0)
        {
            prSwdmxMkvInfo->u4AudChannelNs[prSwdmxMkvInfo->u4AStrmID] = u4Para3;
        }
#endif
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _SWDMX_MKVGetInfo(UINT8 u1SrcId, UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 u8StcValue=0;
    UINT64 *pu8Tmp;
    INT8 i;
    BOOL fgIsZero = TRUE;
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MKVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        u8StcValue=STC_GetStcValue(prSwdmxInst->u1StcId);
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp=0;
        for(i=9;i>=0;i--)
        {
            if (prSwdmxMkvInfo->au8PostionxTime[i][0]!=0 && prSwdmxMkvInfo->au8PostionxTime[i][1]!=0 )
            {
                if (prSwdmxMkvInfo->au8PostionxTime[i][1]<=u8StcValue)
                {
                    *pu8Tmp= prSwdmxMkvInfo->au8PostionxTime[i][0];
                    break;
                }
            }
        }

        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if (prSwdmxMkvInfo->prVidKeyIdxTbl)
        {
            if (prSwdmxMkvInfo->prVidKeyIdxTbl->ui4_number_of_entry < MKV_KEY_TABLE_THRESHOLD)
            {
                *pu4Para1 = FALSE;
            }
            else
            {
                *pu4Para1 = TRUE;
                for(i = 0; i < prSwdmxMkvInfo->prVidKeyIdxTbl->ui4_number_of_entry;i++)
                {
                    if (prSwdmxMkvInfo->prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts != 0)
                    {
                        fgIsZero = FALSE;
                        break;
                    }
                }
                if (fgIsZero)
                {
                    *pu4Para1 = FALSE;
                }
            }
        }
        else
        {
            *pu4Para1 = FALSE;
        }
    }
    else if (u1Type == eSWDMX_GET_AUDIO_ACTIVE_PIDX)
    {
        UINT32 u4AudFifoAddr, u4AudFifoSize,u4DmxAvailSize;
        _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &u4AudFifoAddr, &u4AudFifoSize);
        u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId,
                                                                        (DMX_PID_TYPE_T)DMX_PID_TYPE_ES_AUDIO,
                                                                        prSwdmxMkvInfo->u1AudPid);

        *pu4Para1 = u4AudFifoSize - u4DmxAvailSize;;
    }
    return TRUE;
}

BOOL _SWDMX_MKVSetStrmID(UINT8 u1SrcId, UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    UNUSED(u4StrmSubID);

    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxMkvInfo->u4VStrmID = u4StrmID;
            LOG(5, "_SWDMX_MKVSetStrmID Vid = 0x%08x\n", u4StrmID);
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if (u4StrmID < MKV_MAX_AUD_TRACK)
            {
                prSwdmxMkvInfo->u4AStrmID = u4StrmID;
                LOG(5, "_SWDMX_MKVSetStrmID Aud = 0x%08x\n", u4StrmID);
            }
            else
            {
                LOG(5, "_SWDMX_MKVSetStrmID Aud = 0x%08x Fail!\n", u4StrmID);
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxMkvInfo->u4SpStrmID = u4StrmID;
            LOG(5, "_SWDMX_MKVSetStrmID Sub = 0x%08x\n", u4StrmID);
            break;
        default:
            return FALSE;
    }

    return TRUE;

}


SWDMX_HDR_PRS_STATE_T _SWDMX_MKVHdrParse(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    INT32 i4PlaySpeed;
    UINT64 u8SeekPos;
    BOOL fgSpeedChange;
    UINT32 u4ConsumedSize = 0, u4TargetPTS = 0;
    INT32 i4StcNow;
    INT32 i4Sub;
    UINT8 i = 0;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    #endif
    FEEDER_REQ_DATA_T rFeederReqData;
    VDEC_HDR_INFO_T rHdrInfo;
    BOOL fgSeek = FALSE, fgAudTrigVidDec = FALSE;
    UINT8 u1TrickNomalSwitch = 0;

    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, eSWDMX_HDR_PRS_EMPTY_RANGE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (!prCmd)
    {
        LOG(1, "prCmd null\n");
        return eSWDMX_HDR_UNKNOWN_TYPE;
    }
    i4PlaySpeed = prCmd->i4PlaySpeed;
    u8SeekPos = prCmd->u8SeekPos;
    fgSpeedChange = prCmd->fgChange;

    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    prSwdmxMkvInfo->i4Speed = i4PlaySpeed;
    if (prRangeList->prDmxRangeInfo == NULL)
    {
        LOG(5, "This range is empty\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE;
    }

    //check if init after stop
    if (!prSwdmxMkvInfo->fgSeqHdrParsed)
    {
        if (!_vMkvInfoInit(u1SrcId))
        {
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        //===  Request data for the first time, parsing private data
        if ((prSwdmxMkvInfo->u8CurDmxMoviPos == 0)
            && ((prSwdmxMkvInfo->u4MainVidHeaderPtr > 0) && (prSwdmxMkvInfo->i4PrevSpeed == NORMAL_1X)))
        {
            if (prSwdmxMkvInfo->fgEnVideo && !prSwdmxMkvInfo->fgEnVDmx)
            {
                UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_VIDEO, NULL, FALSE));
                prSwdmxMkvInfo->fgEnVDmx = TRUE;
            }
            _vMkvVidHdrParsing(u1SrcId, prSwdmxMkvInfo->u4MainVidHeaderPtr, prSwdmxMkvInfo->u4MainVidHdrLength);
        }
        prSwdmxMkvInfo->fgSeqHdrParsed = TRUE;
    }

    if (prSwdmxMkvInfo->fgBeginToPlay &&
        i4PlaySpeed == NORMAL_1X)
    {
           if (prSwdmxMkvInfo->fgEnAudio && !prSwdmxMkvInfo->fgEnADmx)
       {
            //audio pre-buffer setting, in case "SetRange" is called after EnableStrm
            UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_AUDIO, NULL, FALSE));
            prSwdmxMkvInfo->fgEnADmx = TRUE;
        }
        if (prSwdmxMkvInfo->fgEnVideo && !prSwdmxMkvInfo->fgEnVDmx)
        {
            UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_VIDEO, NULL, FALSE));
            prSwdmxMkvInfo->fgEnVDmx = TRUE;
        }
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, 0);
        u4TargetPTS = 0;
        prSwdmxMkvInfo->fgBeginToPlay = FALSE;
        fgAudTrigVidDec = TRUE;

        //resume stop
        if (prRangeList->prDmxRangeInfo->t_vid_start_render_pts != 0)
        {
            fgSpeedChange = TRUE;
            u8SeekPos = (UINT64)prRangeList->prDmxRangeInfo->t_vid_start_render_pts;
        }
        else if (prRangeList->prDmxRangeInfo->t_aud_start_render_pts!= 0)
            {
               prSwdmxMkvInfo->u8CurDmxMoviPos=(UINT64)prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position;
               prSwdmxMkvInfo->u8CurMoviPos=(UINT64)prRangeList->prDmxRangeInfo->t_start_pos.ui8_frame_position;
               AUD_SetStartPts(prSwdmxInst->u1AdecId, (UINT64)prRangeList->prDmxRangeInfo->t_aud_start_render_pts);
        }

        #ifdef CC_SWDMX_RENDER_POSITION
        if (prRangeList->fgSetRenderPosition != 0)
        {
             _SWDMXResetFeeder(u1SrcId);
             prSwdmxMkvInfo->u8CurDmxMoviPos=(UINT64)prRangeList->u8OffsetI;
             prSwdmxMkvInfo->u8CurMoviPos=(UINT64)prRangeList->u8OffsetI;
             prSwdmxMkvInfo->u4CurVidPts = prRangeList->u8StartPts;
             prSwdmxMkvInfo->u8AudSeekPts = prRangeList->u8StartPts;
             prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;
             prSwdmxMkvInfo->fgWaitIFrm = TRUE;
             STC_SetStcValue(prSwdmxInst->u1StcId, prRangeList->u8StartPts);
        }
        #endif
        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxMkvInfo->u4CurVidPts);
        #ifdef AUD_PREBUF_EN
        if ((prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF) &&
            prSwdmxMkvInfo->fgEnAudio)
        {
            if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID])
            {
                DMX_AudHandler_SetActivePidxEx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID],
                    (UINT8*)prSwdmxMkvInfo->u4AudHeaderPtr[prSwdmxMkvInfo->u4AStrmID],
                    prSwdmxMkvInfo->u4AudHdrLength[prSwdmxMkvInfo->u4AStrmID]);
            }
            else
            {
                DMX_AudHandler_SetActivePidx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID]);
            }
            prSwdmxMkvInfo->fgActivePidxIsUnKnown = FALSE;
        }
        #endif
        #if 0//CC_DLNA_SUPPORT
        FeederSetReadyThreshold(8*1024*1024);
        FeederSetUnderFlowThreshold(1*1024*1024);
        while(!FeederIsReady(prSwdmxInst->eFeederSrc))
        {
            if (!_bReqDataFromFeeder(u1SrcId,
                (prSwdmxMkvInfo->u8CurMoviPos + prSwdmxMkvInfo->u8MainFileOffset),
                prSwdmxMkvInfo->u8RangeSz - prSwdmxMkvInfo->u8CurMoviPos))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            //=== Wait feeder ack
            if (prSwdmxMkvInfo->i4ReqReadTimes > 0)
            {
                if (!_bGetDataFromFeederAck(u1SrcId, &rFeederReqData))
                {
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
            }
            if (prSwdmxMkvInfo->fgReadEOS)
            {
                FeederSetInfo(FEEDER_SWDMX_EOS,TRUE);
                break;
            }
        }
        #endif
    }

    //=== Check if end of range
    if ((!fgSpeedChange) &&
        ((prSwdmxMkvInfo->u8CurDmxMoviPos >= prSwdmxMkvInfo->u8RangeSz) ||
        ((prSwdmxMkvInfo->u8CurMoviPos >= prSwdmxMkvInfo->u8RangeSz) && (prSwdmxMkvInfo->fgIsUnderFlw)))&&
        (((prSwdmxMkvInfo->i4Speed < 0)&& prSwdmxMkvInfo->fgIsRewindToStart) || (prSwdmxMkvInfo->i4Speed > 0)))
    {
        if (prSwdmxMkvInfo->fgEnVideo && !prSwdmxMkvInfo->fgDeliverVEos)
        {
            LOG(6, "Cur DmxMoviPos = %ld, range size = %ld\n",
                prSwdmxMkvInfo->u8CurDmxMoviPos,
                prSwdmxMkvInfo->u8RangeSz);

            LOG(5, "Deliver Video Eos...\n");
            if (prSwdmxMkvInfo->fgAudTrigVidDec)
            {
                _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA);
            }
            if (!_SWDMX_SendMkvEos(u1SrcId, eSWDMX_MEDIA_VIDEO))
            {
                LOG(5, "Fail\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxMkvInfo->fgDeliverVEos = TRUE;
        }

        if (prSwdmxMkvInfo->fgEnAudio && !prSwdmxMkvInfo->fgDeliverAEos)
        {

       #ifdef AUD_PREBUF_EN
        if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
        {
            if (prSwdmxMkvInfo->fgActivePidxIsUnKnown == TRUE)
            {
                if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID])
                {
                    DMX_AudHandler_SetActivePidxEx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID],
                        (UINT8*)prSwdmxMkvInfo->u4AudHeaderPtr[prSwdmxMkvInfo->u4AStrmID],
                        prSwdmxMkvInfo->u4AudHdrLength[prSwdmxMkvInfo->u4AStrmID]);
                }
                else
                {
                    DMX_AudHandler_SetActivePidx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID]);
                }
            }
        }
       #endif
            LOG(5, "Deliver Audio Eos...\n");
            if (!_SWDMX_SendMkvEos(u1SrcId, eSWDMX_MEDIA_AUDIO))
            {
                LOG(5, "Fail\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxMkvInfo->fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    //reset range status
    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    //=== Check feeder status
    if (prSwdmxMkvInfo->fgFeederInvalid)
    {
        LOG(3, "Feeder is invalid now\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    //=== speed change handler
    if (fgSpeedChange)
    {
        if (prSwdmxMkvInfo->fgAudTrigVidDec)
        {
            _SWDMX_MkvAudioCb(u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE);
        }
        //prSwdmxMkvInfo->fgPendingAudMoveData = FALSE;
        // Reset some values for state change.
        if (prSwdmxMkvInfo->fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (prSwdmxMkvInfo->u4VDecType)
            {
            case ENUM_SWDMX_VDEC_DX311:
            case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8:
            case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_VP8:
            case ENUM_SWDMX_VDEC_MJPEG:
                LOG(7, "%s SearchStartCode = FALSE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            default:
                LOG(7, "%s SearchStartCode = TRUE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            }
            if (!DMX_MM_Set(prSwdmxMkvInfo->u1VidPid, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }

            prSwdmxMkvInfo->fgDeliverVEos = FALSE;
        }
        if (prSwdmxMkvInfo->fgDeliverAEos)
        {
            prSwdmxMkvInfo->fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
        }

        #ifdef AUD_PREBUF_EN
        DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
        prSwdmxMkvInfo->fgActivePidxIsUnKnown = TRUE;
        #endif

        // seek
        u4TargetPTS = VDP_GetPts(0);    //default value
        if ((prCmd->fgSeek) &&
           (prCmd->u4SeekMode == SWDMX_CUR_CMD_SEEK_OFFSET))
        {
            if (!prSwdmxMkvInfo->fgChangeSBDone)//before sb change done,trick,should clear the change info
            {
                _vMkvClearSpInfo(u1SrcId);
            }
            if (prSwdmxMkvInfo->u8CurDmxMoviPos > 0)  //in case of resume stop, will seek at beginning, sps/pps is still in Q
            {
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1VidPid);
                DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1AudPid);
                #ifdef AUD_PREBUF_EN
            //clear pre-buffered audio
            if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                {
                    if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
                    {
                        DMX_MM_FlushBuffer(prSwdmxMkvInfo->auAudPidx[u4Idx]);
                    }
                }
            }
            #endif
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
            }
            prSwdmxMkvInfo->fgIsSeek = FALSE;
            prSwdmxMkvInfo->u4LastCbVidPts = 0;
            prSwdmxMkvInfo->fgWaitIFrm = TRUE;
            _SWDMXResetFeeder(u1SrcId);
            prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;
            prSwdmxMkvInfo->u8CurDmxMoviPos = prCmd->rSeekOffset.ui8_i_frame_position;
            prSwdmxMkvInfo->u8CurMoviPos    = prCmd->rSeekOffset.ui8_i_frame_position;
            prSwdmxMkvInfo->fgSeekOffset = TRUE;
            prSwdmxMkvInfo->u8AudSeekPts = prCmd->rSeekOffset.ui8_pts_info;
            prSwdmxMkvInfo->u8VideoOnlyAdjustPts = prCmd->rSeekOffset.ui8_i_pts_info;
            if (!prSwdmxMkvInfo->fgEnAudio)
            {
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prCmd->rSeekOffset.ui8_pts_info);
            }
            else if (!prSwdmxMkvInfo->fgEnVideo)
            {
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prCmd->rSeekOffset.ui8_pts_info);
                if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID])
                {
                    DMX_AudHandler_SetActivePidxEx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID],
                        (UINT8*)prSwdmxMkvInfo->u4AudHeaderPtr[prSwdmxMkvInfo->u4AStrmID],
                        prSwdmxMkvInfo->u4AudHdrLength[prSwdmxMkvInfo->u4AStrmID]);
                }
                else
                {
                    DMX_AudHandler_SetActivePidx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID]);
                }
            }
            else
            {
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prCmd->rSeekOffset.ui8_pts_info);
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prCmd->rSeekOffset.ui8_pts_info);
                if ((UINT32)ENUM_SWDMX_ADEC_VORBIS == prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID])
                {
                    DMX_AudHandler_SetActivePidxEx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID],
                        (UINT8*)prSwdmxMkvInfo->u4AudHeaderPtr[prSwdmxMkvInfo->u4AStrmID],
                        prSwdmxMkvInfo->u4AudHdrLength[prSwdmxMkvInfo->u4AStrmID]);
                }
                else
                {
                    DMX_AudHandler_SetActivePidx(prSwdmxMkvInfo->auAudPidx[prSwdmxMkvInfo->u4AStrmID]);
                }
                prSwdmxMkvInfo->u4AudTrigVidDecPTS = prCmd->rSeekOffset.ui8_pts_info;
                //VDP_TriggerAudReceive(prCmd->rSeekOffset.ui8_pts_info);
                fgAudTrigVidDec = TRUE;
            }

        }
        else
        if (((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
            || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF))
        {
            if (!prSwdmxMkvInfo->fgChangeSBDone)//before sb change done,trick,should clear the change info
            {
                _vMkvClearSpInfo(u1SrcId);
            }
            u4TargetPTS = (UINT32)u8SeekPos;
            prSwdmxMkvInfo->fgIsSeek = TRUE;
            prSwdmxMkvInfo->u4LastCbVidPts = 0;
            prSwdmxMkvInfo->u8AudSeekPts = u8SeekPos;
            if (prSwdmxMkvInfo->fgEnVideo)
            {
                prSwdmxMkvInfo->fgWaitIFrm = TRUE;
            }
            fgSeek = TRUE;
            fgAudTrigVidDec = TRUE;
            // set stc
            STC_StopStc(prSwdmxInst->u1StcId);
            if ((u8SeekPos == 0) || (!prSwdmxMkvInfo->fgEnVideo))
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u8SeekPos);
            }//else will set in vdec
        }
        //normal (1x, 2x) to trick
        else if (((i4PlaySpeed > FF_2X) || IsBackward(i4PlaySpeed))
            &&
            (cal_abs(prSwdmxMkvInfo->i4PrevSpeed) <= FF_2X))
        {
            u4TargetPTS = VDP_GetPts(0);
            u1TrickNomalSwitch = NORMAL_2_TRICK;
            //query display struct
            if (prSwdmxMkvInfo->u1DispStruct == 0)
            {
                x_memset((void*)&rHdrInfo, 0, sizeof(rHdrInfo));
                if (VDEC_QueryInfo(prSwdmxInst->u1VdecId, &rHdrInfo))
                {
                    if (rHdrInfo.fgProgressiveSeq)
                    {
                        prSwdmxMkvInfo->u1DispStruct = DISP_PROG;
                    }
                    else
                    {
                        prSwdmxMkvInfo->u1DispStruct = DISP_INTERLACE;
                    }
                }
            }
            if (!prSwdmxMkvInfo->fgChangeSBDone)//before sb change done,trick,should clear the change info
            {
                _vMkvClearSpInfo(u1SrcId);
            }
        }
        //trick to normal (1x)
        else if ((((prSwdmxMkvInfo->i4PrevSpeed >= FF_2X) ||
            IsBackward(prSwdmxMkvInfo->i4PrevSpeed)) ||
            (prSwdmxMkvInfo->i4PrevSpeed == STEP_FW))
            &&
            (cal_abs(i4PlaySpeed) <= FF_2X))
        {
            if (prSwdmxMkvInfo->i4PrevSpeed == FF_2X)
            {
                if (prSwdmxMkvInfo->u1FF2xTo1xMode == FLUSH_VFIFO)
                {
                    VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);  //set DEC_FLG_WAIT_NEXT_I, stop put frame to dispQ
                    VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);              //clr dispQ
                    if ((prSwdmxMkvInfo->i4PrevSpeed == FF_2X) && prSwdmxMkvInfo->fgEnVideo)
                    {
                        STC_StopStc(prSwdmxInst->u1StcId);  //continue in VDEC_IsNotDisplay, in case large target/key diff
                    }
                }
                u4TargetPTS = VDP_GetPts(0) + 45000;    //pts for vdp trig audio
            }
            else
            {
                STC_SetStcValue(prSwdmxInst->u1StcId, u4TargetPTS);   //current display PTS
                prSwdmxMkvInfo->u8SeekPts = (UINT64)u4TargetPTS;
            }
            if (i4PlaySpeed == NORMAL_1X)
            {
                prSwdmxMkvInfo->u8AudSeekPts = (UINT64)u4TargetPTS;
                fgAudTrigVidDec = TRUE;
            }

            u1TrickNomalSwitch = TRICK_2_NORMAL;
            if (prSwdmxMkvInfo->prVidKeyIdxTbl==NULL
                &&prSwdmxMkvInfo->i4PrevSpeed>=FF_2X)
            {
                u1TrickNomalSwitch=TRICK_2_NORMAL;
                if (i4PlaySpeed!=NORMAL_1X)
                {
                    fgAudTrigVidDec=FALSE;
                }
            }
            else if ((prSwdmxMkvInfo->i4PrevSpeed == FF_2X) && (i4PlaySpeed == NORMAL_1X))
            {
                if ((prSwdmxMkvInfo->u1FF2xTo1xMode == KEEP_VFIFO) ||
                    (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF))
                {
                    u1TrickNomalSwitch = 0;
                    fgAudTrigVidDec = FALSE;
                }
            }
            else if ((prSwdmxMkvInfo->i4PrevSpeed ==  STEP_FW)&&(i4PlaySpeed == NORMAL_1X))
            {
                u1TrickNomalSwitch = 0;
                prSwdmxMkvInfo->u8AudSeekPts += 45000;
                STC_StartStc(prSwdmxInst->u1StcId);
                DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1AudPid);
            }
        }
        else if (((prSwdmxMkvInfo->i4PrevSpeed > STEP_FW) &&
            (prSwdmxMkvInfo->i4PrevSpeed < NORMAL_1X)) &&
            (i4PlaySpeed == NORMAL_1X))   //SF to 1X
        {
            //VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);  //set DEC_FLG_WAIT_NEXT_I, stop put frame to dispQ
            //VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);              //clr dispQ
            u4TargetPTS = VDP_GetPts(0) + 10000;    //pts for vdp trig audio
            prSwdmxMkvInfo->u8AudSeekPts = (UINT64)u4TargetPTS;
            //fgAudTrigVidDec = TRUE;
            //u1TrickNomalSwitch = TRICK_2_NORMAL;
        }
        if (prSwdmxInst->ePrePlayMode==SWDMX_PLAY_MODE_NORMAL)
        {
            prSwdmxMkvInfo->i4LastSpeed=NORMAL_1X;
        }
        else
        {
            prSwdmxMkvInfo->i4LastSpeed = prSwdmxMkvInfo->i4PrevSpeed;
        }
        prSwdmxMkvInfo->i4PrevSpeed = i4PlaySpeed;                              //current speed

        //TPT
        #ifdef DIVX_PLUS_CER
        if ((i4PlaySpeed >= FF_2X) ||
            IsBackward(i4PlaySpeed))
        {
            prSwdmxMkvInfo->fgIsTPT = _SWDMXMkvSwitchTrack(TRUE, prSwdmxMkvInfo);
            if (prSwdmxMkvInfo->fgIsTPT)
            {
                VDEC_SetTPTFlag(u1SrcId, TRUE);
                fgSeek = TRUE;
                if ((i4PlaySpeed >= FF_2X)&&(i4PlaySpeed <= FF_8X))
                {
                    STC_SetSyncMode(prSwdmxInst->u1StcId,
                    prSwdmxInst->u1AdecId,
                    prSwdmxInst->u1VdecId, AV_SYNC_MODE_VIDEO_MASTER);
                }
            }
        }
        else
        {
            if (prSwdmxMkvInfo->fgIsTPT)
            {
                fgSeek = TRUE;
            }
            prSwdmxMkvInfo->fgIsTPT = _SWDMXMkvSwitchTrack(FALSE, prSwdmxMkvInfo);
        }
        #endif

        //seek and reset feeder
        if ((u1TrickNomalSwitch > 0) || fgSeek)  //except 1x to 2x
        {
            BOOL fgSeekRet=FALSE;
            //flush buffer before "set vdp trigger aud pts" and "set render pts"
            if (prSwdmxMkvInfo->u8CurDmxMoviPos > 0)  //in case of resume stop, will seek at beginning, sps/pps is still in Q
            {
                MPV_FlushEsmQ(prSwdmxInst->u1VdecId, TRUE);
                DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1VidPid);
            }
            DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1AudPid);
            prSwdmxMkvInfo->fgTtickSeekToNormal = TRUE;

            #ifdef AUD_PREBUF_EN
            //clear pre-buffered audio
            if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
                {
                    if (prSwdmxMkvInfo->fgIsAudTrack[u4Idx])
                    {
                        DMX_MM_FlushBuffer(prSwdmxMkvInfo->auAudPidx[u4Idx]);
                    }
                }
            }
            #endif

            //TPT
            #ifdef DIVX_PLUS_CER
            if (prSwdmxMkvInfo->fgIsTPT)
            {
                _vMkvVidHdrParsing(u1SrcId, prSwdmxMkvInfo->u4TPTVidHeaderPtr, prSwdmxMkvInfo->u4TPTVidHdrLength);
            }
            else
            {
                _vMkvVidHdrParsing(u1SrcId, prSwdmxMkvInfo->u4MainVidHeaderPtr, prSwdmxMkvInfo->u4MainVidHdrLength);
            }
            #endif
            if (prSwdmxMkvInfo->prVidKeyIdxTbl==NULL)
            {
                fgSeekRet=_SWDMX_MkvSeekCurPos(u1SrcId);
                u4TargetPTS=prSwdmxMkvInfo->u4CurVidPts;
            }
            else
            {
                fgSeekRet=_bSeekTime(u1SrcId, u4TargetPTS, prRangeList);
            }
            //seek

            if (fgSeekRet)
            {
                LOG(5, "TargetPTS 0x%x, SeekPos 0x%x\n", u4TargetPTS, (UINT32)u8SeekPos);

                if (prSwdmxMkvInfo->fgEnVideo)
                {
                    if (!IsBackward(i4PlaySpeed))
                    {
                        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, u4TargetPTS);
                    }
                    else
                    {   //common part will wet as u4TargetPTS
                        VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, 0);
                    }
                }
                //will clr all frames in DPB @ EsFlwCtrl by setting DEC_FLG_WAIT_NEXT_I
                //will not display frm by checking Vdec_IsNotDisplay
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                //if (fgSeek)
                {
                    prSwdmxMkvInfo->fgIsSeek = TRUE;  // start parsing pos is cluster offset, not key
                    prSwdmxMkvInfo->u4LastCbVidPts = 0;
                    prSwdmxMkvInfo->u8SeekPts = prSwdmxMkvInfo->u4CurVidPts; //start send vid data
                }
            }
            else    //no key table
            {
                prSwdmxMkvInfo->fgIsSeek = FALSE;
                prSwdmxMkvInfo->u4LastCbVidPts = 0;

                //will clr all frames in DPB @ EsFlwCtrl by setting DEC_FLG_WAIT_NEXT_I
                //will not display frm by checking Vdec_IsNotDisplay
                VDEC_ReleaseDispQ(prSwdmxInst->u1VdecId);
                STC_SetStcValue(prSwdmxInst->u1StcId, 0);
                prSwdmxMkvInfo->u8AudSeekPts = 0;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
    }
    //for divx subtitle change
    if (prSwdmxMkvInfo->fgIsSeek)
    {
        prSwdmxMkvInfo->fgChgSbTrack = FALSE;
        prSwdmxMkvInfo->fgChangeSBDone = TRUE;
        _vMkvClearSpInfo(u1SrcId);
    }
    else if (prSwdmxMkvInfo->fgChgSbTrack)
    {
        if (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME
            || (prSwdmxMkvInfo->u8CurDmxMoviPos <= prSwdmxMkvInfo->u8SBClusterPos[prSwdmxMkvInfo->u4SpStrmID]))
        {
            prSwdmxMkvInfo->fgChgSbTrack = FALSE;
            prSwdmxMkvInfo->fgChangeSBDone = TRUE;
            _vMkvClearSpInfo(u1SrcId);
        }
        else
        {
            if (!prSwdmxMkvInfo->fgChangeSBDone)
            {
                LOG(5,"under subtitle reparsing\n");
            }
            else if (prSwdmxMkvInfo->u4CurVidPts <= 45000)
            {
                LOG(5,"too close to the video display, don't reparse 1\n");
            }
            else
            {
                i4StcNow = STC_GetStcValue(prSwdmxInst->u1StcId);
                i4Sub = prSwdmxMkvInfo->u4CurVidPts - 45000;
                if ((prSwdmxMkvInfo->u4SubPts[prSwdmxMkvInfo->u4SpStrmID] + 90000 * 3) <= i4StcNow)
                {
                    LOG(5,"No subtitle available subtitle data\n");
                }
                else if (i4Sub <= i4StcNow)
                {
                    LOG(5,"too close to the video display, don't reparse 2\n");
                }
                else
                {
                    LOG(5,"start to remove data\n");
                    //LOG(0,"%d, %d, %d\n", i4StcNow, prSwdmxMkvInfo->u4CurVidPts, prSwdmxMkvInfo->u4SubPts[prSwdmxMkvInfo->u4SpStrmID]);
                    _SWDMXResetFeeder(u1SrcId);
                    prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;
                    prSwdmxMkvInfo->u8CurDmxMoviPos = prSwdmxMkvInfo->u8SBClusterPos[prSwdmxMkvInfo->u4SpStrmID];
                    prSwdmxMkvInfo->u8CurMoviPos    = prSwdmxMkvInfo->u8SBClusterPos[prSwdmxMkvInfo->u4SpStrmID];
                    prSwdmxMkvInfo->fgChangeSBDone = FALSE;
                    prSwdmxMkvInfo->u4SBSkipVidPts = prSwdmxMkvInfo->u4LastCbVidPts;
                    LOG(1,"prSwdmxMkvInfo->u4SBSkipVidPts = 0x%x,last =0x%x\n",prSwdmxMkvInfo->u4CurVidPts,prSwdmxMkvInfo->u4LastCbVidPts);
                    prSwdmxMkvInfo->fgIsVidSkipDone = FALSE;
                    for (i = 0; i < MKV_MAX_AUD_TRACK; i++)
                    {
                        if (prSwdmxMkvInfo->fgIsAudTrack[i])
                        {
                            prSwdmxMkvInfo->fgIsAudSkipDone[i] = FALSE;
                            prSwdmxMkvInfo->u4BKSBSkipAudPts[i] = prSwdmxMkvInfo->u4SBSkipAudPts[i];
                        }
                    }
                    prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
                    prSwdmxMkvInfo->u4LastAlignOffset = 0;
                    if (!_bReqDataFromFeeder(u1SrcId,
                        (prSwdmxMkvInfo->u8CurMoviPos + prSwdmxMkvInfo->u8FileOffset),
                        (prSwdmxMkvInfo->u8RangeSz - prSwdmxMkvInfo->u8CurMoviPos)))
                    {
                        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                    }
                    prSwdmxMkvInfo->fgWaitStartAddr = TRUE;
                    prSwdmxMkvInfo->u4CurDmxFeederRPtr = prFeederMkvInfo->u4WriteAddr;
                }
            }
             prSwdmxMkvInfo->fgChgSbTrack = FALSE;
        }
    }

    // trigger video dec when audio start to decode(by callback function)
    // avoid VDP_trigger audio aout before aud start to decode
    if ((fgAudTrigVidDec &&
        prSwdmxMkvInfo->fgEnVideo) &&
        (prSwdmxMkvInfo->fgEnAudio &&
        (prSwdmxMkvInfo->u4ADecType[prSwdmxMkvInfo->u4AStrmID] > ENUM_SWDMX_ADEC_UNKNOWN)))
    {
        LOG(1, "VidPause by Swdmx\n");
        VDEC_Pause(prSwdmxInst->u1VdecId);
        prSwdmxMkvInfo->fgAudTrigVidDec = TRUE;
        //prSwdmxMkvInfo->u4AudTrigVidDecPTS = u4TargetPTS;
    }

    //===  Request data for the first time, pre-request
#if 0
    #ifdef CC_SWDMX_RENDER_POSITION
    if ((((prSwdmxMkvInfo->u8CurMoviPos == 0) || fgSeek ||prCmd->fgSeek ||prRangeList->fgSetRenderPosition) ||(u1TrickNomalSwitch == TRICK_2_NORMAL)) &&
        ((((i4PlaySpeed <= NORMAL_1X)&&(i4PlaySpeed >= STEP_FW)) || (i4PlaySpeed == FF_2X) ||
        (prSwdmxMkvInfo->fgIsTPT)) && (prSwdmxMkvInfo->i4ReqReadTimes == 0)))
    {
        prRangeList->fgSetRenderPosition = FALSE;
    #else
    if ((((prSwdmxMkvInfo->u8CurMoviPos == 0) || fgSeek ||prCmd->fgSeek) ||(u1TrickNomalSwitch == TRICK_2_NORMAL)) &&
        ((((i4PlaySpeed <= NORMAL_1X)&&(i4PlaySpeed >= STEP_FW)) || (i4PlaySpeed == FF_2X) ||
        (prSwdmxMkvInfo->fgIsTPT)) && (prSwdmxMkvInfo->i4ReqReadTimes == 0)))
    {
    #endif
    #endif

    if (((prSwdmxMkvInfo->u8CurMoviPos == 0)
            || fgSeek
            ||prCmd->fgSeek
#ifdef CC_SWDMX_RENDER_POSITION
            ||prRangeList->fgSetRenderPosition
#endif
            || (u1TrickNomalSwitch == TRICK_2_NORMAL))
        && (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_NORMAL
            || (prSwdmxMkvInfo->fgIsTPT))
        && (prSwdmxMkvInfo->i4ReqReadTimes == 0))
    {
#ifdef CC_SWDMX_RENDER_POSITION
        prRangeList->fgSetRenderPosition = FALSE;
#endif

        prSwdmxMkvInfo->u4PrsState = MKVPRS_G_SEGMENT_ELEMENT_ID;
        prSwdmxMkvInfo->u4LastAlignOffset = 0;
        if (!_bReqDataFromFeeder(u1SrcId,
            (prSwdmxMkvInfo->u8CurMoviPos + prSwdmxMkvInfo->u8FileOffset),
            (prSwdmxMkvInfo->u8RangeSz - prSwdmxMkvInfo->u8CurMoviPos)))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        prSwdmxMkvInfo->fgWaitStartAddr = TRUE;
        prSwdmxMkvInfo->u4CurDmxFeederRPtr = prFeederMkvInfo->u4WriteAddr;
    }
    //=== Wait feeder ack
    if (prSwdmxMkvInfo->i4ReqReadTimes > 0)
    {
        if (!_bGetDataFromFeederAck(u1SrcId, &rFeederReqData))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        else
        {
            LOG(6, "Ack 0x%x--0x%x, ofst 0x%x\n",
                rFeederReqData.u4WriteAddr,
                rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize,
                prSwdmxMkvInfo->u4LastAlignOffset);
            prSwdmxMkvInfo->fgIsUnderFlw = FALSE;
            if (prSwdmxMkvInfo->fgWaitStartAddr)
            {
                prSwdmxMkvInfo->u4CurDmxFeederRPtr = rFeederReqData.u4WriteAddr +
                    prSwdmxMkvInfo->u4LastAlignOffset;
                prSwdmxMkvInfo->fgWaitStartAddr = FALSE;
            }
            if (prSwdmxMkvInfo->u8CurMoviPos >= prSwdmxMkvInfo->u8RangeSz)
            {
                prSwdmxMkvInfo->fgReadEOS = TRUE;
            }
            else
            {
                prSwdmxMkvInfo->fgReadEOS = FALSE;
            }
        }
    }

    //=== trick mode, 4X/8X/...
    prSwdmxMkvInfo->fgIsJump = FALSE;
    prSwdmxMkvInfo->i4PrevSpeed = i4PlaySpeed;

    if (prSwdmxInst->ePlayMode==SWDMX_PLAY_MODE_IFRAME
        &&(prSwdmxMkvInfo->fgIsTPT == FALSE || i4PlaySpeed > FF_8X ||IsBackward(i4PlaySpeed)))
    {
        prSwdmxMkvInfo->fgIsJump = TRUE;
        return _MKVCuePointParsing(u1SrcId, prRangeList, i4PlaySpeed);
    }
    else
    {
        //=== Normal play, FF_2X, SF
        //=== Request data
        prSwdmxMkvInfo->u4KeyFrmPTS = 0;
        if ((prSwdmxMkvInfo->u4LeftBytesInFeeder < (MKV_BUFFER_SIZE - MIN_READ_SIZE)) &&
            (prSwdmxMkvInfo->u8RangeSz > prSwdmxMkvInfo->u8CurMoviPos))
        {
            if (!_bReqDataFromFeeder(u1SrcId,
                (prSwdmxMkvInfo->u8CurMoviPos + prSwdmxMkvInfo->u8FileOffset),
                (prSwdmxMkvInfo->u8RangeSz - prSwdmxMkvInfo->u8CurMoviPos)))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

        if (prSwdmxMkvInfo->u4LeftBytesInFeeder > 0)
        {
            //=== Sequential parsing from cluster
            _vMKVClusterParsing(u1SrcId,
                (UCHAR*)(VIRTUAL(prSwdmxMkvInfo->u4CurDmxFeederRPtr)),
                prRangeList,
                prSwdmxMkvInfo->u4LeftBytesInFeeder,
                &u4ConsumedSize);
            LOG(6, "Prs 0x%x--0x%x\n", prSwdmxMkvInfo->u4CurDmxFeederRPtr,
                prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize);

            //mkv test
            x_thread_delay(10);

            //=== Update rptr
            prSwdmxMkvInfo->u8CurDmxMoviPos += u4ConsumedSize;
            if (prSwdmxMkvInfo->u4LeftBytesInFeeder < u4ConsumedSize)
            {
                LOG(1, "Parse over available size!, %d\n",
                    (u4ConsumedSize - prSwdmxMkvInfo->u4LeftBytesInFeeder));
                prSwdmxMkvInfo->fgWaitStartAddr = TRUE;
                prSwdmxMkvInfo->u8CurMoviPos += u4ConsumedSize - prSwdmxMkvInfo->u4LeftBytesInFeeder;
                u4ConsumedSize = prSwdmxMkvInfo->u4LeftBytesInFeeder;
                prSwdmxMkvInfo->u4LeftBytesInFeeder = 0;
                _SWDMXResetFeeder(u1SrcId);
            }
            else
            {
                prSwdmxMkvInfo->u4LeftBytesInFeeder -= u4ConsumedSize;
            }

            if ((prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >=
                prFeederMkvInfo->u4EndAddr)
            {
                prSwdmxMkvInfo->u4CurDmxFeederRPtr =  prFeederMkvInfo->u4StartAddr +
                    ((prSwdmxMkvInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -prFeederMkvInfo->u4EndAddr);
            }
            else
            {
                prSwdmxMkvInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
            }

            FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, prSwdmxMkvInfo->u4CurDmxFeederRPtr);
        }
        else
        {
            prSwdmxMkvInfo->u8CurDmxMoviPos = prSwdmxMkvInfo->u8RangeSz;
        }
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


BOOL _SWDMX_MKVEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            LOG(5, "_SWDMX_MKVEnableStrm Vid\n");
            prSwdmxMkvInfo->fgEnVideo = TRUE;
            //if (prSwdmxMkvInfo->fgEnVDmx == FALSE)
            //{
            //    UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_VIDEO, NULL, FALSE));
             //   prSwdmxMkvInfo->fgEnVDmx = TRUE;
            //
            //}
            break;
        case eSWDMX_STRM_TYPE_AUD:
            LOG(5, "_SWDMX_MKVEnableStrm Aud\n");
            //change to set range for audio-prebuffer parameter
            /*if (prSwdmxMkvInfo->fgEnADmx == FALSE)
            {
                UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_AUDIO, NULL, FALSE));
                prSwdmxMkvInfo->fgEnADmx = TRUE;
            }*/

            if ((!prSwdmxMkvInfo->fgBeginToPlay) &&
                prSwdmxMkvInfo->i4PrevSpeed == 1000)
            {
                prSwdmxMkvInfo->fgChgATrack = TRUE;
            }
            prSwdmxMkvInfo->fgEnAudio = TRUE;
            x_memset(&prSwdmxMkvInfo->u4OggPage, 0, sizeof(prSwdmxMkvInfo->u4OggPage));
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if (prSwdmxMkvInfo->fgEnSubDmx)
            {
                prSwdmxMkvInfo->fgChgSbTrack = TRUE;
            }
            if (prSwdmxMkvInfo->fgEnSubDmx == FALSE) //&&
               // (pfnCbFunc))
            {
                UNUSED(_SWDMXInitDmx(u1SrcId, eSWDMX_MEDIA_SUBTITLE, pfnCbFunc, FALSE));
                prSwdmxMkvInfo->fgEnSubDmx = TRUE;
            }
            prSwdmxMkvInfo->fgEnSubTitle = TRUE;
            LOG(5, "EnableStrm Sub\n");
           break;
        default:
            return FALSE;
    }

    return TRUE;

}


BOOL _SWDMX_MKVDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType)
{
    DMX_MM_T rPid;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    x_memset((void*)&rPid, 0, sizeof(DMX_MM_T));

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxMkvInfo->fgEnVideo = FALSE;
            if (prSwdmxMkvInfo->fgIsAudWaitAout)
            {
                if (prSwdmxMkvInfo->fgAudioNnotifyDecodeDone)
                {
                    AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                    prSwdmxMkvInfo->fgIsAudWaitAout = FALSE;
                    STC_StartStc(prSwdmxInst->u1StcId);
                }
            }
            LOG(5, "_SWDMX_MKVDisableStrm Vid\n");
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxMkvInfo->fgEnAudio = FALSE;
            DMX_MM_FlushBuffer(prSwdmxMkvInfo->u1AudPid);
            prSwdmxMkvInfo->fgDeliverAEos= FALSE;
            #ifdef AUD_PREBUF_EN
            if (prSwdmxMkvInfo->u1FF2xTo1xMode == AUD_PRE_BUF)
            {
                DMX_AudHandler_SetActivePidx(MKV_AUD_UNKNOWN_IDX);
                prSwdmxMkvInfo->fgActivePidxIsUnKnown = TRUE;
            }
            #endif
            LOG(5, "_SWDMX_MKVDisableStrm Aud, flush\n");
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            prSwdmxMkvInfo->fgEnSubTitle = FALSE;
            LOG(5, "DisableStrm Sub\n");
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_MKVStop(UINT8 u1SrcId)
{
    DMX_MM_T rPid;
    FEEDER_REQ_DATA_T rFeederReqData;
    #ifdef AUD_PREBUF_EN
    UINT32 u4Idx;
    #endif
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    x_memset((void*)&rFeederReqData, 0, sizeof(FEEDER_REQ_DATA_T));
    x_memset((void*)&rPid, 0, sizeof(DMX_MM_T));
    rPid.fgEnable = FALSE;

    // wait feeder ack
    /*while(prSwdmxMkvInfo->i4ReqReadTimes > 0)
    {
        //prSwdmxMkvInfo->i4ReqReadTimes--;
        if (!_bGetDataFromFeederAck(u1SrcId, &rFeederReqData))
        {
            return FALSE;
        }
    }*/

    if (prSwdmxMkvInfo->fgEnVideo && !prSwdmxMkvInfo->fgDeliverVEos)
    {
        if (!_SWDMX_SendMkvEos(u1SrcId, eSWDMX_MEDIA_VIDEO))
        {
            LOG(3, "Fail to send Video Eos\n");
            //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        prSwdmxMkvInfo->fgDeliverVEos = TRUE;
    }

    if (prSwdmxMkvInfo->fgEnAudio && !prSwdmxMkvInfo->fgDeliverAEos)
    {
        if (!_SWDMX_SendMkvEos(u1SrcId, eSWDMX_MEDIA_AUDIO))
        {
            LOG(3, "Fail to send Audio Eos\n");
            //return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
        prSwdmxMkvInfo->fgDeliverAEos = TRUE;
    }

    if (prSwdmxMkvInfo->fgEnADmx)
    {
        prSwdmxMkvInfo->fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxMkvInfo->u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxMkvInfo->u1AudPid);
        DMX_MUL_FreePidx(prSwdmxMkvInfo->u1AudPid);

        #ifdef AUD_PREBUF_EN
        for (u4Idx = 0; u4Idx < MKV_MAX_AUD_TRACK; u4Idx++)
        {
            if (prSwdmxMkvInfo->fgAudPreBuf[u4Idx])
            {
                VERIFY(DMX_MM_Set(prSwdmxMkvInfo->auAudPidx[u4Idx], DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(prSwdmxMkvInfo->auAudPidx[u4Idx]);
                DMX_AudHandler_Free(prSwdmxMkvInfo->auAudPidx[u4Idx]);
                DMX_MUL_FreePidx(prSwdmxMkvInfo->auAudPidx[u4Idx]);
            }
        }
        DMX_AudHandler_SetEnable(FALSE);
        #endif
    }

    if (prSwdmxMkvInfo->fgEnVDmx == TRUE)
    {
        prSwdmxMkvInfo->fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxMkvInfo->u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxMkvInfo->u1VidPid);
        DMX_MUL_FreePidx(prSwdmxMkvInfo->u1VidPid);

    }

    if (prSwdmxMkvInfo->fgEnSubDmx)
    {
        prSwdmxMkvInfo->fgEnSubDmx = FALSE;
        VERIFY(DMX_MM_Set(prSwdmxMkvInfo->u1SubPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxMkvInfo->u1SubPid);
        DMX_MUL_FreePidx(prSwdmxMkvInfo->u1SubPid);
    }

    x_memset((void*)prSwdmxMkvInfo, 0, sizeof(SWDMX_MKV_INFO_T));
    x_memset((void*)prFeederMkvInfo, 0, sizeof(FEEDER_BUF_INFO_T));

    return TRUE;

}


BOOL _SWDMX_MKVFlush(UINT8 u1SrcId)
{
    //_SWDMX_FlushReqMsgQ();
    //prSwdmxMkvInfo->i4ReqReadTimes = 0;
    UNUSED(u1SrcId);
    return TRUE;
}

BOOL _SWDMX_MkvAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        prSwdmxMkvInfo->fgAudioNnotifyDecodeDone = TRUE;
        if (prSwdmxMkvInfo->fgEnVideo)
        {
            if (prSwdmxMkvInfo->fgAudTrigVidDec)
            {
                switch((ENUM_SWDMX_VDEC_T)prSwdmxMkvInfo->u4VDecType)
                {
                case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
                    break;
                case ENUM_SWDMX_VDEC_DX311:
                case ENUM_SWDMX_VDEC_MPEG4:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
                    break;
                case ENUM_SWDMX_VDEC_H264:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
                    break;
                case ENUM_SWDMX_VDEC_VC1:
                case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8:
                case ENUM_SWDMX_VDEC_WMV9:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
                    break;
                case ENUM_SWDMX_VDEC_VP8:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_VP8);
                    break;
                case ENUM_SWDMX_VDEC_MJPEG:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MJPEG);
                    break;
                default:
                    LOG(1, "VidPlay by AudCb fail, Unknow VidType %d!\n", prSwdmxMkvInfo->u4VDecType);
                    return TRUE;
                }
                if (prSwdmxMkvInfo->u8AudSeekPts == 0)
                {
                    LOG(3,"VidPlay by AudCb, vdp trig aud PTS 0x%x\n", prSwdmxMkvInfo->u4AudTrigVidDecPTS);
                    VDP_TriggerAudReceive(prSwdmxMkvInfo->u4AudTrigVidDecPTS);
                }
                else
                {
                    LOG(1, "AudCb before feeding data!\n");
                    AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);  // for EOS, aud will not notify EOS if Aout Disable
                }
                prSwdmxMkvInfo->fgAudTrigVidDec = FALSE;
            }
            prSwdmxMkvInfo->fgIsAudWaitAout = TRUE;
            if (prSwdmxMkvInfo->u8AudSeekPts == 0)
            {
                LOG(3,"VidPlay by AudCb, vdp trig aud PTS 0x%x\n", prSwdmxMkvInfo->u4AudTrigVidDecPTS);
                VDP_TriggerAudReceive(prSwdmxMkvInfo->u4AudTrigVidDecPTS);
            }
        }
        else
        {
            STC_StartStc(prSwdmxInst->u1StcId);
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
            prSwdmxMkvInfo->fgIsAudWaitAout = FALSE;
        }
    }
    else if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA)
    {
        if (prSwdmxMkvInfo->fgEnVideo)
        {
            if (prSwdmxMkvInfo->fgAudTrigVidDec)
            {
                switch((ENUM_SWDMX_VDEC_T)prSwdmxMkvInfo->u4VDecType)
                {
                case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
                    break;
                case ENUM_SWDMX_VDEC_DX311:
                case ENUM_SWDMX_VDEC_MPEG4:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
                    break;
                case ENUM_SWDMX_VDEC_H264:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
                    break;
                case ENUM_SWDMX_VDEC_VC1:
                case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8:
                case ENUM_SWDMX_VDEC_WMV9:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
                    break;
                case ENUM_SWDMX_VDEC_VP8:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_VP8);
                    break;
                case ENUM_SWDMX_VDEC_MJPEG:
                    VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MJPEG);
                    break;
                default:
                    LOG(1, "VidPlay by AudCb fail, Unknow VidType %d!\n", prSwdmxMkvInfo->u4VDecType);
                    return TRUE;
                }
                prSwdmxMkvInfo->fgAudTrigVidDec = FALSE;
            }
        }
    }
    else
    {
        LOG(1, "AudCb Type Unknow %d\n", (UINT32)eAudioNotifyType);
    }
    return TRUE;
}

BOOL _SWDMX_MkvReadBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType,
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
        SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);

    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            LOG(3, "Mkv Get Subtitle Buffer serial number = 0x%x Frame Address = 0x%08x",
                prReadInfo->u1SerialNumber,
                prReadInfo->u4FrameAddr);

            fgRet = DMX_MM_GetBuffer(prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u1SubPid,
                prReadInfo->u1SerialNumber,
                (UINT32)prReadInfo->u4FrameAddr,
            0, (UINT32)prReadInfo->u4CopyLen,
            prReadInfo->pucDest);

            if (!fgRet)
            {
                LOG(3, "_SWDMX_MkvReadBuffer, get buffer error\n");
            }
            break;
        default:
            return FALSE;
    }

    return fgRet;
}

BOOL _SWDMX_MkvReleaseBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType,
    SWDMX_READ_INFO_T *prReadInfo)
{
    BOOL  fgRet = FALSE;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    switch (eStrmType)
    {
        case eSWDMX_STRM_TYPE_SUBTITLE:
            fgRet = DMX_MM_UnlockBuffer( prSwdmxInst->rFmtInfo.rSwdmxMkvInfo.u1SubPid ,
                prReadInfo->u1SerialNumber,
                (UINT32)prReadInfo->u4CopyLen,
                (UINT32)prReadInfo->u4FrameAddr);

            if (!fgRet)
            {
                LOG(3, "_SWDMX_MkvReleaseBuffer, unlock buffer error\n");
            }
            break;
        default:
            return FALSE;
    }

    return fgRet;
}


static BOOL _SWDMXMkvPrsDrmInfo(UINT8 u1SrcId, UCHAR* pucBitstream, UINT32 u4BufLength)
{
    UCHAR *pucBuf = (UCHAR*)VIRTUAL((UINT32)pucBitstream);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;

    if (u4BufLength < DIVX_DRM_LENGTH)
    {
        LOG(5, "Wrong Length for DivX DRM\n");
        return FALSE;
    }

    prSwdmxMkvInfo->fgDrmEnable = TRUE;
    prSwdmxMkvInfo->u2FrameKeyIdx = (UINT16)((pucBuf[0]) |
                                                                         (pucBuf[1] << 8));

    prSwdmxMkvInfo->u4EncryptOffset = (pucBuf[2] |
                                    (pucBuf[3] << 8) |
                                    (pucBuf[4] << 16) |
                                    (pucBuf[5] << 24));


    prSwdmxMkvInfo->u4EncryptLength = (pucBuf[6] |
                                     (pucBuf[7] << 8) |
                                     (pucBuf[8] << 16) |
                                     (pucBuf[9] << 24));

    return TRUE;
}


static BOOL _SWDMXMkvDecDrmData(UINT8 u1SrcId, UINT8 u1HdrType, UCHAR* pucFifo, UINT32 u4BlkSz)
{
    DMX_MM_DRM_AES_T rDRM = {0};
    DMX_AES_DATA_T rAESData = {0};
    DIVX_DRM_CRYPTO_INFO_T *prDrmInfo;
    UINT32 u4Offset;
    SWDMX_INFO_T *prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_MKV_INFO_T *prSwdmxMkvInfo = NULL;
    FEEDER_BUF_INFO_T* prFeederMkvInfo;
    SWDMX_MKV_VERIFY_NULL_ERROR(prSwdmxInst, FALSE);
    prSwdmxMkvInfo =
        (SWDMX_MKV_INFO_T*)&prSwdmxInst->rFmtInfo.rSwdmxMkvInfo;
    prFeederMkvInfo = &prSwdmxMkvInfo->rFeederMkvInfo;

    if (prSwdmxMkvInfo->prDmxRangeInfo->b_drm)
    {
        rDRM.fgCBC = FALSE;
        rDRM.u4KeyBitLen = MKV_DIVX_DRM_AES_SIZE_128;

        prDrmInfo =
            prSwdmxMkvInfo->prDmxRangeInfo->t_drm_info_drv.u.t_divx_drm_info_drv.pt_divx_drm_crypto_info;

        if ((prSwdmxMkvInfo->fgDrmEnable) && (u1HdrType == eSWDMX_MEDIA_VIDEO) &&
            (prSwdmxMkvInfo->u4EncryptLength != 0) &&
            (prSwdmxMkvInfo->u2FrameKeyIdx < 128) &&
            ((prSwdmxMkvInfo->u4EncryptOffset + prSwdmxMkvInfo->u4EncryptLength) < u4BlkSz))
        {
            u4Offset = prSwdmxMkvInfo->u4EncryptOffset;
            rDRM.u4EncryLen = prSwdmxMkvInfo->u4EncryptLength;

            x_memcpy(rDRM.au1Key,
                prDrmInfo->aui1_frame_keys[prSwdmxMkvInfo->u2FrameKeyIdx],
                (MKV_DIVX_DRM_AES_SIZE_128/8));
        }
        else if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
            (prDrmInfo->ui1_flag & DIVX_DRM_AUDIO_PROTECTION))
        {
            if (u4BlkSz < prDrmInfo->ui1_protected_audio_offset)
            {
                LOG(2, "Audio DRM offset is larger than Audio Chunk Size Impossible\n");
                return FALSE;
            }
            else
            {
                if (u4BlkSz < prDrmInfo->ui1_protected_audio_offset + prDrmInfo->ui1_protected_audio_crypto_size)
                {
                    // DivX SDK 6.1
                    // fs_divx_cesdk_drm.pdf
                    // 9 Audio Protection
                    // If the drmMode is set to Protected Audio, all audio chunks that have a size greater than
                    // (protectedAudioCryptoSize + protectedAudioOffset) are encrypted. If the chunk is less
                    // than that size, then it was not encrypted.
                    // Actually, this spec does not define "equal" case...
                    return TRUE;
                }
                else
                {
                    rDRM.u4EncryLen = prDrmInfo->ui1_protected_audio_crypto_size;
                }
                u4Offset = prDrmInfo->ui1_protected_audio_offset;

                x_memcpy(rDRM.au1Key,
                    prDrmInfo->aui1_frame_keys[0],
                    (MKV_DIVX_DRM_AES_SIZE_128/8));
            }
        }
        else
        {
            LOG(2, "DRM type/param err!\n");
            return FALSE;
        }

        if (!DMX_AES_SetDecryptInfo(DMX_MM_DRM_FLAG_ALL, &rDRM))
        {
            LOG(2, "DMX_AES_SetDecryptInfo fail\n");
        }

        rAESData.u4Size = rDRM.u4EncryLen;

        rAESData.u4SrcBufStart = prFeederMkvInfo->u4StartAddr;
        rAESData.u4SrcBufEnd = prFeederMkvInfo->u4EndAddr;
        rAESData.u4SrcStart = (UINT32)pucFifo + u4Offset;

        if (rAESData.u4SrcBufStart > rAESData.u4SrcStart)
        {
            // workaround for ring copy case
            rAESData.u4SrcBufStart = rAESData.u4SrcStart & ~0xf;
        }

        if (rAESData.u4SrcStart >= prFeederMkvInfo->u4EndAddr)
        {
            rAESData.u4SrcStart =
                (rAESData.u4SrcStart - prFeederMkvInfo->u4EndAddr) + prFeederMkvInfo->u4StartAddr;
        }

        ASSERT(rAESData.u4SrcStart >= rAESData.u4SrcBufStart);
        ASSERT(rAESData.u4SrcStart < rAESData.u4SrcBufEnd);

        rAESData.u4DstBufStart = rAESData.u4SrcBufStart;
        rAESData.u4DstBufEnd = rAESData.u4SrcBufEnd;
        rAESData.u4DstStart = rAESData.u4SrcStart;
        if (!DMX_AES_MoveData(&rAESData))
        {
            LOG(2, "DMX_AES_MoveData fail\n");
        }

        return TRUE;
    }
    UNUSED(u4Offset);
    return FALSE;
}


