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
 * $RCSfile: drv_scaler_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _DRV_SCPOS_C_
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#include "x_hal_arm.h"
#endif
#include "x_hal_5381.h"
#include "x_printf.h"
#include "general.h"
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_dvi.h"
#include "hw_ckgen.h"
#include "hw_ycproc.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "drv_di.h"        
#include "drv_scpos.h"
#include "drv_video.h"
#include "drv_dram.h"
#include "drv_upscaler.h"
#include "drv_display.h"
#include "video_def.h"
#include "drvcust_if.h"
#include "srm_drvif.h"
#include "mute_if.h"
#include "scpos_debug.h"
#include "util.h"
#include "dram_map.h"
#include "source_table.h"
#include "source_select.h"
#include "vdo_misc.h"
#include "tve_if.h"

#ifdef TIME_MEASUREMENT
#include "x_time_msrt.h"
#include "u_time_msrt_name.h"
#endif
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// in 5365/5395, min dram unit is 16
#define CC_MINIMUN_DRAM_RW_UNIT 16


/**************************************************************************
 * Global/Static variables
 *************************************************************************/

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 _u4MainMuteMask = 0;
UINT32 _u4PipMuteMask = 0;
UINT32 _au4HwMuteStatus[2] = {0, 0};
#endif

#ifdef CC_COPLAT_MT82
BOOL _fgPIP,_fgLRPOP;
#endif

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];

#ifdef DYNAMIC_RES_VERIFY
EXTERN UINT8 u1EnDynRes;
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _vScpipInitSyncAdj(void);
static void _vScpipInitPanelProtect(void);
static void _vScpipInitPOCLK(void);
static void _vScpipInitImportProtection(void);
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
EXTERN void vScpipInitFilterOff(void);
EXTERN void vScpipInitDsWaterCtrl(void);
EXTERN void vScpipInitUsWaterCtrl(void);

#if SUPPORT_HDTV_HARDWARE_MUTE
static void vOSTGLVDSHWMute(UINT8 mode)
{
    vRegWriteFldAlign(MUTE_00, mode, R_MUTE_POST_SEL);
}

void vOSTGVideoPlaneHWMute(UINT8 bPath,UINT8 mode)
{
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(MUTE_00, mode, R_M_MUTE_FRONT_SEL);
    }
    else if(bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(MUTE_00, mode, R_P_MUTE_FRONT_SEL);
    }
}

void vScpipConfigHardwareMute(void)
{
    UINT8 u1Dispmode;
    E_QUICK_BLANK_MODE eMode = QUICK_BLANK_NO_FUNCTION;
    
    u1Dispmode = u1ScpipGetDispMode(SV_VP_MAIN);

    if(u1Dispmode==VDP_SCPOS_DISPMODE_AUTO || u1Dispmode == VDP_SCPOS_DISPMODE_MANUAL || u1Dispmode ==VDP_SCPOS_LINESYNC_DISPMODE)
    {
        //only enable in YPbPr and VGA
        if (((bGetSignalType(SV_VP_MAIN) == SV_ST_YP) || (bGetSignalType(SV_VP_MAIN) == SV_ST_VGA) || (bGetSignalType(SV_VP_MAIN) == SV_ST_DVI)))
        {
            eMode = QUICK_BLANK_BY_MAIN;
        }
        else
        {
            eMode = QUICK_BLANK_NO_FUNCTION;
        }
    
    }

    if (eMode == QUICK_BLANK_NO_FUNCTION)
    {
        //disable LVDS quick blank mute
        vOSTGLVDSHWMute(SV_OFF);
        //enable vdp quick mute
        vOSTGVideoPlaneHWMute(SV_VP_MAIN, SV_ON);
    }
    else 
    {
        //enable LVDS quick blank mute
        vOSTGLVDSHWMute(SV_ON);
        //disable vdp quick mute
        vOSTGVideoPlaneHWMute(SV_VP_MAIN, SV_OFF);
    }
}
#endif

/**
 * @brief u4ScpipGetMuteMask

 * Return Main And PIP Mute Mode To fgApiVideoStable

 * @param  bMode
 * @retval UINT32
 */
 #if CC_SCPOS_MUTE_RELATCH_WA
UINT32 u4ScpipGetMuteMask(UINT8 bMode)
{
    if (bMode==0)
    {
        return _u4MainMuteMask;
    }
    else
    {
        return _u4PipMuteMask;
    }
}
#endif

/**
 *  @6896 porting done
 */
void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff)
{
    if(_arScalePrm[bPath].u4MirrorEnable != bOnOff)
    {
        _arScalePrm[bPath].u4MirrorEnable = bOnOff;
    }
}

/**
 *  @6896 porting done
 */
void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff)
{
    if(_arScalePrm[bPath].u4FlipEnable != bOnOff)
    {
        _arScalePrm[bPath].u4FlipEnable = bOnOff;
    }
}

/**
 *  @6896
 */
