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
 * $RCSfile: swdmx_mkv.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file swdmx_mkv.h
 *  
 */

#ifndef SWDMX_MKV_H
#define SWDMX_MKV_H


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

#define AVC_HDR_LEN 4
#define VC1_HDR_LEN 4
#define AAC_HDR_LEN 7
#define OGG_HDR_LEN (27 + 255)
#define MKV_SUBTITLE_BUFSIZE 64 * 1024

#define EBML1BYTE(arg)       ((arg >> 24) & 0x0000007F)
#define EBML2BYTE(arg)       ((arg >> 16) & 0x00003FFF)
#define EBML3BYTE(arg)       ((arg >>  8) & 0x001FFFFF)
#define EBML4BYTE(arg)       ((arg      ) & 0x0FFFFFFF)
#define MKV_EBML_INVALID      (0xFFFFFFFF)

#define NORMAL_1X 1000
#define FF_2X 2000
#define FF_8X 8000
#define STEP_FW 1
#define IsBackward(i4Speed)     (i4Speed < 0)

#define DISP_PROG 1
#define DISP_INTERLACE 2

#define MKV_AUD_PREBUF_IDX 7
#define MKV_AUD_UNKNOWN_IDX 0xFF

#define FLUSH_VFIFO 0
#define KEEP_VFIFO 1
#define AUD_PRE_BUF 2

#define DIVX_DRM_LENGTH 10
#define MKV_DIVX_DRM_AES_SIZE_128 128 // DivX AES 128 bits 
//------------------------------------------------------------
//keep consistent in 536x, 539x
#define BYTE_3(arg)        (*((UINT8 *)VIRTUAL((UINT32)&(arg)) + 3))
#define BYTE_2(arg)        (*((UINT8 *)VIRTUAL((UINT32)&(arg)) + 2))
#define BYTE_1(arg)        (*((UINT8 *)VIRTUAL((UINT32)&(arg)) + 1))
#define BYTE_0(arg)        (* (UINT8 *)VIRTUAL((UINT32)&(arg)))

#define LOAD_1BYTE(arg1, arg2)  BYTE_0(arg2) = *((UINT8 *)VIRTUAL((UINT32)(arg1)))

/* big endian */
#define LOADB_1WORD(arg1, arg2)  BYTE_1(arg2) = *((UINT8 *)VIRTUAL((UINT32)(arg1))); \
                                BYTE_0(arg2) = *((UINT8 *)VIRTUAL((UINT32)(arg1)) + 1)

/* little endian */
#define LOADL_1WORD(arg1, arg2)  BYTE_1(arg2) = *((UINT8 *)VIRTUAL((UINT32)(arg1)) + 1); \
                                BYTE_0(arg2) = *((UINT8 *)VIRTUAL((UINT32)(arg1)))
                             
/* big endian */
#define LOADB_1DWRD(p, arg2)  BYTE_3(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p))); \
                             BYTE_2(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 1); \
                             BYTE_1(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 2); \
                             BYTE_0(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 3)

/* little endian */  
#define LOADL_1DWRD(p, arg2)  BYTE_3(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 3); \
                             BYTE_2(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 2); \
                             BYTE_1(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)) + 1); \
                             BYTE_0(arg2) = *((UINT8 *)VIRTUAL((UINT32)(p)))
                                  
/*#define LOADL_QWRD(p, arg2) ( (arg2) = ((UINT64)(p)[7] << 56)   \
                                        | ((UINT64)(p)[6] << 48)   \
                                        | ((UINT64)(p)[5] << 40)   \
                                        | ((UINT64)(p)[4] << 32)   \
                                        | ((UINT64)(p)[3] << 24)   \
                                        | ((UINT64)(p)[2] << 16)   \
                                        | ((UINT64)(p)[1] << 8)    \
                                        | (UINT64)(p)[0] )           */
                                        
//---------------------------------------------------------------
#define fgMkvIs1cc(addr, a)     ((*((UINT8 *)VIRTUAL((UINT32)(addr)))) == a)
#define fgMkvIs2cc(addr, a, b)  (((*((UINT8 *)VIRTUAL((UINT32)(addr)))) == a) && ((*((UINT8 *)VIRTUAL((UINT32)(addr + 1)))) == b))
#define fgMkvIs3cc(addr, a, b, c)   ((((*((UINT8 *)VIRTUAL((UINT32)(addr)))) == a) && ((*((UINT8 *)VIRTUAL((UINT32)(addr + 1)))) == b)) && \
                                                            ((*((UINT8 *)VIRTUAL((UINT32)(addr + 2)))) == c))
