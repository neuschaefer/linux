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
 * $RCSfile: swdmx_mp4.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_mp4.c
 *  Software demux for mp4 - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_mp4.h"
#include "vdec_drvif.h"
#include "u_midxbuld.h"
#include "swdmx_debug.h"
#include "aud_drvif.h"

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define AAC_FAKE_HDR_SIZE 7
#define AAC_ALLOC_SIZE   (256)

#define MIN_READ_SIZE     (512* 1024)
#define MP4_BUFFER_SIZE (235 * 7 * 1024)
#define MP4_KEY_TABLE_THRESHOLD (3)
#define MP4_ONE_SEC_DURATION (90000 * 2)
#define FPS_PRECISION (1000)

#define H264_STARTCODE_LEN (3)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define min(x, y) ((x < y) ? x : y)
#define max(x, y) ((x > y) ? x : y)

#define abs(x)                  (((x) >= 0) ? (x) : -(x))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static SWDMX_MP4_INFO_T _rSwdmxMp4Info;

static FEEDER_BUF_INFO_T _rFeederInfo;
static UCHAR *_pucAudBuf = 0;

static MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T _rPrevSample = {0};
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4RequestData(void);
static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(UINT8 u1HdrType);
static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsUseIdx(
    INT32 i4Speed,
    SWDMX_RANGE_LIST_T* prRangeList);

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4DataUseIdx(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType);
static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4Keyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList);
static BOOL _SWDMX_InitDmx(UINT8 u1HdrType, BOOL fgEos);

static BOOL _SWDMX_SendMp4Eos(UINT8 u1HdrType);

static UINT32 _SWDMX_Mp4Seek(UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList, 
    INT32 i4PlaySpeed);

static BOOL _SWDMX_Mp4GetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData);

static BOOL _SWDMX_Mp4CalculateRepeatFrame(INT32 i4PlaySpeed);

static BOOL _SWDMX_Mp4ResetFeeder(void);

static void _SWDMX_Mp4VideoWakeUp(void);

BOOL _SWDMX_Mp4SetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3)
{
    if(u1Type == eSWDMX_SET_VIDEOTYPE) 
    {
        #if defined(CC_MP4_MPEG1_DISABLE) && defined(CC_MP4_MPEG2_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_MPEG4_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_DX311_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_H264_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_VC1_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
        {
            return FALSE;
        }
        else
        #endif
        #if defined(CC_MP4_MJPEG_DISABLE)
        if(u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
        {
            return FALSE;
        }
        else
        #endif
        {
        _rSwdmxMp4Info.u4VDecType =  u4Para1;
        return TRUE;
    }
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
#ifndef LINUX_TURNKEY_SOLUTION
                _rSwdmxMp4Info.u4VidHeaderPtr = u4Para1;
                _rSwdmxMp4Info.u4VidHdrLength = u4Para2;
 #else
               _SWDMX_GetInfo(eSWDMX_GET_INTERNAL_BUF, &_rSwdmxMp4Info.u4VidHeaderPtr, 0, 0);
                _rSwdmxMp4Info.u4VidHdrLength = u4Para2;
 
                if(_rSwdmxMp4Info.u4VidHeaderPtr)
                {
                    x_memcpy((VOID*)VIRTUAL(_rSwdmxMp4Info.u4VidHeaderPtr ), 
                        (VOID*)VIRTUAL(u4Para1), 
                        _rSwdmxMp4Info.u4VidHdrLength);
                }
                else
                {
                    LOG(0,"Header Seq Buffer alloc fail\n");
                }
 #endif
        return TRUE;
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxMp4Info.u4ADecType = u4Para1;
        _rSwdmxMp4Info.u4AudSampleRate = u4Para2;
        _rSwdmxMp4Info.u4AudChannelNs = u4Para3;
        return TRUE;
    }
    
    return FALSE;
}

BOOL _SWDMX_Mp4GetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
	LOG(5, "_SWDMX_Mp4GetInfo u1Type = %d\n", u1Type);

	if(u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_Mp4GetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = _rSwdmxMp4Info.u8CurFilepos;
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
        if(_rSwdmxMp4Info.fgEnVideo)
        {
            if (_rSwdmxMp4Info.prVidKeyIdxTbl != NULL)
            {
                if (_rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry < MP4_KEY_TABLE_THRESHOLD)
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = FALSE;
                    }
                    if(pu4Para2)
                    {
                        *pu4Para2 = FALSE;
                    }
                }
                else
                {
                    if(pu4Para1)
                    {
                        *pu4Para1 = TRUE;
                    }
                    if(pu4Para2)
                    {
                        *pu4Para2 = TRUE;
                    }
                }
            }
            else
            {
                if(pu4Para1)
                {
                    *pu4Para1 = FALSE;
                }
                if(pu4Para2)
                {
                    *pu4Para2 = FALSE;
                }
            }
        }
    }
        
    return FALSE;
}

static BOOL _MM_VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    
    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    rPesInfo.u4PTS = _rSwdmxMp4Info.u4LastVidPts;
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    // Pts is valid all the time
    rPesInfo.fgDtsValid = TRUE;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    LOG(11, " prPes->u4Pts = %x\n",  prPes->u4Pts );
   
    if (rPesInfo.fgEos)
    {
        rPesInfo.u4FifoStart = NULL;
        rPesInfo.u4FifoEnd = NULL;
        rPesInfo.u4VldReadPtr = NULL;
    }

    if((_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264) &&
        !rPesInfo.fgEos)
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
    else if ((_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_MPEG4) &&
        !rPesInfo.fgEos)
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
    else if (_rSwdmxMp4Info.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
    {
        rPesInfo.ucPicType = I_TYPE;
    }
    else if (_rSwdmxMp4Info.u4VDecType == (UINT32)ENUM_SWDMX_VDEC_H263)
    {
         UCHAR *pucBuf;

        u4Addr = prPes->u4FrameAddr + 4;
        if(u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        switch (pucBuf[0] & 0x02) 
        {
            case 0:
                rPesInfo.ucPicType = I_TYPE;
                break;
            case 2:
                rPesInfo.ucPicType = P_TYPE;
                break;
            default:
                rPesInfo.ucPicType = UNKNOWN_TYPE;
                break;
        }
    }

    VDEC_SendEs((void*)&rPesInfo);
    
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
BOOL _SWDMX_Mp4Init(ENUM_SWDMX_FMT_T eType)
{
    UNUSED(eType);
    // Initialize feeder
    //FeederOpen(FEEDER_PROGRAM_SOURCE);

    //*********************************************************
    //FeederSetBufferSize(FEEDER_PROGRAM_SOURCE, 5*10*1024); 
    //FeederStart(FEEDER_PROGRAM_SOURCE);
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE, 
    _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo);
    //*********************************************************

    //DMX_MM_DECODER_CALLBACKS_T rCallback; // Isaac: should be DMX_DECODER_CALLBACKS_T, Jack fix me
    DMX_DECODER_CALLBACKS_T rCallback;

    rCallback.pfnSendAudioPes = AUD_SendAudioPes;
    rCallback.pfnSendVideoPes = _MM_VideoCallback;
    rCallback.pfnUpdateVideoWp = VDEC_SetWptr;

    // Default some values
    _rSwdmxMp4Info.u4PrevAStrmID = 0xFFFFFFFF;
    _rSwdmxMp4Info.u4AudSampleRate = 48000;
    _rSwdmxMp4Info.u4AudChannelNs = 2;
    _rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
    _rSwdmxMp4Info.u8VSeekPts <<= 32;
    _rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
    _rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
    _rSwdmxMp4Info.u8ASeekPts <<= 32;
    _rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;

    _rSwdmxMp4Info.u4TriggerAudPts = 0xFFFFFFFF;
    _rSwdmxMp4Info.fgWaitNewRange = FALSE;
    _rSwdmxMp4Info.u4CurRepeatFrameIdx = 0xFFFFFFFF;

    x_memset(&_rPrevSample, 0, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));  
    
    //DDI_BUF_ALIGNMENT == 16 and      
    //for _dmx_mm_movedata  Meet the alignment and packet size (188 bytes) requirements.
    if(_rSwdmxMp4Info.u4ADecType == ENUM_SWDMX_ADEC_AAC &&
        _pucAudBuf == 0)
    {
        _pucAudBuf = 
            (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE,16));
        if(_pucAudBuf == 0)
        {
            LOG(1, "Allocate AAC buffer failed!\n");
            return FALSE;
        }
    }               

    DMX_SetDecoderCallbacks(&rCallback);
    DMX_SetToDecoder(TRUE);
    
    _rSwdmxMp4Info.fgAudStrmDisable = FALSE;

    _rSwdmxMp4Info.fgFeederInvalid = TRUE;
    _rSwdmxMp4Info.fgRepeatRequest = TRUE;
    return TRUE;
}


/**
*/
VOID _SWDMX_Mp4SetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID, 
                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    UINT8 u1ATrack;
    
    if (prRangeList != NULL &&
       prRangeList->prDmxRangeInfo != NULL)
    {
        u1ATrack = 0;
        MM_RANGE_ELMT_IDXTBL_T *pBuldIdxTbl = 
        (MM_RANGE_ELMT_IDXTBL_T*)prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        _rSwdmxMp4Info.prDmxRangeInfo = prRangeList->prDmxRangeInfo;
        while (pBuldIdxTbl != NULL)
        {
            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_AUDIO &&
                u1ATrack < MP4_MAX_AUDIO_TRACK)
            {
                _rSwdmxMp4Info.prAudIdxTbl[u1ATrack] = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange Audio Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxMp4Info.prAudIdxTbl[u1ATrack]));
                u1ATrack++;
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_VIDEO)
            {
                _rSwdmxMp4Info.prVidIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange Video Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxMp4Info.prVidIdxTbl));
            }
            else if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY)
            {
                _rSwdmxMp4Info.prVidKeyIdxTbl = pBuldIdxTbl;
                LOG(5, "_SWDMX_Mp4SetRange Key Index table = 0x%08x\n",
                    (UINT32)(_rSwdmxMp4Info.prVidKeyIdxTbl));
            }
            
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
            
           _rSwdmxMp4Info.ui4_data_offset = prRangeList->prDmxRangeInfo->ui8_fileoffset;
           _rSwdmxMp4Info.u8_range_sz  = prRangeList->prDmxRangeInfo->z_range_sz;
        }

        if (_rSwdmxMp4Info.prVidIdxTbl)
        {
            // The upper layer (mw or playmgr should prevent 0 case for rate and scale)
            if (_rSwdmxMp4Info.prVidIdxTbl->ui4_scale)
            {
                _rSwdmxMp4Info.u4VidFps = 
                    (UINT32)(_rSwdmxMp4Info.prVidIdxTbl->ui4_rate/_rSwdmxMp4Info.prVidIdxTbl->ui4_scale);
            }

            // Add some error handling
            if (!_rSwdmxMp4Info.u4VidFps ||
            	_rSwdmxMp4Info.u4VidFps > 60)
            {
                _rSwdmxMp4Info.u4VidFps = 30;
            }

            LOG(5, "_rSwdmxMp4Info.u4VidFps = %ld\n", _rSwdmxMp4Info.u4VidFps);
        }

        // Set the number of total audio tracks
        _rSwdmxMp4Info.u1TotalATrack = u1ATrack;
        
        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
    else
    {
        LOG(1, "Demuxer Range is NULL\n");
    }
}


