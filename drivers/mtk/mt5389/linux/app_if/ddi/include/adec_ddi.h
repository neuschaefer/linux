/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file adec_ddi.h
 *
 *  ADEC DD를 제어하는 DDI 함수 header 파일.
 *  AUDIO DDI와 ADEC DD를 연결하는 함수들로 주로 AUDIO DDI와 task에서 호출된다.
 *
 *  @author		Park Jeong-Gun(alwaysok@lge.com)
 *  @version	1.0
 *  @date		2006.04.24
 *  @note
 *  @see		adec_ddi.c
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _ADEC_DDI_H_
#define _ADEC_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "osa_api.h"
#include "global_configurations.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
#define	USER_DELAY_UNIT	(10)

#ifndef USE_CUR_USER_DELAY
#define	USE_CUR_USER_DELAY ((SINT8)(0x80))
#define	USE_CUR_CONSTANT_DELAY_TIME	(0xFFFFFFFF)
#define	CHANGE_CUR_USER_DELAY_DB	(0xF0F0F0F0)
#define	CHANGE_CUR_MVD_DELAY_DB	(0xF0F0F0F0)
#endif

/**
 * ADEC Source Input Type.
 * TP, PCM, SPDIF
 *
 */
typedef  enum
{
	ADEC_IN_PORT_NONE		=  0,
	ADEC_IN_PORT_TP			=  1,	// From TPA Stream Input
	ADEC_IN_PORT_SPDIF 		=  2,	// From SERIAL INTERFACE 0
	ADEC_IN_PORT_SIF		=  3,	// From Analog Front End (SIF)
	ADEC_IN_PORT_ADC		=  4,	// Fron ADC Input
	ADEC_IN_PORT_HDMI		=  5,	// From HDMI
	ADEC_IN_PORT_I2S		=  6,	// From I2S
	ADEC_IN_PORT_SYSTEM		=  7,	// From System
#if (PLATFORM_TYPE == LG_PLATFORM)
/* [L9] jongsang.oh (2011/07/19) - LG configuration */

///< To play, set 2nd input port and to stop, set LX_ADEC_IN_PORT_NONE_2ND enum for temp.
	ADEC_IN_PORT_NONE_2ND	=  10,	// No Input for dual decoding
	ADEC_IN_PORT_TP_2ND		=  11,	// From TPA Stream Input for dual decoding
	ADEC_IN_PORT_SPDIF_2ND 	=  12,	// From SERIAL INTERFACE 0 for dual decoding
	ADEC_IN_PORT_SIF_2ND	=  13,	// From Analog Front End (SIF) for dual decoding
	ADEC_IN_PORT_ADC_2ND	=  14,	// Fron ADC Input for dual decoding
	ADEC_IN_PORT_HDMI_2ND	=  15,	// From HDMI for dual decoding
	ADEC_IN_PORT_I2S_2ND	=  16,	// From I2S for dual decoding
	ADEC_IN_PORT_SYSTEM_2ND	=  17,	// From System for dual decoding
#endif
} ADEC_IN_PORT_T;

/**
 * ADEC Source Format Type.
 *
 */
typedef  enum
{
	ADEC_SRC_TYPE_UNKNOWN       = 0,
	ADEC_SRC_TYPE_PCM           = 1,
	ADEC_SRC_TYPE_AC3           = 2,
	ADEC_SRC_TYPE_EAC3          = 3,
	ADEC_SRC_TYPE_MPEG          = 4,
	ADEC_SRC_TYPE_AAC           = 5,
	ADEC_SRC_TYPE_HEAAC         = 6,
	ADEC_SRC_TYPE_MP3           = 7,
	ADEC_SRC_TYPE_WMA           = 8,
	ADEC_SRC_TYPE_DTS           = 9,
	ADEC_SRC_TYPE_SIF           = 10,
	ADEC_SRC_TYPE_SIF_BTSC      = 11,
	ADEC_SRC_TYPE_SIF_A2        = 12,
	ADEC_SRC_TYPE_DEFAULT       = 13,
	ADEC_SRC_TYPE_XPCM			= 14,//16,
	ADEC_SRC_TYPE_WMA_PRO		= 15,//20,
	ADEC_SRC_TYPE_G729			= 16,//22,
	ADEC_SRC_TYPE_DTSBPS		= 17,//23,
	ADEC_SRC_TYPE_PCMWAV		= 18,//24,
	ADEC_SRC_TYPE_NONE			= 19,//25,
} ADEC_SRC_TYPE_T;

/**
 * ADEC SPDIF Type.
 *
 */
typedef  enum
{
	ADEC_SPDIF_NONE			= 0,
	ADEC_SPDIF_PCM			= 1,
	ADEC_SPDIF_AC3			= 2,
	ADEC_SPDIF_MP1			= 3,
	ADEC_SPDIF_MP2			= 4,
	ADEC_SPDIF_MP3			= 5,
	ADEC_SPDIF_AAC			= 6,
	ADEC_SPDIF_DTS			= 7
}  ADEC_SPDIF_MODE_T ;

