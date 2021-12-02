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
 * $Date: 2012/04/27 $
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "drvcust_if.h"
#include "tve_if.h"
#endif
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "drv_scaler_table.h"
#include "frametrack_drvif.h"
#include "scpos_debug.h"

// nptv header files
#include "general.h"       
#include "sv_const.h"       
#include "video_def.h"      
#include "vdo_misc.h"       
#include "vdp_display.h"
#include "drv_video.h"   
#include "source_select.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#ifdef CC_MT5395
#include "drv_mjc.h"
#endif
#ifdef CC_SCPOS_3DTV_SUPPORT
#include "hw_di.h"
#endif

#include "drv_tdtv_drvif.h"

// drvif header files
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "nptv_drvif.h"

// Other header files
#include "c_model.h"
#include "x_bim.h"
#include "x_mid.h"
#include "x_assert.h"
#include "x_util.h"

#ifdef CHANNEL_CHANGE_LOG
#include "x_timer.h"
#include "ir_if.h"
#endif
#if SUPPORT_DOT_TO_DOT_PATTERN
#include "hw_tdc.h"
#endif
#ifdef CC_SCALER_LR_SHIFT_WA
#include "hw_ycproc.h"
#endif
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations : all WA
//-----------------------------------------------------------------------------
//[DTV0088998] when V porch = 0 with VL change ,set DS1_02_IMPORT_V_INV_1 = 0.
#define CC_SCPOS_WA01 1
// CC_SCPOS_WA02  work around pscan display mode mirror issue
#define CC_SCPOS_WA02 0
/* for 1080i pscan dispmode workaround.
   in down scaling pscan dispmode
   1. invert vsync
   2. enter pscan dispmode
   3. wait for N Vsync
   4. invert vsync back to original
 */
#define CC_SCPOS_WA13_PSCAN_DISPMODE_1080I 0
//#define CC_SCPOS_WA13_COUNTER_TO_INVERT_VSYNC 3

// import will fail when output frame rate > input frame rate
//#define CC_SCPOS_WA25

// queue scaler event when freezing
#define CC_QUEUE_EVENT_IN_FREEZE

// do not enable dynamic scaling while nonlinear enable
//#define CC_SCPOS_NO_DS_NONLINEAR

#define CC_SCPOS_FORCE_WOFF 1

#define CC_SCPOS_NEVER_FIX_HPS 1

#define CC_TWO_FRAME_LIMIT 1

#define CC_SCPOS_WA_PMX_DELAY 0

#ifdef TVE_WA
#define TVE_WA2 
#endif

#define CC_NEW_SCALAR_DISPMODE_AUTOTUNE 1
#define CC_SPEED_UP_DISPMODE_TUNE 1  

#define SCPOS_BYPASS_PSCANNR    0xffffffff

#define CC_SPEEDUP_IMPORT   0

//#define SCPIP_TV3D_DEBUG

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define CAL_HDIRECT_FACTOR               (0)
#define CAL_VDIRECT_FACTOR               (1)
#define VDP_MDDI_RESCALE_TIME               (4)
#define VDP_SCPOS_RESCALE_TIME              (7)
#define VDP_SCPOS_IMPORT_PROTECT            (3)
#define VDP_SCPOS_WEN_WAIT                  (5)
#define VDP_SCPOS_TV3D_MUTE_DELAY           (3)
#define VDP_SCPOS_DELAY_UNMUTE_TIME        (10)    
#define VDP_DYNAMICE_SCALE_IDLE             (0)
#define VDP_DYNAMICE_SCALE_SCALING          (1)

#define DFT_SCPOS_MAIN_444_FIFO_LENGTH        1920
#define DFT_SCPOS_MAIN_422_FIFO_LENGTH        2100
#define DFT_SCPOS_SUB_444_FIFO_LENGTH        960
#define DFT_SCPOS_SUB_422_FIFO_LENGTH        960
static const UINT32 _u4SCPOS_FIFO_LENGTH[2][2] = {{DFT_SCPOS_MAIN_422_FIFO_LENGTH, DFT_SCPOS_MAIN_444_FIFO_LENGTH},//main
                                           {DFT_SCPOS_SUB_422_FIFO_LENGTH, DFT_SCPOS_SUB_444_FIFO_LENGTH}};    //sub

#define MAX_FSYNC_TUNE        0x3ff

//US line delay setting
#define DOWN_SCALING_MODE 0
#define UP_SCALING_MODE 1

#define BASE_WADDRESS_CHG ((UINT8)1 << 0)
#define BASE_RADDRESS_CHG ((UINT8)1 << 1)
#define FRAME_SIZE_CHG ((UINT8)1 << 2)
#define FRAME_NUM_CHG ((UINT8)1 << 3)
#define COLOR_DEPTH_CHG ((UINT8)1 << 4) 
#define PARAM_IGNORE    (0xffffffff)

#if CC_SPEEDUP_IMPORT
#define SCPIP_UPDATE_W  (0x1)
#define SCPIP_UPDATE_R  (0x2)
static const UINT32 VDP_SCPOS_SPEEDUP_IMPORT_PROTECT  =  (2);
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#define SCPIP_3DPRWA_DEF_DEPTH 6
#define SCPIP_3DPRWA_DEF_OVERSCAN 20    /* pixel */
#define SCPIP_3DPRWA_MAX_OVERSCAN 50    /* pixel */
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
UINT8 _bLRInverse = SV_FALSE;
UINT8 _bTTDDepth = SCPIP_3DPRWA_DEF_DEPTH;
UINT8 _bTTDOverscan = SCPIP_3DPRWA_DEF_OVERSCAN;

UINT8 _bResetScalerCountFor3DPR;
BOOL _fgScaleTriggerHdmiMchFor3DPR = SV_FALSE;
extern UINT8 _bCurrentInputMSFor3DPR;
#endif


#if SUPPORT_SBS_CONVERT
BOOL _b2dTo3d=SV_FALSE;
BOOL _bSBSOutput= SV_FALSE;
UINT8 _bResetScalerCount;
BOOL _fgScaleTriggerHdmiMch = SV_FALSE;
extern UINT8 _bCurrentInputMS;
#endif
#ifdef IS_SUPPORT_3D_PHOTO
BOOL IsFixRegion = FALSE;
extern RDispInfo _rM3DPhotoDispInfo;
extern RDispInfo _rMDispOriInfo;
#endif
#ifdef SUPPORT_DRAM_DUMP
extern BOOL fgDisplayModeDump;
#endif

#define ARRAY_BOUND_CHECK(idx, bound)  \
{                                      \
    if (idx >= bound)                  \
    {                                  \
        ASSERT(0);                     \
        idx = bound-1;                 \
    }                                  \
}

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of VDP_DYNAMIC_SCALER_PRM_T
 */
typedef struct
{
    UINT32 u4State;
    UINT32 u4VsyncCount;
    UINT32 u4LastFactorValid;

    UINT32 u4ScaleStep;
    UINT32 u4TotalScaleNs;

    INT32 i4SrcXOffsetStep;
    INT32 i4SrcYOffsetStep;
    INT32 i4SrcWidthStep;
    INT32 i4SrcHeightStep;

    INT32 i4OutXOffsetStep;
    INT32 i4OutYOffsetStep;
    INT32 i4OutWidthStep;
    INT32 i4OutHeightStep;

    UINT32 u4CurSrcWidth;
    UINT32 u4CurSrcHeight;
    UINT32 u4CurSrcXOffset;
    UINT32 u4CurSrcYOffset;

    UINT32 u4CurOutWidth;
    UINT32 u4CurOutHeight;
    UINT32 u4CurOutXOffset;
    UINT32 u4CurOutYOffset;

    UINT32 u4OrgSrcWidth;
    UINT32 u4OrgSrcHeight;
    UINT32 u4OrgSrcXOffset;
    UINT32 u4OrgSrcYOffset;

    UINT32 u4OrgOutWidth;
    UINT32 u4OrgOutHeight;
    UINT32 u4OrgOutXOffset;
    UINT32 u4OrgOutYOffset;

    UINT32 u4LastSrcWidth;
    UINT32 u4LastSrcHeight;
    UINT32 u4LastSrcXOffset;
    UINT32 u4LastSrcYOffset;

    UINT32 u4LastOutWidth;
    UINT32 u4LastOutHeight;
    UINT32 u4LastOutXOffset;
    UINT32 u4LastOutYOffset;
} VDP_DYNAMIC_SCALER_PRM_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

// nptv global variable

VDP_SCALER_PRM_T _arScalePrm[VDP_NS] =
{
    {0, DFT_DS_FACTOR,  0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*UNIT_NONLINEAR,*/ 0, 0, /*UINT_NONLINEAR_END_FACTOR,*/ TRUE, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, 0, 0, 0}, {0, 0, 0, 0}, /*0, 0,*/ 0, 0, /*MDDIDramWidth*/ 2000, 0, 0, 0, /* new nonlinear*/ 0, {0, 0, 0, 0, 0}},
    {0, DFT_DS_FACTOR,  0, 250, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*UNIT_NONLINEAR,*/ 0, 0, /*UINT_NONLINEAR_END_FACTOR,*/ FALSE, {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR}, {0, 0, 0, 0}, {0, 0, 0, 0}, /*0, 0,*/ 0, 0, /*MDDIDramWidth*/2000, 0, 0, 0, /* new nonlinear*/ 0, {0, 0, 0, 0, 0}}
};

HANDLE_T h_dramUpdateSema = NULL;
SCALER_FBM_POOL_T rScalerDramInfo;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
//static UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay);
static void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable);
static void _vScpipSetUSInitialFactor(const UINT32 u4VdpId);
static void _vScpipSetUSReg(UINT32 u4VdpId);
static void _vScpipSetDispReg(UINT32 u4VdpId);
static void _vScpipSetDSInitialFactor(UINT32 u4VdpId);
static void _vScpipSetDownScalingFactor(UINT32 u4VdpId);
static void _vScpipCalNewScale(UINT32 u4VdpId);
static void _vScpipUpdateDispPrm(UINT32 u4VdpId);
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode);
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode);
static void _vScpipSetDRAMFifo(UINT8 bPath, UINT8 bExtFifo, UINT8 bIntFifo);
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 u4Source, UINT32 u4Target, UINT32 u4LimitLength, UINT8 bDircection);
static void _vScpipSetDEAlignmentReg(UINT8 index);
static void _vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen);
static UINT32 _u4ScpipGetPscanDispmodeHLen(UINT8 bPath);
#if !CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP

#if !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP
static UINT32 _u4ScpipGetPscanDispmodeHPorch(UINT8 bPath);
#endif
#endif
static void _vScpipSetDispModeReg(void);
static void _vScpipDispmodeFreeRunChk(void);
static void _vScpipWriteCTRLCheck(UINT8 bPath);
//static void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum);
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize);
static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum);
static void _vScpipConfigInputClk(UINT8 bPath);
static UINT8 _u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath);
#if CC_TWO_FRAME_LIMIT
static void _vScpipSetTwoFrameLimit(UINT32 u4VdpId);
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
// config flip/mirror (called in SRM callback function)
static void _vScpipConfigFlip(UINT8 bPath);
#endif

static void _vScpipNormProcByPath(const UINT32 u4VdpId); 
static UINT32 _u4ScpipCalNegInitPhase(const UINT32 u4InW, const UINT32 u4OutW, const UINT32 u4UScl);
static void _vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bmode);
static void _vScpipNewDSPscanDispmodeWaterCtrl(void);
static void _vScpipNewUSPscanDispmodeWaterCtrl(void);
#if SUPPORT_SBS_CONVERT
extern UINT8 bApiVideoSetSrcPosSize(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight);
extern UINT8 bVideoSetDispRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight);
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static volatile UINT32 _u4ResetScposDramBuffer[VDP_NS] = {0, 0};
static UINT32 _au4DelayWriteEnable[VDP_NS] = {0, 0};

static UINT32 _au4GameMode[VDP_NS] = {0, 0};
static UINT32 _au4OriFrameNum[VDP_NS] = {3, 3};

// Initlaize Dispmode Auto Tune Table
static const Disp_Tune AddrStack[] = 
{
    // 1920x1080 Panel
    {1920, 1080, 1920, 1080, 60, 0, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080p_60
    {1920, 1080, 1920, 1080, 50, 0, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080p_50
    {1920, 1080, 1920, 1080, 60, 1, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080i_60
    {1920, 1080, 1920, 1080, 50, 1, 0x7800, 0x7900, 0x8000, 0x8000, 0, 0x78}, // 1080i_50
};
static const UINT32 ADDR_STACK_SIZE_DISP = sizeof(AddrStack)/sizeof(AddrStack[0]);

static UINT32 _au4ForcePsDivClkEn[VDP_NS] = {0, 0};
static UINT32 _au4ForcePsDivClkM[VDP_NS] = {0, 0};
static UINT32 _au4ForcePsDivClkN[VDP_NS] = {0, 0};

static UINT32 _u4BaseAddress[2]= {0, 0}; // scaler dram base address
//static UINT32 _u4DramBufSize[2]= {0, 0};
static UINT8  _bSRMStable[2]= {SV_FALSE, SV_FALSE};
static UINT8  _bIsDramBufInit[2]= {SV_FALSE, SV_FALSE};
//static UINT32 _u4SRMFrameDelay[2]={0,0};// AV sync : Frame dealy number for SRM, the value is Delay*2.

// When TVE on, force dispOff
static UINT32 _u4ForceDispOff[2]={SV_FALSE, SV_FALSE};

RDispInfo* const _pDispInfo[] =
{
    &_rMDispInfo,
    &_rPDispInfo
};

RDispPrm* const _pDispPrm[] =
{
    &_rMDispPrm,
    &_rPDispPrm
};

RVChannel* const _pChannel[] =
{
    &_rMChannel,
    &_rPChannel
};

RPicInfo* const _pPicInfo[] =
{
    &_rMPicInfo,
    &_rPPicInfo
};

static VDP_DYNAMIC_SCALER_PRM_T _arDynamicPrm[VDP_NS];

static UINT32 _u4ScposImportNotReady[VDP_NS] = {0, 0};
static UINT32 _u4Forced10BitMode[VDP_NS] = {0, 0};
static UINT32 _au4ScalingFactorChanged[VDP_NS] = {0, 0};

#define CC_SCPOS_FREERUN_MAX        150
static UINT32 _u4DispModeFreeRunCounter =0;

#if CC_SCPOS_FORCE_WOFF
static UINT32 _au4ForceWriteOff[VDP_NS] = {0, 0};
#endif

//#define CC_SCPOS_LOG_DRAM_PARAM (50)
#ifdef CC_SCPOS_LOG_DRAM_PARAM
static UINT32 _au4TimeLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4AddressLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4ValueLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _u4LogIdx = 0;
#endif

static BOOL _fgIsHNegInitPhaseOn = SV_ON;
static BOOL _fgIsVNegInitPhaseOn = SV_ON;

// TODO: remove
#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
static UINT32 _counterToInvertVSync[VDP_NS] = {0,0};
static UINT32 _flagToInvertVSync[VDP_NS] = {SV_FALSE,SV_FALSE};
#endif

#if CC_SPEEDUP_IMPORT
static UINT32 _au4ImportUpdate[2] = {0, 0};
#endif

#if CC_SCPOS_VSYNC_WA2
static UINT32 _u4UpdateDisplayModeState = 0;
static BOOL _fgDisplayModeEnable = FALSE;
#endif

#ifdef CC_SCPOS_3DTV_SUPPORT
static void _vScpipFullPRProc(void);
static BOOL _fgFprToggleOn = FALSE;
static UINT8 _u1FprSetWPosCounter = 0;

static UINT32 _fpr_delay = 0; // delay = 0 is safe for 240hz output.
                              // if there is demand of using lower output frame rate,
                              // we must consider to extend the delay to make register setting
                              // is out of the blanking region
//static UINT8 _full_pr_mode_sel = 0;
static BOOL _fgSensioWAOn = FALSE;                              
static BOOL _fgTnbWAToggleOn = FALSE;
static BOOL _fgFS3d22dToggleOn[VDP_NS] = {FALSE, FALSE};
//static BOOL _fgFS322WAInited[VDP_NS] = {FALSE, FALSE};
static UINT8 _au1FBField[6]; // 1: bottom, 0: top
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------



/**
* @brief Re-calculate delay counter by input/output frame rate ratio
* @param u4VdpId  
* @param u4Delay delay counter for input frame rate = output frame rate
* @return the delay counter for output domain
*/
// TODO: change back to static
UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay)
{
    UINT8  bPath = getScalerPath(u4VdpId);
    UINT32 u4InputFrameRate = bDrvVideoGetRefreshRate(bPath);
    UINT32 u4OutputFrameRate = vDrvGetLCDFreq();

    if(u4InputFrameRate == 0)
    {
        return u4Delay;
    }

    if(PANEL_IsSupport120Hz())
    {
        u4OutputFrameRate >>= 1;
    }
    
    #ifdef CC_SCPOS_3DTV_SUPPORT        
    //if(u4VdpId == VDP_1)
    {
        if(rScpipTv3dGetInType(u4VdpId) == E_TDTV_DECODER_INPUT_FS_P)
        {
            u4InputFrameRate >>= 1;           
        }
    }        
    #endif

    u4Delay = ((u4OutputFrameRate * u4Delay) + (u4InputFrameRate - 1))/u4InputFrameRate;
    return u4Delay;
}

static UINT32 _u4IsScpipDramBufferInit(UINT8 ucVdpId)
{   
    ARRAY_BOUND_CHECK(ucVdpId, VDP_NS);

    return _bIsDramBufInit[ucVdpId];
}

/**
* @brief Calculate negative initial phase value
* @param u4InW  Input width before up-scale
* @param u4OutW Output width after up-scale
* @param u4UScl Up-scaling factor
* @return Negative initial phase value
*/
static UINT32 _u4ScpipCalNegInitPhase(const UINT32 u4InW, const UINT32 u4OutW, const UINT32 u4UScl)
{
    UINT32 u4Diff = 0;
    UINT32 u4InitPhase = 0x0;

    ASSERT(u4UScl <= DFT_US_FACTOR);

    if((u4UScl == DFT_US_FACTOR) || (_arScalePrm[VDP_1].u4NonlinearEn && (_arScalePrm[VDP_1].u4NonlinearCtrl!= 0))) // no up-scale or non_linear off
    {
        // always set to negative phase to prevent sub window transcient
        return (DFT_US_FACTOR-1);
    }
    else // up scale
    {
        if (u4UScl*(u4OutW-1)>(u4InW-1)*DFT_US_FACTOR)
        {
            u4Diff = (u4UScl*(u4OutW-1)-(u4InW-1)*DFT_US_FACTOR)/2;
            u4InitPhase = DFT_US_FACTOR - u4Diff;

            // prevent truncation error & work around for 3x+
            if ((u4InitPhase + u4UScl) <= DFT_US_FACTOR)
            {
                u4InitPhase = (DFT_US_FACTOR+1) - u4UScl;  // u4Scl + u4InitPhase must > DFT_US_FACTOR
            }

            return u4InitPhase;
        }
        else
        {
            return (DFT_US_FACTOR-1);
        }
    }  
}

#if SUPPORT_SBS_CONVERT
UINT32 u4WAddr_M;
UINT32 u4Size_M;
#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
UINT32 u4WAddr_M_For3DPR;
UINT32 u4Size_M_For3DPR;
#endif

void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum)
{
    UINT32 u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize, u4UpdateNum;
    UINT32 u4OriWAddr, u4OriRAddr, u4OriSize, u4OriNum;
    RDispPrm* dispPrm;
    UINT8 bPath;
    UINT8 offset;
    UINT8 u1UpdateParameter;
    UINT8 u1SwProtect = 0;
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    #if !SUPPORT_POP
    if(u4VdpId==VDP_2)
    {
        return;
    }
    #endif

    // Convert to register-based value
    u4UpdateRAddr = u4Addr >> 4;
    u4UpdateSize = u4FrameSize >> 4;
    //u4UpdateNum = u4FrameNum - 1;
    u4UpdateNum = u4FrameNum;
    u4UpdateWAddr = u4UpdateRAddr;
    u1UpdateParameter = 0;

    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    dispPrm = getDispPrm(bPath);
    offset = getPIPOffset(bPath);

    //re-calculate write address
    if ((_u4IsScpipDramBufferInit(bPath)!=0) && (u4Addr != PARAM_IGNORE))
    {
        if(bScpipFlipEnable[u4VdpId])
        {
            u4UpdateWAddr = u4UpdateWAddr +(dispPrm->wDataLen)*(dispPrm->wVLen-1);
            LOG(3,"Change Scaler Dram address(%d) (0x%X --> 0x%X) with flip\n", u4VdpId, u4UpdateRAddr, u4UpdateWAddr);
        }
        if(bScpipMirrorEnable[u4VdpId] &&
            RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(bPath), PIP1_05_DRAM_BURST_MODE_1))
        {
            //u4UpdateWAddr -= dispPrm->wDataLen - dispPrm->wHLen;
            u4UpdateWAddr -= _pDispPrm[u4VdpId]->u1HPaddingLen;
        }
    }

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_lock(h_dramUpdateSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }
    
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+offset, PIP1_07_WINC_1);    
    //u4OriNum = RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_FSEL_1);
    u4OriNum = u4ScpipGetBufNum(u4VdpId);
    u4OriWAddr = u4ScpipGetWAddr(u4VdpId);
    u4OriRAddr = u4ScpipGetRAddr(u4VdpId);
    if ((u4OriWAddr != u4UpdateWAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_WADDRESS_CHG;        
        u4UpdateWAddr <<= 4;
    }
    else
    {
        u4UpdateWAddr = PARAM_IGNORE;
    }
    
    if ((u4OriRAddr != u4UpdateRAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_RADDRESS_CHG;        
        u4UpdateRAddr <<= 4;
    }
    else
    {
        u4UpdateRAddr = PARAM_IGNORE;
    }
    
    if ((u4OriSize != u4UpdateSize) && (u4FrameSize != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_SIZE_CHG;
        u4UpdateSize <<= 4;
    }
    else
    {
        u4UpdateSize = PARAM_IGNORE;
    }
    
    if ((u4OriNum != u4UpdateNum) && (u4FrameNum != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_NUM_CHG;
        //u4UpdateNum += 1;
    }
    else
    {
        u4UpdateNum = PARAM_IGNORE;
    }
    
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
       || (SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        if(u4VdpId == VDP_1)
        {
            if(u1UpdateParameter & BASE_WADDRESS_CHG )
            {
                u4WAddr_M_For3DPR = u4UpdateWAddr;
            }
            else
            {
                u4WAddr_M_For3DPR = u4OriWAddr<<= 4;
            }
            if (u1UpdateParameter & FRAME_SIZE_CHG)
            {  
                u4Size_M_For3DPR = u4UpdateSize;    
            }
            else
            {
                u4Size_M_For3DPR = u4OriSize<<= 4;   
            }
        }
        if(u4VdpId == VDP_2)
        {
            u4UpdateWAddr = u4WAddr_M_For3DPR;
            u4UpdateSize = u4Size_M_For3DPR;
            u1UpdateParameter |= BASE_WADDRESS_CHG;  
            u1UpdateParameter |= FRAME_SIZE_CHG;
        }
    }
    #endif    
    
    #if SUPPORT_SBS_CONVERT
    if(_bSBSOutput)
    {
        if(u4VdpId == VDP_1)
        {
            if(u1UpdateParameter & BASE_WADDRESS_CHG )
            {
                u4WAddr_M = u4UpdateWAddr;
            }
            else
            {
                u4WAddr_M = u4OriWAddr<<= 4;
            }
            if (u1UpdateParameter & FRAME_SIZE_CHG)
            {  
                u4Size_M = u4UpdateSize;    
            }
            else
            {
                u4Size_M = u4OriSize<<= 4;   
            }
        }
        if(u4VdpId == VDP_2)
        {
            u4UpdateWAddr = u4WAddr_M;
            u4UpdateSize = u4Size_M;
            u1UpdateParameter |= BASE_WADDRESS_CHG;  
            u1UpdateParameter |= FRAME_SIZE_CHG;
            
        }
    }
   #endif
    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        CRIT_STATE_T csState; 
        // protect by SW
        u1SwProtect = 1;    
        csState = x_crit_start(); 
        _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] | 0x80000000;
        x_crit_end(csState);

        if( bScpipFlipEnable[u4VdpId] == SV_FALSE)
        {
            vRegWriteFldAlign(SCPIP_PIP1_09+offset, 0, PIP1_09_WEN_RLH_1);
            vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);
        }
    }

    // update base address and frame size
    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | BASE_RADDRESS_CHG))
    {
        _vScpipSetDram(u4VdpId, u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize);
    }
    // update frame number
    if (u1UpdateParameter & FRAME_NUM_CHG)
    {
        vScpipSetBufNum(u4VdpId, u4UpdateNum);            
    }

    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        if (u1SwProtect)
        {
            // protect by SW
            CRIT_STATE_T csState; 
            csState = x_crit_start(); 
            _u4ResetScposDramBuffer[u4VdpId] = _u4ResetScposDramBuffer[u4VdpId] & 0x7fffffff;
            _u4ResetScposDramBuffer[u4VdpId] = (_u4ResetScposDramBuffer[u4VdpId]  < 2)? 2: _u4ResetScposDramBuffer[u4VdpId];
            x_crit_end(csState);
        }
    }

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_unlock(h_dramUpdateSema) == OSR_OK);
    }

}

static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize) 
{
    UINT8 offset;
    UINT8 bPath;
    CRIT_STATE_T csState;
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    offset = getPIPOffset(bPath);

    if (u4WAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Write Address: 0x%X\n", u4VdpId, u4WAddr);
        csState = x_crit_start(); 
        
         if(bScpipFlipEnable[u4VdpId]==SV_FALSE)
        {
            if (RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_WEN_RLH_1) || RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_W_EN_1))
            {
                ASSERT(0);
            }
        }
        #ifdef CC_SCPOS_LOG_DRAM_PARAM
        HAL_TIME_T dt;
        HAL_GetTime(&dt);
        _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
        _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_05+offset;
        _au4ValueLog[_u4LogIdx] = (u4WAddr>>4);
        _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        #endif

        vScpipSetWAddr(u4VdpId, u4WAddr>>4);
        
        _bIsDramBufInit[u4VdpId] = SV_TRUE;
        x_crit_end(csState);
    }
    if (u4RAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Read Address: 0x%X\n", u4VdpId, u4RAddr);
        if(RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(bPath), PIP1_05_DRAM_BURST_MODE_1))
        {
            ASSERT(((u4RAddr >> 4) % 4) == 0);
        }
        
        vScpipSetRAddr(u4VdpId, u4RAddr>>4);
    }
    if (u4FrameSize != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) frame size: 0x%X\n", u4VdpId, u4FrameSize);
        csState = x_crit_start(); 
        
        if(bScpipFlipEnable[u4VdpId]==SV_FALSE)
        {
            if (RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_WEN_RLH_1) || RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_W_EN_1))
            {
                ASSERT(0);
            }
        }
        #ifdef CC_SCPOS_LOG_DRAM_PARAM
        HAL_TIME_T dt;
        HAL_GetTime(&dt);
        _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
        _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_07+offset;
        _au4ValueLog[_u4LogIdx] = (u4FrameSize>>4);
        _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        #endif
        vRegWriteFldAlign(SCPIP_PIP1_07+offset, (u4FrameSize>>4), PIP1_07_WINC_1);
        x_crit_end(csState);
    }    
    
#if CC_SCPIP_DRAM_OVERWRITE_WA
    UINT8 dsOffset = getDSOffset(u4VdpId);
    UINT8 u1OriCp = RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_VSYNCP_1);
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, !u1OriCp, DS1_02_VSYNCP_1);  
    //HAL_Delay_us(10);
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u1OriCp, DS1_02_VSYNCP_1);    
#endif
}

/**
* @brief Set frame buffer number
* @param u4VdpId VDP_1/VDP_2
* @param u4BufNum frame buffer number
*/
void vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum)
{
    UINT8 offset = getPIPOffset((UINT8)u4VdpId);
    
    if (u4BufNum < 1)
        return;

    // SBS TO FP case need to use original fsel register (4 frames)
    // So we cannot use fsel_dump in all cases. 
    // Currently we use original fsel register when frame number <= 4
    if(u4BufNum > 4)
    {
        vRegWriteFldAlign(SCPIP_PIP1_12+offset, 1, PIP1_12_FSEL_DUMP_EN);
        vRegWriteFldAlign(SCPIP_PIP1_12+offset, u4BufNum-1, PIP1_12_FSEL_DUMP);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_12+offset, 0, PIP1_12_FSEL_DUMP_EN);
        vRegWriteFldAlign(SCPIP_PIP1_09+offset, u4BufNum-1, PIP1_09_FSEL_1);
    }

}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff)
{
    if (_u4ForceDispOff[u4Path] != u4OnOff)
    {
        _u4ForceDispOff[u4Path] = u4OnOff;
    }
}


/**
* @brief Get frame buffer number
* @param u4VdpId VDP_1/VDP_2
*/
UINT32 u4ScpipGetBufNum(UINT32 u4VdpId)
{
    UINT32 u4FrameNum;
    UINT8 offset = getPIPOffset((UINT8)u4VdpId);
    u4FrameNum = 1;
    
    if(!RegReadFldAlign(SCPIP_PIP1_12+offset, PIP1_12_FSEL_DUMP_EN))
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_FSEL_1) + 1;
    }
    else
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_12+offset, PIP1_12_FSEL_DUMP) + 1;
    }
    return u4FrameNum;    
}


/**
* @brief Set drame related information
* @param u4VdpId VDP_1/VDP_2
* @param prFbmPool parameters from srm
*/
void vScpipSrmSetBuf(UINT32 u4VdpId, const SCALER_FBM_POOL_T * prFbmPool) 
{
//    UINT32 u4Size;
    UINT32 u4FbSize;
    UINT8 u1ParamChanged;
    //UINT32 _u4BaseAddress;
    UINT32 u4FrameNum;
    
    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    //_arScalePrm[u4VdpId].u4MddiDramWidth = prFbmPool->u4MddiWidth;
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    u1ParamChanged = 0;

    // extra frame buffer condition
    // 1. MAIN SBS To FS (4 frames, 3D)
    // 2. MAIN other conditions (ex:TNB to FS, 6 frames, 3D)
    // 3. SUB 3D To 2D FS (6 frames, 3D)
    // 4. SUB with flip (4 frame, 3D, not exist yet in 5395. 2010.10.26)
    // 5. SUB other conditions (3 frames, 3D)
    // 6. 2D FLIP (4 frames)
    // 7. 2D normal (3 frames)
    #if defined(DRV_USE_EXTERNAL_3D_FRC) && defined(CC_SCPOS_3DTV_SUPPORT)
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))        /*3D*/
        {
            if ((u4VdpId == VDP_1) && (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FS_INPUT)) && prFbmPool->fg422 == SV_TRUE)
            {     
                u4FrameNum = 6; 
            }
            else
            {
                #if defined(CC_FLIP_MIRROR_SUPPORT)
                if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)    
                {
                    u4FrameNum = 4;
                }
                else
                #endif
                {
                    u4FrameNum = 3;
                }
            }
        }
        else
    #elif defined(CC_SCPOS_3DTV_SUPPORT) && !defined(CC_MT5365) && !defined(DRV_USE_EXTERNAL_3D_FRC)
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))        /*3D*/
        {
            if ((u4VdpId == VDP_1) && prFbmPool->fg422 == SV_TRUE)
            {     
                if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT)                    
                   )
                {
                    u4FrameNum = 3;   
                }
                else
                {
                    u4FrameNum = 6;
                }
            }
            else if ((u4VdpId == VDP_2)
                      && (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)
                      && fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_FS))
                     )
            {
                u4FrameNum = 6;
            }
            else
            {
            #if defined(CC_FLIP_MIRROR_SUPPORT)
                if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)    
                {
                    u4FrameNum = 4;
                }
                else
            #endif
                {
                    u4FrameNum = 3;
                }
            } 
        }
        else       
    #endif  
        {       /*2D*/
        #if defined(CC_FLIP_MIRROR_SUPPORT)
            if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)    
            {
                u4FrameNum = 4;
            }
            else
        #endif
            {   
                u4FrameNum = 3;
            } 
        }         
    
    if (((prFbmPool->fg10bit) && (prFbmPool->fg422 == SV_TRUE)) 
        && (_arScalePrm[u4VdpId].u410BitMode == 0))
    {
        _arScalePrm[u4VdpId].u410BitMode = 1;
        u1ParamChanged = COLOR_DEPTH_CHG;
        LOG(3, "SCPOS 10 Bit mode\n");          
    }
    else if (((prFbmPool->fg8bit) || (prFbmPool->fg422==SV_FALSE))
        && (_arScalePrm[u4VdpId].u410BitMode == 1))
    {
        _arScalePrm[u4VdpId].u410BitMode = 0;
        u1ParamChanged = COLOR_DEPTH_CHG;
        LOG(3, "SCPOS 8 Bit mode\n");         
    }

    _arScalePrm[u4VdpId].u4DramWidth = prFbmPool->u4Width;
    _arScalePrm[u4VdpId].u4DramHeight = prFbmPool->u4Height;
    if ((_arScalePrm[u4VdpId].u4DramWidth == 0) || (_arScalePrm[u4VdpId].u4DramHeight == 0))
    {
        return;
    }
    
    u4FbSize = (prFbmPool->u4FbmSize / u4FrameNum);
    
    #ifdef CC_SCPOS_3DTV_SUPPORT        
    if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D) &&
        (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT) 
          || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR)
          || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
          || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
          || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT)) 
       )
    {
        u4FbSize >>= 1;  // WINC/2  for side by side to frame pack mode *special case*
    }
    #endif
    if(RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(getScalerPath(u4VdpId)), PIP1_05_DRAM_BURST_MODE_1)) // dram burst mode
    {
        u4FbSize = (u4FbSize >> 6) << 6;  // set (u4FbSize >> 4) to multiply of 4 for DRAM burst mode
    }
    _u4BaseAddress[u4VdpId] = prFbmPool->u4Addr;
    //_u4DramBufSize[u4VdpId] = prFbmPool->u4FbmSize;
    _arScalePrm[u4VdpId].u4Is422= prFbmPool->fg422;
    _au4OriFrameNum[u4VdpId] = u4FrameNum;
    if ((_au4GameMode[u4VdpId]) && (u4FrameNum > 2))
    {
        LOG(3, "GameMode: Force scalar to 2 frame mode! (%d)\n", u4FrameNum);
        u4FrameNum = 2;
    }

    u1ParamChanged |= _u1ScpipIsDRAMParamChanged(u4VdpId, _u4BaseAddress[u4VdpId] , u4FbSize, u4FrameNum);

    if (u1ParamChanged)
    {
      //      if ((u1ParamChanged & BASE_ADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG) || (u1ParamChanged & COLOR_DEPTH_CHG))
            {
                /* Dram address or size changed => handle mute peggy 20071107 */
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
            }

            UINT32 u4Addr, u4Size, u4Num;
            u4Addr = PARAM_IGNORE;
            u4Size = PARAM_IGNORE;
            u4Num = PARAM_IGNORE;
            if ((u1ParamChanged & BASE_WADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG))
            {
                u4Addr = _u4BaseAddress[u4VdpId];
                u4Size = u4FbSize;
            }
            if (u1ParamChanged & FRAME_NUM_CHG)
            {
                u4Num = u4FrameNum;
            }

            _vScpipUpdateDramParameter(u4VdpId, u4Addr, u4Size, u4Num);                       
    
            if (u1ParamChanged & COLOR_DEPTH_CHG)
            {
                if (u4VdpId == VDP_1)
                {
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                }
                else
                {
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                }
            }           
    } 
}

/**
* @brief Set initial down scale factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetDSInitialFactor(UINT32 u4VdpId) 
{
    UINT8 dsOffset;
    UINT8 bPath;
    RDispPrm* dispPrm;
    UINT32 u4HInitValue;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);
    u4HInitValue = DFT_DS_FACTOR- dispPrm->u4DSclH;

    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, u4HInitValue, DS1_01_DSCALE_HOFST_1);

    // always keep VOFST == 0
    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, 0, DS1_01_DSCALE_VOFST_1);    

}


/**
* @brief Set pre-scaling down factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetHPreScalingDownFactor(UINT32 u4VdpId)
{        
    UINT32 u4HInitValue;
    UINT32 u4HpsFactor; 

    u4HInitValue = DFT_HPD_FACTOR - _arScalePrm[u4VdpId].u4HpsFactor;
    u4HpsFactor = _arScalePrm[u4VdpId].u4HpsFactor;

    #if SUPPORT_POP
    if (u4VdpId == VDP_2)
    {
        vRegWriteFldAlign(SCPIP_HDS_01, u4HInitValue, SCPIP_HDS_PIP_OFST);
        vRegWriteFldAlign(SCPIP_HDS_01, u4HpsFactor, SCPIP_HDS_PIP_SCALER);
    }
    else
    #endif
    {
        vRegWriteFldAlign(SCPIP_HDS_00, u4HpsFactor, SCPIP_HDS_MAIN_SCALER);
        vRegWriteFldAlign(SCPIP_HDS_00, u4HInitValue, SCPIP_HDS_MAIN_OFST);
    }
}


/**
* @brief Set down scale factor
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetDownScalingFactor(UINT32 u4VdpId)
{
    #if !SUPPORT_POP
        if(u4VdpId == VDP_2)
        {
            return;
        }
    #endif

    RDispPrm* dispPrm;
    UINT8 dsOffset;
    UINT8 bPath;
    UINT32 u4HdsFactor;
    UINT32 u4VdsFactor;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);

    u4HdsFactor = dispPrm->u4DSclH;
    u4VdsFactor = dispPrm->u4DSclV;

    /* Set Down Scaling factor */
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, u4HdsFactor, DS1_00_DSCALER_H_1);
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, u4VdsFactor, DS1_00_DSCALER_V_1);    
}

