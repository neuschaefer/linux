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
 * $Author: chang.ren $
 * $Date: 2013/01/18 $
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #8 $
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
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif
#include "drv_tdtv_drvif.h"
#include "drv_scaler_gfx.h"

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
// queue scaler event when freezing
#define CC_QUEUE_EVENT_IN_FREEZE

#ifdef TVE_WA
#define TVE_WA2 
#endif

#define SCPIP_TV3D_DEBUG

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define DFT_SCPOS_MAIN_444_FIFO_LENGTH        1920
#define DFT_SCPOS_MAIN_422_FIFO_LENGTH        2100
#define DFT_SCPOS_SUB_444_FIFO_LENGTH        960
#define DFT_SCPOS_SUB_422_FIFO_LENGTH        960
static const UINT32 _u4SCPOS_FIFO_LENGTH[2][2] = {{DFT_SCPOS_MAIN_422_FIFO_LENGTH, DFT_SCPOS_MAIN_444_FIFO_LENGTH},//main
    {DFT_SCPOS_SUB_422_FIFO_LENGTH, DFT_SCPOS_SUB_444_FIFO_LENGTH}};    //sub
#define FRAME_TRACK_DEFAULT  ((1<<16)-1)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#if defined(CC_MT5396)
SCALER_OUTTIMING_PRM_T _arOutTimingPrm = {2200,1920,1125,1080,0x8000,0x8000,60};
UINT8 bFSCTimingChanged = SV_FALSE;
#endif
//only for test
UINT8 bImportTrigger = SV_TRUE;
#ifdef IS_SUPPORT_3D_PHOTO
BOOL _fgFixRegion = SV_FALSE;
TDTV_3D_CROP_INTO_T   _rM3DCropInfo;
#endif

#ifdef DYNAMIC_RES_VERIFY
UINT8 fgPscanOversca = SV_FALSE;
EXTERN UINT8 u1EnDynRes;
#endif

// nptv global variable
VDP_SCALER_PRM_T _arScalePrm[VDP_NS] =
{
    {                                                      //SV_VP_MAIN
        0,                                                 //u4Enable;
        SV_FALSE,                                          //u4ModChgDneFlg;
        DFT_DS_FACTOR,                                     //u4HpsFactor;
        0,                                                 //u4HpsWidth;
        0,                                                 //u4HpsAndOverscanedWidth;
        0,                                                 //width after pre-down, overscan & down
        0,                                                 //height after down
        0,                                                 //u4FixedPreScaler;
        0,                                                 //u4ForcedPreScalingactor;
        0,                                                 //u4FirstConfig;
        0,                                                 //u4Forced10BitMode;
        0,                                                 //u410BitMode;
        0,                                                 //u4Is444;
        0,                                                 //u4121MapEn;
        0,                                                 //u4121Map;
        SCPIP_121MAP_H_POS_MID,          //u2121MapHPos
        VDP_SCPOS_DISPMODE_UNKNOWN,                        //u4DispMode;
        SV_FALSE,                                          //u4IsGameMode;
        0,                                                 //u4MirrorEnable;
        0,                                                 //u4FlipEnable
        0,                                                 //u4Is422Up       
        SV_FALSE,                                          //u4IsScalerBobMode
        SV_FALSE,                                          //u4IsLRDataSwap        
        0,                                                 //u4NonlinearEn;
        0,                                                 //u4NonlinearCtrl;
        0,                                                 //u4LastNonlinearCtrl;
        TRUE,                                              //u4NonlinearAuto;
        {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR},    //rAutoNonlinearPrm;
        {0, 0, 0, 0},                                      //rNonlinearPrm;
        {0, 0, 0, 0},                                      //rLastNonlinearPrm;
        SCALER_DRAM_INIT_STATE_NOT_INIT,                   //eDramInitState;
        0,                                                 //u4CntToRstDramPrm;
        0,                                                 //u4DramWidth;
        0,                                                 //u4DramHeight;
        0,                                                 //u4MddiFifoMin;
        0,                                                 //u4MddiFifoMax;
        {0, 0, 0, 0, 0, 0, 0, 0, 0},                       //rFbmInfo;
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},             //rDramAddrInfo;
        0,                                                 //u4NewNonlinear;
        {0, 0, 0, 0, 0},                                   //_NONLINEAR_REG_T rNewNonlinearPrm;
        {SCALER_TDTV_IN_TYPE_2D,
         SCALER_TDTV_OUT_TYPE_2D,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         SV_FALSE},//rTdtvInfo
#if defined(MIB_420_MIRROR_WA)
         0,
#endif
    },
    {                                                      //SV_VP_MAIN
        0,												   //u4Enable
        SV_FALSE,                                          //u4ModChgDneFlg;        
        DFT_DS_FACTOR,                                     //u4HpsFactor
        0,                                                 //u4HpsWidth
        0,                                                 //u4HpsAndOverscanedWidth;        
        0,                                                 //width after pre-down, overscan & down
        0,                                                 //height after down        
        0,                                                 //u4FixedPreScaler
        0,                                                 //u4ForcedPreScalingactor
        0,                                                 //u4FirstConfig
        0,                                                 //u4Forced10BitMode
        0,                                                 //u410BitMode
        0,                                                 //u4Is444
        0,                                                 //u4121MapEn
        0,                                                 //u4121Map
        SCPIP_121MAP_H_POS_MID,          //u2121MapHPos
        VDP_SCPOS_DISPMODE_UNKNOWN,                        //u4DispMode
        SV_FALSE,                                          //u4IsGameMode;
        0,                                                 //u4MirrorEnable;
        0,                                                 //u4FlipEnable
        0,                                                 //u4Is422Up
        SV_FALSE,                                          //u4IsScalerBobMode
        SV_FALSE,                                          //u4IsLRDataSwap        
        0,                                                 //u4NonlinearEn
        0,                                                 //u4NonlinearCtrl
        0,                                                 //u4LastNonlinearCtrl
        FALSE,                                             //u4NonlinearAuto
        {0, UNIT_NONLINEAR, UINT_NONLINEAR_END_FACTOR},    //rAutoNonlinearPrm
        {0, 0, 0, 0},                                      //rNonlinearPrm
        {0, 0, 0, 0},                                      //rLastNonlinearPrm
        SCALER_DRAM_INIT_STATE_NOT_INIT,                   //eDramInitState;        
        0,                                                 //u4CntToRstDramPrm;
        0,                                                 //u4DramWidth
        0,                                                 //u4DramHeight
        0,                                                 //u4MddiFifoMin
        0,                                                 //u4MddiFifoMax
        {0, 0, 0, 0, 0, 0, 0, 0, 0},                       //rFbmInfo;
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,},             //rDramAddrInfo;        
        0,                                                 //u4NewNonlinear
        {0, 0, 0, 0, 0},                                   //rNewNonlinearPrm
        {SCALER_TDTV_IN_TYPE_2D,
         SCALER_TDTV_OUT_TYPE_2D,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         E_TD_RES_NORMAL,
         SV_FALSE},   //rTdtvInfo
#if defined(MIB_420_MIRROR_WA)
         0,
#endif
    }
};

HANDLE_T h_dramUpdateSema = (HANDLE_T) NULL;

MJC_CLIP_INFO_T rMjcClipInfo;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Functions defined in drv_scaler_dram.c
//-----------------------------------------------------------------------------
EXTERN void vScpipUpdateDramPrm(UINT32 u4VdpId, const SCALER_FBM_POOL_T* prFbmPool);
EXTERN void vScpipUpdateDramReg(UINT32 u4VdpId);
EXTERN void vScpipCheckWriteCtrl(UINT8 bPath);
EXTERN void vScpipCalDramLimit(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4OutWidth, UINT32 u4OutHeight, UINT32 *u4DramW, UINT32 *u4DramH);
EXTERN void vScpipResetDramCountDown(UINT8 bPath);
//-----------------------------------------------------------------------------
// Functions defined in drv_scaler_dispmode.c
//-----------------------------------------------------------------------------
EXTERN void vScpipDispmodeFreeRunChk(void);
EXTERN void vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bMode);
EXTERN void vScpipSetPscanDispmodeHLen(UINT8 bPath, UINT32 u4HLen);
EXTERN UINT32 u4ScpipGetPscanDispmodeHLen(UINT8 bPath);
EXTERN void vScpipModeSwitch(UINT8 bPath);
EXTERN void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode);


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
#ifdef CC_FLIP_MIRROR_SUPPORT
EXTERN void vScpipConfigFlip(UINT8 bPath);
#endif

static void _vScpipNormProcByPath(const UINT32 u4VdpId); 
static UINT8 _bScpipGetUpScalerVTap(void);
static void _vScpipConfigScalingMode(UINT32 u4VdpId);
static void _vScpipSetDEAlignmentReg(UINT8 index);
static void _VDP_ScposEnable(UCHAR ucVdpId, UCHAR ucEnable);

//static function for update display parameter
static void _vScpipUpdateDataFormatPrm(UINT32 u4VdpId);
static void _vScpipUpdateHPorchPrm(UINT32 u4VdpId);
static void _vScpipUpdateVPorchPrm(UINT32 u4VdpId);
static void _vScpipUpdateDramHpPrm(UINT32 u4VdpId);
static void _vScpipUpdateDramVpPrm(UINT32 u4VdpId);
static void _vScpipUpdateDramRWLengthPrm(UINT32 u4VdpId);
static void _vScpipUpdateDispmodePrm(UINT32 u4VdpId);
static void _vScpipUpdateDisplayWindowPrm(UINT32 u4VdpId);
static void _vScpipUpdateMirrorFlipPrm(UINT32 u4VdpId);
static void _vScpipUpdateHBoundaryPrm(UINT32 u4VdpId);
static void _vScpipUpdateVBoundaryPrm(UINT32 u4VdpId);
static void _vScpipUpdateInitPhasePrm(UINT32 u4VdpId);

//static function for update regiter
static void _vScpipSetUSReg(UINT32 u4VdpId);
static void _vScpipSetUSInitialFactor(const UINT32 u4VdpId);
static void _vScpipSetDSInitialFactor(UINT32 u4VdpId);
static void _vScpipSetDownScalingFactor(UINT32 u4VdpId);
static void _vScpipSetDispReg(UINT32 u4VdpId);
static void _vScpipSetDataFormatReg(UINT32 u4VdpId);
static void _vScpipSetDisplayWindowReg(UINT32 u4VdpId);
static void _vScpipSetDramRWLengthReg(UINT32 u4VdpId);
static void _vScpipSetHPorchReg(UINT32 u4VdpId);
static void _vScpipSetVPorchReg(UINT32 u4VdpId);
static void _vScpipSetHBoundaryReg(UINT32 u4VdpId);
static void _vScpipSetVBoundaryReg(UINT32 u4VdpId);
static void _vScpipSetMirrorFlipReg(UINT32 u4VdpId);
static void _vScpipSetDispmodeReg(UINT32 u4VdpId);
static void _vScpipSetVLMaskReg(UINT32 u4VdpId);
static void _vScpipSetImportOnTVE(UINT8 bPath);

//static function for calculate scaling factor
static void _vScpipCalNewScale(UINT32 u4VdpId);
static UINT8 _vScpicIsOneToOneAble(UINT32 u4VdpId);
static void _vScpipUpdate121MapEn(UINT32 u4VdpId);
static void _vScpipUpdateMddiFifoInfo(UINT32 u4VdpId);
static void _vScpipUpdateHPSInfo(UINT32 u4VdpId);
static UINT32 _u4ScpipCalNegInitPhase(const UINT32 u4InW, const UINT32 u4OutW, const UINT32 u4UScl);
void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 in, UINT32 out, UINT32 u4LimitLength, UINT8 bDircection);
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode);
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode);

//static function for frame track
static void _vScpipSetFrameTrackOnOff(UINT32 u4OnOff);
static void _vScpipConfigFrameTrack(UINT32 u4Step);

#ifdef SCPIP_TV3D_DEBUG
static void _vScpipLogLRStatus(void);
static UINT8 u1Tv3dLR = 0;
#endif    

#if defined(CC_MT5396)
EXTERN void vScpipConfigPOCLK(UINT8 mode);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT32 _au4OriFrameNum[VDP_NS] = {3, 3};
// When TVE on, force dispOff
static UINT32 _u4ForceDispOff[2]={SV_FALSE, SV_FALSE};
static UINT32 _u4ScposImportNotReady[VDP_NS] = {0, 0};
//static UINT32 _au4ScalingFactorChanged[VDP_NS] = {0, 0};

//#define CC_SCPOS_LOG_DRAM_PARAM (50)
#ifdef CC_SCPOS_LOG_DRAM_PARAM
static UINT32 _au4TimeLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4AddressLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _au4ValueLog[CC_SCPOS_LOG_DRAM_PARAM];
static UINT32 _u4LogIdx = 0;
#endif

static BOOL _fgIsHNegInitPhaseOn = SV_ON;
static BOOL _fgIsVNegInitPhaseOn = SV_ON;

static UINT32 _u4ScpipPanelVOfst;    //PANEL_VOFST for front scaler
static UINT32 _u4ScpipFrameTrackTarget = FRAME_TRACK_DEFAULT;

UINT16 DrvGetScalerForMIBHOffsetWA(UINT8 u1VdpId)
{
    return _arScalePrm[u1VdpId].u2HOffset_MIBMirrorWA;
}

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
UINT32 u4ScpipGetPanelVOfstVar(void)
{
    return _u4ScpipPanelVOfst;
}

void vScpipSetPanelVOfstVar(UINT32 panelVofst)
{
    _u4ScpipPanelVOfst = panelVofst;
}

UINT32 u4ScpipGetFrameTrackTargetVar(void)
{
    return _u4ScpipFrameTrackTarget;
}

void vScpipSetFrameTrackTargetVar(UINT32 target)
{
    _u4ScpipFrameTrackTarget = target;
}

#ifdef DYNAMIC_RES_VERIFY
void vSetSCDynRes(UINT8 fgOnOff)
{
    bImportTrigger = !fgOnOff; 
    vScpipSetImportOnOff(VDP_1, !fgOnOff);
    vRegWriteFldAlign(SCPIP_SYSTEM_10, fgOnOff, SYSTEM_10_IMPORT_SOURCE_SEL_1);
    vRegWriteFldAlign(SCPIP_HDS_05, fgOnOff, HDS_IMPORT_EN1);
}
void vForcePscanOverscan(UINT8 fgOnOff)
{
    fgPscanOversca = fgOnOff;
}
#endif

/**
 * @brief Re-calculate delay counter by input/output frame rate ratio
 * @param u4VdpId  
 * @param u4Delay delay counter for input frame rate = output frame rate
 * @return the delay counter for output domain
 */
// TODO: change back to static
UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay)
{
    UINT8  bPath;
    UINT32 u4InputFrameRate;
    UINT32 u4OutputFrameRate;

    bPath = getScalerPath(u4VdpId);
    u4InputFrameRate = bDrvVideoGetRefreshRate(bPath);
    u4OutputFrameRate = vDrvGetLCDFreq();
    
    if(u4InputFrameRate == 0)
    {
        return u4Delay;
    }

    if(PANEL_IsSupport120Hz())
    {
        u4OutputFrameRate >>= 1;
    }

    //@6896 review this
    //#ifdef CC_SCPOS_3DTV_SUPPORT        
    #if 0
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

#ifdef SCPIP_TV3D_DEBUG
/**
 *  @6896 new
 */
static void _vScpipLogLRStatus()
{
    UINT8 u1CurrentLR;
    
    u1CurrentLR = u1ScpipGetTv3dOutIndicator();
    LOG(9, "Current 3D TV LR is (%d)\n", u1CurrentLR);
    if(u1CurrentLR == u1Tv3dLR)
    {        
        LOG(8, "Current 3D TV LR is repeated\n");
    }
    else
    {
        u1Tv3dLR = u1CurrentLR;
    }
}
#endif

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
        vRegWriteFldAlign(MUTE_00, 1,  R_MUTE_POST_EN);
    }
    else
    {
        _bBlankMuteCnt = 0;
        vRegWriteFldAlign(MUTE_00, 0,  R_MUTE_POST_EN);
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
    
    if(mode != VDP_SCPOS_DISPMODE_OFF)  //can only freeze under dram mode
    {
        return SV_FALSE;
    }
    else
    {
        return SV_TRUE;
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

//@6896 review this
static void vScpipPowerCtrl(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath ==SV_VP_PIP)
    {
            vRegWriteFldAlign(SCPIP_SYSTEM_00, !bOnOff, SYSTEM_00_PIP_DRAMCK_PDN_OFF);
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
        //do...
    }
    else
    {
        vScpipWriteCtrlOnOff(ucVdpId, SV_OFF);
    }
    vScpipPowerCtrl(ucVdpId, ucEnable);        
} 

#ifdef CC_CLI
/**
 * @brief Print current scale paramters
 * @param ucVdpId VDP_1/VDP_2
 */
void vScpipPrintCurrentScale(UCHAR ucVdpId)
{
    UINT8 bPath;
    RVChannel* channel;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;
    RDispPrm* dispPrm;
    UINT8 isChannelOn;

    bPath = getScalerPath(ucVdpId);
    channel = getChannel(bPath);
    isChannelOn = channel->bIsChannelOn;        
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);

    if (isChannelOn != SV_OFF)
    {
        //LOG(1, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
        Printf("SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n", 
                ucVdpId,  
                dispInfo->wXOffset,
                dispInfo->wYOffset,
                dispInfo->wWidth,
                dispInfo->wHeight,
                getMixedHPorch(ucVdpId),
                getMixedVPorch(ucVdpId),
                picInfo->wTargetW,
                picInfo->wTargetH);

        //LOG(1, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
        Printf("HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n\n",
                dispPrm->u4USclH, dispPrm->u4USclV,
                _arScalePrm[ucVdpId].u4HpsFactor, 
                dispPrm->u4DSclH, dispPrm->u4DSclV);
    }
}
#endif

