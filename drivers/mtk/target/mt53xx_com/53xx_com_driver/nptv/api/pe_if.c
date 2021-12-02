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
 * $RCSfile: pe_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _PE_IF_C_

#include "x_util.h"
#include "x_assert.h"
#include "x_os.h"

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
#include "vdo_misc.h"
#include "vdp_drvif.h"
#include "drv_scpos.h"
#include "drv_upscaler.h"
#include "storage_if.h"
#include "vdp_if.h"
#ifndef CC_COPLAT_MT82

#include "eeprom_if.h"
#include "u_drv_cust.h"
#include "nor_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif

#endif
#include "drvcust_if.h"
#include "drv_hdtv.h"
#include "drv_mpeg.h"
#include "nptv_debug.h"
#include "x_hal_5381.h"
#include "drv_vdoclk.h"
#include "drv_adcset.h"
#include "drv_autocolor.h"
#include "drv_di.h"
#include "drv_nr.h"
#include "drv_scaler.h"

#include "pe_if.h"
#include "drv_pq_cust.h"

#include "source_select.h"
#include "c_model.h"
#include "drv_hdmi.h"

#include "hal_ycproc.h"
#include "drv_display.h"
#include "panel.h"
#include "drv_video.h"
#define SUPPORT_GAMMA_DELTA 0

#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
	#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifndef CUSTOM_VIDEO_DRV_INCLUDE_FILE

#define ENABLE_FLASH_PQ_DEBUG 0

BOOL _fgCallFromIsr = SV_OFF;

UINT8 QUALITY_TBL[QUALITY_MAX];
UINT8 bRegisterList[PE_ARG_NS][REGISTER_DIM];
INT16 ai2Arg[VDP_NS][PE_ARG_NS];   // video quality item current value (UI)

struct tagFileHead aFlashPqFileHead;
struct tagTblHead aFlashPqGammaHead, aFlashPqSceHead, aFlashPqQtyHead;
UINT64 u8PqBaseAddr;

#ifdef SUPPORT_FLASH_PQ
    #ifdef SUPPORT_FLASH_SMART_PICTURE
        extern struct _SMART_PIC_GROUP_INFO_T aFlashPqSmartPicGroup;
    #endif // #ifdef SUPPORT_FLASH_SMART_PICTURE
#endif // #ifdef SUPPORT_FLASH_PQ

#if SUPPORT_SIX_COLOR_MANAGE   
STR_SIX_COLOR gSixBaseColor = {0xFF, 0xFF, 0};

CONST  STR_SIX_COLOR_RNG aSixBaseColorRNG[BASE_COLOR_MAX] = 
{
//bStartPhase,  bEndPhase,   partialYStartPhase, partialYEndPhase
	6,       13,   		 4,       6,
	20,	     23,   		 9,      11,
	0,	     1,   		14,      15,
	17,	     19,  		 7,       8,
	2,	     5,   		 2,       3,
	24,	     25,  		12,      13,
};
#endif

#ifdef DRV_SUPPORT_EXTMJC
static UINT8 bMjcUiEffect;
static UINT8 bMjcUiMode;
static UINT8 bMjcUiDemo;
//static UINT8 bMjcUiDemoMode;
#endif

static const UCHAR *ucPeUiType[PE_ARG_NS] = 
{
    "BLENDING",
    "BRIGHTNESS",
    "CONTRAST",
    "HUE",
    "SATURATION",
    "CTI",
    "LTI",
    "SHARPNESS_ON_OFF",
    "SHARPNESS",
    "NR",
    "3D_NR",
    "MPEG_NR",
    "ADAPTIVE_LUMA",
    "COLOR_SUPPRESS",
    "SCE",
    "WHITE_PEAK_LMT",
    "BACK_LIGHT_LVL",
    "ADAPTIVE_BACK_LIGHT",
    "SOFT_BWS",
    "CDS",
    "XVYCC",
    "WCG",
    "BLUE_STRETCH",
    "R_GAIN",
    "G_GAIN",
    "B_GAIN",
    "R_OFFSET",
    "G_OFFSET",
    "B_OFFSET",
    "GAMMA",
    "DEMO",
    "DI_FILM_MODE",
    "DI_MA",
    "DI_EDGE_PRESERVE",
    "MJC_MODE",
    "MJC_EFFECT",
    "MJC_DEMO",
    "GAME_MODE",
    "BLACK_STRETCH",
    "WHITE_STRETCH",
    "OPC_CURVE"
};

EXTERN void DRVCUST_SET_YGAMMA(UINT32 i1Mode);


static PE_UI_VQ_MIN_MAX_DFT_T aUiVqMinMaxDft[EVDP_VIS_MAX][EVDP_UVQI_MAX];

static PE_UI_VQ_MIN_MAX_DFT_T aUiVqMinMaxDftDefault[EVDP_UVQI_MAX] =
{
    // min   max   dft
    {   0,  100,    50},      // EVDP_UVQI_BRIGHTNESS
    {   0,  100,    50},      // EVDP_UVQI_CONTRAST
    { -50,  50,      0},      // EVDP_UVQI_HUE
    {   0,  100,    50},      // EVDP_UVQI_SATURATION
    {   0,    3,     2},      // EVDP_UVQI_CTI
    {   0,   20,    10},      // EVDP_UVQI_SHARPNESS
    {   0,   20,    10},      // EVDP_UVQI_LTI
    {   0,  255,   128},      // EVDP_UVQI_COLOR_GAIN
    {   0,  255,   128},      // EVDP_UVQI_COLOR_OFFSET
    {   0,  100,    50},      // EVDP_UVQI_COLOR_SUPPRESS
    {   0,    3,     2},      // EVDP_UVQI_NR
    {   0,    3,     2},      // EVDP_UVQI_GAMMA
    {   0,    1,     1},      // EVDP_UVQI_WHITE_PEAK_LMT
    {   0,    3,     2},      // EVDP_UVQI_FLESH_TONE
    {   0,    3,     2},      // EVDP_UVQI_LUMA
    {   0,   100,   50},      // EVDP_UVQI_BACK_LIGHT_LVL
    {   0,    2,     0},      // EVDP_UVQI_ADAPTIVEBACK_LIGHT
    {   0,    4,     2},      // EVDP_UVQI_3D_NR
    {   0,    3,     0},      // EVDP_UVQI_DEMO
    {   0,    1,     1},      // EVDP_UVQI_DI_FILM_MODE
    {   0,    1,     0},      // EVDP_UVQI_DI_MA,
    {   0,    1,     0},      // EVDP_UVQI_DI_EDGE_PRESERVE,
    {   0,    1,     0},      // EVDP_UVQI_MJC_MODE,
    {   0,    3,     1},      // EVDP_UVQI_MJC_EFFECT,
    {   0,    1,     0},      // EVDP_UVQI_MJC_DEMO,
    {   0,    1,     0},      // EVDP_UVQI_XVYCC,
    {   0,    1,     0},      // EVDP_UVQI_WCG,
    {   0,    1,     0},      // EVDP_UVQI_GAME_MODE,
    {   0,    1,     0},      // EVDP_UVQI_BLUE_STRETCH,
    {   0,    3,     2},      // EVDP_UVQI_MPEG_NR,
    {   0,    3,     2},      // EVDP_UVQI_BLACK_STRETCH,
    {   0,    3,     2},      // EVDP_UVQI_WHITE_STRETCH,
};

extern void vApiFlashPqSetSmartPicId(UINT8 bIndex);

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

    switch ((UINT8) e_vid_type)
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

    switch ((UINT8) e_vid_type)
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

    // select panel
    case DRV_CUSTOM_DISP_TYPE_PANEL_SELECT:
    {
        if (*pz_size != sizeof(UINT16))
        {
            return (-1);
        }
        *((UINT16*)pv_get_info) = (UINT16)GetCurrentPanelIndex();
        
        return 0;
    }

    // get LVDS spread spectrum
    case DRV_CUSTOM_DISP_TYPE_LVDS_SPREAD_SPECTRUM_SETTING:
    {
        if (*pz_size != sizeof(LVDS_SPREAD_SPECTRUM_INFO_T))
        {
            return (-1);
        }
        LVDS_SPREAD_SPECTRUM_INFO_T *pInfo = (LVDS_SPREAD_SPECTRUM_INFO_T *)pv_get_info;
        pInfo->u4_freq = DDDS_GetSpreadSpectrumFreq();
        pInfo->u4_permillage = DDDS_GetSpreadSpectrum();
        return 0;
    }

    //get LVDS format
    case DRV_CUSTOM_DISP_TYPE_LVDS_FORMAT:
    {
        if (*pz_size != sizeof(DRV_CUSTOM_LVDS_FORMAT_T))
        {
            return (-1);
        }
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        if (u4ControlWord | LVDS_MSB_SW_ON)
        {
            *((DRV_CUSTOM_LVDS_FORMAT_T *)pv_get_info) = DRV_CUSTOM_LVDS_FORMAT_JEIDA;
        }
        else  // NS
        {
            *((DRV_CUSTOM_LVDS_FORMAT_T *)pv_get_info) = DRV_CUSTOM_LVDS_FORMAT_NS;      
        }
        return 0;
    }
    
    // LVDS Bit Depth Setting 
    case DRV_CUSTOM_DISP_TYPE_LVDS_BIT_DEPTH:
    {
        if (*pz_size != sizeof(DRV_CUSTOM_LVDS_BIT_DEPTH_T))
        {
            return (-1);
        }
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        u4ControlWord = u4ControlWord & (DISP_BIT_MSK);
        if (u4ControlWord == DISP_30BIT)
        {
            *((DRV_CUSTOM_LVDS_BIT_DEPTH_T *)pv_get_info) = DRV_CUSTOM_LVDS_BIT_DEPTH_30BIT;
        }
        else if (u4ControlWord == DISP_24BIT)
        {
            *((DRV_CUSTOM_LVDS_BIT_DEPTH_T *)pv_get_info) = DRV_CUSTOM_LVDS_BIT_DEPTH_24BIT;
        }
        else
        {
            *((DRV_CUSTOM_LVDS_BIT_DEPTH_T *)pv_get_info) = DRV_CUSTOM_LVDS_BIT_DEPTH_18BIT;
        }
        return 0;
    }

    // LVDS Channel Swap Setting 
    case DRV_CUSTOM_DISP_TYPE_LVDS_CHANNEL_SWAP:
    {
        if (*pz_size != sizeof(UINT16))
        {
            return (-1);
        }
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        if (u4ControlWord & LVDS_ODD_SW_ON)
        {
            *((UINT16*)pv_get_info) = 1;
        }
        else
        {
            *((UINT16*)pv_get_info) = 0;
        }
        return 0;
    }        

    // Get Panel Resolution 
    case DRV_CUSTOM_DISP_TYPE_GET_PANEL_RESOLUTION:
    {
        if (*pz_size != sizeof(VID_TYPE_PANEL_RESOLUTION_T))
        {
            return (-1);
        }
        VID_TYPE_PANEL_RESOLUTION_T *pInfo = (VID_TYPE_PANEL_RESOLUTION_T *)pv_get_info;
        pInfo->ui4_width = PANEL_GetPanelWidth();
        pInfo->ui4_height = PANEL_GetPanelHeight();
        return 0;
    }

        
#ifdef SUPPORT_FLASH_SMART_PICTURE
    case DRV_CUSTOM_DISP_TYPE_PICTURE_INFO:
        *((struct _PIC_INFO_T *) pv_get_info) = aApiGetPictureInfo();
        return 0;
#endif
#ifdef SUPPORT_FLASH_PQ
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
#ifdef SUPPORT_FLASH_SMART_PICTURE
    case DRV_CUSTOM_DISP_TYPE_SMART_PICTURE:
        *((struct _SMART_PIC_GROUP_INFO_T *) pv_get_info) =
            aFlashPqSmartPicGroup;
        return 0;
