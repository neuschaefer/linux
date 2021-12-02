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
 * $Author: youneng.xu $
 * $Date: 2013/01/17 $
 * $RCSfile: drv_display.c,v $
 * $Revision: #16 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_display.c
 *  Brief of file drv_display.c.
 *  Details of file drv_display.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "drv_vdoclk.h"
#include "util.h"
#include "hw_scpos.h"
#include "hw_mlvds.h"
#include "hw_tcon.h"
#include "hw_tg.h"
#include "drv_scpos.h"
#include "drv_display.h"
#include "drv_scaler.h"
#include "drv_ycproc.h"
#include "drv_lvds.h"
#include "drv_di.h"
#include "vdo_misc.h"
#include "drv_hdtv.h"
#include "video_def.h"
#include "drv_video.h"
#include "drv_tcon.h"
#include "drv_lcdim.h"
#include "nptv_debug.h"
#include "api_backlt.h"
#include "mute_if.h"
#include "srm_drvif.h"
#include "drv_ldmspi.h"
#ifndef CC_COPLAT_MT82
#include "osd_drvif.h"
#include "pmx_drvif.h"
#endif
#include "x_assert.h"
#include "x_lint.h"
#include "x_pinmux.h"
#include "c_model.h"
#include "x_timer.h"
#include "ostg_if.h"
#include "drv_od.h"	
#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif
#ifdef CC_DRV_SUPPORT_SMICRO
#include "SMicro/SMicro_if.h"
#endif
#include "x_gpio.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#include "drv_mjc_if.h"
#endif
#ifdef CC_SCPOS_3DTV_SUPPORT
#include "drv_tdtv.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif
#endif

#include "drv_pwm.h"
#include "drv_vdoclk.h"
#include "drv_scaler_drvif.h"
#include "drvcust_if.h"  //mtk70763, 2010-7-21
#ifdef DRV_SUPPORT_EXTMJC
#include "extmjc_if.h"
#endif
#include "frametrack_drvif.h"

#include "eeprom_if.h"
#include "osd_drvif.h"
//luis@2007/7/5, for vRegWriteFldXXX
/*lint -save -e666*/

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CC_BACKPORCH_MIN (99 + 15)
#define CC_FRONTPORCH_MIN (10)
#define CC_BACKPORCH_MAX (2047)
#define CC_FRONTPORCH_MAX (30)
#define OSTG_UPDATE_PIXEL_SHIFT_H 1
#define OSTG_UPDATE_PIXEL_SHIFT_V 2

// force the HTotal to be 4x-aligned in Dispmode + Dual or Four port panel
#define CC_HTOTAL_4X_LIMIT_WA

// when VOUT_H is smaller than HTotal, VSync will be lost.
#define CC_OSTG_WA1

#if CC_OSD_MUTE_WA
UINT8 _bDddsDelayFlag = 0 ;
#endif

UINT32 _u4NSDHTotal=0xff;
UINT32 _u4NSDVTotal=0xff;
UINT32 _u4NSDFrameRate=0xff;

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

extern UINT32 _u4VPllFrequency;//for dynamic MDR


//-----------------------------------------------------------------------------
// Public variables
//-----------------------------------------------------------------------------

//UINT32 dwSI_DISPLAY_DCLK_TYPICAL = 0x00;
//UINT32 dwSI_DISPLAY_DCLK_50HZ;
//#ifdef SUPPORT_PANEL_48HZ
//UINT32 dwSI_DISPLAY_DCLK_48HZ;
//#endif

//Give initial value instead of using "CC_DVBT_SUPPORT" define
UINT8 bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
UINT8 bOUTPUT_REFRESH_RATE = 0;

UINT8 bDISPLAY_INTERLACE_ENABLE;
BOOL _fgFRS_ENA;
enum eSyncLockMode _eSyncLockMode = eSYNCLOCK_IGNORED;

#define IMPORT_PROTECTION 1
#define IMPORT_PROTECTION_RECOVER 3
UINT8 _bIsImportProtection = IMPORT_PROTECTION;

UINT32 _u4ErrorLimit = 0x3f;

static UINT32 _u4OstgUpdateEvent = 0;

#ifdef SUPPORT_SW_FRAME_CHECK
UINT32 _u4VDistUpperBound = 0x800 + 2;      // target in-out VSync distance
UINT32 _u4VDistLowerBound = 0x800;      // target in-out VSync distance
UINT32 _u4VLossDistTorance = 15;
UINT32 _u4VDistTorance = 1;
UINT32 _u4VDistTrackState = 0;          // 1: down, 2: up, 3:locked
UINT32 _u4UpTrackRange = 0;
UINT32 _u4DownTrackRange = 0;
UINT32 _u4VDistTrackMaxStep = 3;          
UINT32 _u4VDistTrackEn = 0;  
UINT8 Threadhold = 20;
UINT8 _u4VDistTrackLocked = 0;
#endif

#ifdef SUPPORT_CW_CHANGE_BY_STEP
UINT32 _u4TargetCW = 0;
UINT32 _u4CurrentCW = 0;
UINT32 _u4CWStep = 0x500000;
#endif

#ifdef SUPPORT_DDDS_STEP_TRACKING
UINT32 _u4CenterCW= 0;
UINT16 _u2DDDSStepTrackingEn = 0;
UINT32 _u4DDDSTrackingErrorLimit = 2;        
UINT32 _u4StepInCWTolerance = 0x1000;  
UINT32 _u4CurrentStepCW = 0;
UINT32 _u4StepInCWUpperBound = 0;
UINT32 _u4StepInCWLowerBound = 0;
UINT32 _u4DefaultErrorLimit = 0xf;
#endif

#ifdef CC_MT5396
static UINT16 _u2IntFrameRate = 0;
#endif

UINT8 _bForceFrameRate = 0;

UINT8 bSupportLocalDimming = SV_FALSE;

UINT8 _u1IsTTDWxgaWA = 0;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
static void vDrvDDDSTrackingNormProc(void);
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
static INT32 i4OrigControlWord = 0x7fffffff;
#define STEP_CW (4)
#define STEP_CW_DELAY (50)
#define STEP_CW_DELAY2 (250)
static UINT8 u1StepControlWord = 0;
static UINT8 u1StepDelay = 0;
static UINT8 u1StepDelay2 = 0;
static UINT8 u1DDDSLockStatus = 0;
static UINT8 u1SignalStatus = SV_VDO_NOSIGNAL;
static UINT32 TargetErrlimit = 3;
#endif

#ifdef CC_SCPOS_3DTV_SUPPORT 
static void vDrvDisp3DModeCB(void);
static void vSavePanelAttribute(void);
#ifdef DRV_SUPPORT_MT8283
extern BOOL fgForceChangeRegion = FALSE;
#endif
#endif

#endif

static BOOL fgTestmode = FALSE;

static void _vScpipInitPanelProtect(void);
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
UINT16 wDrvGetOutputHTotal(void)
{
    if (fgIsMJCToOSTG())
    {
        return ((UINT16) IO32ReadFldAlign(PSPIP_OA_00, PSPIP_OA_00_PANEL_HTOTAL));
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL));
	}
}

void wDrvSetOutputHTotal(UINT16 u2HTotal)
{
    if (fgIsMJCToOSTG())
    {
        vIO32WriteFldAlign(PSPIP_OA_00, u2HTotal ,PSPIP_OA_00_PANEL_HTOTAL);
    }
    //else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_00, u2HTotal ,PIP_OA_00_PANEL_HTOTAL);
    }
}

static UINT16 wDrvGetOutputHTotalStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) IO32ReadFldAlign(PSPIP_OA_RO_00, PSPIP_OA_RO_00_RD_MOHLEN)+1);
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_RO_00, PIP_OA_RO_00_RD_MOHLEN)+1);
	}
}

UINT16 wDrvGetOutputVTotal(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) IO32ReadFldAlign(PSPIP_OA_01, PSPIP_OA_01_PANEL_VTOTAL));
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL));
	}
}

static UINT16 wDrvGetOutputVTotalStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) IO32ReadFldAlign(PSPIP_OA_RO_00, PSPIP_OA_RO_00_RD_MOVLEN)+1);
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_RO_00, PIP_OA_RO_00_RD_MOVLEN)+1);
	}
}

void wDrvSetOutputVTotal(UINT16 u2VToal)
{

    if (fgIsMJCToOSTG())
    {
	    vIO32WriteFldAlign(PSPIP_OA_01, u2VToal, PSPIP_OA_01_PANEL_VTOTAL);
    }
	//else
	{
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u2VToal, PIP_OA_01_PANEL_VTOTAL);
	}

}

void wDrvSetOutputHActive(UINT16 u2HActive)
{
    if (fgIsMJCToOSTG())
    {
        vIO32WriteFldAlign(PSPIP_OA_00, u2HActive, PSPIP_OA_00_PANEL_HACTIVE); 
    }
    //else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_00, u2HActive, PIP_OA_00_PANEL_HACTIVE); 
    }
}

UINT16 wDrvGetOutputHActive(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) IO32ReadFldAlign(PSPIP_OA_00, PSPIP_OA_00_PANEL_HACTIVE));
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HACTIVE));
	}
}


void wDrvSetOutputVActive(UINT16 u2VActive)
{
    if (fgIsMJCToOSTG())
    {
        vIO32WriteFldAlign(PSPIP_OA_01, u2VActive, PSPIP_OA_01_PANEL_VACTIVE);
    }
    //else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_01, u2VActive, PIP_OA_01_PANEL_VACTIVE);
    }
}

UINT16 wDrvGetOutputVActive(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT16) IO32ReadFldAlign(PSPIP_OA_01, PSPIP_OA_01_PANEL_VACTIVE));
    }
	else
	{
        return ((UINT16) RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VACTIVE));
	}

}

#define ODFPLimit 4
#define REORDERBPLimit 4
void vDrvSetHsyncFp(UINT16 u2Fp) 
{
    if (u2Fp <= ODFPLimit)
    {
        u2Fp = ODFPLimit;
    }

    if(LVDS_OUTPUT_PORT == DUAL_PORT)
	{
	 if( (u2Fp % 2) != 0 )
	 {
	  u2Fp = u2Fp + (2 - (u2Fp % 2));
 	 } 
	}	
	else if(LVDS_OUTPUT_PORT == FOUR_PORT)
	{
	 if (u2Fp % 4 !=  0)
	 {
	  u2Fp = u2Fp + (4 - (u2Fp % 4));
	 }
	} 

    if (fgIsMJCToOSTG())
    {
        vIO32WriteFldAlign(PSPIP_OA_08, u2Fp, PSPIP_OA_08_HSYNC_PORCH_F);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, u2Fp, PIP_OA_08_HSYNC_PORCH_F);
    }
}

void vDrvSetHsyncBp(UINT16 u2Bp) 
{
    if (u2Bp <= REORDERBPLimit)
    {
        u2Bp = REORDERBPLimit;
    }

    if(LVDS_OUTPUT_PORT == DUAL_PORT)
	{
	 if( (u2Bp % 2) != 0 )
	 {
	  u2Bp = u2Bp + (2 - (u2Bp % 2));
 	 } 
	}	
	else if(LVDS_OUTPUT_PORT == FOUR_PORT)
	{
	 if (u2Bp % 4 != 0)
	 {
	  u2Bp = u2Bp + (4 - (u2Bp % 4));
	 }
	} 
    
    if (fgIsMJCToOSTG())
    {
        vIO32WriteFldAlign(PSPIP_OA_08, u2Bp, PSPIP_OA_08_HSYNC_PORCH_B);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_08, u2Bp, PIP_OA_08_HSYNC_PORCH_B);
    }
}

UINT16 wDrvGetHsyncBp(void) 
{
    if (fgIsMJCToOSTG())
    {
        return ((UINT16) IO32ReadFldAlign(PSPIP_OA_08, PSPIP_OA_08_HSYNC_PORCH_B));
    }
    else
    {
        return ((UINT16)RegReadFldAlign(SCPIP_PIP_OA_08, PIP_OA_08_HSYNC_PORCH_B));
    }

}
UINT16 wDrvGetHsyncFp(void)
{
	if (fgIsMJCToOSTG())
	{
	 return ((UINT16)IO32ReadFldAlign(PSPIP_OA_08, PSPIP_OA_08_HSYNC_PORCH_F));
	} 
	else
	{
	 return ((UINT16)RegReadFldAlign(SCPIP_PIP_OA_08, PIP_OA_08_HSYNC_PORCH_F));
	}
}

void vDrvSetVsyncFp(UINT16 u2VFp)
{
 if (fgIsMJCToOSTG())
 {
  vIO32WriteFldAlign(PSPIP_OA_09, u2VFp, PSPIP_OA_09_VSYNC_PORCH_F);
 } 
 else
 {
  vRegWriteFldAlign(SCPIP_PIP_OA_09, u2VFp, PIP_OA_09_VSYNC_PORCH_F);
 }
 
 //for 5396 MP, Front scaler front porch
 if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC))
 {
    UINT32 u4VPorchF;
    UINT32 u4PipVtotal = RegReadFldAlign(SCPIP_PIP_OA_01, PIP_OA_01_PANEL_VTOTAL);
    if ((u4PipVtotal - wDrvGetOutputVActive())/2 > 20) // must larger than pre_vofst + pre_vofst_dram
    {
      u4VPorchF = (u4PipVtotal - wDrvGetOutputVActive())/2;
    }
    else
    {
      u4VPorchF = 4; // previous setting, for WXGA?
    }
    LOG(0, "Vtot %d Vact %d pre_vofst %d pre_vofst_dram %d VPorchF %d", u4PipVtotal, wDrvGetOutputVActive(), RegReadFldAlign(SCPIP_PIP_OA_05, PIP_OA_05_US_VOFST1), RegReadFldAlign(SCPIP_PIP1_0C, PIP1_0C_PRE_VOFST_DRAM_1), u4VPorchF);
    vRegWriteFldAlign(SCPIP_PIP_OA_09, u4VPorchF, PIP_OA_09_VSYNC_PORCH_F); 
 } 
}

void vDrvSetVsyncBp(UINT16 u2VBp)
{
 if (fgIsMJCToOSTG())
 {
  vIO32WriteFldAlign(PSPIP_OA_09, u2VBp, PSPIP_OA_09_VSYNC_PORCH_B);
 } 
 else
 {
 vRegWriteFldAlign(SCPIP_PIP_OA_09, u2VBp, PIP_OA_09_VSYNC_PORCH_B);
 }
}


UINT16 wDrvGetVsyncFp(void)
{
 if (fgIsMJCToOSTG())
 {
  return ((UINT16)IO32ReadFldAlign(PSPIP_OA_09, PSPIP_OA_09_VSYNC_PORCH_F));
 } 
 else
 {
  return ((UINT16)RegReadFldAlign(SCPIP_PIP_OA_09, PIP_OA_09_VSYNC_PORCH_F));
 }
}

UINT16 wDrvGetVsyncBp(void)
{
 if (fgIsMJCToOSTG())
 {
  return ((UINT16)IO32ReadFldAlign(PSPIP_OA_09, PSPIP_OA_09_VSYNC_PORCH_B));
 } 
 else
 {
 return ((UINT16)RegReadFldAlign(SCPIP_PIP_OA_09, PIP_OA_09_VSYNC_PORCH_B));
 }
}

/**
 * @brief bDrvCheckMTKGoodStatus

 * Check MTK Good Clock Lock or Unlock

 * @param  void
 * @retval TRUE/FALSE
 */
UINT8 bDrvCheckMTKGoodStatus(void)
{
    return (IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_LOCK));
}

UINT32 fgDrvSetMJCToOSTG(UINT32 fgOnOff)
{
    #ifdef CC_MT5396
    if (fgOnOff == RegReadFldAlign(OSTG_01, REG_MJC_IN_EN))
    {
        return FALSE;
    }

    vRegWriteFldAlign(OSTG_01, fgOnOff, REG_MJC_IN_EN);
    vDrvSetLCDTiming();

    return TRUE;
    #else
    return FALSE;
    #endif
}

UINT32 fgIsMJCToOSTG(void)
{
    #ifdef CC_MT5396
    return (RegReadFldAlign(OSTG_01, REG_MJC_IN_EN) == 1);
    #else
    return FALSE;
    #endif
}

 #ifndef CC_MTK_LOADER

#ifdef SUPPORT_SW_FRAME_CHECK
UINT16 u4ScposGetInOutVDistance(void)
{
    return RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_VCOUNTER_OUT_1);
}
#endif

#define SUPPORT_IN_LOW_FRAME_RATE 36
/**
 * @brief vDrvSetMTKGoodDclk

 * Set MTK Good Dclk Parameters

 * @param  wVERT_LINE: Input Total Lines in the Vertical Direction
 * @retval void
 */ 
void vDrvSetMTKGoodDclk(UINT16 u2InputVTotal, UINT8 bInputFrameRate) // Called by Video
{
    UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM;
    UINT32 u4HTotal, u4VTotal;
    #if ENABLE_DDDS2_FEEDBACK
     UINT8 u1VopllRatio;
    #endif
    #ifdef SUPPORT_SW_FRAME_CHECK
    UINT8 u4VStep;
    UINT32  u4VDistance;
    #endif

    if ((u2InputVTotal == 0) || (bInputFrameRate == 0))
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    u4HTotal = (UINT32)wDrvGetOutputHTotal();
    u4VTotal = (UINT32)wDrvGetOutputVTotal();

    #ifdef SUPPORT_SW_FRAME_CHECK
    if (_u4VDistTrackState == 1) // down tracking
    {   
        u4VStep =  (_u4DownTrackRange > _u4VDistTrackMaxStep)? _u4VDistTrackMaxStep: _u4DownTrackRange;
        u4VTotal = (u4VTotal > u4VStep)? (u4VTotal -u4VStep): 0; 
    }
    else if (_u4VDistTrackState == 2)  // up tracking
    {
        u4VStep =  (_u4UpTrackRange > _u4VDistTrackMaxStep)? _u4VDistTrackMaxStep: _u4UpTrackRange;
        u4VTotal = u4VTotal +u4VStep; 
    }

    if((_u4VDistTrackState == 1) ||(_u4VDistTrackState == 2))
    {
        u4VDistance = u4ScposGetInOutVDistance();
        if ((u4VDistance >= (wDrvGetOutputVTotal()/ 2))
                && (u4VDistance <= wDrvGetOutputVTotal()))
        {
            u4VDistance = wDrvGetOutputVTotal() - u4VDistance;
        }

        if ((u4VStep <= _u4VDistTrackMaxStep) && (u4VStep >= 2) && (u4VDistance <= Threadhold))
        {
            _u4VDistTrackMaxStep = u4VStep / 2;
        }

    }

    #endif     
    {
        DISPLAY_SIZE = u4HTotal * u4VTotal;
    }
    
    //bInputFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);
    #if SUPPORT_DDDS_ON_24P_25P_30P
    if (((bInputFrameRate== 24) || (bInputFrameRate == 25) || (bInputFrameRate== 30)) 
    	&& (bDrvVideoIsSrcInterlace(SV_VP_MAIN) == SV_FALSE))
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "1DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
    else
    #endif
    if ((bInputFrameRate < SUPPORT_IN_LOW_FRAME_RATE)&& (bDrvVideoIsSrcInterlace(SV_VP_MAIN) == SV_FALSE))
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "0DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
    else		
    if (((bInputFrameRate== 48) || (bInputFrameRate== 100) || (bInputFrameRate == 120)))
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "2DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
    else if (PANEL_IsSupport120Hz())
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "3DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
    else if (PANEL_IsSupport240Hz())
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "4DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
	#ifdef CC_MT5396
	else if (u1DrvMJCIsSupport50To120() == SV_TRUE)
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "4DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }
	#endif
    else
    {
        DISPLAY_SIZE = DISPLAY_SIZE * bOUTPUT_REFRESH_RATE / bInputFrameRate; 
        LOG(3, "5DDDS %dHz --> %dHz\n", bInputFrameRate, bOUTPUT_REFRESH_RATE);
    }


    DISPLAY_SIZE = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? DISPLAY_SIZE * 2 : DISPLAY_SIZE;

    #if ENABLE_DDDS2_FEEDBACK
    if ((LVDS_OUTPUT_PORT == SINGLE_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=1;		
    else  if (LVDS_OUTPUT_PORT == SINGLE_PORT) u1VopllRatio=0;
    else  if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=2;		
    else  if ((LVDS_OUTPUT_PORT == DUAL_PORT)&&(IS_PANEL_PDP)) u1VopllRatio=2;			
    else  if (LVDS_OUTPUT_PORT == DUAL_PORT) u1VopllRatio=1;
    else  if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_LVDS_HIGHSPEED)) u1VopllRatio=3;
    else  if ((LVDS_OUTPUT_PORT == FOUR_PORT)&&(IS_PANEL_PDP)) u1VopllRatio=3;    
    else u1VopllRatio=2;
	if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_EPI)
    {
        if(u1VopllRatio == 1)
            DISPLAY_SIZE = DISPLAY_SIZE / 3;
        else if(u1VopllRatio == 2)
            DISPLAY_SIZE = DISPLAY_SIZE / 6;
            
    }
    else
    {
        DISPLAY_SIZE = DISPLAY_SIZE >> u1VopllRatio;
    }
    #endif

    #ifdef CC_DEBUG
    Printf("u1VopllRatio = %d\n",u1VopllRatio);
	#endif
    
    HLEN_INT = (UINT32) (DISPLAY_SIZE / u2InputVTotal);
    HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * (UINT32) u2InputVTotal);
    LOG(6, "inVTaol: 0x%X, outV: 0x%X, outH: 0x%X, HLEN_INT: 0x%X, HLEN_NUM: 0x%X\n",
    u2InputVTotal, wDrvGetOutputVTotal(), wDrvGetOutputHTotal(), HLEN_INT, HLEN_NUM);
    
    #ifdef SUPPORT_SW_FRAME_CHECK
    #if 0
    HLEN_INT += u4CoarseStepSize;
    
    if(u4CurrentStep!=0 && u4CurrentStep<=200)
    {
        LOG(5,"Chnage HLEN_NUM from (%d) to ",HLEN_NUM);
        if(u4CurrentStep>=FINE_STEP_CENTER)
        {
            HLEN_NUM = HLEN_NUM + (u4FineStepSize * (u4CurrentStep - FINE_STEP_CENTER));
        }
        else
        {
            HLEN_NUM = HLEN_NUM - (u4FineStepSize * (FINE_STEP_CENTER - u4CurrentStep));
        }
        LOG(5,"(%d)\n",HLEN_NUM);        
    }
    #endif    
    #endif    

	#ifdef SUPPORT_DDDS_STEP_TRACKING
    vIO32WriteFldAlign(DDDS_03,_u4ErrorLimit,DDDS_03_DDDS_ERR_LIM);
	#endif
    vIO32WriteFldAlign(DDDS_02,(UINT16)u2InputVTotal,DDDS_02_HLEN_DEN);
    vIO32WriteFldAlign(DDDS_01,(UINT16)HLEN_NUM,DDDS_01_HLEN_NUM);    
    vIO32WriteFldAlign(DDDS_01,HLEN_INT,DDDS_01_HLEN_INT);

   // LOG(3, "HLEN=0x%x . [0x%x / 0x%x] \n",HLEN_INT, HLEN_NUM,u2InputVTotal);    
}
#endif // end of #ifndef CC_MTK_LOADER


