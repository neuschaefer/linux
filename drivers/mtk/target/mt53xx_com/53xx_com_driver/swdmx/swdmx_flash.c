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
 * $RCSfile: swdmx_flash.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_flash.c
 *  Software demux for flash ES - internal interface
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "dmx_mm_if.h"
#include "vdec_drvif.h"
#include "aud_drvif.h"
#include "swdmx_drvif.h"
#include "swdmx_flash.h"
#include "swdmx_mpg_dmx.h"
#include "swdmx_mpg_pvr.h"
#include "swdmx_mpg_psipsr_if.h"
#include "fbm_drvif.h"
#include "x_util.h"
#include "swdmx_debug.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define SWDMX_FLASH_DEBUG_BUFFER
//#define SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DETECT_TIMEOUT  0
#define DETECT_FAIL     1
#define DETECT_OK       2

#define DATA_TIMEOUT  0
#define DATA_FAIL     1
#define DATA_OK       2
#define DATA_DUMMY    3

#define OPEN_TIMEOUT    500

// FixMe: remove it later
#define STREAM_TYPE_H264		0x1B
#define STREAM_TYPE_VC1 		0xEA
#define STREAM_TYPE_VIDEO		2
#define STREAM_TYPE_MPEG1		3
#define STREAM_TYPE_MPEG2		4
#define STREAM_TYPE_AC3_EU		0xBD
#define STREAM_TYPE_AC3			0x81
#define STREAM_TYPE_AAC			0x11
#define STREAM_TYPE_AAC_ISDB    0xF
#define STREAM_TYPE_CYPHER		0x80
#define STREAM_TYPE_UNKNOWN		0xFF


#define FLASH_ALIGN_SIZE (188*4) // 188 and 16 byte align
#define FLASH_ALIGN_SIZE_PVR (192) // 192 and 16 byte align
#define FLASH_ALIGN_SIZE_TS_192 (192) // 192 and 16 byte align
#define FLASH_BATCH_SIZE (188*128)
#define FLASH_BATCH_SIZE_PVR (192*128)
#define FLASH_BATCH_SIZE_TS_192 (192*128)
#define FLASH_BATCH_READ_SIZE (10)
#define FLASH_DMX_RESERVED_SIZE (FLASH_ALIGN_SIZE*5)
#define FLASH_SEND_OK     0
#define FLASH_SEND_FULL   1
#define FLASH_SEND_FAIL   2
#define FLASH_SEND_EOS    3

#define FLASH_REQ_NUM    4
#define FLASH_BUF_NUM    6
#define FLASH_PTS_CHECK_NUM 5

#define FLASH_MPEG2_SEQ_HDR_SIZE 12
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static FEEDER_BUF_INFO_T _rFeederInfo;

//static UINT32 _u4BufStart = 0;
//static UCHAR _aucBuf[5*1024*1024];

static UINT32 _fgSendSeqHdr = FALSE;
static UINT64 _u8OrgSendedSize = 0;

static BOOL _fgGetInfo = FALSE;
static UINT64 _u8SendedSize = 0;
static UINT64 _u8ReadedPos = 0;
static UINT64 _u8FileSize = 0;
static UINT32 _u4FeederReqID = 1;

static HAL_TIME_T _rJumpStartTime;
static HAL_TIME_T _rJumpEndTime;

#ifdef SWDMX_STATISTICS
static HAL_TIME_T _rFeederReqStartTime;
static HAL_TIME_T _rFeederReqEndTime;
static UINT32 _u4FeederAvgTime;
static UINT32 _u4FeederMaxTime;
static UINT32 _u4FeederMinTime;
static UINT32 _u4FeederCount;
#endif
	
static UINT8 _ucFlashSeqHdr[FLASH_MPEG2_SEQ_HDR_SIZE] = {0};
static VDEC_SEQUENCE_DATA_T _rSeqInfo;


static UINT8 _u1PidxVideo = 0xFF;
static UINT8 _u1PidxAudio = 0xFF;

static UINT32 _u4OneTimeSize = FLASH_BATCH_SIZE;
static UINT32 _u4AlignBase = FLASH_ALIGN_SIZE;

#ifdef CC_FLASH_TEST
typedef struct
{
    UINT32 au4PTS[FLASH_PTS_CHECK_NUM];
    UINT16 u2FlashPtsWIdx; 
    UINT16 u2FlashPtsRIdx;
}FLASH_TEST_PTS_T;
static FLASH_TEST_PTS_T _rFlashPTS[2] = {0};
static BOOL _fgSendVideo = FALSE;
static BOOL _fgVideoFirst = FALSE;
#endif
static SWDMX_INFO_T* _prSwdmxInfo = 0;
static ENUM_SWDMX_VDEC_T _eVideo = ENUM_SWDMX_VDEC_UNKNOWN;

typedef struct
{
    // this is buffer we keep after feeder's ack
    UINT32 u4BufAvailCnt;
    UINT32 u4BufReadIdx;
    UINT32 u4BufWriteIdx;
    UINT32 u4BufTotalSize;
    UINT32 au4BufStartAddr[FLASH_BUF_NUM];
    UINT32 au4BufSize[FLASH_BUF_NUM];

    // this is buffer under requesting to feeder
    UINT32 u4ReqTimes;
    UINT32 u4ReqReadIdx;
    UINT32 u4ReqWriteIdx;
    UINT32 u4ReqTotalSize;
    UINT32 au4ReqSize[FLASH_REQ_NUM];
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
    UINT32 au4ReqId[FLASH_REQ_NUM];
#endif

    // this is a small buffer we are consuming currently.
    UINT32 u4UsingBufSize;
    UINT32 u4UsingStartAddr;
//#ifdef FLASH_TEST
    UINT32 u4ActualSize;
//#endif
}SWDMX_FLASH_BUFFER_T;


typedef struct
{
    ENUM_SWDMX_FMT_T eFmt;
    
    ENUM_SWDMX_VDEC_T eVideoType;
    ENUM_SWDMX_ADEC_T eAudioType;
    ENUM_SWDMX_ADEC_T eAudio2Type;
        
	BOOL fgVideo;
	BOOL fgAudio;
	BOOL fgAudio2;
	BOOL fgSubTitle;
	UINT16 u2VideoId;
	UINT16 u2AudioId;
	UINT16 u2AudioSubId;
	UINT16 u2Audio2Id;
	UINT16 u2SubTitleId;
	
}SWDMX_FLASH_FMT_T;

typedef struct
{
    BOOL fgSendEOF;
    BOOL fgSendAudioEos;
    BOOL fgSendVideoEos;
    BOOL fgJumpping;
    BOOL fgFirst;
    BOOL fgNotToDecoder;
    BOOL fgNotToADec;
    BOOL fgResetPosAfterFoundSeq;
    BOOL fgVideoPTSFound;
    BOOL fgSetStc;
    BOOL fgJumpNext;
    BOOL fgCleanAudio;
    
    UINT16 u2OrderFromIPos;
    UINT16 u2JumpAgainCount;
    
    UINT32 u4FirstPTS;
    UINT32 u4FirstVideoPTS;
    UINT32 u4LastPTS;
    UINT32 u4LastPTSCnt;
    UINT32 au4LastPTS[FLASH_PTS_CHECK_NUM];
    UINT32 fgGotVideo;
    UINT32 fgGotAudio;
    UINT32 fgGotAudioReady;
    UINT32 fgVdecPause;
    UINT32 u4PicCnt;
    UINT32 u4TotalTime;
    UINT32 u4TotalTimeOffset;

    UINT32 u4LastVideoWptr;
    UINT32 u4LastAudioWptr;

    UINT64 u8PreviousIPos;
    UINT64 u8PreviousIPosWithPTSFound;
    UINT64 u8Previous2IPosWithPTSFound;
    UINT64 u8PreviousSendedSize;
    UINT64 u8PreviousSendedSizeOfNon1X;
    UINT64 u8JumpStartAddr;    
}SWDMX_FLASH_VARIABLE_T;

typedef struct
{
    SWDMX_FLASH_BUFFER_T rBuf;
    SWDMX_FLASH_FMT_T rFmt;
    SWDMX_FLASH_VARIABLE_T rVar;

}SWDMX_FLASH_INFO_T;

static SWDMX_FLASH_INFO_T _rSwdmxFlashInfo;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
VOID _SWDMX_FlashCreateFindSema(VOID);
VOID _SWDMX_FlashDeleteFindSema(VOID);
//INT32 _SWDMX_FlashSelectPSStream(DMX_NOTIFY_INFO_STREAM_ID_T *prInfo);
VOID _SWDMX_FlashFlushVFifo(VOID);
VOID _SWDMX_FlashFlushAFifo(VOID);
BOOL _SWDMX_FlashWatchVideoPTS(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_FlashWatchAudioPTS(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_FlashAudioCallback(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_FlashVideoCallback(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_FlashAudioCallbackParse(const DMX_AUDIO_PES_T* prPes);
#if 0
BOOL _SWDMX_FlashVideoCallbackFlashParse(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_FlashVideoCallbackH264Parse(const DMX_PES_MSG_T* prPes);
#endif
VOID _SWDMX_FlashSeekFinNotify(UINT32 u4PTS, BOOL fgAgain);

BOOL _SWDMX_FlashEsSetVideoId(VOID);
BOOL _SWDMX_FlashEsSetAudioId(VOID);
BOOL _SWDMX_FlashEsFreeVideoId(VOID);
BOOL _SWDMX_FlashEsFreeAudioId(VOID);

BOOL _SWDMX_FlashWatchDuration(UINT32 u4CurentTime);
VOID _SWDMX_FlashWatchLastVPTS(const DMX_PES_MSG_T* prPes);
VOID _SWDMX_FlashWatchLastAPTS(const DMX_AUDIO_PES_T* prPes);
VOID _SWDMX_FlashResetVariable(VOID);
VOID _SWDMX_FlashJumpToNormal(BOOL fgSeek, BOOL fgJumpping, INT32 i4Speed);
VOID _SWDMX_FlashResetVariable(VOID);
VOID _SWDMX_FlashHandleChange(INT32 i4PlaySpeed, UINT64 u8SeekPos);
VOID _SWDMX_FlashHandleEOS(VOID);
BOOL _SWDMX_FlashResetFeeder(VOID);
VOID _SWDMX_FlashResetData(VOID);
INT32 _SWDMX_FlashHandleFifoFull(VOID);
INT32 _SWDMX_FlashHandleMoveData(DMX_MM_DATA_T rDmxMMData);
INT32 _SWDMX_ReceiveFeederAck(VOID *pvData);
BOOL _SWDMX_FlashGetFirstSeqHdr(VOID);
VOID _SWDMX_FlashHandleSeek(INT32 i4PlaySpeed, BOOL fgSeek);
INT32 _SWDMX_FlashHandleFeeder(SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed);
INT32 _SWDMX_FlashSendData(SWDMX_RANGE_LIST_T* prRangeList, 
            INT32 i4PlaySpeed, UINT64 u8SeekPos, BOOL fgChange);
BOOL _SWDMX_FlashSendDataBatch(VOID);
BOOL _SWDMX_FlashSendDataBatchForTotalTime(VOID);
BOOL _SWDMX_FlashBaseInit(VOID);
BOOL _SWDMX_FlashOpen(ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prOpenInfo);
BOOL _SWDMX_FlashBaseGetInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo);
BOOL _SWDMX_FlashGetSeqInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);
        
#ifdef SWDMX_STATISTICS
VOID _CalFeederTime(VOID)
{
    UINT32 u4Delta;
    HAL_TIME_T rDelta;
    HAL_GetTime(&_rFeederReqEndTime);
    HAL_GetDeltaTime(&rDelta, &_rFeederReqStartTime, &_rFeederReqEndTime);
    
    u4Delta = rDelta.u4Micros;
    if (rDelta.u4Micros > 1000) // 1ms
    {
        _u4FeederAvgTime = ((_u4FeederAvgTime * _u4FeederCount) + u4Delta) / (_u4FeederCount + 1);

        _u4FeederCount ++;

        _u4FeederMaxTime = (_u4FeederMaxTime > u4Delta) ?_u4FeederMaxTime : u4Delta;
        _u4FeederMinTime = (_u4FeederMinTime < u4Delta) ? _u4FeederMinTime : u4Delta;
    }
    
    if ((_u4FeederCount % 1000) == (1000 - 1))
    {    
        LOG(0, "Feeder Count(%d) Time(%d, %d, %d)\n",
            _u4FeederCount,
            _u4FeederMaxTime, _u4FeederAvgTime, _u4FeederMinTime);
    }
}
#endif

static BOOL _SWDMX_DMX_Reset(void)
{
    {
#ifdef CC_FLASH_TEST
        return TRUE;
#else
        return DMX_DDI_RequestReset();
#endif
    }
}


VOID _SWDMX_FlashFlushVFifo(VOID)
{
    BOOL fgRet;
    if(_u1PidxVideo != 0xFF)
    {
        if(_rSwdmxFlashInfo.rVar.u4LastVideoWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(_u1PidxVideo, _rSwdmxFlashInfo.rVar.u4LastVideoWptr, _rSwdmxFlashInfo.rVar.u4LastVideoWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_FlashFlushVFifo DMX_UpdateReadPointer Fail\n");
            }
            _rSwdmxFlashInfo.rVar.u4LastVideoWptr = 0;
        }
    }
}

VOID _SWDMX_FlashFlushAFifo(VOID)
{
    BOOL fgRet;
    if(_u1PidxAudio != 0xFF)
    {
        if(_rSwdmxFlashInfo.rVar.u4LastAudioWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(_u1PidxAudio, _rSwdmxFlashInfo.rVar.u4LastAudioWptr, _rSwdmxFlashInfo.rVar.u4LastAudioWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_FlashFlushAFifo DMX_UpdateReadPointer Fail\n");
            }
            _rSwdmxFlashInfo.rVar.u4LastAudioWptr = 0;
        }
    }
}

BOOL _SWDMX_FlashWatchDuration(UINT32 u4CurentTime)
{
    UINT32 u4NewTotalTime;
    UINT64 u8NewTotalTime;
    UINT64 u8Remainder;

    if((u4CurentTime > (_rSwdmxFlashInfo.rVar.u4TotalTime + _rSwdmxFlashInfo.rVar.u4TotalTimeOffset)) &&
        (_u8SendedSize > 0))
    {
        // u4NewTotalTime = (u4CurentTime / _u8SendedSize) * _u8FileSize;
        u8NewTotalTime = (((UINT64)u4CurentTime)+1);
        u8NewTotalTime *= _u8FileSize;
        u8NewTotalTime *= 90;
        u8NewTotalTime = u8Div6432(u8NewTotalTime, (UINT64)100, &u8Remainder);
        u8NewTotalTime = u8Div6432(u8NewTotalTime, _u8SendedSize, &u8Remainder);
        u4NewTotalTime = (UINT32)u8NewTotalTime;
        _rSwdmxFlashInfo.rVar.u4TotalTimeOffset = (u4NewTotalTime - _rSwdmxFlashInfo.rVar.u4TotalTime);
    }

    return TRUE;
}


VOID _SWDMX_FlashWatchLastVPTS(const DMX_PES_MSG_T* prPes)
{
    UINT32 i;
    UINT32 u4Temp = 0;
    UINT32 u4Big = 0;
    UINT32 u4Small = 0;
    UINT64 u8Temp = 0;
    UINT64 u8Remainder;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashWatchLastVPTS prPes NULL\n");
        return;
    }
    
    if(_rSwdmxFlashInfo.rVar.fgNotToDecoder) // pre-parsing case
    {
        // Handle some unexpect PTS
        u8Temp = 0;
        u4Temp = 0;
        u4Big = 0;
        u4Small = 0xFFFFFFFF;
        for(i=0;i<FLASH_PTS_CHECK_NUM;i++)
        {
            if(_rSwdmxFlashInfo.rVar.au4LastPTS[i] != 0)
            {
                if(u4Big < _rSwdmxFlashInfo.rVar.au4LastPTS[i])
                {
                    u4Big = _rSwdmxFlashInfo.rVar.au4LastPTS[i];
                }
                if(u4Small > _rSwdmxFlashInfo.rVar.au4LastPTS[i])
                {
                    u4Small = _rSwdmxFlashInfo.rVar.au4LastPTS[i];
                }
            }
        }
        for(i=0;i<(FLASH_PTS_CHECK_NUM-1);i++)
        {
            _rSwdmxFlashInfo.rVar.au4LastPTS[i] =
                _rSwdmxFlashInfo.rVar.au4LastPTS[i+1];
            if(u4Big == _rSwdmxFlashInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(u4Small == _rSwdmxFlashInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(_rSwdmxFlashInfo.rVar.au4LastPTS[i] != 0)
            {
                u8Temp += _rSwdmxFlashInfo.rVar.au4LastPTS[i];
                u4Temp++;
            }
        }
        _rSwdmxFlashInfo.rVar.au4LastPTS[FLASH_PTS_CHECK_NUM-1] = prPes->u4Pts;
        _rSwdmxFlashInfo.rVar.u4LastPTSCnt++;
        if(u4Temp > 0)
        {
            u8Temp = u8Div6432(u8Temp, (UINT64)u4Temp, &u8Remainder);
            u4Temp = (UINT32)u8Temp;
            if(prPes->u4Pts > u4Temp)
            {
                if((prPes->u4Pts - u4Temp) > (5*90000*FLASH_PTS_CHECK_NUM))
                {
                    _rSwdmxFlashInfo.rVar.u4LastPTS = u4Temp;
                    _rSwdmxFlashInfo.rVar.au4LastPTS[i] = u4Temp;
                }
            }
            else
            {
                if((u4Temp - prPes->u4Pts) > (5*90000*FLASH_PTS_CHECK_NUM))
                {
                    _rSwdmxFlashInfo.rVar.u4LastPTS = u4Temp;
                    _rSwdmxFlashInfo.rVar.au4LastPTS[i] = u4Temp;
                }
            }
        }
    }
}

VOID _SWDMX_FlashWatchLastAPTS(const DMX_AUDIO_PES_T* prPes)
{
    DMX_PES_MSG_T rPes;
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashWatchLastAPTS prPes NULL\n");
        return;
    }
    x_memset(&rPes, 0, sizeof(rPes));
    rPes.u4Pts = prPes->u4Pts;
    _SWDMX_FlashWatchLastVPTS(&rPes);
}

BOOL _SWDMX_FlashWatchVideoPTS(const DMX_PES_MSG_T* prPes)
{
    UINT32 u4Temp = 0;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashWatchVideoPTS prPes NULL\n");
        return FALSE;
    }
    
    if((prPes->fgPtsDts) && (prPes->u4Pts > 0))
    {
        if(_rSwdmxFlashInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxFlashInfo.rVar.u4FirstPTS = prPes->u4Pts;
            LOG(5, "_SWDMX_FlashWatchVideoPTS u4FirstPTS 0x%x\n", _rSwdmxFlashInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((prPes->u4Pts < _rSwdmxFlashInfo.rVar.u4FirstPTS) &&
                ((_rSwdmxFlashInfo.rVar.u4FirstPTS - prPes->u4Pts) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                _rSwdmxFlashInfo.rVar.u4FirstPTS = prPes->u4Pts;
            } 
        }
        
        if(!_rSwdmxFlashInfo.rVar.fgSetStc)
        {
            _rSwdmxFlashInfo.rVar.fgSetStc = TRUE;
            LOG(5, "_SWDMX_FlashWatchVideoPTS fgSetStc\n");
            
            if(!_rSwdmxFlashInfo.rVar.fgNotToDecoder)
            {
	            LOG(3, "_SWDMX_FlashWatchVideoPTS STC_SetStcValue 0x%x\n", 
	                (prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS /*- 45000*/));
                
                STC_StopStc();
                u4Temp = STC_GetStcValue();
                STC_SetStcValue((UINT32)((prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000));
                u4Temp = STC_GetStcValue();

                if (u4Temp != (UINT32)((prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000))
                {
                    LOG(3, "_SWDMX_FlashWatchVideoPTS STC_SetStcValue Fail\n");
                    //return FALSE;
                }
                
                // do on vdec_flw.c
                //STC_StartStc();
            }
        }
        _rSwdmxFlashInfo.rVar.u4LastPTS = prPes->u4Pts;
        _SWDMX_FlashWatchLastVPTS(prPes);        
        
        UNUSED(_SWDMX_FlashWatchDuration(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));
        LOG(6, "_SWDMX_FlashWatchVideoPTS u4Pts 0x%x\n", (prPes->u4Pts-_rSwdmxFlashInfo.rVar.u4FirstPTS));
    }

    _rSwdmxFlashInfo.rVar.u4LastVideoWptr = prPes->u4FrameAddr;
    
    _rSwdmxFlashInfo.rVar.u4PicCnt++;
    #if 0
    if(_rSwdmxFlashInfo.rVar.u4PicCnt%150)
    {
        LOG(6, "_SWDMX_FlashWatchVideoPTS u4PicCnt 150\n");
    }
    #endif
    
    return TRUE;
}

BOOL _SWDMX_FlashWatchAudioPTS(const DMX_AUDIO_PES_T* prPes)
{
    UINT32 u4Temp;
    UINT32 u4DmxAvailSize;
    
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashWatchAudioPTS prPes NULL\n");
        return FALSE;
    }
    
    if(prPes->u4Pts > 0)
    {
        if(_rSwdmxFlashInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxFlashInfo.rVar.u4FirstPTS = prPes->u4Pts;
            LOG(5, "_SWDMX_FlashWatchAudioPTS u4FirstPTS %d\n", _rSwdmxFlashInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((prPes->u4Pts < _rSwdmxFlashInfo.rVar.u4FirstPTS) &&
                ((_rSwdmxFlashInfo.rVar.u4FirstPTS - prPes->u4Pts) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                _rSwdmxFlashInfo.rVar.u4FirstPTS = prPes->u4Pts;
            } 
        }
        
        if(!_rSwdmxFlashInfo.rVar.fgSetStc)
        {
            _rSwdmxFlashInfo.rVar.fgSetStc = TRUE;
            LOG(5, "_SWDMX_FlashWatchAudioPTS fgSetStc\n");
            
            if(!_rSwdmxFlashInfo.rVar.fgNotToDecoder)
            {
	            LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue 0x%x\n", 
	                ((prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000));
                
                STC_StopStc();
                u4Temp = STC_GetStcValue();
                STC_SetStcValue((UINT32)(prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000);
                u4Temp = STC_GetStcValue();

                if (u4Temp != ((UINT32)(prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000))
                {
                    LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue Fail\n");
                    //return FALSE;
                }

                // do on vdec_flw.c
                //STC_StartStc();
            }
        }        
        
        _rSwdmxFlashInfo.rVar.u4LastPTS = prPes->u4Pts;
        _SWDMX_FlashWatchLastAPTS(prPes);
        
        if(_rSwdmxFlashInfo.rVar.fgGotVideo && (!_rSwdmxFlashInfo.rVar.fgGotAudio))
        {
            if((_u8SendedSize >= _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X) &&
                (prPes->u4Pts >= _rSwdmxFlashInfo.rVar.u4FirstVideoPTS))
            {
                VDP_TriggerAudReceive(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS);
                UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));

                LOG(3, "_SWDMX_FlashWatchAudioPTS AUD_SetStartPts 1 pts 0x%x pos %d\n",
                    (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS), (UINT32)_u8SendedSize);
                _rSwdmxFlashInfo.rVar.fgGotAudio = TRUE;
            }
        }
        else if(!_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            if(!_rSwdmxFlashInfo.rVar.fgGotAudio)
            {
                UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));
                //AUD_MMAoutEnable(0, TRUE);
	            LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue 0x%x\n", 
	                ((prPes->u4Pts - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000));

                //STC_StartStc();
                LOG(3, "_SWDMX_FlashWatchAudioPTS AUD_SetStartPts 2 pts 0x%x pos %d\n",
                    (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS), (UINT32)_u8SendedSize);
                _rSwdmxFlashInfo.rVar.fgGotAudio = TRUE;
            }
        }
        
        
        UNUSED(_SWDMX_FlashWatchDuration(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));        
        #if 1
        {
            static UINT32 u4Cnt = 0;
            if(((u4Cnt++)%15) == 0)
            {
                LOG(6, "_SWDMX_FlashWatchAudioPTS u4Pts 0x%x\n", (prPes->u4Pts-_rSwdmxFlashInfo.rVar.u4FirstPTS));
            }
        }
        #else        
        LOG(6, "_SWDMX_FlashWatchAudioPTS u4Pts 0x%x\n", (prPes->u4Pts-_rSwdmxFlashInfo.rVar.u4FirstPTS));
        #endif
    }
    
    if((_rSwdmxFlashInfo.rVar.fgGotVideo) && (!_rSwdmxFlashInfo.rVar.fgGotAudio))
    {
        // could be video fifo size
        
        if(!_fgGetInfo)
        {
            u4DmxAvailSize = (1024*1024);
            // playbacking, check AV size
            if(_rSwdmxFlashInfo.rFmt.fgVideo)
            {
                u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
            }
            if(_rSwdmxFlashInfo.rFmt.fgAudio)
            {
                if(u4DmxAvailSize > DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO))
                {
                    u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                }
            }
            if(u4DmxAvailSize < _u4OneTimeSize)
            {
                // if there is no audio PTS in till now,
                // we use First video PTS as audio's PTS, but not the last one.
                //AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS);
                UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4FirstPTS));
                
                if(!_rSwdmxFlashInfo.rVar.fgSetStc)
                {
                    _rSwdmxFlashInfo.rVar.fgSetStc = TRUE;
                    STC_StopStc();
                    u4Temp = STC_GetStcValue();
                    STC_SetStcValue((UINT32)(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000);
                    u4Temp = STC_GetStcValue();

    	            LOG(3, "_SWDMX_FlashWatchAudioPTS u4DmxAvailSize STC_SetStcValue 0x%x\n", 
    	                (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));
                    if (u4Temp != ((UINT32)(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000))
                    {
                        LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue Fail\n");
                    }
                }
                if(!_rSwdmxFlashInfo.rFmt.fgVideo)
                {
                    //Check in HandleFifoFull
                    //AUD_MMAoutEnable(0, TRUE);
                    //STC_StartStc();
                }
                else
                {
                    // if there is no audio PTS in till now,
                    // we use First video PTS as audio's PTS, but not the last one.
                    //VDP_TriggerAudReceive(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS);
                    VDP_TriggerAudReceive(_rSwdmxFlashInfo.rVar.u4FirstPTS);
                    // do on vdec_flw.c
                    //STC_StartStc();
                }
                
                _rSwdmxFlashInfo.rVar.fgGotAudio = TRUE;
                _rSwdmxFlashInfo.rVar.fgGotAudioReady = TRUE;
                //ASSERT(0);
    	        LOG(0, "_SWDMX_FlashWatchAudioPTS VDP_TriggerAudReceive due to fifo full");
            }
        }
    }
    else if(!_rSwdmxFlashInfo.rFmt.fgVideo)
    {
        if(!_rSwdmxFlashInfo.rVar.fgGotAudio)
        {
            // if there is no audio PTS in till now,
            // we use First video PTS as audio's PTS, but not the last one.
            //AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS);
            UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4FirstPTS));
            //AUD_MMAoutEnable(0, TRUE);
            LOG(3, "_SWDMX_FlashWatchAudioPTS no V AUD_SetStartPts pts 0x%x pos %d\n",
                (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS), (UINT32)_u8SendedSize);
            _rSwdmxFlashInfo.rVar.fgGotAudio = TRUE;
            
            if(!_rSwdmxFlashInfo.rVar.fgSetStc)
            {
                _rSwdmxFlashInfo.rVar.fgSetStc = TRUE;
                LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue 0x%x\n", 
                    ((_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000));
                STC_StopStc();
                u4Temp = STC_GetStcValue();
                STC_SetStcValue((UINT32)(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000);
                u4Temp = STC_GetStcValue();

                if (u4Temp != ((UINT32)(_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000))
                {
                    LOG(3, "_SWDMX_FlashWatchAudioPTS STC_SetStcValue Fail\n");
                    //return FALSE;
                }
            }
            
            //STC_StartStc();
        }
    }
                    
    _rSwdmxFlashInfo.rVar.u4LastAudioWptr = prPes->u4Wp;

    #if 0
    {
        static UINT32 u4Cnt = 0;
        if(((u4Cnt++)%60) == 0)
        {
            LOG(6, "_SWDMX_FlashWatchAudioPTS u4Cnt 60\n");
        }
    }
    #endif
    
    return TRUE;
}


BOOL _SWDMX_FlashAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if(eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE)
    {
        LOG(3, "audio is ready===========================\n");
        _rSwdmxFlashInfo.rVar.fgGotAudioReady = TRUE;
    }
    else
    {
        //impossible
        ASSERT(0);
    }
    return TRUE;
}


BOOL _SWDMX_FlashAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    DMX_AUDIO_PES_T rPes;
    
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashAudioCallback prPes NULL\n");
        return FALSE;
    }
#ifndef CC_FLASH_TEST    
    if((_rSwdmxFlashInfo.rFmt.fgVideo) &&
        (_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2))
    {
        if((!_fgSendSeqHdr) || (_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_FlashAudioCallbackParse(prPes);
        }
    }
    else if((_rSwdmxFlashInfo.rFmt.fgVideo) &&
        (_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264))
    {
        if((!_fgSendSeqHdr) || (_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_FlashAudioCallbackParse(prPes);
        }
    }
#endif    
    x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));

    if(_rSwdmxFlashInfo.rVar.u4LastAudioWptr)
    {
        // for the first time, _rSwdmxFlashInfo.rVar.u4LastAudioWptr = 0,
        // we use original write pointer.
        rPes.u4Wp = _rSwdmxFlashInfo.rVar.u4LastAudioWptr;
    }
    
    UNUSED(_SWDMX_FlashWatchAudioPTS(prPes));
            
    if(_rSwdmxFlashInfo.rVar.fgNotToADec)
    {
        //_SWDMX_FlashFlushAFifo();
        _rSwdmxFlashInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }
    
    if(_rSwdmxFlashInfo.rVar.fgNotToDecoder)
    {
        //_SWDMX_FlashFlushAFifo();
        _rSwdmxFlashInfo.rVar.fgCleanAudio = TRUE;
        _SWDMX_FlashFlushVFifo();
        return TRUE;
    }

    if((!_rSwdmxFlashInfo.rVar.fgGotVideo) && (_rSwdmxFlashInfo.rFmt.fgVideo))
    {
        /*_SWDMX_FlashFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        }*/
        _rSwdmxFlashInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    if(!_rSwdmxFlashInfo.rVar.fgGotAudio)
    {
        /*_SWDMX_FlashFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        }*/
        _rSwdmxFlashInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }
    
    // On cleaning
    if(_rSwdmxFlashInfo.rVar.fgCleanAudio)
    {
        return TRUE;
    }
        
    if(!_rSwdmxFlashInfo.rFmt.fgAudio)
    {
        return TRUE;
    }

    if(_rSwdmxFlashInfo.rVar.fgSendAudioEos)
    {
        return TRUE;
    }
    
    if(rPes.u4Dts > 0)
    {
        rPes.u4Dts -= _rSwdmxFlashInfo.rVar.u4FirstPTS;
    }
    if(rPes.u4Pts > 0)
    {
        rPes.u4Pts -= _rSwdmxFlashInfo.rVar.u4FirstPTS;
    }    
    if(prPes->fgEOS)
    {
        // for the first time, _rSwdmxFlashInfo.rVar.u4LastAudioWptr = xx,
        // but we use original write pointer.
        LOG(3, "_SWDMX_FlashAudioCallback send EOS\n");
        rPes.u4Wp = prPes->u4Wp;
        _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE;
    }
    fgRet = AUD_SendAudioPes(&rPes);
    if(prPes->fgEOS)
    {
        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
    }
    return fgRet;
}

BOOL _SWDMX_FlashVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashVideoCallback prPes NULL\n");
        return FALSE;
    }
#if 0
    if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_FlashVideoCallbackFlashParse(prPes);
        }
    }
    else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_FlashVideoCallbackH264Parse(prPes);
        }
    }   
