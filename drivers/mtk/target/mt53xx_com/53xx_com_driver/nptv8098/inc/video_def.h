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
 *
 * $Author: dtvbm11 $
 * $Date  $
 * $RCSfile: video_def.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _VIDEO_DEF_H_
#define _VIDEO_DEF_H_

#include "typedef.h"
#include "feature.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_tv_dec.h"

#ifndef CC_DRIVER_PROGRAM
#include "d_drv_cust.h"
#endif
    LINT_EXT_HEADER_END
// *********************************************************************
// Flags to notify event
// *********************************************************************

 // Main Flags
#define MAIN_FLG_MODE_CHG (0x1L)
#define MAIN_FLG_MODE_DET_DONE (0x1L<<1)
#define MAIN_FLG_MODE_FIXCOLOR (0x1L<<2)
// Pip Flags
#define PIP_FLG_MODE_CHG (0x1L<<8)
#define PIP_FLG_MODE_DET_DONE (0x1L<<9)
#define PIP_FLG_MODE_FIXCOLOR (0x1L<<10)
//Scalar Flags
#define SCALER_MAIN_MODE_CHG  ((0x1L)<<14)
#define SCALER_PIP_MODE_CHG  ((0x1L)<<15)
#define MAIN_DISP_PRM_CHG ((0x1L)<<16)
#define PIP_DISP_PRM_CHG ((0x1L)<<17)
#define SCALE_PRM_CHG ((0x1L)<<18)
#define MAIN_3D_RES_CHG ((0x1L)<<19)
#define PIP_3D_RES_CHG ((0x1L)<<20)
#define MAIN_MUTECOLOR_CHG ((0x1L)<<21)
#define PIP_MUTECOLOR_CHG ((0x1L)<<22)
#define SCPOS_MODECHG ((0x1L)<<23)
#define MAIN_HV_CHG ((0x1L)<<24)
#define PIP_HV_CHG ((0x1L)<<25)
// DVI Event Flag
#define DVI_OUTPUTV (0x1L<<26)
#define DVI_DEBUG_MODE (0x1L<<27)


extern BOOL fgIsEventFlgSet(UINT32 u4arg);
extern VOID vSetEventFlg(UINT32 u4arg) reentrant;
extern VOID vClrEventFlg(UINT32 u4arg) reentrant;

extern BOOL fgIsMainFlgSet(UINT32 u4arg);
extern VOID vSetMainFlg(UINT32 u4arg);
extern VOID vClrMainFlg(UINT32 u4arg);

extern BOOL fgIsPipFlgSet(UINT32 u4arg);
extern VOID vSetPipFlg(UINT32 u4arg);
extern VOID vClrPipFlg(UINT32 u4arg);

extern BOOL fgIsScposFlgSet(UINT32 u4arg);
extern VOID vSetScposFlg(UINT32 u4arg);
extern VOID vClrScposFlg(UINT32 u4arg);

extern BOOL fgIsDviFlgSet(UINT32 u4arg);
extern VOID vSetDviFlg(UINT32 u4arg);
extern VOID vClrDviFlg(UINT32 u4arg);

#ifndef __MODEL_slt__
#define MAIN_FREEZE_DELAY	3
#define PIP_FREEZE_DELAY	3
#else
#define MAIN_FREEZE_DELAY	0
#define PIP_FREEZE_DELAY	0
#endif

#define SCALE_VTOTAL_DO_NOT_CARE 0xffff 
#define _MJC_VMAXMIN_DONOTCARE  0xFFFF



// *********************************************************************
// Extern structure declaration
// *********************************************************************

typedef struct RVDecStatus
{
    UINT8 bIsMain;
    UINT8 bIsPip;
} RVDecStatus;

  // *********************************************************************
  // Channel filter: general function and structure defined for Main/Pip
  // *********************************************************************
typedef struct RVChannel
{
    //UINT8 bStatus; should use a status to represent 8 UINT8 status information
    UINT8 bIsChannelOn;
    UINT8 bIsGoingToFreeze;
    UINT8 bIsFreeze;
    UINT8 bIs444;
    UINT8 bDecType;

    UINT16 u2_e_type;
    UINT16 u2_e_ui2_id;
    UINT32 u4_Cap;

#ifndef CC_DRIVER_PROGRAM
    DRV_COMP_ID_T t_comp_id;
    TV_DEC_CTRL_T t_Tvd_Ctrl;

    DRV_ID_T t_mon_id;          // Comp ID for current monitored video source
#endif
    UINT8 u1NeedNotify;
} RVChannel;

  // *********************************************************************
  // Display Related register value
  // *********************************************************************
  // Those register value that should be set is recorded in this struct, to avoid
  // Too Many calculation in VSync
