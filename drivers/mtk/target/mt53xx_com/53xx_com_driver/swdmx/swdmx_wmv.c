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
 * $RCSfile: swdmx_wmv.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_wmv.c
 *  Software demux for wmv - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "vdec_drvif.h"
#include "feeder_if.h"
#include "dmx_mm_if.h"
#include "swdmx_drvif.h"
#include "swdmx_wmv.h"
#include "swdmx_debug.h"
#include "x_util.h"
#include "u_midxbuld.h"
#include "aud_drvif.h"

EXTERN void VDEC_SetWptr(UINT8 u1VdecId, UINT32 u4Wptr);
//EXTERN INT32 AUD_GetAudFifo(UINT32 * pu4Fifo1Start, UINT32 * pu4Fifo1SEnd,UINT32 * pu4Fifo2Start,UINT32 * pu4Fifo2End);
EXTERN void AUD_DrvSetWMAErrFunc(UINT8 u1DecId, AUD_WMAErr_NOTIFY pfClipNotify);

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define WMV_DATA_OBJ_HEADER (50)
#define WMV_NETFLIX_HEADER_SIZE (4)
#define WMV_MAX_FEEDER_RETRY (10)
#define RETRY_TIMES (3)
#define TRICK_MODE_RETRY_TIMES (1000)
#define FEEDER_OFFSET (64*1024)
#define WMV_KEY_TABLE_THRESHOLD (3)
#define MORE_DEMUX_PACKET_COUNT 5
#define WMV_VFIFO_SIZE (10*1024*1024)
#define WMV_DMX_FIFO_ALIGNMENT (64)
#define SWMDX_WMV_FEEDER_DATA_Q_SIZE (50)
#define NETFLIX_UNDERFLOW_THRESHOLD (30)
#ifdef __KERNEL__
#define FM_BLOCK_SIZE (2048)  // tmp solution for DTV00213176, DTV00213223, DTV00213232(?)
#else
#define FM_BLOCK_SIZE (512)
#endif

//option define
#define WMV_FAST_I_FRAME_PARSING
#define WMV_MULTI_REQUEST // always turn on
#define WMV_ERROR_HANDLE_ON
#define WAIT_AUDIO_READY
#define WMV_SUPPORT_AUDIO_SWITCH
//#define WMV_ALLOCATE_VFIFO
//#define DISABLE_AUDIO_IN_BAD_INTERLEAVE
//#define ENABLE_WMV_DRM

#define INPUT_SRC  (_prRangeList->prDmxRangeInfo->e_mm_src_type)

#ifdef CC_DLNA_SUPPORT
//#define WMV_ALLOCATE_VFIFO
#define FEEDER_SEGMENT (282*1024)
#define WMV_PRE_BUFFER_SIZE (2*1024*1024)
//#define ENABLE_FEEDER_THRESHOLD //something wrong....
#else
#ifdef CC_DMX_PURE_AUDIO
// feeder buffer only have 512*1024
// wmv will request two feeder request.
// So wmv can't use 288*1024 or feeder will full and drop that request.
#define WMV_PRE_BUFFER_SIZE (250*1024)
#define FEEDER_SEGMENT (240*1024)
#else
// 282*1024 = 288768
#define WMV_PRE_BUFFER_SIZE (2*1024*1024)
#define FEEDER_SEGMENT (282*1024)
#endif
#endif

#ifdef CC_DLNA_SUPPORT
//#define WMV_2X_TO_1X_PREBUFFER
#endif

#ifdef WMV_2X_TO_1X_PREBUFFER
#define WMV_AUD_PREBUF_IDX     7
#define WMV_AUD_UNKNOWN_IDX    255
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//for wmv parsing
#undef TypeTransform
#define TypeTransform(x) (SIZE_T)((x&0x2)&&(x&0x1)?4:x)
#undef min
#define min(x, y) ((x < y) ? x : y)
#undef max
#define max(x, y) ((x > y) ? x : y)
#undef abs
#define abs(x) (((x) >= 0) ? (x) : -(x))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static SWDMX_RANGE_LIST_T *_prRangeList = NULL;
static MM_RANGE_INFO_T* _prRangeInfo = NULL;

static FEEDER_TRANSMIT_COND _rFeederCond;
static FEEDER_REQ_DATA_T _rFeederReqData;

static UINT8 _u1AudioStreamId = 0xFF;
static FEEDER_BUF_INFO_T _rFeederInfo;

static SWDMX_WMV_INFO_T _rSwdmxWmvInfo;
static VDEC_PES_INFO_T _rPreviousPesInfo;
static DMX_AUDIO_PES_T _rPreviousAudioPes;
static BOOL _fgReceiveAudioPes;
static BOOL _fgReceiveVideoPes;
//static UINT32 _u4DataWithStartCode[10*1024];
static UINT8* _pu1VideoFrameData = NULL;
static UINT32 _u4PrevFramePTS = 0;
static UINT32 _u4DefalutReqSize = 0;
static PayloadParseInfo rPayLoadInfo;
static MultiPayloadInfo rMultiPayloadInfo;
static PayloadHeader rPayLoadHeader;

//for netflix application
static MINFO_TYPE_GENERIC_T* _prWmvGenericInfo = NULL;
static UINT32 _u4NetflixVideoStreamId;
static UINT32 _u4NetflixAudioStreamId;
static BOOL _fgNetflixEos = FALSE;
static BOOL _fgDataExhausted = TRUE;
static ENUM_SWDMX_FMT_T _eCurFMT;
static UINT32 _u4NumberOfBitstream = 1;
//for netflix application

#ifdef ENABLE_WMV_DRM
static UINT8 _u1VideoFrameData[100*1024];
static UINT64 _u8SampleId = 0;

//1.wmv drm file
static UINT8 _au1ContentKey[16] = {0xfd, 0x2b, 0xb7, 0x8c, 0x88, 0xb8 ,0xbe ,0x2f,0x75,
                                   0x15, 0xe9, 0xf6, 0x2f, 0x1a, 0xa4 ,0x3a
                                  };


//2.wmv drm file
/*static UINT8 _au1ContentKey[16] = {0xd3, 0x16, 0x1f, 0xea, 0x9e, 0x12 ,0xaf ,0x1,0x12,
                                   0x19, 0xd1, 0x29, 0x9, 0x8a, 0xe3 ,0xc5
                                  };
*/

#endif

#ifdef WMV_FAST_I_FRAME_PARSING
static UINT32 _u4PreKeyFrameAddr =0;
#endif


//#ifdef WMV_MULTI_REQUEST
static HANDLE_T _hWMVFeederDataQue = 0;
static UINT32 _u4RequestDataSize = 0;
static HAL_TIME_T _rFeederReqTime;
static HAL_TIME_T _rStartTime,_rEndTime;
//#endif

#ifdef WMV_ALLOCATE_VFIFO
static UINT32 _u4VFifoStartAddr = NULL;
#endif

//static UINT32 _u4WmvStpRsmThres = 1;  // for stop resume

#ifdef WMV_2X_TO_1X_PREBUFFER
static INT32 _i4WmvStcAdj = -1500;
#endif

#ifdef __MODEL_slt__
static UINT8 _au1VC1SltSeqHdr[] =
{
    0x21, 0x00, 0x00, 0x01, 0x0F, 0xD3, 0xEE, 0x27,
    0xF1, 0x67, 0x0A, 0x27, 0xF8, 0x59, 0xE2, 0xE9,
    0x40, 0x00, 0x40, 0x01, 0xC0, 0x00, 0x80, 0x01,
    0xA0, 0x00, 0xC0, 0x01, 0x80, 0x01, 0x00, 0x01,
    0x60, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01,
    0x20, 0x01, 0xC0, 0x01, 0x00, 0x02, 0x00, 0x00,
    0xE0, 0x02, 0x40, 0x00, 0xC0, 0x02, 0x80, 0x00,
    0xA0, 0x02, 0xC0, 0x00, 0x80, 0x03, 0x00, 0x00,
    0x60, 0x03, 0x40, 0x00, 0x40, 0x03, 0x80, 0x00,
    0x28, 0x00, 0x00, 0x01, 0x0E, 0x16, 0x93, 0xBB,
    0xD3, 0xEB, 0xF4, 0x04, 0x14, 0x24, 0x3C, 0x44,
    0x54, 0x5C, 0x64, 0x7C, 0x94, 0x9F, 0xC5, 0x9E,
    0x40
};

UINT8 _u1VC1SltLastPicType = 0;
UINT32 _u4VC1SltLastPicPts = 0;
#endif


typedef struct
{
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4RequestFeederSize;
} WMV_FEEDER_REQ_DATA_T;

BOOL _SWDMX_WMVRequestFeederData(UINT32 u4QrySize,UINT64 u8Offset,BOOL fgTrickMode);

VOID _WMVFindMaxPacketSize()
{
#ifdef __KERNEL__
    UINT32 u4Idx;
    UINT32 u4MaxPacketSize = 0;
    for (u4Idx = 0 ; u4Idx < _u4NumberOfBitstream ; u4Idx++)
    {
        if (_prWmvGenericInfo[u4Idx].ui4_sz_max_acces_unt > u4MaxPacketSize)
        {
            u4MaxPacketSize = _prWmvGenericInfo[u4Idx].ui4_sz_max_acces_unt;
        }
    }
    FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_PACKET_SIZE, (u4MaxPacketSize+WMV_NETFLIX_HEADER_SIZE));
#else
    UNUSED(_u4NumberOfBitstream);
#endif
}

BOOL _WMVDmxMoveData(UINT8 u1Idx, DMX_MM_DATA_T *prData, UINT32 u4TimeOut)
{
    BOOL fgRet = TRUE;
#ifdef ENABLE_WMV_DRM
    if (_rSwdmxWmvInfo.fgIsDRM)
    {
        DMX_MM_DRM_AES_T rMMDrmAes;
        UINT32 u4Flag =0;
        x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
        if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4Flag = (UINT32)(DMX_MM_DRM_FLAG_CHUNK_LEN);
            rMMDrmAes.u4ChunkLen = prData->u4FrameSize;
            prData->fgTrigger = TRUE;
            DMX_MM_SetPDDRM(u1Idx,u4Flag,&rMMDrmAes);
            fgRet = DMX_PDDRM_MoveData(u1Idx,prData,u4TimeOut);
        }
        else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
        {
            u4Flag = (UINT32)(DMX_MM_DRM_FLAG_CHUNK_LEN | DMX_MM_DRM_FLAG_ENCRYLEN | DMX_MM_DRM_FLAG_SAMPLE_ID);
            rMMDrmAes.u4ChunkLen = prData->u4FrameSize;
            rMMDrmAes.u4EncryLen = prData->u4FrameSize;
            x_memcpy((VOID*)&rMMDrmAes.au1SampleID[0],&_u8SampleId,8);
            prData->fgTrigger = TRUE;
            DMX_MM_SetNDDRM(u1Idx,u4Flag,&rMMDrmAes);
            fgRet = DMX_NDDRM_MoveData(u1Idx,prData,u4TimeOut);
        }

    }
    else
    {
        fgRet = DMX_MM_MoveData(u1Idx,prData,u4TimeOut);
    }
#else
    fgRet = DMX_MM_MoveData(u1Idx,prData,u4TimeOut);
#endif
    return fgRet;
}

UINT32 _WMVParsingStartCode(UINT32 u4FrameAddr,UINT32 u4FifoStart,UINT32 u4FifoEnd)
{
    UINT32 u4StartCode = 0;

    UINT8 u1Byte1,u1Byte2,u1Byte3,u1Byte4;
#if 0
    if (!(u4FrameAddr + 4 > u4FifoEnd))
    {
        x_memcpy(&u4StartCode, (VOID*)VIRTUAL(u4FrameAddr), 4);
        u4StartCode = ((u4StartCode&0x000000FF)<<24)
                      | ((u4StartCode&0x0000FF00)<<8) | ((u4StartCode&0x00FF0000)>>8)
                      | ((u4StartCode&0xFF000000)>>24);
    }
    else
    {
        //ASSERT(0);
    }
#else
    x_memcpy(&u1Byte1,(VOID*)VIRTUAL(u4FrameAddr),1);
    x_memcpy(&u1Byte2,(VOID*)VIRTUAL((u4FrameAddr+1)%u4FifoEnd),1);
    x_memcpy(&u1Byte3,(VOID*)VIRTUAL((u4FrameAddr+2)%u4FifoEnd),1);
    x_memcpy(&u1Byte4,(VOID*)VIRTUAL((u4FrameAddr+3)%u4FifoEnd),1);
    u4StartCode = u1Byte1<<24 |u1Byte2<<16 |u1Byte3<<8 |u1Byte4;
#endif
    return u4StartCode;
}
BOOL _WMVMoveData(UINT32 u4StartAddress,UINT32 u4CurPosition ,UINT32 u4FileSize,PayloadHeader* prPayLoadHeader,BOOL fgHeadSegment)
{
    //UINT32 static u4AudioCount = 0;
    //UINT32 u4VideoCount =0;
    UINT32 u4FrameCount=0;
    UINT32 u4Data;
    //UINT32 static u4Count =0;
    DMX_MM_DATA_T rDmxMMData;
    DMX_MM_DATA_T rDmxMMData_StartCode;
    DMX_MM_DATA_T rDmxMMData_SequenceHeader;
    UINT32 u4DmxAvailSize;
    BOOL fgSendToDMX = FALSE;
    //UINT32 u4StartCode[10];
    UINT32 u4RetryTimes =0;
    UINT16 u2VdecQueueSize =0;
    UINT16 u2VdecMaxQueueSize=0;
    UINT16 u2WaitTimes=50;
    BOOL fgVideoSegment = (prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4VStreamId) ? TRUE : FALSE;

    x_memset(&rDmxMMData,0,sizeof(DMX_MM_DATA_T));
    x_memset(&rDmxMMData_StartCode,0,sizeof(DMX_MM_DATA_T));
    x_memset(&rDmxMMData_SequenceHeader,0,sizeof(DMX_MM_DATA_T));


    if (_rSwdmxWmvInfo.i4WMVPlaySpeed <0 || _rSwdmxWmvInfo.i4WMVPlaySpeed >= 4000)
        u2WaitTimes = TRICK_MODE_RETRY_TIMES;
    else
        u2WaitTimes = RETRY_TIMES;

    //u4StartCode[1] = 0x0D010000;

    _rSwdmxWmvInfo.u2SendPayloadNo++;

    //VDEC_GetQueueInfo(0,&u2VdecQueueSize,&u2VdecMaxQueueSize);
    //LOG(0,"status : vdec queue size : %d/%d \n",u2VdecQueueSize,u2VdecMaxQueueSize);

    if (prPayLoadHeader->fgKeyFrame && fgHeadSegment && fgVideoSegment)
    {
        _rSwdmxWmvInfo.fgGetKeyHead = TRUE;
        _rSwdmxWmvInfo.u4IFrameOffset = _rSwdmxWmvInfo.u4CurParsingPacketNo;
        _rSwdmxWmvInfo.u4CurPacketPTS = (prPayLoadHeader->u4PTS-_rSwdmxWmvInfo.u4PreRoll)*90;

    }
    else if (!prPayLoadHeader->fgKeyFrame && fgHeadSegment && fgVideoSegment)
    {
        _rSwdmxWmvInfo.u4FrameOffset = _rSwdmxWmvInfo.u4CurParsingPacketNo;
    }

    else
    {
        //null operation
    }

    if (_rSwdmxWmvInfo.u2SendPayloadNo  < _rSwdmxWmvInfo.u2PreviousSendPayloadNo)
    {
        return TRUE;
    }

#if 1
    if (fgVideoSegment)
    {
        if ( (_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000)
                && (!prPayLoadHeader->fgKeyFrame || !_rSwdmxWmvInfo.fgGetKeyHead))
        {
            return TRUE;
        }

        if ((_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000)
                && prPayLoadHeader->fgKeyFrame && fgHeadSegment)
        {
            if (_rSwdmxWmvInfo.u1FWKeyFrameNo!=2)
                _rSwdmxWmvInfo.u1FWKeyFrameNo++;
            else
                return TRUE;
        }


        if ((_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000)
                && prPayLoadHeader->fgKeyFrame && _rSwdmxWmvInfo.u1FWKeyFrameNo ==2)
        {
            _rSwdmxWmvInfo.fgFinishDemuxIFrame = TRUE;
            return TRUE;
        }
    }
#else
    if ((_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000))
    {
        if (!prPayLoadHeader->fgKeyFrame)
            return TRUE;
        else
        {
            if (!_rSwdmxWmvInfo.fgGetKeyHead)
            {
                return TRUE;
            }
            else
            {
                if (prPayLoadHeader->u4OffsetIntoMediaObj + u4FileSize == prPayLoadHeader->u4MediaObjSize)
                {
                    _rSwdmxWmvInfo.fgFinishDemuxIFrame = TRUE;
                }
            }
        }
    }
#endif
    //LOG(0,"packet distance : %d\n",u4CurPosition - u4Address);
    //u4Address = u4CurPosition;

    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData.u4StartAddr = (u4StartAddress + u4CurPosition);

    if (rDmxMMData.u4StartAddr < rDmxMMData.u4BufStart)
    {
        rDmxMMData.u4BufStart -= FEEDER_OFFSET;
    }

    //if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1 && _rSwdmxWmvInfo.fgFileWithStartCode)
    //    rDmxMMData.u4FrameSize = (UINT32)max(6,u4FileSize);
    //else
    rDmxMMData.u4FrameSize = u4FileSize;
    rDmxMMData.u4Pts = ((prPayLoadHeader->u4PTS-_rSwdmxWmvInfo.u4PreRoll)*90);
    //rDmxMMData.fgFrameTail  = fgEndFrame;
    rDmxMMData.fgFrameHead  = fgHeadSegment;

    if (u4FileSize ==0)
        return TRUE;

    if ((prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4VStreamId) && _rSwdmxWmvInfo.fgEnVideo)
    {
        if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if (_rSwdmxWmvInfo.u4VideoCount == 0)
            {
                // DTV00213176
                //x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[0])), (VOID*)VIRTUAL(_rSwdmxWmvInfo.u4SequenceHeader), (_rSwdmxWmvInfo.u4SequenceHeaderLength));
                //_rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL(&_pu1VideoFrameData[0]);
#ifdef __MODEL_slt__
                x_memcpy((VOID *)(VIRTUAL(&_pu1VideoFrameData[0])), (VOID*)_au1VC1SltSeqHdr, 97);
                _rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL(&_pu1VideoFrameData[0]);
                _rSwdmxWmvInfo.u4SequenceHeaderLength = 97;
#endif

                rDmxMMData_SequenceHeader.u4BufStart = (UINT32)(_rSwdmxWmvInfo.u4SequenceHeader)&0xFFFFFFF0;
                rDmxMMData_SequenceHeader.u4BufEnd = ((UINT32)(_rSwdmxWmvInfo.u4SequenceHeader) + 0x10+188)&0xFFFFFFF0;
                rDmxMMData_SequenceHeader.u4StartAddr = _rSwdmxWmvInfo.u4SequenceHeader;//(UINT32)&_rSwdmxWmvInfo.au1SequenceHeader[0];
                rDmxMMData_SequenceHeader.u4FrameSize = (UINT32)(_rSwdmxWmvInfo.u4SequenceHeaderLength);
                rDmxMMData_SequenceHeader.u4Pts = 0;
                rDmxMMData_SequenceHeader.fgFrameHead  = TRUE;
                if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData_SequenceHeader, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move VC-1 sequence header.\n");
                    ASSERT(0);
                    return FALSE;
                }
            }
            _rSwdmxWmvInfo.u4VideoCount++;
        }
        do
        {
            u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_VIDEO); //indicate video fifo
            VDEC_GetQueueInfo(0,&u2VdecQueueSize,&u2VdecMaxQueueSize);



