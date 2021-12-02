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
/**
 * @file SdSound.h
 * @brief Sound header file\n
 Sound 컴포넌트는 Sound Post Chip외에 Amp와 Speaker 모두를 포함. decoder와 speaker 사이에 있는 post-processing unit 기능을 하기도 하지만 특히 아날로그의 경우에는 MTS 검출과 관련하여 Decoder의 일부가 포함됨.
MSP Chip등 Sound와 관련 된 chip들에 대한 셋팅을 담당한다. 특정칩에서만 구현 가능한 부분은 SdMisc에서 구현 한다.
SdSound_Init, SdSound_Connect, SdSound_Get, SdSound_Set, SdSound_GetStatus,SdSound_MuteAmp, SdSound_MuteSpeaker, SdSound_PlayBeeper등 8개 함수로 구성되어 있다. 
AV Decoding에 대한 API는 SdAVDec에 구현되어 있다. Audio Format에 대한 Decorder의 선택은 SdAVDec_Set에서 Audio Foramt을 설정함으로 가능하다. 
API 호출순서는 SdAVDec_Set() -> SdAVDec_DecorderConnect() 이다. 
SDAL에서는 Sound Data가 어느 포트로 들어오느냐는 상관하지 않고, SDAL에서는 단지 Source가 무엇인지, Format이 무엇인지 확인해서 디코딩을 수행한다. 

Sound Component contains not only Sound Post Chip, but also Amp and Speaker.
The function includes post-processing between decoder and speaker, it also includes a part of decoder, MTS detector, for analog signal.
It takes the charge of setting the sound-related chips like MSP Chips etc.  Functions for specific chip are implemented in SdMisc.
It consists of 8 functions such as SdSound_Init, SdSound_Connect, SdSound_Get, SdSound_Set, SdSound_GetStatus, SdSound_MuteAmp, SdSound_MuteSpeaker, SdSound_PlayBeeper.
APIs for AV Decoding is implemented in SdAVDec.
Decoder Audio Format is specified in SdAVDec_Set().
The calling sequence is SdAVDec_Set() -> SdAVDec_DecorderConnect().
SDAL decodes Sound Data using the type of Source and its format. It does not use which port the data comes from.

 * @author S/W Platform Lab
 * @date 2006/07/11
 */

#ifndef	_SDSOUND_H_
#define	_SDSOUND_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#define		MAX_EQUALIZER			(20)
#define		MIN_EQUALIZER			(0)

#define 	MAX_VOLUME 				(100)
#define 	MIN_VOLUME 				(0) 	

#define 	MAX_BALANCE 			(20)
#define 	MIN_BALANCE 			(0) 	

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/
typedef enum
{
	SD_SOUND_STEP0, 
	SD_SOUND_STEP1, 
	SD_SOUND_STEP2,
	SD_SOUND_STEP3,
	SD_SOUND_STEP4,
}SdSound_Step_k;

typedef enum
{
	SD_MTS_MONO			= 0x0,
	SD_MTS_STEREO		= 0x1,
	SD_MTS_FOREIGN 		= 0x2,
	SD_MTS_SAP 			= 0x2,
	SD_MTS_DUAL			= 0x3,
	SD_MTS_STEREO_SAP	= 0x3,

	SD_MTS_NICAM_STEREO	= 0x11,
	SD_MTS_NICAM_MONO,
	SD_MTS_NICAM_DUAL1,
	SD_MTS_NICAM_DUAL2,
	SD_MTS_INVALID 		= SD_INVALID
} SdSound_MtsMode_k;

