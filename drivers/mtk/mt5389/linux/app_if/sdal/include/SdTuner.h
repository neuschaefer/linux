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
 * @file SdTuner.h
 * @brief Tuner header file\n
SdTuner는 Tuner및 VSB decoder를 초기화 하여 사용 가능한 상태로 만든다.\n
초기화 이후 Tuning 수행등의 기능을 한다. \n
Init, Tune, DecodeChannel, Set, Get, SpliterSet, SpliterGet, GetStatus와 \n
POD관련 함수들로 이루어져 있다. 
SdTuner initializes the Tuner and VSB decoder and creates the usable state. 
It performs the features like Tuning after initialization. 
It consists of Init, Tune, DecodeChannel, Set, Get, SpliterSet, SpliterGet, GetStatus and POD-related functions. 
 * @author S/W Platform Lab
 * @date 2006/07/11
 */

#ifndef	_SDTUNER_H_
#define	_SDTUNER_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdUtil.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

typedef enum
{
	SD_MOD_UNKNOWN,
	SD_MOD_NTSC,	
	SD_MOD_64QAM,
	SD_MOD_256QAM,
	SD_MOD_QAM, ///< Auto QAM
	SD_MOD_8VSB,
	SD_MOD_16VSB,
	SD_MOD_PAL,
	SD_MOD_DMBT,
	SD_MOD_OFDM,
	SD_MOD_ISDBT,
} SdTuner_ModFormat_k;

typedef enum
{
	SD_RF_AIR,
	SD_RF_CABLE
} SdTuner_RfMode_k;

typedef enum
{
	SD_CABLE_STD,
	SD_CABLE_HRC,
	SD_CABLE_IRC,
	SD_CABLE_AFN
} SdTuner_CableMode_k;

typedef enum
{
	SD_TUNE_CHANNEL,
	SD_TUNE_FREQUENCY
} SdTuner_TuneMode_k;

typedef enum
{
	SD_POL_RIGHT,
	SD_POL_LEFT
} SdTuner_AftPosition_k;

typedef enum
{	
	SD_RF_SW_ANT_AB_DIRECT,
	SD_RF_SW_ANT_AB_CROSS,
	SD_RF_SW_ANT_A_SPLIT, 
	SD_RF_SW_ANT_B_SPLIT
} SdTuner_RfSwitchType_k; ///< Antena A - Air, Antena B - Cable

typedef enum
{
	SD_LNA_NOT_CHECK,
	SD_LNA_WEAK,
	SD_LNA_STRONG
} SdTuner_LnaSensitive_k;

typedef enum
{
	SD_OOB_NOT_TUNED		 		= 0, ///< not tuned
	SD_OOB_TUNED_AND_LOCKED			= 1, ///< tuned and locked
	SD_OOB_UNLOCKED_BEING_LOCKED	= 2  ///< unlocked after being locked
} SdTuner_OobStatus_k;

typedef enum
{
	SD_H_PRIORITY_HIGH,
	SD_H_PRIORITY_LOW,
} SdTuner_HierarchyPriority_k;

typedef enum
{
	SD_H_NATIVE_ALPHA_NONE,
	SD_H_NATIVE_ALPHA_1,
	SD_H_NATIVE_ALPHA_2,
	SD_H_NATIVE_ALPHA_4,
	SD_H_IN_DEPTH_ALPHA_NONE,
	SD_H_IN_DEPTH_ALPHA_1,
	SD_H_IN_DEPTH_ALPHA_2,
	SD_H_IN_DEPTH_ALPHA_4
} SdTuner_HierarchyInfo_k;

typedef enum
{ 
    SD_TUNER_SOUND_SYS_BG,
    SD_TUNER_SOUND_SYS_DK,
    SD_TUNER_SOUND_SYS_I,
    SD_TUNER_SOUND_SYS_L,
    SD_TUNER_SOUND_SYS_M,
    SD_TUNER_SOUND_SYS_MAX
}SdTuner_SoundSystem_k;

typedef enum
{
	SD_TUNER_STATUS_LNA_SENSITIVE	= 0x0001, ///< LNA(Low Noise Amplifier) sensitive 
	SD_TUNER_STATUS_OOB				= 0x0002, ///< OOB(Out Of Band) status
	SD_TUNER_STATUS_LOCK			= 0x0004, ///< Digital channel lock status
	SD_TUNER_STATUS_SIGNAL_STRENGTH	= 0x0008, ///< Signal strength
	SD_TUNER_STATUS_AFC				= 0x0010, ///< AFC data
	SD_TUNER_STATUS_AGC				= 0x0020, ///< AGC data
	SD_TUNER_STATUS_SNR				= 0x0040, ///< SNR(Signal Noise Ratio) status
	SD_TUNER_STATUS_SIGNAL_LEVEL	= 0x0080, ///< Signal level
	SD_TUNER_STATUS_AUTO_QAM		= 0x0100, ///< Detected QAM Mode. When eModFormat is SD_MOD_QAM, it detects QAM mode automatically.
	SD_TUNER_STATUS_H_INFO	 		= 0x0400, ///< Hierarchy Information
} SdTuner_StatusMask_k;

