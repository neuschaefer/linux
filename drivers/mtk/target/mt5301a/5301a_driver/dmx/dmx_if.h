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
 * $RCSfile: dmx_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_if.h
 *  Interface of demux driver
 */


#ifndef DMX_IF_H
#define DMX_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_timer.h"
#include "mpv_drvif.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define DMX_MEASURE_PSI_TIME

//#define DMX_CHECK_SECTION_CRC

// Use DTCP as general AES engine, so enable it by default
#ifdef ENABLE_MULTIMEDIA
#ifndef DMX_SUPPORT_DTCP
#define DMX_SUPPORT_DTCP
#endif  // DMX_SUPPORT_DTCP
#endif // ENABLE_MULTIMEDIA

#define CC_DMX_AUDIO_PREBUF

// The monitor thread is disabled under SLT.
#ifndef __MODEL_slt__
#define CC_DMX_ENABLE_MONTHREAD
#endif  // __MODEL_slt__

//#define CC_DMX_ERR_RECOVERY_TEST

#define CC_DMX_PURE_AUDIO

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// Demux resource limitations
#define DMX_FRAMER_COUNT                1

#define DMX_MIN_TUNER_INDEX             0       ///< Minimum tuner index
#define DMX_MAX_TUNER_INDEX             3       ///< Maximum tuner index
                                                ///< framer0, DDI, PVR
#define DMX_NUM_ALL_PID_INDEX           64      ///< Number of PIDs
#define DMX_NUM_PID_INDEX               32      ///< Number of PIDs
#define DMX_MAX_CA_MODE                 8       ///< Maximum CA mode
#define DMX_NUM_KEY_INDEX               8       ///< Maximum CA key index
#define DMX_NUM_FILTER_INDEX            32      ///< Number of filters
#define DMX_MIN_FILTER_OFFSET           0       ///< Minimum filter offset
#define DMX_MAX_FILTER_OFFSET           31      ///< Maximum filter offset
#define DMX_NUM_FILTER_BANK             2       ///< Number of filter bank
#define DMX_NUM_FILTER_MODE             4       ///< Number of filter mode

/// Function bitmap of PID operations
#define DMX_PID_FLAG_VALID              0x00000001  ///< Enabled or not
#define DMX_PID_FLAG_PID                0x00000002  ///< PID value
#define DMX_PID_FLAG_BUFFER             0x00000004  ///< Buffer control
#define DMX_PID_FLAG_CALLBACK           0x00000008  ///< Callback handler
#define DMX_PID_FLAG_SCRAMBLE_STATE     0x00000010  ///< Scrambling state
#define DMX_PID_FLAG_TS_INDEX           0x00000020  ///< TS index
#define DMX_PID_FLAG_PCR                0x00000040  ///< PCR mode
#define DMX_PID_FLAG_STEER              0x00000080  ///< Steering
#define DMX_PID_FLAG_DESC_MODE          0x00000100  ///< Descrambling mode
#define DMX_PID_FLAG_DEVICE_ID          0x00000200  ///< Device (decoder) ID
#define DMX_PID_FLAG_KEY_INDEX          0x00000400  ///< Key index
#define DMX_PID_FLAG_DATA_POINTERS      0x00000800  ///< Data pointers
#define DMX_PID_FLAG_PRIMARY            0x00001000  ///< Primary PID
#define DMX_PID_FLAG_STREAM_ID          0x00002000  ///< Stream ID
#define DMX_PID_FLAG_SUBSTREAM_ID       0x00004000  ///< Substream ID (SID)
#define DMX_PID_FLAG_NONE               0           ///< None
#define DMX_PID_FLAG_ALL                (0xffffffff & ~DMX_PID_FLAG_SCRAMBLE_STATE)

/// Function bitmap of filter operations
#define DMX_FILTER_FLAG_VALID           0x00000001  ///< Enabled or not
#define DMX_FILTER_FLAG_PIDX            0x00000002  ///< PID index
#define DMX_FILTER_FLAG_OFFSET          0x00000004  ///< Offset
#define DMX_FILTER_FLAG_CRC             0x00000008  ///< Check CRC or not
#define DMX_FILTER_FLAG_PATTERN         0x00000010  ///< Pattern and mask
#define DMX_FILTER_FLAG_MODE            0x00000020  ///< Filter mode
#define DMX_FILTER_FLAG_NONE            0           ///< None
#define DMX_FILTER_FLAG_ALL             0xffffffff  ///< All

/// Steering bitmap
#define DMX_STEER_TO_1394               0x01        ///< Steering to 1394
#define DMX_STEER_TO_PVR                0x02        ///< Steering to PVR
#define DMX_STEER_TO_FTUP               0x04        ///< Steering to uP

/// Alignments
#define DMX_CA_BUFFER_ALIGNMENT         32          ///< Alignment of CA buffers

// MPEG Picture types
#define PIC_TYPE_I                      0x01        ///< I picture
#define PIC_TYPE_P                      0x02        ///< P picture
#define PIC_TYPE_B                      0x03        ///< B picture
#define PIC_TYPE_SEQ_START              0x04        ///< Sequence start
#define PIC_TYPE_GOP                    0x05        ///< GOP
#define PIC_TYPE_SEQ_END                0x06        ///< Sequence end
#define PIC_TYPE_UNKNOWN                0xff        ///< Unknown picture type
#define MPV_PIC_VALID_TYPE(x)          (((x)>=PIC_TYPE_I)&&((x)<=PIC_TYPE_SEQ_END))

