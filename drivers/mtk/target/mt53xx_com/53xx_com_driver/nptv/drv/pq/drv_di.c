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
 * $RCSfile: drv_di.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////


#include "vdp_drvif.h"
#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_nr.h"
#include "fbm_drvif.h"
#include "source_select.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_hdtv.h"
#include "drv_tvd.h"
#include "srm_drvif.h"
#include "mute_if.h"
#include "x_lint.h"
#include "tve_if.h"

LINT_EXT_HEADER_BEGIN
#include "hw_vdoin.h"
#include "hw_scpos.h"
#include "hw_ckgen.h"
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_nr.h"
#include "general.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"
#include "x_ckgen.h"
#include "nptv_debug.h"
#include "di_debug.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"

typedef enum{
    E_AF_Y = 0x01,
    E_AF_C = 0x02,
    E_BF_Y = 0x04,    
    E_BF_C = 0x08,
    E_CF_Y = 0x10,
    E_CF_C = 0x20,
    E_DF_Y = 0x40,    
    E_DF_C = 0x80,
    E_ALL_FLD = 0XFF,
} E_DI_FLD_R_EN;

typedef enum{
    E_SRC_W = 0x01,
    E_SRC_R = 0x02,
    E_MFF_W = 0x04,    
    E_MFF_R = 0x08,
    E_LTM_W = 0x10,
    E_LTM_R = 0x20,
    E_MV_W = 0x40,    
    E_MV_R = 0x80,
    E_DRAM_ALL_RW = 0XFF,
} E_DI_DRAM_RW_EN;

typedef enum{
    E_FREEZE_NON = 0,
    E_FREEZE_ON_STEP1,
    E_FREEZE_ON_STEP2,
    E_FREEZE_OFF_STEP1,
    E_FREEZE_OFF_STEP2,
    E_FREEZE_OFF_STEP3,    
} E_DI_FREEZE_STATE;

LINT_EXT_HEADER_END LINT_SUPPRESS_BRACE(572)        // Warning 572: Excessive shift value (precision 4 shifted right by 4) [MISRA Rule 38]

////////////////////////////////////////////////////////////////////////////////
// Parameters
////////////////////////////////////////////////////////////////////////////////

#ifdef DI_WA2_TOGGLE_BYPASS
UINT8 fgBypass[VDP_NS] = {0, 0};
UINT8 u1BypassCounter[VDP_NS] = {0, 0};
#define DI_WA2_COUNTER_TO_BYPASS 8
#endif

#define DI_WA3_TOGGLE_SCPIP_WEN_OFF
#define PSCAN_BANDWIDTH_ENHANCE
#define MDDI_MUTE_PERIOD 21//18 // by Linghui.zeng for CC_SCPOS_WA33 scaler import modify

VDP_MDDI_PRM_T _arMDDiPrm[VDP_NS];
UINT8 u1DeRange = 0x80;
UINT8 u1IIR_RATIO = 16;
UINT16 u2IIRM;
UINT16 u2IIRN;
UINT16 u2CalM;
UINT16 u2CalN;

// For pscan display mode hlen update
BOOL bUpdateHLen[VDP_NS] = {FALSE, FALSE};
UINT8 u1UpdateCnt[VDP_NS] = {0, 0};
#define MDDI_UPDATE_HLEN_DELAY 5


#ifdef CC_FLIP_MIRROR_SUPPORT
UINT8 u1FlipMirrorConfig = DI_NORMAL_MODE;
#endif

#define IS_5387_NonECO()       (IS_IC_5387() && (BSP_GetIcVersion() <= IC_VER_5387_AA))   
#define IS_5387_ECO()          (IS_IC_5387() && (BSP_GetIcVersion() >= IC_VER_5387_AB))
#define IS_5363_NonECO()       (IS_IC_5363() && (BSP_GetIcVersion() <= IC_VER_5363_AA))
#define IS_5363_ECO()          (IS_IC_5363() && (BSP_GetIcVersion() >= IC_VER_5363_AB))

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
MDDI_REGTBL_T const CODE MDDI_MIF_INIT[] =
{
{ MDDI_CTRL_00, 0xC9400000, 0xFFFFF0FF},
{ MDDI_CTRL_01, 0x002001FC, 0x00FFFFFF},
{ MDDI_CTRL_02, 0x2D1E02D0, 0x7F7FE7FF},
{ MDDI_CTRL_08, 0x00010006, 0x0FFF0FFF},
{ MDDI_CTRL_09, 0x10000000, 0x1FFF0FFF},
{ MDDI_CTRL_0A, 0x10000000, 0x1FFF0FFF},
{ MDDI_CTRL_0B, 0x00000080, 0x00F73FFF},
{ MDDI_CTRL_0C, 0x00010010, 0xE0777777},
{ MDDI_CTRL_0D, 0x00000000, 0x00030707},
{ MDDI_CTRL_0E, 0x0020D060, 0x007FF7FF},
{ MDDI_CTRL_0F, 0x8900008F, 0xFF3FF7FF},
{ MDDI_CTRL_10, 0x0C000000, 0x0F7FFFFF},
{ MDDI_CTRL_12, 0x00000000, 0x000F0777},
{ MDDI_CTRL_13, 0x020D1652, 0xF7FFFFFF},
{ MDDI_DA_00, 0x00010600, 0xFF7FFFF7},
{ MDDI_DA_01, 0x0539C000, 0x1FFFF800},
{ MDDI_DA_02, 0x0548C800, 0x1FFFF800},
{ MDDI_DA_03, 0x054CB000, 0x1FFFF800},
{ MDDI_DA_04, 0x055BD800, 0x1FFFF800},
{ MDDI_DA_05, 0x054CB000, 0x1FFFF800},
{ MDDI_DA_06, 0x055BD800, 0x1FFFF800},
{ MDDI_DA_07, 0x055F9800, 0x1FFFF800},
{ MDDI_DA_08, 0x0539C000, 0x1FFFF800},
{ MDDI_DA_09, 0x00030303, 0x00070707},
{ MDDI_DA_0A, 0x00000000, 0x0000FFFF},
{ MDDI_DA_0B, 0x055F9800, 0x1FFFF800},
{ MDDI_DA_0C, 0x05617800, 0x1FFFF800},
{ MDDI_DA_0D, 0x055F9800, 0x1FFFF800},
{ MDDI_DA_0E, 0x20080000, 0x3FFFFFFF},
{ MDDI_REGTBL_END, 0x00000000, 0x00000000}};


#ifdef LOG_MDDI_REG
MDDI_REGTBL_T MDDI_REG_LOG[1024]; 
UINT32 bLogReg = 0;

void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
    vRegWriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld);

    //Avoid interupt clear bit access
    if (u2Addr == 0x402C) return;
    
    MDDI_REG_LOG[bLogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM))|0x1;
    MDDI_REG_LOG[bLogReg].u4Value = u4Value;
    MDDI_REG_LOG[bLogReg].u4Mask = u4Fld;
    bLogReg = (bLogReg+1)&0x3FF;
}

void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk) 
{
    vRegWrite4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk);
    MDDI_REG_LOG[bLogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM));
    MDDI_REG_LOG[bLogReg].u4Value = u4Value;
    MDDI_REG_LOG[bLogReg].u4Mask = u4Msk;
    bLogReg = (bLogReg+1)&0x3FF;
}

void MDDI_PRINT_REG_LOG(void)
{
    UINT16 i, index, j = bLogReg;

    Printf("\nIndex    u2Addr         u4Value        u4Mask\n");    
    
    for (i=j; i<(1024+j); i++)
    {
        index = i&0x3FF;
        
        if ((index&0xFF) == 0xFF)
        {
            x_thread_delay(20);
        }
        
        Printf("%4d    %6x        %8x        %08x\n", index,
            MDDI_REG_LOG[index].u2Addr&0xFFFC, MDDI_REG_LOG[index].u4Value,
            (MDDI_REG_LOG[bLogReg].u2Addr & 0x1) 
            ? (Fld2Msk32(MDDI_REG_LOG[index].u4Mask))
            : MDDI_REG_LOG[index].u4Mask);
    }
}
#else
void MDDI_PRINT_REG_LOG(void)
{    
}
#endif

/**
* @brief Load register table
* @param u1VdpId VDP_1/VDP_2
* @param prRegTbl Register table
*/
void vDrvDILoadRegTbl(UINT8 u1VdpId, const MDDI_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != MDDI_REGTBL_END)
    {
    	MDDI_WRITE32_MSK(u1VdpId, prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
    	prRegTbl++;
    }
}


#ifdef DI_WA2_TOGGLE_BYPASS
void vDrvToggleBypass(UINT8 u1VdpId, UINT8 u1ModeChanged)
{
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) 
    {
        if (u1ModeChanged)
        {
            if (_arMDDiPrm[u1VdpId].u1OnOff == SV_FALSE)
            {
                vDrvDIBypass(u1VdpId, SV_OFF);
                fgBypass[u1VdpId] = SV_TRUE;
                u1BypassCounter[u1VdpId] = 0;
            }
            else
            {
                fgBypass[u1VdpId] = SV_FALSE;
            }
        }
        else
        {
            if(fgBypass[u1VdpId] == SV_TRUE)
            {
                if(u1BypassCounter[u1VdpId] < DI_WA2_COUNTER_TO_BYPASS)
                {
                    u1BypassCounter[u1VdpId]++;
                }
                else
                {
                    fgBypass[u1VdpId] = SV_FALSE;
                    vDrvDIBypass(u1VdpId, SV_ON);            
                    LOG(3,"Toggle Bypass PSCAN");
                }
            }
        }
    }
}
#endif


void vDrvDISetDeAdap(UINT8 u1Mode, UINT8 u1Value)
{
    if (u1Mode == 0)
    {
        u1DeRange = u1Value;
        Printf("De Range %d \n", u1DeRange);
    }
    else
    {
        u1IIR_RATIO = u1Value;
        Printf("De IIR  %d \n", u1IIR_RATIO);
    }    
}

/**
* @brief Get oversample information
* @param u1VdpId VDP_1/VDP_2
* @return oversample timing or not
*/
UINT8 bDrvDIIsOversample(UINT8 u1VdpId)
{
    if ((u1VdpId == VDP_1) &&
    (vTvdGetOverSampleEnable() || vHdtvGetOversampleForSD()))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}


/**
* @brief Get oversample information
* @param u1VdpId VDP_1/VDP_2
* @return oversample timing or not
*/
UINT8 bDrvDIIsOversampleOver720(UINT8 u1VdpId)
{
    return ((bDrvDIIsOversample(u1VdpId))
            &&(_arMDDiPrm[VDP_1].u2Width > (720 + DECODER_ADD_WIDTH)));
}