void vScpipDumpResInfo(UINT8 bPath)
{
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;

    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);
    
    Printf("[%s] info:\n", bPath==SV_VP_MAIN?"MAIN":"SUB");
    Printf("=============================================\n");
    Printf("[picInfo]\n");
    Printf("input htotal.............%5d\n",picInfo->wHTotal);
    Printf("input vtotal.............%5d\n",picInfo->wVTotal);
    Printf("input width..............%5d\n",picInfo->wSrcW);
    Printf("input height.............%5d\n",picInfo->wSrcH);    
    Printf("input xOffset............%5d\n",picInfo->wXOffset);
    Printf("input yOffset............%5d\n",picInfo->wYOffset);
    Printf("input TargetW............%5d\n",picInfo->wTargetW);
    Printf("input TargetH............%5d\n",picInfo->wTargetH);
    
    Printf("HPS Width................%5d\n",_arScalePrm[bPath].u4HpsWidth);
    Printf("HPS and overscan width...%5d\n",_arScalePrm[bPath].u4HpsAndOverscanedWidth);
    Printf("Down Scaled Width........%5d\n",_arScalePrm[bPath].u4DownScaledWidth);
    Printf("Down Scaled Height.......%5d\n",_arScalePrm[bPath].u4DownScaledHeight);
    
    Printf("[dispInfo]\n");
    Printf("output xOffset...........%5d\n",dispInfo->wXOffset);
    Printf("output yOffset...........%5d\n",dispInfo->wYOffset);
    Printf("output width.............%5d\n",dispInfo->wWidth);
    Printf("output height............%5d\n",dispInfo->wHeight);    

    Printf("[dispPrm]\n");
    Printf("dram write width.........%5d\n",dispPrm->u2DramWriteWidth);
    Printf("dram write height........%5d\n",dispPrm->u2DramWriteHeight);
    Printf("dram read width..........%5d\n",dispPrm->u2DramReadWidth);
    Printf("dram read height.........%5d\n",dispPrm->u2DramReadHeight);
    Printf("left overscan............%5d\n",dispPrm->wHPosOfst);
    Printf("right overscan(3D).......%5d\n",dispPrm->wOverscanRight);
    Printf("top overscan.............%5d\n",dispPrm->wVPosOfst);
    Printf("bottom overscan(3D)......%5d\n",dispPrm->wOverscanBottom);    
    Printf("RHP......................%5d\n",dispPrm->wRHPos);
    Printf("RHL......................%5d\n",dispPrm->wRHLen);
    Printf("RVP......................%5d\n",dispPrm->wRVPos);
    Printf("RVL......................%5d\n",dispPrm->wRVLen);
    #if defined(IS_SUPPORT_3D_PHOTO)
    if(bPath == SV_VP_MAIN)
    {
        Printf("[3D PHOTO Crop info]\n");
        Printf("fgIsFullResolution.......%5d\n",ScpipGet3DCropInfo()->fgIsFullResolution);
        Printf("fgIsFixSrcAspectRatio....%5d\n",ScpipGet3DCropInfo()->fgIsFixSrcAspectRatio);
        Printf("u4CropLeftOffset.........%5d\n",ScpipGet3DCropInfo()->u4CropLeftOffset);
        Printf("u4CropRightOffset........%5d\n",ScpipGet3DCropInfo()->u4CropRightOffset);
        Printf("u4CropTopOffset..........%5d\n",ScpipGet3DCropInfo()->u4CropTopOffset);
        Printf("u4CropBottomOffset.......%5d\n",ScpipGet3DCropInfo()->u4CropBottomOffset);
    }
    #endif
    #if defined(MIB_420_MIRROR_WA)
    Printf("H Offset MIB Mirror......%5d\n",_arScalePrm[bPath].u2HOffset_MIBMirrorWA);
    #endif
    Printf("[Global variable]\n");
    Printf("vScpipIs3DDoneByMJC......%5d\n",vScpipIs3DDoneByMJC());
    Printf("vScpipIs3DDoneByScaler...%5d\n",vScpipIs3DDoneByScaler(bPath));
    
    Printf("[Scaling Factor]\n");
    Printf("H Pre-down..................0x%4x\n",_arScalePrm[bPath].u4HpsFactor);
    Printf("H Down......................0x%4x\n",dispPrm->u4DSclH);
    Printf("H UP........................0x%4x\n",dispPrm->u4USclH);
    Printf("V Down......................0x%4x\n",dispPrm->u4DSclV);
    Printf("V UP........................0x%4x\n",dispPrm->u4USclV);    

    Printf("[MJC Clip Info]\n");
    Printf("Width.......................%5d\n",rMjcClipInfo.wWidth);
    Printf("Height......................%5d\n",rMjcClipInfo.wHeight);
    Printf("Target Width................%5d\n",rMjcClipInfo.wTargetWidth);
    Printf("Target Height...............%5d\n",rMjcClipInfo.wTargetHeight);
    Printf("Left........................%5d\n",rMjcClipInfo.wLeft);
    Printf("Right.......................%5d\n",rMjcClipInfo.wRight);
    Printf("Top.........................%5d\n",rMjcClipInfo.wTop);
    Printf("Bottom......................%5d\n",rMjcClipInfo.wBottom);
    Printf("Clip X......................%5d\n",rMjcClipInfo.wClipX);
    Printf("Clip Y......................%5d\n",rMjcClipInfo.wClipY);

    #if defined(CC_MT5396)
    Printf("[FSC Time Gen.]\n");
    Printf("FSC Panel HActive...........%5d\n",_arOutTimingPrm.u2PanelHActive);    
    Printf("FSC Panel HTotal............%5d\n",_arOutTimingPrm.u2PanelHTotal);    
    Printf("FSC Panel VActive...........%5d\n",_arOutTimingPrm.u2PanelVActive);    
    Printf("FSC Panel VTotal............%5d\n",_arOutTimingPrm.u2PanelVTotal);    
    Printf("FSC POCLK Dividend..........0x%4x\n",_arOutTimingPrm.u4DPllDividend);    
    Printf("FSC POCLK Divisor...........0x%4x\n",_arOutTimingPrm.u4DPLLDivisor);
    Printf("FSC->MJC Frame Rate.........%5d\n",_arOutTimingPrm.u2FrameRate);
    #endif

    #if defined(CC_MT5396)
    vPSCDumpResInfo();
    #endif
}

/**
 * @brief Get scpos porch
 * @param bPath VDP_1/VDP_2
 * @param bPorchType porch type
 * @return scpos porch
 */
UINT16 _VDP_ScposGetPorch(UINT8 bPath, UINT8 bPorchType)
{
    RPicInfo* picInfo;
    picInfo = getPicInfo(bPath);

    switch(bPorchType)
    {
        case SV_HPORCH_CURRENT:
            return picInfo->wUIHPorch;
        case SV_HPORCH_DEFAULT:
            return 1000;
        case SV_VPORCH_CURRENT:
            return picInfo->wUIVPorch;
        case SV_VPORCH_DEFAULT:
            return 1000;
        case SV_HPORCH_MAX:
            return 1000 + picInfo->wSrcW - picInfo->wTargetW - picInfo->wXOffset;
        case SV_VPORCH_MAX:
            return 1000 + picInfo->wSrcH - picInfo->wTargetH - picInfo->wYOffset;
        case SV_HPORCH_MIN:
            return  1000 - picInfo->wXOffset;
        case SV_VPORCH_MIN:
            return  1000 - picInfo->wYOffset;
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
    RPicInfo* picInfo;
    UINT32 u4Update;

    picInfo = getPicInfo(bPath);
    u4Update = 0;

    if (bPorchType==SV_HPORCH_CURRENT)
    {
        if (wValue != picInfo->wUIHPorch) 
        {
            picInfo->wUIHPorch = wValue;
            u4Update = 1;
        }
    }
    else
    {
        if (wValue != picInfo->wUIVPorch) 
        {
            picInfo->wUIVPorch = wValue;
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




#ifdef CC_FLIP_MIRROR_SUPPORT
/**
 * @brief Set mirror flip mode
 * @param bPath VDP_1/VDP_2
 * @param bDramMode SV_TRUE/SV_FALSE 
 */
void vScpipConfigFlip(UINT8 bPath)
{
    // if(bDramMode ==SV_TRUE) // do flip only when dram mode
    if(u4GetFlipMirrorModule(bPath)==FLIP_BY_SCALER)
    {
        UINT8 u1Config;

        u1Config = u1GetFlipMirrorConfig();

        if((u1Config & SYS_MIRROR_CONFIG_ON))
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

    vScpipReconfig(getScalerVDP(bPath));
}
#endif

/*******************
 * Frame track API *
 *******************/
void vScpipFrameTrackConfig(UINT32 u4Step) 
{
    _vScpipConfigFrameTrack(u4Step);
    #if defined(SCPIP_SUPPORT_POST_SCALER)
        //To avoid panel sync keep moving. Enlarge PSC frame track step size. 
        vPscConfigFrameTrack(u4Step<<1);    
    #endif    
}

extern UINT32 GCD(UINT32 a, UINT32 b);

static void _vScpipConfigFrameTrack(UINT32 u4Step)
{
    UINT8 u1InFR;
    UINT8 u1OutFR;
    UINT8 u1TrackVNum;
    UINT32 u4TrackTargetVal;
    UINT32 u4PanelVTotal;
    UINT32 u4PanelHTotal;
    UINT32 u4UpperBound;
    UINT32 u4LowerBound;
    UINT32 u4LockRange;
    UINT8 bMode;
    UINT32 u4GCD;

    bMode = getScalerMode(SV_VP_MAIN);

    //for 5396 front scaler, input frame rate = output frame rate
    u1InFR = bDrvVideoGetRefreshRate(SV_VP_MAIN);

    u1OutFR = u2DrvGetInternalFrameRate();

    u4GCD = GCD(u1InFR, u1OutFR);
    
    //error handling
    if ((bDrvVideoSignalStatus(SV_VP_MAIN) != SV_VDO_STABLE) || (u1InFR==0) || (u1OutFR==0) || (u4GCD ==0))
    {
    	return;
    }
    
    //calculate track interval        
    u1TrackVNum = u1InFR / u4GCD;

    //get FSC time gen V Total
    u4PanelVTotal = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL);
    u4PanelHTotal = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL);

    if (u4ScpipGetFrameTrackTargetVar() != FRAME_TRACK_DEFAULT)
    {
        u4TrackTargetVal = u4ScpipGetFrameTrackTargetVar();
    }
    else if (u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_OFF)
    {
        #if defined(CC_MT5396)
        u4TrackTargetVal = _arOutTimingPrm.u2PanelVTotal /2; 
        #else
        u4TrackTargetVal = wDISPLAY_HEIGHT / 2;
        #endif        
    }
    else if((u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_AUTO)|| (u1ScpipGetDispMode(SV_VP_MAIN) == VDP_SCPOS_DISPMODE_MANUAL))
    {
        if(RegReadFldAlign(SCPIP_US1_01, US1_01_VUS_BYPASS_1))
        {
            u4TrackTargetVal = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_PRE_VOFST_BYPASS1);// track at pre_vofst_bypass in scaler dispmode + down  
        }
        else
        {
            u4TrackTargetVal = RegReadFldAlign(SCPIP_PIP_OA_03, PIP_OA_03_PRE_VOFST1);// track at pre_vofst in scaler dispmode + up 
        }
    }
    else
    {
        u4TrackTargetVal = wDISPLAY_HEIGHT / 50;
    }
    
    //1. set frame track interval
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, u1TrackVNum-1, PIP_OA_0D_FRAME_TRACK_VSYNC_NUM);  

    //2. set frame track target. current's target is PRE_VOFST_BYPASS
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, 1, PIP_OA_0D_FRAME_TRACK_DIST_TARGET_MODE);  
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, u4TrackTargetVal, PIP_OA_0D_FRAME_TRACK_DIST_TARGET_VAL);

    //3. set reference vsync
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        //in Pscan dispmode, reference vsync before Pscan(Input Vsync)
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 2, PIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL);
    }
    else if(bMode == VDP_SCPOS_DISPMODE_AUTO || 
            bMode == VDP_SCPOS_DISPMODE_MANUAL || 
            bMode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        //in scaler dispmode, reference vsync after porch
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0, PIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL);
    }
    else
    {
        //in other mode, reference vsync of scaler input
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 1, PIP_OA_0D_FRAME_TRACK_REF_VSYNC_SEL);    
    }
    
    //4. set to mask mode
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, 1, PIP_OA_0D_FRAME_TRACK_VSYNC_DIST_MODE);  
    
    //5. set lock speed
    if(u1TrackVNum!=0)
    {
        u4Step /= u1TrackVNum;
    }
    
    if(u4Step == 0)
    {
        u4Step = 1;
    }

    //5368 can select to use DDDS mode or VTotal mode, 5396 should only use VTotal mode
    #if !defined(CC_MT5396)  
    if(fgFrameTrackIsFlgSet(FRAME_TRACK_MODE_DDDS_BY_SCALER)) // DDDS frame track mode
    {
        //set to DDDS Adjust mode
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 1, PIP_OA_0D_FRAME_TRACK_MODE);          
    }else 
    #endif
    {
        //set to V Total Adjust mode
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0, PIP_OA_0D_FRAME_TRACK_MODE);
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4PanelVTotal - u4Step, PIP_OA_0E_VTOTAL_ADJUST_FAST01);
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4PanelVTotal - u4Step * 12, PSPIP_OA_0E_VTOTAL_ADJUST_FAST10);   //for frame track faster          
        vRegWriteFldAlign(SCPIP_PIP_OA_0F, u4PanelVTotal + u4Step, PIP_OA_0F_VTOTAL_ADJUST_SLOW01);
        vRegWriteFldAlign(SCPIP_PIP_OA_0F, u4PanelVTotal + u4Step * 12, PIP_OA_0F_VTOTAL_ADJUST_SLOW10);     //for frame track faster       
    }
    
    #if !defined(CC_MT5396) //for 5368/5389/8227  
    //6. set track range
    u4LockRange = u4Step * ((u1OutFR/u4GCD)/2) + 1; // +1 for bigger lock range
    //6.1 upper bound2
    u4UpperBound = u4TrackTargetVal + u4LockRange*6;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    vRegWriteFldAlign(SCPIP_PIP_OA_10, u4UpperBound, PIP_OA_10_UPPER_BOUND2);  
    //6.2 upper bound1
    u4UpperBound = u4TrackTargetVal + u4LockRange*2;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    vRegWriteFldAlign(SCPIP_PIP_OA_10, u4UpperBound, PIP_OA_10_UPPER_BOUND1);  
    //6.3 lower bound2
    u4LowerBound = (u4TrackTargetVal > u4LockRange*6)?(u4TrackTargetVal-(u4LockRange*6)):(u4TrackTargetVal+u4PanelVTotal-(u4LockRange*6));
    vRegWriteFldAlign(SCPIP_PIP_OA_11, u4LowerBound, PIP_OA_11_LOWER_BOUND2);      
    //6.4 lower bound1
    u4LowerBound = (u4TrackTargetVal > u4LockRange*2)?(u4TrackTargetVal - u4LockRange*2):(u4TrackTargetVal + u4PanelVTotal - u4LockRange*2);
    vRegWriteFldAlign(SCPIP_PIP_OA_11, u4LowerBound, PIP_OA_11_LOWER_BOUND1);   
    #else   //for 5396
    //6. set track range
    u4LockRange = u4Step * ((u1OutFR/u4GCD)/2) + 1; // +1 for bigger lock range
    //6.1 upper bound2
    u4UpperBound = u4TrackTargetVal + u4LockRange*3;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    vRegWriteFldAlign(SCPIP_PIP_OA_10, (u4UpperBound + 12), PIP_OA_10_UPPER_BOUND2);       //for frame track faster   
    //6.2 upper bound1
    u4UpperBound = u4TrackTargetVal + u4LockRange;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    vRegWriteFldAlign(SCPIP_PIP_OA_10, u4UpperBound + 5, PIP_OA_10_UPPER_BOUND1);  
    //6.3 lower bound2
    u4LowerBound = (u4TrackTargetVal > u4LockRange*3)
    				?((u4TrackTargetVal-(u4LockRange*3) > u4PanelVTotal)?(u4TrackTargetVal-(u4LockRange*3)-u4PanelVTotal):(u4TrackTargetVal-(u4LockRange*3)))
    				:(u4TrackTargetVal+u4PanelVTotal-(u4LockRange*3));
    vRegWriteFldAlign(SCPIP_PIP_OA_11, (u4LowerBound - 12), PIP_OA_11_LOWER_BOUND2);      //for frame track faster   
    //6.4 lower bound1
    u4LowerBound = (u4TrackTargetVal > u4LockRange)
    				?((u4TrackTargetVal - u4LockRange > u4PanelVTotal)?(u4TrackTargetVal - u4LockRange - u4PanelVTotal):(u4TrackTargetVal - u4LockRange))
    				:(u4TrackTargetVal + u4PanelVTotal - u4LockRange);
    vRegWriteFldAlign(SCPIP_PIP_OA_11, u4LowerBound - 5, PIP_OA_11_LOWER_BOUND1);   
    #endif
     //7 set protection mode
    //7.1 V lower protection bound
    vRegWriteFldAlign(SCPIP_PIP1_1E, u4TrackTargetVal - 1, PIP1_1E_FSYNC_PROT_V_LBND); 
    //7.2 V up protection bound
    vRegWriteFldAlign(SCPIP_PIP1_1D, u4TrackTargetVal, PIP1_1D_FSYNC_PROT_V_UBND); 
    //7.3 H lower protection bound
    vRegWriteFldAlign(SCPIP_PIP1_1E, u4PanelHTotal - 20, PIP1_1E_FSYNC_PROT_H_LBND);
    //7.4 H up protection bound
    vRegWriteFldAlign(SCPIP_PIP1_1D, 20, PIP1_1D_FSYNC_PROT_H_UBND);    
}

