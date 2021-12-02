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

#if defined(CC_SUPPORT_TVE)
#include "tve_if.h"
#endif

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
extern UINT32 _u4OSTGDelta;

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

UINT8 bScpipMirrorEnable[2]= {SV_FALSE, SV_FALSE};
UINT8 bScpipFlipEnable[2] = {SV_FALSE, SV_FALSE};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _vScpipSetSyncAdj(void)
{
    UINT32 u4MainActiveAdj;
    
    vRegWriteFldAlign(SCPIP_PIP_OA_02, MAIN_ACTIVE_ADJ, PIP_OA_02_HSYNC_ADJ); 
    vRegWriteFldAlign(SCPIP_PIP1_08, 0+_u4OSTGDelta, PIP1_08_READ_DELAY_1);
    u4MainActiveAdj = MAIN_ACTIVE_ADJ+_u4OSTGDelta;
    vRegWriteFldAlign(SCPIP_PIP1_08, u4MainActiveAdj, PIP1_08_ACTIVE_ADJ_1); 
    
    #if defined(CC_MT5365) // 5365 only for negative initial phase 
    vRegWriteFldAlign(SCPIP_PIP_OA_09, MAIN_ACTIVE_ADJ-1, PIP_OA_09_HSYNC_ADJ_NEG_PHASE);
    vRegWriteFldAlign(SCPIP_PIP1_0D, u4MainActiveAdj-1, PIP1_0D_ACTIVE_ADJ_NEG_PHASE);
    #endif       
    
    #if SUPPORT_POP
    vRegWriteFldAlign(SCPIP_PIP2_08, SUB_READ_DELAY+_u4OSTGDelta, PIP2_08_READ_DELAY_2);
    vRegWriteFldAlign(SCPIP_PIP2_08, SUB_ACTIVE_ADJ+_u4OSTGDelta, PIP2_08_ACTIVE_ADJ_2); // PLC_DLY need to adjust
    #endif
}

static void _vScpipPscanDispmodeInit()
{
    vRegWriteFldAlign(SCPIP_PIP_OA_0A, 0, PIP_OA_0A_WATER_CTRL1);
    vRegWriteFldAlign(SCPIP_PIP1_07, 0, PIP1_07_DISPMODE_H_POS_1);
    vRegWriteFldAlign(SCPIP_US1_10, 0, US1_10_REG_PSCAN_DISP_EN_1);
    vRegWriteFldAlign(SCPIP_US1_10, 0, US1_10_REG_CLEAR_SEL_1);
    vRegWriteFldAlign(SCPIP_US1_10, 0, US1_10_REG_DISP_PRETFETCH_1);
    vRegWriteFldAlign(SCPIP_PIP1_11, 0, PIP1_11_DISPMODE_DATA_RATE_1);    
    vRegWriteFldAlign(SCPIP_PIP1_0A, 0, PIP1_0A_VS_DATA_THP_1);
    vRegWriteFldAlign(SCPIP_PIP1_0C, 1, PIP1_0C_PSCAN_VSYNC_REGEN1); //enable vsync regen for Pscan    
}