#endif // #ifdef SUPPORT_FLASH_SMART_PICTURE
#endif // #ifdef SUPPORT_FLASH_PQ
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
            *((UINT8*)pv_get_info) = (UINT16)bApiEepromReadByte(EEP_VIDEO_AUTO_CALIB_AV_GAIN0);
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


    switch ((UINT8) e_vid_type)
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

        ui1Tbl = GAMMA_256_DARK[pu1GammaTbl->ui1Channel];
        break;
    case DRV_CUSTOM_DISP_CT2:
        ui1Tbl = GAMMA_256_MIDDLE[pu1GammaTbl->ui1Channel];
        break;
    case DRV_CUSTOM_DISP_CT3:
        ui1Tbl = GAMMA_256_BRIGHT[pu1GammaTbl->ui1Channel];
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
    switch ((UINT8) e_vid_type)
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
    switch ((UINT8) e_vid_type)
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
        wWriteQualityTable(QUALITY_R_GAIN,
                           (UINT16) ((UINT32) pv_set_info)>>2);
        wWriteQualityTable(QUALITY_R_GAIN_LSB,
                           (UINT16) ((UINT32) pv_set_info) & 0x0003);
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R,
                             (UINT16) ((UINT32) pv_set_info));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_R_GAIN, SV_VP_MAIN, SV_ON);
        }
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN:   /* Color temperature green gain */
        wWriteQualityTable(QUALITY_G_GAIN,
                           (UINT16) ((UINT32) pv_set_info)>>2);
        wWriteQualityTable(QUALITY_G_GAIN_LSB,
                           (UINT16) ((UINT32) pv_set_info) & 0x0003);
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G,
                             (UINT16) ((UINT32) pv_set_info));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_G_GAIN, SV_VP_MAIN, SV_ON);
        }
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN:    /* Color temperature blue gain */
        wWriteQualityTable(QUALITY_B_GAIN,
                           (UINT16) ((UINT32) pv_set_info)>>2);
        wWriteQualityTable(QUALITY_B_GAIN_LSB,
                           (UINT16) ((UINT32) pv_set_info) & 0x0003);
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B,
                             (UINT16) ((UINT32) pv_set_info));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_B_GAIN, SV_VP_MAIN, SV_ON);
        }
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET:   /* Color temperature red offset */
            wWriteQualityTable(QUALITY_R_OFFSET,
                               (UINT16) (((UINT32) pv_set_info)-0x200) >> 2);
            wWriteQualityTable(QUALITY_R_OFFSET_LSB,
                               (UINT16) (((UINT32) pv_set_info)-0x200) & 0x0003);
            if (_fgCallFromIsr == SV_ON)
            {
                vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R,
                               (UINT16) ((UINT32) pv_set_info));
            }
            else
            {
                vApiRegisterVideoEvent(PE_ARG_R_OFFSET, SV_VP_MAIN, SV_ON);
            }
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET: /* Color temperature green offset */
        wWriteQualityTable(QUALITY_G_OFFSET,
                           (UINT16) (((UINT32) pv_set_info)-0x200) >> 2);
        wWriteQualityTable(QUALITY_G_OFFSET_LSB,
                           (UINT16) (((UINT32) pv_set_info)-0x200) & 0x0003);
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G,
                           (UINT16) ((UINT32) pv_set_info));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_G_OFFSET, SV_VP_MAIN, SV_ON);
        }
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET:  /* Color temperature blue offset */
        wWriteQualityTable(QUALITY_B_OFFSET,
                           (UINT16) (((UINT32) pv_set_info)-0x200) >> 2);
        wWriteQualityTable(QUALITY_B_OFFSET_LSB,
                           (UINT16) (((UINT32) pv_set_info)-0x200) & 0x0003);
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B,
                           (UINT16) ((UINT32) pv_set_info));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_B_OFFSET, SV_VP_MAIN, SV_ON);
        }
        return 0;

    case DRV_CUSTOM_DISP_TYPE_CT_RED_GAIN_PIP:     /* Color temperature red gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_R,
                         (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_GAIN_PIP:   /* Color temperature green gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_G,
                         (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_GAIN_PIP:    /* Color temperature blue gain */
        vHalVideoRGBGain(SV_VP_PIP, SV_RGB_B,
                         (UINT16) ((UINT32) pv_set_info));
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_RED_OFFSET_PIP:   /* Color temperature red offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_R,
                           (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_GREEN_OFFSET_PIP: /* Color temperature green offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_G,
                           (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;
    case DRV_CUSTOM_DISP_TYPE_CT_BLUE_OFFSET_PIP:  /* Color temperature blue offset */
        vHalVideoRGBOffset(SV_VP_PIP, SV_RGB_B,
                           (UINT16) ((UINT32)pv_set_info)-0x200);
        return 0;

    case DRV_CUSTOM_DISP_TYPE_GAMMA:
        switch (((GAMMA_INFO_T *) pv_set_info)->e_color_temp)
        {
        case DRV_CUSTOM_DISP_CT1:

            pu1GammaTbl =
                GAMMA_256_DARK[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
            break;
        case DRV_CUSTOM_DISP_CT2:

            pu1GammaTbl =
                GAMMA_256_MIDDLE[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
            break;
        case DRV_CUSTOM_DISP_CT3:

            pu1GammaTbl =
                GAMMA_256_BRIGHT[((GAMMA_INFO_T *) pv_set_info)->ui1Channel];
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
        DRVCUST_SET_YGAMMA((UINT32)pv_set_info);
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

    // select panel
    case DRV_CUSTOM_DISP_TYPE_PANEL_SELECT:
    {
        if (z_size != sizeof(UINT16))
        {
            return (-1);
        }
        UINT16 u2PanelIdx = (*((UINT16 *)pv_set_info));  
        if (u2PanelIdx >= PANEL_TOTAL_NUMBER)
        {
            return (-1);
        }
        SelectPanel(u2PanelIdx);

        // turn off panel
        vApiPanelPowerSequence(FALSE);
        bSI_DISPLAY_DCLK_TYPE = SV_DCLK_UNKNOW;

        vDrvOutputStageInit();    
        vDrvLCDFreqSet(SV_DCLK_60HZ, 60);        
        //OSTG_OnOutputVSync();
        //vFrameTrackConfig(3, 20, 4);
        //IGNORE_RET(OSD_UpdateTiming());

        vApiPanelPowerSequence(TRUE);

        // DumpPanelAttribute(PANEL_DUMP_CURRENT);       
        //LVDS 7->4 FIFO enable 
        //vDrvLVDS7To4FifoEnable();
        break;
    }
    
    // set LVDS spread spectrum
    case DRV_CUSTOM_DISP_TYPE_LVDS_SPREAD_SPECTRUM_SETTING:
    {
        if (z_size != sizeof(LVDS_SPREAD_SPECTRUM_INFO_T))
        {
            return (-1);
        }
        LVDS_SPREAD_SPECTRUM_INFO_T *pInfo = (LVDS_SPREAD_SPECTRUM_INFO_T *)pv_set_info;
        if (pInfo->u4_freq == LVDS_SPREAD_SPECTRUM_IGNORE)
        {
            pInfo->u4_freq = DDDS_GetSpreadSpectrumFreq();
        }
        if (pInfo->u4_permillage == LVDS_SPREAD_SPECTRUM_IGNORE)
        {
            pInfo->u4_permillage = DDDS_GetSpreadSpectrum();
        }        
        DDDS_SetSpreadSpectrum(pInfo->u4_freq, pInfo->u4_permillage);
        break;
    }

    // LVDS format Setting 
    case DRV_CUSTOM_DISP_TYPE_LVDS_FORMAT:
    {
        if (z_size != sizeof(DRV_CUSTOM_LVDS_FORMAT_T))
        {
            return (-1);
        }
        DRV_CUSTOM_LVDS_FORMAT_T *pInfo = (DRV_CUSTOM_LVDS_FORMAT_T *)pv_set_info;
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        if (*pInfo == DRV_CUSTOM_LVDS_FORMAT_JEIDA)
        {
            u4ControlWord = u4ControlWord | LVDS_MSB_SW_ON;
        }
        else if (*pInfo == DRV_CUSTOM_LVDS_FORMAT_NS) // NS
        {
            u4ControlWord = u4ControlWord & (~LVDS_MSB_SW_ON);        
        }
        PANEL_SetControlWord(u4ControlWord);
        vDrvLVDSConfig();

        break;
    }
    
    // LVDS Bit Depth Setting 
    case DRV_CUSTOM_DISP_TYPE_LVDS_BIT_DEPTH:
    {
        if (z_size != sizeof(DRV_CUSTOM_LVDS_BIT_DEPTH_T))
        {
            return (-1);
        }
        DRV_CUSTOM_LVDS_BIT_DEPTH_T *pInfo = (DRV_CUSTOM_LVDS_BIT_DEPTH_T *)pv_set_info;
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        u4ControlWord = u4ControlWord & (~DISP_BIT_MSK);

        if (*pInfo == DRV_CUSTOM_LVDS_BIT_DEPTH_30BIT)
        {
            u4ControlWord |= DISP_30BIT;
        }
        else if (*pInfo == DRV_CUSTOM_LVDS_BIT_DEPTH_24BIT)
        {
            u4ControlWord |= DISP_24BIT;
        }
        else
        {
            u4ControlWord |= DISP_18BIT;
        }

        PANEL_SetControlWord(u4ControlWord);
        vDrvLVDSOn();     

        break;
    }

    // LVDS Channel Swap Setting 
    case DRV_CUSTOM_DISP_TYPE_LVDS_CHANNEL_SWAP:
    {
        if (z_size != sizeof(UINT16))
        {
            return (-1);
        }
        UINT32 u4ControlWord;
        u4ControlWord = PANEL_GetControlWord();
        u4ControlWord = u4ControlWord & (~LVDS_ODD_SW_ON);

        if ((*((UINT16 *)pv_set_info)) == 0)
        {
            u4ControlWord |= LVDS_ODD_SW_OFF;
        }
        else
        {
            u4ControlWord |= LVDS_ODD_SW_ON;
        }

        PANEL_SetControlWord(u4ControlWord);
        vDrvLVDSConfig();         

        break;
    }        

    
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
        if (bApiSetQtyDft(SOURCE_TYPE_TIMING_PC_RGB, (UINT8) e_vid_type, 
                (UINT8) (((VID_INFO_T *) pv_set_info)->ui2Value)) == SV_FALSE)
        {
            return (-1);
        }
        break;
#ifdef SUPPORT_FLASH_SMART_PICTURE
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
#endif // #ifdef SUPPORT_FLASH_SMART_PICTURE
    case DRV_CUSTOM_DISP_TYPE_HDMI_PCHD_MODE:
        switch (*(UINT8 *) pv_set_info)
        {
        default:
        case DRV_CUSTOM_HDMI_AUTO:
            vApiVideoFixColorSpace(SV_VP_MAIN, 0);
            break;
        case DRV_CUSTOM_HDMI_PC:
#ifdef THE_3RD_PARTY_SW_SUPPORT
            THE_3RD_PARTY_SET_SRC(9);   // PC digital
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
            vApiVideoFixColorSpace(SV_VP_MAIN, bHDMIInputType()? 17 : 0);
            break;
        case DRV_CUSTOM_HDMI_HD:
#ifdef THE_3RD_PARTY_SW_SUPPORT
            THE_3RD_PARTY_SET_SRC(8);   // HDMI HD
#endif // #ifdef THE_3RD_PARTY_SW_SUPPORT
            vApiVideoFixColorSpace(SV_VP_MAIN, bHDMIInputType()? 18 : 19);
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

void vApiVideoSRMNRYCOnOff(UINT8 bYOnOff, UINT8 bCOnOff)
{
//    vDrvNRTIIRMotionYCOnOff(bYOnOff,bCOnOff);
}

void vApiVideoNRTIIROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNROnOff(bOnOff);
}


void vApiVideoNR10bitMode(UINT8 bOnOff)
{
//    vDrvNRSetDramMode(bOnOff);
}

void vApiVideo2DNROnOff(UINT8 bOnOff)
{
    vDrvNRSet2DNROnOff(bOnOff);
}

void vApiVideo3DNROnOff(UINT8 bOnOff)
{
    vDrvNRSet3DNRUIOnOff(bOnOff);
}

void vApiVideoMPEGNROnOff(UINT8 bOnOff)
{
    vDrvNRSetDeblockingOnOff(bOnOff);
    vDrvNRSetMosquitoOnOff(bOnOff);
}


void PE_SetUiMinMaxDft(void)
{
    INT32 i4VisIdx;

    // Set all UI video quality item of input source's
    // min/max/default to default value.
    for (i4VisIdx = 0; i4VisIdx < (INT32)EVDP_VIS_MAX; i4VisIdx++)
    {
        x_memcpy(&aUiVqMinMaxDft[i4VisIdx],
                 aUiVqMinMaxDftDefault,
                 sizeof(aUiVqMinMaxDftDefault));
    } // ~for

    // Set all UI video quality items' current to default value.
    PE_SetAllUiVqItemDftValues();
}


void vApiVideoPeInit(void)
{
#ifdef SUPPORT_FLASH_PQ
    // Restore quality table from flash if support FlashPQ function.
    if (bApiFlashPqVerifyHeader() == SV_TRUE)
    {
        if (bApiFlashPqUpdateGamma() == SV_TRUE)
        {
            LOG(0, "Restore FlashPQ Gamma successful\n");
        }
        if (bApiFlashPqUpdateSCE())
        {
            LOG(0, "Restore FlashPQ SCE successful\n");
        }
        if (bApiFlashPqQtyDftInit())
        {
            LOG(0, "Restore FlashPQ Qty successful\n");
        }
    }
#endif    // #ifdef SUPPORT_FLASH_PQ

    UNUSED(bApiParseCustomQtyItem());

#ifdef DRV_SUPPORT_EXTMJC
    if (IS_COMPANION_CHIP_ON())
    {
        vHalVideoExternalGammaInit();
    }
#endif

    // Restore quality table from EEPROM if support factory mode.
    UNUSED(bApiSetQtyTblDft());

    // Load the SCE table
    vHalVideoSCELoadTable(SV_VP_MAIN, 1, 0x80, 0x80, 0x80);
    vHalVideoSCELoadTable(SV_VP_PIP, 1, 0x80, 0x80, 0x80);

    // force gamma apply in vsync even in system boot.
    if(DRVCUST_OptGet(eNVMGamma))
    {
        DRVCUST_SET_GAMMA(); //Read Gamma from EEPROM
    }
    else
    {
        vApiRegisterVideoEvent(PE_ARG_GAMMA, SV_VP_MAIN, SV_ON);
    }

    // initial PQ setting
    vIO32WriteFldAlign(GLOBAL_ADJ_00, 0x80, BRIGHT_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_00, 0x80, HUE_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_02, 0x80, CONT_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_02, 0x80, SAT_DUMMY);

    vIO32WriteFldAlign(GLOBAL_ADJ_01, 0x80, BRIGHT_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_01, 0x80, HUE_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_03, 0x80, SAT_DUMMY);
    vIO32WriteFldAlign(GLOBAL_ADJ_03, 0x80, CONT_DUMMY);
}

/**
 * @brief Set the default value for MDY_QTY_TBL.
 * @param u1SrcTypTmg
 * @param u2QtyItem QUALITY_xxx
 * @param u1Value default value
 */
UINT8 bApiSetQtyDft(UINT8 u1SrcTypTmg, UINT8 u1UiItem, UINT8 u1Value)
{
    UINT8 u1TypTmgGroup;
//    UINT16 u2QtyItem = 0;

    // Get the correct quality index.
    if (u1SrcTypTmg >= SOURCE_TYPE_TIMING_MAX)
    {
        u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(SV_VP_MAIN);
    }

    switch (u1UiItem)
    {
    case DRV_CUSTOM_VID_TYPE_BRIGHTNESS_DFT:
        vHalVideoBrightness(SV_VP_MAIN, u1Value);
        break;
    case DRV_CUSTOM_VID_TYPE_CONTRAST_DFT:
        vHalVideoContrast(SV_VP_MAIN, u1Value);
        break;
    case DRV_CUSTOM_VID_TYPE_HUE_DFT:
        vHalVideoHue(SV_VP_MAIN, u1Value);
        break;
    case DRV_CUSTOM_VID_TYPE_SATURATION_DFT:
        vHalVideoSaturation(SV_VP_MAIN, u1Value);
        break;

    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX:
    case DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT:
        vApiSetPanelBright(u1Value);
        break;
        // Normal means 100% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_NORMAL:
        vHalSetAdpBacklight(ADP_BACKLIGHT_HIGH_IDX, u1Value);
        vApiSetPanelBright(u1Value);
        return SV_TRUE;
        // Middle means 67% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_MIDDLE:
        vHalSetAdpBacklight(ADP_BACKLIGHT_MIDDLE_IDX, u1Value);
        vApiSetPanelBright(u1Value);
        return SV_TRUE;
        // High means 50% backlight luminance.
    case DRV_CUSTOM_DISP_TYPE_ADP_BACK_LIGHT_HIGH:
        vHalSetAdpBacklight(ADP_BACKLIGHT_LOW_IDX, u1Value);
        vApiSetPanelBright(u1Value);
        return SV_TRUE;

    default:
        ASSERT(0);
    }

    // BACKLIGHT setting only in default quality table.
    if ((u1UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN) ||
        (u1UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX) ||
        (u1UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_DFT))
    {
        // Overwrite default quality table value.
        if (u1UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MIN)
        {
            WRITE_DEFUALT_QTY_TBL_MIN(QUALITY_BACKLIGHT, u1Value);
        }
        else if (u1UiItem == DRV_CUSTOM_DISP_TYPE_BACK_LIGHT_MAX)
        {
            WRITE_DEFUALT_QTY_TBL_MAX(QUALITY_BACKLIGHT, u1Value);
        }
        else
        {
            WRITE_DEFUALT_QTY_TBL_DFT(QUALITY_BACKLIGHT, u1Value);
        }
    }
    else                        // Other items in custom quality table.
    {
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
                u1UiItem,
                u1Value);
        }
    }
    return SV_TRUE;
}

/**
 * @brief Set the default value for MDY_QTY_TBL table.
 */
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
 * @brief Mapping from UI_min, UI_max, UI_dft, UI_cur, HW_min, HW_max, HW_dft, to HW cur.
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param u2QtyItem VDP_ARG_xxx
 * @param pi2Min, pi2Max, pi2Dft, pi2Cur: UI values.
 */
UINT16 bApiQtyMapping(UINT8 u1SrcTypTmg, UINT16 u2QtyItem,
                     INT16 const *pi2Min, INT16 const *pi2Max,
                     INT16 const *pi2Dft, INT16 const *pi2Cur)
{
    UINT16 u2HwBegin, u2HwEnd;

    UINT16 u1Min = 0;
    UINT16 u1Max = 0;
    UINT16 u1Dft = 0;

    if ((pi2Min == NULL) || (pi2Max == NULL) || (pi2Dft == NULL)
        || (pi2Cur == NULL))
    {
        return 0;
    }

    if (u2QtyItem >= QUALITY_MAX)
    {
        Printf("DEFUALT_QTY_TBL range index error! %d\n", u2QtyItem);
        return 0;
    }

    u1Min = READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
    u1Max = READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
    u1Dft = bApiGetQtyDft(u2QtyItem, u1SrcTypTmg);

    if(u2QtyItem == QUALITY_SATURATION)
    {
        u2QtyItem = QUALITY_SATURATION_HIBYTE;
        u1Min += ((UINT16)READ_DEFUALT_QTY_TBL_MIN(u2QtyItem)) <<8;
        u1Max += ((UINT16)READ_DEFUALT_QTY_TBL_MAX(u2QtyItem)) <<8;
        u1Dft += ((UINT16)bApiGetQtyDft(u2QtyItem, u1SrcTypTmg)) <<8;
    }

    if(u2QtyItem == QUALITY_CONTRAST)
    {
        u2QtyItem = QUALITY_CONTRAST_HIBYTE;
        u1Min += ((UINT16)READ_DEFUALT_QTY_TBL_MIN(u2QtyItem)) <<8;
        u1Max += ((UINT16)READ_DEFUALT_QTY_TBL_MAX(u2QtyItem)) <<8;
        u1Dft += ((UINT16)bApiGetQtyDft(u2QtyItem, u1SrcTypTmg)) <<8;
    }

    if((u2QtyItem == QUALITY_R_GAIN_LSB) ||
        (u2QtyItem == QUALITY_G_GAIN_LSB) ||
        (u2QtyItem == QUALITY_B_GAIN_LSB))
    {
        u2QtyItem -= 3;
        u1Min += ((UINT16)READ_DEFUALT_QTY_TBL_MIN(u2QtyItem)) <<2;
        u1Max += ((UINT16)READ_DEFUALT_QTY_TBL_MAX(u2QtyItem)) <<2;
        u1Dft += ((UINT16)bApiGetQtyDft(u2QtyItem, u1SrcTypTmg)) <<2;
    }

    // Some qty_item does not need mapping, return default HW value.
    // or UI_cur == UI_dft, return default HW value.
    if (((u1Min == 0) && (u1Max == 0)) || ((INT16) * pi2Cur == *pi2Dft))
    {
        return u1Dft;
    }

    // Some qty_item use 2-level setting as min/max/default, don't mapping.
    if (*pi2Max == 2)           // UI_max=2 means UI option is: off, low, high.
    {
        if ((INT16) * pi2Dft == 0)
        {
            if ((INT16) * pi2Cur == 0)
            {
                return u1Dft;
            }
            else if ((INT16) * pi2Cur == 1)
            {
                return u1Max;
            }
            else if ((INT16) * pi2Cur == 2)
            {
                return u1Max;
            }
        }
        else if ((INT16) * pi2Dft == 1)
        {
            if ((INT16) * pi2Cur == 0)
            {
                return u1Min;
            }
            else if ((INT16) * pi2Cur == 1)
            {
                return u1Dft;
            }
            else if ((INT16) * pi2Cur == 2)
            {
                return u1Max;
            }
        }
        else // pi2Dft == 2
        {
            if ((INT16) * pi2Cur == 0)
            {
                return u1Min;
            }
            else if ((INT16) * pi2Cur == 1)
            {
                return u1Min;
            }
            else if ((INT16) * pi2Cur == 2)
            {
                return u1Dft;
            }
        }
    }

    // Some qty_item use 3-level setting as min/max/default, don't mapping.
    // eg. NR registers don't fit linear mapping, use 3-level LUT instead.

    if(!DRVCUST_OptGet(eFlagUIQTYLinearMap))
    {
    if (*pi2Max == 3)           // UI_max=3 means UI option is: off, low, mid, hi.
    {
        if ((INT16) * pi2Cur == 1)
        {
            return u1Min;
        }
        else if ((INT16) * pi2Cur == 2)
        {
            return u1Dft;
        }
        else if ((INT16) * pi2Cur == 3)
        {
            return u1Max;
        }
    }
    }
    // Get HW value for UI_current > UI_default.
    if ((INT16) * pi2Cur > (INT16) * pi2Dft)
    {
        u2HwBegin = u1Dft;
        u2HwEnd = u1Max;

        // Prevent UI_max, UI_dft miss setting, and div=0.
        if ((*pi2Max - *pi2Dft) <= 0)
        {
            return u1Dft;
        }
        else
        {
            return (UINT16) (((UINT16) * pi2Cur - *pi2Dft) * (u2HwEnd -
                                                             u2HwBegin) /
                            (*pi2Max - *pi2Dft) + u2HwBegin);
        }
    }
    // Get HW value for UI_current < UI_default.
    else
    {
        u2HwBegin = u1Min;
        u2HwEnd = u1Dft;

        // Prevent UI_dft, UI_min miss setting, and div=0.
        if ((*pi2Dft - *pi2Min) <= 0)
        {
            return u1Dft;
        }
        else
        {
            return (UINT16) (((UINT16) * pi2Cur - *pi2Min) * (u2HwEnd -
                                                             u2HwBegin) /
                            (*pi2Dft - *pi2Min) + u2HwBegin);
        }
    }
}

/**
 * @brief Parse the bCustomQtyItem[] table.
 *
 */
UINT8 bApiParseCustomQtyItem()
{
    UINT16 u2SearchDftIndex, u2SearchCustomIndex;

    for (u2SearchDftIndex = 0; u2SearchDftIndex < QUALITY_MAX; u2SearchDftIndex++)
    {
        for (u2SearchCustomIndex = 0; u2SearchCustomIndex <  CUSTOM_QTY_TBL_DIM ; u2SearchCustomIndex++)
        {
            if (READ_CUST_QTY_ITEM(u2SearchCustomIndex) >= QUALITY_MAX)
            {
                // Can not find this item is CustomQtyItem table.
                // So this item will use default value in DefaultQtyTbl[].
                break;
            }

            else if (READ_CUST_QTY_ITEM(u2SearchCustomIndex) == u2SearchDftIndex)
            {
                // This item is found in CustomQtyItem, mark its serial number.
                // CustomQtyTble for this item will be used, not use default value.
                WRITE_DEFUALT_QTY_TBL_REF(u2SearchDftIndex,u2SearchCustomIndex);
                break;
            }
        }
    }
    return SV_SUCCESS;
}

/**
 * @brief Translate source type/timing index into customer defined index.
 * @param bSrcTypTmg SOURCE_TYPE_TIMING_RFTV ~ SOURCE_TYPE_TIMING_MAX
 *
 */
UINT8 bApiGetCustomSourceTypeTiming(UINT8 bSrcTypTmg)
{
    UINT8 i;
    UINT8 bCustomSrcTypTmg;

    //Search 2 Times, first time as original type , second time as parant type
    for (i = 2; i != 0; --i)
    {
        for (bCustomSrcTypTmg = 0;
             //End of the bCustomSourceGroup Table
             (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN) !=
              SOURCE_TYPE_TIMING_MAX)
             && (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END) !=
                 SOURCE_TYPE_TIMING_MAX); bCustomSrcTypTmg++)
        {

            if (READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN) >
                READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END))
            {
                // Fix QTY_IDX_RANGE_END.
                WRITE_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END,
                    READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN));
            }

            if ((bSrcTypTmg >=
                 READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_BEGIN))
                && (bSrcTypTmg <=
                    READ_CUSTOM_SRC_GROUP(bCustomSrcTypTmg,QTY_IDX_RANGE_END)))
            {
                return bCustomSrcTypTmg;
            }
        }

        //Cast Children type to Parent type
        bSrcTypTmg = bSrcTypTmg & 0xF0;
    }

    // Can not find proper quality index.
    return SOURCE_TYPE_TIMING_MAX;
}

