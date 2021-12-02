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
 * $RCSfile: vdec_vp8util.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_vp8util.h
 *  This header file include all global define of vp8 decoder
 */
 
#ifndef VDEC_VP8_UTIL_H
#define VDEC_VP8_UTIL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END
;

//#define CC_VP8_EMULATION
#define SUPPORT_DEC_ISR
#define WAIT_SRAM_STABLE
#define VP8_HEADERPARSE_HWACCELATOR
#define VP8_ENABLE_ERRDETECT
#define VP8_ENABLE_ERRCONCEALMENT
#define VP8_USE_SMALLQT
#define VDEC_DDR3_SUPPORT
#define VP8_64X32_BLK_MODE
//#define VP8_DISABLE_WRAPPER
//#define VP8_ENABLE_MID_PROTECT
#if defined(__MODEL_slt__) || defined(CC_VP8_EMULATION) || defined(MULTIMEDIA_AUTOTEST_SUPPORT)
#define VP8_ENABLE_CRC
#endif

//#define VP8_TEST_BUFRING
//#define VDEC_VP8_DUMP_REG
//#define VP8_EMUCOVERAGE_STATISTIC
//#define USE_VP68_SW_VLD
//#define VDEC_VP8HW_DEBUG
//#define VDEC_VP8_FBMDEBUG
//#define VDEC_VP8_MCPERFORMANCE_TEST
#ifdef CC_DYNAMIC_POWER_ONOFF
    #define VP8_ENABLE_POWER_SAVING
#endif

#ifdef VDEC_TIME_PROFILE
#define VP8_TIME_PROFILE
#define TIME_PROFILE_I_INTERVAL (8)
#define TIME_PROFILE_P_INTERVAL (32)
#endif
#define VDEC_VALUE_ALIGN(value,align) (value= (value&(align-1)) ? ((value+align)&(~(align-1))) : value)
#define VDEC_CLRFLG(flg,index) ((flg) &= ~(1<<(index)))
#define VDEC_SETFLG(flg,index) ((flg) |= (1<<(index)))
#define VDEC_FLGSET(flg,index) (((flg)>>(index))&1)
#define VDEC_SETFLG_COND(flg,index,cond) ((cond) ? VDEC_SETFLG(flg,index) : VDEC_CLRFLG(flg,index))
#define VDEC_MEMALIGN(ptr,align) (ptr+=(align-(((UINT32)ptr)&(align-1))))
#define VDEC_RPOS_INC(src,len,startaddr,endaddr) (src= (src+(len)>=endaddr) ? (startaddr+((src)+(len)-(endaddr))) : ((src)+(len)))
#define VDEC_INTEGER(integer,src,len,startaddr,endaddr) \
{  \
   UCHAR uByte,uIntLen=0; \
   integer=0; \
   while(uIntLen<len) \
   {  \
      uByte=*((UCHAR *)src); \
      integer=(integer<<8)|uByte; \
      uIntLen++;\
      VDEC_RPOS_INC(src,1,startaddr,endaddr); \
   } \
}

#define VDEC_INTREVERSE(integer,len) \
{ \
  UINT32 u4Value=0,uIndex=0; \
  while(uIndex<len) \
  {  \
    u4Value=u4Value<<8|((integer&(0xff<<(uIndex*8)))>>(uIndex*8)); \
    uIndex++; \
  } \
  integer=u4Value;\
}

#define TIME_PROFILE (11)
#define TIME_PROFILE2 (12)
#define TIME_PROFILE3  (13)

#ifdef VP8_EMUCOVERAGE_STATISTIC
#define VP8_EMUCOVERAGE_ITEM_NUM 25
#define VP8_EMUCOVERAGE_BUFSIZE (1024)
typedef struct
{
   UINT8 *puData;
}VDEC_VP8_EMUCOVERAGE_INFO_T;
#endif

