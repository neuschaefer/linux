/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file audio_ddi.h
 *
 *  AUDIO DD를 제어하는 DPI 함수에 대한 header file.
 *  AUDIO API와 AUDIO DD를 연결하는 함수들로 주로 AUDIO Menu및 source 설정과 관련된
 *  함수들로 이루어져 있음.
 *
 *  @author		Park Jeong-Gun(alwaysok@lge.com)
 *  @version	1.0
 *  @date		2006.04.20
 *  @note
 *  @see		audio_ddi.c
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _AUDIO_DDI_H_
#define _AUDIO_DDI_H_

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM // Modified by dilly97: fix include
#include "common.h"
#else
#include "common_win32.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "global_configurations.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

#define USE_TOOL_OPTION

// stonedef - 090911
// Lip sync Delay - temporary
#define AUD_USER_DELAY_MIN	(-10)
#define AUD_USER_DELAY_MAX	(20)

#ifndef USE_CUR_USER_DELAY
#define	USE_CUR_USER_DELAY ((SINT8)(0x80))
#define	USE_CUR_CONSTANT_DELAY_TIME	(0xFFFFFFFF)
#define	CHANGE_CUR_USER_DELAY_DB	(0xF0F0F0F0)
#define	CHANGE_CUR_MVD_DELAY_DB		(0xF0000000)
#endif

#define CV3_GENERAL_SETTING_PARAM_NUM 14
#define CV3_CV_PARAM_NUM 25
#define CV3_UEQ_PARAM_NUM 20
#define CV3_UEQ2_PARAM_NUM 20
#define CV3_SURROUND_PARAM_NUM 23
#define CV_BASS_EQ_PARAM_NUM 124
#define CV3_AVL2_PARAM_NUM 23
#define CV3_GET_LEVEL_PARAM_NUM 10

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
/**
 * Audio SPDIF Information Definition
 *
 */
typedef struct AUDIO_SPDIF_INFO
{
	UINT8   spdifInOnOff	:   1;      /**<   SPDIF Input을 가지는 소스인지 여부 나타냄  */
	UINT8   spdifInExist	:   1;      /**<   SPDIF Input의 존재 여부를 나타냄           */
	UINT8   spdifInType		:   2;      /**<   SPDIF Input이 AC3/PCM/NONE의 여부를 나타냄 */
	UINT8   spdifInSfreq	:   2;      /**<   SPDIF Input의 Sampling Frequency를 나타냄  */
	UINT8   spdifDemOnOff	:   1;      /**<   SPDIF Input의 Preemphasis여부를 나타냄     */
	UINT8   spdifDviSet		:   1;		/**<   SPDIF Input의 DVI Mode 환경설정 여부를 나타냄 */
	UINT8	spdifHdmiSet	:	1;		/**<   SPDIF Input의 HDMI Mode 환경설정 여부를 나타냄 */
}   AUDIO_SPDIF_INFO_T;

/**
 * AUDIO SPDIF Copy Protection Type
 *
 */
typedef  enum
{
	AUDIO_SPDIF_COPY_FREE		= 0,	/* cp-bit : 1, L-bit : 0 */
	AUDIO_SPDIF_COPY_NO_MORE	= 1,	/* cp-bit : 0, L-bit : 1 */
	AUDIO_SPDIF_COPY_ONCE		= 2,	/* cp-bit : 0, L-bit : 0 */
	AUDIO_SPDIF_COPY_NEVER		= 3,	/* cp-bit : 0, L-bit : 1 */
} AUDIO_SPDIF_COPYRIGHT_T ;

/**
 * Audio Stage Information Definition
 *
 */
typedef struct AUDIO_SET_STAGE_INFO
{
	UINT8   init        			:   1;  /**<  Initialize 여부 */
	UINT8   first_set_source		:   1;	/**<  최초 Audio Set Source 여부 */
	UINT8   set_source  			:   1;	/**<  Audio Source change 상태 */
	UINT8   set_type    			:   1;  /**<  Audio Type Change 상태 */
	UINT8   play        			:   1;	/**<  Audio Play 상태 */
	UINT8   done_set_aud_demod		:	1;	/**< 최초 sound system 셋팅 여부 */

}   AUDIO_SET_STAGE_INFO_T;

/**
 * Audio Sampling Frequency Definition
 *
 */
typedef enum    AUDIO_SAMPLING_FREQUENCY
{
	F96K,	/* 96KHz */
	F48K,  /* 48KHz    */
	F44K,  /* 44.1KHz  */
	F32K,  /* 32KHz    */
	F24K,  /* 24KHz	   */
	F22K,  /* 22.05KHz */
	F16K,	/* 16KHz	*/
	F12K,  /* 12KHz    */
	F11K,	/* 11.025KHz */
	F8K,	/* 8KHz		*/
} AUDIO_SAMPLING_FREQUENCY_T;

/**
 * Audio Master Clock Frequency Definition
 *
 */
typedef enum
{
	AUDIO_F12_288MHZ,
	AUDIO_F24_576MHZ,
	AUDIO_F18_432MHZ,
	AUDIO_F8_192MHZ,
	AUDIO_F16_384MHZ,
	AUDIO_F2_048MHZ,
	AUDIO_F3_072MHZ,
	AUDIO_F4_096MHZ,
	AUDIO_F6_144MHZ,
} AUDIO_MCLK_REAL_FREQUENCY_T;

/**
 * Audio Channel Definition
 *
 */
typedef enum    AUDIO_CHANNEL_INFO
{
	ADEC_CH_0,  /* Channel 0 : Main Audio Channel */
	ADEC_CH_1,  /* Channel 1 : Sub Audio Channel */
} AUDIO_CHANNEL_INFO_T;

/**
 * Audio SPDIF Stream Type Definition
 *
 */
typedef enum    SPDIF_STREAM
{
	SPDIF_PCM,
	SPDIF_AC3,
	SPDIF_DTS,
	SPDIF_NONE,
} SPDIF_STREAM_T;

/**
 * Audio Source Type Definition
 *
 */
typedef  enum  AUDIO_SOURCE
{
	AUD_SRC_DTV			= 0,
	AUD_SRC_ATV			= 1,
	AUD_SRC_AV			= 2,
	AUD_SRC_COMP		= 3,
	AUD_SRC_RGB			= 4,
	AUD_SRC_HDMI		= 5,
	AUD_SRC_DVI			= 6,
	AUD_SRC_EMF			= 7,
	AUD_SRC_BOOT		= 8,
	AUD_SRC_BLUETOOTH	= 9,
	AUD_SRC_MEDIASHARE	= 10,
	AUD_SRC_BB			= 11,
	AUD_SRC_FLASH		= 12,
	AUD_SRC_PICWIZARD	= 13,	// Picture Wizard
	AUD_SRC_SUPPORT		= 14,
	AUD_SRC_PLAY_HDD	= 15,
	AUD_SRC_INVALID		= 16
}  AUDIO_SOURCE_T ;

/**
 * Audio Source Delay Index Definition
 *
 */