/**
 * @brief Search the default value from DefaultQtyTbl or CustomQtyTbl.
 * @param wSearchIndex
 * @param bQtyIdx
 */
UINT8 bApiGetQtyDft(UINT16 wSearchItem, UINT8 bQtyIdx)
{
    // After run bApiParseCustomQtyItem, if reference is FROM_DFT, use default table.
    // Or input source is un-defined in customer source group, use default table.
    if ((READ_DEFUALT_QTY_TBL_REF(wSearchItem) == FROM_DFT) ||
        (bQtyIdx >= SOURCE_TYPE_TIMING_MAX))
    {
        // Directly use the default quality table.
        return READ_DEFUALT_QTY_TBL_DFT(wSearchItem);
    }

    // This quality item can be found in custom table.
    return
        READ_CUSTOM_QTY_TBL(bQtyIdx,READ_DEFUALT_QTY_TBL_REF(wSearchItem));
}


/**
 * @brief Search the min value from DefaultQtyTbl.
 * @param wSearchIndex
 */
UINT8 bApiGetQtyMin(UINT16 u2QtyItem)
{
    return READ_DEFUALT_QTY_TBL_MIN(u2QtyItem);
}

/**
 * @brief Search the min value from DefaultQtyTbl.
 * @param wSearchIndex
 */
UINT8 bApiGetQtyMax(UINT16 u2QtyItem)
{
    return READ_DEFUALT_QTY_TBL_MAX(u2QtyItem);
}

/**
 * @brief Register video process event, wait for apply to hardware in output v-sync.
 * @param bVdpArg VDP_ARG_xx
 * @param bPath <SV_VP_MAIN/SV_VP_PIP>
 * @param bOnOff <SV_ON/SV_OFF>
 */
void vApiRegisterVideoEvent(UINT8 bVdpArg, UINT8 bPath, UINT8 bOnOff)
{
    bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_ON;
    bRegisterList[bVdpArg][REGISTER_PATH] = bPath;
    bRegisterList[bVdpArg][REGISTER_ON_OFF] = bOnOff;
}

/**
 * @brief Call corresponding video API in bRegisterList
 *
 */
void vApiApplyVideoEventOutputVSync(void)
{
    UINT8 bVdpArg;

    for (bVdpArg = 0; bVdpArg < PE_ARG_NS; bVdpArg++)
    {
        if (bRegisterList[bVdpArg][REGISTER_CALLBACK] == SV_ON)
        {
            switch (bVdpArg)
            {
            case PE_ARG_BRIGHTNESS:   ///< brightness
                vHalVideoBrightness(bRegisterList[bVdpArg][REGISTER_PATH],
                                    wReadQualityTable(QUALITY_BRIGHTNESS));
                break;
            case PE_ARG_CONTRAST:     ///< contrast
                vHalVideoContrast(bRegisterList[bVdpArg][REGISTER_PATH],
                    wReadQualityTable(QUALITY_CONTRAST) |
                    ((UINT16)wReadQualityTable(QUALITY_CONTRAST_HIBYTE)) << 8);
                break;
            case PE_ARG_HUE:  ///< hue
                vHalVideoHue(bRegisterList[bVdpArg][REGISTER_PATH],
                             wReadQualityTable(QUALITY_HUE));
                break;
            case PE_ARG_SATURATION:   ///< saturation
                vHalVideoSaturation(bRegisterList[bVdpArg][REGISTER_PATH],
                    wReadQualityTable(QUALITY_SATURATION) |
                    ((UINT16)wReadQualityTable(QUALITY_SATURATION_HIBYTE)) << 8);
                break;
            case PE_ARG_GAMMA:
                vHalVideoGammaLoadTable(bRegisterList[bVdpArg][REGISTER_PATH]); //i2Cur==0 means turn off.
                break;
            case PE_ARG_R_GAIN:       ///< R-gain
                vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                 SV_RGB_R,
                                 (wReadQualityTable(QUALITY_R_GAIN) << 2)
                                 | (wReadQualityTable(QUALITY_R_GAIN_LSB)));
                break;
            case PE_ARG_G_GAIN:       ///< G-gain
                vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                 SV_RGB_G,
                                 (wReadQualityTable(QUALITY_G_GAIN) << 2)
                                 | (wReadQualityTable(QUALITY_G_GAIN_LSB)));
                break;
            case PE_ARG_B_GAIN:       ///< B-gain
                vHalVideoRGBGain(bRegisterList[bVdpArg][REGISTER_PATH],
                                 SV_RGB_B,
                                 (wReadQualityTable(QUALITY_B_GAIN) << 2)
                                 | (wReadQualityTable(QUALITY_B_GAIN_LSB)));
                break;
            case PE_ARG_R_OFFSET:     ///< R-offset
                vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                   SV_RGB_R,
                                   (wReadQualityTable(QUALITY_R_OFFSET) << 2)
                                   | wReadQualityTable(QUALITY_R_OFFSET_LSB));
                break;
            case PE_ARG_G_OFFSET:     ///< G-offset
                vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                   SV_RGB_G,
                                   (wReadQualityTable(QUALITY_G_OFFSET) << 2)
                                   | wReadQualityTable(QUALITY_G_OFFSET_LSB));
                break;
            case PE_ARG_B_OFFSET:     ///< B-offset
                vHalVideoRGBOffset(bRegisterList[bVdpArg][REGISTER_PATH],
                                   SV_RGB_B,
                                   (wReadQualityTable(QUALITY_B_OFFSET) << 2)
                                   | wReadQualityTable(QUALITY_B_OFFSET_LSB));
                break;
         // Move Adaptive Luma and LCE switch to VSync ,
         //to aviod transient status when turn off Adaptive Luma. shijiang.feng 2009-07-17
            case PE_ARG_ADAPTIVE_LUMA:
                 #ifdef CC_MT5363
                 vHalVideoAdaptiveLumaOnOff(bRegisterList[bVdpArg][REGISTER_PATH],
                       bRegisterList[bVdpArg][REGISTER_ON_OFF]);
                 #endif
                 break;
            case PE_ARG_SCE:
           		vHalVideoSCELoadTable(bRegisterList[bVdpArg][REGISTER_PATH],
           		    SV_V_SCE_DEFAULT, 0x80, 0x80, 0x80);
           		break;
				
#if SUPPORT_SIX_COLOR_MANAGE	
		    case PE_ARG_SIX_COLOR:
				vHalSCELoadTblPartial(gSixBaseColor.bColor, 
									gSixBaseColor.bHSY, 
									gSixBaseColor.u2Offset,
									gSixBaseColor.bPhaseBegin, 
									gSixBaseColor.bPhaseEnd);
			    break;
#endif
				
           	case PE_ARG_OPC_CURVE:
           	    // OPC Curve update here.
           	    //todo : opc
           	    vHalVideoOPCResultUpdate();
                break;
            default:
            case PE_ARG_CTI:
            case PE_ARG_SHARPNESS_ON_OFF:
            case PE_ARG_SHARPNESS:
            case PE_ARG_NR:
            case PE_ARG_BLACK_STRETCH:
            case PE_ARG_WHITE_STRETCH:
            case PE_ARG_WHITE_PEAK_LMT:
            case PE_ARG_BACK_LIGHT_LVL:
            case PE_ARG_ADAPTIVE_BACK_LIGHT:
            case PE_ARG_3D_NR:
            case PE_ARG_SOFT_BWS:
            case PE_ARG_DEMO:
                break;
            }

            // After call video api, turn-off callback flag.
            bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_OFF;
        }
    }
}

void vApiApplyVideoEventInputVSync(UINT8 u1VidPath)
{
#if 0   //Moved to output vsync: MAIN:VGA, PIP:All
    UINT8 bVdpArg;

    for (bVdpArg = PE_ARG_BRIGHTNESS; bVdpArg <= PE_ARG_SATURATION;
         bVdpArg++)
    {
        if (bRegisterList[bVdpArg][REGISTER_CALLBACK] == SV_ON)
        {
            if ((bRegisterList[bVdpArg][REGISTER_PATH] == u1VidPath) &&
                fgApiVideoIsVgaTiming(bRegisterList[bVdpArg][REGISTER_PATH]))
            {
                switch (bVdpArg)
                {
                case PE_ARG_BRIGHTNESS:
                case PE_ARG_CONTRAST:
                case PE_ARG_HUE:
                case PE_ARG_SATURATION:
                    //vHalVideoColorMatrix(bRegisterList[bVdpArg][REGISTER_PATH]);
                    break;
                default:
                    break;
                }
                // After call video api, turn-off callback flag.
                bRegisterList[bVdpArg][REGISTER_CALLBACK] = SV_OFF;
            }
        }
    }
#endif
}

/**
 * @brief Get video delay from luma histogram to LVDS output.
 * @return delay frame number.
 */
UINT8 bApiGetVideoDelay(void)
{
    return (0);
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/

extern UINT16 wDrvDIGetWidth(void);
extern UINT16 wScpipGetDispWidth(UINT8 bPath);
extern UINT16 wScpipGetDispHeight(UINT8 bPath); 
void vApiVideoDemoMode(UINT8 bmode, Region DemoRec, UINT8 bDEMOModules)
{
    Region DemoRegion;
    UINT8 bSplitOnOff, bMovingOnOff;
    UINT8 bRegion=0;
    UINT16 wDispCenter;
    UINT16 wInputWidth = wDrvDIGetWidth();
    UINT16 wDispWidth = wScpipGetDispWidth(SV_VP_MAIN);
    wDispCenter = (UINT16)u8Div6432(wDispWidth, 2, NULL);
    DemoRegion.wVStart = 0x0;
    DemoRegion.wVEnd = wScpipGetDispHeight(SV_VP_MAIN);

    //Fix Klocwork
    DemoRegion.wHStart = 0x0;
    DemoRegion.wHEnd = wDispCenter;
    DemoRegion.u1OnOff = SV_ON;
    switch (bmode)
    {
    case VDP_PQ_DEMO_ON_LEFT://Split demo
        bSplitOnOff = SV_ON;        
        bRegion = SV_LEFT;
        DemoRegion.u1OnOff = SV_ON;        
        DemoRegion.wHStart = 0x0;
        DemoRegion.wHEnd = wDispCenter;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_ON_RIGHT://Split demo
        bSplitOnOff = SV_ON;    
        bRegion = SV_RIGHT;
        DemoRegion.u1OnOff = SV_ON;        
        DemoRegion.wHStart = wDispCenter;
        DemoRegion.wHEnd = wDispWidth;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_STILL_REC://Rectangular demo
        bSplitOnOff = SV_OFF;
        DemoRegion.u1OnOff = SV_ON;
        DemoRegion.wHStart = DemoRec.wHStart;
        DemoRegion.wHEnd = DemoRec.wHEnd;
        DemoRegion.wVStart = DemoRec.wVStart;
        DemoRegion.wVEnd = DemoRec.wVEnd;
        bMovingOnOff = SV_OFF;
        break;
    case VDP_PQ_DEMO_MOVING_REC:
        bSplitOnOff = SV_OFF;
        DemoRegion.u1OnOff = SV_ON;
        DemoRegion.wHStart = DemoRec.wHStart;
        DemoRegion.wHEnd = DemoRec.wHEnd;
        DemoRegion.wVStart = DemoRec.wVStart;
        DemoRegion.wVEnd = DemoRec.wVEnd;
        bMovingOnOff = SV_ON;
    	 break;
    case VDP_PQ_DEMO_OFF:
    default:
         bSplitOnOff = SV_OFF;
         DemoRegion.u1OnOff = SV_OFF;
         bMovingOnOff = SV_OFF;
        break;
    }
    //Set Demo modules	
    vIO32WriteFldAlign(DEMO_02, bDEMOModules, DEMO_MODULES);
    
    //Split demo
    vDrvHSharpDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_HSHARP_EN)), bRegion, (wInputWidth/2)/8);
    vDrvDIDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_MDDI_EN)), bRegion,(wInputWidth/2)+0x44);   // 0x44 is for MEMA Demo position alignment. why 0x44? just test the position, and got this difference value.
    vDrv2DNRDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_2DNR_EN)), bRegion, (wInputWidth/2));
    vDrvTDCDemo(bSplitOnOff&&(IO32ReadFldAlign(DEMO_02, DEMO_TDC_EN)), bRegion, (wInputWidth/2));

    //Rectangular demo
    vDemoSetRegion(DemoRegion);
    vIO32WriteFldAlign(DEMO_00, bMovingOnOff, MOVINGEMO_ENABLE);
}