#define fgMkvIs4cc(addr, a, b, c, d)    ((((*((UINT8 *)VIRTUAL((UINT32)(addr)))) == a) && ((*((UINT8 *)VIRTUAL((UINT32)(addr + 1)))) == b)) && \
                                                            (((*((UINT8 *)VIRTUAL((UINT32)(addr + 2)))) == c) && ((*((UINT8 *)VIRTUAL((UINT32)(addr + 3)))) == d)))

#define MKV_UNCERTAIN_SZ                          (64) // 4 Byte(s) ID + 8 Byte(s) SZ

// Table 0 (P.09)
#define fgMkvIsEBMLID(address)                    fgMkvIs4cc(address, 0x1A, 0x45, 0xDF, 0xA3)
#define fgMkvIsSegmentID(address)                 fgMkvIs4cc(address, 0x18, 0x53, 0x80, 0x67)

// Table 2 (P.12 ~ 13) // Top Level (Level 1)
#define fgMkvIsSegmentSegmentInfoID(address)      fgMkvIs4cc(address, 0x15, 0x49, 0xA9, 0x66)
#define fgMkvIsSegmentSeekHeadID(address)         fgMkvIs4cc(address, 0x11, 0x4D, 0x9B, 0x74)
#define fgMkvIsSegmentClusterID(address)          fgMkvIs4cc(address, 0x1F, 0x43, 0xB6, 0x75)
#define fgMkvIsSegmentTracksID(address)           fgMkvIs4cc(address, 0x16, 0x54, 0xAE, 0x6B)
#define fgMkvIsSegmentCuesID(address)            fgMkvIs4cc(address, 0x1C, 0x53, 0xBB, 0x6B)
#define fgMkvIsSegmentAttachmentID(address)       fgMkvIs4cc(address, 0x19, 0x41, 0xA4, 0x70)
#define fgMkvIsSegmentChapterID(address)          fgMkvIs4cc(address, 0x10, 0x43, 0xA7, 0x70)
#define fgMkvIsSegmentTagsID(address)             fgMkvIs4cc(address, 0x12, 0x54, 0xC3, 0x67)
#define fgMkvIsCRC32(address)                     fgMkvIs1cc(address, 0xBF)

// SegmentInfo
#define fgMkvIsInfoTimeCodeScaleID(address)       fgMkvIs3cc(address, 0x2A, 0xD7, 0xB1)

// Table 4 (P.16) // Level 2
#define fgMkvIsSeekHeadSeekID(address)            fgMkvIs2cc(address, 0x4D, 0xBB)

// Table 5 (P.16) // Level 3
#define fgMkvIsSeekSeekIDID(address)              fgMkvIs2cc(address, 0x53, 0xAB)
#define fgMkvIsSeekSeekPositionID(address)        fgMkvIs2cc(address, 0x53, 0xAC)

// Table 6 (P.18) // Level 2
#define fgMkvIsTracksTrackEntryID(address)        fgMkvIs1cc(address, 0xAE)

// Table 7 (P.18 ~ 21) // Level 3
#define fgMkvIsTeTrackNumberID(address)           fgMkvIs1cc(address, 0xD7)
#define fgMkvIsTeTrackUidID(address)              fgMkvIs2cc(address, 0x73, 0xC5)
#define fgMkvIsTeTrackTypeID(address)             fgMkvIs1cc(address, 0x83)
#define fgMkvIsTeTrackDurationID(address)         fgMkvIs3cc(address, 0x23, 0xE3, 0x83)
#define fgMkvIsTeLanguageID(address)              fgMkvIs3cc(address, 0x22, 0xB5, 0x9C)
#define fgMkvIsTeCodecIDID(address)               fgMkvIs1cc(address, 0x86)
#define fgMkvIsTeCodecPrivateID(address)          fgMkvIs2cc(address, 0x63, 0xA2)
#define fgMkvIsTeTrackVideoID(address)            fgMkvIs1cc(address, 0xE0)
#define fgMkvIsTeTrackAudioID(address)            fgMkvIs1cc(address, 0xE1)

