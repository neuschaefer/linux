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
 * $RCSfile: vdec_h264util.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_h264util.h
 *  This header file declares exported APIs of MPV.
 */

#ifndef VDEC_H264UTIL_H
#define VDEC_H264UTIL_H


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
#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#define IS_POST_MT5363
#endif
//#define EARLY_DISP
#define FBM_DPB_CTRL
#define WAIT_DISP_UNLOCK_TIME        20         // 40 ms, wait display unlock when change fbg
#define USERDATA_Q_SIZE 16

#define ERROR_CCM_EN
#define STRESS_TESTING

//#define AVC_IRQ_DISABLE
#define DEC_RETRY_NUM 5000//500

#define WAIT_SRAM_STABLE
#define WAIT_THRD 0x1000

#define WAIT_RAP_FRM_CNT 32
#define WAIT_RAP_FRM_REF_CNT 5
//#define BARREL2_SUPPORT
//#define MEM_TEST
//#define REF_TRACE

#define BARREL1 1
#define BARREL2 2

//NALU type
#define NON_IDR_SLICE 0x01
#define IDR_SLICE 0x05
#define SEI 0x06
#define SPS 0x07
#define PPS 0x08
#define AU_DELIMITER 0x09
#define END_OF_SEQ 0x0a

// Slice_type
#define P_Slice 0
#define B_Slice 1
#define I_Slice 2
#define SP_Slice 3
#define SI_Slice 4
#define P_Slice_ALL 5
#define B_Slice_ALL 6
#define I_Slice_ALL 7
#define SP_Slice_ALL 8
#define SI_Slice_ALL 9

// Frame Rate Code
#define VDEC_FRAME_RATE_24_    1    // minus
#define VDEC_FRAME_RATE_24    2
#define VDEC_FRAME_RATE_25    3
#define VDEC_FRAME_RATE_30_    4    // minus
#define VDEC_FRAME_RATE_30    5
#define VDEC_FRAME_RATE_50    6
#define VDEC_FRAME_RATE_60_    7    // minus
#define VDEC_FRAME_RATE_60    8

#define START_CODE 0x000001

#define OUT_OF_FILE 0x000001
#define FORBIDEN_ERR 0x000002
#define DEC_INIT_FAILED 0x000003

#define NO_PIC 0
#define TOP_FIELD 1
#define BOTTOM_FIELD 2
#define FRAME 3

#define NREF_PIC 0
#define SREF_PIC 1
#define LREF_PIC 2

#define BASELINE    66
#define MAIN_PROFILE    77
#define EXT_PROFILE    88
#define FREXT_HP        100      //!< YUV 4:2:0/8 "High"
#define FREXT_Hi10P     110      //!< YUV 4:2:0/10 "High 10"
#define FREXT_Hi422     122      //!< YUV 4:2:2/10 "High 4:2:2"
#define FREXT_Hi444     144      //!< YUV 4:4:4/12 "High 4:4:4"

#define fgChkProfile(u1Byte) ((u1Byte == BASELINE) ||(u1Byte == MAIN_PROFILE) || (u1Byte == EXT_PROFILE) ||\
(u1Byte == FREXT_HP) ||(u1Byte == FREXT_Hi10P) || (u1Byte == FREXT_Hi422) ||\
(u1Byte == FREXT_Hi444))

#define fgChkLevel(u1Byte) (((u1Byte / 10) <=5) && ((u1Byte % 10) <=3))

#define YUV400 0
#define YUV420 1
#define YUV422 2
#define YUV444 3

#define SubWidthC  [4]= { 1, 2, 2, 1};
#define SubHeightC [4]= { 1, 2, 1, 1};

#define fgIsRefPic(u1NalRefIdc) ((u1NalRefIdc > 0))
#define fgIsIDRPic(u1NalUnitType) ((u1NalUnitType == IDR_SLICE))
#define fgIsFrmPic(u1Struct) ((u1Struct == FRAME))
#define fgIsISlice(bType) ((bType == I_Slice) ||(bType == SI_Slice) || (bType == I_Slice_ALL))
#define fgIsPSlice(bType) ((bType == P_Slice) ||(bType == SP_Slice) || (bType == P_Slice_ALL))
#define fgIsBSlice(bType) ((bType == B_Slice) || (bType == B_Slice_ALL))