/**
 *  @6896 Use to enable/disable FSC/ PSC frame tack
 */
void vScpipSwitchFrameTrackOnOff(UINT32 u4FscOnOff, UINT32 u4PscOnOff) 
{
    // can be enable when dram mode and scaler display mode
    static UINT32 u4FscStatus = 0xFF;
    static UINT32 u4PscStatus = 0xFF;

    if (u4FscStatus != u4FscOnOff || u4PscStatus != u4PscOnOff)
    {
        u4FscStatus = u4FscOnOff;
        u4PscStatus = u4PscOnOff;
        LOG(5, "vScpipSwitchFrameTrackOnOff %d %d\n\n", u4FscOnOff, u4PscOnOff);
    }
    else
    {
        return; // no change
    }

    //1. set FSC frame track on/off
    _vScpipSetFrameTrackOnOff(u4FscOnOff);

    //2. set PSC frame track on/off
    #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscSetFrameTrackOnOff(u4PscOnOff);
    #endif
}

/**
 *  @6896 Enable/Disable FSC frame track.
 */
static void _vScpipSetFrameTrackOnOff(UINT32 u4OnOff)
{
    #if defined(__MODEL_slt__) || defined(DRV_SUPPORT_EXTMJC) // Frame tracking always off for SLT and external MJC
    vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0, PIP_OA_0D_FRAME_TRACK_EN);
    #else
    if (u4OnOff)  // notice the order
    {    
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 1, PIP_OA_0D_FRAME_TRACK_EN);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_0D, 0, PIP_OA_0D_FRAME_TRACK_EN);
    }
    #endif    
}

void vScpipOnInputVSync(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }
    vScpipResetDramCountDown(bPath);
}

#ifdef CC_SCPOS_3DTV_SUPPORT
void vScpipOnTVEInputVSync(void)
{
}
#endif

void vScpipOnMJCInputVSync(void)
{
    //vScpipSetPSCImportToggle();
}

void vScpipOnMJCOutputVSync(void)
{
    //vScpipSetPSCImportToggle();
}

void vScpipOnDIOutputISR(void)
{
    #if defined(CC_MT5396) && defined(SCPIP_TOGGLE_IMPORT_AT_INPUTV)
    #ifdef DYNAMIC_RES_VERIFY
    if (!u1EnDynRes)
    #endif	
    {
        vScpipSetFSCImportToggle();
    }
    #endif
}

/**===========================below 6896 porting done==================================================================**/

#if defined(SCPIP_LI_TO_SBS_VSYNC_OPT_WA)
void vScpipDoLIToSBSVsyncOptWA(UINT8 bPath, UINT8 bReset)
{
    E_TD_IN eDQCIn;
    E_TD_OUT eDQCOut;

    if(bPath != SV_VP_MAIN)
    {
        return;
    }

    eDQCIn = TD_DQC_IN(u4DrvTDTVDQCModeQuery());
    eDQCOut = TD_DQC_OUT(u4DrvTDTVDQCModeQuery());
    
    //Line interleave to SBS
    if((eDQCIn == E_TD_IN_LI_I || eDQCIn == E_TD_IN_LI_P) && 
        eDQCOut == E_TD_OUT_3D_SBS)
    {
        vRegWriteFldAlign(SCPIP_DS1_02, 1, DS1_02_VSYNC_OPT_1);    
    }
    else
    {
        if(bReset == SV_TRUE)
        {
            //vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_VSYNC_OPT_1);
            vScpipUpdateImportSetting(bPath, getScalerMode(bPath));
        }
    }
}
#endif

/**
 * @brief Sw Initialize 
 * @6896 wait review
 */
void vScpipSwInit(void)
{
    static UINT8 bScpipSwInited = SV_FALSE;
    //@6896 review this
    //vFrameTrackConfig(SCPIP_FRAME_TRACK_STEP_SMALL);//( u4LockRange,  u4LossLockRange,  u4Step) 
    //@6896 review this    
    #ifdef SUPPORT_SW_FRAME_CHECK
    vSetSWFrameTrackStep(SCPIP_FRAME_TRACK_STEP_SMALL);
    #endif
    //@6896 review this
    vScpipSetDumpCtrl(SV_OFF);

    if(bScpipSwInited == SV_FALSE)
    {
        if (x_sema_create(&h_dramUpdateSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ASSERT(0);
        }
        bScpipSwInited = SV_TRUE;
    }

    //vScpipSetFrameTrackTargetVar(wDISPLAY_HEIGHT / 2);
    vScpipSetPanelVOfstVar(wDISPLAY_HEIGHT / 10);
}

/**
 * @brief Set dynamic scaler
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @param u4ResponseTime response time
 * @6896 remove this function
 */
void vScpipSetDynamicScaler(UCHAR ucVdpId, UCHAR ucOnOff, UINT32 u4ResponseTime)
{
    //removed
}

/**
 * @brief Set game mode
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @6896 porting done
 */
void vScpipSetGameMode(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[ucVdpId].u4IsGameMode != ucOnOff)
    {
        LOG(3, "VDP(%d) Game Mode(%d)\n", ucVdpId, ucOnOff);
        _arScalePrm[ucVdpId].u4IsGameMode = ucOnOff;

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
 *  @6896 new
 */
UINT8 u1ScpipGetMJCClipInfo(UINT8 bPath,MJC_CLIP_INFO_T* clipInfo)
{
    RDispInfo* dispInfo;

    dispInfo = getDispInfo(bPath);
    
    if(clipInfo == NULL)
    {
        return SV_FAIL;
    }
    #if defined(SUPPORT_TV3D_OVERSCAN)
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        clipInfo->wLeft = rMjcClipInfo.wLeft;
        clipInfo->wRight = rMjcClipInfo.wRight;
        clipInfo->wTop = rMjcClipInfo.wTop;
        clipInfo->wBottom = rMjcClipInfo.wBottom;
        clipInfo->wWidth = rMjcClipInfo.wWidth;
        clipInfo->wHeight = rMjcClipInfo.wHeight;
        clipInfo->wClipX = rMjcClipInfo.wClipX;
        clipInfo->wClipY = rMjcClipInfo.wClipY;
        clipInfo->wTargetWidth  = rMjcClipInfo.wTargetWidth;
        clipInfo->wTargetHeight = rMjcClipInfo.wTargetHeight;
    }
    else
    #endif
    {
        clipInfo->wLeft = 0;
        clipInfo->wRight = 0;
        clipInfo->wTop = 0;
        clipInfo->wBottom = 0;
        clipInfo->wClipX = 0;
        clipInfo->wClipY = 0;
        clipInfo->wWidth = dispInfo->wWidth;
        clipInfo->wHeight = dispInfo->wHeight;        
        clipInfo->wTargetWidth  = wDISPLAY_WIDTH;
        clipInfo->wTargetHeight = wDISPLAY_HEIGHT;
    }

    return SV_SUCCESS;
}

/**
 * @brief For pscan to get overscan information. Especially in pscan dispmode.
 * @param bPath VDP_1/VDP_2
 * @param clipInfo structure to describe clip dimension.
 * @return status
 * @6896 porting done
 */
UINT8 u1ScpipGetClipInfo(UINT8 bPath,PSCAN_CLIP_INFO_T* clipInfo)
{
    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT8 bMode;
    picInfo = getPicInfo(bPath);
    u4Factor = u4ScpipGetHwHPS(bPath);
    bMode = u1ScpipGetDispMode(bPath);

#ifdef DYNAMIC_RES_VERIFY
    if (fgPscanOversca)
    {
        bMode = VDP_SCPOS_PSCAN_DISPMODE;
    }
#endif

    if(picInfo->wTargetH ==0 || picInfo->wTargetW==0 || u4Factor == 0)
    {
        return SV_FAIL;
    }


    if(bMode!= VDP_SCPOS_PSCAN_DISPMODE)
    {
        UINT32 u4NTDOfst;

        //If not in pscan dispmode, pscan should not do clip
        clipInfo->wX = 0;
        clipInfo->wWidth = u4ScpipGetPreScaleWidth(bPath);
        clipInfo->wY = 0;
        //clipInfo->wHeight = wDrvVideoInputHeight(bPath);
        #ifdef IS_SUPPORT_3D_PHOTO
        if(_fgFixRegion && (bPath == VDP_1))
            clipInfo->wHeight = wDrvVideoInputHeight(bPath);
        else
        clipInfo->wHeight = picInfo->wSrcH;
        #else
        clipInfo->wHeight = picInfo->wSrcH;
        #endif

        //@6896 new. Have to consider non-standard signal
        u4NTDOfst = u4DrvVideoGetNSDOffset(bPath);
        if (u4NTDOfst != 0x800)
        {
            if (((clipInfo->wHeight + u4NTDOfst) > 0x800) && (u4NTDOfst < 0x800))
            {
                LOG(8, "Non-Standard Signal Detected (%d)! Ofst: %d\n", bPath, u4NTDOfst);
                clipInfo->wHeight = clipInfo->wHeight + u4NTDOfst - 0x800;                
            }
        }
    }
    else    //pscan dispmode
    {
        #ifdef IS_SUPPORT_3D_PHOTO
        if(_fgFixRegion && (bPath == VDP_1))
        {
            TDTV_3D_CROP_INTO_T* eCropInfo;
            UINT32 u4MixHPorch, u4MixVPorch;
            eCropInfo = ScpipGet3DCropInfo();
            u4MixHPorch = (eCropInfo->u4CropLeftOffset + getPicInfo(bPath)->wUIHPorch >1000)? (eCropInfo->u4CropLeftOffset + getPicInfo(bPath)->wUIHPorch - 1000): 0;
            u4MixVPorch = (eCropInfo->u4CropTopOffset + getPicInfo(bPath)->wUIVPorch >1000)? (eCropInfo->u4CropTopOffset + getPicInfo(bPath)->wUIVPorch - 1000): 0;
            clipInfo->wX     = (UINT16)DIVIDE_WITH_CARRIER(u4MixHPorch * u4Factor, DFT_HPD_FACTOR);
            clipInfo->wY = u4MixVPorch; 
        }
        else
        {
        clipInfo->wX     = (UINT16)DIVIDE_WITH_CARRIER((UINT32)getMixedHPorch(bPath) * u4Factor, DFT_HPD_FACTOR);
             clipInfo->wY = getMixedVPorch(bPath); 
        }
        #else 
        clipInfo->wX     = (UINT16)DIVIDE_WITH_CARRIER((UINT32)getMixedHPorch(bPath) * u4Factor, DFT_HPD_FACTOR);
        clipInfo->wY = getMixedVPorch(bPath); 
        #endif
        clipInfo->wWidth = (UINT16)DIVIDE_WITH_CARRIER((UINT32)picInfo->wTargetW * u4Factor,DFT_HPD_FACTOR);           
        clipInfo->wHeight = picInfo->wTargetH;
#if 0   //@6896 review this
        if((clipInfo->wWidth % 2) && !bIsScalerInput444(bPath))
        {
            if(clipInfo->wX ==0)
            {
                clipInfo->wWidth -= 1;
            }
            else
            {
                clipInfo->wX -=1;
                clipInfo->wWidth += 1;
            }
        }
#endif    

    }
	if((clipInfo->wWidth % 2) && !bIsScalerInput444(bPath))
	{
		clipInfo->wWidth += 1;
	}
	
	if((clipInfo->wX % 2) && !bIsScalerInput444(bPath))
	{
		clipInfo->wX -= 1;
	}
    
    #if defined(MIB_420_MIRROR_WA)
    if(u4GetFlipMirrorModule(bPath) == FLIP_BY_PSCAN && bDrvVideoIsSrcInterlace(bPath) == SV_FALSE)
    {        
        _arScalePrm[bPath].u2HOffset_MIBMirrorWA = ALIGN_16_CEIL(clipInfo->wWidth) - clipInfo->wWidth;
        clipInfo->wWidth = ALIGN_16_CEIL(clipInfo->wWidth);
    }
    else
    {
        _arScalePrm[bPath].u2HOffset_MIBMirrorWA = 0;
    }
    #endif	
    
    return SV_SUCCESS;    
}

/**
 * @brief Configure scaling mode
 */
static void _vScpipConfigScalingMode(UINT32 u4VdpId)
{
    UINT8 bTap;
    UINT8 bVUSBypass = SV_OFF;
    UINT8 bVDSBypass = SV_OFF;
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    
    if (u4VdpId >= VDP_NS) 
    { 
        return;
    }

    //1. configure up-scaler Vertical taps
    if(bPath == SV_VP_MAIN)
    {
        bTap = _bScpipGetUpScalerVTap();
        vDrvSetUpScalerVTap(bPath, bTap);        
    }    
    
    //2. configure VDS_BYPASS & VUS_BYPASS
    if(dispPrm->u4USclV == DFT_US_FACTOR)
    {
        bVUSBypass = SV_ON;
    }
    
    if(dispPrm->u4DSclV == DFT_DS_FACTOR)
    {
        bVDSBypass = SV_ON;
    }
    
    #if 1
    //local test code,auto tune fail when VDS_BYPASS=1
    if((u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_AUTO) || (u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_MANUAL))
    {
        bVDSBypass = 0;
        bVUSBypass = 0;
    }
    #endif
    _vScpipSetBypassUpScaler(bPath, bVUSBypass);
    _vScpipSetBypassDownScaler(bPath, bVDSBypass);    
    
    //3. configure US_VOFST & PRE_VOFST
    _vScpipSetDEAlignmentReg(bPath);
}

/**
 * @brief Set DE alignment related registers
 * @param index bypass status
 * @6896 porting done
 */
static void _vScpipSetDEAlignmentReg(UINT8 bPath)
{
    UINT32 u4PanelVOfst;

    if(fgIsMJCToOSTG()==SV_TRUE)
    {
        //MJC & PSC is NOT bypassed. Panel vofst set to smaller value to avoid frame track vertical shaking issue.
        u4PanelVOfst = u4ScpipGetPanelVOfstVar();
    }
    else
    {
        //MJC & PSC is bypassed. Panel vofst set to panel v active.
        u4PanelVOfst = wDISPLAY_HEIGHT;        
    }
    
    if(bPath == SV_VP_MAIN)
    {
        //@6896 todo: add PR+up-scaling mode setting
        if(u4ScpipGetTDTVPRUpFlag(getScalerVDP(bPath)) == SV_TRUE)
        {
            //for PR+up case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x9, PIP_OA_05_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03, (u4PanelVOfst - 0x9), PIP_OA_03_PRE_VOFST1);            
        }
        else
        {
            //for non-bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x5, PIP_OA_05_US_VOFST1);
            vRegWriteFldAlign(SCPIP_PIP_OA_03, (u4PanelVOfst - 0x5), PIP_OA_03_PRE_VOFST1);
        }

        //for bypass case
        vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x1, PIP_OA_05_US_VOFST_BYPASS1);
        vRegWriteFldAlign(SCPIP_PIP_OA_03, (u4PanelVOfst - 0x1), PIP_OA_03_PRE_VOFST_BYPASS1);

        //panel v ofst
        vRegWriteFldAlign(SCPIP_PIP_OA_0A, u4PanelVOfst, PIP_OA_0A_PANEL_VOFST);
        // Set OSD_VOFST
        if((u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_AUTO) || (u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_MANUAL))
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_0A, u4PanelVOfst - 4, PIP_OA_0A_OSD_VOFST);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_0A, u4PanelVOfst - 5, PIP_OA_0A_OSD_VOFST);
        }
    }
    else if(bPath == SV_VP_PIP)
    {
        UINT8 tveEnable;
        TVE_GetEnable(TVE_1, &tveEnable);

        if(tveEnable == SV_TRUE)
        {
            // enlarge v-porch 33 line
            //for non-bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x1, PIP_OA_05_US_VOFST2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, 33, PIP_OA_04_PRE_VOFST2_LSB);

            //for bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x1, PIP_OA_05_US_VOFST_BYPASS2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, 33, PIP_OA_04_PRE_VOFST_BYPASS2_LSB);                
        }
        else
        {
            #if defined(CC_MT5396)
        	//for sub panelvofst add 3
            //for non-bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x2, PIP_OA_05_US_VOFST2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, (u4PanelVOfst + 0x3 - 0x2), PIP_OA_04_PRE_VOFST2_LSB);

            //for bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x1, PIP_OA_05_US_VOFST_BYPASS2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, (u4PanelVOfst + 0x3 - 0x1), PIP_OA_04_PRE_VOFST_BYPASS2_LSB);    
            #else
            //for sub panelvofst add 3
            //for non-bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x2, PIP_OA_05_US_VOFST2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, (u4PanelVOfst + 0x2 - 0x2), PIP_OA_04_PRE_VOFST2_LSB);

            //for bypass case
            vRegWriteFldAlign(SCPIP_PIP_OA_05, 0x1, PIP_OA_05_US_VOFST_BYPASS2);
            vRegWriteFldAlign(SCPIP_PIP_OA_04, (u4PanelVOfst + 0x2 - 0x1), PIP_OA_04_PRE_VOFST_BYPASS2_LSB);  
            #endif
        }        
    }
}

/**
 *  @6896 new
 */
