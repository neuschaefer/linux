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
EXTERN BOOL _DMX_DDI_Get(UINT32 u4Flags, DMX_DDI_T *prDDI);
EXTERN BOOL _DMX_DDI_Free(void);

EXTERN void _DMX_DDI_SetPort(DMX_DDI_PORT_T ePort);
EXTERN DMX_DDI_PORT_T _DMX_DDI_GetPort(void);

EXTERN BOOL _DMX_DDI_Notify(void);

EXTERN BOOL _DMX_DDI_FlushBuf(void);
EXTERN BOOL _DMX_DDI_FillBuf(UINT32 u4DataAddr, UINT32 u4DataSize, UINT32 *pu4FilledSize);
EXTERN UINT32 _DMX_DDI_GetFreeBufSize(void);

EXTERN BOOL _DMX_DDI_StreamStart(void);
EXTERN BOOL _DMX_DDI_StreamStop(void);
EXTERN DMX_DDI_STATE_T _DMX_DDI_GetStreamState(void);
EXTERN BOOL _DMX_DDI_SetPacketSize(UINT8 u1PacketSize);
EXTERN UINT8 _DMX_DDI_GetPacketSize(void);
EXTERN BOOL _DMX_DDI_SetSyncOffset(UINT8 u1Offset);
EXTERN BOOL _DMX_DDI_SingleMove(UINT32 u4BufStart, UINT32 u4BufEnd,
                                UINT32 u4Addr, UINT32 u4Size);
EXTERN BOOL _DMX_DDI_MoveData(const DMX_MM_DATA_T *prData);
EXTERN BOOL _DMX_DDI_PowerDown(VOID);
EXTERN BOOL _DMX_DDI_RequestReset(VOID);
EXTERN BOOL _DMX_DDI_Unlock(void);  // for auto-reset

EXTERN BOOL _DMX_DDI_InitISR(void);
EXTERN void _DMX_DDI_SetDMAInt(BOOL fgEmpty, BOOL fgAlert);
EXTERN BOOL _DDI_ResetDmxBuffers(VOID);


//
// PVR
//
EXTERN void _DMX_PVRPlay_Notify(void);

EXTERN BOOL _DMX_PVRPlay_Init(void);
EXTERN BOOL _DMX_PVRPlay_Free(void);
EXTERN BOOL _DMX_PVRPlay_SetDBMTestPid(void);
EXTERN BOOL _DMX_PVRPlay_FreeDBMTestPid(void);
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
EXTERN BOOL _DMX_PVRPlay_RequestReset(void);
EXTERN BOOL _DMX_PVRPlay_EnableTSOut(BOOL fgEnable);
EXTERN BOOL _DMX_PVRPlay_SetUseDescramble(BOOL fgEnable);
EXTERN BOOL _DMX_PVRPlay_GetUseDescramble(void);

EXTERN void _DMX_PVRPlay_GetCounter(DMX_PVRPLAY_COUNTER_T *prCounter);
EXTERN void _DMX_PVRPlay_ResetCounter(void);

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

//
// Elementary stream
//
EXTERN BOOL _DMX_ES_SetPid(UINT8 u1Pidx, UINT32 u4Flags, const DMX_ES_T *prPs);
EXTERN BOOL _DMX_ES_GetPid(UINT8 u1Pidx, UINT32 u4Flags, DMX_ES_T *prPs);
EXTERN BOOL _DMX_ES_FreePid(UINT8 u1Pidx);
EXTERN BOOL _DMX_ES_SetFileLength(UINT8 u1Pidx, UINT32 u4Len);

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

///
/// dmx_audio_handler
///
#ifdef CC_DMX_AUDIO_PREBUF
EXTERN BOOL _DMX_InitAudHandler(void);
EXTERN BOOL _DMX_AudHandler_AddPes(const DMX_AUDIO_PES_T* prPes);
EXTERN BOOL _DMX_AudHandler_SetActivePidx(UINT8 u1Idx);
EXTERN BOOL _DMX_AudHandler_Set(UINT8 u1Idx, BOOL fgToDec);
EXTERN BOOL _DMX_AudHandler_Free(UINT8 u1Idx);
EXTERN BOOL _DMX_AudHandler_SeekAndUpdate(UINT8 u1Idx, UINT32 u4Pts, UINT32 *pu4Pts);
EXTERN void _DMX_AudHandler_SetEnable(BOOL fgEnable);
EXTERN BOOL _DMX_AudHandler_SetQSize(UINT8 u1Idx, UINT16 u2Size);
#endif  // CC_DMX_AUDIO_PREBUF

#ifdef CC_DMX_PURE_AUDIO
EXTERN BOOL _DMX_PURE_Init(void);
EXTERN BOOL _DMX_PURE_IsPureAudio(UINT8 u1Idx);
EXTERN BOOL _DMX_PURE_Set(UINT8 u1Idx, UINT32 u4Flags, const DMX_MM_T *prMM);
EXTERN BOOL _DMX_PURE_Get(UINT8 u1Idx, UINT32 u4Flags, DMX_MM_T *prMM);
EXTERN BOOL _DMX_PURE_Free(UINT8 u1Idx);
EXTERN UINT32 _DMX_PURE_GetEmptyBufferSize(UINT8 u1Idx);
EXTERN BOOL _DMX_PURE_MoveData(UINT8 u1Idx, const DMX_MM_DATA_T *prData);
EXTERN BOOL _DMX_PURE_FlushBuffer(UINT8 u1Idx);
EXTERN BOOL _DMX_PURE_UpdateReadPointer(UINT8 u1Idx, UINT32 u4Rp, UINT32 u4FrameAddr);
EXTERN void* _DMX_PURE_GetInstanceTag(void);
#endif  // CC_DMX_PURE_AUDIO


#endif  // DMX_MM_H