#define fgIsNonRefFBuf(u1FRefType, u1TRefType, u1BRefType) \
        ((u1FRefType == NREF_PIC) && (u1TRefType == NREF_PIC) && (u1BRefType == NREF_PIC))

#define AVAILABLE_SZ(rptr, wptr) ((rptr <= wptr) ? (wptr - rptr) : (0x500000 - (rptr - wptr)))  //VFIFO 0x500000 
#define MIN_LEN 8   //minimun syntax length

#define AVC_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))
#define AVC_BLK_ALIGMENT         0x3F
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#define MAXIMUMVALUEOFcpb_cnt   32

typedef struct
{
    UINT32 u4CpbCntMinus1;                                                         // ue(v)
    UINT32 u4BitRateScale;                                                           // u(4)
    UINT32 u4CpbSizeScale;                                                         // u(4)
    UINT32 u4BitRateValueMinus1 [MAXIMUMVALUEOFcpb_cnt];     // ue(v)
    UINT32 u4CpbSizeValueMinus1 [MAXIMUMVALUEOFcpb_cnt];    // ue(v)
    BOOL  fgCbrFlag[MAXIMUMVALUEOFcpb_cnt];                         // u(1)
    UINT32 u4InitialCpbRemovalDelayLengthMinus1;                // u(5)
    UINT32 u4CpbRemovalDelayLengthMinus1;                          // u(5)
    UINT32 u4DpbOutputDelayLengthMinus1;                             // u(5)
    UINT32 u4TimeOffsetLength;                                                 // u(5)
} VDEC_AVC_HRD_PARAM_T;


typedef struct
{
    BOOL  fgAspectRatioInfoPresentFlag;                              // u(1)
    UINT32 u4AspectRatioIdc;                                                     // u(8)
    UINT32 u4SarWidth;                                                                // u(16)
    UINT32 u4SarHeight;                                                              // u(16)
    BOOL  fgOverscanInfoPresentFlag;                                   // u(1)
    BOOL  fgOverscanAppropriateFlag;                                    // u(1)
    BOOL  fgVideoSignalTypePresentFlag;                            // u(1)
    UINT32 u4VideoFormat;                                                         // u(3)
    BOOL  fgVideoFullRangeFlag;                                            // u(1)
    BOOL  fgColourDescriptionPresentFlag;                           // u(1)
    UINT32 u4ColourPrimaries;                                                   // u(8)
    UINT32 u4TransferCharacteristics;                                       // u(8)
    UINT32 u4MatrixCoefficients;                                               // u(8)
    BOOL  fgChromaLocationInfoPresentFlag;                      // u(1)
    UINT32 u4ChromaSampleLocTypeTopField;                    // ue(v)
    UINT32 u4ChromaSampleLocTypeBottomField;             // ue(v)
    BOOL  fgTimingInfoPresentFlag;                                      // u(1)
    UINT32 u4NumUnitsInTick;                                                // u(32)
    UINT32 u4TimeScale;                                                            // u(32)
    BOOL  fgFixedFrameRateFlag;                                          // u(1)
    BOOL  fgNalHrdParametersPresentFlag;                       // u(1)
    VDEC_AVC_HRD_PARAM_T rNalHrdParameters;                            // hrd_paramters_t
    BOOL  fgVclHrdParametersPresentFlag;                      // u(1)
    VDEC_AVC_HRD_PARAM_T rVclHrdParameters;                            // hrd_paramters_t
    // if ((nal_hrd_parameters_present_flag || (vcl_hrd_parameters_present_flag))
    BOOL  fgLowDelayHrdFlag;                                               // u(1)
    BOOL  fgPicStructPresentFlag;                                        // u(1)
    BOOL  fgBitstreamRestrictionFlag;                                    // u(1)
    BOOL  fgMotionVectorsOverPicBoundariesFlag;        // u(1)
    UINT32 u4MaxBytesPerPicDenom;                                // ue(v)
    UINT32 u4MaxBitsPerMbDenom;                                   // ue(v)
    UINT32 u4Log2MaxMvLengthVertical;                           // ue(v)
    UINT32 u4Log2MaxMvLengthHorizontal;                       // ue(v)
    UINT32 u4NumReorderFrames;                                         // ue(v)
    UINT32 u4MaxDecFrameBuffering;                                // ue(v)
}VDEC_AVC_VUI_T;


