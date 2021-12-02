/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file nvm_ddi.h
 *
 *  This header file declares the API functions, type definitions and constants
 *  for NVM driver.
 *
 *  @author     BongSeok Kim(bskim@lge.com)
 *  @author     ByoungHo Lee(bright1004@lge.com)
 *  @version    1.0
 *  @date       2005.06.22
 *  @note
 *  @see
 */

/******************************************************************************
    Header File Guarder
******************************************************************************/
#ifndef _NVM_DDI_H_
#define _NVM_DDI_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
    #include 파일들 (File Inclusions)
******************************************************************************/
#include "branches.h"
#include "global_configurations.h"
#include "osa_api.h"
#include "pql_ddi.h"
#include "paneldb_ddi.h"



/******************************************************************************
    상수 정의(Constant Definitions)
******************************************************************************/
#define ADJ_INPUT_NUM                3		//080812 BSG
#define BAR_CODE_SIZE_MAX           17      /* 16 + 1(null) */

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
    형 정의 (Type Definitions)
******************************************************************************/

/**
 *	Tuner Option struct.
 */
typedef enum
{
   	TUNER_PHASE_DEFAULT = 0,
	TUNER_ENHANCED_PHASE,
	TUNER_ENHANCED_GHOST,
	TUNER_ENHANCED_PHASE_HIGH
}	SYS_TUNER_OPT_T;

/**
    System DB 모듈을 위한 실제 사용 type 정의
*/
typedef struct
{
    UINT32          	validMark;
    OSA_PRINT_MASK_T	printMask;
    UINT32          	sys_utt;			/* PDP panel use time */
    UINT8           	powerOnMode;		/* internal micom:  POWER ON MODE	*/
    UINT8           	powerState;			/* internal micom:	POWER STATE		*/
    UINT8           	powerOnStatus;		/* internal micom:  HOTEL: POWER ON STATUS */
    UINT8           	setID;				/* internal micom:  SET ID			*/
    UINT8           	keyLocked;			/* internal micom:  HOTEL: KEY OPERATION MODE */
    UINT8           	irLocked; 			/* internal micom:	IR KEY LOCKED	*/
    UINT8           	localLocked;		/* internal micom:  LOCAL KEY LOCKED*/
    /* taburin : 20090205, reserved(addr : 0x137)를 atsc-saturn5에서 cec on/off 여부를 micom driver에서 확인 할 수 있도록 사용함. 수정 시 확인 바람.*/
    UINT8           	reserved;   		/* internal micom:  reserved */
    UINT8           	dbgCount;   		/* internal micom:  debug counter */
    UINT8          	    frcDownloadMode;	/* FRC auto download or not */
    PANEL_POWER_SEQ_T	panelpowerseq;		/* For panel power sequence timing */
    PANEL_PWM_T			panelpwm;			/* For panel pwm */
    UINT8               systemtype;         /*system type 0:atsc, 1: dvb*/
    UINT8	                ColorDepth;	/**COLOR_DEPTH_T */
    UINT8	                LVDSPixel;	/**LVDS_PIXEL_T */
	UINT8				vcomPgammaChecksum;	/*Vcom Pgamma value checksum */
	UINT8				nDebugStatus;
#ifndef _EMUL_WIN
    UINT8				reset20110129[0];	/* Uncomment and change name of member to reset values in NVRAM */
#endif
	UINT8				bSupportB0LVDSMAP;	/* FOR BCM BOARD have reversed LVDS MAP */
	SYS_TUNER_OPT_T		nTunerOption;
} SYS_DB_T;

/* 아래 gSysNvmDB 구조체는 System DB의 초기값을 가지고 있는 구조체로서
   아래 macro 함수 내부 처리를 위해 이곳에 선언되어 있으나, 다른 모듈에서
   직접 참조할 이유가 없으므로 사용하지 말것. by CS Kim */
extern  SYS_DB_T gSysNvmDB;

/**
    Analog DB 모듈을 위한 실제 사용 type 정의
*/
typedef struct
{
    UINT8			rGain;              /* Red Gain     */
    UINT8			gGain;              /* Green Gain   */
    UINT8			bGain;              /* Blue Gain    */
    UINT8			rOffset;            /* Red Offset   */
    UINT8			gOffset;            /* Green Offset */
    UINT8			bOffset;            /* Blue Offset  */

} WB_ADJ_DB_T;

/**
 *	LUMA White Balance DB struct type
 */
typedef struct
{
    UINT8      		brightness;
    UINT8      		contrast;
    WB_ADJ_DB_T		cool;
    WB_ADJ_DB_T		normal;
    WB_ADJ_DB_T		warm;

} LUMA_WB_DB_T;

/**
 *	ANALOG DB struct type
 */
typedef struct
{
    UINT32              validMark;

//  Adjustment Database
	ADC_CALIB_DATA_T	gen_adj_db[ADJ_INPUT_NUM];

// Sub-Brightness, Sub-Contrast Adjustment
    UINT8               luma_adj_db_sub_brightness;
    UINT8               luma_adj_db_sub_contrast;
// White Balance Adjustment : Addressing Order - Gain R/G/B, Offset R/G/B
    WB_ADJ_DB_T         wb_adj_db_cool;
    WB_ADJ_DB_T         wb_adj_db_normal;
    WB_ADJ_DB_T         wb_adj_db_warm;
// Each Input
// Sub-Brightness, Sub-Contrast Adjustment
// White Balance Adjustment : Addressing Order - Gain R/G/B, Offset R/G/B
    LUMA_WB_DB_T        luma_wb_db_dtv;         // Digital TV : Antenna/Cable
    LUMA_WB_DB_T        luma_wb_db_arf;         // Analog RF : Antenna/Cable
    LUMA_WB_DB_T        luma_wb_db_av1;         // AV1
    LUMA_WB_DB_T        luma_wb_db_av2;         // AV2
    LUMA_WB_DB_T        luma_wb_db_comp1;       // Component1
    LUMA_WB_DB_T        luma_wb_db_comp2;       // Component2
    LUMA_WB_DB_T        luma_wb_db_rgb;         // RGB
    LUMA_WB_DB_T        luma_wb_db_hdmi1;       // HDMI1
    LUMA_WB_DB_T        luma_wb_db_hdmi2;       // HDMI2
    LUMA_WB_DB_T        luma_wb_db_1394;        // IEEE1394
/* Adjust Status Check RF AV 조정 분리 함 */
    UINT8               check_ADC_RF;           /* RF Adjust success check */
    UINT8               check_ADC_AV;           /* AV Adjust success check */
    UINT8               check_ADC_COMP_480I;    /* Component 480i Adjust success check */
    UINT8               check_ADC_COMP_1080P;   /* Component 1080P Adjust success check */
    UINT8               check_ADC_RGB;          /* RGB Adjust success check */
    UINT8               check_WB;               /* WB Adjust success check */
/* EDID D/L Status Check */
    UINT8               check_HDMI1;            /* RF Adjust success check */
    UINT8               check_HDMI2;            /* AV Adjust success check */
    UINT8               check_HDMI3;            /* Component Adjust success check */
    UINT8               check_HDMI4;            /* RGB Adjust success check */
    UINT8               check_RGB;              /* RGB Adjust success check */
    UINT8		     IsPCM;
	//UINT8               check_VCOM;              /* RGB Adjust success check */

/* EDID Version */
	UINT8				RGB_EDID_Version;				/* RGB EDID version */
	UINT8				HDMI_EDID_Version;			/* HDMI EDID version */

#ifndef _EMUL_WIN
	// 20090902 swwoo added for reset gAnaNvmDB data => next event time we will remove.
	UINT8				reset20110129[0];		/* Uncomment and change name of member to reset values in NVRAM */
#endif
} ANA_DB_T;

