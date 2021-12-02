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
#ifndef	_PQL_IO_DDI_H_
#define	_PQL_IO_DDI_H_

#include "common.h"

typedef enum {
	PQL_WB_RED		= 0,				/**< white balance Red data*/
	PQL_WB_GRN		= 1,				/**< white balance Green data*/
	PQL_WB_BLU		= 2,				/**< white balance Blue data*/

} PQL_WB_COLOR_T;

typedef struct PQL_COLOR_TEMP
{
	UINT8	rgbGain[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
	UINT8	rgbOffset[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/
}PQL_COLOR_TEMP_T;

typedef struct PQL_WB_DATA
{
	PQL_COLOR_TEMP_T	wbCool;				/**< white balance Cool data*/
	PQL_COLOR_TEMP_T	wbNorm;				/**< white balance Normal data*/
	PQL_COLOR_TEMP_T	wbWarm;				/**< white balance Warm data*/

}PQL_WB_DATA_T;

//typedef struct PQL_COLOR_TEMP_CORRECT
//{
//	SINT32	rgbGainCorrect[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
//	SINT16	rgbOffsetCorrect[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/
//
//}PQL_COLOR_TEMP_CORRECT_T;
//
//typedef struct PQL_WB_CORRECT_DATA
//{
//	PQL_COLOR_TEMP_CORRECT_T	wbCorrectCool;				/**< white balance Cool data*/
//	PQL_COLOR_TEMP_CORRECT_T	wbCorrectNorm;				/**< white balance Normal data*/
//	PQL_COLOR_TEMP_CORRECT_T	wbCorrectWarm;				/**< white balance Warm data*/
//
//}PQL_WB_CORRECT_DATA_T;

typedef struct {

	UINT8	subBrightness;						/**< sub brightness data*/
	UINT8	subContrast;						/**< sub contrast data*/

} PQL_SUB_CONTBRI_T;

typedef struct PQL_CM_EXPCOLOR_PARAM{
	UINT8	pqlCmRegionID;					/**< ID	*/
	UINT8	pqlCmCoeffID;					/**< DB Table Index	*/
//	SINT8	uiExpSatGainValue;				/**< UI Sat Gain Value */
//	SINT8	uiExpHueGainValue;				/**< UI Hue Gain Value */
	UINT8	uiExpSatGainValue;				/**< UI Sat Gain Value */
	UINT8	uiExpHueGainValue;				/**< UI Hue Gain Value */
}PQL_CM_EXPCOLOR_PARAM_T;

typedef struct PQL_CM_OUT_CS
{
	//Output CSC Setting Val.
//	float fColorStdMatrix1[3];
//	float fColorStdMatrix2[3];
//	float fColorStdMatrix3[3];
	UINT32 fColorStdMatrix1[3];
	UINT32 fColorStdMatrix2[3];
	UINT32 fColorStdMatrix3[3];
	UINT8 uColorStdForRev ;
}PQL_CM_OUT_CS_T;

typedef enum {
	PQL_PATH_MAIN		= 0,		/* main path */
	PQL_PATH_SUB,				/* sub path */
	PQL_PATH_VCR,				/* vcr out path */
	PQL_PATH_MAXN
} PQL_PATH_T;

typedef struct PQL_SHARP_UI_VALUE{
//	float	fSharpnessUi;
	UINT32	fSharpnessUi;
}PQL_SHARP_UI_VALUE_T;


typedef enum {
	  PQL_NR_OFF	  = 0,
	  PQL_NR_LOW,
	  PQL_NR_MEDIUM,
	  PQL_NR_HIGH
} PQL_NR_LEVEL_T;

typedef struct PQL_NR_CONTROL_INFORM
{
	UINT8 uIsProgressive;

} PQL_NR_CONTROL_INFORM_T;

typedef enum {
	PQL_BLACK_RF		= 0,			/**< Black for RF 		*/
	PQL_BLACK_AV,					/**< Black for AV 		*/
	PQL_BLACK_COMP,				/**< Black for Component	*/
	PQL_BLACK_RGB,					/**< Black for RGB 		*/
	PQL_BLACK_HDMI,				/**< Black for HDMI 		*/
	PQL_BLACK_DECODER, 			/**< Black for Decoder type	*/
	PQL_BLACK_OTHERS,			/**< Black for others		*/
	PQL_BLACK_INVALID				/**< invalid		*/
} PQL_BLACK_SOURCE_T;

typedef enum {
	PQL_BLACK_LOW		= 0,			/**< Black Level Low	*/
	PQL_BLACK_HIGH, 					/**< Black Level High	*/
	PQL_BLACK_AUTO, 					/**< Black Level Auto	*/
} PQL_BLACK_T;

//typedef struct PQL_PC_UI_VALUE{
//	float contrastUi;
//	float brightnessUi;
//	float colorUi;
//	float tintUi;
//	float contrastCentreUi;
//}PQL_PC_UI_VALUE_T;
typedef struct PQL_PC_UI_VALUE{
	UINT32 contrastUi;
	UINT32 brightnessUi;
	UINT32 colorUi;
	UINT32 tintUi;
	UINT32 contrastCentreUi;
}PQL_PC_UI_VALUE_T;

 typedef enum {
		 PQL_EYEQ_CONTRAST	 = 0x0000,
		 PQL_EYEQ_BRIGHTNESS,
		 PQL_EYEQ_COLOR,
		 PQL_EYEQ_TINT,
		 PQL_EYEQ_SHARPNESS,
		 PQL_EYEQ_BACKLIGHT
 } PQL_EYEQ_DB_MEMBER_T;

 typedef enum {
		 PQL_KIND_CONTRAST_OFFSET	 = 0x0000,
		 PQL_KIND_BRIGHTNESS_OFFSET,
		 PQL_KIND_COLOR_OFFSET,
		 PQL_KIND_TINT_OFFSET,
		 PQL_KIND_SHARPNESS_OFFSET
 } PQL_KIND_DB_MEMBER_T;

typedef enum {
		  PQL_DIMMING_APL	  = 0x0000,
		  PQL_DIMMING_VBRB
} PQL_DIMMING_DB_MEMBER_T;

typedef enum {
		 PQL_DC2P_LOWPEAK_POS	 = 0x0000,	 /*  1 parameter is required	 */
		 PQL_DC2P_HIGHPEAK_POS, 		 		/*  1 parameter is required	 */

		 PQL_DC2P_LOWPEAK,					 /*  2 parameter is required	 */
		 PQL_DC2P_BRIGHTNESS_OFFSET,	 		/*  2 parameter is required	 */
		 PQL_DC2P_HIGHPEAK, 			 		/*  2 parameter is required	 */
		 PQL_DC2P_CONTRAST_GAIN,		 		/*  2 parameter is required	 */
		 PQL_DC2P_DYNAMIC_COLOR_GAIN	 	/*  2 parameter is required	 */
 } PQL_DYNAMICCONTRAST_DB_MEMBER_T;

typedef enum {
	PQL_APL_TRANSCURVE	   = 0x0000,
	PQL_APL_APLPOINT
} PQL_APL_DB_MEMBER_T;

 typedef enum {
	PQL_CS_SD=0,
	PQL_CS_HD
} PQL_CS_T;

typedef enum {
	SAFEFILTER_RF_TYPE,
	SAFEFILTER_AV_SVIDEO_TYPE,
	SAFEFILTER_SCART_RGB_COMPSD_I_TYPE,
	SAFEFILTER_COMPSD_P_TYPE,
	SAFEFILTER_COMP720_TYPE,
	SAFEFILTER_COMP1080I_TYPE,
	SAFEFILTER_COMP1080P_TYPE
}PQL_AFEF_T;

typedef enum
{
	PQL_AVDEC_MODE_NTSC,
	PQL_AVDEC_MODE_PAL,
	PQL_AVDEC_MODE_PAL_N,
	PQL_AVDEC_MODE_PAL_M,
	PQL_AVDEC_MODE_SECAM,
	PQL_AVDEC_MODE_NTSC_443,
	PQL_AVDEC_MODE_PAL_60,
	PQL_AVDEC_MODE_UNKNOWN_525,
	PQL_AVDEC_MODE_UNKNOWN_625,
	PQL_AVDEC_MODE_UNKNOWN,
}PQL_AVDECODER_VIDEO_MODE_T;

typedef enum PQL_MRE_AV_SOURCE
{
	PQL_AV_SRC_INVALID = 0,		// Source is Invalid
	PQL_AV_SRC_ANTENNA_ANALOG,	// TV Antenna Analog
	#ifdef INCLUDE_CABLE
	PQL_AV_SRC_CABLE_ANALOG,	// TV Cable Analog
	#endif
	PQL_AV_SRC_ANTENNA_DIGITAL,	// TV Antenna Digital
	#ifdef INCLUDE_CABLE
	PQL_AV_SRC_CABLE_DIGITAL,	// TV Cable Digital
	#endif
	PQL_AV_SRC_EXT1,			// External Input 1	: CVBS
	PQL_AV_SRC_EXT2,			// External Input 2 : SVIDEO
	PQL_AV_SRC_EXT3,			// External Input 3 :
	PQL_AV_SRC_COMP1,			// Component (YPbPr) 1
//#if( SYS_DVB_AU || SYS_ATSC )
//	PQL_AV_SRC_COMP2,			// Component (YPbPr) 2
//#endif
	PQL_AV_SRC_RGB_PC,			// RGB - PC
	PQL_AV_SRC_DVI_PC,			// DVI - PC
	PQL_AV_SRC_DVI_DTV,			// DVI - Settop Box
	PQL_AV_SRC_HDMI,			// HDMI
	PQL_AV_SRC_HDMI1,			// HDMI1
	PQL_AV_SRC_HDMI2,			// HDMI2
	PQL_AV_SRC_HDMI3,			// HDMI3
	PQL_AV_SRC_HDMI4,			// HDMI4
	PQL_AV_SRC_AUTOAV1,			// Auto AV Input 1
	PQL_AV_SRC_AUTOAV2,			// Auto AV Input 2
	#ifdef INCLUDE_DVR_READY
	PQL_AV_SRC_PLAYHDD,			// PVR : Recorded Playback mode
	PQL_AV_SRC_DIGITAL_FROM_HDD,//path
	#endif

	PQL_AV_SRC_NOT_CHANGED,		// Source not changed
	PQL_AV_SRC_NOT_DETERMINED	// Source is not determined

} PQL_MRE_AV_SOURCE_T;

//typedef enum {
///* PQL_INPUT_DTV		: Digital_RF + Digital_PVR 													*/
//	PQL_DTV_HD_I				= 0 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Interlace : 	1080i	*/
//	PQL_DTV_HD_P				= 1 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :	 720p	*/
//	PQL_DTV_SD_P				= 2 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Progressive :	 480p	*/
//	PQL_DTV_SD_I				= 3 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Interlace :	 480i	*/
//	PQL_DTV_FHD				= 4 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :  1080p	*/ //gbtogether(070821)
//	PQL_DTV_NONE0				= 5 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :  720p	*/
//	PQL_DTV_NONE1 			= 6 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Progressive :  576p	*/
//	PQL_DTV_HD_I_DVB			= 7 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Interlace : 	 576i	*/
//	PQL_DTV_HD_P_DVB			= 8 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
//	PQL_DTV_SD_P_DVB			= 9 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
//	PQL_DTV_SD_I_DVB			= 10 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
//	PQL_DTV_FHD_DVB			= 11 + PQL_FORMAT_MAX*PQL_INPUT_DTV,	//gbtogether(070821)
//	PQL_DTV_NONE2				= 12 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
//	PQL_DTV_NONE3				= 13 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
//
///* PQL_INPUT_ANA		: Analog_RF/CVBS/S-video + X260_AVDC										*/
//	PQL_ANA_RF					= 0 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* Analog_RF + X260_AVDC	*/
//	PQL_ANA_CVBS				= 1 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* CVBS + X260_AVDC			*/
//	PQL_ANA_SVID				= 2 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* S-video + X260_AVDC		*/
//	PQL_ANA_SCARTRGB			= 3 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* Analog_RF + X260_AVDC	*/ //gbtogether(070821)
//	PQL_ANA_NONE0				= 4 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* CVBS + X260_AVDC			*/
//	PQL_ANA_NONE1				= 5 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* S-video + X260_AVDC		*/
//	PQL_ANA_NONE2				= 6 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//	PQL_ANA_RF_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//	PQL_ANA_CVBS_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//	PQL_ANA_SVID_PAL			= 9 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//	PQL_ANA_RF_SECAM			= 10 + PQL_FORMAT_MAX*PQL_INPUT_ANA,   // RF-SECAM 대응. gbtogether(091029)
//	PQL_ANA_CVBS_SECAM		= 11 + PQL_FORMAT_MAX*PQL_INPUT_ANA,   // AV-SECAM 대응. gbtogether(091029)
//	PQL_ANA_NONE5				= 12 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//	PQL_ANA_NONE6				= 13 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
//
///* PQL_INPUT_EANA		: Analog_RF/CVBS/S-video + uPD64015											*/
//	PQL_EANA_RF				= 0 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* Analog_RF + uPD64015		*/
//	PQL_EANA_CVBS				= 1 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* CVBS + uPD64015			*/
//	PQL_EANA_SVID				= 2 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* S-video + uPD64015		*/
//	PQL_EANA_NONE0			= 3 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* Analog_RF + uPD64015		*/
//	PQL_EANA_NONE1			= 4 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* CVBS + uPD64015			*/
//	PQL_EANA_NONE2			= 5 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* S-video + uPD64015		*/
//	PQL_EANA_NONE3			= 6 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_RF_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_CVBS_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_SVID_PAL			= 9 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//	PQL_EANA_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
//
///* PQL_INPUT_COMP		: Component + MST3367														*/
//	PQL_COMP_FHD				= 0 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080P	*/
//	PQL_COMP_HD_I				= 1 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080i	*/
//	PQL_COMP_HD_P				= 2 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive : 720p	*/
//	PQL_COMP_SD_P				= 3 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Progressive : 480p	*/
//	PQL_COMP_SD_I				= 4 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Interlace :   480i	*/
//	PQL_COMP_NONE0			= 5 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive :1080P	*/
//	PQL_COMP_NONE1			= 6 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080i	*/
//	PQL_COMP_FHD_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive : 720p	*/
//	PQL_COMP_HD_I_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Progressive : 576p	*/
//	PQL_COMP_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Interlace :   576i	*/
//	PQL_COMP_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
//	PQL_COMP_SD_I_PAL			= 11 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
//	PQL_COMP_NONE2			= 12 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
//	PQL_COMP_NONE3			= 13 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
//
///* PQL_INPUT_RGBPC	: RGB_PC + MST3367															*/
//	PQL_RGBPC_VGA				= 0 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 640x480					*/
//	PQL_RGBPC_SVGA			= 1 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 800x600					*/
//	PQL_RGBPC_XGA				= 2 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1280x768					*/
//	PQL_RGBPC_WXGA			= 3 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1366x768					*/
//	PQL_RGBPC_UXGA			= 4 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1600x1200				*/
//	PQL_RGBPC_FHD				= 5 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE1			= 7 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE2			= 8 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE3			= 9 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//	PQL_RGBPC_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
//
///* PQL_INPUT_DVIPC	: DVI_PC + MST3367															*/
//	PQL_DVIPC_VGA				= 0 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 640x480					*/
//	PQL_DVIPC_SVGA			= 1 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 800x600					*/
//	PQL_DVIPC_XGA				= 2 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1280x768					*/
//	PQL_DVIPC_WXGA			= 3 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1366x768					*/
//	PQL_DVIPC_UXGA			= 4 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1600x1200				*/
//	PQL_DVIPC_FHD				= 5 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE1			= 7 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE2			= 8 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE3			= 9 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//	PQL_DVIPC_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
//
///* PQL_INPUT_HDMIAV	: HDMI_AV + MST3367															*/
//	PQL_HDMIAV_FHD			= 0 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 1080P	*/
//	PQL_HDMIAV_HD_I			= 1 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Interlace : 1080i		*/
//	PQL_HDMIAV_HD_P			= 2 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 720p	*/
//	PQL_HDMIAV_SD_P			= 3 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Progressive : 480p	*/
//	PQL_HDMIAV_SD_I			= 4 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Interlace : 480i		*/
//	PQL_HDMIAV_NONE0			= 5 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 1080P	*/
//	PQL_HDMIAV_NONE1			= 6 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Interlace : 1080i		*/
//	PQL_HDMIAV_FHD_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 720p	*/
//	PQL_HDMIAV_HD_I_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Progressive : 576p	*/
//	PQL_HDMIAV_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Interlace : 576i		*/
//	PQL_HDMIAV_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
//	PQL_HDMIAV_SD_I_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
//	PQL_HDMIAV_NONE2			= 12 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
//	PQL_HDMIAV_NONE3			= 13 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
//
///* PQL_INPUT_PVRANA	: PVR_Analog Source															*/
//	PQL_PVRANA_EANA_RF			= 0 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* Analog_RF				*/
//	PQL_PVRANA_EANA_AV			= 1 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* CVBS/S-video				*/
//	PQL_PVRANA_COMP_HD			= 2 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
//	PQL_PVRANA_COMP_SD			= 3 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 480p/480i				*/
//	PQL_PVRANA_HDMI_HD			= 4 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
//	PQL_PVRANA_HDMI_SD			= 5 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 480p/480i				*/
//	PQL_PVRANA_SVID				= 6 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* Analog_S-video		*/ //gbtogether(070821)
//	PQL_PVRANA_EANA_RF_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* CVBS/S-video				*/
//	PQL_PVRANA_EANA_AV_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
//	PQL_PVRANA_COMP_HD_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 576p/576i				*/
//	PQL_PVRANA_COMP_SD_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
//	PQL_PVRANA_HDMI_HD_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 576p/576i				*/
//	PQL_PVRANA_HDMI_SD_PAL		= 12 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,
//	PQL_PVRANA_SVID_PAL			= 13 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,   //gbtogether(070821)
//
///* ETC. */
////gbtogether(070821)
////PVR DTV & PVR ScartRGB & EPF,i-frame .....
//	PQL_PVRDTV_FHD			= 0 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Progressive : 1080P	*/
//	PQL_PVRDTV_HD_I			= 1 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Interlace : 1080i		*/
//	PQL_PVRDTV_HD_P			= 2 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Progressive : 720p	*/
//	PQL_PVRDTV_SD_P			= 3 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* SD Progressive : 480p	*/
//	PQL_PVRDTV_SD_I			= 4 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* SD Interlace : 480i		*/
//	PQL_PVRDTV_SCARTRGB		= 5 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* Scart_RGB						*/
//	PQL_PVRDTV_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
//	PQL_PVRDTV_FHD_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
//	PQL_PVRDTV_HD_I_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
//	PQL_PVRDTV_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
//	PQL_PVRDTV_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
//	PQL_PVRDTV_SD_I_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
//	PQL_EPF					= 12 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
//	PQL_IFRAME					= 13 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
//
//
///* Reserved */
//	PQL_RESERVED0				= 0 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Invalid					*/
//	PQL_RESERVED1				= 1 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED2				= 2 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED3				= 3 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED4				= 4 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED5				= 5 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Invalid					*/
//	PQL_RESERVED6				= 6 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED7				= 7 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED8				= 8 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED9				= 9 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
//	PQL_RESERVED10				= 10 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Invalid					*/
//	PQL_RESERVED11				= 11 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
//	PQL_RESERVED12				= 12 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
//	PQL_RESERVED13				= 13 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
//} PQL_INPUT_SRC_FMT_T;

typedef enum{
	PQL_SRC_PORT_TP			= 0,// Transport Stream Input Port
	PQL_SRC_PORT_INT,			// Internal Input Processing(used internal IP, NTDEC/ADC/HDMI)
	PQL_SRC_PORT_EXT,			// External Input Processing(used external IP, NTDEC/ADC/HDMI)				/*AVI 소스*/
} PQL_VIDEO_SRC_PORT_T;

typedef enum {
	PQL_SRC_TYPE_MIN		= 0,

	PQL_SRC_TYPE_DTV		= PQL_SRC_TYPE_MIN,
	PQL_SRC_TYPE_AVD,		// User Defined, NT/PAL Input using Analog Video Decoder
	PQL_SRC_TYPE_COMP,		// User Defined, Component Input using ADC
	PQL_SRC_TYPE_VGA,		// User Defined, VGA Input using ADC
	PQL_SRC_TYPE_HDMI,		// User Defined, HDMI/DVI Input using HDMI

	PQL_SRC_TYPE_MAX		= PQL_SRC_TYPE_HDMI,
	PQL_SRC_TYPE_MAXN,
	PQL_SRC_TYPE_INVALID	= PQL_SRC_TYPE_MAXN

} PQL_SRC_TYPE_T;

typedef enum {
	PQL_SRC_MSK_VID			= 0x03,
	PQL_SRC_MSK_REG			= 0x0f,
	PQL_SRC_MSK_DTV			= 1 << (PQL_SRC_PORT_TP 	+ 4),			// 0001 0000	// VDEC
	PQL_SRC_MSK_APORT		= 1 << (PQL_SRC_PORT_INT	+ 4),			// 0010 0000	// APORT
	PQL_SRC_MSK_DPORT		= 1 << (PQL_SRC_PORT_EXT	+ 4),			// 0100 0000	// DPORT
	PQL_SRC_MSK_EXT			= (PQL_SRC_MSK_DPORT | PQL_SRC_MSK_APORT),	// 0110 0000	// EXT

	// RVI(YPbPr,RGBPC,RGBDTV,DVI..)

	PQL_SRC_NONE			= 0x00,										// Default DTV/VID_0

	PQL_SRC_DTV				= 0x00	| PQL_SRC_MSK_DTV	| (PQL_SRC_TYPE_DTV	<< 8),	// 010
	PQL_SRC_RF1				= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 120
	PQL_SRC_CVBS1			= 0x01	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 121
	PQL_SRC_SVIDEO1		 	= 0x02	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 123
	PQL_SRC_SCART1			= 0x03	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 124
	PQL_SRC_SCART1_RGB		= 0x04	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 125
	PQL_SRC_SCART2		 	= 0x05	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 126
//#if( SYS_DVB_AU || SYS_ATSC )
//	PQL_SRC_CVBS2		 	= 0x06	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 127
//	PQL_SRC_SVIDEO2		 	= 0x07	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 128
//	PQL_SRC_COMP2			= 0x01	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_COMP<< 8),	// 221
//#endif

	PQL_SRC_COMP			= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_COMP<< 8),	// 220
	PQL_SRC_VGA				= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_VGA	<< 8),	// 320
	PQL_SRC_HDMI1			= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_HDMI<< 8),	// 420
	PQL_SRC_HDMI2			= 0x01	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_HDMI<< 8),	// 421

	PQL_SRC_NO_CHANGE		= 0xffffffff

} PQL_VIDEO_SRC_T;


