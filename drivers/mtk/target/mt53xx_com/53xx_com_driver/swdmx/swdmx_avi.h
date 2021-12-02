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
 * $RCSfile: swdmx_avi.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_flw.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef SWDMX_AVI_H
#define SWDMX_AVI_H


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
// Constant definitions
//-----------------------------------------------------------------------------
#define AVI_CHUNK_ID_OFFSET 4
#define AVI_PARSER_OFFSET 8
#define AVI_DRM_SIZE 10

#define AVI_DRM_CHUNK_SIZE_HT 18
#define AVI_DRM_AES_SIZE_128 128 // DivX AES 128 bits 
#define AVI_DRM_MAX_NUM_KEY 128

#define AVI_PREBUFFER_SIZE (8 * 1024 * 1024)
#define AVI_BUFFER_SIZE (235 * 7 * 1024)
#define MIN_READ_SIZE (512 * 1024)

#define AAC_FAKE_HDR_SIZE 7

#define DEMUX_AVI_AV_PERIOD   90000 // 90000 * 0.1 sec
#define WAIT_DMX_DELIVER_TIME   500

#define AVI_CHUNK_ID_JUNK   0x4B4E554A // "JUNK"
#define AVI_IDX_CHUNK_ID     0x00007869 // "ixxx"
#define AVI_UNCOMPRE_DATA 0x62640000 // "xxdb"
#define AVI_VID_COMPRE_DATA 0x63640000 // "xxdc"
#define AVI_VIDEO_UNCOMPRE_FCC 0x6264FFFF // "xxdc"
#define AVI_VIDEO_COMPRE_FCC 0x6364FFFF // "xxdc"
#define AVI_AUDIO_FCC 0X6277FFFF // "xxwb"
#define AVI_DRM_VID_CHUNK_ID    0x64640000 // "00dd"
#define AVI_SUBTITLE_CHUNK_ID    0x62730000 // "00sb"
#define AVI_KEY_TABLE_THRESHOLD (3)
#define AVI_DIVX_MAX_AUD_TRACK 8
#define AVI_MINIMUM_KEY_NUM 10
#define AVI_FIX_REPEAT_FRAME 32
#define AVI_MAX_REPEAT_FRAME 5
//#define AVI_MIN_REPEAT_FRAME 10

#define AVI_CHANGE_AUDIO_MODE 0x00000001
#define AVI_CHANGE_SUBTITLE_MODE 0x00000002

#define AVI_SUBTITLE_BUFSIZE 64 * 1024

#define ONE_MB 1024 * 1024

#define AVI_AUD_PREBUF_IDX 7
#define AVI_AUD_UNKNOWN_IDX 255

#define MPEGA_LAYER_3 0x01
#define MPEGA_LAYER_2 0x02
#define MPEGA_LAYER_1 0x03

