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
 * $RCSfile: hal_ycproc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _HAL_YCPROC_C_

#include "x_util.h"
#include "x_assert.h"

#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "drv_ycproc.h"
#include "drv_nr.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "drv_di.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "drv_upscaler.h"

#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "nptv_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"

#include "pe_if.h"
#include "drv_pq_cust.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "c_model.h"
#include "drv_hdmi.h"
#include "hal_ycproc.h"
#include "mute_if.h"

extern UINT8 bBackLightCtrl;
extern BOOL _fgANREn;
//extern UINT32 u4Filter16TapCoeff[][6];
//extern UINT32 u4Filter8TapCoeff[][3];
BOOL _fgGammaStressTest = SV_FALSE;

UINT8 u1AdaptiveBackLightPWM[ADP_BACKLIGHT_END_IDX];
UINT8 bPEAtModeChgDone = SV_ON;
UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];

#define wWritePQMatrix(bPath, bItem, bData) (u2PQItemMatrix[(bPath)][(bItem)] = (bData))


/*****************************************************************************************/
/*********************************  HSHARP FRONT *****************************************/
/*****************************************************************************************/
/**
 * @brief Sharpness Control
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoSharpnessOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    vDrvPreSharpOnOff(bPath, bOnOff);

    if (bPath == SV_VP_MAIN)
    {
        vDrvPostSharpOnOff(bOnOff);
    }
}
#ifdef CC_MT5363
void vHalVideoLCEOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bPath == SV_VP_PIP)
    {
        return;
    }
    if(bIsScalerInput444(bPath) || fgApiVideoIsVgaMode(bPath,0))
    {
        vDrvLCEOnOff(SV_OFF);
    }
    else
    {
        vDrvLCEOnOff(bOnOff);
    }
}
#endif

/**
 * @brief Sharpness Control
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue sharpness mode
 */
void vHalVideoSharpnessParam(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
		vDrvPreSharpOnOff(bPath, SV_OFF);
		if (bPath == SV_VP_MAIN)
		{
			vDrvPostSharpOnOff(SV_OFF);
		}
        return;
    }

    vDrvPreSharpParam(bPath);
	if (bPath == SV_VP_MAIN)
	{
	    vDrvPostSharpParam();
	}
}

/*****************************************************************************************/
/********************************  ADAPTIVE LUMA    **************************************/
/*****************************************************************************************/
/**
 * @brief enable/disable adaptive luma
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoAdaptiveLumaOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInput444(bPath))
    {
        vDrvAutoContrastOff(bPath);
    }
    else
    {
        if (bOnOff)
        {
            vDrvAutoContrastOn(bPath);
        }
        else
        {
            vDrvAutoContrastOff(bPath);        
        }
    }
}

/**
 * @brief enable/disable adaptive luma
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoBlackStetchOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if(bIsScalerInput444(bPath))
    {
        vDrvAutoContrastOff(bPath);
    }
    else
    {
        if (bOnOff)
        {
            vDrvAutoContrastOn(bPath);
        }
        else
        {
            vDrvAutoContrastSetBSOff(bPath);
        }
    }
}


/**
 * @brief load default setting of adaptive luma
 */
void vHalVideoAdaptiveLumaSetDefault(void)
{
    vDrvAutoContrastSetDefault();
}

/**
 * @brief Dynamic Back Light param, call by UI
 * @param bMode DYNAMIC_CONTRAST_RATIO_?PERCENT
 * @warning related drivers are in drv_contrast.c
 */
void vHalVideoAdaptiveBackLightParam(UINT8 bMode)
{
    DRVCUST_AdaptiveBacklightParam(bMode);
}

/**
 * @brief Set the default value for back light.
 * @param u1Level
 * @param u1Value default value
 */
void vHalSetAdpBacklight(UINT8 u1Level, UINT8 u1Value)
{
    u1AdaptiveBackLightPWM[u1Level] = u1Value;
}

/**
 * @brief get the default value for back light.
 * @param u1Level
 * @param u1Value default value
 */
UINT8 bHalGetAdpBacklight(UINT8 u1Level)
{
    return u1AdaptiveBackLightPWM[u1Level];
}