/**
 * ADEC Clip Play Type.
 *
 */
typedef enum
{
	ADEC_CLIP_NONE			= 0,
	ADEC_CLIP_PLAY			= 1,
	ADEC_CLIP_DONE			= 2,
	ADEC_CLIP_PAUSE			= 3
} ADEC_CLIP_PLAY_T;

/**
 * ADEC Source Format Type.
 *
 */
typedef enum
{
	ADEC_SPK_MODE_LR		= 0,
	ADEC_SPK_MODE_LL		= 1,
	ADEC_SPK_MODE_RR		= 2,
	ADEC_SPK_MODE_MIX		= 3
}ADEC_SPK_OUTPUT_MODE_T;


 //lip_temp_110517 temp define for distinguish between DVI and No audio(on HDMI)
#define _TEMP_DIVIDE_HDMI_AUDIO_TYPE_
/**
 * ADEC HDMI Format Type.
 *
 */
typedef enum
{
#ifdef _TEMP_DIVIDE_HDMI_AUDIO_TYPE_	//lip_temp_110517
	ADEC_HDMI_DVI			= 0,
	ADEC_HDMI_NO_AUDIO		= 1,
	ADEC_HDMI_PCM			= 2,
	ADEC_HDMI_AC3			= 3,
	ADEC_HDMI_DTS			= 4,
	ADEC_HDMI_DEFAULT
#else
	ADEC_HDMI_DVI			= 0,
	ADEC_HDMI_NO_AUDIO		= 0,
	ADEC_HDMI_PCM			= 1,
	ADEC_HDMI_AC3			= 2,
	ADEC_HDMI_DTS			= 3,
	ADEC_HDMI_DEFAULT
#endif
}ADEC_HDMI_TYPE_T;

/**
 * Sample frequency values merged from Audio InfoFrame and Audio Channel Status
 *
 */
typedef enum
{
    //See CEA-861-D, table 18
	ADEC_SAMPLE_NONE 		= 0,
	ADEC_SAMPLE_32_KHZ		= 1,
	ADEC_SAMPLE_44_1KHZ		= 2,
	ADEC_SAMPLE_48_KHZ		= 3,
	ADEC_SAMPLE_88_2KHZ		= 4,
	ADEC_SAMPLE_96_KHZ		= 5,
	ADEC_SAMPLE_176_4KHZ	= 6,
	ADEC_SAMPLE_192_KHZ		= 7,

	//See IEC60958-3, page 12
	ADEC_SAMPLE_22_05KHZ	= 8,
	ADEC_SAMPLE_24_KHZ		= 9,
	ADEC_SAMPLE_8_KHZ		= 10,
	ADEC_SAMPLE_768_KHZ		= 11
} ADEC_SAMPLE_FREQ_T;

/**
 * Audio Master Clock Frequency Definition
 *
 */
typedef enum
{
	F12_288MHZ,
	F24_576MHZ,
	F18_432MHZ,
	F8_192MHZ,
	F16_384MHZ,
} ADEC_MCLK_REAL_FREQUENCY_T;

/**
 * ADEC Sampling rate for Bluetooth
 *
 */
typedef enum
{
	ADEC_SAMPLERATE_BYPASS	= 0,
	ADEC_SAMPLERATE_48K		= 1,
	ADEC_SAMPLERATE_44_1K	= 2,
	ADEC_SAMPLERATE_32K		= 3,
	ADEC_SAMPLERATE_24K		= 4,
	ADEC_SAMPLERATE_16K		= 5,
	ADEC_SAMPLERATE_12K		= 6,
	ADEC_SAMPLERATE_8K		= 7,
} ADEC_SAMPLERATE_T;

/**
 * ADEC SIF Mode.
 *
 */
typedef enum
{
	ADEC_SIF_MODE_DETECT	= 0,
	ADEC_SIF_BG_NICAM		= 1,
	ADEC_SIF_BG_FM			= 2,
	ADEC_SIF_BG_A2			= 3,
	ADEC_SIF_I_NICAM		= 4,
	ADEC_SIF_I_FM			= 5,
	ADEC_SIF_DK_NICAM		= 6,
	ADEC_SIF_DK_FM			= 7,
	ADEC_SIF_DK1_A2			= 8,
	ADEC_SIF_DK2_A2			= 9,
	ADEC_SIF_DK3_A2			= 10,
	ADEC_SIF_L_NICAM		= 11,
	ADEC_SIF_L_AM			= 12,
	ADEC_SIF_MN_A2			= 13,
	ADEC_SIF_MN_BTSC		= 14,
	ADEC_SIF_MN_EIAJ		= 15,
	ADEC_SIF_NUM_SOUND_STD	= 16,
}ADEC_SIF_STANDARD_T;

/**
 * ADEC SIF Format Type.
 *
 */