#define MAXnum_ref_frames_in_pic_order_cnt_cycle  256
typedef struct
{
    BOOL   fgSPSValid;                  // indicates the parameter set is valid

    UINT32 u4ProfileIdc;                                                          // u(8)
    BOOL  fgConstrainedSet0Flag;                                        // u(1)
    BOOL  fgConstrainedSet1Flag;                                        // u(1)
    BOOL  fgConstrainedSet2Flag;                                        // u(1)
    BOOL  fgConstrainedSet3Flag;                                        // u(1)
    UINT32 u4LevelIdc;                                                            // u(8)
    UINT32 u4SeqParameterSetId;                                      // ue(v)
    UINT32 u4ChromaFormatIdc;                                           // ue(v)
    BOOL  fgResidualColorTransformFlag;
    UINT32 u4BitDepthLumaMinus8;                                  // ue(v)
    UINT32 u4BitDepthChromaMinus8;                              // ue(v)
    BOOL  fgQpprimeYZeroTransformBypassFlag;

    BOOL  fgSeqScalingMatrixPresentFlag;                     // u(1)
    BOOL  fgSeqScalingListPresentFlag[8];                      // u(1)
    CHAR  ppcScalingList4x4[6][16];                                        // se(v)
    CHAR  ppcScalingList8x8[2][64];                                        // se(v)
    BOOL  fgUseDefaultScalingMatrix4x4Flag[6];
    BOOL  fgUseDefaultScalingMatrix8x8Flag[2];

    UINT32 u4Log2MaxFrameNumMinus4;                       // ue(v)
    UINT32 u4MaxFrameNum;
    UINT32 u4PicOrderCntType;
    // if( pic_order_cnt_type == 0 ) 
    UINT32 u4Log2MaxPicOrderCntLsbMinus4;              // ue(v)
    // else if( pic_order_cnt_type == 1 )
    BOOL fgDeltaPicOrderAlwaysZeroFlag;                   // u(1)
    INT32 i4OffsetForNonRefPic;                                     // se(v)
    INT32 i4OffsetForTopToBottomField;                       // se(v)
    UINT32 u4NumRefFramesInPicOrderCntCycle;       // ue(v)
    // for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
    INT32 ai4OffsetForRefFrame[MAXnum_ref_frames_in_pic_order_cnt_cycle];   // se(v)
    UINT32 u4NumRefFrames;                                              // ue(v)
    BOOL   fgGapsInFrameNumValueAllowedFlag;      // u(1)
    UINT32 u4PicWidthInMbsMinus1;                              // ue(v)
    UINT32 u4PicHeightInMapUnitsMinus1;                  // ue(v)
    BOOL   fgFrameMbsOnlyFlag;                                     // u(1)
    // if( !frame_mbs_only_flag ) 
    BOOL   fgMbAdaptiveFrameFieldFlag;                      // u(1)
    BOOL   fgDirect8x8InferenceFlag;                             // u(1)
    BOOL   fgFrameCroppingFlag;                                      // u(1)
    UINT32 u4FrameCropLeftOffset;                                  // ue(v)
    UINT32 u4FrameCropRightOffset;                                // ue(v)
    UINT32 u4FrameCropTopOffset;                                  // ue(v)
    UINT32 u4FrameCropBottomOffset;                            // ue(v)
    BOOL   fgVuiParametersPresentFlag;                       // u(1)
    VDEC_AVC_VUI_T rVUI;                // vui_seq_parameters_t
}VDEC_AVC_SPS_T;


