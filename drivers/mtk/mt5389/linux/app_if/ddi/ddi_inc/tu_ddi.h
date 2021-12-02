/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2008 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file tu_ddi.h
 *
 *  TU Module DDI header file.
 *
 *  @author		Goldman(jhgold@lge.com)
 *  @version	0.9
 *  @date		2008.04.14
 *  @note
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _TU_DDI_H_
#define	_TU_DDI_H_


/******************************************************************************
	Control Constants
******************************************************************************/

//#define	USE_BIT_OPERATION


/******************************************************************************
	File Inclusions
******************************************************************************/

#include "branches.h"
#include "common.h"
#include "global_configurations.h"
#include "audio_ddi.h"


/******************************************************************************
 	Constant Definitions
******************************************************************************/

#define	TU_NO_OF_TUNERS			1

#define	TU_NO_OF_DTV_TUNERS		1
#define	TU_NO_OF_ATV_TUNERS		1

#define	MAX_NO_OF_VHF			8

#define	TU_TPS_INVALID_CELL_ID	0

/**
 * Step Size.
 */
typedef	UINT32	TU_STEP_SIZE_HZ_U32;
#define	TU_STEP_050_000_HZ	 50000
#define	TU_STEP_062_500_HZ	 62500
#define	TU_STEP_125_000_HZ	125000
#define	TU_STEP_166_667_HZ	166667
#define	TU_STEP_250_000_HZ	250000
#define	TU_STEP_500_000_HZ	500000

#define	TU_STEP_SIZE_ATV	(TU_STEP_050_000_HZ / 1000)	/* kHz */

/**
 * channel bandwidth.
 */
typedef UINT16	TU_CH_BW_KHZ_U16;
#define	TU_CH_BW_ALL   		0
#define	TU_CH_BW_6000KHZ	6000
#define	TU_CH_BW_7000KHZ	7000
#define	TU_CH_BW_8000KHZ	8000
#define	TU_CH_BW_UNKNOWN	0xffff

/**
 * IF frequency.
 */
typedef UINT32	TU_IF_FREQ_KHZ_U32;
#define	TU_IF_FREQ_33_900_KHZ	33900
#define	TU_IF_FREQ_38_900_KHZ	38900
#define	TU_IF_FREQ_36_125_KHZ	36125
#define	TU_IF_FREQ_44_000_KHZ	44000
#define	TU_IF_FREQ_36_167_KHZ	36167
#define	TU_IF_FREQ_36_000_KHZ	36000	/* SI2165 */
#define TU_IF_FREQ_04_560_KHZ	4560	/* XC5000 - MICRONAS DRXK */

/**
 * TPS : Symbol rate.
 */
typedef UINT32	TU_SYMBOL_RATE_KHZ_U32;
#define	TU_SYMBOL_RATE_5056_KHZ	5056
#define	TU_SYMBOL_RATE_5309_KHZ	5309
#define	TU_SYMBOL_RATE_5360_KHZ	5360
#define	TU_SYMBOL_RATE_6111_KHZ	6111
#define	TU_SYMBOL_RATE_6875_KHZ	6875
#define	TU_SYMBOL_RATE_6900_KHZ	6900
#define	TU_SYMBOL_RATE_6950_KHZ	6950
#define	TU_SYMBOL_RATE_6957_KHZ	6957 /* = 8/115 * 100, DVB-C Annec A Max Symbol Rate */
#define	TU_SYMBOL_RATE_UNKNOWN	0xFFFFFFFF


/******************************************************************************
	Macro Definitions
******************************************************************************/

#define	TU_MASK_TRANS_IS_DIGITAL(transMode)		\
							((TU_TRANS_DIGITAL_MODE == (transMode & TU_TRANS_MODE_MASK_DTV_ATV)) ? TRUE : FALSE)
#define	TU_MASK_TRANS_IS_ANALOG(transMode)		\
							((TU_TRANS_ANALOG_MODE  == (transMode & TU_TRANS_MODE_MASK_DTV_ATV)) ? TRUE : FALSE)

#define	TU_MASK_TRANS_MEDIA(transMode)		(transMode & TU_TRANS_MODE_MASK_MEDIA)
#define	TU_MASK_TRANS_MEDIA_AREA(transMode)	(transMode & TU_TRANS_MODE_MASK_MEDIA_AREA)