void vScpipSetMirrorFlipAll(UINT8 bOnOff)
{
    vScpipSetMirror(SV_VP_MAIN, bOnOff);
    vScpipSetMirror(SV_VP_PIP, bOnOff);

    vScpipSetFlip(SV_VP_MAIN, bOnOff);
    vScpipSetFlip(SV_VP_PIP, bOnOff);
}

#if defined(CC_MT5396)
#if defined(SUPPORT_TV3D_OVERSCAN)
UINT16 u4ScpipGetMJC3DOverScanHiLimitation(void)
{
    UINT16 u2MJCVTotalMax;
    UINT8 u1MJC3D, u1MJC3DIn ,u1MJC3DOut;
    u1MJC3D = u4DrvTDTVMJCModeQuery();
    u1MJC3DIn = TD_MJC_IN(u1MJC3D);
    u1MJC3DOut = TD_MJC_OUT(u1MJC3D);
    LOG(1,"+++u1MJC3DIn:%d u1MJC3DOut:%d \n",u1MJC3DIn,u1MJC3DOut);
    
    if (MJC_READ_FLD(MJC_FB_01, MJC_DRAM_RPT) == SV_TRUE)
    {
        return 0xffff;
    }
    if ((u1MJC3DOut == E_TD_OUT_3D_FS) || (u1MJC3DOut == E_TD_OUT_3D_FPR) || ((u1MJC3DOut == E_TD_OUT_3D_LI) && IS_PANEL_PDP))
    {
        u2MJCVTotalMax = 2 * ALIGN_16_CEIL(wDISPLAY_HEIGHT);
        #if 0
        if (u1MJC3DIn == E_TD_IN_TB_P)
        {
            u2MJCVTotalMax = 4 * ALIGN_16_CEIL(wDISPLAY_HEIGHT);
        }
        else if (u1MJC3DIn == E_TD_IN_SBS_P)
        {
            u2MJCVTotalMax = 2 * ALIGN_16_CEIL(wDISPLAY_HEIGHT);
        }
        else
        {
            u2MJCVTotalMax = 0xffff;
        }
        #endif
    }
    else  if (u1MJC3DOut == E_TD_OUT_3D_LI)
    {
        if (u1MJC3DIn == E_TD_IN_TB_P)
        {
            u2MJCVTotalMax = 2 * ALIGN_16_CEIL(wDISPLAY_HEIGHT);
        }
        else if (u1MJC3DIn == E_TD_IN_SBS_P)
        {
            u2MJCVTotalMax = ALIGN_16_CEIL(wDISPLAY_HEIGHT);
        }
        else
        {
            u2MJCVTotalMax = 0xffff;
        }
    }
    else
    {
        u2MJCVTotalMax = 0xffff;
    }
    
    LOG(1,"+++u4MJCVTotalMax:%d \n",u2MJCVTotalMax);
    return u2MJCVTotalMax;
}
#endif
UINT32 u4ScpipGetMinSrcRegionHeight(UINT8 u4VdpId)
{
	UINT64 ret;
	UINT64 u4InFrameRate, u4InVtotal;
	UINT64 u4OutHtotal, u4Poclk;
	UINT64 u4VUpFactor;
	UINT64 u4Dividend, u4Divisor;

    UINT16 u2MaxVTotal;
    UINT16 inputHeight;
    #if defined(SUPPORT_TV3D_OVERSCAN)
    UINT16 u2MJCMaxVTotal;
    UINT32 u4MinVup_MJC;
    UINT32 u4FscMaxDisplyWindow;
    #endif    
    UINT32 u4MinVactive;
    UINT32 u4PscDispalyWindow;
    UINT32 u4MinSrcRegion = 0;
    RPicInfo* picInfo;
    u4MinVactive = 0; 
    picInfo = getPicInfo(SV_VP_MAIN);
    inputHeight = wDrvVideoInputHeight(u4VdpId);

    u4PscDispalyWindow  = IO32ReadFldAlign(PSPIP1_02, PSPIP1_02_RVL_1);
	ret = 0;
	
	if(u4VdpId != SV_VP_MAIN || getScalerMode(u4VdpId) != VDP_SCPOS_LINESYNC_DISPMODE)
	{
		return (UINT32)ret;
	}
	//case 1
	/* the time Front scaler output n (n = 0x8000/ V UP factor ) pixel should be less than input one pixel
	  * line sync mode limitation: ( Input Htotal / Input CLK ) >= ( Front SC: Output Htotal / Output CLK ) * ( 0x8000 / V UP factor )
	  *               => ( 1 / ( Input Vtotal * Input Frame rate ) ) >= ( Front SC: Output Htotal / Output CLK ) * ( 0x8000 / V UP factor )
	  */
	u4InFrameRate = bDrvVideoGetRefreshRate(u4VdpId);
	u4InVtotal = wDrvVideoGetVTotal(u4VdpId);

	u4OutHtotal = RegReadFldAlign(SCPIP_PIP_OA_00, PIP_OA_00_PANEL_HTOTAL);
	u4Dividend = IO32ReadFldAlign(CKGEN_POCLK_DPLL_DIVCFG, FLD_POCLK_DPLL_IDEAL_PLL_M);
	u4Divisor = IO32ReadFldAlign(CKGEN_POCLK_DPLL_DIVCFG, FLD_POCLK_DPLL_IDEAL_PLL_N);
	
	u4Poclk = u8Div6464(337 * 1000000 * u4Dividend, u4Divisor, NULL);
	u4VUpFactor = u8Div6464(DFT_US_FACTOR * u4InVtotal * u4InFrameRate * u4OutHtotal, u4Poclk, NULL);
	ret = u8Div6464(VDP_MAX_REGION_HEIGHT * u4VUpFactor, DFT_US_FACTOR, NULL);
    LOG(1,"case 1(the time Front scaler output n pixels should be less than input one pixel)and MinSrcRegion(%d)\n",ret);
    LOG(0, "Div(%x %x) In(fr: %d, vt: %d) Out(ht: %d, Poclk: %d) Vfac(%d) ret(%d)\n",
			(UINT32)u4Dividend, (UINT32)u4Divisor,
			(UINT32)u4InFrameRate, (UINT32)u4InVtotal,
			(UINT32)u4OutHtotal, (UINT32)u4Poclk,
			(UINT32)u4VUpFactor,
			(UINT32)ret);	

    #if defined(SUPPORT_TV3D_OVERSCAN)
    //case 2
    //MJC input vtotal limitation case    
    u2MJCMaxVTotal = u4ScpipGetMJC3DOverScanHiLimitation();
    u4MinVup_MJC = ((UINT32)wDrvVideoGetVTotal(u4VdpId)* DFT_US_FACTOR)/u2MJCMaxVTotal;
    u4FscMaxDisplyWindow = ((UINT32)wDrvVideoInputHeight(u4VdpId) * DFT_US_FACTOR)/u4MinVup_MJC; 
   // if(E_TDTV_UI_3D_MODE_OFF!=eDrvTDTVGetUIPacking(u4VdpId))
    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
       u4MinSrcRegion = (VDP_MAX_REGION_HEIGHT * u4PscDispalyWindow)/u4FscMaxDisplyWindow;
       LOG(1,"case 2(MJC Vtotal limitation): 3d on and MinSrcRegion(%d)\n",u4MinSrcRegion);
    }
    else
    { 
        u4MinSrcRegion = (VDP_MAX_REGION_HEIGHT * u4MinVup_MJC)/DFT_US_FACTOR;
        LOG(1,"case 2(MJC Vtotal limitation): 3d off and MinSrcRegion(%d)\n",u4MinSrcRegion);
    }
    #endif
	
   //case 3 
   //poclk should not larger than 337.5
    u2MaxVTotal = (337*1000000)/(((UINT16)u4OutHtotal)*bDrvVideoGetRefreshRate(SV_VP_MAIN));
    if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_HALF)
    {
        u2MaxVTotal = u2MaxVTotal>>1;
    }
    else if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_DOUBLE)
    {
        u2MaxVTotal = u2MaxVTotal<<1;
    } 
    u4MinVactive = (UINT32)(wDrvGetOutputVTotal() * picInfo->wSrcH)/(2*u2MaxVTotal - wDrvGetOutputVTotal() - 8);
    u4MinVactive= u4MinVactive* VDP_MAX_REGION_WIDTH/inputHeight;
    LOG(1,"case 3(Poclk should not larger than 337.5MHz) and MinSrcRegion(%d)\n",u4MinVactive);

	u4MinVactive = (UINT32)(MAX(ret,u4MinVactive));
    u4MinSrcRegion = MAX(u4MinVactive,u4MinSrcRegion);
    
	return u4MinSrcRegion;
}
#endif