void vHalVideoSetAdaptiveBacklightMode(UINT8 bMode)
{
    DRVCUST_AdaptiveBacklightMode(bMode);
}

/*****************************************************************************************/
/***********************************   CONTRAST   ****************************************/
/*****************************************************************************************/

/**
 * @brief set contrast value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue contrast value
 */
void vHalVideoContrast(UINT8 bPath, UINT16 u2Value)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj)) ||
        (bPath == SV_VP_PIP) ||
        (bIsScalerInput444(bPath)))
    {
        vIO32WriteFldAlign(GLOBAL_ADJ_02 + u1Offset, u2Value, CONT_DUMMY);
        vDrvVideoColorMatrix(bPath);
    }
    else
    {
        vDrvMainContrast(u2Value);
    } 
}


/*****************************************************************************************/
/**********************************   BRIGHTNESS   ***************************************/
/*****************************************************************************************/

/**
 * @brief set brightness value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue brightness value
 */
void vHalVideoBrightness(UINT8 bPath, UINT8 bValue)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj)) ||
        (bPath == SV_VP_PIP) ||
        (bIsScalerInput444(bPath)))
    {
        vIO32WriteFldAlign(GLOBAL_ADJ_00 + u1Offset, bValue, BRIGHT_DUMMY);
        vDrvVideoColorMatrix(bPath);
    }
    else
    {
        vDrvMainBrightness(bValue);
    }
}

/*****************************************************************************************/
/**********************************   SATURATION   ***************************************/
/*****************************************************************************************/

/**
 * @brief set saturation value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue saturation value
 */
void vHalVideoSaturation(UINT8 bPath, UINT16 u2Value)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    if(fgDrvGetAutoNROnOff() && (bPath == SV_VP_MAIN))
    {
        u2Value = (u2Value <= bReducedSaturation) ? 
            0 : (u2Value - bReducedSaturation);
    }

    if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj)) ||
        (bPath == SV_VP_PIP) ||
        (bIsScalerInput444(bPath)))
    {
        vIO32WriteFldAlign(GLOBAL_ADJ_02 + u1Offset, u2Value, SAT_DUMMY);
        vDrvVideoColorMatrix(bPath);
    }
    else
    {
        vDrvMainSaturation(u2Value);
    }
}


/*****************************************************************************************/
/*************************************   HUE   *******************************************/
/*****************************************************************************************/
/**
 * @brief set hue value
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue hue value
 */
void vHalVideoHue(UINT8 bPath, UINT8 bValue)
{
    UINT8 u1Offset = (bPath == SV_VP_MAIN) ? 0 : 0x4;

    if ((DRVCUST_OptGet(eFlagUseMatrixPQAdj)) ||
        (bPath == SV_VP_PIP) ||
        (bIsScalerInput444(bPath)))
    {
        vIO32WriteFldAlign(GLOBAL_ADJ_00 + u1Offset, bValue, HUE_DUMMY);
        vDrvVideoColorMatrix(bPath);
    }
    else
    {
        vDrvMainHue(bValue);
    }
}

/*****************************************************************************************/
/*************************************   SCE   *******************************************/
/*****************************************************************************************/

/**
 * @brief enable/disable SCE
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoSCEOnOff(UINT8 bPath, UINT8 bOnOff)
{
    //SCE cannot be enabled under RGB444 mode
    if (bIsScalerInput444(bPath))
    {
        vDrvSCEAllOnOff(SV_OFF);
    }
    else if (bPath == SV_VP_MAIN)
    {
        vDrvMainSCEOnOff(bOnOff);
        vDrvSCEAllOnOff(SV_ON);
    }
}

/**
 * @brief load SCE table
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bValue SCE table number
 * @dependency See MAIN_SCE_DEFAULT_VALUE[] in color_param.c
 */
void vHalVideoSCELoadTable(UINT8 bPath, UINT8 bMode, UINT8 bSceLumaGain, UINT8 bSceSatGain, UINT8 bSceHueGain)
{
    switch (bMode)
    {
        case SV_V_SCE_DEFAULT:
            vDrvSCELoadTable(bPath);
            break;
        default:
            return;

    }
}

