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
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "pe_if.h"
#include "drv_nr.h"
#include "video_def.h"
#include "drv_video.h"
#include "api_backlt.h"
#include "drv_contrast.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_tvd.h"
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "x_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "source_select.h"
#include "drv_pq_cust.h"
#include "drv_dvi.h"
#include "srm_drvif.h"
#include "drv_meter.h"
#include "drv_od.h"
#include "hw_ospe.h"
// MTK Customized Driver
//david #include "mtk_video_drv.c"

/*****************************************************************************************/
/******************************  Local Dimming SPI customization   ******************************/
/*****************************************************************************************/
#include "hw_ycproc.h"
#include "nptv_debug.h"
#include "panel.h"
#include "drv_lcdim.h"
#include "drv_default.h"

extern BOOL _fgLdmISR;

static UINT8 gbSmartPicIndex = 0;
static UINT8 gbSceUi = 0;
static UINT8 gbOdUi = 0;
static UINT8 gbGammaUi = 0;
UINT8 Clarity_value=0;

extern MMAppInfo sMMAppInfo;

UINT16 vApiFlashPqGetSceId()
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    switch (gbSceUi)
    {
        case 0:
        default:
            return 0;
    }
}

UINT16 vApiFlashPqGetMinSceId()
{
    //This is sample code : SCE ID can be decided 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    return (1);
}

void vApiFlashPqSetSceId(UINT8 bIndex)
{
    gbSceUi = bIndex;
}

UINT16 vApiFlashPqGetOdId()
{
    return gbOdUi;
}

void vApiFlashPqSetOdId(UINT8 bIndex)
{
    gbOdUi = bIndex;
}

UINT16 vApiFlashPqGetGammaId()
{
    //This is sample code : GAMMA ID can be deside 
    //by panel id , ui setting, picture mode .....
    //UINT8 bPanelIdx = GetCurrentPanelIndex();

    switch (gbGammaUi)
    {
        case 1:
            return 0;
        case 2:
            return 1;
        case 3:
            return 2;
        default:
    		return 0;
	}

#if 0 // This is sample code for different gamma table under 3D PR Panel
    switch (gbGammaUi)
    {
        case 1:
            // 3D Using Shutter Glass : IS_LVDS_DISP_3D_SHUTTER
            // 3D Using Polarized Panel : IS_LVDS_DISP_3D_POLARIZED
            // 
            // u4DrvTDTV3DModeQuery() : Query 3D Mode
            
            if((u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF) && (IS_LVDS_DISP_3D_POLARIZED))
            {               
                return 0;       // gamma table for PR panel
            }
            else
			{
                return 1;       // gamma table for normal state
            }
        default:
    		return 0;
	}
#endif    
}

void vApiFlashPqSetGammaId(UINT8 bIndex)
{
    gbGammaUi = bIndex;
}