#if 1
            if (_rSwdmxWmvInfo.fgVideoNormalPlay && fgHeadSegment && prPayLoadHeader->fgKeyFrame)
            {
                _rSwdmxWmvInfo.u4TriggerAudioPTS = rDmxMMData.u4Pts;
                _rSwdmxWmvInfo.u4DisplayPTS = rDmxMMData.u4Pts;
                STC_StopStc();
                STC_SetStcValue((UINT32)(rDmxMMData.u4Pts-3000));
                if (_rSwdmxWmvInfo.fgEnVideo)//protect flag
                {
                    _rSwdmxWmvInfo.fgVideoNormalPlay = FALSE;
                }
            }
            else if (_rSwdmxWmvInfo.fgVideoNormalPlay)
            {
                return TRUE;
            }
#endif

            //if((u4FileSize <= u4DmxAvailSize) && (u2VdecQueueSize <= 3*30))
            if ((u4FileSize <= u4DmxAvailSize) && ((u2VdecMaxQueueSize - u2VdecQueueSize) >256))
            {
                if (prPayLoadHeader->u4OffsetIntoMediaObj == 0 && _rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1
                        &!_rSwdmxWmvInfo.fgFileWithStartCode)
                {
                    UINT32 u4PreStartCode = _WMVParsingStartCode(rDmxMMData.u4StartAddr,rDmxMMData.u4BufStart,rDmxMMData.u4BufEnd);
                    if (!(u4PreStartCode ==0x10D || u4PreStartCode==0x10E || u4PreStartCode==0x10C || u4PreStartCode==0x10F))
                    {
                        UINT32 u4Tmp = 0x0D010000;
                        x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[0])),(VOID*)&u4Tmp,4);

                        //x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[4])),(VOID*)rDmxMMData.u4StartAddr,rDmxMMData.u4FrameSize);
                        rDmxMMData_StartCode.u4BufStart = (UINT32)(_pu1VideoFrameData);
                        rDmxMMData_StartCode.u4BufEnd = ((UINT32)(_pu1VideoFrameData+1*1024) + 0x10+188)&0xFFFFFFF0;
                        rDmxMMData_StartCode.u4StartAddr = (UINT32)(_pu1VideoFrameData);
                        rDmxMMData_StartCode.u4FrameSize = 4;
                        rDmxMMData_StartCode.u4Pts = rDmxMMData.u4Pts;
                        rDmxMMData_StartCode.fgFrameHead= TRUE;

                        //if((_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed <= -2000))
                        //    LOG(0,"send addition start code : %x\n",rDmxMMData.u4Pts);

                        if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData_StartCode, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move frame start_code data.\n");
                            return FALSE;
                        }
                    }
                    /*else
                    {
                        _rSwdmxWmvInfo.fgFileWithStartCode = TRUE;
                        if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move video data.\n");
                            return FALSE;
                        }
                        //LOG(0,"file contains start code\n");
                    }*/
                    else
                    {
                        _rSwdmxWmvInfo.fgFileWithStartCode = TRUE;
                        //LOG(0,"!!!!!!! wmv error bug\n");
                    }

                }
                //else
                {
#ifdef ENABLE_WMV_DRM
                    x_memcpy((VOID*)&_u1VideoFrameData[prPayLoadHeader->u4OffsetIntoMediaObj],
                             (VOID*)rDmxMMData.u4StartAddr,u4FileSize);

                    if ((prPayLoadHeader->u4OffsetIntoMediaObj + u4FileSize == prPayLoadHeader->u4MediaObjSize))
                    {
                        rDmxMMData.u4BufStart = (UINT32)(&(_u1VideoFrameData[0]))&0xFFFFFFF0;
                        rDmxMMData.u4BufEnd = ((UINT32)(&(_u1VideoFrameData[100*1024])) + 0x10+188)&0xFFFFFFF0;
                        rDmxMMData.u4StartAddr = (UINT32)&(_u1VideoFrameData[0]);
                        rDmxMMData.u4FrameSize = prPayLoadHeader->u4MediaObjSize;// + rDmxMMData.u4FrameSize;
                        //rDmxMMData.u4Pts = rDmxMMData.u4Pts;
                        rDmxMMData.fgFrameHead= TRUE;
                        if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData, 500) != TRUE)
                        {
                            LOG(0, "Demuxer fails to move video data.\n");
                            return FALSE;
                        }
                    }
#else
                    if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData, 500) != TRUE)
                    {
                        LOG(0, "Demuxer fails to move video data.\n");
                        return FALSE;
                    }
#endif
                }
                if (_rSwdmxWmvInfo.u2PreviousSendPayloadNo >0)
                {
                    LOG(6,"video start at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"video start again , first 4 bytes : %x\n",u4Data);
                }
                u4RetryTimes = 0;
                //_rSwdmxWmvInfo.u2SendPayloadNo++;
                _rSwdmxWmvInfo.fgBlock = FALSE;
                _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                fgSendToDMX = TRUE;
            }
            else
            {
                //LOG(0,"dmx doesn't have available size to send payload data\n");
                if (u4RetryTimes >= u2WaitTimes)
                {
#ifdef DISABLE_AUDIO_IN_BAD_INTERLEAVE
                    UINT32 u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate video fifo
                    UINT32 u4QueueSize = (288*1024 - u4DmxAvailSize);
                    UINT32 u4Ratio = (100*u4QueueSize)/(288*1024);
                    if (u4Ratio <10 && _rSwdmxWmvInfo.fgEnAudio)
                        _rSwdmxWmvInfo.fgEnAudio = FALSE;
                    //LOG(0,"audio FIFO size : %d %d\n",u4QueueSize,((100*u4QueueSize)/(288*1024)));
#endif
                    _rSwdmxWmvInfo.fgBlock = TRUE;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo = _rSwdmxWmvInfo.u2SendPayloadNo;
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"video payload size : %d/%d \n",u4FileSize,u4DmxAvailSize);
                    LOG(6,"vdec queue size : %d/%d \n",u2VdecQueueSize,u2VdecMaxQueueSize);
                    LOG(6,"video stop at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    LOG(6,"video stop at first 4 bytes : %x\n",u4Data);
                    u4RetryTimes = 0;
#ifdef WAIT_AUDIO_READY
                    if (_rSwdmxWmvInfo.fgVideoPause)
                    {
                        _WMVStartToPlay();
                    }
#endif
                    return FALSE;
                }
                u4RetryTimes++;
                x_thread_delay(1);
                fgSendToDMX = FALSE;
            }
        } while (!fgSendToDMX);
        if (fgHeadSegment)
            u4FrameCount++;
        _rSwdmxWmvInfo.u4VideoCount++;
        //_rSwdmxWmvInfo.u4LastVideoPTS = rDmxMMData.u4Pts;
    }
    else if ((prPayLoadHeader->u1StreamNum == (UINT8)_rSwdmxWmvInfo.u4AStreamId) &&
#ifdef WMV_2X_TO_1X_PREBUFFER
             (_rSwdmxWmvInfo.fgEnAudio || _rSwdmxWmvInfo.fg2XAudParsing) &&
#else
             _rSwdmxWmvInfo.fgEnAudio &&
#endif
             !_rSwdmxWmvInfo.fgChangeAudio &&
#ifdef WMV_2X_TO_1X_PREBUFFER
             (_rSwdmxWmvInfo.i4WMVPlaySpeed == 1000 || _rSwdmxWmvInfo.fg2XAudParsing)
#else
             _rSwdmxWmvInfo.i4WMVPlaySpeed == 1000
#endif
            )
    {
#if 1
        UINT32 u4RetryAudio =0;
        do {
            if (_rSwdmxWmvInfo.u4WmaErrorCode)
            {
                if (_rSwdmxWmvInfo.fgFirstAudioError)
                    return TRUE;
                LOG(0,"wma error code %x\n",_rSwdmxWmvInfo.u4WmaErrorCode);

                //enable video to play
#ifdef WAIT_AUDIO_READY
                if (_rSwdmxWmvInfo.fgVideoPause)
                {
                    _WMVStartToPlay();
                }
#endif
                _rSwdmxWmvInfo.fgFirstAudioError = TRUE;
                return TRUE;
            }
#ifdef WMV_2X_TO_1X_PREBUFFER
            if (_rSwdmxWmvInfo.fg2XAudParsing)
                u4DmxAvailSize = 0xFFFFFFFF;
            else
                u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate audio fifo
#else
            u4DmxAvailSize = DMX_MM_GetBufEmptySize(eSWDMX_MEDIA_AUDIO); //indicate audio fifo
#endif

            if (_rSwdmxWmvInfo.fgAudioNormalPlay && fgHeadSegment
                    && (rDmxMMData.u4Pts >= (UINT32)(_rSwdmxWmvInfo.u4DisplayPTS+3000) || _rSwdmxWmvInfo.u4DisplayPTS==0))
            {
                LOG(0,"WMV start PTS : %x\n",rDmxMMData.u4Pts);
                //VDP_TriggerAudReceive(rDmxMMData.u4Pts);
                _rSwdmxWmvInfo.u4TriggerAudioPTS = rDmxMMData.u4Pts;
                _rSwdmxWmvInfo.u4CurPTS = rDmxMMData.u4Pts;//DTV00213648
                AUD_SetStartPts(0,rDmxMMData.u4Pts);
#ifdef WMV_2X_TO_1X_PREBUFFER
                DMX_AudHandler_SetActivePidx(/*_rSwdmxWmvInfo.u1CurATrackIdx + */WMV_AUD_PREBUF_IDX);  // !!!
#endif
                if (_rSwdmxWmvInfo.fgEnAudio)//protect flag
                {
                    _rSwdmxWmvInfo.fgAudioNormalPlay = FALSE;
                }
            }
            else if (_rSwdmxWmvInfo.fgAudioNormalPlay)
                //else if(_rSwdmxWmvInfo.fgAudioNormalPlay && !fgHeadSegment )
            {
                return TRUE;
            }

            UINT8 u1HdrType;
#ifdef WMV_2X_TO_1X_PREBUFFER
            u1HdrType = WMV_AUD_PREBUF_IDX/* + u1TrackIdx*/;  // !!!
#else
            u1HdrType = eSWDMX_MEDIA_AUDIO;
#endif
            if ((u4FileSize <= u4DmxAvailSize))
            {
                if (_WMVDmxMoveData(u1HdrType, &rDmxMMData, 500) != TRUE)
                {
                    LOG(0, "Demuxer fails to move audio data.\n");
                    return FALSE;
                }
                //printf("Audio pkt pts = %d (%d sec)\n", rDmxMMData.u4Pts, rDmxMMData.u4Pts / 90000);
                if (_rSwdmxWmvInfo.u2PreviousSendPayloadNo >0) {
                    LOG(6,"audio start at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"audio start again , first 4 bytes : %x\n",u4Data);
                }
                _rSwdmxWmvInfo.u4CurPTS = rDmxMMData.u4Pts;//DTV00213648
                u4RetryAudio =0;
                fgSendToDMX = TRUE;
                //LOG(0,"swdmx send audio pes : %x\n",rDmxMMData.u4Pts);
            }
            else
            {
                if (u4RetryAudio >= u2WaitTimes)
                {
                    _rSwdmxWmvInfo.fgBlock = TRUE;
                    _rSwdmxWmvInfo.u2PreviousSendPayloadNo = _rSwdmxWmvInfo.u2SendPayloadNo;
                    x_memcpy(&u4Data,(void*)(VIRTUAL(u4CurPosition+u4StartAddress)),4);
                    LOG(6,"audio payload size : %d/%d \n",u4FileSize,u4DmxAvailSize);
                    LOG(6,"audio stop at packet %d , payload %d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u2SendPayloadNo);
                    LOG(6,"audio stop at first 4 bytes : %x\n",u4Data);
                    return FALSE;
                }
                u4RetryAudio++;
                x_thread_delay(1);
                fgSendToDMX = FALSE;
            }
        } while (!fgSendToDMX);
        _rSwdmxWmvInfo.u4AudioCount++;
        //u4AudioCount++;
#endif
    }
    else
    {
        //LOG(6,"don't play this stream id data :%d\n",prPayLoadHeader->u1StreamNum);
    }

    return TRUE;
}


static BOOL _WMVParrsingMultiPayload(UINT32 u4ParsingCount,PayloadParseInfo* prPayLoadInfo,UINT32 u4PacketStartAddr,UINT32 u4CurrentPos)
{
    //UINT32 u4PayloadSize=0;
    //MultiPayloadInfo rMultiPayloadInfo;

    UINT8 u1Tmp=0;
    UINT8 index=0;
    BOOL fgRet = TRUE;

    x_memset(&rMultiPayloadInfo,0,sizeof(MultiPayloadInfo));

    //u4PayloadSize = (prPayLoadInfo->u4PacketLength) - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
    x_memcpy(&u1Tmp,((VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos)),1);
    u4CurrentPos += 1;
    rMultiPayloadInfo.u1PayloadsNumber = u1Tmp &0x3F;
    rMultiPayloadInfo.u1PayloadLengthType = (u1Tmp>>6) & 0x3;
    rMultiPayloadInfo.u4FilePos = u4CurrentPos;
    //LOG(0,"number of payloads : %d\n",rMultiPayloadInfo.u1PayloadsNumber);
    //do {
    for (index=0 ; index<rMultiPayloadInfo.u1PayloadsNumber ; index++) {
        fgRet = _WMVParsingPayload(u4ParsingCount,prPayLoadInfo,u4PacketStartAddr,rMultiPayloadInfo.u4FilePos,&rMultiPayloadInfo);
        //fgRet = WMVParsingPayload(rMultiPayloadInfo.u4FilePos,prPayLoadInfo,u4PacketStartAddr,rMultiPayloadInfo.u4FilePos,&rMultiPayloadInfo);
        if (fgRet == FALSE)
            return FALSE;
    }
    return TRUE;
    //}while(rMultiPayloadInfo.u4FilePos < u4PayloadSize);
}

BOOL _WMVParsingPayload(UINT32 u4ParsingCount,PayloadParseInfo* prPayLoadInfo,UINT32 u4PacketStartAddr,UINT32 u4CurrentPos,MultiPayloadInfo* prMultiPayloadInfo)
{
    //PayloadHeader rPayLoadHeader;

    UINT8 u1Tmp =0;
    UINT32 u4PayloadLength =0;
    //UINT32 u4ParsingCount =0;
    x_memset(&rPayLoadHeader,0,sizeof(PayloadHeader));

    //ASSERT(((u4PacketStartAddr+u4CurrentPos)&(0xF0000000)) ==0 );


    //ASSERT((u4PacketStartAddr+u4CurrentPos) < _rFeederInfo.u4EndAddr);

    //LOG(0,"address : %x\n",u4PacketStartAddr+u4CurrentPos);
#ifdef WMV_ERROR_HANDLE_ON
    if ((u4CurrentPos& 0xFF000000) != 0)
    {
        _rSwdmxWmvInfo.fgBrokenFile = TRUE;
        LOG(0,"The file is broken1\n");
        return FALSE;
    }
#endif
    //media stream number , there are multiple stream in packet , it represents for multi video stream or audio stream
    x_memcpy(&u1Tmp,((VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos)),1);
    //LOG(0,"u1Tmp : %x\n",u1Tmp);
    //LOG(0,"payload index : %x\n",u4CurrentPos);
    rPayLoadHeader.u1StreamNum = u1Tmp &0x7F;
    rPayLoadHeader.fgKeyFrame = (u1Tmp>>7) & 0x1;
    u4CurrentPos += 1;
    u4ParsingCount += 1;
    //LOG(0,"u1StreamNum : %d\n",rPayLoadHeader.u1StreamNum);

    //media object number , it represent vidoe/audio frame no.
    if (prPayLoadInfo->u1MediaObjectNumType != 0) {
        x_memcpy(&rPayLoadHeader.u4MediaObjNum,(VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prPayLoadInfo->u1MediaObjectNumType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1MediaObjectNumType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1MediaObjectNumType));
        //LOG(0,"u4MediaObjNum : %d\n",rPayLoadHeader.u4MediaObjNum);
    }
    else {
        //ASSERT((prPayLoadInfo->u1MediaObjectNumType));
    }
    //OffsetInto , it represent the offset of corresponding position in that video/audio object
    if (prPayLoadInfo->u1OffsetIntoType != 0) {
        x_memcpy(&rPayLoadHeader.u4OffsetIntoMediaObj,(VOID*)VIRTUAL(u4PacketStartAddr+u4CurrentPos),TypeTransform((prPayLoadInfo->u1OffsetIntoType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1OffsetIntoType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1OffsetIntoType));
        //LOG(0,"u4OffsetIntoMediaObj : %d\n",rPayLoadHeader.u4OffsetIntoMediaObj);
    }
    else {
        //ASSERT((prPayLoadInfo->u1OffsetIntoType));
    }
    // , it represent the offset of corresponding position in that video/audio object
    if (prPayLoadInfo->u1ReplicatedDataType!= 0) {
        x_memcpy(&rPayLoadHeader.u4ReplicatedDataLen,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((prPayLoadInfo->u1ReplicatedDataType)));
        u4CurrentPos += TypeTransform((prPayLoadInfo->u1ReplicatedDataType));
        u4ParsingCount += TypeTransform((prPayLoadInfo->u1ReplicatedDataType));
        //LOG(0,"u4ReplicatedDataLen : %d\n",rPayLoadHeader.u4ReplicatedDataLen);
    }
    else {
        //ASSERT((prPayLoadInfo->u1ReplicatedDataType));
    }
    //parsing single payload
    if (rPayLoadHeader.u4ReplicatedDataLen != 1) {
        //parsing replicated data (PTS and media object size)
        x_memcpy(&rPayLoadHeader.u4MediaObjSize,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),4);
        u4CurrentPos += 4;
        u4ParsingCount += 4;
        x_memcpy(&rPayLoadHeader.u4PTS,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),4);
        u4CurrentPos += 4;
        u4ParsingCount +=4;
        //adding sample id info
#ifdef ENABLE_WMV_DRM
        if (rPayLoadHeader.u4ReplicatedDataLen >= 16)
        {
            UINT32 u4Offset = rPayLoadHeader.u4ReplicatedDataLen -16;
            x_memcpy(&_u8SampleId,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos+u4Offset)),8);
        }
