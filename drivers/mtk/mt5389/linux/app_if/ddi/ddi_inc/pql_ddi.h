/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file pql_ddi.h
 *
 *  PQL include header.
 *
 *  @author	Youm, Seung-Jun(sjyoum@lge.com)
 *  @version	1.0
 *  @date		2006.08.30
 *  @see
 */


/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	__PQL_DDI_H_
#define	__PQL_DDI_H_

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#ifndef __KERNEL__
#include "common.h"
#include "osa_modeldef.h"
#include "branches.h"

/******************************************************************************
	상수 정의
	(Constant Definitions)
*******************************************************************************/
#define	FMT_CONVERT	1	//tmp

/* LG DDI -------------------------------------------------------------------*/
//#if (MAIN_CHIP_TYPE == CHIP_DOUGLAS) //dhjung
#define PQL_NR_PROGRESSIVE_TIMING			0x01
#define PQL_NR_NO_PROGRESSIVE_TIMING		0x02
//#endif

#define DC2APLCTRL_DATA_LOW 	0x00
#define DC2APLCTRL_DATA_HIGH 	0x01
#define DC2APLCTRL_DATA_STORE	0x02

//For Chip Support Data inform (DC2)
#define	SUPPORT_HISTOGRAM_DATA			0x00000001
#define	SUPPORT_APL_TOTAL_DATA			0x00000002
#define	SUPPORT_APL_MIN_DATA				0x00000004
#define	SUPPORT_APL_MAX_DATA			0x00000008
#define	SUPPORT_PEAK_LOW_DATA			0x00000010
#define	SUPPORT_PEAK_HIGH_DATA			0x00000020


#define SUPPORT_HISTOGRAM(_v)			( _v | SUPPORT_HISTOGRAM_DATA)
#define NO_SUPPORT_HISTOGRAM(_v)		( _v & (~SUPPORT_HISTOGRAM_DATA))

#define SUPPORT_APL_TOTAL(_v)			( _v | SUPPORT_APL_TOTAL_DATA)
#define NO_SUPPORT_APL_TOTAL(_v)		( _v & (~SUPPORT_APL_TOTAL_DATA))

#define SUPPORT_APL_MIN(_v)			( _v | SUPPORT_APL_MIN_DATA)
#define NO_SUPPORT_APL_MIN(_v)			( _v & (~SUPPORT_APL_MIN_DATA))

#define SUPPORT_APL_MAX(_v)			( _v | SUPPORT_APL_MAX_DATA)
#define NO_SUPPORT_APL_MAX(_v)		( _v & (~SUPPORT_APL_MAX_DATA))

#define SUPPORT_PEAK_LOW(_v)			( _v | SUPPORT_PEAK_LOW_DATA)
#define NO_SUPPORT_PEAK_LOW(_v)		( _v & (~SUPPORT_PEAK_LOW_DATA))

#define SUPPORT_PEAK_HIGH(_v)			( _v | SUPPORT_PEAK_HIGH_DATA)
#define NO_SUPPORT_PEAK_HIGH(_v)		( _v & (~SUPPORT_PEAK_HIGH_DATA))

/**
* Input Type & Format Type Definition for PQL
*/
/* PQL_INPUT_TYPE */
#define PQL_INPUT_DTV			0		/* Digital_RF + Digital_PVR */
#define PQL_INPUT_ANA			1		/* Analog_RF/CVBS/S-video + X260_AVDC */
#define PQL_INPUT_EANA			2		/* Analog_RF/VBS/S-video + uPD64015 */
#define PQL_INPUT_COMP			3		/* Component + MST3367 */
#define PQL_INPUT_RGBPC			4		/* RGB_PC + MST3367 */
#define PQL_INPUT_DVIPC			5		/* DVI_PC + MST3367 */
#define PQL_INPUT_HDMIAV		6		/* HDMI_AV + MST3367 */
#define PQL_INPUT_PVRANA		7		/* PVR_Analog Source */
#define PQL_INPUT_ETC			8		/* etc Source */
#define PQL_INPUT_RESERVED		9		/* Reserved */
#define PQL_SOURCE_MAX			10		/* Max source in this system */
#define PQL_FORMAT_MAX			14		/* Max format among max format of each source */
#define PQL_SRC_FMT_MAX			PQL_FORMAT_MAX*PQL_SOURCE_MAX			/* Max # of source & format */

#endif

/*******************************************************************************
	매크로 정의
	(Macro Definitions)
*******************************************************************************/


/*******************************************************************************
    형 정의
    (Type Definitions)
*******************************************************************************/


/**
 * color temperature struc.
 *
 * @see
 */
typedef enum {
	PQL_WB_RED		= 0,				/**< white balance Red data*/
	PQL_WB_GRN		= 1,				/**< white balance Green data*/
	PQL_WB_BLU		= 2,				/**< white balance Blue data*/

} PQL_WB_COLOR_T;

/**
 * color temperature 값.
 *
 * @see
 */