UINT16 vApiFlashPqGetQtyId()
{
    if (PANEL_GetPanelWidth() <= 1440)
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

void vApiFlashPqSetSmartPicId(UINT8 bIndex)
{
    gbSmartPicIndex = bIndex;
}

UINT8 vApiFlashPqGetSmartPicId()
{
    return (gbSmartPicIndex);
}

void DRVCUST_HwInit(void)
{
    return;
}

void DRVCUST_VideoInit(void)
{
    DRVCUST_AdaptiveBacklightInit();
    return;
}

void DRVCUST_VdoModeChgDone(UINT8 bPath)
{
    UNUSED(bPath);
    return;
}

void DRVCUST_VideoMainloop()
{
    DRVCUST_AdaptiveBacklightProc();
}

void DRVCUST_OutVSyncISR()
{
    return;
}

void DRVCUST_VdoInISR()
{
    return;
}

UINT32 DRVCUST_RGBOfstRemap(UINT32 u4Value)
{
    return u4Value<<2;
}

UINT16 DRVCUST_Y2RMap(void)
{
    //HSI SPEC change , Always -16 For video mode / Special Mode , 
    //because of DEMO Mode Support
    return  0;
}

/* @ Auto NR default Initial setting
 *      1: low
 *      2: Middle
 */
UINT8 DRVCUST_AutoNRDefSet(void)
{
    return 2;
}

/* @ Auto NR noise level implement
 * @ return noise level
 */
UINT8 DRVCUST_NoiseLevel(void)
{
#if 0
    UINT8 bNoiseLevel = NM_INVALIDLEVEL;
    UINT8 bRFNMLevel;

    if((bGetVideoDecType(bDrvNRGetCurrentPath())==SV_VD_TVD3D)&&
        	 (bGetSignalType(bDrvNRGetCurrentPath())==SV_ST_TV))// TVD RF source
    {

        bRFNMLevel = bTvdSignalStrengthLevel(1);   //RF level 0 ~ 80

        if(bRFNMLevel > 75)
        {
            bNoiseLevel = NM_LEVEL1_3;
        }
        else if(bRFNMLevel > 69)
        {
            bNoiseLevel = NM_LEVEL2;
        }
        else if(bRFNMLevel > 59)
        {
            bNoiseLevel = NM_LEVEL3;
        }
        else if(bRFNMLevel > 55)
        {
            bNoiseLevel = NM_LEVEL4;
        }
        else
        {
            bNoiseLevel = NM_LEVEL5;
        }
    }
    else
    {
        bNoiseLevel = vDrvNMLevel();   //content NR, Noise level 0 ~ 4
    }
    
    return bNoiseLevel;
#endif
    return 0;
}

void DRVCUST_PANEL_GAMMA_REMAP(UINT32 u4GammaSel)
{
    UNUSED(u4GammaSel);
}

void DRVCUST_SetPedestalCtrl(UINT8 bLevel)
{    
#if 0 // Below is SAMPLE CODE for function customization.
    if(fgIsMainVga()||fgIsMainDVI())
    {
        SET_MATRIX_PED(SV_OFF);
        
        switch (bLevel)
        {
            case SV_OFF:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_FULL);
                break;
            case SV_ON:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_LIMIT);
                break;
            default:
                vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
                break;                
        }
    }
    else if (fgIsMainTvd3d()||fgIsMainYPbPr())
    {
        vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);

        switch (bLevel)
        {
            case SV_ON:
                SET_MATRIX_PED(SV_ON);
                break;
            case SV_OFF:
            default:
                SET_MATRIX_PED(SV_OFF);
                break;   
        }
    }
    else
    {
        SET_MATRIX_PED(SV_OFF);
        vSetHDMIRangeMode(SV_HDMI_RANGE_FORCE_AUTO);
    }
#endif
}

UINT8 DRVCUST_ColorTransform(UINT8 bPath, UINT8 bColorSys, UINT8 bHdtv)
{
    UINT8 bMonColorTransform;
    UINT8 bIsVideoTiming;

    bIsVideoTiming = (bGetSignalType(bPath) == SV_ST_DVI) ? bDviIsVideoTiming() : SV_TRUE;
    
    switch (bColorSys)
    {
        case SV_COLOR_YCBCR:
        case SV_COLOR_YCBCR_RGB:
        case SV_COLOR_YPBPR_ANALOG:
        case SV_COLOR_YPBPR_DIGITAL:
        case SV_COLOR_HDMI_422_601:
        case SV_COLOR_HDMI_444_601:
        case SV_COLOR_HDMI_422_709:
        case SV_COLOR_HDMI_444_709:
            if (bIsScalerInput444(bPath))
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR601L_2_RGBF : SV_COLORTRANS_YCBCR601F_2_RGBF;
                }
                else    //HDTV
                {
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR709L_2_RGBF : SV_COLORTRANS_YCBCR709F_2_RGBF;
                }
            }
            else
            {
                if (bHdtv == SV_FALSE)
                {   // SDTV
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR601L_2_YCBCR601L : SV_COLORTRANS_YCBCR601F_2_YCBCR601L;
                }
                else    //HDTV
                {
                    bMonColorTransform = bIsVideoTiming ? 
                        SV_COLORTRANS_YCBCR709L_2_YCBCR601L : SV_COLORTRANS_YCBCR709L_2_YCBCR601L;
                }
            }
            break;
        case SV_COLOR_HDMI_RGB:
            if (bIsScalerInput444(bPath))
            {
                bMonColorTransform = bIsVideoTiming ? 
                    SV_COLORTRANS_RGBL_2_RGBF : SV_COLORTRANS_RGBF_2_RGBF;
            }
            else
            {
                bMonColorTransform = bIsVideoTiming ? 
                    SV_COLORTRANS_RGBL_2_YCBCR601L : SV_COLORTRANS_RGBF_2_YCBCR601L;
            }
            break;
        case SV_COLOR_RGB:
        default:
            bMonColorTransform = bIsVideoTiming ? 
                SV_COLORTRANS_RGBL_2_RGBF : SV_COLORTRANS_RGBF_2_RGBF;
            break;
    }
    return bMonColorTransform;
}

