/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file i2c_ddi.h
 *
 *	Definition of I2C channel mapping for drivers
 *
 *	@author 	dhjung
 *	@author 	bk1472 - modifiler
 *	@version	0.1
 *	@date		2008.02.12
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _I2C_DDI_H_
#define _I2C_DDI_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "cmnio_type.h"
#include "cmnio_ddi.h"

/******************************************************************************
	상수 정의(Constant Definitions)
******************************************************************************/

/*	I2C	MAX CH NUMBER	*/
#if 	(PLATFORM_TYPE == GNSS_PLATFORM)
#define	I2C_PORT_MAX_NUM		2
#undef	INCLUDE_I2C_MUTEX
#undef	INCLUDE_I2C_SLOW_MODE
#define I2C_ADDR_NVM_EDID		0xA0
#elif	(PLATFORM_TYPE == MSTAR_PLATFORM)
#define	I2C_PORT_MAX_NUM		6
#define	INCLUDE_I2C_MUTEX
#undef	INCLUDE_I2C_SLOW_MODE
#elif	(PLATFORM_TYPE == BCM_PLATFORM)
#define	I2C_PORT_MAX_NUM		4
#undef	INCLUDE_I2C_MUTEX
#define	INCLUDE_I2C_SLOW_MODE
#elif	(PLATFORM_TYPE == MTK_PLATFORM)
#define	I2C_PORT_MAX_NUM		2
#undef	INCLUDE_I2C_MUTEX
#define	INCLUDE_I2C_SLOW_MODE
#else
#error "Platform is not defined!"
#endif

/*	I2C CH#	for NOT USED	*/
#define	I2C_PORT_NOT_USED		((UINT8)(-1))

/******************************************************************************
	매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/************************************************************************/
/*  I2C ADDRESS Definition												*/
/************************************************************************/
#define	I2C_TEA6420D_ID			0x98	//	Audio Switch for Venus
/*	default retries */
#define IIC_DEFAULT_RETRIES		3


#define I2C_NM_NVM							"NVM"
#define I2C_NM_NVM_EDID						"EDID"
#define I2C_NM_MSP4458						"MSP4458"
#define I2C_NM_NTP3000						"NTP3000"
#define I2C_NM_NTP3100						"NTP3100"
#define I2C_NM_CS5345						"CS5345"
#define I2C_NM_TEA6420D						"TEA6420D"
#define I2C_NM_TAS5707						"TAS5707"
#define I2C_NM_MICOM						"MICOM"
#define I2C_NM_FRC_MST7323S					"FRCMST7323S"
#define I2C_NM_FRC_SPIFLASH					"FRCSPIFLASH"
#define I2C_NM_FRC_9459M					"FRC9495M"
#define I2C_NM_IO_EXP						"IO_EXP"
#define I2C_NM_HDMI_PS421					"HDMIPS421"
#define I2C_NM_HDMI_PS421_EDID				"HDMIPS421EDID"
#define I2C_NM_RGB_SENSOR					"RGBSENSOR"
#define I2C_NM_SNSR_TEMP					"SNSR_TEMP"
#define I2C_NM_FAN							"FAN"
#define I2C_NM_PDP_MOD						"PDP_MOD"
#define I2C_NM_EYE_Q2						"EYEQ2"
#define I2C_NM_GREEN_EYE					"GREEN_EYE"
#define I2C_NM_A_TU							"TUA"
#define I2C_NM_D_TU							"TUD"
#define I2C_NM_HDMI_TDA9996					"HDMITDA9996"
#define I2C_NM_HDCP							"HDCP"
#define I2C_NM_HDMI_SIL9287					"HDMISil9287"
#define I2C_NM_HDMI_SIL9287_EDID			"HDMISil9287EDID"
#define I2C_NM_HDMI_SIL9287_TMDX			"HDMISil9287TMDX"
#define I2C_NM_HDMI_SIL9287_CORE			"HDMISil9287CORE"
#define I2C_NM_HDMI_SIL9287_MHL				"HDMISil9287MHL"
#define I2C_NM_STA339						"STA339"
#define I2C_NM_NONE							NULL