/**
 *  @6896 No change
 */ 
UINT32 u4ScpipGetMinSrcRegionWidth(UINT8 u4VdpId)
{
    UINT32 ret;
    UINT16 inputWidth;
    #if defined(CC_MT5396)// need to remove this to u4ScpipGetMinSrcRegionHeight() after AP change
    UINT8 bMode;
    UINT32 u4MinVactive;
    #endif
    
    ret = 0;
    inputWidth = wDrvVideoInputWidth(u4VdpId);
    
    #if defined(CC_MT5396)//// need to remove this to u4ScpipGetMinSrcRegionHeight() after AP change
    u4MinVactive = 0;    
    bMode =getScalerMode(u4VdpId);
    
    if(bMode == VDP_SCPOS_LINESYNC_DISPMODE)
    {
        UINT16 u2PanelHTotal;
        UINT16 u2MaxVTotal;
        UINT16 inputHeight;
        RPicInfo* picInfo;
        
        picInfo = getPicInfo(SV_VP_MAIN);
        inputHeight = wDrvVideoInputHeight(u4VdpId);
        
        u2PanelHTotal = wDISPLAY_WIDTH + 150;
        u2MaxVTotal = (337*1000000)/(u2PanelHTotal*bDrvVideoGetRefreshRate(SV_VP_MAIN));
        if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_HALF)
        {
            u2MaxVTotal = u2MaxVTotal>>1;
        }
        else if(eScpipGetTdtvResolutionType(SV_VP_MAIN, SCALER_TDTV_RES_POSITION_PST_V) == E_TD_RES_DOUBLE)
        {
            u2MaxVTotal = u2MaxVTotal<<1;
        }  
        //u4MinVactive = (wDrvGetOutputVTotal() * picInfo->wSrcH)/(u2MaxVTotal - wDrvGetOutputVTotal()/2 - 4)/2 ;
        u4MinVactive = (UINT32)(wDrvGetOutputVTotal() * picInfo->wSrcH)/(2*u2MaxVTotal - wDrvGetOutputVTotal() - 8);
        u4MinVactive= u4MinVactive* VDP_MAX_REGION_WIDTH/inputHeight;
        LOG(3,"in u4ScpipGetMinSrcRegionWidth and u4MinVactive,u2MaxVTotal(%d,%d)\n",u4MinVactive,u2MaxVTotal);
    }
    #endif
    if (inputWidth == 0)
    {
        inputWidth = wDISPLAY_WIDTH; // FIXME
    }
    if (bIsScalerInput444((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE)     // 422 Mode
    {
        ret = (MAX((CC_MINIMUN_DRAM_RW_UNIT*6), u4DrvDIMiniInputWidth(u4VdpId))*VDP_MAX_REGION_WIDTH) /inputWidth;
    }
    else // 444 Mode
    {
        ret = (MAX(((CC_MINIMUN_DRAM_RW_UNIT*16+2)/3), u4DrvDIMiniInputWidth(u4VdpId))*VDP_MAX_REGION_WIDTH) /inputWidth;
    }
    
    if(ret > VDP_MAX_REGION_WIDTH)//Not support zoom when input width < CC_MINIMUN_DRAM_RW_UNIT*6 or CC_MINIMUN_DRAM_RW_UNIT*16/3
    {
        return VDP_MAX_REGION_WIDTH;
    }
    #if defined(CC_MT5396)//// need to remove this to u4ScpipGetMinSrcRegionHeight() after AP change
    ret = MAX(ret,u4MinVactive);
    #endif
    return ret;
}

//@6896 remove. Dram module is all different with 5395
UINT32 u4ScpipDirectSetDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr)
{
    return 0;
}