#endif    
    UNUSED(_SWDMX_FlashWatchVideoPTS(prPes));
        
    if(_rSwdmxFlashInfo.rVar.fgNotToADec)
    {
        _SWDMX_FlashFlushAFifo();
    }
    
    if(_rSwdmxFlashInfo.rVar.fgNotToDecoder)
    {
        _SWDMX_FlashFlushAFifo();
        _SWDMX_FlashFlushVFifo();
        return TRUE;
    }

    if(!_rSwdmxFlashInfo.rFmt.fgVideo)
    {
        return TRUE;
    }

    if(_rSwdmxFlashInfo.rVar.fgSendVideoEos)
    {
        return TRUE;
    }

    // ilii, markout
    /*
    if(_rSwdmxFlashInfo.rVar.fgVdecPause && (_rSwdmxFlashInfo.rVar.fgGotAudio || (!_rSwdmxFlashInfo.rFmt.fgAudio))
        && _rSwdmxFlashInfo.rVar.fgGotAudioReady)
    {
        LOG(5, "_SWDMX_FlashVideoCallback fgVdecPause VDEC_Play\n");
        if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            VDEC_Play(ES0, VDEC_FMT_MPV);
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            VDEC_Play(ES0, VDEC_FMT_H264);
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            VDEC_Play(ES0, VDEC_FMT_WMV);
        }
        else
        {
            ASSERT(0);
        }
        _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
    }
    */

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    /*
    if((_prSwdmxInfo->i4PlaySpeed > 0) 
        && (_prSwdmxInfo->i4PlaySpeed < 4000))
    {
        if(!_rSwdmxFlashInfo.rVar.fgGotVideo)
        {
            if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(prPes->u4FrameType != PIC_TYPE_I)
                {
                    _SWDMX_FlashFlushVFifo();
                    return TRUE;
                }
            }
        }
    }
                       
    if((_prSwdmxInfo->i4PlaySpeed < 1000) 
        || (_prSwdmxInfo->i4PlaySpeed >= 4000))
    {
        if(prPes->u4FrameType == PIC_TYPE_B)
        {
            return TRUE;
        }
    }   */ 
    
    rPesInfo.u4PTS = prPes->u4Pts;
    rPesInfo.u4DTS = prPes->u4Dts;  
    if(rPesInfo.u4PTS > 0)
    {
        _rSwdmxFlashInfo.rVar.fgVideoPTSFound = TRUE;
        //LOG(5, "_SWDMX_FlashVideoCallback Org PTS %d\n", rPesInfo.u4PTS);                    
        rPesInfo.u4PTS -= _rSwdmxFlashInfo.rVar.u4FirstPTS;
        
        if(_rSwdmxFlashInfo.rVar.u4FirstVideoPTS != 0xFFFFFFFF)
        {
            if((prPes->u4Pts < _rSwdmxFlashInfo.rVar.u4FirstVideoPTS) &&
                ((_rSwdmxFlashInfo.rVar.u4FirstVideoPTS - prPes->u4Pts) > (10000))) // 1 sec
            {
                // DTV00209729, first video pts is a fake one.
                // we would not send audio data with PTS small than this first fake video pts.
                
                // u4FirstVideoPTS is used to output audio after this video pts
                // even we assign a smaller PTS that actually is a fake one,
                // we would also not affect the time after we start to output audio
                _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = prPes->u4Pts;
            }
        }
    }
    if(rPesInfo.u4DTS > 0)
    {
        rPesInfo.u4DTS -= _rSwdmxFlashInfo.rVar.u4FirstPTS;
    }
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    //rPesInfo.fgNotDisplay = u4Pts < u4SeekTime;
    rPesInfo.u4TotalTimeOffset = _rSwdmxFlashInfo.rVar.u4TotalTimeOffset;
    rPesInfo.u8Offset = _rSwdmxFlashInfo.rVar.u8PreviousSendedSize;
    rPesInfo.u8OffsetI = _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound;
    rPesInfo.u2DecodingOrder = _rSwdmxFlashInfo.rVar.u2OrderFromIPos;
    _rSwdmxFlashInfo.rVar.u2OrderFromIPos++;
    
    // in es file, we never get pts.
    // so assume we already found pts.
    if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
    {
        _rSwdmxFlashInfo.rVar.fgVideoPTSFound = TRUE; 
    }
    
    if (_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            rPesInfo.ucPicType = MPV_I_TYPE;
            _rSwdmxFlashInfo.rVar.u8PreviousIPos = _rSwdmxFlashInfo.rVar.u8PreviousSendedSize;
            _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound =
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound;
            if(_rSwdmxFlashInfo.rVar.fgVideoPTSFound && !_rSwdmxFlashInfo.rVar.fgSendVideoEos)
            {
                //if((!_rSwdmxFlashInfo.rVar.fgGotVideo) && (_rSwdmxFlashInfo.rFmt.fgAudio))
                if((!_rSwdmxFlashInfo.rVar.fgGotVideo) &&
                    ((_prSwdmxInfo->i4PlaySpeed > 0) 
                       && (_prSwdmxInfo->i4PlaySpeed < 4000)))
                {
                    UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
                    VDEC_Pause(ES0);
                    if(_rSwdmxFlashInfo.rVar.u4FirstVideoPTS == 0xFFFFFFFF)
                    {
                        _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = (rPesInfo.u4PTS + _rSwdmxFlashInfo.rVar.u4FirstPTS);
                    }
                    LOG(5, "_SWDMX_FlashVideoCallback fgGotVideo VDEC_Pause\n");
                    _rSwdmxFlashInfo.rVar.fgGotVideo = TRUE;
                    _rSwdmxFlashInfo.rVar.fgVdecPause = TRUE;
                    UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxFlashInfo.rVar.u4TotalTime));
                }
                else if(!_rSwdmxFlashInfo.rVar.fgGotVideo)
                {
                    if(_rSwdmxFlashInfo.rVar.u4FirstVideoPTS == 0xFFFFFFFF)
                    {
                        _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = (rPesInfo.u4PTS + _rSwdmxFlashInfo.rVar.u4FirstPTS);
                    }
                    _rSwdmxFlashInfo.rVar.fgGotVideo = TRUE;
                    UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxFlashInfo.rVar.u4TotalTime));
                    if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)
                    {
                        UNUSED(VDEC_SetLastTime(ES0, 3000));
                    }
                }
                _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxFlashInfo.rVar.u8PreviousIPos;
                _rSwdmxFlashInfo.rVar.u2OrderFromIPos = 0;
                LOG(6, "_SWDMX_FlashVideoCallback u8PreviousIPosWithPTSFound %d\n",
                    (UINT32)_rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound);
            }
            LOG(6, "_SWDMX_FlashVideoCallback u8PreviousIPos %d\n",
                (UINT32)_rSwdmxFlashInfo.rVar.u8PreviousIPos);
            break;

        case PIC_TYPE_P:
            _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound =
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound;
            rPesInfo.ucPicType = MPV_P_TYPE;
            break;

        case PIC_TYPE_B:
            rPesInfo.ucPicType = MPV_B_TYPE;
            break;

        default:
            rPesInfo.ucPicType = 0;
            ASSERT(0);
            break;
        }
    }
    else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
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
            _rSwdmxFlashInfo.rVar.u8PreviousIPos = _rSwdmxFlashInfo.rVar.u8PreviousSendedSize;
            if(_rSwdmxFlashInfo.rVar.fgVideoPTSFound && !_rSwdmxFlashInfo.rVar.fgSendVideoEos)
            {
                _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound =
                    _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound;
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxFlashInfo.rVar.u8PreviousIPos;
                _rSwdmxFlashInfo.rVar.u2OrderFromIPos = 0;
                LOG(6, "_SWDMX_FlashVideoCallback u8PreviousIPosWithPTSFound %d\n",
                    (UINT32)_rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound);
            }
        }
        else
        {
            // Sample a key frame is there is no IDR
            // Currently, key is 3 second
            UINT32 u4Tmp;
            UINT64 u8Size;
            UINT64 u8Remainder;
            if(_rSwdmxFlashInfo.rVar.u4TotalTime > 0)
            {
                u4Tmp = (_rSwdmxFlashInfo.rVar.u4TotalTime / 90000); // total frames
                u4Tmp *= 3;
                u8Size = u8Div6432(_u8FileSize, (UINT64)u4Tmp, &u8Remainder);
            }
            else
            {
                u8Size = 0;
            }
            if((_rSwdmxFlashInfo.rVar.u8PreviousSendedSize
                - _rSwdmxFlashInfo.rVar.u8PreviousIPos) > u8Size)
            {
                if(_rSwdmxFlashInfo.rVar.fgVideoPTSFound && !_rSwdmxFlashInfo.rVar.fgSendVideoEos)
                {
                    _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                    _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound =
                        _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound;
                    _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxFlashInfo.rVar.u8PreviousIPos;
                    _rSwdmxFlashInfo.rVar.u2OrderFromIPos = 0;
                    LOG(6, "_SWDMX_FlashVideoCallback u8PreviousIPosWithPTSFound %d\n",
                        (UINT32)_rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound);
                }
            }
        }
        
        if(_rSwdmxFlashInfo.rVar.fgVideoPTSFound && !_rSwdmxFlashInfo.rVar.fgSendVideoEos)
        {
            //When change speed, we might in vdec_pause stage.
            //So also do wait fifo ready on 2x speed.
            if((!_rSwdmxFlashInfo.rVar.fgGotVideo) &&
                ((_prSwdmxInfo->i4PlaySpeed > 0) 
                   && (_prSwdmxInfo->i4PlaySpeed < 4000)))
            {
                UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
                VDEC_Pause(ES0);
                if(_rSwdmxFlashInfo.rVar.u4FirstVideoPTS == 0xFFFFFFFF)
                {
                    _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = (rPesInfo.u4PTS + _rSwdmxFlashInfo.rVar.u4FirstPTS);
                }
                LOG(5, "_SWDMX_FlashVideoCallback fgGotVideo VDEC_Pause\n");
                _rSwdmxFlashInfo.rVar.fgGotVideo = TRUE;
                _rSwdmxFlashInfo.rVar.fgVdecPause = TRUE;
                UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxFlashInfo.rVar.u4TotalTime));
            }
            else if(!_rSwdmxFlashInfo.rVar.fgGotVideo)
            {
                if(_rSwdmxFlashInfo.rVar.u4FirstVideoPTS == 0xFFFFFFFF)
                {
                    _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = (rPesInfo.u4PTS + _rSwdmxFlashInfo.rVar.u4FirstPTS);
                }
                _rSwdmxFlashInfo.rVar.fgGotVideo = TRUE;
                UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxFlashInfo.rVar.u4TotalTime));
                if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
                {
                    UNUSED(VDEC_SetLastTime(ES0, 3000));
                }
            }
        }
        LOG(6, "_SWDMX_FlashVideoCallback u8PreviousIPos %d\n",
            (UINT32)_rSwdmxFlashInfo.rVar.u8PreviousIPos);        
    }
    else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
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
    }
    else
    {
        LOG(1, "_SWDMX_FlashVideoCallback unknown video\n");
    }
    
    // in es file, we never get pts.
    // so assume we already found pts.
    if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
    {
        if(_rSwdmxFlashInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxFlashInfo.rVar.u4FirstPTS = 1;
            _rSwdmxFlashInfo.rVar.u4LastPTS = 1;
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4DTS = 1;
            rPesInfo.u4PTS = 1;
            LOG(5, "_SWDMX_FlashVideoCallback u4FirstPTS 0x%x\n", _rSwdmxFlashInfo.rVar.u4FirstPTS);
        }
    }
    else
    {
        if(_rSwdmxFlashInfo.rVar.fgJumpping)
        {
            if(!rPesInfo.fgDtsValid)
            {
                rPesInfo.fgDtsValid = TRUE;
                rPesInfo.u4DTS = (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS);
                rPesInfo.u4PTS = rPesInfo.u4DTS;
            }
        }
    }
    
    if(prPes->fgEOS)
    {
        LOG(3, "_SWDMX_FlashVideoCallback send EOS\n");
        _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE;
    }