#define VDEC_VP8_DEC_PIC_MODE (1)
#define VDEC_VP8_DEC_MCUROW_MODE (0)
#define VDEC_VP8_WAITEMODE_POLLING (1)
#define VDEC_VP8_WAITEMODE_ISR (0)
#define VDEC_VP8_WAIT_DISP_TIME (1500)
#define WAIT_THRD 0x1000
#define DEC_RETRY_NUM 8000
#define VP8WEBP_RINGBUF_MINWIDTH (33)
#define VP68_KEY_FRAME (0)
#define VP68_INTER_FRAME (1)
#define FRM_PIC  (0)
#define VP8_MINQ (0)
#define VP8_MAXQ (127)
#define VP8_QINDEX_RANGE (VP8_MAXQ+1)
#define VP8_SEGMENT_ALTQ (0x01)
#define VP8_SEGMENT_ALTLF (0x02)
#define MB_FEATURE_TREE_PROBS (3)
#define MAX_MB_SEGMENTS (4)
#define MAX_REF_LF_DELTAS (4)
#define MAX_MODE_LF_DELTAS (4)
#define MAX_VP8_DATAPARTITION (9)
#define MAX_FILTER_LVL (63)

#define WEBP_MIN_HW (4)
#define WEBP_MAX_HW (16383)
#define IMGRZ_WORKING_BUFSIZE (1920*12)
typedef enum
{
  VP8PARAM_NO_LPF,
  VP8PARAM_SIMPLER_LPF,
  VP8PARAM_BILINER_MCFILTER,
  VP8PARAM_FULL_PIXEL, // 3
  VP8PARAM_COLOR,
  VP8PARAM_CLAMP_TYPE,
  VP8PARAM_SEGMENT_ENABLE,
  VP8PARAM_SEGMENT_UPDATE_MAP, //7
  VP8PARAM_SEGMENT_UPDATE_DATA,
  VP8PARAM_SEGMENT_ABSDATA, // 0--Delta data, 1--Abs data.
  VP8PARAM_MODEREF_IFDELTA_UPDATE,
  VP8PARAM_MODEREF_LFDELTA_ENABLE, // 11 
  VP8PARAM_REFRESH_GOLDEN,
  VP8PARAM_REFRESH_ALTRF,
  VP8PARAM_QINDEX_UPDATE,
  VP8PARAM_REFRESH_PROBS,  // 15
  VP8PARAM_REFRESH_LASTFRAME,
  VP8PARAM_NOCOEF_SKIP,
  VP8PARAM_64X32_OUTPUT,
  VP8PARAM_DDR3_CONFIG,
  VP8PARAM_WAPPER_OFF,    // 20
  VP8PARAM_RINGBUF_CTRL,
  VP8PARAM_WEBP_MODE,
  VP8PARAM_WEBP_PICMODE,
  VP8PARAM_WEBP_FORCE_PPON,
  VP8PARAM_WEBP_RESET_BUF
}VDEC_PARAM_VP8FLAG_T;

typedef enum
{
  VP8_MBPRED_MODE_DC,
  VP8_MBPRED_MODE_V,
  VP8_MBPRED_MODE_H,
  VP8_MBPRED_MODE_TM,
  VP8_MBPRED_MODE_B,
  VP8_MBPRED_MODE_NEARESTMV,
  VP8_MBPRED_MODE_NEARMV,
  VP8_MBPRED_MODE_ZEROMV,
  VP8_MBPRED_MODE_NEWMV,
  VP8_MBPRED_MODE_SPLITMV,
  VP8_MBPRED_MODE_MAX
}VDEC_VP8_MB_PREDICTION_MODE_T;

