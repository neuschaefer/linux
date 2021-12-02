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
//#define CMPB_TIME_PROFILE

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)

#define abs(x)                  (((x) >= 0) ? (x) : -(x))


#define IS_SLOW_FWD(s)  ((s) > 1 && (s) < 1000)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CMPB_TIME_PROFILE
static HAL_TIME_T _rSwdmxStart;
static HAL_TIME_T _rSwdmxEnd;
static HAL_TIME_T _rSwdmxDif;
#endif

static SWDMX_CMPB_INFO_T _rSwdmxCmpbInfo;

static FEEDER_BUF_INFO_T _rCmpbFeederInfo;

static UINT32 _u4LastAlignOffset;

static DMX_MM_DATA_T _rCmpbDmxMMData;


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static SWDMX_HDR_PRS_STATE_T _SWDMX_PrsCmpbPacket(
    UCHAR* pucBitstream, 
    SWDMX_RANGE_LIST_T* prRangeList,
    UINT32 u4AvailSize,
    UINT32* pu4ConsumeSize, 
    INT32 i4PlaySpeed,
    BOOL fgRangeEnd,
    BOOL fgOneVideoFrm);

static SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsWithoutIdx(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsUseIdx(
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverCmpbKeyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed);

static BOOL _SWDMX_CmpbInitDmx(UINT8 u1HdrType, BOOL fgEos);

static BOOL _SWDMX_CmpbGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_CmpbWaitFeeder(FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_CmpbResetFeeder(VOID);

static VOID _SWDMX_CmpbVideoWakeUp(VOID);

static BOOL _SWDMX_SendCmpbEos(UINT8 u1HdrType);


BOOL _SWDMX_CmpbSetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{
    if (u1Type == eSWDMX_SET_VIDEOTYPE) 
    {
        _rSwdmxCmpbInfo.u4VDecType =  u4Para1;
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

BOOL _SWDMX_CmpbGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    
    //LOG(5, "_SWDMX_CmpbGetInfo u1Type = %d\n", u1Type);

    if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_CmpbGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = _rSwdmxCmpbInfo.u8CurFilePos;
        return TRUE;
    }
        
    return FALSE;
}


#if 0
static VOID vHandleCmpbRingBuf(VOID)
{
    UINT32 u4AddrDelta;
    
    if (_rSwdmxCmpbInfo.u4CurDmxFeederRPtr + _rSwdmxCmpbInfo.u4LeftBytesInFeeder > _rCmpbFeederInfo.u4EndAddr)
    {
        u4AddrDelta = _rCmpbFeederInfo.u4EndAddr - _rSwdmxCmpbInfo.u4CurDmxFeederRPtr;
        if (u4AddrDelta <= FEEDER_RESERVED_SIZE)
        {
            x_memcpy((VOID*)(_rCmpbFeederInfo.u4StartAddr - u4AddrDelta), (VOID*)(_rSwdmxCmpbInfo.u4CurDmxFeederRPtr), u4AddrDelta);
            _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = _rCmpbFeederInfo.u4StartAddr - u4AddrDelta;
            //printf("Cmpb ring buffer copied!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
    }
}
#endif


static BOOL _CmpbVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    rPesInfo.u4PTS = _rSwdmxCmpbInfo.u4CurVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgEos = prPes->fgEOS;
    rPesInfo.u8Offset = _rSwdmxCmpbInfo.u8VidOffset;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif
    
    if(!rPesInfo.fgEos)
    {
        ASSERT((rPesInfo.u4VldReadPtr < rPesInfo.u4FifoEnd) &&
            (rPesInfo.u4VldReadPtr >= rPesInfo.u4FifoStart));
    }
    if(_rSwdmxCmpbInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
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
#if 0  // !!!
        //calculate PTS by decoder if CodecID is V_MS/VFW/FOURCC avi type
        if(_rSwdmxCmpbInfo.u4MainVidHeaderPtr == 0)
        {
            if(IS_IDR_NALU(rPesInfo.ucPicType))
           {
                rPesInfo.fgDtsValid = TRUE;
            }
            else
            {
                rPesInfo.fgDtsValid = FALSE;
            }
        }
#endif
    }

    if((prPes->u4FrameAddr == 0) && (!prPes->fgEOS))
    {
        LOG(3, "MMCallback Addr 0!\n");
    }
    else
    {    
        VDEC_SendEs((void*)&rPesInfo);    
    }
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_CmpbInit(ENUM_SWDMX_FMT_T eType)
{
    DMX_DECODER_CALLBACKS_T rCallback;
    
    UNUSED(eType);

    x_memset((void*)&_rSwdmxCmpbInfo, 0, sizeof(SWDMX_CMPB_INFO_T));
    x_memset((void*)&_rCmpbFeederInfo, 0, sizeof(FEEDER_BUF_INFO_T));
    
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&_rCmpbFeederInfo);

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _CmpbVideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;
    
    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);

    _rSwdmxCmpbInfo.fgBeginToPlay = TRUE;
    _rSwdmxCmpbInfo.fgAudTrigVidDec = FALSE;

    _rSwdmxCmpbInfo.u4CurVidPts = 0;
    _rSwdmxCmpbInfo.fgIsBlock = FALSE;
    
    _rSwdmxCmpbInfo.u8CurFilePos = 0;
    _rSwdmxCmpbInfo.u8CurDmxFilePos = 0;
    
    _rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
    _rSwdmxCmpbInfo.fgFeederTimeout = FALSE;
    _rSwdmxCmpbInfo.i4ReqReadTimes = 0;

    return TRUE;
}


/**
*/
VOID _SWDMX_CmpbSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID, 
                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    
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
        _rSwdmxCmpbInfo.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
    }
    
    if (prRangeList->prDmxRangeInfo != NULL)
    {
        u1ATrack = 0;
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = 
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < CMPB_MAX_AUDIO_TRACK)
            {
                _rSwdmxCmpbInfo.prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Audio Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxCmpbInfo.prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                _rSwdmxCmpbInfo.prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Video Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxCmpbInfo.prVidIdxTbl));
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                _rSwdmxCmpbInfo.prVidKeyIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_CmpbSetRange Key Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxCmpbInfo.prVidKeyIdxTbl));
            }
            
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            
           _rSwdmxCmpbInfo.ui4_data_offset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
           _rSwdmxCmpbInfo.u8_range_sz  = prRangeList->prDmxRangeInfo->z_range_sz;
        }

        if (_rSwdmxCmpbInfo.prVidIdxTbl)
        {
            _rSwdmxCmpbInfo.u4VidFps = 30;

            LOG(5, "_rSwdmxCmpbInfo.u4VidFps = %ld\n", _rSwdmxCmpbInfo.u4VidFps);
        }

        // Set the number of total audio tracks
        if (_rSwdmxCmpbInfo.u1TotalATrack == 0)  // tmp solution for MW not setting this
            _rSwdmxCmpbInfo.u1TotalATrack = u1ATrack;
        
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
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange
)
{
    UINT32 u4PrsState;
    
//BEGIN:
    if (_rSwdmxCmpbInfo.fgBeginToPlay &&
        i4PlaySpeed == 1000)
    {
        if (_rSwdmxCmpbInfo.fgEnAudio && _rSwdmxCmpbInfo.fgEnVideo)
        {
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxCmpbInfo.fgVDecIsPause = TRUE;
            _rSwdmxCmpbInfo.fgAudTrigVidDec = TRUE;

            if (_rSwdmxCmpbInfo.u1CurATrack < CMPB_MAX_AUDIO_TRACK)
            {
#if 1
                _rSwdmxCmpbInfo.u4TriggerAudPts = 0;
                AUD_SetStartPts(0, _rSwdmxCmpbInfo.u4TriggerAudPts);
#else
                AUD_SetStartPts(0, _rSwdmxCmpbInfo.u4AudPts[_rSwdmxCmpbInfo.u1CurATrack]);
                _rSwdmxCmpbInfo.u4TriggerAudPts = 
                    _rSwdmxCmpbInfo.u4AudPts[_rSwdmxCmpbInfo.u1CurATrack];
#endif
            }

            VDEC_SetRenderFromPts(ES0, _rSwdmxCmpbInfo.u4CurVidPts);
        }
        // audio only
        else if (_rSwdmxCmpbInfo.fgEnAudio)
        {
            if (_rSwdmxCmpbInfo.u1CurATrack < CMPB_MAX_AUDIO_TRACK)
            {
                AUD_SetStartPts(0, _rSwdmxCmpbInfo.u4AudPts[_rSwdmxCmpbInfo.u1CurATrack]);
            }
            else
            {
                LOG(1, "Out of Audio Track Number\n");
                AUD_SetStartPts(0, 0);
            }
        }
        // video only
        else if (_rSwdmxCmpbInfo.fgEnVideo)
        {
            VDP_SetPauseMm(0, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxCmpbInfo.fgVDecIsPause = TRUE;

            VDEC_SetRenderFromPts(ES0, _rSwdmxCmpbInfo.u4CurVidPts);
        }

        // set stc for starting playback
        STC_StopStc();
        STC_SetStcValue(_rSwdmxCmpbInfo.u4CurVidPts - 45000);

        _rSwdmxCmpbInfo.fgBeginToPlay = FALSE;
    }     

    // Reset some values for state change.
    if (_rSwdmxCmpbInfo.fgChgATrack ||
        fgSpeedChange)
    {
        if (_rSwdmxCmpbInfo.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxCmpbInfo.u4VDecType)
            {
                case ENUM_SWDMX_VDEC_H264:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
                default:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
            }
            if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }
            
            _rSwdmxCmpbInfo.fgDeliverVEos = FALSE;
        }
        if (_rSwdmxCmpbInfo.fgDeliverAEos)
        {
            _rSwdmxCmpbInfo.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(0);
        }
                 
#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        _rSwdmxCmpbInfo.fgAudIsReady = FALSE;
       
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    if (fgSpeedChange)
    {
    }
    
    u4PrsState = _SWDMX_CmpbHdrPrsWithoutIdx(prRangeList,
                                             i4PlaySpeed,
                                             u8SeekPos,
                                             fgSpeedChange);

    _rSwdmxCmpbInfo.i4PrevSpeed = i4PlaySpeed;
    
    //if (eSWDMX_HDR_PRS_SUCCEED == (SWDMX_HDR_PRS_STATE_T)u4PrsState)
    //    goto BEGIN;  // megaa 20100304: tmp solution
        
    return (SWDMX_HDR_PRS_STATE_T)u4PrsState;
}