typedef struct PQL_COLOR_TEMP
{
	UINT8	rgbGain[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
	UINT8	rgbOffset[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/

}PQL_COLOR_TEMP_T;

/**
 * white balace 값.
 *
 * @see
 */
typedef struct PQL_WB_DATA
{
	PQL_COLOR_TEMP_T	wbCool;				/**< white balance Cool data*/
	PQL_COLOR_TEMP_T	wbNorm;				/**< white balance Normal data*/
	PQL_COLOR_TEMP_T	wbWarm;				/**< white balance Warm data*/

}PQL_WB_DATA_T;

#ifndef __KERNEL__
/**
 * color temperature 값.
 *
 * @see
 */
typedef struct PQL_COLOR_TEMP_CORRECT
{
	SINT32	rgbGainCorrect[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
	SINT16	rgbOffsetCorrect[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/

}PQL_COLOR_TEMP_CORRECT_T;

/**
 * white balace 값.
 *
 * @see
 */
typedef struct PQL_WB_CORRECT_DATA
{
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectCool;				/**< white balance Cool data*/
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectNorm;				/**< white balance Normal data*/
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectWarm;				/**< white balance Warm data*/

}PQL_WB_CORRECT_DATA_T;

#endif

/**
 * Sub-Brightness, Sub-Contrast struct.
 *
 * @see
 */
typedef struct {

	UINT8	subBrightness;						/**< sub brightness data*/
	UINT8	subContrast;						/**< sub contrast data*/

} PQL_SUB_CONTBRI_T;

/**
 * CM : Expert Color Mangement struct.
 *
 * @see
 */
typedef struct PQL_CM_EXPCOLOR_PARAM{
	UINT8	pqlCmRegionID;					/**< 설정하고자 하는 영역 ID	*/
	UINT8	pqlCmCoeffID;					/**< DB Table Index	*/
#ifndef __KERNEL__
	SINT8	uiExpSatGainValue;				/**< UI Sat Gain Value */
	SINT8	uiExpHueGainValue;				/**< UI Hue Gain Value */
#else
	UINT8	uiExpSatGainValue;				/**< UI Sat Gain Value */
	UINT8	uiExpHueGainValue;				/**< UI Hue Gain Value */
#endif
}PQL_CM_EXPCOLOR_PARAM_T;

/**
 * CM : Output CSC coefficient struct.
 *
 * @see
 */
typedef struct PQL_CM_OUT_CS
{
	//Output CSC Setting Val.
#ifndef __KERNEL__
	float fColorStdMatrix1[3];
	float fColorStdMatrix2[3];
	float fColorStdMatrix3[3];
#else
	UINT32 fColorStdMatrix1[3];
	UINT32 fColorStdMatrix2[3];
	UINT32 fColorStdMatrix3[3];
#endif
	UINT8 uColorStdForRev ;
}PQL_CM_OUT_CS_T;

/**
 * PQL Process Path.
 * Main Path(Main External Input, Main Format converter, Main Post Processing)와\n
 * Sub Path (Sub External Input, Sub Format converter, Sub Post Processing, DENC)를
 * 지정할 때 사용한다.
 * @ingroup
 */
typedef enum {
	PQL_PATH_MAIN		= 0,		/* main path */
	PQL_PATH_SUB,				/* sub path */
	PQL_PATH_VCR,				/* vcr out path */
	PQL_PATH_MAXN
} PQL_PATH_T;

/**
 * SH : Sharpness UI value struct.
 *
 * @see
 */
typedef struct PQL_SHARP_UI_VALUE{
#ifndef __KERNEL__
	float	fSharpnessUi;
#else
	UINT32	fSharpnessUi;
#endif
}PQL_SHARP_UI_VALUE_T;


/**
 * NR : NR Level Enumeration.
 *
 * @see
 */
typedef enum {
	  PQL_NR_OFF	  = 0,
	  PQL_NR_LOW,
	  PQL_NR_MEDIUM,
	  PQL_NR_HIGH
} PQL_NR_LEVEL_T;

/**
 * NR : NR Control Infomation Structure.
 *
 * @see
 */
typedef struct PQL_NR_CONTROL_INFORM
{
	UINT8 uIsProgressive;

} PQL_NR_CONTROL_INFORM_T;

#if 0
/**
 * BLACK : Black Level Source ID.
 *
 * @see
 */
typedef enum {
	PQL_BLACK_HDMI		= 0,			/**< Black for HDMI 		*/
	PQL_BLACK_COMP, 					/**< Black for Component 	*/
	PQL_BLACK_DECODER, 					/**< Black for Decoder type	*/
	PQL_BLACK_OTHERS,					/**< Black for others		*/
} PQL_BLACK_SOURCE_T;
#else
/**
 * BLACK : Black Level Source ID.
 *
 * @see
 */
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
#endif

/**
 * BLACK : Black Level Control Setting Value Enumeration.
 *
 * @see
 */
typedef enum {
	PQL_BLACK_LOW		= 0,			/**< Black Level Low	*/
	PQL_BLACK_HIGH, 					/**< Black Level High	*/
	PQL_BLACK_AUTO, 					/**< Black Level Auto	*/
} PQL_BLACK_T;

/**
 * PC : PC Control UI value Structure.
 *
 * @see
 */
typedef struct PQL_PC_UI_VALUE{
#ifndef __KERNEL__
	float contrastUi;
	float brightnessUi;
	float colorUi;
	float tintUi;
	float contrastCentreUi;
#else
	UINT32 contrastUi;
	UINT32 brightnessUi;
	UINT32 colorUi;
	UINT32 tintUi;
	UINT32 contrastCentreUi;
#endif
}PQL_PC_UI_VALUE_T;

 /**
 * Loading UI matching table val in case of EyeQ.
 *
 * @see
 */
 typedef enum {
		 PQL_EYEQ_CONTRAST	 = 0x0000,
		 PQL_EYEQ_BRIGHTNESS,
		 PQL_EYEQ_COLOR,
		 PQL_EYEQ_TINT,
		 PQL_EYEQ_SHARPNESS,
		 PQL_EYEQ_BACKLIGHT
 } PQL_EYEQ_DB_MEMBER_T;

 /**
 * Loading UI matching Offset table val in case of KIND.
 *
 * @see
 */
 typedef enum {
		 PQL_KIND_CONTRAST_OFFSET	 = 0x0000,
		 PQL_KIND_BRIGHTNESS_OFFSET,
		 PQL_KIND_COLOR_OFFSET,
		 PQL_KIND_TINT_OFFSET,
		 PQL_KIND_SHARPNESS_OFFSET
 } PQL_KIND_DB_MEMBER_T;

 /**
  * Loading DC2P DynamicContrast value parameters table val.
  *
  * @see
  */
  typedef enum {
		  PQL_DIMMING_APL	  = 0x0000,
		  PQL_DIMMING_VBRB
  } PQL_DIMMING_DB_MEMBER_T;

 /**
 * Loading DC2P DynamicContrast parameters table value.
 *
 * @see
 */
 typedef enum {
		 PQL_DC2P_LOWPEAK_POS	 = 0x0000,	 /*  1 parameter is required	 */
		 PQL_DC2P_HIGHPEAK_POS, 		 		/*  1 parameter is required	 */

		 PQL_DC2P_LOWPEAK,					 /*  2 parameter is required	 */
		 PQL_DC2P_BRIGHTNESS_OFFSET,	 		/*  2 parameter is required	 */
		 PQL_DC2P_HIGHPEAK, 			 		/*  2 parameter is required	 */
		 PQL_DC2P_CONTRAST_GAIN,		 		/*  2 parameter is required	 */
		 PQL_DC2P_DYNAMIC_COLOR_GAIN	 	/*  2 parameter is required	 */
 } PQL_DYNAMICCONTRAST_DB_MEMBER_T;

 /**
   * Loading DC2P APL parameters table val.
   *
   * @see
   */
typedef enum {
	PQL_APL_TRANSCURVE	   = 0x0000,
	PQL_APL_APLPOINT
} PQL_APL_DB_MEMBER_T;


/**
 * CM : Color Standard Setting Value Enumeration.
 *
 * @see
 */
 typedef enum {
	PQL_CS_SD=0,
	PQL_CS_HD
} PQL_CS_T;

/**
 * AFEFilter : AFEFilter Setting type Enumeration.
 *
 * @see
 */
typedef enum {
	SAFEFILTER_RF_TYPE,
	SAFEFILTER_AV_SVIDEO_TYPE,
	SAFEFILTER_SCART_RGB_COMPSD_I_TYPE,
	SAFEFILTER_COMPSD_P_TYPE,
	SAFEFILTER_COMP720_TYPE,
	SAFEFILTER_COMP1080I_TYPE,
	SAFEFILTER_COMP1080P_TYPE
}PQL_AFEF_T;

/**
 * AVDEC : Analog Video Decoder Input Mode Enumeration.
 *
 * @see
 */
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

//by hwangbos 0307
//move from pql.h, pq.h
/**
 * mre src by pql.
 *
 * @see API_MRE_AV_SOURCE_T.
*/
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
//#if( SYS_DVB_AU )	// mskn
#ifndef __KERNEL__
#if( SYS_DVB_AU || SYS_ATSC )	// totaesun
	PQL_AV_SRC_COMP2,			// Component (YPbPr) 2
#endif
#endif
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
	PQL_AV_SRC_DIGITAL_FROM_HDD,//PLAYFromHDD에서의 vcr 출력을 위한 path:ksyoon 2004.3.23. added
	#endif

	PQL_AV_SRC_NOT_CHANGED,		// Source not changed
	PQL_AV_SRC_NOT_DETERMINED	// Source is not determined

} PQL_MRE_AV_SOURCE_T;

/**
 * pql input src/fmt type.
 *
 * @see x260 각 block setting에 사용.
*/
#ifndef __KERNEL__
typedef enum {
/* PQL_INPUT_DTV		: Digital_RF + Digital_PVR 													*/
	PQL_DTV_HD_I				= 0 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Interlace : 	1080i	*/
	PQL_DTV_HD_P				= 1 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :	 720p	*/
	PQL_DTV_SD_P				= 2 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Progressive :	 480p	*/
	PQL_DTV_SD_I				= 3 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Interlace :	 480i	*/
	PQL_DTV_FHD				= 4 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :  1080p	*/ //gbtogether(070821)
	PQL_DTV_NONE0				= 5 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* HD Progressive :  720p	*/
	PQL_DTV_NONE1 			= 6 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Progressive :  576p	*/
	PQL_DTV_HD_I_DVB			= 7 + PQL_FORMAT_MAX*PQL_INPUT_DTV,		/* SD Interlace : 	 576i	*/
	PQL_DTV_HD_P_DVB			= 8 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
	PQL_DTV_SD_P_DVB			= 9 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
	PQL_DTV_SD_I_DVB			= 10 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
	PQL_DTV_FHD_DVB			= 11 + PQL_FORMAT_MAX*PQL_INPUT_DTV,	//gbtogether(070821)
	PQL_DTV_NONE2				= 12 + PQL_FORMAT_MAX*PQL_INPUT_DTV,
	PQL_DTV_NONE3				= 13 + PQL_FORMAT_MAX*PQL_INPUT_DTV,

/* PQL_INPUT_ANA		: Analog_RF/CVBS/S-video + X260_AVDC										*/
	PQL_ANA_RF					= 0 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* Analog_RF + X260_AVDC	*/
	PQL_ANA_CVBS				= 1 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* CVBS + X260_AVDC			*/
	PQL_ANA_SVID				= 2 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* S-video + X260_AVDC		*/
	PQL_ANA_SCARTRGB			= 3 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* Analog_RF + X260_AVDC	*/ //gbtogether(070821)
	PQL_ANA_NONE0				= 4 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* CVBS + X260_AVDC			*/
	PQL_ANA_NONE1				= 5 + PQL_FORMAT_MAX*PQL_INPUT_ANA,		/* S-video + X260_AVDC		*/
	PQL_ANA_NONE2				= 6 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
	PQL_ANA_RF_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
	PQL_ANA_CVBS_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
	PQL_ANA_SVID_PAL			= 9 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
	PQL_ANA_RF_SECAM			= 10 + PQL_FORMAT_MAX*PQL_INPUT_ANA,   // RF-SECAM 대응. gbtogether(091029)
	PQL_ANA_CVBS_SECAM		= 11 + PQL_FORMAT_MAX*PQL_INPUT_ANA,   // AV-SECAM 대응. gbtogether(091029)
	PQL_ANA_NONE5				= 12 + PQL_FORMAT_MAX*PQL_INPUT_ANA,
	PQL_ANA_NONE6				= 13 + PQL_FORMAT_MAX*PQL_INPUT_ANA,

/* PQL_INPUT_EANA		: Analog_RF/CVBS/S-video + uPD64015											*/
	PQL_EANA_RF				= 0 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* Analog_RF + uPD64015		*/
	PQL_EANA_CVBS				= 1 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* CVBS + uPD64015			*/
	PQL_EANA_SVID				= 2 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* S-video + uPD64015		*/
	PQL_EANA_NONE0			= 3 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* Analog_RF + uPD64015		*/
	PQL_EANA_NONE1			= 4 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* CVBS + uPD64015			*/
	PQL_EANA_NONE2			= 5 + PQL_FORMAT_MAX*PQL_INPUT_EANA,		/* S-video + uPD64015		*/
	PQL_EANA_NONE3			= 6 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_RF_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_CVBS_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_SVID_PAL			= 9 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_EANA,
	PQL_EANA_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_EANA,

/* PQL_INPUT_COMP		: Component + MST3367														*/
	PQL_COMP_FHD				= 0 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080P	*/
	PQL_COMP_HD_I				= 1 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080i	*/
	PQL_COMP_HD_P				= 2 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive : 720p	*/
	PQL_COMP_SD_P				= 3 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Progressive : 480p	*/
	PQL_COMP_SD_I				= 4 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Interlace :   480i	*/
	PQL_COMP_NONE0			= 5 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive :1080P	*/
	PQL_COMP_NONE1			= 6 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Interlace :  1080i	*/
	PQL_COMP_FHD_PAL			= 7 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* HD Progressive : 720p	*/
	PQL_COMP_HD_I_PAL			= 8 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Progressive : 576p	*/
	PQL_COMP_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_COMP,		/* SD Interlace :   576i	*/
	PQL_COMP_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
	PQL_COMP_SD_I_PAL			= 11 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
	PQL_COMP_NONE2			= 12 + PQL_FORMAT_MAX*PQL_INPUT_COMP,
	PQL_COMP_NONE3			= 13 + PQL_FORMAT_MAX*PQL_INPUT_COMP,

/* PQL_INPUT_RGBPC	: RGB_PC + MST3367															*/
	PQL_RGBPC_VGA				= 0 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 640x480					*/
	PQL_RGBPC_SVGA			= 1 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 800x600					*/
	PQL_RGBPC_XGA				= 2 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1280x768					*/
	PQL_RGBPC_WXGA			= 3 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1366x768					*/
	PQL_RGBPC_UXGA			= 4 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,		/* 1600x1200				*/
	PQL_RGBPC_FHD				= 5 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE1			= 7 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE2			= 8 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE3			= 9 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,
	PQL_RGBPC_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_RGBPC,

/* PQL_INPUT_DVIPC	: DVI_PC + MST3367															*/
	PQL_DVIPC_VGA				= 0 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 640x480					*/
	PQL_DVIPC_SVGA			= 1 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 800x600					*/
	PQL_DVIPC_XGA				= 2 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1280x768					*/
	PQL_DVIPC_WXGA			= 3 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1366x768					*/
	PQL_DVIPC_UXGA			= 4 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,		/* 1600x1200				*/
	PQL_DVIPC_FHD				= 5 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE1			= 7 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE2			= 8 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE3			= 9 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE4			= 10 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE5			= 11 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE6			= 12 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,
	PQL_DVIPC_NONE7			= 13 + PQL_FORMAT_MAX*PQL_INPUT_DVIPC,

/* PQL_INPUT_HDMIAV	: HDMI_AV + MST3367															*/
	PQL_HDMIAV_FHD			= 0 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 1080P	*/
	PQL_HDMIAV_HD_I			= 1 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Interlace : 1080i		*/
	PQL_HDMIAV_HD_P			= 2 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 720p	*/
	PQL_HDMIAV_SD_P			= 3 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Progressive : 480p	*/
	PQL_HDMIAV_SD_I			= 4 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Interlace : 480i		*/
	PQL_HDMIAV_NONE0			= 5 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 1080P	*/
	PQL_HDMIAV_NONE1			= 6 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Interlace : 1080i		*/
	PQL_HDMIAV_FHD_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* HD Progressive : 720p	*/
	PQL_HDMIAV_HD_I_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Progressive : 576p	*/
	PQL_HDMIAV_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,	/* SD Interlace : 576i		*/
	PQL_HDMIAV_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
	PQL_HDMIAV_SD_I_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
	PQL_HDMIAV_NONE2			= 12 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,
	PQL_HDMIAV_NONE3			= 13 + PQL_FORMAT_MAX*PQL_INPUT_HDMIAV,

/* PQL_INPUT_PVRANA	: PVR_Analog Source															*/
	PQL_PVRANA_EANA_RF			= 0 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* Analog_RF				*/
	PQL_PVRANA_EANA_AV			= 1 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* CVBS/S-video				*/
	PQL_PVRANA_COMP_HD			= 2 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
	PQL_PVRANA_COMP_SD			= 3 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 480p/480i				*/
	PQL_PVRANA_HDMI_HD			= 4 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
	PQL_PVRANA_HDMI_SD			= 5 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 480p/480i				*/
	PQL_PVRANA_SVID				= 6 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* Analog_S-video		*/ //gbtogether(070821)
	PQL_PVRANA_EANA_RF_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* CVBS/S-video				*/
	PQL_PVRANA_EANA_AV_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
	PQL_PVRANA_COMP_HD_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 576p/576i				*/
	PQL_PVRANA_COMP_SD_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 1080p/1080i/720p			*/
	PQL_PVRANA_HDMI_HD_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,	/* 576p/576i				*/
	PQL_PVRANA_HDMI_SD_PAL		= 12 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,
	PQL_PVRANA_SVID_PAL			= 13 + PQL_FORMAT_MAX*PQL_INPUT_PVRANA,   //gbtogether(070821)

/* ETC. */
//gbtogether(070821)
//PVR DTV & PVR ScartRGB & EPF,i-frame .....
	PQL_PVRDTV_FHD			= 0 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Progressive : 1080P	*/
	PQL_PVRDTV_HD_I			= 1 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Interlace : 1080i		*/
	PQL_PVRDTV_HD_P			= 2 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* HD Progressive : 720p	*/
	PQL_PVRDTV_SD_P			= 3 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* SD Progressive : 480p	*/
	PQL_PVRDTV_SD_I			= 4 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* SD Interlace : 480i		*/
	PQL_PVRDTV_SCARTRGB		= 5 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* Scart_RGB						*/
	PQL_PVRDTV_NONE0			= 6 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
	PQL_PVRDTV_FHD_PAL		= 7 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
	PQL_PVRDTV_HD_I_PAL		= 8 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
	PQL_PVRDTV_HD_P_PAL		= 9 + PQL_FORMAT_MAX*PQL_INPUT_ETC,		/* etc						*/
	PQL_PVRDTV_SD_P_PAL		= 10 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
	PQL_PVRDTV_SD_I_PAL		= 11 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
	PQL_EPF					= 12 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/
	PQL_IFRAME					= 13 + PQL_FORMAT_MAX*PQL_INPUT_ETC,	/* etc						*/


/* Reserved */
	PQL_RESERVED0				= 0 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Invalid					*/
	PQL_RESERVED1				= 1 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED2				= 2 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED3				= 3 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED4				= 4 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED5				= 5 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Invalid					*/
	PQL_RESERVED6				= 6 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED7				= 7 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED8				= 8 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED9				= 9 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,	/* Reserved					*/
	PQL_RESERVED10				= 10 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Invalid					*/
	PQL_RESERVED11				= 11 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
	PQL_RESERVED12				= 12 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
	PQL_RESERVED13				= 13 + PQL_FORMAT_MAX*PQL_INPUT_RESERVED,/* Reserved					*/
} PQL_INPUT_SRC_FMT_T;
#endif

/**
 * PQL Stream type.
 *
 */
typedef enum{
	PQL_SRC_PORT_TP			= 0,// Transport Stream Input Port
	PQL_SRC_PORT_INT,			// Internal Input Processing(used internal IP, NTDEC/ADC/HDMI)
	PQL_SRC_PORT_EXT,			// External Input Processing(used external IP, NTDEC/ADC/HDMI)				/*AVI 소스*/
} PQL_VIDEO_SRC_PORT_T;

/**
 * PQL Input Category(VDEC,CVD,D1, Raster Video Input:Component).
 * PQL_SRC_TYPE_VDEC,
 * Project마다 다를 수 있으며, 수정시, PQL_SRC_T도 함께 수정되어야 한다.
 *
 * 일련 번호로 구성되며, PQL_SRC_RVI_... 에 상위 8비트에 합쳐져 있으며,
 * gPQLCfg.vcfInCfg[]의 인덱스로 사용된다.
 * [주] PQL_SRC_TYPE_T 은 24개 이하여야 한다.
 *
 * @see PQL_SRC_T
 * @see PQL_CONFIG_T
 * @see PQL_IO_SPEC_T
 */
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

/**
 * PQL Input Port Type.(Video Input Port Type.참조).
 * PQL의 FC source를 설정할 때(VDEC 포함), 그 Source를 지정할 때 사용한다.
 * PQL_system_ctrl.main_src_sel, sub_src_sel을 설정하고,
 * EXT일 경우, ext_main_ctrl0.ext_type, bit_mode 를 함께 설정한다.
 * [주] PQL_SRC_RVI_... 을 선언할 때, PQL_RVI_TYPE 을 16비트 중 상위 8비트에 위치시키도록 한다.
 *
 * DDI 외부에서 사용하는 FC Source type.
 * @li PQL_SRC_NONE			: NOT connected.
 * @li PQL_SRC_VDEC_0		: VDEC 0
 * @li PQL_SRC_VDEC_1		: VDEC 1
 *
 * @li PQL_SRC_RVI_COMP		: EXT, RVI, Component
 * @li PQL_SRC_RVI_EXAV		: EXT, RVI, Tuner/S-Video Input
 * @li PQL_SRC_RVI_RGBPC	: EXT, RVI, RGB PC Category
 * @li PQL_SRC_RVI_RGBDTV	: EXT, RVI, RGB DTV Category
 * @li PQL_SRC_RVI_DVIPC	: EXT, RVI, DVI PC Category
 * @li PQL_SRC_RVI_DVIDTV	: EXT, RVI, DVI DTV Category
 *
 * @li PQL_SRC_CVD			: EXT, CVD ( COMPOSITE )
 * @li PQL_SRC_D1			: EXT, D1
 * @li PQL_SRC_MAIN_HIST	: (SFC ONLY) MFC Output, after Histogram Equalizer.
 * @li PQL_SRC_MAIN_PICQ	: (SFC ONLY) MFC Output, after Picture Quality Improvement
 * @li PQL_SRC_MAIN_OSD1	: (SFC ONLY) MFC Output, after OSD 0 Mixed
 * @li PQL_SRC_MAIN_OSD2	: (SFC ONLY) MFC Output, after OSD 1 Mixed
 * @li PQL_SRC_MAIN_OSD3	: (SFC ONLY) MFC Output, after OSD 2 Mixed
 * @li PQL_SRC_MAIN_CSC		: (SFC ONLY) MFC Output, after CSC(Color Space Conversion)
 *
 * DDI 내부에서 사용하는 FC Source MASK.
 * @li PQL_SRC_MSK_VID		: for PQL_SRC_T to w0_vid, w1_vid,
 * @li PQL_SRC_MSK_REG		: for PQL_SRC_T to main/sub_src_sel field
 * @li PQL_SRC_MSK_VDEC		: for VDEC    Group
 * @li PQL_SRC_MSK_RVI		: for RVI     Group
 * @li PQL_SRC_MSK_EXT		: for EXT     Group (CVD,D1,RVI)
 *
 * 입력 parameter range check용. (PQL_SRC_MSK_REG에 해당 되는 부분만 검사.)
 * @li PQL_SRC_MIN			: main/sub 공통의 최소값.
 * @li PQL_SRC_MAIN_MAX 	: main의 최대값.
 * @li PQL_SRC_SUB_MAX		: sub의 최대값.
 *
 * @see
 */
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
//#if( SYS_DVB_AU ) // mskn
#ifndef __KERNEL__
#if( SYS_DVB_AU || SYS_ATSC )	// totaesun
	PQL_SRC_CVBS2		 	= 0x06	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 127
	PQL_SRC_SVIDEO2		 	= 0x07	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_AVD	<< 8),	// 128
	PQL_SRC_COMP2			= 0x01	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_COMP<< 8),	// 221
#endif
#endif
	PQL_SRC_COMP			= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_COMP<< 8),	// 220
	PQL_SRC_VGA				= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_VGA	<< 8),	// 320
	PQL_SRC_HDMI1			= 0x00	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_HDMI<< 8),	// 420
	PQL_SRC_HDMI2			= 0x01	| PQL_SRC_MSK_APORT	| (PQL_SRC_TYPE_HDMI<< 8),	// 421

	PQL_SRC_NO_CHANGE		= 0xffffffff

} PQL_VIDEO_SRC_T;


 /**
 * Define width and height for In/Out Size.
 *
 * @see
 */
