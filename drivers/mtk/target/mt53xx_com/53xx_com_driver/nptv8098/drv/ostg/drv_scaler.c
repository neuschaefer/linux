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
#include "drv_predown.h"
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
#if defined(CC_MT5398)
#include "hw_predown.h"
#include "hw_regen.h"
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

#include "hw_di.h"
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
    }
};

SCALER_OUTTIMING_PRM_T _arPanelInfo = {2200,1920,1125,1080,0x8000,0x8000,60};

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

//static function for calculate scaling factor
static void _vScpipCalNewScale(UINT32 u4VdpId);
static UINT8 _vScpicIsOneToOneAble(UINT32 u4VdpId);
static void _vScpipUpdate121MapEn(UINT32 u4VdpId);
static void _vScpipUpdateMddiFifoInfo(UINT32 u4VdpId);
static void _vScpipUpdateHPSInfo(UINT32 u4VdpId);
static UINT32 _u4ScpipCalNegInitPhase(const UINT32 u4InW, const UINT32 u4OutW, const UINT32 u4UScl);
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 in, UINT32 out, UINT32 u4LimitLength, UINT8 bDircection);
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
        u4TrackTargetVal = wDISPLAY_HEIGHT / 2;
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
        vRegWriteFldAlign(SCPIP_PIP_OA_0E, u4PanelVTotal - u4Step*2, PSPIP_OA_0E_VTOTAL_ADJUST_FAST10);          
        vRegWriteFldAlign(SCPIP_PIP_OA_0F, u4PanelVTotal + u4Step, PIP_OA_0F_VTOTAL_ADJUST_SLOW01);
        vRegWriteFldAlign(SCPIP_PIP_OA_0F, u4PanelVTotal + u4Step*2, PIP_OA_0F_VTOTAL_ADJUST_SLOW10);          
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
    vRegWriteFldAlign(SCPIP_PIP_OA_10, u4UpperBound, PIP_OA_10_UPPER_BOUND2);  
    //6.2 upper bound1
    u4UpperBound = u4TrackTargetVal + u4LockRange;
    if(u4UpperBound > u4PanelVTotal)
    {
        u4UpperBound -= u4PanelVTotal;
    }
    vRegWriteFldAlign(SCPIP_PIP_OA_10, u4UpperBound, PIP_OA_10_UPPER_BOUND1);  
    //6.3 lower bound2
    u4LowerBound = (u4TrackTargetVal > u4LockRange*3)?(u4TrackTargetVal-(u4LockRange*3)):(u4TrackTargetVal+u4PanelVTotal-(u4LockRange*3));
    vRegWriteFldAlign(SCPIP_PIP_OA_11, u4LowerBound, PIP_OA_11_LOWER_BOUND2);      
    //6.4 lower bound1
    u4LowerBound = (u4TrackTargetVal > u4LockRange)?(u4TrackTargetVal - u4LockRange):(u4TrackTargetVal + u4PanelVTotal - u4LockRange);
    vRegWriteFldAlign(SCPIP_PIP_OA_11, u4LowerBound, PIP_OA_11_LOWER_BOUND1);   
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


void vScpipOnDIOutputISR(void)
{
    #if defined(CC_MT5396) && defined(SCPIP_TOGGLE_IMPORT_AT_INPUTV)
    vScpipSetFSCImportToggle();
    #endif
}

/**===========================below 6896 porting done==================================================================**/

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
        clipInfo->wHeight = picInfo->wSrcH;

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
        clipInfo->wX     = (UINT16)DIVIDE_WITH_CARRIER((UINT32)getMixedHPorch(bPath) * u4Factor, DFT_HPD_FACTOR);
        clipInfo->wWidth = (UINT16)DIVIDE_WITH_CARRIER((UINT32)picInfo->wTargetW * u4Factor,DFT_HPD_FACTOR);           
        clipInfo->wY = getMixedVPorch(bPath);        
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
            u4Delay= 2;  //delay 1 frame
        }
        else                                    // 3 frame mode
        {
            u4Delay= 3; // delay 1.5 frame
        }
    }

    return u4Delay * 10;    // (*10) request from cook.
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

                    vRegWriteFldAlign(SCPIP_US1_00, u4StartFactor,US1_00_USCALE_H_1);
                    vRegWriteFldAlign(SCPIP_US1_01, u4StepFactor, US1_01_NONL_H_USCALE_1); 
                    //vRegWriteFldAlign(SCPIP_US1_01, 0, US1_01_NONLINEAR_H_NEG_1); // always keep zero
                    vRegWriteFldAlign(SCPIP_US1_17, _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint << 2, US1_17_NONL_H_MIDDLE_1);
                    vRegWriteFldAlign(SCPIP_US1_17, 1, US1_17_NONL_H_STEP_1);
                    vRegWriteFldAlign(SCPIP_US1_17, (u4EndFactor - u4StartFactor)/u4StepFactor, US1_17_NONL_H_STOP_POINT_1);            
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
         /* Horizontal Direction */
        // scaler only handle even width because of h boundary
        if((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth % 2) && !bIsScalerInput444(u4VdpId))
        {
            _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth += 1;
        }
        if (_arScalePrm[u4VdpId].u4FixedPreScaler != 0)
        {
            UINT32 u4HpsFactor = 0;
            
            u4HpsFactor = u4ScpipGetHwHPS(bPath);            
            
            if (u4HpsFactor != _arScalePrm[u4VdpId].u4HpsFactor)            
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_PRE_DOWN, u4VdpId, 
                                            (VDP_SCPOS_RESCALE_TIME + VDP_MDDI_RESCALE_TIME),TRUE);
                //if Pre-scaling down factor changed, fire scaler mode change to enable scaler to recalculate parameter of pscan dispmode
                if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE)
                {
                    LOG(4,"HPS changed ==> fireSCPOSModeChange\n");
                    fireSCPOSModeChange(u4VdpId);
                }                
            }
        }
    }

    //calculate predown width
    _arScalePrm[u4VdpId].u4HpsWidth = DIVIDE_WITH_ROUND((picInfo->wSrcW * _arScalePrm[u4VdpId].u4HpsFactor), DFT_DS_FACTOR);
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
    }
}

static void _vScpipSetDisplayWindowReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    RDispPrm* dispPrm;
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    pipOffset = getPIPOffset(bPath);
       
    /* RVP/RHP/RVL/RHL -- Display Window Position and Size */
    #if defined(SUPPORT_TV3D_OVERSCAN)
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

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dramOffset = getDramOffset(bPath);
    dispPrm = getDispPrm(bPath);

    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        //when 3D is done by MJC, V overscan will done by MJC & H overscan will be done by PSC
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_HP_OFST_1);    
    }
    else if(vScpipIs3DDoneByScaler(u4VdpId)== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wHPosOfst, DRAM_M_1F_DA_OVERSCAN_LF);
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, dispPrm->wOverscanRight, DRAM_M_1F_DA_OVERSCAN_RT);        
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_HP_OFST_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_LF);    
        vRegWriteFldAlign(SCPIP_DRAM_M_1F+dramOffset, 0, DRAM_M_1F_DA_OVERSCAN_RT);
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, dispPrm->wHPosOfst, PIP1_04_HP_OFST_1);
    }
}