/**
* @brief Get NR data mode information
* @param u1VdpId VDP_1/VDP_2
* @param u4NRAug NR data mode information
*/
UINT8 bDrvGetNRDataMode(UINT8 u1VdpId)
{    
    UINT32 u4Mode = _arMDDiPrm[u1VdpId].rFbmPool.u4Mode; 

#if 0
    // Force no dram access to avoid garbage during atv channel search without mute 
    if ((bGetSignalType(u1VdpId) == SV_ST_TV) && (_fgAutoSearch))
    {
        return E_DI_NR_OFF;
    }
#endif

    if (_arMDDiPrm[VDP_1].u1DebugNRDataMode != E_DI_NR_UNKNOWN ||
       _arMDDiPrm[VDP_2].u1DebugNRDataMode != E_DI_NR_UNKNOWN)
    {
        //for setting NR data mode in CLI command
        if (_arMDDiPrm[u1VdpId].u1DebugNRDataMode != E_DI_NR_UNKNOWN)
        {
            if (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_0W0R)
            {
                return E_DI_NR_OFF;
            }  
            else
            {
                return _arMDDiPrm[u1VdpId].u1DebugNRDataMode;
            }
        }
        
        if(u1VdpId == VDP_1)
        {
            if ((_arMDDiPrm[VDP_2].u1DebugNRDataMode != E_DI_NR_UNKNOWN)
                || (_arMDDiPrm[VDP_1].u1DataMode == E_DI_YCBOB_0W0R))
            {
                return E_DI_NR_OFF;
            }
        }
        else if(u1VdpId == VDP_2)
        {
            if ((_arMDDiPrm[VDP_1].u1DebugNRDataMode != E_DI_NR_UNKNOWN)
                || (_arMDDiPrm[VDP_2].u1DataMode == E_DI_YCBOB_0W0R))
            {
                return E_DI_NR_OFF;
            }    
        }        
    }
    
    if (u4Mode & FBM_POOL_MODE_MDDI_NR_OFF)
    {
        return E_DI_NR_OFF;
    }
    else 
    {
        if (bIsScalerInput444(u1VdpId))
        {
            return E_DI_NR_RGB;
        }
        else
        {
            if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_C)
            {
                return ((bGetVideoDecType(u1VdpId) == SV_VD_MPEGHD) 
                                                ? E_DI_NR_YC_420 : E_DI_NR_YC_422);
            }
            else if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
            {
                return E_DI_NR_Y_ONLY;
            }
            else
            {
                return E_DI_NR_UNKNOWN;
            }
        }        
    }
}


/**
* @brief Get PSCAN data mode 
* @param u1VdpId VDP_1/VDP_2
* @return E_DI_NOBOB/E_DI_YCBOB/E_DI_CBOB/E_DI_FLIP_MIRROR
*/
UINT8 u1DrvDIGetDataModeFromSRM(UINT8 u1VdpId)
{
    UINT8 u1DataMode, u1PSCANDisplay, u1NROnOff;
    
    if (u1VdpId >= VDP_NS)
    {
        return E_DI_UNKNOWN;
    }
    
    u1PSCANDisplay = _arMDDiPrm[u1VdpId].u1SrmPSCANDisplay;
    u1NROnOff = ((bDrvGetNRDataMode(u1VdpId) ==  E_DI_NR_OFF) 
                    || (bDrvGetNRDataMode(u1VdpId) ==  E_DI_NR_UNKNOWN)) ? 0 : 1;

    switch (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & 0x00070000)
    {
    	case FBM_POOL_MODE_MDDI_FULL:
            #ifdef CC_FLIP_MIRROR_SUPPORT
                u1DataMode = u1PSCANDisplay ? E_DI_FLIP_MIRROR_1W4R : E_DI_FULL_1W3R;
            #else
                u1DataMode = u1PSCANDisplay ? E_DI_FULL_1W4R : E_DI_FULL_1W3R;
            #endif
            break;
    	case FBM_POOL_MODE_MDDI_BOB:
    	    #ifdef CC_FLIP_MIRROR_SUPPORT
                u1DataMode = (u1PSCANDisplay || u1NROnOff) ? E_DI_FLIP_MIRROR_YCBOB_1W1R : E_DI_YCBOB_0W0R;
            #else
                u1DataMode = (u1PSCANDisplay || u1NROnOff) ? E_DI_YCBOB_1W1R: E_DI_YCBOB_0W0R;
            #endif
            break;
    	case FBM_POOL_MODE_MDDI_CBOB:
    	    #ifdef CC_FLIP_MIRROR_SUPPORT
                u1DataMode = u1PSCANDisplay ? E_DI_FLIP_MIRROR_CBOB_1W4R : E_DI_CBOB_1W3R;
            #else
                u1DataMode = u1PSCANDisplay ? E_DI_CBOB_1W4R : E_DI_CBOB_1W3R;
            #endif
            break;
        #if 0
    	case FBM_POOL_MODE_MDDI_FLIP_MIRROR:
    		u1DataMode = E_DI_FLIP_MIRROR;
    		break;
        #endif
    	default:
            u1DataMode = (u1NROnOff) ? E_DI_YCBOB_1W1R : E_DI_YCBOB_0W0R;
            break;
    }

#if 0
    // Force no dram access to avoid garbage during atv channel search without mute 
    if ((bGetSignalType(u1VdpId) == SV_ST_TV) && (_fgAutoSearch))
    {
        u1DataMode = E_DI_YCBOB_0W0R;
    }
#endif

    return u1DataMode;
}


/**
* @brief Set PSCAN Y/C interpolation mode
* @param u1VdpId VDP_1/VDP_2
* @param bMode E_DI_NOBOB/E_DI_YCBOB/E_DI_CBOB/E_DI_FLIP_MIRROR
*/
void vDrvDISetInterpMode(UINT8 u1VdpId, E_DI_DMODE bMode)
{
    UINT8 u1FixBobY, u1FixBobC;

    switch (bMode)
    {
    	case E_DI_FULL_1W3R:
    	case E_DI_FULL_1W4R:
        case E_DI_FLIP_MIRROR_1W4R:
            u1FixBobY = 0x0;
            u1FixBobC = 0x0;
            break;
    	case E_DI_CBOB_1W3R:	
        case E_DI_CBOB_1W4R:
        case E_DI_FLIP_MIRROR_CBOB_1W4R:
            u1FixBobY = 0x0;
            u1FixBobC = 0x1;
           break;
    	case E_DI_YCBOB_0W0R:
    	case E_DI_YCBOB_1W1R:
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
    	default:
            u1FixBobY = 0x1;
            u1FixBobC = 0x1;
            break;
    }
    
    MDDI_WRITE_FLD(u1VdpId, MDDI_SB_05, u1FixBobY, SB_FIX_BOB_MODE_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SB_05, u1FixBobC, SB_FIX_BOB_MODE_C);

    #if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_SB_01, ((bMode == E_DI_CBOB_1W3R) 
        || (bMode == E_DI_CBOB_1W4R)) ? 0xF : 0, SB_FILM_DIS);
    }
    else
    #endif
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_SB_01, u1FixBobY, SB_FILM_DIS_Y);
        MDDI_WRITE_FLD(u1VdpId, MDDI_SB_01, u1FixBobC, SB_FILM_DIS_C);
    }
}

   
/**
* @brief Set PSCAN DRAM controller registers
* @param u1VdpId VDP_1/VDP_2
* @param u1DramCOnOff Enable/disable PSCAN DRAM controller
*/
void vDrvDISetDramCtrl(UINT8 u1VdpId, UINT8 u1DramCOnOff)
{
    UINT8 u1AF_2_CF;
    UINT8 u1AF_SRC_SEL;
    UINT8 u1FLD_R_EN, u1DRAM_RW_EN;

    /* DRAM controller part */
    if (u1DramCOnOff)
    {
    	switch (_arMDDiPrm[u1VdpId].u1DataMode)
    	{
            case E_DI_FULL_1W3R:			
                u1AF_SRC_SEL = 0x1;
            	u1AF_2_CF = 0x0;
            	u1FLD_R_EN = E_BF_Y | E_CF_Y | E_DF_Y | E_BF_C | E_CF_C | E_DF_C;
            	u1DRAM_RW_EN = E_DRAM_ALL_RW;
            	break;
            case E_DI_FULL_1W4R:
            case E_DI_FLIP_MIRROR_1W4R:
                u1AF_SRC_SEL = 0x0;
            	u1AF_2_CF = 0x0;
            	u1FLD_R_EN = E_ALL_FLD;
            	u1DRAM_RW_EN = E_DRAM_ALL_RW;
            	break;
            case E_DI_CBOB_1W3R:            
                u1AF_SRC_SEL = 0x1;
            	u1AF_2_CF = 0x0;
            	u1FLD_R_EN = E_BF_Y | E_CF_Y | E_DF_Y | E_CF_C;
            	u1DRAM_RW_EN = E_DRAM_ALL_RW;
            	break;
            case E_DI_CBOB_1W4R:
            case E_DI_FLIP_MIRROR_CBOB_1W4R:
                u1AF_SRC_SEL = 0x0;
            	u1AF_2_CF = 0x0;
            	u1FLD_R_EN = E_AF_Y | E_BF_Y | E_CF_Y | E_DF_Y | E_CF_C;
            	u1DRAM_RW_EN = E_DRAM_ALL_RW;
            	break;
            case E_DI_YCBOB_0W0R:			
                u1AF_SRC_SEL = 0x1;
            	u1AF_2_CF = 0x1;
            	u1FLD_R_EN = 0x0;
            	u1DRAM_RW_EN = 0;  
            	break;
            case E_DI_YCBOB_1W1R:
            case E_DI_FLIP_MIRROR_YCBOB_1W1R:
                u1AF_SRC_SEL = ((_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD) 
                                && (!_arMDDiPrm[u1VdpId].u1Interlace)) ? 0x01 : 0x0;
            	u1AF_2_CF = 0x1;
            	u1FLD_R_EN = E_AF_Y | E_AF_C;
            	u1DRAM_RW_EN = E_SRC_R | E_SRC_W;
            	break;
            default:
            	u1AF_SRC_SEL = 0x1;
            	u1AF_2_CF = 0x1;
            	u1FLD_R_EN = E_AF_Y | E_AF_C;
            	u1DRAM_RW_EN = 0x0;
            	break;
    	}
    }
    else
    {
    	u1AF_2_CF = 0x0;
    	u1AF_SRC_SEL = 0x0;
    	u1FLD_R_EN = 0x0;
    	u1DRAM_RW_EN = 0x0;
    }

    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, u1AF_2_CF, AF_TO_CF);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, u1AF_SRC_SEL, AF_SRC_SEL);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, u1DRAM_RW_EN, EN_DRAM_RW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, (_arMDDiPrm[u1VdpId].u1Interlace) ? u1FLD_R_EN : 0, ABCD_YC_EN);

    if (!u1DramCOnOff)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, 0x0, RG_NR_READ_ENABLE);
    }
    
    if (u1DramCOnOff)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, (_arMDDiPrm[u1VdpId].u1Interlace) ? 1 : 0, EN_SRCR);
        // Patch for Non-standard signal 0W0R have garbage on the bottom
        // WA : Enable E_SRC_W, Disable pixel write dram agent 
        if (((_arMDDiPrm[u1VdpId].u1DecType == SV_VD_TVD3D) || 
            (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_YPBPR))
            && ((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_0W0R) &&
            (bDrvVideoSignalStatus(u1VdpId) == SV_VDO_STABLE)))
        {
            MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 1, EN_SRCW);
            MDDI_WRITE_FLD(VDP_1, MDDI_DA_0F, 0, 
                (u1VdpId==VDP_1) ? R_DMAG2EN_PIX_W_M : R_DMAG3EN_PIX_W_P);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_DA_0F, 1, 
                (u1VdpId==VDP_1) ? R_DMAG2EN_PIX_W_M : R_DMAG3EN_PIX_W_P);
        }
        
        // enable HD_READ when normal/PIP mode. 
        if (_arMDDiPrm[VDP_1].u1OnOff && 
        (_arMDDiPrm[VDP_1].u2Width > (960 + DECODER_ADD_WIDTH)) &&
        ((SRM_GetTvMode() == SRM_TV_MODE_TYPE_NORMAL) || 
        (SRM_GetTvMode() == SRM_TV_MODE_TYPE_PIP))
        )
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_CTRL_01, 1, HD_READ);
        }
        else
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_CTRL_01, 0, HD_READ);
        }

        // Oversample when main path and TVD or Comp SD input
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, bDrvDIIsOversampleOver720(u1VdpId), SD_1440_MODE);

        // PSCAN display mode lock to scaler
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_13, _arMDDiPrm[u1VdpId].u1PSCANDisplay ? 1 : 0, OUT_TG_MODE);
    }
}


