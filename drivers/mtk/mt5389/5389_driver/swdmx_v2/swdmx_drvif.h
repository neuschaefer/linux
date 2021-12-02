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
 * $RCSfile: swdmx_drvif.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_drvif.h
 *  Internal interface of software demux driver
 */


#ifndef SWDMX_DRVIF_H
#define SWDMX_DRVIF_H

// This is the version 2 swdmx.

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "swdmx_if.h"
#include "x_mm_common.h"
#include "u_minfo.h"
#include "dmx_mm_if.h"
#include "feeder_if.h"

#ifdef CC_CMPB_PUSH_MODE
#include "../swdmx_v2/swdmx_stream.h"
#include "../swdmx_v2/swdmx_demuxer.h"
#include "../swdmx_v2/swdmx_util.h"
#include "../swdmx_v2/swdmx_pb_ctrl.h"
#endif /*CC_CMPB_PUSH_MODE*/

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#if defined(CC_MT5398)
#define SWDMX_IS_POST_MT5398
#endif

#if defined(CC_MT5389) || defined(SWDMX_IS_POST_MT5398)
#define SWDMX_IS_POST_MT5389
#endif

#if defined(CC_MT5368) || defined(CC_MT5396) || defined(SWDMX_IS_POST_MT5389)
#define SWDMX_IS_POST_MT5368
#endif

#define SWDMX_ADD_RANGE         0
#define SWDMX_DEL_RANGE         1

#define SWDMX_DATA_Q_SIZE          16  // megaa 20100414
#define SWDMX_PLAY_SPEED_1X        1000
#define SWDMX_PLAY_SPEED_2X        2000
#define SWDMX_AUDIO_PREBUF_SIZE 8*288*1024
#define SWDMX_AUDIO_ALLOC_SIZE   256

#if 0  // megaa 20100421
#define SWDMX_FEEDER_TIMEOUT 20000
#else
#define SWDMX_FEEDER_TIMEOUT_HIGH_SPEED 1000
#define SWDMX_FEEDER_TIMEOUT_NETWORK 60000
#endif

#define SWDMX_INTERNAL_BUF_SIZE (50 * 1024)

#define AVI_DIVX_MAX_AUD_TRACK 8
#define MP4_MAX_AUDIO_TRACK 8
#define MP4_MAX_SUB_TRACK 8
#define MP4_MAX_KEY_TABLE 8

#define OGG_MAX_AUDIO_TRACK 8
#define WMV_MAX_AUDIO_TRACK 4

//mkv related
#define MKV_MAX_AUD_TRACK 26 // 1 video + 8 audio + 16 subtitle and 1-base index
#define MKV_MAX_SB_TRACK 26 // 1 video + 8 audio + 16 subtitle and 1-base index
#define MKV_MAX_LEVEL 5
#define MKV_MAX_FRAME_COUNT 256

//********* definitions used in swdmx_mpg**//
//#define SWDMX_MPG_DEBUG_BUFFER
//#define SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
//#define SWDMX_MPG_DEBUG_INPUT_DATA
//#define SWDMX_MPG_ACCEPT_NON_SEQUENCE_ACK_ID
#define SWDMX_MPG_CHECK_MP3_AUDIO
#define SWDMX_MPG_CHECK_AUDIO_SCRAMBLE
//#define SWDMX_MPG_SET_PCR_ID
#define SWDMX_MPG_USE_INDEX
//#define SWDMX_MPG_USE_MTMPLAYER_PVR


#if !defined(CC_TS_VC1_DISABLE) || !defined(CC_ES_VC1_DISABLE)
#define SWDMX_MPG_VC1_SUPPORT
#endif
#if !defined(CC_PS_MPEG2_DISABLE) || !defined(CC_PS_MPEG1_DISABLE) \
 || !defined(CC_TS_MPEG2_DISABLE) || !defined(CC_TS_MPEG1_DISABLE) \
 || !defined(CC_ES_MPEG2_DISABLE) || !defined(CC_ES_MPEG1_DISABLE)
#define SWDMX_MPG_MPEG2_SUPPORT
#endif
#if !defined(CC_ES_MPEG4_DISABLE)
#define SWDMX_MPG_MPEG4_SUPPORT
#endif
#if !defined(CC_ES_AVS_DISABLE)
#define SWDMX_MPG_AVS_SUPPORT
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
#define STREAM_TYPE_MVC         0x20
#define STREAM_TYPE_MULTITRACK  0x101
#define STREAM_TYPE_AVS         0x42
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
#define STREAM_TYPE_SWDMX_LPCM  0x100

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
#define MPG_FEEDER_ALIGN (1024) // current is 512
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
#define SWDMX_NORMAL_SPEED (1000)
#ifdef SWDMX_MPG_CHECK_MP3_AUDIO
#define MPG_MP3_CHECK_CNT 3
#endif

#ifdef CC_TIME_SHIFT_NO_TICK_DETAIL
#define SWDMX_MPG_USE_MTMPLAYER_PVR
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


// definition move from swdmx_psipsr_if.h
#define SWDMX_MAX_PMT_NUM                       (96)
// definition move from swdmx_dmx.c
#define NUM_FILTER                    32     // Number of filter supported
#define NUM_PID                         32       // Number of PID supported
#define NUM_MESSAGE                32      // Number of message slots

// RM multi-instance
#define RM_MAX_AUDIO_TRACK 1
#define RM_SLICE_SZ_BUF_SA   ((UINT8 *)prSwdmxInst->rFmtInfo.rSwdmxRmInfo.au4SliceSzBuf)
#define RM_SLICE_SZ_BUF_NS   5000

#ifdef RM_LOGICAL_STREAM_SUPPORT
#define RMLS
#endif

// definition moved from swdmx_cmpb.h
#define CMPB_MAX_AUDIO_TRACK    8

#define SWDMX_BUFFER_REFINE
#define MKV_CONTENT_ENCODING_SUPPORT // We only support header stripping now!!!

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    SWDMX_SRC_TYPE_UNKNOWN = 0,
    SWDMX_SRC_TYPE_HIGH_SPEED_STORAGE,
    SWDMX_SRC_TYPE_NETWORK_DLNA,
    SWDMX_SRC_TYPE_NETWORK_NETFLIX,
    SWDMX_SRC_TYPE_NETWORK_VUDU,
    SWDMX_SRC_TYPE_NETWORK_RHAPSODY,
    SWDMX_SRC_TYPE_PUSH,
    SWDMX_SRC_TYPE_NETWORK_CMPB,
    SWDMX_SRC_TYPE_NETWORK_XUNLEI,
    SWDMX_SRC_TYPE_NETWORK_MHP,
    SWDMX_SRC_TYPE_NETWORK_SKYPE,
    SWDMX_SRC_TYPE_MAX
    // this definition is exactly same with MM_SRC_TYPE_T.
    // please do not change it's sequence
}SWDMX_SOURCE_TYPE_T;

typedef enum
{
    SWDMX_BUFFER_STATE_NORMAL = 0,
    SWDMX_BUFFER_STATE_READY = 1,
    SWDMX_BUFFER_STATE_UNDER_FLOW=2
}SWDMX_BUFFER_STATE_T;


typedef enum SWDMX_DELIVER_MEDIA_TYPE
{
    eSWDMX_MEDIA_VIDEO,            // video
    eSWDMX_MEDIA_AUDIO,              // audio
    eSWDMX_MEDIA_DRM_VIDEO,           // digital right management for video
    eSWDMX_MEDIA_DRM_AUDIO,           // digital right management for audio
    eSWDMX_MEDIA_SUBTITLE,             // subtitle
    eSWDMX_MEDIA_SKIPPED_AUDIO,
    eSWDMX_MEDIA_SKIPPED,
    eSWDMX_MEDIA_NONE
} SWDMX_DELIVER_MEDIA_TYPE_T;

typedef enum _SWDMX_RANGE_STATE
{
    eSWDMX_RANGE_STATE_NOT_FINISHED,    // the range is not finished
    eSWDMX_RANGE_STATE_FINISHED,             // the range is finished
    eSWDMX_RANGE_STATE_INVALID,            // the range is invalid
    eSWDMX_RANGE_STATE_SKIP,                    // the range is skipped
    eSWDMX_RANGE_STATE_REQUEST,                 // request for a new range
    eSWDMX_RANGE_STATE_PENDING,               // the range is finished but need to wait
    eSWDMX_RANGE_STATE_NO_INDEX
} SWDMX_RANGE_STATE_T;

typedef enum _SWDMX_HDR_PRS_STATE
{
    eSWDMX_HDR_FED_BUF_NOT_ENOUGH,            // the buffer is not enough
    eSWDMX_HDR_DMX_BUF_NOT_ENOUGH,               // the buffer is not enough
    eSWDMX_HDR_PRS_FED_MOVE_FAIL,            // the feeder fails to move date from the IO device
    eSWDMX_HDR_PRS_DMX_MOVE_FAIL,            // the demuxer fails to move data from the feeder
    eSWDMX_HDR_PRS_SUCCEED,                     // header parsing succeed
    eSWDMX_HDR_PRS_FAIL,                         // header parsing fail
    eSWDMX_HDR_PRS_EMPTY_RANGE,
    eSWDMX_HDR_UNKNOWN_TYPE                    // header parsing fail
} SWDMX_HDR_PRS_STATE_T;

typedef enum _SWDMX_OGG_ACT_STATE
{
    eSWDMX_OGG_TIME_SEEK = 1,
    eSWDMX_OGG_SEEK_LEN,
    eSWDMX_OGG_FIND_HEADER,
    eSWDMX_OGG_FIND_KEY
}SWDMX_OGG_ACT_STATE;

typedef enum
{
    SWDMX_PVR_SENDDATA_OK,
    SWDMX_PVR_SENDDATA_FULL,
    SWDMX_PVR_SENDDATA_FAIL,
    SWDMX_PVR_SENDDATA_NOTENABLED,
    SWDMX_PVR_SENDDATA_STOP
} SWDMX_PVR_SENDDATA_RET_T;

typedef struct SWDMX_RANGE_LIST
{
    BOOL fgIsFull;
    BOOL fgInUse;
    BOOL fgIsSeekTarget;

    UINT32 u4CurSentVidIdx;
    UINT32 u4CurSentAudIdx;

    UINT32 u4CurRangeVidIdx;
    UINT32 u4CurRangeAudIdx;
    UINT32 u4CurRangeSubIdx;

    UINT32 u4CurPTS;
    UINT32 u4RangeId;

    UINT32 u4ReqPTS;

    SWDMX_RANGE_STATE_T     eRangeStats;
    MM_RANGE_INFO_T     *prDmxRangeInfo;

#ifdef CC_SWDMX_RENDER_POSITION
    BOOL fgSetRenderPosition;
    INT32 i4TempRef;
    UINT16 u2DecodingOrder;
    UINT32 u4Timestap;
    UINT64 u8Offset;
    UINT64 u8OffsetI;
    UINT64 u8StartPts;
    UINT64 u8StartPtsI;
    UINT64 u8StartAudPts;
    UINT64 u8StartAudOffset;
#endif

    VOID    *pvNext;
} SWDMX_RANGE_LIST_T;

#define SWDMX_CUR_CMD_SEEK_ORIGINAL  0
#define SWDMX_CUR_CMD_SEEK_OFFSET    1
typedef struct SWDMX_CUR_CMD
{
    BOOL fgChange;      // change include speed change or position change

    SWDMX_PLAY_MODE_T ePlayMode;
    INT32 i4PlaySpeed;
    UINT64 u8SeekPos;

    // new method
    BOOL fgSeek;        // is doing seek
    UINT32 u4SeekMode;  // 0: Original method, 1: seek offset
    START_POS_INFO_T rSeekOffset;
} SWDMX_CUR_CMD_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define SWDMX_BYTE3(arg)        (*((UINT8 *)&(arg) + 3))
#define SWDMX_BYTE2(arg)        (*((UINT8 *)&(arg) + 2))
#define SWDMX_BYTE1(arg)        (*((UINT8 *)&(arg) + 1))
#define SWDMX_BYTE0(arg)        (* (UINT8 *)&(arg))

#define SWDMX_LOAD_BYTE(arg1, arg2)  SWDMX_BYTE0(arg2) = *((UINT8 *)(arg1));