typedef enum
{
    SD_AUDIO_ATV_UNKNOWN = 0,
    SD_AUDIO_ATV_BTSC,            ///< TV-Sys: M/N, Audio Modulation: BTSC-Stereo + SAP
    SD_AUDIO_ATV_EIAJ,              ///< TV-Sys: M/N, Audio Modulation: FM-FM (EIAJ)
    SD_AUDIO_ATV_A2_KOREA,  ///< TV-Sys: M/N, Audio Modulation: FM-Stereo
    SD_AUDIO_ATV_BG_A2,         ///< TV-Sys: B/G, Audio Modulation: FM-A2
    SD_AUDIO_ATV_DK1_A2,        ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K1) 
    SD_AUDIO_ATV_DK2_A2,        ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K2) 
    SD_AUDIO_ATV_DK3_A2,        ///< TV-Sys: D/K, Audio Modulation: FM-A2 (D/K3) 
    SD_AUDIO_ATV_I,                    ///< TV-Sys: I,   Audio Modulation: FM-Mono/NICAM
    SD_AUDIO_ATV_BG,                ///< TV-Sys: B/G, Audio Modulation: FM-Mono/NICAM
    SD_AUDIO_ATV_DK,                ///< TV-Sys: D/K, Audio Modulation: FM-Mono/NICAM (D/K)
    SD_AUDIO_ATV_L,                    ///< TV-Sys: L,   Audio Modulation: AM-Mono/NICAM
    SD_AUDIO_ATV_NUM
}SdSound_SoundSys_k; 

typedef enum
{
	SD_TSXT_OFF		= 0,
	SD_TSXT_STEREO,
	SD_TSXT_3D_MONO,
	SD_TSXT_MAX,
	SD_TSXT_INVALID = SD_INVALID
} SdSound_TsxtMode_k;

typedef enum
{
	SD_AMP_GAIN_0,
	SD_AMP_GAIN_1,
	SD_AMP_GAIN_2,
	SD_AMP_GAIN_3,
	SD_AMP_GAIN_MAX,
	SD_AMP_GAIN_INVALID = SD_INVALID
} SdSound_AmpGainLevel_k;

typedef enum
{
	SD_MELODY_POWER_OFF,
	SD_MELODY_POWER_ON,
	SD_MELODY_POWER_OFF_NOGAIN,
	SD_MELODY_POWER_ON_NOGAIN,
	SD_MELODY_FAST_POWER_ON,
	SD_MELODY_FAST_POWER_ON_NOGAIN
} SdSound_MelodyMode_k;

typedef enum
{
	SD_SOUND_STATUS_ANALOGMTS	= 0x01,
	SD_SOUND_STATUS_GETAMPGAIN 	= 0x02,
	SD_SOUND_STATUS_PLAYAUI		= 0x04,
} SdSound_StatusMask_k;

typedef enum
{
	SD_SUBWOOFER_LOUD_CHANNEL	= 0x00,	///< loudspeaker channel unfiltered
	SD_SUBWOOFER_COMP_HIGHPASS	= 0x01,	///< a complementary high-pass is processed in the loudspeaker output channel
	SD_SUBWOOFER_MICRONAS_BASS	= 0x02,	///< Micronas BASS added onto main channel
	SD_SUBWOOFER_INVALID		= SD_INVALID
} SdSound_SubwooferFilterType_k;

typedef enum
{
	SD_DNSE_NONE,
	SD_DNSE_WIDE_STEREO,
	SD_DNSE_VIRTUAL_SPEAKER,
	SD_DNSE_VIRTUAL_HEADPHONE,
} SdSound_DNSeMode_k;

typedef enum
{
	SD_PLAY_AUI_INIT,
	SD_PLAY_AUI_ING,
	SD_PLAY_AUI_DONE,
} SdSound_PlayAUI_State_k;

typedef enum
{
	SD_MAIN_STEREO_SUB_STEREO,
	SD_MAIN_MIXED_SUB_STEREO,
	SD_MAIN_STEREO_SUB_MIXED,
	SD_MAIN_MIXED_SUB_MIXED,	
} SdSound_AdPath_k; ///< Sound 모듈 출력이 AD-Mixed와 Stereo의 두개 출력으로 나올 경우, Main/Sub Speaker의 경로를 설정함. (Main - Speaker and Aux,  Sub - headset)

typedef struct
{ 
	uint32_t eqBand100Hz;	///<100Hz loudness(dB)
	uint32_t eqBand300Hz;	///<300Hz loudness(dB)
	uint32_t eqBand1KHz;	///<1KHz loudness(dB)
	uint32_t eqBand3KHz;	///<3KHz loudness(dB)
	uint32_t eqBand10KHz;	///<10KHz loudness(dB)
} SdSound_EqLevel5_t;  ///<Band별 center frequency,  Band별 Frequency값은 기존에 VD사업부에서 정의한 SPEC 임. Center Frequence for each Band. The frequencies are specified in VD SPEC.

