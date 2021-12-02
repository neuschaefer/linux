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
 * $RCSfile: vdp_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdp_if.h
 *  Video plane interface, including init, enable,
 *  mode selection and region desciption.
 */

#ifndef B2R_IF_H
#define B2R_IF_H

#include "x_typedef.h"
#include "stc_drvif.h"
#include "x_timer.h"
#include "drv_common.h"
#include "drv_dbase.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// Trick Play at 20081001
#define VDP_PLAY_NORMAL 0 ///< Normal Play
#define VDP_PLAY_TRICK 1 ///< Trick Play
#define VDP_PLAY_STEP_FORWARD 2 ///< Step Forward

/**
 *  VDP capture picture information
 */
typedef struct
{
    UINT32 u4AddrY;
    UINT32 u4AddrC;
    UINT32 u4MemHSize;
    UINT32 u4MemVSize;
    UINT32 u4HSize;
    UINT32 u4VSize;
    UINT32 u4RRMode;
    UINT32 u4IsRsIn;
    UINT32 u4SrcSwap;
} VDP_CAPTURE_INTO_T;

/**
 *  VDP position information
 */
typedef struct
{
    UINT64 u8PtsI;
    UINT64 u8Pts;

    UINT64 u8Stc;

    UINT64 u8AudPts;
    UINT64 u8AudOffset;

    UINT64 u8OffsetI;
    UINT64 u8Offset;
    UINT64 u8OffsetDisp;
    INT32  i4TemporalRef;
    UINT16 u2DecodingOrder;
    UINT32 u4Timestap;
} VDP_POS_INTO_T;

/**
 *  VDP AB position information
 */
typedef struct
{
    BOOL fgValid;
    BOOL fgPosition; // use position or pts.
    VDP_POS_INTO_T rA;
    VDP_POS_INTO_T rB;
} VDP_AB_INTO_T;

// This structure must be same with FBM_PIC_EXTRA_INFO_T
/**
 *  VDP PIC EXTRA INFO
 */
typedef struct
{
    BOOL fgValid;

    UINT64 u8PTSI;                                  //  I frame pts
    UINT64 u8AudPTS;
    UINT64 u8AudOffset;
} VDP_PIC_EXTRA_INTO_T;

/** Brief of FRC_PRM_T
 */
