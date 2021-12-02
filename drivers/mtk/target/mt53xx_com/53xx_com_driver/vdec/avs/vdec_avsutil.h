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
 * $RCSfile: vdec_avsutil.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_avsutil.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_AVSUTIL_H
#define VDEC_AVSUTIL_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
#include "vdec_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#define DEFINE_IS_LOG    VDEC_IsLog
#include "x_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define JIZHUN  0x20
#define ZENGQIANG  0x40

#define VDEC_AVS_UNKNOWN_IDX 0xFF

//#define CC_AVS_EMULATION
//#define CC_DUMP_AVS_RISC_SETTING

//#define VDEC_TIME_PROFILE

#define WAIT_SRAM_STABLE

// hw related definitions
#define WAIT_THRD 0x1000
#define DEC_RETRY_NUM 5000 //500

#define AVS_WAIT_DEC_TIME                500    //SW wait decoding time (ms)

//#define WAIT_SRAM_STABLE

// decoder buffer definitions
#define MAX_FRM_BUFNUM 22
#define MAX_REF_PICNUM 3

#define REF_LIST_0 0
#define REF_LIST_1 1
#define REF_LIST_2 2

#define AVS_WAIT_DISP_TIME 1500

#define AVS_MIN_LEN 8   //minimun syntax length

#define AVAILABLE_SZ(rptr, wptr) ((rptr <= wptr) ? (wptr - rptr) : (0x500000 - (rptr - wptr)))  //VFIFO 0x500000 

#define fgChkProfile(u1Byte) ((u1Byte == JIZHUN) ||(u1Byte == ZENGQIANG))

// define the size of working buffer
#define AVS_VLD_PRED_SZ 20 * 1024
#define AVS_VLD_MV_SZ 150 *1024

#define fgChkLevel(u1Byte) (((u1Byte / 10) <=4) && ((u1Byte % 10) <=2))

#define I_IMG 0
#define P_IMG 1
#define B_IMG 2

#define FLD_ENCODE 0
#define FRM_ENCODE 1

#define NO_PIC 0
#define TOP_FIELD 1
#define BOTTOM_FIELD 2
#define FRAME 3

#define NREF_PIC 0
#define REF_PIC 1

#define WAIT_DISP_UNLOCK_TIME        20         // 40 ms, wait display unlock when change fbg

#define BARREL1 1
#define BARREL2 2

// Data unit type
#define SEQ_HDR_SC          0xB0
#define SEQ_END_SC          0xB1
#define I_PICTURE_SC        0xB3
#define PB_PICTURE_SC     0xB6
#define USER_DATA_SC      0xB2
#define EXTENSION_SC       0xB5
#define VIDEO_EDIT_SC      0xB7
#define SLICE_SC_MIN        0x00
#define SLICE_SC_MAX       0xAF

// Slice_type

// Frame Rate Code
#define VDEC_FRAME_RATE_24_    1    // minus
#define VDEC_FRAME_RATE_24    2
#define VDEC_FRAME_RATE_25    3
#define VDEC_FRAME_RATE_30_    4    // minus
#define VDEC_FRAME_RATE_30    5
#define VDEC_FRAME_RATE_50    6
#define VDEC_FRAME_RATE_60_    7    // minus
#define VDEC_FRAME_RATE_60    8

#define fgIsRefPic(u4PicCodingType) ((u4PicCodingType < B_IMG))
#define fgIsFrmPic(u1Struct) ((u1Struct == FRAME))
#define fgIsNonRefFBuf(u1FRefType, u1TRefType, u1BRefType) \
        ((u1FRefType == NREF_PIC) && (u1TRefType == NREF_PIC) && (u1BRefType == NREF_PIC))

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct
{ 
    BOOL fgNonExisting;
    BOOL fgIsBufRef;    //will be TRUE if any field is ref
    BOOL fgIsErr;
    BOOL fgIsNotDisp;
    BOOL fgPTSValid;
    
    UINT8 u1FBufStatus;  // 1:Top decoded, 2: Bottom decoded
    UINT8 u1FBufRefType;
    UINT8 u1TFldRefType;
    UINT8 u1BFldRefType;

    UINT32 u4FrameNum;
    UINT32 u4SliceType;
    INT32 i4FrameNumWrap;
  
    UINT32 u4W;
    UINT32 u4H;  
    UINT32 u4DramPicSize;  
    UINT32 u4DramPicArea;
    UINT32 u4YAddr;
    UINT32 u4CAddr;
    UINT32 u4MVBufAddr;

    //FBM
    UCHAR ucFbId;

    // Picture coding type
    UINT8 u1PicCodingType;

    // Frame picture distance
    UINT32 u4PicDistance;
}VDEC_AVS_FBUF_INFO_T;

typedef struct
{ 
    BOOL fgValid;

    //FBM
    UCHAR ucFbId;

    // Fb Index
    UINT8 u1DecFBufIdx;
}VDEC_AVS_REF_BUF_T;