static UINT32 _u4ScpipGetDramAutoMaskOption(UINT32 u4MinDataLen)
{
    UINT32 u4Ret;
    
    switch(u4MinDataLen)
    {
        case 32:
            u4Ret = 0;
            break;
        case 16:
            u4Ret = 1;
            break;
        case 8:
            u4Ret = 2;
            break;
        case 4:
            u4Ret = 3;
            break;
        default:
            u4Ret = 0;
    }

    return u4Ret;
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 u4ScpipGetHwMuteStatus(UINT8 bPath)
{
    if (bPath >= SV_VP_NA)
    {
        return 0;
    }
    return _au4HwMuteStatus[bPath];
}

void vScpipSetHwMuteStatus(UINT8 bPath, UINT8 u1OnOff)
{
    if (bPath >= SV_VP_NA)
    {
        return;
    }

    _au4HwMuteStatus[bPath] = u1OnOff;
}
#endif

UINT8 u1ScpipGetTv3dOutIndicator()
{
    return RegReadFldAlign(SCPIP_SYSTEM_RO_00, SYSTEM_RO_00_RD_TV3D_OUT);
}

/**
* @brief Set scpip clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vScpipSetClock(UINT8 bPath, SCPIP_CLOCK_TYPE eType)
{
    if(bPath == SV_VP_MAIN)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG, eType, FLD_SCPIP_MAIN_CLK_SEL);
    }
    else if(bPath == SV_VP_PIP)
    {
        vIO32WriteFldAlign(CKGEN_DISP_CKCFG, eType, FLD_SCPIP_PIP_CLK_SEL);
    }
}

//-----------------------------------------------------------------------------
/** Brief of vScposInit.
 * Initial SCPOS related setting
 * @param  bPath
 * @retval void
 */
//-----------------------------------------------------------------------------
void vScposInit(UINT8 bPath)
{
    UINT8 pipOffset = getPIPOffset(bPath);  

    vDrvDISetPSCANClock(bPath, DI_CLK_TYPE_VDOIN);
    vScpipSetClock(bPath, SCPIP_CLK_TYPE_VDOIN);
    
    // DS to odd pixel, the right most pixel color issue.
    vRegWriteFldAlign(SCPIP_PIP1_04+pipOffset, 0, PIP1_04_IMPORT_SEL_1);
    //H Boundary protection enable
    vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, 1, PIP1_0F_BOUNDARY_H_EN_1);
    // turn on VLMASK
    vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 1, PIP1_09_VLMASK_1);

    #if !CC_SCPIP_DRAM_OVERWRITE_WA
    // Import delay =2 , both DS and PIP
    vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, 2, PIP1_03_IMPORT_DELAY_1);
    vRegWriteFldAlign(SCPIP_PIP1_03+pipOffset, 1, PIP1_03_IMPORT_DELAY_EN_1);
    #if 0
    vRegWriteFldAlign(SCPIP_DS1_04+pipOffset, 1, DS1_04_IMPORT_V_DELAY);
    vRegWriteFldAlign(SCPIP_DS1_04+pipOffset, 1, DS1_04_IMPORT_V_DELAY_EN);
    #else
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_DS1_02+pipOffset, 1, DS1_02_IMPORT_V_INV_1);
    }
    #endif
    #endif // end of CC_SCPIP_DRAM_OVERWRITE_WA

    // reset write enable to off state in initialization
    vRegWriteFldAlign(SCPIP_PIP1_09+pipOffset, 0, PIP1_09_W_EN_1);
    
    #ifdef CC_MT5395
    vRegWriteFldAlign(SCPIP_PIP1_0D+pipOffset, 0, PIP1_0D_VL_MASKL_Y_1);
    vRegWriteFldAlign(SCPIP_PIP_OA_00, 2, PIP_OA_00_TV3D_VOFST);
    #endif
    
    #if (CC_SCPOS_DRAM_BURST_READ)
    vRegWriteFldAlign(SCPIP_PIP1_05+pipOffset, 1, PIP1_05_DRAM_BURST_MODE_1); 
    #endif
    
    vRegWriteFldAlign(SCPIP_PIP1_0F+pipOffset, 1, PIP1_0F_DRAM_RREQ_ENH_1);  // for non-linear scaling non-precise DATALEN issue    
                                                                             // (the second read request will not be sent)
    #ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    vRegWriteFldAlign(SCPIP_SYSTEM_01, 1, SYSTEM_01_MEM_WDLEA_SEL); 
    vRegWriteFldAlign(SCPIP_SYSTEM_01, 1, SYSTEM_01_MEM_WDLEB_SEL);     
    #endif

    if (bPath == SV_VP_MAIN)
    {
        _vScpipPscanDispmodeInit();        

        #if SUPPORT_HDTV_HARDWARE_MUTE
        vScpipConfigHardwareMute();
        #endif

        //------MAIN Special Part ---------------------------
        // init US filter for negative initial phase
        vRegWriteFldAlign(SCPIP_US1_01, 1, US1_01_HORIZONTAL_FILTER_OFF_1);

        //default: OUTSTG_VSYNC_DELSEL = 4 and OUTSTG_VSYNC_DELSEL2 = 5 in 4-taps
        #ifdef CC_MT5395
        // for CBE module extra line delay    
        vRegWriteFldAlign(OUTSTG_OS_75, 6, OS_75_VSYNC_DELSEL2);
        vRegWriteFldAlign(OUTSTG_OS_75, 5, OS_75_VSYNC_DELSEL);  
        #else
        vRegWriteFldAlign(OUTSTG_OS_75, 4, OS_75_VSYNC_DELSEL);
        vRegWriteFldAlign(OUTSTG_OS_75, 5, OS_75_VSYNC_DELSEL2);
        #endif
        vRegWriteFldAlign(OUTSTG_OS_15, 1, OS_15_LSB_OFF);
        // non-linear down scaling mode: set default to new mode
        vRegWriteFldAlign(SCPIP_DS1_07, 1, DS1_07_NONL_H_OPT_1);
        
        // v boundary enable
        vRegWriteFldAlign(SCPIP_US1_14, 1, US1_14_UP_VBOUND_EN1);

        // Minimum DATA_LEN for auto calculation
        vRegWriteFldAlign(SCPIP_PIP_OA_13, _u4ScpipGetDramAutoMaskOption(CC_MINIMUN_DRAM_RW_UNIT), PIP_OA_13_DRAM_AUTO_MASK_OPTION1);
        
        // To prevent transient at the top of the video in 5365/5395
        vRegWriteFldAlign(SCPIP_PIP_OA_13, 1, PIP_OA_13_RDFIFO_AUTO_SELECTION1);
        
        // scaler display mode eco option
        if((BSP_GetIcVersion() != IC_VER_5365_AA)&&(BSP_GetIcVersion() != IC_VER_5395_AA))
        {
            vRegWriteFldAlign(SCPIP_PIP_OA_13, 1, PIP_OA_13_FLAG_DETECT_OPTION);            
        }
        #ifdef CC_MT5395
        if(BSP_GetIcVersion() != IC_VER_5395_AA)
        {
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 1, SYSTEM_03_TV3D_REGEN_IN);    
            vRegWriteFldAlign(SCPIP_SYSTEM_03, 0, SYSTEM_03_TV3D_REGEN_SEL);
            vRegWriteFldAlign(SCPIP_PIP1_0C, 1, PIP1_0C_TV3D_POS_INV);    
            vRegWriteFldAlign(SCPIP_SYSTEM_01, 2, SYSTEM_01_TV3D_OVSEL);    
        }
        #endif

        _vScpipSetSyncAdj();
    }
    else
    {
        #if SUPPORT_HDTV_HARDWARE_MUTE
        //enable sub path quick off function by default
        vOSTGVideoPlaneHWMute(SV_VP_PIP,SV_ON);
        #endif
        /* initially hide sub window output region */
        vRegWriteFldAlign(SCPIP_PIP2_02, 0, PIP2_02_RVL_2);
        
        // v boundary enable
        vRegWriteFldAlign(SCPIP_US2_03, 0, US2_03_UP_VBOUND_EN2);
        
        // Minimum DATA_LEN for auto calculation
        vRegWriteFldAlign(SCPIP_PIP_OA_13, _u4ScpipGetDramAutoMaskOption(CC_MINIMUN_DRAM_RW_UNIT), PIP_OA_13_DRAM_AUTO_MASK_OPTION2);
        
        // To prevent transient at the top of the video in 5365/5395
        vRegWriteFldAlign(SCPIP_PIP_OA_13, 1, PIP_OA_13_RDFIFO_AUTO_SELECTION2);

        //for tve bypass garbage problem [88091]
        vRegWriteFldAlign(SCPIP_PIP_OA_07, 1, PIP_OA_07_DATALEN_MASK2);

        #ifdef CC_MT5395
        // for CBE module extra line delay
        vRegWriteFldAlign(SCPIP_PIP_OA_03, 8, PIP_OA_03_PRE_VOFST_BYPASS2_LSB);
        vRegWriteFldAlign(SCPIP_PIP_OA_03, 8, PIP_OA_03_PRE_VOFST2_LSB);        
        #endif
    }
}