typedef  enum  AUDIO_DELAY_SRC_INDEX
{
	AUD_DELAY_SRC_DTV		= 0,
	AUD_DELAY_SRC_ATV,
	AUD_DELAY_SRC_AV1,
	AUD_DELAY_SRC_AV2,
	AUD_DELAY_SRC_AV3,
	AUD_DELAY_SRC_COMP1,
	AUD_DELAY_SRC_COMP2,
	AUD_DELAY_SRC_COMP3,
	AUD_DELAY_SRC_RGB,
	AUD_DELAY_SRC_HDMI1,
	AUD_DELAY_SRC_HDMI2,
	AUD_DELAY_SRC_HDMI3,
	AUD_DELAY_SRC_HDMI4,
	AUD_DELAY_SRC_EMF_PHOTO_MUSIC,
	AUD_DELAY_SRC_EMF_MOVIE,
	AUD_DELAY_SRC_BLUETOOTH,
#if 0
	AUD_DELAY_SRC_MEDIASHARE,
	AUD_DELAY_SRC_BB,
	AUD_DELAY_SRC_FLASH,
	AUD_DELAY_SRC_PICWIZARD,	// Picture Wizard
#endif
	AUD_DELAY_SRC_PLAY_HDD,
	AUD_DELAY_SRC_DEFAULT,
	AUD_DELAY_SRC_INDEX_MAX
}  AUDIO_DELAY_SRC_INDEX_T;

/**
 * Audio Input source Information
 *
 */
typedef struct AUDIO_SOURCE_INFO
{
	UINT8	srcType;	/** source type */
	UINT8	index;		/** source index */
	UINT8 	Attr;		/** source Attribution */
} AUDIO_SOURCE_INFO_T;

/**
 * Audio PVR Source Type Definition
 *
 */
typedef  enum  AUDIO_PVR_SOURCE
{
	AUD_PVR_SOURCE_UNKNOWN = 0,
	AUD_PVR_SOURCE_DTV,//AUD_PVR_SOURCE_ATSV
	AUD_PVR_SOURCE_ATV_MAIN,//AUD_PVR_SOURCE_NTSC_MAIN
	AUD_PVR_SOURCE_ATV_SUB, //AUD_PVR_SOURCE_NTSC_SUB
	AUD_PVR_SOURCE_VIDEO1,
	AUD_PVR_SOURCE_VIDEO2,
	AUD_PVR_SOURCE_COMPONENT1,
	AUD_PVR_SOURCE_COMPONENT2,
	AUD_PVR_SOURCE_DVI,
	AUD_PVR_SOURCE_HDMI1,
	AUD_PVR_SOURCE_HDMI2,
	AUD_PVR_SOURCE_HDMI3,
	AUD_PVR_SOURCE_HDMI4,
	AUD_PVR_SOURCE_IEEE1394,
	AUD_PVR_SOURCE_END
}  AUDIO_PVR_SOURCE_T ;

/**
 * Audio Volume Information Definition
 *
 */
typedef  struct  AUDIO_VOLUME
{
	UINT8           left;			/**< left volume  */
	UINT8           right;		/**< right volume  */
	UINT8           center;		/**< center volume  */
	UINT8           leftsur;		/**< left surround volume  */
	UINT8           rightsur;		/**< right surround volume  */
	UINT8           subwoofer;	/**< subwoober volume  */
	UINT8           balance;		/**< balance value  */
	UINT8           fade;		/**< fade value */
	UINT8           mask;		/**< mask  */

#define AUDIO_VOLUME_MASK_LEFT          0x80	/**< left volume mask  */
#define AUDIO_VOLUME_MASK_RIGHT         0x40	/**< right volume mask  */
#define AUDIO_VOLUME_MASK_CENTER        0x20	/**< center volume mask  */
#define AUDIO_VOLUME_MASK_LEFTSUR       0x10	/**< left surround volume mask  */
#define AUDIO_VOLUME_MASK_RIGHTSUR      0x08	/**< right surround volume mask  */
#define AUDIO_VOLUME_MASK_SUBWOOFER     0x04	/**< subwoober volume mask  */
#define AUDIO_VOLUME_MASK_BALANCE       0x02	/**< balance mask  */
#define AUDIO_VOLUME_MASK_FADE          0x01	/**< fade mask  */

}  AUDIO_VOLUME_T ;

/**
 * Audio Surround Type Definition
 *
 */
typedef enum AUDIO_SURROUND_EFFECT
{
	DDI_SURROUND_OFF    = 0,
	DDI_SURROUND_CV,
	DDI_SURROUND_INFINITESOUND
} AUDIO_SURROUND_EFFECT_T ;

/**
 * Audio SPDIF Output Type Definition
 *
 */
typedef enum AUDIO_SPDIF_OUTPUT_MODE
{
	SPDIF_OUTPUT_AUTO,
	SPDIF_OUTPUT_AC3,
	SPDIF_OUTPUT_AAC,
	SPDIF_OUTPUT_PCM
} AUDIO_SPDIF_OUTPUT_MODE_T ;

/**
 * Audio HDMI Input Type Definition
 * heeowen: 20090331
 */
typedef enum AUDIO_HDMI_INPUT_MODE
{
	HDMI_INPUT_UNKNOW = 0,
	HDMI_INPUT_PCM	 = 1,
	HDMI_INPUT_AC3	 = 2,
	HDMI_INPUT_DTS	 = 3,

}AUDIO_HDMI_INPUT_MODE_T;

/**
 * AUDIO SIF Mode.
 *
 */
typedef enum
{
	AUDIO_SIF_MODE_DETECT	= 0,
	AUDIO_SIF_BG_NICAM		= 1,
	AUDIO_SIF_BG_FM			= 2,
	AUDIO_SIF_BG_A2			= 3,
	AUDIO_SIF_I_NICAM		= 4,
	AUDIO_SIF_I_FM			= 5,
	AUDIO_SIF_DK_NICAM		= 6,
	AUDIO_SIF_DK_FM			= 7,
	AUDIO_SIF_DK1_A2		= 8,
	AUDIO_SIF_DK2_A2		= 9,
	AUDIO_SIF_DK3_A2		= 10,
	AUDIO_SIF_L_NICAM		= 11,
	AUDIO_SIF_L_AM			= 12,
	AUDIO_SIF_MN_A2			= 13,
	AUDIO_SIF_MN_BTSC		= 14,
	AUDIO_SIF_MN_EIAJ		= 15,
	AUDIO_SIF_STD_END		= 16,	/* 20080724 : Added by Goldman for FE/TU */
	AUDIO_SIF_STD_UNKNWON 	= 0xF0	/* 20080724 : Added by Goldman for FE/TU */
} AUDIO_SIF_STANDARD_T;

/**
 * Analog Audio NICAM Detection Type Definition
 *
 */
typedef enum ATV_AUDIO_NICAM_SIGNATURE
{
	ATV_AUDIO_NICAM_ABSENT		= 0x00,
	ATV_AUDIO_NICAM_PRESENT		= 0x01,
	ATV_AUDIO_NICAM_DETECTING	= 0x02
} ATV_AUDIO_NICAM_SIGNATURE_T;

/**
 * Speaker Output Type Definition
 *
 */