typedef struct
{ 
	uint32_t eqBand100Hz;	///<100Hz loudness(dB)
	uint32_t eqBand500Hz;	///<500Hz loudness(dB)
	uint32_t eqBand1KHz;	///<1KHz loudness(dB)
	uint32_t eqBand2KHz;	///<2KHz loudness(dB)
	uint32_t eqBand4KHz;	///<4KHz loudness(dB)
	uint32_t eqBand8KHz;	///<8KHz loudness(dB)
	uint32_t eqBand15KHz;	///<15KHz loudness(dB)
} SdSound_EqLevel7_t;  

typedef struct
{
	uint32_t volume;		///<Step100(0~100)으로 나누어 각각 Gain이 설정될 수 있도록 해야 함. Range of volume is Step100 (0~100)
	uint32_t cornerFreq;	///<50~200 Hz
	SdSound_SubwooferFilterType_k eFilterType;
} SdSound_WooferSet_t;

typedef struct
{
	uint32_t volume;  				///<Step100(0~100)으로 나누어 각각 Gain이 설정될 수 있도록 해야 함. Range of volume is Step100 (0~100)
	uint32_t balance;				///<Step20(0~20) : 20(left -127dB right 0dB), ... 15(...., right 0dB), ... 10(both 0dB), ... 5(left 0dB, ...), ...0(left 0dB, right -127dB)
									
	uint32_t adVolume;              ///< volume for Audio Description. same step as volume. (only for DVB)
	SdSound_AdPath_k eAdPath;		///< path for Audio Description. (only for DVB)
	
	uint32_t speakerDelay;			///<Speaker Sound delay (ms)
	uint32_t headsetDelay;			///<HeadSet Sound delay (ms)
	uint32_t auxAnalogDelay;		///<Auxout Analog Output(L/R) delay (ms)
	uint32_t auxSpdifDelay;			///<Auxout Spdif Output delay (ms)
	SdBool_t bAutoVolume;			///<True = ON
	SdBool_t bBBE;					///<Used when supported 	
	SdSound_DNSeMode_k eDNSeMode;
	SdSound_MtsMode_k eOutputAnalogMts; ///<선택된 값(Mono입력이 아닌 경우 입력 MTS에 따라서 선택 범위가 결정됨.)
	SdSound_TsxtMode_k eTsxtMode;
	SdBool_t bSupportEQ7; ///< True : used Eq7, False: used Eq5
	union{
	SdSound_EqLevel5_t sEqLevel5;	///<5Level EQ
	SdSound_EqLevel7_t sEqLevel7;	///<7Level EQ
	}uEQ;
	SdSound_AmpGainLevel_k eAmpGainLevel;
	SdSound_WooferSet_t sWooferSet;
	SdBool_t bExtSpdifIn;			///<External Sound Source flag : True - Spdif Sound Input, False - L/R Sound Input
} SdSound_Settings_t;

typedef struct
{
	SdSound_MtsMode_k eInputAnalogMts; ///<입력 Analog MTS 값.
	SdSound_AmpGainLevel_k eAmpGainLevel;
	SdSound_PlayAUI_State_k eAUIState; ///< Play AUI Status
} SdSound_Status_t;

typedef void (*SdSound_CallBackCapture)(uint8_t *pBuffer, uint32_t size, uint32_t param);

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief Sound 관련 디바이스 및 자원을 초기화 한다.
 It initialize the Sound-related device and resource.
 * @remarks 
 * @param step Sound 초기화 Step, 제품 사향마다 다를 수 있다. Initialization step for Sound Component depending on the sound chip vendor
 * @param ePowerState
 * @code
 	typedef enum
	{
		SD_SOUND_STEP0, 
		SD_SOUND_STEP1,
		SD_SOUND_STEP2,
		SD_SOUND_STEP3,
		SD_SOUND_STEP4,
	}SdSound_Step_k;

	typedef enum
	{
		SD_PWROFF_TO_NORMAL,
		SD_PWROFF_TO_STANDBY,
		SD_STANDBY_TO_NORMAL
	} SdPowerState_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
*/
SdResult SdSound_Init(SdSound_Step_k step, SdPowerState_k ePowerState);

