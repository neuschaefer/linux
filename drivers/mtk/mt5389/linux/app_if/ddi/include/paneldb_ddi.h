/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file paneldb_ddi.h
 *
 * This source file defines the DDI functions related to panel spec
 *
 *  @author	Lee, byung gul(lachesis@lge.com)
 *  @version	0.1
 *  @date		2009.03.09
 *  @note		Additional information.
 *  @see		paneldb_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_PANELDB_DDI_H_
#define	_PANELDB_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "osa_modeldef.h"

//#define USE_RGB_DAC_1024X768	// RGB DAC for 1024X768

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
   로컬 상수 정의 (Local Constant Definitions)
******************************************************************************/

/******************************************************************************
    로컬 형 정의 (Local Type Definitions)
******************************************************************************/

/******************************************************************************
    전역 형 정의 (Global Type Definitions)
******************************************************************************/
/**
 * panel type.
 *
 * @see
*/
typedef enum
{
	PANEL_TYPE_LCD,
	PANEL_TYPE_LCD_TCON,    // shjang_090904
	PANEL_TYPE_LCD_FRC,
	PANEL_TYPE_PDP,
	PANEL_TYPE_LED,
	PANEL_TYPE_OLED,

	PANEL_TYPE_NUM,
}PANEL_TYPE_T;

/**
 * panel maker.
 *
 * @see
*/
typedef enum
{
	LCD_PANEL_MAKER_LGD,
	LCD_PANEL_MAKER_CMI,
	LCD_PANEL_MAKER_AUO,
	LCD_PANEL_MAKER_SHARP,
	LCD_PANEL_MAKER_IPS,
	PDP_PANEL_MAKER_LGE,
	PANEL_MAKER_NUM,
}PANEL_MAKER_T;

/**
 * LVDS bit depth.
 *
 * @see
*/
typedef enum
{
	COLOR_DEPTH_8BIT,
	COLOR_DEPTH_10BIT,
	COLOR_DEPTH_NUM,
}COLOR_DEPTH_T;

/**
 * LVDS chanel info.
 *
 * @see
*/
typedef enum
{
	LVDS_PIXEL_SINGLE,
	LVDS_PIXEL_DUAL,
	LVDS_PIXEL_NUM,
}LVDS_PIXEL_T;

/**
 * LVDS type.
 *
 * @see
*/
typedef enum
{
	LVDS_SELECT_VESA,
	LVDS_SELECT_JEIDA,
	LVDS_SELECT_NUM,
}LVDS_SELECT_T;

/**
 * Panel Interface type.
 *
 * @see
*/
typedef enum
{
	PANEL_INTERFACE_TTL,
	PANEL_INTERFACE_LVDS,
	PANEL_INTERFACE_MINILVDS,
	PANEL_INTERFACE_RSDS,
}PANEL_INTERFACE_T;


/**
 * panel attribute.
 *
 * @see
*/
typedef struct
{
	PANEL_TYPE_T	ePanelType;	/**PANEL_TYPE_T */
	COLOR_DEPTH_T	eColorDepth;	/**COLOR_DEPTH_T */
	LVDS_SELECT_T	eLVDSSelect;	/**LVDS_SELECT_T */
	LVDS_PIXEL_T	eLVDSPixel;	/**LVDS_PIXEL_T */
	PANEL_INTERFACE_T	ePanelInterface;/**PANEL_INTERFACE_T*/
	FRC_CHIP_TYPE_T	eFRCType;/**FRC_CHIP_TYPE_T*/

	SINT32 panelResolution; 	/** */
	INCH_TYPE_T ePanelInch;		 	/** */
	//------------ signal timing -----------------
	//UINT16 TypDclkHz; 		/** panel D clock*/
	UINT16 horizTypPeriod;		/** panel Horizon Period */
	UINT16 vertTypPeriod60Hz;	/** panel Vertical Period for NTSC*/
	UINT8 vertSyncWidth60Hz;	/** panel Vertical Sync Width for NTSC*/
	UINT8 vertBackPorch60Hz;	/** panel Vertical Back Porch for NTSC*/
	UINT16 vertTypPeriod50Hz;	/** panel Vertical Period for PAL*/
	UINT8 vertSyncWidth50Hz;	/** panel Vertical Sync Width for PAL*/
	UINT8 vertBackPorch50Hz;	/** panel Vertical Back Porch for PAL*/
	UINT16 vertTypPeriod48Hz;	/** panel Vertical Period for 48Hz*/
	UINT8 vertSyncWidth48Hz;	/** panel Vertical Sync Width for 48Hz*/
	UINT8 vertBackPorch48Hz;	/** panel Vertical Back Porch for 48Hz*/
}PANEL_ATTRIBUTE_T;