/**
 * @brief vScpipImportOnOff 
 * Turn on/off import protection
 * @param  bPath: SV_VP_MAIN/SV_VP_PIP
 *         bOnOff: SV_ON/SV_OFF
 * @retval void
 */
void vScpipImportOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(SCPIP_PIP1_09, bOnOff, PIP1_09_IMPORT_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP2_09, bOnOff, PIP2_09_IMPORT_2);
    }    
}

UINT32 u4ScpipGetMinSrcRegionWidth(UINT8 u4VdpId)
{
    UINT32 ret = 0;
    UINT16 inputWidth;
    inputWidth = wDrvVideoInputWidth(u4VdpId);
    
    if (u1ScpipGet444Mode((u4VdpId == VDP_1)? SV_VP_MAIN: SV_VP_PIP) == SV_FALSE)     // 422 Mode
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
    return ret;
}

void vScpipSetDramBurst(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 offset = getPIPOffset(bPath);
    if(RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_DRAM_BURST_MODE_1)!= bOnOff)
    {
        LOG(3, "SCPIP: Switch DRAM burst read mode (%d)\n", bOnOff);
        vRegWriteFldAlign(SCPIP_PIP1_05 + offset, bOnOff, PIP1_05_DRAM_BURST_MODE_1);
    }
}