static void _vScpipSetVPorchReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 pipOffset;
    UINT16 dramOffset;
    RDispPrm* dispPrm;
    UINT32 vOfst;

    bPath = getScalerPath(u4VdpId);
    pipOffset = getPIPOffset(bPath);
    dramOffset = getDramOffset(bPath);    
    dispPrm = getDispPrm(bPath);
    
    /*6. WSTARTP_OFST -- Picture Target Y Offset */
    #ifdef __MODEL_slt__        
    vOfst = dispPrm->wVPosOfst;
    // to fix error lines at the top of the screen
    //vOfst = dispPrm->wVPosOfst+1;
    #else
    vOfst = dispPrm->wVPosOfst;
    #endif

    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        //when 3D is done by MJC, V overscan will done by MJC & H overscan will be done by PSC    
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_WSTARTP_OFST_1);
    }
    else if(vScpipIs3DDoneByScaler(u4VdpId)== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, vOfst, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, dispPrm->wOverscanBottom, DRAM_M_1E_DA_OVERSCAN_DN);
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_WSTARTP_OFST_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_UP);
        vRegWriteFldAlign(SCPIP_DRAM_M_1E+dramOffset, 0, DRAM_M_1E_DA_OVERSCAN_DN);
        vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, vOfst, PIP1_04_WSTARTP_OFST_1);        
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

        //5396 & 5368 will use 422 up in video timing. 5389 don't support 422 up.
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
    
    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    
    u4XOfstBeforeHPS =  getMixedHPorch(u4VdpId);
    u4HpsFactor = _arScalePrm[u4VdpId].u4HpsFactor;
    u4XOfstAfterHps = DIVIDE_WITH_CARRIER(u4XOfstBeforeHPS * u4HpsFactor, DFT_HPD_FACTOR);
    bMode = u1ScpipGetDispMode(bPath);
    
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
        dispPrm->wHPosOfst = u4XOfstAfterHps;
    }
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
    UINT32 u4DownScaledWidth;
    UINT32 u4DownScaledHeight;
    
    bPath = getScalerPath(u4VdpId);
    bMode = u1ScpipGetDispMode(bPath);    
    dispPrm = getDispPrm(bPath);
    dispInfo = getDispInfo(bPath);
    
    //Calculate down scaled width.
    u4DownScaledWidth = _arScalePrm[u4VdpId].u4DownScaledWidth;
    //calculate down scaled height.
    u4DownScaledHeight = _arScalePrm[u4VdpId].u4DownScaledHeight;

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
            dispPrm->u4USclHInit = _u4ScpipCalNegInitPhase(u4DownScaledWidth, dispInfo->wWidth, dispPrm->u4USclH);
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
            dispPrm->u4USclVInit = _u4ScpipCalNegInitPhase(u4DownScaledHeight, dispInfo->wHeight, dispPrm->u4USclV);
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
    vScpipSetHwHPSFactor(u4VdpId,_arScalePrm[u4VdpId].u4HpsFactor);

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
static void _vScpipCalScaleFactor(UINT32 u4VdpId, UINT32 in, UINT32 out, UINT32 u4LimitLength, UINT8 bDirection) 
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
    UINT32 u4SrcXOffset;
    UINT32 u4SrcYOffset;

    UINT32 u4OutWidth;
    UINT32 u4OutHeight;
    UINT32 u4OutXOffset;
    UINT32 u4OutYOffset;

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

    //pixel-based source (x,y,width,height) after apply source region & overscan
    u4SrcXOffset = getMixedHPorch(u4VdpId);
    u4SrcYOffset = getMixedVPorch(u4VdpId);
    
    u4SrcWidth  = (vScpipIs3DDoneByMJC()==SV_TRUE)? _arScalePrm[u4VdpId].u4HpsWidth : _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    u4SrcHeight = (vScpipIs3DDoneByMJC()==SV_TRUE)? picInfo->wSrcH : picInfo->wTargetH;

    //pixel-based output window
    u4OutXOffset = dispInfo->wXOffset;
    u4OutYOffset = dispInfo->wYOffset;
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
        //_au4ScalingFactorChanged[u4VdpId] = 1;        

        //if scaling factor changed, inform scaler to re-enter scaler dispmode or pscan dispmode
        if(u4ScalerMode==VDP_SCPOS_PSCAN_DISPMODE ||
                u4ScalerMode==VDP_SCPOS_DISPMODE_AUTO||
                u4ScalerMode==VDP_SCPOS_DISPMODE_MANUAL)
        {
            LOG(4,"Scaling factor changed ==>fireSCPOSModeChange()\n");
            fireSCPOSModeChange(u4VdpId);
        }        
    }
    #if 0
    else
    {
        _au4ScalingFactorChanged[u4VdpId] = 0;            
    }
    #endif

    if (isChannelOn != SV_OFF)
    {
        LOG(3, "SCALER(%d) O(%d, %d, %d, %d) S(%d, %d, %d, %d)\n",u4VdpId, 
                u4OutXOffset, u4OutYOffset,u4OutWidth,u4OutHeight,
                u4SrcXOffset,u4SrcYOffset,u4SrcWidth,u4SrcHeight);

        LOG(3, "HU(0x%x) VU(0x%x) HD(0x%x-0x%x) VD(0x%x)\n",
                dispPrm->u4USclH, dispPrm->u4USclV,
                _arScalePrm[u4VdpId].u4HpsFactor, 
                dispPrm->u4DSclH, dispPrm->u4DSclV);
    }
}

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

    // dispmode mode change handling
    if (fgIsMain && fgIsScposFlgSet(SCALER_MAIN_MODE_CHG))
    {
        vScpipModeSwitch(SV_VP_MAIN); 
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

#if defined(CC_MT5398)
VIDEO_RES_DESCRIPTION_T _vdoIORes[VDP_NS];
MODULE_RES_DESCRIPTION_T _modIORes[VDP_NS][VIDEO_RES_MODULE_NS];
SCALERINFO _fscInfo[VDP_NS];
#endif

#if defined(CC_MT5398)
/************* for 5398 FPGA*******************/
void vScpipSetForcedPDS(UINT32 u4VdpId, UINT32 hPds, UINT32 vPds)
{
    if(u4VdpId>= VDP_NS)
    {
        return;
    }

    _fscInfo[u4VdpId].u4ForcedHPreDsFactor = (hPds>=DFT_PDS_FACTOR)?DFT_PDS_FACTOR:hPds;
    _fscInfo[u4VdpId].u4ForcedVPreDsFactor = (vPds>=DFT_PDS_FACTOR)?DFT_PDS_FACTOR:vPds;

	vOnScalerInfoChg(u4VdpId);    
}

void vScpipSetForcedFrontBackDramMode(UINT32 u4VdpId,UINT32 u4Mode)
{
    if(u4VdpId>= VDP_NS)
    {
        return;
    }
    _fscInfo[u4VdpId].u4ForcedDramMode = u4Mode;
	vOnScalerInfoChg(u4VdpId);
}

void vScpipSetForced444Mode(UINT32 u4VdpId,UINT32 u4Mode)
{
    if(u4VdpId>= VDP_NS)
    {
        return;
    }
    _fscInfo[u4VdpId].u4Forced444= u4Mode;
    vOnScalerInfoChg(u4VdpId);
}

void vScpipSetForcedMFMode(UINT32 u4VdpId,UINT32 u4Mode)
{
    if(u4VdpId>= VDP_NS)
    {
        return;
    }
    _fscInfo[u4VdpId].u4Mirror = u4Mode & 0x2;
    _fscInfo[u4VdpId].u4Flip   = u4Mode & 0x1;
    vOnScalerInfoChg(u4VdpId);
}

void vScpipSetForcedNonLinear( UINT32 iNonLinearEN, UINT32 iInputTotal, UINT32 iInputMiddle, UINT32 iOutputTotal, UINT32 iOutputMiddle )
{
    UINT32 u4VdpId = SV_VP_MAIN;
    
    _fscInfo[u4VdpId].u4NonLinear = iNonLinearEN;
    _fscInfo[u4VdpId].u4NonLinearInTotal = iInputTotal;
    _fscInfo[u4VdpId].u4NonLinearInMiddle = iInputMiddle;
    _fscInfo[u4VdpId].u4NonLinearOutTotal = iOutputTotal;
    _fscInfo[u4VdpId].u4NonLinearOutMiddle = iOutputMiddle;
    vOnScalerInfoChg(u4VdpId);    
}

void vScpipSetForcedNonLinearFactor( UINT32 iInputTotal, UINT32 iInputMiddle, UINT32 iOutputTotal, UINT32 iOutputMiddle )
{
    UINT32 u4VdpId = SV_VP_MAIN;
    UINT32 InputSide  = ( iInputTotal  - iInputMiddle  )/2;
    UINT32 OutputSide = ( iOutputTotal - iOutputMiddle )/2;

    UINT32 MiddlePhase = ((INT64) (iInputMiddle-1)<<DFT_FRAC_PHASE_BITS(SV_VP_MAIN)) / (iOutputMiddle-1);
    UINT32 SidePhase   = ((INT64) (InputSide-1)   <<DFT_FRAC_PHASE_BITS(SV_VP_MAIN)) / (OutputSide-1);
    UINT32 NonlinearPhase;

    _fscInfo[u4VdpId].u4NonLinearStopPos   = OutputSide ;
    _fscInfo[u4VdpId].u4NonLinearMiddlePos = iOutputTotal/2;

    if( MiddlePhase <= SidePhase ) // upscaling at center
    {
        _fscInfo[u4VdpId].u4NonLinearPhaseSign = 0;
    }
    else // downscaling at center
    {
        _fscInfo[u4VdpId].u4NonLinearPhaseSign = 1;
    }

    if( _fscInfo[u4VdpId].u4NonLinearPhaseSign == 0 )
    {
        _fscInfo[u4VdpId].u4NonLinearPhaseIntg =          2*(InputSide*DFT_UNIT_PHASE(SV_VP_MAIN) - MiddlePhase*OutputSide )      /( OutputSide*OutputSide + OutputSide );
        _fscInfo[u4VdpId].u4NonLinearPhaseFrac = ((INT64)(2*(InputSide*DFT_UNIT_PHASE(SV_VP_MAIN) - MiddlePhase*OutputSide ))<<12)/( OutputSide*OutputSide + OutputSide ) & ( 4096 -1 );
        NonlinearPhase = MiddlePhase + OutputSide*_fscInfo[u4VdpId].u4NonLinearPhaseIntg + (( OutputSide*_fscInfo[u4VdpId].u4NonLinearPhaseFrac )>>12);
    }
    else
    {
        _fscInfo[u4VdpId].u4NonLinearPhaseIntg =          2*(MiddlePhase*OutputSide - InputSide*DFT_UNIT_PHASE(SV_VP_MAIN) )      /( OutputSide*OutputSide + OutputSide );
        _fscInfo[u4VdpId].u4NonLinearPhaseFrac = ((INT64)(2*(MiddlePhase*OutputSide - InputSide*DFT_UNIT_PHASE(SV_VP_MAIN) ))<<12)/( OutputSide*OutputSide + OutputSide ) & ( 4096 -1 );
        NonlinearPhase = MiddlePhase - OutputSide*_fscInfo[u4VdpId].u4NonLinearPhaseIntg - (( OutputSide*_fscInfo[u4VdpId].u4NonLinearPhaseFrac )>>12);
    }
    
    _fscInfo[u4VdpId].u4HDeltaPhaseIntg = NonlinearPhase>>DFT_FRAC_PHASE_BITS(SV_VP_MAIN); // Non-linear on main path only
    _fscInfo[u4VdpId].u4HDeltaPhaseFrac = NonlinearPhase &DFT_FRAC_PHASE_MASK(SV_VP_MAIN);
    _fscInfo[u4VdpId].u4HInitPhaseSign = 0; 
    _fscInfo[u4VdpId].u4HInitPhaseIntg = 0;
    _fscInfo[u4VdpId].u4HInitPhaseFrac = 0;

    
    Printf("Scaling phase H(0x%x-0x%x 0x%x 0x%x 0x%x)\n", _fscInfo[u4VdpId].u4NonLinearStopPos, _fscInfo[u4VdpId].u4NonLinearMiddlePos,
                                          _fscInfo[u4VdpId].u4NonLinearPhaseSign,
                                          _fscInfo[u4VdpId].u4NonLinearPhaseIntg,
                                          _fscInfo[u4VdpId].u4NonLinearPhaseFrac);
}


void vScpipIsTGUnderflow(UINT32 u4VdpId)
{
    UINT8 bIsUnder;

    if(u4VdpId >= VDP_NS)
    {
        return;
    }
    
    if(u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_SCSYS_01, 1, SCSYS_01_CLR_MTG_UDFL_FLG);
        //x_thread_delay(1);
        vRegWriteFldAlign(SCPIP_SCSYS_01, 0, SCSYS_01_CLR_MTG_UDFL_FLG);

        bIsUnder = RegReadFldAlign(SCPIP_SCSYS_01, SCSYS_01_STA_M_TG_UNDERFLOW);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SCSYS_01, 1, SCSYS_01_CLR_PTG_UDFL_FLG);
        //x_thread_delay(1);
        vRegWriteFldAlign(SCPIP_SCSYS_01, 0, SCSYS_01_CLR_PTG_UDFL_FLG);

        bIsUnder = RegReadFldAlign(SCPIP_SCSYS_01, SCSYS_01_STA_P_TG_UNDERFLOW);
    
    }
    Printf("%s Scaler underflow: %s\n", ((u4VdpId == VDP_1)?"Main":"Sub"), ((bIsUnder==1)?"TRUE":"FALSE"));
}