void vDrvSetDDDSCloseloop(UINT8 bOn_Off)
{
 static UINT8 fgCloseloop_2nd = 0;

 //if ((IS_DISPLAY_FREERUN) || (fgDisplayFreeRun))
 //{
//	 vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN)|P_Fld(0,DDDS_00_FIX_FS_DDDS_SEL));
// }
 //else
 {
 	if (bOn_Off)
 	{
    	if(fgCloseloop_2nd)
    	{
			vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));	
    	}
		else
		{
	    	// overcome the clock jump when 1st enter close loop
			vIO32WriteFldAlign(DDDS_00, 1, DDDS_00_DISP_EN);	
			HAL_Delay_us(20);
			vIO32WriteFldAlign(DDDS_00, 1, DDDS_00_FIX_FS_DDDS_SEL);
			fgCloseloop_2nd = 1;
		}
 	}
 	else
 	{
			vIO32WriteFldMulti(DDDS_00,P_Fld(0,DDDS_00_DISP_EN)|P_Fld(0,DDDS_00_FIX_FS_DDDS_SEL));
 	}
 }
}

/**
 * @brief vDrvSwitchMTKGoodDclk

 * MTK Good Dclk ON/OFF

 * @param  bOn_Off: SV_ON/SV_OFF
 * @retval void
 */
void vDrvSwitchMTKGoodDclk(UINT8 bOn_Off)
{
#ifndef CC_MTK_LOADER
#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    UINT32 u4TmpCw;
    UINT8 u1Kp0=0;
    UINT8 u1Ki0=0;
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
    {
        if(u1DDDSLockStatus != bOn_Off)
        {
            u1DDDSLockStatus = bOn_Off;
            u4TmpCw = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
            u1StepControlWord = 0;
            u1StepDelay = 0;
            u1StepDelay2 = 0;
            if( i4OrigControlWord != _u4TargetCW )
            {
                i4OrigControlWord = _u4TargetCW;
            }
            if (DRVCUST_PanelQuery(eDDDSErrorLimit, &TargetErrlimit) == 0)
            {
                        if(TargetErrlimit > 3)
                vDrvSetErrorLimit(3);
            }
        }
    }
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if (bOn_Off == SV_ON)
        {
          if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
          {
//            if( bGetVideoDecType(SV_VP_MAIN) != SV_VD_DVI)
//                vIO32WriteFldMulti(DDDS_00,P_Fld(1,DDDS_00_DISP_EN)|P_Fld(1,DDDS_00_FIX_FS_DDDS_SEL));
          }
          else
          {
             vDrvSetDDDSCloseloop(1);
          }
    
        }
#else
    if (bOn_Off == SV_ON)
    {
		vDrvSetDDDSCloseloop(1);
    }
#endif
    else
    {
        #ifdef SUPPORT_SW_FRAME_CHECK
            _u4UpTrackRange = 0;
            _u4DownTrackRange = 0;
            _u4VDistTrackState = 0;
            _u4VDistTrackLocked = 0;
        #endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
        {
            u4TmpCw = IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_FREQ_CW);
            u1Ki0 = IO32ReadFldAlign(DDDS_02, DDDS_02_DDDS_KI_0);
            u1Kp0 = IO32ReadFldAlign(DDDS_02, DDDS_02_DDDS_KP_0);
            vIO32WriteFldMulti(DDDS_02, P_Fld(0,DDDS_02_DDDS_KI_0)|P_Fld(0,DDDS_02_DDDS_KP_0));
            vIO32WriteFldAlign(DDDS_00, u4TmpCw, DDDS_00_DDDS_FREQ_CW);
        }
#endif

        vDrvSetDDDSCloseloop(0); 

        vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
        #ifdef CC_COPLAT_MT82
        vUtDelay2us(50);
        #else
        HAL_Delay_us(100);
        #endif
        vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT);   

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
        if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
        {
            vIO32WriteFldMulti(DDDS_02, P_Fld(u1Ki0,DDDS_02_DDDS_KI_0)|P_Fld(u1Kp0,DDDS_02_DDDS_KP_0));
        }
#endif
    }
    #endif //end of #ifndef CC_MTK_LOADER 
}

UINT32 u4GetPanelVSyncBp(void)
{
    UINT32 u4VPorchB;

    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC 
    u4VPorchB = PANEL_GetVSyncBpByTiming(bSI_DISPLAY_DCLK_TYPE);
    #else
    u4VPorchB = (V_POS < (0x400 - VS_WIDTH + 6))?((0x400 - V_POS - VS_WIDTH + 6)):(0);

    if( (BSP_GetIcVersion() >= IC_VER_5368_AB && BSP_GetIcVersion() <= IC_VER_5368_AC) ||
        (BSP_GetIcVersion() >= IC_VER_5389_AB && BSP_GetIcVersion() <= IC_VER_5389_AC))    
    {
        LOG(3, "VS_WIDTH(%d) u4VPorchB(%d) \n", VS_WIDTH, u4VPorchB);

        if((VS_WIDTH + u4VPorchB) < 9)    //9=5+4. 5 for us_ofst and 4 for pre_vofst_dram in scaler dram mode.
        {
            u4VPorchB = 9 - VS_WIDTH;
            LOG(3, "Change u4VPorchB to (%d)\n", u4VPorchB);
        }
    }
    #endif

    return (u4VPorchB);
} 

// LVDS power------------------------------------------------------------------
void vDrvTurnOnPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        if(DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)value);
        }
    } while (0);
}

void vDrvTurnOffPanel()
{
    do 
    { 
        UINT32 gpio, value; 
        if(DRVCUST_PanelQuery(ePanelPowerGpio, &gpio) == 0 && 
           DRVCUST_PanelQuery(ePanelPowerOnVal, &value) == 0)
        {
            vDrvGpioSet((INT32)gpio, (INT32)!value);
        }
    } while (0);
}
//-----------------------------------------------------------------------------
/** turn on/off panel lvds signal. (used by MTAL)
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSSignalOnOff(BOOL fgOnOff)
{
	if(fgOnOff)
	{
		vDrvLVDSOn();
	}
	else
	{
		vDrvLVDSOff();
	}
}

//-----------------------------------------------------------------------------
/** turn on/off panel lvds powe. (used by MTAL)
 *  @param fgEnable: Control mode. It could be TRUE or FALSE
 *  @return
 */
//-----------------------------------------------------------------------------

void vDrvLVDSPowerOnOff(BOOL fgOnOff)
{
	if(fgOnOff)
	{
		vDrvTurnOnPanel();
	}
	else
	{
		vDrvTurnOffPanel();
	}
}

void vDrvVsyncISRSetPosition(UINT16 u2INTRLine)
{
    vIO32WriteFldAlign(INTR_01, 1, REG_INTR_LINE_LATCH_OFF);
    vIO32WriteFldAlign(INTR_01, u2INTRLine, REG_INTR_LINE); 
    vIO32WriteFldAlign(INTR_01, 0, REG_INTR_LINE_LATCH_OFF);
}

static UINT16 vDrvVsyncISRGetPosition(void)
{
    return(IO32ReadFldAlign(INTR_01, REG_INTR_LINE)); 
}

static void vDrvVsyncISRInit(void)
{
    if (IS_PANEL_PDP)
    {
        vDrvVsyncISRSetPosition((wDrvGetOutputVTotal()/2)-4); 
    }
    else
    {
        vDrvVsyncISRSetPosition(wDrvGetOutputVTotal()-4); 
		
    }
}

void vDrvSetTCONIrqClear(UINT32 u4INTRSource)
{
    vIO32WriteFldAlign(INTR_02, u4INTRSource, C_INTR_CLEAR);
    vIO32WriteFldAlign(INTR_02, 0, C_INTR_CLEAR);
}

void vDrvGetTCONIrqStatus(UINT32 * u4INTRStatus)
{
    *u4INTRStatus = IO32ReadFldAlign(INTR_00, STATUS_INTR);
}

static void vDrvVsyncISRSetMask(UINT16 u2Mask)
{
    vIO32WriteFldAlign(INTR_02, u2Mask,C_INTR_MASK); 
}

static UINT16 vDrvVsyncISRGetMask(void)
{
    return(IO32ReadFldAlign(INTR_02, C_INTR_MASK)); 
}

void vDrvVsyncISRStatus(void)
{
    UINT16 u2MaskStatus;

	u2MaskStatus = vDrvVsyncISRGetMask();
		
	Printf("====Interrupt status====\n");
    Printf("[Interrupt Line] %d \n",vDrvVsyncISRGetPosition());
    Printf("[Interrupt Mask] 0x%x \n",u2MaskStatus);	
	if((u2MaskStatus >> 0) & 0x01)
	Printf("[Interrupt Mask] disable LR_RISIN \n");	
	if((u2MaskStatus >> 1) & 0x01)
	Printf("[Interrupt Mask] disable LR_FALLING \n");	
	if((u2MaskStatus >> 2) & 0x01)
	Printf("[Interrupt Mask] disable L_RISING \n");	
	if((u2MaskStatus >> 3) & 0x01)
	Printf("[Interrupt Mask] disable L_FALLING \n");	
	if((u2MaskStatus >> 4) & 0x01)
	Printf("[Interrupt Mask] disable R_RISING \n");	
	if((u2MaskStatus >> 5) & 0x01)
	Printf("[Interrupt Mask] disable R_FALLING \n");
	if((u2MaskStatus >> 6) & 0x01)
	Printf("[Interrupt Mask] disable OSTGLINE \n");
	if((u2MaskStatus >> 7) & 0x01)
	Printf("[Interrupt Mask] disable AMBILIT \n");
	
}

void vDrvVsyncISRSetting(UINT16 u2Line, UINT16 u2Mask)
{
    vDrvVsyncISRSetPosition(u2Line);
	vDrvVsyncISRSetMask(u2Mask);

	if ((u2Line>wDrvGetOutputVTotal())&&(!(IS_PANEL_PDP)))
	{
     Printf("u2INTRLine(%d) over Panel V total (%d)\n",u2Line, wDrvGetOutputVTotal());
	 Printf("Interrupt will not active\n");
	}
}


static UINT8 wDrvGetScalerDISPR_INStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT8) IO32ReadFldAlign(PSPIP_OA_RO_08, PSPIP_OA_RO_08_RD_DISP_IN_R));
    }
	else
	{
        return ((UINT8) RegReadFldAlign(SCPIP_PIP_OA_RO_08, PIP_OA_RO_08_RD_DISP_IN_R));
	}
}

static UINT8 wDrvGetScalerDISPR_OUTStatus(void)
{
    if (fgIsMJCToOSTG())
    {
	    return ((UINT8) IO32ReadFldAlign(PSPIP_OA_RO_08, PSPIP_OA_RO_08_RD_DISP_OUT_R));
    }
	else
	{
        return ((UINT8) RegReadFldAlign(SCPIP_PIP_OA_RO_08, PIP_OA_RO_08_RD_DISP_OUT_R));
	}
}

static UINT8 wDrvGetLVDSDISPR_OUTStatus(void)
{
    return ((UINT8) IO32ReadFldAlign(MLVDS_SO8, ST_LVDS_DISP_R));
}