UINT8 u1ScpipGetDramBurst(UINT8 bPath)
{
    UINT8 offset = getPIPOffset(bPath);
    return (UINT8)RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_DRAM_BURST_MODE_1);    
}

void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 offset = getPIPOffset(bPath);
    bScpipMirrorEnable[bPath]= bOnOff;

    if(bOnOff== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 1, PIP1_0B_HDIR_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 0, PIP1_0B_HDIR_1);
    }
}

void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff)
{
    UINT8 offset;

    if(bScpipFlipEnable[bPath]==bOnOff)
        return;
    
    bScpipFlipEnable[bPath] = bOnOff;
    offset = getPIPOffset(bPath);

    if(bOnOff== SV_TRUE)
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 1, PIP1_0B_VDIR_1);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_0B + offset, 0, PIP1_0B_VDIR_1);
    }
}

void vScpipSetMirrorFlipAll(UINT8 bOnOff)
{
    vScpipSetMirror(SV_VP_MAIN, bOnOff);
    vScpipSetMirror(SV_VP_PIP, bOnOff);

    vScpipSetFlip(SV_VP_MAIN, bOnOff);
    vScpipSetFlip(SV_VP_PIP, bOnOff);
}


UINT8 u1ScpipGetDispmodeLineBufNum(UINT32 bPath)
{
    UINT8 bCline, bCfgCline;
    bCline = RegReadFldAlign(SCPIP_PIP_OA_06+bPath,  PIP_OA_06_C_LINE1);
    bCfgCline = RegReadFldAlign(SCPIP_PIP_OA_06+bPath,  PIP_OA_06_CFG_LINE1);

    if(bCline ==1)
    {
        return bCfgCline+2;
    }
    else // (bCline ==0)
    {
        switch (bCfgCline)
        {
            case 0: return 3;
            case 1: return 4;
            case 2: return 6;
            case 3: return 8;
            default: return 0;
        }
    }
}


#if SUPPORT_HDTV_HARDWARE_MUTE
static void vOSTGLVDSHWMute(E_QUICK_BLANK_MODE mode)
{
    vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_QUICK_BLANK_SEL);
}

void vOSTGVideoPlaneHWMute(UINT8 bPath,UINT8 mode)
{
    if(bPath == SV_VP_MAIN)
    {
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_MAIN_QUICK_OFF_EN);
    }
    else if(bPath == SV_VP_PIP)
    {
        vRegWriteFldAlign(OUTSTG_OS_1A, mode, OS_1A_PIP_QUICK_OFF_EN);
    }
}

void vScpipConfigHardwareMute(void)
{
    UINT8 u1Dispmode;
    E_QUICK_BLANK_MODE eMode = QUICK_BLANK_NO_FUNCTION;
    
    u1Dispmode = u1ScpipGetDispMode(SV_VP_MAIN);

    if(u1Dispmode==VDP_SCPOS_DISPMODE_AUTO || u1Dispmode == VDP_SCPOS_DISPMODE_MANUAL)
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
        vOSTGLVDSHWMute(eMode);
        //enable vdp quick mute
        vOSTGVideoPlaneHWMute(SV_VP_MAIN, SV_ON);
    }
    else 
    {
        //enable LVDS quick blank mute
        vOSTGLVDSHWMute(eMode);
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
        return _u4MainMuteMask;
    else
        return _u4PipMuteMask;
}
#endif