typedef enum
{
	AUDIO_MODE_MONO	 			= 0,
	AUDIO_MODE_JOINT_STEREO 	= 1,
	AUDIO_MODE_STEREO		 	= 2,
	AUDIO_MODE_DUAL_MONO 		= 3,
	AUDIO_MODE_MULTI			= 4,
	AUDIO_MODE_UNKNOWN			= 5,
} AUDIO_STEREO_MODE_T;

/**
 * Speaker Output Type Definition
 *
 */
typedef enum
{
	AUDIO_SPK_MODE_LR		= 0,
	AUDIO_SPK_MODE_LL		= 1,
	AUDIO_SPK_MODE_RR		= 2,
	AUDIO_SPK_MODE_MIX		= 3
} AUDIO_SPK_OUTPUT_MODE_T;

/**
 * Audio Decoder Information Definition
 * Currently Not Used
 *
 */
typedef struct AUDIO_INFO
{
	UINT8 decoderMode;  /**<  decoding mode :: AC3, MPEG1, MPEG2,… */
	UINT8 dataType;     /**<  decoding type :: PES, ES, … */
	UINT8 sfreq;        /**<  Sampling Frequency */
	UINT8 syncInfo;     /**<  Decoder Sync Status */
}  AUDIO_INFO_T ;

/**
 * Analog Audio Check Status Type Definition
 *
 */
typedef enum
{
	AUDIO_SIF_CHECK_STATUS_OFF				= 0, // Initial Status
	AUDIO_SIF_CHECK_STATUS_DETECT_NICAM		= 1, // Detection Process for Nicam
	AUDIO_SIF_CHECK_STATUS_DETECT_A2		= 2, // Detection Process for A2
	AUDIO_SIF_CHECK_STATUS_DETECT_A2DK3		= 3, // Detection Process for DK3
	AUDIO_SIF_CHECK_STATUS_DETECT_FM		= 4, // Detection Process for FM(AM)
	AUDIO_SIF_CHECK_STATUS_DONE				= 5  // Detection Completed
}AUDIO_SIF_CHECK_STATUS_T;

/**
 * Audio Decoder Debug Information Definition
 * Currently Not Used
 *
 */
typedef  struct  AUDIO_DEBUG_INFO
{
	/* AC3 status modes */
	UINT8 ac3_fscod;			/**< FS Code */
	UINT8 ac3_bitratecode;		/**< bitrate code */
	UINT8 ac3_lfe;				/**< low frequency effect */
	UINT8 ac3_acmod;			/**< audio coding mode */
	UINT8 ac3_bsmod;			/**< bs mode */
	UINT8 ac3_bsid;				/**< bs id */
	UINT8 ac3_dsurmod;			/**< surround mode */
	UINT8 ac3_copyright;		/**< copyright */
	UINT8 ac3_origbs;			/**< origbs */
	UINT8 ac3_lancode;			/**< language code */
	UINT8 ac3_langcod;			/**< langcod */
	UINT8 ac3_roomType;			/**< room type */
	UINT8 ac3_mixlevel;			/**< mix level */
	UINT8 ac3_audprodie;		/**< audprodie */

	/* MPEG status mode */
	UINT8 mpeg_bri;     /**<  Bit rate index */
	UINT8 mpeg_p;       /**<  Protection Bit */
	UINT8 mpeg_lay;     /**<  Layer */
	UINT8 mpeg_id;      /**<  Identifier */
	UINT8 mpeg_sfr;     /**<  Sampling Frequency */
	UINT8 mpeg_pad;     /**<  Padding Bit */
	UINT8 mpeg_pri;     /**<  Private Bit */
	UINT8 mpeg_mod;     /**<  Mode */
	UINT8 mpeg_mex;     /**<  Mode Extension */
	UINT8 mpeg_c;       /**<  Coryright */
	UINT8 mpeg_ocb;     /**<  Original/Copy Bit */
	UINT8 mpeg_emp;     /**<  Emphasis rate index */
	UINT8 mpeg_cen;     /**<  Centre */
	UINT8 mpeg_sur;     /**<  Surround */
	UINT8 mpeg_lfe;     /**<  LFE */
	UINT8 mpeg_amx;     /**<  Audio mix */
	UINT8 mpeg_dem;     /**<  Dematrix procedure */
	UINT8 mpeg_ext;     /**<  Extension bitstream present */
	UINT8 mpeg_nml;     /**<  Number of Multi-lingual Channels */
	UINT8 mpeg_mfs;     /**<  Multi-lingual FS */
	UINT8 mpeg_mly;     /**<  Multi-lingual Layer */
	UINT8 mpeg_cib;     /**<  Copyright ID Bit */
	UINT8 mpeg_cis;     /**<  Copyright ID Start */
}  AUDIO_DEBUG_INFO_T ;

/**
 * Audio Transport Stream Input Port Definition
 * Currently Not Used
 *
 */
typedef enum	AUDIO_TP_INSEL
{
	AUDIO_INTTP,
	AUDIO_EXTTP,
	AUDIO_NONTP
} AUDIO_TP_INSEL_T;

/**
 * Analog Audio SIF Type Definition
 *
 */
typedef enum	AUDIO_SIF_TYPE_SEL
{
	MAIN_SIF	=0,
	SUB_SIF
} AUDIO_SIF_TYPE_SEL_T;

/**
 * Basic Equalizer Type Definition
 *
 */
typedef enum AUDIO_BASIC_EQ
{
	AUDIO_BASIC_EQ_OFF	= 0,
	AUDIO_BASIC_EQ_ON	= 1,
	AUDIO_BASIC_EQ_CV	= 2,
	AUDIO_BASIC_EQ_UNDEFINE
} AUDIO_BASIC_EQ_T;

/**
 * PVC Setting Mode Type Definition
 *
 */
typedef enum AUDIO_PVC_MODE
{
	AUDIO_PVC_OFF		= 0,	/* PVC Mode Off */
	AUDIO_PVC_NORMALL	= 1,	/* PVC Mode for normal input */
	AUDIO_PVC_HALF		= 2,	/* PVC Mode for mp3 input */
	AUDIO_PVC_BB		= 3,	/* PVC Mode for Netflix/Yahoo/Youtube input */
	AUDIO_PVC_ATV		= 4,	/* PVC Mode for ATV */
	AUDIO_PVC_UNDEFINE
} AUDIO_PVC_MODE_T;

/**
 * Audio Sound Mode Definition
 * STD, MUSIC, MOVIE, SPORTS, GAME
 *
 */
typedef enum
{
	AUDIO_EZ_SOUND_STANDARD = 0,
	AUDIO_EZ_SOUND_MUSIC,
	AUDIO_EZ_SOUND_MOVIE,
	AUDIO_EZ_SOUND_SPORTS,
	AUDIO_EZ_SOUND_GAME,
	AUDIO_EZ_SOUND_DEFAULT = AUDIO_EZ_SOUND_STANDARD,
} AUDIO_EZ_SOUND_T;

/**
 * Audio Room Mode Definition
 * STD, WALLMOUNT, STAND
 *
 */
