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
 * $RCSfile: dmx_mm_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm_if.h
 *  Interface of demux driver
 */


#ifndef DMX_MM_IF_H
#define DMX_MM_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "dmx_if.h"
#include "mpv_drvif.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
;
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// PVR Play
#define PVRPLAY_FLAGS_BUFFER               0x00000001
#define PVRPLAY_FLAGS_TIMESTAMP            0x00000002
#define PVRPLAY_FLAGS_THRESHOLD            0x00000004
#define PVRPLAY_FLAGS_CALLBACK             0x00000008
#define PVRPLAY_FLAGS_MODE                 0x00000010
#define PVRPLAY_FLAGS_ALL                  0xFFFFFFFF
#define PVRPLAY_FLAGS_NONE                 0x00000000

#define DMX_MM_STRUCT_NUM                  (32)

/// Multi-media
#define DMX_MM_FLAG_NONE                    0x00000000
#define DMX_MM_FLAG_VALID                   0x00000001
#define DMX_MM_FLAG_TYPE                    0x00000002
#define DMX_MM_FLAG_BUF                     0x00000004
#define DMX_MM_FLAG_SEARCH_START_CODE       0x00000008
#define DMX_MM_FLAG_NOTIFY                  0x00000020
#define DMX_MM_FLAG_DEVICE_ID               0x00000040
#define DMX_MM_FLAG_INSTANCE_TAG            0x00008000  /* DMX_PID_FLAG_INSTANCE_TAG */
#define DMX_MM_FLAG_ALL                     0xFFFFFFFF

/// DRM
#define DMX_MM_DRM_FLAG_OFFSET              0x00000001
#define DMX_MM_DRM_FLAG_ENCRYLEN            0x00000002
#define DMX_MM_DRM_FLAG_CBC                 0x00000004
#define DMX_MM_DRM_FLAG_KEY                 0x00000008
#define DMX_MM_DRM_FLAG_ENCRYPT             0x00000010
#define DMX_MM_DRM_FLAG_NONE                0x00000000
#define DMX_MM_DRM_FLAG_ALL                 0xFFFFFFFF
#if defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
#define DMX_MM_DRM_FLAG_SAMPLE_ID           0x00000010
#define DMX_MM_DRM_FLAG_CHUNK_LEN           0x00000020
#endif //defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)

#define DMX_AUDIO_HANDLER_NULL_PIDX         0xFF

#define DMX_MAX_INSTANCE                    2   // TS, PS max instance

#define DMX_PS_MOVE_DATA_Q_SIZE     40
#define DMX_PS_MOVE_DATA_Q_PIDX     35
#define DMX_PS_STRUCT_NUM           16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    BOOL fgEnable;                  ///< En/Dis-able (see DMX_PID_FLAG_VALID)
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    BOOL fgEnableSsidFilter;        ///< Enable substream_id filtering (see DMX_PID_FLAG_SUBSTREAM_ID)
    UINT8  u1KeyIndex;              ///< Index of the scrambling key
    UINT8  u1StreamID;              ///< Stream ID
    UINT8  u1SubstreamID;           ///< Substream ID
    UINT32 u4BufAddr;               ///< Buffer address
    UINT32 u4BufSize;               ///< Buffer size
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4PeakBufFull;           ///< Peak fullness of the buffer, to
                                    ///< estimate the required ES FIFO size
    DMX_PID_TYPE_T  ePidType;       ///< Type of data (PSI, Video, Audio, etc.)
    DMX_DESC_MODE_T eDescMode;      ///< Descrambling mode
    PFN_DMX_NOTIFY pfnNotify;       ///< Callback function
    void*          pvNotifyTag;     ///< Tag value of callback function
    PFN_DMX_NOTIFY pfnScramble;     ///< Callback function of scramble state
    void*          pvScrambleTag;   ///< Tag value of scramble callback function

    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
    UINT8 u1ChannelId;
    UINT8 u1DeviceId;
} DMX_PS_T;