/********************* 
  * 0 -- frn_dram_mode
  * 1 -- bck_dram_mode
  *********************/
UINT32 ScpipIsDramBckMode(UINT32 vdpId, UINT32 inputwidth, 
							      UINT32 inputheight, 
							      UINT32 outputwidth, 
							      UINT32 outputheight)
{
	UINT32 ret;
	UINT32 inres, outres;

    ret = SV_FALSE;
    
    if(_fscInfo[vdpId].u4ForcedDramMode == 0)  //auto
    {
        inres = inputwidth * inputheight;
	    outres = outputwidth * outputheight;

    	if(inres > outres)
    	{
    		ret = SV_TRUE;
    	}
    	else
    	{
    		ret = SV_FALSE;
    	}        
    }
    else if(_fscInfo[vdpId].u4ForcedDramMode == 1) //force front mode
    {
        ret = SV_FALSE;
    }
    else    //force back mode
    {
		ret = SV_TRUE;
    }

	return ret;
}

void vScpipCalcPreDSFactor(UINT32 u4VdpId, MODULE_RES_DESCRIPTION_T* ioRes)
{
    UINT32 hFactor;
    UINT32 vFactor;
    Printf("PreDS in(x,y,w,h)= (%d,%d,%d,%d)\n",ioRes->inRes.u4X,ioRes->inRes.u4Y,ioRes->inRes.u4W,ioRes->inRes.u4H);
    Printf("PreDS out(x,y,w,h)= (%d,%d,%d,%d)\n",ioRes->outRes.u4X,ioRes->outRes.u4Y,ioRes->outRes.u4W,ioRes->outRes.u4H);    

    //H pre ds factor
    if(_fscInfo[u4VdpId].u4ForcedHPreDsFactor != 0)
    {
        hFactor = _fscInfo[u4VdpId].u4ForcedHPreDsFactor;
    }
    else
    {
        if(ioRes->inRes.u4W <= ioRes->outRes.u4W)
        {
            hFactor = DFT_HPD_FACTOR;
        }
        else
        {
            hFactor = (ioRes->outRes.u4W * DFT_HPD_FACTOR)/ ioRes->inRes.u4W;
        }
    }
    //V pre ds factor
    if(_fscInfo[u4VdpId].u4ForcedVPreDsFactor != 0)
    {
        vFactor = _fscInfo[u4VdpId].u4ForcedVPreDsFactor;
    }
    else
    {
        if(ioRes->inRes.u4H <= ioRes->outRes.u4H)
        {
            vFactor = DFT_HPD_FACTOR;
        }
        else
        {
            vFactor = (ioRes->outRes.u4H * DFT_HPD_FACTOR)/ ioRes->inRes.u4H;
        }
    }

    _fscInfo[u4VdpId].u4HPreDsFactor = hFactor;
    _fscInfo[u4VdpId].u4VPreDsFactor = vFactor;
    _fscInfo[u4VdpId].u4PreDsInW = ioRes->inRes.u4W;
    _fscInfo[u4VdpId].u4PreDsInH = ioRes->inRes.u4H;
    _fscInfo[u4VdpId].u4PreDsOutW = ioRes->outRes.u4W;
    _fscInfo[u4VdpId].u4PreDsOutH = ioRes->outRes.u4H;    
}

void vScpipCalcScalingFactor ( UINT32 u4VdpId,UINT32 inW,UINT32 inH,UINT32 outW,UINT32 outH)
{
    UINT32 u4HPhaseInc;
 	UINT32 u4VPhaseInc;
 	UINT8 bPath;
 
	if(outW ==0 || outH == 0)
	{
	    Printf("Output Width or Height is 0\n");
	    return;
	}
    bPath = getScalerPath(u4VdpId);
	u4HPhaseInc = (inW * DFT_UNIT_PHASE(bPath)) / outW;
	u4VPhaseInc = (inH * DFT_UNIT_PHASE(bPath)) / outH;

	Printf("scaling factor in(%d %d) out(%d %d)\n", inW, inH, outW, outH);
		
	_fscInfo[u4VdpId].u4HDeltaPhaseIntg = u4HPhaseInc >> DFT_FRAC_PHASE_BITS(bPath);	
	_fscInfo[u4VdpId].u4HDeltaPhaseFrac = u4HPhaseInc & DFT_FRAC_PHASE_MASK(bPath);
	_fscInfo[u4VdpId].u4VDeltaPhaseIntg = u4VPhaseInc >> DFT_FRAC_PHASE_BITS(bPath);
	_fscInfo[u4VdpId].u4VDeltaPhaseFrac = u4VPhaseInc & DFT_FRAC_PHASE_MASK(bPath);

	Printf("scaling factor H(0x%x 0x%x) V(0x%x 0x%x)\n", _fscInfo[u4VdpId].u4HDeltaPhaseIntg, 
                                        	    _fscInfo[u4VdpId].u4HDeltaPhaseFrac, 
                                        	    _fscInfo[u4VdpId].u4VDeltaPhaseIntg, 
                                        	    _fscInfo[u4VdpId].u4VDeltaPhaseFrac);

	return;
}

void vScpipSetRegen3DMode(UINT32 u4VdpId,UINT32 u4Regen3DType)
{
    if(u4VdpId > VDP_NS)
    {
        return;
    }
    
    _fscInfo[u4VdpId].eRegenIn3DType = (SCALER_TDTV_IN_TYPE)u4Regen3DType;
}