/* big endian */
#define SWDMX_LOADB_WORD(arg1, arg2)  SWDMX_BYTE1(arg2) = *((UINT8 *)(arg1)); \
                                      SWDMX_BYTE0(arg2) = *((UINT8 *)(arg1) + 1)

/* little endian */
#define SWDMX_LOADL_WORD(arg1, arg2)  SWDMX_BYTE1(arg2) = *((UINT8 *)(arg1) + 1); \
                                      SWDMX_BYTE0(arg2) = *((UINT8 *)(arg1))

/* big endian */
#define SWDMX_LOADB_DWRD(p, arg2)     SWDMX_BYTE3(arg2) = *((UINT8 *)(p)); \
                                      SWDMX_BYTE2(arg2) = *((UINT8 *)(p) + 1); \
                                      SWDMX_BYTE1(arg2) = *((UINT8 *)(p) + 2); \
                                      SWDMX_BYTE0(arg2) = *((UINT8 *)(p) + 3)

/* little endian */
#define SWDMX_LOADL_DWRD(p, arg2)     SWDMX_BYTE3(arg2) = *((UINT8 *)(p) + 3); \
                                      SWDMX_BYTE2(arg2) = *((UINT8 *)(p) + 2); \
                                      SWDMX_BYTE1(arg2) = *((UINT8 *)(p) + 1); \
                                      SWDMX_BYTE0(arg2) = *((UINT8 *)(p))

#define SWDMX_LOADL_QWRD(p, arg2) ( (arg2) = ((UINT64)(p)[7] << 56)   \
                                        | ((UINT64)(p)[6] << 48)   \
                                        | ((UINT64)(p)[5] << 40)   \
                                        | ((UINT64)(p)[4] << 32)   \
                                        | ((UINT64)(p)[3] << 24)   \
                                        | ((UINT64)(p)[2] << 16)   \
                                        | ((UINT64)(p)[1] << 8)    \
                                        | (UINT64)(p)[0] )


/**
*/
typedef BOOL (*PFN_SWDMX_INIT)(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType);

/**
*/
typedef BOOL (*PFN_SWDMX_GET_STREAM_INFO)(
    UINT8 u1SrcId,
    ENUM_SWDMX_FMT_T eType,
    UINT32 u4InfoMask,
    UINT64 u8FileSize,
    SWDMX_PROGRAM_INFO_T* pPInfo);

/**
*/
typedef BOOL (*PFN_SWDMX_PLAY)(UINT8 u1SrcId);

/**
*/
typedef BOOL (*PFN_SWDMX_PAUSE)(UINT8 u1SrcId);

/**
*/
typedef BOOL (*PFN_SWDMX_STOP_CMD)(VOID);


/**
 To interupt DDI Full-Pull mode.
*/
typedef BOOL (*PFN_SWDMX_ABORT)(UINT8 u1SrcId);

/**
*/
typedef VOID (*PFN_SWDMX_SET_RANGE)(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prDmxRangeInfo,
    UINT32 u4VidStrmID,
    UINT32 u4AudStrmID,
    UINT32 u4SubTitleID);


/**
*/
typedef SWDMX_HDR_PRS_STATE_T (*PFN_SWDMX_HEADER_PARSE)(
    UINT8 u1SrcId,
    SWDMX_RANGE_LIST_T* prRangeList,
    SWDMX_CUR_CMD_T* prCmd);

/**
*/
typedef BOOL (*PFN_SWDMX_STOP)(UINT8 u1SrcId);

/**
*/
typedef BOOL (*PFN_SWDMX_FLUSH)(UINT8 u1SrcId);

typedef BOOL (*PFN_SWDMX_SET_INFO)(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32 u4Para1,
    UINT32 u4Para2,
    UINT32 u4Para3);

typedef BOOL (*PFN_SWDMX_GET_INFO)(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);

typedef BOOL (*PFN_SWDMX_SET_STREAM_ID)(
    UINT8 u1SrcId,
    UINT8 u1Type,
    UINT32 u4StrmID,
    UINT32 u4StrmSubID);

typedef BOOL (*PFN_SWDMX_ENABLE_STREAM)(
    UINT8 u1SrcId,
    UINT8 u1StrmType,
    VOID* pfnCbFunc);

typedef BOOL (*PFN_SWDMX_DISABLE_STREAM)(
    UINT8 u1SrcId,
    UINT8 u1StrmType);

typedef BOOL (*PFN_SWDMX_READ_BUFFER)(UINT8 u1SrcId,
    ENUM_SWDMX_STRM_TYPE_T eStrmType,
    SWDMX_READ_INFO_T *prReadInfo);