typedef struct
{
    // Common
    UCHAR ucVdpId;
    UCHAR ucFbgId;
    UCHAR ucReady;              // if Ready, forceBg = 0;
    UCHAR ucNotSupport;
    UCHAR ucFastChangeEn;

    // Frame Buffer
    UCHAR ucFbId;
    UCHAR ucLastFbId;           // check temporal reference
    UCHAR ucReleaseFbId;
    UCHAR ucPendingFbId;        // Store FB which is pending for display at start
    UCHAR ucReleased;           // for Racing Mode

    UCHAR ucPendingNewFb;       // Pending for New Frame Buffer, FBM (MPEG) can put new frame buffer to VDP, instread of polling by VDP thread
    UCHAR ucNewFbArrive;        // New Frame Buffer Arrive
    UCHAR ucMMWaitSTC;          // MM mode, wait STC while PTS is larger than STC
    UCHAR ucRealChangeFb;       // MM mode, Read change FB, not change to pending FB

    // Output
    UCHAR ucOutputMode;
    UCHAR ucOutFrameRate;
    UCHAR ucProgressiveOut;

    // I 2 I
    UCHAR ucInterlace2Interlace;
    UCHAR ucWrongField;

    // for 420 chroma mode
    UCHAR ucTff;
    UCHAR ucRff;
    UCHAR ucLastTff;
    UCHAR ucLastRff;
    UCHAR ucProgressiveFrame;
    UCHAR ucLastProgressiveFrame;
    UCHAR ucPicCdTp;
    UCHAR ucHwFrameEncode;
    UCHAR ucTffStableCounter;

    // Frc
    UCHAR ucMultipleField;      // multiple field display due to RFF/TFF
    UCHAR ucStopAuto;

    // Display Ns
    UCHAR ucTargetNs;
    UCHAR ucChangeFieldNs;
    UCHAR ucTargerVsyncNs;
    UCHAR ucChangeFieldVsyncNs;
    UCHAR ucCurrentVsyncNs;
    UCHAR ucDisplayFailNs;

    // 32 flag source
    UCHAR ucValid32Seq;
    UCHAR ucPd32SeqStep;
    UCHAR ucRelease32Cnt;

    // Mpeg2 Sequence/Picture Header
    UCHAR ucEarlyNotify;
    UCHAR ucSeqInfoValid;
    UCHAR ucProgressiveSeqFlag;
    UCHAR ucLastAcsFrame;
    UCHAR ucLastAcsAuto;
    UCHAR ucInFrameRate;
    INT32 i4TemporalRef;
    UINT16 u2DecodingOrder;

    // Frame Rate Conversion
    UINT32 u4InStcPeriodBase;
    UINT32 u4InStcPeriod;
    UINT32 u4OutStcPeriod;
    UINT32 u4OutFrameStc;
    UINT32 u4InFrameStc;

    // AV Sync
    UINT32 u4Pts;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4PtsAsyncNs;
    UINT32 u4PtsAvSyncNotAllow;
    INT32 i4PtsDrift;
    UINT32 u4KeepDropB;         // Too much slower than Headend
    UINT32 u4WaitUnmute;
	UINT32 u4B2rSkipFrameNo;

    // Partial PES Handle
    UINT32 u4LastPts;
    UINT32 u4PtsDelta;       // PTS delta (IIR) between two non-zero PTS
    UINT32 u4ZeroPtsNs;	     // handle Zero PTS & PTS Jitter Filter

#ifdef CC_VDP_FRC_DEBUG
    UINT32 u4Stc;
    UINT32 u4PtsBackup;
    INT32 i4PtsStcDelta;
#endif

    // Frame Success Counter
    UINT32 u4FrameSuccess;

    // Frame Repeated Counter
    UINT32 u4FrameRepeated;

#ifdef VDP_DEBUG_TR
    // Debug Temporal Reference
    CHAR szDebugBuf[VDP_DEBUG_BUF_SIZE];
    CHAR szDebugBufTr[VDP_DEBUG_BUF_SIZE];
    UINT32 u4DebugIdx;
#endif

    // PTS Call Back
    UINT32 u4CbPtsEn;
    UINT32 u4CbPts;
    UINT32 u4CbArg;
    UINT32 u4CbCntDown;
    UINT32 u4CbLastPts;

    // LipSync Call Back
    UINT32 u4CbSyncEn;
    UINT32 u4CbSyncThrsd;
    UINT32 u4CbSyncChkFrmCnt;
    UINT32 u4CbSyncFrmCnt;
    UINT32 u4CbSyncDone;

    //File position
    UINT32 u4TickNum;
    UINT64 u8FilePosition;
    UINT64 u8FilePositionI; // closed I frame pos

    // Last Sync Time
    HAL_TIME_T rLastAvSyncTime;

    PARAM_VDP_FB_COUNTER_T* prFbCounter;

    //other info
    BOOL fgReleaseDispQ;

    //b2r 3d info
    BOOL fgB2R3DError;
    UINT32 u4B2R3DSuccessCount;
    UINT32 u4B2R3DErrorCount;
    BOOL fgB2R3DEnable;
    BOOL fgB2RForce2D;
    UINT8 u1B2R3DType;
    UINT8 u1B2RForce2DType;
    UINT32 u4B2R3DWidthOffset;
    UINT32 u4B2R3DHeightOffset;
    BOOL fgDispFail;
    UINT8 u1B2R3DTrickModeType;
    // for multiple instance
    UINT32 u4AttachedSrcId;
    struct _VDP_PRM_T*    pVdpPrm;

    // for AB repeat
    VDP_PIC_EXTRA_INTO_T rExtra;
    VDP_AB_INTO_T rAB;

    //for fastforward info
    BOOL fgFastForward;

    //for tracking mode information
    BOOL fgTrackingMode;
    BOOL fgDummyFrame;
} FRC_PRM_T;

/** Brief of VDP_PRM_T
 */
typedef struct _VDP_PRM_T
{
    UCHAR ucMode;

    // Last Field
    UCHAR ucBottom;
    //BOOL fgPauseMM;
    FRC_PRM_T rPortPrm;
} VDP_PRM_T;

