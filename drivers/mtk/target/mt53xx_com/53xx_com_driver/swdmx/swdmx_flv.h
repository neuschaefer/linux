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
 * $RCSfile: swdmx_flv.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_flv.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef SWDMX_FLV_H
#define SWDMX_FLV_H



//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_drvif.h"
#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
//#include "x_os.h"
//#include "x_assert.h"
//#include "x_typedef.h"
//LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define FLV_MINIMUM_KEY_NUM 10
#define FLV_MAX_ERRORRCV_TIME 8
#define DEMUX_FLV_AV_PERIOD 90000
#define DEMUX_FLV_PTS_BASE 1000
#define DEMXU_FLV_GET_PTS(val) ((val*90))
#define FLV_DEFAULT_TAT_SIZE (24*1024)
#define FLV_ERROR_RECOVERY_LEN (4*1024)

#ifdef LINUX_TURNKEY_SOLUTION
#define FLV_BUFFER_SIZE (64*4*2048)
#define FLV_MIN_READ_SIZE (64*2048)
#else
#define FLV_BUFFER_SIZE (235*5*1024)
#define FLV_MIN_READ_SIZE (235*1024)
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

typedef enum
{
   eFLV_TAG_TYPE_AUD=8,
   eFLV_TAG_TYPE_VID=9,
   eFLV_TAG_TYPE_DATA=18
}FLV_TAG_TYPE;

typedef enum
{
  FLV_SEEK_RET_FIND=0,
  FLV_SEEK_RET_FILEEND,
  FLV_SEEK_RET_PLAYABORT,
  FLV_SEEK_RET_FILEERR
}FLV_SEEK_RET;

typedef enum
{
  eFLV_SEEK_MODE_BACKWARD_BEFORE_TIME,
  eFLV_SEEK_MODE_NEXT_I,
  eFLV_SEEK_MODE_PREVIOUS_I,
  eFLV_SEEK_MODE_BACKWARD_AFTER_TIME,
  eFLV_SEEK_MODE_FORWARD_TO_TIME,
  eFLV_SEEK_MODE_BACKWARD_TIME,
  eFLV_SEEK_MODE_NEXT_AUD
}FLV_SEEK_MODE;

typedef enum
{
 eFLV_SEEK_TYPE_TIME,
 eFLV_SEEK_TYPE_TRICK
}FLV_SEEK_TYPE;

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
     BOOL fgMp3Abr;
     BOOL fgH264SendSeqHeader;
     BOOL fgAacSendHeader;
     BOOL fgStartTrick;
     BOOL fgNeedSetAudStartPts;
     BOOL fgErrorConvery;
     BOOL fgRecoveredFromErr;
	 BOOL fgPlayAbort;
     UINT8 uAudChannelNs;
	 // Feeder

	 INT32 i4ReqReadTimes;
	 UINT32 u4LeftBytesInFeeder;
	 UINT32 u4CurDmxFeederRPtr;
	 UINT32 u4FeederReqID;
	 UINT32 u4FeederReqReceiveID;
     UINT32 u4FrameAddrToVdec;
     UINT32 u4ValidSeekPos;
     //flvparse
	 UINT8 uTagHeaderBrokenIndex;
	 UINT8 uLastTagType;
	 UINT32 u4LastPts;
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
	 UINT16 u2SkipVidCount;	
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
     
	 // Range Information
	 FEEDER_BUF_INFO_T rFeederInfo;
	 SWDMX_RANGE_LIST_T *prRangeList;
	 MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
}SWDMX_FLV_INFO_T;

BOOL _SWDMX_FlvAbort(VOID);

BOOL _SWDMX_FlvInit(ENUM_SWDMX_FMT_T eType);

/**
*/
void _SWDMX_FlvSetRange(
    SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID);

BOOL _SWDMX_FlvSetDecoderInfo(
    UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3);

BOOL _SWDMX_FlvGetInfo(
    UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);

SWDMX_HDR_PRS_STATE_T _SWDMX_FlvHdrParse(
    SWDMX_RANGE_LIST_T* prRangeList,
    INT32 i4PlaySpeed,
    UINT64 u8SeekPos,
    BOOL fgSpeedChange);

BOOL _SWDMX_FlvEnableStrm(
    UINT8 u1StrmType, 
    VOID* pfnCbFunc);

BOOL _SWDMX_FlvDisableStrm(UINT8 u1StrmType);
BOOL _SWDMX_FlvSetStrmID(UINT8 u1HdrType,UINT32 u4StrmID, UINT32 u4StrmSubID);


BOOL _SWDMX_FlvAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

BOOL _SWDMX_FlvStop(VOID);



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_FLW_H
