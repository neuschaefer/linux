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
 * $RCSfile: vdec_wmvutil.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_wmvutil.h
 *  This header file declares exported APIs of wmv.
 */

#ifndef VDEC_WMVUTIL_H
#define VDEC_WMVUTIL_H

#include "x_lint.h"
#include "vdec_drvif.h"
//LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "vdec_wmvhw.h"
#include "../vdec_debug.h"
//LINT_EXT_HEADER_END


//#define CC_WMV_EMULATION


#define MAXPANSCANWINDOWS 4

#define MAXHALFQP           8
#define MAXHIGHRATEQP       8
#define MAX3QP              8

#define MIN_BITRATE_MB_TABLE 50
#define MAX_BITRATE_DCPred_IMBInPFrame 128
#define NUMBITS_SLICE_SIZE 5 // maximum 32 MB's
#define NUMBITS_SLICE_SIZE_WMV2 3 // To indicate Processor's #
//error code
#define END_OF_FILE 0x00000031
//decoding option
#define cbResetVDec 0
#define cbDiffStop 1
#define cbWMVHeaderOnly 0

#if defined (CC_MT5392B) || defined(CC_MT5365) || defined(CC_MT5395)
#define cbWMVFWBP 0 //ilii
#else
#define cbWMVFWBP 1 //ilii
#endif

//disable decoding finish interrupt

//spork
//#define AVC_IRQ_DISABLE
#define WAIT_SRAM_STABLE
//#define WMV_PRINT_FBM_STATUS

#define MAX_MBX 128 // for 1920 pixels, 1920/16=120
#define MAX_MBY 68 // for 1080 pixels, 1080/16=67.5
#define BP_MB_BITS 64 // represent 64 MB in a MB-row

#define MAX_RETRY_COUNT 200*1024 //100KBytes

/********/
/* enum */
/********/
enum vdec_STATUS
{
    VDEC_NORM_INIT_PRM =0,
    VDEC_NORM_VPARSER,
    VDEC_NORM_WAIT_TO_DEC,
    VDEC_NORM_WAIT_DECODE,
    VDEC_NORM_DEC_END,
    VDEC_NORM_ERROR
};

enum tagWMVDecodeStatus
{
    WMV_Succeeded = 0,
    WMV_Failed, // non-specific error; WMVideoDecReset() and try again at next keyframe.
    WMV_BadMemory, // Catastrophic error; close codec and reinit before playing more content.
    WMV_NoKeyFrameDecoded, // A keyframe must be the first frame after starting or a reset.  WMVideoDecReset(), seek to a keyframe & continue.
    WMV_CorruptedBits, // Corrupt bitstream;  WMVideoDecReset() then seek to the next keyframe and try again.
    WMV_UnSupportedOutputPixelFormat, // Try another color space (we generally like YV12, YUY2, and some RGB formats but not all WMV profiles map to all color spaces)
    WMV_UnSupportedCompressedFormat, // Either FourCC or internal stream info indicates we can't play this clip.
    WMV_InValidArguments, // Bad Arguement (can also occur when memory is corrupt).
    WMV_BadSource, // N/A in a production decoder -- treat as a catastrophic error.

    WMV_NoMoreOutput, // WMVideoDecGetOutput called when no output is available.  Don't update screen but OK to continue to decode
    WMV_EndOfFrame, // WMVDecCBGetData returns this when there is no more data available for this frame.
    WMV_BrokenFrame, // Decoder thinks more data is needed but no more is available, treat as WMV_CorruptedBits

    WMV_UnSupportedTransform // Returned by the CallBack if an indicated external output transform is not supported in hardware
    // this is a request to the Codec to do the output transform inside the codec as part of GetOutput.
};

enum tagCodecVersion
{
    MMIDRATE4S =0,
    MPG4,
    MMIDRATE42,
    MMIDRATE43,
    WMV1,
    WMV2,
    WMV3,
    WMVA,
    UNKNOW_TYPE
};