/**
   아래 gAnaNvmDB 구조체는 Analog DB의 초기값을 가지고 있는 구조체로서
   아래 macro 함수 내부 처리를 위해 이곳에 선언되어 있으나, 다른 모듈에서
   직접 참조할 이유가 없으므로 사용하지 말것. by CS Kim
  */
extern  ANA_DB_T gAnaNvmDB;
extern  ANA_DB_T gALEFNvmDB;





/**
 *	ANALOG SYSTEM struct.
 */
typedef enum
{
    ANA_NTSC            = 0,
    ANA_3_SYS           = 1,
    ANA_4_SYS           = 2,
    ANA_7_SYS           = 3

}	SYS_ANALOG_T;

/**
 *	BAUDRATE struct.
 */
typedef enum
{
    BAUDRATE_2400       = 0,
    BAUDRATE_4800       = 1,
    BAUDRATE_9600       = 2,
    BAUDRATE_14400      = 3,
    BAUDRATE_19200      = 4,
    BAUDRATE_38400      = 5,
    BAUDRATE_57600      = 6,
    BAUDRATE_115200     = 7,
    BAUDRATE_460800     = 8

}	SYS_BAUDRATE_T;

/**
 *	AUDIO DELAY struct.
 */
typedef enum
{
    AUD_DELAY_0         = 0,
    AUD_DELAY_10        = 1,
    AUD_DELAY_20        = 2,
    AUD_DELAY_30        = 3,
    AUD_DELAY_40        = 4,
    AUD_DELAY_50        = 5,
    AUD_DELAY_60        = 6,
    AUD_DELAY_70        = 7,
    AUD_DELAY_80        = 8

}	SYS_AUDIO_DELAY_T;

/**
 *	Booster AGC value
 */
typedef struct
{
	UINT16      		booster_ON_VHF;
    	UINT16     		booster_OFF_VHF;
    	UINT16			booster_ON_UHF;
	UINT16			booster_OFF_UHF;
} BOOSTER_AGC_T;

/**
*	VCR Format
*/
typedef enum
{
	VCR_FORMAT_PAL60 = 0,
	VCR_FORMAT_NTSC_4_43
}	VCR_FORMAT_T;

/**
*	VBI Line
*/
typedef enum
{
	VBI_18_LINE = 0,
	VBI_21_LINE,
	VBI_22_LINE,
	VBI_MULTI_LINE,
}VBI_LINE_T;

/**
 *	SYSTEM PARAMTER struct.
 */
typedef struct
{
   	UINT8          		bIs2HourOff;            /* 2시간 자동 Off 기능동작 여부 */
   	UINT8          		nAudioEQ;               /* Audio EQ On/Off*/
	UINT8				bAudioBassEQ;			/* Audio Bass EQ On/Off */
	UINT16				nA2_threshold;
	UINT8				bHDMI_SoundPath;		/* Hdmi sound path(HDMI Port1/RGB Phone Jack) */
	UINT8				b15MinForceOff;
	UINT8				bBlueback;
	SINT8				nLipSyncAdjust[32];
	UINT8				bDimming;
	SYS_TUNER_OPT_T		nTunerOption;
	UINT8				bAttenate;
	UINT8				bScreenSaverCubeOnOff;
	UINT8				bChinaCableSO;
	BOOSTER_AGC_T		nBoosterAGC;
	UINT8				bHDEV;					/* China HDEV Control - stonedef 091208*/
	UINT8				nNVRAMType;
	UINT8				bIsScreenMute2HourOff;	/* Screen mute 상태에서 2시간 자동 off 기능 동작 여부 */
	UINT8				bRS232CControl;
	UINT8				bSSID;					/* SSID control */
	UINT8				nTunerVideoOffset;	/* Tuner Video Offset */
	UINT8				nVCRFormat;			/* VCR Format - 4.43 or PAL60*/
	UINT8				bChina3G;			/*xianghua.yin     China 3G  20100805*/
	UINT32				nTunerCRThreshold;	/* Tuner CR unlock threshold - 홍경덕 J 요청 */
	UINT8				bChina4AudioSystem;
}	SYSTEM_PARAM_T;

/**
 *	PANEL AV or PC.
 */
typedef enum
{
    PANNEL_AV         	= 0,
    PANNEL_PC           = 1,
    PANNEL_3D           = 2
}	PANNEL_AVPC_T;

/**
 *	PANEL ISM type
 */
typedef enum
{
    PANNEL_ISM_OFF      = 0,
    PANNEL_ISM_MOVE     = 1,
    PANNEL_ISM_FLIP     = 2,
    PANNEL_ISM_WHITEWASH= 3

}	PANNEL_ISM_T;

/**
 *	PANEL Pawer Save type
 */