/**
 *  @6896 porting done
 */
void vScpipSetWPos(UINT32 u4VdpId, BOOL fgOnOff, UINT8 u1Index)
{  
    if(u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_12, fgOnOff, PIP_OA_12_SET_W_POS_EN_1);
        vRegWriteFldAlign(SCPIP_PIP_OA_12, u1Index, PIP_OA_12_SET_W_POS_1);
    }
    else if(u4VdpId == VDP_2)
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_13, fgOnOff, PIP_OA_13_SET_W_POS_EN_2);
        vRegWriteFldAlign(SCPIP_PIP_OA_13, u1Index, PIP_OA_13_SET_W_POS_2);    
    }
}

/**
 *  @6896 porting done
 */
UINT32 u4ScpipCheckFifoOverflow(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset;
    UINT8 fOverflow;
    UINT8 fUnderflow;

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);

    if(u1ScpipGetDispMode(bPath) != VDP_SCPOS_DISPMODE_OFF)
    {
        LOG(6, "FIFO overflow/underflow flag is not supported in display mode\n");
        return 4; // not supported in display mode
    }
    else
    {
        fOverflow = RegReadFldAlign(SCPIP_DRAM_M_00 + dramOffset,DRAM_M_00_DA_OVERFLOW_CLR);
        fUnderflow = RegReadFldAlign(SCPIP_DRAM_M_00 + dramOffset,DRAM_M_00_DA_UNDERFLOW_CLR); 
        return ((fUnderflow<<1) | fOverflow);
    }    
}

/**
 *  @6896 porting done
 */
void vScpipResetFifoOverflow(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset;

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 1, DRAM_M_00_DA_OVERFLOW_CLR);
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 1, DRAM_M_00_DA_UNDERFLOW_CLR);    
    x_thread_delay(20);
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 0, DRAM_M_00_DA_OVERFLOW_CLR);
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 0, DRAM_M_00_DA_UNDERFLOW_CLR);
}

/**
 * @brief Get pre-scaling down factor
 * @param bPath VDP_1/VDP_2
 * @return pre-scaling down factor
 * @6896 porting done
 */
UINT32 u4ScpipGetHwHPS(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        return RegReadFldAlign(SCPIP_HDS_03, HDS_03_HDS_MAIN_SCALER);
    }
    else
    {
        return RegReadFldAlign(SCPIP_HDS_04, HDS_04_HDS_PIP_SCALER);
    }
}