UINT16 u2ScpipGetDispWidth(UINT8 bPath)
{    
    RDispPrm* dispPrm;    
    dispPrm = getDispPrm(bPath);    
    return dispPrm->wRHLen;
}

UINT16 u2ScpipGetDispHeight(UINT8 bPath)
{            
    RDispPrm* dispPrm;            
    dispPrm = getDispPrm(bPath);            
    return dispPrm->wRVLen;
}

/**
* @brief Set up scaler related registers
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetUSInitialFactor(const UINT32 u4VdpId)
{
    RDispPrm* dispPrm;
    UINT8 bPath;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);    

    if(u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_US1_11,dispPrm->u4USclHInit,US1_11_USCALER_H_INIT_1);
        vRegWriteFldAlign(SCPIP_US1_10,dispPrm->u1USclHInitSigned,US1_10_USCALER_H_INIT_SIGNED_1);
       
        vRegWriteFldAlign(SCPIP_US1_11,dispPrm->u4USclVInit,US1_11_USCALER_V_INIT_1);
        vRegWriteFldAlign(SCPIP_US1_10,dispPrm->u1USclVInitSigned,US1_10_USCALER_V_INIT_SIGNED_1);
        
        if((dispPrm->u4USclVInit == 0) && (dispPrm->u1USclVInitSigned == 0))
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_01, 0, PIP_OA_01_USCALER_INI_V_ADJ_SEL);
        }
        else
        {
            // do not shift scaler output v sync by 1T when negative initial phase is on
            vRegWriteFldAlign(SCPIP_PIP_OA_01, 1, PIP_OA_01_USCALER_INI_V_ADJ_SEL);
        }

        // turn on filter for negative initial phase
        vRegWriteFldAlign(SCPIP_US1_01,(dispPrm->u1USclHInitSigned)?1:0,US1_01_HORIZONTAL_FILTER_OFF_1);

        // sub must adjust because of main path negative initial phase
        if (dispPrm->u1USclHInitSigned)
        {
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_ACTIVE_ADJ - 1, PIP2_08_ACTIVE_ADJ_2);
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_READ_DELAY - 1, PIP2_08_READ_DELAY_2);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_ACTIVE_ADJ, PIP2_08_ACTIVE_ADJ_2);
            vRegWriteFldAlign(SCPIP_PIP2_08, SUB_READ_DELAY, PIP2_08_READ_DELAY_2);
        }
    }
    else
    {
        ASSERT(0); // only support main path negative initial phase
    }
}

/**
* @brief Set up scaler related registers
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetUSReg(UINT32 u4VdpId) 
{
    //if don't support POP function
    #if !SUPPORT_POP
    if(u4VdpId == VDP_2)
    {
        return;
    }
    #endif

    RVChannel* channel;
    RDispPrm* dispPrm;
    UINT8 usOffset;
    UINT8 bPath;
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    bPath = getScalerPath(u4VdpId);
    usOffset = getUSOffset(bPath);
    channel = getChannel(bPath);
    dispPrm = getDispPrm(bPath);

    /* Up Scaler for Main */
    if (channel->bIsChannelOn != SV_OFF)
    {  
        #ifdef CC_SCPOS_3DTV_SUPPORT
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)
            && (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)||fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
            && (dispPrm->u4USclV == 0x4000))
        {
            vRegWriteFldAlign(SCPIP_US1_00+usOffset,dispPrm->u4USclV+1,US1_00_USCALE_V_1);
        }        
        else
        #endif        
        {
            vRegWriteFldAlign(SCPIP_US1_00+usOffset,dispPrm->u4USclV,US1_00_USCALE_V_1);
        }
        if (_arScalePrm[u4VdpId].u4NonlinearCtrl == 0)
        {
            vRegWriteFldAlign(SCPIP_US1_00+usOffset,dispPrm->u4USclH,US1_00_USCALE_H_1);
            // clear non-linear setting
            if(u4VdpId == VDP_1)
            {
                vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONL_H_USCALE_1); 
            }
        } 
        else //non-linear scaling
        {  
            if(u4VdpId == VDP_1)
            {
                if(_arScalePrm[u4VdpId].u4NewNonlinear)
                {
                    vRegWriteFldAlign(SCPIP_US1_00, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StartFactor,US1_00_USCALE_H_1);
                    vRegWriteFldAlign(SCPIP_US1_01, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StepFactor, US1_01_NONL_H_USCALE_1); 
                    //vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_H_NEG_1); // always keep zero
                    vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4MiddlePoint, US1_15_NONL_H_MIDDLE_1);
                    vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNewNonlinearPrm.u1Step, US1_15_NONL_H_STEP_1);
                    vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StopPoint, US1_15_NONL_H_STOP_POINT_1);                            
                }
                else // old non-linear (convert register value to new HW)
                {
                    static const UINT32 _au4StepFactorTable[16] = {0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};            
                    UINT32 u4StartFactor = (_arScalePrm[u4VdpId].rNonlinearPrm.u4Start << 4);
                    UINT32 u4EndFactor = (_arScalePrm[u4VdpId].rNonlinearPrm.u4End << 4);
                    UINT32 u4StepFactor = _au4StepFactorTable[_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope];
                    UINT32 u4StopPoint = (u4EndFactor - u4StartFactor)/u4StepFactor;
                    if(u4StopPoint > _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint << 2)					
                    {                       
                        u4StopPoint = _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint << 2; // to avoid unreasonable stop point value                  
                    }                    
                    vRegWriteFldAlign(SCPIP_US1_00, u4StartFactor,US1_00_USCALE_H_1);
                    vRegWriteFldAlign(SCPIP_US1_01, u4StepFactor, US1_01_NONL_H_USCALE_1); 
                    //vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_H_NEG_1); // always keep zero
                    vRegWriteFldAlign(SCPIP_US1_15, _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint << 2, US1_15_NONL_H_MIDDLE_1);
                    vRegWriteFldAlign(SCPIP_US1_15, 1, US1_15_NONL_H_STEP_1);
                    vRegWriteFldAlign(SCPIP_US1_15, u4StopPoint, US1_15_NONL_H_STOP_POINT_1);            
                }
            }
        }   
    }
}


/**
* @brief Set two frame limit
* @param u4VdpId VDP_1/VDP_2
*/
#if CC_TWO_FRAME_LIMIT
static void _vScpipSetTwoFrameLimit(UINT32 u4VdpId)
{
    UINT8 pipOffset;
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);

    //if (RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_FSEL_1) == 1) // two frame
    if (u4ScpipGetBufNum(u4VdpId) == 2) // two frame
    {
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, (dispPrm->wVLen/16)-1, PIP1_09_TWO_FRAME_LIMIT_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 0x0, PIP1_09_TWO_FRAME_LIMIT_1);
    }
}
#endif


/**
* @brief Set display related registers
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipSetDispReg(UINT32 u4VdpId)
{
    #if !SUPPORT_POP
        if(u4VdpId == VDP_2)
        {
            return;
        }
    #endif

    //RVChannel* channel;
    RDispPrm* dispPrm;
    UINT8 dsOffset;
    UINT8 pipOffset;
    UINT8 bPath;
    UINT8 bMode;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);
    //channel = getChannel(bPath);
    bMode = u1ScpipGetDispMode(bPath);    
                
    /* Down Scaler for Main */
    /* DTV00209813, update display window even when no source is selected */
    //if (channel->bIsChannelOn == SV_ON)
    {
        UINT32 u4Val1, u4Val2;
        UINT32 u4RHP, u4RHL, u4RVP, u4RVL;
        #ifdef CC_FLIP_MIRROR_SUPPORT
        UINT8 u1Config = u1GetFlipMirrorConfig();
        #endif

        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_VD10BSEL_1) != u4ScpipGet10BitMode(u4VdpId))
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif        
        // 1. set 8 bit/10bit
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4ScpipGet10BitMode(u4VdpId), DS1_02_VD10BSEL_1);
        
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            vRegWriteFldAlign(SCPIP_PIP1_11+pipOffset, u4ScpipGet10BitMode(u4VdpId), PIP1_11_R10B_SEL_DUMP);
        }        
        #endif                  
        /*2. Set 444/422 Selector & blanksel*/
        u4Val1 = (bIsScalerInput444(bPath) == SV_TRUE)? 1 :0;
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4Val1, DS1_02_SEL422TO444_1);
        
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            vRegWriteFldAlign(SCPIP_PIP1_11+pipOffset, u4Val1, PIP1_11_SEL444_DUMP);
        }        
        #endif         

        u4Val2 = (u1ScpipGet444Mode(bPath) == SV_TRUE)? 1 :0;
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, u4Val2, PIP1_09_BLANKSEL_1);
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_DS1_02+dsOffset, DS1_02_SEL444_1) != u4Val1)
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif  
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, u4Val2, DS1_02_SEL444_1);
        
        /*3. RMVP/RMHP/RMVL/RMHL -- Display Window Position and Size */
        u4RHP = dispPrm->wRHPos;
        u4RHL = dispPrm->wRHLen; 
        u4RVP = dispPrm->wRVPos;
        u4RVL = dispPrm->wRVLen;

    	#if defined(CC_FLIP_MIRROR_SUPPORT)
    	if(u4GetFlipMirrorModule(VDP_1) !=FLIP_BY_MJC)
    	{
            if (u1Config & SYS_MIRROR_CONFIG_ON)
            {
                u4RHP = (wDISPLAY_WIDTH > (u4RHP+u4RHL))? (wDISPLAY_WIDTH - u4RHP - u4RHL): 0;
            }
            if (u1Config & SYS_FLIP_CONFIG_ON)
            {
                u4RVP = (wDISPLAY_WIDTH > (u4RVP+u4RVL))? (wDISPLAY_HEIGHT - u4RVP - u4RVL): 0;
            }
    	}
        #endif

        #if CC_SPEEDUP_IMPORT
        if ((RegReadFldAlign(SCPIP_PIP1_03+pipOffset, PIP1_03_RHP_1) != u4RHP)
            ||(RegReadFldAlign(SCPIP_PIP1_03+pipOffset, PIP1_03_RHL_1) != u4RHL)
            ||(RegReadFldAlign(SCPIP_PIP1_02+pipOffset, PIP1_02_RVP_1) != u4RVP)
            ||(RegReadFldAlign(SCPIP_PIP1_02+pipOffset, PIP1_02_RVL_1) != u4RVL)
            )
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R);
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, u4RHP, PIP1_03_RHP_1);
        vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, u4RHL, PIP1_03_RHL_1);
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, u4RVP, PIP1_02_RVP_1);
        #ifdef CC_MT5395
        vDrvMJCSet32PdInfoShiftPatch(u4RVP);
        #endif
        {
            vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, u4RVL, PIP1_02_RVL_1);
            
            #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
            if((fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
                || (SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
                && (VDP_2 == u4VdpId))
            {
                vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, 0, PIP1_02_RVL_1);
            }
            #endif                  
        }
        /*4. MVP/MHP/MVL/MHL -- Position inside the display window controlled by RMVP/RMHP/RMVL/RMHL */
        #if CC_SPEEDUP_IMPORT
        if ((RegReadFldAlign(SCPIP_PIP1_01+pipOffset, PIP1_01_HP_1) != dispPrm->wHPos)
            ||(RegReadFldAlign(SCPIP_PIP1_01+pipOffset, PIP1_01_HL_1) != dispPrm->wHLen)
            ||(RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VP_1) != dispPrm->wVPos)
            ||(RegReadFldAlign(SCPIP_PIP1_00+pipOffset, PIP1_00_VL_1) != dispPrm->wVLen)
            )
        {
            _au4ImportUpdate[u4VdpId] |= (SCPIP_UPDATE_R |SCPIP_UPDATE_W);
        }
        #endif  		
		#if CC_SCPOS_WA01
		if( (u4VdpId == VDP_1)&&
			(dispPrm->wVPosOfst == 0)&&
			(RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1) != dispPrm->wVLen)						
		  )
		{
			vRegWriteFldAlign(SCPIP_DS1_02,0, DS1_02_IMPORT_V_INV_1);
		}
		else if(u4VdpId == VDP_1)
		{
			vRegWriteFldAlign(SCPIP_DS1_02,1, DS1_02_IMPORT_V_INV_1);
		}
		#endif
        vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->wHPos, PIP1_01_HP_1);
        vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->wHLen, PIP1_01_HL_1);
        vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVPos, PIP1_00_VP_1);
        vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVLen, PIP1_00_VL_1);
        
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
        if((fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)       /* no matter inverse or not, TNB/FP will be the same */
            || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
            || ((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
            || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
            && (VDP_2 == u4VdpId))
        {           
            vRegWriteFldAlign(SCPIP_PIP2_01, dispPrm->wHLen, PIP2_01_HP_2);
        }
        if((fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
           && (u4VdpId == VDP_1))
        {
            vRegWriteFldAlign(SCPIP_PIP1_00, (dispPrm->wVLen) * 2, PIP1_00_VL_1);
        }
        #endif        

        #if SUPPORT_SBS_CONVERT
        if(_bSBSOutput && (u4VdpId == VDP_2))
        {
            vRegWriteFldAlign(SCPIP_PIP2_01, dispPrm->wHLen, PIP2_01_HP_2);
        }
        #endif        
        /*5. MHP_OFST -- Picture Target X Offset */
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_04+pipOffset, PIP1_04_HP_OFST_1) != dispPrm->wHPosOfst)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);

        /*6. WSTARTP_OFST -- Picture Target Y Offset */
        #ifdef __MODEL_slt__        
        // to fix error lines at the top of the screen
        u4Val1 = dispPrm->wVPosOfst+1;
        #else
        u4Val1 = dispPrm->wVPosOfst;
        #endif
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_04+pipOffset, PIP1_04_WSTARTP_OFST_1) != u4Val1)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_W;
        }
        #endif        
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, u4Val1, PIP1_04_WSTARTP_OFST_1);
        
        /*7. DATA_LENGTH */
        #if CC_SPEEDUP_IMPORT
        if (RegReadFldAlign(SCPIP_PIP1_08+pipOffset, PIP1_08_DATA_LENGTH_1) != dispPrm->wDataLen)
        {
            _au4ImportUpdate[u4VdpId] |= SCPIP_UPDATE_R;
        }
        #endif 

        if ((bPath == SV_VP_MAIN) && (bMode != VDP_SCPOS_DISPMODE_OFF))
        {
            _vScpipDispmodeLineCfg(bPath, bMode);
        }
         //[DTV00145304] line_cfg need to recal when change aspect 
        if((bPath == SV_VP_MAIN) && 
            (dispPrm->wDataLen != RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1)) && 
            ((bMode == VDP_SCPOS_DISPMODE_AUTO) || (bMode == VDP_SCPOS_DISPMODE_MANUAL)))
        { 
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);
        }
        vRegWriteFldAlign(SCPIP_PIP1_08+pipOffset, dispPrm->wDataLen, PIP1_08_DATA_LENGTH_1);
        
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
        if( fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
           || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            vRegWriteFldAlign(SCPIP_PIP1_10+pipOffset, (dispPrm->wDataLen)/2, PIP1_10_DATA_LENGTH_DUMP);
        }     
        #endif
        
        /*8. H Boundary protection */
        vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, dispPrm->u4HBound,PIP1_0F_BOUNDARY_H_POS_1);

        /*9. V Boundary protection */
        if(dispPrm->u4USclV == DFT_US_FACTOR)  // no up scale, turn off v boundary
        {
            if(u4VdpId == VDP_1)
            {
                vRegWriteFldAlign(SCPIP_US1_14, 0, US1_14_UP_VBOUND_EN1);
            }
            else
            {
                vRegWriteFldAlign(SCPIP_US2_03, 0, US2_03_UP_VBOUND_EN2);
            }
        }
        else
        {
            if(u4VdpId == VDP_1)
            {
                vRegWriteFldAlign(SCPIP_US1_14, 1, US1_14_UP_VBOUND_EN1);
                vRegWriteFldAlign(SCPIP_US1_14, dispPrm->u4VBound, US1_14_UP_VBOUND_Y1);
            }
            else //VDP_2
            {
                vRegWriteFldAlign(SCPIP_US2_03, 1, US2_03_UP_VBOUND_EN2);
                vRegWriteFldAlign(SCPIP_US2_03, dispPrm->u4VBound, US2_03_UP_VBOUND_Y2);
            }
        }

        /*10. Mirror */
        vRegWriteFldAlign(SCPIP_PIP1_0B + pipOffset, bScpipMirrorEnable[u4VdpId], PIP1_0B_HDIR_1);
        vRegWriteFldAlign(SCPIP_PIP1_0B + pipOffset, dispPrm->u4ReadInitP, PIP1_0B_READ_INITP_1);
        #if defined(CC_FLIP_MIRROR_SUPPORT)
        if (u4GetFlipMirrorModule(u4VdpId) == FLIP_BY_PSCAN)
        {
            vRegWriteFldAlign(SCPIP_PIP1_0F + pipOffset, (bIsScalerInput444(u4VdpId) ? 0 : 1), PIP1_0F_MIRROR_YCMATCH_1);
        }
        else
        #endif
        {
            vRegWriteFldAlign(SCPIP_PIP1_0F + pipOffset, dispPrm->u1Mirror422Reg, PIP1_0F_MIRROR_YCMATCH_1);
        }

        /*11. Flip */
        vRegWriteFldAlign(SCPIP_PIP1_0B + pipOffset, bScpipFlipEnable[u4VdpId], PIP1_0B_VDIR_1); 

        /* 5366 3D PR WA for test RealD */
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_REALD))
        {
            vOSTGSetBorderColor(VDP_1, 0x0); /*set color ==> black*/
            vOSTGSetBorderParam(VDP_1, 0, PANEL_GetPanelHeight()-4, PANEL_GetPanelWidth(), 4, 2);
            vOSTGSetBorderOnOff(VDP_1, SV_ON);
        }
        else
        {
            vOSTGSetBorderOnOff(VDP_1, SV_OFF);
        }
        #endif            
    }
}

#if defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5395)
static void _vScpipSetTv3dReg(UINT32 u4VdpId)
{
    UINT8 pipOffset = getPIPOffset(getScalerPath(u4VdpId));
    RDispPrm* dispPrm = getDispPrm(getScalerPath(u4VdpId));    
    static BOOL _fgLast3dStatus[VDP_NS] = {FALSE, FALSE};
    
    // if any of the following condition is met, mute the video
    // 1. 3d timing change 
    // 2. 3d <--> 2d switch
    if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D) 
        || (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D) != _fgLast3dStatus[u4VdpId]))                
    {
        _fgLast3dStatus[u4VdpId] = fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D);
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u4VdpId, FOREVER_MUTE, TRUE);
        if((u1ScpipGetDispMode(getScalerPath(u4VdpId)) == VDP_SCPOS_DISPMODE_OFF) && _bSRMStable[u4VdpId])
        {
            vScpipSrmSetBuf(u4VdpId,&rScalerDramInfo);
        }
        
        LOG(3, "Clear 3D related registers\n");

        // Main path only settings
        if(u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_TV3D_PORCH_EN);
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_TV3D_WIDTH);                    
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_TV3D_GEN_SIGANL_EN);
            vRegWriteFldAlign(SCPIP_PIP1_0D, 0, PIP1_0D_TV3D_FRAME_LINE_MODE_1);                
            vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_VLMASK_1);                    
            vRegWriteFldAlign(SCPIP_PIP1_0D, 0, PIP1_0D_TV3D_LINE_INTER_EN_1);  
            vRegWriteFldAlign(SCPIP_PIP1_0C, 0, PIP1_0C_DISPMODE_FIFO_SEP_REG_1);
            vRegWriteFldAlign(SCPIP_PIP1_11, 0, PIP1_11_REG_SHUTTER);
            vRegWriteFldAlign(SCPIP_PIP1_11, 0, PIP1_11_REG_SBS_TO_FP);
            vRegWriteFldAlign(SCPIP_PIP1_11, 0, PIP1_11_REG_3D_TO_2D);
            vRegWriteFldAlign(SCPIP_SYSTEM_01, 0, SYSTEM_01_TV3D_OINV);                        
            vOSTGSetBorderOnOff(VDP_1, SV_OFF);                
            _fgFprToggleOn = FALSE;
            _fgTnbWAToggleOn = FALSE;
            _fgSensioWAOn = FALSE;

             // LR shift related
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_SYNC_RIGHT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_DATA_RIGHT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_SYNC_LEFT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_DATA_LEFT_EN); 
            vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ, PIP_OA_02_HSYNC_ADJ);
            vRegWriteFldAlign(SCPIP_PIP1_08, MAIN_ACTIVE_ADJ, PIP1_08_ACTIVE_ADJ_1);
            
        }

        // Main + Sub
        vRegWriteFldAlign(SCPIP_DS1_01+pipOffset, 0x0, DS1_01_DSCALE_VOFST_1);                                            
        vScpipSetWPos(u4VdpId, SV_FALSE, 0);
        vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 0, PIP1_09_RLH_OFF_1);                  
        _fgFS3d22dToggleOn[u4VdpId] = FALSE;
        //_fgFS322WAInited[u4VdpId] = FALSE;                
    }
    
    if(_fgLast3dStatus[u4VdpId])
    {
        UINT32 u4LeftPorch = 0;
        UINT32 u4RightPorch = 0;
        BOOL fgValid3dtvOp = TRUE;
        
        LOG(3, "VDP(%d) 3D Operation ...\n", u4VdpId);
        // Main only operations
        if(u4VdpId == VDP_1)
        {
            if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE))
            {
                LOG(3, "SCPIP_TV3D_LINE_INTERLEAVE\n");
                vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);                    
                vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);                    
                if(LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_RLRL)
                {
                    vRegWriteFldAlign(SCPIP_PIP1_0D, 1, PIP1_0D_TV3D_LINE_INTER_EN_1);                            
                }                                                
            }  
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FRAME_INTERLEAVE))
            {
                LOG(3, "SCPIP_TV3D_FRAME_INTERLEAVE\n");                            
                vRegWriteFldAlign(SCPIP_PIP1_0D, 1, PIP1_0D_TV3D_FRAME_LINE_MODE_1);
                vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->wVLen >> 1, PIP1_00_VL_1);                        
                // turn off VLMASK in frame packing to frame interleave mode
                vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_VLMASK_1); 
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR))
            {
                LOG(3, "SCPIP_TV3D_FHD_PR\n");                                                
                vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);                    
                vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);  
                vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SBS_TO_FP); 
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);  
                vScpipSetWPos(VDP_1, SV_TRUE, 2);
                _fgFprToggleOn = TRUE;
                _u1FprSetWPosCounter = 1;
                if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FHD_PR_USE_V2))    
                {
                    // turn on OSTG border to mask the garbage of the first 2 lines
                    vOSTGSetBorderColor(VDP_1, 0x0); // set color ==> black
                    #ifdef CC_FLIP_MIRROR_SUPPORT
                    if (u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON) // flip
                    {
                        vOSTGSetBorderParam(VDP_1, 0, PANEL_GetPanelHeight() - 2 , PANEL_GetPanelWidth(), 2, 1);
                    }
                    else
                    #endif
                    {
                        vOSTGSetBorderParam(VDP_1, 0, 0, PANEL_GetPanelWidth(), 2, 1);
                    }
                    vOSTGSetBorderOnOff(VDP_1, SV_ON);
                }
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA))
            {
                LOG(3, "SCPIP_TV3D_TNB_SW_WA\n");
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);
                vScpipSetWPos(VDP_1, SV_TRUE, 1);
                _fgTnbWAToggleOn = TRUE;                    
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SHUTTER_OUT))
            {
                LOG(3, "SCPIP_TV3D_SHUTTER_OUT\n");                                                
                #if 1//!defined(DRV_USE_EXTERNAL_3D_FRC)//when 3d input and out type is native or FS
                vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SHUTTER);
                #endif
                if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT))
                {
                    LOG(3, "SCPIP_TV3D_SBSH_SPLIT\n");                                                
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);                    
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);                          
                    vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SBS_TO_FP);                        
                    if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_REALD) 
                        &&(_pPicInfo[u4VdpId]->rOverScan.u4Bottom == 0))    
                    {
                        // turn on OSTG border to mask the garbage of the last 4 lines
                        vOSTGSetBorderColor(VDP_1, 0x0); // set color ==> black
                        #ifdef CC_FLIP_MIRROR_SUPPORT
                        if (u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON) // flip
                        {
                            vOSTGSetBorderParam(VDP_1, 0, 0 , PANEL_GetPanelWidth(), 0, 4);
                        }
                        else
                        #endif
                        {
                            vOSTGSetBorderParam(VDP_1, 0, PANEL_GetPanelHeight()-4, PANEL_GetPanelWidth(), 4, 2);
                        }
                        vOSTGSetBorderOnOff(VDP_1, SV_ON);
                    }                    
                }
                else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT))
                {
                    LOG(3, "SCPIP_TV3D_TNB_SPLIT\n");
                }
                else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT))
                {
                    LOG(3, "SCPIP_TV3D_SBS_Q_SPLIT\n");
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);
                    vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SBS_TO_FP);
                    if(!IS_LVDS_DISP_3D_POLARIZED)
                    {
                        vRegWriteFldAlign(SCPIP_DS1_01+pipOffset, 0x4000, DS1_01_DSCALE_VOFST_1);                                                        
                    }
                }
                else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT))
                {
                    LOG(3, "SCPIP_TV3D_SBS_Q_I_SPLIT\n");
                    vRegWriteFldAlign(SCPIP_DS1_01, 0x4000, DS1_01_DSCALE_VOFST_1);                            
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);
                    vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SBS_TO_FP);
                    vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);
                    _fgSensioWAOn = TRUE;
                }
                else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
                {
                    LOG(3, "SCPIP_TV3D_INTERLEAVE_SPLIT\n"); 
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_PORCH_EN);
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, dispPrm->u4Tv3dWidth, SYSTEM_02_TV3D_WIDTH);                    
                    vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_TV3D_GEN_SIGANL_EN);                          
                    vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_SBS_TO_FP);                        
                }                     
            }            
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_FS))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_FS\n");
                #if 0 // SW 3d to 2d
                _fgFS3d22dToggleOn[u4VdpId] = TRUE;
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);                    
                vScpipSetWPos(u4VdpId, SV_TRUE, 1);                    
                #endif
                vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_REG_3D_TO_2D);                
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_SBS_Q\n");
                vRegWriteFldAlign(SCPIP_DS1_01+pipOffset, 0x4000, DS1_01_DSCALE_VOFST_1);
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_SBS_Q_I\n");
                vRegWriteFldAlign(SCPIP_DS1_01+pipOffset, 0x4000, DS1_01_DSCALE_VOFST_1);
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);
                _fgSensioWAOn = TRUE;
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_SBS\n");
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_TNB\n");
            }
            else
            {
                fgValid3dtvOp = FALSE;
            }
        }
        else // Sub only operations
        {
            if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_FS))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_FS\n");
                _fgFS3d22dToggleOn[u4VdpId] = TRUE;
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_RLH_OFF_1);                    
                vScpipSetWPos(u4VdpId, SV_TRUE, 1);                    
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_SBS\n");
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q))
            {
                vRegWriteFldAlign(SCPIP_DS1_01+pipOffset, 0x4000, DS1_01_DSCALE_VOFST_1);                            
                LOG(3, "SCPIP_TV3D_3D_TO_2D_SBS_Q\n");
            }            
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
            {
                LOG(3, "SCPIP_TV3D_3D_TO_2D_TNB\n");
            }
            else
            {
                fgValid3dtvOp = FALSE;
            }
        }

        if(!fgValid3dtvOp) // 3d but no specified type
        {
            LOG(3, "!!!!INVALID CONDITION!!!!\n");                                                
        }
        ASSERT(fgValid3dtvOp); 
        
        // L/R shift               
        if(dispPrm->i1LShift > 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_DELAY_DATA_LEFT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_02, MIN(dispPrm->i1LShift - 1, 0xF), SYSTEM_02_DELAY_DATA_LEFT);
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_SYNC_LEFT_EN); 
            u4LeftPorch = MIN(dispPrm->i1LShift - 1, 0xF) + 1;
        }
        else if (dispPrm->i1LShift < 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 1, SYSTEM_02_DELAY_SYNC_LEFT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_02, MIN((-dispPrm->i1LShift) - 1, 0xF), SYSTEM_02_DELAY_SYNC_LEFT);
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_DATA_LEFT_EN);             
            u4RightPorch = MIN((-dispPrm->i1LShift) - 1, 0xF) + 1;
        }
        else //(dispPrm->i1LShift == 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_DATA_LEFT_EN);             
            vRegWriteFldAlign(SCPIP_SYSTEM_02, 0, SYSTEM_02_DELAY_SYNC_LEFT_EN);             
        }
        
        if(dispPrm->i1RShift > 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 1, SYSTEM_03_DELAY_DATA_RIGHT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_03, MIN(dispPrm->i1RShift - 1, 0xF), SYSTEM_03_DELAY_DATA_RIGHT);
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_SYNC_RIGHT_EN);             
            u4LeftPorch = MAX(u4LeftPorch, MIN(dispPrm->i1RShift - 1, 0xF) + 1);
        }
        else if (dispPrm->i1RShift < 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 1, SYSTEM_03_DELAY_SYNC_RIGHT_EN); 
            vRegWriteFldAlign(SCPIP_SYSTEM_03, MIN((-dispPrm->i1RShift) - 1, 0xF), SYSTEM_03_DELAY_SYNC_RIGHT);
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_DATA_RIGHT_EN);             
            u4RightPorch = MAX(u4RightPorch, MIN((-dispPrm->i1RShift) - 1, 0xF) + 1);
        }
        else //(dispPrm->i1RShift == 0)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_DATA_RIGHT_EN);             
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_DELAY_SYNC_RIGHT_EN);             
        }                

        {
            UINT32 u4RHP;                    
            UINT32 u4RHL;

            if(bScpipMirrorEnable[VDP_1]==SV_TRUE)
            {
                SWAP(u4LeftPorch, u4RightPorch);
            }
            
            u4LeftPorch =  DIVIDE_WITH_CARRIER((u4LeftPorch * DFT_US_FACTOR), dispPrm->u4USclH);
            u4RightPorch =  DIVIDE_WITH_CARRIER((u4RightPorch * DFT_US_FACTOR), dispPrm->u4USclH);
            vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ+u4LeftPorch, PIP_OA_02_HSYNC_ADJ);
            vRegWriteFldAlign(SCPIP_PIP1_08, MAIN_ACTIVE_ADJ+u4LeftPorch, PIP1_08_ACTIVE_ADJ_1);

            u4RHP = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHP_1);
            vRegWriteFldAlign(SCPIP_PIP1_03, u4RHP+((u4LeftPorch+u4RightPorch)>>1), PIP1_03_RHP_1);
            u4RHL = RegReadFldAlign(SCPIP_PIP1_03, PIP1_03_RHL_1);
            vRegWriteFldAlign(SCPIP_PIP1_03, u4RHL-(u4LeftPorch+u4RightPorch), PIP1_03_RHL_1);
        }
    }
}
#endif 

/**
* @brief Calculate dynamic scale parameters
* @param u4VdpId VDP_1/VDP_2
* @param u4SrcWidth
* @param u4SrcHeight
* @param u4SrcXOffset
* @param u4SrcYOffset
* @param u4OutWidth
* @param u4OutHeight
* @param u4OutXOffset
* @param u4OutYOffset
*/
static UINT32 _vScpipCalDynamicScale(UINT32 u4VdpId,
                                       UINT32 u4SrcWidth, UINT32 u4SrcHeight,  UINT32 u4SrcXOffset, UINT32 u4SrcYOffset, 
                                       UINT32 u4OutWidth,  UINT32 u4OutHeight,  UINT32 u4OutXOffset,   UINT32 u4OutYOffset)
{
    
#ifdef CC_SCPOS_NO_DS_NONLINEAR
    if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP)) ||
        (_arScalePrm[u4VdpId].u4NonlinearEn == 1))
#else
    if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP)))