/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrs(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange
)
{
    UINT32 u4TargetPTS;
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl;
    INT32  i;

    if ((!_rSwdmxMp4Info.fgBeginToPlay) &&
        (i4PlaySpeed == 1000))
    {
        if (_rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts != 0)
        {
             if ((_rSwdmxMp4Info.fgEnVideo) && 
                (_rSwdmxMp4Info.fgEnAudio) &&
                (_rSwdmxMp4Info.prVidIdxTbl) &&
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
            {
                _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
                UNUSED(_SWDMX_Mp4Seek(_rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts, prRangeList, i4PlaySpeed));
                LOG(5, "Video Start Render Pts = %lld\n", 
                _rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts);

                if (prRangeList->fgIsSeekTarget)
                {
                    prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
                    VDEC_SetRenderFromPts(0, _rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts);
                    
                    // set stc for starting playback
                    STC_StopStc();
                    STC_SetStcValue(_rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts - 45000);

                    prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                    AUD_SetStartPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
                    //VDP_SetPauseMm(0, TRUE);

                    _rSwdmxMp4Info.u4TriggerAudPts = 
                        prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                    VDP_SetPauseMm(0, TRUE);
                    VDEC_Pause(ES0);
                    _rSwdmxMp4Info.fgVDecIsPause = TRUE;
               }
                else
                {
                    _rSwdmxMp4Info.u8VSeekPts = _rSwdmxMp4Info.prDmxRangeInfo->t_vid_start_render_pts;
                    _rSwdmxMp4Info.fgIsSeek = TRUE;
                }
            }
            // Video only
            else if (_rSwdmxMp4Info.fgEnVideo && _rSwdmxMp4Info.prVidIdxTbl)
            {
                prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts);
                
                // set stc for starting playback
                STC_StopStc();
                STC_SetStcValue(prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                VDP_SetPauseMm(0, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxMp4Info.fgVDecIsPause = TRUE;
            }
            // Audio only
            else if ((_rSwdmxMp4Info.fgEnAudio) && 
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
            }
            else
            {
                LOG(1, "SWDMX_Mp4 Unknow state for starting playback\n");
                return eSWDMX_HDR_PRS_FAIL;
            }
            _rSwdmxMp4Info.fgBeginToPlay = TRUE;
       }
        else
        {
            if ((_rSwdmxMp4Info.fgEnVideo) && 
                (_rSwdmxMp4Info.fgEnAudio) &&
                (_rSwdmxMp4Info.prVidIdxTbl) &&
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts);
                
                // set stc for starting playback
                STC_StopStc();
                STC_SetStcValue(prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
                //VDP_SetPauseMm(0, TRUE);

                _rSwdmxMp4Info.u4TriggerAudPts = 
                    prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                VDP_SetPauseMm(0, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxMp4Info.fgVDecIsPause = TRUE;
            }
            // Video only
            else if (_rSwdmxMp4Info.fgEnVideo && _rSwdmxMp4Info.prVidIdxTbl)
            {
                prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
                VDEC_SetRenderFromPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts);
                
                // set stc for starting playback
                STC_StopStc();
                STC_SetStcValue(prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_pts - 45000);

                VDP_SetPauseMm(0, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxMp4Info.fgVDecIsPause = TRUE;
            }
            // Audio only
            else if ((_rSwdmxMp4Info.fgEnAudio) && 
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
            {
                prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                AUD_SetStartPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);
            }
            else
            {
                LOG(1, "SWDMX_Mp4 Unknow state for starting playback\n");
                return eSWDMX_HDR_PRS_FAIL;
            }
            _rSwdmxMp4Info.fgBeginToPlay = TRUE;
        }
    }     

    if (_rSwdmxMp4Info.fgChgATrack ||
        fgSpeedChange)
    {
        _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        //_rSwdmxMp4Info.u4NumSentAud = 0;
        //_rSwdmxMp4Info.u4NumSentVid = 0;
        _rSwdmxMp4Info.fgAudIsReady = FALSE;
        _rSwdmxMp4Info.fgBlockAud = FALSE;
        _rSwdmxMp4Info.fgBlockVid = FALSE;
        _rSwdmxMp4Info.fgDeliverAEos = FALSE;
    }
    
    if ((_rSwdmxMp4Info.fgChgATrack) && 
        (_rSwdmxMp4Info.u1TotalATrack > 0) &&
        (_rSwdmxMp4Info.fgEnAudio) &&
        (_rSwdmxMp4Info.u1TotalATrack < MP4_MAX_AUDIO_TRACK))
    {
        BOOL fgFindAudio = FALSE, fgFindATrack = FALSE;
        UINT32 u4CurPts;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl;   //current range

        fgFindATrack = FALSE;
        for (i = 0; (i < _rSwdmxMp4Info.u1TotalATrack) && (i < MP4_MAX_AUDIO_TRACK); i++)
        {
            if (_rSwdmxMp4Info.prAudIdxTbl[i] && 
                _rSwdmxMp4Info.prAudIdxTbl[i]->t_strm_id.u.t_stm_mp4_id == _rSwdmxMp4Info.u4AStrmID)
            {
                _rSwdmxMp4Info.u1CurATrack = i;
                fgFindATrack = TRUE;
            }
        }
        if (!fgFindATrack)
        {
            LOG(3, "_SWDMX_Mp4EnableStrm didn't find audio stream\n");
            return eSWDMX_HDR_PRS_EMPTY_RANGE;
        }

        if (!_rSwdmxMp4Info.fgEnVideo)
        {
            u4CurPts = STC_GetStcValue();
            // set audio range idx
            prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
            for(i = 1; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if((prIdxTbl->u.pt_pb_idx_tbl_entry[i-1].ui4_pts <= u4CurPts) &&
                    (prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u4CurPts)||
                    (_rSwdmxMp4Info.fgWaitNewRange))
                {
                    prRangeList->u4CurRangeAudIdx = i;
                    _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
                    LOG(5, "Cur Audio Index = %d Audio Track PTS = 0x%08x\n", 
                        prRangeList->u4CurRangeAudIdx,
                        _rSwdmxMp4Info.u4CurAudPts);
                    fgFindAudio = TRUE;
                    _rSwdmxMp4Info.fgChgATrack = FALSE;
			              AUD_SetStartPts(0, _rSwdmxMp4Info.u4CurAudPts);
                    break;
                }
            }
            if (!fgFindAudio)
            {
                LOG(3, "Didn't find Audio\n");
                prRangeList->u4CurPTS = u4CurPts;
                if(!_rSwdmxMp4Info.fgEnVideo)
			          {	
			              _rSwdmxMp4Info.fgWaitNewRange = TRUE;
			          }
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        else
        {
            u4CurPts = VDP_GetPts(0);

            if (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
            {
                // set audio range idx
                prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                {
                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u4CurPts)
                    {
                        prRangeList->u4CurRangeAudIdx = i;
                        _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
                        LOG(5, "Cur Audio Index = %d Audio Track PTS = 0x%08x\n", 
                            prRangeList->u4CurRangeAudIdx,
                            _rSwdmxMp4Info.u4CurAudPts);
                        fgFindAudio = TRUE;
                        _rSwdmxMp4Info.fgChgATrack = FALSE;
                        _rSwdmxMp4Info.u4TriggerAudPts =  _rSwdmxMp4Info.u4CurAudPts;
                        AUD_SetStartPts(0, _rSwdmxMp4Info.u4CurAudPts);
                        break;
                    }
                }
            }
        }

        LOG(5, "Chang to Track %d Cur PTS = 0x%08x\n", 
            _rSwdmxMp4Info.u1CurATrack,
            u4CurPts);
        
        DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 
    }


    if(fgSpeedChange)
    {
        _rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
        _rSwdmxMp4Info.u8VSeekPts <<= 32;
        _rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
        _rSwdmxMp4Info.fgWaitNewRange =  FALSE;  
        _rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
        _rSwdmxMp4Info.u8ASeekPts <<= 32;
        _rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;
	
        if (i4PlaySpeed > 2000 || i4PlaySpeed < 0)
        {
            _rSwdmxMp4Info.fgStartTrick = TRUE;
        }

        if (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        }

        if((_rSwdmxMp4Info.fgRecSeekFlag)&&(_rSwdmxMp4Info.i4PrevSpeed == 1))
        {
             _rSwdmxMp4Info.fgSeekAfterPause = TRUE;
        } 
         
        if(_rSwdmxMp4Info.fgVDecIsPause)
        {
            _SWDMX_Mp4VideoWakeUp();
            _rSwdmxMp4Info.fgVDecIsPause = FALSE;
        }
    
        UINT32 u4CurPts = VDP_GetPts(0);
        LOG(5, "Mp4 Speed change while VDP Pts = 0x%08x\n", u4CurPts);

        _rSwdmxMp4Info.fgSpeedChange = TRUE;

        // Reset some values for state change.
        if (_rSwdmxMp4Info.fgDeliverVEos)
        {
            UINT32 u4Flag;
            DMX_MM_T rDmxMMInfo;

            rDmxMMInfo.fgEnable = TRUE;
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;

            switch (_rSwdmxMp4Info.u4VDecType)
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
            if (!DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo))
            {
                LOG(3, "Fail to reset demuxer\n");
            }
            
            _rSwdmxMp4Info.fgDeliverVEos = FALSE;
        }
        if (_rSwdmxMp4Info.fgDeliverAEos)
        {
            _rSwdmxMp4Info.fgDeliverAEos = FALSE;
            AUD_ClearEOSFlag(0);
        }
        if (i4PlaySpeed == 1000)
        {
            _rSwdmxMp4Info.u4VidRepeatFrame = 0;
        }
                 
        if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF) || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
        {  
            if ((i4PlaySpeed < 4000) && (i4PlaySpeed > 0))
            {
                _rSwdmxMp4Info.fgIsSeek = TRUE;
                _rSwdmxMp4Info.fgRecSeekFlag = TRUE;
                _rSwdmxMp4Info.u8VSeekPts = u8SeekPos;
		            _rSwdmxMp4Info.u8ASeekPts = u8SeekPos;
                //should enable audio if parsing to audio entry larger than target PTS
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            }
            else
            {
                _SWDMX_Mp4Seek(u8SeekPos, prRangeList, i4PlaySpeed);
            }

            if(i4PlaySpeed >0)
            {    
                if(_rSwdmxMp4Info.fgEnVideo)
                {
                    VDEC_SetRenderPts(ES0, u8SeekPos);
                }
            }
            else
            {    
                VDEC_SetRenderPts(ES0, 0);
            }

            VDEC_StartPTS(ES0,FALSE,u8SeekPos,NULL);

            // set stc
            STC_StopStc();
            STC_SetStcValue(u8SeekPos);
        }
        // 2X -> 1X
        else if (((_rSwdmxMp4Info.i4PrevSpeed == 2000) ||
            (_rSwdmxMp4Info.i4PrevSpeed == 1) ||
            ((_rSwdmxMp4Info.i4PrevSpeed < 1000) && (_rSwdmxMp4Info.i4PrevSpeed > 1))) &&
            i4PlaySpeed == 1000)
        {
            LOG(5, "Mp4 Speed 2X -> 1X\n");

		        if ((_rSwdmxMp4Info.i4PrevSpeed == 1) || 
		           ((_rSwdmxMp4Info.i4PrevSpeed < 1000) && (_rSwdmxMp4Info.i4PrevSpeed > 1)))
		        {
		      
			          STC_StartStc();
		        }

            if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) && 
                (_rSwdmxMp4Info.prVidIdxTbl))
            {
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 
                // set audio range idx
                MM_RANGE_ELMT_IDXTBL_T *prIdxTbl;
                prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
                for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
                {
                    if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u4CurPts)
                    {
                        prRangeList->u4CurRangeAudIdx = i;
                        _rSwdmxMp4Info.fgHasAud = TRUE;
                        break;
                    }
                }

                if (_rSwdmxMp4Info.fgHasAud)
                {
                    AUD_SetStartPts(0, prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts);

                    _rSwdmxMp4Info.u4TriggerAudPts = 
                        prIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;
                }
            }
        }
        else if (_rSwdmxMp4Info.i4PrevSpeed < 0 &&
            (i4PlaySpeed == 2000 || i4PlaySpeed == 1000))
        {
            LOG(5, "Rewind -> 2X/1X\n");

            _SWDMX_Mp4Seek(u4CurPts, prRangeList, i4PlaySpeed);

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO); 
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 

            MPV_FlushEsmQ(0, TRUE);
            VDEC_ReleaseDispQ(0);

            _SWDMX_FlushReqMsgQ();

            VDEC_SetRenderFromPts(0, 
                _rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts);

            _rSwdmxMp4Info.fgIsSeek = TRUE;
            _rSwdmxMp4Info.u8VSeekPts = u4CurPts;

            // set stc
            STC_StopStc();
            STC_SetStcValue(u4CurPts);
        }
        // else if NOT 1X -> 2X
        else if (!(_rSwdmxMp4Info.i4PrevSpeed == 1000 && (i4PlaySpeed == 2000 || i4PlaySpeed == 1)))
        {
            if (!((i4PlaySpeed < 1000) && (i4PlaySpeed > 1)))
            {
                _SWDMX_Mp4Seek(u4CurPts, prRangeList, i4PlaySpeed);

                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO); 
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO); 

                MPV_FlushEsmQ(0, TRUE);
                VDEC_ReleaseDispQ(0);

                _SWDMX_FlushReqMsgQ();

                if (i4PlaySpeed > 1000)
                {
                    u4CurPts = _rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts;
                    VDEC_SetRenderFromPts(0, 
                        _rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts);
                }
                else if (i4PlaySpeed <= 1000 &&
                    i4PlaySpeed > 0)
                {
                    VDEC_SetRenderFromPts(0, u4CurPts);
                }
                else
                {
                    VDEC_SetRenderFromPts(0, 
                        _rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts);
                }

                // Things for 32/16/8/4/-2/-4/-8/-16/-32 -> 1X are the same
                if ((_rSwdmxMp4Info.i4PrevSpeed > 2000 || _rSwdmxMp4Info.i4PrevSpeed < 0) &&
                    i4PlaySpeed == 1000)
                {
                    LOG(5, "Trick -> normal PTS = 0x%08x\n", u4CurPts);

                    _rSwdmxMp4Info.fgIsSeek = TRUE;
                    _rSwdmxMp4Info.u8VSeekPts = u4CurPts;
                }

                // set stc
                STC_StopStc();
                STC_SetStcValue(u4CurPts);

                LOG(5, "_rSwdmxAviInfo.i4PrevSpeed != 1000 i4PlaySpeed != 2000\n");
                // don't need to handle 1X->2X
            }
        }

    }
    
    if(_rSwdmxMp4Info.fgIsSeek)
    {
        u4TargetPTS = _SWDMX_Mp4Seek(_rSwdmxMp4Info.u8VSeekPts, prRangeList, i4PlaySpeed);

        if(prRangeList->fgIsSeekTarget)
        {
            LOG(5, "prRangeList->fgIsSeekTarget\n");

            prRangeList->fgIsSeekTarget = FALSE;
            _rSwdmxMp4Info.fgIsSeek = FALSE;

            // set stc for starting playback
            STC_StopStc();
            STC_SetStcValue(_rSwdmxMp4Info.u8VSeekPts);

            //VDEC_SetRenderFromPts(0, _rSwdmxMp4Info.u8SeekPts);

            if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                (_rSwdmxMp4Info.fgHasAud))
            {
                _rSwdmxMp4Info.fgBlockVid = TRUE;
                _rSwdmxMp4Info.fgBlockAud = FALSE;
                
                _rSwdmxMp4Info.u4TriggerAudPts = 
                    _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_pts;                    

                AUD_SetStartPts(0, _rSwdmxMp4Info.u4TriggerAudPts);
            }
        }
        else
        {
            //need new range containing target PTS
            LOG(0, "Seek: Need New Range\n");
            prRangeList->u4CurPTS = u4TargetPTS;
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_SKIP;
            if(!_rSwdmxMp4Info.fgEnVideo)
            {	
                _rSwdmxMp4Info.fgWaitNewRange = TRUE;
            }
            return eSWDMX_HDR_PRS_SUCCEED;
        }
    }

    if((prRangeList->prDmxRangeInfo != NULL) &&
       (prRangeList->prDmxRangeInfo->pt_idxtbl_lst != NULL) &&
       (prRangeList->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry != NULL))
    {
        _rSwdmxMp4Info.i4PrevSpeed = i4PlaySpeed;
        return _SWDMX_Mp4HdrPrsUseIdx(i4PlaySpeed, prRangeList);
    }
    else
    {
        return eSWDMX_HDR_PRS_FAIL;
    }
}