/**
 * @brief Set pre-scaling down factor
 * @param u4VdpId VDP_1/VDP_2
 */
void vScpipSetHwHPSFactor(UINT32 u4VdpId, UINT32 u4Factor)
{        
    UINT32 u4HpsFactor; 

    // defaul value:0x8000 means auto init phase, no need to set the phase again.
    //u4HInitValue = DFT_HPD_FACTOR - u4Factor;
    u4HpsFactor = u4Factor;

#if SUPPORT_POP
    if (u4VdpId == VDP_2)
    {
        vRegWriteFldAlign(SCPIP_HDS_04, (UINT16)u4HpsFactor, HDS_04_HDS_PIP_SCALER);
        vRegWriteFldAlign(SCPIP_HDS_04, (UINT16)(0x8000-u4HpsFactor), HDS_04_HDS_PIP_OFST);
    }
    else
#endif
    {
        vRegWriteFldAlign(SCPIP_HDS_03, (UINT16)u4HpsFactor, HDS_03_HDS_MAIN_SCALER);
        vRegWriteFldAlign(SCPIP_HDS_03, (UINT16)(0x8000-u4HpsFactor), HDS_03_HDS_MAIN_OFST);
    }
}


void vScpipHwInitOnPanelInit(UINT8 bPath)
{
   if (bPath == SV_VP_MAIN)
    {
        _vScpipInitSyncAdj();
        _vScpipInitPanelProtect();

    }
    else
    {
        vScpipSetImportOnOff(bPath, SV_OFF);
        vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
        vRegWriteFldAlign(SCPIP_PIP2_03, 0, PIP2_03_RHL_2);
        vScpipSetImportOnOff(bPath, SV_ON);       
    }
}

//-----------------------------------------------------------------------------
/** Brief of vScpipHwInit.
 * Initial SCPOS related setting
 * @param  bPath
 * @retval void
 * @6896 porting done
 */
//-----------------------------------------------------------------------------
void vScpipHwInit(UINT8 bPath)
{
    UINT16 pipOffset;
    UINT16 importOffset;
    UINT16 pipOAOffset;
    UINT16 dramOffset;

    pipOffset = getPIPOffset(bPath);  
    importOffset = getImportOffset(bPath);
    pipOAOffset = getPIPOAOffset(bPath);
    dramOffset = getDramOffset(bPath);
  
    //Select import method
    vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_SEL_1);
    
    //Enable H Boundary protection
    vRegWriteFldAlign(SCPIP_PIP1_1B+pipOffset, 1, PIP1_1B_BOUNDARY_H_EN_1);
    
    //Reset write enable to off state in initialization
    vScpipSetDramWrite(bPath, SV_OFF);
        
    if (bPath == SV_VP_MAIN)
    {
        vScpipInitDsWaterCtrl();

        vScpipInitUsWaterCtrl();        

        //init water control option. When set to 1, scaler will start to fill line buffer to init_thr once pscan or mjc return sync signal.
        vRegWriteFldAlign(SCPIP_PIP1_13, 1, PIP1_13_WATER_INI_OPTION2_1);

        #if SUPPORT_HDTV_HARDWARE_MUTE
        vScpipConfigHardwareMute();
        #endif

        /* Always set filter off to 1. 
         * Under this setting, HW will bypass FIR filter when scaling factor is 0x8000 and 
         * will auto turn on FIR filter when scaling factor isn't 0x8000.
         */
        vScpipInitFilterOff();
		
        // non-linear down scaling mode: set default to new mode
        //@6896 review this
        vRegWriteFldAlign(SCPIP_DS1_0A, 1, DS1_0A_NONL_H_OPT_1);
        
        // v boundary enable
        vRegWriteFldAlign(SCPIP_US1_1D, 1, US1_1D_UP_VBOUND_EN1);
               
        _vScpipInitSyncAdj();

        _vScpipInitPanelProtect();
        //init poclk
        _vScpipInitPOCLK();
        //init import protection parameter
        _vScpipInitImportProtection();

        vRegWriteFldAlign(SCPIP_DS1_02, 0, DS1_02_VSYNC_OPT_1);        

        #if defined(CC_MT5389)
        vDrvUsPeiBypass(SV_ON); //5389 don't have PE
        #else
        vDrvUsPeiBypass(SV_OFF);        
        #endif

        #if defined(SUPPORT_3D_OVSN_BY_SCL)
        vRegWriteFldAlign(SCPIP_SYSTEM_01, 1, SYSTEM_01_TV3D_IN_INV);
        #endif
    }
    else
    {
        #if SUPPORT_HDTV_HARDWARE_MUTE
        //enable sub path quick off function by default
        vOSTGVideoPlaneHWMute(SV_VP_PIP,SV_ON);
        #endif
        
        /* initially hide sub window output region */
        vScpipSetImportOnOff(bPath, SV_OFF);
        vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
		vRegWriteFldAlign(SCPIP_PIP2_03, 0, PIP2_03_RHL_2);
        vScpipSetImportOnOff(bPath, SV_ON);       
        // v boundary enable
        vRegWriteFldAlign(SCPIP_US2_03, 0, US2_03_UP_VBOUND_EN2);                
        
		//set sub defaule import 
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 2, SYSTEM_12_IMPORT_SEL_2); 

        vRegWriteFldAlign(SCPIP_DS2_02, 1, DS2_02_VSYNC_OPT_2); 
    }

    //turn on V MASK
    vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WR_V_MASK_EN_1);

    //set Disp_R/Field/PD_info sample time to vsync rising (ES & MP version register define is different)
    if(BSP_GetIcVersion() == IC_VER_5396_AA || 
        BSP_GetIcVersion() == IC_VER_5368_AA ||
        (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC))
    {    
        vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WR_INFO_SAMPLE_SEL_1);    
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WR_INFO_SAMPLE_SEL_1);    
    }

    //set reference v-sync to short v-sync(1 line v-sync)
    vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WPOS_REF_SYNC_1);        
    //After change reference v-sync to short one, this setting will change wpos immediately.
    vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 1, PIP_OA_12_DRAM_WPOS_REF_MODE_1);    
    vRegWriteFldAlign(SCPIP_PIP_OA_12+pipOAOffset, 0, PIP_OA_12_DRAM_WPOS_REF_NUM_1);    

    //for 5396 & 5368
    vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 0, PIP1_09_SP_PRO_1);  //always set to 0

	//for 5396 & 5368, when rhp > x ,deop a line 
	vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_AUTO_PREVOFST_ADJUST1);
	vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_AUTO_PREVOFST_ADJUST2);

	//VDS_CEN_SEL default set to 1
	vRegWriteFldAlign(SCPIP_DS1_02 + pipOffset, 1, DS1_02_VDS_CEN_SEL_1);
    //enable power down function
    vRegWriteFldAlign(SCPIP_SYSTEM_20, 1, SYSTEM_20_PDN_DSCALER);    

    //turn on snake mapping (turn off snake mapping and enable mirror will cause problem)
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 1, DRAM_M_00_DA_SNAKE_MAPPING);
}