#endif
        u4CurrentPos += rPayLoadHeader.u4ReplicatedDataLen - 8;
        u4ParsingCount += rPayLoadHeader.u4ReplicatedDataLen - 8;
        //parsing payload data
        if (prMultiPayloadInfo != NULL) {
            //spork , error handling
#ifdef WMV_ERROR_HANDLE_ON
            if ( (u4CurrentPos& 0xFF000000) != 0)
            {
                _rSwdmxWmvInfo.fgBrokenFile = TRUE;
                LOG(0,"The file is broken2\n");
                return FALSE;
            }
#endif
            x_memcpy(&u4PayloadLength,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((prMultiPayloadInfo->u1PayloadLengthType)));
            u4CurrentPos += TypeTransform((prMultiPayloadInfo->u1PayloadLengthType));
            prMultiPayloadInfo ->u4FilePos = u4CurrentPos+u4PayloadLength;
            /*if(((prMultiPayloadInfo ->u4FilePos)&(0xF0000000)) !=0 ) {
                _rSwdmxWmvInfo.u4FilePos = prMultiPayloadInfo ->u4FilePos;
                return FALSE;
            }*/
            //LOG(0,"filepos : %x ,multi payload length : %d %d\n",prMultiPayloadInfo ->u4FilePos,u4PayloadLength,(prMultiPayloadInfo->u1PayloadLengthType));
        }
        else {
            u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
            //LOG(0,"payload size : %x %x %x %x %x\n",u4PayloadLength,prPayLoadInfo->u4PacketLength,u4CurrentPos,u4ParsingCount,prPayLoadInfo->u4PaddingLength);
        }
        //u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
        //printf("payload size : %x %x %x %x %x\n",u4PayloadLength,prPayLoadInfo->u4PacketLength,u4CurrentPos,u4ParsingCount,prPayLoadInfo->u4PaddingLength);
        /*
        if((rPayLoadHeader.u4OffsetIntoMediaObj + u4PayloadLength) == rPayLoadHeader.u4MediaObjSize) {
            DMAMove(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,TRUE);
        }
        else {
            DMAMove(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,FALSE);
        }
        */
        if (rPayLoadHeader.u4OffsetIntoMediaObj == 0) {
            _WMVMoveData(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,TRUE);
        }
        else {
            _WMVMoveData(u4PacketStartAddr,u4CurrentPos,u4PayloadLength,&rPayLoadHeader,FALSE);
        }
        if (_rSwdmxWmvInfo.fgBlock)
            return FALSE;
        /*if((rPayLoadHeader.u4OffsetIntoMediaObj + u4PayloadLength) == rPayLoadHeader.u4MediaObjSize)
            SendES();
        else
            ;//for lint free
        */
    }
    //parsing single payload with compressed payload data
    else {
        UINT32 u4ParsingSize =0;
        rPayLoadHeader.u4PTS = rPayLoadHeader.u4OffsetIntoMediaObj; //in compressed payload , offset into media object field represent PTS
        x_memcpy(&rPayLoadHeader.u4PTSDelta,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        if (prMultiPayloadInfo != NULL) {
            x_memcpy(&u4PayloadLength,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((prMultiPayloadInfo->u1PayloadLengthType)));
            u4CurrentPos += TypeTransform((prMultiPayloadInfo->u1PayloadLengthType));
            prMultiPayloadInfo ->u4FilePos = u4CurrentPos+u4PayloadLength;
            /*if(((prMultiPayloadInfo ->u4FilePos)&(0xF0000000)) !=0 ) {
                _rSwdmxWmvInfo.u4FilePos = prMultiPayloadInfo ->u4FilePos;
                return FALSE;
            }*/
            //LOG(0,"filepos : %x, multi payload length : %d %d\n",prMultiPayloadInfo ->u4FilePos,u4PayloadLength,TypeTransform((prMultiPayloadInfo->u1PayloadLengthType)));
        }
        else {
            u4PayloadLength = prPayLoadInfo->u4PacketLength - u4ParsingCount - (prPayLoadInfo->u4PaddingLength);
            //LOG(0,"payload size : %x\n",u4PayloadLength);
        }
        do {
            UINT8 u1SubPayloadLen =0;
            x_memcpy(&u1SubPayloadLen,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
            u4CurrentPos += 1;
            _WMVMoveData(u4PacketStartAddr,u4CurrentPos,u1SubPayloadLen,&rPayLoadHeader,TRUE);
            if (_rSwdmxWmvInfo.fgBlock)
                return FALSE;
            //SendES(); //for compressed payload data, every payload represent one media object
            u4CurrentPos += u1SubPayloadLen;
            u4ParsingSize += u1SubPayloadLen + 1;
        } while (u4ParsingSize < u4PayloadLength);
    }
    return TRUE;
}

BOOL _WMVParsingPacket(UINT32 u4PacketStartAddr,UINT32 u4CurrentPos,UINT32 u4PacketSize)
{
    //UINT32 u4CurrentPos = 0;
    //UINT64 u8TotalDataPackets=0;
    //UINT32 index=0;
    //UINT32 u4InitialAddr = u4CurrentPos + 50;
    UINT32 u4ParsingCount = 0;
    UINT64 u8Remainder;

    //x_memcpy(&u8TotalDataPackets,u4PacketStartAddr+u4CurrentPos+40,8);
    //LOG(0,"total data packet : %d \n",u8TotalDataPackets);
    //u4CurrentPos += 50; //packet header size is 50 bytes

    //LOG(0,"packet start pos : %x\n",u4CurrentPos);

    //u4Address = u4CurrentPos;

    //handle other than payload paring
    //do {
    UINT8 u1Tmp =0;
    if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
    {
        UINT8 u1IdxOfPraingBitstream=0;
        UINT8 u1PacketType =0;
        UINT8 u1EOS =0;
        x_memcpy(&u1IdxOfPraingBitstream,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        u4CurrentPos +=1;
        x_memcpy(&u1PacketType,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        u4CurrentPos +=1;
        x_memcpy(&u1EOS,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        u4CurrentPos +=1;

        //reserved bytes
        u4CurrentPos +=1;

        if (u1EOS == TRUE)
        {
            _SWDMX_WMVSendEOS();
            return TRUE;
        }
        _rSwdmxWmvInfo.u4VStreamId = _u4NetflixVideoStreamId;
        _rSwdmxWmvInfo.u4AStreamId = _u4NetflixAudioStreamId;
        _rSwdmxWmvInfo.u4PreRoll = (UINT32)u8Div6432(_prWmvGenericInfo[u1IdxOfPraingBitstream].ui8_preroll, 90, &u8Remainder);
        _rSwdmxWmvInfo.u4PacketSize = (UINT32)_prWmvGenericInfo[u1IdxOfPraingBitstream].ui4_sz_max_acces_unt;

        if (u1PacketType == NETFLIX_VIDEO) //video
        {
            _rSwdmxWmvInfo.u4AStreamId = 0xFF;
        }
        else if (u1PacketType == NETFLIX_AUDIO) //audio
        {
            _rSwdmxWmvInfo.u4VStreamId = 0xFF;
        }

    }

    x_memcpy(&u1Tmp,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
    //LOG(0,"u1Tmp : %x\n",u1Tmp);
    //LOG(0,"index : %x\n",u4CurrentPos);
    //u4CurrentPos += 1;
    u4ParsingCount = 0;
    if (u1Tmp&0x80) //parsing error correction data
    {
        //UINT8 u1ErrorCorrectFlag;
        //UINT8 u1ErrorCorrectionDataLen=0;
        //UINT8 u1ErrorCorrectionLenType=0;
        //UINT8 u1ErrorCorrectionPresent=0;
        //parsing Error Correction Data
        //u1ErrorCorrectionDataLen = (u1Tmp&0xF);
        //u1ErrorCorrectionLenType = (u1Tmp>>5)&0x3;
        //u1ErrorCorrectionPresent = (u1Tmp>>7)&0x1;
        u4CurrentPos += 1;
        u4CurrentPos += 2;
        u4ParsingCount += 3;
    }
    //else //parsing payload information
    {
        //UINT8 u1Length=0;

        //PayloadParseInfo rPayLoadInfo;

        x_memset(&rPayLoadInfo,0,sizeof(PayloadParseInfo));
        //LOG(0,"index : %x\n",u4CurrentPos);
        x_memcpy(&u1Tmp,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        //LOG(0,"u1Tmp : %x\n",u1Tmp);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        rPayLoadInfo.fgMultiplePayload = u1Tmp & 0x1;
        rPayLoadInfo.u1SequenceType= (u1Tmp>>1) & 0x3;
        rPayLoadInfo.u1PaddingLenghtType = (u1Tmp>>3) & 0x3;
        rPayLoadInfo.u1PacketLengthType = (u1Tmp>>5) & 0x3;
        rPayLoadInfo.fgErrorCorrectPresent = (u1Tmp>>7) & 0x1;

        x_memcpy(&u1Tmp,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),1);
        //LOG(0,"u1Tmp : %x\n",u1Tmp);
        u4CurrentPos += 1;
        u4ParsingCount += 1;
        rPayLoadInfo.u1ReplicatedDataType = (u1Tmp)&0x3;
        rPayLoadInfo.u1OffsetIntoType = (u1Tmp>>2)&0x3;
        rPayLoadInfo.u1MediaObjectNumType = (u1Tmp>>4)&0x3;
        rPayLoadInfo.u1StreamNumberType = (u1Tmp>>6)&0x3;

        if (rPayLoadInfo.u1PacketLengthType != 0) { //packet length
            x_memcpy(&rPayLoadInfo.u4PacketLength,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((rPayLoadInfo.u1PacketLengthType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1PacketLengthType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1PacketLengthType));
            //LOG(0,"packet length : %d %d\n",rPayLoadInfo.u4PacketLength,TypeTransform((rPayLoadInfo.u1PacketLengthType)));
        }
        else {
            rPayLoadInfo.u4PacketLength = _rSwdmxWmvInfo.u4PacketSize; //get form upper middleware
        }
        if (rPayLoadInfo.u1SequenceType != 0) { //sequence length
            x_memcpy(&rPayLoadInfo.u4Sequence,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((rPayLoadInfo.u1SequenceType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1SequenceType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1SequenceType));
            //LOG(0,"sequence length : %d %d\n",rPayLoadInfo.u4Sequence,TypeTransform((rPayLoadInfo.u1SequenceType)));
        }
        else {
            rPayLoadInfo.u4Sequence = 0;
        }
        if (rPayLoadInfo.u1PaddingLenghtType != 0) { //padding length
            x_memcpy(&rPayLoadInfo.u4PaddingLength,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),TypeTransform((rPayLoadInfo.u1PaddingLenghtType)));
            u4CurrentPos += TypeTransform((rPayLoadInfo.u1PaddingLenghtType));
            u4ParsingCount += TypeTransform((rPayLoadInfo.u1PaddingLenghtType));
            //LOG(0,"padding length : %d %d\n",rPayLoadInfo.u4PaddingLength,TypeTransform((rPayLoadInfo.u1PaddingLenghtType)));
        }
        else {
            rPayLoadInfo.u4PaddingLength = 0;
        }
        x_memcpy(&rPayLoadInfo.u4SendTime,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),4);

        u4CurrentPos += 4;
        u4ParsingCount += 4;
        x_memcpy(&rPayLoadInfo.u2Duration,VIRTUAL((void*)(u4PacketStartAddr+u4CurrentPos)),2);
        u4CurrentPos += 2;
        u4ParsingCount += 2;
        //LOG(0,"index : %x\n",u4CurrentPos);
        //LOG(0,"u4ParsingCount : %x\n",u4ParsingCount);
        if (rPayLoadInfo.fgMultiplePayload)
        {
            return _WMVParrsingMultiPayload(u4ParsingCount,&rPayLoadInfo,u4PacketStartAddr,u4CurrentPos);
        }
        else {
            return _WMVParsingPayload(u4ParsingCount,&rPayLoadInfo,u4PacketStartAddr,u4CurrentPos,NULL);
        }
    }
    //index++;
    //u4CurrentPos = u4InitialAddr + index*u4PacketSize;
    //}while(index < u8TotalDataPackets);
    //return TRUE;
}

VOID _WMVAudioErrorCallback(UINT32 u4ErrorCode)
{
    _rSwdmxWmvInfo.u4WmaErrorCode = u4ErrorCode;
}

VOID _WMVStartToPlay(VOID)
{
    if (_rSwdmxWmvInfo.fgEnVideo)//protect func
    {
        VDP_SetPauseMm(B2R_1, FALSE);
    }

    switch (_rSwdmxWmvInfo.eVideoType)
    {
    case ENUM_SWDMX_VDEC_VC1:
#if 0  // disable WMV7/8
    case ENUM_SWDMX_VDEC_WMV7:
    case ENUM_SWDMX_VDEC_WMV8:
#endif
    case ENUM_SWDMX_VDEC_WMV9:
        VDEC_Play(ES0, VDEC_FMT_WMV);
        break;
    case ENUM_SWDMX_VDEC_MPEG4:
    case ENUM_SWDMX_VDEC_DX311:
        VDEC_Play(ES0, VDEC_FMT_MP4);
        break;
    default :
        //do nothing
        break;
    }
    _rSwdmxWmvInfo.fgVideoPause = FALSE;
    LOG(0,"_WMVStartToPlay start to play video\n");
}

BOOL _WMVAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet = TRUE;
    if (_fgReceiveAudioPes)
    {
        _rPreviousAudioPes.u4Pts = prPes->u4Pts;
        fgRet =  AUD_SendAudioPes(&_rPreviousAudioPes);
        if (prPes->fgEOS)
        {
            fgRet =  AUD_SendAudioPes(prPes);
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }
    }
    else
    {
        if (prPes->fgEOS)
        {
            fgRet =  AUD_SendAudioPes(prPes);
            AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
        }
    }
    _fgReceiveAudioPes = TRUE;
    x_memcpy((VOID*)&_rPreviousAudioPes,(VOID*)prPes,sizeof(DMX_AUDIO_PES_T));
    return fgRet;
}

VOID _WMVCheckDataExhausted(VOID)
{
    UINT16 u2VdecQueueSize,u2VdecMaxQueueSize;
    u2VdecMaxQueueSize =0;
    u2VdecQueueSize =0;

    VDEC_GetQueueInfo(0,&u2VdecQueueSize,&u2VdecMaxQueueSize);
    if (u2VdecQueueSize < NETFLIX_UNDERFLOW_THRESHOLD && !_fgNetflixEos && !_fgDataExhausted &&
            _prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
    {
        LOG(0,"swdmx wmv netflix data exhausted 3\n");
        _fgDataExhausted = TRUE;
        FeederNotifyHandle(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_EXHAUSTED, 1);
    }
    else if (u2VdecQueueSize > (NETFLIX_UNDERFLOW_THRESHOLD<<1) && _fgDataExhausted)
    {
        LOG(0,"swdmx wmv netflix data exhausted 4\n");
        _fgDataExhausted = FALSE;
    }
}

BOOL _WMVVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UCHAR *pucBuf;
    UINT32 u4Addr;
    UINT32 u4StartCode = 0;

    BOOL fgTrickMode = (_rSwdmxWmvInfo.i4WMVPlaySpeed>=4000 || _rSwdmxWmvInfo.i4WMVPlaySpeed<=-2000)
                       ? TRUE : FALSE;

    //UINT16 u2VdecQueueSize,u2VdecMaxQueueSize;
    //VDEC_GetQueueInfo(0,&u2VdecQueueSize,&u2VdecMaxQueueSize);

    //LOG(0,"vdec queue %d/%d\n",u2VdecQueueSize,u2VdecMaxQueueSize);

    //pu1Temp = (UINT8*)prPes->u4FrameAddr;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    //LOG(6,"wmv call back PTS : %d\n",prPes->u4Pts);

    rPesInfo.fgDtsValid = TRUE;//prPes->fgPtsDts;
    rPesInfo.u4PTS = (prPes->u4Pts);
    rPesInfo.u4DTS = prPes->u4Dts;
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;//(prPes->u4FrameAddr - prPes->u4BufStart);
    rPesInfo.fgEos = prPes->fgEOS;

    rPesInfo.u8OffsetI = (UINT64)_rSwdmxWmvInfo.u4IFrameOffset;
    rPesInfo.u8Offset = (UINT64)_rSwdmxWmvInfo.u4FrameOffset;

    if (rPesInfo.fgEos)
    {
        rPesInfo.u4VldReadPtr =0;
        rPesInfo.u4FifoStart =0;
        rPesInfo.u4FifoEnd =0;
        if (_rSwdmxWmvInfo.fgVideoPause)
        {
            _WMVStartToPlay();
        }
    }


    _rSwdmxWmvInfo.u4LastVideoWptr = prPes->u4FrameAddr;

    switch (_rSwdmxWmvInfo.eVideoType)
    {
    case ENUM_SWDMX_VDEC_VC1:
        rPesInfo.u4VideoType = DMX_VIDEO_VC1;
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
    case ENUM_SWDMX_VDEC_MPEG4:
        rPesInfo.u4VideoType = DMX_VIDEO_MPEG4;
        break;
    default:
        rPesInfo.u4VideoType = DMX_VIDEO_WMV9;
        break;
    }

    if ( _rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_DX311)
    {
        u4Addr = prPes->u4FrameAddr;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);

        if ((pucBuf[0] >> 6) & 0x1)
        {
            rPesInfo.ucPicType  = P_TYPE;
        }
        else
        {
            rPesInfo.ucPicType = I_TYPE;
        }
    }
    else if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
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
    }
    else if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        u4Addr = prPes->u4FrameAddr+3;
        if (u4Addr >= prPes->u4BufEnd)
        {
            u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
        }
        pucBuf = (UCHAR*)VIRTUAL(u4Addr);
        u4StartCode = pucBuf[0];
        u4StartCode |= 0x100;
        if ((u4StartCode == 0x10E || u4StartCode ==0x10F))
        {
            VDEC_SendEs((void*)&rPesInfo);
            if (rPesInfo.u4PTS!=0 || u4StartCode ==0x10F)
                _u4PrevFramePTS = (rPesInfo.u4PTS);
            return TRUE;
        }
        else if ((u4StartCode == 0x10C || u4StartCode ==0x10D))
        {
            if (rPesInfo.u4PTS ==0)
                rPesInfo.u4PTS = _u4PrevFramePTS;
            if (fgTrickMode)
                LOG(0,"frame pts : %x %d\n",rPesInfo.u4PTS,rPesInfo.u4PTS/90000);
        }
        else
        {
            //ASSERT(0);
        }

    }

    if (fgTrickMode)
    {
        if (!rPesInfo.fgEos && rPesInfo.u4PTS!=0)
        {
            if ((rPesInfo.u4PTS < (UINT32)_rSwdmxWmvInfo.u4DisplayPTS))
            {
                if (_rSwdmxWmvInfo.i4WMVPlaySpeed >0)
                {
                    LOG(3,"wmv video pts is too small for FF %x %x\n",rPesInfo.u4PTS,_rSwdmxWmvInfo.u4DisplayPTS);
                    return TRUE;
                }
            }
            else
            {
                if (_rSwdmxWmvInfo.i4WMVPlaySpeed <0)
                {
                    LOG(3,"wmv video pts is too big for FR %x %x \n",rPesInfo.u4PTS,_rSwdmxWmvInfo.u4DisplayPTS);
                    return TRUE;
                }
            }
        }

        if ( _fgReceiveVideoPes && (u4StartCode != 0x10C))
        {
            UINT32 u4PtsDelta;
            INT32 i4PtsDelta = (INT32)((INT32)rPesInfo.u4PTS - (INT32)_rPreviousPesInfo.u4PTS);
            UINT32 u4RepeatTimes =5;
            UINT32 i;
            u4PtsDelta = (UINT32)abs(i4PtsDelta);
            UINT32 u4FeederReqTime = (_rFeederReqTime.u4Micros*9/100);
            if (u4FeederReqTime > u4PtsDelta)
                u4PtsDelta = 0;
            else
                u4PtsDelta -= u4FeederReqTime;
            if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
            {
                u4RepeatTimes = (UINT32)(_rSwdmxWmvInfo.u4FrameRate*u4PtsDelta)/(90*(abs(_rSwdmxWmvInfo.i4WMVPlaySpeed)));
            }
#ifdef CC_DLNA_SUPPORT
            else if (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA)
            {
                u4RepeatTimes = (UINT32)(_rSwdmxWmvInfo.u4FrameRate*u4PtsDelta)/(90*(abs(_rSwdmxWmvInfo.i4WMVPlaySpeed)));
                u4RepeatTimes = max(1,u4RepeatTimes);
            }
#endif
            LOG(5,"repeat times : %d %d %d\n",rPesInfo.u4PTS/90000,_rPreviousPesInfo.u4PTS/90000,u4RepeatTimes);
            _rPreviousPesInfo.u1SliceCount = TRUE;
            if (rPesInfo.fgEos)
                u4RepeatTimes = 5;

            //x_thread_delay(100); // to avoid dmx not finish send data to vfifo

            for (i =0 ; i<u4RepeatTimes ; i++)
            {
                if (i == u4RepeatTimes-1)
                    _rPreviousPesInfo.u1SliceCount = FALSE;
                VDEC_SendEs((void*)&_rPreviousPesInfo);
            }
            if (rPesInfo.fgEos)
                VDEC_SendEs((void*)&rPesInfo);
        }
        else if ( _fgReceiveVideoPes && u4StartCode == 0x10C)
        {
            UINT32 u4idx =0;
            for (u4idx =0 ; u4idx <_rSwdmxWmvInfo.u4RepeatTimes ; u4idx++)
            {
                VDEC_SendEs((void*)&_rPreviousPesInfo);
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
        else
        {
            if (rPesInfo.fgEos)
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
    }
    else
    {
#ifdef __MODEL_slt__
        if (rPesInfo.u4PTS > _u4VC1SltLastPicPts)
        {
            _u1VC1SltLastPicType = rPesInfo.ucPicType;
            _u4VC1SltLastPicPts = rPesInfo.u4PTS;
        }
#endif
        VDEC_SendEs((void*)&rPesInfo);
    }

    if (fgTrickMode)
    {
        if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if ((u4StartCode == 0x10D || u4StartCode==0x10C))
            {
                x_memcpy((VOID*)&_rPreviousPesInfo,(VOID*)&rPesInfo,sizeof(VDEC_PES_INFO_T));
                _fgReceiveVideoPes = TRUE;
            }
            else
            {
                VDEC_SendEs((void*)&rPesInfo);
            }
        }
        else
        {
            x_memcpy((VOID*)&_rPreviousPesInfo,(VOID*)&rPesInfo,sizeof(VDEC_PES_INFO_T));
            _fgReceiveVideoPes = TRUE;
        }
    }
    return TRUE;

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
/**
*/
BOOL _SWDMX_WMVFeederIsReady(VOID)
{
    if ( (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
            || (_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA))
    {
#ifdef ENABLE_FEEDER_THRESHOLD
        //with feeder threshold
#else
        //without feeder threshold
#if 1
        if (_fgNetflixEos && _prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            return TRUE;
        }
#endif
        if (!_SWDMX_WMVIsFeederBufferFull())
        {
            FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo);
            UINT32 u4FeederSize = _rFeederInfo.u4BytesInBuf;
            if (u4FeederSize < WMV_PRE_BUFFER_SIZE && _u4RequestDataSize !=0/*!_rSwdmxWmvInfo.fgReqFeederDataEOS*/)
            {
                return FALSE;
            }
            return TRUE;
        }
        else
        {
            return TRUE;
        }
#endif
    }
    else
    {
        return TRUE;
    }
}

VOID _SWDMX_WMVFlushFeederDataQ(VOID)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    WMV_FEEDER_REQ_DATA_T rData;

    zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);

    while (x_msg_q_receive(&u2MsgQIdx, &rData, &zMsgSize,
                           &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hWMVFeederDataQue, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}

BOOL _SWDMX_WMVIsFeederBufferFull(VOID)
{
    UINT32 u4FeederFreeSize;

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != FEEDER_E_OK)
    {
        LOG(7, "Feeder input buffer error.\n");
        return FALSE;
    }

    u4FeederFreeSize = _rFeederInfo.u4FreeSize;

    if (u4FeederFreeSize >= (_u4DefalutReqSize+FM_BLOCK_SIZE+FM_BLOCK_SIZE))
        return FALSE;
    else
        return TRUE;
}

BOOL _SWDMX_WMVGetFeederData(BOOL fgNoWait)
{
    BOOL fgRet = FALSE;
    INT32 i4Ret;
    WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;

    //if(_rSwdmxWmvInfo.fgReqFeederDataEOS)
    if ( _u4RequestDataSize == 0)
    {
        return FALSE;
    }

    x_memset(&_rFeederReqData,0,sizeof(FEEDER_REQ_DATA_T));

    if (!fgNoWait)
    {
        do
        {
            fgRet = _SWDMX_ReceiveFeederAck((VOID*)&_rFeederReqData);
            if (_rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
            {
                _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                LOG(3,"feeder invalid 1\n");
                return FALSE;
            }
        } while (_rFeederReqData.u4Id+1 != _rSwdmxWmvInfo.u4FeederReqID);

        if (!fgRet)
        {
            LOG(3,"feeder timeout1\n");
            _SWDMX_WMVSendEOS();
            return FALSE;
        }
    }
    else
    {
        do
        {
            i4Ret = _SWDMX_ReceiveFeederAckNoWait((VOID*)&_rFeederReqData);
            if (i4Ret == OSR_NO_MSG)
            {
                return FALSE;
            }
            else if (i4Ret != OSR_OK)
            {
                _SWDMX_WMVSendEOS();
                return FALSE;
            }
            else
            {
                if (_rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
                {
                    _rSwdmxWmvInfo.fgFeederInvalid = TRUE;
                    LOG(3,"feeder invalid 1\n");
                    return FALSE;
                }
            }
        } while (_rFeederReqData.u4Id+1 != _rSwdmxWmvInfo.u4FeederReqID);
    }

    LOG(6,"feeder ack %d\n",_rFeederReqData.u4Id);

    x_memcpy((VOID*)&(rWmvFeederReqData.rFeederReqData),(VOID*)&_rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
    rWmvFeederReqData.u4RequestFeederSize = _u4RequestDataSize;

    i4Ret = x_msg_q_send(_hWMVFeederDataQue, &rWmvFeederReqData, sizeof(WMV_FEEDER_REQ_DATA_T), 255);
    if (_rSwdmxWmvInfo.fgFirstFrame)
    {
        _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER+
                ((_u4RequestDataSize-WMV_DATA_OBJ_HEADER)/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
    }
    else
    {
        _rSwdmxWmvInfo.u8LastFileParsingPosition += (_u4RequestDataSize/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
    }

    return TRUE;
}

BOOL _SWDMX_WMVRequestFeederData(UINT32 u4QrySize,UINT64 u8Offset,BOOL fgTrickMode)
{
    //HAL_TIME_T rStartTime,rEndTime;
    UINT32 u4RemainedDataSize,u4RequestDataSize;
    UINT32 u4DefalutReqSize;
    //UINT32 u4QrySize;
    //UINT64 u8Offset;
    x_memset(&_rFeederCond,0,sizeof(FEEDER_TRANSMIT_COND));

    if (_rSwdmxWmvInfo.fgFirstFrame)
    {
        u4RemainedDataSize = (UINT32)_rSwdmxWmvInfo.u8FileSize;
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4DefalutReqSize = ((FEEDER_SEGMENT/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt) + 50;
        }
        else
        {
            u4DefalutReqSize = FEEDER_SEGMENT;
        }
    }
    else
    {
        u4RemainedDataSize = (UINT32)_rSwdmxWmvInfo.u8FileSize -
                             ((_rSwdmxWmvInfo.u8LastFileParsingPosition)-(_prRangeList->prDmxRangeInfo->ui8_fileoffset));
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            u4DefalutReqSize = (FEEDER_SEGMENT/_prRangeInfo->ui4_sz_min_acces_unt)*_prRangeInfo->ui4_sz_min_acces_unt;
        }
        else
        {
            u4DefalutReqSize = FEEDER_SEGMENT;
        }
    }

    u4RequestDataSize = min(u4DefalutReqSize,u4RemainedDataSize);

    if (!fgTrickMode)
    {
        u4QrySize = u4RequestDataSize;
        u8Offset = _rSwdmxWmvInfo.u8LastFileParsingPosition;
    }

    _u4RequestDataSize = u4QrySize;

    if (u4QrySize != 0)
    {
        INT32 i4Ret;
        LOG(6,"feeder request id: %d\n",_rSwdmxWmvInfo.u4FeederReqID);
        i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,u4QrySize,u8Offset,&_rFeederCond,_rSwdmxWmvInfo.u4FeederReqID++);

        //HAL_GetTime(&rStartTime);
        //_SWDMX_WMVGetFeederData();
        //HAL_GetTime(&rEndTime);
        //HAL_GetDeltaTime(&_rFeederReqTime, &rStartTime, &rEndTime);
        //_rSwdmxWmvInfo.fgTureReqFeederData = TRUE;
        if (i4Ret != FEEDER_E_OK)
        {
            _u4RequestDataSize = 0;
        }
        return TRUE;
    }
    else
    {
        _rSwdmxWmvInfo.fgReqFeederDataEOS = TRUE;
        FeederSetInfo(FEEDER_SWDMX_EOS,TRUE);
        return FALSE;
    }
}

BOOL _SWDMX_WMVSetStrmID(UINT8 u1HdrType,UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    switch (u1HdrType)
    {
    case eSWDMX_STRM_TYPE_VID:
        _rSwdmxWmvInfo.u4VStreamId= u4StrmID;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        _u1AudioStreamId = (UINT8)_rSwdmxWmvInfo.u4AStreamId;
        _rSwdmxWmvInfo.u4AStreamId = u4StrmID;
#ifdef WMV_SUPPORT_AUDIO_SWITCH
        if ((_u1AudioStreamId != (UINT8)_rSwdmxWmvInfo.u4AStreamId) &&_u1AudioStreamId !=0xff)
        {
            _rSwdmxWmvInfo.fgChangeAudio = TRUE;
            LOG(0,"WMV file change audio channel\n");
        }
        else
        {
            _rSwdmxWmvInfo.u4WmaErrorCode = 0; //DTV215322
            _rSwdmxWmvInfo.fgChangeAudio = FALSE;
        }
#endif
        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.u4SpStreamId = u4StrmID;
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

BOOL _SWDMX_WMVEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        if (!_rSwdmxWmvInfo.fgEnVDmx)
        {
            _SWDMX_WMVInitDmx(eSWDMX_MEDIA_VIDEO,TRUE,FALSE);
        }
        _rSwdmxWmvInfo.fgEnVDmx = TRUE;
        _rSwdmxWmvInfo.fgEnVideo = TRUE;

        break;
    case eSWDMX_STRM_TYPE_AUD:
        if (!_rSwdmxWmvInfo.fgEnADmx)
        {
            _SWDMX_WMVInitDmx(eSWDMX_MEDIA_AUDIO,TRUE,FALSE);
        }
        _rSwdmxWmvInfo.fgEnADmx = TRUE;
        _rSwdmxWmvInfo.fgEnAudio = TRUE;
#ifdef WMV_2X_TO_1X_PREBUFFER
        _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
#endif
        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.fgEnSubTitle = TRUE;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}


BOOL _SWDMX_WMVDisableStrm(UINT8 u1StrmType)
{
    DMX_MM_T rPid;
    rPid.fgEnable = FALSE;

    switch (u1StrmType)
    {
    case eSWDMX_STRM_TYPE_VID:
        if (_rSwdmxWmvInfo.fgEnVDmx)
        {
            _rSwdmxWmvInfo.fgEnVDmx = FALSE;
            VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
        }
        _rSwdmxWmvInfo.fgEnVideo = FALSE;
        break;
    case eSWDMX_STRM_TYPE_AUD:
        if (_rSwdmxWmvInfo.fgVideoPause)
        {
            _WMVStartToPlay();
        }
        if (_rSwdmxWmvInfo.fgEnADmx)
        {
            _rSwdmxWmvInfo.fgEnADmx = FALSE;
#ifndef WMV_2X_TO_1X_PREBUFFER
            VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rPid));
            DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
#endif
        }
        _rSwdmxWmvInfo.fgEnAudio = FALSE;
#ifdef WMV_2X_TO_1X_PREBUFFER
        //_rSwdmxWmvInfo.fg2XAudParsing = TRUE;
#endif

        break;
    case eSWDMX_STRM_TYPE_SUBTITLE:
        _rSwdmxWmvInfo.fgEnSubTitle = FALSE;
        break;
    default:
        return FALSE;
    }
    return TRUE;
}

BOOL _SWDMX_WMVInitDmx(UINT8 u1HdrType,BOOL fgEnable,BOOL fgEos)
{
    UINT32 u4Flag;
    FBM_POOL_T* prFbmPool;
    DMX_MM_T rDmxMMInfo;

    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));

    if (u1HdrType == eSWDMX_MEDIA_VIDEO)
    {
        rDmxMMInfo.fgEnable = fgEnable;
        rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;

        if (!_rSwdmxWmvInfo.fgEnVDmx)
        {
            // Allocate video buffer from FBM
            prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
            if ((prFbmPool == NULL) || (prFbmPool->u4Addr == 0))
            {
                LOG(1, "_SWDMX_SetVideoStreamID (prFbmPool == NULL) || (prFbmPool->u4Addr == NULL)\n");
                return FALSE;
            }

            u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
                              DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE);

            rDmxMMInfo.fgAllocBuf = FALSE;

#ifndef WMV_ALLOCATE_VFIFO
            rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
            rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
#else
            if (_u4VFifoStartAddr ==NULL)
            {
                _u4VFifoStartAddr = (UINT32)BSP_AllocAlignedDmaMemory(WMV_VFIFO_SIZE,WMV_DMX_FIFO_ALIGNMENT);
            }
            if (_u4VFifoStartAddr != NULL)
            {
                rDmxMMInfo.u4BufAddr = _u4VFifoStartAddr;
                rDmxMMInfo.u4BufSize = WMV_VFIFO_SIZE;
            }
            else
            {
                rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
                rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
            }
#endif

            switch (_rSwdmxWmvInfo.eVideoType)
            {
            case ENUM_SWDMX_VDEC_WMV7:
            case ENUM_SWDMX_VDEC_WMV8:
            case ENUM_SWDMX_VDEC_WMV9:
            case ENUM_SWDMX_VDEC_DX311:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                rDmxMMInfo.fgSearchStartCode = FALSE;
                break;
            case ENUM_SWDMX_VDEC_MPEG4:
            case ENUM_SWDMX_VDEC_VC1:
                LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            default:
                //ASSERT(0); //unknow wmv type
                //LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                //rDmxMMInfo.fgSearchStartCode = TRUE;
                break;
            }
            DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
#ifdef ENABLE_WMV_DRM
            if (_rSwdmxWmvInfo.fgIsDRM)
            {
                DMX_MM_DRM_AES_T rMMDrmAes;
                UINT32 u4Flag =0;

                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
                {
                    u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY);
                    x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                    x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                    DMX_MM_SetDRMMode(eSWDMX_MEDIA_VIDEO,DMX_DRM_MODE_PD);
                    DMX_MM_SetNDDRM(eSWDMX_MEDIA_VIDEO,u4Flag,&rMMDrmAes);
                }
                else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
                {
                    u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY | DMX_MM_DRM_FLAG_SAMPLE_ID);
                    x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                    x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                    x_memset((VOID*)&rMMDrmAes.au1SampleID[0],0,sizeof(rMMDrmAes.au1SampleID));

                    DMX_MM_SetDRMMode(eSWDMX_MEDIA_VIDEO,DMX_DRM_MODE_ND);
                    DMX_MM_SetNDDRM(eSWDMX_MEDIA_VIDEO,u4Flag,&rMMDrmAes);
                }

            }
#endif
        }
        else
        {
            u4Flag = DMX_MM_FLAG_SEARCH_START_CODE;
            if (fgEos)
            {
                rDmxMMInfo.fgSearchStartCode = FALSE;
            }
            else
            {
                switch (_rSwdmxWmvInfo.eVideoType)
                {
                case ENUM_SWDMX_VDEC_WMV7:
                case ENUM_SWDMX_VDEC_WMV8:
                case ENUM_SWDMX_VDEC_WMV9:
                case ENUM_SWDMX_VDEC_DX311:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = FALSE\n");
                    rDmxMMInfo.fgSearchStartCode = FALSE;
                    break;
                case ENUM_SWDMX_VDEC_MPEG4:
                case ENUM_SWDMX_VDEC_VC1:
                    LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
                default:
                    //ASSERT(0); //unknow wmv type
                    //LOG(7, "_SWDMX_SetVideoStreamID SearchStartCode = TRUE\n");
                    //rDmxMMInfo.fgSearchStartCode = TRUE;
                    break;
                }
            }
            DMX_MM_Set(eSWDMX_MEDIA_VIDEO, u4Flag, &rDmxMMInfo);
        }
    }
    else if (u1HdrType == eSWDMX_MEDIA_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

#ifdef WMV_2X_TO_1X_PREBUFFER
        if (_rSwdmxWmvInfo.fgAlreadySet)
            return TRUE;
        else
            _rSwdmxWmvInfo.fgAlreadySet = TRUE;
#endif

        u4Flag = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_VALID | DMX_MM_FLAG_NOTIFY
                          | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE);

        rDmxMMInfo.fgEnable = fgEnable;
#ifdef CC_DMX_PURE_AUDIO
        if(_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
        {
            rDmxMMInfo.ePidType = DMX_PID_TYPE_PURE_AUDIO;
        }
        else
#endif
        {
            rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_AUDIO;
        }
        rDmxMMInfo.fgSearchStartCode = FALSE;

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
        DMX_MM_Set(eSWDMX_MEDIA_AUDIO, u4Flag, &rDmxMMInfo);

#ifdef WMV_2X_TO_1X_PREBUFFER
        int i;
        BOOL fgRet;

        DMX_AudHandler_SetEnable(TRUE);

        if (!DMX_AudHandler_Set(eSWDMX_MEDIA_AUDIO, TRUE))
        {
            LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
        }

        for (i = 0; i < 1/*WMV_MAX_AUD_TRACK*/; i++)  // !!!
        {
            rDmxMMInfo.fgAllocBuf = TRUE;
            rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];
            fgRet = DMX_MM_Set(WMV_AUD_PREBUF_IDX + i, u4Flag, &rDmxMMInfo);
            ASSERT(fgRet);
            if (!DMX_AudHandler_Set(WMV_AUD_PREBUF_IDX + i, FALSE))
            {
                LOG(1, "%s:%d, DMX_AudHandler_Set fail\n", __FUNCTION__, __LINE__);
            }
        }
        fgRet = DMX_AudHandler_SetActivePidx(WMV_AUD_PREBUF_IDX);
        ASSERT(fgRet);
        UNUSED(fgRet);
#else
#ifdef CC_DLNA_SUPPORT
        DMX_AudHandler_SetEnable(FALSE);
#endif
#endif

#ifdef ENABLE_WMV_DRM
        if (_rSwdmxWmvInfo.fgIsDRM)
        {
            DMX_MM_DRM_AES_T rMMDrmAes;
            UINT32 u4Flag =0;

            if (INPUT_SRC == MM_SRC_TYPE_NETWORK_NETFLIX)
            {
                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY);
                x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));

                DMX_MM_SetDRMMode(eSWDMX_MEDIA_AUDIO,DMX_DRM_MODE_PD);
                DMX_MM_SetNDDRM(eSWDMX_MEDIA_AUDIO,u4Flag,&rMMDrmAes);
            }
            else if (INPUT_SRC == MM_SRC_TYPE_NETWORK_DLNA)
            {
                x_memset((VOID*)&rMMDrmAes,0,sizeof(DMX_MM_DRM_AES_T));
                u4Flag = (UINT32)(DMX_MM_DRM_FLAG_KEY | DMX_MM_DRM_FLAG_SAMPLE_ID);
                x_memset((VOID*)&rMMDrmAes.au1Key[0],0,sizeof(rMMDrmAes.au1Key));
                x_memcpy((VOID*)&rMMDrmAes.au1Key[0],(VOID*)&_au1ContentKey[0],sizeof(_au1ContentKey));
                x_memset((VOID*)&rMMDrmAes.au1SampleID[0],0,sizeof(rMMDrmAes.au1SampleID));

                DMX_MM_SetDRMMode(eSWDMX_MEDIA_AUDIO,DMX_DRM_MODE_ND);
                DMX_MM_SetNDDRM(eSWDMX_MEDIA_AUDIO,u4Flag,&rMMDrmAes);
            }

        }