typedef enum
{
    PANNEL_PS_MODE0     = 0,
    PANNEL_PS_MODE1     = 1,
    PANNEL_PS_MODE2     = 2,
    PANNEL_PS_MODE3     = 3,
    PANNEL_PS_MODE4     = 4,
    PANNEL_PS_MODE5     = 5,
    PANNEL_PS_MODE6     = 6,
    PANNEL_PS_MODE7     = 7,
    PANNEL_PS_MODE8     = 8,
    PANNEL_PS_MODE9     = 9,
    PANNEL_PS_MODE10    = 10,
    PANNEL_PS_MODE11    = 11

}	PANNEL_POWER_SAVE_T;

/**
 *	PANEL BRIGHT type
 */
typedef enum
{
    BRIGHT_100          = 0,
    BRIGHT_63           = 1,
    BRIGHT_40           = 2

}	PANNEL_BRIGHT_T;

/**
 * GAMMA SET TABLE MODE.
 */
typedef enum
{
    PANNEL_GAMMA_TABLE1 = 0,
    PANNEL_GAMMA_TABLE2 = 1,
    PANNEL_GAMMA_TABLE3 = 2,
    PANNEL_GAMMA_TABLE4 = 3

} 	PANNEL_GAMMA_T;

/**
 *	PANEL LOCK type
 */
typedef enum
{
    PANNEL_LOCK         = 0,
    PANNEL_FREE60       = 1,
    PANNEL_FREE50       = 2

}	PANNEL_LOCK_T;

/**
 *	PANEL ADJUST struct type
 */
typedef struct
{
    PANNEL_AVPC_T       eSelAvPC;
    PANNEL_ISM_T        eSelISM;
    PANNEL_GAMMA_T      nGamma;
    PANNEL_POWER_SAVE_T ePowerSaving;
    UINT8               nOrbitPixel;            /* 1 ~ 3 */
    UINT8               nOrbitStep;             /* 1 ~ 10 Step */
    UINT8               nOrbitTime;             /* 1 ~ 255 sec */
    UINT8               nInvTime;               /* 1 ~ 255 min */
	UINT8            	nMREMode;                 /* TRUE : FMC ON */
    UINT8        		bIsDPS;                 /* TRUE : DPS ON */
    UINT32              nUseTime;               /* USE Time */
}	PANNEL_ADJ_T;

/**
 *	TEMPTH DB struct type
 */
typedef struct
{
    UINT8 dnLow;
    UINT8 dnHigh;
    UINT8 upLow;
    UINT8 upHigh;

}	TEMPTH_DB_T;

#if (PLATFORM_TYPE == BCM_PLATFORM ||PLATFORM_TYPE == MSTAR_PLATFORM || PLATFORM_TYPE == MTK_PLATFORM)
/**
 *	Spread Spectrum
 */
typedef struct {
	UINT8  bSSEnable1 ;
	UINT16 nSSPercent1;
	UINT16 nSSPeriod1;
	UINT8  bSSEnable2 ;
	UINT16 nSSPercent2;
	UINT16 nSSPeriod2;
}SPREADSPECTRUM_DB_T;

/* [L8] soyoung.kim (2010/08/30) -- merge GP3 and L8 */
#elif (PLATFORM_TYPE == LG_PLATFORM)
/**
 *	Spread Spectrum
 */
typedef struct {
	UINT8  bSSEnable1 ;
	UINT16 nSSPercent1;
	UINT16 nSSPeriod1;
	UINT8  bSSEnable2 ;
	UINT16 nSSPercent2;
	UINT16 nSSPeriod2;
}SPREADSPECTRUM_DB_T;

// javamaze
#elif (PLATFORM_TYPE == X86_PLATFORM)
/**
 *	Spread Spectrum
 */
typedef struct {
	UINT8  bSSEnable ;
	UINT16 nSSSpan ;
	UINT16 nSSStep ;
	UINT16 reserved1 ;
	UINT16 reserved2 ;
	UINT16 reserved3 ; //balup_090701
}SPREADSPECTRUM_DB_T;

#else
/**
 *	Spread Spectrum
 */
typedef struct {
	UINT8  bSSEnable1 ;
	UINT16 nSSPercent1;
	UINT16 nSSPeriod1;
	UINT8  bSSEnable2 ;
	UINT16 nSSPercent2;
	UINT16 nSSPeriod2;
}SPREADSPECTRUM_DB_T;

#endif


/**
 * Spread Spectrum Basic DB
 */
typedef struct {
	UINT8	 	bSSEnable;
	UINT8 		uSSPercent;
	UINT8 		uSSPeriod;
} SS_BASIC_DB_T;

/**
 * PDP LG8300 Spread Spectrum
 */
typedef struct {
	SS_BASIC_DB_T 	stDisSSBasic;
	SS_BASIC_DB_T 	stSysSSBasic;
	SS_BASIC_DB_T 	stDdrSSBasic;
} SS_PDP_DB_T;


// TODO: drmyung MRE Type과 Attr에 sync를 맞춰야 함.
/**
*	Src Type for stable count
*	MRE Type과 Attr에 sync를 맞춰야 함.
*/
typedef enum
{
	STABLE_COUNT_SRC_DTV		=	0,	// TV Input
	STABLE_COUNT_SRC_ATV,				// AV Input
	STABLE_COUNT_SRC_SCART,			// Scart Input
	STABLE_COUNT_SRC_AV,				// Scart Input
	STABLE_COUNT_SRC_AUTOAV,			// Auto AV(SCART) Input

	STABLE_COUNT_SRC_COMP, 			// Component(YPbPr) Input
	STABLE_COUNT_SRC_RGB,				// RGB Input
	STABLE_COUNT_SRC_HDMI, 			// HDMI Input
	STABLE_COUNT_SRC_PLAYHDD,			// PVR Input- added by ikarus :20080604
	STABLE_COUNT_SRC_USB,

	STABLE_COUNT_SRC_BT,
	STABLE_COUNT_SRC_MEDIASHARE,
	STABLE_COUNT_SRC_PICWIZARD,
	STABLE_COUNT_SRC_SUPPORT,
	STABLE_COUNT_SRC_BB,

	STABLE_COUNT_SRC_FLASH,
	STABLE_COUNT_SRC_MAX,				// MAX Input
}STABLE_COUNT_SRCTYPE_T;

/**
 *	ODC Test Item
 */
typedef struct {
	UINT8 bEnable ;
	UINT8 nTableIndex ;
}ODC_TEST_T;

#if 1 //CRASH_LOG_TEST
/**
 *	Spread Spectrum
 */