/**
* @brief Set bit resolution
* @param u1VdpId VDP_1/VDP_2
* @param u1BitRes Bit resolution (8 or 10)
*/
void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u18bit)
{
    vDrvDISetWritePrecision(u1VdpId, u18bit);

    vDrvNRSetBitSel(u1VdpId,(u18bit == 1) ? NR_BIT_SEL_8BIT : NR_BIT_SEL_10BIT);
    
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, u18bit, ERR_DFU);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, u18bit, ERR_DFU_RANDOM_INI);    
}


/**
* @brief Calculate and set base addresses for all buffers
* @param u1VdpId VDP_1/VDP_2
* @param prFbmPool FBM information
*/
void vDrvDISrmSetBufImpl(UINT8 u1VdpId, const MDDI_FBM_POOL_T * prFbmPool)
{
    UINT16 u2Width, u2Height;
    UINT8 u1YFieldNum, u1CFieldNum;
    UINT8 u18bit, u1BlockSize, u1NROnOff;
    UINT32 u4HPixelPageMsb, u4VPixelPageMsb;
    UINT32 u4HPixelPageLsb, u4VPixelPageLsb;
    UINT32 u4HPageMsb, u4VPageYMsb, u4VPageCMsb;
    UINT32 u4HPageLsb, u4VPageYLsb, u4VPageCLsb;
    UINT32 u4AddrYMsb, u4AddrUMsb, u4AddrVMsb;	
    UINT32 u4AddrYLsb, u4AddrULsb, u4AddrVLsb;	
    UINT32 u4HPageFlg, u4VPageFlg;
    UINT32 u4AddrFlg, u4AddrEnd;

    u18bit = (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0;    
    u2Width = _arMDDiPrm[u1VdpId].u2Width;
    u2Height = (_arMDDiPrm[u1VdpId].u1Interlace) 
                ? (_arMDDiPrm[u1VdpId].u2Height/2) 
                : _arMDDiPrm[u1VdpId].u2Height;
    u2Height = (_arMDDiPrm[u1VdpId].u1420Mode == MDDI_420_MODE) 
                ? ((u2Height % 2) ? (u2Height + 1) : u2Height) 
                : u2Height;
    
    vDrvDISetBitRes(u1VdpId, u18bit);

    //Decide buffer number according to data mode
    switch (_arMDDiPrm[u1VdpId].u1DataMode)
    {
        case E_DI_FULL_1W3R:
            u1YFieldNum = 4; 
            u1CFieldNum = 4;
            break;
        case E_DI_FULL_1W4R:
            u1YFieldNum = 5; 
            u1CFieldNum = 5;
            break;
        case E_DI_CBOB_1W3R:
            u1YFieldNum = 4;
            u1CFieldNum = 3; 
            break;
        case E_DI_CBOB_1W4R:
            u1YFieldNum = 5; 
            u1CFieldNum = 4; 
            break;
        case E_DI_YCBOB_0W0R:
            u1YFieldNum = 0; 
            u1CFieldNum = 0;
            break;
        case E_DI_YCBOB_1W1R:
            u1YFieldNum = 2; 
            u1CFieldNum = 2;
            break;
        case E_DI_FLIP_MIRROR_1W4R:
            u1YFieldNum = 6; 
            u1CFieldNum = 6;
            break;
        case E_DI_FLIP_MIRROR_CBOB_1W4R:
            u1YFieldNum = 6; 
            u1CFieldNum = 5;
            break;
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
            u1YFieldNum = 3; 
            u1CFieldNum = 3;
            break;
        default:
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
    }


    u1BlockSize = MDDI_READ_FLD(u1VdpId, MDDI_DA_00, RG_BLOCK_SIZE);
    u1NROnOff = ((_arMDDiPrm[u1VdpId].u1NRDataMode ==  E_DI_NR_OFF) 
                    || (_arMDDiPrm[u1VdpId].u1NRDataMode ==  E_DI_NR_UNKNOWN)) ? 0 : 1;

    if ((_arMDDiPrm[u1VdpId].u1Interlace == 0) && u1NROnOff) // progressive with NR on
    {
        u1YFieldNum = (u1YFieldNum > 0) ? (u1YFieldNum - 1) : u1YFieldNum;
        u1CFieldNum = (u1CFieldNum > 0) ? (u1CFieldNum- 1) : u1CFieldNum;
    }

    //Set the number of frame buffer
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_09, u1YFieldNum ? (u1YFieldNum - 1) : 0, RG_FRAME_NUM_Y);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_09, u1CFieldNum ? (u1CFieldNum - 1) : 0, RG_FRAME_NUM_U);
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_09, u1CFieldNum ? (u1CFieldNum - 1) : 0, RG_FRAME_NUM_V);	

    LOG(6, "u2Width %d  u2Height %d u1YFieldNum %d  u1CFieldNum %d \n", 
        u2Width, u2Height, u1YFieldNum, u1CFieldNum);
    
    /* Calculate buffer size */
    u4HPixelPageMsb = 1 << (6 + u1BlockSize);
    u4VPixelPageMsb = 1 << (5 - u1BlockSize);
    u4HPageMsb = MPY_AND_CEIL(u2Width, 1, u4HPixelPageMsb);  
    u4VPageYMsb = MPY_AND_CEIL((u2Height * u1YFieldNum), 1, u4VPixelPageMsb);
    u4VPageCMsb = MPY_AND_CEIL((u2Height * u1CFieldNum), 1, u4VPixelPageMsb);
    u4VPageCMsb = (_arMDDiPrm[u1VdpId].u1420Mode == MDDI_420_MODE) ? 
        MPY_AND_CEIL(u4VPageCMsb, 1, 2) : u4VPageCMsb;

    u4HPixelPageLsb = 256;
    u4VPixelPageLsb = 32;
    
    u4HPageLsb = u18bit ? 0 : (MPY_AND_CEIL(u2Width, 1, u4HPixelPageLsb));
    u4VPageYLsb = u18bit ? 0 : (MPY_AND_CEIL((u2Height * u1YFieldNum), 1, u4VPixelPageLsb));
    u4VPageCLsb = u18bit ? 0 : (MPY_AND_CEIL((u2Height * u1CFieldNum), 1, u4VPixelPageLsb));
    u4VPageCLsb = (_arMDDiPrm[u1VdpId].u1420Mode == MDDI_420_MODE) ? 
        MPY_AND_CEIL(u4VPageCLsb, 1, 2) : u4VPageCLsb;

    //Flag
    u4HPageFlg = MPY_AND_CEIL(u2Width, 1, 512);
    u4VPageFlg = MPY_AND_CEIL(u2Height*2, 1, 32);

#ifdef PSCAN_BANDWIDTH_ENHANCE    
    u4AddrYMsb = prFbmPool->u4Addr>>11;
    u4AddrYLsb = MAKE_ALIGN_TO((u4AddrYMsb + (u4HPageMsb * u4VPageYMsb)), 3, (((prFbmPool->u4Addr>>11)%8)+2)%8);
    u4AddrUMsb = MAKE_ALIGN_TO((u4AddrYLsb + (u4HPageLsb * u4VPageYLsb)), 3, (((prFbmPool->u4Addr>>11)%8)+4)%8);
    u4AddrULsb = MAKE_ALIGN_TO((u4AddrUMsb + (u4HPageMsb * u4VPageCMsb)), 3, (((prFbmPool->u4Addr>>11)%8)+6)%8);
#else
    u4AddrYMsb = prFbmPool->u4Addr>>11;
    u4AddrYLsb = u4AddrYMsb + (u4HPageMsb * u4VPageYMsb);
    u4AddrUMsb = u4AddrYLsb + (u4HPageLsb * u4VPageYLsb);
    u4AddrULsb = u4AddrUMsb + (u4HPageMsb * u4VPageCMsb);
#endif

    LOG(6, "u4HPageMsb  %d u4HPageLsb  %d \n", u4HPageMsb,  u4HPageLsb);
    LOG(6, "u4VPageYMsb %d u4VPageYLsb %d \n", u4VPageYMsb, u4VPageYLsb);
    LOG(6, "u4VPageCMsb %d u4VPageCLsb %d \n", u4VPageCMsb, u4VPageCLsb);
    LOG(6, "u4HPageFlg  %d u4VPageFlg  %d \n", u4HPageFlg,  u4VPageFlg);

    // RGB case
    if (_arMDDiPrm[u1VdpId].u1NRDataMode == E_DI_NR_RGB)
    {
        #ifdef PSCAN_BANDWIDTH_ENHANCE
        u4AddrVMsb = MAKE_ALIGN_TO((u4AddrULsb + (u4HPageLsb * u4VPageCLsb)), 3, (((prFbmPool->u4Addr>>11)%8)+0)%8);
        u4AddrVLsb = MAKE_ALIGN_TO((u4AddrVMsb + (u4HPageMsb * u4VPageCMsb)), 3, (((prFbmPool->u4Addr>>11)%8)+2)%8);
        #else
        u4AddrVMsb = u4AddrULsb + (u4HPageLsb * u4VPageCLsb);
        u4AddrVLsb = u4AddrVMsb + (u4HPageMsb * u4VPageCMsb);
        #endif
    }
    else
    {
        u4AddrVMsb = u4AddrUMsb;
        u4AddrVLsb = u4AddrULsb;
    }
    
    u4AddrFlg = u4AddrVLsb + (u4HPageLsb * u4VPageCLsb);

    //There is only MA for sub path                         
    if ((u1VdpId == VDP_2) 
        || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_0W0R)
        || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R))
    {
        u4AddrEnd = u4AddrFlg;
    }
    else
    {            
        u4AddrEnd = u4AddrFlg + (u4HPageFlg * u4VPageFlg);
    }

    LOG(6, "u4AddrYMsb %x u4AddrYLsb %x\n", u4AddrYMsb, u4AddrYLsb);
    LOG(6, "u4AddrUMsb %x u4AddrULsb %x\n", u4AddrUMsb, u4AddrULsb);
    LOG(6, "u4AddrVMsb %x u4AddrVLsb %x\n", u4AddrVMsb, u4AddrVLsb);
    LOG(6, "u4AddrFlg  %x u4AddrEnd  %x\n", u4AddrFlg, u4AddrEnd);
 
    vDrvDISetBufferAddr(u1VdpId, u4AddrYMsb, u4AddrYLsb, u4AddrUMsb, u4AddrULsb, u4AddrVMsb, u4AddrVLsb, u4AddrFlg);

    u4AddrYMsb = (u4AddrYMsb < (prFbmPool->u4Addr>>11)) ? (prFbmPool->u4Addr>>11) : u4AddrYMsb;
    u4AddrFlg = (u4AddrFlg > ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11)) ? ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11) : u4AddrFlg;
    u4AddrEnd = (u4AddrEnd > ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11)) ? ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11) : u4AddrEnd;

    if (u4AddrFlg > ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11))
    {
        Printf("\n\nRG_FLAG_ADDR_BASE is larger than SRM allocated size!\n");
    }
    if (u4AddrEnd > ((prFbmPool->u4Addr + prFbmPool->u4Size)>>11))
    {
        Printf("\n\nRG_FLAG_ADDR_END is larger than SRM allocated size!\n");
    }
    vDrvDISetBufferProtect(u1VdpId, u4AddrYMsb, u4AddrFlg, u4AddrEnd);
}