#define MAXnum_slice_groups_minus1  8
typedef struct
{
    BOOL   fgPPSValid;                  // indicates the parameter set is valid
    //DWRD   pic_parameter_set_id;                                     // ue(v)
    UINT32 u4SeqParameterSetId;                                  // ue(v)
    BOOL   fgEntropyCodingModeFlag;                            // u(1)

    BOOL   fgTransform8x8ModeFlag;                             // u(1)

    BOOL   fgPicScalingMatrixPresentFlag;                   // u(1)
    BOOL   fgPicScalingListPresentFlag[8];                   // u(1)
    CHAR  ppcScalingList4x4[6][16];                                     // se(v)
    CHAR   ppcScalingList8x8[2][64];                                     // se(v)
    BOOL   fgUseDefaultScalingMatrix4x4Flag[6];
    BOOL   fgUseDefaultScalingMatrix8x8Flag[2];

    // if( pic_order_cnt_type < 2 )  in the sequence parameter set
    BOOL  fgPicOrderPresentFlag;                                   // u(1)
    UINT32 u4NumSliceGroupsMinus1;                            // ue(v)
    UINT32 u4SliceGroupMapType;                                  // ue(v)
    // if( slice_group_map_type = = 0 )
    UINT32 u4RunLengthMinus1[MAXnum_slice_groups_minus1]; // ue(v)
    // else if( slice_group_map_type = = 2 )
    UINT32 u4TopLeft[MAXnum_slice_groups_minus1];              // ue(v)
    UINT32 u4BottomRight[MAXnum_slice_groups_minus1];     // ue(v)
    // else if( slice_group_map_type = = 3 || 4 || 5
    BOOL   fgSliceGroupChangeDirectionFlag;               // u(1)
    UINT32 u4SliceGroupChangeRateMinus1;                 // ue(v)
    // else if( slice_group_map_type = = 6 )
    UINT32 u4PicSizeInMapUnitsMinus1;                     // ue(v)
    UINT32 au4SliceGroupId[8];                                              // complete MBAmap u(v)
    UINT32 u4NumRefIdxL0ActiveMinus1;                    // ue(v)
    UINT32 u4NumRefIdxL1ActiveMinus1;                     // ue(v)
    BOOL  fgWeightedPredFlag;                                         // u(1)
    UINT32 u4WeightedBipredIdc;                                      // u(2)
    INT32 i4PicInitQpMinus26;                                      // se(v)
    INT32 i4PicInitQsMinus26;                                      // se(v)
    INT32 i4ChromaQpIndexOffset;                               // se(v)

    INT32 i4SecondChromaQpIndexOffset;                  // se(v)

    BOOL  fgDeblockingFilterControlPresentFlag;         // u(1)
    BOOL  fgConstrainedIntraPredFlag;                          // u(1)
    BOOL  fgRedundantPicCntPresentFlag;                   // u(1)
} VDEC_AVC_PPS_T;


typedef struct
{
    UINT32 u4FirstMbInSlice;
    UINT32 u4SliceType;
    UINT32 u4FrameNum;

    UINT32 u4IdrPicId;
    INT32 i4PicOrderCntLsb;
    INT32 i4PicOrderCntMsb;
    INT32 i4DeltaPicOrderCntBottom;

    BOOL fgFieldPicFlag;
    BOOL fgBottomFieldFlag;

    BOOL fgDirectSpatialMvPredFlag;
    BOOL fgNumRefIdxActiveOverrideFlag;
    INT32 ai4DeltaPicOrderCnt[2];
    UINT32 u4RedundantPicCnt;
    UINT32 u4NumRefIdxL0ActiveMinus1;
    UINT32 u4NumRefIdxL1ActiveMinus1;    

    UINT32 u4ReorderingOfPicNumsIdc;
    UINT32 u4AbsDiffPicNumMinus1;
    UINT32 u4LongTermPicNum;

    UINT32 u4CabacInitIdc;
    INT32 i4SliceQpDelta;

    UINT32 u4LumaLog2WeightDenom;
    UINT32 u4ChromaLog2WeightDenom;
    BOOL fgLumaWeightL0Flag;
    BOOL fgLumaWeightL1Flag;
    BOOL fgChromaWeightL0Flag;
    BOOL fgChromaWeightL1Flag;
    UINT32 au4LumaWeightL0[32];
    UINT32 au4LumaOffsetl0[32];
    UINT32 au4ChromaWeightL0[32][2];
    UINT32 au4ChromaOffsetL0[32][2];
    UINT32 au4LumaWeightL1[32];
    UINT32 au4LumaOffsetL1[32];
    UINT32 au4ChromaWeightL1[32][2];
    UINT32 au4ChromaOffsetL1[32][2];

    UINT32 au4MemoryManagementControlOperation[17];
    UINT32 u4DifferencOfPicNumsMinus1;
    UINT32 u4LongTermFrameIdx;
    UINT32 u4MaxLongTermFrameIdxPlus1;
    
    BOOL fgNoOutputOfPriorPicsFlag;
    BOOL fgLongTermReferenceFlag;
    BOOL fgAdaptiveRefPicMarkingModeFlag;

    BOOL fgSpForSwitchFlag;
    INT32 i4SliceQsDelta;
    UINT32 u4DisableDeblockingFilterIdc;
    INT32 i4SliceAlphaC0OffsetDiv2;
    INT32 i4SliceBetaOffsetDiv2;
    UINT32 u4SliceGroupChangeCycle;
    BOOL fgMmco5;

    BOOL fgRefPicListReorderingFlagL0;
    BOOL fgRefPicListReorderingFlagL1;    
}VDEC_AVC_SLICE_HDR_T;