#endif
    }
    return TRUE;
}

VOID _SWDMX_WMVComsumeFeeder()
{
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
            FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        //return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }

    FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rFeederInfo.u4WriteAddr);

}

VOID _SWDMX_WMVSeek(UINT64 u8SeekTime, SWDMX_RANGE_LIST_T* prRangeList)
{
    UINT32 i;

    UINT32 u4NumberOfEntry = 0;

    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
    {
        u4NumberOfEntry = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry;
    }
    else
    {
        LOG(3,"_SWDMX_WMVSeek : _rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL\n");
        ASSERT(_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL);
        return;
    }


    LOG(0, "_SWDMX_WMVSeek u8SeekTime = 0x%08x\n", u8SeekTime);

    for (i = 0; i < u4NumberOfEntry ; i++)
    {
        if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts >= (UINT32)u8SeekTime)
        {
            break;
        }
    }

    if (i > 0)
    {
        if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[i].ui4_pts == (UINT32)u8SeekTime)
        {
            _rSwdmxWmvInfo.u4CurKeyIdx = i;
        }
        else
        {
            _rSwdmxWmvInfo.u4CurKeyIdx = i - 1;
        }

    }
    else
    {
        _rSwdmxWmvInfo.u4CurKeyIdx = 0;
    }

    _SWDMX_WMVComsumeFeeder();

    if (_rSwdmxWmvInfo.u4PacketSize == 0 && _prRangeInfo !=0)
    {
        _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
    }

    if (_rSwdmxWmvInfo.u4PacketSize != 0)
    {
        _rSwdmxWmvInfo.u4CurParsingPacketNo = (_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry[_rSwdmxWmvInfo.u4CurKeyIdx].ui4_relative_offset)/(_rSwdmxWmvInfo.u4PacketSize);
        _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

    }
    else
    {
        _SWDMX_WMVSendEOS();
    }
    LOG(0, "_SWDMX_WmvSeek Key = %ld\n", _rSwdmxWmvInfo.u4CurKeyIdx);

}



