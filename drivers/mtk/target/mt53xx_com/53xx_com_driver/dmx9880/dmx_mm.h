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
 * $RCSfile: dmx_mm.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_mm.h
 *  Demux multi-media driver - common definitions
 */

#ifndef DMX_MM_H
#define DMX_MM_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm_if.h"
#include "dmx.h"
#include "x_hal_5381.h"


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DDI_BUF_ALIGNMENT                       16
#define DDI_POINTER_ALIGNMENT                   4

#define MM_WAITDMX_POLLING_TIMES                50


//
// DDI registers
//

#define DDI_BASE                                (IO_VIRT + 0x1e000) // 0x2001e000

#define DDI_REG_GLOBAL_CTRL                     0xc00

#define DDI_REG_PERIOD_M                        0x410
#define DDI_REG_PERIOD_N                        0x414
#define DDI_REG_PERIOD_K                        0x418
#define DDI_REG_RATE_CMD                        0x420
#define DDI_REG_LATCHED_PERIOD_M                0x440
#define DDI_REG_LATCHED_PERIOD_N                0x444
#define DDI_REG_PKT_QUADBYTE_LIMIT              0x450
#define DDI_REG_DMX_RX_CTRL                     0x460
#define DDI_REG_DMA_REAL_RP                     0x4D0

#define DDI_REG_DCR_INT_SET                     0x800
#define DDI_REG_DCR_INT_CLR                     0x804
#define DDI_REG_DCR_INT_MASK                    0x808
#define DDI_REG_DMA_BUF_START                   0x850
#define DDI_REG_DMA_BUF_END                     0x854
#define DDI_REG_DMA_RP                          0x858
#define DDI_REG_DMA_RP_INIT                     0x85c
#define DDI_REG_DMA_WP                          0x864
#define DDI_REG_DMA_AP                          0x868
#define DDI_REG_DMA_CTRL                        0x86c


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#if 0
//// Program Stream Struct
typedef struct
{
    BOOL fgEnable;              // Valid
    BOOL fgAllocateBuffer;      // Allocate buffer or not

    // Buffer info
    UINT32 u4BufStart;          // Buffer start
    UINT32 u4BufEnd;            // Buffer end
    UINT32 u4BufLen;            // Buffer length
    UINT32 u4Rp;                // Read pointer
    UINT32 u4Wp;                // Write pointer
    UINT32 u4SectionRp;         // Section read pointer
    UINT32 u4PesRp;             // PES read pointer

    UINT32 u4HeaderBufAddr;     // Header buffer start
    UINT32 u4NextPic;           // Next picture number in current PES

    UINT8 u1StreamID;
    DMX_PS_STREAM_TYPE_T eStreamType;  ///< Stream type
} PS_STRUCT_T;
#endif

/// DDI
typedef struct
{
    DMX_DDI_MODE_T eMode;
    DMX_DDI_STATE_T eState;
    UINT32 u4RateN;         // rate = (N / M) Mbits/sec
    UINT32 u4RateM;         // rate = (N / M) Mbits/sec
    BOOL fgAllocBuf;        // Indicate if DDI allocates buffer for users.
    UINT32 u4BufAddr;       // the starting address of buffer
    UINT32 u4BufSize;       // buffer size
    UINT32 u4Threshold;     // copy data if the threshold of free space is met
    UINT32 u4Rp;            // read pointer to buffer
    UINT32 u4Wp;            // write pointer to buffer
    PFN_DDI_NOTIFY pfnDDINotify;
    UINT32 u4TransferredSize;   // transferred data size
} DMX_DDI_STRUCT_T;

typedef enum
{
    DDI_DATA_TYPE_PROLOG,   // Prolog: for Framer's Internal Sync Lock
    DDI_DATA_TYPE_EPILOG,   // Epilog: for flushing Demux's internal buffer
    DDI_DATA_TYPE_USER      // User data
} DDI_DATA_TYPE_T;

typedef struct
{
    UINT32 u4SN;            // serial number
    DDI_DATA_TYPE_T eDataType;
    DMX_MM_DATA_T rMmData;
} DDI_MSG_T;

//// Multi-media


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
///
/// DDI regsiter access commands
///
#define DDI_READ32(off)                 IO_READ32(DDI_BASE, (off))
#define DDI_WRITE32(off, val)           IO_WRITE32(DDI_BASE, (off), (val))
#define DDI_REG32(off)                  IO_READ32(DDI_BASE, (off))

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//
// DDI Emu
//

