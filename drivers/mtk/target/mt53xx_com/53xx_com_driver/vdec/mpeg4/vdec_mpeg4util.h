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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: vdec_mpeg4util.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file vdec_mpeg4util.h
 *  This header file declares exported APIs of mpeg4 decoder.
 */

#ifndef VDEC_MPEG4UTIL_H
#define VDEC_MPEG4UTIL_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "x_lint.h"
#include "vdec_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "../vdec_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/*#define DCAC_SZ               ((((720 / 16) * 4) * (4)) * 4)
#define MVEC_SZ               (((720 / 16) * 4 * (576 / 16)) * 4)
#define BMB1_SZ               (((720 / 16) * 4 * (576 / 16)) * 4)
#define BMB2_SZ               (((720 / 16) * 4 * (576 / 16)) * 4)
#define BCODE_SZ              (((720 / 16) * 2) * 4)*/

#define MPEG4_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))

#define MP4_DCAC_SZ               ((((1920 / 16) * 4) * (4)) * 4)
#define MP4_MVEC_SZ               (((1920 / 16) * 4 * (1088 / 16)) * 4)
#define MP4_BMB1_SZ               (((1920 / 16) * 4 * (1088 / 16)) * 4)
#define MP4_BMB2_SZ               (((1920 / 16) * 4 * (1088 / 16)) * 4)
#define MP4_BCODE_SZ              (((1920 / 16) * 2) * 4)


#define M4V_START_CODE			0x000001	
#define H263_START_CODE		0x000080

#define M4V_VOL_START_CODE_MIN	0x00000120
#define M4V_VOL_START_CODE_MAX	0x0000012F
#define M4V_VOP_START_CODE		0x000001B6
#define M4V_USER_DATA_START_CODE	0x000001B2

#define H263_VIDEO_START_MASK            	0xfffffc00
#define H263_VIDEO_START_CODE          	0x00008000

#define FLV_VIDEO_START_CODE 0x00008400

#define DX311_VIDEO_START_CODE		0x80020000

#define VIDEO_TYPE_H263     		0x00
#define VIDEO_TYPE_DX311    		0x01
#define VIDEO_TYPE_ISO_MPEG4	0x02
#define VIDEO_TYPE_DIVX_MPEG4	0x04
#define VIDEO_TYPE_FLV                0x08
#define VIDEO_TYPE_UNKNOWN	0x10

#define VOP_CODING_TYPE_I		0x00
#define VOP_CODING_TYPE_P		0x01
#define VOP_CODING_TYPE_B		0x02
#define VOP_CODING_TYPE_S		0x03

#define MP4_TOP_FLD_PIC    1
#define MP4_BTM_FLD_PIC    2
#define MP4_FRM_PIC            3

#define BARREL1 1
#define BARREL2 2

#define DPB_ADDR 0x179c000
#define DPB_SIZE  0x01000000	//size (12288*1024)

#define MP4_MC_TOP_FLD              0
#define MP4_MC_BOTTOM_FLD       1

#define MP4_MC_2ND_FLD_OFF     0
#define MP4_MC_2ND_FLD_ON       1

#define FBufInfoSz 4

#define MP4_REF_BUF_0 		0
#define MP4_REF_BUF_1 		1
#define MP4_B_BUF_0           	2
#define MP4_B_BUF_1              	3
#define MP4_MC_DBK_BUF      4

#define DEC_FLG_MP4_2ND_FLD_PIC   (0x1 << 0)

#define DEC_RETRY_NUM 500000

#define VLD_MPEG4_VECTOR    24

#define MPEG4_TIME_SCALE 90000

enum
{
    MP4_T32_NOP,
    MP4_T32_EXEC,
    MP4_T32_RESTART,
    MP4_T32_EXIT,
    MP4_T32_EXEC_RELOAD_SYM,
    MP4_T32_END
};

//#define CC_MPEG4_DEBLOCKING

//#define IC_MPEG4_VERIFICATION

#define MP4_BLK_ALIGMENT         0x3F

#if defined(CC_MT5363) || defined(CC_MT5365) || defined(CC_MT5395)
#define CC_IS_MPEG4_POST_MT5363
#endif
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MPEG4_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef void (*pfnPrsHdrFunc)(VDEC_HDLR hVdec, INT32* pi4RetValue);
typedef void (*pfnDecInitFunc)(VDEC_HDLR hVdec, INT32* pi4RetValue);