BOOL _SWDMX_WMVTrickModeParsing(VOID)
{
    //parsing one key frame in trick mode
    UINT32 u4NumberOfEntry;
    UINT32 u4address;
    UINT32 u4RetryTimes =0;
    MIDXBULD_ELMT_STRM_ENTRY_T* prKeyidxEntry;
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;


    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
    {
        u4NumberOfEntry = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry;
    }
    else
    {
        LOG(3,"_SWDMX_WMVTrickModeParsing : _rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL\n");
        ASSERT(_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL);
        return TRUE;
    }

    if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL && !_rSwdmxWmvInfo.fgFinishKeyTable)
    {
        UINT32 i=0;
        UINT32 u8Baseoffset = (UINT32)_rSwdmxWmvInfo.prWmvKeyIdxTbl->ui8_base_offset;
        UINT32 u4DemuxSize;
        UINT32 u4DemuxPacketCount;
        UINT32 u4ParsingPacketNo;
        UINT32 u4MoreParsingPacket;

        prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
        u4address = u8Baseoffset + prKeyidxEntry->ui4_relative_offset;

        //printf("i frame position : %d\n",_rSwdmxWmvInfo.u4CurKeyIdx);

        u4ParsingPacketNo = (prKeyidxEntry->ui4_relative_offset/_rSwdmxWmvInfo.u4PacketSize);
        u4MoreParsingPacket = (UINT32)min(MORE_DEMUX_PACKET_COUNT,_rSwdmxWmvInfo.u4TotalPacketNo-u4ParsingPacketNo
                                          -(prKeyidxEntry->ui4_size/_rSwdmxWmvInfo.u4PacketSize));
        u4DemuxSize = prKeyidxEntry->ui4_size + (u4MoreParsingPacket*_rSwdmxWmvInfo.u4PacketSize);
        u4DemuxPacketCount = (prKeyidxEntry->ui4_size/_rSwdmxWmvInfo.u4PacketSize) + u4MoreParsingPacket;

        if (u4DemuxSize != 0)
        {
            _SWDMX_WMVRequestFeederData(u4DemuxSize,u4address,TRUE);
            HAL_GetTime(&_rStartTime);
            _SWDMX_WMVGetFeederData(FALSE);
            HAL_GetTime(&_rEndTime);
            HAL_GetDeltaTime(&_rFeederReqTime, &_rStartTime, &_rEndTime);
            if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
            {
                return FALSE;
            }
        }

        WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;
        x_memset(&rWmvFeederReqData,0,sizeof(rWmvFeederReqData));

        zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);
        i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT);

        if (i4Ret != OSR_OK)
        {
            ASSERT(0);
        }

        x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
        _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

        _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset;
        _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize;

        _rSwdmxWmvInfo.u1FWKeyFrameNo =0;

        _rSwdmxWmvInfo.u4CurParsingPacketNo = prKeyidxEntry->ui4_relative_offset/_rSwdmxWmvInfo.u4PacketSize;

        _rSwdmxWmvInfo.fgGetKeyHead = FALSE;

        _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
        _rSwdmxWmvInfo.u2SendPayloadNo = 0;
        _rSwdmxWmvInfo.fgBlock = FALSE;

        for (i=0 ; i<u4DemuxPacketCount ; i++)
        {
            UINT32 u4AddrDelta = _rFeederInfo.u4EndAddr - _rSwdmxWmvInfo.u4PacketStartAddr;
            if (u4AddrDelta < _rSwdmxWmvInfo.u4PacketSize)
            {
                x_memcpy((VOID*)(_rFeederInfo.u4StartAddr-u4AddrDelta),(VOID*)(_rSwdmxWmvInfo.u4PacketStartAddr),u4AddrDelta);
                _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederInfo.u4StartAddr-u4AddrDelta;
            }
            _WMVParsingPacket(_rSwdmxWmvInfo.u4PacketStartAddr,0,_rSwdmxWmvInfo.u4PacketSize);
            _rSwdmxWmvInfo.u4PacketStartAddr += _rSwdmxWmvInfo.u4PacketSize;
            if (_rSwdmxWmvInfo.fgFinishDemuxIFrame)
            {
                _rSwdmxWmvInfo.fgFinishDemuxIFrame = FALSE;
                break;
            }
        }

        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxWmvInfo.u4PacketStartAddr);
#ifdef WMV_FAST_I_FRAME_PARSING
        _u4PreKeyFrameAddr = u4address;
#endif
    }

#ifdef WMV_FAST_I_FRAME_PARSING
NEXT_I_FRAME:
#endif

    if ((_rSwdmxWmvInfo.i4WMVPlaySpeed < 0 && (_rSwdmxWmvInfo.u4CurKeyIdx == 0 || _rSwdmxWmvInfo.u4CurParsingPacketNo==0 ))
            || (_rSwdmxWmvInfo.i4WMVPlaySpeed >=4000 && _rSwdmxWmvInfo.u4CurKeyIdx >= u4NumberOfEntry-1)
       )
    {
        if (!_rSwdmxWmvInfo.fgFinishKeyTable)
        {
            _SWDMX_WMVSendEOS();
            _rSwdmxWmvInfo.fgFinishKeyTable = TRUE;
            return TRUE;
        }
    }
    else
    {
        if (_rSwdmxWmvInfo.i4WMVPlaySpeed<0)
        {
            _rSwdmxWmvInfo.u4CurKeyIdx--;
        }
        else if (_rSwdmxWmvInfo.i4WMVPlaySpeed>0)
        {
            _rSwdmxWmvInfo.u4CurKeyIdx++;
        }
    }
#ifdef WMV_FAST_I_FRAME_PARSING
    prKeyidxEntry = (MIDXBULD_ELMT_STRM_ENTRY_T*)(_rSwdmxWmvInfo.prWmvKeyIdxTbl->u.pt_pb_idx_tbl_entry+_rSwdmxWmvInfo.u4CurKeyIdx);
    u4address = _rSwdmxWmvInfo.prWmvKeyIdxTbl->ui8_base_offset + prKeyidxEntry->ui4_relative_offset;
    if (u4address == _u4PreKeyFrameAddr)
    {
        u4RetryTimes++;
        if (u4RetryTimes < 100)
            goto NEXT_I_FRAME;
    }
#endif
    UNUSED(u4RetryTimes);
    return TRUE;
}