#endif  
    {
        // dead lock protection
        _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;

        _arDynamicPrm[u4VdpId].u4LastFactorValid = 1;

        _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
        _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
        _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
        _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;

        _arDynamicPrm[u4VdpId].u4LastOutWidth = u4OutWidth;
        _arDynamicPrm[u4VdpId].u4LastOutHeight = u4OutHeight;
        _arDynamicPrm[u4VdpId].u4LastOutXOffset = u4OutXOffset;
        _arDynamicPrm[u4VdpId].u4LastOutYOffset = u4OutYOffset;

        if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastSrcHeight == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastOutWidth == 0) ||
            (_arDynamicPrm[u4VdpId].u4LastOutHeight == 0))
        {
            _arDynamicPrm[u4VdpId].u4LastFactorValid = 0;

            LOG(3, "Last Invalid\n");
        }    
    }

    else
    {
        UINT32 u4UseCurrentValue;
    
        // Handle Dynamic Scaling
        u4UseCurrentValue = 0;
        if (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE)
        {
            if ((_arDynamicPrm[u4VdpId].u4TotalScaleNs > 1) &&
                (_arDynamicPrm[u4VdpId].u4LastFactorValid != 0))
            {
                INT32 i4Var;

                // Backup Original/Final Source/Output Region
                _arDynamicPrm[u4VdpId].u4OrgSrcWidth = u4SrcWidth;
                _arDynamicPrm[u4VdpId].u4OrgSrcHeight = u4SrcHeight;
                _arDynamicPrm[u4VdpId].u4OrgSrcXOffset = u4SrcXOffset;
                _arDynamicPrm[u4VdpId].u4OrgSrcYOffset = u4SrcYOffset;

                _arDynamicPrm[u4VdpId].u4OrgOutWidth = u4OutWidth;
                _arDynamicPrm[u4VdpId].u4OrgOutHeight = u4OutHeight;
                _arDynamicPrm[u4VdpId].u4OrgOutXOffset = u4OutXOffset;
                _arDynamicPrm[u4VdpId].u4OrgOutYOffset = u4OutYOffset;

                // Last Value Protection
                if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth + _arDynamicPrm[u4VdpId].u4LastSrcXOffset) >
                    (_pPicInfo[u4VdpId]->wSrcW))
                {
                    LOG(3, "DS Protection-H (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcXOffset, _arDynamicPrm[u4VdpId].u4LastSrcWidth);
                    _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
                }

                if ((_arDynamicPrm[u4VdpId].u4LastSrcHeight + _arDynamicPrm[u4VdpId].u4LastSrcYOffset) >
                    (_pPicInfo[u4VdpId]->wSrcH))
                {
                    LOG(3, "DS Protection-V (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcYOffset, _arDynamicPrm[u4VdpId].u4LastSrcHeight);
                    _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;
                }

                // Calculate Source Region Step (New - Last)
                i4Var = (((INT32) u4SrcWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth));
                _arDynamicPrm[u4VdpId].i4SrcWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight));
                _arDynamicPrm[u4VdpId].i4SrcHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset));
                _arDynamicPrm[u4VdpId].i4SrcXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4SrcYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset));
                _arDynamicPrm[u4VdpId].i4SrcYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);


                // Calculate Output Region Step (New - Last)
                i4Var = (((INT32) u4OutWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth));
                _arDynamicPrm[u4VdpId].i4OutWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight));
                _arDynamicPrm[u4VdpId].i4OutHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset));
                _arDynamicPrm[u4VdpId].i4OutXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                i4Var = (((INT32) u4OutYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset));
                _arDynamicPrm[u4VdpId].i4OutYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                LOG(3, "Step(%d, %d) S(%d %d %d %d) O(%d %d %d %d)\n",
                    _arDynamicPrm[u4VdpId].u4TotalScaleNs,
                    _arScalePrm[u4VdpId].u4ResponseTime,
                    _arDynamicPrm[u4VdpId].i4SrcXOffsetStep,
                    _arDynamicPrm[u4VdpId].i4SrcYOffsetStep,
                    _arDynamicPrm[u4VdpId].i4SrcWidthStep,
                    _arDynamicPrm[u4VdpId].i4SrcHeightStep,
                    _arDynamicPrm[u4VdpId].i4OutXOffsetStep,
                    _arDynamicPrm[u4VdpId].i4OutYOffsetStep,
                    _arDynamicPrm[u4VdpId].i4OutWidthStep,
                    _arDynamicPrm[u4VdpId].i4OutHeightStep);

                // Update Current Source/Output Region
                _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);

                // Update Step
                _arDynamicPrm[u4VdpId].u4ScaleStep = 1;

                // Update State
                _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_SCALING;

                // Reset Vsync Count
                _arDynamicPrm[u4VdpId].u4VsyncCount = 0;

                u4UseCurrentValue = 1;
            }

            // Update Last Source/Output Region
            _arDynamicPrm[u4VdpId].u4LastFactorValid = 1;

            _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
            _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
            _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
            _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;

            _arDynamicPrm[u4VdpId].u4LastOutWidth = u4OutWidth;
            _arDynamicPrm[u4VdpId].u4LastOutHeight = u4OutHeight;
            _arDynamicPrm[u4VdpId].u4LastOutXOffset = u4OutXOffset;
            _arDynamicPrm[u4VdpId].u4LastOutYOffset = u4OutYOffset;

            if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastSrcHeight == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastOutWidth == 0) ||
                (_arDynamicPrm[u4VdpId].u4LastOutHeight == 0))
            {
                _arDynamicPrm[u4VdpId].u4LastFactorValid = 0;

                LOG(3, "Last Invalid\n");
            }            
        }
        else
        {
            if ((_arDynamicPrm[u4VdpId].u4OrgSrcWidth == u4SrcWidth) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcHeight == u4SrcHeight) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcXOffset == u4SrcXOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgSrcYOffset == u4SrcYOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutWidth == u4OutWidth) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutHeight == u4OutHeight) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutXOffset == u4OutXOffset) &&
                (_arDynamicPrm[u4VdpId].u4OrgOutYOffset == u4OutYOffset))
            {
                if (++_arDynamicPrm[u4VdpId].u4ScaleStep < _arDynamicPrm[u4VdpId].u4TotalScaleNs)
                {
                    // Update Current Source/Output Region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                    _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4CurOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);
                
                    u4UseCurrentValue = 1;
                }
                else
                {
                    _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;

                    // For 121 Mapping

                    // Set source region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = u4SrcXOffset;
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = u4SrcYOffset;
                
                    // Change display region
                    _arDynamicPrm[u4VdpId].u4CurOutWidth = u4OutWidth;
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = u4OutHeight;
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = u4OutXOffset;
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = u4OutYOffset;
                }
            }
            else
            {
                // Upper Layer ask for rescale during dynamic scaling                
                if (_arDynamicPrm[u4VdpId].u4TotalScaleNs > 1)
                {
                    INT32 i4Var;

                    // Last Region = Current Region
                    _arDynamicPrm[u4VdpId].u4LastSrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
                    _arDynamicPrm[u4VdpId].u4LastSrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
                    _arDynamicPrm[u4VdpId].u4LastSrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
                    _arDynamicPrm[u4VdpId].u4LastSrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

                    _arDynamicPrm[u4VdpId].u4LastOutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
                    _arDynamicPrm[u4VdpId].u4LastOutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight ;
                    _arDynamicPrm[u4VdpId].u4LastOutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset ;
                    _arDynamicPrm[u4VdpId].u4LastOutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;

                    // Backup Original/Final Source/Output Region
                    _arDynamicPrm[u4VdpId].u4OrgSrcWidth = u4SrcWidth;
                    _arDynamicPrm[u4VdpId].u4OrgSrcHeight = u4SrcHeight;
                    _arDynamicPrm[u4VdpId].u4OrgSrcXOffset = u4SrcXOffset;
                    _arDynamicPrm[u4VdpId].u4OrgSrcYOffset = u4SrcYOffset;

                    _arDynamicPrm[u4VdpId].u4OrgOutWidth = u4OutWidth;
                    _arDynamicPrm[u4VdpId].u4OrgOutHeight = u4OutHeight;
                    _arDynamicPrm[u4VdpId].u4OrgOutXOffset = u4OutXOffset;
                    _arDynamicPrm[u4VdpId].u4OrgOutYOffset = u4OutYOffset;

                    // Last Value Protection
                    if ((_arDynamicPrm[u4VdpId].u4LastSrcWidth + _arDynamicPrm[u4VdpId].u4LastSrcXOffset) >
                        (_pPicInfo[u4VdpId]->wSrcW))
                    {
                        LOG(3, "DS Protection-H (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcXOffset, _arDynamicPrm[u4VdpId].u4LastSrcWidth);
                        _arDynamicPrm[u4VdpId].u4LastSrcWidth = u4SrcWidth;
                        _arDynamicPrm[u4VdpId].u4LastSrcXOffset = u4SrcXOffset;
                    }

                    if ((_arDynamicPrm[u4VdpId].u4LastSrcHeight + _arDynamicPrm[u4VdpId].u4LastSrcYOffset) >
                        (_pPicInfo[u4VdpId]->wSrcH))
                    {
                        LOG(3, "DS Protection-V (%d, %d)\n", _arDynamicPrm[u4VdpId].u4LastSrcYOffset, _arDynamicPrm[u4VdpId].u4LastSrcHeight);
                        _arDynamicPrm[u4VdpId].u4LastSrcHeight = u4SrcHeight;
                        _arDynamicPrm[u4VdpId].u4LastSrcYOffset = u4SrcYOffset;
                    }

                    // Calculate Source Region Step (New - Last)
                    i4Var = (((INT32) u4SrcWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth));
                    _arDynamicPrm[u4VdpId].i4SrcWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight));
                    _arDynamicPrm[u4VdpId].i4SrcHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset));
                    _arDynamicPrm[u4VdpId].i4SrcXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4SrcYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset));
                    _arDynamicPrm[u4VdpId].i4SrcYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    // Calculate Output Region Step (New - Last)
                    i4Var = (((INT32) u4OutWidth) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth));
                    _arDynamicPrm[u4VdpId].i4OutWidthStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutHeight) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight));
                    _arDynamicPrm[u4VdpId].i4OutHeightStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutXOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset));
                    _arDynamicPrm[u4VdpId].i4OutXOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    i4Var = (((INT32) u4OutYOffset) - ((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset));
                    _arDynamicPrm[u4VdpId].i4OutYOffsetStep = i4Var / ((INT32) _arDynamicPrm[u4VdpId].u4TotalScaleNs);

                    LOG(3, "Step(%d, %d) S(%d %d %d %d) O(%d %d %d %d)\n",
                        _arDynamicPrm[u4VdpId].u4TotalScaleNs,
                        _arScalePrm[u4VdpId].u4ResponseTime,
                        _arDynamicPrm[u4VdpId].i4SrcXOffsetStep,
                        _arDynamicPrm[u4VdpId].i4SrcYOffsetStep,
                        _arDynamicPrm[u4VdpId].i4SrcWidthStep,
                        _arDynamicPrm[u4VdpId].i4SrcHeightStep,
                        _arDynamicPrm[u4VdpId].i4OutXOffsetStep,
                        _arDynamicPrm[u4VdpId].i4OutYOffsetStep,
                        _arDynamicPrm[u4VdpId].i4OutWidthStep,
                        _arDynamicPrm[u4VdpId].i4OutHeightStep);

                    // Update Current Source/Output Region
                    _arDynamicPrm[u4VdpId].u4CurSrcWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcWidth) + _arDynamicPrm[u4VdpId].i4SrcWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcHeight ) + _arDynamicPrm[u4VdpId].i4SrcHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcXOffset) + _arDynamicPrm[u4VdpId].i4SrcXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurSrcYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastSrcYOffset) + _arDynamicPrm[u4VdpId].i4SrcYOffsetStep);

                    _arDynamicPrm[u4VdpId].u4CurOutWidth = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutWidth) + _arDynamicPrm[u4VdpId].i4OutWidthStep);
                    _arDynamicPrm[u4VdpId].u4CurOutHeight = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutHeight ) + _arDynamicPrm[u4VdpId].i4OutHeightStep);
                    _arDynamicPrm[u4VdpId].u4CurOutXOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutXOffset) + _arDynamicPrm[u4VdpId].i4OutXOffsetStep);
                    _arDynamicPrm[u4VdpId].u4CurOutYOffset = (UINT32) (((INT32) _arDynamicPrm[u4VdpId].u4LastOutYOffset) + _arDynamicPrm[u4VdpId].i4OutYOffsetStep);

                    // Update Step
                    _arDynamicPrm[u4VdpId].u4ScaleStep = 1;

                    // Update State
                    _arDynamicPrm[u4VdpId].u4State = VDP_DYNAMICE_SCALE_SCALING;

                    // Reset Vsync Count
                    _arDynamicPrm[u4VdpId].u4VsyncCount = 0;

                    u4UseCurrentValue = 1;
                }
            }
        }
        return u4UseCurrentValue;
    }
    return 0;
}


#if 0
static void _vScpipCheckDramSize(UINT32 u4VdpId)
{
    if ((u1ScpipGetDispMode(getScalerPath(u4VdpId)) == VDP_SCPOS_DISPMODE_OFF) && _bSRMStable[u4VdpId])  // dram mode
    {
        if((_arScalePrm[u4VdpId].u4DramHeight < _pPicInfo[u4VdpId]->wTargetH) && (_arScalePrm[u4VdpId].u4DramHeight < _pDispInfo[u4VdpId]->wRHeight))
        {
            Printf("===============================================\n");
            Printf("[Error] SRM allocated buffer for Scaler(%d) is too small!!, Please call SRM owner to check this.\n",u4VdpId);
            Printf("[Error] Scaler Dram Height:%d, Input height:%d, Output height:%d\n",_arScalePrm[u4VdpId].u4DramHeight , _pPicInfo[u4VdpId]->wTargetH, _pDispInfo[u4VdpId]->wRHeight);
            Printf("===============================================\n");
            ASSERT(0);
        }
    }
}
#endif

#define SCPIP_DEBUG_DRAM_HEIGHT 10
static void _vScpipCalDramLimit(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4OutWidth, UINT32 u4OutHeight, UINT32 *u4DramW, UINT32 *u4DramH)
{
    UINT32 u4WidthLimitation, u4HeightLimitation, u4DispWidth;//, u4DramWidth;
    
    u4HeightLimitation = wDISPLAY_HEIGHT;
    u4DispWidth = wDISPLAY_WIDTH/(u4VdpId+1);    
    u4WidthLimitation = u4DispWidth;
    
    if ((u1ScpipGetDispMode(getScalerPath(u4VdpId)) == VDP_SCPOS_DISPMODE_OFF) && _bSRMStable[u4VdpId])  // dram mode
    {
        #if 0 // do not check height limitation from FBM because it is not accurate
        if((_arScalePrm[u4VdpId].u4DramHeight < u4SrcHeight) && (_arScalePrm[u4VdpId].u4DramHeight < u4OutHeight))
        {
            Printf("===============================================\n");
            Printf("[Error] SRM allocated buffer for Scaler(%d) is too small!!, Please call SRM owner to check this.\n",u4VdpId);
            Printf("[Error] Scaler Dram Height:%d, Input height:%d, Output height:%d\n",_arScalePrm[u4VdpId].u4DramHeight ,u4SrcHeight,u4OutHeight);
            Printf("===============================================\n");
            //ASSERT(0);
        }
        #endif
        
        // vertical limitation
        if(u4HeightLimitation >u4SrcHeight)
                u4HeightLimitation =u4SrcHeight;

        if(u4HeightLimitation >u4OutHeight)
                u4HeightLimitation =u4OutHeight;
        
        #if 0 // do not check height limitation from FBM because it is not accurate
        if(u4HeightLimitation> _arScalePrm[u4VdpId].u4DramHeight)
            u4HeightLimitation = _arScalePrm[u4VdpId].u4DramHeight;
        #endif
        
        // horizontal limitation
        if(u4WidthLimitation > u4SrcWidth)
                u4WidthLimitation = u4SrcWidth;

        if(u4WidthLimitation > u4OutWidth)
                u4WidthLimitation = u4OutWidth;

        if(u4WidthLimitation> _arScalePrm[u4VdpId].u4DramWidth)
            u4WidthLimitation = _arScalePrm[u4VdpId].u4DramWidth;

        // check if current size setting exceeds FBM buffer size
        if((u4WidthLimitation * u4HeightLimitation) 
            > (_arScalePrm[u4VdpId].u4DramWidth * _arScalePrm[u4VdpId].u4DramHeight))
        {
            u4HeightLimitation = SCPIP_DEBUG_DRAM_HEIGHT; // this will cause scaler to do v up+down            
            Printf("===============================================\n");
            Printf("[_vScpipCalDramLimit] Scaler Dram Height:%d, Input height:%d, Output height:%d\n",_arScalePrm[u4VdpId].u4DramHeight ,u4SrcHeight,u4OutHeight);
            Printf("===============================================\n");            
        }

        LOG(3, "Scaler Dram Limit Path(%d) W(%d) H(%d)\n", u4VdpId, u4WidthLimitation, u4HeightLimitation);
     }

    *u4DramW = u4WidthLimitation;
    *u4DramH = u4HeightLimitation;
}


/**
* @brief Calculate new scale parameters
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipCalNewScale(UINT32 u4VdpId) 
{
    /* Get New US/DS Factors */
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4TargetW;  // input width after pre-down
    UINT32 u4TargetH;
    UINT32 u4WidthPlus;

    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4SrcXOffset;
    UINT32 u4SrcYOffset;

    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4OutXOffset;
    UINT32 u4OutYOffset;

    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;
    UINT32 u4WidthLimitation;    
    UINT32 u4HeightLimitation;
    UINT32 u4FifoLength;
    RDispPrm rDispPrm;
    UINT32 u4ScalerMode;

    UINT8 bPath;

    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    bPath = getScalerPath(u4VdpId);
    u4ScalerMode = u1ScpipGetDispMode(bPath);    
                                          
    // Check NR/PSCAN fifo limit
    if (_arScalePrm[u4VdpId].u4MddiDramWidth != SCPOS_BYPASS_PSCANNR)
    {
        u4MddiFifoMin = u4DrvDIMiniInputWidth(u4VdpId);
        u4MddiFifoMax = u4DrvDIMaxInputWidth(u4VdpId);

        if (_arScalePrm[u4VdpId].u4MddiDramWidth < u4MddiFifoMax)
        {
            u4MddiFifoMax = _arScalePrm[u4VdpId].u4MddiDramWidth;
            LOG(3, "DI DRAM Limit (%d)\n", _arScalePrm[u4VdpId].u4MddiDramWidth);        
        }    
    }
    else
    {                
        u4MddiFifoMin = 0;
        u4MddiFifoMax = 2000;
    }      
    
    #ifdef CC_SCPOS_3DTV_SUPPORT
    if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FP_I_HDS_HALF))
    {
        if(u4MddiFifoMax > 960)
        {
            u4MddiFifoMax = 960;
        }
    }
    #endif

    LOG(3, "DI Fifo(%d) range (%d - %d)\n", u4VdpId, u4MddiFifoMin, u4MddiFifoMax);

    // First Config #3
    if ((_arScalePrm[u4VdpId].u4SrcHeight != _pPicInfo[u4VdpId]->wSrcH) ||
        (_arScalePrm[u4VdpId].u4SrcWidth != _pPicInfo[u4VdpId]->wSrcW) ||
        (_arScalePrm[u4VdpId].u4MddiFifoMax != u4MddiFifoMax) ||
        (_arScalePrm[u4VdpId].u4MddiFifoMin != u4MddiFifoMin) ||
        (_pPicInfo[u4VdpId]->u1Interlace != bDrvVideoIsSrcInterlace(u4VdpId)))
    {
        _arScalePrm[u4VdpId].u4FirstConfig = 1;

        _arScalePrm[u4VdpId].u4SrcHeight = _pPicInfo[u4VdpId]->wSrcH;
        _arScalePrm[u4VdpId].u4SrcWidth = _pPicInfo[u4VdpId]->wSrcW;
        _arScalePrm[u4VdpId].u4MddiFifoMax = u4MddiFifoMax;
        _arScalePrm[u4VdpId].u4MddiFifoMin = u4MddiFifoMin;

        LOG(3, "1st Config#3\n");
    }

    u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
    u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
    u4SrcXOffset = getMixedHPorch(u4VdpId);
    u4SrcYOffset = getMixedVPorch(u4VdpId);
    u4OutWidth = _pDispInfo[u4VdpId]->wRWidth;
    u4OutHeight = _pDispInfo[u4VdpId]->wRHeight;
    u4OutXOffset = _pDispInfo[u4VdpId]->wRXOffset;
    u4OutYOffset = _pDispInfo[u4VdpId]->wRYOffset;
    u4WidthPlus = 0;
#ifdef IS_SUPPORT_3D_PHOTO
		if(IsFixRegion && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF))
		{
			if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
					||fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
			{
				u4OutWidth = _rM3DPhotoDispInfo.wRWidth;
				u4OutHeight = _rM3DPhotoDispInfo.wRHeight;
				u4OutXOffset = _rM3DPhotoDispInfo.wRXOffset;
				u4OutYOffset = _rM3DPhotoDispInfo.wRYOffset;
			}
			else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
					|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
			{
				//todo: TNB
				u4OutWidth = _rM3DPhotoDispInfo.wRWidth;
				u4OutHeight = _rM3DPhotoDispInfo.wRHeight;
				u4OutXOffset = _rM3DPhotoDispInfo.wRXOffset;
				u4OutYOffset = _rM3DPhotoDispInfo.wRYOffset;
			}
			//LOG(2, "\n*********replace _pDispInfo with _rM3DPhotoDispInfo*******\n");
		}
		else
		{
			//LOG(2, "\n*******back to _pDispInfo********\n");
		}
#endif

#ifdef CC_SCPOS_3DTV_SUPPORT
    //if(u4VdpId == VDP_1)
    {
        LOG(1, "_vScpipCalNewScale: before 3d u4SrcWidth=%d, u4SrcHeight=%d, u4OutHeight=%d\n", u4SrcWidth, u4SrcHeight, u4OutHeight);
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_REPEAT_DS_HALF))
        {
            u4OutHeight >>= 1;       // down scale to half of output height
        }
        else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))
        {
            
            if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR))
            {
                if(u4OutHeight != u4SrcHeight)
                {
                    u4OutHeight = u4SrcHeight;
                }
            }
            else
            {
                if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q))
                {
                    u4SrcWidth >>= 1; // side by side video width will be cut half by scaler
                                      // so we need to compute H scaling factor by 1/2 input width
                }
                else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
                        || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
                {
                    u4SrcWidth >>= 1; // side by side video width will be cut half by scaler
                                      // so we need to compute H scaling factor by 1/2 input width
                    u4SrcHeight >>= 1; // Tnb split in NR so the src height will be 1/2
                }
                else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
                {
                    u4SrcHeight >>= 1; // Tnb split in NR so the src height will be 1/2
                }                    
                else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
                {
                    u4SrcHeight >>= 1;  // interleave will be split by scaler, we compute V scaling factor by 1/2 input height
                }  
                
                if(IS_LVDS_DISP_3D_POLARIZED && (u4VdpId == VDP_1)
                    && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
                    && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                    && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)
                    && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
                {
                    if(((u4OutHeight >> 1) > u4SrcHeight) 
                    && (BSP_GetIcVersion() == IC_VER_5395_AA))
                    {
                        u4OutHeight = u4SrcHeight; // prevent 3d content to up-scale
                    }
                    else
                    {
                        u4OutHeight >>= 1;       // down scale to half of output height
                    }
                }
            }            
        }
        LOG(1, "_vScpipCalNewScale: after 3d u4SrcWidth=%d, u4SrcHeight=%d, u4OutHeight=%d\n", u4SrcWidth, u4SrcHeight, u4OutHeight);
    }
#endif    

    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(SV_FALSE == _bLRInverse) /* no  inverse */
    {
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR))
        {
            u4OutHeight >>= 1;
            if(VDP_1 == u4VdpId)    /* VDP_1 NOT CLIP SRC REGION */
            {
                u4SrcHeight = DIVIDE_WITH_ROUND(4898 * u4SrcHeight, 10000); 
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
        {
            u4SrcWidth >>= 1;
            u4OutHeight >>= 1;
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))          
        {
            u4OutHeight >>= 1;
            if(VDP_1 == u4VdpId)    /* VDP_1 NOT CLIP SRC REGION */
            {
                u4SrcHeight >>= 1;
            }
            if(720 == wDrvVideoInputHeight(VDP_1))
            {
                u4SrcHeight <<= 1;
                /*the last line in top will be wrong because it will reference to the first line of bottom because of DI up, so kick it out*/
                u4SrcHeight -= 2;
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            u4SrcWidth -= (_bTTDOverscan* 2);     /* overscan for TTD */
            u4OutHeight >>= 1;
        }
    }
    else
    {
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR))
        {
            u4OutHeight >>= 1;
            if(VDP_1 == u4VdpId)    /* VDP_1 NOT CLIP SRC REGION*/
            {
                u4SrcYOffset = DIVIDE_WITH_ROUND(5102 * u4SrcHeight, 10000);              
                u4SrcHeight = DIVIDE_WITH_ROUND(4898 * u4SrcHeight, 10000);   
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
        {
            u4SrcWidth >>= 1;                
            u4OutHeight >>= 1;
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))          
        {
            u4OutHeight >>= 1;
            if(VDP_1 == u4VdpId)    /* VDP_1 NOT CLIP SRC REGION*/
            {
                u4SrcHeight >>= 1;
                u4SrcYOffset = u4SrcHeight;
            }
            if(720 == wDrvVideoInputHeight(VDP_1))
            {
                u4SrcHeight <<= 1;
                if(VDP_1 == u4VdpId)
                {
                    u4SrcYOffset = u4SrcHeight;
                }                
                /*the last line in top will be wrong because it will reference to the first line of bottom because of DI up, so kick it out*/
                u4SrcHeight -= 2;
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            u4SrcWidth -= (_bTTDOverscan * 2);     /* overscan for TTD */        
            u4OutHeight >>= 1;
        }        
    }
    
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
       || ((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {   
        if((BSP_GetIcVersion() >= IC_VER_5365_AB) && IS_IC_5365())
        {
            u4MddiFifoMax = 960;
        }
        else 
        {
            u4MddiFifoMax = 720;
        }
    }   

    /* 720P TNB WINC_1 is not enough */
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR) && (720 == wDrvVideoInputHeight(VDP_1)))
    {
        u4MddiFifoMax = PANEL_GetPanelWidth()/2;
    }   
    #endif

    // window width or height is 0, no need to calculate hps factor
    if ((u4OutWidth == 0) || (u4OutHeight == 0)) return;
    
    // handle 121-mapping
    _arScalePrm[u4VdpId].u4121MapEn = 0;
    if ((_arScalePrm[u4VdpId].u4121Map) &&
        //(u4ScalerMode == VDP_SCPOS_DISPMODE_OFF))         // No 121Map at Display Mode
        ((u4ScalerMode != VDP_SCPOS_DISPMODE_AUTO) && (u4ScalerMode != VDP_SCPOS_DISPMODE_MANUAL)))
    {
        // Enable 121, If
        // 1) H/V Scaling Up
        // 2) MDDI FIFO > Source Width
        if ((u4OutWidth >= u4SrcWidth) && 
            (u4OutHeight >= u4SrcHeight) &&
            (u4MddiFifoMax >= _pPicInfo[u4VdpId]->wSrcW))
        {
            _arScalePrm[u4VdpId].u4121MapEn = 1;
        }
        
        // [DTV00112299] 720*400 needs to be dot by dot. Peggy 20080122
        else if (bDrvVideoGetWidthOverSample(u4VdpId) &&
            (u4OutWidth >= (u4SrcWidth >> 1)))
        {
            _arScalePrm[u4VdpId].u4121MapEn = 1;
        }
        
        else
        {
            LOG(3, "Skip 121 Mapping\n");
        }
    }
    
    if (_arScalePrm[u4VdpId].u4121MapEn)
    {
        UINT32 u4X;
        UINT32 u4Y;
        UINT32 u4W;
        UINT32 u4H;

        // Change display region
        if (bDrvVideoGetWidthOverSample(u4VdpId))
        {
            u4W = (u4SrcWidth >> 1) - u4WidthPlus;
            u4X = u4OutXOffset + ((u4OutWidth - (u4SrcWidth>>1)) >> 1);
        }
        else
        {
            u4W = u4SrcWidth - u4WidthPlus;
            u4X = u4OutXOffset + ((u4OutWidth - u4SrcWidth) >> 1);
        }
        u4H = u4SrcHeight;
        u4Y = u4OutYOffset + ((u4OutHeight - u4SrcHeight) >> 1);
#ifdef CC_SCPOS_3DTV_SUPPORT
		if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D) && IS_LVDS_DISP_3D_SHUTTER)
		{
			if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
				|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
				|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
				|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
				|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q))
			{
	
				u4W <<= 1;
				u4X = u4OutXOffset + ((u4OutWidth - (u4SrcWidth << 1)) >>1);
				//LOG(0, "u4OutWidth = %d, u4OutXOffset = %d, u4SrcWidth = %d", u4OutWidth, u4OutXOffset, u4SrcWidth);
			}
		    else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
            {
                u4H <<= 1;
                u4Y = u4OutYOffset + ((u4OutHeight - (u4SrcHeight <<1)) >> 1); 
			    LOG(3,"u4H = %d, u4Y = %d", u4H, u4Y);
            }
            else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
            {
                u4H <<= 1;
                u4Y = u4OutYOffset + ((u4OutHeight - (u4SrcHeight <<1)) >> 1);
            }
			else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
			{
			    u4W <<= 1;
				u4X = u4OutXOffset + ((u4OutWidth - (u4SrcWidth << 1)) >>1);
			    u4H <<= 1;
				u4Y = u4OutYOffset + ((u4OutHeight - (u4SrcHeight <<1)) >> 1); 
			}
		}
#endif

        LOG(3, "VDP(%d) 121 Display Region (%d, %d, %d, %d)\n", u4VdpId, u4X, u4Y, u4W, u4H);

        u4OutWidth = u4W;
        u4OutHeight = u4H;
        u4OutXOffset = u4X;
        u4OutYOffset = u4Y;

        // do not update current source & output while doing dynamic scaling
        if ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||(_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE))
        {
            // Set source region
            _arDynamicPrm[u4VdpId].u4CurSrcWidth = u4SrcWidth;
            _arDynamicPrm[u4VdpId].u4CurSrcHeight = u4SrcHeight;
            _arDynamicPrm[u4VdpId].u4CurSrcXOffset = u4SrcXOffset;
            _arDynamicPrm[u4VdpId].u4CurSrcYOffset = u4SrcYOffset;
        
            // Change display region
            _arDynamicPrm[u4VdpId].u4CurOutWidth = u4OutWidth;
            _arDynamicPrm[u4VdpId].u4CurOutHeight = u4OutHeight;
            _arDynamicPrm[u4VdpId].u4CurOutXOffset = u4OutXOffset;
            _arDynamicPrm[u4VdpId].u4CurOutYOffset = u4OutYOffset;
        }
    }


    if (_vScpipCalDynamicScale(u4VdpId,u4SrcWidth,u4SrcHeight, u4SrcXOffset, u4SrcYOffset,u4OutWidth, u4OutHeight, u4OutXOffset, u4OutYOffset))
    {
        u4SrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
        u4SrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
        u4SrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
        u4SrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

        u4OutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
        u4OutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight;
        u4OutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset;
        u4OutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;
    }

    // store original scaling parameters
    rDispPrm = *_pDispPrm[u4VdpId];

    u4Width = u4OutWidth;
    u4Height = u4OutHeight;
    u4TargetW = u4SrcWidth;
    u4TargetH = u4SrcHeight;
    //Calculate dram width and height limit.
    _vScpipCalDramLimit(u4VdpId,  u4SrcWidth, u4SrcHeight, u4OutWidth, u4OutHeight, &u4WidthLimitation, &u4HeightLimitation);

    if (DRVCUST_OptGet(eFbmMemSize) == FBM_MEM_CFG_MT5387_64MB)
    {
        if ((u4VdpId == VDP_1) && (u4ScalerMode == VDP_SCPOS_DISPMODE_OFF))
        {
            if ((_pPicInfo[u4VdpId]->wSrcW == 1920) && (bDrvVideoIsSrcInterlace(u4VdpId) == 0))
            {
                 _VDP_SetVspTune(0, 360);
            }
            else
            {
                 _VDP_SetVspTune(0, 0);
            }
        }
    }
    
    // consider H-prescale down
    // Force to specific pre-scaling down factors for debugging
    _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    
    if (_arScalePrm[u4VdpId].u4ForcedPreScalingactor)
    {
         _arScalePrm[u4VdpId].u4HpsFactor = _arScalePrm[u4VdpId].u4ForcedPreScalingactor;
         u4TargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
         LOG(1, "Force PreScale to %d\n", _arScalePrm[u4VdpId].u4ForcedPreScalingactor);
    }
#ifdef CC_SUPPORT_PRESCALE_DOWN
    else if (((_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_YPBPR) || (_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_DVI)) && bDrvVideoGetWidthOverSample(u4VdpId))
    {
        // 1440 >> 720
        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_HPD_FACTOR >> 1);
        u4TargetW = DIVIDE_WITH_CARRIER(u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor, DFT_HPD_FACTOR);
    }
#else
	#if SD_DEFINITION
		else if (((_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_YPBPR) || (_pChannel[u4VdpId]->bDecType == (UINT8) SV_VD_DVI)) && bDrvVideoGetWidthOverSample(u4VdpId))
		{		
			_arScalePrm[u4VdpId].u4HpsFactor = DIVIDE_WITH_CARRIER(DEFINITION_POINTS * DFT_HPD_FACTOR,wDrvVideoInputWidth(u4VdpId));
			u4TargetW = DIVIDE_WITH_CARRIER(u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor, DFT_HPD_FACTOR);
		}
	#endif
	
#endif
    else
    {
        UINT32 u4NewTargetW;
        u4NewTargetW = u4TargetW;

        if (u4Width >= u4TargetW)
        {
            // Up Scale, Compare Picture Size to FIFO Size

            if (_pPicInfo[u4VdpId]->wSrcW > u4MddiFifoMax)
            {
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMax)/ _pPicInfo[u4VdpId]->wSrcW;
                LOG(1, "Two Scaling! (HPSDOWN + UP) \n");

                // Calculate Source Width After PreScale Down
                u4NewTargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
        }
        else
        {
            // Down Scale
            UINT32 u4PictureSize;

            // Picture width After Down Scale
            u4PictureSize = DIVIDE_WITH_CARRIER((_pPicInfo[u4VdpId]->wSrcW * u4Width), u4TargetW);

            if (u4MddiFifoMax >= u4PictureSize)
            {
                // Do PreScale Down First       
                if ((u4PictureSize < u4MddiFifoMin) && (u4MddiFifoMin > 0))
                {                                
                    LOG(1, "Two Scaling! (DOWN + DOWN) MIN#0\n"); 

                    if (u4MddiFifoMin >= u4TargetW)
                    {
                    
                        LOG(1, "Warnning: Input width is already more than DI min FIFO limitation! (%d, %d)\n", u4TargetW, u4MddiFifoMin); 
                        _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    
                        u4NewTargetW = u4TargetW;
                    }
                    else
                    {
                        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMin)/ u4TargetW;
    
                        // Calculate Source Width After PreScale Down
                        u4NewTargetW = u4MddiFifoMin;
                    }

                }
                else
                {
                    _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4Width)/ u4TargetW;
                    // Calculate Source Width After PreScale Down
                    u4NewTargetW = u4Width;
                }
            }
            else
            {
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMax)/ _pPicInfo[u4VdpId]->wSrcW;
                LOG(1, "Two Scaling! (DOWN + DOWN) \n");

                // Calculate Source Width After PreScale Down
                u4NewTargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
        }

        if (_arScalePrm[u4VdpId].u4FixedPreScaler != 0)
        {
             // Optimized?
            UINT32 u4NewHpsFac;
            UINT32 u4HpsFactor = 0;
            if (u4VdpId == VDP_1)
            {
                u4HpsFactor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
            }
            else
            {
#if SUPPORT_POP
                u4HpsFactor = RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
#endif
            }

            u4NewHpsFac = _arScalePrm[u4VdpId].u4HpsFactor;            
            if (u4HpsFactor != u4NewHpsFac)            
            {
                if (_arScalePrm[u4VdpId].u4FirstConfig == 0)
                {

#if (!CC_SCPOS_NEVER_FIX_HPS)
                    UINT32 u4PictureSize;
                    UINT32 u4CanNotFixed=0;

                    if ((_arScalePrm[u4VdpId].u4HpsFactor >= (u4HpsFactor << 1)) ||
                        (_arScalePrm[u4VdpId].u4HpsFactor <= (u4HpsFactor >> 1)))
                    {
                        // 0.5X~2X
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Big rescale (0x%x >> 0x%x)\n", u4HpsFactor, _arScalePrm[u4VdpId].u4HpsFactor);
                    }

                    // Picture Size After Down Scale
                    u4PictureSize = DIVIDE_WITH_CARRIER((_pPicInfo[u4VdpId]->wSrcW * u4HpsFactor), DFT_DS_FACTOR);
                   
                    if (u4PictureSize  > u4MddiFifoMax)
                    {
                        // Bigger than MDDI FIFO
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Pic(%d) > MDDI_MAX(%d)\n", u4PictureSize, u4MddiFifoMax);
                    }
                    else if ((u4PictureSize < u4MddiFifoMin) && (u4MddiFifoMin > 0) && (u4MddiFifoMin < u4SrcWidth))
                    {
                        // smaller than MDDI FIFO min bound
                        u4CanNotFixed = 1;

                        LOG(1, "Fixed PreScaler Fail! Pic(%d) < MDDI_MIN(%d)\n", u4PictureSize, u4MddiFifoMin);
                    }

                    if (u4CanNotFixed == 0)
                    {
                        LOG(1, "PreScaler Not Optimized (0x%x >> 0x%x)\n", _arScalePrm[u4VdpId].u4HpsFactor, u4HpsFactor);
                                            
                        _arScalePrm[u4VdpId].u4HpsFactor  = u4HpsFactor;
                        _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 1;

                        if (_arScalePrm[u4VdpId].u4HpsFactor  != DFT_DS_FACTOR)
                        {
                            // Calculate Source Width After Old PreScale Down
                            u4TargetW = DIVIDE_WITH_CARRIER((u4TargetW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
                        }                                
                    }
                    else //CanNotFixed==1
#endif//(!CC_SCPOS_NEVER_FIX_HPS)
                    {
                        // Force rescale due to some reason
                        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                            (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),
                                            TRUE);
                        LOG(6, "MUTE_MODULE_SCPOS_PRE_DOWN (1)\n");                
                            _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
                    
                        u4TargetW = u4NewTargetW;
                    }
                }
                else
                {            
                    LOG(3, "Do 1st Config\n");

                    _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                      (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),
                                      TRUE);
                    LOG(6, "MUTE_MODULE_SCPOS_PRE_DOWN (2)\n");

                    _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
                    _arScalePrm[u4VdpId].u4FirstConfig = 0;
                
                    u4TargetW = u4NewTargetW;
                }
                
                //if Pre-scaling down factor changed, fire scaler mode change to enable scaler to recalculate parameter of pscan dispmode
                if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE)
                {
                    LOG(4,"HPS changed ==> fireSCPOSModeChange\n");
                    fireSCPOSModeChange(u4VdpId);
                }                
            }
            else
            {
                _arScalePrm[u4VdpId].u4PreScalerNotOptimized = 0;
            
                u4TargetW = u4NewTargetW;
            }
        }
        else
        {
            u4TargetW = u4NewTargetW;
        }
    }
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
    {
        u4TargetW = u4SrcWidth;
        _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    }    
    #endif    

    _vScpipSetHPreScalingDownFactor(u4VdpId);
    
    u4FifoLength = (_u4SCPOS_FIFO_LENGTH[u4VdpId][(u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP))]);
    
    if (u4FifoLength > u4WidthLimitation)
    {
         u4FifoLength = u4WidthLimitation;
    }

    /* Horizontal Direction */
    // scaler only handle even width because of h boundary
    if((u4TargetW % 2) && !u1ScpipGet444Mode(u4VdpId))
    {
        u4TargetW -= 1;
    }
    #if SUPPORT_SBS_CONVERT
    if(_bSBSOutput)
    {
        u4Width = PANEL_GetPanelWidth()/2;
        if(u4VdpId == VDP_2)
        {
            u4FifoLength = 960;
            u4HeightLimitation = 1080;
        }
    }
    #endif
    
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
       || ((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        if((BSP_GetIcVersion() >= IC_VER_5365_AB) && IS_IC_5365())
        {
            u4FifoLength = 960;
        }
        else
        {
            u4FifoLength = 720;
        }

        /* 720P TNB WINC_1 is not enough */
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR) && (720 == wDrvVideoInputHeight(VDP_1)))
        {
            u4FifoLength = PANEL_GetPanelWidth()/2;
        }      
        u4HeightLimitation = 1080;   
    }
    #endif

    #ifdef CC_SCPOS_3DTV_SUPPORT
    if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)
        && fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
        && (IS_LVDS_DISP_3D_POLARIZED))
    {
        UINT32 u4HalfSrcHeight = u4SrcHeight >> 1;
        if(u4HeightLimitation > u4HalfSrcHeight)
        {
            u4HeightLimitation = u4HalfSrcHeight;
        }            
    }
    #endif
    
    _vScpipCalScaleFactor(u4VdpId, u4Width, u4TargetW, u4FifoLength, CAL_HDIRECT_FACTOR);

    /* Vertical Direction */
    _vScpipCalScaleFactor(u4VdpId, u4Height , u4TargetH , u4HeightLimitation, CAL_VDIRECT_FACTOR);    

    _arScalePrm[u4VdpId].u4NonlinearCtrl = 0;