UINT32 u4ScpipCheckFifoOverflow(UINT32 u4VdpId)
{
    IC_VERSION_T rVer; 
    UINT8 u1Path = getScalerPath(u4VdpId);
    UINT8 pipOffset = getPIPOffset(u1Path);

    rVer = BSP_GetIcVersion();

    if(u1ScpipGetDispMode(u1Path) != VDP_SCPOS_DISPMODE_OFF)
    {
        LOG(6, "FIFO overflow flag is not supported in display mode\n");        
        return 1; // not supported in display mode
    }
    else if((u1ScpipGetDramBurst(u1Path))
        &&((rVer == IC_VER_5365_AA) || (rVer == IC_VER_5395_AA)))
    {
        LOG(6, "FIFO overflow flag is not supported in this version IC\n");
        return 1; // not supported
    }
    else
    {
        if(RegReadFldAlign(SCPIP_PIP1_RO_04 + pipOffset, PIP1_RO_04_FIFO_OVERW_1)
            || RegReadFldAlign(SCPIP_PIP1_RO_04 + pipOffset, PIP1_RO_04_FIFO_OVERR_1))
        {
            return 2; // fifo overflow
        }
        else
        {
            return 0; // no fifo overflow
        }
    }    
}

void vScpipResetFifoOverflow(UINT32 u4VdpId)
{
    UINT8 bPath = getScalerPath(u4VdpId);
    UINT8 pipOffset = getPIPOffset(bPath);

    if(u1ScpipGetDramBurst(bPath))
    {
        vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 1, PIP1_RO_04_DRAM_FLAG_OPTION);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 0, PIP1_RO_04_DRAM_FLAG_OPTION);        
    }
    
    vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 1, PIP1_RO_04_FIFO_RSTR_1);
    vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 1, PIP1_RO_04_FIFO_RSTW_1);
    x_thread_delay(20);
    vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 0, PIP1_RO_04_FIFO_RSTR_1);
    vRegWriteFldAlign(SCPIP_PIP1_RO_04 + pipOffset, 0, PIP1_RO_04_FIFO_RSTW_1);
    
}

UINT32 u4ScpipGetDSInputHTotal(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_00 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_00_DS_INHLEN_1);
}


UINT32 u4ScpipGetDSInputVTotal(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_00 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_00_DS_INVLEN_1);
}


UINT32 u4ScpipGetDSInputHActive(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_01 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_01_DS_INHACT_DSCALER_H_1);
}


UINT32 u4ScpipGetDSInputVActive(const UINT32 u4VdpId)
{
    return RegReadFldAlign(SCPIP_DS1_RO_01 + getDSOffset(getScalerPath(u4VdpId)), DS1_RO_01_DS_INVACT_DSCALER_H_1);    
}


UINT32 u4ScpipGetDramAddrWithChannelOfst(UINT32 u4VdpId, UINT32 u4Addr)
{
#ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    UINT32 u4ChannelOfst = 0;

    if (u4VdpId == VDP_1)
    {
        u4ChannelOfst = RegReadFldAlign(SCPIP_SYSTEM_01, SYSTEM_01_DRAM_MAIN_CH_SEL);
    }
    else
    {
        u4ChannelOfst = RegReadFldAlign(SCPIP_SYSTEM_01, SYSTEM_01_DRAM_PIP_CH_SEL);
    }
    if (u4ChannelOfst)
    {
        u4Addr += (SCPIP_CHANNELB_OFST);
    }    
#endif

    return u4Addr;
}


UINT32 u4ScpipGetWAddr(const UINT32 u4VdpId)
{
    UINT32 u4Addr;
    UINT32 offset = getPIPOffset(u4VdpId);
    
    u4Addr = RegReadFldAlign(SCPIP_PIP1_05+offset, PIP1_05_WSTARTP_1);
    #ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    u4Addr = u4ScpipGetDramAddrWithChannelOfst(u4VdpId, u4Addr << 4) >> 4;
    #endif

    return u4Addr;    
}


UINT32 u4ScpipGetRAddr(const UINT32 u4VdpId)
{
    UINT32 u4Addr;
    UINT32 offset = getPIPOffset(u4VdpId);
    
    u4Addr = RegReadFldAlign(SCPIP_PIP1_06+offset, PIP1_06_RSTARTP_1);
    #ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    u4Addr = u4ScpipGetDramAddrWithChannelOfst(u4VdpId, u4Addr << 4) >> 4;
    #endif

    return u4Addr;        
}


VOID vScpipSetWAddr(const UINT32 u4VdpId, UINT32 u4WAddr)
{
    UINT32 offset = getPIPOffset(u4VdpId);

    #ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    BOOL   fgIsChannelB = SV_FALSE;
    if(u4WAddr >= (SCPIP_CHANNELB_OFST>>4))
    {
        fgIsChannelB = SV_TRUE;
        u4WAddr -= (SCPIP_CHANNELB_OFST>>4); 
    }
    
    if (u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_01, fgIsChannelB, SYSTEM_01_DRAM_MAIN_CH_SEL);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_01, fgIsChannelB, SYSTEM_01_DRAM_PIP_CH_SEL);
    }  
    #endif

    vRegWriteFldAlign(SCPIP_PIP1_05+offset, u4WAddr, PIP1_05_WSTARTP_1);
}