typedef enum
{
	ADEC_SIF_NICAM,
	ADEC_SIF_A2,
	ADEC_SIF_FM,
	ADEC_SIF_DETECTING_AVALIBILITY
}ADEC_SIF_AVAILE_STANDARD_T;

/**
 * ADEC Source Format Type.
 *
 */
typedef enum
{
	ADEC_SIF_SYSTEM_BG	= 0x00,	// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_I,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_DK,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_L,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_MN,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	ADEC_SIF_SYSTEM_UNKNOWN = 0xf0
} ADEC_SIF_SOUNDSYSTEM_T;

/**
 * ADEC Source Format Type.
 *
 */
typedef enum
{
	ADEC_SIF_ABSENT	 = 0,
	ADEC_SIF_PRESENT 		 = 1,
	ADEC_SIF_DETECTING_EXSISTANCE
}ADEC_SIF_EXISTENCE_INFO_T;

/**
 * ADEC Source User Delay Index Definition
 *
 */
typedef  enum
{
	AUD_GAIN_DEFAULT		= 0,
	AUD_GAIN_DTV_MPEG,
	AUD_GAIN_DTV_BITSTREAM,
	AUD_GAIN_ATV_PAL_BG_NICAM,
	AUD_GAIN_ATV_PAL_BG_MONO,
	AUD_GAIN_ATV_PAL_BG_STEREO,
	AUD_GAIN_ATV_PAL_IDK,
	AUD_GAIN_ATV_PAL_I_NICAM,
	AUD_GAIN_ATV_PAL_DK_NICAM,
	AUD_GAIN_ATV_PAL_L_NICAM,
	AUD_GAIN_ATV_PAL_L_AM,
	AUD_GAIN_ATV_NTSC_A2,
	AUD_GAIN_ATV_NTSC_A2_SAP,
	AUD_GAIN_ATV_BTSC_MONO,
	AUD_GAIN_ATV_BTSC_STEREO,
	AUD_GAIN_ATV_BTSC_SAP,
	AUD_GAIN_ATV_EIAJ,
	AUD_GAIN_AV,
	AUD_GAIN_COMP,
	AUD_GAIN_RGB_DVI,
	AUD_GAIN_HDMI,
/*	AUD_GAIN_HDMI_PCM,
	AUD_GAIN_HDMI_BITSREAM, */
	AUD_GAIN_PLAY_HDD,
#if 1	// Set by input
	AUD_GAIN_EMF_PHOTO_MUSIC,
	AUD_GAIN_EMF_MOVIE,
#else	// Set by audio format
	AUD_GAIN_EMF_MPEG,
	AUD_GAIN_EMF_BITSTREAM,
#endif
	AUD_GAIN_BLUETOOTH,
	AUD_GAIN_MEDIASHARE,
	AUD_GAIN_PICWZARD,
	AUD_GAIN_BB,
	AUD_GAIN_FLASH,

	AUD_GAIN_INDEX_MAX
}  ADEC_GAIN_INDEX_T;

/**
 * ADEC Audio Control Type
 *
 */
typedef struct ADEC_CTL_TYPE
{
	UINT32	country_mask;
	UINT8	audModuleMask;
	UINT8	audDelyTypeIndex;
	UINT8	audVolCrvIndex;		/* AMP Volume */
	UINT8	audAmpEqIdex;		/* AMP Parametric EQ */
	UINT8	audAmpCtlIndex;		/* AMP register Control value */
	UINT8	audI2SGainIndex;
	UINT8	audSpdifGainIndex;
#if 0
	UINT8	audHpVolIndex;
#endif
} ADEC_CTL_TYPE_T;

/**
 * Audio Decoder Debug Information Definition
 * Source, Type, Input Port 관련 변수 모음.
 *
 */