EXTERN BOOL _DMX_DDI_EMU_Init(UINT8 u1TsIndex);
EXTERN BOOL _DMX_DDI_EMU_Transfer(UINT8 u1Type, UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _DMX_DDI_EMU_SetBuffer(UINT32 u4Addr, UINT32 u4Size);

//
// DDI
//
EXTERN BOOL _DMX_DDI_Init(void);
EXTERN BOOL _DMX_DDI_Set(UINT32 u4Flags, const DMX_DDI_T *prDDI);
EXTERN BOOL _DMX_DDI_Get(UINT8 u1TsIdx, UINT32 u4Flags, DMX_DDI_T *prDDI);
EXTERN BOOL _DMX_DDI_Free(void);

EXTERN void _DMX_DDI_SetPort(DMX_DDI_PORT_T ePort, BOOL fgForce);
EXTERN void _DMX_DDI_SetPortEx(DMX_DDI_PORT_T ePort, UINT8 u1Pidx, BOOL fgForce,  BOOL fgTs);
EXTERN DMX_DDI_PORT_T _DMX_DDI_GetPort(void);
EXTERN UINT8 _DMX_DDI_GetTunerIndex(void);

EXTERN BOOL _DMX_DDI_Notify(void);

EXTERN BOOL _DMX_DDI_FlushBuf(void);
EXTERN UINT32 _DMX_DDI_GetFreeBufSize(UINT8 u1Inst);

EXTERN BOOL _DMX_DDI_SetPacketSize(UINT8 u1TsIdx, UINT8 u1PacketSize);
EXTERN UINT8 _DMX_DDI_GetPacketSize(void);
EXTERN BOOL _DMX_DDI_SetSyncOffset(UINT8 u1Inst, UINT8 u1Offset);
EXTERN BOOL _DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                                UINT32 u4Addr, UINT32 u4Size);

EXTERN BOOL _DMX_DDI_TSMoveData(DMX_MM_DATA_T *prData);

EXTERN BOOL _DMX_DDI_PowerDown(VOID);
EXTERN BOOL _DMX_DDI_TSRequestReset(UINT8 u1Inst);

EXTERN BOOL _DMX_DDI_Unlock(void);  // for auto-reset

EXTERN void _DMX_DDI_HWReset(void);

EXTERN BOOL _DMX_DDI_WaitTransfer(UINT8 u1TsIdx, UINT32 u4BufStart, UINT32 u4BufEnd,
                              UINT32 u4SendAddr, UINT32 u4SendSize);

EXTERN BOOL _DMX_DDI_InitISR(void);
EXTERN void _DMX_DDI_SetDMAInt(BOOL fgEmpty, BOOL fgAlert);
EXTERN void _DMX_DDI_SetDMAAlert_Pointer(UINT32 u4AlertPoint);

EXTERN BOOL _DDI_TsResetDmxBuffers(UINT8 u1Inst);

EXTERN BOOL _DMX_SetTSFilePlayPath(UINT8 u1TsIndex, DMX_TSFMT_T eFmt);
EXTERN void _DMX_DDI_SetDelayByte(UINT8 u1DelayByte);

//
// PVR
//
EXTERN void _DMX_PVRPlay_Notify(void);

EXTERN BOOL _DMX_PVRPlay_Init(void);
EXTERN BOOL _DMX_PVRPlay_Free(void);
EXTERN BOOL _DMX_PVRPlay_Start(void);
EXTERN BOOL _DMX_PVRPlay_Stop(BOOL fgForce);
EXTERN BOOL _DMX_PVRPlay_Pause(DMX_PVRPLAY_PTR_T *prPtr);
EXTERN BOOL _DMX_PVRPlay_Resume(DMX_PVRPLAY_PTR_T *prPtr);
EXTERN VOID _DMX_PVRPlay_Reset(void);
EXTERN DMX_PVR_PLAYSTATE_T _DMX_PVRPlay_GetState(void);
EXTERN BOOL _DMX_PVRPlay_Set(UINT32 u4Flags, const DMX_PVR_PLAY_T* prPVRPlay);
EXTERN BOOL _DMX_PVRPlay_Get(UINT32 u4Flags, DMX_PVR_PLAY_T* prPVRPlay);
EXTERN BOOL _DMX_PVRPlay_UpdateWp(UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _DMX_PVRPlay_FreeBuf(void);
EXTERN BOOL _DMX_PVRPlay_FlushBuf(void);
EXTERN BOOL _DMX_PVRPlay_SingleMove(const DMX_MM_DATA_T* prData);
EXTERN BOOL _DMX_PVRPlay_GetBufPointer(DMX_PVRPLAY_BUFPTR_T *prPtr);
EXTERN BOOL _DMX_PVRPlay_SetBuffer(UINT32 u4BufStart, UINT32 u4BufEnd,
                                   UINT32 u4Wp, UINT32 u4Rp);
EXTERN BOOL _DMX_PVRPlay_RequestReset(UINT8 u1TsIdx);
EXTERN void _DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter);
EXTERN void _DMX_PVRPlay_ResetCounter(void);
EXTERN void _DMX_PVRPlay_SetPort(DMX_PVRPLAY_PORT_T ePort);
EXTERN DMX_PVRPLAY_PORT_T _DMX_PVRPlay_GetPort(void);
EXTERN BOOL _DMX_PVRPlay_SetMoveMode(DMX_TSFMT_T eTSFmt, BOOL fgForce);