// Table 8 (P.21 ~ 22) // Level 4
#define fgMkvIsVPixelWidthID(address)             fgMkvIs1cc(address, 0xB0)
#define fgMkvIsVPixelHeightID(address)            fgMkvIs1cc(address, 0xBA)
#define fgMkvIsVCropBottomID(address)             fgMkvIs2cc(address, 0x54, 0xAA)
#define fgMkvIsVCropTopID(address)                fgMkvIs2cc(address, 0x54, 0xBB)
#define fgMkvIsVCropLeftID(address)               fgMkvIs2cc(address, 0x54, 0xCC)
#define fgMkvIsVCropRightID(address)              fgMkvIs2cc(address, 0x54, 0xDD)
#define fgMkvIsVDisplayWidthID(address)           fgMkvIs2cc(address, 0x54, 0xB0)
#define fgMkvIsVDisplayHeightID(address)          fgMkvIs2cc(address, 0x54, 0xBA)

// Table 9 (P.22 ~ 23) // Level 4
#define fgMkvIsASamplingFrequencyID(address)             fgMkvIs1cc(address, 0xB5)
#define fgMkvIsAOutputSamplingFrequencyID(address)       fgMkvIs2cc(address, 0x78, 0xB5)
#define fgMkvIsAChannelsID(address)                      fgMkvIs1cc(address, 0x9F)
#define fgMkvIsABitDepthID(address)                      fgMkvIs2cc(address, 0x64, 0x64)

// Table 13 (P.25) // Values of TrackType
#define fgMkvIsTrackVideo(value)                  (value == 0x01)
#define fgMkvIsTrackAudio(value)                  (value == 0x02)
#define fgMkvIsTrackComplex(value)                (value == 0x03)
#define fgMkvIsTrackLogo(value)                   (value == 0x10)
#define fgMkvIsTrackSubtitle(value)               (value == 0x11)
#define fgMkvIsTrackButton(value)                 (value == 0x12)
#define fgMkvIsTrackControl(value)                (value == 0x20)

// Table 16 (P.28) // Level 2
#define fgMkvIsClusterTimeCodeID(address)         fgMkvIs1cc(address, 0xE7)
#define fgMkvIsClusterPositionID(address)         fgMkvIs1cc(address, 0xA7)
#define fgMkvIsClusterPrevSizeID(address)         fgMkvIs1cc(address, 0xAB)
#define fgMkvIsClusterBlockGroupID(address)       fgMkvIs1cc(address, 0xA0)
#define fgMkvIsClusterSimpleBlockID(address)      fgMkvIs1cc(address, 0xA3)

// Table 17 (P.29) // Level 3
#define fgMkvIsBGBlockID(address)                 fgMkvIs1cc(address, 0xA1)
#define fgMkvIsBGReferenceBlockID(address)        fgMkvIs1cc(address, 0xFB)
#define fgMkvIsBGBlockDurationID(address)         fgMkvIs1cc(address, 0x9B)
// DivX DRM Info
#define fgMkvIsBGDrmBlockID(address)          fgMkvIs1cc(address, 0xDD)

// Table 18 (P.31) // Level 2
#define fgMkvIsCueCuePointID(address)             fgMkvIs1cc(address, 0xBB)

// Table 19 (P.31) // Level 3
#define fgMkvIsCPTimeID(address)                  fgMkvIs1cc(address, 0xB3)
#define fgMkvIsCPTrackPositionID(address)         fgMkvIs1cc(address, 0xB7)

// Table 20 (P.31) // Level 4
#define fgMkvIsTPTrackID(address)                 fgMkvIs1cc(address, 0xF7)
#define fgMkvIsTPClusterPositionID(address)       fgMkvIs1cc(address, 0xF1)
#define fgMkvIsTPBlockNumberID(address)           fgMkvIs2cc(address, 0x53, 0x78)