void vDrvDIChangeISRType(UINT8 u1VdpId, UINT8 bType)
{
    if (u1VdpId < VDP_NS)  
    {
       MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, bType, INT_TYPE);
    }
}

#if CC_SCPOS_WA_FLIP_IMPORT
HANDLE_T _hFlipImportSema[2] = {NULL_HANDLE, NULL_HANDLE};
HANDLE_T _hFlipImportThread[2] = {NULL_HANDLE, NULL_HANDLE};



static VOID _ScpipFlipImortThreadMain(VOID* pv_arg)
{
    while(1)
    {
        VERIFY(x_sema_lock(_hFlipImportSema[VDP_1], X_SEMA_OPTION_WAIT)== OSR_OK);
        vScpipTriggerFlipImport(VDP_1);
    }
}
static VOID _ScpipFlipImortThreadPIP(VOID* pv_arg)
{
    while(1)
    {
        VERIFY(x_sema_lock(_hFlipImportSema[VDP_2], X_SEMA_OPTION_WAIT)== OSR_OK);
        vScpipTriggerFlipImport(VDP_2);
    }
}
#endif

/**
* @brief PSCAN ISR
* @param u2Vector ISR vector
*/
HANDLE_T _hDIPQIsrSema;

void vDrvDIISR(UINT16 u2Vector)
{
    UINT8 u1IntOn[2] = {1, 0};
    UINT8 u1VdpId;

    ASSERT(u2Vector == VECTOR_PSCAN);

    #if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() >= IC_VER_5387_AB) // After ECO
    #endif
    {
        u1IntOn[VDP_1] = MDDI_READ_FLD(VDP_1, MDDI_STA_01, STA_ISR_IND);
        u1IntOn[VDP_2] = MDDI_READ_FLD(VDP_2, MDDI_STA_01, STA_ISR_IND);
    }

    for (u1VdpId = VDP_1; u1VdpId < VDP_NS; u1VdpId++)    
    {
        //LOG(6, "DI[%d] ISR  %d\n", u1VdpId, MDDI_READ_FLD(u1VdpId, MDDI_STA_02, STATUS_OUT_VCNT));
        
        if (u1IntOn[u1VdpId])
        {
            if (u1VdpId == VDP_1)
            {
                vDrvDISetQualityISR();
            }
            
            vDrvDIFreezeFlow(u1VdpId);   
            vDrvDICsPatch(u1VdpId);  
            MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, SV_ON, INT_CLEAR);//Clear interrupt
            #if CC_SCPOS_WA_FLIP_IMPORT
            x_sema_unlock(_hFlipImportSema[u1VdpId]);   
            #endif
        }
    }
    
    x_sema_unlock(_hDIPQIsrSema);    
    VERIFY(BIM_ClearIrq(VECTOR_PSCAN));
}

/**
* @brief This is to register PSCAN ISR
*/
void vDrvDIInitISR(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1Initialized)
    {
        VERIFY(x_sema_create(&_hDIPQIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    
    	if (x_reg_isr(VECTOR_PSCAN, vDrvDIISR, &pfnOldIsr) != OSR_OK)
    	{
    		LOG(7, "Error: fail to register PSCAN ISR!\n");
    	}
    	u1Initialized = SV_TRUE;
        MDDI_WRITE_FLD(VDP_1, MDDI_CTRL_0B, E_DI_OUT_V_F, INT_TYPE);
        MDDI_WRITE_FLD(VDP_2, MDDI_CTRL_0B, E_DI_OUT_V_F, INT_TYPE);

        #if CC_SCPOS_WA_FLIP_IMPORT
        VERIFY(x_sema_create(&_hFlipImportSema[VDP_1], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_sema_create(&_hFlipImportSema[VDP_2], X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
        VERIFY(x_thread_create(&_hFlipImportThread[VDP_1], 
               "h_FlipImport_main", 
               2048, 
               20, 
               _ScpipFlipImortThreadMain, 
               0,
               NULL)==OSR_OK);
        VERIFY(x_thread_create(&_hFlipImportThread[VDP_2], 
               "h_FlipImport_pip", 
               2048, 
               20, 
               _ScpipFlipImortThreadPIP, 
               0,
               NULL)==OSR_OK);
        #endif
        
        LOG(7, "PSCAN ISR registered!\n");
    }
}

/**
* @brief This is to register PSCAN ISR
*/
void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable)
{
#if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
    {
        if (u1VdpId != VDP_1)
        {
            LOG(6, "VDP_2 not support Freeeze\n");
            return;
        }
    }
#endif
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, u1Enable, INT_ENABLE);
}


/**
* @brief Thread Pending Until PSCAN Interrupt Finished
* @param None
*/
void vDrvWaitDIVSyncSema(void)
{
    VERIFY (x_sema_lock(_hDIPQIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}


/**
* @brief Initialize PSCAN register values
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIInit(UINT8 u1VdpId)
{
    vDrvDIConfigFlipMirror();
    vDrvDIReset(u1VdpId, SV_ON);
    
    //Initialize PSCAN structure
    _arMDDiPrm[u1VdpId].u1OnOff = 0xFF;
    _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = 0xFF;
    _arMDDiPrm[u1VdpId].u1DebugIndex = 0;
    _arMDDiPrm[u1VdpId].u1Freeze = 0;
    _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugNRDataMode = E_DI_NR_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1CsEnable = SV_OFF;
    _arMDDiPrm[u1VdpId].u1DeRule = 0xFF;
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].u2Width = 720;
    _arMDDiPrm[u1VdpId].u2Height = 576;
    _arMDDiPrm[u1VdpId].u1TVmode = SRM_TV_MODE_TYPE_NORMAL;
        
    #if PREPROC_ENABLE_SYNC_REGEN
    _arMDDiPrm[u1VdpId].u2HClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2VClip = 0xFFFF;
    #endif
    
    vDrvDILoadRegTbl(u1VdpId, MDDI_MIF_INIT);
    vDrvDIInitQuality(u1VdpId);
    vDrvDIInitClock(u1VdpId);    
    vDrvDIInitISR();

    vDrvDIReset(u1VdpId, SV_OFF);
}

/**
* @brief Reset software global variables
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIResetSw(UINT8 u1VdpId)
{
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].u1DeRule = 0xFF;
}

/**
* @brief Set demo mode active region
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @param u1Region SV_LEFT/SV_RIGHT
* @param u2Center Center line position
*/
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center)
{
	_arMDDiPrm[SV_VP_MAIN].u1Region = u1Region;
	_arMDDiPrm[SV_VP_MAIN].u2Center = u2Center;

    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06,	(_arMDDiPrm[SV_VP_MAIN].u1Region == SV_LEFT)?1:0, SB_DEMO_SIDE);
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, u1OnOff, SB_DEMO_MODE_MEMA);    	
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_SB_06, u2Center, SB_DEMO_SPLIT);
}


/**
* @brief Set total number of lines to be clipped for ONLY DTV case
* @param u1VdpId VDP_1/VDP_2
* @param u2VClip Total number of lines to be clipped
*/
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip)
{
    if (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD)
    {
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, (_arMDDiPrm[u1VdpId].u2VClip + u2DTVClip), Y_SHIFT);

    	_arMDDiPrm[u1VdpId].u2DTVClip = u2DTVClip;
    }
}


/**
* @brief Set vertical direction related parameters
* @param u1VdpId VDP_1/VDP_2
* @param u2VTotal VTotal
* @param u2Height Height of video
* @param u2Clip Total number of lines to be clipped
*/
void vDrvDISetVParam(UINT8 u1VdpId, UINT16 u2VTotal, UINT16 u2Height, UINT16 u2Clip)
{
    UINT16 u2Vdew = MPY_AND_TRUN(u2Height, 1, 2);
    u2Vdew = (_arMDDiPrm[u1VdpId].u1420Mode == MDDI_420_MODE) 
                ? ((u2Vdew % 2) ? (u2Vdew + 1) : u2Vdew) 
                : u2Vdew;

    //Height of active video (in unit of 2 lines)
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_02, u2Vdew, VDEW);

    //Total number of lines to be clipped
    #if PREPROC_ENABLE_SYNC_REGEN
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, 0x0, Y_SHIFT);
    #else
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, u2Clip, Y_SHIFT);
    #endif

    //FRC Line
    if (!MDDI_READ_FLD(u1VdpId, MDDI_DA_00, RG_DISPLAY_MODE))
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0F, 0, FRC_LINE);
    }
    
    //Output Vtotal under free run mode
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_13, u2VTotal, OUT_VT);
}


/**
* @brief Set SRCW_V_BGN, KERNEL_H_BGN, KERNEL_V_BGN
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetFldParam(UINT8 u1VdpId)
{
    UINT8 u1S_V_Bgn = (u1VdpId == VDP_1) ? 2 : 8;
    UINT8 u1K_V_Bgn = (u1VdpId == VDP_1) ? 3 : 9;
    UINT8 u1K_H_Bgn = (_arMDDiPrm[u1VdpId].u2Height <= 576) ? 0x18 : 0x14;

    vDrvDISetBgn(u1VdpId, u1S_V_Bgn, u1K_H_Bgn, u1K_V_Bgn);

    #if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
    {
        vDrvDISetFldPrediction(u1VdpId, SV_OFF);
    }
    else
    #endif
    {
        // DTV, disable field prediction
        // [DTV00138714] active Height is too small(288), the video will be dithering
        // Enable field prediction can fix this problem.
        // [DTV00139695]active Width is too small(360), the video will dithering
        // Enable field prediction can fix this problem
        if ((_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD) &&
        (_arMDDiPrm[u1VdpId].u2Height > 288)&&(_arMDDiPrm[u1VdpId].u2Width > 360))        
        {
            vDrvDISetFldPrediction(u1VdpId, SV_OFF);
        }
        else
        {
            vDrvDISetFldPrediction(u1VdpId, SV_ON);
        }
    }
}

/**
* @brief Set horizontal direction related parameters
* @param u1VdpId VDP_1/VDP_2
* @param u2HTotal HTotal
* @param u2OutputWidth Width of video output from PSCAN
* @param u2Clip Total number of lines to be clipped
*/
void vDrvDISetHParam(UINT8 u1VdpId, UINT16 u2HTotal, UINT16 u2Width, UINT16 u2Clip)
{
    UINT32 u2OutHT;
    UINT32 u4DataRate;

    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0E, (bDrvDIIsOversampleOver720(u1VdpId) ? 0xC0 : 0x60), H_BLANK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_02, u2Width, HDEW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_02, MPY_AND_CEIL(u2Width, 1, 16), H_PITCH);

    // 1W4R + not pscan dispmode
    if (_arMDDiPrm[u1VdpId].u2VTotal == 1125)
    {
    	u4DataRate = 14835; // 148.35MHz
    }
    else
    {
    	u4DataRate = 2700; // 27 MHz
    }

    LOG(6, "Oversample %d   HTotal %d  Clock rate %d \n", 
            bDrvDIIsOversampleOver720(u1VdpId), 
            _arMDDiPrm[u1VdpId].u2HTotal, 
            u4DrvDIGetClkRate(u1VdpId));

    u2OutHT = (bDrvDIIsOversample(u1VdpId)) ? (_arMDDiPrm[u1VdpId].u2HTotal/2) : _arMDDiPrm[u1VdpId].u2HTotal;
    u2OutHT = u2OutHT * u4DrvDIGetClkRate(u1VdpId) / u4DataRate;
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_13, u2OutHT, OUT_HT);

    //Total number of pixels to be clipped
    #if PREPROC_ENABLE_SYNC_REGEN
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, 0x0, X_SHIFT);
    #else
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, u2Clip, X_SHIFT);
    #endif

    if ((bGetVideoDecType(u1VdpId) == SV_VD_TVD3D) &&
        (bGetSignalType(u1VdpId) == SV_ST_TV))
    {
        u2CalN = _arMDDiPrm[u1VdpId].u2HTotal>>1;
        u2CalM = u2OutHT>>1;
        u2IIRN = u2CalN;
        u2IIRM = u2CalM;    
    }
}