EXTERN BOOL _DMX_PVRPlay_130byteEnable(BOOL fgEnable, UINT8 u1PktSize);

EXTERN BOOL _DMX_OpenTimeshift(BOOL fgOpen);
EXTERN BOOL _DMX_IsTimeshiftOpen(void);

//
// Program stream
//
EXTERN BOOL _DMX_PS_SetStream(UINT8 u1Pidx, UINT32 u4Flags, const DMX_PS_T *prPs);
EXTERN BOOL _DMX_PS_GetStream(UINT8 u1Pidx, UINT32 u4Flags, DMX_PS_T *prPs);
EXTERN BOOL _DMX_PS_FreeStream(UINT8 u1Pidx);
EXTERN BOOL _DMX_PS_DetectStreamID(BOOL fgDetect, UINT16 u2Pairs, UINT32 u4Len);
EXTERN BOOL _DMX_PS_SetAudioType(UINT8 u1Pidx, DMX_AUDIO_TYPE_T eAudioType);
EXTERN DMX_AUDIO_TYPE_T _DMX_PS_GetAudioType(void);
EXTERN BOOL _DMX_PS_SetDataFormat(DMX_DATA_FORMAT_T eDataFormat);
EXTERN DMX_DATA_FORMAT_T _DMX_PS_GetDataFormat(void);

#ifdef CC_DMX_PS_CPU
EXTERN BOOL _DMX_PS_CPU_Init(void);
EXTERN BOOL _DMX_PS_CPU_SetStream(UINT8 u1Idx, UINT32 u4Flags, const DMX_PS_T *prPs);
EXTERN BOOL _DMX_PS_CPU_GetStream(UINT8 u1Idx, UINT32 u4Flags, DMX_PS_T *prPs);
EXTERN BOOL _DMX_PS_CPU_FreeStream(UINT8 u1Idx);
EXTERN BOOL _DMX_PS_CPU_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL _DMX_PS_CPU_Reset(UINT8 u1Inst);
EXTERN BOOL _DMX_PS_CPU_SetAudioType(UINT8 u1Idx, DMX_AUDIO_TYPE_T eAudioType);
EXTERN DMX_AUDIO_TYPE_T _DMX_PS_CPU_GetAudioType(UINT8 u1Pidx);
EXTERN BOOL _DMX_PS_CPU_SetDataFormat(UINT8 u1Inst, DMX_DATA_FORMAT_T eDataFormat);
EXTERN DMX_DATA_FORMAT_T _DMX_PS_CPU_GetDataFormat(UINT8 u1Inst);
EXTERN BOOL _DMX_PS_CPU_MoveData(DMX_MM_DATA_T *prData);
EXTERN void _DMX_PS_CPU_SetVideoHeader(UINT8 u1Idx, DMX_PES_MSG_T *prPes);
EXTERN void _DMX_PS_CPU_SetAudioHeader(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes);
EXTERN void _DMX_PS_CPU_GetExtraAudioInfo(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes);
EXTERN BOOL _DMX_PS_MoveData(UINT8 u1Idx, const DMX_PS_MOVEDATA_CMD_T *prData, UINT32 u4TimeOut);
#endif  // CC_DMX_PS_CPU