typedef struct PQL_SIZE{
	UINT16	inWidth;				/*input width*/
	UINT16	inHeight;				/*input height*/
	UINT16	outWidth;				/*output width*/
	UINT16	outHeight;				/*output height*/
}PQL_SIZE_T;

/**
* pql status.
*
* @see
*/
#ifndef __KERNEL__
typedef struct PQL_STATUS{
	BOOLEAN					pqlPath[PQL_PATH_MAXN];					/* path selection */
	PQL_INPUT_SRC_FMT_T		pqlSrcFmt[PQL_PATH_MAXN];				/* src and fmt */
	PQL_SIZE_T				pqlSize[PQL_PATH_MAXN];					/* size*/
}PQL_STATUS_T;
#endif

/**
*	Coefficient ID 선택위함.
*	Coefficient ID 추가시 동시에 추가.
*	EZ Picture			: 4EA		-> See PC Index
*	XD_Contrast			: 1EA		-> See DC Index
*	XD_Color			: 8EA		-> See CM Index
*	Sharpness/XD_Noise	: 18EA		-> See Sharp Index
*	BackLight_Control	: 1EA		-> See PC Index
*	Total				: 32EA
*
* @see
*/
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

/**
*   cPQ_ModelName -> 화질 설정 DB 모델명 정의 : In-Start Menu에서 Display.
*       ModelName은 Max 15글자로 제한.
*   PQ Version -> 화질 설정 버젼 + 화질라이브러리 소프트웨어 버젼
*       ex) 01.60.10 => cPQ_DbVersion : 1.6, cPQ_DrvVersion : 0.10
*   cPQ_DbVersion -> 화질 설정 Database version
*       0.x : DV, 1.x : PV, 2.x : PQ, 3.x ~ : MP
*   cPQ_DrvVersion -> 화질 Driver version
*       0.x : DV, 1.x : PV, 2.x : PQ, 3.x ~ : MP
*       X.xx : X - major change, xx - minor change
*       만약 major change가 10회 이상일경우 맨앞 숫자를 사용한다.
*       따라서, 총 99번의 major change를 표시할 수 있다.(숫자단위)
*
* @see
*/
typedef struct {

    char    cPQ_ModelName[15];      /**< model name*/
    char    cPQ_DbVersion[4];           /**< DB ver.*/
    char    cPQ_DrvVersion[4];      /**< DVR ver.*/

} PQL_VERSION_T;