#ifdef CC_FLASH_TEST
//    rPesInfo.fgDtsValid = TRUE;
    //LOG(0, "video PTS %d\n", rPesInfo.u4PTS);
#endif
    VDEC_SendEs((void*)&rPesInfo);
    
    return TRUE;
}


BOOL _SWDMX_FlashAudioCallbackParse(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashAudioCallbackParse prPes NULL\n");
        return FALSE;
    }
    if(_u1PidxAudio != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxAudio, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_FlashAudioCallbackParse DMX_UpdateReadPointer Fail\n");
        }
    }
    
    return TRUE;
}

#if 0
BOOL _SWDMX_FlashVideoCallbackFlashParse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    //UCHAR ucByte0, ucByte1, ucByte2, ucByte3;
    UINT16 u2QueueSize;
    UINT16 u2MaxQueueSize;
    UINT32 u4Tmp;
    VDEC_PES_INFO_T rPesInfo;
    
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashVideoCallbackFlashParse prPes NULL\n");
        return FALSE;
    }
    
    if(prPes->fgSeqHeader)
    {
        UNUSED(VDEC_SetGetSeqHdrOnly(ES0, TRUE));

        x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));        
        rPesInfo.u4PTS = prPes->u4Pts;
        rPesInfo.u4DTS = prPes->u4Dts;        
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        rPesInfo.fgEos = prPes->fgEOS;
        //rPesInfo.fgNotDisplay = u4Pts < u4SeekTime;
        rPesInfo.u4TotalTimeOffset = 0;
        rPesInfo.u8Offset = 0;
        rPesInfo.u8OffsetI = 0;//_rSwdmxFlashInfo.rVar.u8PreviousIPos;    
        UNUSED(VDEC_SendEs((void*)&rPesInfo));
        UNUSED(VDEC_SendEs((void*)&rPesInfo));

        if((prPes->u4BufEnd - prPes->u4FrameAddr) > FLASH_MPEG2_SEQ_HDR_SIZE)
        {
            x_memcpy(_ucFlashSeqHdr, (VOID*)VIRTUAL(prPes->u4FrameAddr), FLASH_MPEG2_SEQ_HDR_SIZE);
        }
        else
        {
            x_memcpy(_ucFlashSeqHdr,
                (VOID*)VIRTUAL(prPes->u4FrameAddr),
                (prPes->u4BufEnd - prPes->u4FrameAddr));                
            x_memcpy(_ucFlashSeqHdr+(prPes->u4BufEnd - prPes->u4FrameAddr),
                (VOID*)VIRTUAL(prPes->u4BufStart),
                FLASH_MPEG2_SEQ_HDR_SIZE-(prPes->u4BufEnd - prPes->u4FrameAddr));
        }
        
        u4Tmp = 30;
        LOG(5, "_SWDMX_FlashVideoCallbackFlashParse send SEQ ");
        while(u4Tmp--)
        {
            VDEC_GetQueueInfo(ES0, &u2QueueSize, &u2MaxQueueSize);
            if(u2QueueSize == 0)
            {
                MPV_FlushEsmQ(ES0, 0);
                LOG(5, "Finish\n");
                break;
            }
            x_thread_delay(1);
        }
        if(u4Tmp == 0)
        {
            LOG(5, "Not Finish\n");
            MPV_FlushEsmQ(ES0, 0);
        }
        UNUSED(VDEC_SetGetSeqHdrOnly(ES0, FALSE));

        if(_eVideo == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if((_ucFlashSeqHdr[0] == 0x0) && (_ucFlashSeqHdr[1] == 0x0) && 
                (_ucFlashSeqHdr[2] == 0x1) && (_ucFlashSeqHdr[3] == 0xB3))
            {
                // got it
            }
            else
            {
                // find another one
                _SWDMX_FlashFlushVFifo();
                return TRUE;
            }
        }
        
        LOG(5, "_SWDMX_FlashVideoCallbackFlashParse send sequence\n");
        _fgSendSeqHdr = TRUE;
        return TRUE;
    }
    
    if(_u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_FlashVideoCallbackFlashParse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}


BOOL _SWDMX_FlashVideoCallbackH264Parse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;
    
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_FlashVideoCallbackH264Parse prPes NULL\n");
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
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
        fgRet = VDEC_PrsSeqHdr(VDEC_FMT_H264, &rPesInfo, &_rSeqInfo);
        if(fgRet)
        {
            //LOG(5, "_SWDMX_FlashVideoCallbackH264Parse rSeqInfo %dX%d\n",
            //    rSeqInfo.u2_width, rSeqInfo.u2_height);
            _fgSendSeqHdr = TRUE;
        }
        return TRUE;
    }
    
    if(_u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_FlashVideoCallbackH264Parse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}
#endif

VOID _SWDMX_FlashSeekFinNotify(UINT32 u4PTS, BOOL fgAgain)
{
    LOG(6, "_SWDMX_FlashSeekFinNotify\n");
    //VERIFY (x_sema_unlock(_hPlayMgrSemaFinSeek) == OSR_OK);
    _rSwdmxFlashInfo.rVar.fgJumpNext = TRUE;
    if(fgAgain)
    {
        _rSwdmxFlashInfo.rVar.u2JumpAgainCount++;
    }
    else
    {
        _rSwdmxFlashInfo.rVar.u2JumpAgainCount = 0;
    }
    UNUSED(u4PTS);
}

#if 0
BOOL _SWDMX_FlashPsInfoNotify(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
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
        fgRet = _SWDMX_FlashSelectPSStream(prInfo);
        // ~parse

        if(fgRet)
        {
            u4DetectFin = DETECT_OK;
        }
        else
        {
            u4DetectFin = DETECT_FAIL;
        }
        
        i4Ret = x_msg_q_send(_hFindMsgQ, (VOID *)&u4DetectFin, sizeof(UINT32), 255);

        if(i4Ret != OSR_OK)
        {
        	return FALSE;
        }
    }
    return TRUE;
}
#endif

BOOL _SWDMX_FlashResetFeeder(VOID)
{
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
    UINT32 u4ReqCnt;
#endif
    
    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }
    
    #ifdef SWDMX_FLASH_DEBUG_BUFFER
    if((_rSwdmxFlashInfo.rBuf.u4BufAvailCnt > 0) || (_rSwdmxFlashInfo.rBuf.u4ReqTimes > 0))
    {
	    LOG(7, "Flash Fd Flh UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d UAdr %d\n",
	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
            _rSwdmxFlashInfo.rBuf.u4UsingStartAddr);
    }
    #endif

    _u4FeederReqID += 50; // always a even value
    _rSwdmxFlashInfo.rBuf.u4BufAvailCnt = 0;
    _rSwdmxFlashInfo.rBuf.u4BufReadIdx = 0;
    _rSwdmxFlashInfo.rBuf.u4BufWriteIdx = 0;
    _rSwdmxFlashInfo.rBuf.u4BufTotalSize = 0;
    _rSwdmxFlashInfo.rBuf.u4ReqTimes = 0;
    _rSwdmxFlashInfo.rBuf.u4ReqReadIdx = 0;
    _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx = 0;
    _rSwdmxFlashInfo.rBuf.u4ReqTotalSize = 0;
    _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
    
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
    for(u4ReqCnt = 0;u4ReqCnt < FLASH_REQ_NUM;u4ReqCnt++)
    {
        // make it invalid, not a even value
        _rSwdmxFlashInfo.rBuf.au4ReqId[u4ReqCnt] = 1;
    }
#endif
    
    // Update read ptr to feeder
    if(_rFeederInfo.u4WriteAddr)
    {
        if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rFeederInfo.u4WriteAddr) != FEEDER_E_OK)
        {
            LOG(1, "_SWDMX_FlashResetFeeder FeederUpdateReadPointer fail\n");
        }
    }
    return TRUE;
}

VOID _SWDMX_FlashResetData()
{
    BOOL fgRet;
    _SWDMX_FlashFlushAFifo();
    _SWDMX_FlashFlushVFifo();
    if(_u1PidxAudio != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
    }
    if(_u1PidxVideo != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(_u1PidxVideo));
    }
    _rSwdmxFlashInfo.rVar.fgFirst = TRUE;
    //if(_prSwdmxInfo->fgEnVideoDmx)
    //{
    //    SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //    SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //}
    //_u4BufStart = 0;
    _SWDMX_FlushReqMsgQ();
    UNUSED(_SWDMX_FlashResetFeeder());
    // This might put before _SWDMX_FlashFlushAFifo, DMX_MM_FlushBuffer
    // because write might meet read pointer, and DDI is in full status,
    // then _SWDMX_DMX_Reset will cause DMX in strange status.
    // so DMX_DDI_MoveData can't return when next call DMX_DDI_MoveData.
    if(_rSwdmxFlashInfo.rFmt.eFmt != SWDMX_FMT_MP3_AUDIO_ES)
    {
        fgRet = _SWDMX_DMX_Reset();//DMX_DDI_RequestReset();
        if(!fgRet)
        {
            LOG(3, "_SWDMX_FlashResetData _SWDMX_DMX_Reset Fail\n");
        }
    }
    //#define DMXCMD_READ321(offset)			IO_READ32((IO_VIRT + 0x17000), ((offset) * 4))
    //LOG(3, "_SWDMX_FlashResetData DMXCMD 0x%x\n", DMXCMD_READ321(26));
    
    // For es file, DMX_DDI_RequestReset will reset the value set
    // by DMX_ES_SetFileLength. So set it again.
    if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
    {
        if(_u1PidxVideo != 0xFF)
        {
#ifndef CC_FLASH_TEST        
            fgRet = DMX_ES_SetFileLength(_u1PidxVideo,(UINT32)0xFFFFFFFF);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_FlashResetData _DMX_ES_SetFileLength Fail\n");
            }
#endif            
        }
    }
        
    _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
    //_u4AvailSize = 0;
}

VOID _SWDMX_FlashResetVariable()
{
    UINT32 i;
    _SWDMX_FlashResetData();
    //_SWDMX_FlushDataMsgQ();
        
    _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
    _rSwdmxFlashInfo.rVar.fgSendAudioEos = FALSE;
    _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
    _rSwdmxFlashInfo.rVar.fgJumpping = FALSE;
    //_rSwdmxFlashInfo.rVar.fgFirst = TRUE;
    _rSwdmxFlashInfo.rVar.u8JumpStartAddr = 0;
    _rSwdmxFlashInfo.rVar.fgNotToDecoder = FALSE;
    _rSwdmxFlashInfo.rVar.fgNotToADec = FALSE;
    
    _rSwdmxFlashInfo.rVar.fgSetStc = FALSE;
    _rSwdmxFlashInfo.rVar.u4FirstPTS = 0;
    _rSwdmxFlashInfo.rVar.u4LastPTS = 0;
    _rSwdmxFlashInfo.rVar.u4LastPTSCnt = 0;
    for(i=0;i<FLASH_PTS_CHECK_NUM;i++)
    {
        _rSwdmxFlashInfo.rVar.au4LastPTS[i] = 0;
    }
    _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
    _rSwdmxFlashInfo.rVar.fgGotVideo = FALSE;
    _rSwdmxFlashInfo.rVar.fgGotAudio = FALSE;
    _rSwdmxFlashInfo.rVar.fgGotAudioReady = FALSE;
    _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
    _rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
    _rSwdmxFlashInfo.rVar.u4PicCnt = 0;
    //_rSwdmxFlashInfo.rVar.u4TotalTime = 0;
    _rSwdmxFlashInfo.rVar.u4TotalTimeOffset = 0;

    _rSwdmxFlashInfo.rVar.u4LastVideoWptr = 0;
    _rSwdmxFlashInfo.rVar.u4LastAudioWptr = 0;

    _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
    _rSwdmxFlashInfo.rVar.u8PreviousIPos = 0;
    _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = 0;
    _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = 0;
    _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = 0;
    _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
}

VOID _SWDMX_FlashJumpToNormal(BOOL fgSeek, BOOL fgJumpping, INT32 i4Speed)
{
//    BOOL fgRet;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    VDP_PIC_INFO_T rPicInfo;
    
    if(fgJumpping || fgSeek)
    {
        _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        
        if(_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            if(!fgSeek)
            {
                u8Offset = 0;
                u8OffsetI = 0;
                u2DecodingOrder = 0;
                i4TempRef = 0;
                // from now, we use file position of last displayed picture.
                UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
                if(u8OffsetI > _u8FileSize)
                {
                    LOG(3, "_SWDMX_FlashJumpToNormal VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
                        (UINT32)u8OffsetI, (UINT32)_u8FileSize);
                    u8Offset = _u8FileSize;
                    u8OffsetI = _u8FileSize;
                }
                _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxFlashInfo.rVar.u8JumpStartAddr = u8OffsetI;
                _u8SendedSize = u8OffsetI;
                _u8ReadedPos = u8OffsetI;
                _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
                _rSwdmxFlashInfo.rVar.u8PreviousIPos = u8OffsetI;
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
                _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = u8OffsetI;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = u8Offset;
                UNUSED(VDEC_SetRenderFromPos(ES0, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                //VDEC_SetRenderFromPos(ES0, u8Offset, 0, 0, FALSE);
                UNUSED(VDP_GetPicInfo(B2R_1, &rPicInfo));
                LOG(5, "_SWDMX_FlashJumpToNormal VDEC_SetRenderFromPos I:%d, %d, PTS:%d\n", (UINT32)u8OffsetI,
                    (UINT32)u8Offset, rPicInfo.u4Pts);
                _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = rPicInfo.u4Pts;
            }
            else
            {
                UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, _u8FileSize));
                LOG(5, "_SWDMX_FlashJumpToNormal fgSeek\n");
            }
            //STC_StopStc();
            //STC_SetStcValue(0);            
            MPV_FlushEsmQ(ES0, FALSE);
            UNUSED(VDEC_ReleaseDispQ(ES0));
            VDP_ResetPts();
            UNUSED(VDEC_RegTrickPtsCb(NULL));
            /*
            if(!fgSeek)
            {
                if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    VDEC_Play(ES0, VDEC_FMT_MPV);
                }
                else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    VDEC_Play(ES0, VDEC_FMT_H264);
                }
                else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    VDEC_Play(ES0, VDEC_FMT_WMV);
                }
                else
                {
                    ASSERT(0);
                }
            }*/
            // audio is stopped already.
            ////AUD_WaitDspFlush(INTERNAL_DAC_ID_MAIN);
        }
          
        _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxFlashInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
        //_rSwdmxFlashInfo.rVar.fgFirst = TRUE;
        
        _SWDMX_FlashResetData();
        
        _rSwdmxFlashInfo.rVar.fgSetStc = FALSE; // PTS reset
        _rSwdmxFlashInfo.rVar.fgGotVideo = FALSE;
        _rSwdmxFlashInfo.rVar.fgGotAudio = FALSE;
        _rSwdmxFlashInfo.rVar.fgGotAudioReady = FALSE;
        _rSwdmxFlashInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
        _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
        _rSwdmxFlashInfo.rVar.u4PicCnt = 0;
        //_fgSetAudioStartTime = FALSE;

        LOG(5, "_SWDMX_FlashJumpToNormal Jump\n");
    }
    else
    {        
        LOG(5, "_SWDMX_FlashJumpToNormal\n");
    }
    UNUSED(i4Speed);
}

// return TRUE if we found sequence
BOOL _SWDMX_FlashGetFirstSeqHdr()
{	// this is first run for one file, we have to get sequence header
	if((_rSwdmxFlashInfo.rFmt.fgVideo) &&
	    ((_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2) ||
	    (_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)))
    {
    	if((!_fgSendSeqHdr) && (!_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq))
        {
            _u8OrgSendedSize = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq = TRUE;
            _u8SendedSize = 0;
            _u8ReadedPos = 0;
            _SWDMX_FlashResetData();
            LOG(5, "_SWDMX_FlashGetFirstSeqHdr to find sequence\n");
        }
    	else if(!_fgSendSeqHdr)
        {
            LOG(6, "_SWDMX_FlashGetFirstSeqHdr to finding sequence\n");
        }
    	else if(_rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq)
        {
            _rSwdmxFlashInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
            _u8SendedSize = _u8OrgSendedSize;
            _u8ReadedPos = _u8OrgSendedSize;        
            _SWDMX_FlashResetData();
            LOG(6, "_SWDMX_FlashGetFirstSeqHdr to found sequence\n");
            return TRUE;
        }
    }
	return FALSE;
}