static UINT8 _bScpipGetUpScalerVTap(void)
{
    UINT8 bDispMode;
    UINT8 ret = UPSCALER_6TAP;  //default is 6 taps
    bDispMode = getScalerMode(SV_VP_MAIN);

    //only scaler dispmode use 6+ taps
    if(bDispMode == VDP_SCPOS_DISPMODE_AUTO || bDispMode == VDP_SCPOS_DISPMODE_MANUAL)
    {
        ret = UPSCALER_6PLUS_TAP;
    }
    
    return ret;
}

/**
 *  @6896 new
 */
void vScpipInitFilterOff(void)
{
    //only set Horizontal Filter off. Because 5396 Didn't implement Vertical Filter off function
    vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_HORIZONTAL_FILTER_OFF_1);    
}

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
    if(scalerMode != VDP_SCPOS_DISPMODE_OFF)
    {       
        ret = SV_FALSE;
    }

    return ret;
}

/**
 * @brief Set up scaler bypass
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bMode
 * @6896 porting done
 */
static void _vScpipSetBypassUpScaler(UINT8 bPath,UINT8 bMode)
{
    UINT16 offset;
    
    offset = getUSOffset(bPath);

    vRegWriteFldAlign(SCPIP_US1_01+offset, (bMode==SV_ON)?1:0, US1_01_VUS_BYPASS_1);
}

/**
 * @brief Set down scaler bypass
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bMode
 * @6896 porting done
 */
static void _vScpipSetBypassDownScaler(UINT8 bPath,UINT8 bMode)
{
    UINT16 offset = getDSOffset(bPath);

    vRegWriteFldAlign(SCPIP_DS1_02+offset, (bMode==SV_ON)?1:0, DS1_02_VDS_BYPASS_1);
}

/**
 * @brief vScpipSetFreeze
 * Freeze Video in scalar
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 * @6896 porting done
 */
void vScpipSetFreeze(UINT8 bPath, UINT8 bOnOff) 
{
    CRIT_STATE_T csState;
    UINT8 freezeDelay=0;
    RVChannel* channel;

    csState = x_crit_start();

    channel = getChannel(bPath);

    if (bPath == SV_VP_MAIN) 
    {
#if (MAIN_FREEZE_DELAY)
        freezeDelay = MAIN_FREEZE_DELAY;
#else
        freezeDelay = 0;
#endif
    }
    else if(bPath == SV_VP_PIP)
    {
#if (PIP_FREEZE_DELAY)
        freezeDelay = PIP_FREEZE_DELAY;
#else
        freezeDelay = 0;
#endif
    }        

    if(bOnOff == SV_ON)
    {
        channel->bIsGoingToFreeze = freezeDelay;
    }
    else
    {
        channel->bIsGoingToFreeze = 0;
        channel->bIsFreeze = 0;
    }

    x_crit_end(csState);
}

/**
 * @brief Handle channel freeze
 * @param u4VdpId VDP_1/VDP_2
 */
static void _vScpipHandleChannelFreeze(UINT32 u4VdpId)
{  
    UINT8 bPath;   
    RVChannel* channel;
    UINT8 isChannelOn;

    bPath = getScalerPath(u4VdpId);
    channel = getChannel(bPath);
    isChannelOn = channel->bIsChannelOn;

    if(isChannelOn == SV_ON && channel->bIsGoingToFreeze!=0 && _arScalePrm[u4VdpId].u4Enable)
    {    
        // wait for some VSync to fill the whole scaler frame buffers before freezing
        UINT8 bMute;
        LOG(3, "Channel is going to freeze: %d\n", channel->bIsGoingToFreeze);
        bMute =  ((u4VdpId == VDP_1) ? fgMainMuteOn() : fgPIPMuteOn());

        if (bMute)
        {
            LOG(3, "Channel is still in mute mode\n");
        }
        else
        {
            channel->bIsGoingToFreeze --;
            if (channel->bIsGoingToFreeze == 0)
            {
                LOG(3, "Channel freeze on\n");
                channel->bIsFreeze = SV_ON;
            }
        }
    }    
}

/**
 * @brief Set pre-scaling down factor
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Factor pre-scaling down factor
 * @6896 porting done
 */
void vScpipSetForcedHpsFactor(UINT32 u4VdpId,UINT32 u4Factor) 
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    _arScalePrm[u4VdpId].u4ForcedPreScalingactor = u4Factor;

    vScpipReconfig(u4VdpId);
}

/**
 *  @6896 review done
 */
UINT16 u2ScpipGetDispWidth(UINT8 bPath)
{    
    RDispPrm* dispPrm;    
    dispPrm = getDispPrm(bPath);    
    return dispPrm->wRHLen;
}
/**
 *  @6896 review done
 */
UINT16 u2ScpipGetDispHeight(UINT8 bPath)
{            
    RDispPrm* dispPrm;            
    dispPrm = getDispPrm(bPath);            
    return dispPrm->wRVLen;
}

/**
 *  @6896 review done
 */
void vScpipForceDispOff(UINT32 u4Path, UINT32 u4OnOff)
{
    if (_u4ForceDispOff[u4Path] != u4OnOff)
    {
        _u4ForceDispOff[u4Path] = u4OnOff;
    }
}

/**
 * @brief Set mode change flag
 * @param ucVdpId VDP_1/VDP_2
 * @6896 review done
 */
void vScpipReconfig(UINT32 u4VdpId)
{
    if (u4VdpId == VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);
    }
    else if (u4VdpId == VDP_2)
    {
        vSetScposFlg(PIP_DISP_PRM_CHG);
    }

    #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscFireModeChanged();
    #endif
}

/**
 * @brief Get pre-scaling down factor
 * @param ucVdpId VDP_1/VDP_2
 * @return pre-scaling down factor
 * @6896 porting done
 */
UINT32 u4ScpipGetHorizontalPreScaleDownFactor(UCHAR ucVdpId)
{
    return _arScalePrm[ucVdpId].u4HpsFactor;
}

/**
 *  @6896 review done
 */
UINT32 u4ScpipGetFrameDelay(UINT32 u4VdpId)
{
    UINT32 u4Delay = 0;
    UINT32 u4Mode;
    
    u4Mode = u1ScpipGetDispMode(getScalerPath(u4VdpId));

    if(u4Mode != VDP_SCPOS_DISPMODE_OFF)        // display mode on, there is no frame delay
    {
        u4Delay= 0;
    }
    else                                        // scaler dram mode
    {
        if(u4ScpipGetBufNum(u4VdpId) <= 2)      // 2 (or 1) frame mode
        {
            u4Delay= 10;  //delay 1 frame
        }
        else                                    // 3 frame mode
        {
            u4Delay= 15; // delay 1.5 frame
        }
    }

    return u4Delay;    // (*10) request from cook.
}

/**
 *  @6896 review done
 */ 
void vScpipSetHNegInitPhaseOnOff(const BOOL fgOnOff)
{
    if(_fgIsHNegInitPhaseOn != fgOnOff)
    {
        _fgIsHNegInitPhaseOn = fgOnOff;
        LOG(3, "[Scpip] Set horizontal negative initial phase: (%d)\n", fgOnOff);
    }
}
/**
 *  @6896 review done
 */
void vScpipSetVNegInitPhaseOnOff(const BOOL fgOnOff)
{
    if(_fgIsVNegInitPhaseOn != fgOnOff)
    {
        _fgIsVNegInitPhaseOn = fgOnOff;
        LOG(3, "[Scpip] Set vertical negative initial phase: (%d)\n", fgOnOff);
    }
}

/**
 * @brief Set up scaler related registers
 * @param u4VdpId VDP_1/VDP_2
 * @6896 porting done
 */
static void _vScpipSetUSInitialFactor(const UINT32 u4VdpId)
{
    RDispPrm* dispPrm;
    UINT8 bPath;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);    

    if(u4VdpId == VDP_1)
    {
        //H init & H sign
        vRegWriteFldAlign(SCPIP_US1_12,dispPrm->u4USclHInit,US1_12_US_H_INIT_1);
        vRegWriteFldAlign(SCPIP_US1_11,dispPrm->u4USclHInitUV,US1_11_US_H_INIT_UV_1);
        vRegWriteFldAlign(SCPIP_US1_11,dispPrm->u1USclHInitSigned,US1_11_US_H_INIT_SIGNED_1);
        //V init & V sign
        vRegWriteFldAlign(SCPIP_US1_13,dispPrm->u1USclV3DFieldEn,US1_13_US_3D_FIELD_EN);
        
        vRegWriteFldAlign(SCPIP_US1_12,dispPrm->u4USclVInit,US1_12_US_V_INIT_1);
        vRegWriteFldAlign(SCPIP_US1_14,dispPrm->u4USclVInit,US1_14_US_V_INIT_R_T_1);
        vRegWriteFldAlign(SCPIP_US1_14,dispPrm->u4USclVInit_Btm,US1_14_US_V_INIT_L_B_1);
        vRegWriteFldAlign(SCPIP_US1_15,dispPrm->u4USclVInit_Btm,US1_15_US_V_INIT_R_B_1);
        
        vRegWriteFldAlign(SCPIP_US1_11,dispPrm->u1USclVInitSigned,US1_11_US_V_INIT_SIGNED_1);
        vRegWriteFldAlign(SCPIP_US1_13,dispPrm->u1USclVInitSigned,  US1_13_US_V_INIT_SIGNED_R_T_1);
        vRegWriteFldAlign(SCPIP_US1_13,dispPrm->u1USclVInitSign_Btm,US1_13_US_V_INIT_SIGNED_L_B_1);
        vRegWriteFldAlign(SCPIP_US1_13,dispPrm->u1USclVInitSign_Btm,US1_13_US_V_INIT_SIGNED_R_B_1);
        
    }
    else
    {
        ASSERT(0); // only support main path negative initial phase
    }
}

/**
 * @brief Calculate negative initial phase value
 * @param u4InW  Input width before up-scale
 * @param u4OutW Output width after up-scale
 * @param u4UScl Up-scaling factor
 * @return Negative initial phase value
 * @6896 porting done
 */
static UINT32 _u4ScpipCalNegInitPhase(const UINT32 u4InW, const UINT32 u4OutW, const UINT32 u4UScl)
{
    UINT32 u4Diff = 0;
    UINT32 u4InitPhase = 0x0;

    ASSERT(u4UScl <= DFT_US_FACTOR);

    if((u4UScl == DFT_US_FACTOR) || (_arScalePrm[VDP_1].u4NonlinearEn && (_arScalePrm[VDP_1].u4NonlinearCtrl!= 0))) // no up-scale or non_linear off
    {
        return 0;
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
            return 0;
        }
    }  
}

/**
 * @brief Set up scaler related registers
 * @param u4VdpId VDP_1/VDP_2
 * @6896 porting done
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
    UINT16 usOffset;
    UINT8 bPath;

    bPath = getScalerPath(u4VdpId);
    usOffset = getUSOffset(bPath);
    channel = getChannel(bPath);
    dispPrm = getDispPrm(bPath);

    /* Up Scaler for Main */
    if (channel->bIsChannelOn != SV_OFF)
    {  
        //update V up-factor
        vRegWriteFldAlign(SCPIP_US1_00+usOffset,dispPrm->u4USclV,US1_00_USCALE_V_1);
        
        //update H up-factor & nonlinear factor
        if ((_arScalePrm[u4VdpId].u4NonlinearCtrl == 0) || (vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE))
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
                    vRegWriteFldAlign(SCPIP_US1_17, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4MiddlePoint, US1_17_NONL_H_MIDDLE_1);
                    vRegWriteFldAlign(SCPIP_US1_17, _arScalePrm[u4VdpId].rNewNonlinearPrm.u1Step, US1_17_NONL_H_STEP_1);
                    vRegWriteFldAlign(SCPIP_US1_17, _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StopPoint, US1_17_NONL_H_STOP_POINT_1);                            
                }
                else // old non-linear (convert register value to new HW)
                {
                    static const UINT32 _au4StepFactorTable[16] = {0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512};            
                    UINT32 u4StartFactor = (_arScalePrm[u4VdpId].rNonlinearPrm.u4Start << 4);
                    UINT32 u4EndFactor = (_arScalePrm[u4VdpId].rNonlinearPrm.u4End << 4);
                    UINT32 u4StepFactor = _au4StepFactorTable[_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope];
                    UINT32 u4StopPoint = (u4EndFactor - u4StartFactor)/u4StepFactor;
                    UINT32 u4MiddlePoint = _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint << 2;
                    if(((u4MiddlePoint - u4StopPoint)* 2 * u4EndFactor/32768) > _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth)//when the width after hps and overscan is smaller than linear width, is not support nonlinear
                    {
                        vRegWriteFldAlign(SCPIP_US1_00, dispPrm->u4USclH,US1_00_USCALE_H_1);
                        LOG(3,"Nonlinear not support!!!!!!! source middle(%d),u4HpsAndOverscanedWidth(%d)\n",((u4MiddlePoint - u4StopPoint)*u4EndFactor/32768),_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth);
                        vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONL_H_USCALE_1); 
                        //vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_H_NEG_1); // always keep zero
                        vRegWriteFldAlign(SCPIP_US1_17, 0, US1_17_NONL_H_MIDDLE_1);
                        vRegWriteFldAlign(SCPIP_US1_17, 0, US1_17_NONL_H_STEP_1);
                        vRegWriteFldAlign(SCPIP_US1_17, 0, US1_17_NONL_H_STOP_POINT_1);  
                    }
                    else
                    {
                        vRegWriteFldAlign(SCPIP_US1_00, u4StartFactor,US1_00_USCALE_H_1);
                        vRegWriteFldAlign(SCPIP_US1_01, u4StepFactor, US1_01_NONL_H_USCALE_1); 
                        //vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_H_NEG_1); // always keep zero
                        vRegWriteFldAlign(SCPIP_US1_17, u4MiddlePoint, US1_17_NONL_H_MIDDLE_1);
                        vRegWriteFldAlign(SCPIP_US1_17, 1, US1_17_NONL_H_STEP_1);
                        vRegWriteFldAlign(SCPIP_US1_17, u4StopPoint, US1_17_NONL_H_STOP_POINT_1);
                    }
                }
            }
        }   
    }
}



/**
 * @brief Get pre-scaling down width
 * @param bPath VDP_1/VDP_2
 * @return pre-scaling down width
 * @6896 porting done
 */
UINT32 u4ScpipGetPreScaleWidth(UINT8 bPath)
{
    UINT16 u2Width;
    UINT32 u4Factor;
    RPicInfo* picInfo;
    
    picInfo = getPicInfo(bPath);
    u4Factor = u4ScpipGetHwHPS(bPath);
    
    if (u4Factor == DFT_HPD_FACTOR)
    {
        u2Width = picInfo->wSrcW;
    }
    else
    {
        u2Width = (((UINT32)(picInfo->wSrcW+1) * u4Factor)/ DFT_HPD_FACTOR);
    }
    #ifdef IS_SUPPORT_3D_PHOTO
    if(_fgFixRegion && (bPath == VDP_1))
        {
            u2Width = wDrvVideoInputWidth(bPath);
        }
    #endif
    return u2Width;
}

/**
 * @brief Set dot by dot mode
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @6896 Review done
 */
void vScpipSet121Map(UCHAR ucVdpId, UCHAR ucOnOff)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    LOG(2, "Set 121 Map to (%d)\n",ucOnOff);
    if (_arScalePrm[ucVdpId].u4121Map != (UINT32) ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4NonlinearEn = 0;
        }

        _arScalePrm[ucVdpId].u4121Map = (UINT32) ucOnOff;

        vScpipReconfig((UINT32)ucVdpId);
    }
}

/**
 * @6896 Review done
 */
UINT32 vScpipGet121Map(UCHAR ucVdpId)
{
    return _arScalePrm[ucVdpId].u4121Map;
}


void vScpipSet121MapHPos(UCHAR ucVdpId, UINT16 u2HPos)
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    if(_arScalePrm[ucVdpId].u2121MapHPos != u2HPos)
    {
    	_arScalePrm[ucVdpId].u2121MapHPos = u2HPos;
   	vScpipReconfig((UINT32)ucVdpId);
    }
}


/**
 * @brief Set down scale factor
 * @param u4VdpId VDP_1/VDP_2
 * @6896 review done
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
    UINT16 dsOffset;
    UINT8 bPath;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);

    /* Set Down Scaling factor */
    //if ds factor is 0, dram read/write will be hold and scaler register will not effect
    if(dispPrm->u4DSclV == 0)
    {
        LOG(1, "DEBUG [%d] DS Vfactor = 0\n", u4VdpId);
        dispPrm->u4DSclV = 0x8000;
    }
    if(dispPrm->u4DSclH== 0)
    {
        LOG(1, "DEBUG [%d] DS Hfactor = 0\n", u4VdpId);
        dispPrm->u4DSclH = 0x8000;
    }
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, dispPrm->u4DSclH, DS1_00_DSCALER_H_1);
    vRegWriteFldAlign(SCPIP_DS1_00+dsOffset, dispPrm->u4DSclV, DS1_00_DSCALER_V_1);    
}

/**
 * @brief Set initial down scale factor
 * @param u4VdpId VDP_1/VDP_2
 * @6896 review done
 */
static void _vScpipSetDSInitialFactor(UINT32 u4VdpId) 
{
    UINT16 dsOffset;
    UINT8 bPath;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    
	//In 5368,5396, set to 0x8000 means ds init factor will be calculated automatically.   
    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, 0x8000, DS1_01_DSCALE_HOFST_1);
    vRegWriteFldAlign(SCPIP_DS1_01+dsOffset, 0x8000, DS1_01_DSCALE_VOFST_1);    
}

