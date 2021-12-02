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
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "drv_predown.h"
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

LINT_EXT_HEADER_END

#define CC_SCPOS_FREERUN_MAX        150
static UINT32 _u4DispModeFreeRunCounter =0;

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];

//function for dispmode (move to drv_scaler_dispmode.c)
//static void _vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen);
static UINT8 _u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath);
static UINT8 _u1ScpipIsScalerDispmodeSupported(UINT8 bPath);
static void vScpipSetFixPscanVsync(UINT8 bPath, UINT8 bOnOff);

void vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen);
UINT32 u4ScpipGetPscanPrelen(UINT8 bPath);

void vScpipSetAutoTune(UINT8 bOnOff);
void vScpipSetAutoTuneStep(UINT8 step);
void vScpipSetFModeSel(UINT8 bOnOff);
void vScpipSetFSync(UINT8 bOnOff);
void vScpipSetFSyncProtecionMode(UINT8 bOnOff);
void vScpipSetLineSync(UINT8 bOnOff);
void vScpipSetPscanDispmodeHLen(UINT8 bPath, UINT32 u4HLen);
UINT32 u4ScpipGetPscanDispmodeHLen(UINT8 bPath);
void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode);
void vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bmode);
void vScpipSetLineSyncDispmode(UINT8 bPath,UINT8 bMode);


/*********************************************************
 ****************** ONLY FOR MT5396 **********************
 *********************************************************/
#if defined(CC_MT5396)
#define POCLK_SOURCE_CLK 337500000
#define POCLK_MINIMUM_PSCAN_DISPMODE_444 74000000
#define POCLK_MINIMUM_PSCAN_DISPMODE_422 68000000

EXTERN SCALER_OUTTIMING_PRM_T _arOutTimingPrm;
EXTERN UINT8 bFSCTimingChanged;

void vScpipConfigPOCLK(UINT8 mode);
void vScpipSetPOCLKDivider(UINT32 dividend, UINT32 divisor);
//void vScpipSetFSCPanelHTotal(UINT32 u4HTotal);
//void vScpipSetFSCPanelV(void);
void vScpipSetFSCTimeGen(UINT16 u2HTotal,UINT16 u2HActive,UINT16 u2VTotal,UINT16 u2VActive);
void vScpipConfigPOCLKSel(void);
static void _vScpipUpdateInternalFrameRate(UINT8 mode);
static void _vScpipUpdateFSCHTotal(UINT8 mode);
static void _vScpipUpdateFSCHActive(UINT8 mode);
static void _vScpipUpdateFSCVTotal(UINT8 mode);
static void _vScpipUpdateFSCVActive(UINT8 mode);
static void _vScpipUpdatePoclkDivider(UINT8 mode);
#endif

/**
 * @brief Scaler dispmode freerun check
 * @6896 wait review. need to use new flow. in coorporate with sync protection.
 */
void vScpipDispmodeFreeRunChk(void)
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
                vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_F_SYNC_1);
                vScpipSetAutoTune(SV_OFF);
            }

            if( eScpipGetFrameTrackStatus()==SCPIP_FRAME_TRACK_LOCK)
            {
                if(bStableCount <0xff)
                {
                    bStableCount++;
                }

                if(bStableCount==3)
                {
                    vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_F_SYNC_1);	
                }
                else if(bStableCount==5)
                {
                    vScpipSetAutoTune(SV_ON);
                }
                else if(bStableCount==15)
                {
                    vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_F_SYNC_1);
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
                vRegWriteFldAlign(SCPIP_PIP1_09, 1, PIP1_09_F_SYNC_1);
                vScpipSetAutoTune(SV_ON);
            }
            else if(_u4DispModeFreeRunCounter==1)
            {
                vRegWriteFldAlign(SCPIP_PIP1_09, 0, PIP1_09_F_SYNC_1);
            }

            if(_u4DispModeFreeRunCounter)
            {
                _u4DispModeFreeRunCounter --;
            }

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
                vScpipSetAutoTune(SV_ON);
            }
        }
        else if (_u4DispModeFreeRunCounter)
        {
            _u4DispModeFreeRunCounter--;
        }
    }
}

/**
 *  @6896 review done
 */
void vScpipSetDispModeFreeRunCount(UINT32 u4Count)
{
    _u4DispModeFreeRunCounter= u4Count;
}

/**
 *  @6896 new
 */
void vScpipSetAutoTune(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;

    vRegWriteFldAlign(SCPIP_PIP1_12, value, PIP1_12_DFIFO_AUTO_TUNE_1);    
}
/**
 *  @6896 new
 */
void vScpipSetAutoTuneStep(UINT8 step)
{
    vRegWriteFldAlign(SCPIP_PIP1_12, step, PIP1_12_DFIFO_TUNE_STEP_1);
}
/**
 *  @6896 new
 */
void vScpipSetFModeSel(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;
    
    vRegWriteFldAlign(SCPIP_PIP1_09, value, PIP1_09_FMODESEL_1);
}
/**
 *  @6896 new
 */
void vScpipSetFSync(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;
    
    vRegWriteFldAlign(SCPIP_PIP1_09, value, PIP1_09_F_SYNC_1);
}

void vScpipSetFSyncProtecionMode(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;
    
    vRegWriteFldAlign(SCPIP_PIP1_1D, value, PIP1_1D_FSYNC_PRO_MODE);
}

/**
 *  @6896 new
 */
void vScpipSetLineSync(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;
    
    vRegWriteFldAlign(SCPIP_PIP1_09, value, PIP1_09_LINE_SYNC_1);
}

/**
 * @brief Switch to dispmode
 * @param u4OnOff On/Off
 * @6896 wait review
 */