typedef struct	ADEC_DRV_COMMON			/* Source, Type, Input Port 관련 변수 모음 */
{
	ADEC_SRC_TYPE_T   			curAdecFormat;				/**<  Current Audio Format Type */
	ADEC_SRC_TYPE_T   			prevAdecFormat;				/**<  Previous Audio Format Type */
	ADEC_IN_PORT_T				curAdecInputPort;			/**<  Current SPDIF Output Type */
	ADEC_SPK_OUTPUT_MODE_T		curAdecSPKmode;
	ADEC_MCLK_REAL_FREQUENCY_T	curAdecMCLKFreq;
	ADEC_SIF_SOUNDSYSTEM_T		curAdecSifBand;
	ADEC_SIF_EXISTENCE_INFO_T	curAdecSifIsNicam;
	ADEC_SIF_EXISTENCE_INFO_T	curAdecSifIsA2;
	ADEC_SIF_STANDARD_T			curAdecSifStandard;
	ADEC_SAMPLE_FREQ_T			curAdecHdmiSampleFreq;
	UINT8						curAdecVolume;				/**<  Current Adec Volume */
	UINT8						curADCPortNum;				/**<  Current ADC Port Number */ 
	BOOLEAN						bAdecStart;					/**<  Current DDI ADEC Start 여부 in the DTV Mode */
	BOOLEAN						bAdecPlay;					/**<  Current DDI ADEC Start 여부 in the other Mode*/
	BOOLEAN						bAdecMute;					/**<  Current ADEC Mute ON/OFF 여부 */
	BOOLEAN						bAdecSpdifOutPCM;			/**<  Current SPDIF Output is PCM? */
	BOOLEAN						bCurAdecSpdifOutPCM;		/**<  Current Set SPDIF Output is PCM? */
	BOOLEAN						bSRSOnOff;					/**<  Current SRS ON/OFF 여부 ? */
	BOOLEAN						bCVOnOff;					/**<  Current CV3 CV ON/OFF 여부 ? */
	BOOLEAN						bAVL2OnOff;					/**<  Current CV3 AVL2 ON/OFF 여부 ? */
	BOOLEAN						bSurroundOnOff;				/**<  Current CV3 Surround ON/OFF 여부 ? */
	BOOLEAN						bUEQOnOff;					/**<  Current CV3 UEQ ON/OFF 여부 ? */
	SINT32						curAdecI2SOutDelay;
	SINT32						curAdecSPDIFOutDelay;
	SINT32						curAdecDACOutDelay;
	BOOLEAN						bAdecESExist;				/**< Current ES 존재 여부 */
	UINT32						curAdecAdditionalDelay;

} ADEC_DRV_COMMON_T;

/**
 * AC3 ES Info
 *
 * @see
*/
typedef struct ADEC_AC3_ES_INFO{
    UINT8 bitRate;
    UINT8 sampleRate;
    UINT8 channelNum;/* it is not fixed yet depend on mstar definition mono=0x0,Lo/Ro= 0x1,L/R/=0x2,L/R/LS/RS =0x3*/
    UINT8  EAC3;       /* AC3 0x0, EAC3 0x1*/
}ADEC_AC3_ES_INFO_T;

/**
 * MPEG ES Info
 *
 * @see
*/
typedef struct ADEC_MPEG_ES_INFO{
    UINT8 bitRate;
    UINT8 sampleRate;
    UINT8 layer;
    UINT8  channelNum; /* mono =0x0,stereo=0x1,multi-channel =0x2*/
}ADEC_MPEG_ES_INFO_T;

/**
 * HE-AAC ES Info
 *
 * @see
*/
typedef struct ADEC_HEAAC_ES_INFO{
    UINT8 version;    		  /*AAC 0x0  HE-AACv1= 0x1,HE-AACv2=0x2 */
    UINT8 transmissionformat;     /* LOAS/LATM =0x0 , ADTS=0x1*/
    UINT8 channelNum;
}ADEC_HEAAC_ES_INFO_T;

/**
 * ADEC_AUDIO_MODE
 *
 * @see
*/
typedef enum ADEC_AUDIO_MODE
{
	ADEC_AUDIO_MODE_MONO	 			= 0,
	ADEC_AUDIO_MODE_JOINT_STEREO 		= 1,
	ADEC_AUDIO_MODE_STEREO		 		= 2,
	ADEC_AUDIO_MODE_DUAL_MONO 			= 3,
	ADEC_AUDIO_MODE_MULTI				= 4,
	ADEC_AUDIO_MODE_UNKNOWN				= 5,
}ADEC_AUDIO_MODE_T;

/**
 * ADEC ES Info
 *
 * @see
*/
typedef struct ADEC_ES_INFO{
    ADEC_SRC_TYPE_T adecFormat;
    ADEC_AUDIO_MODE_T audioMode;
    ADEC_HEAAC_ES_INFO_T heAAcEsInfo;
    ADEC_MPEG_ES_INFO_T  mpegESInfo;
    ADEC_AC3_ES_INFO_T   ac3ESInfo;
}ADEC_ES_INFO_T;

/**
 * SRS parameter
 *
 * @see
*/
typedef struct ADEC_SRSTSXT_PARAMETER
{
	UINT8		inputGain;
	UINT8		dcGain;
	UINT8		trubassGain;
	UINT8		speakerSize;
	UINT8		inputMode;
	UINT8		outputGain;
	UINT8		bypassGain;
#if 0
	UINT8		trubassGain2;	/* Volume 구간에 따른 Gain 조정을 위해 2nd 값을 지정함. */
	UINT8		outputGain2;	/* Volume 구간에 따른 Gain 조정을 위해 2nd 값을 지정함. */
#endif
} ADEC_SRSTSXT_PARAMETER_T;

/**
 * Audio I2S Volume Type
 *
 */