UINT8 DRVCUST_MMAPPGetTiming(UINT8 bOutputTiming)
{
    if (sMMAppInfo.u1AppType == 0xFF) //Pretend for Netflix App
    {
        // Temp threshold
        bOutputTiming = (sMMAppInfo.u4BitRate < 100) ?  
            SOURCE_TYPE_TIMING_MM_720P_SD : SOURCE_TYPE_TIMING_MM_720P_HD;
    }

    LOG(5, "DRVCUST_MMAPPGetTiming %d %d\n", sMMAppInfo.u4BitRate, bOutputTiming);

    return (bOutputTiming);
}

void DRVCUST_SET_GAMMA(void)
{
    /* ============================= Sample code ===================================
    
    1. Read gamma data from EEPROM, maybe need some check flow
        => Example 8bit 6 control points with 12bit gamma data
        INT32 iCtlPos[6] =   {0x000, 0x020, 0x040, 0x080, 0x0C0, 0x0FF};
        INT32 iCtlValue_R[6] = {0x013, 0x233, 0x3E5, 0x7C4, 0xB20, 0xEF4};

    2. Interplaotion to 257 red gamma table
        static INT32 OutputLut[257];
        vDrvGammaInterpolation(6, iCtlPos, iCtlValue_R, OutputLut);        

    3. Copy 257 data to u2GammaDecodedTable[] 
        
    ============================= Sample code End ===================================*/        
}

void DRVCUST_AutoColorTempUpdate(AUTO_COLORTEMP_SETS_INFO_T *aCTInfo)
{

}

void DRVCUST_ClarityUpdate(INT8 u1Value)
{
    UNUSED(u1Value);
    return;
}

void DRVCUST_MMAPProc(void)
{
    if (sMMAppInfo.u1AppType == 0xFF) /*Pretend for Netflix App */
    {
        //Update new QtyTbl
        bApiVideoProc(SV_VP_MAIN, PE_ARG_3D_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_MPEG_NR);
        bApiVideoProc(SV_VP_MAIN, PE_ARG_SHARPNESS);
    }
}


//david: need porting
void DRVCUST_AdaptiveBacklightMode(UINT8 bMode)
{
     UNUSED(bMode);
     return;
}
void DRVCUST_AutoBacklightDisable(void)
{    
    return;
}
void DRVCUST_EPG_FLAG(UINT8 bMode)
{
   UNUSED(bMode);
   return;
}	
void DRVCUST_FMRADIO_FLAG(UINT8 bMode)
{
   UNUSED(bMode);
   return;
}	
void DRVCUST_USBOSD_MODE(UINT8 bMode)
{
   UNUSED(bMode);
   return;
}
void DRVCUST_USBPHOTO_MODE(UINT8 bMode)
{
   UNUSED(bMode);
   return;
}