enum tagPictureType
{
    IVOP = 0,
    PVOP,
    BVOP,
    BIVOP,
    SKIPFRAME
};

enum tagFrameCodingMode
{
    PROGRESSIVE = 0,
    INTERLACEFRAME,
    INTERLACEFIELD
};

enum
{
    NORM2 = 0,
    NORM6,
    ROWSKIP,
    COLSKIP,
    DIFF2,
    DIFF6,
    RAW
};

enum tagMbQtProfile
{
    ALL_4EDGES = 0,
    DOUBLE_EDGES,
    SINGLE_EDGES,
    ALL_MBS
};

enum tagMvRangeIndex
{
    MVRANGE0 = 0,
    MVRANGE1,
    MVRANGE2,
    MVRANGE3
};

enum tagMvMode
{
    MIXED_MV,
    ALL_1MV,
    ALL_1MV_HALFPEL,
    ALL_1MV_HALFPEL_BILINEAR,
    INTENSITY_COMPENSATION
};

enum
{
    NOT_WMV3 = -1,
    WMV3_SIMPLE_PROFILE,
    WMV3_MAIN_PROFILE,
    WMV3_PC_PROFILE,
    WMV3_ADVANCED_PROFILE,
    WMV3_SCREEN
};

enum SKIPBITCODINGMODE
{
    Normal = 0,
    RowPredict,
    ColPredict
};

enum
{
    LowRate = 1,
    MidRate,
    HighRate
};

enum
{
    XFORMMODE_8x8,
    XFORMMODE_8x4,
    XFORMMODE_4x8,
    XFORMMODE_MBSWITCH/* pseudo-mode */,
    XFORMMODE_4x4
};

enum Vop_Coding_Type
{
    I_VOP,
    P_VOP,
    B_VOP,
    S_VOP
};

/************************/
/* variables definition */
/************************/
// vparser variables


/**********/
/* struct */
/**********/
typedef struct TPicInf
{
    BOOL fgMpeg1;
    BOOL fgNtsc;
    BOOL fg16_9;
    UINT8 bFrameRate;
    UINT32 dwDispMd; // 0: PS and LB, 1: PS only, 2: LB only
    UINT32 dwW;
    UINT32 dwH;
    UINT32 dwTargetW;
    UINT32 dwTargetH;
    UINT32 dwTargetXOff;
    UINT32 dwTargetYOff;
} TPicInf;

typedef struct TFrmBufInf
{
    UINT8 bPicCdTp;
    UINT8 bPicStruct;
    UINT8 bStatus;
    UINT32 dwPicPSXOff; // for Pan Scan
    UINT32 dwFlag;
    UINT32 dwTemporalRef;
    UINT32 dwPts;
    UINT32 dwLba;
    UINT32 dwTimeCode;
} TFrmBufInf;

typedef struct TVSyncPrm
{
    UINT32 dwPtrY;
    UINT32 dwPtrC;
} TVSyncPrm;

typedef struct tIcomp
{
    int iScale;
    int iShift;
    int iEnable;
} Icomp;

typedef struct tIcompSet
{
    Icomp New;
    Icomp Old;
} IcompSet;

typedef struct tIcompAll
{
    IcompSet OldTop;//Field;
    IcompSet NewTop;//Field;
    IcompSet OldBot;//Field;
    IcompSet NewBot;//Field;
} CompSet;

typedef struct _DQuantDecParam
{
    UINT32 iDoubleStepSize;
    UINT32 iStepMinusStepIsEven;
    UINT32 i2DoublePlusStepSize;
    UINT32 i2DoublePlusStepSizeNeg;
    UINT32 iDCStepSize; // For Intra
} DQuantDecParam;

typedef struct _CMVScale
{
    UINT32 iMaxZone1ScaledFarMVX;
    UINT32 iMaxZone1ScaledFarMVY;
    UINT32 iZone1OffsetScaledFarMVX;
    UINT32 iZone1OffsetScaledFarMVY;
    UINT32 iFarFieldScale1;
    UINT32 iFarFieldScale2;
    UINT32 iNearFieldScale;
} CMVScale;