typedef struct PQL_SIZE{
	UINT16	inWidth;				/*input width*/
	UINT16	inHeight;				/*input height*/
	UINT16	outWidth;				/*output width*/
	UINT16	outHeight;				/*output height*/
}PQL_SIZE_T;

//typedef struct PQL_STATUS{
////	BOOLEAN					pqlPath[PQL_PATH_MAXN];					/* path selection */
//	UINT8					pqlPath[PQL_PATH_MAXN];					/* path selection */
//	PQL_INPUT_SRC_FMT_T		pqlSrcFmt[PQL_PATH_MAXN];				/* src and fmt */
//	PQL_SIZE_T				pqlSize[PQL_PATH_MAXN];					/* size*/
//}PQL_STATUS_T;

typedef enum
{
	/* EZ Picture			=	4EA      */
	PQL_COEFF_PC			=	0x00001001,
	PQL_COEFF_PC_KIND		=	0x00001002,
	PQL_COEFF_PC_CE			=	0x00001003,
	PQL_COEFF_PC_EYEQ		=	0x00001004,
	/* XD_Contrast			=	1EA      */
	PQL_COEFF_DC2P_DYNCONT =	0x00002001,
	PQL_COEFF_DC2P_APL	    =	0x00002002,
	PQL_COEFF_DC2P_BACKCTRL	=	0x00002003,
	/* XD_Color				=	8EA      */
	PQL_COEFF_CM_MGT		=	0x00003001,
	PQL_COEFF_CM_RED		=	0x00003002,
	PQL_COEFF_CM_YEL		=	0x00003003,
	PQL_COEFF_CM_GRN		=	0x00003004,
	PQL_COEFF_CM_CYN		=	0x00003005,
	PQL_COEFF_CM_BLU		=	0x00003006,
	PQL_COEFF_CM_FT			=	0x00003007,
	PQL_COEFF_CM_BS			=	0x00003008,
	PQL_COEFF_CM_CS			=	0x00003009,
	/* there are dependent on the chip sharpness algorithm
	, so Please refer to pql_GetSocCoeffID function.
	Please refer to pql.h*/
#if 0	//by hwangbos 0428
	/* Sharpness                       =     8EA      */ //gbtogether(070626)
	PQL_COEFF_SHARP_EE			=	0x00004101,

	PQL_COEFF_SHARP_SHOOTGAIN	=	0x00004102,
	PQL_COEFF_SHARP_NOISECORING	=	0x00004103,
	PQL_COEFF_SHARP_NONLINEAR	=	0x00004104,
	PQL_COEFF_SHARP_HOR_LTI		=	0x00004105,
	PQL_COEFF_SHARP_VERT_LTI		=	0x00004106,
	PQL_COEFF_SHARP_HOR_CTI		=	0x00004107,
	PQL_COEFF_SHARP_VERT_CTI		=	0x00004108,
#endif
	/* White Balance Gain/Offset Correction = 1EA*/
	PQL_COEFF_WB_OFFSET		=	0x00005201,
	/* MADI Control = 1EA*/
	PQL_COEFF_MADI			=	0x00006001,
	/* there are dependent on the chip NR algorithm
	, so Please refer to pql_GetSocCoeffID function.
	Please refer to pql.h*/
#if 0	//by hwangbos 0428
	/* NR Control = 2EA*/
	PQL_COEFF_MPEGNR			=	0x00007001,
	PQL_COEFF_SNR				=	0x00007002,
	//[LGE jykim 20070903 : TNR]
	PQL_COEFF_TNR			=	0x00008001,
#endif
	//[LGE jykim 20070804 : Gamma]
	PQL_COEFF_GAMMA_19 	=	0x00006101,
	PQL_COEFF_GAMMA_22 	=	0x00006102,
	PQL_COEFF_GAMMA_24 	=	0x00006103
} PQL_COEFF_ID_T;