// H264 Picture types
#define H264_PIC_TYPE_NONIDR            0x11
#define H264_PIC_TYPE_IDR               0x12
#define H264_PIC_TYPE_SEI               0x13
#define H264_PIC_TYPE_SEQ               0x14
#define H264_PIC_TYPE_PIC               0x15
#define H264_PIC_TYPE_AU                0x16
#define H264_PIC_TYPE_UNKNOWN           0xff        ///< Unknown picture type
#define H264_PIC_VALID_TYPE(x)          (((x)>=H264_PIC_TYPE_NONIDR)&&((x)<=H264_PIC_TYPE_AU))

// VC1 Picture types
#define VC1_PIC_TYPE_I                  0x21
#define VC1_PIC_TYPE_P                  0x22
#define VC1_PIC_TYPE_B                  0x23
#define VC1_PIC_TYPE_BI                 0x24
#define VC1_PIC_TYPE_SKIP               0x25
#define VC1_PIC_TYPE_ENTRY              0x2E
#define VC1_PIC_TYPE_SEQ_START          0x2F
#define VC1_PIC_TYPE_UNKNOWN            0xFF
#define VC1_PIC_VALID_TYPE(x)          (((x)>=VC1_PIC_TYPE_I)&&((x)<=VC1_PIC_TYPE_SEQ_START))

// MPEG4 Picture types
#define MPEG4_PIC_TYPE_PIC              0x31
#define MPEG4_PIC_TYPE_OTHERS           0x32
#define MPEG4_PIC_TYPE_UNKNOWN          0xff        ///< Unknown picture type
#define MPEG4_PIC_VALID_TYPE(x)          (((x)>=MPEG4_PIC_TYPE_PIC)&&((x)<=MPEG4_PIC_TYPE_OTHERS))

// Misc
#define DMX_MAX_SERIAL_NUM              15

#define DMX_STARTCODE_NONINTR_NUM       4
#define DMX_STARTCODE_INTR_NUM          5


//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
EXTERN BOOL _fgAwEraserCheckBegin;

#define DMX_AW_ERR_OK                   0
#define DMX_AW_ERR_ADDRESS_RANGE        -3301
#define DMX_AW_ERR_SECTION_LEN          -3302
#define DMX_AW_ERR_NO_MEM               -3303
#define DMX_AW_ERR_COPY_BUFFER          -3304
#define DMX_AW_ERR_INSUFFICIENT_LEN     -3305
#define DMX_AW_ERR_NOT_ZEROED_OUT       -3306
#define DMX_AW_ERR_TIMEOUT              -3307

#define DMX_AW_MAX_PSI_LEN              4096
#define DMX_AW_VCT_HEADER               10
#define DMX_AW_VCT_SUBHEADER            32
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

#define DMX_PVRPLAY_TS_INDEX            2
#define DMX_DBM_TESTPID_IDX             31


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define DMX_IN_BROADCAST_MASK       0xF0
#define DMX_IN_PLAYBACK_MASK        0xF00

typedef enum
{
    DMX_IN_NONE         = 0,
    DMX_IN_DIAG         = 1,
    DMX_IN_JCHIP        = 2,
    DMX_IN_BROADCAST_TS = 0x10,
    DMX_IN_PLAYBACK_TS  = 0x100,
    DMX_IN_PLAYBACK_PS  = 0x200,
    DMX_IN_PLAYBACK_MM  = 0x300,
    DMX_IN_PVR_TS       = 0x400,
    DMX_IN_PLAYBACK_ES  = 0x500
} DMX_INPUT_TYPE_T;

typedef enum
{
    DMX_FE_ATSC_DEMOD = 0,
    DMX_FE_DVB_DEMOD,
    DMX_FE_EXT_PARALLEL,
    DMX_FE_EXT_SERIAL
} DMX_FRONTEND_T;

typedef enum
{
    DMX_FRAMER_PARALLEL,
    DMX_FRAMER_SERIAL,
    DMX_FRAMER_UNKNOWN
} DMX_FRAMER_MODE_T;

/// PMT data structure for PID swap
typedef struct
{
    UINT16 u2Pid;               ///< PID of this PMT
    UINT16 u2ProgramNumber;     ///< Program number
    UINT16 u2PcrPid;            ///< PCR PID
    UINT16 u2VideoPid;          ///< Video PID
    UINT16 u2AudioPid;          ///< Audio PID
    UINT8 u1AudioType;          ///< Type of audio stream
} DMX_PMT_T;

/// PID types
typedef enum
{
    DMX_PID_TYPE_NONE = 0,      ///< None
    DMX_PID_TYPE_PSI,           ///< PSI
    DMX_PID_TYPE_PES,           ///< PES
    DMX_PID_TYPE_PES_TIME,      ///< PES with time information
    DMX_PID_TYPE_ES_VIDEO,      ///< Video ES
    DMX_PID_TYPE_ES_AUDIO,      ///< Audio ES
    DMX_PID_TYPE_ES_VIDEOCLIP,  ///< Video clip ES
    DMX_PID_TYPE_ES_OTHER,      ///< Other ES
    DMX_PID_TYPE_ES_DTCP,
    DMX_PID_TYPE_PURE_AUDIO     ///< (CC_DMX_PURE_AUDIO) Simplex audio data such as MP3
} DMX_PID_TYPE_T;