typedef struct
{
    //pic_timing
    UINT32 u4CpbRemovalDelay;
    UINT32 u4DpbOutputDelay;
    UINT8 u1PicStruct;
    UINT8 u1ClkTimeStampFlag[5];
    UINT8 u1CtType;
    UINT8 u1NuitFieldBasedFlag;
    UINT8 u1CountingType;
    UINT8 u1FullTimeStampFlag;
    UINT8 u1DiscontinuityFlag;
    UINT8 u1CntDroppedFlag;
    UINT8 u1NFrames;
    UINT8 u1SecondsValue;
    UINT8 u1MinutesValue;
    UINT8 u1HoursValue;
    UINT8 u1SecondsFlag;
    UINT8 u1MinutesFlag;
    UINT8 u1HoursFlag;
    INT8 i1TimeOffset;
    UINT32 u4RecoveryFrmCnt;
    UINT8 u1ExtMatchFlg;
    UINT8 u1WaitRAPCnt;
    UINT8 u1WaitRAPICnt;
    UINT8 u1WaitRAPRefCnt;
    UINT32 u4UserDataWptr;
    UINT8 au1UsrData[MAX_USERDATA_SIZE];
}VDEC_AVC_SEI_T;


typedef struct
{
    BOOL fgLastMmco5;
    BOOL fgLastIsRef;
    UINT8 u1LastPicStruct;

    INT32 i4LastPOC;  
    INT32 i4LastTFldPOC;
    INT32 i4LastBFldPOC;
    INT32 i4LastRefPOC;  
    INT32 i4LastRefTFldPOC;
    INT32 i4LastRefBFldPOC;
  
    INT32 i4LastRefPOCMsb;
    INT32 i4LastRefPOCLsb;  
  
    INT32 i4LastFrameNumOffset;  
    UINT32 u4LastFrameNum; 
  
    UINT32 u4LastPicW;
    UINT32 u4LastPicH;  
    UINT32 u4LastSPos;
    UINT32 u4LastNaluType;

    UINT32 u4SARWidth;
    UINT32 u4SARHeight;
    UINT32 u4AspRatInf;
}VDEC_LAST_INFO_T;

typedef enum {
    SEI_BUFFERING_PERIOD = 0,
    SEI_PIC_TIMING,
    SEI_PAN_SCAN_RECT,
    SEI_FILLER_PAYLOAD,
    SEI_USER_DATA_REGISTERED_ITU_T_T35,
    SEI_USER_DATA_UNREGISTERED,
    SEI_RECOVERY_POINT,
    SEI_DEC_REF_PIC_MARKING_REPETITION,
    SEI_SPARE_PIC,
    SEI_SCENE_INFO,
    SEI_SUB_SEQ_INFO,
    SEI_SUB_SEQ_LAYER_CHARACTERISTICS,
    SEI_SUB_SEQ_CHARACTERISTICS,
    SEI_FULL_FRAME_FREEZE,
    SEI_FULL_FRAME_FREEZE_RELEASE,
    SEI_FULL_FRAME_SNAPSHOT,
    SEI_PROG_REFINEMENT_SEGMENT_START,
    SEI_PROG_REFINEMENT_SEGMENT_END,
    SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,
    SEI_FILM_GRAIN_CHARACTERISTICS,
    SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,
    SEI_STEREO_VIDEO_INFO,

    SEI_MAX_ELEMENTS  //!< number of maximum syntax elements
} ENUM_SEI_TYPE;