typedef BOOL (*PFN_SWDMX_AUDIO_CB)(
    UINT8 u1SrcId,
    ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

/// Callback handler of sending video PES. This handler will be called
/// whenever demux driver detects a picture start code in video ES FIFO.
typedef BOOL (*PFN_SWDMX_SEND_VIDEO_PES)(const DMX_PES_MSG_T* prPes);

/// Callback handler of sending audio PES. This handler will be called
/// whenever demux driver receives an audio frame.
typedef BOOL (*PFN_SWDMX_SEND_AUDIO_PES)(const DMX_AUDIO_PES_T* prPes);

// move from swdmx_mpg_dmx.h
//typedef VOID (*PFN_SWDMX_CALLBACK)(INT32 i4Tag);
typedef VOID (*PFN_SWDMX_CALLBACK)(
    UINT8 u1SrcId,
    INT32 i4Tag);


typedef struct {
    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnSubTitle;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;
    BOOL fgEnSubDmx;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBeginToPlay;

    BOOL fgChgATrack;
    BOOL fgChgSbTrack;

    BOOL fgKeyExist;

    BOOL fgStartTrick;

    BOOL fgAudIsReady;
    BOOL fgVDecIsPause;
    BOOL fgIsFirstFrame;
    BOOL fgMp3Abr;
    BOOL fgMp3Vbr;
    BOOL fgGetMp3Status;
    BOOL fgSendVolInfo;
    BOOL fgNoCalPts;
    BOOL fgSetActivePid;
    BOOL fgVdecNotSupport;
    // Feeder
    BOOL fgFeederInvalid;
    BOOL fgFeederIsReady;
    BOOL fgRequestData;
    INT32 i4ReqReadTimes;
    UINT32 u4FeederReqID;
    UINT32 u4FeederReqReceiveID;
    UINT32 u4RecFeederWPtr;
    UINT32 u4RecFeederSize;
    UINT32 u4Mp3Rate;
    UINT32 u4Mp3LastRate;
    UINT32 u4AudFrameSize;
    UINT32 u4AudNotParseSize;
    FEEDER_BUF_INFO_T rFeederInfo;

    UINT64 u8CurMoviPos;
    UINT64 u8CurDmxMoviPos;
    UINT64 u8RecDmxMoviPos;
    UINT32 u4LeftBytesInFeeder;

    UINT32 u4VDecType;
    UINT32 u4ADecType;

    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4SpStrmID;
    UINT32 u4PrevAStrmID;
    UINT32 u4PrevSpStrmID;

    UINT32 u4CurDmxFeederRPtr;

    UINT32 u4CurVidPts;
    //UINT32 u4ModeChangPts;

    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;
    UINT32 u4NumSentSub;

    UINT16 u2SkipVidCount;

    UINT32 u4ChangeMode;

    UINT32 u4CurSentKey;

    BOOL fgIsBlock;
    UINT32 u4BlockChunkSize;


    BOOL fgSpeedChange;
    INT32 i4PrevSpeed;

    UINT64 u8VRemainder;
    UINT64 u8ARemainder[AVI_DIVX_MAX_AUD_TRACK];

    //Video
    UINT8 u1VidPid;
    UINT8 u1SubPid;
    UINT32 u4VidFps;
    UINT64 u8VidOffset;
    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;

    UCHAR *pucAviVidHeaderPtr;

    //Audio
    UINT8 u1AudPid;
    UINT8 u1AudPreBufPid[AVI_DIVX_MAX_AUD_TRACK];
    BOOL fgBadAudio;
    BOOL fgAudIsLoaded;
    BOOL fgRecSpeedChangeForAud;
    BOOL fgAudStrmDisable;
    UINT8 u1CurATrackIdx;
    UINT8 u1TotalATrack;
    INT32 i4RecPrevSpeedForAud;
    UINT32 u4AudSampleRate;
    UINT32 u4AudChannelNs;
    UINT32 u4AudPts[AVI_DIVX_MAX_AUD_TRACK];
    UINT32 u4TriggerAudPts;
    UINT32 u4AudStartPts;
    UINT32 u4AFifoSize;
    BOOL fgQAud;
    UINT32 u4QAudAddr;
    UINT32 u4QAudSize;
    UINT32 u4QAudPts;
    UINT32 u4QAudTrack;
    UINT32 u4TotalTime;
    UINT32 u4LastIPts;
    UINT64 u8LastIPos;
    //Drm
    UINT8 u1DrmType;
    UINT16 u2FrameKeyIdx;
    UINT32 u4EncryptOffset;
    UINT32 u4EncryptLength;
    UINT32 u4VidHeaderPtr;
    UINT32 u4VidHdrLength;
 // Subtitle
    UINT32 u4RecSubPts;
    UINT64 u8PrevSubPos;
    UINT32 u4RecVidPtsForSub;
    UINT32 u4RecAudPtsForSub[AVI_DIVX_MAX_AUD_TRACK];
    UINT32 u4ErrorData;

    //mutil riff, if the first riff has idx1 data
    BOOL fgIsSkipIdxData;

    //Seek
    BOOL fgPreviouIsSeek;
    UINT32 u4RecSeekTime;
    BOOL fgIsSeek;
    BOOL fgIsNonSeekable;
    UINT64 u8ASeekPts;
    UINT64 u8VSeekPts;
    UINT64 u8AbRepeatVidPts;
    UINT64 u8AbRepeatAudPts;
    BOOL fgPauseToSeek;
    UINT64 u8PauseToSeekPos;
    // DLNA
    BOOL fgReqDataMiss;
    BOOL fgReqSubDataMiss;
    // Range Information
    MM_RANGE_INFO_T *prDmxRangeInfo;

    UINT32 u4LastAlignOffset;

    BOOL fgAbRepeat;
    UINT32 u4AbChunkSize;
    UINT32 u4AbDmxPos;
    UINT32 u4AbPts;
    BOOL fgAbOneMore;
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prCurVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *parAudKeyIdxTbl[AVI_DIVX_MAX_AUD_TRACK];
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl[AVI_DIVX_MAX_AUD_TRACK];
    //MM_RANGE_ELMT_IDXTBL_T *prSubTitleIdxTbl;
    //add for 3d media
    BOOL fgEnV2Dmx;  //for video 2
    BOOL fgIsLeftVideo;
    BOOL fgIsLeftViewDone;
    BOOL fgIsSupport3DTrick;
    BOOL fgSubVideoRequestData;
    UINT32 u4RecSubVFeederWPtr;
    UINT32 u4RecSubVFeederSize;
    UINT32 u4VSubStrmID;   //for video 2
    UINT32 u4CurSubVidPts;
    UINT32 u4CurSentSubKey;
    UINT64 u8VRRemainder;
    UINT8 u1Vid2Pid;    //for video 2
    UINT64 u8SubVidOffset;
    UINT32 u4CurRepeatSubFrameIdx;
    UINT32 u4PreRepeatFrameIdx;
    UINT32 u4PreRepeatSubFrameIdx;
    UINT32 u4SubVidRepeatFrame;
    MM_RANGE_ELMT_IDXTBL_T *prCurSubVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prSubVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prSubVidIdxTbl;
} SWDMX_AVI_INFO_T;

typedef struct {

    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnSubTitle;

    BOOL fgBeginToPlay;
    BOOL fgFeederInvalid;
    BOOL fgInvalidForSub;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBlockVid;
    BOOL fgBlockAud;

    BOOL fgAudIsReady;

    BOOL fgPartialRangeDone;
    BOOL fgFeederLeftNotEnough;

    BOOL fgVDecIsPause;

    BOOL fgChgATrack;
    BOOL fgChgSubTrack;
    BOOL fgAudStrmDisable;

    BOOL fgRequestData;

    BOOL fgWaitNewRange;

    BOOL fgSeekAfterPause;

    UINT32 ui4_data_offset;
    UINT64 u8_range_sz;

    UINT32 u4VDecType;
    UINT32 u4ADecType;

    UINT32 u4VidHeaderPtr;
    UINT32 u4VidHdrLength;

    UINT32 u4VidHeaderSecPtr;
    UINT32 u4VidHdrSecLength;
    UINT8 u1CurATrack;
    UINT8 u1CurSubTrack;
    UINT8 u1TotalATrack;
    UINT8 u1TotalSubTrack;

    UINT8 u1VidPid;
    UINT8 u1AudPid;
    UINT8 u1SubPid;

    UINT32 u4AudHeaderPtr;
    UINT32 u4AudHdrLength;

    UINT32 u4AudSampleRate;
    UINT32 u4AudChannelNs;

    UINT32 u4PrevAStrmID;
    UINT32 u4PrevSubStrmID;

    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4SubTitleID;

    UINT64 u8VidLatestOff;
    UINT64 u8AudLatestOff;
    UINT64 u8SubLatestOff;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;
    BOOL fgEnSubDmx;

    UINT32 u4VidFps;

    BOOL fgRepeatRequest;

    BOOL fgStartTrick;

    // For feeder usage
    UINT32 u4FeederReqID;
    UINT32 u4LeftBytesInFeeder;
    UINT32  u1StartCodeLenMinus1;
    UINT64 u8CurFilepos;
    UINT32 u4RecFeederWPtr;

    // Record current demuxing position.
    UINT32 u4CurDmxFeederRPtr;
    INT32 i4ReqReadTimes;
    INT32 i4FeederInvalidCount;

    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *parVidKeyIdxTblArray[MP4_MAX_KEY_TABLE];

    MM_RANGE_ELMT_IDXTBL_T *prSubIdxTbl[MP4_MAX_SUB_TRACK];
    MM_RANGE_ELMT_IDXTBL_T * prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T * prAudIdxTbl[MP4_MAX_AUDIO_TRACK];

    BOOL fgIsSeek;
    BOOL fgRecSeekFlag;

    UINT64 u8SeekPts;
    UINT64 u8VSeekPts;
    UINT64 u8ASeekPts;
    UINT32 u4CurSentKey;
    UINT32 u4LastSentKey;
    BOOL fgHasAud;

    UINT32 u4NumSentVid;
    //UINT32 u4NumSentAud;

    BOOL fgSpeedChange;
    INT32 i4PrevSpeed;

    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;

    UINT32 u4LastVidPts;
    UINT32 u4LastIPts;
    UINT32 u4TotalTime;
    UINT32 u4TriggerAudPts;

    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;

    UINT32 u4AudBufSize;

    MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T rPrevSample;
    UINT64 u8PrevSampleOffset;

    //for over 4G mp4
    UINT32 u4NumOfKeyTable;
    UINT32 u4CurKeyTableIdx;

    MM_RANGE_INFO_T     *prDmxRangeInfo;
} SWDMX_MP4_INFO_T;

typedef struct {

    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnVDmx;
    BOOL fgEnADmx;
    BOOL fgBeginToPlay;
    BOOL fgFeederInvalid;
    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;
    BOOL fgVDecIsPause;
    BOOL fgChgATrack;
    BOOL fgSpeedChange;
    BOOL fgIsSeek;
    BOOL fgFindKey;
    BOOL fgRepeatRequest;
    BOOL fgAudIsReady;
    BOOL fgKeyFrame;
    BOOL fgSendKeyDone;
    BOOL fgSetActivePid;
    BOOL fgIsFirstFrame;
    BOOL fgSeekError;
    BOOL fgPlayAbort;
    BOOL fgFirstFrame;
    BOOL fgSendOver;
    BOOL fgSeekDone;
    BOOL fgParseHeadPacket;          //ogg vorbise page
    UINT8 u1CurATrackIdx;
    UINT8 u1TotalATrack;
    UINT8 u1VidPid;
    UINT8 u1AudPid;
    UINT8 u1AudPreBufPid[OGG_MAX_AUDIO_TRACK];
    UINT8 u1StreamType;
    UINT8 u1PageSegment;

    INT32 i4PrevSpeed;
    INT32 i4ReqReadTimes;
    INT32 u4RepeatCount;

    UINT32 u4PageCount;
    UINT32 u4TotalTime;

    UINT32 u4VDecType;
    UINT32 u4ADecType;
    UINT32 u4AudSampleRate;
    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4PageSeqNum;
    UINT32 u4CurSentKey;
    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;
    UINT32 u4TriggerAudPts;
    UINT32 u4ChangeAudPts;
    UINT32 u4PrevAStrmID;
    UINT32 u4LastVidPts;
    UINT32 u4LastAudPts;

    UINT32 u4LastIPts;
    UINT32 u4AudChannelNs;
    UINT32 u4VSeekPts;
    UINT32 u4ASeekPts;
    UINT32 u4AudHeaderPtr[OGG_MAX_AUDIO_TRACK];
    UINT32 u4AudHdrLength[OGG_MAX_AUDIO_TRACK];

    //For ogg Vorbis header packet offset
    MM_AUDIO_VORBIS_OFFSET_T u4AudVorbisOffset[OGG_MAX_AUDIO_TRACK];
    // For feeder usage
    UINT32 u4FeederReqID;
    UINT32 u4LeftBytesInFeeder;
    UINT32 u4FeederSize;
    UINT32 u4CurDmxFeederRPtr;
    UINT32 u4AlignOffset;


    UINT64 ui8_data_offset;
    UINT64 u8LastIPos;
    UINT64 u8VidLatestOff;
    UINT64 u8AudLatestOff;
    UINT64 u8_range_sz;
    UINT64 u8SeekPts;
    UINT64 u8CurMoviPos;
    UINT64 u8FileOffset;
    UINT64 u8CurFilePos;
    UINT64 u8FilePosBeforeSeek;
    UINT64 u8KeyPos;
    UINT64 u8CurDmxMoviPos;
    UINT64 u8PreDmxPos;
    UINT64 u8AudGranulePos[OGG_MAX_AUDIO_TRACK];
    UINT64 u8AudPreGranulePos;
    UINT64 u8VidGranulePos;
    UINT64 u8VidPreGranulePos;

    SWDMX_OGG_ACT_STATE eActStatus;
    MM_RANGE_ELMT_IDXTBL_T * prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T * prAudIdxTbl[OGG_MAX_AUDIO_TRACK];

    MM_RANGE_INFO_T     *prDmxRangeInfo;
} SWDMX_OGG_INFO_T;

//netflix stream type
typedef struct
{
    UINT16 u2PacketSize;
    UINT8  u1PacketType;
    BOOL   fgEos;
} NetflixEmbeddedCmd_T;


typedef struct
{
    UINT32 u4FilePos;
    UINT8 u1PayloadLengthType;
    UINT8 u1PayloadsNumber;
    UINT8 u1CurPayloadIdx;
} MultiPayloadInfo;
typedef struct
{
     BOOL fgEnVideo;
     BOOL fgEnAudio;
     BOOL fgEnVDmx;
     BOOL fgEnADmx;
     BOOL fgDeliverVEos;
     BOOL fgDeliverAEos;
     BOOL fgBeginToPlay;
     BOOL fgKeyExist;
     BOOL fgTrickMode;
     BOOL fgTrickRcv;
     BOOL fgAudIsReady;
     BOOL fgVDecIsPause;
     BOOL fgFeederInvalid;
     BOOL fgFeederIsReady;
     BOOL fgTagHeaderParsed;
     BOOL fgNewRp;
     BOOL fgTagHeaderBroken;
     BOOL fgTagDataBroken;
     BOOL fgAacCfgBroken;
     BOOL fgBadAudio;
     BOOL fgAudIsLoaded;
     BOOL fgPreviouIsSeek;
     BOOL fgH264SeqHeaderParsed;
     BOOL fgAacSendHeader;
     BOOL fgGetAacHeader;
     BOOL fgStartTrick;
     BOOL fgNeedSetAudStartPts;
     BOOL fgErrorConvery;
     BOOL fgRecoveredFromErr;
     BOOL fgPlayAbort;
     UINT8 uAudChannelNs;
     UINT8 uAvcNalLenBytes;
     UINT8 uLastNalType;
     UINT8 uCurNalType;
     UINT8 u1SrcId;
     UINT8 u1EsId;
     UINT8 u1VidPid;
     UINT8 u1AudPid;
     // Feeder

     INT32 i4ReqReadTimes;
     UINT32 u4LeftBytesInFeeder;
     UINT32 u4CurDmxFeederRPtr;
     UINT32 u4FeederReqID;
     UINT32 u4FeederReqReceiveID;
     UINT32 u4FrameAddrToVdec;
     UINT32 u4ValidSeekPos;
     UINT32 u4LastKeyPos;
     //flvparse
     UINT8 uTagHeaderBrokenIndex;
     UINT8 uLastTagType;
     UINT32 u4LastPts;
     UINT32 u4AvcPtsOffset;
     UINT32 u4LastNalPtr;
     UINT32 u4LastNalPts;
     UINT32 u4NalLen;
     UINT32 u4NalSkipLen;
     UINT32 u4AvcHeaderParseInfo; //byte0:tag info,byte1: Nals Type, byte2:pts offset,byte3:nal len
     UINT32 u4SkipDatLen;
     UINT32 u4LastDataLen;
     UINT32 u4LastDataBrokenNum;
     UINT64 u8CurMoviPos;
     UINT64 u8CurDmxMoviPos;
     INT32  i4DebugerCounter;
     //AV
     UINT32 u4VDecType;
     UINT32 u4ADecType;
     UINT32 u4CurVidPts;
     INT32 i4PrevSpeed;

     //Video
     UINT32 u4VidFps;
     UINT32 u4VStrmID;
     UINT32 u4VidHdrPtr;
     UINT32 u4VidHdrLen;
     UINT32 u4LastKeyPts;
     //Audio

     UINT32 u4AStrmID;
     UINT32 u4AudSampleRate;
     UINT32 u4AudPts;
     UINT32 u4TriggerAudPts;
     UINT32 u4AudStartPts;
     UINT32 u4AFifoSize;
     UCHAR* pucAacCfg;
     UCHAR  aucHeaderBuf[15];

     //Seek
     UINT32 u4RecSeekTime;
     UINT32 u4RecSeekAtime;
     UINT32 u4VidRepeatFrame;
     UINT32 u4CurSentKey;
     UINT32 u4SeekTagLen;
     UINT32 u4CurRepeatFrameIdx;

     //Error Recovery
     UINT16 u2TagIdentify;
     UINT16 u2FindCounter;
     UINT32 u4ErrorLen;
     UCHAR  *PRecoverPos;
     UINT32 u4SwdmxInst;
     // Range Information
     FEEDER_BUF_INFO_T *prFeederInfo;
     SWDMX_RANGE_LIST_T *prRangeList;
     MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
}SWDMX_FLV_INFO_T;

typedef struct
{
    UINT8 u1StreamNum;
    BOOL  fgKeyFrame;
    UINT32 u4MediaObjNum;
    UINT32 u4OffsetIntoMediaObj;
    UINT32 u4ReplicatedDataLen;
    UINT32 u4PTS;
    UINT32 u4PTSDelta;
    UINT32 u4MediaObjSize;
} PayloadHeader;

typedef struct
{
    //Length Type Flags
    BOOL  fgMultiplePayload;
    UINT8 u1SequenceType;
    UINT8 u1PaddingLenghtType;
    UINT8 u1PacketLengthType;
    BOOL  fgErrorCorrectPresent;
    //Property Flags
    UINT8 u1ReplicatedDataType;
    UINT8 u1OffsetIntoType;
    UINT8 u1MediaObjectNumType;
    UINT8 u1StreamNumberType;
    UINT32 u4PacketLength;
    UINT32 u4Sequence;
    UINT32 u4PaddingLength;
    UINT32 u4SendTime;
    UINT16 u2Duration;
} PayloadParseInfo;

typedef struct
{
    FEEDER_REQ_DATA_T rFeederReqData;
    UINT32 u4RequestFeederSize;
} WMV_FEEDER_REQ_DATA_T;

typedef struct
{
    BOOL fgSeqHdr;
    BOOL fgGop;
    BOOL fgDtsValid;      //DTS is valid
    BOOL fgNoVdecCalPts;
    UCHAR ucMpvId;
    UCHAR ucEsId;
    UCHAR ucPicType;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4VldReadPtr;
    UINT32 u4PTS;
    UINT32 u4DTS;
    UINT32 u4VideoType;
    BOOL fgEos;
    BOOL fgSeqEnd;
    BOOL fgNonBuffer;
    UINT8 u1SliceCount;
    UINT32 u4RenderPts;
    UINT32 u4TotalTimeOffset;
    UINT32 u4TickNum;
    UINT64 u8Offset;
    UINT64 u8OffsetI;
    UINT16 u2DecodingOrder;
    UINT32 u4DmxFrameType;
    UINT16 u2SkipVidCount;
    #ifdef CC_DMX_PES_EXT_BUFFER
    UINT32 u4PesExtBufStart;
    UINT32 u4PesExtBufEnd;
    UINT32 u4PesExtBufCurrentRp;
    UINT32 u4PesExtBufCurrentWp;
    #endif
    //BOOL fgSkipFrame;  // only enable this for WMV emulation!
} VDEC_PES_INFO_T_DUP;   // megaa 20100326: tmp solution

typedef enum
{
    BAD_INTLV_STT_NONE,
    BAD_INTLV_STT_TO_PATCH_A,
    BAD_INTLV_STT_PATCH_A,
    BAD_INTLV_STT_TO_REPRS_V,
    BAD_INTLV_STT_REPARSE_V,
    BAD_INTLV_STT_TO_PATCH_V,
    BAD_INTLV_STT_PATCH_V,
    BAD_INTLV_STT_TO_REPRS_A,
    BAD_INTLV_STT_REPARSE_A
} WMV_BAD_INTLV_STT_T;

typedef struct
{
    BOOL fgEverEntered;
    WMV_BAD_INTLV_STT_T eState;
    BOOL fgRecord;

    BOOL fgAudioEnd;
    BOOL fgVideoEnd;

    UINT32 u4BeginSkipPktNo;     // packet no that begins skipping A/V packet
    UINT32 u4BeginSkipPldNo;
    UINT64 u8BeginSkipPktOfst;

    UINT32 u4EndPatchPktNo;      // packet no that ends patching A/V packet
    UINT32 u4EndPatchPldNo;
} WMV_BAD_INTLV_STATUS_T;

typedef struct
{
    BOOL fgInternalInited;

    ENUM_SWDMX_VDEC_T eVideoType;
    ENUM_SWDMX_ADEC_T eAudioType;

    UINT64 u8FileSize;
    UINT64 u8PacketMarker;
    UINT64 u8LastSeekPos;
    UINT32 u4PacketMarkerLen;
    UINT32 u4WMASeekTimes;
    UINT32 u4SampleRate;
    UINT32 u4SequenceHeader;
    UINT32 u4SequenceHeaderLength;
    UINT32 u4DataObjSize;
    UINT32 u4PacketSize;
    UINT32 u4PreRoll;

    UINT32 u4FeederReqID;
    UINT32 u4PreviousPos;
    UINT32 u4PreviousIdx;
    UINT32 u4CurParsingPacketNo;
    UINT32 u4TotalPacketNo;
    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;
    UINT16 u2SendPayloadNo;
    UINT16 u2PreviousSendPayloadNo;
    UINT32 u4WmaErrorCode;

    UINT32 u4VStreamId;
    UINT32 u4VSubStreamId;
    UINT32 u4AStreamId;
    UINT32 u4SpStreamId;

    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnSubTitle;
    BOOL fgFirstFrame;
    BOOL fgBlock;
    BOOL fgReSend;
    BOOL fgEnADmx;
    BOOL fgEnVDmx;
    BOOL fgEnV2Dmx;
    BOOL fgSeekingNow;
    BOOL fgPlayAbort;
    UINT8 u1VidPid;
    UINT8 u1Vid2Pid;
    UINT8 u1AudPid;
    UINT8 u1AudPreBufPid[WMV_MAX_AUDIO_TRACK];
    BOOL fgFirstAudioError;
    BOOL fgFeederInvalid;
    UINT32 u4PacketStartAddr;
    UINT32 u4CurParsingBytes;
    UINT32 u4CurPTS;
    UINT32 u4FeederReadSize;
    BOOL fgDemuxFinish;
    //for DRM
    BOOL fgIsDRM;
    //for trick mode
    BOOL fgReqFeederDataEOS;
    BOOL fgFirstReqFeederData;
    UINT32 u4PrePacketNo;
    BOOL fgFileWithStartCode;
    BOOL fgFlushFeederQueue;
    UINT64 u8LastFileParsingPosition;
    UINT32 u4ConsumedSize;
    BOOL fgVideoPause;
    UINT32 u4TriggerAudioPTS;
    BOOL fgTrickReceivePreviousPTS;
    BOOL fgChangeAudio;
    BOOL fgBrokenFile;
    BOOL fgGetKeyHead;
    BOOL fgGetKeyTail;
    //UINT32 u4TargetVideoPTS; //record the pts when changing audio channel
    //UINT32 u4LastVideoPTS;
    UINT32 u4RepeatTimes;
    UINT32 u4CurPacketPTS;
    BOOL fgFinishDemuxIFrame;
    UINT32 u4VideoCount;
    UINT32 u4AudioCount;
    UINT32 u4FirstVideoPTS;
    BOOL fgBeginPlay;
    UINT32 u4FrameRate;
    UINT8 u1FWKeyFrameNo;
    UINT64 u8SeekPTS;
    UINT32 u4DisplayPTS;
    UINT32 u4FrameOffset;
    UINT32 u4IFrameOffset;
    UINT32 u4CurKeyIdx;
    BOOL fgFinishKeyTable;
    INT32 i4WMVPlaySpeed;
    BOOL fgWMVSpeedChange;
    UINT64 u8WMVFileOffset;
    BOOL fgAudioNormalPlay;
    BOOL fgVideoNormalPlay;
    UINT32 u4LastVideoWptr;
    MM_RANGE_ELMT_IDXTBL_T * prWmvKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T * prWmvKeyIdxTbl2;

    BOOL fgAlreadySet;
    BOOL fg2XAudParsing;

    // below are original local static variables
    SWDMX_RANGE_LIST_T *prRangeList;
    MM_RANGE_INFO_T* prRangeInfo;

    FEEDER_TRANSMIT_COND rFeederCond;
    FEEDER_REQ_DATA_T rFeederReqData;

    UINT8 u1AudioStreamId;
    FEEDER_BUF_INFO_T rFeederInfo;

    VDEC_PES_INFO_T_DUP rPreviousPesInfo;
    DMX_AUDIO_PES_T rPreviousAudioPes;
    BOOL fgReceiveAudioPes;
    BOOL fgReceiveVideoPes;
    //UINT32 u4DataWithStartCode[10*1024];
    //UINT8 *pu1VideoFrameData;  // moved to common part
    UINT32 u4PrevFramePTS;
    UINT32 u4DefalutReqSize;

    //for netflix application begin
    MINFO_TYPE_GENERIC_T* prWmvGenericInfo;
    UINT32 u4NetflixVideoStreamId;
    UINT32 u4NetflixAudioStreamId;
    BOOL fgNetflixEos;
    BOOL fgDataExhausted;
    ENUM_SWDMX_FMT_T eCurFMT;
    UINT32 u4NumberOfBitstream;
    //for netflix application end

    PayloadParseInfo rPayLoadInfo;
    MultiPayloadInfo rMultiPayloadInfo;
    PayloadHeader rPayLoadHeader;

    //#ifdef WMV_FAST_I_FRAME_PARSING
    UINT64 u8PreKeyFrameAddr;
    UINT32 u4IndexPts;
    //#endif

    //#ifdef WMV_MULTI_REQUEST
    //HANDLE_T hWMVFeederDataQue;  // moved to common part
    UINT32 u4RequestDataSize;
    //#endif

    UINT32 u4VFifoStartAddr;
    UINT32 u4VFifoSize;
    UINT32 u4AFifoStartAddr;
    UINT32 u4AFifoSize;

    UINT32 u4WmaBytesPerSec;
    WMV_BAD_INTLV_STATUS_T rBadIntlv;

    UINT64 u8BeginParseOfst;  // CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT

    UINT32 u4InvalidStreamNumCount;     // Continuous invalid stream number count
} SWDMX_WMV_INFO_T;


#ifdef MKV_CONTENT_ENCODING_SUPPORT
typedef struct _SWDMX_MKV_CONTENT_COMPRESSION_T {
    UINT32 u4CompAlgo;
    UINT32 u4CompSettingsLen;
    UCHAR *pucCompSettings;
} SWDMX_MKV_CONTENT_COMPRESSION_T;


typedef struct _SWDMX_MKV_CONTENT_ENCODING_T {
    UINT32 u4Order;
    UINT32 u4Scope;
    UINT32 u4Type;
    SWDMX_MKV_CONTENT_COMPRESSION_T tCompression;
//    VOID *pvEncryption; for later support
    struct _SWDMX_MKV_CONTENT_ENCODING_T *ptNext;
} SWDMX_MKV_CONTENT_ENCODING_T;


typedef struct _SWDMX_MKV_CONTENT_ENCODINGS_T {
    SWDMX_MKV_CONTENT_ENCODING_T *ptContentEncoding;
    BOOL fgValid;
} SWDMX_MKV_CONTENT_ENCODINGS_T;
#endif


typedef struct {
    BOOL fgReadEOS;
    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnAudioLast;
    BOOL fgEnSubTitle;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBeginToPlay;
    BOOL fgSeqHdrParsed;

    BOOL fgChgATrack;
    BOOL fgAudTrigVidDec;

    BOOL fgChgSbTrack;
    BOOL fgEnSubDmx;

    BOOL fgIsRewindToStart;
    UINT8 u1FF2xTo1xMode;   // 0: flush fifo, 1: not flush fifo, start sending audio from swdmx's position, 2: audio pre-buffer
    // Feeder
    BOOL fgIsUnderFlw;
    BOOL fgFeederInvalid;
    BOOL fgFeederAck;
    INT32 i4ReqReadTimes;

    UINT64 u8CurMoviPos;
    UINT64 u8IClusterPos;
    UINT64 u8CurDmxMoviPos;
    UINT64 u8CurClusterPos;
    UINT32 u4LeftBytesInFeeder;

    UINT32 u4VDecType;
    UINT32 u4ADecType[MKV_MAX_AUD_TRACK];
    UINT32 u4MkvAvcType;

    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4CurAStrmID;
    UINT32 u4SpStrmID;

    UINT32 u4CurDmxFeederRPtr;

    UINT32 u4LastCbVidPts;
    UINT32 u4CurVidPts;
    UINT32 u4SBSkipVidPts;  //FOR DIVX SPS CHANGE
    UINT32 u4CurBlockPts;
    UINT32 u4ModeChangPts;

    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;

    UINT32 u4CurSentKey;
    UINT32 u4NumOfKeyEntry;
    //TPT
    UINT32 u4MainNumOfKeyEntry;
    UINT32 u4TPTNumOfKeyEntry;
    UINT64  u8FileOffset;
    UINT64  u8RangeSz;
    UINT64  u8MainFileOffset;
    UINT64  u8MainRangeSz;
    UINT64  u8TPTFileOffset;
    UINT64  u8TPTRangeSz;
    BOOL fgIsTPT;

    BOOL fgIsBlock;
    UINT32 u4BlockChunkSize;

    UINT32 u4FeederReqID;

    INT32 i4PrevSpeed;
    INT32 i4LastSpeed;

    //Video
    UINT32 u4KeyFrmSz;
    UINT32 u4KeyFrmPTS;
    UINT32 u4AudTrigVidDecPTS;
    UINT32 u4ChgATrackVidPTS;
    UINT32 u4VidFps;
    UINT64 u8VidOffset;
    UINT32 u4CurRptPTS;
    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;
    UINT32 u4MainVidHeaderPtr;
    UINT32 u4MainVidHdrLength;
    UINT32 u4TPTVidHeaderPtr;
    UINT32 u4TPTVidHdrLength;
    UINT8* aucAVCHdr;
    UINT8* aucVC1Hdr;
    UINT8 u1DispStruct;
    BOOL fgIsJump;

    //Audio
    BOOL fgIsAudWaitAout;
    BOOL fgActivePidxIsUnKnown;
    UINT8 u1CurATrackIdx;
    UINT8 u1TotalATrack;
    UINT8* aucAACHdr;
    UINT8* aucOggHdr;
    UINT32 u4OggPage[MKV_MAX_AUD_TRACK];
    UINT32 u4AudSampleRate[MKV_MAX_AUD_TRACK];
    UINT32 u4AudChannelNs[MKV_MAX_AUD_TRACK];
    UINT32 u4AudPts[MKV_MAX_AUD_TRACK];
    UINT32 u4SBSkipAudPts[MKV_MAX_AUD_TRACK];  //FOR DIVX SPS CHANGE
    UINT32 u4BKSBSkipAudPts[MKV_MAX_AUD_TRACK];   //FOR DIVX SPS CHANGE
    UINT32 u4AudHeaderPtr[MKV_MAX_AUD_TRACK];
    UINT32 u4AudHdrLength[MKV_MAX_AUD_TRACK];
    BOOL fgIsAudSkipDone[MKV_MAX_AUD_TRACK];   //FOR DIVX SPS CHANGE
    UINT8  auAudPidx[MKV_MAX_AUD_TRACK];
    BOOL fgAudPreBuf[MKV_MAX_AUD_TRACK];
    BOOL fgIsAudTrack[MKV_MAX_AUD_TRACK];
    BOOL fgIsVidSkipDone;   //FOR DIVX SPS CHANGE
    UINT64 au8PostionxTime[10][2];

    //Subtitle
    UINT8 u1SubPid;
    BOOL fgPendingSbMoveData;
    BOOL fgChangeSBDone;
    UINT32 i4BlkDuration;
    DMX_MM_DATA_T rSbDmxMMData;
    UINT64 u8SBClusterPos[MKV_MAX_SB_TRACK];   //FOR DIVX SPS CHANGE
    UINT32 u4SubPts[MKV_MAX_SB_TRACK];   //FOR DIVX SPS CHANGE
    BOOL fgIsSBTrack[MKV_MAX_SB_TRACK];   //FOR DIVX SPS CHANGE

    //Drm
    BOOL fgDrmEnable;
    UINT16 u2FrameKeyIdx;
    UINT32 u4EncryptOffset;
    UINT32 u4EncryptLength;

#ifdef MKV_CONTENT_ENCODING_SUPPORT
    // content encoding
    SWDMX_MKV_CONTENT_ENCODINGS_T tVidContentEncodings;
    SWDMX_MKV_CONTENT_ENCODINGS_T tAudContentEncodings[MKV_MAX_AUD_TRACK];
    BOOL fg1stAvc;
#endif

    //Seek
    BOOL fgIsSeek;
    UINT64 u8SeekPts;
    UINT64 u8VideoOnlyAdjustPts;
    UINT64 u8AudSeekPts;
    BOOL fgKeyDelivered;
    BOOL fgWaitIFrm;
    UINT32 u4KeyPTS;
    UINT32 u4KeyDeliveredPTS;

    UINT64 u8MkvSegmentInfoTimeCodeScale;
    UINT32 u4ClusterTimeCode;
    UINT32 u4PrsState;
    UINT32 au4MkvElementSa[MKV_MAX_LEVEL];
    UINT32 au4MkvElementSz[MKV_MAX_LEVEL];
    UINT32 au4MkvElementEa[MKV_MAX_LEVEL];
    UINT32 au4MkvElementLevel;
    UINT8 u1StrmType;
    UINT8 u1MkvAvcNaluLen;

    UINT32 u4MkvFrameCount;
    UINT32 u4MkvBHeaderSa;
    UINT32 u4MkvBHeaderSz; // size of _dMkvFrameCount

    UINT32 u4MkvFrmCntPrs;
    UINT32 u4MkvFrmAccSz;
    UINT32 au4MkvFrmCntPrsSz[MKV_MAX_FRAME_COUNT];

    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prMainVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prTPTVidKeyIdxTbl;

    MM_RANGE_INFO_T     *prDmxRangeInfo;

    FEEDER_BUF_INFO_T rFeederMkvInfo;
    UINT32 u4LastAlignOffset;
    BOOL fgWaitStartAddr;
    BOOL fgAudioNnotifyDecodeDone;

    UINT8 u1VidPid;
    UINT8 u1AudPid;

    UINT8* pAVADmaAddr;
    INT32 i4Speed;
    BOOL fgTtickSeekToNormal;
    BOOL fgSeekOffset;
    INT32 i4TtickSeekToNormalCount;
    BOOL fgHDRDELIVER;
} SWDMX_MKV_INFO_T;

//**********SWDMX MPG INFO structs*********//
// structs move from swdmx_mpg_psipsr_if.h
typedef enum _E_PSIPSR_TBL_TYPE
{
    E_PSIPSR_TBL_TVCT,
    E_PSIPSR_TBL_CVCT,
    E_PSIPSR_TBL_MGT,
    E_PSIPSR_TBL_PAT,
    E_PSIPSR_TBL_PMT    ,
    E_PSIPSR_TBL_STT    ,
    E_PSIPSR_TBL_PSI
} E_PSIPSR_TBL_TYPE;

typedef enum _E_PSIPSR_MSG
{
    E_PSIPSR_OK,
    E_PSIPSR_TIMEOUT,
    E_PSIPSR_SLD_NOT_MATCH,
    E_PSIPSR_TABLE_ERROR,
    E_PSIPSR_OUT_OF_BOUNDRY,
    E_PSIPSR_OUT_OF_RESOURCE,
    E_PSIPSR_ERROR
} E_PSIPSR_MSG;

typedef struct _PSIPSR_TBL_TS2FREQ_T
{
    UINT16 u2TsId;
    UINT8 u1PhyCh; // physical channel, to acquire frequency
    UINT8 u1Version;
} PSIPSR_TBL_TS2FREQ_T;

typedef struct _PSIPSR_TBL_PMT_T
{
    UINT32 u4PID;
    UINT32 u4PCR;
    UINT32 u4VPID;
    UINT32 u4VPID2;
    UINT32 u4SubVPID;
    UINT32 u4VPIDType; // add for h264
    UINT32 u4VPID2Type;
    UINT32 u4SubVPIDType; // add for multi-track
    UINT32 u4APID1;
    UINT32 u4APID2;
    UINT32 u4APID3;
    UINT32 u4APID1Type;
    UINT32 u4APID2Type;
    UINT32 u4APID3Type;
    UINT32 u4ProgNum;
    UINT8  au1VData[SWDMX_MVC_HDR_SIZE]; // for VPID only
} PSIPSR_TBL_PMT_T;


// 16 bytes (*128=2048bytes!)
typedef struct _PSIPSR_TBL_VCT_T
{
    UINT16 u2MajorChNum;
    UINT16 u2MinorChNum;
    UINT8 u1NextCh; //max 256 ch
    UCHAR szShortName[8];
    UINT16 u2ChTsId;
    UINT16 u2ProgNum;
    // volatile parts
    UINT16 u2PCR;
    UINT16 u2VPID;
    UINT16 u2VPIDType;
    UINT16 u2APID1;
    UINT16 u2APID1Type;
    UINT16 u2APID2;
    UINT16 u2APID2Type;
    UINT16 u2APID3;
    UINT16 u2APID3Type;

    UINT16 u2PhyFreq;
} PSIPSR_TBL_VCT_T;

typedef struct _PSIPSR_TBL_MGT_T
{
    UINT32 u4Type;
    UINT32 u4Pid;
    UINT32 u4Version;
    UINT32 u4Length;
} PSIPSR_TBL_MGT_T;

typedef struct _PSIPSR_TBL_STT_T
{
    UINT32 u4SysTime;
    UINT8 u1GPSUTCOffset;
    UINT16 u2DayLightSavings;

} PSIPSR_TBL_STT_T;

typedef struct _PSIPSR_SECTION
{
    UINT32 u4BufID;
    UINT32 u4Len;
    UINT32 u4Pidx;
} PSIPSR_SECTION;



typedef struct _PSIPSR_PARAM_T {
    UINT32 u4MaxEleNum;
    UINT32 u4RetEleNum;
    UINT32 u4TblType;
    UINT32 u4PMTTblIdx;
    UINT32 u4Timeout;
    //UINT32 u4Pidx;
    //UINT32 u4Filter;
    PSIPSR_TBL_VCT_T* prVctTbl;
    PSIPSR_TBL_TS2FREQ_T* prTsTbl;
    PSIPSR_TBL_MGT_T* prMgtTbl;
    PSIPSR_TBL_PMT_T* prPMTTbl;
    PSIPSR_TBL_STT_T* prSttTbl;

    UINT32 u4TsIndex;

} PSIPSR_PARAM_T;

typedef struct _PSIPSR_RTC_T
{
    UINT8   u1Year;
    UINT8   u1Month;
    UINT8   u1Day;
    UINT8   u1Hour;
    UINT8   u1Minute;
    UINT8   u1Second;
} PSIPSR_RTC_T;

// structs move from swdmx_mpg_idx.c
typedef struct
{
    UINT32 u4TotalTime;
    UINT32 u4NewTotalTime;
    UINT32 u4BasePosition;
    UINT32 u4BaseTime;
}SWDMX_MPG_INDEX_T;

typedef struct _SWDMX_MPG_TICK_DETAIL_T
{
    BOOL                fg_valid;
    UINT32              ui4_tick_num;
    UINT64              ui8_lba;
    UINT32              ui4_entries;
    /* number of picture info */
    /* PVR_PIC_INFO_T*  pt_pics;picture info */
    /* UINT64           ui8_system_time; check ui4_flags in PVR_TICK_INDEX_T*/
} SWDMX_MPG_TICK_DETAIL_T;

// structs move from swdmx_mpg_dmx.h
typedef struct
{
    UINT8                   u1SerialNumber;     ///< Serial number
    UINT32                  u4FrameAddr;        ///< Frame address
    UINT32                  u4Size;             ///< Frame size
} SWDMX_DMX_DATA_T;

/// Demux message type
typedef enum
{
    SWDMX_DMX_MSG_TYPE_PSI,                           ///< PSI message
    SWDMX_DMX_MSG_TYPE_PES                            ///< PES message
} SWDMX_DMX_MSG_TYPE_T;

/// Demux message structure
typedef struct
{
    SWDMX_DMX_MSG_TYPE_T      eType;                  ///< Message type
    UINT8               u1Id;                   ///< Filter id or PID id
    SWDMX_DMX_DATA_T      rData;                  ///< Type specific data
    const VOID*               pvNotifyTag;            ///< User tag value
} SWDMX_DMX_MSG_T;

// structs move from swdmx_mpg_dmx.c
typedef struct
{
    BOOL fgInUse;                   ///< Is this filter in-use or not
    UINT8 u1Pidx;                   ///< The pid id used of this filter
} _DMX_FILTER_T;

/// Internal PID structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this pid in-use or not
    UINT8 u1FilterId;               ///< The corresponding filter id if exist
    PFN_SWDMX_CALLBACK pfnHandler;    ///< The PES or section callback handler
} _DMX_PID_T;

