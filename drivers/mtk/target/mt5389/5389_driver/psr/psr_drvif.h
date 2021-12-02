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
 * Description:
 *
 *         Parser driver exported interface
 *
 *---------------------------------------------------------------------------*/

#ifndef PSR_DRVIF_H
#define PSR_DRVIF_H

#include "x_typedef.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_os.h"

LINT_EXT_HEADER_END

//===========================================================================
//
// Parser interfaces
//
//===========================================================================

//---------------------------------------------------------------------
// Parameters

//---------------------------------------------------------------------
// Constant definitions

#define MAX_CHANNEL_NUM                 8
#define MAX_VLD_NUM                     2
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#define MAX_DSP_NUM                     4
#else
#define MAX_DSP_NUM                     2
#endif
#define MAX_PIC_HEADERS                 16
#define MAX_PIC_HEADER_COUNT            256

/*
#define HD_VIDEO_BUF_SIZE               1230000             // 1.17MB
#define SD_VIDEO_BUF_SIZE               230000              // 224KB
*/

#ifndef HD_VIDEO_BUF_SIZE
#define HD_VIDEO_BUF_SIZE               0x300000            // 3MB
#endif

#define SD_VIDEO_BUF_SIZE               0x60000             // 384KB

#define MAX_HD_VIDEO_NUM                2
#define MAX_SD_VIDEO_NUM                8

#define AUDIO_BUF_BOUNDARY              0x1000000           // 16MB

#define PES_HEADER_BUF_SIZE             1024
#define HEADER_BUF_ALIGNMENT            256
#define FIFO_ALIGNMENT                  256

#define FIFO_ALMOST_FULL_THRESHOLD      80                  // In percentage

// Soft parser parameters
#define PSR_SOFT_FIFO_ALIGNMENT         256
#define PSR_MAX_PDMA_SIZE               0xf7ffff            // 15.5M bytes


//---------------------------------------------------------------------
// Type definitions

// Stream type
typedef enum
{
    PSR_STREAM_UNKNOWN,
    PSR_STREAM_VIDEO,
    PSR_STREAM_AUDIO
} PSR_STREAM_T;

typedef enum
{
    PSR_PIC_UNKNOWN,
    PSR_PIC_I,
    PSR_PIC_P,
    PSR_PIC_B,
    PSR_PIC_SEQ_START,
    PSR_PIC_SEQ_END,
    PSR_PIC_GOP
} PSR_PICTURE_TYPE_T;

typedef struct
{
    UINT64              u8Pts;
    UINT64              u8Dts;
    PSR_PICTURE_TYPE_T  eType;
    UINT32              u4Addr;
//    UINT32              u4Postpone;
} PIC_HEADER_T;

typedef struct
{
//    UINT64              u8Pts;
//    UINT64              u8Dts;
    UINT32              u4FifoStart;
    UINT32              u4FifoEnd;
    UINT32              u4HeaderNum;
    PIC_HEADER_T        arHeader[MAX_PIC_HEADERS];
} PSR_VIDEO_PES_T;

typedef struct
{
    UINT64              u8Pts;
    UINT64              u8Dts;
    UINT32              u4PayloadAddr;
    UINT16              u2PayloadSize;
} PSR_AUDIO_PES_T;

typedef enum
{
    CS_FREE,
    CS_RUNNING,
    CS_STOP
} CHANNEL_STATE_T;

typedef struct
{
    CHANNEL_STATE_T     eState;
    PSR_STREAM_T        eStreamType;
    UINT32              u4FifoSize;
    UINT32              u4AlmostFullThreshold;
    void*               pvFifoAddr;
    void*               pvHeader;
    UINT8               u1DeviceId;
} CHANNEL_T;

// Parser events
typedef enum
{   // Event type                       // pvData
    //
    PSR_EVENT_UNKNOWN,                  // NULL
    PSR_EVENT_PES_PACKET,               // PSR_VIDEO_PES_T/PES_AUDIO_PES_T
    PSR_EVENT_FIFO_FULL                 // NULL
} PSR_EVENT_T;

// Parser event handler
// Note: In the callback handler, it's not allowed to copy the pointer pvData
// for later use. Once the handler returned, the pointer pvData is invalid
// immediately.
typedef void (*PFN_PARSER_CALLBACK)(PSR_EVENT_T eEvent, UINT8 u1ChannelId,
    UINT8 u1DeviceId, PSR_STREAM_T eStreamType, void* pvData);