//
// Elementary stream
//
EXTERN BOOL _DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prPs);
EXTERN BOOL _DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prPs);
EXTERN BOOL _DMX_ES_FreePid(UINT8 u1Pidx);
EXTERN BOOL _DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len);
EXTERN void _DMX_ES_SetMoveIndex(UINT8 u1Idx);
EXTERN UINT8 _DMX_ES_GetMoveIndex(void);
EXTERN BOOL _DMX_ES_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL _DMX_ES_Reset(UINT8 u1Idx);
EXTERN BOOL _DMX_ES_MoveData(UINT8 u1Idx, DMX_MM_DATA_T *prData);

//
// Multi-media playback
//
EXTERN BOOL _DMX_MM_Init(void);
EXTERN BOOL _DMX_MM_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMM);
EXTERN BOOL _DMX_MM_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMM);
EXTERN BOOL _DMX_MM_Free(UINT8 u1Idx);

EXTERN BOOL _DMX_MM_GetSearchStartCode(UINT8 u1Idx);

EXTERN UINT32 _DMX_MM_GetBufEmptySize(UINT8 u1Idx);

EXTERN BOOL _DMX_MM_MoveData(UINT8 u1Pidx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut);
EXTERN void _DMX_MM_SetHeader(UINT8 u1Idx, DMX_PES_MSG_T *prPes);
EXTERN void _DMX_MM_SetAudioHeader(UINT8 u1Idx, DMX_AUDIO_PES_T *prPes);

EXTERN void _DMX_MM_ISRHandler(UINT8 u1Idx);
EXTERN BOOL _DMX_MM_IsFrameHead(void);