static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4HdrPrsUseIdx(
        INT32 i4Speed, SWDMX_RANGE_LIST_T* prRangeList)
{
    BOOL fgRangeDone = TRUE;

    UINT32   u4VidOffset = 0;
    UINT32   u4AudOffset = 0;
 
    UINT32 *pu4VidCurIdx  = NULL;
    UINT32 *pu4AudCurIdx = NULL;
    
    MM_RANGE_ELMT_IDXTBL_T *pKeyIdxTbl = _rSwdmxMp4Info.prVidKeyIdxTbl; 

    if (!prRangeList)
    {
        LOG(3, "Range list is NULL\n");
        return eSWDMX_HDR_PRS_EMPTY_RANGE; 
    }

    if ((i4Speed > 2000) || (i4Speed < 0))
    {
        fgRangeDone = FALSE;
        if (pKeyIdxTbl != NULL && 
            (_rSwdmxMp4Info.u4CurSentKey < pKeyIdxTbl->ui4_number_of_entry))
        {

            LOG(6, "pKeyIdxTbl->ui4_number_of_entry = %x\n", pKeyIdxTbl->ui4_number_of_entry);
            LOG(6, "_rSwdmxMp4Info.u4CurSentKey = %x\n", _rSwdmxMp4Info.u4CurSentKey);
            pu4VidCurIdx = &_rSwdmxMp4Info.u4CurSentKey;

            if (_rSwdmxMp4Info.u4NumSentVid == 0)
            {
                _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
            }

            _SWDMX_DeliverMp4Keyframe(i4Speed,
                _rSwdmxMp4Info.prVidKeyIdxTbl, 
                pu4VidCurIdx, 
                eSWDMX_MEDIA_VIDEO,
                prRangeList);
        }
        else if((pKeyIdxTbl != NULL) && (_rSwdmxMp4Info.u4CurSentKey >= pKeyIdxTbl->ui4_number_of_entry)&&
                  (!_rSwdmxMp4Info.fgDeliverVEos))
        {
            if (!_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
            {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
            _rSwdmxMp4Info.fgDeliverVEos = TRUE;
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
    }
    else
    {
        if((_rSwdmxMp4Info.fgEnVideo) && (!_rSwdmxMp4Info.prVidIdxTbl))
        {
            LOG(3, "No video index table now!\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;        
        }

        if((_rSwdmxMp4Info.fgEnAudio) && 
            (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
            (!_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
        {
            LOG(3, "No Audio index table now!\n");
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;        
        }

        if ((i4Speed == 2000 || i4Speed == 1) &&
            (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
            (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
        {
            prRangeList->u4CurRangeAudIdx = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry;
        }
   
        if ((_rSwdmxMp4Info.fgEnVideo) && (_rSwdmxMp4Info.fgEnAudio))
        {
            if (_rSwdmxMp4Info.fgSpeedChange && i4Speed == 1000 && _rSwdmxMp4Info.fgHasAud)
            {
                fgRangeDone = FALSE;
                LOG(5, "Pause Video for A/V Sync\n");
                VDP_SetPauseMm(0, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxMp4Info.fgVDecIsPause = TRUE;
                _rSwdmxMp4Info.fgSpeedChange = FALSE;

                _rSwdmxMp4Info.fgBlockVid = TRUE;
                _rSwdmxMp4Info.fgBlockAud = FALSE;

                _rSwdmxMp4Info.fgHasAud = FALSE;
            }
        }
        else if (_rSwdmxMp4Info.fgEnVideo && _rSwdmxMp4Info.fgSpeedChange)
        {
             fgRangeDone = FALSE;
            _rSwdmxMp4Info.fgBlockVid = FALSE;
            _rSwdmxMp4Info.fgBlockAud = FALSE;
            _rSwdmxMp4Info.fgSpeedChange = FALSE;
        }

        if(_rSwdmxMp4Info.fgFeederInvalid)
        {
            INT32 i;
            FEEDER_REQ_DATA_T rFeederReqData;
            
            for (i = 0; i < _rSwdmxMp4Info.i4ReqReadTimes; i++)
            {
                _SWDMX_Mp4GetDataFromFeeder(&rFeederReqData);
            }
           
            _rSwdmxMp4Info.u4LeftBytesInFeeder = 0;
            _rSwdmxMp4Info.fgRepeatRequest = TRUE;
            
            _rSwdmxMp4Info.fgFeederInvalid = FALSE;
            _rSwdmxMp4Info.u4FeederReqID = 0;
            _rSwdmxMp4Info.i4ReqReadTimes = 0;

            // 2X
            if (i4Speed == 2000 || i4Speed == 1)
            {
                _rSwdmxMp4Info.u8VidLatestOff = 
                    _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                _rSwdmxMp4Info.u8CurFilepos   = _rSwdmxMp4Info.u8VidLatestOff;
            }
            else if(_rSwdmxMp4Info.fgBlockVid)
            {
                if (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
                {
                    LOG(6, "Block Video Audio offset = %x\n", 
                        _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_relative_offset);

                    _rSwdmxMp4Info.u8AudLatestOff = 
                    _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_relative_offset;
                    _rSwdmxMp4Info.u8CurFilepos = _rSwdmxMp4Info.u8AudLatestOff;
                }
                else
                {
                    LOG(1, "Out of Max Audio Tracks\n");
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
            else if(_rSwdmxMp4Info.fgBlockAud)
            {
                _rSwdmxMp4Info.u8VidLatestOff = 
                    _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                _rSwdmxMp4Info.u8CurFilepos = _rSwdmxMp4Info.u8VidLatestOff;
            }
            else
            {
                if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                    (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                    (_rSwdmxMp4Info.fgEnAudio) &&
                    (prRangeList->u4CurRangeAudIdx <_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry))
                {
                    _rSwdmxMp4Info.u8AudLatestOff = 
                        _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeAudIdx].ui4_relative_offset;
                }
                else
                {
                    _rSwdmxMp4Info.u8AudLatestOff = 0xFFFFFFFFFFFFFFFF;
                }

                if (_rSwdmxMp4Info.prVidIdxTbl &&
                    _rSwdmxMp4Info.fgEnVideo &&
                    (prRangeList->u4CurRangeVidIdx < _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry))
                {
                    _rSwdmxMp4Info.u8VidLatestOff = 
                        _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                }
                else
                {
                    _rSwdmxMp4Info.u8VidLatestOff = 0xFFFFFFFFFFFFFFFF;
                }

                _rSwdmxMp4Info.u8CurFilepos = min(_rSwdmxMp4Info.u8VidLatestOff, _rSwdmxMp4Info.u8AudLatestOff);
            }
            
            _rPrevSample.ui4_relative_offset = _rSwdmxMp4Info.u8CurFilepos;
            _rPrevSample.ui4_size = 0;

            _SWDMX_Mp4ResetFeeder();
         }
        
        if ((_rSwdmxMp4Info.fgEnVideo) && (_rSwdmxMp4Info.fgEnAudio))
        {
            pu4VidCurIdx  = &prRangeList->u4CurRangeVidIdx;
            pu4AudCurIdx = &prRangeList->u4CurRangeAudIdx;

            if ((_rSwdmxMp4Info.prVidIdxTbl) &&
                (_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry) &&
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry))
            {
                u4VidOffset  = 
                    _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[*pu4VidCurIdx].ui4_relative_offset;
                u4AudOffset = 
                    _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_relative_offset;
            }
            else
            {
               if((_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry == 0))
               {
               	  u4AudOffset = 0xFFFFFFFF;
               }
               if((_rSwdmxMp4Info.prVidIdxTbl)&&
                (_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry == 0))
               {
               	  u4VidOffset = 0xFFFFFFFF;
               }
            }
                
            if ((_rSwdmxMp4Info.prVidIdxTbl)&&
                (prRangeList->u4CurRangeVidIdx >= _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry) &&
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                (prRangeList->u4CurRangeAudIdx >=_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry))
            {
                LOG(5, "A/V Range is done\n");
                _rSwdmxMp4Info.fgFeederInvalid = TRUE;
            }
            else if ((_rSwdmxMp4Info.prVidIdxTbl)&&
                (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                (prRangeList->u4CurRangeVidIdx < _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry) &&
                (prRangeList->u4CurRangeAudIdx <_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry))
            {
                fgRangeDone = FALSE;
                _rSwdmxMp4Info.fgPartialRangeDone = FALSE;

                if ((_rSwdmxMp4Info.prVidIdxTbl)&&(_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry)&&
                    ((u4VidOffset + _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[*pu4VidCurIdx].ui4_size) > (_rSwdmxMp4Info.u8_range_sz)) ||
                    ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                    (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                    (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry)&&
                    ((u4AudOffset +_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_size)> (_rSwdmxMp4Info.u8_range_sz))))
                {
                    LOG(5, "Video offset or Audio offset is larger than range size\n");
                    
                    if((_rSwdmxMp4Info.prVidIdxTbl) && (!_rSwdmxMp4Info.fgDeliverVEos))
                    {
                        if (_rSwdmxMp4Info.fgVDecIsPause)
                        {
                            _SWDMX_Mp4VideoWakeUp();
                            _rSwdmxMp4Info.fgVDecIsPause = FALSE;
                        }

                        if (_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                        {
                            _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                        }            
                    }
                    if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                        (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                        (!_rSwdmxMp4Info.fgDeliverAEos))
                    {
                        if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_AUDIO))
                        {
                            _rSwdmxMp4Info.fgDeliverAEos = TRUE;
                        } 
                        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
                    }
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }

                LOG(6, "Vid Block = %x Aud Block = %x Vid Offset = 0x%08x, Aud Offset = 0x%08x\n", 
                    _rSwdmxMp4Info.fgBlockVid,
                    _rSwdmxMp4Info.fgBlockAud,
                    u4VidOffset, u4AudOffset);

                if (_rSwdmxMp4Info.u4NumSentVid == 0)
                {
                    _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
                    //_rSwdmxMp4Info.u8CurFilepos = min(u4VidOffset, u4AudOffset);
                    //_rPrevSample.ui4_relative_offset = _rSwdmxMp4Info.u8CurFilepos;
                }

                // Check if video/audio is blocked or not
                if ((_rSwdmxMp4Info.fgBlockVid) &&
                    (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK))
                {
                    if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack],  
                        pu4AudCurIdx,   eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                else if (_rSwdmxMp4Info.fgBlockAud)
                {
                    if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prVidIdxTbl,  
                        pu4VidCurIdx,   eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                // Then check the file position.
                else if (u4VidOffset < u4AudOffset)
                {
                    if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prVidIdxTbl,  
                        pu4VidCurIdx,   eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
                else
                {
                    if (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK)
                    {
                        if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack],  
                            pu4AudCurIdx,   eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                        {
                            return eSWDMX_HDR_PRS_FAIL;
                        }
                    }
                    else
                    {
                        return eSWDMX_HDR_PRS_FAIL;
                    }
                }
            }
            else if ((_rSwdmxMp4Info.prVidIdxTbl)&&
                (prRangeList->u4CurRangeVidIdx < _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry))
            {
                fgRangeDone = FALSE;

                if(!_rSwdmxMp4Info.fgPartialRangeDone)
                {
                    LOG(5, "Audio Range is done\n");
                    _rSwdmxMp4Info.fgBlockVid = FALSE;
                    _rSwdmxMp4Info.fgBlockAud = TRUE;
                    _rSwdmxMp4Info.fgFeederInvalid = TRUE;
                    _rSwdmxMp4Info.fgPartialRangeDone = TRUE;
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
                
                pu4VidCurIdx  = &prRangeList->u4CurRangeVidIdx;
                if ((_rSwdmxMp4Info.prVidIdxTbl)&&(_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry)&&
                    ((u4VidOffset + _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[*pu4VidCurIdx].ui4_size)> (_rSwdmxMp4Info.u8_range_sz)))
                {
                    LOG(5, "Video Offset is larger than range size\n");
                    
                    if((_rSwdmxMp4Info.prVidIdxTbl) && (_rSwdmxMp4Info.fgDeliverVEos == FALSE))
                    {
                        if (_rSwdmxMp4Info.fgVDecIsPause)
                        {
                            _SWDMX_Mp4VideoWakeUp();
                            _rSwdmxMp4Info.fgVDecIsPause = FALSE;
                        }

                        if (_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                        {
                            _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                        }            
                    }
                    if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                        (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                        (_rSwdmxMp4Info.fgDeliverAEos == FALSE))
                    {
                        if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_AUDIO))
                        {        
                              _rSwdmxMp4Info.fgDeliverAEos = TRUE;
                        }
                        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
                    }
                  return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
               }

                if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prVidIdxTbl,  
                    pu4VidCurIdx,   eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                {
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
            else if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                (prRangeList->u4CurRangeAudIdx <_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry))
            {
                fgRangeDone = FALSE;

                if(!_rSwdmxMp4Info.fgPartialRangeDone)
                {
                    LOG(5, "Video Range is done\n");
                    _rSwdmxMp4Info.fgBlockVid = TRUE;
                    _rSwdmxMp4Info.fgBlockAud = FALSE;
                    _rSwdmxMp4Info.fgFeederInvalid = TRUE;
                    _rSwdmxMp4Info.fgPartialRangeDone = TRUE;
                    return eSWDMX_HDR_PRS_SUCCEED;
                }
               
                pu4AudCurIdx = &prRangeList->u4CurRangeAudIdx;
                if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                    (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                    (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry)&&
                    (u4AudOffset + _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_size) > (_rSwdmxMp4Info.u8_range_sz))
                {   
                    LOG(5, "Audio Offset is larger than range size\n");
                    
                    if((_rSwdmxMp4Info.prVidIdxTbl) && (_rSwdmxMp4Info.fgDeliverVEos == FALSE))
                    {
                        if (_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                        {
                            _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                        }            
                    }
                    if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                        (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                        (!_rSwdmxMp4Info.fgDeliverAEos))
                    {
                        if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_AUDIO))
                        {        
                            _rSwdmxMp4Info.fgDeliverAEos = TRUE;
                        }
                        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
                    }
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }
                if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack],  
                    pu4AudCurIdx,   eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                {
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
        }
        else if(_rSwdmxMp4Info.fgEnVideo)
        {         
            if (_rSwdmxMp4Info.u4NumSentVid == 0)
            {
                _SWDMX_BeforeDeliverUseIdx(eSWDMX_MEDIA_VIDEO);
            }
                
            if((_rSwdmxMp4Info.prVidIdxTbl) &&
                (prRangeList->u4CurRangeVidIdx < _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry))
            {
                fgRangeDone = FALSE;
                pu4VidCurIdx  = &prRangeList->u4CurRangeVidIdx;
                
                if ((_rSwdmxMp4Info.prVidIdxTbl)&&
                    (_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry))
                {
                    u4VidOffset  = 
                        _rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[prRangeList->u4CurRangeVidIdx].ui4_relative_offset;
                }
                else
                {
                    LOG(1, "Index table error\n");
                    return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
                }

                if ((_rSwdmxMp4Info.prVidIdxTbl)&&
                    (_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry)&&
                    ((u4VidOffset+_rSwdmxMp4Info.prVidIdxTbl->u.pt_pb_idx_tbl_entry[*pu4VidCurIdx].ui4_size)> (_rSwdmxMp4Info.u8_range_sz)))
                {
                    LOG(5, "Video Offset is larger than range size\n");
                    if((_rSwdmxMp4Info.prVidIdxTbl) && (_rSwdmxMp4Info.fgDeliverVEos == FALSE))
                    {
                        if (_rSwdmxMp4Info.fgVDecIsPause)
                        {
                            _SWDMX_Mp4VideoWakeUp();
                            _rSwdmxMp4Info.fgVDecIsPause = FALSE;
                        }

                        if (_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                        {
                            _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                        }            
                    }
                }

                if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prVidIdxTbl,  
                    pu4VidCurIdx,   eSWDMX_MEDIA_VIDEO) == eSWDMX_HDR_PRS_FAIL)
                {
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
        }
        else if(_rSwdmxMp4Info.fgEnAudio)
        {
            if((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack])&&
                (prRangeList->u4CurRangeAudIdx <_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry))
            {
                fgRangeDone = FALSE;
                pu4AudCurIdx = &prRangeList->u4CurRangeAudIdx;

                u4AudOffset = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_relative_offset;

                if((u4AudOffset + _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->u.pt_pb_idx_tbl_entry[*pu4AudCurIdx].ui4_size) > (_rSwdmxMp4Info.u8_range_sz))
                {
                    LOG(5, "Audio Offset is larger than range size\n");
                    if ((_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&(_rSwdmxMp4Info.fgDeliverAEos == FALSE))
                    {
                        if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_AUDIO))
                        {        
                            _rSwdmxMp4Info.fgDeliverAEos = TRUE;
                        }
                        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
                    }
                }
                if (_SWDMX_DeliverMp4DataUseIdx(i4Speed, _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack],  
                    pu4AudCurIdx,   eSWDMX_MEDIA_AUDIO) == eSWDMX_HDR_PRS_FAIL)
                {
                    return eSWDMX_HDR_PRS_FAIL;
                }
            }
        }

        if ((!fgRangeDone) &&
            (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING))
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
        }

        if (prRangeList->prDmxRangeInfo->b_endofstream)
        {
            if (fgRangeDone)
            {
                LOG(5, "Last Range is done set it as pending\n");
                fgRangeDone = FALSE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            
            if((_rSwdmxMp4Info.prVidIdxTbl) &&
                (_rSwdmxMp4Info.fgEnVideo) &&
                (prRangeList->u4CurRangeVidIdx == _rSwdmxMp4Info.prVidIdxTbl->ui4_number_of_entry) && 
                (!_rSwdmxMp4Info.fgDeliverVEos))
            {
                LOG(5, "_SWDMX_Mp4HdrPrsUseIdx Last Video Range\n");

                if (_rSwdmxMp4Info.fgVDecIsPause)
                {
                    _SWDMX_Mp4VideoWakeUp();
                    _rSwdmxMp4Info.fgVDecIsPause = FALSE;
                }

                if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                {
                     _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                }           
            }
            if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]) &&
                (_rSwdmxMp4Info.fgEnAudio) &&
                (prRangeList->u4CurRangeAudIdx == _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]->ui4_number_of_entry) &&
                (!_rSwdmxMp4Info.fgDeliverAEos))
            {
                LOG(5, "_SWDMX_Mp4HdrPrsUseIdx Last Audio Range\n");
                
                if(_SWDMX_SendMp4Eos(eSWDMX_MEDIA_AUDIO))
                {        
                    _rSwdmxMp4Info.fgDeliverAEos = TRUE;
                }
                AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
            }
        }

        if (!_rSwdmxMp4Info.fgFeederInvalid)
        {
            if (_SWDMX_Mp4RequestData() != eSWDMX_HDR_PRS_SUCCEED)
            {
                LOG(5, "Feeder fails to move data\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
        }

    }
     
    if (fgRangeDone)  
    {
        INT32 i;
        BOOL   fgPending = FALSE;

        if (_rSwdmxMp4Info.fgEnVideo)
        {
            UINT16 u2MaxQNum = 0; 
            UINT16 u2CurQNum = 0;

            VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
            if (u2CurQNum > (_rSwdmxMp4Info.u4VidFps/2))
            {
               LOG(5, "Range is pending\n");
               fgPending = TRUE;
            }
        }
        else
        {
            UINT32 u4CurPts = 0;
            UINT32 u4CurDmxPts = 0;
            
            u4CurPts = STC_GetStcValue();
            u4CurDmxPts = _rSwdmxMp4Info.u4CurAudPts;
     
            if ((u4CurDmxPts >= u4CurPts) &&
                ((u4CurDmxPts - u4CurPts)> MP4_ONE_SEC_DURATION))
            {
                LOG(5, "Range is pending\n");
                fgPending = TRUE;
            }
        }
        
        if(fgPending) 
        {   
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
	      else
	      {
            LOG(5, "Range is finished\n");
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
            _rSwdmxMp4Info.prVidIdxTbl = NULL;

            for (i = 0; (i < _rSwdmxMp4Info.u1TotalATrack) && (i < MP4_MAX_AUDIO_TRACK); i++)
            {
                _rSwdmxMp4Info.prAudIdxTbl[i] = NULL;
            }
            _rSwdmxMp4Info.u1TotalATrack = 0;

            _rSwdmxMp4Info.fgBlockVid = FALSE;
            _rSwdmxMp4Info.fgBlockAud = FALSE;
        } 
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}
        
static SWDMX_HDR_PRS_STATE_T _SWDMX_Mp4RequestData(void)
{
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;   
    UINT32 u4ReadSize = 0;
    UINT32 u4MaxDataByte = 0;

    u4MaxDataByte  = MP4_BUFFER_SIZE - MIN_READ_SIZE;
      
    if((_rSwdmxMp4Info.u4LeftBytesInFeeder < u4MaxDataByte) && 
        ((_rSwdmxMp4Info.u8CurFilepos) < _rSwdmxMp4Info.u8_range_sz))
    {
        u4ReadSize = min((_rSwdmxMp4Info.u8_range_sz - _rSwdmxMp4Info.u8CurFilepos), MIN_READ_SIZE);

        if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
            u4ReadSize, 
            (UINT64)(_rSwdmxMp4Info.u8CurFilepos),
            &rFeederCond,
            _rSwdmxMp4Info.u4FeederReqID++) == FEEDER_E_BUFFER_FULL)
        {
            LOG(3, "Feeder buffer is not enough\n");
            return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
        }
             
        _rSwdmxMp4Info.i4ReqReadTimes++;
                    
        if (!_SWDMX_Mp4GetDataFromFeeder(&rFeederReqData))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }    

        _rSwdmxMp4Info.i4ReqReadTimes--;
    
        if(_rSwdmxMp4Info.fgRepeatRequest)  
        {
            _rSwdmxMp4Info.u4CurDmxFeederRPtr = rFeederReqData.u4WriteAddr+ rFeederReqData.u4AlignOffset;

            if(_rSwdmxMp4Info.u4CurDmxFeederRPtr > VIRTUAL(_rFeederInfo.u4EndAddr))
            {
                _rSwdmxMp4Info.u4CurDmxFeederRPtr = _rSwdmxMp4Info.u4CurDmxFeederRPtr  + 
                VIRTUAL(_rFeederInfo.u4StartAddr)   - VIRTUAL(_rFeederInfo.u4EndAddr);
            }
            _rSwdmxMp4Info.fgRepeatRequest = FALSE;
        }
          
        _rSwdmxMp4Info.u8CurFilepos += rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset; 
        _rSwdmxMp4Info.u4LeftBytesInFeeder+= rFeederReqData.u4ReadSize -rFeederReqData.u4AlignOffset ;
    }
    else if ((_rSwdmxMp4Info.u8CurFilepos) >= _rSwdmxMp4Info.u8_range_sz)
    {
        if (_rSwdmxMp4Info.fgEnVideo && _rSwdmxMp4Info.fgEnAudio)
        {
            if (_rSwdmxMp4Info.fgDeliverVEos && _rSwdmxMp4Info.fgDeliverAEos)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            else
            {
                if (((!_rSwdmxMp4Info.fgPartialRangeDone)||
                    (!_rSwdmxMp4Info.fgDeliverVEos) ||( !_rSwdmxMp4Info.fgDeliverAEos))&&
                    (_rSwdmxMp4Info.fgFeederLeftNotEnough))
                {
                    _rSwdmxMp4Info.fgFeederInvalid = TRUE;
                }
                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        else if (_rSwdmxMp4Info.fgEnVideo && 
            (_rSwdmxMp4Info.fgDeliverVEos||_rSwdmxMp4Info.fgFeederLeftNotEnough))
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
        else if (_rSwdmxMp4Info.fgEnAudio && _rSwdmxMp4Info.fgDeliverAEos)
        {
            return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}


static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4Keyframe(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType,
    SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT32 u4DmxAvailSize = 0;

    DMX_MM_DATA_T rDmxMMData;
    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UINT32 u4WriteAddr = 0,u4NaluSize = 0, u4FrmSize = 0;
    UINT8  i = 0;

    UCHAR *pucFdrBuf = NULL;

    if(_rSwdmxMp4Info.fgFeederInvalid)
    {
        INT32 i;
        FEEDER_REQ_DATA_T rFeederReqData;
        
        for (i = 0; i < _rSwdmxMp4Info.i4ReqReadTimes ; i++)
        {
            _SWDMX_Mp4GetDataFromFeeder(&rFeederReqData);
        }
        
        _rSwdmxMp4Info.u4LeftBytesInFeeder = 0;
        _rSwdmxMp4Info.fgRepeatRequest = TRUE;
        _rSwdmxMp4Info.fgFeederInvalid = FALSE;
        _rSwdmxMp4Info.u4FeederReqID = 0;
        _rSwdmxMp4Info.i4ReqReadTimes = 0;

        _SWDMX_Mp4ResetFeeder();
         
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
     }

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    u4DmxAvailSize = DMX_MM_GetBufEmptySize(u1HdrType);

    LOG(6, "_rSwdmxMp4Info.u4CurSentKey = %ld Key Pts = 0x%08x Dmx available size = 0x%08x\n", 
        _rSwdmxMp4Info.u4CurSentKey,
        prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts,
        u4DmxAvailSize);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > u4DmxAvailSize)
    {
        LOG(7, "Demuxer Buffer is not enough Hdr type = %x u4DmxAvailSize = %ld  frame size = %ld.\n", 
            u1HdrType,
            u4DmxAvailSize,
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
        
        x_thread_delay(1);
        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    // Send sequence header for decoder's initialization?

    if (((*pu4CurIdx) < prIdxTbl->ui4_number_of_entry)&& !_rSwdmxMp4Info.fgDeliverVEos)
    {
        if (u1HdrType == eSWDMX_MEDIA_VIDEO)
        {
            //UCHAR *pBuf;
            UINT16 u2MaxQNum, u2CurQNum;
            VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
            LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);

            // Temporally use 30 for fps
            // Should use fps in the fucture.
            if (u2CurQNum > _rSwdmxMp4Info.u4VidFps)
            {
                x_thread_delay(1);
                return eSWDMX_HDR_PRS_SUCCEED;
            }

	      if ((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset 
		  	+ prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)> (_rSwdmxMp4Info.u8_range_sz))
            {
                LOG(5, "Video Offset is larger than range size\n");
                    
                if((_rSwdmxMp4Info.prVidIdxTbl) && (_rSwdmxMp4Info.fgDeliverVEos == FALSE))
                {
                    if (_rSwdmxMp4Info.fgVDecIsPause)
                    {
                        _SWDMX_Mp4VideoWakeUp();
                        _rSwdmxMp4Info.fgVDecIsPause = FALSE;					
                    }

                    if (_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                    {
                        _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                    }    
					
			              prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
               return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }
            LOG(7, "_rSwdmxMp4Info.u4NumSentVid %ld  ui4_size = 0x%08x\n", _rSwdmxMp4Info.u4NumSentVid, prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
            LOG(7, "ui4_relative_offset = %08x.\n", prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset);

            if (FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size, 
                (UINT64)(prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset),
                &rFeederCond,
                _rSwdmxMp4Info.u4FeederReqID++) == FEEDER_E_BUFFER_FULL)
            {
                LOG(3, "Feeder's buffer is full\n");
            }

            _rSwdmxMp4Info.i4ReqReadTimes++;

            if (!_SWDMX_Mp4GetDataFromFeeder(&rFeederReqData))
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }

            _rSwdmxMp4Info.i4ReqReadTimes--;

            if (rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset ||
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size > (rFeederReqData.u4ReadSize - rFeederReqData.u4AlignOffset))
            {
                LOG(3, "_SWDMX_DeliverMp4Keyframe rFeederReqData.u4ReadSize <= rFeederReqData.u4AlignOffset\n");
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }

            u4WriteAddr = VIRTUAL(rFeederReqData.u4WriteAddr + rFeederReqData.u4AlignOffset);
            _rSwdmxMp4Info.u4RecFeederWPtr = u4WriteAddr;
            _rSwdmxMp4Info.fgRequestData = FALSE;
            
            //replace length with start code            
            if (_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264)
            {
                UCHAR* pucBuf = NULL;
                
                pucFdrBuf = (UCHAR*)u4WriteAddr;
                pucBuf       = pucFdrBuf;
                u4FrmSize = 0;
                
                while(u4FrmSize < prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)
                {
                    u4NaluSize = 0;
                    //insert start code
                    for(i = 0; i < _rSwdmxMp4Info.u1StartCodeLenMinus1; i++)
                    {
                        u4NaluSize += (UINT32)*pucFdrBuf;
                        u4NaluSize <<= 8;
                        *pucFdrBuf++ = 0;
                        //check ring buffer
                        if((UINT32)pucFdrBuf == VIRTUAL(_rFeederInfo.u4EndAddr))
                        {
                            pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
                        }
                    }
                    u4NaluSize += (UINT32)*pucFdrBuf;
                    if (u4NaluSize == 0)
                    {
                	    (*pu4CurIdx)++;
                	    return eSWDMX_HDR_PRS_SUCCEED;
                    }

                    *pucFdrBuf++ = 1;
                    if((_rSwdmxMp4Info.u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                        (_rSwdmxMp4Info.u1StartCodeLenMinus1 > 0))
                    {
                         UINT32   u4Loop = 0;
                         UINT32   u4DataSize = 0;

                        HalFlushInvalidateDCache();
                         
                        for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN-
                                                     _rSwdmxMp4Info.u1StartCodeLenMinus1); u4Loop++)
                        {
                            if((UINT32)pucBuf == VIRTUAL(_rFeederInfo.u4StartAddr))
                            {
                                pucBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4EndAddr-1);
                            }
                            else
                            {
                                pucBuf--;
                            }
                            *pucBuf = 0;
                        }
                        u4DataSize = u4NaluSize + (H264_STARTCODE_LEN-
                                                                        _rSwdmxMp4Info.u1StartCodeLenMinus1)+
                                                                        (_rSwdmxMp4Info.u1StartCodeLenMinus1 + 1);
                        
                        rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
                        rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
                        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
                        rDmxMMData.u4FrameSize = u4DataSize;
                        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                        
                        u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO);
                        if (u4DataSize >=  u4DmxAvailSize)
                        {
                        	  _rSwdmxMp4Info.fgFeederInvalid = TRUE;
                            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
                        }
                        
                        if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move data.\n");
                            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                        }
                    }
                    //check ring buffer
                    if((UINT32)pucFdrBuf == VIRTUAL(_rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
                    }
                    // nalu size
                    u4FrmSize += ((_rSwdmxMp4Info.u1StartCodeLenMinus1 + 1) + u4NaluSize);
                    pucFdrBuf += u4NaluSize;
                    //check ring buffer
                    if((UINT32)pucFdrBuf >= VIRTUAL(_rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = 
                            (UCHAR*)((UINT32)pucFdrBuf - 
                            VIRTUAL(_rFeederInfo.u4EndAddr) + 
                            VIRTUAL(_rFeederInfo.u4StartAddr));
                    }
                    pucBuf = pucFdrBuf;
                }
                HalFlushInvalidateDCache();
            }

            _rSwdmxMp4Info.u4NumSentVid++;

            if(((UINT32)(_rSwdmxMp4Info.u8VSeekPts>>32) != 0xFFFFFFFF)
                || ((UINT32)(_rSwdmxMp4Info.u8VSeekPts) != 0xFFFFFFFF))
            {
                UINT32 u4Pts, u4CurSTC;
                
                u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                if(u4Pts > (UINT32)_rSwdmxMp4Info.u8VSeekPts)
                {
                    STC_SetStcValue(u4Pts - 18000);
                    u4CurSTC = STC_GetStcValue();                
                    LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n", 
                        u4Pts, u4CurSTC, (UINT32)_rSwdmxMp4Info.u8VSeekPts);            
                    _rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
                    _rSwdmxMp4Info.u8VSeekPts <<= 32;
                    _rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
                }
            }
        }

	  _rSwdmxMp4Info.u4LastVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
		
        if((_rSwdmxMp4Info.u4VDecType != ENUM_SWDMX_VDEC_H264)||
            ((_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264) && 
            (_rSwdmxMp4Info.u1StartCodeLenMinus1 == H264_STARTCODE_LEN)) )                        
        {
            rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
            rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
            rDmxMMData.u4StartAddr = u4WriteAddr;
            rDmxMMData.u4FrameSize = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
            rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

            if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
            {
                LOG(1, "Demuxer fails to move data.\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }


        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(u4WriteAddr));

        if (!_SWDMX_Mp4CalculateRepeatFrame(i4Speed))
        {
            if (i4Speed > 2000)
            {
                (*pu4CurIdx)++;
            }
            else if ((i4Speed < 0) &&
                (*pu4CurIdx > 0))
            {
                (*pu4CurIdx)--;
            }
        }

        // The last key frame
        if (_rSwdmxMp4Info.u4VidRepeatFrame == 0)
        {
            if ((i4Speed > 2000) &&
            (_rSwdmxMp4Info.u4CurSentKey == prIdxTbl->ui4_number_of_entry) &&
            (_rSwdmxMp4Info.u4CurRepeatFrameIdx == prIdxTbl->ui4_number_of_entry - 1) &&
            !_rSwdmxMp4Info.fgDeliverVEos)
            {
                LOG(5, "Fast Forward _SWDMX_Mp4HdrPrsUseIdx Deliver Video Eos\n");
                if (!_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                {
                    LOG(3, "Fail to send Video Eos\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            else if ((i4Speed < 0) &&
                        (_rSwdmxMp4Info.u4CurSentKey == 0) &&
                        (_rSwdmxMp4Info.u4CurRepeatFrameIdx == 0) &&
                        !_rSwdmxMp4Info.fgDeliverVEos)
            {
                if (!_SWDMX_SendMp4Eos(eSWDMX_MEDIA_VIDEO))
                {
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                }
                _rSwdmxMp4Info.fgDeliverVEos = TRUE;
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
        }
    }
    
    return eSWDMX_HDR_PRS_SUCCEED;
}

static BOOL _SWDMX_Mp4ResetFeeder()
{
     if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_AviResetFeeder Cur Fed's WPtr = 0x%08x\n", _rFeederInfo.u4WriteAddr);
    _rSwdmxMp4Info.u4CurDmxFeederRPtr = _rFeederInfo.u4WriteAddr ;

    // Update read ptr to feeder
    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxMp4Info.u4CurDmxFeederRPtr));
    return TRUE;
}

static SWDMX_HDR_PRS_STATE_T  _SWDMX_BeforeDeliverUseIdx(UINT8 u1HdrType)
{
       UCHAR *pucHdrBuf = NULL;
       UCHAR *pucFdrBuf = NULL;
       
       UINT8 u1NumOfPS = 0, u1PSNum = 0;

       DMX_MM_DATA_T rDmxMMData;

       UINT16 u2PSLength = 0;
       UINT8      i = 0;

       x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

       if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
        FEEDER_E_OK)
        {
              LOG(7, "Feeder input buffer error.\n");
             return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
        }

        if (_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264)
        {  
            pucHdrBuf = (UCHAR*)VIRTUAL(_rSwdmxMp4Info.u4VidHeaderPtr);
            pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
            pucHdrBuf += 4; //skip unused info
            _rSwdmxMp4Info.u1StartCodeLenMinus1 = (*pucHdrBuf++) & 0x3;
            for(u1PSNum = 0; u1PSNum < 2; u1PSNum++)    // 0 for SPS, 1 for PPS
            {
                if(u1PSNum == 0)
                {
                    u1NumOfPS = (*pucHdrBuf++) & 0x1f;
                }
                else
                {
                    u1NumOfPS = *pucHdrBuf++;
                }
                for (i = 0; i < u1NumOfPS; i++)
                {
                    u2PSLength = (UINT16)(*pucHdrBuf++);
                    u2PSLength <<= 8;
                    u2PSLength += (UINT16)(*pucHdrBuf++);

                    //handle ring buffer
                    if(VIRTUAL(_rFeederInfo.u4EndAddr) <= (((UINT32)pucFdrBuf + (UINT32)u2PSLength) + 4))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
                    }
                    //set move data parameter
                    rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
                    rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
                    rDmxMMData.u4StartAddr = (UINT32)pucFdrBuf;
                    rDmxMMData.u4FrameSize = (UINT32)u2PSLength + 4;    // 4 for start code

                    //insert start code
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 0;
                    *pucFdrBuf++ = 1;      
                    //copy SPS or PPS
                    x_memcpy((VOID*)pucFdrBuf, (VOID*)pucHdrBuf, u2PSLength);
                    pucFdrBuf += u2PSLength;
                    pucHdrBuf += u2PSLength;
                    HalFlushInvalidateDCache();
                    
                    if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                    {
                        LOG(1, "Demuxer fails to move sequence header.\n");
                        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                    }
                }
            }
        }
        else if (_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_MPEG4)
        {
            x_memcpy((VOID*)VIRTUAL(_rFeederInfo.u4StartAddr), 
                (VOID*)VIRTUAL(_rSwdmxMp4Info.u4VidHeaderPtr), 
                _rSwdmxMp4Info.u4VidHdrLength);

            rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
            rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
            rDmxMMData.u4StartAddr = VIRTUAL(_rFeederInfo.u4WriteAddr);
            rDmxMMData.u4FrameSize = _rSwdmxMp4Info.u4VidHdrLength;

            if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
            {
                LOG(1, "Demuxer fails to move sequence header.\n");
                return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
            }
        }
        _rSwdmxMp4Info.u4NumSentVid++;
       return eSWDMX_HDR_PRS_FED_MOVE_FAIL;

}

static SWDMX_HDR_PRS_STATE_T _SWDMX_DeliverMp4DataUseIdx(
    INT32 i4Speed,
    MM_RANGE_ELMT_IDXTBL_T *prIdxTbl, 
    UINT32* pu4CurIdx, 
    UINT8 u1HdrType)
{
    UINT32 u4DmxAvailSize          = 0;
    UINT32 u4DmxVdecAvailSize = 0;
    UINT32 u4DmxAdecAvailSize = 0;

    DMX_MM_DATA_T rDmxMMData;

    UINT32 u4NaluSize = 0, u4FrmSize = 0;

    UCHAR *pucFdrBuf = NULL;

    UINT32 u4WriteOffset = 0;

    int i = 0;
    
    MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T *prTmpSample;

    prTmpSample = &prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx];

    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    u4DmxVdecAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO);
    u4DmxAdecAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO);

    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size == 0)
    {
        (*pu4CurIdx)++;
        return eSWDMX_HDR_PRS_FAIL;
    }

    if(u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
          u4DmxAvailSize = u4DmxVdecAvailSize;
    }
    else
    {
          u4DmxAvailSize = u4DmxAdecAvailSize;
          
          if(_rSwdmxMp4Info.u4ADecType == ENUM_SWDMX_ADEC_AAC)
          {
              u4DmxAvailSize-=7;
          }
    }
   
    if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size >=  u4DmxAvailSize)
    {
        if (_rSwdmxMp4Info.fgVDecIsPause)
        {
            _SWDMX_Mp4VideoWakeUp();
            _rSwdmxMp4Info.fgVDecIsPause = FALSE;
        }

        if (u1HdrType == eSWDMX_MEDIA_AUDIO && 
            _rSwdmxMp4Info.fgEnVideo && 
            !_rSwdmxMp4Info.fgPartialRangeDone)
        {
            _rSwdmxMp4Info.fgBlockAud = TRUE;
            _rSwdmxMp4Info.fgBlockVid = FALSE;
            _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        }
        if (u1HdrType == eSWDMX_MEDIA_VIDEO && 
            _rSwdmxMp4Info.fgEnAudio &&
            !_rSwdmxMp4Info.fgPartialRangeDone)
        {
            _rSwdmxMp4Info.fgBlockAud = FALSE;
            _rSwdmxMp4Info.fgBlockVid = TRUE;
            _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        }

        if (u1HdrType == eSWDMX_MEDIA_AUDIO &&
            !_rSwdmxMp4Info.fgEnVideo &&
            _rSwdmxMp4Info.fgAudIsReady)
        {
            LOG(5, "Trigger audio by _SWDMX_DeliverMp4DataUseIdx\n");
            AUD_MMAoutEnable(0, TRUE);
            _rSwdmxMp4Info.fgAudIsReady = FALSE;
        }

        LOG(6, "Dmx Buffer is not enough. Header Type = %x\n", u1HdrType);
        x_thread_delay(1);

        return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
    }

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
        FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        UINT16 u2MaxQNum, u2CurQNum;

        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        if ((u2CurQNum > (u2MaxQNum - 256)) ||
            (u2CurQNum > (_rSwdmxMp4Info.u4VidFps * 10)))
        {
        	
            	if (_rSwdmxMp4Info.fgVDecIsPause)
            	{
        		      _SWDMX_Mp4VideoWakeUp();
                	_rSwdmxMp4Info.fgVDecIsPause = FALSE;
            	}
            
            	LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
            	x_thread_delay(1);
            	return eSWDMX_HDR_PRS_SUCCEED;
        }    

        if (_rSwdmxMp4Info.fgEnAudio &&
            (!_rSwdmxMp4Info.fgPartialRangeDone) &&
            (_rSwdmxMp4Info.u4CurVidPts > (_rSwdmxMp4Info.u4CurAudPts + MP4_ONE_SEC_DURATION)))
        {
            LOG(5, "Video is enough, send audio data in the same timeline\n");
            _rSwdmxMp4Info.fgBlockVid = TRUE;
            _rSwdmxMp4Info.fgBlockAud = FALSE;
            _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        }
        else if(_rSwdmxMp4Info.fgEnAudio &&
            (!_rSwdmxMp4Info.fgPartialRangeDone) &&
            (_rSwdmxMp4Info.u4CurVidPts > _rSwdmxMp4Info.u4CurAudPts))
        {
            _rSwdmxMp4Info.fgBlockVid = FALSE;
            _rSwdmxMp4Info.fgBlockAud = FALSE;
        }
    }

    if(prTmpSample->ui4_size > _rSwdmxMp4Info.u4LeftBytesInFeeder)
    {
        _rSwdmxMp4Info.fgFeederLeftNotEnough = TRUE;
        LOG(3, "left in feeder is not enough\n");       
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;         
    }
    else
    {
        _rSwdmxMp4Info.fgFeederLeftNotEnough = FALSE;
    }
    
    if (prTmpSample->ui4_relative_offset >= (_rPrevSample.ui4_relative_offset + _rPrevSample.ui4_size))
    {
        u4WriteOffset =  
            prTmpSample->ui4_relative_offset - (_rPrevSample.ui4_relative_offset + _rPrevSample.ui4_size);
    }
    else
    {
        _rSwdmxMp4Info.u8CurFilepos = prTmpSample->ui4_relative_offset;
        _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    if((u4WriteOffset + prTmpSample->ui4_size) > (_rSwdmxMp4Info.u4LeftBytesInFeeder))
    {
        LOG(3, "fgFeederInvalid , left byte too small\n");
        _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
    }

    _rSwdmxMp4Info.u4CurDmxFeederRPtr += u4WriteOffset;
    if(_rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(_rFeederInfo.u4EndAddr))
    {
         _rSwdmxMp4Info.u4CurDmxFeederRPtr = 
            _rSwdmxMp4Info.u4CurDmxFeederRPtr - 
            VIRTUAL(_rFeederInfo.u4EndAddr) + 
            VIRTUAL(_rFeederInfo.u4StartAddr);
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        //replace length with start code            
        if (_rSwdmxMp4Info.u4VDecType == ENUM_SWDMX_VDEC_H264)
        {
            UCHAR* pucBuf = NULL;
            
            pucFdrBuf = (UCHAR*)_rSwdmxMp4Info.u4CurDmxFeederRPtr ;
            pucBuf = pucFdrBuf;
            u4FrmSize = 0;
            
            if (_rSwdmxMp4Info.u1StartCodeLenMinus1 <= H264_STARTCODE_LEN)
            {
                while(u4FrmSize < prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size)
                {
                    u4NaluSize = 0;
                    //insert start code
                    for(i = 0; i < _rSwdmxMp4Info.u1StartCodeLenMinus1; i++)
                    {
                        u4NaluSize += (UINT32)*pucFdrBuf;
                        u4NaluSize <<= 8;
                        *pucFdrBuf++ = 0;
                        //check ring buffer
                        if((UINT32)pucFdrBuf == VIRTUAL(_rFeederInfo.u4EndAddr))
                        {
                            pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
                        }
                    }
                    u4NaluSize += (UINT32)*pucFdrBuf;
                    if (u4NaluSize == 0)
                    {
                        _rSwdmxMp4Info.u8VidLatestOff = 
                                 prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset + 
                                                         prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                        _rSwdmxMp4Info.u4NumSentVid++;
                        _rSwdmxMp4Info.u4CurVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                        _rSwdmxMp4Info.u4LeftBytesInFeeder -= 
                                            (u4WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);
                        _rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                       if(_rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                       {
                             _rSwdmxMp4Info.u4CurDmxFeederRPtr = 
                                         _rSwdmxMp4Info.u4CurDmxFeederRPtr - 
                                         VIRTUAL(_rFeederInfo.u4EndAddr) + 
                                         VIRTUAL(_rFeederInfo.u4StartAddr);
                       }
   
                       FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxMp4Info.u4CurDmxFeederRPtr));

                       x_memcpy(&_rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

                       (*pu4CurIdx)++;
                	      return eSWDMX_HDR_PRS_SUCCEED;
                    }
                    
                    *pucFdrBuf++ = 1;

                    if((_rSwdmxMp4Info.u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
                        (_rSwdmxMp4Info.u1StartCodeLenMinus1 > 0))
                    {
                        UINT32   u4Loop = 0;
                        UINT32   u4DataSize = 0;
                        
                        HalFlushInvalidateDCache();
                        for(u4Loop=0; u4Loop<(H264_STARTCODE_LEN -_rSwdmxMp4Info.u1StartCodeLenMinus1); u4Loop++)
                        {
                            if((UINT32)pucBuf == VIRTUAL(_rFeederInfo.u4StartAddr))
                            {
                                pucBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4EndAddr - 1);
                            }
                            else
                            {
                                pucBuf--;
                            }

                            *pucBuf = 0;
                        }

                        u4DataSize = u4NaluSize + (H264_STARTCODE_LEN-
                                            _rSwdmxMp4Info.u1StartCodeLenMinus1)+
                                            (_rSwdmxMp4Info.u1StartCodeLenMinus1 + 1);
                        
                        rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
                        rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
                        rDmxMMData.u4StartAddr = (UINT32)pucBuf;
                        rDmxMMData.u4FrameSize = u4DataSize;
                        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

                        u4DmxVdecAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO);
                        if (u4DataSize >=  u4DmxVdecAvailSize)
                        {
                            _rSwdmxMp4Info.fgFeederInvalid = TRUE;
                            return eSWDMX_HDR_DMX_BUF_NOT_ENOUGH;
                        }
                        
                        if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                        {
                            LOG(1, "Demuxer fails to move data.\n");
                            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                        }
                    }

                    //check ring buffer
                    if((UINT32)pucFdrBuf == VIRTUAL(_rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = (UCHAR*)VIRTUAL(_rFeederInfo.u4StartAddr);
                    }
                    // nalu size
                    u4FrmSize += ((_rSwdmxMp4Info.u1StartCodeLenMinus1 + 1) + u4NaluSize);
                    pucFdrBuf += u4NaluSize;
                    //check ring buffer
                    if((UINT32)pucFdrBuf >= VIRTUAL(_rFeederInfo.u4EndAddr))
                    {
                        pucFdrBuf = 
                        (UCHAR*)((UINT32)pucFdrBuf - 
                        VIRTUAL(_rFeederInfo.u4EndAddr) + 
                        VIRTUAL(_rFeederInfo.u4StartAddr));
                    }
                    pucBuf = pucFdrBuf;
                }
            }
            HalFlushInvalidateDCache();
        }

        if(((UINT32)(_rSwdmxMp4Info.u8VSeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(_rSwdmxMp4Info.u8VSeekPts) != 0xFFFFFFFF))
        {
            UINT32 u4Pts, u4CurSTC;
            
            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > (UINT32)_rSwdmxMp4Info.u8VSeekPts)
            {
                STC_SetStcValue(u4Pts - 18000);
                u4CurSTC = STC_GetStcValue();                
                LOG(1, "Got Vid T PTS 0x%x, STC 0x%x, Seek 0x%x\n", 
                    u4Pts, u4CurSTC, (UINT32)_rSwdmxMp4Info.u8VSeekPts);            
                _rSwdmxMp4Info.u8VSeekPts = 0xFFFFFFFF;
                _rSwdmxMp4Info.u8VSeekPts <<= 32;
                _rSwdmxMp4Info.u8VSeekPts += 0xFFFFFFFF;
            }
        }
    }
        
    if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {  
        if (_rSwdmxMp4Info.fgEnVideo &&
            (!_rSwdmxMp4Info.fgPartialRangeDone) &&
            (_rSwdmxMp4Info.u4CurAudPts > (_rSwdmxMp4Info.u4CurVidPts + MP4_ONE_SEC_DURATION)))
        {
            LOG(5, "Audio is enough, send video data in the same timeline\n");
            _rSwdmxMp4Info.fgBlockVid = FALSE;
            _rSwdmxMp4Info.fgBlockAud = TRUE;
            _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        }
        else if (_rSwdmxMp4Info.fgEnVideo &&
            (!_rSwdmxMp4Info.fgPartialRangeDone) && 
            (_rSwdmxMp4Info.u4CurAudPts > _rSwdmxMp4Info.u4CurVidPts))
        {
        		 _rSwdmxMp4Info.fgBlockVid = FALSE;
             _rSwdmxMp4Info.fgBlockAud = FALSE;
        } 

        if(((UINT32)(_rSwdmxMp4Info.u8ASeekPts>>32) != 0xFFFFFFFF)
            || ((UINT32)(_rSwdmxMp4Info.u8ASeekPts) != 0xFFFFFFFF))
        {
            UINT32 u4Pts;

            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > (UINT32)_rSwdmxMp4Info.u8ASeekPts)
            {
                AUD_SetStartPts(0, u4Pts);         

                if (_rSwdmxMp4Info.fgEnVideo)
                {
                    _rSwdmxMp4Info.u4TriggerAudPts = (UINT32)u4Pts;
                }	  

                LOG(7, "Got Aud T PTS 0x%x, Seek 0x%x\n", 
                u4Pts, (UINT32)_rSwdmxMp4Info.u8ASeekPts);            
                _rSwdmxMp4Info.u8ASeekPts = 0xFFFFFFFF;
                _rSwdmxMp4Info.u8ASeekPts <<= 32;
                _rSwdmxMp4Info.u8ASeekPts += 0xFFFFFFFF;
            }
            else
            {
                _rSwdmxMp4Info.u8AudLatestOff = 
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                _rSwdmxMp4Info.u4LeftBytesInFeeder -= 
                (u4WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

                _rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                if(_rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                {
                    _rSwdmxMp4Info.u4CurDmxFeederRPtr = 
                    _rSwdmxMp4Info.u4CurDmxFeederRPtr - 
                    VIRTUAL(_rFeederInfo.u4EndAddr) + 
                    VIRTUAL(_rFeederInfo.u4StartAddr);
                } 
                FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxMp4Info.u4CurDmxFeederRPtr));		
                //record the previous sample data
                x_memcpy(&_rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

                (*pu4CurIdx)++;

                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }	
        if(_rSwdmxMp4Info.fgSeekAfterPause)
        {
            UINT32 u4Pts = 0;
            UINT32 u4PlayPts = VDP_GetPts(0);

            u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
            if(u4Pts > u4PlayPts)
            {
                AUD_SetStartPts(0, u4Pts);         

                if (_rSwdmxMp4Info.fgEnVideo)
                {
                    _rSwdmxMp4Info.u4TriggerAudPts = (UINT32)u4Pts;
                }
                _rSwdmxMp4Info.fgSeekAfterPause = FALSE;
            }
            else
            {
                _rSwdmxMp4Info.u8AudLatestOff = 
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
                prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                _rSwdmxMp4Info.u4LeftBytesInFeeder -= 
                (u4WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

                _rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
                if(_rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(_rFeederInfo.u4EndAddr))
                {
                    _rSwdmxMp4Info.u4CurDmxFeederRPtr = 
                    _rSwdmxMp4Info.u4CurDmxFeederRPtr - 
                    VIRTUAL(_rFeederInfo.u4EndAddr) + 
                    VIRTUAL(_rFeederInfo.u4StartAddr);
                } 
                FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxMp4Info.u4CurDmxFeederRPtr));		
                //record the previous sample data
                x_memcpy(&_rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));

                (*pu4CurIdx)++;

                return eSWDMX_HDR_PRS_SUCCEED;
            }
        }
        if (_rSwdmxMp4Info.u4ADecType == ENUM_SWDMX_ADEC_AAC)
        {
            UINT8 u1SampleRateIdx = 3;
            if(_pucAudBuf == 0)
            {
                LOG(1, "pucAudBuf should be allocated in init function");
                _pucAudBuf = 
                    (UCHAR*)VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(AAC_ALLOC_SIZE,16));                           
            }                               
            if (_pucAudBuf)
            {
                /* 8b: syncword */
                _pucAudBuf[0] = 0xFF;
                /* 4b: syncword */
                /* 1b: mpeg id = 1 */
                /* 2b: layer = 0 */
                /* 1b: protection absent */
                _pucAudBuf[1] = 0xF9;
                /* 2b: profile */
                /* 4b: sampling_frequency_index */
                /* 1b: private = 0 */
                /* 1b: channel_configuration */
                switch (_rSwdmxMp4Info.u4AudSampleRate)
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

                _pucAudBuf[2] = (1 << 6) |
                         ((u1SampleRateIdx<< 2) & 0x3C) |
                         ((_rSwdmxMp4Info.u4AudChannelNs >> 2) & 0x1);
                /* 2b: channel_configuration */
                /* 1b: original */
                /* 1b: home */
                /* 1b: copyright_id */
                /* 1b: copyright_id_start */
                /* 2b: aac_frame_length */
                _pucAudBuf[3] = ((_rSwdmxMp4Info.u4AudChannelNs & 0x3) << 6) |
                         (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) >> 11) & 0x3);
                /* 8b: aac_frame_length */
                _pucAudBuf[4] = (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) >> 3) & 0xFF);
                /* 3b: aac_frame_length */
                /* 5b: adts_buffer_fullness */
                _pucAudBuf[5] = (((prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size + AAC_FAKE_HDR_SIZE) << 5) & 0xE0) |
                         ((0x7FF >> 6) & 0x1F);
                /* 6b: adts_buffer_fullness */
                /* 2b: number_of_raw_data_blocks_in_frame */
                _pucAudBuf[6] = ((0x7FF << 2) & 0xFC);
                rDmxMMData.u4BufStart = (UINT32)_pucAudBuf;
                rDmxMMData.u4BufEnd = rDmxMMData.u4BufStart + AAC_ALLOC_SIZE;
                rDmxMMData.u4StartAddr = rDmxMMData.u4BufStart;
                rDmxMMData.u4FrameSize = 7;
                rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
                
                if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move data.\n");
                    return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
                } 
            }
        }
    }

    if(u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
	    _rSwdmxMp4Info.u4LastVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
    }

    if(!((u1HdrType == eSWDMX_MEDIA_VIDEO) &&
        (_rSwdmxMp4Info.u1StartCodeLenMinus1 < H264_STARTCODE_LEN) &&
        (_rSwdmxMp4Info.u1StartCodeLenMinus1 > 0)))
    {
        rDmxMMData.u4BufStart = VIRTUAL(_rFeederInfo.u4StartAddr);
        rDmxMMData.u4BufEnd = VIRTUAL(_rFeederInfo.u4EndAddr);
        rDmxMMData.u4StartAddr = _rSwdmxMp4Info.u4CurDmxFeederRPtr;
        rDmxMMData.u4FrameSize = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        rDmxMMData.u4Pts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;

        if (DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
        {
            LOG(1, "Demuxer fails to move data.\n");
            return eSWDMX_HDR_PRS_DMX_MOVE_FAIL;
        }
    }

    if((u1HdrType == eSWDMX_MEDIA_VIDEO) && 
        (_rSwdmxMp4Info.u8VidLatestOff <= prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset))
    {
        _rSwdmxMp4Info.u8VidLatestOff = 
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset + 
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        _rSwdmxMp4Info.u4NumSentVid++;
		
	      if (prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts > 
			     _rSwdmxMp4Info.u4CurVidPts)
        {
	          _rSwdmxMp4Info.u4CurVidPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
	      }  
		
        LOG(6, "_rSwdmxMp4Info.u4CurVidPts = 0x%08x\n", _rSwdmxMp4Info.u4CurVidPts);
    }
    else if(u1HdrType == eSWDMX_MEDIA_AUDIO && 
        (_rSwdmxMp4Info.u8AudLatestOff  <= prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset) )
    {
        _rSwdmxMp4Info.u8AudLatestOff = 
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_relative_offset +
            prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
        _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_pts;
        //_rSwdmxMp4Info.u4NumSentAud++;
        LOG(7, "_rSwdmxMp4Info.u4CurAudPts = 0x%08x\n", _rSwdmxMp4Info.u4CurAudPts);
    }
                                                                    
    _rSwdmxMp4Info.u4LeftBytesInFeeder -= 
        (u4WriteOffset + prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size);

    _rSwdmxMp4Info.u4CurDmxFeederRPtr += prIdxTbl->u.pt_pb_idx_tbl_entry[*pu4CurIdx].ui4_size;
    if(_rSwdmxMp4Info.u4CurDmxFeederRPtr >= VIRTUAL(_rFeederInfo.u4EndAddr))
    {
        _rSwdmxMp4Info.u4CurDmxFeederRPtr = 
            _rSwdmxMp4Info.u4CurDmxFeederRPtr - 
            VIRTUAL(_rFeederInfo.u4EndAddr) + 
            VIRTUAL(_rFeederInfo.u4StartAddr);
    }
   
   FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, VIRTUAL(_rSwdmxMp4Info.u4CurDmxFeederRPtr));

    //record the previous sample data
    x_memcpy(&_rPrevSample, prTmpSample, sizeof(MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T));
           
    (*pu4CurIdx)++;

   return eSWDMX_HDR_PRS_SUCCEED;
}

BOOL _SWDMX_Mp4SetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    UNUSED(u4StrmSubID);
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxMp4Info.u4VStrmID = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxMp4Info.u4AStrmID = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_Mp4EnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    //BOOL fgFindATrack;
    //INT32 i;
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxMp4Info.fgEnVideo = TRUE;
            if (_rSwdmxMp4Info.fgEnVDmx == FALSE)
            {
                _SWDMX_InitDmx(eSWDMX_MEDIA_VIDEO, FALSE);
                _rSwdmxMp4Info.fgEnVDmx = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if (_rSwdmxMp4Info.fgEnADmx == FALSE)
            {
                _SWDMX_InitDmx(eSWDMX_MEDIA_AUDIO, FALSE);
                _rSwdmxMp4Info.fgEnADmx = TRUE;
            }
            /*fgFindATrack = FALSE;
            for (i = 0; i < _rSwdmxMp4Info.u1TotalATrack; i++)
            {
                if (_rSwdmxMp4Info.prAudIdxTbl[i] && 
                    _rSwdmxMp4Info.prAudIdxTbl[i]->t_strm_id.u.t_stm_mp4_id == _rSwdmxMp4Info.u4AStrmID)
                {
                    _rSwdmxMp4Info.u1CurATrack = i;
                    fgFindATrack = TRUE;
                }
            }
            if (!fgFindATrack)
            {
                LOG(3, "_SWDMX_Mp4EnableStrm didn't find audio stream\n");
                return FALSE;
            }
            if (_rSwdmxMp4Info.u4NumSentAud != 0)
            {
                _rSwdmxMp4Info.fgChgATrack = TRUE;
            }*/
            if ((_rSwdmxMp4Info.u4PrevAStrmID != 0xFFFFFFFF)&&
                 (_rSwdmxMp4Info.u4PrevAStrmID != _rSwdmxMp4Info.u4AStrmID))
            {
                _rSwdmxMp4Info.fgChgATrack = TRUE;
            }
            else if (_rSwdmxMp4Info.fgAudStrmDisable && !_rSwdmxMp4Info.fgRecSeekFlag&&
            	!_rSwdmxMp4Info.fgBeginToPlay)
            {
                LOG(5, "Audio stream is disabled and seek is not performed\n");
                _rSwdmxMp4Info.fgChgATrack = TRUE;
                _rSwdmxMp4Info.fgAudStrmDisable = FALSE;
            }
            if(!_rSwdmxMp4Info.fgRecSeekFlag)
            {	
            		_rSwdmxMp4Info.fgWaitNewRange =  FALSE;
            }  
            _rSwdmxMp4Info.fgRecSeekFlag = FALSE;
            _rSwdmxMp4Info.u4PrevAStrmID = _rSwdmxMp4Info.u4AStrmID;
            _rSwdmxMp4Info.fgEnAudio = TRUE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}


BOOL _SWDMX_Mp4DisableStrm(UINT8 u1StrmType)
{
    DMX_MM_T rPid;

    rPid.fgEnable = FALSE;
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:   
            _rSwdmxMp4Info.fgEnVideo = FALSE;
            if (_rSwdmxMp4Info.fgEnVDmx == TRUE)
            {
                _rSwdmxMp4Info.fgEnVDmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
            }
            if(_rSwdmxMp4Info.fgVDecIsPause)
            {
                _SWDMX_Mp4VideoWakeUp();
                _rSwdmxMp4Info.fgVDecIsPause = FALSE;
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxMp4Info.fgEnAudio = FALSE;
            if (_rSwdmxMp4Info.fgEnADmx == TRUE)
            {
                _rSwdmxMp4Info.fgEnADmx = FALSE;
                VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
                DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
            }
            _rSwdmxMp4Info.fgAudStrmDisable = TRUE;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

static BOOL _SWDMX_InitDmx(UINT8 u1HdrType, BOOL fgEos)
{
    BOOL fgRet = FALSE;
    UINT32 u4Flag = 0; 
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        if (!_rSwdmxMp4Info.fgEnVDmx)
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
            rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
        }

        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        u4Flag |= DMX_MM_FLAG_SEARCH_START_CODE;

        switch (_rSwdmxMp4Info.u4VDecType)
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

        fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
    
        u4Flag = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
          DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);
        
        
        rDmxMMInfo.fgEnable = TRUE;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
        
#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
             &au4BufEnd[1]) != AUD_OK)