/// PES message type
typedef enum
{
    DMX_PES_MSG_TYPE_PES,               ///< PES message
    DMX_PES_MSG_TYPE_PURGE,             ///< Purge message
    DMX_PES_MSG_TYPE_PURGE_REENABLE,    ///< Purge and re-enable message
    DMX_PES_MSG_TYPE_ERR_HANDLE
} DMX_PES_MSG_TYPE_T;

typedef enum
{
    DMX_VIDEO_MPEG,
    DMX_VIDEO_H264,
    DMX_VIDEO_VC1,
    DMX_VIDEO_MPEG4,
    DMX_VIDEO_WMV7,
    DMX_VIDEO_WMV8,
    DMX_VIDEO_WMV9,
    DMX_VIDEO_MP4_IN_WMV,
    DMX_VIDEO_VP6,
    DMX_VIDEO_VP8
} DMX_VIDEO_TYPE_T;

// Specify the audio format in the bit stream.
// This enumeration is only used in PS file playback.
typedef enum
{
    DMX_AUDIO_UNKNOWN,
    DMX_AUDIO_MPEG,
    DMX_AUDIO_LPCM,
    DMX_AUDIO_AC3
} DMX_AUDIO_TYPE_T;

// Specify whether the input data format is VCD (.DAT) format or not.
// This enumeration is only used in PS file playback.
typedef enum
{
    DMX_DATA_FORMAT_UNKNOWN,
    DMX_DATA_FORMAT_RAW,
    DMX_DATA_FORMAT_DAT
} DMX_DATA_FORMAT_T;


/// PES message
typedef struct
{
    DMX_PES_MSG_TYPE_T eMsgType;        ///< Message type
    DMX_PID_TYPE_T ePidType;            ///< PID type
    UINT32 u4BufStart;                  ///< Buffer start address
    UINT32 u4BufEnd;                    ///< Buffer end address
    UINT32 u4FrameAddr;                 ///< Frame (picture) address
    UINT32 u4FrameType;                 ///< Frame (picture) type
    UINT32 u4Wp;                        ///< Write pointer
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT8 u1Pidx;                       ///< PID index
    UINT8 u1Channel;                    ///< Video channel
    UINT8 u1DeviceId;                   ///< Device (decoder) ID
    UINT8 u1SerialNumber;               ///< Serial number
    BOOL fgPtsDts;                      ///< Is PTS/DTS field valid
    BOOL fgSeqHeader;                   ///< Is this frame is sequence header
    BOOL fgGop;                         ///< Is this frame is GOP

    BOOL fgEOS;                         ///< File playback: End of stream
    DMX_VIDEO_TYPE_T eVideoType;        ///< Video type of the input data (used by VC1 and WMV)
    UINT8 u1SliceCount;                ///< slice count in one picture (H.264)
} DMX_PES_MSG_T;

/// PSI message
typedef struct
{
    UINT8 u1Pidx;
    UINT8 u1SecCount;
    UINT32 u4EndAddr;
    UINT8 u1SN;                         ///< Serial Number
} DMX_PSI_MSG_T;

/// Audio PES structure
typedef struct
{
    UINT32 u4Wp;                        ///< Write pointer
    UINT32 u4PesRp;                     ///< Previous write pointer
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT8 u1PidIndex;                   ///< PID index
    UINT8 u1DeviceId;                   ///< Device (audio decoder) ID
    BOOL fgEOS;                         ///< File playback: End of stream
    UINT32 u4Info;                      ///< Audio info (for LPCM and AC3)
    BOOL fgCopyright;                   ///< The copyright flag in PES header
    BOOL fgOriginal;                    ///< The original_or_copy flag
} DMX_AUDIO_PES_T;

///
/// Prototypes of decoder callback functions
///

/// Callback handler of sending video PES. This handler will be called
/// whenever demux driver detects a picture start code in video ES FIFO.
typedef BOOL (*PFN_DMX_SEND_VIDEO_PES)(const DMX_PES_MSG_T* prPes);

/// Callback handler of sending audio PES. This handler will be called
/// whenever demux driver receives an audio frame.
typedef BOOL (*PFN_DMX_SEND_AUDIO_PES)(const DMX_AUDIO_PES_T* prPes);

/// Callback handler updating FIFO write pointer. This handler will be called
/// whenever demux driver updates write pointer of video ES FIFO.
typedef void (*PFN_DMX_UPDATE_WP)(UINT8 u1PidIndex, UINT32 u4Wp);

/// Decoder callback handlers
typedef struct
{
    PFN_DMX_SEND_VIDEO_PES pfnSendVideoPes;     ///< Send video PES
    PFN_DMX_UPDATE_WP pfnUpdateVideoWp;         ///< Update video WP
    PFN_DMX_SEND_AUDIO_PES pfnSendAudioPes;     ///< Send audio PES
} DMX_DECODER_CALLBACKS_T;