/**
 * panel power seq.
 *
 * @see
*/
typedef struct
{
	UINT8 panelPowOnToData;	/** Power Sequence for T2, refer to CAS*/
	UINT8 dataToLampOn;	/** Power Sequence for T3, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 lampOffToData;	/** Power Sequence for T4, refer to CAS*/	/* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 dataToPanelPowOff;	/** Power Sequence for T5, refer to CAS*/

	UINT8 SOEtoDPM;			/** Power Sequence for T6, refer to CAS*/
	UINT8 DPMtoVGH;			/** Power Sequence for T6, refer to CAS*/
	UINT8 VGHtoVST;			/** Power Sequence for T6, refer to CAS*/
	UINT8 VSTtoGCLK;		/** Power Sequence for T7, refer to CAS*/

	UINT8 nRLOntoDimSig;	/** Power Sequence for Power Input for BLU to Dimmng Control Signal, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
	UINT8 nDimSigtoLamp;	/** Power Sequence for Dimmng Control Signal to Lamp On, refer to CAS*/ /* 초기값은 10ms 단위로 설정하고, 실제 delay 함수 사용시 X10 할것 */
}PANEL_POWER_SEQ_T;

/**
 * panel pwm info.
 *
 * @see
*/
typedef struct
{
	UINT8 vbrAMinVdc;	/** refer to CAS*/
	UINT8 vbrATypVdc;	/** refer to CAS*/
	UINT8 vbrAMaxVdc;	/** refer to CAS*/
	UINT8 vbrBMinDuty;	/** refer to CAS*/
	UINT8 vbrBMaxDuty;	/** refer to CAS*/
	UINT8 vbrBMaxDuty3D; /** 측정치 for 3D on*/
	UINT8 vbrBDCRDuty;/** refer to CAS*/
	UINT8 vbrBNoSignalDuty;	/** refer to CAS*/
	UINT8 vbrBFreq60hz; /** refer to CAS*/
	UINT8 vbrBFreq50hz; /** refer to CAS*/
}PANEL_PWM_T;


/**
 * panel info.
 *
 * @see
*/
typedef struct
{
	UINT8				*pPanelName;
	PANEL_MAKER_T		ePanelMaker;
	PANEL_ATTRIBUTE_T	stAttribute;
	PANEL_POWER_SEQ_T	stPowSeq;
	const PANEL_PWM_T 		*pstPWM;
}PANEL_INFO_T;

/******************************************************************************
    Extern 전역변수와 함수 prototype 선언
    (Extern Variables & Function Prototype Declarations)
******************************************************************************/

/******************************************************************************
    Static 변수와 함수 prototype 선언
    (Static Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    로컬 변수와 함수 prototype 선언
    (Local Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    전역 변수와 함수 prototype 선언
    (Global Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
	함수 정의 (Function Definitions)
******************************************************************************/
DTV_STATUS_T DDI_PANELDB_Initialize(void);
DTV_STATUS_T DDI_SysDB_Verify(void);

DTV_STATUS_T DDI_PANELDB_GetPanelInfo(PANEL_INFO_T * pstPanelInfo);
DTV_STATUS_T DDI_PANELDB_GetPanelAttrInfo(PANEL_ATTRIBUTE_T* pstPanelAttrInfo);
DTV_STATUS_T DDI_PANELDB_GetPanelPWMInfo(PANEL_PWM_T * pstPanelPWMInfo);
DTV_STATUS_T DDI_PANELDB_GetPanelPowSeqInfo(PANEL_POWER_SEQ_T* pstPanelPowSeqInfo);

DTV_STATUS_T DDI_PANELDB_NotifyPanelInfo(PANEL_INFO_T * pstPanelInfo);
DTV_STATUS_T DDI_PANELDB_ShowPanelInfo(PANEL_INFO_T * pstPanelInfo);
DTV_STATUS_T DDI_PANELDB_UpdateSysDB(void);
API_STATE_T	 API_DDM_PANELLCD_SetTCONPower(BOOLEAN bEnable, PANEL_POWER_SEQ_T tcon_power);


#endif