typedef enum
{
  VP8_BPRED_MODE_B_DC,
  VP8_BPRED_MODE_B_TM,
  VP8_BPRED_MODE_B_VE,
  VP8_BPRED_MODE_B_HE,
  VP8_BPRED_MODE_B_LD,
  VP8_BPRED_MODE_B_RD,
  VP8_BPRED_MODE_B_VR,
  VP8_BPRED_MODE_B_VL,
  VP8_BPRED_MODE_B_HD,
  VP8_BPRED_MODE_B_HU,
  VP8_BPRED_MODE_LEFT4X4,
  VP8_BPRED_MODE_ABOVE4X4,
  VP8_BPRED_MODE_ZEOR4X4,
  VP8_BPRED_MODE_NEW4X4,
  VP8_BPRED_MODE_MAX
}VDEC_VP8_B_PREDICTION_MODE_T;

typedef enum
{
  VP8_MVREF_INTRA_FRAME,
  VP8_MVREF_LAST_FRAME,
  VP8_MVREF_GOLDEN_FRAME,
  VP8_MVREF_ALTREF_FRAME,
  VP8_MVREF_MAX
}VDEC_VP8_MVREF_FRAME_T;

typedef enum
{
  VP8_QTYPE_Y1AC=0,
  VP8_QTYPE_Y1DC,
  VP8_QTYPE_Y2DC,
  VP8_QTYPE_Y2AC,
  VP8_QTYPE_UVDC,
  VP8_QTYPE_UVAC,
  VP8_QTYPE_MAX
}VDEC_VP8_QTYPE_T;

typedef enum
{
  VP8_QTABLE_Y1,
  VP8_QTABLE_Y2,
  VP8_QTABLE_UV,
  VP8_QTALBE_MAX
}VDEC_VP8_QTABLE_T;

typedef enum
{
  VP8_MBLVL_ALT_Q,
  VP8_MBLVL_ALT_LF,
  VP8_MBLVL_MAX
}VDEC_VP8_MBLVL_FEATURE;

typedef enum
{
  VP8_LF_TYPE_NORMAL=0,
  VP8_LF_TYPE_SIMPLE
}VDEC_VP8_FILTER_TYPE_T;

typedef enum
{
  VDEC_VP8_PIC_STATE,
  VDEC_VP8_MB_STATE,
  VDEC_VP8_COEF_STATE
}VDEC_VP8_DECSTATE_T;

typedef enum
{
  VP8_DEC_TYPE_WEBM,
  VP8_DEC_TYPE_WEBP
}VDEC_VP8_DECTYPE_T;

typedef enum
{
  VP8_MVDEF_MV_MAX=1024,
  VP8_MVDEF_MVVALS=(2*VP8_MVDEF_MV_MAX)+1,
  VP8_MVDEF_MVLONG_WIDHT=10,
  VP8_MVDEF_MVNUM_SHORT=8,
  VP8_MVDEF_MVPIS_SHORT=0,
  VP8_MVDEF_MVPSIGN,
  VP8_MVDEF_MVPSHORT,
  VP8_MVDEF_MVPBITS=VP8_MVDEF_MVPSHORT+VP8_MVDEF_MVNUM_SHORT-1,  // value is 9
  VP8_MVDEF_MVPCOUNT=VP8_MVDEF_MVPBITS+VP8_MVDEF_MVLONG_WIDHT   //value is 19
}VDEC_VP8_MVDEF_T;

#ifdef USE_VP68_SW_VLD
typedef struct _BOOL_VLD
{
    UINT32 u4LowValue;
    UINT32 u4Range;
    UINT32 u4Value;
    INT32 i4Count;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
    UINT32 u4ReadPtr;
}BOOL_VLD_T;

typedef struct _BARREL_SHIFTER
{
    UINT32 u4Value;
    INT32  i4BitsAvailable;
    UINT32 u4ReadPtr;
    UINT32 u4BufStart;
    UINT32 u4BufEnd;
}BERRAL_SHIFTER;

#endif