#if SUPPORT_SIX_COLOR_MANAGE
void vHalSCELoadTblPartial(UINT8 bColor, UINT8 bHSYType, INT16 i2Value, UINT8 bPhaseBegin, UINT8 bPhaseEnd)
{
	vDrvSCELoadTblPartial(bColor, bHSYType, i2Value, bPhaseBegin, bPhaseEnd);
}
#endif



/**
 * @brief Set SCE pattern
 * @param bOnOff SV_ON/SV_OFF
 * @param wYdata SCE pattern Y level
 * @return NONE
 * @retval void
 */
extern INT32 const YCBCR2RGBOS[3][15];
extern INT32 COLOR_TRANSFORM_ADJ[15];
void vHalSetSCEPat(UINT8 bOnOff, UINT16 wYdata)
{
    BOOL fgBSOnOff = SV_OFF;
    BOOL fgxvOnOff = SV_OFF;
    INT32 COLOR_TRANSFORM_Y2R[15];

    vDrvSCEPatGen(bOnOff, wYdata);

    if (bOnOff) 
    { 
        fgBSOnOff = fgDrvGetBlueStretchOnOff(); /* store latest setting of blue stretch*/
        if (fgBSOnOff)
        {
            vDrvSetBlueStretchOnOff(SV_OFF); /* bypass blue stretch */
        }

        fgxvOnOff = fgDrvGetxvYCCOnOff(); /* store latest setting of xvYCC*/
        if (fgxvOnOff)
        {
            vDrvxvYCCOnOff(SV_OFF); /* bypass xvYCC */
        }
        
        x_memcpy(COLOR_TRANSFORM_Y2R, YCBCR2RGBOS[2], sizeof(COLOR_TRANSFORM_Y2R));
        vDrvSCEAllOnOff(SV_OFF);
        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_Y2R);
        vDrvConfigQuickMute(SV_VP_MAIN, SV_OFF); //Disable HDMI mute during pattern on
        _vDrvVideoForceUnMute(SV_VP_MAIN, SV_ON);
    }
    else
    { 
        if (fgBSOnOff != fgDrvGetBlueStretchOnOff())
        {
            vDrvSetBlueStretchOnOff(fgBSOnOff); /* restore user blue stretch setting */
        }
        
        if (fgxvOnOff != fgDrvGetxvYCCOnOff())
        {
            vDrvxvYCCOnOff(fgBSOnOff); /* restore user blue stretch setting */
        }

        vHalVideoSCEOnOff(SV_VP_MAIN, SV_ON);
        vLoadOSColorMatrix(SV_VP_MAIN, COLOR_TRANSFORM_ADJ);
        vDrvConfigQuickMute(SV_VP_MAIN, SV_ON); //Enable HDMI mute
        _vDrvVideoForceUnMute(SV_VP_MAIN, SV_OFF);
    }	
}

/*****************************************************************************************/
/********************************** ANGULAR SCE MODEL ************************************/
/*****************************************************************************************/
void vHalMainSCEAngularReset(void)
{
    vDrvMainSCEAngularReset();

    vDrvMainSCEAngularIntp(SV_ANG_SCE_HUE);
    vDrvSCEApplyAngularHueResult(SV_VP_MAIN);

    vDrvMainSCEAngularIntp(SV_ANG_SCE_SAT);
    vDrvSCEApplyAngularSatResult(SV_VP_MAIN);

    vDrvMainSCEAngularIntp(SV_ANG_SCE_BRI);
    vDrvSCEApplyAngularBriResult(SV_VP_MAIN);
}

void vHalMainSCEAngularSetHueVal(UINT8 bColor, UINT16 wHueOffset)
{
    vDrvMainSCEAngularSetHueVal(bColor, wHueOffset);
    vDrvMainSCEAngularIntp(SV_ANG_SCE_HUE);
    vDrvSCEApplyAngularHueResult(SV_VP_MAIN);
}

void vHalMainSCEAngularSetSatVal(UINT8 bColor, UINT8 bSatGain)
{
    vDrvMainSCEAngularSetSatVal(bColor, bSatGain);
    vDrvMainSCEAngularIntp(SV_ANG_SCE_SAT);
    vDrvSCEApplyAngularSatResult(SV_VP_MAIN);
}