/**
 * @brief Return the minimum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MiniInputWidth The minimum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId)
{
    UINT32 u4MiniInputWidth;

    #if 1
    //The minimum width is defined as 360 for the following reasons
    //a. For normal signal, the test result for 480i/576i/1080i is 290 (1W3R)
    //b. For non-standard signal, the test result for 480i/576i is 346 (1W3R)
    u4MiniInputWidth = 360;
    #endif

    return u4MiniInputWidth;
}


/**
 * @brief Return the maximum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MaxInputWidth The maximum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId)
{
    UINT32 u4MaxInputWidth;
    SRM_TV_MODE_T eMode = SRM_GetTvMode();

    //The FIFO width limitation for main path under POP mode is 1920, in actual.
    //Now we return 960 because FBM buffer size configuration
    if (eMode == SRM_TV_MODE_TYPE_POP)
    {
    	u4MaxInputWidth = (u1VdpId == VDP_1) ? 960 : 720;
    }
    else if ((eMode == SRM_TV_MODE_TYPE_PIP) && (u1VdpId == VDP_2))
    {
    	u4MaxInputWidth = 720; //Y3DCBOB
    }
    else
    {
    	u4MaxInputWidth = 1920;
    }

    return u4MaxInputWidth;
}


/**
* @brief Turn on/off trick mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @warning This should be invoked by TVD only
*/
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    vDrvDISetAutoTrickOnOff(u1VdpId,u1OnOff);

    //Trick mode is an one-field-only mode. PSCAN has to work in 2D mode under this case
    vDrvDISetInterpMode(u1VdpId, (u1OnOff ? E_DI_YCBOB_0W0R : _arMDDiPrm[u1VdpId].u1DataMode));
}


/**
* @brief Turn on/off frame encode mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @param u1BtmFirst SV_TRUE/SV_FALSE
* @warning This should be invoked by B2R only
*/
void vDrvDIFrmEncModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1BtmFirst)
{
    //TODO: check if this is necessary in MT5387
    #if 0
    MDDI_WRITE_FLD(u1VdpId, MDDI_MIXER_03, (u1OnOff ? 1 : 0), FRAME_ENCODE_C);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MIXER_03, (u1BtmFirst ? 1 : 0), BOTTOM_FIRST);
    #endif
}


/**
* @brief Set AUTO STOP
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, u1OnOff, SRCW_AUTO_STOP);
}

/**
* @brief Set AUTO TRICK ON/OFF
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetAutoTrickOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, (u1OnOff ? 1 : 0), AUTO_TRICK_ON);
}

/**
* @brief Get AUTO STOP
* @param u1VdpId VDP_1/VDP_2
* @return u1OnOff On/Off
*/
UINT8 u1DrvDIGetAutoStop(UINT8 u1VdpId)
{
    return MDDI_READ_FLD(u1VdpId, MDDI_CTRL_10, SRCW_AUTO_STOP);
}


/**
* @brief DI Field Prediction
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
*/
void vDrvDISetFldPrediction(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, u1OnOff, FLD_PRED_EN);
}


/**
* @brief DI Frame Repeat
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
* @warning This should be invoked by B2R only
*/
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff)
{   
    if (fgMainMuteOn())
    {
        u1OnOff = SV_OFF;
    }
    vDrvDIFilmModeFreeze(u1VdpId, (!u1OnOff));
    vDrvDISetAutoStop(u1VdpId, u1OnOff);
}


/**
* @brief Check to see if PSCAN should be turned on
* @param u1VdpId VDP_1/VDP_2
* @return SV_TRUE/SV_FALSE
*/
UINT8 u1DrvDIMaskedOnOff(UINT8 u1VdpId)
{
    UINT8 u1IsChannelOn = (u1VdpId == VDP_1) ? _rMChannel.bIsChannelOn : _rPChannel.bIsChannelOn;
    UINT8 u1SigStatus = bDrvVideoSignalStatus(u1VdpId);
    UINT8 u1SigType = bGetSignalType(u1VdpId);

    if ((u1IsChannelOn == SV_FALSE)     //When channel is off
    	|| (u1SigStatus == SV_VDO_NOSUPPORT)        //When timing is not supported
    	//Snow flake screen should be displayed even signal is not stable (TV)
    	|| ((u1SigType != SV_ST_TV) && ((u1SigStatus == SV_VDO_NOSIGNAL) || (u1SigStatus == SV_VDO_UNKNOWN)))
        || ((u1SigType != SV_ST_MPEG) && (!bDrvVideoIsSrcInterlace(u1VdpId)))
        //for Progressive Mpeg 420->422 
        || ((u1SigType == SV_ST_MPEG) && (!bDrvVideoIsSrcInterlace(u1VdpId)) && (wDrvVideoGetHTotal(u1VdpId) >= 2200)))
    {
    	return SV_OFF;
    }
    else
    {
        return SV_ON;
    }
}


/**
* @brief Get SRM PSCAN display mode 
* @param u1VdpId VDP_1/VDP_2
* @return True/False
*/
UINT8 u1DrvDIGetDispmodeFromSRM(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1SrmPSCANDisplay;
}


/**
* @brief Check if PSCAN needs to do mode change
* @param u1VdpId VDP_1/VDP_2
* @return SV_TRUE/SV_FALSE
*/
UINT8 u1DrvDIModeChange(UINT8 u1VdpId)
{
    UINT8 u1Ret, u1OnOff, u1DecType, u1Interlace, u1BitRes;
    UINT8 u1PSCANDisplay, u1DataMode, u1NRDataMode;
    UINT16 u2HTotal, u2VTotal, u2Width, u2Height;
    UINT16 wSrcXOffset, wSrcYOffset, u2BufWidth, u2BufHeight;
    UINT32 u4BufAddr, u4NTDOfst;
    PSCAN_CLIP_INFO_T ScalerOverscanInfo;
    SRM_TV_MODE_T bMode;

    if (u1VdpId >= VDP_NS)
    {
        return SV_FALSE;
    }

    /* Get video signal status */
    u1OnOff = u1DrvDIMaskedOnOff(u1VdpId);
    u1DecType = bGetVideoDecType(u1VdpId);
    u1Interlace = bDrvVideoIsSrcInterlace(u1VdpId);
    u2VTotal = wDrvVideoGetVTotal(u1VdpId);
    u2HTotal = wDrvVideoGetHTotal(u1VdpId);
    bMode = SRM_GetTvMode();
     
    if (bScalerGetClipInfo(u1VdpId, &ScalerOverscanInfo))
    {
    	u2Width = ((ScalerOverscanInfo.wWidth+1)>>1)<<1;
    	u2Height = ScalerOverscanInfo.wHeight;
    	wSrcYOffset = ScalerOverscanInfo.wY;
    	wSrcXOffset = ScalerOverscanInfo.wX;
    }
    else
    {
    	wSrcXOffset = _arMDDiPrm[u1VdpId].u2HClip;
    	wSrcYOffset = _arMDDiPrm[u1VdpId].u2VClip;
    	u2Width = _arMDDiPrm[u1VdpId].u2Width;
    	u2Height = _arMDDiPrm[u1VdpId].u2Height;
    }

    #ifdef CC_TVE_UPSCALE_WA
    UINT8 ucFmt;
    UINT8 ucEnable;
    TVE_GetEnable(TVE_1, &ucEnable);
    TVE_GetFmt(TVE_1, &ucFmt);

    // TVE is enabled && VGA/HDMI progressive timing
    if ((u1VdpId == VDP_2) && ucEnable && (!u1Interlace) &&
       ((u1DecType == SV_VD_VGA) || (u1DecType == SV_VD_DVI)))
    {
        if (ucFmt == TVE_FMT_480I)
        {
            if (u2Height < 480)
            {
                u1OnOff = SV_ON;
                u2Height = u2Height * 2;
            }
        }
        else if (ucFmt == TVE_FMT_576I)
        {
            if (u2Height < 576)
            {
                u1OnOff = SV_ON;
                u2Height = u2Height * 2;
            }
        }	
    }
    #endif
        
    //Get SRM buffer information
    u4BufAddr = _arMDDiPrm[u1VdpId].rFbmPool.u4Addr;
    u2BufWidth = _arMDDiPrm[u1VdpId].rFbmPool.u4Width;
    u2BufHeight = _arMDDiPrm[u1VdpId].rFbmPool.u4Height;
    u1BitRes = (_arMDDiPrm[u1VdpId].rFbmPool.fg10bit) ? 10 : 8;
    
    //Get PSCAN display mode
    if (_arMDDiPrm[u1VdpId].u1DebugPSCANDisplay != 0xFF)
    {
        u1PSCANDisplay = _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay;
        _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = 0xFF;
    }
    else
    {
        u1PSCANDisplay = u1DrvDIGetDispmodeFromSRM(u1VdpId);
    }

    // handle non-standard signal at dram mode
    if ((!u1PSCANDisplay) &&
    (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_YPBPR))
    {
        u4NTDOfst = u4DrvVideoGetNSDOffset(u1VdpId);
        if (u4NTDOfst != 0x800)
        {
            if (((u2Height + u4NTDOfst) > 0x800) && (u4NTDOfst < 0x800))
            {
                LOG(3, "Non-Standard Signal Detected (%d)! Ofst: %d\n", u1VdpId, u4NTDOfst);
                u2Height = u2Height + u4NTDOfst - 0x800;                
            }
        }
    }
    
    //Get data mode
    if (_arMDDiPrm[u1VdpId].u1DebugDataMode != E_DI_UNKNOWN)
    {
    	u1DataMode = _arMDDiPrm[u1VdpId].u1DebugDataMode;
    }
    else
    {
        u1DataMode = u1DrvDIGetDataModeFromSRM(u1VdpId);
    }

    //Get NR data mode
    u1NRDataMode = bDrvGetNRDataMode(u1VdpId);

    //Please note that u1OnOff is not updated here for control flow purpose
    if ((_arMDDiPrm[u1VdpId].u1OnOff != u1OnOff)
    	|| (_arMDDiPrm[u1VdpId].u1DecType != u1DecType)
    	|| (_arMDDiPrm[u1VdpId].u1Interlace != u1Interlace)
    	|| (_arMDDiPrm[u1VdpId].u2HTotal != u2HTotal)
    	|| (_arMDDiPrm[u1VdpId].u2VTotal != u2VTotal)
    	|| (_arMDDiPrm[u1VdpId].u2Width != u2Width)
    	|| (_arMDDiPrm[u1VdpId].u2Height != u2Height)
    	//The following settings are controlled by SRM in vDrvDISrmSetBuf function call
    	|| (_arMDDiPrm[u1VdpId].u4BufAddr != u4BufAddr)
    	|| (_arMDDiPrm[u1VdpId].u1BitRes != u1BitRes)
    	|| (_arMDDiPrm[u1VdpId].u1DataMode != u1DataMode)
        || (_arMDDiPrm[u1VdpId].u1NRDataMode != u1NRDataMode)
    	|| (_arMDDiPrm[u1VdpId].u1PSCANDisplay != u1PSCANDisplay)
    	|| (_arMDDiPrm[u1VdpId].u1TVmode != bMode))
    {
        _arMDDiPrm[u1VdpId].u1TVmode = bMode;
    	_arMDDiPrm[u1VdpId].u1OnOff = u1OnOff;
    	_arMDDiPrm[u1VdpId].u1DecType = u1DecType;
    	_arMDDiPrm[u1VdpId].u1Interlace = u1Interlace;
    	_arMDDiPrm[u1VdpId].u2HTotal = u2HTotal;
    	_arMDDiPrm[u1VdpId].u2VTotal = u2VTotal;
    	_arMDDiPrm[u1VdpId].u2Width = u2Width;
        _arMDDiPrm[u1VdpId].u2Height = u2Height;
    	_arMDDiPrm[u1VdpId].u4BufAddr = u4BufAddr;
    	_arMDDiPrm[u1VdpId].u2BufWidth = u2BufWidth;
    	_arMDDiPrm[u1VdpId].u2BufHeight = u2BufHeight;
    	_arMDDiPrm[u1VdpId].u1BitRes = u1BitRes;
    	_arMDDiPrm[u1VdpId].u1DataMode = (E_DI_DMODE)u1DataMode;
    	_arMDDiPrm[u1VdpId].u1NRDataMode = (E_NR_DMODE)u1NRDataMode;
    	_arMDDiPrm[u1VdpId].u1PSCANDisplay = u1PSCANDisplay;
    	_arMDDiPrm[u1VdpId].u1Bob = _arMDDiPrm[u1VdpId].u1NewBob;    	
        _arMDDiPrm[u1VdpId].u1420Mode = (u1DecType == SV_VD_MPEGHD) ? MDDI_420_MODE : MDDI_422_MODE;
        
        u1Ret = SV_TRUE;
    }
    else
    {
    	u1Ret = SV_FALSE;
    }
    
    //We don't need to do mode change to reflect offset adjustment
    //We check if offsets are changed here and set registers directly
    if ((_arMDDiPrm[u1VdpId].u2HClip != wSrcXOffset)
    || (_arMDDiPrm[u1VdpId].u2VClip != wSrcYOffset))
    {
        #if PREPROC_ENABLE_SYNC_REGEN
        //vDrvPreprocSetSyncRegen(u1VdpId,wSrcXOffset,wSrcYOffset,u2Width,u2Height);
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, 0x0, X_SHIFT);
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, 0x0, Y_SHIFT);        
        vDrvPreprocSetSyncRegen(u1VdpId, wSrcXOffset,wSrcYOffset,
                                                _arMDDiPrm[u1VdpId].u2Width,
                                                _arMDDiPrm[u1VdpId].u2Height);    		 
        #else
    	//Total number of pixels to be clipped
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, wSrcXOffset, X_SHIFT);
    	//Total number of lines to be clipped
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_10, wSrcYOffset, Y_SHIFT);
    	#endif

    	_arMDDiPrm[u1VdpId].u2HClip = wSrcXOffset;
    	_arMDDiPrm[u1VdpId].u2VClip = wSrcYOffset;
    }

    return u1Ret;
}

