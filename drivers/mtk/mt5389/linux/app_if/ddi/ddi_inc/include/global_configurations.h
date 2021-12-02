/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   Permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 	global_configurations.h
 *
 *  Definitions for global configurations
 *  DESCRIPTION : In this file, all global configurations, enumerations, data
			   or structures should be shared between modules positioned in
			   different layers.
			   For example, enumeration for external video supported in the system,
			   should be referenced in User Interface module (APP layer) and,
			   at the same time, referenced Video module(Driver Layer).
			   So, these kind of definition or declaration should be stated
			   here.
 *  @author	Jackee, Lee(이재길, jackee@lge.com)
 *  @author  	Lee,illo(b215@lge.com) - modifier
 *  @version 	1.1
 *  @date	2007.12.05
 *  @date	2008.04.07
 *  @see
 */

#ifndef	_GLOBAL_CONFIGS_H_
#define	_GLOBAL_CONFIGS_H_

/*-----------------------------------------------------------------------------
	제어 상수
	(Control Constants)
------------------------------------------------------------------------------*/
#define VBI_SLICER_CHANGE // 임시 define drmyung 080526
/*-----------------------------------------------------------------------------
	#include 파일들
	(File Inclusions)
------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	매크로 함수  정의
	(Macros Definitions)
------------------------------------------------------------------------------*/
#define VRES_WIDTH(_v)			((_v & 0xfff) << 12   )
#define VRES_HEIGHT(_v)			((_v & 0xfff)         )
#define VRES_WIDTH_PART(_v)		((_v & 0xfff000) >> 12)
#define VRES_PROG				( 0x80000000          )	/* Progresive */
#define VRES_INTL				( 0x00000000          )	/* Interlaced */

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/
//
// Global Display Resolution Constant Table
//
#define VRES_1920X1088			( VRES_WIDTH(1920) | VRES_HEIGHT(1088) ) // ATSC 1920X1088 format
#define VRES_1920X1080			( VRES_WIDTH(1920) | VRES_HEIGHT(1080) )
#define VRES_1440X1088			( VRES_WIDTH(1440) | VRES_HEIGHT(1088) )
#define VRES_1440X1080			( VRES_WIDTH(1440) | VRES_HEIGHT(1080) )
#define VRES_1440X_480			( VRES_WIDTH(1440) | VRES_HEIGHT( 480) )
#define VRES_1428X_480			( VRES_WIDTH(1428) | VRES_HEIGHT( 480) ) // for composite double-sampling
#define VRES_1418X_576			( VRES_WIDTH(1418) | VRES_HEIGHT( 576) ) //         "           "
#define VRES_1397X_576			( VRES_WIDTH(1397) | VRES_HEIGHT( 576) ) //         "           "
#define VRES_1397X_480			( VRES_WIDTH(1397) | VRES_HEIGHT( 480) ) //         "           "
#define VRES_1387X_576			( VRES_WIDTH(1387) | VRES_HEIGHT( 576) ) //         "           "
#define VRES_1387X_480			( VRES_WIDTH(1387) | VRES_HEIGHT( 480) ) //         "           "
#define VRES_1280X1088			( VRES_WIDTH(1280) | VRES_HEIGHT(1088) )
#define VRES_1280X1080			( VRES_WIDTH(1280) | VRES_HEIGHT(1080) )
#define VRES_1280X1152			( VRES_WIDTH(1280) | VRES_HEIGHT(1152) )
#define VRES_1280X_720			( VRES_WIDTH(1280) | VRES_HEIGHT( 720) )
#define VRES__960X_576			( VRES_WIDTH( 960) | VRES_HEIGHT( 576) )
#define VRES__768X_576			( VRES_WIDTH( 768) | VRES_HEIGHT( 576) )
#define VRES__720X1152			( VRES_WIDTH( 720) | VRES_HEIGHT(1152) )
#define VRES__720X_576			( VRES_WIDTH( 720) | VRES_HEIGHT( 576) )
#define VRES__720X_480			( VRES_WIDTH( 720) | VRES_HEIGHT( 480) )
#define VRES__720X_483			( VRES_WIDTH( 720) | VRES_HEIGHT( 483) ) //lachesis_080827 VG-828D No.978 483P
#define VRES__720X_240			( VRES_WIDTH( 720) | VRES_HEIGHT( 240) )
#define VRES__704X_576			( VRES_WIDTH( 704) | VRES_HEIGHT( 576) )
#define VRES__704X_480			( VRES_WIDTH( 704) | VRES_HEIGHT( 480) )
#define VRES__640X_576			( VRES_WIDTH( 640) | VRES_HEIGHT( 576) )
#define VRES__528X_576			( VRES_WIDTH( 528) | VRES_HEIGHT( 576) )
#define VRES__528X_480			( VRES_WIDTH( 528) | VRES_HEIGHT( 480) )    // totaesun 080725, for ATSC
#define VRES__544X_576			( VRES_WIDTH( 544) | VRES_HEIGHT( 576) )
#define VRES__480X_576			( VRES_WIDTH( 480) | VRES_HEIGHT( 576) )
#define VRES__352X_576			( VRES_WIDTH( 352) | VRES_HEIGHT( 576) )
#define VRES__352X_288			( VRES_WIDTH( 352) | VRES_HEIGHT( 288) )
#define VRES__320X_240			( VRES_WIDTH( 320) | VRES_HEIGHT( 240) )	// jmkim..BBCi Drip
#define VRES__640X_480			( VRES_WIDTH( 640) | VRES_HEIGHT( 480) )
#define VRES__640X_400			( VRES_WIDTH( 640) | VRES_HEIGHT( 400) )
#define VRES__640X_350			( VRES_WIDTH( 640) | VRES_HEIGHT( 350) )
#define VRES__720X_350			( VRES_WIDTH( 720) | VRES_HEIGHT( 350) )
#define VRES__720X_400			( VRES_WIDTH( 720) | VRES_HEIGHT( 400) )
#define VRES__800X_600			( VRES_WIDTH( 800) | VRES_HEIGHT( 600) )
#define VRES__832X_624			( VRES_WIDTH( 832) | VRES_HEIGHT( 624) )
#define VRES__848X_480			( VRES_WIDTH( 848) | VRES_HEIGHT( 480) )
#define VRES__852X_480			( VRES_WIDTH( 852) | VRES_HEIGHT( 480) ) // only for display out
#define VRES_1024X_768			( VRES_WIDTH(1024) | VRES_HEIGHT( 768) )
#define VRES_1024X_720			( VRES_WIDTH(1024) | VRES_HEIGHT( 720) ) // only for display out
#define VRES_1152X_864			( VRES_WIDTH(1152) | VRES_HEIGHT( 864) )
#define VRES_1152X_870			( VRES_WIDTH(1152) | VRES_HEIGHT( 870) )
#define VRES_1152X_900			( VRES_WIDTH(1152) | VRES_HEIGHT( 900) )
#define VRES_1280X_768			( VRES_WIDTH(1280) | VRES_HEIGHT( 768) )
#define VRES_1280X_960			( VRES_WIDTH(1280) | VRES_HEIGHT( 960) )
#define VRES_1280X1024			( VRES_WIDTH(1280) | VRES_HEIGHT(1024) )
#define VRES_1360X_768			( VRES_WIDTH(1360) | VRES_HEIGHT( 768) )
#define VRES_1365X_768			( VRES_WIDTH(1365) | VRES_HEIGHT( 768) ) // only for display out
#define VRES_1366X_768			( VRES_WIDTH(1366) | VRES_HEIGHT( 768) )
#define VRES_1376X_776			( VRES_WIDTH(1376) | VRES_HEIGHT( 776) ) // only for display out
#define VRES_1400X1050			( VRES_WIDTH(1400) | VRES_HEIGHT(1050) )	// 20080531 swwoo FHD Model only
#define VRES_1440X1050			( VRES_WIDTH(1440) | VRES_HEIGHT(1050) )
#define VRES_1440X_576			( VRES_WIDTH(1440) | VRES_HEIGHT( 576) )
#define VRES_1600X1200			( VRES_WIDTH(1600) | VRES_HEIGHT(1200) )
#define VRES_1920X1200			( VRES_WIDTH(1920) | VRES_HEIGHT(1200) )
#define VRES__960X_540			( VRES_WIDTH( 960) | VRES_HEIGHT( 540) ) // only for osd resolution
#define VRES_FMT_INVALID		( VRES_WIDTH(0xffe)| VRES_HEIGHT(0xffe))
#define VRES_FMT_NOSIGNAL		( VRES_WIDTH(0)    | VRES_HEIGHT(0)    )
#define VRES__END_MARK			( VRES_WIDTH(0xfff)| VRES_HEIGHT(0xfff))


