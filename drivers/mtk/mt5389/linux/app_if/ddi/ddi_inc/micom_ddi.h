/******************************************************************************
*	DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA.
*	Copyright(c) 1999 by LG Electronics Inc.
*
*	 All rights reserved. No part of this work may be reproduced, stored in a
*	 retrieval system, or transmitted by any means without prior written
*	 permission of LG Electronics Inc.
*****************************************************************************/

/** @file micom_ddi.h
*
*	Micom Interface header file.
*
*	@author Moon Kang yun
*	@author Changseob Kim
*	@author Yong-Dong Lee(cosstar@lge.com) 	- modifier
*	@author Kun-IL Lee(dreamer@lge.com) 	- modifier
*	@version	1.0
*	@date	  2006.07.08
*	@note
*/

/*---------------------------------------------------------
    (Header File Guarder )
---------------------------------------------------------*/
#ifndef _MICOM_DDI_H_
#define _MICOM_DDI_H_

/*---------------------------------------------------------
    Control 상수 정의
    (Control Constants)
---------------------------------------------------------*/

/*---------------------------------------------------------
    #include 파일들
    (File Inclusions)
---------------------------------------------------------*/
#include "global_configurations.h"

/*---------------------------------------------------------
    상수 정의
    (Constant Definitions)
---------------------------------------------------------*/

// moved to global_configurations.h
// #define MICOM_SIZE_OF_POWER_OFF_DATA			(128)	// 0x80: index(1) + power off status(127)

/*---------------------------------------------------------
    매크로 함수 정의
    (Macro Definitions)
---------------------------------------------------------*/

/*	I2C Access Macro */
//#if		(PLATFORM_TYPE == BCM_PLATFORM)    
#if 0
#define I2cReadMICOM(  a, b, c, d, e )	DDI_I2C_SlowRead(  I2C_DEV_MICOM,(a),(b),(c),(d),(e) )
#define I2cWriteMICOM( a, b, c, d, e )	DDI_I2C_SlowWrite( I2C_DEV_MICOM,(a),(b),(c),(d),(e) )
#else
#define I2cReadMICOM(  a, b, c, d, e )	DDI_I2C_Read(  I2C_DEV_MICOM,(a),(b),(c),(d),(e) )
#define I2cWriteMICOM( a, b, c, d, e )	DDI_I2C_Write( I2C_DEV_MICOM,(a),(b),(c),(d),(e) )
#endif

/*---------------------------------------------------------
    Type 정의
    (Type Definitions)
---------------------------------------------------------*/

/**
 * MICOM's version number
 *
 * @see
*/
typedef	struct
{
	UINT8	number[6];	/**< version number	*/

}	MICOM_VERSION_T;

#ifdef INCLUDE_HDMICEC
/**
 * HDMI CEC
 *
 * @see
*/
typedef	struct
{
	UINT8	command;		/**< HDMI CEC: command	*/
	UINT8	number;			/**< HDMI CEC: number	*/
	UINT8	msgID;			/**< HDMI CEC: message ID	*/
	UINT8	cecdata[20];	/**< HDMI CEC: CEC data	*/

}	MICOM_HDMICEC_T;

#endif	/* #ifdef INCLUDE_HDMICEC */

#ifdef INCLUDE_HOTEL_MODE
/**
 * HOTEL_MODE: Power Control Mode
 *
 * @see
*/
typedef enum
{
	MICOM_PWC_LAST_POWER		= 0,	/**< Power Control Mode: LAST POWER	*/
	MICOM_PWC_ONLY_STBY 		= 1,	/**< Power Control Mode: Only Stand by  */
	MICOM_PWC_ONLY_POWON		= 2,	/**< Power Control Mode: Only Power ON	*/
	MICOM_PWC_UNKNOWN   		= 3		/**< Power Control Mode: UNKNOWN(PARAM 범위) */

} 	MICOM_PWC_T;

/**
 * HOTEL_MODE: key operarion type.
 *
 * @see
*/
typedef enum
{
	MICOM_KEYOP_NORMAL      	= 0,	/**< key Operation NORMAL 상태				*/
	MICOM_KEYOP_ONLY_IR     	= 1,	/**< key Operation IR만 처리하는 상태		*/
	MICOM_KEYOP_ONLY_LOCAL   	= 2,	/**< key Operation LOCAL키만 처리하는 상태	*/
	MICOM_KEYOP_ONLY_IR_POWER	= 3,	/**< key Operation IR POWER만 처리하는 상태	*/
	MICOM_KEYOP_UNKNOWN      	= 4 	/**< key Operation UNKNOWN 상태(PARAM 범위)	*/

} 	MICOM_KEY_OPERATION_T;