typedef struct {
	UINT8	bEnable ;
	UINT8	nServerIP[4];
	UINT8	nTestRoom;
}CRASH_LOG_OPT_T;
#endif

/**
* NVRAM Type
*/
typedef enum
{
	NVRAM_TYPE_EEPROM = 0,
	NVRAM_TYPE_NAND_FLASH,
}NVRAM_TYPE_T;


/**
 *	FACTORY RESET DB struct type
 */
typedef struct
{
	UINT32              validMark;
	ADJUST_SUB_BC_T		stSubBC;			/* Sub Bright, Contrast */
	SYSTEM_PARAM_T		stSysParam;
	PANNEL_ADJ_T		stPannelAdj;		/* Pannel Adjust Items */

	UINT8				bAutoTestStatus;	/* Instart -> Test Option -> Auto Test */
	UINT8				bOSDTimeOut;		/* Instart -> Test Option -> ui time out */
	UINT8				bStoreModeTest;		/* Instart -> Test Option -> Store Mode Test*/
	UINT8				nStartupSound;
	UINT8				nStandbyLight;
	UINT8				nPoweronLight;
	#ifdef INCLUDE_1ST_PONTIME_REDUCE
	UINT8				bIsFactoryReset;	//dhjung
	#endif
	SPREADSPECTRUM_DB_T	spreadspectrum;
	UINT8				bChannelMute;
	UINT8			 	compSyncLevel;
	UINT8				hdmiSyncLevel;
//	UINT8				nDebugStatus;
	UINT8				nWBBacklight;
	UINT8				srcChStableCount[STABLE_COUNT_SRC_MAX];	/* input change stable count */
	UINT8				bSubstrateMode;
	ODC_TEST_T			odcTest;			/* Instart -> ODC Test */
	UINT8				selfDiagnosisResult[32];	// ScartRGB_CI_Check
#if 1 // GP2 NEW FACTORY DB
	VBI_LINE_T			palN_VBILine;

	UINT8				colorKillerFix;
	SINT8				nColorKillTune1;
	SINT8				nColorKillTune2;

	UINT8				hShakingFix;
	SINT8				nHpllTune1;
	SINT8				nHpllTune2;
	SINT8				nHpllTune3;

	SINT8				nLockThereshold1;
	SINT8				nLockThereshold2;

	UINT8				lgt10ColorMode;
	UINT8				nAutoADC;
	UINT8				hdmiHPDDefault;
	UINT8				lvdsSkewLeft;	/*dot noise 개선을 위해 추가 */
	UINT8				lvdsSkewRight; /*dot noise 개선을 위해 추가 */

#if 1 //def INCLUDE_LG8300
	SS_PDP_DB_T 		stSSDataForPDP;		/* LG8300 Spread Spectrum */
#endif

#if 1 //CRASH_LOG_TEST
	CRASH_LOG_OPT_T 	stCrashLog;
#endif

#endif

#ifndef _EMUL_WIN
	UINT8				reset20110715[0];	/* Uncomment and change name of member to reset values in NVRAM */
#endif
}	FACTORY_DB_T;

/**
 *	TOOL OPTION struct type
 */
typedef struct
{
	UINT32              validMark;
	TOOL_OPTION1_T		nToolOption1;
	TOOL_OPTION2_T		nToolOption2;
	TOOL_OPTION3_T		nToolOption3;
	TOOL_OPTION4_T		nToolOption4;
	TOOL_OPTION5_T		nToolOption5;
	TOOL_OPTION6_T		nToolOption6;
	TOOL_OPTION7_T		nToolOption7;
	AREA_OPTION_T		stAreaOptions;		/* Area Option for 아주/중아/중남미 */
	AREA_OPTION_EU_T	stEuAreaOptions;	/* Area Option for 구주 */
#ifdef INCLUDE_EXT_INPUT_ADJUST
	EXT_VIDEO_INPUT_ADJ_T stExtVideoInputADJ;
	EXT_AUDIO_INPUT_ADJ_T stExtAudioInputADJ;
#endif
	UINT8 				nConfig;

}	TOOLOPTION_DB_T;



extern FACTORY_DB_T gFactoryDB;
extern TOOLOPTION_DB_T gToolOptionDB;

/**
 *	VERSION DB struct type
 */
typedef struct VERSION_DB {
    UINT16              epkVersion;
    UINT16              bootVersion;
    UINT16              rootfsVersion;
    UINT16              kernelVersion;
    UINT32              appVersion;
    UINT16              micomVersion;

} VERSION_DB_T;

//stonedef - 091030
//GP2 에서는 THX 조정을 Service Mode 의 10 Point WB 조정으로 대체하였으므로 삭제함
// 대신 아래쪽의 SUMODE 10 PointWB DB 를 추가함
//stonedef - 091207
// 화질 요청으로 THX DB 도 원래대로 사용함
/**
 *	THX_DB struct type
 */
typedef struct THX_DB
{
	UINT32		validMark;
	UINT8		backlight;
	SINT8 		wb_red[MAX_WB_IRE_STEP];
	SINT8 		wb_green[MAX_WB_IRE_STEP];
	SINT8 		wb_blue[MAX_WB_IRE_STEP];

}THX_DB_T;

extern THX_DB_T gTHXDB;
extern THX_DB_T gALEFTHXDB;


/**
*	Service Mode 10 Point WB DB
*/
typedef struct{
	UINT32		validMark;
	UINT8		wb_onoff;			// 조정 모드 10 point wb 기능을 on/off 하는 기능
	UINT8 		wb_patternType;         	// Inner, Outer
	UINT8 		wb_ire;          			// 0IRE , 10IRE, ... , 100IRE
	UINT16 		wb_luminance;            	// Luminance
	SINT8 		wb_red[MAX_WB_IRE_STEP];
	SINT8 		wb_green[MAX_WB_IRE_STEP];
	SINT8 		wb_blue[MAX_WB_IRE_STEP];
}SUMODE_10POINT_WB_T;

extern SUMODE_10POINT_WB_T gSumode10PointWBDB;


#if 1

/**
*	NEW FACTORY DB ( 양산 이후에 적용해야 하는 사항이 생겨 DB 를 따로 구성함 )
*/
typedef struct NEW_FACTORY_DB
{

	UINT32		validMark;
	UINT8		factorySsuChannel;
	UINT8		bHdmiAvMuteClear;
	UINT8		bSil9287HpdControl;
	UINT8		nTunerAgcSpeed;
	UINT8		nTunerOverModulation;
}NEW_FACTORY_DB_T;