/* Macro moved from micom_ddi.h	*/
#define MICOM_SIZE_OF_POWER_OFF_DATA	(128)


/*-----------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/
/**
 * Analog Audio 음성다중모드 current State Definition.
 *
 */
typedef enum
{
	ATV_GET_PAL_MONO				=	0x00,	// PAL Mono
	ATV_GET_PAL_STEREO				=	0x01,	// PAL Stereo
	ATV_GET_PAL_DUAL				=	0x02,	// PAL Dual
	ATV_GET_PAL_NICAM_MONO			=	0x03,	// PAL NICAM Mono
	ATV_GET_PAL_NICAM_STEREO		=	0x04,	// PAL NICAM Stereo
	ATV_GET_PAL_NICAM_DUAL			=	0x05,	// PAL NICAM Dual
	ATV_GET_PAL_UNKNOWN				=	0x06,	// PAL Unkown State
	ATV_GET_NTSC_A2_MONO			=	0x10,	// NTSC(A2) Mono
	ATV_GET_NTSC_A2_STEREO			=	0x11,	// NTSC(A2) Stereo
	ATV_GET_NTSC_A2_SAP				=	0x12,	// NTSC(A2) SAP
	ATV_GET_NTSC_A2_UNKNOWN			=	0x13,	// NTSC(A2) Unkown State
	ATV_GET_NTSC_BTSC_MONO			=	0x20,	// NTSC(BTSC) Mono
	ATV_GET_NTSC_BTSC_STEREO		=	0x21,	// NTSC(BTSC) Stereo
	ATV_GET_NTSC_BTSC_SAP_MONO		=	0x22,	// NTSC(BTSC) SAP Mono
	ATV_GET_NTSC_BTSC_SAP_STEREO	=	0x23,	// NTSC(BTSC) SAP Stereo
	ATV_GET_NTSC_BTSC_UNKNOWN		=	0x24,	// NTSC(BTSC) Unkown State
} ATV_AUDIO_MODE_GET_T;