#endif	/* #ifdef INCLUDE_HOTEL_MODE */

/**
 * Power indicator command.
 *
 * @see	MICOM_GetImageType( UINT8 * pImageType)
 */
typedef enum
{
	PWR_IND_STANDBY_LED	= 0, 	/**< STANDBY LED의 level 조정  	*/
	PWR_IND_POWER_LED	= 1, 	/**< POWER LED ON/OFF			*/
	PWR_IND_SA_SOUND	= 2,	/**< SA SOUND ON/OFF			*/
	PWR_IND_DEMO_MODE	= 3,	/**< DEMO MODE ON/OFF			*/
	PWR_IND_LAST				/**< LAST VALUE to Check Params */

}	DDI_MICOM_PWR_IND_CMD_T;	// POWER INDICATOR

/** UI Messaging System에 사용되어지는 Callback 함수를 위한 정의.
 *	API_UTIL_InitUIMessagingSystem 함수 호출 시 사용되어진다.
 */
#define  MAX_MICOM2UI_MSG_PARAM 	4

typedef void (*DDI_MICOM_UI_MSG_CALLBACK_PTR_T) (UINT32 msg, UINT32 param[MAX_MICOM2UI_MSG_PARAM]);
typedef void (*DDI_MICOM_UI_KEY_CALLBACK_PTR_T) (UINT32 key, UINT32 keyType );
//typedef void (*DDI_MICOM_UI_KEY_CALLBACK_PTR_T) (UINT32 key, BOOLEAN bRepeat);
typedef void (*DDI_MICOM_AUTOTEST_CALLBACK_PTR_T) (UINT8 key, UINT8 keyType );

/*---------------------------------------------------------
    함수 선언
    (Function Declaration)
---------------------------------------------------------*/

/*=======================================================
	MICOM Interface Function 선언 (micom_task.c)
=======================================================*/
extern	DTV_STATUS_T	DDI_MICOM_Initialize(void);
extern	void 			DDI_MICOM_AutoTest(UINT8 type, UINT8 value);
extern	DTV_STATUS_T	DDI_MICOM_SetPowerKeyLock( BOOLEAN isLocked );
extern	DTV_STATUS_T	DDI_MICOM_SetTVLinkIRMode( BOOLEAN isEnabled );

extern	void 			DDI_MICOM_RegisterUICallback( 	DDI_MICOM_UI_MSG_CALLBACK_PTR_T pfnMsgCbFn,
														DDI_MICOM_UI_KEY_CALLBACK_PTR_T pfnKeyCbFn  );
extern 	void 			DDI_MICOM_RegisterAutoTestCallback( DDI_MICOM_AUTOTEST_CALLBACK_PTR_T pfnAutoTestCbFn );

/*=======================================================
	MICOM Interface Function 선언 (micom_if.c)
=======================================================*/
extern	DTV_STATUS_T	DDI_MICOM_GetVersion (MICOM_VERSION_T *pVersion);

//extern	DTV_STATUS_T	DDI_MICOM_GetRTCTime (TIME_T *pTime);
extern	DTV_STATUS_T	DDI_MICOM_SetRTCTime (TIME_T *pTime);
extern	DTV_STATUS_T	DDI_MICOM_SetOnTime (UINT8 hour, UINT8 minute, UINT8 repeat);
extern	DTV_STATUS_T	DDI_MICOM_CancelOnTime (void);
extern	DTV_STATUS_T	DDI_MICOM_SetOffTime (UINT8 hour, UINT8 minute, UINT8 repeat);
extern	DTV_STATUS_T	DDI_MICOM_CancelOffTime (void);
extern	DTV_STATUS_T	DDI_MICOM_SetReserveTime (TIME_T *pTime, BOOLEAN isReserved );
extern	DTV_STATUS_T	DDI_MICOM_CancelReserveTime (void);
extern	DTV_STATUS_T	DDI_MICOM_SetOTAOnTime (TIME_T *pTime);
extern	DTV_STATUS_T	DDI_MICOM_GetOTATime (TIME_T *pTime);
extern	DTV_STATUS_T	DDI_MICOM_SetOTATime (TIME_T *pTime);