#ifdef SUPPORT_FLASH_PQ
UINT64 u8CalcFlashPqOffset(
    UINT16 nType,
    UINT16 nModel,
    UINT16 nSmartPic,
    UINT16 nSource,
    UINT16 nIdx)
{
    if (nType == TYPE_QTY_CONTENT_MIN)
    {
        nSmartPic = aFlashPqFileHead.bNumOfSmartPic;    // The end of all smart pic + 1 is MIN table.
        nType = TYPE_QTY_CONTENT;
    }
    else if (nType == TYPE_QTY_CONTENT_MAX)
    {
        nSmartPic = aFlashPqFileHead.bNumOfSmartPic + 1;    // The end of all smart pic + 2 is MAX table.
        nType = TYPE_QTY_CONTENT;
    }

    switch (nType)
    {
    case TYPE_PQ_HEADER:
        return u8PqBaseAddr;
    case TYPE_GAMMA_HEADER:
        return
            u8PqBaseAddr +
            (sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * nModel);
    case TYPE_GAMMA_CONTENT:
        return
            u8PqBaseAddr +
            (sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * nModel +
             TBL_HEAD_SIZE) + nIdx;
    case TYPE_SCE_HEADER:
        return
            u8PqBaseAddr +
            (sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * nModel);
    case TYPE_SCE_CONTENT:
        return
            u8PqBaseAddr +
            (sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * nModel +
             TBL_HEAD_SIZE + nIdx);
    case TYPE_QTY_HEADER:
        return
            u8PqBaseAddr +
            ( sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * aFlashPqFileHead.wNumOfSCE +

            (TBL_HEAD_SIZE +
            (aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) *
            (aFlashPqFileHead.bNumOfSmartPic + ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0))) *
            nModel);
    case TYPE_QTY_DESCRIPT:
        return
            u8PqBaseAddr +
            ( sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * aFlashPqFileHead.wNumOfSCE +

            (TBL_HEAD_SIZE +
            (aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) *
            (aFlashPqFileHead.bNumOfSmartPic + ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0))) *
            nModel +

            TBL_HEAD_SIZE +
            (nSmartPic * aFlashPqFileHead.wSizeOfQtyTable *
            aFlashPqFileHead.bNumOfTiming) +

            nSource * aFlashPqFileHead.wSizeOfQtyTable);
    case TYPE_QTY_CONTENT:
        return
            u8PqBaseAddr +
            ( sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * aFlashPqFileHead.wNumOfSCE +

            (TBL_HEAD_SIZE +
            (aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) *
            (aFlashPqFileHead.bNumOfSmartPic + ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0))) *
            nModel +

            TBL_HEAD_SIZE +
            (nSmartPic * aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) +

            nSource * aFlashPqFileHead.wSizeOfQtyTable +
            ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)? TIMING_SCRIPT_SIZE : 0) +
            nIdx);
  case TYPE_QTY_ITEM:
        return
            u8PqBaseAddr +
            ( sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * aFlashPqFileHead.wNumOfSCE +

            (TBL_HEAD_SIZE +
            (aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) *
            (aFlashPqFileHead.bNumOfSmartPic + ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0))) *
            aFlashPqFileHead.wNumOfQty);
	default:
    case TYPE_FILE_END:
        return
            u8PqBaseAddr +
            ( sizeof(struct tagFileHead) +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfGammaTable) * aFlashPqFileHead.wNumOfGamma +
            (TBL_HEAD_SIZE +
             aFlashPqFileHead.wSizeOfSCETable) * aFlashPqFileHead.wNumOfSCE +

            (TBL_HEAD_SIZE +
            (aFlashPqFileHead.wSizeOfQtyTable *
             aFlashPqFileHead.bNumOfTiming) *
            (aFlashPqFileHead.bNumOfSmartPic + ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN)? 2 : 0))) *
             aFlashPqFileHead.wNumOfQty) +
            ((aFlashPqFileHead.bRes & RES_SUPPORT_SAVE_QTY_HEADER)?
             aFlashPqFileHead.wSizeOfQtyTable * ITEM_BYTES : 0);
    }
}

int nCalcFlashPqQtyNumber(void)
{
    int i;

    for (i=0; i<CUSTOM_QTY_TBL_DIM; i++)
    {
        if (READ_CUST_QTY_ITEM(i) == QUALITY_MAX)
        {
            return i;
        }
    }

    return (-1);
}

// Compare the hash between QtyItem.h and FlashPQ QtyHeader.
BOOL fgCheckFlashPqQtyHash(void)
{
	int nQtyNumber = 0;
	UINT8 i;

	// Get the hash from QtyItem.h
	nQtyNumber = nCalcFlashPqQtyNumber();
    if (nQtyNumber == (-1))
    {
        LOG(0, "QTY item error!\n");
        return SV_FALSE;
    }
	nQtyNumber ++; // Get the next position after the QUALITY_MAX entry.

	// Compare the hashs.
	for (i=0; i< TBL_HEAD_HASH; i++)
	{
		// Array index range check.
		if (nQtyNumber+i < CUSTOM_QTY_TBL_DIM + TBL_HEAD_HASH)
		{
			if (READ_CUST_QTY_ITEM(nQtyNumber+i) !=
				aFlashPqQtyHead.bHash[i])
			{
				LOG(0, "hash error at %d\n", i);
				return SV_FALSE;
			}
		}
		else
		{
			LOG(0, "Qty index range error %d\n", nQtyNumber+i);
			return SV_FALSE;
		}
	}
	// Compare finished.
	return SV_TRUE;
}

void vApiFlashPqReadFlash(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    if (DRVCUST_OptGet(eFlashPqUseNandFalsh))
    {
        //NANDPART_Read(u8Offset, u4MemPtr, u4MemLen);
        STORG_SyncNandRead((DRVCUST_OptGet(eFlashPqBasePartition)) ,u8Offset, (VOID*)u4MemPtr, u4MemLen);
    }
    else
    {
        NOR_Read(u8Offset, u4MemPtr, u4MemLen);
    }
}

UINT8 * bApiFlashPqGetVersion(UINT8 bType)
{
    switch (bType)
    {
    default:
    case FLASHPQ_XML_VER:
        return (UINT8 *)(aFlashPqFileHead.bQtyXmlVer);
    case FLASHPQ_GAMMA_VER:
        return (UINT8 *)(aFlashPqGammaHead.bScript);
    case FLASHPQ_SCE_VER:
        return (UINT8 *)(aFlashPqSceHead.bScript);
    case FLASHPQ_QTY_VER:
        return (UINT8 *)(aFlashPqQtyHead.bScript);
    }
}

UINT8 bApiFlashPqVerifyHeader(void)
{
    if (DRVCUST_OptGet(eFlagFlashPqEnable))
    {
        // Check if flash PQ address & block size definition.
        if ((((UINT64)DRVCUST_OptGet(eFlashPqBasePartition)<<32) +
            DRVCUST_OptGet(eFlashPqBaseAddress) == 0) ||
            (DRVCUST_OptGet(eFlashPqBlockSize) == 0))
        {
#if ENABLE_FLASH_PQ_DEBUG
            LOG(0, "FlashPQ base address zero!!!\n");
#endif // #if ENABLE_FLASH_PQ_DEBUG
            return (SV_FALSE);
        }

        // Get PQ base address.
        u8PqBaseAddr =
            ((UINT64)DRVCUST_OptGet(eFlashPqBasePartition)<<32) +
            DRVCUST_OptGet(eFlashPqBaseAddress) +
            DRVCUST_OptGet(eFlashPqOffset);

        // PQ base address must be 32-bit alignment.
        ASSERT((u8PqBaseAddr % 4) == 0);

        vApiFlashPqReadFlash(u8PqBaseAddr,
            (UINT32)&aFlashPqFileHead,
            (UINT32)(sizeof(struct tagFileHead)));

#if ENABLE_FLASH_PQ_DEBUG
        LOG(0, "FlashPQ check header\n");
        LOG(0, "PQ base address:%X %X\n", u8PqBaseAddr>>32, (UINT32)u8PqBaseAddr);
        LOG(0, "id=%s\n", aFlashPqFileHead.bID);
        LOG(0, "Gamma:\n");
        LOG(0, "num=%d\n", aFlashPqFileHead.wNumOfGamma);
        LOG(0, "size header=%d\n", TBL_HEAD_SIZE);
        LOG(0, "size table=%d\n", aFlashPqFileHead.wSizeOfGammaTable);

        LOG(0, "SCE\n");
        LOG(0, "num=%d\n", aFlashPqFileHead.wNumOfSCE);
        LOG(0, "size header=%d\n", TBL_HEAD_SIZE);
        LOG(0, "size table=%d\n", aFlashPqFileHead.wSizeOfSCETable);

        LOG(0, "Qty\n");
        LOG(0, "num=%d\n", aFlashPqFileHead.wNumOfQty);
        LOG(0, "size header=%d\n", TBL_HEAD_SIZE);
        LOG(0, "size table=%d\n", aFlashPqFileHead.wSizeOfQtyTable);
        LOG(0, "size timing=%d\n", aFlashPqFileHead.bNumOfTiming);
        LOG(0, "ver=%s\n\n", aFlashPqFileHead.bQtyXmlVer);
#endif // #if ENABLE_FLASH_PQ_DEBUG

        if (x_strcmp((CHAR *) (aFlashPqFileHead.bID), (CHAR *) FLASHPQ_ID) == 0)
        {
			if ((aFlashPqFileHead.wNumOfGamma>>8) == 48)
			{
				aFlashPqFileHead.wNumOfGamma = aFlashPqFileHead.wNumOfGamma & 0xFF;
			}
			if ((aFlashPqFileHead.wNumOfSCE>>8) == 48)
			{
				aFlashPqFileHead.wNumOfSCE= aFlashPqFileHead.wNumOfSCE & 0xFF;
			}
			if ((aFlashPqFileHead.wNumOfQty>>8) == 48)
			{
				aFlashPqFileHead.wNumOfQty = aFlashPqFileHead.wNumOfQty & 0xFF;
			}

            LOG(0, "FlashPQ ID OK\n");
            return (SV_TRUE);
        }
        else
        {
            LOG(0, "FlashPQ ID NG\n");
            return (SV_FALSE);
        }
    }
    else
    {
        LOG(0, "Don't Support FlashPQ\n");
        return (SV_FALSE);
    }
}

#define FLASH_PQ_GAMMA_CHANNEL_NUM 3
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_256  256
#define FLASH_PQ_GAMMA_CHANNEL_SIZE_257  257

void bApiGammaTopPt(UINT8 * pu1Table)
{
    if(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] >= pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2])
    {
        pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
            pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 1] +
            (pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] - pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 - 2]);
    }
    else
    {
        pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256] =
            pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1] +
            (UINT8)((UINT16)(pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -1]+0x100) - pu1Table[FLASH_PQ_GAMMA_CHANNEL_SIZE_256 -2]);
    }
}