// ilii, TODO, Outer to see, like VDEC_DISP_PIC_INFO_T in 264
typedef struct
{
    UINT32 iFrmWidthSrc;
    UINT32 iFrmHeightSrc;
    UINT32 iWidthDec;
    UINT32 iHeightDec;
    UINT32 dwNumMBX;
    UINT32 dwNumMBY;
} MULTIRES_PARAMS;

typedef struct
{
    UINT32 dwPanScanHorizOffset;
    UINT32 dwPanScanVertOffset;
    UINT32 dwPanScanWidth;
    UINT32 dwPanScanHeight;
} PanScanWindowInfo;

typedef struct tagPicType
{
    UINT8 bPicType;
    UINT32 dwBits;
} tPICTYPE;
/*
 typedef struct TVSyncPrm
 {
 UINT32 dwPtrY;
 UINT32 dwPtrC;
 } TVSyncPrm;
 */

// Sequence layer bitstream
typedef struct SequenceLayerBitstream
{
    UINT32 i4Profile;
    UINT32 i4WMV3Profile;
    BOOL fgVC1;
    UINT32 i4FrameRate;
    UINT32 i4BitRate;
    BOOL fgPostProcInfoPresent;
    BOOL fgBroadcastFlags; //PULLDOWN in spec
    BOOL fgInterlacedSource;
    BOOL fgTemporalFrmCntr;
    BOOL fgSeqFrameInterpolation;
    BOOL fgProgressiveDisp;//PSF in spec
    UINT32 i4Aspect_Ratio;
    UINT32 i4Aspect_Horiz_Size;
    UINT32 i4Aspect_Vert_Size;
    UINT32 i4FrameRateNr;
    UINT32 i4FrameRateDr;
    UINT32 i4FrameRateExp;
    BOOL fgHRD_PARAM_FLAG;
    UINT32 i4hrd_num_leaky_buckets;
} SeqLayer;

// Simple & Main Profile
typedef struct SimpleMainProfile
{
    BOOL fgYUV411;
    BOOL fgSpriteMode;
    BOOL fgXintra8Switch;
    BOOL fgMultiresEnabled;
    UINT32 i4ResIndex;
    BOOL fgDCTTable_MB_ENABLED;
    BOOL fgPreProcRange;
    UINT32 i4NumBFrames;
    BOOL fgRotatedIdct;
    // WMV7 & WMV8
    BOOL fgMixedPel;
    BOOL fgFrmHybridMVOn;
    BOOL fgXintra8;
    UINT32 i4SliceCode;
    BOOL fgSKIPBIT_CODING;
    BOOL fgNEW_PCBPCY_TABLE;
    BOOL fgCODFlagOn;
    UINT32 i4skip_bit_mode_v87;
    UINT32 i4wmv8_bp_mode;
    UINT32 i4SkipBitCodingMode;
    UINT32 i4HufNewPCBPCYDec;
} SMProfile;

// Sequence header related
typedef struct SequenceHeaderRelated
{
    UINT32 u4MaxCodedWidth;
    UINT32 u4MaxCodedHeight;
    UINT32 u4MaxPicWidthSrc;
    UINT32 u4MaxPicHeightSrc;
    UINT32 u4PicWidthSrc;
    UINT32 u4PicHeightSrc;
    UINT32 u4PrevPicWidthSrc;
    UINT32 u4PrevPicHeightSrc;
    UINT32 u4PicWidthCmp;
    UINT32 u4PicHeightCmp;
    UINT32 u4PicWidthDec;
    UINT32 u4PicHeightDec;
    UINT32 u4NumMBX;
    UINT32 u4NumMBY;
    MULTIRES_PARAMS rMultiResParams[4];
} SeqHdrR;