#ifdef CC_SCPOS_NO_DS_NONLINEAR
    if (_arScalePrm[u4VdpId].u4NonlinearEn != 0)
#else
    if ((_arScalePrm[u4VdpId].u4NonlinearEn != 0) &&
        ((_arScalePrm[u4VdpId].u4DynamicScale == 0) ||
        (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_IDLE)))
#endif        
    {
        if ((u4VdpId == VDP_1) &&
            (u4Width != 0) &&
            (u4TargetW != 0))
        {
            if (_arScalePrm[u4VdpId].u4NonlinearAuto) 
            {
                vScpipCalNonLinear(u4VdpId, u4TargetW, (u4Width + u4WidthPlus)); // For CC_SCPOS_WA1
            }
            else if(_arScalePrm[u4VdpId].u4NewNonlinear)
            {
                vScpipCalNewNonlinear(u4VdpId, u4TargetW, (u4Width + u4WidthPlus));
            }
            else // not auto and not new nonlinear
            {
                _arScalePrm[u4VdpId].u4NonlinearCtrl = 1;
            }
        }
    }
          
    // store original scaling parameters
    if ((rDispPrm.u4DSclH != _pDispPrm[u4VdpId]->u4DSclH) || (rDispPrm.u4DSclV != _pDispPrm[u4VdpId]->u4DSclV) ||
        // [DTV00112610] Flash a bright line when change "ASPECT" at 1152*864@85Hz.
        // Up scaling factor changed => Need mute, Peggy20080110
        (rDispPrm.u4USclH != _pDispPrm[u4VdpId]->u4USclH) || (rDispPrm.u4USclV != _pDispPrm[u4VdpId]->u4USclV))
    {
        LOG(3, "Scaler factor changed!: HU(0x%x) VU(0x%x) HD(0x%x) VD(0x%x)\n",
            rDispPrm.u4USclH, rDispPrm.u4USclV,
            rDispPrm.u4DSclH, rDispPrm.u4DSclV);
        _au4ScalingFactorChanged[u4VdpId] = 1;        
        
        //if scaling factor changed, inform scaler to re-enter scaler dispmode or pscan dispmode
        if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE ||
           u4ScalerMode==VDP_SCPOS_DISPMODE_AUTO||
           u4ScalerMode==VDP_SCPOS_DISPMODE_MANUAL)
        {
            LOG(4,"Scaling factor changed ==>fireSCPOSModeChange()\n");
            fireSCPOSModeChange(u4VdpId);
        }        
    }
    else
    {
        _au4ScalingFactorChanged[u4VdpId] = 0;            
    }
    
    if (_pChannel[u4VdpId]->bIsChannelOn != SV_OFF)
    {
        LOG(3, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
            u4VdpId,  
            u4OutXOffset,
            u4OutYOffset,
            u4OutWidth,
            u4OutHeight,
            u4SrcXOffset,
            u4SrcYOffset,
            u4SrcWidth,
            u4SrcHeight);

        LOG(3, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n",
            _pDispPrm[u4VdpId]->u4USclH, _pDispPrm[u4VdpId]->u4USclV,
            _arScalePrm[u4VdpId].u4HpsFactor, 
            _pDispPrm[u4VdpId]->u4DSclH, _pDispPrm[u4VdpId]->u4DSclV);
    }
}


/**
* @brief Calculate up/down scale factor
* @param u4VdpId VDP_1/VDP_2
* @param u4Source input width/height
* @param u4Target output width/height
* @param u4LimitLength fifo limit length
* @param bDirection horizontal/vertical direcrion
*/
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 u4Source, UINT32 u4Target, UINT32 u4LimitLength, UINT8 bDirection) 
{
    if ((u4Source == 0) || (u4Target == 0) || (_pChannel[u4VdpId]->bIsChannelOn == SV_OFF)
        #ifndef CC_SCPOS_3DTV_SUPPORT
        || ((u4Target <=u4LimitLength) && (u4Source == u4Target)) // mark for Sensio 3d WA
        #endif
        ) 
    {
        // no scaling
        if (bDirection == CAL_HDIRECT_FACTOR) // horizontal
        {
            _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
        }
        else // vertical
        {
            _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
            _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
        }
    }
    else if ((u4Source < u4Target) && (u4Source <= u4LimitLength))
    {
        // down scaling
        UINT32 u4Factor;
        
        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
        }
        else
        {
            _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
        }
        
        u4Factor = (u4Source << DFT_DS_BIT_NUM)/ u4Target;

        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            _pDispPrm[u4VdpId]->u4DSclH =u4Factor;
            if (_pDispPrm[u4VdpId]->u4DSclH > DFT_DS_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
            }
        }
        else
        {
            _pDispPrm[u4VdpId]->u4DSclV = u4Factor;
            if (_pDispPrm[u4VdpId]->u4DSclV > DFT_DS_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
            }
        }
    }
    else   // up scaling or down+up scaling case
    {
         UINT32 u4Factor;
         UINT32 u4OutPutLength;

         if (u4Target > u4LimitLength)
         {
            u4Factor =(u4LimitLength<< DFT_DS_BIT_NUM)/ u4Target;

            if (bDirection == CAL_HDIRECT_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4DSclH = u4Factor;
                if (_pDispPrm[u4VdpId]->u4DSclH > DFT_DS_FACTOR)
                {
                    _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
                }
            }
            else
            {
                _pDispPrm[u4VdpId]->u4DSclV = u4Factor;
                if (_pDispPrm[u4VdpId]->u4DSclV > DFT_DS_FACTOR)
                {
                    _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
                }
            }

            u4OutPutLength = u4LimitLength;
            LOG(3, "SCPOS H-Scaling DOWN+UP: %d -> %d -> %d\n", u4Target, u4LimitLength, u4Source); 
         }
         else
         {
             if (bDirection == CAL_HDIRECT_FACTOR)
             {
                 _pDispPrm[u4VdpId]->u4DSclH = DFT_DS_FACTOR;
             }
             else
             {
                 _pDispPrm[u4VdpId]->u4DSclV = DFT_DS_FACTOR;
             }
             u4OutPutLength= u4Target;
         }

         // up scaling
         if (bDirection == CAL_HDIRECT_FACTOR)
         {        
            // enlarge H outptu length according to up-scaling ratio to prevent right most line thick
            if(u4OutPutLength!=0)
            {
                u4Source += ((u4Source/u4OutPutLength)-1);
            }
         }
         
         #ifdef CC_SCPOS_3DTV_SUPPORT
         // Sensio vertical down scaler phase work around will cause the last line in DRAM 
         // to have a darker color. Vertical up scale more to make it go outside of the screen.
         if ((bDirection == CAL_VDIRECT_FACTOR)
                 && fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)
                 && (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
                     || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
                     || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                     || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
            )
         {
             if(!(IS_LVDS_DISP_3D_POLARIZED && fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)))
             {
                 u4OutPutLength -= 1; 
             }
         }
         #endif

         u4Factor =(u4OutPutLength<< DFT_DS_BIT_NUM)/u4Source;

         if (bDirection == CAL_HDIRECT_FACTOR)
         {
            _pDispPrm[u4VdpId]->u4USclH = u4Factor;
            if (_pDispPrm[u4VdpId]->u4USclH > DFT_US_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            }
         }
         else
         {
             _pDispPrm[u4VdpId]->u4USclV = u4Factor;
             if (_pDispPrm[u4VdpId]->u4USclV > DFT_US_FACTOR)
             {
                 _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR;
             }
         }
    }
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))
    {        
        if(bDirection == CAL_HDIRECT_FACTOR)
        {
            _pDispPrm[u4VdpId]->u4USclH = (DFT_US_FACTOR * u4Target) / PANEL_GetPanelWidth();
            
            if (_pDispPrm[u4VdpId]->u4USclH > DFT_US_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            }            
        }
        if(bDirection == CAL_VDIRECT_FACTOR)
        {
           _pDispPrm[u4VdpId]->u4USclV = DFT_US_FACTOR; 
        }
    }
    else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR))
    {
        if(bDirection == CAL_HDIRECT_FACTOR)
        {          
            _pDispPrm[u4VdpId]->u4USclH = (DFT_US_FACTOR * u4Target) / PANEL_GetPanelWidth();

            if (_pDispPrm[u4VdpId]->u4USclH > DFT_US_FACTOR)
            {
                _pDispPrm[u4VdpId]->u4USclH = DFT_US_FACTOR;
            }              
        }     
    }
    #endif      
}

static UINT32 _u4ScpipCalReadInitP(UINT32 u4Width, UINT8 b10bit, UINT8 b444mode)
{
    UINT32 u4Pack;
    UINT32 u4InitP=0;       // default is 0

    if(b444mode==SV_FALSE)  // 422
    {
        if(u4Width%2)       // special case input width is odd number 
        {
            u4Width+=1;
        }
        u4Pack = (b10bit)?6:8;
    }
    else                    // 444 8bit mode, 444 10bit not support
    {
        u4Pack =5;
    }
    if(u4Width%u4Pack!=0)
    {
        u4InitP = u4Pack -(u4Width%u4Pack);
    }
    return u4InitP;
}

/**
* @brief Update display paramters
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipUpdateDispPrm(UINT32 u4VdpId) 
{
    UINT32 u4TargetW;
    UINT32 u4XOffset;

    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4SrcXOffset;
    UINT32 u4SrcYOffset;

    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4OutXOffset;
    UINT32 u4OutYOffset;
    RDispPrm rDispPrm;
    UINT8 bPath;
    UINT8 bMode;
    UINT32 u4WidthDS; // source width after down-scaling
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    bPath = getScalerPath(u4VdpId);
    bMode = u1ScpipGetDispMode(bPath);  
    rDispPrm = *_pDispPrm[u4VdpId];

    if (((_arScalePrm[u4VdpId].u4DynamicScale != 0) &&
        (_arDynamicPrm[u4VdpId].u4State == VDP_DYNAMICE_SCALE_SCALING)) ||
        (_arScalePrm[u4VdpId].u4121MapEn != 0))        
    {
        u4SrcWidth = _arDynamicPrm[u4VdpId].u4CurSrcWidth;
        u4SrcHeight = _arDynamicPrm[u4VdpId].u4CurSrcHeight;
        u4SrcXOffset = _arDynamicPrm[u4VdpId].u4CurSrcXOffset;
        u4SrcYOffset = _arDynamicPrm[u4VdpId].u4CurSrcYOffset;

        u4OutWidth = _arDynamicPrm[u4VdpId].u4CurOutWidth;
        u4OutHeight = _arDynamicPrm[u4VdpId].u4CurOutHeight;
        u4OutXOffset = _arDynamicPrm[u4VdpId].u4CurOutXOffset;
        u4OutYOffset = _arDynamicPrm[u4VdpId].u4CurOutYOffset;
    }
    else
    {
        u4SrcWidth = _pPicInfo[u4VdpId]->wTargetW;
        u4SrcHeight = _pPicInfo[u4VdpId]->wTargetH;
        u4SrcXOffset = getMixedHPorch(u4VdpId);
        u4SrcYOffset = getMixedVPorch(u4VdpId);
        u4OutWidth = _pDispInfo[u4VdpId]->wRWidth;
        u4OutHeight = _pDispInfo[u4VdpId]->wRHeight;
        u4OutXOffset = _pDispInfo[u4VdpId]->wRXOffset;
        u4OutYOffset = _pDispInfo[u4VdpId]->wRYOffset;
#ifdef IS_SUPPORT_3D_PHOTO	
				if(IsFixRegion && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF))
				{
					if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
						||fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
					{
						u4OutWidth = _rM3DPhotoDispInfo.wRWidth;
						u4OutHeight = _rM3DPhotoDispInfo.wRHeight;
						u4OutXOffset = _rM3DPhotoDispInfo.wRXOffset;
						u4OutYOffset = _rM3DPhotoDispInfo.wRYOffset;
						u4SrcXOffset = u4SrcXOffset << 1;
					}
					else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
						|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
					{
						//u4SrcYOffset <<= 1;
						//u4SrcHeight <<= 1;	   
						//todo: TNB
						u4OutWidth = _rM3DPhotoDispInfo.wRWidth;
						u4OutHeight = _rM3DPhotoDispInfo.wRHeight;
						u4OutXOffset = _rM3DPhotoDispInfo.wRXOffset;
						u4OutYOffset = _rM3DPhotoDispInfo.wRYOffset;
					}
					//LOG(2, "\n********u4OutXOffset %d, u4SrcXOffset %d*******\n", u4OutXOffset, u4SrcXOffset);
				}
				else
				{
					//LOG(2, "\n*********back to _pDispInfo*******\n");
				}
#endif

    }
#ifdef CC_SCPOS_3DTV_SUPPORT
    //if(u4VdpId == VDP_1)
    {
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))
        {
            if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q))
            {
                //_pDispPrm[u4VdpId]->u4Tv3dWidth = _pPicInfo[u4VdpId]->wSrcW >> 1;
                u4SrcWidth >>= 1;
                u4SrcXOffset >>= 1; // side by side video width will be cut half by scaler
                                    // so we need to compute HL/datalen by 1/2 input width
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
            {
                //_pDispPrm[u4VdpId]->u4Tv3dWidth = _pPicInfo[u4VdpId]->wSrcW >> 1;
                u4SrcWidth >>= 1;
                u4SrcXOffset >>= 1; // side by side video width will be cut half by scaler
                                    // so we need to compute HL/datalen by 1/2 input width                
                u4SrcYOffset >>= 1;                                    
                u4SrcHeight >>= 1; // Tnb split in NR so the src height will be 1/2
            }
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
            {
                u4SrcYOffset >>= 1;
                u4SrcHeight >>= 1;                
            }     
            else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR))
            {
                //_pDispPrm[u4VdpId]->u4Tv3dWidth = _pPicInfo[u4VdpId]->wSrcW + 2;
                
                if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR))
                {
                    u4SrcYOffset >>= 1;
                    u4SrcHeight >>= 1;
                    //u4OutHeight >>= 1;
                }
            }
            else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
            {
                u4SrcHeight >>= 1;  // interleave will be split by scaler, we compute V scaling factor by 1/2 input height
            }   

            if((u4VdpId == VDP_1) && (IS_LVDS_DISP_3D_POLARIZED)
                && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
                && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)
                && !fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB))
            {                
                u4OutYOffset >>= 1;
                u4OutHeight >>= 1;                                
            }
        }
    }
#endif   

    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(SV_FALSE == _bLRInverse) /*no inverse*/
    {
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR))
        {
            if(VDP_1 == u4VdpId)    // VDP_1 NOT CLIP SRC REGION
            {
                u4SrcHeight = DIVIDE_WITH_ROUND(4898 * u4SrcHeight, 10000);   
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))          
        {
            if(VDP_1 == u4VdpId)    // VDP_1 NOT CLIP SRC REGION
            {
                u4SrcHeight >>= 1;
            }
            if(720 == wDrvVideoInputHeight(VDP_1))
            {
                u4SrcHeight <<= 1;
                /*the last line in top will be wrong because of DI up, so kick it out*/
                u4SrcHeight -= 2;
                if(VDP_2 == u4VdpId)
                {
                    u4SrcYOffset <<= 1;
                }
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            u4SrcWidth -= (_bTTDOverscan * 2);
            if(VDP_1 == u4VdpId)
            {
                u4SrcXOffset = _bTTDOverscan + _bTTDDepth;
            }
            else
            {
                u4SrcXOffset = _bTTDOverscan - _bTTDDepth;
            }
            
        }
    }
    else    /* inverse */
    {
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR))
        {
            if(VDP_1 == u4VdpId)    // VDP_1 NOT CLIP SRC REGION
            {
                u4SrcYOffset = DIVIDE_WITH_ROUND(5102 * u4SrcHeight, 10000); 
                u4SrcHeight = DIVIDE_WITH_ROUND(4898 * u4SrcHeight, 10000);                 
            }
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
        {          
            u4SrcWidth >>= 1;
            //u4SrcXOffset = u4SrcWidth;       
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))          
        {
            if(VDP_1 == u4VdpId)    // VDP_1 NOT CLIP SRC REGION
            {
                u4SrcHeight >>= 1;
                u4SrcYOffset = u4SrcHeight;
            }
            if(720 == wDrvVideoInputHeight(VDP_1))
            {
                u4SrcHeight <<= 1;
                if(VDP_1 == u4VdpId)
                {
                    u4SrcYOffset = u4SrcHeight;                  
                }
                /*the last line in top will be wrong because of DI up, so kick it out*/
                u4SrcHeight -= 2;
            }
        }     
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
        {
            u4SrcWidth -= (_bTTDOverscan * 2);
            if(VDP_1 == u4VdpId)
            {
                u4SrcXOffset = _bTTDOverscan - _bTTDDepth;
            }
            else
            {
                u4SrcXOffset = _bTTDOverscan + _bTTDDepth;
            }           
        }
    }
    #endif

    // move _VDP_vPreAdjustWidth to here
#ifdef SUPPORT_SPECIAL_POP
    if (!_fgSpecialPOP)
#endif
    {
        _pDispInfo[u4VdpId]->wXOffset = 0;
        _pDispInfo[u4VdpId]->wYOffset = 0;
    }
#ifdef CC_SCPOS_3DTV_SUPPORT
			if((_arScalePrm[u4VdpId].u4121MapEn)
				&&fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)
				&& IS_LVDS_DISP_3D_SHUTTER)
				{
					if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SPLIT)
							|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA)
							|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_TNB)
							||fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
							|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)
							|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
						{
							u4SrcHeight <<= 1;	
							LOG(3, "u4SrcHeight back to normal in updatedispprm, u4SrcHeight = %d\n", u4SrcHeight);
						}
				}
	#endif

    //1. consider H-prescale down
    u4TargetW = DIVIDE_WITH_CARRIER(u4SrcWidth * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR);
    u4XOffset = (u4SrcXOffset * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_HPD_FACTOR;
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))
    {
        if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
        {
            _pDispPrm[u4VdpId]->u4Tv3dWidth = 
                DIVIDE_WITH_CARRIER(wDrvVideoInputWidth(bPath) * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR) >> 1;
        }
        else if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FHD_PR))
        {
            _pDispPrm[u4VdpId]->u4Tv3dWidth = 
                DIVIDE_WITH_CARRIER(wDrvVideoInputWidth(bPath) * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR) + 2;
        }
        else if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INTERLEAVE_SPLIT))
        {
            _pDispPrm[u4VdpId]->u4Tv3dWidth = 
                DIVIDE_WITH_CARRIER(wDrvVideoInputWidth(bPath) * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId),  DFT_HPD_FACTOR) + 2;            
        }

        if (((_pDispPrm[u4VdpId]->u4Tv3dWidth % 2) != 0) && !u1ScpipGet444Mode(u4VdpId))
        {
            _pDispPrm[u4VdpId]->u4Tv3dWidth -= 1; // TV3D_PORCH cannot set to odd number
        }
    }
#endif    
    /* Horizontal Direction */

    //2. update H Porch
    // Horizontal display offset on the left side 
    // value 1 represent 1 pixels in 5360. 
    // It is used to compensate video front porch.
    #if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE && u4XOffset > SCPIP_HCLIP_LENGTH)
    {           
        _pDispPrm[u4VdpId]->wHPosOfst = SCPIP_HCLIP_LENGTH;
    }
	else
	#elif !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {   
        //when scaler is in pscan dispmode, overscan was done in PSCAN
        _pDispPrm[u4VdpId]->wHPosOfst = _u4ScpipGetPscanDispmodeHPorch(bPath);
    }
    else
    #endif
    {
        #if 0
        if ((u4XOffset%2)&& !u1ScpipGet444Mode(u4VdpId)) // if hporch is odd, force it to add 1 to be even
        {
            u4XOffset = u4XOffset + 1;  
        }
        #endif
        _pDispPrm[u4VdpId]->wHPosOfst = u4XOffset;
    }

    /* 3. update HP and HL and initial point
      422 mode: 16-bit Y/C mode  : value 1 represent 8 pixels.
                20-bit Y/C mode  : value 1 represent 6 pixels.
      444 mode: 24-bit R/G/B mode: value 1 represent 16/3 pixels.
    */
    u4WidthDS = RSHIFT_WITH_CARRIER((u4TargetW * _pDispPrm[u4VdpId]->u4DSclH), DFT_DS_BIT_NUM);
#ifdef CC_SCPOS_3DTV_SUPPORT
    if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D)&&_arScalePrm[u4VdpId].u4121MapEn)
	{
	    if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
			|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBSH_SPLIT)
			|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT)
			|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS)
			|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
			|| fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
            || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
			{	
    	            u4WidthDS <<=1;  	            
	    	}
    }
#endif
    
     // scaler only handle even width because of h boundary
    if((u4WidthDS % 2) && !u1ScpipGet444Mode(u4VdpId))
    {
        u4WidthDS -= 1;
    }

    _pDispPrm[u4VdpId]->u1Mirror422Reg = 0;
    
    if(u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE)     // 422 Mode
    {
        _pDispPrm[u4VdpId]->u4HBound = u4WidthDS+(u4WidthDS%2); // H bound need to be even
        #if SUPPORT_SBS_CONVERT
        if(_bSBSOutput && (u4VdpId == VDP_1))
        {
            _pDispPrm[u4VdpId]->u4HBound = PANEL_GetPanelWidth();
        }
        #endif
        if(bScpipMirrorEnable[u4VdpId])
        {
            _pDispPrm[u4VdpId]->u4ReadInitP = _u4ScpipCalReadInitP(u4WidthDS, u4ScpipGet10BitMode(u4VdpId), SV_FALSE);
            if(u4WidthDS%2)    // odd
            {
                _pDispPrm[u4VdpId]->u1Mirror422Reg = 3;
            }
            else               //even
            {
                _pDispPrm[u4VdpId]->u1Mirror422Reg = 2;
            }
        }

        if (u4ScpipGet10BitMode(u4VdpId))
        {       // 10 bit mode
            _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset / 6;         //  ignore rounding
            _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 6);
        }
        else  // 8 bit mode
        {          
            _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset >> 3;         //  ignore rounding
            if(bMode== VDP_SCPOS_DISPMODE_OFF)
            {//128 bits pack
              _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 3);
            }
            else
            {// 120 bits pack
                _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 7);
                if((!(u4WidthDS%7)) && (u4WidthDS%2))//when width can be mod by 7 and is odd, the color of last line will be wrong
                {
                    _pDispPrm[u4VdpId]->wHLen += 1;
                }
            }
        }
    }
    else                                            // 444 Mode
    {
        _pDispPrm[u4VdpId]->u4HBound = u4WidthDS;
         
        if(bScpipMirrorEnable[u4VdpId])
        {
            _pDispPrm[u4VdpId]->u4ReadInitP = _u4ScpipCalReadInitP(u4WidthDS, u4ScpipGet10BitMode(u4VdpId), SV_TRUE);
        }

        if (u4ScpipGet10BitMode(u4VdpId))
        {       // 10 bit mode
              _pDispPrm[u4VdpId]->wHPos = _pDispInfo[u4VdpId]->wXOffset >>2;         //  ignore rounding
              _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 2);
        }
        else
        {
            _pDispPrm[u4VdpId]->wHPos = (_pDispInfo[u4VdpId]->wXOffset * 3) >> 4;   //  ignore rounding
            
            if((bMode == VDP_SCPOS_DISPMODE_OFF)&&(bScpipMirrorEnable[u4VdpId]==SV_FALSE))  
            {   //128 bits pack
                _pDispPrm[u4VdpId]->wHLen = RSHIFT_WITH_CARRIER((u4WidthDS * 3), 4);
            }
            else
            {   // 120 bits pack
                  _pDispPrm[u4VdpId]->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 5);
            }
        }
    }   
    
#ifdef SUPPORT_SPECIAL_POP
    if (_fgSpecialPOP)
    {
        if (u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE) // 422 Mode: 16-bit
        {
            if (u4ScpipGet10BitMode(u4VdpId))
            {       // 10 bit mode
                _pDispPrm[u4VdpId]->wDataLen = DIVIDE_WITH_CARRIER(u4OutWidth, 6);
            }
            else
            {       //8 bit mode
                _pDispPrm[u4VdpId]->wDataLen = RSHIFT_WITH_CARRIER(u4OutWidth, 3);
            }
        }
        else       // 444 Mode
        {
            _pDispPrm[u4VdpId]->wDataLen = RSHIFT_WITH_CARRIER((u4OutWidth * 3), 4);
        }

        LOG(3, "Special POP (%d, %d, %d, %d), R(%d, %d, %d, %d), DataLen (%d)\n", 
            _pDispInfo[u4VdpId]->wXOffset,
            _pDispInfo[u4VdpId]->wYOffset,
            u4OutWidth,
            u4OutHeight,
            u4OutXOffset,
            u4OutYOffset,
            u4OutWidth,
            u4OutHeight,
            _pDispPrm[u4VdpId]->wDataLen);
    }
    else
#endif
    {
        if((bMode == VDP_SCPOS_DISPMODE_OFF) && 
            RegReadFldAlign(SCPIP_PIP1_05+getPIPOffset(bPath), PIP1_05_DRAM_BURST_MODE_1)) // dram burst mode
        {
            _pDispPrm[u4VdpId]->wDataLen = ((_pDispPrm[u4VdpId]->wHLen + 3) / 4) * 4; // 4 pack 1 burst read
            _pDispPrm[u4VdpId]->u1HPaddingLen = _pDispPrm[u4VdpId]->wDataLen - _pDispPrm[u4VdpId]->wHLen;
            //_pDispPrm[u4VdpId]->wHLen = _pDispPrm[u4VdpId]->wDataLen;
        }        
        else
        {
            _pDispPrm[u4VdpId]->wDataLen = _pDispPrm[u4VdpId]->wHLen;
        }

        // DATA_LEN & HL both add 1 for pscan display mode mirror
        if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            _pDispPrm[u4VdpId]->wHLen += 1;
            _pDispPrm[u4VdpId]->wDataLen += 1;
        }
    }
    #if SUPPORT_SBS_CONVERT
    if(_bSBSOutput)
    {
        _pDispPrm[u4VdpId]->wDataLen = _pDispPrm[u4VdpId]->wDataLen *2;
    }
    #endif
    
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
       || ((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        _pDispPrm[u4VdpId]->wDataLen = _pDispPrm[u4VdpId]->wDataLen *2;
    }
    #endif
    
    // [DTV00111581] mute when dataLen changed and frame number is smaller than 3, Peggy20071226
    if ((rDispPrm.wDataLen!= _pDispPrm[u4VdpId]->wDataLen))
    {
        #ifdef CC_SCPOS_WA25
        if (bDrvVideoGetRefreshRate(bPath) > vDrvGetLCDFreq())
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_WA, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_IMPORT_PROTECT, TRUE);
            LOG(6, "MUTE_MODULE_SCPOS_WA :CC_SCPOS_WA25: Set Mute , VDP(%d)\n", u4VdpId);
        }
        #endif
    }

    /* 4. update RHP and RHL. Value 1 represent 1 pixel. */
    _pDispPrm[u4VdpId]->wRHPos = u4OutXOffset;
    _pDispPrm[u4VdpId]->wRHLen = u4OutWidth;

    //=================================================================
    /* Vertical Direction */

    //5. update V Porch
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
            //when scaler is in pscan dispmode, overscan was done in PSCAN    
            _pDispPrm[u4VdpId]->wVPosOfst = 0;
    }
    else
    {
        if(_pDispPrm[u4VdpId]->wVPosOfst !=u4SrcYOffset + _pPicInfo[u4VdpId]->wVPorch)
        {
            u4ForceFrameTrackCtrl |= SCPIP_FRAME_TRACK_OFF_V_PORCH;
            vScpipSwitchFrameTrackOnOff(SV_OFF); 
            //Printf("vScpipSwitchFrameTrackOnOff: SCPIP_FRAME_TRACK_OFF_V_PORCH\n\n");
            _pDispPrm[u4VdpId]->wVPosOfst = u4SrcYOffset + _pPicInfo[u4VdpId]->wVPorch;
        }
    }
    
    /*6. update VP. Value 1 represent 1 line */
    _pDispPrm[u4VdpId]->wVPos = _pDispInfo[u4VdpId]->wYOffset;

    //7. update VL. Value 1 represent 1 line
    _pDispPrm[u4VdpId]->wVLen = RSHIFT_WITH_CARRIER(((UINT32)(u4SrcHeight) * _pDispPrm[u4VdpId]->u4DSclV), DFT_DS_BIT_NUM);

    // 9. update V bound
    _pDispPrm[u4VdpId]->u4VBound = _pDispPrm[u4VdpId]->wVLen;
    
    #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
    {
        _pDispPrm[u4VdpId]->u4VBound = _pDispPrm[u4VdpId]->wVLen * 2;
    }
    #endif
    
    #ifdef CC_SCPOS_3DTV_SUPPORT
    //if(u4VdpId == VDP_1)
    {
        if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D))
        {
            if (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_LINE_INTERLEAVE)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_FRAME_INTERLEAVE)
                || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_TNB_SW_WA))
            {
                _pDispPrm[u4VdpId]->wVLen <<= 1; // 3d special case VL * 2   
            }
        }
    }
    #endif
    
    _pDispPrm[u4VdpId]->wRVPos = u4OutYOffset;
    if(_u4ForceDispOff[u4VdpId])// for TVE use, not to turn on sub window.
    {
        _pDispPrm[u4VdpId]->wRVLen =0;
    }
    else
    {
        _pDispPrm[u4VdpId]->wRVLen = u4OutHeight;
    }
    

    // 10. up scaler initial phase
    if(u4VdpId == VDP_1) // only main path has this feature
    {   
        if(!_fgIsHNegInitPhaseOn)
        {
            _pDispPrm[u4VdpId]->u4USclHInit = 0;
            _pDispPrm[u4VdpId]->u1USclHInitSigned = 0;  
        }
        else
        {
            _pDispPrm[u4VdpId]->u4USclHInit = _u4ScpipCalNegInitPhase(u4WidthDS, u4OutWidth, _pDispPrm[u4VdpId]->u4USclH);
            _pDispPrm[u4VdpId]->u1USclHInitSigned = (_pDispPrm[u4VdpId]->u4USclHInit)?1:0;
        }
        
        if(!_fgIsVNegInitPhaseOn||(bMode == VDP_SCPOS_DISPMODE_AUTO)||(bMode == VDP_SCPOS_DISPMODE_MANUAL)) // turn off v negative initial phase in scaler display mode
        {
            _pDispPrm[u4VdpId]->u4USclVInit = 0;
            _pDispPrm[u4VdpId]->u1USclVInitSigned = 0;
        }
        else
        {
            _pDispPrm[u4VdpId]->u4USclVInit = _u4ScpipCalNegInitPhase(_pDispPrm[u4VdpId]->wVLen, u4OutHeight, _pDispPrm[u4VdpId]->u4USclV);
            _pDispPrm[u4VdpId]->u1USclVInitSigned = (_pDispPrm[u4VdpId]->u4USclVInit)?1:0;
        }
    }

    /* For debug */
    //_arScalePrm[u4VdpId].u4Width = RSHIFT_WITH_CARRIER((u4TargetW * _pDispPrm[u4VdpId]->u4DSclH), (DFT_DS_BIT_NUM + 3)) << 3;
    //_arScalePrm[u4VdpId].u4Height = _pDispPrm[u4VdpId]->wVLen;
}


/**
* @brief Calculate output vtotal
* @return Vtotal
*/
UINT16 u2ScpipCalculateOutputVTotal(void) 
{
    UINT16 u2VOutputTotal;
    UINT32 u4InVTotal;

    //get input VTotal
    u4InVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);    
    u2VOutputTotal = u4InVTotal;
    
    if(_pPicInfo[VDP_1]->wTargetH != 0)
    {
        u2VOutputTotal = DIVIDE_WITH_CARRIER(u4InVTotal * wDISPLAY_HEIGHT , _pPicInfo[VDP_1]->wTargetH);
    }
    return u2VOutputTotal;
}

// TODO: FIXME: should this function be global??
/**
* @brief Switch to dispmode
* @param u4OnOff On/Off
*/
void vSwitchDispMode(UINT32 u4OnOff) 
{
    UINT8 bScalerMode;
    bScalerMode = u1ScpipGetDispMode(SV_VP_MAIN);

    if (u4OnOff == SV_ON)
    {
        vScpipWriteCtrlOnOff(VDP_1,SV_OFF);

        /* Enable Display Mode */
        //vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_F_SYNC_1);
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_FMODESEL_1);
    }
    else
    {
        if (_arScalePrm[VDP_1].u4Enable)
        {
            CRIT_STATE_T csState = x_crit_start(); 
            // avoid other thread turn on write control before all related registers are set.          
            _u4ResetScposDramBuffer[VDP_1] = (_u4ResetScposDramBuffer[VDP_1] < 2)? 2: _u4ResetScposDramBuffer[VDP_1];
            x_crit_end(csState);
        }    
    
        /* Disable Display Mode */
        //vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
        /* Disable Display Mode */
        
        if (bScalerMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            //If current mode is pscan dispmode, to prevent FMODESEL_1 from keeping toggling between 0 and 1.
            //Just keep it as original value.
            //vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_FMODESEL_1);               
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_FMODESEL_1);
         }

         // disable auto tune
         #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new AutoTune
         vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
         #else
         vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
         #endif
    }
}


/**
* @brief Search dispmode tune record
* @return table index
*/
static UINT32 _u4ScpipSearchRecord(void)
{
    UINT32 index;

    for (index = 0; index < ADDR_STACK_SIZE_DISP; index ++)
    {
        if ((AddrStack[index].u4PanelW == wDISPLAY_WIDTH) &&
            (AddrStack[index].u4PanelH == wDISPLAY_HEIGHT) &&
            (AddrStack[index].u4SrcW == _rMPicInfo.wSrcW) &&
            (AddrStack[index].u4SrcH == _rMPicInfo.wSrcH) && 
            (AddrStack[index].u4FrameRate == bDrvVideoGetRefreshRate(SV_VP_MAIN)) &&
            (AddrStack[index].u4Interlace == bDrvVideoIsSrcInterlace(SV_VP_MAIN)) &&
            (AddrStack[index].u4USclVMin <= _rMDispPrm.u4USclV) && 
            (AddrStack[index].u4USclVMax >= _rMDispPrm.u4USclV) && 
            (AddrStack[index].u4DSclVMin <= _rMDispPrm.u4DSclV) && 
            (AddrStack[index].u4DSclVMax >= _rMDispPrm.u4DSclV))
        {
            return index;
        }
    }
    return ADDR_STACK_SIZE_DISP;
}