void vHalMainSCEAngularSetBriVal(UINT8 bColor, UINT8 bBriOffset)
{
    vDrvMainSCEAngularSetBriVal(bColor, bBriOffset);
    vDrvMainSCEAngularIntp(SV_ANG_SCE_BRI);
    vDrvSCEApplyAngularBriResult(SV_VP_MAIN);
}

/*****************************************************************************************/
/********************************** BLUE STRETCH  ****************************************/
/*****************************************************************************************/

void vHalVideoBlueStretchSetDefault()
{
    BSParam sBSParam;

    sBSParam.bRGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_R);
    sBSParam.bGGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_G);
    sBSParam.bBGain = wReadQualityTable(QUALITY_BLUE_STRETCH_GAIN_B);
    sBSParam.bLumaThr = wReadQualityTable(QUALITY_BLUE_STRETCH_LUMA_THD);    
    sBSParam.bChromaThr = wReadQualityTable(QUALITY_BLUE_STRETCH_SAT_THD);            

    vDrvSetBlueStretchParam(sBSParam);
}

void vHalVideoBlueStretchOnOff(UINT8 bOnOff)
{
    vDrvSetBlueStretchOnOff(bOnOff);
}
/*****************************************************************************************/
/************************************   CTI   *******************************************/
/*****************************************************************************************/

/**
 * @brief Chroma transient improvement
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCTIOnOff(UINT8 bPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(bPath))
    {
        bOnOff = SV_OFF;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFOnOff(bOnOff);
        vDrvCTIROnOff(bOnOff);
    }
}

/**
 * @brief Chroma transient improvement
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCTIParam(UINT8 bPath)
{
    if (bIsScalerInput444(bPath))
    {
        vDrvCTIFOnOff(SV_OFF);
        vDrvCTIROnOff(SV_OFF);
        return;
    }

    if (bPath == SV_VP_MAIN)
    {
        vDrvCTIFParam();
        vDrvCTIRParam();
    }
}

/*****************************************************************************************/
/************************************   CDS   *******************************************/
/*****************************************************************************************/
/**
 * @brief Color dependent sharpness
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCDSOnOff(UINT8 bOnOff)
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        bOnOff = SV_OFF;
    }

    vDrvCDSOnOff(bOnOff);
}

/**
 * @brief Color dependent sharpness
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoCDSParam()
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        vDrvCDSOnOff(SV_OFF);
        return;
    }

    vDrvCDSParam();
}

/*****************************************************************************************/
/************************************   LTI   *******************************************/
/*****************************************************************************************/
/**
 * @brief Color dependent sharpness
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoLTIOnOff(UINT8 u1VidPath, UINT8 bOnOff)
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        bOnOff = SV_OFF;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIOnOff(bOnOff);
    }
}

/**
 * @brief LTI
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 * @param bOnOff SV_ON/SV_OFF
 * @warning VGA/DVI/HDMI should turn off
 */
void vHalVideoLTIParam(UINT8 u1VidPath)
{
    if (bIsScalerInput444(SV_VP_MAIN))
    {
        vDrvLTIOnOff(SV_OFF);
        return;
    }

    if (u1VidPath == SV_VP_MAIN)
    {
	    vDrvLTIParam();
    }
}


/*****************************************************************************************/
/***********************************   RGB GAIN   ****************************************/
/*****************************************************************************************/

/**
 * @brief set RGB gain
 * @param bRGB SV_RGB_R/SV_RGB_G/SV_RGB_B
 * @param bValue gain value
 */
void vHalVideoRGBGain(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    if (bPath == SV_VP_MAIN)
    {
        switch (bRGB)
        {
            case SV_RGB_R:
                vDrvSetRGain1(bPath, wValue);
                break;
            case SV_RGB_G:
                vDrvSetGGain1(bPath, wValue);
                break;
            case SV_RGB_B:
                vDrvSetBGain1(bPath, wValue);
                break;
            default:
                ASSERT(0);
                break;
        }
    }
}

/*****************************************************************************************/
/**********************************   RGB OFFSET   ***************************************/
/*****************************************************************************************/
/**
 * @brief Adjust RGB offset
 * @param bRGB SV_RGB_R/SV_RGB_G/SV_RGB_B
 * @param bValue Gain value to set
 * @warning Not used yet!!!
 */