VOID vScpipSetRAddr(const UINT32 u4VdpId, UINT32 u4RAddr)
{
    UINT32 offset = getPIPOffset(u4VdpId);

    #ifdef CC_SCPOS_SUPPORT_DRAM_CHANNELB
    BOOL   fgIsChannelB = SV_FALSE;
    if(u4RAddr >= (SCPIP_CHANNELB_OFST>>4))
    {
        fgIsChannelB = SV_TRUE;
        u4RAddr -= (SCPIP_CHANNELB_OFST>>4); 
    }
    
    if (u4VdpId == VDP_1)
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_01, fgIsChannelB, SYSTEM_01_DRAM_MAIN_CH_SEL);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_SYSTEM_01, fgIsChannelB, SYSTEM_01_DRAM_PIP_CH_SEL);
    }    
    #endif

    vRegWriteFldAlign(SCPIP_PIP1_06+offset, u4RAddr, PIP1_06_RSTARTP_1);
}


void vScpipSetWPos(UINT32 u4VdpId, BOOL fgOnOff, UINT8 u1Index)
{
    UINT8 pipOffset = getPIPOffset(getScalerPath(u4VdpId));
    
    vRegWriteFldAlign(SCPIP_PIP1_12+pipOffset, fgOnOff, PIP1_12_SET_W_POS_EN_1);
    vRegWriteFldAlign(SCPIP_PIP1_12+pipOffset, u1Index, PIP1_12_SET_W_POS_1);
}