/**
* @brief _vScpipSetDispModeReg
*/
static void _vScpipSetDispModeReg(void) 
{
    if (u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_AUTO)    // enable adative fine-tune mechanism in Display Mode
    {
        #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE //5387, use new auto tune
        UINT32 index;

        index = _u4ScpipSearchRecord();
        if (index != ADDR_STACK_SIZE_DISP) // find record
        {
            _VDP_SetVspTune(AddrStack[index].VSP, AddrStack[index].TUNE);
            vScpipSetDispModeVar(SV_VP_MAIN, VDP_SCPOS_DISPMODE_MANUAL);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_RESET_AUTO_TUNE_1);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
        }        
        else
        {
            UINT32 u4TuneStep;
            if ( (wDISPLAY_WIDTH == 1440) && (wDISPLAY_HEIGHT == 900)
            && ((bDrvVideoGetTiming(SV_VP_MAIN) == MODE_1080i_50) || (bDrvVideoGetTiming(SV_VP_MAIN) == MODE_1080i)) )
            {
                 u4TuneStep = 0x03;        // for 538x 1080i display mode status error
            }
            else
            {
                 u4TuneStep = 0x08;
            }
    
            // set tune step
            vRegWriteFldAlign(SCPIP_PIP1_0A, u4TuneStep, PIP1_0A_TUNE_STEP_1);        
    
            // reset VSP and TUNE
            if ((wDISPLAY_WIDTH == 1440) && (wDISPLAY_HEIGHT == 900) &&
                (_rMPicInfo.wSrcW == 1440) && (_rMPicInfo.wSrcH == 900))
            {
                 // Auto tune workaround
                 _VDP_SetVspTune(1, 0);
            }
            else
            {
                 _VDP_SetVspTune(0, 0);
            }
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
            vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_RESET_AUTO_TUNE_1);
        }        
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
        #endif
    }    
    else if (u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_MANUAL)
    {
        UINT32 index;
        index = _u4ScpipSearchRecord();

        if (index != ADDR_STACK_SIZE_DISP) // find record
        {
            _VDP_SetVspTune(AddrStack[index].VSP, AddrStack[index].TUNE);
        }
        else
        {        
            UINT32 u4Vsp;
            UINT32 u4Tune;
        
            u4Vsp = 0;
            u4Tune = 0;
    
            // disable auto tune
            #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
            #else
            vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
            #endif
            
            if ((_rMDispPrm.u4USclH != DFT_US_FACTOR) || (_rMDispPrm.u4USclV != DFT_US_FACTOR) ||
                (_rMDispPrm.u4DSclH != DFT_DS_FACTOR) || (_rMDispPrm.u4DSclV != DFT_DS_FACTOR))
            {
                if ((_rMDispPrm.u4USclV == DFT_US_FACTOR) && (_rMDispPrm.u4DSclV == DFT_DS_FACTOR))
                {   
                    /* Vertical No Scaling: DISP_VSP = 2, DISP_TUNE = 0 */
                    u4Vsp = 2;
                    u4Tune = 0;                
                }
                else if (_rMDispPrm.u4DSclV != DFT_DS_FACTOR)
                {
                    /* Vertical Down Scaling: DISP_VSP = 0, DISP_TUNE = 0 */
                    u4Vsp = 0;
                    u4Tune = 0x10;             
                }                
                else
                if ((_rMDispPrm.u4USclV >= 1900) && (_rMDispPrm.u4USclV < DFT_US_FACTOR))
                { 
                    /* Vertical Up Scaling Factor 1900~2048: DISP_VSP = 2, DISP_TUNE = 0 */
                    u4Vsp = 2;
                    if ((_rMPicInfo.wTargetW == 1280) && (_rMPicInfo.wTargetH == 960) &&
                            (_rMDispInfo.wWidth == 1280) && (_rMDispInfo.wHeight == 1024))
                    {
                        u4Tune = 0x0a;             
                    }
                    else
                    {
                        u4Tune = 0;             
                    }
                }        
                else 
                if ((_rMDispPrm.u4USclV >= 1024) && (_rMDispPrm.u4USclV <= 1899))
                { 
                    /* Vertical Up Scaling Factor 1024~1899: DISP_VSP = 3, DISP_TUNE = 0 */
                    u4Vsp = 0x03;
                    u4Tune = 0;             
                }        
                else 
                if (_rMDispPrm.u4USclV < 1024)
                {
                    /* Vertical Up Scaling Factor < 1024: DISP_VSP = 3, DISP_TUNE = ?? Need to Fine Tune */
                    u4Vsp = 3;
                    if ((_rMPicInfo.wTargetW == 640) && (_rMPicInfo.wTargetH == 480) &&
                            (_rMDispInfo.wWidth == 1280) && (_rMDispInfo.wHeight == 1024))
                    {
                        u4Tune = 0x32;             
                    }
                }            
            }// end of scaling mode
            
            else
            {                       
                // No Scaling: DISP_VSP = 2, DISP_TUNE = 0 
                u4Vsp = 2;
                u4Tune = 0;                     
            }
            _VDP_SetVspTune(u4Vsp, u4Tune);
        }
    }
}


/**
* @brief Get dispmode status
* @param bPath VDP_1/VDP_2
* @return Dispmode type
*/
UINT8 u1ScpipGetDispMode(UINT8 bPath)
{
    ARRAY_BOUND_CHECK(bPath, SV_VP_NA);
    if(bPath == SV_VP_MAIN)
    {
        return _arScalePrm[SV_VP_MAIN].u4DispMode;
    }
    else
    {
        return _arScalePrm[SV_VP_PIP].u4DispMode;
    }
}

/**
* @brief Set dispmode status
* @param bPath VDP_1/VDP_2
* @param bPath VDP_1/VDP_2
*/
void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode)
{
    _arScalePrm[bPath].u4DispMode = (UINT32) u1Dispmode;
}


/**
* @brief Force to turn Off Channel Write Control 
* @param u4VdpId VDP_1/VDP_2
* @return u1OnOff On/Off
*/
void vScpipForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff) 
{
#if CC_SCPOS_FORCE_WOFF
    CRIT_STATE_T csState;
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    if (u1OnOff)
    {
        LOG(9, "Enable SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    else
    {
        LOG(9, "Cancel SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    csState = x_crit_start();    
    _au4ForceWriteOff[u4VdpId] = u1OnOff;
    x_crit_end(csState);
#else
#ifndef CC_UP8032_ATV
    UNUSED(u4VdpId);
    UNUSED(u1OnOff);
#endif       
#endif       
}

static void _vScpipSetDramWrite(UCHAR ucVdpId, UCHAR ucOnOff)
{
    UINT8 pipOffset;
    
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    pipOffset = getPIPOffset(ucVdpId);
    vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, ucOnOff, PIP1_09_W_EN_1);
    LOG(9, "_vScpipSetDramWrite(Path:%d, OnOff %d)\n", ucVdpId,ucOnOff );
}

static void _vScpipSetDramRead(UCHAR ucVdpId, UCHAR ucOnOff)
{
    UINT8 pipOffset;
    
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    pipOffset = getPIPOffset(ucVdpId);
    vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, ucOnOff, PIP1_09_R_EN_1);
    LOG(9, "_vScpipSetDramRead(Path:%d, OnOff:%d)\n", ucVdpId,ucOnOff);
}

/**
* @brief Turn On/Off Main Channel Write Control 
* @param u1OnOff On/Off
*/
//void vDrvMainWriteCtrl(UCHAR ucVdpId, UCHAR ucOnOff)
void vScpipWriteCtrlOnOff(UCHAR ucVdpId, UCHAR ucOnOff)
{
    CRIT_STATE_T csState;
    RVChannel *prChannel;
    UINT8 pipOffset;
    
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
   
    pipOffset = getPIPOffset(ucVdpId);
    if (_u4IsScpipDramBufferInit(ucVdpId) == 0)
    {
        return;
    }

    prChannel = getChannel(ucVdpId);
    csState = x_crit_start();   
 
    if (((ucOnOff == SV_ON) && 
#if CC_SCPOS_FORCE_WOFF
        (_au4ForceWriteOff[ucVdpId] == 0) &&
#endif       
        (_arScalePrm[ucVdpId].u4Enable ==1) &&
        (prChannel->bIsChannelOn == SV_ON) && 
        (prChannel->bIsFreeze == SV_OFF) &&
        (_arScalePrm[ucVdpId].u4DispModeEn == 0) &&
        (u1ScpipGetDispMode(getScalerPath(ucVdpId)) == VDP_SCPOS_DISPMODE_OFF))
        )
    {
        if (_u4ResetScposDramBuffer[ucVdpId] != 0)
        {
            _au4DelayWriteEnable[ucVdpId] = 1;
        }
        else
        {
            if(ucVdpId==VDP_1)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_06, 1, PIP_OA_06_RFIFO_PRO1);   
            }
            #ifdef CC_SCPOS_LOG_DRAM_PARAM
            if (RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_W_EN_1) == 0)
            {
                HAL_TIME_T dt;
                HAL_GetTime(&dt);
                _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
                _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
                _au4ValueLog[_u4LogIdx] = (1);
                _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
            }
            #endif
            #ifdef CC_SCPOS_3DTV_SUPPORT                    
            //{
            //    UINT32 u4MuteDelay = _vDrvVideoGetMuteDelay(getScalerPath(ucVdpId), MUTE_MODULE_SCPOS_TV3D);
                
            //    if(u4MuteDelay >= 0xffff) // when mute delay is big, it's FOREVER_MUTE
            //    {
            //        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, ucVdpId, _u4ScpipReCalDelayCounter(ucVdpId, VDP_SCPOS_TV3D_MUTE_DELAY), FALSE);
            //    }
            //}
            #else
	        if ((RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_W_EN_1) == 0)
				&&(u1ScpipGetDispMode(ucVdpId) == VDP_SCPOS_DISPMODE_OFF)
				&&(RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_R_EN_1) != 1))
	    	{
	    		 _vDrvVideoSetMute(MUTE_MODULE_SCPOS_MISC, ucVdpId, VDP_SCPOS_WEN_WAIT, FALSE);
	                 LOG(6, "MUTE_MODULE_SCPOS_MISC : Before Turn ON Write Mute\n");
	    	}
	        #endif
            #if SUPPORT_SBS_CONVERT
            if(_bSBSOutput || _b2dTo3d)
            {
                if(VDP_1 == ucVdpId)
                {
                    if(_bResetScalerCount)
                    {
                    _bResetScalerCount --;
                    }
                    else
                    {
                    _vScpipSetDramWrite(VDP_2, SV_ON);
                    _vScpipSetDramWrite(VDP_1, SV_ON);   
                    _vScpipSetDramRead(VDP_2, SV_ON);                    
                    _vScpipSetDramRead(VDP_1, SV_ON); 

                    }
                }
                else
                {
                /*SUB DO NOTING*/
                }
            }            
            else
            #endif
            #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
            if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR)
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR)
                || ((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))
            {
                if(VDP_1 == ucVdpId)
                {
                    if(_bResetScalerCountFor3DPR)
                    {
                        _bResetScalerCountFor3DPR--;
                    }
                    else
                    {
                        _vScpipSetDramWrite(VDP_2, SV_ON);
                        _vScpipSetDramWrite(VDP_1, SV_ON);
                        _vScpipSetDramRead(VDP_2, SV_ON);                    
                        _vScpipSetDramRead(VDP_1, SV_ON);                   
                    }       
                }
                else
                {
                    /*SUB DO NOTING*/
                }
            }
            else
            #endif
            {
                _vScpipSetDramWrite(ucVdpId, SV_ON);
                _vScpipSetDramRead(ucVdpId, SV_ON);
            }
        }
    } 
    else
    {        

        #ifdef CC_SCPOS_LOG_DRAM_PARAM
        if (RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_W_EN_1) == 1)
        {
                HAL_TIME_T dt;
                HAL_GetTime(&dt);
                _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
                _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
                _au4ValueLog[_u4LogIdx] = (0);
                _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        }
        #endif
        _au4DelayWriteEnable[ucVdpId] = 0;
        _vScpipSetDramWrite(ucVdpId, SV_OFF);
#ifndef __MODEL_slt__
        //if not in freeze mode, turn off read control
        if(prChannel->bIsFreeze == SV_OFF)
        {
        	 if((u1ScpipGetDispMode(ucVdpId) == VDP_SCPOS_DISPMODE_OFF) &&(RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_W_EN_1) == 1))
 		{
 			_vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, ucVdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
 		}
            //if ((ucVdpId==VDP_1)|| 
            //    ((ucVdpId==VDP_2) &&(SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL)))
            //{  // Single View: do not read to turn on Read control
                _vScpipSetDramRead(ucVdpId, SV_OFF);    
           // }
                
            if(ucVdpId==VDP_1)
            {
                vRegWriteFldAlign(SCPIP_PIP_OA_06, 0, PIP_OA_06_RFIFO_PRO1);   
            }
        }
#endif        
    }
    x_crit_end(csState);
}


/**
* @brief Check write control status
* @param bPath VDP_1/VDP_2
*/
static void _vScpipWriteCTRLCheck(UINT8 bPath)
{
    RVChannel *prChannel;
    UINT8 bSigStatus;
    UINT8 bWriteOnOff= SV_OFF;
       
    bSigStatus = bDrvVideoSignalStatus(bPath);
    prChannel = getChannel(bPath);

    if (prChannel->bIsChannelOn == (UINT8)SV_ON)
    {
        #if SUPPORT_ATV_SNOWSCREEN
        if(fgIsSrcAtv(bPath) && (u4ApiVideoGetSnowScreen() ==SV_TRUE))
        {
            bWriteOnOff = ((prChannel->bIsFreeze == SV_OFF)? SV_ON : SV_OFF);
        }
        else // non-atv input
        #endif//  #if SUPPORT_ATV_SNOWSCREEN
        {
            if ((bSigStatus == (UINT8)SV_VDO_STABLE) && (prChannel->bIsFreeze == (UINT8)SV_OFF))
            {
                bWriteOnOff = SV_ON;
            }
            else
            {
                bWriteOnOff = SV_OFF;
            }
        }
    }

    vScpipWriteCtrlOnOff(bPath, bWriteOnOff);
}

/**
* @brief Set game mode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void vScpipSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_au4GameMode[ucVdpId] != ucOnOff)
    {
        LOG(3, "VDP(%d) Game Mode(%d)\n", ucVdpId, ucOnOff);
        _au4GameMode[ucVdpId] = ucOnOff;

        if (ucOnOff)
        {
            UINT32 u4FrameNum; 
    
            // force frame number to be less than 2.
            u4FrameNum =  u4ScpipGetBufNum((UINT32)ucVdpId);
            if (u4FrameNum > 2)
            {
                vScpipSetBufNum((UINT32)ucVdpId, 2);
            }
        }
        else
        {
            vScpipSetBufNum((UINT32)ucVdpId, _au4OriFrameNum[ucVdpId]);
        }
    }
}


/**
* @brief Set dot by dot mode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void vScpipSet121Map(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4121Map != (UINT32) ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
        }
    
        _arScalePrm[ucVdpId].u4121Map = (UINT32) ucOnOff;

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }
}


UINT32 vScpipGet121Map(UCHAR ucVdpId)
{
    return _arScalePrm[ucVdpId].u4121Map;
}

#if 0
/**
* @brief Set dot by dot mode and bypass VDS/VUS
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void vScpipForceBypassOnOff(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4ForceBypass != (UINT32) ucOnOff) 
    {
        _arScalePrm[ucVdpId].u4ForceBypass = (UINT32) ucOnOff;
        vScpipSet121Map(ucVdpId, ucOnOff);    

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }
}
#endif
/**
* @brief Fix pre-scaling down factor
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
* @param uc1stConfig configure paramter
*/
void vDrvVideoFixedPreScaler(UCHAR ucVdpId, UCHAR ucOnOff, UCHAR uc1stConfig)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4FixedPreScaler != (UINT32) ucOnOff) 
    {
        if (_arScalePrm[ucVdpId].u4FixedPreScaler != 0)
        {
            if (_arScalePrm[ucVdpId].u4PreScalerNotOptimized != 0)
            {
                // re-scale if NOT optimized!
                if (ucVdpId == VDP_1)
                {
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                }
                else
                {
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                }                
            }
        }

        _arScalePrm[ucVdpId].u4FixedPreScaler = (UINT32) ucOnOff;
    }

    _arScalePrm[ucVdpId].u4FirstConfig = (UINT32) uc1stConfig;
}


/**
* @brief Set pre-scaling down factor
* @param u4VdpId VDP_1/VDP_2
* @param u4Factor pre-scaling down factor
*/
void _VDP_SetHorizontalPreScaleDownFactor(UINT32 u4VdpId, UINT32 u4Factor)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    _arScalePrm[u4VdpId].u4ForcedPreScalingactor = u4Factor;
    
    if (u4VdpId == VDP_1)
    {
           vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    {
          vSetScposFlg(PIP_DISP_PRM_CHG);
    }                
}

/**
* @brief Set 10/8 bit mode
* @param u4VdpId VDP_1/VDP_2
* @param u4Mode 10/8 bit
*/
void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (_u4Forced10BitMode[u4VdpId] != u4Mode)
    {
        _u4Forced10BitMode[u4VdpId] = u4Mode;
        if (u4VdpId == VDP_1)
        {
              vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
              vSetScposFlg(PIP_DISP_PRM_CHG);
        }                
    }

}


/**
* @brief Set dynamic scaler
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
* @param u4ResponseTime response time
*/
void vScpipSetDynamicScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime)
{
    UINT32 u4OutputRate;
    
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4DynamicScale != (UINT32) ucOnOff)
    {
        // Protection, Stop dynamic Scaler while scaling
        if (_arDynamicPrm[ucVdpId].u4State != VDP_DYNAMICE_SCALE_IDLE)
        {
            if (ucVdpId == VDP_1)
            {
                vSetScposFlg(MAIN_DISP_PRM_CHG);
            }
            else
            {
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
        }
    
        _arDynamicPrm[ucVdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;
    }

    _arScalePrm[ucVdpId].u4DynamicScale = (UINT32) ucOnOff;
    _arScalePrm[ucVdpId].u4ResponseTime = u4ResponseTime;
    
    u4OutputRate = (UINT32)((vDrvGetLCDFreq() > 0) ? vDrvGetLCDFreq(): 60);
    _arDynamicPrm[ucVdpId].u4TotalScaleNs = 1 + DIVIDE_WITH_CARRIER((u4ResponseTime * u4OutputRate), 
        (1000 * VDP_SCPOS_RESCALE_TIME));

    if (_arScalePrm[ucVdpId].u4DynamicScale != 0)
    {
        if (_arScalePrm[ucVdpId].u4FixedPreScaler == 0)
        {
            LOG(1, "Warning: Dynamic Scaling but PreScale Not Fixed!\n");
        }
    }
}


/**
* @brief Enable/Disable scaler dispmode
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucOnOff)
{
    UINT8 bPath = getScalerPath(ucVdpId);
    
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    
    #if CC_SCPOS_VSYNC_WA2
        if (LVDS_MUTE_IN_DISPLAY_MODE == LVDS_MUTE_IN_DISPLAY_MODE_ON)
        {
            // DE channel: need to check (SCPOS_LVDS_PD_2?)           
            if (ucOnOff && (RegReadFldAlign(OUTSTG_OS_15, OS_15_DEN_EN) == 0)
            && (u1ScpipGetDispMode(bPath) != ucOnOff))
            {
                PMX_SetDisplayMode(SV_TRUE);
                LOG(1, "PMX_SetDisplayMode(TRUE)\n");
              return;
            }
            else if (!ucOnOff)
            {
                PMX_SetDisplayMode(SV_FALSE);
                LOG(1, "PMX_SetDisplayMode(FALSE)\n");
            }
        }
    #endif

    LOG(4,"Set scaler mode by SRM. VDP(%d) mode(%d)\n",ucVdpId,ucOnOff);
    
    if (u1ScpipGetDispMode(bPath) != ucOnOff) 
    {
        LOG(4,"Scaler mode changed. VDP(%d) mode(%d->%d)\n",ucVdpId,u1ScpipGetDispMode(bPath),ucOnOff);
        vScpipSetDispModeVar(bPath, (UINT8)ucOnOff);

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
            vSetScposFlg(SCALER_MAIN_MODE_CHG);
        }
        else
        {
            if(ucOnOff != VDP_SCPOS_DISPMODE_OFF)
            {
                //Hardware Limitation, sub path can't get into scaler dispmode
                ASSERT(0);
            }            
        }
    }
}


#ifdef CC_CLI
/**
* @brief Set vsp and tune
* @param u4Vsp
* @param u4Tune
*/
void _VDP_SetVspTune(UINT32 u4Vsp, UINT32 u4Tune)
{
    vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
    vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);
}


/**
* @brief Set scaler dispmode related registers
* @param u4Auto Auto/Manual
* @param u4Step Tune step
* @param u4Print Log information
*/
void _VDP_DispModeTune(UINT32 u4Auto, UINT32 u4Step, UINT32 u4Print)
{
    UINT32 u4Vsp;
    UINT32 u4Tune;
    UINT32 u4Down;
    UINT32 u4SingleStep;

    u4Down = u4Step & 0x80000000;
    u4SingleStep = u4Step & 0x7fffffff;

    //1. get current tune and vsp value
    u4Tune = RegReadFldAlign(SCPIP_PIP1_0A,PIP1_0A_FSYNC_TUNE_1);
    u4Vsp = RegReadFldAlign(SCPIP_PIP1_0A,PIP1_0A_FSYNC_VSP_1);
    
    if (u4Print != 0)
    {
        UINT32 u4FifoOver1 = 0;
        UINT32 u4FifoOver2 = 0;
        UINT32 u4FifoOverLine1 = 0;
        UINT32 u4FifoOverLine2 = 0;
        UINT32 u4WAP = 0;

        //2. get overflow information
        u4FifoOver1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER1_1);
        u4FifoOver2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER2_1);       
        u4FifoOverLine1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE1_1);
        u4FifoOverLine2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE2_1);
        u4WAP = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_WAP_RA_1);
               
        LOG(1, "Vsp(%d) Tune(%d) O1(%d, %d) O2(%d, %d) WAP_RA(%d)\n", 
            u4Vsp, u4Tune, 
            u4FifoOver1, u4FifoOverLine1,
            u4FifoOver2, u4FifoOverLine2, u4WAP);

        UNUSED(u4FifoOver1);
        UNUSED(u4FifoOver2);
        UNUSED(u4FifoOverLine1);
        UNUSED(u4FifoOverLine2);
    }
    else if (u4Auto != 0)
    {
    }
    else
    {
        if (u4Down == 0)
        {
            if ((u4Tune + u4SingleStep) <= MAX_FSYNC_TUNE)
            {
                u4Tune += u4SingleStep;
                //set tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);
            }
            else
            {
                u4Tune = 0;
                u4Vsp++;
                //set vsp and tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                
            }
        }
        else
        {
            if (u4Tune >= u4SingleStep)
            {
                u4Tune -= u4SingleStep;
                //set tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                                
            }
            else
            if (u4Vsp > 0)
            {
                u4Tune = MAX_FSYNC_TUNE;
                u4Vsp--;
                //set vsp and tune value
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Vsp , PIP1_0A_FSYNC_VSP_1);
                vRegWriteFldAlign(SCPIP_PIP1_0A, u4Tune , PIP1_0A_FSYNC_TUNE_1);                
            }
        }
    }
}
#endif


/**
* @brief Set scpos delay enable time
* @param ucVdpId VDP_1/VDP_2
* @param ucEnable Enable/Disable
* @param u4DelayCount Delay time
*/
void vScpipDelayEnable(UCHAR ucVdpId, UCHAR ucEnable, UINT32 u4DelayCount)
{
    LOG(3, "vScpipDelayEnable(%d) %d\n", ucVdpId, ucEnable);

    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    
    if (ucEnable)
    {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, u4DelayCount, FALSE);
            _VDP_ScposEnable(ucVdpId, TRUE);
    }
    else
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PLA_EN, ucVdpId, FOREVER_MUTE, FALSE);
        _VDP_ScposEnable(ucVdpId, FALSE);
    }
}


static void vScpipPowerCtrl(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath ==SV_VP_PIP)
    {
        if(bOnOff==0)
        {
            vIO32WriteFldAlign(CKGEN_DISP_CKCFG, 0, FLD_SCPIP_PIP_CLK_EN);
        }
        else
        {
            vIO32WriteFldAlign(CKGEN_DISP_CKCFG, 1, FLD_SCPIP_PIP_CLK_EN);
        }
    }
}

/**
* @brief Set scpos enable/disable
* @param ucVdpId VDP_1/VDP_2
* @param ucEnable Enable/Disable
*/
static void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable)
{
    _arScalePrm[ucVdpId].u4Enable = ucEnable;    

    if (ucEnable)
    {
        UINT32 u4OutputRate;
        u4OutputRate = (UINT32)((vDrvGetLCDFreq() > 0) ? vDrvGetLCDFreq(): 60);
        _arDynamicPrm[ucVdpId].u4TotalScaleNs = 1 + DIVIDE_WITH_CARRIER((_arScalePrm[ucVdpId].u4ResponseTime * u4OutputRate), 
            (1000 * VDP_SCPOS_RESCALE_TIME));
       // vScpipWriteCtrlOnOff(ucVdpId, SV_ON); 
    }
    else
    {
        // Reset Dynamic Scaling State
        _arDynamicPrm[ucVdpId].u4State = VDP_DYNAMICE_SCALE_IDLE;
        _arDynamicPrm[ucVdpId].u4LastFactorValid = 0;
        vScpipWriteCtrlOnOff(ucVdpId, SV_OFF);
    }
    vScpipPowerCtrl(ucVdpId, ucEnable);        
} 

/**
* @brief Get pre-scaling down factor
* @param ucVdpId VDP_1/VDP_2
* @return pre-scaling down factor
*/
UINT32 u4ScpipGetHorizontalPreScaleDownFactor(UCHAR ucVdpId)
{
    ARRAY_BOUND_CHECK(ucVdpId, VDP_NS);
    return _arScalePrm[ucVdpId].u4HpsFactor;
}

/**
* @brief Check if dram paramters change
* @param u4VdpId VDP_1/VDP_2
* @param u4Addr base address
* @param u4Size frame buffer size
* @param u4FrameNum frame buffer number
*/
static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum)
{
    UINT8 u1Changed = 0;
    UINT32 u4OriFrameNum;
    UINT32 u4OriAdr;
    UINT32 u4OriSize;

    ASSERT(u4FrameNum > 0);
    
    u4Addr >>= 4;
    u4Size >>= 4;
    //u4FrameNum -= 1;

    #if !SUPPORT_POP
    if(u4VdpId == VDP_2)
    {
        return 0;
    }
    #endif

    //get frame number
    //u4OriFrameNum = RegReadFldAlign(SCPIP_PIP1_09+getPIPOffset(u4VdpId), PIP1_09_FSEL_1);
    u4OriFrameNum = u4ScpipGetBufNum(u4VdpId);
    //get frame size
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+getPIPOffset(u4VdpId), PIP1_07_WINC_1);
    u4OriAdr = u4ScpipGetWAddr(u4VdpId);
    if (u4OriFrameNum != u4FrameNum)
    {
        u1Changed |= FRAME_NUM_CHG;
    }
    if (u4OriAdr != u4Addr)
    {
        u1Changed |= BASE_WADDRESS_CHG;
    }
    if (u4OriSize != u4Size)
    {
        u1Changed |= FRAME_SIZE_CHG;
    }
    if (u1Changed > 0)
    {
        LOG(3, "VDP(%d) DRAM parameters are changed! Original:(0x%X, %d, %d), New:(0x%X, %d, %d)\n", 
            u4VdpId, u4OriAdr<<4, u4OriSize<<4, u4OriFrameNum, u4Addr<<4, u4Size<<4, u4FrameNum);
    }

    return u1Changed;
}
    


/**
* @brief Enable/Disable scaler dispmode freerun mechanism
* @param bEnable On/Off
*/
void vScpipDispmodeFreeRun(BOOL bEnable)
{
#ifndef CC_UP8032_ATV
    vRegWriteFldAlign(SCPIP_PIP1_0B, bEnable, PIP1_0B_SP_PRO_1);
#else
    if(bEnable)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_SP_PRO_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_SP_PRO_1);
    }
#endif
}

void vScpipSetDispModeFreeRunCount(UINT32 u4Count)
{
	_u4DispModeFreeRunCounter= u4Count;
}

/**
* @brief Scaler dispmode freerun check
*/
static void _vScpipDispmodeFreeRunChk(void)
{
	static UINT8 bStableCount=0;

	if(u4GetFrameTrackMode() == 2)// SW frame track
	{
		if(_u4DispModeFreeRunCounter==0)
		{
			bStableCount=0;
			return;
		}
		else
		{	
			if(bStableCount==0)
			{
				vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
	    			vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);      
			}
			
	    		if( eScpipGetFrameTrackStatus()==SCPIP_FRAME_TRACK_LOCK)
	    		{
				if(bStableCount <0xff)
	    			{
	    				bStableCount++;
	    			}
				
	    			if(bStableCount==3)
	    			{
		    			vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_F_SYNC_1);	
				}
	    			else if(bStableCount==5)
	    			{
	    				vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);   
	    			}
				else if(bStableCount==15)
				{
					vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
	    				LOG(3, "_vScpipDispmodeFreeRunChk %d %d\n\n", _u4DispModeFreeRunCounter, eScpipGetFrameTrackStatus());
	    				_u4DispModeFreeRunCounter = 0;
				}
	    		}
	    		else
	    		{
	    			bStableCount=0;
	    		}

	    		if(_u4DispModeFreeRunCounter==3)
			{
				vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_F_SYNC_1);
				vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);  
			}
	    		else if(_u4DispModeFreeRunCounter==1)
	    		{
				vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_F_SYNC_1);
	    		}
	    		
	    		if(_u4DispModeFreeRunCounter)
	    			_u4DispModeFreeRunCounter --;

		}
	}
	else //hw frame track
	{
	    if(((_u4DispModeFreeRunCounter == 1) || ( eScpipGetFrameTrackStatus()==SCPIP_FRAME_TRACK_LOCK))&&
	        _u4DispModeFreeRunCounter!=0)
	    {
	        LOG(3, "_vScpipDispmodeFreeRunChk %d %d\n\n", _u4DispModeFreeRunCounter, eScpipGetFrameTrackStatus());
	            
	        _u4DispModeFreeRunCounter = 0;
	        if (u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_AUTO)    // enable adative fine-tune mechanism in Display Mode
	        {
	            #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new AutoTune
	            vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_RESET_AUTO_TUNE_1);       
	            vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_AUTO_TUNE_EN_1);
	            #else
	            vRegWriteFldAlign(SCPIP_PIP_OA_08, 1, PIP_OA_08_DFIFO_AUTO_TUNE1);       
	            #endif
	        }
	    }
	    else if (_u4DispModeFreeRunCounter)
	    {
	        _u4DispModeFreeRunCounter--;
	    }
	}
}


/**
* @brief Check if scaler dispmode fifo overflow
* @return overflow status
*/
UINT32 u4ScpipDispmodeFifoOverflow(void)
{
    UINT32 u4DispActive;
    UINT32 u4FifoOver1 = 0;
    UINT32 u4FifoOver2 = 0;
    UINT32 u4FifoOverLine1 = 0;
    UINT32 u4FifoOverLine2 = 0;
    UINT32 u4WapRA;

    if (_rMDispPrm.u4USclV != DFT_US_FACTOR)        // US
    {
        u4DispActive = _rMPicInfo.wTargetH;
    }
    else                                            // DS
    {
        u4DispActive = _rMDispInfo.wHeight; //wDISPLAY_HEIGHT;
    }

    u4FifoOver1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER1_1);
    u4FifoOver2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_FIFO_OVER2_1);       
    u4FifoOverLine1 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE1_1);
    u4FifoOverLine2 = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_OVERLINE2_1);
    u4WapRA = RegReadFldAlign(SCPIP_PIP1_RO_03, PIP1_RO_03_WAP_RA_1);
    
    if ((u4FifoOver1) && (u4FifoOverLine1 > _rMPicInfo.wYOffset) && (u4FifoOverLine1 < u4DispActive))
    {
        return 1;
    }
    else if ((u4FifoOver2) && (u4FifoOverLine2 > _rMPicInfo.wYOffset) && (u4FifoOverLine2 < u4DispActive))
    {
        return 2;
    }
    else
    {
        if(u4WapRA<=2)
        {
            return 0;
        }
        else
        {
            return 3;
        }
    }
 }


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#ifdef CC_CLI
/**
* @brief Print current scale paramters
* @param ucVdpId VDP_1/VDP_2
*/
void vScpipPrintCurrentScale(UCHAR ucVdpId)
{
    if (_pChannel[ucVdpId]->bIsChannelOn != SV_OFF)
    {
        //LOG(1, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
        Printf("SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
            ucVdpId,  
            _pDispInfo[ucVdpId]->wRXOffset,
            _pDispInfo[ucVdpId]->wRYOffset,
            _pDispInfo[ucVdpId]->wRWidth,
            _pDispInfo[ucVdpId]->wRHeight,
            getMixedHPorch(ucVdpId),
            getMixedVPorch(ucVdpId),
            _pPicInfo[ucVdpId]->wTargetW,
            _pPicInfo[ucVdpId]->wTargetH);

        //LOG(1, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
        Printf("HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
            _pDispPrm[ucVdpId]->u4USclH, _pDispPrm[ucVdpId]->u4USclV,
            _arScalePrm[ucVdpId].u4HpsFactor, 
            _pDispPrm[ucVdpId]->u4DSclH, _pDispPrm[ucVdpId]->u4DSclV);
    }
}
#endif

/**
* @brief Get scpos porch
* @param bPath VDP_1/VDP_2
* @param bPorchType porch type
* @return scpos porch
*/
UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    UINT32 u4VdpId=(UINT32)getScalerVDP(bPath); /* use u4VdpId makes _pPicInfo[u4VdpId] search easier*/
    switch(bPorchType)
    {
        case SV_HPORCH_CURRENT:
            return _pPicInfo[u4VdpId]->wUIHPorch;
        case SV_HPORCH_DEFAULT:
            return 1000;
        case SV_VPORCH_CURRENT:
            return _pPicInfo[u4VdpId]->wUIVPorch;
        case SV_VPORCH_DEFAULT:
            return 1000;
        case SV_HPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcW-_pPicInfo[u4VdpId]->wTargetW - _pPicInfo[u4VdpId]->wXOffset;
        case SV_VPORCH_MAX:
            return 1000+_pPicInfo[u4VdpId]->wSrcH -_pPicInfo[u4VdpId]->wTargetH - _pPicInfo[u4VdpId]->wYOffset;
        case SV_HPORCH_MIN:
            return  1000 - _pPicInfo[u4VdpId]->wXOffset;
        case SV_VPORCH_MIN:
            return  1000 - _pPicInfo[u4VdpId]->wYOffset;
        default: /*MinMax*/
            return 1000;
    }
}


/**
* @brief Set scpos porch
* @param bPath VDP_1/VDP_2
* @param bPorchType porch type
* @param wValue porch value
*/
void _VDP_ScposSetPorch(UINT8 bPath,UINT8 bPorchType,UINT16 wValue)
{
    /* trigger from UI*/
    UINT32 u4VdpId = (UINT32)getScalerVDP(bPath); /* use u4VdpId makes _pPicInfo[u4VdpId] search easier*/
    UINT32 u4Update;

    u4Update = 0;
    if (bPorchType==SV_HPORCH_CURRENT)
    {
        if (wValue != _pPicInfo[u4VdpId]->wUIHPorch) 
        {
            _pPicInfo[u4VdpId]->wUIHPorch = wValue;
            u4Update = 1;
        }
    }
    else
    {
        if (wValue != _pPicInfo[u4VdpId]->wUIVPorch) 
        {
            _pPicInfo[u4VdpId]->wUIVPorch = wValue;
            u4Update = 1;
        }
    }

    if (u4Update)
    {
        if (bPath == VDP_1)
        {
            vSetScposFlg(MAIN_HV_CHG);
        }
        else
        {
            vSetScposFlg(PIP_HV_CHG);
        }
    }
    
}
    
/**
* @brief Initialize tve related global paramters
*/
#ifdef CC_FAST_INIT 
static vScpipResume(void)
{
	UINT8 bForCount;
	//Global variable init
	#if SUPPORT_SBS_CONVERT
	UINT8 _bSBSOutput= SV_FALSE;
	#endif

	// Static variable init
	for(bForCount=0; bForCount <VDP_NS; bForCount++)
	{
		_u4ResetScposDramBuffer[bForCount] = 0;
		_au4DelayWriteEnable[bForCount] = 0;

		_au4GameMode[bForCount] = 0;
		_au4OriFrameNum[bForCount] = 0;

		_au4ForcePsDivClkEn[bForCount] = 0;
		_au4ForcePsDivClkM[bForCount] = 0;
		_au4ForcePsDivClkN[bForCount] = 0;

		_u4BaseAddress[bForCount]= 0; // scaler dram base address
		_bSRMStable[bForCount]= SV_FALSE;
		_bIsDramBufInit[bForCount]= SV_FALSE;

		// When TVE on, force dispOff
		_u4ForceDispOff[bForCount]=SV_FALSE;

		_u4ScposImportNotReady[bForCount] = 0;
		_u4Forced10BitMode[bForCount] = 0;
		_au4ScalingFactorChanged[bForCount] = 0;
		
		#if CC_SCPOS_FORCE_WOFF
		_au4ForceWriteOff[bForCount] = 0;
		#endif
		#if CC_SPEEDUP_IMPORT
		_au4ImportUpdate[bForCount] = 0;
		#endif
		// TODO: remove
		#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
		_counterToInvertVSync[bForCount] =0;
		_flagToInvertVSync[bForCount] = SV_FALSE;
		#endif
	}

	_u4DispModeFreeRunCounter =0;

	//#define CC_SCPOS_LOG_DRAM_PARAM (50)
	#ifdef CC_SCPOS_LOG_DRAM_PARAM
	_au4TimeLog[CC_SCPOS_LOG_DRAM_PARAM];
	_au4AddressLog[CC_SCPOS_LOG_DRAM_PARAM];
	_au4ValueLog[CC_SCPOS_LOG_DRAM_PARAM];
	_u4LogIdx = 0;
	#endif

	_fgIsHNegInitPhaseOn = SV_ON;
	_fgIsVNegInitPhaseOn = SV_ON;

	#if CC_SCPOS_VSYNC_WA2
	_u4UpdateDisplayModeState = 0;
	_fgDisplayModeEnable = FALSE;
	#endif

}