typedef struct
{
    HANDLE_T hUsrDataQ;

    BOOL fgLogSimPat;

    BOOL fgEnPP;

    BOOL fgFirstI;

    BOOL fgSecField;

    UINT8 u1LastRefIdx;

    //wptr
    UINT32 u4WPtr;

    // For re-trigger usage
    UINT32 u4ByteCnt;

    // Decoding buffer
    UCHAR* pucDecWorkBufY;
    UCHAR* pucDecWorkBufC;
    UCHAR* pucDecWorkBufMV;

    // Prediction working buffer
    UINT32 u4VldPredBufAddr;

    // Motion vector working buffer
    UINT32 u4TopMvBufAddr;
    UINT32 u4BottomMvBufAddr;

    //Display order
    BOOL fgOutDispOrder;
    BOOL fgDispCurDecPic;

    BOOL fgDumpSetting;
    
    // Keep the last Picture Width/Height
    UINT32 u4LastHSize;
    UINT32 u4LastVSize;

    UINT32 u4LineSize;

    UINT32 u4TimeOutNum;

    // Picture Type
    UINT8 u1PicStruct;

    //FBM
    BOOL fgAllocFbFail;
    BOOL fgEos;
    BOOL bWaitDecLock;
    UINT8 u1DramMoniter;     //0: none, 1: Total, 2: Mpeg
    UCHAR ucFbgId;
    UCHAR ucFbgType;
    UCHAR ucFbId;
    UINT32 u4WaitFbCnt;
    UINT32 u4WaitFbTime;
    UCHAR ucSyncMode;
    UCHAR ucStcId;
    UINT8 u1DecFBufIdx;
    UINT32 u4CurWaitFbMicro;
    UINT32 u4CurDecMs;
    UINT32 u4CurDecCycle;
    UINT32 u4CurDramCycle;
    UINT32 u4CurRstMs;
    UINT32 u4CurSwMicro;

    //Display order

    //emulation, repeat frame
    UINT8 u1RptFrm;

    UINT32 u4MaxFBufNum;

    //error concealment mode
    UINT8 u1SliceCnt;
    UINT8 u1ECMode;
    UINT32 u4ECType;
    UINT32 u4DupFrmNumId;   //firmware error handling

    //mm playback
    BOOL fgIsMMPlay;
    BOOL fgNotSupport;
    UCHAR ucSkipMode;
    UINT16 u2DecodingOrder;
    UINT32 u4TickNum;
    UINT64 u8Offset;
    UINT64 u8OffsetI;

    // Addressing Mode
    UINT32 u4AddrMode;

    UINT32 u4PicCnt;

    // Bit Count
    UINT32 u4BitCnt;

    VDEC_AVS_FBUF_INFO_T arFBufInfo[MAX_FRM_BUFNUM];

    VDEC_AVS_REF_BUF_T arRefFBufInfo[MAX_REF_PICNUM];

    // Frame Buffer Info
    VDEC_AVS_FBUF_INFO_T* prCurrFBufInfo;

} VDEC_AVS_DECPRM_T;


typedef struct
{
    UINT32 u4ProfileID                  :    8;
    UINT32 u4LevelID                    :    8;
    UINT32 u4IsProgSeq                :    1;
    UINT32 u4HSize                        :    14;
    UINT32 u4VSize                        :    14;
    UINT32 u4ChromaFmt              :    2;
    UINT32 u4SamplePrec              :    3;
    UINT32 u4AspRatio                   :    4;
    UINT32 u4FrmRate                   :    4;
    UINT32 u4BitRateL                    :    18;
    UINT32 u4MarketBitBR              :    1;
    UINT32 u4BitRateU                   :    12;
    UINT32 u4LowDelay                  :    1;
    UINT32 u4MarkerBit                  :    1;
    UINT32 u4BBVSize                     :    18;
    UINT32 u4RsvBits                      :    3;
    UINT32 u4IsValid                       :    1;
} VDEC_AVS_SEQ_T;


typedef struct
{
    UINT32 u4BBVDelay                  :    16;
    UINT32 u4TimeCodeFg             :    1;
    UINT32 u4TimeCode                 :    24;
    UINT32 u4MarkerBit                 :    1;
    UINT32 u4BBVCheckTimes        :    32;
    UINT32 u4PicCodingType         :    2;
    UINT32 u4PicDistance              :    8;
    UINT32 u4ProgFrm                   :    1;
    UINT32 u4PicStruct                   :    1;
    UINT32 u4AdvPredModeDisable:    1;
    UINT32 u4TFT                           :    1;
    UINT32 u4RFF                           :    1;
    UINT32 u4FixedPicQP               :    1;
    UINT32 u4PicQP                        :    6;
    UINT32 u4PicRefFg                   :    1;
    UINT32 u4NoForwardRefFg       :    1;
    UINT32 u4SkipModeFg              :    1;
    UINT32 u4RsvBits                      :    4;
    UINT32 u4LoopFilterDisable     :    1;
    UINT32 u4LoopFilterParamFg   :    1;
    UINT32 u4IsValid                       :    1;    //stuffing bit
    INT32 i4AlphaCOffset                :    32;   
    INT32 i4BetaOffset                    :    32;    
} VDEC_AVS_PIC_T;


typedef struct
{
    UINT16 u2VPosExt                    :    3;
    UINT16 u2FixedSliceQP            :    1;
    UINT16 u2SliceQP                     :    6;
    UINT16 u2SliceWeightFg          :    1;
    UINT16 u2MBWeightFg             :    1;

    UINT8 u1LumaScale[2];
    INT8 i1LumaShift[2];
    UINT8 u1ChromaScale[2];
    INT8 i1ChromaShift[2];
} VDEC_AVS_SLICE_T;


typedef struct
{
    VDEC_PES_INFO_T rPesInfo;
    VDEC_AVS_DECPRM_T rDecParam;
    VDEC_AVS_SEQ_T rSeqHdr;
    VDEC_AVS_PIC_T rPicHdr;
    VDEC_AVS_SLICE_T rSliceHdr;

    UINT32 u4PrevPrsPtr;
}VDEC_AVS_INFO_T;


typedef struct
{  
    UINT32 u4RefPicCnt;
    UINT8 au1PicType[32];
    UINT8 au1RefType[32];
    UINT32 au4FBufIdx[32];  
}VDEC_AVS_REF_PIC_LIST_T;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#ifdef VDP_RACE_LOG
EXTERN void VDP_FrcRacingLog(CHAR cLog);
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_AVSUTIL_H