/**
 * @brief Sound Component와 Sound Component 앞단과의 연결을 설정하는 함수.Sound 출력의  확장성을 고려하여 정의 함 
 The function to set the connection between Sound Component and the front end of Sound Component.
 * @remarks Sound 연결시 하나의 입력소스에서 여러개의 Sound를 동시에 연결할 수 있으므로 Sound 에서 입력 소스를 찾아 연결하는 방식으로 Connect를 수행한다. 
 Since it is available to connect several sounds at one input source simultaneously, connect it in the way of finding out the input source in the Sound.
 * 샘플조합은 아래와 같음. Sample combination
 * Digital : eSrc, eInput
 * Analog : eSrc, X(Don't Care이지만, 사용하는 ExtIn에 맞춰서 설정한다. Don't Care but one defined in ExtIn)
 * @param eSoundId 연결하고자 하는 Sound Component ID. SD_SOUND0,  SD_SOUND1 로 두개까지 정의 함.
 *    만약 Sound chip이 여러개 더 추가 된다면 SD_SOUNDn 이 정의 될것이며, 하나의 소스입력에 대해 Sound 출력을 동시에 여러곳(여러가지로 예: Speaker, Headphone 등)에서 할 수있음.
 Sound Component ID to be used. Two IDs are defined: SD_SOUND0, SD_SOUND1 and can be extended to SD_SOUNDn if more sound chips are connected.
 A single source sound can be connected to several output simultaneous (both Speaker and Headphone)
 * @param eSrc 연결하고자 하는 Sound Source, Sound 모듈에서 Sound의 소스를 판단하기 위해서 사용.아날로그의 경우에는 eSrc만으로도 사운드 소스를 판단할 수 있음 
 Sound Source to be connected. For Analog Signal, eSrc is sufficient to find  the input component.
 * @param eInputId Sound Component의 입력과 연결될 Component의 ID. Sound 모듈에서 Sound의 소스를 판단하기 위해서 사용되며, 
 *    디지털의 경우에는 eSrc만으로는 어느 Decoder를 통과하여 Sound가 나오는지 알 수 없기 때문에 사용 Decoder를 알려주는 인자
 Input Component to be connected to the Sound Component.
 For Digital signal, Sound Component does not know by eSrc which decoder the signal comes from.
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdSound_Disconnect
 */
SdResult SdSound_Connect(SdSoundID_k eSoundId, SdSource_k eSrc, SdBackEndInID_k eInputId);

/**
 * @brief Sound Component와 연결을 해제하는 함수 
 The function of releasing the connection of Sound Component
 * @remarks 
 * @param eSoundId 연결을 해제하고자 하는 Sound Component ID. Sound ID to be disconnected
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdSound_Connect
 */
SdResult SdSound_Disconnect(SdSoundID_k eSoundId);

/**
 * @brief Speaker에 어떤 Sound가 연결이 되는지 설정한다. 
  It sets which sound would be connected to Speaker. 
 * @remarks Speaker 연결시 하나의 Speaker에서 여러개의 Sound를 동시에 연결할 수 있으므로 Speaker 에서 연결 Sound를 찾아 연결하는 방식으로 Connect를 수행한다. 
 Since it is available to connect several sounds to one Speaker simultaneously, the connection will be made in the way of finding out the connected Sound in Speaker and connecting it.
 * @param eSpeakerId 연결 하고자 하는 Speaker ID. Speaker ID to be connected
 * @param eSoundId Speaker에 연결될 Sound ID. Sound ID connected to the speaker
  * @return SdResult SD_OK/SD_NOT_OK
 * @see SdSound_DisconnectSpeaker
 */
SdResult SdSound_SpeakerConnect(SdSpeakerID_k eSpeakerId, SdSoundID_k eSoundId);

/**
 * @brief Speaker와 Sound의 연결 설정을 해제한다.
 It releases the connection settings between Speaker and Sound.
 * @remarks 
 * @param eSpeakerId 연결을 해제하고자 하는 Speaker ID. Speaker ID to be disconnected
 * @return SdResult SD_OK/SD_NOT_OK
 * @see SdSound_ConnectSpeaker
 */
SdResult SdSound_SpeakerDisconnect(SdSpeakerID_k eSpeakerId);