void vScpipInit(UINT8 bResume)
#else
void vScpipInit(void)
#endif  //CC_FAST_INIT
{
    vFrameTrackConfig(SCPIP_FRAME_TRACK_LOCK_RANGE, SCPIP_FRAME_TRACK_LOSS_LOCK_RANGE, SCPIP_FRAME_TRACK_STEP_SMALL);//( u4LockRange,  u4LossLockRange,  u4Step) 
    #ifdef SUPPORT_SW_FRAME_CHECK
    vSetSWFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
    #endif
    
    vScpipSetDumpCtrl(SV_OFF);

    #ifdef CC_FAST_INIT 
    if(bResume==0)  //0:normal, 1:resume, don't creat semaphore again when system resume
    {
	    if (x_sema_create(&h_dramUpdateSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
	    {
	        ASSERT(0);
	    }
    }    
    #else
    if (x_sema_create(&h_dramUpdateSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ASSERT(0);
    }
    #endif//CC_FAST_INIT

    #ifdef CC_SCPOS_3DTV_SUPPORT
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_SCALER, (vTDTVModeChangeCB)vScpipTv3dModeSwitch);
    vScpipTv3dClrFlg(VDP_1, SCPIP_TV3D_ALL);
    vScpipTv3dClrFlg(VDP_2, SCPIP_TV3D_ALL);
    #if defined(CC_MT5395)
    if((LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_RLRL) && (BSP_GetIcVersion() == IC_VER_5395_AA))
    {
        vRegWriteFldAlign(SCPIP_PIP1_0D, 1, PIP1_0D_TV3D_IN_1ST_1);
    }     
    #endif
    #endif
    
	//CR[95333] when open write and dram burst mode on ,PIP2_08_DATA_LENGTH_2 default value is 90 (when first enter pip mode).
	vRegWriteFldAlign(SCPIP_PIP2_08, 0, PIP2_08_DATA_LENGTH_2);
    #ifdef CC_FAST_INIT 
    vScpipResume();
    #endif

}


static void _vScpipConfigInputClk(UINT8 bPath)
{

    UINT32 u4VdpId = getScalerVDP(bPath);    
    UINT8 bMode = u1ScpipGetDispMode(bPath);
    DI_OUTPUT_CLOCK_TYPE eDiOClk;
    SCPIP_CLOCK_TYPE eScpipClk;
    UINT16 u2PSDivClkM;
    UINT16 u2PSDivClkN;

    u2PSDivClkM = 0;
    u2PSDivClkN = 0;

    if (_au4ForcePsDivClkEn[u4VdpId] && _au4ForcePsDivClkM[u4VdpId] && _au4ForcePsDivClkN[u4VdpId])
    {
        u2PSDivClkM = (UINT16)_au4ForcePsDivClkM[u4VdpId];
        u2PSDivClkN = (UINT16)_au4ForcePsDivClkN[u4VdpId];
        eDiOClk = DI_CLK_TYPE_PS_DIG_DIV_CK;
        eScpipClk = SCPIP_CLK_TYPE_PS_DIV_CLK;      

        LOG(1, "Force Scpip Input Clock to PS_DIV_CLK (%d/%d)\n", u2PSDivClkM, u2PSDivClkN);
    }
    else if (bMode == VDP_SCPOS_PSCAN_DISPMODE)  //pscan dispmode 
    {
            eDiOClk = DI_CLK_TYPE_VDOIN;
            eScpipClk = SCPIP_CLK_TYPE_VDOIN;
    }
    else
    {
        eDiOClk = DI_CLK_TYPE_VDOIN;
        eScpipClk = SCPIP_CLK_TYPE_VDOIN;
    }

    if (u2PSDivClkM && u2PSDivClkN)
    {
        vDrvDISetClkDiv(bPath, u2PSDivClkM, u2PSDivClkN);
    }
    vDrvDISetPSCANClock(bPath, eDiOClk);
    
    ASSERT(bPath != SV_VP_PIP); // sub pscan display mode has been removed    
    vScpipSetClock(SV_VP_MAIN, eScpipClk);

}


void vScpipToggleForceOff(UINT32 u4VdpId)
{
    CRIT_STATE_T csState; 
    LOG(3, "vDrvScpipForceWENOff: Disable (%d) Write Relatch\n", u4VdpId); 
    if (u4VdpId == VDP_1)
    {          
        csState = x_crit_start(); 
        _u4ResetScposDramBuffer[VDP_1] = (_u4ResetScposDramBuffer[VDP_1] < 1)? 1: _u4ResetScposDramBuffer[VDP_1];
        vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_WEN_RLH_1);
        x_crit_end(csState);
        vScpipWriteCtrlOnOff(VDP_1, SV_OFF);            
    }
    else
    {
        csState = x_crit_start(); 
        _u4ResetScposDramBuffer[VDP_2] = (_u4ResetScposDramBuffer[VDP_2] < 1)? 1: _u4ResetScposDramBuffer[VDP_2];
        vRegWriteFldAlign(SCPIP_PIP2_09, 0, PIP2_09_WEN_RLH_2);
        x_crit_end(csState);
        vScpipWriteCtrlOnOff(VDP_2, SV_OFF);            
    }
}


/**
* @brief Set mode change flag
* @param ucVdpId VDP_1/VDP_2
*/
void vScpipReconfig(UCHAR ucVdpId)
{
    if (ucVdpId == VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else
    if (ucVdpId == VDP_2)
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }
}


/**
* @brief Get pre-scaling down width
* @param bPath VDP_1/VDP_2
* @return pre-scaling down width
*/
UINT32 u4ScpipGetPreScaleWidth(UINT8 bPath)
{
    UINT16 u2Width;
    UINT32 u4Factor;

    if (bPath == VDP_1) // Main
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
    else
    {
        u4Factor = RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }
    u2Width = DIVIDE_WITH_CARRIER(((UINT32)wDrvVideoInputWidth(bPath) * u4Factor), DFT_DS_FACTOR);
    return u2Width;
}


/**
* @brief Handle dynamic scaling
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipHandleDynamicScaling(UINT32 u4VdpId)
{
//    RVChannel* prChannel;
//    prChannel = ((u4VdpId == VDP_1) ? &_rMChannel : &_rPChannel);
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
    if ((_arScalePrm[u4VdpId].u4DynamicScale != 0) &&
        (_pChannel[u4VdpId]->bIsChannelOn != SV_OFF) &&
        (!_pChannel[u4VdpId]->bIsFreeze) &&
        (_arDynamicPrm[u4VdpId].u4State != VDP_DYNAMICE_SCALE_IDLE))
    {
        if (++_arDynamicPrm[u4VdpId].u4VsyncCount >= VDP_SCPOS_RESCALE_TIME)
        {
            _arDynamicPrm[u4VdpId].u4VsyncCount = 0;
            vSetScposFlg(((u4VdpId == VDP_1) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG));
        }
    }
}


/**
* @brief Handle channel freeze
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipHandleChannelFreeze(UINT32 u4VdpId)
{
   // RVChannel* prChannel;
   ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);
  //  prChannel = ((u4VdpId == VDP_1) ? &_rMChannel : &_rPChannel);
    if ( _pChannel[u4VdpId]->bIsGoingToFreeze && (_pChannel[u4VdpId]->bIsChannelOn == SV_ON) && (_arScalePrm[u4VdpId].u4Enable))
    {    // wait for some VSync to fill the whole scaler frame buffers before freezing
        UINT8 bMute;
        LOG(3, "Channel is going to freeze: %d\n", _pChannel[u4VdpId]->bIsGoingToFreeze);
        bMute =  ((u4VdpId == VDP_1) ? fgMainMuteOn() : fgPIPMuteOn());

        if (bMute)
        {
            LOG(3, "Channel is still in mute mode\n");
        }
        else
        {
            _pChannel[u4VdpId]->bIsGoingToFreeze --;
            if (_pChannel[u4VdpId]->bIsGoingToFreeze == 0)
            {
                LOG(3, "Channel freeze on\n");
                _pChannel[u4VdpId]->bIsFreeze = SV_ON;
            }
        }
    }    
}


/**
* @brief Configure scaling mode
*/
static void _vScpipConfigScalingMode(UINT32 u4VdpId)
{
    //static UINT8 _au1ExtFifo[VDP_NS] = {0xf, 0xf};
    static UINT8 _au1IntFifo[VDP_NS] = {0xf, 0xf};

    UINT8 bMode = 0; // for main path only
    UINT8 bTap = 0;  // for main path only
    UINT8 bVUSBypass = SV_OFF;
    UINT8 bVDSBypass = SV_OFF;
    UINT8 bUseIntFifo = SV_ON;
    UINT8 bPath = getScalerPath(u4VdpId);
    UINT8 bDispMode = u1ScpipGetDispMode(bPath);
    UINT8 pipOffset = getPIPOffset(bPath);
    ARRAY_BOUND_CHECK(bPath, SV_VP_NA);
    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }

    if (bPath == SV_VP_MAIN)
    {
        if (bDispMode == VDP_SCPOS_DISPMODE_AUTO || bDispMode == VDP_SCPOS_DISPMODE_MANUAL) //scaler dispmode
        {
            #if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_LARGE);
            #endif

            if(_pDispPrm[u4VdpId]->u4USclV==DFT_US_FACTOR)   //down-scaling
            {
                bMode = SCALER_DISPMODE_DOWN_SCALING_MODE;            
                bVUSBypass = SV_ON;
            }
            else    //up-scaling
            {
                bMode = SCALER_DISPMODE_UP_SCALING_MODE;
                bVDSBypass = SV_ON;
            }
            bTap = UPSCALER_4TAP;
        }
        else if (bDispMode == VDP_SCPOS_PSCAN_DISPMODE)  //pscan dispmode 
        {
            #if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
            #endif
            if (_u1ScpipGetPSCANDispmodeScalingMode(bPath) == UP_SCALING_MODE)   //up-scaling
            {
                bVDSBypass = SV_ON;  
                if(u1ScpipGet444Mode(bPath))
                {
                    bTap = UPSCALER_4TAP;
                    bMode = PSCAN_DISPMODE_UP_4TAP; 
                }
                else
                {
                    bTap = UPSCALER_6TAP;
                    bMode = PSCAN_DISPMODE_UP_6TAP;
                }
            }
            else 
            {
                bMode = PSCAN_DISPMODE_DOWN;   
                bVUSBypass = SV_ON;  
                bTap = UPSCALER_4TAP;
            }
        }
        else //scalar DRAM mode
        {
            #if CC_SPEED_UP_DISPMODE_TUNE
            vScpipFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
            #endif

            if (_pDispPrm[u4VdpId]->u4USclV==DFT_US_FACTOR)   //down-scaling
            {
                bMode = DRAM_MODE_BYPASS_UPSCALER;
                bVUSBypass = SV_ON; 
                bTap = UPSCALER_4TAP;
                bUseIntFifo = SV_ON;                
            }
            else // up scaling
            {
                if(u1ScpipGet444Mode(bPath))
                {
                    bMode = DRAM_MODE_4_TAPS;
                    bTap = UPSCALER_4TAP;
                }
                else
                {
                    bMode = DRAM_MODE_6_TAPS;
                    bTap = UPSCALER_6TAP;
                }
                bVDSBypass = SV_ON;
            }

            #ifdef CC_SCPOS_3DTV_SUPPORT 
            if(fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_INPUT_IS_3D) 
                && (fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_SPLIT) 
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_SBS_Q_I_SPLIT)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                    || fgIsScpipTv3dFlgSet(u4VdpId, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)))
            {
                bMode = DRAM_MODE_4_TAPS;
                bTap = UPSCALER_4TAP;
                bVDSBypass = SV_OFF;
                bVUSBypass = SV_OFF; 
            }
            #endif        
        }
        LOG(3, "ConfigScalingMode(%d), DispMode(%d), VDS_Bypass(%d), VUS_Bypass(%d), UseIntFifo(%d), Tap(%d)\n", 
            u4VdpId, bDispMode, bVDSBypass, bVUSBypass, bUseIntFifo, bTap);

    }
    else  // sub path    
    {
        ASSERT(bDispMode == VDP_SCPOS_DISPMODE_OFF); // sub path only dram mode

        //scalar DRAM mode
        if (_pDispPrm[u4VdpId]->u4USclV==DFT_US_FACTOR)   //down-scaling
        {
            bVUSBypass = SV_ON; 
        }
        else // up scaling
        {
            bVDSBypass = SV_ON;
        }        

        LOG(3, "ConfigScalingMode(%d), DispMode(%d), VDS_Bypass(%d), VUS_Bypass(%d), UseIntFifo(%d)\n", 
            u4VdpId, bDispMode, bVDSBypass, bVUSBypass, bUseIntFifo);
    }

    if (_arScalePrm[u4VdpId].u4ForceBypass)
    {
        bVUSBypass = SV_ON;
        bVDSBypass = SV_ON;
        if (bPath == SV_VP_MAIN)
        {
            bMode = DRAM_MODE_BYPASS_UPSCALER;
        }
    }
    

    if (_au1IntFifo[bPath] != bUseIntFifo)
    {
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_FIFO, u4VdpId, 5, TRUE);
    }
    //_au1ExtFifo[bPath] = bUseExtFifo;
    _au1IntFifo[bPath] = bUseIntFifo;


    // 5365/5395 new setting
    // set REG_VTAP_SEL_DOMAIN & REG_VUS_BYPASS_SEL_DOMAIN
    vRegWriteFldAlign(SCPIP_PIP1_04 + pipOffset, bVUSBypass, PIP1_04_REG_VTAP_SEL_DOMAIN_1);
    vRegWriteFldAlign(SCPIP_PIP1_04 + pipOffset, bVUSBypass, PIP1_04_REG_VUS_BYPASS_SEL_1);
    vRegWriteFldAlign(SCPIP_PIP1_12 + pipOffset, bVUSBypass, PIP1_12_REG_BOUND_EN_SEL);

    //set UP-SCALER BYPASS 
    _vScpipSetBypassUpScaler(bPath, bVUSBypass);
    _vScpipSetBypassDownScaler(bPath, bVDSBypass);    
    
    // Set DRAM r/w fifo
    // 5365/5395 new setting
    _vScpipSetDRAMFifo(bPath, SV_OFF, bUseIntFifo);
    if (bPath == SV_VP_MAIN)
    {
        vDrvSetUpScalerVTap(bPath, bTap);
        _vScpipSetDEAlignmentReg(bMode);            
    }
    else // sub path
    {
        #if defined(TVE_WA2) && (defined(CC_MT5363) || defined(CC_TVE6595))
        UINT8 ucEnable;
        TVE_GetEnable(TVE_1, &ucEnable);
        // enlarge v-porch 33 line
        if (ucEnable && bDrvVideoIsSrcInterlace(bPath) && ((_pPicInfo[bPath]->wSrcH == 480) || (_pPicInfo[bPath]->wSrcH == 576)))
        {
            bMode = SCALER_SUB_TVE_WA_CONFIG;
            LOG(3, "SCALER_SUB_TVE_WA_CONFIG\n");
        }
        else
        {
            bMode = SCALER_SUB_NORMAL_CONFIG;
            LOG(3, "SCALER_SUB_NORMAL_CONFIG\n");
        }
        _vScpipSetDEAlignmentReg(bMode);
        #endif
    }
}


/**
* @brief Set DE alignment related registers
* @param index bypass status
*/
static void _vScpipSetDEAlignmentReg(UINT8 index)
{
    UINT8 u1MJCFlipMirrorWA = 0;
    #if defined(CC_FLIP_MIRROR_SUPPORT) 
    if ((BSP_GetIcVersion() == IC_VER_5395_AA) && (u4GetFlipMirrorModule(VDP_1)==FLIP_BY_MJC))
    {
        u1MJCFlipMirrorWA = 3;
    }
    #endif

    
    // turn off auto up adjust in scaler display mode
    #if 0
    if((index == SCALER_DISPMODE_DOWN_SCALING_MODE) || (index == SCALER_DISPMODE_UP_SCALING_MODE))
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 0, PIP_OA_01_AUTO_UP_ADJUST);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_01, 1, PIP_OA_01_AUTO_UP_ADJUST);
    }
    #endif
    vRegWriteFldAlign(SCPIP_PIP_OA_01,  0, PIP_OA_01_AUTO_UP_ADJUST);
    vRegWriteFldAlign(SCPIP_PIP1_0B,    0, PIP1_0B_VS_EDGE_SEL_1);
    vRegWriteFldAlign(SCPIP_US1_04,     0, US1_04_VS_DATA_FH_1);
    vRegWriteFldAlign(SCPIP_PIP1_0A,    0, PIP1_0A_VS_DATA_THP_1);                        
    #ifdef CC_MT5395
    // for CBE module extra line delay    
    vRegWriteFldAlign(OUTSTG_OS_75,     6, OS_75_VSYNC_DELSEL2);
    vRegWriteFldAlign(OUTSTG_OS_75,     5, OS_75_VSYNC_DELSEL);  

    if(u1MJCFlipMirrorWA != 0)
    {
        vDrvMJCSetVsyncDelay(0, 6); // apply work around
    }
    else
    {
        vDrvMJCSetVsyncDelay(5, 6); // no work around
    }
    #else
    vRegWriteFldAlign(OUTSTG_OS_75,     5, OS_75_VSYNC_DELSEL2);
    vRegWriteFldAlign(OUTSTG_OS_75,     4, OS_75_VSYNC_DELSEL);
    #endif

    
    switch(index)
    {
        case SCALER_DISPMODE_DOWN_SCALING_MODE :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_US_VOFST_BYPASS1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3 + u1MJCFlipMirrorWA, PIP_OA_03_PRE_VOFST_BYPASS1);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  0, PIP_OA_02_VSOUT_SEL);
        break;
        
        case SCALER_DISPMODE_UP_SCALING_MODE :
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2 + u1MJCFlipMirrorWA, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  0, PIP_OA_02_VSOUT_SEL);
        break;        
        
        case DRAM_MODE_4_TAPS :
        case PSCAN_DISPMODE_UP_4TAP:    
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  2, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  4 + u1MJCFlipMirrorWA, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
        break;        

        case DRAM_MODE_6_TAPS :
        case PSCAN_DISPMODE_UP_6TAP:    
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3, PIP_OA_03_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  3 + u1MJCFlipMirrorWA, PIP_OA_03_PRE_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
        break;
        
        case DRAM_MODE_BYPASS_UPSCALER:
        case PSCAN_DISPMODE_DOWN:
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  1, PIP_OA_03_US_VOFST_BYPASS1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03,  5 + u1MJCFlipMirrorWA, PIP_OA_03_PRE_VOFST_BYPASS1);            
            vRegWriteFldAlign(SCPIP_PIP_OA_02,  2, PIP_OA_02_VSOUT_SEL);
        break;

        #ifdef TVE_WA2
        case SCALER_SUB_NORMAL_CONFIG:
            #ifdef CC_MT5395
            {                
                UINT8 ucEnable;
                TVE_GetEnable(TVE_1, &ucEnable);
                if(ucEnable)
                    vRegWriteFldAlign(SCPIP_PIP_OA_03, 7, PIP_OA_03_PRE_VOFST2_LSB);            
                else
                    vRegWriteFldAlign(SCPIP_PIP_OA_03, 8, PIP_OA_03_PRE_VOFST2_LSB);            
            }
            #else
            vRegWriteFldAlign(SCPIP_PIP_OA_03, 7, PIP_OA_03_PRE_VOFST2_LSB);            
            #endif
            vRegWriteFldAlign(SCPIP_PIP_OA_10, 0, PIP_OA_10_PRE_VOFST2_MSB);        
            break;
        case SCALER_SUB_TVE_WA_CONFIG:
            // enlarge v-porch 33 line
            vRegWriteFldAlign(SCPIP_PIP_OA_03, 8, PIP_OA_03_PRE_VOFST2_LSB);
            vRegWriteFldAlign(SCPIP_PIP_OA_10, 2, PIP_OA_10_PRE_VOFST2_MSB);
            break;
        #endif    
        default:
        break;                
    }
}


/**
* @brief Get pre-scaling down factor
* @param bPath VDP_1/VDP_2
* @return pre-scaling down factor
*/
UINT32 u4ScpipGetHPS(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    }
    else
    {
        return RegReadFldAlign(SCPIP_HDS_01, SCPIP_HDS_PIP_SCALER);
    }
}


/**
* @brief Set scaler dispmode
* @param bPath VDP_1/VDP_2
* @param bMode mode type
*/
void vScpipSetScalerDispmode(UINT8 bPath,UINT8 bMode)
{
    //only main will enter dispmode
    if(bPath == SV_VP_PIP)
    {
        return;
    }

    #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
    vRegWriteFldAlign(SCPIP_PIP1_0A, 0x8, PIP1_0A_TUNE_STEP_1);
    #else    
    if(_rMPicInfo.wHTotal * _rMPicInfo.wVTotal * bDrvVideoGetRefreshRate(SV_VP_MAIN) >100000000)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0xF, PIP_OA_08_DFIFO_TUNE_STEP_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0x10, PIP_OA_08_DFIFO_TUNE_STEP_1);
    }
    #endif
    
    if(bMode == SV_ON)
    {
        #if CC_SPEED_UP_DISPMODE_TUNE
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DISP_TUNE, SV_VP_MAIN, 60, TRUE);
        #endif
        /* set DDDS and MOVLEN,MOHLEN,... */
        vDrvSetDynamicDisplay(SV_SUCCESS);                
        /* turn ON disp mode */
        vSwitchDispMode(SV_ON);
        /* Set tune parameter */
        _vScpipSetDispModeReg();
        /* set disp mode tuning register */
        #if !CC_NEW_SCALAR_DISPMODE_AUTOTUNE // 5387, use new auto tune
        vRegWriteFldAlign(SCPIP_PIP1_0B, 1, PIP1_0B_RESET_AUTO_TUNE_1);
        vRegWriteFldAlign(SCPIP_PIP1_0B, 0, PIP1_0B_AUTO_TUNE_EN_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP_OA_08, 0, PIP_OA_08_DFIFO_AUTO_TUNE1);       
        #endif

        vScpipSetDispModeFreeRunCount(CC_SCPOS_FREERUN_MAX);
        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_DISPMODE_AUTO;
    }
    else if(bMode == SV_OFF)
    {
        /* set dispmode tuning register to default value */
        /* turn OFF disp mode */
        vSwitchDispMode(SV_OFF);
        vDrvSetDynamicDisplay(SV_FAIL);                
        vScpipDispmodeFreeRun(SV_OFF);
        vScpipSetDispModeFreeRunCount(0);
    }
}


/**
* @brief Check if Scaler dispmode can be supported or not
* @param bPath VDP_1/VDP_2
*/
static UINT8 _u1ScpipIsScalerDispmodeSupported(UINT8 bPath)
{
    if(bDrvVideoSignalStatus(bPath) == SV_VDO_STABLE &&
       bDrvDispSupportFlag(u2ScpipCalculateOutputVTotal()) == SV_SUCCESS)
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

/**
* @brief For pscan to get overscan information. Especially in pscan dispmode.
* @param bPath VDP_1/VDP_2
* @param clipInfo structure to describe clip dimension.
* @return status
*/
UINT8 u1ScpipGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo)
{
    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT8 bMode;

 //   if (!u1ScpipModeChangeDone(bPath)) return SV_FAIL;

    picInfo = getPicInfo(bPath);
    u4Factor = u4ScpipGetHPS(bPath);
    bMode = u1ScpipGetDispMode(bPath);
        
    if(picInfo->wTargetH ==0 || picInfo->wTargetW==0 || u4Factor == 0)
    {
        return SV_FAIL;
    }


    if((bMode!= VDP_SCPOS_PSCAN_DISPMODE)
#ifdef SUPPORT_DRAM_DUMP
        && (!fgDisplayModeDump)
#endif
      )
    {
        //If not in pscan dispmode, pscan should not do clip
        clipInfo->wX = 0;
        clipInfo->wWidth = u4ScpipGetPreScaleWidth(bPath);
        clipInfo->wY = 0;
        clipInfo->wHeight = wDrvVideoInputHeight(bPath);
        
        #ifdef CC_SCALER_LR_SHIFT_WA
        if(bPath == SV_VP_MAIN)
        {
            TV3D_STATUS_T r3dStatus;
            vTV3DGetStatus(bPath, &r3dStatus);
            
            if(r3dStatus.u13DEnable && !r3dStatus.u1Force2D)          
            {
                if (IS_LVDS_DISP_3D_POLARIZED)
                {
                    clipInfo->wHeight+=1;
                }
            }
        }
        #endif
        #if defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5395)
        if(fgIsScpipTv3dFlgSet(bPath, SCPIP_TV3D_INPUT_IS_3D))
        {
            if(fgIsScpipTv3dFlgSet(bPath, SCPIP_TV3D_3D_TO_2D_SBS)
                || fgIsScpipTv3dFlgSet(bPath, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                || fgIsScpipTv3dFlgSet(bPath, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
            {
                clipInfo->wWidth >>= 1;
            }
            else if (fgIsScpipTv3dFlgSet(bPath, SCPIP_TV3D_3D_TO_2D_TNB))
            {
                clipInfo->wHeight >>= 1;
            }
        }
        #endif
        #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))        
        if((SV_TRUE == _bLRInverse) && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
        {
            clipInfo->wX = 0;
            clipInfo->wWidth = wDrvVideoInputWidth(bPath);
            clipInfo->wY = 0;
            clipInfo->wHeight = wDrvVideoInputHeight(bPath);  
            
            clipInfo->wWidth >>= 1;
            if(SV_VP_MAIN == bPath)
            {               
                clipInfo->wX = clipInfo->wWidth;
            }
        }
        #endif               
    }
    else    //pscan dispmode
    {
		#if !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP
		UINT32 DI_wx = 0;       // check this change
		UINT32 RightPorch = 0;  // if overscan mostly occure in right region! 2010.10.28
		UINT32 u4MixedHPorch;		
		
        u4MixedHPorch = (UINT32)getMixedHPorch(bPath);
    	DI_wx = (UINT32)(u4MixedHPorch * u4Factor / DFT_HPD_FACTOR);
        RightPorch = (UINT32)((picInfo->wSrcW - u4MixedHPorch - picInfo->wTargetW) * u4Factor / DFT_HPD_FACTOR);
		
		#if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP 		
		if(DI_wx < SCPIP_HCLIP_LENGTH && RightPorch < SCPIP_HCLIP_LENGTH)  /*if overscan small-scale in both region*/   
		{
			clipInfo->wX = 0;
			clipInfo->wWidth = u4ScpipGetPreScaleWidth(bPath);
		}
        else if(DI_wx > SCPIP_HCLIP_LENGTH && RightPorch < SCPIP_HCLIP_LENGTH)  /*if overscan large-scale in left region*/
		{
            clipInfo->wX = DI_wx  - SCPIP_HCLIP_LENGTH;
            clipInfo->wWidth = DIVIDE_WITH_CARRIER((((UINT32)picInfo->wTargetW) * u4Factor),DFT_HPD_FACTOR) + SCPIP_HCLIP_LENGTH+RightPorch;
		}	
        else if(DI_wx < SCPIP_HCLIP_LENGTH && RightPorch > SCPIP_HCLIP_LENGTH)   /*if overscan large-scale in right region*/
        {
            clipInfo->wX = 0;
            clipInfo->wWidth = DIVIDE_WITH_CARRIER((((UINT32)picInfo->wTargetW) * u4Factor),DFT_HPD_FACTOR) + SCPIP_HCLIP_LENGTH+DI_wx;
        }
		else		
		{
			clipInfo->wX = DI_wx  - SCPIP_HCLIP_LENGTH;
			clipInfo->wWidth = DIVIDE_WITH_CARRIER((((UINT32)picInfo->wTargetW) * u4Factor),DFT_HPD_FACTOR) + 2*SCPIP_HCLIP_LENGTH;
		}			 
		#else
        clipInfo->wX = DI_wx;
        clipInfo->wWidth = DIVIDE_WITH_CARRIER(((UINT32)picInfo->wTargetW * u4Factor),DFT_HPD_FACTOR);   
		#endif
        if((clipInfo->wWidth % 2) && !u1ScpipGet444Mode(bPath))
        {
            if(clipInfo->wX ==0)
                clipInfo->wWidth -= 1;
            else
            {
                clipInfo->wX -=1;
                clipInfo->wWidth += 1;
            }
        }
		#else
        clipInfo->wX = 0;
        clipInfo->wWidth = u4ScpipGetPreScaleWidth(bPath);
        #endif
        
        clipInfo->wY = getMixedVPorch(bPath);        
        clipInfo->wHeight = picInfo->wTargetH;
    }

    #ifdef MDDI_WA1
    if (BSP_GetIcVersion() >= IC_VER_5387_AB) // After ECO
    {
        clipInfo->wWidth = vDrvDISetWAWidth(clipInfo->wWidth);
    }
    #endif
    
    return SV_SUCCESS;    
}

#if 0
/**
* @brief Check if scaler mode change done
* @param bPath VDP_1/VDP_2
* @return status
*/
UINT8 u1ScpipModeChangeDone(UINT8 bPath)
{    
    if (bPath == SV_VP_MAIN)
    {
        if (!fgApiVideoIsEvtFlgSet(VDO_FLG_MAIN_MODECHG_DONE)) return FALSE;
                   
        if (fgIsScposFlgSet(MAIN_HV_CHG|MAIN_DISP_PRM_CHG) || _bMainState == VDO_STATE_WAIT_MODE_DET)
        {
            return FALSE;
        }
        else
        {
            return TRUE;        
        }    
    }
    else
    {
        if (!fgApiVideoIsEvtFlgSet(VDO_FLG_PIP_MODECHG_DONE)) return FALSE;
    
        if (fgIsScposFlgSet(PIP_HV_CHG|PIP_DISP_PRM_CHG) || _bPipState == VDO_STATE_WAIT_MODE_DET)
        {
            return FALSE;
        }
        else
        {
            return TRUE;        
        }        
    }
}
#endif

/**
* @brief Get H clip width
* @param bPath VDP_1/VDP_2
* @return clip width
*/
static UINT16 _u2ScpipGetHClipWidth(UINT8 bPath)
{

#if !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP || CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
    
    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT16 HClipWidth;
	#if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
	UINT32 LeftPorch;
	UINT32 RightPorch;
	#endif

    //if not in pscan dispmode, just return pre-scale downed width
    if(u1ScpipGetDispMode(bPath)!= VDP_SCPOS_PSCAN_DISPMODE)
    {
        return u4ScpipGetPreScaleWidth(bPath);
    }

    //get input info
    picInfo = getPicInfo(bPath);
    //get pre-scale down factor
    u4Factor = u4ScpipGetHPS(bPath);
    #if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
	LeftPorch=getMixedHPorch(bPath) * u4Factor / DFT_HPD_FACTOR;
	RightPorch = (UINT32)((picInfo->wSrcW - getMixedHPorch(bPath) - picInfo->wTargetW) * u4Factor / DFT_HPD_FACTOR);
	if(LeftPorch < SCPIP_HCLIP_LENGTH && RightPorch < SCPIP_HCLIP_LENGTH)
	{
		HClipWidth = u4ScpipGetPreScaleWidth(bPath);
	}
	else if(LeftPorch < SCPIP_HCLIP_LENGTH && RightPorch > SCPIP_HCLIP_LENGTH)
	{
		HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR + SCPIP_HCLIP_LENGTH + LeftPorch;
	}	
	else if(LeftPorch > SCPIP_HCLIP_LENGTH && RightPorch < SCPIP_HCLIP_LENGTH)
	{
		HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR + SCPIP_HCLIP_LENGTH + RightPorch;
	}
	else //if(LeftPorch > SCPIP_HCLIP_LENGTH && RightPorch > SCPIP_HCLIP_LENGTH)
	{
		HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR + 2*SCPIP_HCLIP_LENGTH;
	}
	LOG(3, "-----> HClipWidth(%d)  LeftPorch(%d)  RightPorch(%d)\n", HClipWidth, LeftPorch, RightPorch);
	#else
    //get the width pscan will read and write after doing overscan in pscan module.
    HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR;
    #endif
    return HClipWidth;
#else
    return u4ScpipGetPreScaleWidth(bPath);
#endif
}

/**
* @brief Configure Pscan dispmode
* @param bPath VDP_1/VDP_2
* @param bScalingMode down/up/bypass
* @param bOnOff On/Off
*/
static void vScpipConfigPscanDispmode(UINT8 bPath,UINT8 bScalingMode,UINT8 bOnOff)
{
    UINT8 offset = getPIPOffset(bPath);
    UINT8 bmode = u1ScpipGetDispMode(bPath);

    if (bPath >= SV_VP_NA)
    {
        return;
    }

    //turn off pscan dispmode
    if(bOnOff == SV_OFF)
    {
        LOG(3, "VDP(%d) Turn off dispmode", (UINT32)bPath);
        
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_IN_1);
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_SYNC_SEL_1);
        
        if(bmode == VDP_SCPOS_DISPMODE_AUTO || bmode == VDP_SCPOS_DISPMODE_MANUAL)
        {
            //If current mode is scaler dispmode, to prevent FMODESEL_1 from keep toggling between 0 and 1.
            //Just keep it as original value.
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0B+offset, 0, PIP1_0B_FMODESEL_1);
        }
        
        vRegWriteFldAlign(SCPIP_DS1_02 + offset, 0, DS1_02_VSYNC_OPT_1);
        //switch pscan clock to video in
        _vScpipConfigInputClk(bPath);
    }
    else if(bOnOff == SV_ON)
    {        
        UINT32 u4VBlank;

        //set global variable to pscan dispmode
        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_PSCAN_DISPMODE;
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 0, PIP1_0C_PSCAN_IN_1);
        //switch pscan clock to video in
        _vScpipConfigInputClk(bPath);

        //PSCAN vsync prefetch
        u4VBlank = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_MOVLEN) + 1 - TV_HEIGHT;
        ASSERT((u4VBlank-5) >= 6); // if PRELEN is set to < 6 it must be failed
        _vScpipSetPscanPrelen(bPath,MIN((u4VBlank-5), 0x14));            
        
        if(bScalingMode == DOWN_SCALING_MODE)
        {
            LOG(3, "VDP(0) PSCAN Dispmode + DS");                
            _vScpipNewDSPscanDispmodeWaterCtrl();
        }
        else
        {
            LOG(3, "VDP(0) PSCAN Dispmode + US");                
            _vScpipNewUSPscanDispmodeWaterCtrl();
        }
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, 1, PIP1_0C_PSCAN_SYNC_SEL_1);

        //Write data to line buffer
        vRegWriteFldAlign(SCPIP_PIP1_0B+offset, 1, PIP1_0B_FMODESEL_1);                 
        //Horizontal pixel per line = pre-scale down size + pscan HBLANK       
        // For pscan display mode hlen update
        vRegWriteFldAlign(SCPIP_PIP1_0C+offset, _u4ScpipGetPscanDispmodeHLen(bPath), PIP1_0C_HLEN_PSCAN_1);        
        
        vRegWriteFldAlign(SCPIP_DS1_02 + offset, 1, DS1_02_VSYNC_OPT_1);
    }
}


static void _vScpipNewDSPscanDispmodeWaterCtrl()
{
    vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x70, PIP_OA_0A_WTHR31_DS);
    vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x60, PIP_OA_0A_WTHR21_DS);
    vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0x40, PIP_OA_0A_WTHR11_DS);

    vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x40, PIP_OA_0B_WTHR_INI1_DS);
    vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x40, PIP_OA_0B_WTOKEN21_DS);
    vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x60, PIP_OA_0B_WTOKEN11_DS);
    if((wDISPLAY_WIDTH == 1366) &&(_u1ScpipGetPSCANDispmodeScalingMode(SV_VP_MAIN)==DOWN_SCALING_MODE))
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x7f, PIP_OA_0B_WTOKEN01_DS);    //[DTV00081423]       
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0B, 0x80, PIP_OA_0B_WTOKEN01_DS);
    }
}