UINT8 bApiFlashPqUpdateGamma(void)
{
    UINT8 bi, u1GammaNum;
    UINT8* u1GammaBufPtr;
    UINT64 u8GammaAddr;
#if ENABLE_FLASH_PQ_DEBUG
    UINT32 i;
#endif // #if ENABLE_FLASH_PQ_DEBUG
    UINT16 u2GammaMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead.wSizeOfGammaTable;
    UINT16 u2GammaChlSize;

    u1GammaNum = DRVCUST_OptGet(eFlagFlashPqGammaX3) ? 3 : 1;

    if (DRVCUST_OptGet(eFlagFlashPqEnableGamma) == 0)
    {
        LOG(0, "FlashPQ not support Gamma\n");
        return (SV_FALSE);
    }

    // Calculate start address of Gamma table.
    u8GammaAddr = u8CalcFlashPqOffset(
        TYPE_GAMMA_HEADER,
        vApiFlashPqGetGammaId((UINT8) GetCurrentPanelIndex()),
        0,
        0,
        0);

    // Gamma head base address must be 32-bit alignment.
    if ((u8GammaAddr % 4) != 0)
    {
        LOG(0, "FlashPQ gamma address error!!!(%d)\n", u8GammaAddr);
        return (SV_FALSE);
    }

    // Check Gamma table size in FlashPQ bin file: 256 or 257
    if (aFlashPqFileHead.wSizeOfGammaTable == FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM)
    {
        u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_256;
    }
    else if (aFlashPqFileHead.wSizeOfGammaTable == FLASH_PQ_GAMMA_CHANNEL_SIZE_256 * FLASH_PQ_GAMMA_CHANNEL_NUM + 16)
    {
        u2GammaChlSize = FLASH_PQ_GAMMA_CHANNEL_SIZE_257;
    }
    else
    {
        LOG(0, "FlashPQ Gamma size error!!!\n");
        return (SV_FALSE);
    }

    // Read all gamma table from flash: 1 or 3 sets of Gamma
    if ((u1GammaBufPtr = (UINT8*)x_mem_alloc(u2GammaMdlSize * u1GammaNum)) == NULL)
    {
        LOG(0, "FlashPQ Gamma memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u8GammaAddr,
        (UINT32)u1GammaBufPtr,
        (UINT32)(u2GammaMdlSize * u1GammaNum));

    // Check all Gamma ID: 1 or 3 sets
    for (bi = 0; bi < u1GammaNum; bi ++)
    {
        x_memcpy(&aFlashPqGammaHead, u1GammaBufPtr + bi * u2GammaMdlSize, TBL_HEAD_SIZE);

        if (x_strcmp((CHAR *) (aFlashPqGammaHead.bID), (CHAR *) GAMMA_ID) == 0)
        {
#if ENABLE_FLASH_PQ_DEBUG
            LOG(0, "Panel no=%d\n",
                vApiFlashPqGetGammaId((UINT8) GetCurrentPanelIndex()));
            LOG(0, "id=%s\n", aFlashPqGammaHead.bID);
            LOG(0, "script=%s\n", aFlashPqGammaHead.bScript);
            LOG(0, "date=%s\n", aFlashPqGammaHead.bDate);
#endif // #if ENABLE_FLASH_PQ_DEBUG
            LOG(0, "Gamma ID OK\n");
        }
        else
        {
            LOG(0, "Gamma ID No.%d NG\n", bi + vApiFlashPqGetGammaId((UINT8) GetCurrentPanelIndex()));
            x_mem_free(u1GammaBufPtr);
            return (SV_FALSE);
        }
    }

    // Parse the Gamma table from buffer: GammaX3 case
    if (DRVCUST_OptGet(eFlagFlashPqGammaX3))
    {
        for (bi = 0; bi < FLASH_PQ_GAMMA_CHANNEL_NUM; bi ++)
        {
            x_memcpy(&GAMMA_256_DARK[bi], 
                u1GammaBufPtr + TBL_HEAD_SIZE + u2GammaChlSize * bi, 
                u2GammaChlSize);
            x_memcpy(&GAMMA_256_MIDDLE[bi], 
                u1GammaBufPtr + u2GammaMdlSize + TBL_HEAD_SIZE + u2GammaChlSize * bi, 
                u2GammaChlSize);
            x_memcpy(&GAMMA_256_BRIGHT[bi], 
                u1GammaBufPtr + u2GammaMdlSize * 2 + TBL_HEAD_SIZE + u2GammaChlSize * bi, 
                u2GammaChlSize);

            // Calculate point 257 if bin file only store 256 points
            if (u2GammaChlSize == FLASH_PQ_GAMMA_CHANNEL_SIZE_256)
            {
                bApiGammaTopPt(GAMMA_256_DARK[bi]);
                bApiGammaTopPt(GAMMA_256_MIDDLE[bi]);
                bApiGammaTopPt(GAMMA_256_BRIGHT[bi]);
            }
        }
    }
    else        // Parse the Gamma table from buffer: GammaX1 case
    {
        for (bi = 0; bi < FLASH_PQ_GAMMA_CHANNEL_NUM; bi ++)
        {
            x_memcpy(&GAMMA_256_DARK[bi], 
                u1GammaBufPtr + TBL_HEAD_SIZE + u2GammaChlSize * bi, 
                u2GammaChlSize);

            // Calculate point 257 if bin file only store 256 points
            if (u2GammaChlSize == FLASH_PQ_GAMMA_CHANNEL_SIZE_256)
            {
                bApiGammaTopPt(GAMMA_256_DARK[bi]);
            }
        }

        x_memcpy(&GAMMA_256_MIDDLE, &GAMMA_256_DARK, FLASH_PQ_GAMMA_CHANNEL_SIZE_257 * FLASH_PQ_GAMMA_CHANNEL_NUM);
        x_memcpy(&GAMMA_256_BRIGHT, &GAMMA_256_DARK, FLASH_PQ_GAMMA_CHANNEL_SIZE_257 * FLASH_PQ_GAMMA_CHANNEL_NUM);

#if ENABLE_FLASH_PQ_DEBUG
        Printf("GAMMA_256_MIDDLE = \n");
        for (i = 0; i < u2GammaChlSize; i ++)
        {
            Printf("%2x ", GAMMA_256_MIDDLE[0][i]);
            if ((i+1) % 16 == 0)
            {
                Printf("\n");
            }
        }
#endif // #if ENABLE_FLASH_PQ_DEBUG
    }

    x_mem_free(u1GammaBufPtr);
    return (SV_TRUE);
}

UINT8 bApiFlashPqUpdateSCE(void)
{
    UINT8* u1SceBufPtr;
    UINT64 u8SceAddr;
#if ENABLE_FLASH_PQ_DEBUG
    UINT32 i;
#endif // #if ENABLE_FLASH_PQ_DEBUG
    UINT16 u2SceMdlSize = TBL_HEAD_SIZE + aFlashPqFileHead.wSizeOfSCETable;

    if (DRVCUST_OptGet(eFlagFlashPqEnableSCE) == 0)
    {
        LOG(0, "FlashPQ not support SCE\n");
        return (SV_FALSE);
    }

    // Calcuate start address of SCE table.
    u8SceAddr = u8CalcFlashPqOffset(
        TYPE_SCE_HEADER,
        vApiFlashPqGetSceId((UINT8) GetCurrentPanelIndex()),
        0,
        0,
        0);

    // SCE head base address must be 32-bit alignment.
    if ((u8SceAddr % 4) != 0)
    {
        LOG(0, "FlashPQ SCE address error!!!(%d)\n", u8SceAddr);
        return (SV_FALSE);
    }

    if ((u1SceBufPtr = (UINT8*)x_mem_alloc(u2SceMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ SCE memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u8SceAddr,
        (UINT32)u1SceBufPtr,
        (UINT32)u2SceMdlSize);

    x_memcpy(&aFlashPqSceHead, u1SceBufPtr, TBL_HEAD_SIZE);

#if ENABLE_FLASH_PQ_DEBUG
    LOG(0, "Panel no=%d\n",
        vApiFlashPqGetSceId((UINT8) GetCurrentPanelIndex()));
    LOG(0, "id=%s\n", aFlashPqSceHead.bID);
    LOG(0, "script=%s\n", aFlashPqSceHead.bScript);
    LOG(0, "date=%s\n", aFlashPqSceHead.bDate);
#endif // #if ENABLE_FLASH_PQ_DEBUG

    // Fix Klocwork warning
    if (aFlashPqFileHead.wSizeOfSCETable > 624)
    {
        LOG(0, "SCE size set in header error!\n", aFlashPqFileHead.wSizeOfSCETable);
        x_mem_free(u1SceBufPtr);
        return (SV_FALSE);
    }

    if (x_strcmp((CHAR *) (aFlashPqSceHead.bID), (CHAR *) SCE_ID) == 0)
    {
        LOG(0, "SCE ID OK\n");

        x_memcpy(&DEFAULT_SCE_TBL, (u1SceBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead.wSizeOfSCETable);
        x_memcpy(&BASE_SCE_TBL, &DEFAULT_SCE_TBL, aFlashPqFileHead.wSizeOfSCETable);
        x_mem_free(u1SceBufPtr);

        if(DRVCUST_OptGet(eFlagFlashPqSCEMax) == SV_OFF) // Normanl Mode
        {
#if ENABLE_FLASH_PQ_DEBUG
            LOG(0, "DEFAULT_SCE_TBL=\n");
            for (i = 0; i < aFlashPqFileHead.wSizeOfSCETable; i++)
            {
                Printf("DEFAULT_SCE_TBL[%d][%d] = %X \n", i/28, i%28, DEFAULT_SCE_TBL[i]);
            }
#endif // #if ENABLE_FLASH_PQ_DEBUG
            return (SV_TRUE);
        }
        else    //Use Base .Max to Calculate SCE
        {
            // Calcuate start address of SCE table.
            u8SceAddr = u8CalcFlashPqOffset(
                TYPE_SCE_HEADER,
                vApiFlashPqGetMaxSceId((UINT8) GetCurrentPanelIndex()),
                0,
                0,
                0);

            // SCE head base address must be 32-bit alignment.
            if ((u8SceAddr % 4) != 0)
            {
                LOG(0, "FlashPQ sce address error!!!(%d)\n", u8SceAddr);
                return (SV_FALSE);
            }

            if ((u1SceBufPtr = (UINT8*)x_mem_alloc(u2SceMdlSize)) == NULL)
            {
                LOG(0, "FlashPQ SCE memory request fail\n");
                return (SV_FALSE);
            }

            vApiFlashPqReadFlash(u8SceAddr,
                (UINT32)u1SceBufPtr,
                (UINT32)u2SceMdlSize);
            
            x_memcpy(&aFlashPqSceHead, u1SceBufPtr, TBL_HEAD_SIZE);

            if (x_strcmp((CHAR *) (aFlashPqSceHead.bID), (CHAR *) SCE_ID) == 0)
            {
                LOG(0, "SCE MAX ID OK\n");
                x_memcpy(&MAX_SCE_TBL, (u1SceBufPtr + TBL_HEAD_SIZE), aFlashPqFileHead.wSizeOfSCETable);
                x_mem_free(u1SceBufPtr);
                return (SV_TRUE);
            }
        }
    }

    LOG(0, "SCE ID NG\n");
    x_mem_free(u1SceBufPtr);
    return (SV_FALSE);
}

UINT8 bApiFlashPqQtyDftInit(void)
{
    UINT8* u1QtyPtr;
    UINT8* u1QtyBufPtr;
    UINT16 u2QtyMdlSize, i;
    UINT64 u8QtyAddr;
    int nQtyNumber = 0;
    char szEnding[TBL_END_SIZE+1];
#if ENABLE_FLASH_PQ_DEBUG
    UINT16 j;
#endif // #if ENABLE_FLASH_PQ_DEBUG

    if (DRVCUST_OptGet(eFlagFlashPqEnableQty) == 0)
    {
        LOG(0, "FlashPQ not support Qty\n");
        return SV_FALSE;
    }
    
    if (DRVCUST_OptGet(eFlagFlashPqEnableQtySmartPic) == 1)
    {
        LOG(0, "SmartPic enabled. Do not update Qty at bootup\n");
        return SV_FALSE;
    }

    for (i = 0; i < TBL_END_SIZE + 1; i ++)
    {
        szEnding[i] = '\0';
    }
    
    // Calculate start address of Qty table.
    if (vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()) >= aFlashPqFileHead.wNumOfQty)
    {
        LOG(0, "Qty Index Error!\n");
        return SV_FALSE;
    }
    
    u8QtyAddr = u8CalcFlashPqOffset(
        TYPE_QTY_HEADER,
        vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()),
        0,
        0,
        0);
    
    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
        LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
        return SV_FALSE;
    }
    
    u2QtyMdlSize = TBL_HEAD_SIZE + 
        aFlashPqFileHead.wSizeOfQtyTable * aFlashPqFileHead.bNumOfTiming;
    
    if ((u1QtyBufPtr = (UINT8*)x_mem_alloc(u2QtyMdlSize)) == NULL)
    {
        LOG(0, "FlashPQ Qty memory request fail\n");
        return (SV_FALSE);
    }

    vApiFlashPqReadFlash(u8QtyAddr,
        (UINT32)u1QtyBufPtr,
        (UINT32)u2QtyMdlSize);

    x_memcpy(&aFlashPqQtyHead, u1QtyBufPtr, TBL_HEAD_SIZE);

#if ENABLE_FLASH_PQ_DEBUG
        LOG(0, "Panel no=%d\n",
            vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()));
        LOG(0, "id=%s\n", aFlashPqQtyHead.bID);
        LOG(0, "script=%s\n", aFlashPqQtyHead.bScript);
        LOG(0, "date=%s\n", aFlashPqQtyHead.bDate);
        LOG(0, "QtyAddr=%d\n", u8QtyAddr);
//        LOG(0, "SmartPic=%d\n", bSmartPicIndex);
#endif // #if ENABLE_FLASH_PQ_DEBUG
    
    if (x_strcmp((CHAR *) (aFlashPqQtyHead.bID), (CHAR *) QTY_ID) == 0)
    {
        LOG(0, "QTY ID OK\n");

        // Check the quality table ending magic number..
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_ITEM_ENDING)
        {
            nQtyNumber = nCalcFlashPqQtyNumber();

            if (nQtyNumber == (-1))
            {
                LOG(0, "QTY item error!\n");
                x_mem_free(u1QtyBufPtr);
                return SV_FALSE;
            }
        }

        // Check the hash info inside the Qty header.
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_HASH)
        {
            if (!fgCheckFlashPqQtyHash())
            {
                LOG(0, "QTY hash error!\n");
                x_mem_free(u1QtyBufPtr);
                return SV_FALSE;
            }
        }

        // Check QTY table size.
        if (aFlashPqFileHead.wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
        {
            LOG(0, "FlashPQ qty table size error!!!(%d!=%d)\n",
                aFlashPqFileHead.wSizeOfQtyTable,
                CUSTOM_QTY_TBL_DIM);
            x_mem_free(u1QtyBufPtr);
            return SV_FALSE;
        }

        u1QtyPtr = u1QtyBufPtr + TBL_HEAD_SIZE;

        // The first TIMING_SCRIPT_SIZE bytes are description text.
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)
        {
            for (i = 0; i < aFlashPqFileHead.bNumOfTiming; i ++)
            {
                if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_ITEM_ENDING)
                {
                    x_memcpy(&(szEnding), 
                        u1QtyPtr + TIMING_SCRIPT_SIZE + nQtyNumber, 
                        TBL_END_SIZE);
                
                    if (x_strcmp((CHAR *) (szEnding), (CHAR *) QTY_ITEM_ENDING))
                    {
                        LOG(0, "Qty table[%d] ending error!\n",i);
                        x_mem_free(u1QtyBufPtr);
                        return (SV_FALSE);
                    }
                }
                
                x_memcpy(&(bCustomQtyTbl[i]), 
                    u1QtyPtr + TIMING_SCRIPT_SIZE, 
                    aFlashPqFileHead.wSizeOfQtyTable - TIMING_SCRIPT_SIZE);

                u1QtyPtr += aFlashPqFileHead.wSizeOfQtyTable;
            }
        }
        else
        {
            x_memcpy(&bCustomQtyTbl, 
                u1QtyBufPtr,
                aFlashPqFileHead.wSizeOfQtyTable * aFlashPqFileHead.bNumOfTiming);
        }

        if (aFlashPqFileHead.bRes & RES_SUPPORT_SAVE_QTY_HEADER)
        {
            if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)
            {
                x_memcpy(&wCustomQtyItem, 
                    u1QtyBufPtr,
                    CUSTOM_QTY_TBL_DIM - TIMING_SCRIPT_SIZE);
            }
            else
            {
                x_memcpy(&wCustomQtyItem, 
                    u1QtyBufPtr,
                    CUSTOM_QTY_TBL_DIM);
            }
        }

#if ENABLE_FLASH_PQ_DEBUG
        for (j = 0; j < aFlashPqFileHead.bNumOfTiming; j++)
        {
            Printf("\nTiming:%d\n", j);
            for (i = 0; i < aFlashPqFileHead.wSizeOfQtyTable; i++)
            {
                Printf("%02X ", (int)READ_CUSTOM_QTY_TBL(j,i));
                if (i % 16 == 15)
                {
                    Printf("\n");
                }
            }
        }
#endif // #if ENABLE_FLASH_PQ_DEBUG
        x_mem_free(u1QtyBufPtr);
        return (SV_TRUE);
    }
    else
    {
        LOG(0, "QTY ID NG\n");
        x_mem_free(u1QtyBufPtr);
        return (SV_FALSE);
    }
}

UINT8 bApiFlashPqUpdateQtyDft(void)
{
    UINT64 u8QtyAddr;
    UINT16 i;
    UINT8 bSmartPicIndex;
    int nQtyNumber = 0;
    char szEnding[TBL_END_SIZE+1];
#if ENABLE_FLASH_PQ_DEBUG
    UINT16 j;
#endif // #if ENABLE_FLASH_PQ_DEBUG

    if (_fgAutoSearch == TRUE)
    {
        LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
        return SV_FALSE;
    }

    if (DRVCUST_OptGet(eFlagFlashPqEnableQty) == 0)
    {
        LOG(0, "FlashPQ not support Qty\n");
        return SV_FALSE;
    }

    bSmartPicIndex = vApiFlashPqGetSmartPicId((UINT8) GetCurrentPanelIndex());
    if (bSmartPicIndex >= aFlashPqFileHead.bNumOfSmartPic)
    {
        LOG(0, "SmartPic Index Error!\n");
        return SV_FALSE;
    }

    for (i = 0; i < TBL_END_SIZE + 1; i ++)
    {
        szEnding[i] = '\0';
    }

    // Calculate start address of Qty table.
    if (vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()) >= aFlashPqFileHead.wNumOfQty)
    {
        LOG(0, "Qty Index Error!\n");
        return SV_FALSE;
    }

    u8QtyAddr = u8CalcFlashPqOffset(
        TYPE_QTY_HEADER,
        vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()),
        0,
        0,
        0);

    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
		LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
		return SV_FALSE;
    }

    vApiFlashPqReadFlash(u8QtyAddr,
        (UINT32)&aFlashPqQtyHead,
        (UINT32)sizeof(struct tagTblHead));

#if ENABLE_FLASH_PQ_DEBUG
    LOG(0, "Panel no=%d\n",
        vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()));
    LOG(0, "id=%s\n", aFlashPqQtyHead.bID);
    LOG(0, "script=%s\n", aFlashPqQtyHead.bScript);
    LOG(0, "date=%s\n", aFlashPqQtyHead.bDate);
    LOG(0, "QtyAddr=%d\n", u8QtyAddr);
    LOG(0, "SmartPic=%d\n", bSmartPicIndex);
#endif // #if ENABLE_FLASH_PQ_DEBUG

    if (x_strcmp((CHAR *) (aFlashPqQtyHead.bID), (CHAR *) QTY_ID) == 0)
    {
        LOG(0, "QTY ID OK\n");

        // Check the quality table ending magic number..
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_ITEM_ENDING)
        {
            nQtyNumber = nCalcFlashPqQtyNumber();

            if (nQtyNumber == (-1))
            {
                LOG(0, "QTY item error!\n");
                return SV_FALSE;
            }
        }

        // Check the hash info inside the Qty header.
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_HASH)
        {
            if (!fgCheckFlashPqQtyHash())
            {
                LOG(0, "QTY hash error!\n");
                return SV_FALSE;
            }
        }

        u8QtyAddr = u8CalcFlashPqOffset(
            TYPE_QTY_DESCRIPT,
            vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()),
            bSmartPicIndex,
            0,
            0);

        // Qty table base address must be 32-bit alignment.
        if ((u8QtyAddr % 4) != 0)
        {
            LOG(0, "FlashPQ qty table address error!!!(%d)\n", u8QtyAddr);
            return (SV_FALSE);
        }

        // Check QTY table size.
        if (aFlashPqFileHead.wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
        {
            LOG(0, "FlashPQ qty table size error!!!(%d!=%d)\n",
                aFlashPqFileHead.wSizeOfQtyTable,
                CUSTOM_QTY_TBL_DIM);
            return (SV_FALSE);
        }

        // The first TIMING_SCRIPT_SIZE bytes are description text.
        if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)
        {
            for (i = 0; i < aFlashPqFileHead.bNumOfTiming; i++)
            {
                if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_ITEM_ENDING)
                {
                    vApiFlashPqReadFlash((u8QtyAddr + TIMING_SCRIPT_SIZE + nQtyNumber),
                        (UINT32)&(szEnding),
                        (UINT32)TBL_END_SIZE);

                    if (x_strcmp((CHAR *) (szEnding), (CHAR *) QTY_ITEM_ENDING))
                    {
                        LOG(0, "Qty table[%d] ending error!\n",i);
                        return (SV_FALSE);
                    }
                }

                vApiFlashPqReadFlash((u8QtyAddr + TIMING_SCRIPT_SIZE),
                    (UINT32)&(bCustomQtyTbl[i]),
                    (UINT32)aFlashPqFileHead.wSizeOfQtyTable - TIMING_SCRIPT_SIZE);

                u8QtyAddr += aFlashPqFileHead.wSizeOfQtyTable;
            }
        }
        else
        {
            vApiFlashPqReadFlash(u8QtyAddr,
                (UINT32)&bCustomQtyTbl,
                (UINT32)aFlashPqFileHead.wSizeOfQtyTable * aFlashPqFileHead.bNumOfTiming);

            u8QtyAddr +=
                aFlashPqFileHead.wSizeOfQtyTable *
                aFlashPqFileHead.bNumOfTiming;
        }

        if (aFlashPqFileHead.bRes & RES_SUPPORT_SAVE_QTY_HEADER)
        {
            if (aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)
            {
                u8QtyAddr += TIMING_SCRIPT_SIZE;
                vApiFlashPqReadFlash(u8QtyAddr,
                    (UINT32)&wCustomQtyItem,
                    (UINT32)CUSTOM_QTY_TBL_DIM - TIMING_SCRIPT_SIZE);
            }
            else
            {
                vApiFlashPqReadFlash(u8QtyAddr,
                    (UINT32)&wCustomQtyItem,
                    (UINT32)CUSTOM_QTY_TBL_DIM);
            }
        }