void vHalVideoRGBOffset(UINT8 bPath, UINT8 bRGB, UINT16 wValue)
{
    EXTERN UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value);
    wValue = DRVCUST_RGBOfstRemap(wValue);
    if (bPath == SV_VP_MAIN)
    {
         switch (bRGB)
         {
             // 10 bit RGB offset
             case SV_RGB_R:
                 vDrvSetROffset11(bPath, (wValue));
                 break;
             case SV_RGB_G:
                 vDrvSetGOffset11(bPath, (wValue));
                 break;
             case SV_RGB_B:
                 vDrvSetBOffset11(bPath, (wValue));
                 break;
             default:
                 ASSERT(0);
                 break;
        }
    }
}

void vHalOutRGBColorOnOff(UINT8 bOnOff)
{
    vDrvOutRGBColorOnOff(bOnOff);
}

void vHalOutRGBColor(UINT8 RValue, UINT8 GValue, UINT8 BValue)
{
    vDrvOutRGBColor(RValue, GValue, BValue);
}


/*****************************************************************************************/
/************************************  Gamma  ********************************************/
/*****************************************************************************************/
#define ENABLE_GAMMA_DELTA_FUNC 0

#if ENABLE_GAMMA_DELTA_FUNC
void vHalSetGammaSaveDelta(DRV_CUSTOM_GAMMA_DELTA_T *pv_set_info)
{
		DRVCUST_SetGammaDelta( SV_RGB_R,
				(INT16)pv_set_info->ui2_delta_r[0]-0x80,
				(INT16)pv_set_info->ui2_delta_r[1]-0x80,
				(INT16)pv_set_info->ui2_delta_r[2]-0x80,
				(INT16)pv_set_info->ui2_delta_r[3]-0x80);
		DRVCUST_SetGammaDelta( SV_RGB_B,
				(INT16)pv_set_info->ui2_delta_b[0]-0x80,
				(INT16)pv_set_info->ui2_delta_b[1]-0x80,
				(INT16)pv_set_info->ui2_delta_b[2]-0x80,
				(INT16)pv_set_info->ui2_delta_b[3]-0x80);
		DRVCUST_SetGammaLevel(
				pv_set_info->ui2_green_level[0],
				pv_set_info->ui2_green_level[1],
				pv_set_info->ui2_green_level[2],
				pv_set_info->ui2_green_level[3]);
}
#endif

#ifdef DRV_SUPPORT_EXTMJC
void vHalVideoExternalGammaInit(void)
{
//	//UINT8 bData, bChannel;
//    UINT32 ui4_start_time;
//    UINT8 *pDark[3],*pMiddle[3],*pBright[3];
//
//    ui4_start_time = x_os_get_sys_tick();
//	//CRIT_STATE_T csState;
//    pDark[0]=GAMMA_1024_DARK[SV_RGB_R];
//    pDark[1]=GAMMA_1024_DARK[SV_RGB_G];
//    pDark[2]=GAMMA_1024_DARK[SV_RGB_B];
//    pMiddle[0]=GAMMA_1024_MIDDLE[SV_RGB_R];
//    pMiddle[1]=GAMMA_1024_MIDDLE[SV_RGB_G];
//    pMiddle[2]=GAMMA_1024_MIDDLE[SV_RGB_B];
//    pBright[0]=GAMMA_1024_BRIGHT[SV_RGB_R];
//    pBright[1]=GAMMA_1024_BRIGHT[SV_RGB_G];
//    pBright[2]=GAMMA_1024_BRIGHT[SV_RGB_B];
//    drv_extmjc_gamma_write(pDark,pMiddle,pBright,1024);
//    drv_extmjc_set_gamma(SV_V_GAMMA_DARK, NULL);

//    Printf( "\r\n vHalVideoExternalGammaInit() cost %u ms\r\n\r\n", (x_os_get_sys_tick()-ui4_start_time)*5 );
}
#endif

/**
 * @brief select gamma mode
 * @param bMode SV_V_GAMMA_OFF/SV_V_GAMMA_BRIGHT/SV_V_GAMMA_MIDDLE/SV_V_GAMMA_DARK
 */
