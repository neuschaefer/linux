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
 * $RCSfile: swdmx_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file swdmx_drvif.h
 *  Internal interface of software demux driver
 */


#ifndef SWDMX_DRVIF_H
#define SWDMX_DRVIF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_if.h"
#include "x_mm_common.h"
#include "u_minfo.h"
#include "dmx_mm_if.h"
#include "feeder_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SWDMX_ADD_RANGE 		0
#define SWDMX_DEL_RANGE 		1

#define SWDMX_DATA_Q_SIZE	  	16  // megaa 20100414
#define SWDMX_PLAY_SPEED_1X	    1000
#define SWDMX_PLAY_SPEED_2X	    2000

#if 0  // megaa 20100421
#define SWDMX_FEEDER_TIMEOUT 20000
#else
#define SWDMX_FEEDER_TIMEOUT 1000
#endif

#define SWDMX_INTERNAL_BUF_SIZE 1024
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    SWDMX_SRC_TYPE_UNKNOWN = 0,
    SWDMX_SRC_TYPE_HIGH_SPEED_STORAGE,
    SWDMX_SRC_TYPE_NETWORK_DLNA,
    SWDMX_SRC_TYPE_NETWORK_NETFLIX,
    SWDMX_SRC_TYPE_MAX
}SWDMX_SOURCE_TYPE_T;


typedef enum SWDMX_DELIVER_MEDIA_TYPE
{
    eSWDMX_MEDIA_VIDEO,			// video
    eSWDMX_MEDIA_AUDIO,      		// audio
    eSWDMX_MEDIA_DRM_VIDEO,           // digital right management for video
    eSWDMX_MEDIA_DRM_AUDIO,           // digital right management for audio
    eSWDMX_MEDIA_SUBTITLE,         	// subtitle
    eSWDMX_MEDIA_SKIPPED_AUDIO,
    eSWDMX_MEDIA_SKIPPED,
    eSWDMX_MEDIA_NONE
} SWDMX_DELIVER_MEDIA_TYPE_T;

typedef enum _SWDMX_RANGE_STATE
{
    eSWDMX_RANGE_STATE_NOT_FINISHED,	// the range is not finished
    eSWDMX_RANGE_STATE_FINISHED, 			// the range is finished
    eSWDMX_RANGE_STATE_INVALID,			// the range is invalid
    eSWDMX_RANGE_STATE_SKIP, 			       // the range is skipped
    eSWDMX_RANGE_STATE_REQUEST,                 // request for a new range
    eSWDMX_RANGE_STATE_PENDING                  // the range is finished but need to wait
} SWDMX_RANGE_STATE_T;

typedef enum _SWDMX_HDR_PRS_STATE
{
    eSWDMX_HDR_FED_BUF_NOT_ENOUGH,			// the buffer is not enough
    eSWDMX_HDR_DMX_BUF_NOT_ENOUGH,		       // the buffer is not enough
    eSWDMX_HDR_PRS_FED_MOVE_FAIL,			// the feeder fails to move date from the IO device
    eSWDMX_HDR_PRS_DMX_MOVE_FAIL,			// the demuxer fails to move data from the feeder
    eSWDMX_HDR_PRS_SUCCEED, 					// header parsing succeed
    eSWDMX_HDR_PRS_FAIL, 						// header parsing fail
    eSWDMX_HDR_PRS_EMPTY_RANGE,
    eSWDMX_HDR_UNKNOWN_TYPE					// header parsing fail
} SWDMX_HDR_PRS_STATE_T;

typedef struct SWDMX_RANGE_LIST
{
    BOOL fgIsFull;
    BOOL fgInUse;
    BOOL fgIsSeekTarget;

    UINT32 u4CurSentVidIdx;
    UINT32 u4CurSentAudIdx;

    UINT32 u4CurRangeVidIdx;
    UINT32 u4CurRangeAudIdx;

    UINT32 u4CurPTS;
    UINT32 u4RangeId;

    UINT32 u4ReqPTS;

    SWDMX_RANGE_STATE_T 	eRangeStats;
    MM_RANGE_INFO_T 	*prDmxRangeInfo;

#ifdef CC_SWDMX_RENDER_POSITION
    BOOL fgSetRenderPosition;
    INT32 i4TempRef;
    UINT16 u2DecodingOrder;
    UINT64 u8Offset;
    UINT64 u8OffsetI;
    UINT64 u8StartPts;
#endif

    void	*pvNext;
} SWDMX_RANGE_LIST_T;


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
typedef BOOL (*PFN_SWDMX_INIT)(ENUM_SWDMX_FMT_T eType);