/**
*	Default Data of Global Variables when White Balance Adjustment.
*	White Balance 조정 시 HD2.4 Video 내부 패턴을 이용한다.
*	이때 아래의 변수 설정을 통해
*	Gain 조정을 위해서는 High Luminance Level을 설정하고,
*	Offset 조정을 위해서는 Low Luminance Level을 설정한다.
*	HD2.4 Contrast Value = 0~12~255 (0~1.0~1.99)
*
* @see
*/
typedef struct {

	UINT8	gLuma_WBGainAdj;
	UINT8	gLuma_WBOffsetAdj;

} PQL_LUMA_WB_T;

/**
 * Getting coefficient ID and Max value from pqldb, that is not dependent on the chip specific algorithm.
 *
 * @see .
 */
typedef struct PQL_COEFF_MEMBER {

    UINT8    uPqlCoeffID;      /**< model name*/
    UINT8    uPqlCoeffIDMax;          /**< DB ver.*/

} PQL_COEFF_MEMBER_T;

#define	MAX_CHIP_BIN_SIZE	64
#define	MAX_CHIP_NUM_TRANSCURVE		16

/**
*	pql DC2 parameter.
*
* @see
*/
typedef struct PQL_DC2P_RAWDATA_INFO {
	UINT32		nSupportDataType;
	UINT32		nBinNum;
#ifndef __KERNEL__
	SINT32		nByPassCurveTable[MAX_CHIP_NUM_TRANSCURVE];		//KWON_0716
	SINT32		nBinStep;
#else
	UINT32		nByPassCurveTable[MAX_CHIP_NUM_TRANSCURVE];		//KWON_0716
	UINT32		nBinStep;
#endif
	UINT32		uCountBins[MAX_CHIP_BIN_SIZE];
	UINT32		uAPL;
#ifndef __KERNEL__
	SINT32		nMin;
	SINT32		nMax;
	SINT32		nPeakLow;
	SINT32		nPeakHigh;
#else
	UINT32		nMin;
	UINT32		nMax;
	UINT32		nPeakLow;
	UINT32		nPeakHigh;
#endif
} PQL_DC2P_RAWDATA_INFO_T;