typedef struct
{
    BOOL fgEnable;                  ///< En/Dis-able (see DMX_PID_FLAG_VALID)
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    UINT8  u1KeyIndex;              ///< Index of the scrambling key
    UINT32 u4BufAddr;               ///< Buffer address
    UINT32 u4BufSize;               ///< Buffer size
    UINT32 u4Rp;                    ///< Read pointer
    UINT32 u4Wp;                    ///< Write pointer
    UINT32 u4PeakBufFull;           ///< Peak fullness of the buffer, to
                                    ///< estimate the required ES FIFO size
    DMX_PID_TYPE_T  ePidType;       ///< Type of data (PSI, Video, Audio, etc.)
    DMX_DESC_MODE_T eDescMode;      ///< Descrambling mode
    PFN_DMX_NOTIFY pfnNotify;       ///< Callback function
    void*          pvNotifyTag;     ///< Tag value of callback function
    PFN_DMX_NOTIFY pfnScramble;     ///< Callback function of scramble state
    void*          pvScrambleTag;   ///< Tag value of scramble callback function

    void *pvInstanceTag;            ///< Tag for supporting multiple instances (used only by v-dec)
    UINT8 u1ChannelId;
    UINT8 u1DeviceId;
} DMX_ES_T;


/// DRM mode
typedef enum
{
    DMX_DRM_MODE_BYPASS,
    DMX_DRM_MODE_AES,
    DMX_DRM_MODE_AES_SCTE52,
    DMX_DRM_MODE_ND,
    DMX_DRM_MODE_PD,
    DMX_DRM_MODE_AES_ENCRYPTION,
#ifdef DMX_SUPPORT_CPRM
    DMX_DRM_MODE_CPRM_C2D,
    DMX_DRM_MODE_CPRM_C2E,
    DMX_DRM_MODE_CPRM_C2G,
    DMX_DRM_MODE_CPRM_C2DCBC,
#endif // DMX_SUPPORT_CPRM
    DMX_DRM_MODE_UNKNOWN
} DMX_DRM_MODE_T;

typedef struct
{
    BOOL fgCBC;
    UINT8 au1IV[16];
    UINT32 u4KeyBitLen;
    UINT8 au1Key[32];
    UINT32 u4Offset;
    UINT32 u4EncryLen;
    BOOL fgEncrypt;
#if defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
    UINT32 u4ChunkLen;
    UINT8 au1SampleID[8];
#endif  //defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
} DMX_MM_DRM_AES_T;

#if defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
typedef struct
{
    UINT32 u4ContectKeyAddr;
    UINT32 u4ContectKeyLen;
    UINT32 u4ChunkLen;
    UINT32 u4EncryLen;
} DMX_MM_DRM_PD_T;
#endif

//---------------------------------------------------------------------
/// DDI
//---------------------------------------------------------------------
typedef enum
{
    DDI_EVENT_TRANSFER_OK,
    DDI_EVENT_TRANSFER_FAILED
} DDI_EVENT_CODE_T;

typedef BOOL (*PFN_DDI_NOTIFY)(DDI_EVENT_CODE_T);

#define DMX_DDI_FLAG_NONE               0x00000000
#define DMX_DDI_FLAG_MODE               0x00000001      // Set Mode
#define DMX_DDI_FLAG_ALLOCBUF           0x00000002      // Allocate Buffer
#define DMX_DDI_FLAG_CALLBACK           0x00000004      // Set/Get callback
#define DMX_DDI_FLAG_RATE               0x00000008      // Set/Get rate (M/N)
#define DMX_DDI_FLAG_DATA_SIZE          0x00000010      // Get data size in DDI
#define DMX_DDI_FLAG_DEBUG_INFO         0x00001000      // Get debug info
#define DMX_DDI_FLAG_ALL                0xFFFFFFFF

typedef enum
{
    DMX_DDI_PORT_FRAMER0,        // to Demux framer 0
    DMX_DDI_PORT_FRAMER1,
    DMX_DDI_PORT_FRAMER3,
    DMX_DDI_PORT_NO_FRAMER      // to Demux TS index 1 (DDI; bypass framer)
} DMX_DDI_PORT_T;

typedef enum
{
    DMX_DDI_MODE_SINGLE,        // Move a single chunk of data
    DMX_DDI_MODE_STREAM,        // Streaming data may have several chunks
    DMX_DDI_MODE_NONBLOCKING    // Non-blocking DDI DMA operation
} DMX_DDI_MODE_T;

typedef enum
{
    DMX_DDI_STOP,               // DMA is de-activated (in the Stop state)
    DMX_DDI_PLAY                // DMA is activated (in the Play state)
} DMX_DDI_STATE_T;

typedef struct
{
    DMX_DDI_STATE_T eState;         // internal DDI state
    UINT32 u4Rp;                    // Read pointer
    UINT32 u4Rip;                   // Read Initial pointer
    UINT32 u4Wp;                    // Write pointer
    UINT32 u4Ap;                    // Alert pointer
    UINT32 u4BufStart;              // starting address of DDI buffer
    UINT32 u4BufEnd;                // end address of DDI buffer
    UINT32 u4TransferredSize;       // transferred data size
}DMX_DDI_DEBUG_T;