void vGetPostScalerStatus(void)
{
    UINT8 u1Cnt;
#ifdef CC_VESTEL_ESD_TEST
    Printf("[ESD]_fgVideoReady=%d\n", _fgVideoReady);
#endif

    Printf("===== [Setting] ===== \n");
	Printf("[HSync] Htoal=%d Hactive=%d  Hfp= %d Hbp= %d Hwidth=%d\n", 
		   wDrvGetOutputHTotal(),
		   wDrvGetOutputHActive(), 
		   wDrvGetHsyncFp(), 
		   wDrvGetHsyncBp(),
           wDrvGetOutputHTotal()-wDrvGetOutputHActive()-wDrvGetHsyncFp()-wDrvGetHsyncBp()
	);
	Printf("[VSync] Vtoal=%d Vactive=%d  Vfp= %d Vbp= %d Vwidth=%d\n", 
		   wDrvGetOutputVTotal(),
		   wDrvGetOutputVActive(), 
		   wDrvGetVsyncFp(), 
		   wDrvGetVsyncBp(),
		   wDrvGetOutputVTotal()-wDrvGetOutputVActive()-wDrvGetVsyncFp()-wDrvGetVsyncBp()
	);
	if (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)
	Printf("[Frame rate] SV_DCLK_48HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_50HZ)
	Printf("[Frame rate] SV_DCLK_50HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ)
	Printf("[Frame rate] SV_DCLK_60HZ\n");
	else if  (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS)
	Printf("[Frame rate] SV_DCLK_OTHERS\n");
	else if  ( bSI_DISPLAY_DCLK_TYPE == SV_DCLK_UNKNOW)
	Printf("[Frame rate] SV_DCLK_UNKNOW\n");

    Printf("Output refresh rate =%d Hz\n", vDrvGetLCDFreq());	

    Printf("===== [Status] ===== \n");
	Printf("HTotal =%d\n", wDrvGetOutputHTotalStatus());
	Printf("VTotal =%d\n", wDrvGetOutputVTotalStatus());
	Printf("DISP_R_IN =%d\n", wDrvGetScalerDISPR_INStatus());
	Printf("DISP_R_OUT=%d\n", wDrvGetScalerDISPR_OUTStatus());	

    Printf("===== [OSTG Status] ===== \n");
	Printf("[Input]DISP_R=%d\n", RegReadFldAlign(DEBUG_01, DEBUG_01_PANEL_DISP_R));	
	Printf("[Input]HTotal =%d\n", RegReadFldAlign(DEBUG_02, DEBUG_02_INPUT_HTOTAL));
	Printf("[Input]VTotal =%d\n", RegReadFldAlign(DEBUG_02, DEBUG_02_INPUT_VTOTAL));

	Printf("[Color]HTotal =%d\n", RegReadFldAlign(TIMING_DETECTION_0, COLOR_HTOTAL_CNT));
	Printf("[Color]HActive =%d\n", RegReadFldAlign(TIMING_DETECTION_1, COLOR_HDE_CNT)); 
	Printf("[Color]VTotal =%d\n", RegReadFldAlign(TIMING_DETECTION_0, COLOR_VTOTAL_CNT));
	Printf("[Color]VActive =%d\n", RegReadFldAlign(TIMING_DETECTION_1, COLOR_VDE_CNT)); 
	
	vRegWriteFldAlign(DEBUG_00, 1, DEBUG_00_R_DEBUG_OSTG_EN);
	HAL_Delay_us(20000);

    for (u1Cnt = 0; u1Cnt<4; u1Cnt++)
    {
	  vRegWriteFldAlign(DEBUG_01, u1Cnt, DEBUG_01_R_HVM_INPUT_SEL);	
	  HAL_Delay_us(40000);

	  if (RegReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 0)
	  {
       Printf("The interface between OD to OSTG (default)\n");
	  }
	  else if (RegReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 1)
	  {
       Printf("The interface between OSTG to OD\n");
	  }	
	  else if (RegReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 2)
	  {
       Printf("The interface between MJC to OSTG\n");
	  }	
	  else if (RegReadFldAlign(DEBUG_01,DEBUG_01_R_HVM_INPUT_SEL) == 3)
	  {
       Printf("The input of OSTG (Note: this option will be fail when project exists MJC)\n");
	  }		  

	 Printf("[Output]HTotal =%d\n", RegReadFldAlign(DEBUG_03, DEBUG_03_HVM_HTOTAL_CNT));
	 Printf("[Output]H Active =%d\n", RegReadFldAlign(DEBUG_04, DEBUG_04_HVM_HDE_CNT));	 
	 
	 Printf("[Output]VTotal =%d\n", RegReadFldAlign(DEBUG_03, DEBUG_03_HVM_VTOTAL_CNT));	
	 Printf("[Output]V Active =%d\n", RegReadFldAlign(DEBUG_04, DEBUG_04_HVM_VDE_CNT));
    }

	vRegWriteFldAlign(DEBUG_00, 0, DEBUG_00_R_DEBUG_OSTG_EN);

}


void vGetDISP_RStatus(void)
{
    Printf("==[DISP_R Setting - Normal Mode] == \n");
    if(IO32ReadFldAlign(MLVDS_SO0,MLVDS_SO0_RG_RES_FLD))
	Printf("DISP_R Encode in A Channel\n");		
    if(IO32ReadFldAlign(MLVDS_SO12,RG_RES_FIELD_L))
	Printf("DISP_R Encode in B Channel\n");	
    if(IO32ReadFldAlign(MLVDS_SO13,RG_RES_FIELD_U_4CH))
	Printf("DISP_R Encode in C Channel\n"); 	
    if(IO32ReadFldAlign(MLVDS_SO13,RG_RES_FIELD_L_4CH))
	Printf("DISP_R Encode in D Channel\n"); 
		
    Printf("==[DISP_R Setting - OSD Mode] == \n");
    if(IO32ReadFldAlign(MLVDS_SO2,RG_OSD_3D_IN_HS))
	Printf("DISP_R Encode in HSync\n");	
    if(IO32ReadFldAlign(MLVDS_SO2,RG_OSD_3D_IN_VS))
	Printf("DISP_R Encode in VSync\n");	
	
    Printf("==[DISP_R Status] == \n");
	Printf("[Scaler] DISP_R_IN =%d\n", wDrvGetScalerDISPR_INStatus());
	Printf("[Scaler] DISP_R_OUT=%d\n", wDrvGetScalerDISPR_OUTStatus());	
    Printf("[LVDS] DISP_R_OUT=%d\n", wDrvGetLVDSDISPR_OUTStatus());
}

void u1SetDISP_REncodeEn(UINT8 u1Enable)
{     
    vIO32WriteFldAlign(MLVDS_SO0,(u1Enable>>0 & 0x01),MLVDS_SO0_RG_RES_FLD);
    vIO32WriteFldAlign(MLVDS_SO12,(u1Enable>>1 & 0x01),RG_RES_FIELD_L);	
    vIO32WriteFldAlign(MLVDS_SO13,(u1Enable>>2 & 0x01),RG_RES_FIELD_U_4CH);	
    vIO32WriteFldAlign(MLVDS_SO13,(u1Enable>>3 & 0x01),RG_RES_FIELD_L_4CH);
    vIO32WriteFldAlign(MLVDS_SO2,(u1Enable>>4 & 0x01),RG_OSD_3D_IN_HS);
    vIO32WriteFldAlign(MLVDS_SO2,(u1Enable>>5 & 0x01),RG_OSD_3D_IN_VS);	
	if ((u1Enable>>4 & 0x01)==1 || (u1Enable>>5 & 0x01))
	{
	 vIO32WriteFldAlign(MLVDS_SO0,0x01,MLVDS_SO0_RG_RES_FLD);
	}
}

void vGetLVDS_HVEncodeStatus(void)
{   
    Printf("==[LVDS H/V Encode Value] == \n");
	if (IO32ReadFldAlign(MLVDS_SO0,MLVDS_SO0_RG_VS_SEL))
	{
     Printf("[VSYNC Enocde] Vsync encode value=%d\n", IO32ReadFldAlign(MLVDS_SO0,MLVDS_SO0_RG_CNTLF));
	}
	if (IO32ReadFldAlign(MLVDS_SO0,MLVDS_SO0_RG_HS_SEL))
	{
     Printf("[HSYNC Enocde] Hsync encode value=%d\n", IO32ReadFldAlign(MLVDS_SO0,MLVDS_SO0_RG_CNTLE));
	}	
}

void u1SetLVDS_HVEncodeValue(UINT8 u1Enable)
{     
    vIO32WriteFldAlign(MLVDS_SO0,(u1Enable>>0 & 0x01),MLVDS_SO0_RG_VS_SEL);
    vIO32WriteFldAlign(MLVDS_SO0,(u1Enable>>1 & 0x01),MLVDS_SO0_RG_CNTLF);	
    vIO32WriteFldAlign(MLVDS_SO0,(u1Enable>>2 & 0x01),MLVDS_SO0_RG_HS_SEL);	
    vIO32WriteFldAlign(MLVDS_SO0,(u1Enable>>3 & 0x01),MLVDS_SO0_RG_CNTLE);	
}

BOOL TconVsyncCheck = 0;
UINT16 u2HsyncFp_Tbl = 0;

/**
 * @brief vDrvSetLCDTiming

 * Set the Display Output Timing for LCD Panel

 * @param  void
 * @retval void
 */
void vDrvSetLCDTiming(void)
{
    UINT32 u4SyncLen;
    UINT32 u4HPorchF, u4HPorchB;
    UINT32 u4VPorchF, u4VPorchB;
    UINT32 u4VTotal;
    static UINT8 _initImport = 0;
	#if defined(CC_MT5396)
    UINT32 u4VDelay, u4HDelay, u4HDutyCycle;
    #endif

    if (_initImport == 0)
    {
        _initImport = 1;
        vDrvSwitchImportProtection(0);
    }
    else if( _bIsImportProtection )
    {
        vDrvSwitchImportProtection(1);
    }

    // in most case, active width/height is not changed.
    #if CC_WXGA_TTD_WA
    if (wDISPLAY_WIDTH % 8 != 0)
	{
	    wDrvSetOutputHActive(((wDISPLAY_WIDTH + 7) >> 3) << 3);
	    _u1IsTTDWxgaWA = 1;
    }
    else
	{
	    wDrvSetOutputHActive(wDISPLAY_WIDTH);
	    _u1IsTTDWxgaWA = 0;
	}
    #else
	    wDrvSetOutputHActive(wDISPLAY_WIDTH);
    #endif
	wDrvSetOutputVActive(wDISPLAY_HEIGHT);

    if ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_60HZ) || 
        ((bSI_DISPLAY_DCLK_TYPE == SV_DCLK_OTHERS) && ((bOUTPUT_REFRESH_RATE >= (55*(PANEL_IsSupport120Hz()? 2: 1))) || bOUTPUT_REFRESH_RATE == 24 || bOUTPUT_REFRESH_RATE == 30) ))
    {
        //Set display output timing
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        #endif

        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC 
        u4SyncLen = PANEL_GetHSyncWidthByTiming(SV_DCLK_60HZ);
        #else        
        u4SyncLen = PANEL_GetHSyncWidth();
		#endif
        u4HPorchF = (H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0);
        u4HPorchB = HLEN_TOTAL_TYPI - wDISPLAY_WIDTH - u4HPorchF - u4SyncLen;

        #ifndef NDEBUG
        Printf("H_POS_60HZ=%d\n",H_POS_60HZ);
        Printf("HLEN_TOTAL_TYPI=%d\n",HLEN_TOTAL_TYPI);
        Printf("HSYNCLEN_TYPI=%d\n",u4SyncLen);
        Printf("u4HPorchF=%d\n",u4HPorchF);
        Printf("wDISPLAY_WIDTH=%d\n",wDISPLAY_WIDTH);        
		#endif
        
		wDrvSetOutputHTotal(HLEN_TOTAL_TYPI);
        vDrvSetHsyncFp(u4HPorchF);
        vDrvSetHsyncBp(u4HPorchB);

        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(FALSE);
        #endif
        u4VTotal = VLEN_TOTAL_TYPI;
		wDrvSetOutputVTotal(u4VTotal);

		#if defined(CC_MT5396)
        u4VDelay = PANEL_GetVDelay60Hz();
        u4HDelay = PANEL_GetHDelay60Hz();
        u4HDutyCycle = PANEL_GetHDutyCycle60Hz();
        Printf("GetVDelay60Hz     = %d\n",u4VDelay);
        Printf("GetHDelay60Hz     = %d\n",u4HDelay);
        Printf("GetHDutyCycle60Hz = %d\n",u4HDutyCycle);
        #endif
		
    }
    #ifdef SUPPORT_PANEL_48HZ
    else if (bSI_DISPLAY_DCLK_TYPE == SV_DCLK_48HZ)
    {
        //Set display output timing
        _OSD_AlwaysUpdateReg(TRUE);

            #ifdef SUPPORT_PANEL_CUSTOMER_SPEC 
			u4SyncLen = PANEL_GetHSyncWidthByTiming(SV_DCLK_48HZ);
            #else
            u4SyncLen = PANEL_GetHSyncWidth();
			#endif


            #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
            u4HPorchF = (PANEL_GetHPositionByTiming(SV_DCLK_48HZ) > 0x800)?(PANEL_GetHPositionByTiming(SV_DCLK_48HZ)-0x800):(0);
            #else
			  #ifdef SUPPORT_PANEL_48HZ
            	u4HPorchF = (H_POS_48HZ > 0x800)?(H_POS_48HZ-0x800):(0);            	
			  #else
                u4HPorchF = (H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0);
			  #endif
            #endif
			
			#ifdef SUPPORT_PANEL_48HZ
            u4HPorchB = HLEN_TOTAL_48HZ - wDISPLAY_WIDTH - u4HPorchF - u4SyncLen;    
			#else
            u4HPorchB = HLEN_TOTAL_TYPI - wDISPLAY_WIDTH - u4HPorchF - u4SyncLen;    
			#endif
			wDrvSetOutputHTotal(HLEN_TOTAL_48HZ);
    
        vDrvSetHsyncFp(u4HPorchF);
        vDrvSetHsyncBp(u4HPorchB);        

        _OSD_AlwaysUpdateReg(FALSE);
    
        u4VTotal = VLEN_TOTAL_48HZ;
        wDrvSetOutputVTotal(u4VTotal);

		#if defined(CC_MT5396)
        u4VDelay = PANEL_GetVDelay48Hz();
        u4HDelay = PANEL_GetHDelay48Hz();
        u4HDutyCycle = PANEL_GetHDutyCycle48Hz();
        Printf("GetVDelay48Hz     = %d\n",u4VDelay);
        Printf("GetHDelay48Hz     = %d\n",u4HDelay);
        Printf("GetHDutyCycle48Hz = %d\n",u4HDutyCycle);
        #endif

    }
    #endif //SUPPORT_PANEL_48HZ
    else
    {
        //Set display output timing
        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(TRUE);
        #endif

		#ifdef SUPPORT_PANEL_CUSTOMER_SPEC 
        u4SyncLen = PANEL_GetHSyncWidthByTiming(SV_DCLK_50HZ);
		#else
        u4SyncLen = PANEL_GetHSyncWidth();
		#endif
		
        if (H_POS_50HZ > 0)
        {
            u4HPorchF = (H_POS_50HZ > 0x800)?(H_POS_50HZ-0x800):(0);
        }
        else
        {
            u4HPorchF = (H_POS_60HZ > 0x800)?(H_POS_60HZ-0x800):(0);
        }
		
		u4HPorchB = HLEN_TOTAL_50HZ - wDISPLAY_WIDTH - u4HPorchF - u4SyncLen;
      
		wDrvSetOutputHTotal(HLEN_TOTAL_50HZ);
        vDrvSetHsyncFp(u4HPorchF);
        vDrvSetHsyncBp(u4HPorchB);         

        #ifndef CC_COPLAT_MT82
        _OSD_AlwaysUpdateReg(FALSE);
        #endif
        u4VTotal = VLEN_TOTAL_50HZ;
		wDrvSetOutputVTotal(u4VTotal);

		#if defined(CC_MT5396)
        u4VDelay = PANEL_GetVDelay50Hz();
        u4HDelay = PANEL_GetHDelay50Hz();
        u4HDutyCycle = PANEL_GetHDutyCycle50Hz();
        Printf("GetVDelay50Hz     = %d\n",u4VDelay);
        Printf("GetHDelay50Hz     = %d\n",u4HDelay);
        Printf("GetHDutyCycle50Hz = %d\n",u4HDutyCycle);
        #endif
    }

    // VPOS = (0x400 -u2VSyncWidth -u2VBackPorch +u2VSyncDelSel);
    #ifdef SUPPORT_PANEL_CUSTOMER_SPEC  
     u4VPorchB = u4GetPanelVSyncBp();	
     u4VPorchF = u4VTotal - wDISPLAY_HEIGHT - PANEL_GetVSyncWidthByTiming(bSI_DISPLAY_DCLK_TYPE) - u4VPorchB;
    #else 
	 u4VPorchB = u4GetPanelVSyncBp();
     u4VPorchF = u4VTotal - wDISPLAY_HEIGHT - VS_WIDTH - u4VPorchB;
    #endif
    LOG(0, "VPorch (%d %d)\n", u4VPorchF, u4VPorchB);
    vDrvSetVsyncFp(u4VPorchF);
    vDrvSetVsyncBp(u4VPorchB);

    if( _bIsImportProtection )
    {
	    vDrvFireImportPortection();
           _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
    }

    if(IS_PANEL_PDP)
    {
	 if (fgIsMJCToOSTG())
	 {
     vIO32WriteFldAlign(PSPIP_OA_02, 1, PSPIP_OA_02_SET_DATA_SEL);
     vIO32WriteFldMulti(PSPIP_OA_0A, P_Fld((wDISPLAY_HEIGHT), PSPIP_OA_0A_PANEL_VOFST)|
                                          P_Fld((wDISPLAY_HEIGHT-6), PSPIP_OA_0A_OSD_VOFST));    
     vIO32WriteFldAlign(PSPIP_OA_02, 0, PSPIP_OA_02_SET_DATA_SEL);	 
	 }
	 else
	 {
     vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SET_DATA_SEL);
     vRegWriteFldMulti(SCPIP_PIP_OA_0A, P_Fld((wDISPLAY_HEIGHT), PIP_OA_0A_PANEL_VOFST)|
                                          P_Fld((wDISPLAY_HEIGHT-6), PIP_OA_0A_OSD_VOFST));    
     vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SET_DATA_SEL);
	 }
    }

    // ISR Setting
    vDrvVsyncISRInit();

    // For 3D_Glasses Control - software work around
    vIO32WriteFldAlign(VINTF_P8, wDrvGetOutputVTotal(),  VINTF_REYE_WIDTH); 
    vIO32WriteFldAlign(VINTF_P9, 1,  VINTF_REYE_RST_MODE);    
    vIO32WriteFldAlign(VINTF_P10, wDrvGetOutputVTotal(),  VINTF_LEYE_WIDTH); 
    vIO32WriteFldAlign(VINTF_P11, 1,  VINTF_LEYE_RST_MODE);

    vErrorHandleSetByTiming();
	// osd Hstart setting 
	OSD_Base_SetHStart();

    if(IS_PANEL_PDP)
    {
	  TconVsyncCheck = 1;
	  u2HsyncFp_Tbl = IO32ReadFldAlign(PSPIP_OA_08, PSPIP_OA_08_HSYNC_PORCH_F);
	  Printf(" === 1st === u2HsyncFp_Tbl = %d!!!\n,",u2HsyncFp_Tbl);
    }

    #ifdef CC_SUPPORT_PANEL_PWMSCAN
	vDrvSetScanPWM(SrcPWM3,1,300,200);
	vDrvSetScanPWM(SrcPWM4,120,300,200);
	vDrvSetScanPWM(SrcPWM5,270,300,200);
	#endif
	
	#if defined(CC_MT5396)
	Panel_3DGlassesCTL(e3DGlasses_P_3DBL,e3DGlasses_S_LEYE,e3DGlasses_POl_Bypass,e3DGlasses_POS_1stR,u4VDelay,u4HDelay);
	Panel_3DGlassesDutyCTL(e3DGlasses_S_LEYE,u4HDutyCycle);
	Panel_3DGlassesCTLEn(e3DGlasses_P_3DL,1);
	#endif

}


#ifndef CC_MTK_LOADER
/**
 * @brief bDrvDispSupportTiming

 * Check if Dispmdoe supports the input timing

 * @param  wHeight: Source height
 * @param  wVTotal: Total Lines in the Vertical Direction
 * @param wFrameRate: Frame rate
 * @retval SV_SUCCESS/SV_FAIL
 */
UINT8 bDrvDispSupportTiming(UINT16 wHeight, UINT16 wVTotal, UINT16 wFrameRate)
{
    UINT32 u4OutputVTotal;
    UINT32 u4PanelHeight;

    u4PanelHeight = TV_HEIGHT;
    u4OutputVTotal = (u4PanelHeight * (UINT32)wVTotal + (wHeight >> 1))/(UINT32)wHeight;

    return bDrvDispSupport((UINT16)u4OutputVTotal, wFrameRate);
}


/**
 * @brief bDrvDispSupport

 * Check if Dispmdoe supports the output setting

 * @param  wOutputVTotal: Output Total Lines in the Vertical Direction
 * @param wOutputFrameRate: Output frame rate
 * @retval SV_SUCCESS/SV_FAIL
 */
UINT8 bDrvDispSupport(UINT16 wOutputVTotal, UINT16 wOutputFrameRate)
{
    UINT32 u4RefreshRate;
    UINT32 u4HTotal;
    UINT32 u4HTotalMinBySyncMin;
    UINT32 u4HTotalMinByPixelClock;
    UINT32 u4HTotalMaxByPixelClock;

    LOG(1, "DM Panel V(%d %d %d) H(%d %d %d) P(%d %d %d) F(%d %d)\n",
        DVS_TOTAL_MIN, VLEN_TOTAL_TYPI, DVS_TOTAL_MAX,
        DHS_TOTAL_MIN, HLEN_TOTAL_TYPI, DHS_TOTAL_MAX,
        DCLK_MIN, PANEL_GetPixelClk60Hz(), DCLK_MAX,
        DVS_FREQ_MIN, DVS_FREQ_MAX);

    if ((wOutputVTotal < DVS_TOTAL_MIN) ||
        (wOutputVTotal > DVS_TOTAL_MAX))
    {
        LOG(1, "DM FAIL V(%d)\n", wOutputVTotal);

        return SV_FAIL;
    }

    u4RefreshRate = (UINT32)wOutputFrameRate;

    if (u4RefreshRate == 0)
    {
        LOG(1, "DM FAIL Refresh(%d)\n", u4RefreshRate);

        return SV_FAIL;
    }

    u4HTotalMinByPixelClock = (((UINT32) DCLK_MIN) / u4RefreshRate) / (UINT32) wOutputVTotal;
    u4HTotalMinByPixelClock = ((u4HTotalMinByPixelClock + 1) & 0xfffffffe);     //make as even value
    u4HTotalMaxByPixelClock = (((UINT32) DCLK_MAX) / u4RefreshRate) / (UINT32) wOutputVTotal;
    u4HTotalMaxByPixelClock = ((u4HTotalMaxByPixelClock - 1) & 0xfffffffe);     //make as even value
    u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_BACKPORCH_MIN + CC_FRONTPORCH_MIN);
    u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);     //make as even value

#ifdef CC_HTOTAL_4X_LIMIT_WA
    if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
//    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        u4HTotalMinByPixelClock = (u4HTotalMinByPixelClock + 3) & (~3);
        u4HTotalMinBySyncMin = (u4HTotalMinBySyncMin + 3) & (~3);
        u4HTotalMaxByPixelClock = u4HTotalMaxByPixelClock & (~3);
    }
#endif

    LOG(1, "DM H MIN(%d %d) H MAX(%d) \n",
        u4HTotalMinByPixelClock,
        u4HTotalMinBySyncMin,
        u4HTotalMaxByPixelClock);

    // Find H-Total such that,
    // 1) Bigger than H-Total MIN calculate by DCLK_MIN
    // 2) Small than H-Total MAX calculate by DCLK_MAX
    // 3) Bigger than DHS_TOTAL_MIN
    // 4) Small than DHS_TOTAL_MAX
    // 5) Bigger than H-Total MIN calculate by CC_BACKPORCH_MIN
    // 6) Close to HLEN_TOTAL_TYPI

#ifdef CC_HTOTAL_4X_LIMIT_WA
    if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
//    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        u4HTotal = (UINT32) ((HLEN_TOTAL_TYPI) + 3) & (~3);
    }
    else
#endif
    {
        u4HTotal = (UINT32) (HLEN_TOTAL_TYPI);
    }

    if (u4HTotal > u4HTotalMaxByPixelClock)
    {
        u4HTotal = u4HTotalMaxByPixelClock;
    }

#ifdef CC_HTOTAL_4X_LIMIT_WA
    if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
//    if (LVDS_OUTPUT_PORT == DUAL_PORT)
    {
        if (u4HTotal > (UINT32) ((DHS_TOTAL_MAX + 1) & (~3)))
        {
            u4HTotal = (UINT32)((DHS_TOTAL_MAX + 1) & (~3));
        }
    }
    else
#endif
    {
        if (u4HTotal > (UINT32)(DHS_TOTAL_MAX + 1))
        {
            u4HTotal = (UINT32) (DHS_TOTAL_MAX + 1);
        }
    }

    if (u4HTotal < u4HTotalMinByPixelClock)
    {
        u4HTotal = u4HTotalMinByPixelClock;
    }

    if (u4HTotal < u4HTotalMinBySyncMin)
    {
        u4HTotal = u4HTotalMinBySyncMin;
    }

    if (u4HTotal < (UINT32) (DHS_TOTAL_MIN + 1))
    {
        u4HTotal = (UINT32) (DHS_TOTAL_MIN + 1);
    }

    if ((u4HTotal > u4HTotalMaxByPixelClock) ||
        (u4HTotal > (UINT32) DHS_TOTAL_MAX))
    {
        LOG(1, "DM FAIL V(%d) H(%d) F(%d)\n",
            wOutputVTotal,
            u4HTotal,
            u4RefreshRate);

        return SV_FAIL;
    }

    LOG(1, "DM OK V(%d) H(%d) F(%d)\n",
        wOutputVTotal,
        u4HTotal,
        u4RefreshRate);

    return SV_SUCCESS;

}

/**
 * @brief bDrvDispSupportFlag

 * When DISP_MODE_USE is DISP_MODE_ON, Check V_TOTAL and Pixel Clock Rate and Calculate H_Total.

 * @param  DV_TOTAL: Output Total Lines in the Vertical Direction
 * @retval SV_SUCCESS/SV_FAIL
 */
UINT8 bDrvDispSupportFlag(UINT16 DV_TOTAL)
{
    UINT16 u2RefreshRate;

    u2RefreshRate = (UINT16) bDrvVideoGetRefreshRate(SV_VP_MAIN);

    return bDrvDispSupport(DV_TOTAL, u2RefreshRate);
}
#endif //end of #ifndef  CC_MTK_LOADER


/**
 * @brief vDrvOutputStageInit

 * Init the Register Settings of Output Stage Logic

 * @param  void
 * @retval void
 */
extern UINT32 u4gbDitherMode;
extern BOOL fgIsDitherInited;
void vDrvVideoDitherInit(void)
{
    // DITHER
    UINT32 u4PanelDither = u4gbDitherMode;//PANEL_GetDither();

    UINT32 u4Round, u4RDither, u4EDither, u4LFSR, u4Subpxl, u4LsbOff;   

    if((u4gbDitherMode & 0x8000) != 0)
    {
        fgIsDitherInited = SV_TRUE;
    }
    
    // 1. OSTG Dither
    // 1.1 Set Dither Depth
    if (DISP_BIT == DISP_30BIT)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1OD_ON == SV_TRUE)
#else        
        if(0)
