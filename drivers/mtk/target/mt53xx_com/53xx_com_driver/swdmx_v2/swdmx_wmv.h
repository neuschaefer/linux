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
 * $RCSfile: swdmx_wmv.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_flw.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef SWDMX_WMV_H
#define SWDMX_WMV_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "swdmx_drvif.h"
#include "x_lint.h"
//LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
//LINT_EXT_HEADER_END

#if 0  // moved to swdmx_drvif.h for multi-instance

//netflix stream type
typedef enum
{
    NETFLIX_MULTIPLEXED, // Multiplexed Stream
    NETFLIX_AUDIO,       // Audio Only Stream
    NETFLIX_VIDEO        // Video Only Stream
} NetflixStreamType;



typedef struct
{
    UINT32 u4FilePos;
    UINT8 u1PayloadLengthType;
    UINT8 u1PayloadsNumber;
} MultiPayloadInfo;


typedef struct
{
    UINT8 u1StreamNum;
    BOOL  fgKeyFrame;
    UINT32 u4MediaObjNum;
    UINT32 u4OffsetIntoMediaObj;
    UINT32 u4ReplicatedDataLen;
    UINT32 u4PTS;
    UINT32 u4PTSDelta;
    UINT32 u4MediaObjSize;
} PayloadHeader;

typedef struct
{
    //Length Type Flags
    BOOL  fgMultiplePayload;
    UINT8 u1SequenceType;
    UINT8 u1PaddingLenghtType;
    UINT8 u1PacketLengthType;
    BOOL  fgErrorCorrectPresent;
    //Property Flags
    UINT8 u1ReplicatedDataType;
    UINT8 u1OffsetIntoType;
    UINT8 u1MediaObjectNumType;
    UINT8 u1StreamNumberType;
    UINT32 u4PacketLength;
    UINT32 u4Sequence;
    UINT32 u4PaddingLength;
    UINT32 u4SendTime;
    UINT16 u2Duration;
} PayloadParseInfo;


typedef struct
{
    ENUM_SWDMX_VDEC_T eVideoType;
    ENUM_SWDMX_ADEC_T eAudioType;

    UINT64 u8FileSize;
    UINT32 u4SequenceHeader;
    UINT32 u4SequenceHeaderLength;
    UINT32 u4DataObjSize;
    UINT32 u4PacketSize;
    UINT32 u4PreRoll;

    UINT32 u4FeederReqID;
    UINT32 u4PreviousPos;
    UINT32 u4PreviousIdx;
    UINT32 u4CurParsingPacketNo;
    UINT32 u4TotalPacketNo;
    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;
    UINT16 u2SendPayloadNo;
    UINT16 u2PreviousSendPayloadNo;
    UINT32 u4WmaErrorCode;

    UINT32 u4VStreamId;
    UINT32 u4AStreamId;
    UINT32 u4SpStreamId;

    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnSubTitle;
    BOOL fgFirstFrame;
    BOOL fgBlock;
    BOOL fgReSend;
    BOOL fgEnADmx;
    BOOL fgEnVDmx;
    BOOL fgFirstAudioError;
    BOOL fgFeederInvalid;
    UINT32 u4PacketStartAddr;
    UINT32 u4CurParsingBytes;
    UINT32 u4CurPTS;
    UINT32 u4FeederReadSize;
    BOOL fgDemuxFinish;
    //for DRM
    BOOL fgIsDRM;

    //for trick mode
    BOOL fgReqFeederDataEOS;
    BOOL fgFirstReqFeederData;
    UINT32 u4PrePacketNo;
    BOOL fgFileWithStartCode;
    BOOL fgFlushFeederQueue;
    UINT64 u8LastFileParsingPosition;
    UINT32 u4ConsumedSize;
    BOOL fgVideoPause;
    UINT32 u4TriggerAudioPTS;
    BOOL fgTrickReceivePreviousPTS;
    BOOL fgChangeAudio;
    BOOL fgBrokenFile;
    BOOL fgGetKeyHead;
    BOOL fgGetKeyTail;
    //UINT32 u4TargetVideoPTS; //record the pts when changing audio channel
    //UINT32 u4LastVideoPTS;
    UINT32 u4RepeatTimes;
    UINT32 u4CurPacketPTS;
    BOOL fgFinishDemuxIFrame;
    UINT32 u4VideoCount;
    UINT32 u4AudioCount;
    UINT32 u4FirstVideoPTS;
    BOOL fgBeginPlay;
    UINT32 u4FrameRate;
    UINT8 u1FWKeyFrameNo;
    UINT64 u8SeekPTS;
    UINT32 u4DisplayPTS;
    UINT32 u4FrameOffset;
    UINT32 u4IFrameOffset;
    UINT32 u4CurKeyIdx;
    BOOL fgFinishKeyTable;
    INT32 i4WMVPlaySpeed;
    BOOL fgWMVSpeedChange;
    UINT64 u8WMVFileOffset;
    BOOL fgAudioNormalPlay;
    BOOL fgVideoNormalPlay;
    UINT32 u4LastVideoWptr;
    MM_RANGE_ELMT_IDXTBL_T * prWmvKeyIdxTbl;

    BOOL fgAlreadySet;
    BOOL fg2XAudParsing;
} SWDMX_WMV_INFO_T;