void vSwitchDispMode(UINT32 u4OnOff) 
{
    UINT8 bScalerMode;
    bScalerMode = u1ScpipGetDispMode(SV_VP_MAIN);

    if (u4OnOff == SV_ON)
    {
        /* Enable Display Mode */
        vScpipSetFModeSel(SV_ON);
    }
    else    //turn off
    {
        if (_arScalePrm[VDP_1].u4Enable)
        {
            CRIT_STATE_T csState = x_crit_start(); 
            // avoid other thread turn on write control before all related registers are set.          
            _arScalePrm[VDP_1].u4CntToRstDramPrm = (_arScalePrm[VDP_1].u4CntToRstDramPrm < 2)? 2 : _arScalePrm[VDP_1].u4CntToRstDramPrm;
            x_crit_end(csState);
        }

        /* Disable Display Mode */
        if (bScalerMode != VDP_SCPOS_DISPMODE_OFF)  // not dram mode.
        {
            //If current mode is other dispmode, to prevent FMODESEL_1 from keeping toggling between 0 and 1.
            //Just keep it as original value.
        }
        else
        {
            vScpipSetFModeSel(SV_OFF);        
        }

        // disable auto tune
        vScpipSetAutoTune(SV_OFF);
    }
}

/**
 * @brief Set scaler dispmode
 * @param bPath VDP_1/VDP_2
 * @param bMode mode type
 * @6896 wait review
 */
void vScpipSetScalerDispmode(UINT8 bPath,UINT8 bMode)
{
    RPicInfo* picInfo;
    picInfo = getPicInfo(bPath);
    
    //only main will enter dispmode
    if(bPath == SV_VP_PIP)
    {
        return;
    }
    
    //input clk > 100MHz
    if(picInfo->wHTotal * picInfo->wVTotal * bDrvVideoGetRefreshRate(SV_VP_MAIN) >100000000)
    {
        vScpipSetAutoTuneStep(0xF);
    }
    else
    {
        vScpipSetAutoTuneStep(0x10);
    }

    if(bMode == SV_ON)
    {
        #if CC_SPEED_UP_DISPMODE_TUNE
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DISP_TUNE, SV_VP_MAIN, 60, TRUE);
        #endif
        
        /* set DDDS and MOVLEN,MOHLEN,... */
        vDrvSetDynamicDisplay(SV_SUCCESS);
        /* turn ON disp mode */
        vSwitchDispMode(SV_ON);
         /* Enable FSYNC */
        vScpipSetFSync(SV_ON);
          /* Enable FSYNC protection*/
        vScpipSetFSyncProtecionMode(SV_ON);
        /* set disp mode tuning register */
        vScpipSetAutoTune(SV_OFF);
        /* Set initial tune postion */
        vRegWriteFldAlign(SCPIP_PIP1_12, 1, PIP1_12_USRCFG_POS_EN1); 
        vRegWriteFldAlign(SCPIP_PIP1_12, 10, PIP1_12_USRCFG_POS1);   
        /* set freerun counter */
        vScpipSetDispModeFreeRunCount(CC_SCPOS_FREERUN_MAX);
    }
    else if(bMode == SV_OFF)
    {
        vDrvSetDynamicDisplay(SV_FAIL);                
        /* turn OFF disp mode */
        vSwitchDispMode(SV_OFF);
        /* Disable FSYNC */
        vScpipSetFSync(SV_OFF);
        /* Disable FSYNC protection*/
        vScpipSetFSyncProtecionMode(SV_OFF);
        /* set freerun counter */        
        vScpipSetDispModeFreeRunCount(0);
        vRegWriteFldAlign(SCPIP_PIP1_12, 0, PIP1_12_USRCFG_POS_EN1); 
    }
}

void vScpipSetLineSyncDispmode(UINT8 bPath,UINT8 bMode)
{
    UINT8 bScalerMode;
    
    //only main support line sync dispmode
    if(bPath != SV_VP_MAIN)
    {
        return;
    }

    bScalerMode = getScalerMode(bPath);
    
    if (bMode == SV_ON)
    {
        /* Enable FMODESEL */
        vScpipSetFModeSel(SV_ON);
        /* Enable FSYNC */
        vScpipSetFSync(SV_ON);
        /* Enable LINE_SYNC */
        vScpipSetLineSync(SV_ON);
    }
    else    //turn off
    {
        /* Disable LINE_SYNC */
        vScpipSetLineSync(SV_OFF);
        /* Disable FSYNC */
        vScpipSetFSync(SV_OFF);
        /* Disable FMODESEL */
        if (bScalerMode != VDP_SCPOS_DISPMODE_OFF)  // not dram mode.
        {
            //If current mode is other dispmode, to prevent FMODESEL_1 from keeping toggling between 0 and 1.
            //Just keep it as original value.
        }
        else
        {
            vScpipSetFModeSel(SV_OFF);        
        }
    }        
}

/**
 * @brief Calculate output vtotal
 * @return Vtotal
 * @6896 review done
 */
UINT16 u2ScpipCalculateOutputVTotal(void) 
{
    UINT16 u2VOutputTotal;
    UINT32 u4InVTotal;
    RPicInfo* picInfo;

    picInfo = getPicInfo(SV_VP_MAIN);

    //get input VTotal
    u4InVTotal = wDrvVideoGetVTotal(SV_VP_MAIN);    
    u2VOutputTotal = u4InVTotal;

    if(picInfo->wTargetH != 0)
    {
        u2VOutputTotal = DIVIDE_WITH_CARRIER(u4InVTotal * wDISPLAY_HEIGHT , picInfo->wTargetH);
    }
    return u2VOutputTotal;
}

/**
 * @brief Check if Scaler dispmode can be supported or not
 * @param bPath VDP_1/VDP_2
 * @6896 wait review
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
 * @brief Set Pscan prelength
 * @param bPath VDP_1/VDP_2
 * @param u4PreLen
 * @6896 porting done
 */