void vHalVideoGammaLoadTable(UINT8 bMode)
{
    vDrvGammaOnOff(SV_OFF);

    switch (bMode)
    {
    case SV_V_GAMMA_BRIGHT:
        vDrvGammaLoadTable(E_GAMMA_BRIGHT);
        break;
    case SV_V_GAMMA_MIDDLE:
        vDrvGammaLoadTable(E_GAMMA_MIDDLE);
        break;
    case SV_V_GAMMA_DARK:
        vDrvGammaLoadTable(E_GAMMA_DARK);
        break;
    case SV_V_GAMMA_OFF:
        break;
    default:
        ASSERT(0);
    }

    if (bMode != SV_V_GAMMA_OFF)
    {
        vDrvGammaOnOff(SV_ON);
    }
}


/**
 * @brief Verify gamma table
 * @param bMode SV_V_GAMMA_OFF/SV_V_GAMMA_BRIGHT/SV_V_GAMMA_MIDDLE/SV_V_GAMMA_DARK
 */

BOOL bHalVideoVerifyGammaTable(UINT8 bMode)
{
    BOOL bVerifyR=0, bVerifyG=0, bVerifyB=0;

    switch (bMode)
    {
    case SV_V_GAMMA_BRIGHT:
        bVerifyR = bDrvGammaReadTable(SV_RGB_R, SV_V_GAMMA_BRIGHT);
        bVerifyG = bDrvGammaReadTable(SV_RGB_G, SV_V_GAMMA_BRIGHT);
        bVerifyB = bDrvGammaReadTable(SV_RGB_B, SV_V_GAMMA_BRIGHT);
        break;
    case SV_V_GAMMA_MIDDLE:
        bVerifyR = bDrvGammaReadTable(SV_RGB_R, SV_V_GAMMA_MIDDLE);
        bVerifyG = bDrvGammaReadTable(SV_RGB_G, SV_V_GAMMA_MIDDLE);
        bVerifyB = bDrvGammaReadTable(SV_RGB_B, SV_V_GAMMA_MIDDLE);
        break;
    case SV_V_GAMMA_DARK:
        bVerifyR = bDrvGammaReadTable(SV_RGB_R, SV_V_GAMMA_DARK);
        bVerifyG = bDrvGammaReadTable(SV_RGB_G, SV_V_GAMMA_DARK);
        bVerifyB = bDrvGammaReadTable(SV_RGB_B, SV_V_GAMMA_DARK);
        break;
    case SV_V_GAMMA_OFF:
        break;
    default:
        assert(0);
    }

    return (bVerifyR && bVerifyG && bVerifyB);
}

void vHalVideoGammaStressTest(void)
{
	static UINT32 u4Mode=1;
	static UINT32 u4Counter=0;

    // Gamma Stress test
    if(_fgGammaStressTest && (u4Counter++%10==0))
    {
        vHalVideoGammaLoadTable(u4Mode);
        if(bHalVideoVerifyGammaTable(u4Mode))
        {
            LOG(0, "PASSED: Verify Gamma Table %d, on %d iteration\n", u4Mode, u4Counter/10);
        }
        else
        {
            LOG(0, "FAILED: Verify Gamma Table %d on %d iteration\n", u4Mode, u4Counter/10);
            _fgGammaStressTest = SV_FALSE;
        }

        if(++u4Mode > 3){
            u4Mode= 1;
        }
    }
}

/*****************************************************************************************/
/**
 * @brief enable/disable white peak limit
 * @param bOnOff SV_ON/SV_OFF
 */
void vHalVideoWhitePeakLimit(UINT8 bOnOff)
{
    vDrvWhitePeakLimit(bOnOff);
}

/*****************************************************************************************/
/************************************  Noise Reduction  **********************************/
/*****************************************************************************************/
void vHalVideoAutoNROnOff(UINT8 bOnOff)
{

}

void vHalVideo2DNRParam(UINT8 bCurUI)
{
    vDrv2DNRParam(bCurUI);
}

void vHalVideoMPEGNRParam(UINT8 bCurUI)
{
    vDrvMPEGNRParam(bCurUI);
}

void vHalVideo3DNRParam(void)
{
    vDrv3DNRParam();
}

/***************************************************************************/
/********************************** Video level extend**********************/
/***************************************************************************/

/**
 * @brief Extend video level to max.
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param fgIsModeChange SV_TRUE/SV_FALSE
 */