#define	IS_TRANS_MEDIA_DIGITAL(transMedia)	(FE_TRANS_MEDIA_DIGITAL_MODE == (transMedia & FE_TRANS_MEDIA_MASK))
#define	IS_TRANS_MEDIA_ANALOG(transMedia)	(FE_TRANS_MEDIA_ANALOG_MODE  == (transMedia & FE_TRANS_MEDIA_MASK))

#define	TU_OSA_PRINT	OSA_PRINT
#if 0
#define	TU_DBG_PRINT	OSA_PRINT
#else
#define TU_DBG_PRINT(format, args...) 		TU_PRINT(0, format, ##args)
#endif

/******************************************************************************
	Type Definitions
******************************************************************************/


#define	TU_AUDIO_SYSTEM_T	AUDIO_SIF_SOUNDSYSTEM_T
#define	TU_AUD_SYS_BG		AUDIO_SIF_SYSTEM_BG
#define	TU_AUD_SYS_I		AUDIO_SIF_SYSTEM_I
#define	TU_AUD_SYS_DK		AUDIO_SIF_SYSTEM_DK
#define	TU_AUD_SYS_L		AUDIO_SIF_SYSTEM_L
#define	TU_AUD_SYS_M		AUDIO_SIF_SYSTEM_MN
#define	TU_AUD_SYS_LP		AUDIO_SIF_SYSTEM_LP
#define	TU_AUD_SYS_END		AUDIO_SIF_SYSTEM_END
#define	TU_AUD_SYS_UNKNOWN	AUDIO_SIF_SYSTEM_UNKNOWN


#define	TU_NUMBER_T			MRE_TUNER_NUMBER_T
#define	TU_NUMBER_0			MRE_TUNER_0
#define	TU_NUMBER_1			MRE_TUNER_1
#define	TU_NUMBER_2			MRE_TUNER_2
#define	TU_NUMBER_UNKNOWN	MRE_TUNER_UNKNOWN


/**
 * TU_FLAG.
 */
typedef enum TU_FLAG
{
	TU_FLAG_ON 			= 0x30,
	TU_FLAG_OFF,

	TU_FLAG_VALID		= 0x40,
	TU_FLAG_INVALID,

	TU_FLAG_SUCCESS		= 0x50,
	TU_FLAG_FAIL,

	TU_FLAG_ENABLE 		= 0x60,
	TU_FLAG_DISABLE,

	TU_FLAG_START		= 0x90,
	TU_FLAG_ING,
	TU_FLAG_STOP,
	TU_FLAG_FINISH,
	TU_FLAG_COMPLETE,

	TU_FLAG_UNKNOWN		= 0xF0
} TU_FLAG_T;


/**
 * Tuner Port.
 */
typedef enum TU_PORT
{
	TU_PORT_0		= 0,
	TU_PORT_1,

	TU_PORT_UNKNOWN	= 0x80
} TU_PORT_T;


/**
 * Lock state.
 */
typedef enum TU_LOCK_STATE
{
	TU_LOCK_OK			= 0x00,
	TU_LOCK_FAIL,
	TU_LOCK_UNSTABLE,

	TU_LOCK_WEAK		= 0x10,
	TU_LOCK_POOR,
	TU_LOCK_ATV_PROGRESS,		/* used in auto search */

	TU_LOCK_UNKNOWN 	= 0x80
} TU_LOCK_STATE_T;


/**
 * transmission mode.
 */