void vScpipSetPscanPrelen(UINT8 bPath,UINT32 u4PreLen)
{
    UINT16 pipOffset;
    pipOffset = getPIPOffset(bPath);
    vRegWriteFldAlign(SCPIP_PIP1_0A+pipOffset, u4PreLen, PIP1_0A_VS_PSCAN_PRELEN_1);    
}

/**
 * @brief Get Pscan prelength
 * @param bPath VDP_1/VDP_2
 * @return u4PreLen
 * @6896 porting done
 */
 UINT32 u4ScpipGetPscanPrelen(UINT8 bPath)
{
    UINT16 pipOffset;
    pipOffset = getPIPOffset(bPath);
    return (RegReadFldAlign(SCPIP_PIP1_0A+pipOffset, PIP1_0A_VS_PSCAN_PRELEN_1));
}

static void vScpipSetFixPscanVsync(UINT8 bPath, UINT8 bOnOff)
{
    //only apply on 5396/5368 MP & 5389
    if( (BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) || 
        (BSP_GetIcVersion() >= IC_VER_5368_AB && BSP_GetIcVersion() <= IC_VER_5368_AC) ||
        (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC))
    {
        UINT16 pipOffset = getPIPOAOffset(bPath);

        if(bOnOff == SV_ON)
        {
            vRegWriteFldAlign(SCPIP_PIP1_0A+pipOffset, 1, PIP1_0A_FIX_PSCAN_VSYNC_1);
        }
        else
        {
            vRegWriteFldAlign(SCPIP_PIP1_0A+pipOffset, 0, PIP1_0A_FIX_PSCAN_VSYNC_1);    
        }    
    }
}

/**
 * @brief Configure Pscan dispmode
 * @param bPath VDP_1/VDP_2
 * @param bScalingMode down/up/bypass
 * @param bOnOff On/Off
 * @6896 porting done
 */
static void vScpipConfigPscanDispmode(UINT8 bPath,UINT8 bScalingMode,UINT8 bOnOff)
{
    UINT8 bmode;
    
    if (bPath >= SV_VP_NA)
    {
        return;
    }

    bmode = getScalerMode(bPath);
    //turn off pscan dispmode
    if(bOnOff == SV_OFF)
    {
        LOG(3, "VDP(%d) Turn off PScan dispmode", (UINT32)bPath);

        vScpipSetFixPscanVsync(bPath, SV_OFF);

        if(bmode != VDP_SCPOS_DISPMODE_OFF)
        {
            //If current mode is other dispmode, to prevent FMODESEL_1 from keep toggling between 0 and 1.
            //Just keep it as original value.
        }
        else
        {
            vScpipSetFModeSel(SV_OFF);
        }
    }
    else if(bOnOff == SV_ON)
    {        
        UINT32 u4VBlank;

        vScpipSetFixPscanVsync(bPath, SV_ON);

        //PSCAN vsync prefetch
        u4VBlank = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL) - TV_HEIGHT;
        //ASSERT((u4VBlank-5) >= 6); // if PRELEN is set to < 6 it must be failed
        //_vScpipSetPscanPrelen(bPath,MIN((u4VBlank-5), 0x14));
        if (MODE_525I == bDrvVideoGetTiming(bPath) || MODE_625I == bDrvVideoGetTiming(bPath)) // interlace: 480i, 576i
        {
            vScpipSetPscanPrelen(bPath, MAX((u4VBlank-7), 0x14));
        }
        else // others
        {
            vScpipSetPscanPrelen(bPath, u2GetMainPrelenValue(bPath)); //FIXME
        }

        //Horizontal pixel per line = pre-scale down size + pscan HBLANK       
        // For pscan display mode hlen update
        vScpipSetPscanDispmodeHLen(bPath, u4ScpipGetPscanDispmodeHLen(bPath));
        
        //Write data to line buffer
        vScpipSetFModeSel(SV_ON);
    }
}

/**
 * @brief get pscan display mode type
 * @param bPath VDP_1/VDP_2
 * @return bMode DOWN_SCALING_MODE/UP_SCALING_MODE
 * @6896 porting done
 */
static UINT8 _u1ScpipGetPSCANDispmodeScalingMode(UINT8 bPath)
{
    RDispPrm* dispPrm;
    UINT8 bMode;

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
        bMode = DOWN_SCALING_MODE;
    }

    return bMode;    
}

/**
 * @brief set pscan display mode on/off
 * @param bPath VDP_1/VDP_2
 * @param bMode what mode to set
 * @6896 porting done
 */
void vScpipSetPSCANDispmode(UINT8 bPath,UINT8 bOnOff)
{
    UINT8 bScalingMode;

    bScalingMode = _u1ScpipGetPSCANDispmodeScalingMode(bPath);

    vScpipConfigPscanDispmode(bPath, bScalingMode, bOnOff);    
}

void vScpipSetDramMode(UINT8 bPath,UINT8 bOnOff)
{
    UINT16 pipOffset;
    UINT16 pipOAOffset;
//    UINT16 importOffset;

    pipOffset = getPIPOffset(bPath);
    pipOAOffset = getPIPOAOffset(bPath);
 //   importOffset = getImportOffset(bPath);

	if(bOnOff == SV_ON)
	{
	    if(bPath == SV_VP_MAIN)
	    {
	    	vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 4, PIP1_0C_PRE_VOFST_DRAM_1);
	    }
		else if(bPath == SV_VP_PIP)
		{
	    	vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 1, PIP1_0C_PRE_VOFST_DRAM_1);
    	}

        //scaler vsync setting for dram mode
        //vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNC_OPT_1);
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WPOS_REF_SYNC_1);
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WPOS_REF_MODE_1);
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WPOS_REF_NUM_1);
	}	
	else
	{
		vRegWriteFldAlign(SCPIP_PIP1_0C+pipOffset, 0, PIP1_0C_PRE_VOFST_DRAM_1);
		
        //scaler vsync setting for non-dram mode
        //vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);		
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WPOS_REF_SYNC_1);
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WPOS_REF_MODE_1);
		vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WPOS_REF_NUM_1);        
	}
    
    if(bOnOff == SV_OFF)
    {
        //R/W enable will turned on in scaler loop, so we don't have to do this here.
        //turn off R/W enable.
        vScpipWriteCtrlOnOff(bPath, SV_OFF);
    }
    #if 0//remove this code to _VDP_DispModeEnable to avoid srm call back before scpip hw init
    //set import method (Dram mode use method 2)
    if(bOnOff == SV_ON)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 2, SYSTEM_10_IMPORT_SEL_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_SEL_1);
    }
    #endif 
}