typedef struct
{
	SdTuner_ModFormat_k eModFormat; ///< Modulation format : NTSC, VSB, QAM etc
	SdTuner_RfMode_k	eRfMode; ///< RF mode : air, cable
	SdTuner_CableMode_k	eCableMode; ///< Cable mode : STD, HRC, IRC, AFN 
	SdTuner_TuneMode_k	eTuneMode; ///< Tune mode : channel number, frequency 
	uint32_t channel; ///< Channel number
	SdFloat32_t frequency; ///< frequency value(MHz)
	int32_t iOffset; ///< frequency offset. It can be negative or positive value
	SdBool_t bAftOn; ///< AFT(Auto Fine Tune) On: TRUE, FALSE
	
	SdTuner_HierarchyPriority_k eHP; ///< Stream's Hierarchical Priority : SD_H_PRIORITY_HIGH, SD_H_PRIORITY_LOW (for DVB)	
	SdBool_t bTripleOffsetOn; ///< Triple offset tune On : TRUE, FALSE (for DVB)
	uint32_t bandwidth; ///< Bandwidth(Mhz), Range : 5 ~ 8 (for DVB)
	SdTuner_SoundSystem_k eSoundSys; ///< Sound System (for DVB)
} SdTuner_Parameter_t;

typedef struct
{
	SdBool_t bLnaOn; ///< LNA control : off, auto
} SdTuner_Settings_t;

typedef struct
{
	SdTuner_RfSwitchType_k eSwType; ///< RF spliter switch
} SdTuner_SplitterSettings_t;

typedef struct
{
	SdTuner_LnaSensitive_k eLnaSensitive; ///< LNA sensitive status : not check, weak, strong etc
	SdTuner_OobStatus_k eOobStatus; ///< OOB status
	SdTuner_ModFormat_k eAutoQAM; ///< Auto Detected QAM Mode. Valid Enum : SD_MOD_64QAM, SD_MOD_256QAM, SD_MOD_UNKNOWN	
	SdBool_t bLock; ///< Digital channel lock status
	uint8_t signalStrength;	///< Signal strength. value : 0 ~ 10
	uint8_t afcData; ///< For AFT, read AFT voltage value from tuner
	uint8_t agcData; ///< For LNA, read AGC voltage value from tuner
	int32_t snr; ///< SNR status (dB * 10)
	int32_t signalLevel; ///< Signal level	
	SdTuner_HierarchyInfo_k eHI; ///< Hierarchy Information (for DVB)
} SdTuner_Status_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