typedef enum TU_TRANSMISSION_MODE
{
	/*	---------- DIGITAL ---------------------	*/
	TU_TRANS_DIGITAL_MODE				= 0x1000,	/* 1 X X X */

	TU_TRANS_DTV_TERRESTRIAL			= 0x1100,	/* 1 1 X X */

		TU_TRANS_DTV_TER_ATSC			= 0x1110,	/* 1 1 1 X */

		TU_TRANS_DTV_TER_DVB			= 0x1120,	/* 1 1 2 X */

		TU_TRANS_DTV_TER_CHINA 			= 0x1130,	/* 1 1 3 X */
		TU_TRANS_DTV_TER_CHINA_MC,
		TU_TRANS_DTV_TER_CHINA_SC,
		TU_TRANS_DTV_TER_DTMB,

		TU_TRANS_DTV_TER_BRAZIL 		= 0x1140,	/* 1 1 4 X */

	TU_TRANS_DTV_CABLE 					= 0x1200,	/* 1 2 X X */

		TU_TRANS_DTV_CAB_DVB			= 0x1220,	/* 1 2 2 X */

		TU_TRANS_DTV_CAB_KOR			= 0x1240,	/* 1 2 4 X */
		TU_TRANS_DTV_CAB_KOR_ATSC,
		TU_TRANS_DTV_CAB_KOR_QAM,

		TU_TRANS_DTV_CAB_USA			= 0x1250,	/* 1 2 5 X */

		TU_TRANS_DTV_CAB_CHINA			= 0x1260,	/* 1 2 6 X */

	TU_TRANS_DTV_SATELLITE				= 0x1300,	/* 1 3 X X */

	/*	---------- ANALOG 	---------------------	*/
	TU_TRANS_ANALOG_MODE					= 0x2000,	/* 2 X X X */

	TU_TRANS_ATV_TERRESTRIAL			= 0x2100,	/* 2 1 X X */

		TU_TRANS_ATV_TER_NTSC			= 0x2110,	/* 2 1 1 X */
		TU_TRANS_ATV_TER_PAL			= 0x2120,	/* 2 1 2 X */
		TU_TRANS_ATV_TER_CHINA			= 0x2130,	/* 2 1 3 X */

	TU_TRANS_ATV_CABLE 					= 0x2200,	/* 2 2 X X */
		TU_TRANS_ATV_CAB_NTSC			= 0x2210,	/* 2 2 1 X */
		TU_TRANS_ATV_CAB_PAL			= 0x2220,	/* 2 2 2 X */
		TU_TRANS_ATV_CAB_CHINA			= 0x2230,	/* 2 2 3 X */

	TU_TRANS_ATV_SATELLITE				= 0x2300,	/* 2 3 X X */

	/*	---------- SLEEP 	---------------------	*/
	TU_TRANS_MODE_SLEEP					= 0x4000,	/* 4 X X X */

	/*	---------- UNKNOWN 	---------------------	*/
	TU_TRANS_MODE_UNKNOWN				= 0x8000,	/* 8 X X X */

	/*	---------- MASK 	---------------------	*/
	TU_TRANS_MODE_MASK_DTV_ATV			= 0xF000,	/* F X X X */
	TU_TRANS_MODE_MASK_MEDIA			= 0xFF00,	/* F F X X */
	TU_TRANS_MODE_MASK_MEDIA_AREA		= 0xFFF0 	/* F F F X */
} TU_TRANS_MODE_T;


/**
 * transmission system.
 */
typedef enum TU_TRANSMISSION_SYSTEM
{
	TU_TRANS_SYSTEM_DIGITAL	= 0x10,
	TU_TRANS_SYS_VSB,
	TU_TRANS_SYS_OFDM,
	TU_TRANS_SYS_DTMB,
	TU_TRANS_SYS_ISDBT,		/* Japan */
	TU_TRANS_SYS_SBTVD,		/* Brazil */

	TU_TRANS_SYS_DVBC,
	TU_TRANS_SYS_QAM,
	TU_TRANS_SYS_VSBC,		/* Korea Calbe only */

	TU_TRANS_SYSTEM_ANALOG	= 0x20,
	TU_TRANS_SYS_NTSC,
	TU_TRANS_SYS_PAL,
	TU_TRANS_SYS_MULTI,

	TU_TRANS_SYS_UNKNOWN	= 0x80,

	TU_TRANS_SYSTEM_MASK	= 0xF0
} TU_TRANS_SYSTEM_T;


/**
 * tune mode.
 */
#define	TU_IS_TUNE_NORMAL(tuneMode)	(TU_TUNE_NORMAL	== (tuneMode & TU_TUNE_MODE_MASK))
#define	TU_IS_TUNE_MANUAL(tuneMode)	(TU_TUNE_MANUAL	== (tuneMode & TU_TUNE_MODE_MASK))
#define	TU_IS_TUNE_AUTO(tuneMode)	(TU_TUNE_AUTO 	== (tuneMode & TU_TUNE_MODE_MASK))