// Entry-point layer bitstream for Advanced Profile
typedef struct EntryPointLayer
{
    BOOL fgBrokenLink;
    BOOL fgClosedEntryPoint;
    BOOL fgPanScanPresent;
    BOOL fgRefDistPresent;
    UINT32 i4RefFrameDistance;
    BOOL fgLoopFilter;
    BOOL fgUVHpelBilinear;
    UINT32 i4RangeState;
    UINT32 i4ReconRangeState; // WMVA PROGRESSIVE doesn't use this.
    BOOL fgExtendedMvMode;
    UINT32 i4MVRangeIndex;
    UINT32 i4DQuantCodingOn;
    BOOL fgXformSwitch;
    BOOL fgSequenceOverlap;
    BOOL fgExplicitSeqQuantizer;
    BOOL fgExplicitFrameQuantizer;
    BOOL fgExplicitQuantizer;
    BOOL fgNewDCQuant;
    UINT32 u2Coded_Width;
    UINT32 u2Coded_Height;
    BOOL fgExtendedDeltaMvMode;
    UINT32 i4DeltaMVRangeIndex;
    UINT32 i4ExtendedDMVX;
    UINT32 i4ExtendedDMVY;
    BOOL fgRangeRedY_Flag;
    BOOL fgRangeRedUV_Flag;
    UINT32 i4RangeRedY;
    UINT32 i4Range_MapUV;
} EntryPoint;

// Picture layer bitstream for Advance Profile
typedef struct PictureLayerRelatedOfAdvanceProfile
{
    UINT8 u1FrameCodingMode; // (FCM)

    //Interlaced
    BOOL fgInterlaceV2;
    BOOL fgFieldMode;
    UINT32 i4CurrentField; // 0:TOP, 1:BOTTOM field
    UINT32 i4CurrentTemporalField; // 0:1st field or frame picture, 1: 2nd field

    UINT8 u1PicType; // (PTYPE)
    UINT8 u1FirstFieldType;
    UINT8 u1SecondFieldType;
    UINT8 u1PrevPicType;

    UINT32 i4TemporalRef; // (TFCNTR)

    UINT32 i4MaxZone1ScaledFarMVX;
    UINT32 i4MaxZone1ScaledFarMVY;
    UINT32 i4Zone1OffsetScaledFarMVX;
    UINT32 i4Zone1OffsetScaledFarMVY;
    UINT32 i4FarFieldScale1;
    UINT32 i4FarFieldScale2;
    UINT32 i4NearFieldScale;

    UINT32 i4MaxZone1ScaledFarBackMVX;
    UINT32 i4MaxZone1ScaledFarBackMVY;
    UINT32 i4Zone1OffsetScaledFarBackMVX;
    UINT32 i4Zone1OffsetScaledFarBackMVY;
    UINT32 i4FarFieldScaleBack1;
    UINT32 i4FarFieldScaleBack2;
    UINT32 i4NearFieldScaleBack;

    BOOL fgTwoRefPictures;
    BOOL fgUseSameFieldForRef;
    BOOL fgUseOppFieldForRef;
    BOOL fgUseMostRecentFieldForRef;
    BOOL fgBackRefUsedHalfPel;
    BOOL fgBackRefTopFieldHalfPelMode;
    BOOL fgBackRefBottomFieldHalfPelMode;
    BOOL fgMvResolution;
    BOOL fgTopFieldFirst; // (TFF)
    BOOL fgRepeatFirstField; // (RFF)
    UINT32 u1RepeatFrameCount; // (RPTFRM)
    PanScanWindowInfo sWindowInfo[MAXPANSCANWINDOWS]; // (PS_HOFFSET, PS_VOFFSET, PS_WIDTH, PS_HEIGHT)
    //local EXTERN BOOL _fgRndCtrlOn;
    UINT32 i4RndCtrl;
    BOOL fgPostRC1;

    UINT32 i4BNumerator;
    UINT32 i4BDenominator;
    UINT32 i4BFrameReciprocal;
    UINT32 i4PicQtIdx; // (PQINDEX)
    UINT32 i4StepSize;
    UINT32 i4DCStepSize;
    UINT32 i4Overlap;

    DQuantDecParam *prDQuantParam;
    DQuantDecParam rDQuantParam3QPDeadzone[64];
    DQuantDecParam rDQuantParam5QPDeadzone[64];

    BOOL fgHalfStep; // (HALFQP)
    BOOL fgUse3QPDZQuantizer; // (PQUANTIZER)

    UINT32 u4DCTACInterTableIndx; // (TRANSACFRM)
    UINT32 u4DCTACIntraTableIndx; // (TRANSACFRM2)
    BOOL fgIntraDCTDCTable; // (TRANSDCTAB)
    BOOL fgDCTTable_MB;

    UINT32 i4SlicePicHeaderNum;//rWMVDecInfo.rPicLayer.i4SlicePicHeaderNum;
    UINT32 i4SlicePicHeaderNumField;//rWMVDecInfo.rPicLayer.i4SlicePicHeaderNum_field;
} PicLayer;