extern	DTV_STATUS_T	DDI_MICOM_SetRMCKeyLock( BOOLEAN isLocked );
extern	DTV_STATUS_T	DDI_MICOM_SetChildLock( BOOLEAN isLocked );
extern	DTV_STATUS_T	DDI_MICOM_GetReserveTime (TIME_T *pTime);
extern	DTV_STATUS_T	DDI_MICOM_SetPollingMode( BOOLEAN isEnabled );
extern	DTV_STATUS_T	DDI_MICOM_SetCommSetID (UINT8 setID);
extern	DTV_STATUS_T	DDI_MICOM_EnableEmergencyMode(BOOLEAN isEnabled);

#ifdef INCLUDE_HOTEL_MODE
extern	DTV_STATUS_T	DDI_MICOM_PowerModeCtrl (MICOM_PWC_T powermode );
extern	DTV_STATUS_T	DDI_MICOM_KeyOperationModeCtrl (MICOM_KEY_OPERATION_T keyoperation );
#endif

#ifdef INCLUDE_HDMICEC
extern	void        	MTAL_MICOM_InitCecModule(void);//080811_yongs
extern	DTV_STATUS_T	MTAL_MICOM_RequestCecForward(void);
extern	DTV_STATUS_T	MTAL_MICOM_SendCecData (UINT8 *pCmd);
#endif


/*	MICOM's POWER MANAGEMENT	micom_power.c  */
extern	DTV_STATUS_T	DDI_MICOM_GetPowerState (DDI_POWER_STATE_T *pPowerState);
extern	DTV_STATUS_T	DDI_MICOM_GetPowerOnMode (DDI_POWER_ON_MODE_T *pPowerOnMode);
extern	DTV_STATUS_T	DDI_MICOM_SetPowerOnMode (DDI_POWER_ON_MODE_T PowerOnMode);
extern	DTV_STATUS_T	DDI_MICOM_SetPowerOffMode( DDI_POWER_OFF_MODE_T tPowerOffMode );
extern	DTV_STATUS_T	DDI_MICOM_GetPowerOffMode( DDI_POWER_OFF_MODE_T * pPowerOffMode );
extern	DTV_STATUS_T	DDI_MICOM_GetPowerOffModeData( UINT8 index, UINT8 * pData );
extern	DTV_STATUS_T	DDI_MICOM_SendSystemPowerControlCommand (DDI_POWER_STATE_T powerState, int powerOnOffMode);
extern	DTV_STATUS_T	DDI_MICOM_SetAutoPowerOffOn(UINT8 rSecond,UINT8 powerOffMode);
extern	DTV_STATUS_T	DDI_MICOM_CancelPowerOff (void);
extern	DTV_STATUS_T	DDI_MICOM_SendHostReadyCommand (void);
extern	DTV_STATUS_T	DDI_MICOM_SendStartedApplication (void);
extern	DTV_STATUS_T	DDI_MICOM_SendHostSystemAliveSignal (void);

/*	MICOM's GPIO 		micom_gpio.c	*/
extern	DTV_STATUS_T	DDI_MICOM_GPIO_Init(void);
extern	DTV_STATUS_T	DDI_MICOM_GPIO_GetInputPort( UINT32 portIndex, UINT8 *pData );
extern	DTV_STATUS_T	DDI_MICOM_GPIO_SetOutputPort( UINT32 portIndex, UINT8 data );

/*	MICOM's etc 		micom_etc.c 	*/
extern	DTV_STATUS_T 	DDI_MICOM_SetPowerIndicator( DDI_MICOM_PWR_IND_CMD_T tCmd, UINT8 tVal);
extern	DTV_STATUS_T	DDI_MICOM_EnableTouchSensitivityMode( BOOLEAN bOnOff );
extern	DTV_STATUS_T	DDI_MICOM_SetTouchSensitivity( UINT8 tVal );
extern	DTV_STATUS_T	DDI_MICOM_GetTouchSensitivity( UINT8 *pVal );
extern	DTV_STATUS_T	DDI_MICOM_EnableRGBSensor( BOOLEAN bOnOff );

/*	MICOM's	RGB edid	micom_rgb_edid.c	*/
extern	DTV_STATUS_T	DDI_MICOM_ReadRGBEDID( UINT32 tIndex, UINT32 tSize, UINT8 *pRdData );
extern	DTV_STATUS_T	DDI_MICOM_WriteRGBEDID( UINT32 tIndex, UINT32 tSize, UINT8 *pWrData );


#endif /* #ifndef _MICOM_DDI_H_ */