// Matroska block Layout and Lacing
#define fgMkvIsKeyframe(flag8b)        (((flag8b >> 7) & 0x1) > 0)
#define fgMkvIsInvisible(flag8b)       (((flag8b >> 3) & 0x1) > 0)
#define fgMkvIsLacing(flag8b)          (((flag8b >> 1) & 0x3) > 0)
  #define fgMkvIsNoLacing(flag8b)        (((flag8b >> 1) & 0x3) == 0x0)
  #define fgMkvIsXiphLacing(flag8b)      (((flag8b >> 1) & 0x3) == 0x1)
  #define fgMkvIsFixedSizeLacing(flag8b) (((flag8b >> 1) & 0x3) == 0x2)
  #define fgMkvIsEBMLLacing(flag8b)      (((flag8b >> 1) & 0x3) == 0x3)
#define fgMkvIsDiscardable(flag8b)     (((flag8b     ) & 0x1) > 0)


#define MKV_AVC_TYPE_AVI    0x00
#define MKV_AVC_TYPE_MP4    0x01
#define MKV_AVC_TYPE_NONE   0xFF
#define fgMkvIsAvcTypeAvi(u4MkvAvcType) (u4MkvAvcType == MKV_AVC_TYPE_AVI)
#define fgMkvIsAvcTypeMp4(u4MkvAvcType) (u4MkvAvcType == MKV_AVC_TYPE_MP4)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum  // parser states
{
    MKVPRS_G_EBML_ID                         = 0x11,
  
    MKVPRS_G_SEGMENT_ID                      = 0x21,
    MKVPRS_G_SEGMENT_ELEMENT_ID              = 0x22,
  
    MKVPRS_G_SEGMENT_SEEK_ID                 = 0x31,
    MKVPRS_G_SEGMENT_SEEK_ELEMENT_ID         = 0x32,
  
    MKVPRS_G_SEGMENT_INFO_ID                 = 0xB1,
    
    MKVPRS_G_SEGMENT_TE_ID                   = 0x41,
    MKVPRS_G_SEGMENT_TE_ELEMENT_ID           = 0x42,
  
    MKVPRS_G_SEGMENT_TE_ELEMENT_V_BIH        = 0x43,
  
    MKVPRS_G_SEGMENT_TE_ELEMENT_VIDEO        = 0x44,
    MKVPRS_G_SEGMENT_TE_ELEMENT_CODEC_PRIVATE         = 0x45,
    MKVPRS_G_SEGMENT_TE_ELEMENT_CODEC_PRIVATE_SPS_NUM = 0x46,
    MKVPRS_G_SEGMENT_TE_ELEMENT_CODEC_PRIVATE_SPS     = 0x47,
    MKVPRS_G_SEGMENT_TE_ELEMENT_CODEC_PRIVATE_PPS_NUM = 0x48,
    MKVPRS_G_SEGMENT_TE_ELEMENT_CODEC_PRIVATE_PPS     = 0x49,
  
  
  //MKVPRS_G_SEGMENT_CLUSTER_ID              = 0x51, // Not Exist
    MKVPRS_G_SEGMENT_CLUSTER_ELEMENT_ID      = 0x52,
    MKVPRS_G_SEGMENT_CLUSTER_BG_ELEMENT_ID   = 0x53,
    MKVPRS_G_BLOCK_DATA                      = 0x54,
  
    MKVPRS_G_SEGMENT_CP_ID                   = 0x61,
    MKVPRS_G_SEGMENT_CP_ELEMENT_ID           = 0x62,
    MKVPRS_G_SEGMENT_CP_TP_ELEMENT_ID        = 0x63,
  
    MKVPRS_G_DATA                            = 0x7F,
    MKVPRS_G_DATA_NO_LACING                  = 0x80,
    MKVPRS_G_DATA_XIPH_LACING_HDR       = 0x81,
    MKVPRS_G_DATA_FIXED_LACING               = 0x82,
    MKVPRS_G_DATA_EBML_LACING_HDR         = 0x83,
  
    MKVPRS_G_DATA_AVC                        = 0x84,
    MKVPRS_G_DATA_LACING                  = 0x85,

    MKVPRS_P_TXING_ALT_DATA                  = 0x91,
    MKVPRS_P_TXING                           = 0x92,
  
    MKVPRS_P_TX_SEC_PSA_PEA                  = 0xA1,
    MKVPRS_P_TX_PSA_SEC                      = 0xA4,
    MKVPRS_P_TX_PSA_SEC_PEA                  = 0xA2,
    MKVPRS_P_TX_PSA_PEA_SEC                  = 0xA3
}MKV_ELMT_ID_STATE;