// Other?
typedef struct OtherVariables
{
    //WMV7 & WMV8
    BOOL fgDCPred_IMBInPFrame;
    // add later
    UINT8 u1BPMask[MAX_MBY][MAX_MBX]; // 720x576 --> 36 MB-row x 45 MB-col
    UINT64 hyBP[MAX_MBY*2]; // reconstructed bitplane unsigned __int64 rWMVDecInfo.rOther.hyBP[MAX_MBY];
    UINT8 u1DQuantBiLevelStepSize;
    BOOL fgDQuantOn;
    UINT32 i4Panning;
    UINT8 u1DiffQtProfile;
    BOOL fgDQuantBiLevel;
    UINT32 i4X9MVMode;
    INT32 i4LumScale;       // megaa 20091216: change the following 6 lines of UINT32 to INT32 to
    INT32 i4LumShift;       //                 avoid arithmetic error of > or < against INT values
    INT32 i4LumScaleTop;
    INT32 i4LumScaleBottom;
    INT32 i4LumShiftTop;
    INT32 i4LumShiftBottom;
    UINT32 i4MBModeTable;
    UINT32 i4MvTable;
    UINT32 i4CBPTable;
    UINT32 i42MVBPTable;
    UINT32 i44MVBPTable;
    BOOL fgMBXformSwitching;
    UINT32 i4FrameXformMode;
    UINT32 u4CodecFOURCC;
    UINT32 i4CodecVersion;
    UINT32 u4FileLen;
    UINT32 u4DispBufIdx;
    UINT32 u4BBufIdx;
    UINT32 u4DecPicNo;
    UINT32 u4DecTime;
    UINT32 u4SetDecTime;
    UINT32 u4wSpecificFrame;
    UINT32 u4VldPosByte;
    UINT32 u4VldPosBit;

    BOOL fgLuminanceWarp;
    BOOL fgLuminanceWarpTop;
    BOOL fgLuminanceWarpBottom;
    //For RCV file format, ilii, should be outer send it in. i.e. shouldn't exist here.
    UINT32 u4RcvNumFrames;
    UINT32 u4CodecVersion;
    UINT32 u4RCVVersion;
    UINT32 u4HeaderLen;
    UINT32 i4PIC_HORIZ_SIZE;
    UINT32 i4PIC_VERT_SIZE;
    UINT32 i4hdrext;
    UINT32 i4SeqHdrData1, i4SeqHdrData2, i4SeqHdrDataLen;
    UINT32 i4SetPos;
    UINT32 u4BitCount;
    UINT32 u4LastStartPos;

    UINT8 u1PreProcessFrameStatus;
    /*multi resolution*/
    //MULTIRES_PARAMS* pMultiResParams;
    /* Intensity compensation */
    //in jup.c
    UINT32 i4BoundaryUMVIcomp;
    UINT32 i4SecondFieldParity;
    UINT32 i4BoundaryUMVIcompEnable;
    UINT32 i4FirstFieldIntensityComp;
    UINT8 u1FrameTypeLast;
    UINT8 u1FrameTypeLastTop;
    UINT8 u1FrameTypeLastBot;
    UINT32 u4ResetMVDram;
    IcompSet rOldTopField;
    IcompSet rNewTopField;
    IcompSet rOldBotField;
    IcompSet rNewBotField;

    BOOL fgFirstFrame;
    BOOL fgEnCalPTS;
    BOOL fgFirstDispFrame;
    UINT32 u4FrameRate;
    UINT32 u4DeltaPTS;
    UINT32 u4LastPTS;
    //debug and file info
    UINT32 u4Test01;
    UINT32 u4FileEnd;
    //in vprs_wmv.c
    BOOL fgCounting;
    UINT32 i4PicHdrBits;
    //for vparser.c
    UINT32 u4new_entry_point;
    UINT32 i4pred_use_wdle;
    UINT32 u4Datain;
    UINT32 u4GetVal;

    //for fbm
    UINT8 u1FbgId;
    UINT8 u1FbId0;
    UINT8 u1FbId1;
    UINT8 u1FbId2;
    UINT8 u1GetSeqHdr;
    UINT8 u1GetPicHdr;

    //for emulation
    UINT8 u1DecState;
    UINT32 i4VprErr; //error message
    UINT32 u4Dummy;
    BOOL fgDumpSimPat;
    UINT16 u4repeatframe;
    UINT32 u4CurrPicNo;
    TPicInf rPicInfo;
    //TFrmBufInf _ptFrmBuf[3];
    TVSyncPrm rVSyncPrm;
    BOOL fgSkipFrame;
    BOOL fgChangeResolution;
    //for hw and interrupt
    UINT32 u4BPRawFlag;
    BOOL fgRet;
    HANDLE_T hVdecFinSema;

    //for dmx
    UINT32 u4WPtr;
    BOOL fgSWFindStartCode;

    // for new mode
    BOOL   fgNewMode;
    UINT32 use_quarter_pel_mv;
    UINT32 use_rule_b;
    INT32  i4FwdRefDist;
    INT32  i4BwdRefDist;
} Others;