/**
 * Analog Audio 음성다중모드 Setting Parameter Definition.
 *
 */
typedef enum
{
	ATV_SET_PAL_MONO				=	0x00,	// PAL Mono
	ATV_SET_PAL_MONO_FORCED			=	0x01,	// PAL Mono Force Mono
	ATV_SET_PAL_STEREO				=	0x02,	// PAL Stereo
	ATV_SET_PAL_STEREO_FORCED		=	0x03,	// PAL Stereo Force Mono
	ATV_SET_PAL_DUALI				=	0x04,	// PAL Dual I
	ATV_SET_PAL_DUALII				=	0x05,	// PAL Dual II
	ATV_SET_PAL_DUALI_II			=	0x06,	// PAL Dual I+II
	ATV_SET_PAL_NICAM_MONO			=	0x07,	// PAL NICAM Mono
	ATV_SET_PAL_NICAM_MONO_FORCED	=	0x08,	// PAL NICAM Mono Force Mono
	ATV_SET_PAL_NICAM_STEREO		=	0x09,	// PAL NICAM Stereo
	ATV_SET_PAL_NICAM_STEREO_FORCED	=	0x0A,	// PAL NICAM Stereo Force Mono
	ATV_SET_PAL_NICAM_DUALI			=	0x0B,	// PAL NICAM Dual I
	ATV_SET_PAL_NICAM_DUALII		=	0x0C,	// PAL NICAM Dual II
	ATV_SET_PAL_NICAM_DUALI_II		=	0x0D,	// PAL NICAM Dual I+II
	ATV_SET_PAL_NICAM_DUAL_FORCED	=	0x0E,	// PAL NICAM Dual Forced Mono(Not Supported)
	ATV_SET_PAL_UNKNOWN				=	0x0F,	// PAL Unkown State
	ATV_SET_NTSC_A2_MONO			=	0x10,	// NTSC(A2) Mono
	ATV_SET_NTSC_A2_STEREO			=	0x11,	// NTSC(A2) Stereo
	ATV_SET_NTSC_A2_SAP				=	0x12,	// NTSC(A2) SAP
	ATV_SET_NTSC_A2_UNKNOWN			=	0x13,	// NTSC(A2) Unkown State
	ATV_SET_NTSC_BTSC_MONO			=	0x20,	// NTSC(BTSC) Mono
	ATV_SET_NTSC_BTSC_STEREO		=	0x21,	// NTSC(BTSC) Stereo
	ATV_SET_NTSC_BTSC_SAP_MONO		=	0x22,	// NTSC(BTSC) SAP Mono
	ATV_SET_NTSC_BTSC_SAP_STEREO	=	0x23,	// NTSC(BTSC) SAP Stereo
	ATV_SET_NTSC_BTSC_UNKNOWN		=	0x24,	// NTSC(BTSC) Unkown State
} ATV_AUDIO_MODE_SET_T;