typedef struct
{
    DMX_DDI_MODE_T eMode;   // Single mode or Stream mode
    BOOL fgAllocBuf;
    UINT32 u4BufAddr;
    UINT32 u4BufSize;
    UINT32 u4Threshold;     // INT when buffer is decreased to this value
    UINT32 u4DataSize;      // how much data is in the DDI buffer
    UINT32 u4RateN;         // rate = (N / M)  Mbits/sec
    UINT32 u4RateM;         // rate = (N / M)  Mbits/sec
    PFN_DDI_NOTIFY pfnDDINotify;
                            // The callback needs to return FALSE if error.
                            // Otherwise, return TRUE.
    DMX_DDI_DEBUG_T rDebugInfo;     // DDI debug info
} DMX_DDI_T;


//---------------------------------------------------------------------
/// Multi-media
//---------------------------------------------------------------------

typedef struct
{
    BOOL fgEnable;

    BOOL fgSearchStartCode;
    DMX_PID_TYPE_T ePidType;
    PFN_DMX_NOTIFY pfnCallback;

    BOOL fgAllocBuf;
    UINT32 u4BufAddr;
    UINT32 u4BufSize;
    UINT32 u4Rp;
    UINT32 u4Wp;
    void *pvInstanceTag;    // Tag for supporting multiple instances (used only by v-dec)
    UINT8 u1ChannelId;
    UINT8 u1DeviceId;
} DMX_MM_T;

typedef struct
{
    UINT8 u1Idx;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4StartAddr;
    UINT32 u4FrameSize;     // the data size to be transferred in a transaction
                            // For WMV, it's the size of a segment, not of a frame.
    UINT32 u4Pts;
    UINT32 u4Dts;

    BOOL fgEOS;             // End of stream
    BOOL fgFrameHead;       // Is a chunk the frame head? (for WMV/TS/PS/ES)
    BOOL fgIgnoreData;
#if defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
    BOOL fgTrigger;         // Set the Trigger (for NDDRM)
#endif  //defined(DMX_SUPPORT_NDDRM) || defined(DMX_SUPPORT_PDDRM) || defined(CC_DMX_EMULATION)
} DMX_MM_DATA_T;

typedef struct
{
    BOOL fgUsed;
    UINT8 u1CmdIndex;

    UINT8 u1Idx;
    UINT8 u1StreamID;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4StartAddr;
    UINT32 u4FrameSize;     // the data size to be transferred in a transaction
                            // For WMV, it's the size of a segment, not of a frame.
    UINT32 u4Pts;
    UINT32 u4Dts;

    //BOOL fgEOS;             // End of stream
    //BOOL fgFrameHead;       // Is a chunk the frame head? (for WMV/TS/PS/ES)

    UINT16 u2SkipLen[DMX_PS_MOVE_DATA_Q_SIZE];
    UINT16 u2PayloadLen[DMX_PS_MOVE_DATA_Q_SIZE];
    UINT32 u4PayloadEndAddr[DMX_PS_MOVE_DATA_Q_SIZE];

    UINT32 u4TimeOut;
} DMX_PS_MOVEDATA_CMD_T;

typedef struct
{
    PFN_DMX_SEND_VIDEO_PES pfnSendVideoPes;     ///< Send video PES
    PFN_DMX_UPDATE_WP pfnUpdateVideoWp;         ///< Update video WP
    PFN_DMX_SEND_AUDIO_PES pfnSendAudioPes;     ///< Send audio PES
} DMX_MM_DECODER_CALLBACKS_T;

//---------------------------------------------------------------------
/// Program Stream
//---------------------------------------------------------------------
#if 0
/// PID types
typedef enum
{
    DMX_PS_STREAM_TYPE_NONE = 0,      ///< None
    DMX_PS_STREAM_TYPE_AUDIO,
    DMX_PS_STREAM_TYPE_VIDEO,
    DMX_PS_STREAM_TYPE_OTHER          ///
} DMX_PS_STREAM_TYPE_T;

typedef struct
{
    BOOL fgEnable;                  ///< Enable or disable
    BOOL fgAllocateBuffer;          ///< Allocate buffer
    UINT8 u1StreamID;               ///< Stream ID
    UINT32 u4BufAddr;               ///< Buffer address
    UINT32 u4BufSize;               ///< Buffer size
    DMX_PS_STREAM_TYPE_T eStreamType;  ///< Stream type
} DMX_PS_STREAM_T;
#endif