#else
        if (AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
        {
                LOG(1, "_SWDMX_SetAudioStreamID Can't get audio buffers!\n");
                return FALSE;
        }
            
        rDmxMMInfo.fgAllocBuf = FALSE;
        
        rDmxMMInfo.u4BufAddr = au4BufStart[0];
        rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];

        _rSwdmxMp4Info.u4AudBufSize = rDmxMMInfo.u4BufSize;

        if (fgEos)
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            rDmxMMInfo.fgSearchStartCode = FALSE;
        }

        fgRet = DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);
    }

    return fgRet;
}


static BOOL _SWDMX_SendMp4Eos(UINT8 u1HdrType)
{
    UINT16 u2MaxQNum, u2CurQNum;
    //UINT32 u4DummyData[4];
    DMX_MM_DATA_T rDmxMMData;

    LOG(5, "SWDMX Send Mp4 Eos Hdr Type = %d\n", u1HdrType);

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        VDEC_GetQueueInfo(0, &u2CurQNum, &u2MaxQNum);
        LOG(7, "u2CurQNum = %ld, u2MaxQNum = %ld .\n", u2CurQNum, u2MaxQNum);
        //if (u2CurQNum > (_rSwdmxAviInfo.u4VidFps/2))
        if (u2CurQNum > (u2MaxQNum - 10))
        {
            x_thread_delay(1);
            return FALSE;
        }
    }
    
    x_memset(&rDmxMMData, 0, sizeof(DMX_MM_DATA_T));

    if (_SWDMX_InitDmx(u1HdrType, TRUE) == FALSE)
    {
        return FALSE;
    }

    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4FrameSize = 4;
    rDmxMMData.fgEOS = TRUE;

    return DMX_MM_MoveData(u1HdrType, &rDmxMMData, 500);
}