/**
 * @brief Set scaler mode (DRAM/scaler dispmode/PSCAN dispmode)
 * @param bPath VDP_1/VDP_2
 */
void vScpipModeSwitch(UINT8 bPath)
{
   // UINT16 dsOffset;
    UINT8 mode;
    
    if (bPath >= SV_VP_NA)
    {
        return;
    }

   // dsOffset = getDSOffset(bPath);
    mode = getScalerMode(bPath);
    
    //PIP path don't support Scaler dispmode
    if (bPath == SV_VP_PIP && mode != VDP_SCPOS_DISPMODE_OFF)
    {
        return;
    }

    //config dispmode line buffer
    vScpipDispmodeLineCfg(bPath, mode);
    #if 0//remove this code to _VDP_DispModeEnable to avoid srm call back before scpip hw init
    //5396/68 dram agent need short vsync from down scaler. Set this reg to 1 will do this. 
    if (mode == VDP_SCPOS_DISPMODE_OFF || mode == VDP_SCPOS_PSCAN_DISPMODE) //FIXME
    {
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNC_OPT_1);        
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);
    }
    #endif
    //set import w pos update timing and set PDN_USCALER in dram mode
    if (mode == VDP_SCPOS_DISPMODE_OFF) //dram mode
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_W_VSYNC_SEL1); //FIXME
        vRegWriteFldAlign(SCPIP_SYSTEM_20, 1, SYSTEM_20_PDN_USCALER);
    }
    else //other mode
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10, 0, SYSTEM_10_IMPORT_W_VSYNC_SEL1);
        vRegWriteFldAlign(SCPIP_SYSTEM_20, 0, SYSTEM_20_PDN_USCALER);
    }

    if (mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL)   //scaler dispmode
    {
        /* have to turn off PSCAN DISPMODE first
         * To test if scaler dispmode was supported or not, DS1_INVLEN_1 value is needed.
         * When scaler is in up-scaling pscan dispmode, DS1_INVLEN_1 won't be correct.
         * So before testing scaler dispmode, we have to turn off pscan dispmode first.
         */
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipSetDramMode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_OFF);        

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
    else if (mode == VDP_SCPOS_PSCAN_DISPMODE)      //pscan dispmode
    {
        vScpipSetScalerDispmode(bPath,SV_OFF);
        vScpipSetDramMode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_ON);
    }
    else if (mode == VDP_SCPOS_DISPMODE_OFF)        //dram mode
    {    
        vScpipSetScalerDispmode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_OFF);
        vScpipSetDramMode(bPath,SV_ON);
    }
    else if(mode== VDP_SCPOS_LINESYNC_DISPMODE)     //line sync dispmode
    {
        vScpipSetScalerDispmode(bPath,SV_OFF);
        vScpipSetDramMode(bPath,SV_OFF);
        vScpipSetPSCANDispmode(bPath,SV_OFF);
        vScpipSetLineSyncDispmode(bPath,SV_ON);
    }

    //after mode switch done, turn on import protection
    vScpipOnVdoModeChangeDone(bPath);    

#if SUPPORT_HDTV_HARDWARE_MUTE
    vScpipConfigHardwareMute();
#endif
    return;
}

#if defined(CC_MT5396)
void vScpipConfigPOCLK(UINT8 mode)
{
    if(mode == VDP_SCPOS_DISPMODE_AUTO || mode == VDP_SCPOS_DISPMODE_MANUAL)
    {
        return;
    }
    
    _vScpipUpdateInternalFrameRate(mode);
    _vScpipUpdateFSCHTotal(mode);
    _vScpipUpdateFSCHActive(mode);
    _vScpipUpdateFSCVTotal(mode);
    _vScpipUpdateFSCVActive(mode);

    if(_arOutTimingPrm.u2PanelVTotal <= _arOutTimingPrm.u2PanelVActive)
    {
        _arOutTimingPrm.u2PanelVTotal = _arOutTimingPrm.u2PanelVActive + 20;
    }    
    _vScpipUpdatePoclkDivider(mode);
  
    //update poclk divider
    vScpipSetPOCLKDivider(_arOutTimingPrm.u4DPllDividend,
                          _arOutTimingPrm.u4DPLLDivisor);
    
    //update internal frame rate between Front scaler and MJC
    vDrvSetInternalFrameRate(_arOutTimingPrm.u2FrameRate);
    //update Front scaler HTotal
    vScpipSetFSCTimeGen(_arOutTimingPrm.u2PanelHTotal,
                        _arOutTimingPrm.u2PanelHActive,
                        _arOutTimingPrm.u2PanelVTotal,
                        _arOutTimingPrm.u2PanelVActive);
    //if MJC do 3D & in line sync dispmode
    vScpipConfigPOCLKSel();
}