static BOOL _CheckCmpbPacketType(UINT8 *pu1Buf, UINT64 *pu8Pts, UINT8 *pu1Flag, UINT32 *pu4PacketSize)
{
    UINT64 u8Pts = 0;
    UINT32 u4PacketSize = 0;
    int i;
    
    *pu1Flag = *(pu1Buf + 12);
    
    for (i = 0; i < 8; i++)
    {
        u8Pts = (u8Pts << 8) + (UINT64)(*(pu1Buf + 7 - i));
    }
    *pu8Pts = u8Pts;
    
    for (i = 0; i < 4; i++)
    {
        u4PacketSize = (u4PacketSize << 8) + (UINT32)(*(pu1Buf + 11 - i));
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
    UINT8 u1TrackIdx;
    UINT8 u1HdrType;
    UINT32 u4DmxAvailSize;
    
    while (i < u4AvailSize)
    {
        if (_CheckCmpbPacketType(pucBitstream + i, &u8Pts, &u1Flag, &u4PacketSize))
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
        return eSWDMX_HDR_PRS_FAIL;
    }

    switch (u1Flag & 0x7)
    {
    case 1:
        u1HdrType = eSWDMX_MEDIA_VIDEO;
        _rSwdmxCmpbInfo.u4CurVidPts = (UINT32)u8Pts;
        break;

    case 2:
        u1HdrType = eSWDMX_MEDIA_AUDIO;
        break;

    case 3:
        u1HdrType = eSWDMX_MEDIA_SUBTITLE;
        break;

    default:
        u1HdrType = eSWDMX_MEDIA_SKIPPED;
        break;
    }

    _rCmpbDmxMMData.u4StartAddr = (UINT32)(pucBitstream + i + 14);
    if (_rCmpbDmxMMData.u4StartAddr >= (UINT32)(VIRTUAL(_rCmpbFeederInfo.u4EndAddr)))
        ASSERT(0);
    
    if (_rSwdmxCmpbInfo.u8RecDmxMoviPos)
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
            _rSwdmxCmpbInfo.fgIsBlock = TRUE;
            //_rSwdmxCmpbInfo.u4BlockChunkSize = u4PacketSize;  !!!
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
        ASSERT(0);  // !!! ToDo: add error handling
        *pu4ConsumeSize = i;
        return eSWDMX_HDR_PRS_FAIL;
    }

    // Disable streams here
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        u1TrackIdx = _rSwdmxCmpbInfo.u1CurATrack;
        if (i4PlaySpeed != SWDMX_PLAY_SPEED_1X)
        {
            _rSwdmxCmpbInfo.u4PullBackAudPts = (UINT32)u8Pts;  // record the PTS for later usage

            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
#if 0  // !!!
        else if (_rSwdmxCmpbInfo.u4ModeChangPts != 0xFFFFFFFF)
        {
            if (u1TrackIdx < CMPB_MAX_AUDIO_TRACK)
            {
                if (u4Pts >= _rSwdmxCmpbInfo.u4ModeChangPts)
                {
                    LOG(5, "Audio Pts (0x%08x) > Mode ChangePts (0x%08x)\n", 
                        u4Pts, 
                        _rSwdmxCmpbInfo.u4ModeChangPts);
                    //VDP_TriggerAudReceive(u4Pts);
                    _rSwdmxCmpbInfo.u4TriggerAudPts = u4Pts;
                    AUD_SetStartPts(0, u4Pts);
                    _rSwdmxCmpbInfo.u4ModeChangPts = 0xFFFFFFFF;
                    _rSwdmxCmpbInfo.u4ChangeMode = 0;
                }
                else
                {
                    u1HdrType = eSWDMX_MEDIA_SKIPPED;
                }
            }
        }
        else if(((UINT32)(_rSwdmxCmpbInfo.u8ASeekPts>>32) != 0xFFFFFFFF) || 
                ((UINT32)(_rSwdmxCmpbInfo.u8ASeekPts) != 0xFFFFFFFF))
        {
            if((u1TrackIdx < CMPB_MAX_AUDIO_TRACK) &&
               (u4Pts < (UINT32)_rSwdmxCmpbInfo.u8ASeekPts))
            {
                u1HdrType = eSWDMX_MEDIA_SKIPPED;
            }
        }
#endif
    }
    else if (u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
    }

    if (u1HdrType == eSWDMX_MEDIA_SKIPPED ||
        u1HdrType == eSWDMX_MEDIA_SKIPPED_AUDIO)
    {
        *pu4ConsumeSize = u4PacketSize + 14;
        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Get available size here (we don't have to query subtitle available size here)
    if (u1HdrType != eSWDMX_MEDIA_SUBTITLE)
    {
        u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);
        if (u4PacketSize > u4DmxAvailSize)
        {    
            if (_rSwdmxCmpbInfo.fgVDecIsPause)
            {
                _SWDMX_CmpbVideoWakeUp();  
                _rSwdmxCmpbInfo.fgVDecIsPause = FALSE;
            }
            
            if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
                !_rSwdmxCmpbInfo.fgEnVideo &&
                _rSwdmxCmpbInfo.fgAudIsReady)
            {
                AUD_MMAoutEnable(0, TRUE);
                _rSwdmxCmpbInfo.fgAudIsReady = FALSE;
            }
            
            LOG(7, "Chunk is blocked u1HdrType = %ld, u4PacketSize = %ld, u4DmxAvailSize = %ld\n", 
                u1HdrType, 
                u4PacketSize,
                u4DmxAvailSize);
            *pu4ConsumeSize = 0;
            x_thread_delay(10);
            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
        }

        _SWDMX_CmpbInitDmx(u1HdrType, FALSE);
    }

    _rCmpbDmxMMData.u4FrameSize = u4PacketSize;

    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        u1TrackIdx = _rSwdmxCmpbInfo.u1CurATrack;

        if (u1TrackIdx < CMPB_MAX_AUDIO_TRACK)
        {
            //UINT32 u4TimeSeekAudPts = (UINT32)u8Pts;
            
            //_rSwdmxCmpbInfo.u4NumSentAud++;
            //u4Pts = _rSwdmxCmpbInfo.u4AudPts[u1TrackIdx];

            //enable audio according to pts, need fix by aud index table
#if 0  // !!!
            if(((UINT32)(_rSwdmxCmpbInfo.u8ASeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(_rSwdmxCmpbInfo.u8ASeekPts) != 0xFFFFFFFF))
            {
                if ((u4TimeSeekAudPts >= (UINT32)_rSwdmxCmpbInfo.u8ASeekPts))
                {
                    AUD_SetStartPts(0, u4TimeSeekAudPts);

                    if (_rSwdmxCmpbInfo.fgEnVideo)
                    {
                        VDP_SetPauseMm(0, TRUE);
                        _rSwdmxCmpbInfo.u4TriggerAudPts = (UINT32)u4TimeSeekAudPts;
                
                        VDEC_Pause(ES0);
                        _rSwdmxCmpbInfo.fgVDecIsPause = TRUE;
                    }
                    
                    LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n", 
                        u4TimeSeekAudPts, (UINT32)_rSwdmxCmpbInfo.u8ASeekPts);            
                    _rSwdmxCmpbInfo.u8ASeekPts = 0xFFFFFFFF;
                    _rSwdmxCmpbInfo.u8ASeekPts <<= 32;
                    _rSwdmxCmpbInfo.u8ASeekPts += 0xFFFFFFFF;
                }
                else
                {
                    //skip audio data in seek process
                    *pu4ConsumeSize = u4PacketSize + 14;
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
            }
#endif
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        // !!!
#if 1
        UINT16 u2MaxQNum, u2CurQNum;
        
        if (_rSwdmxCmpbInfo.fgEnVideo == FALSE)
        {
            u1HdrType = eSWDMX_MEDIA_SKIPPED;
        }
        
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        if (u2CurQNum > 60/*(_rSwdmxCmpbInfo.u4VidFps * 3)*/)
        {
            if (_rSwdmxCmpbInfo.fgVDecIsPause)
            {
                _SWDMX_CmpbVideoWakeUp();
                _rSwdmxCmpbInfo.fgVDecIsPause = FALSE;
            }
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            *pu4ConsumeSize = 0;            
            x_thread_delay(10);
            return eSWDMX_HDR_PRS_SUCCEED;
        }

        //u4Pts = _rSwdmxCmpbInfo.u4CurVidPts;

        // Record every video's chunk position.
        _rSwdmxCmpbInfo.u8VidOffset = _rSwdmxCmpbInfo.u8CurDmxFilePos;
        
#if 0
        if(((UINT32)(_rSwdmxCmpbInfo.u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(_rSwdmxCmpbInfo.u8VSeekPts) != 0xFFFFFFFF))
        {
            if(u4Pts > (UINT32)_rSwdmxCmpbInfo.u8VSeekPts)
            {
                LOG(5, "Got Vid T PTS 0x%x, Seek 0x%x\n", 
                    u4Pts, (UINT32)_rSwdmxCmpbInfo.u8VSeekPts);            
                _rSwdmxCmpbInfo.u8VSeekPts = 0xFFFFFFFF;
                _rSwdmxCmpbInfo.u8VSeekPts <<= 32;
                _rSwdmxCmpbInfo.u8VSeekPts += 0xFFFFFFFF;
            }
            /*else if (prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)_rSwdmxCmpbInfo.u8VSeekPts)
            {
                if (!_rSwdmxCmpbInfo.fgDeliverVEos)
                {
                    LOG(5, "prRangeList->prDmxRangeInfo->ui8_vid_duration < (UINT32)_rSwdmxCmpbInfo.u8VSeekPts\n"); 
                    if (!_SWDMX_SendCmpbEos(eSWDMX_MEDIA_VIDEO))
                    {
                        LOG(3, "Fail to send Video Eos\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                    _rSwdmxCmpbInfo.fgDeliverVEos = TRUE;
                }
            }*/
        }
#endif
#endif
    }

    _rCmpbDmxMMData.u4BufStart = (UINT32)(VIRTUAL(_rCmpbFeederInfo.u4StartAddr));
    _rCmpbDmxMMData.u4BufEnd = (UINT32)(VIRTUAL(_rCmpbFeederInfo.u4EndAddr));
    _rCmpbDmxMMData.u4Pts = (UINT32)u8Pts;
    _rCmpbDmxMMData.u4Dts = (UINT32)u8Pts;
    
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        if (1/*_rSwdmxCmpbInfo.u4ADecType == ENUM_SWDMX_ADEC_AAC*/)
        {
            _rCmpbDmxMMData.u4FrameSize = u4PacketSize;

            if (_rCmpbDmxMMData.u4StartAddr < _rCmpbDmxMMData.u4BufStart)
                _rCmpbDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

            if (DMX_MM_MoveData(eSWDMX_MEDIA_AUDIO, &_rCmpbDmxMMData, 500) != TRUE)
            {
                ASSERT(0);  // !!!
                LOG(0, "Demuxer fails to move data.\n");
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_VIDEO)  // video
    {
        if (1/*_rSwdmxCmpbInfo.u4VDecType == ENUM_SWDMX_VDEC_H264*/)
        {
            _rCmpbDmxMMData.u4FrameSize = u4PacketSize;

            if (_rCmpbDmxMMData.u4StartAddr < _rCmpbDmxMMData.u4BufStart)
                _rCmpbDmxMMData.u4BufStart -= FEEDER_RESERVED_SIZE;

            if (DMX_MM_MoveData(eSWDMX_MEDIA_VIDEO, &_rCmpbDmxMMData, 500) != TRUE)
            {
                ASSERT(0);  // !!!
                LOG(0, "Demuxer fails to move data.\n");
                *pu4ConsumeSize = 0;
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
    }
    else
    {
        ASSERT(0);
    }

//CMPB_PKT_END:
    *pu4ConsumeSize = u4PacketSize + 14;

    if (_rSwdmxCmpbInfo.fgIsBlock == TRUE)
    {
        _rSwdmxCmpbInfo.fgIsBlock = FALSE;
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsWithoutIdx(
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

    if (_rSwdmxCmpbInfo.fgFeederInvalid)
    {
        ASSERT(_rSwdmxCmpbInfo.i4ReqReadTimes == 0);
        LOG(3, "Feeder is invalid, Cur Feeder ID = %ld, req times = %ld, Cur Dmx Pos = %lld\n",
            _rSwdmxCmpbInfo.u4FeederReqID,
            _rSwdmxCmpbInfo.i4ReqReadTimes,
            _rSwdmxCmpbInfo.u8CurDmxFilePos);

        for ( ; _rSwdmxCmpbInfo.i4ReqReadTimes > 0;)
        {
            if (_SWDMX_CmpbWaitFeeder(&rFeederReqData))
                _rSwdmxCmpbInfo.i4ReqReadTimes--;
        }
        
        _SWDMX_CmpbResetFeeder();

        _rSwdmxCmpbInfo.fgIsSeek = TRUE;
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder = 0;
        _rSwdmxCmpbInfo.u8CurFilePos = _rSwdmxCmpbInfo.u8CurDmxFilePos;
        _rSwdmxCmpbInfo.fgFeederInvalid = FALSE;
        _rSwdmxCmpbInfo.i4ReqReadTimes = 0;
        _rSwdmxCmpbInfo.u4FeederReqID = 0;
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Check if demuxing is done
    if (_rSwdmxCmpbInfo.u8CurDmxFilePos >= prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset)
    {
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        if (_rSwdmxCmpbInfo.fgEnVideo && !_rSwdmxCmpbInfo.fgDeliverVEos)
        {
            if (_rSwdmxCmpbInfo.fgVDecIsPause)
            {
                _SWDMX_CmpbVideoWakeUp();
                _rSwdmxCmpbInfo.fgVDecIsPause = FALSE;
            }

            LOG(5, "Cur DmxMoviPos = %ld, range size = %ld, range offset = %ld\n",
                _rSwdmxCmpbInfo.u8CurDmxFilePos,
                prRangeList->prDmxRangeInfo->z_range_sz,
                prRangeList->prDmxRangeInfo->ui8_fileoffset);
            
            LOG(5, "_SWDMX_CmpbHdrPrsWithoutIdx Deliver Video Eos\n");
            if (!_SWDMX_SendCmpbEos(eSWDMX_MEDIA_VIDEO))
            {
                LOG(3, "Fail to send Video Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxCmpbInfo.fgDeliverVEos = TRUE;
        }

        if (_rSwdmxCmpbInfo.fgEnAudio && !_rSwdmxCmpbInfo.fgDeliverAEos)
        {
            LOG(5, "_SWDMX_CmpbHdrPrsWithoutIdx Deliver Audio Eos\n");
            if (!_SWDMX_SendCmpbEos(eSWDMX_MEDIA_AUDIO))
            {
                LOG(3, "Fail to send Audio Eos\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxCmpbInfo.fgDeliverAEos = TRUE;
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }

        return eSWDMX_HDR_PRS_SUCCEED;
    }

    // Request data for the first time
    if ((UINT32)_rSwdmxCmpbInfo.u8CurFilePos == 0 && 
        (UINT32)(_rSwdmxCmpbInfo.u8CurFilePos >> 32) == 0 &&
        _rSwdmxCmpbInfo.i4ReqReadTimes == 0)
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rCmpbFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(7, "Feeder input buffer error.\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

#if 1
        u4ReadSize = 1;
#else
        u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz;
        u4ReadSize = min(u4ReadSize, CMPB_MIN_READ_SIZE);
#endif
        _u4LastAlignOffset = 0;

        _rSwdmxCmpbInfo.u8CurFilePos = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        _rSwdmxCmpbInfo.u8CurDmxFilePos = _rSwdmxCmpbInfo.u8CurFilePos;

        while (_rSwdmxCmpbInfo.i4ReqReadTimes < FEEDER_DATA_Q_SIZE - 1)  // megaa 20100304: ToDo: check if the -1 is needed
        {
            if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                    u4ReadSize,
#if 1
                    0,
#else
                    _rSwdmxCmpbInfo.u8CurFilePos,
#endif
                    &rFeederCond,
                    _rSwdmxCmpbInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
            {
#ifdef CMPB_TIME_PROFILE
                HAL_GetTime(&_rSwdmxStart);
#endif
                _rSwdmxCmpbInfo.i4ReqReadTimes++;
            }
            else
            {
                ASSERT(0);
                break;
            }
        }
        
        if (!_SWDMX_CmpbGetDataFromFeeder(&rFeederReqData))
        {
            //_rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FAIL;
        }
#ifdef CMPB_TIME_PROFILE
        HAL_GetTime(&_rSwdmxEnd);
        HAL_GetDeltaTime(&_rSwdmxDif, &_rSwdmxStart, &_rSwdmxEnd);
        printf("Duration: %d\n", _rSwdmxDif.u4Seconds * 1000000 + _rSwdmxDif.u4Micros);
#endif
        _rSwdmxCmpbInfo.i4ReqReadTimes--;
        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
        _rSwdmxCmpbInfo.u8CurFilePos += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
#if 1
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
#else
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = VIRTUAL(_rCmpbFeederInfo.u4StartAddr + _u4LastAlignOffset);
#endif
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }
    
    if (_rSwdmxCmpbInfo.u4LeftBytesInFeeder == 0)
    {
        if (!_SWDMX_CmpbGetDataFromFeeder(&rFeederReqData))
        {
             //_rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
            return eSWDMX_HDR_PRS_FAIL;
        }
#ifdef CMPB_TIME_PROFILE
        HAL_GetTime(&_rSwdmxEnd);
        HAL_GetDeltaTime(&_rSwdmxDif, &_rSwdmxStart, &_rSwdmxEnd);
        printf("Duration: %d\n", _rSwdmxDif.u4Seconds * 1000000 + _rSwdmxDif.u4Micros);
#endif
        _rSwdmxCmpbInfo.i4ReqReadTimes--;
        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;
        _rSwdmxCmpbInfo.u8CurFilePos += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
#if 1
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
#else
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = VIRTUAL(_rCmpbFeederInfo.u4StartAddr + _u4LastAlignOffset);
#endif
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
        
        //return eSWDMX_HDR_PRS_FAIL;
    }
    
#if 1
    if ((_rSwdmxCmpbInfo.u4LeftBytesInFeeder == 0) || _rSwdmxCmpbInfo.i4ReqReadTimes < FEEDER_DATA_Q_SIZE / 2 ||
#else
    if ((_rSwdmxCmpbInfo.u4LeftBytesInFeeder <= CMPB_BUFFER_SIZE) ||
#endif
        (_rSwdmxCmpbInfo.fgIsBlock == TRUE))
    {
#if 1
        u4ReadSize = 1;
#else
        if (prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset > _rSwdmxCmpbInfo.u8CurFilePos)
        {
            u4ReadSize = prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset - 
                                   _rSwdmxCmpbInfo.u8CurFilePos;
        }
        u4ReadSize = min(u4ReadSize, CMPB_MIN_READ_SIZE);

        if (u4ReadSize == 0)
        {
            fgRangeEnd = TRUE;
        }
#endif

#if defined(CC_DLNA_SUPPORT) && 0
        // DLNA
        if ((!_rSwdmxCmpbInfo.fgFeederIsReady) ||
            (_rSwdmxCmpbInfo.u4LeftBytesInFeeder + u4ReadSize < CMPB_BUFFER_SIZE))
#else
        if (_rSwdmxCmpbInfo.u4LeftBytesInFeeder + u4ReadSize < CMPB_BUFFER_SIZE)
#endif
        {
            while (_rSwdmxCmpbInfo.i4ReqReadTimes < FEEDER_DATA_Q_SIZE - 1)  // megaa 20100304: ToDo: check if the -1 is needed
            {
                LOG(6, "Left bytes = %ld offset = %lld id = %ld.\n", 
                        _rSwdmxCmpbInfo.u4LeftBytesInFeeder,
                        _rSwdmxCmpbInfo.u8CurFilePos,
                        _rSwdmxCmpbInfo.u4FeederReqID);

                if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                        u4ReadSize, 
#if 1
                        0,
#else
                        _rSwdmxCmpbInfo.u8CurFilePos,
#endif
                        &rFeederCond,
                        _rSwdmxCmpbInfo.u4FeederReqID++) != FEEDER_E_BUFFER_FULL)
                {
#ifdef CMPB_TIME_PROFILE
                    HAL_GetTime(&_rSwdmxStart);
#endif
                    _u4LastAlignOffset = 0;
                    _rSwdmxCmpbInfo.i4ReqReadTimes++;
                }
                else
                {
                    LOG(3, "Feeder Buffer Full\n");
                    ASSERT(0);
                    //_rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
                    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rCmpbFeederInfo) != 
                        FEEDER_E_OK)
                    {
                        LOG(1, "Feeder input buffer error.\n");
                        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                    }
                    LOG(3, "Cur Fed's write ptr = 0x%08x\n", _rCmpbFeederInfo.u4WriteAddr);
                    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
                }
            }
        }
    }

#if defined(CC_DLNA_SUPPORT) && 0
    if (u4ReadSize != CMPB_MIN_READ_SIZE)
    {
        // DLNA
        FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
    }

    // DLNA
    if (!_rSwdmxCmpbInfo.fgFeederIsReady && FeederIsReady(FEEDER_PROGRAM_SOURCE))
    /*if ((!_rSwdmxCmpbInfo.fgFeederIsReady) && 
        ((_rSwdmxCmpbInfo.u4LeftBytesInFeeder >= CMPB_PREBUFFER_SIZE) || (_rSwdmxCmpbInfo.u4LeftBytesInFeeder >= prRangeList->prDmxRangeInfo->z_range_sz)))*/
    {
        _rSwdmxCmpbInfo.fgFeederIsReady = TRUE;
    }
#endif

#ifdef SWDMX_TIME_PROFILE
    HAL_TIME_T rTimeRstS, rTimeRstE, rTimeRstDt; 
    HAL_GetTime(&rTimeRstS);
#endif

#if defined(CC_DLNA_SUPPORT) && 0
    if ((_rSwdmxCmpbInfo.u4LeftBytesInFeeder > 0) && _rSwdmxCmpbInfo.fgFeederIsReady)
#else
    if (_rSwdmxCmpbInfo.u4LeftBytesInFeeder)
#endif
    {
        if (_rSwdmxCmpbInfo.fgSpeedChange && i4PlaySpeed == 1000)
        {
            _rSwdmxCmpbInfo.fgSpeedChange = FALSE;
        }
    
        // Start to parse data
        u4PrsState = _SWDMX_PrsCmpbPacket((UCHAR*)(_rSwdmxCmpbInfo.u4CurDmxFeederRPtr),
            prRangeList,
            _rSwdmxCmpbInfo.u4LeftBytesInFeeder,
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
            // Prevent unknow chunk in the first one mega
            // If the file size is smaller than one mega, CMPB demuxer
            // will check if demuxing is done in the beginning of _SWDMX_CmpbHdrPrsWithoutIdx
            if ((_rSwdmxCmpbInfo.u8CurDmxFilePos >= prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset) ||
                (_rSwdmxCmpbInfo.u8CurDmxFilePos > prRangeList->prDmxRangeInfo->ui8_fileoffset + ONE_MB))
            {
                LOG(3, "No CMPB chunks was discovered\n");
                //_rSwdmxCmpbInfo.u8CurDmxFilePos = prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
                //_rSwdmxCmpbInfo.u4LeftBytesInFeeder = 0;
                ASSERT(0);
                return eSWDMX_HDR_PRS_FAIL;
            }
            break;
        default:
            break;
    }

    if (_rSwdmxCmpbInfo.u4LeftBytesInFeeder >= u4ConsumedSize)
    {
        _rSwdmxCmpbInfo.u8CurDmxFilePos += u4ConsumedSize;
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder -= u4ConsumedSize;
    }
    else
    {
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder = 0;
    }

    if (u4ConsumedSize != 0)
    {
        LOG(6, "u4ConsumedSize = %ld u4LeftBytesInFeeder = 0x%08x.\n", 
            u4ConsumedSize, _rSwdmxCmpbInfo.u4LeftBytesInFeeder);
    }

    if ((_rSwdmxCmpbInfo.u4CurDmxFeederRPtr + u4ConsumedSize) >= 
        VIRTUAL(_rCmpbFeederInfo.u4EndAddr))
    {
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr =  VIRTUAL(_rCmpbFeederInfo.u4StartAddr) + 
            ((_rSwdmxCmpbInfo.u4CurDmxFeederRPtr + u4ConsumedSize) -VIRTUAL(_rCmpbFeederInfo.u4EndAddr));
    }
    else
    {
        _rSwdmxCmpbInfo.u4CurDmxFeederRPtr += u4ConsumedSize;
    }
    
    if (u4ConsumedSize != 0)
    {
        // Update read ptr to feeder
        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, 0);  // megaa 20100304: tmp solution (0 to distinguish push mode)
    }

    LOG(9, "_rSwdmxCmpbInfo.u4CurDmxMoviPos = %ld.\n", _rSwdmxCmpbInfo.u8CurDmxFilePos);

#if 0  // megaa 20100225: push mode behavior does not need this block
    if (_rSwdmxCmpbInfo.i4ReqReadTimes > 0)
    {
        if (!_SWDMX_CmpbGetDataFromFeeder(&rFeederReqData))
        {
            //_rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
            vHandleCmpbRingBuf();
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        LOG(6, "rFeederReqData.u4WriteAddr = 0x%08x. ReadSize = 0x%08x id = %ld\n", 
            rFeederReqData.u4WriteAddr,
            rFeederReqData.u4ReadSize,
            rFeederReqData.u4Id);

        _u4LastAlignOffset = rFeederReqData.u4AlignOffset;

        if ((rFeederReqData.u4ReadSize > _u4LastAlignOffset) &&
            (rFeederReqData.u4ReadSize != 0))
        {
            _rSwdmxCmpbInfo.u8CurFilePos += rFeederReqData.u4ReadSize - _u4LastAlignOffset;
        }
        else
        {
            LOG(3, "Feeder read size is smaller than align offset!!!\n");
            _rSwdmxCmpbInfo.u8CurDmxFilePos = prRangeList->prDmxRangeInfo->z_range_sz + prRangeList->prDmxRangeInfo->ui8_fileoffset;
            _rSwdmxCmpbInfo.u4LeftBytesInFeeder = 0;
            return eSWDMX_HDR_PRS_FAIL;
        }
                
        _rSwdmxCmpbInfo.u4LeftBytesInFeeder += rFeederReqData.u4ReadSize;
        _rSwdmxCmpbInfo.i4ReqReadTimes--;

        if(_rSwdmxCmpbInfo.fgIsSeek)
        {
            _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = VIRTUAL(rFeederReqData.u4WriteAddr + _u4LastAlignOffset);
            _rSwdmxCmpbInfo.fgIsSeek = FALSE;
        }
        LOG(7, "_SWDMX_ReceiveFeederAck [%ld],  u4QrySize= %ld u8Offset= 0x%x \n\n", rFeederReqData.u4Id, rFeederReqData.u4ReadSize, rFeederReqData.u8FilePos);
    }        
    vHandleCmpbRingBuf();
#endif
    
    return eSWDMX_HDR_PRS_SUCCEED;
}


/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_CmpbHdrPrsUseIdx(
        INT32 i4Speed, SWDMX_RANGE_LIST_T* prRangeList)
{
    return eSWDMX_HDR_PRS_SUCCEED;
}


/*static */SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverCmpbKeyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    return eSWDMX_HDR_PRS_SUCCEED;
}


static BOOL _SWDMX_CmpbResetFeeder(VOID)
{
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rCmpbFeederInfo) != 
        FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_CmpbResetFeeder Cur Fed's WPtr = 0x%08x\n", _rCmpbFeederInfo.u4WriteAddr);
    _rSwdmxCmpbInfo.u4CurDmxFeederRPtr = _rCmpbFeederInfo.u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxCmpbInfo.u4CurDmxFeederRPtr));
    return TRUE;
}


BOOL _SWDMX_CmpbSetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    UNUSED(u4StrmSubID);
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxCmpbInfo.u4VStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxCmpbInfo.u4AStrmID = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_CmpbEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    switch (u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxCmpbInfo.fgEnVideo = TRUE;
            if (_rSwdmxCmpbInfo.fgEnVDmx == FALSE)
            {
                _SWDMX_CmpbInitDmx(eSWDMX_MEDIA_VIDEO, FALSE);
                _rSwdmxCmpbInfo.fgEnVDmx = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if (_rSwdmxCmpbInfo.fgEnADmx == FALSE)
            {
                _SWDMX_CmpbInitDmx(eSWDMX_MEDIA_AUDIO, FALSE);
                _rSwdmxCmpbInfo.fgEnADmx = TRUE;
            }
            if ((_rSwdmxCmpbInfo.u4PrevAStrmID != 0xFFFFFFFF) &&
                (_rSwdmxCmpbInfo.u4AStrmID != _rSwdmxCmpbInfo.u4PrevAStrmID))
            {
                _rSwdmxCmpbInfo.fgChgATrack = TRUE;
            }
            _rSwdmxCmpbInfo.u4PrevAStrmID = _rSwdmxCmpbInfo.u4AStrmID;
            _rSwdmxCmpbInfo.fgEnAudio = TRUE;
            //_rSwdmxCmpbInfo.u1TotalATrack = 1;  // tmp solution for MW not setting this
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_CmpbDisableStrm(UINT8 u1StrmType)
{
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(_rSwdmxCmpbInfo.fgVDecIsPause)
            {
                _SWDMX_CmpbVideoWakeUp();
                _rSwdmxCmpbInfo.fgVDecIsPause = FALSE;
            }
            _rSwdmxCmpbInfo.fgEnVideo = FALSE;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxCmpbInfo.fgEnAudio = FALSE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_CmpbInitDmx(UINT8 u1HdrType, BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0; 
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));
    
    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!_rSwdmxCmpbInfo.fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = VIRTUAL(prFbmPool->u4Addr);
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
            _rSwdmxCmpbInfo.u4VFifoSa = VIRTUAL(prFbmPool->u4Addr);
            _rSwdmxCmpbInfo.u4VFifoEa = VIRTUAL(prFbmPool->u4Addr + prFbmPool->u4Size);

            //ASSERT(_rSwdmxCmpbInfo.u4VDecType == ENUM_SWDMX_VDEC_H264);
            if (_rSwdmxCmpbInfo.u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                rDmxMMInfo.fgSearchStartCode = TRUE;
                u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
            }
            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
    
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;

        if (!_rSwdmxCmpbInfo.fgEnADmx)
        {
            u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
                
            if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
                 &au4BufEnd[1]) != AUD_OK)
            {
                LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                return FALSE;
            }
            
            rDmxMMInfo.fgAllocBuf = FALSE;
            rDmxMMInfo.u4BufAddr = au4BufStart[0];
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
            _rSwdmxCmpbInfo.u4AFifoSa = VIRTUAL(rDmxMMInfo.u4BufAddr);
            _rSwdmxCmpbInfo.u4AFifoSz = rDmxMMInfo.u4BufSize;
        }
        else if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;

            fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
        }
    }

    return fgRet;
}


static BOOL _SWDMX_SendCmpbEos(UINT8 u1HdrType)
{
    BOOL fgRet;
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];

    LOG(5, "SWDMX Send CMPB Eos Hdr Type = %d\n", u1HdrType);

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rCmpbFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxCmpbInfo.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
    }
    
    x_memset(&_rCmpbDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_CmpbInitDmx(u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    _rCmpbDmxMMData.u4BufStart = _rCmpbFeederInfo.u4StartAddr;
    _rCmpbDmxMMData.u4BufEnd = _rCmpbFeederInfo.u4EndAddr;
    _rCmpbDmxMMData.u4StartAddr = _rCmpbFeederInfo.u4StartAddr;
    _rCmpbDmxMMData.u4FrameSize = 4;
    _rCmpbDmxMMData.fgEOS = TRUE;

    fgRet = DMX_MM_MoveData(u1HdrType, &_rCmpbDmxMMData, 500);
    ASSERT(fgRet);
    
    _rCmpbDmxMMData.fgEOS = FALSE;
    
    return fgRet;
}


/**
*/
BOOL _SWDMX_CmpbStop(VOID)
{
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;
    
    if (_rSwdmxCmpbInfo.fgEnVDmx)
    {
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
        _rSwdmxCmpbInfo.fgEnVDmx = FALSE;
    }

    if (_rSwdmxCmpbInfo.fgEnADmx)
    {
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
        _rSwdmxCmpbInfo.fgEnADmx = FALSE;
    }

    return TRUE;
}


static BOOL _SWDMX_CmpbGetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData)
{
    if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_CmpbGetDataFromFeeder got Feeder ACK failed!!!\n");
        _rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
        LOG(3, "_SWDMX_CmpbGetDataFromFeeder timeout\n");
        _rSwdmxCmpbInfo.fgFeederTimeout = TRUE;
        return FALSE;
    }
    
#if 0
    if ((prFeederData->u4Id + 1) != _rSwdmxCmpbInfo.u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        _rSwdmxCmpbInfo.fgFeederInvalid = TRUE;
        return FALSE;
    }
#endif
    
    _rSwdmxCmpbInfo.fgFeederTimeout = FALSE;

    return TRUE;
}


// tmp solution
static BOOL _SWDMX_CmpbWaitFeeder(FEEDER_REQ_DATA_T *prFeederData)
{
    while (1)
    {
        if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
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


BOOL _SWDMX_CmpbAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{  
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if(_rSwdmxCmpbInfo.fgEnVideo)
        {
            if(_rSwdmxCmpbInfo.fgAudTrigVidDec)
            {
                switch((ENUM_SWDMX_VDEC_T)_rSwdmxCmpbInfo.u4VDecType)
                {
                    case ENUM_SWDMX_VDEC_H264:
                        VDEC_Play(ES0, VDEC_FMT_H264);
                    break;
                    case ENUM_SWDMX_VDEC_MJPEG:
                        VDEC_Play(ES0, VDEC_FMT_MJPEG);
                        break;
#if defined(CC_MT5365) || defined(CC_MT5395)
                    case ENUM_SWDMX_VDEC_RAW:
                        VDEC_Play(ES0, VDEC_FMT_RAW);
                        break;
#endif
                    default:
                        LOG(1, "VidPlay by AudCb fail, Unknow VidType %d!\n", _rSwdmxCmpbInfo.u4VDecType);
                    return TRUE;
                }
#if 0
                if(_rSwdmxCmpbInfo.u8AudSeekPts == 0)
                {
                    LOG(3,"VidPlay by AudCb, vdp trig aud PTS 0x%x\n", _rSwdmxCmpbInfo.u4AudTrigVidDecPTS);
                    VDP_TriggerAudReceive(_rSwdmxCmpbInfo.u4AudTrigVidDecPTS);
                }
                else
#endif
                {
                    LOG(1, "AudCb before feeding data!\n");
                    AUD_MMAoutEnable(0, TRUE);  // for EOS, aud will not notify EOS if Aout Disable 
                }
                _rSwdmxCmpbInfo.fgAudTrigVidDec = FALSE;
            }
            //_rSwdmxCmpbInfo.fgIsAudWaitAout = TRUE;
        }
        else
        {
            STC_StartStc();
            AUD_MMAoutEnable(0, TRUE);
        }
    }
    else
    {
        LOG(1, "AudCb Type Unknow %d\n", (UINT32)eAudioNotifyType);
    }
    return TRUE;
}


static VOID _SWDMX_CmpbVideoWakeUp(VOID)
{
    LOG(5, "_SWDMX_CmpbVideoWakeUp\n");
    VDP_SetPauseMm(0, FALSE);
    switch (_rSwdmxCmpbInfo.u4VDecType)
    {
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(ES0, VDEC_FMT_H264);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(ES0, VDEC_FMT_MJPEG);
            break;
#if defined(CC_MT5365) || defined(CC_MT5395)
        case ENUM_SWDMX_VDEC_RAW:
            VDEC_Play(ES0, VDEC_FMT_RAW);
            break;
#endif
        default:
            LOG(1, "_SWDMX_CmpbVideoWakeUp Unknown Video Type\n");
            ASSERT(0);
            break;
    }
}