#endif
        {        
            // Front Dither
            vRegWriteFldAlign(DITHER_00, 1, DITHER_DRMOD);          // 12 to 10 Dither Mode
            vRegWriteFldAlign(DITHER_00, 2, DITHER_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vRegWriteFldAlign(DITHER_00, 2, DITHER_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)      
            // Post Dither
            vRegWriteFldAlign(DITHER_03, 1, DITHER_DRMOD_POST);     // 12 to 10 Dither Mode
            vRegWriteFldAlign(DITHER_03, 2, DITHER_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vRegWriteFldAlign(DITHER_03, 2, DITHER_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                        
        }
        else
        {
            // Front Dither
            vRegWriteFldAlign(DITHER_00, 1, DITHER_DRMOD);          // 12 to 10 Dither Mode
            vRegWriteFldAlign(DITHER_00, 2, DITHER_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vRegWriteFldAlign(DITHER_00, 2, DITHER_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)      

            // Post Dither (Off When OD Disabled)
            vRegWriteFldAlign(DITHER_03, 0, DITHER_DRMOD_POST);     // 12 to 10 Dither Mode
            vRegWriteFldAlign(DITHER_03, 0, DITHER_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0
            vRegWriteFldAlign(DITHER_03, 2, DITHER_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)     
        }        
    }
    else if (DISP_BIT == DISP_24BIT)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1OD_ON == SV_TRUE)
#else        
        if(0)
#endif
        {
            // Front Dither (12 to 10)
            vRegWriteFldAlign(DITHER_00, 1, DITHER_DRMOD);          // 12 to  8 Dither Mode
            vRegWriteFldAlign(DITHER_00, 2, DITHER_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vRegWriteFldAlign(DITHER_00, 2, DITHER_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)   

            // Post Dither (10 to 8)
            vRegWriteFldAlign(DITHER_03, 2, DITHER_DRMOD_POST);     // 12 to  8 Dither Mode
            vRegWriteFldAlign(DITHER_03, 1, DITHER_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vRegWriteFldAlign(DITHER_03, 1, DITHER_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                
                                                                    
        }
        else
        {
            // Front Dither (12 to 8) 
            vRegWriteFldAlign(DITHER_00, 2, DITHER_DRMOD);          // 12 to  8 Dither Mode
            vRegWriteFldAlign(DITHER_00, 1, DITHER_TABLE_EN);       // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vRegWriteFldAlign(DITHER_00, 1, DITHER_FPHASE_CTRL);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)   
            // Post Dither (Off When OD Disabled)
            vRegWriteFldAlign(DITHER_03, 0, DITHER_DRMOD_POST);     // 12 to  8 Dither Mode
            vRegWriteFldAlign(DITHER_03, 0, DITHER_TABLE_EN_POST);  // active bit of magic table for running order.
                                                                    //  01: phase active in bit3 ~ bit0 (12 to 8)
                                                                    //  10: phase active in bit1 ~ bit0 (12 to 10)
                                                                    //  else phase active in bit5 ~ bit0                                                                        
            vRegWriteFldAlign(DITHER_03, 1, DITHER_FPHASE_CTRL_POST);    // phase range select.
                                                                    //  00 : 0~63. (12 to 6)
                                                                    //  01 : 0~16. (12 to 8)
                                                                    //  10 : 0~3.  (12 to 10)                                                                
                                                             

        }
    }
    else
    {
        Printf("DISP_BIT=%d error\n", DISP_BIT);
    }

    // 1.2 Set Front Dither Type
    if ((u4PanelDither & 0x0F) != 0)
    {
        //u4LsbOff = (u4PanelDither & OSTG_LSBOFF_FUNCTION)? 1: 0; 
        //u4Round = (u4PanelDither & OSTG_ROUND_FUNCTION)? 1: 0;
        //u4RDither = (u4PanelDither & OSTG_RDITHER_FUNCTION)? 1 : 0;
        //u4EDither = (u4PanelDither & OSTG_ERROR_DITHER_FUNCTION)? 1 : 0;
        //u4LFSR = (u4PanelDither & OSTG_LFSR_FUNCTION)? 1 : 0;
        //u4Subpxl = (u4PanelDither & OSTG_SUB_PIXEL_FUNCTION)? 1 : 0;
        
        u4LsbOff = 1; 
        u4Round = (u4PanelDither & 0x01)? 1: 0;
        u4RDither = (u4PanelDither & 0x02)? 1 : 0;
        u4EDither = (u4PanelDither & 0x04)? 1 : 0;
        u4LFSR = (u4PanelDither & 0x08)? 1 : 0;
        u4Subpxl = 1;
    }
    else    // No Define
    {
        u4LsbOff = 1;
        u4Round = 1;
        u4RDither = 0;
        u4EDither = 0;
        u4LFSR = 0;        
        u4Subpxl = 1;        
    }

    vRegWriteFldAlign(DITHER_02, u4LsbOff, DITHER_LSB_OFF);
    vRegWriteFldAlign(DITHER_00, u4Round, DITHER_ROUND_EN);
    vRegWriteFldAlign(DITHER_00, u4RDither, DITHER_RDITHER_EN);        
    vRegWriteFldAlign(DITHER_00, u4EDither, DITHER_EDITHER_EN);
    vRegWriteFldAlign(DITHER_00, u4LFSR, DITHER_LFSR_EN);      
    vRegWriteFldAlign(DITHER_00, u4Subpxl, DITHER_SUBPIX_EN);    

    // 1.3 Common Dither Setting
    vRegWriteFldAlign(DITHER_00, 0, DITHER_SUB_R);
    vRegWriteFldAlign(DITHER_00, 1, DITHER_SUB_G);        
    vRegWriteFldAlign(DITHER_00, 2, DITHER_SUB_B);
    
    vRegWriteFldAlign(DITHER_00, SV_TRUE, DITHER_FPHASE_EN);    
    vRegWriteFldAlign(DITHER_00, 0, DITHER_FPHASE_R);           // phase select for running order.
                                                                //  0: internal control register from reg_fphase[5:0]
                                                                //  1: LFSR

    vRegWriteFldAlign(DITHER_00, 3, DITHER_FPHASE);             // Running order dithering frame phase increment.
                                                                // 1. If ROUND_EN = 1, the output will only rounding only 
                                                                // (no matter what RDITHER_EN and EDITHER_EN setting)
                                                                // 2. RDITHER_EN and EDITHER_EN can work together.
    vRegWriteFldAlign(DITHER_00, 1, DITHER_FPHASE_SEL);         // fphase XOR function bit. Only bit2,bit4,bit6 is available.                                                                
    vRegWriteFldAlign(DITHER_00, 1, DITHER_FPHASE_BIT);         // fphase_bit XOR bus number. 
                                                                // Note: this function will check reg_fphase_sel.
                                                                // Only 0,1,2,4,6 is available."                     

    // 2. OD Dither
    // 2.1 Set Dither Type
    if (u4PanelDither != 0xffffffff)
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1OD_ON == SV_TRUE)
#else        
        if(0)
#endif
        {        
            u4LsbOff = (u4PanelDither & TCON_LSBOFF_FUNCTION)? 1: 0; 
            u4Round = (u4PanelDither & TCON_ROUND_FUNCTION)? 1: 0;
            u4RDither = (u4PanelDither & TCON_RDITHER_FUNCTION)? 1 : 0;
            u4EDither = (u4PanelDither & TCON_ERROR_DITHER_FUNCTION)? 1 : 0;
            u4LFSR = (u4PanelDither & TCON_LFSR_FUNCTION)? 1 : 0;
            u4Subpxl = (u4PanelDither & TCON_SUB_PIXEL_FUNCTION)? 1 : 0;
        }
        else
        {
            u4LsbOff = 0;
            u4Round = 0;
            u4RDither = 0;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 0;  
        }
    }
    else    // No Define
    {
#ifndef CC_MTK_LOADER                
        if (_ODParam.u1OD_ON == SV_TRUE)
#else        
        if(0)
#endif
        {        
            u4LsbOff = 1;
            u4Round = 0;
            u4RDither = 1;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 1;        
        }
        else
        {
            u4LsbOff = 0;
            u4Round = 0;
            u4RDither = 0;
            u4EDither = 0;
            u4LFSR = 0;        
            u4Subpxl = 0;  
        }
    }

    vRegWriteFldAlign(DITHER_04, u4LsbOff, DITHER_LSB_OFF_POST);
    vRegWriteFldAlign(DITHER_03, u4Round, DITHER_ROUND_EN_POST);
    vRegWriteFldAlign(DITHER_03, u4RDither, DITHER_RDITHER_EN_POST);        
    vRegWriteFldAlign(DITHER_03, u4EDither, DITHER_EDITHER_EN_POST);
    vRegWriteFldAlign(DITHER_03, u4LFSR, DITHER_LFSR_EN_POST);      
    vRegWriteFldAlign(DITHER_03, u4Subpxl, DITHER_SUBPIX_EN_POST);    

    // 2.2 Common Dither Setting
    vRegWriteFldAlign(DITHER_03, 0, DITHER_SUB_R_POST);
    vRegWriteFldAlign(DITHER_03, 1, DITHER_SUB_G_POST);        
    vRegWriteFldAlign(DITHER_03, 2, DITHER_SUB_B_POST);
    
    vRegWriteFldAlign(DITHER_03, SV_TRUE, DITHER_FPHASE_EN_POST);    
    vRegWriteFldAlign(DITHER_03, 0, DITHER_FPHASE_R_POST);      // phase select for running order.
                                                                //  0: internal control register from reg_fphase[5:0]
                                                                //  1: LFSR

    vRegWriteFldAlign(DITHER_03, 3, DITHER_FPHASE_POST);        // Running order dithering frame phase increment.
                                                                // 1. If ROUND_EN = 1, the output will only rounding only 
                                                                // (no matter what RDITHER_EN and EDITHER_EN setting)
                                                                // 2. RDITHER_EN and EDITHER_EN can work together.
    vRegWriteFldAlign(DITHER_03, 1, DITHER_FPHASE_SEL_POST);    // fphase XOR function bit. Only bit2,bit4,bit6 is available.                                                                
    vRegWriteFldAlign(DITHER_03, 1, DITHER_FPHASE_BIT_POST);    // fphase_bit XOR bus number. 
                                                                // Note: this function will check reg_fphase_sel.
                                                                // Only 0,1,2,4,6 is available."    


#ifdef __MODEL_slt__    
    vRegWriteFldAlign(DITHER_00, 0, DITHER_DRMOD);
#endif
}


void vDrvOutputStageInit(void)
{

    #ifdef CC_SCPOS_3DTV_SUPPORT
	UINT32 u4EnPair;
    #endif

    #if (SRGB_FUNCTION_EN == SRGB_EN_ON)
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_DEN_EN);
    }
    #else
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_DEN_EN);
    }
    #endif

    #if (DEN_TYPE == DEN_ACIVE)
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_DENTPY);
    }
    #else
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_DENTPY);
    }
    #endif

    #if (DEN_POLARITY == DEN_LOW)
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_DENP);
    }
    #else
    {
        vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_DENP);
    }
    #endif

    vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_HSYNCP);
    vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_VSYNCP);
    vIO32WriteFldAlign(MLVDS_P0, 0, RG_HSYNC_IN_P);                
    vIO32WriteFldAlign(MLVDS_P0, 0, RG_VSYNC_IN_P);                    

//    vDrvVideoDitherInit();

    // Config HV alignment
    //vRegWriteFldAlign(OUTSTG_OS_08, 3, OS_08_REG_VSYNC_OUT_SEL);

    if ((DISP_BIT == DISP_30BIT)
        #ifdef DRV_SUPPORT_EXTMJC
        || (IS_COMPANION_CHIP_ON() && (vDrvGetCurLvdsMode() !=  eLvdsVidNormal))
        #endif
        )
    {
        vIO32WriteFldAlign(MLVDS_SO0, 1, MLVDS_SO0_RG_10B_EN);                
    }
    else
    {
        vIO32WriteFldAlign(MLVDS_SO0, 0, MLVDS_SO0_RG_10B_EN);                
    }

#ifdef CC_SCPOS_3DTV_SUPPORT
		if (DRVCUST_PanelQuery(eDISP_RLVDSEncodeLink, &u4EnPair) == 0)
		{
		  u1SetDISP_REncodeEn((UINT8)u4EnPair);
		}	 
		else
		{
		 vIO32WriteFldAlign(MLVDS_SO0, 0x1, MLVDS_SO0_RG_RES_FLD);
		} 
		vRegWriteFldAlign(OSTG_01, 0x1, REG_3DTV_OUT_SEL);
#endif


    // scaler timing initial
	if (fgIsMJCToOSTG())
    vIO32WriteFldAlign(PSPIP_OA_07, 1, PSPIP_OA_07_SCALER_SET_DATA_POS_SEL);
    else
    vRegWriteFldAlign(SCPIP_PIP_OA_07, 1 ,PIP_OA_07_SCALER_SET_DATA_POS_SEL);			

    // local dimming initialization
    #ifdef CC_MTK_LOADER    
    #else
    if (fgIsSWRegInit()!=SV_FALSE)
    {
        if(PANEL_IsSupportLocalDimming())
        {
            if(bDrvLcdimSpiInit() == SV_FAIL)
            {
                bSupportLocalDimming = SV_FALSE;
            }        
            else    // PQ setting is only initialized when SPI is ready
            {
                if(bDrvLcDimPanelInit() == SV_SUCCESS)
                {
                    vDrvLcDinQualityInit();            
                    bSupportLocalDimming = SV_TRUE;
                }
                else
                {
                    bSupportLocalDimming = SV_FALSE;
                }
            }
        }    
        else
        {
            bSupportLocalDimming = SV_FALSE;
        }
    }
    #endif

    vIO32WriteFldAlign(TMGR5, 1, TMGR5_fifo_en);
    switch(PANEL_GetDisplayType())
    {
        default:
        case PANEL_DISPLAY_TYPE_MAX:
            // error display type, defaultly do nothing
            break;
        case PANEL_DISPLAY_TYPE_LVDS:
            vIO32WriteFldAlign(TMGR5, 3, TMGR5_fifo_ctrl);
            vIO32WriteFldAlign(MLVDS_B0, 0, RG_FIFO_SEL);			
            break;
        case PANEL_DISPLAY_TYPE_MLVDS:
            vIO32WriteFldAlign(TMGR5, 4, TMGR5_fifo_ctrl);			
            vIO32WriteFldAlign(MLVDS_B0, 1, RG_FIFO_SEL);						
            break;
        #ifdef CC_MT5396
            case PANEL_DISPLAY_TYPE_VB1:
                vIO32WriteFldAlign(TMGR5, 0, TMGR5_fifo_ctrl);			
                vIO32WriteFldAlign(MLVDS_B0, 2, RG_FIFO_SEL);									
                break;
            case PANEL_DISPLAY_TYPE_EPI:
                vIO32WriteFldAlign(TMGR5, 2, TMGR5_fifo_ctrl);						
                vIO32WriteFldAlign(MLVDS_B0, 4, RG_FIFO_SEL);												
                break;
        #endif
    }

#ifndef CC_MTK_LOADER
#ifdef CC_SCPOS_3DTV_SUPPORT 
    vSavePanelAttribute();
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_OSTG, (vTDTVModeChangeCB) vDrvDisp3DModeCB);
#endif
#endif

    vRegWriteFldAlign(OSTG_01, 0, REG_MAIN_TOP_EN); // set sub plane mixer order above main
}

/**
 *  @6896 new
 */
static void _vScpipInitPanelProtect(void)
{
    //init H panel protect (disable H protection)
    vRegWriteFldAlign(SCPIP_PIP_OA_23, 1, PIP_OA_23_PANEL_HTOTAL_PROTECT_EN);
    vRegWriteFldAlign(SCPIP_PIP_OA_23, 0x7FFF, PIP_OA_23_PANEL_HTOTAL_PROTECT);    
    //init H panel protect
    vRegWriteFldAlign(SCPIP_PIP_OA_23, 1, PIP_OA_23_PANEL_VTOTAL_PROTECT_EN);
    vRegWriteFldAlign(SCPIP_PIP_OA_23, 0x7FFF, PIP_OA_23_PANEL_VTOTAL_PROTECT);    
    
    #if defined(SCPIP_SUPPORT_POST_SCALER)
    //init H panel protect (disable H protection)
    vIO32WriteFldAlign(PSPIP_OA_23, 1, PSPIP_OA_23_PANEL_HTOTAL_PROTECT_EN);
    vIO32WriteFldAlign(PSPIP_OA_23, 0x7FFF, PSPIP_OA_23_PANEL_HTOTAL_PROTECT);    
    //init H panel protect
    vIO32WriteFldAlign(PSPIP_OA_23, 1, PSPIP_OA_23_PANEL_VTOTAL_PROTECT_EN);
    vIO32WriteFldAlign(PSPIP_OA_23, 0x7FFF, PSPIP_OA_23_PANEL_VTOTAL_PROTECT);
    #endif
}

/**
 * @brief vDrvDisplayInit

 * Init the Display Settings

 * @param  void
 * @retval void
 */
void vDrvDisplayInit(void)
{

#ifndef CC_MTK_LOADER    
#if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscPresetDisplayWidth();
#endif
#endif

    //set panel protect parameter
    _vScpipInitPanelProtect();
    
// vpll init
    #ifndef CC_MTK_LOADER
        #ifdef NEW_COUNTRY_TYPE
        if(GetTargetCountryMask(COUNTRY_ATV_MASK)==COUNTRY_ATV_EU)
        #else
            #ifdef COUNTRY_FROM_EEPROM
            u1TargetCountryInfo = (GetTargetCountry() & COUNTRY_SCAN_ATV_MASK) >> SCAN_ATV_POS;;
            if (u1TargetCountryInfo == COUNTRY_EU)
            #else
                if(GetTargetCountry()==COUNTRY_EU)
            #endif
        #endif
    #else
    if (0)
    #endif
    {
        vDDDSInit();    
	
        if (PANEL_IsSupport120Hz())
        {
            vDrvLCDFreqSet(SV_DCLK_50HZ, 100);
        }
        else
        {
            vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
        }
    }
    else
    {
        // if panel already on, don't set again.
        if(!bIsPanelOn())
        {
         vDDDSInit();
        if (PANEL_IsSupport240Hz())
        {
            vDrvLCDFreqSet(SV_DCLK_60HZ, 240);
        }
        else if (PANEL_IsSupport120Hz())
            {
                vDrvLCDFreqSet(SV_DCLK_60HZ, 120);
            }
            else
            {
                vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
            }
        }
        else
        {
            bSI_DISPLAY_DCLK_TYPE = SV_DCLK_60HZ;
        if (PANEL_IsSupport240Hz())
        {
            bOUTPUT_REFRESH_RATE = 240;
        }
        else if (PANEL_IsSupport120Hz())
            {
                bOUTPUT_REFRESH_RATE = 120;
            }
            else
            {
                bOUTPUT_REFRESH_RATE = 60;
            }
            _u4VPllFrequency = PANEL_GetPixelClk60Hz();//for dynamic MDR
            #ifndef CC_MTK_LOADER
            vDrvSetSSMDRAsInputHsync();//for dynamic MDR
            #ifdef CC_MT5396
            bDrvUpdateInternalFrameRate();
            #endif
            #endif
            vDrvSwitchImportProtection(0);
            #ifndef CC_MTK_LOADER
            #ifdef SUPPORT_DDDS_STEP_TRACKING
            vDrvUpdateDisplayCenterCW(IO32ReadFldAlign(DDDS_00,DDDS_00_DDDS_FREQ_CW));
            #endif
            #endif
    
        }
    }
    

    vDrvOutputStageInit();
    vDrvInitSCANPWM();

    //SET main/pip mute works immediately
    vRegWriteFldAlign(MUTE_00, 1 ,R_MUTE_POST_SEL);
    vRegWriteFldAlign(MUTE_00, 1 ,R_P_MUTE_FRONT_SEL);
    vRegWriteFldAlign(MUTE_00, 1 ,R_M_MUTE_FRONT_SEL);       
    // disable video plane
    vRegWriteFldAlign(SCPIP_PIP1_02, 0, PIP1_02_RVL_1);
    vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
}

void vDrvLCDSetFrame(UINT16 wFrameRate)
{
    if (wFrameRate==50)
    {
        vDrvLCDFreqSet(SV_DCLK_50HZ, 50);
    }
    else if (wFrameRate==60)
    {
        vDrvLCDFreqSet(SV_DCLK_60HZ, 60);
    }
    else if (wFrameRate==100)
    {
        vDrvLCDFreqSet(SV_DCLK_50HZ, 100);
    }
    else if (wFrameRate==120)
    {
        vDrvLCDFreqSet(SV_DCLK_60HZ, 120);
    }
    #ifdef SUPPORT_PANEL_48HZ
    else if (wFrameRate == 48)
    {
        vDrvLCDFreqSet(SV_DCLK_48HZ, wFrameRate);
    }
    #endif	
    else
    {
        vDrvLCDFreqSet(SV_DCLK_OTHERS, (UINT8)wFrameRate);
    }
}


#ifndef CC_MTK_LOADER
/**
 * @brief vDrvUpdateInternalFrameRate

 * update internal frame rate between up scalar and MJC

 * @param  void
 * @retval update or not
 */
UINT8 bDrvUpdateInternalFrameRate(void)
{
    #ifdef CC_MT5396
        UINT32 u4PCLK_Sel;

        if (fgIsMJCToOSTG())
        {
            u4PCLK_Sel = 2; // poclk_dpll
        }
        else
        {
            u4PCLK_Sel = 0; // oclk
        }
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG, u4PCLK_Sel, FLD_POCLK_SEL);                                   
    #endif

    return SV_TRUE;
}
#endif // CC_MTK_LOADER

//get internal frame rate between Front scaler and MJC
UINT16 u2DrvGetInternalFrameRate(void)
{
    #ifdef CC_MT5396
    return _u2IntFrameRate;
    #else
    return (UINT16)bOUTPUT_REFRESH_RATE;
    #endif
}

//set internal frame rate between Front scaler and MJC
void vDrvSetInternalFrameRate(UINT16 frameRate)
{
    #if defined(CC_MT5396)
    _u2IntFrameRate = frameRate;
    #else
    //no-nothing
    #endif
}