typedef struct RScalerInfo
{
	UINT32 wMJCInputWidth;
	UINT32 wMJCInputHeight;
	
	UINT32 wMJCOutputXOffset;
	UINT32 wMJCOutputYOffset;
	UINT32 wMJCOutputWidth;
	UINT32 wMJCOutputHeight;
	UINT32 wMJCOutputVTotal;

} RScalerInfo;

typedef struct RDispPrm
{
    // For Disp Windodw
    UINT16 wHPos;               // HP
    UINT16 wVPos;               // VP
    UINT16 wHLen;               // HL
    UINT16 wVLen;               // VL    
    UINT16 wRHPos;              // RHP
    UINT16 wRVPos;              // RVP
    UINT16 wRHLen;              // RHL
    UINT16 wRVLen;              // RVL
    // Picture related
    UINT16 wHPosOfst;           // H Porch
    UINT16 wVPosOfst;           // V Porch
    UINT16 wOverscanRight;      // 3d overscan right side
    UINT16 wOverscanBottom;     // 3d overscan bottom side
    UINT16 wDataLen;            // Read Length  (review this in 6896, should be no use)

    UINT32 u4USclH;             // H Up Scaling Factor
    UINT32 u4USclV;             // V Up Scaling Factor
    UINT32 u4DSclH;             // H Down scaling Factor
    UINT32 u4DSclV;             // V Down scaling Factor
    
    UINT32 u4HBound;        // Bounary protection of width
    UINT32 u4VBound;            // Bounary protection of height
    
    UINT8  u1USclHInitSigned;   // H negative initial phase on/off
    UINT32 u4USclHInit;         // H negative initial phase value
    UINT32 u4USclHInitUV;
    
    UINT8  u1USclVInitSigned;   // V negative initial phase on/off
    UINT32 u4USclVInit;         // V negative initial phase value : top filed or frame
    UINT8  u1USclVInitSign_Btm;   // V negative initial phase on/off
    UINT32 u4USclVInit_Btm;         // V negative initial phase value : bottom field
    UINT8  u1USclV3DFieldEn;     // top/bottom field apply different init phase
    
    UINT8  u1MirrorYCMatch;     // Chroma match setting for mirror mode
    UINT8  u1Us422UVMirrorMode; 
    
    UINT8  u1HPaddingLen;       // the padding of each horizontal line (review this in 6896, should be no use. Cause there's no burst mode)
    #ifdef CC_SCPOS_3DTV_SUPPORT 
    UINT32 u4Tv3dWidth;
    INT8  i1LShift;
    INT8  i1RShift;
    #endif

    UINT16 u2DramWriteWidth;
    UINT16 u2DramWriteHeight;
    UINT16 u2DramReadWidth;
    UINT16 u2DramReadHeight;
    UINT16 u2DramLinePitch;    
} RDispPrm;

  // *********************************************************************
  // Display Related Information
  // *********************************************************************
/** Brief of VDP_REGION_T
 *  Video plane region (source/output).
 */
typedef struct
{
    UINT32 u4X;
    UINT32 u4Y;
    UINT32 u4Width;
    UINT32 u4Height;
} VDP_DISP_REGION_T;

/** Brief of VDP_OVERSCAN_REGION_T
 *  Video plane overscan region.
 */
typedef struct
{
    UINT32 u4Top;
    UINT32 u4Bottom;
    UINT32 u4Left;
    UINT32 u4Right;
} VDP_PIC_OVERSCAN_REGION_T;

typedef struct RDispInfo
{
    UINT16 wXOffset;
    UINT16 wYOffset;
    UINT16 wWidth;
    UINT16 wHeight;    
    
    VDP_DISP_REGION_T rOutRegion;
    
    UINT32 u4MuteColor;    // Mute color
} RDispInfo;