static UINT8 _vScpicIsOneToOneAble(UINT32 u4VdpId)
{
    UINT8 ret = SV_TRUE;
    UINT8 bPath;
    UINT32 u4ScalerMode;
    bPath = getScalerPath(u4VdpId);
    u4ScalerMode = u1ScpipGetDispMode(bPath);    

    if(u4ScalerMode == VDP_SCPOS_DISPMODE_AUTO || 
        u4ScalerMode == VDP_SCPOS_DISPMODE_MANUAL ||
        (u4ScalerMode == VDP_SCPOS_LINESYNC_DISPMODE && vScpipIs3DDoneByMJC()==SV_FALSE))    // display mode
    {
        ret = SV_FALSE;
    }
    #if 0
    else if(dispInfo->wWidth < picInfo->wTargetW || 
            dispInfo->wHeight < picInfo->wTargetH ||
            _arScalePrm[u4VdpId].u4MddiFifoMax < picInfo->wSrcW)        //output region < input region
    {
        ret = SV_FALSE;
    }
    else if(bDrvVideoGetWidthOverSample(u4VdpId) && dispInfo->wWidth < (picInfo->wTargetW >> 1))    //oversample case
    {
        ret = SV_FALSE;
    }
    #endif    
    
    return ret;
}

static void _vScpipUpdate121MapEn(UINT32 u4VdpId)
{   
    // handle 121-mapping
    _arScalePrm[u4VdpId].u4121MapEn = SV_OFF;
    if(_arScalePrm[u4VdpId].u4121Map == SV_ON)
    {
        if(_vScpicIsOneToOneAble(u4VdpId) == SV_TRUE)
        {
            _arScalePrm[u4VdpId].u4121MapEn = SV_ON;
            LOG(3,"121 is True and can be enabled.\n");
        }
        else
        {
            LOG(3,"Unable to do 121\n");
        }        
    }
}

static void _vScpipUpdateMddiFifoInfo(UINT32 u4VdpId)
{
    UINT8 bPath;
    RPicInfo* picInfo;
    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;

    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);

    // Check NR/PSCAN fifo limit
    // _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth was updated in vDrvScpipSrmDispCb()
    if (_arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth != SCPOS_BYPASS_PSCANNR)
    {
        u4MddiFifoMin = u4DrvDIMiniInputWidth(u4VdpId);
        u4MddiFifoMax = u4DrvDIMaxInputWidth(u4VdpId);

        if (_arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth < u4MddiFifoMax)
        {
            u4MddiFifoMax = _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth;
            LOG(3, "DI DRAM Limit (%d)\n", _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth);        
        }    
    }
    else
    {                
        u4MddiFifoMin = 0;
        u4MddiFifoMax = 2000;
    }      
    LOG(3, "DI Fifo(%d) range (%d - %d)\n", u4VdpId, u4MddiFifoMin, u4MddiFifoMax);

    // First Config #3
    if ((_arScalePrm[u4VdpId].u4MddiFifoMax != u4MddiFifoMax) ||
        (_arScalePrm[u4VdpId].u4MddiFifoMin != u4MddiFifoMin) ||
        (picInfo->u1Interlace != bDrvVideoIsSrcInterlace(u4VdpId)))
    {
        _arScalePrm[u4VdpId].u4FirstConfig = 1;
        _arScalePrm[u4VdpId].u4MddiFifoMax = u4MddiFifoMax;
        _arScalePrm[u4VdpId].u4MddiFifoMin = u4MddiFifoMin;

        LOG(3, "1st Config#3\n");
    }    
}

/**
 * @brief Fix pre-scaling down factor
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @param uc1stConfig configure paramter
 * @6896 review done
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
            // re-scale if NOT optimized!
            vScpipReconfig((UINT32)ucVdpId);
        }

        _arScalePrm[ucVdpId].u4FixedPreScaler = (UINT32) ucOnOff;
    }

    _arScalePrm[ucVdpId].u4FirstConfig = (UINT32) uc1stConfig;
}

static void _vScpipUpdateHPSInfo(UINT32 u4VdpId)
{
    UINT32 u4MddiFifoMin;
    UINT32 u4MddiFifoMax;
    UINT32 u4SrcWidth;
    UINT32 u4OutWidth;
    
    UINT8 bPath;
    RVChannel* channel;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;
    UINT8 decType;          //decoder type
    UINT32 u4ScalerMode;
    
    //prepare data
    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);    
    channel = getChannel(bPath);
    
    decType = channel->bDecType;
    u4ScalerMode = u1ScpipGetDispMode(bPath);
    
    u4MddiFifoMin = _arScalePrm[u4VdpId].u4MddiFifoMin;
    u4MddiFifoMax = _arScalePrm[u4VdpId].u4MddiFifoMax;
    u4SrcWidth = picInfo->wTargetW;
    u4OutWidth = dispInfo->wWidth;
    
    // calculate H pre-down factor
    _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
    _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = u4SrcWidth;

    //error protection
    // window width or height is 0, no need to calculate hps factor
    if((u4SrcWidth == 0) || (picInfo->wSrcW == 0) || (u4OutWidth == 0))
    {
        return;
    }
    
    // 1. forced pre-down factor
    if (_arScalePrm[u4VdpId].u4ForcedPreScalingactor != 0)
    {
        _arScalePrm[u4VdpId].u4HpsFactor = _arScalePrm[u4VdpId].u4ForcedPreScalingactor;
        _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = DIVIDE_WITH_ROUND((u4SrcWidth * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
        LOG(1, "Force PreScale to %d\n", _arScalePrm[u4VdpId].u4ForcedPreScalingactor);
    }
    #ifdef CC_SUPPORT_PRESCALE_DOWN
    // 2. oversample case, pre-down to half size
    else if (( decType == (UINT8) SV_VD_YPBPR || decType == (UINT8) SV_VD_DVI) && 
            bDrvVideoGetWidthOverSample(u4VdpId))
    {
        // 1440 >> 720
        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_HPD_FACTOR >> 1);
        _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = DIVIDE_WITH_ROUND((u4SrcWidth * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);        
    }
    #endif
    else
    {
        if (u4OutWidth >= u4SrcWidth) //==> do up-scaling
        {
            //if MddiFifoMax is smaller than input source width, have to do predown....
            if (picInfo->wSrcW > u4MddiFifoMax)
            {
                //pre-down from source width (ex: 1280) to MddiFiFoMax (ex:960)
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_HPD_FACTOR * u4MddiFifoMax)/ picInfo->wSrcW;
                LOG(1, "Two Scaling! (HPSDOWN + UP) \n");
                // Calculate Source Width After PreScale Down & overscan
                _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = DIVIDE_WITH_ROUND((u4SrcWidth * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
            else
            {
                //don't have to do predown
                _arScalePrm[u4VdpId].u4HpsFactor = DFT_HPD_FACTOR;
                _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = u4SrcWidth;
            }
        }
        else
        {
            // Down Scale
            UINT32 u4PredownSize;

            /* For down scaling case, we hope to do most of h-down in pre-down scaler.
             * We hope after pre-down and overscan. the size is exactly same with output window size.
             * For example, SrcWidth = 1280, Overscaned SrcWidth = 1200, OutputWidth = 960.
             * Pre-down size should fulfill this equation: (1200/1280) = (960/predownSize)
             * predownSize = 1280 * 960 / 1200 = 1024.
             * After overscan, width change from 1024 to 960. Don't have to do Down & Up.
             */            
            // (u4SrcWidth/ picInfo->wSrcW) = (u4OutWidth / pre-down size)
            u4PredownSize = DIVIDE_WITH_ROUND((picInfo->wSrcW * u4OutWidth), u4SrcWidth);
            
            //make it to even number
            u4PredownSize = u4PredownSize + (u4PredownSize%2);

            if (u4MddiFifoMax >= u4PredownSize)
            {
                // Do PreScale Down First       
                if ((u4PredownSize < u4MddiFifoMin) && (u4MddiFifoMin > 0))
                {                                
                    //if (u4MddiFifoMin >= u4TargetW)
                    if (u4MddiFifoMin >= u4SrcWidth)    
                    {
                        LOG(1, "Warnning: Input width is already more than DI min FIFO limitation! (%d, %d)\n", u4SrcWidth, u4MddiFifoMin); 
                        _arScalePrm[u4VdpId].u4HpsFactor = DFT_DS_FACTOR;
                        _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = u4SrcWidth;
                    }
                    else //ex: srcW = 200, MddiFifoMin = 180, OutW = 100.
                    {                        
                        LOG(1, "Two Scaling! (DOWN + DOWN) MIN#0\n");                     
                        //_arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMin)/ u4TargetW;
                        _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMin)/ u4SrcWidth;                        
                        // Calculate Source Width After PreScale Down
                        _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = u4MddiFifoMin;                        
                    }
                }
                else
                {
                    //just predown to predown size
					_arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4OutWidth)/ u4SrcWidth;
                    // Calculate Source Width After PreScale Down
                    _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = u4OutWidth;
                }
            }
            else /* u4PredownSize > u4MddiFifoMax, means have to predown to u4MddiFifoMax and then down scale to output size */
            {
                //pre-down from source width to MddiFifoMax
                _arScalePrm[u4VdpId].u4HpsFactor = (DFT_DS_FACTOR * u4MddiFifoMax)/ picInfo->wSrcW;
                LOG(1, "Two Scaling! (DOWN + DOWN) \n");
                // Calculate Source Width After PreScale Down & overscan
                _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth = DIVIDE_WITH_ROUND((u4SrcWidth * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
            }
        }
         
        if (_arScalePrm[u4VdpId].u4FixedPreScaler != 0)
        {
            UINT32 u4HpsFactor = 0;
            
            u4HpsFactor = u4ScpipGetHwHPS(bPath);            
            
            if (u4HpsFactor != _arScalePrm[u4VdpId].u4HpsFactor)            
            {
                /*_vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                            (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),TRUE);*/
                //if Pre-scaling down factor changed, fire scaler mode change to enable scaler to recalculate parameter of pscan dispmode
                if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE)
                {
                    LOG(4,"HPS changed ==> fireSCPOSModeChange\n");
                    fireSCPOSModeChange(u4VdpId);
                }                
            }
        }
    }
    /* Horizontal Direction */
    // scaler only handle even width because of h boundary
    if((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth % 2) && !bIsScalerInput444(u4VdpId))
    {
        _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth += 1;
    }
    //calculate predown width
    _arScalePrm[u4VdpId].u4HpsWidth = DIVIDE_WITH_ROUND((picInfo->wSrcW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
    if((_arScalePrm[u4VdpId].u4HpsWidth % 2) && !bIsScalerInput444(u4VdpId))
    {
        _arScalePrm[u4VdpId].u4HpsWidth += 1;
}

    if (_arScalePrm[u4VdpId].u4HpsWidth < _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth)
    {
        LOG(0, "HPS %d < overscan %d \n", _arScalePrm[u4VdpId].u4HpsWidth, _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth);
        _arScalePrm[u4VdpId].u4HpsWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    }
}

/**
 * @brief Set 10/8 bit mode
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Mode 10/8 bit, or 0 means not forced.
 * @6896 porting done
 */
void _VDP_Set10bitMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (_arScalePrm[u4VdpId].u4Forced10BitMode != u4Mode)
    {
        _arScalePrm[u4VdpId].u4Forced10BitMode = u4Mode;

        vScpipReconfig(u4VdpId);
    }
}

/**
 *  @6896 porting done
 */
UINT32 u4ScpipGetForced10BitMode(UINT32 u4VdpId)
{
    return _arScalePrm[u4VdpId].u4Forced10BitMode;
}

/**
 *  @6896 porting done
 */
UINT32 u4ScpipGet10BitMode(UINT32 u4VdpId)
{
    UINT32 u410BitMode = 0xffffffff;
    UINT32 u4Forced10BitMode;

    u4Forced10BitMode = u4ScpipGetForced10BitMode(u4VdpId);

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

/**
 *  @6896 porting done
 */
static void _vScpipSetDataFormatReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dsOffset;
    UINT8 bIs444;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dsOffset = getDSOffset(bPath);
    dispPrm = getDispPrm(bPath);
    
    /* 1. set 8 bit/10bit */
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, _arScalePrm[u4VdpId].u410BitMode, DS1_02_VD10BSEL_1);

    /* 2. Set 444/422 format */
    bIs444 = _arScalePrm[u4VdpId].u4Is444;
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, bIs444, DS1_02_SEL422TO444_1);
    vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, bIs444, DS1_02_SEL444_1);

    /* 3. set US422 register */
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_US1_1E, _arScalePrm[u4VdpId].u4Is422Up, US1_1E_US422_MODE);
        vRegWriteFldAlign(SCPIP_US1_1E, dispPrm->u1Us422UVMirrorMode, US1_1E_US422_UV_MIRROR_MODE);
        #if defined(CC_MT5389)
        vRegWriteFldAlign(SCPIP_US1_2C, _arScalePrm[u4VdpId].u4Is422Up, US1_2C_US422_CHROMA_AVG);
        #endif
    }
}

static void _vScpipSetDisplayWindowReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    RDispPrm* dispPrm;
#if defined(CC_FLIP_MIRROR_SUPPORT)
    UINT8 u1Config;
#endif
        
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    pipOffset = getPIPOffset(bPath);
       
    /* RVP/RHP/RVL/RHL -- Display Window Position and Size */
#if defined(CC_FLIP_MIRROR_SUPPORT)
    u1Config = u1GetFlipMirrorConfig();
    if(u4GetFlipMirrorModule(VDP_1) !=FLIP_BY_MJC)
    {
        if (u1Config & SYS_MIRROR_CONFIG_ON)
        {
            dispPrm->wRHPos = (wDISPLAY_WIDTH > (dispPrm->wRHPos+ dispPrm->wRHLen))? (wDISPLAY_WIDTH - dispPrm->wRHPos - dispPrm->wRHLen): 0;
        }
        if (u1Config & SYS_FLIP_CONFIG_ON)
        {
            dispPrm->wRVPos = (wDISPLAY_HEIGHT > (dispPrm->wRVPos+ dispPrm->wRVLen))? (wDISPLAY_HEIGHT - dispPrm->wRVPos - dispPrm->wRVLen): 0;
        }
    }
#endif

    #if defined(SUPPORT_TV3D_OVERSCAN) || (defined(SUPPORT_3D_OVSN_BY_SCL) && defined(SUPPORT_3D_H_OVSN_BY_PSC))
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, 0, PIP1_03_RHP_1);
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, 0, PIP1_02_RVP_1);
    }
    else
    #endif
    {
        vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, dispPrm->wRHPos, PIP1_03_RHP_1);
        vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, dispPrm->wRVPos, PIP1_02_RVP_1);    
    }
    
    vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, dispPrm->wRHLen, PIP1_03_RHL_1);
    vRegWriteFldAlign(SCPIP_PIP1_02+pipOffset, dispPrm->wRVLen, PIP1_02_RVL_1);
}

static void _vScpipSetDramRWLengthReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset;
    UINT16 pipOffset;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);

    //1. Dram Write Width
    vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->u2DramWriteWidth,PIP1_00_DRAM_WR_WIDTH_H_1);
    //2. Dram Write Height
    vRegWriteFldAlign(SCPIP_PIP1_00+pipOffset, dispPrm->u2DramWriteHeight,PIP1_00_DRAM_WR_WIDTH_V_1);    
    //3. Dram Read Width
    vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->u2DramReadWidth,PIP1_01_DRAM_RD_WIDTH_H_1);            
    //4. Dram Read Height
    vRegWriteFldAlign(SCPIP_PIP1_01+pipOffset, dispPrm->u2DramReadHeight,PIP1_01_DRAM_RD_WIDTH_V_1);
    //5. Dram Line picth
    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        SCALER_TDTV_IN_TYPE in = eScpipGetTdtvInType(u4VdpId);
        SCALER_TDTV_OUT_TYPE out = eScpipGetTdtvOutType(u4VdpId);
        
        if(in == SCALER_TDTV_IN_TYPE_SBS)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, (dispPrm->u2DramLinePitch +1)/2,DRAM_M_00_DA_DRAM_LINE_PITCH);
        }
        else if(out == SCALER_TDTV_OUT_TYPE_SBS)
        {
            // when SBS output, the max value of line pitch is 960/16=60
            vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, MIN(60,dispPrm->u2DramLinePitch),DRAM_M_00_DA_DRAM_LINE_PITCH);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, dispPrm->u2DramLinePitch,DRAM_M_00_DA_DRAM_LINE_PITCH); 
        }
     //   vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, ((in == SCALER_TDTV_IN_TYPE_SBS) || (out == SCALER_TDTV_OUT_TYPE_SBS))?(dispPrm->u2DramLinePitch +1)/2:dispPrm->u2DramLinePitch, DRAM_M_00_DA_DRAM_LINE_PITCH);      
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, dispPrm->u2DramLinePitch,DRAM_M_00_DA_DRAM_LINE_PITCH);  
    }          
}
static void _vScpipSetHPorchReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    UINT16 dramOffset;
    RDispPrm* dispPrm;
    #ifdef IS_SUPPORT_3D_PHOTO
    TDTV_3D_CROP_INTO_T* eCropInfo;
    UINT8 bMode;
    #endif
    
    #if defined(SUPPORT_3D_OVSN_BY_SCL)
    UINT32 u4TTD;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
    
    u4TTD = u4DrvTDTVTTDModeQuery();
    eTTDIn  = TD_TTD_IN(u4TTD);
    eTTDOut = TD_TTD_OUT(u4TTD);    
    #endif

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dramOffset = getDramOffset(bPath);
    dispPrm = getDispPrm(bPath);
    #ifdef IS_SUPPORT_3D_PHOTO
    eCropInfo = ScpipGet3DCropInfo();
    bMode = u1ScpipGetDispMode(bPath);    
    #endif

    #if defined(SUPPORT_3D_OVSN_BY_SCL) && defined(SUPPORT_3D_H_OVSN_BY_FSC)
    if(eScpipGetTdtvInType(u4VdpId) == SCALER_TDTV_IN_TYPE_FS && 
        eScpipGetTdtvOutType(u4VdpId) == SCALER_TDTV_OUT_TYPE_SG)   //input FS & output FS
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);    
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        #ifdef IS_SUPPORT_3D_PHOTO
           vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (dispPrm->wHPosOfst + eCropInfo->u4CropLeftOffset) : dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #endif    
    }
    else 
    if(eScpipGetTdtvInType(u4VdpId) != SCALER_TDTV_IN_TYPE_FS && 
        eScpipGetTdtvOutType(u4VdpId) == SCALER_TDTV_OUT_TYPE_SG && 
        u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_OFF)            //input SBS/TAB/LI & output FS and scaler in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wHPosOfst, DRAM_M_1F_DA_OVERSCAN_LF);
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wOverscanRight, DRAM_M_1F_DA_OVERSCAN_RT);        
        #ifdef IS_SUPPORT_3D_PHOTO
           vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropLeftOffset : 0, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_HP_OFST_1);
        #endif    
    }
    else
    if ( ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_FS)) ||
         ((eTTDIn == E_TD_IN_FS_P) && ((eTTDOut == E_TD_OUT_3D_FS) || (eTTDOut == E_TD_OUT_NATIVE))))
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);    
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        #ifdef IS_SUPPORT_3D_PHOTO
           vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (dispPrm->wHPosOfst + eCropInfo->u4CropLeftOffset) : dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #endif    
    }    
    else
    #endif
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        //when 3D is done by MJC, V overscan will done by MJC & H overscan will be done by PSC
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        
        #ifdef IS_SUPPORT_3D_PHOTO
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropLeftOffset : 0, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_HP_OFST_1);    
        #endif
            
    }
    else if(vScpipIs3DDoneByScaler(u4VdpId)== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wHPosOfst, DRAM_M_1F_DA_OVERSCAN_LF);
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wOverscanRight, DRAM_M_1F_DA_OVERSCAN_RT);        
        #ifdef IS_SUPPORT_3D_PHOTO
           vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropLeftOffset : 0, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_HP_OFST_1);
        #endif
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);    
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        #ifdef IS_SUPPORT_3D_PHOTO
           vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (dispPrm->wHPosOfst + eCropInfo->u4CropLeftOffset) : dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
        #endif
    }
}