/// PCR mode
typedef enum
{
    DMX_PCR_MODE_NONE = 0,              ///< None
    DMX_PCR_MODE_OLD,                   ///< Old mode (5371 type)
    DMX_PCR_MODE_NEW                    ///< New mode
} DMX_PCR_MODE_T;

/// Descrambling schemes
typedef enum
{
    DMX_DESC_MODE_NONE = 0,             ///< None
    DMX_DESC_MODE_DES_ECB,              ///< DES ECB
    DMX_DESC_MODE_DES_CBC,              ///< DES CBC
    DMX_DESC_MODE_3DES_ECB,             ///< 3DES ECB
    DMX_DESC_MODE_3DES_CBC,             ///< 3DES CBC
    DMX_DESC_MODE_DVB,                  ///< DVB
    DMX_DESC_MODE_DVB_CONF,             ///< DVB conformance
    DMX_DESC_MODE_MULTI2_BIG,           ///< Multi-2 big-endian
    DMX_DESC_MODE_MULTI2_LITTLE,        ///< Multi-2 little-endian
    DMX_DESC_MODE_AES_ECB,              ///< AES ECB
    DMX_DESC_MODE_AES_CBC               ///< AES CBC
} DMX_DESC_MODE_T;

/// Notify info of PSI
typedef struct
{
    UINT32 u4SecAddr;                   ///< Section address
    UINT32 u4SecLen;                    ///< Section length
    UINT8 u1SerialNumber;               ///< Serial number
    UINT32 u4MatchBitmap;
} DMX_NOTIFY_INFO_PSI_T;

/// Notify info of ES data
typedef struct
{
    UINT32 u4Pts;                       ///< PTS
    UINT32 u4Dts;                       ///< DTS
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
} DMX_NOTIFY_INFO_ES_T;

/// Notify info of PES data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
    UINT8 u1SerialNumber;               ///< Serial number
} DMX_NOTIFY_INFO_PES_T;

/// Notify info of PES data
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
    UINT32 u4Pts;
    UINT32 u4Dts;
    UINT8 u1SerialNumber;               ///< Serial number
} DMX_NOTIFY_INFO_PES_TIME_T;

/// Notification structure for notification code DMX_NOTIFY_CODE_STREAM_ID
typedef struct
{
    UINT32 u4DataAddr;                  ///< Data address
    UINT32 u4DataSize;                  ///< Data size
} DMX_NOTIFY_INFO_STREAM_ID_T;

/// Scrambling state
typedef enum
{
    DMX_SCRAMBLE_STATE_CLEAR,           ///< Clear data
    DMX_SCRAMBLE_STATE_SCRAMBLED,       ///< Scrambled data
    DMX_SCRAMBLE_STATE_UNKNOWN          ///< Unknown data
} DMX_SCRAMBLE_STATE_T;

/// Notification code
typedef enum
{
    DMX_NOTIFY_CODE_PSI,                ///< PSI notification
    DMX_NOTIFY_CODE_ES,                 ///< ES notification
    DMX_NOTIFY_CODE_PES,                ///< PES notification
    DMX_NOTIFY_CODE_PES_TIME,           ///< PES notification with time information
    DMX_NOTIFY_CODE_SCRAMBLE_STATE,     ///< Scramble state change notification
    DMX_NOTIFY_CODE_OVERFLOW,           ///< Overflow notification
    DMX_NOTIFY_CODE_STREAM_ID           ///< Report pre-parsed Stream IDs
} DMX_NOTIFY_CODE_T;

/// Demux notification callback function
typedef BOOL (*PFN_DMX_NOTIFY)(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag);

/// PID data structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    BOOL fgPrimary;                 ///< Primary PID
    UINT8 u1TsIndex;                ///< TS index
    UINT8 u1DeviceId;               ///< Device ID
    UINT8 u1KeyIndex;               ///< Descramble key index
    UINT8 u1SteerMode;              ///< Steering mode
    UINT16 u2Pid;                   ///< PID
    UINT32 u4BufAddr;               ///< Buffer address
    UINT32 u4BufSize;               ///< Buffer size
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4PeakBufFull;           ///< Peak buffer fullness, to estimate
                                    ///< required ES FIFO size
    DMX_PCR_MODE_T ePcrMode;        ///< PCR mode
    DMX_PID_TYPE_T ePidType;        ///< PID type
    DMX_DESC_MODE_T eDescMode;      ///< Descramble mode
    PFN_DMX_NOTIFY pfnNotify;       ///< Callback function
    void* pvNotifyTag;              ///< Tag value of callback function
    PFN_DMX_NOTIFY pfnScramble;     ///< Callback function of scramble state
    void* pvScrambleTag;            ///< Tag value of scramble callback function
} DMX_PID_T;

/// Memory usage of each PID
typedef struct
{
    BOOL fgValid;                   ///< Is this struct valid or not?
    UINT32 u4PeakUsage;             ///< Peak memory usage per PID (preserved between "free")
    UINT32 u4CurrentUsage;          ///< Current memory usage per PID
} DMX_PID_MEM_USAGE_T;

/// Memory usage tracker
typedef struct
{
    DMX_PID_MEM_USAGE_T arPidMemUsage[DMX_NUM_PID_INDEX];
    UINT32 u4GlobalPeak;            ///< Global peak memory usage (preserved between "free")
    UINT32 u4GlobalCurrent;         ///< Global current memory usage
} DMX_MEM_TRACKER_T;