BOOL _SWDMX_WMVDeliverData(SWDMX_RANGE_LIST_T* prRangeList)
{
    INT32 i4Ret;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    BOOL fgRet = FALSE;

    BOOL fgTrickMode = (_rSwdmxWmvInfo.i4WMVPlaySpeed>=4000 || _rSwdmxWmvInfo.i4WMVPlaySpeed<=-2000)
                       ? TRUE : FALSE;

    if (_rSwdmxWmvInfo.fgDemuxFinish)
    {
        return FALSE;
    }
    if (_rSwdmxWmvInfo.fgFeederInvalid)
    {
        _rSwdmxWmvInfo.fgFeederInvalid = FALSE;
    }

    _WMVCheckDataExhausted();

    if ( !_rSwdmxWmvInfo.fgBlock && _rSwdmxWmvInfo.fgFirstFrame && !fgTrickMode )
    {
        //VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);
        if (_rSwdmxWmvInfo.fgEnVideo)//protect func
        {
            switch (_rSwdmxWmvInfo.eVideoType)
            {
            case ENUM_SWDMX_VDEC_VC1:
                DMX_SetVideoType(DMX_VIDEO_VC1);
                break;
            case ENUM_SWDMX_VDEC_WMV7:
                DMX_SetVideoType(DMX_VIDEO_WMV7);
                break;
            case ENUM_SWDMX_VDEC_WMV8:
                DMX_SetVideoType(DMX_VIDEO_WMV8);
                break;
            case ENUM_SWDMX_VDEC_WMV9:
                DMX_SetVideoType(DMX_VIDEO_WMV9);
                break;
            case ENUM_SWDMX_VDEC_MPEG4:
                DMX_SetVideoType(DMX_VIDEO_MPEG4);
            case ENUM_SWDMX_VDEC_DX311:
                DMX_SetVideoType(DMX_VIDEO_MP4_IN_WMV);
            default:
                DMX_SetVideoType(DMX_VIDEO_WMV9);
                break;
            }
        }
#if 0
        if (prVdecEsInfo != NULL)
        {
            switch (prVdecEsInfo->rMMSeqInfo.e_frame_rate)
            {
            case VDEC_SRC_FRAME_RATE_23_976:
                _rSwdmxWmvInfo.u4FrameRate = 23976;
                break;
            case VDEC_SRC_FRAME_RATE_24:
                _rSwdmxWmvInfo.u4FrameRate = 24000;
                break;
            case VDEC_SRC_FRAME_RATE_25:
                _rSwdmxWmvInfo.u4FrameRate = 25000;
                break;
            case VDEC_SRC_FRAME_RATE_29_97:
                _rSwdmxWmvInfo.u4FrameRate = 29970;
                break;
            case VDEC_SRC_FRAME_RATE_30:
                _rSwdmxWmvInfo.u4FrameRate = 30000;
                break;
            case VDEC_SRC_FRAME_RATE_50:
                _rSwdmxWmvInfo.u4FrameRate = 50000;
                break;
            case VDEC_SRC_FRAME_RATE_59_94:
                _rSwdmxWmvInfo.u4FrameRate = 59940;
                break;
            case VDEC_SRC_FRAME_RATE_60:
                _rSwdmxWmvInfo.u4FrameRate = 60000;
                break;
            default:
                _rSwdmxWmvInfo.u4FrameRate = 23976;
                break;
            }
        }
#endif

        //_SWDMX_WMVComsumeFeeder();

        if (_rSwdmxWmvInfo.fgFirstReqFeederData)
        {
            _SWDMX_WMVRequestFeederData(0,0,FALSE);
            _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
        }

        fgRet = _SWDMX_WMVGetFeederData(FALSE);
        if (!fgRet)
        {
            if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
            {
                return FALSE;
            }
        }
        else
        {
            _rSwdmxWmvInfo.fgFirstFrame = FALSE;
            _SWDMX_WMVRequestFeederData(0,0,FALSE);
            _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
        }

        if(_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {

            WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;

            zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);
            i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                    &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT);

            if (i4Ret != OSR_OK)
            {
                ASSERT(0);
            }

            x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
            _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset;
            _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize;

            //get feeder buffer information
            if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != FEEDER_E_OK)
            {
                LOG(3, "_SWDMX_WMVDeliverData FeederGetBufferInfo() error.\n");
                return FALSE;
            }

            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxWmvInfo.u4PacketStartAddr);
        }
#ifndef ENABLE_WMV_DRM
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            x_memcpy(&(_rSwdmxWmvInfo.u4DataObjSize),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr+16),8);
            x_memcpy(&(_rSwdmxWmvInfo.u4TotalPacketNo),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr+40),8);

            if (_rSwdmxWmvInfo.u4TotalPacketNo ==0 || _rSwdmxWmvInfo.u4DataObjSize==0 || _prRangeInfo->ui4_sz_min_acces_unt ==0)
            {
                UINT64 u8Remainder;
                if (_prRangeInfo->ui4_sz_min_acces_unt ==0)
                {
                    _SWDMX_WMVSendEOS();
                    return FALSE;
                }
                _rSwdmxWmvInfo.u4DataObjSize = (UINT32)_rSwdmxWmvInfo.u8FileSize;
                _rSwdmxWmvInfo.u4TotalPacketNo = (UINT32)u8Div6432((_rSwdmxWmvInfo.u8FileSize-WMV_DATA_OBJ_HEADER),
                                                 _prRangeInfo->ui4_sz_min_acces_unt, &u8Remainder);

                if (_rSwdmxWmvInfo.u4TotalPacketNo == 0)  // tmp solution for DTV00212590
                    _rSwdmxWmvInfo.u4TotalPacketNo = 1;   // this will cause _SWDMX_WMVSendEOS immediately

                //_SWDMX_WMVSendEOS();
                //return FALSE;
            }
            _rSwdmxWmvInfo.u4PacketSize = (_rSwdmxWmvInfo.u4DataObjSize-WMV_DATA_OBJ_HEADER)/_rSwdmxWmvInfo.u4TotalPacketNo;

            if (_rSwdmxWmvInfo.u4PacketSize < _prRangeInfo->ui4_sz_min_acces_unt)
            {
                _rSwdmxWmvInfo.u4PacketSize = _prRangeInfo->ui4_sz_min_acces_unt;
            }
        }
        else
        {
            _rSwdmxWmvInfo.u4DataObjSize = 0xFFFFFFF0;
            _rSwdmxWmvInfo.u4TotalPacketNo = 0xFFFFFFF0;
        }
#else
        _rSwdmxWmvInfo.u4TotalPacketNo = 1416;
        _rSwdmxWmvInfo.u4PacketSize = 5175;
#endif
        LOG(5,"total data packet : %d \n",_rSwdmxWmvInfo.u4TotalPacketNo);
        LOG(5,"packet size : %d \n",_rSwdmxWmvInfo.u4PacketSize);

        //_rSwdmxWmvInfo.u4CurPos = WMV_DATA_OBJ_HEADER; //packet header size is 50 bytes
        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            _rSwdmxWmvInfo.u4PacketStartAddr += WMV_DATA_OBJ_HEADER;
            _rSwdmxWmvInfo.u4ConsumedSize = WMV_DATA_OBJ_HEADER;
        }
        else
        {
            _rSwdmxWmvInfo.u4ConsumedSize = 0;
        }

        _rSwdmxWmvInfo.fgFirstFrame = FALSE;

        if (_rSwdmxWmvInfo.fgEnVideo)
        {
            VDEC_StartPTS(ES0,TRUE,0,NULL);
        }

#if 1
        if (!_SWDMX_WMVFeederIsReady())
        {
            return TRUE;
        }
#endif

    }

    else if (!_rSwdmxWmvInfo.fgBlock && (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rSwdmxWmvInfo.u4TotalPacketNo)
             && !fgTrickMode )
    {
        if (!_SWDMX_WMVIsFeederBufferFull())
        {
            if ( _rSwdmxWmvInfo.fgFirstReqFeederData )
            {
                _SWDMX_WMVRequestFeederData(0,0,FALSE);
                _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
            }

            fgRet = _SWDMX_WMVGetFeederData(FALSE);
            if (!fgRet)
            {
                if (_rSwdmxWmvInfo.fgFeederInvalid || _rSwdmxWmvInfo.fgDemuxFinish)
                {
                    return FALSE;
                }
                else if ( _u4RequestDataSize ==0 )
                {
                    //need to request data again
                    _SWDMX_WMVRequestFeederData(0,0,FALSE);
                    _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
                }
            }
            else
            {
                //need to request data again
                _SWDMX_WMVRequestFeederData(0,0,FALSE);
                _rSwdmxWmvInfo.fgFirstReqFeederData = FALSE;
            }

            if (!_SWDMX_WMVFeederIsReady())
            {
                return TRUE;
            }

        }

        if ( _rSwdmxWmvInfo.u4ConsumedSize ==0 )
        {
            WMV_FEEDER_REQ_DATA_T rWmvFeederReqData;
            zMsgSize = sizeof(WMV_FEEDER_REQ_DATA_T);

            i4Ret = x_msg_q_receive(&u2MsgQIdx, &rWmvFeederReqData, &zMsgSize,
                                    &(_hWMVFeederDataQue), 1, X_MSGQ_OPTION_WAIT); 
            if (i4Ret != OSR_OK)
            {
                 ASSERT(0);
            }

            x_memcpy((VOID*)&_rFeederReqData,(VOID*)&rWmvFeederReqData.rFeederReqData,sizeof(FEEDER_REQ_DATA_T));
            _u4DefalutReqSize = rWmvFeederReqData.u4RequestFeederSize;

            _rSwdmxWmvInfo.u4FeederReadSize = _rFeederReqData.u4ReadSize;
            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederReqData.u4WriteAddr + _rFeederReqData.u4AlignOffset;
            FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxWmvInfo.u4PacketStartAddr);
        }
    }
    else if ((_rSwdmxWmvInfo.u4CurParsingPacketNo >= _rSwdmxWmvInfo.u4TotalPacketNo)
             && (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX))
    {
        _SWDMX_WMVSendEOS();
        return TRUE;
    }

    else if (fgTrickMode)
    {
        return _SWDMX_WMVTrickModeParsing();
    }

    //pasrsing packet
    //do
    {
        UINT32 u4AddrDelta;

        _rSwdmxWmvInfo.u2SendPayloadNo = 0;
        _rSwdmxWmvInfo.fgBlock = FALSE;

        u4AddrDelta = _rFeederInfo.u4EndAddr - _rSwdmxWmvInfo.u4PacketStartAddr;

        if (u4AddrDelta < _rSwdmxWmvInfo.u4PacketSize)
        {
            LOG(1,"swdmx wmv handle ring buffer\n");
            x_memcpy((VOID*)VIRTUAL(_rFeederInfo.u4StartAddr-u4AddrDelta),(VOID*)VIRTUAL(_rSwdmxWmvInfo.u4PacketStartAddr),u4AddrDelta);
            _rSwdmxWmvInfo.u4PacketStartAddr = _rFeederInfo.u4StartAddr-u4AddrDelta;
        }

        _WMVParsingPacket(_rSwdmxWmvInfo.u4PacketStartAddr,0,_rSwdmxWmvInfo.u4PacketSize);

        if (_rSwdmxWmvInfo.fgBrokenFile || _rSwdmxWmvInfo.fgDemuxFinish)
        {
            _SWDMX_WMVSendEOS();
            return TRUE;
        }

        if (_rSwdmxWmvInfo.fgBlock)
        {
            //LOG(0,"fgBlock : TRUE , leaving _SWDMX_WMVSendData\n");
            return FALSE;
        }
        else
        {
            _rSwdmxWmvInfo.u2PreviousSendPayloadNo = 0;
        }

        _rSwdmxWmvInfo.u4PacketStartAddr += _rSwdmxWmvInfo.u4PacketSize;

        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            _rSwdmxWmvInfo.u4ConsumedSize += _rSwdmxWmvInfo.u4PacketSize;
        }
        else
        {
            _rSwdmxWmvInfo.u4PacketStartAddr += WMV_NETFLIX_HEADER_SIZE;
            _rSwdmxWmvInfo.u4ConsumedSize += 1;
        }

        _rSwdmxWmvInfo.u4CurParsingPacketNo++;

        if (_rSwdmxWmvInfo.u4CurParsingPacketNo == _rSwdmxWmvInfo.u4TotalPacketNo
#ifdef __MODEL_slt__
                || _u4VC1SltLastPicPts > 33030
#endif
           )
        {
#ifdef __MODEL_slt__
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay();
            }
#endif
            _SWDMX_WMVSendEOS();
            return TRUE;
            //break;
        }

        FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rSwdmxWmvInfo.u4PacketStartAddr);

        if (_prRangeInfo->e_mm_src_type != MM_SRC_TYPE_NETWORK_NETFLIX)
        {
            if (_rSwdmxWmvInfo.u4ConsumedSize + _rSwdmxWmvInfo.u4PacketSize > _u4DefalutReqSize)
            {
                if (_rSwdmxWmvInfo.fgFirstFrame)
                {
                    _rSwdmxWmvInfo.fgFirstFrame = FALSE;
                }
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                return TRUE;
            }

            if (_rSwdmxWmvInfo.u4ConsumedSize + _rSwdmxWmvInfo.u4PacketSize > _rSwdmxWmvInfo.u4FeederReadSize)
            {
                LOG(0,"feeder read size is smaller than request data size\n");
                _SWDMX_WMVSendEOS();
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                return TRUE;
            }
        }
        else
        {
            if (_rSwdmxWmvInfo.u4ConsumedSize + 1 > _rSwdmxWmvInfo.u4FeederReadSize)
            {
                if (_rSwdmxWmvInfo.fgFirstFrame)
                {
                    _rSwdmxWmvInfo.fgFirstFrame = FALSE;
                }
                _rSwdmxWmvInfo.u4ConsumedSize = 0;
                //LOG(0,"swdmx finish consume feeder data size = %d\n",_rSwdmxWmvInfo.u4FeederReadSize);
                return TRUE;
            }
        }
        //return to main loop when finishing parsing a packet
        //LOG(0,"parsing packet number : %d/%d\n",_rSwdmxWmvInfo.u4CurParsingPacketNo,_rSwdmxWmvInfo.u4TotalPacketNo);
        return TRUE;
    }
    //} while (_rSwdmxWmvInfo.u4CurParsingPacketNo < _rSwdmxWmvInfo.u4TotalPacketNo);

    //return TRUE;
}

VOID _SWDMX_WMVSendEOS(VOID)
{
    DMX_MM_DATA_T rDmxMMData_EOS;
    //UINT32 u4DummyData[3]={0xFFFFFFFF};

#ifdef __MODEL_slt__
    LOG(5, "VC1 SLT last pic type = 0x%X, PTS = %d\n", _u1VC1SltLastPicType, _u4VC1SltLastPicPts);
#endif

    x_memset((VOID*)&rDmxMMData_EOS,0,sizeof(DMX_MM_DATA_T));
    rDmxMMData_EOS.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData_EOS.u4BufEnd = _rFeederInfo.u4EndAddr;
    rDmxMMData_EOS.u4StartAddr = _rFeederInfo.u4StartAddr;
    rDmxMMData_EOS.u4FrameSize = 4;
    rDmxMMData_EOS.fgEOS = TRUE;
    rDmxMMData_EOS.fgFrameHead= TRUE;
    rDmxMMData_EOS.fgEOS = TRUE;
    LOG(3,"!!!!!!!! deliever EOS flag\n");

    //update variable to know we have sent EOS flag
    _rSwdmxWmvInfo.fgDemuxFinish = TRUE;
    _prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;

    if (_rSwdmxWmvInfo.fgEnVideo)//protect func
    {
        if (_SWDMX_WMVInitDmx(eSWDMX_MEDIA_VIDEO, TRUE, TRUE) == FALSE)
        {
            LOG(0,"set dmx error when sending EOS flag\n");
            return;
        }

        if (_WMVDmxMoveData(eSWDMX_MEDIA_VIDEO, &rDmxMMData_EOS, 500) != TRUE)
        {
            LOG(0, "Demuxer fails to move frame start_code data.\n");
        }
    }
    if (_rSwdmxWmvInfo.fgEnAudio)
    {
        if (_WMVDmxMoveData(eSWDMX_MEDIA_AUDIO, &rDmxMMData_EOS, 500) != TRUE)
        {
            LOG(0, "Demuxer fails to move frame start_code data.\n");
        }
    }

}


BOOL _SWDMX_WMVInit(ENUM_SWDMX_FMT_T eType)

{
    BOOL fgRet;

    LOG(5, "_SWDMX_WMVInit \n");
    FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE,
                          _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo);

    //set audio call back function
    AUD_DrvSetWMAErrFunc(0,_WMVAudioErrorCallback);

    // Initialize dmx
    fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
    if (fgRet == FALSE)
    {
        LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM\n");
        return fgRet;
    }

    DMX_SetToDecoder(TRUE);
    {
        DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
        rDecoderCallbacks.pfnSendVideoPes = _WMVVideoCallback;//_VideoCallback;
        rDecoderCallbacks.pfnSendAudioPes = _WMVAudioCallback;//AUD_SendAudioPes;
        rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
        //rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;
        DMX_SetDecoderCallbacks(&rDecoderCallbacks);
    }

#if 1  // sometimes &_eCurFMT is not 4 byte aligned so that accessing it as UINT32* will cause data abort
    {
        UINT32 u4CurFMT;

        _SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE,(UINT32*)(&u4CurFMT),0,0);
        _eCurFMT = (ENUM_SWDMX_FMT_T)u4CurFMT;
    }
