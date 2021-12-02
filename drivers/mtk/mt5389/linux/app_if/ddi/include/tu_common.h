/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2008 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file tu_common.h
 *
 *  common functions.
 *
 *  @author		Goldman(jhgold@lge.com)
 *  @version	1.0
 *  @date		2008.07.03
 *  @note		Additional information.
 *  @see		reference (file/function/URL/etc)
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/
#ifndef _TU_COMMON_H_
#define	_TU_COMMON_H_


/******************************************************************************
	Control Constants
******************************************************************************/

/******************************************************************************
	File Inclusions
******************************************************************************/

#include "common.h"
#include "country.h"
#include "tu_ddi.h"
#include "i2c_ddi.h"

/******************************************************************************
 	Constant Definitions
******************************************************************************/

#define	TU_PRINT	FE_PRINT
//#define TU_PRINT(mode, format, args...) 				TU_OSA_PRINT (format, ##args)

#define	TU_INVAL_HANDLE			0x00

#define	TU_STEP_NUMERATOR		1000	/* kHz */

#define TU_SCALE_ARRAY_SIZE 	8
#define	TU_DEFAULT_AUD_SYS		AUDIO_SIF_SYSTEM_BG
#define	TU_DEFAULT_AUD_STD		AUDIO_SIF_BG_FM

#define TU_PLL_LOCK_WAIT_COUNT	10

#define	TU_ATV_AFT_LIMIT		1800
#define	TU_ATV_AFT_LIMIT_IRC	2600

/* XC5000_ATSC : tuner latch up 회피를 위한 임시 코드 대응 */
#define AVOID_LATCH_UP

#define RMM_GROUP_IS_CENTERFREQ_SEARCH		(RMM_GROUP_IS_KR || RMM_GROUP_IS_US || RMM_GROUP_IS_BR || RMM_GROUP_IS_TW||RMM_GROUP_IS_XA)

/**
 * Step Size.
 */
typedef	UINT32	TU_STEP_SIZE_HZ_U32;
#define	TU_STEP_031_250_HZ		 31250		/* 32 */
#define	TU_STEP_050_000_HZ		 50000		/* 20 */
#define	TU_STEP_062_500_HZ		 62500		/* 16 */
#define	TU_STEP_125_000_HZ		125000		/*  8 */
#define	TU_STEP_142_857_HZ		142857		/*  7 */
#define	TU_STEP_166_667_HZ		166667		/*  6 */
#define	TU_STEP_250_000_HZ		250000		/*  4 */
#define	TU_STEP_500_000_HZ		500000		/*  2 */

typedef	UINT32	TU_STEP_DENOMINATOR_U08;
#define	TU_STEP_DENOMINATOR_031_250		32		/*  31250 = 1,000,000 (Hz) / 32 */
#define	TU_STEP_DENOMINATOR_050_000		20		/*  50000 = 1,000,000 (Hz) / 20 */
#define	TU_STEP_DENOMINATOR_062_500		16		/*  62500 = 1,000,000 (Hz) / 16 */
#define	TU_STEP_DENOMINATOR_125_000		 8		/* 125000 = 1,000,000 (Hz) /  8 */
#define	TU_STEP_DENOMINATOR_142_857		 7		/* 142857 = 1,000,000 (Hz) /  7 */
#define	TU_STEP_DENOMINATOR_166_667		 6		/* 166667 = 1,000,000 (Hz) /  6 */
#define	TU_STEP_DENOMINATOR_250_000		 4		/* 250000 = 1,000,000 (Hz) /  4 */
#define	TU_STEP_DENOMINATOR_500_000		 2		/* 500000 = 1,000,000 (Hz) /  2 */

/**
 * IF frequency.
 */
typedef UINT32	TU_IF_FREQ_KHZ_U32;
#define	DEMOD_IF_4063_KHZ			4063
#define	DEMOD_IF_4560_KHZ			4560
#define	DEMOD_IF_5000_KHZ			5000
#define	DEMOD_IF_6000_KHZ			6000
#define TU_IF_FREQ_04_063_KHZ		 4063
#define TU_IF_FREQ_04_560_KHZ		 4560
#define TU_IF_FREQ_05_000_KHZ		 5000
#define TU_IF_FREQ_06_000_KHZ		 6000
#define	TU_IF_FREQ_33_100_KHZ		33100
#define	TU_IF_FREQ_33_900_KHZ		33900
#define	TU_IF_FREQ_36_000_KHZ		36000
#define	TU_IF_FREQ_36_125_KHZ		36125
#define	TU_IF_FREQ_36_143_KHZ		36143
#define	TU_IF_FREQ_36_150_KHZ		36150
#define	TU_IF_FREQ_36_167_KHZ		36167
#define	TU_IF_FREQ_36_500_KHZ		36500
#define	TU_IF_FREQ_37_000_KHZ		37000
#define	TU_IF_FREQ_37_500_KHZ		37500
#define	TU_IF_FREQ_37_600_KHZ		37600
#define	TU_IF_FREQ_37_700_KHZ		37700
#define	TU_IF_FREQ_38_000_KHZ		38000
#define	TU_IF_FREQ_38_250_KHZ		38250
#define	TU_IF_FREQ_38_400_KHZ		38400
#define	TU_IF_FREQ_38_900_KHZ		38900
#define	TU_IF_FREQ_44_000_KHZ		44000