#define MPEGA_MPEG_1  0x01
#define MPEGA_MPEG_2  0x02
#define MPEGA_MPEG_3  0x03

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct {
    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnSubTitle;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;
    BOOL fgEnSubDmx;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBeginToPlay;

    BOOL fgChgATrack;
    BOOL fgChgSbTrack;

    BOOL fgKeyExist;

    BOOL fgStartTrick;

    BOOL fgAudIsReady;
    BOOL fgVDecIsPause;
    BOOL fgIsFirstFrame;
    BOOL fgMp3Abr;
    BOOL fgMp3Vbr;
    BOOL fgGetMp3Status;

    BOOL fgNoCalPts;
    // Feeder
    BOOL fgFeederInvalid;
    BOOL fgFeederIsReady;
    BOOL fgRequestData;
    INT32 i4ReqReadTimes;
    UINT32 u4RecFeederWPtr;
    UINT32 u4RecFeederSize;
    UINT32 u4Mp3Rate;
    UINT32 u4Mp3LastRate;
    UINT32 u4AudFrameSize;
    UINT32 u4AudNotParseSize;

    UINT64 u8CurMoviPos;
    UINT64 u8CurDmxMoviPos;
    UINT64 u8RecDmxMoviPos;
    UINT32 u4LeftBytesInFeeder;

    UINT32 u4VDecType;
    UINT32 u4ADecType;

    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4SpStrmID;
    UINT32 u4PrevAStrmID;
    UINT32 u4PrevSpStrmID;

    UINT32 u4CurDmxFeederRPtr;

    UINT32 u4CurVidPts;
    //UINT32 u4ModeChangPts;

    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;
    UINT32 u4NumSentSub;

    UINT16 u2SkipVidCount;

    UINT32 u4ChangeMode;

    UINT32 u4CurSentKey;

    BOOL fgIsBlock;
    UINT32 u4BlockChunkSize;

    UINT32 u4FeederReqID;

    BOOL fgSpeedChange;
    INT32 i4PrevSpeed;

    UINT64 u8VRemainder;
    UINT64 u8ARemainder;

    //Video 
    UINT32 u4VidFps;
    UINT64 u8VidOffset;
    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;
    
    //Audio
    BOOL fgBadAudio;
    BOOL fgAudIsLoaded;
    BOOL fgRecSpeedChangeForAud;
    BOOL fgAudStrmDisable;
    UINT8 u1CurATrackIdx;
    UINT8 u1TotalATrack;
    INT32 i4RecPrevSpeedForAud;
    UINT32 u4AudSampleRate;
    UINT32 u4AudChannelNs;
    UINT32 u4AudPts[AVI_DIVX_MAX_AUD_TRACK];
    UINT32 u4TriggerAudPts;
    UINT32 u4AudStartPts;
    UINT32 u4AFifoSize;
    BOOL fgQAud;
    UINT32 u4QAudAddr;
    UINT32 u4QAudSize;
    UINT32 u4QAudPts;
    UINT32 u4TotalTime;
    //Drm
    UINT8 u1DrmType;
    UINT16 u2FrameKeyIdx;
    UINT32 u4EncryptOffset;
    UINT32 u4EncryptLength;
    UINT32 u4VidHeaderPtr;
    UINT32 u4VidHdrLength;
    UINT32 u4ErrorData;

    //Seek
    BOOL fgPreviouIsSeek;
    UINT32 u4RecSeekTime;
    BOOL fgIsSeek;
    UINT64 u8ASeekPts;
    UINT64 u8VSeekPts;

    // DLNA 
    BOOL fgReqDataMiss;

    // Range Information
    MM_RANGE_INFO_T *prDmxRangeInfo;
    
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prCurVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *parAudKeyIdxTbl[AVI_DIVX_MAX_AUD_TRACK];
    MM_RANGE_ELMT_IDXTBL_T *prVidIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prAudIdxTbl[AVI_DIVX_MAX_AUD_TRACK];
    //MM_RANGE_ELMT_IDXTBL_T *prSubTitleIdxTbl;
} SWDMX_AVI_INFO_T;


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_AVIInit(ENUM_SWDMX_FMT_T eType);

/**
*/
void _SWDMX_AVISetRange(SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID);


BOOL _SWDMX_AVISetDecoderInfo(UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3);

BOOL _SWDMX_AVIGetInfo(UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);

BOOL _SWDMX_AVISetStrmID(UINT8 u1HdrType,  
    UINT32 u4StrmID, UINT32 u4StrmSubID);

/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_AVIHdrParse(
	SWDMX_RANGE_LIST_T* prRangeList,
	INT32 i4PlaySpeed,
	UINT64 u8SeekPos,
	BOOL fgSpeedChange);


BOOL _SWDMX_AVIEnableStrm(UINT8 u1StrmType, VOID* pfnCbFunc);

BOOL _SWDMX_AVIDisableStrm(UINT8 u1StrmType);

BOOL _SWDMX_AviReadBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo);

BOOL _SWDMX_AviReleaseBuffer(ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo);

BOOL _SWDMX_AviAudioCb(ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

/**
*/
BOOL _SWDMX_AVIStop(void);



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_FLW_H