typedef enum TU_TUNE_MODE
{
	/*	---------- NORMAL 	---------------------	*/
	TU_TUNE_NORMAL					= 0x20,		/* 001X XXXX */
		TU_TUNE_NOR_FINE,
		TU_TUNE_NOR_CHANNEL,
		TU_TUNE_NOR_CHAN_FINE,
		TU_TUNE_NOR_FREQUENCY,
		TU_TUNE_NOR_FREQ_FINE,

	/*	---------- SCAN 	---------------------	*/
	TU_TUNE_MANUAL					= 0x40,		/* 010X XXXX */
		TU_TUNE_MAN_CHANNEL,
		TU_TUNE_MAN_FREQUENCY,
		TU_TUNE_MAN_SYSTEM,
		TU_TUNE_MAN_FINE_UP,
		TU_TUNE_MAN_FINE_DN,
		TU_TUNE_MAN_SEARCH_UP,
		TU_TUNE_MAN_SEARCH_DN,

	TU_TUNE_AUTO					= 0x60,		/* 011X XXXX */
		TU_TUNE_AUTO_START,
		TU_TUNE_AUTO_CONTINUE,

	/*	---------- OTHERS 	---------------------	*/
	TU_TUNE_STOP					= 0x80,		/* 100X XXXX */
	TU_TUNE_FINISH,						/* Don't use this */
	TU_TUNE_PAUSE,						/* Don't use this */

	/*	---------- UNKNOWN 	---------------------	*/
	TU_TUNE_UNKNOWN					= 0xA0,		/* 101X XXXX */

	/*	---------- MASK 	---------------------	*/
	TU_TUNE_MODE_MASK				= 0xE0,		/* 111X XXXX */
} TU_TUNE_MODE_T;


/**
 * lock info struct.
 */
typedef struct
{
	BOOLEAN			bLockValid;
	TU_LOCK_STATE_T lockState;
} TU_LOCK_INFO_T;


/* 	____________________ D V B ____________________	*/


/**
 * output TS mode.
 */
typedef enum TU_OUTPUT_TS_MODE
{
	TU_OUTPUT_TS_SERIAL		= 0x00,
	TU_OUTPUT_TS_PARALLEL,

	TU_OUTPUT_TS_UNKNOWN
} TU_OUTPUT_TS_MODE_T;


/*
**	=======================================================
**	======================== D T V ========================
**	=======================================================
*/


/**
 * signal state.
 */
typedef struct TU_SIGNAL_STATE
{
	TU_FLAG_T	flagSignalValid;

	UINT8	strength;
	UINT8	quality;

	UINT32	packetError;
	UINT32	unBER;			/* unit : e-10 */
	UINT32	unAGC;
	UINT32	unSNR;
} TU_SIGNAL_STATE_T;


/*
**	=======================================================
**	======================= COUNTRY =======================
**	=======================================================
*/


/**
 * TPS : constellation.
 */
typedef enum TU_TPS_CONSTELLATION	/* 4 bit */
{
	TU_TPS_CONST_QPSK 		= 0x00,
	TU_TPS_CONST_QAM_4,
	TU_TPS_CONST_PSK_8,
	TU_TPS_CONST_QAM_16,
	TU_TPS_CONST_QAM_32,
	TU_TPS_CONST_QAM_64,
	TU_TPS_CONST_QAM_128,
	TU_TPS_CONST_QAM_256,
	TU_TPS_CONST_VSB_8,

	TU_TPS_CONST_END,

	TU_TPS_CONST_UNKNOWN 	= 0x0F
} TU_TPS_CONSTELLATION_T;


/**
 * TPS : guard interval.
 */
typedef enum TU_TPS_GUARD_INTERVAL	/* 4 bit */
{
	TU_TPS_GUARD_1_4 		= 0x00,
	TU_TPS_GUARD_1_8,
	TU_TPS_GUARD_1_16 ,
	TU_TPS_GUARD_1_32,
	TU_TPS_GUARD_1_9,

	TU_TPS_GUARD_595,
	TU_TPS_GUARD_420_C,
	TU_TPS_GUARD_420_V,
	TU_TPS_GUARD_945_C,
	TU_TPS_GUARD_945_V,

	TU_TPS_GUARD_END,

	TU_TPS_GUARD_UNKNOWN 	= 0x0F
} TU_TPS_GUARD_INTERVAL_T;