#ifndef __KERNEL__

/*******************************************************************************
	Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
*******************************************************************************/


/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
/* LG DDI -------------------------------------------------------------------*/
/* DB    */
//void DDI_PQL_InitPicData(PQL_CONFIG_T tempPql);
//void DDI_PQL_InitDB(PQL_CONFIG_T tempPql);
//void DDI_PQL_InitDBIndex(PQL_CONFIG_T tempPql);
#ifdef PQL_OLD_MODEL_DEF
EXTERN void DDI_PQL_GetDftInitWBData(PQL_DISPLAY_TYPE_T uDisplayModuleSel, UINT8 uRegionSel, UINT8 uModelSel, PQL_WB_DATA_T *pAdjWBdata);
#else
EXTERN void DDI_PQL_GetDftInitWBData(DISPLAY_TYPE_T uDisplayModuleSel, UINT8 uRegionSel, UINT8 uModelSel, PQL_WB_DATA_T *pAdjWBdata);
#endif
EXTERN PQL_VERSION_T *DDI_PQL_GetPQVersion(void);
EXTERN PQL_WB_DATA_T *DDI_PQL_GetDftWBData(void);
EXTERN UINT8 DDI_PQL_GetDftSubBrightness(void);
EXTERN UINT8 DDI_PQL_GetDftSubContrast(void);
EXTERN DTV_STATUS_T DDI_PQL_GetDBCMCSMeasureData(PQL_CS_T pqlCsSelect, UINT8 uCSCoeffId, void *pqlMeasureCS);
EXTERN UINT8 DDI_PQL_GetDBEyeQOSDValue( PQL_EYEQ_DB_MEMBER_T memberID, UINT8 index, UINT16 uEyeQStep, UINT8 uKindStep);
EXTERN float DDI_PQL_GetDBKindOSDOffset( PQL_KIND_DB_MEMBER_T memberID, UINT8 index, UINT8 uKindStep);
EXTERN SINT32 DDI_PQL_GetDBDimmingCtrl(PQL_DIMMING_DB_MEMBER_T  memberID, UINT8 index, UINT8 uCurveNum);
EXTERN SINT32 DDI_PQL_GetDBDC2PDynamicContastCtrl(PQL_DYNAMICCONTRAST_DB_MEMBER_T memberID, UINT8 index, UINT8 uCurveNum);
EXTERN SINT32 DDI_PQL_GetDBDC2PApl(PQL_APL_DB_MEMBER_T memberID, UINT8 type, UINT8 index, UINT8 uCurveNum, UINT8 uFreePointNum);