/**
*/
BOOL _SWDMX_Mp4Stop(VOID)
{
    BOOL fgEnVDmx = _rSwdmxMp4Info.fgEnVDmx;
    BOOL fgEnADmx = _rSwdmxMp4Info.fgEnADmx;

    x_memset(&_rSwdmxMp4Info, 0, sizeof(SWDMX_MP4_INFO_T));

    _rSwdmxMp4Info.fgEnVDmx = fgEnVDmx;
    _rSwdmxMp4Info.fgEnADmx = fgEnADmx;

    return TRUE;
}


static UINT32 _SWDMX_Mp4Seek(UINT64 u8SeekTime, 
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed)
{
    //find previous key's PTS
    //if within cur range, 
    // 1. prRangeList->fgIsSeekTarget will be TRUE, 
    // 2. range idx will set to the entry with target PTS
    //else, update new range according to target PTS
    INT32 i;
    UINT32 u4TargetPTS = 0xFFFFFFFF, u4StartPTS, u4EndPTS;
    MM_RANGE_ELMT_IDXTBL_T* prIdxTbl;   //current range
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl = _rSwdmxMp4Info.prVidKeyIdxTbl;

    prRangeList->fgIsSeekTarget = FALSE;

    if ((_rSwdmxMp4Info.prVidKeyIdxTbl) &&
        (_rSwdmxMp4Info.prVidIdxTbl) &&
        (_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
        (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
    {
        for (i = 0; i < _rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry; i++)
        {
            if (_rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
            {
                break;
            }
        }

        // Error Handling 
        // i = 0 is impossible
        if (i > 0)
        {
            _rSwdmxMp4Info.u4CurSentKey = i - 1;
        }
        else
        {
            _rSwdmxMp4Info.u4CurSentKey= 0;
        }

        if (i4PlaySpeed > 2000)
        {
            _rSwdmxMp4Info.u4CurRepeatFrameIdx = _rSwdmxMp4Info.u4CurSentKey + 1;
            _rSwdmxMp4Info.u4CurSentKey++;
        }
        else
        {
            _rSwdmxMp4Info.u4CurRepeatFrameIdx = _rSwdmxMp4Info.u4CurSentKey;
        }

        // check if target key frame PTS is in current range
        _rSwdmxMp4Info.u4LastVidPts = prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts;
        u4TargetPTS = prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts;
        prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
        u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        if((u4TargetPTS >= u4StartPTS) && (u4TargetPTS <= u4EndPTS))
        {
            LOG(5, "u4StartPTS = 0x%08x u4EndPts = 0x%08x u4TargetPTS = 0x%08x\n", 
                u4StartPTS, 
                u4EndPTS,
                u4TargetPTS);

            prRangeList->fgIsSeekTarget = TRUE;

            //set video range idx to the entry with target PTS
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == u4TargetPTS)
                {
                    prRangeList->u4CurRangeVidIdx = i;
                    _rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
                    break;
                }
            }

            // set audio range idx
            prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
                {
                    prRangeList->u4CurRangeAudIdx = i;
                    _rSwdmxMp4Info.fgHasAud = TRUE;
                    _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
                    LOG(5, "Audio PTS = 0x%08x\n", _rSwdmxMp4Info.u4CurAudPts);
                    break;
                }
            }

            if (!_rSwdmxMp4Info.fgHasAud)
            {
                prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
            }
           
            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
                _rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
        }
    }
    else if (_rSwdmxMp4Info.prVidKeyIdxTbl &&
        _rSwdmxMp4Info.prVidIdxTbl)
    {
        for (i = 0; i < _rSwdmxMp4Info.prVidKeyIdxTbl->ui4_number_of_entry; i++)
        {
            if (_rSwdmxMp4Info.prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts > u8SeekTime)
            {
                break;
            }
        }

        // Error Handling 
        // i = 0 is impossible
        if (i > 0)
        {
            _rSwdmxMp4Info.u4CurSentKey = i - 1;
        }
        else
        {
            _rSwdmxMp4Info.u4CurSentKey= 0;
        }

        if (i4PlaySpeed > 2000)
        {
            _rSwdmxMp4Info.u4CurRepeatFrameIdx = _rSwdmxMp4Info.u4CurSentKey + 1;
            _rSwdmxMp4Info.u4CurSentKey++;
        }
        else
        {
            _rSwdmxMp4Info.u4CurRepeatFrameIdx = _rSwdmxMp4Info.u4CurSentKey;
        }

        // check if target key frame PTS is in current range
        u4TargetPTS = prVidKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurSentKey].ui4_pts;
        prIdxTbl = _rSwdmxMp4Info.prVidIdxTbl;
        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
        u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        if((u4TargetPTS >= u4StartPTS) && (u4TargetPTS <= u4EndPTS))
        {
            LOG(5, "u4StartPTS = 0x%08x u4EndPts = 0x%08x u4TargetPTS = 0x%08x\n", 
                u4StartPTS, 
                u4EndPTS,
                u4TargetPTS);

            prRangeList->fgIsSeekTarget = TRUE;

            //set video range idx to the entry with target PTS
            for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
            {
                if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == u4TargetPTS)
                {
                    prRangeList->u4CurRangeVidIdx = i;
                    _rSwdmxMp4Info.u4CurVidPts = u4TargetPTS;
                    break;
                }
            }

            LOG(2, "_SWDMX_Mp4Seek Key = %ld, Key PTS = %ld\n", 
                _rSwdmxMp4Info.u4CurSentKey, u4TargetPTS);
        }

    }
    else if ((_rSwdmxMp4Info.u1CurATrack < MP4_MAX_AUDIO_TRACK) &&
                (_rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack]))
    {
        u4TargetPTS = (UINT32)u8SeekTime;
        // set audio range idx
        prIdxTbl = _rSwdmxMp4Info.prAudIdxTbl[_rSwdmxMp4Info.u1CurATrack];
           
        u4StartPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[0].ui4_pts;
        u4EndPTS = prIdxTbl->u.pt_pb_idx_tbl_entry[prIdxTbl->ui4_number_of_entry - 1].ui4_pts;
        
        for(i = 0; i < prIdxTbl->ui4_number_of_entry; i++)
        {
            if(prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= u8SeekTime)
            {
                prRangeList->u4CurRangeAudIdx = i;
                _rSwdmxMp4Info.fgHasAud = TRUE;
                prRangeList->fgIsSeekTarget = TRUE;
                _rSwdmxMp4Info.u4CurAudPts = prIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts;
                break;
            }
        }
        
        if((u8SeekTime < u4StartPTS)&&(!_rSwdmxMp4Info.fgEnVideo)&&(!_rSwdmxMp4Info.fgWaitNewRange))
        {
                _rSwdmxMp4Info.fgHasAud = FALSE;
                prRangeList->fgIsSeekTarget = FALSE;
        }
        
        if (!_rSwdmxMp4Info.fgHasAud)
        {
            prRangeList->u4CurRangeAudIdx = prIdxTbl->ui4_number_of_entry;
        }

    }

    return u4TargetPTS;
}