/* ========== PRINT MASK ========== */
#define	PRN_CRI		0		// Critical
#define	PRN_IMP		1		// Important
#define	PRN_NOR		2		// Normal
#define	PRN_TST		3		// Test

#define	PRN_DDI_C	PRN_CRI
#define	PRN_DDI_I	PRN_IMP
#define	PRN_DDI_N	PRN_NOR
#define	PRN_DDI_T	PRN_TST

#define	PRN_TUN_C	PRN_CRI
#define	PRN_TUN_I	PRN_IMP
#define	PRN_TUN_N	PRN_NOR
#define	PRN_TUN_T	PRN_TST

#define	PRN_ETC_C	PRN_CRI
#define	PRN_ETC_I	PRN_IMP
#define	PRN_ETC_N	PRN_NOR
#define	PRN_ETC_T	PRN_TST


/******************************************************************************
	Macro Definitions
******************************************************************************/

#define	TU_ABS(_a)			(((SINT32)(_a) > 0  )  ? (_a) : (-(_a)))
#define	TU_ABS_DIF(_a, _b)	(((_a) > (_b)  )  ? ((_a) - (_b)) : ((_b) - (_a)))

#if (PLATFORM_TYPE == LG_PLATFORM)  //[L8] jeongpil.yun
#ifndef __F__
#define	__F__		__FUNCTION__
#endif
#define	__L__		__LINE__
#else
#define	__F__		__FUNCTION__
#define	__L__		__LINE__
#endif

#define	TU_DELAY_MS	OSA_SuspendTask

#define INPUT_HEX	OSA_DBG_GetHexInput
#define INPUT_DEC	OSA_DBG_GetDecimalInput
#define INPUT_INT	OSA_DBG_GetIntegerInput
#define INPUT_STR	OSA_ReadCmdString

#define DBG_PRINT	OSA_DBG_PrintMenu

#define	TU_SCALE_DOWN(number, factor)	((number + (factor >> 1)) / factor)
#define	TU_SCALE_UP(number, factor)		(number * factor)

#define	TU_FREQ_2_PLL(frequency, denominator)	((UINT32)(((denominator * frequency) + (TU_STEP_NUMERATOR >> 1)) / TU_STEP_NUMERATOR))
#define	TU_PLL_2_FREQ(PLL, denominator)			((UINT32)((PLL * TU_STEP_NUMERATOR) / denominator))

/******************************************************************************
	Type Definitions
******************************************************************************/


/**
 * TU_FLAG.
 */
typedef enum TU_FLAG
{
	TU_FLAG_ON			= 0x30,
	TU_FLAG_OFF,

	TU_FLAG_VALID		= 0x40,
	TU_FLAG_INVALID,

	TU_FLAG_SUCCESS 	= 0x50,
	TU_FLAG_FAIL,

	TU_FLAG_ENABLE		= 0x60,
	TU_FLAG_DISABLE,

	TU_FLAG_START		= 0x90,
	TU_FLAG_ING,
	TU_FLAG_STOP,
	TU_FLAG_FINISH,
	TU_FLAG_COMPLETE,

	TU_FLAG_UNKNOWN 	= 0xF0
} TU_FLAG_T;

#define	TU_RETURN_VALUE_T	DTV_STATUS_T
#define	TU_RET_OK			OK
#define	TU_RET_FAIL 		NOT_OK

#define TU_HANDLE 		UINT32
#define TU_I2C_HANDLE 	TU_HANDLE
#define TU_I2C_DEV_ID 	UINT16

#define	TU_TIME_TYPE	UINT64


/* ========== I2C Channel ========== */
/**
 * I2C info.
*/
typedef struct
{
	TU_I2C_CH_INDEX_T 	chIndex;

	TU_I2C_DEV_ID	deviceID;
	TU_I2C_HANDLE	handle;
} TU_I2C_INFO_T;


/**
 * Tuner NAME.
 */