//---------------------------------------------------------------------
/// PVR play
//---------------------------------------------------------------------
typedef enum
{
    DMX_PVRPLAY_PORT_FRAMER0,       // TS index: 0
    DMX_PVRPLAY_PORT_FRAMER1,       // TS index: 1
    DMX_PVRPLAY_PORT_DBM            // TS index: 2
} DMX_PVRPLAY_PORT_T;

typedef enum
{
    DMX_PVRPLAY_STATE_STOP,
    DMX_PVRPLAY_STATE_PLAYING
} DMX_PVR_PLAYSTATE_T;

typedef enum
{
    DMX_PVR_PLAY_STREAM,
    DMX_PVR_PLAY_SINGLEMOVE
} DMX_PVR_PLAYMODE_T;

typedef BOOL (*PFN_DMX_PVR_NOTIFY)(UINT32 u4Addr, UINT32 u4Size);

typedef struct
{
    DMX_PVR_PLAYSTATE_T eState;
    BOOL fgAllocBuf;
    UINT32 u4BufSize;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4Wp;
    UINT32 u4Rp;
    UINT32 u4ThresholdSize;

    BOOL fgIgnoreTimeStamp;
    BOOL fgContainTimeStamp;
    UINT16 u2TimeStampFreqDiv;

    DMX_PVR_PLAYMODE_T eMode;

    PFN_DMX_PVR_NOTIFY pfnPVRNotify;
} DMX_PVR_PLAY_T;

typedef struct
{
    UINT32 u4HwRp;
    UINT32 u4HwWp;
    UINT32 u4SwRp;
    UINT32 u4SwWp;
} DMX_PVRPLAY_BUFPTR_T;

typedef struct
{
    UINT32 u4Rp;
    UINT32 u4Wp;
} DMX_PVRPLAY_PTR_T;

//---------------------------------------------------------------------
/// DTCP
//---------------------------------------------------------------------
typedef struct
{
    UINT32 u4Size;

    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4SrcStart;

    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DstStart;
} DMX_DTCP_DATA_T;

typedef struct
{
    UINT32 u4Size;
    UINT32 u4SrcAddr;
    UINT32 u4DstAddr;
} DMX_DTCP_BLKDATA_T;

typedef struct
{
    UINT32 u4Size;

    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4SrcStart;

    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DstStart;
} DMX_AES_DATA_T;

typedef struct
{
    UINT32 u4Size;
    UINT32 u4SrcAddr;
    UINT32 u4DstAddr;
} DMX_AES_BLKDATA_T;

typedef struct
{
    BOOL fgUseTimestamp;
    UINT32 u4SingleCount;
    UINT32 u4SingleSize;
    UINT32 u4SingleCheck;
    UINT32 u4MoveCount;
    UINT32 u4MoveSize;
    UINT32 u4MoveCheck;
    UINT32 u4ShiftingCount;
    UINT32 u4DataSize;
} DMX_PVRPLAY_COUNTER_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------
/// PVR
//---------------------------------------------------------------------
EXTERN BOOL DMX_PVRPlay_SetDBMTestPid(void);
EXTERN BOOL DMX_PVRPlay_FreeDBMTestPid(void);
EXTERN BOOL DMX_PVRPlay_Start(void);
EXTERN BOOL DMX_PVRPlay_Stop(BOOL fgForce);
EXTERN BOOL DMX_PVRPlay_Pause(DMX_PVRPLAY_PTR_T *prPtr);
EXTERN BOOL DMX_PVRPlay_Resume(DMX_PVRPLAY_PTR_T *prPtr);
EXTERN VOID DMX_PVRPlay_Reset(void);
EXTERN DMX_PVR_PLAYSTATE_T DMX_PVRPlay_GetState(void);
EXTERN BOOL DMX_PVRPlay_Set(UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay);
EXTERN BOOL DMX_PVRPlay_Get(UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay);
EXTERN BOOL DMX_PVRPlay_UpdateWp(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL DMX_PVRPlay_FreeBuf(void);
EXTERN BOOL DMX_PVRPlay_FlushBuf(void);
EXTERN BOOL DMX_PVRPlay_SingleMove(const DMX_MM_DATA_T* prData);
EXTERN BOOL DMX_PVRPlay_GetBufPointer(DMX_PVRPLAY_BUFPTR_T *prPtr);
EXTERN BOOL DMX_PVRPlay_SetBuffer(UINT32 u4BufStart, UINT32 u4BufEnd,
                                  UINT32 u4Wp, UINT32 u4Rp);
EXTERN BOOL DMX_PVRPlay_RequestReset(void);
EXTERN BOOL DMX_PVRPlay_SetUseDescramble(BOOL fgEnable);

EXTERN void DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter);
EXTERN void DMX_PVRPlay_ResetCounter(void);