void vScpipCalcScalingPhase(UINT32 u4VdpId, UINT32 inW,UINT32 inH, UINT32 outW,UINT32 outH)
{
	UINT32 u4HPhaseInc;
	UINT32 u4VPhaseInc;
	UINT32 u4PhaseExceeded;
	UINT8 bPath;
	
	if(outW ==0 || outH ==0)
	{
		Printf("!!Output size is 0. outW(%d) outH(%d)\n", outW, outH);
		return;
	}
    bPath = getScalerPath(u4VdpId);
	u4HPhaseInc = (inW * DFT_UNIT_PHASE(bPath)) / outW;
	u4VPhaseInc = (inH * DFT_UNIT_PHASE(bPath)) / outH;

	// H Initial Phase
    if(inW < outW)    /* h up scaling */
    {
	    _fscInfo[u4VdpId].u4HInitPhaseIntg = 0;
	    if(_fscInfo[u4VdpId].u4HDeltaPhaseFrac == 0)
    	{
			_fscInfo[u4VdpId].u4HInitPhaseFrac = 0;
			_fscInfo[u4VdpId].u4HInitPhaseSign = 0;    	
    	}
	    else
    	{
			_fscInfo[u4VdpId].u4HInitPhaseFrac = DFT_UNIT_PHASE(bPath) + 
				( ((inW-1)*DFT_UNIT_PHASE(bPath)) - ((outW-1)*_fscInfo[u4VdpId].u4HDeltaPhaseFrac))/2;
			_fscInfo[u4VdpId].u4HInitPhaseSign = 1;            
    	}	    
    }
    else    /* h down scaling */
    {
    	u4PhaseExceeded = (inW -1)* DFT_UNIT_PHASE(bPath) - (outW-1)*u4HPhaseInc;
    	
		_fscInfo[u4VdpId].u4HInitPhaseIntg = (u4PhaseExceeded/2)>>DFT_FRAC_PHASE_BITS(bPath);
		_fscInfo[u4VdpId].u4HInitPhaseFrac = (u4PhaseExceeded/2) & DFT_FRAC_PHASE_MASK(bPath);
		_fscInfo[u4VdpId].u4HInitPhaseSign = 0;		
    }

	// V Initial Phase
    if(inH < outH)    /* v up scaling */
    {
    	_fscInfo[u4VdpId].u4VInitPhaseIntg = 0;
    	
		if(_fscInfo[u4VdpId].u4VDeltaPhaseFrac == 0)
		{
			_fscInfo[u4VdpId].u4VInitPhaseFrac = 0;
			_fscInfo[u4VdpId].u4VInitPhaseSign = 0;
		}
		else
		{
			_fscInfo[u4VdpId].u4VInitPhaseFrac = DFT_UNIT_PHASE(bPath) + 
				( ((inH-1)*DFT_UNIT_PHASE(bPath)) - ((outH-1)*_fscInfo[u4VdpId].u4VDeltaPhaseFrac))/2;
			_fscInfo[u4VdpId].u4VInitPhaseSign = 1;
		}
    }
    else    		/* v down scaling */
    {
    	u4PhaseExceeded = (inH -1)* DFT_UNIT_PHASE(bPath) - (outH-1)*u4VPhaseInc;
		_fscInfo[u4VdpId].u4VInitPhaseIntg = (u4PhaseExceeded/2)>>DFT_FRAC_PHASE_BITS(bPath);
		_fscInfo[u4VdpId].u4VInitPhaseFrac = (u4PhaseExceeded/2) & DFT_FRAC_PHASE_MASK(bPath);
		_fscInfo[u4VdpId].u4VInitPhaseSign = 0;
    }

    if(_fscInfo[u4VdpId].u4Is444)
    {
        _fscInfo[u4VdpId].u4HUVInitPhaseSign = _fscInfo[u4VdpId].u4HInitPhaseSign;
        _fscInfo[u4VdpId].u4HUVInitPhaseIntg = _fscInfo[u4VdpId].u4HInitPhaseIntg;
        _fscInfo[u4VdpId].u4HUVInitPhaseFrac = _fscInfo[u4VdpId].u4HInitPhaseFrac;
    }
    else // 422
    {
        _fscInfo[u4VdpId].u4HUVInitPhaseSign = _fscInfo[u4VdpId].u4HInitPhaseSign;
        _fscInfo[u4VdpId].u4HUVInitPhaseIntg = _fscInfo[u4VdpId].u4HInitPhaseIntg;
        if(_fscInfo[u4VdpId].u4HInitPhaseSign == 0) // downscaling or NO scaling
        {
            _fscInfo[u4VdpId].u4HUVInitPhaseFrac = _fscInfo[u4VdpId].u4HInitPhaseFrac;
            if(_fscInfo[u4VdpId].u4Mirror != 0 && _fscInfo[u4VdpId].u4IsDramFrontMode)
            {
                _fscInfo[u4VdpId].u4HUVInitPhaseFrac = _fscInfo[u4VdpId].u4HInitPhaseFrac + DFT_UNIT_PHASE(bPath);
            }
        }
        else // upscaling
        {
            _fscInfo[u4VdpId].u4HUVInitPhaseFrac = _fscInfo[u4VdpId].u4HInitPhaseFrac + DFT_UNIT_PHASE(bPath);
            if(_fscInfo[u4VdpId].u4Mirror != 0 && _fscInfo[u4VdpId].u4IsDramFrontMode)
            {
                _fscInfo[u4VdpId].u4HUVInitPhaseFrac = _fscInfo[u4VdpId].u4HInitPhaseFrac;
            }
        }
    }

	Printf("Scaling phase H(0x%x 0x%x 0x%x)\n", _fscInfo[u4VdpId].u4HInitPhaseSign,
										  _fscInfo[u4VdpId].u4HInitPhaseIntg,
										  _fscInfo[u4VdpId].u4HInitPhaseFrac);
	Printf("Scaling phase V(0x%x 0x%x 0x%x)\n", _fscInfo[u4VdpId].u4VInitPhaseSign,
										  _fscInfo[u4VdpId].u4VInitPhaseIntg,
										  _fscInfo[u4VdpId].u4VInitPhaseFrac);
	Printf("Scaling phase HUV(0x%x 0x%x 0x%x)\n", _fscInfo[u4VdpId].u4HUVInitPhaseSign,
										  _fscInfo[u4VdpId].u4HUVInitPhaseIntg,
										  _fscInfo[u4VdpId].u4HUVInitPhaseFrac);
	
	return;
}