typedef enum
{
	/* LGIT H-NIM */
	TU_TUNER_LGT10_H		= 0x00,
	TU_TUNER_TDVJ_H0,			/* NTSC */
	TU_TUNER_TDVJ_H1,			/* NTSC TUNER_Z_TDVJ_H0 Revision */
	TU_TUNER_TDTJ_S0,			/* PAL */
	TU_TUNER_TDTJ_S1,			/* PAL */	/* 2010.10.06 : Revision of S0 */
	TU_TUNER_TDSS_G1,			/* PAL(Si2176) */	
	TU_TUNER_TDTD_V0,			/* Channel Browser */
	TU_TUNER_TDTD_V1,			/* Revision Ver. of Channel Browser */
	TU_TUNER_TDTR_T0,			/* Brazil, Taiwan */	

	/* LGIT F-NIM */
	TU_TUNER_LGT10_F		= 0x20,
	TU_TUNER_TDFR_G0,			/* CXD2820 : DVB-T2 */
	TU_TUNER_TDFR_G1,			/* CXD1978 */
	TU_TUNER_TDFR_G2,			/* MSB1231 */
	TU_TUNER_TDFR_C0,			/* LGDT3911 */
	TU_TUNER_TDFR_C1,			/* LGS8G85 */
	TU_TUNER_TDFR_C2,			/* ATBM884x */
	TU_TUNER_TDTQ_G0,			/* SI2165 */
	TU_TUNER_TDFR_T0,			/* SI2165 + LGT10 [TW]*///TU_TUNER_TDFR_T0(SI2165 + LGT10) TW FullNIM 대응.
	TU_TUNER_TDSN_G2,			/* MSB1231 + Si2176*/

	/* FRESCO */
	TU_TUNER_UCA45XL		= 0x40,	/* SANYO */
	TU_TUNER_UCA45EL, 			/* SANYO DVB RF AGC Control Revison*/

	/* ETC */
	TU_TUNER_XC5000			= 0x60,	/* XCEIVE */
	TU_TUNER_M88TS2022,			/* LGIT (DVB-S) */
	TU_TUNER_TDSB_H0,			/* NTSC CHB(SI2173) */

	TU_TUNER_MASK			= 0xE0,

	TU_TUNER_UNKNOWN 		= 0xFF
} TU_TUNER_NAME_T;

#define	TU_IS_TUNER_LGT10_H(tunerName)	(TU_TUNER_LGT10_H 	== (tunerName & TU_TUNER_MASK))
#define	TU_IS_TUNER_LGT10_F(tunerName)	(TU_TUNER_LGT10_F 	== (tunerName & TU_TUNER_MASK))


/**
 * Demodulator IC for Analog and Digital.
 */
typedef enum
{
	/* ANALOG */
	TU_DEMOD_ANALOG 	= 0x00,
	TU_DEMOD_LGT10,
	TU_DEMOD_XC5000,
	TU_DEMOD_UCA45XL,
	TU_DEMOD_SI2173,
	TU_DEMOD_SI2176,

	/* MULTI */
	TU_DEMOD_DIGITAL 	= 0x80,
	TU_DEMOD_BCM35230,

	/* ATSC */
	TU_DEMOD_S7_ATSC,
	TU_DEMOD_S8_ATSC,
	TU_DEMOD_BCM3549,
	TU_DEMOD_LGDT3305,
	TU_DEMOD_L8_ATSC,   // [L8] jeongpil.yun

	/* DVB */
	TU_DEMOD_S7_DVB,
	TU_DEMOD_S8_DVB,
	TU_DEMOD_MSB1231,
	TU_DEMOD_BCM3556,
	TU_DEMOD_CXD1978,
	TU_DEMOD_CXD2820,
	TU_DEMOD_DRXK,
	TU_DEMOD_CE6355,
	TU_DEMOD_MT5133,
	TU_DEMOD_SI2165,
	TU_DEMOD_L8_DVB,  // [L8] jeongpil.yun

	TU_DEMOD_MTM88DS3002,
	TU_DEMOD_MTM88DS3103,

	/* DTMB */
	TU_DEMOD_LGDT3900,
	TU_DEMOD_LGDT3911,
	//TU_DEMOD_LGS8G75,
	TU_DEMOD_LGS8G85, // modified by How.C (chen_hao@lge.com)
	TU_DEMOD_ATBM884X,
	TU_DEMOD_LGDT3881,

	/* BRAZIL */
	TU_DEMOD_MN884433,
	TU_DEMOD_TC90517,

	/* NONE */
	TU_DEMOD_NONE,

	TU_DEMOD_TYPE_END,

	TU_DEMOD_UNKNOWN 	= 0xFF
} TU_DEMOD_TYPE_T;

/**
 * structure for strength and quality scaling.
*/
typedef struct		// cannot be changed in any other files
{
	UINT8	index;
	UINT8	activeWidth;
	UINT8	oldestValue;
	UINT16	sum;

	UINT8	array[TU_SCALE_ARRAY_SIZE];
} TU_SCALE_INFO_T;