VOID _SWDMX_FlashHandleSeek(INT32 i4PlaySpeed, BOOL fgSeek)
{
    UINT32 u4Speed;
    UINT32 u4Tmp;
    UINT32 u4SkipTime;
    UINT64 u8Tmp;
    UINT64 u8Size;
    UINT64 u8Remainder;
    HAL_TIME_T rDelta;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    
    MPV_FlushEsmQ(ES0, FALSE);
    _SWDMX_FlashResetData();
    
    HAL_GetTime(&_rJumpEndTime);
    HAL_GetDeltaTime(&rDelta, &_rJumpStartTime, &_rJumpEndTime);
    _rJumpStartTime.u4Micros = _rJumpEndTime.u4Micros;
    _rJumpStartTime.u4Seconds = _rJumpEndTime.u4Seconds;
    if(rDelta.u4Seconds > 3)
    {
        rDelta.u4Seconds = 3;
    }
    u4SkipTime = ((rDelta.u4Seconds*1000000) + rDelta.u4Micros);

    // from now, we use file position of last displayed picture.
    /*
    u8Offset = u8OffsetI = 0;
    u2DecodingOrder = 0;
    i4TempRef = 0;
    VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
    if(u8OffsetI > _u8FileSize)
    {
        LOG(3, "_SWDMX_FlashHandleSeek VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
            (UINT32)u8OffsetI, (UINT32)_u8FileSize);
        u8OffsetI = _u8FileSize;
    }*/
    u8Offset = 0;
    u8OffsetI = 0;
    u2DecodingOrder = 0;
    i4TempRef = 0;
    UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
    if(u8OffsetI > _u8FileSize)
    {
        LOG(3, "_SWDMX_FlashJumpToNormal VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
            (UINT32)u8OffsetI, (UINT32)_u8FileSize);
        u8OffsetI = _u8FileSize;
        u2DecodingOrder = 0;
        i4TempRef = 0;
    }
    LOG(5, "_SWDMX_FlashJumpToNormal VDP_GetFilePosition i4TempRef %d u2DecodingOrder %d u8Offset %d\n",
        i4TempRef, u2DecodingOrder, (UINT32)u8Offset, (UINT32)_u8FileSize);

    if(i4PlaySpeed == -1)
    {
        VDP_PIC_INFO_T rPicInfo;
        
        //u8Offset = u8OffsetI = 0;
        //u2DecodingOrder = 0;
        //i4TempRef = 0;
        // from now, we use file position of last displayed picture.
        /*VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
        if(u8OffsetI > _u8FileSize)
        {
            LOG(3, "_SWDMX_FlashJumpToNormal VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
                (UINT32)u8OffsetI, (UINT32)_u8FileSize);
            u8OffsetI = _u8FileSize;
            u2DecodingOrder = 0;
            i4TempRef = 0;
        }
        LOG(5, "_SWDMX_FlashJumpToNormal VDP_GetFilePosition i4TempRef %d u2DecodingOrder %d u8Offset %d\n",
            i4TempRef, u2DecodingOrder, (UINT32)u8Offset, (UINT32)_u8FileSize);
        */
        if(u8OffsetI == u8Offset)
        {
            if(u8Offset == 0)
            {
                u8OffsetI = _u8FileSize;
                u8Offset = _u8FileSize;
            }
            else
            {
                // GOP boundry is there. Back a GOP.
                // _rSwdmxFlashInfo.rVar.u4TotalTime unit is 90000
                //u4Tmp = (_rSwdmxFlashInfo.rVar.u4TotalTime/90000)*30; // total frames
                //u8Size = (_u8FileSize/u4Tmp); // avg size/frame
                //u8Size *= (u4Speed/1000);
                u4Tmp = (_rSwdmxFlashInfo.rVar.u4TotalTime / 3000); // total frames
                u8Size = u8Div6432(_u8FileSize, (UINT64)u4Tmp, &u8Remainder);
                u8Size *= (30); // 1 sec
                if(_rSwdmxFlashInfo.rVar.u2JumpAgainCount)
                {
                    u8Size *= _rSwdmxFlashInfo.rVar.u2JumpAgainCount;
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
            LOG(5, "_SWDMX_FlashJumpToNormal new u8OffsetI %d\n",
                (UINT32)u8Offset);
        }
        
        _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxFlashInfo.rVar.u8JumpStartAddr = u8OffsetI;
        _u8SendedSize = u8OffsetI;
        _u8ReadedPos = u8OffsetI;
        _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
        _rSwdmxFlashInfo.rVar.u8PreviousIPos = u8OffsetI;
        _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
        _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = u8OffsetI;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = u8Offset;
        UNUSED(VDEC_SetRenderBackStep(ES0, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
        //VDEC_SetRenderFromPos(ES0, u8Offset, i4TempRef-1, u2DecodingOrder-1, TRUE);
        UNUSED(VDP_GetPicInfo(B2R_1, &rPicInfo));
        LOG(5, "_SWDMX_FlashJumpToNormal VDEC_SetRenderBackStep I:%d, %d, PTS:%d\n", (UINT32)u8OffsetI,
            (UINT32)u8Offset, rPicInfo.u4Pts);
    }
    else
    {
        u4Speed = (i4PlaySpeed < 0) ? ((UINT32)-i4PlaySpeed) : ((UINT32)i4PlaySpeed);
        
        if((_rSwdmxFlashInfo.rVar.u4TotalTime > 0) && (_u8FileSize > 0))
        {
            // _rSwdmxFlashInfo.rVar.u4TotalTime unit is 90000
            //u4Tmp = (_rSwdmxFlashInfo.rVar.u4TotalTime/90000)*30; // total frames
            //u8Size = (_u8FileSize/u4Tmp); // avg size/frame
            //u8Size *= (u4Speed/1000);
            u4Tmp = (_rSwdmxFlashInfo.rVar.u4TotalTime / 3000); // total frames
            u8Size = u8Div6432(_u8FileSize, (UINT64)u4Tmp, &u8Remainder);
            u8Tmp = u8Size;
            u8Size *= (u4Speed / 1000);
            if(i4PlaySpeed < 0)
            {
                if(_rSwdmxFlashInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * _rSwdmxFlashInfo.rVar.u2JumpAgainCount);
                }
            }
            
            // This is the time we spent on the sending I frame
            if(u4SkipTime > 3000000) // 3 sec
            {
                LOG(5, "_SWDMX_FlashHandleSeek u4SkipTime %d\n", (UINT32)u4SkipTime);
                u4SkipTime = 3000000;
            }
            if(u4SkipTime > 10000) // 10 ms
            {
                // a = _u8FileSize / u4TotalTime -> 90K base
                // b = (u4SkipTIme / 1000000) * (a/90K)
                // b = (u4SkipTIme / 1000000) * ((_u8FileSize / u4TotalTime)/90000)
                // b = (u4SkipTIme / 100) * ((_u8FileSize / u4TotalTime)/9)
                u8Tmp = (u4SkipTime/100);
                u8Tmp *= 9;
                u8Tmp = (u8Tmp * _u8FileSize); //1000000 / 90000 -> 100/9
                u8Tmp = u8Div6432(u8Tmp, (UINT64)_rSwdmxFlashInfo.rVar.u4TotalTime, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
            }
        }
        else
        {
            u8Size = (UINT64)VDEC_GetAvgPicSize(0);
            u8Tmp = u8Size;
            u8Size *= (UINT64)(u4Speed / 1000);
            if(i4PlaySpeed < 0)
            {
                if(_rSwdmxFlashInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * _rSwdmxFlashInfo.rVar.u2JumpAgainCount);
                }
            }
            // This is the time we spent on the sending I frame
            if(u4SkipTime > 3000000) // 3 sec
            {
                LOG(5, "_SWDMX_FlashHandleSeek u4SkipTime %d\n", (UINT32)u4SkipTime);
                u4SkipTime = 3000000;
            }
            if(u4SkipTime > 10000) // 10 ms
            {
                u8Tmp = (UINT64)VDEC_GetAvgPicSize(0);
                u8Tmp *= 30;
                u8Tmp *= u4SkipTime;
                u8Tmp = u8Div6432(u8Tmp, (UINT64)1000000, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
            }
        }
            
        u4Tmp = (UINT32)u8Size;
        u4Tmp = (u4Tmp%(_u4AlignBase));
        u8Size -= (UINT64)u4Tmp;
        if(i4PlaySpeed < 0)
        {
            if(_rSwdmxFlashInfo.rVar.u8JumpStartAddr > u8Size)
            {
                _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
                _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
                _u8SendedSize = (_rSwdmxFlashInfo.rVar.u8JumpStartAddr - u8Size);
                /*if(u8OffsetI > _rSwdmxFlashInfo.rVar.u8JumpStartAddr)
                {
                    u4Tmp = (UINT32)(u8OffsetI - _rSwdmxFlashInfo.rVar.u8JumpStartAddr);
                    _u8SendedSize -= (u4Tmp/2);
                }*/
                _u8ReadedPos = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousIPos = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
                if(_rSwdmxFlashInfo.rFmt.fgVideo)
                {
                    if(fgSeek)
                    {
                        // alwasy render it.
                        UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                    }
                    else
                    {
                        // only render it before u8Offset.
                        UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, u8Offset));
                    }
                }
            }
            else
            {
                // end seek
                _rSwdmxFlashInfo.rVar.fgNotToADec = TRUE; // FixMe, should be FALSE
                _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
                _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
                _u8SendedSize = _u8FileSize;
                _u8ReadedPos = _u8FileSize;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousIPos = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
                if(_rSwdmxFlashInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, (UINT64)0));
                }
            }
        }
        else // i4PlaySpeed > 0
        {
            _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
            _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
            _u8SendedSize = (_rSwdmxFlashInfo.rVar.u8JumpStartAddr + u8Size);
            /*if(u8OffsetI > _rSwdmxFlashInfo.rVar.u8JumpStartAddr)
            {
                u4Tmp = (UINT32)(u8OffsetI - _rSwdmxFlashInfo.rVar.u8JumpStartAddr);
                _u8SendedSize += (u4Tmp/2);
            }*/
            _u8ReadedPos = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u8PreviousIPos = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
            if(_rSwdmxFlashInfo.rFmt.fgVideo)
            {
                // alwasy render it.
                UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
            }
        }
    }
    
    if(_rSwdmxFlashInfo.rFmt.fgVideo)
    {
        UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
        if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_MPV))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_PlayOneFrm VDEC_FMT_MPV fail\n");
            }
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_H264))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_PlayOneFrm VDEC_FMT_H264 fail\n");
            }
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_WMV))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_VC1 fail\n");
            }
        }
        else
        {
            ASSERT(0);
        }
    }
    
    LOG(6, "_SWDMX_FlashHandleSeek new addr %d\n", (UINT32)_u8SendedSize);

    // reset jump
    _rSwdmxFlashInfo.rVar.u8JumpStartAddr = _u8SendedSize;
    _rSwdmxFlashInfo.rVar.u4PicCnt = 0;
}


VOID _SWDMX_FlashHandleChange(INT32 i4PlaySpeed, UINT64 u8SeekPos)
{
//    BOOL fgRet;
    BOOL fgSeek;
    UINT32 u4Count;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    
    fgSeek = FALSE;
    if(((UINT32)(u8SeekPos>>32) != 0xFFFFFFFF)
        || ((UINT32)(u8SeekPos) != 0xFFFFFFFF))
    {
        LOG(5, "_SWDMX_FlashHandleChange change pos: %d -> %d ================\n",
            (UINT32)_u8SendedSize, (UINT32)u8SeekPos);
        
        _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        _rSwdmxFlashInfo.rVar.fgSetStc = FALSE; // PTS reset
        _rSwdmxFlashInfo.rVar.fgGotVideo = FALSE;
        _rSwdmxFlashInfo.rVar.fgGotAudio = FALSE;
        _rSwdmxFlashInfo.rVar.fgGotAudioReady = FALSE;
        _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
        //_fgSetAudioStartTime = FALSE;
        //_rSwdmxFlashInfo.rVar.fgFirst = TRUE;
        _rSwdmxFlashInfo.rVar.u4PicCnt = 0;
        u4Count = (UINT32)u8SeekPos;
        if(u8SeekPos < (_u4AlignBase))
        {
            _u8SendedSize = 0;
            _rSwdmxFlashInfo.rVar.u8JumpStartAddr = _u8SendedSize;
            _u8ReadedPos = _u8SendedSize;
        }
        else
        {
            _u8SendedSize = 
                (u8SeekPos - (UINT64)(u4Count % (_u4AlignBase))); // 188 and 16 byte align
            _rSwdmxFlashInfo.rVar.u8JumpStartAddr = _u8SendedSize;
            _u8ReadedPos = _u8SendedSize;
        }
        _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxFlashInfo.rVar.u8PreviousIPos = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
        
        _SWDMX_FlashResetData();
        fgSeek = TRUE;
    }
    
    if(_rSwdmxFlashInfo.rFmt.fgVideo)
    {
        LOG(5, "_SWDMX_FlashHandleChange change speed: %d ====================\n", i4PlaySpeed);
        if(i4PlaySpeed == 1000)
        {
            _SWDMX_FlashJumpToNormal(fgSeek, TRUE,
                i4PlaySpeed);
            
            _rSwdmxFlashInfo.rVar.fgJumpping = FALSE;
            _rSwdmxFlashInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxFlashInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxFlashInfo.rVar.fgNotToADec = FALSE;
            
        }
        else if (((i4PlaySpeed >= 2000) && (i4PlaySpeed < 4000)) ||
        	(i4PlaySpeed == 1))
        {
            _SWDMX_FlashJumpToNormal(fgSeek,
                _rSwdmxFlashInfo.rVar.fgJumpping,
                i4PlaySpeed);
            
            _rSwdmxFlashInfo.rVar.fgJumpping = FALSE;
            _rSwdmxFlashInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxFlashInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxFlashInfo.rVar.fgNotToADec = TRUE;
        }
        else
        {
            MPV_FlushEsmQ(ES0, FALSE);
            UNUSED(VDEC_ReleaseDispQ(ES0));
            VDP_ResetPts();
            
            _SWDMX_FlashResetData();
            _rSwdmxFlashInfo.rVar.fgJumpping = TRUE;
            _rSwdmxFlashInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxFlashInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxFlashInfo.rVar.fgNotToADec = TRUE;
            _rSwdmxFlashInfo.rVar.fgSendEOF = FALSE;
            _rSwdmxFlashInfo.rVar.fgSendAudioEos = FALSE;
            _rSwdmxFlashInfo.rVar.fgSendVideoEos = FALSE;
            //_rSwdmxFlashInfo.rVar.fgFirst = TRUE;
            
            _rSwdmxFlashInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
            _rSwdmxFlashInfo.rVar.fgSetStc = FALSE; // PTS reset
            _rSwdmxFlashInfo.rVar.fgGotVideo = FALSE;
            _rSwdmxFlashInfo.rVar.fgGotAudio = FALSE;
            _rSwdmxFlashInfo.rVar.fgGotAudioReady = FALSE;
            _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
            //_fgSetAudioStartTime = FALSE;
            _rSwdmxFlashInfo.rVar.u8JumpStartAddr = _u8SendedSize;
            _rSwdmxFlashInfo.rVar.u4PicCnt = 0;
            // from now, we use file position of last displayed picture.
            u8OffsetI = 0;
            u8Offset = u8OffsetI;
            u2DecodingOrder = 0;
            i4TempRef = 0;
            if(!fgSeek)
            {
                // if seek and trick, we might get the older position before seek.
                // we have to ignore the older position when seek.
                UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
                if(i4PlaySpeed > 0)
                {
                    _rSwdmxFlashInfo.rVar.u8JumpStartAddr = u8Offset;
                    LOG(5, "_SWDMX_FlashHandleChange Jumpping 1 _u8SendedSize %d u8JumpStartAddr %d\n",
                        (UINT32)_u8SendedSize, (UINT32)_rSwdmxFlashInfo.rVar.u8JumpStartAddr);
                }
                else
                {
                    _rSwdmxFlashInfo.rVar.u8JumpStartAddr = u8OffsetI;
                    LOG(5, "_SWDMX_FlashHandleChange Jumpping 2 _u8SendedSize %d u8JumpStartAddr %d\n",
                        (UINT32)_u8SendedSize, (UINT32)_rSwdmxFlashInfo.rVar.u8JumpStartAddr);
                }
            }
            _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxFlashInfo.rVar.u8PreviousIPos = _rSwdmxFlashInfo.rVar.u8JumpStartAddr;
            _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxFlashInfo.rVar.u8JumpStartAddr;
            _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _rSwdmxFlashInfo.rVar.u8JumpStartAddr;
            _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _rSwdmxFlashInfo.rVar.u8JumpStartAddr;
            _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _rSwdmxFlashInfo.rVar.u8JumpStartAddr;
            
            LOG(5, "_SWDMX_FlashHandleChange VDEC_GetAvgPicSize %d\n",
                VDEC_GetAvgPicSize(0));
            
            HAL_GetTime(&_rJumpStartTime);
            UNUSED(VDEC_RegTrickPtsCb(_SWDMX_FlashSeekFinNotify));
            _SWDMX_FlashHandleSeek(i4PlaySpeed, fgSeek);
        }
    }
}

VOID _SWDMX_FlashHandleEOS()
{
    // if i4PlaySpeed = 1x or 2x, fgVdecPause = TRUE
    // then got EOS and VDec in pause.
    //if(_rSwdmxFlashInfo.rVar.fgSendEOF)
    if((((_rSwdmxFlashInfo.rFmt.fgAudio) && (!_rSwdmxFlashInfo.rVar.fgGotAudioReady)) 
        ||
        (!_rSwdmxFlashInfo.rFmt.fgAudio)) &&
        ((_rSwdmxFlashInfo.rFmt.fgVideo) && (_rSwdmxFlashInfo.rVar.fgVdecPause)))
    {
        UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
        if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if(!VDEC_Play(ES0, VDEC_FMT_MPV))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
            }
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            if(!VDEC_Play(ES0, VDEC_FMT_H264))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
            }
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if(!VDEC_Play(ES0, VDEC_FMT_WMV))
            {
                LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
            }
        }
        else
        {
            ASSERT(0);
        }
        _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
    }
    
    if((_rSwdmxFlashInfo.rFmt.fgAudio) && (!_rSwdmxFlashInfo.rVar.fgSendAudioEos))
    {
        DMX_AUDIO_PES_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        //_SWDMX_FlashAudioCallback(&rPes);
        UNUSED(AUD_SendAudioPes(&rPes));
        _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE;
        if(!_rSwdmxFlashInfo.rVar.fgGotAudio)
        {
            UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS));
            //AUD_MMAoutEnable(0, TRUE);
            if(!_rSwdmxFlashInfo.rVar.fgSetStc)
            {
                _rSwdmxFlashInfo.rVar.fgSetStc = TRUE;
                STC_StopStc();
                STC_SetStcValue((_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS) - 45000);
                LOG(3, "_SWDMX_FlashHandleEOS AUD_SetStartPts 4 pts 0x%x pos %d\n",
                    (_rSwdmxFlashInfo.rVar.u4LastPTS - _rSwdmxFlashInfo.rVar.u4FirstPTS), (UINT32)_u8SendedSize);
            }
            //STC_StartStc();

            _rSwdmxFlashInfo.rVar.fgGotAudio = TRUE;
            _rSwdmxFlashInfo.rVar.fgGotAudioReady = TRUE;
        }
        LOG(5, "_SWDMX_FlashHandleEOS send dummy EOS to Adec\n");
    }
    if((_rSwdmxFlashInfo.rFmt.fgVideo) && (!_rSwdmxFlashInfo.rVar.fgSendVideoEos))
    {
        DMX_PES_MSG_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        rPes.u4FrameType = PIC_TYPE_I;
        rPes.fgSeqHeader = TRUE;
        rPes.fgGop = TRUE;
        if(!_SWDMX_FlashVideoCallback(&rPes))
        {
            LOG(3, "_SWDMX_FlashHandleSeek _SWDMX_FlashVideoCallback fail\n");
        }
        _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE;
        // i4PlaySpeed = 4x or up.
        // if now in VDEC_PlayOneFrm, VDec might in pause status.
        if((_prSwdmxInfo->i4PlaySpeed < 1000) 
            || (_prSwdmxInfo->i4PlaySpeed >= 4000)
            || (_rSwdmxFlashInfo.rVar.fgVdecPause))
        {
            UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
            if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                {
                    LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                }
            }
            else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_H264))
                {
                    LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                }
            }
            else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                {
                    LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                }
            }
            else
            {
                ASSERT(0);
            }
            _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
        }
        LOG(5, "_SWDMX_FlashHandleEOS send dummy EOS to Vdec\n");
    }
}