/// Internal message structure
typedef struct
{
    BOOL fgInUse;                   ///< Is this message in-use or free
    SWDMX_DMX_MSG_T rMsg;             ///< The message body
} _DMX_MSG_T;

// stucts move from swdmx_mpg_pvr.c
typedef enum
{
    SWDMX_PVR_STATE_IDLE,
    SWDMX_PVR_STATE_PLAYING,
    SWDMX_PVR_STATE_PAUSE,
    SWDMX_PVR_STATE_NODATA,
    SWDMX_PVR_STATE_ERROR
} SWDMX_PVR_STATE_T;

typedef struct
{
    BOOL fgEnable;
    UINT16 u2PidNum;
    DMX_PID_TYPE_T eType;
} SWDMX_PVR_PID_T;

// structs move from swdmx_mpg.c
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
    UINT32 afgBufFeederIBCType[MPG_BUF_NUM_MAX];
#ifdef SWDMX_MPG_DEBUG_REQUEST_FILE_OFFSET
    UINT64 au8BufFileOffset[MPG_BUF_NUM_MAX];
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
    UINT64 au8ReqFileOffset[MPG_REQ_NUM_MAX];
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

    //ENUM_SWDMX_VDEC_T eVideoType;
    //ENUM_SWDMX_ADEC_T eAudioType;
    //ENUM_SWDMX_ADEC_T eAudio2Type;

    BOOL fgVideo;
    BOOL fgVideo2;
    BOOL fgAudio;
    BOOL fgAudio2;
    BOOL fgSubTitle;
    BOOL fgSeekaud;
    BOOL fgSeekvid;
    //BOOL fgPcr;
    UINT8 u1VideoKeyIndex;
    UINT8 u1Video2KeyIndex;
    UINT8 u1AudioKeyIndex;
    UINT8 u1Audio2KeyIndex;
    UINT16 u2VideoId;
    UINT16 u2Video2Id;
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
    UINT32 u4Video2DescMode;
    UINT32 u4AudioDescMode;
    UINT32 u4Audio2DescMode;

    VOID *pfnVScrambleNfy;
    VOID *pfnAScrambleNfy;
}SWDMX_MPG_FMT_T;

