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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: swdmx_mp4.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_rm.h
 *  This header file declares exported APIs of SWDMX_RM.
 */

#ifndef SWDMX_RM_H
#define SWDMX_RM_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_drvif.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define COOK_WRITE_AFIFO
#ifdef RM_LOGICAL_STREAM_SUPPORT
#define RMLS
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define RM_FIX_REPEAT_FRAME 32
#define RM_MINIMUM_KEY_NUM 10
#define RM_MAX_REPEAT_FRAME 20
#define RM_MIN_REPEAT_FRAME 10

#define RM_MAX_AUDIO_TRACK 1

#define RM_PACKET_ID_OFFSET 2
#if 1 // david:change RM_PARSER_OFFSET to 13 for object version 1
#define RM_PARSER_OFFSET 13
#else
#define RM_PARSER_OFFSET 12
#endif

#define RM_BUFFER_SIZE (235 * 7 * 1024)
#define RM_MIN_READ_SIZE (235 * 1024)

#define AAC_FAKE_HDR_SIZE 7
#define AAC_ALLOC_SIZE   (256)

#ifdef COOK_WRITE_AFIFO
  #define RA_ALLOC_SIZE    0
#else
  #define RA_ALLOC_SIZE    (25 * 1024)
#endif

#define DEMUX_RM_AV_PERIOD   90000 // 90000 * 0.1 sec

#define ONE_MB 1024 * 1024

#define RM_CHANGE_AUDIO_MODE 0x00000001

/* Real Media Logical Stream Index */
enum
{
    RM_STRM_VDO,
#ifdef RMLS
    RM_STRM_AUD,
#endif
    RM_STRM_MAX
};

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
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

    UINT8 u1EccMask;

    COOK_INFO_T rCookInfo;
    BOOL fgCookPtsPending;
    BOOL fgCookSBPending;
    UINT32 u4CookSBPendingCnt;
    UINT32 u4CookSBPts;
} SWDMX_RM_INFO_T;


enum {
  RMPRS_PACKET = 0x13,                            // 13
  RMPRS_FRMHDR,                                   // 14
  RMPRS_FRM_ENTIRE_SIZE,                          // 15
  RMPRS_FRM_PARTIAL_OFST,                         // 16
  RMPRS_FRM_SEQ_NUM,                              // 17
  RMPRS_FRM_ONE_OF_MLTI_SIZE,                     // 18
  RMPRS_FRM_NEXT_MULTIPLE,                        // 19
  RMPRS_FRM_TIMESTAMP,                            // 1A
  RMPRS_FRAME                                     // 1B
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


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_RmInit(ENUM_SWDMX_FMT_T eType);

/**
*/
void _SWDMX_RmSetRange(SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID);


BOOL _SWDMX_RmSetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3);

BOOL _SWDMX_RmGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);

BOOL _SWDMX_RmSetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID);

/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_RmHdrPrs(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

BOOL _SWDMX_RmEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc);

BOOL _SWDMX_RmDisableStrm(UINT8 u1StrmType);

BOOL _SWDMX_RmAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);


/**
*/
BOOL _SWDMX_RmStop(void);



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif  // SWDMX_RM_H