/// Filter mode
typedef enum
{
    DMX_FILTER_MODE_SHORT = 0,      ///< Short mode
    DMX_FILTER_MODE_14_2 = 1,       ///< 14 + 2 mode
    DMX_FILTER_MODE_LONG = 2,       ///< Long mode
    DMX_FILTER_MODE_POS_NEG = 3,    ///< Positive + negative mode
    DMX_FILTER_MODE_GENERIC = 4     ///< Generic mode
} DMX_FILTER_MODE_T;

/// Filter structure
typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index
    UINT8 u1Offset;                 ///< Offset
    UINT32 au4Data[2];              ///< Pattern
    UINT32 au4Mask[2];              ///< Mask
    DMX_FILTER_MODE_T eMode;        ///< Mode
} DMX_FILTER_T;

typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgCheckCrc;                ///< Check CRC or not
    UINT8 u1Pidx;                   ///< PID index, 0 ~ 31
    UINT8 u1Offset;                 ///< Offset
    UINT8 au1Data[16];              ///< Pattern
    UINT8 au1Mask[16];              ///< Mask
    UINT8 au1PosNeg[16];            ///< Positive or negative
} DMX_FILTER_GENERIC_T;

/// PID statistics
typedef struct
{
    UINT32 u4PesCount;              ///< PES counter
    UINT32 u4PicCount;              ///< Picture counter
    UINT32 u4SecCount;              ///< Section counter
    UINT32 u4OneByteSecCount;       ///< One Byte Section counter
    UINT32 u4TotalErrors;           ///< Total errors
    UINT32 u4DbmErrors;             ///< DBM errors
    UINT32 u4DescErrors;            ///< Descrambler errors
    UINT32 u4PcrErrors;             ///< PCR errors
    UINT32 u4SteerErrors;           ///< Steering errors
    UINT32 u4FTuPErrors;            ///< uP errors
    UINT32 u4MemoryErrors;          ///< Memory errors
    UINT32 u4PvrErrors;             ///< PVR errors
} DMX_PID_COUNTERS_T;

/// Notification code of capture buffer
typedef enum
{
    DMX_CAPTURE_NOTIFY_CODE_RECEIVE_DATA,   ///< Receive data
    DMX_CAPTURE_NOTIFY_CODE_BUFFER_FULL     ///< Buffer overflow
} DMX_CAPTURE_NOTIFY_CODE_T;

/// Capture info
typedef struct
{
    UINT32 u4BufStart;              ///< Buffer start address
    UINT32 u4BufEnd;                ///< Buffer end address
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4DataSize;              ///< Data size
} DMX_CAPTURE_INFO_T;

/// Callback handler of capture buffer
typedef BOOL (*PFN_DMX_CAPTURE_HANDLER)(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag);

typedef enum
{
    DMX_CAPTURE_FRAMER0,
    DMX_CAPTURE_DDI,
    DMX_CAPTURE_PVR,
    DMX_CAPTURE_TS_IDX3
} DMX_CAPTURE_DEV_T;


#define DMX_CAPTURE_BYPASS_NONE             0x00000000
#define DMX_CAPTURE_BYPASS_FRAMER0          0x04000000
#define DMX_CAPTURE_BYPASS_FRAMER1          0x08000000
#define DMX_CAPTURE_BYPASS_PVR              0x10000000
#define DMX_CAPTURE_BYPASS_DDI              0x20000000

/// Capture settings
typedef struct
{
    UINT32 u4BytePassDev;
    UINT32 u4BufSize;                           ///< Buffer size
    UINT32 u4Threshold;                         ///< Interrupt threshold
    PFN_DMX_CAPTURE_HANDLER pfnCaptureHandler;  ///< Capture handler
    void* pvCaptureTag;                         ///< Tag of capture handler
} DMX_CAPTURE_T;

typedef struct
{
    UINT32 u4BytePassDev;
    UINT32 u4BufStart;
    UINT32 u4BufSize;                           ///< Buffer size
    UINT32 u4Threshold;                         ///< Interrupt threshold
    PFN_DMX_CAPTURE_HANDLER pfnCaptureHandler;  ///< Capture handler
    void* pvCaptureTag;                         ///< Tag of capture handler
} DMX_CAPTURE2_T;

typedef enum
{
    DMX_LOCAL_ARBITOR_DMX = 0,
    DMX_LOCAL_ARBITOR_DDI,
    DMX_LOCAL_ARBITOR_PARSER,
    DMX_LOCAL_ARBITOR_NUM
} DMX_LOCAL_ARBITOR_DEVICE_T;

typedef enum
{
    DMX_ERR_ASSERT = 0,
    DMX_ERR_RESET,
    DMX_ERR_BYPASS
} DMX_ERR_LEVEL_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Data size calculation
#define DATASIZE(rp, wp, size)      \
    (((rp) <= (wp)) ? ((wp) - (rp)) : (((wp) + (size)) - (rp)))


#define EMPTYSIZE(rp, wp, size)     \
    (((wp) < (rp)) ? ((rp) - (wp)) : (((rp) + (size)) - (wp)))

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
// Startup