/**
 * Audio Type Definition
 *
 */
typedef  enum  AUDIO_TYPE
{
	AUDIO_SRC_TYPE_UNKNOWN = 0,
	AUDIO_SRC_TYPE_PCM,
	AUDIO_SRC_TYPE_AC3,
	AUDIO_SRC_TYPE_EAC3,
	AUDIO_SRC_TYPE_MPEG,
	AUDIO_SRC_TYPE_AAC,
	AUDIO_SRC_TYPE_HEAAC,
	AUDIO_SRC_TYPE_MP3,
	AUDIO_SRC_TYPE_DTS,
	AUDIO_SRC_TYPE_DEFAULT
}  AUDIO_SRC_TYPE_T ;



/**
 * FRC MC type
 *
 */
typedef enum {
	FRC_MC_TYPE_OFF = 0,
	FRC_MC_TYPE_LOW,
	FRC_MC_TYPE_HIGH,
	FRC_MC_TYPE_FILMRATE,
} FRC_MC_TYPE_T;



/**
 * Power off Mode.
 *
 * @see
 */
typedef enum
{
	/* Micom 자체원인 */
	POWER_OFF_BY_CPUCMD				= 0x00,		/**< CPU Command*/
	POWER_OFF_BY_ABN				= 0x01,		/**< */
	POWER_OFF_BY_KEYTIMEOUT			= 0x02,		/**< 일정시간 동안 Power off 하지 못했을때 강제 Power off*/
	POWER_OFF_BY_ACDET				= 0x03,		/**< AC Detec 하지 못했을때 Power off : 비정상적인 경우*/
	POWER_OFF_BY_RESET				= 0x04,		/**< Micom Reset에 의한 Power off */
	POWER_OFF_BY_5VMNT				= 0x05,		/**< 5V MNT를 Detec 하지 못했을때 Power off : 비정상적인 경우*/
	POWER_OFF_BY_NO_POLLING			= 0x06,

	/* CPU 원인 */
	POWER_OFF_BY_REMOTE_KEY 		= 0x10,		/**< 리모콘 Power fof*/
	POWER_OFF_BY_OFF_TIMER			= 0x11,		/**< off Timer에 의한 Power off*/
	POWER_OFF_BY_SLEEP_TIMER		= 0x12,		/**< Sleep Timer에 의한 Power off*/
#ifndef INCLUDE_DVR_READY
	POWER_OFF_BY_ABNORMAL1			= 0x13,		/**< */
#else
	POWER_OFF_BY_TS_END				= 0x13, 	/* TS종료 및 DVR Play 종료후에 Power off*/
#endif
	POWER_OFF_BY_FAN_CONTROL		= 0x14,		/**< Fan Control에 의한 Power of*/
	POWER_OFF_BY_INSTOP_KEY 		= 0x15,		/**< Instop Key에 의한 Power off*/
	POWER_OFF_BY_AUTO_OFF			= 0x16,		/**< 자동 꺼짐 기능에 의한 Power off */
	POWER_OFF_BY_ON_TIMER			= 0x17,		/**< */
	POWER_OFF_BY_RS232C 			= 0x18,		/**< Rs232C에 의한 Power off*/
	POWER_OFF_BY_RESREC 			= 0x19,		/**< 예약녹화에 의한 Power off*/
	POWER_OFF_BY_RECEND 			= 0x1A,		/**< 녹화 종료에 의한 Power off*/
	POWER_OFF_BY_SWDOWN 			= 0x1B,		/**< Software Downlod후 Power off */
	POWER_OFF_BY_LOCAL_KEY			= 0x1C,		/**< Gemstar Error*/
	POWER_OFF_BY_CPU_ABNORMAL		= 0x1D,		/**< CPU Abnormal 상태에 의한 Power off*/
	POWER_OFF_BY_INV_ERROR			= 0x1E,		/**< LCD Tornado Module Inverter 동작 Error*/
//	POWER_OFF_BY_HOMING_COMPLETE	= 0x1E,		/**< 북미향 Cable Card Update후 Power off*/
	POWER_OFF_BY_OTA				= 0x1F,		/**< OTA update 종료에 의한 Power off*/
	POWER_OFF_BY_UNKNOWN			= 0xFF 		/**< 기타 */

}	DDI_POWER_OFF_MODE_T;