/**
 * TPS : code rate.
 */
typedef enum TU_TPS_CODERATE	/* 4 bit */
{
	TU_TPS_CODE_1_2 		= 0x00,
	TU_TPS_CODE_2_3,
	TU_TPS_CODE_3_4,
	TU_TPS_CODE_4_5,
	TU_TPS_CODE_5_6,
	TU_TPS_CODE_6_7,
	TU_TPS_CODE_7_8,
	TU_TPS_CODE_8_9,
	TU_TPS_CODE_2_5,
	TU_TPS_CODE_3_5,

	TU_TPS_CODE_END,

	TU_TPS_CODE_UNKNOWN 	= 0x0F
} TU_TPS_CODERATE_T;


/**
 * TPS : FFT mode.
 */
typedef enum TU_TPS_FFT_MODE	/* 3 bit */
{
	TU_TPS_FFT_2K 			= 0x00,
	TU_TPS_FFT_4K,
	TU_TPS_FFT_8K,

	TU_TPS_FFT_MC,
	TU_TPS_FFT_SC,

	TU_TPS_FFT_END,

	TU_TPS_FFT_UNKNOWN 		= 0x07
} TU_TPS_FFT_MODE_T;


/**
 * TPS : hierarchy.
 */
typedef enum TU_TPS_HIERARCHY	/* 3 bit */
{
	TU_TPS_HIERA_NONE 		= 0x00,
	TU_TPS_HIERA_1,
	TU_TPS_HIERA_2,
	TU_TPS_HIERA_4,

	TU_TPS_HIERA_END,

	TU_TPS_HIERA_UNKNOWN 	= 0x07
} TU_TPS_HIERARCHY_T;


/**
 * TPS : profile.
 */
typedef enum TU_TPS_PROFILE		/* 2 bit */
{
	TU_TPS_PROFILE_HP 		= 0x00,
	TU_TPS_PROFILE_LP,

	TU_TPS_PROFILE_END,

	TU_TPS_PROFILE_UNKNOWN	= 0x03
} TU_TPS_PROFILE_T;


/**
 * TPS : transmission mode.
 */
typedef enum TU_TPS_TRANS_MODE		/* 4 bit */
{
	TU_TPS_TRANS_M720 		= 0x00,		/* only DTMB */
	TU_TPS_TRANS_M240		= 0x01,		/* only DTMB */

//	TU_TPS_TRANS_QAM		= 0x02, 	/* only KOREA CABLE */
//	TU_TPS_TRANS_VSB 		= 0x03,		/* only KOREA CABLE */

	TU_TPS_TRANS_END,

	TU_TPS_TRANS_UNKNOWN	= 0x07 		/* Don't change the value */
} TU_TPS_TRANS_MODE_T;


/**
 * Special data : information struct.
 */
typedef struct TU_SPECIAL_DATA_ATV
{
	TU_AUDIO_SYSTEM_T		audioSystem;
	AUDIO_SIF_STANDARD_T	audioStandard;
} TU_SPECIAL_DATA_ATV_T;


/**
 * Special data : information struct.
 */
typedef struct TU_SPECIAL_DATA_DTV_TERR
{
	TU_TPS_TRANS_MODE_T		trasnMode;		/* 2 bit */

	TU_TPS_PROFILE_T		profile;		/* 2 bit */
	TU_TPS_HIERARCHY_T		hierarchy;		/* 3 bit */
	TU_TPS_FFT_MODE_T		enumFFTMode;	/* 3 bit */

	TU_TPS_CODERATE_T		codeRate4LP;	/* 4 bit */
	TU_TPS_CODERATE_T		codeRate4HP;	/* 4 bit */

	TU_TPS_GUARD_INTERVAL_T	guardInterval;	/* 4 bit */
	TU_TPS_CONSTELLATION_T	constellation;	/* 4 bit */

	UINT16					cellID; 		/* 16 bit : don't save this value to NVRAM */
} TU_SPECIAL_DATA_DTV_TERR_T;