typedef struct
{
    BOOL fgSendEOF;
    BOOL fgSendAudioEos;
    BOOL fgSendVideoEos;
    BOOL fgSendVideo2Eos;
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
    BOOL fgGetFirstAudPts;   //  DTV00093908
    BOOL fgPesFound;
    BOOL fgSystemB;

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
    UINT32 u4SystemBPacketSize;

    UINT32 u4GetFirstAudioPTS;         //DTV00146545 to record the first audio pts which has been get ,after reset fgSetStc,

    UINT32 u4LastVideoWptr;
    UINT32 u4LastVideo2Wptr;
    UINT32 u4LastAudioWptr;
    UINT32 u4LastAudioFifoDataSize;
    UINT32 u4PreviousPTS;
    UINT32 u4SeekAudPTS;

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

#ifndef CC_TS_DISABLE
    PSIPSR_TBL_PMT_T arPMTTbl[SWDMX_MAX_PMT_NUM];
#endif

#ifdef SWDMX_MPG_PS_GET_INFO
 HANDLE_T hFindTimer;
 HANDLE_T hFindMsgQ;
 HANDLE_T hDataTimer;
 HANDLE_T hDataMsgQ;
#endif

 UINT32 fgSendSeqHdr;
#ifdef SWDMX_MPG_VC1_SUPPORT
 UINT32 fgSendSeqHdrVc1;
#endif
 UINT64 u8OrgSendedSize;

 BOOL fgGetInfo;
 BOOL fgCheckPSISize;
 UINT64 u8PlayPos;
 UINT64 u8CheckedPos;
 UINT64 u8SendedSize;
 UINT64 u8ReadedPos;
 UINT64 u8StartFileOffset;
 UINT64 u8FileSize;

 UINT32 u4FeederReqID;
 UINT32 u4FeederBufSize;
 UINT32 u4FeederBufNum ;
 UINT32 u4FeederReqNum ;

 HAL_TIME_T rJumpStartTime;
 HAL_TIME_T rJumpEndTime;

#ifdef SWDMX_STATISTICS
 HAL_TIME_T rFeederReqStartTime;
 HAL_TIME_T rFeederReqEndTime;
 UINT32 u4FeederAvgTime;
 UINT32 u4FeederMaxTime;
 UINT32 u4FeederMinTime;
 UINT32 u4FeederCount;
#endif

UINT8 u1PidxVideo;
UINT8 u1PidxVideo2;
UINT8 u1PidxAudio;
#ifdef SWDMX_MPG_SET_PCR_ID
 UINT8 u1PidxPCR;
#endif

 UINT32 u4OneTimeSize ;
 UINT32 u4OneTimeReadSize ;
 UINT32 u4AlignBase ;

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
    BOOL fgInScramble;
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
    UINT8 u1NumNoSignal;
    BOOL fgNeedAdjust;                // needed to adjust u8CurTime
    SWDMX_MPG_PVR_TICK_INDEX_T *prLastTickIndex;
    SWDMX_MPG_PVR_TICK_INDEX_T *prLastEndTickIndex;
    MM_REC_BUF_RANGE_INFO_T rRangeInfo;
    PVR_TICK_CTRL_BLK_T rTickCtl;
#endif
   // move to swdmx_mpg.c, because swdmx_drvif.h and vdec_drvif.h include each other.
   //VDEC_SEQUENCE_DATA_T rSeqInfo;
}SWDMX_MPG_VARIABLE_T;