#if ENABLE_FLASH_PQ_DEBUG
        for (j = 0; j < aFlashPqFileHead.bNumOfTiming; j++)
        {
            Printf("\nTiming:%d\n", j);
            for (i = 0; i < aFlashPqFileHead.wSizeOfQtyTable; i++)
            {
                Printf("%02X ", (int)READ_CUSTOM_QTY_TBL(j,i));
                if (i % 16 == 15)
                {
                    Printf("\n");
                }
            }
        }
#endif // #if ENABLE_FLASH_PQ_DEBUG
        return (SV_TRUE);
    }
    else
    {
        LOG(0, "QTY ID NG\n");
        return (SV_FALSE);
    }
}

UINT8 bApiFlashPqUpdateQtyMinMax(UINT8 u1VidPath)
{
    UINT64 u8QtyAddr;
    UINT16 i;
    UINT8 QtyMin[CUSTOM_QTY_TBL_DIM];
    UINT8 QtyMax[CUSTOM_QTY_TBL_DIM];
    UINT8 u1SrcTypTmg;

    if (_fgAutoSearch == TRUE)
    {
        LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
        return SV_FALSE;
    }

    if (((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_MAX_MIN) == 0) ||
        (DRVCUST_OptGet(eFlagFlashPqEnableQty) == 0) ||
        (DRVCUST_OptGet(eFlagFlashPqEnableQtyMaxMin) == 0) ||
        (bApiFlashPqVerifyHeader() == FALSE) )
    {
#if ENABLE_FLASH_PQ_DEBUG
        LOG(0, "Disable Qty Max/Min\n");
#endif // #if ENABLE_FLASH_PQ_DEBUG
        return SV_FALSE;
    }

    // Check QTY table size.
    if (aFlashPqFileHead.wSizeOfQtyTable != CUSTOM_QTY_TBL_DIM)
    {
		LOG(0, "FlashPQ qty table size error!!!(%d!=%d)\n",
			aFlashPqFileHead.wSizeOfQtyTable,
			CUSTOM_QTY_TBL_DIM);
		return (SV_FALSE);
    }

    for (i=0; i< CUSTOM_QTY_TBL_DIM; i++)
    {
        QtyMin[i] = 0;
        QtyMax[i] = 0;
    }

    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);

    // Transfrom the QtyIndex.
    // Find a match SourceTypeTiming in CustomQtyIdx[].
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);

    if (u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
    {
        return SV_FALSE;
    }

    // Calculate start address of Qty table.
    u8QtyAddr = u8CalcFlashPqOffset(
        TYPE_QTY_CONTENT_MIN,
        vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()),
        0,
        u1SrcTypTmg,
        0);

    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
		LOG(0, "FlashPQ qty address error!!!(%d)\n", u8QtyAddr);
		return (SV_FALSE);
    }
    vApiFlashPqReadFlash(u8QtyAddr,
        (UINT32)&QtyMin,
        (UINT32)aFlashPqFileHead.wSizeOfQtyTable -
            ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)?
            TIMING_SCRIPT_SIZE : 0));
    // Calculate start address of Qty table.
    u8QtyAddr = u8CalcFlashPqOffset(
        TYPE_QTY_CONTENT_MAX,
        vApiFlashPqGetQtyId((UINT8) GetCurrentPanelIndex()),
        0,
        u1SrcTypTmg,
        0);

    // Qty head base address must be 32-bit alignment.
    if ((u8QtyAddr % 4) != 0)
    {
		LOG(0, "FlashPQ qty table address error!!!(%d)\n", u8QtyAddr);
		return (SV_FALSE);
    }
    vApiFlashPqReadFlash(u8QtyAddr,
        (UINT32)&QtyMax,
        (UINT32)aFlashPqFileHead.wSizeOfQtyTable -
            ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)?
            TIMING_SCRIPT_SIZE : 0));

#if ENABLE_FLASH_PQ_DEBUG
    for (i=0; i<aFlashPqFileHead.wSizeOfQtyTable -
        ((aFlashPqFileHead.bRes & RES_SUPPORT_QTY_TIMING_DESCRIPT)? TIMING_SCRIPT_SIZE : 0);
        i++)
    {
        Printf("%02X  ", QtyMin[i]);
    }
#endif // #if ENABLE_FLASH_PQ_DEBUG

    for (i=0; i<QUALITY_MAX; i++)
    {
        if ((READ_DEFUALT_QTY_TBL_MIN(i) == 0xFF) &&
            (READ_DEFUALT_QTY_TBL_MAX(i) == 0xFF) &&
            (READ_DEFUALT_QTY_TBL_DFT(i) == 0xFF))
        {
            break;
        }

        if ((READ_DEFUALT_QTY_TBL_REF(i) != FROM_DFT) &&
            (READ_DEFUALT_QTY_TBL_REF(i) < CUSTOM_QTY_TBL_DIM))
        {
            WRITE_DEFUALT_QTY_TBL_MIN(i,
                QtyMin[READ_DEFUALT_QTY_TBL_REF(i)]);
            WRITE_DEFUALT_QTY_TBL_MAX(i,
                QtyMax[READ_DEFUALT_QTY_TBL_REF(i)]);
        }
#if ENABLE_FLASH_PQ_DEBUG
        Printf("Dft[%d]=%d, %d\n",
            i,
            READ_DEFUALT_QTY_TBL_MIN(i),
            READ_DEFUALT_QTY_TBL_MAX(i));
#endif
    }

    return SV_TRUE;
}

#endif // #ifdef SUPPORT_FLASH_PQ

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

/**
 * u4ApiSetUiVqItemDftValue
 *
 * @param void
 * @return void
 */
UINT32 u4ApiSetUiVqItemDftValue(UCHAR u1VidPath, UCHAR ucArg, INT16 i2Value)
{

    if (ucArg >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }

    ai2Arg[u1VidPath][ucArg] = i2Value;

    return SV_SUCCESS;
}
/**
 * u1ApiVidInSrcType2VisType
 *
 * @param void
 * @return void
 */
UINT8 u1ApiVidInSrcType2VisType(UINT8 u1SrcTypTmg)
{
LINT_SAVE_AND_DISABLE
    switch (u1SrcTypTmg)
    {
    case SV_VS_TUNER1:
        return (UINT8)EVDP_VIS_TUNER_ANALOG;

    case SV_VS_CVBS1:
    case SV_VS_CVBS2:
    case SV_VS_CVBS3:
        return (UINT8)EVDP_VIS_AVC_COMP_VIDEO;

    case SV_VS_S1:
    case SV_VS_S2:
    case SV_VS_S3:
        return (UINT8)EVDP_VIS_AVC_S_VIDEO;

    case SV_VS_YPbPr1:
    case SV_VS_YPbPr2:
    case SV_VS_YPbPr3:
        return (UINT8)EVDP_VIS_AVC_Y_PB_PR;

    case SV_VS_HDMI1:
    case SV_VS_HDMI2:
        return (UINT8)EVDP_VIS_AVC_HDMI;

    case SV_VS_VGA:
        return (UINT8)EVDP_VIS_AVC_VGA;

    case SV_VS_DVI:
        return (UINT8)EVDP_VIS_AVC_DVI;

    case SV_VS_DT1:     // DTV
//    case SV_VS_DT2:     // DTV
        return (UINT8)EVDP_VIS_TUNER_DIGITAL;

//    case SV_VS_SCART:
//    case SV_VS_SCART1:
//    case SV_VS_SCART2:
//    case SV_VS_SCART3:
//        return (UINT8)EVDP_VIS_AVC_SCART;

    default:
        return (UINT8)EVDP_VIS_MAX;
    }
LINT_RESTORE
}

/**
 * vApiArgType2UiVqItem
 *
 * @param void
 * @return void
 */
PE_UI_VIDEO_QUALITY_ITEM_T vApiArgType2UiVqItem(UCHAR ucArgType)
{
    switch (ucArgType)
    {
    case PE_ARG_BRIGHTNESS:            // brightness
        return EVDP_UVQI_BRIGHTNESS;

    case PE_ARG_CONTRAST:              // contrast
        return EVDP_UVQI_CONTRAST;

    case PE_ARG_HUE:                   // hue
        return EVDP_UVQI_HUE;

    case PE_ARG_SATURATION:            // saturation
        return EVDP_UVQI_SATURATION;

    case PE_ARG_CTI:                   // cti
        return EVDP_UVQI_CTI;

    case PE_ARG_LTI:
        return EVDP_UVQI_LTI;

    case PE_ARG_SHARPNESS:             // sharpness (v, h)
        return EVDP_UVQI_SHARPNESS;

    case PE_ARG_NR:                    // noise reduction
        return EVDP_UVQI_NR;

    case PE_ARG_ADAPTIVE_LUMA:         // adaptive luma
        return EVDP_UVQI_LUMA;
    case PE_ARG_BLACK_STRETCH:
        return EVDP_UVQI_BLACK_STRETCH;
    case PE_ARG_WHITE_STRETCH:
        return EVDP_UVQI_WHITE_STRETCH;
    case PE_ARG_COLOR_SUPPRESS:
        return EVDP_UVQI_COLOR_SUPPRESS;

    case PE_ARG_SCE:                   // second color enhance
        return EVDP_UVQI_FLESH_TONE;

    case PE_ARG_R_GAIN:                // color gain
    case PE_ARG_G_GAIN:
    case PE_ARG_B_GAIN:
        return EVDP_UVQI_COLOR_GAIN;

    case PE_ARG_R_OFFSET:              // color offset
    case PE_ARG_G_OFFSET:
    case PE_ARG_B_OFFSET:
        return EVDP_UVQI_COLOR_OFFSET;

    case PE_ARG_GAMMA:                 // demo mode
        return EVDP_UVQI_GAMMA;

    case PE_ARG_WHITE_PEAK_LMT:        // white peak limit
        return EVDP_UVQI_WHITE_PEAK_LMT;



    case PE_ARG_BACK_LIGHT_LVL:        // back light level
        return EVDP_UVQI_BACK_LIGHT_LVL;

    case PE_ARG_ADAPTIVE_BACK_LIGHT:   // adaptive back light (on/off)
        return EVDP_UVQI_ADAPTIVE_BACK_LIGHT;

    case PE_ARG_3D_NR:                 // 3D noise reduction
        return EVDP_UVQI_3D_NR;

    case PE_ARG_DEMO:                 // demo mode
        return EVDP_UVQI_DEMO;


    // New UI
    case PE_ARG_DI_FILM_MODE:
        return EVDP_UVQI_DI_FILM_MODE;

    case PE_ARG_DI_MA:
        return EVDP_UVQI_DI_MA;

    case PE_ARG_DI_EDGE_PRESERVE:
        return EVDP_UVQI_DI_EDGE_PRESERVE;

    case PE_ARG_MJC_MODE:
        return EVDP_UVQI_MJC_MODE;

    case PE_ARG_MJC_EFFECT:
        return EVDP_UVQI_MJC_EFFECT;

    case PE_ARG_MJC_DEMO:
        return EVDP_UVQI_MJC_DEMO;

    case PE_ARG_XVYCC:
        return EVDP_UVQI_XVYCC;

    case PE_ARG_WCG:
        return EVDP_UVQI_WCG;

    case PE_ARG_GAME_MODE:
        return EVDP_UVQI_GAME_MODE;

    case PE_ARG_BLUE_STRETCH:
        return EVDP_UVQI_BLUE_STRETCH;

    case PE_ARG_MPEG_NR:
        return EVDP_UVQI_MPEG_NR;

#if SUPPORT_SIX_COLOR_MANAGE 
		 case PE_ARG_SIX_COLOR:
			 return EVDP_UVQI_SIX_COLOR;
#endif

    default:
        return EVDP_UVQI_MAX;
    }
}

/**
 * u1ApiUiVqItem2ArgType
 *
 * @param void
 * @return void
 */
UINT8 u1ApiUiVqItem2ArgType(PE_UI_VIDEO_QUALITY_ITEM_T eUiVqItem)
{
    switch (eUiVqItem)
    {
    case EVDP_UVQI_BRIGHTNESS:              // brightness
        return (UINT8)PE_ARG_BRIGHTNESS;

    case EVDP_UVQI_CONTRAST:                // contrast
        return (UINT8)PE_ARG_CONTRAST;

    case EVDP_UVQI_HUE:                     // hue
        return (UINT8)PE_ARG_HUE;

    case EVDP_UVQI_SATURATION:              // saturation
        return (UINT8)PE_ARG_SATURATION;

    case EVDP_UVQI_CTI:                     // cti
        return (UINT8)PE_ARG_CTI;

   case EVDP_UVQI_SHARPNESS:               // sharpness (v, h)
        return (UINT8)PE_ARG_SHARPNESS;

   case EVDP_UVQI_LTI:               // sharpness (v, h)
        return (UINT8)PE_ARG_LTI;

    case EVDP_UVQI_COLOR_GAIN:              // color gain
        return (UINT8)PE_ARG_R_GAIN;

    case EVDP_UVQI_COLOR_OFFSET:            // color offset
        return (UINT8)PE_ARG_R_OFFSET;

    case EVDP_UVQI_NR:                      // noise reduction
        return (UINT8)PE_ARG_NR;

    case EVDP_UVQI_WHITE_PEAK_LMT:          // white peak limit
        return (UINT8)PE_ARG_WHITE_PEAK_LMT;

    case EVDP_UVQI_FLESH_TONE:              // second color enhance
        return (UINT8)PE_ARG_SCE;

    case EVDP_UVQI_LUMA:                    // adaptive luma
        return (UINT8)PE_ARG_ADAPTIVE_LUMA;

    case EVDP_UVQI_BLACK_STRETCH:
        return (UINT8)PE_ARG_BLACK_STRETCH;

    case EVDP_UVQI_WHITE_STRETCH:
        return (UINT8)PE_ARG_WHITE_STRETCH;

    case EVDP_UVQI_BACK_LIGHT_LVL:          // back light level
        return (UINT8)PE_ARG_BACK_LIGHT_LVL;

    case EVDP_UVQI_ADAPTIVE_BACK_LIGHT:     // adaptive back light
        return (UINT8)PE_ARG_ADAPTIVE_BACK_LIGHT;

    case EVDP_UVQI_3D_NR:                   // 3D noise reduction
        return (UINT8)PE_ARG_3D_NR;

    case EVDP_UVQI_DEMO:                   // demo mode
        return (UINT8)PE_ARG_DEMO;

    case EVDP_UVQI_COLOR_SUPPRESS:
        return (UINT8)PE_ARG_COLOR_SUPPRESS;

    case EVDP_UVQI_GAMMA:
        return (UINT8)PE_ARG_GAMMA;

    // New UI
    case EVDP_UVQI_DI_FILM_MODE:
        return (UINT8)PE_ARG_DI_FILM_MODE;

    case EVDP_UVQI_DI_MA:
        return (UINT8)PE_ARG_DI_MA;

    case EVDP_UVQI_DI_EDGE_PRESERVE:
        return (UINT8)PE_ARG_DI_EDGE_PRESERVE;

    case EVDP_UVQI_MJC_MODE:
        return (UINT8)PE_ARG_MJC_MODE;

    case EVDP_UVQI_MJC_EFFECT:
        return (UINT8)PE_ARG_MJC_EFFECT;

    case EVDP_UVQI_MJC_DEMO:
        return (UINT8)PE_ARG_MJC_DEMO;

    case EVDP_UVQI_XVYCC:
        return (UINT8)PE_ARG_XVYCC;

    case EVDP_UVQI_WCG:
        return (UINT8)PE_ARG_WCG;

    case EVDP_UVQI_GAME_MODE:
        return (UINT8)PE_ARG_GAME_MODE;

    case EVDP_UVQI_BLUE_STRETCH:
        return (UINT8)PE_ARG_BLUE_STRETCH;

    case EVDP_UVQI_MPEG_NR:
        return (UINT8)PE_ARG_MPEG_NR;
		
#if SUPPORT_SIX_COLOR_MANAGE  
    case EVDP_UVQI_SIX_COLOR:
        return (UINT8)PE_ARG_SIX_COLOR;
#endif

    default:
        return (UINT8)PE_ARG_NS;
    }
}