INT32 _SWDMX_FlashHandleFifoFull()
{
    BOOL fgRet;
    BOOL fgFull;
	DMX_DDI_T rDDI;    
    UINT32 u4DmxAvailSize;
    UINT32 u4DmxAvailSizeAudio;
    UINT16 u2QueueSize, u2MaxQueueSize;

    fgFull = FALSE;
    
	x_memset(&rDDI, 0, sizeof(rDDI));
	fgRet = DMX_DDI_Get(DMX_DDI_FLAG_DATA_SIZE, &rDDI);
	rDDI.u4DataSize += (10*_u4AlignBase);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_FlashHandleFifoFull DMX_DDI_Get fail.\n");
        return FLASH_SEND_FAIL;
    }

	u4DmxAvailSize = (1024*1024); // ToDo: 3M?
	u4DmxAvailSizeAudio = (1024*1024);
    if(!_fgGetInfo)
    {
        // playbacking, check AV size
        if(_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
        }
        if(_rSwdmxFlashInfo.rFmt.fgAudio)
        {
            u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
            if(u4DmxAvailSize > u4DmxAvailSizeAudio)
            {
                u4DmxAvailSize = u4DmxAvailSizeAudio;
            }
        }
        if (rDDI.u4DataSize >= u4DmxAvailSize)
        {
            u4DmxAvailSize = 0;
        }
        else
        {
            u4DmxAvailSize -= rDDI.u4DataSize;
        }
        if (FLASH_DMX_RESERVED_SIZE >= u4DmxAvailSize)
        {
            u4DmxAvailSize = 0;
        }
        else
        {
            u4DmxAvailSize -= FLASH_DMX_RESERVED_SIZE;
        }
    }
       
	if(u4DmxAvailSize < _u4OneTimeSize)
    {
		LOG(9, "_SWDMX_FlashHandleFifoFull SWDMX Demuxer Buffer is not enough.\n");
		fgFull = TRUE;
    }
	
    if((!_fgGetInfo) && (!fgFull))
    {
        if(_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            VDEC_GetQueueInfo(0, &u2QueueSize, &u2MaxQueueSize);
            if(u2QueueSize > 500)
            {
                fgFull = TRUE;
            }
            else if(u2QueueSize > 30)
            {
                if((_rSwdmxFlashInfo.rFmt.fgVideo) &&
                    (!_rSwdmxFlashInfo.rVar.fgGotVideo))
                {
                	// There are many frames we send to vdec,
                	// but there is still have no sync point.
                	// continue send to vdec.
                    fgFull = FALSE;
                }
                else if(_rSwdmxFlashInfo.rFmt.fgAudio)
                {                    
                    UINT32 au4BufStart[2], au4BufEnd[2];
                    UINT32 u4BufSize;
#ifdef AUD_OLDFIFO_INTF
                    VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], 
                                          &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
                    VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
 #endif
                    u4BufSize          = au4BufEnd[0] - au4BufStart[0];
                    // from aud_drvif.c
                    //#define FIFO_EMPTY_THRESHOLD    (UINT32)(3300) 
                    // avoid audio underflow. CR-DTV00130988
                    //if(!AUD_IsFifoEmpty(AUD_DEC_MAIN))
                    if((u4BufSize - u4DmxAvailSizeAudio) > (10000))
                    {
                        // ToDo: check second audio driver buffer.
                        // maybe don't need this.
                        /*
                        if(_rSwdmxFlashInfo.rFmt.fgAudio2)
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
                    if((fgFull) &&
                       (!_rSwdmxFlashInfo.rVar.fgGotAudio)) // not get audio yet
                    {
                        if(u4DmxAvailSize > _u4OneTimeSize)
                        {
                            fgFull = FALSE; // send more
                        }
                    }
                }
                else
                {
                    fgFull = TRUE;
                }
        		LOG(9, "_SWDMX_FlashHandleFifoFull Vdec esm queue is not enough.\n");
            }
        }
        else if(_rSwdmxFlashInfo.rFmt.fgAudio)// audio only
        {
            // start to play     
            UINT32 au4BufStart[2], au4BufEnd[2];
            UINT32 u4BufSize;

#ifdef AUD_OLDFIFO_INTF
            VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], 
                                  &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
            VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif

            u4BufSize          = au4BufEnd[0] - au4BufStart[0];
            // from aud_drvif.c
            //#define FIFO_EMPTY_THRESHOLD    (UINT32)(3300) 
            // avoid audio underflow. CR-DTV00130988
            if((u4BufSize - u4DmxAvailSizeAudio) > (100000))
            {
                // ToDo: check second audio driver buffer.
                // maybe don't need this.
                /*
                if(_rSwdmxFlashInfo.rFmt.fgAudio2)
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
        }
        else
        {
            // TS MW parsing case, no A/V
        }
    }

    // Handle dmx might use old read/write point when we update this on 
    // _SWDMX_FlashAudioCallback. So do it again.
    if(!_rSwdmxFlashInfo.rVar.fgGotAudio)
    {
        _SWDMX_FlashFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        }
    }
    // ~handle dmx

    if(fgFull)
    {
        if(_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            /*if(_rSwdmxFlashInfo.rVar.fgVdecPause &&
                (((!_rSwdmxFlashInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                  (_rSwdmxFlashInfo.rVar.fgGotAudio &&
                   _rSwdmxFlashInfo.rVar.fgGotAudioReady)) &&
                    (_rSwdmxFlashInfo.rFmt.fgAudio)))
                    */
            BOOL fgAudioReady;
            BOOL fgFeederReady;
            if(!_rSwdmxFlashInfo.rFmt.fgAudio)
            {
                fgAudioReady = TRUE;
            }
            else
            {
                fgAudioReady =
                      ((!_rSwdmxFlashInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                        (_rSwdmxFlashInfo.rVar.fgGotAudio &&
                         _rSwdmxFlashInfo.rVar.fgGotAudioReady));
            }
#ifdef CC_FLASH_TEST
            fgFeederReady = TRUE;
#else
            fgFeederReady = FALSE;
            if(_rSwdmxFlashInfo.rBuf.u4BufAvailCnt >= 2)
            {
                fgFeederReady = TRUE;
            }
            else if(_u8FileSize == (_u8ReadedPos + _rSwdmxFlashInfo.rBuf.u4ReqTotalSize + _rSwdmxFlashInfo.rBuf.u4BufTotalSize))
            {
                // File end
                fgFeederReady = TRUE;
            }
#endif            
            if((_rSwdmxFlashInfo.rVar.fgVdecPause) && (fgAudioReady)
                && (fgFeederReady))
            {
                /*if(!_rSwdmxFlashInfo.rVar.fgGotVideo)
                {
                	// In this case, vdec ESQ is almost full.
                	// Although there is no sync point,
                	// we must start to play.
                    LOG(1, "_SWDMX_FlashHandleFifoFull NG fgGotVideo -> VDEC_Play\n");
                }
                else*/
                {
                    LOG(5, "_SWDMX_FlashHandleFifoFull full -> VDEC_Play\n");
                }
                UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
                if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                    {
                        LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                    }
                }
                else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_H264))
                    {
                        LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                    }
                }
                else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                    {
                        LOG(3, "_SWDMX_FlashHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                    }
                }
                else
                {
                    ASSERT(0);
                }
                _rSwdmxFlashInfo.rVar.fgVdecPause = FALSE;
                AUD_MMAoutEnable(0, TRUE);
            }
        }
        else if((!_rSwdmxFlashInfo.rFmt.fgVideo)
            && (_rSwdmxFlashInfo.rVar.fgGotAudioReady))// audio only
        {
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }
        return FLASH_SEND_FULL;
    }
    
	return FLASH_SEND_OK;
}



INT32 _SWDMX_FlashHandleMoveData(DMX_MM_DATA_T rDmxMMData)
{
    BOOL fgRet;
#ifdef CC_FLASH_TEST   
    static UINT32 u4LastAudioPts = 1, u4LastVideoPts = 1;
    UINT8 u1AID, u1VId;
#endif
    _rSwdmxFlashInfo.rVar.fgCleanAudio = FALSE;
    
    if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_MP3_AUDIO_ES)
    {
        if(_rSwdmxFlashInfo.rFmt.fgAudio)
        {
        	fgRet = DMX_MM_MoveData(_u1PidxAudio, &rDmxMMData, 1000);
            LOG(7, "_SWDMX_FlashHandleMoveData DMX_MM_MoveData %x, S=%d.\n",
                rDmxMMData.u4StartAddr, rDmxMMData.u4FrameSize);
            if(!fgRet)
            {
        		LOG(5, "_SWDMX_FlashHandleMoveData DMX_MM_MoveData Fail.\n");
        		return FLASH_SEND_FAIL;
            }
        }
    }
#ifdef CC_FLASH_TEST
    else if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
    {
        if (_rSwdmxFlashInfo.rFmt.fgAudio && !_fgSendVideo)
        {    
            rDmxMMData.u4Pts = _rFlashPTS[1].au4PTS[_rFlashPTS[1].u2FlashPtsRIdx++];
            if (_rFlashPTS[1].u2FlashPtsRIdx >= FLASH_PTS_CHECK_NUM)
            {
                _rFlashPTS[1].u2FlashPtsRIdx = 0;
            }
            u4LastAudioPts = rDmxMMData.u4Pts;
            u1AID = (_u1PidxAudio != 0xFF) ? _u1PidxAudio : 1;
            fgRet = DMX_MM_MoveData(u1AID, &rDmxMMData, 1000);
        }
        else
        {
            rDmxMMData.u4Pts = _rFlashPTS[0].au4PTS[_rFlashPTS[0].u2FlashPtsRIdx++];
            if (_rFlashPTS[0].u2FlashPtsRIdx >= FLASH_PTS_CHECK_NUM)
            {
                _rFlashPTS[0].u2FlashPtsRIdx = 0;
            }        
            u4LastVideoPts = rDmxMMData.u4Pts;
            u1VId = (_u1PidxVideo != 0xFF) ? _u1PidxVideo : 0;
            fgRet = DMX_MM_MoveData(u1VId, &rDmxMMData, 1000);
        }
        _fgSendVideo = (u4LastAudioPts + 45000 < u4LastVideoPts) ? FALSE : TRUE;
        if (!_rSwdmxFlashInfo.rFmt.fgVideo)
        {
            _fgSendVideo = FALSE;            
            u4LastVideoPts = 1;
        }
        else if (!_rSwdmxFlashInfo.rFmt.fgAudio)
        {
            _fgSendVideo = TRUE;
            u4LastAudioPts = 1;
        }
        LOG(7, "_SWDMX_FlashHandleMoveData DMX_MM_MoveData %x, S=%d.\n",
            rDmxMMData.u4StartAddr, rDmxMMData.u4FrameSize);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_FlashHandleMoveData DMX_MM_MoveData Fail.\n");
        	return FLASH_SEND_FAIL;
        }
    }
#endif
    else
    {/*
        if(_u4BufStart < (4*1024*1024))
        {
            if(rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize > 
                rDmxMMData.u4BufEnd)
            {
            x_memcpy(&_aucBuf[_u4BufStart], rDmxMMData.u4StartAddr,
                rDmxMMData.u4BufEnd - rDmxMMData.u4StartAddr);
            x_memcpy(
                &_aucBuf[_u4BufStart+rDmxMMData.u4BufEnd - rDmxMMData.u4StartAddr],
                rDmxMMData.u4BufStart,
                rDmxMMData.u4FrameSize -
                (rDmxMMData.u4BufEnd - rDmxMMData.u4StartAddr));
            }
            else
            {
                x_memcpy(&_aucBuf[_u4BufStart], rDmxMMData.u4StartAddr,
                    rDmxMMData.u4FrameSize);
            }
            _u4BufStart += rDmxMMData.u4FrameSize;
        }*/
    	fgRet = DMX_DDI_MoveData(&rDmxMMData);
        LOG(9, "_SWDMX_FlashHandleMoveData _DMX_DDI_MoveData 0x%x, S=%d.\n",
            rDmxMMData.u4StartAddr, rDmxMMData.u4FrameSize);
        if(!fgRet)
        {
    		LOG(5, "_SWDMX_FlashHandleMoveData DMX_DDI_MoveData Fail.\n");
    		return FLASH_SEND_FAIL;
        }
    }

    if(_rSwdmxFlashInfo.rVar.fgCleanAudio)
    {
        _SWDMX_FlashFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        }
        _rSwdmxFlashInfo.rVar.fgCleanAudio = FALSE;
    }
    
    return FLASH_SEND_OK;
}


INT32 _SWDMX_FlashHandleFeeder(SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed)
{
    BOOL fgRet;
    BOOL fgBlockReq;
    INT32 i4Ret;
    UINT32 u4Count;
    UINT32 u4ReadSize;
    UINT32 u4Ptr;
	FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
    UINT32 u4ReqCnt;
#endif
    
#ifdef CC_FLASH_TEST
    UINT16 u2Comp = 0;
    u4ReadSize = _u4OneTimeSize;
#else
    u4ReadSize = (_u4OneTimeSize * FLASH_BATCH_READ_SIZE);
#endif
    if(u4ReadSize > ((UINT32)(_u8FileSize - (_u8ReadedPos + _rSwdmxFlashInfo.rBuf.u4ReqTotalSize + _rSwdmxFlashInfo.rBuf.u4BufTotalSize))))
    {
        u4ReadSize = ((UINT32)(_u8FileSize - (_u8ReadedPos + _rSwdmxFlashInfo.rBuf.u4ReqTotalSize + _rSwdmxFlashInfo.rBuf.u4BufTotalSize)));
    }

    fgBlockReq = ((_fgGetInfo) || (!_fgSendSeqHdr) ||
        ((i4PlaySpeed < 0) || (i4PlaySpeed >= 4000))); // 4x use BlockReq
    //fgBlockReq = ((_fgGetInfo) || (!_fgSendSeqHdr)); // 4x use multi request
    //fgBlockReq = TRUE; // request one and wait to got one.
    
    if(fgBlockReq)
    {
        if ((u4ReadSize > 0) &&
            (_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0) &&
            (_rSwdmxFlashInfo.rBuf.u4ReqTimes == 0) &&
            (_rSwdmxFlashInfo.rBuf.u4BufAvailCnt == 0))
        {
#ifdef CC_FLASH_TEST        
            if (!_fgSendVideo && _rSwdmxFlashInfo.rFmt.fgAudio)
            {
                rFeederCond.eMode = FEEDER_AUDIO_CHUNK;
                rFeederCond.fgFirst = (_fgVideoFirst) ? FALSE : TRUE;
            }
            else if (_rSwdmxFlashInfo.rFmt.fgVideo)
            {
                rFeederCond.eMode = FEEDER_VIDEO_CHUNK;
                _fgSendVideo = TRUE;
                rFeederCond.fgFirst = _fgVideoFirst;
            }
            else
            {
                rFeederCond.eMode = FEEDER_NORMAL_MM;
            }
#endif        
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                    u4ReadSize, 
                    0,//(UINT64)(_u8ReadedPos+_rSwdmxFlashInfo.rBuf.u4ReqTotalSize+_rSwdmxFlashInfo.rBuf.u4BufTotalSize),
                    &rFeederCond,
                    _u4FeederReqID);
            _u4FeederReqID+=2;
            if(i4Ret != FEEDER_E_OK)
            {
        	    LOG(1, "Flash Fd FeederSetRequest Fail\n");
        	    _rSwdmxFlashInfo.rVar.fgSendEOF = TRUE;
                if(!_fgGetInfo)
                {
                    _SWDMX_FlashHandleEOS();
                }        	    
        		if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                }
        		return FLASH_SEND_FAIL;
            }
            _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;            
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
            _rSwdmxFlashInfo.rBuf.au4ReqId[_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx] = (_u4FeederReqID-2);
#endif
            _rSwdmxFlashInfo.rBuf.u4ReqTimes++;
            _rSwdmxFlashInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_FLASH_DEBUG_BUFFER
    	    LOG(7, "Flash Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS %d Id %d\n",
    	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
    	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
    	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
    	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
    	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
    	        u4ReadSize,
    	        (UINT32)_u8ReadedPos,
    	        (_u4FeederReqID-2));
            #endif
            _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx++;
            if(_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx >= FLASH_REQ_NUM)
            {
                _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx = 0;
            }     
        }
        else
        {
            #ifdef SWDMX_FLASH_DEBUG_BUFFER
    	    LOG(9, "Flash Fd Use UB %d BT %d RT %d\n",
    	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
    else
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
          FEEDER_E_OK)
        {
            LOG(1, "Flash Fd input buffer error.\n");
            return FALSE;
        }
        if ((u4ReadSize > 0) &&
            (_rSwdmxFlashInfo.rBuf.u4ReqTimes < FLASH_REQ_NUM) &&
            ((_rSwdmxFlashInfo.rBuf.u4ReqTimes + _rSwdmxFlashInfo.rBuf.u4BufAvailCnt) < FLASH_BUF_NUM) &&
            //((_rSwdmxFlashInfo.rBuf.u4UsingBufSize + _rSwdmxFlashInfo.rBuf.u4ReqTotalSize +
            //  _rSwdmxFlashInfo.rBuf.u4BufTotalSize + u4ReadSize) <= 
            // (_u4OneTimeSize*FLASH_BATCH_READ_SIZE*FLASH_BUF_NUM)))
            (_rFeederInfo.u4FreeSize > (_u4OneTimeSize*FLASH_BATCH_READ_SIZE*3)))
        {
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE, 
                    u4ReadSize, 
                    0,//(UINT64)(_u8ReadedPos+_rSwdmxFlashInfo.rBuf.u4ReqTotalSize+_rSwdmxFlashInfo.rBuf.u4BufTotalSize),
                    &rFeederCond,
                    _u4FeederReqID);
            _u4FeederReqID+=2;
            if(i4Ret != FEEDER_E_OK)
            {
        	    LOG(1, "Flash Fd FeederSetRequest Fail\n");
        	    _rSwdmxFlashInfo.rVar.fgSendEOF = TRUE;
                if(!_fgGetInfo)
                {
                    _SWDMX_FlashHandleEOS();
                }        	    
        		if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                }
        		return FLASH_SEND_FAIL;
            }
            _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
            _rSwdmxFlashInfo.rBuf.au4ReqId[_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx] = (_u4FeederReqID-2);