/**
* @brief Config De Ratio 
* @param u1VdpId VDP_1/VDP_2
* @param u1SDERule
* @param u1KDERule
*/
void vDrvDISetDeRule(UINT8 u1VdpId, UINT8 u1SDERule, UINT8 u1KDERule)
{
    UINT8 u1Ratio = (bDrvDIIsOversampleOver720(u1VdpId)) ? 3 : 6;
    UINT8 u1RatioNoCs = (bDrvDIIsOversampleOver720(u1VdpId)) ? 2 : 4;
    UINT16 u2AutoSdeN = MDDI_READ_FLD(u1VdpId, MDDI_STA_0D, AUTO_SDE_N);
    UINT16 u2AutoSdeM = MDDI_READ_FLD(u1VdpId, MDDI_STA_0D, AUTO_SDE_M);
    UINT16 u2AutoOdeN = MDDI_READ_FLD(u1VdpId, MDDI_STA_0E, AUTO_ODE_N);
    UINT16 u2AutoOdeM = MDDI_READ_FLD(u1VdpId, MDDI_STA_0E, AUTO_ODE_M);
    
    LOG(5, "[%d] SDE Rule = %d   KDE Rule = %d \n", u1VdpId, u1SDERule, u1KDERule);

    if (u1SDERule == 0x0)          
    {
        vDrvDISetSDE(u1VdpId, _arMDDiPrm[u1VdpId].u1PSCANDisplay ? 0 : 1, 0, 0);
        vDrvDISetODE(u1VdpId, _arMDDiPrm[u1VdpId].u1PSCANDisplay ? 0 : 1, 0, 0);
    }
    else if (u1SDERule == 0x1)
    {
        vDrvDISetSDE(u1VdpId, 0, 1, u1Ratio);
        vDrvDISetODE(u1VdpId, 0, 1, u1Ratio);     
    }
    else if (u1SDERule == 0x2)
    {
        vDrvDISetSDE(u1VdpId, 0, 295, 360);
        vDrvDISetODE(u1VdpId, 0, 295, 360);     
    }
    else if (u1SDERule == 0x3)
    {   
        // Dram has a bug while the M too big
        // Range should be smaller than the other
        if (((u2CalN*u2AutoSdeM) < ((u2CalM-(u1DeRange<<1))*u2AutoSdeN)) ||
           ((u2CalN*u2AutoSdeM) > ((u2CalM+(u1DeRange))*u2AutoSdeN)))
        {        
            u2CalN = ((u4ScalerGetHPS(u1VdpId)*_arMDDiPrm[u1VdpId].u2HTotal+0x4000)/0x8000)>>1;
            u2IIRN = u2CalN;
            u2IIRM = u2CalM;    
            
            LOG(4, "IIR reset   N %d   M %d   AutoSdeN %d   AutoSdeM %d\n",
                u2IIRN, u2IIRM, u2AutoSdeN, u2AutoSdeM);
        }
        else
        {
            u2IIRN = MPY_RATIO_SUM(u1IIR_RATIO, u2AutoSdeN, u2IIRN);
            u2IIRM = MPY_RATIO_SUM(u1IIR_RATIO, u2AutoSdeM, u2IIRM);        
        }        

        vDrvDISetSDE(u1VdpId, 0, u2IIRN, u2IIRM);
        vDrvDISetODE(u1VdpId, 0, u2IIRN, u2IIRM);     
    }
    else if (u1SDERule == 0x4) // dtv progressive, set ode to be 1/2 sde
    {
        vDrvDISetSDE(u1VdpId, 1, 0, 0);
        // vDrvDISetODE(u1VdpId, 0, u2AutoOdeN/2, u2AutoOdeM); 
		if(u2AutoOdeM>3*u2AutoOdeN) //DTV00141744 MTK70646 20091119
		{
			vDrvDISetODE(u1VdpId, 0, 1, 6);
		}
		else
		{
        	vDrvDISetODE(u1VdpId, 0, u2AutoOdeN/2, u2AutoOdeM);
		}    
    }

    if (u1KDERule == 0x0)
    {
        vDrvDISetKDE(u1VdpId, 0, 0);   
    }
    else if (u1KDERule == 0x1)         
    {
        vDrvDISetKDE(u1VdpId, 1, u1RatioNoCs);         
    }
    else if (u1KDERule == 0x2)         
    {
        vDrvDISetKDE(u1VdpId, 1, u1Ratio);         
    }       

    _arMDDiPrm[u1VdpId].u1DeRule = (u1SDERule | (u1KDERule<<4));
}