static void _vScpipUpdateInternalFrameRate(UINT8 mode)
{
    UINT16 u2FrameRate;
    
    u2FrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    LOG(1,"Input Frame Rate (%d)\n", u2FrameRate);
    
    if(mode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        //Line sync dispmode. Input frame rate = Output frame rate.
    }
    else if(mode == VDP_SCPOS_PSCAN_DISPMODE || mode == VDP_SCPOS_DISPMODE_OFF)
    {        
        if(vScpipIs3DDoneByMJC() == SV_TRUE &&
            eScpipGetTdtvInType(SV_VP_MAIN) == SCALER_TDTV_IN_TYPE_FS )
        {
            //keep the same frame rate
        }
        else if(u2FrameRate > 60)
        {
            //when input frame rate > 60 (ex: PC timing), output 60 as internal frame rate        
            u2FrameRate = 60;            
        }
    }
    
    LOG(1,"Set Internal Frame Rate (%d)\n", u2FrameRate);
    _arOutTimingPrm.u2FrameRate = u2FrameRate;
}

static void _vScpipUpdateFSCHTotal(UINT8 mode)
{
    UINT16 hTotal;
    if(mode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        hTotal = wDISPLAY_WIDTH + 150;
    }
    else if(mode == VDP_SCPOS_PSCAN_DISPMODE || mode == VDP_SCPOS_DISPMODE_OFF)
    {
        hTotal = wDrvGetOutputHTotal();
    }
    else
    {
        hTotal = 0; // FIXME
        LOG(1,"Dispmode (%d)\n", mode);        
    }
    
    LOG(1,"Set Front Scaler HTotal (%d)\n", hTotal);
    _arOutTimingPrm.u2PanelHTotal = hTotal;
}

static void _vScpipUpdateFSCHActive(UINT8 mode)
{
    UINT16 hActive = wDISPLAY_WIDTH;
    
    LOG(1,"Set Front Scaler HActive (%d)\n", hActive);    
    _arOutTimingPrm.u2PanelHActive = hActive;
}

static void _vScpipUpdateFSCVTotal(UINT8 mode)
{
    UINT16 u2PanelVTotal;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;

    dispInfo = getDispInfo(SV_VP_MAIN);
    picInfo = getPicInfo(SV_VP_MAIN);
    
    //error handling
    if(picInfo->wTargetH == 0 || picInfo->wSrcH ==0 || dispInfo->wHeight ==0)
    {
        return;
    }

    //In line sync dispmode & MJC do 3D, panel v active need to set to front scaler's output v active.
    if(mode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        u2PanelVTotal = ((wDrvGetOutputVTotal() * (picInfo->wSrcH - picInfo->wYOffset)) / picInfo->wTargetH) + 4; 
    }
    else
    {
        if(vScpipIs3DDoneByMJC() == SV_TRUE)
        {
            u2PanelVTotal = (wDISPLAY_HEIGHT == 0)? wDrvGetOutputVTotal() : ( (wDrvGetOutputVTotal() * dispInfo->wHeight) / wDISPLAY_HEIGHT);
        }
        else
        {
            u2PanelVTotal = wDrvGetOutputVTotal();
        }
    }

    //error handling
    if(u2PanelVTotal ==0)
    {
        return;
    }

    //change v total according TDTV's prm
    if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_HALF)
    {
        u2PanelVTotal = u2PanelVTotal<<1;
    }
    else if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_DOUBLE)
    {
        u2PanelVTotal = u2PanelVTotal>>1;
    }

    LOG(1,"Set Front Scaler VTotal (%d)\n", u2PanelVTotal);

    _arOutTimingPrm.u2PanelVTotal = u2PanelVTotal;
}

static void _vScpipUpdateFSCVActive(UINT8 mode)
{
    UINT16 u2PanelVActive;
    RDispInfo* dispInfo;
    RPicInfo* picInfo;

    dispInfo = getDispInfo(SV_VP_MAIN);
    picInfo = getPicInfo(SV_VP_MAIN);

    //error handling
    if(picInfo->wTargetH == 0 || picInfo->wSrcH ==0 || dispInfo->wHeight ==0)
    {
        return;
    }

    //In line sync dispmode & MJC do 3D, panel v active need to set to front scaler's output v active.
    if(getScalerMode(SV_VP_MAIN) == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        if(vScpipIs3DDoneByMJC() == SV_TRUE)
        {
            //overscan done by MJC
            u2PanelVActive = dispInfo->wHeight + 4;
        }
        else
        {
            u2PanelVActive = ((dispInfo->wHeight * (picInfo->wSrcH - picInfo->wYOffset)) / picInfo->wTargetH) + 4; 
        }
    }
    else
    {
        if(vScpipIs3DDoneByMJC() == SV_TRUE)
        {
            u2PanelVActive = dispInfo->wHeight+4;
        }
        else
        {
            u2PanelVActive = wDrvGetOutputVActive();
        }        
    }

    //error handling
    if(u2PanelVActive == 0)
    {
        return;
    }

    LOG(1,"Set Front Scaler VActive (%d)\n", u2PanelVActive);    

    _arOutTimingPrm.u2PanelVActive= u2PanelVActive;    
}