typedef struct
{ 
	UINT8 u1PicCodingType; // Picture Coding Type (I, P, B)
	UINT8 u1PicStructure; // Picture Structure (frame or field) 

	// Forward and Backward Reference Buffer Index
	UINT8 u1FRefBufIdx; 
	UINT8 u1BRefBufIdx;

	// Current Frame Number
	UINT32 u4FrameNum;
	  
	// Picture Width and Height
	UINT32 u4W;
	UINT32 u4H;  

	// Buffer Address
	UINT32 u4BufAddr;

}VDEC_MPEG4_FBUF_INFO_T;

typedef struct
{
  UINT8   u1AltIAcChromDct;
  UINT8   u1AltIAcChromDctIdx;
  UINT8   u1AltIAcLumDct;
  UINT8   u1AltIAcLumDctIdx;
  UINT8   u1AltIDcDct;
  UINT8   u1HasSkip;
  UINT8   u1AltPAcDct;
  UINT8   u1AltPAcDctIdx;
  UINT8   u1AltPDcDct;
  UINT8   u1AltMv;
  UINT8   u1FrameMode;
  BOOL    fgSwitchRounding;
} VDEC_DX311_PIC_INFO_T;

typedef struct 
{
  UINT32  u4Trd;
  UINT32  u4Trb;
  UINT32  u4Trdi;
  UINT32  u4Trbi;
  UINT32  u4TFrm;
} VDEC_M4V_DIRECT_MODE_INFO_T;

typedef struct 
{
  INT32   i4GmcYMvX;
  INT32   i4GmcYMvY;
  INT32   i4GmcCMvX;
  INT32   i4GmcCMvY;
} VDEC_M4V_GMC_INFO_T;

typedef struct 
{
  BOOL    fgShortHeader;

  UINT32	u4DivxCodecVersion;
  UINT32	u4UserDataBuildNumber;

  UINT8   u1VideoTypeIndication;
  UINT8   u1VideoObjectLayerVerid;
  
  UINT8   u1AspectRatioInfo;
  
  UINT32 u4TimeIncrementResolution;
  UINT8   u1VopTimeIncrementResolutionBits;

  UINT16 u2FixedVopTimeIncrement;
 
  BOOL    fgInterlaced;
  BOOL	fgObmcDisable;

  UINT8   u1QuantPrecision;
	
  UINT8	u1SpriteEnable;
  UINT8   u1NoOfSpriteWarpingPoints;
  UINT8   u1SpriteWarpingAccuracy;
  
  UINT8   u1QuantType;
  BOOL    fgLoadIntraQuantMat;
  UINT8   au1IntraQuantMat[64];
  BOOL    fgLoadNonIntraQuantMat;
  UINT8   au1NonIntraQuantMat[64];
  
  BOOL    fgQuarterSample;
  
  BOOL    fgResyncMarkerDisable;
  BOOL    fgDataPartitioned;
  BOOL    fgReversibleVlc;

  BOOL	fgReducedResolutionVopEnable;
} VDEC_M4V_VOL_INFO_T;

typedef struct 
{
    UINT8   u1IntraDcVlcThr;

    BOOL    fgTopFldFirst;
    BOOL	fgAlternateVerticalScanFlag;

    UINT8   u1ForwardFCode;
    UINT8   u1BackwardFCode;

    UINT16 u2VopTimeIncrement;
    UINT16 u2VopPrevTimeIncrement;

    INT32  	ppi4WarpingMv[4][2];

    UINT8	u1EffectiveWarpingPoints;

    BOOL	fgVopReducedResolution;

    VDEC_M4V_GMC_INFO_T   rGmcParam;

    // For Direct Mode
    VDEC_M4V_DIRECT_MODE_INFO_T   rDirectMd;   

    // For Calculating Prediction
    UINT32	u4TimeBase;
    UINT32	u4RefPicTimeBase;
    UINT32 	u4PrevDispTime;
    UINT32	u4CurDispTime;
    UINT32	u4NextDispTime;


    // For Short Header
    UINT8   u1SplitScreenIndicator;
    UINT8	u1DocCameraIndicator;
    UINT8	u1FullPicFreezeRelease;
    UINT8	u1SourceFormat;

} VDEC_M4V_PIC_INFO_T;