/**
* @brief Check De Ratio condition 
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetDeRatio(UINT8 u1VdpId)
{
    UINT16 u2AutoSdeN, u2AutoSdeM;
    UINT8 u1Ratio = (bDrvDIIsOversampleOver720(u1VdpId)) ? 3 : 6;
    UINT8 u1KDERule, u1SDERule;
    UINT8 u1CsOnOff;
    
    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    u2AutoSdeN = MDDI_READ_FLD(u1VdpId, MDDI_STA_0D, AUTO_SDE_N);
    u2AutoSdeM = MDDI_READ_FLD(u1VdpId, MDDI_STA_0D, AUTO_SDE_M);

    if (_arMDDiPrm[u1VdpId].u1PSCANDisplay)        
    {
        u1SDERule = 0x0;
    }
    else if ((!_arMDDiPrm[u1VdpId].u1Interlace) 
        && (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD))
    {
        u1SDERule = ((u1VdpId == VDP_1) && (_arMDDiPrm[u1VdpId].u2Height == 720)) ? 0x4 : 0x0;
    }
    else if ((bGetVideoDecType(u1VdpId) == SV_VD_TVD3D) &&
                (bGetSignalType(u1VdpId) == SV_ST_TV))
    {
        u1SDERule = _fgAutoSearch ? 0x1 : 0x3;           
    }
    else if ((_arMDDiPrm[u1VdpId].u1CsEnable)
        && (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W3R) 
        && ((_arMDDiPrm[u1VdpId].u1DecType == SV_VD_TVD3D) 
        || (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_YPBPR)))
    {
        // CS DE ratio constraint
        u1SDERule = ((u2AutoSdeN*u1Ratio) <= u2AutoSdeM) ? 0x0 : 0x1;       
    }
    else if ((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W4R) ||
               (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R) ||
               (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_CBOB_1W4R))
    {
        // for Non-Standard signal
        // input H_Total increase, output H_Total keep the same
        // So auto SDE/ODE too slow 
        if (_arMDDiPrm[u1VdpId].u2Height <= 576) 
        {
            u1SDERule = 0x1;    // 1/6 or 1/3
        }
        else
        {
            u1SDERule = 0x2;    // 148.5/180 = 295/360
        }
    }
    else
    {
        u1SDERule = 0x0;    // auto 0/0
    }
 
    if ((_arMDDiPrm[u1VdpId].u1PSCANDisplay) 
        || (_arMDDiPrm[u1VdpId].u2Height > 576)
        ||  ((!_arMDDiPrm[u1VdpId].u1Interlace) 
        && (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD)))
    {
        u1KDERule = 0x0;    // 0/0
        u1CsOnOff = SV_OFF;
    }
    else 
    {       
        // for Non-Standard signal
        // CVBS 858(864) -> 800 H blanking too short to process data
        if ( (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_TVD3D)
            && (RegReadFldAlign(STA_CDET_00, FH_POS)) )
        {
            u1KDERule = 0x1;    // 1/4 or 1/2
            u1CsOnOff = SV_OFF;      
        }
        else
        {
            u1KDERule = 0x2;    // 1/6 or 1/3
            u1CsOnOff = SV_ON;      
        }
    }

    // for RF & CVBS weak signal
    if ((bGetVideoDecType(u1VdpId) == SV_VD_TVD3D)         
        && (bTvdSignalStrengthLevel(1) < 60))
    {
        u1CsOnOff = SV_OFF;
    }        
    
    if ((_arMDDiPrm[u1VdpId].u1DeRule != (u1SDERule | (u1KDERule<<4)))
        || (u1SDERule == 0x3) || (u1SDERule == 0x4))
    {
        vDrvDISetDeRule(u1VdpId, u1SDERule, u1KDERule);       
    }
    
    if (_arMDDiPrm[u1VdpId].u1CsEnable != u1CsOnOff)
    {
        vDrvCSOnOffEx(u1VdpId, u1CsOnOff);
    }    
}


/**
* @brief Set PSCAN block size
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetBlockSize(UINT8 u1VdpId)
{
    if (_arMDDiPrm[u1VdpId].u1NRDataMode == E_DI_NR_RGB)
    {
        //Set to smaller block size in VGA mode to avoid buffer underflow
        MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, 0x1, RG_BLOCK_SIZE);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, 0x2, RG_BLOCK_SIZE);
    }
}


void vDrvCounterIncrease(UINT8 u1VdpId)
{
    _arMDDiPrm[u1VdpId].u1StatusCnt++;
}


/**
* @brief Set PSCAN FIFO share mode
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetFifoShareMode(UINT8 u1VdpId)
{
    if (_arMDDiPrm[u1VdpId].u1NRDataMode == E_DI_NR_RGB)
    {
        //Use fifo share mode 2 in VGA input
        vRegWriteFldAlign(MDDI_DA_00, 0x2, RG_FIFO_SHARE_MODE);        
    }
    else
    {
        switch(_arMDDiPrm[u1VdpId].u1TVmode)
        {
            case SRM_TV_MODE_TYPE_NORMAL:
                if (IS_5387_ECO() || IS_5363_ECO())
                {
                    vRegWriteFldAlign(MDDI_DA_00, 0x0, RG_FIFO_SHARE_MODE);
                }
                else
                {
                    vRegWriteFldAlign(MDDI_DA_00, 0x2, RG_FIFO_SHARE_MODE);
                }
                break;
            case SRM_TV_MODE_TYPE_PIP:
                vRegWriteFldAlign(MDDI_DA_00, 0x2, RG_FIFO_SHARE_MODE);
                break;
            case SRM_TV_MODE_TYPE_POP:
                vRegWriteFldAlign(MDDI_DA_00, 0x2, RG_FIFO_SHARE_MODE);
                break;
            default:
                vRegWriteFldAlign(MDDI_DA_00, 0x0, RG_FIFO_SHARE_MODE);
                break;
        }
    }
}

/**
* @brief PSCAN main loop
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIChkImpl(UINT8 u1VdpId)
{ 
    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    #ifdef __MODEL_slt__
        vDrvDICRCTest();
    #endif

    if ( (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN) && (u1VdpId == VDP_1))
    	|| (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_SUB) && (u1VdpId == VDP_2)) )
    {
        if (u1DrvDIModeChange(u1VdpId))
        {                
            _vDrvVideoSetMute(MUTE_MODULE_MDDI01, u1VdpId, MDDI_MUTE_PERIOD, TRUE);

            #ifdef DI_WA3_TOGGLE_SCPIP_WEN_OFF            
            vDrvScpipToggleForceOff((UINT32)u1VdpId); // DI reset will cause scpip DRAM overwrite
            #endif

            vDrvDIReset(u1VdpId, SV_ON); 

            vDrvDISetParam(u1VdpId);
            
            vDrvDIReset(u1VdpId, SV_OFF);     
            
            vDrvDISetDramCtrl(u1VdpId, SV_ON);
            
            vDrvCSForceOnOff(u1VdpId, SV_OFF);

            // disable freeze
            _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_NON;
            MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 0, FREEZE_ON);

            // For pscan display mode hlen update
            if (_arMDDiPrm[u1VdpId].u1PSCANDisplay)
            {
                bUpdateHLen[u1VdpId] = TRUE;
            }

            vDrvDIBypass(u1VdpId,
            ((_arMDDiPrm[u1VdpId].u1OnOff) ? SV_FALSE : SV_TRUE));

            #if PREPROC_ENABLE_SYNC_REGEN
            vDrvPreprocSetSyncRegen(u1VdpId,_arMDDiPrm[u1VdpId].u2HClip,
                                            _arMDDiPrm[u1VdpId].u2VClip,
                                            _arMDDiPrm[u1VdpId].u2Width,
                                            _arMDDiPrm[u1VdpId].u2Height);
            #endif
            
            #if defined(CC_MT5387) && !defined(CC_MT5363)
            if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
            {
                vDrvPreprocSetFieldResync(u1VdpId);
            }
            else
            #endif
            {
                // For DTV field inverse bug
                // Need do field inverse when overscan in v direction
                if ((_arMDDiPrm[u1VdpId].u2VClip != 0) &&
                (bGetVideoDecType(u1VdpId) == SV_VD_MPEGHD))
                {
                    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 1, FLD_IN_P);
                }           
                else
                {
                    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 0, FLD_IN_P);
                }
            }
            
            #ifdef DI_WA2_TOGGLE_BYPASS
            vDrvToggleBypass(u1VdpId, SV_TRUE);
            #endif                
        }

        if (_arMDDiPrm[u1VdpId].u1OnOff == SV_ON)
        {
            vDrvDISetDeRatio(u1VdpId);
            vDrvDIChkStatus(u1VdpId);
            vDrvCounterIncrease(u1VdpId);    
    	}

        // For pscan display mode hlen update
    	if (bUpdateHLen[u1VdpId])
    	{
    	    if (u1UpdateCnt[u1VdpId] == MDDI_UPDATE_HLEN_DELAY)
    	    {
    	        u1UpdateCnt[u1VdpId] = 0;
    	        bUpdateHLen[u1VdpId] = FALSE;
                vScpipSetPscanDispmodeHLen(u1VdpId);
    	    }
    	    else
    	    {
                u1UpdateCnt[u1VdpId]++;
    	    }
    	}
    }

    //Quality main loop
    #ifndef __MODEL_slt__
    vDrvDIQtyMainLoop(u1VdpId);
    #endif

    #ifdef DI_WA2_TOGGLE_BYPASS
    vDrvToggleBypass(u1VdpId, SV_FALSE);
    #endif        
}


void vDrvDICsPatch(UINT8 u1VdpId)
{
    if (u1VdpId != VDP_1)
    {
        return;
    }
    
#ifndef __MODEL_slt__
    vDrvCSOnOff(u1VdpId, _arMDDiPrm[u1VdpId].u1CsEnable);    
#endif
}


void vDrvDIFreezeFlow(UINT8 u1VdpId)
{
    static UINT8 bVcount[VDP_NS] = {0, 0};

    if (_arMDDiPrm[u1VdpId].u1Freeze)
    {
        LOG(6, "Path[%d] Freeze state %d - RW %8x  Top/Bot %d  SrcW %d\n", 
            u1VdpId, _arMDDiPrm[u1VdpId].u1Freeze,
            MDDI_READ_FLD(u1VdpId, MDDI_DA_15, STA_DEBUG), 
            MDDI_READ_FLD(u1VdpId, MDDI_STA_00, VIN_FLD),
            MDDI_READ_FLD(u1VdpId, MDDI_CTRL_00, EN_SRCW));           
    }
    
    // Freeze On
    if ((_arMDDiPrm[u1VdpId].u1Freeze == E_FREEZE_ON_STEP1)
        && (MDDI_READ_FLD(u1VdpId, MDDI_STA_00, VIN_FLD) == 1))
    { 
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 0, FREEZE_SEL_PXLR);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 1, FREEZE_SEL_PXLW);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 1, FREEZE_ON);
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_ON_STEP2;                
    }
    else if (_arMDDiPrm[u1VdpId].u1Freeze == E_FREEZE_ON_STEP2)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 0x0, EN_SRCW);          
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_NON;              
    }
    // Freeze Off
    else if ((_arMDDiPrm[u1VdpId].u1Freeze == E_FREEZE_OFF_STEP1)
            && (MDDI_READ_FLD(u1VdpId, MDDI_STA_00, VIN_FLD) == 1))
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 0, FREEZE_SEL_PXLR);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 0, FREEZE_SEL_PXLW);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0C, 0, FREEZE_ON);
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_OFF_STEP2;                
    }        
    else if (_arMDDiPrm[u1VdpId].u1Freeze == E_FREEZE_OFF_STEP2)
    {
        vDrvDIFilmModeFreeze(u1VdpId, SV_OFF);
        MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_00, 0x1, EN_SRCW);
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_OFF_STEP3;        
    }
    else if (_arMDDiPrm[u1VdpId].u1Freeze == E_FREEZE_OFF_STEP3)
    {
        if (bVcount[u1VdpId]++ >= 3)
        {           
            bVcount[u1VdpId] = 0;
            vDrvDIFilmModeFreeze(u1VdpId, SV_ON);
            MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, E_DI_OUT_V_F, INT_TYPE);
            _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_NON;
        }
    }
}


/**
* @brief Enable/disable bob mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff)
{
    _arMDDiPrm[u1VdpId].u1NewBob = u1OnOff;
}


/**
* @brief Return current bob mode status
* @param u1VdpId VDP_1/VDP_2
* @return SV_ON/SV_OFF
*/
UINT8 u1DrvDIGetBob(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1Bob;
}


/**
* @brief Enable/disable PSCAN freeze mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{    
#if defined(CC_MT5387) && !defined(CC_MT5363)
    if (BSP_GetIcVersion() <= IC_VER_5387_AA) // Before ECO
    {
        if (u1VdpId != VDP_1)
        {
            // No isr status for VDP_2, wait for ECO 
            LOG(3, "VDP_2 not support Freeeze\n");
            return;
        }
    }
#endif

    if (u1OnOff)    
    {
    	MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_0B, E_DI_IN_V_F, INT_TYPE);
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_ON_STEP1;
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1Freeze = E_FREEZE_OFF_STEP1;
    }  
}


/**
* @brief Check if freeze function works under the current configuration
* @param u1VdpId VDP_1/VDP_2
* @return SV_TRUE/SV_FALSE
*/
UINT8 u1DrvDIFreezable(UINT8 u1VdpId)
{    
    return ((((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W4R) ||
    (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_CBOB_1W4R) ||
    (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R) ||
    (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_1W4R) ||
    (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_CBOB_1W4R) ||
    (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_YCBOB_1W1R))) ? SV_TRUE : SV_FALSE);
}


/**
* @brief Get PSCAN delay
* @param u1VdpId VDP_1/VDP_2
* @rtVal PSCAN delay
*/
UINT8 vDrvDIGetFrameDelay(UINT8 u1VdpId)
{
    UINT8 u1PSCANDisplay;
    u1PSCANDisplay = _arMDDiPrm[u1VdpId].u1PSCANDisplay;

    switch (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & 0x00070000)
    {
    	case FBM_POOL_MODE_MDDI_FULL:
    	    return (u1PSCANDisplay ? 3 : 2);
    	case FBM_POOL_MODE_MDDI_BOB:
    	    return (u1PSCANDisplay ? 1 : 0);
    	case FBM_POOL_MODE_MDDI_CBOB:
    	    return (u1PSCANDisplay ? 3 : 2);
    	default:
            return 0;
    }
}


