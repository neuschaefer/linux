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

// === INCLUDE =============================================================================
#include "x_assert.h"
#include "sv_const.h"
#include "nptv_debug.h"
#include "pe_if.h"
#include "vdp_if.h"
#include "vdp_drvif.h"
#include "drv_pq_cust.h"	// TODO: Remove
#include "u_drv_cust.h"	
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "drv_ycproc.h"
#include "vdo_misc.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_vdoclk.h"
#include "drv_scaler.h"
#include "source_select.h"
#include "drv_video.h"
// === DEFINE =============================================================================
#define SUPPORT_GAMMA_DELTA 0

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
extern struct _PIC_INFO_T aApiGetPictureInfo(void);

// === GLOBAL VARIABLE ====================================================================

// === EXTERN VARIABLE ====================================================================
extern struct _SMART_PIC_GROUP_INFO_T aFlashPqSmartPicGroup;

// === STATIC VARIABLE ====================================================================

// === BODY ===============================================================================

/*****************************************************************************************/
/********************** Interface between MW_if and NPTV Api *****************************/
/*****************************************************************************************/
#ifndef CC_COPLAT_MT82
/**
 * @brief Get the min/max value from quality table.
 * @param u1UiItem VDP_ARG_xxx
 */
extern INT32 d_custom_vid_get_min_max(DRV_CUSTOM_VID_TYPE_T e_vid_type,
                                      DRV_CUSTOM_VID_INP_T e_vid_inp,
                                      UINT16 * pui2_min, UINT16 * pui2_max)
{
    UINT16 u2QtyItem;
    UINT8 u1SrcTypTmg;

    if ((pui2_min == NULL) || (pui2_max == NULL))
    {
        return -1;
    }

    *pui2_min = 0;
    *pui2_max = 0;

    // Mapping input source timing.
    switch (e_vid_inp)
    {
    default:
        return (-1);
    case DRV_CUSTOM_VID_INP_FLAG_AV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_CVBS_NTSC_358;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_ATV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_RFTV_NTSC_358;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_DTV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DTV_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_YPBPR:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_YPBPR_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_VGA:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_PC_RGB;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_HDMI:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DIGI_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_DVI:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DIGI_SD_I_50;
        break;
    }

    // Don't care detail timing. Eg: take YPbPr1080i as YPbPr.
    u1SrcTypTmg = u1SrcTypTmg & 0xF0;

    switch (e_vid_type)
    {
    case DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT:
        u2QtyItem = QUALITY_BRIGHTNESS;
        break;
    case DRV_CUSTOM_VID_TYPE_CONTRAST_DFT:
        u2QtyItem = QUALITY_CONTRAST;
        break;
    case DRV_CUSTOM_VID_TYPE_HUE_DFT:
        u2QtyItem = QUALITY_HUE;
        break;
    case DRV_CUSTOM_VID_TYPE_SATURATION_DFT:
        u2QtyItem = QUALITY_SATURATION;
        break;

    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL:
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE:
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
#if 0
    case DRV_CUSTOM_VID_TYPE_YPBPR_H_POS:      /* YPbPr horizontal position (by timing) */
        *pui2_min = vHdtv_CustPos_HPosMin();
        *pui2_max = vHdtv_CustPos_HPosMax();
        return 0;
    case DRV_CUSTOM_VID_TYPE_YPBPR_V_POS:      /* YPbPr vertical position (by timing) */
        *pui2_min = vHdtv_CustPos_VPosMin();
        *pui2_max = vHdtv_CustPos_VPosMax();
        return 0;
#endif
    case DRV_CUSTOM_VID_TYPE_ADC_R_GAIN:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_GAIN:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_GAIN:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET:
        *pui2_min = 0;
        *pui2_max = 0xFF;
        return 0;

	case DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE:  //kaloverrule
		*pui2_min = 0;
		*pui2_max = 0xFF;
		return 0;

    case DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM:
        *pui2_min = 0;
        *pui2_max = 1000;
        return 0;
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN_EEPROM:
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN:
        *pui2_min = 0;
        *pui2_max = 0x7F;
        return 0;
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET_EEPROM:
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET:
        *pui2_min = 244;//256-12
        *pui2_max = 268;//256+12
        return 0;
    default:
        ASSERT(0);
        return -1;
    }
    *pui2_min = READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
    *pui2_max = READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
    return 0;
}

/**
 * @brief Get the default value from quality table.
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP, Prepare input signal before using this function.
 * @param u1UiItem VDP_ARG_xxx
 */