typedef enum
{
	AUDIO_CV_ROOMEQ_STANDARD = 0,
	AUDIO_CV_ROOMEQ_WALMOUNT,
	AUDIO_CV_ROOMEQ_STAND,
	AUDIO_CV_ROOMEQ_DEFAULT = AUDIO_CV_ROOMEQ_STANDARD,
} AUDIO_CV_ROOMEQ_T;


/**
 * Headphone Source Type Definition
 *
 */
typedef enum AUDIO_HP_SOURCE
{
	AUDIO_HP_SRC_BEFORE_PP = 0,
	AUDIO_HP_SRC_AFTER_PP = 1,
} AUDIO_HP_SOURCE_T;

/**
 * EMF attribute definitions
 *
 */
typedef enum AUDIO_EMF_SOURCE_ATTR
{
	AUD_EMF_ATTR_PHOTO_MUSIC		= 0,
	AUD_EMF_ATTR_MOVIE,

} AUDIO_EMF_SOURCE_ATTR_T;


/**
 * AMP Type Definition
 *
 */
typedef enum AUDIO_AMPCHIP_TYPE
{
	AUDIO_AMP_NTP7000	= 0,
	AUDIO_AMP_NTP7400	= 1,
	AUDIO_AMP_NTP2AMP	= 2,		/* NTP2AMP	*/
#if 0
	AUDIO_AMP_NTP3AMP	= 3,
#endif
	AUDIO_AMP_NTPSNDBAR	= 3,

	AUDIO_AMP_TASSNDBAR	= 4,
#if 0
	AUDIO_AMP_TAS5713	= 5,
#endif
	AUDIO_AMP_TAS5727	= 5,
#if 0
	AUDIO_AMP_TAS3AMP	= 6,
#endif
	AUDIO_AMP_TAS2AMP	= 6,

	AUDIO_AMP_STA368	= 7,
	AUDIO_AMP_TAS5709	= 8,

	AUDIO_AMP_UNKOWN	= 0xff,
} AUDIO_AMPCHIP_TYPE_T;

/**
 * Audio Amp Chip Status
 *
 */
typedef enum AUDIO_AMP_STATUS
{
	AUD_AMP_NORMAL,
	AUD_AMP_MUTE,	/* Soft Mute */
	AUD_AMP_ABNORMAL,
	AUD_AMP_PWM_OUT_MUTE,
	AUD_AMP_PLL_UNLOCK,
	AUD_AMP_MCLK_INCORRECT,

}AUDIO_AMP_STATUS_T;

/**
 * Wired HP / BT 연결 상태.
 * By Jonghyuk, Lee 090428
 */
typedef enum
{
	HP_CONNECTED_DEFAULT		= 0,
	HP_CONNECTED_BT_STEREO		= 1,
	HP_CONNECTED_BT_MONO		= 2,
	HP_CONNECTED_WIRED_BT_STEREO	= 3,
	HP_CONNECTED_WIRED_BT_MONO		= 4,
	HP_CONNECTED_WIRED			= 5,
	HP_CONNECTED_NOT_CONNECTED	= 6
} AUDIO_HP_INFO_T;

/**
 * Analog Audio Status Information Definition
 *
 */
typedef struct AUDIO_DRV_ANALOG_INFO		/* Analog Audio Status 관련 변수 모음 */
{
	ATV_AUDIO_MODE_SET_T 		audSystemAnalogMode;  		/**<  Analog(NTSC) 음성다중 모드 설정값(by Task) */
	ATV_AUDIO_MODE_SET_T 		audUserAnalogMode;    		/**<  Analog(NTSC) 음성다중 모드 설정값(by UI) */
	ATV_AUDIO_MODE_GET_T		audCurAnalogStatus;   		/**<  Analog(NTSC) 음성다중 모드(by Reading Register) */
	AUDIO_SIF_SOUNDSYSTEM_T		audSoundSystem;
	AUDIO_SIF_STANDARD_T		audAnaAudioStandards;
	AUDIO_SIF_CHECK_STATUS_T	audAnaAudioCheckStatus;
	ATV_AUDIO_NICAM_SIGNATURE_T	audCurNicamSignature;		/** < Analog auido Nicam 인지 여부 (by reading register) */
	UINT16						audA2StereoIdentityLevel;	/**<  Current A2 Stereo Identity Level 값 - KOR ONLY */
	UINT16						audA2ThresholdLevel;		/**<  A2 Threshold Level 값 - KOR ONLY */
	BOOLEAN 					audA2ThresholdLevelDown;	/**<  A2 threshold Down 여부 - KOR ONLY */
	BOOLEAN						audATVScanMode;
	BOOLEAN						audPrevATVScanMode;
	UINT8						audDelayForNTSCDetection;
	UINT16						audDelayForDK;
	UINT16						audDelayForNICAM;
	UINT16						audDelayForPopAudioMode;
	UINT16						audDelayForPopBeforeSetAnalogMode;
	UINT16						audDelayForPopAfterSetAnalogMode;
	UINT16						audThresholdForNTSC;
	UINT16						audThresholdForBTSC;
	BOOLEAN						audIsSIFDetected;			/**< stereo mode detection 여부 */
	BOOLEAN						audIsNoised;
} AUDIO_DRV_ANALOG_INFO_T;

/**
 * Audio Sound Effects Information Definition
 * AVC, BBE, Surround Mode
 *
 */
typedef struct AUDIO_DRV_EFFECT			/* Sound Effect 관련 변수 모음 */
{
	BOOLEAN 			audAVCOnOff;			/**<  AVC ON/OFF  */
	AUDIO_PVC_MODE_T	audPVCMode;				/**<	PVC (Auto Volume) Parameter Mode, 입력 종류에 따라 설정된 PVC mode */
	AUDIO_SURROUND_EFFECT_T 	audSurroundMode;    	/**<  Surround Mode(OFF/ClearVoiceII/SRS) */
	UINT8				audCVLevel;			/**<  Clear Voice II Level */
	AUDIO_EZ_SOUND_T	audezSound;				/**<  uEQ Mode */
	AUDIO_CV_ROOMEQ_T	audcvRoomEQ;			/**<  uEQ2 Mode */
	BOOLEAN 			audBassEQOnOff;			/**<  Bass EQ ON/OFF  */
} AUDIO_DRV_EFFECT_T;

/**
* Define Structure for Audio Delay Value
*
*/
typedef struct AUDIO_DELAY_VALUE
{
	UINT8		audDelayType;
	SINT16		audSpkDelayValue[(AUD_SRC_INVALID+1)];
	SINT16		audDacDelayValue[(AUD_SRC_INVALID+1)];
	SINT16		audSpdifDelayValue[(AUD_SRC_INVALID+1)];
} AUDIO_DELAY_VALUE_T;

/**
 * Audio Sound Effects Information Definition
 * AVC, BBE, Surround Mode
 *
 */