#if 0
void calcScalingFactorAndInitPhase(int inWidth, int inHeight,int outWidth, int outHeight, UINT drammode)
{
    int v_phase_inc, h_phase_inc;
    int v_delta_int, v_delta_frac, h_delta_int, h_delta_frac;
    int v_init_phase_sign, v_init_phase_int, v_init_phase_frac;
    int h_init_phase_sign, h_init_phase_int, h_init_phase_frac;
    int h_uv_init_phase_sign, h_uv_init_phase_int, h_uv_init_phase_frac;
    int dram_444, h_422_mode;
    int iphaseExceeded;

    printf("global 444(%d) mirror(%d) drammode(%d)\n", _Is444Input, _IsScalerMirrorOn, _ForceDramMode);

    v_phase_inc = inHeight * DEFAULT_FACTOR / outHeight;
    v_delta_int = v_phase_inc >> DEFAULT_BITS;
    v_delta_frac = v_phase_inc & DEFAULT_MASK;

    if(0 == v_delta_int)    /* v up scaling */
    {
        v_init_phase_int = 0;
        if(0 == v_delta_frac)
        {
            v_init_phase_frac = 0;
            v_init_phase_sign = 0;
        }
        else
        {
            v_init_phase_frac = DEFAULT_FACTOR +
                            ((inHeight - 1) * DEFAULT_FACTOR - (outHeight - 1) * v_delta_frac) / 2;
            v_init_phase_sign = 1;
        }
    }
    else    /* v down scaling */
    {
        iphaseExceeded = (inHeight - 1) * DEFAULT_FACTOR - (outHeight - 1) * v_phase_inc;
        v_init_phase_int = (iphaseExceeded / 2) >> DEFAULT_BITS;
        v_init_phase_frac = (iphaseExceeded / 2) & DEFAULT_MASK;
        v_init_phase_sign = 0;
    }

    /* TO DO, add 3d related phase calculation */
    /* TO DO, add nonlinear related phase calculation */

    h_phase_inc = inWidth * DEFAULT_FACTOR / outWidth;
    h_delta_int = h_phase_inc >> DEFAULT_BITS;
    h_delta_frac = h_phase_inc & DEFAULT_MASK;

    if(0 == h_delta_int)    /* h up scaling */
    {
        h_init_phase_int = 0;
        if(0 == h_delta_frac)
        {
            h_init_phase_frac = 0;
            h_init_phase_sign = 0;
        }
        else
        {
            h_init_phase_frac = DEFAULT_FACTOR +
                            ((inWidth - 1) * DEFAULT_FACTOR - (outWidth - 1) * h_delta_frac) / 2;
            h_init_phase_sign = 1;
        }
    }
    else    /* h down scaling */
    {
        iphaseExceeded = (inWidth - 1) * DEFAULT_FACTOR - (outWidth - 1) * h_phase_inc;
        h_init_phase_int = (iphaseExceeded / 2) >> DEFAULT_BITS;
        h_init_phase_frac = (iphaseExceeded / 2) & DEFAULT_MASK;
        h_init_phase_sign = 0;
    }

    if(TRUE == _Is444Input)
    {
        dram_444 = 1;
        h_422_mode = 0;
        h_uv_init_phase_sign = h_init_phase_sign;
        h_uv_init_phase_int = h_init_phase_int;
        h_uv_init_phase_frac = h_init_phase_frac;
    }
    else
    {
        h_422_mode = 1;
        h_uv_init_phase_sign = h_init_phase_sign;
        h_uv_init_phase_int = h_init_phase_int;

        if(FRN_MODE == drammode)
        {
            dram_444 = 0;
        }
        else if(BCK_MODE == drammode)
        {
            dram_444 = 1;
        }
        else
        {
            printf("my GOD, you lost again\n");
        }

        if(0 == h_init_phase_sign)    /*h down scaling*/
        {
            if(_IsScalerMirrorOn)
            {
                if(FRN_MODE == drammode)
                {
                    h_uv_init_phase_frac = h_init_phase_frac + DEFAULT_FACTOR;
                }
                else if(BCK_MODE == drammode)
                {
                    h_uv_init_phase_frac = h_init_phase_frac;
                }
                else
                {
                    printf("how can you take this way?");
                }
            }
            else
            {
                h_uv_init_phase_frac = h_init_phase_frac;
            }
        }
        else                    /*h up scaling*/
        {
            if(_IsScalerMirrorOn)
            {
                if(FRN_MODE == drammode)
                {
                    h_uv_init_phase_frac = h_init_phase_frac;
                }
                else if(BCK_MODE == drammode)
                {
                    h_uv_init_phase_frac = h_init_phase_frac + DEFAULT_FACTOR;
                }
                else
                {
                    printf("how can you take this way?");
                }
            }
            else
            {
                h_uv_init_phase_frac = h_init_phase_frac + DEFAULT_FACTOR;
            }
        }
        
        if(h_uv_init_phase_frac >= DEFAULT_FACTOR * 2)
        {
            h_uv_init_phase_int += 1;
            h_uv_init_phase_frac -= (DEFAULT_FACTOR * 2);
        }
    }

    value[H_PHASE_DELTA_INTG_1] = h_delta_int;
    value[H_PHASE_DELTA_FRAC_1] = h_delta_frac;
    value[H_PHASE_INIT_SIGN_1] = h_init_phase_sign;
    value[H_PHASE_INIT_INTG_1] = h_init_phase_int;
    value[H_PHASE_INIT_FRAC_1] = h_init_phase_frac;

    value[V_PHASE_DELTA_INTG_1] = v_delta_int;
    value[V_PHASE_DELTA_FRAC_1] = v_delta_frac;
    value[V_PHASE_INIT_SIGN_1] = v_init_phase_sign;
    value[V_PHASE_INIT_INTG_1] = v_init_phase_int;
    value[V_PHASE_INIT_FRAC_1] = v_init_phase_frac;

    value[DRAM_W_444_1] = dram_444;
    value[H_422_MODE_EN_1] = h_422_mode;
    value[H_UV_PHASE_INIT_SIGN] = h_uv_init_phase_sign;
    value[H_UV_PHASE_INIT_INTG] = h_uv_init_phase_int;
    value[H_UV_PHASE_INIT_FRAC] = h_uv_init_phase_frac;

    value[V_PHASE_INIT_SIGN_BTM_LEFT_1] = v_init_phase_sign;
    value[V_PHASE_INIT_INTG_BTM_LEFT_1] = v_init_phase_int;
    value[V_PHASE_INIT_FRAC_BTM_LEFT_1] = v_init_phase_frac;

    printf("inter(%x %x) frac(%x %x) 444(%d) h_422(%d)\n",
                                         value[H_PHASE_DELTA_INTG_1],
                                         value[V_PHASE_DELTA_INTG_1],
                                         value[H_PHASE_DELTA_FRAC_1],
                                         value[V_PHASE_DELTA_FRAC_1],
                                         value[DRAM_W_444_1],
                                         value[H_422_MODE_EN_1]);
    printf("init phase H(SIGN: %d, INT: %d, FRAC: %x) UV(SIGN: %d, INT: %d, FRAC: %x) V(SIGN: %d, INT: %d, FRAC: %x)\n",
            h_init_phase_sign, h_init_phase_int, h_init_phase_frac,
            h_uv_init_phase_sign, h_uv_init_phase_int, h_uv_init_phase_frac,
            v_init_phase_sign, v_init_phase_int, v_init_phase_frac);
}
#endif


void vUpdateRptGenCount(UINT32 u4VdpId, UINT32 u4DramHeight)
{
	_fscInfo[u4VdpId].u4RptGenCount = (u4DramHeight * 3) >> 2;

	return;
}

void vUpdateColorFormat(UINT32 u4VdpId, UINT32 u4ColorFormat)
{
    if(u4ColorFormat == 1) // force 422
    {
        _fscInfo[u4VdpId].u4Is444 = 1;
    }
    else if (u4ColorFormat == 2) // force 444
    {
        _fscInfo[u4VdpId].u4Is444 = 0;
    }

    return;
}

void vScpipUpdateRegenPrm(UINT32 u4VdpId, MODULE_RES_DESCRIPTION_T* regenRes)
{
    if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_SBS)
    {
       	_fscInfo[u4VdpId].u4RegenOutX = regenRes->outRes.u4X>>1;
    	_fscInfo[u4VdpId].u4RegenOutY = regenRes->outRes.u4Y;
    	_fscInfo[u4VdpId].u4RegenOutW = regenRes->outRes.u4W>>1;	
    	_fscInfo[u4VdpId].u4RegenOutH = regenRes->outRes.u4H;    
        _fscInfo[u4VdpId].u4RegenInW = regenRes->inRes.u4W>> 1;	
    	_fscInfo[u4VdpId].u4RegenInH = regenRes->inRes.u4H;
    }
    else if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_TAB)
    {
       	_fscInfo[u4VdpId].u4RegenOutX = regenRes->outRes.u4X;
    	_fscInfo[u4VdpId].u4RegenOutY = regenRes->outRes.u4Y>>1;
    	_fscInfo[u4VdpId].u4RegenOutW = regenRes->outRes.u4W;	
    	_fscInfo[u4VdpId].u4RegenOutH = regenRes->outRes.u4H>>1;    
        _fscInfo[u4VdpId].u4RegenInW = regenRes->inRes.u4W;	
    	_fscInfo[u4VdpId].u4RegenInH = regenRes->inRes.u4H>>1;
    }
    else
    {
       	_fscInfo[u4VdpId].u4RegenOutX = regenRes->outRes.u4X;
    	_fscInfo[u4VdpId].u4RegenOutY = regenRes->outRes.u4Y;
    	_fscInfo[u4VdpId].u4RegenOutW = regenRes->outRes.u4W;	
    	_fscInfo[u4VdpId].u4RegenOutH = regenRes->outRes.u4H;	
    	_fscInfo[u4VdpId].u4RegenInW = regenRes->inRes.u4W;	
    	_fscInfo[u4VdpId].u4RegenInH = regenRes->inRes.u4H;
    }     
}

static void _vScpipUpdateRegenReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 regenOffset;
    UINT8 b3DTypeRegValue;
    bPath = getScalerPath(u4VdpId);
    regenOffset = getRegenOffset(bPath);    
    b3DTypeRegValue = 0;    // defaule is 2D
    
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_00+regenOffset, 1, RG_M_REGEN_ENABLE);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_00+regenOffset, 1, RG_M_CLIP_EN);    
    if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_SBS)
    {
        b3DTypeRegValue = 1<<3;
    }
    else if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_TAB)
    {
        b3DTypeRegValue = 1<<2;
    }

    vRegWriteFldAlign(PRE_RE_GEN_MAIN_00+regenOffset, b3DTypeRegValue, RG_M_IN_FORMAT);        
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_03+regenOffset, _fscInfo[u4VdpId].u4RegenOutX, RG_M_H_START_L);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_03+regenOffset, _fscInfo[u4VdpId].u4RegenOutY, RG_M_V_START_L);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_04+regenOffset, _fscInfo[u4VdpId].u4RegenOutX, RG_M_H_START_R);    
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_04+regenOffset, _fscInfo[u4VdpId].u4RegenOutY, RG_M_V_START_R);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_02+regenOffset, _fscInfo[u4VdpId].u4RegenOutW, RG_M_OUT_ACTIVE_WIDTH);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_02+regenOffset, _fscInfo[u4VdpId].u4RegenOutH, RG_M_OUT_ACTIVE_HEIGHT);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_01+regenOffset, _fscInfo[u4VdpId].u4RegenInW, RG_M_IN_ACTIVE_WIDTH);
    vRegWriteFldAlign(PRE_RE_GEN_MAIN_01+regenOffset, _fscInfo[u4VdpId].u4RegenInH, RG_M_IN_ACTIVE_HEIGHT);
}