/**
 * Power off Mode.
 *
 * @see
 */
typedef enum
{
	POWER_ON_BY_REMOTE_KEY			= 0x20,		/**< Remocon에 의한 Power on*/
	POWER_ON_BY_ON_TIMER			= 0x21,		/**< On Time에 의한 Power on */
	POWER_ON_BY_LOCAL_KEY			= 0x22,		/**< Local Key에 의한 Power on*/
	POWER_ON_BY_RESERVE 			= 0x23,		/**< 예약 시간에 의한 Power on  */
	POWER_ON_BY_RS232C				= 0x24,		/**< Rs232C Command에 의한 Power on*/
	POWER_ON_BY_POWER_ONLY			= 0x25,		/**< Power Only에 의한 Power on*/
	POWER_ON_BY_REMIND 				= 0x26,		/**< 예약 시청에 의한 Power on */
	POWER_ON_BY_SWDOWN				= 0x27,		/**< Software Download후 Power on*/
	POWER_ON_BY_LAST_COLD			= 0x28,		/**< 이전 COLD(standby) 상태에서 Plug off, Plug on을 통한 Power on */
	POWER_ON_BY_LAST_COOL			= 0x29,		/**< 이전 COOL 상태에서 Plug off, Plug on을 통한 Power on - 사용안함 삭제 예정*/
	POWER_ON_BY_LAST_WARM			= 0x2A,		/**< 이전 WARM(active standby) 상태에서 Plug off, Plug on을 통한 Power on*/
	POWER_ON_BY_LAST_POWERON		= 0x2B,		/**< 이전 ON(running) 상태에서 Plug off, Plug on을 통한 Power on*/
	POWER_ON_BY_OTA					= 0x2C,		/**< OTA 예약에 의해서 Power on 됨(기존 POWER_ON_BY_COOL위치 )*/
	POWER_ON_BY_CPU_ABNORMAL		= 0x2D,		/**< CPU Abnormal*/
	POWER_ON_BY_HOMING				= 0x2E,		/**< 북미향 Cable Card에 의한 Power on */
	POWER_ON_BY_HDMI_CEC			= 0x2F,		/**< HDMI CEC Message "image view on"에 의한 Power on*/

	POWER_ON_BY_UNKNOWN 			= 0xFF		/**< 기타 */

}	DDI_POWER_ON_MODE_T;


/**
 * Power State.
 *
 * @see	DDI_MICOM_GetPowerState (DDI_POWER_STATE_T *pPowerState)
 */
typedef enum
{
	POWER_STATE_COLD_STANDBY	= 0, 	/**< Micom에만 전원 공급, CPU, Module은 공급 되지 않는 상태  */
	POWER_STATE_COOL_STANDBY	= 1, 	/**< Cool 상태 북미향 only*/
	POWER_STATE_WARM_STANDBY	= 2,	/**< Micom,CPU에 전원공급 , Module은 공급되지 않는 상태 (예약녹화시 사용)*/
	POWER_STATE_ACTIVE 			= 3,	/**< 일반적인 Power on 동작 상태 */
	POWER_STATE_BOOT			= 4,	/**< System Booting 중일때*/
	POWER_STATE_SWDOWN			= 5,	/**< Software Download 상태 */
	POWER_STATE_UNKNOWN			= 6		/**< 알수 없는 상태 */

}	DDI_POWER_STATE_T;