typedef struct AUDIO_DRV_DELAY			/* Audio Delay Value  */
{
	BOOLEAN		curDelayOnOff;
	SINT32		curSpkDelay;
	SINT32		curDacDelay;
	SINT32		curSpdifDelay;
	SINT32		curAdjustDelay;

	const AUDIO_DELAY_VALUE_T 	*audConstDelay;

#if 0
	const AUDIO_DELAY_VALUE_T 	*audSpkConstDelay;					/**<  Surround Mode(OFF/ClearVoiceII/SRS) */
	const AUDIO_DELAY_VALUE_T	*audDacConstDelay;
	const AUDIO_DELAY_VALUE_T	*audSpdifConstDelay;
#endif
	BOOLEAN		audUserDelayOnOff[AUD_DELAY_SRC_INDEX_MAX];
	SINT8		audSpkUserDelay[AUD_DELAY_SRC_INDEX_MAX];	/* user delay setting value for speaker */
	SINT8		audSpdifUserDelay[AUD_DELAY_SRC_INDEX_MAX];	/* user delay setting value for spdif */
	SINT8		audAdjustDelay[AUD_DELAY_SRC_INDEX_MAX];	/**<  in-start menu DelayValue */

} AUDIO_DRV_DELAY_T;


/**
 * Audio Sound Effects Information Definition
 * AVC, BBE, Surround Mode
 *
 */
typedef struct AUDIO_DELAY_CONTRL			/* Audio Delay Value  */
{
	BOOLEAN		bUserDelayOnOff;	/**<  user delay on/off */
	SINT8		userSpkDelay;		/**<  user delay for spk out, -5 ~ 15 */
	SINT8		userSpdifDelay;		/**<  user delay for spdif out, -5 ~ 15 */
	SINT8		adjustSpkDelay;		/**<  adjust menu delay for spk, -10 ~ 20 */
#if	0
	SINT8		adjustSpdifDelay;	/**<  current treble :: 0 ~ 100 */
	SINT8		userDACDelay;		/**<  current_Description :: -15 ~ 15 */
	SINT8		adjustDACDelay;		/**<  current surround */
#endif
	UINT8		bUserDelayOnOffMask	: 1,	/**< 0. flag for volume masking */
				userSpkDelayMask	: 1,	/**< 1. flag for balance masking */
				userSpdifDelayMask	: 1,	/**< 2. flag for clearvoice masking */
				adjustSpkDelayMask	: 1;	/**< 3. flag for audio_description masking */
			#if	0
				adjustSpdifMask		: 1,	/**< 4.  flag for bass masking */
				userDacDelayMask	: 1,	/**< 5.  flag for treble masking */
				adjustDacDelayMask	: 1, 	/**< 6.  flag for clearvoice masking */
				reservedMask		: 1;	/**< 7.  flag for audio_description masking */
			#endif	    	/**<  Surround Mode(OFF/ClearVoiceII/SRS) */
} AUDIO_DELAY_CONTRL_T;


/**
 * Audio Debug Information Definition
 * Source, Type, Mute, Volume 관련 변수 모음.
 *
 */
typedef struct AUDIO_DRV_COMMON			/* Source, Type, Mute, Volume 관련 변수 모음 */
{
	BOOLEAN					audBlockOnOff;					/**<  Current Audio Block 여부 */
	BOOLEAN					audSCARTOutBlockOnOff;			/**<  Current Audio TVOut Block 여부 */
	UINT8					audVolumeMute;					/**<  Current Speaker Block On 여부 */
	UINT8					audUiVolumeMute;				/**<  Current Speaker 조용히 여부 */
	UINT8					audMuteVolume;					/**<  Current 조용히 설정 시 Volume */
	UINT8					audSysDBVolume;					/**<  Current System DB Volume(UI Volume) */
	/* 이상 3 변수는 DDI_AUDIO_GetMuteState(UINT8 *volume)함수를 위한 API 변수 Backup용 변수 */
	UINT8 					audCurVolume;					/**<  Current Speaker Volume */
	UINT8					audCurBalance;					/**<  Current Balance Value */
	BOOLEAN 				audSCARTOutputMute;				/**<  TV Output Mute 여부 */
	BOOLEAN 				audSpdifOutputMute;				/**<  S/PDIF Output Mute 여부 */
	AUDIO_SOURCE_INFO_T		audPreSetSource;          		/**<  Previous Audio Source */
	AUDIO_SOURCE_INFO_T		audSetSource;          			/**<  Current Audio Source */
	AUDIO_PVR_SOURCE_T		audSetPVRSource;				/**<  Current PVR Audio Source */
	AUDIO_SRC_TYPE_T		audSetType;            			/**<  Current Audio Type */
	AUDIO_SPDIF_OUTPUT_MODE_T		audSpdifOutType;		/**<  Current SPDIF Output Type */
	BOOLEAN					audAnalogAudTaskMute;			/**<  Current Analog Audio Task Mute 여부 */
	AUDIO_SPK_OUTPUT_MODE_T audSPKOutMode;
	UINT8					audHDEVmode;					/**<	High Deviation Mode 여부 */
	BOOLEAN					audParametricEQOnOff;			/**< 	Parametric EQ(SPK EQ) on/off control   */
	AUDIO_BASIC_EQ_T		audSysPanoramicEQ;				/**< 	GP 에서 사용치 않음.  CV와 ParaEQ 간의 dependency가 있을때 사용한 것으로 보임.	*/
	BOOLEAN					audHeadsetConnection;			/**<	유선 헤드셋 연결 여부 */
	AUDIO_HP_INFO_T			audBTStatus;					/**<	HP 및 BT 해드셋 연결 여부 */
	BOOLEAN					audTVSpeakerOnOff;				/**<	TV speaker On/Off 여부 */
	BOOLEAN					audIsInputBlocked;				/**<	현재 입력의 input의 block 여부(UI input block, channel block) */
	BOOLEAN					audAMPMute;						/**<	Amp Mute 여부 */
	BOOLEAN					audHDMIInputBlocked;			/**<	HDMI Input Block 여부 */
	BOOLEAN					audIsDTVtoAutoAV;				/**< 	DTV에서 auto AV 동작 여부 */
	BOOLEAN					audIsPowerOnEnd;				/**<	Power ON end 여부 */
	BOOLEAN					audIsTVoutAvailable;			/**<	현재 입력에 대해 TV out 지원 여부 */
	BOOLEAN					audIsStartSIF;					/**<	SIF decoding start 동작 여부 */
	BOOLEAN					audIsInputSupportMenu;			/**<	현재 입력이 support menu 인지의 여부 */
	BOOLEAN					audIsNeedFastMuteForChCh;		/**<	 Is Fast Mute necessary during channel change*/
	BOOLEAN					audIsPreSourceAutoAV;			/**<	Check previous source is Auto AV or not */
	BOOLEAN					audARCEnable;					/**<	Enable Audio Return Channel */
	BOOLEAN					audSelectedAMPType;				/**<	Sound bar 대응 모델인 경우 현재 AMP type을 판단해 주는 변수 */
	BOOLEAN					audIsSetMonoNecessary;			/**<	BCM 모델에서 DTV/PicWizard 등의 경우에서 입력 전환 시 mono set 필요 */
} AUDIO_DRV_COMMON_T;




/**
 * Audio Control Type
 *
 */