INT32 d_custom_vid_get(DRV_CUSTOM_VID_TYPE_T e_vid_type,
                       DRV_CUSTOM_VID_INP_T e_vid_inp,
                       VOID * pv_get_info, SIZE_T * pz_size)
{
    UINT16 u2QtyItem;
    UINT8 u1SrcTypTmg;

    if (pv_get_info == NULL)
    {
        return (-1);
    }

    // Mapping input source timing.
    switch (e_vid_inp)
    {
    default:
        return (-1);
    case DRV_CUSTOM_VID_INP_FLAG_AV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_CVBS_NTSC_358;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_ATV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_RFTV_NTSC_358;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_DTV:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DTV_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_YPBPR:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_YPBPR_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_VGA:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_PC_RGB;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_HDMI:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DIGI_SD_I_50;
        break;
    case DRV_CUSTOM_VID_INP_FLAG_DVI:
        u1SrcTypTmg = SOURCE_TYPE_TIMING_DIGI_SD_I_50;
        break;
    }

    // Don't care detail timing. Eg: take YPbPr1080i as YPbPr.
    u1SrcTypTmg = u1SrcTypTmg & 0xF0;

    switch (e_vid_type)
    {
    case DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT:
        u2QtyItem = QUALITY_BRIGHTNESS;
        break;
    case DRV_CUSTOM_VID_TYPE_CONTRAST_DFT:
        u2QtyItem = QUALITY_CONTRAST;
        break;
    case DRV_CUSTOM_VID_TYPE_HUE_DFT:
        u2QtyItem = QUALITY_HUE;
        break;
    case DRV_CUSTOM_VID_TYPE_SATURATION_DFT:
        u2QtyItem = QUALITY_SATURATION;
        break;
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
        u2QtyItem = QUALITY_BACKLIGHT;
        break;

    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL:
        *((UINT8 *) pv_get_info) = bHalGetAdpBacklight(ADP_BACKLIGHT_LOW_IDX);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE:
        *((UINT8 *) pv_get_info) = bHalGetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH:
        *((UINT8 *) pv_get_info) = bHalGetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX);
        return 0;
#if 0
    case DRV_CUSTOM_VID_TYPE_YPBPR_H_POS:      /* YPbPr horizontal position (by timing) */
        *((UINT16 *) pv_get_info) = vHdtv_CustPos_GetHPos();
        return 0;
    case DRV_CUSTOM_VID_TYPE_YPBPR_V_POS:      /* YPbPr vertical position (by timing) */
        *((UINT16 *) pv_get_info) = vHdtv_CustPos_GetVPos();
        return 0;
#endif
    case DRV_CUSTOM_VID_TYPE_ADC_R_GAIN:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_R_Gain_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_GAIN:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_G_Gain_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_GAIN:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_B_Gain_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_R_Offset_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_G_Offset_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET:
        *((UINT16 *) pv_get_info) = (UINT16) vCust_B_Offset_Read();
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE: //kaloverrule
  	    *((UINT16 *) pv_get_info) = (UINT16) vCust_EBGain_OverRule_Read();
		return 0;

       case DRV_CUSTOM_VID_TYPE_ADC_R_MAX:
		*((UINT16*)pv_get_info) = (UINT16)vCust_R_MAX_Read();
		return 0;
       case DRV_CUSTOM_VID_TYPE_ADC_G_MAX:
		*((UINT16*)pv_get_info) = (UINT16)vCust_G_MAX_Read();
		return 0;
       case DRV_CUSTOM_VID_TYPE_ADC_B_MAX:
		*((UINT16*)pv_get_info) = (UINT16)vCust_B_MAX_Read();
		return 0;

	   case DRV_CUSTOM_VID_TYPE_ADC_B_BLANK_LEVEL:
		*((UINT16*)pv_get_info) = (UINT16)vCust_B_Blank_Read();
			return 0;
	   case DRV_CUSTOM_VID_TYPE_ADC_G_BLANK_LEVEL:
	    *((UINT16*)pv_get_info) = (UINT16)vCust_G_Blank_Read();
		    return 0;
   	   case DRV_CUSTOM_VID_TYPE_ADC_R_BLANK_LEVEL:
		*((UINT16*)pv_get_info) = (UINT16)vCust_R_Blank_Read();
		    return 0;
  	   case DRV_CUSTOM_VID_TYPE_ADC_B_MAX_LEVEL:
		*((UINT16*)pv_get_info) = (UINT16)vCust_B_Max_Read();
		    return 0;
	   case DRV_CUSTOM_VID_TYPE_ADC_G_MAX_LEVEL:
		*((UINT16*)pv_get_info) = (UINT16)vCust_G_Max_Read();
		    return 0;
	   case DRV_CUSTOM_VID_TYPE_ADC_R_MAX_LEVEL:
	    *((UINT16*)pv_get_info) = (UINT16)vCust_R_Max_Read();
	    	return 0;
     
    case DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM:
        *((UINT8 *) pv_get_info) = (UINT16) DDDS_GetSpreadSpectrum();
        return 0;
#ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_DISP_TYPE_PICTURE_INFO:
        *((struct _PIC_INFO_T *) pv_get_info) = aApiGetPictureInfo(); 
        return 0;
#endif //#ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_DISP_TYPE_PQ_VER:
        if (bApiFlashPqVerifyHeader() == SV_TRUE)
        {
            x_strcpy((CHAR *)pv_get_info, (CHAR *)bApiFlashPqGetVersion(FLASHPQ_XML_VER));
            return 0;
        }
        else
        {
            return (-1);
        }
    case DRV_CUSTOM_DISP_TYPE_GAMMA_VER:
        if (bApiFlashPqVerifyHeader() == SV_TRUE)
        {
            x_strcpy((CHAR *)pv_get_info, (CHAR *)bApiFlashPqGetVersion(FLASHPQ_GAMMA_VER));
            return 0;
        }
        else
        {
            return (-1);
        }
    case DRV_CUSTOM_DISP_TYPE_SCE_VER:
        if (bApiFlashPqVerifyHeader() == SV_TRUE)
        {
            x_strcpy((CHAR *)pv_get_info, (CHAR *)bApiFlashPqGetVersion(FLASHPQ_SCE_VER));
            return 0;
        }
        else
        {
            return (-1);
        }
    case DRV_CUSTOM_DISP_TYPE_QTY_VER:
        if (bApiFlashPqVerifyHeader() == SV_TRUE)
        {
            x_strcpy((CHAR *)pv_get_info, (CHAR *)bApiFlashPqGetVersion(FLASHPQ_QTY_VER));
            return 0;
        }
        else
        {
            return (-1);
        }
#ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_DISP_TYPE_SMART_PICTURE:
        *((struct _SMART_PIC_GROUP_INFO_T *) pv_get_info) =
            aFlashPqSmartPicGroup;
        return 0;
#endif //#ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN:
        *((UINT8*)pv_get_info) =  bDrvGetTVDADCGain();
        return 0;

    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN_EEPROM:
        if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
        {
            *((UINT8*)pv_get_info) = (UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_GAIN0);
        }
        else
        {
            if((DRVCUST_OptGet(eTVDSupportEFuseCVBS))&&(bApiEepromReadByte(EEP_VIDEO_BYPASS_AV_EFUSE)!=0xaa))
            {
                *((UINT8*)pv_get_info) = (UINT16)((IO32ReadFldAlign(0xF000868c, AC_FULLDW)>>16)&0xff);
            }
            else
            {
                *((UINT8*)pv_get_info) = (UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0);
            }
        }
        return 0;

    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET:
        *((UINT16*)pv_get_info) = bDrvGetTVDADCOffset();
        return 0;

    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET_EEPROM:
        if((bGetSignalType(SV_VP_MAIN) == (UINT8)SV_ST_TV) || (bGetSignalType(SV_VP_PIP) == (UINT8)SV_ST_TV))
        {
            *((UINT16*)pv_get_info) = (UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_TV_OFFSET)+200;//only store the value - 200
        }
        else
        {
            *((UINT16*)pv_get_info) = (UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_OFFSET)+200;//only store the value - 200
        }
        return 0;

    case DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX:
        *((UINT16*)pv_get_info) =  wDrvGetTVDADCMAX();
        return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_R_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_G_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_B_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_R_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_R_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_G_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_G_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_VGA_B_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_B_Offset_Read();
		return 0;

	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_Y_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_PB_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_PR_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_Y_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_Y_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PB_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_PB_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_CMP_PR_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_PR_Offset_Read();
		return 0;

	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_R_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_G_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_GAIN:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_B_Gain_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_R_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_R_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_G_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_G_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_ADC_EEPROM_SCART_B_OFFSET:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_B_Offset_Read();
		return 0;
	case DRV_CUSTOM_VID_TYPE_CMP_AUTOCOLOR_STATUS:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_CMP_STATUS();
		return 0;
	case DRV_CUSTOM_VID_TYPE_VGA_AUTOCOLOR_STATUS:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_VGA_STATUS();
		return 0;
	case DRV_CUSTOM_VID_TYPE_SCART_AUTOCOLOR_STATUS:
		*((UINT16*)pv_get_info) = (UINT16)vCust_EEPROM_SCART_STATUS();
		return 0;

	case DRV_CUSTOM_VID_TYPE_DEFAULT_GAMMA_VALUE:
#if SUPPORT_GAMMA_DELTA
		((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->ui2_gamma_value
			= DRVCUST_GetGammaVal( ((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->e_gamma_color_type,
								((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->ui2_gamma_level );
#endif//SUPPORT_GAMMA_DELTA
		return 0;
	case DRV_CUSTOM_VID_TYPE_COMP_GAMMA_VALUE:
#if SUPPORT_GAMMA_DELTA
		((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->ui2_gamma_value
			= DRVCUST_GetCompGammaVal( ((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->e_gamma_color_type,
								((DRV_CUSTOM_GAMMA_VALUE_T *)pv_get_info)->ui2_gamma_level );
#endif//SUPPORT_GAMMA_DELTA
		return 0;

    default:
        ASSERT(0);
        return -1;
    }


    switch (e_vid_type)
    {
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
        *((UINT8 *) pv_get_info) = READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
        break;
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
        *((UINT8 *) pv_get_info) = READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
        break;
    default:
        *((UINT8 *) pv_get_info) = READ_DEFUALT_QTY_TBL_DFT(u2QtyItem);
        break;
    }
    return 0;

}

INT32 d_custom_vid_get_gamma(GAMMA_INFO_T * pu1GammaTbl)
{
    int i;
    UINT8 *ui1Tbl;

    if (pu1GammaTbl == NULL)
    {
        return (-1);
    }

    switch ((UINT8) (pu1GammaTbl->e_color_temp))
    {
    case DRV_CUSTOM_DISP_CT1:
        ui1Tbl = GAMMA_256[pu1GammaTbl->ui1Channel];
        break;
    case DRV_CUSTOM_DISP_CT2:
        ui1Tbl = GAMMA_256[pu1GammaTbl->ui1Channel];
        break;
    case DRV_CUSTOM_DISP_CT3:
        ui1Tbl = GAMMA_256[pu1GammaTbl->ui1Channel];
        break;
    default:
        return SV_FALSE;
    }

    for (i = 0; i < 256; i++)
    {
        pu1GammaTbl->ui1Tbl[i] = ui1Tbl[i];
    }
    return 0;
}

extern INT32 d_custom_vid_set(DRV_CUSTOM_VID_TYPE_T e_vid_type,
                              VOID * pv_set_info, SIZE_T z_size, BOOL b_store)
{
    UINT8 *pu1GammaTbl;
    UINT16 i;
    VID_NON_LINEAR_INFO_T *prNonlinear;
    Region rDemoRegion;
#if SUPPORT_DOT_TO_DOT_PATTERN
    VID_TYPE_DOT_TO_DOT_PATTERN_T *prDot2Dot;
#endif
    /* return value: 0 is ok, -1 is fail */

    /* check size */
    switch (e_vid_type)
    {
        /* ADC correction */
    case DRV_CUSTOM_VID_TYPE_ADC_R_GAIN:
    case DRV_CUSTOM_VID_TYPE_ADC_G_GAIN:
    case DRV_CUSTOM_VID_TYPE_ADC_B_GAIN:
    case DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET:
    case DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET:
    case DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET:
    case DRV_CUSTOM_VID_TYPE_ADC_R_MAX:
    case DRV_CUSTOM_VID_TYPE_ADC_G_MAX:
    case DRV_CUSTOM_VID_TYPE_ADC_B_MAX:
    //case DRV_CUSTOM_VID_TYPE_ADC_B_BLANK_LEVEL:
    //case DRV_CUSTOM_VID_TYPE_ADC_G_BLANK_LEVEL:
    //case DRV_CUSTOM_VID_TYPE_ADC_R_BLANK_LEVEL:
    //case DRV_CUSTOM_VID_TYPE_ADC_B_MAX_LEVEL:
    //case DRV_CUSTOM_VID_TYPE_ADC_G_MAX_LEVEL:
    //case DRV_CUSTOM_VID_TYPE_ADC_R_MAX_LEVEL:
    case DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE: //kaloverrule
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX:
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET:
        /* YPbPr Position */
    case DRV_CUSTOM_VID_TYPE_YPBPR_H_POS:
    case DRV_CUSTOM_VID_TYPE_YPBPR_V_POS:
        /* Color temperature */
    case DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN:
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN:
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN:
    case DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET:
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET:
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET:
        /* Spread spectrum value */
    case DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM:
        if (z_size != sizeof(UINT16))
        {
            return (-1);
        }
        break;

        /* gamma offset*/
    case DRV_CUSTOM_DISP_TYPE_GAMMA_OFFSET:
        if (z_size != sizeof(INT8))
        {
            return (-1);
        }
        break;

    case DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN:
        if (z_size != sizeof(UINT32))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_VID_TYPE_PQ_ALL_ONOFF:
    case DRV_CUSTOM_DISP_TYPE_PC_VIDEO_ONOFF:
        if (z_size != sizeof(BOOL))
        {
            return (-1);
        }
        break;

        /* Default value */
    case DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT:
    case DRV_CUSTOM_VID_TYPE_CONTRAST_DFT:
    case DRV_CUSTOM_VID_TYPE_SATURATION_DFT:
    case DRV_CUSTOM_VID_TYPE_HUE_DFT:
        /* Backlight value */
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_TOP:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_STANDARD:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_BOTTOM:
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL:
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE:
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH:
        if (z_size != sizeof(VID_INFO_T))
        {
            return (-1);
        }
        break;

        /* Gamma table */
    case DRV_CUSTOM_DISP_TYPE_GAMMA:
        if (z_size != sizeof(GAMMA_INFO_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF:
        if (z_size != sizeof(BOOL))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR:
        if (z_size != sizeof(OUTRGB_INFO_T))
        {
            return (-1);
        }
        break;
        /* Non-linear factor */
    case DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR:
        if (z_size != sizeof(VID_NON_LINEAR_INFO_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE:
    case DRV_CUSTOM_DISP_TYPE_HDMI_RANGE_MODE:
    case DRV_CUSTOM_VID_TYPE_YPBPR_VTH:
    case DRV_CUSTOM_VID_TYPE_YPBPR_VTL:
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN:
    case DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX:
        if (z_size != sizeof(UINT8))
        {
            return (-1);
        }
        break;

	case DRV_CUSTOM_VID_TYPE_GAMMA_SAVE_DELTA:
		if (z_size != sizeof(DRV_CUSTOM_GAMMA_DELTA_T))
        {
            return (-1);
        }
        break;
	case DRV_CUSTOM_VID_TYPE_GAMMA_PATTERN:
    case DRV_CUSTOM_VID_TYPE_MUTE_PATTERN:
		if (z_size != sizeof(DRV_CUSTOM_PATTERN_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_VID_TYPE_SCE_PATTERN:
        if (z_size != sizeof(DRV_CUSTOM_SCE_PATTERN_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_AUTO_BACK_LIGHT_SETS:
        if (z_size != sizeof(AUTO_BACKLIGHT_SETS_INFO_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_SETS:
        if (z_size != sizeof(AUTO_COLORTEMP_SETS_INFO_T))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_DISP_TYPE_CLARITY:
        if (z_size != sizeof(INT8))
        {
            return (-1);
        }
        break;
    case DRV_CUSTOM_VID_TYPE_PQ_DEMO:
        if (z_size != sizeof(PQ_DEMO_INFO_T))
        {
            return (-1);
        }
        break;         
    #if SUPPORT_DOT_TO_DOT_PATTERN
    case DRV_CUSTOM_VID_TYPE_DOT_TO_DOT_PATTERN:
        if(z_size != sizeof(VID_TYPE_DOT_TO_DOT_PATTERN_T) || (pv_set_info == NULL))
        {
            return (-1);
        }
        prDot2Dot = (VID_TYPE_DOT_TO_DOT_PATTERN_T*) pv_set_info;
        VDP_DotToDotPattern(prDot2Dot->b_on_off, prDot2Dot->ui4_rgb_value);
        break;
    #endif
    default:
        return (-1);
    }

    /* setup the data */
    switch (e_vid_type)
    {
    case DRV_CUSTOM_VID_TYPE_NON_LINEAR_FACTOR:
        prNonlinear = (VID_NON_LINEAR_INFO_T *) pv_set_info;
        VDP_SetNonLinear_Cust(prNonlinear->ui4_path, prNonlinear->b_auto,
                              prNonlinear->ui4_start, prNonlinear->ui4_slope,
                              prNonlinear->ui4_middle, prNonlinear->ui4_stop);
        return 0;
#if 0
    case DRV_CUSTOM_VID_TYPE_YPBPR_H_POS:
        vHdtv_CustPos_SetHPos(0, (UINT16) ((UINT32) pv_set_info));
        if (b_store)
        {
            vHdtv_CustPos_Update();
        }
        return 0;
    case DRV_CUSTOM_VID_TYPE_YPBPR_V_POS:
        vHdtv_CustPos_SetVPos(0, (UINT16) ((UINT32) pv_set_info));
        if (b_store)
        {
            vHdtv_CustPos_Update();
        }
        return 0;
#endif
    case DRV_CUSTOM_VID_TYPE_YPBPR_VTH:
        vCust_SOGY_VTH_Set((UINT32) pv_set_info);
        return 0;
    case DRV_CUSTOM_VID_TYPE_YPBPR_VTL:
        vCust_SOGY_VTL_Set((UINT32) pv_set_info);
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_R_GAIN:
        bDrvSetHDADCGain(2, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_R_Gain_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_GAIN:
        bDrvSetHDADCGain(1, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_G_Gain_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_GAIN:
        bDrvSetHDADCGain(0, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_B_Gain_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_R_OFFSET:
        bDrvSetHDADCOffset(2, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_R_Offset_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_G_OFFSET:
        bDrvSetHDADCOffset(1, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_G_Offset_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_ADC_B_OFFSET:
        bDrvSetHDADCOffset(0, (UINT16) ((UINT32) pv_set_info));
        if (b_store)            //save to EEPROM
            vCust_B_Offset_Update((UINT16) ((UINT32) pv_set_info));
        return 0;
    	case DRV_CUSTOM_VID_TYPE_ADC_R_MAX:
		if(b_store)	//save to EEPROM
			vCust_R_MAX_Update((UINT16)((UINT32)pv_set_info));
		return 0;
    	case DRV_CUSTOM_VID_TYPE_ADC_G_MAX:
		if(b_store)	//save to EEPROM
			vCust_G_MAX_Update((UINT16)((UINT32)pv_set_info));
		return 0;
    	case DRV_CUSTOM_VID_TYPE_ADC_B_MAX:
		if(b_store)	//save to EEPROM
			vCust_B_MAX_Update((UINT16)((UINT32)pv_set_info));
		return 0;

    case DRV_CUSTOM_VID_TYPE_ADC_EBGAIN_OVERRULE: //kaloverrule
		if(b_store)	//save to EEPROM
			vCust_EBGain_OverRule_Update((UINT16)((UINT32)pv_set_info));
		return 0;
	case DRV_CUSTOM_VID_TYPE_CLEAR_ADC_EEPROM:
        if(b_store)            //Clear to EEPROM
            vCust_clear_eeprom((UINT16) ((UINT32) pv_set_info));	
        return 0; 
    case DRV_CUSTOM_AUTOCOLOR_EEPROM_ENABLE:
         vCust_AutoColor_EEPROM_Enable((UINT8) ((UINT32) pv_set_info));
         return 0;

    case DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN:     /* Color temperature red gain */
        wWriteQualityTable(QUALITY_R_GAIN, (UINT16) ((UINT32) pv_set_info));
        vApiRegisterVideoEvent(PE_EVENT_R_GAIN, SV_VP_MAIN, SV_ON);        
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN:   /* Color temperature green gain */
        wWriteQualityTable(QUALITY_G_GAIN, (UINT16) ((UINT32) pv_set_info));
        vApiRegisterVideoEvent(PE_EVENT_G_GAIN, SV_VP_MAIN, SV_ON);        
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN:    /* Color temperature blue gain */
        wWriteQualityTable(QUALITY_B_GAIN, (UINT16) ((UINT32) pv_set_info));        
        vApiRegisterVideoEvent(PE_EVENT_B_GAIN, SV_VP_MAIN, SV_ON); 
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET:   /* Color temperature red offset */
        wWriteQualityTable(QUALITY_R_OFFSET, (UINT16) (((UINT32) pv_set_info)-0x200));
        vApiRegisterVideoEvent(PE_EVENT_R_OFFSET, SV_VP_MAIN, SV_ON);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET: /* Color temperature green offset */
        wWriteQualityTable(QUALITY_G_OFFSET, (UINT16) (((UINT32) pv_set_info)-0x200));
        vApiRegisterVideoEvent(PE_EVENT_G_OFFSET, SV_VP_MAIN, SV_ON);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET:  /* Color temperature blue offset */
        wWriteQualityTable(QUALITY_B_OFFSET, (UINT16) (((UINT32) pv_set_info)-0x200));
        vApiRegisterVideoEvent(PE_EVENT_B_OFFSET, SV_VP_MAIN, SV_ON); 
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN_PIP:     /* Color temperature red gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_R, (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN_PIP:   /* Color temperature green gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_G, (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN_PIP:    /* Color temperature blue gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_B, (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET_PIP:   /* Color temperature red offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_R, (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET_PIP: /* Color temperature green offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_G, (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET_PIP:  /* Color temperature blue offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_B, (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_GAMMA:
        switch (((GAMMA_INFO_T *) pv_set_info)->e_color_temp)
        {
        case DRV_CUSTOM_DISP_CT1:
            pu1GammaTbl = GAMMA_256[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
            break;
        case DRV_CUSTOM_DISP_CT2:
            pu1GammaTbl = GAMMA_256[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
            break;
        case DRV_CUSTOM_DISP_CT3:
            pu1GammaTbl = GAMMA_256[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
            break;
        case DRV_CUSTOM_DISP_CT0:
            pu1GammaTbl = NULL;
            // Means gamma off, don't do anything.
            return 0;
        default:
            pu1GammaTbl = NULL;
            return (-1);
        }

        for (i = 0; i < 256; i++)
        {
            pu1GammaTbl[i] = ((GAMMA_INFO_T *) pv_set_info)->ui1Tbl[i];
        }
        return 0;

    case DRV_CUSTOM_DISP_TYPE_GAMMA_OFFSET:
        return 0;

    case DRV_CUSTOM_DISP_TYPE_PC_VIDEO_ONOFF:
        vSetVGA422En((UINT8)((UINT32)pv_set_info));
        vVgaTriggerModeChange();
        // TBD.
        return 0;

    case DRV_CUSTOM_DISP_TYPE_SMART_PICTURE_INDEX:
        vApiFlashPqSetSmartPicId((UINT8) ((UINT32) pv_set_info));
        return 0;

    case DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR_ONOFF:      /*Output RGB test color enable */
        vHalOutRGBColorOnOff((UINT8) ((UINT32) pv_set_info));
        return 0;

    case DRV_CUSTOM_DISP_TYPE_OUTRGB_COLOR:    /*Output specific test color in RGB color space */
        vHalOutRGBColor(((OUTRGB_INFO_T *) pv_set_info)->ui1_r_value,
                        ((OUTRGB_INFO_T *) pv_set_info)->ui1_g_value,
                        ((OUTRGB_INFO_T *) pv_set_info)->ui1_b_value);
        return 0;

        // Spread spectrum
    case DRV_CUSTOM_DISP_TYPE_SPREAD_SPECTRUM:
        vDrvSetVOSpread((UINT16) ((UINT32) pv_set_info));
        return 0;
   	
    case DRV_CUSTOM_VID_TYPE_PQ_ALL_ONOFF:  //PQ effect all turn ON/OFF
        vVideoAllPQItemOnOff((UINT8)((UINT32)pv_set_info));
        return 0;

    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
        if ((UINT8) (((VID_INFO_T *) pv_set_info)->b_ignore_hw) == SV_FALSE)
        {
            vApiSetPanelBright((UINT8) (((VID_INFO_T *) pv_set_info)->ui2Value));
        }
        if (bApiSetQtyDft(SOURCE_TYPE_TIMING_PC_RGB, (UINT32)e_vid_type, 
                (((VID_INFO_T *) pv_set_info)->ui2Value)) == SV_FALSE)
        {
            return (-1);
        }
        break;
#ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_TOP:
        vSetPwmRatioTop(((VID_INFO_T *) pv_set_info)->ui2Value);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_STANDARD:
        vSetPwmRatioStandard(((VID_INFO_T *) pv_set_info)->ui2Value);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_RATIO_BOTTOM:
        vSetPwmRatioBottom(((VID_INFO_T *) pv_set_info)->ui2Value);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_FEATURE_SELECTIOIN:
        vApiFlashPqProcFeatureSel((UINT32 *) pv_set_info);
        return 0;
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
    case DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE:
        switch (*(UINT8 *) pv_set_info)
        {
            default:
            case DRV_CUSTOM_HDMI_AUTO:
                vApiVideoSetFixColorSpaceMode(SV_VP_MAIN, SV_FIXCOLORSPACE_OFF);
                break;
            case DRV_CUSTOM_HDMI_PC:
#ifdef THE_3RD_PARTY_SW_SUPPORT
                THE_3RD_PARTY_SET_SRC(9);   // PC digital
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
                vApiVideoSetFixColorSpaceMode(SV_VP_MAIN, bHDMIInputType()? SV_FIXCOLORSPACE_RGB_444 : SV_FIXCOLORSPACE_OFF);
                break;
            case DRV_CUSTOM_HDMI_HD:
#ifdef THE_3RD_PARTY_SW_SUPPORT
                THE_3RD_PARTY_SET_SRC(8);   // HDMI HD
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
                vApiVideoSetFixColorSpaceMode(SV_VP_MAIN, bHDMIInputType()? SV_FIXCOLORSPACE_RGB_422 : SV_FIXCOLORSPACE_YCBCR);
                break;
        }
        return 0;
        case DRV_CUSTOM_DISP_TYPE_HDMI_RANGE_MODE:
            vSetHDMIRangeMode((UINT8)((UINT32)pv_set_info));
            vApiHdmiColorModeChg(SV_VP_MAIN);
        return 0;
    //TVD ADC
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_GAIN:
        vDrvSetTVDADCGain((UINT8)((UINT32)pv_set_info));
        if(b_store)	//save to EEPROM
            vCust_TVD_Gain_Update((UINT8)((UINT32)pv_set_info));
        return 0;
    //TVD Offset
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_OFFSET:
        vDrvSetTVDADCOffset((UINT16)((UINT32)pv_set_info));
        if(b_store)	//save to EEPROM
            vCust_TVD_Offset_Update((UINT16)((UINT32)pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_TVD_ADC_MAX:
        if(b_store)	//save to EEPROM
            vCust_TVD_ADC_MAX_Update((UINT16)((UINT32)pv_set_info));
        return 0;

	case DRV_CUSTOM_VID_TYPE_GAMMA_SAVE_DELTA:
#if SUPPORT_GAMMA_DELTA
	    vHalSetGammaSaveDelta((DRV_CUSTOM_GAMMA_DELTA_T *)pv_set_info);
#endif
		return 0;
	case DRV_CUSTOM_VID_TYPE_GAMMA_PATTERN:
        if (((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui1_OnOff)
		{
		    vDrvGammaMute(((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_r,
        				((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_g,
        				((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_b);
        }
        else
		{
		    vDrvGammaUnmute();
        }
		return 0;
    case DRV_CUSTOM_VID_TYPE_MUTE_PATTERN:
        if (((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui1_OnOff)
		{
		    VDP_SetMode(VDP_1, VDP_MODE_BG); 
		    VDP_SetBg(VDP_1, (((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_r<<16)
		                        | (((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_g<<8)
		                        | (((DRV_CUSTOM_PATTERN_T *)pv_set_info)->ui2_b));
        }
        else
		{
		    VDP_SetMode(VDP_1, VDP_MODE_NORMAL); 
        }
        return 0;
    case DRV_CUSTOM_VID_TYPE_SCE_PATTERN:
        vHalSetSCEPat(((DRV_CUSTOM_SCE_PATTERN_T *)pv_set_info)->ui1_OnOff,
            ((DRV_CUSTOM_SCE_PATTERN_T *)pv_set_info)->ui2_Ydata);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_AUTO_BACK_LIGHT_SETS:
        DRVCUST_AutoBacklightUpdate((AUTO_BACKLIGHT_SETS_INFO_T *)pv_set_info);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_AUTO_COLORTEMP_SETS:
        DRVCUST_AutoColorTempUpdate((AUTO_COLORTEMP_SETS_INFO_T *)pv_set_info);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CLARITY:
        DRVCUST_ClarityUpdate((INT8)((UINT32)pv_set_info));
        return 0;
    case DRV_CUSTOM_VID_TYPE_PQ_DEMO:
    	rDemoRegion.wHStart = ((PQ_DEMO_INFO_T *)pv_set_info)->wHStart;
    	rDemoRegion.wHEnd = ((PQ_DEMO_INFO_T *)pv_set_info)->wHEnd;
    	rDemoRegion.wVStart = ((PQ_DEMO_INFO_T *)pv_set_info)->wVStart;
    	rDemoRegion.wVEnd = ((PQ_DEMO_INFO_T *)pv_set_info)->wVEnd;    	
    	vApiVideoDemoMode(((PQ_DEMO_INFO_T *)pv_set_info)->bDemoMode,
    	 					 rDemoRegion,
    	 					 ((PQ_DEMO_INFO_T*)pv_set_info)->bDemoModules);
    	  return 0;        
    default:
        break;
    }
    return 0;
}
#endif

/**
 * @brief Set the default value for MDY_QTY_TBL table.
 */
 // TODO:: Rename
UINT8 bApiSetQtyTblDft(void)
{
    // Read PWM value from panel_table.c
    vHalSetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX, PANEL_GetBacklightHigh());
    vHalSetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX, PANEL_GetBacklightMiddle());
    vHalSetAdpBacklight(ADP_BACKLIGHT_LOW_IDX, PANEL_GetBacklightLow());

    bApiSetQtyDft(0, DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX,
                  PANEL_GetBacklightHigh());
    bApiSetQtyDft(0, DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN,
                  PANEL_GetBacklightLow());
    bApiSetQtyDft(0, DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT,
                  PANEL_GetBacklightMiddle());

    return SV_TRUE;
}

/**
 * @brief Set the default value for MDY_QTY_TBL.
 * @param u1SrcTypTmg
 * @param u2QtyItem QUALITY_xxx
 * @param u1Value default value
 */
UINT8 bApiSetQtyDft(UINT8 u1SrcTypTmg, UINT32 u4UiItem, UINT16 u2Value)
{
    //UINT8 u1TypTmgGroup;

    LOG(0,"Timing %8x  item %8x  value %8x \n", u1SrcTypTmg, u4UiItem, u2Value);

    // Get the correct quality index.
    if (u1SrcTypTmg >= SOURCE_TYPE_TIMING_MAX)
    {
        u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);
    }

    switch (u4UiItem)
    {
    case DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT:
        vHalVideoBrightness(SV_VP_MAIN, u2Value);
        break;
    case DRV_CUSTOM_VID_TYPE_CONTRAST_DFT:
        vHalVideoContrast(SV_VP_MAIN, u2Value);
        break;
    case DRV_CUSTOM_VID_TYPE_HUE_DFT:
        vHalVideoHue(SV_VP_MAIN, u2Value);
        break;
    case DRV_CUSTOM_VID_TYPE_SATURATION_DFT:
        vHalVideoSaturation(SV_VP_MAIN, u2Value);
        break;

    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
        vApiSetPanelBright(u2Value);
        break;
        // Normal means 100% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL:
        vHalSetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX, u2Value);
        vApiSetPanelBright(u2Value);
        return SV_TRUE;
        // Middle means 67% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE:
        vHalSetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX, u2Value);
        vApiSetPanelBright(u2Value);
        return SV_TRUE;
        // High means 50% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH:
        vHalSetAdpBacklight(ADP_BACKLIGHT_LOW_IDX, u2Value);
        vApiSetPanelBright(u2Value);
        return SV_TRUE;

    default:
        ASSERT(0);
    }

    // BACKLIGHT setting only in default quality table.
    if ((u4UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN) ||
        (u4UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX) ||
        (u4UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT))
    {
        // Overwrite default quality table value.
        if (u4UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN)
        {
            WRITE_DEFUALT_QTY_TBL_MIN(QUALITY_BACKLIGHT, u2Value);
        }
        else if (u4UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX)
        {
            WRITE_DEFUALT_QTY_TBL_MAX(QUALITY_BACKLIGHT, u2Value);
        }
        else
        {
            WRITE_DEFUALT_QTY_TBL_DFT(QUALITY_BACKLIGHT, u2Value);
        }
    }
    else                        // Other items in custom quality table.
    {
        #if 0
        // Update all default value for the same group. Eg. Update Ypp, Ypp480i/p, Ypp1080i/p.
        u1SrcTypTmg = u1SrcTypTmg & 0xF0;
        for (u1TypTmgGroup = 0; u1TypTmgGroup < 16; u1TypTmgGroup++)
        {
            if (bApiGetCustomSourceTypeTiming(u1SrcTypTmg + u1TypTmgGroup) ==
                SOURCE_TYPE_TIMING_MAX)
            {
                break;
            }
            WRITE_CUSTOM_QTY_TBL(
                bApiGetCustomSourceTypeTiming(u1SrcTypTmg + u1TypTmgGroup),
                u4UiItem,
                u2Value);
        }
        #endif

        printf("fix me ==> item %8x \n", u4UiItem);
    }
    return SV_TRUE;
}

