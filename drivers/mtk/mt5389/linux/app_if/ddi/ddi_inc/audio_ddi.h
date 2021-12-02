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
#include "branches.h"
#ifndef WIN32
#include "common.h"
#else
#include "common_win32.h"
#endif
#include "global_configurations.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/
#undef SUBWOOFER_USED

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/
#if (PLATFORM_TYPE == GNSS_PLATFORM)
#define	_AUDIO_MSP44XX_USED
#else
#undef _AUDIO_MSP44XX_USED
#endif
#define	FAST_NT_MODE_DETECT
#define	MAX_AUDIO2UI_MSG_PARAM 		4


/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
/* ddiaudio.h로부터 이곳으로 통합 */
typedef void (*pfnAudioSendMessageToUI) (UINT32 msg, UINT32 param[MAX_AUDIO2UI_MSG_PARAM]);

/**
 * Audio SPDIF Information Definition
 *
 */
typedef struct AUDIO_SPDIF_INFO
{
	UINT8   spdifInOnOff	:   1;      /**<   SPDIF Input을 가지는 소스인지 여부 나타냄  */
	UINT8   spdifInExist	:   1;      /**<   SPDIF Input의 존재 여부를 나타냄           */
	UINT8   spdifInYype		:   2;      /**<   SPDIF Input이 AC3/PCM/NONE의 여부를 나타냄 */
	UINT8   spdifInSfreq	:   2;      /**<   SPDIF Input의 Sampling Frequency를 나타냄  */
	UINT8   spdifDemOnOff	:   1;      /**<   SPDIF Input의 Preemphasis여부를 나타냄     */
	UINT8   spdifDviSet		:   1;		/**<   SPDIF Input의 DVI Mode 환경설정 여부를 나타냄 */
	UINT8	spdifHdmiSet	:	1;		/**<   SPDIF Input의 HDMI Mode 환경설정 여부를 나타냄 */
}   AUDIO_SPDIF_INFO_T;

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
	AUD_SRC_EPF			= 7,
	AUD_SRC_BLUETOOTH	= 8,
	AUD_SRC_PLAY_HDD	= 9,
	AUD_SRC_BOOT		= 10,
	AUD_SRC_INVALID		= 11
}  AUDIO_SOURCE_T ;

/**
 * Audio Input source Information
 *
 */