#endif
            _rSwdmxFlashInfo.rBuf.u4ReqTimes++;
            _rSwdmxFlashInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_FLASH_DEBUG_BUFFER
    	    LOG(7, "Flash Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS %d Id %d\n",
    	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
    	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
    	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
    	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
    	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
    	        u4ReadSize,
    	        (UINT32)_u8ReadedPos,
    	        (_u4FeederReqID-2));
            #endif
            _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx++;
            if(_rSwdmxFlashInfo.rBuf.u4ReqWriteIdx >= FLASH_REQ_NUM)
            {
                _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx = 0;
            }
        }
        else
        {
            #ifdef SWDMX_FLASH_DEBUG_BUFFER
    	    LOG(9, "Flash Fd Use UB %d BT %d RT %d\n",
    	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
    	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
    	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
    
    #ifdef SWDMX_STATISTICS
    HAL_GetTime(&_rFeederReqStartTime);
    #endif

    if((_rSwdmxFlashInfo.rBuf.u4ReqTimes) ||
        (_rSwdmxFlashInfo.rBuf.u4BufAvailCnt == 0) || (_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0))
    {
        //if((_rSwdmxFlashInfo.rBuf.u4BufAvailCnt == 0) || (_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0))
        if(_rSwdmxFlashInfo.rBuf.u4ReqTimes > 0)
        {
            u4Count = (5 + FLASH_REQ_NUM); // the times try to remove previous feeder's ack
        	while(u4Count--)
            {
                if((_rSwdmxFlashInfo.rBuf.u4BufAvailCnt == 0) && (_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0))
                //if(_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0)
                {
                    fgRet = _SWDMX_ReceiveFeederAck((VOID*)&rFeederReqData);
                }
                else
                {
                    fgRet = TRUE;
                    i4Ret = _SWDMX_ReceiveFeederAckNoWait((VOID*)&rFeederReqData);
                    if(i4Ret == OSR_NO_MSG) 
                    {
                        break; //return FLASH_SEND_OK;
                    }
                    else if(i4Ret != OSR_OK)
                    {
                	    LOG(5, "Flash Fd AckNoWait i4Ret %d\n", i4Ret);
                        fgRet = FALSE;
                    }
                }
                // check rFeederReqData.u4Id
                if(rFeederReqData.eDataType == FEEDER_SOURCE_INVALID)
                {
            		LOG(5, "Flash Fd FEEDER_SOURCE_INVALID.\n");
            		return FLASH_SEND_FULL;
                }
                if((!fgRet) || (rFeederReqData.u4ReadSize == 0))
                {
            		LOG(3, "Flash Fd _SWDMX_ReceiveFeederAck Fail.\n");
            	    _rSwdmxFlashInfo.rVar.fgSendEOF = TRUE;
                    if(!_fgGetInfo)
                    {
                        _SWDMX_FlashHandleEOS();
                    }            	    
            		if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
            		return FLASH_SEND_FAIL;
                }
#ifdef SWDMX_FLASH_ACCEPT_NON_SEQUENCE_ACK_ID
                for(u4ReqCnt = 0;u4ReqCnt < FLASH_REQ_NUM;u4ReqCnt++)
                {
                    if(rFeederReqData.u4Id == _rSwdmxFlashInfo.rBuf.au4ReqId[u4ReqCnt])
                    {
                        break;
                    }
                }
                if(u4ReqCnt < FLASH_REQ_NUM)
#else
                if(rFeederReqData.u4Id ==
                    ((UINT32)(_u4FeederReqID -
                    (_rSwdmxFlashInfo.rBuf.u4ReqTimes*2))))
#endif
                {
                    _rSwdmxFlashInfo.rBuf.u4ReqTimes--;                    
                    _rSwdmxFlashInfo.rBuf.u4ReqTotalSize -= _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx];
                    if(rFeederReqData.u4ReadSize < _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx])
                    {
                        //LOG(1, "Flash Fd read size too small %d -> %d\n",
                          //  _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx], rFeederReqData.u4ReadSize);
                        //ASSERT(0);
                        //_rSwdmxFlashInfo.rBuf.u4ReqReadIdx++;
                        //if(_rSwdmxFlashInfo.rBuf.u4ReqReadIdx >= FLASH_REQ_NUM)
                        //{
                        //    _rSwdmxFlashInfo.rBuf.u4ReqReadIdx = 0;
                        //}
                        //return FLASH_SEND_FAIL;
                    }            
#ifdef CC_FLASH_TEST

                        //a new frame
                        if (rFeederReqData.eMode == FEEDER_VIDEO_CHUNK)
                        {
                            u2Comp = 0;
                            if (!rFeederReqData.u8Pts)
                            {
                                rFeederReqData.u8Pts = 1;
                            }
                        }
                        else if(rFeederReqData.eMode == FEEDER_AUDIO_CHUNK)
                        {
                            u2Comp = 1;
                            if (!rFeederReqData.u8Pts)
                            {
                                rFeederReqData.u8Pts = 1;
                            }
                        }
                        else
                        {
                            LOG(0, "error ack!!!\n");
                            return FLASH_SEND_FAIL;
                        }
                        _rFlashPTS[u2Comp].au4PTS[_rFlashPTS[u2Comp].u2FlashPtsWIdx++] = 
                            (UINT32)rFeederReqData.u8Pts;
                        if (_rFlashPTS[u2Comp].u2FlashPtsWIdx >= FLASH_PTS_CHECK_NUM)
                        {
                            _rFlashPTS[u2Comp].u2FlashPtsWIdx = 0;
                        }

                    //Actual size from feeder ack
                    _rSwdmxFlashInfo.rBuf.u4ActualSize = rFeederReqData.u4ReadSize;          
#endif
                    _rSwdmxFlashInfo.rBuf.au4BufStartAddr[_rSwdmxFlashInfo.rBuf.u4BufWriteIdx] =
                            rFeederReqData.u4WriteAddr;// +
                            //rFeederReqData.u4AlignOffset;
                    _rSwdmxFlashInfo.rBuf.au4BufSize[_rSwdmxFlashInfo.rBuf.u4BufWriteIdx] = _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx];
                    _rSwdmxFlashInfo.rBuf.u4BufTotalSize += _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx];
                    _rSwdmxFlashInfo.rBuf.u4BufAvailCnt++;
                    #ifdef SWDMX_FLASH_DEBUG_BUFFER
            	    LOG(7, "Flash Fd Get UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d FId %d FAdr %d FOfs %d\n",
            	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
            	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
            	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
            	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
            	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
            	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
            	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
            	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
            	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
            	        _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx],
            	        rFeederReqData.u4Id,
                        rFeederReqData.u4WriteAddr,
                        rFeederReqData.u4AlignOffset);
                    #endif
                    _rSwdmxFlashInfo.rBuf.u4ReqReadIdx++;
                    if(_rSwdmxFlashInfo.rBuf.u4ReqReadIdx >= FLASH_REQ_NUM)
                    {
                        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx = 0;
                    }
                    _rSwdmxFlashInfo.rBuf.u4BufWriteIdx++;
                    if(_rSwdmxFlashInfo.rBuf.u4BufWriteIdx >= FLASH_BUF_NUM)
                    {
                        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx = 0;
                    }     
                    break;
                }
                else
                {
                    if(rFeederReqData.u4WriteAddr)
                    {
                        #ifdef SWDMX_FLASH_DEBUG_BUFFER
                	    LOG(7, "Flash Fd Skp UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d FId %d FAdr %d FRS %d\n",
                	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
                	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
                	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
                	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
                	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
                	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
                	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
                	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
                	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
                	        _rSwdmxFlashInfo.rBuf.au4ReqSize[_rSwdmxFlashInfo.rBuf.u4ReqReadIdx],
                	        rFeederReqData.u4Id,
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4ReadSize);
                        #endif
                        #ifdef CC_FLASH_TEST
                        u4Ptr = rFeederReqData.u4WriteAddr + u4ReadSize;
                        #else
                        u4Ptr = rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
                        #endif
                        if(u4Ptr >= _rFeederInfo.u4EndAddr)
                        {
                            u4Ptr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
                        }
                        if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4Ptr) != FEEDER_E_OK)
                        {
                    		LOG(1, "Flash Fd FeederUpdateReadPointer fail\n");
                    		//ASSERT(0);
                        }
                    }
                }
            }
        	if(u4Count == 0)
            {
            	LOG(0, "Flash Fd _SWDMX_ReceiveFeederAck id is not correct\n");
            	//ASSERT(u4Count > 0);
            	return FLASH_SEND_FAIL;
            }
        }
        if(_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0)
        {
            if(_rSwdmxFlashInfo.rBuf.u4BufAvailCnt > 0) 
            {
#ifdef CC_FLASH_TEST
                _u8ReadedPos += rFeederReqData.u4ReadSize;
#else
                _u8ReadedPos += _rSwdmxFlashInfo.rBuf.au4BufSize[_rSwdmxFlashInfo.rBuf.u4BufReadIdx];
#endif
                _rSwdmxFlashInfo.rBuf.u4UsingBufSize += _rSwdmxFlashInfo.rBuf.au4BufSize[_rSwdmxFlashInfo.rBuf.u4BufReadIdx];                
                _rSwdmxFlashInfo.rBuf.u4UsingStartAddr = _rSwdmxFlashInfo.rBuf.au4BufStartAddr[_rSwdmxFlashInfo.rBuf.u4BufReadIdx];
                _rSwdmxFlashInfo.rBuf.u4BufTotalSize -= _rSwdmxFlashInfo.rBuf.au4BufSize[_rSwdmxFlashInfo.rBuf.u4BufReadIdx];
                _rSwdmxFlashInfo.rBuf.u4BufAvailCnt--;
                
                #ifdef SWDMX_FLASH_DEBUG_BUFFER
        	    LOG(7, "Flash Fd Eat UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d BRS %d UAdr %d\n",
        	        _rSwdmxFlashInfo.rBuf.u4UsingBufSize,
        	        _rSwdmxFlashInfo.rBuf.u4BufTotalSize,
        	        _rSwdmxFlashInfo.rBuf.u4ReqTotalSize,
        	        _rSwdmxFlashInfo.rBuf.u4BufWriteIdx,
        	        _rSwdmxFlashInfo.rBuf.u4BufReadIdx,
        	        _rSwdmxFlashInfo.rBuf.u4BufAvailCnt,
        	        _rSwdmxFlashInfo.rBuf.u4ReqWriteIdx,
        	        _rSwdmxFlashInfo.rBuf.u4ReqReadIdx,
        	        _rSwdmxFlashInfo.rBuf.u4ReqTimes,
        	        _rSwdmxFlashInfo.rBuf.au4BufSize[_rSwdmxFlashInfo.rBuf.u4BufReadIdx],
                    _rSwdmxFlashInfo.rBuf.u4UsingStartAddr);
                #endif
                _rSwdmxFlashInfo.rBuf.u4BufReadIdx++;
                if(_rSwdmxFlashInfo.rBuf.u4BufReadIdx >= FLASH_BUF_NUM)
                {
                    _rSwdmxFlashInfo.rBuf.u4BufReadIdx = 0;
                }
            }
            else
            {
            	LOG(3, "Flash Fd u4BufAvailCnt 0 u4UsingBufSize 0\n");
                ASSERT(_rSwdmxFlashInfo.rBuf.u4BufAvailCnt > 0);
            	return FLASH_SEND_FAIL;
            }
        }
    }
    
	#ifdef SWDMX_STATISTICS
	_CalFeederTime();
	#endif
	
	return FLASH_SEND_OK;
}
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
/**
*/
INT32 _SWDMX_FlashSendData(SWDMX_RANGE_LIST_T* prRangeList, 
            INT32 i4PlaySpeed, UINT64 u8SeekPos, BOOL fgChange)
{
	BOOL fgFirst = FALSE;	
	BOOL fgLast = FALSE;
	INT32 i4Ret;
	UINT32 u4SendSize;
	UINT32 u4Ptr;
    UINT32 u4WaitCnt;
		
	DMX_MM_DATA_T rDmxMMData;
	
	// this is first run for one file, we have to get sequence header
	if(_SWDMX_FlashGetFirstSeqHdr())
    {        
        u8SeekPos = _u8OrgSendedSize;
        fgChange = TRUE;
    }
	
	// handle speed and position
	if(fgChange)
    {
        _SWDMX_FlashHandleChange(i4PlaySpeed, u8SeekPos);
    }

	LOG(9, "_SWDMX_FlashSendData pos %d\n", (UINT32)_u8SendedSize);
		
	if(_rSwdmxFlashInfo.rVar.fgJumpping && _rSwdmxFlashInfo.rVar.fgJumpNext)
    {
        // In worse case,
        // vdp will consume frame in display queue within 3 VSync (pull down).
        // With FPS 25, 1 VSync = 1000/(25*2) = 20;
        // We have to make sure vdp eat this frame.
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
        if((i4PlaySpeed == -1) && (_rSwdmxFlashInfo.rVar.u2JumpAgainCount == 0))
        {
            // no need to pause
            //SWDMX_Pause();
        }
        else
        {
            _SWDMX_FlashHandleSeek(i4PlaySpeed, FALSE);
            _rSwdmxFlashInfo.rVar.fgJumpNext = FALSE;
        }
    }

	// must put ahead _SWDMX_FlashHandleSeek
	// there is a case, after send EOS to decoder
	// and before decoder look EOS, decoder found PIC and notify 
	// _rSwdmxFlashInfo.rVar.fgJumpNext = TRUE
	// we must let _SWDMX_FlashHandleSeek call again to deliver EOS.
	if(_rSwdmxFlashInfo.rVar.fgSendEOF)
    {
		LOG(7, "_SWDMX_FlashSendData _fgSendDataEOS true.\n");
		return FLASH_SEND_EOS;
    }

    if(_rSwdmxFlashInfo.rVar.fgFirst) // first send data
    {
        _rSwdmxFlashInfo.rVar.fgFirst = FALSE;
        fgFirst = TRUE;
        
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) != 
          FEEDER_E_OK)
        {
    	    LOG(3, "_SWDMX_FlashSendData FeederGetBufferInfo Fail\n");
            return FLASH_SEND_FAIL;
        }
    }

    // We have to request feeder data first.
    // Then check fifo status.
	//if((i4Ret = _SWDMX_FlashHandleFifoFull()) != FLASH_SEND_OK)
    //{
    //    return i4Ret;
    //}
	
	u4SendSize = _u4OneTimeSize;

#if 1
    //FIXME: find correct file size
    _u8FileSize = (UINT64)0xFFFFFFFF;
#endif
    
    if(_u8SendedSize >= _u8FileSize)
    {
	    LOG(5, "_SWDMX_FlashSendData SWDMX send (EOF)\n");
	    _rSwdmxFlashInfo.rVar.fgSendEOF = TRUE;

        if(!_fgGetInfo)
        {
            _SWDMX_FlashHandleEOS();            
        }
	    
		if(prRangeList)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
        }
        return FLASH_SEND_EOS;
    }
    
    if(u4SendSize > ((UINT32)(_u8FileSize - _u8SendedSize)))
    {
        u4SendSize = ((UINT32)(_u8FileSize - _u8SendedSize));
    }
    
	x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));
	rDmxMMData.u4FrameSize = u4SendSize;
		
    if(u4SendSize == 0)
    {
        LOG(1, "_SWDMX_FlashSendData u4SendSize < 0 Fail\n");
        ASSERT(0);
    }
    
    if((i4Ret = _SWDMX_FlashHandleFeeder(prRangeList, i4PlaySpeed)) != FLASH_SEND_OK)
    {
        return i4Ret;
    }

	if(_rSwdmxFlashInfo.rBuf.u4UsingBufSize == 0)
    {
    	LOG(0, "_SWDMX_FlashSendData u4UsingBufSize 0\n");
        ASSERT(_rSwdmxFlashInfo.rBuf.u4UsingBufSize > 0);
    	return FLASH_SEND_FAIL;
    }
	
    // Move from above (before _SWDMX_FlashHandleFeeder)
	if((i4Ret = _SWDMX_FlashHandleFifoFull()) != FLASH_SEND_OK)
    {
        return i4Ret;
    }
#ifdef CC_FLASH_TEST
    if((_u8SendedSize + _rSwdmxFlashInfo.rBuf.u4ActualSize) >= _u8FileSize)
#else    
    if((_u8SendedSize + u4SendSize) >= _u8FileSize)
#endif        
    {
        fgLast = TRUE;
    }
#ifdef CC_FLASH_TEST
    rDmxMMData.u4FrameSize = _rSwdmxFlashInfo.rBuf.u4ActualSize;
#endif
    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    if(_rSwdmxFlashInfo.rBuf.u4UsingStartAddr >= rDmxMMData.u4BufEnd)
    {
        _rSwdmxFlashInfo.rBuf.u4UsingStartAddr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
    }
    rDmxMMData.u4StartAddr = (_rSwdmxFlashInfo.rBuf.u4UsingStartAddr);
    if(rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
    {
        rDmxMMData.u4StartAddr -= (rDmxMMData.u4BufEnd - rDmxMMData.u4BufStart);
    }
    
    if(fgFirst)
    {
        rDmxMMData.fgFrameHead = TRUE;
        LOG(7, "_SWDMX_FlashSendData fgFrameHead 0x%x-0x%x, 0x%x, S=%d.\n",        
            rDmxMMData.u4BufStart, rDmxMMData.u4BufEnd,
            rDmxMMData.u4StartAddr, rDmxMMData.u4FrameSize);
    }
    if(fgLast)
    {
        rDmxMMData.fgEOS = TRUE;
    }
    
    if((i4Ret = _SWDMX_FlashHandleMoveData(rDmxMMData)) != FLASH_SEND_OK)
    {
        return i4Ret;
    }

    _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
#ifdef CC_FLASH_TEST    
    _u8SendedSize += rDmxMMData.u4FrameSize;
#else
    //_rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
    _u8SendedSize += u4SendSize;
#endif    
    if(_rSwdmxFlashInfo.rBuf.u4UsingBufSize >= u4SendSize)
    {
        _rSwdmxFlashInfo.rBuf.u4UsingStartAddr += u4SendSize;
        _rSwdmxFlashInfo.rBuf.u4UsingBufSize -= u4SendSize;
        //_u4AvailSize += u4SendSize;
    }
    else
    {
        if(!fgLast)
        {
    		LOG(1, "_SWDMX_FlashSendData u4UsingBufSize not correct\n");
    		ASSERT(0);
        }
        _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
    }
    if(_u8SendedSize > _u8ReadedPos)
    {
		LOG(1, "_SWDMX_FlashSendData _u8SendedSize > _u8ReadedPos\n");
		ASSERT(0);
    }
#ifdef CC_FLASH_TEST
    u4Ptr = rDmxMMData.u4StartAddr + u4SendSize;
#else
    u4Ptr = rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize;
#endif
    if(u4Ptr >= _rFeederInfo.u4EndAddr)
    {
        u4Ptr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
    }
    if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4Ptr) != FEEDER_E_OK)
    {
		LOG(1, "_SWDMX_FlashSendData FeederUpdateReadPointer fail\n");
		//ASSERT(0);
    }
    
    return FLASH_SEND_OK;
}


BOOL _SWDMX_FlashSendDataBatch(VOID)
{
    INT32 i4Ret;
    // detect first
    while(1)
    {
        i4Ret = _SWDMX_FlashSendData(NULL, 0, (UINT64)0, FALSE);
		if(i4Ret == FLASH_SEND_OK)
		{
			continue;
		}
		else if(i4Ret == FLASH_SEND_FULL)
		{
			x_thread_delay(1);
		}  
		else if(i4Ret == FLASH_SEND_EOS)
		{
			LOG(7, "_SWDMX_FlashSendDataBatch _SWDMX_FlashSendData Finish.\n");
			break;
		}   
		else if(i4Ret == FLASH_SEND_FAIL)
		{
			LOG(5, "_SWDMX_FlashSendDataBatch _SWDMX_FlashSendData Fail.\n");
			return FALSE;
		}     
		else
		{
			LOG(5, "_SWDMX_FlashSendDataBatch _SWDMX_FlashSendData Unknow return.\n");
			return FALSE;
		}

		//if(_rSwdmxFlashInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_FlashResetData();
        //}
    }
    return TRUE;
}