static void _vScpipNewUSPscanDispmodeWaterCtrl()
{
    UINT32 u4HLen; // = _rMDispPrm.wHLen;
    UINT32 u4WidthDS = _u2ScpipGetHClipWidth(VDP_1);
    UINT32 u4ClkMhz = (PANEL_GetPixelClk50Hz()/u2DrvGetInternalPCLKRatio())/1000000;
    
    if(u1ScpipGet444Mode(VDP_1)== SV_FALSE)     // 422 Mode
    {
        if (u4ScpipGet10BitMode(VDP_1))
        {       // 10 bit mode
            u4HLen = DIVIDE_WITH_CARRIER(u4WidthDS, 6);
        }
        else  // 8 bit mode
        {          
            // 120 bits pack
            u4HLen = DIVIDE_WITH_CARRIER(u4WidthDS, 7);
            if((!(u4WidthDS%7)) && (u4WidthDS%2))//when width can be mod by 7 and is odd, the color of last line will be wrong
            {
                u4HLen += 1;
            }
        }
    }
    else                                            // 444 Mode
    {
        if (u4ScpipGet10BitMode(VDP_1))
        {   // 10 bit mode
            u4HLen = RSHIFT_WITH_CARRIER(u4WidthDS, 2);
        }
        else
        {
            // 120 bits pack
            u4HLen = DIVIDE_WITH_CARRIER(u4WidthDS, 5);
        }
    }    

    
    vRegWriteFldAlign(SCPIP_PIP_OA_0C, (u4HLen - 1)>> 3, PIP_OA_0C_WTHR31_US);
    vRegWriteFldAlign(SCPIP_PIP_OA_0C, (u4HLen >> 3) >> 1, PIP_OA_0C_WTHR21_US);
    vRegWriteFldAlign(SCPIP_PIP_OA_0C, (u4HLen >> 3) >> 2, PIP_OA_0C_WTHR11_US);

    // set initial threshold to (1 line + (0.5*HL) / 8)
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x3C + ((u4HLen >> 1) >> 3) , PIP_OA_0D_WTHR_INI_US);
        
    if(u4ClkMhz <80)
    {
        // for input FHD, output WXGA V up scale fail
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x7f, PIP_OA_0D_WTOKEN21_US);
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x7f, PIP_OA_0D_WTOKEN11_US);
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x7f, PIP_OA_0D_WTOKEN01_US);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN21_US);
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN11_US);
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0x80, PIP_OA_0D_WTOKEN01_US);
    }
}



// For pscan display mode hlen update
void vScpipSetPscanDispmodeHLen(UINT8 bPath)
{
    UINT8 offset = getPIPOffset(bPath);
    vRegWriteFldAlign(SCPIP_PIP1_0C + offset, _u4ScpipGetPscanDispmodeHLen(bPath), PIP1_0C_HLEN_PSCAN_1);
}


/**
* @brief Get Pscan dispmode horizontal length (HLEN_PSCAN_1)
* @param bPath VDP_1/VDP_2
* @param horizontal length
*/
static UINT32 _u4ScpipGetPscanDispmodeHLen(UINT8 bPath)
{
    UINT32 ret, u4HMin;
    u4HMin=((_u2ScpipGetHClipWidth(bPath)+15)/16)*16 + u4DrvDIGetHBlank(bPath);
    ret = u4HMin;
    if(_u1ScpipGetPSCANDispmodeScalingMode(bPath)==UP_SCALING_MODE)
    {
        UINT32 u4ClkMhz = (PANEL_GetPixelClk50Hz()/u2DrvGetInternalPCLKRatio())/1000000;
        
        if(u4ClkMhz <80)
        {
            RDispPrm* dispPrm;
            dispPrm = getDispPrm(bPath);

            // PScan_HLEN =  HL *2.5T * input clk/ output clk;  for 444 8 bit pack
            // PScan_HLEN =  HL *2.0T * input clk/ output clk;  for 422 10 bit pack
#if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
			#ifdef CC_MT5395
			ret = (_u2ScpipGetHClipWidth(bPath)/5)*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;			
			#else
			ret = (_u2ScpipGetHClipWidth(bPath)/5)*((14336000)/(u4ClkMhz))/dispPrm->u4USclV;
			#endif
#elif !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP    
            #ifdef CC_MT5395
            //ret = _pDispPrm[bPath]->wHLen*((2.5*183*32768)/(u4ClkMhz))/dispPrm->u4USclV;
            ret = dispPrm->wHLen*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;            
            #else
            //ret = _pDispPrm[bPath]->wHLen*((2.5*175*32768)/(u4ClkMhz))/dispPrm->u4USclV;
            ret = dispPrm->wHLen*((14336000)/(u4ClkMhz))/dispPrm->u4USclV;
            #endif
#else
            #ifdef CC_MT5395
            ret = (_u2ScpipGetHClipWidth(bPath)/5)*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;            
            #else
            ret = (_u2ScpipGetHClipWidth(bPath)/5)*((14336000)/(u4ClkMhz))/dispPrm->u4USclV;
            #endif
#endif

        }
    }


    if(ret <= u4HMin)
    {
        return u4HMin;
    }
    else
    {
        return MIN(ret, MAX(HLEN_TOTAL_TYPI, u4HMin));         
    }
}

#if !CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
#if !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP
/**
* @brief Get Pscan dispmode h porch
* @param bPath VDP_1/VDP_2
* @return h porch
*/
static UINT32 _u4ScpipGetPscanDispmodeHPorch(UINT8 bPath)
{    
    return u4DrvDIGetProgDispmodeClip(getScalerVDP(bPath));    
}
#endif
#endif
/**
* @brief Set Pscan prelength
* @param bPath VDP_1/VDP_2
* @param u4PreLen
*/
static void _vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen)
{
    UINT8 pipOffset;
    pipOffset = getPIPOffset(bPath);
    vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, (u4PreLen & 0xff), PIP1_0C_VS_PSCAN_PRELEN_7_0_1);
    vRegWriteFldAlign(SCPIP_PIP1_0B+pipOffset, ((u4PreLen >>8) & 0x7), PIP1_0B_VS_PSCAN_PRELEN_10_8_1);
}

/**
* @brief get pscan display mode type
* @param bPath VDP_1/VDP_2
* @return bMode DOWN_SCALING_MODE/UP_SCALING_MODE
*/
static UINT8 _u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath)
{
    RDispPrm* dispPrm;
    UINT8 bMode;
    ARRAY_BOUND_CHECK(bPath, SV_VP_NA);    
    dispPrm = getDispPrm(bPath);
            
    if (dispPrm->u4DSclV != DFT_DS_FACTOR)
    {
        bMode = DOWN_SCALING_MODE;
    }
    else if(dispPrm->u4USclV != DFT_US_FACTOR)
    {
        bMode = UP_SCALING_MODE;
    }
    else  // dot by dot
    {
        bMode = UP_SCALING_MODE;
    }

    return bMode;    
}


/**
* @brief set pscan display mode on/off
* @param bPath VDP_1/VDP_2
* @param bMode what mode to set
*/
void vScpipSetPSCANDispmode(UINT8 bPath,UINT8 bMode)
{
    UINT8 bScalingMode;

    bScalingMode = _u1ScpipGetPSCANDispmodeScalingMode(bPath);
            
    vScpipConfigPscanDispmode(bPath, bScalingMode, bMode);    
}

static void _vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bmode)
{
    UINT8 bCfg;
    UINT8 bCLine = 0;
    UINT8 bLineNum;

    ASSERT(bPath == SV_VP_MAIN); // only main path support display mode
    
    if(bPath == SV_VP_MAIN)
    {
        if(bmode==VDP_SCPOS_DISPMODE_OFF)
        {
            bCfg = 0;
        }
        else if(bmode==VDP_SCPOS_PSCAN_DISPMODE) //pscan  display mode
        {   
            if(_u1ScpipGetPSCANDispmodeScalingMode(bPath) == UP_SCALING_MODE)
            {
                bCLine = 1;
            }
            else
            {
                bCLine = 0;
            }
            bCfg = 0;
        }
        else  // scaler display mode
        {
            bLineNum = 1440/_pDispPrm[SV_VP_MAIN]->wDataLen;
            if(bLineNum < 4)
            {
                bCfg = 0;
            }
            else if(bLineNum < 6)
            {
                bCfg = 1;
            }
            else if(bLineNum < 8)
            {
                bCfg = 2;
            }
            else 
            {
                bCfg = 3;
            }                
        }        
        vRegWriteFldAlign(SCPIP_PIP_OA_06, bCLine, PIP_OA_06_C_LINE1);             
        vRegWriteFldAlign(SCPIP_PIP_OA_06, bCfg, PIP_OA_06_CFG_LINE1); 
    }
}

#ifdef CC_FLIP_MIRROR_SUPPORT
/**
* @brief Set mirror flip mode
* @param bPath VDP_1/VDP_2
* @param bDramMode SV_TRUE/SV_FALSE 
*/
static void _vScpipConfigFlip(UINT8 bPath)
{
   // if(bDramMode ==SV_TRUE) // do flip only when dram mode
   if(u4GetFlipMirrorModule(bPath)==FLIP_BY_SCALER)
    {
        UINT8 u1Config;
        //UINT8 ucEnable;

        u1Config = u1GetFlipMirrorConfig();
        //TVE_GetEnable(TVE_1, &ucEnable);

        if((u1Config & SYS_MIRROR_CONFIG_ON)/* && !(ucEnable != SV_FALSE && bPath == SV_VP_PIP)*/)
        {
            vScpipSetMirror(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip(%d): MIRROR ON\n", bPath);
        }    
        else 
        {
            vScpipSetMirror(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip(%d): MIRROR OFF\n", bPath);
        }
        
        if((u1Config & SYS_FLIP_CONFIG_ON) /*&& !(ucEnable != SV_FALSE && bPath == SV_VP_PIP)*/)
        {
            vScpipSetFlip(bPath, SV_ON);
            LOG(4, "vScalerConfigFlip(%d): FLIP ON\n", bPath);
        }
        else
        {
            vScpipSetFlip(bPath, SV_OFF);
            LOG(4, "vScalerConfigFlip(%d): FLIP OFF\n", bPath);
        }
    }
    else
    {
        vScpipSetMirror(bPath, SV_OFF);
        vScpipSetFlip(bPath, SV_OFF);
        LOG(4, "vScalerConfigFlip(%d): Both FLIP and MIRROR OFF\n", bPath);
    }
    //_vScpipSetDramProtect();

    vScpipReconfig(bPath);
}
#endif


/**
* @brief Set scaler mode (DRAM/scaler dispmode/PSCAN dispmode)
* @param bPath VDP_1/VDP_2
*/
static void _vScpipModeSwitch(UINT8 bPath)
{
    UINT8 mode;

    //get scaler current mode by path
    mode = u1ScpipGetDispMode(bPath);
    //PIP path don't support Scaler dispmode
    if (bPath == SV_VP_PIP && 
       (mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL))
    {
        return;
    }

    _vScpipDispmodeLineCfg(bPath, mode);
                
    if (mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL) 
    {
        /* have to turn off PSCAN DISPMODE first
         * To test if scaler dispmode was supported or not, DS1_INVLEN_1 value is needed.
         * When scaler is in up-scaling pscan dispmode, DS1_INVLEN_1 won't be correct.
         * So before testing scaler dispmode, we have to turn off pscan dispmode first.
         */
        vRegWriteFldAlign(SCPIP_SYSTEM_01, 1, SYSTEM_01_SRAM_VDS_BYPASS_SEL); // turn on this bit only in scaler display mode
        vRegWriteFldAlign(SCPIP_PIP_OA_06, 0, PIP_OA_06_RFIFO_PRO1);   
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipWriteCtrlOnOff(bPath, SV_OFF);

        //if scaler dispmode can be supported 
        if (_u1ScpipIsScalerDispmodeSupported(bPath)==SV_TRUE)
        {
            vScpipSetScalerDispmode(bPath,SV_ON);
            #ifdef CC_SCPOS_DISPMODE_NOT_SUPPORT
            LOG(1,"VDP_DispStatusNotify: Scaler display mode support OK (1) \n");
   // if (fgIsMainFlgSet(MAIN_FLG_MODE_DET_DONE))
            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_STABLE);
            VDP_DispStatusNotify(SV_VP_MAIN, 1);
            #endif
        }
        else
        {
            vScpipSetScalerDispmode(bPath,SV_OFF);
            #ifdef CC_SCPOS_DISPMODE_NOT_SUPPORT
            LOG(1,"VDP_DispStatusNotify: Scaler display mode support fail (0) \n");
            vApiNotifyResolutionChg(SV_VP_MAIN, SV_VDO_NOSUPPORT);
            VDP_DispStatusNotify(SV_VP_MAIN, 0); 
            #endif

            SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_ZOOM_MODE, SV_FALSE, SV_FALSE);
            LOG(1, "VDP(%d) [DM] fail to enter scaler dispmode\n", bPath);
        }
    }
    else if (mode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_01, 1, SYSTEM_01_SRAM_VDS_BYPASS_SEL); // turn on this bit only in scaler display mode        
        vRegWriteFldAlign(SCPIP_PIP_OA_06, 0, PIP_OA_06_RFIFO_PRO1);           
        vScpipSetScalerDispmode(bPath,SV_OFF);
        vScpipWriteCtrlOnOff(bPath, SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_ON);
    }
    else if (mode == VDP_SCPOS_DISPMODE_OFF)
    {    
        vRegWriteFldAlign(SCPIP_SYSTEM_01, 0, SYSTEM_01_SRAM_VDS_BYPASS_SEL); // turn on this bit only in scaler display mode        
        vRegWriteFldAlign(SCPIP_PIP_OA_06, 1, PIP_OA_06_RFIFO_PRO1);           
        vScpipSetScalerDispmode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        //vScpipWriteCtrlOnOff(bPath, SV_ON);
        _arScalePrm[getScalerVDP(bPath)].u4DispModeEn = VDP_SCPOS_DISPMODE_OFF;
    }

#if SUPPORT_HDTV_HARDWARE_MUTE
    vScpipConfigHardwareMute();
#endif
    return;
}


/**
* @brief Set up scaler bypass
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode)
{
    UINT8 offset = getUSOffset(bPath);
    
    vRegWriteFldAlign(SCPIP_US1_01+offset, (bMode==SV_ON)?1:0, US1_01_VUS_BYPASS_1);
}

/**
* @brief Set down scaler bypass
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode)
{
    UINT8 offset = getDSOffset(bPath);
    
    vRegWriteFldAlign(SCPIP_DS1_02+offset, (bMode==SV_ON)?1:0, DS1_02_VDS_BYPASS_1);
}

/**
* @brief Set DRAM r/w fifo
* @param bPath SV_VP_MAIN/SV_VP_PIP
* @param bMode
*/
static void _vScpipSetDRAMFifo(UINT8 bPath, UINT8 bExtFifo, UINT8 bIntFifo)
{
    if (bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_06, (bIntFifo==SV_ON)?1:0, PIP_OA_06_USE_INTF_EN1);
        // no need to config USE_EXTF_EN for 5365/5395
        //vRegWriteFldAlign(SCPIP_PIP_OA_06, (bExtFifo==SV_ON)?1:0, PIP_OA_06_USE_EXTF_EN1);
    }
    else if (bPath == SV_VP_PIP)        
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_07, 1, PIP_OA_07_USE_INTF_EN2);
    }
}

// TODO: remove
#if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
/**
* @brief Inverse scale vsync
* @param ucVdpId VDP_1/VDP_2
*/
static void vWA13_SetVSyncInvert(UCHAR ucVdpId)
{
    UINT8 dsOffset = getDSOffset(getScalerPath(ucVdpId));

    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    
    if(_flagToInvertVSync[ucVdpId] == SV_TRUE)
    {
        if(_counterToInvertVSync[ucVdpId] !=0)
        {
            _counterToInvertVSync[ucVdpId]--;
        }
        if (_counterToInvertVSync[ucVdpId] == 1)
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNCP_1);
        }
        else if (_counterToInvertVSync[ucVdpId] == 0)
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNCP_1);
            _flagToInvertVSync[ucVdpId] = SV_FALSE;
        }
    }
}
#endif

/**
* @brief Check if video is freezable
* @param bPath VDP_1/VDP_2
*/
UINT8 u1ScpipIsVideoFreezable(UINT8 bPath)
{
    UINT8 scalerMode;
    UINT8 ret;
    
    ret = SV_TRUE;
    scalerMode = u1ScpipGetDispMode(bPath);

    // If scaler is in scaler dispmode or pscan dispmode, can't support freeze function.
    if(scalerMode == VDP_SCPOS_DISPMODE_AUTO || 
       scalerMode == VDP_SCPOS_DISPMODE_MANUAL ||
       scalerMode == VDP_SCPOS_PSCAN_DISPMODE)
    {       
        ret = SV_FALSE;
    }

    return ret;
}


/**
* @brief Update scaler parameters protected by import
* @param u4VdpId VDP_1/VDP_2
*/
static void _vScpipUpdateRegister(UINT32 u4VdpId)
{
    BOOL fgTrigger;
    UINT8 pipOffset = getPIPOffset(getScalerPath(u4VdpId));
    
    /* Set DS horizontal initial offset for Main channel. */
    _vScpipSetDSInitialFactor(u4VdpId);
    _vScpipSetDownScalingFactor(u4VdpId);
    if (u4VdpId == VDP_1)
    {
        _vScpipSetUSInitialFactor(VDP_1);
    }
    _vScpipSetUSReg(u4VdpId);                
    _vScpipSetDispReg(u4VdpId);
    
    #if defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5395)
    _vScpipSetTv3dReg(u4VdpId);
    #endif
    // toggle RLH_TRIGGER to make read domain register setting take effect for import mode (3)
    fgTrigger = RegReadFldAlign(SCPIP_PIP1_09 + pipOffset,  PIP1_09_RLH_TRIGGER_1);
    vRegWriteFldAlign(SCPIP_PIP1_09 + pipOffset, !fgTrigger, PIP1_09_RLH_TRIGGER_1);
}

/**
* @brief Scpos main path normal procedure
*/
static void _vScpipNormProcByPath(const UINT32 u4VdpId) 
{
    UINT8 pipOffset = getPIPOffset(getScalerPath(u4VdpId));
    BOOL fgIsMain = (u4VdpId == VDP_1);
    UINT8 bPath = getScalerPath(u4VdpId);
    UINT32 u4Update = 0;    
    BOOL fgChannelIsFreeze = (fgIsMain) ? (_rMChannel.bIsFreeze) : (_rPChannel.bIsFreeze);

    #if CC_SCPOS_FW_MUTE_WA
    if (u4ScpipGetMuteMask(u4VdpId) && (!u4ScpipGetHwMuteStatus(u4VdpId)))
    {
        return;
    }
    #endif    

    if(vScpipGetDumpCtrl()==SV_FALSE) //use NCSTool to access this register via 0xC000 in XDATA
    {        
        //if (RegReadFldAlign(SCPIP_PIP1_05+pipOffset, PIP1_05_WSTARTP_1) != 0) // srm not ready
        {
            _vScpipWriteCTRLCheck(u4VdpId);
        }
    }        

    #ifdef CC_SCPOS_3DTV_SUPPORT
    {
        RVChannel *prChannel = getChannel(u4VdpId);
        if (((_arScalePrm[u4VdpId].u4Enable == 1) &&
            (prChannel->bIsChannelOn == SV_ON) && 
            (prChannel->bIsFreeze == SV_OFF)))
        {
            UINT32 u4MuteDelay = _vDrvVideoGetMuteDelay(getScalerPath(u4VdpId), MUTE_MODULE_SCPOS_TV3D);
            if(u4MuteDelay >= 0xffff) // when mute delay is big, it's FOREVER_MUTE
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u4VdpId, _u4ScpipReCalDelayCounter(u4VdpId, VDP_SCPOS_TV3D_MUTE_DELAY), FALSE);
            }
        }
    }
    #endif
        
    if (fgIsMain) 
    {
        _vScpipDispmodeFreeRunChk();
        #if(CC_SCPOS_WA13_PSCAN_DISPMODE_1080I==1)
        vWA13_SetVSyncInvert(u4VdpId);
        #endif
    } 
    
    if((_u4ScposImportNotReady[u4VdpId] != 0) && (RegReadFldAlign(SCPIP_PIP1_09+pipOffset, PIP1_09_R_EN_1)==1))
    {
        _u4ScposImportNotReady[u4VdpId]--;
        return;  // import not ready, return from this function without any display parameter change          
    }
    else
    {
        _u4ScposImportNotReady[u4VdpId]=0;
    }
    
    //------------------------------------------------------
    // import ready, go on processing
    //------------------------------------------------------
    
    if(fgIsMain) 
    {
        u4ForceFrameTrackCtrl &= (~SCPIP_FRAME_TRACK_OFF_V_PORCH);
    }
        
    #if CC_SPEEDUP_IMPORT
    // clean import update flag
    _au4ImportUpdate[u4VdpId] = 0;
    #endif
    
    // Handle Dynamic Scaling
    _vScpipHandleDynamicScaling(u4VdpId);
    _vScpipHandleChannelFreeze(u4VdpId);

    if ((fgIsScposFlgSet((fgIsMain) ? (MAIN_DISP_PRM_CHG|MAIN_HV_CHG) : (PIP_DISP_PRM_CHG|PIP_HV_CHG)))
    #ifdef CC_QUEUE_EVENT_IN_FREEZE
            && (!fgChannelIsFreeze)
    #endif            
        )
    {        
        #ifdef __MODEL_slt__
        if (((fgIsMain) ? (_rMChannel.bIsChannelOn) : (_rPChannel.bIsChannelOn)) && (!fgChannelIsFreeze))                
        #endif
        {  
            //re-calculate scaling factor
            _vScpipCalNewScale(u4VdpId);
            if ((fgIsMain && (!fgMainWriteOn()) && _au4ScalingFactorChanged[VDP_1]))
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_MISC, VDP_1, VDP_SCPOS_DELAY_UNMUTE_TIME, FALSE);
                LOG(6, "MUTE_MODULE_SCPOS_MISC : scaling factor change when write off\n");
            }
            else if ((!fgIsMain && (!fgPIPWriteOn()) && _au4ScalingFactorChanged[VDP_2]))
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_MISC, VDP_2, VDP_SCPOS_DELAY_UNMUTE_TIME, FALSE);
                LOG(6, "MUTE_MODULE_SCPOS_MISC : scaling factor change when write off\n");
            }
            
            _vScpipUpdateDispPrm(u4VdpId);  // _VDP_vUpdateMainDispPrm();
            if(fgIsMain && (u1ScpipGetDispMode(u4VdpId)==VDP_SCPOS_PSCAN_DISPMODE))
            {
                if(_u1ScpipGetPSCANDispmodeScalingMode(u4VdpId) == DOWN_SCALING_MODE)                           
                {
                    LOG(3, "VDP PSCAN Dispmode + DS");                
                    _vScpipNewDSPscanDispmodeWaterCtrl();
                }
                else
                {
                    LOG(3, "VDP PSCAN Dispmode + US");                
                    _vScpipNewUSPscanDispmodeWaterCtrl();
                }
                vRegWriteFldAlign(SCPIP_PIP1_0C, _u4ScpipGetPscanDispmodeHLen(u4VdpId), PIP1_0C_HLEN_PSCAN_1);   
            }
            
            u4Update = 1;    
        }

        vClrScposFlg((fgIsMain) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG);
        vClrScposFlg((fgIsMain) ? MAIN_HV_CHG : PIP_HV_CHG);
    }            

    if (u4Update)
    {
        UINT8 bInputRefreshRate = bDrvVideoGetRefreshRate(u4VdpId);
        if((bInputRefreshRate <=30) && (bInputRefreshRate>0)) 
        {
            _u4ScposImportNotReady[u4VdpId] = (VDP_SCPOS_IMPORT_PROTECT * vDrvGetLCDFreq() + (bInputRefreshRate -1))/bInputRefreshRate;
        }
        else
        {
            _u4ScposImportNotReady[u4VdpId] = VDP_SCPOS_IMPORT_PROTECT;
        }

        vScpipImportOnOff(bPath, SV_OFF);       

        _vScpipUpdateRegister(u4VdpId);
        _vScpipConfigScalingMode(u4VdpId); 

        _vScpipUpdateDramParameter(u4VdpId, _u4BaseAddress[u4VdpId], PARAM_IGNORE, PARAM_IGNORE);
        #if 0
        if (bScpipFlipEnable[u4VdpId])
        {
            if(fgIsMain)
            {
                vDrvMainUpdateVBound(_pDispPrm[VDP_1]->wVLen);
            }                
        }
        #endif
        vScpipImportOnOff(bPath, SV_ON);

        #if CC_TWO_FRAME_LIMIT
        _vScpipSetTwoFrameLimit(u4VdpId);
        #endif
        
        #if CC_SPEEDUP_IMPORT
        LOG(3, "ImportUpdate(%d) (%d)\n", u4VdpId, _au4ImportUpdate[u4VdpId]);
        if ((_au4ImportUpdate[u4VdpId] & (SCPIP_UPDATE_W|SCPIP_UPDATE_R)) != (SCPIP_UPDATE_W|SCPIP_UPDATE_R))
        {
            LOG(3, "Speed up import\n");
            _u4ScposImportNotReady[u4VdpId] = VDP_SCPOS_SPEEDUP_IMPORT_PROTECT;
        }
        #endif 
    }
    
    // dram mode/scaler display mode/pscan display mode change handling
    // sub path operations are removed
    // update scaler mode 
    if (fgIsMain && fgIsScposFlgSet(SCALER_MAIN_MODE_CHG))
    {
        #if CC_SCPOS_FW_MUTE_WA
        if (u4ScpipGetMuteMask(VDP_1) && (!u4ScpipGetHwMuteStatus(VDP_1)))
        {
            return;
        }
        #endif
        _vScpipModeSwitch(SV_VP_MAIN);     
        //vSetScposFlg(MAIN_DISP_PRM_CHG);   
        vClrScposFlg(SCALER_MAIN_MODE_CHG);        
    }
}

/**
* @brief Scpos normal procedure
*/
void vScpipNormProc(void) 
{
    #ifdef SCPIP_TV3D_DEBUG
    static UINT8 u1Tv3dLR = 0;
    UINT8 u1CurrentLR = u1ScpipGetTv3dOutIndicator();
    LOG(9, "Current 3D TV LR is (%d)\n", u1CurrentLR);    
    if(u1CurrentLR == u1Tv3dLR)
    {        
        LOG(8, "Current 3D TV LR is repeated\n");
    }
    else
    {
        u1Tv3dLR = u1CurrentLR;
    }
    #endif
    
    vOstgNormProc();
    #ifndef CC_COPLAT_MT82
    #ifndef __MODEL_slt__
    OSD_PLA_Unmute();
    // temporily mark since the osd_dma is not ready, Tadd 2008/03/24
    //OSD_DMA_OnOutputVsync();
    #endif //__MODEL_slt__
    #endif //CC_COPLAT_MT82
    
    #ifdef DRV_SUPPORT_EXTMJC
    if(IS_COMPANION_CHIP_ON())
    {
        OSD_DMA_OnOutputVsync();
    }
    #endif   
    
    _vScpipNormProcByPath(VDP_1);    
    #if SUPPORT_POP
    _vScpipNormProcByPath(VDP_2);
    #endif
}


/**
* @brief Switch DE on/off when entering scaler dispmode
*/
// TODO: rename + extern in drv_scaler.h
void vSwitchDEOnOff(void)
{
#if CC_SCPOS_VSYNC_WA2
    if (LVDS_MUTE_IN_DISPLAY_MODE == LVDS_MUTE_IN_DISPLAY_MODE_ON)
    {
        if (_u4UpdateDisplayModeState == 1)
        {
            LVDS_SwitchDataEnable(SV_FALSE);
            _u4UpdateDisplayModeState = 2;
        }
        else if (_u4UpdateDisplayModeState == 2)
        {
            _VDP_DispModeEnable(VDP_1, _fgDisplayModeEnable);
            _u4UpdateDisplayModeState = 3;
        }
        else if (_u4UpdateDisplayModeState >= 3)
        {
            // check if ddds lock or lock timeout
            if (_u4UpdateDisplayModeState == 22)
            {
                LVDS_SwitchDataEnable(SV_TRUE);
                _u4UpdateDisplayModeState = 0; // initilaze
            }
            else
            {
                _u4UpdateDisplayModeState++;
            }
        }
    }
#endif
}


/**
* @brief Enable/Disable scaler dispmode
* @param fgEnable On/Off
*/
// TODO: local static + rename
void PMX_SetDisplayMode(BOOL fgEnable)
{
#if CC_SCPOS_VSYNC_WA2
    if (fgEnable)
    {
        _u4UpdateDisplayModeState = 1;
        _fgDisplayModeEnable = fgEnable;
    }
    else if (_fgDisplayModeEnable && (_u4UpdateDisplayModeState<3))
    {
        _u4UpdateDisplayModeState = 3;
        _fgDisplayModeEnable = fgEnable;
    }    
#endif
}

#ifdef CC_COPLAT_MT82
UINT8  _bBlankMuteCnt;

UINT8 bDrvIsScalerFlagCleared(UINT8 bPath)
{
    if(bPath == SV_VP_MAIN){
        if(fgIsScposFlgSet(MAIN_DISP_PRM_CHG | SCALER_MAIN_MODE_CHG))
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }
    }
    else
    {
        if(fgIsScposFlgSet(PIP_DISP_PRM_CHG | SCALER_PIP_MODE_CHG))
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }        
    }
    
}

void vDrvSetFastBlank(UINT8 bOnOff, UINT8 bCount)
{
    if(bOnOff == SV_ON && bCount > 0)
    {
        if(bCount > _bBlankMuteCnt)
        {
            _bBlankMuteCnt = bCount;
        }
        vRegWriteFldAlign(OUTSTG_OS_29, 1, OS_29_FAST_BLANK);
    }
    else
    {
        _bBlankMuteCnt = 0;
        vRegWriteFldAlign(OUTSTG_OS_29, 0, OS_29_FAST_BLANK);
    }
}

/**
 * @brief  to know if video can be freezed
 * @param  bPath path info.(SV_VP_MAIN/SV_VP_PIP)
 * @retval void
 */
UINT8 bIsVideoFreezable(UINT8 bPath)
{
    UINT8 mode;
    mode = u1ScpipGetDispMode(bPath);
    if((mode==VDP_SCPOS_DISPMODE_AUTO)||(mode==VDP_SCPOS_PSCAN_DISPMODE))
    {
        return SV_FALSE;
    }
    else
    {
        UINT8 bTiming = bDrvVideoGetTiming(bPath);
        //DTV00121401    The video can be freezed in PIP/POP sub window in HDMI/YPbPr 1080P source.
        if ((bTiming == MODE_1080p_50) || (bTiming == MODE_1080p_60))
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }
    }
}

#else  //#ifdef CC_COPLAT_MT82


#if 0
/**
* @brief Set frame delay
* @param u4VdpId VDP_1/VDP_2
* @return delay time
*/
UINT32 u4ScpipGetFrameDelay(UINT32 u4VdpId)
{
    UINT32 u4Delay;
    UINT32 u4RPos;
    UINT32 u4WPos;
    UINT8 bPath;
    UINT32 u4PipOffset;
    UINT32 u4FrameNum;
    
    bPath = getScalerPath(u4VdpId);
    u4PipOffset = getPIPOffset(bPath);
    
    if (u4VdpId >= VDP_NS)
    {
        return 0;
    }

    if (_arScalePrm[u4VdpId].u4DispModeEn)
    {
        u4Delay = 0;
    }
    else
    {
        u4FrameNum = RegReadFldAlign(SCPIP_PIP1_09+u4PipOffset, PIP1_09_FSEL_1);
        u4FrameNum += 1;
        
        u4WPos = RegReadFldAlign(SCPIP_PIP1_RO_00+u4PipOffset, PIP1_RO_00_W_POS_1);
        u4RPos = RegReadFldAlign(SCPIP_PIP1_RO_00+u4PipOffset, PIP1_RO_00_R_POS_1);

        if (u4RPos > u4WPos)
        {
            u4WPos += u4FrameNum;
        }

        u4Delay = u4WPos + 1 - u4RPos;    
    }

    return u4Delay;    
}
#else
UINT32 u4ScpipGetFrameDelay(UINT32 u4VdpId)
{
    UINT32 u4Delay = 0;
    UINT32 u4Mode = u1ScpipGetDispMode(getScalerPath(u4VdpId));
    
    if(u4Mode != VDP_SCPOS_DISPMODE_OFF)        // display mode on, there is no frame delay
    {
        u4Delay= 0;
    }
    else                                        // scaler dram mode
    {
        if(u4ScpipGetBufNum(u4VdpId) <= 2)      // 2 (or 1) frame mode
        {
            u4Delay= 2;  //delay 1 frame
        }
        else                                    // 3 frame mode
        {
            u4Delay= 3; // delay 1.5 frame
        }
    }

    return u4Delay;
}
#endif

/**
 *  scaler control call back
 *
 * @param void
 * @return void
 */
void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
    #ifdef CC_SCPOS_EN
    FBM_POOL_T* prPool;
    FBM_POOL_T* prMddiPool;
    #endif

    LOG(3, "[SRM] DISP CTRL vdp%d mode=0x%x, pds=%d, sram=%d\n", u4VdpId, u4Mode, u4PDSize, u4SramMode);

    #ifdef CC_SCPOS_EN
    
    if (u4VdpId == VDP_1)
    {
        if (u4Mode & FBM_POOL_MODE_UNSTABLE)
        {
            LOG(3, "[SRM] Unstable callback! Disable Dispmode!\n");
            _VDP_DispModeEnable((UINT8)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
        }
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);

    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_PIP, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }
    
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
        vScpipForceWriteOff(u4VdpId, SV_ON);    
        vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);
        _bSRMStable[u4VdpId]=SV_FALSE;
    }
    if ((prPool != NULL) && 
        (((u4Mode & FBM_POOL_MODE_UNSTABLE) == 0) || (u4Mode & FBM_POOL_MODE_SCPOS_MM_BUFCHG)))
    {
        UINT32 u4Address;
        UINT32 u4Width, u4Height, u4MddiWidth, u4FbmSize;
        

        u4Address = prPool->u4Addr;
        u4Width = prPool->u4Width;
        u4Height = prPool->u4Height;
        u4MddiWidth = prMddiPool->u4Width;
        u4FbmSize = prPool->u4Size;

        rScalerDramInfo.u4Addr = u4Address;
        rScalerDramInfo.u4Width = u4Width;
        rScalerDramInfo.u4Height = u4Height;

        if (prMddiPool->u4Mode & FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR)
        {
            rScalerDramInfo.u4MddiWidth = SCPOS_BYPASS_PSCANNR;
        }
        else
        {
            rScalerDramInfo.u4MddiWidth = u4MddiWidth;
        }
        
        _arScalePrm[u4VdpId].u4MddiDramWidth = rScalerDramInfo.u4MddiWidth;
        
        rScalerDramInfo.u4FbmSize = u4FbmSize;
        rScalerDramInfo.fg2FB = ((u4Mode & FBM_POOL_MODE_SCPOS_2FB) > 0)? 1: 0;
        rScalerDramInfo.fg10bit = ((u4Mode & FBM_POOL_MODE_10BIT) > 0)? 1: 0;
        rScalerDramInfo.fg8bit = ((u4Mode & FBM_POOL_MODE_8BIT) > 0)? 1: 0;

        LOG(3, "[SRM Callback]Set Scaler Buffer: a=0x%x, w=%d, h=%d, 2FB=%d, 10bit=%d, DIWidth=%d\n", 
            u4Address, u4Width, u4Height, (UINT32)rScalerDramInfo.fg2FB, (UINT32)rScalerDramInfo.fg10bit, rScalerDramInfo.u4MddiWidth);
        
        rScalerDramInfo.fg422 = ((u4Mode & FBM_POOL_MODE_422_FB) > 0)? 1: 0;
        #ifdef CC_FLIP_MIRROR_SUPPORT
    	_vScpipConfigFlip(u4VdpId);
    	#endif
        vScpipSrmSetBuf(u4VdpId,&rScalerDramInfo);
        _bSRMStable[u4VdpId]=SV_TRUE;
        vScpipForceWriteOff(u4VdpId, SV_OFF);
    }
    else
    {
        if (prPool == NULL)
        {
            LOG(1, "_VdpSrmScalerWeCb: fail to get pool\n");
        }
    }

    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
        return;
    }

    // u4Arg3 is pre-scale size
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
	#if 0
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
	#endif
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_AUTO);
	#if 0
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
	#endif
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_PSCAN_DISPMODE);
	#if 0
        if (bVideoUpdateSrcRegion(u4VdpId) != 1)
        {
        }
	#endif
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_MJC_DISP)
    {
        ASSERT(0); // no mjc display mode in 5365/5395        
    }
    else
    {
        ASSERT(0); // should never get in here!
    }

    vScpipReconfig((UCHAR)u4VdpId);
    #endif
}
#endif//#ifdef CC_COPLAT_MT82