/*
typedef struct {
    BOOL fgReadEOS;
    BOOL fgEnVideo;
    BOOL fgEnAudio;
    BOOL fgEnAudioLast;
    BOOL fgEnSubTitle;

    BOOL fgEnVDmx;
    BOOL fgEnADmx;

    BOOL fgDeliverVEos;
    BOOL fgDeliverAEos;

    BOOL fgBeginToPlay;
    BOOL fgSeqHdrParsed;

    BOOL fgChgATrack;
    BOOL fgAudTrigVidDec;
    
    BOOL fgChgSbTrack;
    BOOL fgEnSubDmx;

    UINT8 u1FF2xTo1xMode;   // 0: flush fifo, 1: not flush fifo, start sending audio from swdmx's position, 2: audio pre-buffer
    // Feeder
    BOOL fgIsUnderFlw;
    BOOL fgFeederInvalid;
    BOOL fgFeederAck;
    INT32 i4ReqReadTimes;

    UINT64 u8CurMoviPos;
    UINT64 u8CurDmxMoviPos;
    UINT64 u8CurClusterPos;
    UINT32 u4LeftBytesInFeeder;

    UINT32 u4VDecType;
    UINT32 u4ADecType[MKV_MAX_AUD_TRACK];
    UINT32 u4MkvAvcType;
    
    UINT32 u4VStrmID;
    UINT32 u4AStrmID;
    UINT32 u4CurAStrmID;
    UINT32 u4SpStrmID;

    UINT32 u4CurDmxFeederRPtr;

    UINT32 u4CurVidPts;
    UINT32 u4CurBlockPts;
    UINT32 u4ModeChangPts;

    UINT32 u4NumSentAud;
    UINT32 u4NumSentVid;

    UINT32 u4CurSentKey;
    UINT32 u4NumOfKeyEntry;
    //TPT
    UINT32 u4MainNumOfKeyEntry;
    UINT32 u4TPTNumOfKeyEntry;
    UINT64  u8FileOffset;
    UINT64  u8RangeSz;
    UINT64  u8MainFileOffset;
    UINT64  u8MainRangeSz;
    UINT64  u8TPTFileOffset;
    UINT64  u8TPTRangeSz;
    BOOL fgIsTPT;

    BOOL fgIsBlock;
    UINT32 u4BlockChunkSize;

    UINT32 u4FeederReqID;

    INT32 i4PrevSpeed;
    INT32 i4LastSpeed;
    
    //Video 
    UINT32 u4KeyFrmSz;
    UINT32 u4KeyFrmPTS;
    UINT32 u4AudTrigVidDecPTS;
    UINT32 u4ChgATrackVidPTS;
    UINT32 u4VidFps;
    UINT64 u8VidOffset;
    UINT32 u4CurRptPTS;
    UINT32 u4CurRepeatFrameIdx;
    UINT32 u4VidRepeatFrame;
    UINT32 u4MainVidHeaderPtr;
    UINT32 u4MainVidHdrLength;
    UINT32 u4TPTVidHeaderPtr;
    UINT32 u4TPTVidHdrLength;    
    UINT8* aucAVCHdr;
    UINT8* aucVC1Hdr;
    UINT8 u1DispStruct;
    
    //Audio
    BOOL fgIsAudWaitAout;
    BOOL fgPendingAudMoveData[MKV_MAX_AUD_TRACK];    
    UINT8 u1CurATrackIdx;
    UINT8 u1TotalATrack;
    UINT8* aucAACHdr;
    UINT32 u4AudSampleRate[MKV_MAX_AUD_TRACK];
    UINT32 u4AudChannelNs[MKV_MAX_AUD_TRACK];
    UINT32 u4AudPts[MKV_MAX_AUD_TRACK];
    BOOL fgAudPreBuf[MKV_MAX_AUD_TRACK];
    BOOL fgIsAudTrack[MKV_MAX_AUD_TRACK];
    DMX_MM_DATA_T rAudDmxMMData[MKV_MAX_AUD_TRACK];
	UINT64 au8PostionxTime[10][2];

    //Subtitle
    BOOL fgPendingSbMoveData;
    UINT32 i4BlkDuration;
    DMX_MM_DATA_T rSbDmxMMData;
    
    //Drm
    BOOL fgDrmEnable;
    UINT16 u2FrameKeyIdx;
    UINT32 u4EncryptOffset;
    UINT32 u4EncryptLength;
    
    //Seek
    BOOL fgIsSeek;
    UINT64 u8SeekPts;
    UINT64 u8AudSeekPts;
    BOOL fgKeyDelivered;
    BOOL fgWaitIFrm;
    UINT32 u4KeyPTS;
    UINT32 u4KeyDeliveredPTS;
    
    UINT64 u8MkvSegmentInfoTimeCodeScale;
    UINT32 u4ClusterTimeCode;
    UINT32 u4PrsState;
    UINT32 au4MkvElementSa[MKV_MAX_LEVEL];
    UINT32 au4MkvElementSz[MKV_MAX_LEVEL];
    UINT32 au4MkvElementEa[MKV_MAX_LEVEL];
    UINT32 au4MkvElementLevel;
    UINT8 u1StrmType;
    UINT8 u1MkvAvcNaluLen;

    UINT32 u4MkvFrameCount;
    UINT32 u4MkvBHeaderSa;    
    UINT32 u4MkvBHeaderSz; // size of _dMkvFrameCount

    UINT32 u4MkvFrmCntPrs;
    UINT32 u4MkvFrmAccSz;
    UINT32 au4MkvFrmCntPrsSz[255];
    
    MM_RANGE_ELMT_IDXTBL_T *prVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prMainVidKeyIdxTbl;
    MM_RANGE_ELMT_IDXTBL_T *prTPTVidKeyIdxTbl;

    MM_RANGE_INFO_T 	*prDmxRangeInfo;

    //MM_RANGE_ELMT_IDXTBL_T *prSubTitleIdxTbl;
} SWDMX_MKV_INFO_T;
*/


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