/**
 * Special data : information struct.
 */
typedef struct TU_SPECIAL_DATA_DTV_CABLE
{
	TU_SYMBOL_RATE_KHZ_U32	symbolRate;		/* 16 bit */

	TU_TPS_CONSTELLATION_T	constellation;	/* 4 bit */
} TU_SPECIAL_DATA_DTV_CABLE_T;


/**
 * Special data : information struct.
 */
typedef union TU_SPDATA_UNION
{
	TU_SPECIAL_DATA_DTV_TERR_T	terrDTV;	/* DTV TERRESTRIAL */
	TU_SPECIAL_DATA_DTV_CABLE_T	cableDTV;	/* DTV CABLE */
	TU_SPECIAL_DATA_ATV_T		analog;		/* ANALOG */
} TU_SPDATA_UNION_T;


/**
 * Special data : information struct.
 */
typedef struct TU_SPECIAL_DATA
{
	BOOLEAN				bValid;
	TU_SPDATA_UNION_T	data;
} TU_SPECIAL_DATA_T;


/*
**	=======================================================
**	====================== PARAMETER ======================
**	=======================================================
*/


/**
 * DDI Parameter : INIT.
 */
typedef struct TU_INIT_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_IF_FREQ_KHZ_U32	unIFFreq;
	TU_CH_BW_KHZ_U16	channelBW;

	TU_OUTPUT_TS_MODE_T	outputTSMode;
}	TU_INIT_PARAM_T;


/**
 * DDI Parameter : Activation.
 */
typedef struct TU_ACTIVATION_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_OUTPUT_TS_MODE_T	outputTSMode;	// DTV only

	TU_FLAG_T			flagActivateOn;
}	TU_ACTIVATION_PARAM_T;


/**
 * DDI Parameter : Activate.
 */
typedef struct TU_ACTIVATE_PARAM
{
	TU_NUMBER_T			tunerNo;
	FE_TRANS_MEDIA_T	transMedia;

	BOOLEAN				bActive;
}	TU_ACTIVATE_PARAM_T;


/**
 * DDI Parameter : Tune.
 */
typedef struct TU_TUNE_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_TUNE_MODE_T		tuneMode;

//	UINT8		channelNo;		/* 0 : invalid */
	UINT32		frequency;		/* 0 : invalid */
	SINT32		freqOffset;		/* will be deleted. ATSC only */

	TU_SPECIAL_DATA_T	specData;
	TU_IF_FREQ_KHZ_U32	unIFFreq;
	TU_STEP_SIZE_HZ_U32 stepSize;
	TU_CH_BW_KHZ_U16 	channelBW;
	UINT16				captureRange;	/* DTV : kHz */
}	TU_TUNE_PARAM_T;

typedef TU_TUNE_PARAM_T TU_SET_PLL_PARAM_T;

#if 0
/**
 * DDI Parameter : Tune.
 */
typedef struct TU_SET_PLL_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_TUNE_MODE_T		tuneMode;

	UINT32	frequency;		/* 0 : invalid */
	SINT32	freqOffset;		/* will be deleted. ATSC only */

	TU_IF_FREQ_KHZ_U32	unIFFreq;
	TU_STEP_SIZE_HZ_U32 stepSize;
	TU_CH_BW_KHZ_U16 	channelBW;
	UINT16				captureRange;	/* DTV : kHz */

	TU_SPECIAL_DATA_T	specData;
}	TU_SET_PLL_PARAM_T;
#endif


/**
 * DDI Parameter : Tune postjob.
 */
typedef struct TU_TUNE_POSTJOB_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_TUNE_MODE_T		tuneMode;

	BOOLEAN		bAutoBooster;

	SINT32		*pFrequency;		/* frequency or offset */
	TU_FLAG_T 	*pFlagPostJob;		/* TU_FLAG_ING, TU_FLAG_FAIL(fail, stop), TU_FLAG_FINISH(lock state is valid) */
}	TU_TUNE_POSTJOB_PARAM_T;


/**
 * DDI Parameter : control output.
 */
typedef struct
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_FLAG_T			flagOutputEnable;
} TU_CONTROL_OUTPUT_PARAM_T;


/**
 * DDI Parameter : control TS output.
 */