/**
 * @brief Sound 관련 설정값을 읽어온다.
 It reads in the Sound-related setting values.
 * @remarks
 * @param eSoundId
 * @param pSettings 현재 setting 되어 있는 Sound 설정값. current Sound setting
 * @code
	typedef struct
	{
		uint32_t volume;
		uint32_t balance;
		uint32_t soundDelay;
		uint32_t auxDelay;
		SdBool_t bAutoVolume;
		SdBool_t bBBE;
		SdSound_DNSeMode_k eDNSeMode;
		SdSound_MtsMode_k eMtsMode;
		SdSound_TsxtMode_k eTsxtMode;
		SdSound_EqLevel_t sEqLevel;
		SdSound_AmpGainLevel_k eAmpGainLevel;
		SdSound_WooferSet_t sWooferSet;
	} SdSound_Settings_t; 

	typedef enum
	{
		SD_MTS_MONO			= 0,
		SD_MTS_STEREO		= 1,
		SD_MTS_FOREIGN 		= 2,
		SD_MTS_SAP 			= 2,
		SD_MTS_DUAL			= 3,
		SD_MTS_STEREO_SAP	= 3,

		SD_MTS_NICAM_STEREO	= 1,
		SD_MTS_NICAM_DUAL,
		SD_MTS_NICAM_DUAL1,
		SD_MTS_NICAM_DUAL2,
		SD_MTS_INVALID 		= SD_INVALID
	} SdSound_MtsMode_k;

	typedef enum
	{
		SD_TSXT_OFF		= 0,
		SD_TSXT_STEREO,
		SD_TSXT_3D_MONO,
		SD_TSXT_MAX,
		SD_TSXT_INVALID = SD_INVALID
	} SdSound_TsxtMode_k;	

	typedef struct
	{
		uint32_t eqBand100Hz;
		uint32_t eqBand300Hz;
		uint32_t eqBand1KHz;
		uint32_t eqBand3KHz;
		uint32_t eqBand10KHz;
	} SdSound_EqLevel_t; 


	typedef enum
	{
		SD_AMP_GAIN_0,
		SD_AMP_GAIN_1,
		SD_AMP_GAIN_2,
		SD_AMP_GAIN_3,
		SD_AMP_GAIN_MAX,
		SD_AMP_GAIN_INVALID = SD_INVALID
	} SdSound_AmpGainLevel_k;

	typedef struct
	{
		uint32_t volume;
		uint32_t cornerFreq;
		SdSound_SubwooferFilterType_k eFilterType;
	} SdSound_WooferSet_t;

	typedef enum
	{
		SD_SUBWOOFER_LOUD_CHANNEL	 = 0x00,  
		SD_SUBWOOFER_COMP_HIGHPASS  = 0x01,  
		SD_SUBWOOFER_MICRONAS_BASS  = 0x02,  
		SD_SUBWOOFER_INVALID		= SD_INVALID
	} SdSound_SubwooferFilterType_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @code
 		//////------SAMPLE CODE------//////
		dSound_Settings_t mSoundSettings;
		SdSound_Get(SD_SPEAKER_MAIN, &mSoundSettings);
		mSoundSettings.volume = bVolume;
		mSoundSettings.eAmpGainLevel = SD_AMP_GAIN_3
		......
		SdSound_Set(SD_SPEAKER_MAIN, &mSoundSettings);
 * @endcode
 * @see 
 */
SdResult SdSound_Get(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings);

/**
 * @brief Sound 관련 설정값을 설정한다.
 It sets the Sound-related setting values.
 * @remarks
 * @param eSoundId
 * @param pSettings setting할 Sound 설정값. new value of Sound setting
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_Set(SdSoundID_k eSoundId, SdSound_Settings_t *pSettings);

/**
 * @brief Sound 관련 Information을 가져온다.
 It takes the Sound-related Information.
 * @remarks 
 * @param eSoundId
  * @param mask
 * @code
	typedef enum
	{
		SD_SOUND_STATUS_ANALOGMTS	= 0x01,
		SD_SOUND_STATUS_GETAMPGAIN 	= 0x02,
	} SdSound_StatusMask_k;
 * @endcode
 * @param pState
 * @code
	typedef struct
	{
		SdSound_MtsMode_k eAnalogMtsMode;
		SdSound_AmpGainLevel_k eAmpGainLevel;
	} SdSound_Status_t;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @code
	SdSound_Status_t SoundState;
	SdSound_GetStatus(SD_SPEAKER_MAIN, SD_SOUND_STATUS_GETAMPGAIN, &SoundState);
	gain = (AMP_GAIN_LEVEL)SoundState.eAmpGainLevel;
 * @endcode
 * @param timeout
 * @see 
 */