EXTERN PQL_COEFF_MEMBER_T DDI_PQL_GetCoeffParam(UINT32 pqlIdCoeff, PQL_INPUT_SRC_FMT_T pqlSrcFmt);
EXTERN void DDI_PQL_GetBrightContFor10pWB( PQL_PC_UI_VALUE_T pcUiVal, UINT8 pcId, float *fContrast, float *fBrightness );

EXTERN void	DDI_PQL_DebugMenu(void);


/* Color */
/* NONE */

/* CT */
EXTERN DTV_STATUS_T DDI_PQL_SetFreshWhiteData(UINT32 uAdvanceUiValue);
EXTERN DTV_STATUS_T DDI_PQL_GetCorrectValOfWBGainOffset(UINT8 index, PQL_WB_CORRECT_DATA_T *pWBCorrectVal);

/* Gamma */
/* NONE */

/* SH */
EXTERN DTV_STATUS_T DDI_PQL_SetSharpBySystem(PQL_STATUS_T pqlStatus);
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByUserControl(PQL_SHARP_UI_VALUE_T	sPqUiVal, PQL_STATUS_T pqlStatus);

/* Scaling Filter */
/* NONE */

/* NR */
EXTERN DTV_STATUS_T DDI_PQL_SetNRbySystem(PQL_STATUS_T pqlStatus);
EXTERN DTV_STATUS_T DDI_PQL_SetNRbyUserControl(PQL_STATUS_T pqlStatus, PQL_NR_LEVEL_T pqlNrLevel);