typedef struct
{
  VDEC_M4V_VOL_INFO_T  	rVol;
  VDEC_M4V_PIC_INFO_T   	rVop;
} VDEC_M4V_DEC_PARAM_INFO_T;

typedef struct
{
    BOOL    fgPicQueue;
    BOOL    fgPesQueue;

    BOOL    fgDecError;
    UINT32 u4VopNotCoded;

    UINT8	u1FbId;
    UINT8	u1FbgId;
    UINT8   u1FbgType;
    UINT8 u1DbkFbId;

    UINT8   u1WorkingBufFbId;
    UINT8   u1WorkingBufFbgId;

    UINT32 u4DCACBufAddr;
    UINT32 u4MVECBufAddr;
    UINT32 u4BMB1BufAddr;
    UINT32 u4BMB2BufAddr;
    UINT32 u4BCODEBufAddr;

    UINT32 u4PrevPrsPtr;

    UINT32 u4DecFlag;

    UINT32 u4RefPts;
    UINT64 u8RecOffset;
    UINT64 u8RecOffsetI;

    BOOL fgEnCalPts;
    BOOL fgValidPts;
    UINT32 u4DispPts;

    UINT64 u8Remainder;
    UINT64 u8PtsScale;
    UINT32 u4Rate;

    UINT16 u2Width;
    UINT16 u2Height;

    UINT32  u4DramWidth;
    UINT32  u4DramHeight;

    UINT32  u4LineSize;

    UINT32 u4DecXOff;
    UINT32 u4DecYOff;

    UINT32 u4BufStart;

    UINT32 u4FrameCount;

    VDEC_PES_INFO_T rPesInfo;

    UINT8   u1VideoType;

    // Common for MPEG4 and DivX3
    UINT8	u1PicStructure;
    UINT8   u1VopCodingType;  
    UINT8	u1BackwardRefVopType;
    UINT8   u1VopQuant; 
    UINT8   u1VopRoundingType; 

    // For unrestricted mv
    UINT32    u4UmvPicW;
    UINT32    u4UmvPicH;
    UINT32    u4QPelType;
    UINT32    u4CMvType;

    // For deblocking usage
    //for DBK para test 
    BOOL fgDbkEngrMode;
    UINT32 u4P1; 
    UINT32 u4P2; 
    UINT32 u4P3; 
    UINT32 u4P4; 
    UINT32 u4P5; 
    UINT32 u4P6; 
    UINT32 u4P7; 
    UINT32 u4P8; 
    UINT32 u4P9; 
    UINT32 u4P10; 
    UINT32 u4P11; 
    UINT32 u4P12; 
    UINT32 u4P13; 
    UINT32 u4P14; 
    UINT32 u4P15; 
    UINT32 u4P16; 
    UINT32 u4P17; 
    UINT32 u4P18; 
    UINT32 u4P19; 
    UINT32 u4P20; 
    
    UINT16 u2DbkXStartT;
    UINT16 u2DbkWidthT;
    UINT16 u2DbkYStartT;
    UINT16 u2DbkHeightT;
    UINT32 u4DbkXStart;
    UINT32 u4DbkXEnd;
    UINT32 u4DbkYStart;
    UINT32 u4DbkYEnd;
        
    // for decoder buffer information
    VDEC_MPEG4_FBUF_INFO_T arFBufInfo[FBufInfoSz];

    // Function Pointer for Parser
    pfnPrsHdrFunc pfnPrsHdr;

    // Function Pointer for Decoder Initialization
    pfnDecInitFunc pfnDecInit;

    UCHAR *ucDecYPtr;
    UCHAR *ucDecCbCrPtr;

    UINT32 u4BitCount;
    UINT32 u4AddrMode;
    UINT32 u4WPtr;

    INT32 i4TemporalRef;

    VDEC_DX311_PIC_INFO_T	rDx3Params;
    VDEC_M4V_DEC_PARAM_INFO_T rM4vParams;
} VDEC_MPEG4_DEC_PARAM_INFO_T;

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#endif //VDEC_MPEG4UTIL_H

