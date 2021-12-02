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
 *  @author     BongSeok Kim
 *  @author     ByoungHo Lee
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

/******************************************************************************
    상수 정의(Constant Definitions)
******************************************************************************/
#define ADJ_INPUT_NUM               3 //080812 BSG
#define BAR_CODE_SIZE_MAX           17      /* 16 + 1(null) */

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
    형 정의 (Type Definitions)
******************************************************************************/
/**
    System DB 모듈을 위한 실제 사용 type 정의
*/
typedef struct
{
    UINT32          	validMark;
    OSA_PRINT_MASK_T	printMask;
    UINT32          	sys_utt;/* PDP panel use time */
    //UINT16          	powerMode;
    UINT8           	powerOnMode;	/* internal micom:  POWER ON MODE	*/
    UINT8           	powerState;		/* internal micom:	POWER STATE		*/
    UINT8           	powerOnStatus;	/* internal micom:  HOTEL: POWER ON STATUS */
    UINT8           	setID;			/* internal micom:  SET ID			*/
    UINT8           	keyLocked;		/* internal micom:  HOTEL: KEY OPERATION MODE */
    UINT8           	irLocked; 		/* internal micom:	IR KEY LOCKED	*/
    UINT8           	localLocked;	/* internal micom:  LOCAL KEY LOCKED*/


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
    LUMA_WB_DB_T        luma_wb_db_dtv;     // Digital TV : Antenna/Cable
    LUMA_WB_DB_T        luma_wb_db_arf;     // Analog RF : Antenna/Cable
    LUMA_WB_DB_T        luma_wb_db_av1;     // AV1
    LUMA_WB_DB_T        luma_wb_db_av2;     // AV2
    LUMA_WB_DB_T        luma_wb_db_comp1;   // Component1
    LUMA_WB_DB_T        luma_wb_db_comp2;   // Component2
    LUMA_WB_DB_T        luma_wb_db_rgb;     // RGB
    LUMA_WB_DB_T        luma_wb_db_hdmi1;   // HDMI1
    LUMA_WB_DB_T        luma_wb_db_hdmi2;   // HDMI2
    LUMA_WB_DB_T        luma_wb_db_1394;    // IEEE1394
/* Adjust Status Check RF AV 조정 분리 함 */
    UINT8               check_ADC_RF;       /* RF Adjust success check */
    UINT8               check_ADC_AV;       /* AV Adjust success check */
    UINT8               check_ADC_COMP_480I;     /* Component 480i Adjust success check */
    UINT8               check_ADC_COMP_1080P;     /* Component 1080P Adjust success check */
    UINT8               check_ADC_RGB;      /* RGB Adjust success check */
    UINT8               check_WB;            /* WB Adjust success check */
/* Serial Number추가 */
    UINT8               aSerialNum[BAR_CODE_SIZE_MAX];
/* EDID D/L Status Check */
    UINT8               check_HDMI1;        /* RF Adjust success check */
    UINT8               check_HDMI2;        /* AV Adjust success check */
    UINT8               check_HDMI3;        /* Component Adjust success check */
    UINT8               check_HDMI4;        /* RGB Adjust success check */
    UINT8               check_RGB;          /* RGB Adjust success check */
} ANA_DB_T;

/* 아래 gAnaNvmDB 구조체는 Analog DB의 초기값을 가지고 있는 구조체로서
   아래 macro 함수 내부 처리를 위해 이곳에 선언되어 있으나, 다른 모듈에서
   직접 참조할 이유가 없으므로 사용하지 말것. by CS Kim */
extern  ANA_DB_T gAnaNvmDB;

/**
 *  Tool Option of Ez Adjust Menu(Service Menu)
 */
typedef union
{
	UINT32 all;
	struct
	{
		INCH_TYPE_T 	eModelInchType  	: 4;
//		TOOL_TYPE_T		eModelToolType		: 5;	// PDP Tool Type add
		TOOL_TYPE_T		eModelToolType		: 4;	// PDP Tool Type add
		MODULE_MAKER_T	eModelModuleType	: 3;
		UINT8       	bModuleVer      	: 2;
		UINT8       	bIntelligentSensor	: 1;
		UINT8       	bPowerIndicator 	: 1;
		UINT8       	bSRSHD          	: 1;
	} flags;

} TOOL_OPTION1_T;

/**
 *	AREAR OPTION struct
 */