typedef struct
{
    _DMX_FILTER_T arFilter[NUM_FILTER];
    _DMX_PID_T arPid[NUM_PID];
    _DMX_MSG_T arMsg[NUM_MESSAGE];
    //CRIT_STATE_T rLock;
    //BOOL fgDmxLocking ;
}SWDMX_MPG_DMX_T;

typedef struct
{
    UINT32 u4State;
    //HANDLE_T hPsiSema;
    UINT8 au1Buf[4096];
    SWDMX_DMX_MSG_T rDmxMsg;
}SWDMX_MPG_PSI_T;

typedef struct
{
     UINT32 u4Threshold;
     SWDMX_PVR_STATE_T eSWDMXPVRState;
     UINT32 u4PVRPlayTime;
     SWDMX_PVR_PID_T arPVRPlayPid[DMX_NUM_PID_INDEX];
     UINT32 u4PVRStopCount;
     INT32 i4PVRSpeed;
     BOOL fgPVRMpgResume;
     BOOL fgCheckHang;
     DMX_PVRPLAY_PTR_T rPVRPauseBufPtr;
     DMX_PVRPLAY_BUFPTR_T rCurPtr;
     HAL_TIME_T rSwDmxPVRTime;
}SWDMX_MPG_PVR_T;

typedef struct
{
     UINT32 u4SwdmxMpgIdxType;
     UINT32 u4SwdmxIdxSize;
     //static PVR_TICK_INDEX_T* _prTickIdx = NULL;
     // move to rSwdmxInfo
     //SWDMX_MPG_TICK_DETAIL_T* prSwdmxMpgTickTime;
     //SWDMX_MPG_TICK_DETAIL_T* prSwdmxMpgTickPosition;
     // ~move
     SWDMX_MPG_INDEX_T rSwdmxMpgIdx;

}SWDMX_MPG_IDX_T;

typedef struct
{
    SWDMX_MPG_BUFFER_T rBuf;
    SWDMX_MPG_FMT_T rFmt;
    SWDMX_MPG_VARIABLE_T rVar;
    SWDMX_MPG_DMX_T rDmx;
    SWDMX_MPG_PSI_T rPsi;
    SWDMX_MPG_PVR_T rPvr;
    SWDMX_MPG_IDX_T rIdx;
}SWDMX_MPG_INFO_T;


/* Real Media Logical Stream Index */
enum
{
    RM_STRM_VDO,
#ifdef RMLS
    RM_STRM_AUD,
#endif
    RM_STRM_MAX
};


/* Frame type enum */
typedef enum
{
    RVFrameTypePartial,
    RVFrameTypeWhole,
    RVFrameTypeLastPartial,
    RVFrameTypeMultiple
} RVFrameType;


/* Struct which holds frame header info */
typedef struct
{
    RVFrameType eType;
    UINT32      u4PacketNum;
    UINT32      u4NumPackets;
    UINT32      u4FrameSize;
    UINT32      u4PartialFrameSize;
    UINT32      u4PartialFrameOffset;
    UINT32      u4Timestamp;
    UINT8       u1SeqNum;
    UINT32      u4HeaderSize;
    UINT32      u4HeaderOffset;
    BOOL        fgBrokenUpByUs;
} RVFrameHdr;


typedef struct {

    BOOL fgEnVideo;
    BOOL fgEnAudio;

    BOOL fgBeginToPlay;
    BOOL fgFeederInvalid;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBlockVid;
    BOOL fgBlockAud;

    BOOL fgAudIsReady;

    BOOL fgPartialRangeDone;
    BOOL fgFeederLeftNotEnough;

    BOOL fgVDecIsPause;

    BOOL fgChgATrack;

    BOOL fgRequestData;

    UINT64 au8FileOffset[RM_STRM_MAX];
    UINT64 au8RangeSz[RM_STRM_MAX];

    UINT32 u4VDecType;
    UINT32 u4ADecType;

    UINT32 u4VidHeaderPtr;
    UINT32 u4VidHdrLength;

    UINT8 u1VidPid;
    UINT8 u1AudPid;

    UINT8 u1CurATrack;
    UINT8 u1TotalATrack;
    UINT32 u4AudPts[RM_MAX_AUDIO_TRACK];

    UINT32 u4AudHeaderPtr;
    UINT32 u4AudHdrLength;

    UINT32 u4AudSampleRate;
    UINT32 u4AudChannelNs;

    UINT32 u4PrevAStrmID;

    UINT32 u4VStrmID;
    UINT32 u4AStrmID;

    UINT64 u8VidLatestOff;
    UINT64 u8AudLatestOff;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;

    UINT32 u4VidFps;
    UINT64 u8PtsDiff;

    BOOL fgRepeatRequest;

    BOOL fgStartTrick;

    // For feeder usage
    BOOL fgReqDataMiss;
    UINT32 u4FeederReqID;
    UINT32 u4LeftBytesInFeeder;
    UINT32  u1StartCodeLenMinus1;
    UINT32 u4RecFeederWPtr;

    UINT64 au8CurFilePos[RM_STRM_MAX];
    UINT64 au8CurDmxFilePos[RM_STRM_MAX];
    UINT64 u8RecDmxMoviPos;

    // Record current demuxing position.
    UINT32 u4CurDmxFeederRPtr;
    INT32 i4ReqReadTimes;

    // Range Information
    MM_RANGE_INFO_T *prDmxRangeInfo;

    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prCurVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl[RM_MAX_AUDIO_TRACK];

    BOOL fgIsSeek;
    UINT64 u8ASeekPts;
    UINT64 u8VSeekPts;
    UINT64 u8SeekPts;
    UINT32 u4CurSentKey;

    UINT32 u4ModeChangPts;
    UINT32 u4ChangeMode;
    UINT32 u4PullBackAudPts;

    BOOL fgHasAud;

    UINT32 u4NumSentVid;
    UINT32 u4NumSentAud;

    BOOL fgSpeedChange;
    INT32 i4PrevSpeed;

    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;
#ifdef RMLS
    UINT32 u4LsPts[RM_STRM_MAX];
#endif

    UINT32 u4TriggerAudPts;

    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;

    UINT32 u4AFifoSa;
    UINT32 u4AFifoSz;

    BOOL fgIsBlock;

    UINT32 u4VFifoSa;
    UINT32 u4VFifoEa;
    UINT32 u4VFifoWpBak;
    UINT64 u8VFileOffsetBak;

    UINT64  u8LastIPtsInfo;
    UINT64  u8LastIFramePosition;

    UINT8 u1EccMask;

    COOK_INFO_T rCookInfo;
    BOOL fgCookPtsPending;
    BOOL fgCookSBPending;
    UINT32 u4CookSBPendingCnt;
    UINT32 u4CookSBPts;

#if 1  // for multi-instance
    FEEDER_BUF_INFO_T rFeederInfo;

    UCHAR *pucAudBuf;

//    MM_RANGE_ELEMT_IDXTBL_PB_ENTRY_T rPrevSample; no use in rm

    UINT32 u4LastAlignOffset;

//    HANDLE_T hRmIsrMsgQ; move to upper layer to prevent clear by _SWDMX_ResetSwdmxInfo

    DMX_MM_DATA_T rRmDmxMMData;

    UINT32 dwPrsState;
    UINT32 dwPktSz;
    UINT8 *pbHdrBuf;
    UINT16 wGTxLen;
    UINT32 u4FrmNo;

    RVFrameHdr rFrameHdr;

#ifdef SWDMX_BUFFER_REFINE
    UINT32 *au4SliceSz;
    UINT32 *au4SliceOfst;

    UINT32 *au4SliceSzBuf;
#else  // !!! refine the follows
    UINT32 au4SliceSz[128];
    UINT32 au4SliceOfst[128];

    UINT32 au4SliceSzBuf[RM_SLICE_SZ_BUF_NS];
#endif

    UINT32 dwSliceSzBufWp;
    UINT32 dwSliceSzBufRp;

    UINT32 dwFordwardRefPts;
    UINT32 dwBackwardRefPts;
    UINT32 dwCurrPicTr;
    UINT32 dwFordwardRefTr;
    UINT32 dwBackwardRefTr;

    UINT32 dwCurCookFrameInSB;

#ifdef SWDMX_BUFFER_REFINE
    UINT16 *pbPattern;
#else
    UINT16 pbPattern[100];
#endif

    UINT32 dwCurAFifoOfst;
#ifdef COOK_WRITE_AFIFO
    BOOL fgAFifoForCookSBFull;
#endif

    INT32 i4RmPtsAdj;

#ifdef __MODEL_slt__
    UINT8 u1RmSltLastPicType;
    UINT32 u4RmSltLastPicPts;
#endif

// for Real Media Logical Stream Support
    UINT32 u4RmStrmIdx;
#ifdef RMLS
    BOOL fgRmLs;
    BOOL fgRmChgStrm;
#endif

#endif

    UINT32 u4RequestSz;
} SWDMX_RM_INFO_T;