static void _vScpipUpdatePoclkDivider(UINT8 mode)
{
    UINT64 dividend;
    UINT64 divisor;
    UINT64 u4HTotal;
    UINT64 u4VTotal;
    UINT64 u4FrameRate;
    
    divisor = 0x8000;
    u4HTotal = _arOutTimingPrm.u2PanelHTotal;
    u4VTotal = _arOutTimingPrm.u2PanelVTotal;
    u4FrameRate = _arOutTimingPrm.u2FrameRate;
    
    if(mode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        if(u4FrameRate == 0)
        {
            dividend = 0x4000;
        }
        else
        {
            dividend = u8Div6464(0x4000 * u4FrameRate, 60, NULL);
        }

    }
    else if(mode == VDP_SCPOS_PSCAN_DISPMODE || mode == VDP_SCPOS_DISPMODE_OFF)
    {               
        if(u4HTotal == 0 || u4VTotal ==0 || u4FrameRate ==0)
        {
            dividend = 0x4000;
        }
        else
        {
            //special case. will change htotal
            if(mode == VDP_SCPOS_PSCAN_DISPMODE)
            {
                UINT64 minimumClk = (_arScalePrm[VDP_1].u4Is444==SV_TRUE)?POCLK_MINIMUM_PSCAN_DISPMODE_444:POCLK_MINIMUM_PSCAN_DISPMODE_422;
                
                if(u4HTotal * u4VTotal * u4FrameRate < minimumClk)
                {
                    //enlarge H Total
                    u4HTotal = u8Div6464(minimumClk, u4VTotal * u4FrameRate , NULL);
                    //update HTotal
                    _arOutTimingPrm.u2PanelHTotal = (UINT16)u4HTotal;
                }
            }
            
            dividend = u8Div6464((u4HTotal * u4VTotal * u4FrameRate * 0x8000), 337500000, NULL);
        }        
    }
    else // other dispmode...
    {
        return; // FIXME
    }
    
    LOG(1,"Set POCLK dividend (0x%x)\n", dividend);
    LOG(1,"Set POCLK divisor (0x%x)\n", divisor);
    _arOutTimingPrm.u4DPllDividend = dividend;
    _arOutTimingPrm.u4DPLLDivisor = divisor;
}

void vScpipSetPOCLKDivider(UINT32 dividend, UINT32 divisor)
{
    //1. set divider m/ n 
    vIO32WriteFldAlign(CKGEN_POCLK_DPLL_DIVCFG, dividend, FLD_POCLK_DPLL_IDEAL_PLL_M);
    vIO32WriteFldAlign(CKGEN_POCLK_DPLL_DIVCFG, divisor, FLD_POCLK_DPLL_IDEAL_PLL_N);
}

void vScpipSetFSCTimeGen(UINT16 u2HTotal,UINT16 u2HActive, UINT16 u2VTotal, UINT16 u2VActive)
{    
    //update FSC output time gen.
    if(RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL) != u2HTotal)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_00, u2HTotal, PIP_OA_00_PANEL_HTOTAL);
        LOG(1,"Update FSC PANEL HTOTAL from (%d) to (%d)\n", RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL), u2HTotal);
        bFSCTimingChanged = SV_TRUE;
    }

    if(RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HACTIVE) != u2HActive)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_00, u2HActive, PIP_OA_00_PANEL_HACTIVE);    
        LOG(1,"Update FSC PANEL HACTIVE from (%d) to (%d)\n", RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HACTIVE), u2HActive);
        bFSCTimingChanged = SV_TRUE;
    }

    if(RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL) != u2VTotal)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u2VTotal, PIP_OA_01_PANEL_VTOTAL);    
        LOG(1,"Update FSC PANEL VTOTAL from (%d) to (%d)\n",RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL), u2VTotal);
        bFSCTimingChanged = SV_TRUE;
    }

    if(RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VACTIVE) != u2VActive)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u2VActive, PIP_OA_01_PANEL_VACTIVE);    
        LOG(1,"Update FSC PANEL VACTIVE from (%d) to (%d)\n",RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VACTIVE), u2VActive);
        bFSCTimingChanged = SV_TRUE;
    }
}

void vScpipSetFSCImportToggle(void)
{
    if(bFSCTimingChanged == SV_TRUE)
    {        
        vScpipSetFSCImportOff(SV_ON,SV_VP_MAIN);        
        LOG(1,"Set FSC import_off to 1\n");
        vScpipSetFSCImportOff(SV_OFF,SV_VP_MAIN);
        LOG(1,"Set FSC import_off to 0\n");
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SET_DATA_SEL);
        LOG(1,"Set SET_DATA_SEL to 1\n");
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SET_DATA_SEL);
        LOG(1,"Set SET_DATA_SEL to 0\n");        
#if 0
        #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscSetImportOff(SV_ON);
        LOG(1,"Set PSC import_off to 1\n");
        vPscSetImportOff(SV_OFF);
        LOG(1,"Set PSC import_off to 0\n");        
        #endif
#endif
        bFSCTimingChanged = SV_FALSE;
        
    }
}

void vScpipSetPSCImportToggle(void)
{
    if(bFSCTimingChanged == SV_TRUE)
    {        
        #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscSetImportOff(SV_ON);
        LOG(1,"Set PSC import_off to 1\n");
        vPscSetImportOff(SV_OFF);
        LOG(1,"Set PSC import_off to 0\n");        
        #endif
        bFSCTimingChanged = SV_FALSE;
    }
}

#if 0
void vScpipSetFSCPanelV(void)
{
    UINT16 u2PanelVActive;
    UINT16 u2PanelVTotal;
    RDispInfo* dispInfo = getDispInfo(SV_VP_MAIN);
    RPicInfo* picInfo = getPicInfo(SV_VP_MAIN);

    //error handling
    if(picInfo->wTargetH == 0 || picInfo->wSrcH ==0 || dispInfo->wHeight ==0)
    {
        return;
    }

    //In line sync dispmode & MJC do 3D, panel v active need to set to front scaler's output v active.
    if(getScalerMode(SV_VP_MAIN) == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        /* +4 to delay DISP_R sample time.   
         * Input disp_r will transient at v falling. 
         * When doing vertical down scaling or overscan, diff phase will make last line to output before or after disp_r transient.
         * This will cause disp_r sampling error.
         * To avoid this, add the value of panel vactive by 4 to delay disp_r sample time.
         */        
        u2PanelVActive = ((dispInfo->wHeight * (picInfo->wSrcH - picInfo->wYOffset)) / picInfo->wTargetH) + 4; 
        u2PanelVTotal = ((wDrvGetOutputVTotal() * (picInfo->wSrcH - picInfo->wYOffset)) / picInfo->wTargetH) + 4; 
    }
    else
    {
        u2PanelVActive = wDrvGetOutputVActive();
        u2PanelVTotal = wDrvGetOutputVTotal();
    }

    //error handling
    if(u2PanelVTotal ==0 || u2PanelVActive == 0)
    {
        return;
    }

    if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_HALF)
    {
        u2PanelVTotal = u2PanelVTotal<<1;
    }
    else if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_DOUBLE)
    {
        u2PanelVTotal = u2PanelVTotal>>1;
    }

    //to avoid panel vtotal < vactive
    if(u2PanelVTotal < u2PanelVActive)
    {
        u2PanelVTotal = u2PanelVActive + 20;
    }
    LOG(0,"Set Front Scaler VTotal (%d)\n", u2PanelVTotal);
    LOG(0,"Set Front Scaler VActive (%d)\n", u2PanelVActive);
    
    vRegWriteFldAlign(SCPIP_PIP_OA_01, u2PanelVTotal, PIP_OA_01_PANEL_VTOTAL);    
    vRegWriteFldAlign(SCPIP_PIP_OA_01, u2PanelVActive, PIP_OA_01_PANEL_VACTIVE);    
}
#endif