typedef struct RPicInfo
{
    UINT8 u1Interlace;                      //is interlace source
    UINT16 wHTotal;                         //(in pixel) Source H Total
    UINT16 wVTotal;                         //(in pixel) Source V Total
    UINT16 wSrcW;                           //(in pixel) Source original width
    UINT16 wSrcH;                           //(in pixel) Source original height
    
    VDP_DISP_REGION_T rSrcRegion;           //(0~10000)Source region
    VDP_PIC_OVERSCAN_REGION_T rOverScan;    //(0~10000)Overscan region
    
    UINT16 wXOffset;                        //(in pixel) X start point after doing overscan & source region
    UINT16 wYOffset;                        //(in pixel) Y start point after doing overscan & source region 
    UINT16 wTargetW;                        //(in pixel) Width after doing overscan & source region 
    UINT16 wTargetH;                        //(in pixel) Height after doing overscan & source region
    
    UINT16 wHPorch;                         // decoder porch, usually for workaround
    UINT16 wVPorch;                         // decoder porch, usually for workaround
    UINT16 wUIHPorch;                       // 1000 bias. less than 1000 means minus value.
    UINT16 wUIVPorch;                       // 1000 bias. less than 1000 means minus value.    
} RPicInfo;

// Peggy 20080128 for [DTV00022615]
typedef struct Disp_Tune
{
    UINT32 u4PanelW;
    UINT32 u4PanelH;
    UINT32 u4SrcW;
    UINT32 u4SrcH;
    UINT32 u4FrameRate;
    UINT32 u4Interlace;
    UINT32 u4USclVMin;
    UINT32 u4USclVMax;
    UINT32 u4DSclVMin;
    UINT32 u4DSclVMax;
    UINT32 VSP;
    UINT32 TUNE;
} Disp_Tune;

typedef struct
{
    UINT16 u4SrcWidth;
    UINT16 u4SrcHeight;    
    UINT32 u4SrcAddr;    
    UINT16 u4DesWidth;
    UINT16 u4DesHeight;    
    UINT32 u4DesAddrL;    
    UINT32 u4DesAddrR;	
    UINT32 u4SrcStartX;
    UINT32 u4SrcStartY;
    UINT32 u4Subsample;
    UINT32 u4DumpSize;
    UINT8 u4Index;
} DUMP_3D_INFO_T;


// *********************************************************************
// Video state machine define
// *********************************************************************
  /* Define for States of Video State Machine */
enum
{
    VDO_STATE_IDLE,
    VDO_STATE_WAIT_MODE_DET
};

  // define the auto state machine
enum
{
    VDO_AUTO_NOT_BEGIN,
    VDO_AUTO_POSITION_START,
    VDO_AUTO_POSITION_1_START,
    VDO_AUTO_POSITION_SET,
    VDO_AUTO_PHASE_START,
    VDO_AUTO_PHASE_1_START,
    VDO_AUTO_CLOCK_START,
    VDO_AUTO_CLOCK_1_START,
    VDO_AUTO_CLOCK_2_START
};

enum
{
    VDO_AUTO_COLOR_NOT_BEGIN,
    VDO_AUTO_COLOR_START,
    VDO_AUTO_COLOR_1_START,
    VDO_AUTO_COLOR_2_START,
    VDO_AUTO_COLOR_3_START,
    VDO_AUTO_COLOR_3_START_Delay1,
    VDO_AUTO_COLOR_4_START,
    VDO_AUTO_COLOR_5_START,
    VDO_AUTO_COLOR_CLAMP_DELAY,
    VDO_AUTO_COLOR_END,
    VDO_AUTO_COLOR_BLANK1,
    VDO_AUTO_COLOR_2_START_Interation1,
    VDO_AUTO_COLOR_2_START_Interation_End,
    VDO_AUTO_COLOR_1P0_START,
};

enum
{
    ON_CHIP_AUTO_COLOR_DO_NOTHING,
    ON_CHIP_AUTO_COLOR_INITIAL_STATE,
    ON_CHIP_AUTO_COLOR_SEARCH_TARGET,
    ON_CHIP_AUTO_COLOR_WAIT_VALUE,
    ON_CHIP_AUTO_COLOR_CHECK_VALUE,
    ON_CHIP_AUTO_COLOR_FINISH,
    ON_CHIP_AUTO_COLOR_END
};