typedef struct {
    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgBeginToPlay;
    BOOL fgAudTrigVidDec;

    BOOL fgGotEosIbc;
    BOOL fgFeederInvalid;
    BOOL fgFeederTimeout;
    BOOL fgVDecIsPause;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;
    BOOL fgEnVDmx;
    BOOL fgEnADmx;

    BOOL fgChgATrack;
    BOOL fgSpeedChange;
    BOOL fgIsSeek;
    BOOL fgIsBlock;

    BOOL fgAudIsReady;
    BOOL fgWaitFirstAudio;
    BOOL fgWaitFirstVideo;
    BOOL fgAdjustPTS;

    UINT8 u1CurATrack;
    UINT8 u1TotalATrack;
    UINT8 u1VidPid;
    UINT8 u1AudPid;

    UINT32 u4VDecType;
    UINT32 u4ADecType;
    UINT32 u4AudPts[CMPB_MAX_AUDIO_TRACK];
    UINT32 u4AudSampleRate;
    UINT32 u4AudChannelNs;
    UINT32 u4PrevAStrmID;
    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4CurVidPts;
    UINT32 u4CurAudPts;
    UINT32 u4TriggerAudPts;
    UINT32 u4AFifoSa;
    UINT32 u4AFifoSz;
    UINT32 u4VFifoSa;
    UINT32 u4VFifoEa;
    UINT32 u4LastAlignOffset;
    UINT32 u4CurDmxFeederRPtr; // Record current demuxing position.
    UINT32 u4FeederReqID;
    UINT32 u4LeftBytesInFeeder;
    UINT32 u1StartCodeLenMinus1;
    UINT32 u4RecFeederWPtr;
    UINT32 u4StartCodeLen;
    UINT32 u4VidFps;
    UINT32 u4Req;
    UINT32 u4Ack;
    UINT32 u4Con;

    INT32 i4ReqReadTimes;
    INT32 i4PrevSpeed;

    // Range Information
    MM_RANGE_INFO_T *prDmxRangeInfo;

    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prCurVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl[CMPB_MAX_AUDIO_TRACK];

    UINT64 u8VidOffset;
    UINT64 u8VidLatestOff;
    UINT64 u8AudLatestOff;
    UINT64 u8PtsDiff;

    UINT64 u8CurFilePos;
    UINT64 u8CurDmxFilePos;
    UINT64 u8RecDmxMoviPos;
    UINT64 u4PullBackAudPts;

    // below are original local static variables
    FEEDER_BUF_INFO_T rCmpbFeederInfo;
    DMX_MM_DATA_T rCmpbDmxMMData;
} SWDMX_CMPB_INFO_T;

typedef struct {

    BOOL    fgValid;
    //BOOL    fgSwDmxInit;
    BOOL    fgFifoFull;
    BOOL    fgEnVideoDmx;
    BOOL    fgEnVideo2Dmx;
    BOOL    fgEnAudioDmx;
    BOOL    fgEnSubTitleDmx;
    BOOL    fgEnPcrDmx;
    BOOL    fgPlayChange;
    BOOL    fgPlayChangeOffset;
    BOOL    fgFlush;
    BOOL    fgRcvEOS;
    BOOL    fgDataExhausted;
    BOOL    fgWaitDataTimerStart;
    BOOL    fgSendUnderflow2MW;
#ifdef CC_DLNA_SUPPORT
    BOOL    fgFeederUnderFlow;
#endif
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    BOOL    fgIsBadInterleaved;                  //bad Interleaved file
#endif

    BOOL fgPreviouIsPause;
    UCHAR   *pucSwdmxInternalBuf;
    UINT32 u4UsedInternalBufPtr;

    ENUM_SWDMX_VDEC_T eVideoType;
    ENUM_SWDMX_VDEC_T eSubVideoType;
    ENUM_SWDMX_VDEC_T eVideo2Type;
    ENUM_SWDMX_ADEC_T eAudioType;
    ENUM_SWDMX_ADEC_T eAudio2Type;

    INT32   i4PlaySpeed;
    UINT32  u4VidStrmID;
    UINT32  u4SubVidStrmID;
    UINT32  u4AudStrmID;
    UINT32  u4AudSubStrmID;
    UINT32  u4Aud2StrmID;
    UINT32  u4SubTitleStrmID;
    UINT32  u4PcrStrmID;
    UINT32  u4TotalTime;
    UINT32  u4SeekTime;
    UINT32 u4DataUnderFlowCount;

    UINT8 u1SwdmxId;
    UINT8 u1DmxId;
    //UINT8 u1VdpId;
    UINT8 u1B2rId;
    UINT8 u1VdecId;
    UINT8 u1Vdec2Id;
    UINT8 u1AdecId;
    UINT8 u1StcId;
    UINT8 u1DataSrcType;
    SWDMX_PLAY_MODE_T ePlayMode;
    SWDMX_PLAY_MODE_T ePrePlayMode;

    //for playback performance
    ENUM_SWDMX_BUFFER_TYPE_T ePlaybackBufType;
    UINT32 u4PlaybackBufThreshold;
    UINT64  u8SeekPos;
    UINT64  u8FileSize;
    UINT64  u8FileOffset;
    UINT64  u8RangeSize;

    ENUM_SWDMX_PLAY_STATE_T eSwDmxPrevousStatus;
    ENUM_SWDMX_PLAY_STATE_T eSwDmxStatus;
    ENUM_SWDMX_FMT_T eCurFMT;

    DMX_MM_T rDmxVidMMInfo;
    DMX_MM_T rDmxAudMMInfo;

    DMX_MM_DATA_T rDmxMMData;

    HANDLE_T hRangeSema; // currently for time-shfit lock the range info
    HANDLE_T hWaitDataTimer;

    VOID*           pvSwdmxNfyTag;
    PFN_SWDMX_NFY_FCT pfSwdmxNfyFct;
    START_POS_INFO_T rSeekOffset;
    SWDMX_CUR_CMD_T rCmd;

    PFN_SWDMX_INIT    pfnSwdmxInit;
    PFN_SWDMX_PLAY pfnSwdmxPlay;
    PFN_SWDMX_PAUSE pfnSwdmxPause;
    PFN_SWDMX_ABORT pfnSwdmxAbort;
    PFN_SWDMX_GET_STREAM_INFO pfnSwdmxGetStreamInfo;
    PFN_SWDMX_SET_STREAM_ID pfnSwdmxSetStrmID;
    PFN_SWDMX_ENABLE_STREAM pfnSwdmxEnableStrm;
    PFN_SWDMX_DISABLE_STREAM pfnSwdmxDisableStrm;
    PFN_SWDMX_SET_RANGE pfnSwdmxSetRange;
    PFN_SWDMX_HEADER_PARSE pfnSwdmxHdrParse;
    PFN_SWDMX_STOP pfnSwdmxStop;
    PFN_SWDMX_FLUSH pfnSwdmxFlush;
    PFN_SWDMX_SET_INFO pfnSwdmxSetInfo;
    PFN_SWDMX_GET_INFO pfnSwdmxGetInfo;
    PFN_SWDMX_READ_BUFFER pfnSwdmxReadBuf;
    PFN_SWDMX_READ_BUFFER pfnSwdmxReleaseBuf;
    PFN_SWDMX_AUDIO_CB pfnSwdmxAudioCb;
    PFN_SWDMX_SEND_VIDEO_PES pfnSwdmxSendVideoPes;
    PFN_SWDMX_SEND_AUDIO_PES pfnSwdmxSendAudioPes;
    x_mm_range_nfy_fct pfnSwdmxRangeCb;
    x_mm_range_nfy_fct_ex pfnSwdmxRangeCbEx;

    SWDMX_RANGE_LIST_T *prRangeList;

    HANDLE_T hSwDmxMainThread;
    HANDLE_T hRangeMutex;
    HANDLE_T hWaitIdxBuild;
    HANDLE_T hDmxFeederReqDataQ;
    HANDLE_T hDmxPlayCmdQ;
    HANDLE_T hFlushMutex;
    HANDLE_T hFlushSema;
    HANDLE_T hRmIsrMsgQ;
    HANDLE_T hWMVFeederDataQue;  // moved from WMV rFmtInfo
    UINT8 *pu1VideoFrameData;    // moved from WMV rFmtInto

    SWDMX_RANGE_LIST_T rCurRangeReq;
    SWDMX_RANGE_LIST_T rQRangeReq;
#ifdef CC_MMP_BAD_INTERLEAVED_FILE_SUPPORT
    MM_RANGE_INFO_T rMMRangeInfo;
#endif

    FeederSourceType eFeederSrc;
    FEEDER_BUF_INFO_T rFeederInfo;

    UCHAR *pucAudBuf;

    SWDMX_MPG_TICK_DETAIL_T* prSwdmxMpgTickTime;
    SWDMX_MPG_TICK_DETAIL_T* prSwdmxMpgTickPosition;

    union
    {
        SWDMX_AVI_INFO_T rSwdmxAviInfo;
        SWDMX_MP4_INFO_T rSwdmxMp4Info;
        SWDMX_WMV_INFO_T rSwdmxWmvInfo;
        SWDMX_MKV_INFO_T rSwdmxMkvInfo;
        SWDMX_MPG_INFO_T rSwdmxMpgInfo;
        SWDMX_FLV_INFO_T rSwdmxFlvInfo;
        SWDMX_RM_INFO_T  rSwdmxRmInfo;
        SWDMX_OGG_INFO_T rSwdmxOggInfo;
        SWDMX_CMPB_INFO_T rSwdmxCmpbInfo;
    } rFmtInfo;
#ifdef CC_CMPB_PUSH_MODE
    // push mode
    UINT8 u1PushMode;
    sd_demuxer_t demuxer;
    sd_pb_ctrl_t pb_ctrl;
    sd_mem_list *pr_mem_list;
#endif

    #ifdef SWDMX_DBG_USB
    MM_DBG_INFO_T rDbgInfo;
    #endif // SWDMX_DBG_USB
    
} SWDMX_INFO_T;


typedef struct {
    HANDLE_T hSwdmxInfoMutex;
    HANDLE_T hSwdmxMoveSema;
    // debug info
    UCHAR ucLockSwdmxId;
    UINT32 u4SwdmxVFifoStart[SWDMX_SOURCE_MAX];
    UINT32 u4SwdmxVFifoEnd[SWDMX_SOURCE_MAX];
} SWDMX_KEEP_INFO_T;


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//--------------------
// swdmx_drvif.c

extern VOID _SWDMX_LockFlushMutex(UINT8 u1SrcId);
extern VOID _SWDMX_UnlockFlushMutex(UINT8 u1SrcId);
extern VOID _SWDMX_LockFlushSema(UINT8 u1SrcId);
extern VOID _SWDMX_UnlockFlushSema(UINT8 u1SrcId);