EXTERN BOOL DMX_OpenTimeshift(BOOL fgOpen);

//---------------------------------------------------------------------
/// DDI
//---------------------------------------------------------------------
EXTERN BOOL DMX_DDI_Init(void);
EXTERN BOOL DMX_DDI_Set(UINT32 u4Flags, const DMX_DDI_T *prDDI);
EXTERN BOOL DMX_DDI_Get(UINT32 u4Flags, DMX_DDI_T *prDDI);
EXTERN BOOL DMX_DDI_Free(void);

EXTERN BOOL DMX_DDI_FlushBuf(void);
EXTERN UINT32 DMX_DDI_GetFreeBufSize(UINT8 u1Inst);
EXTERN BOOL DMX_DDI_SetPacketSize(UINT8 u1PacketSize);
EXTERN BOOL DMX_DDI_SetSyncOffset(UINT8 u1Inst, UINT8 u1Offset);
EXTERN BOOL DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                               UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL DMX_DDI_PowerDown(VOID);
EXTERN BOOL DMX_DDI_RequestReset(VOID);
EXTERN BOOL DMX_DDI_130byteEnable(BOOL fgEnable, UINT8 u1PktSize);

//---------------------------------------------------------------------
/// Program Stream
//---------------------------------------------------------------------
EXTERN BOOL DMX_PS_SetStream(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PS_T *prPs);
EXTERN BOOL DMX_PS_GetStream(UINT8 u1Pidx, UINT32 u4Flags, DMX_PS_T *prPs);
EXTERN BOOL DMX_PS_FreeStream(UINT8 u1Pidx);
EXTERN BOOL DMX_PS_DetectStreamID(BOOL fgDetect, UINT16 u2Pairs, UINT32 u4Len);
EXTERN BOOL DMX_PS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType);
EXTERN DMX_AUDIO_TYPE_T DMX_PS_GetAudioType(UINT8 u1Pidx);
EXTERN BOOL DMX_PS_SetDataFormat(UINT8 u1Inst, DMX_DATA_FORMAT_T eDataFormat);
EXTERN DMX_DATA_FORMAT_T DMX_PS_GetDataFormat(UINT8 u1Inst);


//---------------------------------------------------------------------
/// Elementary Stream
//---------------------------------------------------------------------
EXTERN BOOL DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prPs);
EXTERN BOOL DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prPs);
EXTERN BOOL DMX_ES_FreePid(UINT8 u1Pidx);
EXTERN BOOL DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len);
EXTERN void DMX_ES_SetMoveIndex(UINT8 u1Idx);

//---------------------------------------------------------------------
/// Multi-media
//---------------------------------------------------------------------
EXTERN BOOL DMX_MM_Init(void);
EXTERN BOOL DMX_MM_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMMStruct);
EXTERN BOOL DMX_MM_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMMStruct);
EXTERN BOOL DMX_MM_Free(UINT8 u1Idx);

EXTERN VOID DMX_MM_SetSearchStartCode(BOOL fgEnable);
EXTERN BOOL DMX_MM_GetSearchStartCode(VOID);

EXTERN BOOL DMX_MM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T* prData, UINT32 u4TimeOut);
EXTERN UINT32 DMX_MM_GetBufEmptySize(UINT8 u1Idx);

EXTERN BOOL DMX_MM_SetDRMMode(UINT8 u1Idx, DMX_DRM_MODE_T eMode);
EXTERN DMX_DRM_MODE_T DMX_MM_GetDRMMode(UINT8 u1Idx);
EXTERN BOOL DMX_MM_SetAES(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);

EXTERN BOOL DMX_MM_UnlockBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4Size,
                                UINT32 u4FrameAddr);
EXTERN BOOL DMX_MM_GetBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
                    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);
EXTERN BOOL DMX_MM_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL DMX_MM_AllocateBuffer(UINT8 u1Idx, UINT32 u4Size);
EXTERN BOOL DMX_MM_FreeBuffer(UINT8 u1Idx);
EXTERN BOOL DMX_MM_SendAudioChunk(UINT8 u1Pidx, UINT32 u4Wp, UINT32 u4Pts, BOOL fgEOS);
EXTERN UINT8 DMX_MM_GetEnabledPidIndex(DMX_INPUT_TYPE_T eInputType, DMX_PID_TYPE_T ePidType);

