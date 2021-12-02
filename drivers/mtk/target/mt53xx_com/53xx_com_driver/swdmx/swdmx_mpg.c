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
 * $RCSfile: swdmx_mpg.c,v $
 * $Revision: #1 $
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

#if defined(CC_MT5387) || defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#include "b2r_drvif.h"
#include "b2r_if.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
//#define SWDMX_MPG_DEBUG_BUFFER
//#define SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
//#define SWDMX_MPG_DEBUG_INPUT_DATA
//#define SWDMX_MPG_ACCEPT_NON_SEQUENCE_ACK_ID
#define SWDMX_MPG_CHECK_MP3_AUDIO
#define SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
//#define SWDMX_MPG_SET_PCR_ID
#define SWDMX_MPG_USE_INDEX
//#define SWDMX_MPG_USE_MTMPLAYER_PVR



#ifdef CC_MT5391
#if !defined(CC_TS_VC1_DISABLE) || !defined(CC_ES_VC1_DISABLE)
#define SWDMX_MPG_VC1_SUPPORT
#endif
#endif
#if !defined(CC_PS_MPEG2_DISABLE) || !defined(CC_PS_MPEG1_DISABLE) \
 || !defined(CC_TS_MPEG2_DISABLE) || !defined(CC_TS_MPEG1_DISABLE) \
 || !defined(CC_ES_MPEG2_DISABLE) || !defined(CC_ES_MPEG1_DISABLE)
#define SWDMX_MPG_MPEG2_SUPPORT
#endif
#if !defined(CC_ES_MPEG4_DISABLE)
#define SWDMX_MPG_MPEG4_SUPPORT
#endif
#if !defined(CC_TS_H264_DISABLE) || !defined(CC_ES_H264_DISABLE)
#define SWDMX_MPG_H264_SUPPORT
#endif

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
#define STREAM_TYPE_H264        0x1B
#define STREAM_TYPE_VC1         0xEA
#define STREAM_TYPE_VIDEO       2
#define STREAM_TYPE_MPEG1       3
#define STREAM_TYPE_MPEG2       4
#define STREAM_TYPE_AC3_EU      0xBD
#define STREAM_TYPE_AC3         0x81
#define STREAM_TYPE_EAC3        0x87
#define STREAM_TYPE_AAC         0x11
#define STREAM_TYPE_AAC_ISDB    0xF
#define STREAM_TYPE_CYPHER      0x80
#define STREAM_TYPE_UNKNOWN     0xFF

#define SWDMX_PIC_TYPE_OTHER    0x0
#define SWDMX_PIC_TYPE_I        0x1
#define SWDMX_PIC_TYPE_P        0x2
#define SWDMX_PIC_TYPE_NO_I     0x4

#define MPG_REQ_NUM            4
#define MPG_REQ_NUM_MAX        4
#define MPG_REQ_TIMEOUT_NUM   10
#define MPG_REQ_BUFFULL_NUM   30
#define MPG_BUF_NUM            6
#define MPG_BUF_NUM_RESERVED   3
#define MPG_BUF_NUM_MAX       40
#define MPG_PTS_CHECK_NUM      5

#define MPG_STC_BASE (1) // when we send PTS to a/vdec, alwasy add this base
#define MPG_ALIGN_SIZE (188*4) // 188 and 16 byte align
#define MPG_ALIGN_SIZE_PVR (1536) // 192 and 512 align. Org: (192) 192 and 16 byte align
#define MPG_ALIGN_SIZE_TS_192 (192) // 192 and 16 byte align
#define MPG_BATCH_SIZE (188*128)
#define MPG_BATCH_SIZE_PVR (192*128)
#define MPG_BATCH_SIZE_TS_192 (192*128)
#define MPG_BATCH_READ_SIZE (10)
#define MPG_BATCH_TIME_SMALL_FILE (16)
#define MPG_BATCH_TIME_RHAPSODY (2)
#define MPG_DLNA_FEEDER_UNREACH (1024*1024*1024)
#define MPG_DLNA_FEEDER_READY_SIZE (8*1024*1024)
#define MPG_DLNA_FEEDER_UNDERFLOW_SIZE (1*1024*1024)
#define MPG_DLNA_INDEX_FEEDER_READY_SIZE (1)
#define MPG_DLNA_INDEX_FEEDER_UNDERFLOW_SIZE (0)
#define MPG_FEEDER_RESERVED_SIZE (MPG_BATCH_SIZE_PVR*MPG_BATCH_READ_SIZE*MPG_BUF_NUM_RESERVED)
#define MPG_DMX_RESERVED_SIZE (MPG_ALIGN_SIZE*5)
#define MPG_LBA_BLOCK_SIZE (192)
#define MPG_AUDIO_ES_SKIP_TIME (90000)
#define MPG_SEND_OK     0
#define MPG_SEND_FULL   1
#define MPG_SEND_FAIL   2
#define MPG_SEND_EOS    3
#define MPG_SEND_LAST   4                       // for DLNA handle EOF,  the last buffer will used for dmx data sending.
#define MPG_SEND_SKIP   5
#define MPG_TIMER_DELAY_TIMES 10
//#define MPG_TIME_SHIFT_SAFE_RANGE_END_THRESHOLD  (5)
#define MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER (3)

#define MPG_RING_OK         0
#define MPG_RING_FAIL       1
#define MPG_RING_END_MEET   2

#define MPG_AUDIO_INDEX_OK     0
#define MPG_AUDIO_INDEX_FAIL   1
#define MPG_AUDIO_INDEX_MEET   2

#define MPG_MPEG2_SEQ_HDR_SIZE 12

//TIME_SHIFT_STAGE TSS
#define MPG_TSS_NORMAL               0
#define MPG_TSS_PREPARE_NO_SIGNAL    11
#define MPG_TSS_PREPARE_BAD_STORAGE  12
#define MPG_TSS_UNDER_NO_SIGNAL      21
#define MPG_TSS_UNDER_BAD_STORAGE    22

#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
#define MPG_MP3_CHECK_CNT 3
#endif

#ifdef SWDMX_MPG_USE_MTMPLAYER_PVR

#define SWDMX_MPG_TICK_FLAG_CHECK(_pt_tk_idx, _ui4_flags) (0)
/* tick index info, synchronize with MTMPLAYER */
typedef struct _SWDMX_MPG_PVR_TICK_INDEX_T
{
    UINT32              ui4_tick_num;  // the tick number
    UINT32              ui4_lba;       // the corresponding lba position
                                       // LBA: Logical Block Address
} SWDMX_MPG_PVR_TICK_INDEX_T;

#else

#define SWDMX_MPG_TICK_FLAG_CHECK(_pt_tk_idx, _ui4_flags)                      \
    (((_pt_tk_idx)->ui4_flags & _ui4_flags))
#define SWDMX_MPG_PVR_TICK_INDEX_T PVR_TICK_INDEX_T

#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static FEEDER_BUF_INFO_T _rFeederInfo;

#ifndef CC_TS_DISABLE
static PSIPSR_TBL_PMT_T _arPMTTbl[SWDMX_MAX_PMT_NUM];
#endif

//static UINT32 _u4BufStart = 0;
//static UCHAR _aucBuf[5*1024*1024];

#ifdef SWDMX_MPG_PS_GET_INFO
static HANDLE_T _hFindTimer = 0;
static HANDLE_T _hFindMsgQ = 0;
static HANDLE_T _hDataTimer = 0;
static HANDLE_T _hDataMsgQ = 0;
#endif

static UINT32 _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
static UINT32 _fgSendSeqHdrVc1 = FALSE;
#endif
static UINT64 _u8OrgSendedSize = 0;

static BOOL _fgGetInfo = FALSE;
static BOOL _fgCheckPSISize = FALSE;
static UINT64 _u8PlayPos = 0;
static UINT64 _u8SendedSize = 0;
static UINT64 _u8ReadedPos = 0;
static UINT64 _u8StartFileOffset = 0;
static UINT64 _u8FileSize = 0;
static UINT32 _u4FeederReqID = 1;
#if 1//def CC_DLNA_SUPPORT
static UINT32 _u4FeederBufSize = 0;
#endif
static UINT32 _u4FeederBufNum = MPG_BUF_NUM;
static UINT32 _u4FeederReqNum = MPG_REQ_NUM;

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

//static UINT8 _ucMpgSeqHdr[MPG_MPEG2_SEQ_HDR_SIZE] = {0};
#ifdef SWDMX_MPG_VC1_SUPPORT
//static UINT8 _ucVc1SeqHdr[MPG_VC1_SEQ_HDR_SIZE] = {0};
//static UINT8 _ucVc1EntryHdr[MPG_VC1_ENTRY_HDR_SIZE] = {0};
#endif
static VDEC_SEQUENCE_DATA_T _rSeqInfo;


static UINT8 _u1PidxVideo = 0xFF;
static UINT8 _u1PidxAudio = 0xFF;
/* not support now
static UINT8 _u1PidxAudio2 = 0xFF;
*/
#ifdef SWDMX_MPG_SET_PCR_ID
static UINT8 _u1PidxPCR = 0xFF;
#endif

static UINT32 _u4OneTimeSize = MPG_BATCH_SIZE;
static UINT32 _u4OneTimeReadSize = (MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE);
static UINT32 _u4AlignBase = MPG_ALIGN_SIZE;


static SWDMX_INFO_T* _prSwdmxInfo = 0;
//static ENUM_SWDMX_VDEC_T _eVideo = ENUM_SWDMX_VDEC_UNKNOWN;

// the count for check if no sequence header in file
//static UINT32 _u4SeqHdrChk=0;

#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
typedef struct _SWDMX_MPG_MP3_HDR_FIELD_T
{
    /* 32 Bit */
    UINT32            u4Emphasis        :    2;
    UINT32            u4Original        :    1;
    UINT32            u4Copyright       :    1;
    UINT32            u4ModeExtionsion  :    2;
    UINT32            u4ChannelMode     :    2;
    UINT32            u4Private         :    1;
    UINT32            u4Padding         :    1;
    UINT32            u4SampleRate      :    2;
    UINT32            u4BitateIdx       :    4;
    UINT32            u4Protection      :    1;
    UINT32            u4Layer           :    2;
    UINT32            u4Version         :    2;
    UINT32            u4FrameSync       :    11;
} SWDMX_MPG_MP3_HDR_FIELD_T;

typedef union _SWDMX_MPG_MP3_HDR
{
LINT_SUPPRESS_BRACE(960)

    UINT32                       au4Reg[1];
    SWDMX_MPG_MP3_HDR_FIELD_T    rField;
} SWDMX_MPG_MP3_HDR;
#endif

typedef struct
{
 #if 1//def CC_DLNA_SUPPORT                              //for DLNA handle EOF
    BOOL fgGetEof;          // this is the flag for DLNA, when get EOF from feeder.
    BOOL fgLastData;      // this is the flag for DLNA,  have got EOF and there is no  availble buffer remain
  #endif

    // this is buffer we keep after feeder's ack
    UINT32 u4BufAvailCnt;
    UINT32 u4BufReadIdx;
    UINT32 u4BufWriteIdx;
    UINT32 u4BufTotalSize;
    UINT32 au4BufStartAddr[MPG_BUF_NUM_MAX];
    UINT32 au4BufSize[MPG_BUF_NUM_MAX];
    UINT32 au4BufPTS[MPG_BUF_NUM_MAX];
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
    UINT64 au4BufFileOffset[MPG_BUF_NUM_MAX];
#endif

    // this is buffer under requesting to feeder
    UINT32 u4ReqTimes;
    UINT32 u4ReqTimeoutCount;
    UINT32 u4ReqBufFullCount;
    UINT32 u4ReqReadIdx;
    UINT32 u4ReqWriteIdx;
    UINT32 u4ReqTotalSize;
    UINT32 au4ReqSize[MPG_REQ_NUM_MAX];
    UINT32 au4ReqPTS[MPG_REQ_NUM_MAX];
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
    UINT64 au4ReqFileOffset[MPG_REQ_NUM_MAX];
#endif
    UINT32 au4ReqId[MPG_REQ_NUM_MAX];
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
    UINT32 au4ReqDup[MPG_REQ_NUM_MAX]; // duplicate request flag
#endif

    // this is a small buffer we are consuming currently.
    UINT32 u4UsingPTS;
    UINT32 u4UsingBufSize;
    UINT32 u4UsingStartAddr;
}SWDMX_MPG_BUFFER_T;


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
    UINT8 u1VideoKeyIndex;
    UINT8 u1AudioKeyIndex;
    UINT8 u1Audio2KeyIndex;
    UINT16 u2VideoId;
    UINT16 u2AudioId;
    UINT16 u2AudioSubId;
    UINT16 u2Audio2Id;
    UINT16 u2SubTitleId;
    UINT16 u2PcrId;
    UINT16 u2PidIdxStart;
    UINT16 u2VideoPidIdx;
    UINT16 u2AudioPidIdx;
    UINT16 u2Audio2PidIdx;
    UINT32 u4VideoDescMode;
    UINT32 u4AudioDescMode;
    UINT32 u4Audio2DescMode;

}SWDMX_MPG_FMT_T;

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
    BOOL fgEnableVideo;
    BOOL fgEnableAudio;
    BOOL fgGotVideo;
    BOOL fgGotAudio;
    BOOL fgGotVideoReady;
    BOOL fgGotAudioReady;
    BOOL fgVideoPause;
    BOOL fgChangeVideo;
    BOOL fgCleanAudio;
    #ifdef SWDMX_MPG_USE_INDEX
    BOOL fgUseIdx;
    #endif
    BOOL fgVdecPowerOff;

    UINT16 u2OrderFromIPos;
    UINT16 u2JumpAgainCount;

    INT32 i4CurSpeed;

    UINT32 u4CurIndexNum;        //Send index num

    UINT32 u4FirstPTS;
    UINT32 u4FirstVideoPTS;
    UINT32 u4SecondVideoPTS;      // to keep original PTS in STC
    UINT32 u4FirstAudioModifyPTS;   // first audio pts audio - first pts
    UINT32 u4LastAudioPTS;
    UINT32 u4LastVideoPTS;             //DTV00139313     to record the last video frame's PTS,which swdmx have send to vdec
    UINT32 u4LastPTS;
    UINT32 u4LastPTSCnt;
    UINT32 au4LastPTS[MPG_PTS_CHECK_NUM];
    UINT32 u4PicCnt;
    UINT32 u4TotalTime;
    UINT32 u4TotalTimeOffset;

	UINT32 u4GetFirstAudioPTS;         //DTV00146545 to record the first audio pts which has been get ,after reset fgSetStc,

    UINT32 u4LastVideoWptr;
    UINT32 u4LastAudioWptr;
    UINT32 u4LastAudioFifoDataSize;
    UINT32 u4PreviousPTS;

    UINT32 u4SeqHdrChk;         // the count for check if no sequence header in file

    UINT64 u8OffsetKeep;                         // attribute of keeped frame in vdp
    UINT64 u8OffsetIKeep;
    UINT16 u2DecodingOrderKeep;
    INT32  i4TempRefKeep;

#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    UINT32 u4Mp3Cnt;
#endif

    UINT64 u8PreviousIPos;
    UINT64 u8PreviousIPosWithPTSFound;
    UINT64 u8Previous2IPosWithPTSFound;
    UINT64 u8PreviousSendedSize;
    UINT64 u8PreviousSendedSizeOfNon1X;
    UINT64 u8JumpStartAddr;

    MM_SRC_TYPE_T e_mm_src_type;
    SWDMX_RANGE_LIST_T *prCurRangeReq;

#ifdef TIME_SHIFT_SUPPORT
    BOOL fgNoUseIFrmInfo;
    BOOL fgValidRange;
    BOOL fgOutOfRange;
    //BOOL fgOutOfSafeRange;
    BOOL fgMonitorLBA;
    BOOL fgMonitorSafeStart;
    BOOL fgMonitorSafeEnd;
    BOOL fgMonitorValidStart;
    BOOL fgMonitorValidEnd;
    UINT32 u4NoSignalStage;     // 0: Signal, 1: Wait VDP no Signal, 2: Swdmx no signal
    UINT32 u4TickBase;
    UINT32 u4QueueTickNum;
    UINT32 u4NoSignalTickNum;   //No signal tick num
    UINT32 u4CurTickNum;        //Send tick num
    UINT32 u4ReadTickNum;       //Read tick num
    UINT32 u4SafeStartTick;
    UINT32 u4SafeEndTick;
    UINT32 u4ValidStartTick;
    UINT32 u4ValidEndTick;
    UINT32 u8QueueStartAddr[MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER];
    UINT64 u8CurTime;           // milli-second, not 90K unit
    UINT64 u8MonitorAddr;
    UINT64 u8SafeStartAddr;
    UINT64 u8SafeEndAddr;
    UINT64 u8ValidStartAddr;
    UINT64 u8ValidEndAddr;
    UINT64 u8FifoOffset;
    UINT64 u8FifoSize;
    UINT32 u4OrgtDeltaTime;           //when speed=1000, in no signal status calibrate u8CurTime
    BOOL fgNeedAdjust;                // needed to adjust u8CurTime
    SWDMX_MPG_PVR_TICK_INDEX_T *prLastTickIndex;
    SWDMX_MPG_PVR_TICK_INDEX_T *prLastEndTickIndex;
    MM_REC_BUF_RANGE_INFO_T rRangeInfo;
    PVR_TICK_CTRL_BLK_T rTickCtl;
#endif
}SWDMX_MPG_VARIABLE_T;

typedef struct
{
    SWDMX_MPG_BUFFER_T rBuf;
    SWDMX_MPG_FMT_T rFmt;
    SWDMX_MPG_VARIABLE_T rVar;

}SWDMX_MPG_INFO_T;

static SWDMX_MPG_INFO_T _rSwdmxMpgInfo;
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
VOID _SWDMX_MpgCreateFindSema(VOID);
VOID _SWDMX_MpgDeleteFindSema(VOID);
//INT32 _SWDMX_MpgSelectPSStream(DMX_NOTIFY_INFO_STREAM_ID_T *prInfo);
VOID _SWDMX_MpgFlushVFifo(VOID);
VOID _SWDMX_MpgFlushAFifo(VOID);
BOOL _SWDMX_MpgWatchVideoPTS(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_MpgWatchAudioPTS(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_MpgAudioCallback(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_MpgVideoCallback(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_MpgAudioCallbackParse(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_MpgVideoCallbackMpgParse(const DMX_PES_MSG_T* prPes);
#ifdef SWDMX_MPG_VC1_SUPPORT
BOOL _SWDMX_MpgVideoCallbackVc1Parse(const DMX_PES_MSG_T* prPes);
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
BOOL _SWDMX_MpgVideoCallbackH264Parse(const DMX_PES_MSG_T* prPes);
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
BOOL _SWDMX_MpgVideoCallbackMpeg4Parse(const DMX_PES_MSG_T* prPes);
#endif
VOID _SWDMX_MpgSeekFinNotify(UINT32 u4PTS, BOOL fgAgain);

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetVideoId(UINT16 u2VideoId);
BOOL _SWDMX_MpgPsSetAudioId(UINT16 u2AudioId, UINT16 u2AudioSubId);
BOOL _SWDMX_MpgPsFreeVideoId(VOID);
BOOL _SWDMX_MpgPsFreeAudioId(VOID);
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetVideoId(UINT16 u2VideoId, UINT8 u1KeyIndex, UINT32 u4DescMode);
BOOL _SWDMX_MpgTsSetAudioId(UINT16 u2AudioId, UINT16 u2DeviceId, UINT8 u1KeyIndex, UINT32 u4DescMode);
BOOL _SWDMX_MpgTsSetPCRId(UINT16 u2PCRId);
BOOL _SWDMX_MpgTsFreeVideoId(VOID);
BOOL _SWDMX_MpgTsFreeAudioId(VOID);
BOOL _SWDMX_MpgTsFreePCRId(VOID);
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetVideoId(VOID);
BOOL _SWDMX_MpgEsSetAudioId(VOID);
BOOL _SWDMX_MpgEsFreeVideoId(VOID);
BOOL _SWDMX_MpgEsFreeAudioId(VOID);
#endif

BOOL _SWDMX_MpgWatchDuration(UINT32 u4CurentTime);
UINT32 _SWDMX_MpgWatchLastPTS(UINT32 u4Type, UINT32 u4Pts);
VOID _SWDMX_MpgResetVariable(VOID);
VOID _SWDMX_MpgJumpToNormal(BOOL fgSeek, BOOL fgJumpping, INT32 i4Speed);
VOID _SWDMX_MpgHandleChange(INT32 i4PlaySpeed, UINT64 u8SeekPos);
VOID _SWDMX_MpgHandleEOS(INT32 i4PlaySpeed);
BOOL _SWDMX_MpgResetFeeder(VOID);
VOID _SWDMX_MpgResetData(VOID);
INT32 _SWDMX_MpgHandleFifoFull(INT32 i4PlaySpeed, BOOL fgLast);
INT32 _SWDMX_MpgHandleMoveData(DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed);
UINT32 _SWDMX_MpgHandleAudioIndex(UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, UINT32 *pu4OutReadPTS, INT32 i4PlaySpeed);
UINT32 _SWDMX_MpgHandleRing(UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, INT32 i4PlaySpeed);
INT32 _SWDMX_ReceiveFeederAck(VOID *pvData);
BOOL _SWDMX_MpgGetFirstSeqHdr(VOID);
VOID _SWDMX_MpgHandleSeek(INT32 i4PlaySpeed, BOOL fgSeek);
VOID _SWDMX_MpgHandleSeekAudio(INT32 i4PlaySpeed, BOOL fgSeek);
INT32 _SWDMX_MpgHandleFeeder(SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed);
INT32 _SWDMX_MpgSendData(SWDMX_RANGE_LIST_T* prRangeList,
            INT32 i4PlaySpeed, UINT64 u8SeekPos, BOOL fgChange);
BOOL _SWDMX_MpgSendDataBatch(VOID);
BOOL _SWDMX_MpgSendDataBatchForTotalTime(VOID);
BOOL _SWDMX_MpgBaseInit(VOID);
BOOL _SWDMX_MpgOpen(ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prOpenInfo);
BOOL _SWDMX_MpgBaseGetInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo);
BOOL _SWDMX_MpgGetSeqInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo);
BOOL _SWDMX_MpgIsSpeedAvailable(BOOL fgForward);
INT32 _SWDMX_MpgHandleNoSignel(UINT64 *pu8SeekPos, INT32 i4PlaySpeed);
INT32 _SWDMX_MpgHandleEmpty(UINT64 *pu8SeekPos, INT32 i4PlaySpeed);
BOOL _SWDMX_MpgResetFeederReq(void);
#ifdef TIME_SHIFT_SUPPORT
UINT64 _SWDMX_MpgTSAlignDown(UINT64 u8Position);
UINT64 _SWDMX_MpgLBA2Pos(UINT32 u4LBA);
#endif

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
#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_Stop())
        {
            LOG(3, "_SWDMX_DMX_Reset _SWDMX_PVR_Stop fail\n");
            return FALSE;
        }
        return DMX_PVRPlay_RequestReset();
    }
    else
#endif
    {
        return DMX_DDI_RequestReset();
    }
}

#ifdef SWDMX_MPG_PS_GET_INFO
static VOID _SWDMX_FlushFindMsgQ(VOID)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(INT32);

    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &_hFindMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hFindMsgQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}

static VOID _SWDMX_TimerFindTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4DecFin;

    u4DecFin = DETECT_TIMEOUT;
    VERIFY(x_msg_q_send(_hFindMsgQ, (VOID *)&u4DecFin, sizeof(UINT32), 255) == OSR_OK);

    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
}
#endif

#ifdef SWDMX_MPG_PS_GET_INFO
static VOID _SWDMX_FlushDataMsgQ(VOID)
{
    UINT16 u2Cnt;
    UINT16 u2MsgQIdx;
    UINT32 u4DecFin;
    SIZE_T zMsgSize;

    zMsgSize = sizeof(INT32);

    while (x_msg_q_receive(&u2MsgQIdx, &u4DecFin, &zMsgSize,
        &_hDataMsgQ, 1, X_MSGQ_OPTION_NOWAIT) == OSR_OK)
    {
    }

    VERIFY(x_msg_q_num_msgs(_hDataMsgQ, &u2Cnt) == OSR_OK);
    ASSERT(u2Cnt== 0);
}


static VOID _SWDMX_TimerDataTimeout(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UINT32 u4DecFin;

    u4DecFin = DETECT_TIMEOUT;
    VERIFY(x_msg_q_send(_hDataMsgQ, (VOID *)&u4DecFin, sizeof(UINT32), 255) == OSR_OK);

    UNUSED(pt_tm_handle);
}
#endif

#ifdef SWDMX_MPG_PS_GET_INFO
VOID _SWDMX_MpgCreateFindSema(VOID)
{
    if(!_hFindMsgQ)
    {
        VERIFY(x_msg_q_create(&_hFindMsgQ, "Find-Q", sizeof(UINT32), 2) == OSR_OK);
    }
    if(!_hDataMsgQ)
    {
        VERIFY(x_msg_q_create(&_hDataMsgQ, "Data-Q", sizeof(UINT32), 2) == OSR_OK);
    }
    if(!_hFindTimer)
    {
        VERIFY(x_timer_create(&_hFindTimer) == OSR_OK);
    }
    if(!_hDataTimer)
    {
        VERIFY(x_timer_create(&_hDataTimer) == OSR_OK);
    }
}

VOID _SWDMX_MpgDeleteFindSema(VOID)
{
    if(_hFindMsgQ)
    {
        VERIFY(x_msg_q_delete(_hFindMsgQ) == OSR_OK);
        _hFindMsgQ = 0;
    }
    if(_hDataMsgQ)
    {
        VERIFY(x_msg_q_delete(_hDataMsgQ) == OSR_OK);
        _hDataMsgQ = 0;
    }
    if(_hFindTimer)
    {
        VERIFY(x_timer_delete(_hFindTimer) == OSR_OK);
        _hFindTimer = 0;
    }
    if(_hDataTimer)
    {
        VERIFY(x_timer_delete(_hDataTimer) == OSR_OK);
        _hDataTimer = 0;
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

VOID _SWDMX_MpgFlushVFifo(VOID)
{
    BOOL fgRet;
    if(_u1PidxVideo != 0xFF)
    {
        if(_rSwdmxMpgInfo.rVar.u4LastVideoWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(_u1PidxVideo, _rSwdmxMpgInfo.rVar.u4LastVideoWptr, _rSwdmxMpgInfo.rVar.u4LastVideoWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgFlushVFifo DMX_UpdateReadPointer Fail\n");
            }
            _rSwdmxMpgInfo.rVar.u4LastVideoWptr = 0;
        }
    }
}

VOID _SWDMX_MpgFlushAFifo(VOID)
{
    BOOL fgRet;
    if(_u1PidxAudio != 0xFF)
    {
        if(_rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
        {
            fgRet = DMX_UpdateReadPointer(_u1PidxAudio, _rSwdmxMpgInfo.rVar.u4LastAudioWptr, _rSwdmxMpgInfo.rVar.u4LastAudioWptr);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgFlushAFifo DMX_UpdateReadPointer Fail\n");
            }
            _rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
        }
    }
}

BOOL _SWDMX_MpgWatchDuration(UINT32 u4CurentTime)
{
    UINT32 u4FilteredTime;
    UINT32 u4NewTotalTime;
    UINT64 u8NewTotalTime;
    UINT64 u8Remainder;

    u4FilteredTime = _SWDMX_MpgWatchLastPTS(0xFF, (u4CurentTime + _rSwdmxMpgInfo.rVar.u4FirstPTS));
    u4FilteredTime -= _rSwdmxMpgInfo.rVar.u4FirstPTS;
    // be careful with u4FilteredTime when it is closed to u4TotalTime
    // add 10 second time for monitor
    if((u4FilteredTime >
        ((_rSwdmxMpgInfo.rVar.u4TotalTime + _rSwdmxMpgInfo.rVar.u4TotalTimeOffset) + (10*90000))) &&
        (_u8SendedSize > 0))
    {
        u8NewTotalTime = _SWDMX_Div6464((UINT64)(u4FilteredTime+1), _u8SendedSize, &u8Remainder);
        u8NewTotalTime = u8NewTotalTime * (UINT64)(_u8FileSize - _u8StartFileOffset);
        u8NewTotalTime *= 90;
        u8NewTotalTime = _SWDMX_Div6464(u8NewTotalTime, (UINT64)100, &u8Remainder);
        if(u8NewTotalTime < (UINT64)0xFFFFFFFF)
        {
            u4NewTotalTime = (UINT32)u8NewTotalTime;
            if(u4NewTotalTime > _rSwdmxMpgInfo.rVar.u4TotalTime)
            {
                _rSwdmxMpgInfo.rVar.u4TotalTimeOffset = (u4NewTotalTime - _rSwdmxMpgInfo.rVar.u4TotalTime);
                LOG(3, "_SWDMX_MpgWatchDuration u4TotalTimeOffset %d\n",
                    _rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
            }
        }
    }

    return TRUE;
}


UINT32 _SWDMX_MpgWatchLastPTS(UINT32 u4Type, UINT32 u4Pts)
{
    UINT32 i;
    UINT32 u4Temp = 0;
    UINT32 u4Big = 0;
    UINT32 u4Small = 0;
    UINT32 u4RetPts = u4Pts;
    UINT64 u8Temp = 0;
    UINT64 u8Remainder;

    //if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
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
            if(_rSwdmxMpgInfo.rVar.au4LastPTS[i] != 0)
            {
                if(u4Big < _rSwdmxMpgInfo.rVar.au4LastPTS[i])
                {
                    u4Big = _rSwdmxMpgInfo.rVar.au4LastPTS[i];
                }
                if(u4Small > _rSwdmxMpgInfo.rVar.au4LastPTS[i])
                {
                    u4Small = _rSwdmxMpgInfo.rVar.au4LastPTS[i];
                }
            }
        }
        for(i=0;i<(MPG_PTS_CHECK_NUM-1);i++)
        {
            _rSwdmxMpgInfo.rVar.au4LastPTS[i] =
                _rSwdmxMpgInfo.rVar.au4LastPTS[i+1];
            if(u4Big == _rSwdmxMpgInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(u4Small == _rSwdmxMpgInfo.rVar.au4LastPTS[i])
            {
                continue;
            }
            if(_rSwdmxMpgInfo.rVar.au4LastPTS[i] != 0)
            {
                u8Temp += _rSwdmxMpgInfo.rVar.au4LastPTS[i];
                u4Temp++;
            }
        }
        // we don't record non a/v PTS.
        if(u4Type != 0xFF)
        {
            _rSwdmxMpgInfo.rVar.au4LastPTS[MPG_PTS_CHECK_NUM-1] = u4Pts;
            _rSwdmxMpgInfo.rVar.u4LastPTSCnt++;
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
                    if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
                    {
                        _rSwdmxMpgInfo.rVar.u4LastPTS = u4Temp;
                    }
                    if(u4Type != 0xFF)
                    {
                        _rSwdmxMpgInfo.rVar.au4LastPTS[i] = u4Temp;
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
                    if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
                    {
                        _rSwdmxMpgInfo.rVar.u4LastPTS = u4Temp;
                    }
                    if(u4Type != 0xFF)
                    {
                        _rSwdmxMpgInfo.rVar.au4LastPTS[i] = u4Temp;
                    }
                    u4RetPts = u4Temp;
                }
            }
        }
    }
    return u4RetPts;
}

BOOL _SWDMX_MpgWatchVideoPTS(const DMX_PES_MSG_T* prPes)
{
    UINT32 u4Temp = 0;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgWatchVideoPTS prPes NULL\n");
        return FALSE;
    }

    if((prPes->fgPtsDts) && (prPes->u4Pts > 0))
    {
        if(_rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxMpgInfo.rVar.u4FirstPTS = (prPes->u4Pts);

            if(_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU)
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
               (!_fgGetInfo))
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
#endif
            LOG(5, "_SWDMX_MpgWatchVideoPTS u4FirstPTS 0x%x\n", _rSwdmxMpgInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((prPes->u4Pts < _rSwdmxMpgInfo.rVar.u4FirstPTS) &&
                ((_rSwdmxMpgInfo.rVar.u4FirstPTS - prPes->u4Pts) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                _rSwdmxMpgInfo.rVar.u4FirstPTS = prPes->u4Pts;
            }
        }

        if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
           (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
            LOG(5, "_SWDMX_MpgWatchVideoPTS fgSetStc\n");

            if(!_rSwdmxMpgInfo.rVar.fgNotToDecoder)
            {
                //DTV00144863 When time shift for audio only file
                //no place to resume STC, so we can't stop stc
                LOG(3, "_SWDMX_MpgWatchVideoPTS STC_SetStcValue 0x%x\n",
                    ((prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                STC_StopStc();
                // When there is video stream, stc value will set at vdec_flw.c
                if(!_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    u4Temp = STC_GetStcValue();
                    STC_SetStcValue((UINT32)((prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                    u4Temp = STC_GetStcValue();
                    if (u4Temp != (UINT32)((prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE))
                    {
                        LOG(3, "_SWDMX_MpgWatchVideoPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }

                // do on vdec_flw.c
                //STC_StartStc();
            }
        }

        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            UNUSED(_SWDMX_MpgWatchLastPTS(_u1PidxVideo, prPes->u4Pts));
        }

        #ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
        #endif
            // DTV00210035, two wrong audio PTS. We should not update duration
            //firstPTS=0x8ca9,LastPTS=0xcb139
            //audio PTS=0x3e4cc936
            //audio PTS=0x3e4cc936
            if(((prPes->u4Pts > _rSwdmxMpgInfo.rVar.u4LastPTS) && ((prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4LastPTS) < (600*90000)))||
               ((prPes->u4Pts < _rSwdmxMpgInfo.rVar.u4LastPTS) && ((_rSwdmxMpgInfo.rVar.u4LastPTS - prPes->u4Pts) < (600*90000))))
            {
                UNUSED(_SWDMX_MpgWatchDuration(prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4FirstPTS));
            }
        #ifdef TIME_SHIFT_SUPPORT
        }
        #endif

        LOG(6, "video u4Pts 0x%x, pos 0x%x\n",
            ((prPes->u4Pts - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE),
            (UINT32)_rSwdmxMpgInfo.rVar.u8PreviousSendedSize);

        _rSwdmxMpgInfo.rVar.u4LastPTS = prPes->u4Pts;
       _rSwdmxMpgInfo.rVar.u4LastVideoPTS=prPes->u4Pts;  // DTV00139313  record last video pts
    }

    _rSwdmxMpgInfo.rVar.u4LastVideoWptr = prPes->u4FrameAddr;

    _rSwdmxMpgInfo.rVar.u4PicCnt++;
    #if 0
    if(_rSwdmxMpgInfo.rVar.u4PicCnt%150)
    {
        LOG(6, "_SWDMX_MpgWatchVideoPTS u4PicCnt 150\n");
    }
    #endif

    return TRUE;
}

BOOL _SWDMX_MpgWatchAudioPTS(const DMX_AUDIO_PES_T* prPes)
{
    UINT32 u4PTS;
    UINT32 u4Temp;
    UINT32 u4DmxAvailSize;

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
    if((!_rSwdmxMpgInfo.rVar.fgGotAudio)&&
       (!_rSwdmxMpgInfo.rFmt.fgVideo)&&
       (u4PTS == 0)&&
       (_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU))
    {
        u4PTS = (MPG_STC_BASE);
    }
    
    if(u4PTS > 0)
    {
        if(_rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxMpgInfo.rVar.u4FirstPTS = (u4PTS);

            if(_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU)
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }

#ifdef TIME_SHIFT_SUPPORT
            if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
               (!_fgGetInfo))
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
#endif
            // in es file, we never get pts.
            // so assume we already found pts.
            if (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
                LOG(5, "_SWDMX_MpgWatchAudioPTS u4FirstPTS 0x%x\n", _rSwdmxMpgInfo.rVar.u4FirstPTS);
            }

            LOG(5, "_SWDMX_MpgWatchAudioPTS u4FirstPTS %d\n", _rSwdmxMpgInfo.rVar.u4FirstPTS);
        }
        else
        {
            if((u4PTS < _rSwdmxMpgInfo.rVar.u4FirstPTS) &&
                ((_rSwdmxMpgInfo.rVar.u4FirstPTS - u4PTS) < (8*90000)))
            {
                // only consider PTS in same GOP.
                // First PTS might be I frame's PTS.
                // current one might be PTS of B frame before I frame.
                // temp reference max is 255
                // 255 / 30 -> 9 second.
                _rSwdmxMpgInfo.rVar.u4FirstPTS = u4PTS;
            }
        }

        if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
           (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
			//record the first audio pts after reset fgSetStc(for exp:do seek)
			_rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS=u4PTS;    //record the first audio pts after reset fgSetStc(for exp:do seek)

            LOG(5, "_SWDMX_MpgWatchAudioPTS fgSetStc GetFirstAudioPTS:0x%x\n",
				_rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS);


            if(!_rSwdmxMpgInfo.rVar.fgNotToDecoder)
            {
                LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                    (((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE)));
                STC_StopStc();
                // When there is video stream, stc value will set at vdec_flw.c
                if(!_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    u4Temp = STC_GetStcValue();
                    STC_SetStcValue((UINT32)((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                    u4Temp = STC_GetStcValue();
                    if (u4Temp != (UINT32)((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE))
                    {
                        LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }
                // do on vdec_flw.c
                //STC_StartStc();
            }
        }

        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            UNUSED(_SWDMX_MpgWatchLastPTS(_u1PidxAudio, u4PTS));
        }

#ifdef SWDMX_MPG_SYNC_DTV_MODE
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;

            _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
            //VDP_TriggerAudReceive(_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
            //UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));

            //LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 1 pts 0x%x pos 0x%llx\n",
            //    ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)),
            //    _rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
        }
#endif
        //DTV00146545, for audio only ts file, if we have get 8sec audio data but still can't get video data, we
        // should set fgGotVideo TRUE, and let audio start
		if((_rSwdmxMpgInfo.rFmt.fgVideo)&&
			(!_rSwdmxMpgInfo.rVar.fgGotVideo)&&
			(_rSwdmxMpgInfo.rVar.u4LastAudioPTS>_rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)&&
			((_rSwdmxMpgInfo.rVar.u4LastAudioPTS-_rSwdmxMpgInfo.rVar.u4GetFirstAudioPTS)>(90000*8)))


		{
			VDEC_Notify(ES0,VDEC_DEC_DECODE_NO_DATA);
			//VDEC_Notify(ES0,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
			_rSwdmxMpgInfo.rVar.fgGotVideo=TRUE;
			_rSwdmxMpgInfo.rVar.u4FirstVideoPTS=_rSwdmxMpgInfo.rVar.u4FirstPTS;
			_rSwdmxMpgInfo.rVar.u4SecondVideoPTS=_rSwdmxMpgInfo.rVar.u4FirstPTS;

			LOG(3,"_swdmx_mpgAudioCallback no video data and set fgGotVideo=TRUE \n");
		}

        if(_rSwdmxMpgInfo.rVar.fgGotVideo && (!_rSwdmxMpgInfo.rVar.fgGotAudio))
        {
            if((_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X) &&
               // delay 0.3 second to output audio in time-shfit
               (((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (u4PTS >= (_rSwdmxMpgInfo.rVar.u4FirstVideoPTS + (30000)))) ||
                ((_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (u4PTS >= _rSwdmxMpgInfo.rVar.u4FirstVideoPTS)))
               &&
               // For 2x->1x, we must send audio data after the last PTS
               ((u4PTS >= _rSwdmxMpgInfo.rVar.u4PreviousPTS)
                ||
                // Error handle for we can't find target the PTS
                ((_rSwdmxMpgInfo.rVar.u4PreviousPTS - u4PTS) > (90000*10))))
            // This is orginal code for trigger audio.
            //if((_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X) &&
            //    (u4PTS >= _rSwdmxMpgInfo.rVar.u4FirstVideoPTS))
            {
                BOOL fgOK = TRUE;
                if(_rSwdmxMpgInfo.rVar.fgEnableVideo)
                {
                    // DTV00140198 [MM]Can't play the video file smoothly after select it to play
                    // when fifo full and video are playing, we have to check current playback time
                    fgOK = FALSE;
                    if(_rSwdmxMpgInfo.rVar.u4SecondVideoPTS == 0xFFFFFFFF)
                    {
                        _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = (STC_GetStcValue() + 90000 + _rSwdmxMpgInfo.rVar.u4FirstPTS);
                        LOG(3, "_SWDMX_MpgWatchAudioPTS u4SecondVideoPTS 0x%x, Current time 0x%x\n",
                            _rSwdmxMpgInfo.rVar.u4SecondVideoPTS, STC_GetStcValue());
                    }
                    if(u4PTS > _rSwdmxMpgInfo.rVar.u4SecondVideoPTS)
                    {
                        fgOK = TRUE;
                    }
                    // DTV00141121, [DLNA] After repeatedly change audio stream, audio doesn't output.
                    // error handle, u4SecondVideoPTS is far from the u4Pts value
                    else if((_rSwdmxMpgInfo.rVar.u4SecondVideoPTS - u4PTS) > (UINT32)(2*90000))
                    {
                        fgOK = TRUE;
                        LOG(3, "_SWDMX_MpgWatchAudioPTS error handle, ignore u4SecondVideoPTS 0x%x\n",
                            _rSwdmxMpgInfo.rVar.u4SecondVideoPTS);
                    }
                }
                if(fgOK)
                {
                    _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                    _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                    _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;

                    _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                        ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
                    #ifdef TIME_SHIFT_SUPPORT
                    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    #endif
                    {
                        VDP_TriggerAudReceive(_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                        UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));

                        LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 1 pts 0x%x pos 0x%llx\n",
                            ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)),
                            _rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
                    }

                    /*
                    _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                    _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                    _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                    */
               }
            }
        }
        else if(!_rSwdmxMpgInfo.rFmt.fgVideo)
        {
            if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
            {
                _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                    ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                    //AUD_MMAoutEnable(0, TRUE);
                    LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                        ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)));

                    //STC_StartStc();
                    LOG(3, "_SWDMX_MpgWatchAudioPTS AUD_SetStartPts 2 pts 0x%x pos 0x%llx\n",
                        ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)), _u8SendedSize);
                }
                //_rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            }
        }

        #ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
        #endif
            // DTV00210035, two wrong audio PTS. We should not update duration
            //firstPTS=0x8ca9,LastPTS=0xcb139
            //audio PTS=0x3e4cc936
            //audio PTS=0x3e4cc936
            if(((u4PTS > _rSwdmxMpgInfo.rVar.u4LastPTS) && ((u4PTS - _rSwdmxMpgInfo.rVar.u4LastPTS) < (600*90000)))||
               ((u4PTS < _rSwdmxMpgInfo.rVar.u4LastPTS) && ((_rSwdmxMpgInfo.rVar.u4LastPTS - u4PTS) < (600*90000))))
            {
                UNUSED(_SWDMX_MpgWatchDuration(u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS));
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
                    ((u4PTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE),
                    (UINT32)_rSwdmxMpgInfo.rVar.u8PreviousSendedSize);
            }
        }
        #else
        LOG(6, "_SWDMX_MpgWatchAudioPTS u4Pts 0x%x\n", (u4PTS-_rSwdmxMpgInfo.rVar.u4FirstPTS));
        #endif
        _rSwdmxMpgInfo.rVar.u4LastAudioPTS = u4PTS;
        _rSwdmxMpgInfo.rVar.u4LastPTS = u4PTS;
    }

    if((_rSwdmxMpgInfo.rVar.fgGotVideo) && (!_rSwdmxMpgInfo.rVar.fgGotAudio))
    {
        // could be video fifo size

        if(!_fgGetInfo)
        {
            u4DmxAvailSize = (1024*1024);
            // playbacking, check AV size
            if(_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
            }
            if(_rSwdmxMpgInfo.rFmt.fgAudio)
            {
#ifdef CC_DMX_PURE_AUDIO
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    if(u4DmxAvailSize > DMX_MM_GetBufEmptySize(
                        _u1PidxAudio))
                    {
                        u4DmxAvailSize = DMX_MM_GetBufEmptySize(
                            _u1PidxAudio);
                    }
               	}
                else
#endif
                {
                    if(u4DmxAvailSize > DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO))
                    {
                        u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
                }
            }
            if(u4DmxAvailSize < _u4OneTimeSize)
            {
                _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                _rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

                // if there is no audio PTS in till now,
                // we use First video PTS as audio's PTS, but not the last one.
                //AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS);
                //_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                //    (_rSwdmxMpgInfo.rVar.u4FirstPTS + (MPG_STC_BASE));
                _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = (MPG_STC_BASE);
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                }

                if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
                   (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;

				    STC_StopStc();
                    // When there is video stream, stc value will set at vdec_flw.c
                    if(!_rSwdmxMpgInfo.rFmt.fgVideo)
                    {
                        u4Temp = STC_GetStcValue();
                        STC_SetStcValue((UINT32)((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                        u4Temp = STC_GetStcValue();

                        LOG(3, "_SWDMX_MpgWatchAudioPTS u4DmxAvailSize STC_SetStcValue 0x%x\n",
                            ((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE));
                        if (u4Temp != ((UINT32)((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + MPG_STC_BASE)))
                        {
                            LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        }
                    }
                }
                if(!_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    //Check in HandleFifoFull
                    //AUD_MMAoutEnable(0, TRUE);
                    //STC_StartStc();
                }
                else
                {
                    // if there is no audio PTS in till now,
                    // we use First video PTS as audio's PTS, but not the last one.
                    //VDP_TriggerAudReceive(_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS);
                    #ifdef TIME_SHIFT_SUPPORT
                    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    #endif
                    {
                        VDP_TriggerAudReceive(MPG_STC_BASE);
                        // do on vdec_flw.c
                        //STC_StartStc();
                    }
                }

                /*
                _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                _rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
                */
                //ASSERT(0);
                LOG(0, "_SWDMX_MpgWatchAudioPTS VDP_TriggerAudReceive due to fifo full");
            }
        }
    }
    else if(!_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
        {
            BOOL fgCheckAudFull=FALSE;
            if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
               (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
               (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
               (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT)||                 // [DTV00211805]
               (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS)||                 // for PS, TS file
               (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
               //||(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))    //DTV00086567, the first second audio can't output
            {
                u4DmxAvailSize = (1024*1024);
                // playbacking, check AV size
                if(_rSwdmxMpgInfo.rFmt.fgAudio)
                {
                    if(u4DmxAvailSize > DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO))
                    {
                        u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
                }
                if(u4DmxAvailSize < _u4OneTimeSize)         //DTV00212016 check audio fifo is full or not
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
                _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;

                // if there is no audio PTS in till now,
                // we use First video PTS as audio's PTS, but not the last one.
                //AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS);
                //_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                //    (_rSwdmxMpgInfo.rVar.u4FirstPTS + (MPG_STC_BASE));
                _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = (MPG_STC_BASE);
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                #endif
                {
                    UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                    //AUD_MMAoutEnable(0, TRUE);
                    LOG(3, "_SWDMX_MpgWatchAudioPTS no V AUD_SetStartPts pts 0x%x pos 0x%llx\n",
                           ((_rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE)), _u8SendedSize);
                    //_rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
                }

                if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
                   (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;

                    //LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n",
                        //(((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE))));
                    LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue 0x%x\n", _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);

                    STC_StopStc();
                    u4Temp = STC_GetStcValue();
                    STC_SetStcValue(_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                    u4Temp = STC_GetStcValue();

                    if (u4Temp != _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS)
                    {
                        LOG(3, "_SWDMX_MpgWatchAudioPTS STC_SetStcValue Fail\n");
                        //return FALSE;
                    }
                }
            }
            //STC_StartStc();
        }
    }

    _rSwdmxMpgInfo.rVar.u4LastAudioWptr = prPes->u4Wp;

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


BOOL _SWDMX_MpgAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType)
{
    if((eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_DECODE_DONE) ||
       (eAudioNotifyType == ENUM_SWDMX_AUDIO_NOTIFY_NO_DATA))
    {
        LOG(3, "audio is ready===========================\n");

        _rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

        /*
        if(_rSwdmxMpgInfo.rVar.fgVideoPause && (_rSwdmxMpgInfo.rVar.fgGotAudio || (!_rSwdmxMpgInfo.rFmt.fgAudio)))
        {
            LOG(5, "_SWDMX_MpgAudioCb _rSwdmxMpgInfo.rVar.fgVideoPause VDEC_Play\n");
            if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                VDEC_Play(ES0, VDEC_FMT_MPV);
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                VDEC_Play(ES0, VDEC_FMT_H264);
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                VDEC_Play(ES0, VDEC_FMT_WMV);
            }
            else
            {
                ASSERT(0);
            }
            _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        }
        else if(!_rSwdmxMpgInfo.rFmt.fgVideo) // audio only
        {
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
        }*/

        #ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        #endif
        {
            if((_rSwdmxMpgInfo.rVar.fgEnableVideo) &&
               (_rSwdmxMpgInfo.rVar.fgGotVideoReady) &&
               (_rSwdmxMpgInfo.rFmt.fgVideo))
            {
                // DTV00134760
                // In this case, because video fifo is full.
                // Video start to play without wait audio ready.
                // So after audio ready, notify aout in first audio PTS.
                LOG(3, "trigger audio again, PTS: 0x%x\n", _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
                VDP_TriggerAudReceive(_rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS);
            }
            //_rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
        }
        if(_rSwdmxMpgInfo.rVar.fgEnableAudio)
        {
            LOG(3, "re-enable audio\n");
            AUD_MMAoutEnable(0, TRUE);
        }
    }
    else
    {
        //impossible
        ASSERT(0);
    }
    return TRUE;
}

BOOL _SWDMX_MpgPcrCallBack(UINT32 u4PcrBase)
{
    if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
       (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
    {
        _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
        STC_StopStc();
        LOG(3, "_SWDMX_MpgPcrCallBack STC_SetStcValue 0x%x\n", (u4PcrBase));
        STC_SetStcValue((UINT32)(u4PcrBase));
        STC_StartStc();
    }
    return TRUE;
}

BOOL _SWDMX_MpgAudioCallback(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    DMX_AUDIO_PES_T rPes;
    #ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    UINT32 u4Tmp;
    SWDMX_MPG_MP3_HDR rMp3Hdr;
    UCHAR *pRptr;
    UINT32 au4BufStart[2], au4BufEnd[2];
    #endif

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgAudioCallback prPes NULL\n");
        return FALSE;
    }

    x_memset(&rPes, 0, sizeof(rPes));

    #ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    if((_rSwdmxMpgInfo.rFmt.fgAudio) &&
       ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)) &&
       (_rSwdmxMpgInfo.rFmt.eAudioType == ENUM_SWDMX_ADEC_MPEG) &&
       (!_rSwdmxMpgInfo.rVar.fgGotAudioReady)
      )
    {
#ifdef AUD_OLDFIFO_INTF
        VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                              &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
        VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
        if(_rSwdmxMpgInfo.rVar.u4LastAudioWptr)
        {
            pRptr = (UCHAR*)_rSwdmxMpgInfo.rVar.u4LastAudioWptr;
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
            _rSwdmxMpgInfo.rVar.u4Mp3Cnt ++;
            if(_rSwdmxMpgInfo.rVar.u4Mp3Cnt >= MPG_MP3_CHECK_CNT)
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
    if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
        (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2))
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgAudioCallbackParse(prPes);
        }
    }
    else if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
        (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264))
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgAudioCallbackParse(prPes);
        }
    }
    #endif

    if(_rSwdmxMpgInfo.rFmt.fgVideo)
    {
#ifdef SWDMX_MPG_MPEG2_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                return _SWDMX_MpgAudioCallbackParse(prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                return _SWDMX_MpgAudioCallbackParse(prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
        {
            if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                return _SWDMX_MpgAudioCallbackParse(prPes);
            }
        }
        else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
            {
                // to parse first sequence header
                return _SWDMX_MpgAudioCallbackParse(prPes);
            }
        }
        else
#endif
        {
        }
    }

    x_memcpy(&rPes, prPes, sizeof(DMX_AUDIO_PES_T));

    if(_rSwdmxMpgInfo.rVar.u4LastAudioWptr)
    {
        // for the first time, _rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0,
        // we use original write pointer.
        rPes.u4Wp = _rSwdmxMpgInfo.rVar.u4LastAudioWptr;
    }

    UNUSED(_SWDMX_MpgWatchAudioPTS(prPes));

    if(_rSwdmxMpgInfo.rVar.fgNotToADec)
    {
        //_SWDMX_MpgFlushAFifo();
        _rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
    {
        //_SWDMX_MpgFlushAFifo();
        _rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        _SWDMX_MpgFlushVFifo();
        return TRUE;
    }

    if((!_rSwdmxMpgInfo.rVar.fgGotVideo) && (_rSwdmxMpgInfo.rFmt.fgVideo))
    {
        //_SWDMX_MpgFlushAFifo();
        //if(_u1PidxAudio != 0xFF)
        //{
        //    UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        //}
        _rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
    {
        //_SWDMX_MpgFlushAFifo();
        //if(_u1PidxAudio != 0xFF)
        //{
        //    UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        //}
        _rSwdmxMpgInfo.rVar.fgCleanAudio = TRUE;
        return TRUE;
    }

    // On cleaning
    if(_rSwdmxMpgInfo.rVar.fgCleanAudio)
    {
        return TRUE;
    }

    if(!_rSwdmxMpgInfo.rFmt.fgAudio)
    {
        return TRUE;
    }

    if(_rSwdmxMpgInfo.rVar.fgSendAudioEos)
    {
        return TRUE;
    }

    #ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPes.u4TickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;
    }
    #endif

    if(rPes.u4Dts > 0)
    {
        rPes.u4Dts -= _rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPes.u4Dts += (MPG_STC_BASE);
    }
    if(rPes.u4Pts > 0)
    {
        rPes.u4Pts -= _rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPes.u4Pts += (MPG_STC_BASE);
    }
    else
    {
        // DTV00135223, audio driver will ignore zero pts now.
        // DTV00209856, audio PTS is zero sometimes. Send the last one.
        //if(_rSwdmxMpgInfo.rVar.u4LastAudioPTS > 0)
        //{
        //    rPes.u4Pts = ((_rSwdmxMpgInfo.rVar.u4LastAudioPTS -
        //        _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
        //}
        //LOG(6, "_SWDMX_MpgAudioCallback rPes.u4Pts = 0 -> %d\n", rPes.u4Pts);
    }
    if(prPes->fgEOS)
    {
        // DTV00065338, [Acceptance test] Playback Time length NOT equal in Playback Screen and File Information.
        // because audio driver does not get the lastest Wptr,
        // add this to set the last one.
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            DMX_AUDIO_PES_T rPes1Last;
            x_memset(&rPes1Last, 0, sizeof(rPes1Last));
            if(_rSwdmxMpgInfo.rVar.u4TotalTime)
            {
                rPes1Last.u4Dts =  _rSwdmxMpgInfo.rVar.u4TotalTime + (MPG_STC_BASE);
                rPes1Last.u4Pts =  _rSwdmxMpgInfo.rVar.u4TotalTime + (MPG_STC_BASE);
            }
            rPes1Last.u4Wp =  rPes.u4Wp;
            UNUSED(AUD_SendAudioPes(&rPes1Last));
        }

        // for the first time, _rSwdmxMpgInfo.rVar.u4LastAudioWptr = xx,
        // but we use original write pointer.
        LOG(3, "_SWDMX_MpgAudioCallback send EOS\n");
        rPes.u4Wp = prPes->u4Wp;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;
    }
    fgRet = AUD_SendAudioPes(&rPes);
    if(prPes->fgEOS)
    {
        AUD_MM_Set_Avsync_event(AUD_DEC_MAIN);
    }
    return fgRet;
}

BOOL _SWDMX_MpgVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rPesInfo;
    UINT32 u4Addr;
    UINT32 u4SwdmxPicType = SWDMX_PIC_TYPE_OTHER;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallback prPes NULL\n");
        return FALSE;
    }

#ifdef SWDMX_MPG_MPEG2_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackMpgParse(prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_H264_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackH264Parse(prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            //_fgSendSeqHdr = TRUE;
            return _SWDMX_MpgVideoCallbackMpeg4Parse(prPes);
        }
    }
    else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        if((!_fgSendSeqHdr) || (_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            // to parse first sequence header
            return _SWDMX_MpgVideoCallbackVc1Parse(prPes);
        }
    }
    else
#endif
    {
        LOG(0, "SwdmxMpg Not Support\n");
    }


    UNUSED(_SWDMX_MpgWatchVideoPTS(prPes));

    if(_rSwdmxMpgInfo.rVar.fgNotToADec)
    {
        _SWDMX_MpgFlushAFifo();
    }

    if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
    {
        _SWDMX_MpgFlushAFifo();
        _SWDMX_MpgFlushVFifo();
        return TRUE;
    }

    if(!_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        return TRUE;
    }

    if(_rSwdmxMpgInfo.rVar.fgSendVideoEos)
    {
        return TRUE;
    }

    // ilii, markout
    /*
    if(_rSwdmxMpgInfo.rVar.fgVideoPause && (_rSwdmxMpgInfo.rVar.fgGotAudio || (!_rSwdmxMpgInfo.rFmt.fgAudio))
        && _rSwdmxMpgInfo.rVar.fgGotAudioReady)
    {
        LOG(5, "_SWDMX_MpgVideoCallback fgVideoPause VDEC_Play\n");
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            VDEC_Play(ES0, VDEC_FMT_MPV);
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            VDEC_Play(ES0, VDEC_FMT_H264);
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            VDEC_Play(ES0, VDEC_FMT_WMV);
        }
        else
        {
            ASSERT(0);
        }
        _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
    }
    */

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

    /*
    if((_prSwdmxInfo->i4PlaySpeed > 0)
        && (_prSwdmxInfo->i4PlaySpeed < 4000))
    {
        if(!_rSwdmxMpgInfo.rVar.fgGotVideo)
        {
            if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(prPes->u4FrameType != PIC_TYPE_I)
                {
                    _SWDMX_MpgFlushVFifo();
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
	//  This part of code has been move down ,  DTV00079400  there is a case  in the 00:00:55, pts=0, and fgPtsDts=true,  	
    // when jumping to the point , pts will wrong,
    //DTV00139582.. if (fgPtsDts) ,PTS=0 is also valid used as  first video PTS.
    #if 0
    if((!_rSwdmxMpgInfo.rVar.fgGotVideo)&&(prPes->u4Pts==0)&&(prPes->fgPtsDts))
    {
        rPesInfo.u4PTS= (MPG_STC_BASE);
    }
    #endif 
	
    if(rPesInfo.u4PTS > 0)
    {
        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        //LOG(5, "_SWDMX_MpgVideoCallback Org PTS %d\n", rPesInfo.u4PTS);
        rPesInfo.u4PTS -= _rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPesInfo.u4PTS += (MPG_STC_BASE);

        if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS != (UINT32)0xFFFFFFFF)
        {
            if((prPes->u4Pts < _rSwdmxMpgInfo.rVar.u4FirstVideoPTS) &&
                ((_rSwdmxMpgInfo.rVar.u4FirstVideoPTS - prPes->u4Pts) > (10000))) // 1 sec
            {
                // DTV00209729, first video pts is a fake one.
                // we would not send audio data with PTS small than this first fake video pts.

                // u4FirstVideoPTS is used to output audio after this video pts
                // even we assign a smaller PTS that actually is a fake one,
                // we would also not affect the time after we start to output audio
                _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = prPes->u4Pts;
            }
        }
    }
    if(rPesInfo.u4DTS > 0)
    {
        rPesInfo.u4DTS -= _rSwdmxMpgInfo.rVar.u4FirstPTS;
        rPesInfo.u4DTS += (MPG_STC_BASE);
    }
	
    //DTV00139582.. if (fgPtsDts) ,PTS=0 is also valid used as  first video PTS.
    if((!_rSwdmxMpgInfo.rVar.fgGotVideo)&&(prPes->u4Pts==0)&&(prPes->fgPtsDts))
    {
	    _rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        rPesInfo.u4PTS = (MPG_STC_BASE);
		rPesInfo.u4DTS = (MPG_STC_BASE);
		
    }
	
    rPesInfo.u4FifoStart = prPes->u4BufStart;
    rPesInfo.u4FifoEnd = prPes->u4BufEnd;
    rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
    rPesInfo.fgDtsValid = prPes->fgPtsDts;
    rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
    rPesInfo.fgGop = prPes->fgGop;
    rPesInfo.fgEos = prPes->fgEOS;
    //rPesInfo.fgNotDisplay = u4Pts < u4SeekTime;
    rPesInfo.u4TotalTimeOffset = _rSwdmxMpgInfo.rVar.u4TotalTimeOffset;
    rPesInfo.u8Offset = _rSwdmxMpgInfo.rVar.u8PreviousSendedSize;
    rPesInfo.u8OffsetI = _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound;
    rPesInfo.u2DecodingOrder = _rSwdmxMpgInfo.rVar.u2OrderFromIPos;
    #ifdef CC_VDEC_FMT_DETECT
    rPesInfo.u4DmxFrameType = prPes->u4FrameType;
    #endif

    #ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        rPesInfo.u4TickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;
    }
    #endif

    _rSwdmxMpgInfo.rVar.u2OrderFromIPos++;

    // in es file, we never get pts.
    // so assume we already found pts.
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
    {
        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
        if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
           (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        {
            _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
            STC_StopStc();
            // When there is video stream, stc value will set at vdec_flw.c

        }
    }

#ifdef SWDMX_MPG_MPEG2_SUPPORT
    if (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
        switch (prPes->u4FrameType)
        {
        case PIC_TYPE_I:
            rPesInfo.ucPicType = MPV_I_TYPE;
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                _rSwdmxMpgInfo.rVar.u8PreviousIPos);
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
            if((_rSwdmxMpgInfo.rVar.u4TotalTime) > 90000)
            {
                u4Tmp = (_rSwdmxMpgInfo.rVar.u4TotalTime / 90000); // total frames
                //u4Tmp *= 3;  wrong -> u8Size *= 3;
                u8Size = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);
                u8Size *= 3;
            }
            else
            {
                u8Size = (UINT64)VDEC_GetAvgPicSize(0);
                u8Size *= 90;
            }
            if((_rSwdmxMpgInfo.rVar.u8PreviousSendedSize
                - _rSwdmxMpgInfo.rVar.u8PreviousIPos) > u8Size)
            {
                u4SwdmxPicType = SWDMX_PIC_TYPE_NO_I;
            }
        }
    }
    else
#endif
#ifdef SWDMX_MPG_VC1_SUPPORT
    if (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;

        switch (prPes->u4FrameType)
        {
        case VC1_PIC_TYPE_I:
            u4SwdmxPicType = SWDMX_PIC_TYPE_I;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                _rSwdmxMpgInfo.rVar.u8PreviousIPos);
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
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
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
            if((_rSwdmxMpgInfo.rVar.u4TotalTime) > 90000)
            {
                u4Tmp = (_rSwdmxMpgInfo.rVar.u4TotalTime / 90000); // total frames
                u4Tmp *= 3;
                u8Size = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);
            }
            else
            {
                u8Size = (UINT64)VDEC_GetAvgPicSize(0);
                u8Size *= 90;
            }
            if((_rSwdmxMpgInfo.rVar.u8PreviousSendedSize
                - _rSwdmxMpgInfo.rVar.u8PreviousIPos) > u8Size)
            {
                u4SwdmxPicType = SWDMX_PIC_TYPE_NO_I;
            }
        }

        LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
            _rSwdmxMpgInfo.rVar.u8PreviousIPos);
    }
    else
#endif
#ifdef SWDMX_MPG_MPEG4_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
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
                    LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPos 0x%llx\n",
                        _rSwdmxMpgInfo.rVar.u8PreviousIPos);
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
        }
        else if ((pucBuf[0] & 0x20) == 0x20)
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
        (_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)))
    {
        _rSwdmxMpgInfo.rVar.u8PreviousIPos = _rSwdmxMpgInfo.rVar.u8PreviousSendedSize;
        _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound =
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound;
        if(_rSwdmxMpgInfo.rVar.fgVideoPTSFound && !_rSwdmxMpgInfo.rVar.fgSendVideoEos)
        {
            //if((!_rSwdmxMpgInfo.rVar.fgGotVideo) && (_rSwdmxMpgInfo.rFmt.fgAudio))
            if((!_rSwdmxMpgInfo.rVar.fgGotVideo) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
               (
                ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                 (_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                 (_rSwdmxMpgInfo.rVar.i4CurSpeed <= 1500)
                )
                ||
                ((_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)&&
                 (_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                 (_rSwdmxMpgInfo.rVar.i4CurSpeed < 4000)
                )
               )
#else
               ((_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
                (_rSwdmxMpgInfo.rVar.i4CurSpeed < 4000))
#endif
              )
            {
                // In time-shift, we only pause vdp, but not vdec
                if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if(!_rSwdmxMpgInfo.rVar.fgVideoPause)
                    {
                        UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
                        VDEC_Pause(ES0);
                        LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                        _rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                        _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                    }
                }
                if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                {
                    if(prPes->u4Pts==0)          //DTV00139313
                    {
                        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS=_rSwdmxMpgInfo.rVar.u4LastVideoPTS;
                        LOG(5,"set FirstVideoPTS=u4LastPTS=0x%x STC(0x%x)\n",_rSwdmxMpgInfo.rVar.u4LastVideoPTS, STC_GetStcValue());

                    }
                    else
                    {
                        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                        LOG(5,"set FirstVideoPTS= First I pic Pts=0x%x STC(0x%x)\n",prPes->u4Pts, STC_GetStcValue());
                    }
                }
                LOG(5, "_SWDMX_MpgVideoCallback fgGotVideo\n");
                _rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxMpgInfo.rVar.u4TotalTime));
                UNUSED(VDEC_SetFileSize(ES0, _u8FileSize));
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    UNUSED(VDEC_SetLastTime(ES0, MPG_STC_BASE));
                }
            }
            else if(!_rSwdmxMpgInfo.rVar.fgGotVideo)
            {
                if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                {
                    _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                }
                _rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxMpgInfo.rVar.u4TotalTime));
                UNUSED(VDEC_SetFileSize(ES0, _u8FileSize));
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    UNUSED(VDEC_SetLastTime(ES0, MPG_STC_BASE));
                }
            }
            _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxMpgInfo.rVar.u8PreviousIPos;
            _rSwdmxMpgInfo.rVar.u2OrderFromIPos = 0;
            LOG(6, "_SWDMX_MpgVideoCallback u8PreviousIPosWithPTSFound 0x%llx\n",
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound);

            #ifdef SWDMX_MPG_USE_INDEX
            if(_rSwdmxMpgInfo.rVar.fgUseIdx)
            {
                UNUSED(_SWDMX_MpgIdxUpdate(rPesInfo.u4PTS,
                    _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound));
            }
            #endif
        }
    }
    else if(u4SwdmxPicType == SWDMX_PIC_TYPE_P)
    {
        _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound =
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound;
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
    if(!_rSwdmxMpgInfo.rVar.fgSendVideoEos)
    {
        //if((!_rSwdmxMpgInfo.rVar.fgGotVideo) && (_rSwdmxMpgInfo.rFmt.fgAudio))
        // In time-shift, we only pause vdp, but not vdec
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((!_rSwdmxMpgInfo.rVar.fgGotVideo) &&
               (!_rSwdmxMpgInfo.rVar.fgVideoPause) &&
                ((_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0)
                   && (_rSwdmxMpgInfo.rVar.i4CurSpeed < 4000)))
            {
                VDEC_HDR_INFO_T rHdrInfo;
                x_memset(&rHdrInfo, 0, sizeof(rHdrInfo));
                UNUSED(VDEC_QueryInfo(ES0, &rHdrInfo));
                if(rHdrInfo.u4DisplayQPicCnt > 0)
                {
                    // DTV00136474 [MM]No audio ouput when play the special ts file.
                    // When all picture is P/B frame, we have to know this information from vdec
                    if((_rSwdmxMpgInfo.rVar.i4CurSpeed > 0)
                       &&
                       (_rSwdmxMpgInfo.rVar.i4CurSpeed < 4000))
                    {
                        // In time-shift, we only pause vdp, but not vdec
                        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                        {
                            if(!_rSwdmxMpgInfo.rVar.fgVideoPause)
                            {
                                UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
                                VDEC_Pause(ES0);
                                LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                                _rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                                _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                            }
                        }
                        if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            if(prPes->u4Pts==0)          //DTV00139313
                            {
                                _rSwdmxMpgInfo.rVar.u4FirstVideoPTS=_rSwdmxMpgInfo.rVar.u4LastVideoPTS;
                                LOG(5,"set FirstVideoPTS=u4LastPTS=0x%x\n",_rSwdmxMpgInfo.rVar.u4LastVideoPTS);

                            }
                            else
                            {
                                _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                                LOG(5,"set FirstVideoPTS= First I pic Pts=0x%x\n",prPes->u4Pts);
                            }
                        }
                        LOG(5, "_SWDMX_MpgVideoCallback fgGotVideo\n");
                        _rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                        UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxMpgInfo.rVar.u4TotalTime));
                        UNUSED(VDEC_SetFileSize(ES0, _u8FileSize));
                        if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                        {
                            UNUSED(VDEC_SetLastTime(ES0, MPG_STC_BASE));
                        }
                    }
                    else //if(!_rSwdmxMpgInfo.rVar.fgGotVideo)
                    {
                        if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = (prPes->u4Pts);
                        }
                        _rSwdmxMpgInfo.rVar.fgGotVideo = TRUE;
                        UNUSED(VDEC_SetTotalTime(ES0, _rSwdmxMpgInfo.rVar.u4TotalTime));
                        UNUSED(VDEC_SetFileSize(ES0, _u8FileSize));
                        if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                        {
                            UNUSED(VDEC_SetLastTime(ES0, MPG_STC_BASE));
                        }
                    }
                }
                else
                {
                    UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
                    VDEC_Pause(ES0);
                    LOG(5, "_SWDMX_MpgVideoCallback VDEC_Pause\n");
                    _rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
                }
            }
        }
    }

    // in es file, we never get pts.
    // so assume we already found pts.
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) &&
         ((rPesInfo.ucPicType & 0x1F) <= 0x5))
#ifdef SWDMX_MPG_VC1_SUPPORT
        ||
        ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) &&
         (rPesInfo.ucPicType >= VC1_PIC_TYPE_I) &&
         (rPesInfo.ucPicType <= VC1_PIC_TYPE_SKIP))
#endif
      )
    {
        if(_rSwdmxMpgInfo.rVar.u4FirstPTS == 0)
        {
            _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
            _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            rPesInfo.fgDtsValid = TRUE;
            rPesInfo.u4DTS = MPG_STC_BASE;
            rPesInfo.u4PTS = MPG_STC_BASE;

            LOG(5, "_SWDMX_MpgVideoCallback u4FirstPTS 0x%x\n", _rSwdmxMpgInfo.rVar.u4FirstPTS);
        }

        if(_rSwdmxMpgInfo.rVar.fgJumpping)
        {
            UINT32 u4PTS;
            UINT64 u8Tmp;
            UINT64 u8Remainder;

            // to make PTS increament, use file position to calculate a fake one
            u8Tmp = VDEC_GetAvgPicSize(0);
            // 90000 / 30 fps = 3000
            u4PTS = (UINT32)_SWDMX_Div6464((_u8SendedSize * (UINT64)3000), u8Tmp, &u8Remainder);

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
        if(_rSwdmxMpgInfo.rVar.fgJumpping)
        {
            if(!rPesInfo.fgDtsValid)
            {
                rPesInfo.fgDtsValid = TRUE;
                rPesInfo.u4DTS = ((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS)
                                  + (MPG_STC_BASE));
                rPesInfo.u4PTS = rPesInfo.u4DTS;
            }
        }
    }

    if(prPes->fgEOS)
    {
        LOG(3, "_SWDMX_MpgVideoCallback send EOS\n");
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
    }

    UNUSED(VDEC_SendEs((void*)&rPesInfo));

    UNUSED(u4Addr);
    return TRUE;
}


BOOL _SWDMX_MpgAudioCallbackParse(const DMX_AUDIO_PES_T* prPes)
{
    BOOL fgRet;
    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgAudioCallbackParse prPes NULL\n");
        return FALSE;
    }
    if(_u1PidxAudio != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxAudio, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgAudioCallbackParse DMX_UpdateReadPointer Fail\n");
        }
    }

    return TRUE;
}

BOOL _SWDMX_MpgVideoCallbackMpgParse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    //UCHAR ucByte0, ucByte1, ucByte2, ucByte3;
    //UINT16 u2QueueSize;
    //UINT16 u2MaxQueueSize;
    //UINT32 u4Tmp;
    VDEC_PES_INFO_T rPesInfo;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallbackMpgParse prPes NULL\n");
        return FALSE;
    }

    _rSwdmxMpgInfo.rVar.u4SeqHdrChk++;
    if( _rSwdmxMpgInfo.rVar.u4SeqHdrChk>256)         // define the check size is  "256"  more than  temporal reference
    {
        _rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;
        VDEC_Notify(ES0,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
        LOG(3,"there is no sequence header has be find\n");

        return TRUE;
    }

    if(prPes->fgSeqHeader)
    {
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        _rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(ES0, VDEC_FMT_MPV, &_rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
        // in getting sequence, we should not send pts to vdec
        // or we may get wrong start pts on vdec.
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
        fgRet = VDEC_PrsSeqHdr(VDEC_FMT_MPV, &rPesInfo, &_rSeqInfo);
        if(fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackMpgParse send sequence\n");
            _fgSendSeqHdr = TRUE;
        }

        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(ES0, _rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        return TRUE;
    }

    if(_u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackMpgParse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}


#ifdef SWDMX_MPG_VC1_SUPPORT
BOOL _SWDMX_MpgVideoCallbackVc1Parse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    //UCHAR ucByte0, ucByte1, ucByte2, ucByte3;
    VDEC_PES_INFO_T rPesInfo;

    if(prPes == NULL)
    {
        LOG(1, "_SWDMX_MpgVideoCallbackMpgParse prPes NULL\n");
        return FALSE;
    }

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));
    if(prPes->u4FrameType == VC1_PIC_TYPE_SEQ_START)
    {
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(ES0, VDEC_FMT_WMV, &_rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        // in getting sequence, we should not send pts to vdec
        // or we may get wrong start pts on vdec.
        rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
        rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = TRUE;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
        fgRet = VDEC_PrsSeqHdr(VDEC_FMT_WMV, &rPesInfo, &_rSeqInfo);
        if(fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse send sequence\n");
            _fgSendSeqHdrVc1 = TRUE;
        }

        //UNUSED(VDEC_SetGetSeqHdrOnly(ES0, FALSE));

        return TRUE;
    }
    else if(prPes->u4FrameType == VC1_PIC_TYPE_ENTRY)
    {
        if(_fgSendSeqHdrVc1 == TRUE)
        {
            //UNUSED(VDEC_SetGetSeqHdrOnly(ES0, TRUE));

            // in getting sequence, we should not send pts to vdec
            // or we may get wrong start pts on vdec.
            rPesInfo.u4VideoType = (UINT32)DMX_VIDEO_VC1;
            rPesInfo.ucPicType = (UCHAR)prPes->u4FrameType;
            rPesInfo.u4FifoStart = prPes->u4BufStart;
            rPesInfo.u4FifoEnd = prPes->u4BufEnd;
            rPesInfo.u4VldReadPtr = prPes->u4FrameAddr;
            rPesInfo.fgDtsValid = prPes->fgPtsDts;
            rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
            rPesInfo.fgGop = prPes->fgGop;
            x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
            fgRet = VDEC_PrsSeqHdr(VDEC_FMT_WMV, &rPesInfo, &_rSeqInfo);
            if(fgRet)
            {
                LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse send sequence\n");
                _fgSendSeqHdr = TRUE;
            }

            UNUSED(VDEC_SetGetSeqHdrOnlyEnd(ES0, _rSwdmxMpgInfo.rVar.fgVdecPowerOff));
            _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

            return TRUE;
        }
    }

    if(_u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackVc1Parse DMX_UpdateReadPointer Fail\n");
        }
    }

    return TRUE;
}
#endif


#ifdef SWDMX_MPG_MPEG4_SUPPORT
BOOL _SWDMX_MpgVideoCallbackMpeg4Parse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UCHAR *pucBuf;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;

    x_memset(&rPesInfo, 0, sizeof(VDEC_PES_INFO_T));

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
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(ES0, VDEC_FMT_MP4, &_rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
        fgRet = VDEC_PrsSeqHdr(VDEC_FMT_MP4, &rPesInfo, &_rSeqInfo);
        if(fgRet)
        {
            _fgSendSeqHdr = TRUE;
        }
        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(ES0, _rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
    }

    return TRUE;
}
#endif

BOOL _SWDMX_MpgVideoCallbackH264Parse(const DMX_PES_MSG_T* prPes)
{
    BOOL fgRet;
    UINT32 u4Addr;
    VDEC_PES_INFO_T rPesInfo;

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
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;
        UNUSED(VDEC_SetGetSeqHdrOnlyBegin(ES0, VDEC_FMT_H264, &_rSwdmxMpgInfo.rVar.fgVdecPowerOff));

        rPesInfo.u4FifoStart = prPes->u4BufStart;
        rPesInfo.u4FifoEnd = prPes->u4BufEnd;
        rPesInfo.fgDtsValid = prPes->fgPtsDts;
        rPesInfo.fgSeqHdr = prPes->fgSeqHeader;
        rPesInfo.fgGop = prPes->fgGop;
        x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
        fgRet = VDEC_PrsSeqHdr(VDEC_FMT_H264, &rPesInfo, &_rSeqInfo);
        if(fgRet)
        {
            //LOG(5, "_SWDMX_MpgVideoCallbackH264Parse rSeqInfo %dX%d\n",
            //    rSeqInfo.u2_width, rSeqInfo.u2_height);
            _fgSendSeqHdr = TRUE;
        }

        UNUSED(VDEC_SetGetSeqHdrOnlyEnd(ES0, _rSwdmxMpgInfo.rVar.fgVdecPowerOff));
        _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

        return TRUE;
    }

    if(_u1PidxVideo != 0xFF)
    {
        fgRet = DMX_UpdateReadPointer(_u1PidxVideo, prPes->u4Wp, prPes->u4Wp);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgVideoCallbackH264Parse DMX_UpdateReadPointer Fail\n");
        }
    }
    return TRUE;
}

VOID _SWDMX_MpgSeekFinNotify(UINT32 u4PTS, BOOL fgAgain)
{
    LOG(6, "_SWDMX_MpgSeekFinNotify\n");
    //VERIFY (x_sema_unlock(_hPlayMgrSemaFinSeek) == OSR_OK);
    _rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
    if(fgAgain)
    {
        _rSwdmxMpgInfo.rVar.u2JumpAgainCount++;
    }
    else
    {
        _rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
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

        i4Ret = x_msg_q_send(_hFindMsgQ, (VOID *)&u4DetectFin, sizeof(UINT32), 255);

        if(i4Ret != OSR_OK)
        {
            return FALSE;
        }
    }
    return TRUE;
}
#endif

BOOL _SWDMX_MpgResetFeeder(VOID)
{
    UINT32 u4ReqCnt;
    INT32 i4Ret;
    UINT32 u4AlignSize;
    FEEDER_TRANSMIT_COND rFeederCond;

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }

    #ifdef SWDMX_MPG_DEBUG_BUFFER
    if((_rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0) || (_rSwdmxMpgInfo.rBuf.u4ReqTimes > 0))
    {
        LOG(7, "Mpg Fd Flh UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d UAdr %d\n",
            _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
            _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
            _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
            _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
            _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
            _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
            _rSwdmxMpgInfo.rBuf.u4ReqTimes,
            _rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    }
    #endif

    _u4FeederReqID += 50; // always a even value
    _rSwdmxMpgInfo.rBuf.u4BufAvailCnt = 0;
    _rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4BufTotalSize = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTimes = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;
    _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    _u8ReadedPos = _u8SendedSize;

    for(u4ReqCnt = 0;u4ReqCnt < _u4FeederReqNum;u4ReqCnt++)
    {
        // make it invalid, not a even value
        _rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt] = 1;
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        _rSwdmxMpgInfo.rBuf.au4ReqDup[u4ReqCnt] = 0;
#endif
    }

    #if 1//def CC_DLNA_SUPPORT
    FeederSetReadyThreshold(MPG_DLNA_FEEDER_UNREACH);
    FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNREACH);
    FeederSetInfo(FEEDER_SWDMX_THRESHOLD, FALSE);
    #endif
    // Update read ptr to feeder
    if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else
    {
        if(_rFeederInfo.u4WriteAddr)
        {
            if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rFeederInfo.u4WriteAddr) != FEEDER_E_OK)
            {
                LOG(1, "_SWDMX_MpgResetFeeder FeederUpdateReadPointer fail\n");
            }
        }
    }
    // DDI is 4 byte alignment
    u4AlignSize = (_rFeederInfo.u4WriteAddr % 4);
    if(u4AlignSize > 0)
    {
        LOG(5, "Mpg Fd align _rFeederInfo.u4WriteAddr %d\n", _rFeederInfo.u4WriteAddr);
        i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                (4-u4AlignSize),
                (UINT64)(0),
                &rFeederCond,
                (_u4FeederReqID-25)); // 25 is a past requestID
        if(i4Ret != FEEDER_E_OK)
        {
            LOG(1, "Mpg Fd align FeederSetRequest Fail\n");
            /* not necessary
            _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
            if(!_fgGetInfo)
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

VOID _SWDMX_MpgResetData()
{
    BOOL fgRet;
    UINT32 i;
    _SWDMX_MpgFlushAFifo();
    _SWDMX_MpgFlushVFifo();
    if(_u1PidxAudio != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
    }
    if(_u1PidxVideo != 0xFF)
    {
        UNUSED(DMX_MM_FlushBuffer(_u1PidxVideo));
    }
    _rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    //if(_prSwdmxInfo->fgEnVideoDmx)
    //{
    //    SWDMX_SetInfo((UINT8)eSWDMX_DISABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //    SWDMX_SetInfo((UINT8)eSWDMX_ENABLE_STREAM, (UINT32)eSWDMX_STRM_TYPE_VID, 0, 0);
    //}
    //_u4BufStart = 0;
    _SWDMX_FlushReqMsgQ();
    UNUSED(_SWDMX_MpgResetFeeder());
    // This might put before _SWDMX_MpgFlushAFifo, DMX_MM_FlushBuffer
    // because write might meet read pointer, and DDI is in full status,
    // then _SWDMX_DMX_Reset will cause DMX in strange status.
    // so DMX_DDI_MoveData can't return when next call DMX_DDI_MoveData.
    if (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        fgRet = _SWDMX_DMX_Reset();//DMX_DDI_RequestReset();
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgResetData _SWDMX_DMX_Reset Fail\n");
        }
    }
    //#define DMXCMD_READ321(offset)            IO_READ32((IO_VIRT + 0x17000), ((offset) * 4))
    //LOG(3, "_SWDMX_MpgResetData DMXCMD 0x%x\n", DMXCMD_READ321(26));

    // For es file, DMX_DDI_RequestReset will reset the value set
    // by DMX_ES_SetFileLength. So set it again.
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
    {
        if(_u1PidxVideo != 0xFF)
        {
            fgRet = DMX_ES_SetFileLength(_u1PidxVideo,(UINT32)0xFFFFFFFF);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgResetData _DMX_ES_SetFileLength Fail\n");
            }
        }
    }


    _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    // DTV00211818, mp3 audio es
    // pause and seek, then play
    // we will have fgSendEOF = 1, clean it.
    _rSwdmxMpgInfo.rVar.fgSendEOF = 0;
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = 0;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = 0;
    //_u4AvailSize = 0;

    // DTV00212560, [5387-FLIP][MM]Total time change after time search to the beginning with special file
    // when change position, we should clean LastPTS data.
    _rSwdmxMpgInfo.rVar.u4LastPTSCnt = 0;
    for(i=0;i<MPG_PTS_CHECK_NUM;i++)
    {
        _rSwdmxMpgInfo.rVar.au4LastPTS[i] = 0;
    }

#ifdef TIME_SHIFT_SUPPORT
    _rSwdmxMpgInfo.rVar.u4QueueTickNum = 0xFFFFFFFF;
    for(i=0;i<MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER;i++)
    {
        _rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] = (0);
    }
#endif

    //DTV00135555 [MM]TV stop at current video file after excute play and FF
    //DTV00210492 when do seek or trick after sended EOS, RangeStatus should set NOT_FINISHED
    if(_rSwdmxMpgInfo.rVar.prCurRangeReq)
    {
        _rSwdmxMpgInfo.rVar.prCurRangeReq->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }
}

VOID _SWDMX_MpgResetVariable()
{
    UINT32 i;
    _SWDMX_MpgResetData();
    //_SWDMX_FlushDataMsgQ();

    #if 0 // move to ResetData
    _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
    #endif
#if 1//def CC_DLNA_SUPPORT
    FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
    _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
    //_rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;
    _rSwdmxMpgInfo.rVar.fgNotToDecoder = FALSE;
    _rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;

    _rSwdmxMpgInfo.rVar.fgSetStc = FALSE;
    _rSwdmxMpgInfo.rVar.u4FirstPTS = 0;
    _rSwdmxMpgInfo.rVar.u4LastPTS = 0;
    _rSwdmxMpgInfo.rVar.u4LastPTSCnt = 0;
    for(i=0;i<MPG_PTS_CHECK_NUM;i++)
    {
        _rSwdmxMpgInfo.rVar.au4LastPTS[i] = 0;
    }
    _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
    _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
    _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
    _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
    _rSwdmxMpgInfo.rVar.u4LastVideoPTS = 0;                          //DTV00139313
    _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
    _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
    _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
    _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
    _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
    _rSwdmxMpgInfo.rVar.fgNeedAdjust = FALSE;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
    _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
    _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
    _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
    _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
    _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
    _rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
    _rSwdmxMpgInfo.rVar.u4PicCnt = 0;
    //_rSwdmxMpgInfo.rVar.u4TotalTime = 0;
    _rSwdmxMpgInfo.rVar.u4TotalTimeOffset = 0;
    _rSwdmxMpgInfo.rVar.u4CurIndexNum = 0;

    _rSwdmxMpgInfo.rVar.u4LastVideoWptr = 0;
    _rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
    _rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize = 0;

    _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
    _rSwdmxMpgInfo.rVar.u8PreviousIPos = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = 0;
    _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
    _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    _rSwdmxMpgInfo.rVar.u4SeqHdrChk=0;

    _rSwdmxMpgInfo.rVar.u8OffsetKeep = 0;
    _rSwdmxMpgInfo.rVar.u8OffsetIKeep = 0;
    _rSwdmxMpgInfo.rVar.i4TempRefKeep = 0;
    _rSwdmxMpgInfo.rVar.u2DecodingOrderKeep = 0;

    #if 1//def CC_DLNA_SUPPORT
    _rSwdmxMpgInfo.rBuf.fgGetEof = FALSE;
    _rSwdmxMpgInfo.rBuf.fgLastData = FALSE;
    #endif
}

VOID _SWDMX_MpgJumpToNormal(BOOL fgSeek, BOOL fgJumpping, INT32 i4Speed)
{
    BOOL fgRet;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;
    VDP_PIC_INFO_T rPicInfo;
    BOOL fgNoI;

    x_memset(&rPicInfo, 0, sizeof(VDP_PIC_INFO_T));
    if(fgJumpping || fgSeek)
    {
        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;

        if(_rSwdmxMpgInfo.rFmt.fgVideo)
        {
            if(!fgSeek)
            {
                u8Offset = 0;
                u8OffsetI = 0;
                u2DecodingOrder = 0;
                i4TempRef = 0;
                fgNoI = FALSE;
                #ifdef TIME_SHIFT_SUPPORT
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                   ((_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL) ||
                    (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE)))
                {
                    // Do not change corrent position when we are no signal/bad now.
                    u8Offset = _u8SendedSize;
                    u8OffsetI = _u8SendedSize;
                }
                else
                #endif
                {
                    // from now, we use file position of last displayed picture.
                    fgRet = (VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));

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
                        u8Offset = _u8SendedSize;
                        u8OffsetI = _u8SendedSize;
                        fgNoI = TRUE;
                        LOG(5, "_SWDMX_MpgJumpToNormal VDP_GetFilePosition fail\n");
                    }

                    if(u8OffsetI > _u8FileSize)
                    {
                        LOG(3, "_SWDMX_MpgJumpToNormal VDP_GetFilePosition u8OffsetI 0x%llx > _u8FileSize 0x%llx\n",
                            u8OffsetI, _u8FileSize);
                        u8Offset = _u8FileSize;
                        u8OffsetI = _u8FileSize;
                    }

                    fgRet = (VDP_GetPicInfo(B2R_1, &rPicInfo));
                    if(fgRet)
                    {
                        #ifdef TIME_SHIFT_SUPPORT
                        _rSwdmxMpgInfo.rVar.u4CurTickNum = rPicInfo.u4TickNum;
                        _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;
                        #endif
                    }
                    else
                    {
                        #ifdef TIME_SHIFT_SUPPORT
                        //_rSwdmxMpgInfo.rVar.u4CurTickNum = rPicInfo.u4TickNum;
                        _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;
                        #endif
                        LOG(5, "_SWDMX_MpgJumpToNormal VDP_GetPicInfo fail\n");
                    }
                }
                _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                _u8SendedSize = u8OffsetI;
                _u8ReadedPos = u8OffsetI;
                _u8PlayPos = u8OffsetI;
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
                _rSwdmxMpgInfo.rVar.u8PreviousIPos = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = u8Offset;
                if(fgNoI)
                {
                    UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                    LOG(5, "_SWDMX_MpgJumpToNormal VDEC_SetRenderFromFirstPic from 0x%llx\n", u8OffsetI);
                    _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                }
                else
                {
                    UNUSED(VDEC_SetRenderFromPos(ES0, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                    LOG(5, "_SWDMX_MpgJumpToNormal VDEC_SetRenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        u8OffsetI, u8Offset, rPicInfo.u4Pts, i4TempRef);
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, _u8FileSize));
                    _rSwdmxMpgInfo.rVar.u4PreviousPTS = ((rPicInfo.u4Pts - (MPG_STC_BASE))+ _rSwdmxMpgInfo.rVar.u4FirstPTS);
                }
                _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
                _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
                UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
            }
            else
            {
                UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                LOG(5, "_SWDMX_MpgJumpToNormal fgSeek\n");
            }
            //STC_StopStc();
            //STC_SetStcValue(0);
            MPV_FlushEsmQ(ES0, FALSE);
            UNUSED(VDEC_ReleaseDispQ(ES0));
            VDP_ResetPts();
            UNUSED(VDEC_RegTrickPtsCb(NULL));

			if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)||
				(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)) // set the flag for TS H.264 wait IDR frame
			{
			   VDEC_SetWaitPvrSps(ES0,1);
			}
            /*
            if(!fgSeek)
            {
                if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    VDEC_Play(ES0, VDEC_FMT_MPV);
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    VDEC_Play(ES0, VDEC_FMT_H264);
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
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
        else
        {
            #ifdef TIME_SHIFT_SUPPORT
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                BOOL fgRet;
                u8Offset = _u8SendedSize;
                fgRet = _SWDMX_MpgGetPosByTickNum(
                    TRUE,
                    _rSwdmxMpgInfo.rVar.u4CurTickNum,
                    &u8Offset);
                if(fgRet)
                {
#if 1
                    _rSwdmxMpgInfo.rVar.u4ReadTickNum =
                        _rSwdmxMpgInfo.rVar.u4CurTickNum;
#endif
                    _u8SendedSize = u8Offset;
                    _u8ReadedPos = u8Offset;
                    _u8PlayPos = u8Offset;
                }
                else
                {
                    LOG(5, "_SWDMX_MpgJumpToNormal: CurTickNum %d fail\n", _rSwdmxMpgInfo.rVar.u4CurTickNum);
                }
            }
            #endif
            _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
            _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
            _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
        }

        _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        //_rSwdmxMpgInfo.rVar.fgFirst = TRUE;

        _SWDMX_MpgResetData();

        _rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
        // Don't need to change it when chagne speed.
        //_rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        _rSwdmxMpgInfo.rVar.u4PicCnt = 0;
        //_fgSetAudioStartTime = FALSE;

        LOG(5, "_SWDMX_MpgJumpToNormal Jump\n");
    }
    else
    {
        LOG(5, "_SWDMX_MpgJumpToNormal\n");
    }
    UNUSED(i4Speed);
}

// return TRUE if we found sequence
BOOL _SWDMX_MpgGetFirstSeqHdr()
{   // this is first run for one file, we have to get sequence header
    if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
        ((_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2) ||
        (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264) ||
        (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1) ||
        (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)))
    {
        if((!_fgSendSeqHdr) && (!_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq))
        {
            _u8OrgSendedSize = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = TRUE;
            _u8SendedSize = 0;
            _u8ReadedPos = 0;
            _u8PlayPos = 0;
            #if 1//def CC_DLNA_SUPPORT
            FeederSetReadyThreshold(MPG_DLNA_FEEDER_UNREACH);
            FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNREACH);
            FeederSetInfo(FEEDER_SWDMX_THRESHOLD, FALSE);
            #endif
            _SWDMX_MpgResetData();
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr to find sequence\n");
        }
        else if(!_fgSendSeqHdr)
        {
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr finding sequence\n");
        }
        else if(_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq)
        {
            _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
#ifdef CC_SWDMX_RENDER_POSITION
            if((_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq->fgSetRenderPosition))
            {
                _rSwdmxMpgInfo.rVar.prCurRangeReq->fgSetRenderPosition = FALSE;
                _u8OrgSendedSize = _rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI;
                _rSwdmxMpgInfo.rVar.u4PreviousPTS = _rSwdmxMpgInfo.rVar.prCurRangeReq->u8StartPts;

                if(_u8OrgSendedSize == (UINT64)-1)
                {
                    _u8OrgSendedSize = _rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset;
                    UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                    LOG(5, "_SWDMX_MpgGetFirstSeqHdr VDEC_SetRenderFromFirstPic I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, _rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset, _rSwdmxMpgInfo.rVar.u4PreviousPTS, _rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef);
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                }
                else
                {
                    _u8OrgSendedSize = _rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI;
                    UNUSED(VDEC_SetRenderFromPos(ES0, TRUE,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->u2DecodingOrder));
                    LOG(5, "_SWDMX_MpgGetFirstSeqHdr VDEC_SetRenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, _rSwdmxMpgInfo.rVar.prCurRangeReq->u8Offset, _rSwdmxMpgInfo.rVar.u4PreviousPTS, _rSwdmxMpgInfo.rVar.prCurRangeReq->i4TempRef);
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, _u8FileSize));
                }
            }
#endif
            _rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
            _u8SendedSize = _u8OrgSendedSize;
            _u8ReadedPos = _u8OrgSendedSize;
            _u8PlayPos = _u8OrgSendedSize;
            _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
            _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8OrgSendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8OrgSendedSize;
            _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8OrgSendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8OrgSendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8OrgSendedSize;
            _SWDMX_MpgResetData();
            MPV_FlushEsmQ(ES0, FALSE);
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr found sequence, move to 0x%llx\n", _u8OrgSendedSize);
            return TRUE;
        }
    }
    else if(!_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        if(_rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq)
        {
            _rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = FALSE;
            _u8SendedSize = _u8OrgSendedSize;
            _u8ReadedPos = _u8OrgSendedSize;
            _u8PlayPos = _u8OrgSendedSize;
            _SWDMX_MpgResetData();
            MPV_FlushEsmQ(ES0, FALSE);
            LOG(6, "_SWDMX_MpgGetFirstSeqHdr reset position\n");
            return TRUE;
        }
    }
    return FALSE;
}


VOID _SWDMX_MpgHandleSeekAudio(INT32 i4PlaySpeed, BOOL fgSeek)
{
    UINT32 u4Speed;
    UINT64 u8Tmp;
    UINT64 u8Remainder;

    if(i4PlaySpeed >= 0)
    {
        LOG(3, "_SWDMX_MpgHandleSeekAudio i4PlaySpeed >= 0\n");
        _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
        return;
    }
    if(_rSwdmxMpgInfo.rVar.u4TotalTime == 0)
    {
        LOG(3, "_SWDMX_MpgHandleSeekAudio u4TotalTime == 0\n");
        _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
        return;
    }

    u4Speed = (i4PlaySpeed > 0) ? i4PlaySpeed : -i4PlaySpeed;
    u4Speed /= 1000;
    u8Tmp = _SWDMX_Div6464(((_u8FileSize - _u8StartFileOffset)*MPG_AUDIO_ES_SKIP_TIME), (UINT64)(_rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
    u8Tmp *= u4Speed;
    UNUSED(u8Remainder);
    if(_u8PlayPos > u8Tmp)
    {
        _u8PlayPos -= u8Tmp;

        {
            UINT64 u8Remainder;
            UNUSED(_SWDMX_Div6464((_u8PlayPos + _u8StartFileOffset), (UINT64)_u4AlignBase, &u8Remainder));
            _u8PlayPos -= u8Remainder;
        }

        if((_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                UINT64 u8IdxBaseAddr;
                UINT64 u8IdxStartAddr;
                UINT64 u8IdxEndAddr;
                u8IdxBaseAddr =
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
                prIdxTbl = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                prTblEntry = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if((prTblEntry) && (prIdxTbl))
                {
                    if(prIdxTbl->ui4_number_of_entry > 0)
                    {
                        u8IdxEndAddr = u8IdxBaseAddr +
                            (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                            (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                        if(((_u8PlayPos) >=
                            (u8IdxEndAddr)
                           )
                           ||
                           (_rSwdmxMpgInfo.rVar.u4CurIndexNum >= prIdxTbl->ui4_number_of_entry)
                          )
                        {
                            _rSwdmxMpgInfo.rVar.u4CurIndexNum = (prIdxTbl->ui4_number_of_entry-1);
                        }
                        while(1)
                        {
                            u8IdxEndAddr = u8IdxBaseAddr +
                                (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                                (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                            if(((_u8PlayPos) >=
                                (u8IdxBaseAddr +
                                 prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset
                                )
                               )
                               &&
                               ((_u8PlayPos) <
                                (u8IdxEndAddr)
                               )
                              )
                            {
                                break;
                            }
                            if(_rSwdmxMpgInfo.rVar.u4CurIndexNum == 0)
                            {
                                break;
                            }
                            _rSwdmxMpgInfo.rVar.u4CurIndexNum--;
                        }
                        u8IdxStartAddr = u8IdxBaseAddr +
                            prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset;
                        _u8PlayPos = u8IdxStartAddr;
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

        LOG(5, "_SWDMX_MpgHandleSeekAudio new pos 0x%llx\n", _u8PlayPos);
    }
    else
    {
        // end seek
        _u8PlayPos = _u8FileSize;

        _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.u4CurIndexNum = 0;
#if 1//def CC_DLNA_SUPPORT
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
        LOG(5, "_SWDMX_MpgHandleSeekAudio end seek\n");
    }

    // can't flush audio data, or we can't play audio
    // _SWDMX_MpgResetData
    _SWDMX_FlushReqMsgQ();
    UNUSED(_SWDMX_MpgResetFeeder());
    _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    if(_rSwdmxMpgInfo.rVar.prCurRangeReq)
    {
        _rSwdmxMpgInfo.rVar.prCurRangeReq->eRangeStats = eSWDMX_RANGE_STATE_NOT_FINISHED;
    }

    _u8SendedSize = _u8PlayPos;
    _u8ReadedPos = _u8PlayPos;
    _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8PlayPos;
}

VOID _SWDMX_MpgHandleSeek(INT32 i4PlaySpeed, BOOL fgSeek)
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

    MPV_FlushEsmQ(ES0, FALSE);
    _SWDMX_MpgResetData();

    u8SendSizeWOIdx = _u8SendedSize;

    x_memset(&rPicInfo, 0, sizeof(rPicInfo));
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
        LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
            (UINT32)u8OffsetI, (UINT32)_u8FileSize);
        u8OffsetI = _u8FileSize;
    }*/
    u8Offset = 0;
    u8OffsetI = 0;
    u2DecodingOrder = 0;
    i4TempRef = 0;
    fgNoI = FALSE;

#ifdef TIME_SHIFT_SUPPORT
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (DMX_GetScrambleState(_u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        u8Offset = _u8SendedSize;
        u8OffsetI = _u8SendedSize;
        u2DecodingOrder = 0;
        i4TempRef = 0;
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
        LOG(5, "_SWDMX_MpgHandleSeek Scramble _u8SendedSize 0x%llx\n",
            _u8SendedSize);
    }
    else
#endif
    {
        UNUSED(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI));
        UNUSED(VDP_GetPicInfo(B2R_1, &rPicInfo));
        LOG(5, "_SWDMX_MpgHandleSeek VDP_GetFilePosition i4TempRef %d u2DecodingOrder %d I:0x%llx 0x%llx\n",
            i4TempRef, u2DecodingOrder, u8OffsetI, u8Offset);

        _rSwdmxMpgInfo.rVar.u4PreviousPTS =
            ((rPicInfo.u4Pts - (MPG_STC_BASE)) + _rSwdmxMpgInfo.rVar.u4FirstPTS);
    }

    if(((UINT32)(u8OffsetI>>32) == (UINT32)0xFFFFFFFF)
        && ((UINT32)(u8OffsetI) == (UINT32)0xFFFFFFFF))
    {
        u8OffsetI = u8Offset;
        LOG(3, "_SWDMX_MpgHandleSeek No I, start from 0x%llx\n",
            u8OffsetI);
        fgNoI = TRUE;
    }

    if(u8OffsetI > _u8FileSize)
    {
        LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI 0x%llx > _u8FileSize 0x%llx\n",
            u8OffsetI, _u8FileSize);
        u8OffsetI = _u8FileSize;
        u2DecodingOrder = 0;
        i4TempRef = 0;
    }

    /*
    _rSwdmxMpgInfo.rVar.u4PreviousPTS =
        ((rPicInfo.u4Pts - (MPG_STC_BASE)) + _rSwdmxMpgInfo.rVar.u4FirstPTS);

#ifdef TIME_SHIFT_SUPPORT
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (DMX_GetScrambleState(_u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        // no need this
        //_rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
    }
#endif
    */

    if(i4PlaySpeed == -1)
    {

        //u8Offset = u8OffsetI = 0;
        //u2DecodingOrder = 0;
        //i4TempRef = 0;
        // from now, we use file position of last displayed picture.
        /*VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
        if(u8OffsetI > _u8FileSize)
        {
            LOG(3, "_SWDMX_MpgHandleSeek VDP_GetFilePosition u8OffsetI %d > _u8FileSize %d\n",
                (UINT32)u8OffsetI, (UINT32)_u8FileSize);
            u8OffsetI = _u8FileSize;
            u2DecodingOrder = 0;
            i4TempRef = 0;
        }
        LOG(5, "_SWDMX_MpgHandleSeek VDP_GetFilePosition i4TempRef %d u2DecodingOrder %d u8Offset %d\n",
            i4TempRef, u2DecodingOrder, (UINT32)u8Offset, (UINT32)_u8FileSize);
        */
        //if(u8OffsetI == u8Offset)
        {
            if((u8OffsetI == u8Offset) &&
               (u8Offset == 0) &&
               (_rSwdmxMpgInfo.rVar.u2JumpAgainCount))
            {
                // end seek
                _rSwdmxMpgInfo.rVar.fgNotToADec = TRUE; // FixMe, should be FALSE
                _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
                _u8SendedSize = _u8FileSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    _rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
                    _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4Ret = _SWDMX_MpgHandleRing(
                        0, // begining of file. _u8SendedSize,
                        (_u4OneTimeSize * MPG_BATCH_READ_SIZE),
                        &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                    UNUSED(u4Ret);
                    _u8SendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                #endif
                {
                    LOG(5, "_SWDMX_MpgHandleSeek end seek\n");
                }
                u8SendSizeWOIdx = _u8SendedSize;
                _u8ReadedPos = _u8FileSize;
                _u8PlayPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
                if(_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                }
            }
            else
            {
                if(_rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    // GOP boundry is there. Back a GOP.
                    // _rSwdmxMpgInfo.rVar.u4TotalTime unit is 90000
                    //u4Tmp = (_rSwdmxMpgInfo.rVar.u4TotalTime/90000)*30; // total frames
                    //u8Size = (_u8FileSize/u4Tmp); // avg size/frame
                    //u8Size *= (u4Speed/1000);
                    u4Tmp = ((_rSwdmxMpgInfo.rVar.u4TotalTime + _rSwdmxMpgInfo.rVar.u4TotalTimeOffset)
                              / 3000); // total frames
                    u8Size = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);

                    //DTV00212070  check  is _rSwdmxMpgInfo.rVar.u4TotalTimeOffset  normal ?
                    u8SizeCompare = (UINT64)VDEC_GetAvgPicSize(0);
                    if(u8SizeCompare>6*u8Size)
                    {
                         u8Size= u8SizeCompare;
                         fgTotalTimeOffsetFake=TRUE;
                         LOG(5,"SWDMX_HandleSeek TotalTimeOffset is Wrong\n");
                    }
                    u8Size *= (30); // 1 sec
                    ASSERT(u8Size < (UINT64)0xFFFFFFFF);
                    u4Tmp = (UINT32)u8Size;
                    u4Tmp = (u4Tmp%(_u4AlignBase));
                    u8Size -= (UINT64)u4Tmp;
                    if(_rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                    {
                        u8Size *= _rSwdmxMpgInfo.rVar.u2JumpAgainCount;
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

                _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                _u8SendedSize = u8OffsetI;
                _u8ReadedPos = u8OffsetI;
                _u8PlayPos = u8OffsetI;
                u8SendSizeWOIdx = _u8SendedSize;
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
                _rSwdmxMpgInfo.rVar.u8PreviousIPos = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = u8OffsetI;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                if(fgNoI)
                {
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                    LOG(5, "_SWDMX_MpgHandleSeek VDEC_SetRenderFromFirstPic from 0x%llx\n", u8OffsetI);
                }
                else
                {
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(ES0, TRUE, u8Offset, i4TempRef, u2DecodingOrder));
                    UNUSED(VDP_GetPicInfo(B2R_1, &rPicInfo));
                    LOG(5, "_SWDMX_MpgHandleSeek VDEC_SetRenderBackStep I:0x%llx, 0x%llx, PTS:%d, i4TempRef:%d, u2DecodingOrder:%d\n", u8OffsetI,
                        u8Offset, rPicInfo.u4Pts, i4TempRef, (UINT32)u2DecodingOrder);
                }
                UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
            }
        }
    }
    else
    {
        u4Speed = (i4PlaySpeed < 0) ? ((UINT32)-i4PlaySpeed) : ((UINT32)i4PlaySpeed);

        // remove _rSwdmxMpgInfo.rVar.u4TotalTimeOffset
        // when totaltime is 0, we use average frame size from vdec.
        if(((_rSwdmxMpgInfo.rVar.u4TotalTime)
            > 0) && (_u8FileSize > 0))
        {
            // _rSwdmxMpgInfo.rVar.u4TotalTime unit is 90000
            //u4Tmp = (_rSwdmxMpgInfo.rVar.u4TotalTime/90000)*30; // total frames
            //u8Size = (_u8FileSize/u4Tmp); // avg size/frame
            //u8Size *= (u4Speed/1000);
            u4Tmp = ((_rSwdmxMpgInfo.rVar.u4TotalTime + _rSwdmxMpgInfo.rVar.u4TotalTimeOffset)
                        / 3000); // total frames
            u8Size = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset), (UINT64)u4Tmp, &u8Remainder);

            //DTV00212070  check  is _rSwdmxMpgInfo.rVar.u4TotalTimeOffset  normal ?
            u8SizeCompare=(UINT64)VDEC_GetAvgPicSize(0);
            if(u8SizeCompare>6*u8Size)
            {
                 u8Size = u8SizeCompare;
                 fgTotalTimeOffsetFake=TRUE;
                 LOG(5,"SWDMX_HandleSeek TotalTimeOffset is Wrong\n");
           	}
            u8Tmp = u8Size;
            u8Size *= (u4Speed / 1000);
            //if(i4PlaySpeed < 0) //DTV00212070
            {
                if(_rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * _rSwdmxMpgInfo.rVar.u2JumpAgainCount);
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

                u8Tmp = (UINT64)VDEC_GetAvgPicSize(0);
                u8Tmp *= 30;
                u8Tmp *= u4SkipTime;
                u8Tmp = _SWDMX_Div6464(u8Tmp, (UINT64)1000000, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
               }
              else
              {
                // a = _u8FileSize / u4TotalTime -> 90K base
                // b = (u4SkipTIme / 1000000) * (a/90K)
                // b = (u4SkipTIme / 1000000) * ((_u8FileSize / u4TotalTime)/90000)
                // b = (u4SkipTIme / 100) * ((_u8FileSize / u4TotalTime)/9)
                u8Tmp = (u4SkipTime/100);
                u8Tmp *= 9;
                u8Tmp = (u8Tmp * (_u8FileSize - _u8StartFileOffset)); //1000000 / 90000 -> 100/9
                u8Tmp = _SWDMX_Div6464(u8Tmp,
                    ((UINT64)_rSwdmxMpgInfo.rVar.u4TotalTime + (UINT64)_rSwdmxMpgInfo.rVar.u4TotalTimeOffset),
                    &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
              }
            }
        }
        else
        {
            u8Size = (UINT64)VDEC_GetAvgPicSize(0);
            u8Tmp = u8Size;
            u8Size *= (UINT64)(u4Speed / 1000);
            //if(i4PlaySpeed < 0)       //DTV00212070
            {
                if(_rSwdmxMpgInfo.rVar.u2JumpAgainCount)
                {
                    u8Size += (u8Tmp * _rSwdmxMpgInfo.rVar.u2JumpAgainCount);
                }
            }
            u8SizeWithoutSkipTime = u8Size;
            {
                UINT64 u8Remainder;
                UNUSED(_SWDMX_Div6464(u8SizeWithoutSkipTime, (UINT64)_u4AlignBase, &u8Remainder));
                u8SizeWithoutSkipTime -= u8Remainder;
            }

            // This is the time we spent on the sending I frame
            if(u4SkipTime > 3000000) // 3 sec
            {
                LOG(5, "_SWDMX_MpgHandleSeek u4SkipTime %d\n", (UINT32)u4SkipTime);
                u4SkipTime = 3000000;
            }
            if(u4SkipTime > 10000) // 10 ms
            {
                u8Tmp = (UINT64)VDEC_GetAvgPicSize(0);
                u8Tmp *= 30;
                u8Tmp *= u4SkipTime;
                u8Tmp = _SWDMX_Div6464(u8Tmp, (UINT64)1000000, &u8Remainder);
                u8Tmp *= (u4Speed / 1000); // we should also consider the speed.
                u8Size += u8Tmp;
            }
        }

        ASSERT(u8Size < (UINT64)0xFFFFFFFF);
        {
            UINT64 u8Remainder;
            UNUSED(_SWDMX_Div6464(u8Size, (UINT64)_u4AlignBase, &u8Remainder));
            u8Size -= u8Remainder;
        }

        if(i4PlaySpeed < 0)
        {
            if(_rSwdmxMpgInfo.rVar.u8JumpStartAddr > u8Size)
            {
                _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
                // DTV00136835 [MM]Video auto resume when time seek to end in fr mode.
                if(fgSeek)
                {
                    // When we are seek, make the first file position const.
                    // Not affected by skip time.
                    if(_rSwdmxMpgInfo.rVar.u8JumpStartAddr > (u8SizeWithoutSkipTime * 2))
                    {
                        _u8SendedSize =
                            (_rSwdmxMpgInfo.rVar.u8JumpStartAddr - (u8SizeWithoutSkipTime * 2));
                    }
                    else
                    {
                        _u8SendedSize = 0;
                    }
                }
                else
                {
                    _u8SendedSize = (_rSwdmxMpgInfo.rVar.u8JumpStartAddr - u8Size);
                }
                /*if(u8OffsetI > _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    u4Tmp = (UINT32)(u8OffsetI - _rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                    _u8SendedSize -= (u4Tmp/2);
                }*/
                u8SendSizeWOIdx = _u8SendedSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    u4SkipTime += (u4Speed * 33); // 30 fps
                    if(_rSwdmxMpgInfo.rVar.u4CurTickNum >=
                        (u4SkipTime / _rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period))
                    {
                        _rSwdmxMpgInfo.rVar.u4CurTickNum -=
                            (u4SkipTime / _rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period);
                    }
                    _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4OutReadSize = 0;
                    u4Ret = _SWDMX_MpgHandleRing(
                        _u8SendedSize,
                        (_u4OneTimeSize * MPG_BATCH_READ_SIZE),
                        &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                    if(u4Ret == MPG_RING_END_MEET)
                    {
                        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
                    }
                    else if(u4Ret != MPG_RING_OK)
                    {
                        _u8SendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                    }
                    else
                    {
                        if(_SWDMX_MpgGetIPosByTickNum(_u8SendedSize,
                             (_rSwdmxMpgInfo.rVar.u4ReadTickNum),
                             &u8OutReadAddr))
                        {
                            // change I frame start position
                            _u8SendedSize = u8OutReadAddr;
                        }
                    }
                }
                #endif
                #if 0 // move up
                u8SendSizeWOIdx = _u8SendedSize;
                #endif
                #ifdef SWDMX_MPG_USE_INDEX
                if(_rSwdmxMpgInfo.rVar.fgUseIdx)
                {
                    UINT64 u8RetPos;
                    if(!_SWDMX_MpgIdxGetPosByPos(TRUE, _u8SendedSize, &u8RetPos))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgIdxGetPosByPos fail\n");
                    }
                    LOG(5, "_SWDMX_MpgHandleSeek ff GetPosByPos 0x%llx -> 0x%llx\n",
                        _u8SendedSize, u8RetPos);
                    // if previous jump get a initial value in index table
                    // this time we might get a correct one, but that is a bigger one.
                    // avoid this case
                    if(u8RetPos <= _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                    {
                        _u8SendedSize = u8RetPos;
                    }
                }
                #endif
                _u8ReadedPos = _u8SendedSize;
                _u8PlayPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
                if(_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
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
                _rSwdmxMpgInfo.rVar.fgNotToADec = TRUE; // FixMe, should be FALSE
                _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
                FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
                _u8SendedSize = _u8FileSize;
                #ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    UINT32 u4Ret;
                    UINT32 u4OutReadSize;
                    UINT64 u8OutReadAddr;

                    _rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
                    _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

                    u4Ret = _SWDMX_MpgHandleRing(
                        0, // begining of file. _u8SendedSize,
                        (_u4OneTimeSize * MPG_BATCH_READ_SIZE),
                        &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                    UNUSED(u4Ret);
                    _u8SendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                #endif
                {
                    LOG(5, "_SWDMX_MpgHandleSeek end seek\n");
                }
                u8SendSizeWOIdx = _u8SendedSize;
                _u8ReadedPos = _u8FileSize;
                _u8PlayPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
                _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
                _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
                if(_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, (UINT64)0));
                    UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                }
            }
        }
        else // i4PlaySpeed > 0
        {
            _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
            _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
            FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
            // DTV00136835 [MM]Video auto resume when time seek to end in fr mode.
            if(fgSeek)
            {
                // When we are seek, make the first file position const.
                // Not affected by skip time.
                _u8SendedSize =
                    (_rSwdmxMpgInfo.rVar.u8JumpStartAddr + (u8SizeWithoutSkipTime * 2));
            }
            else
            {
                _u8SendedSize = (_rSwdmxMpgInfo.rVar.u8JumpStartAddr + u8Size);
            }
            u8SendSizeWOIdx = _u8SendedSize;
            #ifdef TIME_SHIFT_SUPPORT
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                UINT32 u4Ret;
                UINT32 u4OutReadSize;
                UINT64 u8OutReadAddr;

                u4SkipTime += (u4Speed * 33); // 30 fps
                _rSwdmxMpgInfo.rVar.u4CurTickNum +=
                    (u4SkipTime / _rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period);
                _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

                u4Ret = _SWDMX_MpgHandleRing(
                    _u8SendedSize,
                    (_u4OneTimeSize * MPG_BATCH_READ_SIZE),
                    &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
                if(u4Ret != MPG_RING_OK)
                {
                    // this is end of current range case
                    // do not move send position.
                    if(_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8ValidEndAddr)
                    {
                        u8SendSizeWOIdx = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                        LOG(6, "_SWDMX_MpgHandleSeek _u8SendedSize(0x%llx) >= u8ValidEndAddr(0x%llx)\n",
                            _u8SendedSize, _rSwdmxMpgInfo.rVar.u8ValidEndAddr);
                    }
                    _u8SendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                }
                else
                {
                    if(_SWDMX_MpgGetIPosByTickNum(_u8SendedSize,
                         (_rSwdmxMpgInfo.rVar.u4ReadTickNum),
                         &u8OutReadAddr))
                    {
                        // change I frame start position
                        _u8SendedSize = u8OutReadAddr;
                    }
                }
            }
            #endif
            #if 0 // move up
            u8SendSizeWOIdx = _u8SendedSize;
            #endif
            /*if(u8OffsetI > _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
            {
                u4Tmp = (UINT32)(u8OffsetI - _rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                _u8SendedSize += (u4Tmp/2);
            }*/
            #ifdef SWDMX_MPG_USE_INDEX
            if(_rSwdmxMpgInfo.rVar.fgUseIdx)
            {
                UINT64 u8RetPos;
                if(!_SWDMX_MpgIdxGetPosByPos(FALSE, _u8SendedSize, &u8RetPos))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgIdxGetPosByPos fail\n");
                }
                LOG(5, "_SWDMX_MpgHandleSeek fb GetPosByPos 0x%llx -> 0x%llx\n",
                    _u8SendedSize, u8RetPos);
                // if previous jump get a initial value in index table
                // this time we might get a correct one, but that is a small one.
                // avoid this case
                if(u8RetPos >= _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    _u8SendedSize = u8RetPos;
                }
            }
            #endif
            _u8ReadedPos = _u8SendedSize;
            _u8PlayPos = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
            _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
            _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
            if(_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                if(fgSeek)
                {
                    // alwasy render it.
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE, _u8FileSize));
                }
                else
                {
                    // only render it after u8Offset + onetimesize.
                    UNUSED(VDEC_SetRenderFromFirstPic(ES0, TRUE,
                            (u8Offset + (_u4OneTimeSize))));
                }
            }
        }
    }

    if(_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_MPV))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm VDEC_FMT_MPV fail\n");
            }
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_H264))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm VDEC_FMT_H264 fail\n");
            }
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_WMV))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_VC1 fail\n");
            }
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
        {
            if(!VDEC_PlayOneFrm(ES0, VDEC_FMT_MP4))
            {
                LOG(3, "_SWDMX_MpgHandleSeek VDEC_PlayOneFrm ENUM_SWDMX_VDEC_MPEG4 fail\n");
            }
        }
        else
        {
            ASSERT(0);
        }
    }

    LOG(6, "_SWDMX_MpgHandleSeek new addr 0x%llx (WO Idx 0x%llx)\n",
        _u8SendedSize, u8SendSizeWOIdx);

    // reset jump
    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8SendSizeWOIdx;
    _rSwdmxMpgInfo.rVar.u4PicCnt = 0;
}

VOID _SWDMX_MpgHandleChange(INT32 i4PlaySpeed, UINT64 u8SeekPos)
{
    BOOL fgRet;
    BOOL fgSeek;
    UINT32 u4Count;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    UINT64 u8Offset, u8OffsetI;

    fgSeek = FALSE;
    if(((UINT32)(u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
        || ((UINT32)(u8SeekPos) != (UINT32)0xFFFFFFFF))
    {
        LOG(5, "_SWDMX_MpgHandleChange change pos: 0x%llx -> 0x%llx ================\n",
            _u8SendedSize, u8SeekPos);

        _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;

        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        _rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
     #if 0   // DTV00077914 remove it  when in no signal status ,FW to end and speed change to 1x, will send a wrong "  MM_SIGNAL_N_TO_Y" to mw
        if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL)
        {
            LOG(3, "_SWDMX_MpgHandleChange: MM_SIGNAL_N_TO_Y u4NoSignalStage 0\n");
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _prSwdmxInfo->pfnSwdmxRangeCb(
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_SIGNAL_N_TO_Y,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF);
                }
            }
        }
        else if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE)
        {
            LOG(3, "_SWDMX_MpgHandleChange: MM_BAD_TICK_Y_TO_N u4NoSignalStage 0\n");
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    _prSwdmxInfo->pfnSwdmxRangeCb(
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_BAD_TICK_Y_TO_N,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF);
                }
            }
        }
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
	#endif 	
//#if 1
#if 0
        _rSwdmxMpgInfo.rVar.prLastTickIndex = NULL;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = NULL;

        {
            BOOL fgRet;
            UINT32 u4Temp;
            fgRet = _SWDMX_MpgGetTickByPos(
                TRUE,
                _u8SendedSize,
                i4PlaySpeed,
                &u4Temp);
            if(fgRet)
            {
                _rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
                _rSwdmxMpgInfo.rVar.u4ReadTickNum =
                    _rSwdmxMpgInfo.rVar.u4CurTickNum;
                LOG(5, "_SWDMX_MpgHandleChange tick num: %d\n",
                    _rSwdmxMpgInfo.rVar.u4CurTickNum);
            }
            else
            {
                // give a initial value
                _rSwdmxMpgInfo.rVar.u4CurTickNum =
                    _rSwdmxMpgInfo.rVar.u4SafeStartTick;
                _rSwdmxMpgInfo.rVar.u4ReadTickNum =
                    _rSwdmxMpgInfo.rVar.u4CurTickNum;
                LOG(5, "_SWDMX_MpgHandleChange, NG, use safe start tick num: %d\n",
                    _rSwdmxMpgInfo.rVar.u4CurTickNum);
            }
        }
#endif
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        //_fgSetAudioStartTime = FALSE;
        //_rSwdmxMpgInfo.rVar.fgFirst = TRUE;
        _rSwdmxMpgInfo.rVar.u4PicCnt = 0;
        u4Count = (UINT32)u8SeekPos;
        UNUSED(u4Count);
        if(u8SeekPos < (_u4AlignBase))
        {
            _u8SendedSize = 0;
            _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
        }
        else
        {
#ifdef TIME_SHIFT_SUPPORT
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                _u8SendedSize = _SWDMX_MpgTSAlignDown(u8SeekPos);
            }
            else
#endif
            {
                {
                    UINT64 u8Remainder;
                    _u8SendedSize = u8SeekPos;
                    UNUSED(_SWDMX_Div6464((u8SeekPos + _u8StartFileOffset), (UINT64)_u4AlignBase, &u8Remainder));
                    _u8SendedSize -= u8Remainder;
                }
            }
            _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
        }
        _u8ReadedPos = _u8SendedSize;
        _u8PlayPos = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

        //  DTV00211793 only audio, no video ,do seek when have send EOF, need clear the flags
        _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#ifdef TIME_SHIFT_SUPPORT           //DTV00077914
		if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
		{
			UINT32 u4Temp;
			fgRet = _SWDMX_MpgGetTickByPos(
								FALSE,
								_u8SendedSize,
								i4PlaySpeed,
								&u4Temp);
			if((_rSwdmxMpgInfo.rVar.prLastTickIndex) &&
							   (fgRet))
			{
				_rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
				_rSwdmxMpgInfo.rVar.u4ReadTickNum = u4Temp;
				//LOG(5, "Trace u4CurTickNum(%d) _u8SendedSize(0x%llx)\n", _rSwdmxMpgInfo.rVar.u4CurTickNum, _u8SendedSize);
				_rSwdmxMpgInfo.rVar.u8CurTime =
				(UINT64)_rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
				_rSwdmxMpgInfo.rVar.u8CurTime *=
				_rSwdmxMpgInfo.rVar.u4TickBase;
			}
			else
			{
				_rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
				_rSwdmxMpgInfo.rVar.u4ReadTickNum = 0;
				_rSwdmxMpgInfo.rVar.u8CurTime = 0;
				_rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
			}
		}
#endif

        _SWDMX_MpgResetData();
        // DTV00132905, should stop timer bar
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            STC_StopStc();
        }
        fgSeek = TRUE;
    }

    if(_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        LOG(5, "_SWDMX_MpgHandleChange change speed: %d ====================\n", i4PlaySpeed);
#ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(!_SWDMX_PVR_SetDataMode(i4PlaySpeed))
            {
                LOG(3, "_SWDMX_MpgHandleChange _SWDMX_PVR_SetDataMode fail\n");
            }
        }
#endif
        if(i4PlaySpeed == 1000)
        {
            // always do start to play behavior
            //_SWDMX_MpgJumpToNormal(fgSeek,
            //    _rSwdmxMpgInfo.rVar.fgJumpping,
            //    i4PlaySpeed);
            //_SWDMX_MpgJumpToNormal(fgSeek, TRUE,
            //    i4PlaySpeed);
            //DTV00078801  for H264 video ES  flush will cause H.264 decdode can't found PPS.
            // video es 2x-->1x no needed to do flush 
            if(((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)||
			    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
			    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
			    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES))&&
				(_rSwdmxMpgInfo.rVar.i4CurSpeed >=1 )&&(_rSwdmxMpgInfo.rVar.i4CurSpeed <= 2000))
            {

              _SWDMX_MpgJumpToNormal(fgSeek, FALSE,
                i4PlaySpeed);			  
            }
			else
			{
              _SWDMX_MpgJumpToNormal(fgSeek, TRUE,
                i4PlaySpeed);
			}			

            _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
            _rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;

        }
        else if (
            ((i4PlaySpeed > 1000) && (i4PlaySpeed <= 1500))
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
            ||
             // when play time-shift, usb performace is not good enough
             // use jump mode to play 2x speed.
            (((i4PlaySpeed > 1500) && (i4PlaySpeed < 4000))
             &&
             (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
#endif
            ||
            ((i4PlaySpeed >= 1) && (i4PlaySpeed < 1000)))
        {
            BOOL fgStartToPlay = FALSE;
            // DTV00211049 [MM Compatibility]No audio output when play after FF & FB with special file
            // If previous speed == step and now != step, do a start to play behavior
            if (((_rSwdmxMpgInfo.rVar.i4CurSpeed == 1) &&
                 (i4PlaySpeed != 1))
                ||
                // even we are in 1x speed,
                // vdec will fail to get a frame vdp are pause
                // so if we change to step, we will lose one frame to display
                // add the condition to avoid it
                ((_rSwdmxMpgInfo.rVar.i4CurSpeed != 1) &&
                 (i4PlaySpeed == 1))
                ||
                _rSwdmxMpgInfo.rVar.fgJumpping)
            {
                fgStartToPlay = TRUE;
            }

            //DTV00078801  for H264 video ES  flush will cause H.264 decdode can't found PPS.

		 if(( _rSwdmxMpgInfo.rVar.i4CurSpeed ==1000)&&
		     (i4PlaySpeed >= 1)&& (i4PlaySpeed < 1000)&&
		     ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES)||
		   	 (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES)||
			 (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES)||
			 (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES)))
			{
			  fgStartToPlay = FALSE;
			}


            _SWDMX_MpgJumpToNormal(fgSeek,
                fgStartToPlay,
                i4PlaySpeed);

            _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
            _rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxMpgInfo.rVar.fgNotToADec = TRUE;
        }
        else
        {
            MPV_FlushEsmQ(ES0, FALSE);
            UNUSED(VDEC_ReleaseDispQ(ES0));
            VDP_ResetPts();

            _SWDMX_MpgResetData();
            _rSwdmxMpgInfo.rVar.fgJumpping = TRUE;
            _rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
            _rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
            _rSwdmxMpgInfo.rVar.fgNotToADec = TRUE;
            _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
            _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
            _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
            FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
            //_rSwdmxMpgInfo.rVar.fgFirst = TRUE;

            _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
            _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
            _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
            _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
            _rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
            _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
            _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
            _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
            _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
            // Don't need to change it when chagne speed.
            //_rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
            _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
            _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
            _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
            _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
            _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
            //_fgSetAudioStartTime = FALSE;
            _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
            _rSwdmxMpgInfo.rVar.u4PicCnt = 0;
            // from now, we use file position of last displayed picture.
            u8OffsetI = 0;
            u8Offset = u8OffsetI;
            u2DecodingOrder = 0;
            i4TempRef = 0;
            if(!fgSeek)
            {
                // if seek and trick, we might get the older position before seek.
                // we have to ignore the older position when seek.
                fgRet = VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI);
                if(fgRet)
                {
                    if(i4PlaySpeed > 0)
                    {
                        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8Offset;
                        LOG(5, "_SWDMX_MpgHandleChange Jumpping 1 _u8SendedSize 0x%llx u8JumpStartAddr 0x%llx\n",
                            _u8SendedSize, _rSwdmxMpgInfo.rVar.u8JumpStartAddr);
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
                        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8OffsetI;
                        LOG(5, "_SWDMX_MpgHandleChange Jumpping 2 _u8SendedSize 0x%llx u8JumpStartAddr 0x%llx\n",
                            _u8SendedSize, _rSwdmxMpgInfo.rVar.u8JumpStartAddr);
                    }
                }
                else
                {
                    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = u8Offset;
                    LOG(5, "_SWDMX_MpgHandleChange VDP_GetFilePosition fail\n");
                }
            }
            _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
            _rSwdmxMpgInfo.rVar.u8PreviousIPos = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
            _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
            _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;

            LOG(5, "_SWDMX_MpgHandleChange VDEC_GetAvgPicSize %d\n",
                VDEC_GetAvgPicSize(0));

            HAL_GetTime(&_rJumpStartTime);
            UNUSED(VDEC_RegTrickPtsCb(_SWDMX_MpgSeekFinNotify));
            #ifdef TIME_SHIFT_SUPPORT
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL)
                {
                    _SWDMX_MpgHandleSeek(i4PlaySpeed, fgSeek);
                }
            }
            else
            #endif
            {
                _SWDMX_MpgHandleSeek(i4PlaySpeed, fgSeek);
            }
        }
    }
    else if(((_rSwdmxMpgInfo.rFmt.fgAudio) &&
             // when RAW_AUDIO_ES, we support -?x speed in driver build
             (((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
               (_rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
               (i4PlaySpeed < 0))
              ||
              (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
             )
            )
            ||
            // we should reset variable in audio only stream
            // (might not set audio yet)
            (!_rSwdmxMpgInfo.rFmt.fgAudio)
           )
    {
        LOG(5, "_SWDMX_MpgHandleChange no video change speed: %d ====================\n", i4PlaySpeed);

        _SWDMX_MpgResetData();
        if(i4PlaySpeed < 0)
        {
            _rSwdmxMpgInfo.rVar.fgJumpping = TRUE;
        }
        _rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
        _rSwdmxMpgInfo.rVar.u2JumpAgainCount = 0;
        _rSwdmxMpgInfo.rVar.fgNotToADec = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
#if 1//def CC_DLNA_SUPPORT
        FeederSetInfo(FEEDER_SWDMX_EOS, FALSE);
#endif
         // when audio es change speed or change position, 
		 // we should send data begining at the position which pts>0(begining of each index entry)
		 // so there will be a jumpping, after do seek, or change speed  _u8SendedSize = _u8PlayPos;
	#if 1   
		_u8SendedSize = _u8ReadedPos;		
        _u8ReadedPos = _u8PlayPos;
        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8PlayPos;
	#else  
		//reuse this code for trick mode -->normal mode jummping
		_u8SendedSize = _u8ReadedPos;
		_u8ReadedPos = _u8ReadedPos;
		_rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8ReadedPos;
	#endif 
	
        //_SWDMX_MpgHandleSeekAudio(i4PlaySpeed, fgSeek);

        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
         _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        _rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
        _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
        _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
#ifdef TIME_SHIFT_SUPPORT
            // Don't need to change it when chagne speed.
            //_rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
#endif
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
        _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif
        _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
        _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
        _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
        _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
        _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u4PicCnt = 0;

        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxMpgInfo.rVar.u8PreviousIPos = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    }
    else if((_rSwdmxMpgInfo.rFmt.fgAudio) &&
             (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
               (i4PlaySpeed == 1000)&&(_rSwdmxMpgInfo.rVar.i4CurSpeed !=1000))
   {
   	  _u8SendedSize = _u8PlayPos;
	  _u8ReadedPos = _u8PlayPos;
	  _rSwdmxMpgInfo.rVar.u8JumpStartAddr = _u8PlayPos;
		
      _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
	  _rSwdmxMpgInfo.rVar.fgSetStc = FALSE; // PTS reset
      _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
      _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
	  _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
	  
   }


    _rSwdmxMpgInfo.rVar.i4CurSpeed = i4PlaySpeed;
}

VOID _SWDMX_MpgHandleEOS(INT32 i4PlaySpeed)
{
    UINT32 au4BufStart[2], au4BufEnd[2];

    #if 1//def CC_DLNA_SUPPORT
    if(_rSwdmxMpgInfo.rVar.e_mm_src_type !=
        MM_SRC_TYPE_HIGH_SPEED_STORAGE)
    {
        FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
    }
    #endif

    // if i4PlaySpeed = 1x or 2x, fgVideoPause = TRUE
    // then got EOS and VDec in pause.
    //if(_rSwdmxMpgInfo.rVar.fgSendEOF)
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if((((_rSwdmxMpgInfo.rFmt.fgAudio) && (!_rSwdmxMpgInfo.rVar.fgGotAudioReady))
            ||
            (!_rSwdmxMpgInfo.rFmt.fgAudio)) &&
            ((_rSwdmxMpgInfo.rFmt.fgVideo) && (_rSwdmxMpgInfo.rVar.fgVideoPause)))
        {
            UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
            if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_H264))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_MP4))
                {
                    LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                }
            }
            else
            {
                ASSERT(0);
            }
            _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
            _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
            _rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
        }
    }

    // DTV00132905, seek to end without sending any a/v data.
    if(!_rSwdmxMpgInfo.rVar.fgSetStc)
    {
        _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
        STC_StopStc();
        STC_SetStcValue(_rSwdmxMpgInfo.rVar.u4TotalTime + _rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
        LOG(3, "_SWDMX_MpgHandleEOS STC_SetStcValue 0x%x\n",
            _rSwdmxMpgInfo.rVar.u4TotalTime + _rSwdmxMpgInfo.rVar.u4TotalTimeOffset);
    }

    if((_rSwdmxMpgInfo.rFmt.fgAudio) && (!_rSwdmxMpgInfo.rVar.fgSendAudioEos))
    {
        DMX_AUDIO_PES_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        if(_rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
        {
            rPes.u4Wp = _rSwdmxMpgInfo.rVar.u4LastAudioWptr;
        }
        else
        {
#ifdef AUD_OLDFIFO_INTF
            VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
            VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
            rPes.u4Wp = (au4BufStart[0] + 4); // Jessica.
        }
        UNUSED(AUD_SendAudioPes(&rPes)); // audio driver said they need this one.
        rPes.fgEOS = TRUE;
        //_SWDMX_MpgAudioCallback(&rPes);
        UNUSED(AUD_SendAudioPes(&rPes));
        _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;
        if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
        {
            _rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            _rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;

            _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS =
                ((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS) + (MPG_STC_BASE));
            #ifdef TIME_SHIFT_SUPPORT
            if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            #endif
            {
                UNUSED(AUD_SetStartPts(AUD_DEC_MAIN, _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS));
                //AUD_MMAoutEnable(0, TRUE);
                /*
                if(!_rSwdmxMpgInfo.rVar.fgSetStc)
                {
                    _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                    STC_StopStc();
                    STC_SetStcValue((_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS));
                    LOG(3, "_SWDMX_MpgHandleEOS AUD_SetStartPts 4 pts 0x%x pos %d\n",
                        (_rSwdmxMpgInfo.rVar.u4LastPTS - _rSwdmxMpgInfo.rVar.u4FirstPTS), (UINT32)_u8SendedSize);
                }
                */
                //STC_StartStc();
            }

            //_rSwdmxMpgInfo.rVar.fgGotAudio = TRUE;
            //_rSwdmxMpgInfo.rVar.fgGotAudioReady = TRUE;
        }
        LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Adec\n");
    }
    if((_rSwdmxMpgInfo.rFmt.fgVideo) && (!_rSwdmxMpgInfo.rVar.fgSendVideoEos))
    {
        DMX_PES_MSG_T rPes;
        x_memset(&rPes, 0, sizeof(rPes));
        rPes.fgEOS = TRUE;
        rPes.u4FrameType = PIC_TYPE_I;
        rPes.fgSeqHeader = TRUE;
        rPes.fgGop = TRUE;
        if(!_SWDMX_MpgVideoCallback(&rPes))
        {
            LOG(3, "_SWDMX_MpgHandleSeek _SWDMX_MpgVideoCallback fail\n");
        }
        _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
        // i4PlaySpeed = 4x or up.
        // if now in VDEC_PlayOneFrm, VDec might in pause status.
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((i4PlaySpeed < 1)
                || (i4PlaySpeed >= 4000)
                || (_rSwdmxMpgInfo.rVar.fgVideoPause))
            {
                UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
                if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_MPV fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_H264))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_H264 fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play VDEC_FMT_WMV fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_MP4))
                    {
                        LOG(3, "_SWDMX_MpgHandleSeek VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                    }
                }
                else
                {
                    ASSERT(0);
                }
                _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                _rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
            }
        }
        LOG(5, "_SWDMX_MpgHandleEOS send dummy EOS to Vdec\n");
    }
}

INT32 _SWDMX_MpgHandleFifoFull(INT32 i4PlaySpeed, BOOL fgLast)
{
    BOOL fgRet;
    BOOL fgFull;
    BOOL fgAudioSmaller;
    DMX_DDI_T rDDI;
    UINT32 u4DmxAvailSize;
    UINT32 u4DmxAvailSizeAudio;
    UINT16 u2QueueSize, u2MaxQueueSize;

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
    if(!_fgGetInfo)
    {
        // playbacking, check AV size
        if(_rSwdmxMpgInfo.rFmt.fgVideo)
        {
            // DTV00139611, [MM]Only flash progress bar after select current video file to play
            // notify not support when this clip is ts file with scramble data
            if((!_rSwdmxMpgInfo.rVar.fgGotVideo) &&
               ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)))
            {
                if(DMX_GetScrambleState(_u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
                    VDEC_Notify(ES0,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
                    // VDEC_DEC_DECODE_NO_DATA
                    LOG(3,"video scramble\n");
                }
            }

            u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO);
            if(u4DmxAvailSize <=
                (1024*1024))
            {
                u2QueueSize = 0;
                u2MaxQueueSize = 0;
                VDEC_GetQueueInfo(0, &u2QueueSize, &u2MaxQueueSize);
                if(u2QueueSize < 2)
                {
                    // we have two choices.
                    // 1. notfiy video not exist.
                    // 2. just flush fifo data
                    #if 0
                    // 1.
                    VDEC_Notify(ES0,VDEC_DEC_DECODE_CODEC_NOT_SUPPORT);
                    // We don't know where to jump to.
                    //if(_rSwdmxMpgInfo.rFmt.fgAudio)
                    //{
                    //    _rSwdmxMpgInfo.rVar.fgResetPosAfterFoundSeq = TRUE;
                    //}
                    LOG(3,"there is no video frame in queue, disable it\n");
                    #else
                    // 2.
                    _SWDMX_MpgFlushVFifo();
                    if(_u1PidxVideo != 0xFF)
                    {
                        UNUSED(DMX_MM_FlushBuffer(_u1PidxVideo));
                    }
                    LOG(3,"there is no video frame in queue, flush it\n");
                    #endif
                    return TRUE;
                }
            }
        }
        if(_rSwdmxMpgInfo.rFmt.fgAudio)
        {
            // DTV00139611, [MM]Only flash progress bar after select current video file to play
            // notify not support when this clip is ts file with scramble data
            if((!_rSwdmxMpgInfo.rVar.fgGotAudio) &&
               ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)))
            {
                if(DMX_GetScrambleState(_u1PidxAudio) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
#ifdef SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
                    AUD_MM_Set_Dec_Fmt_Scramble(0, TRUE);
#endif
                    LOG(3,"audio scramble\n");
                }
            }

#ifdef CC_DMX_PURE_AUDIO
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        	{
        	    u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
        	        _u1PidxAudio);
        	}
            else
#endif
        	{
                u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
        	}
            if(u4DmxAvailSize > u4DmxAvailSizeAudio)
            {
                u4DmxAvailSize = u4DmxAvailSizeAudio;
                fgAudioSmaller = TRUE;
            }
        }
        /* need to complete audio2
        // DTV00139611, [MM]Only flash progress bar after select current video file to play
        // notify not support when this clip is ts file with scramble data
        if(_rSwdmxMpgInfo.rFmt.fgAudio2)
        {
            if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
            {
                if(DMX_GetScrambleState(_u1PidxAudio) == DMX_SCRAMBLE_STATE_SCRAMBLED)
                {
#ifdef SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
                    AUD_MM_Set_Dec_Fmt_Scramble(0, TRUE);
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

    if(_fgCheckPSISize)
    {
        static UINT32 u4BufferUnderRunCnt = 0;
        // parsing, check psi buffer size
        u4DmxAvailSize = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_PSI);
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
                        if(rPidStruct.u4BufSize < _u4OneTimeSize)
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

    if(u4DmxAvailSize < _u4OneTimeSize)
    {
        LOG(9, "_SWDMX_MpgHandleFifoFull SWDMX Demuxer Buffer is not enough.\n");
        fgFull = TRUE;
    }

    if((!_fgGetInfo) && (!fgFull))
    {
        if(_rSwdmxMpgInfo.rFmt.fgVideo)
        {
            VDEC_GetQueueInfo(0, &u2QueueSize, &u2MaxQueueSize);
            if(u2QueueSize > 500)
            {
                fgFull = TRUE;
            }
            else if(
                    /*((u2QueueSize > 30) &&
                     (!_rSwdmxMpgInfo.rVar.fgEnableVideo) &&
                     (_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_VUDU)) ||*/
                    ((u2QueueSize > 180) &&
                     (_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_VUDU)) ||
                    ((u2QueueSize > 30) &&
                     (_rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_NETWORK_VUDU) &&
                     (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                   )
            {
                if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
                    (!_rSwdmxMpgInfo.rVar.fgGotVideo))
                {
                    // There are many frames we send to vdec,
                    // but there is still have no sync point.
                    // continue send to vdec.
                    fgFull = FALSE;
                }
                else if(_rSwdmxMpgInfo.rFmt.fgAudio)
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
                    //[DTV00055801] [AMMP153] MM audio output not smoothly
                    // change from 10000 to 15000
                    if(((u4BufSize - u4DmxAvailSizeAudio) > (15000))
                       &&
                       // in time-shfit, we might in pcr pause stage in begin play
                       // we have to send more data to let STC(pcr) increase.
                       // then video PTS will meat STC and start to play.
                       (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                    {
                        // ToDo: check second audio driver buffer.
                        // maybe don't need this.
                        /*
                        if(_rSwdmxMpgInfo.rFmt.fgAudio2)
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
                    if(!_rSwdmxMpgInfo.rVar.fgGotAudio)
                    {
                        if(fgFull) // not get audio yet
                        {
                            if(u4DmxAvailSize > _u4OneTimeSize)
                            {
                                fgFull = FALSE; // send more
                            }
                        }
                        else // audio fifo doesn't have data
                        {
                            if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT)||                 // [DTV00211805]
                                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS)||                         //
                                (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
                            {
                                // [DTV00054145] [MMP] Video display not smoothly before press MTS twice.
                                // in ts file, there are many DTV ts file that doesn't really have audio
                                // stream. For such case, we can't split audio data out but send many
                                // video data. To limit this situation, only keep 1.5 second in video
                                // esmq.
                                // Later we will have the value comes from MW to know if this is a DTV
                                // TS or general TS file.

                                // We send many video data but less audio data.
                                // a/v interleave is not good. Play video first.

                                /*
                                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT)||                 // [DTV00211805]
                                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS))
                                {
                                    if(u2QueueSize>90)
                                    {
                                        fgFull = TRUE;
                                    }
                                }
                                else
                                {
                                    fgFull = TRUE;
                                }
                                */
                                // DTV00140198, [MM]Can't play the video file smoothly after select it to play
                                // first data of clip is not a/v well interleaved.
                                // enlarge this condition to all format
                                if(u2QueueSize>90)
                                {
                                    fgFull = TRUE;
                                }
                            }
                        }
                    }
                }
                else
                {
                    fgFull = TRUE;
                }
                LOG(9, "_SWDMX_MpgHandleFifoFull Vdec esm queue is not enough.\n");
            }
        }
        else if(_rSwdmxMpgInfo.rFmt.fgAudio // audio only
#ifdef TIME_SHIFT_SUPPORT
                //&&
                //(i4PlaySpeed == 1000)
                &&
                (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
               )
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
                if(_rSwdmxMpgInfo.rFmt.fgAudio2)
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
    // _SWDMX_MpgAudioCallback. So do it again.
    if((!_rSwdmxMpgInfo.rVar.fgGotAudio) ||
    // time-shift will set fgGotAudio but fgCleanAudio is on.
       (_rSwdmxMpgInfo.rVar.fgCleanAudio))
    {
        _SWDMX_MpgFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
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
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if((!_rSwdmxMpgInfo.rVar.fgVideoPause) &&
           (!_rSwdmxMpgInfo.rVar.fgEnableVideo) &&
           (!_rSwdmxMpgInfo.rVar.fgGotVideoReady) &&
           (_rSwdmxMpgInfo.rFmt.fgVideo) &&
           (i4PlaySpeed >= 0) &&
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
           (i4PlaySpeed <= 1500)
#else
           (i4PlaySpeed < 4000)
#endif
          )
        {
            // pause video
            //UNUSED(VDP_SetPauseMm(B2R_1, TRUE));
            //STC_StopStc();           //DTV00144863 When time shift for audio only file ,can't stop STC

            _rSwdmxMpgInfo.rVar.fgVideoPause = TRUE;
            //LOG(3, "video pause  ==============\n");

            // 4x -> 1x, vdec might in pause status. play it.
            if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_MPV fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_H264))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_H264 fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_WMV fail\n");
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
            {
                if(!VDEC_Play(ES0, VDEC_FMT_MP4))
                {
                    LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                }
            }
            else
            {
                ASSERT(0);
            }

        }

        if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
           (_rSwdmxMpgInfo.rVar.fgGotVideo))
        {
            if(!_rSwdmxMpgInfo.rVar.fgEnableVideo)
            {
                _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                _rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
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
        //(_u8FileSize == (_u8ReadedPos + _rSwdmxMpgInfo.rBuf.u4ReqTotalSize + _rSwdmxMpgInfo.rBuf.u4BufTotalSize)))
        // DTV00211080 [MM Compatibility]Progress bar doesn't start from the beginning when play special file
        // we should check start to play after we have send almost all data,
        // but not read all data.
       (fgLast) ||
       // when rendering thumbnail, not wait fifo full to start playback.
       ((_rSwdmxMpgInfo.rFmt.fgVideo) &&
        (!_rSwdmxMpgInfo.rFmt.fgAudio) &&
        (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
        (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
        (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_vid_render_counts > 0)
       )
      )
    {
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                /*if(_rSwdmxMpgInfo.rVar.fgVideoPause &&
                    (((!_rSwdmxMpgInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                      (_rSwdmxMpgInfo.rVar.fgGotAudio &&
                       _rSwdmxMpgInfo.rVar.fgGotAudioReady)) &&
                        (_rSwdmxMpgInfo.rFmt.fgAudio)))
                        */
                BOOL fgAudioReady;
                BOOL fgFeederReady;
                if(!_rSwdmxMpgInfo.rFmt.fgAudio)
                {
                    fgAudioReady = TRUE;
                }
                else
                {
                    fgAudioReady =
                          ((!_rSwdmxMpgInfo.rVar.fgGotAudio) || // error handle, can't fuind audio
                            (_rSwdmxMpgInfo.rVar.fgGotAudio &&
                             _rSwdmxMpgInfo.rVar.fgGotAudioReady));
                }

                fgFeederReady = FALSE;
                #ifdef CC_DLNA_SUPPORT
                if (_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_NETWORK_DLNA)
                {
                    if(_fgSendSeqHdr)
                    {
                        UINT32 u4FeederReadySize = (MPG_DLNA_FEEDER_READY_SIZE);
                        if((_u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE))
                            > (u4FeederReadySize))
                        {
                            FeederSetReadyThreshold(u4FeederReadySize);
                        }
                        else
                        {
                            u4FeederReadySize = (_u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE));
                            FeederSetReadyThreshold(u4FeederReadySize);
                        }
                        //FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNDERFLOW_SIZE);
                        FeederSetUnderFlowThreshold(u4FeederReadySize/8);
                        FeederSetInfo(FEEDER_SWDMX_THRESHOLD, TRUE);
                    }
                    else
                    {
                        FeederSetReadyThreshold(MPG_DLNA_FEEDER_UNREACH);
                        FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNREACH);
                        FeederSetInfo(FEEDER_SWDMX_THRESHOLD, FALSE);
                    }
                    if(FeederIsReady(FEEDER_PROGRAM_SOURCE))
                    {
                        fgFeederReady = TRUE;
                    }
                }
                else
                #endif
                if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_NETWORK_RHAPSODY)
                {
                    fgFeederReady = TRUE;
                }
                else
                //if (_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                {
                    // when audio buffer is 512K
                    // only could only have 1 buffer data (one is u4UsingBufSize)
                    //if(_rSwdmxMpgInfo.rBuf.u4BufAvailCnt >= 2)
                    if(_rSwdmxMpgInfo.rBuf.u4BufAvailCnt >= 1)
                    {
                        fgFeederReady = TRUE;
                    }
                }

                if((_u8FileSize == (_u8ReadedPos + _rSwdmxMpgInfo.rBuf.u4ReqTotalSize
                                    + _rSwdmxMpgInfo.rBuf.u4BufTotalSize)) ||
                   // while vdec is pause then we got eof with vfifo full
                   // we will block here. Add this condition.
                   (_rSwdmxMpgInfo.rBuf.fgGetEof))
                {
                    // File end
                    fgFeederReady = TRUE;
                }

                if((_rSwdmxMpgInfo.rVar.fgVideoPause) && (fgAudioReady)
                    && (fgFeederReady))
                {
                    // DTV00136461 [MM]Can't full display the special ts file.
                    // if there is no video PTS
                    if((!_rSwdmxMpgInfo.rVar.fgGotVideo) &&
                       (!_rSwdmxMpgInfo.rVar.fgVideoPTSFound))
                    {
                        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
                        if(_rSwdmxMpgInfo.rVar.u4FirstPTS)
                        {
                            _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = _rSwdmxMpgInfo.rVar.u4FirstPTS;
                        }
                        else
                        {
                            _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = MPG_STC_BASE;
                        }
                        if((!_rSwdmxMpgInfo.rVar.fgSetStc) &&
                           (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                        {
                            _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                            STC_StopStc();
                            // When there is video stream, stc value will set at vdec_flw.c
                        }
                    }


                    // if there is no video PTS
                    /*
                    if(!_rSwdmxMpgInfo.rVar.fgGotVideo)
                    {
                        // DTV00135223
                        // we never found a video PTS
                        if(_rSwdmxMpgInfo.rVar.u4FirstVideoPTS == (UINT32)0xFFFFFFFF)
                        {
                            _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = _rSwdmxMpgInfo.rVar.u4LastPTS;
                        }
                        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = TRUE;
                        if(!_rSwdmxMpgInfo.rVar.fgSetStc)
                        {
                            _rSwdmxMpgInfo.rVar.fgSetStc = TRUE;
                            STC_StopStc();
                            // When there is video stream, stc value will set at vdec_flw.c
                        }
                    }
                    */

                    /*if(!_rSwdmxMpgInfo.rVar.fgGotVideo)
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
                    UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
                    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                    {
                        if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_MPV fail\n");
                        }
                    }
                    else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                    {
                        if(!VDEC_Play(ES0, VDEC_FMT_H264))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_H264 fail\n");
                        }
                    }
                    else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
                    {
                        if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play VDEC_FMT_WMV fail\n");
                        }
                    }
                    else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                    {
                        if(!VDEC_Play(ES0, VDEC_FMT_MP4))
                        {
                            LOG(3, "_SWDMX_MpgHandleFifoFull VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                        }
                    }
                    else
                    {
                        ASSERT(0);
                    }

                    _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                    _rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
                    _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                    /* This part will be done when vdp display first video frame.
                    AUD_MMAoutEnable(0, TRUE);
                    _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                    */
                }

                // DTV00136461 [MM]Can't full display the special ts file.
                // audio full without triger again
                if((fgFull) && (fgAudioSmaller))
                {
                    VDEC_GetQueueInfo(0, &u2QueueSize, &u2MaxQueueSize);
                    if(u2QueueSize == 0)
                    {
                        AUD_MMAoutEnable(0, TRUE);
                        _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                        LOG(3, "_SWDMX_MpgHandleFifoFull AUD_MMAoutEnable video empty\n");
                    }

                }
            }
            else if((!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                    (!_rSwdmxMpgInfo.rVar.fgEnableAudio) &&
                    (_rSwdmxMpgInfo.rVar.fgGotAudioReady))// audio only
            {
                #ifdef CC_DLNA_SUPPORT
                // DTV00212740, audio notify no data
                // audio should be pause when data underflow
                if (_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_NETWORK_DLNA)
                {

                    if((_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry))
                    {
                        // we can't generate audio index size, we will block in
                        // buf count is full but data is still not full enough
                        FeederSetReadyThreshold(MPG_DLNA_INDEX_FEEDER_READY_SIZE);
                        FeederSetUnderFlowThreshold(MPG_DLNA_INDEX_FEEDER_UNDERFLOW_SIZE);
                    }
                    else
                    {
                        UINT32 u4FeederReadySize = (MPG_DLNA_FEEDER_READY_SIZE);
                        if((_u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE))
                            > (u4FeederReadySize))
                        {
                            FeederSetReadyThreshold(u4FeederReadySize);
                        }
                        else
                        {
                            u4FeederReadySize = (_u4FeederBufNum*(MPG_BATCH_SIZE*MPG_BATCH_READ_SIZE));
                            FeederSetReadyThreshold(u4FeederReadySize);
                        }
                        //FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNDERFLOW_SIZE);
                        FeederSetUnderFlowThreshold(u4FeederReadySize/8);
                    }
                    FeederSetInfo(FEEDER_SWDMX_THRESHOLD, TRUE);
                }
                #endif

                AUD_MMAoutEnable(0, TRUE);
                STC_StartStc();
                _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
            }
        }
        if(fgFull)
        {
            return MPG_SEND_FULL;
        }
    }

    return MPG_SEND_OK;
}



INT32 _SWDMX_MpgHandleMoveData(DMX_MM_DATA_T* prDmxMMData, INT32 i4PlaySpeed)
{
    BOOL fgRet;
    INT32 i4Ret;
#ifdef TIME_SHIFT_SUPPORT
    UINT32 i;
    SWDMX_PVR_SENDDATA_RET_T ePVRRet;
    UINT32 u4MinSize = 0;
    UINT32 u4Temp = 0;
#endif
    if(!prDmxMMData)
    {
        LOG(1, "prDmxMMData null\n");
        return MPG_SEND_FAIL;
    }

    #ifdef CC_MT5395
    if((prDmxMMData->u4StartAddr < prDmxMMData->u4BufEnd) &&
       ((prDmxMMData->u4StartAddr + prDmxMMData->u4FrameSize) >= prDmxMMData->u4BufEnd))
    {
        HalFlushDCacheMultipleLine(prDmxMMData->u4StartAddr, (prDmxMMData->u4BufEnd - prDmxMMData->u4StartAddr));
        HalFlushDCacheMultipleLine(prDmxMMData->u4BufStart, (prDmxMMData->u4FrameSize - (prDmxMMData->u4BufEnd - prDmxMMData->u4StartAddr)));
    }
    else
    {
        HalFlushDCacheMultipleLine(prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
    }
    #else
    HalFlushInvalidateDCache();
    #endif

    _rSwdmxMpgInfo.rVar.fgCleanAudio = FALSE;
    i4Ret = MPG_SEND_OK;
    UNUSED(fgRet);

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        // ToDo: Handle 0x47 sync byte
        #if 1
        if((*(UCHAR*)VIRTUAL(prDmxMMData->u4StartAddr+4)) != 0x47)
        {
            LOG(3, "Move !0x47\n");
            ASSERT(_rSwdmxMpgInfo.rVar.u8FifoSize);
            if(_rSwdmxMpgInfo.rVar.u8FifoSize)
            {
                UINT64 u8Tmp;
                UINT64 u8Remainder;
                u8Tmp = _SWDMX_Div6464((_u8SendedSize - _rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)192, &u8Remainder);
                UNUSED(_SWDMX_Div6464((_u8SendedSize - _rSwdmxMpgInfo.rVar.u8FifoOffset), _rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));

                LOG(3, "!0x47, FileOffset 0x%llx, LBA 0x%llx, FilePos 0x%llx\n",
                    _rSwdmxMpgInfo.rVar.u8FifoOffset,
                    (UINT64)u8Tmp,
                    (UINT64)(u8Remainder + _rSwdmxMpgInfo.rVar.u8FifoOffset)
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
        if((_rSwdmxMpgInfo.rVar.u4ValidEndTick >= _rSwdmxMpgInfo.rVar.u4CurTickNum) &&
           ((_rSwdmxMpgInfo.rVar.u4ValidEndTick - _rSwdmxMpgInfo.rVar.u4CurTickNum) <=
             MPG_TIME_SHIFT_SAFE_RANGE_END_THRESHOLD))
        {
            _rSwdmxMpgInfo.rVar.fgOutOfSafeRange = TRUE;
        }
        */

        for(i=0;i<(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1);i++)
        {
            if((_rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] == 0) ||
               (_rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1] == 0))
            {
                break;
            }
            else
            {
                u4Temp = (UINT32)(_rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1] -
                    _rSwdmxMpgInfo.rVar.u8QueueStartAddr[i]);
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
           ((_rSwdmxMpgInfo.rFmt.fgVideo) ||
            (_rSwdmxMpgInfo.rFmt.fgAudio))
            #if 0
           &&
            // can't use fast move when audio only, swdmx speed is too fast
           (
            // video with audio also have this problem
            //(!_rSwdmxMpgInfo.rFmt.fgVideo)
            //||
            // can't use fast move when out of safe range
            (_rSwdmxMpgInfo.rVar.fgOutOfSafeRange)
             #if 1 // enable this to speed up Nx -> 1x speed
           )
             #endif
             #endif
          )
        {
            //LOG(3, "_SWDMX_PVR_SendData normal _u8SendedSize(0x%llx)\n", _u8SendedSize);
            /*
            if(_rSwdmxMpgInfo.rVar.fgOutOfSafeRange)
            {
                _rSwdmxMpgInfo.rVar.fgOutOfSafeRange = FALSE;
            }*/
            ePVRRet = _SWDMX_PVR_SendData(prDmxMMData, u4MinSize, FALSE);
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
            //LOG(3, "_SWDMX_PVR_SendData fast _u8SendedSize(0x%llx)\n", _u8SendedSize);
            ePVRRet = _SWDMX_PVR_SendData(prDmxMMData, u4MinSize, TRUE);
            if(ePVRRet != SWDMX_PVR_SENDDATA_OK)
            {
                return MPG_SEND_FAIL;
            }

        }
        // this is tick size < _u4OneTimeSize
        if(_rSwdmxMpgInfo.rVar.u4QueueTickNum !=
            _rSwdmxMpgInfo.rVar.u4CurTickNum)
        {
            _rSwdmxMpgInfo.rVar.u4QueueTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;
            for(i=0;i<(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1);i++)
            {
                _rSwdmxMpgInfo.rVar.u8QueueStartAddr[i] =
                    _rSwdmxMpgInfo.rVar.u8QueueStartAddr[i+1];
            }
            _rSwdmxMpgInfo.rVar.u8QueueStartAddr[(MPG_TIME_SHIFT_ADDRESS_CHECK_NUMBER-1)] =
                _u8SendedSize;
        }
    }
    else
#endif
    if (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(_rSwdmxMpgInfo.rFmt.fgAudio)
        {
            fgRet = DMX_MM_MoveData(_u1PidxAudio, prDmxMMData, 1000);
            LOG(7, "_SWDMX_MpgHandleMoveData DMX_MM_MoveData 0x%x, S=%d.\n",
                prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
            if(!fgRet)
            {
                LOG(5, "_SWDMX_MpgHandleMoveData DMX_MM_MoveData Fail.\n");
                i4Ret = MPG_SEND_FAIL;
            }
        }
        else               //DTV00214170,  For ES audio only file swdmx can't send data before audio stream enable
        {
        	LOG(5, "_SWDMX_MpgHandleMoveData DMX_MM_MoveData Fail audio didn't enable.\n");
        	i4Ret = MPG_SEND_FAIL;
        }
    }
    else
    {
        fgRet = DMX_DDI_MoveData(prDmxMMData);
        LOG(9, "_SWDMX_MpgHandleMoveData _DMX_DDI_MoveData 0x%x, S=%d.\n",
            prDmxMMData->u4StartAddr, prDmxMMData->u4FrameSize);
        if(!fgRet)
        {
            LOG(5, "_SWDMX_MpgHandleMoveData DMX_DDI_MoveData Fail.\n");
            i4Ret = MPG_SEND_FAIL;
        }
    }

    if(_rSwdmxMpgInfo.rVar.fgCleanAudio)
    {
        _SWDMX_MpgFlushAFifo();
        if(_u1PidxAudio != 0xFF)
        {
            UNUSED(DMX_MM_FlushBuffer(_u1PidxAudio));
        }
        _rSwdmxMpgInfo.rVar.fgCleanAudio = FALSE;
    }

    UNUSED(i4PlaySpeed);
    return i4Ret;
}


//#define TickMethod
UINT32 _SWDMX_MpgHandleAudioIndex(UINT64 u8StartAddr, UINT32 u4ReadSize,
    UINT64 *pu8OutStartAddr, UINT32 *pu4OutReadSize, UINT32 *pu4OutReadPTS, INT32 i4PlaySpeed)
{
    *pu8OutStartAddr = u8StartAddr;
    *pu4OutReadSize = u4ReadSize;
    *pu4OutReadPTS = 0;
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        return MPG_AUDIO_INDEX_OK;
    }
    if((_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst)&&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
      )
    {
        MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
        {
            UINT64 u8IdxBaseAddr;
            UINT64 u8IdxStartAddr;
            UINT64 u8IdxEndAddr;
            u8IdxBaseAddr =
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->ui8_base_offset;
            prIdxTbl = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
            prTblEntry = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
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
                    if(_rSwdmxMpgInfo.rVar.u4CurIndexNum >= prIdxTbl->ui4_number_of_entry)
                    {
                        _rSwdmxMpgInfo.rVar.u4CurIndexNum = 0;
                    }

                    u8IdxEndAddr = u8IdxBaseAddr +
                        (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset +
                        (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                    if(((u8StartAddr) >=
                        (u8IdxBaseAddr +
                         prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset
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
                    _rSwdmxMpgInfo.rVar.u4CurIndexNum++;
                    u4Cnt++;
                }
                if(u4Cnt >= prIdxTbl->ui4_number_of_entry)
                {
                    *pu4OutReadSize = 0;
                    LOG(3, "_SWDMX_MpgHandleAudioIndex out of index num (from %d)\n", _rSwdmxMpgInfo.rVar.u4CurIndexNum);
                    return MPG_AUDIO_INDEX_OK;
                }
                u8IdxStartAddr = u8IdxBaseAddr +
                    (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_relative_offset;

                u8IdxEndAddr = u8IdxStartAddr + (UINT64)prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_size;

                //if(u8IdxStartAddr == u8StartAddr)	
                if(u8IdxStartAddr <= u8StartAddr)  //for handle jumpping to normal get  pts =0
                {
                    *pu4OutReadPTS = prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_pts;
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
                    _rSwdmxMpgInfo.rVar.u4CurIndexNum++;
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
        if ((_rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
            (_u8FileSize > 0))
        {
            UINT64 u8Tmp;
            UINT64 u8SendPos;
            UINT64 u8Remainder;
            // 90000 -> 90000
            u8Tmp = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset) * (UINT64)90000,
                              _rSwdmxMpgInfo.rVar.u4TotalTime,
                              &u8Remainder);
            if(u8Tmp > 0)
            {
                //DTV00141518, [MM]Can not display current time when play special .mp3 file.
                // This is call from handlefeeder, we should consider buffer status
                // Or the position is updated late.
                u8SendPos = (_u8SendedSize + (UINT64)_rSwdmxMpgInfo.rBuf.u4BufTotalSize
                    + (UINT64)_rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
                if(u8SendPos >= _u8StartFileOffset)
                {
                    u4Time = (UINT32)_SWDMX_Div6464((UINT64)(u8SendPos - _u8StartFileOffset) * (UINT64)90000,
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
UINT32 _SWDMX_MpgHandleRing(UINT64 u8StartAddr, UINT32 u4ReadSize,
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

    if((pu8OutStartAddr == NULL) || (pu4OutReadSize == NULL))
    {
        return MPG_RING_FAIL;
    }

#ifdef TickMethod
    u4ReadTick = 0;
#endif
    fgRet = TRUE;
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8OutStartAddr = u8StartAddr;
        *pu4OutReadSize = u4ReadSize;
        return MPG_RING_OK;
    }
    else//if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_rSwdmxMpgInfo.rVar.fgValidRange)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            LOG(9, "_SWDMX_MpgHandleRing valid range is not set\n");
            return MPG_RING_FAIL;
        }

        u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;

        ASSERT(_rSwdmxMpgInfo.rVar.u8FifoSize);
        if(_rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            return MPG_RING_FAIL;
        }

        if(i4PlaySpeed >= 0)
        {
            fgRet = _SWDMX_MpgGetPosByTickNum(
                FALSE,
                (_rSwdmxMpgInfo.rVar.u4ReadTickNum),
                &u8Temp);

            if(fgRet)
            {
                if(u8EndAddr > u8Temp)
                {
                    u8EndAddr = _SWDMX_MpgTSAlignDown(u8Temp);
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
                        _rSwdmxMpgInfo.rVar.u4ReadTickNum++;
                        if((_rSwdmxMpgInfo.rVar.u4ReadTickNum + 200) <
                            _rSwdmxMpgInfo.rVar.u4CurTickNum)
                        {
                            LOG(3, "u4ReadTickNum + 200 < u4CurTickNum\n");
                            _rSwdmxMpgInfo.rVar.u4ReadTickNum += 100;
                        }
                    }
                }
            }
        }

#ifdef TickMethod
        fgRet = _SWDMX_MpgGetTickByPos(
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
                        u8EndAddr = _SWDMX_MpgTSAlignDown(u8Temp);
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
            u4ReadTick = _rSwdmxMpgInfo.rVar.u4CurTickNum;
        }
#endif

        u8Temp = _SWDMX_Div6464((u8StartAddr - _rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)_rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder);
        u8EndAddr = _SWDMX_Div6464((u8EndAddr - _rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)_rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder);
        if(u8EndAddr == u8Temp)
        {
            u8EndAddr = u8StartAddr + (UINT64)u4ReadSize;
        }
        else
        {
            u8EndAddr = (u8EndAddr * _rSwdmxMpgInfo.rVar.u8FifoSize) +
                _rSwdmxMpgInfo.rVar.u8FifoOffset;
            u4ReadSize = (u8EndAddr - u8StartAddr);
        }

        if(_prSwdmxInfo)
        {
            if((_rSwdmxMpgInfo.rVar.fgMonitorLBA)
                &&
               // while we are change, mw will set new monitor addr
               // but we are still in old speed
               // we should make sure the speed information is synced.
               (!_prSwdmxInfo->fgPlayChange) // not changing speed
                &&
               (
                ((i4PlaySpeed >= 0) &&
                 (u8EndAddr >= _rSwdmxMpgInfo.rVar.u8MonitorAddr))
                ||
                ((i4PlaySpeed < 0) &&
                 (u8StartAddr < _rSwdmxMpgInfo.rVar.u8MonitorAddr))
               )
              )
            {
                VDP_PIC_INFO_T rVdpPicInfo;
                UINT64 u8DispOffset = 0;
                if(i4PlaySpeed > 0)
                {
                    u8DispOffset = (UINT64)-1;
                }
                if(_rSwdmxMpgInfo.rFmt.fgVideo)
                {
                    if (VDP_GetPicInfo(B2R_1, &rVdpPicInfo) == VDP_SET_OK)
                    {
                        u8DispOffset = rVdpPicInfo.u8OffsetDisp;
                    }
                }

                if(
                   (
                    ((i4PlaySpeed >= 0) &&
                     (u8DispOffset >= _rSwdmxMpgInfo.rVar.u8MonitorAddr))
                    ||
                    ((i4PlaySpeed < 0) &&
                     (u8DispOffset < _rSwdmxMpgInfo.rVar.u8MonitorAddr))
                   )
                  )
                {
                    LOG(3, "fgMonitorLBA addr 0x%llx, start addr 0x%llx end addr 0x%llx, u8DispOffset 0x%llx\n",
                        _rSwdmxMpgInfo.rVar.u8MonitorAddr,
                        u8StartAddr,
                        u8EndAddr,
                        u8DispOffset);
                    if((_prSwdmxInfo) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                    {
                        if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                        {
                            if(_prSwdmxInfo->pfnSwdmxRangeCb)
                            {
                                _prSwdmxInfo->pfnSwdmxRangeCb(
                                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                    MM_MONITOR_LBA_ARRIVAL,
                                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                    0xFFFFFFFF);
                            }
                            else
                            {
                                LOG(3, "pfnSwdmxRangeCb null\n");
                            }
                        }
                    }
                    _rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgHandleRing _prSwdmxInfo null\n");
        }
#ifdef TickMethod
        if((u4ReadTick + 1) >=
            _rSwdmxMpgInfo.rVar.u4SafeEndTick)
#else
        if(u8EndAddr >=
            (_rSwdmxMpgInfo.rVar.u8SafeEndAddr))
#endif
        {
            // allow this position
            //*pu8OutStartAddr = u8StartAddr;
            //*pu4OutReadSize = 0;

            if((_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
               (i4PlaySpeed >= 0))
               //&&
               // codition is _u8SendedSize, not _u8ReadPos
               // Then we will notify on playback to this position.
               // -> rollback to original method
               //(_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8SafeEndAddr))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorSafeEnd tick + 1 %d safe end tick %d\n",
                    (u4ReadTick + 1),
                    _rSwdmxMpgInfo.rVar.u4SafeEndTick);
#else
                LOG(3, "fgMonitorSafeEnd addr 0x%llx safe end addr 0x%llx\n",
                    u8EndAddr,
                    _rSwdmxMpgInfo.rVar.u8SafeEndAddr);
#endif
                if((_prSwdmxInfo) &&
                   (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        if(_prSwdmxInfo->pfnSwdmxRangeCb)
                        {
                            _prSwdmxInfo->pfnSwdmxRangeCb(
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_OUT_OF_SAFE_RANGE_END,
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF);
                        }
                        else
                        {
                            LOG(3, "pfnSwdmxRangeCb null\n");
                        }
                    }
                }
                _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
            }
            // allow this position
            //if(i4PlaySpeed > 0)
            //{
            //    u4Ret = MPG_RING_FAIL;
            //}
        }
        else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
        {
            _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
        }

#ifdef TickMethod
        if((u4ReadTick) <
            _rSwdmxMpgInfo.rVar.u4SafeStartTick)
#else
        if((u8StartAddr) <
            (_rSwdmxMpgInfo.rVar.u8SafeStartAddr))
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            if((_rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
               (i4PlaySpeed < 0))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorSafeStart tick %d safe end tick %d\n",
                    (u4ReadTick),
                    _rSwdmxMpgInfo.rVar.u4SafeStartTick);
#else
                LOG(3, "fgMonitorSafeStart addr 0x%llx safe start addr 0x%llx\n",
                    u8StartAddr,
                    _rSwdmxMpgInfo.rVar.u8SafeStartAddr);
#endif
                if((_prSwdmxInfo) &&
                   (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        if(_prSwdmxInfo->pfnSwdmxRangeCb)
                        {
                            _prSwdmxInfo->pfnSwdmxRangeCb(
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_OUT_OF_SAFE_RANGE_START,
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF);
                        }
                        else
                        {
                            LOG(3, "pfnSwdmxRangeCb null\n");
                        }
                    }
                }
                _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
            }
            if(i4PlaySpeed < 0)
            {
                u4Ret = MPG_RING_FAIL;
            }
        }
        else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
        {
            _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
        }

#ifdef TickMethod
        if((u4ReadTick + 1) >=
            _rSwdmxMpgInfo.rVar.u4ValidEndTick)
#else
        if(u8EndAddr >= _rSwdmxMpgInfo.rVar.u8ValidEndAddr)
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            u4ReadSize = 0;

            if((_rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
               (i4PlaySpeed >= 0))
               //&&
               // codition is _u8SendedSize, not _u8ReadPos
               // Then we will notify on playback to this position.
               //(_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8ValidEndAddr))
               // we never out of valid range when we check with _u8SendedSize
               // -> rollback to original method
               //(_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8SafeEndAddr))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorValidEnd tick + 1 %d valid end tick %d\n",
                    (u4ReadTick + 1),
                    _rSwdmxMpgInfo.rVar.u4ValidEndTick);
#else
                LOG(3, "fgMonitorValidEnd addr 0x%llx valid end addr 0x%llx\n",
                    u8EndAddr,
                    _rSwdmxMpgInfo.rVar.u8ValidEndAddr);
#endif
                if((_prSwdmxInfo) &&
                   (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        if(_prSwdmxInfo->pfnSwdmxRangeCb)
                        {
                            _prSwdmxInfo->pfnSwdmxRangeCb(
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_REACH_VALID_RANGE_END,
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF);
                        }
                        else
                        {
                            LOG(3, "pfnSwdmxRangeCb null\n");
                        }
                    }
                }
                _rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
            }

            u4Ret = MPG_RING_FAIL;

            // if we are doing backward
            //and there is no signal anymore (u8ValidEndAddr)
            if((_rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
               (i4PlaySpeed < 0) &&
               (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                u4Ret = MPG_RING_END_MEET;
                _rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
            }
        }

#ifdef TickMethod
        if((u4ReadTick) <
            _rSwdmxMpgInfo.rVar.u4ValidStartTick)
#else
        if(u8StartAddr < _rSwdmxMpgInfo.rVar.u8ValidStartAddr)
#endif
        {
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            if((_rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
               (i4PlaySpeed < 0))
            {
#ifdef TickMethod
                LOG(3, "fgMonitorValidStart tick %d valid start tick %d\n",
                    (u4ReadTick),
                    _rSwdmxMpgInfo.rVar.u4ValidStartTick);
#else
                LOG(3, "fgMonitorValidStart addr 0xllx valid start addr 0x%llx\n",
                    u8StartAddr,
                    _rSwdmxMpgInfo.rVar.u8ValidStartAddr);
#endif
                if((_prSwdmxInfo) &&
                   (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                {
                    if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                    {
                        if(_prSwdmxInfo->pfnSwdmxRangeCb)
                        {
                            _prSwdmxInfo->pfnSwdmxRangeCb(
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_REACH_VALID_RANGE_START,
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF);
                        }
                        else
                        {
                            LOG(3, "pfnSwdmxRangeCb null\n");
                        }
                    }
                }
                _rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
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

        ASSERT(_rSwdmxMpgInfo.rVar.u8FifoSize);
        if(_rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            // valid range is not set
            *pu8OutStartAddr = u8StartAddr;
            *pu4OutReadSize = 0;
            return MPG_RING_FAIL;
        }
        UNUSED(_SWDMX_Div6464((u8StartAddr - _rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)_rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));
        u8Temp = u8Remainder;

        *pu8OutStartAddr =
            u8Temp + _rSwdmxMpgInfo.rVar.u8FifoOffset;
        *pu4OutReadSize = u4ReadSize;

        if((u4Ret == MPG_RING_OK) &&
           (_rSwdmxMpgInfo.rVar.fgFirst) &&
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

INT32 _SWDMX_MpgHandleFeeder(SWDMX_RANGE_LIST_T* prRangeList, INT32 i4PlaySpeed)
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

    u8ReadAddr = (_u8ReadedPos+_rSwdmxMpgInfo.rBuf.u4ReqTotalSize+_rSwdmxMpgInfo.rBuf.u4BufTotalSize);
    u4ReadSize = _u4OneTimeReadSize;
    u4ReadPTS = 0;
#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        UINT32 u4Ret;
        UINT32 u4OutReadSize;
        UINT64 u8OutReadAddr;
        u4Ret = _SWDMX_MpgHandleRing(u8ReadAddr, u4ReadSize,
            &u8OutReadAddr, &u4OutReadSize, i4PlaySpeed);
        if(u4Ret == MPG_RING_OK)
        {
            u8ReadAddr = u8OutReadAddr;
            u4ReadSize = u4OutReadSize;
        }
        else
        {
            if ((_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
                (_rSwdmxMpgInfo.rBuf.u4ReqTimes == 0) &&
                (_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
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
    if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
        u8ReadAddr = 0;
        u4ReadSize = 1;
        u4ReadPTS = 0;
    }
    else
    {
        if((UINT64)u4ReadSize > ((_u8FileSize - (_u8ReadedPos + _rSwdmxMpgInfo.rBuf.u4ReqTotalSize + _rSwdmxMpgInfo.rBuf.u4BufTotalSize))))
        {
            u4ReadSize = ((UINT32)(_u8FileSize - (_u8ReadedPos + _rSwdmxMpgInfo.rBuf.u4ReqTotalSize + _rSwdmxMpgInfo.rBuf.u4BufTotalSize)));
            #if 1//def CC_DLNA_SUPPORT
            if((u4ReadSize == 0) &&
               (_rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE))
            {
                FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
            }
            #endif
        }
        if((u4ReadSize > 0) &&
           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
        {
            UINT32 u4Ret;
            UINT32 u4OutReadSize;
            UINT32 u4OutReadPTS;
            UINT64 u8OutReadAddr;

            u4Ret = _SWDMX_MpgHandleAudioIndex(u8ReadAddr, u4ReadSize,
                &u8OutReadAddr, &u4OutReadSize, &u4OutReadPTS, i4PlaySpeed);
            if(u4Ret == MPG_AUDIO_INDEX_OK)
            {
                u8ReadAddr = u8OutReadAddr;
                u4ReadSize = u4OutReadSize;
                u4ReadPTS = u4OutReadPTS;
            }
        }
    }

    fgBlockReq = ((_fgGetInfo) ||
        // for audio only file, we also could use multi-request
        ((!_fgSendSeqHdr) && (_rSwdmxMpgInfo.rFmt.fgVideo)) ||
        ((i4PlaySpeed < 0) ||
#ifdef VDEC_TIME_SHIFT_2X_I_MODE
        ((i4PlaySpeed > 1500)
         &&
         (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
        ||
        ((i4PlaySpeed >= 4000)
         &&
         (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT))));
#else
        (i4PlaySpeed >= 4000))); // 4x use BlockReq
#endif

    //fgBlockReq = ((_fgGetInfo) || (!_fgSendSeqHdr)); // 4x use multi request
    //fgBlockReq = TRUE; // request one and wait to got one.

#if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
   if(!_rSwdmxMpgInfo.rBuf.fgGetEof)
   {
#endif
    if(fgBlockReq)
    {
        if ((u4ReadSize > 0) &&
            (_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
            (_rSwdmxMpgInfo.rBuf.u4ReqTimes == 0) &&
            (_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
        {
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            UINT32 u4i;
            for(u4i=0;u4i<2;u4i++)
            {
            #endif

            //LOG(3, "Mpg Fd 1 FeederSetRequest size(%d)\n", u4ReadSize);
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                    u4ReadSize,
                    (UINT64)u8ReadAddr,
                    &rFeederCond,
                    _u4FeederReqID);
            _u4FeederReqID+=2;
            if(i4Ret != FEEDER_E_OK)
            {
                if(i4Ret == FEEDER_E_BUFFER_FULL)
                {
                    // workaround for buffer full
                    _rSwdmxMpgInfo.rBuf.u4ReqBufFullCount++;
                    if(_rSwdmxMpgInfo.rBuf.u4ReqBufFullCount <= (UINT32)MPG_REQ_BUFFULL_NUM)
                    {
                        LOG(1, "Mpg Fd FeederSetRequest Buffer Full\n");
                        return MPG_SEND_FULL;
                    }
                }
                _rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;

#ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_DATA_READ_ERROR,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    LOG(1, "Mpg Fd FeederSetRequest Fail(%d)\n", (UINT32)i4Ret);
                    UNUSED(_SWDMX_MpgResetFeederReq());
                    return MPG_SEND_FAIL;
                }
#endif

                LOG(1, "Mpg Fd FeederSetRequest Fail\n");
                _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                if(!_fgGetInfo)
                {
                    _SWDMX_MpgHandleEOS(i4PlaySpeed);
                }
                if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
                if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
                   (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
                   (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
                }
                return MPG_SEND_FAIL;
            }
            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;
            _rSwdmxMpgInfo.rBuf.au4ReqPTS[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadPTS;
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
            _rSwdmxMpgInfo.rBuf.au4ReqFileOffset[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u8ReadAddr;
#endif
            _rSwdmxMpgInfo.rBuf.au4ReqId[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = (_u4FeederReqID-2);
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            _rSwdmxMpgInfo.rBuf.au4ReqDup[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] =
                  u4i;
#endif
            _rSwdmxMpgInfo.rBuf.u4ReqTimes++;
            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(7, "Mpg Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS 0x%llx RPTS %d Id %d\n",
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                _rSwdmxMpgInfo.rBuf.u4ReqTimes,
                u4ReadSize,
                u8ReadAddr,
                u4ReadPTS,
                (_u4FeederReqID-2));
            #endif
            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx++;
            if(_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx >= _u4FeederReqNum)
            {
                _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
            }
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            }
            #endif
        }
        else
        {
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(9, "Mpg Fd Use UB %d BT %d RT %d\n",
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                _rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
    else
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(1, "Mpg Fd input buffer error.\n");
            return FALSE;
        }
        if ((u4ReadSize > 0) &&
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            ((_rSwdmxMpgInfo.rBuf.u4ReqTimes + 1) < _u4FeederReqNum) &&
            ((_rSwdmxMpgInfo.rBuf.u4ReqTimes + 1 + _rSwdmxMpgInfo.rBuf.u4BufAvailCnt) < _u4FeederBufNum) &&
            #else
            (_rSwdmxMpgInfo.rBuf.u4ReqTimes < _u4FeederReqNum) &&
            ((_rSwdmxMpgInfo.rBuf.u4ReqTimes + _rSwdmxMpgInfo.rBuf.u4BufAvailCnt) < _u4FeederBufNum) &&
            #endif
            //((_rSwdmxMpgInfo.rBuf.u4UsingBufSize + _rSwdmxMpgInfo.rBuf.u4ReqTotalSize +
            //  _rSwdmxMpgInfo.rBuf.u4BufTotalSize + u4ReadSize) <=
            // (_u4OneTimeReadSize*_u4FeederBufNum)))
            // feeder now is 512 K in audio es case
            ((_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH) ||
             (_rFeederInfo.u4FreeSize > (_u4OneTimeReadSize + 1024)))
           )
        {
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            UINT32 u4i;
            for(u4i=0;u4i<2;u4i++)
            {
            #endif

            //LOG(3, "Mpg Fd 2 FeederSetRequest size(%d)\n", u4ReadSize);
            i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                    u4ReadSize,
                    (UINT64)u8ReadAddr,
                    &rFeederCond,
                    _u4FeederReqID);
            _u4FeederReqID+=2;
            if(i4Ret != FEEDER_E_OK)
            {
                if(i4Ret == FEEDER_E_BUFFER_FULL)
                {
                    // workaround for buffer full
                    _rSwdmxMpgInfo.rBuf.u4ReqBufFullCount++;
                    if(_rSwdmxMpgInfo.rBuf.u4ReqBufFullCount <= (UINT32)MPG_REQ_BUFFULL_NUM)
                    {
                        LOG(1, "Mpg Fd FeederSetRequest Buffer Full\n");
                        return MPG_SEND_FULL;
                    }
                }
                _rSwdmxMpgInfo.rBuf.u4ReqBufFullCount = 0;

#ifdef TIME_SHIFT_SUPPORT
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                {
                    if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_DATA_READ_ERROR,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    LOG(1, "Mpg Fd FeederSetRequest Fail(%d)\n", (UINT32)i4Ret);
                    UNUSED(_SWDMX_MpgResetFeederReq());
                    return MPG_SEND_FAIL;
                }
#endif

                LOG(1, "Mpg Fd FeederSetRequest Fail\n");
                _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                if(!_fgGetInfo)
                {
                    _SWDMX_MpgHandleEOS(i4PlaySpeed);
                }
                if(prRangeList)
                {
                    prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                }
                if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
                   (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
                   (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
                }
                return MPG_SEND_FAIL;
            }
            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadSize;
            _rSwdmxMpgInfo.rBuf.au4ReqPTS[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u4ReadPTS;
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
            _rSwdmxMpgInfo.rBuf.au4ReqFileOffset[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = u8ReadAddr;
#endif
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            _rSwdmxMpgInfo.rBuf.au4ReqDup[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] =
                  u4i;
#endif
            _rSwdmxMpgInfo.rBuf.au4ReqId[_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx] = (_u4FeederReqID-2);
            _rSwdmxMpgInfo.rBuf.u4ReqTimes++;
            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize += u4ReadSize;
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(7, "Mpg Fd Set UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPS 0x%llx RPTS %d Id %d\n",
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                _rSwdmxMpgInfo.rBuf.u4ReqTimes,
                u4ReadSize,
                u8ReadAddr,
                u4ReadPTS,
                (_u4FeederReqID-2));
            #endif
            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx++;
            if(_rSwdmxMpgInfo.rBuf.u4ReqWriteIdx >= _u4FeederReqNum)
            {
                _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
            }
            #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
            }
            #endif
        }
        else
        {
            #ifdef SWDMX_MPG_DEBUG_BUFFER
            LOG(9, "Mpg Fd Use UB %d BT %d RT %d\n",
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                _rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
            #endif
        }
    }
#if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
   }
#endif
    #ifdef SWDMX_STATISTICS
    HAL_GetTime(&_rFeederReqStartTime);
    #endif

    if((_rSwdmxMpgInfo.rBuf.u4ReqTimes) ||
        (_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0) || (_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0))
    {
        //if((_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0) || (_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0))
        if(_rSwdmxMpgInfo.rBuf.u4ReqTimes > 0)
        {
            u4Count = (5 + _u4FeederReqNum); // the times try to remove previous feeder's ack
            while(u4Count--)
            {
                if((_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0) && (_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0))
                //if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
                {
                    fgRet = _SWDMX_ReceiveFeederAck((VOID*)&rFeederReqData);
                }
                else
                {
                    fgRet = TRUE;
                    i4Ret = _SWDMX_ReceiveFeederAckNoWait((VOID*)&rFeederReqData);
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
                    LOG(5, "Mpg Fd FEEDER_SOURCE_INVALID.\n");
                    // Feeder add error code for us.
                    if(fgRet==OSR_TIMEOUT)
                    {
                        // workaround for reset feeder request
                        _rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount++;
                        if(_rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount > (UINT32)MPG_REQ_TIMEOUT_NUM)
                        {
                            UNUSED(_SWDMX_MpgResetFeederReq());
                            _rSwdmxMpgInfo.rBuf.u4ReqTimeoutCount = 0;
                            //reset request CR [DTV00211387]
                            LOG(3, "Mpg Fd Timeout count meet\n");
                        }
                    }
                    return MPG_SEND_FULL;
                }
                // Feeder add error code for us. u4WriteAddr = 0 is error
                if(rFeederReqData.u4WriteAddr == 0)
                {
                    LOG(5, "Mpg Fd u4WriteAddr = 0. Reset request.\n");
                    UNUSED(_SWDMX_MpgResetFeederReq());         //reset request CR [DTV00211387]
                    return MPG_SEND_FULL;
                }
                // Linux NetApp want to have eos
                #if 1//def CC_DLNA_SUPPORT                                 //for DLNA handle EOF
                if(rFeederReqData.fgEof)                                      // get EOF from feeder
                {
                    _rSwdmxMpgInfo.rBuf.fgGetEof=TRUE;
                    #if 1//def CC_DLNA_SUPPORT
                    if(_rSwdmxMpgInfo.rVar.e_mm_src_type !=
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                    {
                        FeederSetInfo(FEEDER_SWDMX_EOS, TRUE);
                    }
                    #endif
                    LOG(5,"get Eof from feeder\n ");
                }
                #endif

                if((!fgRet)
            #if 1//ndef CC_DLNA_SUPPORT                                  //for DLNA handle EOF
              || ((rFeederReqData.u4ReadSize == 0) &&                        // "EOF" has been got  from feeder with nomore  other data
                  (_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                        MM_SRC_TYPE_HIGH_SPEED_STORAGE))
             #endif                                                                       // rFeederReqData.u4ReadSize =0
              )
                {
#ifdef TIME_SHIFT_SUPPORT
                    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                    {
                        if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
                           (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
                           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
                        {
                            _prSwdmxInfo->pfnSwdmxRangeCb(
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                MM_DATA_READ_ERROR,
                                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                0xFFFFFFFF);
                        }
                        LOG(1, "Mpg Fd _SWDMX_ReceiveFeederAck Fail\n");
                        UNUSED(_SWDMX_MpgResetFeederReq());
                        return MPG_SEND_FAIL;
                    }
#endif
                    LOG(3, "Mpg Fd _SWDMX_ReceiveFeederAck Fail.\n");
                    _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                    if(!_fgGetInfo)
                    {
                        _SWDMX_MpgHandleEOS(i4PlaySpeed);
                    }
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                    }
                    if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
                       (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                       // DTV00211818
                       // when audio es playback.
                       // pause -> seek -> seek -> play
                       // we will send finish event. this is not correct.
                       (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                    {
                        if(prRangeList)
                        {
                            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                        }
                    }
                    return MPG_SEND_FAIL;
                }
#ifdef SWDMX_MPG_ACCEPT_NON_SEQUENCE_ACK_ID
                for(u4ReqCnt = 0;u4ReqCnt < _u4FeederReqNum;u4ReqCnt++)
                {
                    if(rFeederReqData.u4Id == _rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt])
                    {
                        break;
                    }
                }
                if(u4ReqCnt < _u4FeederReqNum)
#else
                #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
                if(rFeederReqData.u4Id ==
                    _rSwdmxMpgInfo.rBuf.au4ReqId[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                #else
                if(rFeederReqData.u4Id ==
                    ((UINT32)(_u4FeederReqID -
                    (_rSwdmxMpgInfo.rBuf.u4ReqTimes*2))))
                #endif
#endif
                {
                    BOOL fgPut2Buf = TRUE;
                    #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
                    if(0 != _rSwdmxMpgInfo.rBuf.au4ReqDup[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                    {
                        // compare data
                        INT32 i4Ret;
                        UINT32 u4Size;
                        UINT32 u4SizeOrg;
                        UINT32 u4Size1;
                        UINT32 u4Size2;
                        UINT32 u4BufWriteIdx = _rSwdmxMpgInfo.rBuf.u4BufWriteIdx;
                        if(u4BufWriteIdx == 0)
                        {
                            u4BufWriteIdx =
                                (_rSwdmxMpgInfo.rVar.u4FeederBufNum - 1);
                        }
                        else
                        {
                            u4BufWriteIdx--;
                        }
                        u4Size = _rSwdmxMpgInfo.rBuf.au4BufSize[u4BufWriteIdx];
                        u4SizeOrg = u4Size;
                        u4Size1 = prSwdmxInst->rFeederInfo.u4EndAddr -
                            (_rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]);
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
                            (VOID*)(_rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]),
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
                                (VOID*)(u4Size + _rSwdmxMpgInfo.rBuf.au4BufStartAddr[u4BufWriteIdx]),
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

                    _rSwdmxMpgInfo.rBuf.u4ReqTimes--;
                    _rSwdmxMpgInfo.rBuf.u4ReqTotalSize -= _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
                    if(rFeederReqData.u4ReadSize < _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx])
                    {
                        LOG(1, "Mpg Fd read size too small %d -> %d\n",
                            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx], rFeederReqData.u4ReadSize);
              #if 1//def CC_DLNA_SUPPORT        //for DLNA handle eof
                        if(_rSwdmxMpgInfo.rVar.e_mm_src_type !=
                            MM_SRC_TYPE_HIGH_SPEED_STORAGE)
                        {
                            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx] = rFeederReqData.u4ReadSize;
                        }
              #endif
                        //ASSERT(0);
                        //_rSwdmxMpgInfo.rBuf.u4ReqReadIdx++;
                        //if(_rSwdmxMpgInfo.rBuf.u4ReqReadIdx >= _u4FeederReqNum)
                        //{
                        //    _rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
                        //}
                        //return MPG_SEND_FAIL;
                    }

                    if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                    {
                        _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx] = rFeederReqData.u4ReadSize;
                    }

                    if(fgPut2Buf)
                    {
                        _rSwdmxMpgInfo.rBuf.au4BufStartAddr[_rSwdmxMpgInfo.rBuf.u4BufWriteIdx] =
                                rFeederReqData.u4WriteAddr +
                                rFeederReqData.u4AlignOffset;
                        _rSwdmxMpgInfo.rBuf.au4BufSize[_rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
                        _rSwdmxMpgInfo.rBuf.au4BufPTS[_rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = _rSwdmxMpgInfo.rBuf.au4ReqPTS[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
                        _rSwdmxMpgInfo.rBuf.au4BufFileOffset[_rSwdmxMpgInfo.rBuf.u4BufWriteIdx] = _rSwdmxMpgInfo.rBuf.au4ReqFileOffset[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx];
#endif
                        _rSwdmxMpgInfo.rBuf.u4BufTotalSize += _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx];

                       if((_rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx])!=0)                        //for DLNA handle EOF
                       {
                           _rSwdmxMpgInfo.rBuf.u4BufAvailCnt++;
                       }
                        #ifdef SWDMX_MPG_DEBUG_BUFFER
                        LOG(7, "Mpg Fd Get UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d RRS %d RPTS %d FId %d FAdr %d FOfs %d\n",
                            _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                            _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                            _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                            _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                            _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                            _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                            _rSwdmxMpgInfo.rBuf.u4ReqTimes,
                            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            _rSwdmxMpgInfo.rBuf.au4ReqPTS[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            rFeederReqData.u4Id,
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4AlignOffset);
                        #endif
                    }
                    _rSwdmxMpgInfo.rBuf.u4ReqReadIdx++;
                    if(_rSwdmxMpgInfo.rBuf.u4ReqReadIdx >= _u4FeederReqNum)
                    {
                        _rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
                    }
                    if(fgPut2Buf)
                    {
                        _rSwdmxMpgInfo.rBuf.u4BufWriteIdx++;
                        if(_rSwdmxMpgInfo.rBuf.u4BufWriteIdx >= _u4FeederBufNum)
                        {
                            _rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
                        }
                    }

#ifdef TIME_SHIFT_SUPPORT
                    if(fgPut2Buf)
                    {
                        if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
                           ((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+4)) != 0x47))
                        {
                            UINT32 i;
                            LOG(3, "Mpg Fd !0x47\n");
                            ASSERT(_rSwdmxMpgInfo.rVar.u8FifoSize);
                            if(_rSwdmxMpgInfo.rVar.u8FifoSize)
                            {
                                UINT64 u8Tmp;
                                UINT64 u8Remainder;
                                u8Tmp = _SWDMX_Div6464((_u8SendedSize-_rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)192, &u8Remainder);
                                UNUSED(_SWDMX_Div6464((_u8SendedSize-_rSwdmxMpgInfo.rVar.u8FifoOffset), _rSwdmxMpgInfo.rVar.u8FifoSize, &u8Remainder));
                                LOG(1, "!0x47, FileOffset 0x%llx, _u8SendedSize 0x%llx, LBA 0x%llx, FilePos 0x%llx\n",
                                    _rSwdmxMpgInfo.rVar.u8FifoOffset,
                                    _u8SendedSize,
                                    (UINT64)u8Tmp,
                                    (UINT64)(u8Remainder + _rSwdmxMpgInfo.rVar.u8FifoOffset)
                                    );
                            }
                            //ASSERT(0);
                            for(i=5;i<196;i++)
                            {
                                if((*(UCHAR*)VIRTUAL((rFeederReqData.u4WriteAddr+rFeederReqData.u4AlignOffset)+i)) == 0x47)
                                {
                                    _SWDMX_MpgResetData();
                                    _u8SendedSize += (i - 4);
                                    _u8ReadedPos = _u8SendedSize;
                                    _u8PlayPos = _u8SendedSize;
                                }
                            }
                            return MPG_SEND_FULL;
                        }
                    }
#endif
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
                            _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                            _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                            _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                            _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                            _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                            _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                            _rSwdmxMpgInfo.rBuf.u4ReqTimes,
                            _rSwdmxMpgInfo.rBuf.au4ReqSize[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            _rSwdmxMpgInfo.rBuf.au4ReqPTS[_rSwdmxMpgInfo.rBuf.u4ReqReadIdx],
                            rFeederReqData.u4Id,
                            rFeederReqData.u4WriteAddr,
                            rFeederReqData.u4ReadSize);
                        #endif
                        u4Ptr = rFeederReqData.u4WriteAddr + rFeederReqData.u4ReadSize;
                        if(u4Ptr >= _rFeederInfo.u4EndAddr)
                        {
                            u4Ptr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
                        }
                        if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                        {
                            u4Ptr = 0;
                        }
                        if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4Ptr) != FEEDER_E_OK)
                        {
                            LOG(1, "Mpg Fd FeederUpdateReadPointer fail\n");
                            //ASSERT(0);
                        }
                    }
                    // DTV00140229, [DLNA] When play attached mp3 file,repeatedly Fast Backward, system hangs up.
                    // when we are skip the first ack from feeder, it should be a correct one.
                    if((_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) &&
                       (_rSwdmxMpgInfo.rBuf.u4BufAvailCnt == 0))
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
        if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
        {
            if(_rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0)
            {
                _u8ReadedPos += _rSwdmxMpgInfo.rBuf.au4BufSize[_rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                _rSwdmxMpgInfo.rBuf.u4UsingPTS = _rSwdmxMpgInfo.rBuf.au4BufPTS[_rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                _rSwdmxMpgInfo.rBuf.u4UsingBufSize += _rSwdmxMpgInfo.rBuf.au4BufSize[_rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                _rSwdmxMpgInfo.rBuf.u4UsingStartAddr = _rSwdmxMpgInfo.rBuf.au4BufStartAddr[_rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                _rSwdmxMpgInfo.rBuf.u4BufTotalSize -= _rSwdmxMpgInfo.rBuf.au4BufSize[_rSwdmxMpgInfo.rBuf.u4BufReadIdx];
                _rSwdmxMpgInfo.rBuf.u4BufAvailCnt--;

                #ifdef SWDMX_MPG_DEBUG_BUFFER
                LOG(7, "Mpg Fd Eat UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d BRS %d BPTS %d UAdr %d\n",
                    _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
                    _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
                    _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
                    _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
                    _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
                    _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
                    _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
                    _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
                    _rSwdmxMpgInfo.rBuf.u4ReqTimes,
                    _rSwdmxMpgInfo.rBuf.au4BufSize[_rSwdmxMpgInfo.rBuf.u4BufReadIdx],
                    _rSwdmxMpgInfo.rBuf.au4BufPTS[_rSwdmxMpgInfo.rBuf.u4BufReadIdx],
                    _rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
                #endif
                _rSwdmxMpgInfo.rBuf.u4BufReadIdx++;
                if(_rSwdmxMpgInfo.rBuf.u4BufReadIdx >= _u4FeederBufNum)
                {
                    _rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
                }
            }
            else
            {
                LOG(3, "Mpg Fd u4BufAvailCnt 0 u4UsingBufSize 0\n");
                #if 1//def CC_DLNA_SUPPORT                                 //for DLNA handle EOF
                if(_rSwdmxMpgInfo.rBuf.fgGetEof)                                      // get EOF from feeder
                {
                     LOG(3, "Mpg Fd u4BufAvailCnt 0 u4UsingBufSize and send EOS 0\n");
                    _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
                    if(!_fgGetInfo)
                    {
                        _SWDMX_MpgHandleEOS(i4PlaySpeed);
                    }
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
                    }
                    if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
                       (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                       (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
                    {
                        if(prRangeList)
                        {
                            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                        }
                    }
                    return MPG_SEND_FAIL;
                }
                #endif
                if(_u8SendedSize != _u8FileSize)
                {
                    ASSERT(_rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0);
                }
                return MPG_SEND_FAIL;
            }
        }
    }
    #if 1//def CC_DLNA_SUPPORT                           //for DLNA handle EOF
    if(( _rSwdmxMpgInfo.rBuf.fgGetEof==TRUE) &&(_rSwdmxMpgInfo.rBuf.u4BufAvailCnt==0))
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
INT32 _SWDMX_MpgSendData(SWDMX_RANGE_LIST_T* prRangeList,
            INT32 i4PlaySpeed, UINT64 u8SeekPos, BOOL fgChange)
{
#ifdef TIME_SHIFT_SUPPORT
    BOOL fgRet;
#endif
    BOOL fgLast = FALSE;
    INT32 i4Ret;
    UINT32 u4SendSize;
    UINT32 u4Ptr;
    UINT32 u4WaitCnt;

    DMX_MM_DATA_T rDmxMMData;

#ifdef CC_SWDMX_RENDER_POSITION
    if((prRangeList) &&
       (prRangeList->fgSetRenderPosition) &&
       // TS file, no sequence header need, decide position here
       // audio es file, should decide position here
       ((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) ||
        (!_rSwdmxMpgInfo.rFmt.fgVideo) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES) ||
//        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_AVS_VIDEO_ES) ||        
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
        (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)))
    {
        UINT64 u8FilePos = prRangeList->u8OffsetI;
        if (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            if(_SWDMX_MpgGetPosByTime(TRUE, prRangeList->u8StartPts, &u8FilePos))
            {
            }
            else
            {
                u8FilePos = prRangeList->u8OffsetI;
            }
            STC_StopStc();
            STC_SetStcValue(prRangeList->u8StartPts);
        }

        prRangeList->fgSetRenderPosition = FALSE;
        UNUSED(VDEC_SetRenderFromPos(ES0, TRUE,
            prRangeList->u8Offset,
            prRangeList->i4TempRef,
            prRangeList->u2DecodingOrder));
        _u8OrgSendedSize = u8FilePos;
        _u8SendedSize = _u8OrgSendedSize;
        _u8ReadedPos = _u8OrgSendedSize;
        _u8PlayPos = _u8OrgSendedSize;
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = prRangeList->u8StartPts;
        if(_rSwdmxMpgInfo.rVar.prCurRangeReq)
        {
            LOG(5, "_SWDMX_MpgSendData fgSetRenderPosition VDEC_SetRenderFromPos I:0x%llx, 0x%llx, PTS:0x%x, i4TempRef: %d\n",
                _rSwdmxMpgInfo.rVar.prCurRangeReq->u8OffsetI, prRangeList->u8Offset, _rSwdmxMpgInfo.rVar.u4PreviousPTS, prRangeList->i4TempRef);
        }
        UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
        UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, _u8FileSize));

        _SWDMX_MpgResetData();
        MPV_FlushEsmQ(ES0, FALSE);
    }
#endif

    // handle speed and position
    // must handle position before we parsing first sequence header.
    if(fgChange)
    {
        _SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
    }

    // this is first run for one file, we have to get sequence header
    if(_SWDMX_MpgGetFirstSeqHdr())
    {
        u8SeekPos = _u8OrgSendedSize;
        fgChange = TRUE;
        // I am not sure if we need this
        //_SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
    }

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        BOOL fgVideoNotSupport = FALSE;
        VDEC_HDR_INFO_T rHdrInfo;
		HAL_TIME_T NosigCheckTime;
        UNUSED(VDEC_QueryInfo(ES0, &rHdrInfo));
        /*
        if(DMX_GetScrambleState(_u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED)
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

        if(_rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_NORMAL)
        {
            // DTV00214748 when speed=1000, and in no signal status, we should calibrate _rSwdmxMpgInfo.rVar.u8CurTime
            //as x_thread_delay() not very exactly
            if((!_rSwdmxMpgInfo.rVar.fgNeedAdjust)&&(i4PlaySpeed==1000))
            {
                _rSwdmxMpgInfo.rVar.fgNeedAdjust=TRUE;
				HAL_GetTime(&NosigCheckTime);
				_rSwdmxMpgInfo.rVar.u4OrgtDeltaTime= (NosigCheckTime.u4Seconds*1000 + NosigCheckTime.u4Micros/1000)
					                                 - _rSwdmxMpgInfo.rVar.u8CurTime;
            }
            i4Ret = _SWDMX_MpgHandleNoSignel(&u8SeekPos, i4PlaySpeed);
            if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL)
            {
                // DTV00145526, The audio does NOT match the video after unplug/plug the signal cable.
                // Because MW does not close audio in this case,
                // we can't flush audio fifo.
                // Audio driver does not know we flush data if we didn't close
                // audio.

#if 1
                //_SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
                _rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
                MPV_FlushEsmQ(ES0, FALSE);
                UNUSED(VDEC_ReleaseDispQ(ES0));
#else
                _rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
                _SWDMX_MpgHandleChange(i4PlaySpeed, u8SeekPos);
                //MPV_FlushEsmQ(ES0, FALSE);
                //UNUSED(VDEC_ReleaseDispQ(ES0));
#endif
                // TODO
                // Audio also need a flush
                // and resync action
            }
            return i4Ret;
        }
        else if(((!_rSwdmxMpgInfo.rFmt.fgVideo)
                 ||
                 ((_rSwdmxMpgInfo.rFmt.fgVideo) &&
                  (fgVideoNotSupport)))
                &&
                // while we are parsing table, not handle this.
                // DTV00142030, Can't pause time shift playback mode in any audio channel.
                // when there is no video, we do not send data anymore
                // while beginin parsing table, i4PlaySpeed = 0;
                //(!_fgCheckPSISize)
                //&&
                // both audio on and off have to check this.
                //(_rSwdmxMpgInfo.rFmt.fgAudio) &&
                (i4PlaySpeed != 1000) &&
                (i4PlaySpeed != 0))
        {
            i4Ret = _SWDMX_MpgHandleEmpty(&u8SeekPos, i4PlaySpeed);
            return i4Ret;
        }
		else
		{
			_rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
		}

    }
#endif

    LOG(9, "_SWDMX_MpgSendData pos 0x%llx\n", _u8SendedSize);

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(_rSwdmxMpgInfo.rVar.fgChangeVideo)
        {
            LOG(5, "_SWDMX_MpgSendData fgChangeVideo\n");
            _rSwdmxMpgInfo.rVar.fgChangeVideo = FALSE;
            fgRet = _SWDMX_DMX_Reset();//DMX_DDI_RequestReset();
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgSendData _SWDMX_DMX_Reset Fail\n");
            }
        }
    }
#endif


    if(_rSwdmxMpgInfo.rVar.fgJumpping && _rSwdmxMpgInfo.rVar.fgJumpNext)
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
        if(_rSwdmxMpgInfo.rVar.u2JumpAgainCount == 0)
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
                if(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
                {
                }
                // to check vdp have change frame, so we could leave earlier.
                if(((u8Offset != _rSwdmxMpgInfo.rVar.u8OffsetKeep) &&
                    (u8OffsetI != _rSwdmxMpgInfo.rVar.u8OffsetIKeep) &&
                    (i4TempRef != _rSwdmxMpgInfo.rVar.i4TempRefKeep) &&
                    (u2DecodingOrder != _rSwdmxMpgInfo.rVar.u2DecodingOrderKeep))
                   ||
                   (_SWDMX_GetNumOfCmd() > 0))
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
            _rSwdmxMpgInfo.rVar.u8OffsetKeep = u8Offset;
            _rSwdmxMpgInfo.rVar.u8OffsetIKeep = u8OffsetI;
            _rSwdmxMpgInfo.rVar.i4TempRefKeep = i4TempRef;
            _rSwdmxMpgInfo.rVar.u2DecodingOrderKeep = u2DecodingOrder;
        }
        #endif

        if(((i4PlaySpeed == -1)
            ||
            (i4PlaySpeed == 1)
           )
           && (_rSwdmxMpgInfo.rVar.u2JumpAgainCount == 0))
        {
            // no need to pause
            //SWDMX_Pause();
        }
        else
        {
            _SWDMX_MpgHandleSeek(i4PlaySpeed, FALSE);
            _rSwdmxMpgInfo.rVar.fgJumpNext = FALSE;
        }
    }
    else if((_rSwdmxMpgInfo.rVar.fgJumpping)
            &&
            (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES))
    {
        if(_rSwdmxMpgInfo.rVar.u4TotalTime > 0)
        {
            UINT64 u8Tmp;
            UINT64 u8Remainder;
            u8Tmp = _SWDMX_Div6464(((_u8FileSize - _u8StartFileOffset)*MPG_AUDIO_ES_SKIP_TIME), (UINT64)(_rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
            UNUSED(u8Remainder);
            if((_u8PlayPos - _rSwdmxMpgInfo.rVar.u8JumpStartAddr) >= u8Tmp)
            {
                {
                    UINT32 u4DmxAvailSizeAudio;
                    UINT32 au4BufStart[2], au4BufEnd[2];
                    UINT32 u4BufSize;
#ifdef CC_DMX_PURE_AUDIO
                    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                            _u1PidxAudio);
               	    }
                    else
#endif
                    {
                        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
#ifdef AUD_OLDFIFO_INTF
                    VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                                          &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
                    VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
                    u4BufSize          = au4BufEnd[0] - au4BufStart[0];
                    if(((u4BufSize - u4DmxAvailSizeAudio) < 100000)
                       &&
                       ((u4BufSize - u4DmxAvailSizeAudio) ==
                         _rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize)
                      )
                    {
                        _SWDMX_MpgHandleSeekAudio(i4PlaySpeed, FALSE);
                        return MPG_SEND_OK;
                    }
                    else
                    {
                        if((_rSwdmxMpgInfo.rVar.fgGotAudioReady) &&
                           (!_rSwdmxMpgInfo.rVar.fgEnableAudio))
                        {
                            AUD_MMAoutEnable(0, TRUE);
                            STC_StartStc();
                            _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
                        }

                        _rSwdmxMpgInfo.rVar.u4LastAudioFifoDataSize =
                            (u4BufSize - u4DmxAvailSizeAudio);
                        // wait audio consume
                        x_thread_delay(60);
                        return MPG_SEND_OK;
                    }
                }
            }
        }
    }

    // must put ahead _SWDMX_MpgHandleSeek
    // there is a case, after send EOS to decoder
    // and before decoder look EOS, decoder found PIC and notify
    // _rSwdmxMpgInfo.rVar.fgJumpNext = TRUE
    // we must let _SWDMX_MpgHandleSeek call again to deliver EOS.
    if(_rSwdmxMpgInfo.rVar.fgSendEOF)
    {
        // audio still not aout after EOS
        if((_rSwdmxMpgInfo.rFmt.fgAudio) &&
           (_rSwdmxMpgInfo.rVar.fgGotAudioReady) &&
           (!_rSwdmxMpgInfo.rVar.fgEnableAudio))
        {
            AUD_MMAoutEnable(0, TRUE);
            STC_StartStc();
            _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
        }

        // DTV00136518, The special .mpe video freezed after input time=00:00:20 and do time seek function.
        // Audio ready comes after eos.
        if((((_rSwdmxMpgInfo.rFmt.fgAudio) && (_rSwdmxMpgInfo.rVar.fgGotAudioReady))
            ||
            (!_rSwdmxMpgInfo.rFmt.fgAudio)) &&
            ((_rSwdmxMpgInfo.rFmt.fgVideo) &&
             (_rSwdmxMpgInfo.rVar.fgSendVideoEos) &&
             (_rSwdmxMpgInfo.rVar.fgVideoPause)))
        {
            // i4PlaySpeed = 1x and audio ready later after video send eos
            // if now in VDEC_Pause, VDec might in pause status.
            if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                UNUSED(VDP_SetPauseMm(B2R_1, FALSE));
                if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_MPV))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_MPV fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_H264))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_H264 fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_WMV))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play VDEC_FMT_WMV fail\n");
                    }
                }
                else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
                {
                    if(!VDEC_Play(ES0, VDEC_FMT_MP4))
                    {
                        LOG(3, "_SWDMX_MpgSendData VDEC_Play ENUM_SWDMX_VDEC_MPEG4 fail\n");
                    }
                }
                else
                {
                    ASSERT(0);
                }
                _rSwdmxMpgInfo.rVar.fgVideoPause = FALSE;
                _rSwdmxMpgInfo.rVar.fgGotVideoReady = TRUE;
                _rSwdmxMpgInfo.rVar.fgEnableVideo = TRUE;
                LOG(5, "_SWDMX_MpgSendData VDEC_Play after EOS\n");
            }
        }

        LOG(7, "_SWDMX_MpgSendData _fgSendDataEOS true.\n");
        return MPG_SEND_EOS;
    }

    if(_rSwdmxMpgInfo.rVar.fgFirst) // first send data
    {
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
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

    u4SendSize = _u4OneTimeSize;

    if((_u8SendedSize >= _u8FileSize)
    #if 1//def CC_DLNA_SUPPORT                               //for DLNA handle EOF
    ||((_rSwdmxMpgInfo.rBuf.fgLastData)&&(_rSwdmxMpgInfo.rBuf.u4UsingBufSize==0))
    #endif
        )        // have sended all the data
    {
#ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo))
            {
                _prSwdmxInfo->pfnSwdmxRangeCb(
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_INTERNAL_ERROR,
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF);
            }
            LOG(1, "_SWDMX_MpgSendData SWDMX send (EOF), impossible\n");
            //UNUSED(_SWDMX_MpgResetFeederReq());
            //return MPG_SEND_FAIL;
        }
#endif

        LOG(5, "_SWDMX_MpgSendData SWDMX send (EOF)\n");
        _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;

        if(!_fgGetInfo)
        {
            _SWDMX_MpgHandleEOS(i4PlaySpeed);
        }

        if(prRangeList)
        {
            prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
        }
        if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
           (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
           // DTV00211818
           // when audio es playback.
           // pause -> seek -> seek -> play
           // we will send finish event. this is not correct.
           (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
        {
            if(prRangeList)
            {
                prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
            }
        }
        return MPG_SEND_EOS;
    }

    if((UINT64)u4SendSize > ((_u8FileSize - _u8SendedSize)))
    {
        u4SendSize = (UINT32)((_u8FileSize - _u8SendedSize));
    }


    if(u4SendSize == 0)
    {
        LOG(1, "_SWDMX_MpgSendData u4SendSize < 0 Fail\n");
        ASSERT(0);
    }

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        UINT32 u4Temp;
        fgRet = _SWDMX_MpgGetTickByPos(
            TRUE,
            _u8SendedSize,
            i4PlaySpeed,
            &u4Temp);
        if((_rSwdmxMpgInfo.rVar.prLastTickIndex) &&
           (fgRet))
        {
            _rSwdmxMpgInfo.rVar.u4CurTickNum = u4Temp;
            //LOG(5, "Trace u4CurTickNum(%d) _u8SendedSize(0x%llx)\n", _rSwdmxMpgInfo.rVar.u4CurTickNum, _u8SendedSize);
            _rSwdmxMpgInfo.rVar.u8CurTime =
                (UINT64)_rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
            _rSwdmxMpgInfo.rVar.u8CurTime *=
                _rSwdmxMpgInfo.rVar.u4TickBase;

            LOG(9, "_SWDMX_MpgSendData u4CurTickNum 0x%x\n",
                _rSwdmxMpgInfo.rVar.u4CurTickNum);
            if(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
                PVR_TICK_INDEX_FLAG_EMPTY) &&
               (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                _rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
                    _rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

                _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_NO_SIGNAL;
                LOG(3, "u4NoSignalStage 1.2.1, PREPARE_NO_SIGNAL tick %d\n",
                    _rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
                return MPG_SEND_FULL;
            }
            else if(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
                PVR_TICK_INDEX_FLAG_BAD) &&
               (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
            {
                _rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
                    _rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

                _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_BAD_STORAGE;
                LOG(3, "u4NoSignalStage 1.2.2, PREPARE_BAD_STORAGE tick %d\n",
                    _rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
                return MPG_SEND_FULL;
            }
        }
        else
        {
            // DTV00060341 [Time Shift] Play directly and display "Retrieving Data..." by press PLAY/PAUSE after record more than 2 hrs.
            // Handle out of range case
            if((_rSwdmxMpgInfo.rVar.fgOutOfRange) &&
               (_rSwdmxMpgInfo.rVar.u8SafeEndAddr > 0) &&
               (_rSwdmxMpgInfo.rVar.u8SafeStartAddr > 0))
            {
                UINT64 u8Temp;
                _rSwdmxMpgInfo.rVar.fgOutOfRange = FALSE;
                if(_rSwdmxMpgInfo.rVar.i4CurSpeed < 0)
                {
                    u8Temp = _rSwdmxMpgInfo.rVar.u8SafeEndAddr;
                }
                else
                {
                    u8Temp = _rSwdmxMpgInfo.rVar.u8SafeStartAddr;
                }
                u8Temp = _SWDMX_MpgTSAlignDown(u8Temp);
                if(i4PlaySpeed >= 0)
                {
                    u8Temp += _u4AlignBase;
                }
                LOG(3, "Change start position 0x%llx ==========\n", _u8SendedSize);
                _SWDMX_MpgHandleChange(i4PlaySpeed, u8Temp);
                return MPG_SEND_FULL;
            }
        }
    }
#endif

    i4Ret = _SWDMX_MpgHandleFeeder(prRangeList, i4PlaySpeed);

    if(i4Ret != MPG_SEND_OK)
    {
       #if 1//def CC_DLNA_SUPPORT                       // for DLNA handle EOF
       if(i4Ret==MPG_SEND_LAST)
       {
        if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize<=u4SendSize)     // for DLAN handle  EOF
        {
               fgLast=TRUE;                                               // the last group of data will be send to dmx
              _rSwdmxMpgInfo.rBuf.fgLastData=TRUE;
                u4SendSize = _rSwdmxMpgInfo.rBuf.u4UsingBufSize;
           }
       }
    else
    #endif
    {
           if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
         {
            if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
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

    if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0)
    {
#ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            LOG(5, "_SWDMX_MpgSendData u4UsingBufSize 0\n");
            return MPG_SEND_FULL;
        }
#endif
        LOG(0, "_SWDMX_MpgSendData u4UsingBufSize 0\n");
        ASSERT(_rSwdmxMpgInfo.rBuf.u4UsingBufSize > 0);
        return MPG_SEND_FAIL;
    }



    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
       ||
       (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
       ||
       (_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
       ||
       // Network application
       // when eof with request count > 0, but not receive feeder's EOF yet,
       // we don't know it is eof.
       (_rSwdmxMpgInfo.rVar.e_mm_src_type != MM_SRC_TYPE_HIGH_SPEED_STORAGE)
       )
    {
        if(u4SendSize > _rSwdmxMpgInfo.rBuf.u4UsingBufSize)
        {
            u4SendSize = _rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        }
    }

    if((_u8SendedSize + u4SendSize) >= _u8FileSize)
    {
        fgLast = TRUE;
    }

    // Move from above (before _SWDMX_MpgHandleFeeder)
    if((i4Ret = _SWDMX_MpgHandleFifoFull(i4PlaySpeed, fgLast)) != MPG_SEND_OK)
    {
        return i4Ret;
    }


    x_memset(&rDmxMMData, 0, sizeof(rDmxMMData));
    rDmxMMData.u4FrameSize = u4SendSize;
    rDmxMMData.u4BufStart = _rFeederInfo.u4StartAddr;
    rDmxMMData.u4BufEnd = _rFeederInfo.u4EndAddr;
    if(_rSwdmxMpgInfo.rBuf.u4UsingStartAddr >= rDmxMMData.u4BufEnd)
    {
        _rSwdmxMpgInfo.rBuf.u4UsingStartAddr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
    }
    rDmxMMData.u4StartAddr = (_rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    if(rDmxMMData.u4StartAddr >= rDmxMMData.u4BufEnd)
    {
        rDmxMMData.u4StartAddr -= (rDmxMMData.u4BufEnd - rDmxMMData.u4BufStart);
    }

    if(_rSwdmxMpgInfo.rVar.fgFirst)
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
    //if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)&&(_rSwdmxMpgInfo.rFmt.fgAudio))
    {
        if((_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            if(_rSwdmxMpgInfo.rBuf.u4UsingPTS)
            {
                rDmxMMData.u4Pts = _rSwdmxMpgInfo.rBuf.u4UsingPTS;
                rDmxMMData.u4Dts = _rSwdmxMpgInfo.rBuf.u4UsingPTS;
                _rSwdmxMpgInfo.rBuf.u4UsingPTS = 0;
            }
        }
        else
        {
            UINT64 u8Remainder;

            if(_rSwdmxMpgInfo.rBuf.u4UsingPTS)
            {
                rDmxMMData.u4Pts = _rSwdmxMpgInfo.rBuf.u4UsingPTS;
                rDmxMMData.u4Dts = _rSwdmxMpgInfo.rBuf.u4UsingPTS;
                _rSwdmxMpgInfo.rBuf.u4UsingPTS = 0;
            }
            // actually, below condition will not entry anymore.
            // because we have calculated it in _SWDMX_MpgHandleAudioIndex
            else
            if((rDmxMMData.fgFrameHead) &&
               (_u8FileSize > 0))
            {
                rDmxMMData.u4Pts = _SWDMX_Div6464((_u8SendedSize - _u8StartFileOffset)*((UINT64)_rSwdmxMpgInfo.rVar.u4TotalTime),
                    (UINT64)(_u8FileSize - _u8StartFileOffset), &u8Remainder);
                rDmxMMData.u4Pts += MPG_STC_BASE;
                rDmxMMData.u4Dts = rDmxMMData.u4Pts;
            }
        }
    }

    if((i4Ret = _SWDMX_MpgHandleMoveData(&rDmxMMData, i4PlaySpeed)) != MPG_SEND_OK)
    {
        return i4Ret;
    }
    if(_rSwdmxMpgInfo.rVar.fgFirst)
    {
        _rSwdmxMpgInfo.rVar.fgFirst = FALSE;
    }

    _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
    //_rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
    //_rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    if(_rSwdmxMpgInfo.rBuf.u4UsingBufSize >= u4SendSize)
    {
        _u8SendedSize += u4SendSize;
        _rSwdmxMpgInfo.rBuf.u4UsingStartAddr += u4SendSize;
        _rSwdmxMpgInfo.rBuf.u4UsingBufSize -= u4SendSize;
        //_u4AvailSize += u4SendSize;
        if((_rSwdmxMpgInfo.rBuf.u4UsingBufSize == 0) && (_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH))
        {
            if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, 0) != FEEDER_E_OK)
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
            _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
            if(!_fgGetInfo)
            {
                 _SWDMX_MpgHandleEOS(i4PlaySpeed);
            }
            if(prRangeList)
            {
                 prRangeList->eRangeStats = eSWDMX_RANGE_STATE_PENDING;
            }
            if((!_rSwdmxMpgInfo.rFmt.fgAudio) &&
               (!_rSwdmxMpgInfo.rFmt.fgVideo) &&
                   // DTV00211818
                   // when audio es playback.
                   // pause -> seek -> seek -> play
                   // we will send finish event. this is not correct.
               (_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES))
               {
                    if(prRangeList)
                    {
                        prRangeList->eRangeStats = eSWDMX_RANGE_STATE_FINISHED;
                    }
               }
                return MPG_SEND_FAIL;
        }
        _u8SendedSize += _rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        _rSwdmxMpgInfo.rBuf.u4UsingStartAddr += _rSwdmxMpgInfo.rBuf.u4UsingBufSize;
        _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
        if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
        {
            if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, 0) != FEEDER_E_OK)
            {
                LOG(1, "_SWDMX_MpgSendData FeederUpdateReadPointer fail\n");
            }
        }
    }
    if(_u8SendedSize > _u8ReadedPos)
    {
        LOG(1, "_SWDMX_MpgSendData _u8SendedSize > _u8ReadedPos\n");
        ASSERT(0);
    }

    if (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
        UINT32 u4DmxAvailSizeAudio;
        UINT32 au4BufStart[2], au4BufEnd[2];
        UINT32 u4BufSize;
#ifdef CC_DMX_PURE_AUDIO
        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
            _u1PidxAudio);
#else
        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
#endif
#ifdef AUD_OLDFIFO_INTF
        VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                              &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
        VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
        u4BufSize          = au4BufEnd[0] - au4BufStart[0];
        if(_u8SendedSize > (UINT64)(u4BufSize - u4DmxAvailSizeAudio))
        {
            _u8PlayPos = _u8SendedSize - (UINT64)(u4BufSize - u4DmxAvailSizeAudio);
        }
        else
        {
            _u8PlayPos = 0;
        }
    }

    u4Ptr = rDmxMMData.u4StartAddr + rDmxMMData.u4FrameSize;
    if(u4Ptr >= _rFeederInfo.u4EndAddr)
    {
        u4Ptr -= (_rFeederInfo.u4EndAddr - _rFeederInfo.u4StartAddr);
    }
    if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, u4Ptr) != FEEDER_E_OK)
    {
        LOG(1, "_SWDMX_MpgSendData FeederUpdateReadPointer fail\n");
        //ASSERT(0);
    }

#ifdef TIME_SHIFT_SUPPORT
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) &&
       (_rSwdmxMpgInfo.rVar.fgJumpping) &&
       (DMX_GetScrambleState(_u1PidxVideo) == DMX_SCRAMBLE_STATE_SCRAMBLED))
    {
        _rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
    }
#endif

    return MPG_SEND_OK;
}


BOOL _SWDMX_MpgSendDataBatch(VOID)
{
    INT32 i4Ret;
    // detect first
    while(1)
    {
        i4Ret = _SWDMX_MpgSendData(NULL, 0, (UINT64)0, FALSE);
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

        //if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_MpgResetData();
        //}
    }
    return TRUE;
}

BOOL _SWDMX_MpgSendDataBatchForTotalTime(VOID)
{
    INT32 i4Ret;
    // detect first
    while(1)
    {
        i4Ret = _SWDMX_MpgSendData(NULL, 0, (UINT64)0, FALSE);
        if(_rSwdmxMpgInfo.rVar.u4FirstPTS)
        {
            // there is some fake PTS in this file end.
            // please check program 4 and 8. (1 base).
            // [DTV00047346] [TS file playback] Can NOT display the TS, log will print fail info.
            if(_rSwdmxMpgInfo.rVar.u4LastPTSCnt >= MPG_PTS_CHECK_NUM)
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

        //if(_rSwdmxMpgInfo.rVar.fgNotToDecoder)
        //{
        //    _SWDMX_MpgResetData();
        //}
    }
    return TRUE;
}

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFillBufferStart(VOID)
{
    BOOL fgRet;

    LOG(5, "_SWDMX_MpgTsFillBufferStart\n");

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = TRUE; // not parse sequence header
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;

    _rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE; // skip it
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it

    fgRet = _SWDMX_MpgSendDataBatch();

    return fgRet;
}
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFillBufferEnd(VOID)
{
    LOG(5, "_SWDMX_MpgTsFillBufferEnd\n");

    _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;

    return TRUE;
}
#endif

/**
*/
BOOL _SWDMX_MpgBaseInit(VOID)
{
    LOG(5, "_SWDMX_MpgBaseInit\n");

#ifdef SWDMX_MPG_PS_GET_INFO
    _SWDMX_MpgCreateFindSema();
#endif

    UNUSED(FeederInstallCallback(FEEDER_PROGRAM_SOURCE, FEEDER_NOTIFY_DATA_REQ_DONE,
        _SWDMX_FeederCallBack, (UINT32)&_rFeederInfo));

    //DMX_Init();
    //DMX_DDI_Init();
    _SWDMX_MpgResetVariable();

    #ifdef SWDMX_MPG_USE_INDEX
    _rSwdmxMpgInfo.rVar.fgUseIdx = FALSE;
    #endif

    _rSwdmxMpgInfo.rFmt.u2PidIdxStart = 4;
    _rSwdmxMpgInfo.rFmt.u2VideoPidIdx = 0xFF;
    _rSwdmxMpgInfo.rFmt.u2AudioPidIdx = 0xFF;
    _rSwdmxMpgInfo.rFmt.u2Audio2PidIdx = 0xFF;
    //_rSwdmxMpgInfo.rFmt.u2PcrPidIdx = 0xFF;
    _rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = 0;
    _rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = 0;
    _rSwdmxMpgInfo.rFmt.u1Audio2KeyIndex = 0;
    _rSwdmxMpgInfo.rFmt.u4VideoDescMode = 0;
    _rSwdmxMpgInfo.rFmt.u4AudioDescMode = 0;
    _rSwdmxMpgInfo.rFmt.u4Audio2DescMode = 0;

#if 1//def CC_DLNA_SUPPORT
    // always check, some format feeder buffer is different
    //if(_u4FeederBufSize == 0)
    {
        UINT32 u4FeederBufNum;
        if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
          FEEDER_E_OK)
        {
            LOG(1, "Feeder input buffer error.\n");
            return FALSE;
        }
        _u4FeederBufSize = (_rFeederInfo.u4Size);
        u4FeederBufNum = (_u4FeederBufSize / (MPG_BATCH_SIZE_PVR*MPG_BATCH_READ_SIZE));
        if((_u4FeederBufSize > 0) &&
           (u4FeederBufNum > MPG_BUF_NUM_RESERVED)) // - 3 * (192*188*10)
        {
            if((u4FeederBufNum - MPG_BUF_NUM_RESERVED) > _u4FeederBufNum)
            {
                _u4FeederBufNum = (u4FeederBufNum - MPG_BUF_NUM_RESERVED);
                if(_u4FeederBufNum > MPG_BUF_NUM_MAX)
                {
                    _u4FeederBufNum = MPG_BUF_NUM_MAX;
                }
            }
        }
        else if((_u4FeederBufSize > 0) &&
                 (u4FeederBufNum > 0))
        {
            _u4FeederBufNum = (u4FeederBufNum);
            if(_u4FeederBufNum > MPG_BUF_NUM_MAX)
            {
                _u4FeederBufNum = MPG_BUF_NUM_MAX;
            }
        }
        else
        {
            ASSERT(0);
            LOG(1, "feeder buf size 0\n");
        }
        if(_u4FeederBufNum != MPG_BUF_NUM)
        {
            LOG(5, "feeder buf size 0x%x, buf num %d\n",
                _u4FeederBufSize, _u4FeederBufNum);
        }
        if((_u4FeederBufSize > 0) &&
           (u4FeederBufNum > 0))
        {
            _u4FeederReqNum = (u4FeederBufNum);
            if(_u4FeederReqNum > MPG_REQ_NUM_MAX)
            {
                _u4FeederReqNum = MPG_REQ_NUM_MAX;
            }
        }
        else
        {
            ASSERT(0);
            LOG(1, "feeder buf size 0\n");
        }
        if(_u4FeederReqNum > _u4FeederBufNum)
        {
            _u4FeederReqNum = _u4FeederBufNum;
        }
        if(_u4FeederReqNum != MPG_REQ_NUM)
        {
            LOG(5, "feeder buf size 0x%x, req num %d\n",
                _u4FeederBufSize, _u4FeederReqNum);
        }
        #ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        if(_u4FeederReqNum < 2)
        {
            LOG(0, "req num %d\n",
                _u4FeederReqNum);
            ASSERT(0);
        }
        #endif
    }
#endif

    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgEsInit(ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;

    LOG(5, "_SWDMX_MpgEsInit\n");

    UNUSED(_SWDMX_MpgBaseInit());

    // Initialize dmx
    if (eType == SWDMX_FMT_RAW_AUDIO_ES)
    {
#if 0//def CC_DMX_PURE_AUDIO // it is not cared in pure audio (no dmx)
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
            LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_MM\n");
            return fgRet;
        }
    }
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
    _rSwdmxMpgInfo.rFmt.eFmt = eType;
    _u4OneTimeSize = MPG_BATCH_SIZE;
    _u4OneTimeReadSize = (_u4OneTimeSize*MPG_BATCH_READ_SIZE);
    _u4AlignBase = MPG_ALIGN_SIZE;

    DMX_SetToDecoder(TRUE);
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();

    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgPsInit(ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;

    LOG(5, "_SWDMX_MpgPsInit\n");

    UNUSED(_SWDMX_MpgBaseInit());

    // Initialize dmx
    fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_PS);
    if(!fgRet)
    {
        LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_PS\n");
        return fgRet;
    }
    fgRet = DMX_SetVideoType(DMX_VIDEO_MPEG);
    if(!fgRet)
    {
        LOG(3, "DMX_SetVideoType DMX_VIDEO_MPEG\n");
        return fgRet;
    }
    _rSwdmxMpgInfo.rFmt.eFmt = eType;
    _u4OneTimeSize = MPG_BATCH_SIZE;
    _u4OneTimeReadSize = (_u4OneTimeSize*MPG_BATCH_READ_SIZE);
    _u4AlignBase = MPG_ALIGN_SIZE;

    if(eType == SWDMX_FMT_MPEG1_DAT)
    {
#if defined(CC_MT5365)  || defined(CC_MT5395)
        fgRet = DMX_PS_SetDataFormat(0, DMX_DATA_FORMAT_DAT);
#else
        fgRet = DMX_PS_SetDataFormat(DMX_DATA_FORMAT_DAT);
#endif
        if(!fgRet)
        {
            LOG(3, "DMX_PS_SetDataFormat DMX_DATA_FORMAT_RAW\n");
            return fgRet;
        }
    }
    else if(eType == SWDMX_FMT_MPEG2_PS)
    {
#if defined(CC_MT5365)  || defined(CC_MT5395)
        fgRet = DMX_PS_SetDataFormat(0, DMX_DATA_FORMAT_RAW);
#else
        fgRet = DMX_PS_SetDataFormat(DMX_DATA_FORMAT_RAW);
#endif
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
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();

    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;

    #ifdef SWDMX_MPG_USE_INDEX
    _rSwdmxMpgInfo.rVar.fgUseIdx = TRUE;
    #endif

    return TRUE;
}

/**
*/
#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsInit(ENUM_SWDMX_FMT_T eType)
{
    BOOL fgRet;
    DMX_DECODER_CALLBACKS_T rDecoderCallbacks;

    LOG(5, "_SWDMX_MpgTsInit\n");

    UNUSED(_SWDMX_MpgBaseInit());

#ifdef TIME_SHIFT_SUPPORT
    if(eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(DMX_GetInputType() != DMX_IN_BROADCAST_TS)
        {
            LOG(3, "Time-shift DMX mode not using DMX_IN_BROADCAST_TS\n");
            return FALSE;
        }

        if(!_SWDMX_PVR_Init())
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_Init Fail\n");
            return FALSE;
        }
        if(!_SWDMX_PVR_OPEN())
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_OPEN Fail\n");
            return FALSE;
        }

        // Make it single move in the begining
        if(!_SWDMX_PVR_SetDataMode(1))
        {
            LOG(3, "_SWDMX_MpgTsInit _SWDMX_PVR_SetDataMode fail\n");
        }

        _u4OneTimeSize = MPG_BATCH_SIZE_PVR;
        _u4OneTimeReadSize = (_u4OneTimeSize*MPG_BATCH_READ_SIZE);
        _u4AlignBase = MPG_ALIGN_SIZE_PVR;

        _rSwdmxMpgInfo.rVar.fgValidRange = FALSE;
        _rSwdmxMpgInfo.rVar.fgOutOfRange = FALSE;
        //_rSwdmxMpgInfo.rVar.fgOutOfSafeRange = FALSE;
        _rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
        _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        _rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        _rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        _rSwdmxMpgInfo.rVar.u4TickBase = 0;
        _rSwdmxMpgInfo.rVar.u4NoSignalTickNum = 0;
        _rSwdmxMpgInfo.rVar.u4CurTickNum = 0;
        _rSwdmxMpgInfo.rVar.u4ReadTickNum = 0;
        _rSwdmxMpgInfo.rVar.u4SafeStartTick = 0;
        _rSwdmxMpgInfo.rVar.u4SafeEndTick = 0;
        _rSwdmxMpgInfo.rVar.u4ValidStartTick = 0;
        _rSwdmxMpgInfo.rVar.u4ValidEndTick = 0;
        _rSwdmxMpgInfo.rVar.u8CurTime = 0;
        _rSwdmxMpgInfo.rVar.u8MonitorAddr = 0;
        _rSwdmxMpgInfo.rVar.u8SafeStartAddr = 0;
        _rSwdmxMpgInfo.rVar.u8SafeEndAddr = 0;
        _rSwdmxMpgInfo.rVar.u8ValidStartAddr = 0;
        _rSwdmxMpgInfo.rVar.u8ValidEndAddr = 0;
        _rSwdmxMpgInfo.rVar.u8FifoOffset = 0;
        _rSwdmxMpgInfo.rVar.u8FifoSize = 0;
        _rSwdmxMpgInfo.rVar.prLastTickIndex = NULL;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = NULL;
        x_memset(&_rSwdmxMpgInfo.rVar.rRangeInfo, 0, sizeof(MM_REC_BUF_RANGE_INFO_T));
        x_memset(&_rSwdmxMpgInfo.rVar.rTickCtl, 0, sizeof(PVR_TICK_CTRL_BLK_T));
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
            fgRet = DMX_SelectInputType(DMX_IN_PLAYBACK_TS);
            if(!fgRet)
            {
                LOG(3, "DMX_SelectInputType DMX_IN_PLAYBACK_TS\n");
                return fgRet;
            }
        }
        if((eType == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
            (eType == SWDMX_FMT_MPEG2_TS_192))
        {
            _u4OneTimeSize = MPG_BATCH_SIZE_TS_192;
            _u4AlignBase = MPG_ALIGN_SIZE_TS_192;
            UNUSED(DMX_DDI_SetPacketSize(192));
            UNUSED(DMX_SetPacketSize(192));
        }
#ifdef TIME_SHIFT_SUPPORT
        else if(eType == SWDMX_FMT_MPEG2_TS_ENCRYPT_192)
        {
            if(!_SWDMX_PVR_Init())
            {
                LOG(3, "%s,%d: _SWDMX_PVR_Init Fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }
            if(!_SWDMX_PVR_OPEN())
            {
                LOG(3, "%s,%d: _SWDMX_PVR_OPEN Fail\n", __FUNCTION__, __LINE__);
                return FALSE;
            }

            _u4OneTimeSize = MPG_BATCH_SIZE_TS_192;
            _u4AlignBase = MPG_ALIGN_SIZE_TS_192;
            UNUSED(DMX_PVRPlay_SetUseDescramble(TRUE));
        }
#endif
        else
        {
            _u4OneTimeSize = MPG_BATCH_SIZE;
            _u4AlignBase = MPG_ALIGN_SIZE;
        }
        _u4OneTimeReadSize = (_u4OneTimeSize*MPG_BATCH_READ_SIZE);
    }
    fgRet = DMX_SetVideoType(DMX_VIDEO_MPEG);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgTsInit DMX_SetVideoType Fail\n");
        return fgRet;
    }
    _rSwdmxMpgInfo.rFmt.eFmt = eType;

    DMX_SetToDecoder(TRUE);
    rDecoderCallbacks.pfnSendVideoPes = _SWDMX_MpgVideoCallback;
    rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
    rDecoderCallbacks.pfnSendAudioPes = _SWDMX_MpgAudioCallback;
    DMX_SetDecoderCallbacks(&rDecoderCallbacks);

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();

    SWDMX_DmxInit();

    _rSwdmxMpgInfo.rVar.prCurRangeReq = NULL;
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;

    return TRUE;
}
#endif


/**
*/
BOOL _SWDMX_MpgOpen(ENUM_SWDMX_FMT_T eType,
    UINT32 u4TrackId,
    UINT64 u8FileSize,
    SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet;

    if(prStreamInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgOpen prStreamInfo NULL\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_MpgOpen eType %d\n", eType);
    LOG(5, "    u8FileSize in 32 bits = 0x%llx\n", u8FileSize);

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();

    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    //_u4AvailSize = 0;
    _fgCheckPSISize = FALSE;
    _u8FileSize = u8FileSize;
    _u8StartFileOffset = 0;

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

            fgRet = _SWDMX_MpgSetInfo((UINT8)eSWDMX_SET_VIDEOTYPE, (UINT32)prStreamInfo->aeVideo[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetInfo V Fail\n");
                return FALSE;
            }

            fgRet = _SWDMX_MpgSetStrmID((UINT8)eSWDMX_STRM_TYPE_VID, prStreamInfo->au2VideoId[u4TrackId], 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID V Fail\n");
                return FALSE;
            }

            fgRet = _SWDMX_MpgEnableStrm((UINT8)eSWDMX_STRM_TYPE_VID, NULL);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgEnableStrm V Fail\n");
                return FALSE;
            }
        }
        if((prStreamInfo->u2AudioNum > 0) && (u4TrackId < prStreamInfo->u2AudioNum))
        {
            if(u4TrackId >= SWDMX_MAX_AUD_TRACK)
            {
                LOG(3, "_SWDMX_MpgOpen aud u4TrackId %d -> %d\n",
                    u4TrackId, (SWDMX_MAX_AUD_TRACK-1));
                u4TrackId = (SWDMX_MAX_AUD_TRACK-1);
            }

            fgRet = _SWDMX_MpgSetInfo((UINT8)eSWDMX_SET_AUDIOTYPE, (UINT32)prStreamInfo->aeAudio[u4TrackId], 0, 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetInfo A Fail\n");
                return FALSE;
            }

            fgRet = _SWDMX_MpgSetStrmID((UINT8)eSWDMX_STRM_TYPE_AUD, prStreamInfo->au2AudioId[u4TrackId], prStreamInfo->au2AudioSubId[u4TrackId]);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID A Fail\n");
                return FALSE;
            }

            fgRet = _SWDMX_MpgEnableStrm((UINT8)eSWDMX_STRM_TYPE_AUD, NULL);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgEnableStrm A Fail\n");
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

            fgRet = _SWDMX_MpgSetStrmID((UINT8)eSWDMX_STRM_TYPE_PCR, prStreamInfo->au2PcrId[u4TrackId], 0);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgSetStrmID PCR Fail\n");
                return FALSE;
            }

            fgRet = _SWDMX_MpgEnableStrm((UINT8)eSWDMX_STRM_TYPE_PCR, NULL);
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgOpen _SWDMX_MpgEnableStrm A Fail\n");
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

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();

    _fgCheckPSISize = FALSE;

    UNUSED(prStreamInfo);
    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgPlay(VOID)
{
#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
    
	   _rSwdmxMpgInfo.rVar.fgNeedAdjust = FALSE;	  //should clear the flag of needed adjust current time in no signal status	
        return _SWDMX_PVR_Resume();
    }
#endif

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgPause(VOID)
{
#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return _SWDMX_PVR_Pause();
    }
#endif

    return TRUE;
}


/**
*/
BOOL _SWDMX_MpgAbort(VOID)
{
    return TRUE;
}


#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetVideoId(UINT16 u2VideoId)
{
    FBM_POOL_T* prFbmPool;
    DMX_PS_T rDmxPS;
    UINT32 u4Flags;
    UINT8 u1Pidx;
    BOOL fgRet;

    u1Pidx = 0;
    fgRet = FALSE;

    if(_u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_MpgPsSetVideoId _u1PidxVideo != 0xFF\n");
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
            | DMX_PID_FLAG_STREAM_ID);
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.u1StreamID = (UINT8)u2VideoId;
        {
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
            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = prFbmPool->u4Addr;
            rDmxPS.u4BufSize          = prFbmPool->u4Size;
        }
        rDmxPS.ePidType = DMX_PID_TYPE_ES_VIDEO;
        rDmxPS.fgEnable = TRUE;

        fgRet = DMX_PS_SetStream(u1Pidx, u4Flags, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsSetVideoId DMX_PS_SetStream Fail\n");
            return FALSE;
        }

        _u1PidxVideo = u1Pidx;
    }
    return TRUE;
}
#endif

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsSetAudioId(UINT16 u2AudioId, UINT16 u2AudioSubId)
{
    DMX_PS_T rDmxPS;
    UINT32 u4Flags;
    UINT8 u1Pidx = 1;
    BOOL fgRet = FALSE;

    if(_u1PidxAudio != 0xFF)
    {
        LOG(3, "_SWDMX_MpgPsSetAudioId _u1PidxAudio != 0xFF\n");
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
            | DMX_PID_FLAG_STREAM_ID);
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.u1StreamID = (UINT8)u2AudioId;
        {
            UINT32 au4BufStart[2], au4BufEnd[2];
#ifdef AUD_OLDFIFO_INTF
            VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                                  &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
            VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif

            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = au4BufStart[0];
            rDmxPS.u4BufSize          = au4BufEnd[0] - au4BufStart[0];
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
            else if(u2AudioSubId >= 0xA0 && u2AudioSubId <= 0xA8)
            {
                fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_LPCM);
            }
            else
            {
                LOG(3, "u2AudioId 0xBD u2AudioSubId 0x%x Unknown\n", u2AudioSubId);
                fgRet = FALSE;
            }
        }
        else if(u2AudioId >= 0xC0 && u2AudioId <= 0xDF)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else
        {
            LOG(3, "u2AudioId 0xBD u2AudioSubId 0x%x Unknown\n", u2AudioId);
            fgRet = FALSE;
        }
        /*
        if(_rSwdmxMpgInfo.rFmt.eAudioType == ENUM_SWDMX_ADEC_AC3)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_AC3);
        }
        else if(_rSwdmxMpgInfo.rFmt.eAudioType == ENUM_SWDMX_ADEC_PCM)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_LPCM);
        }
        else if(_rSwdmxMpgInfo.rFmt.eAudioType == ENUM_SWDMX_ADEC_MPEG)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else if(_rSwdmxMpgInfo.rFmt.eAudioType == ENUM_SWDMX_ADEC_MP3)
        {
            fgRet = DMX_PS_SetAudioType(u1Pidx, DMX_AUDIO_MPEG);
        }
        else
        {
            LOG(3, "_rSwdmxMpgInfo.rFmt.eAudioType Unknown\n");
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
            UINT32 au4BufStart[2], au4BufEnd[2];
#ifdef AUD_OLDFIFO_INTF
            VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
            VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif

            rDmxPS.fgAllocateBuffer   = FALSE;
            rDmxPS.u4BufAddr          = au4BufStart[0];
            rDmxPS.u4BufSize          = au4BufEnd[0] - au4BufStart[0];
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

        _u1PidxAudio = u1Pidx;
    }
    return TRUE;
}
#endif

#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsFreeVideoId(VOID)
{
    BOOL fgRet;
    DMX_PS_T rDmxPS;

    LOG(5, "_SWDMX_MpgPsFreeVideoId\n");

    if(_u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.fgEnable = FALSE;
        fgRet = DMX_PS_SetStream(_u1PidxVideo, DMX_PID_FLAG_VALID, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeVideoId DMX_PS_SetStream V Fail\n");
            return FALSE;
        }
        fgRet = DMX_PS_FreeStream(_u1PidxVideo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeVideoId DMX_PS_FreeStream V Fail\n");
            return FALSE;
        }
        _u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgPsFreeVideoId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_PS_DISABLE
BOOL _SWDMX_MpgPsFreeAudioId(VOID)
{
    BOOL fgRet;
    DMX_PS_T rDmxPS;

    LOG(5, "_SWDMX_MpgPsFreeAudioId\n");

    if(_u1PidxAudio != 0xFF)
    {
        x_memset(&rDmxPS, 0, sizeof(rDmxPS));
        rDmxPS.fgEnable = FALSE;
        fgRet = DMX_PS_SetStream(_u1PidxAudio, DMX_PID_FLAG_VALID, &rDmxPS);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeAudioId DMX_PS_SetStream A Fail\n");
            //return FALSE;
        }
        fgRet = DMX_PS_FreeStream(_u1PidxAudio);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgPsFreeAudioId DMX_PS_FreeStream A Fail\n");
            //return FALSE;
        }
        _u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgPsFreeAudioId Fail\n");
    return FALSE;
}
#endif

#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetVideoId(UINT16 u2VideoId, UINT8 u1KeyIndex, UINT32 u4DescMode)
{
    SWDMX_DMX_PES_T rPes;
    UINT8 u1Pidx;
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "_SWDMX_MpgTsSetVideoId u2VideoId = %d\n", u2VideoId);

    if(_u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }
    if(!u2VideoId)
    {
        LOG(3, "_SWDMX_MpgTsSetVideoId u2VideoId != 0\n");
        return FALSE;
    }

#ifndef CC_TS_H264_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
    {
    }
    else
#endif
#ifndef CC_TS_MPEG2_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
    }
    else
#endif
#ifndef CC_TS_VC1_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
    }
    else
#endif
    {
        LOG(3, "TsSetVideoId Unknown eVideoType %d\n", _rSwdmxMpgInfo.rFmt.eVideoType);
        return FALSE;
    }

    u1Pidx = _rSwdmxMpgInfo.rFmt.u2PidIdxStart;
    if(_rSwdmxMpgInfo.rFmt.u2VideoPidIdx != 0xFF)
    {
        u1Pidx = _rSwdmxMpgInfo.rFmt.u2VideoPidIdx;
    }

    x_memset(&rPes, 0, sizeof(rPes));
    rPes.u2Pid = u2VideoId;
    rPes.u2DeviceId = 0;
    rPes.u4BufferSize = TS_PES_VIDEO_FIFO_SIZE;
#ifdef TIME_SHIFT_SUPPORT
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
       (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
    {
        rPes.u1TsIndex = DMX_PVRPLAY_TS_INDEX;
        rPes.u1KeyIndex = u1KeyIndex;
        rPes.u4DescMode = u4DescMode;
    }
#endif

    eRet = SWDMX_DmxPesAllocateBuffer(&rPes, &u1Pidx, DMX_PID_TYPE_ES_VIDEO);
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

    _u1PidxVideo = u1Pidx;

    return TRUE;
}
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsSetAudioId(UINT16 u2AudioId, UINT16 u2DeviceId, UINT8 u1KeyIndex, UINT32 u4DescMode)
{
    SWDMX_DMX_PES_T rPes;
    UINT8 u1Pidx;
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "_SWDMX_MpgTsSetAudioId u2AudioId = %d\n", u2AudioId);

    if((u2DeviceId == 0) && (_u1PidxAudio != 0xFF))
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId _u1PidxAudio != 0xFF\n");
        return FALSE;
    }
    /* not support now
    if((u2DeviceId == 1) && (_u1PidxAudio2 != 0xFF))
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId _u1PidxAudio != 0xFF\n");
        return FALSE;
    }
    */
    if(!u2AudioId)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u2AudioId != 0\n");
        return FALSE;
    }
    if(u2DeviceId>2)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId u2DeviceId %d > 2\n", u2DeviceId);
        return FALSE;
    }

    u1Pidx = (UINT8)((_rSwdmxMpgInfo.rFmt.u2PidIdxStart + 1) + u2DeviceId);
    if(_rSwdmxMpgInfo.rFmt.u2AudioPidIdx != 0xFF)
    {
        if(u2DeviceId == 0)
        {
            u1Pidx = _rSwdmxMpgInfo.rFmt.u2AudioPidIdx;
        }
        /* not support now
        else if(u2DeviceId == 1)
        {
            u1Pidx = _rSwdmxMpgInfo.rFmt.u2Audio2PidIdx;
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
    rPes.u2DeviceId = u2DeviceId;
    rPes.u4BufferSize = TS_PES_AUDIO_FIFO_SIZE;
#ifdef TIME_SHIFT_SUPPORT
    if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT) ||
       (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192))
    {
        rPes.u1TsIndex = DMX_PVRPLAY_TS_INDEX;
        rPes.u1KeyIndex = u1KeyIndex;
        rPes.u4DescMode = u4DescMode;
    }
#endif

    eRet = SWDMX_DmxPesAllocateBuffer(&rPes, &u1Pidx, DMX_PID_TYPE_ES_AUDIO);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId SWDMX_DmxPesAllocateBuffer A Fail\n");
        return FALSE;
    }
    eRet = SWDMX_DmxPesStart(u1Pidx);
    if(eRet != SWDMX_E_OK)
    {
        LOG(3, "_SWDMX_MpgTsSetAudioId SWDMX_DmxPesStart A Fail\n");
        return FALSE;
    }
    if(u2DeviceId == 0)
    {
        _u1PidxAudio = u1Pidx;
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
BOOL _SWDMX_MpgTsSetPCRId(UINT16 u2PCRId)
{
    SWDMX_DMX_PES_T rPes;
    UINT8 u1Pidx;
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "%s u2PCRId = %d\n", __FUNCTION__, u2PCRId);

    if(_u1PidxPCR != 0xFF)
    {
        LOG(3, "%s _u1PidxPCR != 0xFF\n", __FUNCTION__);
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
    eRet = SWDMX_DmxPesAllocateBuffer(&rPes, &u1Pidx, DMX_PID_TYPE_NONE);
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

    _u1PidxPCR = u1Pidx;

    return TRUE;
}
#endif
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFreeVideoId(VOID)
{
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "_SWDMX_MpgTsFreeVideoId\n");

    if(_u1PidxVideo != 0xFF)
    {
        eRet = SWDMX_DmxPesStop(_u1PidxVideo);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeVideoId SWDMX_DmxPesStop V Fail\n");
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(_u1PidxVideo);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeVideoId SWDMX_DmxPesReleaseBuffer V Fail\n");
            //return FALSE;
        }
        _u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgTsFreeVideoId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsFreeAudioId(VOID)
{
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "_SWDMX_MpgTsFreeAudioId\n");

    if(_u1PidxAudio != 0xFF)
    {
        eRet = SWDMX_DmxPesStop(_u1PidxAudio);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeAudioId SWDMX_DmxPesStop A Fail\n");
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(_u1PidxAudio);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "_SWDMX_MpgTsFreeAudioId SWDMX_DmxPesReleaseBuffer A Fail\n");
            //return FALSE;
        }
        _u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgTsFreeAudioId Fail\n");
    return FALSE;
}
#endif


#ifndef CC_TS_DISABLE
#ifdef SWDMX_MPG_SET_PCR_ID
BOOL _SWDMX_MpgTsFreePCRId(VOID)
{
    SWDMX_ERR_CODE_T eRet;

    LOG(5, "%s\n", __FUNCTION__);

    if(_u1PidxPCR != 0xFF)
    {
        if(!DMX_SetPcr(_u1PidxPCR, TRUE, FALSE))
        {
            return FALSE;
        }

        eRet = SWDMX_DmxPesStop(_u1PidxPCR);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "%s SWDMX_DmxPesStop Fail\n", __FUNCTION__);
            //return FALSE;
        }
        eRet = SWDMX_DmxPesReleaseBuffer(_u1PidxPCR);
        if(eRet != SWDMX_E_OK)
        {
            LOG(3, "%s SWDMX_DmxPesReleaseBuffer A Fail\n", __FUNCTION__);
            //return FALSE;
        }
        _u1PidxPCR = 0xFF;
        return TRUE;
    }
    LOG(3, "%s Fail\n", __FUNCTION__);
    return FALSE;
}
#endif
#endif


#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetVideoId(VOID)
{
    DMX_ES_T rDmxES;
    UINT8 u1Pidx = 0;
    UINT32 u4Flags;
    BOOL fgRet;
    FBM_POOL_T* prFbmPool;

    LOG(5, "_SWDMX_MpgEsSetVideoId\n");

    if(_u1PidxVideo != 0xFF)
    {
        LOG(3, "_SWDMX_MpgEsSetVideoId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }

#ifndef CC_ES_H264_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
    {
    }
    else
#endif
#ifndef CC_ES_MPEG2_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
    {
    }
    else
#endif
#ifndef CC_ES_MPEG4_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4)
    {
    }
    else
#endif
#ifndef CC_ES_VC1_DISABLE
    if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
    {
    }
    else
#endif
    {
        LOG(3, "EsSetVideoId Unknown eVideoType %d\n", _rSwdmxMpgInfo.rFmt.eVideoType);
        return FALSE;
    }

    //set es pid for playback

    u4Flags = (UINT32)(DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_VALID);
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
    _SWDMX_MpgResetData();
    _u1PidxVideo = u1Pidx;

    return TRUE;
}
#endif


#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsSetAudioId(VOID)
{
    DMX_MM_T rDmxMMInfo;
    UINT8 u1Pidx = 0;
    UINT32 u4Flags;
    BOOL fgRet;
    UINT32 au4BufStart[2], au4BufEnd[2];

    LOG(5, "_SWDMX_MpgEsSetAudioId\n");

    if(_u1PidxAudio != 0xFF)
    {
        LOG(3, "_SWDMX_MpgEsSetAudioId _u1PidxVideo != 0xFF\n");
        return FALSE;
    }

#if !defined(CC_ES_MP3_DISABLE) && !defined(CC_ES_MP2_DISABLE)
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
    }
    else
#endif
    {
        LOG(3, "EsSetAudioId Unknown eFmt %d\n", _rSwdmxMpgInfo.rFmt.eFmt);
        return FALSE;
    }

    u4Flags = (UINT32)(DMX_MM_FLAG_TYPE | DMX_MM_FLAG_NOTIFY |
      DMX_MM_FLAG_VALID | DMX_MM_FLAG_BUF);

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

#ifdef AUD_OLDFIFO_INTF
    if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
         &au4BufEnd[1]) != AUD_OK)
#else
    if (AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK)
#endif
    {
        LOG(1, "_SWDMX_MpgEsSetAudioId Can't get audio buffers!\n");
        return FALSE;
    }

    rDmxMMInfo.fgAllocBuf = FALSE;

    rDmxMMInfo.u4BufAddr = au4BufStart[0];
    rDmxMMInfo.u4BufSize = au4BufEnd[0] - au4BufStart[0];

    fgRet = DMX_MM_Set(u1Pidx, u4Flags, &rDmxMMInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgEsSetAudioId DMX_MM_Set Fail\n");
        return FALSE;
    }
    _u1PidxAudio = u1Pidx;

    return TRUE;
}
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsFreeVideoId(VOID)
{
    BOOL fgRet;
    DMX_ES_T rDmxES;

    LOG(5, "_SWDMX_MpgEsFreeVideoId\n");

    if(_u1PidxVideo != 0xFF)
    {
        x_memset(&rDmxES, 0, sizeof(rDmxES));
        rDmxES.fgEnable = FALSE;
        fgRet = DMX_ES_SetPid(_u1PidxVideo, DMX_PID_FLAG_VALID, &rDmxES);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeVideoId DMX_ES_SetPid V Fail\n");
            //return FALSE;
        }
        fgRet = DMX_ES_FreePid(_u1PidxVideo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeVideoId SWDMX_DmxPesReleaseBuffer V Fail\n");
            //return FALSE;
        }
        _u1PidxVideo = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgEsFreeVideoId Fail\n");
    return FALSE;
}
#endif

#ifndef CC_ES_DISABLE
BOOL _SWDMX_MpgEsFreeAudioId(VOID)
{
    BOOL fgRet;
    DMX_MM_T rDmxMMInfo;

    LOG(5, "_SWDMX_MpgEsFreeAudioId\n");

    if(_u1PidxAudio != 0xFF)
    {
        x_memset(&rDmxMMInfo, 0, sizeof(rDmxMMInfo));
        rDmxMMInfo.fgEnable = FALSE;
        fgRet = DMX_MM_Set(_u1PidxAudio, DMX_PID_FLAG_VALID, &rDmxMMInfo);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeAudioId DMX_MM_Set Fail\n");
            //return FALSE;
        }
        fgRet = DMX_MM_Free(_u1PidxAudio);
        if(!fgRet)
        {
            LOG(3, "_SWDMX_MpgEsFreeAudioId DMX_MM_Free Fail\n");
            //return FALSE;
        }
        _u1PidxAudio = 0xFF;
        return TRUE;
    }
    LOG(3, "_SWDMX_MpgEsFreeAudioId Fail\n");
    return FALSE;
}
#endif


/**
 To get first and last PTS
*/
BOOL _SWDMX_MpgBaseGetInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_STREAM_INFO_T* prStreamInfo)
{
    BOOL fgRet = TRUE;
    UINT64 u8SendSize;
    UINT64 u8Tmp, u8Remainder;
    UINT32 u4LastPTS, u4FirstPTS;
    UINT32 u4PicCnt;

    if(prStreamInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgBaseGetInfo prStreamInfo NULL\n");
        return FALSE;
    }

    u8SendSize = 0;
    u4FirstPTS = 0;
    u4LastPTS = 0;

    LOG(5, "_SWDMX_MpgBaseGetInfo\n");

    fgRet = _SWDMX_MpgOpen(eType, 0, u8FileSize, prStreamInfo);
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgOpen Fail.\n");
        return FALSE;
    }

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = TRUE; // not parse sequence header
    _rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it

    // first send
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _u8FileSize = u8FileSize;
    _u8StartFileOffset = 0;
    if(_u8FileSize > FIRST_PTS_DETECT_SIZE)
    {
        _u8FileSize = FIRST_PTS_DETECT_SIZE;
    }
    ASSERT(_u8FileSize < (UINT64)0xFFFFFFFF);
    u8SendSize = _u8FileSize;


    fgRet = _SWDMX_MpgSendDataBatchForTotalTime();
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgSendDataBatch First Fail.\n");
    }
    LOG(5, "First: u4FirstPTS %d, u4PicCnt %d, u8SendSize 0x%llx\n",
        _rSwdmxMpgInfo.rVar.u4FirstPTS, _rSwdmxMpgInfo.rVar.u4PicCnt, u8SendSize);
    u4FirstPTS = _rSwdmxMpgInfo.rVar.u4FirstPTS;
    u4LastPTS = _rSwdmxMpgInfo.rVar.u4LastPTS;
    u4PicCnt = _rSwdmxMpgInfo.rVar.u4PicCnt;
    // ~First

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _fgCheckPSISize = FALSE;
    _rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip send eos it
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip send eos it

    // Second
    _u8SendedSize = 0;
    _u8FileSize = u8FileSize;
    _u8StartFileOffset = 0;
    if(_u8FileSize > LAST_PTS_DETECT_SIZE)
    {
        _u8SendedSize = (_u8FileSize - LAST_PTS_DETECT_SIZE);
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    }
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;

    ASSERT((_u8FileSize - _u8SendedSize) < (UINT64)0xFFFFFFFF);
    u8SendSize += (_u8FileSize - _u8SendedSize);

    fgRet = _SWDMX_MpgSendDataBatchForTotalTime();
    if(!fgRet)
    {
        LOG(3, "_SWDMX_MpgBaseGetInfo _SWDMX_MpgSendDataBatch Second Fail.\n");
    }
    LOG(5, "Second: u4LastPTS %d, u4PicCnt %d, u8SendSize 0x%llx\n",
        _rSwdmxMpgInfo.rVar.u4LastPTS, _rSwdmxMpgInfo.rVar.u4PicCnt, u8SendSize);
    if(_rSwdmxMpgInfo.rVar.u4LastPTS)
    {
        u4LastPTS = _rSwdmxMpgInfo.rVar.u4LastPTS;
        u4PicCnt += _rSwdmxMpgInfo.rVar.u4PicCnt;
    }
    // ~Second

    // Calculate TotalTime
    if((u4FirstPTS == 0) || (u4LastPTS == 0) ||
        (eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
        (eType == SWDMX_FMT_H264_VIDEO_ES) ||
        (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
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
            ASSERT(u8Tmp < (UINT64)0xFFFFFFFF);
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

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    _fgSendSeqHdrVc1 = FALSE;
#endif

    fgRet = _SWDMX_MpgCloseGetInfo();
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
BOOL _SWDMX_MpgGetSeqInfo(ENUM_SWDMX_FMT_T eType,
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
        LOG(1, "_SWDMX_MpgGetSeqInfo prStreamInfo NULL\n");
        return FALSE;
    }

    LOG(5, "_SWDMX_MpgGetSeqInfo\n");

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    _fgSendSeqHdrVc1 = FALSE;
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
            x_memset(&_rSeqInfo, 0, sizeof(_rSeqInfo));
            if(j >= SWDMX_MAX_VID_TRACK)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo SWDMX_MAX_VID_TRACK Fail.\n");
                break;
            }
            //_eVideo = prPInfo->rPgm[i].aeVideo[j];
            fgRet = _SWDMX_MpgOpen(eType, j, u8FileSize, &(prPInfo->rPgm[i]));
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgOpen Fail.\n");
                continue;
            }

            _SWDMX_MpgResetData();
            _SWDMX_MpgResetVariable();
            _u8SendedSize = 0;
            _u8ReadedPos = _u8SendedSize;
            _u8PlayPos = _u8SendedSize;
            _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
            _fgSendSeqHdrVc1 = FALSE;
#endif
            _fgCheckPSISize = FALSE;
            _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
            _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it


            _u8FileSize = u8FileSize;
            _u8StartFileOffset = 0;
            if(_u8FileSize > SEQ_DETECT_SIZE)
            {
                _u8FileSize = SEQ_DETECT_SIZE;
            }

            // send
            fgRet = TRUE;
            while(1)
            {
                i4Ret = _SWDMX_MpgSendData(NULL, 0, (UINT64)0, FALSE);
                if(_fgSendSeqHdr)
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
            if(_fgSendSeqHdr)
            {
                prPInfo->rPgm[i].u4Width[j] = _rSeqInfo.u2_width;
                prPInfo->rPgm[i].u4Height[j] = _rSeqInfo.u2_height;
            }
            _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
            _fgSendSeqHdrVc1 = FALSE;
#endif
            // ~Parsing

            fgRet = _SWDMX_MpgCloseGetInfo();
            if(!fgRet)
            {
                LOG(3, "_SWDMX_MpgGetSeqInfo _SWDMX_MpgCloseGetInfo Fail.\n");
                //return FALSE;
            }
        }
    }

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
    _fgSendSeqHdrVc1 = FALSE;
#endif

    UNUSED(u4InfoMask);

    return TRUE;
}

/**
*/
#ifndef CC_PS_DISABLE
#ifdef SWDMX_MPG_PS_GET_INFO
BOOL _SWDMX_MpgPsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
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

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgPsGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

    zMsgSize = sizeof(INT32);
    u1Pidx = 0;
    fgRet = FALSE;

    LOG(5, "_SWDMX_MpgPsGetStreamInfo\n");

    _SWDMX_MpgPsInit(eType);

    _SWDMX_FlushFindMsgQ();

    prPInfo->u4PgmNum = 0;

    u4Flags = (DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_STREAM_ID);
    x_memset(&rDmxPS, 0, sizeof(rDmxPS));
    {
        FBM_POOL_T* prFbmPool;
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
        rDmxPS.fgAllocateBuffer   = FALSE;
        rDmxPS.u4BufAddr          = prFbmPool->u4Addr;
        rDmxPS.u4BufSize          = prFbmPool->u4Size;
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
    u8Tmp = _SWDMX_Div6464(_u8FileSize, (UINT64)(2048), &u8Remainder);
    UNUSED(_SWDMX_Div6464(_u8FileSize, (UINT64)(_u4AlignBase), &u8Remainder));
    u4Tmp = u8Remainder;
    if(_u8FileSize > (_u4AlignBase*2))
    {
        fgRet = DMX_PS_DetectStreamID(TRUE,
            (u8Tmp),
            (_u8FileSize - (u4Tmp) - (_u4AlignBase*2)));
    }
    else
    {
        u8Tmp = _SWDMX_Div6464(_u8FileSize, (UINT64)(2048), &u8Remainder);
        fgRet = DMX_PS_DetectStreamID(TRUE,
            (u8Tmp),//(_u8FileSize/2048),
            (_u8FileSize - (u4Tmp)));
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

    _SWDMX_FlushDataMsgQ();

    _rSwdmxMpgInfo.rVar.fgSendEOF = FALSE;
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;
    _rSwdmxMpgInfo.rVar.fgJumpping = FALSE;
    //_rSwdmxMpgInfo.rVar.fgFirst = TRUE;
    _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
    _u8SendedSize = 0;
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    //_u4AvailSize = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousIPos = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = 0;
    _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = 0;
    _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
    _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;

    _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE; // skip it
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE; // skip it

    _SWDMX_MpgSendDataBatch();

    #if 0
    //detect PS
    //dmx callback to swdmx tell streamid or pid for a/v
    VERIFY(x_timer_start(_hFindTimer, OPEN_TIMEOUT, X_TIMER_FLAG_ONCE,
        _SWDMX_TimerFindTimeout, (VOID*)0) == OSR_OK);
    #endif
    VERIFY(x_msg_q_receive(&u2MsgQIdx, &u4FindFin, &zMsgSize, &(_hFindMsgQ), 1,
    X_MSGQ_OPTION_WAIT) == OSR_OK);
    VERIFY(x_timer_stop(_hFindTimer) == OSR_OK);

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
        LOG(1, "PS open fail, findfin = %d, VideoStreamId = %d, AudioStreamId = %d", u4FindFin, (INT32)_ucPSVideoStreamId, (INT32)_ucPSAudioStreamId);
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
    _SWDMX_MpgPsInit(eType);

    if(_ucPSVideoStreamId || _ucPSAudioStreamId)
    {
        // to get total duration
        return _SWDMX_MpgBaseGetInfo(eType, prPInfo->rPgm[0]);
    }

    UNUSED(u4InfoMask);

    return FALSE;
}
#endif
#endif

/**
*/
#ifndef CC_TS_DISABLE
BOOL _SWDMX_MpgTsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    UINT32 u4Src, u4Num;
    UINT32 u4Timeout;
    UINT32 u4AudPid;
    INT32 i4Ret;
    BOOL fgRet;
    UINT32 i, j;

    if(prPInfo == NULL)
    {
        LOG(1, "_SWDMX_MpgTsGetStreamInfo prStreamInfo NULL\n");
        return FALSE;
    }

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_GetInfo(prPInfo))
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

    if(!_SWDMX_MpgTsInit(eType))
    {
        LOG(1, "_SWDMX_MpgTsGetStreamInfo _SWDMX_MpgTsInit fail\n");
        return FALSE;
    }

    x_memset(_arPMTTbl, 0, sizeof(PSIPSR_TBL_PMT_T)*SWDMX_MAX_PMT_NUM);
    u4Src = 0;
    u4Num = 0;
    u4Timeout = OPEN_TIMEOUT; // timeout

    {
        prPInfo->u4PgmNum = 0;
        //detect TS
        i4Ret = SWDMX_PsiPsrParsePsi(_arPMTTbl, &u4Num, u4Timeout, u4Src);

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
                LOG(5, "prog=0x%x, pcr=0x%x, v=0x%x(0x%x), a1=0x%x(0x%x), a2=0x%x(0x%x), a3=0x%x(0x%x)\n",
                    _arPMTTbl[i].u4ProgNum, _arPMTTbl[i].u4PCR,
                    _arPMTTbl[i].u4VPID, _arPMTTbl[i].u4VPIDType,
                    _arPMTTbl[i].u4APID1, _arPMTTbl[i].u4APID1Type,
                    _arPMTTbl[i].u4APID2, _arPMTTbl[i].u4APID2Type,
                    _arPMTTbl[i].u4APID3, _arPMTTbl[i].u4APID3Type);

                if(_arPMTTbl[i].u4VPID)
                {
                    if(prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum < SWDMX_MAX_VID_TRACK)
                    {
                        prPInfo->rPgm[prPInfo->u4PgmNum].au2VideoId[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = (UINT16)_arPMTTbl[i].u4VPID;
                        if(_arPMTTbl[i].u4VPIDType == STREAM_TYPE_VIDEO)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_MPEG1_MPEG2;
                        }
                        else if(_arPMTTbl[i].u4VPIDType == STREAM_TYPE_H264)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_H264;
                        }
                        else if(_arPMTTbl[i].u4VPIDType == STREAM_TYPE_VC1)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_VC1;
                        }
                        else
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].aeVideo[prPInfo->rPgm[prPInfo->u4PgmNum].u2VideoNum] = ENUM_SWDMX_VDEC_UNKNOWN;
                            LOG(3, "_SWDMX_MpgTsOpen Unknown Video Type\n");
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
                for (j=0; j<3 ;j++)
                {
                    u4AudPid = 0;
                    if((j == 0) && (_arPMTTbl[i].u4APID1 > 0))
                    {
                        u4AudPid = _arPMTTbl[i].u4APID1;
                    }
                    else if((j == 1) && (_arPMTTbl[i].u4APID2 > 0))
                    {
                        u4AudPid = _arPMTTbl[i].u4APID2;
                    }
                    else if((j == 2) && (_arPMTTbl[i].u4APID3 > 0))
                    {
                        u4AudPid = _arPMTTbl[i].u4APID3;
                    }
                    if(u4AudPid > 0)
                    {
                        if(prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum < SWDMX_MAX_AUD_TRACK)
                        {
                            prPInfo->rPgm[prPInfo->u4PgmNum].au2AudioId[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = (UINT16)_arPMTTbl[i].u4APID1;
                            if((_arPMTTbl[i].u4APID1Type == STREAM_TYPE_MPEG1) ||
                                (_arPMTTbl[i].u4APID1Type == STREAM_TYPE_MPEG2))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_MPEG;
                            }
                            else if((_arPMTTbl[i].u4APID1Type == STREAM_TYPE_AC3) ||
                                (_arPMTTbl[i].u4APID1Type == STREAM_TYPE_AC3_EU) ||
                                (_arPMTTbl[i].u4APID1Type == STREAM_TYPE_EAC3))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_AC3;
                            }
                            else if((_arPMTTbl[i].u4APID1Type == STREAM_TYPE_AAC) ||
                                (_arPMTTbl[i].u4APID1Type == STREAM_TYPE_AAC_ISDB))
                            {
                                prPInfo->rPgm[prPInfo->u4PgmNum].aeAudio[prPInfo->rPgm[prPInfo->u4PgmNum].u2AudioNum] = ENUM_SWDMX_ADEC_AAC;
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
                    fgRet = _SWDMX_MpgBaseGetInfo(eType,
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
BOOL _SWDMX_MpgEsGetStreamInfo(ENUM_SWDMX_FMT_T eType,
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

    LOG(5, "_SWDMX_MpgEsGetStreamInfo\n");

    if(!_SWDMX_MpgEsInit(eType))
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
    fgRet = _SWDMX_MpgBaseGetInfo(eType,
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
BOOL _SWDMX_MpgGetStreamInfo(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* prPInfo)
{
    BOOL fgRet;

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

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = TRUE;
    _fgGetInfo = TRUE;
    _u8FileSize = u8FileSize;
    _u8StartFileOffset = 0;
    _rSwdmxMpgInfo.rVar.fgNotToDecoder = TRUE;
    _rSwdmxMpgInfo.rVar.fgSendVideoEos = TRUE;
    _rSwdmxMpgInfo.rVar.fgSendAudioEos = TRUE;


    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_TRACK)
    {
        fgRet = FALSE;
#ifdef SWDMX_MPG_PS_GET_INFO
        if((eType == SWDMX_FMT_MPEG2_PS) ||
            (eType == SWDMX_FMT_MPEG1_DAT))
        {
            if(u8FileSize > PS_OPEN_FILE_SIZE)
            {
                _u8FileSize = PS_OPEN_FILE_SIZE;
            }
            fgRet = _SWDMX_MpgPsGetStreamInfo(eType, u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
#ifndef CC_TS_DISABLE
#ifdef TIME_SHIFT_SUPPORT
        if(eType == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            fgRet =  _SWDMX_MpgTsGetStreamInfo(eType,
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
                _u8FileSize = TS_OPEN_FILE_SIZE;
            }
            fgRet =  _SWDMX_MpgTsGetStreamInfo(eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
#ifndef CC_ES_DISABLE
        if((eType == SWDMX_FMT_MPEG2_VIDEO_ES) ||
            (eType == SWDMX_FMT_H264_VIDEO_ES) ||
            (eType == SWDMX_FMT_MPEG4_VIDEO_ES) ||
            (eType == SWDMX_FMT_VC1_ES))
        {
            if(u8FileSize > ES_OPEN_FILE_SIZE)
            {
                _u8FileSize = ES_OPEN_FILE_SIZE;
            }
            fgRet =  _SWDMX_MpgEsGetStreamInfo(eType,
                u4InfoMask, u8FileSize, prPInfo);
        }
        else
#endif
        {
            LOG(3, "_SWDMX_MpgGetStreamInfo Unknown eType\n");
        }
        if(!fgRet)
        {
            _fgGetInfo = FALSE;
            LOG(3, "_SWDMX_MpgGetStreamInfo TRACK FALSE\n");
            return FALSE;
        }
    }
    if(u4InfoMask & SWDMX_STREAM_INFO_MASK_RESOLUTION)
    {
        fgRet =  _SWDMX_MpgGetSeqInfo(eType, u4InfoMask, u8FileSize, prPInfo);
        if(!fgRet)
        {
            _fgGetInfo = FALSE;
            LOG(3, "_SWDMX_MpgGetStreamInfo RESOLUTION FALSE\n");
            return FALSE;
        }
    }

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _fgGetInfo = FALSE;

    LOG(5, "_SWDMX_MpgGetStreamInfo Result:\n");
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
BOOL _SWDMX_MpgCloseGetInfo(VOID)
{
    LOG(5, "_SWDMX_MpgCloseForOpen\n");
    UNUSED(_SWDMX_MpgDisableStrm((UINT8)eSWDMX_STRM_TYPE_VID));
    UNUSED(_SWDMX_MpgDisableStrm((UINT8)eSWDMX_STRM_TYPE_AUD));
    UNUSED(_SWDMX_MpgDisableStrm((UINT8)eSWDMX_STRM_TYPE_AUD2));
    UNUSED(_SWDMX_MpgDisableStrm((UINT8)eSWDMX_STRM_TYPE_SUBTITLE));
    UNUSED(_SWDMX_MpgDisableStrm((UINT8)eSWDMX_STRM_TYPE_PCR));
    return _SWDMX_MpgStop();
}

/**
*/
BOOL _SWDMX_MpgClose(VOID)
{
    LOG(5, "_SWDMX_MpgClose\n");

    if(!_SWDMX_MpgStop())
    {
        return FALSE;
    }

    return TRUE;
}


/**
*/
VOID _SWDMX_MpgSetRange(SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                                      UINT32 u4AudStrmID, UINT32 u4SubTitleID)
{
    MM_RANGE_INFO_T* prRangeInfo = 0;
    LOG(5, "_SWDMX_MpgSetRange\n");

    if(prRangeList == NULL)
    {
        LOG(0, "Demuxer Range is NULL\n");
        return;
    }

    prRangeInfo = prRangeList->prDmxRangeInfo;
    _rSwdmxMpgInfo.rVar.prCurRangeReq = prRangeList;

    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
    {
        if(//(_rSwdmxMpgInfo.rFmt.fgAudio) &&
           //(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
           //(_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            //MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                prIdxTbl = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                //prTblEntry = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if(/*(prTblEntry) && */(prIdxTbl))
                {
                    if(prIdxTbl->ui4_number_of_entry > 0)
                    {
                        // with index, we don't need to check alignment
                        _u4AlignBase = 1;
                    }
                    else if(prIdxTbl->ui4_align > 0)
                    {
                        _u4AlignBase = prIdxTbl->ui4_align;
                    }
                }
            }
        }
    }

    if (prRangeInfo != NULL)
    {
        _SWDMX_MpgResetData();
        _SWDMX_MpgResetVariable();

        _u8FileSize = (prRangeInfo->z_range_sz + prRangeInfo->ui8_fileoffset);
        _u8StartFileOffset = prRangeInfo->ui8_fileoffset;
        // for resume stop.
        _rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        if(_u8StartFileOffset >= _u8FileSize)
        {
            LOG(3, "_u8StartFileOffset >= _u8FileSize\n");
            if(_u8FileSize > 0)
            {
                _u8StartFileOffset = (_u8FileSize - 1);
            }
            else
            {
                _u8StartFileOffset = 0;
            }
        }
        if(_prSwdmxInfo)
        {
            if((_prSwdmxInfo->fgPlayChange) &&
               (((UINT32)(_prSwdmxInfo->u8SeekPos>>32) != (UINT32)0xFFFFFFFF)
                || ((UINT32)(_prSwdmxInfo->u8SeekPos) != (UINT32)0xFFFFFFFF)))
            {
                _u8SendedSize = _prSwdmxInfo->u8SeekPos;
                _prSwdmxInfo->u8SeekPos = (UINT64)0xFFFFFFFF;
                _prSwdmxInfo->u8SeekPos <<= 32;
                _prSwdmxInfo->u8SeekPos += (UINT64)0xFFFFFFFF;
                _prSwdmxInfo->fgPlayChange = FALSE;
                UNUSED(VDEC_SetRenderFromPos(ES0, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderBackStep(ES0, FALSE, 0, 0, 0));
                UNUSED(VDEC_SetRenderFromFirstPic(ES0, FALSE, _u8FileSize));
            }
            else
            {
                _u8SendedSize = prRangeInfo->ui8_fileoffset;
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgSetRange _prSwdmxInfo null\n");
        }

        _rSwdmxMpgInfo.rVar.e_mm_src_type = MM_SRC_TYPE_HIGH_SPEED_STORAGE;
        if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
        {
            _rSwdmxMpgInfo.rVar.e_mm_src_type =
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->e_mm_src_type;
            if(_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_VUDU)
            {
                _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
                _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            }
        }

#ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            if(prRangeInfo->pt_idxtbl_lst)
            {
                if(prRangeInfo->pt_idxtbl_lst->u.pt_ts_idx_tbl_entry)
                {
                    x_memcpy(&_rSwdmxMpgInfo.rVar.rTickCtl,
                        &prRangeInfo->pt_idxtbl_lst->u.pt_ts_idx_tbl_entry->t_tick_ctrl_blk,
                        sizeof(PVR_TICK_CTRL_BLK_T));

                    _rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address = VIRTUAL(_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address);
                    _rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address = VIRTUAL(_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address);

                    if((UINT32)_rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size != (UINT32)sizeof(SWDMX_MPG_PVR_TICK_INDEX_T))
                    {
                        LOG(0, "ui4_entry_size(%d) != sizeof(SWDMX_MPG_PVR_TICK_INDEX_T)(%d)\n",
                            _rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size,
                            sizeof(SWDMX_MPG_PVR_TICK_INDEX_T));
                    }

                    if((UINT32)((UINT32)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address -
                                (UINT32)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address) !=
                        (UINT32)((_rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num) *
                                 _rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size))
                    {
                        LOG(0, "pv_end_address(0x%x) - pv_start_address(0x%x) != ui4_entry_num(%d) * ui4_entry_size(%d)\n",
                            _rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address,
                            _rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address,
                            _rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num,
                            _rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_size);
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
            _rSwdmxMpgInfo.rVar.u4FirstPTS = MPG_STC_BASE;
            _rSwdmxMpgInfo.rVar.u4LastPTS = MPG_STC_BASE;
            _u8FileSize = 0x0FFFFFFF;
            _u8FileSize <<= 32;
            _u8FileSize += 0xFFFFFFFF;
            _u8StartFileOffset = 0;
            _u8SendedSize =
                (UINT64)_rSwdmxMpgInfo.rVar.rTickCtl.ui4_lba_init *
                 (UINT64)MPG_LBA_BLOCK_SIZE ;

            {
                UINT64 u8Temp;
                if(_rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num > 0)
                {
                    UINT64 u8Remainder;
                    u8Temp = _SWDMX_Div6464(
                        _u8SendedSize,
                        (UINT32)((UINT64)_rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num *
                                 (UINT64)MPG_LBA_BLOCK_SIZE),
                        &u8Remainder);
                    u8Temp = u8Remainder;
                }
                else
                {
                    LOG(3, "ui4_fifo_pkt_num = 0\n");
                    u8Temp = 0;
                }

                if(_rSwdmxMpgInfo.rVar.fgFirst)
                {
                    LOG(5, "SetRange File Pos 0x%llx\n",
                        (u8Temp + _rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset));
                }
                UNUSED(u8Temp);
            }

            _u8SendedSize +=
                 _rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset;
        }
#endif
        _u8ReadedPos = _u8SendedSize;
        _u8PlayPos = _u8SendedSize;
        _rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
        //_u4AvailSize = 0;
        _rSwdmxMpgInfo.rVar.fgVideoPTSFound = FALSE;
        _rSwdmxMpgInfo.rVar.u8PreviousIPos = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousIPosWithPTSFound = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8Previous2IPosWithPTSFound = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSize = _u8SendedSize;
        _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = 0;
        // move up
        //_rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
        _rSwdmxMpgInfo.rVar.u4TotalTime = (UINT32)prRangeInfo->ui8_pb_duration;
        _rSwdmxMpgInfo.rVar.u4TotalTimeOffset = 0;
        _rSwdmxMpgInfo.rVar.u4FirstPTS = ((UINT32)prRangeInfo->t_pts_offset);
        _rSwdmxMpgInfo.rVar.u4FirstVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4SecondVideoPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4FirstAudioModifyPTS = 0xFFFFFFFF;
        _rSwdmxMpgInfo.rVar.u4LastAudioPTS = 0;
        LOG(5, "_SWDMX_MpgSetRange _u8FileSize 0x%llx, _u8SendedSize 0x%llx, u4TotalTime %d, u4FirstPTS %d\n",
            _u8FileSize, _u8SendedSize,
            (UINT32)_rSwdmxMpgInfo.rVar.u4TotalTime,
            (UINT32)_rSwdmxMpgInfo.rVar.u4FirstPTS);

        // DTV00211081 [MM Compatibility]Video FB for a moment after FF then play with special file
        // For small file with 10 second up duration.
        // divide send data size to a small one.
        // Or when we send one time we will get 20 frames.
        // We can't identify the same one when we resend data.
        if((_u4OneTimeSize >= MPG_BATCH_SIZE) &&
           (_u8FileSize < (1024*1024)) &&
           ((_rSwdmxMpgInfo.rVar.u4TotalTime > (90000*10)) ||
            (_rSwdmxMpgInfo.rVar.u4TotalTime == 0)))
        {
            _u4OneTimeSize /= (UINT32)MPG_BATCH_TIME_SMALL_FILE;
            LOG(3, "_SWDMX_MpgSetRange use small send size\n");
        }
        if((_u4OneTimeSize >= MPG_BATCH_SIZE) &&
           ((_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_RHAPSODY) ||
            ((_rSwdmxMpgInfo.rVar.e_mm_src_type ==
                MM_SRC_TYPE_NETWORK_CMPB) &&
             (!_rSwdmxMpgInfo.rFmt.fgVideo))))
        {
            _u4OneTimeSize /= (UINT32)MPG_BATCH_TIME_RHAPSODY;
            _u4OneTimeReadSize /= (UINT32)MPG_BATCH_TIME_RHAPSODY;
            LOG(3, "_SWDMX_MpgSetRange use RHAPSODY send size\n");
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

        #ifdef SWDMX_MPG_USE_INDEX
        if(_rSwdmxMpgInfo.rVar.fgUseIdx)
        {
            if(!_SWDMX_MpgIdxOpen(
                (UINT32)(SWDMX_MPG_IDX_TYPE_POSITION | SWDMX_MPG_IDX_TYPE_TIME),
                _rSwdmxMpgInfo.rVar.u4TotalTime, _u8FileSize))
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
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange)
{
    INT32 i4Ret;
    //LOG(9, "_SWDMX_MpgHeaderParse\n");

    i4Ret = _SWDMX_MpgSendData(prRangeList, i4PlaySpeed, u8SeekPos, fgSpeedChange);
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
BOOL _SWDMX_MpgStop(VOID)
{
    LOG(5, "_SWDMX_MpgStop\n");

#ifdef TIME_SHIFT_SUPPORT
    UNUSED(DMX_PVRPlay_SetUseDescramble(FALSE));
#endif

#ifdef TIME_SHIFT_SUPPORT
    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        if(!_SWDMX_PVR_Stop())
        {
            LOG(3, "_SWDMX_MpgStop _SWDMX_PVR_Stop fail\n");
        }
    }

    if(!_SWDMX_PVR_Close())
    {
        return FALSE;
    }
#endif

    _SWDMX_MpgResetData();
    _SWDMX_MpgResetVariable();
    _rSwdmxMpgInfo.rVar.i4CurSpeed = 1000;
    _rSwdmxMpgInfo.rVar.u8JumpStartAddr = 0;
    _fgCheckPSISize = FALSE;
    _fgSendSeqHdr = FALSE; // not parse sequence header
#ifdef SWDMX_MPG_VC1_SUPPORT
    _fgSendSeqHdrVc1 = FALSE;
#endif
    _u8SendedSize = 0;
    _u8ReadedPos = 0;

    #ifdef SWDMX_MPG_USE_INDEX
    if(_rSwdmxMpgInfo.rVar.fgUseIdx)
    {
        UNUSED(_SWDMX_MpgIdxClose());
    }
    #endif

    UNUSED(VDEC_SetGetSeqHdrOnlyEnd(ES0, _rSwdmxMpgInfo.rVar.fgVdecPowerOff));
    _rSwdmxMpgInfo.rVar.fgVdecPowerOff = FALSE;

    return TRUE;
}

/**
*/
BOOL _SWDMX_MpgFlush(VOID)
{
    _SWDMX_MpgResetData();
    return TRUE;
}

BOOL _SWDMX_MpgSetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTickIndex;
#endif

#ifdef TIME_SHIFT_SUPPORT
    if(u1Type == (UINT8)eSWDMX_SET_REC_BUF_RANGE)
    {
        LOG(7, "_SWDMX_MpgSetInfo u1Type = %d, u4Para1 = %d\n", u1Type, u4Para1);
    }
    else
#endif
    {
        LOG(5, "_SWDMX_MpgSetInfo u1Type = %d, u4Para1 = %d\n", u1Type, u4Para1);
    }

    if(u1Type == (UINT8)eSWDMX_SET_VIDEOTYPE)
    {
        _rSwdmxMpgInfo.rFmt.eVideoType =  (ENUM_SWDMX_VDEC_T)u4Para1;

        if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG1_MPEG2)
        {
            return DMX_SetVideoType(DMX_VIDEO_MPEG);
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_H264)
        {
            return DMX_SetVideoType(DMX_VIDEO_H264);
        }
        else if(_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_VC1)
        {
            return DMX_SetVideoType(DMX_VIDEO_VC1);
        }
        else if((_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_DX311) ||
            (_rSwdmxMpgInfo.rFmt.eVideoType == ENUM_SWDMX_VDEC_MPEG4))
        {
            return DMX_SetVideoType(DMX_VIDEO_MPEG4);
        }
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUDIOTYPE)
    {
        _rSwdmxMpgInfo.rFmt.eAudioType = (ENUM_SWDMX_ADEC_T)u4Para1;
        //_rSwdmxMpgInfo.rFmt.eAudio2Type = (ENUM_SWDMX_ADEC_T)u4Para2;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_VIDEO_DECRYPT)
    {
        _rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = (UINT16)u4Para1;
        _rSwdmxMpgInfo.rFmt.u4VideoDescMode = (UINT8)u4Para2;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUDIO_DECRYPT)
    {
        _rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = (UINT16)u4Para1;
        _rSwdmxMpgInfo.rFmt.u4AudioDescMode = (UINT8)u4Para2;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_VID_PID_IDX)
    {
        _rSwdmxMpgInfo.rFmt.u2VideoPidIdx = (UINT16)u4Para1;
        _rSwdmxMpgInfo.rFmt.u1VideoKeyIndex = (UINT8)u4Para2;
        _rSwdmxMpgInfo.rFmt.u4VideoDescMode = u4Para3;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AUD_PID_IDX)
    {
        _rSwdmxMpgInfo.rFmt.u2AudioPidIdx = (UINT16)u4Para1;
        _rSwdmxMpgInfo.rFmt.u1AudioKeyIndex = (UINT8)u4Para2;
        _rSwdmxMpgInfo.rFmt.u4AudioDescMode = u4Para3;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_PCR_PID_IDX)
    {
        //_rSwdmxMpgInfo.rFmt.u2PcrPidIdx = (UINT16)u4Para1;
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE)
    {
        _fgCheckPSISize = ((u4Para1 > 0) ? TRUE : FALSE);
        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_PID_INDEX_START)
    {
        _rSwdmxMpgInfo.rFmt.u2PidIdxStart = (UINT16)(u4Para1);
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

        _SWDMX_LockRangeSema();

        x_memcpy(&_rSwdmxMpgInfo.rVar.rRangeInfo,
            (MM_REC_BUF_RANGE_INFO_T*)u4Para1, sizeof(MM_REC_BUF_RANGE_INFO_T));

        _rSwdmxMpgInfo.rVar.u4TickBase =
            _rSwdmxMpgInfo.rVar.rTickCtl.ui4_tick_period;
        if(_rSwdmxMpgInfo.rVar.u4TickBase == 0)
        {
            LOG(1, "_SWDMX_MpgSetInfo ui4_tick_period 0\n");
            //ASSERT(0);
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        // start addr
        prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
        if(prStartAddr == NULL)
        {
            LOG(3, "_SWDMX_MpgSetInfo pv_start_address null\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        // end addr
        prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
        if(prEndAddr == NULL)
        {
            LOG(3, "_SWDMX_MpgSetInfo pv_end_address null\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        //  0  u8FifoOffset (u8FifoStart)   lba_init (Start lba)    u8FifoSize
        // (Max file size = u8FifoSize + u8FifoOffset)
        _rSwdmxMpgInfo.rVar.u8FifoOffset =
            _rSwdmxMpgInfo.rVar.rTickCtl.ui8_fifo_offset;

        _rSwdmxMpgInfo.rVar.u8FifoSize =
            (((UINT64)_rSwdmxMpgInfo.rVar.rTickCtl.ui4_fifo_pkt_num)
            *(UINT64)MPG_LBA_BLOCK_SIZE);

        if(_rSwdmxMpgInfo.rVar.u8FifoSize == 0)
        {
            LOG(3, "_SWDMX_MpgSetInfo u8FifoSize 0\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        // safe start
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start);
        if((prTickIndex == NULL) || (prTickIndex >= prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo safe start prTickIndex fail\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }
        _rSwdmxMpgInfo.rVar.u8SafeStartAddr =
            _SWDMX_MpgLBA2Pos((prTickIndex)->ui4_lba);
        _rSwdmxMpgInfo.rVar.u4SafeStartTick =
            prTickIndex->ui4_tick_num;

        // safe end
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
        if((prTickIndex == NULL) || (prTickIndex >= prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo safe end prTickIndex fail\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }
        _rSwdmxMpgInfo.rVar.u8SafeEndAddr =
            _SWDMX_MpgLBA2Pos((prTickIndex)->ui4_lba);
        _rSwdmxMpgInfo.rVar.u4SafeEndTick =
            prTickIndex->ui4_tick_num;

        // valid start
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
        if((prTickIndex == NULL) || (prTickIndex >= prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo valid start prTickIndex fail\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }
        _rSwdmxMpgInfo.rVar.u8ValidStartAddr =
            _SWDMX_MpgLBA2Pos((prTickIndex)->ui4_lba);
        _rSwdmxMpgInfo.rVar.u4ValidStartTick =
            prTickIndex->ui4_tick_num;

        // valid end
        prTickIndex = (SWDMX_MPG_PVR_TICK_INDEX_T*)
            (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
        if((prTickIndex == NULL) || (prTickIndex >= prEndAddr))
        {
            LOG(3, "_SWDMX_MpgSetInfo valid end prTickIndex fail\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        _rSwdmxMpgInfo.rVar.u8ValidEndAddr =
            _SWDMX_MpgLBA2Pos((prTickIndex)->ui4_lba);
        _rSwdmxMpgInfo.rVar.u4ValidEndTick =
            prTickIndex->ui4_tick_num;

        _rSwdmxMpgInfo.rVar.fgMonitorValidStart = TRUE;
        _rSwdmxMpgInfo.rVar.fgMonitorValidEnd = TRUE;
        _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
        _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
        _rSwdmxMpgInfo.rVar.fgValidRange = TRUE;

        if(_prSwdmxInfo)
        {
            if(_prSwdmxInfo->eSwDmxStatus == eSWDMX_STATE_PAUSE)
            {
                if((_u8ReadedPos) <
                    (_rSwdmxMpgInfo.rVar.u8SafeStartAddr))
                {
                    if((_rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
                       (!_prSwdmxInfo->fgPlayChange) && // not changing speed
                       (_rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                    {
                        LOG(3, "UpdateRange fgMonitorSafeStart addr 0x%llx safe start addr 0x%llx\n",
                            _u8ReadedPos,
                            _rSwdmxMpgInfo.rVar.u8SafeStartAddr);
                        if((_prSwdmxInfo) &&
                           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                        {
                            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                            {
                                if(_prSwdmxInfo->pfnSwdmxRangeCb)
                                {
                                    _prSwdmxInfo->pfnSwdmxRangeCb(
                                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                        MM_OUT_OF_SAFE_RANGE_START,
                                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF);
                                }
                                else
                                {
                                    LOG(3, "pfnSwdmxRangeCb null\n");
                                }
                            }
                        }
                    }
                }

                if(_u8ReadedPos < _rSwdmxMpgInfo.rVar.u8ValidStartAddr)
                {
                    if((_rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
                       (!_prSwdmxInfo->fgPlayChange) && // not changing speed
                       (_rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                    {
                        LOG(3, "UpdateRange fgMonitorValidStart addr 0x%llx valid start addr 0x%llx\n",
                            _u8ReadedPos,
                            _rSwdmxMpgInfo.rVar.u8ValidStartAddr);
                        if((_prSwdmxInfo) &&
                           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
                        {
                            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                            {
                                if(_prSwdmxInfo->pfnSwdmxRangeCb)
                                {
                                    _prSwdmxInfo->pfnSwdmxRangeCb(
                                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                                        MM_REACH_VALID_RANGE_START,
                                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                                        0xFFFFFFFF);
                                }
                                else
                                {
                                    LOG(3, "pfnSwdmxRangeCb null\n");
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            LOG(3, "_SWDMX_MpgSetInfo _prSwdmxInfo null\n");
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }

        _SWDMX_UnlockRangeSema();

        return TRUE;
    }
    else if(u1Type == (UINT8)eSWDMX_PRINT_REC_BUF_STATUS)
    {
        _SWDMX_LockRangeSema();
        LOG(0, "Monitor: Safe start(%d) end(%d), Valid start(%d) end(%d)\n",
            _rSwdmxMpgInfo.rVar.fgMonitorSafeStart,
            _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd,
            _rSwdmxMpgInfo.rVar.fgMonitorValidStart,
            _rSwdmxMpgInfo.rVar.fgMonitorValidEnd);
        LOG(0, "Range Addr: 0x%llx 0x%llx 0x%llx 0x%llx\n",
            _rSwdmxMpgInfo.rVar.u8ValidStartAddr,
            _rSwdmxMpgInfo.rVar.u8SafeStartAddr,
            _rSwdmxMpgInfo.rVar.u8SafeEndAddr,
            _rSwdmxMpgInfo.rVar.u8ValidEndAddr);
        LOG(0, "Range num (0x%x-0x%x): %d %d %d %d\n",
            (UINT32)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address,
            (UINT32)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address,
            _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
            _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start,
            _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end,
            _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
        LOG(0, "Range Tick (%d): %d %d %d %d\n",
            _rSwdmxMpgInfo.rVar.u4TickBase,
            _rSwdmxMpgInfo.rVar.u4ValidStartTick,
            _rSwdmxMpgInfo.rVar.u4SafeStartTick,
            _rSwdmxMpgInfo.rVar.u4SafeEndTick,
            _rSwdmxMpgInfo.rVar.u4ValidEndTick);
        LOG(0, "Fifo: Start 0x%llx, Size 0x%llx\n",
            _rSwdmxMpgInfo.rVar.u8FifoOffset,
            _rSwdmxMpgInfo.rVar.u8FifoSize);
        _SWDMX_UnlockRangeSema();
    }
    else if(u1Type == (UINT8)eSWDMX_SET_MONITOF_LBA)
    {
        if(u4Para1 == (UINT32)0xFFFFFFFF)
        {
            _rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
            _rSwdmxMpgInfo.rVar.u8MonitorAddr = 0;
        }
        else
        {
            _rSwdmxMpgInfo.rVar.fgMonitorLBA = TRUE;
            _rSwdmxMpgInfo.rVar.u8MonitorAddr = _SWDMX_MpgLBA2Pos(u4Para1);
        }
        return TRUE;
    }
#endif
    else if(u1Type == (UINT8)eSWDMX_PRINT_INTERNAL_STATUS)
    {
#ifdef TIME_SHIFT_SUPPORT
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
        {
            VDP_PIC_INFO_T rVdpPicInfo;
            UCHAR ucVdpId = 0; // B2R_1
            UINT32 u4TickAudio = 0;
            UINT32 u4TickVideo = 0;
            if((_rSwdmxMpgInfo.rFmt.fgVideo) &&
               (VDP_GetPicInfo(ucVdpId, &rVdpPicInfo) == VDP_SET_OK))
            {
                u4TickVideo = rVdpPicInfo.u4TickNum;
            }
            if(_rSwdmxMpgInfo.rFmt.fgAudio)
            {
                UNUSED(AUD_MMFindTickInfo(0, &u4TickAudio));
            }

            LOG(0, "CurTickNum: Video(%d) Audio(%d) Swdmx(%d)\n",
                u4TickVideo, u4TickAudio, _rSwdmxMpgInfo.rVar.u4CurTickNum);
        }
#endif
        LOG(0, "Cur: _u8SendedSize(0x%llx) _u8ReadedPos(0x%llx)\n",
            _u8SendedSize,
            _u8ReadedPos);
        LOG(0, "rBuf: u4BufTotalSize(%d) u4ReqTotalSize(%d)\n",
            _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize);
#ifdef CC_DMX_PURE_AUDIO
        if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
        {
            LOG(0, "AvailableAFifo(%d) AvailableVFifo(%d)\n",
                DMX_MM_GetBufEmptySize(
                    _u1PidxAudio),
                DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO));
        }
        else
#endif
        {
            LOG(0, "AvailableAFifo(%d) AvailableVFifo(%d)\n",
                DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO),
                DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO));
        }
        LOG(0, "Speed: %d\n",
            _rSwdmxMpgInfo.rVar.i4CurSpeed);
    }
    else if(u1Type == (UINT8)eSWDMX_SET_AOUT_ENABLE)
    {
        _rSwdmxMpgInfo.rVar.fgEnableAudio = TRUE;
        LOG(5, "Call AUD_MMAoutEnable from VDP\n");
    }

    UNUSED(u4Para2);
    UNUSED(u4Para3);
    return FALSE;
}


BOOL _SWDMX_MpgGetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3)
{
    UINT64 *pu8Tmp;
    UINT64 u8Offset, u8OffsetI;
    UINT16 u2DecodingOrder;
    INT32 i4TempRef;
    LOG(6, "_SWDMX_MpgGetInfo u1Type = %d\n", u1Type);

    if(u1Type == (UINT8)eSWDMX_SET_CHK_PSI_BUF_SIZE)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = (UINT32)_fgCheckPSISize;
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

            u8CurPos = _u8SendedSize;
            if(_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                u8Offset = 0;
                u8OffsetI = 0;
                u2DecodingOrder = 0;
                i4TempRef = 0;
                if(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
                {
                    if(u8Offset > _u8FileSize)
                    {
                        LOG(7, "_SWDMX_MpgGetInfo VDP_GetFilePosition u8Offset %d > _u8FileSize %d\n",
                            (UINT32)u8Offset, (UINT32)_u8FileSize);
                        u8CurPos = _u8FileSize;
                    }
                }
            }
            else if(_rSwdmxMpgInfo.rFmt.fgAudio)
            {
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                   &&
                   (_rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
                   // do not need to consider changing
                   // because we could get our swdmx current position
                   // that's the correct one.
                   //(_prSwdmxInfo) &&
                   //(!_prSwdmxInfo->fgPlayChange) && // not changing speed
                {
                    u8CurPos = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                    // seek end
                    if(_u8FileSize == _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                    {
                        u8CurPos = _u8FileSize;
                    }
                }
                else
                {
                    UINT32 u4DmxAvailSizeAudio;
                    UINT32 au4BufStart[2], au4BufEnd[2];
                    UINT32 u4BufSize;
                    UINT32 u4DataSize;
#ifdef CC_DMX_PURE_AUDIO
                    if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                    {
                        u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                            _u1PidxAudio);
               	    }
                    else
#endif
                    {
                        u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO);
                    }
#ifdef AUD_OLDFIFO_INTF
                    VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                                          &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
                    VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif

                    u4BufSize = au4BufEnd[0] - au4BufStart[0];
                    u4DataSize = u4BufSize - u4DmxAvailSizeAudio;
                    if(_u8SendedSize > u4DataSize)
                    {
                        u8CurPos = (_u8SendedSize - u4DataSize);
                    }
                    else
                    {
                        u8CurPos = 0;
                    }
                    #if 0
                    LOG(0, "---GetInfo %d %d %d (%d-%d)\n",
                        (UINT32)*pu8Tmp, (UINT32)_u8SendedSize, u4DataSize,
                        u4BufSize, u4DmxAvailSizeAudio);
                    #endif
                }
            }

            if ((_rSwdmxMpgInfo.rVar.u4TotalTime > 0) &&
                    (_u8FileSize > 0))
            {
                //90000->90000
                u8Tmp = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset)*(UINT64)90000,
                                  (UINT64)_rSwdmxMpgInfo.rVar.u4TotalTime ,
                                  &u8Remainder);
                if(u8Tmp > 0)
                {
                    *pu4Para1 = (UINT32)_SWDMX_Div6464(u8CurPos*(UINT64)90000,
                                               u8Tmp, &u8Remainder);
                }
                UNUSED(u8Remainder);
            }
        }

        if(//(_rSwdmxMpgInfo.rFmt.fgAudio) &&
           (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES) &&
           (_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
          )
        {
            MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
            MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
            {
                prIdxTbl = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
                prTblEntry = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
                if((prTblEntry) && (prIdxTbl))
                {
                    //*pu4Para1 = prTblEntry[_rSwdmxMpgInfo.rVar.u4CurIndexNum].ui4_pts;
                    *pu4Para1 = STC_GetStcValue();
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

        *pu8Tmp = _u8SendedSize;
        if(_rSwdmxMpgInfo.rFmt.fgVideo)
        {
            u8Offset = 0;
            u8OffsetI = 0;
            u2DecodingOrder = 0;
            i4TempRef = 0;
		    if(_rSwdmxMpgInfo.rVar.fgEnableVideo)
			{		
               if(VDP_GetFilePosition(B2R_1, &i4TempRef, &u2DecodingOrder, &u8Offset, &u8OffsetI))
               {
                  if(u8Offset > _u8FileSize)
                  {
                    LOG(7, "_SWDMX_MpgGetInfo VDP_GetFilePosition u8Offset 0x%llx > _u8FileSize 0x%llx\n",
                        u8Offset, _u8FileSize);
                    *pu8Tmp = _u8FileSize;
                  }
                }
		    } 
			else       //video has not play
			{
				*pu8Tmp=0;
			}
        }
        else if(_rSwdmxMpgInfo.rFmt.fgAudio)
        {
            if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
               &&
               (_rSwdmxMpgInfo.rVar.i4CurSpeed < 0))
               // do not need to consider changing
               // because we could get our swdmx current position
               // that's the correct one.
               //(_prSwdmxInfo) &&
               //(!_prSwdmxInfo->fgPlayChange) && // not changing speed
            {
                *pu8Tmp = _rSwdmxMpgInfo.rVar.u8JumpStartAddr;
                // seek end
                if(_u8FileSize == _rSwdmxMpgInfo.rVar.u8JumpStartAddr)
                {
                    *pu8Tmp = 0;
                }
            }
            else
            {
                UINT32 u4DmxAvailSizeAudio;
                UINT32 au4BufStart[2], au4BufEnd[2];
                UINT32 u4BufSize;
                UINT32 u4DataSize;
#ifdef CC_DMX_PURE_AUDIO
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    u4DmxAvailSizeAudio = DMX_MM_GetBufEmptySize(
                        _u1PidxAudio);
           	    }
                else
#endif
                {
                    //u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO)
                    //DTV00214257  should add 1880 (10 pes) as  sub by dmx
                    u4DmxAvailSizeAudio = DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO)+1880;
                    //u4DmxAvailSizeAudio = DMX_GetMinFreeDRAMBufferSize(DMX_PID_TYPE_ES_AUDIO);
                }
#ifdef AUD_OLDFIFO_INTF
                VERIFY(AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0],
                    &au4BufStart[1], &au4BufEnd[1]) == AUD_OK);
#else
                VERIFY(AUD_GetAudFifo(AUD_DEC_MAIN, &au4BufStart[0], &au4BufEnd[0]) == AUD_OK);
#endif
                u4BufSize = au4BufEnd[0] - au4BufStart[0];
                u4DataSize = u4BufSize - u4DmxAvailSizeAudio;
                if(_u8SendedSize > u4DataSize)
                {
                    *pu8Tmp = (_u8SendedSize - u4DataSize);
                }
                else
                {
                    *pu8Tmp = 0;
                }
                #if 0
                LOG(0, "---GetInfo %d %d %d (%d-%d)\n",
                    (UINT32)*pu8Tmp, (UINT32)_u8SendedSize, u4DataSize,
                    u4BufSize, u4DmxAvailSizeAudio);
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
        if((!_rSwdmxMpgInfo.rFmt.fgVideo) &&
           (_rSwdmxMpgInfo.rFmt.fgAudio)&&
           (_rSwdmxMpgInfo.rVar.fgEnableAudio))      //DTV0077970  for audio only program, when nosignal status, we use u4CurTickNum,
                                                     //but not AUD_MMFindTickInfo
        {
            // pu4Para1 will be set in AUD_MMFindTickInfo
            if(!AUD_MMFindTickInfo(0, pu4Para1))
            {
                *pu4Para1 = _rSwdmxMpgInfo.rVar.u4CurTickNum;
            }
        }
        else
        {
            *pu4Para1 = _rSwdmxMpgInfo.rVar.u4CurTickNum;
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
		if(_u8SendedSize>=_rSwdmxMpgInfo.rVar.u8FifoOffset)
		{
			u8Tmp = _SWDMX_Div6464(_u8SendedSize-_rSwdmxMpgInfo.rVar.u8FifoOffset, (UINT64)(MPG_LBA_BLOCK_SIZE), &u8Remainder);
			// Current MW use UINT32 to get LBA,
			// We might change to UINT64 in the future.
			*pu4Para1 = (UINT32)u8Tmp;
		     //		  x_memcpy((void*)pu4Para1, // UINT64
		      //			  &u8Tmp,
		      //			  sizeof(UINT64));
		 }
		 else
		 {
			*pu4Para1 =0;
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
        if(*pu4Para2 == 0)
        {
            fgForward = FALSE;
        }
        *pu4Para1 = _SWDMX_MpgIsSpeedAvailable(fgForward);

#ifdef SWDMX_MPG_SYNC_DTV_MODE
        if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
#endif
        {
            // to protect trigger audio on right time
            if(((_rSwdmxMpgInfo.rFmt.fgAudio) &&
                (_rSwdmxMpgInfo.rVar.fgGotAudio) &&
                (!_rSwdmxMpgInfo.rVar.fgEnableAudio))
               /*
               ||
               ((_rSwdmxMpgInfo.rFmt.fgVideo) &&
                (_rSwdmxMpgInfo.rVar.fgGotVideo) &&
                (!_rSwdmxMpgInfo.rVar.fgEnableVideo))
               */
              )
            {
                *pu4Para1 = FALSE;
            }
        }
        return TRUE;
    }
    else if(u1Type == eSWDMX_GET_STC_OFFSET)
    {
        if(pu4Para1 == NULL)
        {
            LOG(1, "_SWDMX_MpgGetInfo pu4Para1 = NULL\n");
            return FALSE;
        }
        *pu4Para1 = _rSwdmxMpgInfo.rVar.u4FirstPTS;
        return TRUE;
    }

    UNUSED(pu4Para2);
    UNUSED(pu4Para3);
    return FALSE;
}

BOOL _SWDMX_MpgSetStrmID(UINT8 u1HdrType,
    UINT32 u4StrmID, UINT32 u4StrmSubID)
{
    LOG(5, "_SWDMX_MpgSetStrmID u1HdrType = %d, u4StrmID = %d, u4StrmSubID = %d\n",
        u1HdrType, u4StrmID, u4StrmSubID);

    switch(u1HdrType)
    {
        case eSWDMX_STRM_TYPE_VID:
            _rSwdmxMpgInfo.rFmt.u2VideoId = (UINT16)u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_AUD:
            _rSwdmxMpgInfo.rFmt.u2AudioId = (UINT16)u4StrmID;
            _rSwdmxMpgInfo.rFmt.u2AudioSubId = (UINT16)u4StrmSubID;
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            _rSwdmxMpgInfo.rFmt.u2Audio2Id = (UINT16)u4StrmID;
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxMpgInfo.rFmt.u2SubTitleId = (UINT16)u4StrmID;
            break;
        #ifdef SWDMX_MPG_SET_PCR_ID
        case eSWDMX_STRM_TYPE_PCR:
            _rSwdmxMpgInfo.rFmt.u2PcrId = (UINT16)u4StrmID;
            break;
        #endif
        default:
            return FALSE;
    }

    return TRUE;
}

BOOL _SWDMX_MpgEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc)
{
    BOOL fgRet = TRUE;
    LOG(5, "_SWDMX_MpgEnableStrm, u1StrmType =%d, eFmt = %d\n",
        u1StrmType, _rSwdmxMpgInfo.rFmt.eFmt);

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(!_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                _fgSendSeqHdr = FALSE;
#ifdef SWDMX_MPG_VC1_SUPPORT
                _fgSendSeqHdrVc1 = FALSE;
#endif
                // when current is not 1x
                // force to jump next position
                if(_rSwdmxMpgInfo.rVar.fgJumpping)
                {
                    _rSwdmxMpgInfo.rVar.fgJumpNext = TRUE;
                }
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                   (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
                   ||
                   (_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
                  )
                {
                    // We can't change file position on TS file
                    _fgSendSeqHdr = TRUE;
                }

                _rSwdmxMpgInfo.rVar.fgGotVideo = FALSE;
                _rSwdmxMpgInfo.rVar.fgGotVideoReady = FALSE;
                _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendVideoEos = FALSE;

#ifndef CC_PS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsSetVideoId(_rSwdmxMpgInfo.rFmt.u2VideoId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetVideoId(_rSwdmxMpgInfo.rFmt.u2VideoId,
                        _rSwdmxMpgInfo.rFmt.u1VideoKeyIndex,
                        _rSwdmxMpgInfo.rFmt.u4VideoDescMode);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    fgRet = _SWDMX_MpgEsSetVideoId();
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxMpgInfo.rFmt.fgVideo = TRUE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(!_rSwdmxMpgInfo.rFmt.fgAudio)
            {
                if(_rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X == 0)
                {
                    _rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
                }
                // _rSwdmxMpgInfo.rVar.u4LastPTS might be old value (before seek)
                /*if(_rSwdmxMpgInfo.rVar.u4PreviousPTS == 0)
                {
                    _rSwdmxMpgInfo.rVar.u4PreviousPTS = _rSwdmxMpgInfo.rVar.u4LastPTS;
                }*/


                _rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
                _rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
                _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;
                _rSwdmxMpgInfo.rVar.fgSendAudioEos = FALSE;
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
                _rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;
#endif

#ifndef CC_PS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsSetAudioId(_rSwdmxMpgInfo.rFmt.u2AudioId,
                        _rSwdmxMpgInfo.rFmt.u2AudioSubId);
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetAudioId(
                        _rSwdmxMpgInfo.rFmt.u2AudioId, 0,
                        _rSwdmxMpgInfo.rFmt.u1AudioKeyIndex,
                        _rSwdmxMpgInfo.rFmt.u4AudioDescMode);
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    fgRet = _SWDMX_MpgEsSetAudioId();
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxMpgInfo.rFmt.fgAudio = TRUE;
                }

                // There is a case, MW will open audio stream after we deliver video EOS
                // Call handle EOS to send audio eos for this propose
                if(_prSwdmxInfo)
                {
                    if((_rSwdmxMpgInfo.rVar.fgSendEOF) &&
                       (_prSwdmxInfo->eSwDmxStatus == eSWDMX_STATE_PLAY))
                    {
                        x_thread_delay(1); // let handle eos finish
                        LOG(5, "_SWDMX_MpgEnableStrm _SWDMX_MpgHandleEOS\n");
                        _SWDMX_MpgHandleEOS(_rSwdmxMpgInfo.rVar.i4CurSpeed);
                    }
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD2:
            if(!_rSwdmxMpgInfo.rFmt.fgAudio2)
            {
                //_rSwdmxMpgInfo.rVar.u8PreviousSendedSizeOfNon1X = _u8SendedSize;
                //_rSwdmxMpgInfo.rVar.u4PreviousPTS = 0;
                //_rSwdmxMpgInfo.rVar.fgGotAudio = FALSE;
                //_rSwdmxMpgInfo.rVar.fgGotAudioReady = FALSE;
                //_rSwdmxMpgInfo.rVar.u4Mp3Cnt = 0;

#ifndef CC_TS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsSetAudioId(
                        _rSwdmxMpgInfo.rFmt.u2Audio2Id, 1,
                        _rSwdmxMpgInfo.rFmt.u1Audio2KeyIndex,
                        _rSwdmxMpgInfo.rFmt.u4Audio2DescMode);
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxMpgInfo.rFmt.fgAudio2 = TRUE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            if(!_rSwdmxMpgInfo.rFmt.fgSubTitle)
            {
                _rSwdmxMpgInfo.rFmt.fgSubTitle = TRUE;
            }
            break;
        case eSWDMX_STRM_TYPE_PCR:
#ifndef CC_TS_DISABLE
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                /*
                if(!_rSwdmxMpgInfo.rFmt.fgPcr)
                {
                    _rSwdmxMpgInfo.rVar.fgGotPcr = FALSE;
                    _rSwdmxMpgInfo.rVar.fgGotPcrReady = FALSE;
                    _rSwdmxMpgInfo.rVar.fgEnablePcr = FALSE;
                    // Currently, MW set PCR via dmx_mw.c, but not dmx_mm_mw.c
                    // It's enough for us to know PCR is enable or not.
                    #ifdef SWDMX_MPG_SET_PCR_ID
                    fgRet = _SWDMX_MpgTsSetPCRId(_rSwdmxMpgInfo.rFmt.u2PcrId);
                    if(fgRet)
                    {
                        _rSwdmxMpgInfo.rFmt.fgPcr = TRUE;
                    }
                    #else
                    _rSwdmxMpgInfo.rFmt.fgPcr = TRUE;
                    #endif
                }
                */
            }
            break;
#endif
        default:
            break;
    }

    LOG(3, "_SWDMX_MpgEnableStrm ret(%d)\n", fgRet);
    UNUSED(pfnCbFunc);
    return TRUE;
}


BOOL _SWDMX_MpgDisableStrm(UINT8 u1StrmType)
{
    BOOL fgRet = TRUE;
    LOG(5, "_SWDMX_MpgDisableStrm u1StrmType = %d\n", u1StrmType);

    switch(u1StrmType)
    {
        case eSWDMX_STRM_TYPE_VID:
            if(_rSwdmxMpgInfo.rFmt.fgVideo)
            {
                _rSwdmxMpgInfo.rVar.fgEnableVideo = FALSE;
                _rSwdmxMpgInfo.rVar.fgChangeVideo = TRUE;

#ifndef CC_PS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsFreeVideoId();
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsFreeVideoId();
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_H264_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG4_VIDEO_ES) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_VC1_ES))
                {
                    fgRet = _SWDMX_MpgEsFreeVideoId();
                }
                else
#endif
                {
                    fgRet = FALSE;
                }
                if(fgRet)
                {
                    _rSwdmxMpgInfo.rFmt.fgVideo = FALSE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_AUD:
            if(_rSwdmxMpgInfo.rFmt.fgAudio)
            {
                UINT8 u1PidxAudio = _u1PidxAudio;
                _rSwdmxMpgInfo.rVar.fgEnableAudio = FALSE;

#ifndef CC_PS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_PS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG1_DAT))
                {
                    fgRet = _SWDMX_MpgPsFreeAudioId();
                }
                else
#endif
#ifndef CC_TS_DISABLE
                if((_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ZERO_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_ENCRYPT_192) ||
                    (_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT))
                {
                    fgRet = _SWDMX_MpgTsFreeAudioId();
                }
                else
#endif
#ifndef CC_ES_DISABLE
                if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_RAW_AUDIO_ES)
                {
                    fgRet = _SWDMX_MpgEsFreeAudioId();
                }
                else
#endif
                {
                    fgRet = FALSE;
                }

                // flush audio data
                // same with
                // _SWDMX_MpgFlushVFifo();
                // UNUSED(DMX_MM_FlushBuffer(_u1PidxVideo));
                {
                    if(u1PidxAudio != 0xFF)
                    {
                        if(_rSwdmxMpgInfo.rVar.u4LastAudioWptr != 0)
                        {
                            BOOL fgRet1;
                            fgRet1 = DMX_UpdateReadPointer(u1PidxAudio, _rSwdmxMpgInfo.rVar.u4LastAudioWptr, _rSwdmxMpgInfo.rVar.u4LastAudioWptr);
                            if(!fgRet1)
                            {
                                LOG(5, "_SWDMX_MpgDisableStrm DMX_UpdateReadPointer Fail\n");
                            }
                            _rSwdmxMpgInfo.rVar.u4LastAudioWptr = 0;
                        }
                        UNUSED(DMX_MM_FlushBuffer(u1PidxAudio));
                    }
                }

                if(fgRet)
                {
                    _rSwdmxMpgInfo.rFmt.fgAudio = FALSE;
                }
            }
            break;
        case eSWDMX_STRM_TYPE_SUBTITLE:
            _rSwdmxMpgInfo.rFmt.fgSubTitle = FALSE;
            break;
        case eSWDMX_STRM_TYPE_PCR:
#ifndef CC_TS_DISABLE
            if(_rSwdmxMpgInfo.rFmt.eFmt == SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
            {
                /*
                if(_rSwdmxMpgInfo.rFmt.fgPcr)
                {
                    _rSwdmxMpgInfo.rVar.fgEnablePcr = FALSE;

                    // Currently, MW set PCR via dmx_mw.c, but not dmx_mm_mw.c
                    // It's enough for us to know PCR is enable or not.
                    #ifdef SWDMX_MPG_SET_PCR_ID
                    fgRet = _SWDMX_MpgTsFreePCRId();
                    if(fgRet)
                    {
                        _rSwdmxMpgInfo.rFmt.fgPcr = FALSE;
                    }
                    #else
                    _rSwdmxMpgInfo.rFmt.fgPcr = FALSE;
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

    LOG(3, "_SWDMX_MpgDisableStrm do nothing\n");
    return TRUE;
}


VOID _SWDMX_MpgSetSwdmxInfo(SWDMX_INFO_T* prSwdmxInfo)
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


BOOL _SWDMX_MpgGetPosByTime(BOOL fgUpdate, UINT32 u4Time, UINT64 *pu8Pos)
{
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_RAW_AUDIO_ES)
    {
        *pu8Pos = 0;
        return FALSE;
    }

    if(//(_rSwdmxMpgInfo.rFmt.fgAudio) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst) &&
       (_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry)
      )
    {
        MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T* prTblEntry = NULL;
        MM_RANGE_ELMT_IDXTBL_T* prIdxTbl = NULL;
        {
            prIdxTbl = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst;
            prTblEntry = _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pt_idxtbl_lst->u.pt_pb_idx_tbl_entry;
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

                LOG(3, "_SWDMX_MpgGetInfo out of last index time %d\n", prTblEntry[prIdxTbl->ui4_number_of_entry-1].ui4_pts);
            }
            else
            {
                UINT64 u8Remainder;
                if (_rSwdmxMpgInfo.rVar.u4TotalTime > 0)
                {
                    *pu8Pos = _SWDMX_Div6464((_u8FileSize - _u8StartFileOffset)*(u4Time),
                                            (UINT64)(_rSwdmxMpgInfo.rVar.u4TotalTime), &u8Remainder);
                    {
                        UNUSED(_SWDMX_Div6464(*pu8Pos, (UINT64)_u4AlignBase, &u8Remainder));
                        *pu8Pos -= u8Remainder;
                    }
                    return TRUE;
                }
                else
                {
                    *pu8Pos = 0;
                }
                *pu8Pos += _u8StartFileOffset;
            }
        }
    }

    UNUSED(fgUpdate);
    return FALSE;
}

BOOL _SWDMX_MpgGetPosByTick(BOOL fgUpdate, UINT32 u4Tick, UINT64 *pu8Pos)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!_rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetPosByTick valid range is not set\n");
        return FALSE;
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetPosByTick prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTick valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTick valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    if(_rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end >=
       _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start)
    {
        if((u4Tick > _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end) ||
           (u4Tick < _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start))
        {
            if(fgUpdate)
            {
                LOG(3, "1 _SWDMX_MpgGetPosByTick u4Tick(%d) not in %d-%d\n",
                    u4Tick,
                    _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
                    _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
            }
            *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
            *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
            *pu8Pos += _u4AlignBase;
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }
    }
    else
    {
        if((u4Tick > _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end) &&
           (u4Tick < _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start))
        {
            if(fgUpdate)
            {
                LOG(3, "2 _SWDMX_MpgGetPosByTick u4Tick(%d) not in %d-%d\n",
                    u4Tick,
                    _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start,
                    _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
            }
            *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
            *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
            *pu8Pos += _u4AlignBase;
            _SWDMX_UnlockRangeSema();
            return FALSE;
        }
    }

    prTargetAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + u4Tick);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetPosByTick prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
        *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
        *pu8Pos += _u4AlignBase;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    // DTV00142188, When ring buffer begins working,offset time jumps step by step to the biggest value.
    // MW change prEndAddr to not include the valid tick information
    // We have to change not to use prEndAddr for meta-data
    if(prTargetAddr >= prEndAddr)
    {
        prTargetAddr -= (prEndAddr - prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
    *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);

    if(fgUpdate)
    {
        _rSwdmxMpgInfo.rVar.u4CurTickNum = prTargetAddr->ui4_tick_num;
        // When we start with seek to last, we only update u4CurTickNum
        // Read need many time to reach curticknum.
        // We should update read tick too.
        // swdmx must be in pause state now for this change read position.
        _rSwdmxMpgInfo.rVar.u4ReadTickNum = prTargetAddr->ui4_tick_num;
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetPosByTick u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    _SWDMX_UnlockRangeSema();
#endif
    return TRUE;
}

BOOL _SWDMX_MpgGetPosByTickNum(BOOL fgUpdate, UINT32 u4TickNum, UINT64 *pu8Pos)
{
#ifdef TIME_SHIFT_SUPPORT
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prValidEnd;
    SWDMX_MPG_PVR_TICK_INDEX_T *prTargetAddr;
#endif
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!_rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetPosByTickNum valid range is not set\n");
        return FALSE;
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
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
        *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
        *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
        *pu8Pos += _u4AlignBase;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetPosByTickNum prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
        *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
        *pu8Pos += _u4AlignBase;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    if(prTargetAddr >= prEndAddr)
    {
        prTargetAddr -= (prEndAddr - prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
    *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);

    if(fgUpdate)
    {
        _rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetPosByTickNum u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    _SWDMX_UnlockRangeSema();
#endif
    return TRUE;
}

BOOL _SWDMX_MpgGetIPosByTickNum(UINT64 u8Start, UINT32 u4TickNum, UINT64 *pu8Pos)
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
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        *pu8Pos = 0;
        return FALSE;
    }
    fgRet = FALSE;
#ifdef TIME_SHIFT_SUPPORT

    i4LBAOffset = 0;
    if(!_rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetIPosByTickNum valid range is not set\n");
        return FALSE;
    }
    if(_rSwdmxMpgInfo.rVar.fgNoUseIFrmInfo)
    {
        return FALSE;
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
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
        *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
        *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
        *pu8Pos += _u4AlignBase;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetIPosByTickNum prTargetAddr null\n");
        *pu8Pos = _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba);
        *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);
        *pu8Pos += _u4AlignBase;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    if(prTargetAddr >= prEndAddr)
    {
        prTargetAddr -= (prEndAddr - prStartAddr);
    }
    *pu8Pos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
    if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetAddr, PVR_TICK_INDEX_FLAG_HAS_I_FRAME))
    {
        #ifndef SWDMX_MPG_USE_MTMPLAYER_PVR
        if((prTargetAddr->pt_tick_detail) && (VIRTUAL(prTargetAddr->pt_tick_detail)))
        {
            UINT32 u4Tmp;
            UINT32 u4PicType = 0x3;
            UINT32 u4Lba = 0;
            UINT32 u4EntryNum = (UINT32)(PVR_TICK_DETAIL_GET_ENTRIES((PVR_TICK_DETAIL_T *)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)))));
            u4Lba = prTargetAddr->ui4_lba;
            if(u4EntryNum > 100)
            {
                u4EntryNum = 0; // error handle
                LOG(3, "_SWDMX_MpgGetIPosByTickNum u4EntryNum > 100\n");
            }
            for(u4Tmp = 0 ; u4Tmp < u4EntryNum ; u4Tmp++)
            {
                if(((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))) == 0)
                {
                    LOG(3, "_SWDMX_MpgGetIPosByTickNum GET_PIC_TYPE null\n");
                    break;
                }
                if((!prTargetAddr->pt_tick_detail) || (!VIRTUAL(prTargetAddr->pt_tick_detail)))
                {
                    LOG(3, "_SWDMX_MpgGetIPosByTickNum pt_tick_detail null\n");
                    break;
                }
                u4PicType = PVR_PIC_INFO_GET_PIC_TYPE((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T))));
                if(u4PicType == 0) // I frame
                {
                    PVR_PIC_INFO_GET_LBA_OFFSET(((PVR_PIC_INFO_T*)((UINT32)(VIRTUAL(prTargetAddr->pt_tick_detail)) + sizeof(PVR_TICK_DETAIL_T) + (u4Tmp * sizeof(PVR_PIC_INFO_T)))), &i4LBAOffset);
                    if(i4LBAOffset < 0)
                    {
                        if(u4Lba < (-i4LBAOffset))
                        {
                            LOG(3, "_SWDMX_MpgGetIPosByTickNum (u4Lba < (-i4LBAOffset))\n");
                            //ASSERT(0);
                            break;
                        }
                    }
                    if((_rSwdmxMpgInfo.rVar.i4CurSpeed >= 0) &&
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
                    else if((_rSwdmxMpgInfo.rVar.i4CurSpeed < 0) &&
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
        }
        #endif
    }
    *pu8Pos += (i4LBAOffset*MPG_LBA_BLOCK_SIZE);
    *pu8Pos = _SWDMX_MpgTSAlignDown(*pu8Pos);

    #if 0
    if(fgUpdate)
    {
        _rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        LOG(5, "_SWDMX_MpgGetIPosByTickNum u8Pos = 0x%llx\n", (UINT64)*pu8Pos);
    }
    #endif
    _SWDMX_UnlockRangeSema();
#endif
    return fgRet;
}

BOOL _SWDMX_MpgGetTickByPos(BOOL fgUpdate, UINT64 u8Pos, INT32 i4PlaySpeed, UINT32 *pu4TickNum)
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
    *pu4TickNum = 0;
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT

    if(!_rSwdmxMpgInfo.rVar.fgValidRange)
    {
        LOG(9, "_SWDMX_MpgGetTickByPos valid range is not set\n");
        return FALSE;
    }

    if(u8Pos < _rSwdmxMpgInfo.rVar.u8FifoOffset)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos (u8Pos < _rSwdmxMpgInfo.rVar.u8FifoOffset) fail\n");
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prSafeEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
    if((prSafeEnd == NULL) ||
       (prSafeEnd >= prEndAddr) ||
       (prSafeEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos safe end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    // [DTV00141987] After fast rewind,the fast forward to right boundary ,audio isn't smooth
    // We should not use fast mode when out of safe range
    // = should be include. So we could have complete 2 tick between safe and valid
    if((u8Pos >= (((UINT64)prSafeEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                  _rSwdmxMpgInfo.rVar.u8FifoOffset))
      )
    {
        //LOG(5, "_SWDMX_MpgGetTickByPos u8Pos(0x%llx) bigger than safe end 0x%llx\n",
        //    u8Pos,
        //    ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE),
        //    ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE));

        /*
        if(fgUpdate)
        {
            _rSwdmxMpgInfo.rVar.fgOutOfSafeRange = TRUE;
        }
        */
    }

    if((u8Pos >= (((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) + _rSwdmxMpgInfo.rVar.u8FifoOffset)) ||
       (u8Pos < (((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE) + _rSwdmxMpgInfo.rVar.u8FifoOffset)))
    {
        LOG(3, "_SWDMX_MpgGetTickByPos u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Pos,
            _SWDMX_MpgLBA2Pos(prValidStart->ui4_lba),
            _SWDMX_MpgLBA2Pos(prValidEnd->ui4_lba));
        *pu4TickNum = prValidStart->ui4_tick_num;

        if(fgUpdate)
        {
            _rSwdmxMpgInfo.rVar.fgOutOfRange = TRUE;
        }

        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    if(_rSwdmxMpgInfo.rVar.prLastTickIndex == NULL)
    {
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
    }
    if(_rSwdmxMpgInfo.rVar.prLastEndTickIndex == NULL)
    {
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
    }

    if(prValidStart > prValidEnd)
    {
        if((_rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) &&
           (_rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
        }
        if((_rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) &&
           (_rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
        }
    }
    else
    {
        if((_rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) ||
           (_rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
        }
        if((_rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) ||
           (_rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
        }
    }

    prTargetAddr = _rSwdmxMpgInfo.rVar.prLastTickIndex;
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prTargetAddr null\n");
        *pu4TickNum = prValidStart->ui4_tick_num;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    prTargetEndAddr = _rSwdmxMpgInfo.rVar.prLastEndTickIndex;
    if(prTargetEndAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgGetTickByPos prTargetEndAddr null\n");
        *pu4TickNum = prValidStart->ui4_tick_num;
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    for(u4Tmp=0;u4Tmp<_rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
    {
        if(i4PlaySpeed >= 0)
        {
            if(prTargetAddr >= prEndAddr)
            {
                prTargetAddr = prStartAddr;
            }
            if(u8Pos <= (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                         _rSwdmxMpgInfo.rVar.u8FifoOffset))
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
                         _rSwdmxMpgInfo.rVar.u8FifoOffset))
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


    for(u4Tmp=0;u4Tmp<_rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
    {
        if(i4PlaySpeed >= 0)
        {
            if(prTargetEndAddr >= prEndAddr)
            {
                prTargetEndAddr = prStartAddr;
            }
            if((u8Pos + (UINT64)(_u4OneTimeSize * MPG_BATCH_READ_SIZE))
                < (((UINT64)prTargetEndAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                   _rSwdmxMpgInfo.rVar.u8FifoOffset))
            {
                break;
            }
            if(SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
                (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD)))
            {
                break;
            }
            if(prTargetEndAddr->ui4_tick_num >= _rSwdmxMpgInfo.rVar.u4SafeEndTick)
            {
                break;
            }
            prTargetEndAddr = prTargetEndAddr + 1;
        }
    }

    if(fgUpdate)
    {
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetEndAddr;
        if((i4PlaySpeed >= 0) &&
           (SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
            (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetEndAddr;
        }
    }

    *pu4TickNum = _rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
    LOG(8, "_SWDMX_MpgGetTickByPos u4TickNum = %d\n", *pu4TickNum);
    _SWDMX_UnlockRangeSema();
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
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return FALSE;
    }
#ifdef TIME_SHIFT_SUPPORT
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prStartAddr >= prEndAddr fail\n");
        return FALSE;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return FALSE;
    }

    _SWDMX_UnlockRangeSema();

    if((u8Pos >= ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE)) ||
       (u8Pos < ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE)))
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Pos,
            ((UINT64)prValidStart->ui4_lba * MPG_LBA_BLOCK_SIZE),
            ((UINT64)prValidEnd->ui4_lba * MPG_LBA_BLOCK_SIZE));
        return FALSE;
    }

    if(_rSwdmxMpgInfo.rVar.prLastTickIndex == NULL)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex null\n");
        return FALSE;
    }
    if(_rSwdmxMpgInfo.rVar.prLastEndTickIndex == NULL)
    {
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
    }

    if(prValidStart > prValidEnd)
    {
        if((_rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) &&
           (_rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex out of range 1\n");
            return FALSE;
        }
        if((_rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) &&
           (_rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastEndTickIndex out of range 2\n");
            return FALSE;
        }
    }
    else
    {
        if((_rSwdmxMpgInfo.rVar.prLastTickIndex > prValidEnd) ||
           (_rSwdmxMpgInfo.rVar.prLastTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastTickIndex out of range 3\n");
            return FALSE;
        }
        if((_rSwdmxMpgInfo.rVar.prLastEndTickIndex > prValidEnd) ||
           (_rSwdmxMpgInfo.rVar.prLastEndTickIndex < prValidStart))
        {
            _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prValidStart;
            LOG(3, "_SWDMX_MpgCheckNoSignal prLastEndTickIndex out of range 4\n");
            return FALSE;
        }
    }

    prTargetAddr = _rSwdmxMpgInfo.rVar.prLastTickIndex;
    if(prTargetAddr == NULL)
    {
        LOG(3, "_SWDMX_MpgCheckNoSignal prTargetAddr null\n");
        return FALSE;
    }

    for(u4Tmp=0;u4Tmp<_rSwdmxMpgInfo.rVar.rTickCtl.ui4_entry_num;u4Tmp++)
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
        _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
        _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
        if((i4PlaySpeed > 0) &&
           (SWDMX_MPG_TICK_FLAG_CHECK(prTargetEndAddr,
            (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))))
        {
            _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetEndAddr;
        }
    }

    *pu4TickNum = _rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
    LOG(8, "_SWDMX_MpgCheckNoSignal u4TickNum = %d\n", *pu4TickNum);
#endif
    return TRUE;
}
#endif

BOOL _SWDMX_MpgIsSpeedAvailable(BOOL fgForward)
{
#ifdef TIME_SHIFT_SUPPORT
    UINT64 u8Position;
    SWDMX_MPG_PVR_TICK_INDEX_T *prStartAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prEndAddr;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeStart;
    SWDMX_MPG_PVR_TICK_INDEX_T *prSafeEnd;
#endif
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return TRUE;
    }

#ifdef TIME_SHIFT_SUPPORT
    // handle no signa case
    if(_rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_NORMAL)
    {
        return TRUE;
    }
    // handle empty case
    else if((!_rSwdmxMpgInfo.rFmt.fgVideo) &&
            // while we are parsing table, not handle this.
            (!_fgCheckPSISize) &&
            // both audio on and off have to check this.
            //(_rSwdmxMpgInfo.rFmt.fgAudio) &&
            (_rSwdmxMpgInfo.rVar.i4CurSpeed != 1000) &&
            (_rSwdmxMpgInfo.rVar.i4CurSpeed != 0))
               // do not need to consider changing
               // because we could get our swdmx current position
               // that's the correct one.
               //(_prSwdmxInfo) &&
               //(!_prSwdmxInfo->fgPlayChange) && // not changing speed
    {
        return TRUE;
    }
#endif

#ifdef TIME_SHIFT_SUPPORT
    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable prStartAddr >= prEndAddr fail\n");
        return TRUE;
    }

    _SWDMX_LockRangeSema();

    prSafeStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_start);
    if((prSafeStart == NULL) ||
       (prSafeStart >= prEndAddr) ||
       (prSafeStart < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return TRUE;
    }

    prSafeEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_safe.ui4_end);
    if((prSafeEnd == NULL) ||
       (prSafeEnd >= prEndAddr) ||
       (prSafeEnd < prStartAddr))
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return TRUE;
    }

    if(_u8SendedSize < _rSwdmxMpgInfo.rVar.u8FifoOffset)
    {
        LOG(3, "_SWDMX_MpgIsSpeedAvailable (_u8SendedSize < _rSwdmxMpgInfo.rVar.u8FifoOffset) fail\n");
        _SWDMX_UnlockRangeSema();
        return TRUE;
    }

    u8Position = (_u8SendedSize);
    // DTV00142192, After repeatedly press"Step Forward" key, video freezes and offset time adds.
    // use the new seek position
    if((_prSwdmxInfo) &&
       (_prSwdmxInfo->fgPlayChange) &&
       (_prSwdmxInfo->u8SeekPos != (UINT64)(-1)) &&
       (_prSwdmxInfo->eSwDmxStatus == eSWDMX_STATE_PAUSE))
    {
        // pause and change position
        u8Position = (_prSwdmxInfo->u8SeekPos);
    }

    if((u8Position > (((UINT64)prSafeEnd->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      _rSwdmxMpgInfo.rVar.u8FifoOffset)) &&
       (fgForward))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(prSafeStart->ui4_lba),
            _SWDMX_MpgLBA2Pos(prSafeEnd->ui4_lba));

        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    if((u8Position < (((UINT64)prSafeStart->ui4_lba * MPG_LBA_BLOCK_SIZE) +
                      _rSwdmxMpgInfo.rVar.u8FifoOffset)) &&
       (!fgForward))
    {
        LOG(5, "_SWDMX_MpgIsSpeedAvailable u8Pos(0x%llx) not in 0x%llx-0x%llx\n",
            u8Position,
            _SWDMX_MpgLBA2Pos(prSafeStart->ui4_lba),
            _SWDMX_MpgLBA2Pos(prSafeEnd->ui4_lba));

        _SWDMX_UnlockRangeSema();
        return FALSE;
    }
    _SWDMX_UnlockRangeSema();
#endif
    return TRUE;
}

INT32 _SWDMX_MpgHandleNoSignel(UINT64 *pu8SeekPos, INT32 i4PlaySpeed)
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
    i4Ret = MPG_SEND_OK;
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return MPG_SEND_FAIL;
    }
#ifdef TIME_SHIFT_SUPPORT
    // Step
    if(i4PlaySpeed == 1)
    {
        SWDMX_Pause();
        if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            _prSwdmxInfo->pfnSwdmxRangeCb(
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                MM_RANGE_SEEK_DONE,
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                0xFFFFFFFF);
        }
        else
        {
            LOG(3, "pfnSwdmxRangeCb null\n");
        }
        LOG(5, "NoSignel: MM_RANGE_SEEK_DONE\n");
    }

    // DTV00214748 we should calibrate _rSwdmxMpgInfo.rVar.u8CurTime as x_thread_delay() is not evry exactly
	if((_rSwdmxMpgInfo.rVar.fgNeedAdjust==TRUE)&&(i4PlaySpeed==1000))
    {
	   HAL_GetTime(&CheckTime);
       u4CurDeltaTime= CheckTime.u4Seconds*1000+ CheckTime.u4Micros/1000 - _rSwdmxMpgInfo.rVar.u8CurTime;
	   if((u4CurDeltaTime-_rSwdmxMpgInfo.rVar.u4OrgtDeltaTime)>300)
	   {
          _rSwdmxMpgInfo.rVar.u8CurTime += (u4CurDeltaTime-_rSwdmxMpgInfo.rVar.u4OrgtDeltaTime);
	      LOG(5,"_SWDMX_MpgHandleNoSignal adjust u8CurTime Delta:%d ms\n",u4CurDeltaTime-_rSwdmxMpgInfo.rVar.u4OrgtDeltaTime);
	   }
	}
	else
	{
	   _rSwdmxMpgInfo.rVar.fgNeedAdjust=FALSE;
	}

    x_thread_delay(1000/MPG_TIMER_DELAY_TIMES);
    if(i4PlaySpeed >= 0)
    {
        _rSwdmxMpgInfo.rVar.u8CurTime +=
            i4PlaySpeed/MPG_TIMER_DELAY_TIMES;
    }
    else
    {
        if(_rSwdmxMpgInfo.rVar.u8CurTime >= ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES))
        {
            _rSwdmxMpgInfo.rVar.u8CurTime -=
                ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES);
        }
        else
        {
            _rSwdmxMpgInfo.rVar.u8CurTime = 0;
        }
    }

    if(_rSwdmxMpgInfo.rVar.u4TickBase > 0)
    {
        u8TickNum = _SWDMX_Div6464(
            _rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(_rSwdmxMpgInfo.rVar.u4TickBase),
            &u8Remainder);
    }
    else
    {
        ASSERT(0);
        u8TickNum = _SWDMX_Div6464(
            _rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(500),  // u8CurTime is milli-second unit
            &u8Remainder);
    }
    ASSERT(u8TickNum < (UINT32)0xFFFFFFFF);
    u4TickNum = (UINT32)u8TickNum;
    _rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
    _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

    // DTV00142071, After plug RF cable again, mini pane time displays incorrectly.
    // set current tick to vdp, so MW will get correct one after plug signal
    if (_rSwdmxMpgInfo.rFmt.fgVideo)
    {
        VDP_SET_PIC_INFO_T rVdpSetPicInfo;
        x_memset(&rVdpSetPicInfo, 0, sizeof(rVdpSetPicInfo));
        rVdpSetPicInfo.u4TickNum = u4TickNum;
        if (VDP_SetPicInfo(B2R_1, rVdpSetPicInfo) != VDP_SET_OK)
        {
            LOG(3, "NoSignel: VDP_SetPicInfo u4TickNum(%d) fail\n", u4TickNum);
        }
    }

    u4LogCnt++;
    if(u4LogCnt >= MPG_TIMER_DELAY_TIMES)
    {
        LOG(5, "NoSignel: CurTickNum %d\n", _rSwdmxMpgInfo.rVar.u4CurTickNum);
        u4LogCnt = 0;
        UNUSED(u4LogCnt);
    }

    if((_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL) ||
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE))
    {
        if(((i4PlaySpeed >= 0) &&
            (_rSwdmxMpgInfo.rVar.u4CurTickNum >=
             _rSwdmxMpgInfo.rVar.u4NoSignalTickNum))
           ||
           ((i4PlaySpeed < 0) &&
            (_rSwdmxMpgInfo.rVar.u4CurTickNum <=
             _rSwdmxMpgInfo.rVar.u4NoSignalTickNum))
          )
        {
            LOG(3, "NoSignel: u4NoSignalStage 2, tick num %d\n",
                u4TickNum);
            if((_prSwdmxInfo) &&
               (_prSwdmxInfo->pfnSwdmxRangeCb) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_SIGNAL_Y_TO_N,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else //(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_BAD_TICK_N_TO_Y,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                }
            }
            if(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_NO_SIGNAL)
            {
                _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_NO_SIGNAL;
            }
            else //(_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_PREPARE_BAD_STORAGE)
            {
                _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_BAD_STORAGE;
            }
        }
    }

    if((_rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_NO_SIGNAL) &&
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage != MPG_TSS_UNDER_BAD_STORAGE))
    {
        LOG(3, "NoSignel: u4NoSignalStage 1, waiting, cur tick %d, ng tick %d\n",
            u4TickNum,
            _rSwdmxMpgInfo.rVar.u4NoSignalTickNum);
        return MPG_SEND_FULL;
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "NoSignel:  prStartAddr >= prEndAddr fail\n");
        return MPG_SEND_FAIL;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "NoSignel: valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "NoSignel: valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        LOG(3, "NoSignel: invalid u4TickNum %d, %d-%d\n", u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        if(i4PlaySpeed >= 0)
        {
            LOG(3, "NoSignel: invalid u4TickNum %d->%d\n", u4TickNum, prValidEnd->ui4_tick_num);
            u4TickNum = prValidEnd->ui4_tick_num;
        }
        else
        {
            LOG(3, "NoSignel: invalid u4TickNum %d->%d\n", u4TickNum, prValidStart->ui4_tick_num);
            u4TickNum = prValidStart->ui4_tick_num;
        }
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "NoSignel: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }
    if(prTargetAddr >= prEndAddr)
    {
        prTargetAddr -= (prEndAddr - prStartAddr);
    }
    if(prTargetAddr == NULL)
    {
        LOG(3, "NoSignel: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }
    _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
    _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;

    if((!(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_EMPTY))) &&
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_NO_SIGNAL))
    {
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(*pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage 0, from have signal tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((_prSwdmxInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                if(_prSwdmxInfo->pfnSwdmxRangeCb)
                {
                    _prSwdmxInfo->pfnSwdmxRangeCb(
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_SIGNAL_N_TO_Y,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF);
                }
                else
                {
                    LOG(3, "pfnSwdmxRangeCb null\n");
                }
            }
        }
    }
    else if((!(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_BAD))) &&
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_UNDER_BAD_STORAGE))
    {
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_NORMAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(*pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage 0, from good storage tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((_prSwdmxInfo) &&
           (_prSwdmxInfo->pfnSwdmxRangeCb) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _prSwdmxInfo->pfnSwdmxRangeCb(
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_BAD_TICK_Y_TO_N,
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF);
            }
        }
    }

    if(((SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_EMPTY))) &&
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_NO_SIGNAL;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(*pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage UNDER_NO_SIGNAL, no signal tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((_prSwdmxInfo) &&
           (_prSwdmxInfo->pfnSwdmxRangeCb) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _prSwdmxInfo->pfnSwdmxRangeCb(
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_SIGNAL_Y_TO_N,
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF);
            }
        }
    }
    else if(((SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
          PVR_TICK_INDEX_FLAG_BAD))) &&
       (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_UNDER_BAD_STORAGE;
        *pu8SeekPos = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
        *pu8SeekPos = _SWDMX_MpgTSAlignDown(*pu8SeekPos);
        LOG(3, "NoSignel: u4NoSignalStage UNDER_BAD_STORAGE, bad tick num %d\n",
            prTargetAddr->ui4_tick_num);
        if((_prSwdmxInfo) &&
           (_prSwdmxInfo->pfnSwdmxRangeCb) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                _prSwdmxInfo->pfnSwdmxRangeCb(
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                    MM_BAD_TICK_N_TO_Y,
                    _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                    0xFFFFFFFF);
            }
        }
    }

    // because here is empty, lba value is invalid
    /*
    if((_rSwdmxMpgInfo.rVar.fgMonitorLBA)
        &&
       (
        ((i4PlaySpeed >= 0) &&
         (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) >= _rSwdmxMpgInfo.rVar.u8MonitorAddr))
        ||
        ((i4PlaySpeed < 0) &&
         (((UINT64)prTargetAddr->ui4_lba * MPG_LBA_BLOCK_SIZE) < _rSwdmxMpgInfo.rVar.u8MonitorAddr))
       )
      )
    {
        LOG(3, "NoSignel: fgMonitorLBA addr 0x%llx in start addr 0x%llx end addr 0x%llx\n",
            _rSwdmxMpgInfo.rVar.u8MonitorAddr,
            u8StartAddr,
            u8EndAddr);
        if((_prSwdmxInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                if(_prSwdmxInfo->pfnSwdmxRangeCb)
                {
                    _prSwdmxInfo->pfnSwdmxRangeCb(
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_MONITOR_LBA_ARRIVAL,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF);
                }
                else
                {
                    LOG(3, "pfnSwdmxRangeCb null\n");
                }
            }
        }
        _rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
    }
    */

    if(prTargetAddr->ui4_tick_num >=
        (_rSwdmxMpgInfo.rVar.u4SafeEndTick))
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "NoSignel: fgMonitorSafeEnd tick %d safe end tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4SafeEndTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_END,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        }
    }
    else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
    {
        _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
    }

    if((prTargetAddr->ui4_tick_num) <
        (_rSwdmxMpgInfo.rVar.u4SafeStartTick))
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "NoSignel: fgMonitorSafeStart tick %d safe start tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4SafeStartTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_START,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        }
    }
    else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
    {
        _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
    }

    if(prTargetAddr->ui4_tick_num >= _rSwdmxMpgInfo.rVar.u4ValidEndTick)
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "NoSignel: fgMonitorValidEnd tick %d valid end tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4ValidEndTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_END,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    if(prTargetAddr->ui4_tick_num < _rSwdmxMpgInfo.rVar.u4ValidStartTick)
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "NoSignel: fgMonitorValidStart tick %d valid start tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4ValidStartTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_START,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    _SWDMX_UnlockRangeSema();
#endif
    return i4Ret;
}

INT32 _SWDMX_MpgHandleEmpty(UINT64 *pu8SeekPos, INT32 i4PlaySpeed)
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
    i4Ret = MPG_SEND_OK;
    if(_rSwdmxMpgInfo.rFmt.eFmt != SWDMX_FMT_MPEG2_TS_TIME_SHIFT)
    {
        return MPG_SEND_FAIL;
    }
#ifdef TIME_SHIFT_SUPPORT
    // Step
    if(i4PlaySpeed == 1)
    {
        SWDMX_Pause();
        if((_prSwdmxInfo->pfnSwdmxRangeCb) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            _prSwdmxInfo->pfnSwdmxRangeCb(
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                MM_RANGE_SEEK_DONE,
                _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                0xFFFFFFFF);
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
        _rSwdmxMpgInfo.rVar.u8CurTime +=
            i4PlaySpeed/MPG_TIMER_DELAY_TIMES;
    }
    else
    {
        if(_rSwdmxMpgInfo.rVar.u8CurTime >= ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES))
        {
            _rSwdmxMpgInfo.rVar.u8CurTime -=
                ((UINT32)(-i4PlaySpeed)/MPG_TIMER_DELAY_TIMES);
        }
        else
        {
            _rSwdmxMpgInfo.rVar.u8CurTime = 0;
        }
    }

    if(_rSwdmxMpgInfo.rVar.u4TickBase > 0)
    {
        u8TickNum = _SWDMX_Div6464(
            _rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(_rSwdmxMpgInfo.rVar.u4TickBase),
            &u8Remainder);
    }
    else
    {
        ASSERT(0);
        u8TickNum = _SWDMX_Div6464(
            _rSwdmxMpgInfo.rVar.u8CurTime,
            (UINT64)(45000),
            &u8Remainder);
    }
    ASSERT(u8TickNum < (UINT32)0xFFFFFFFF);
    u4TickNum = (UINT32)u8TickNum;
    _rSwdmxMpgInfo.rVar.u4CurTickNum = u4TickNum;
    _rSwdmxMpgInfo.rVar.u4ReadTickNum = _rSwdmxMpgInfo.rVar.u4CurTickNum;

    // This function is audio only case. We don't need this.
    #if 0
    {
        VDP_SET_PIC_INFO_T rVdpSetPicInfo;
        x_memset(&rVdpSetPicInfo, 0, sizeof(rVdpSetPicInfo));
        rVdpSetPicInfo.u4TickNum = u4TickNum;
        if (VDP_SetPicInfo(B2R_1, &rVdpSetPicInfo) != VDP_SET_OK)
        {
            LOG(3, "Empty: VDP_SetPicInfo u4TickNum(%d) fail\n", u4TickNum);
        }
    }
    #endif

    u4LogCnt++;
    if(u4LogCnt >= MPG_TIMER_DELAY_TIMES)
    {
        LOG(5, "Empty: CurTickNum %d\n", _rSwdmxMpgInfo.rVar.u4CurTickNum);
        u4LogCnt = 0;
        UNUSED(u4LogCnt);
    }

    prEndAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_end_address;
    prStartAddr = (SWDMX_MPG_PVR_TICK_INDEX_T*)_rSwdmxMpgInfo.rVar.rTickCtl.pv_start_address;
    if(prStartAddr >= prEndAddr)
    {
        LOG(3, "Empty:  prStartAddr >= prEndAddr fail\n");
        return MPG_SEND_FAIL;
    }

    _SWDMX_LockRangeSema();

    prValidStart = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_start);
    if((prValidStart == NULL) ||
       (prValidStart >= prEndAddr) ||
       (prValidStart < prStartAddr))
    {
        LOG(3, "Empty:  valid start prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }

    prValidEnd = (SWDMX_MPG_PVR_TICK_INDEX_T*)
        (prStartAddr + _rSwdmxMpgInfo.rVar.rRangeInfo.t_valid.ui4_end);
    if((prValidEnd == NULL) ||
       (prValidEnd >= prEndAddr) ||
       (prValidEnd < prStartAddr))
    {
        LOG(3, "Empty: valid end prTickIndex fail\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }

    if((u4TickNum >= prValidEnd->ui4_tick_num) ||
       (u4TickNum < prValidStart->ui4_tick_num))
    {
        LOG(3, "Empty: invalid u4TickNum %d, %d-%d\n", u4TickNum, prValidStart->ui4_tick_num, prValidEnd->ui4_tick_num);
        if(i4PlaySpeed >= 0)
        {
            LOG(3, "Empty: invalid u4TickNum %d->%d\n", u4TickNum, prValidEnd->ui4_tick_num);
            u4TickNum = prValidEnd->ui4_tick_num;
        }
        else
        {
            LOG(3, "Empty: invalid u4TickNum %d->%d\n", u4TickNum, prValidStart->ui4_tick_num);
            u4TickNum = prValidStart->ui4_tick_num;
        }
    }

    prTargetAddr = prValidStart + (u4TickNum - prValidStart->ui4_tick_num);
    if(prTargetAddr == NULL)
    {
        LOG(3, "Empty: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }
    if(prTargetAddr >= prEndAddr)
    {
        prTargetAddr -= (prEndAddr - prStartAddr);
    }
    if(prTargetAddr == NULL)
    {
        LOG(3, "Empty: prTargetAddr null\n");
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FAIL;
    }
    _rSwdmxMpgInfo.rVar.prLastTickIndex = prTargetAddr;
    _rSwdmxMpgInfo.rVar.prLastEndTickIndex = prTargetAddr;
    _u8SendedSize = _SWDMX_MpgLBA2Pos(prTargetAddr->ui4_lba);
    _u8SendedSize = _SWDMX_MpgTSAlignDown(_u8SendedSize);
    if(i4PlaySpeed >= 0)
    {
        _u8SendedSize += _u4AlignBase;
    }
    _u8ReadedPos = _u8SendedSize;
    _u8PlayPos = _u8SendedSize;
    _SWDMX_MpgResetData();

    if((SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
        (PVR_TICK_INDEX_FLAG_EMPTY | PVR_TICK_INDEX_FLAG_BAD))) &&
        (_rSwdmxMpgInfo.rVar.u4NoSignalStage == MPG_TSS_NORMAL))
    {
        _rSwdmxMpgInfo.rVar.u8CurTime =
            (UINT64)_rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;
        _rSwdmxMpgInfo.rVar.u8CurTime *=
            _rSwdmxMpgInfo.rVar.u4TickBase;

        _rSwdmxMpgInfo.rVar.u4NoSignalTickNum =
            _rSwdmxMpgInfo.rVar.prLastTickIndex->ui4_tick_num;

        if(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
           (PVR_TICK_INDEX_FLAG_EMPTY)))
        {
            LOG(3, "Empty: u4NoSignalStage 1.2.1 audio only, PREPARE_NO_SIGNAL tick %d\n",
                _rSwdmxMpgInfo.rVar.u4NoSignalTickNum);

            _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_NO_SIGNAL;
        }
        else //if(SWDMX_MPG_TICK_FLAG_CHECK(_rSwdmxMpgInfo.rVar.prLastTickIndex,
             //   (PVR_TICK_INDEX_FLAG_BAD)))
        {
            LOG(3, "Empty: u4NoSignalStage 1.2.2 audio only, PREPARE_BAD_STORAGE tick %d\n",
                _rSwdmxMpgInfo.rVar.u4NoSignalTickNum);

            _rSwdmxMpgInfo.rVar.u4NoSignalStage = MPG_TSS_PREPARE_BAD_STORAGE;
        }
        _SWDMX_UnlockRangeSema();
        return MPG_SEND_FULL;
    }


    if((_rSwdmxMpgInfo.rVar.fgMonitorLBA)
        &&
       (
        ((i4PlaySpeed >= 0) &&
         (_u8SendedSize >= _rSwdmxMpgInfo.rVar.u8MonitorAddr))
        ||
        ((i4PlaySpeed < 0) &&
         (_u8SendedSize < _rSwdmxMpgInfo.rVar.u8MonitorAddr))
       )
      )
    {
        LOG(3, "Empty: fgMonitorLBA addr 0x%llx in start addr 0x%llx end addr 0x%llx\n",
            _rSwdmxMpgInfo.rVar.u8MonitorAddr,
            _u8SendedSize,
            _u8SendedSize);
        if((_prSwdmxInfo) &&
           (_rSwdmxMpgInfo.rVar.prCurRangeReq))
        {
            if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
            {
                if(_prSwdmxInfo->pfnSwdmxRangeCb)
                {
                    _prSwdmxInfo->pfnSwdmxRangeCb(
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                        MM_MONITOR_LBA_ARRIVAL,
                        _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                        0xFFFFFFFF);
                }
                else
                {
                    LOG(3, "pfnSwdmxRangeCb null\n");
                }
            }
        }
        _rSwdmxMpgInfo.rVar.fgMonitorLBA = FALSE;
    }

    if(prTargetAddr->ui4_tick_num >=
        (_rSwdmxMpgInfo.rVar.u4SafeEndTick))
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "Empty: fgMonitorSafeEnd tick %d safe end tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4SafeEndTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_END,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = FALSE;
        }
    }
    else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeEnd)
    {
        _rSwdmxMpgInfo.rVar.fgMonitorSafeEnd = TRUE;
    }

    if((prTargetAddr->ui4_tick_num) <
        (_rSwdmxMpgInfo.rVar.u4SafeStartTick))
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorSafeStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "Empty: fgMonitorSafeStart tick %d safe start tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4SafeStartTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_OUT_OF_SAFE_RANGE_START,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = FALSE;
        }
    }
    else if(!_rSwdmxMpgInfo.rVar.fgMonitorSafeStart)
    {
        _rSwdmxMpgInfo.rVar.fgMonitorSafeStart = TRUE;
    }

    if(prTargetAddr->ui4_tick_num >= _rSwdmxMpgInfo.rVar.u4ValidEndTick)
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorValidEnd) &&
           (i4PlaySpeed >= 0))
        {
            LOG(3, "Empty: fgMonitorValidEnd tick %d valid end tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4ValidEndTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_END,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorValidEnd = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    if(prTargetAddr->ui4_tick_num < _rSwdmxMpgInfo.rVar.u4ValidStartTick)
    {
        if((_rSwdmxMpgInfo.rVar.fgMonitorValidStart) &&
           (i4PlaySpeed < 0))
        {
            LOG(3, "Empty: fgMonitorValidStart tick %d valid start tick %d\n",
                prTargetAddr->ui4_tick_num,
                _rSwdmxMpgInfo.rVar.u4ValidStartTick);
            if((_prSwdmxInfo) &&
               (_rSwdmxMpgInfo.rVar.prCurRangeReq))
            {
                if(_rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo)
                {
                    if(_prSwdmxInfo->pfnSwdmxRangeCb)
                    {
                        _prSwdmxInfo->pfnSwdmxRangeCb(
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->pv_tag,
                            MM_REACH_VALID_RANGE_START,
                            _rSwdmxMpgInfo.rVar.prCurRangeReq->prDmxRangeInfo->ui4_range_id,
                            0xFFFFFFFF);
                    }
                    else
                    {
                        LOG(3, "pfnSwdmxRangeCb null\n");
                    }
                }
            }
            _rSwdmxMpgInfo.rVar.fgMonitorValidStart = FALSE;
        }
        i4Ret = MPG_SEND_FULL;
    }

    _SWDMX_UnlockRangeSema();
#endif
    return i4Ret;
}

BOOL _SWDMX_MpgResetFeederReq(void)          // CR [DTV00211387]  reset feeder request when comes timeout error
{
    UINT32 u4ReqCnt;
    INT32 i4Ret;
    UINT32 u4AlignSize;
    FEEDER_TRANSMIT_COND rFeederCond;

    if (FeederGetBufferInfo(FEEDER_PROGRAM_SOURCE, &_rFeederInfo) !=
      FEEDER_E_OK)
    {
        LOG(1, "Feeder input buffer error.\n");
        return FALSE;
    }
LOG(5,"reset Feeder Req\n");          //bill
#ifdef SWDMX_MPG_DEBUG_BUFFER
    if((_rSwdmxMpgInfo.rBuf.u4BufAvailCnt > 0) || (_rSwdmxMpgInfo.rBuf.u4ReqTimes > 0))
    {
        LOG(7, "Mpg Fd Flh UB %d BT %d RT %d BWI %d BRI %d BAC %d RWI %d RRI %d RT %d UAdr %d\n",
            _rSwdmxMpgInfo.rBuf.u4UsingBufSize,
            _rSwdmxMpgInfo.rBuf.u4BufTotalSize,
            _rSwdmxMpgInfo.rBuf.u4ReqTotalSize,
            _rSwdmxMpgInfo.rBuf.u4BufWriteIdx,
            _rSwdmxMpgInfo.rBuf.u4BufReadIdx,
            _rSwdmxMpgInfo.rBuf.u4BufAvailCnt,
            _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx,
            _rSwdmxMpgInfo.rBuf.u4ReqReadIdx,
            _rSwdmxMpgInfo.rBuf.u4ReqTimes,
            _rSwdmxMpgInfo.rBuf.u4UsingStartAddr);
    }
#endif

    _u4FeederReqID += 50; // always a even value
    //_rSwdmxMpgInfo.rBuf.u4BufAvailCnt = 0;
    //_rSwdmxMpgInfo.rBuf.u4BufReadIdx = 0;
    //_rSwdmxMpgInfo.rBuf.u4BufWriteIdx = 0;
    //_rSwdmxMpgInfo.rBuf.u4BufTotalSize = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTimes = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqReadIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqWriteIdx = 0;
    _rSwdmxMpgInfo.rBuf.u4ReqTotalSize = 0;
    //_rSwdmxMpgInfo.rBuf.u4UsingBufSize = 0;
    //_u8ReadedPos = _u8SendedSize;

    for(u4ReqCnt = 0;u4ReqCnt < _u4FeederReqNum;u4ReqCnt++)
    {
        // make it invalid, not a even value
        _rSwdmxMpgInfo.rBuf.au4ReqId[u4ReqCnt] = 1;
#ifdef SWDMX_MPG_DEBUG_INPUT_DATA
        _rSwdmxMpgInfo.rBuf.au4ReqDup[u4ReqCnt] = 0;
#endif
    }

#if 1//def CC_DLNA_SUPPORT
    FeederSetReadyThreshold(MPG_DLNA_FEEDER_UNREACH);
    FeederSetUnderFlowThreshold(MPG_DLNA_FEEDER_UNREACH);
    FeederSetInfo(FEEDER_SWDMX_THRESHOLD, FALSE);
#endif
    // Update read ptr to feeder
    if(_rSwdmxMpgInfo.rVar.e_mm_src_type == MM_SRC_TYPE_PUSH)
    {
    }
    else if(_rFeederInfo.u4WriteAddr)
    {
        if(FeederUpdateReadPointer(FEEDER_PROGRAM_SOURCE, _rFeederInfo.u4WriteAddr) != FEEDER_E_OK)
        {
            LOG(1, "_SWDMX_MpgResetFeeder FeederUpdateReadPointer fail\n");
        }
    }
    // DDI is 4 byte alignment
    u4AlignSize = (_rFeederInfo.u4WriteAddr % 4);
    if(u4AlignSize > 0)
    {
        LOG(5, "Mpg Fd align _rFeederInfo.u4WriteAddr %d\n", _rFeederInfo.u4WriteAddr);
        i4Ret = FeederSetRequest(FEEDER_PROGRAM_SOURCE,
                (4-u4AlignSize),
                (UINT64)(0),
                &rFeederCond,
                (_u4FeederReqID-25)); // 25 is a past requestID
        if(i4Ret != FEEDER_E_OK)
        {
            LOG(1, "Mpg Fd align FeederSetRequest Fail\n");
            /* not necessary
            _rSwdmxMpgInfo.rVar.fgSendEOF = TRUE;
            if(!_fgGetInfo)
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
    return TRUE;
}

#ifdef TIME_SHIFT_SUPPORT
UINT64 _SWDMX_MpgTSAlignDown(UINT64 u8Position)
{
    UINT64 u8Remainder;
    UNUSED(_SWDMX_Div6464((u8Position - _rSwdmxMpgInfo.rVar.u8FifoOffset), (UINT64)_u4AlignBase, &u8Remainder));
    u8Position -= u8Remainder;

    return u8Position;
}

UINT64 _SWDMX_MpgLBA2Pos(UINT32 u4LBA)
{
    return (((UINT64)(u4LBA) * MPG_LBA_BLOCK_SIZE) + _rSwdmxMpgInfo.rVar.u8FifoOffset);
}
#endif