BOOL _SWDMX_FlashSendDataBatchForTotalTime(VOID)
{
    INT32 i4Ret;
    // detect first
    while(1)
    {
        i4Ret = _SWDMX_FlashSendData(NULL, 0, (UINT64)0, FALSE);
        if(_rSwdmxFlashInfo.rVar.u4FirstPTS)
        {
	        // there is some fake PTS in this file end.
	        // please check program 4 and 8. (1 base).
	        // [DTV00047346] [TS file playback] Can NOT display the TS, log will print fail info.
		    if(_rSwdmxFlashInfo.rVar.u4LastPTSCnt >= FLASH_PTS_CHECK_NUM)
	        {
	            break;
	        }
        }
		if(i4Ret == FLASH_SEND_OK)
		{
			continue;
		}
		else if(i4Ret == FLASH_SEND_FULL)
		{
			x_thread_delay(1);
		}  
		else if(i4Ret == FLASH_SEND_EOS)
		{
			LOG(7, "_SWDMX_FlashSendDataBatchForTotalTime _SWDMX_FlashSendData Finish.\n");
			break;
		}   
		else if(i4Ret == FLASH_SEND_FAIL)
		{
			LOG(5, "_SWDMX_FlashSendDataBatchForTotalTime _SWDMX_FlashSendData Fail.\n");
			return FALSE;
		}     
		else
		{
			LOG(5, "_SWDMX_FlashSendDataBatchForTotalTime _SWDMX_FlashSendData Unknow return.\n");
			return FALSE;
		}

		//if(_rSwdmxFlashInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_FlashResetData();
        //}
    }
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashBaseInit(VOID)
{
	LOG(5, "_SWDMX_FlashBaseInit\n");
    
	UNUSED(FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE, 
		_SWDMX_FeederCallBack, (UINT32)&_rFeederInfo));
	    
    //DMX_Init();
    //DMX_DDI_Init();
    _SWDMX_FlashResetVariable();
        
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashEsInit(ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;
    FEEDER_BUF_INFO_T rBuffInfo;
    
	LOG(5, "_SWDMX_FlashEsInit\n");
	
    UNUSED(_SWDMX_FlashBaseInit());
    
    // Initialize dmx
    if(eType == SWDMX_FMT_MP3_AUDIO_ES)
    {
        fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        if(!fgRet)
        {
            LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM\n");
            return fgRet;
        }
    }
#ifdef CC_FLASH_TEST    
    else if (eType == SWDMX_FMT_ES_WITH_PTS)
    {
        fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_MM);
        if(!fgRet)
        {
            LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM\n");
            return fgRet;
        }
        fgRet = DMX_SetVideoType(DMX_VIDEO_H264);
        if(!fgRet)
        {
            LOG(3, "DMX_SetVideoType DMX_VIDEO_MPEG\n");
            return fgRet;
        }
        _fgSendVideo = FALSE;
        _fgVideoFirst = FALSE;
    }
#endif
    else
    {
        fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_ES);
        if(!fgRet)
        {
            LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_ES\n");
            return fgRet;
        }
        fgRet = DMX_SetVideoType(DMX_VIDEO_MPEG);
        if(!fgRet)
        {
            LOG(3, "DMX_SetVideoType DMX_VIDEO_MPEG\n");
            return fgRet;
        }
    }
    _rSwdmxFlashInfo.rFmt.eFmt = eType;
    _u4OneTimeSize = FLASH_BATCH_SIZE;
    _u4AlignBase = FLASH_ALIGN_SIZE;

    FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &rBuffInfo);
    if (rBuffInfo.u4Size % _u4OneTimeSize)
    {
        _u4OneTimeSize = 20 * 1024;
    }

    DMX_SetToDecoder(TRUE);
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_FlashVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_FlashAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);
         
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
#ifdef CC_FLASH_TEST
    x_memset(&_rFlashPTS[0], 0x0, sizeof(FLASH_TEST_PTS_T) * 2);
#endif    
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashOpen(ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet;

    if(prStreamInfo == NULL)
    {
    	LOG(1, "_SWDMX_FlashOpen prStreamInfo NULL\n");
    	return FALSE;
    }
    
	LOG(5, "_SWDMX_FlashOpen eType %d\n", eType);
    LOG(5, "    u8FileSize in 32 bits = %d\n", (UINT32)u8FileSize);
	
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
    //_u4AvailSize = 0;
    _u8FileSize = u8FileSize;
    
    if(u8FileSize == 0)
    {
        LOG(1, "_SWDMX_FlashOpen u8FileSize Fail\n");
        return FALSE;
    }     

    fgRet = FALSE;
    if((eType == SWDMX_FMT_MPEG1_DAT) ||
        (eType == SWDMX_FMT_MPEG2_PS) ||
        (eType == SWDMX_FMT_MPEG2_TS) ||
        (eType == SWDMX_FMT_MPEG2_TS_192) ||
        (eType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (eType == SWDMX_FMT_H264_VIDEO_ES) ||
        (eType == SWDMX_FMT_ES_WITH_PTS) ||
        (eType == SWDMX_FMT_MP3_AUDIO_ES))
    {
        if((prStreamInfo->u2VideoNum > 0) && (u4TrackId < prStreamInfo->u2VideoNum))
        {
            if(u4TrackId >= SWDMX_MAX_VID_TRACK)
            {
                LOG(3, "_SWDMX_FlashOpen vid u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_VID_TRACK-1));
                u4TrackId = (SWDMX_MAX_VID_TRACK-1);
            }
            fgRet = _SWDMX_FlashSetInfo((UINT8)eSWDMX_SET_VIDEOTYPE, (UINT32)prStreamInfo->aeVideo[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashSetInfo V Fail\n");
                return FALSE;
            }
            
            fgRet = _SWDMX_FlashSetStrmID((UINT8)eSWDMX_STRM_TYPE_VID, prStreamInfo->au2VideoId[u4TrackId], 0);
            if(!fgRet)
            {
    	        LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashSetStrmID V Fail\n");
    	        return FALSE;
            }
            
            fgRet = _SWDMX_FlashEnableStrm((UINT8)eSWDMX_STRM_TYPE_VID, NULL);
            if(!fgRet)
            {
    	        LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashEnableStrm V Fail\n");
    	        return FALSE;
            }
        }
        if((prStreamInfo->u2AudioNum > 0) && (u4TrackId < prStreamInfo->u2AudioNum))
        {
            if(u4TrackId >= SWDMX_MAX_AUD_TRACK)
            {
                LOG(3, "_SWDMX_FlashOpen aud u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_AUD_TRACK-1));
                u4TrackId = (SWDMX_MAX_AUD_TRACK-1);
            }
            fgRet = _SWDMX_FlashSetInfo((UINT8)eSWDMX_SET_AUDIOTYPE, (UINT32)prStreamInfo->aeAudio[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashSetInfo A Fail\n");
                return FALSE;
            }
            
            fgRet = _SWDMX_FlashSetStrmID((UINT8)eSWDMX_STRM_TYPE_AUD, prStreamInfo->au2AudioId[u4TrackId], prStreamInfo->au2AudioSubId[u4TrackId]);
            if(!fgRet)
            {
    	        LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashSetStrmID A Fail\n");
    	        return FALSE;
            }
            
            fgRet = _SWDMX_FlashEnableStrm((UINT8)eSWDMX_STRM_TYPE_AUD, NULL);
            if(!fgRet)
            {
    	        LOG(3, "_SWDMX_FlashOpen _SWDMX_FlashEnableStrm A Fail\n");
    	        return FALSE;
            }
        }
        if((prStreamInfo->u2VideoNum == 0) && (prStreamInfo->u2AudioNum == 0))
        {
            LOG(3, "_SWDMX_FlashOpen No A/V\n");
            return FALSE;
        }
    }
    else
    {
	    LOG(3, "_SWDMX_FlashOpen Unknown eType\n");
    }
    
    if(!fgRet)
    {
	    LOG(3, "_SWDMX_FlashOpen FALSE\n");
        return FALSE;
    }
    
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    
    UNUSED(prStreamInfo);
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashPlay(VOID)
{    
    BOOL fgRet = TRUE;

    return fgRet;
}

/**
*/
BOOL _SWDMX_FlashPause(VOID)
{
    
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashAbort(VOID)
{
    return TRUE;
}


BOOL _SWDMX_FlashEsSetVideoId(VOID)
{
    DMX_ES_T rDmxES;
    UINT8 u1Pidx = 0;
    UINT32 u4Flags;
    BOOL fgRet;
    FBM_POOL_T* prFbmPool;

	LOG(5, "_SWDMX_FlashEsSetVideoId\n");
	
    if(_u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_FlashEsSetVideoId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }
    
    //set es pid for playback
    
    u4Flags = (DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_VALID);
    x_memset(&rDmxES, 0, sizeof(rDmxES));
    
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    if(prFbmPool == NULL)
    {
    	LOG(1, "FBM_GetPoolInfo Fail\n");
        return FALSE;
    }
    if((prFbmPool->u4Addr == 0) || (prFbmPool->u4Size == 0))
    {
    	LOG(1, "prFbmPool->u4Addr/u4Size 0\n");
        return FALSE;
    }
    rDmxES.fgAllocateBuffer   = FALSE;
    rDmxES.u4BufAddr          = prFbmPool->u4Addr;
    rDmxES.u4BufSize          = prFbmPool->u4Size;
    rDmxES.fgEnable = TRUE;
    rDmxES.ePidType = DMX_PID_TYPE_ES_VIDEO;
    fgRet = DMX_ES_SetPid(u1Pidx, u4Flags, &rDmxES);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_FlashEsSetVideoId DMX_ES_SetPid Fail\n");
        return FALSE;
    }
    //set es filesize
    fgRet = DMX_ES_SetFileLength(u1Pidx,(UINT32)0xFFFFFFFF);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_FlashEsSetVideoId _DMX_ES_SetFileLength Fail\n");
        return FALSE;
    }
    _SWDMX_FlashResetData();
	_u1PidxVideo = u1Pidx;
    
    return TRUE;
}

#ifdef CC_FLASH_TEST
BOOL _FLASH_TESTEsSetVideoId(VOID)
{
    DMX_MM_T rDmxMMInfo;
    UINT8 u1Pidx = 0;
    UINT32 u4Flags;
    BOOL fgRet;

	LOG(5, "_SWDMX_FlashEsSetVideoId\n");
	
    if(_u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_FlashEsSetVideoId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }
    u4Flags = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
      DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF | DMX_MM_FLAG_SEARCH_START_CODE);
    x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));
    rDmxMMInfo.fgEnable = TRUE;
    rDmxMMInfo.ePidType = DMX_PID_TYPE_ES_VIDEO;
    
    //set es pid for playback
    FBM_POOL_T* prFbmPool;                
    prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
    VERIFY(prFbmPool != NULL);
    VERIFY(prFbmPool->u4Addr != 0);
    
    rDmxMMInfo.fgAllocBuf = FALSE;    
    rDmxMMInfo.u4BufAddr = prFbmPool->u4Addr;
    rDmxMMInfo.u4BufSize = prFbmPool->u4Size;
    rDmxMMInfo.fgSearchStartCode = TRUE;     
    
    fgRet = DMX_MM_Set(u1Pidx, u4Flags, &rDmxMMInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_FlashEsSetVideoId DMX_MM_Set Fail\n");
        return FALSE;
    }
	_u1PidxVideo = u1Pidx;
    
    return TRUE;
}
#endif


BOOL _SWDMX_FlashEsSetAudioId(VOID)
{
    DMX_MM_T rDmxMMInfo;
    UINT8 u1Pidx = 1;
    UINT32 u4Flags;
    BOOL fgRet;
    UINT32 au4BufStart[2], au4BufEnd[2];
    
	LOG(5, "_SWDMX_FlashEsSetAudioId\n");
	
    if(_u1PidxAudio != 0xFF)
    {
        LOG(3, "_SWDMX_FlashEsSetAudioId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }

    u4Flags = (DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY | 
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
        LOG(1, "_SWDMX_FlashEsSetAudioId Can't get audio buffers!\n");
        return FALSE;
    }
        
    rDmxMMInfo.fgAllocBuf = FALSE;
    
    rDmxMMInfo.u4BufAddr = au4BufStart[0];
    rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];
    
    fgRet = DMX_MM_Set(u1Pidx, u4Flags, &rDmxMMInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_FlashEsSetAudioId DMX_MM_Set Fail\n");
        return FALSE;
    }
	_u1PidxAudio = u1Pidx;
    
    return TRUE;
}

BOOL _SWDMX_FlashEsFreeVideoId(VOID)
{
    BOOL fgRet;
    DMX_ES_T rDmxES;
    
	LOG(5, "_SWDMX_FlashEsFreeVideoId\n");
	
    if(_u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxES, 0, sizeof(rDmxES));
        rDmxES.fgEnable = FALSE;
        fgRet = DMX_ES_SetPid(_u1PidxVideo, DMX_PID_FLAG_VALID, &rDmxES);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeVideoId DMX_ES_SetPid V Fail\n");
	        //return FALSE;
	    }
        fgRet = DMX_ES_FreePid(_u1PidxVideo);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeVideoId SWDMX_DmxPesReleaseBuffer V Fail\n");
	        //return FALSE;
	    }
        _u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_FlashEsFreeVideoId Fail\n");
    return FALSE;
}

#ifdef CC_FLASH_TEST
BOOL _FLASH_TESTEsFreeVideoId(VOID)
{
    BOOL fgRet;
    DMX_MM_T rDmxMMInfo;
    
	LOG(5, "_SWDMX_FlashEsFreeVideoId\n");
	
    if(_u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxMMInfo, 0, sizeof(DMX_MM_T));
        rDmxMMInfo.fgEnable = FALSE;
        fgRet = DMX_MM_Set(eSWDMX_MEDIA_VIDEO, DMX_PID_FLAG_VALID, &rDmxMMInfo);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeVideoId DMX_MM_Set Fail\n");
	        //return FALSE;
	    }
        fgRet = DMX_MM_Free(_u1PidxVideo);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeVideoId DMX_MM_Free Fail\n");
	        //return FALSE;
	    }
        _u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_FlashEsFreeVideoId Fail\n");
    return FALSE;
}
#endif

BOOL _SWDMX_FlashEsFreeAudioId(VOID)
{
    BOOL fgRet;
    DMX_MM_T rDmxMMInfo;
    
	LOG(5, "_SWDMX_FlashEsFreeAudioId\n");
    
    if(_u1PidxAudio != 0xFF)
    {
        x_memset(&rDmxMMInfo, 0, sizeof(rDmxMMInfo));
        rDmxMMInfo.fgEnable = FALSE;
        fgRet = DMX_MM_Set(_u1PidxAudio, DMX_PID_FLAG_VALID, &rDmxMMInfo);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeAudioId DMX_MM_Set Fail\n");
	        //return FALSE;
	    }
        fgRet = DMX_MM_Free(_u1PidxAudio);
    	if(!fgRet)
	    {
	        LOG(3, "_SWDMX_FlashEsFreeAudioId DMX_MM_Free Fail\n");
	        //return FALSE;
	    }
        _u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_FlashEsFreeAudioId Fail\n");
    return FALSE;
}

      
/**
 To get first and last PTS
*/
BOOL _SWDMX_FlashBaseGetInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet = TRUE;
    UINT32 u4SendSize;
    UINT64 u8Tmp, u8Remainder;
    UINT32 u4LastPTS, u4FirstPTS;
    UINT32 u4PicCnt;
        
    if(prStreamInfo == NULL)
    {
    	LOG(1, "_SWDMX_FlashBaseGetInfo prStreamInfo NULL\n");
    	return FALSE;
    }
    
    u4SendSize = 0;
    u4FirstPTS = 0;
    u4LastPTS = 0;
    
    LOG(5, "_SWDMX_FlashBaseGetInfo\n");

    fgRet = _SWDMX_FlashOpen(eType, 0, u8FileSize, prStreamInfo);
    if(!fgRet)
    {
    	LOG(3, "_SWDMX_FlashBaseGetInfo _SWDMX_FlashOpen Fail.\n");
    	return FALSE;
    }

    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _fgSendSeqHdr = TRUE; // not parse sequence header
    _rSwdmxFlashInfo.rVar.fgNotToDecoder = TRUE;    
    _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
    _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it
        
    // first send    
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8FileSize = u8FileSize;
    if(_u8FileSize > FIRST_PTS_DETECT_SIZE)
    {
        _u8FileSize = FIRST_PTS_DETECT_SIZE;
    }      
    u4SendSize = (UINT32)_u8FileSize;
    
    
    fgRet = _SWDMX_FlashSendDataBatchForTotalTime();
    if(!fgRet)
    {
    	LOG(3, "_SWDMX_FlashBaseGetInfo _SWDMX_FlashSendDataBatch First Fail.\n");
    }
    LOG(5, "First: u4FirstPTS %d, u4PicCnt %d, u4SendSize %d\n",
        _rSwdmxFlashInfo.rVar.u4FirstPTS, _rSwdmxFlashInfo.rVar.u4PicCnt, u4SendSize);
    u4FirstPTS = _rSwdmxFlashInfo.rVar.u4FirstPTS;
    u4LastPTS = _rSwdmxFlashInfo.rVar.u4LastPTS;
    u4PicCnt = _rSwdmxFlashInfo.rVar.u4PicCnt;
    // ~First

    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _rSwdmxFlashInfo.rVar.fgNotToDecoder = TRUE;    
    _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
    _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it
    
    // Second
    _u8SendedSize = 0;
    _u8FileSize = u8FileSize;
    if(_u8FileSize > LAST_PTS_DETECT_SIZE)
    {
        _u8SendedSize = (_u8FileSize - LAST_PTS_DETECT_SIZE);
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;        
    }
    _u8ReadedPos = _u8SendedSize; 
    
    u4SendSize += (UINT32)(_u8FileSize - _u8SendedSize);
    
    fgRet = _SWDMX_FlashSendDataBatchForTotalTime();
    if(!fgRet)
    {
    	LOG(3, "_SWDMX_FlashBaseGetInfo _SWDMX_FlashSendDataBatch Second Fail.\n");
    }
    LOG(5, "Second: u4LastPTS %d, u4PicCnt %d, u4SendSize %d\n",
        _rSwdmxFlashInfo.rVar.u4LastPTS, _rSwdmxFlashInfo.rVar.u4PicCnt, u4SendSize);
    if(_rSwdmxFlashInfo.rVar.u4LastPTS)
    {
        u4LastPTS = _rSwdmxFlashInfo.rVar.u4LastPTS;
        u4PicCnt += _rSwdmxFlashInfo.rVar.u4PicCnt;
    }
    // ~Second

    // Calculate TotalTime
    if((u4FirstPTS == 0) || (u4LastPTS == 0) ||         
        (eType == SWDMX_FMT_ES_WITH_PTS))
    {
        if((u4PicCnt > 0) && (u4SendSize > 0))
        {
            u8Tmp = u8FileSize;
            u8Tmp *= u4PicCnt;
            // ToDo: To have frame rate, 30 frame rate            
            //u8Tmp = u8Div6432(u8Tmp, (UINT64)(u4SendSize*30), &u8Remainder);
            //u8Tmp *= 90000; // milli-seconds -> 90K unit
            u8Tmp = u8Div6432(u8Tmp, (UINT64)(u4SendSize), &u8Remainder);
            u8Tmp *= 3000; // milli-seconds -> 90K unit
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
        prStreamInfo->u4FirstPts = u4FirstPTS;
        // Use 90K time unit now
        // To milli-seconds
        //prStreamInfo->rVar.u4TotalTime *= 1000;
        //prStreamInfo->rVar.u4TotalTime /= 90000;
    }
    // ~Calc
        
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _fgSendSeqHdr = FALSE;
    
    fgRet = _SWDMX_FlashCloseGetInfo();
    if(!fgRet)
    {
    	LOG(3, "_SWDMX_FlashBaseGetInfo _SWDMX_FlashCloseGetInfo Fail.\n");
    	return FALSE;
    }
    UNUSED(u4InfoMask);
    return TRUE;
}