static void _vScpipSetVPorchReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    UINT16 dramOffset;
    RDispPrm* dispPrm;
    UINT32 vOfst;
    #ifdef IS_SUPPORT_3D_PHOTO
    TDTV_3D_CROP_INTO_T* eCropInfo;
    UINT8 bMode;
    #endif
    #if defined(SUPPORT_3D_OVSN_BY_SCL)
    UINT32 u4TTD;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
    
    u4TTD = u4DrvTDTVTTDModeQuery();
    eTTDIn  = TD_TTD_IN(u4TTD);
    eTTDOut = TD_TTD_OUT(u4TTD);    
    #endif    

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dramOffset = getDramOffset(bPath);    
    dispPrm = getDispPrm(bPath);
    #ifdef IS_SUPPORT_3D_PHOTO
    bMode = u1ScpipGetDispMode(bPath);   
    eCropInfo = ScpipGet3DCropInfo();
    #endif
    
    /*6. WSTARTP_OFST -- Picture Target Y Offset */
    #ifdef __MODEL_slt__        
    vOfst = dispPrm->wVPosOfst;
    // to fix error lines at the top of the screen
    //vOfst = dispPrm->wVPosOfst+1;
    #else
    vOfst = dispPrm->wVPosOfst;
    #endif

    #if defined(SUPPORT_3D_OVSN_BY_SCL)
    if(eScpipGetTdtvInType(u4VdpId) == SCALER_TDTV_IN_TYPE_FS && 
        eScpipGetTdtvOutType(u4VdpId) == SCALER_TDTV_OUT_TYPE_SG)   //input FS & output FS
    {
        //overscan done by WSTART_P
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (vOfst + eCropInfo->u4CropTopOffset) : vOfst, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, vOfst, PIP1_04_WSTARTP_OFST_1);        
        #endif        
    }
    else
    if(eScpipGetTdtvInType(u4VdpId) != SCALER_TDTV_IN_TYPE_FS && 
        eScpipGetTdtvOutType(u4VdpId) == SCALER_TDTV_OUT_TYPE_SG && 
        u1ScpipGetDispMode(bPath) == VDP_SCPOS_DISPMODE_OFF)            //input SBS/TAB/LI & output FS and scaler in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, vOfst, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, dispPrm->wOverscanBottom, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropTopOffset : 0, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_WSTARTP_OFST_1);
        #endif    
    }
    else
    if ( ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_FS)) ||
         ((eTTDIn == E_TD_IN_FS_P) && ((eTTDOut == E_TD_OUT_3D_FS) || (eTTDOut == E_TD_OUT_NATIVE))))
    {
        //overscan done by WSTART_P
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (vOfst + eCropInfo->u4CropTopOffset) : vOfst, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, vOfst, PIP1_04_WSTARTP_OFST_1);        
        #endif    
    }    
    else        
    #endif
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        //when 3D is done by MJC, V overscan will done by MJC & H overscan will be done by PSC    
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropTopOffset : 0, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_WSTARTP_OFST_1);
        #endif
    }
    else if(vScpipIs3DDoneByScaler(u4VdpId)== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, vOfst, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, dispPrm->wOverscanBottom, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? eCropInfo->u4CropTopOffset : 0, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_WSTARTP_OFST_1);
        #endif
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        #ifdef IS_SUPPORT_3D_PHOTO
            vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, (_fgFixRegion && (u4VdpId ==SV_VP_MAIN) && (bMode != VDP_SCPOS_PSCAN_DISPMODE)) ? (vOfst + eCropInfo->u4CropTopOffset) : vOfst, PIP1_04_WSTARTP_OFST_1);
        #else
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, vOfst, PIP1_04_WSTARTP_OFST_1);        
        #endif
    }
}

static void _vScpipSetHBoundaryReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    RDispPrm* dispPrm;
    
    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);
    
    /* H Boundary protection */
    vRegWriteFldAlign(SCPIP_PIP1_1B+pipOffset, dispPrm->u4HBound,PIP1_1B_BOUNDARY_H_POS_1);
}

static void _vScpipSetVBoundaryReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);

    /* V Boundary protection */
    if(dispPrm->u4USclV == DFT_US_FACTOR)  // no up scale, turn off v boundary
    {
        if(u4VdpId == VDP_1)
        {
            vRegWriteFldAlign(SCPIP_US1_1D, 0, US1_1D_UP_VBOUND_EN1);
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
            vRegWriteFldAlign(SCPIP_US1_1D, 1, US1_1D_UP_VBOUND_EN1);
            vRegWriteFldAlign(SCPIP_US1_1D, dispPrm->u4VBound, US1_1D_UP_VBOUND_Y1);
        }
        else //VDP_2
        {
            vRegWriteFldAlign(SCPIP_US2_03, 1, US2_03_UP_VBOUND_EN2);
            vRegWriteFldAlign(SCPIP_US2_03, dispPrm->u4VBound, US2_03_UP_VBOUND_Y2);
        }
    }
}

static void _vScpipSetMirrorFlipReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset; 
    UINT16 pipOffset;
    UINT8 bMirrorFlipMode;
    RDispPrm* dispPrm;
    
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    pipOffset = getPIPOffset(bPath);
    dispPrm = getDispPrm(bPath);
    bMirrorFlipMode = ((_arScalePrm[u4VdpId].u4FlipEnable << 1) | (_arScalePrm[u4VdpId].u4MirrorEnable));

    //1. set display mode (bit1: Mirror bit0: Flip)
    vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, bMirrorFlipMode, DRAM_M_00_DA_DISPLAY_MODE);

    //2. set Mirror YC match
    vRegWriteFldAlign(SCPIP_PIP1_1B+pipOffset, dispPrm->u1MirrorYCMatch, PIP1_1B_MIRROR_YCMATCH_1);
}

static void _vScpipSetDispmodeReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    pipOffset = getPIPOffset(bPath);

    vRegWriteFldAlign(SCPIP_PIP1_07+pipOffset, dispPrm->wHLen, PIP1_07_DISP_WIDTH_H_1);
    vRegWriteFldAlign(SCPIP_PIP1_07+pipOffset, dispPrm->wVLen, PIP1_07_DISP_WIDTH_V_1);
}
    
static void _vScpipSetVLMaskReg(UINT32 u4VdpId)
{
	UINT8 bPath;
	UINT16 pipOffset;

	bPath = getScalerPath(u4VdpId);
	pipOffset = getPIPOffset(bPath);

	//set mask to black
	if(_arScalePrm[u4VdpId].u4Is444)
	{
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0, DRAM_M_02_DA_OOB_DATA_V);
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0, DRAM_M_02_DA_OOB_DATA_U);
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0, DRAM_M_02_DA_OOB_DATA_Y);
	}
	else
	{
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0x200, DRAM_M_02_DA_OOB_DATA_V);
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0x200, DRAM_M_02_DA_OOB_DATA_U);
		vRegWriteFldAlign(SCPIP_DRAM_M_02 + pipOffset, 0, DRAM_M_02_DA_OOB_DATA_Y);
	}
	
}

static void _vScpipSetDispReg(UINT32 u4VdpId)
{   
    //1. set data format (8/10 bit, 444/422)
    _vScpipSetDataFormatReg(u4VdpId);
    //2. set display window (RHP,RVP,RHL,RVL)
    _vScpipSetDisplayWindowReg(u4VdpId);
    //3. set Dram Read/Write length
    _vScpipSetDramRWLengthReg(u4VdpId);
    //4. set h Porch
    _vScpipSetHPorchReg(u4VdpId);
    //5. set v Porch
    _vScpipSetVPorchReg(u4VdpId);
    //6. set h boundary
    _vScpipSetHBoundaryReg(u4VdpId);
    //7. set v boundary
    _vScpipSetVBoundaryReg(u4VdpId);
    //8. set mirror/filp
    _vScpipSetMirrorFlipReg(u4VdpId);
    //9. set dispmode HL & VL
    _vScpipSetDispmodeReg(u4VdpId);
 	//10.set VLMask
 	_vScpipSetVLMaskReg(u4VdpId);
    /**
    //[DTV00145304] line_cfg need to recal when change aspect 
    if((bPath == SV_VP_MAIN) && 
            (dispPrm->wDataLen != RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1)) && 
            ((bMode == VDP_SCPOS_DISPMODE_AUTO) || (bMode == VDP_SCPOS_DISPMODE_MANUAL)))
    { 
        vScpipSetAutoTune(SV_OFF);
        vScpipSetAutoTune(SV_ON);
    }
    
    */
}

static void _vScpipSetImportOnTVE(UINT8 bPath)
{
    UINT8 tveEnable;
    TVE_GetEnable(TVE_1, &tveEnable);    
    if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) || 
        (BSP_GetIcVersion() >= IC_VER_5368_AB && BSP_GetIcVersion() <= IC_VER_5368_AC) ||
        (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC))
    {
        if(bPath == SV_VP_MAIN)
        {
            return;
        }
        
        if(bPath == SV_VP_PIP && tveEnable == SV_TRUE)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_12, 0, SYSTEM_12_IMPORT_R_VSYNC_SEL2);            
        }
        else
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_R_VSYNC_SEL2);    
        }
    }
}

static void _vScpipUpdateDataFormatPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);

    //1. update 8/10 bits parameter
    _arScalePrm[u4VdpId].u410BitMode = u4ScpipGet10BitMode(u4VdpId);

    //2. update 422/444 parameter
    _arScalePrm[u4VdpId].u4Is444 = (bIsScalerInput444(bPath) == SV_TRUE)? 1 :0;    

    //3. update 422 up parameter
    if(bPath == SV_VP_MAIN)
    {
        //set default value
        _arScalePrm[u4VdpId].u4Is422Up = 0;
        dispPrm->u1Us422UVMirrorMode = 0;

        //5396 & 5368 will use 422 up in video timing.
        #if defined(CC_MT5396) || defined(CC_MT5368)
        if(_arScalePrm[u4VdpId].u4Is444 == 0)
        {
            //3.1 determine to use 422 up mode
            _arScalePrm[u4VdpId].u4Is422Up = 1;
            //3.2 422 up + mirror
            if(_arScalePrm[u4VdpId].u4MirrorEnable == 1)
            {
                dispPrm->u1Us422UVMirrorMode = 1;                
            }
        }
        #endif

        //5389 will turn on 422 up only when PR-up
        #if defined(CC_MT5389)
        if(_arScalePrm[u4VdpId].u4Is444 == 0 && getTdtvInfo(u4VdpId)->u4IsPRUp == SV_TRUE)  // input is 422 and PR-up
        {
            //3.1 determine to use 422 up mode
            _arScalePrm[u4VdpId].u4Is422Up = 1;            
            //3.2 422 up + mirror
            if(_arScalePrm[u4VdpId].u4MirrorEnable == 1)
            {
                dispPrm->u1Us422UVMirrorMode = 1;                
    }
}
        #endif
    }
}

static void _vScpipUpdateHPorchPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    UINT32 u4XOfstBeforeHPS;    //left overscan before pre-scaling down
    UINT32 u4HpsFactor;
    UINT32 u4XOfstAfterHps;     //left overscan after pre-scaling down
    UINT8 bMode;
    #if defined(MIB_420_MIRROR_WA)   
    PSCAN_CLIP_INFO_T clipInfo;
    #endif
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    u4XOfstBeforeHPS =  getMixedHPorch(u4VdpId);
    u4HpsFactor = _arScalePrm[u4VdpId].u4HpsFactor;
    u4XOfstAfterHps = DIVIDE_WITH_CARRIER(u4XOfstBeforeHPS * u4HpsFactor, DFT_HPD_FACTOR);
    bMode = u1ScpipGetDispMode(bPath);
    
#ifdef DYNAMIC_RES_VERIFY
    if (fgPscanOversca)
    {
        bMode = VDP_SCPOS_PSCAN_DISPMODE;
    }
#endif

    //update H Porch
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {  
        //when scaler is in pscan dispmode, overscan was done in PSCAN
        dispPrm->wHPosOfst = 0;
    }
    else
    {
    	#if 0 /* dram mode or line sync display mode support odd pixels hp_ofst */
        if ((u4XOfstAfterHps%2)&& !bIsScalerInput444(u4VdpId)) // if hporch is odd, force it to add 1 to be even
        {
            u4XOfstAfterHps = u4XOfstAfterHps - 1;  
        }
        #endif
	    if((u4XOfstAfterHps +_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) > _arScalePrm[u4VdpId].u4HpsWidth)
        {
            u4XOfstAfterHps = _arScalePrm[u4VdpId].u4HpsWidth - _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
        }

        #if defined(CC_FLIP_MIRROR_SUPPORT)
        UINT8 u1Config;
        
        u1Config = u1GetFlipMirrorConfig();
        if((u4GetFlipMirrorModule(VDP_1) != FLIP_BY_SCALER) && u4GetFlipMirrorModule(VDP_1) !=FLIP_BY_MJC && (u1Config & SYS_MIRROR_CONFIG_ON))
        {
            dispPrm->wHPosOfst = _arScalePrm[u4VdpId].u4HpsWidth - _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth - u4XOfstAfterHps;
        }
        else
        {
            dispPrm->wHPosOfst = u4XOfstAfterHps ;            
        }
        #else
        dispPrm->wHPosOfst = u4XOfstAfterHps ;
        #endif        
    }

    #if defined(MIB_420_MIRROR_WA)       
    u1ScpipGetClipInfo(bPath,&clipInfo);    //this call is only used to update "_arScalePrm[bPath].u2HOffset_MIBMirrorWA"
    dispPrm->wHPosOfst = dispPrm->wHPosOfst + _arScalePrm[bPath].u2HOffset_MIBMirrorWA;
    #endif
}

static void _vScpipUpdateVPorchPrm(UINT32 u4VdpId)
{
    UINT8 bMode;
    UINT8 bPath;
    RDispPrm* dispPrm;
    RPicInfo* picInfo;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    picInfo = getPicInfo(bPath);
    bMode = u1ScpipGetDispMode(bPath);
    
#ifdef DYNAMIC_RES_VERIFY
    if (fgPscanOversca)
    {
        bMode = VDP_SCPOS_PSCAN_DISPMODE;
    }
#endif

    //update V Porch
    if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        //when scaler is in pscan dispmode, overscan was done in PSCAN
        dispPrm->wVPosOfst = 0;
    }
    else
    {
        //if v porch is different.
        if(dispPrm->wVPosOfst != getMixedVPorch(u4VdpId) + picInfo->wVPorch)
        {
            dispPrm->wVPosOfst = getMixedVPorch(u4VdpId) + picInfo->wVPorch;
        }

        #if defined(CC_FLIP_MIRROR_SUPPORT)
        UINT8 u1Config;
        
        u1Config = u1GetFlipMirrorConfig();
        if((u4GetFlipMirrorModule(VDP_1) != FLIP_BY_SCALER) && u4GetFlipMirrorModule(VDP_1) !=FLIP_BY_MJC && (u1Config & SYS_FLIP_CONFIG_ON))
        {
            dispPrm->wVPosOfst = picInfo->wSrcH - picInfo->wTargetH - (getMixedVPorch(u4VdpId) + picInfo->wVPorch);
        }
        #endif        
    }
}

static void _vScpipUpdateDramHpPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);

    //In normal situation, HP just set to 0.
    //Consider preview mode later
    dispPrm->wHPos = 0;
}

static void _vScpipUpdateDramVpPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);

    //In normal situation, VP just set to 0.
    //Consider preview mode later
    dispPrm->wVPos = 0;
}