#endif  // 0, moved to swdmx_drvif.h for multi-instance

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

VOID _SWDMX_WMVSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, UINT32 u4VidStrmID,
                        UINT32 u4AudStrmID, UINT32 u4SubTitleID);

//#ifdef WMV_SEEK_OFFSET
SWDMX_HDR_PRS_STATE_T _SWDMX_WMVHeaderParse(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList,
        SWDMX_CUR_CMD_T* prCmd);
//#else
//SWDMX_HDR_PRS_STATE_T _SWDMX_WMVHeaderParse(UINT8 u1SrcId, SWDMX_RANGE_LIST_T* prRangeList,
//        INT32 i4PlaySpeed, UINT64 u8SeekPos, BOOL fgSpeedChange);
//#endif



BOOL _SWDMX_WMVStop(UINT8 u1SrcId);
BOOL _SWDMX_WMVAbort(UINT8 u1SrcId);
BOOL _SWDMX_WMVInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType);
BOOL _SWDMX_WMVSetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32 u4Para1,UINT32 u4Para2,UINT32 u4Para3);
BOOL _SWDMX_WMVGetInfo(UINT8 u1SrcId, UINT8 u1Type,UINT32* pu4Para1,UINT32* pu4Para2,UINT32* pu4Para3);
BOOL _SWDMX_WMVSendData(SWDMX_RANGE_LIST_T* prRangeList);
BOOL _SWDMX_WMVDeliverData(SWDMX_INFO_T *prSwdmxInst, SWDMX_RANGE_LIST_T* prRangeList);
BOOL _SWDMX_WMVDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType);
BOOL _SWDMX_WMVEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc);
BOOL _SWDMX_WMVSetStrmID(UINT8 u1SrcId, UINT8 u1HdrType,UINT32 u4StrmID, UINT32 u4StrmSubID);
BOOL _SWDMX_WMVInitDmx(SWDMX_INFO_T* prSwdmxInst, UINT8 u1HdrType,UINT8 u1Index,BOOL fgEnable,BOOL fgEos);
VOID _SWDMX_WMVSendEOS(SWDMX_INFO_T *prSwdmxInst);
VOID _SWDMX_WMVComsumeFeeder(SWDMX_INFO_T *prSwdmxInst);
BOOL _SWDMX_WMVIsFeederBufferFull(SWDMX_INFO_T *prSwdmxInst);
/*
#ifdef CC_DLNA_SUPPORT
BOOL _SWDMX_WMVRequestFeederData(UINT32 u4QrySize,UINT64 u8Offset);
#else
BOOL _SWDMX_WMVRequestFeederData(UINT32 u4QrySize,UINT64 u8Offset,BOOL fgForceGetData);
#endif
*/
BOOL _SWDMX_WMVAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

BOOL _SWDMX_WmvAudioCallback(const DMX_AUDIO_PES_T* prPes);
BOOL _SWDMX_WmvVideoCallback(const DMX_PES_MSG_T* prPes);

BOOL _WMVParsingPayload(SWDMX_INFO_T *prSwdmxInst, UINT32 u4ParsingCount,PayloadParseInfo* prPayLoadInfo,UINT32 u4PacketStartAddr,UINT32 u4CurrentPos,MultiPayloadInfo* prMultiPayloadInfo);
BOOL _WMVParsingPacket(SWDMX_INFO_T *prSwdmxInst, UINT32 u4PacketStartAddr,UINT32 u4CurrentPos,UINT32 u4PacketSize);
BOOL _WMVMoveData(SWDMX_INFO_T *prSwdmxInst, UINT32 u4StartAddress,UINT32 u4CurPosition ,UINT32 u4FileSize,PayloadHeader* prPayLoadHeader,BOOL fgEndFrame);
VOID _WMVStartToPlay(SWDMX_INFO_T *prSwdmxInst);
UINT32 _WMVParsingStartCode(UINT32 u4FrameAddr,UINT32 u4FifoStart,UINT32 u4FifoEnd);







//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //SWDMX_WMV_H