typedef struct AUDIO_CTL_TYPE
{
	/* If Change this structure.
	  YOU MUST CHANGE 'ADEC_CTL_TYPE_T' in adec_ddi.h file !!! */
	UINT32	country_mask;
	UINT8	audModuleMask;
	UINT8	audDelyTypeIndex;
	UINT8	audVolCrvIndex;		/* AMP Volume */
	UINT8	audAmpEqIndex;		/* AMP Parametric EQ */
	UINT8	audAmpCtlIndex;		/* AMP register Control value */
	UINT8	audI2SGainIndex;
	UINT8	audSpdifGainIndex;
} AUDIO_CTL_TYPE_T;

/**
 * Audio Control Type
 *
 */
typedef struct AUDIO_MODEL_TYPE
{
	UINT8	inch_type;
	UINT8	tool_type;
	//UINT8	hw_type;	/* AUD_HW_BASE, AUD_HW_2ND, AUD_HW_2ND */
	//위의 hw type은 module 변경시.. power 변경으로 audio setting이 바뀌는 경우가 있음.

	AUDIO_CTL_TYPE_T	audioCtlTbl[];
} AUDIO_MODEL_TYPE_T;


/**
 * Audio Volume Curve Structure
 *
 */
typedef struct AUDIO_VOL_CURVE
{
	UINT8		audVolCrvIndex;
	UINT16		audVolumeCurve[101];
} AUDIO_VOL_CURVE_T;

/**
* Define Structure for DAC vol setting
*
*/
typedef struct AUDIO_DAC_VOLUME
{
	UINT16		DAC_PAL_BG;
	UINT16		DAC_I_DK;
	UINT16		DAC_L_AM;
	UINT16		DAC_BG_NICAM;
	UINT16		DAC_I_DK_NICAM;
	UINT16		DAC_L_NICAM;

	UINT16		DAC_NTSC_A2;
	UINT16		DAC_BTSC_Mono;
	UINT16		DAC_BTSC_Stereo;
	UINT16		DAC_BTSC_SAP;

	UINT16		DAC_AV;
	UINT16		DAC_DTV;
	UINT16		DAC_DEFAULT;
} AUDIO_DAC_VOLUME_T;




/**
 * Audio Country Mask
 *
 */
typedef enum AUD_COUNTRY_MASK
{
	AUD_DEFAULT_COUNTRY_MASK			= 0x00000000,

	AUD_EU_COUNTRY_MASK 				= 0x00000001,	/* EU		*/
	AUD_CN_COUNTRY_MASK 				= 0x00000002,	/* China 	*/
/*	AUD_ANAL_PAL_COUNTRY_MASK			= 0x00000004,*/	/* Analog Model PAL Area, 중아,아주 아날로그 국가 */
	AUD_ANAL_PAL_ASIA_DEF_COUNTRY_MASK	= 0x00000004,	/* Analog Model PAL Area Default - except India */
	AUD_ANAL_PAL_INDIA_COUNTRY_MASK		= 0x00000008,	/* Analog Model PAL Area - Only India */

	AUD_AU_COUNTRY_MASK 				= 0x00000010,	/* Australia	*/
	AUD_SG_COUNTRY_MASK 				= 0x00000020,	/* Singapore	*/
	AUD_ZA_COUNTRY_MASK 				= 0x00000040,	/* South Africa	*/
	AUD_VN_COUNTRY_MASK 				= 0x00000080,	/* Viet Nam		*/

	AUD_ID_COUNTRY_MASK 				= 0x00000100,	/* Indonesia	*/
	AUD_MY_COUNTRY_MASK 				= 0x00000200,	/* Malaysia		*/
	AUD_IL_COUNTRY_MASK 				= 0x00000400,	/* Israel		*/
	AUD_IR_COUNTRY_MASK 				= 0x00000800,	/* Iran			*/


	AUD_US_COUNTRY_MASK 				= 0x00100000,	/* United States	*/
	AUD_KR_COUNTRY_MASK 				= 0x00200000,	/* Korea, Republic of 	*/
	AUD_TW_COUNTRY_MASK 				= 0x00400000,	/* Taiwan	*/
	AUD_BR_COUNTRY_MASK 				= 0x00800000,	/* Brazil	*/
	AUD_ANAL_NTSC_COUNTRY_MASK			= 0x01000000,	/* Analog Model NTSC Area, 중남미 아날로그 국가 */

	/* Emerging Market Group */
	AUD_EMERGING_MARKET_COUNTRY_MASK 	= AUD_AU_COUNTRY_MASK | AUD_SG_COUNTRY_MASK \
										| AUD_ZA_COUNTRY_MASK | AUD_VN_COUNTRY_MASK \
										| AUD_ID_COUNTRY_MASK | AUD_MY_COUNTRY_MASK \
										| AUD_IL_COUNTRY_MASK | AUD_IR_COUNTRY_MASK,

	/* A-ASIA Market Group */
	AUD_ANAL_PAL_COUNTRY_MASK			= AUD_ANAL_PAL_ASIA_DEF_COUNTRY_MASK	\
										| AUD_ANAL_PAL_INDIA_COUNTRY_MASK,

	/* ATSC Country Group */
	AUD_ATSC_COUNTRY_MASK				= AUD_US_COUNTRY_MASK | AUD_KR_COUNTRY_MASK,

	/* PAL Country Group */
	AUD_PAL_COUNTRY_MASK				= AUD_EU_COUNTRY_MASK | AUD_CN_COUNTRY_MASK \
										| AUD_AU_COUNTRY_MASK | AUD_SG_COUNTRY_MASK \
										| AUD_ZA_COUNTRY_MASK | AUD_VN_COUNTRY_MASK \
										| AUD_ID_COUNTRY_MASK | AUD_MY_COUNTRY_MASK \
										| AUD_IL_COUNTRY_MASK | AUD_IR_COUNTRY_MASK	\
 										| AUD_ANAL_PAL_COUNTRY_MASK,

	/* NT Country Group */
	AUD_NTSC_COUNTRY_MASK				= AUD_ATSC_COUNTRY_MASK | AUD_TW_COUNTRY_MASK \
										| AUD_BR_COUNTRY_MASK | AUD_ANAL_NTSC_COUNTRY_MASK,

	AUD_UNAVAILABLE_COUNTRY_MASK		= 0xFFFFFFFF,

} AUD_COUNTRY_MASK_MASK_T;


/**
 * Audio Module Mask
 *
 */
