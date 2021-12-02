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
 * $RCSfile: swdmx_cmpb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_cmpb.c
 *  Software demux for CMPB - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_cmpb.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"
#include "x_util.h"
#include "x_timer.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CMPB_DMX_MV_RETRY (10)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//#define CMPB_TIME_PROFILE
#define cmpb_min(x, y) (((x) < (y)) ? (x) : (y))
#define cmpb_max(x, y) (((x) > (y)) ? (x) : (y))

#define cmpb_abs(x)                  (((x) >= 0) ? (x) : -(x))

#define CMPB_RPOS_INC(src,len,startaddr,endaddr) (src= (src+(len)>=endaddr) ? (startaddr+((src)+(len)-(endaddr))) : ((src)+(len)))

#define CMPB_HDR_LEN    (14 + prSwdmxCmpbInfo->u4StartCodeLen)

#define IS_SLOW_FWD(s)  ((s) > 1 && (s) < 1000)

#define SWDMX_CMPB_VERIFY_NULL(ptr)                                                         \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(2, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                ASSERT(0);                                                                  \
                return; }                                                                   \
          } while (0)

#define SWDMX_CMPB_VERIFY_NULL_ERROR(ptr)                                                   \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(2, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                ASSERT(0);                                                                  \
                return FALSE; }                                                             \
          } while (0)