void vScpipConfigPOCLKSel(void)
{
    //In line sync dispmode & MJC do 3D, panel v active need to set to front scaler's output v active.
    if(getScalerMode(SV_VP_MAIN) == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        vScpipSetPOCLKSelReg(FLD_POCLK_SEL_MIB_OCLK);
    }
    else
    {
        //set dpll source from ethernet pll /2
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x1, FLD_POCLK_DPLL_SEL);
        vScpipSetPOCLKSelReg(FLD_POCLK_SEL_DPLL);
    }
}

#endif

/**
 *  @6896 porting done
 */
void vScpipDispmodeLineCfg(UINT8 bPath, UINT8 bMode)
{
    UINT8 bCfgLine = 0;
    UINT8 bCLine = 0;
    UINT8 bLineNum;
    RDispPrm* dispPrm;
    dispPrm = getDispPrm(bPath);

    ASSERT(bPath == SV_VP_MAIN); // only main path support display mode

    if(bPath == SV_VP_MAIN)
    {
        if(bMode==VDP_SCPOS_DISPMODE_OFF)
        {
            bCLine = 1;                         //use 2 line buffer
            bCfgLine = 1;                       //config to 2 lines
        }
        else if(bMode==VDP_SCPOS_PSCAN_DISPMODE)//pscan  display mode
        {   
            if(_u1ScpipGetPSCANDispmodeScalingMode(bPath) == UP_SCALING_MODE)   
            {                                   //pscan + up
                bCLine = 1;                     //use 2 line buffer
                bCfgLine = 1;                   //config to 2 lines
            }
            else                                //pscan + down
            {
                bCLine = 2;                     //use 3 line buffer
                bCfgLine = 2;                   //config to 3 lines
            }
        }
        else if(bMode == VDP_SCPOS_DISPMODE_AUTO || bMode == VDP_SCPOS_DISPMODE_MANUAL)
        {
            bCLine = 2;                         //use 3 line buffer  
            bLineNum = 1440/dispPrm->wHLen;
            if(bLineNum < 6)
            {
                bCfgLine = 3;
            }
            else 
            {
                bCfgLine = (bLineNum-2) > 7 ? 7 :(bLineNum-2);
            }                    
        }
        else if(bMode == VDP_SCPOS_LINESYNC_DISPMODE)
        {
            bCLine = 1;                         //use 2 line buffer
            bCfgLine = 1;                       //config to 2 lines        
        }
        
        vRegWriteFldAlign(SCPIP_PIP1_11, bCLine, PIP1_11_C_LINE_1);             
        vRegWriteFldAlign(SCPIP_PIP1_11, bCfgLine, PIP1_11_CFG_LINE_1); 
    }
}

// For pscan display mode hlen update
/**
 *  @6896 porting done
 */
void vScpipSetPscanDispmodeHLen(UINT8 bPath, UINT32 u4HLen)
{
    UINT16 offset;
    offset = getPIPOffset(bPath);
    vRegWriteFldAlign(SCPIP_PIP1_0A + offset, u4HLen, PIP1_0A_HLEN_PSCAN_1);
}

/**
 * @brief Get H clip width
 * @param bPath VDP_1/VDP_2
 * @return clip width
 */
static UINT16 _u2ScpipGetHClipWidth(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return 0;
    }
#if !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP || CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP

    UINT32 u4Factor;
    RPicInfo* picInfo;
    UINT16 HClipWidth;

    //if not in pscan dispmode, just return pre-scale downed width
    if(u1ScpipGetDispMode(bPath)!= VDP_SCPOS_PSCAN_DISPMODE)
    {
        return u4ScpipGetPreScaleWidth(bPath);
    }

    //get input info
    picInfo = getPicInfo(bPath);
    //get pre-scale down factor
    u4Factor = u4ScpipGetHwHPS(bPath);
    
    #if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
    if(((UINT32)getMixedHPorch(bPath) * u4Factor / DFT_HPD_FACTOR) < SCPIP_HCLIP_LENGTH)
    {
        HClipWidth = u4ScpipGetPreScaleWidth(bPath);
    }
    else
    {
        HClipWidth = (UINT32)picInfo->wTargetW * u4Factor / DFT_HPD_FACTOR + 2*SCPIP_HCLIP_LENGTH;
    }
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
 * @brief Get Pscan dispmode horizontal length (HLEN_PSCAN_1)
 * @param bPath VDP_1/VDP_2
 * @param horizontal length
 * @6896 wait review 
 */