/**
 * Named size of panel, comes from nvm_ddi.h
 */
 
#if 0 
typedef enum
{
    INCH_70		= 0,
    INCH_60		= 1,
    INCH_55		= 2,
    INCH_52		= 3,
    INCH_50		= 4,
    INCH_47		= 5,
    INCH_42		= 6,
    INCH_37		= 7,
    INCH_32		= 8,
    INCH_26		= 9,
    INCH_22		= 10,
    INCH_19		= 11,

    INCH_BASE	= 12
} INCH_TYPE_T;


/**
 *	Type of tool, comes from nvm_ddi.h
 */
typedef enum
{
#if 0
// PDP Only Start	
	TOOL_PS80=0,			//브라질, 호주, 멕시코/캐나다, US
	TOOL_PS60,			//멕시코/캐나다, US
	TOOL_PQ60,			//싱/뉴/남, 브라질, 멕시코/캐나다, US
	TOOL_PS70,			// 호주
	TOOL_PQ30,			// 브라질
	TOOL_PS70_CI,		//멕시코/캐나다
	TOOL_PS8900,		//구주
	TOOL_PS8000,		//구주
	TOOL_PS7000,		//구주
	TOOL_PQ6000,		//구주
	TOOL_PS3000,		//구주
	TOOL_PQ3000,		//구주
// PDP Only End
#else
    TOOL_PG70 = 0,
    TOOL_PG60,
    TOOL_PG50,
    TOOL_PG30,
    TOOL_PG10,
#endif
    TOOL_LH90,
    TOOL_LH80,
    TOOL_LH70,
    TOOL_LH60,
    TOOL_LH50,
    TOOL_LH40,
    TOOL_LH30,
    TOOL_LH20,
    TOOL_BASE
} TOOL_TYPE_T;


/**
 *	Type of module, comes from nvm_ddi.h
 */
typedef enum
{
    MODULE_BASE         = 0,
    LGE_FHD             = 1,
    LGE_WXGA            = 2,
    LGE_XGA             = 3,
    LGE_32XGA           = 4,
    LGE_FHD_120         = 5,
    LGE_WXGA_120        = 6,

    LPL_FHD             = 32,
    LPL_WXGA            = 33,
    LPL_FHD_120         = 34,
    LPL_WXGA_120        = 35,
    AUO_FHD             = 36,
    AUO_WXGA            = 37,
    AUO_FHD_120         = 38,
    AUO_WXGA_120        = 39,
    CMO_FHD             = 40,
    CMO_WXGA            = 41,
    CMO_FHD_120         = 42,
    CMO_WXGA_120        = 43,
    SHARP_FHD           = 44,
    SHARP_WXGA          = 45,
    SHARP_FHD_120       = 46,
    SHARP_WXGA_120      = 47,
} MODULE_TYPE_T;

/**
 *	Type of module maker, comes from nvm_ddi.h
 */
typedef enum
{
    EN_LCD_LPL,
    EN_LCD_CMO,
    EN_LCD_AUO,
    EN_LCD_SHARP,
    EN_LCD_MODULE_NUM
} MODULE_MAKER_T;

/**
 * Paired structure for brightness and contrast, moved from nvm_ddi.h
 */
typedef struct
{
	UINT8	nSubBrightness;
	UINT8	nSubContrast;
} ADJUST_SUB_BC_T;

#endif
/**
 *	Calibration data structure, moved from nvm_ddi.h
 */
typedef struct
{
	UINT16	adcRedGain;
	UINT16	adcGreenGain;
	UINT16	adcBlueGain;
	UINT16	adcRedOffset;
	UINT16	adcGreenOffset;
	UINT16	adcBlueOffset;

} ADC_CALIB_DATA_T;

/*
typedef struct
{
	UINT16	adcRedOffset1;
	UINT16	adcGreenOffset1;
	UINT16	adcBlueOffset1;
	UINT16	adcRedOffset2;
	UINT16	adcGreenOffset2;
	UINT16	adcBlueOffset2;
	UINT16	adcRedGain;
	UINT16	adcGreenGain;
	UINT16	adcBlueGain;
} ADC_CALIB_DATA_T;
*/