/**
 * @brief vDrvLCDFreqSet

 * wDCLK Setting, MTK Good Dclk ON/OFF and the Display Output Timing for LCD panel

 * @param  bDCLKType: SV_DCLK_50HZ/SV_DCLK_60HZ
 * @param  bFrameRate: frame rate
 * @retval void
 */
void vDrvLCDFreqSet(UINT8 bDCLKType, UINT8 bFrameRate)    // called by Video
{
    static BOOL fgInit = FALSE;
    UINT32 dwDCLK = PANEL_GetPixelClk60Hz();
    UINT32 u4Ratio;

    LOG(3, "vDrvLCDFreqSet(bDCLKType=%d,bFrameRate=%d)\n",bDCLKType,bFrameRate);
    LOG(3, "vDrvLCDFreqSet(bSI_DISPLAY_DCLK_TYPE=%d,bOUTPUT_REFRESH_RATE=%d)\n",bSI_DISPLAY_DCLK_TYPE,bOUTPUT_REFRESH_RATE);

    if ((bDCLKType == bSI_DISPLAY_DCLK_TYPE)
        #ifndef CC_MTK_LOADER		
        && ((bOUTPUT_REFRESH_RATE == bFrameRate)) 
        // for 3D case, there might be FHD60 <-> 720p120 switching with same DCLKType
        //              or 24/48/60 with DCLK_60HZ type. 
        // || (bDCLKType != SV_DCLK_OTHERS))
        #endif        
        )
    {
        #ifndef CC_MTK_LOADER		
        bDrvUpdateInternalFrameRate();
        vDrvSetSSMDRAsInputHsync();//for dynamic MDR
        #endif        
        return;
    }

    // Update Current DCLK Type
    bSI_DISPLAY_DCLK_TYPE = bDCLKType;
    if (PANEL_IsSupport240Hz())
    {
        u4Ratio = 4;
    }
    else if (PANEL_IsSupport120Hz())
    {
        u4Ratio = 2;
    }
    else
    {
        u4Ratio = 1;
    }

    //  wDCLK  Setting
    switch(bDCLKType)
    {
    case SV_DCLK_50HZ:
		if ((bFrameRate == 25) && (DRVCUST_PanelGet(ePanel242530pOutput) == 1))
		{
		 dwDCLK = (PANEL_GetPixelClk50Hz() /(50*u4Ratio)) * bFrameRate;
		 bOUTPUT_REFRESH_RATE = (UINT8)(bFrameRate * u4Ratio);
		}
		else
		{
         dwDCLK = PANEL_GetPixelClk50Hz();
         bOUTPUT_REFRESH_RATE = (UINT8)(50 * u4Ratio);
		}
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_50HZ;
        #ifndef CC_MTK_LOADER
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_50HZ);
        #endif
        #endif        
        break;

    case SV_DCLK_60HZ:
		if ((bFrameRate == 30) && (DRVCUST_PanelGet(ePanel242530pOutput) == 1))
		{
		 dwDCLK = (PANEL_GetPixelClk60Hz() /(60*u4Ratio)) * bFrameRate;
		 bOUTPUT_REFRESH_RATE = (UINT8)(bFrameRate * u4Ratio);
		}
		else
		{
         dwDCLK = PANEL_GetPixelClk60Hz();
         bOUTPUT_REFRESH_RATE = (UINT8)(60 * u4Ratio);
		}
        bSI_DISPLAY_DCLK_TYPE= SV_DCLK_60HZ;
        #ifndef CC_MTK_LOADER
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_60HZ);
        #endif
        #endif        
        break;
#ifndef CC_MTK_LOADER		
    #ifdef SUPPORT_PANEL_48HZ
    case SV_DCLK_48HZ:    
        if(PANEL_IsSupportExtAttribute()!=0)
        {		
		if ((bFrameRate == 24) && (DRVCUST_PanelGet(ePanel242530pOutput) == 1))
		{
		 dwDCLK = (PANEL_GetPixelClk48Hz() /(48*u4Ratio)) * bFrameRate;
		 bOUTPUT_REFRESH_RATE = (UINT8)(bFrameRate * u4Ratio);
		}
		else
		{
         dwDCLK = PANEL_GetPixelClk48Hz();
         bOUTPUT_REFRESH_RATE = (UINT8)(48 * u4Ratio);
		}
        }
        else
        {
            /* panel table lacks of the 48hz setting, keep 60hz refresh rate */
            dwDCLK = PANEL_GetPixelClk60Hz();
            bOUTPUT_REFRESH_RATE = (UINT8)(60 * u4Ratio);
        }
        bSI_DISPLAY_DCLK_TYPE= SV_DCLK_48HZ;		
        #ifdef CC_DRV_SUPPORT_SMICRO
        vSMicroPreSetFreq(FREQ_48HZ);
        #endif
        break;
    #endif  // SUPPORT_PANEL_48HZ
    case SV_DCLK_OTHERS:
		// SUPPORT Non-Standard signal
		if ((bFrameRate<SUPPORT_IN_LOW_FRAME_RATE) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) 
		bOUTPUT_REFRESH_RATE = bFrameRate *2;
		else 	
        bOUTPUT_REFRESH_RATE = bFrameRate;

		#ifdef SUPPORT_PANEL_48HZ
        if (bOUTPUT_REFRESH_RATE < (UINT8)(24 * u4Ratio))
        {   
            if(DRVCUST_PanelGet(ePanel242530pOutput) == 1)
            dwDCLK = (PANEL_GetPixelClk48Hz() / 2 /(24*u4Ratio)) * bOUTPUT_REFRESH_RATE;			
			else
			dwDCLK = (PANEL_GetPixelClk48Hz() /(24*u4Ratio)) * bOUTPUT_REFRESH_RATE;
			
		#ifndef CC_MTK_LOADER
		#ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_48HZ);
		#endif
		#endif		  
        }
		else			
		#endif	
        if (bOUTPUT_REFRESH_RATE < (UINT8)(SUPPORT_IN_LOW_FRAME_RATE * u4Ratio))
        {
            if(DRVCUST_PanelGet(ePanel242530pOutput) == 1)
			dwDCLK = (PANEL_GetPixelClk60Hz() /2 /(30*u4Ratio)) * bOUTPUT_REFRESH_RATE;
			else
            dwDCLK = (PANEL_GetPixelClk60Hz() /(30*u4Ratio)) * bOUTPUT_REFRESH_RATE;
			
			#ifndef CC_MTK_LOADER
			#ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_60HZ);
			#endif
			#endif		  
        }
		else							
        if (bOUTPUT_REFRESH_RATE < (UINT8)(55 * u4Ratio))
        {
            dwDCLK = (PANEL_GetPixelClk50Hz() /(50*u4Ratio)) * bOUTPUT_REFRESH_RATE;
            #ifndef CC_MTK_LOADER
            #ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_50HZ);
            #endif
            #endif        
        }
        else
        {
            dwDCLK = (PANEL_GetPixelClk60Hz() /(60*u4Ratio)) * bOUTPUT_REFRESH_RATE;
            #ifndef CC_MTK_LOADER
            #ifdef CC_DRV_SUPPORT_SMICRO
            vSMicroPreSetFreq(FREQ_60HZ);
            #endif
            #endif        
        }
        bSI_DISPLAY_DCLK_TYPE= SV_DCLK_OTHERS;
        break;
#endif // end of #ifndef CC_MTK_LOADER		
    default:
        break;
    }

    LOG(3, "bSI_DISPLAY_DCLK_TYPEe=%d\n",bSI_DISPLAY_DCLK_TYPE);
    LOG(3, "bOUTPUT_REFRESH_RATE=%d\n",bOUTPUT_REFRESH_RATE);    

    if (fgInit)
    {
        BeforeOutputFrameRateChange(bOUTPUT_REFRESH_RATE);
    }

    vDrvSwitchMTKGoodDclk(SV_OFF);  //turn off DDDS 8202 (Dragon)

#ifndef CC_COPLAT_MT82
#ifndef __MODEL_slt__
        #ifdef WO_OSD_MUTE_VTOTAL_CHG
		if ((PANEL_GetPixelClk50Hz() != PANEL_GetPixelClk60Hz())
			|| (PANEL_GetHTotal50Hz() != PANEL_GetHTotal60Hz())
		#ifdef SUPPORT_PANEL_48HZ
			|| (PANEL_GetPixelClk48Hz() != PANEL_GetHTotal60Hz())
			|| (PANEL_GetHTotal48Hz() != PANEL_GetHTotal60Hz())
		#endif
		   )
		#endif  
    OSD_PLA_Mute();
#endif
#endif

    vDrvSetLCDTiming();

#ifndef CC_MTK_LOADER
#ifdef CC_SCPOS_3DTV_SUPPORT 
    if( u4DrvGetDisp3DModeDclk() != 0)
    {
        dwDCLK = u4DrvGetDisp3DModeDclk();
        bOUTPUT_REFRESH_RATE = u1DrvGetDisp3DModeRefreshRate();
    }
#endif
#endif

#ifdef CC_MTK_LOADER
    vDrvVOPLLFreqSet(dwDCLK);
#else
    vDrvVOFreqSet(dwDCLK);
    vDrvSetSSMDRAsInputHsync();//for dynamic MDR
#endif

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
#if !defined(SUPPORT_CW_CHANGE_BY_STEP) && !defined(CC_MTK_LOADER)
    // store the original control word
    i4OrigControlWord = IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW);
#endif
#endif

    if (_fgFRS_ENA == SV_ON)
    {
        vDrvSwitchMTKGoodDclk(SV_ON);
    }

    if (fgInit)
    {
        AfterOutputFrameRateChange();
    }

    #ifndef CC_MTK_LOADER
    #ifdef CC_DRV_SUPPORT_SMICRO
    {
        SMICRO_VERTICAL_FREQ eFreq;
        SMICRO_MSG rMsg;
        #ifdef SUPPORT_PANEL_48HZ
        if (bOUTPUT_REFRESH_RATE == (UINT8)(48 * u4Ratio))
        {
            eFreq = FREQ_48HZ;
        }
        else 
        #endif    
        if (bOUTPUT_REFRESH_RATE < (UINT8)(55 * u4Ratio))
        {
            eFreq = FREQ_50HZ;
        }
        else
        {
            eFreq = FREQ_60HZ;
        }
        rMsg.u4ModuleId = MODULEID_PANEL_PARAM;
        rMsg.u4CommandId = CMD17_SET_VERTICAL_FREQ;
        rMsg.aPrm[0] = (UINT8)eFreq;
        rMsg.pCallbackFcn = NULL;
        vSMicroSendEvent(rMsg);  
    }
    #endif
    #endif        

    if (!fgInit)
    {
        fgInit = TRUE;
    }
}

/**
 * @brief wDrvCalOutputVTotal
 *
 * When DISP_MODE_USE is DISP_MODE_ON, Get the recommended V_TOTAL with additional VTotal limitation.
 *
 * @param  u1OutFrameRate: Output frame rate
 * @param  u2OutVTotalMin: Minimum Output Total Lines in the Vertical Direction
 * @param  u2OutVTotalMax: Maximum Output Total Lines in the Vertical Direction
 * @retval recommended V_TOTAL
 */
UINT16 wDrvCalOutputVTotal(UINT8 u1OutFrameRate, UINT16 u2OutVTotalMin, UINT16 u2OutVTotalMax)
{
    UINT32 u4VTotalMinByPixelClock;
    UINT32 u4VTotalMaxByPixelClock;
    UINT32 u4FrameRate;
    UINT32 u4HTotalMinBySyncMin;
    UINT32 u4HTotalMaxBySyncMax;

    if ((u2OutVTotalMin < DVS_TOTAL_MIN) && (u2OutVTotalMin != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMin = DVS_TOTAL_MIN;
    }

    if ((u2OutVTotalMax > DVS_TOTAL_MAX) && (u2OutVTotalMax != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMax = DVS_TOTAL_MAX;
    }

    u4FrameRate = (UINT32)u1OutFrameRate;

    u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_BACKPORCH_MIN + CC_FRONTPORCH_MIN);
    u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);     //make as even value
    u4HTotalMaxBySyncMax = (UINT32) (wDISPLAY_WIDTH + CC_BACKPORCH_MAX + CC_FRONTPORCH_MAX);
    u4HTotalMaxBySyncMax = ((u4HTotalMaxBySyncMax + 1) & 0xfffffffe);           //make as even value       

    if (u4HTotalMinBySyncMin < DHS_TOTAL_MIN)
    {
        u4HTotalMinBySyncMin = DHS_TOTAL_MIN;
    }
    if (u4HTotalMaxBySyncMax > DHS_TOTAL_MAX)
    {
        u4HTotalMaxBySyncMax = DHS_TOTAL_MAX;
    }    

    if (u4FrameRate ==0)
    {
        return  (0);
    }
    
    u4VTotalMinByPixelClock = ((UINT32) DCLK_MIN / u4FrameRate) / u4HTotalMaxBySyncMax;
    u4VTotalMaxByPixelClock = ((UINT32) DCLK_MAX / u4FrameRate) / u4HTotalMinBySyncMin;

    if ((u2OutVTotalMin < u4VTotalMinByPixelClock) && (u2OutVTotalMin != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMin = u4VTotalMinByPixelClock;
    }

    if ((u2OutVTotalMax > u4VTotalMaxByPixelClock) && (u2OutVTotalMax != SCALE_VTOTAL_DO_NOT_CARE))
    {
        u2OutVTotalMax = u4VTotalMaxByPixelClock;
    }

    if (u2OutVTotalMin == SCALE_VTOTAL_DO_NOT_CARE)
    {
        u2OutVTotalMin = DVS_TOTAL_MIN;
    }   
    if (u2OutVTotalMax == SCALE_VTOTAL_DO_NOT_CARE)
    {
        u2OutVTotalMax = DVS_TOTAL_MAX;
    }

    if (u2OutVTotalMin < u2OutVTotalMax)
    {
        u2OutVTotalMin = ((u2OutVTotalMax+u2OutVTotalMin)>>1);
    }
    else
    {
        u2OutVTotalMin = 0;
    }

    return u2OutVTotalMin;
}


UINT8 vDrvGetLCDFreq(void)   
{
	return bOUTPUT_REFRESH_RATE;
}

/**
 * @brief vDrvSetDynamicDisplay

 * When DISP_MODE_USE is DISP_MODE_ON:
 * According to V_TOTAL That the Scaler Driver Calculates and HLEN_TOTAL_TYPI, Calculate Output Clock.
   If BiMode is SV_SUCCESS, Set Video Output Clock and the New Display Output Timing.

 * @param  BiMode: SV_SUCCESS/SV_FAIL
 * @retval void
 */
 #ifndef CC_MTK_LOADER
void  vDrvSetDynamicDisplay(UINT8 BiMode)
{
        static BOOL fgPreviousMode = FALSE;


    if (fgPreviousMode == BiMode)
    {
        if (BiMode)
        {
        }
        else
        {
            return;
        }
    }    

    if( _bIsImportProtection )
    {
        vDrvSwitchImportProtection(1);
    }

    if (BiMode)
    {
        UINT32 u4PixelClock;
        UINT32 u4HSyncLen;
        UINT32 u4HTotalFinal;
        UINT32 u4VTotalFinal;

        UINT32 u4RefreshRate;
        UINT32 u4VTotal;
        UINT32 u4HTotal;

        UINT32 u4HTotalMinBySyncMin;
        UINT32 u4HTotalMinByPixelClock;
        UINT32 u4HTotalMaxByPixelClock;
        UINT32 wOldHTotal, wOldVTotal;

        u4RefreshRate = (UINT16) bDrvVideoGetRefreshRate(SV_VP_MAIN);
        u4VTotal = (UINT32) u2ScpipCalculateOutputVTotal();

        if ((u4VTotal < DVS_TOTAL_MIN) ||
            (u4VTotal > DVS_TOTAL_MAX) ||
            (u4RefreshRate == 0))
        {

            if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
            {
                LVDS_SwitchDataEnable(TRUE);
            }


            if( _bIsImportProtection )
            {
                vDrvSwitchImportProtection(0);
            }
            return;
        }
        
        u4HTotalMinByPixelClock = (((UINT32) DCLK_MIN) / u4RefreshRate) / u4VTotal;
        u4HTotalMinByPixelClock = ((u4HTotalMinByPixelClock + 1) & 0xfffffffe);     //make as even value
        u4HTotalMaxByPixelClock = (((UINT32) DCLK_MAX) / u4RefreshRate) / u4VTotal;
        u4HTotalMaxByPixelClock = ((u4HTotalMaxByPixelClock - 1) & 0xfffffffe);     //make as even value
        u4HTotalMinBySyncMin = (UINT32) (wDISPLAY_WIDTH + CC_BACKPORCH_MIN + CC_FRONTPORCH_MIN);
        u4HTotalMinBySyncMin = ((u4HTotalMinBySyncMin + 1) & 0xfffffffe);           //make as even value

#ifdef CC_HTOTAL_4X_LIMIT_WA
        if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
        {
            u4HTotalMinByPixelClock = (u4HTotalMinByPixelClock + 3) & (~3);
            u4HTotalMinBySyncMin = (u4HTotalMinBySyncMin + 3) & (~3);
            u4HTotalMaxByPixelClock = u4HTotalMaxByPixelClock & (~3);
        }
#endif

        // Find H-Total such that,
        // 1) Bigger than H-Total MIN calculate by DCLK_MIN
        // 2) Small than H-Total MAX calculate by DCLK_MAX
        // 3) Bigger than DHS_TOTAL_MIN
        // 4) Small than DHS_TOTAL_MAX
        // 5) Bigger than H-Total MIN calculate by CC_BACKPORCH_MIN
        // 6) Close to HLEN_TOTAL_TYPI

#ifdef CC_HTOTAL_4X_LIMIT_WA
        if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
//        if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
            u4HTotal = (UINT32) ((HLEN_TOTAL_TYPI) + 3) & (~3);
        }
        else
#endif
        {
            u4HTotal = (UINT32) (HLEN_TOTAL_TYPI);
        }

        if (u4HTotal > u4HTotalMaxByPixelClock)
        {
            u4HTotal = u4HTotalMaxByPixelClock;
        }

#ifdef CC_HTOTAL_4X_LIMIT_WA
        if ((LVDS_OUTPUT_PORT == DUAL_PORT) || (LVDS_OUTPUT_PORT == FOUR_PORT))
//        if (LVDS_OUTPUT_PORT == DUAL_PORT)
        {
            if (u4HTotal > (UINT32) ((DHS_TOTAL_MAX ) & (~3)))
            {
                u4HTotal = (UINT32)((DHS_TOTAL_MAX ) & (~3));
            }
        }
        else
#endif
        {
            if (u4HTotal > (UINT32)(DHS_TOTAL_MAX ))
            {
                u4HTotal = (UINT32) (DHS_TOTAL_MAX );
            }
        }

        if (u4HTotal < u4HTotalMinByPixelClock)
        {
            u4HTotal = u4HTotalMinByPixelClock;
        }

        if (u4HTotal < u4HTotalMinBySyncMin)
        {
            u4HTotal = u4HTotalMinBySyncMin;
        }

        if (u4HTotal < (UINT32) (DHS_TOTAL_MIN ))
        {
            u4HTotal = (UINT32) (DHS_TOTAL_MIN );
        }

        // Pixel Clock
        u4PixelClock = (u4RefreshRate * u4VTotal) * u4HTotal;

        // [DTV00111681] Update related register and Do OSD_PLA_Mute only when u4VTotal or u4HTotal changed.
        // Peggy 20080109
        wOldHTotal = wDrvGetOutputHTotal();
        wOldVTotal = wDrvGetOutputVTotal(); 
        	
        if ((wOldHTotal != u4HTotal) || (wOldVTotal != u4VTotal))
        {        	
            #ifndef CC_COPLAT_MT82
            #ifndef __MODEL_slt__
			#ifdef WO_OSD_MUTE_VTOTAL_CHG
				if ((PANEL_GetPixelClk50Hz() != PANEL_GetPixelClk60Hz())
					|| (PANEL_GetHTotal50Hz() != PANEL_GetHTotal60Hz())
			#ifdef SUPPORT_PANEL_48HZ
					|| (PANEL_GetPixelClk48Hz() != PANEL_GetHTotal60Hz())
					|| (PANEL_GetHTotal48Hz() != PANEL_GetHTotal60Hz())
			#endif
				   )			
			#endif	   
            OSD_PLA_Mute();
            #endif
            #endif
        
            if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
            {
                LVDS_SwitchDataEnable(FALSE);
            }
    
            vDrvVOFreqSet(u4PixelClock);
           #ifndef CC_MTK_LOADER		
           vDrvSetSSMDRAsInputHsync();//for dynamic MDR
	    #endif	
    
            // HSync Length
            u4HSyncLen = (u4HTotal - wDISPLAY_WIDTH) ;
            if (u4HSyncLen >= CC_BACKPORCH_MAX)
            {
                u4HSyncLen = CC_BACKPORCH_MAX ;
            }
    
            // H Total
            u4HTotalFinal = u4HTotal ;
    
            // V Total
            u4VTotalFinal = u4VTotal ;

            wDrvSetOutputVTotal(u4VTotalFinal);
            #ifndef CC_COPLAT_MT82
            _OSD_AlwaysUpdateReg(TRUE);
			wDrvSetOutputHTotal(u4HTotalFinal);
            _OSD_AlwaysUpdateReg(FALSE);
            #else
			wDrvSetOutputHTotal(u4HTotalFinal);
            #endif
			wDrvSetOutputHActive(wDISPLAY_WIDTH);
    
            LOG(1, "DM OK HS(%d)\n", u4HSyncLen);
    
        }
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        vDrvSwitchMTKGoodDclk(SV_ON);    
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_OTHERS;
    }
    else
    {
        // FIXME
        // Call vDDDSLCDSetting() instead
        if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 50)
        {
            vDrvLCDFreqSet(SV_DCLK_50HZ, (PANEL_IsSupport120Hz()?100 : 50));
        }
        else if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 25)
        {
            if (DRVCUST_PanelGet(ePanel242530pOutput))
            {
                vDrvLCDFreqSet(SV_DCLK_OTHERS, 25);
            }
            else
            {                
                vDrvLCDFreqSet(SV_DCLK_50HZ, (PANEL_IsSupport120Hz()?100 : 50));
            }
        }
        else if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 60)
        {
            vDrvLCDFreqSet(SV_DCLK_60HZ, (PANEL_IsSupport120Hz()?120 : 60));
        }
        else if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 30)
        {
            if (DRVCUST_PanelGet(ePanel242530pOutput))
            {
                vDrvLCDFreqSet(SV_DCLK_OTHERS, 30);
            }
            else
            {                
                vDrvLCDFreqSet(SV_DCLK_60HZ, (PANEL_IsSupport120Hz()?120 : 60));
            }
        }
        else if (bDrvVideoGetRefreshRate(SV_VP_MAIN) == 24)
	 {
            if (DRVCUST_PanelGet(ePanel242530pOutput))
            {
                vDrvLCDFreqSet(SV_DCLK_OTHERS, 24);
            }
            else
            {                
                #ifdef SUPPORT_PANEL_48HZ
                vDrvLCDFreqSet(SV_DCLK_48HZ, (PANEL_IsSupport120Hz()?96 : 48));
                #else
                vDrvLCDFreqSet(SV_DCLK_60HZ, (PANEL_IsSupport120Hz()?120 : 60));
                #endif
            }
	 }
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }
	
    if (WFB_PANEL_RESET == WFB_PANEL_RESET_ON)
    {
        LVDS_SwitchDataEnable(TRUE);
    }

    fgPreviousMode = BiMode;

    if( _bIsImportProtection )
    {
        vDrvFireImportPortection();
        _bIsImportProtection = IMPORT_PROTECTION_RECOVER;
    }
}
#endif // end of #ifndef CC_MTK_LOADER


