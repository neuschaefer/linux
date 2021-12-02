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
 * $RCSfile: mtvdo.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 

/** @file mtstc.h
 *  This header file declares exported APIs of Video (VDO) module.
 */

#ifndef MT_B2R_H
#define MT_B2R_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#include "mtstc.h"
#include "mtvdec.h"
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MTB2R_DEVICE_NODE              "/dev/b2r"


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#define MTB2R_SEEK_STEP_OK              0
#define MTB2R_SEEK_STEP_NO_DATA         1

#define MTB2R_B2R_NS                    2
#define MTB2R_B2R_1                     0
#define MTB2R_B2R_2                     1

#define MTB2R_VDP_1                     0    	///< VDP1
#define MTB2R_VDP_2                     1    	///< VDP2
#define MTB2R_VDP_3                     2    	///< OSD1, obsolete
#define MTB2R_VDP_4                     3    	///< OSD2, obsolete
#define MTB2R_VDP_5                     4    	///< OSD3, obsolete

#define MTB2R_PLAY_NORMAL               0       ///< Normal Play
#define MTB2R_PLAY_TRICK                1       ///< Trick Play
#define MTB2R_PLAY_STEP_FORWARD         2       ///< Step Forward

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

typedef enum
{
    MTB2R_COND_ERROR = -1,
    MTB2R_COND_EOS,
    MTB2R_COND_SYNC_POINT,
    MTB2R_COND_RANGE_AB_DONE,
}  MTB2R_COND_T;

typedef enum
{
    MTB2R_CM_420 = 0,
    MTB2R_CM_422,
    MTB2R_CM_444,
    MTB2R_CM_ARGB8888,
    MTB2R_CM_MAX
}MTB2R_DISPLAY_FORMAT_T;


typedef enum
{
    MTB2R_BLOCK1_MODE = 0,
    MTB2R_BLOCK4_MODE,
    MTB2R_RASTER_MODE,
    MTB2R_ADDR_MODE_MAX
}MTB2R_ADDR_MODE_T;

typedef enum
{
    MTB2R_FBG_DTV_MODE =0,
    MTB2R_FBG_MM_MODE,
    MTB2R_FBG_MODE_NS
}MTB2R_FBG_MODE_T;

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
} MTB2R_POS_INFO_T;

typedef struct
{
    UCHAR ucHours;
    UCHAR ucMinutes;
    UCHAR ucSeconds;
    UCHAR ucPictures;
    UINT32 u4Pts;
    UINT32 u4TotlaTimeOffset;
    UINT32 u4TickNum;
    UINT64 u8Offset;     // file offset
    UINT64 u8OffsetDisp; // file offset for display

} MTB2R_PIC_INFO_T;

typedef struct
{
    BOOL fgValid;
    BOOL fgPosition; // use position or pts.
    MTB2R_POS_INFO_T rA;
    MTB2R_POS_INFO_T rB;
} MTB2R_AB_INFO_T;

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
} MTB2R_CAPTURE_INFO_T;

typedef struct
{
    UINT8 u1FbId;
    UINT32 u4YAddr;
    UINT32 u4CAddr;
    UINT32 u4YAddrOffset;
    UINT32 u4CAddrOffset;
    UINT32 u4Pts;
} MTB2R_PIC_HDR_T;

typedef struct
{
    UINT32 u4FrameWidth;
    UINT32 u4FrameHeight;
    UINT32 u4LineSize;
    BOOL fgProgressiveSeq;
    MTVDEC_FRAME_RATE_T eFrameRate;
    MTB2R_ADDR_MODE_T eAddrMode; 
} MTB2R_SEQ_HDR_T;

typedef struct
{
    UINT32 u4VdpId;
} MTB2R_CHG_FRAME_MSG;

EXTERN MT_RESULT_T MTB2R_GetPts(UCHAR ucVdpId,UINT32* pu4PtsValue);
EXTERN MT_RESULT_T MTB2R_GetStepForward(UCHAR ucVdpId,UINT8* pu1StepFroward);
EXTERN MT_RESULT_T MTB2R_GetPosInfo(UCHAR ucVdpId,MTB2R_POS_INFO_T* prPosInfo);
EXTERN MT_RESULT_T MTB2R_GetPicInfo(UCHAR ucVdpId,MTB2R_PIC_INFO_T* prPicInfo);      
EXTERN MT_RESULT_T MTB2R_GetPlayMode(UCHAR ucVdpId,UINT8* pu1PlayMode);  
EXTERN MT_RESULT_T MTB2R_GetSlowForward(UCHAR ucVdpId,UINT8* pu1SlowForward);
EXTERN MT_RESULT_T MTB2R_SetABInfo(UCHAR ucVdpId,MTB2R_AB_INFO_T* prABInfo);
EXTERN MT_RESULT_T MTB2R_SetThumbnailMode(UCHAR ucVdpId,BOOL fgThumbnailMode);
EXTERN MT_RESULT_T MTB2R_SetSeek(UCHAR ucVdpId,BOOL fgSet,UINT32 u4Pts);
EXTERN MT_RESULT_T MTB2R_SetPauseMM(UCHAR ucVdpId,BOOL fgPause);        
EXTERN MT_RESULT_T MTB2R_SetStepForward(UCHAR ucVdpId,UINT8 u1StepForward);
EXTERN MT_RESULT_T MTB2R_SetFBNoTimeout(UCHAR ucVdpId,BOOL fgFBNoTimeout);
EXTERN MT_RESULT_T MTB2R_SetSpeed(UCHAR ucVdpId,MTSTC_SPEED_TYPE_T eSpeed);
EXTERN MT_RESULT_T MTB2R_SetTrickMode(UCHAR ucVdpId,BOOL fgTrickMode);
EXTERN MT_RESULT_T MTB2R_SetInput(UCHAR ucVdpId,UCHAR ucEsId,UCHAR ucPort);
EXTERN MT_RESULT_T MTB2R_SetSlowForward(UCHAR ucVdpId,BOOL fgSlowForward);
EXTERN MT_RESULT_T MTB2R_SetCbAttachedSrcId(UCHAR ucVdpId,UINT8 u1SrcId);
EXTERN MT_RESULT_T MTB2R_GetLockFrameBufferAddr(UCHAR ucVdpId,MTB2R_CAPTURE_INFO_T* prCaptureInfo);
EXTERN MT_RESULT_T MTB2R_SelectGroupType(UINT8* pu1FbgId,UINT8 u1EsId,UINT32 u4FrameWidth,UINT32 u4FrameHeight,MTB2R_DISPLAY_FORMAT_T eFrameFormat);
EXTERN MT_RESULT_T MTB2R_SetSeqHdr(UINT8 u1FbgId,MTB2R_SEQ_HDR_T* prFrameHdrInfo);
EXTERN MT_RESULT_T MTB2R_GetEmptyFrameBuffer(UINT8 u1FbgId,MTB2R_PIC_HDR_T* prFrameInfo);
EXTERN MT_RESULT_T MTB2R_SetDisplayFrame(UINT8 u1FbgId,MTB2R_PIC_HDR_T* prFrameInfo);
EXTERN MT_RESULT_T MTB2R_ReleaseFbg(UINT8 u1FbgId,UINT8 u1EsId);
EXTERN MT_RESULT_T MTB2R_FreeFrameBuffer(UINT8 u1FbgId,MTB2R_PIC_HDR_T* prFrameInfo);
EXTERN MT_RESULT_T MTB2R_GetDispDrvStable(UCHAR ucVdpId,UINT8* pu1DispStable);
#ifdef __cplusplus
}
#endif

#endif //MT_STC_H