#define B2R_SPEED_TYPE_FORWARD_1X            (1 << 10)
#define B2R_SPEED_TYPE_FORWARD_2X            (2 << 10)
#define B2R_SPEED_TYPE_FORWARD_3X            (3 << 10) // FixMe
#define B2R_SPEED_TYPE_FORWARD_4X            (4 << 10)
#define B2R_SPEED_TYPE_FORWARD_8X            (8 << 10)
#define B2R_SPEED_TYPE_FORWARD_16X           (16 << 10)
#define B2R_SPEED_TYPE_FORWARD_32X           (32 << 10)
#define B2R_SPEED_TYPE_FORWARD_64X           (64 << 10)
#define B2R_SPEED_TYPE_FORWARD_128X          (128 << 10)
#define B2R_SPEED_TYPE_FORWARD_256X          (256 << 10)
#define B2R_SPEED_TYPE_FORWARD_512X          (512 << 10)
#define B2R_SPEED_TYPE_FORWARD_1024X         (1024 << 10)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_2X      (1 << 9)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_3X      (1 << 9) // FixMe
#define B2R_SPEED_TYPE_FORWARD_1_DIV_4X      (1 << 8)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_8X      (1 << 7)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_16X     (1 << 6)
#define B2R_SPEED_TYPE_FORWARD_1_DIV_32X     (1 << 5)
#define B2R_SPEED_TYPE_FORWARD_0_DOT_6X      (6 << 10)   /* Slow Forward 0.6X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_7X      (7<< 10)    /* Slow Forward 0.7X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_8X      (8 << 10)   /* Slow Forward 0.8X */
#define B2R_SPEED_TYPE_FORWARD_0_DOT_9X      (9 << 10)   /* Slow Forward 0.9X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_1X      (11 << 10)  /* Fast Forward 1.1X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_2X      (12 << 10)  /* Fast Forward 1.2X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_3X      (13 << 10)  /* Fast Forward 1.3X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_4X      (14 << 10)  /* Fast Forward 1.4X */
#define B2R_SPEED_TYPE_FORWARD_1_DOT_5X      (15 << 10)  /* Fast Forward 1.5X */
#define B2R_SPEED_TYPE_REWIND_1X
#define B2R_SPEED_TYPE_REWIND_2X
#define B2R_SPEED_TYPE_REWIND_3X
#define B2R_SPEED_TYPE_REWIND_4X
#define B2R_SPEED_TYPE_REWIND_8X
#define B2R_SPEED_TYPE_REWIND_16X
#define B2R_SPEED_TYPE_REWIND_32X
#define B2R_SPEED_TYPE_REWIND_64X
#define B2R_SPEED_TYPE_REWIND_128X
#define B2R_SPEED_TYPE_REWIND_256X
#define B2R_SPEED_TYPE_REWIND_512X
#define B2R_SPEED_TYPE_REWIND_1024X


///-----------------------------------------------------------------------------
/// Public functions
///-----------------------------------------------------------------------------

/**
 * VDP connect to video decoder.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucEsId specify video decoder id.
 * @param ucPort always 0.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_SetInput(UCHAR ucVdpId, UCHAR ucEsId, UCHAR ucPort);

/**
 * VDP query video plane input connection.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucPort always 0.
 * @param pucFbgId video decoder id.
 * @return VDP_SET_ERROR or VDP_SET_OK
 */
EXTERN UINT32 VDP_GetInput(UCHAR ucVdpId, UCHAR ucPort, UCHAR* pucFbgId);

EXTERN UINT32 VDP_GetLockFrameBufferAddr(UCHAR ucVdpId, VDP_CAPTURE_INTO_T* prCapInfo);

EXTERN UINT32 VDP_SetSpeed(UCHAR ucVdpId, STC_SPEED_TYPE_T eSpeed);

EXTERN UINT32 VDP_GetPts(UCHAR ucVdpId);

EXTERN UINT32 VDP_SetStepForward(UINT8 u1B2rId, UCHAR ucSet);

EXTERN UCHAR VDP_GetStepForward(UINT8 u1B2rId);

EXTERN UINT32 VDP_SetSlowForward(UINT8 u1B2rId, UCHAR ucSet);

EXTERN UCHAR VDP_GetSlowForward(UINT8 u1B2rId);

EXTERN UINT32 VDP_SetFBNoTimeout(UINT8 u1B2rId, UCHAR ucSet);
EXTERN UINT32 VDP_GetFrameRate(UCHAR u1B2rId);
EXTERN UINT32 VDP_TrickModeSkipFrameNo(UCHAR u1B2rId);

#ifdef CC_53XX_SWDMX_V2
EXTERN UINT32 VDP_SetTrickMode(UINT8 u1B2rId,UCHAR ucTrick);
EXTERN UCHAR VDP_GetTrickMode(UINT8 u1B2rId);
#else
EXTERN UINT32 VDP_SetTrickMode(UCHAR ucTrick);
EXTERN UCHAR VDP_GetTrickMode(VOID);
#endif
EXTERN void VDP_SetSeek(UCHAR ucVdpId, UCHAR ucSet, UINT32 u4Pts);

EXTERN void VDP_SetABRepeat(UCHAR ucVdpId, UCHAR ucSet);

EXTERN UCHAR VDP_SetThumbnailMode(UCHAR ucVdpId, UCHAR ucSet);

EXTERN void VDP_SetCbAttachedSrcId(UCHAR ucVdpId, UINT32 u4AttachId);

EXTERN BOOL VDP_GetFastForward(UINT8 u1B2rId);

#endif    // B2R_IF_H