//---------------------------------------------------------------------
/// DTCP
//---------------------------------------------------------------------
EXTERN void DMX_DTCP_SetFormat(DMX_INPUT_TYPE_T eInputType);
EXTERN BOOL DMX_DTCP_Set(void);
EXTERN BOOL DMX_DTCP_Free(void);
EXTERN BOOL DMX_DTCP_SetAES(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL DMX_DTCP_MoveData(const DMX_DTCP_DATA_T *prData);
EXTERN BOOL DMX_DTCP_MoveBlockData(const DMX_DTCP_BLKDATA_T *prData);

EXTERN BOOL DMX_AES_SetDecryptInfo(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL DMX_AES_MoveData(const DMX_AES_DATA_T *prData);
EXTERN BOOL DMX_AES_MoveBlockData(const DMX_AES_BLKDATA_T *prBlkData);

EXTERN UINT8 DMX_AES_AllocEngine(void);
EXTERN BOOL DMX_AES_FreeEngine(UINT8 u1Idx);
EXTERN BOOL DMX_AES_SetDecryptInfoEx(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL DMX_AES_MoveBlockDataEx(UINT8 u1Idx, const DMX_AES_BLKDATA_T *prBlkData);

EXTERN BOOL DMX_Marlin_SetDecryptInfo(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL DMX_Marlin_MoveData(UINT8 u1Idx, const DMX_DTCP_DATA_T *prData);

//---------------------------------------------------------------------
/// ND-DRM
//---------------------------------------------------------------------
#ifdef DMX_SUPPORT_NDDRM
EXTERN BOOL DMX_MM_SetNDDRM(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL DMX_NDDRM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut);
#endif  // DMX_SUPPORT_NDDRM

//---------------------------------------------------------------------
/// DMX AUDIO HANDLER
//---------------------------------------------------------------------
#ifdef CC_DMX_AUDIO_PREBUF
EXTERN BOOL DMX_AudHandler_SetActivePidx(UINT8 u1Idx);
EXTERN BOOL DMX_AudHandler_SetActivePidxEx(UINT8 u1Idx, UINT8* pu1Buf, UINT32 u4Size);
EXTERN BOOL DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec);
EXTERN BOOL DMX_AudHandler_Free(UINT8 u1Idx);
EXTERN BOOL DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts);
EXTERN void DMX_AudHandler_SetEnable(BOOL fgEnable);
EXTERN BOOL DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size);
#endif

//---------------------------------------------------------------------
/// DMX Multiple instance
//---------------------------------------------------------------------
EXTERN void DMX_MUL_EnableMultipleInst(BOOL fgEnable);
EXTERN UINT8 DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_T eConnType);
EXTERN BOOL DMX_MUL_FreeInst(UINT8 u1Inst);
EXTERN UINT8 DMX_MUL_GetPidxInstId(UINT8 u1Pidx);
EXTERN BOOL DMX_MUL_SetInstType(UINT8 u1Inst, DMX_INPUT_TYPE_T eInputType);
EXTERN UINT8 DMX_MUL_GetAvailablePidx(UINT8 u1Inst);
EXTERN BOOL DMX_MUL_SetSubtitlePidx(UINT8 u1Inst, UINT8 u1Pidx);
EXTERN UINT8 DMX_MUL_GetSubtitlePidx(UINT8 u1Inst);
EXTERN BOOL DMX_MUL_SetPidxInstance(UINT8 u1Inst, UINT8 u1Pidx);
EXTERN BOOL DMX_MUL_FreePidx(UINT8 u1Pidx);
EXTERN BOOL DMX_MUL_MoveData(UINT8 u1Id, DMX_MM_DATA_T *prData);
EXTERN BOOL DMX_MUL_RequestReset(UINT8 u1Id);
EXTERN UINT32 DMX_MUL_GetEmptySize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx);
EXTERN UINT32 DMX_MUL_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx);
EXTERN BOOL DMX_MUL_SetDecoderCallbacks(UINT8 u1Inst, const DMX_DECODER_CALLBACKS_T* prCallbacks);
EXTERN BOOL DMX_MUL_SetVideoType(UINT8 u1Inst, DMX_VIDEO_TYPE_T eVideoType);

#endif  // DMX_MM_IF_H