typedef enum {
    SEI_FRAME = 0,
    SEI_TOP_FIELD,
    SEI_BOTTOM_FIELD,
    SEI_TOP_BOTTOM,
    SEI_BOTTOM_TOP,
    SEI_TOP_BOTTOM_TOP,
    SEI_BOTTOM_TOP_BOTTOM,
    SEI_FRAME_DOUBLING,
    SEI_FRAME_TRIPLING,
    SEI_FRAME_UNKNOWN
} ENUM_DISP_PIC_STRUCT;

typedef struct
{ 
    BOOL fgNonExisting;
    BOOL fgIsBufRef;    //will be TRUE if any field is ref
    BOOL fgIsErr;
    BOOL fgIsNotDisp;
    
    UINT8 u1FBufStatus;  // 1:Top decoded, 2: Bottom decoded
    UINT8 u1FBufRefType;
    UINT8 u1TFldRefType;
    UINT8 u1BFldRefType;

    UINT32 u4LongTermFrameIdx;
    UINT32 u4TFldLongTermFrameIdx;
    UINT32 u4BFldLongTermFrameIdx;

    UINT32 u4FrameNum;
    UINT32 u4SliceType;
    INT32 i4FrameNumWrap;
  
    INT32 i4PicNum;
    INT32 i4TFldPicNum;
    INT32 i4BFldPicNum;  
    INT32 i4LongTermPicNum;
    INT32 i4TFldLongTermPicNum;
    INT32 i4BFldLongTermPicNum;
  
    UINT32 u4W;
    UINT32 u4H;  
    UINT32 u4DramPicSize;  
    UINT32 u4DramPicArea;
    UINT32 u4YAddr;
    UINT32 u4CAddr;
    UINT32 u4MVBufAddr;
      
    INT32 i4POC;
    INT32 i4TFldPOC;
    INT32 i4BFldPOC;

    UINT32 u4TFldPara;
    UINT32 u4BFldPara;   // record for MV
                               // bit 0: field_pic_flag
                               // bit 1: mbaff_flag
    UINT32 u4FrameCnt;

    //FBM
    UCHAR ucFbId;

    //MV buffer idx;
    UCHAR ucMvBufIdx;

    BOOL fgFirstFrmTag;
    UINT32 u4UserDataFrmWptr;
    UINT8 au1UsrDataFrm[MAX_USERDATA_SIZE];
    UINT32 u4UserDataPESWptr;
    UINT8 au1UsrDataPES[MAX_USERDATA_SIZE];

    UINT32 u4SkipFrmCnt;
    BOOL fgPTSValid;
}VDEC_FBUF_INFO_T;

typedef struct
{  
    UINT32 u4RefPicCnt;
    UINT8 au1PicType[32];
    UINT8 au1RefType[32];
    UINT32 au4FBufIdx[32];  
}VDEC_REF_PIC_LIST_T;