// Dram Related
typedef struct DramRelated
{
    //TVSyncPrm rWMVDecInfo.rOther.rVSyncPrm;

    UINT32 u4DecBufIdx; //decode buffer index
    UINT32 u4BRefBufIdx; //reference frame1 buffer index
    UINT32 u4FRefBufIdx; //reference frame2 buffer index

    UINT32 u4ForwardRefPicType;
    UINT32 u4BackwardRefPicType;

    UINT8 *pu1VFifo; //fifo start
    UINT8 *pu1FifoEnd; // fifo end
    UINT8 *pu1Pic0Y; // Video Output Pic 0, Ref0
    UINT8 *pu1Pic0C; // Video Output Pic 0, Ref0
    UINT8 *pu1Pic1Y; // Video Output Pic 1, Ref1
    UINT8 *pu1Pic1C; // Video Output Pic 1, Ref1
    UINT8 *pu1Pic2Y; // Video Output Pic 2, B
    UINT8 *pu1Pic2C; // Video Output Pic 2, B
    UINT8 *pu1Pic3Y; // Video Output Pic 3, B
    UINT8 *pu1Pic3C; // Video Output Pic 3, B

    UINT8 *pu1Dcac;
    UINT8 *pu1Mv_1;
    UINT8 *pu1Mv_2;
    UINT8 *pu1Bp_1;
    UINT8 *pu1Bp_2;
    UINT8 *pu1Bp_3;
    UINT8 *pu1Bp_4;
    UINT8 *pu1Mv_3;
    UINT8 *pu1Mv_1_2;
    UINT8 *pu1Dcac_2;
    UINT8 *pu1Pp_1;
    UINT8 *pu1Pp_2;
} Dram;

typedef struct
{
    VDEC_PES_INFO_T rPesInfo;

    SeqLayer rSeqLayer;
    SMProfile rSMProfile;
    SeqHdrR rSeqHdrR;
    EntryPoint rEntryPoint;
    PicLayer rPicLayer;
    Others rOther;
    Dram rDram;
    CompSet rComp;
} VDEC_WMV_INFO_T;

