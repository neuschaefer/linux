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
#include "branches.h"
#include "common.h"
#include "osa_api.h"
#include "global_configurations.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
#define	ADEC_CRITICAL_ERROR(args...) 	OSA_DBG_CriticalError("[ADEC ERROR] " args)
#undef _ADEC_INTERNAL_SIF_USED

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
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
} ADEC_IN_PORT_T;

/**
 * ADEC Source Format Type.
 *
 */
typedef  enum
{
	ADEC_SRC_TYPE_UNKNOWN	= 0,
	ADEC_SRC_TYPE_PCM		= 1,
	ADEC_SRC_TYPE_AC3		= 2,
	ADEC_SRC_TYPE_EAC3		= 3,
	ADEC_SRC_TYPE_MPEG		= 4,
	ADEC_SRC_TYPE_AAC		= 5,
	ADEC_SRC_TYPE_HEAAC		= 6,
	ADEC_SRC_TYPE_MP3		= 7,
	ADEC_SRC_TYPE_WMA		= 8,
	ADEC_SRC_TYPE_DTS		= 9,
	ADEC_SRC_TYPE_SIF		= 10,
	ADEC_SRC_TYPE_SIF_BTSC	= 11, // added by Allan.Liang
	ADEC_SRC_TYPE_DEFAULT	= 12,
	ADEC_SRC_TYPE_CDLPCM	= 13, //The following is defined by MStar
	ADEC_SRC_TYPE_RA8LBR	= 14,
	ADEC_SRC_TYPE_XPCM		= 15,
	ADEC_SRC_TYPE_NONE		= 16,
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
	ADEC_CLIP_DONE			= 2
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

/**
 * ADEC Source Format Type.
 *
 */
typedef enum
{
	ADEC_HDMI_DVI			= 0,
	ADEC_HDMI_NO_AUDIO,
	ADEC_HDMI_HDMI,
	ADEC_HDMI_DEFAULT
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
	ADEC_SAMPLERATE_16K		= 4,
} ADEC_SAMPLERATE_T;

/**
 * ADEC Channel Mode for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_MONO			= 0,
	ADEC_BT_DUAL_CHANNEL	= 1,
	ADEC_BT_STEREO			= 2,
	ADEC_BT_JOINT_STEREO	= 3,
} ADEC_BT_CHANNEL_MODE_T;

/**
 * ADEC Block Length for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_BLOCK_4		= 0,
	ADEC_BT_BLOCK_8		= 1,
	ADEC_BT_BLOCK_12	= 2,
	ADEC_BT_BLOCK_16	= 3,
} ADEC_BT_BLOCK_LENGTH_T;

/**
 * ADEC Subbands for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_SUBBANDS_4		= 0,
	ADEC_BT_SUBBANDS_8		= 1,
} ADEC_BT_SUBBANDS_T;

/**
 * ADEC Allocation Method for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_ALLOCATION_SNR			= 0,
	ADEC_BT_ALLOCATION_LOUDNESS		= 1,
} ADEC_BT_ALLOCATION_METHOD_T;

/**
 * ADEC Control Information for Bluetooth
 *
 */
typedef struct ADEC_BLUETOOTH_COMMON
{
	void *pBufAddr;
	UINT32 totalBufSize;
	UINT32 bufSize;
	UINT32 frameRate;
	ADEC_SAMPLERATE_T sampleRate;
	BOOLEAN SBCOnOff;
	ADEC_BT_CHANNEL_MODE_T channelMode;
	ADEC_BT_BLOCK_LENGTH_T blockLength;
	ADEC_BT_SUBBANDS_T subBands;
	ADEC_BT_ALLOCATION_METHOD_T allocationMethod;
	UINT8 minBitpool;
	UINT8 maxBitpool;
} ADEC_BT_T;

/**
 * ADEC Callback Status for Bluetooth
 *
 */
typedef enum
{
	ADEC_BT_NONE		= 0,
	ADEC_BT_START		= 1,
	ADEC_BT_STOP		= 2,
	ADEC_BT_NOTIFY		= 3,
	ADEC_BT_OVERFLOW	= 4,
	ADEC_BT_UNDERFLOW	= 5,
} ADEC_BT_MESSAGE_T;

/**
 * ADEC HeadPhone Output Source Type
 *
 */
typedef enum
{
	ADEC_HP_SRC_BEFORE_PP = 0,
	ADEC_HP_SRC_AFTER_PP = 1,
} ADEC_HP_SRC_TYPE_T;

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
 * ADEC Audio Decoding Type.
 *
 */
#if (PLATFORM_TYPE == BCM_PLATFORM)
typedef  enum
{
    ADEC_DECODE_MODE		= 0,
    ADEC_SIMUL_MODE			= 1,
    ADEC_BYPASS_MODE		= 2,
}  ADEC_DECODING_MODE_T ;
#endif

/**
 * Audio Decoder Debug Information Definition
 * Source, Type, Input Port 관련 변수 모음.
 *
 */
typedef struct	ADEC_DRV_COMMON			/* Source, Type, Input Port 관련 변수 모음 */
{
	ADEC_SRC_TYPE_T   			curAdecFormat;				/**<  Current Audio Format Type */
	ADEC_IN_PORT_T				curAdecInputPort;			/**<  Current SPDIF Output Type */
	ADEC_SPK_OUTPUT_MODE_T		curAdecSPKmode;
	ADEC_MCLK_REAL_FREQUENCY_T	curAdecMCLKFreq;
	ADEC_SIF_SOUNDSYSTEM_T		curAdecSifBand;
	ADEC_SIF_EXISTENCE_INFO_T	curAdecSifIsNicam;
	ADEC_SIF_EXISTENCE_INFO_T	curAdecSifIsA2;
	ADEC_SIF_STANDARD_T			curAdecSifStandard;
	ADEC_SAMPLE_FREQ_T			curAdecHdmiSampleFreq;
	UINT8						curAdecVolume;				/**<  Current Adec Volume */
	BOOLEAN						bAdecStart;					/**<  Current DDI ADEC Start 여부 in the DTV Mode */
	BOOLEAN						bAdecPlay;					/**<  Current DDI ADEC Start 여부 in the other Mode*/
	BOOLEAN						bAdecMute;					/**<  Current ADEC Mute ON/OFF 여부 */
	BOOLEAN						bAdecSpdifOutPCM;			/**<  Current SPDIF Output is PCM? */
	BOOLEAN						bCurAdecSpdifOutPCM;		/**<  Current Set SPDIF Output is PCM? */
	BOOLEAN						bSRSOnOff;					/**<  Current SRS ON/OFF 여부 ? */
	BOOLEAN						bCVOnOff;					/**<  Current Clear Voice ON/OFF 여부 ? */
#if (PLATFORM_TYPE == BCM_PLATFORM)
	BOOLEAN						bAdecConnect;				/**<  Current ADEC Connection 여부 */
	ADEC_DECODING_MODE_T		curAdecDecodingMode;		/**<  Current Audio Decoding Mode */
	BOOLEAN						bAdecTSMMode;				/**<  Current TSM Enalbe 여부 */
	BOOLEAN   					Analog_play;				/**<  Analog Audio Play 상태 : ADEC Start가 된 이후에 Analog audio info의 정보를 받을 수 있음 */
	UINT8						curAdecMntVolume;			/**<  Current ADEC Monitor Output Volume */
	BOOLEAN						bClearVoiceOnOff;			/**<  Current Clear Voice ON/OFF 여부 */
#endif

} ADEC_DRV_COMMON_T;

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/

/*--------------------------------------------------------------------------------------*/

/*------ ADEC DDI Functions -------------------------------------------------------------*/
DTV_STATUS_T DDI_ADEC_InitializeModule(void);
DTV_STATUS_T DDI_ADEC_SetSource(ADEC_IN_PORT_T adecSource, UINT8 portNum, ADEC_SRC_TYPE_T adecType);

DTV_STATUS_T DDI_ADEC_SetSPDIFOutputType(ADEC_SPDIF_MODE_T eSpdifMode,BOOLEAN bForced);
DTV_STATUS_T DDI_ADEC_SetSPDIFMute(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetClearVoice( BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetSRS(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetBBE(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetAutoVolumeControl(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetAudioPllSFreq(ADEC_SAMPLE_FREQ_T eSfreq);
DTV_STATUS_T DDI_ADEC_SetEqualizerLevel(UINT8 band,UINT8 levelIndex);

DTV_STATUS_T DDI_ADEC_LoadAudioClip(UINT32 bufSize,void *pClipBufferPointer);
DTV_STATUS_T DDI_ADEC_DeleteAudioClip(void);
DTV_STATUS_T DDI_ADEC_PlayAudioClip(UINT32 repeatNumber, void (*pfnAclipDone)(void));
DTV_STATUS_T DDI_ADEC_StopAudioClip(void);
DTV_STATUS_T DDI_ADEC_PauseAudioClip(void);
DTV_STATUS_T DDI_ADEC_ResumeAudioClip(void);

DTV_STATUS_T DDI_ADEC_GetDecodingType(ADEC_SRC_TYPE_T *pAdecType);
DTV_STATUS_T DDI_ADEC_StartDecoding(ADEC_SRC_TYPE_T adecType);
DTV_STATUS_T DDI_ADEC_StopDecoding ( void );
DTV_STATUS_T DDI_ADEC_SetVolume(UINT8  volume) ;
DTV_STATUS_T DDI_ADEC_SetBalance(UINT8 balance);
DTV_STATUS_T DDI_ADEC_SetBass(UINT8 bass) ;
DTV_STATUS_T DDI_ADEC_SetTreble(UINT8   treble) ;
DTV_STATUS_T DDI_ADEC_SetUserAnalogMode(ATV_AUDIO_MODE_SET_T eMode);
DTV_STATUS_T DDI_ADEC_GetCurAnalogMode(ATV_AUDIO_MODE_GET_T *pATVAudioMode);
DTV_STATUS_T  DDI_ADEC_GetCurAnalogAudioStatus(ATV_AUDIO_MODE_GET_T *pAtvMode);
DTV_STATUS_T DDI_ADEC_SetSPKOutMode(ADEC_SPK_OUTPUT_MODE_T outputMode);
DTV_STATUS_T DDI_ADEC_SetDACOutMute(UINT8 portNum, BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_GetHDMIAudioMode(ADEC_HDMI_TYPE_T *pHdmiMode);
DTV_STATUS_T DDI_ADEC_SetPCMDelayTime(UINT32 delayTime);
DTV_STATUS_T DDI_ADEC_GetAdecStatus(ADEC_DRV_COMMON_T *pAdecDriverInfo);
DTV_STATUS_T DDI_ADEC_GetBtscA2StereoLevel(UINT16 *pLevel);
DTV_STATUS_T DDI_ADEC_SetBtscA2ThresholdLevel(UINT16 thrLevel);
DTV_STATUS_T DDI_ADEC_SetHDEVMode(BOOLEAN bOnOff);

DTV_STATUS_T DDI_ADEC_SIF_EnableDACOut(UINT8 portNum, BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetDACOutVolume(UINT8 portNum, UINT8 volume);
DTV_STATUS_T DDI_ADEC_SetADVolumeDBG(UINT16 u8Volume );

DTV_STATUS_T DDI_ADEC_StartUploadPCM(ADEC_BT_T *btData, void (*pfnAdecBTNotify) (ADEC_BT_MESSAGE_T msg));
DTV_STATUS_T DDI_ADEC_StopUploadPCM(void);
DTV_STATUS_T DDI_ADEC_StartDownloadPCM(ADEC_BT_T *btData, void (*pfnAdecBTNotify) (ADEC_BT_MESSAGE_T msg));
DTV_STATUS_T DDI_ADEC_StopDownloadPCM(void);
DTV_STATUS_T DDI_ADEC_SetADVolume(UINT8 volume);
DTV_STATUS_T DDI_ADEC_SetMixVolume(UINT8 volume);
DTV_STATUS_T DDI_ADEC_SetAudioDescription(BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetHPOutputType(ADEC_HP_SRC_TYPE_T srcType);

#if 1//((PLATFORM_TYPE == MSTAR_PLATFORM) && (PLATFORM_TYPE == MTK_PLATFORM)) /* sebeom 08 13 temporal audio description function for France spec*/
DTV_STATUS_T DDI_ADEC_StartAudioDescription(ADEC_SRC_TYPE_T adType);
DTV_STATUS_T DDI_ADEC_StopAudioDescription(void);
DTV_STATUS_T DDI_ADEC_SetUEQ( UINT32 *eqLevel);
#endif
DTV_STATUS_T DDI_ADEC_SetClearVoiceII( UINT32 *cvParam);
DTV_STATUS_T DDI_ADEC_SetClearVoiceOnOff( BOOLEAN bOnOff);
DTV_STATUS_T DDI_ADEC_SetPVC( UINT32 *avLevel);


//SIF Functions by Chaplin
DTV_STATUS_T DDI_ADEC_SIF_SetBandSetup(ADEC_SIF_SOUNDSYSTEM_T sifBand);
DTV_STATUS_T DDI_ADEC_SIF_GetBandDetect(ADEC_SIF_SOUNDSYSTEM_T soundSystem,UINT32 *pBandStrength);
DTV_STATUS_T DDI_ADEC_SIF_SetModeSetup(ADEC_SIF_STANDARD_T sifStandard);
DTV_STATUS_T DDI_ADEC_SIF_CheckNicamDigital(ADEC_SIF_EXISTENCE_INFO_T *pIsNicamDetect);
DTV_STATUS_T DDI_ADEC_SIF_CheckAvailableSystem(ADEC_SIF_AVAILE_STANDARD_T standard,
														ADEC_SIF_EXISTENCE_INFO_T *pAvailability);
#ifdef INCLUDE_DVR_READY
DTV_STATUS_T DDI_ADEC_SyncAV(UINT8 port , UINT8 onOff);
BOOLEAN	DDI_ADEC_CheckAudioDecoderStart(void);
#endif
DTV_STATUS_T DDI_ADEC_MainSpeakerStatusChange(BOOLEAN bOnOff);
#endif /* _ADEC_DDI_H_ */