//-----------------------------------------------------------------------------
/** Initialize demux
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_Init(void);

EXTERN VOID DMX_SetIgnorePESLen(BOOL fgEnable);

EXTERN VOID DMX_SetScrambleScheme(BOOL fgOrg);

EXTERN VOID DMX_EnablePower(BOOL fgEnable);

EXTERN BOOL DMX_SelectInputType(DMX_INPUT_TYPE_T rInputType);

EXTERN DMX_INPUT_TYPE_T DMX_GetInputType(void);


//-----------------------------------------------------------------------------
/** Reset demux driver
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_Reset(void);

EXTERN BOOL DMX_SetFrontEnd(DMX_FRONTEND_T eFEType);

EXTERN DMX_FRONTEND_T DMX_GetFrontEnd(void);

EXTERN void DMX_ResetFramer(void);

EXTERN UINT8 DMX_GetFramerIndex(void);

EXTERN BOOL DMX_Start(void);

EXTERN BOOL DMX_Stop(void);

EXTERN VOID DMX_Version(void);

//---------------------------------------------------------------------
// PID operations

//-----------------------------------------------------------------------------
/** Set a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param  prPid           The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PID_T* prPes);

//-----------------------------------------------------------------------------
/** Get a PID
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Flags         Function flags
 *  @param[out] prPid       The PID structure of the PES
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid);

//-----------------------------------------------------------------------------
/** Get a PID
 *  CLI version, no API lock
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_CLIGetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_PID_T* prPid);

//-----------------------------------------------------------------------------
/** Free a PID
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_FreePid(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Find PID index for a given PID value
 *
 *  @param  u2Pid           PID
 *  @param[out] pu1PidIndex The PID index if found
 *
 *  @retval TRUE            Found
 *  @retval FALSE           Not found
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPidIndex(UINT16 u2Pid, UINT8* pu1PidIndex);

//-----------------------------------------------------------------------------
/** DMX_FlushPidBuffer(UINT8 u1Pidx)
 *  Flush PID buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_FlushPidBuffer(UINT8 u1Pidx);

//---------------------------------------------------------------------
// PCR

//-----------------------------------------------------------------------------
/** Set a PCR
 *
 *  @param  u1PidIndex      PID index (0 - 31)
 *  @param  fgEnable        Enable PCR of the PID or not
 *  @param  fgNewMode       Use the new PCR mode or not
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetPcr(UINT8 u1Pidx, BOOL fgEnable, BOOL fgNewMode);

//---------------------------------------------------------------------
// Descrambler

//-----------------------------------------------------------------------------
/** Set a desrambler key
 *
 *  @param  u1Index         The key index
 *  @param  fg3Des          TRUE: 3DES, FALSE: DES
 *  @param  fgEven          TRUE: even key, FALSE: odd key
 *  @param  au4Key          The key
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetDesKey(UINT8 u1Index, BOOL fg3Des, BOOL fgEven,
    const UINT32 au4Key[6]);

//-----------------------------------------------------------------------------
/** Set the initial vector of DES
 *
 *  @param  u4IvHi          The high word of the initial vector
 *  @param  u4IvLo          The low word of the initial vector
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDesIV(UINT32 u4IvHi, UINT32 u4IvLo);

EXTERN BOOL DMX_SetAesKeyLen(UINT32 u4KeyLen);

EXTERN BOOL DMX_SetAesKey(UINT8 u1Index, BOOL fgEven, const UINT32 au4Key[8]);

EXTERN void DMX_SetAesIV(const UINT32 au4Iv[4]);

//-----------------------------------------------------------------------------
/** Setup descrambler output buffer
 *
 *  @param  fgEnable        Enable or disable output buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCaOutputBuffer(BOOL fgEnable, BOOL fgIsPs,
    UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);

//-----------------------------------------------------------------------------
/** Setup descrambler input buffer
 *
 *  @param  fgEnable        Enable or disable input buffer
 *  @param  fgIsPs          TRUE: program stream, FALSE: transport stream
 *  @param  u4BufStart      Buffer start address
 *  @param  u4BufEnd        Buffer end address
 *  @param  u4Threshold     Interrupt threshold
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCaInputBuffer(BOOL fgEnable, BOOL fgIsPs,
    UINT32 u4BufStart, UINT32 u4BufEnd, UINT32 u4Threshold);


//---------------------------------------------------------------------
// Capture function

//-----------------------------------------------------------------------------
/** Setup capture using CA output buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCapture(BOOL fgEnable, const DMX_CAPTURE_T* prCapture);


//-----------------------------------------------------------------------------
/** Setup capture using CA output buffer, no allocate buffer
 *
 *  @param  fgEnable        Enable or disable capture function
 *  @param  prCapture       Capture settings
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetCapture2(BOOL fgEnable, const DMX_CAPTURE2_T* prCapture);


//-----------------------------------------------------------------------------
/** Update read pointer of capture buffer
 *
 *  @param  u4NewRp         The new read pointer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateCaptureReadPointer(UINT32 u4NewRp);

//-----------------------------------------------------------------------------
/** Get capture info
 *
 *  @param[out] prInfo      The capture info
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetCaptureInfo(DMX_CAPTURE_INFO_T* prInfo);

//---------------------------------------------------------------------
// PSI

//-----------------------------------------------------------------------------
/** Setup a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param  prFilter        Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetFilter(UINT8 u1FilterIndex, UINT8 u1Bank,
    UINT32 u4Flags, const DMX_FILTER_T* prFilter);

//-----------------------------------------------------------------------------
/** DMX_SetGenFilter
 *  Setup a generic section filter
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                               const DMX_FILTER_GENERIC_T* prFilter);

//-----------------------------------------------------------------------------
/** Get a section filter
 *
 *  @param  u1FilterIndex   Filter index (0 - 31)
 *  @param  u1Bank          Filter bank (0 - 1)
 *  @param  u4Flags         Function flags
 *  @param[out] prFilter    Filter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetFilter(UINT8 u1FilterIndex, UINT8 u1Bank, UINT32 u4Flags,
    DMX_FILTER_T* prFilter);

//-----------------------------------------------------------------------------
/** DMX_GetGenFilter
 *  Get a generic section filter setting
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetGenFilter(UINT8 u1FilterIndex, UINT32 u4Flags,
                DMX_FILTER_GENERIC_T* prFilter);


//-----------------------------------------------------------------------------
/** DMX_SetVideoType
 *  Set video type
 *
 *  @param  eVDec           VDec type
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetVideoType(DMX_VIDEO_TYPE_T eVDec);


//-----------------------------------------------------------------------------
/** DMX_GetVideoType
 *  Get video type
 */