typedef  enum  ADEC_I2S_VOL_TYPE
{
	I2S_VOL_DEFAULT	= 0,

	/* LCD Model */
	I2S_VOL_EU,
	I2S_VOL_EU_LV37,
	I2S_VOL_AU,
	I2S_VOL_NT,
	I2S_VOL_NT_KR,
	I2S_VOL_CN,
	I2S_VOL_BR,
	I2S_VOL_BR_LW95,
	I2S_VOL_TW,
	I2S_VOL_AN_NT,
	I2S_VOL_AN_PAL,
	I2S_VOL_EU_47LW75, //101016 shyang
	I2S_VOL_EU_47LW95, //101213 shyang
	I2S_VOL_BR_LZ97,
	I2S_VOL_AU_LZ96,

	/* PDP Model */
	I2S_VOL_EU_PDP,
	I2S_VOL_AU_PDP,
	I2S_VOL_NT_PDP,
	I2S_VOL_BR_PDP,
	I2S_VOL_TW_PDP,

	/* If you add new index type,
	please add to here. */
	I2S_VOL_END

}  ADEC_I2S_VOL_TYPE_T ;

/**
 * Audio SPDIF Volume Type
 *
 */
typedef  enum  ADEC_SPDIF_VOL_TYPE
{
	SPDIF_VOL_DEFAULT	= 0,
	SPDIF_VOL_EU,
	SPDIF_VOL_EU_LV37,
	SPDIF_VOL_AU,
	SPDIF_VOL_NT,
	SPDIF_VOL_CN,
	SPDIF_VOL_BR,
	SPDIF_VOL_TW,
	SPDIF_VOL_AN_NT,
	SPDIF_VOL_AN_PAL,

	SPDIF_VOL_EU_PDP,
	SPDIF_VOL_AU_PDP,
	SPDIF_VOL_NT_PDP,
	SPDIF_VOL_BR_PDP,
	SPDIF_VOL_TW_PDP,

	/* If you add new index type,
	please add to here. */
	SPDIF_VOL_END

}  ADEC_SPDIF_VOL_TYPE_T ;

/**
 * Audio HP Volume Type
 *
 */
typedef  enum  ADEC_HP_VOL_TYPE
{
	HP_VOL_DEFAULT	= 0,
	HP_VOL_EU,
	HP_VOL_AU,
	HP_VOL_NT,
	HP_VOL_CN,
	HP_VOL_BR,
	HP_VOL_TW,
	HP_VOL_AN_NT,
	HP_VOL_AN_PAL,

	HP_VOL_EU_PDP,
	HP_VOL_AU_PDP,
	HP_VOL_NT_PDP,

	/* If you add new index type,
	please add to here. */
	HP_VOL_END

}  ADEC_HP_VOL_TYPE_T ;

/**
 * ADEC DVR Trick Mode Type.
 *
 */
typedef  enum
{
	ADEC_TRICK_FORCED_STOP_OFF		= 0,	/**< stop mode of video only trick play */
	ADEC_TRICK_FORCED_STOP_ON		= 1,	/**< start mode of video only trick play */
#if 0
	ADEC_TRICK_MUTE_OFF				= 2,	/**< muted : OFF during Trick Play */	//Not used
	ADEC_TRICK_MUTE_ON				= 3,	/**< muted : ON during Trick Play */	//Not used
	ADEC_TRICK_TSM_DISABLED			= 4, 	/**< tsmEnabled : FALSE */			//Not used
	ADEC_TRICK_TSM_ENABLED			= 5, 	/**< tsmEnabled : TRUE */			//Not used
#endif
	ADEC_TRICK_PAUSE				=6, 	/**< rate : 0 - Pause */
	ADEC_TRICK_NORMAL_PLAY			=7, 	/**< rate : NEXUS_NORMAL_DECODE_RATE - Normal Play */
	ADEC_TRICK_SLOW_MOTION_0P5X		=12, 	/**< rate : NEXUS_NORMAL_DECODE_RATE / 2 - 0.5배속 Play */
#if 0// In GP3 ADEC Speed Control is Spec Outed.
	ADEC_TRICK_FAST_FOWARD_1P2X		=8, 	/**< rate : NEXUS_NORMAL_DECODE_RATE x 1.2 - 1.2배속 Play */
	ADEC_TRICK_FAST_FOWARD_1P5X		=9, 	/**< rate : NEXUS_NORMAL_DECODE_RATE x 1.5 - 1.5배속 Play */
	ADEC_TRICK_FAST_FOWARD_2X		=10, 	/**< rate : NEXUS_NORMAL_DECODE_RATE x 2.0 - 2배속 Play */
	ADEC_TRICK_SLOW_MOTION_0P8X		=11, 	/**< rate : NEXUS_NORMAL_DECODE_RATE / 1.25 - 0.8배속 Play */
	ADEC_TRICK_SLOW_MOTION_0P5X		=12, 	/**< rate : NEXUS_NORMAL_DECODE_RATE / 2 - 0.5배속 Play */
#endif
}  ADEC_TRICK_MODE_T ;

/**
 * ADEC AC3/AC3+ ACMOD Type.
 *
 */