void vUpdateDramHVwidth( UINT32 u4VdpId, 
     					   UINT32 inputwidth, 
     				       UINT32 inputheight, 
     				       UINT32 outputwidth, 
     				       UINT32 outputheight)
{
	INT32 IsDramBckMode = 0;
	IsDramBckMode = ScpipIsDramBckMode(u4VdpId,inputwidth,inputheight,outputwidth,outputheight);

	if(IsDramBckMode)
	{
	    _fscInfo[u4VdpId].u4IsDramFrontMode = SV_FALSE;	
		_fscInfo[u4VdpId].u4DramSRCWidth = outputwidth;		
		_fscInfo[u4VdpId].u4DramWidth =  outputwidth;
		_fscInfo[u4VdpId].u4DramHeight = outputheight;
		_fscInfo[u4VdpId].u4DramHPitch = outputwidth;
		_fscInfo[u4VdpId].u4DramVPitch = outputheight;

		Printf("==> DramBckMode, dram HV(%d %d), pitch(%d %d)\n", outputwidth, outputheight, outputwidth, outputheight);
	}
	else
	{
	    _fscInfo[u4VdpId].u4IsDramFrontMode = SV_TRUE;	
		_fscInfo[u4VdpId].u4DramSRCWidth = inputwidth;		
		_fscInfo[u4VdpId].u4DramWidth =  inputwidth;
		_fscInfo[u4VdpId].u4DramHeight = inputheight;
		_fscInfo[u4VdpId].u4DramHPitch = inputwidth;
		_fscInfo[u4VdpId].u4DramVPitch = inputheight;

		Printf("==> DramFrnMode, dram HV(%d %d), pitch(%d %d)\n", inputwidth, inputheight, inputwidth, inputheight);
	}

	return;
}

void vScpipSetScalerReg(UINT32 u4VdpId)
{   
    UINT8 bPath;
    UINT16 pdsOffset;
    UINT16 ctrlOffset;
    UINT16 mskOffset;

    bPath = getScalerPath(u4VdpId);
    pdsOffset = getPDSOffset(bPath);
    ctrlOffset = getCtrlOffset(bPath);
    mskOffset = getMskOffset(bPath);
    
    //pre ds
    vRegWriteFldAlign(PRE_DS_MAIN_00+pdsOffset, _fscInfo[u4VdpId].u4HPreDsFactor, PRE_DS_MAIN_00_PRE_DSCALE_H_1);	
    vRegWriteFldAlign(PRE_DS_MAIN_00+pdsOffset, _fscInfo[u4VdpId].u4VPreDsFactor, PRE_DS_MAIN_00_PRE_DSCALER_V_1);
    vRegWriteFldAlign(PRE_DS_MAIN_01+pdsOffset, DFT_PDS_FACTOR - _fscInfo[u4VdpId].u4HPreDsFactor, PRE_DS_MAIN_01_PRE_DSCALE_H_INIT_1);	
    vRegWriteFldAlign(PRE_DS_MAIN_01+pdsOffset, DFT_PDS_FACTOR - _fscInfo[u4VdpId].u4VPreDsFactor, PRE_DS_MAIN_01_PRE_DSCALE_V_INIT_1);    
    vRegWriteFldAlign(PRE_DS_MAIN_04+pdsOffset, _fscInfo[u4VdpId].u4PreDsInW, PRE_DS_MAIN_04_PREDS_MAIN_IN_H);	
    vRegWriteFldAlign(PRE_DS_MAIN_04+pdsOffset, _fscInfo[u4VdpId].u4PreDsInH, PRE_DS_MAIN_04_PREDS_MAIN_IN_V);	
    vRegWriteFldAlign(PRE_DS_MAIN_03+pdsOffset, _fscInfo[u4VdpId].u4PreDsOutW, PRE_DS_MAIN_03_PREDS_MAIN_OUT_H);	
    vRegWriteFldAlign(PRE_DS_MAIN_03+pdsOffset, _fscInfo[u4VdpId].u4PreDsOutH, PRE_DS_MAIN_03_PREDS_MAIN_OUT_V);

    //regen register
    _vScpipUpdateRegenReg(u4VdpId);

    if(BSP_IsFPGA())
    {
        //only for fpga
        vIO32WriteFldAlign(FPGA_NXNR_RES, _fscInfo[u4VdpId].u4NXNRWidth, FPGA_NXNR_WIDTH);    
        vIO32WriteFldAlign(FPGA_NXNR_RES, _fscInfo[u4VdpId].u4NXNRHeight, FPGA_NXNR_HEIGHT);    
        //FPGA only for main
        vRegWriteFldAlign(MCVP_KC_0A, _fscInfo[u4VdpId].u4NXNRWidth, HDEW);
        vRegWriteFldAlign(MCVP_KC_0A, _fscInfo[u4VdpId].u4NXNRHeight, VDEW);
    }
    
	//add import
    vRegWriteFldAlign(SCPIP_SCCTRL1_0A+ctrlOffset, 0, SCCTRL1_0A_SC_SET_RES_TOGGLE_1);	
	/* input timing */
	vRegWriteFldAlign(SCPIP_SCCTRL1_09+ctrlOffset, _fscInfo[u4VdpId].u4InputWidth, SCCTRL1_09_INPUT_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_09+ctrlOffset, _fscInfo[u4VdpId].u4InputHeight, SCCTRL1_09_INPUT_HEIGHT_1);

	/* dram width or height */
	vRegWriteFldAlign(SCPIP_SCCTRL1_0C+ctrlOffset, _fscInfo[u4VdpId].u4DramSRCWidth, SCCTRL1_0C_SC_DRAM_SRC_W_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1C+ctrlOffset, _fscInfo[u4VdpId].u4DramWidth, SCCTRL1_1C_DRAM_W_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1F+ctrlOffset, _fscInfo[u4VdpId].u4DramHPitch, SCCTRL1_1F_WR_DRAM_H_PITCH_1);

	vRegWriteFldAlign(SCPIP_SCCTRL1_1C+ctrlOffset, _fscInfo[u4VdpId].u4DramHeight, SCCTRL1_1C_DRAM_W_HEIGHT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1E+ctrlOffset, _fscInfo[u4VdpId].u4DramVPitch, SCCTRL1_1E_WR_DRAM_V_PITCH_1);

	/* scaling factor */
	vRegWriteFldAlign(SCPIP_SCCTRL1_01+ctrlOffset, _fscInfo[u4VdpId].u4HDeltaPhaseIntg, SCCTRL1_01_H_PHASE_DELTA_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_01+ctrlOffset, _fscInfo[u4VdpId].u4HDeltaPhaseFrac, (bPath==SV_VP_MAIN)?SCCTRL1_01_H_PHASE_DELTA_FRAC_1: SCCTRL2_01_H_PHASE_DELTA_FRAC_2);
	vRegWriteFldAlign(SCPIP_SCCTRL1_03+ctrlOffset, _fscInfo[u4VdpId].u4VDeltaPhaseIntg, SCCTRL1_03_V_PHASE_DELTA_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_03+ctrlOffset, _fscInfo[u4VdpId].u4VDeltaPhaseFrac, (bPath==SV_VP_MAIN)?SCCTRL1_03_V_PHASE_DELTA_FRAC_1: SCCTRL2_03_V_PHASE_DELTA_FRAC_2);	

	/* initial phase */
	vRegWriteFldAlign(SCPIP_SCCTRL1_00+ctrlOffset, _fscInfo[u4VdpId].u4HInitPhaseSign, SCCTRL1_00_H_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_00+ctrlOffset, _fscInfo[u4VdpId].u4HInitPhaseIntg, SCCTRL1_00_H_PHASE_INIT_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_00+ctrlOffset, _fscInfo[u4VdpId].u4HInitPhaseFrac, (bPath==SV_VP_MAIN)?SCCTRL1_00_H_PHASE_INIT_FRAC_1:SCCTRL2_00_H_PHASE_INIT_FRAC_2);

	vRegWriteFldAlign(SCPIP_SCCTRL1_02+ctrlOffset, _fscInfo[u4VdpId].u4VInitPhaseSign, SCCTRL1_02_V_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_02+ctrlOffset, _fscInfo[u4VdpId].u4VInitPhaseIntg, SCCTRL1_02_V_PHASE_INIT_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_02+ctrlOffset, _fscInfo[u4VdpId].u4VInitPhaseFrac, (bPath==SV_VP_MAIN)?SCCTRL1_02_V_PHASE_INIT_FRAC_1:SCCTRL2_02_V_PHASE_INIT_FRAC_2);

	if (u4VdpId == SV_VP_MAIN)
	{
	vRegWriteFldAlign(SCPIP_SCCTRL1_04+ctrlOffset, _fscInfo[u4VdpId].u4HUVInitPhaseSign, SCCTRL1_04_H_UV_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_04+ctrlOffset, _fscInfo[u4VdpId].u4HUVInitPhaseIntg, SCCTRL1_04_H_UV_PHASE_INIT_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_04+ctrlOffset, _fscInfo[u4VdpId].u4HUVInitPhaseFrac, SCCTRL1_04_H_UV_PHASE_INIT_FRAC_1);

	/* non-linear */
	vRegWriteFldAlign(SCPIP_SCCTRL1_05+mskOffset, _fscInfo[u4VdpId].u4NonLinear, SCCTRL1_05_NONL_H_ENABLE);
	vRegWriteFldAlign(SCPIP_SCCTRL1_05+mskOffset, _fscInfo[u4VdpId].u4NonLinearPhaseSign, SCCTRL1_05_NONL_H_ADD_MINUS_SEL);
	vRegWriteFldAlign(SCPIP_SCCTRL1_05+mskOffset, _fscInfo[u4VdpId].u4NonLinearStopPos, SCCTRL1_05_NONL_H_STOP_POS);
	vRegWriteFldAlign(SCPIP_SCCTRL1_07+mskOffset, _fscInfo[u4VdpId].u4NonLinearMiddlePos, SCCTRL1_07_NONL_H_MIDDLE_POS);
	vRegWriteFldAlign(SCPIP_SCCTRL1_06+mskOffset, _fscInfo[u4VdpId].u4NonLinearPhaseIntg, SCCTRL1_06_NONL_H_PHASE_DELTA);
	vRegWriteFldAlign(SCPIP_SCCTRL1_06+mskOffset, _fscInfo[u4VdpId].u4NonLinearPhaseFrac, SCCTRL1_06_NONL_H_PHASE_DELTA_FLOAT);
	}
	
	/* output timing*/
	vRegWriteFldAlign(SCPIP_SCSYS_06+mskOffset, _fscInfo[u4VdpId].u4OutputX, SCSYS_06_HMSK_STA_OFST_1);
	vRegWriteFldAlign(SCPIP_SCSYS_07+mskOffset, _fscInfo[u4VdpId].u4OutputY, SCSYS_07_VMSK_STA_OFST_1);	
	vRegWriteFldAlign(SCPIP_SCCTRL1_11+ctrlOffset, _fscInfo[u4VdpId].u4OutputWidth, SCCTRL1_11_OUTPUT_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_11+ctrlOffset, _fscInfo[u4VdpId].u4OutputHeight, SCCTRL1_11_OUTPUT_HEIGHT_1);

	/* dram back mode || frn mode */
	if(_fscInfo[u4VdpId].u4Dispmode != VDP_SCPOS_DISPMODE_OFF)
	{
    	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 0, SCCTRL1_13_BCK_DRAM_MODE_1);
    	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 0, SCCTRL1_13_FRN_DRAM_MODE_1);
	}
	else
	{
    	if(_fscInfo[u4VdpId].u4IsDramFrontMode == SV_TRUE)
    	{
        	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 0, SCCTRL1_13_BCK_DRAM_MODE_1);
        	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 1, SCCTRL1_13_FRN_DRAM_MODE_1);
    	}
    	else
    	{
        	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 1, SCCTRL1_13_BCK_DRAM_MODE_1);
        	vRegWriteFldAlign(SCPIP_SCCTRL1_13+ctrlOffset, 0, SCCTRL1_13_FRN_DRAM_MODE_1);
    	}
	}	
	/* the time gen read pointer */
	vRegWriteFldAlign(SCPIP_SCCTRL1_1E+ctrlOffset, _fscInfo[u4VdpId].u4RptGenCount, SCCTRL1_1E_RPT_GEN_VCNT_1);

    vRegWriteFldAlign(SCPIP_SCCTRL1_0A+ctrlOffset, 1, SCCTRL1_0A_SC_SET_RES_TOGGLE_1);	
    vRegWriteFldAlign(SCPIP_SCSYS_01, 1, SCSYS_01_SET_DATA);    
	return;
}