typedef enum AUD_MODULE_MASK
{
	AUD_MODULE_DEFAULT 	= 0x00,
	AUD_MODULE_LGD		= 0x01,
	AUD_MODULE_CMI		= 0x02,
	AUD_MODULE_AUO		= 0x04,
	AUD_MODULE_SHARP 	= 0x08,
	AUD_MODULE_IPS		= 0x10,
	AUD_MODULE_INNOLUX	= 0x20,

	AUD_MODULE_LGE		= 0x40,
	AUD_MODULE_PANASONIC = 0x80,

	AUD_MODULE_LCD		= 0x3F,
	AUD_MODULE_PDP		= 0xC0,
	AUD_MODULE_ALL		= 0xFF,

	AUD_MODULE_NOT_LGD		= (AUD_MODULE_ALL ^ AUD_MODULE_LGD),
	AUD_MODULE_NOT_CMI		= (AUD_MODULE_ALL ^ AUD_MODULE_CMI),
	AUD_MODULE_NOT_AUO		= (AUD_MODULE_ALL ^ AUD_MODULE_AUO),
	AUD_MODULE_NOT_SHARP	= (AUD_MODULE_ALL ^ AUD_MODULE_SHARP),
	AUD_MODULE_NOT_IPS		= (AUD_MODULE_ALL ^ AUD_MODULE_IPS),
	AUD_MODULE_NOT_INNOLUX	= (AUD_MODULE_ALL ^ AUD_MODULE_INNOLUX),

	AUD_MODULE_NOT_LGE		= (AUD_MODULE_ALL ^ AUD_MODULE_LGE),
	AUD_MODULE_NOT_PANASONIC	= (AUD_MODULE_ALL ^ AUD_MODULE_PANASONIC),

	AUD_MODULE_MASK_END = AUD_MODULE_ALL
} AUD_MODULE_MASK_T;


/**
 * Audio AMP Parametric EQ Mode, if SIF System is SECAM_L, EQ is setted Base EQ Off & SECAM_L EQ
 *
 */
typedef enum AUDIO_AMP_CHANGE_EQ_MODE
{
	AUD_AMP_USE_CURRENT_SETTING		= 0,
	AUD_AMP_CHANGE_EQ_NORMAL		= 1,
	AUD_AMP_CHANGE_EQ_CN_ATV 		= 2,
	AUD_AMP_CHANGE_EQ_OFF			= 3,
	AUD_AMP_CHANGE_EQ_TABLE			= 4,
} AUDIO_AMP_CHANGE_EQ_MODE_T;


/**
 * Audio Country Group Index
 *
 */
typedef enum AUD_COUNTRY_GROUP_INDEX
{
	AUD_COUNTRY_GROUP_DEFAULT,
	AUD_COUNTRY_GROUP_EU = AUD_COUNTRY_GROUP_DEFAULT,
	AUD_COUNTRY_GROUP_AU,
	AUD_COUNTRY_GROUP_CN,
	AUD_COUNTRY_GROUP_AN_PAL,
	AUD_COUNTRY_GROUP_KR_US,
	AUD_COUNTRY_GROUP_BR,
	AUD_COUNTRY_GROUP_TW,
	AUD_COUNTRY_GROUP_AN_NT,
	AUD_COUNTRY_GROUP_MAX
} AUD_COUNTRY_GROUP_INDEX_T;


/**
 * pointer strut for AMP function.
 */
typedef struct AUDIO_DDI_AMP
{
	DTV_STATUS_T		(*pfnInitializeAMP) (void);
	DTV_STATUS_T		(*pfnSetSpeakerVolumeAMP) (UINT8 volSpk);
	DTV_STATUS_T		(*pfnSetBalanceAMP) (UINT8 balance);
	DTV_STATUS_T		(*pfnSetBassAMP) (UINT8 bass);
	DTV_STATUS_T		(*pfnSetTrebleAMP) (UINT8 treble);
	DTV_STATUS_T		(*pfnSetAVCAMP) (BOOLEAN bOnOff);
	DTV_STATUS_T		(*pfnSetVolumeAsInputAMP) (AUDIO_SOURCE_INFO_T eAudioSource, AUDIO_SIF_STANDARD_T sifStandard);
	DTV_STATUS_T		(*pfnSetParametricEqAMP) (BOOLEAN bOnOff);
	DTV_STATUS_T		(*pfnChangeEqModeAMP) (AUDIO_AMP_CHANGE_EQ_MODE_T mode);
	DTV_STATUS_T		(*pfnSetMuteAMP) (BOOLEAN bOnOff);
	DTV_STATUS_T		(*pfnSetPwmOutAMP) (BOOLEAN bOnOff, BOOLEAN bSndbarType);
	DTV_STATUS_T		(*pfnSetMasterClkAMP) (AUDIO_MCLK_REAL_FREQUENCY_T fMasterClk);
	DTV_STATUS_T		(*pfnSelectAMP) (BOOLEAN bSelectAmp);
	AUDIO_AMP_STATUS_T	(*pfnGetAmpStatusAMP) (UINT8 checkType);
	void				(*pfnDebugMenuAMP) (void);
} AUDIO_DDI_AMP_T;

#define MAX_SUPPORT_CHIP	20
/**
 * Runtime binding을 위한 structure
 */
typedef struct AUDIO_DDI_RUNBIND
{
	AUDIO_AMPCHIP_TYPE_T	ampType;
	AUDIO_DDI_AMP_T			*pFuncSet;
} AUDIO_DDI_RUNBIND_T;

