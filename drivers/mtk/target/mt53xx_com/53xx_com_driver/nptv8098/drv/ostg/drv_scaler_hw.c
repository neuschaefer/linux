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

/**
 *  @6896 No change
 */ 
UINT32 u4ScpipGetMinSrcRegionWidth(UINT8 u4VdpId)
{
    UINT32 ret;
    UINT16 inputWidth;
    #if defined(CC_MT5396)
    UINT8 bMode;
    UINT32 u4MinVactive;
    #endif
    
    ret = 0;
    inputWidth = wDrvVideoInputWidth(u4VdpId);
    
    #if defined(CC_MT5396)
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
        u4MinVactive = (wDrvGetOutputVTotal() * picInfo->wSrcH)/(u2MaxVTotal - wDrvGetOutputVTotal()/2 - 4)/2 ;
        u4MinVactive= u4MinVactive* VDP_MAX_REGION_WIDTH/inputHeight;
    }
    #endif
    
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
    
    #if defined(CC_MT5396)
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

    //turn off snake mapping (for ease dram dump)
    vRegWriteFldAlign(SCPIP_DRAM_M_00 + dramOffset, 0, DRAM_M_00_DA_SNAKE_MAPPING);
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
        if(BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC)
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
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 1, DS1_02_VSYNC_OPT_1); //5396/68 dram agent need short vsync from down scaler. Set this reg to 1 will do this.//FIXME
        if(BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC)
        {
            //import triggered at pscan vsync.
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 0,PIP_OA_2C_IMPORT_RVSYNC_TRIG_SEL);
            vRegWriteFldAlign(SCPIP_PIP_OA_2C, 1,PIP_OA_2C_IMPORT_VSYNC_SRC_SEL);
            vRegWriteFldAlign(SCPIP_SYSTEM_2C, 0,SYSTEM_2C_IMPORT_VSYNC_US_SEL);
        }        
    }
    else        //for line sync dispmode
    {
        LOG(4,"Set Line sync dispmode import setting\n");
        vRegWriteFldAlign(SCPIP_SYSTEM_10+importOffset, 0, SYSTEM_10_IMPORT_SEL_1);
        vRegWriteFldAlign(SCPIP_DS1_02+dsOffset, 0, DS1_02_VSYNC_OPT_1);
        if(BSP_GetIcVersion() >= IC_VER_5396_AB && BSP_GetIcVersion() <= IC_VER_5396_AC)
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
    //sub active adj
    vRegWriteFldAlign(SCPIP_PIP2_06, SUB_ACTIVE_ADJ, PIP2_06_ACTIVE_ADJ_2); // PLC_DLY need to adjust
    //sub read delay
    vRegWriteFldAlign(SCPIP_PIP2_06, SUB_READ_DELAY, PIP2_06_READ_DELAY_2);
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

#if defined(CC_MT5396)
//review this
UINT32 u4ScpipGetMinSrcRegionHeight(UINT8 u4VdpId)
{
    return 0;
}
#endif

#if defined(CC_MT5398)
void vScpipSetHInitPhase(UINT32 u4VdpId, UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_00+getSCOffset(getScalerPath(u4VdpId)), u4InitSign, SCCTRL1_00_H_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_00+getSCOffset(getScalerPath(u4VdpId)), u4InitIntg, SCCTRL1_00_H_PHASE_INIT_INTG_1);
	(u4VdpId==VDP_1) ? vRegWriteFldAlign(SCPIP_SCCTRL1_00, u4InitFrac, SCCTRL1_00_H_PHASE_INIT_FRAC_1)
					 : vRegWriteFldAlign(SCCTRL2_00, u4InitFrac, SCCTRL2_00_H_PHASE_INIT_FRAC_2);
}

void vScpipSetHDeltaPhase(UINT32 u4VdpId, UINT32 u4DeltaIntg, UINT32 u4DeltaFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_01+getSCOffset(getScalerPath(u4VdpId)), u4DeltaIntg, SCCTRL1_01_H_PHASE_DELTA_INTG_1);
	(u4VdpId==VDP_1) ? vRegWriteFldAlign(SCPIP_SCCTRL1_01, u4DeltaFrac, SCCTRL1_01_H_PHASE_DELTA_FRAC_1)
					 : vRegWriteFldAlign(SCCTRL2_01, u4DeltaFrac, SCCTRL2_01_H_PHASE_DELTA_FRAC_2);	
}