static void _vScpipUpdateDramRWLengthPrm(UINT32 u4VdpId)
{
    UINT16 bPath;
    RDispPrm* dispPrm;
    UINT32 u4DownScaledWidth;
    UINT32 u4DownScaledHeight;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);

    //Calculate down scaled width. Normally, we will set this to dram R/W length.
    u4DownScaledWidth = _arScalePrm[u4VdpId].u4DownScaledWidth;
    //Calculate down scaled height. Normally, we will set this to dram R/W Height.
    u4DownScaledHeight = _arScalePrm[u4VdpId].u4DownScaledHeight;
    
    //1. update Dram Write width
    dispPrm->u2DramWriteWidth = (UINT16)u4DownScaledWidth;
    //2. update Dram Write Height    
    dispPrm->u2DramWriteHeight = (UINT16)u4DownScaledHeight;
    //3. update Dram Read width    
    dispPrm->u2DramReadWidth = (UINT16)u4DownScaledWidth;
    //4. update Dram Read Height    
    dispPrm->u2DramReadHeight = (UINT16)u4DownScaledHeight;
    //5. update write picth  
    //dispPrm->u2DramLinePitch = DIVIDE_WITH_CARRIER((UINT16)u4DownScaledWidth,16);
}

static void _vScpipUpdateDispmodePrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    UINT32 u4WidthDS;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    u4WidthDS = _arScalePrm[u4VdpId].u4DownScaledWidth;
    
    //1. update DISP_WIDTH_W
    if(bIsScalerInput444(bPath) == SV_FALSE){     // 422 Mode
        if (u4ScpipGet10BitMode(u4VdpId)){        // 422 10 bit
            dispPrm->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 6);
        }else{                                    // 422  8 bit
            dispPrm->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 7);
        }
    }else{                                        // 444 Mode
        if (u4ScpipGet10BitMode(u4VdpId)){        // 444 10 bit
            dispPrm->wHLen = RSHIFT_WITH_CARRIER(u4WidthDS, 2);
        }else{                                    // 444  8 bit
            dispPrm->wHLen = DIVIDE_WITH_CARRIER(u4WidthDS, 5);
        }
    }    

    //2. update DISP_WIDTH_H
    dispPrm->wVLen = _arScalePrm[u4VdpId].u4DownScaledHeight;
}

static void _vScpipUpdateMirrorFlipPrm(UINT32 u4VdpId)
{
	UINT8 bPath;
	RDispPrm* dispPrm;

	bPath = getScalerPath(u4VdpId);
	dispPrm = getDispPrm(bPath);
	

	if(_arScalePrm[u4VdpId].u4Is422Up == SV_ON) //422-up
	{
		dispPrm->u1MirrorYCMatch = 3;
	}
	else										//444-up
	{
		if(_arScalePrm[u4VdpId].u4MirrorEnable == SV_ON)	//mirror
		{
			if(_arScalePrm[u4VdpId].u4Is444 == 1)
			{
				dispPrm->u1MirrorYCMatch = 0;		//mirror + 444
			}
			else
			{
				dispPrm->u1MirrorYCMatch = 1;		//mirror + 422
			}
		}
		else
		{
			dispPrm->u1MirrorYCMatch = 0;			 //non-mirror
		}
	}
}

static void _vScpipUpdateHBoundaryPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    //UINT32 u4DownScaledWidth;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    
    if(bIsScalerInput444(bPath) == SV_FALSE)     // 422 Mode
    {
        dispPrm->u4HBound = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth % 2); // H bound need to be even
    }
    else                                            // 444 Mode
    {
        dispPrm->u4HBound = dispPrm->u2DramReadWidth;
    }    
}

static void _vScpipUpdateVBoundaryPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    dispPrm->u4VBound = dispPrm->u2DramReadHeight;
    //workaround for FPR flip
    if((vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE) && (BSP_GetIcVersion() == IC_VER_5396_AA || BSP_GetIcVersion() == IC_VER_5368_AA))
    {
        if(eScpipGetTdtvOutType(u4VdpId) == SCALER_TDTV_OUT_TYPE_FPR)
        {
            if(_arScalePrm[u4VdpId].u4FlipEnable == SV_TRUE)
            {
                dispPrm->u4VBound = dispPrm->u2DramReadHeight >> 1;
            }
        }
    }
    if(u4ScpipGetTDTVPRUpFlag(u4VdpId) == SV_TRUE)
    {
        dispPrm->u4VBound += 3;
    }
}

static void _vScpipUpdateInitPhasePrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT8 bMode;
    RDispPrm* dispPrm;
    RDispInfo* dispInfo;
    UINT32 u4UpScaleInputWidth;
    UINT32 u4UpScaleInputHeight;
    
    bPath = getScalerPath(u4VdpId);
    bMode = u1ScpipGetDispMode(bPath);    
    dispPrm = getDispPrm(bPath);
    dispInfo = getDispInfo(bPath);
    
    //Calculate down scaled width.
    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        u4UpScaleInputWidth = (UINT32)dispPrm->u2DramReadWidth;
    }
    else
    {
        u4UpScaleInputWidth = _arScalePrm[u4VdpId].u4DownScaledWidth;
    }
    //calculate up scaled height.
    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        u4UpScaleInputHeight = (UINT32)dispPrm->u2DramReadHeight;
    }
    else
    {
        u4UpScaleInputHeight = _arScalePrm[u4VdpId].u4DownScaledHeight;
    }

    // ------ Init phase setting of H up scaling ------------
    if(u4VdpId == VDP_1) // only main path has this feature
    {   
        if(!_fgIsHNegInitPhaseOn)
        {
            dispPrm->u4USclHInit = 0;
            dispPrm->u1USclHInitSigned = 0;  
        }
        else
        {
            dispPrm->u4USclHInit = _u4ScpipCalNegInitPhase(u4UpScaleInputWidth, dispInfo->wWidth, dispPrm->u4USclH);
            dispPrm->u1USclHInitSigned = (dispPrm->u4USclHInit)?1:0;

            if(bIsScalerInput444(u4VdpId))
            {
            	dispPrm->u4USclHInitUV = dispPrm->u4USclHInit;
            }
            else // 422 input, check mirror or not.
            {
                if(((_arScalePrm[u4VdpId].u4MirrorEnable == SV_OFF) && (dispPrm->u1USclHInitSigned==0)) ||
                	((_arScalePrm[u4VdpId].u4MirrorEnable == SV_ON) && (dispPrm->u1USclHInitSigned==1)))
                {
                    dispPrm->u4USclHInitUV = dispPrm->u4USclHInit;
                }
                else
                {
                    dispPrm->u4USclHInitUV = dispPrm->u4USclHInit + DFT_US_FACTOR;
                }
            }
        }

        // ------ Init phase setting of V up scaling ------------
        if(!_fgIsVNegInitPhaseOn||(bMode == VDP_SCPOS_DISPMODE_AUTO)||(bMode == VDP_SCPOS_DISPMODE_MANUAL)) // turn off v negative initial phase in scaler display mode
        {
            dispPrm->u4USclVInit = 0;
            dispPrm->u1USclVInitSigned = 0;
        }
        else
        {
            dispPrm->u4USclVInit = _u4ScpipCalNegInitPhase(u4UpScaleInputHeight, dispInfo->wHeight, dispPrm->u4USclV);
            dispPrm->u1USclVInitSigned = (dispPrm->u4USclVInit)?1:0;
        }
    	// handle interlace input, set different init phase for top and bottom field.
    	if(_arScalePrm[u4VdpId].u4IsScalerBobMode == SV_TRUE)
    	{
    	     dispPrm->u1USclV3DFieldEn =1;
    	     // bottom filed phase setting.
    	     dispPrm->u1USclVInitSign_Btm = dispPrm->u1USclVInitSigned;
    	     dispPrm->u4USclVInit_Btm = dispPrm->u4USclVInit - (DFT_US_FACTOR/4);

    		 // top filed phase setting.
    	     dispPrm->u4USclVInit  += (DFT_US_FACTOR/4);
    	     if(dispPrm->u4USclVInit > DFT_US_FACTOR)
    	     {
    	     		dispPrm->u4USclVInit = dispPrm->u4USclVInit - DFT_US_FACTOR;
    	     		dispPrm->u1USclVInitSigned = 0;
    	     }
    	}
    	else
    	{
    		dispPrm->u1USclV3DFieldEn =0;
    		dispPrm->u1USclVInitSign_Btm = 0;
    	    dispPrm->u4USclVInit_Btm = 0;
    	}
    }
}

static void _vScpipUpdateDisplayWindowPrm(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;

    bPath = getScalerPath(u4VdpId);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);

    //Widnow H Start
    dispPrm->wRHPos = dispInfo->wXOffset;

    //Window V Start
    dispPrm->wRVPos = dispInfo->wYOffset;    
    
    //Window Height    
    if(_u4ForceDispOff[u4VdpId])// for TVE use, not to turn on sub window.
    {
        dispPrm->wRVLen = 0;
        if(dispInfo->wWidth != 0)
        {
             //window width
            dispPrm->wRHLen = dispInfo->wWidth;
        }
    }
    else
    {
        dispPrm->wRVLen = dispInfo->wHeight;
        //window width
        dispPrm->wRHLen = dispInfo->wWidth;
    }
}

static void _vScpipUpdateDispPrm(UINT32 u4VdpId) 
{
    //update 
    vScpipUpdateUs3DFieldFlag(u4VdpId);
    //update mddi min/max fifo length
    _vScpipUpdateMddiFifoInfo(u4VdpId);
    //calculate HPS factor, and store in _arScalePrm[u4VdpId].u4HpsFactor
    _vScpipUpdateHPSInfo(u4VdpId);

    //1. update H porch for overscan (HP_OFST)
    _vScpipUpdateHPorchPrm(u4VdpId);
    
    //2. update V porch for overscan (WSTARTP_OFST)
    _vScpipUpdateVPorchPrm(u4VdpId);
    
    if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
    {
        vScpipUpdate3DPrm(u4VdpId);
    }
    else
    {
        //re-calculate scaling factor
        _vScpipCalNewScale(u4VdpId);
        //4. update Dram/Fifo R/W length (original HL/ VL/ Data_Length)
        _vScpipUpdateDramRWLengthPrm(u4VdpId);
    }

    vScpipUpdate3DPRUpFlag(u4VdpId);    

    //0. update 8/10 bit, 422/444 data format, and 422 up-related prm
    _vScpipUpdateDataFormatPrm(u4VdpId);
       
    //3. update Dram write start point (original HP & VP)
    _vScpipUpdateDramHpPrm(u4VdpId);
    _vScpipUpdateDramVpPrm(u4VdpId);
        
    //5. update display window (RHP/ RVP/ RHL/ RVL)
    _vScpipUpdateDisplayWindowPrm(u4VdpId);
    
    //6. update mirror flip related setting
    _vScpipUpdateMirrorFlipPrm(u4VdpId);
    
    //7. update H&V boundary protection setting (BOUNDARY_H_POS_1 & UP_VBOUND_Y1)
    _vScpipUpdateHBoundaryPrm(u4VdpId);
    _vScpipUpdateVBoundaryPrm(u4VdpId);
    
    //8. update initial phase
    _vScpipUpdateInitPhasePrm(u4VdpId);
    
    //9. update Width & Height for dispmode (HL & VL -> DISP_WIDTH_H & DISP_WIDTH_V)
    _vScpipUpdateDispmodePrm(u4VdpId);
}

/**
 * @brief Update scaler parameters protected by import
 * @param u4VdpId VDP_1/VDP_2
 */
static void _vScpipUpdateRegister(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT8 bMode;

    bPath = getScalerPath(u4VdpId);
    bMode = getScalerMode(bPath);

    /* set HPS Factor */
    if (_arScalePrm[u4VdpId].u4HpsFactor == 0) // FIXME
    {
        vScpipSetHwHPSFactor(u4VdpId, DFT_HPD_FACTOR);
        LOG(1, "DEBUG [%d] PREDOWN = 0\n", u4VdpId);
    }
    else
    {
    vScpipSetHwHPSFactor(u4VdpId,_arScalePrm[u4VdpId].u4HpsFactor);
    }

    /* Set DS horizontal initial offset for Main channel. */
    _vScpipSetDSInitialFactor(u4VdpId);

    /* Set DS Factor */
    _vScpipSetDownScalingFactor(u4VdpId);

    /* Set US Init Phase */
    if (u4VdpId == VDP_1)
    {
        _vScpipSetUSInitialFactor(u4VdpId);
    }
    
    /* Set Up Scale Factor & Nonlinear Factor */
    _vScpipSetUSReg(u4VdpId);               
    
    /* Set Display Related Register */
    _vScpipSetDispReg(u4VdpId);
    
    /* config line buffer setting */
    if (bPath == SV_VP_MAIN && bMode != VDP_SCPOS_DISPMODE_OFF)
    {
        vScpipDispmodeLineCfg(bPath, bMode);
    }    

    /* config import setting for TVE output */
    _vScpipSetImportOnTVE(bPath);
}

/**
 * @brief Calculate up/down scale factor
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Source input width/height
 * @param u4Target output width/height
 * @param u4LimitLength fifo limit length
 * @param bDirection horizontal/vertical direcrion
 * @6896 review done
 */
void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 in, UINT32 out, UINT32 u4LimitLength, UINT8 bDirection) 
{
    UINT8 bPath;
    RVChannel* channel;
    RDispPrm* dispPrm;
    UINT8 isChannelOn;
    bPath = getScalerPath(u4VdpId);
    channel = getChannel(bPath);
    dispPrm = getDispPrm(bPath);
    isChannelOn = channel->bIsChannelOn;    

    if ( in == 0 || 
         out == 0 || 
         isChannelOn == SV_OFF || 
         (out <= u4LimitLength && out == in))
    {
        // no scaling
        if (bDirection == CAL_HDIRECT_FACTOR) // horizontal
        {
            dispPrm->u4USclH = DFT_US_FACTOR;
            dispPrm->u4DSclH = DFT_DS_FACTOR;
        }
        else // vertical
        {
            dispPrm->u4USclV = DFT_US_FACTOR;
            dispPrm->u4DSclV = DFT_DS_FACTOR;
        }
    }
    else if ((out < in) && (out <= u4LimitLength))
    {
        // down scaling
        UINT32 u4Factor;

        //no up-scaling
        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            dispPrm->u4USclH = DFT_US_FACTOR;
        }
        else
        {
            dispPrm->u4USclV = DFT_US_FACTOR;
        }

        u4Factor = (out << DFT_DS_BIT_NUM)/ in;

        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            dispPrm->u4DSclH =u4Factor;
            if (dispPrm->u4DSclH > DFT_DS_FACTOR)
            {
                dispPrm->u4DSclH = DFT_DS_FACTOR;
            }
        }
        else
        {
            dispPrm->u4DSclV = u4Factor;
            if (dispPrm->u4DSclV > DFT_DS_FACTOR)
            {
                dispPrm->u4DSclV = DFT_DS_FACTOR;
            }
        }
    }
    else   // up scaling or down+up scaling case
    {
        UINT32 u4Factor;
        UINT32 u4OutPutLength;

        if (in > u4LimitLength)            
        {
            u4Factor =(u4LimitLength<< DFT_DS_BIT_NUM)/ in;

            if (bDirection == CAL_HDIRECT_FACTOR)
            {
                dispPrm->u4DSclH = u4Factor;
                if (dispPrm->u4DSclH > DFT_DS_FACTOR)
                {
                    dispPrm->u4DSclH = DFT_DS_FACTOR;
                }
            }
            else
            {
                dispPrm->u4DSclV = u4Factor;
                if (dispPrm->u4DSclV > DFT_DS_FACTOR)
                {
                    dispPrm->u4DSclV = DFT_DS_FACTOR;
                }
            }

            u4OutPutLength = u4LimitLength;
            LOG(3, "SCPOS H-Scaling DOWN+UP: %d -> %d -> %d\n", in, u4LimitLength, out); 
        }
        else
        {
            if (bDirection == CAL_HDIRECT_FACTOR)
            {
                dispPrm->u4DSclH = DFT_DS_FACTOR;
            }
            else
            {
                dispPrm->u4DSclV = DFT_DS_FACTOR;
            }
            u4OutPutLength= in;            
        }

        // up scaling
        if (bDirection == CAL_HDIRECT_FACTOR)
        {        
            // enlarge H outptu length according to up-scaling ratio to prevent right most line thick
            if(u4OutPutLength!=0)
            {
                out += ((out/u4OutPutLength)-1);
            }
        }

        u4Factor =(u4OutPutLength<< DFT_DS_BIT_NUM)/out;        

        if (bDirection == CAL_HDIRECT_FACTOR)
        {
            dispPrm->u4USclH = u4Factor;
            if (dispPrm->u4USclH > DFT_US_FACTOR)
            {
                dispPrm->u4USclH = DFT_US_FACTOR;
            }
        }
        else
        {
            dispPrm->u4USclV = u4Factor;
            if (dispPrm->u4USclV > DFT_US_FACTOR)
            {
                dispPrm->u4USclV = DFT_US_FACTOR;
            }
        }
    }
}

/**
 * @brief Calculate new scale parameters
 * @param u4VdpId VDP_1/VDP_2
 */