enum
{
   BLK_NOTHING,
   BLK_START,
   BLK_LEVEL_STABLE,
   BLK_OFFSET_ADJ,
   BLK_OFFSET_DELAY,  // [SA7_Michael] 080828 for adjusting blanking level stable
   BLK_OFFSET_DELAY2,
   BLK_OFFSET_CHECK,
   BLK_GAIN_ADJ,
   BLK_DELAY,
   BLK_MEASURE,
   BLK_CLAMP_DELAY,
   BLK_APICMD,
   BLK_END  
};

enum
{
    ON_CHIP_GAIN_MODE,
    ON_CHIP_OFFSET_MODE,
    OFFSET_MODE,
    GAIN_MODE,
    DIGITAL_OFFSET_MODE
};

enum
{
    EXT_AUTO_COLOR_NOT_BEGIN,
    EXT_AUTO_COLOR_START,
    EXT_AUTO_COLOR_WAIT,
    EXT_AUTO_COLOR_OFFSETGAIN,
    // EXT_AUTO_COLOR_GAIN,
    EXT_AUTO_COLOR_END
};

enum
{
    INT_HDTV,
    INT_VGA,
    INT_SCART,
    INT_VGA_COMPOENT,
    EXT_HDTV,
    EXT_VGA
};

// *********************************************************************
// Global Variable Declaration
// *********************************************************************

// Used for Video State Machine for Main Loop
extern UINT8 _bMainState;
extern UINT8 _bPipState;

// Decoder Status
extern RVDecStatus _rTvd3dStat;
extern RVDecStatus _rVGAStat;
extern RVDecStatus _rYPBPRStat;
extern RVDecStatus _rDVIStat;

// add for 5371
extern RVDecStatus _rMpegSDStat;
extern RVDecStatus _rMpegHDStat;

// Main Channel
extern RVChannel _rMChannel;

// Pip Channel
extern RVChannel _rPChannel;

#define fgIsMainDec() (_rMChannel.bDecType)
#define fgIsPipDec() (_rPChannel.bDecType)
#define fgIsSrcDec(bPath) ((bPath == SV_VP_MAIN) ? fgIsMainDec() : fgIsPipDec())

#define fgIsMainTvd3d() (_rTvd3dStat.bIsMain)
#define fgIsPipTvd3d() (_rTvd3dStat.bIsPip)
#define fgIsMainVga() (_rVGAStat.bIsMain)
#define fgIsPipVga() (_rVGAStat.bIsPip)
#define fgIsMainYPbPr() (_rYPBPRStat.bIsMain)
#define fgIsPipYPbPr() (_rYPBPRStat.bIsPip)
#define fgIsMainDVI() (_rDVIStat.bIsMain)
#define fgIsPipDVI() (_rDVIStat.bIsPip)

#define fgIsMainDTV() ((_rMpegHDStat.bIsMain) || (_rMpegSDStat.bIsMain))
#define fgIsPipDTV() ((_rMpegHDStat.bIsPip) || (_rMpegSDStat.bIsPip))
#define fgIsSrcDTV(bPath) ((bPath == SV_VP_MAIN) ? fgIsMainDTV() : fgIsPipDTV())

#define fgIsMainScart() (VSS_MAJOR(_bSrcMainNew) == VSS_SCART)
#define fgIsPipScart() (VSS_MAJOR(_bSrcSubNew) == VSS_SCART)


#if SUPPORT_POP
#define bGetVideoDecType(bPath) (((bPath)==SV_VP_MAIN) ? _rMChannel.bDecType : _rPChannel.bDecType)
#else
#define bGetVideoDecType(bPath) (((bPath)<=SV_VP_PIP) ? _rMChannel.bDecType : SV_VD_MAX)
#endif

// Display related information
extern RDispInfo _rMDispInfo;
extern RDispInfo _rPDispInfo;

// Display related register parameter
extern RDispPrm _rMDispPrm;
extern RDispPrm _rPDispPrm;

// Pic related info
extern RPicInfo _rMPicInfo;
extern RPicInfo _rPPicInfo;


// for mute
extern BOOL _fgMainBlueMute;
extern BOOL _fgSubBlueMute;
extern BOOL _fgMainBlackMute;
extern BOOL _fgSubBlackMute;

extern BOOL _fgVideoReady;

#endif