UINT32 u4ScpipGetPscanDispmodeHLen(UINT8 bPath)
{
    UINT32 ret, u4HMin;
    UINT32 u4HClipWidth;
    
    u4HClipWidth = _u2ScpipGetHClipWidth(bPath);
    
    u4HMin=((u4HClipWidth+15)/16)*16 + u4DrvDIGetHBlank(bPath);
    ret = u4HMin;
    
    if(_u1ScpipGetPSCANDispmodeScalingMode(bPath)==UP_SCALING_MODE)
    {
        UINT32 u4ClkMhz = (PANEL_GetPixelClk50Hz())/1000000;

        if(u4ClkMhz <80)
        {
            RDispPrm* dispPrm;
            dispPrm = getDispPrm(bPath);

            // PScan_HLEN =  HL *2.5T * input clk/ output clk;  for 444 8 bit pack
            // PScan_HLEN =  HL *2.0T * input clk/ output clk;  for 422 10 bit pack
            #if CC_PSCAN_DISPMODE_DI_SCPIP_DO_HCLIP
            ret = (u4HClipWidth/5)*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;			
            #elif !CC_PSCAN_DISPMODE_SCPIP_DO_HCLIP    
            ret = dispPrm->wHLen*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;            
            #else
            ret = (u4HClipWidth/5)*((14991360)/(u4ClkMhz))/dispPrm->u4USclV;            
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

/**
 *  @brief set water control parameter for pscan dispmode
 *  @6896 new
 */
void vScpipInitDsWaterCtrl(void)
{
    //set all speed to 0x80
    vRegWriteFldAlign(SCPIP_PIP1_14, 0x80, PIP1_14_WTOKEN_VB_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_14, 0x80, PIP1_14_WTOKEN_INI_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_13, 0x80, PIP1_13_WTOKEN0_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_13, 0x80, PIP1_13_WTOKEN1_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_13, 0x80, PIP1_13_WTOKEN2_DS_1);
    //set init thr to 1090
    vRegWriteFldAlign(SCPIP_PIP1_15, 1090, PIP1_15_WTHR_INI_DS_1);
    //set other thr to 1024
    vRegWriteFldAlign(SCPIP_PIP1_16, 1024, PIP1_16_WTHR1_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_16, 1024, PIP1_16_WTHR2_DS_1);
    vRegWriteFldAlign(SCPIP_PIP1_15, 1024, PIP1_15_WTHR3_DS_1);    
}

void vScpipInitUsWaterCtrl(void)
{
    //set all speed to 0x80
    vRegWriteFldAlign(SCPIP_PIP1_18, 0x80, PIP1_18_WTOKEN_VB_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_18, 0x80, PIP1_18_WTOKEN_INI_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_17, 0x80, PIP1_17_WTOKEN0_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_17, 0x80, PIP1_17_WTOKEN1_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_17, 0x80, PIP1_17_WTOKEN2_US_1);
    //set init thr to 546
    vRegWriteFldAlign(SCPIP_PIP1_19, 546, PIP1_19_WTHR_INI_US_1);
    //set other thr to 512
    vRegWriteFldAlign(SCPIP_PIP1_1A, 540, PIP1_1A_WTHR1_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_1A, 540, PIP1_1A_WTHR2_US_1);
    vRegWriteFldAlign(SCPIP_PIP1_19, 540, PIP1_19_WTHR3_US_1);
}

/**
 * @brief Enable/Disable scaler dispmode
 * @param ucVdpId VDP_1/VDP_2
 * @param ucOnOff On/Off
 * @6896 review done. change name??
 */
void _VDP_DispModeEnable(UCHAR ucVdpId, UCHAR ucMode)
{
    UINT8 bPath;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(ucVdpId);

    #if !defined(SCPIP_SUPPORT_POST_SCALER)
        if(ucMode == VDP_SCPOS_LINESYNC_DISPMODE)
        {
            LOG(0,"Line sync dispmode is not supported\n");
            ASSERT(0);
        }
    #endif

    LOG(4,"Set scaler mode by SRM. VDP(%d) mode(%d)\n",ucVdpId,ucMode);
    vScpipUpdateImportSetting(bPath,ucMode);

    if (u1ScpipGetDispMode(bPath) != ucMode) 
    {
        LOG(4,"Scaler mode changed. VDP(%d) mode(%d->%d)\n",ucVdpId,u1ScpipGetDispMode(bPath),ucMode);
        vScpipSetDispModeVar(bPath, (UINT8)ucMode);
        //Scaler disp mode changed, turn off import protection.
        #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)
        LOG(0,"Scaler mode changed. Turn off import protection\n");
        vScpipTurnOffImportProtection(bPath);      
        #endif

        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
            vSetScposFlg(SCALER_MAIN_MODE_CHG);
        }
        else
        {
            if(ucMode != VDP_SCPOS_DISPMODE_OFF)
            {
                //Hardware Limitation, sub path can't get into scaler dispmode
                ASSERT(0);
            }            
        }
    }
}

/**
 * @brief Set dispmode status
 * @param bPath VDP_1/VDP_2
 * @param bPath VDP_1/VDP_2
 */
void vScpipSetDispModeVar(UINT8 bPath, UINT8 u1Dispmode)
{
    if(bPath >=VDP_2 && u1Dispmode != VDP_SCPOS_DISPMODE_OFF)
    {
        Printf("Set VDP(%d) to Non-dram mode(%d)\n",getScalerVDP(bPath),u1Dispmode);
        ASSERT(0);
    }
    _arScalePrm[bPath].u4DispMode = (UINT32) u1Dispmode;
}

/**
 * @brief Get dispmode status
 * @param bPath VDP_1/VDP_2
 * @return Dispmode type
 * @6896 porting done
 */
UINT8 u1ScpipGetDispMode(UINT8 bPath)
{
    ASSERT(bPath <= SV_VP_PIP);
    return _arScalePrm[bPath].u4DispMode;
}