static void _vScpipCalNewScale(UINT32 u4VdpId) 
{
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4WidthLimitation;    
    UINT32 u4HeightLimitation;
    UINT32 u4FifoLength;

    UINT8 bPath;
    RVChannel* channel;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;
    RDispPrm* dispPrm;
    RDispPrm rOriDispPrm;      //local variable used to keep original data    
    UINT32 u4ScalerMode;
    UINT8 isChannelOn;

    //prepare data
    bPath = getScalerPath(u4VdpId);
    channel = getChannel(bPath);    
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);    
    dispPrm = getDispPrm(bPath);
    u4ScalerMode = u1ScpipGetDispMode(bPath);
    isChannelOn = channel->bIsChannelOn;
    // store original scaling parameters
    rOriDispPrm = *dispPrm;

    #if defined(SUPPORT_3D_OVSN_BY_SCL)
        #if defined(SUPPORT_3D_H_OVSN_BY_FSC)
            u4SrcWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
        #elif defined(SUPPORT_3D_H_OVSN_BY_PSC)
            u4SrcWidth  = (vScpipIs3DDoneByMJC()==SV_TRUE)? _arScalePrm[u4VdpId].u4HpsWidth : _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
        #else
            u4SrcWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
        #endif
        u4SrcHeight = picInfo->wTargetH;
    #else
        u4SrcWidth  = (vScpipIs3DDoneByMJC()==SV_TRUE)? _arScalePrm[u4VdpId].u4HpsWidth : _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
        u4SrcHeight = (vScpipIs3DDoneByMJC()==SV_TRUE)? picInfo->wSrcH : picInfo->wTargetH;
    #endif

    //for sw checker board to SBS to 2D case, scaler will use overscan to do 3D sbs to 2D.
    if(vScpipIs3DDoneByScaler(u4VdpId)==SV_FALSE && 
        eScpipGetTdtvInType(u4VdpId) == SCALER_TDTV_IN_TYPE_SBS &&
        eScpipGetTdtvOutType(u4VdpId)== SCALER_TDTV_OUT_TYPE_3D_TO_2D)
    {
        u4SrcWidth = DIVIDE_WITH_ROUND(picInfo->wTargetW * _arScalePrm[u4VdpId].u4HpsWidth, picInfo->wSrcW) >> 1;
    }
    
    //pixel-based output window
    u4OutWidth = dispInfo->wWidth;
    u4OutHeight = dispInfo->wHeight;

    // window width or height is 0, no need to calculate hps factor
    if ((u4OutWidth == 0) || (u4OutHeight == 0)) 
    {
        return;
    }    
    //Calculate dram width and height limit.
    vScpipCalDramLimit(u4VdpId,  u4SrcWidth, u4SrcHeight, u4OutWidth, u4OutHeight, &u4WidthLimitation, &u4HeightLimitation);    
        
    u4FifoLength = (_u4SCPOS_FIFO_LENGTH[u4VdpId][bIsScalerInput444(getScalerPath(u4VdpId))]);

    if (u4FifoLength > u4WidthLimitation)
    {
        u4FifoLength = u4WidthLimitation;
    }

    /* Horizontal Direction */
    _vScpipCalScaleFactor(u4VdpId, u4SrcWidth, u4OutWidth, u4FifoLength, CAL_HDIRECT_FACTOR);
    /* Vertical Direction */
    _vScpipCalScaleFactor(u4VdpId, u4SrcHeight, u4OutHeight, u4HeightLimitation, CAL_VDIRECT_FACTOR);

    //Calculate down scaled width.
    _arScalePrm[u4VdpId].u4DownScaledWidth = 
        DIVIDE_WITH_CARRIER(u4SrcWidth * dispPrm->u4DSclH,  DFT_DS_FACTOR);
    //Calculate down scaled height. Normally, we will set this to dram R/W Height.
    _arScalePrm[u4VdpId].u4DownScaledHeight = 
        DIVIDE_WITH_CARRIER(u4SrcHeight * dispPrm->u4DSclV,  DFT_DS_FACTOR);    

    _arScalePrm[u4VdpId].u4NonlinearCtrl = 0;

    if(_arScalePrm[u4VdpId].u4NonlinearEn != 0)
    {
        if ((u4VdpId == VDP_1) &&
                (u4OutWidth != 0) &&
                (u4SrcWidth != 0))
        {
            if (_arScalePrm[u4VdpId].u4NonlinearAuto) 
            {
                vScpipCalNonLinear(u4VdpId, u4SrcWidth, u4OutWidth); // For CC_SCPOS_WA1
            }
            else if(_arScalePrm[u4VdpId].u4NewNonlinear)
            {
                vScpipCalNewNonlinear(u4VdpId, u4SrcWidth, u4OutWidth);
            }
            else // not auto and not new nonlinear
            {
                _arScalePrm[u4VdpId].u4NonlinearCtrl = 1;
            }
        }
    }

    // store original scaling parameters
    if ((rOriDispPrm.u4DSclH != dispPrm->u4DSclH) || (rOriDispPrm.u4DSclV != dispPrm->u4DSclV) ||
            (rOriDispPrm.u4USclH != dispPrm->u4USclH) || (rOriDispPrm.u4USclV != dispPrm->u4USclV))
    {
        LOG(3, "Scaler factor changed!: HU(0x%x) VU(0x%x) HD(0x%x) VD(0x%x)\n",
                rOriDispPrm.u4USclH, rOriDispPrm.u4USclV,
                rOriDispPrm.u4DSclH, rOriDispPrm.u4DSclV);

        //if scaling factor changed, inform scaler to re-enter scaler dispmode or pscan dispmode
        if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE ||
                u4ScalerMode==VDP_SCPOS_DISPMODE_AUTO||
                u4ScalerMode==VDP_SCPOS_DISPMODE_MANUAL)
        {
            LOG(4,"Scaling factor changed ==>fireSCPOSModeChange()\n");
            fireSCPOSModeChange(u4VdpId);
        }        
    }

    if (isChannelOn != SV_OFF)
    {
        LOG(3, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n",u4VdpId, 
                dispInfo->wXOffset, dispInfo->wYOffset,u4OutWidth,u4OutHeight,
                getMixedHPorch(u4VdpId),getMixedVPorch(u4VdpId),u4SrcWidth,u4SrcHeight);

        LOG(3, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n",
                dispPrm->u4USclH, dispPrm->u4USclV,
                _arScalePrm[u4VdpId].u4HpsFactor, 
                dispPrm->u4DSclH, dispPrm->u4DSclV);
    }
}

#ifdef CC_SUPPORT_VSS
void _vScpipVideoScreenshot(UINT32 bPath)
{
    if (_VSS_Path == bPath && (bDrvVideoSignalStatus(_VSS_Path) == SV_VDO_STABLE))
    {
        if (_VSS_ConnectTrigger > 4)
        {
            _VSS_Cnt = RegReadFldAlign(SCPIP_DRAM_M_03, DRAM_M_03_STA_GFX_WR_FRAME_CNT);
            LOG(1, "Connect Trigger [%d]\n", _VSS_ConnectTrigger);
            _VSS_ConnectTrigger--;
        }

        if (((_VSS_ConnectTrigger == 4) || (_VSS_ConnectTrigger == 2))
            && (RegReadFldAlign(SCPIP_DRAM_M_14, DRAM_M_14_STA_GFX_WRITE_EN) == 0))
        {
            u1Scpip_GFX_Write_Trigger();
            LOG(1, "Connect Trigger [%d]\n", _VSS_ConnectTrigger);
            _VSS_ConnectTrigger--;
        }

        if ((_VSS_Cnt != RegReadFldAlign(SCPIP_DRAM_M_03, DRAM_M_03_STA_GFX_WR_FRAME_CNT)) && (RegReadFldAlign(SCPIP_DRAM_M_14, DRAM_M_14_STA_GFX_WRITE_EN) == 0))
        {
            _VSS_Cnt = RegReadFldAlign(SCPIP_DRAM_M_03, DRAM_M_03_STA_GFX_WR_FRAME_CNT);
            LOG(1, "Scaler Dump [%d]\n", _VSS_Cnt);
            if (_VSS_ConnectTrigger != 0)
            {
                _VSS_ConnectTrigger--;
            }
            
            if (_VSS_ConnectTrigger == 0)
            {
                _VdoVSSCb();
            }
        }
    }
}
#endif

void vScpipOnVdoModeChange(UINT8 bPath)
{

    if(bPath == SV_VP_MAIN)
    {
        //turn off frame track
        vScpipSwitchFrameTrackOnOff(SV_OFF, SV_OFF);
        //turn off scaler dispmode
        vSwitchDispMode(SV_OFF);
        //set to dram mode
        if (u1ScpipGetDispMode(SV_VP_MAIN) != VDP_SCPOS_PSCAN_DISPMODE)
        {
            _VDP_DispModeEnable((UCHAR)SV_VP_MAIN, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
        }
    }

    #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)
    //turn off import
    vScpipTurnOffImportProtection(bPath);
    LOG(1,"Mode Chg. Turn Off import protection [%d]\n", bPath);
    #endif
}

void vScpipOnVdoModeChangeDone(UINT8 bPath)
{
    #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)
    _arScalePrm[getScalerVDP(bPath)].u4ModChgDneFlg = SV_TRUE;
    LOG(1,"Mode Chg done.Set u4ModChgDneFlg to true\n");    
    #endif
}

/**
 * @brief Scpos main path normal procedure
 */
static void _vScpipNormProcByPath(const UINT32 u4VdpId) 
{
    UINT32 u4Update = 0;
    UINT8 bPath = getScalerPath(u4VdpId);
    BOOL fgIsMain = (u4VdpId == VDP_1);
    RVChannel* channel = getChannel(bPath);
    BOOL fgChannelIsFreeze = channel->bIsFreeze;
    UINT8 bMode = getScalerMode(bPath);
    #ifdef __MODEL_slt__
    UINT8 bIsChannelOn = channel->bIsChannelOn;
    #endif

    //#6896 review this. Dram dump??
    if(vScpipGetDumpCtrl()==SV_FALSE) //use NCSTool to access this register via 0xC000 in XDATA
    {        
        vScpipCheckWriteCtrl(u4VdpId);
    }        

    if(((channel->bIsChannelOn == (UINT8)SV_ON) && (!fgChannelIsFreeze)) && (_arScalePrm[u4VdpId].u4Enable ==1))
    {
        UINT32 u4MuteDelay = _vDrvVideoGetMuteDelay(getScalerPath(u4VdpId), MUTE_MODULE_SCPOS_TV3D);

        if(u4MuteDelay >= 0xffff) // when mute delay is big, it's FOREVER_MUTE
        {
            _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u4VdpId, _u4ScpipReCalDelayCounter(u4VdpId, VDP_SCPOS_TV3D_MUTE_DELAY), FALSE);
        }
    }
    //@6896 review this: for frame track & frame sync??
    if (fgIsMain) 
    {
        vScpipDispmodeFreeRunChk();
    } 

    //@6896 import not ready, just return from this function. Do we need to do this in 5396??
    if((_u4ScposImportNotReady[u4VdpId] != 0) && bScpipGetDramRead(u4VdpId) == SV_ON)
    {
        _u4ScposImportNotReady[u4VdpId]--;
        return;  // import not ready, return from this function without any display parameter change          
    }
    else
    {
        _u4ScposImportNotReady[u4VdpId]=0;
    }

    #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)	
    // when input unstable, Turn Off import protection
    if(bDrvVideoSignalStatus(bPath) != SV_VDO_STABLE)
    {
        vScpipTurnOffImportProtection(bPath); // FIXME
    }
    #endif

    //------------------------------------------------------
    // import ready, go on processing
    //------------------------------------------------------

    //handle freeze count down
    _vScpipHandleChannelFreeze(u4VdpId);

    if(fgIsScposFlgSet((fgIsMain)?(MAIN_3D_RES_CHG|MAIN_DISP_PRM_CHG):(PIP_3D_RES_CHG|PIP_DISP_PRM_CHG)))
    {
        RDispInfo* dispInfo = getDispInfo(bPath);
        RPicInfo* picInfo = getPicInfo(bPath);
        //decide is 121 and 121-able
        _vScpipUpdate121MapEn(u4VdpId);
        bApiVideoSetSrcOverScan(bPath, picInfo->rOverScan.u4Top, picInfo->rOverScan.u4Bottom, picInfo->rOverScan.u4Left, picInfo->rOverScan.u4Right);
        bApiVideoSetSrcRegion(bPath, picInfo->rSrcRegion.u4X,picInfo->rSrcRegion.u4Y, picInfo->rSrcRegion.u4Width, picInfo->rSrcRegion.u4Height);
        //update dispInfo pixel-based (x, y, width, height)
        
#ifdef IS_SUPPORT_3D_PHOTO
        if(_fgFixRegion && (bPath == SV_VP_MAIN))
        {
            VDP_REGION_T rOutRegion;
            vReCal3DPhotoOutputRegion(&rOutRegion);
            VDP_SetOutRegion(VDP_1, 0, rOutRegion);
        }
#endif
        bApiVideoSetDispRegion(bPath, dispInfo->rOutRegion.u4X, dispInfo->rOutRegion.u4Y, dispInfo->rOutRegion.u4Width, dispInfo->rOutRegion.u4Height);
        vScpipUpdateSrm3DResolution(u4VdpId);
        
        vClrScposFlg((fgIsMain)?(MAIN_3D_RES_CHG):(PIP_3D_RES_CHG));
    }

    if ((fgIsScposFlgSet((fgIsMain) ? (MAIN_DISP_PRM_CHG|MAIN_HV_CHG) : (PIP_DISP_PRM_CHG|PIP_HV_CHG))) && 
        (fgChannelIsFreeze==SV_FALSE)) // if still in freeze, don't apply scaler mode change
    {        
        #ifdef __MODEL_slt__
        if (bIsChannelOn && (!fgChannelIsFreeze))   //channel is on & not freezed
        #endif
        {  
            //update display parameter
            _vScpipUpdateDispPrm(u4VdpId);  
            u4Update = 1;    
        }
        //clear scaler mode change flag
        vClrScposFlg((fgIsMain) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG);
        vClrScposFlg((fgIsMain) ? MAIN_HV_CHG : PIP_HV_CHG);
    }            

    if (u4Update)
    {
        UINT8 bInputRefreshRate = bDrvVideoGetRefreshRate(u4VdpId);
        //@6896 review this. need to do this??
        if((bInputRefreshRate <=30) && (bInputRefreshRate>0)) 
        {
            _u4ScposImportNotReady[u4VdpId] = (VDP_SCPOS_IMPORT_PROTECT * vDrvGetLCDFreq() + (bInputRefreshRate -1))/bInputRefreshRate;
        }
        else
        {
            _u4ScposImportNotReady[u4VdpId] = VDP_SCPOS_IMPORT_PROTECT;
        }

        /**** start import protect ****/
        vScpipSetImportOnOff(bPath, SV_OFF);                
        
        /* Set display-related reg */
        _vScpipUpdateRegister(u4VdpId);         

        /* Set Scaler Taps, Bypass & DE Alignment */
        _vScpipConfigScalingMode(u4VdpId);                      
        
        /* Set 3D Input/ Output Convert register */
        vScpipSet3DReg(u4VdpId);

        //config pscan dispmode parameter            
        if(bMode == VDP_SCPOS_PSCAN_DISPMODE)
        {
            vScpipSetPscanDispmodeHLen(bPath, u4ScpipGetPscanDispmodeHLen(bPath));
        }
        
        if(fgIsMain)
        {
            #if defined(CC_MT5396)
            //1. Set POCLK
            vScpipConfigPOCLK(getScalerMode(getScalerPath(u4VdpId)));
            #endif        
            //2. reconfig Scaler frame track (both FSC & PSC)
            vFrameTrackConfig(1);
        }
        
        #if defined(SCPIP_LI_TO_SBS_VSYNC_OPT_WA)
        vScpipDoLIToSBSVsyncOptWA(bPath, SV_TRUE);
        #endif        
        
        if(bImportTrigger == SV_TRUE)
        {
            vScpipSetImportOnOff(bPath, SV_ON);
        }

        /**** end of import protect ****/

        #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)
        if(_arScalePrm[u4VdpId].u4ModChgDneFlg == SV_TRUE)
        {   
            vScpipTurnOnImportProtection(bPath);
            LOG(1,"Mode Chg Done & import applied. Turn On import protection [%d]\n", bPath);

            _arScalePrm[u4VdpId].u4ModChgDneFlg = SV_FALSE;
        }
        #endif
    }

    #ifdef CC_SUPPORT_VSS
    _vScpipVideoScreenshot(bPath);
    #endif

    // dispmode mode change handling
    if (fgIsMain && fgIsScposFlgSet(SCALER_MAIN_MODE_CHG))
    {
        vScpipModeSwitch(SV_VP_MAIN); 
        #if defined(SCPIP_LI_TO_SBS_VSYNC_OPT_WA)
        vScpipDoLIToSBSVsyncOptWA(bPath, SV_FALSE);
        #endif
        vSetScposFlg(MAIN_DISP_PRM_CHG); 
        vClrScposFlg(SCALER_MAIN_MODE_CHG);        
    }
}

/**
 * @brief Scpos normal procedure
 */
void vScpipNormProc(void) 
{
    #ifdef SCPIP_TV3D_DEBUG
    //Keep LR Status & print debug message
    _vScpipLogLRStatus();
    #endif

    //@6896 review this.
    vOstgNormProc();

    //@6896 review this
    #ifndef CC_COPLAT_MT82
    #ifndef __MODEL_slt__
    OSD_PLA_Unmute();
    #endif //__MODEL_slt__
    #endif //CC_COPLAT_MT82

    //@6896 review this
    #ifdef DRV_SUPPORT_EXTMJC
    if(IS_COMPANION_CHIP_ON())
    {
        OSD_DMA_OnOutputVsync();
    }
    #endif

    //Main Path Normal procedure
    _vScpipNormProcByPath(VDP_1);    

    //Sub Path Normal procedure
    #if SUPPORT_POP
    _vScpipNormProcByPath(VDP_2);
    #endif

    //Post scaler Normal procedure
    #if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscNormProc();
    #endif
}