void OSTG_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    BOOL fgOriPixelShiftEnable;
    
    if ((ABS(i4HPixelShift) > PIXEL_SHIFT_H_MAX) || (ABS(i4VPixelShift) > PIXEL_SHIFT_V_MAX))
    {
        //LOG(1, "pixel shift out of range, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);
        return;
    }
    //LOG(5, "PMX_SetPixelShift, i4HPixelShift=%d, i4VPixelShift=%d\n",  i4HPixelShift, i4VPixelShift);

    fgOriPixelShiftEnable =  PANEL_IsPixelShift();
    PANEL_SetPixelShift(fgEnable, i4HPixelShift, i4VPixelShift);
     
    // turn on pixel shift
    if (fgEnable)
    {
//        _i4HPixelShift = i4HPixelShift;
        if (!fgOriPixelShiftEnable)
        {
        #ifndef CC_COPLAT_MT82
            IGNORE_RET(OSD_UpdateTiming());
        #endif
            //LOG(5, "turn on pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
        }
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_H;
        _u4OstgUpdateEvent |= OSTG_UPDATE_PIXEL_SHIFT_V;
    }
    else if (!fgEnable && fgOriPixelShiftEnable)     // previous is enable, restore timing
    {
        vDrvSetVsyncBp(u4GetPanelVSyncBp());
        #ifndef CC_COPLAT_MT82
        IGNORE_RET(OSD_UpdateTiming());
        #endif

      //  LOG(5, "turn off pixel shift, width=%d, height=%d\n", PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
    }
}

void Panel_SetDisplayShift(BOOL fgEnable, UINT8 u1ShiftMode,UINT8 u1L1DisplayShift, UINT8 u1R2DisplayShift)	
{
  vIO32WriteFldAlign(VINTF_P17, fgEnable, VINTF_3D_DISP_SHIFT_EN);
  vIO32WriteFldAlign(VINTF_P17, u1ShiftMode, VINTF_3D_DISP_SHIFT_FRAME);  
  vIO32WriteFldAlign(VINTF_P17, 1, VINTF_3D_DISP_SHIFT_CTL);  
  vIO32WriteFldAlign(VINTF_P17, u1L1DisplayShift, VINTF_3D_DISP_SHIFT_VALUE_L);    
  vIO32WriteFldAlign(VINTF_P17, u1R2DisplayShift, VINTF_3D_DISP_SHIFT_VALUE_R);    
}

void Panel_SetDisplayShiftColor(UINT8 u1ShiftColorB,UINT8 u1ShiftColorG, UINT8 u1ShiftColorR)	
{
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorB, VINTF_3D_DISP_SHIFT_B);    
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorG, VINTF_3D_DISP_SHIFT_G);  
  vIO32WriteFldAlign(VINTF_P18, u1ShiftColorR, VINTF_3D_DISP_SHIFT_R);   
}

void Panel_DISPR_FIELD_Delay(UINT8 u1Tpye, UINT8 u1AlignType,UINT16 u1DelayLine)	
{
 if (u1Tpye == eDispRFieldAlign_S_Field)
 {
   // FIELD without delay setting
   vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_FIELD_ALLIGN_TYPE);
   vIO32WriteFldAlign(VINTF_P19, u1DelayLine, VINTF_FIELD_R_H_DLY);   
 }
 else
 {
   #if defined(CC_MT5368) || defined(CC_MT5389)
   if ((BSP_GetIcVersion() >= IC_VER_5368_AB)||(BSP_GetIcVersion() >= IC_VER_5389_AB))
   {
    vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE);
   } 
   else
   {
    vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE_NOECO);
   }
   #else
   vIO32WriteFldAlign(VINTF_P19, u1AlignType, VINTF_DISP_R_ALLIGN_TYPE);
   #endif
   
   // DISP_R delay limitaion: "0~1122" OK, "1123" need bypass 6 line buffer, 1124 can't reach
   if(u1DelayLine < (wDrvGetOutputVTotal()-2))
   {
    vIO32WriteFldAlign(VINTF_P19, u1DelayLine, VINTF_DISP_R_H_DLY);	
   }
   else
   {
    vIO32WriteFldAlign(VINTF_P19, 0, VINTF_DISP_R_H_DLY); 
   }
 }
}

void Panel_3DGlassesCTL(UINT8 u1Pin, UINT8 u1Tpye, BOOL fgPol,BOOL fgPos, UINT8 u1DelayFrame, UINT16 u2DelayLine)
{
  #if defined(CC_MT5396)
  if (u1Pin == e3DGlasses_P_3DR)
  {
   UNUSED(BSP_PinSet(PIN_GPIO52, PINMUX_FUNCTION3)); 
  }
  else if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO53, PINMUX_FUNCTION3)); 
  }
  else if (u1Pin == e3DGlasses_P_3DBL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO54, PINMUX_FUNCTION1)); 
  }
  #elif defined(CC_MT5368)
  if (u1Pin == e3DGlasses_P_3DR)
  {
   UNUSED(BSP_PinSet(PIN_GPIO6, PINMUX_FUNCTION2)); 
  }
  else if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO7, PINMUX_FUNCTION2)); 
  }
  else if (u1Pin == e3DGlasses_P_3DBL)
  {
   UNUSED(BSP_PinSet(PIN_GPIO8, PINMUX_FUNCTION2)); 
  }  
  #elif defined(CC_MT5389)
  if (u1Pin == e3DGlasses_P_3DL)
  {
   UNUSED(BSP_PinSet(PIN_DEMOD_TSDATA0, PINMUX_FUNCTION5)); 
  }
  #endif
  
  if ((BSP_GetIcVersion() >= IC_VER_5396_AB)||(BSP_GetIcVersion() >= IC_VER_5368_AB))
  {
   vIO32WriteFldAlign(VINTF_P9 + (u1Pin *8), u1Tpye, VINTF_REYE_SEL_ECO);
  }
  else
  {
   vIO32WriteFldAlign(VINTF_P9 + (u1Pin *8), u1Tpye, VINTF_REYE_SEL);
  } 
  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgPos, VINTF_2EYE_POL);    
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgPos, VINTF_REYE_V_POS); // 2EYE with V_POS  
  
  if ((BSP_GetIcVersion() >= IC_VER_5396_AB)||(BSP_GetIcVersion() >= IC_VER_5368_AB))
  {
   if (u1Tpye == e3DGlasses_S_BL)
   {
    vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), 1, VINTF_3DBL_LRLR);  
   }	
   else
   {
    vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), 1, VINTF_2EYE_LRLR);  
   }	
  } 
  else
  {
   if (u1Tpye == e3DGlasses_S_BL)
   {
   vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), 1, VINTF_3DBL_LRLR);  
   }
  }
  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), u1DelayFrame, VINTF_2EYE_V_DLY);
  vIO32WriteFldAlign(VINTF_P6 + (u1Tpye *8), u2DelayLine, VINTF_2EYE_H_DLY);
  if (u1Tpye == e3DGlasses_S_2EYE)
  {
   vIO32WriteFldAlign(VINTF_P7, u1Tpye, VINTF_2EYE_SEL);
   vIO32WriteFldAlign(VINTF_P7, fgPol, VINTF_2EYE_POL);
   vIO32WriteFldAlign(VINTF_P7, u1DelayFrame, VINTF_2EYE_V_DLY);
   vIO32WriteFldAlign(VINTF_P6, u2DelayLine, VINTF_2EYE_H_DLY);
  }
  if ((BSP_GetIcVersion() >= IC_VER_5396_AB)||(BSP_GetIcVersion() >= IC_VER_5368_AB))
  {
   if (u1Tpye == e3DGlasses_S_BL2)
   {
    vIO32WriteFldAlign(VINTF_P26, u1Tpye, VINTF_3DBL_2_SEL);
    vIO32WriteFldAlign(VINTF_P26, fgPol, VINTF_3DBL_2_POL);
    vIO32WriteFldAlign(VINTF_P26, 1, VINTF_3DBL_2_LRLR);   
    vIO32WriteFldAlign(VINTF_P26, u1DelayFrame, VINTF_3DBL_2_V_DLY);
    vIO32WriteFldAlign(VINTF_P25, u2DelayLine, VINTF_3DBL_2_H_DLY);
   }  
  }
}

void Panel_3DGlassesRSTCTL(UINT8 u1Tpye, BOOL fgOnOff, UINT16 u2RSTLine)
{  
  vIO32WriteFldAlign(VINTF_P7 + (u1Tpye *8), fgOnOff, VINTF_2EYE_RST_MODE);    
  vIO32WriteFldAlign(VINTF_P6 + (u1Tpye *8), u2RSTLine, VINTF_2EYE_WIDTH);
}

void Panel_3DGlassesDutyCTL(UINT8 u1Tpye, UINT8 u1DutyH)
{ 
    UINT32 u4Vtotal,u4VtotalH;
	u4Vtotal = wDrvGetOutputVTotal();
	
	if (IS_PANEL_PDP)
	{
     u4Vtotal = u4Vtotal >>1;
	}	
	
	 u4VtotalH = u4Vtotal *  u1DutyH / 100 ;


    if (u4VtotalH >= 1)
    {
      u4VtotalH = u4VtotalH - 1;
    }

    Panel_3DGlassesRSTCTL(u1Tpye, 1, u4VtotalH);
}

// fgonoff = 0 [off ] fgonoff = 1 [on ]
void Panel_3DGlassesCTLEn(UINT8 u1Pin, BOOL fgonfff)
{
   vIO32WriteFldAlign(VINTF_P9 + (u1Pin *8), !fgonfff, VINTF_REYE_OP_EN);  
}

void vDrvVideoSetMuteOnOff(UINT8 ucVdpId, UINT8 fgEnable)
{
    if(bDrvVideoSignalStatus(ucVdpId) == SV_VDO_STABLE)
    {
        if(ucVdpId == SV_VP_MAIN)
        {
            vRegWriteFldAlign(MUTE_00, fgEnable, R_M_MUTE_FRONT_EN);
        }
        else
        {
            vRegWriteFldAlign(MUTE_00, fgEnable, R_P_MUTE_FRONT_EN);
        }
    }
}

void vDrvVideoSetMuteColor(UINT8 bPath, UINT32 u4Bg) 
{
    RDispInfo *prDispInfo;
    prDispInfo = getDispInfo(bPath);
    
    if (prDispInfo->u4MuteColor != u4Bg)
    {
        prDispInfo->u4MuteColor = u4Bg;
        if (bPath == SV_VP_MAIN)
        {
            vSetScposFlg(MAIN_MUTECOLOR_CHG);
        }
        else
        {
            vSetScposFlg(PIP_MUTECOLOR_CHG);
        }
    }
}

/**
* @brief Output stage normproc
*/
void vOstgNormProc(void)
{
#ifndef CC_MTK_LOADER
#if 0//def SUPPORT_CW_CHANGE_BY_STEP
    static UINT32 u4Val = 0;
    static UINT32 u4LocalTargetCW = 0;
    static BOOL isIncrease;
    static BOOL isFirstStep = TRUE;

    if( (u4LocalTargetCW != _u4TargetCW) && isFirstStep )
    {
        u4Val = _u4CurrentCW; //inital the first CW
        isIncrease = (_u4TargetCW > _u4CurrentCW ) ? TRUE : FALSE;
        isFirstStep = FALSE;

        //give-up first entry, delay the pll setting to next vsync (osd muted)
        return;
    }

    if( (_u4CurrentCW == _u4TargetCW) || (u4Val == _u4TargetCW) )
    {
        u4LocalTargetCW = _u4TargetCW;
        isFirstStep = TRUE;

        return;
    }



    if( 0xfffffff == _u4CWStep )
    {
        u4Val = _u4TargetCW;
    }
    else
    {
        if (isIncrease)
        {
            u4Val = ( (u4Val+_u4CWStep) >= _u4TargetCW ) ? _u4TargetCW : (u4Val+_u4CWStep);
        }
        else
        {
            u4Val = ( (u4Val-_u4CWStep) <= _u4TargetCW ) ? _u4TargetCW : (u4Val-_u4CWStep);
        }
    }

    if( u4Val == _u4TargetCW )
    {
        u4LocalTargetCW = u4Val;
        isFirstStep = TRUE;
    }
    
    vIO32WriteFldAlign(DDDS_00,u4Val,DDDS_00_DDDS_FREQ_CW);

#ifdef SUPPORT_FREERUN_DCLK_BY_STEP
    // store the original control word
    i4OrigControlWord = u4Val;
#endif
    
    vDrvVoDclkSpreadInit(1);
    HAL_Delay_us(100);
    vDrvVoDclkSpreadInit(0);   
#endif

    
    #ifdef SUPPORT_DDDS_STEP_TRACKING
    vDrvDDDSTrackingNormProc();
    #endif
    
#ifdef SUPPORT_FREERUN_DCLK_BY_STEP 
        static INT32 i4DeltaControlWord = 0;
        if(PANEL_GetDisplayType()== PANEL_DISPLAY_TYPE_VB1)
        {
    
        if (u1DDDSLockStatus == 1)    
        {
            if(u1SignalStatus != bDrvVideoSignalStatus(SV_VP_MAIN))
            {
                 u1SignalStatus = bDrvVideoSignalStatus(SV_VP_MAIN);
             }
            if(u1SignalStatus == SV_VDO_STABLE)
            {
               if(u1StepDelay2 < STEP_CW_DELAY2)
               {
                   u1StepDelay2++;
               }
               else
               {
                   if(_u4ErrorLimit < TargetErrlimit)
                   {
                       vDrvSetErrorLimit(_u4ErrorLimit+1);
//                       LOG(2,"_u4ErrorLimit %d\n", _u4ErrorLimit);
                   }
				   vDrvSetDDDSCloseloop(1);
               }
            }
        }
    //    if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 1)    
        {
            if(u1StepDelay < STEP_CW_DELAY)
            {
                u1StepDelay++;
           }
            else
        {
            if(u1StepControlWord < STEP_CW)
            {
                u1StepControlWord++;       
    
                if(u1StepControlWord < STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    if(u1StepControlWord == 1)
                    {
                        i4DeltaControlWord = ((INT32)IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW) - i4OrigControlWord)/STEP_CW;
                    }
                    
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord + i4DeltaControlWord*(STEP_CW - u1StepControlWord), DDDS_00_DDDS_FREQ_CW);
                    
                }
                else if(u1StepControlWord == STEP_CW && i4OrigControlWord != 0x7fffffff)
                {
                    vIO32WriteFldAlign(DDDS_00, i4OrigControlWord, DDDS_00_DDDS_FREQ_CW);
                }
    
                vIO32WriteFldAlign(DDDS_03, 1, DDDS_03_SPREAD_INIT);
                HAL_Delay_us(20);
                LOG(2,"step %d, freerun-CW: %x\n", u1StepControlWord, IO32ReadFldAlign(DDDS_00, DDDS_00_DDDS_FREQ_CW));
                vIO32WriteFldAlign(DDDS_03, 0, DDDS_03_SPREAD_INIT);
            }
        }
        }
        }
#endif

#endif // CC_MTK_LOADER
    return;
}

#ifdef CC_VESTEL_ESD_TEST
#ifndef CC_MTK_LOADER
	extern BOOL _fgVideoReady;
#endif
#endif
// OSTG Event on Output VSync
// Note: Keep this function simple since this function is called by ISR
void OSTG_OnOutputVSync(void)
{
#ifdef CC_VESTEL_ESD_TEST
#ifndef CC_MTK_LOADER
	static UINT8 u1ClockSkewCnt = 0;
	BOOL fgclockskew = 0;
#endif
#endif
    
#ifndef CC_MTK_PRELOADER
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_H)
    {
        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_H;
    }
    if (_u4OstgUpdateEvent & OSTG_UPDATE_PIXEL_SHIFT_V)
    {
        vDrvSetVsyncBp(u4GetPanelVSyncBp());
        _u4OstgUpdateEvent &= ~OSTG_UPDATE_PIXEL_SHIFT_V;
    }    
#endif

#ifndef CC_MTK_LOADER
    if (fgIsScposFlgSet(MAIN_MUTECOLOR_CHG))
    {
        vDrvVideoMuteColor(SV_VP_MAIN, _rMDispInfo.u4MuteColor);
        vClrScposFlg(MAIN_MUTECOLOR_CHG);
    }
    if (fgIsScposFlgSet(PIP_MUTECOLOR_CHG))
    {
        vDrvVideoMuteColor(SV_VP_PIP, _rPDispInfo.u4MuteColor);
        vClrScposFlg(PIP_MUTECOLOR_CHG);
    }

    if (_bIsImportProtection > IMPORT_PROTECTION)
    {
        _bIsImportProtection --;
        if (_bIsImportProtection == IMPORT_PROTECTION)
        {
            vDrvSwitchImportProtection(0);
        }
    }
#endif 
#ifdef CC_VESTEL_ESD_TEST
#ifndef CC_MTK_LOADER
	if (_fgVideoReady)

	{
		fgclockskew = u1ChkLVDSClockSkewStatus(); 
		//Printf("[SA8_Nick] fgclockskew = %d \n",fgclockskew);
		if (fgclockskew)
		{
			Printf("[LVDS] clock skew \n");
			u1ClockSkewCnt ++;
			if (u1ClockSkewCnt > 5)
			{
				u1LVDSClockSkew_Recover();
				u1ClockSkewCnt = 0 ;
			}
				u1LVDSClockSkew_Reset();				
		}	
		else
		{
			u1ClockSkewCnt = 0;
		}
	}
#endif 
#endif
}


void vDrvSwitchImportProtection(UINT8 bOn_Off)
{
    if ( bOn_Off )
    {
        vRegWriteFldAlign(OSTG_00, 0, REG_SET_DATA_READY);
		if (fgIsMJCToOSTG())
		{
         vIO32WriteFldAlign(PSPIP_OA_02, 0, PSPIP_OA_02_SET_DATA_SEL);
		} 
		else
		{
         vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SET_DATA_SEL);
		}
    }
    else
    {
        vRegWriteFldAlign(OSTG_00, 1, REG_SET_DATA_READY);
		if (fgIsMJCToOSTG())
		{
         vIO32WriteFldAlign(PSPIP_OA_02, 1, PSPIP_OA_02_SET_DATA_SEL);
		} 
		else
		{
         vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SET_DATA_SEL);
		} 
    }
}