#else
    _SWDMX_GetInfo(eSWDMX_CONTAINER_TYPE,(UINT32*)(&_eCurFMT),0,0);
#endif

    x_memset(&_rSwdmxWmvInfo,0,sizeof(SWDMX_WMV_INFO_T));

    if ( _hWMVFeederDataQue == NULL )
    {
        // create receive feeder data queue
        VERIFY(x_msg_q_create(&_hWMVFeederDataQue, "SWDMX_WMV_FEEDER_DATA_QUEUE", sizeof(WMV_FEEDER_REQ_DATA_T),
                              SWMDX_WMV_FEEDER_DATA_Q_SIZE) == OSR_OK);
    }
    _SWDMX_WMVFlushFeederDataQ();


    _rSwdmxWmvInfo.fgFirstFrame = TRUE;
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.fgBeginPlay = TRUE;
    _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
    _rSwdmxWmvInfo.u8SeekPTS <<= 32;
    _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
    _rSwdmxWmvInfo.u4AStreamId = 0xff;
    _rSwdmxWmvInfo.u4VStreamId = 0xff;
    _fgNetflixEos = FALSE;
    _fgDataExhausted = TRUE;
    _u1AudioStreamId = 0xFF;
    _u4PrevFramePTS = 0;
    _u4DefalutReqSize = 0;
    _fgReceiveAudioPes = FALSE;
    _fgReceiveVideoPes = FALSE;
#ifdef ENABLE_WMV_DRM
    _u8SampleId =0;
    _rSwdmxWmvInfo.fgIsDRM = TRUE;
#endif

    if (_pu1VideoFrameData == NULL)
    {
        _pu1VideoFrameData = BSP_AllocAlignedDmaMemory(1*1024,16);
    }

#ifdef __MODEL_slt__
    _u4VC1SltLastPicPts = 0;
#endif

    return TRUE;
}

VOID _SWDMX_WMVSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                        UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{

    //UINT16 u2idx =0;
    //MIDXBULD_ELMT_STRM_ENTRY_T* prIdxEntry;
    UINT64 u8Remainder;

    LOG(5, "_SWDMX_WMV_SET_RANGE\n");

    if (prRangeList != NULL &&
            prRangeList->prDmxRangeInfo != NULL)
    {
        MM_RANGE_ELMT_IDXTBL_T* pBuldIdxTbl =
            (MM_RANGE_ELMT_IDXTBL_T*) prRangeList->prDmxRangeInfo->pt_idxtbl_lst;

        MM_RANGE_INFO_T* rRangeInfo = (prRangeList->prDmxRangeInfo);
        ASSERT(rRangeInfo != NULL);

        _prRangeInfo = rRangeInfo;
        //x_memcpy(&_rRangeInfo,prRangeList->prDmxRangeInfo,sizeof(MM_RANGE_INFO_T));
        //_prRangeInfo = &_rRangeInfo;
        _prRangeList = prRangeList;

        _rSwdmxWmvInfo.u4PreRoll = (UINT32)u8Div6432(prRangeList->prDmxRangeInfo->t_pts_offset, 90, &u8Remainder);
        _rSwdmxWmvInfo.u4FrameRate = 24;
        _rSwdmxWmvInfo.u8LastFileParsingPosition = prRangeList->prDmxRangeInfo->ui8_fileoffset;
        //_rSwdmxWmvInfo.u4PreRoll = (prRangeList->prDmxRangeInfo->t_pts_offset/90);
        _rSwdmxWmvInfo.u8FileSize = prRangeList->prDmxRangeInfo->z_range_sz;

        while (pBuldIdxTbl != NULL)
        {

            if (pBuldIdxTbl->e_elem_type == MM_ELEMT_TYPE_KEY && pBuldIdxTbl->t_strm_id.u.t_stm_asf_id ==u4VidStrmID)
            {
                if (_rSwdmxWmvInfo.prWmvKeyIdxTbl ==NULL)
                    _rSwdmxWmvInfo.prWmvKeyIdxTbl = pBuldIdxTbl;
            }
            pBuldIdxTbl = (MM_RANGE_ELMT_IDXTBL_T*)pBuldIdxTbl->pv_next_tbl;
        }

        /*if(_prRangeInfo->e_mm_src_type == MM_SRC_TYPE_NETWORK)
        {
            FeederSetInfo(FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_NETWORK);
        }
        else
        {
            FeederSetInfo(FEEDER_SWDMX_SRC_TYPE,FEEDER_SRC_TYPE_HIGH_SPEED_STORAGE);
        }*/
        LOG(0,"pts offset : %d\n",_rSwdmxWmvInfo.u4PreRoll);

        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;


    }
    else
    {
        LOG(0, "Demuxer Range is NULL\n");
    }
}

SWDMX_HDR_PRS_STATE_T _SWDMX_WMVHeaderParse(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    UNUSED(i4PlaySpeed);
    UNUSED(fgSpeedChange);

    VDEC_ES_INFO_T *prVdecEsInfo = _VDEC_GetEsInfo(0);

    LOG(9, "_SWDMX_WMV_HEADER_PARSE\n");

    if (_rSwdmxWmvInfo.fgBeginPlay && i4PlaySpeed == 1000)
    {
        //VDP_TriggerAudReceive(0);
        if (_rSwdmxWmvInfo.fgEnAudio)//protect func
        {
            AUD_SetStartPts(ES0,0);
        }

        if (_rSwdmxWmvInfo.fgEnVideo)//protect func
        {
            VDEC_SetRenderFromPts(ES0,0);
        }

        if (_rSwdmxWmvInfo.fgEnVideo  // make sure that video is prior than audio
            && _rSwdmxWmvInfo.eVideoType != ENUM_SWDMX_VDEC_UNKNOWN)  // DTV00079781
        {
            _rSwdmxWmvInfo.u4TriggerAudioPTS = 0xF0000000;
            _rSwdmxWmvInfo.u4DisplayPTS = 0xF0000000;
        }
        else
        {
            _rSwdmxWmvInfo.u4TriggerAudioPTS = 0x00000000;
            _rSwdmxWmvInfo.u4DisplayPTS = 0x00000000;
        }

        _rSwdmxWmvInfo.fgBeginPlay = FALSE;

        if (_rSwdmxWmvInfo.fgEnAudio)//protect flag
        {
            _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        }
        if (_rSwdmxWmvInfo.fgEnVideo)//protect flag
        {
            _rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
        }
        //_fgGotVideoFrame = FALSE;
        //_fgVideoPause = FALSE;
#ifdef WAIT_AUDIO_READY
        if (!_rSwdmxWmvInfo.fgVideoPause && _rSwdmxWmvInfo.fgEnVideo)
        {
            VDP_SetPauseMm(B2R_1, TRUE);
            VDEC_Pause(ES0);
            _rSwdmxWmvInfo.fgVideoPause = TRUE;
        }
#endif
        STC_StopStc();
        STC_SetStcValue((UINT32)(0-3000));
    }

#if 0
    if (i4PlaySpeed == 1000 && ((_rSwdmxWmvInfo.i4WMVPlaySpeed>=1 && _rSwdmxWmvInfo.i4WMVPlaySpeed<1000)||_rSwdmxWmvInfo.i4WMVPlaySpeed >= 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed<=-2000))
    {
        _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        _rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
    }
#else
    if (i4PlaySpeed == 1000 && (_rSwdmxWmvInfo.i4WMVPlaySpeed > 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed < 0))
    {
        _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
#ifdef WMV_2X_TO_1X_PREBUFFER
        UINT8 u1Idx = 0;
        UINT32 u4APts = 0;
        for ( ; u1Idx < 1/*_rSwdmxWmvInfo.u1TotalATrack*/; u1Idx++) // !!!
        {
            DMX_AudHandler_SeekAndUpdate(u1Idx + WMV_AUD_PREBUF_IDX, 0xffffffff, &u4APts);
        }
#endif
    }
    if (i4PlaySpeed == 1000 && (_rSwdmxWmvInfo.i4WMVPlaySpeed != 1000 && _rSwdmxWmvInfo.i4WMVPlaySpeed >= 1 && _rSwdmxWmvInfo.i4WMVPlaySpeed <= 2000))
    {
#ifndef WMV_2X_TO_1X_PREBUFFER
        _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
        //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
#endif
    }
#endif

    // stop resume handling
    if (prVdecEsInfo->fgFmtDectFinish&& _prRangeInfo->t_vid_start_render_pts != 0)
    {
        fgSpeedChange = TRUE;
        u8SeekPos = _prRangeInfo->t_vid_start_render_pts;
        _prRangeInfo->t_vid_start_render_pts = 0;
    }

    //handle speed change
    if (fgSpeedChange || _rSwdmxWmvInfo.fgChangeAudio)
    {
        UINT16 u2DecodingOrder;
        INT32 i4TempRef;
        UINT64 u8FrameOffset =0;
        UINT64 u8IFrameOffset =0;
        VDEC_ES_INFO_T *prVdecEsInfo;

        prVdecEsInfo = _VDEC_GetEsInfo(ES0);
        if (_rSwdmxWmvInfo.fgEnVideo)//protect func
        {
            VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder,
                                &u8FrameOffset, &u8IFrameOffset);
        }
        _rSwdmxWmvInfo.fgFinishKeyTable = FALSE;

        LOG(6,"speed %d: %d \n",_rSwdmxWmvInfo.i4WMVPlaySpeed,i4PlaySpeed);

        if (_rSwdmxWmvInfo.fgChangeAudio)
        {
#if 0
            UINT32 u4CurPts;
            u4CurPts = VDP_GetPts(0);

            _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
            //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
            _rSwdmxWmvInfo.u2SendPayloadNo = 0;
            _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
            _rSwdmxWmvInfo.fgBlock = FALSE;
            _rSwdmxWmvInfo.u4DisplayPTS= u4CurPts;

            _rSwdmxWmvInfo.fgChangeAudio = FALSE;
            _rSwdmxWmvInfo.u4AudioCount = 0;
            _rSwdmxWmvInfo.u4WmaErrorCode = 0;
            _rSwdmxWmvInfo.fgFirstAudioError = FALSE;

            MPV_FlushEsmQ(0, TRUE);
            VDEC_ReleaseDispQ(0);
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);


            VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder,
                                &u8FrameOffset, &u8IFrameOffset);
            _rSwdmxWmvInfo.u4CurParsingPacketNo = (UINT32)u8IFrameOffset;
            _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                    _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

            _SWDMX_WMVComsumeFeeder();

            VDEC_SetRenderFromPts(ES0, u4CurPts);
            VDEC_StartPTS(ES0,TRUE,u4CurPts,NULL);

            _SWDMX_WMVFlushFeederDataQ();
            _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
            _rSwdmxWmvInfo.u4ConsumedSize = 0;

            _fgReceiveVideoPes = FALSE;
            _fgReceiveAudioPes = FALSE;
#ifdef WAIT_AUDIO_READY

            if (!_rSwdmxWmvInfo.fgVideoPause && _rSwdmxWmvInfo.fgEnVideo)
            {
                VDP_SetPauseMm(B2R_1, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxWmvInfo.fgVideoPause = TRUE;
            }
#endif
            STC_StopStc();
            STC_SetStcValue((UINT32)u4CurPts);
#else
            _rSwdmxWmvInfo.fgChangeAudio = FALSE;
            _rSwdmxWmvInfo.u4AudioCount = 0;
            _rSwdmxWmvInfo.u4WmaErrorCode = 0;
            _rSwdmxWmvInfo.fgFirstAudioError = FALSE;
            if (_rSwdmxWmvInfo.fgEnAudio)//DTV00213648
            {
                AUD_SetStartPts(ES0,_rSwdmxWmvInfo.u4CurPTS);
            }
#endif
        }
        if (((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
                || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF))
        {
            _rSwdmxWmvInfo.u8SeekPTS = u8SeekPos;
            _rSwdmxWmvInfo.u4DisplayPTS = (UINT32)u8SeekPos;
            _rSwdmxWmvInfo.fgAudioNormalPlay = TRUE;
            //_rSwdmxWmvInfo.fgVideoNormalPlay = TRUE;
            _rSwdmxWmvInfo.u2SendPayloadNo = 0;
            _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
            _rSwdmxWmvInfo.fgBlock = FALSE;

            if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {
                VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder,
                                    &u8FrameOffset, &u8IFrameOffset);
            }

            if (_eCurFMT == SWDMX_FMT_WMA_AUDIO_ES)
            {
                _SWDMX_WMVComsumeFeeder();
                _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER;
                _rSwdmxWmvInfo.u4CurParsingPacketNo = 0;
            }
            else
            {
                _SWDMX_WMVSeek(u8SeekPos, prRangeList);
            }
            //to be done , speed up searching time

            if ((i4PlaySpeed >0)&&(_rSwdmxWmvInfo.fgEnVideo))
            {
                VDEC_SetRenderPts(ES0,(UINT32)u8SeekPos);
            }
            else if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {

                VDEC_SetRenderPts(ES0,0);
            }

            if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {
                MPV_FlushEsmQ(0, TRUE);
                VDEC_ReleaseDispQ(0);
                DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            }

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
            _SWDMX_WMVFlushFeederDataQ();
            _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
            _rSwdmxWmvInfo.fgDemuxFinish = FALSE;
            _rSwdmxWmvInfo.u4ConsumedSize = 0;

            _fgReceiveVideoPes = FALSE;
            _fgReceiveAudioPes = FALSE;
            _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
            _rSwdmxWmvInfo.u8SeekPTS <<= 32;
            _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
            if (_rSwdmxWmvInfo.fgEnVideo)//protect func
            {
                VDEC_StartPTS(ES0,TRUE,(UINT32)u8SeekPos,NULL);
            }
            //_fgGotVideoFrame = FALSE;
#ifdef WAIT_AUDIO_READY
            if (!_rSwdmxWmvInfo.fgVideoPause && i4PlaySpeed !=1 && _rSwdmxWmvInfo.fgEnVideo)
            {
                VDP_SetPauseMm(B2R_1, TRUE);
                VDEC_Pause(ES0);
                _rSwdmxWmvInfo.fgVideoPause = TRUE;
            }
#endif
            STC_StopStc();
            STC_SetStcValue((UINT32)u8SeekPos);

#ifdef WMV_2X_TO_1X_PREBUFFER
            UINT8 u1Idx = 0;
            UINT32 u4APts = 0;
            for ( ; u1Idx < 1/*_rSwdmxWmvInfo.u1TotalATrack*/; u1Idx++) // !!!
            {
                DMX_AudHandler_SeekAndUpdate(u1Idx + WMV_AUD_PREBUF_IDX, 0xffffffff, &u4APts);
            }
#endif
        }
        // 1x->2x
        else if ((_rSwdmxWmvInfo.i4WMVPlaySpeed>1 && _rSwdmxWmvInfo.i4WMVPlaySpeed <= 1000)&&
                 (/*i4PlaySpeed==2000 || */((i4PlaySpeed >= 1) && (i4PlaySpeed < 1000))))
        {
            //do nothing
            if (_rSwdmxWmvInfo.fgVideoPause && i4PlaySpeed==1)
            {
                _WMVStartToPlay();
            }

#ifdef WMV_2X_TO_1X_PREBUFFER
            //if (INPUT_SRC != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
            {
                // Set active pic to 255
                DMX_AudHandler_SetActivePidx(DMX_AUDIO_HANDLER_NULL_PIDX);
            }
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
            _rSwdmxWmvInfo.fg2XAudParsing = TRUE;
#endif
        }
#ifdef WMV_2X_TO_1X_PREBUFFER
        // 2x->1x (step? slow forward?)
        //else if (_rSwdmxWmvInfo.i4WMVPlaySpeed >= 2000 && _rSwdmxWmvInfo.i4WMVPlaySpeed <= 2000 && i4PlaySpeed == 1000)
        else if (_rSwdmxWmvInfo.fg2XAudParsing && i4PlaySpeed == 1000)
        {
            //_fgMegaaTest = TRUE;
#if 1
            UINT32 u4CurPts;

            u4CurPts = VDP_GetPts(0);
            _rSwdmxWmvInfo.u4DisplayPTS= u4CurPts;
            _rSwdmxWmvInfo.u4TriggerAudioPTS = u4CurPts;

            if (prVdecEsInfo != NULL)
            {
                //reset seek operation flag when speed change
                if (prVdecEsInfo->fgRenderPts)
                {
                    prVdecEsInfo->fgRenderPts = FALSE;
                }
            }
#endif
            //_fgReceiveVideoPes = FALSE;  !!!
            //_fgReceiveAudioPes = FALSE;  !!!

            if (_rSwdmxWmvInfo.i4WMVPlaySpeed != 2000)  // step => 1X
            {
                STC_StopStc();
                STC_SetStcValue(u4CurPts + _i4WmvStcAdj);
                STC_StartStc();
            }

            UINT32 u4APts;
            BOOL fgRet;
            fgRet = DMX_AudHandler_SeekAndUpdate(/*_rSwdmxWmvInfo.u1CurATrackIdx + */WMV_AUD_PREBUF_IDX,
                    _rSwdmxWmvInfo.u4TriggerAudioPTS, &u4APts);

            if (fgRet)
            {
                LOG(5, "Find audio in the pre-buffer\n");
                _rSwdmxWmvInfo.u4TriggerAudioPTS = u4APts;
            }
            else
            {
                LOG(5, "Can't find audio in the pre-buffer\n");
                ASSERT(0);  // !!!
                //_rSwdmxWmvInfo.u4TriggerAudPts = _rSwdmxAviInfo.u4AudPts[_rSwdmxAviInfo.u1CurATrackIdx];
            }

            AUD_SetStartPts(0, _rSwdmxWmvInfo.u4TriggerAudioPTS);
            DMX_AudHandler_SetActivePidx(/*_rSwdmxWmvInfo.u1CurATrackIdx + */WMV_AUD_PREBUF_IDX);  // !!!

            LOG(5, "_rSwdmxWmvInfo.u4TriggerAudioPTS = 0x%08x\n", _rSwdmxWmvInfo.u4TriggerAudioPTS);
            _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
        }