/**
 * VDP get min, max, default, and current values of UI video quality items.
 *
 * @param void
 * @return void
 */
/*lint -save -e818 -e830 */
UINT32 PE_GetUiVqItemMinMaxDftCur(UINT8 u1VidPath, UCHAR ucArgType,
    INT16 *pi2Min, INT16 *pi2Max, INT16 *pi2Dft, INT16 *pi2Cur)
{
    UINT32 i4Ret = SV_FAIL;
    INT32 i4Offset;
    UINT8 u1NptvSrcType, u1VdpVisType, u1UiVqItem;
    PE_UI_VIDEO_QUALITY_ITEM_T eUiVqItem;
    PE_UI_VQ_MIN_MAX_DFT_T *prVqTbl;

    if (ucArgType >= (UCHAR) PE_ARG_NS)
    {
        return SV_FAIL;
    }

    if ((pi2Min == NULL) || (pi2Max == NULL) ||
        (pi2Dft == NULL) || (pi2Cur == NULL))
    {
        return SV_FAIL;
    }


    // get current video input source type
    u1NptvSrcType = bApiVideoGetSrcType(u1VidPath);
    u1VdpVisType = u1ApiVidInSrcType2VisType(u1NptvSrcType);

    eUiVqItem = vApiArgType2UiVqItem(ucArgType);
    if (eUiVqItem >= EVDP_UVQI_MAX)
    {
        return SV_FAIL;
    }

    u1UiVqItem = (UINT8)eUiVqItem;

    if (u1VdpVisType >= (UINT8)EVDP_VIS_MAX)
    {
        u1VdpVisType = (UINT8)EVDP_VIS_TUNER_DIGITAL;
        prVqTbl = &aUiVqMinMaxDft[u1VdpVisType][u1UiVqItem];
    }
    else
    {
        prVqTbl = &aUiVqMinMaxDft[u1VdpVisType][u1UiVqItem];
    }

    i4Offset = ((prVqTbl->i4Min < 0) ? prVqTbl->i4Min : 0);

    *pi2Min = (INT16)(prVqTbl->i4Min - i4Offset);
    *pi2Max = (INT16)(prVqTbl->i4Max - i4Offset);
    *pi2Dft = (INT16)(prVqTbl->i4Dft - i4Offset);

    i4Ret = PE_GetArg(u1VidPath, (PE_ARG_TYPE)ucArgType, pi2Cur);

    return i4Ret;
}
/*lint -restore */

/**
 * VDP set min/max/default values of UI video quality items.
 *
 * @param void
 * @return void
 */
/*lint -save -e613 -e661 -e662 -e818 -e830 -e831 */
void PE_SetUiVqMinMaxDft(UINT8 u1VdpVisType, UINT8 u1UiVqItem,
    PE_UI_VQ_MIN_MAX_DFT_T *prUiVqMinMaxDft)
{
    // check args
    VERIFY(u1VdpVisType < (UINT8)EVDP_VIS_MAX);
    VERIFY(u1UiVqItem < (UINT8)EVDP_UVQI_MAX);
    VERIFY(prUiVqMinMaxDft != NULL);

    if ((u1VdpVisType < (UINT8)EVDP_VIS_MAX) &&
        (u1UiVqItem < (UINT8)EVDP_UVQI_MAX) &&
        (prUiVqMinMaxDft != NULL))
    {
        aUiVqMinMaxDft[u1VdpVisType][u1UiVqItem].i4Min = prUiVqMinMaxDft->i4Min;
        aUiVqMinMaxDft[u1VdpVisType][u1UiVqItem].i4Max = prUiVqMinMaxDft->i4Max;
        aUiVqMinMaxDft[u1VdpVisType][u1UiVqItem].i4Dft = prUiVqMinMaxDft->i4Dft;
    }
    else
    {
        Printf("aUiVqMinMaxDft range index error! %d,%d,%d\n",
            u1VdpVisType, u1UiVqItem, prUiVqMinMaxDft);
    }
}
/*lint -restore */


/**
 * VDP set min/max/default values of all UI video quality items.
 *
 * @param void
 * @return void
 */
void PE_SetAllUiVqItemDftValues(void)
{
    UCHAR ucVidPath, ucArgType;
    INT16 i2Default;
    INT32 i4UiVqItem, i4Offset, i4VdpVisType;
    PE_UI_VIDEO_QUALITY_ITEM_T eUiVqItem;
    PE_UI_VQ_MIN_MAX_DFT_T *prVqTbl;

    // Use cvbs as default
    i4VdpVisType = (INT32)EVDP_VIS_AVC_COMP_VIDEO;

    for (ucVidPath = 0; ucVidPath < VDP_NS; ucVidPath++)
    {
        for (i4UiVqItem = 0; i4UiVqItem < ((INT32)EVDP_UVQI_MAX); i4UiVqItem++)
        {
            prVqTbl = &aUiVqMinMaxDft[i4VdpVisType][i4UiVqItem];
            i4Offset = ((prVqTbl->i4Min < 0) ? prVqTbl->i4Min : 0);
            i2Default = (INT16)(prVqTbl->i4Dft - i4Offset);

            if (i4UiVqItem == (INT32)EVDP_UVQI_COLOR_GAIN)
            {
                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_R_GAIN,
                            i2Default) == SV_SUCCESS);

                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_G_GAIN,
                            i2Default) == SV_SUCCESS);

                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_B_GAIN,
                            i2Default) == SV_SUCCESS);
            }
            else if (i4UiVqItem == (INT32)EVDP_UVQI_COLOR_OFFSET)
            {
                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_R_OFFSET,
                            i2Default) == SV_SUCCESS);

                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_G_OFFSET,
                            i2Default) == SV_SUCCESS);

                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, (UCHAR) PE_ARG_B_OFFSET,
                            i2Default) == SV_SUCCESS);
            }
            else
            {
                eUiVqItem = (PE_UI_VIDEO_QUALITY_ITEM_T)i4UiVqItem;
                ucArgType = u1ApiUiVqItem2ArgType(eUiVqItem);

                VERIFY(u4ApiSetUiVqItemDftValue(ucVidPath, ucArgType,
                            i2Default) == SV_SUCCESS);
            }
        }   // ~for
    }   // ~for
}

/**
 * VDP set video plane argument.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucArg specify the color item
 * @param i2Value specify the value of the color item
 * @return SV_FAIL or SV_SUCCESS
 */
UINT32 PE_SetArg(UCHAR ucVdpId, UCHAR ucArg, INT16 i2Value)
{
    if ((ucVdpId >= (UCHAR) VDP_NS) || (ucArg >= (UCHAR) PE_ARG_NS))
    {
        return SV_FAIL;
    }

    ai2Arg[ucVdpId][ucArg] = i2Value;

    // color process entry point
    if (bApiVideoProc(ucVdpId, ucArg) != SV_TRUE)
    {
        return SV_FAIL;
    }

    return SV_SUCCESS;
}


#if SUPPORT_SIX_COLOR_MANAGE 
UINT32 PE_SetSixColorArg(UCHAR ucVdpId, UINT8 bColor, UINT8 bHSYType, INT16 i2Value)
{
    if ((ucVdpId >= (UCHAR) VDP_NS) || (bColor >= (UCHAR) BASE_COLOR_MAX))
    {
        return SV_FAIL;
    }
	
	gSixBaseColor.bColor = bColor;
	gSixBaseColor.bHSY = bHSYType;
	gSixBaseColor.u2Offset = i2Value;
	
    // color process entry point
    if (bApiVideoProc(ucVdpId, PE_ARG_SIX_COLOR) != SV_TRUE)
    {
        return SV_FAIL;
    }

    return SV_SUCCESS;
}
#endif


/**
 * VDP query video plane argument.
 *
 * @param ucVdpId specify the video plane id.
 * @param ucArg
 * @param pi2Value
 * @return SV_FAIL or SV_SUCCESS.
 */
//-----------------------------------------------------------------------------
UINT32 PE_GetArg(UCHAR ucVdpId, UCHAR ucArg, INT16* pi2Value)
{
    if (pi2Value==NULL)
    {
        return SV_FAIL;
    }

    if ((ucVdpId >= (UCHAR) VDP_NS) || (ucArg >= (UCHAR) PE_ARG_NS))
    {
        return SV_FAIL;
    }

    *pi2Value = ai2Arg[ucVdpId][ucArg];

    return SV_SUCCESS;
}

UINT8 bApiGetPeUiDebugMode(void)
{
    return bDrvGetPeUiDebugMode();
}

void vApiSetPeUiDebugMode(UINT8 bOnOff)
{
    vDrvSetPeUiDebugMode(bOnOff);
}

/**
 * @brief Interface between MW_if and NPTV Hal
 * @param u1VidPath SV_VP_MAIN/SV_VP_PIP
 * @param u2QtyItem PE_ARG_xxx
 */
UINT8 bApiVideoProc(UINT8 u1VidPath, UINT8 u1UiItem)
{
    INT16 i2Min, i2Max, i2Dft, i2Cur;   //UI min,max,def, current
    UINT8 u1SrcTypTmg;
    UINT16 u2QtyItem;
    UINT32 u4Tmp;

    if ((_u4BypassNptvMloop & BYPASS_COLOR_PROC) != 0)
    {
        return SV_FALSE;
    }

    if ((bGetSignalType(u1VidPath) == SV_ST_TV) && (_fgAutoSearch == TRUE))
    {
        LOG(6, "Search Chaneel, Do Not Update PQ Setting.\n");
        return SV_FALSE;
    }

    if (PE_GetUiVqItemMinMaxDftCur(u1VidPath, u1UiItem,
                                    &i2Min, &i2Max, &i2Dft,
                                    &i2Cur) == SV_FAIL)
    {
        LOG(6, "PE_GetUiVqItemMinMaxDftCur %d FAIL\n", u1UiItem);
        return SV_FALSE;
    }

    if (bApiGetPeUiDebugMode() == SV_ON)
    {
        if (u1UiItem <= PE_ARG_NS)
            LOG(0, "PE UI: %s, Path(%d), Min(%d), Max(%d), Dft(%d), Cur(%d)\n", 
                ucPeUiType[u1UiItem], u1VidPath, i2Min, i2Max, i2Dft, i2Cur);
    }

    LOG(5, "bApiVideoProc %d %d %d %d %d %d\n", u1VidPath, u1UiItem, i2Min, i2Max, i2Dft, i2Cur);

    u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1VidPath);
    LOG(6, "bDrvVideoGetSourceTypeTiming(%d)=0x%X\n",
        u1VidPath, bDrvVideoGetSourceTypeTiming(u1VidPath));

    // While input path has no signal, only backlight relative functions can work.
    if ((u1SrcTypTmg == SOURCE_TYPE_TIMING_MAX)
        && (u1UiItem != PE_ARG_BACK_LIGHT_LVL)
        && (u1UiItem != PE_ARG_ADAPTIVE_BACK_LIGHT)
        && (u1UiItem != PE_ARG_GAMMA)
        && (u1UiItem != PE_ARG_R_GAIN)
        && (u1UiItem != PE_ARG_G_GAIN)
        && (u1UiItem != PE_ARG_B_GAIN)
        && (u1UiItem != PE_ARG_R_OFFSET)
        && (u1UiItem != PE_ARG_G_OFFSET)
        && (u1UiItem != PE_ARG_B_OFFSET)
    #ifdef DRV_SUPPORT_EXTMJC
        && (u1UiItem != PE_ARG_MJC_EFFECT)
        && (u1UiItem != PE_ARG_MJC_MODE)
        && (u1UiItem != PE_ARG_MJC_DEMO)
    #endif
        )
    {
        return (SV_TRUE);
    }

#ifdef THE_3RD_PARTY_SW_SUPPORT
    // These function controled by 3rd Party SW directly
    switch (u1UiItem)
    {
        case PE_ARG_BLUE_STRETCH:
   	 case PE_ARG_BLACK_STRETCH:
   	 case PE_ARG_WHITE_STRETCH:
        case PE_ARG_3D_NR:
        case PE_ARG_NR:
    	case PE_ARG_SCE:
    	case PE_ARG_GAMMA:
        case PE_ARG_ADAPTIVE_LUMA:
//        case PE_ARG_BACK_LIGHT_LVL:
        case PE_ARG_MPEG_NR:
            return SV_TRUE;
        case PE_ARG_ADAPTIVE_BACK_LIGHT:
    		if (!((bIsScalerInput444(SV_VP_MAIN) == SV_TRUE) && ( _rMChannel.bIsChannelOn==SV_ON)))
    		{
    			vApiVideoMapAutoBacklight((UINT8)i2Cur);
    		}
            return SV_TRUE;
        default:
            break;
    }

#endif


    // Transfrom the QtyIndex.
    // Find a match SourceTypeTiming in CustomQtyIdx[].
    u1SrcTypTmg = bApiGetCustomSourceTypeTiming(u1SrcTypTmg);

    switch (u1UiItem)
    {
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // The following items take i2Cur == 0 as turn off.
    // And their UI_min should reset as 1 for currect linear mapping.
    case PE_ARG_CTI:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        if (i2Min == 0)
        {
            i2Min = 1;
        }

        for (u2QtyItem = QUALITY_CTI_BEGIN; u2QtyItem <= QUALITY_CTI_END;
             u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }
        if (i2Cur == 0)
        {
            vHalVideoCTIOnOff(u1VidPath, SV_OFF);
        }
        else
        {
            vHalVideoCTIOnOff(u1VidPath, SV_ON);
            vHalVideoCTIParam(u1VidPath);
        }
        break;
    case PE_ARG_CDS:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        if (i2Min == 0)
        {
            i2Min = 1;
        }

        for (u2QtyItem = QUALITY_CDS_BEGIN; u2QtyItem <= QUALITY_CDS_END;
             u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }

        if (i2Cur == 0)
        {
            vHalVideoCDSOnOff(SV_OFF);
        }
        else
        {
            vHalVideoCDSOnOff(SV_ON);
            vHalVideoCDSParam();
        }
        break;
    case PE_ARG_LTI:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        if (i2Min == 0)
        {
            i2Min = 1;
        }

        for (u2QtyItem = QUALITY_LTI_BEGIN; u2QtyItem <= QUALITY_LTI_END;
             u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }

        if (i2Cur == 0)
        {
            vHalVideoLTIOnOff(u1VidPath, SV_OFF);
        }
        else
        {
            vHalVideoLTIOnOff(u1VidPath, SV_ON);
            vHalVideoLTIParam(u1VidPath);
        }
        break;
    case PE_ARG_3D_NR:

        if (bDrvNRGetCurrentPath() != u1VidPath)
        {
            vDrvNRSetSystem();
            break;
        }
        // Reset UI_min as 1.
        if (i2Min == 0)
        {
            i2Min = 1;
        }

        if (i2Cur == 0)
        {
            vApiVideo3DNROnOff(SV_OFF);
#if ANM_ENABLE
            vDrvSetAutoNROnOff(SV_OFF);// Turn On Off ANR
#endif
        }
        else
        {
#if ANM_ENABLE
            // Auto NR default Initial setting
            vDrvSetAutoNROnOff(i2Cur == 4);// Turn On Off ANR
            if(i2Cur == 4)
            {
                i2Cur = (INT16)DRVCUST_AutoNRDefSet();
            }

            if (i2Max == 4)
            {
                i2Max = 3;
            }
#endif

            for (u2QtyItem = QUALITY_3DNR_BEGIN;
                 u2QtyItem <= QUALITY_3DNR_END; u2QtyItem++)
            {
                wWriteQualityTable(u2QtyItem,
                                   bApiQtyMapping(u1SrcTypTmg, u2QtyItem,
                                                  &i2Min, &i2Max, &i2Dft,
                                                  &i2Cur));
            }



#if ANM_ENABLE
            for (u2QtyItem = QUALITY_ANR_BEGIN;
                 u2QtyItem <= QUALITY_ANR_END; u2QtyItem++)
            {
                wWriteQualityTable(u2QtyItem,
                                   bApiQtyMapping(u1SrcTypTmg, u2QtyItem,
                                                  &i2Min, &i2Max, &i2Dft,
                                                  &i2Cur));
            }
            vDrvANRSetSWRegister();
#endif
            //vHalVideoNRTIIROnOff(SV_ON);
            vHalVideo3DNRParam();
            vApiVideo3DNROnOff(SV_ON);


        }

        vDrvNRSetSystem();
        break;
    case PE_ARG_NR:

        if(DRVCUST_OptGet(eFlagBind2DNRWithMpegNR))
        {
            break;
        }

        if (bDrvNRGetCurrentPath() != u1VidPath)
        {
            break;
        }
        // Reset UI_min as 1.
        if (i2Min == 0)
        {
            i2Min = 1;
        }


        for (u2QtyItem = QUALITY_2DNR_BEGIN;
             u2QtyItem <= QUALITY_2DNR_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem,
                                              &i2Min, &i2Max, &i2Dft,
                                              &i2Cur));
        }
        vHalVideo2DNRParam(i2Cur);   // if i2Cur == 0, set blending = 0.

        vDrvNRSetSystem();
        break;

    case PE_ARG_MPEG_NR:
        if (bDrvNRGetCurrentPath() != u1VidPath)
        {
            break;
        }
        // Reset UI_min as 1.
        if (i2Min == 0)
        {
            i2Min = 1;
        }

        for (u2QtyItem = QUALITY_MPEGNR_BEGIN;
             u2QtyItem <= QUALITY_MPEGNR_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem,
                                              &i2Min, &i2Max, &i2Dft,
                                              &i2Cur));
        }
        vHalVideoMPEGNRParam(i2Cur);    // if i2Cur == 0, set blending = 0.

        vDrvNRSetSystem();
        break;

    case PE_ARG_SCE:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        for (u2QtyItem = QUALITY_SCE_BEGIN; u2QtyItem <= QUALITY_SCE_END;
             u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }


#ifdef SUPPORT_FLASH_PQ
        vApiFlashPqSetSceId(i2Cur);   // SCE set flash PQ Index
        bApiFlashPqUpdateSCE();      // SCE load table
#endif // #ifdef SUPPORT_FLASH_PQ

        vDrvSetSCELevel(i2Cur,SV_ON);
        vHalVideoSCEOnOff(u1VidPath, i2Cur!=SV_OFF);    //i2Cur==0 means turn off.
        break;
		
#if SUPPORT_SIX_COLOR_MANAGE			 
	case PE_ARG_SIX_COLOR:
		
		if(gSixBaseColor.bHSY == COLOR_Y)
		{
			gSixBaseColor.bPhaseBegin = aSixBaseColorRNG[gSixBaseColor.bColor].bPartialYStartPhase;
			gSixBaseColor.bPhaseEnd = aSixBaseColorRNG[gSixBaseColor.bColor].bPartialYEndPhase;
		}
		else 
		{
			gSixBaseColor.bPhaseBegin = aSixBaseColorRNG[gSixBaseColor.bColor].bStartPhase;
			gSixBaseColor.bPhaseEnd = aSixBaseColorRNG[gSixBaseColor.bColor].bEndPhase; 
		}
		
		vApiRegisterVideoEvent(u1UiItem, u1VidPath, SV_ON);
		break;
#endif

    case PE_ARG_ADAPTIVE_LUMA:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        for (u2QtyItem = QUALITY_ADAPTIVE_LUMA_AL_BEGIN;
             u2QtyItem <= QUALITY_ADAPTIVE_LUMA_AL_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }

        if(i2Cur==0)
        {
            wWriteQualityTable(QUALITY_AL_GAIN,0x0);
            wWriteQualityTable(QUALITY_AL_OFFSET,0x0);
        }

        vHalVideoAdaptiveLumaSetDefault();
#ifdef CC_MT5363   //Turn on or turn off Adaptive and LCE in Vsync. shijiang.feng 2009/07/17
        vApiRegisterVideoEvent(PE_ARG_ADAPTIVE_LUMA, u1VidPath, i2Cur);
#else
        vHalVideoAdaptiveLumaOnOff(u1VidPath, i2Cur!=0);   //i2Cur==0 means turn off.
#endif
        vDrvChromaBoostParam();

        break;
    case PE_ARG_BLACK_STRETCH:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        for (u2QtyItem = QUALITY_ADAPTIVE_LUMA_BLS_BEGIN;
             u2QtyItem <= QUALITY_ADAPTIVE_LUMA_BLS_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }

        //wWriteQualityTable(QUALITY_BWS_BLACK_ON_1_OFF_0,i2Cur!=SV_OFF);

        if(i2Cur==0)
        {
            wWriteQualityTable(QUALITY_BWS_BLACK_GAIN,0x80);
            wWriteQualityTable(QUALITY_BWS_BLACK_OFFSET,0x0);
            wWriteQualityTable(QUALITY_BWS_BLACK_LIMIT,0x0);
            wWriteQualityTable(QUALITY_DYNBS_ONOFF,0x0);
        }

        vHalVideoAdaptiveLumaSetDefault();

        break;

    case PE_ARG_WHITE_STRETCH:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        for (u2QtyItem = QUALITY_ADAPTIVE_LUMA_WLS_BEGIN;
             u2QtyItem <= QUALITY_ADAPTIVE_LUMA_WLS_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }

         if(i2Cur==0)
         {
             wWriteQualityTable(QUALITY_BWS_WHITE_GAIN,0x80);
             wWriteQualityTable(QUALITY_BWS_WHITE_OFFSET,0x0);
             wWriteQualityTable(QUALITY_BWS_WHITE_LIMIT,0x0);

         }

        vHalVideoAdaptiveLumaSetDefault();

        break;
    case PE_ARG_BLUE_STRETCH:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        for (u2QtyItem = QUALITY_BLUE_STRETCH_BEGIN;
             u2QtyItem <= QUALITY_BLUE_STRETCH_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
        }
        vHalVideoBlueStretchSetDefault();
        vHalVideoBlueStretchOnOff(i2Cur!=SV_OFF);   //i2Cur==0 means turn off.
        break;

    case PE_ARG_ADAPTIVE_BACK_LIGHT:
        vHalVideoAdaptiveBackLightParam(i2Cur); //i2Cur==0 means turn off.
        vHalVideoSetAdaptiveBacklightMode(i2Cur); //i2Cur==0 means turn off.
        break;
    case PE_ARG_WHITE_PEAK_LMT:
        vHalVideoWhitePeakLimit(i2Cur); //i2Cur==0 means turn off.
        break;

    case PE_ARG_GAMMA:

        //Used for Gamma Calculating , No use if no Gamma Remap
        for (u2QtyItem  = QUALITY_GAMMA_CURVE_SEL;
             u2QtyItem <= QUALITY_GAMMA_CURVE_SEL; u2QtyItem++)
            {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem, &i2Min,
                                              &i2Max, &i2Dft, &i2Cur));
            }

                #ifdef SUPPORT_FLASH_PQ
                vApiFlashPqSetGammaId(i2Cur);
                #endif

        if(DRVCUST_OptGet(eNVMGamma))
        {
            vDrvSetGammaRemap(SV_ON);
        }else{
                vApiRegisterVideoEvent(PE_ARG_GAMMA, i2Cur, SV_ON);
            }

        break;
    case PE_ARG_SHARPNESS_ON_OFF:
        vHalVideoSharpnessOnOff(u1VidPath, i2Cur);
        break;

        ///////////////////////////////////////////////////////////////////////////////////////////////
        // The following items do not have turn off function.
    case PE_ARG_SHARPNESS:
        for (u2QtyItem = QUALITY_SHARP_BEGIN;
             u2QtyItem <= QUALITY_SHARP_END; u2QtyItem++)
        {
            wWriteQualityTable(u2QtyItem,
                               bApiQtyMapping(u1SrcTypTmg, u2QtyItem,
                                              &i2Min, &i2Max, &i2Dft,
                                              &i2Cur));
        }

        vHalVideoLTIOnOff(u1VidPath, SV_ON);
        vHalVideoSharpnessOnOff(u1VidPath, SV_ON);

        vHalVideoLTIParam(u1VidPath);
        vHalVideoSharpnessParam(u1VidPath);
        break;
    case PE_ARG_BRIGHTNESS:
        wWriteQualityTable(QUALITY_BRIGHTNESS,
                           bApiQtyMapping(u1SrcTypTmg, QUALITY_BRIGHTNESS,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur));
#ifdef THE_3RD_PARTY_SW_SUPPORT
        if(bIsScalerInput444(SV_VP_MAIN) == SV_TRUE)
        {
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoBrightness(u1VidPath,
                                wReadQualityTable(QUALITY_BRIGHTNESS));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_BRIGHTNESS, u1VidPath, SV_ON);
        }
        }
        break;
#else
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoBrightness(u1VidPath,
                                wReadQualityTable(QUALITY_BRIGHTNESS));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_BRIGHTNESS, u1VidPath, SV_ON);
        }
        break;
#endif

    case PE_ARG_CONTRAST:
        wWriteQualityTable(QUALITY_CONTRAST,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_CONTRAST,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur)));
        wWriteQualityTable(QUALITY_CONTRAST_HIBYTE,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_CONTRAST,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur)>>8));
#ifdef THE_3RD_PARTY_SW_SUPPORT
        ai2Arg[SV_VP_MAIN][PE_ARG_BACK_LIGHT_LVL] = i2Cur;
        if ((bIsScalerInput444(SV_VP_MAIN) == SV_TRUE) 
        	//&& (bGetSignalType(SV_VP_MAIN) == SV_ST_VGA)
        	)
        {
            bApiVideoProc(u1VidPath, PE_ARG_BACK_LIGHT_LVL);
        }
        break;
#endif
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoContrast(u1VidPath,
                                  wReadQualityTable(QUALITY_CONTRAST)|
                                  ((UINT16)wReadQualityTable(QUALITY_CONTRAST_HIBYTE))<<8);
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_CONTRAST, u1VidPath, SV_ON);
        }
        if(DRVCUST_OptGet(eFlagBindContrastSaturation))
        {
            bApiVideoProc(u1VidPath,PE_ARG_SATURATION);
        }
        break;

    case PE_ARG_HUE:
        wWriteQualityTable(QUALITY_HUE,
                           bApiQtyMapping(u1SrcTypTmg, QUALITY_HUE,
                                          &i2Min, &i2Max, &i2Dft,
                                          &i2Cur));

        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoHue(u1VidPath, wReadQualityTable(QUALITY_HUE));
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_HUE, u1VidPath, SV_ON);
        }
        break;

    case PE_ARG_SATURATION:
        u4Tmp=bApiQtyMapping(u1SrcTypTmg, QUALITY_SATURATION,&i2Min, &i2Max, &i2Dft, &i2Cur);

        if(DRVCUST_OptGet(eFlagBindContrastSaturation))
        {
            if(bApiGetQtyDft(QUALITY_CONTRAST, u1SrcTypTmg)|((UINT16)bApiGetQtyDft(QUALITY_CONTRAST_HIBYTE, u1SrcTypTmg) <<8))
            {
                u4Tmp = u4Tmp *
                (wReadQualityTable(QUALITY_CONTRAST)| ((UINT16)wReadQualityTable(QUALITY_CONTRAST_HIBYTE))<<8)/
                (bApiGetQtyDft(QUALITY_CONTRAST, u1SrcTypTmg)|((UINT16)bApiGetQtyDft(QUALITY_CONTRAST_HIBYTE, u1SrcTypTmg) <<8));
            }
        }

        wWriteQualityTable(QUALITY_SATURATION,(UINT8)u4Tmp);
        wWriteQualityTable(QUALITY_SATURATION_HIBYTE,u4Tmp>>8);

#ifdef THE_3RD_PARTY_SW_SUPPORT
        break;
#endif
        if (_fgCallFromIsr == SV_ON)
        {
            vHalVideoSaturation(u1VidPath,
                    wReadQualityTable(QUALITY_SATURATION)|
                    ((UINT16)wReadQualityTable(QUALITY_SATURATION_HIBYTE))<<8);
        }
        else
        {
            vApiRegisterVideoEvent(PE_ARG_SATURATION, u1VidPath, SV_ON);
        }
        break;

    case PE_ARG_DI_FILM_MODE:
        if(i2Cur==SV_OFF)
        {
            vDrvDIFilmModeOnOff(u1VidPath, SV_OFF);
        }
        else
        {
            vDrvDIFilmModeOnOff(u1VidPath, SV_ON);

            //Slow Motion (1) Map to Aggressive ,Big Motion (2) Map to Defensive
            vDrvDISetQualityMode(E_DI_ENGINE_FILM, i2Cur==SV_CN_MOVIE ?
                E_DI_QUALITY_AGGRESSIVE : E_DI_QUALITY_DEFENSIVE);
        }
        break;

    case PE_ARG_DI_MA:
        vDrvDISetQualityMode(E_DI_ENGINE_MA, i2Cur==SV_MA_SLOW ?
            E_DI_QUALITY_AGGRESSIVE : E_DI_QUALITY_DEFENSIVE);
        break;

    case PE_ARG_DI_EDGE_PRESERVE:
        vDrvDISetQualityMode(E_DI_ENGINE_EDGEP, i2Cur==SV_V_EDGE_LOW ?
            E_DI_QUALITY_DEFENSIVE : E_DI_QUALITY_AGGRESSIVE);
        break;

    case PE_ARG_BACK_LIGHT_LVL:
        wWriteQualityTable(QUALITY_BACKLIGHT,
                bApiQtyMapping(u1SrcTypTmg, QUALITY_BACKLIGHT, &i2Min,
                                &i2Max, &i2Dft, &i2Cur));
        DRVCUST_SetBacklightDuty(wReadQualityTable(QUALITY_BACKLIGHT));
        break;

    // RGB gain, hw range 0~255, default 128.
    case PE_ARG_R_GAIN:
        wWriteQualityTable(QUALITY_R_GAIN,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_R_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) >> 2));
        wWriteQualityTable(QUALITY_R_GAIN_LSB,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_R_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) & 0x3));

        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_R_GAIN, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_G_GAIN:
        wWriteQualityTable(QUALITY_G_GAIN,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_G_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) >> 2));
        wWriteQualityTable(QUALITY_G_GAIN_LSB,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_G_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) & 0x3));


        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_G_GAIN, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_B_GAIN:
        wWriteQualityTable(QUALITY_B_GAIN,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_B_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) >> 2));
        wWriteQualityTable(QUALITY_B_GAIN_LSB,
                           (UINT8)(bApiQtyMapping(u1SrcTypTmg, QUALITY_B_GAIN_LSB,
                                          &i2Min, &i2Max, &i2Dft, &i2Cur) & 0x3));

        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_B_GAIN, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_R_OFFSET:
        wWriteQualityTable(QUALITY_R_OFFSET,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) >> 2);
        wWriteQualityTable(QUALITY_R_OFFSET_LSB,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) & 0x0003);

        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_R_OFFSET, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_G_OFFSET:
        wWriteQualityTable(QUALITY_G_OFFSET,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) >> 2);
        wWriteQualityTable(QUALITY_G_OFFSET_LSB,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) & 0x0003);
        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_G_OFFSET, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_B_OFFSET:
        wWriteQualityTable(QUALITY_B_OFFSET,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) >> 2);
        wWriteQualityTable(QUALITY_B_OFFSET_LSB,
                           ((UINT16) i2Cur -
                            (UINT16) ((i2Min + i2Max + 1) / 2)) & 0x0003);
        if (_fgCallFromIsr == SV_OFF)
        {
            vApiRegisterVideoEvent(PE_ARG_B_OFFSET, u1VidPath, SV_ON);
        }
        break;
    case PE_ARG_DEMO:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }
        Region rDummyRegion;
        //support only Split demo.
        vApiVideoDemoMode(i2Cur, rDummyRegion, 0xFF);
        break;

#ifdef DRV_SUPPORT_EXTMJC
    case PE_ARG_MJC_EFFECT:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        if (i2Cur == VDP_ARG_MJC_EFFECT_OFF)
        {
            bMjcUiEffect = MJC_CTRL_EFCT_OFF;
        }
        else if (i2Cur == VDP_ARG_MJC_EFFECT_LOW)
        {
            bMjcUiEffect = MJC_CTRL_EFCT_LOW;
        }
        else if (i2Cur == VDP_ARG_MJC_EFFECT_MIDDLE)
        {
            bMjcUiEffect = MJC_CTRL_EFCT_MID;
        }
        else
        {
            bMjcUiEffect = MJC_CTRL_EFCT_HIGH;
        }

        LOG(6, "VDP_ARG_MJC_EFFECT %d\n", i2Cur);
        break;

    case PE_ARG_MJC_MODE:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        bMjcUiMode = i2Cur;

        LOG(6, "VDP_ARG_MJC_MODE %d\n", i2Cur);
        break;

    case PE_ARG_MJC_DEMO:
        if (u1VidPath != SV_VP_MAIN)
        {
            break;
        }

        bMjcUiDemo = i2Cur;
        vMJCSetDemoMode(bMjcUiEffect, bMjcUiMode, bMjcUiDemo);

        LOG(6, "VDP_ARG_MJC_DEMO %d\n", i2Cur);
        break;
#endif

    default:
        return SV_FALSE;
    }

    return SV_TRUE;
}