UINT8 u4CheckIsNeedRecalc(UINT32 u4VdpId)
{
	Printf("vdp(%d) in(%d %d), out(%d %d)\n", u4VdpId,
								    _vdoIORes[u4VdpId].inRes.u4W,
								    _vdoIORes[u4VdpId].inRes.u4H,
								    _vdoIORes[u4VdpId].outRes.u4W,
								    _vdoIORes[u4VdpId].outRes.u4H);

	if(_vdoIORes[u4VdpId].inRes.u4W == 0 || 
	   _vdoIORes[u4VdpId].inRes.u4H == 0 || 
	   _vdoIORes[u4VdpId].outRes.u4W == 0 || 
	   _vdoIORes[u4VdpId].outRes.u4H == 0)
	{
		return 0;
	}

	return 1;
}

void vVdoUpdateInOutResolution(UINT32 u4VdpId)
{
    MODULE_RES_DESCRIPTION_T* preDSRes;
    MODULE_RES_DESCRIPTION_T* regenRes;
    MODULE_RES_DESCRIPTION_T* mibRes;
    MODULE_RES_DESCRIPTION_T* fscRes;
    VIDEO_RES_DESCRIPTION_T* vdoRes;
    
    vdoRes = &_vdoIORes[u4VdpId];
    preDSRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_PREDS];
    regenRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_REGEN];
    mibRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_MIB];
    fscRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_FSC];
    //update PREDS Resoluton
    preDSRes->inRes = vdoRes->inRes;   //input = video input
    preDSRes->outRes = preDSRes->inRes;   //output = input
    //if fix predown, use predown to determine output
    if(_fscInfo[u4VdpId].u4ForcedHPreDsFactor != 0)
    {
        preDSRes->outRes.u4W = (preDSRes->inRes.u4W * _fscInfo[u4VdpId].u4ForcedHPreDsFactor)/DFT_PDS_FACTOR;
    }
    else        //do predown if in < out
    {
        if(preDSRes->inRes.u4W > vdoRes->outRes.u4W)
        {
           preDSRes->outRes.u4W = vdoRes->outRes.u4W; 
        }
    }

    if(_fscInfo[u4VdpId].u4ForcedVPreDsFactor != 0)
    {
        preDSRes->outRes.u4H = (preDSRes->inRes.u4H * _fscInfo[u4VdpId].u4ForcedVPreDsFactor)/DFT_PDS_FACTOR;        
    }
    else
    {
        if(preDSRes->inRes.u4H > vdoRes->outRes.u4H)
        {
           preDSRes->outRes.u4H = vdoRes->outRes.u4H; 
        }
    }

    /* update Regen Resolution */
    regenRes->inRes = preDSRes->outRes;
    regenRes->outRes = vdoRes->ovsnRes;
    if(preDSRes->inRes.u4W > preDSRes->outRes.u4W)
    {
        regenRes->outRes.u4X = (regenRes->outRes.u4X * preDSRes->outRes.u4W) / preDSRes->inRes.u4W;
        regenRes->outRes.u4W = (regenRes->outRes.u4W * preDSRes->outRes.u4W) / preDSRes->inRes.u4W;
    }

    if(preDSRes->inRes.u4H > preDSRes->outRes.u4H)
    {
        regenRes->outRes.u4Y = (regenRes->outRes.u4Y * preDSRes->outRes.u4H) / preDSRes->inRes.u4H;
        regenRes->outRes.u4H = (regenRes->outRes.u4H * preDSRes->outRes.u4H) / preDSRes->inRes.u4H;
    }
    
    if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_SBS)
    {
        regenRes->outRes.u4W = (regenRes->outRes.u4W>>1)<<1;
    }
    else if(_fscInfo[u4VdpId].eRegenIn3DType == SCALER_TDTV_IN_TYPE_TAB)
    {
        regenRes->outRes.u4H = (regenRes->outRes.u4H>>1)<<1;
    }
    
    /* update MIB resolution */
    mibRes->inRes = regenRes->outRes;
    mibRes->inRes.u4X = 0;
    mibRes->inRes.u4Y = 0;
    mibRes->outRes = mibRes->inRes;

    /* update FSC resolution */
    fscRes->inRes = mibRes->outRes;
    fscRes->outRes = vdoRes->outRes;
}