static BOOL _SWDMX_Mp4GetDataFromFeeder(FEEDER_REQ_DATA_T *prFeederData)
{
    if (!_SWDMX_ReceiveFeederAck((VOID*)prFeederData))
    {
        LOG(3, "_SWDMX_Mp4GetDataFromFeeder FeederTimeOut!!!\n");
        _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return FALSE;        
    }

    if (prFeederData->eDataType == FEEDER_SOURCE_INVALID)
    {
         LOG(3, "_SWDMX_Mp4GetDataFromFeeder invalid\n");
         _rSwdmxMp4Info.fgFeederInvalid = TRUE;
         return FALSE;
    }
    
    if ((prFeederData->u4Id + 1) != _rSwdmxMp4Info.u4FeederReqID)
    {
        LOG(3, "Feeder ID mismatch!!! feeder id = %ld\n", prFeederData->u4Id);
        _rSwdmxMp4Info.fgFeederInvalid = TRUE;
        return FALSE;
    }


    return TRUE;
}


static BOOL _SWDMX_Mp4CalculateRepeatFrame(INT32 i4PlaySpeed)
{
    UINT32 u4CurKey = _rSwdmxMp4Info.u4CurSentKey;
    MM_RANGE_ELMT_IDXTBL_T *prKeyIdxTbl = _rSwdmxMp4Info.prVidKeyIdxTbl;

    if (_rSwdmxMp4Info.u4VidRepeatFrame == 0)
    {
        if (_rSwdmxMp4Info.u4CurRepeatFrameIdx != u4CurKey &&
            (_rSwdmxMp4Info.u4CurRepeatFrameIdx < prKeyIdxTbl->ui4_number_of_entry))
        {
            UINT32 u4PtsDif = 0, u4MaxRepeatFrame;
    
            u4MaxRepeatFrame = (_rSwdmxMp4Info.u4VidFps * 1000 * 2)/abs(i4PlaySpeed);

            if (i4PlaySpeed > 2000)
            {
                if ((u4CurKey < (prKeyIdxTbl->ui4_number_of_entry)) &&
                    (prKeyIdxTbl->ui4_number_of_entry > MP4_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurRepeatFrameIdx].ui4_pts;

                    _rSwdmxMp4Info.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * _rSwdmxMp4Info.u4VidFps)/(90 * i4PlaySpeed));
                }
                else
                {
                    _rSwdmxMp4Info.u4VidRepeatFrame = (_rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }
            else if (i4PlaySpeed < 0)
            {
                if ((prKeyIdxTbl->ui4_number_of_entry > MP4_MINIMUM_KEY_NUM))
                {
                    u4PtsDif = prKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxMp4Info.u4CurRepeatFrameIdx].ui4_pts -
                        prKeyIdxTbl->u.pt_pb_idx_tbl_entry[u4CurKey].ui4_pts;

                    _rSwdmxMp4Info.u4VidRepeatFrame = 
                        (UINT32)((u4PtsDif * _rSwdmxMp4Info.u4VidFps)/(90 * abs(i4PlaySpeed)));
                }
                else
                {
                    _rSwdmxMp4Info.u4VidRepeatFrame = (_rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                }
            }

            _rSwdmxMp4Info.u4VidRepeatFrame = (u4MaxRepeatFrame > _rSwdmxMp4Info.u4VidRepeatFrame) ?
                (_rSwdmxMp4Info.u4VidRepeatFrame) : u4MaxRepeatFrame;

            if ((u4CurKey == prKeyIdxTbl->ui4_number_of_entry - 1) ||
                (u4CurKey == 0))
            {
                _rSwdmxMp4Info.u4VidRepeatFrame = 
                    (MP4_FIX_REPEAT_FRAME/(abs(i4PlaySpeed)/1000));

                if (_rSwdmxMp4Info.u4VidRepeatFrame > MP4_MAX_REPEAT_FRAME)
                {
                    _rSwdmxMp4Info.u4VidRepeatFrame = MP4_MAX_REPEAT_FRAME;
                }
                else if (_rSwdmxMp4Info.u4VidRepeatFrame < MP4_MIN_REPEAT_FRAME)
                {
                    _rSwdmxMp4Info.u4VidRepeatFrame = MP4_MIN_REPEAT_FRAME;
                }                        
            }

            LOG(5, "u4PtsDif = %ld Current Idx = %ld Repeat frame = %ld\n", 
                u4PtsDif, u4CurKey, _rSwdmxMp4Info.u4VidRepeatFrame);

            if (_rSwdmxMp4Info.u4VidRepeatFrame == 0)
            {
                return FALSE;
            }

            _rSwdmxMp4Info.u4CurRepeatFrameIdx = u4CurKey;
            LOG(5, "Repeat Idx = %ld\n", _rSwdmxMp4Info.u4CurRepeatFrameIdx);

            return TRUE;
        }
        else
        {
            // Force to play the first key frame
            if (_rSwdmxMp4Info.fgStartTrick)
            {
                _rSwdmxMp4Info.fgStartTrick = FALSE;
                _rSwdmxMp4Info.u4VidRepeatFrame = (_rSwdmxMp4Info.u4VidFps * 1000)/abs(i4PlaySpeed);
                return TRUE;
            }
            
            return FALSE;
        }
    }

    _rSwdmxMp4Info.u4VidRepeatFrame--;
    return TRUE;
}