extern NEW_FACTORY_DB_T gNewFactoryDB;
#endif


#if 1	//refactoring_bsw_0
/**
*	MODEL INFO DB ( 모델 정보 관련 DB )
*/
typedef struct MODEL_INFO_DB
{
	UINT32		validMark;
	UINT8		aModelName[13]; //090212 odrie20 Model Name을 eeprom에 저장하고 초기 data만 inch, tool의 조합으로 만들도록 洋
	UINT8		aSerialNum[BAR_CODE_SIZE_MAX];
	UINT8    group_code;      /* country group code which is defined in "common/include/country.h" */
	UINT32  country_code;    /* country code which is defined in "common/include/country_codes.h" */
}MODEL_INFO_DB_T;

extern MODEL_INFO_DB_T gModelInfoDB;

/**
*	PRESERVE DB ( 양산 이후에 Reset 되거나 임의로 변경되지 말아야 하는 DB )
*/
typedef struct PRESERVE_DB
{
	UINT32		validMark;
	UINT8		bInstopPressed;
   	SYS_BAUDRATE_T     	eBaudrate;
}PRESERVE_DB_T;

extern PRESERVE_DB_T gPreserveDB;
#endif

#ifdef INCLUDE_SDP
#define	NETLOG_SIZE	100

/**
 * NETLOG ITEM
 */
typedef struct NETLOG_ITEM
{
	UINT32		module;
	UINT32		logIndex;
} NETLOG_ITEM_T;

/**
 * NET LOG
 */
typedef struct NETLOG_DB
{
	SINT32			lastIndex;
	NETLOG_ITEM_T	netlog[NETLOG_SIZE];
} NETLOG_DB_T;

extern NETLOG_DB_T	gNetLogDB;
#endif

#if 1	//TEMP_CPLIST_DB

/**
*	TMP_CPLIST_DB
*/
typedef struct TMP_CPLIST_DB
{
	UINT32			validMark;
	UINT8 		bEditCPList;
	char			editCPList[5][128];
	UINT8			numOfEditCP;
	UINT8 		bNotifyCPUpdate;
} TMP_CPLIST_DB_T;

extern TMP_CPLIST_DB_T gCpListDB;


/**
*	MAC ADDRESS back-up data
*/
typedef struct MAC_ADDRESS
{
	UINT32		validMark;
	UINT8		macAddr[17];
}MAC_ADDRESS_T;

extern MAC_ADDRESS_T gMacAddress;


#endif

/*********************************************************************
    Function Prototypes
**********************************************************************/
/*  nvm_ddi.c Functions */
extern  DTV_STATUS_T DDI_NVM_Initialize (void);
extern  DTV_STATUS_T DDI_NVM_Uninitialize (void);
extern  DTV_STATUS_T DDI_NVM_Sync (void);
extern	DTV_STATUS_T DDI_NVM_ClearBackupDB (void);
extern	DTV_STATUS_T DDI_NVM_LoadBackupDB (void);
extern	DTV_STATUS_T DDI_NVM_SaveBackupDB (void);
extern  DTV_STATUS_T DDI_NVM_Read (UINT32 offset, UINT32 nData, UINT8 *pRxBuf);
extern  DTV_STATUS_T DDI_NVM_Write (UINT32 offset, UINT32 nData, UINT8 *pTxBuf);
extern  DTV_STATUS_T DDI_NVM_WriteOne (UINT32 offset, UINT8 value);
extern  DTV_STATUS_T DDI_NVM_ReadOne (UINT32 offset, UINT8 *value);
extern  DTV_STATUS_T DDI_NVM_WriteEDID  (UINT32 offset, UINT32 nData, UINT8 *pTxBuf,UINT8 port);
extern  DTV_STATUS_T DDI_NVM_ReadEDID  (UINT32 offset, UINT32 nData, UINT8 *pRxBuf,UINT8 port);
extern  void       	 DDI_NVM_Debug(void);
extern	UINT32		 DDI_NVM_GetPageSize(void);
extern	DTV_STATUS_T DDI_NVM_Lock ( BOOLEAN isLocked );

extern  DTV_STATUS_T DDI_NVM_ALEFNVM_Read (UINT32 offset, UINT32 nData, UINT8 *pRxBuf);
extern  DTV_STATUS_T DDI_NVM_ALEFNVM_Write (UINT32 offset, UINT32 nData, UINT8 *pTxBuf);

/*  nvmdb_ddi.c Functions */
extern  DTV_STATUS_T DDI_NVM_InitSysDB (void);
extern void DDI_NVM_CheckFirstBootUp(void);
extern  DTV_STATUS_T DDI_NVM_FinishFirstBootUp(void);
extern  DTV_STATUS_T DDI_NVM_InitFactoryDB (void);
extern DTV_STATUS_T DDI_NVM_InitToolOptionDB (void);
extern  DTV_STATUS_T DDI_NVM_InitAnalogDB (PQL_WB_DATA_T *pWBData);
extern  DTV_STATUS_T DDI_NVM_ADCCalDataWrite(UINT32 adjSrc, ADC_CALIB_DATA_T *pADCVal);
extern  DTV_STATUS_T DDI_NVM_ADCCalDataRead( UINT32 adjSrc, ADC_CALIB_DATA_T *pADCVal);
extern  DTV_STATUS_T DDI_NVM_GetModelOptions(TOOL_OPTION1_T *pstModelOpt);
extern	BOOLEAN      DDI_NVM_IsDefaultSysDB(void);

DTV_STATUS_T DDI_NVM_InitALEFDB ( PQL_WB_DATA_T *pWBData );
DTV_STATUS_T DDI_NVM_CopyALEFWBData ( PQL_WB_DATA_T *pWBData );
DTV_STATUS_T DDI_NVM_InitALEFTHXDB (void);
DTV_STATUS_T DDI_NVM_CopyALEF_THXDB (void);

//stonedef - 091207
//화질요청으로 원래대로 THX DB 사용함
//stonedef - 091030
//GP2 에서는 THX 조정을 Service Mode 의 10 Point WB 조정으로 대체하였으므로 삭제함
extern  DTV_STATUS_T DDI_NVM_InitTHXDB (void); /* odrie20.09.03.31 THX eeprom map추가.*/