/**
 * @brief vScpipImportOnOff 
 * Turn on/off import protection
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 * @6896 porting done
 */
void vScpipSetImportOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10, bOnOff, SYSTEM_10_IMPORT_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_12, bOnOff, SYSTEM_12_IMPORT_2);
    }    
}

void vScpipSetFSCImportOff(UINT8 bOnOff,UINT8 bPath)
{
    UINT16 importOffset;
    
    importOffset = getImportOffset(bPath);
    
    if(bOnOff == SV_ON)
    {        
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 1, SYSTEM_10_IMPORT_OFF_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_OFF_1);
    }
}

void vScpipTurnOffImportProtection(UINT8 bPath)
{
    #if defined(CC_MT5396)
    UINT8 bMode;
    #endif

    vScpipSetFSCImportOff(SV_ON,bPath);
    
    #if defined(CC_MT5396)  
    bMode = getScalerMode(bPath);
    
    if((bPath == SV_VP_MAIN) && (bMode == VDP_SCPOS_LINESYNC_DISPMODE))
    {
        vRegWriteFldAlign(SCPIP_PIP_OA_02, 1, PIP_OA_02_SET_DATA_SEL);    
        #if defined(SCPIP_SUPPORT_POST_SCALER)
        vPscSetImportOff(SV_ON);
        #endif
    }    
    #endif
}