void vScpipSetVInitPhase(UINT32 u4VdpId, UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_02+getSCOffset(getScalerPath(u4VdpId)), u4InitSign, SCCTRL1_02_V_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_02+getSCOffset(getScalerPath(u4VdpId)), u4InitIntg, SCCTRL1_02_V_PHASE_INIT_INTG_1);
	(u4VdpId==VDP_1) ? vRegWriteFldAlign(SCPIP_SCCTRL1_02, u4InitFrac, SCCTRL1_02_V_PHASE_INIT_FRAC_1)
					 : vRegWriteFldAlign(SCCTRL2_02, u4InitFrac, SCCTRL2_02_V_PHASE_INIT_FRAC_2);
}

void vScpipSetVDeltaPhase(UINT32 u4VdpId, UINT32 u4DeltaIntg, UINT32 u4DeltaFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_03+getSCOffset(getScalerPath(u4VdpId)), u4DeltaIntg, SCCTRL1_03_V_PHASE_DELTA_INTG_1);
	(u4VdpId==VDP_1) ? vRegWriteFldAlign(SCPIP_SCCTRL1_03, u4DeltaFrac, SCCTRL1_03_V_PHASE_DELTA_FRAC_1)
					 : vRegWriteFldAlign(SCCTRL2_03, u4DeltaFrac, SCCTRL2_03_V_PHASE_DELTA_FRAC_2);		
}

/**
 * @brief Set UV initial phase in 422 domain (only in MAIN path)
 * @8098 porting done
 */
void vScpipSetHUVInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u4InitSign, SCCTRL1_04_H_UV_PHASE_INIT_SIGN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u4InitIntg, SCCTRL1_04_H_UV_PHASE_INIT_INTG_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u4InitFrac, SCCTRL1_04_H_UV_PHASE_INIT_FRAC_1);	
}

void vScpipSetInputResolution(UINT32 u4VdpId, UINT32 u4InputWidth, UINT32 u4InputHeight)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_09+getSCOffset(getScalerPath(u4VdpId)), u4InputWidth, SCCTRL1_09_INPUT_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_09+getSCOffset(getScalerPath(u4VdpId)), u4InputHeight, SCCTRL1_09_INPUT_HEIGHT_1);	
}

void vScpipSetDramSrcWidth(UINT32 u4VdpId, UINT32 u4DramWidth)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_0C+getSCOffset(getScalerPath(u4VdpId)), u4DramWidth, SCCTRL1_0C_SC_DRAM_SRC_W_WIDTH_1);	
}

void vScpipSetDramWidthHeight(UINT32 u4VdpId, UINT32 u4DramWidth, UINT32 u4DramHeight)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1C+getSCOffset(getScalerPath(u4VdpId)), u4DramWidth, SCCTRL1_1C_DRAM_W_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1C+getSCOffset(getScalerPath(u4VdpId)), u4DramHeight, SCCTRL1_1C_DRAM_W_HEIGHT_1);
}

void vScpipSetDramHVPitch(UINT32 u4VdpId, UINT32 u4DramWidth, UINT32 u4DramHeight)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1F+getSCOffset(getScalerPath(u4VdpId)), u4DramWidth, SCCTRL1_1F_WR_DRAM_H_PITCH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1E+getSCOffset(getScalerPath(u4VdpId)), u4DramHeight, SCCTRL1_1E_WR_DRAM_V_PITCH_1);
}

void vScpipSetOutputResolution(UINT32 u4VdpId, UINT32 u4OutputWidth, UINT32 u4OutputHeight)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_11+getSCOffset(getScalerPath(u4VdpId)), u4OutputWidth, SCCTRL1_11_OUTPUT_WIDTH_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_11+getSCOffset(getScalerPath(u4VdpId)), u4OutputHeight, SCCTRL1_11_OUTPUT_HEIGHT_1);	
}


/**
 * @brief Set vcnt, read begin after writed vcnt lines in the same frame buffer.
 * @8098 porting done
 */
void vScpipSetReadPosGen(UINT32 u4VdpId, UINT32 u4VCnt)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1E+getSCOffset(getScalerPath(u4VdpId)), u4VCnt, SCCTRL1_1E_RPT_GEN_VCNT_1);		
}

/**
 * @brief switch for color domain which scaler work on (only in MAIN path)
 * @param: u4OnOff 1: one-step 422 to 444 enable (SC work on 422 domain)
 *                              0: one-step 422 to 444 disable (SC work on 444 domain)
 * @8098 porting done
 */