/* PC */
/* NONE */

/* DC */
/* NONE */

/* Deinterlace */
/* NONE */

/* Blacklevel */
/* NONE */

/* etc */
/*//etc mw body 함수가 되는 것이 나을 것 같음.  */
EXTERN UINT32 DDI_PQL_CheckSVideoConnection(PQL_PATH_T pqlPath, PQL_MRE_AV_SOURCE_T mreMainSrc);

/* SOC DDI -------------------------------------------------------------------*/
/* Color */
EXTERN DTV_STATUS_T DDI_PQL_SetFreshColor(UINT8 *pPqlCmCoeffId, PQL_PATH_T pqlPath, UINT8 uiFreshColorValue);
EXTERN DTV_STATUS_T DDI_PQL_SetColorStandard(PQL_CM_OUT_CS_T *pqlOutCSData, UINT8 userColStd);
EXTERN DTV_STATUS_T DDI_PQL_SetExpColorParam(UINT8 pqlCMCtrl, PQL_CM_EXPCOLOR_PARAM_T pqlExpColorParam);

/* CT */
EXTERN DTV_STATUS_T DDI_PQL_SetColorTemp(PQL_COLOR_TEMP_T cscPcCtData);

/* Gamma */
#ifdef INCLUDE_10POINTS
EXTERN DTV_STATUS_T DDI_PQL_SetGamma(PQL_PATH_T pqlPath, UINT32 *pRedGammaTable, UINT32 *pGreenGammaTable, UINT32 *pBlueGammaTable);
#else
EXTERN DTV_STATUS_T DDI_PQL_SetGamma(PQL_PATH_T pqlPath, UINT8 uiGamma, UINT8 uCmCoeffId);
#endif
/* SH */
#ifdef INCLUDE_NEW_DDI
EXTERN DTV_STATUS_T DDI_PQL_SetChipSharpByUserControl(PQL_SHARP_UI_VALUE_T sPqUiVal, PQL_STATUS_T pqlStatus,UINT8 *pShCoeffId);
#else
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByShootGain(UINT16 *pPqlShootGainVal, PQL_SHARP_UI_VALUE_T sPqUiVal);
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByChipSpecificAlgorithm(UINT8 *pPqlChipSpecifId , PQL_PATH_T pqlPath);
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByLTI(UINT8 *pPqlLTICoeffId, PQL_PATH_T pqlPath);
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByCTI(UINT8 *pPqCTICoeffId, PQL_PATH_T pqlPath);
EXTERN DTV_STATUS_T DDI_PQL_SetSharpByEdgeEnhance(UINT8 pPqEECoeffId, PQL_PATH_T pqlPath);
EXTERN DTV_STATUS_T DDI_PQL_CtrlSharpByScalingFilter(UINT8 upqlCoeffId, PQL_PATH_T pqlPath);
#endif