void vScpipTurnOnImportProtection(UINT8 bPath)
{
    vScpipSetFSCImportOff(SV_OFF,bPath);
    
    #if defined(CC_MT5396)

    vRegWriteFldAlign(SCPIP_PIP_OA_02, 0, PIP_OA_02_SET_DATA_SEL);    
    #if defined(SCPIP_SUPPORT_POST_SCALER)
    vPscSetImportOff(SV_OFF);
    #endif
    
    #endif
}
void vScpipUpdateImportSetting(UCHAR ucVdpId,UCHAR ucMode)
{
    UINT8 bPath = getScalerPath(ucVdpId);
    UINT16 importOffset = getImportOffset(bPath);
    UINT16 dsOffset= getDSOffset(bPath);

    if(ucMode == VDP_SCPOS_DISPMODE_OFF)    //dram mode
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 2, SYSTEM_10_IMPORT_SEL_1);
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNC_OPT_1);//5396/68 dram agent need short vsync from down scaler. Set this reg to 1 will do this.//FIXME
        #ifdef DYNAMIC_RES_VERIFY
        if (u1EnDynRes)
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);
        }
        #endif
        if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) ||
           (BSP_GetIcVersion() >= IC_VER_5389_AB && BSP_GetIcVersion() <= IC_VER_5389_AC))
        {
            //import triggered at panel vsync.
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 1,PIP_OA_2C_IMPORT_RVSYNC_TRIG_SEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_VSYNC_SRC_SEL);
            vRegWriteFldAlign(SCPIP_SYSTEM_2C, 0,SYSTEM_2C_IMPORT_VSYNC_US_SEL);
        }
    }
    else if(ucMode == VDP_SCPOS_PSCAN_DISPMODE) //pscan dispmode
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_SEL_1);

        if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) ||
           (BSP_GetIcVersion() >= IC_VER_5389_AB && BSP_GetIcVersion() <= IC_VER_5389_AC))
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);
            //import triggered at pscan vsync.
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_RVSYNC_TRIG_SEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_VSYNC_SRC_SEL);
            vRegWriteFldAlign(SCPIP_SYSTEM_2C, 1,SYSTEM_2C_IMPORT_VSYNC_US_SEL);
        }        
        else
        {
            vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNC_OPT_1); //5396/68 dram agent need short vsync from down scaler. Set this reg to 1 will do this.//FIXME
        }
    }
    else        //for line sync dispmode
    {
        LOG(4,"Set Line sync dispmode import setting\n");
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_SEL_1);
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);
        if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) ||
           (BSP_GetIcVersion() >= IC_VER_5389_AB && BSP_GetIcVersion() <= IC_VER_5389_AC))
        {
            LOG(4,"Set Line sync dispmode ECO import setting\n");
            //import triggered at input vsync delay 8 lines.
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_RVSYNC_TRIG_SEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_VSYNC_SRC_SEL);
            vRegWriteFldAlign(SCPIP_SYSTEM_2C, 1,SYSTEM_2C_IMPORT_VSYNC_US_SEL);
        }         
    }
}

/**
 *  @6896 porting done
 */
UINT8 u1ScpipGetTv3dOutIndicator()
{
    return RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_TV3D_OUT_R_STATUS);
}

#if CC_SCPOS_MUTE_RELATCH_WA
/**
 *  @6896 No change
 */ 
UINT32 u4ScpipGetHwMuteStatus(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return 0;
    }
    return _au4HwMuteStatus[bPath];
}
/**
 *  @6896 No change
 */ 
void vScpipSetHwMuteStatus(UINT8 bPath, UINT8 u1OnOff)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }

    _au4HwMuteStatus[bPath] = u1OnOff;
}
#endif

/**
 *  @6896 porting done
 */
static void _vScpipInitSyncAdj(void)
{
    //hsync adj
    vRegWriteFldAlign(SCPIP_PIP_OA_02, HSYNC_ADJ, PIP_OA_02_HSYNC_ADJ); 
    //main active adj
    vRegWriteFldAlign(SCPIP_PIP1_06, MAIN_ACTIVE_ADJ, PIP1_06_ACTIVE_ADJ_1); 
    //main read relay
    vRegWriteFldAlign(SCPIP_PIP1_06, MAIN_READ_DELAY, PIP1_06_READ_DELAY_1);
   
    #if SUPPORT_POP   
    if ( ((BSP_GetIcVersion() >= IC_VER_5396_AB) && IS_IC_5396()) )
  	 { 
	  	 	//sub active adj for ECO
	  	 		vRegWriteFldAlign(SCPIP_PIP2_06, SUB_ACTIVE_ADJ_E4, PIP2_06_ACTIVE_ADJ_2); // PLC_DLY need to adjust
	  	 	//sub read delay for ECO
	  	 	vRegWriteFldAlign(SCPIP_PIP2_06, SUB_READ_DELAY_E4, PIP2_06_READ_DELAY_2);
     }
		 else
		 {
			 //sub active adj
			 vRegWriteFldAlign(SCPIP_PIP2_06, SUB_ACTIVE_ADJ, PIP2_06_ACTIVE_ADJ_2); 
			 //sub read delay
	   		vRegWriteFldAlign(SCPIP_PIP2_06, SUB_READ_DELAY, PIP2_06_READ_DELAY_2);
		 }
    #endif
}

/**
 *  @6896 new
 */
static void _vScpipInitPOCLK(void)
{
    #if defined(CC_MT5396)
    //1. set POCLK to divider PLL
    vScpipSetPOCLKSelReg(FLD_POCLK_SEL_DPLL);
    //2. set divider PLL source to Ethernet PLL / 2
    vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x1, FLD_POCLK_DPLL_SEL);    
    //3. set divider m/ n to 0x4000/0x8000 as default value
    vIO32WriteFldAlign(CKGEN_POCLK_DPLL_DIVCFG, 0x4000, FLD_POCLK_DPLL_IDEAL_PLL_M);
    vIO32WriteFldAlign(CKGEN_POCLK_DPLL_DIVCFG, 0x8000, FLD_POCLK_DPLL_IDEAL_PLL_N);    
    //4. set PIP POCLK_TVE to divier PLL
    vIO32WriteFldAlign(CKGEN_OCLK_TEST, 0x2, FLD_OCLK_TVE_SEL);
    #endif
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
}