BOOL _SWDMX_MKVInit(UINT8 u1SrcId, ENUM_SWDMX_FMT_T eType);

/**
*/
void _SWDMX_MKVSetRange(UINT8 u1SrcId, SWDMX_RANGE_LIST_T *prRangeList, 
    UINT32 u4VidStrmID, 
    UINT32 u4AudStrmID, 
    UINT32 u4SubTitleID);


BOOL _SWDMX_MKVSetDecoderInfo(UINT8 u1SrcId, UINT8 u1Type, 
    UINT32 u4Para1, 
    UINT32 u4Para2, 
    UINT32 u4Para3);


BOOL _SWDMX_MKVGetInfo(UINT8 u1SrcId, UINT8 u1Type,
    UINT32* pu4Para1,
    UINT32* pu4Para2,
    UINT32* pu4Para3);


BOOL _SWDMX_MKVSetStrmID(UINT8 u1HdrType,  
    UINT8 u1SrcId, UINT32 u4StrmID, UINT32 u4StrmSubID);

/**
*/
SWDMX_HDR_PRS_STATE_T _SWDMX_MKVHdrParse(
       UINT8 u1SrcId,
	SWDMX_RANGE_LIST_T* prRangeList,
	SWDMX_CUR_CMD_T* prCmd);


BOOL _SWDMX_MKVEnableStrm(UINT8 u1SrcId, UINT8 u1StrmType, VOID* pfnCbFunc);

BOOL _SWDMX_MKVDisableStrm(UINT8 u1SrcId, UINT8 u1StrmType);


/**
*/
BOOL _SWDMX_MKVStop(UINT8 u1SrcId);

BOOL _SWDMX_MKVFlush(UINT8 u1SrcId);

BOOL _SWDMX_MkvAudioCb(UINT8 u1SrcId, ENUM_SWDMX_AUDIO_NOTIFY_TYPE_T eAudioNotifyType);

BOOL _SWDMX_MkvReadBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo);
BOOL _SWDMX_MkvReleaseBuffer(UINT8 u1SrcId, ENUM_SWDMX_STRM_TYPE_T eStrmType, 
    SWDMX_READ_INFO_T *prReadInfo);


BOOL _SWDMX_MkvVideoCallback(const DMX_PES_MSG_T* prPes);
BOOL _SWDMX_MkvAudioCallback(const DMX_AUDIO_PES_T* prPes);


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //SWDMX_MKV_H