//-----------------------------------------------------------------------------
EXTERN DMX_VIDEO_TYPE_T DMX_GetVideoType(void);


//-----------------------------------------------------------------------------
/** Allocate section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Free a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_FreeBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Unlock a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4Size, UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Flush a section buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_FlushBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Copy data from a section buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param[out] pu1Dest     Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PSI_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

//-----------------------------------------------------------------------------
/** Get the free size of the PTS-PSI buffer.
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval                 The free size of the PTS-PSI buffer.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_PSI_GetMinFreeBufferSize(VOID);

#ifdef DMX_MEASURE_PSI_TIME
EXTERN VOID DMX_PSI_GetMaxTime(HAL_TIME_T* prTime);

EXTERN VOID DMX_ResetPSIMaxTime(void);
#endif  // DMX_MEASURE_PSI_TIME

//---------------------------------------------------------------------
// PES

//-----------------------------------------------------------------------------
/** Allocate PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u4Size          Buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_AllocateBuffer(UINT8 u1Pidx, UINT32 u4Size);

//-----------------------------------------------------------------------------
/** Free a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_FreeBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Unlock a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4Size          Unlocked data size
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_UnlockBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4Size, UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Flush a PES buffer
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_FlushBuffer(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Copy data from a PES buffer
 *
 *  @param  u1Pidx          PID index
 *  @param  u1SerialNumber  The serial number
 *  @param  u4FrameAddr     Frame address
 *  @param  u4SkipSize      Skip size before copying
 *  @param  u4CopySize      Data size to copy
 *  @param[out] pu1Dest     Destination buffer
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_PES_GetBuffer(UINT8 u1Pidx, UINT8 u1SerialNumber,
    UINT32 u4FrameAddr, UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);

//---------------------------------------------------------------------
// IDE test port

//-----------------------------------------------------------------------------
/** Enable or disable IDE test port.
 *  This function is used for IC emulation only.
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_IDE_Enable(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** Query if IDE is enabled.
 *  This function is used for IC emulation only.
 *
 *  @retval TRUE            Enabled
 *  @retval FALSE           Disabled
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_IDE_IsEnabled(void);

//---------------------------------------------------------------------
// Inter-driver interfaces

//-----------------------------------------------------------------------------
/** Update PID read pointer.
 *  This function is used by decoder drivers only.
 *
 *  @param  u1PIdx          PID index
 *  @param  u4Rp            The read pointer
 *  @param  u4FrameAddr     Original frame address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateReadPointer(UINT8 u1Pidx, UINT32 u4Rp,
    UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Update read pointer of video PID buffer.
 *  This function is used by video decoder driver only.
 *
 *  @param  u1Channel       The channel
 *  @param  u4Rp            The new read pointer
 *  @param  u4FrameAddr     The frame (picture) address
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_UpdateVideoReadPointer(UINT8 u1Channel, UINT32 u4Rp,
    UINT32 u4FrameAddr);

//-----------------------------------------------------------------------------
/** Set interrupt threshold for partial PES.
 *  This function is used by decoder drivers only.
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param  u4Bytes         Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetInterruptThreshold(DMX_PID_TYPE_T ePidType,
    UINT32 u4Bytes);

//-----------------------------------------------------------------------------
/** Get interrupt threshold for partial PES.
 *  This function is used by decoder drivers only.
 *
 *  @param  ePidType        PID type, valid for video and audio only
 *  @param[out] pu4Bytes    Threshold in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetInterruptThreshold(DMX_PID_TYPE_T ePidType,
    UINT32* pu4Bytes);

//---------------------------------------------------------------------
// Misc.

//-----------------------------------------------------------------------------
/** Set decoder callback functions
 *
 *  @param  prCallbacks     The decoder callback functions
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDecoderCallbacks(
    const DMX_DECODER_CALLBACKS_T* prDecoderCallbacks);

//-----------------------------------------------------------------------------
/** Get corresponding video channel of a given pid index
 *
 *  @param  u1Pid           PID index
 *  @param[out] pu1Channel  The channel
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetVideoChannel(UINT8 u1Pidx, UINT8* pu1Channel);

//-----------------------------------------------------------------------------
/** Set demux (DBM) to bypass mode, mainly used for frontend capture function
 *
 *  @param  u1TsIndex       TS index (0 - 1)
 *  @param  u1PacketSize    TS packet size in byte
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_SetBypassMode(UINT8 u1TsIndex, UINT8 u1PacketSize);

//-----------------------------------------------------------------------------
/** Set data sink to real decoders
 *
 *  @param  fgToDecoder     TRUE: to decoders, FALSE: not to decoders
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetToDecoder(BOOL fgToDecoder);

//-----------------------------------------------------------------------------
/** Query if data sink is set to real decoders
 *
 *  @retval TRUE            Yes
 *  @retval FALSE           No
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_IsToDecoder(void);

//---------------------------------------------------------------------
// Utilities

//-----------------------------------------------------------------------------
/** Calculate aligned address by a given address and an alignment
 *
 *  @param  u4Addr          The address to be aligned
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @return     The aligned address
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_Align(UINT32 u4Addr, UINT32 u4Alignment);

//-----------------------------------------------------------------------------
/** Query if an address is aligned or not
 *
 *  @param  u4Addr          The address to be checked
 *  @param  u4Alignment     The alignment requirement in byte
 *
 *  @retval TRUE            Aligned
 *  @retval FALSE           Not aligned
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_IsAligned(UINT32 u4Addr, UINT32 u4Alignment);

//---------------------------------------------------------------------
// Debug

EXTERN BOOL DMX_GetLockState(void);

EXTERN BOOL DMX_GetLockStateEx(UINT8 u1FramerIdx);

EXTERN BOOL DMX_IsDebugPictureFinder(void);

//-----------------------------------------------------------------------------
/** Enable or disable debugging picture finder.
 *  This function is for debugging purpose only.
 *
 *  @param  fgEnable        TRUE: enable, FALSE: disable
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetDebugPictureFinder(BOOL fgEnable);

//-----------------------------------------------------------------------------
/** Get the counter structure of a given pid index.
 *  This function is for debugging purpose only.
 *
 *  @param  u1Pidx          PID index
 *  @param[out] prCounters  Pointer to the PID counter structure
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_GetPidCounters(UINT8 u1Pidx,
    DMX_PID_COUNTERS_T* const prCounters);

//-----------------------------------------------------------------------------
/** Reset counters of a given pid index.
 *  This function is for debugging purpose only.
 *
 *  @param  u1Pidx          PID index
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL DMX_ResetPidCounters(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Set error handling table of framers.
 *  This function is for capture tool only.
 *
 *  @param  u4Value         New value of error handling table
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void DMX_SetFramerErrorHandlingTable(UINT32 u4Value);

//-----------------------------------------------------------------------------
/** Get error handling table of framers.
 *  This function is for capture tool only.
 *
 *  @return     The current value of error handling table
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 DMX_GetFramerErrorHandlingTable(void);

//-----------------------------------------------------------------------------
/** Get scramble state
 *
 *  @param  u1Pidx          PID index
 *
 *  @return     The scramble state
 */