#endif  // WMV_2X_TO_1X_PREBUFFER
        else if(fgSpeedChange)
        {
            UINT32 u4CurPts;

            if (_rSwdmxWmvInfo.prWmvKeyIdxTbl == NULL)
            {
                return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
            }

            u4CurPts = VDP_GetPts(0);

            _rSwdmxWmvInfo.u4DisplayPTS= u4CurPts;

            //MPV_FlushEsmQ(0, TRUE);
            //VDEC_ReleaseDispQ(0);
            if (prVdecEsInfo != NULL)
            {
                //reset seek operation flag when speed change
                if (prVdecEsInfo->fgRenderPts)
                {
                    prVdecEsInfo->fgRenderPts = FALSE;
                }
            }

            DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
            DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
#ifdef WMV_2X_TO_1X_PREBUFFER
            _rSwdmxWmvInfo.fg2XAudParsing = FALSE;
#endif

            _SWDMX_FlushReqMsgQ();
            _SWDMX_WMVFlushFeederDataQ();
            _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
            _rSwdmxWmvInfo.u4ConsumedSize = 0;

            _fgReceiveVideoPes = FALSE;
            _fgReceiveAudioPes = FALSE;
            //VDEC_SetRenderFromPts(0, u4CurPts);
            LOG(0,"VDEC_SetRenderFromPts : %x\n",u4CurPts);

            // Things for 32/16/8/4 -> 1X are the same
            if ((_rSwdmxWmvInfo.i4WMVPlaySpeed >= 2000 || _rSwdmxWmvInfo.i4WMVPlaySpeed < 0
                    || (_rSwdmxWmvInfo.i4WMVPlaySpeed >=1 && _rSwdmxWmvInfo.i4WMVPlaySpeed<1000))
                    && (i4PlaySpeed == 1000 || i4PlaySpeed==2000 || i4PlaySpeed==500))
            {
                //UINT32 u4RemainedDataSize;
                VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder,
                                    &u8FrameOffset, &u8IFrameOffset);
                _rSwdmxWmvInfo.u2SendPayloadNo = 0;
                _rSwdmxWmvInfo.u2PreviousSendPayloadNo =0;
                _rSwdmxWmvInfo.fgBlock = FALSE;
                _rSwdmxWmvInfo.u4CurParsingPacketNo = (UINT32)u8IFrameOffset;
                _rSwdmxWmvInfo.u8LastFileParsingPosition = _prRangeInfo->ui8_fileoffset + WMV_DATA_OBJ_HEADER +
                        _rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize;

                //u4RemainedDataSize = _rSwdmxWmvInfo.u4DataObjSize-
                //(_rSwdmxWmvInfo.u8LastFileParsingPosition-prRangeList->prDmxRangeInfo->ui8_fileoffset);
                _SWDMX_WMVComsumeFeeder();

                //since pts is not precious, jump to next 2 frame to avoid frame flicking
                if (_rSwdmxWmvInfo.eVideoType == ENUM_SWDMX_VDEC_VC1 && _rSwdmxWmvInfo.i4WMVPlaySpeed ==2000)
                {
                    u4CurPts += 6000;
                }
                
                if((_rSwdmxWmvInfo.i4WMVPlaySpeed >=1 && _rSwdmxWmvInfo.i4WMVPlaySpeed<1000)&&(i4PlaySpeed == 1000))
                {
                    UINT32 u4CurSTC;
                    u4CurSTC = STC_GetStcValue();
                    u4CurPts = max(u4CurPts,u4CurSTC);
                }
                
                VDEC_SetRenderFromPts(ES0, u4CurPts);

                LOG(3,"vdp get PTS : %d\n",(u4CurPts/90000));
                VDEC_StartPTS(ES0,TRUE,u4CurPts,NULL);

#ifdef WAIT_AUDIO_READY
                if (!_rSwdmxWmvInfo.fgVideoPause && _rSwdmxWmvInfo.fgEnVideo)
                {
                    VDP_SetPauseMm(B2R_1, TRUE);
                    VDEC_Pause(ES0);
                    _rSwdmxWmvInfo.fgVideoPause = TRUE;
                }
#endif
                STC_StopStc();
                STC_SetStcValue(u4CurPts);
            }
            // Things for 32/16/8/4/2X/1X -> -2X/-4X/-8X/-16X/-32X are the same
            else if (_rSwdmxWmvInfo.i4WMVPlaySpeed > 0 && i4PlaySpeed < 0 )
            {
                _SWDMX_WMVSeek(u4CurPts, prRangeList);
                VDEC_SetRenderFromPts(ES0,0);

            }
            //else if( _rSwdmxWmvInfo.i4WMVPlaySpeed >=2000 && i4PlaySpeed>2000)
            else if (i4PlaySpeed>=2000)
            {
                _SWDMX_WMVSeek(u4CurPts, prRangeList);
                if (_rSwdmxWmvInfo.u4CurParsingPacketNo < u8IFrameOffset)
                    _rSwdmxWmvInfo.u4CurParsingPacketNo = (UINT32)u8IFrameOffset;
                VDEC_SetRenderFromPts(ES0, u4CurPts);
            }
            else if (i4PlaySpeed <=-2000)
            {
                _SWDMX_WMVSeek(u4CurPts, prRangeList);
                VDEC_SetRenderFromPts(ES0, 0);
            }
            else
            {
                //null operation
            }

        }

        if (!((_rSwdmxWmvInfo.i4WMVPlaySpeed==1000 && (i4PlaySpeed==2000 || i4PlaySpeed==1 ||i4PlaySpeed == 500))
            ||(_rSwdmxWmvInfo.i4WMVPlaySpeed > 1 && _rSwdmxWmvInfo.i4WMVPlaySpeed < 1000 && i4PlaySpeed >1 && i4PlaySpeed < 1000)))
        {
            if (_rSwdmxWmvInfo.fgDemuxFinish && _rSwdmxWmvInfo.u4PacketSize != 0)
            {
                SET_DECFLAG(prVdecEsInfo, DEC_FLG_WAIT_NEXT_I);
                AUD_ClearEOSFlag(0);

                if (_SWDMX_WMVInitDmx(eSWDMX_MEDIA_VIDEO, TRUE, FALSE) == FALSE)
                {
                    LOG(0,"set dmx error when sending EOS flag\n");
                    //return;
                }

                _rSwdmxWmvInfo.fgDemuxFinish = FALSE;
            }
            if (prRangeList->eRangeStats == eSWDMX_RANGE_STATE_PENDING && _rSwdmxWmvInfo.u4PacketSize != 0)
            {
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
            }
        }
    }

    if (_rSwdmxWmvInfo.i4WMVPlaySpeed != i4PlaySpeed)
        _rSwdmxWmvInfo.i4WMVPlaySpeed = i4PlaySpeed;
    //_rSwdmxWmvInfo.u4RepeatTimes = (UINT32)(_rSwdmxWmvInfo.u1FrameRate * 2000)/(abs(i4PlaySpeed));

    //_rSwdmxWmvInfo.fgWMVSpeedChange = fgSpeedChange;
    //_rSwdmxWmvInfo.u8WMVFileOffset = u8SeekPos;

    if (_SWDMX_WMVDeliverData(prRangeList) == FALSE)
    {
        LOG(9, "_SWDMX_WMV_HEADER_PARSE FALSE\n");
        return eSWDMX_HDR_PRS_FED_MOVE_FAIL;
    }
    return eSWDMX_HDR_PRS_SUCCEED;
}


/**
*/
BOOL _SWDMX_WMVStop(VOID)
{
    DMX_MM_T rMM;

    //BOOL fgEnVDmx = _rSwdmxWmvInfo.fgEnVDmx;
    //BOOL fgEnADmx = _rSwdmxWmvInfo.fgEnADmx;

    LOG(5, "_SWDMX_WMV_STOP\n");

    _prRangeList = NULL;
    _prRangeInfo = NULL;
    //_prWmvGenericInfo = NULL;
    //_pprStmAttribute = NULL;

#ifdef WMV_ALLOCATE_VFIFO
    if (_u4VFifoStartAddr!= NULL)
    {
        BSP_FreeAlignedDmaMemory((VOID*)_u4VFifoStartAddr);
        _u4VFifoStartAddr = NULL;
    }
#endif

    if (_rSwdmxWmvInfo.fgEnVideo)//protect func
    {
        DMX_MM_FlushBuffer(eSWDMX_MEDIA_VIDEO);
    }
    if (_rSwdmxWmvInfo.fgEnAudio)//protect func
    {
        DMX_MM_FlushBuffer(eSWDMX_MEDIA_AUDIO);
    }
    //_SWDMX_WMVFlushFeederDataQ();

    rMM.fgEnable = FALSE;

    if(_rSwdmxWmvInfo.fgEnVDmx)
    {
        _rSwdmxWmvInfo.fgEnVDmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_MM_FLAG_VALID, &rMM));
        DMX_MM_Free(eSWDMX_MEDIA_VIDEO);
    }

    if(_rSwdmxWmvInfo.fgEnADmx)
    {
        _rSwdmxWmvInfo.fgEnADmx = FALSE;
        VERIFY(DMX_MM_Set(eSWDMX_MEDIA_AUDIO, DMX_MM_FLAG_VALID, &rMM));
        DMX_MM_Free(eSWDMX_MEDIA_AUDIO);
    }

    x_memset(&_rSwdmxWmvInfo,0,sizeof(SWDMX_WMV_INFO_T));

    _rSwdmxWmvInfo.fgFirstFrame = TRUE;
    _rSwdmxWmvInfo.fgFirstReqFeederData = TRUE;
    _rSwdmxWmvInfo.fgBeginPlay = TRUE;
    _rSwdmxWmvInfo.u8SeekPTS = 0xFFFFFFFF;
    _rSwdmxWmvInfo.u8SeekPTS <<= 32;
    _rSwdmxWmvInfo.u8SeekPTS += 0xFFFFFFFF;
    _rSwdmxWmvInfo.u4AStreamId = 0xff;
    _rSwdmxWmvInfo.u4VStreamId = 0xff;
    _fgNetflixEos = FALSE;
    _fgDataExhausted = TRUE;
    _u1AudioStreamId = 0xFF;
    _u4PrevFramePTS = 0;
    _u4DefalutReqSize = 0;
    _fgReceiveAudioPes = FALSE;
    _fgReceiveVideoPes = FALSE;

    _SWDMX_WMVFlushFeederDataQ();

#if 0
    if (_pu1VideoFrameData != NULL)
    {
        BSP_FreeAlignedDmaMemory(_pu1VideoFrameData);
        _pu1VideoFrameData = NULL;
    }
#endif

#ifdef ENABLE_WMV_DRM
    _u8SampleId =0;
    _rSwdmxWmvInfo.fgIsDRM = TRUE;
#endif

    //_rSwdmxWmvInfo.fgEnVDmx = fgEnVDmx;
    //_rSwdmxWmvInfo.fgEnADmx = fgEnADmx;

#ifdef WMV_2X_TO_1X_PREBUFFER
    DMX_MM_T rPid;
    rPid.fgEnable = FALSE;
    int i;
    for (i = 0; i < 1/*WMV_MAX_AUD_TRACK*/; i++)  // !!!
    {
        VERIFY(DMX_MM_Set(WMV_AUD_PREBUF_IDX + i, DMX_MM_FLAG_VALID, &rPid));
        DMX_MM_Free(WMV_AUD_PREBUF_IDX + i);
        DMX_AudHandler_Free(WMV_AUD_PREBUF_IDX + i);
    }
    DMX_AudHandler_SetEnable(FALSE);
#endif

    return TRUE;
}

BOOL _SWDMX_WMVSetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
    if (u1Type == eSWDMX_SET_VIDEOTYPE)
    {
#if 1  // disable WMV7/8
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_WMV7 || u4Para1 == (UINT32)ENUM_SWDMX_VDEC_WMV8)
        {
            return FALSE;
        }
        else
#endif
#if defined(CC_WMV_MPEG1_DISABLE) && defined(CC_WMV_MPEG2_DISABLE)
        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return FALSE;
        }
        else
#endif
#if defined(CC_WMV_MPEG4_DISABLE)
            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MPEG4)
            {
                return FALSE;
            }
            else
#endif
#if defined(CC_WMV_DX311_DISABLE)
                if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_DX311)
                {
                    return FALSE;
                }
                else
#endif
#if defined(CC_WMV_H264_DISABLE)
                    if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_H264)
                    {
                        return FALSE;
                    }
                    else
#endif
#if defined(CC_WMV_VC1_DISABLE)
                        if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_VC1)
                        {
                            return FALSE;
                        }
                        else
#endif
#if defined(CC_WMV_MJPEG_DISABLE)
                            if (u4Para1 == (UINT32)ENUM_SWDMX_VDEC_MJPEG)
                            {
                                return FALSE;
                            }
                            else
#endif
                            {
                                _rSwdmxWmvInfo.eVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;
                            }
    }
    else if (u1Type == eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxWmvInfo.eAudioType = (ENUM_SWDMX_ADEC_T)u4Para1;
    }
    else if (u1Type == eSWDMX_SET_VIDSEQHEADER)
    {
#if 0
        _rSwdmxWmvInfo.u4SequenceHeader = u4Para1;
        _rSwdmxWmvInfo.u4SequenceHeaderLength = u4Para2;
#else
        x_memcpy((VOID*)(VIRTUAL(&_pu1VideoFrameData[0])), (VOID*)VIRTUAL(u4Para1), (u4Para2));
        _rSwdmxWmvInfo.u4SequenceHeader = (UINT32)VIRTUAL(&_pu1VideoFrameData[0]);
        _rSwdmxWmvInfo.u4SequenceHeaderLength = u4Para2;
#endif
    }
    else if (u1Type == eSWDMX_SET_NETFLIX_STREAM_ID)
    {
        _u4NetflixVideoStreamId = (UINT32)(u4Para1);
        _u4NetflixAudioStreamId = (UINT32)(u4Para2);
    }
    else if (u1Type == eSWDMX_SET_NETFLIX_EOS)
    {
        _fgNetflixEos = (BOOL)(u4Para1);
    }
    else if (u1Type == eSWDMX_SET_SYSTEMINFO)
    {
        _prWmvGenericInfo = (MINFO_TYPE_GENERIC_T*)(u4Para1);
        _u4NumberOfBitstream = (UINT32)(u4Para2);
        _WMVFindMaxPacketSize();
    }

    return TRUE;
}

BOOL _SWDMX_WMVGetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    LOG(5, "_SWDMX_WMVGetInfo u1Type = %d\n", u1Type);

    if (u1Type == eSWDMX_GET_CURRENT_POSITION)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_WMVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)pu4Para1;
        *pu8Tmp = 0;
        if (_prRangeInfo != NULL)
        {
            *pu8Tmp = (_prRangeInfo->ui8_fileoffset+(_rSwdmxWmvInfo.u4CurParsingPacketNo*_rSwdmxWmvInfo.u4PacketSize)+WMV_DATA_OBJ_HEADER);
        }
        return TRUE;
    }
    else if (u1Type == eSWDMX_GET_IS_TRICK)
    {
		if (_rSwdmxWmvInfo.prWmvKeyIdxTbl != NULL)
		{
			if (_rSwdmxWmvInfo.prWmvKeyIdxTbl->ui4_number_of_entry < WMV_KEY_TABLE_THRESHOLD)
			{
				*pu4Para1 = FALSE;
				if (pu4Para2 != NULL)
				{
					*pu4Para2 = FALSE;
				}
			}
			else
			{
				*pu4Para1 = TRUE;
				if (pu4Para2 != NULL)
				{
					*pu4Para2 = TRUE;
				}
			}
		}
		else
		{
			*pu4Para1 = FALSE;
			if (pu4Para2 != NULL)
			{
				*pu4Para2 = FALSE;
			}
		}

    }
    else if (u1Type == eSWDMX_GET_CURRENT_TIME)
    {
        if (pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_WMVGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }

        *pu4Para1 = STC_GetStcValue();

        return TRUE;
    }
    return FALSE;
}

BOOL _SWDMX_WMVAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
#ifdef WAIT_AUDIO_READY
    if (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        if (_rSwdmxWmvInfo.fgEnVideo)
        {
            VDP_TriggerAudReceive(_rSwdmxWmvInfo.u4TriggerAudioPTS);
            LOG(0,"!!!!!! audio is ready %d\n",_rSwdmxWmvInfo.fgVideoPause);
            if (_rSwdmxWmvInfo.fgVideoPause)
            {
                _WMVStartToPlay();
            }
        }
        else
        {
            STC_StartStc();
            AUD_MMAoutEnable(0, TRUE);
        }
    }
    else
    {
        //impossible
        //ASSERT(0);
    }
#endif
    return TRUE;
}