typedef  enum
{
	ADEC_AC3ACMOD_TWO_MONO_1_CH1_CH2			= 0,	/* 1+1 */
	ADEC_AC3ACMOD_ONE_CENTER_1_0_C				= 1,	/* 100 */
	ADEC_AC3ACMOD_TWO_CHANNEL_2_0_L_R			= 2,	/* 200 */
	ADEC_AC3ACMOD_THREE_CHANNEL_3_0_L_C_R		= 3,	/* 300 */
	ADEC_AC3ACMOD_THREE_CHANNEL_2_1_L_R_S		= 4,	/* 210 */
	ADEC_AC3ACMOD_FOUR_CHANNEL_3_1_L_C_R_S		= 5,	/* 310 */
	ADEC_AC3ACMOD_FOUR_CHANNEL_2_2_L_R_SL_SR	= 6,	/* 220 */
	ADEC_AC3ACMOD_FIVE_CHANNEL_3_2_L_C_R_SL_SR	= 7,	/* 320 & 321 */
} ADEC_AC3_ACMOD_T ;


/**
 * ADEC AAC ACMOD Type.
 *
 */
typedef  enum
{
	ADEC_AACACMOD_TWO_MONO_1_CH1_CH2			= 0,	/* 1+1 */
	ADEC_AACACMOD_ONE_CENTER_1_0_C				= 1,	/* 100 */
	ADEC_AACACMOD_TWO_CHANNEL_2_0_L_R			= 2,	/* 200 */
	ADEC_AACACMOD_THREE_CHANNEL_3_0_L_C_R		= 3,	/* 300 */
	ADEC_AACACMOD_THREE_CHANNEL_2_1_L_R_S		= 4,	/* 210 */
	ADEC_AACACMOD_FOUR_CHANNEL_3_1_L_C_R_S		= 5,	/* 310 */
	ADEC_AACACMOD_FOUR_CHANNEL_2_2_L_R_SL_SR	= 6,	/* 220 */
	ADEC_AACACMOD_FIVE_CHANNEL_3_2_L_C_R_SL_SR	= 7,	/* 320 & 321 */
} ADEC_AAC_ACMOD_T ;

#if  (PLATFORM_TYPE == LG_PLATFORM)
/* [L8] jongsang.oh (2010/10/01) - LG configuration */

/**
 * ADEC Audio Sample Frequency.
 * Sample frequency values merged from HDMI Audio InfoFrame and Audio Channel Status.
 * Also, this enumeration describes the sampling rate for ADEC driver.
 * If this value is changed, LX_HDMI_SAMPLING_FREQ_T is also changed in HDMI kernel driver.
 */
typedef enum
{
	ADEC_SAMPLING_FREQ_NONE		= 0,		///< None
	ADEC_SAMPLING_FREQ_4_KHZ	= 4,		///< 4 Kbps
	ADEC_SAMPLING_FREQ_8_KHZ	= 8,		///< 8 Kbps
	ADEC_SAMPLING_FREQ_11_025KHZ= 11,		///< 11.025 Kbps
	ADEC_SAMPLING_FREQ_12_KHZ	= 12,		///< 12 kbps
	ADEC_SAMPLING_FREQ_16_KHZ	= 16,		///< 16 Kbps
	ADEC_SAMPLING_FREQ_22_05KHZ	= 22,		///< 22.05 Kbps
	ADEC_SAMPLING_FREQ_24_KHZ	= 24,		///< 24 Kbps
	ADEC_SAMPLING_FREQ_32_KHZ	= 32,		///< 32 Kbps
	ADEC_SAMPLING_FREQ_44_1KHZ	= 44,		///< 44.1 Kbps
	ADEC_SAMPLING_FREQ_48_KHZ	= 48,		///< 48 Kbps
	ADEC_SAMPLING_FREQ_64_KHZ	= 64,		///< 64 Kbps
	ADEC_SAMPLING_FREQ_88_2KHZ	= 88,		///< 88.2 Kbps
	ADEC_SAMPLING_FREQ_96_KHZ	= 96,		///< 96 Kbps
	ADEC_SAMPLING_FREQ_128_KHZ 	= 128,		///< 128 Kbps
	ADEC_SAMPLING_FREQ_176_4KHZ	= 176,		///< 176.4 Kbps
	ADEC_SAMPLING_FREQ_192_KHZ	= 192,		///< 192 Kbps
	ADEC_SAMPLING_FREQ_768_KHZ	= 768,		///< 768 Kbps
	ADEC_SAMPLING_FREQ_DEFAULT	= 999		///< default
} ADEC_SAMPLING_FREQ_T;

/* [L8] jongsang.oh (2010/10/01) - LG configuration */

/**
 * HDMI Auido and Video Info.
 * Get HDMI AUDIO AND VIDEO INFORMATION.
 */