/************************************************************************
 *	FRONT END
 ************************************************************************/

/**
 * tuner number difinition
 *
 */
typedef enum MRE_TUNER_NUMBER
{
	MRE_TUNER_DEFAULT		= 0,
	MRE_TUNER_0				= 0,
	MRE_TUNER_1,
	MRE_TUNER_2,
	MRE_TUNER_UNKNOWN,
	MRE_TUNER_UN_CHANGED

} MRE_TUNER_NUMBER_T;

#if 1
/**
* transmission media.
*/
typedef enum FE_TRANSMISSION_MEDIA
{
	 FE_TRANS_MEDIA_DIGITAL_MODE		= 0x10,

		 FE_TRANS_MEDIA_D_TERRESTRIAL,
		 FE_TRANS_MEDIA_D_CABLE,
		 FE_TRANS_MEDIA_D_SATELLITE,

	 FE_TRANS_MEDIA_ANALOG_MODE 		= 0x20,

		 FE_TRANS_MEDIA_A_TERRESTRIAL,
		 FE_TRANS_MEDIA_A_CABLE,
		 FE_TRANS_MEDIA_A_SATELLITE,

	 FE_TRANS_MEDIA_SLEEP				= 0x70,

	 FE_TRANS_MEDIA_UNKNOWN 			= 0x80,

	 FE_TRANS_MEDIA_MASK				= 0xF0
} FE_TRANS_MEDIA_T;
#else
/**
* transmission media.
*/
typedef enum FE_TRANSMISSION_MEDIA
{
	FE_TRANS_MEDIA_TERRESTRIAL			= 0x01,		/* Don't use this value, only for mask */
	FE_TRANS_MEDIA_CABLE				= 0x02,		/* Don't use this value, only for mask */
	FE_TRANS_MEDIA_SATELLITE			= 0x03,		/* Don't use this value, only for mask */

	FE_TRANS_MEDIA_DIGITAL_MODE			= 0x10,
		FE_TRANS_MEDIA_D_TERRESTRIAL	= FE_TRANS_MEDIA_DIGITAL_MODE + FE_TRANS_MEDIA_TERRESTRIAL,
		FE_TRANS_MEDIA_D_CABLE			= FE_TRANS_MEDIA_DIGITAL_MODE + FE_TRANS_MEDIA_CABLE,
		FE_TRANS_MEDIA_D_SATELLITE		= FE_TRANS_MEDIA_DIGITAL_MODE + FE_TRANS_MEDIA_SATELLITE,

	FE_TRANS_MEDIA_ANALOG_MODE 			= 0x20,
		FE_TRANS_MEDIA_A_TERRESTRIAL	= FE_TRANS_MEDIA_ANALOG_MODE + FE_TRANS_MEDIA_TERRESTRIAL,
		FE_TRANS_MEDIA_A_CABLE			= FE_TRANS_MEDIA_ANALOG_MODE + FE_TRANS_MEDIA_CABLE,
		FE_TRANS_MEDIA_A_SATELLITE		= FE_TRANS_MEDIA_ANALOG_MODE + FE_TRANS_MEDIA_SATELLITE,

	FE_TRANS_MEDIA_SLEEP				= 0x30,

	FE_TRANS_MEDIA_UNKNOWN 				= 0x80,

	FE_TRANS_MEDIA_MASK					= 0x0F,
	FE_TRANS_MEDIA_MASK_AD				= 0xF0
} FE_TRANS_MEDIA_T;
#endif


/**
 * Analog Audio Sound System Type Definition
 *
 */
#if 0 
typedef enum
{
	AUDIO_SIF_SYSTEM_BG	= 0x00,		// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_I,				// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_DK,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_L,				// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_MN,			// keep the position.	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_LP,			/* 20080724 : Added by Goldman for FE/TU */
	AUDIO_SIF_SYSTEM_END,			/* 20080724 : Added by Goldman for FE/TU */
 	AUDIO_SIF_SYSTEM_UNKNOWN = 0xf0
} AUDIO_SIF_SOUNDSYSTEM_T;
#endif
/*-----------------------------------------------------------------------------
	외부 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _GLOBAL_CONFIGS_H_ */