BOOL _SWDMX_Mp4AudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (_rSwdmxMp4Info.fgEnVideo)
        {
            LOG(5, "SWDMX_AviAudioCb Audio is ready\n");
            if(_rSwdmxMp4Info.u4TriggerAudPts != 0xFFFFFFFF)
            {
            	VDP_TriggerAudReceive(_rSwdmxMp4Info.u4TriggerAudPts);
            	LOG(5, "SWDMX_Mp4AudioCb VDP Trigger PTS = 0x%08x\n", 
                _rSwdmxMp4Info.u4TriggerAudPts);
            	_rSwdmxMp4Info.u4TriggerAudPts = 0xFFFFFFFF;
          	}

            if(_rSwdmxMp4Info.fgVDecIsPause)
            {
                _SWDMX_Mp4VideoWakeUp();
                _rSwdmxMp4Info.fgVDecIsPause = FALSE;
            }
            _rSwdmxMp4Info.fgAudIsReady = TRUE;
        }
        else 
        {
            // Audio Only case
            LOG(5, "Trigger audio by _SWDMX_Mp4AudioCb\n");
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }
    }
    else
    {
        // impossible
        LOG(1, "_SWDMX_Mp4AudioCb eAudioNotifyType != ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE\n");
    }

    return TRUE;
}