typedef struct
{
  ADEC_HDMI_TYPE_T			audioType;		///< HDMI Audio Type.
  ADEC_SAMPLING_FREQ_T		samplingFreq;	///< Sampling Frequency
  BOOLEAN					bPortConnected;	///< Port connect status (1 : connected, 0 : not connected)
  BOOLEAN					bInterlaced;	///< Scan type (1 : interlace, 0 : progressive)
} ADEC_HDMI_AUDIO_INFO_T;
#endif


/**
 * ADEC SPDIF Copy Protection Type
 *
 */
typedef  enum
{
	ADEC_SPDIF_COPY_FREE		= 0,	/* cp-bit : 1, L-bit : 0 */
	ADEC_SPDIF_COPY_NO_MORE		= 1,	/* cp-bit : 0, L-bit : 1 */
	ADEC_SPDIF_COPY_ONCE		= 2,	/* cp-bit : 0, L-bit : 0 */
	ADEC_SPDIF_COPY_NEVER		= 3,	/* cp-bit : 0, L-bit : 1 */
} ADEC_SPDIF_COPYRIGHT_T ;


typedef enum
{
	ADEC_LGSE_INIT_ONLY	= 0, // init only parameter will be writen.
	ADEC_LGSE_VARIABLES	= 1, // variables will be written.
	ADEC_LGSE_ALL	= 2 // init only parameter and variables wiil be written simultaneously
} ADEC_LGSE_DATA_MODE_T;

typedef enum
{
	ADEC_LGSE_MODE_VARIABLES0	= 0, // VARIABLES_00 will be writen.
	ADEC_LGSE_MODE_VARIABLES1	= 1, // VARIABLES_01 will be written.
	ADEC_LGSE_MODE_VARIABLES2	= 2, // VARIABLES_02 will be written.
	ADEC_LGSE_MODE_VARIABLES3	= 3, // VARIABLES_03 will be written.
	ADEC_LGSE_MODE_VARIABLES4	= 4, // VARIABLES_04 will be written.
	ADEC_LGSE_MODE_VARIABLESALL	= 5 // All VARIABLES will be written simultaneously. Data will be arranged from 0 to 4.
} ADEC_LGSE_VARIABLE_MODE_T;


/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/

/*------ ADEC DDI Functions -------------------------------------------------------------*/
DTV_STATUS_T DDI_ADEC_InitializeModule(void);
DTV_STATUS_T DDI_ADEC_SetSource(ADEC_IN_PORT_T adecSource, UINT8 portNum, ADEC_SRC_TYPE_T adecType);

DTV_STATUS_T DDI_ADEC_CV_SetAVL(UINT32 *pAVLParam);
DTV_STATUS_T DDI_ADEC_CV_SetUEQ(UINT32 *pUEQParams);
DTV_STATUS_T DDI_ADEC_CV_SetUEQ2(UINT32 *pUEQParams);
DTV_STATUS_T DDI_ADEC_CV_SetBassEQ(UINT32 *pBassEQParams);
DTV_STATUS_T DDI_ADEC_CV_SendOSDVolume(UINT32 volParam);
DTV_STATUS_T DDI_ADEC_CV_SetClearVoiceLevel(UINT32 *pCVParams);
DTV_STATUS_T DDI_ADEC_CV_GetInputLevel(SINT32 *pAUDInputLevel);
DTV_STATUS_T DDI_ADEC_CV_SetInfiniteSound(UINT32 *pInfiniteSoundParams);
DTV_STATUS_T DDI_ADEC_CV_SetMode(UINT32 cvMode);

DTV_STATUS_T DDI_ADEC_CV_GetInputLevel_Set(UINT32 *pInputLevelParams);


DTV_STATUS_T DDI_ADEC_SetVolume(UINT8  volume) ;
DTV_STATUS_T DDI_ADEC_SetBalance(UINT8 balance);
DTV_STATUS_T DDI_ADEC_SetBass(UINT8 bass) ;
DTV_STATUS_T DDI_ADEC_SetTreble(UINT8   treble) ;
DTV_STATUS_T DDI_ADEC_SetDACOutVolume(UINT8 portNum, UINT16 volume, BOOLEAN bForced);

DTV_STATUS_T DDI_ADEC_SetADVolume(UINT8 volume);
// Not Used. 2010.07.24
//DTV_STATUS_T DDI_ADEC_SetMixVolume(UINT8 volume);
//DTV_STATUS_T DDI_ADEC_SetEqualizerLevel(UINT8 band,UINT8 levelIndex);
//DTV_STATUS_T DDI_ADEC_SetSRS(BOOLEAN bOnOff);
//DTV_STATUS_T DDI_ADEC_SetBBE(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetTrickState(ADEC_TRICK_MODE_T adedTrickMode);