#define VP8_BINTRAMODES (VP8_BPRED_MODE_B_HU+1)  // 10
#define VP8_SUBMVPREFS (1+VP8_BPRED_MODE_NEW4X4-VP8_BPRED_MODE_LEFT4X4)
#define VP8_YMODES (VP8_MBPRED_MODE_B+1)
#define VP8_UVMODES (VP8_MBPRED_MODE_TM+1)
#define VP8_MVREFS (1+VP8_MBPRED_MODE_SPLITMV-VP8_MBPRED_MODE_NEARESTMV)
#define VP8_BLOCK_TYPES (4)
#define VP8_COEF_BANDS (8)
#define VP8_PREV_COEF_CONTEXTS (3)
#define VP8_COEF_TOKENS (12)
#define VP8_PROB_TYPE(pBrob,btype) (pBrob+btype*VP8_COEF_BANDS*VP8_PREV_COEF_CONTEXTS*(VP8_COEF_TOKENS-1))
#define VP8_PROB_BAND(pBtype,band) (pBtype+band*VP8_PREV_COEF_CONTEXTS*(VP8_COEF_TOKENS-1))
#define VP8_PROB_CTXT(pBand,ctxtype) (pBand+ctxtype*(VP8_COEF_TOKENS-1))
#define VP8_PROB_TOKEN(pCtxt,token) (pCtxt+token)

typedef struct
{
  UINT8 Prob[VP8_MVDEF_MVPCOUNT];
}VDEC_VP8_MV_CONTEXT_T;

typedef struct
{
  //UINT8 BModeProb[VP8_BINTRAMODES-1];
  UINT8 YModeProb[VP8_YMODES-1];
  UINT8 UVModeProb[VP8_UVMODES-1];
  //UINT8 SubMVRefProb[VP8_SUBMVPREFS-1];
  UINT8 CoefProbs[VP8_BLOCK_TYPES][VP8_COEF_BANDS][VP8_PREV_COEF_CONTEXTS][VP8_COEF_TOKENS-1];
  VDEC_VP8_MV_CONTEXT_T MVC[2];
  //VDEC_VP8_MV_CONTEXT_T PRE_MVC[2];
}VDEC_VP8FRAME_CONTEXT_T;

typedef struct
{
  UINT32 u4FlagParam;
  UINT32 u4LastParam;
  INT8 SegmentFeatureData[VP8_MBLVL_MAX][MAX_MB_SEGMENTS];
  UINT8 SegmentTreeProbs[MB_FEATURE_TREE_PROBS];
  INT8 RefLFDeltas[MAX_REF_LF_DELTAS];
  INT8 ModeLFDeltas[MAX_MODE_LF_DELTAS];
  INT8 RefFrameSignBias[VP8_MVREF_MAX];
  VDEC_VP8_FILTER_TYPE_T eLoopFilterType;
  INT8 iLoopFilterLvl;
  INT8 iSharpLvl;
  UINT8 uDataPartitionNum;
  UINT8 uDataPartitionToken;
  UINT8 uSkipFalseProb;
  UINT8 uIntraProb;
  UINT8 uLastProb;
  UINT8 uGoldenProb;
  UINT8 uCopyBuf2Gf; // 0:no 1:last-->gf 2: arf-->gf
  UINT8 uCopyBuf2Arf;  // 0:no 1:laast-->arf   2:gf-->arf
  INT16 QIndexInfo[VP8_QTYPE_MAX];
  UINT8 *puWorkingBuf;
  VDEC_VP8FRAME_CONTEXT_T rLastFC;
  VDEC_VP8FRAME_CONTEXT_T rCurFc;
}VDEC_PARAM_VP8DEC_T;