/* Scaling Filter */
EXTERN DTV_STATUS_T DDI_PQL_InitMainScalerFilterID(void);
EXTERN DTV_STATUS_T DDI_PQL_InitSubScalerFilterID(void);

/* NR */
EXTERN DTV_STATUS_T DDI_PQL_SetTNRParam(PQL_PATH_T pqlPath, UINT8 nTNRCoeffId, PQL_NR_LEVEL_T nrLevel, PQL_NR_CONTROL_INFORM_T *pNRControlinform);
EXTERN DTV_STATUS_T DDI_PQL_SetSpatialNRParam(UINT8  nPqSNRCoeffId, PQL_PATH_T pqlPath);
//--> 삭제 void DDI_PQL_CtrlXdNoise(UINT8 uiSharpVal, UINT8 pqlPath, UINT8 onOff, UINT8 defaultOnOff);
EXTERN DTV_STATUS_T DDI_PQL_SetMpegNRParam(UINT8 uNRCoeffId, PQL_PATH_T pqlPath);

/* PC */
EXTERN void DDI_PQL_GetAdjSubBrightCont( PQL_SUB_CONTBRI_T *pAdjSubBriCont );
EXTERN void DDI_PQL_SetAdjSubBrightCont( PQL_SUB_CONTBRI_T adjSubBriCont );
EXTERN DTV_STATUS_T DDI_PQL_SetPC(PQL_PC_UI_VALUE_T sPcUiVal, UINT8  sPqCoeffId, PQL_PATH_T pqlPath);

/* DC */
EXTERN DTV_STATUS_T DDI_PQL_SetDCFreshContrast(UINT8 nIsEnable);
EXTERN DTV_STATUS_T DDI_PQL_SetDCFreshContrastLUT(SINT32	*pAccLUT);
EXTERN DTV_STATUS_T DDI_PQL_GetDCChipRawData(PQL_DC2P_RAWDATA_INFO_T *pHistoInfo);
EXTERN DTV_STATUS_T  DDI_PQL_GetDCChipDataTypeInfo(UINT32 *pChipDataType);


/* Deinterlace */
EXTERN DTV_STATUS_T DDI_PQL_SetFilmMode(PQL_PATH_T  pqlPath, UINT8 filmOnOff);
EXTERN DTV_STATUS_T DDI_PQL_SetDeinterParam(UINT8 uMDCoeffId, PQL_PATH_T pqlPath);
EXTERN DTV_STATUS_T DDI_PQL_SetDCDynamicColorVal(PQL_PATH_T pqlPath, SINT32 pcDynColorVal);

/* Blacklevel */
EXTERN DTV_STATUS_T DDI_PQL_SetBlackLevel( PQL_BLACK_SOURCE_T eSource, PQL_BLACK_T blackLevelType, UINT8 nHDMIIsDTV);

/* etc */
EXTERN PQL_AVDECODER_VIDEO_MODE_T DDI_PQL_GetColorSystem(void);
EXTERN DTV_STATUS_T DDI_PQL_SetSCARTClamp(UINT8 nScartRGB);
EXTERN DTV_STATUS_T DDI_PQL_SetAFEFilter(UINT16 uFilterSourceType, UINT8 uCoeffiId);
EXTERN DTV_STATUS_T DDI_PQL_CtrlDecoderParam(UINT16 nIsSvideoType);

/* Blue Only */
EXTERN DTV_STATUS_T DDI_PQL_SetBlueOnly(UINT8 uPqBlueOnlyOnOff);

#endif

#endif	/* __PQL_DDI_H_ */