extern DTV_STATUS_T DDI_NVM_InitSumode10PointWBDB (void);

extern DTV_STATUS_T DDI_NVM_InitNewFactoryDB (void);
#if 1	//refactoring_bsw_0
extern DTV_STATUS_T DDI_NVM_InitModelInfoDB (void);
extern DTV_STATUS_T DDI_NVM_InitPreserveDB (void);
#endif
extern DTV_STATUS_T DDI_NVM_InitFactoryOTAChannel (void);

extern DTV_STATUS_T DDI_NVM_InitCPListDB (void);	//TMP_CPLIST_DB

#ifdef INCLUDE_1ST_NVM_ASYNC_MODE
extern void 	DDI_NVM_SetCacheAsyncMode(BOOLEAN bAsyncMode);
extern BOOLEAN 	DDI_NVM_IsCacheAsyncMode(void);
#endif

#ifdef __cplusplus
}
#endif

#if ( PLATFORM_TYPE == LG_PLATFORM )
/* [L8] 20100709 seokjoo.lee : CRITICAL UPDATE for GP2 & GP3
 *
 * strict type checking macro added to check memory overrun.
 *
 * LIMITATION :
 * function accessing array member in NVM DB, such as aSerialName
 *
 * void SUMDSVC_CTRL_GetSerialNumber(UINT8 *pSerial)
 * {
 * 	NVM_ANA_DB_LOAD_ITEM(aSerialNum, pSerial);
 * 	return;
 * }
 * -> Solution : modify that function as a macro.
 * So, I modify above series of useless wrapper functions to macro definition
 * which directly passes its pointer argument to _TYPE_CHECK_NVM_DB() macro.
 * (actually, the actual meaning of existence of wrapper function is
 * to separate inclusion of DDI header file from UI level source.
 * e.g nvm_ddi.h shall not included in UI layer source using INCLUDE_BY_UI_LAYER macro )
 *
 * USAGE
 * -----
 * M/W and UI layer check.
 * 1. disable header inclusion check by changing AWK_PATH_VAR =.... to  AWK_PATH_VAR ?= ... in rules.mk
 * 2. comment out #ifndef INCLUDE_BY_UI_LAYER in mw/include/sumdsvc_api.h
 * 3. define default _USE_STRICT_TYPE_CHECKING_NVM_DB to 1 below.
 * 4. if duplicated API_SUMDSVC_*(), SUMDSVC_CTRL_*() in mw/sumdsvc/*.c, *.h, block it with
 * 		#if (_USE_STRICT_TYPE_CHECKING_NVM_DB > 0 )
 * 		...
 * 		#endif
 * 5. modify rules.mk : AWK_PATH_VAR= -> AWK_PATH_VAR?=
 * 6. make by following command
 * 		AWK_PATH_VAR="" make
 * 7. check "warning: comparison of distinct pointer types lacks a cast" for possible stack overflow.
 * 8. after checking completed for a file, #define _USE_STRICT_TYPE_CHECKING_NVM_DB 0 to skip checking.
 *
 * NOTE
 * ----
 *  This macro may produces false alarm such as caller have enough buffer
 *  for example M/W or UI declarates UINT8 aaa[25] for storage, but actual gSysNvmDB has UINT8 bbb[17],
 *	{
 *		UINT8 aaa[25];
 *      NVM_SYS_DB_LOAD_ITEM(bbb,&aaa);
 *  }
 *
 *  Because of typeof(UINT8[25]) != typeof(UINT8[17]), warning shall be produced but it is false warning:
 *  just ignore above case.
 *
 * */
#ifndef _USE_STRICT_TYPE_CHECKING_NVM_DB
#define _USE_STRICT_TYPE_CHECKING_NVM_DB 0
#endif

#if ( _USE_STRICT_TYPE_CHECKING_NVM_DB > 0)
#define _TYPE_CHECK_NVM_DB( _db, _item, _pvalue)				\
				typeof(&_db._item) __item	= &(_db._item);		\
				typeof(_pvalue  ) __pValue	= (_pvalue);		\
				(void) (__item  == __pValue);
#else
#define _TYPE_CHECK_NVM_DB(_db,_item,_pvalue)
#endif	/* _USE_STRICT_TYPE_CHECKING_NVM_DB */

#else
#define _TYPE_CHECK_NVM_DB(_db,_item,_pvalue)
#endif	/* PLATFORM_TYPE */


/**
 * Macros to manipulate some NVM DB
 *
 *  NVM_SYS_DB_STORE_ITEM(item, pValue) / NVM_SYS_DB_LOAD_ITEM(item, pValue) / NVM_SYS_DB_GET_DEFAULT(item, pValue)
 *  NVM_ANA_DB_STORE_ITEM(item, pValue) / NVM_ANA_DB_LOAD_ITEM(item, pValue) / ANA_DB_GetDefault(item, pValue)
 *
 *----------------------------------------------------------------------------
 *  실행내용 :  해당 DB의 특정 item에 원하는 값을 저장하거나,
 *              해당 item 값을 읽어온다.
 *----------------------------------------------------------------------------
 *  item    :   모듈별 DB 구조체의 member
 *  pValue  :   저장하거나 읽어올 변수의 pointer
 *              단.. *pValue의 type은 반드시 item의 type과 일치해야 한다.
 * ----------------------------------------------------------------------------
 *
 *  예를 들어 SYS_DB_T 구조체의 member중 validMark에 어떤 값을
 *  저장하고 싶을 경우 아래와 같이 call 한다.
 *
 *  < 실행 예 >
 *
 *  {
 *      UINT32  writeValue;
 *      UINT32  readValue;
 *
 *      NVM_SYS_DB_LOAD_ITEM (validMark, &readValue);
 *      writeValue = readValue | 0x01;
 *      NVM_SYS_DB_STORE_ITEM (validMark, &writeValue);
 *  }
 */

//balup_090610
#define	_DDI_NVM_READ		DDI_NVM_Read
#define	_DDI_NVM_WRITE		DDI_NVM_Write

#if 0
#if	(DEV_STATE == DEBUG_STATE)
#define	_DDI_NVM_READ		DDI_NVM_Read
#define	_DDI_NVM_WRITE		DDI_NVM_Write
#else
#define	_DDI_NVM_READ		DDI_NVM_Read
#define	_DDI_NVM_WRITE		DDI_NVM_Write
#endif
#endif