typedef struct AUDIO_SOURCE_INFO
{
	UINT32
	srcType		: 16,
	index		: 16;
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
	DDI_SURROUND_ON ,
	DDI_SURROUND_CV,
	DDI_SURROUND_SRS
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
	AUDIO_SIF_DK3_A2		= 9,
	AUDIO_SIF_L_NICAM		= 10,
	AUDIO_SIF_L_AM			= 11,
	AUDIO_SIF_MN_A2			= 12,
	AUDIO_SIF_MN_BTSC		= 13,
	AUDIO_SIF_MN_EIAJ		= 14,
	AUDIO_SIF_STD_END		= 15,	/* 20080724 : Added by Goldman for FE/TU */
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
	AUDIO_SPK_MODE_LR		= 0,
	AUDIO_SPK_MODE_LL		= 1,
	AUDIO_SPK_MODE_RR		= 2,
	AUDIO_SPK_MODE_MIX	= 3
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
 * Headphone Source Type Definition
 *
 */
typedef enum AUDIO_HP_SOURCE
{
	AUDIO_HP_SRC_BEFORE_PP = 0,
	AUDIO_HP_SRC_AFTER_PP = 1,
} AUDIO_HP_SOURCE_T;

/**
 * AMP Type Definition
 *
 */
typedef enum AUDIO_AMPCHIP_TYPE
{
	AUDIO_AMP_NTP3000	= 0,
	AUDIO_AMP_NTP3100	= 1,
	AUDIO_AMP_TAS5707	= 2,
	AUDIO_AMP_STA339	= 3,
	AUDIO_AMP_TEA6420	= 4,
	AUDIO_AMP_CS5345	= 5,
	AUDIO_AMP_UNKOWN	= 0xff,
} AUDIO_AMPCHIP_TYPE_T;


/**
 * Analog Audio Status Information Definition
 *
 */
typedef struct AUDIO_DRV_ANALOG_INFO		/* Analog Audio Status 관련 변수 모음 */
{
	ATV_AUDIO_MODE_GET_T 		audSystemAnalogMode;  		/**<  Analog(NTSC) 음성다중 모드 설정값(by Task) */
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
} AUDIO_DRV_ANALOG_INFO_T;

/**
 * Audio Sound Effects Information Definition
 * AVC, BBE, Surround Mode
 *
 */
typedef struct AUDIO_DRV_EFFECT			/* Sound Effect 관련 변수 모음 */
{
	BOOLEAN 			audAVCOnOff;			/**<  AVC ON/OFF  */
	BOOLEAN				audSetBBEOnOff;			/**<  BBE 설정 여부 */
	AUDIO_SURROUND_EFFECT_T 	audSurroundMode;    	/**<  Surround Mode(OFF/SURROUND/SRS) */

} AUDIO_DRV_EFFECT_T;

/**
 * Audio Debug Information Definition
 * Source, Type, Mute, Volume 관련 변수 모음.
 *
 */
typedef struct AUDIO_DRV_COMMON			/* Source, Type, Mute, Volume 관련 변수 모음 */
{
	BOOLEAN					audBlockOnOff;					/**<  Current Audio Block 여부 */
	BOOLEAN					audTVOutBlockOnOff;				/**<  Current Audio TVOut Block 여부 */
	UINT8					audVolumeMute;					/**<  Current Speaker Block On 여부 */
	UINT8					audUiVolumeMute;				/**<  Current Speaker 조용히 여부 */
	UINT8					audMuteVolume;					/**<  Current 조용히 설정 시 Volume */
	UINT8					audSysDBVolume;					/**<  Current System DB Volume(UI Volume) */
	/* 이상 3 변수는 DDI_AUDIO_GetMuteState(UINT8 *volume)함수를 위한 API 변수 Backup용 변수 */
	UINT8 					audCurVolume;					/**<  Current Speaker Volume */
	UINT8					audCurBalance;					/**<  Current Balance Value */
	BOOLEAN 				audMonitorOutputMute;			/**<  Monitor Output Mute 여부 */
	BOOLEAN 				audTVOutputMute;			/**<  TV Output Mute 여부 */
	BOOLEAN 				audSpdifOutputMute;				/**<  S/PDIF Output Mute 여부 */
	AUDIO_SOURCE_INFO_T		audSetSource;          			/**<  Current Audio Source */
	AUDIO_PVR_SOURCE_T		audSetPVRSource;				/**<  Current PVR Audio Source */
	AUDIO_SRC_TYPE_T		audSetType;            			/**<  Current Audio Type */
	AUDIO_SPDIF_OUTPUT_MODE_T		audSpdifOutType;				/**<  Current SPDIF Output Type */
	BOOLEAN					audAnalogAudTaskMute;			/**<  Current Analog Audio Task Mute 여부 */
	AUDIO_SPK_OUTPUT_MODE_T audSPKOutMode;
	UINT8					audHDEVmode;					/**<	High Deviation Mode 여부 */
	UINT8					audPanoramicEQ;					/** < 	Audio EQ on/off control   */
	UINT8					audStartAudMode;				/**<	Start Audio 사용 여부	*/
	AUDIO_BASIC_EQ_T		audSysPanoramicEQ;
} AUDIO_DRV_COMMON_T;

/**
 * pointer strut for AMP function.
*/
typedef struct AUDIO_DDI_AMP
{
	DTV_STATUS_T	(*pfnInitializeAMP) (void);
	DTV_STATUS_T	(*pfnSetSpeakerVolumeAMP) (UINT8 volSpk);
	DTV_STATUS_T	(*pfnSetBalanceAMP) (UINT8 balance);
	DTV_STATUS_T	(*pfnSetBassAMP) (UINT8 bass);
	DTV_STATUS_T	(*pfnSetTrebleAMP) (UINT8 treble);
	DTV_STATUS_T	(*pfnSetAVCAMP) (BOOLEAN bOnOff);
	DTV_STATUS_T	(*pfnSetVolumeAsInputAMP) (AUDIO_SOURCE_INFO_T eAudioSource, AUDIO_SIF_SOUNDSYSTEM_T soundSystem);
	DTV_STATUS_T	(*pfnParametricEQSettingOnAMP) (void);
	DTV_STATUS_T	(*pfnParametricEQSettingOffAMP) (void);
	DTV_STATUS_T	(*pfnFastMuteAMP) (BOOLEAN bOn);
	DTV_STATUS_T	(*pfnPwmMaskResetAMP) (BOOLEAN bOn);
	DTV_STATUS_T	(*pfnSetMasterClockAMP) (AUDIO_SAMPLING_FREQUENCY_T eFs);
	DTV_STATUS_T	(*pfnSetMasterClockRealFreqAMP) (AUDIO_MCLK_REAL_FREQUENCY_T fMasterClk);
	DTV_STATUS_T	(*pfn7BandGraphicEQAMP) (UINT8 band,UINT8 levelIndex);
	void			(*pfnDebugMenuAMP) (void);
} AUDIO_DDI_AMP_T;


#define MAX_SUPPORT_CHIP	20
/**
 * Runtime binding을 위한 structure
 */
typedef struct _run_bind_amp_chip_
{
	AUDIO_AMPCHIP_TYPE_T	ampType;
	AUDIO_DDI_AMP_T			*funcSet;
} AUDIO_DDI_RUNBIND_T;

/******************************************************************************
	변수 선언 (Variables Declaration)
******************************************************************************/
/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T DDI_AUDIO_GetCurAnalogMode(ATV_AUDIO_MODE_GET_T *pAtvMode);
extern DTV_STATUS_T DDI_AUDIO_Init ( void );
extern DTV_STATUS_T DDI_AUDIO_EnableMonitorOutput(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_EnableTVOutput(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetAmpMute(BOOLEAN   bOn);
extern UINT16		 DDI_AUDIO_GetCurA2ThresholdLevel(void);
extern DTV_STATUS_T DDI_AUDIO_SetA2ThresholdLevel(UINT16 a2ThresholdLvl);
extern DTV_STATUS_T DDI_AUDIO_SetBalance(UINT8 balance);
extern DTV_STATUS_T DDI_AUDIO_SetEqualizerLevel(UINT8 band,UINT8 levelIndex);
extern DTV_STATUS_T DDI_AUDIO_SetBass(UINT8 bass) ;
extern DTV_STATUS_T DDI_AUDIO_SetTreble(UINT8 treble) ;
extern DTV_STATUS_T DDI_AUDIO_SetBBE(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetAutoVolumeControl(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetMonitorOutputMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetTVOutputMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetUserAnalogMode(ATV_AUDIO_MODE_SET_T eMode);
extern DTV_STATUS_T DDI_AUDIO_SetSource (AUDIO_SOURCE_INFO_T  eAudioSource, AUDIO_SRC_TYPE_T eAudioType) ;
extern DTV_STATUS_T DDI_AUDIO_SetSpdifOutputMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetSpdifOutputType( AUDIO_SPDIF_OUTPUT_MODE_T eMode );
extern DTV_STATUS_T DDI_AUDIO_SetSurroundEffect(AUDIO_SURROUND_EFFECT_T eSurround);
extern DTV_STATUS_T DDI_AUDIO_SetVolume(UINT8 volume) ;
extern DTV_STATUS_T DDI_AUDIO_SetEqualizerLevel(UINT8 band, UINT8 levelIndex);
/*--------      AUDIO DDI Functions related with MP3      ------------------------------------------*/
extern DTV_STATUS_T DDI_AUDIO_SetSamplingRate(AUDIO_SAMPLING_FREQUENCY_T eSfreq);
/*--------      AUDIO DDI Functions related with Audio Task  ------------------------------------------*/
extern void         DDI_AUDIO_RunSpdifPeriodic(void);
extern AUDIO_SIF_STANDARD_T DDI_AUDIO_CheckSoundStandard(AUDIO_SIF_SOUNDSYSTEM_T audSoundSystem);
extern AUDIO_SIF_STANDARD_T DDI_AUDIO_GetSoundStandard(void);
extern DTV_STATUS_T DDI_AUDIO_RunAnalogAudioPeriodic(ATV_AUDIO_MODE_SET_T analogAudioMode);
extern void         DDI_AUDIO_EnableAnalogAudioScanPeriodic(BOOLEAN	bOnOff );
extern DTV_STATUS_T DDI_AUDIO_DetectSoundSystem(AUDIO_SIF_SOUNDSYSTEM_T soundSystem, UINT32 *pSifStrength);
extern DTV_STATUS_T DDI_AUDIO_SetSoundSystem(AUDIO_SIF_SOUNDSYSTEM_T sifBand);
extern DTV_STATUS_T DDI_AUDIO_SetSPKOutMode(AUDIO_SPK_OUTPUT_MODE_T outputMode);
extern DTV_STATUS_T DDI_AUDIO_GetAudioScanStatus(BOOLEAN *pbAudScanStatus);
extern DTV_STATUS_T DDI_AUDIO_SetHDEVMode(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetPanoramicEQ(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetStartAudioMode(UINT8 soundMode);
extern DTV_STATUS_T DDI_AUDIO_SetAmpFrequency(AUDIO_MCLK_REAL_FREQUENCY_T fMasterClk);
extern DTV_STATUS_T DDI_AUDIO_GetHPCon(BOOLEAN *pbOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetHPMute(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetHPOutputType(AUDIO_HP_SOURCE_T hpSrc);
extern DTV_STATUS_T DDI_AUDIO_SetBTMonoPath(BOOLEAN bOnOff);
extern DTV_STATUS_T DDI_AUDIO_SetADVolume(UINT8 volume);
extern DTV_STATUS_T DDI_AUDIO_SetClearVoiceII(UINT8 uCVIIValue);
extern DTV_STATUS_T DDI_AUDIO_SetUEQ(UINT8 uEQMode);


extern BOOLEAN				gAudioTaskPrintOn;	/* Auido Task Debug Print On/Off Flag */
extern BOOLEAN				gAudTvSPKOnOff;         /* Current TV SPK On/Off 상태 */
extern BOOLEAN				gAudTvMNTOnOff;         /* Current TV MNT On/Off 상태       */
extern AUDIO_DDI_RUNBIND_T	gRunTimeBindAmpTbl[];
extern int					gMaxBindAmpChip;

#endif  /* _AUDIO_DDI_H_ */