void vOnScalerInfoChg(UINT32 u4VdpId)
{
	UINT8 res;
	MODULE_RES_DESCRIPTION_T* fscRes;
	MODULE_RES_DESCRIPTION_T* regenRes;
	MODULE_RES_DESCRIPTION_T* mibRes;
	fscRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_FSC];
	mibRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_MIB];
	regenRes = &_modIORes[u4VdpId][VIDEO_RES_MODULE_REGEN];
	
	res = u4CheckIsNeedRecalc(u4VdpId);
	if(!res)
	{
		Printf("===> NOT CHG \n");
		return;
	}

    //0. update in out resolution of all module
    vVdoUpdateInOutResolution(u4VdpId);

    //1. calculate Pre DS parameter
    vScpipCalcPreDSFactor(u4VdpId, &_modIORes[u4VdpId][VIDEO_RES_MODULE_PREDS]);

    _fscInfo[u4VdpId].u4NXNRWidth = mibRes->inRes.u4W;
    _fscInfo[u4VdpId].u4NXNRHeight = mibRes->inRes.u4H;
    
    _fscInfo[u4VdpId].u4InputWidth  = fscRes->inRes.u4W;
    _fscInfo[u4VdpId].u4InputHeight = fscRes->inRes.u4H;
    _fscInfo[u4VdpId].u4OutputX     = fscRes->outRes.u4X;
    _fscInfo[u4VdpId].u4OutputY     = fscRes->outRes.u4Y;
	_fscInfo[u4VdpId].u4OutputWidth = fscRes->outRes.u4W;
	_fscInfo[u4VdpId].u4OutputHeight= fscRes->outRes.u4H;

	//2. update regen parameter
	vScpipUpdateRegenPrm(u4VdpId, regenRes);
	
	//1. calculate scaling factor
	vScpipCalcScalingFactor(u4VdpId,
                            _fscInfo[u4VdpId].u4InputWidth,  
                            _fscInfo[u4VdpId].u4InputHeight, 
                            _fscInfo[u4VdpId].u4OutputWidth, 
                            _fscInfo[u4VdpId].u4OutputHeight);

	vUpdateColorFormat(u4VdpId, _fscInfo[u4VdpId].u4Forced444);
	
	//2. calculate scaling phase
	vScpipCalcScalingPhase(u4VdpId, 
                            _fscInfo[u4VdpId].u4InputWidth,  
                            _fscInfo[u4VdpId].u4InputHeight, 
                            _fscInfo[u4VdpId].u4OutputWidth, 
                            _fscInfo[u4VdpId].u4OutputHeight);
	if (_fscInfo[u4VdpId].u4NonLinear == SV_ON)
	{
		vScpipSetForcedNonLinearFactor(_fscInfo[u4VdpId].u4NonLinearInTotal,_fscInfo[u4VdpId].u4NonLinearInMiddle,_fscInfo[u4VdpId].u4NonLinearOutTotal,_fscInfo[u4VdpId].u4NonLinearOutMiddle);
	}

	//3. Update dram mode, dram widht height setting
	vUpdateDramHVwidth(u4VdpId, 
                            _fscInfo[u4VdpId].u4InputWidth,  
                            _fscInfo[u4VdpId].u4InputHeight, 
                            _fscInfo[u4VdpId].u4OutputWidth, 
                            _fscInfo[u4VdpId].u4OutputHeight);

	//4. Update R_POS setting
	vUpdateRptGenCount(u4VdpId, _fscInfo[u4VdpId].u4DramHeight);
	//5. set regsiter
	vScpipSetScalerReg(u4VdpId);

	return;
}

void vScpipSetPanelInfo(UINT32 u4PanelWidth, UINT32 u4PanelHeight)
{
	if(_arPanelInfo.u2PanelHActive != u4PanelWidth ||
		_arPanelInfo.u2PanelVActive != u4PanelHeight)
	{
		Printf("panel timing chg (%d,%d) --> (%d,%d)\n", _arPanelInfo.u2PanelHActive, 
												      _arPanelInfo.u2PanelVActive,
												      u4PanelWidth,
												      u4PanelHeight);
		_arPanelInfo.u2PanelHActive = u4PanelWidth;
		_arPanelInfo.u2PanelVActive = u4PanelHeight;
	}
}

void vScpipSetOverscanInfo(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4W, UINT32 u4H)
{
    UINT8 u1IsNeedRecalc = 0;

    if(u4X + u4W > _vdoIORes[u4VdpId].inRes.u4W)
    {
        Printf("Overscan is invalid. InputWidth=(%d) Overscaned X=(%d) Overscaned Width=(%d)\n",_vdoIORes[u4VdpId].inRes.u4W, u4X,u4W);
        return;
    }

    if(u4Y + u4H > _vdoIORes[u4VdpId].inRes.u4H)
    {
        Printf("Overscan is invalid. InputHeight=(%d) Overscaned Y=(%d) Overscaned Height=(%d)\n",_vdoIORes[u4VdpId].inRes.u4H, u4Y,u4H);
        return;
    }

    if(_vdoIORes[u4VdpId].ovsnRes.u4X != u4X)
    {
        _vdoIORes[u4VdpId].ovsnRes.u4X = u4X;
		u1IsNeedRecalc = 1;
    }

    if(_vdoIORes[u4VdpId].ovsnRes.u4Y != u4Y)
    {
        _vdoIORes[u4VdpId].ovsnRes.u4Y = u4Y;
		u1IsNeedRecalc = 1;
    }

    if(_vdoIORes[u4VdpId].ovsnRes.u4W != u4W)
    {
        _vdoIORes[u4VdpId].ovsnRes.u4W = u4W;
		u1IsNeedRecalc = 1;
    }

    if(_vdoIORes[u4VdpId].ovsnRes.u4H != u4H)
    {
        _vdoIORes[u4VdpId].ovsnRes.u4H = u4H;
		u1IsNeedRecalc = 1;
    }

	if(u1IsNeedRecalc)
	{
		vOnScalerInfoChg(u4VdpId);
	}

	return;
}

void vScpipSetInputInfo(UINT32 u4VdpId, UINT32 u4InputWidth, UINT32 u4InputHeight)
{
	UINT8 u1IsNeedRecalc = 0;

    //X, Y default set to 0
    _vdoIORes[u4VdpId].inRes.u4X = 0;
    _vdoIORes[u4VdpId].inRes.u4Y = 0;

    if(u4InputWidth != _vdoIORes[u4VdpId].inRes.u4W)
    {
        _vdoIORes[u4VdpId].inRes.u4W = u4InputWidth;
		u1IsNeedRecalc = 1;
    }

    if(u4InputHeight != _vdoIORes[u4VdpId].inRes.u4H)
	{
		_vdoIORes[u4VdpId].inRes.u4H = u4InputHeight;
		u1IsNeedRecalc = 1;
	}

	if(u1IsNeedRecalc)
	{
	    //reset overscan info same as input info
	    _vdoIORes[u4VdpId].ovsnRes = _vdoIORes[u4VdpId].inRes;
		vOnScalerInfoChg(u4VdpId);
	}

	return;
}

void vScpipSetOutputInfo(UINT32 u4VdpId, 
							UINT32 u4OutputX, 
							UINT32 u4OutputY,
							UINT32 u4OutputWidth, 
							UINT32 u4OutputHeight)
{
	UINT8 u1IsNeedRecalc = 0;

	if(u4OutputX >= _arPanelInfo.u2PanelHActive || u4OutputY >= _arPanelInfo.u2PanelVActive)
	{
		Printf("output (x: %d,y: %d) out of range of panel (%d,%d)\n", u4OutputX, 
															   u4OutputY, 
															   _arPanelInfo.u2PanelHActive,
															   _arPanelInfo.u2PanelVActive);
		return;
	}

	if(u4OutputX + u4OutputWidth > _arPanelInfo.u2PanelHActive)
	{
		u4OutputWidth = _arPanelInfo.u2PanelHActive - u4OutputX;
	}
	if(u4OutputY + u4OutputHeight > _arPanelInfo.u2PanelVActive)
	{
		u4OutputHeight =  _arPanelInfo.u2PanelVActive - u4OutputY;
	}

    if(u4OutputX != _vdoIORes[u4VdpId].outRes.u4X)
	{
		_vdoIORes[u4VdpId].outRes.u4X = u4OutputX;
		u1IsNeedRecalc = 1;
	}
	
	if(u4OutputY != _vdoIORes[u4VdpId].outRes.u4Y)
	{
		_vdoIORes[u4VdpId].outRes.u4Y = u4OutputY;
		u1IsNeedRecalc = 1;
	}

	if(u4OutputWidth != _vdoIORes[u4VdpId].outRes.u4W)
	{
		_vdoIORes[u4VdpId].outRes.u4W = u4OutputWidth;
		u1IsNeedRecalc = 1;
	}
	
	if(u4OutputHeight != _vdoIORes[u4VdpId].outRes.u4H)
	{
		_vdoIORes[u4VdpId].outRes.u4H = u4OutputHeight;
		u1IsNeedRecalc = 1;
	}

	if(u4OutputY != _fscInfo[u4VdpId].u4OutputY)
	{
		_fscInfo[u4VdpId].u4OutputY = u4OutputY;
		u1IsNeedRecalc = 1;
	}

	if(u4OutputWidth != _fscInfo[u4VdpId].u4OutputWidth)
	{
		_fscInfo[u4VdpId].u4OutputWidth = u4OutputWidth;
		u1IsNeedRecalc = 1;
	}
	
	if(u4OutputHeight != _fscInfo[u4VdpId].u4OutputHeight)
	{
		_fscInfo[u4VdpId].u4OutputHeight = u4OutputHeight;
		u1IsNeedRecalc = 1;
	}

	if(u1IsNeedRecalc)
	{
		vOnScalerInfoChg(u4VdpId);
	}

	return;
}

void vScpipSetDispMode(UINT32 u4VdpId,UINT32 u4Dispmode)
{
    if(_fscInfo[u4VdpId].u4Dispmode != u4Dispmode)
    {
       _fscInfo[u4VdpId].u4Dispmode = u4Dispmode;
       vOnScalerInfoChg(u4VdpId);
    }
}

#endif