typedef struct {

    char    cPQ_ModelName[15];      /**< model name*/
    char    cPQ_DbVersion[4];           /**< DB ver.*/
    char    cPQ_DrvVersion[4];      /**< DVR ver.*/

} PQL_VERSION_T;

typedef struct {

	UINT8	gLuma_WBGainAdj;
	UINT8	gLuma_WBOffsetAdj;

} PQL_LUMA_WB_T;

typedef struct PQL_COEFF_MEMBER {

    UINT8    uPqlCoeffID;      /**< model name*/
    UINT8    uPqlCoeffIDMax;          /**< DB ver.*/

} PQL_COEFF_MEMBER_T;

#define	MAX_CHIP_BIN_SIZE	64
#define	MAX_CHIP_NUM_TRANSCURVE		16

typedef struct PQL_DC2P_RAWDATA_INFO {
	UINT32		nSupportDataType;
	UINT32		nBinNum;
//	SINT32		nByPassCurveTable[MAX_CHIP_NUM_TRANSCURVE];		//KWON_0716
	UINT32		nByPassCurveTable[MAX_CHIP_NUM_TRANSCURVE];		//KWON_0716
//	SINT32		nBinStep;
	UINT32		nBinStep;
	UINT32		uCountBins[MAX_CHIP_BIN_SIZE];
	UINT32		uAPL;
//	SINT32		nMin;
	UINT32		nMin;
//	SINT32		nMax;
	UINT32		nMax;
//	SINT32		nPeakLow;
	UINT32		nPeakLow;
//	SINT32		nPeakHigh;
	UINT32		nPeakHigh;
} PQL_DC2P_RAWDATA_INFO_T;

#endif	// _PQL_IO_DDI_H_