//-----------------------------------------------------------------------------
EXTERN DMX_SCRAMBLE_STATE_T DMX_GetScrambleState(UINT8 u1Pidx);

//-----------------------------------------------------------------------------
/** Demux diagnostic function.
 *  This function is for debugging purpose only.
 *
 *  @retval 0               Succeed
 *  @retval otherwise       Fail. The return value is error code.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 DMX_Diag(void);

EXTERN INT32 DMX_JChipDiag(UINT32 u1TsIndex);

EXTERN UINT32 DMX_GetReceivedPacketNumber(void);

EXTERN UINT32 DMX_GetDroppedPacketNumber(void);

EXTERN BOOL DMX_SetPacketSize(UINT8 u1PacketSize);

EXTERN UINT32 DMX_GetMinFreeBufferSize(DMX_PID_TYPE_T ePidType);

EXTERN BOOL DMX_SetTsIndex(UINT8 u1Idx);

EXTERN void DMX_GetMemoryUsage(DMX_MEM_TRACKER_T *pMemTracker);

EXTERN void DMX_PcrInit(void);

EXTERN BOOL DMX_EnableLocalArbitor(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, BOOL fgEnable);

EXTERN BOOL DMX_DRAMMeasure(DMX_LOCAL_ARBITOR_DEVICE_T eDevice, UINT32 u4TimeMs);

EXTERN void DMX_CheckAwSupport(BOOL *pfgDriver, BOOL *pfgMicroCode);

//-----------------------------------------------------------------------------
// --- Beginning of "check for AW Eraser" ---
EXTERN void DMX_AwEraserCheck_Init(void);
EXTERN void DMX_AwEraserCheck_Free(void);
EXTERN void DMX_AwEraserCheck_Set(BOOL fgEnable);
EXTERN INT32 DMX_AwEraserCheck_Get(UINT32 u4Timeout);
// --- End of "check for AW Eraser" ---
//-----------------------------------------------------------------------------

EXTERN void DMX_SetErrLevel(DMX_ERR_LEVEL_T eLevel);

#endif  // DMX_IF_H