void vScpip422ModeOnoff(UINT32 u4OnOff)
{
	UINT8 value;
    value = (u4OnOff==SV_ON)?1:0;
	
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, value, SCCTRL1_04_H_422_MODE_EN_1);
}

/**
 * @brief reorder UV sequence (only in MAIN path)
 * @param: u4Inverse 1: y0cr y1cb (when mirror)
 *                              	  0: y0cb y1cr
 * @8098 porting done
 */
void vScpipReorderUV(UINT32 u4Inverse)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u4Inverse, SCCTRL1_04_REORDER_UV_INV_1);
}

/**
 * @brief set 422 mirror enable, if mirror made before scaler (ex. scaler front dram mode) 
 * @and scaler work in 422 domain, need enable this.
 * @8098 porting done
 */
void vScpip422MirrorEnable(UINT32 u4Enable)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_0A, u4Enable, SCCTRL1_0A_SC_H_422_MIRROR_EN);
}

/**
 * @brief select dram data format.
 * @param: u4Sel10b 1: 10bit / 0: 8bit 
 * @8098 porting done
 */
void vScpipSetDram10bFmt(UINT32 u4VdpId, UINT32 u4Sel10b)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B, u4Sel10b, SCCTRL1_1B_DRAM_WR_10B_SEL_1);
}

/**
 * @brief select dram data format
 * @param: u4Sel444 1: 444 / 0: 422
 * @8098 porting done
 */
void vScpipSetDram444Fmt(UINT32 u4VdpId, UINT32 u4Sel444)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B+getSCOffset(getScalerPath(u4VdpId)), u4Sel444, SCCTRL1_1B_DRAM_W_444_1);
}

/**
 * @brief set Scaler front/back mode
 * @param: u4Mode 0: other mode / 1: front dram mode / 2: back dram mode
 * @8098 porting done
 */
void vScpipSetSCFrnBckMode(UINT32 u4VdpId, UINT32 u4Mode)
{
	UINT32 u4BckMode, u4FrnMode;

	u4BckMode = (u4Mode & 2) >> 1;
	u4FrnMode = (u4Mode & 1);

	if(1 == u4BckMode && 1 == u4FrnMode)
	{
		LOG(0, "Ooops! Wrong Scaler Mode!\n");
		return;
	}

	vRegWriteFldAlign(SCPIP_SCCTRL1_13+getSCOffset(getScalerPath(u4VdpId)), u4BckMode, SCCTRL1_13_BCK_DRAM_MODE_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_13+getSCOffset(getScalerPath(u4VdpId)), u4FrnMode, SCCTRL1_13_FRN_DRAM_MODE_1);
}


/**
 * @brief enable 444 to 422 format transfer
 * @param: u4Enable 0: original fmt to dram / 1: 444 to 422 enable
 * @8098 porting done
 */
void vScpipEnable444To422BeforeDram(UINT32 u4VdpId, UINT32 u4Enable)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_13+getSCOffset(getScalerPath(u4VdpId)), u4Enable, SCCTRL1_13_DRAM_YUV2YC_EN_1);
}


/**
 * @brief enable 422 to 444 format transfer
 * @param: u4Enable 0: original fmt from dram / 1: 422 to 444 enable
 * @8098 porting done
 */
void vScpipEnable422To444AfterDram(UINT32 u4VdpId, UINT32 u4Enable)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_13+getSCOffset(getScalerPath(u4VdpId)), u4Enable, SCCTRL1_13_DRAM_YC2YUV_EN_1);
}

/**
 * @brief enable setting resolution import protection
 * @param: u4OnOff 1: ON / 0: OFF
 * @8098 porting done
 */
void vScpipSetResImportOnOff(UINT32 u4VdpId, UINT32 u4OnOff)
{
	UINT8 value;
    value = (u4OnOff==SV_ON)?1:0;
	
	vRegWriteFldAlign(SCPIP_SCCTRL1_0A+getSCOffset(getScalerPath(u4VdpId)), value, SCCTRL1_0A_SET_RES_IMPORT_ENABLE_1);
}

void vScpipSetSCImportOff(UINT32 u4VdpId, UINT32 u4OnOff)
{
	UINT8 value;
    value = (u4OnOff==SV_ON)?1:0;	
	
	vRegWriteFldAlign(SCPIP_SCCTRL1_0A+getSCOffset(getScalerPath(u4VdpId)), value, SCCTRL1_0A_SC_SET_RES_TOGGLE_1);
}