/******************************************************************************
	변수 선언 (Variables Declaration)
******************************************************************************/
/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T DDI_AUDIO_GetCurAnalogMode(ATV_AUDIO_MODE_GET_T *pAtvMode);
extern DTV_STATUS_T DDI_AUDIO_Init ( void );
extern DTV_STATUS_T DDI_AUDIO_EnableSCARTOutput(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetAmpMute(BOOLEAN   bOn);
extern UINT16		DDI_AUDIO_GetCurA2ThresholdLevel(void);
extern DTV_STATUS_T DDI_AUDIO_SetA2ThresholdLevel(UINT16 a2ThresholdLvl);
extern DTV_STATUS_T DDI_AUDIO_SetBalance(UINT8 balance);
extern DTV_STATUS_T DDI_AUDIO_SetEqualizerLevel(UINT8 band,UINT8 levelIndex);
extern DTV_STATUS_T DDI_AUDIO_SetBass(UINT8 bass) ;
extern DTV_STATUS_T DDI_AUDIO_SetTreble(UINT8 treble) ;
extern DTV_STATUS_T DDI_AUDIO_SetBBE(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetAutoVolumeControl(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetSCARTOutputMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetATVOutVolume(void);
extern DTV_STATUS_T DDI_AUDIO_SetUserAnalogMode(ATV_AUDIO_MODE_SET_T eMode);
extern DTV_STATUS_T DDI_AUDIO_SetSource (AUDIO_SOURCE_INFO_T  eAudioSource, AUDIO_SRC_TYPE_T eAudioType) ;
extern DTV_STATUS_T DDI_AUDIO_SetSpdifOutputMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetSpdifOutputType( AUDIO_SPDIF_OUTPUT_MODE_T eMode );
DTV_STATUS_T  DDI_AUDIO_SetSpdifOutputSCMS(AUDIO_SPDIF_COPYRIGHT_T copyRight);
DTV_STATUS_T  DDI_AUDIO_EnableAudioReturnChannel(BOOLEAN bOnOff);
DTV_STATUS_T DDI_AUDIO_PlayAudioClip(UINT32 bufSize,void *pClipBufferPointer,UINT32 repeatNumber, AUDIO_SRC_TYPE_T clipType, BOOLEAN bMix, void (* pfnAclipDone)(void));
DTV_STATUS_T DDI_AUDIO_PauseAudioClip(void);
DTV_STATUS_T DDI_AUDIO_ResumeAudioClip(void);
DTV_STATUS_T DDI_AUDIO_StopAudioClip(void);
DTV_STATUS_T DDI_AUDIO_StartAudioDescription(AUDIO_SRC_TYPE_T adType);
DTV_STATUS_T DDI_AUDIO_StopAudioDescription(void);
DTV_STATUS_T DDI_AUDIO_StartDecoding(AUDIO_SRC_TYPE_T adecType);
DTV_STATUS_T DDI_AUDIO_StopDecoding(void);
DTV_STATUS_T DDI_AUDIO_GetAudioDecodingType(AUDIO_SRC_TYPE_T *srcType);//임시 생성한 DDI임.
extern DTV_STATUS_T DDI_AUDIO_SetSurroundEffect(AUDIO_SURROUND_EFFECT_T eSurround);
extern DTV_STATUS_T DDI_AUDIO_SetVolume(UINT8 volume) ;
extern DTV_STATUS_T DDI_AUDIO_SetEqualizerLevel(UINT8 band, UINT8 levelIndex);
/*--------      AUDIO DDI Functions related with MP3      ------------------------------------------*/
extern DTV_STATUS_T DDI_AUDIO_SetSamplingRate(AUDIO_SAMPLING_FREQUENCY_T eSfreq);
/*--------      AUDIO DDI Functions related with Audio Task  ------------------------------------------*/
extern void         DDI_AUDIO_RunSpdifPeriodic(BOOLEAN bUsed);
extern AUDIO_SIF_STANDARD_T DDI_AUDIO_CheckSoundStandard(AUDIO_SIF_SOUNDSYSTEM_T audSoundSystem);
extern AUDIO_SIF_STANDARD_T DDI_AUDIO_GetSoundStandard(void);
extern DTV_STATUS_T DDI_AUDIO_RunAnalogAudioPeriodic(ATV_AUDIO_MODE_SET_T analogAudioMode);
extern void         DDI_AUDIO_EnableAnalogAudioScanPeriodic(BOOLEAN	bOnOff );
extern DTV_STATUS_T DDI_AUDIO_DetectSoundSystem(AUDIO_SIF_SOUNDSYSTEM_T soundSystem, UINT32 *pSifStrength, BOOLEAN bRetryRequested);
extern DTV_STATUS_T DDI_AUDIO_SetSoundSystem(AUDIO_SIF_SOUNDSYSTEM_T sifBand);
extern DTV_STATUS_T DDI_AUDIO_SetSPKOutMode(AUDIO_SPK_OUTPUT_MODE_T outputMode);
extern DTV_STATUS_T DDI_AUDIO_GetAudioScanStatus(BOOLEAN *pbAudScanStatus);
extern DTV_STATUS_T DDI_AUDIO_SetHDEVMode(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetParametricEQ(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetAmpFrequency(AUDIO_MCLK_REAL_FREQUENCY_T fMasterClk);
extern DTV_STATUS_T DDI_AUDIO_SetHPMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetHPOutputType(AUDIO_HP_SOURCE_T hpSrc);
extern DTV_STATUS_T DDI_AUDIO_SetADVolume(UINT8 volume);
extern DTV_STATUS_T DDI_AUDIO_SetClearVoice(UINT8 uCVIIValue);
extern DTV_STATUS_T DDI_AUDIO_SetUEQ(AUDIO_EZ_SOUND_T ezSound);
extern DTV_STATUS_T DDI_AUDIO_SetUEQ2(AUDIO_CV_ROOMEQ_T ezSound);
extern DTV_STATUS_T DDI_AUDIO_SetBassEQOnOff(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetInputBlock(BOOLEAN bOnOff);
extern BOOLEAN DDI_AUDIO_IsSIFDetectCompleted(void);
extern BOOLEAN DDI_AUDIO_IsESExist(void);

extern BOOLEAN DDI_AUDIO_IsSIFDetected(void);

extern BOOLEAN DDI_AUDIO_IsSIFExist(void);
extern BOOLEAN DDI_AUDIO_IsAudioInputExist(SINT32 centerLevel);
extern BOOLEAN DDI_AUDIO_IsAudioInputExistForBSI(SINT32 centerLevel);

extern DTV_STATUS_T DDI_AUDIO_SetDACOutMute(UINT8 PortNum, BOOLEAN bOnOff);
extern AUDIO_HDMI_INPUT_MODE_T DDI_AUDIO_GetHDMIAudioType(void);
DTV_STATUS_T DDI_AUDIO_ForcePCAudioInHDMI(BOOLEAN bForcedPcAudioInHDMI);
DTV_STATUS_T DDI_AUDIO_GetDTVStereoMode(AUDIO_STEREO_MODE_T *pAudioStereoMode);

DTV_STATUS_T DDI_AUDIO_PlayBeep(UINT32 repeatNumber, void (*pfnClip_callback_function)(void) );

DTV_STATUS_T DDI_AUDIO_StartSelfTest(UINT32 repeatNumber, void (*pfnClip_callback_function)(void) );
DTV_STATUS_T DDI_AUDIO_StopSelfTest(void);
DTV_STATUS_T DDI_AUDIO_ClearAnalogAudioStatus( void );
DTV_STATUS_T DDI_AUDIO_SetSyncAV(BOOLEAN bOnOff);
#ifdef INCLUDE_WIRELESS_READY  
void            DDI_AUDIO_SetWirelessInputConnectStatus(BOOLEAN bSet,AUDIO_SOURCE_INFO_T * srcInfo);  
BOOLEAN         DDI_AUDIO_GetWirelessInputConnectStatus(void);  
void            DDI_AUDIO_SetWirelessHDMISrcType(UINT8 srcType);  
void            DDI_AUDIO_GetWirelessInputSource(AUDIO_SOURCE_INFO_T *srcInfo);  
#endif  
extern DTV_STATUS_T DDI_AUDIO_ChangeAMPType(void);



extern BOOLEAN				gAudTvSPKOnOff;         /* Current TV SPK On/Off 상태 */
extern AUDIO_DDI_RUNBIND_T	gRunTimeBindAmpTbl[];
extern int					gMaxBindAmpChip;

UINT8		DDI_AUDIO_GetAudioDelayIndex( AUDIO_SOURCE_INFO_T eSource );
DTV_STATUS_T DDI_AUDIO_SetDelayTime( AUDIO_DELAY_CONTRL_T sDelayControl, BOOLEAN bForced );

#if 0
BOOLEAN DDI_AUDIO_GetPlayBeepStatus(void);
void DTV_AUDIO_SetPlayBeepStatusDone(void);
#endif
DTV_STATUS_T DDI_AUDIO_SelectAMP(BOOLEAN bSelectAmp);
DTV_STATUS_T DDI_AUDIO_GetSNDBARAMPType(BOOLEAN bAmpType);
AUD_COUNTRY_GROUP_INDEX_T AUDIO_GetAudioCountryGroupIndex(void);

#ifdef __cplusplus
}
#endif

#endif  /* _AUDIO_DDI_H_ */