typedef struct _VDEC_INFO_VP8_FRM_HDR_T_
{
   UINT8 uFrameType;
   UINT8 uShowFrame;
   UINT8 uHScale;
   UINT8 uVScale;
   UINT16 u2Width;
   UINT16 u2Height;
   UINT8 uVersion;
   UINT32 u4VldStartPos;
   UINT32 u4FifoStart;
   UINT32 u4FifoEnd;
   UINT32 u4FrameSize;
   UINT32 u4WritePos;
   UINT32 u4GldYAddr;
   UINT32 u4AlfYAddr;
   UINT32 u4LstYAddr;
   UINT32 u4CurYAddr;
   UINT32 u4CurCAddr;
   UINT32 u4VLDWrapper;
   UINT32 u4SegIdWrapper;
   UINT32 u4PPWrapperY;
   UINT32 u4PPWrapperC;
   UINT32 u4rPartitionSize[MAX_VP8_DATAPARTITION];
   VDEC_PARAM_VP8DEC_T rVp8DecParam;
   #ifdef VP8_EMUCOVERAGE_STATISTIC
   VDEC_VP8_EMUCOVERAGE_INFO_T *prEmuItems;
   #endif
}VDEC_INFO_VP8_FRM_HDR_T;

#ifdef VDEC_VP8_HWDEBLOCK
#define VDEC_PP_ENABLE TRUE
#else
#define VDEC_PP_ENABLE FALSE    
#endif

#define VDEC_VP8WEBP_MAX_WAIT_TIME (1000)
#define VDEC_VP8WEBP_MCUROW_WAITTIME (400)
#define VDEC_VP8WEBP_PIC_WAITTIME (10000)
#define VDEC_VP8WEBP_PICRING_WAITTIME (10000)
#define VDEC_VP8WAIT_TIME (1000)
#define VDEC_VP8_FBNUM (8)
#define VDEC_VP8DEF_FRAME_RATE (30000)
#define IPBMode 0x0
#define IPMode  0x1
#define IMode   0x2

#define VP8_NO_PIC 0
#define VP8_TOP_FIELD 1
#define VP8_BOTTOM_FIELD 2
#define VP8_FRAME 3
#define VP8_PARTITION_BUF_MINSIZE (1024)
#define VP8_KEYHDR_PREPARSE_LEN (10)
#define VP8_INTTYPE_DECFIN 0
#define VP8_INTTYPE_PARTCHG 4
#define VP8_INTTYPE_BUFCTRL 8

typedef enum
{
  VP8_DEC_FLG_UNSUPPORT=0,
  VP8_DEC_FLG_INITED,
  VP8_DEC_FLG_ENDCALPTS,
  VP8_DEC_FLG_DECERROR,
  VP8_DEC_FLG_FINISHED,
  VP8_DEC_FLG_PARTCHG,
  VP8_DEC_FLG_MCUROW,
  VP8_DEC_FLG_BSPWKBUF,
  VP8_DEC_FLG_LOCKED,
  VP8_DEC_FLG_STARTED,
  VP8_DEC_FLG_DATAEMPTY,
  VP8_DEC_FLG_FORCESTOP,
  VP8_DEC_FLG_MULTIINSTANCE,
  VP8_DEC_FLG_LOGERRINFO
} VP8_DEC_FLAG;

typedef struct
{
  BOOL  fgKeyFrame;
  UCHAR ucFbgId;
  UCHAR ucFbgType;
  UCHAR ucSyncMode;
  UCHAR ucSkipMode;
  UCHAR ucCurFbId;
  UCHAR ucLastFbId;
  UCHAR ucGoldenFbId;
  UCHAR ucAltFbId;
  UINT16 u2NewWidth;
  UINT16 u2NewHeight;
  UINT16 u2Width;
  UINT16 u2Height;
  UINT32 u4Flag;
  UINT32 u4DispPts;
  UINT32 u4Rate;
  UINT32 u4FrameCounter;
  UINT32 u4KekFrameIndex;
  UINT32 u4WorkBuf;
  UINT32 u4DeltaPTS;
  INT32  i4DecRet;
  UINT32 u4WPtr;
  UINT32 u4VldReadPtr;
  UINT32 u4VldReadBits;
  UINT32 u4FramePitch;
  UINT64 u8Offset;
#if defined(VP8_ENABLE_CRC) || defined(WEBP_ENABLE_CRC)
  UINT32 u4DbkType;
  UINT32 pu4CrcVal[2][4];
  UINT32 u4RzCrc;
#endif
#ifdef CC_VP8_EMULATION
  UINT32 u4LogFrameIndex;
  UINT32 u4DbgFrame;
  UINT32 u4DumpFrameIndex;
  UCHAR *puFileName;
#endif
  INT8 RefTime[VDEC_VP8_FBNUM];
}VP8_DEC_PARAM_T;