/**
* @brief Set NR Data Mode
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDISetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{
    UINT8 bReadEn = ((u1DataMode == E_DI_NR_OFF) ? SV_OFF : SV_ON);
    DI_COLOR_MODE colorModeSel = MDDI_422_MODE;
    DI_YUV_MODE rgYUVMode = DI_YUV_MODE_YC;
    NR_YUV_MODE nrYUVMode = NR_YUV_MODE_422;
    NR_SCAN_MODE scanMode = _arMDDiPrm[u1VdpId].u1Interlace ? NR_SCAN_MODE_INTERLACE : NR_SCAN_MODE_PROGRASSIVE;

    _arMDDiPrm[u1VdpId].u1NRDataMode = u1DataMode;
    //pass NR data mode to NR driver
    vDrvNRSetNRDataMode(u1VdpId, u1DataMode);    
    
    switch (u1DataMode)
    {        
        case E_DI_NR_Y_ONLY:
            //set to 420 when input is DTV and 422 for others
            colorModeSel = (bGetVideoDecType(u1VdpId) == SV_VD_MPEGHD) ? MDDI_420_MODE : MDDI_422_MODE;
            rgYUVMode = _arMDDiPrm[u1VdpId].u1Interlace ? DI_YUV_MODE_YC : DI_YUV_MODE_Y_ONLY;
            nrYUVMode = NR_YUV_MODE_Y_ONLY;
            MDDI_WRITE_FLD(u1VdpId, MDDI_SB_05, 0x0, SB_CHROMA_OFF);
            break;
        case E_DI_NR_YC_420:
            colorModeSel = MDDI_420_MODE;
            rgYUVMode = DI_YUV_MODE_YC;
            nrYUVMode = NR_YUV_MODE_420;
            break;
        case E_DI_NR_YC_422:
            colorModeSel = MDDI_422_MODE;
            rgYUVMode = DI_YUV_MODE_YC;
            nrYUVMode = NR_YUV_MODE_422;
            break;
        case E_DI_NR_RGB:
            colorModeSel = MDDI_422_MODE;
            rgYUVMode = DI_YUV_MODE_YUV;
            nrYUVMode = NR_YUV_MODE_444;
            break;
        case E_DI_NR_OFF:
        default:
            colorModeSel = (bGetVideoDecType(u1VdpId) == SV_VD_MPEGHD) ? MDDI_420_MODE : MDDI_422_MODE;
            rgYUVMode = _arMDDiPrm[u1VdpId].u1Interlace ? DI_YUV_MODE_YC : DI_YUV_MODE_Y_ONLY;
            nrYUVMode = NR_YUV_MODE_Y_ONLY;
            break;
    }

    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, bReadEn, RG_NR_READ_ENABLE); // pscan read enable
    MDDI_WRITE_FLD(u1VdpId, MDDI_DA_00, rgYUVMode, RG_MODE_YUV);
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_01, (colorModeSel ? 0 : 1), M422);

    vDrvNRSetReadEnable(u1VdpId,bReadEn);
    vDrvNRSetYUVMode(u1VdpId,nrYUVMode);
    vDrvNRSetScanMode(u1VdpId,scanMode);

    if ((u1DataMode != E_DI_NR_OFF) || (u1DataMode != E_DI_NR_UNKNOWN))
    {
        vDrvNRSetIIRPath(u1VdpId);
    }
}


/**
* @brief PSCAN callback function
* @param u4Path VDP_1/VDP_2
* @param u4Mode 
* @param u4Address dram base address
* @param u4Width dram width
* @param u4Height dram height
*/
void vDrvSetDIBufferInfo(UINT32 u4Path, UINT32 u4Mode, 
            UINT32 u4Address, UINT32 u4Size, UINT32 u4Width, UINT32 u4Height)
{
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.u4Addr = u4Address;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.u4Size = u4Size;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.u4Width = u4Width;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.u4Height = u4Height;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.fg10bit = (u4Mode & FBM_POOL_MODE_10BIT) > 0;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.fg422 = (u4Mode & FBM_POOL_MODE_422_FB) > 0;
    _arMDDiPrm[(UINT8)u4Path].rFbmPool.u4Mode = u4Mode;

    if (u4Mode & FBM_POOL_MODE_MDDI_DISP)
    {
        _arMDDiPrm[(UINT8)u4Path].u1SrmPSCANDisplay = SV_ON;
    }
    else
    {
        _arMDDiPrm[(UINT8)u4Path].u1SrmPSCANDisplay = SV_OFF;
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT
        if (u4Path == VDP_1)
        {
            if (_arMDDiPrm[u4Path].u1SrmPSCANDisplay)
            {
                vDrvDISetFlipMirror(u4Path, u1FlipMirrorConfig);
            }
            else
            {
                vDrvDISetFlipMirror(u4Path, DI_NORMAL_MODE);
            }
        }
    #endif
}


/**
* @brief PSCAN callback function
* @param u4Arg1 VDP_1/VDP_2
* @param u4Arg2 configuration
* @param u4SramMode sram mode
*/
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;

    vDrvVideoFixedPreScaler((UCHAR)u4Arg1, SV_TRUE, 1);

    if (u4Arg1 == VDP_1)
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);
    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }

    if (prPool != NULL)
    {
        vDrvSetDIBufferInfo(u4Arg1, u4Arg2, prPool->u4Addr, prPool->u4Size, prPool->u4Width, prPool->u4Height);
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}


/**
* @brief Set pscan output clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetPSCANClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType)
{
    DI_KERNEL_CLOCK_TYPE ekClk;
    DI_CLK_DIV_TYPE eDivType;
    // DTV progressive: kernel clock = output clock
    eDivType = ((_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD) && (!_arMDDiPrm[u1VdpId].u1Interlace)) ? DI_CLK_DIV_BY_1 : DI_CLK_DIV_BY_2;

    switch (eType)
    {
        case DI_OCLK_TYPE_VDOIN:
            #ifdef CC_MT5363
            ekClk = (eDivType == DI_CLK_DIV_BY_1) ? DI_KCLK_TYPE_VDOIN : DI_KCLK_TYPE_VDOIN_DIV_2;
            #else
            ekClk = DI_KCLK_TYPE_VDOIN;
            #endif
            break;
        case DI_OCLK_TYPE_OCLK:
            #ifdef CC_MT5363
            ekClk = DI_KCLK_TYPE_OCLK_DIV_2;
            #else // MT5387
            ekClk = DI_KCLK_TYPE_OCLK;
            #endif
            break;
        case DI_OCLK_TYPE_DIV_CK:
            #ifdef CC_MT5363
            ekClk = (eDivType == DI_CLK_DIV_BY_1) ? DI_KCLK_TYPE_DIV_CK : DI_KCLK_TYPE_DIV_CK_DIV_2;
            #else
            ekClk = DI_KCLK_TYPE_DIV_CK;
            #endif
            break;
        case DI_OCLK_OFF:
            ekClk = DI_KCLK_OFF;
            break;
        default:
            return;
    }

    vDrvDISetOutputClock(u1VdpId, eType);
    vDrvDISetKernelClock(u1VdpId, ekClk);
    
    #if defined(CC_MT5387) && !defined(CC_MT5363)
    vIO32WriteFldAlign(CKGEN_VOPROCCFG, eDivType, ((u1VdpId == VDP_1) ? FLD_PS_DISP_CLK_DIV_SEL : FLD_PIP_PS_DISP_CLK_DIV_SEL));        
    #endif
}

/**
* @brief Set pscan output time gen
* @param bPath VDP_1/VDP_2
* @param eType time gen
*/
void vDrvDISetOutputTimeGen(UINT8 u1VdpId, DI_OUTPUT_TIME_GEN eType)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_CTRL_13, (eType == DI_TG_FREE_RUN) ? 0 : 1, OUT_TG_MODE);
}


/**
* @brief Set NR Data mode
* @param u1VdpId VDP_1/VDP_2
* @param u1DataMode datamode
*/
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{
    if((u1VdpId != VDP_1) && (u1VdpId != VDP_2))
    {
        _arMDDiPrm[VDP_1].u1DebugNRDataMode = E_DI_NR_UNKNOWN;        
        _arMDDiPrm[VDP_2].u1DebugNRDataMode = E_DI_NR_UNKNOWN;                
    }
    else
    {    
        _arMDDiPrm[u1VdpId].u1DebugNRDataMode = u1DataMode;
    }
}


/**
* @brief Set PSCAN parameters
*/
void vDrvDISetParam(UINT8 u1VdpId)
{
    UINT8 u1Dtv, u1Progressive;
    UINT16 u2Clip;

    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    u1Dtv = (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD) ? 1 : 0;
    u1Progressive = (_arMDDiPrm[u1VdpId].u1Interlace == SV_FALSE) ? 1 : 0;
    u2Clip = (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_MPEGHD) 
                        ? (_arMDDiPrm[u1VdpId].u2VClip + _arMDDiPrm[u1VdpId].u2DTVClip)
                        : (_arMDDiPrm[u1VdpId].u2VClip);
                        
    vDrvDISetAutoTrickOnOff(u1VdpId,SV_OFF);
	
    vDrvDIFrameRepeat(u1VdpId, SV_OFF);

    vDrvDISetInterpMode(u1VdpId, _arMDDiPrm[u1VdpId].u1DataMode);

    vDrvDISetBlockSize(u1VdpId);

    vDrvDISetFifoShareMode(u1VdpId);

    vDrvDISrmSetBufImpl(u1VdpId, &_arMDDiPrm[u1VdpId].rFbmPool);

    vDrvDISRCWOutIOnOff(u1VdpId, _arMDDiPrm[u1VdpId].u1Interlace);

    //Set color mode (DTV 420; otherwise 422)    
    vDrvDISetColorMode(u1VdpId, _arMDDiPrm[u1VdpId].u1420Mode);

    //Enable progressive 420 input mode (DTV only)
    vDrvDIProgModeOnOff(u1VdpId, (u1Dtv && u1Progressive) ? SV_ON : SV_OFF);

    vDrvDISetHParam(u1VdpId, _arMDDiPrm[u1VdpId].u2HTotal,
    	_arMDDiPrm[u1VdpId].u2Width, _arMDDiPrm[u1VdpId].u2HClip);

    vDrvDISetVParam(u1VdpId, _arMDDiPrm[u1VdpId].u2VTotal,
        _arMDDiPrm[u1VdpId].u2Height, u2Clip);   

    vDrvDISetFldParam(u1VdpId);
    
    vDrvDIResetSw(u1VdpId);

    vDrvDIEnableISR(u1VdpId, SV_ON);

    //Set video quality parameters
#ifndef __MODEL_slt__
    vDrvDISetQuality(u1VdpId);
#endif

    vDrvDISetNRDataMode(u1VdpId, _arMDDiPrm[u1VdpId].u1NRDataMode);
}

void vDrvDIModeChangeDone(UINT8 u1VdpId)
{
    vDrvDISetFieldInverse(u1VdpId, SV_FALSE);
}

UINT16 wDrvDIGetWidth(void)
{
    return MDDI_READ_FLD(SV_VP_MAIN, MDDI_CTRL_02, HDEW);
}