#define NVM_MAGIC_STORE(pValue)		(_DDI_NVM_WRITE(TNVM_MAGIC_BASE, TNVM_MAGIC_SIZE, (UINT8 *)(pValue)))
#define NVM_MAGIC_LOAD( pValue)		(_DDI_NVM_READ (TNVM_MAGIC_BASE, TNVM_MAGIC_SIZE, (UINT8 *)(pValue)))

#define NVM_SYS_DB_STORE_ITEM(item, pValue)      \
		({																						\
			_TYPE_CHECK_NVM_DB(gSysNvmDB,item,pValue);											\
        (_DDI_NVM_WRITE (SYS_DB_BASE + (UINT32)&(gSysNvmDB.item) - (UINT32)&gSysNvmDB,          \
                sizeof(gSysNvmDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_SYS_DB_LOAD_ITEM(item, pValue)       \
		({																						\
			_TYPE_CHECK_NVM_DB(gSysNvmDB,item,pValue);											\
        (_DDI_NVM_READ (SYS_DB_BASE + (UINT32)&(gSysNvmDB.item) - (UINT32)&gSysNvmDB,           \
                sizeof(gSysNvmDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_SYS_DB_GET_DEFAULT(item, pValue)     \
        (*pValue = gSysNvmDB.item)


#define NVM_ANA_DB_STORE_ITEM(item, pValue)      \
		({																						\
			_TYPE_CHECK_NVM_DB(gAnaNvmDB,item,pValue);											\
        (_DDI_NVM_WRITE (ANA_DB_BASE + (UINT32)&(gAnaNvmDB.item) - (UINT32)&gAnaNvmDB,          \
                sizeof(gAnaNvmDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_ANA_DB_LOAD_ITEM(item, pValue)       \
		({																						\
			_TYPE_CHECK_NVM_DB(gAnaNvmDB,item,pValue);											\
        (_DDI_NVM_READ (ANA_DB_BASE + (UINT32)&(gAnaNvmDB.item) - (UINT32)&gAnaNvmDB,           \
                sizeof(gAnaNvmDB.item), (UINT8 *)(pValue)));									\
		})


#define NVM_ALEF_DB_STORE_ITEM(item, pValue)      \
		({																						\
			_TYPE_CHECK_NVM_DB(gLAEFNvmDB,item,pValue);											\
        (DDI_NVM_ALEFNVM_Write (ALEF_DB_BASE + (UINT32)&(gALEFNvmDB.item) - (UINT32)&gALEFNvmDB,          \
                sizeof(gALEFNvmDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_ALEF_DB_LOAD_ITEM(item, pValue)       \
		({																						\
			_TYPE_CHECK_NVM_DB(gLAEFNvmDB,item,pValue);											\
        (DDI_NVM_ALEFNVM_Read (ALEF_DB_BASE + (UINT32)&(gALEFNvmDB.item) - (UINT32)&gALEFNvmDB,           \
                sizeof(gALEFNvmDB.item), (UINT8 *)(pValue)));									\
		})

#define NVM_ALEF_THX_DB_STORE_ITEM(item, pValue)	\
		({																						\
			_TYPE_CHECK_NVM_DB(gLAEFNvmDB,item,pValue);											\
	(DDI_NVM_ALEFNVM_Write (ALEF_THX_DB_BASE + (UINT32)&(gALEFTHXDB.item) - (UINT32)&gALEFTHXDB,          \
                sizeof(gALEFTHXDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_ALEF_THX_DB_LOAD_ITEM(item, pValue)       \
		({																						\
			_TYPE_CHECK_NVM_DB(gLAEFNvmDB,item,pValue);											\
        (DDI_NVM_ALEFNVM_Read (ALEF_THX_DB_BASE + (UINT32)&(gALEFTHXDB.item) - (UINT32)&gALEFTHXDB,           \
                sizeof(gALEFTHXDB.item), (UINT8 *)(pValue)));									\
		})


#define NVM_ANA_DB_GET_DEFAULT(item, pValue)     \
                (*pValue = gAnaNvmDB.item)

#define NVM_TOOLOPT_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gToolOptionDB,item,pValue);										\
        (_DDI_NVM_WRITE (TOOL_OPTION_DB_BASE + (UINT32)&(gToolOptionDB.item) - (UINT32)&gToolOptionDB,    \
        		sizeof(gToolOptionDB.item), (UINT8 *)(pValue)));								\
		})
#define NVM_TOOLOPT_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gToolOptionDB,item,pValue);										\
		(_DDI_NVM_READ (TOOL_OPTION_DB_BASE + (UINT32)&(gToolOptionDB.item) - (UINT32)&gToolOptionDB, 	\
				sizeof(gToolOptionDB.item), (UINT8 *)(pValue)));								\
		})

#define NVM_FACTORY_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gFactoryDB,item,pValue);											\
        (_DDI_NVM_WRITE (FACTORY_DB_BASE + (UINT32)&(gFactoryDB.item) - (UINT32)&gFactoryDB,    \
                sizeof(gFactoryDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_FACTORY_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gFactoryDB,item,pValue);											\
        (_DDI_NVM_READ (FACTORY_DB_BASE + (UINT32)&(gFactoryDB.item) - (UINT32)&gFactoryDB,     \
                sizeof(gFactoryDB.item), (UINT8 *)(pValue)));									\
		})
#define NVM_FACTORY_DB_GET_DEFAULT(item, pValue) \
        (*pValue = gFactoryDB.item)

// stonedef - 091207
// 화질 요청으로 THX DB 원래대로 사용함
//stonedef - 091030
//GP2 에서는 THX 조정을 Service Mode 의 10 Point WB 조정으로 대체하였으므로 삭제함
// 대신 아래쪽의 SUMODE 10 PointWB DB 를 추가함 - Size 는 THX DB 와 동일하게 사용함
/* odrie20.09.03.31 THX eeprom map추가.*/

#define NVM_THX_DB_STORE_ITEM(item, pValue)  \
		({																			\
			_TYPE_CHECK_NVM_DB(gTHXDB,item,pValue);									\
        (_DDI_NVM_WRITE (THX_DB_BASE + (UINT32)&(gTHXDB.item) - (UINT32)&gTHXDB,    \
        		sizeof(gTHXDB.item), (UINT8 *)(pValue)));							\
		})
#define NVM_THX_DB_LOAD_ITEM(item, pValue)   \
		({																			\
			_TYPE_CHECK_NVM_DB(gTHXDB,item,pValue);									\
			(_DDI_NVM_READ (THX_DB_BASE + (UINT32)&(gTHXDB.item) - (UINT32)&gTHXDB,	\
					sizeof(gTHXDB.item), (UINT8 *)(pValue)));						\
		})

/*stonedef - 091030 Service Menu 의 10 Point WB DB 추가 */
#define NVM_SUMODE_10POINTWB_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gSumode10PointWBDB,item,pValue);									\
        (_DDI_NVM_WRITE (SUMODE_10POINTWB_DB_BASE + (UINT32)&(gSumode10PointWBDB.item) - (UINT32)&gSumode10PointWBDB,    \
        		sizeof(gSumode10PointWBDB.item), (UINT8 *)(pValue)));							\
		})
#define NVM_SUMODE_10POINTWB_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gSumode10PointWBDB,item,pValue);									\
			(_DDI_NVM_READ (SUMODE_10POINTWB_DB_BASE + (UINT32)&(gSumode10PointWBDB.item) - (UINT32)&gSumode10PointWBDB,	\
					sizeof(gSumode10PointWBDB.item), (UINT8 *)(pValue)));						\
		})
#if 1
/*daesuk.park - 100222 New Factory DB 추가 - 양산 이후 수정을 위해 */
#define NVM_NEW_FACTORY_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gNewFactoryDB,item,pValue);										\
        (_DDI_NVM_WRITE (NEW_FACTORY_DB_BASE + (UINT32)&(gNewFactoryDB.item) - (UINT32)&gNewFactoryDB,    \
        		sizeof(gNewFactoryDB.item), (UINT8 *)(pValue)));								\
		})

#define NVM_NEW_FACTORY_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gNewFactoryDB,item,pValue);										\
	(_DDI_NVM_READ (NEW_FACTORY_DB_BASE + (UINT32)&(gNewFactoryDB.item) - (UINT32)&gNewFactoryDB,	\
			sizeof(gNewFactoryDB.item), (UINT8 *)(pValue)));									\
		})
#endif

#if 1	//refactoring_bsw_0
/*sw.byeon - 100326 Mode Info DB 추가 - 모델명, 시리얼넘버, 국가 관련 정보를 별도 DB로 관리함. */
#define NVM_MODEL_INFO_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gModelInfoDB,item,pValue);										\
			(_DDI_NVM_WRITE (MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.item) - (UINT32)&gModelInfoDB,	  \
					sizeof(gModelInfoDB.item), (UINT8 *)(pValue)));				\
		})
#define NVM_MODEL_INFO_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gModelInfoDB,item,pValue);										\
		(_DDI_NVM_READ (MODEL_INFO_DB_BASE + (UINT32)&(gModelInfoDB.item) - (UINT32)&gModelInfoDB,	\
				sizeof(gModelInfoDB.item), (UINT8 *)(pValue)));					\
		})

/*sw.byeon - 100326 Mode Info DB 추가 - 모델명, 시리얼넘버, 국가 관련 정보를 별도 DB로 관리함. */
#define NVM_PRESERVE_DB_STORE_ITEM(item, pValue)  \
		({																						\
			_TYPE_CHECK_NVM_DB(gPreserveDB,item,pValue);										\
			(_DDI_NVM_WRITE (PRESERVE_DB_BASE + (UINT32)&(gPreserveDB.item) - (UINT32)&gPreserveDB,	  \
					sizeof(gPreserveDB.item), (UINT8 *)(pValue)));				\
		})

#define NVM_PRESERVE_DB_LOAD_ITEM(item, pValue)   \
		({																						\
			_TYPE_CHECK_NVM_DB(gPreserveDB,item,pValue);										\
		(_DDI_NVM_READ (PRESERVE_DB_BASE + (UINT32)&(gPreserveDB.item) - (UINT32)&gPreserveDB,	\
				sizeof(gPreserveDB.item), (UINT8 *)(pValue)));					\
		})
#endif


//kyungmee.lee 20101011
#ifdef INCLUDE_SDP
#define NVM_NETLOG_DB_STORE_ITEM(item, pValue)      \
        (_DDI_NVM_WRITE (NETLOG_DB_BASE + (UINT32)&(gNetLogDB.item) - (UINT32)&gNetLogDB,          \
                sizeof(gNetLogDB.item), (UINT8 *)(pValue)))
#define NVM_NETLOG_DB_LOAD_ITEM(item, pValue)       \
        (_DDI_NVM_READ (NETLOG_DB_BASE + (UINT32)&(gNetLogDB.item) - (UINT32)&gNetLogDB,           \
                sizeof(gNetLogDB.item), (UINT8 *)(pValue)))
#endif
#if 1	//TEMP_CPLIST_DB
#define NVM_TMP_CPLIST_DB_STORE_ITEM(item, pValue)  \
			(_DDI_NVM_WRITE (TMP_CPLIST_DB_BASE + (UINT32)&(gCpListDB.item) - (UINT32)&gCpListDB,	  \
					sizeof(gCpListDB.item), (UINT8 *)(pValue)))
#define NVM_TMP_CPLIST_DB_LOAD_ITEM(item, pValue)   \
		(_DDI_NVM_READ (TMP_CPLIST_DB_BASE + (UINT32)&(gCpListDB.item) - (UINT32)&gCpListDB,	\
				sizeof(gCpListDB.item), (UINT8 *)(pValue)))
#endif


#if 1	//MAC ADDRESS BACK-UP

#define NVM_MAC_ADDRESS_STORE_ITEM(item, pValue)  \
		(_DDI_NVM_WRITE (MAC_ADDRESS_BASE + (UINT32)&(gMacAddress.item) - (UINT32)&gMacAddress,	  \
			sizeof(gMacAddress.item), (UINT8 *)(pValue)))


#define NVM_MAC_ADDRESS_LOAD_ITEM(item, pValue)   \
		(_DDI_NVM_READ (MAC_ADDRESS_BASE + (UINT32)&(gMacAddress.item) - (UINT32)&gMacAddress,	\
			sizeof(gMacAddress.item), (UINT8 *)(pValue)))
#endif

#endif  /* End of _NVM_DDI_H_ */