UINT32 u4ScpipDirectSetDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr)
{
    UINT32 u4DataFormat = 0;
    UINT32 u410BitMode;
    UINT32 u4RGBMode;
    UINT32 u4GY;
    UINT32 u4BCb;
    UINT32 u4RCr;
    UINT32 u4BaseAddress;
    UINT32 u4FrameSize;
    UINT32 u4HL, u4VL;
    UINT32 u4Data1, u4Data2;
    UINT32 i, j, idx;
    UINT32 u4Offset1, u4Offset2;
    UINT32 u4DramUnitOffset;
    UINT32 u4Factor;
    UINT32 u4OverscanX, u4OverscanY;

    // support main only
    if (u4VdpId != VDP_1)
    {
        Printf("Not Support!\n");
        return 1;
    }

    // check freeze status
    if (fgMainWriteOn())
    {
        Printf("Freeze before set scaler DRAM!\n");
        return 1;
    }

    // Get Data format
    u410BitMode = RegReadFldAlign(SCPIP_DS1_02, DS1_02_VD10BSEL_1);
    u4RGBMode = fgMain444Mode();
    if (u4RGBMode && u410BitMode)
    {
        u4DataFormat = 3;
    }
    else if (u4RGBMode && (u410BitMode == 0))
    {
        u4DataFormat = 2;
    }
    else if ((u4RGBMode == 0) && u410BitMode)
    {
        u4DataFormat = 1;
    }
    else
    {
        u4DataFormat = 0;
    }
    u4GY = (u4GBRYCbCr & 0xff0000) >> 16;
    u4BCb = (u4GBRYCbCr & 0x00ff00) >> 8;
    u4RCr = (u4GBRYCbCr & 0x0000ff);


    // Get DRAM base address
    u4FrameSize = RegReadFldAlign(SCPIP_PIP1_07, PIP1_07_WINC_1);
    u4BaseAddress = u4ScpipGetWAddr(VDP_1);
    u4BaseAddress += (u4FrameSize * RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1));
    u4BaseAddress <<= 4;
    LOG(3, "Set Scaler DRAM: Format(%d), Data(0x%X), BaseAddress(0x%X), R_Pos(%d)\n",  u4DataFormat, u4GBRYCbCr, u4BaseAddress, RegReadFldAlign(SCPIP_PIP1_RO_00, PIP1_RO_00_R_POS_1));

    // Get DRAM parameter
    u4HL = RegReadFldAlign(SCPIP_PIP1_08, PIP1_08_DATA_LENGTH_1);
    u4VL = RegReadFldAlign(SCPIP_PIP1_00, PIP1_00_VL_1);

    // mapping source coordination to scaler DRAM
    LOG(3, "Rect (%d, %d, %d, %d) -->", u4X, u4Y, u4Width, u4Height);
    u4Factor = (RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER));
    u4X = (u4X * u4Factor/DFT_DS_FACTOR);
    u4Width = (u4Width * u4Factor/DFT_DS_FACTOR);
    LOG(3, " (%d, %d, %d, %d) -->", u4X, u4Y, u4Width, u4Height);
    if (u4DataFormat == 0) // YCbCr16 bit
    {
        u4OverscanX = u4HL * 8;      // pixel per-line in DRAM
    }
    else if (u4DataFormat == 1) // YCbCr20 bit
    {
        u4OverscanX = u4HL * 6;    // pixel per-line in DRAM
    }
    else // RGB 24
    {
        u4OverscanX = u4HL * 16/3;   // pixel per-line in DRAM
    }
    u4OverscanY = u4VL * 2;
    if (u4X + u4Width > u4OverscanX)
    {
        if (u4Width < (u4OverscanX -u4X))
        {
           u4Width = 0;
        }
        else
        {
            u4Width -= u4OverscanX -u4X;
        }
    }
    if (u4Y + u4Height > u4OverscanY)
    {
        if (u4Height < (u4OverscanY -u4Y))
        {
           u4Height = 0;
        }
        else
        {
            u4Height -= u4OverscanY -u4Y;
        }
    }
    u4Factor = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_H_1);
    u4Width = (u4Width * u4Factor)/DFT_DS_FACTOR;

    u4Factor = RegReadFldAlign(SCPIP_DS1_00, DS1_00_DSCALER_V_1);
    u4Height = (u4Height * u4Factor)/DFT_DS_FACTOR;
    LOG(3, " (%d, %d, %d, %d)\n", u4X, u4Y, u4Width, u4Height);

    #ifndef CC_COPLAT_MT82
    HalFlushInvalidateDCache();
    #endif

    // Set DRAM
    switch (u4DataFormat)
    {
        case 0:        // YCbCr16 bit
            u4BaseAddress += u4HL * u4Y * 16;
            u4Data1 = (u4GY << 24) | (u4BCb << 16) | (u4GY << 8) | u4RCr;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                for (i = u4X; i < (u4X+u4Width); i += 2)   // handle 2 pixels once
                {
                    u4Offset1 = i >> 3;
                    u4Offset1 = (u4Offset1 << 4) + (16 - ((i - (u4Offset1 << 3))*2) - 4);
                    vWriteDramD(VIRTUAL(u4BaseAddress + u4Offset1), u4Data1);
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }
            break;
        case 1:        // YCbCr20 bit
            // shift 8 bit data to 10 bit data
            u4GY = u4GY << 2;
            u4BCb = u4BCb << 2;
            u4RCr = u4RCr << 2;

            u4BaseAddress += u4HL * u4Y * 16;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                u4DramUnitOffset = u4X/6;
                u4DramUnitOffset *= 16;
                for (i = ((u4X /6) * 6); i < (u4X+u4Width); i += 6)   // handle 6 pixels once
                {
                    // pixel 0, 1
                    if (i >=  u4X)
                    {
                        u4Data2 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 8));
                        u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 12));
                        u4Data2 = (u4Data2 & 0x0000ffff) | ((u4GY & 0x3f) << 26) | (u4RCr << 16);
                        u4Data1 = (u4Data1 & 0xff000000) | (u4GY << 14) | (u4BCb << 4) | ((u4GY & 0x3c0) >> 6);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 8), u4Data2);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 12), u4Data1);
                    }

                    // pixel 2, 3
                    if (((i + 2) >=  u4X) && ((i + 2) < (u4X+u4Width)))
                    {
                        u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 4));
                        u4Data2 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 8));
                        u4Data1 = (u4Data1 & 0x000000ff) | (u4RCr << 8) | (u4GY << 18) | ((u4BCb & 0xf) << 28);
                        u4Data2 = (u4Data2 & 0xffff0000) | (u4GY << 6) | ((u4BCb & 0x3f0) >> 4);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 4), u4Data1);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 8), u4Data2);
                    }

                    // pixel 4, 5
                    if ((i + 4) < (u4X+u4Width))
                    {
                        u4Data2 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 4));
                        u4Data1 = ((u4GY & 0x3) << 30) | (u4BCb << 20) | (u4GY << 10) | (u4RCr);
                        u4Data2 = (u4Data2 & 0xffffff00) | ((u4GY & 0x3fc) >> 2);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset), u4Data1);
                        vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + 4), u4Data2);
                    }

                    u4DramUnitOffset += 16;
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }


            break;
        case 2:        // RGB24 bit
            u4BaseAddress += u4HL * u4Y * 16;
            for (j = u4Y; j < (u4Y+u4Height); j += 1)
            {
                for (i = u4X ; i < (u4X+u4Width); i ++)
                {
                    u4DramUnitOffset = (i >> 4) * 16 * 3;
                    idx = i - ((i >> 4) << 4);
                    switch (idx)
                    {
                        case 0:
                        case 4:
                        case 8:
                        case 12:
                            if (idx == 0)
                            {
                                u4Offset1 = 12;
                            }
                            else if (idx == 4)
                            {
                                u4Offset1 = 0;
                            }
                            else if (idx == 8)
                            {
                                u4Offset1 = 20;
                            }
                            else //if (idx == 12)
                            {
                                u4Offset1 = 40;
                            }
                            u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1));
                            u4Data1 = (u4Data1 & 0x000000ff) | (u4RCr << 8) | (u4BCb << 16) | (u4GY << 24);
                            vWriteDramD((u4BaseAddress + u4DramUnitOffset + u4Offset1), u4Data1);
                            break;

                        case 1:
                        case 5:
                        case 9:
                        case 13:
                            if (idx == 1)
                            {
                                u4Offset1 = 8;
                                u4Offset2 = 12;
                            }
                            else if (idx == 5)
                            {
                                u4Offset1 = 28;
                                u4Offset2 = 0;
                            }
                            else if (idx == 9)
                            {
                                u4Offset1 = 16;
                                u4Offset2 = 20;
                            }
                            else //if (idx == 13)
                            {
                                u4Offset1 = 36;
                                u4Offset2 = 40;
                            }
                            u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1));
                            u4Data2 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset2));
                            u4Data1 = (u4Data1 & 0x0000ffff) | (u4RCr << 16) | (u4BCb << 24);
                            u4Data2 = (u4Data2 & 0xffffff00) | u4GY;
                            vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1), u4Data1);
                            vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset2), u4Data2);
                            break;

                        case 2:
                        case 6:
                        case 10:
                        case 14:
                            if (idx == 2)
                            {
                                u4Offset1 = 4;
                                u4Offset2 = 8;
                            }
                            else if (idx == 6)
                            {
                                u4Offset1 = 24;
                                u4Offset2 = 28;
                            }
                            else if (idx == 10)
                            {
                                u4Offset1 = 44;
                                u4Offset2 = 16;
                            }
                            else //if (idx == 14)
                            {
                                u4Offset1 = 32;
                                u4Offset2 = 36;
                            }
                            u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1));
                            u4Data2 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset2));
                            u4Data1 = (u4Data1 & 0x00ffffff) | (u4RCr << 24) ;
                            u4Data2 = (u4Data2 & 0xffff0000) | (u4GY << 8) | (u4BCb);
                            vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1), u4Data1);
                            vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset2), u4Data2);
                            break;

                        case 3:
                        case 7:
                        case 11:
                        case 15:
                            if (idx == 3)
                            {
                                u4Offset1 = 4;
                            }
                            else if (idx == 7)
                            {
                                u4Offset1 = 24;
                            }
                            else if (idx == 11)
                            {
                                u4Offset1 = 44;
                            }
                            else //if (idx == 15)
                            {
                                u4Offset1 = 32;
                            }
                            u4Data1 = dwReadDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1));
                            u4Data1 = (u4Data1 & 0xff000000) | (u4RCr) | (u4BCb << 8) | (u4GY << 16);
                            vWriteDramD(VIRTUAL(u4BaseAddress + u4DramUnitOffset + u4Offset1), u4Data1);
                            break;
                        default:
                            //ASSERT(0);
                            break;
                    }
                }
                u4BaseAddress += u4HL * 16;  // calculate dram base address of next line
            }
            break;
        case 3:        // RGB30 bit
            Printf("Not Support data format!\n");
            break;

        default:
            //ASSERT(0);
            break;

    }

    #ifndef CC_COPLAT_MT82
    HalFlushInvalidateDCache();    
    #endif

    return 0;
}