static void _SWDMX_Mp4VideoWakeUp(void)
{
    LOG(5, "_SWDMX_Mp4VideoWakeUp\n");
    VDP_SetPauseMm(0, FALSE);
    switch(_rSwdmxMp4Info.u4VDecType)
    {
        case ENUM_SWDMX_VDEC_VC1:
        case ENUM_SWDMX_VDEC_WMV7:
        case ENUM_SWDMX_VDEC_WMV8:
        case ENUM_SWDMX_VDEC_WMV9:
            VDEC_Play(ES0, VDEC_FMT_WMV);
            break;
        case ENUM_SWDMX_VDEC_DX311:
        case ENUM_SWDMX_VDEC_MPEG4: 
        case ENUM_SWDMX_VDEC_H263: 
            VDEC_Play(ES0, VDEC_FMT_MP4);
            break;
        case ENUM_SWDMX_VDEC_MJPEG:
            VDEC_Play(ES0, VDEC_FMT_MJPEG);
            break;
        case ENUM_SWDMX_VDEC_MPEG1_MPEG2:
            VDEC_Play(ES0, VDEC_FMT_MPV);
            break;
        case ENUM_SWDMX_VDEC_H264:
            VDEC_Play(ES0, VDEC_FMT_H264);
            break;
        default:
            LOG(1, "SWDMX_Mp4AudioCb Unknown Video Type\n");
            break;
    }
}