void vHalVideoExtLevel(UINT8 u1VidPath, BOOL fgIsModeChange)
{
    UINT16 u2Gain, u2Offset;

    u2Gain = VIDEO_GAIN_DFT;
    u2Offset = VIDEO_OFST_DFT;

    // On mode change done, use one time setting to extend video level.
    if (fgIsModeChange == SV_TRUE)
    {
        // Use output stage 3x3 matrix to extend video level since there is only one set of RGB gain/offset
        u2Gain = VIDEO_GAIN_DFT;
        u2Offset = VIDEO_OFST_DFT;
    }
    else
    {
        // Only apply dynamic RGB gain when adaptive backlight is active.
        if (bBackLightCtrl == SV_OFF)
        {
        	return;
        }
    }

    vDrvSetRGain1(u1VidPath, u2Gain);
    vDrvSetGGain1(u1VidPath, u2Gain);
    vDrvSetBGain1(u1VidPath, u2Gain);
    vDrvSetROffset11(u1VidPath, u2Offset);
    vDrvSetGOffset11(u1VidPath, u2Offset);
    vDrvSetBOffset11(u1VidPath, u2Offset);
}

void vHalHDMIBlackControl(UINT8 bOnOff)
{
    if ((_rMChannel.bIs444 == SV_TRUE) && (fgIsVgaTiming(_bDviTiming)))
    {
        if (bOnOff != SV_TRUE)
        {
            // RGB 0 ~ 255
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x80);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x80);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x80);

            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0);
        }
        else
        {
            // RGB 16 ~ 235
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x5B);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x5B);
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x5B);

            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R, 0x10);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G, 0x10);
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B, 0x10);
        }
    }
    else
    {
        LOG(9, "Not Dvi timing RGB444\n");
    }
}


/***************************************************************************/
/********************************** Misc Hal *****************************/
/**************************************************************************/
/**
 * @brief get chroma histogram
 * @param wHist nomalized Hist value
 */
void vHalGetChromaHist(UINT16 * u2aHist)
{
    UINT8 i;
    UINT32 dwTotal = 0;
    UINT32 dwMulti = 0;

    vDrvGetChromaHist(u2aHist, SV_FALSE, 0xFFFF);

    for (i = 0; i < NUM_CHROMA_HIST_BIN; i++)
    {
        dwTotal = dwTotal + u2aHist[i];
    }

    dwMulti = (dwTotal == 0) ? 0 : (0xFFFFFFFF / dwTotal);

    for (i = 0; i < NUM_CHROMA_HIST_BIN; i++)
    {
        u2aHist[i] = (UINT16)((u2aHist[i] * dwMulti) >> 16);
    }

    return;
}

void vHalGetRGBMax(UINT32 * dwHist)
{
    UINT8 i;
    UINT32 dwTotal = 0;
    UINT32 dwMulti = 0;

    vDrvGetRGBMax(dwHist);

    for (i = 0; i < NUM_RGB_MAX_HIST_BIN; i++)
    {
        dwTotal += dwHist[i];
    }

    dwMulti = (dwTotal == 0) ? 0 : (0xFFFFFFFF / dwTotal);

    for (i = 0; i < NUM_RGB_MAX_HIST_BIN; i++)
    {
        dwHist[i] = (dwHist[i] * dwMulti) >> 16;
    }

    return;
}

void HalGetRGBMaxHistNorm(UINT16 * dwHist)
{
    vDrvGetRGBMaxNormHist(dwHist);
}

/**
 * @brief Get luma histogram max level.
 * @return LPF of max level.
 */
UINT8 bGetHistMaxLvl(void)
{
	INT8	i;
	UINT16	u2LongTimeGain;
    UINT16	u2Hist[LUMA_HIST_NUM];
	static UINT8	u1MaxLvlArray[VIDEO_EXT_FRAME_COUNT];
	static UINT8	u1FrameCount;

	bDrvGetHist32(u2Hist);
	for (i = LUMA_HIST_NUM -1; i >= 0; i--)
	{
		if (u2Hist[i] > VIDEO_EXT_FIND_MAX_HIST_INDEX_TH)
		{
            if (u1FrameCount < VIDEO_EXT_FRAME_COUNT)
            {
			    u1MaxLvlArray[u1FrameCount] = i*8 + 4;	// return value is 0~255.
            }
            else
            {
                Printf("u1MaxLvlArray index range error!\n");
            }
			if (u1FrameCount++ == VIDEO_EXT_FRAME_COUNT)
			{
				u1FrameCount = 0;
			}
			break;
		}
	}

	u2LongTimeGain = 0;
	for (i=0; i<VIDEO_EXT_FRAME_COUNT; i++)
	{
		u2LongTimeGain += u1MaxLvlArray[i];
	}

	return (UINT8)(u2LongTimeGain/VIDEO_EXT_FRAME_COUNT);

}