typedef struct
{
    HANDLE_T hUsrDataQ;
    UINT32 u4DPBSize;
    UINT32 u4DPBAddr;
    UINT32 u4PredSA;
    //wptr
    UINT32 u4WPtr;

    UINT8 u1NalRefIdc;
    UINT8 u1NalUnitType;    
    UINT8* pu1DecWorkBufY;
    UINT8* pu1DecWorkBufC;
    UINT8* pu1DecWorkBufMV;
    UINT32 u4DramPicSize;
    
    BOOL fgStartPlay;
    BOOL fgIsFirstFrm;
    BOOL fgFirstFrmTag;
    BOOL fgWaitPPS;
    //BOOL fgIDRPic;
    BOOL fgPOCWrap;
    BOOL fgAllocFbFail;
    BOOL fgIsResChg;
    
    // MPEG version: 1, 2, 3, 4 (3: DivX 3.11)
    UINT8 u1MpegVer;
    UINT8 u1PicStruct;
    UINT8 u1DecFld;

    UINT8 u1MaxLongTermFrameIdx;
    UINT8 u1DecFBufIdx;
    UINT8 u1MinPOCBufIdx;
    //BYTE bMaxUsedFBufNum;
    UINT32 u4MaxFBufNum;
    INT32 i4LastPOC;
    
    // Picture Width/Height
    UINT32 u4PicW;
    UINT32 u4PicH;
    UINT32 u4RealPicH;
    UINT32 u4DispPts;
    UINT32 u4Dts;
    UINT64 u8PtsScale;
    UINT64 u8VRemainder;
    UINT32 u4Rate;
    BOOL fgEnCalPTS;

    UINT32 u4PicNum;
    INT32 i4FrmNumOffset;

    BOOL   fgUserScalingMatrixPresentFlag;    // u(1)
    BOOL   afgUserScalingListPresentFlag[8];    // u(1)

    //FBM
    BOOL fgEos;
    BOOL bWaitDecLock;
    UINT8 u1DramMoniter;     //0: none, 1: Total, 2: Mpeg
    UCHAR ucFbgId;
    UINT32 u4WaitFbCnt;
    UINT32 u4WaitFbTime;
    UCHAR ucSyncMode;
    UCHAR ucStcId;
    UINT32 u4CurWaitFbMicro;
    UINT32 u4CurDecMs;
    UINT32 u4CurDecCycle;
    UINT32 u4CurDramCycle;
    UINT32 u4CurRstMs;
    UINT32 u4CurSwMicro;
    
    //MV buffer array
    UCHAR ucEmpMVBufMaxNum; 
    UCHAR aucEmpMVBufIdxMap[18];
    
    //RR mode
    BOOL fgChgRRMode;
    BOOL fgRRModeYEn;
    BOOL fgRRModeCEn;
    BOOL fgRRWrapper;
    UINT8 u1RRModeY;    //0: 1/2, 1: 3/4
    UINT8 u1RRModeC;    //0: 1/2, 1: 3/4
    UINT8 u1MaxDisHbands;   //default 3

    //Display order
    BOOL fgOutDispOrder;
    BOOL fgDispCurDecPic;

    //emulation, repeat frame
    UINT8 u1RptFrm;

    //error concealment mode
    UINT8 u1SliceCnt;
    UINT8 u1ECMode;
    UINT32 u4ECType;
    UINT32 u4DupFrmNumId;   //firmware error handling
    
    //dram bandwith simulation
    UINT32 u4DramMaxLen;
    UINT32 u4DramReqConnDis;
    UINT32 u4DramFifoBank;
    BOOL fgLogSimPat;

    //mm playback
    BOOL fgSeamlessDisp;
    BOOL fgIsMMPlay;
    BOOL fgNotSupport;
    UCHAR ucSkipMode;
    UINT16 u2DecodingOrder;
    UINT32 u4TickNum;
    UINT64 u8Offset;
    UINT64 u8OffsetI;
    
    #ifdef EARLY_DISP
    BOOL fgWaitFirstIDR;
    #endif
    VDEC_AVC_SPS_T* prSPS;
    VDEC_AVC_PPS_T* prPPS;
    VDEC_AVC_SLICE_HDR_T* prSliceHdr;  
    VDEC_AVC_SEI_T rSEI;

    UINT32 u4PESUserDataWptr;
    UINT8 au1PESUsrData[MAX_USERDATA_SIZE];
    
    //TVDSCLPrm *prVDSCLPrm;
    //TFGTPrm *tFGTPrm;
    
    VDEC_LAST_INFO_T rLastInfo;
    VDEC_FBUF_INFO_T arFBufInfo[18];
    VDEC_FBUF_INFO_T* prCurrFBufInfo;
    
    VDEC_REF_PIC_LIST_T arRefPicList[8]; //0:B0 top, 1:B0 bottom, 2:B1 top, 3:B1 bottom, 4:long term top, 5:long term bottom
} VDEC_AVC_DecPrm;


typedef struct
{
    UINT8 u1NalRefIdc;
    UINT8 u1NalUnitType;
    BOOL bPicQueue;
    VDEC_PES_INFO_T rPesInfo;
    VDEC_PES_INFO_T rPesInfoSkip;    //emulaiton, skip pes
    VDEC_AVC_DecPrm rDecParam;
    VDEC_AVC_SLICE_HDR_T rSliceHdr;
    VDEC_AVC_PPS_T arPPS[256];
    VDEC_AVC_SPS_T arSPS[32];
}VDEC_H264_INFO_T;

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#ifdef VDP_RACE_LOG
EXTERN void VDP_FrcRacingLog(CHAR cLog);
#endif
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


#endif //VDEC_H264UTIL_H