#define SWDMX_CMPB_VERIFY_NULL_RET(ptr, ret)                                                \
          do {                                                                              \
            if (ptr == NULL) {                                                              \
                LOG(2, "SWDMX Instance is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__); \
                ASSERT(0);                                                                  \
                return ret; }                                                               \
          } while (0)

#define IS_SPEC_AP(a)        ((a) == MM_SRC_TYPE_NETWORK_SKYPE || (a) == MM_SRC_TYPE_NETWORK_MHP)
#define IS_NO_AVSYNC_AP(a)   ((a) == MM_SRC_TYPE_NETWORK_SKYPE)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifdef CMPB_TIME_PROFILE
static HAL_TIME_T _rSwdmxStart;
static HAL_TIME_T _rSwdmxEnd;
static HAL_TIME_T _rSwdmxDif;
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsCmpbPacket(
    UCHAR u1SrcId,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm);

static SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsWithoutIdx(
    UCHAR u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsUseIdx(
    UCHAR u1SrcId,
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverCmpbKeyframe(
    UCHAR u1SrcId,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed);

static BOOL _SWDMX_CmpbInitDmx(UCHAR u1SrcId, UINT8 u1HdrType, BOOL fgEos);

static BOOL _SWDMX_CmpbGetDataFromFeeder(UCHAR u1SrcId, FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_CmpbWaitFeeder(UCHAR u1SrcId, FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_CmpbResetFeeder(UCHAR u1SrcId);

static VOID _SWDMX_CmpbVideoWakeUp(UCHAR u1SrcId);

static BOOL _SWDMX_SendCmpbEos(UCHAR u1SrcId, UINT8 u1HdrType);


BOOL _SWDMX_CmpbSetDecoderInfo(UCHAR u1SrcId,
    UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if (u1Type == eSWDMX_SET_VIDEOTYPE)
    {
        prSwdmxCmpbInfo->u4VDecType =  u4Para1;
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL _SWDMX_CmpbGetInfo(UCHAR u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    //LOG(5, "_SWDMX_CmpbGetInfo u1Type = %d\n", u1Type);

    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_CmpbGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = prSwdmxCmpbInfo->u8CurFilePos;
        return TRUE;
    }

    return FALSE;
}


BOOL _SWDMX_CmpbVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    SWDMX_INFO_T *prSwdmxInst = (SWDMX_INFO_T*)prPes->pvInstanceTag;
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.ucEsId = (UCHAR)prSwdmxInst->u1VdecId;
    if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucMpvId = VLD1; // video format all use VLD expect jpeg
    }
    else
    {
        rPesInfo.ucMpvId = VLD0; // video format all use VLD expect jpeg
    }
#if 1  // to fix video jitter problem
    rPesInfo.u4PTS = (prPes->u4Pts != 0 ? prPes->u4Pts : prSwdmxCmpbInfo->u4CurVidPts);
#else
    rPesInfo.u4PTS = prSwdmxCmpbInfo->u4CurVidPts;
#endif
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = prSwdmxCmpbInfo->u8VidOffset;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    if(!rPesInfo.fgEos)
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }
    if(prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_H264)
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
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_H263)
    {
        if (!rPesInfo.fgEos)
        {
            UINT8 uByte = 0, uPicType = 0, uCodecType = 0;
            UCHAR *puReadPtr = NULL, *puBufStart = NULL, *puBufEnd = NULL;

            puBufStart = (UCHAR *)VIRTUAL(rPesInfo.u4FifoStart);
            puBufEnd = (UCHAR *)VIRTUAL(rPesInfo.u4FifoEnd);
            puReadPtr = (UCHAR *)VIRTUAL(rPesInfo.u4VldReadPtr);

            rPesInfo.fgGop = prPes->fgGop;
            rPesInfo.rExtra.fgValid = TRUE;
            // ??? rPesInfo.rExtra.u8PTSI=prSwdmxFlvInfo->u4LastKeyPts;
            // ??? rPesInfo.rExtra.u8AudPTS=prSwdmxFlvInfo->u4AudPts;

            uByte = *puReadPtr;
            //prSwdmxFlvInfo->u2TagIdentify=(uByte&0xf)|(prSwdmxFlvInfo->u2TagIdentify&0xff00);
            uPicType = (uByte&0xf0)>>4;
            uCodecType = uByte&0xf;
            UNUSED(uCodecType);
            CMPB_RPOS_INC(puReadPtr, 1, puBufStart, puBufEnd);
            if (uPicType == 1)
            {
                rPesInfo.ucPicType = I_TYPE;
            }
            else if (uPicType == 2)
            {
                rPesInfo.ucPicType = P_TYPE;
            }
            else if (uPicType == 3)
            {
                rPesInfo.ucPicType = I_TYPE;
            }
            else
            {
                LOG(1, "_SWDMX_CmpbVideoCallback UnknowPicType:%d\n", uByte&0xf0);
                rPesInfo.ucPicType = UNKNOWN_TYPE;
            }
            rPesInfo.u4VldReadPtr = (UINT32)PHYSICAL((UINT32)puReadPtr);
        }
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
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
                rPesInfo.fgDtsValid = TRUE;
                break;
            case 1: case 3:
                rPesInfo.ucPicType  = P_TYPE;
                rPesInfo.fgDtsValid = FALSE;
                break;
            case 2:
                rPesInfo.ucPicType = B_TYPE;
                rPesInfo.fgDtsValid = FALSE;
                break;
            default:
              rPesInfo.ucPicType = UNKNOWN_TYPE;
              rPesInfo.fgDtsValid = FALSE;
              break;
            }
        }
        else if ((pucBuf[0] & 0x20) == 0x20)
        {
            rPesInfo.fgSeqHdr = TRUE;
        }
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        // should fill pic type for VDEC !!!
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            rPesInfo.ucPicType = MPV_I_TYPE;
            break;

        case PIC_TYPE_P:
            rPesInfo.ucPicType = MPV_P_TYPE;
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
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType  = I_TYPE;
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_RAW_NV12)
    {
        rPesInfo.ucPicType  = I_TYPE;
    }
    else if (prSwdmxCmpbInfo->u4VDecType == ENUM_SWDMX_VDEC_VP6)
    {
        // TODO: Add vp6 related parsing
    }
    else
    {
        LOG(0, "Unknown video type: %d\n", prSwdmxCmpbInfo->u4VDecType);
        ASSERT(0);
    }

    if((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(3, "MMCallback Addr 0!\n");
    }
    else
    {
        // push out the frame for MHP single frame decoding
        if (prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_MHP)
        {
            rPesInfo.fgNonBuffer = TRUE;
        }

        VDEC_SendEs((void*)&rPesInfo);

        // push out the frame for Skype
        if (prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_SKYPE)
        {
            // push 264 data
            VDEC_PES_INFO_T rPesInfo1;
            x_memcpy((VOID*)&rPesInfo1, (VOID*)&rPesInfo, sizeof(VDEC_PES_INFO_T));
            rPesInfo1.ucPicType = I_TYPE;
            rPesInfo1.fgSeqEnd = TRUE;
            VDEC_SendEs((void*)&rPesInfo1);
        }
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_CmpbInit(UCHAR u1SrcId, ENUM_SWDMX_FMT_T eType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    FEEDER_BUF_INFO_T *prCmpbFeederInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;
    prCmpbFeederInfo= &prSwdmxCmpbInfo->rCmpbFeederInfo;

    UNUSED(eType);

    x_memset((void*)prSwdmxCmpbInfo, 0, sizeof(SWDMX_CMPB_INFO_T));
    x_memset((void*)prCmpbFeederInfo, 0, sizeof(FEEDER_BUF_INFO_T));

    prSwdmxCmpbInfo->fgGotEosIbc = FALSE;
    prSwdmxCmpbInfo->fgBeginToPlay = TRUE;
    prSwdmxCmpbInfo->fgAudTrigVidDec = FALSE;

    prSwdmxCmpbInfo->u4CurVidPts = 0;
    prSwdmxCmpbInfo->fgIsBlock = FALSE;

    prSwdmxCmpbInfo->u8CurFilePos = 0;
    prSwdmxCmpbInfo->u8CurDmxFilePos = 0;

    prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
    prSwdmxCmpbInfo->fgFeederTimeout = FALSE;
    prSwdmxCmpbInfo->i4ReqReadTimes = 0;

    prSwdmxCmpbInfo->fgWaitFirstAudio = TRUE;
    prSwdmxCmpbInfo->fgWaitFirstVideo = TRUE;
    prSwdmxCmpbInfo->fgAdjustPTS      = FALSE;

    prSwdmxCmpbInfo->u4StartCodeLen = 4;

    return TRUE;
}


/**
*/
VOID _SWDMX_CmpbSetRange(UCHAR u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                         UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if (prRangeList == NULL)
    {
        ASSERT(0);
        LOG(1, "Demuxer Range is NULL\n");
        return;
    }

    // megaa test 20100212
    if (prRangeList->prDmxRangeInfo != NULL)
    {
        prRangeList->prDmxRangeInfo->ui8_fileoffset = 0;
        prRangeList->prDmxRangeInfo->z_range_sz = 0xFFFFFFFF;
        prSwdmxCmpbInfo->prDmxRangeInfo = prRangeList->prDmxRangeInfo;
    }

    if (prRangeList->prDmxRangeInfo != NULL)
    {
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl =
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        u1ATrack = 0;

        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < CMPB_MAX_AUDIO_TRACK)
            {
                prSwdmxCmpbInfo->prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Audio Index table = 0x%08x\n",
                    (UINT32)(prSwdmxCmpbInfo->prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                prSwdmxCmpbInfo->prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Video Index table = 0x%08x\n",
                    (UINT32)(prSwdmxCmpbInfo->prVidIdxTbl));
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                prSwdmxCmpbInfo->prVidKeyIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Key Index table = 0x%08x\n",
                    (UINT32)(prSwdmxCmpbInfo->prVidKeyIdxTbl));
            }

            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;

           //prSwdmxCmpbInfo->u8_data_offset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
           //prSwdmxCmpbInfo->u8_range_sz  = prRangeList->prDmxRangeInfo->z_range_sz;
        }

        if (prSwdmxCmpbInfo->prVidIdxTbl)
        {
            prSwdmxCmpbInfo->u4VidFps = 30;

            LOG(5, "prSwdmxCmpbInfo->u4VidFps = %ld\n", prSwdmxCmpbInfo->u4VidFps);
        }

        // Set the number of total audio tracks
        if (prSwdmxCmpbInfo->u1TotalATrack == 0)  // tmp solution for MW not setting this
            prSwdmxCmpbInfo->u1TotalATrack = u1ATrack;

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
        LOG(1, "Demuxer Range is NULL\n");
    }
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrs(
    UCHAR u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd)
{
    UINT32 u4PrsState;
    INT32 i4PlaySpeed;
    UINT64 u8SeekPos;
    BOOL fgSpeedChange;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_RET(prSwdmxInst, eSWDMX_HDR_PRS_FAIL);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if (!prCmd)
    {
        LOG(3, "prCmd is NULL %s,%d, oops....\n", __FUNCTION__, __LINE__);
        ASSERT(0);
        return eSWDMX_HDR_PRS_FAIL;
    }
    i4PlaySpeed = prCmd->i4PlaySpeed;
    u8SeekPos = prCmd->u8SeekPos;
    fgSpeedChange = prCmd->fgChange;

//BEGIN:
    if (prSwdmxCmpbInfo->fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
        if (prSwdmxCmpbInfo->fgEnAudio && prSwdmxCmpbInfo->fgEnVideo)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);

#if 1
            {
                #define _LOOP_CNT_    200
                UINT32 u4Cnt = 0;
                while (u4Cnt < _LOOP_CNT_)
                {
                    UINT32 u4Status = MPV_GetDecMode(prSwdmxInst->u1VdecId);
                    if ((u4Status == VDEC_ST_STOP) || (u4Status == VDEC_ST_IDLE))
                    {
                        x_thread_delay(10);
                        u4Cnt++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
#endif
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxCmpbInfo->fgVDecIsPause = TRUE;
            prSwdmxCmpbInfo->fgAudTrigVidDec = TRUE;

            if (IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type))
            {
                prSwdmxCmpbInfo->u4TriggerAudPts = 0;
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxCmpbInfo->u4TriggerAudPts);
            }

            if (IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type))
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxCmpbInfo->u4CurVidPts);
        }
        // audio only
        else if (prSwdmxCmpbInfo->fgEnAudio)
        {
            if (prSwdmxCmpbInfo->u1CurATrack < CMPB_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(prSwdmxInst->u1AdecId, prSwdmxCmpbInfo->u4AudPts[prSwdmxCmpbInfo->u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(prSwdmxInst->u1AdecId, 0);
            }
        }
        // video only
        else if (prSwdmxCmpbInfo->fgEnVideo)
        {
            VDP_SetPauseMm(prSwdmxInst->u1B2rId, TRUE);
            VDEC_Pause(prSwdmxInst->u1VdecId);
            prSwdmxCmpbInfo->fgVDecIsPause = TRUE;

            VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, prSwdmxCmpbInfo->u4CurVidPts);
        }

        // set stc for starting playback
        STC_StopStc(prSwdmxInst->u1StcId);
        STC_SetStcValue(prSwdmxInst->u1StcId, prSwdmxCmpbInfo->u4CurVidPts - 45000);

        prSwdmxCmpbInfo->fgBeginToPlay = FALSE;
    }

    // Reset some values for state change.
    if (prSwdmxCmpbInfo->fgChgATrack || fgSpeedChange)
    {
        if (prSwdmxCmpbInfo->fgDeliverVEos)
        {
            UINT32 u4Flag = 0;
            DMX_MM_T rDmxMMInfo;

            x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));
            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;
            switch (prSwdmxCmpbInfo->u4VDecType)
            {
            case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            case ENUM_SWDMX_VDEC_MPEG4:
            case ENUM_SWDMX_VDEC_H264:
            case ENUM_SWDMX_VDEC_MVC:
            case ENUM_SWDMX_VDEC_VC1:
            case ENUM_SWDMX_VDEC_AVS:
                LOG(7, "%s SearchStartCode = TRUE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;

            default:
                 LOG(7, "%s SearchStartCode = FALSE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            }
            if (!DMX_MM_Set(prSwdmxCmpbInfo->u1VidPid, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }

            prSwdmxCmpbInfo->fgDeliverVEos = FALSE;
        }
        if (prSwdmxCmpbInfo->fgDeliverAEos)
        {
            prSwdmxCmpbInfo->fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(prSwdmxInst->u1AdecId);
        }

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, FALSE);
#endif
        prSwdmxCmpbInfo->fgAudIsReady = FALSE;

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
    }

    u4PrsState = _SWDMX_CmpbHdrPrsWithoutIdx(u1SrcId,
                                             prRangeList,
                                             i4PlaySpeed,
                                             u8SeekPos,
                                             fgSpeedChange);

    prSwdmxCmpbInfo->i4PrevSpeed = i4PlaySpeed;

    //if (eSWDMX_HDR_PRS_SUCCEED == (SWDMX_HDR_PRS_STATE_T)u4PrsState)
    //    goto BEGIN;  // megaa 20100304: tmp solution

    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}


static BOOL _CheckCmpbPacketType(UCHAR u1SrcId, UINT8 *pu1Buf, UINT64 *pu8Pts, UINT8 *pu1Flag, UINT32 *pu4PacketSize)
{
    UINT64 u8Pts = 0;
    UINT32 u4PacketSize = 0;
    int i;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if ((4 == prSwdmxCmpbInfo->u4StartCodeLen)
    && ((pu1Buf[0] != 0x3E) || (pu1Buf[1] != 0xD1) || (pu1Buf[2] != 0xA7) || (pu1Buf[3] != 0xE4)))
    {
        prSwdmxCmpbInfo->u4StartCodeLen = 0;
    }

    *pu1Flag = *(pu1Buf + 12 + prSwdmxCmpbInfo->u4StartCodeLen);

    for (i = 0; i < 8; i++)
    {
        u8Pts = (u8Pts << 8) + (UINT64)(*(pu1Buf + 7 + prSwdmxCmpbInfo->u4StartCodeLen - i));
    }

    if (prSwdmxCmpbInfo->fgAdjustPTS && (u8Pts == 0))
    {
        u8Pts = 1;
    }

    if (u8Pts & ((UINT64)0x1) << 63)
    {
        prSwdmxCmpbInfo->fgAdjustPTS = TRUE;
        u8Pts = 0;
    }

    *pu8Pts = u8Pts;

    for (i = 0; i < 4; i++)
    {
        u4PacketSize = (u4PacketSize << 8) + (UINT32)(*(pu1Buf + 11 + prSwdmxCmpbInfo->u4StartCodeLen - i));
    }
    *pu4PacketSize = u4PacketSize;

    switch ((*pu1Flag) & 0x07)
    {
    case 1:  // video
        switch ((*pu1Flag) & 0x18)
        {
        case 0x08:  // I-frame
        case 0x10:  // B-frame
        case 0x18:  // P-frame
        case 0:     // tmp solution
            return TRUE;

        default:
            return FALSE;
        }

    case 2:  // audio
        return TRUE;

    case 3:  // subpicture
        return TRUE;
    }

    return FALSE;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsCmpbPacket(
    UCHAR u1SrcId,
    UCHAR* pucBitstream,
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize,
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm)
{
    UINT32 i = 0;
    BOOL fgFindPkt = FALSE;
    UINT32 u4OrgAvailSize = u4AvailSize;
    UINT64 u8Pts;
    UINT8 u1Flag;
    UINT32 u4PacketSize;
    UINT8 u1HdrType;
    UINT32 u4DmxAvailSize = 0, u4MoveRetryCnt;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    DMX_MM_DATA_T *prCmpbDmxMMData;
    FEEDER_BUF_INFO_T *prCmpbFeederInfo;
    SWDMX_CMPB_VERIFY_NULL_RET(prSwdmxInst, eSWDMX_HDR_PRS_FAIL);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;
    prCmpbDmxMMData = &prSwdmxCmpbInfo->rCmpbDmxMMData;
    prCmpbFeederInfo= &prSwdmxCmpbInfo->rCmpbFeederInfo;

#if 0  // debug log code
    {
        int n;
        UINT8 *pu1Ptr = (UINT8 *)pucBitstream;
        LOG(7, "        got    : ");
        for (n = 0; n < 32; n++)
        {
            LOG(7, "%02X", (UINT32)(*pu1Ptr++));
        }
        LOG(7, " len = %d\n", u4AvailSize);
    }
#endif

    while (i < u4AvailSize)
    {
        if (_CheckCmpbPacketType(u1SrcId, pucBitstream + i, &u8Pts, &u1Flag, &u4PacketSize))
        {
            fgFindPkt = TRUE;
            break;
        }
        i++;
    }

    // Didn't find the packet
    if (!fgFindPkt)
    {
        *pu4ConsumeSize = u4OrgAvailSize;
        LOG(5, "unknown packet!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return eSWDMX_HDR_PRS_FAIL;
    }

    switch (u1Flag & 0x7)
    {
    case 1:
        u1HdrType = eSWDMX_MEDIA_VIDEO;
        LOG(5, "video packet\n");
        prSwdmxCmpbInfo->u4CurVidPts = (UINT32)u8Pts;
        break;

    case 2:
        u1HdrType = eSWDMX_MEDIA_AUDIO;
        LOG(5, "audio packet\n");
        break;

    case 3:
        u1HdrType = eSWDMX_MEDIA_SUBTITLE;
        LOG(5, "subtitle packet\n");
        break;

    default:
        u1HdrType = eSWDMX_MEDIA_SKIPPED;
        LOG(5, "skipped packet\n");
        break;
    }

    prCmpbDmxMMData->u4StartAddr = (UINT32)(pucBitstream + i + CMPB_HDR_LEN);
    if (prCmpbDmxMMData->u4StartAddr >= (UINT32)(VIRTUAL(prCmpbFeederInfo->u4EndAddr)))
        ASSERT(0);

    if (prSwdmxCmpbInfo->u8RecDmxMoviPos)
    {
    }

    // Error Handling
    if (u4PacketSize > CMPB_BUFFER_SIZE)
    {
        LOG(3, "u4PacketSize > CMPB_BUFFER_SIZE Chunk size = 0x%08x\n", u4PacketSize);
        *pu4ConsumeSize = u4OrgAvailSize;
        return eSWDMX_HDR_PRS_FAIL;
    }
    else if (u4PacketSize > u4AvailSize)
    {
        LOG(3, "Chunk size is 0x%08x larger than feeder's, the size is 0x%08x\n", u4PacketSize, u4AvailSize);

        x_thread_delay(10);

        if (!fgRangeEnd)
        {
            prSwdmxCmpbInfo->fgIsBlock = TRUE;
            //prSwdmxCmpbInfo->u4BlockChunkSize = u4PacketSize;  !!!
            *pu4ConsumeSize = 0;
        }
        else
        {
            *pu4ConsumeSize = u4AvailSize;
            LOG(5, "Range End!!!\n");
        }

        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
    }
    else if (!u4PacketSize)
    {
#if 1
        LOG(0, "\n\n\n!!!!!!!!!!!!!!!!!!!!!!!!! Payload size is 0, AP problem??????\n\n\n");
#else
        ASSERT(0);  // !!! ToDo: add error handling
#endif
        *pu4ConsumeSize = i + CMPB_HDR_LEN;
        return eSWDMX_HDR_PRS_FAIL;
    }

    // Disable streams here
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        if ((i4PlaySpeed != SWDMX_PLAY_SPEED_1X) || (prSwdmxCmpbInfo->fgEnAudio == FALSE))
        {
            prSwdmxCmpbInfo->u4PullBackAudPts = (UINT32)u8Pts;  // record the PTS for later usage

            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        if (prSwdmxCmpbInfo->fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
    }

    if (u1HdrType == eSWDMX_MEDIA_SKIPPED ||
        u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        *pu4ConsumeSize = u4PacketSize + CMPB_HDR_LEN;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Get available size here (we don't have to query subtitle available size here)
    if (u1HdrType != eSWDMX_MEDIA_SUBTITLE)
    {
#if 1
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            UINT8 u1PidIdx = prSwdmxCmpbInfo->u1VidPid;
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_VIDEO, u1PidIdx);
        }
        else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
        {
            UINT8 u1PidIdx = prSwdmxCmpbInfo->u1AudPid;
            u4DmxAvailSize = DMX_MUL_GetEmptySize(prSwdmxInst->u1DmxId, DMX_PID_TYPE_ES_AUDIO, u1PidIdx);
        }
#else
        u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
#endif
        if (u4PacketSize > u4DmxAvailSize)
        {
            if (prSwdmxCmpbInfo->fgVDecIsPause)
            {
                LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 1\n");
                _SWDMX_CmpbVideoWakeUp(u1SrcId);
                prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
            }

            if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
                !prSwdmxCmpbInfo->fgEnVideo/* &&  megaa 20100621: no need to check this flag??
                prSwdmxCmpbInfo->fgAudIsReady*/)
            {
                AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
                prSwdmxCmpbInfo->fgAudIsReady = FALSE;
            }

            LOG(7, "Chunk is blocked u1HdrType = %ld, u4PacketSize = %ld, u4DmxAvailSize = %ld\n",
                u1HdrType,
                u4PacketSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            prSwdmxInst->fgFifoFull = TRUE;
            x_thread_delay(10);
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        //_SWDMX_CmpbInitDmx(u1SrcId, u1HdrType, FALSE);  megaa 20100618
    }

    prCmpbDmxMMData->u4FrameSize = u4PacketSize;

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        // !!!
#if 1
        UINT16 u2MaxQNum, u2CurQNum;
/*
        if (prSwdmxCmpbInfo->fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
*/
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);

        if (IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type))
        {
            if ((!IS_SPEC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type) && u2CurQNum > 60/*(prSwdmxCmpbInfo->u4VidFps * 3)*/)
            || ( IS_SPEC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type) && prSwdmxCmpbInfo->fgVDecIsPause))
            {
                if (prSwdmxCmpbInfo->fgVDecIsPause)
                {
                    LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 2\n");
                    _SWDMX_CmpbVideoWakeUp(u1SrcId);
                    prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
                }
                LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
                *pu4ConsumeSize = 0;
                prSwdmxInst->fgFifoFull = TRUE;
                x_thread_delay(10);
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }

        if (u2CurQNum > u2MaxQNum - 10)
        {
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);

            // megaa 20110822: patch for video only case
            if (!prSwdmxCmpbInfo->fgEnAudio && prSwdmxCmpbInfo->fgVDecIsPause)
            {
                LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 2A\n");
                _SWDMX_CmpbVideoWakeUp(u1SrcId);
                prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
            }

            *pu4ConsumeSize = 0;
            x_thread_delay(10);
            return eSWDMX_HDR_PRS_SUCCEED;
        }
        //u4Pts = prSwdmxCmpbInfo->u4CurVidPts;

        // Record every video's chunk position.
        prSwdmxCmpbInfo->u8VidOffset = prSwdmxCmpbInfo->u8CurDmxFilePos;
#endif
    }

    prCmpbDmxMMData->u4BufStart = (UINT32)(VIRTUAL(prCmpbFeederInfo->u4StartAddr));
    prCmpbDmxMMData->u4BufEnd = (UINT32)(VIRTUAL(prCmpbFeederInfo->u4EndAddr));
    prCmpbDmxMMData->u4Pts = (UINT32)u8Pts;
    prCmpbDmxMMData->u4Dts = (UINT32)u8Pts;

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        prCmpbDmxMMData->u4FrameSize = u4PacketSize;

        if (prCmpbDmxMMData->u4StartAddr < prCmpbDmxMMData->u4BufStart)
            prCmpbDmxMMData->u4BufStart -= FEEDER_RESERVED_SIZE;

        if (!IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type) && prSwdmxCmpbInfo->fgWaitFirstAudio)
        {
            prSwdmxCmpbInfo->u4TriggerAudPts = (UINT32)u8Pts;
            LOG(3, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 1st audio pts = 0x%08X(%dms)\n", (UINT32)u8Pts, (UINT32)u8Pts / 90);
            AUD_SetStartPts(prSwdmxInst->u1AdecId, (UINT32)u8Pts);

            if (prSwdmxCmpbInfo->fgEnVideo)
            {
                VDEC_SetRenderFromPts(prSwdmxInst->u1VdecId, (UINT32)u8Pts);
            }

            prSwdmxCmpbInfo->fgWaitFirstAudio = FALSE;
        }

#if 1
        u4MoveRetryCnt = 0;
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1AudPid;
        while (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, prCmpbDmxMMData) != TRUE)
        {
            LOG(0, "%s(%d): _SWDMX_MoveData fail(%d)\n", __FUNCTION__, __LINE__, u4MoveRetryCnt);
            u4MoveRetryCnt++;
            if (u4MoveRetryCnt > CMPB_DMX_MV_RETRY)
            {
                LOG(0, "%s(%d): _SWDMX_MoveData retry fail\n", __FUNCTION__, __LINE__);
                ASSERT(0);  // !!!
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
#else
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1AudPid;
        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, prCmpbDmxMMData) != TRUE)
        {
            LOG(0, "Demuxer fails to move data.\n");
            ASSERT(0);  // !!!
            *pu4ConsumeSize = 0;
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
#endif
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)  // video
    {
        prCmpbDmxMMData->u4FrameSize = u4PacketSize;

        if (prCmpbDmxMMData->u4StartAddr < prCmpbDmxMMData->u4BufStart)
            prCmpbDmxMMData->u4BufStart -= FEEDER_RESERVED_SIZE;

        if (!IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type) && prSwdmxCmpbInfo->fgWaitFirstVideo)
        {
            LOG(3, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 1st video pts = 0x%08X(%dms)\n", (UINT32)u8Pts, (UINT32)u8Pts / 90);
            VDEC_Pause(prSwdmxInst->u1VdecId);  // tmp solution to deal with unexpected MW's calling VDEC_Play
            prSwdmxCmpbInfo->fgWaitFirstVideo = FALSE;
        }
#if 1
        u4MoveRetryCnt = 0;
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1VidPid;
        while (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, prCmpbDmxMMData) != TRUE)
        {
            LOG(0, "%s(%d): _SWDMX_MoveData fail(%d)\n", __FUNCTION__, __LINE__, u4MoveRetryCnt);
            u4MoveRetryCnt++;
            if (u4MoveRetryCnt > CMPB_DMX_MV_RETRY)
            {
                LOG(0, "%s(%d): _SWDMX_MoveData retry fail\n", __FUNCTION__, __LINE__);
                ASSERT(0);  // !!!
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
#else
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1VidPid;
        if (_SWDMX_MoveData(prSwdmxInst->u1SwdmxId, prCmpbDmxMMData) != TRUE)
        {
            ASSERT(0);  // !!!
            LOG(0, "Demuxer fails to move data.\n");
            *pu4ConsumeSize = 0;
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
#endif

#if 0  // debug log code
        {
            int n;
            UINT8 *pu1Ptr = (UINT8 *)VIRTUAL(prCmpbDmxMMData->u4StartAddr);
            LOG(7, "        v-piece: ");
            for (n = 0; n < 16; n++)
            {
                LOG(7, "%02X", (UINT32)(*pu1Ptr++));
            }
            LOG(7, " len = %d\n", prCmpbDmxMMData->u4FrameSize);
        }
#endif
    }
    else
    {
        ASSERT(0);
    }

//CMPB_PKT_END:
    *pu4ConsumeSize = u4PacketSize + CMPB_HDR_LEN;

    if (prSwdmxCmpbInfo->fgIsBlock == TRUE)
    {
        prSwdmxCmpbInfo->fgIsBlock = FALSE;
    }

    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsWithoutIdx(
    UCHAR u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    BOOL fgRangeEnd = FALSE;

    UINT32 u4ReadSize = 0;
    UINT32 u4ConsumedSize = 0;
    UINT32 u4PrsState = eSWDMX_HDR_PRS_SUCCEED;

    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    FEEDER_BUF_INFO_T *prCmpbFeederInfo;
    SWDMX_CMPB_VERIFY_NULL_RET(prSwdmxInst, eSWDMX_HDR_PRS_FAIL);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;
    prCmpbFeederInfo= &prSwdmxCmpbInfo->rCmpbFeederInfo;

    if (prSwdmxCmpbInfo->fgFeederInvalid)
    {
        ASSERT(prSwdmxCmpbInfo->i4ReqReadTimes == 0);
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            prSwdmxCmpbInfo->u4FeederReqID,
            prSwdmxCmpbInfo->i4ReqReadTimes,
            prSwdmxCmpbInfo->u8CurDmxFilePos);

        for ( ; prSwdmxCmpbInfo->i4ReqReadTimes > 0;)
        {
            if (_SWDMX_CmpbWaitFeeder(u1SrcId, &rFeederReqData))
                prSwdmxCmpbInfo->i4ReqReadTimes--;
        }

        _SWDMX_CmpbResetFeeder(u1SrcId);

        prSwdmxCmpbInfo->fgIsSeek = TRUE;
        prSwdmxCmpbInfo->u4LeftBytesInFeeder = 0;
        prSwdmxCmpbInfo->u8CurFilePos = prSwdmxCmpbInfo->u8CurDmxFilePos;
        prSwdmxCmpbInfo->fgFeederInvalid = FALSE;
        prSwdmxCmpbInfo->i4ReqReadTimes = 0;
        prSwdmxCmpbInfo->u4FeederReqID = 0;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (prSwdmxCmpbInfo->fgGotEosIbc/* ||
        prSwdmxCmpbInfo->u8CurDmxFilePos >= prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset*/)
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (prSwdmxCmpbInfo->fgEnVideo && !prSwdmxCmpbInfo->fgDeliverVEos)
        {
            if (prSwdmxCmpbInfo->fgVDecIsPause)
            {
                LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 3\n");
                _SWDMX_CmpbVideoWakeUp(u1SrcId);
                prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %ld, range size = %ld, range offset = %ld\n",
                prSwdmxCmpbInfo->u8CurDmxFilePos,
                prRangeList->prDmxRangeInfo->z_range_sz,
                prRangeList->prDmxRangeInfo->ui8_fileoffset);

            LOG(5, "_SWDMX_CmpbHdrPrsWithoutIdx Deliver Video Eos\n");
            if (!_SWDMX_SendCmpbEos(u1SrcId, eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxCmpbInfo->fgDeliverVEos = TRUE;
        }

        if (prSwdmxCmpbInfo->fgEnAudio && !prSwdmxCmpbInfo->fgDeliverAEos)
        {
            LOG(5, "_SWDMX_CmpbHdrPrsWithoutIdx Deliver Audio Eos\n");
            if (!_SWDMX_SendCmpbEos(u1SrcId, eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            prSwdmxCmpbInfo->fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(prSwdmxInst->u1AdecId);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if ((UINT32)prSwdmxCmpbInfo->u8CurFilePos == 0 &&
        (UINT32)(prSwdmxCmpbInfo->u8CurFilePos >> 32) == 0 &&
        prSwdmxCmpbInfo->i4ReqReadTimes == 0)
    {
#if 1  // megaa 20100618
        if (prSwdmxCmpbInfo->fgEnVDmx == FALSE && prSwdmxCmpbInfo->fgEnVideo)
        {
            _SWDMX_CmpbInitDmx(u1SrcId, eSWDMX_MEDIA_VIDEO, FALSE);
            prSwdmxCmpbInfo->fgEnVDmx = TRUE;
        }
        if (prSwdmxCmpbInfo->fgEnADmx == FALSE && prSwdmxCmpbInfo->fgEnAudio)
        {
            _SWDMX_CmpbInitDmx(u1SrcId, eSWDMX_MEDIA_AUDIO, FALSE);
            prSwdmxCmpbInfo->fgEnADmx = TRUE;
        }
#endif
        if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prCmpbFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

#if 1
        u4ReadSize = 1;
#else
        u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz;
        u4ReadSize = cmpb_min(u4ReadSize, CMPB_MIN_READ_SIZE);
#endif
        prSwdmxCmpbInfo->u4LastAlignOffset = 0;

        prSwdmxCmpbInfo->u8CurFilePos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        prSwdmxCmpbInfo->u8CurDmxFilePos = prSwdmxCmpbInfo->u8CurFilePos;

        while (prSwdmxCmpbInfo->i4ReqReadTimes < FEEDER_DATA_Q_SIZE - 1)  // megaa 20100304: ToDo: check if the -1 is needed
        {
            if (FeederSetRequest(prSwdmxInst->eFeederSrc,
                    u4ReadSize,
#if 1
                    0,
#else
                    prSwdmxCmpbInfo->u8CurFilePos,
#endif
                    &rFeederCond,
                    prSwdmxCmpbInfo->u4FeederReqID++) == FEEDER_E_OK)
            {
#ifdef CMPB_TIME_PROFILE
                HAL_GetTime(&_rSwdmxStart);
#endif
                prSwdmxCmpbInfo->i4ReqReadTimes++;
                prSwdmxCmpbInfo->u4Req++;
            }
            else
            {
                ASSERT(0);
                break;
            }
        }

        if (!_SWDMX_CmpbGetDataFromFeeder(u1SrcId, &rFeederReqData))
        {
            //prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FAIL;
        }
#ifdef CMPB_TIME_PROFILE
        HAL_GetTime(&_rSwdmxEnd);
        HAL_GetDeltaTime(&_rSwdmxDif, &_rSwdmxStart, &_rSwdmxEnd);
        printf("Duration: %d\n", _rSwdmxDif.u4Seconds * 1000000 + _rSwdmxDif.u4Micros);
#endif
        prSwdmxCmpbInfo->i4ReqReadTimes--;
        prSwdmxCmpbInfo->u4LastAlignOffset = rFeederReqData.u4AlignOffset;
        prSwdmxCmpbInfo->u8CurFilePos += rFeederReqData.u4ReadSize - prSwdmxCmpbInfo->u4LastAlignOffset;
        prSwdmxCmpbInfo->u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxCmpbInfo->u4LastAlignOffset);
        prSwdmxCmpbInfo->u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }

    if (prSwdmxCmpbInfo->fgGotEosIbc)
    {
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if (prSwdmxCmpbInfo->u4LeftBytesInFeeder == 0)
    {
        if (!_SWDMX_CmpbGetDataFromFeeder(u1SrcId, &rFeederReqData))
        {
             //prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FAIL;
        }
#ifdef CMPB_TIME_PROFILE
        HAL_GetTime(&_rSwdmxEnd);
        HAL_GetDeltaTime(&_rSwdmxDif, &_rSwdmxStart, &_rSwdmxEnd);
        printf("Duration: %d\n", _rSwdmxDif.u4Seconds * 1000000 + _rSwdmxDif.u4Micros);
#endif
        prSwdmxCmpbInfo->i4ReqReadTimes--;
        prSwdmxCmpbInfo->u4LastAlignOffset = rFeederReqData.u4AlignOffset;
        prSwdmxCmpbInfo->u8CurFilePos += rFeederReqData.u4ReadSize - prSwdmxCmpbInfo->u4LastAlignOffset;
        prSwdmxCmpbInfo->u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + prSwdmxCmpbInfo->u4LastAlignOffset);
        prSwdmxCmpbInfo->u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);

        //return eSWDMX_HDR_PRS_FAIL;
    }

    if (prSwdmxCmpbInfo->fgGotEosIbc)
    {
        // Update read ptr to feeder
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 0);  // megaa 20100304: tmp solution (0 to distinguish push mode)
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    if ((prSwdmxCmpbInfo->u4LeftBytesInFeeder == 0)
        || (prSwdmxCmpbInfo->i4ReqReadTimes < FEEDER_DATA_Q_SIZE / 2)
        || prSwdmxCmpbInfo->fgIsBlock)
    {
#if 1
        u4ReadSize = 1;
#else
        if (prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset > prSwdmxCmpbInfo->u8CurFilePos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset -
                                   prSwdmxCmpbInfo->u8CurFilePos;
        }
        u4ReadSize = cmpb_min(u4ReadSize, CMPB_MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }
#endif

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        if ((!prSwdmxCmpbInfo->fgFeederIsReady) ||
            (prSwdmxCmpbInfo->u4LeftBytesInFeeder + u4ReadSize < CMPB_BUFFER_SIZE))
#else
        if (prSwdmxCmpbInfo->u4LeftBytesInFeeder + u4ReadSize < CMPB_BUFFER_SIZE)
#endif
        {
            while (prSwdmxCmpbInfo->i4ReqReadTimes < FEEDER_DATA_Q_SIZE - 1)  // megaa 20100304: ToDo: check if the -1 is needed
            {
                LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n",
                        prSwdmxCmpbInfo->u4LeftBytesInFeeder,
                        prSwdmxCmpbInfo->u8CurFilePos,
                        prSwdmxCmpbInfo->u4FeederReqID);

                if (FeederSetRequest(prSwdmxInst->eFeederSrc,
                        u4ReadSize,
#if 1
                        0,
#else
                        prSwdmxCmpbInfo->u8CurFilePos,
#endif
                        &rFeederCond,
                        prSwdmxCmpbInfo->u4FeederReqID++) == FEEDER_E_OK)
                {
#ifdef CMPB_TIME_PROFILE
                    HAL_GetTime(&_rSwdmxStart);
#endif
                    prSwdmxCmpbInfo->u4LastAlignOffset = 0;
                    prSwdmxCmpbInfo->i4ReqReadTimes++;
                    prSwdmxCmpbInfo->u4Req++;
                }
                else
                {
                    LOG(3, "Feeder Buffer Full\n");
                    ASSERT(0);
                    //prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
                    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prCmpbFeederInfo) !=
                        FEEDER_E_OK)
                    {
                        LOG(1, "Feeder input buffer error.\n");
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }
                    LOG(3, "Cur Fed's write ptr = 0x%08x\n", prCmpbFeederInfo->u4WriteAddr);
                    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                }
            }
        }
    }

#if defined(CC_DLNA_SUPPORT) && 0
    if (u4ReadSize != CMPB_MIN_READ_SIZE)
    {
        // DLNA
        FeederSetInfo(prSwdmxInst->eFeederSrc, FEEDER_SWDMX_EOS, TRUE);
    }

    // DLNA
    if (!prSwdmxCmpbInfo->fgFeederIsReady && FeederIsReady(prSwdmxInst->eFeederSrc))
    /*if ((!prSwdmxCmpbInfo->fgFeederIsReady) &&
        ((prSwdmxCmpbInfo->u4LeftBytesInFeeder >= CMPB_PREBUFFER_SIZE) || (prSwdmxCmpbInfo->u4LeftBytesInFeeder >= prRangeList->prDmxRangeInfo->z_range_sz)))*/
    {
        prSwdmxCmpbInfo->fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt;
    HAL_GetTime(&rTimeRstS);
#endif

#if defined(CC_DLNA_SUPPORT) && 0
    if ((prSwdmxCmpbInfo->u4LeftBytesInFeeder > 0) && prSwdmxCmpbInfo->fgFeederIsReady)
#else
    if (prSwdmxCmpbInfo->u4LeftBytesInFeeder)
#endif
    {
        if (prSwdmxCmpbInfo->fgSpeedChange && i4PlaySpeed == 1000)
        {
            prSwdmxCmpbInfo->fgSpeedChange = FALSE;
        }

        // Start to parse data
        u4PrsState = _SWDMX_PrsCmpbPacket(u1SrcId,
            (UCHAR*)(prSwdmxCmpbInfo->u4CurDmxFeederRPtr),
            prRangeList,
            prSwdmxCmpbInfo->u4LeftBytesInFeeder,
            &u4ConsumedSize,
            i4PlaySpeed,
            fgRangeEnd,
            FALSE);
    }

#ifdef SWDMX_TIME_PROFILE
    HAL_GetTime(&rTimeRstE);
    HAL_GetDeltaTime(&rTimeRstDt, &rTimeRstS, &rTimeRstE);
    LOG(0, "Swdmx parsing and deliver time = (%ld) \n", rTimeRstDt.u4Micros);
#endif

    switch (u4PrsState)
    {
    case eSWDMX_HDR_PRS_FAIL:
        LOG(0, "No CMPB chunks was discovered!!!! No start code?\n");
        ASSERT(0);
        return eSWDMX_HDR_PRS_FAIL;
        //break;  mark off to shut "unreachable break" warning

    default:
        break;
    }

    if (prSwdmxCmpbInfo->u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        prSwdmxCmpbInfo->u8CurDmxFilePos += u4ConsumedSize;
        prSwdmxCmpbInfo->u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        prSwdmxCmpbInfo->u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n",
            u4ConsumedSize, prSwdmxCmpbInfo->u4LeftBytesInFeeder);
    }

    if ((prSwdmxCmpbInfo->u4CurDmxFeederRPtr + u4ConsumedSize) >=
        VIRTUAL(prCmpbFeederInfo->u4EndAddr))
    {
        prSwdmxCmpbInfo->u4CurDmxFeederRPtr =  VIRTUAL(prCmpbFeederInfo->u4StartAddr) +
            ((prSwdmxCmpbInfo->u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(prCmpbFeederInfo->u4EndAddr));
    }
    else
    {
        prSwdmxCmpbInfo->u4CurDmxFeederRPtr += u4ConsumedSize;
    }

    if (u4ConsumedSize != 0)
    {
        // Update read ptr to feeder
        FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, 0);  // megaa 20100304: tmp solution (0 to distinguish push mode)
        prSwdmxCmpbInfo->u4Con++;
    }

    LOG(9, "prSwdmxCmpbInfo->u4CurDmxMoviPos = %ld.\n", prSwdmxCmpbInfo->u8CurDmxFilePos);

    return eSWDMX_HDR_PRS_SUCCEED;
}


/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsUseIdx(
        UCHAR u1SrcId, INT32 i4Speed, SWDMX_RANGE_LIST_T* prRangeList)
{
    return eSWDMX_HDR_PRS_SUCCEED;
}


/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverCmpbKeyframe(
    UCHAR u1SrcId,
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl,
    UINT32* pu4CurIdx,
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    return eSWDMX_HDR_PRS_SUCCEED;
}


static BOOL _SWDMX_CmpbResetFeeder(UCHAR u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    FEEDER_BUF_INFO_T *prCmpbFeederInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;
    prCmpbFeederInfo= &prSwdmxCmpbInfo->rCmpbFeederInfo;

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prCmpbFeederInfo) !=
        FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_CmpbResetFeeder Cur Fed's WPtr = 0x%08x\n", prCmpbFeederInfo->u4WriteAddr);
    prSwdmxCmpbInfo->u4CurDmxFeederRPtr = prCmpbFeederInfo->u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(prSwdmxInst->eFeederSrc, VIRTUAL(prSwdmxCmpbInfo->u4CurDmxFeederRPtr));
    return TRUE;
}


BOOL _SWDMX_CmpbSetStrmID(UCHAR u1SrcId, UINT8 u1HdrType, UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    UNUSED(u4StrmSubID);
    switch(u1HdrType)
    {
    case eSWDMX_STRM_TYPE_VID:
        prSwdmxCmpbInfo->u4VStrmID = u4StrmID;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        prSwdmxCmpbInfo->u4AStrmID = u4StrmID;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_CmpbEnableStrm(UCHAR u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    switch (u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            prSwdmxCmpbInfo->fgEnVideo = TRUE;
#if 0  // megaa 20100618: moved to first time of header parse to avoid MW sequence problem
            if (prSwdmxCmpbInfo->fgEnVDmx == FALSE)
            {
                _SWDMX_CmpbInitDmx(u1SrcId, eSWDMX_MEDIA_VIDEO, FALSE);
                prSwdmxCmpbInfo->fgEnVDmx = TRUE;
            }
#endif
            break;
        case eSWDMX_STRM_TYPE_AUD:
#if 0  // megaa 20100618: moved to first time of header parse to avoid MW sequence problem
            if (prSwdmxCmpbInfo->fgEnADmx == FALSE)
            {
                _SWDMX_CmpbInitDmx(u1SrcId, eSWDMX_MEDIA_AUDIO, FALSE);
                prSwdmxCmpbInfo->fgEnADmx = TRUE;
            }
#endif
            if ((prSwdmxCmpbInfo->u4PrevAStrmID != 0xFFFFFFFF) &&
                (prSwdmxCmpbInfo->u4AStrmID != prSwdmxCmpbInfo->u4PrevAStrmID))
            {
                prSwdmxCmpbInfo->fgChgATrack = TRUE;
            }
            prSwdmxCmpbInfo->u4PrevAStrmID = prSwdmxCmpbInfo->u4AStrmID;
            prSwdmxCmpbInfo->fgEnAudio = TRUE;
            //prSwdmxCmpbInfo->u1TotalATrack = 1;  // tmp solution for MW not setting this
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_CmpbDisableStrm(UCHAR u1SrcId, UINT8 u1StrmType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(prSwdmxCmpbInfo->fgVDecIsPause)
            {
                LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 4\n");
                _SWDMX_CmpbVideoWakeUp(u1SrcId);
                prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
            }
            prSwdmxCmpbInfo->fgEnVideo = FALSE;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            prSwdmxCmpbInfo->fgEnAudio = FALSE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_CmpbInitDmx(UCHAR u1SrcId, UINT8 u1HdrType, BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0;
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

#if 1  // v2
        DMX_SetToDecoder(TRUE);
#endif
        if (!prSwdmxCmpbInfo->fgEnVDmx)
        {
#if 1  // v2
            UNUSED(prFbmPool);
            prSwdmxCmpbInfo->u1VidPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_INSTANCE_TAG |
                              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
#else
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
#endif

            rDmxMMInfo.fgAllocBuf = FALSE;
#if 1  // v2
            _SWDMX_GetVideoFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);
            rDmxMMInfo.u1ChannelId = prSwdmxInst->u1VdecId;
            prSwdmxCmpbInfo->u4VFifoSa = VIRTUAL(rDmxMMInfo.u4BufAddr);
            prSwdmxCmpbInfo->u4VFifoEa = VIRTUAL(rDmxMMInfo.u4BufAddr) + rDmxMMInfo.u4BufSize;
#else
            rDmxMMInfo.u4BufAddr = VIRTUAL(prFbmPool->u4Addr);
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
            prSwdmxCmpbInfo->u4VFifoSa = VIRTUAL(prFbmPool->u4Addr);
            prSwdmxCmpbInfo->u4VFifoEa = VIRTUAL(prFbmPool->u4Addr + prFbmPool->u4Size);
#endif
            u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;
            switch (prSwdmxCmpbInfo->u4VDecType)
            {
            case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            case ENUM_SWDMX_VDEC_MPEG4:
            case ENUM_SWDMX_VDEC_H264:
            case ENUM_SWDMX_VDEC_MVC:
            case ENUM_SWDMX_VDEC_VC1:
            case ENUM_SWDMX_VDEC_AVS:
                LOG(7, "%s SearchStartCode = TRUE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;

            default:
                LOG(7, "%s SearchStartCode = FALSE\n", __FUNCTION__);
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            }
            fgRet = DMX_MM_Set(prSwdmxCmpbInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxCmpbInfo->u1VidPid, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
#if 0
        UINT32 au4BufStart[2], au4BufEnd[2];
#endif
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

#if 1  // v2
        DMX_SetToDecoder(TRUE);
#endif
        if (!prSwdmxCmpbInfo->fgEnADmx)
        {
#if 1  // v2
            prSwdmxCmpbInfo->u1AudPid =
                DMX_MUL_GetAvailablePidx(prSwdmxInst->u1DmxId);

            _SWDMX_GetAudioFifo(prSwdmxInst->u1SwdmxId, &rDmxMMInfo.u4BufAddr, &rDmxMMInfo.u4BufSize);

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_INSTANCE_TAG
                              | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE);

            rDmxMMInfo.pvInstanceTag = (void*)prSwdmxInst;
            rDmxMMInfo.fgSearchStartCode = FALSE;
#else
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
#ifdef AUD_OLDFIFO_INTF
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
#else
            if (AUD_GetAudFifo(prSwdmxInst->u1AdecId, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
            {
                LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                return FALSE;
            }
#endif

            rDmxMMInfo.fgAllocBuf = FALSE;
#if 0
            rDmxMMInfo.u4BufAddr = au4BufStart[0];
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];
#endif
            fgRet = DMX_MM_Set(prSwdmxCmpbInfo->u1AudPid, u4Flag, &rDmxMMInfo);
            prSwdmxCmpbInfo->u4AFifoSa = VIRTUAL(rDmxMMInfo.u4BufAddr);
            prSwdmxCmpbInfo->u4AFifoSz = rDmxMMInfo.u4BufSize;
        }
        else if (fgEos)
        {
            // necessary?
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
            fgRet = DMX_MM_Set(prSwdmxCmpbInfo->u1AudPid, u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendCmpbEos(UCHAR u1SrcId, UINT8 u1HdrType)
{
    BOOL fgRet;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    DMX_MM_DATA_T *prCmpbDmxMMData;
    FEEDER_BUF_INFO_T *prCmpbFeederInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;
    prCmpbDmxMMData = &prSwdmxCmpbInfo->rCmpbDmxMMData;
    prCmpbFeederInfo= &prSwdmxCmpbInfo->rCmpbFeederInfo;

    LOG(5, "SWDMX Send CMPB Eos Hdr Type = %d\n", u1HdrType);

    if (FeederGetBufferInfo(prSwdmxInst->eFeederSrc, prCmpbFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    x_memset(prCmpbDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(prSwdmxInst->u1VdecId, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (prSwdmxCmpbInfo->u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            prSwdmxInst->fgFifoFull = TRUE;
            x_thread_delay(1);
            return FALSE;
        }
#if 1  // v2
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1VidPid;
#endif
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
#if 1  // v2
        prCmpbDmxMMData->u1Idx = prSwdmxCmpbInfo->u1AudPid;
#endif
    }

    if (_SWDMX_CmpbInitDmx(u1SrcId, u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    prCmpbDmxMMData->u4BufStart = prCmpbFeederInfo->u4StartAddr;
    prCmpbDmxMMData->u4BufEnd = prCmpbFeederInfo->u4EndAddr;
    prCmpbDmxMMData->u4StartAddr = prCmpbFeederInfo->u4StartAddr;
    prCmpbDmxMMData->u4FrameSize = 4;
    prCmpbDmxMMData->fgEOS = TRUE;

#if 1
    fgRet = _SWDMX_MoveData(prSwdmxInst->u1SwdmxId, prCmpbDmxMMData);
#else
    fgRet = DMX_MM_MoveData(u1HdrType, prCmpbDmxMMData, 500);
#endif
    ASSERT(fgRet);

    prCmpbDmxMMData->fgEOS = FALSE;

    return fgRet;
}


/**
*/
BOOL _SWDMX_CmpbStop(UCHAR u1SrcId)
{
    DMX_MM_T rPid;
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    rPid.fgEnable = FALSE;

    if (prSwdmxCmpbInfo->fgEnVDmx)
    {
#if 1  // v2
        VERIFY(DMX_MM_Set(prSwdmxCmpbInfo->u1VidPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxCmpbInfo->u1VidPid);
        DMX_MUL_FreePidx(prSwdmxCmpbInfo->u1VidPid);
#endif
        prSwdmxCmpbInfo->fgEnVDmx = FALSE;
    }

    if (prSwdmxCmpbInfo->fgEnADmx)
    {
#if 1  // v2
        VERIFY(DMX_MM_Set(prSwdmxCmpbInfo->u1AudPid, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(prSwdmxCmpbInfo->u1AudPid);
        DMX_MUL_FreePidx(prSwdmxCmpbInfo->u1AudPid);
#endif
        prSwdmxCmpbInfo->fgEnADmx = FALSE;
    }

    return TRUE;
}


static BOOL _SWDMX_CmpbGetDataFromFeeder(UCHAR u1SrcId, FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_CmpbGetDataFromFeeder got Feeder ACK failed!!!\n");
        prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
        return FALSE;
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "_SWDMX_CmpbGetDataFromFeeder timeout\n");
        prSwdmxCmpbInfo->fgFeederTimeout = TRUE;
        return FALSE;
    }

#if 0
    if ((prFeederData->u4Id + 1) != prSwdmxCmpbInfo->u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        prSwdmxCmpbInfo->fgFeederInvalid = TRUE;
        return FALSE;
    }
#endif

    if (prFeederData->fgEof)
    {
        prSwdmxCmpbInfo->fgGotEosIbc = TRUE;
    }

    prSwdmxCmpbInfo->fgFeederTimeout = FALSE;
    prSwdmxCmpbInfo->u4Ack++;

    return TRUE;
}


// tmp solution
static BOOL _SWDMX_CmpbWaitFeeder(UCHAR u1SrcId, FEEDER_REQ_DATA_T *prFeederData)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);

    while (1)
    {
        if (!_SWDMX_ReceiveFeederAck(prSwdmxInst->u1SwdmxId, (VOID*)prFeederData))
        {
            LOG(3, "_SWDMX_CmpbWaitFeeder got Feeder ACK failed!!!\n");
            ASSERT(0);
            return FALSE;
        }
        if (prFeederData->eDataType != FEEDER_SOURCE_INVALID)
        {
            break;
        }
    }

    return TRUE;
}


BOOL _SWDMX_CmpbAudioCb(UCHAR u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL_ERROR(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if(prSwdmxCmpbInfo->fgEnVideo)
        {
            if(prSwdmxCmpbInfo->fgAudTrigVidDec)
            {
                if (IS_NO_AVSYNC_AP(prSwdmxCmpbInfo->prDmxRangeInfo->e_mm_src_type))
                {
                    _SWDMX_CmpbVideoWakeUp(u1SrcId);
                    LOG(1, "AudCb before feeding data!\n");
                    AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);  // for EOS, aud will not notify EOS if Aout Disable
                }
                else
                {
                    VDP_TriggerAudReceive(prSwdmxCmpbInfo->u4TriggerAudPts);
                    if (prSwdmxCmpbInfo->fgVDecIsPause)
                    {
                        LOG(3, "++++++++++++++++++++++++++++++++++++++ wake up 5\n");
                        _SWDMX_CmpbVideoWakeUp(u1SrcId);
                        prSwdmxCmpbInfo->fgVDecIsPause = FALSE;
                    }
                }
                prSwdmxCmpbInfo->fgAudTrigVidDec = FALSE;
            }
        }
        else
        {
            STC_StartStc(prSwdmxInst->u1StcId);
            AUD_MMAoutEnable(prSwdmxInst->u1AdecId, TRUE);
        }
    }
    else
    {
        LOG(1, "AudCb Type Unknow %d\n", (UINT32)eAudioNotifyType);
    }
    return TRUE;
}


static VOID _SWDMX_CmpbVideoWakeUp(UCHAR u1SrcId)
{
    SWDMX_INFO_T* prSwdmxInst = _SWDMX_GetInst(u1SrcId);
    SWDMX_CMPB_INFO_T *prSwdmxCmpbInfo;
    SWDMX_CMPB_VERIFY_NULL(prSwdmxInst);
    prSwdmxCmpbInfo = &prSwdmxInst->rFmtInfo.rSwdmxCmpbInfo;

    LOG(5, "_SWDMX_CmpbVideoWakeUp\n");
    VDP_SetPauseMm(prSwdmxInst->u1B2rId, FALSE);
    switch (prSwdmxCmpbInfo->u4VDecType)
    {
    default:
        LOG(0, "_SWDMX_CmpbVideoWakeUp Unknown Video Type (%d)\n",
            prSwdmxCmpbInfo->u4VDecType);
        ASSERT(0);
        break;
    case ENUM_SWDMX_VDEC_H264:
        LOG(5, "it is H264 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_H264);
        break;
    case ENUM_SWDMX_VDEC_MJPEG:
        LOG(5, "it is MJPEG !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MJPEG);
        break;
    case ENUM_SWDMX_VDEC_RAW_NV12:
        LOG(5, "it is NV12!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_RAW);
        break;
    case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
        LOG(5, "it is MPEG1/2 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MPV);
        break;
    case ENUM_SWDMX_VDEC_MPEG4:
        LOG(5, "it is MPEG4 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
        break;
    case ENUM_SWDMX_VDEC_H263:
        LOG(5, "it is H263 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_MP4);
        break;
    case ENUM_SWDMX_VDEC_VP6:
        LOG(5, "it is VP6 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_VP6);
        break;
    case ENUM_SWDMX_VDEC_VC1:
        LOG(5, "it is VC1 !!!!!!!!!!!!!!!!!!!!\n");
        VDEC_Play(prSwdmxInst->u1VdecId, VDEC_FMT_WMV);
        break;
    }
}