/**
 * @brief Get the luma histogram big level.
 * @return index of the big level(32 bins).
 */
UINT8 bGetHistBigLvl(void)
{
	INT8	i;
    UINT16	u2Hist[LUMA_HIST_NUM];

	bDrvGetHist32(u2Hist);
	for (i = LUMA_HIST_NUM -1; i >= 0; i--)
	{
		if (u2Hist[i] > ADAPTIVE_BACKLIGHT_FIND_MAX_HIST_INDEX_TH)
		{
			return (i*8 + 4);	// return value is 0~255.
		}
	}
	return 0;
}

/**
 * @control PE at ModeChgDone
 * @param u1OnOff SV_VP_MAIN/SV_VP_PIP
 */
void vHalSetPEAtModeChgDone(UINT8 u1OnOff)
{
	bPEAtModeChgDone = u1OnOff;
}

/**************************************************************************************/
/***********************************Bypass All PQ Item****************************************/
/**************************************************************************************/

void vVideoAllPQItemOnOff(UINT8 bOnOff)
{
	UINT16 wRGBgain;
	UINT8 i;

	wRGBgain=0x200;

	vDrvSCEAllOnOff(bOnOff);
	vDrvCTIROnOff(bOnOff);
	vDrvCTIFOnOff(bOnOff);
	vDrvLTIOnOff(bOnOff);
    vDrvPreSharpOnOff(SV_VP_MAIN, bOnOff);
    vDrvPostSharpOnOff(bOnOff);
    vDrvCDSOnOff(bOnOff);
#if defined(CC_MT5363)      
       vDrvLCEOnOff(bOnOff);
#endif
	vDrvGammaOnOff(bOnOff);

	if (bOnOff)
	{
		vDrvNRBypass(SV_OFF);
		for (i=PE_ARG_R_GAIN; i<=PE_ARG_B_OFFSET;i++)
		{
			bApiVideoProc(SV_VP_MAIN, i);
		}
	}
	else
	{
		vDrvNRBypass(SV_ON);

		vHalVideoRGBGain(SV_VP_MAIN,SV_RGB_R ,wRGBgain);
		vHalVideoRGBGain(SV_VP_MAIN,SV_RGB_G ,wRGBgain);
		vHalVideoRGBGain(SV_VP_MAIN,SV_RGB_B ,wRGBgain);
		vHalVideoRGBOffset(SV_VP_MAIN,SV_RGB_R ,0);
		vHalVideoRGBOffset(SV_VP_MAIN,SV_RGB_G ,0);
		vHalVideoRGBOffset(SV_VP_MAIN,SV_RGB_B ,0);
	}
}


/**************************************************************************************/
/********************************* OPC              ***********************************/
/**************************************************************************************/

void vHalVideoOPCResultUpdate()
{
    // write opc curve
	vRegWriteFldMulti(DBL_CFG1, 
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P1), DBL_P1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_P2), DBL_P2) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G1), DBL_G1) |
	                    P_Fld(IO32ReadFldAlign(OPC_0B, OPC_SW_G2), DBL_G2));    
	vRegWriteFldAlign(DBL_CFG2, 
	                          IO32ReadFldAlign(OPC_0C, OPC_SW_G_SLOPE), DBL_G_SLOPE);


    // write backlight
	if (PANEL_BRIGHT_SETTING == PWM_HIGH_PANEL_BRIGHT)
  	{
       	vApiSetPanelBright(IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));
	}
	else
	{
       	vApiSetPanelBright(255-IO32ReadFldAlign(OPC_0C, OPC_SW_PWM));
	}    
}