SdResult SdSound_GetStatus(SdSoundID_k eSoundId, SdMask_t mask, SdSound_Status_t *pState, uint32_t timeout);

/**
 * @brief Amp를 Mute 시킨다.
 It makes the Amp muted.
 * @remarks AMP 는 아날로그 엠프로 디지털 신호를 아날로그로 변환해서 스피커에 보내기 전에 증폭을 해주는 모듈이다.
 AMP converts the digital sound signal to analog, amplifies it and feed to the Speaker.
 * @param eSpeakerId
 * @param bMuteOn TRUE : Mute On, FALSE : Mute Off  
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_AmpMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn);

/**
 * @brief 최종 출력단인 Speaker 출력을 Mute 시킨다.
 It makes the Speaker output muted.
 * @param eSpeakerId
 * @param bMuteOn TRUE : Mute On, FALSE : Mute Off   
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_SpeakerMute(SdSpeakerID_k eSpeakerId, SdBool_t bMuteOn);

/**
 * @brief Power On/Off시 Beeper의 Melody를 출력시킨다.
 It outputs the Melogy of Beeper upon turning Power On/Off.
  Beeper음은 SoundChip에서 Frequency를 제어해서 내는 소리로, mp3나 PCM 음원을 사용해서 Beeper음을 만드는 경우에는 Decoder의 연결이 필요하기 때문에 이 API와는 별개로 구현되어야 함. 
  Beeper is produced by controlling sequence of frequencies in SoundChip. mp3 or PCM play requires connection of a audio decoder and should be implemented in another method.
  feeder를 이용한 clip play는 별개의 함수로(SdSound_PlayAUI) 구현된다.
  SdSound_PlayAUI implements the clip play with feeder.
  
 Beeper is implemented by managing Sound Chip frequencies.
 Beeper can also be implemented using mp3 or PCM clip through sound decoder and it is beyond the scope of this function.
 * @remarks Power On시 Frequency 별 Delay : 512Hz 70ms, 656Hz 70ms , 768Hz 70ms , 656Hz 70ms , 768Hz 70ms ,1040Hz 180ms
 * Power Off시  Frequency 별 Delay : 1312Hz 70ms, 1040Hz 70ms, 768Hz 70ms , 656Hz 70ms, 512Hz 180ms 
 * @param eSoundId
 * @param eMelodyMode Power On/Off 상황에 따른 Melody Mode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_PlayBeeper(SdSoundID_k eSoundId, SdSound_MelodyMode_k eMelodyMode);

/**
 * @brief Audio Data를 플레이하는 UI로서 시작음 또는 종료음으로 플레이시킬 때 사용한다.
 It is Audio data playing UI and is used in start up and shut down melodies.
 * @param eSoundId
 * @param eFormat 
 * @param pBuf
 * @param size
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_PlayAUI(SdSoundID_k eSoundId, SdAudioFormat_k eFormat, uint8_t *pBuf, uint32_t size);

SdResult SdSound_SetSifType(SdSoundID_k eSoundId, SdSound_SoundSys_k  eSoundTyp);



/**
 * @brief Speaker로 출력할 PCM 데이터를 Capture하여 Callback으로 상위 모듈에 일정 sample 간격마다 올려준다.
 * @remarks
 * @param eSoundId
 * @param callback Callback function, uninstall하려면 callback을 NULL로 준다.
 * @param param Callback function 함수 호출 시 같이 전달될 정보. parameters for the callback function call
 * @param period PCM sample 수. period 만큼의 sample이 Capture 되었을 때마다 Callback function이 호출되게 된다.
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_InstallCallbackPCMCapture(SdSoundID_k eSoundId, SdSound_CallBackCapture callback, uint32_t param, uint32_t period);

/**
 * @brief SdSound API의 Manual Debug 함수
 Manual Debug function of SdSound API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdSound_Debug(void);

/**
 * @brief get ATV sound system
 * @remarks should call this function before playing ATV or scanning channel
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdSound_GetSoundSystem(SdSoundID_k eSoundId, SdSound_SoundSys_k eDefaultSys, SdSound_SoundSys_k *peSoundSys);

#endif