/**
*/
typedef BOOL (*PFN_SWDMX_GET_STREAM_INFO)(ENUM_SWDMX_FMT_T eType,
        UINT32 u4InfoMask,
        UINT64 u8FileSize,
        SWDMX_PROGRAM_INFO_T* pPInfo);

/**
*/
typedef BOOL (*PFN_SWDMX_PLAY)(VOID);

/**
*/
typedef BOOL (*PFN_SWDMX_PAUSE)(VOID);

/**
*/
typedef BOOL (*PFN_SWDMX_STOP_CMD)(VOID);


/**
 To interupt DDI Full-Pull mode.
*/
typedef BOOL (*PFN_SWDMX_ABORT)(VOID);
/**
*/
typedef void (*PFN_SWDMX_SET_RANGE)(SWDMX_RANGE_LIST_T *prDmxRangeInfo,
                                    UINT32 u4VidStrmID,
                                    UINT32 u4AudStrmID,
                                    UINT32 u4SubTitleID);


/**
*/
typedef SWDMX_HDR_PRS_STATE_T (*PFN_SWDMX_HEADER_PARSE)(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

/**
*/
typedef BOOL (*PFN_SWDMX_STOP)(void);

/**
*/
typedef BOOL (*PFN_SWDMX_FLUSH)(void);

typedef BOOL (*PFN_SWDMX_SET_INFO)(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3);

typedef BOOL (*PFN_SWDMX_GET_INFO)(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);

typedef BOOL (*PFN_SWDMX_SET_STREAM_ID)(UINT8 u1Type,UINT32 u4StrmID, UINT32 u4StrmSubID);

typedef BOOL (*PFN_SWDMX_ENABLE_STREAM)(UINT8 u1StrmType, VOID* pfnCbFunc);

typedef BOOL (*PFN_SWDMX_DISABLE_STREAM)(UINT8 u1StrmType);

typedef BOOL (*PFN_SWDMX_READ_BUFFER)(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo);

typedef BOOL (*PFN_SWDMX_AUDIO_CB)(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);


typedef struct {
    ENUM_SWDMX_VDEC_T rVideoType;
    ENUM_SWDMX_ADEC_T rAudioType;
    UINT32 u4SequenceHeader;
    UINT32 u4SequenceHeaderLength;
} SWDMX_DECODER_INFO_T;

typedef struct {
    BOOL	fgSwDmxInit;
    BOOL	fgEnVideoDmx;
    BOOL	fgEnAudioDmx;
    BOOL	fgEnSubTitleDmx;
    BOOL	fgEnPcrDmx;
    BOOL    fgPlayChange;
    BOOL    fgFlush;
    BOOL  fgRcvEOS;
    BOOL  fgDataExhausted;
    BOOL  fgWaitDataTimerStart;

    UCHAR *pucSwdmxInternalBuf;

    INT32   i4PlaySpeed;
    UINT32  u4VidStrmID;
    UINT32  u4AudStrmID;
    UINT32  u4AudSubStrmID;
    UINT32  u4Aud2StrmID;
    UINT32  u4SubTitleStrmID;
    UINT32  u4PcrStrmID;
    UINT32  u4TotalTime;

    UINT64  u8SeekPos;
    UINT64  u8FileSize;
    UINT64  u8FileOffset;

    ENUM_SWDMX_PLAY_STATE_T eSwDmxPrevousStatus;
    ENUM_SWDMX_PLAY_STATE_T eSwDmxStatus;
    ENUM_SWDMX_FMT_T eCurFMT;

    DMX_MM_T rDmxVidMMInfo;
    DMX_MM_T rDmxAudMMInfo;

    DMX_MM_DATA_T rDmxMMData;

    HANDLE_T hRangeSema; // currently for time-shfit lock the range info
    HANDLE_T hWaitDataTimer;

    void*           pvSwdmxNfyTag;
    PFN_SWDMX_NFY_FCT pfSwdmxNfyFct;

    PFN_SWDMX_INIT	pfnSwdmxInit;
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
    x_mm_range_nfy_fct pfnSwdmxRangeCb;
} SWDMX_INFO_T;

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

//--------------------
// swdmx_drvif.c

extern VOID _SWDMX_LockFlushMutex(VOID);
extern VOID _SWDMX_UnlockFlushMutex(VOID);
extern void _SWDMX_LockFlushSema(VOID);
extern void _SWDMX_UnlockFlushSema(VOID);

extern INT32 _SWDMX_ReceiveFeederAck(VOID *pvData);
extern INT32 _SWDMX_ReceiveFeederAckNoWait(VOID *pvData);
extern VOID _SWDMX_FlushReqMsgQ(VOID);
extern ENUM_SWDMX_PLAY_STATE_T _SWDMX_QueryStatus(VOID);
extern BOOL _SWDMX_SetRangeCb(VOID* pfnCbFunc);

extern BOOL _SWDMX_MpgTsFillBufferStart(VOID);
extern BOOL _SWDMX_MpgTsFillBufferEnd(VOID);
extern VOID _SWDMX_FeederCallBack(MM_NOTIFY_INFO_T *prNfyInfo, UINT32 u4Param);


extern BOOL _SWDMX_Init(VOID);

extern BOOL _SWDMX_GetStreamInfo(ENUM_SWDMX_FMT_T eType,
                                 UINT32 u4InfoMask,
                                 UINT64 u8FileSize,
                                 SWDMX_PROGRAM_INFO_T* prPInfo);

extern BOOL _SWDMX_Play(VOID);

extern BOOL _SWDMX_Pause(VOID);

extern BOOL _SWDMX_Stop(VOID);

extern BOOL _SWDMX_Flush(VOID);
extern BOOL _SWDMX_DoFlush(VOID);

extern BOOL _SWDMX_GetInfo(UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);

extern BOOL _SWDMX_SetInfo(UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3);

extern BOOL _SWDMX_AddRange(MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID);

extern BOOL _SWDMX_DelRange(MM_RANGE_INFO_T *prDmxRange, UINT32 u4TagID, BOOL fgDelAllRange);

extern BOOL _SWDMX_SetVideoStreamID(UINT32 u4VidStrmID);
extern BOOL _SWDMX_SetAudioStreamID(UINT32 u4AudStrmID, UINT32 u4AudStrmSubID);
extern BOOL _SWDMX_SetAudio2StreamID(UINT32 u4AudStrmID);
extern BOOL _SWDMX_SetSubTitleStreamID(UINT32 u4SubTitleStrmID);
extern BOOL _SWDMX_SetPcrStreamID(UINT32 u4PcrStrmID);

extern BOOL _SWDMX_SetContainerType(ENUM_SWDMX_FMT_T eContainerType);
extern BOOL _SWDMX_SetSpeed(INT32 i4Speed);
extern BOOL _SWDMX_SeekTime(UINT32 u4SeekTime, UINT64* pu8FilePos);
extern BOOL _SWDMX_SeekPos(UINT64 u8SeekPos, UINT64 u8SeekPosI);
extern BOOL _SWDMX_SeekPercent(UINT32 u4SeekPercent, UINT64* pu8FilePos);
extern BOOL _SWDMX_RenderFromPos(VOID *prPosition);

extern BOOL _SWDMX_EnableStrm(UINT8 u1StrmType);
extern BOOL _SWDMX_DisableStrm(UINT8 u1Strmtype);

extern VOID _SWDMX_QueryRangeInfo(SWDMX_RANGE_INFO_T* prRangeInfo);

extern INT32 _SWDMX_SetDmxNfy(const SWDMX_NFY_INFO_T* prSwdmxNfyInfo);

extern BOOL _SWDMX_ReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo);
extern BOOL _SWDMX_ReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, SWDMX_READ_INFO_T *prReadInfo);
extern BOOL _SWDMX_AudioNotify(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);
extern UINT16 _SWDMX_GetFeederQueueSize(VOID);
extern BOOL _SWDMX_PcrCallBack(UINT32 u4PcrBase);
extern UINT16 _SWDMX_GetNumOfCmd(VOID);
extern VOID _SWDMX_LockRangeSema(VOID);
extern VOID _SWDMX_UnlockRangeSema(VOID);
extern UINT64 _SWDMX_Div6464(UINT64 u8Dividend, UINT64 u8Divider, UINT64 *pu8Remainder);


#ifdef SWDMX_STATISTICS
EXTERN void _SWDMX_ResetTime(void);
EXTERN void _SWDMX_PrintTime(void);
#endif

#ifdef CC_SWDMX_RENDER_PTS
EXTERN void _SWDMX_SetRenderPts(UINT32 u4RenderPts);
#endif

#if 0//def CC_SWDMX_RENDER_POSITION
EXTERN VOID _SWDMX_GetRenderPosition(INT32 *pi4TempRef, UINT16 *pu2DecodingOrder, UINT64 *pu8Offset, UINT64 *pu8OffsetI);
EXTERN VOID _SWDMX_SetRenderPosition(INT32 i4TempRef, UINT16 u2DecodingOrder, UINT64 u8Offset, UINT64 u8OffsetI);
#endif
#endif