void vDrvFireImportPortection(void)
{
    // trigger by the edge from 0 to 1
    if (fgIsMJCToOSTG())
    {
     vIO32WriteFldAlign(PSPIP_OA_06, 0, PSPIP_OA_06_SET_DATA_TRIGGER);
     vIO32WriteFldAlign(PSPIP_OA_06, 1, PSPIP_OA_06_SET_DATA_TRIGGER);
    }
	else
	{
     vRegWriteFldAlign(SCPIP_PIP_OA_06, 0, PIP_OA_06_SET_DATA_TRIGGER);
     vRegWriteFldAlign(SCPIP_PIP_OA_06, 1, PIP_OA_06_SET_DATA_TRIGGER);
	} 
}

void vDrvSetErrorLimit(UINT32 u4Error)
{
    if (u4Error != _u4ErrorLimit)
    {
        _u4ErrorLimit = u4Error;
        vIO32WriteFldAlign(DDDS_03, _u4ErrorLimit, DDDS_03_DDDS_ERR_LIM);
    }
}

#define bISRVcounter()	IO32ReadFldAlign(INTR_00,FCONT)
static UINT8 u1ISRVCnt = 0;
static UINT8 u1ISRVvalue_backup = 0;
BOOL TconVsyncRecover = 0;
UINT16 u2HsyncFp_dead;

void vDrvChkVcounter(void)
{
  UINT8 u1ISRVvalue;

  if (TconVsyncCheck)
  {
    if (u1ISRVCnt>=10)
    {
     u1ISRVCnt = 0;
     TconVsyncRecover = 1;
	 Printf("Do PDP Vsync recover for disappear!!!\n");
	 Printf("u2HsyncFp_dead = %d!!!\n",u2HsyncFp_dead);	 
	 // TconVsyncCheck = 0;
    }
    u1ISRVvalue = bISRVcounter();

	// Printf("===== [%d] u1ISRVvalue = %d u1ISRVvalue_backup = %d \n",u1ISRVCnt, u1ISRVvalue,u1ISRVvalue_backup);
    if (u1ISRVvalue == u1ISRVvalue_backup)
    {   
     u1ISRVCnt ++;
	 u2HsyncFp_dead = IO32ReadFldAlign(PSPIP_OA_08, PSPIP_OA_08_HSYNC_PORCH_F);
    }
	else
	{
     u1ISRVCnt = 0;
	}
	
    u1ISRVvalue_backup = u1ISRVvalue;
  }
}

void vDrvVsyncHFpAdj(void)
{
  UINT16 u2HsyncFp_Adj;

  if (TconVsyncRecover)
  {
    u2HsyncFp_Adj = IO32ReadFldAlign(PSPIP_OA_08, PSPIP_OA_08_HSYNC_PORCH_F)+1;
	if (u2HsyncFp_dead == u2HsyncFp_Tbl)
	{
      u2HsyncFp_Adj = u2HsyncFp_Tbl +1;
	}
	else if (u2HsyncFp_dead == (u2HsyncFp_Tbl+1))
	{
      u2HsyncFp_Adj = u2HsyncFp_Tbl;
	}		
	else
	{
	 u2HsyncFp_Adj = u2HsyncFp_Tbl;
	}
	
    if((u2HsyncFp_Adj == u2HsyncFp_Tbl) || (u2HsyncFp_Adj == (u2HsyncFp_Tbl +1)))
    {
     vIO32WriteFldAlign(PSPIP_OA_08,u2HsyncFp_Adj, PSPIP_OA_08_HSYNC_PORCH_F);
     Printf("u2HsyncFp_Adj = %d!!!\n,",u2HsyncFp_Adj);
     TconVsyncRecover = 0;   
    } 
  }
}


#ifndef CC_MTK_LOADER
#ifdef SUPPORT_SW_FRAME_CHECK
void vSetSWFrameTrackOnOff(UINT32 u4OnOff)
{
    if (_u4VDistTrackEn != u4OnOff)
    {
        LOG(3, "SW Frame Track (%d)\n", u4OnOff);
        _u4VDistTrackEn = u4OnOff;

	 // Speed up DDDS lock when SW frame track is enable.
         Set_DDDS_KPI(u4OnOff);
    }
}

void vSetSWFrameTrackStep(UINT32 u4Step)
{
    if (_u4VDistTrackMaxStep != u4Step)
    {
        LOG(3, "SW Frame Track Step (%d)\n", u4Step);
        _u4VDistTrackMaxStep = u4Step;
    }
}

void vSetSWFrameTrackLockRange(UINT32 u4Upper, UINT32 u4Lower)
{
    if (_u4VDistUpperBound != u4Upper)
    {
        LOG(3, "SW Frame Track Upper Bound (%d --> %d)\n", _u4VDistUpperBound, u4Upper);
        _u4VDistUpperBound = u4Upper;
    }
    if (_u4VDistLowerBound != u4Lower)
    {
        LOG(3, "SW Frame Track Lower Bound (%d --> %d)\n", _u4VDistLowerBound, u4Lower);
        _u4VDistLowerBound = u4Lower;
    }
}

void vDrvVDistanceCheck(void)
{
    UINT32 u4VDistance, bmode;
    UINT32 u4LowerBound, u4UpperBound;
    UINT32 u4VTotal;
    UINT32 u4UpTrackRange, u4DownTrackRange;
    UINT8 bScaleMode =  u1ScpipGetDispMode(SV_VP_MAIN) ;
    if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN)== 0)    
    {
        _u4UpTrackRange = 0;
        _u4DownTrackRange = 0;
        _u4VDistTrackState = 0;
        _u4VDistTrackLocked = 0;
	 LOG(6, "vDrvVDistanceCheck() : DISP_EN=0, return.\n");

        return;
    }

    if(RegReadFldAlign(SCPIP_PIP1_09,  PIP1_09_F_SYNC_1)==1)
    {
       LOG(6, "vDrvVDistanceCheck() : PIP1_0B_F_SYNC_1=1, return.\n");
    	return;
    }

    bmode =u1ScpipGetDispMode(SV_VP_MAIN);
    if(bmode == VDP_SCPOS_PSCAN_DISPMODE)
    {
        u4VDistance = u2DrvDIGetVcnt(VDP_1);
    }
    else
    {
        u4VDistance = u4ScpipGetInOutVDistance();
    }

    #if 1
    u4VTotal = (UINT32)wDrvGetOutputVTotal();
    
    u4LowerBound = (_u4VDistLowerBound > 0x800)? (_u4VDistLowerBound-0x800) : (u4VTotal + _u4VDistLowerBound - 0x800);
    u4UpperBound = (_u4VDistUpperBound > 0x800)? (_u4VDistUpperBound-0x800) : (u4VTotal + _u4VDistLowerBound - 0x800);

    // get tracking distance
    u4UpTrackRange = 0;
    u4DownTrackRange = 0;
    if (u4UpperBound >= u4LowerBound)
    {
        if (u4VDistance > u4UpperBound)
        {
            u4DownTrackRange = u4VDistance - u4UpperBound;
            u4UpTrackRange = u4VTotal + u4LowerBound - u4VDistance;
        }
        else if (u4VDistance < u4LowerBound)
        {
            u4DownTrackRange = u4VDistance + u4VTotal - u4UpperBound;
            u4UpTrackRange = u4LowerBound - u4VDistance;        
        }
    }
    else  // u4UpperBound < u4LowerBound
    {
        if ((u4VDistance > u4UpperBound) && (u4VDistance < u4LowerBound))
        {
            u4DownTrackRange = u4VDistance - u4UpperBound;
            u4UpTrackRange = u4LowerBound - u4VDistance;        
        }
    }

    if (_u4VDistTrackEn)
    {
        if (_u4VDistTrackLocked < 2)
        {
            if ((u4UpTrackRange > _u4VDistTorance) && (u4DownTrackRange > _u4VDistTorance))
            {
                // start tracking
                _u4VDistTrackLocked = 0;
                _u4UpTrackRange = u4UpTrackRange;
                _u4DownTrackRange = u4DownTrackRange;
                if (u4UpTrackRange > u4DownTrackRange)
                {  // down tracking
                    _u4VDistTrackState = 1;
                }
                else
                {  // up tracking
                    _u4VDistTrackState = 2;
                }
                vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),bDrvVideoGetRefreshRate(SV_VP_MAIN));
                LOG(5, "SW Frame Track Bound(%d ~ %d), Dir(%d),Current: %d, Dist(%d, %d)\n", u4LowerBound, u4UpperBound, _u4VDistTrackState, u4VDistance, u4DownTrackRange, u4UpTrackRange);
            }
            else
            {
                _u4VDistTrackLocked ++;
                if (_u4VDistTrackLocked == 2)
                {
                    _u4VDistTrackState = 3;// SW frame track locked.
                    vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),bDrvVideoGetRefreshRate(SV_VP_MAIN));
                    LOG(5, "SW frame track locked.\n");
                }
            }
        }
        else  if ((u4UpTrackRange > _u4VLossDistTorance) && (u4DownTrackRange > _u4VLossDistTorance))
        {
            _u4VDistTrackLocked = 0;
	     _u4VDistTrackState = 0;
               if (bScaleMode == VDP_SCPOS_DISPMODE_AUTO || bScaleMode == VDP_SCPOS_DISPMODE_MANUAL) 
               	{
         	   vScpipSetDispModeFreeRunCount(150);
               	} 
	       LOG(5, "SW frame track : Loss lock.  (Range: %d, %d)\n",u4UpTrackRange, u4DownTrackRange);
        }

    }
    else
    {
        if (_u4VDistTrackState)
        {
            // recover
            _u4UpTrackRange = 0;
            _u4DownTrackRange = 0;
            _u4VDistTrackState = 0;
            _u4VDistTrackLocked = 0;
            vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),bDrvVideoGetRefreshRate(SV_VP_MAIN));
            LOG(5, "SW frame track : Off.\n");
        }
    }

    
    #else
    
    if(u4VDistance >= V_DISTANCE_RANGE)
    {
        LOG(5,"u4VDistance(%d)\n",u4VDistance);
        u4CoarseStepSize = COARSE_STEP_SIZE;
        u4FineStepSize = FINE_STEP_SIZE;
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN));
    }
    else
    {   
        u4CoarseStepSize = 0;
        
        if(u4InOutVDistance != u4VDistance)
        {
            LOG(5,"IN OUT V DISTANCE CHANGED FROM (%d) TO (%d)\n",u4InOutVDistance,u4VDistance);

            if(u4VDistance < u4InOutVDistance)
            {
                if(u4CurrentStep<FINE_STEP_MAX)
                {
                    u4CurrentStep++;
                }
                else
                {
                    u4CurrentStep = FINE_STEP_MAX;
                }            
            }
            else
            {
                if(u4CurrentStep>FINE_STEP_MIN)
                {
                    u4CurrentStep--;
                }
                else
                {
                    u4CurrentStep = FINE_STEP_MIN;
                }        
            }
            
            LOG(5,"u4CurrentStep(%d)\n",u4CurrentStep);        
            
            //update current distance
            u4InOutVDistance = u4VDistance;
            
            vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN));
        }
        else
        {                
            u4NoChangeCounter++;
            if(u4NoChangeCounter == V_STABLE_COUNT)
            {
                LOG(5,"V DISTANCE IS STABLE(%d)\n",u4VDistance);
                u4NoChangeCounter = 0;
                u4CurrentStep = FINE_STEP_CENTER;
            }
        }
    }
    #endif
}

#endif
#endif

UINT8 u1SetFlipMirrorConfig(BOOL fgMirrorEn, BOOL fgFlipEn)
{
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
    DTVCFG_T rDtvCfg;
    
    if(EEPDTV_GetCfg(&rDtvCfg))
    {
        Printf("Read Flip-Mirror Config Fail\n");
        return SV_FAIL;
    }

    if (fgMirrorEn)
    {
        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_MIRROR_ON);
    }
    else
    {
        rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_MIRROR_ON);
    }
    if(EEPDTV_SetCfg(&rDtvCfg))
    {
        Printf("Set Mirror Config Fail\n");
        return SV_FAIL;
    }

    if (fgFlipEn)
    {
        rDtvCfg.u1Flags2 |= (DTVCFG_FLAG2_FLIP_ON);
    }
    else
    {
        rDtvCfg.u1Flags2 &= ~(DTVCFG_FLAG2_FLIP_ON);
    }
    if(EEPDTV_SetCfg(&rDtvCfg))
    {
        Printf("Set Flip Config Fail\n");
        return SV_FAIL;
    }
    Printf("fgMirrorEn: %d\tfgFlipEn: %d\trDtvCfg.u1Flags2: %d\n", fgMirrorEn, fgFlipEn, rDtvCfg.u1Flags2);
    return SV_SUCCESS;
    
#else
    Printf("CC_FLIP_MIRROR_FROM_EEPROM is not defined!\n");
    return SV_FAIL;
#endif
}


#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
UINT8 u1GetFlipMirrorConfig(void)
{
    static UINT8 u1Init = 10 ;
    static UINT8 u1FlipMirrorEnable= 0 ;
#ifdef CC_FLIP_MIRROR_FROM_EEPROM
	DTVCFG_T rDtvCfg;
#endif
    
    //LOG(0, "======u1GetFlipMirrorConfig=======.\n\n", u1FlipMirrorEnable);
    if(u1Init >= 255)
    {
      u1Init = 0;
    }
    //Printf("\n------------------ fgTestmode = %d -----------------\n",fgTestmode);
     if(fgTestmode == FALSE)
     {
         if ( u1Init > 0 && u1FlipMirrorEnable == 0)
         {
          //LOG(0, "======u1GetFlipMirrorConfig,ucnt=%d,flip=%d=======.\n\n",u1Init,u1FlipMirrorEnable);
          if(LVDS_VFLIP_SETTING == LVDS_VFLIP_ON)
          {
             u1FlipMirrorEnable |=SYS_FLIP_CONFIG_ON;
          }
          if(LVDS_HFLIP_SETTING == LVDS_HFLIP_ON)
          {
             u1FlipMirrorEnable |=SYS_MIRROR_CONFIG_ON;
          }
    
         }   
     }
     else
     {
         u1FlipMirrorEnable &= ~(SYS_FLIP_CONFIG_ON);
         u1FlipMirrorEnable &= ~(SYS_MIRROR_CONFIG_ON);
         //LOG(0, "==2====u1GetFlipMirrorConfig,ucnt=%d,flip=%d=======.\n\n",u1Init,u1FlipMirrorEnable);
    
     }

    u1Init--;
    //LOG(0, "======u1FlipMirrorEnable %d\n\n", u1FlipMirrorEnable);
    return u1FlipMirrorEnable;
}

UINT32 u4GetFlipMirrorModule(UINT32 u4VdpId)
{
#ifndef CC_MTK_LOADER
	UINT32 u4Module =FLIP_NONE;
	if(u1GetFlipMirrorConfig())
	{
		u4Module= u4QueryFlipModule(u4VdpId);
	}
	
       LOG(9, "======u4GetFlipMirrorModule(%d) %d\n\n", u4VdpId, u4Module);

	return u4Module;
#else
	return 0;
#endif
}
#endif  //CC_FLIP_MIRROR_SUPPORT

#ifndef CC_MTK_LOADER
#ifdef SUPPORT_DDDS_STEP_TRACKING
void vDrvDDDSStepTrackingOnOff(UINT16 u2OnOff)
{
    if (_u2DDDSStepTrackingEn == u2OnOff)
    {
        return;
    }
    LOG(3, "DDDSStepTrackingOnOff: %d\n", u2OnOff);
    _u2DDDSStepTrackingEn = u2OnOff;

    // update error limit
    if (u2OnOff)
    {
        _u4DefaultErrorLimit = IO32ReadFldAlign(DDDS_03,DDDS_03_DDDS_ERR_LIM);
        vDrvSetErrorLimit(_u4DDDSTrackingErrorLimit);
    }
    else
    {
        vDrvSetErrorLimit(_u4DefaultErrorLimit);
    }
    
}

void vDrvUpdateDisplayCenterCW(UINT32 u4CW)
{

    if (_u4CenterCW == u4CW)
    {
        return;
    }    

    // update center frequence
    _u4CenterCW = u4CW;
    _u4CurrentStepCW = u4CW;

    // calculate CW upper/lower bound
    _u4StepInCWUpperBound = _u4CenterCW + ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);    
    _u4StepInCWLowerBound = _u4CenterCW - ((_u4DefaultErrorLimit - _u4DDDSTrackingErrorLimit) * 32768);   

    // if target freq delta is over  _u4CenterFreqStep, FW will update display freq.  
//    _u4StepInCW = _u4CenterFreq * (_u2DDDSTrackingSpeed - _u4CenterFreqStepInTolerance)/1000000;   
}

static void vDrvDDDSTrackingNormProc(void)
{
    if (_u2DDDSStepTrackingEn)
    {
        UINT32 u4CW;
        UINT32 u4CWUpperBound, u4CWLowerBound;
        
        // only step tracking with DDDS close loop
        if (IO32ReadFldAlign(DDDS_00,DDDS_00_DISP_EN) == 0)
        {
            return;
        }
        if ((_u4CenterCW == 0) || (_u4CurrentStepCW == 0))
        {
            LOG(1, "Invalid DDDS center CW\n");
            return;
        }

        u4CW = IO32ReadFldAlign(STA_DDDS_00, STA_DDDS_00_DDDS_FREQ_CW);
        u4CWUpperBound = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768) - _u4StepInCWTolerance;
        u4CWLowerBound = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768) + _u4StepInCWTolerance;
        if (u4CWUpperBound <= u4CWLowerBound)
        {
            LOG(2, "Invalid DDDS Step Tracking Bound (%d, %d)\n", u4CWLowerBound, u4CWUpperBound);
            return;
        }
        
        if ((u4CW > u4CWUpperBound) || (u4CW < u4CWLowerBound))
        {
            UINT32 u4NewCW;

            if (u4CW > u4CWUpperBound)
            {
                u4NewCW = _u4CurrentStepCW + (_u4DDDSTrackingErrorLimit * 32768); 
            }
            else //             (u4CW < u4CWLowerBound))
            {
                u4NewCW = _u4CurrentStepCW - (_u4DDDSTrackingErrorLimit * 32768); 
            }

            if (u4NewCW > _u4StepInCWUpperBound)
            {               
                u4NewCW = _u4StepInCWUpperBound;               
            }
            else if (u4NewCW < _u4StepInCWLowerBound)
            {
                u4NewCW = _u4StepInCWLowerBound;               
            }

            if (u4NewCW != _u4CurrentStepCW)
            {
                LOG(3, "DDDS Step Tracking: CW (0x%X) --> (0x%X)\n", _u4CurrentStepCW, u4NewCW);
                _u4CurrentStepCW = u4NewCW;
                vIO32WriteFldAlign(DDDS_00,u4NewCW,DDDS_00_DDDS_FREQ_CW);                
            }
            
            
        }

    }
}
#endif

void vDrvSetSyncLockMode(enum eSyncLockMode eMode)
{
    _eSyncLockMode = eMode;

    if(_eSyncLockMode == eSYNCLOCK_OFF)
    {
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }
    else
    if(_eSyncLockMode == eSYNCLOCK_ON)
    {
        vDrvSetMTKGoodDclk(wDrvVideoGetVTotal(SV_VP_MAIN),bDrvVideoGetRefreshRate(SV_VP_MAIN));
        vDrvSwitchMTKGoodDclk(SV_ON);
    }
    else
    {
        /* do nothing. */
    }
    return;
}

/**
 * @brief vDrvSetDDDSFrameTrackTarget

 * Set DDDS Frame track Parameters

 * @param  u1SlowFast: slow/fast parameter
 * @param  u4TargetVTotal: target output Vtotal
 * @retval void
 */ 