DTV_STATUS_T DDI_ADEC_SetSPDIFOutputType(ADEC_SPDIF_MODE_T eSpdifMode,BOOLEAN bForced);
DTV_STATUS_T DDI_ADEC_SetSPDIFSCMS(ADEC_SPDIF_COPYRIGHT_T copyRight);
DTV_STATUS_T DDI_ADEC_SetSPDIFMute(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetAudioReturnChannel(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetSPKOutMode(ADEC_SPK_OUTPUT_MODE_T outputMode);
DTV_STATUS_T DDI_ADEC_SetDACOutMute(UINT8 portNum, BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_GetHDMIAudioMode(ADEC_HDMI_TYPE_T *pHdmiMode);

DTV_STATUS_T DDI_ADEC_SetI2SOutDelayTime( SINT32 delayTime, BOOLEAN bForced);
DTV_STATUS_T DDI_ADEC_SetSPDIFOutDelayTime( SINT32 delayTime, BOOLEAN bForced);
DTV_STATUS_T DDI_ADEC_SetDACOutDelayTime( SINT32 delayTime, BOOLEAN bForced);
DTV_STATUS_T DDI_ADEC_SetUserAnalogMode(ATV_AUDIO_MODE_SET_T eMode);
DTV_STATUS_T DDI_ADEC_GetCurAnalogMode(ATV_AUDIO_MODE_GET_T *pATVAudioMode);
DTV_STATUS_T DDI_ADEC_GetBtscA2StereoLevel(UINT16 *pLevel);
DTV_STATUS_T DDI_ADEC_SetBtscA2ThresholdLevel(UINT16 thrLevel);
DTV_STATUS_T DDI_ADEC_GetESInfo(ADEC_ES_INFO_T *pAudioESInfo);
DTV_STATUS_T DDI_ADEC_SetHDEVMode(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SIF_SetBandSetup(ADEC_SIF_SOUNDSYSTEM_T sifBand);
DTV_STATUS_T DDI_ADEC_SIF_GetBandDetect(ADEC_SIF_SOUNDSYSTEM_T soundSystem,UINT32 *pBandStrength, BOOLEAN doubleChecked);


DTV_STATUS_T DDI_ADEC_SIF_SetModeSetup(ADEC_SIF_STANDARD_T sifStandard);
DTV_STATUS_T DDI_ADEC_SIF_CheckNicamDigital(ADEC_SIF_EXISTENCE_INFO_T *pIsNicamDetect);
DTV_STATUS_T DDI_ADEC_SIF_CheckAvailableSystem(ADEC_SIF_AVAILE_STANDARD_T standard,
														ADEC_SIF_EXISTENCE_INFO_T *pAvailability);
DTV_STATUS_T DDI_ADEC_SIF_CheckA2DK( ADEC_SIF_STANDARD_T standard, ADEC_SIF_EXISTENCE_INFO_T *pAvailability );

DTV_STATUS_T DDI_ADEC_SetATVOutVolume(void);
DTV_STATUS_T DDI_ADEC_SetSPDIFVolume(UINT8 volume);
DTV_STATUS_T DDI_ADEC_SetVolumeAsInput(ADEC_GAIN_INDEX_T index);
DTV_STATUS_T DDI_ADEC_InitToolOptionParameter(const ADEC_CTL_TYPE_T * audCtlIndex);

DTV_STATUS_T DDI_ADEC_GetAudioPTS(UINT32 *pPts);

DTV_STATUS_T DDI_ADEC_GetAdecStatus(ADEC_DRV_COMMON_T *pAdecDriverInfo);
DTV_STATUS_T DDI_ADEC_GetDecodingType(ADEC_SRC_TYPE_T *pAdecType);
DTV_STATUS_T DDI_ADEC_StartDecoding(ADEC_SRC_TYPE_T adecType);
DTV_STATUS_T DDI_ADEC_StopDecoding ( void );
DTV_STATUS_T DDI_ADEC_SetAudSyncMode(UINT8 onOff);

DTV_STATUS_T DDI_ADEC_StartAudioDescription(ADEC_SRC_TYPE_T adType);
DTV_STATUS_T DDI_ADEC_StopAudioDescription(void);

DTV_STATUS_T DDI_ADEC_PlayAudioClip(UINT32 bufSize,void *pClipBufferPointer,UINT32 repeatNumber, ADEC_SRC_TYPE_T clipType, BOOLEAN bMix, void (* pfnAclipDone)(void));
DTV_STATUS_T DDI_ADEC_StopAudioClip(void);
DTV_STATUS_T DDI_ADEC_PauseAudioClip(void);
DTV_STATUS_T DDI_ADEC_ResumeAudioClip(void);

BOOLEAN DDI_ADEC_IsESExist(void);

BOOLEAN DDI_ADEC_IsSIFExist(void);

#if (PLATFORM_TYPE == LG_PLATFORM)
/* [L8] jongsang.oh (2010/10/01) - LG configuration */
DTV_STATUS_T DDI_ADEC_GetHDMIAudioInfo(ADEC_HDMI_AUDIO_INFO_T *pHdmiInfo);
DTV_STATUS_T DDI_ADEC_FinalizeModule(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* _ADEC_DDI_H_ */