static void _vScpipInitImportProtection(void)
{
    //@5396 todo: after video path import integreation is ok, remove this init
    vRegWriteFldAlign(SCPIP_HDS_05, 0, HDS_IMPORT_EN1);
    vRegWriteFldAlign(SCPIP_HDS_05, 0, HDS_IMPORT_EN2);    

    //set import w pos update timing
	vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_W_VSYNC_SEL1);
	vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_W_VSYNC_SEL2);

    //set import r pos update timing, only apply on 5396/5368 MP & 5389
    if((BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC) || 
        (BSP_GetIcVersion() >= IC_VER_5368_AB && BSP_GetIcVersion() <= IC_VER_5368_AC) ||
        (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC))
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_R_VSYNC_SEL1);
        vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_R_VSYNC_SEL2);    
    }    
    
    //@5396 todo: after video path import integreation is ok, remove this init
    vRegWriteFldAlign(SCPIP_SYSTEM_10, 1, SYSTEM_10_IMPORT_OFF_1);
    vRegWriteFldAlign(SCPIP_SYSTEM_12, 1, SYSTEM_12_IMPORT_OFF_2);
    
    //config SET_DATA also can be triggered by scaler import.
	vRegWriteFldAlign(SCPIP_PIP_OA_07, 1, PIP_OA_07_SCALER_SET_DATA_IMPORT_MODE);
}

//@6896 porting done
UINT32 u4ScpipGetDSInputHTotal(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_00 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_00_RD_DS1_INHLEN_1);
}

//@6896 porting done
UINT32 u4ScpipGetDSInputVTotal(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_00 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_00_RD_DS1_INVLEN_1);
}

//@6896 porting done
UINT32 u4ScpipGetDSInputHActive(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_01 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_01_RD_DS1_INHACT_1);
}

//@6896 porting done
UINT32 u4ScpipGetDSInputVActive(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_01 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_01_RD_DS1_INVACT_1);    
}

UINT8 bScpipIsDramSnakeMapping(UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DRAM_M_00 + getDramOffset(getScalerPath(u4VdpId)), DRAM_M_00_DA_SNAKE_MAPPING);
}

UINT8 bScpipDramWriteFrameIndex(UINT32 u4VdpId)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        return RegReadFldAlign(SCPIP_PIP_OA_RO_0A, PIP_OA_RO_0A_RD_W_POS_1);
    }
    else
    {
        return RegReadFldAlign(SCPIP_PIP_OA_RO_0A, PIP_OA_RO_0A_RD_W_POS_2);    
    }
}

UINT8 bScpipDramReadFrameIndex(UINT32 u4VdpId)
{
    if (u4VdpId == SV_VP_MAIN)
    {
        return RegReadFldAlign(SCPIP_PIP_OA_RO_0A, PIP_OA_RO_0A_RD_R_POS_1);
    }
    else
    {
        return RegReadFldAlign(SCPIP_PIP_OA_RO_0A, PIP_OA_RO_0A_RD_R_POS_2);    
    }
}

void vScpipSetTveEnable(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;

    vRegWriteFldAlign(SCPIP_PIP_OA_02, value, PIP_OA_02_TVE_EN);
}

void vScpipSetTveUpEnable(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;

    vRegWriteFldAlign(SCPIP_PIP_OA_02, value, PIP_OA_02_TVE_UP_EN);
}

void vScpipSetTvePal(UINT8 bOnOff)
{
    UINT8 value;
    value = (bOnOff==SV_ON)?1:0;

    vRegWriteFldAlign(SCPIP_PIP_OA_02, value, PIP_OA_02_TVE_PAL);    
}

void vScpipSetTveOsdPrelen(UINT8 prelen)
{
    vRegWriteFldAlign(SCPIP_PIP_OA_02, prelen, PIP_OA_02_TVE_OSD_PRELEN);
}

#if defined(CC_MT5396)
void vScpipSetPOCLKSelReg(UINT8 poclkSel)
{
    UINT8 tveEnable;
    TVE_GetEnable(TVE_1, &tveEnable);    

    if(poclkSel > FLD_POCLK_SEL_MIB_OCLK)
    {
        return;
    }
    //Set POCLK (for main path)
    vIO32WriteFldAlign(CKGEN_DISP_CKCFG, poclkSel, FLD_POCLK_SEL);    
    //if TVE is not enabled 
    if(tveEnable==SV_FALSE)
    {
        //set OCLK_TVE (for sub path)
        vIO32WriteFldAlign(CKGEN_OCLK_TEST, poclkSel, FLD_OCLK_TVE_SEL);
    }
}
#endif

UINT32 u4ScpipWriteDramFrameWidth(UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_PIP1_00 + getPIPOffset(getScalerPath(u4VdpId)), PIP1_00_DRAM_WR_WIDTH_H_1);   
}

UINT32 u4ScpipWriteDramFrameHeight(UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_PIP1_00 + getPIPOffset(getScalerPath(u4VdpId)), PIP1_00_DRAM_WR_WIDTH_V_1);   
}