typedef struct
{
	UINT8				nAreaCode;
	UINT8				nA2_ST;
    BOOLEAN             bIsI_IISave;            /* I/II Save 지원유무 */
	UINT8               nHDEV;                  /* Audio HDEV On/Off*/
	BOOLEAN             bIsMono;                /* 강제 모노 지원유무 */
	UINT8				nLangGr;				/* Langage Group */
	UINT8				nTeletextLangGr;		/* Teletext Langage Group */
	BOOLEAN				bOverModulation;
	UINT8				reserved;

}	AREA_OPTIONS_T;

/**
 *	ANALOG SYSTEM struct
 */
typedef enum
{
    ANA_NTSC            = 0,
    ANA_3_SYS           = 1,
    ANA_4_SYS           = 2,
    ANA_7_SYS           = 3

}	SYS_ANALOG_T;

/**
 *	BAUDRATE struct
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
 *	AUDIO DELAY struct
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
 *	SYSTEM PARAMTER struct
 */
typedef struct
{
    SYS_ANALOG_T        eAnalogSystem;
    SYS_BAUDRATE_T      eBaudrate;
    UINT8               bIs2HourOff;            /* 2시간 자동 Off 기능동작 여부 */
    UINT8               nAudioEQ;               /* Audio EQ On/Off*/
	UINT8				nA2_threshold;
	BOOLEAN				bHDMI_SoundPath;		/* Hdmi sound path(HDMI Port1/RGB Phone Jack) */
	BOOLEAN				b15MinForceOff;
	BOOLEAN				bBlueback;
	UINT8				nTouchSensitivity;

}	SYSTEM_PARAM_T;

/**
 *	PANEL AV or PC
 */
typedef enum
{
    PANNEL_AUTO         = 0,
    PANNEL_AV           = 1,
    PANNEL_PC           = 2

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
    PANNEL_POWER_SAVE_T eSelPowerSave;
    PANNEL_BRIGHT_T     eSelBright;
    PANNEL_LOCK_T       eSelPnlLock;
    PANNEL_POWER_SAVE_T ePowerSaving;
    UINT8               nOrbitPixel;            /* 1 ~ 3 */
    UINT8               nOrbitStep;             /* 1 ~ 10 Step */
    UINT8               nOrbitTime;             /* 1 ~ 255 sec */
    UINT8               nInvTime;               /* 1 ~ 255 min */
    BOOLEAN             bIsModuleDL;            /* TRUE : Module Download 상태(고압 Off) */
    BOOLEAN             bIsFMC;                 /* TRUE : FMC ON */
    BOOLEAN             bIsDPS;                 /* TRUE : DPS ON */
    BOOLEAN             bIsAXCC;                /* TRUE : AXCC ON */
    BOOLEAN             bIsFreshWhite;          /* TRUE : FreshWhite ON */
    UINT32              nUseTime;               /* USE Time */
    UINT8               nPrevDPMode;            /*전시모드 진입 이전의 av/pc mode; PDP Only */
    UINT8               nPrevPSModeValue;       /* 전시모드 진입시 이전의 PSMode 값을 backup 한다 */

}	PANNEL_ADJ_T;

/**
 *	TEMPTH DB struct type
 */
typedef struct
{
    UINT8 DnLow;
    UINT8 DnHigh;
    UINT8 UpLow;
    UINT8 UpHigh;

}	TEMPTH_DB_T;

/**
 *	FACTORY RESET DB struct type
 */
typedef struct
{
    UINT32              validMark;
	TOOL_OPTION1_T  	nToolOption1;
    //MODEL_OPTIONS_T     stModelOptions;         /* Model Options */
	AREA_OPTIONS_T		stAreaOptions;			/* Area Option for 아주/중아*/
	ADJUST_SUB_BC_T			stSubBC;		/* Sub Bright, Contrast */
    SYSTEM_PARAM_T      stSysParam;
    PANNEL_ADJ_T        stPannelAdj;            /* Pannel Adjust Items */
    TEMPTH_DB_T         stTempthreshold;        /* temperature threshold 결정하여 fan control */
    BOOLEAN             AutoTestStatus;
    BOOLEAN             StoreModeTest;          /*Test Option-> Store Mode Test*/
    UINT8               nStartupSound;
    UINT8               nStandbyLight;
    UINT8               nPoweronLight;
}	FACTORY_DB_T;