void vScpipDramImportOnOff(UINT32 u4VdpId, UINT32 u4OnOff)
{
	UINT8 value;
    value = (u4OnOff==SV_ON)?1:0;

	vRegWriteFldAlign(SCPIP_SCCTRL1_1B+getSCOffset(getScalerPath(u4VdpId)), value, SCCTRL1_1B_DRAM_IMPORT_ENABLE_1);	
}

void vScpipSetDramFrmDely(UINT32 u4VdpId, UINT32 u4Delay)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B+getSCOffset(getScalerPath(u4VdpId)), u4Delay, SCCTRL1_1B_DRAM_WR_FRM_DLY_1);
}

/**
 * @brief set resolution related to LR pair when in 3D mode
 * @param u4Ref 00,01: ingnore disp_r / 10: disp_r = 0 / 11: disp_r = 1
 * @8098 porting done
 */
void vScpipSetResRefLRPair(UINT32 u4Ref)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_0A, u4Ref, SCCTRL1_0A_SC_LR_PAIR_CTRL);	
}

/**
 * @brief fix dram read pointer
 * @param u4Pointer: fix read the certain frame buffer.
 * @8098 porting done
 */
void vScpipFixDramReadPointerEn(UINT32 u4VdpId, UINT32 u4Enable, UINT32 u4Pointer)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B+getSCOffset(getScalerPath(u4VdpId)), u4Enable, SCCTRL1_1B_FIX_DRAM_RPTR_EN_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B+getSCOffset(getScalerPath(u4VdpId)), u4Pointer, SCCTRL1_1B_FIX_DRAM_RPTR_1);
}

/**
 * @brief dram write freeze,  if scaler is not in dram mode, this function should fail.
 * @8098 porting done
 */
void vScpipDramWriteFreeze(UINT32 u4VdpId, UINT32 u4OnOff)
{
	UINT8 value;
    value = (u4OnOff==SV_ON)?1:0;
	
	vRegWriteFldAlign(SCPIP_SCCTRL1_1B, value, SCCTRL1_1B_DRAM_W_FREEZE_1);	
}

void vScpipSetDramWriteOffset(UINT32 u4VdpId, UINT32 u4HOfst, UINT32 u4VOfst)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1D+getSCOffset(getScalerPath(u4VdpId)), u4HOfst, SCCTRL1_1D_DRAM_W_HSTR_1);	
	vRegWriteFldAlign(SCPIP_SCCTRL1_1D+getSCOffset(getScalerPath(u4VdpId)), u4VOfst, SCCTRL1_1D_DRAM_W_VSTR_1);	
}

//@8098 review the name of function
void vScpipBobVScalingOnOff(UINT32 u43DVUpOnOff, UINT32 u4FieldVUpOnOff, UINT32 u4EnableDiffInitPhase)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u43DVUpOnOff, SCCTRL1_04_V_3D_EN);
	vRegWriteFldAlign(SCPIP_SCCTRL1_04, u4FieldVUpOnOff, SCCTRL1_04_V_FIELD_EN);
	vRegWriteFldAlign(SCPIP_SCCTRL1_18, u4EnableDiffInitPhase, SCCTRL1_18_V_PHASE_INIT_3D_FIELD_EN);
}

void vScpipSetBobBtmLeftInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_18, u4InitSign, SCCTRL1_18_V_PHASE_INIT_SIGN_BTM_LEFT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_18, u4InitIntg, SCCTRL1_18_V_PHASE_INIT_INTG_BTM_LEFT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_18, u4InitFrac, SCCTRL1_18_V_PHASE_INIT_FRAC_BTM_LEFT_1);	
}

void vScpipSetBobTopRightInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_19, u4InitSign, SCCTRL1_19_V_PHASE_INIT_SIGN_TOP_RIGHT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_19, u4InitIntg, SCCTRL1_19_V_PHASE_INIT_INTG_TOP_RIGHT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_19, u4InitFrac, SCCTRL1_19_V_PHASE_INIT_FRAC_TOP_RIGHT_1);	
}

void vScpipSetBobBtmRightInitPhase(UINT32 u4InitSign, UINT32 u4InitIntg, UINT32 u4InitFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_1A, u4InitSign, SCCTRL1_1A_V_PHASE_INIT_SIGN_BTM_RIGHT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1A, u4InitIntg, SCCTRL1_1A_V_PHASE_INIT_INTG_BTM_RIGHT_1);
	vRegWriteFldAlign(SCPIP_SCCTRL1_1A, u4InitFrac, SCCTRL1_1A_V_PHASE_INIT_FRAC_BTM_RIGHT_1);	
}