/**
 * @brief vScpipSetFreeze
 
 * Freeze Video in scalar
 
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 */
void vScpipSetFreeze(UINT8 bPath, UINT8 bOnOff) 
{
    CRIT_STATE_T csState;
    csState = x_crit_start();
    // Use Main/Pip Memory Write Enable to Perform Freeze
    if (bPath == SV_VP_MAIN) 
    {
        #if (MAIN_FREEZE_DELAY)
        {
            if(bOnOff)
            {
                _rMChannel.bIsGoingToFreeze = MAIN_FREEZE_DELAY;
            }
            else
            {
                _rMChannel.bIsGoingToFreeze = 0;
                _rMChannel.bIsFreeze = 0;
            }            
        }
        #else
        {
            _rMChannel.bIsFreeze = bOnOff;
        }
        #endif
    } 
    else if (bPath == SV_VP_PIP) 
    {
        #if (PIP_FREEZE_DELAY)
        {
            if(bOnOff)
            {
                _rPChannel.bIsGoingToFreeze = PIP_FREEZE_DELAY;
            }
            else
            {
                _rPChannel.bIsGoingToFreeze = 0;
                _rPChannel.bIsFreeze = 0;
            }
        }
        #else
        {
            _rPChannel.bIsFreeze = bOnOff;
        }
        #endif
    }
    x_crit_end(csState);
}


void vScpipResetDramCountDown(UINT8 bPath)
{
    UINT8 pipOffset; 
    CRIT_STATE_T csState;
    ARRAY_BOUND_CHECK(bPath, SV_VP_NA);    
    pipOffset= getPIPOffset(getScalerPath(bPath));
    csState = x_crit_start();
    {    
        if ((_u4ResetScposDramBuffer[bPath]) && ((_u4ResetScposDramBuffer[bPath] & 0x80000000) == 0))
        {
            _u4ResetScposDramBuffer[bPath]--;
            if (_u4ResetScposDramBuffer[bPath] == 0)
            {
                vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 1, PIP1_09_WEN_RLH_1);    
                if (_au4DelayWriteEnable[bPath])
                {
                    _au4DelayWriteEnable[bPath] = 0;
                }
            }
        }    
    }    
    x_crit_end(csState);
}

#ifdef CC_SCPOS_3DTV_SUPPORT
static void _vScpipFullPRProc()
{
    static UINT8 u1State = 0;
    static UINT8 u1BaseRPos;  
    UINT8  u1CurWPos;
    UINT8  u1CurLR;    
    UINT32 u4RAddr;
    UINT8  u1RPos = 0;
    UINT8 u1FrameNum;
    CRIT_STATE_T csState;
    u1FrameNum = u4ScpipGetBufNum(VDP_1);

    if(_u1FprSetWPosCounter > 0)
    {
        _u1FprSetWPosCounter--;
    }
    else
    {
        vScpipSetWPos(VDP_1, SV_TRUE, 1);  // we need to wait 1 output vsync to set_w_pos to prevent
                                           // r_pos 0 --> 0 case
                                           
        // state machine initialization if state = 0
        if(u1State == 0) 
        {
            csState = x_crit_start();
            u1CurWPos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
            u1CurLR = RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_IN);
            x_crit_end(csState);
        
            if (u1CurLR == 0) // input is L
            {
                u1BaseRPos = (u1CurWPos + (u1FrameNum-2));        
            }
            else
            {
                u1BaseRPos = (u1CurWPos + (u1FrameNum-3));                    
            }
            if(u1BaseRPos >= u1FrameNum)
            {
                u1BaseRPos -= u1FrameNum; // u4BaseRPos %= u1FrameNum
            }
            u1BaseRPos <<= 1; // u1BaseRPos *= 2
            LOG(7, "u1CurWPos=%d, u1CurLR=%d, u1BaseRPos=%d\n", u1CurWPos, u1CurLR, u1BaseRPos);            
        }
        
        u1FrameNum <<= 1;
        //if(_full_pr_mode_sel == 1) // FHD PR mode 2
        if (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FHD_PR_USE_V2))    
        {
            switch(u1State) 
            {
                case 0: 
                    u1RPos = (u1BaseRPos);      //L_odd
                    break;
                case 1:
                    u1RPos = (u1BaseRPos+3);    //R_even
                    break;
                case 2:
                    u1RPos = (u1BaseRPos+1);    //L_even 
                    break;
                case 3:
                    u1RPos = (u1BaseRPos+2);    //R_odd 
                    break;
            }
            
            if(u1RPos >= u1FrameNum)
            {
                u1RPos -= u1FrameNum; // u4RPos %= u1FrameNum
            }         
            
            u4RAddr = (_u4BaseAddress[VDP_1]>>4) 
                + u1RPos * RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);             
        
            if(u1State == 2) // special case for L_EVEN
            {
                u4RAddr -= RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);
            }
        }
        else    // fhd pr mode 1
        {
            switch(u1State) 
            {
        
                case 0:
                    u1RPos = (u1BaseRPos);      //L_odd
                    break;
                case 1:
                    u1RPos = (u1BaseRPos+2);    //R_odd
                    break;
                case 2:
                    u1RPos = (u1BaseRPos+1);    //L_even  
                    break;
                case 3:
                    u1RPos = (u1BaseRPos+3);    //R_even 
                    break;
            }
            if(u1RPos >= u1FrameNum)
            {
                u1RPos -= u1FrameNum; // u4BaseRPos %= u1FrameNum
            }         
            
            u4RAddr = (_u4BaseAddress[VDP_1]>>4) 
                + u1RPos * RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);            
        }
        
        HAL_Delay_us(_fpr_delay);
        // When SBS_TO_FP is turn on, tv3d_in_disp_r = r_pos % 2
        // So in this work around tv3d_in_disp_r == 0 all the time
        vRegWriteFldAlign(SCPIP_SYSTEM_01, (u1State & 1) /*u1State % 2*/, SYSTEM_01_TV3D_OINV);            
        vScpipSetRAddr(VDP_1, u4RAddr); 
        
        u1CurLR = RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_OUT);
        LOG(7, "u1State=%d, R_POS=%d, RSTARTP=0x%x, LR_OUT=%d\n", u1State, u1RPos, u4RAddr, u1CurLR);
        u1State = (u1State+1) & 3; // (u1State+1) % 4
    }

}

// Word around for Top-n-Bottom input to frame sequential output
// main only for now
static void _vScpipTnbWAProc()
{
    static UINT8 u1State = 0;
    static UINT8 u1BaseRPos;  
    UINT8  u1CurWPos;
    UINT32 u4RAddr;
    UINT8  u1FrameNum;
    UINT32 u4VStartPos;

    u1FrameNum = u4ScpipGetBufNum(VDP_1);

    // state machine initialization if state = 0
    if(u1State == 0) 
    {
        u1CurWPos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);

        u1BaseRPos = (u1CurWPos + (u1FrameNum-1)); 
        
        if(u1BaseRPos >= u1FrameNum)
        {
            u1BaseRPos -= u1FrameNum; // u4BaseRPos %= u1FrameNum
        }
        LOG(7, "u1CurWPos=%d, u1BaseRPos=%d\n", u1CurWPos, u1BaseRPos);            
    }

    if(bScpipFlipEnable[VDP_1])
    {
        u4VStartPos = ((u1State+1)&1) * RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1) / 2;
    }
    else
    {
        u4VStartPos = u1State * RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1) / 2;
    }

    u4RAddr = (_u4BaseAddress[VDP_1]>>4) 
        + u1BaseRPos * RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1)
        + u4VStartPos * RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);

    vScpipSetRAddr(VDP_1, u4RAddr); 
    vScpipSetOutputDispR((u1State & 1));
    LOG(7, "u1State=%d, RSTARTP=0x%x\n", u1State, u4RAddr);
    u1State = (u1State+1) & 1; // (u1State+1) % 2
    
}

static void _vScpipFS322WAProc(UINT32 u4VdpId)
{
    UINT8  u1BaseRPos;  
    UINT32 u4RAddr;
    UINT8  u1FrameNum;
    UINT8  u1PipOffset;
    UINT8  u1CurWPos;

    u1PipOffset = getDSOffset(getScalerPath(u4VdpId));
    u1FrameNum = u4ScpipGetBufNum(u4VdpId);
    u1CurWPos = RegReadFldAlign(SCPIP_PIP1_RO_00 + u1PipOffset, PIP1_RO_00_W_POS_1);
   
    u1BaseRPos = (u1CurWPos + (u1FrameNum-1)); // Set RPos = N - 1

    if(u1BaseRPos >= u1FrameNum)
    {
        u1BaseRPos -= u1FrameNum; // u4BaseRPos %= u1FrameNum
    }

    u1BaseRPos = (u1BaseRPos >> 1) << 1; // u4BaseRPos = u4BaseRPos - (u4RBasePos % 2)

    LOG(7, "VDP(%d) u1CurWPos=%d, u1BaseRPos=%d\n", u4VdpId, u1CurWPos, u1BaseRPos);
    u4RAddr = (_u4BaseAddress[u4VdpId]>>4) 
        + u1BaseRPos * RegReadFldAlign(SCPIP_PIP1_07 + u1PipOffset, PIP1_07_WINC_1);

    vScpipSetRAddr(u4VdpId, u4RAddr); 
    LOG(7, "VDP(%d) RSTARTP=0x%x\n",u4VdpId, u4RAddr);
}

static void _vScpipSensioInterlaceWA()
{
    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_INPUT_IS_3D)
        && (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_I_SPLIT)||fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)))
    {
        UINT8 u1RPos;
        UINT32 u4USVofst = 0;
        UINT8 u1FrameNum = u4ScpipGetBufNum(VDP_1);
        UINT32 au4USVofstVal[2];

        if(bScpipFlipEnable[VDP_1])
        {
            au4USVofstVal[0] = 0x7fff;
            au4USVofstVal[1] = 0x4000;
        }
        else
        {
            au4USVofstVal[0] = 0x4000;
            au4USVofstVal[1] = 0x7fff;        
        }
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_I_SPLIT))
        {
            u1FrameNum <<= 1;
        }
        
        // Because the setting will be applied in the next vsync
        // Set vertical up-scaler phase for RPos+1
        // ** notice that this will be wrong if output frame rate is not 2x ** //
        u1RPos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1);
        u1RPos += 1;
        if(u1RPos >= u1FrameNum)
        {
            u1RPos -= u1FrameNum; 
        }
        
        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_I_SPLIT))
        {
            UINT8 u1ActualRPos = u1RPos >> 1;
            ARRAY_BOUND_CHECK(u1ActualRPos, 6);                        
            u4USVofst = au4USVofstVal[(_au1FBField[u1ActualRPos])];
            LOG(7, "RPOS=%d Fld=%d USVofst=0x%x\n",u1RPos, _au1FBField[u1ActualRPos], u4USVofst);            
        }
        else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
        {
            ARRAY_BOUND_CHECK(u1RPos, 6);            
            u4USVofst = au4USVofstVal[(_au1FBField[u1RPos])];            
            LOG(7, "RPOS=%d Fld=%d USVofst=0x%x\n",u1RPos, _au1FBField[u1RPos], u4USVofst);            
        }
        
        vRegWriteFldAlign(SCPIP_US1_11, u4USVofst, US1_11_USCALER_V_INIT_1);
    }
}

void vScpipOnTVEInputVSync()
{   
    if(_fgFS3d22dToggleOn[VDP_2])
    {        
        _vScpipFS322WAProc(VDP_2);
    }
}


void vScpipOnMJCInputVSync()
{   
    if(_fgFprToggleOn)
    {        
        _vScpipFullPRProc();
    }
    if(_fgTnbWAToggleOn)
    {
        _vScpipTnbWAProc();
    }
    if(_fgFS3d22dToggleOn[VDP_1])
    {        
        _vScpipFS322WAProc(VDP_1);
    }
    if(_fgSensioWAOn)
    {
        _vScpipSensioInterlaceWA();
    }
}

// this function can only be use in 2d input source (input DISP_R should be a stable value)
void vScpipSetOutputDispR(BOOL fgIsR) // set 1 is R, 0 is L
{
    BOOL fgInputIsR = RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_IN);
    vRegWriteFldAlign(SCPIP_SYSTEM_01, (fgIsR != fgInputIsR), SYSTEM_01_TV3D_OINV);            
}
#endif

void vScpipOnInputVSync(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }
    LOG(9, "Input VSync ISR: Current W_POS = %d\n", RegReadFldAlign(SCPIP_PIP1_RO_00+getPIPOffset(bPath), PIP1_RO_00_W_POS_1));
    vScpipResetDramCountDown(bPath);
}


void vScpipOnDIOutputISR()
{
#ifdef CC_SCPOS_3DTV_SUPPORT    
    if(_fgSensioWAOn
        && fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_INPUT_IS_3D)
        && (fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_I_SPLIT)||fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS_Q_I)))
    {
        UINT8 u1WPos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
        ARRAY_BOUND_CHECK(u1WPos, 6);
        _au1FBField[u1WPos] = RegReadFldAlign(MDDI_KC_04, CF_BOTTOM);
        LOG(7, "WPOS=%d Fld=%d\n",u1WPos, _au1FBField[u1WPos]);
    }
#if 0 //def CC_SCPOS_3DTV_SUPPORT  
        {
            static UINT8 au1Tv3dPos[6];
            CRIT_STATE_T csState;
            UINT8 u1CurWPos;
            UINT8 u1CurLR;
            csState = x_crit_start();
            u1CurWPos = RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_W_POS_1);
            u1CurLR = RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_IN);
            x_crit_end(csState);
            au1Tv3dPos[u1CurWPos] = u1CurLR;
            LOG(6, "u1CurWPos=%d ", u1CurWPos);        
            LOG(6, "au1Tv3dPos: %d %d %d %d %d %d\n",au1Tv3dPos[0], au1Tv3dPos[1], au1Tv3dPos[2], au1Tv3dPos[3], au1Tv3dPos[4], au1Tv3dPos[5]);        
        }
#endif    
#endif
}

void vScpipSetHNegInitPhaseOnOff(const BOOL fgOnOff)
{
    if(_fgIsHNegInitPhaseOn != fgOnOff)
    {
        _fgIsHNegInitPhaseOn = fgOnOff;
        LOG(3, "[Scpip] Set horizontal negative initial phase: (%d)\n", fgOnOff);
    }
}

void vScpipSetVNegInitPhaseOnOff(const BOOL fgOnOff)
{
    if(_fgIsVNegInitPhaseOn != fgOnOff)
    {
        _fgIsVNegInitPhaseOn = fgOnOff;
        LOG(3, "[Scpip] Set vertical negative initial phase: (%d)\n", fgOnOff);
    }
}

/***************************************************************************
puac_src_buffer:DRAM start address
ui4_buffer_length: one frame size in dram
ui1_src_format:   8bit: 0-RGB422,1-RGB444,2-YCbCr422,3-YCbCr444
                 10bit:4-RGB422,5-RGB444,6-YCbCr422,7-YCbCr444
ui4_src_height:line in dram
ui4_src_width:pixel number in dram
ui4_src_pitch:packet number in a line ,1 means 128 bit
****************************************************************************/

#if defined(CC_MT5365) || defined(CC_MT5395)
void vScpipGetDramInf(UINT8 bPath,DRAM_INFO_T* inf)
{
		UINT8 format;
		UINT16 data_length;
		UINT32 src_width;
		UINT32 src_height;
		UINT32 dramSize;
		UINT32 startAddr;
#ifdef CC_FLIP_MIRROR_SUPPORT
		UINT8 FlipMirrorModule = u4GetFlipMirrorModule(bPath);
#endif
	
		if(inf == NULL)
			return ;
	
		if(VDP_SCPOS_DISPMODE_OFF == u1ScpipGetDispMode(bPath))
		{
			format = RegReadFldAlign(SCPIP_DS1_02, DS1_02_SEL444_1);
			data_length =RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1) ;
			dramSize = (RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1)<< 4);
			startAddr = ((u4ScpipGetRAddr(VDP_1)<<4) + (dramSize) * (RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1)));
			//src_height = RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1) -2;
			//for 65/95 vl will not add 2
			src_height = RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);
			src_width = _pPicInfo[bPath]->wTargetW;
			startAddr = u4ScpipGetDramAddrWithChannelOfst(bPath, startAddr);
			if( _arScalePrm[bPath].u4HpsFactor != DFT_DS_FACTOR)
			{
				src_width = DIVIDE_WITH_CARRIER((src_width * _arScalePrm[bPath].u4HpsFactor), DFT_DS_FACTOR);
			}
			src_width = DIVIDE_WITH_CARRIER(( src_width * _pDispPrm[bPath]->u4DSclH), DFT_DS_FACTOR);
	
			if(RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1))
			{
				format = format + 4;
			}
			if(fgIsVideoTiming(bDrvVideoGetTiming(bPath)))
			{
				format = format + 2;
			}
	
			inf->scaler_inf.startAddr =  startAddr;
			inf->scaler_inf.format = format;
			inf->scaler_inf.dramSize = dramSize;
			inf->scaler_inf.src_height = src_height;
			inf->scaler_inf.src_width = src_width;
			inf->scaler_inf.data_length = data_length;
			inf->scaler_inf.dram_from = 3;
			
	#ifdef CC_FLIP_MIRROR_SUPPORT
			if(FlipMirrorModule & ( FLIP_BY_SCALER | FLIP_BY_PSCAN | FLIP_BY_B2R ))
			{	
				if(bScpipMirrorEnable[bPath])
				{
					inf->scaler_inf.dram_from |= 0x10;
				}
				if(bScpipFlipEnable[bPath])
				{
					inf->scaler_inf.dram_from |= 0x20;
				}
			}
	#endif
			return ;
		}
		else if((SV_VD_MPEGHD ==  bGetVideoDecType(bPath)) ||(SV_VD_MPEGSD ==  bGetVideoDecType(bPath)))
		{
#ifdef CC_B2R_SKYPE_SUPPORT
            VDP_HalGetB2rDispInfo(bPath,(B2R_DISP_INFO_T *)inf);//(&(inf->B2R_inf));
#else
			VDP_HalGetB2rDispInfo((B2R_DISP_INFO_T *)inf);//(&(inf->B2R_inf));
#endif
			inf->B2R_inf.dram_from = 1;
			inf->scaler_inf.dram_from = 1;
			return ;
		}
		else if(VDP_SCPOS_PSCAN_DISPMODE == u1ScpipGetDispMode(bPath))
		{
			vDrvDIDispInfoForCapLog(bPath,(DI_DISP_INFO_T*)inf);
			inf->DI_inf.dram_from = 2; //HD exclude DTV from DI
			inf->scaler_inf.dram_from = 2;		
	#ifdef CC_FLIP_MIRROR_SUPPORT
			if(FlipMirrorModule == FLIP_BY_B2R)    //b2r enable flip & mirror at the same time
			{	
				inf->scaler_inf.dram_from |= 0x10;
				inf->scaler_inf.dram_from |= 0x20;
			}
			else
			{
				inf->scaler_inf.dram_from |= (inf->DI_inf.u1CfgFlipMirror<<4);
			}
	#endif
			return;
		}  
		else
		{
			inf->B2R_inf.dram_from = 0xF;
			return ;
		}
	
}

#endif
UINT32 u4ScpipGet10BitMode(UINT32 u4VdpId)
{
    UINT32 u410BitMode = 0xffffffff;
    UINT32 u4Forced10BitMode = u4ScpipGetForced10BitMode(u4VdpId);
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);    
    if(u4Forced10BitMode == 0) // FW auto
    {
        u410BitMode = _arScalePrm[u4VdpId].u410BitMode;
    }
    else if (u4Forced10BitMode == VDP_SCPOS_8BitMode) // force 8 bit mode
    {
        u410BitMode = 0;
    }
    else if (u4Forced10BitMode == VDP_SCPOS_10BitMode) // force 10 bit mode
    {
        u410BitMode = 1;
    }            

    ASSERT(u410BitMode < 2);

    return u410BitMode;
}

UINT32 u4ScpipGetForced10BitMode(UINT32 u4VdpId)
{
    ARRAY_BOUND_CHECK(u4VdpId, VDP_NS);    
    return _u4Forced10BitMode[u4VdpId];
}

UINT8 u1ScpipGet444Mode(UINT8 bPath)
{
    #ifdef CC_FLIP_MIRROR_SUPPORT
    #if CC_SCPOS_WA02
    if((u1ScpipGetDispMode(bPath) == VDP_SCPOS_PSCAN_DISPMODE)        
        && (u4GetFlipMirrorModule(bPath) == FLIP_BY_PSCAN)
        && (u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON))
    {
        return SV_TRUE;
    }
    else
    #endif
    #endif
    {
        return bIsScalerInput444(bPath);
    }
}

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
void vScpipLRInverseWA(UINT8 bInverse)
{
    _bLRInverse = bInverse;
    vScpip3DPRWA();
    return;
}

void vScpipSetDepthWA(UINT8 bDepth)
{
    if(bDepth > _bTTDOverscan)
    {
        bDepth = _bTTDOverscan;
    }
    _bTTDDepth = bDepth;
    vScpip3DPRWA();

    return;
}

void vScpipSetOverscanWA(UINT8 bOverscan)
{
    if(bOverscan > SCPIP_3DPRWA_MAX_OVERSCAN)
    {
        bOverscan = SCPIP_3DPRWA_MAX_OVERSCAN;
    }
    _bTTDOverscan = bOverscan;
    vScpip3DPRWA();

    return;
}

static void vScpipMainSubEnableWA(void)
{
    UINT8 u1Ret;
        
    _bResetScalerCountFor3DPR = 4;
    _u4Forced10BitMode[VDP_1] = VDP_SCPOS_8BitMode;        
    _vScpipSetDramWrite(VDP_1, SV_OFF);
    _vScpipSetDramWrite(VDP_2, SV_OFF);  

    u1Ret = bApiVideoSetVideoSrc(VDP_2, _bCurrentInputMSFor3DPR);
    u1Ret = VDP_SetEnable(VDP_2, 1);
    if(u1Ret)
    {
        Printf("** Successfully select src for sub **\n");
    }
    else
    {
        Printf("** Fail to select src for sub **\n");  
    }  
    vScpipSetDramBurst(VDP_1,0);
    vScpipSetDramBurst(VDP_2,0);   
    vScpipSetBufNum(VDP_1, 3);  
    vScpipSetBufNum(VDP_2, 3);
    
    vRegWriteFldAlign(SCPIP_PIP1_0C, 1, PIP1_0C_DISPMODE_FIFO_SEP_REG_1);      
    vRegWriteFldAlign(SCPIP_PIP_OA_02,1,PIP_OA_02_CORE_RST); 
    vRegWriteFldAlign(SCPIP_PIP_OA_02,0,PIP_OA_02_CORE_RST);           
    bVideoSetDispRegion(VDP_2, 0, 0, 10000, 10000);  

    return;
}

void vScpip3DPRWA(void)
{
    static UINT8 bFirstTriggerHDMI = SV_TRUE;

    if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_FP_PR) 
       || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))
    {        
        vScpipMainSubEnableWA();
        
        bApiVideoSetSrcOverScan(VDP_1,0,0,0,0);
        bApiVideoSetSrcOverScan(VDP_2,0,0,0,0);

        if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_PR))
        {
            if(SV_FALSE == _bLRInverse)
            {
                bApiVideoSetSrcPosSize(VDP_2, 0, 5000, 10000, 5000);
            }
            else
            {
                bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 5000);
            }
            Printf(" TV3D in TNB mode \n");
        }
        else
        {
            if(SV_FALSE == _bLRInverse)
            {
                bApiVideoSetSrcPosSize(VDP_2, 0, 5102, 10000, 4898);
            }
            else
            {
                bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 4898);
            }
            Printf(" TV3D in FP mode \n");
        } 

        /* make W_EN_2 enable */
        if((SV_VD_DVI ==  bGetVideoDecType(VDP_1)) && (SV_TRUE == bFirstTriggerHDMI))
        {
            _fgScaleTriggerHdmiMchFor3DPR = SV_TRUE;
            bFirstTriggerHDMI = SV_FALSE;
        }        
    }
    else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_PR))
    {
        if(SV_FALSE == _bLRInverse)
        {
            VDP_SetEnable(VDP_2, 0);
            _u4Forced10BitMode[VDP_1] = 0;      
            vRegWriteFldAlign(SCPIP_PIP1_0C, 1, PIP1_0C_DISPMODE_FIFO_SEP_REG_1);
            vScpipSetDramBurst(VDP_1,0);
            bApiVideoSetSrcOverScan(VDP_1, 0, 0, 0, 0);     
            bVideoSetDispRegion(VDP_2, 0, 0, 0, 0);
        }
        else    /* SBS inverse need SUB's help */
        {               
            vScpipMainSubEnableWA();
            
            bApiVideoSetSrcOverScan(VDP_1,0,0,0,0);
            bApiVideoSetSrcOverScan(VDP_2,0,0,0,0);
            
            /* set full src region to make SUB having src , we use sync region to clip src region */
            bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 10000);
            
            if((SV_VD_DVI ==  bGetVideoDecType(VDP_1)) && (SV_TRUE == bFirstTriggerHDMI))
            {
                _fgScaleTriggerHdmiMchFor3DPR = SV_TRUE;
                bFirstTriggerHDMI = SV_FALSE;
            }            
        }    
        Printf(" TV3D in SBS mode \n");
    }
    else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TTD_PR))  /* 2D TO 3D */
    {
        vScpipMainSubEnableWA(); 
        bApiVideoSetSrcOverScan(VDP_1,0,0,0,0);
        bApiVideoSetSrcOverScan(VDP_2,0,0,0,0);
        bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 10000);
        if((SV_VD_DVI ==  bGetVideoDecType(VDP_1)) && (SV_TRUE == bFirstTriggerHDMI))
        {
            _fgScaleTriggerHdmiMchFor3DPR = SV_TRUE;
            bFirstTriggerHDMI = SV_FALSE;
        }              
        Printf(" TV3D in TTD mode \n");
    }
    else
    {
        VDP_SetEnable(VDP_2, 0);
        _u4Forced10BitMode[VDP_1] = 0;
        vRegWriteFldAlign(SCPIP_PIP1_0C, 0, PIP1_0C_DISPMODE_FIFO_SEP_REG_1); 
        bVideoSetDispRegion(VDP_2, 0, 0, 0, 0);
        if(SV_VD_DVI ==  bGetVideoDecType(VDP_1))
        {
            bFirstTriggerHDMI = SV_TRUE;
        }    
        Printf(" TV3D in normal mode \n");
    }


    vSetScposFlg(MAIN_DISP_PRM_CHG);
    vSetScposFlg(PIP_DISP_PRM_CHG);    
    
    return;
}
#endif


#if SUPPORT_SBS_CONVERT
void vScpipSBSConVert(UINT8 ucOnOff)
{
   UINT8 bType;
   UINT8 u1Ret=0;
   bType = bGetVideoDecType(VDP_1);
   TV3D_STATUS_T r3dStatus;
   vTV3DGetStatus(VDP_1, &r3dStatus);
   switch (bType)
   {
        case SV_VD_DVI:
        {
            if(ucOnOff)
            {
                _bSBSOutput= SV_TRUE;
                _bResetScalerCount = 4;
                _u4Forced10BitMode[VDP_1] = VDP_SCPOS_8BitMode;
                _vScpipSetDramWrite(VDP_1, SV_OFF);
                _vScpipSetDramWrite(VDP_2, SV_OFF);
                vRegWriteFldAlign(SCPIP_PIP_OA_02,1,PIP_OA_02_CORE_RST); 
                vRegWriteFldAlign(SCPIP_PIP_OA_02,0,PIP_OA_02_CORE_RST); 
                u1Ret = bApiVideoSetVideoSrc(VDP_2, _bCurrentInputMS);
                if(u1Ret)
                {
                    Printf("Successful to select to DVI\n");
                }
                else
                {
                    Printf("Fail to select to DVI\n");
                }
                if(r3dStatus.r3DType == TV3D_TYPE_FRAME_SEQUENTIAL)
                {
                    bVideoSetDispRegion(VDP_2, 5000, 0, 5000, 10000);
                    bApiVideoSetSrcPosSize(VDP_1, 0, 0, 10000, 4898);
                    bApiVideoSetSrcPosSize(VDP_2, 0, 5102, 10000, 4898);
                    _fgScaleTriggerHdmiMch = SV_TRUE;
                    _arScalePrm[VDP_1].u4ForcedPreScalingactor = 0x8000;
                    _arScalePrm[VDP_2].u4ForcedPreScalingactor = 0x8000;
                    
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                    LOG(0,"3d type is frame packing \n");
                }
                else if(r3dStatus.r3DType == TV3D_TYPE_TOP_N_BOTTOM)
                {
                    bVideoSetDispRegion(VDP_2, 5000, 0, 5000, 10000);
                    bApiVideoSetSrcPosSize(VDP_1, 0, 0, 10000, 5000);
                    bApiVideoSetSrcPosSize(VDP_2, 0, 5000, 10000, 5000);
                    _fgScaleTriggerHdmiMch = SV_TRUE;
                    _arScalePrm[VDP_1].u4ForcedPreScalingactor = 0x8000;
                    _arScalePrm[VDP_2].u4ForcedPreScalingactor = 0x8000;
                    _bSBSOutput= SV_TRUE;
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                    LOG(0,"3d type is top and bottom \n");
                }
                
                
            }
            else
            {
                _bSBSOutput= SV_FALSE;
                bApiVideoSetSrcPosSize(VDP_1, 0, 0, 10000, 10000);
                bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 10000);
                _u4Forced10BitMode[VDP_1] = 0;
                _fgScaleTriggerHdmiMch = SV_FALSE;
                _arScalePrm[VDP_1].u4ForcedPreScalingactor = 0;
                _arScalePrm[VDP_2].u4ForcedPreScalingactor = 0;
                vSetScposFlg(MAIN_DISP_PRM_CHG);
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
            
                    
        }
        break;
        
        case SV_VD_MPEGHD:
        {
            if(ucOnOff)
            {
                _bSBSOutput= SV_TRUE;
                _bResetScalerCount = 4;
                _u4Forced10BitMode[VDP_1] = VDP_SCPOS_8BitMode;
                u1Ret = bApiVideoSetVideoSrc(VDP_2, _bCurrentInputMS);
                if(u1Ret)
                {
                    Printf("Successful to select to MPEGHD\n");
                }
                else
                {
                    Printf("Fail to select to MPGEGHD\n");
                }
                _vScpipSetDramWrite(VDP_1, SV_OFF);
                _vScpipSetDramWrite(VDP_2, SV_OFF);
                 vRegWriteFldAlign(SCPIP_PIP_OA_02,1,PIP_OA_02_CORE_RST); 
                 vRegWriteFldAlign(SCPIP_PIP_OA_02,0,PIP_OA_02_CORE_RST); 
               
                    bVideoSetDispRegion(VDP_2, 5000, 0, 5000, 10000);
                    bApiVideoSetSrcPosSize(VDP_1, 0, 0, 10000, 5000);
                    bApiVideoSetSrcPosSize(VDP_2, 0, 5000, 10000, 5000);
                    _arScalePrm[VDP_1].u4ForcedPreScalingactor = 0x8000;
                    _arScalePrm[VDP_2].u4ForcedPreScalingactor = 0x8000;
                    _bSBSOutput= SV_TRUE;
                    vSetScposFlg(MAIN_DISP_PRM_CHG);
                    vSetScposFlg(PIP_DISP_PRM_CHG);
                    LOG(0,"3d type is top and bottom \n");
                    
             }
            else
            {
                _bSBSOutput= SV_FALSE;
                bApiVideoSetSrcPosSize(VDP_1, 0, 0, 10000, 10000);
                bApiVideoSetSrcPosSize(VDP_2, 0, 0, 10000, 10000);
                _u4Forced10BitMode[VDP_1] = 0;
                _arScalePrm[VDP_1].u4ForcedPreScalingactor = 0;
                _arScalePrm[VDP_2].u4ForcedPreScalingactor = 0;
                vSetScposFlg(MAIN_DISP_PRM_CHG);
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
            //TODO: add DTV 3d SBS output  flow
        }
        case SV_VD_MPEGSD:
            // TODO: add DTV 3d support flow
        break;
        
        default:
            LOG(0,"Current input is not hdmi and MPEG\n");
    } 
}

#endif
#ifdef IS_SUPPORT_3D_PHOTO
void Set3DPhotoRegion(BOOL fgIsFullResolution, BOOL fgIsFixSrcAspectRatio, UINT32 u4CropTopOffset,UINT32 u4CropBottomOffset,UINT32 u4CropLeftOffset,UINT32 u4CropRightOffset,BOOL bFix)
{
	UINT32 u4Width;
    UINT32 u4Height;
	UINT16 u4XOffSet = 0;
	UINT16 u4YOffSet = 0;
	UINT32 u4PanelWidth, u4PanelHeight;

	u4PanelWidth = PANEL_GetPanelWidth();
	u4PanelHeight = PANEL_GetPanelHeight();
	if(TRUE == bFix && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)) 
    {
    	IsFixRegion = TRUE;
		u4Height = u4PanelHeight - u4CropTopOffset - u4CropBottomOffset;
		u4Width = u4PanelWidth - u4CropLeftOffset - u4CropRightOffset;
		LOG(2,"\nu4CropTopOffset %d, u4CropBottomOffset %d, u4CropLeftOffset %d, u4CropRightOffset %d\n", u4CropTopOffset, u4CropBottomOffset, u4CropLeftOffset, u4CropRightOffset);
	    //LOG(3,"\n******u4Height %d, u4Width %d after crop******\n", u4Height, u4Width);

		if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBSH_SPLIT)
							|| fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_SPLIT)
							|| fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS)
							|| fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS_Q)
                            || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_SBS_Q_I_SPLIT)
                            || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_SBS_Q_I))
	    {
	        u4Width = u4Width >> 1;
		}
		else if(fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_SPLIT)
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_3D_TO_2D_TNB)
                || fgIsScpipTv3dFlgSet(VDP_1, SCPIP_TV3D_TNB_SW_WA))
		{
		    u4Height = u4Height >> 1;
		}

		/* Horizontal Direction */
		_vScpipCalScaleFactor(VDP_1, u4PanelWidth, u4Width, 1920, CAL_HDIRECT_FACTOR);//1920?
        /* Vertical Direction */
        _vScpipCalScaleFactor(VDP_1, u4PanelHeight, u4Height, 1080, CAL_VDIRECT_FACTOR);//1080?

        //LOG(3,"_pDispPrm[VDP_1]->u4USclH %X, _pDispPrm[VDP_1]->u4USclV %X", _pDispPrm[VDP_1]->u4USclH, _pDispPrm[VDP_1]->u4USclV);
        //check which side factor is smaller       
		if(_pDispPrm[VDP_1]->u4USclH < _pDispPrm[VDP_1]->u4USclV)
		{
		    u4Height = u4PanelHeight;
			u4Width =  DIVIDE_WITH_CARRIER((u4Width * DFT_US_FACTOR), _pDispPrm[VDP_1]->u4USclV);
			u4XOffSet = (u4PanelWidth - u4Width) >>1;
			u4YOffSet = 0;
		}
		else if(_pDispPrm[VDP_1]->u4USclH > _pDispPrm[VDP_1]->u4USclV)
		{
		    u4Height = DIVIDE_WITH_CARRIER((u4Height * DFT_US_FACTOR), _pDispPrm[VDP_1]->u4USclH);
			u4Width = u4PanelWidth;
			u4YOffSet = (u4PanelHeight - u4Height) >>1;
			u4XOffSet = 0;
		}
		else
		{
		    u4Height = u4PanelHeight;
			u4Width = u4PanelWidth;		
		}

        //LOG(3, "u4Height %d, u4Width %d, u4XOffSet %d, u4YOffSet %d", u4Height, u4Width, u4XOffSet, u4YOffSet);
		//UINT8 bApiVideoSetSrcOverScan(UINT8 bPath, UINT16 wTop, UINT16 wBottom, UINT16 wLeft, UINT16 wRight) 
		bApiVideoSetSrcOverScan(VDP_1, 0, 0, 0, 0);
		//bVideoSetDispRegion(UINT8 bPath, UINT16 wXOff, UINT16 wYOff, UINT16 wWidth, UINT16 wHeight)	
		u4XOffSet = u4XOffSet * VDP_MAX_REGION_WIDTH / u4PanelWidth;
		u4YOffSet = u4YOffSet * VDP_MAX_REGION_HEIGHT / u4PanelHeight;
		u4Width = u4Width * VDP_MAX_REGION_WIDTH / u4PanelWidth;
		u4Height = u4Height * VDP_MAX_REGION_HEIGHT / u4PanelHeight;
		bVideoSetDispRegion(VDP_1, u4XOffSet, u4YOffSet, u4Width, u4Height);
		LOG(2,"\nTRUE == bFix 3D mode on\n");
    }
	else if (FALSE == bFix && IsFixRegion)
	{
	    //here exit 3D photo enter other sources
	    LOG(2,"\n******FALSE == bFix************\n");
	    bDrvUpdateInternalFrameRate();
	    IsFixRegion = FALSE;
	}
	else if(TRUE == bFix && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_OFF))
	{
	    //here exit current photo and back to thumbnail, update MOVLEN
	    //LOG(2,"\nTRUE == bFix 3D mode off\n");
	    //bDrvUpdateInternalFrameRate();
	}
}
#endif