//temp method
extern VDEC_WMV_INFO_T rWMVDecInfo;

/*
 #ifdef _GLOBAL_
 int _iStepRemap[32] = { NO_USE,  1,  2,  3,  4,  5,  6,  7,  // (PQUANT)
 8,  6,  7,  8,  9, 10, 11, 12,
 13, 14, 15, 16, 17, 18, 19, 20,
 21, 22, 23, 24, 25, 27, 29, 31  };
 int _iNumShortVLC[7] = {1, 1, 2, 1, 3, 1, 2};
 int _iDenShortVLC[7] = {2, 3, 3, 4, 4, 5, 5};
 int _iNumLongVLC[14] = {3, 4, 1, 5, 1, 2, 3, 4, 5, 6, 1, 3, 5, 7};
 int _iDenLongVLC[14] = {5, 5, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8};
 int _iBInverse[8] = { 256, 128, 85, 64, 51, 43, 37, 32 };
 #else
 EXTERN int _iStepRemap[32];
 EXTERN int _iNumShortVLC[7];
 EXTERN int _iDenShortVLC[7];
 EXTERN int _iNumLongVLC[14];
 EXTERN int _iDenLongVLC[14];
 EXTERN int _iBInverse[8];
 #endif
 */

#ifndef MAKEFOURCC_WMV
#define MAKEFOURCC_WMV(ch0, ch1, ch2, ch3) \
        ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) |   \
        ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))

#define mmioFOURCC_WMV(ch0, ch1, ch2, ch3)  MAKEFOURCC_WMV(ch0, ch1, ch2, ch3)
#endif
/******* video output type guids, in preference order  *****/
#define FOURCC_WVC1_WMV     mmioFOURCC_WMV('W','V','C','1')
#define FOURCC_WMVA_WMV     mmioFOURCC_WMV('W','M','V','A')
#define FOURCC_wmva_WMV     mmioFOURCC_WMV('w','m','v','a')
#define FOURCC_WMVP_WMV     mmioFOURCC_WMV('W','M','V','P')
#define FOURCC_wmvp_WMV     mmioFOURCC_WMV('w','m','v','p')
#define FOURCC_WMV3_WMV     mmioFOURCC_WMV('W','M','V','3')
#define FOURCC_wmv3_WMV     mmioFOURCC_WMV('w','m','v','3')
#define FOURCC_WMV2_WMV     mmioFOURCC_WMV('W','M','V','2')
#define FOURCC_wmv2_WMV     mmioFOURCC_WMV('w','m','v','2')
#define FOURCC_WMV1_WMV     mmioFOURCC_WMV('W','M','V','1')
#define FOURCC_wmv1_WMV     mmioFOURCC_WMV('w','m','v','1')
#define FOURCC_M4S2_WMV     mmioFOURCC_WMV('M','4','S','2')
#define FOURCC_m4s2_WMV     mmioFOURCC_WMV('m','4','s','2')
#define FOURCC_MP43_WMV     mmioFOURCC_WMV('M','P','4','3')
#define FOURCC_mp43_WMV     mmioFOURCC_WMV('m','p','4','3')
#define FOURCC_MP4S_WMV     mmioFOURCC_WMV('M','P','4','S')
#define FOURCC_mp4s_WMV     mmioFOURCC_WMV('m','p','4','s')
#define FOURCC_MP42_WMV     mmioFOURCC_WMV('M','P','4','2')
#define FOURCC_mp42_WMV     mmioFOURCC_WMV('m','p','4','2')

#define vSetDecBuf(arg)       rWMVDecInfo.rDram.u4DecBufIdx = (arg)
#define vSetBRefBuf(arg)      rWMVDecInfo.rDram.u4BRefBufIdx = (arg)
#define vSetFRefBuf(arg)      rWMVDecInfo.rDram.u4FRefBufIdx = (arg)

//#define fgIsIPic(Idx)         (_ptFrmBuf[Idx].bPicCdTp == I_VOP)

#endif //VDEC_WMVUTIL_H