/**
 * @brief Nonlinear scaling on off (only in MAIN path)
 * @param: u4OnOff 1: ON / 0: OFF
 * 		     u4AddMinusSel: 1: side delta phase < middle delta phase 
 *						   0: side delta phase > middle delta phase
 * @8098 porting done
 */
void vScpipNonlinearOnOff(UINT32 u4OnOff, UINT32 u4AddMinusSel)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4OnOff, SCCTRL1_05_NONL_H_ENABLE);
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4AddMinusSel, SCCTRL1_05_NONL_H_ADD_MINUS_SEL);		
}

void vScpipSetNonlinearPos(UINT32 u4StartPos, UINT32 u4StopPos, UINT32 u4MiddlePos)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4StartPos, SCCTRL1_05_NONL_H_START_POS);
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4StopPos, SCCTRL1_05_NONL_H_STOP_POS);	
	vRegWriteFldAlign(SCPIP_SCCTRL1_07, u4MiddlePos, SCCTRL1_07_NONL_H_MIDDLE_POS);		
}

/**
 * @brief set Nonlinear DeltaPhase increament on slope
 * @8098 porting done
 */
void vScpipSetNonlinearDeltaPhase(UINT32 u4DeltaIntg, UINT32 u4DeltaFrac)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_06, u4DeltaIntg, SCCTRL1_06_NONL_H_PHASE_DELTA);
	vRegWriteFldAlign(SCPIP_SCCTRL1_06, u4DeltaFrac, SCCTRL1_06_NONL_H_PHASE_DELTA_FLOAT);		
}


void vScpipSetDispFifoWaterLevel(UINT32 u4VdpId, UINT32 u4Level)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_0B+getSCOffset(getScalerPath(u4VdpId)), u4Level, SCCTRL1_0B_DISP_FF_WTLV_1);
}

/**
 * @brief set Scaler 3D mode
 * @param u4Mode 0:2d / 2:frame packing / 3:frame seq / 4:t&b / 8:s by s / c:line interleave
 * @8098 porting done
 */
void vScpipSetSC3DMode(UINT32 u4Mode)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4Mode, SCCTRL1_05_3D_MODE);	
}

/**
 * @brief usage???
 */
void vScpipMaster3DDispMode(UINT32 u4OnOff)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_05, u4OnOff, SCCTRL1_05_SC_MSK_DISP_R);		
}


/**
 * @brief main dram read request after delay
 */
void vScpipSetDramReadReqDly(UINT32 u4Delay)
{
	vRegWriteFldAlign(SCPIP_SCCTRL1_0B, u4Delay, SCCTRL1_0B_DRAM_RDREQ_DLY_1);
}

/***
  * @ sc_msk_dispff_mib_disp/ sc_dram_3d_enable_1 / sc_eng_input_end_dly_1 to be added
  * @ scctrl?_atpg_ct / scctrl?_atpg_ob
  */


/**
 * @brief set tve repeat mode
 * @8098 porting done
 */
void vScpipSetTveRptMode(UINT32 u4Mode)
{
	vRegWriteFldAlign(SCCTRL2_0A, u4Mode, SCCTRL2_0A_SC_TVE_RPT_MODE);
}

/**
 * @brief reorder UV sequence (only in MAIN path)
 * @return: u4Inverse  1: y0cr y1cb (when mirror)
 *                              	  0: y0cb y1cr
 * @8098 porting done
 */
UINT32 u4ScpipGetUVOrder(void)
{
	return (UINT32)RegReadFldAlign(SCPIP_SCCTRL1_04, SCCTRL1_04_REORDER_UV_INV_1);
}

UINT32 u4ScpipGetSetResInStatus(UINT32 u4VdpId)
{
	return (UINT32)RegReadFldAlign(SCPIP_SCCTRL1_0A+getSCOffset(getScalerPath(u4VdpId)), SCCTRL1_0A_SC_STA_SET_RES_I_1);
}

UINT32 u4ScpipGetSetResOutStatus(UINT32 u4VdpId)
{
	return (UINT32)RegReadFldAlign(SCPIP_SCCTRL1_0A+getSCOffset(getScalerPath(u4VdpId)), SCCTRL1_0A_SC_STA_SET_RES_O_1);
}

// add get function to determine scaler in which mode (dram mode / display mode)
#endif