/**
 * @brief Tuner를 Reset하고 Decoder Chip을 초기화 한다.
 It resets the Tuner and initializes the Decoder Chip.
 * @remarks 
 * @param eTuner
 * @code
	typedef enum
	{
		SD_TUNER0,
		SD_TUNER1,
		SD_TUNER_MAX
	} SdTunerID_k; 
 * @endcode
 * @param ePowerState PowerState. Default is SD_PWROFF_TO_NORMAL. 
 * @code
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
SdResult SdTuner_Init(SdTunerID_k eTuner, SdPowerState_k ePowerState);

/**
 * @brief Analog/Digital Channel을 Tuning한다.
 It tunes the Analog/Digital Channel.
 * @remarks
 * @param eTuner
 * @param pTunerParam Tune를 위한 정보들을 모아서 보낸다. Information for the tuner.
 * @code
	typedef struct
	{
		SdTuner_ModFormat_k eModFormat;
		SdTuner_RfMode_k	eRfMode;
		SdTuner_CableMode_k	eCableMode;
		SdTuner_TuneMode_k	eTuneMode;
		uint32_t channel;
		SdFloat32_t frequency;
		int32_t iOffset;
		SdBool_t bAftOn;
		uint32_t reserved[5];
	} SdTuner_Parameter_t; 
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_Tune(SdTunerID_k eTuner, SdTuner_Parameter_t *pTunerParam);

/**
 * @brief Mode Format에 따른 채널 Decoding을 한다. 
 It decodes the channel depending on Mode Format. 
 * @remarks
 * @param eTuner
 * @param eMode 
 * @code
	typedef enum
	{
		SD_MOD_UNKNOWN,
		SD_MOD_NTSC,
		SD_MOD_64QAM,
		SD_MOD_256QAM,
		SD_MOD_8VSB,
		SD_MOD_16VSB,
		SD_MOD_PAL,
		SD_MOD_DMBT,
		SD_MOD_OFDM,
	} SdTuner_ModFormat_k; 
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_StartChannel(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode);

/**
 * @brief Tuner관련 설정값을 설정한다. 
 It sets the Tuner-related setting values. 
 * @remarks
 * @param eTuner
 * @param pSettings
 * @code
	typedef struct
	{
		SdBool_t bLnaOn;
	}SdTuner_Settings_t; 
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_Set(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings);

/**
 * @brief Tuner관련 설정값을 읽어온다. 
 It reads in the Tuner-related setting values.
 * @remarks
 * @param eTuner
 * @param pSettings
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_Get(SdTunerID_k eTuner, SdTuner_Settings_t *pSettings);

/**
 * @brief Spliter관련 설정값을 설정한다.
 It sets the Spliter-related setting values.
 * @remarks
 * @param pSettings
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_SplitterSet(SdTuner_SplitterSettings_t *pSettings);

/**
 * @brief Spliter관련 설정값을 읽어온다.
 It reads in the Spliter-related setting values. 
 * @remarks
 * @param pSettings
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_SplitterGet(SdTuner_SplitterSettings_t *pSettings);

/**
 * @brief Tuner 관련 Status를 읽어온다.
 It reads in the Tuner-related Satus.
 * @remarks 
 * @param eTuner
 * @param eMode
 * @param mask
 * @code
	typedef enum
	{
		SD_TUNER_STATUS_LNA_SENSITIVE		= 0x0001,
		SD_TUNER_STATUS_OOB					= 0x0002,
		SD_TUNER_STATUS_LOCK				= 0x0004,
		SD_TUNER_STATUS_SIGNAL_STRENGTH		= 0x0008,		
		SD_TUNER_STATUS_AFC					= 0x0010,
		SD_TUNER_STATUS_AGC					= 0x0020,
		SD_TUNER_STATUS_SNR					= 0x0040,
		SD_TUNER_STATUS_SIGNAL_LEVEL		= 0x0080,		
	} SdTuner_StatusMask_k; 
 * @endcode
 * @param pState
 * @code
	typedef struct
	{
		SdTuner_LnaSensitive_k eLnaSensitive;
		SdTuner_OobStatus_k eOobStatus;
		SdBool_t bLock;
		uint8_t signalStrength;
		uint8_t afcData;
		uint8_t agcData;
		int32_t snr;
		int32_t signalLevel;
	} SdTuner_Status_t;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @code
	SdTuner_Status_t TunerStatus;
	SdMask_t mask = SD_TUNER_STATUS_LOCK|SD_TUNER_STATUS_SNR|SD_TUNER_STATUS_SIGNAL_LEVEL;
	SdTuner_GetStatus(TUNER_1, fat_mode, mask, &TunerStatus); 
 * @endcode
 * @param timeout
 * @see 
 */
SdResult SdTuner_GetStatus(SdTunerID_k eTuner, SdTuner_ModFormat_k eMode, SdMask_t mask, SdTuner_Status_t *pState, uint32_t timeout);

/**
 * @brief OOB Tx Channel을 Tuning한다.
 It tunes the OOB Tx Channel.
 * @remarks 
 * @param fFreq
 * @param powerlevel
 * @param bitrate
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_OobTxTune(SdFloat32_t fFreq, uint32_t powerlevel, uint32_t bitrate);

/**
 * @brief OOB Rx Channel을 Tuning한다.
 It tunes the OOB Rx Channel.
 * @remarks 
 * @param fFreq
 * @param bitrate
 * @param spectrum
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_OobRxTune(SdFloat32_t fFreq, uint32_t bitrate, uint8_t spectrum);

/**
 * @brief OOB Rx Channel을 ReTuning한다.
 It Retunes the OOB Rx Channel.
 * @remarks 
 * @see 
 */
SdResult SdTuner_OobRxRetune(void);

/**
 * @brief OOB Lock Status(Carrier Lock, PCR Lock)를 가져온다.
 It brings the OOB Lock Status(Carrier Lock, PCR Lock).
 * @remarks 
 * @param eMode
 * @param pCarrierlock
 * @param pPCRlock
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdTuner_OobGetLockStatus(SdTuner_ModFormat_k eMode, SdBool_t * pCarrierlock, SdBool_t * pPCRlock);

/**
 * @brief SdTuner API의 Manual Debug 함수
 Manual Debug function of SdTuner API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdTuner_Debug(void);

#endif