EXTERN BOOL _DMX_MM_SetDRMMode(UINT8 u1Idx, DMX_DRM_MODE_T eMode);
EXTERN DMX_DRM_MODE_T _DMX_MM_GetDRMMode(UINT8 u1Idx);
EXTERN BOOL _DMX_MM_SetAES(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_MM_GetAES(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_MM_UnlockBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4Size,
                                 UINT32 u4FrameAddr);
EXTERN BOOL _DMX_MM_GetBuffer(UINT8 u1Idx, UINT8 u1SerialNumber, UINT32 u4FrameAddr,
                    UINT32 u4SkipSize, UINT32 u4CopySize, UCHAR* pu1Dest);
EXTERN BOOL _DMX_MM_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL _DMX_MM_AllocateBuffer(UINT8 u1Idx, UINT32 u4Size);
EXTERN BOOL _DMX_MM_FreeBuffer(UINT8 u1Idx);

EXTERN UINT32 _DMXMMGetData(void);

EXTERN BOOL _DMX_MM_SendAudioChunk(UINT8 u1Pidx, UINT32 u4Wp, UINT32 u4Pts, BOOL fgEOS);
EXTERN UINT8 _DMX_MM_GetEnabledPidIndex(DMX_INPUT_TYPE_T eInputType, DMX_PID_TYPE_T ePidType);

EXTERN BOOL _DMX_MM_TSFlushBuffer(UINT8 u1Idx);
EXTERN BOOL _DMX_MM_ESFlushBuffer(UINT8 u1Idx);

//
// DTCP
//
EXTERN BOOL _DMX_DTCP_SetAES(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_DTCP_MoveData(const DMX_DTCP_DATA_T *prData);
EXTERN BOOL _DMX_DTCP_MoveBlockData(const DMX_DTCP_BLKDATA_T *prBlkData);

EXTERN BOOL _DMX_AES_SetDecryptInfo(UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_AES_MoveData(const DMX_AES_DATA_T *prData);
EXTERN BOOL _DMX_AES_MoveBlockData(const DMX_AES_BLKDATA_T *prBlkData);

EXTERN UINT8 _DMX_AES_AllocEngine(void);
EXTERN BOOL _DMX_AES_FreeEngine(UINT8 u1Idx);
EXTERN BOOL _DMX_AES_SetDecryptInfoEx(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_AES_MoveDataEx(UINT8 u1Idx, const DMX_AES_DATA_T *prData);
EXTERN BOOL _DMX_AES_MoveBlockDataEx(UINT8 u1Idx, const DMX_AES_BLKDATA_T *prBlkData);

EXTERN BOOL _DMX_Marlin_MoveData(UINT8 u1Idx, const DMX_DTCP_DATA_T *prData);
EXTERN BOOL _DMX_Marlin_SetDecryptInfo(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
///
/// ND-DRM
///
#ifdef DMX_SUPPORT_NDDRM
EXTERN BOOL _DMX_MM_SetNDDRM(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_DRM_AES_T *prDRM);
EXTERN BOOL _DMX_NDDRM_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData, UINT32 u4TimeOut);
#endif  // DMX_SUPPORT_NDDRM

///
/// dmx_audio_handler
///
#ifdef CC_DMX_AUDIO_PREBUF
EXTERN BOOL _DMX_InitAudHandler(void);
EXTERN BOOL _DMX_AudHandler_AddPes(DMX_AUDIO_PES_T* prPes);
EXTERN BOOL _DMX_AudHandler_SetActivePidx(UINT8 u1Idx);
EXTERN BOOL _DMX_AudHandler_SetActivePidxEx(UINT8 u1Idx, UINT8* pu1Buf, UINT32 u4Size);
EXTERN BOOL _DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec);
EXTERN BOOL _DMX_AudHandler_Free(UINT8 u1Idx);
EXTERN BOOL _DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts);
EXTERN void _DMX_AudHandler_SetEnable(BOOL fgEnable);
EXTERN BOOL _DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size);
EXTERN BOOL _DMX_AudHandler_UpdateRp(UINT8 u1Idx, UINT32 u4Size);
EXTERN BOOL _DMX_AudHandler_FlushQ(UINT8 u1Idx);
#endif  // CC_DMX_AUDIO_PREBUF

// dmx_multimm_api.c
EXTERN BOOL _DMX_MUL_Init(void);
EXTERN void _DMX_MUL_EnableMultipleInst(BOOL fgEnable);
EXTERN BOOL _DMX_MUL_GetMultipleInstEnabled(void);
EXTERN UINT8 _DMX_MUL_GetAvailableInst(DMX_CONN_TYPE_T eConnType);
EXTERN BOOL _DMX_MUL_FreeInst(UINT8 u1Inst);
EXTERN BOOL _DMX_MUL_SetInstType(UINT8 u1Inst, DMX_INPUT_TYPE_T eInputType);
EXTERN UINT8 _DMX_MUL_GetAvailablePidx(UINT8 u1Inst);
EXTERN BOOL _DMX_MUL_SetSubtitlePidx(UINT8 u1Inst, UINT8 u1Pidx);
EXTERN UINT8 _DMX_MUL_GetSubtitlePidx(UINT8 u1Inst);
EXTERN UINT8 _DMX_MUL_GetTsIdx(UINT8 u1Inst);
// Temporary, for old mw_if
EXTERN BOOL _DMX_MUL_SetPidxInstance(UINT8 u1Inst, UINT8 u1Pidx);
EXTERN BOOL _DMX_MUL_FreePidx(UINT8 u1Pidx);
EXTERN BOOL _DMX_MUL_MoveData(UINT8 u1Inst, DMX_MM_DATA_T *prData);
EXTERN BOOL _DMX_MUL_RequestReset(UINT8 u1Inst);
EXTERN UINT32 _DMX_MUL_GetEmptySize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx);
EXTERN BOOL _DMX_MUL_SetDecoderCallbacks(UINT8 u1Inst, const DMX_DECODER_CALLBACKS_T* prCallbacks);
EXTERN UINT32 _DMX_MUL_GetMinFreeDRAMBufferSize(UINT8 u1Inst, DMX_PID_TYPE_T eType, UINT8 u1Pidx);
EXTERN DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetInstDecoderCallback(UINT8 u1Inst);
EXTERN DMX_DECODER_CALLBACKS_T* _DMX_MUL_GetPidxDecoderCallback(UINT8 u1Pidx);
EXTERN UINT8 _DMX_MUL_GetPidxInstId(UINT8 u1Pidx);
EXTERN DMX_INPUT_TYPE_T _DMX_MUL_GetInstInputType(UINT8 u1Inst);
EXTERN BOOL _DMX_MUL_SetVideoType(UINT8 u1Inst, DMX_VIDEO_TYPE_T eVideoType);
EXTERN DMX_VIDEO_TYPE_T _DMX_MUL_GetVideoType(UINT8 u1Inst);
EXTERN BOOL _DMX_MUL_SetTSMode(UINT8 u1Inst, DMX_TSFMT_T eFmt);

EXTERN BOOL _DMX_DDI_130byteEnable(BOOL fgEnable, UINT8 u1PktSize);

// Emulation
#ifdef CC_DMX_EMULATION
EXTERN void _DDI_EMU_Notify(void);
EXTERN void _PVR_EMU_Notify(void);
#endif // CC_DMX_EMULATION

#endif  // DMX_MM_H