typedef struct
{
    CHANNEL_T           arChannel[MAX_CHANNEL_NUM];
    HANDLE_T            hSemLock;
    UINT32              au4AudioBufAddr[MAX_DSP_NUM];
    UINT32              au4AudioBufSize[MAX_DSP_NUM];
    PFN_PARSER_CALLBACK pfnHandler;
 } PARSER_T;

typedef struct
{
    CHANNEL_STATE_T     eState;
    PSR_STREAM_T        eStreamType;
    UINT32              u4FifoStart;
    UINT32              u4FifoEnd;
    UINT32              u4Wp;
    UINT32              u4Rp;
    UINT32              u4DataSize;
    UINT8               u1DeviceId;
    BOOL                fgFull;
} CHANNEL_INFO_T;

//---------------------------------------------------------------------
// Exported functions

/*
extern PARSER_T* GetParserObject(void);

extern void ReleaseParserObject(PARSER_T* prParser);
*/

 //---------------------------------------------------------------------
// Init and exit

extern BOOL PSR_Init(const UINT32 au4AudioBufAddr[MAX_DSP_NUM],
    const UINT32 au4AudioBufSize[MAX_DSP_NUM], PFN_PARSER_CALLBACK pfnHandler);

extern BOOL PSR_Exit(void);


//---------------------------------------------------------------------
// Operations

extern BOOL PSR_Reset(void);

extern BOOL PSR_Start(void);

extern BOOL PSR_Stop(void);

extern BOOL PSR_Flush(void);


//---------------------------------------------------------------------
// Channel operations

extern BOOL PSR_SetChannel(UINT8 u1Channel, PSR_STREAM_T eStreamType,
    UINT8 u1DeviceId, BOOL fgIsHD);

extern BOOL PSR_FreeChannel(UINT8 u1Channel);

extern BOOL PSR_SemiFreeChannel(UINT8 u1Channel);

extern BOOL PSR_IsChannelActive(UINT8 u1Channel);

extern BOOL PSR_IsChannelFree(UINT8 u1Channel);

extern BOOL PSR_StartChannel(UINT8 u1Channel);

extern BOOL PSR_StopChannel(UINT8 u1Channel);

extern BOOL PSR_ResetChannel(UINT8 u1Channel);

extern PSR_STREAM_T PSR_GetChannelType(UINT8 u1Channel);

extern UINT8 PSR_GetDeviceId(UINT8 u1Channel);

extern BOOL PSR_GetChannelInfo(UINT8 u1Channel, CHANNEL_INFO_T* prInfo);

extern BOOL PSR_IsChannelAlmostFull(UINT8 u1Channel);


//---------------------------------------------------------------------
// Misc

extern PFN_PARSER_CALLBACK PSR_GetHandler(void);

extern BOOL PSR_SetDma(BOOL fgEnableDma);

extern BOOL PSR_SetMessagePeriod(UINT32 u4Period);

extern BOOL PSR_IsInterruptInhibited(void);

extern INT32 PSR_Diag(void);

extern void PSR_PowerControl(BOOL fgOn);

// Update VLD read pointer
extern BOOL PSR_UpdateReadPointer(UINT8 u1Channel, UINT8 u1DeviceId,
    PSR_STREAM_T eStreamType, UINT32 u4ReadPointer, UINT32 u4PicAddr);

// VLD switch channel
extern BOOL PSR_VldSwitchChannel(UINT8 u1DeviceId, UINT8 u1Channel);


//===========================================================================
// Soft parser interface

extern BOOL PSR_SoftInit(void);

extern BOOL PSR_SoftReset(void);

extern BOOL PSR_SoftTransfer(PSR_STREAM_T eStreamType, UINT8 u1DeviceId,
    UINT32 u4SrcAddr, UINT32 u4Size,
    UINT32 u4DstAddr, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd,
    BOOL fgPollMode);
extern BOOL PSR_SoftSetAudioFIFO(UINT8 ucDecId, UINT32 u4DstFifoStart,
    UINT32 u4DstFifoEnd);

extern BOOL PSR_RiscSetAudioWp(UINT8 u1DeviceId, UINT32 u4WritePointer);

extern BOOL PSR_RiscSetAudFIFO(UINT8 u1DeviceId, UINT32 u4DstFifoStart,
	UINT32 u4DstFifoEnd);
extern BOOL PSR_DmaTransfer(UINT32 u4SrcAddr, UINT32 u4Size, UINT32 u4DstAddr);

extern UINT32 PSR_SoftGetAudioWp(void);

extern UINT32 PSR_SoftGetAudioRp(void);

#endif  // PSR_DRVIF_H