extern FACTORY_DB_T gFactoryDB;

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

extern VERSION_DB_T    gVersionDB;

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
#if 0
extern  DTV_STATUS_T DDI_NVM_WriteTwo (UINT32 offset, UINT16 value);
extern  DTV_STATUS_T DDI_NVM_ReadTwo (UINT32 offset, UINT16 *value);
#endif

/*  nvmdb_ddi.c Functions */
extern  DTV_STATUS_T DDI_NVM_InitSysDB (void);
extern  DTV_STATUS_T DDI_NVM_InitFactoryDB (void);
extern  DTV_STATUS_T DDI_NVM_InitAnalogDB (PQL_WB_DATA_T *pWBData);
extern  DTV_STATUS_T DDI_NVM_ADCCalDataWrite(UINT32 adjSrc, ADC_CALIB_DATA_T *pADCVal);
extern  DTV_STATUS_T DDI_NVM_ADCCalDataRead( UINT32 adjSrc, ADC_CALIB_DATA_T *pADCVal);
extern  DTV_STATUS_T DDI_NVM_GetModelOptions(TOOL_OPTION1_T *pstModelOpt);

#ifdef __cplusplus
}
#endif

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

#if	(DEV_STATE == DEBUG_STATE)
#define	_DDI_NVM_READ		DDI_NVM_Read
#define	_DDI_NVM_WRITE		DDI_NVM_Write
#else
#error
#define	_DDI_NVM_READ		DDI_NVM_Read
#define	_DDI_NVM_WRITE		DDI_NVM_Write
#endif

#define NVM_MAGIC_STORE(pValue)		(_DDI_NVM_WRITE(TNVM_MAGIC_BASE, TNVM_MAGIC_SIZE, (UINT8 *)(pValue)))
#define NVM_MAGIC_LOAD( pValue)		(_DDI_NVM_READ (TNVM_MAGIC_BASE, TNVM_MAGIC_SIZE, (UINT8 *)(pValue)))

#define NVM_SYS_DB_STORE_ITEM(item, pValue)      \
        (_DDI_NVM_WRITE (SYS_DB_BASE + (UINT32)&(gSysNvmDB.item) - (UINT32)&gSysNvmDB,          \
                sizeof(gSysNvmDB.item), (UINT8 *)(pValue)))
#define NVM_SYS_DB_LOAD_ITEM(item, pValue)       \
        (_DDI_NVM_READ (SYS_DB_BASE + (UINT32)&(gSysNvmDB.item) - (UINT32)&gSysNvmDB,           \
                sizeof(gSysNvmDB.item), (UINT8 *)(pValue)))
#define NVM_SYS_DB_GET_DEFAULT(item, pValue)     \
        (*pValue = gSysNvmDB.item)

#define NVM_ANA_DB_STORE_ITEM(item, pValue)      \
        (_DDI_NVM_WRITE (ANA_DB_BASE + (UINT32)&(gAnaNvmDB.item) - (UINT32)&gAnaNvmDB,          \
                sizeof(gAnaNvmDB.item), (UINT8 *)(pValue)))
#define NVM_ANA_DB_LOAD_ITEM(item, pValue)       \
        (_DDI_NVM_READ (ANA_DB_BASE + (UINT32)&(gAnaNvmDB.item) - (UINT32)&gAnaNvmDB,           \
                sizeof(gAnaNvmDB.item), (UINT8 *)(pValue)))
#define NVM_ANA_DB_GET_DEFAULT(item, pValue)     \
                (*pValue = gAnaNvmDB.item)

#define NVM_FACTORY_DB_STORE_ITEM(item, pValue)  \
        (_DDI_NVM_WRITE (FACTORY_DB_BASE + (UINT32)&(gFactoryDB.item) - (UINT32)&gFactoryDB,    \
                sizeof(gFactoryDB.item), (UINT8 *)(pValue)))
#define NVM_FACTORY_DB_LOAD_ITEM(item, pValue)   \
        (_DDI_NVM_READ (FACTORY_DB_BASE + (UINT32)&(gFactoryDB.item) - (UINT32)&gFactoryDB,     \
                sizeof(gFactoryDB.item), (UINT8 *)(pValue)))
#define NVM_FACTORY_DB_GET_DEFAULT(item, pValue) \
        (*pValue = gFactoryDB.item)

#endif  /* End of _NVM_DDI_H_ */