extern INT32 _SWDMX_ReceiveFeederAck(UINT8 u1SrcId, VOID *pvData);
extern INT32 _SWDMX_ReceiveFeederAckNoWait(UINT8 u1SrcId, VOID *pvData);
extern VOID _SWDMX_FlushReqMsgQ(UINT8 u1SrcId);
extern ENUM_SWDMX_PLAY_STATE_T _SWDMX_QueryStatus(UINT8 u1SrcId);
extern BOOL _SWDMX_SetRangeCb(UINT8 u1SrcId, VOID* pfnCbFunc);
extern BOOL _SWDMX_SetRangeCbEx(UINT8 u1SrcId, VOID* pfnCbFunc);
extern VOID _SWDMX_RANGE_CALLBACK (VOID*            pv_nfy_tag,
                               MM_RANGE_COND_T  e_nfy_cond,
                               UINT32           ui4_data_1,
                               UINT32           ui4_data_2,
                               UINT32           ui4_data_3);
extern BOOL _SWDMX_MpgTsFillBufferStart(UINT8 u1SrcId);
extern BOOL _SWDMX_MpgTsFillBufferEnd(UINT8 u1SrcId);
extern VOID _SWDMX_FeederCallBack(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);


extern BOOL _SWDMX_Init(VOID);

extern BOOL _SWDMX_GetStreamInfo(UINT8 u1SrcId,
                                 ENUM_SWDMX_FMT_T eType,
                                 UINT32 u4InfoMask,
                                 UINT64 u8FileSize,
                                 SWDMX_PROGRAM_INFO_T* prPInfo);

extern BOOL _SWDMX_Play(UINT8 u1SrcId);

extern BOOL _SWDMX_Pause(UINT8 u1SrcId);

extern BOOL _SWDMX_Stop(UINT8 u1SrcId);

extern BOOL _SWDMX_Flush(UINT8 u1SrcId);
extern BOOL _SWDMX_DoFlush(UINT8 u1SrcId);

extern BOOL _SWDMX_GetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);

extern BOOL _SWDMX_SetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3);

extern BOOL _SWDMX_AddRange(UINT8 u1SrcId, MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID);

extern BOOL _SWDMX_DelRange(UINT8 u1SrcId, MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID, BOOL fgDelAllRange);

extern BOOL _SWDMX_SetVideoStreamID(UINT8 u1SrcId, UINT32 u4VidStrmID);
extern BOOL _SWDMX_SetVideo2StreamID(UINT8 u1SrcId, UINT32 u4VidStrmID);
extern BOOL _SWDMX_SetAudioStreamID(UINT8 u1SrcId, UINT32 u4AudStrmID, UINT32 u4AudStrmSubID);
extern BOOL _SWDMX_SetAudio2StreamID(UINT8 u1SrcId, UINT32 u4AudStrmID);
extern BOOL _SWDMX_SetSubTitleStreamID(UINT8 u1SrcId, UINT32 u4SubTitleStrmID);
extern BOOL _SWDMX_SetPcrStreamID(UINT8 u1SrcId, UINT32 u4PcrStrmID);

extern BOOL _SWDMX_SetContainerType(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eContainerType);
extern BOOL _SWDMX_SetSpeed(UINT8 u1SrcId, INT32 i4Speed);
extern BOOL _SWDMX_SetPlayMode(UINT8 u1SrcId, SWDMX_PLAY_MODE_T ePlayMode);
extern BOOL _SWDMX_SeekTime(UINT8 u1SrcId, UINT32 u4SeekTime, UINT64* pu8FilePos);
extern BOOL _SWDMX_SeekPos(UINT8 u1SrcId, UINT64 u8SeekPos, UINT64 u8SeekPosI);
extern BOOL _SWDMX_SeekPercent(UINT8 u1SrcId, UINT32 u4SeekPercent, UINT64* pu8FilePos);
extern BOOL _SWDMX_SeekOffset(UINT8 u1SrcId, VOID *prPosition);
extern BOOL _SWDMX_RenderFromPos(UINT8 u1SrcId, VOID *prPosition);

extern BOOL _SWDMX_EnableStrm(UINT8 u1SrcId, UINT8 u1StrmType);
extern BOOL _SWDMX_DisableStrm(UINT8 u1SrcId, UINT8 u1Strmtype);

extern VOID _SWDMX_QueryRangeInfo(UINT8 u1SrcId, SWDMX_RANGE_INFO_T* prRangeInfo);

extern INT32 _SWDMX_SetDmxNfy(UINT8 u1SrcId, const SWDMX_NFY_INFO_T* prSwdmxNfyInfo);

extern BOOL _SWDMX_ReadBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo);
extern BOOL _SWDMX_ReleaseBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo);
extern BOOL _SWDMX_AudioNotify(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);
extern UINT16 _SWDMX_GetFeederQueueSize(UINT8 u1SrcId);
extern BOOL _SWDMX_PcrCallBack(UINT32 u4PcrBase);
extern UINT16 _SWDMX_GetNumOfCmd(UINT8 u1SrcId);
extern VOID _SWDMX_LockRangeSema(UINT8 u1SrcId);
extern VOID _SWDMX_UnlockRangeSema(UINT8 u1SrcId);
extern SWDMX_INFO_T* _SWDMX_GetInst(UINT8 u1SrcId);
extern BOOL _SWDMX_LockValidSrc(UINT8 u1SrcId);
extern VOID _SWDMX_UnlockSrc(UINT8 u1SrcId);
extern BOOL _SWDMX_MoveData(UINT8 u1SrcId, DMX_MM_DATA_T *prDmxMMData);
extern VOID _SWDMX_LockMoveMutex(UINT8 u1SrcId);
extern VOID _SWDMX_UnlockMoveMutex(UINT8 u1SrcId);
extern VOID _SWDMX_SetVideoFormat(UINT8 u1SrcId);
extern BOOL _SWDMX_GetVideoFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size);
extern BOOL _SWDMX_GetVideo2Fifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size);
extern BOOL _SWDMX_GetAudioFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size);
extern BOOL _SWDMX_GetAudioPreFifo(UINT8 u1SrcId, UINT32 *pu4Addr, UINT32 *pu4Size);
extern UINT64 _SWDMX_Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);

extern BOOL _SWDMX_VideoCallback(const DMX_PES_MSG_T* prPes);
extern BOOL _SWDMX_AudioCallback(const DMX_AUDIO_PES_T* prPes);

#ifdef SWDMX_STATISTICS
EXTERN VOID _SWDMX_ResetTime(VOID);
EXTERN VOID _SWDMX_PrintTime(VOID);
#endif

#ifdef CC_SWDMX_RENDER_PTS
EXTERN VOID _SWDMX_SetRenderPts(UINT8 u1SrcId, UINT32 u4RenderPts);
#endif

#ifdef CC_SWDMX_RENDER_POSITION_TEST
EXTERN VOID _SWDMX_GetRenderPosition(VOID *prRenderPos);
EXTERN VOID _SWDMX_SetRenderPosition(VOID *prRenderPos);
#endif
#ifdef CC_CMPB_PUSH_MODE
extern BOOL _SWDMX_VideoCallbackPush(const DMX_PES_MSG_T* prPes);
#endif

EXTERN BOOL SWDMX_PVR_Init(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_Pause(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_Resume(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_FlushBuf(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_Stop(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_SetPid(UINT8 u1SrcId, UINT8 u1Pidx, UINT16 u2PidNum, DMX_PID_TYPE_T ePidType,
                              BOOL fgEnable);
EXTERN void SWDMX_PVR_SetTime(UINT8 u1SrcId, UINT32 u4TimSec);
//EXTERN VOID SWDMX_PVR_SetIsPVR(BOOL fgPVRPlay);

EXTERN BOOL SWDMX_PVR_GetInfo(UINT8 u1SrcId, SWDMX_PROGRAM_INFO_T* prOpenInfo);
EXTERN BOOL SWDMX_PVR_OPEN(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_OPEN2(UINT8 u1SrcId, UINT32 u4BufStart, UINT32 u4BufSize);
EXTERN BOOL SWDMX_PVR_Close(UINT8 u1SrcId);
EXTERN BOOL SWDMX_PVR_SetDataMode(UINT8 u1SrcId, INT32 i4PlaySpeed);

EXTERN SWDMX_PVR_SENDDATA_RET_T SWDMX_PVR_SendData(UINT8 u1SrcId,
    const DMX_MM_DATA_T *prDmxMMData,
    UINT32 u4MinBuf,
    BOOL fgFastMode);

#if defined(SWDMX_DBG_USB) || defined(SWDMX_DBG_SEMIHOST)
enum
{
    DUMP_DATA_FEEDER_SWDMX = 0,
    DUMP_DATA_SWDMX_DMX_VIDEO,
    DUMP_DATA_SWDMX_DMX_AUDIO,
    DUMP_DATA_DMX_SWDMX_VDEC,
    DUMP_DATA_DMX_SWDMX_AUD,
    DUMP_DATA_VDEC_FRM,
    DUMP_DATA_FBM_DISPQ,
    DUMP_DATA_USER_DEF,
    DUMP_DATA_SIZE_MAX
};

#define SWDMX_DBG_SWITCH_NUM    DUMP_DATA_SIZE_MAX

EXTERN UINT32 Dbg_GetDbgSrc(UINT32 u4SrcId);
EXTERN VOID Dbg_SetDbgSrc(UINT32 u4SrcId, UINT32 u4Value);
EXTERN VOID Dbg_ResetSrc(VOID);
EXTERN BOOL Dbg_GetLogOnOff(VOID);
EXTERN VOID Dbg_SetLogOnOff(BOOL fgOn);
EXTERN CHAR *Dbg_GetInfo(UINT32 u4Src);

#define DUMP_DATA_USB_ON             (1)
#define DUMP_DATA_SEMI_ON            (2)
#define DUMP_DATA_OFF                   (0)

#define USB_DUMP_ON(id)                (DUMP_DATA_USB_ON == Dbg_GetDbgSrc(id))
#define SEMI_DUMP_ON(id)               (DUMP_DATA_SEMI_ON == Dbg_GetDbgSrc(id))
#define DUMP_LOG                           (TRUE == Dbg_GetLogOnOff())
#endif

#if defined(SWDMX_DBG_USB) 
#ifdef __KERNEL__
EXTERN BOOL Dbg_Register_DBG_CB(UINT8 u1SrcId, VOID *pvFunc);
EXTERN BOOL SWDMX_Release_source(VOID);
#endif

#ifndef NAME_MAX_LEN
    #define NAME_MAX_LEN      ((UINT32) 256)
#endif

#ifdef __KERNEL__
    #define FILE_LOCAL          "."
#else
    #define FILE_LOCAL          "/mnt/usb/Mass-000"
#endif

EXTERN BOOL Dbg_Util_Quit(VOID);
EXTERN BOOL Dbg_OpenUsbFile( CHAR *pcFileName, UINT32 *pu4hFile, BOOL fgAppend );
EXTERN BOOL Dbg_CloseUsbFile( UINT32 u4hFile, UINT32 *pu4hFile);
EXTERN BOOL Dbg_SaveBlock(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size);
EXTERN UINT32 Dbg_LoadUsbFile(CHAR *pcFileName, UINT32 u4Addr, UINT32 u4Size );
EXTERN BOOL Dbg_SaveUsbFile(HANDLE_T hFile, UINT32 u4Addr, UINT32 u4Size );
EXTERN BOOL Dbg_SaveCrc(HANDLE_T h_file, UINT32 u4_file_pos, UINT32 u4_addr, UINT32 u4_size );
EXTERN UINT32 Dbg_GetOrignalCrc( HANDLE_T hFile,  UINT64 u8Offset, UINT32 *pu4Crc );
EXTERN BOOL  Dbg_GetCrc( UINT32 addr, UINT32 len, UINT32 *pu4_crc);
#endif 

#if defined(SWDMX_DBG_SEMIHOST)
EXTERN INT32 T32_HostExec(INT32 i4TargetFlag, const CHAR *szFormat, ...);
EXTERN INT32 T32_HostSaveData(CHAR *szFileName, void *pvMemoryStart, UINT32 u4ByteSize);
EXTERN INT32 T32_HostLoadData(CHAR *szFileName, void *pvMemoryStart);
#endif // SWDMX_DBG_SEMIHOST

#endif