/**
 To get sequence header
*/
BOOL _SWDMX_FlashGetSeqInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet = TRUE;
    INT32 i4Ret;
    UINT32 i;
    UINT32 j;
        
    if(prPInfo == NULL)
    {
    	LOG(1, "_SWDMX_FlashGetSeqInfo prStreamInfo NULL\n");
    	return FALSE;
    }
    
    LOG(5, "_SWDMX_FlashGetSeqInfo\n");

    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _fgSendSeqHdr = FALSE;

    for(i=0;i<prPInfo->u4PgmNum;i++)
    {        
        if(i >= SWDMX_MAX_PROGRAM)
        {
        	LOG(3, "_SWDMX_FlashGetSeqInfo SWDMX_MAX_PROGRAM Fail.\n");
            break;
        }
        for(j=0;j<prPInfo->rPgm[i].u2VideoNum;j++)
        {
            //x_memset(_ucFlashSeqHdr, 0, FLASH_MPEG2_SEQ_HDR_SIZE);
            x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
            if(j >= SWDMX_MAX_VID_TRACK)
            {
            	LOG(3, "_SWDMX_FlashGetSeqInfo SWDMX_MAX_VID_TRACK Fail.\n");
                break;
            }
            //_eVideo = prPInfo->rPgm[i].aeVideo[j];
            fgRet = _SWDMX_FlashOpen(eType, j, u8FileSize, &(prPInfo->rPgm[i]));
            if(!fgRet)
            {
            	LOG(3, "_SWDMX_FlashGetSeqInfo _SWDMX_FlashOpen Fail.\n");
            	continue;
            }
             
            _SWDMX_FlashResetData();
            _SWDMX_FlashResetVariable();
            _u8SendedSize = 0;
            _u8ReadedPos = _u8SendedSize;
            _fgSendSeqHdr = FALSE;
            _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE; // skip it
            _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE; // skip it
            
            
            _u8FileSize = u8FileSize;
            if(_u8FileSize > SEQ_DETECT_SIZE)
            {
                _u8FileSize = SEQ_DETECT_SIZE;
            }        

            // send
            fgRet = TRUE;
            while(1)
            {
                i4Ret = _SWDMX_FlashSendData(NULL, 0, (UINT64)0, FALSE);
                if(_fgSendSeqHdr)
                {
        			break;
                }
        		if(i4Ret == FLASH_SEND_OK)
        		{
        			continue;
        		}
        		else if(i4Ret == FLASH_SEND_FULL)
        		{
        			x_thread_delay(1);
        		}  
        		else if(i4Ret == FLASH_SEND_EOS)
        		{
        			LOG(7, "_SWDMX_FlashGetSeqInfo _SWDMX_FlashSendData Finish.\n");
        			break;
        		}   
        		else if(i4Ret == FLASH_SEND_FAIL)
        		{
        			LOG(5, "_SWDMX_FlashGetSeqInfo _SWDMX_FlashSendData Fail.\n");
        			fgRet = FALSE;
        			break;
        		}     
        		else
        		{
        			LOG(5, "_SWDMX_FlashGetSeqInfo _SWDMX_FlashSendData Unknow return.\n");
        			fgRet = FALSE;
        			break;
        		}        
            }    
            // ~send
            
            // Parsing
            if(_fgSendSeqHdr)
            {
                if(_eVideo == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    prPInfo->rPgm[i].u4Width[j] = _ucFlashSeqHdr[4];
                    prPInfo->rPgm[i].u4Width[j] <<= 4;
                    prPInfo->rPgm[i].u4Width[j] += ((_ucFlashSeqHdr[5] & 0xF0) >> 4);
                    
                    prPInfo->rPgm[i].u4Height[j] = (_ucFlashSeqHdr[5] & 0xF);
                    prPInfo->rPgm[i].u4Height[j] <<= 8;
                    prPInfo->rPgm[i].u4Height[j] += _ucFlashSeqHdr[6];
                }
                else if(_eVideo == ENUM_SWDMX_VDEC_H264)
                {
                    prPInfo->rPgm[i].u4Width[j] = _rSeqInfo.u2_width;
                    prPInfo->rPgm[i].u4Height[j] = _rSeqInfo.u2_height;
                }
            }
            _fgSendSeqHdr = FALSE;
            // ~Parsing
            
            fgRet = _SWDMX_FlashCloseGetInfo();
            if(!fgRet)
            {
            	LOG(3, "_SWDMX_FlashGetSeqInfo _SWDMX_FlashCloseGetInfo Fail.\n");
            	//return FALSE;
            }
        }
    }

    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _fgSendSeqHdr = FALSE;

    UNUSED(u4InfoMask);
    
    return TRUE;
}


/**
*/
BOOL _SWDMX_FlashEsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet;
	
    if(prPInfo == NULL)
    {
    	LOG(1, "_SWDMX_FlashEsGetStreamInfo prStreamInfo NULL\n");
    	return FALSE;
    }
    
	LOG(5, "_SWDMX_FlashEsGetStreamInfo\n");
    
    if(!_SWDMX_FlashEsInit(eType))
    {
    	LOG(1, "_SWDMX_FlashEsGetStreamInfo _SWDMX_FlashEsInit fail\n");
    	return FALSE;
    }

    prPInfo->u4PgmNum = 0;
    if(eType == SWDMX_FMT_ES_WITH_PTS)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_H264;
    }
    else if(eType == SWDMX_FMT_MPEG2_VIDEO_ES)
    {
        prPInfo->rPgm[0].aeVideo[0] = ENUM_SWDMX_VDEC_MPEG1_MPEG2;
    }
    else
    {
        LOG(3, "_SWDMX_FlashEsGetStreamInfo Unknown eType %d\n", eType);
        return FALSE;
    }
    prPInfo->u4PgmNum = 1;
    prPInfo->rPgm[0].u2VideoNum = 1;
    prPInfo->rPgm[0].u2AudioNum = 0;
    prPInfo->rPgm[0].u2SubpicNum = 0;
    prPInfo->rPgm[0].au2VideoId[0] = 0xE0;
    
    // to get total duration
#ifdef CC_FLASH_TEST
    fgRet = TRUE;
#else
    fgRet = _SWDMX_FlashBaseGetInfo(eType, 
                        u4InfoMask,
                        u8FileSize,
                        &(prPInfo->rPgm[0]));
#endif
    if(fgRet)
    {
        LOG(5, "_SWDMX_FlashEsGetStreamInfo return TRUE\n");
        return TRUE;
    }
    LOG(3, "_SWDMX_FlashEsGetStreamInfo return FALSE\n");
    return FALSE;
}


/**
*/
BOOL _SWDMX_FlashGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet;

    if(prPInfo == NULL)
    {
    	LOG(1, "_SWDMX_FlashGetStreamInfo prStreamInfo NULL\n");
    	return FALSE;
    }
    
	LOG(5, "_SWDMX_FlashGetStreamInfo eType %d\n", eType);
	LOG(5, "    u8FileSize in 32 bits= %d\n", (UINT32)u8FileSize);

    if(u8FileSize == 0)
    {
        LOG(5, "_SWDMX_FlashGetStreamInfo u8FileSize Fail\n");
        return FALSE;
    }     
    
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _fgSendSeqHdr = TRUE;
	_fgGetInfo = TRUE;
    _u8FileSize = u8FileSize;
    _rSwdmxFlashInfo.rVar.fgNotToDecoder = TRUE;    
    _rSwdmxFlashInfo.rVar.fgSendVideoEos = TRUE;
    _rSwdmxFlashInfo.rVar.fgSendAudioEos = TRUE;
    

    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_TRACK)
    {
        fgRet = FALSE; 

        if (eType == SWDMX_FMT_ES_WITH_PTS)
        {
        	if(u8FileSize > ES_OPEN_FILE_SIZE)
    	    {
    	        _u8FileSize = ES_OPEN_FILE_SIZE;
    	    }
            fgRet =  _SWDMX_FlashEsGetStreamInfo(eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
        {
    	    LOG(3, "_SWDMX_FlashGetStreamInfo Unknown eType\n");
        }
        if(!fgRet)
        {
            _fgGetInfo = FALSE;
    	    LOG(3, "_SWDMX_FlashGetStreamInfo TRACK FALSE\n");
            return FALSE;
        }
    }	
    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_RESOLUTION)
    {
        fgRet =  _SWDMX_FlashGetSeqInfo(eType, u4InfoMask, u8FileSize, prPInfo);
        if(!fgRet)
        {
            _fgGetInfo = FALSE;
    	    LOG(3, "_SWDMX_FlashGetStreamInfo RESOLUTION FALSE\n");
            return FALSE;
        }
    }
    
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();
    _fgGetInfo = FALSE;
	
    LOG(5, "_SWDMX_FlashGetStreamInfo Result:\n"); 
    LOG(5, "    u2VideoNum     = %d\n", prPInfo->rPgm[0].u2VideoNum);
    LOG(5, "    u2AudioNum     = %d\n", prPInfo->rPgm[0].u2AudioNum);
    LOG(5, "    u2VideoId[0]   = %d\n", prPInfo->rPgm[0].au2VideoId[0]);
    LOG(5, "    u2AudioId[0]   = %d\n", prPInfo->rPgm[0].au2AudioId[0]);
    LOG(5, "    u4TotalTime    = %d\n", prPInfo->rPgm[0].u4TotalTime);
    LOG(5, "    u8FileSize in 32 bits = %d\n", (UINT32)u8FileSize);
    LOG(5, "    eVideo[0]      = %d\n", prPInfo->rPgm[0].aeVideo[0]);
    LOG(5, "    eAudio[0]      = %d\n", prPInfo->rPgm[0].aeAudio[0]);
    
    return TRUE;
}

/**
*/
BOOL _SWDMX_FlashCloseGetInfo(VOID)
{
	LOG(5, "_SWDMX_FlashCloseForOpen\n");
    UNUSED(_SWDMX_FlashDisableStrm((UINT8)eSWDMX_STRM_TYPE_VID));
    UNUSED(_SWDMX_FlashDisableStrm((UINT8)eSWDMX_STRM_TYPE_AUD));
    UNUSED(_SWDMX_FlashDisableStrm((UINT8)eSWDMX_STRM_TYPE_SUBTITLE));
    return _SWDMX_FlashStop();
}

/**
*/
BOOL _SWDMX_FlashClose(VOID)
{
	LOG(5, "_SWDMX_FlashClose\n");
    return _SWDMX_FlashStop();
}


/**
*/
VOID _SWDMX_FlashSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID, 
								      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    MM_RANGE_INFO_T* prRangeInfo = 0;;
	LOG(5, "_SWDMX_FlashSetRange\n");

	if(prRangeList == NULL)
	{
		LOG(0, "Demuxer Range is NULL\n");
		return;
	}
	
    prRangeInfo = prRangeList->prDmxRangeInfo;
        
	if (prRangeInfo != NULL)
	{        
        _SWDMX_FlashResetData();
        _SWDMX_FlashResetVariable();

		_u8FileSize = prRangeInfo->z_range_sz;
		if((_prSwdmxInfo->fgPlayChange) &&
          (((UINT32)(_prSwdmxInfo->u8SeekPos>>32) != 0xFFFFFFFF)
            || ((UINT32)(_prSwdmxInfo->u8SeekPos) != 0xFFFFFFFF)))
	    {
    		_u8SendedSize = _prSwdmxInfo->u8SeekPos;
            _prSwdmxInfo->u8SeekPos = 0xFFFFFFFF;
            _prSwdmxInfo->u8SeekPos <<= 32;
            _prSwdmxInfo->u8SeekPos += 0xFFFFFFFF;
            _prSwdmxInfo->fgPlayChange = FALSE;
	    }
		else
	    {
    		_u8SendedSize = prRangeInfo->ui8_fileoffset;
	    }
        _u8ReadedPos = _u8SendedSize;
        _rSwdmxFlashInfo.rBuf.u4UsingBufSize = 0;
        //_u4AvailSize = 0;
        _rSwdmxFlashInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxFlashInfo.rVar.u8PreviousIPos = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
		_rSwdmxFlashInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
		_rSwdmxFlashInfo.rVar.u4TotalTime = (UINT32)prRangeInfo->ui8_pb_duration;
        _rSwdmxFlashInfo.rVar.u4TotalTimeOffset = 0;
		_rSwdmxFlashInfo.rVar.u4FirstPTS = (UINT32)prRangeInfo->t_pts_offset;
		_rSwdmxFlashInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        LOG(5, "_SWDMX_FlashSetRange _u8FileSize %d, _u8SendedSize %d, u4TotalTime %d, u4FirstPTS %d\n",
            (UINT32)_u8FileSize, (UINT32)_u8SendedSize,
            (UINT32)_rSwdmxFlashInfo.rVar.u4TotalTime, (UINT32)_rSwdmxFlashInfo.rVar.u4FirstPTS);
        
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
SWDMX_HDR_PRS_STATE_T _SWDMX_FlashHeaderParse(
	SWDMX_RANGE_LIST_T* prRangeList,
	INT32 i4PlaySpeed,
	UINT64 u8SeekPos,
	BOOL fgSpeedChange)
{
    INT32 i4Ret;
	//LOG(9, "_SWDMX_FlashHeaderParse\n");
	    
	i4Ret = _SWDMX_FlashSendData(prRangeList, i4PlaySpeed, u8SeekPos, fgSpeedChange);
	if(i4Ret == FLASH_SEND_OK)
	{
		return eSWDMX_HDR_PRS_SUCCEED;
	}
	else if(i4Ret == FLASH_SEND_FULL)
	{
        x_thread_delay(10);
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
	}  
	else if(i4Ret == FLASH_SEND_EOS)
	{
		LOG(7, "_SWDMX_FlashHeaderParse _SWDMX_FlashSendData Finish.\n");
		return eSWDMX_HDR_PRS_SUCCEED;
	}   
	else if(i4Ret == FLASH_SEND_FAIL)
	{
        x_thread_delay(1);
		LOG(5, "_SWDMX_FlashHeaderParse _SWDMX_FlashSendData Fail.\n");
        return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
	}
	else
	{
	}

	LOG(5, "_SWDMX_FlashHeaderParse _SWDMX_FlashSendData Unknow return.\n");
    return eSWDMX_HDR_FED_BUF_NOT_ENOUGH;
}
				

/**
*/
BOOL _SWDMX_FlashStop(VOID)
{    
	LOG(5, "_SWDMX_FlashStop\n");
    
    _SWDMX_FlashResetData();
    _SWDMX_FlashResetVariable();    
    _fgSendSeqHdr = FALSE; // not parse sequence header
        
	return TRUE;
}

/**
*/
BOOL _SWDMX_FlashFlush(VOID)
{
    _SWDMX_FlashResetData();
    return TRUE;
}

BOOL _SWDMX_FlashSetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
	LOG(5, "_SWDMX_FlashSetInfo u1Type = %d, u4Para1 = %d\n", u1Type, u4Para1);
	
    if(u1Type == (UINT8)eSWDMX_SET_VIDEOTYPE)
    {
        _rSwdmxFlashInfo.rFmt.eVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;
        
        if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return DMX_SetVideoType(DMX_VIDEO_MPEG);
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            return DMX_SetVideoType(DMX_VIDEO_H264);
        }
        else if(_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            return DMX_SetVideoType(DMX_VIDEO_VC1);
        }
        else if((_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_DX311) || 
            (_rSwdmxFlashInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4))
        {
            return DMX_SetVideoType(DMX_VIDEO_MPEG4);
        }
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxFlashInfo.rFmt.eAudioType = (ENUM_SWDMX_ADEC_T)u4Para1;
        return TRUE;
    }
        
    UNUSED(u4Para2);
    UNUSED(u4Para3);
    return FALSE;
}


BOOL _SWDMX_FlashGetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    UINT64 u8Offset, u8OffsetI;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
	LOG(6, "_SWDMX_FlashGetInfo u1Type = %d\n", u1Type);
	
    if(u1Type == (UINT8)eSWDMX_GET_CURRENT_POSITION)
    {
        if(pu4Para1 == NULL)
        {
        	LOG(1, "_SWDMX_FlashGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        pu8Tmp = (UINT64*)(VOID*)pu4Para1;

        u8Offset = 0;
        u8OffsetI = 0;
        u2DecodingOrder = 0;
        i4TempRef = 0;
        UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
        if(u8OffsetI > _u8FileSize)
        {
            LOG(7, "_SWDMX_FlashGetInfo VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
                (UINT32)u8OffsetI, (UINT32)_u8FileSize);
            u8OffsetI = _u8FileSize;
        }
        if(u8OffsetI)
        {
            *pu8Tmp = _u8SendedSize;
        }
        else
        {
            *pu8Tmp = _u8SendedSize;
        }
        return TRUE;
    }

    UNUSED(pu4Para2);
    UNUSED(pu4Para3);
    return FALSE;
}

BOOL _SWDMX_FlashSetStrmID(UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
	LOG(5, "_SWDMX_FlashSetStrmID u1HdrType = %d, u4StrmID = %d, u4StrmSubID = %d\n",
	    u1HdrType, u4StrmID, u4StrmSubID);
	
    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxFlashInfo.rFmt.u2VideoId = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxFlashInfo.rFmt.u2AudioId = u4StrmID;
            _rSwdmxFlashInfo.rFmt.u2AudioSubId = u4StrmSubID;
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            _rSwdmxFlashInfo.rFmt.u2Audio2Id = u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxFlashInfo.rFmt.u2SubTitleId = u4StrmID;
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

BOOL _SWDMX_FlashEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    BOOL fgRet = TRUE;
    LOG(5, "_SWDMX_FlashEnableStrm, u1StrmType =%d, eFmt = %d\n",
        u1StrmType, _rSwdmxFlashInfo.rFmt.eFmt);
    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(!_rSwdmxFlashInfo.rFmt.fgVideo)
            {
                _fgSendSeqHdr = FALSE;
                if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
                {
#ifdef CC_FLASH_TEST                
                    fgRet = _FLASH_TESTEsSetVideoId();
#else
                    fgRet = _SWDMX_FlashEsSetVideoId();
#endif
                }
                else
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxFlashInfo.rFmt.fgVideo = TRUE;
#ifdef CC_FLASH_TEST
                    if (_rSwdmxFlashInfo.rFmt.fgAudio)
                    {
                        _fgVideoFirst = FALSE;
                    }
                    else
                    {
                        _fgVideoFirst = TRUE;
                    }
#endif
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(!_rSwdmxFlashInfo.rFmt.fgAudio)
            {
                _rSwdmxFlashInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
                _rSwdmxFlashInfo.rVar.fgGotAudio = FALSE;
                _rSwdmxFlashInfo.rVar.fgGotAudioReady = FALSE;

                if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_MP3_AUDIO_ES)
                {
                    fgRet = _SWDMX_FlashEsSetAudioId();
                }
#ifdef CC_FLASH_TEST
                else if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
                {
                    fgRet = _SWDMX_FlashEsSetAudioId();
                }
#endif
                else
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxFlashInfo.rFmt.fgAudio = TRUE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            if(!_rSwdmxFlashInfo.rFmt.fgAudio2)
            {                
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if(!_rSwdmxFlashInfo.rFmt.fgSubTitle)
            {
                _rSwdmxFlashInfo.rFmt.fgSubTitle = TRUE;
                return TRUE;
            }
            break;
        default:
            break;
    }
    
    LOG(3, "_SWDMX_FlashEnableStrm %d\n", fgRet);
    UNUSED(pfnCbFunc);
    return TRUE;
}


BOOL _SWDMX_FlashDisableStrm(UINT8 u1StrmType)
{
    BOOL fgRet = TRUE;
	LOG(5, "_SWDMX_FlashDisableStrm u1StrmType = %d\n", u1StrmType);
	    
    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(_rSwdmxFlashInfo.rFmt.fgVideo)
            {
                if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
                {
#ifdef CC_FLASH_TEST
                    fgRet = _FLASH_TESTEsFreeVideoId();
#else
                    fgRet = _SWDMX_FlashEsFreeVideoId();
#endif
                }
                else if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_MP3_AUDIO_ES)
                {
                    fgRet = _SWDMX_FlashEsFreeVideoId();
                }
                else
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxFlashInfo.rFmt.fgVideo = FALSE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(_rSwdmxFlashInfo.rFmt.fgAudio)
            {
                if(_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_MP3_AUDIO_ES)
                {
                    fgRet = _SWDMX_FlashEsFreeAudioId();
                }
#ifdef CC_FLASH_TEST
                else if (_rSwdmxFlashInfo.rFmt.eFmt == SWDMX_FMT_ES_WITH_PTS)
                {
                    fgRet = _SWDMX_FlashEsFreeAudioId();
                }
#endif                
                else
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxFlashInfo.rFmt.fgAudio = FALSE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxFlashInfo.rFmt.fgSubTitle = FALSE;
            //fgRet = TRUE;
            break;
        default:
            //fgRet = FALSE;
            break;
    }

    LOG(3, "_SWDMX_FlashDisableStrm do nothing\n");
    return TRUE;
}


VOID _SWDMX_FlashSetSwdmxInfo(SWDMX_INFO_T* prSwdmxInfo)
{
    if(prSwdmxInfo == NULL)
    {
        return;
    }
    _prSwdmxInfo = prSwdmxInfo;
    UNUSED(_prSwdmxInfo);
}


#ifdef SWDMX_STATISTICS
void _SWDMX_ResetTime(void)
{
    _u4FeederAvgTime = 0;
    _u4FeederCount = 0;
    _u4FeederMaxTime = 0;
    _u4FeederMinTime = 0;
}

void _SWDMX_PrintTime(void)
{
    LOG(0, "Feeder Count(%d) Time(%d, %d, %d)\n",
        _u4FeederCount,
        _u4FeederMaxTime, _u4FeederAvgTime, _u4FeederMinTime);
}
    
#endif