CMN_VAR(I2C_DESC_T*, I2C_DEV_NVM,               NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NVM_EDID,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MSP4458,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3000,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3100,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_CS5345,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TEA6420D,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS5707,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MICOM,             NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_MST7323S,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_SPIFLASH,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_9459M,         NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_IO_EXP,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_PS421,        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_PS421_EDID,   NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_RGB_SENSOR,        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_SNSR_TEMP,         NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FAN,               NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_PDP_MOD,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_EYE_Q2,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_GREEN_EYE,         NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_A_TU,              NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_D_TU,              NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_TDA9996,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDCP,              NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_EDID, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_TMDX, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_CORE, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_MHL,  NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_STA339,            NULL);


#ifdef _CMN_VAR_CONF_C_
I2C_DESC_T	gI2cDescTable[I2C_DEV_MAX_CNT];
char*		gI2cNameTable[] =
{
	I2C_NM_NVM				,
	I2C_NM_NVM_EDID			,
	I2C_NM_MSP4458			,
	I2C_NM_NTP3000			,
	I2C_NM_NTP3100			,
	I2C_NM_CS5345			,
	I2C_NM_TEA6420D			,
	I2C_NM_TAS5707			,
	I2C_NM_MICOM			,
	I2C_NM_FRC_MST7323S		,
	I2C_NM_FRC_SPIFLASH 	,
	I2C_NM_FRC_9459M		,
	I2C_NM_IO_EXP			,
	I2C_NM_HDMI_PS421		,
	I2C_NM_HDMI_PS421_EDID	,
	I2C_NM_RGB_SENSOR		,
	I2C_NM_SNSR_TEMP		,
	I2C_NM_FAN				,
	I2C_NM_PDP_MOD			,
	I2C_NM_EYE_Q2			,
	I2C_NM_GREEN_EYE		,
	I2C_NM_A_TU             ,
	I2C_NM_D_TU             ,
	I2C_NM_HDMI_TDA9996	    ,
	I2C_NM_HDCP			    ,
	I2C_NM_HDMI_SIL9287		,
	I2C_NM_HDMI_SIL9287_EDID,
	I2C_NM_HDMI_SIL9287_TMDX,
	I2C_NM_HDMI_SIL9287_CORE,
	I2C_NM_HDMI_SIL9287_MHL ,
	I2C_NM_STA339 			,
	I2C_NM_NONE
};
#else
extern I2C_DESC_T	gI2cDescTable[];
extern char*		gI2cNameTable[];
#endif

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T	DDI_I2C_Init     ( void );
extern DTV_STATUS_T	DDI_I2C_Lock    ( I2C_DESC_T *pPort, BOOLEAN	isLocked );
extern DTV_STATUS_T	DDI_I2C_Read    ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			   	  UINT16 nBytes , UINT8 *pRxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_Write   ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			      UINT16 nBytes , UINT8 *pTxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_FastRead( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			      UINT16 nBytes , UINT8 *pRxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_FastWrite(I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
									  UINT16 nBytes , UINT8 *pTxBuf   , UINT8 retry);
#ifdef	INCLUDE_I2C_SLOW_MODE
extern DTV_STATUS_T	DDI_I2C_SlowRead (I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					   			   	  UINT16 nBytes , UINT8 *pRxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_SlowWrite(I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
									  UINT16 nBytes , UINT8 *pTxBuf   , UINT8 retry);
#endif	/* #ifdef	INCLUDE_I2C_SLOW_MODE */

extern int			I2C_CommonIO_Dev_Init( void );
extern I2C_DESC_T	*CONF_Add_I2c_Device(char *pName, UINT32 portNum, UINT32 addr);
extern void			DDI_I2C_Debug(void);

#ifdef __cplusplus
}
#endif

#endif /* _I2C_DDI_H_ */