typedef struct
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_OUTPUT_TS_MODE_T	outputTSMode;
} TU_CONTROL_TS_PARAM_T;


/**
 * DDI Parameter : lock state.
 */
typedef struct
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	BOOLEAN				bReset;

	TU_LOCK_STATE_T		*pLockState;
} TU_CHECK_LOCK_STATE_PARAM_T;


/**
 * DDI Parameter : signal state.
 */
typedef struct TU_CHECK_SIGNAL_STATE_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;
	BOOLEAN				bReset;

	TU_SIGNAL_STATE_T	*pSignalState;
} TU_CHECK_SIGNAL_STATE_PARAM_T;


/**
 * DDI Parameter : special data.
 */
typedef struct TU_CHECK_SPECIAL_DATA_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_SPECIAL_DATA_T	*pSpecData;

	UINT16 cellID;
} TU_CHECK_SPECIAL_DATA_PARAM_T;


/**
 * DDI Parameter : special data.
 */
typedef struct TU_SET_SPECIAL_DATA_PARAM
{
	TU_NUMBER_T			tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	TU_SPECIAL_DATA_T	specData;
} TU_SET_SPECIAL_DATA_PARAM_T;


/**
 * DDI Parameter : frequency offset.
 */
typedef struct TU_CHECK_FREQ_OFFSET_PARAM
{
	TU_NUMBER_T		tunerNo;

	TU_TRANS_MODE_T		transMode;	// will be deleted
	FE_TRANS_MEDIA_T	transMedia;
	TU_TRANS_SYSTEM_T	transSystem;

	SINT32	*pFreqOffset;	/* offseted frequency */
} TU_CHECK_FREQ_OFFSET_PARAM_T;


/******************************************************************************
	Function Declaration
******************************************************************************/

extern	void 			DDI_TU_PowerOnReset(void);
extern	DTV_STATUS_T 	DDI_TU_Probe(TU_NUMBER_T tunerNo);									/* === mandatory === */
extern	DTV_STATUS_T 	DDI_TU_Initialize(TU_INIT_PARAM_T paramStruct);								/* optional 	*/
extern	DTV_STATUS_T 	DDI_TU_Activate (TU_ACTIVATE_PARAM_T paramStruct);							/* optional 	*/
extern	DTV_STATUS_T 	DDI_TU_Activation(TU_ACTIVATION_PARAM_T paramStruct);						/* optional 	*/
extern	DTV_STATUS_T 	DDI_TU_SetPLL(TU_SET_PLL_PARAM_T paramStruct);						/* === mandatory === */
extern	DTV_STATUS_T 	DDI_TU_SetDemod(TU_TUNE_PARAM_T paramStruct);						/* === mandatory === */
extern	DTV_STATUS_T 	DDI_TU_TunePostJob(TU_TUNE_POSTJOB_PARAM_T paramStruct);				/* === mandatory === */
extern	DTV_STATUS_T 	DDI_TU_ControlOutput(TU_CONTROL_OUTPUT_PARAM_T paramStruct);				/* optional 	*/
extern	DTV_STATUS_T 	DDI_TU_ControlTSMode(TU_CONTROL_TS_PARAM_T paramStruct);					/* optional 	*/
extern 	DTV_STATUS_T 	DDI_TU_CheckLockState(TU_CHECK_LOCK_STATE_PARAM_T paramStruct);		/* === mandatory === */
extern	DTV_STATUS_T 	DDI_TU_CheckSignalState(TU_CHECK_SIGNAL_STATE_PARAM_T paramStruct);		/* optional 	*/
extern	DTV_STATUS_T	DDI_TU_CheckSpecialData(TU_CHECK_SPECIAL_DATA_PARAM_T paramStruct);		/* optional 	*/
extern	DTV_STATUS_T	DDI_TU_SetSpecialData(TU_SET_SPECIAL_DATA_PARAM_T paramStruct);			/* optional 	*/
extern	DTV_STATUS_T 	DDI_TU_CheckFrequencyOffset(TU_CHECK_FREQ_OFFSET_PARAM_T paramStruct);	/* optional 	*/

extern	DTV_STATUS_T	DDI_TU_DebugMenu(TU_NUMBER_T tunerNo);

#endif	/* _TU_DDI_H_ */