/**
 * GPIO control data format.
*/
typedef struct
{
	BOOLEAN bValid[TU_CONTROL_END];
	BOOLEAN bState[TU_CONTROL_END];
} TU_CONTROL_STATE_T;

/**
 * TUNER info.
*/
typedef struct
{
	/* HANDLE (I2C or CHIP) */
	TU_HANDLE	handleDTV;
	TU_HANDLE	handleATV;
	TU_HANDLE	handlePLL;

	TU_HANDLE	handleDTV_S;
	TU_HANDLE	handlePLL_S;
	TU_HANDLE	handleLNB_S;

	TU_TUNER_NAME_T		tunerName;

	TU_DEMOD_TYPE_T		demodATV;
	TU_DEMOD_TYPE_T		demodDTV;
	TU_DEMOD_TYPE_T		demodDTV_S;

	BOOLEAN				bInitDTV;
	BOOLEAN				bInitATV;

	TRANS_MEDIA_T 		transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_TUNE_MODE_T	 	tuneMode;

	UINT8				unPLP;
	//UINT8				unGroupID;
	UINT32				freqTuned;
	UINT32				freqCenter;

	UINT32				analogIF;
	UINT32				digitalIF;

	TU_IF_FREQ_KHZ_U32		unIFFreq;
	TU_STEP_SIZE_HZ_U32 	stepSize;			/* Don't use. Use stepDenominator */
	TU_STEP_DENOMINATOR_U08 stepDenominator;
	TU_CH_BW_KHZ_U16 		channelBW;

	TU_SPECDATA_T		specData;

	BOOLEAN 			bIsSerial;
	BOOLEAN 			bWithCAM;

	BOOLEAN 			bLockStable;
	TU_LOCK_STATE_T		lockState;

	BOOLEAN 			bSignalStable;
	TU_SIGNAL_STATE_T	signalState;

	TU_CONTROL_STATE_T	controlState;

	TU_SCALE_INFO_T		scaleInfoStrength;
	TU_SCALE_INFO_T		scaleInfoQuality;

	TU_SUMD_DATA_T		sumdData;
	TU_HW_CONFIG_T		hardwareConfig;
} TU_TUNER_INFO_T;


/******************************************************************************
	Function Declaration
******************************************************************************/

extern	char* 	TU_CMN_WhatTunerName(TU_TUNER_NAME_T tunerName);
extern	char* 	TU_CMN_WhatDemodType(TU_DEMOD_TYPE_T demodType);
extern	char*	TU_CMN_WhatTransMedia(TRANS_MEDIA_T transMedia);
extern	char*	TU_CMN_WhatTransSystem(TU_TRANS_SYSTEM_T transSystem);
extern	char*	TU_CMN_WhatAudioSystem(AUDIO_SIF_SOUNDSYSTEM_T audioSystem);

//extern	TU_RETURN_VALUE_T 	TU_CMN_GetCurrentTime(TU_TIME_TYPE *pCurrentTime);
//extern	TU_RETURN_VALUE_T 	TU_CMN_GetElapsedTime(TU_TIME_TYPE startTime, TU_TIME_TYPE *pElapsedTime);

extern	TU_SPECDATA_T 		TU_CMN_ClearSpecialData(void);
extern	TU_CH_BW_KHZ_U16 	TU_CMN_SymbolRate2Bandwidth(UINT32 symbolrate);

extern	TU_RETURN_VALUE_T 	TU_CMN_SetPLL(TUNER_NUMBER_T tunerNo, UINT32 frequency);

extern	UINT32 	TU_CMN_FrequencyRoundUP(TUNER_NUMBER_T tunerNo, UINT32 frequency);

extern	UINT8	TU_CMN_CalculateSSI(TUNER_NUMBER_T tunerNo, UINT8* pPowerLevel);
extern	UINT8 	TU_CMN_CalculateSQI(UINT32 packetError, UINT32 unBER);
extern	void 	TU_CMN_ScaleReset(TU_SCALE_INFO_T *pScaleInfo);
extern	UINT8 	TU_CMN_ScaleSave(TU_SCALE_INFO_T *pScaleInfo, UINT8 value);

extern	BOOLEAN	TU_CMN_AutoBooster(TUNER_NUMBER_T tunerNo, SINT8 referencLevel);

extern	void 	TU_DGB_Main (TUNER_NUMBER_T tunerNo);

extern DTV_STATUS_T 	TU_DDI_CheckPowerLevel(TUNER_NUMBER_T tunerNo, SINT32 *pPowerLevel);
extern	DTV_STATUS_T	TU_DDI_SetPLLPostJob(TUNER_NUMBER_T tunerNo);

#ifndef MTK_UNIT_TEST
extern TU_TUNER_INFO_T gTunerInfo[TUNER_2];
#endif

#endif /* End of _TU_COMMON_H_ */