void vDrvSetDDDSFrameTrackTarget(UINT8 u1SlowFast, UINT32 u4TargetVTotal) 
{
	UINT32 DISPLAY_SIZE, HLEN_INT, HLEN_NUM, InputVTotal;
	UINT32 u4HTotal, u4VTotal;
//	Printf("slowfast(%d), vtotal(%d)\n",u1SlowFast,u4TargetVTotal);

	InputVTotal = IO32ReadFldAlign(DDDS_02, DDDS_02_HLEN_DEN);
	HLEN_NUM = IO32ReadFldAlign(DDDS_01, DDDS_01_HLEN_NUM);
	HLEN_INT = IO32ReadFldAlign(DDDS_01, DDDS_01_HLEN_INT);
	DISPLAY_SIZE = (HLEN_INT * InputVTotal) + HLEN_NUM;
    

    u4HTotal = (UINT32)wDrvGetOutputHTotal();
    u4VTotal = (UINT32)wDrvGetOutputVTotal();

	if(u4TargetVTotal > u4VTotal)
		DISPLAY_SIZE = DISPLAY_SIZE +(u4TargetVTotal - u4VTotal)*u4HTotal;
	else
		DISPLAY_SIZE = DISPLAY_SIZE -(u4VTotal - u4TargetVTotal)*u4HTotal;

	if (InputVTotal ==0) InputVTotal = 1;
	
    HLEN_INT = (DISPLAY_SIZE / InputVTotal);
    HLEN_NUM = DISPLAY_SIZE - (HLEN_INT * InputVTotal);
	
    switch(u1SlowFast)
    {
    case SV_DDDS_FRAMETRACK_FAST1:
		vIO32WriteFldAlign(DDDS_0C,(UINT16)InputVTotal,DDDS_0C_HLEN_DEN_E1);
		vIO32WriteFldAlign(DDDS_0D,(UINT16)HLEN_NUM,DDDS_0D_HLEN_NUM_E1);	  
		vIO32WriteFldAlign(DDDS_0C,HLEN_INT,DDDS_0C_HLEN_INT_E1);
        break;

    case SV_DDDS_FRAMETRACK_FAST2:
		vIO32WriteFldAlign(DDDS_0E,(UINT16)InputVTotal,DDDS_0E_HLEN_DEN_E2);
		vIO32WriteFldAlign(DDDS_0E,(UINT16)HLEN_NUM,DDDS_0E_HLEN_NUM_E2);	  
		vIO32WriteFldAlign(DDDS_0D,HLEN_INT,DDDS_0D_HLEN_INT_E2);
        break;

    case SV_DDDS_FRAMETRACK_SLOW1:
		vIO32WriteFldAlign(DDDS_0F,(UINT16)InputVTotal,DDDS_0F_HLEN_DEN_L1);
		vIO32WriteFldAlign(DDDS_10,(UINT16)HLEN_NUM,DDDS_10_HLEN_NUM_L1);	  
		vIO32WriteFldAlign(DDDS_0F,HLEN_INT,DDDS_0F_HLEN_INT_L1);
        break;

	case SV_DDDS_FRAMETRACK_SLOW2:
		vIO32WriteFldAlign(DDDS_11,(UINT16)InputVTotal,DDDS_11_HLEN_DEN_L2);
		vIO32WriteFldAlign(DDDS_11,(UINT16)HLEN_NUM,DDDS_11_HLEN_NUM_L2);	  
		vIO32WriteFldAlign(DDDS_10,HLEN_INT,DDDS_10_HLEN_INT_L2);
		break;

    default:
        break;
    }
	
}

/**
 * @brief vDrvSetDDDSFrameTrackMode

 * Set DDDS Frame track mode

 * @param  u1Mode: 0 => off, 1 => form scaler, 2 => form MJC
 * @retval void
 */ 
void vDrvSetDDDSFrameTrackMode(UINT8 u1Mode) 
{
    switch(u1Mode)
    {
	case 0: //off
		vIO32WriteFldAlign(DDDS_0C,SV_OFF,DDDS_0C_VSYNC_TRACK_EN);
		break;

	case 1: //from scaler
		vIO32WriteFldAlign(DDDS_0C,SV_ON,DDDS_0C_VSYNC_TRACK_EN);
		vIO32WriteFldAlign(DDDS_03,1,DDDS_03_SC_MJC_TRACK_SEL);
		break;

	case 2: //from MJC
		vIO32WriteFldAlign(DDDS_0C,SV_ON,DDDS_0C_VSYNC_TRACK_EN);
		vIO32WriteFldAlign(DDDS_03,0,DDDS_03_SC_MJC_TRACK_SEL);
		break;

	default:
		break;
    }
}

void vDDDSLCDSetting(UINT16 u2InVTotal, UINT8 bRefreshRate)
{
//    UINT8 bRefreshRate;
    UINT8 bLCDFreqType, bDDDSOn;
    UINT8 bOutFrameRate;

    // --------LCD Freq Settings-----------
    //bRefreshRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

    LOG(3, "vDDDSLCDSetting(u2InVTotal=%d,bRefreshRate=%d)\n",u2InVTotal,bRefreshRate);

    if ( _bForceFrameRate != 0 )
    {
        bRefreshRate = _bForceFrameRate;
    }

    bLCDFreqType = SV_DCLK_60HZ;
    bDDDSOn = SV_TRUE;
    bOutFrameRate = bRefreshRate;

    if (((bRefreshRate == 25) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 50) || (bRefreshRate == 100))
    {    
        bLCDFreqType = SV_DCLK_50HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 100: 50;
		#if defined(CC_MT5396)
        if ((((u1DrvMJCIsSupport50To120()&MJC_SUPPORT_50TO120_2D) && E_TDTV_UI_3D_MODE_OFF == u4DrvTDTV3DModeQuery())||
        ((u1DrvMJCIsSupport50To120()&MJC_SUPPORT_50TO120_3D) && E_TDTV_UI_3D_MODE_OFF != u4DrvTDTV3DModeQuery())))
        {
            bLCDFreqType = SV_DCLK_60HZ;
			bOutFrameRate = PANEL_IsSupport120Hz()? 120: 60;
            LOG(2, "MJC_50_TO_120 bDCLKType = SV_DCLK_60HZ\n");
        }
	    LOG(2,"u4DrvTDTV3DModeQuery :%d \n",u4DrvTDTV3DModeQuery());
	    #endif
    }
    else if (((bRefreshRate == 30) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 60)|| (bRefreshRate == 120))
    {
        bLCDFreqType =SV_DCLK_60HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 120: 60;
    }
    else if (((bRefreshRate == 24) && (DRVCUST_PanelGet(ePanel242530pOutput) == 0)) || (bRefreshRate == 48))
    {
        #ifdef SUPPORT_PANEL_48HZ
        bLCDFreqType =SV_DCLK_48HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 96: 48;
        #else
        bLCDFreqType =SV_DCLK_60HZ;
        bOutFrameRate = PANEL_IsSupport120Hz()? 120: 60;
        #endif
    }
    else
    {
        UINT32 u4LockOthers;
        UINT32 u4Clk;
        if (DRVCUST_PanelQuery(eDDDSLockOnOthers, &u4LockOthers) != 0)
        {
            u4LockOthers = 1;
        }
        if (PANEL_IsSupport120Hz())
        {
            if (bRefreshRate >= 110)     // > 110; try 1:1 output
            {
                u4Clk = PANEL_GetPixelClk60Hz()/120 * bRefreshRate;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 120;
            }
            else if (bRefreshRate >= 77)  // 77 ~ 110; try 1:1 output 
            {
                u4Clk = PANEL_GetPixelClk50Hz()/100 * bRefreshRate;
                if ((u4Clk < DCLK_MAX) && (u4Clk > DCLK_MIN) && (u2InVTotal > 0) && (u4LockOthers))   // 1:1 output
                {
                    bLCDFreqType = SV_DCLK_50HZ;
                    bOutFrameRate = 100;
                }
                else    // 1:2 output
                {
                    bLCDFreqType = SV_DCLK_60HZ;
                    bOutFrameRate = 120;
                }
            }
            else if (bRefreshRate < 55)   // < 55; try 1:2 output
            {
                u4Clk = PANEL_GetPixelClk50Hz()/100 * bRefreshRate * 2;
                bLCDFreqType = SV_DCLK_50HZ;
                bOutFrameRate = 100;
            }
            else        // 55 ~ 77; try 1:2 output
            {
                u4Clk = PANEL_GetPixelClk60Hz()/120 * bRefreshRate * 2;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 120;
            }
        }
        else
        {
			#ifdef SUPPORT_PANEL_48HZ
            if(bRefreshRate < 24) 
            { 
                if(DRVCUST_PanelGet(ePanel242530pOutput) == 0)
				bRefreshRate = bRefreshRate *2;	               
				
                u4Clk = PANEL_GetPixelClk48Hz()/48 * (bRefreshRate*2);
                bLCDFreqType = SV_DCLK_48HZ;
                bOutFrameRate = 48;				
            }
			else 
			#endif
			if(bRefreshRate < SUPPORT_IN_LOW_FRAME_RATE)
            { 
                if(DRVCUST_PanelGet(ePanel242530pOutput) == 0)
				bRefreshRate = bRefreshRate *2;
			   
                u4Clk = PANEL_GetPixelClk60Hz()/60 * (bRefreshRate*2);
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 60;				
            }
			else        
            if (bRefreshRate < 55) 
            {
                u4Clk = PANEL_GetPixelClk50Hz()/50 * bRefreshRate;
                bLCDFreqType = SV_DCLK_50HZ;
                bOutFrameRate = 50;
            }
            else
            {
                u4Clk = PANEL_GetPixelClk60Hz()/60 * bRefreshRate;
                bLCDFreqType = SV_DCLK_60HZ;
                bOutFrameRate = 60;
            }
        }        

			LOG(3, "u4Clk=%d,bLCDFreqType=%d,bOutFrameRate=%d)\n",u4Clk,bLCDFreqType,bOutFrameRate);


        #ifdef DRV_SUPPORT_EXTMJC
        if ((IS_COMPANION_CHIP_ON()) && (u4Clk < DCLK_MAX) && (u2InVTotal > 0) && (u4LockOthers))
        {
            // ignore DLCK_MIN checking when connecting with companion chip
            LOG(3, "DDDSLCDSetting: ignore DLCK_MIN/DCLK_MAX checking when connecting with companion chip\n");
            bLCDFreqType = SV_DCLK_OTHERS;
            bOutFrameRate = bRefreshRate;
        }
        else
        #endif        
        if ((u4Clk < DCLK_MAX) && (u4Clk > DCLK_MIN) && (u2InVTotal > 0) && (u4LockOthers))
        {  
            bLCDFreqType = SV_DCLK_OTHERS;
            if ((bRefreshRate < 77) && PANEL_IsSupport120Hz())
            {
                bOutFrameRate = bRefreshRate * 2;
            }
            else
            {
                bOutFrameRate = bRefreshRate;
            }
        }
        else
        {
            LOG(3, "vDDDSLCDSetting - clock protect u4Clk=%d DCLK_MAX = %d, DCLK_MIN =%d \n",u4Clk,DCLK_MAX,DCLK_MIN);
            // Cannot turn on DDDS
            bDDDSOn = SV_FALSE;
        }

        if(DRVCUST_PanelGet(ePanel242530pOutput) != 0)
        {
            if(bRefreshRate == 24 || bRefreshRate == 25 || bRefreshRate == 30)
            {
                bOutFrameRate = bRefreshRate;
				LOG(3, "support ePanel_242530pOutput mode\n");
            }
        }		
    }

    if ((bRefreshRate == 24) || (bRefreshRate == 25) || (bRefreshRate == 30))
    {
        #if SUPPORT_DDDS_ON_24P_25P_30P
        if (bDrvVideoIsSrcInterlace(SV_VP_MAIN))
        #endif
        {
            bDDDSOn = SV_FALSE;
        }
    }

    vDrvLCDFreqSet(bLCDFreqType, bOutFrameRate);

    // ---------DDDS Settings------------
    if (bDDDSOn ==SV_TRUE && _eSyncLockMode != eSYNCLOCK_OFF)
    {
        vDrvSetMTKGoodDclk(u2InVTotal, bRefreshRate);
		vFrameTrackConfig(1);
        if (fgIsMainTvd3d())
        {
            _fgFRS_ENA = SV_ON;    //Delay for VCR mode: Only for TVD
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
    else
    {
        _fgFRS_ENA = SV_OFF;
        vDrvSwitchMTKGoodDclk(SV_OFF);
    }

    LOG(3, "DDDSLCDSetting: bRefreshRate(%d), bLCDFreqType(%d), bDDDSOn(%d)\n", bRefreshRate, bLCDFreqType, bDDDSOn);
}

void vDddsSettingCheck(void)
{

#if CC_OSD_MUTE_WA
    //channel change will reset out region
    if (_bDddsDelayFlag && (!(_fgAutoSearch || SRM_IsEpgMode())) && SRM_IsMainVideoStrictStable())
    {
        vDDDSLCDSetting(wDrvVideoGetVTotal(SV_VP_MAIN), bDrvVideoGetRefreshRate(SV_VP_MAIN));
        // reset non-standard signal status
        _u4NSDHTotal=0xff;
        _u4NSDVTotal=0xff;
        _u4NSDFrameRate=0xff;
        _bDddsDelayFlag = 0 ;
        vSetScposFlg(MAIN_DISP_PRM_CHG);

        #ifdef DRV_SUPPORT_EXTMJC
        vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
        #endif
		#if defined(CC_MT5396)      
		vIO32MJCWriteFldAlign(MJC_MISC_00, SV_ON , MJC_MODE_CHG_TRIG_AGAIN);
		#endif
    }
#endif

    if ((_fgFRS_ENA == SV_ON) && fgIsMainTvd3d() 
        && (_eSyncLockMode == eSYNCLOCK_IGNORED))  	// Switch MTK Good Clock
    {
        if (bhwVCRSetting||(_na_state == 0x03)) //||!_fgHLOCK) //When VCR mode or Signal unsatble will OFF
        {
            vDrvSwitchMTKGoodDclk(SV_OFF);
        }
        else
        {
            vDrvSwitchMTKGoodDclk(SV_ON);
        }
    }
}

void vDrvSetAllMute(UINT8 bOnOff)
{
    vRegWriteFldAlign(MUTE_00, bOnOff,  R_MUTE_POST_EN);
}

#ifdef CC_SCPOS_3DTV_SUPPORT 
 
static void updatePanelAttribute(UINT16 u2HTotal,
                                 UINT16 u2VTotal,
                                 UINT16 u2Width,
                                 UINT16 u2Height,
                                 UINT16 u2FrameRate)
{
    PANEL_SetPanelWidth(u2Width);
    PANEL_SetPanelHeight(u2Height);

    if(u2FrameRate >= 96)
    {
        PANEL_SetVClkMax(122);
    }
    else
    {
        PANEL_SetVClkMax(61);
    }

    if(u2FrameRate == 25 || u2FrameRate == 50 || u2FrameRate == 100)
    {
        PANEL_SetHTotal50Hz(u2HTotal);
        PANEL_SetVTotal50Hz(u2VTotal);
    }
#ifdef SUPPORT_PANEL_48HZ
    else if(u2FrameRate == 48 || u2FrameRate == 96)
    {
        PANEL_SetHTotal48Hz(u2HTotal);
        PANEL_SetVTotal48Hz(u2VTotal);
    }
#endif
    else 
    {
        PANEL_SetHTotal60Hz(u2HTotal);
        PANEL_SetVTotal60Hz(u2VTotal);
    }

    return;
}

static UINT16 au2OrigHTotal[3];
static UINT16 au2OrigVTotal[3];
static UINT16 u2OrigWidth, u2OrigHeight;
static UINT32 au4OrigPixelMinMax[2];
static UINT8 u1OrigVClkMax; 

static UINT32 u43DModeDclk = 0;
static UINT8 u13DModeRefreshRate = 0;
static void vSavePanelAttribute(void)
{
    u2OrigWidth = PANEL_GetPanelWidth();
    u2OrigHeight = PANEL_GetPanelHeight();

    au2OrigHTotal[0] = PANEL_GetHTotal60Hz();
    au2OrigVTotal[0] = PANEL_GetVTotal60Hz();

    au2OrigHTotal[1] = PANEL_GetHTotal50Hz();
    au2OrigVTotal[1] = PANEL_GetVTotal50Hz();

#ifdef SUPPORT_PANEL_48HZ
    au2OrigHTotal[2] = PANEL_GetHTotal48Hz();
    au2OrigVTotal[2] = PANEL_GetVTotal48Hz();
#endif

    au4OrigPixelMinMax[0] = PANEL_GetPixelClkMin();
    au4OrigPixelMinMax[1] = PANEL_GetPixelClkMax();
    u1OrigVClkMax = PANEL_GetVClkMax();
    
}

static void vDrvDisp3DModeCB(void)
{
    S_TDTV_OSTG_CONFIG rOstgCnf;
    //UINT16 u2FrameRate; 

    rOstgCnf = eDrvTDTVOSTGModeQuery();
    if(rOstgCnf.eOSTGCfg == E_TDTV_OSTG_CFG_BY_PANEL_TBL)
    {
        //resotred the panel attributes
#ifdef PANEL_SUPPORT_48HZ
        updatePanelAttribute(au2OrigHTotal[2],
                             au2OrigVTotal[2],
                             u2OrigWidth,
                             u2OrigHeight,
                             48);
#endif
        updatePanelAttribute(au2OrigHTotal[1],
                             au2OrigVTotal[1],
                             u2OrigWidth,
                             u2OrigHeight,
                             50);
        updatePanelAttribute(au2OrigHTotal[0],
                             au2OrigVTotal[0],
                             u2OrigWidth,
                             u2OrigHeight,
                             60);

        u43DModeDclk = 0;
        PANEL_SetPixelClkMin(au4OrigPixelMinMax[0]);
        PANEL_SetPixelClkMax(au4OrigPixelMinMax[1]);
        PANEL_SetVClkMax(u1OrigVClkMax);
    }
    else
    {
        updatePanelAttribute(rOstgCnf.u4HTotal,
                             rOstgCnf.u4VTotal,
                             rOstgCnf.u4Width,
                             rOstgCnf.u4Height,
                             rOstgCnf.u4FrameRate);

        u43DModeDclk = rOstgCnf.u4Clk;
        u13DModeRefreshRate = rOstgCnf.u4FrameRate;

        // clock range to cover 24/25/30Hz output clock
        PANEL_SetPixelClkMin(50000000);
    }
    vDrvOutputStageInit();
    vDrvLVDSConfig();	
    vErrorHandleSetByPanel();
    vDrvSetLCDTiming();
    #ifdef DRV_SUPPORT_EXTMJC
    vDrvExtmjcRedoTwoChipConnection( SV_VP_MAIN, SV_TRUE);
    #endif
#ifdef DRV_SUPPORT_MT8283
    fgForceChangeRegion = TRUE;
#endif
    return;
}

UINT32 u4DrvGetDisp3DModeDclk(void)
{
    return u43DModeDclk;
}

UINT8 u1DrvGetDisp3DModeRefreshRate(void)
{
    return u13DModeRefreshRate;
}

#endif

#endif

#ifdef CC_FAST_INIT
void vDrvDisplayInit_pm_resume(void)
{
  bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;
  VOPLL_pm_resume();
  #if SUPPORT_DISPLAY
  printf("[SA7]vApiHwInit\n");	
  vDrvLVDSInit();
  vDrvDisplayInit();
  LOG(0, "Display Init");
  #endif
}
#endif

#ifdef SUPPORT_VESTEL_FLASH_PANEL
extern UINT32 u4VestelFlashPNLIndex;
#endif
void SetPanel(UINT32 i4panelid)
{
    UINT16 ui2_width,ui2_height;
	#ifndef SUPPORT_VESTEL_FLASH_PANEL
    	WritePanelIndexToEeprom(i4panelid);
	#else
		u4VestelFlashPNLIndex = i4panelid;
	#endif

    fgTestmode = TRUE;
	LoadPanelIndex();

	#ifndef ANDROID
	OSD_PLA_Enable(1, FALSE);
	#endif
	    
	#ifdef TWO_OSD_PLANE
	OSD_PLA_Enable(2, FALSE);
	#endif

	// turn off panel
    vApiPanelPowerSequence(FALSE);

    //vApiHwInit();
    //Printf("[SA7] Pmx_Cmd \n");
    //vDDDSInit();    
#ifdef CC_MT5396
    if(PANEL_IsSupportVb1()==FALSE)
#endif
    {
        vDrvLVDSInit();
    }
	//vDrvDisplayInit();
    vDrvOutputStageInit();
    // vDrvLVDSConfig();
    vDrvVOFreqSet(
        (vDrvGetLCDFreq()==25 || vDrvGetLCDFreq()==50 || vDrvGetLCDFreq()==100)?
            (PANEL_GetPixelClk50Hz()):
#ifdef SUPPORT_PANEL_48HZ
            (vDrvGetLCDFreq()==24 || vDrvGetLCDFreq()==48)?
            (PANEL_GetPixelClk48Hz()):
#endif
            (PANEL_GetPixelClk60Hz()));
    vDrvSetLCDTiming();
    vErrorHandleSetByPanel();
    OSTG_OnOutputVSync();
    IGNORE_RET(OSD_UpdateTiming());
    IGNORE_RET(OSD_Base_SetHStart());

    vApiPanelPowerSequence(TRUE);

    DumpPanelAttribute(PANEL_DUMP_CURRENT);
    
#ifdef CC_MT5396
    if(PANEL_IsSupportVb1()==FALSE)
#endif
    {
        //LVDS 7->4 FIFO enable 
        vDrvLVDS7To4FifoEnable();
    }
    //od init after change panel
    vDrvODInit();

	x_thread_delay(200);
    OSD_SetFlipMirror(FALSE, FALSE);

	ui2_width =PANEL_GetPanelWidth();
	ui2_height =PANEL_GetPanelHeight();
	OSD_SC_SetDstSize(OSD_SCALER_2,ui2_width,ui2_height);
	OSD_SC_SetDstSize(OSD_SCALER_3,ui2_width,ui2_height);
	ui2_width =((ui2_width + 7)>>3)<<3;
	ui2_height =((ui2_height + 7)>>3)<<3;
	vDrvTTDSetPicSize(ui2_width,ui2_height);
	vSetMainFlg(MAIN_FLG_MODE_CHG);
	vSetMainFlg(MAIN_FLG_MODE_DET_DONE);

	#ifndef ANDROID
	OSD_PLA_Enable(1, TRUE);
	#endif
	    
	#ifdef TWO_OSD_PLANE
	OSD_PLA_Enable(2, TRUE);
	#endif
}

LINT_RESTORE