typedef struct
{
   UINT32 u4ColorBufY;
   UINT32 u4ColorBufC;
   UINT32 u4VLDWrapper;
   UINT32 u4PPWrapperY;
   UINT32 u4PPWrapperC;
   UINT32 u4SegIdWapper;
   UINT32 u4RzTempBuf;
}WEBP_DEC_BUFINFO_T;

typedef struct
{
  UINT32 u4Start;
  UINT32 u4End;
  UINT32 u4ReadPos;
  UINT32 u4WritePos;
  UINT32 u4ReadOffset;
  UINT32 u4RequestedSize;
  UINT32 u4UsedBitCount;
  UINT32 u4RealWritePos;
}WEBP_BUF_INFO;

typedef struct
{
  UINT32 u4Tag;
  PFN_EWBP_EVENT_NOTIFY pfnWebpCb;
  WEBP_BUF_INFO rSrcBufInfo[MAX_VP8_DATAPARTITION];
  UINT32 u4BufStartAddr;
  UINT32 u4BufSize;
  UINT32 u4FileOffset;
  WEBP_DECIN_PARAM_T *prDecInParam;
  WEBP_DECOUT_PARAM_T *prDecOutParam;
  WEBP_DEC_BUFINFO_T rBufLayOut;
  UINT32 u4RingBufIndex;
  UINT32 u4MbRowIndex;
  UINT32 u4HeaderLen;
  UINT32 u4BitStreamStartPos;
  UINT32 u4ScalFactor;
  BOOL fgWrapperOff;
  BOOL fgPartitionRing;
  BOOL fgBufEnough;
  UINT8 u1DecMode;
}WEBP_DEC_INFO_T;

typedef struct
{
  UINT32 u4DecMs;
  UINT32 u4SwMicro;
  UINT32 u4CurDecCycle;
  UINT32 u4CurDramCycle;
  UINT32 u4DelayClock;
  UINT32 u4IFrameCounter;
  UINT32 u4PFrameCounter;
  UINT32 u4TotalIDecTime;
  UINT32 u4TotalPDecTime;
  UINT32 u4TotalISwTime;
  UINT32 u4TotalPSwTime;
  UINT32 u4TotalIDecCycle;
  UINT32 u4TotalPDecCycle;
  UINT32 u4TotalIDramCycle;
  UINT32 u4TotalPDramCycle;
  UINT32 u4MaxIDecCycle;
  UINT32 u4MaxPDecCycle;
  UINT32 u4MaxIDramCycle;
  UINT32 u4MaxPDramCycle;
  UINT32 u4MaxISwTime;
  UINT32 u4MaxPSwTime;
  UINT32 u4MaxIDecTime;
  UINT32 u4MinIDecTime;
  UINT32 u4MaxPDecTime;
  UINT32 u4MinPDecTime;
  HAL_TIME_T rTimeS;
  HAL_TIME_T rTimeE;
  HAL_TIME_T rTimeDif;
}VDEC_VP8_TIMEPROFILE;

typedef struct
{
  VDEC_PES_INFO_T rPesInfo;
  VP8_DEC_PARAM_T rDecParam;
  WEBP_DEC_INFO_T rWebPInfo;
  VDEC_INFO_VP8_FRM_HDR_T rVp8FrmHdr;
#ifdef VP8_TIME_PROFILE
  VDEC_VP8_TIMEPROFILE rTimeProfile;
#endif
  UINT8 uEsId;
  VDEC_VP8_DECTYPE_T eDecType;
}VDEC_VP8_INFO_T;

#endif  // VDEC_VP8_UTIL_H
