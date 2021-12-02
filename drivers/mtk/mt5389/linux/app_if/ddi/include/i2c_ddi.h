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
 *	@author 	dhjung(dhjung77@lge.com)
 *	@author 	bk1472(bk1472@lge.com) - modifiler
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
#if	(PLATFORM_TYPE == MSTAR_PLATFORM)
#define	I2C_PORT_MAX_NUM		12
#define	INCLUDE_I2C_MUTEX
//#undef	INCLUDE_I2C_SLOW_MODE
#define	INCLUDE_I2C_SLOW_MODE
#define SW_I2C_START


#elif	(PLATFORM_TYPE == BCM_PLATFORM)
#define	I2C_PORT_MAX_NUM		6	/* H/W: 3 + S/W: 3 ( add more one channel for pdp ) */
#define SW_I2C_MUTEX			3
#define	INCLUDE_I2C_MUTEX
//#undef	INCLUDE_I2C_MUTEX
//#define	INCLUDE_I2C_SLOW_MODE//GP3에서는 Normal Mode 사용.

#elif	(PLATFORM_TYPE == LG_PLATFORM)
/* [L8] ingyu.yang - L8 configuration */
#define	I2C_PORT_MAX_NUM		7   /* H/W 4 + S/W 7(rgb + chb + tu + hdmi*4)*/
#define SW_I2C_MUTEX			4
#define	INCLUDE_I2C_MUTEX
#define	INCLUDE_I2C_SLOW_MODE

#elif	(PLATFORM_TYPE == MTK_PLATFORM)
#define	I2C_PORT_MAX_NUM	    11		/* H/W: 3 + S/W: 3 ( add more one channel for pdp ) */
#define SW_I2C_MUTEX			3
#define	INCLUDE_I2C_MUTEX

// javamaze
#elif	(PLATFORM_TYPE == X86_PLATFORM)
#define	I2C_PORT_MAX_NUM		4
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
#define I2C_NM_RGB_EDID						"RGB_EDID"  //ieeum_rgb
#define I2C_NM_MSP4458						"MSP4458"
#if 0
#define I2C_NM_NTP3000						"NTP3000"
#define I2C_NM_NTP3100						"NTP3100"
#endif
#define I2C_NM_NTP7000						"NTP7000"
#define I2C_NM_NTP7400						"NTP7400"
#if 0
#define I2C_NM_NTP3AMP						"NTP3AMP"
#define I2C_NM_NTP3AMP_1					"NTP3AMP_1"
#endif
#define I2C_NM_NTPSNDBAR_M					"NTPSNDBAR_M"
#define I2C_NM_NTPSNDBAR_S					"NTPSNDBAR_S"
#define I2C_NM_NTP2AMP_M					"NTP2AMP_M"
#define I2C_NM_NTP2AMP_W					"NTP2AMP_W"
#define I2C_NM_CS5345						"CS5345"
#define I2C_NM_TEA6420D						"TEA6420D"
#define I2C_NM_TAS5709						"TAS5709"
#define I2C_NM_TAS5713						"TAS5713"
#define I2C_NM_TAS5727						"TAS5727"
#define I2C_NM_TAS2AMP_M					"TAS2AMP_M"
#define I2C_NM_TAS2AMP_W					"TAS2AMP_W"
#define I2C_NM_TASSNDBAR_M					"TASSNDBAR_M"
#define I2C_NM_TASSNDBAR_S					"TASSNDBAR_S"
#define I2C_NM_STA368						"STA368"
#define I2C_NM_MICOM						"MICOM"
#define I2C_NM_LG5111						"LG5111"
#define I2C_NM_FRC_MST7323S					"FRCMST7323S"
#define I2C_NM_FRC_MST7398Q					"FRCMST7398Q" //ursa4 youngkil.kim

#define I2C_NM_FRC_SPIFLASH					"FRCSPIFLASH"
#define I2C_NM_FRC_URSA5DDC					"FRCURSA5DDC" //ursa5 DDC2BI
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
#define	I2C_NM_S_TU							"TUS" // ieeum.lee work1
#define	I2C_NM_LNB_TU						"TU_LNB"
#define I2C_NM_HDMI_TDA19997					"HDMITDA19997"
#define I2C_NM_HDCP							"HDCP"
#define I2C_NM_HDMI_SIL9287					"HDMISil9287"
#define I2C_NM_HDMI_SIL9287_EDID			"HDMISil9287EDID"
#define I2C_NM_HDMI_SIL9287_TMDX			"HDMISil9287TMDX"
#define I2C_NM_HDMI_SIL9287_CORE			"HDMISil9287CORE"
#define I2C_NM_HDMI_SIL9287_MHL				"HDMISil9287MHL"

#define I2C_NM_HDMI_SIL9587_B0			"HDMISIL9587_B0"
#define I2C_NM_HDMI_SIL9587_66			"HDMISIL9587_66"
#define I2C_NM_HDMI_SIL9587_68			"HDMISIL9587_68"
#define I2C_NM_HDMI_SIL9587_50			"HDMISIL9587_50"
#define I2C_NM_HDMI_SIL9587_52			"HDMISIL9587_52"
#define I2C_NM_HDMI_SIL9587_54			"HDMISIL9587_54"
#define I2C_NM_HDMI_SIL9587_C0			"HDMISIL9587_C0"
#define I2C_NM_HDMI_SIL9587_E0			"HDMISIL9587_E0"
#define I2C_NM_HDMI_SIL9587_64			"HDMISIL9587_64"
#define I2C_NM_HDMI_SIL9587_90			"HDMISIL9587_90"
#define I2C_NM_HDMI_SIL9587_E6			"HDMISIL9587_E6"
#define I2C_NM_HDMI_SIL9587_D0			"HDMISIL9587_D0"
#define I2C_NM_HDMI_SIL9587_E8			"HDMISIL9587_E0"
#define I2C_NM_HDMI_SIL9587_72			"HDMISIL9587_72"

#define I2C_NM_STA339						"STA339"
#define I2C_NM_BUF16821						"BUF16821"
#define I2C_NM_HDMI_A_EDID					"HDMI_A_EDID"
#define I2C_NM_HDMI_B_EDID					"HDMI_B_EDID"
#define I2C_NM_HDMI_C_EDID					"HDMI_C_EDID"
#define I2C_NM_HDMI_D_EDID					"HDMI_D_EDID"
#define I2C_NM_FRC_LG1120					"LG1120"
#define I2C_NM_FRC_LG1122					"LG1122"
#define I2C_NM_SHARP_TCON					"SHARP_TCON"
#define I2C_NM_MAX9668						"MAX9668"
#define I2C_NM_C2DTO3D_LG1131					"LG1131"
#define I2C_NM_MAX9668B						"MAX9668B"
#define I2C_NM_MAX17139						"MAX17139"
#define I2C_NM_LG5811						"LG5811"
#define I2C_NM_WIRELESS_DONGLE				"WIRELESS_DONGLE"
#define I2C_NM_NONE							NULL

#define I2C_NM_TRIDFMT						"TRIDFMT"
#define I2C_NM_TCON_ODC						"TCON_ODC"
#define I2C_NM_TCON_ODC_LUT_L				"TCON_ODC_LUT_L"
#define I2C_NM_TCON_ODC_LUT_H				"TCON_ODC_LUT_H"
#define I2C_NM_NVM_MONITOR 				"MONITOR_NVM" //balup_lex9

#define	I2C_NM_DP_TX							"DP_TX"
#define	I2C_NM_DP_RX							"DP_RX"
#define	I2C_NM_DP_TX_H							"DP_TX_H"
#define	I2C_NM_DP_RX_H							"DP_RX_H"
#define	I2C_NM_LG8300_IND						"LG8300_IND"
#define	I2C_NM_LG8300_DIR						"LG8300_DIR"
#define	I2C_NM_PDP_MOD_2ND						"PDP_MOD_2ND"

#if (PLATFORM_TYPE == LG_PLATFORM)
/* [L8] ingyu.yang - BEGIN : add L8 related variable */
#define I2C_NM_NTP7000_1					"NTP7000_1"
#define	I2C_NM_FRCII						"FRCII"
#define	I2C_NM_CHB_TUA						"CHB_TUA"
#define	I2C_NM_CHB_TUD						"CHB_TUD"
#endif




CMN_VAR(I2C_DESC_T*, I2C_DEV_NVM,               NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_RGB_EDID,          NULL); //ieeum_rgb
CMN_VAR(I2C_DESC_T*, I2C_DEV_MSP4458,           NULL);
#if 0
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3000,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3100,           NULL);
#endif
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP7000,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP7400,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_STA368,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_CS5345,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TEA6420D,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS5709,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS5727,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS5713,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MICOM,             NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_MST7323S,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_MST7398Q,      NULL); //ursa4 youngkil.kim
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_SPIFLASH,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_URSA5DDC,		NULL);
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
CMN_VAR(I2C_DESC_T*, I2C_DEV_S_TU,              NULL);  // ieeum.lee work1
CMN_VAR(I2C_DESC_T*, I2C_DEV_LNB_TU,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_TDA19997,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDCP,              NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_EDID, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_TMDX, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_CORE, NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9287_MHL,  NULL);

CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_B0,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_66,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_68,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_50,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_52,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_54,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_C0,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_E0,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_64,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_90,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_E6,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_D0,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_E8,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_SIL9587_72,      NULL);

CMN_VAR(I2C_DESC_T*, I2C_DEV_STA339,            NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_BUF16821,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_A_EDID,       NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_B_EDID,       NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_C_EDID,       NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_HDMI_D_EDID,       NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_LG5111,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_LG1120,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRC_LG1122,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_SHARP_TCON,      NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_C2DTO3D_LG1131,  NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MAX9668,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MAX9668B,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_MAX17139,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_LG5811,          NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_WIRELESS_DONGLE,          NULL);
#if 0
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3AMP,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP3AMP_1,           NULL);
#endif
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTPSNDBAR_M,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTPSNDBAR_S,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP2AMP_M,	        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP2AMP_W,           NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS2AMP_M,		        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TAS2AMP_W,		        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TASSNDBAR_M,	        NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TASSNDBAR_S,	        NULL);


CMN_VAR(I2C_DESC_T*, I2C_DEV_TRIDFMT,   	     NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TCON_ODC,   	     NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TCON_ODC_LUT_L,   	     NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_TCON_ODC_LUT_H,   	     NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_NVM_MONITOR,   	     NULL);

CMN_VAR(I2C_DESC_T*, I2C_DEV_DP_TX,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_DP_RX,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_DP_TX_H,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_DP_RX_H,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_LG8300_IND,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_LG8300_DIR,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_PDP_MOD_2ND,           NULL);

#if (PLATFORM_TYPE == LG_PLATFORM)
CMN_VAR(I2C_DESC_T*, I2C_DEV_NTP7000_1,			NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_FRCII,				NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_CHB_TUA,			NULL);
CMN_VAR(I2C_DESC_T*, I2C_DEV_CHB_TUD,			NULL);
#endif


#ifdef _CMN_VAR_CONF_C_
I2C_DESC_T	*gpI2cDescTable = NULL;
char*		gI2cNameTable[] =
{
	I2C_NM_NVM				,           /* 01 */
	I2C_NM_RGB_EDID			,  //ieeum_rgb
	I2C_NM_MSP4458			,
//	I2C_NM_NTP3000			,
//	I2C_NM_NTP3100			,
	I2C_NM_NTP7000			,
	I2C_NM_NTP7400			,
	I2C_NM_CS5345			,
	I2C_NM_TEA6420D			,
	I2C_NM_TAS5709			,
	I2C_NM_TAS5713			,
	I2C_NM_TAS5727			,
	I2C_NM_STA368			,
	I2C_NM_MICOM			,		/* 10 */
	I2C_NM_FRC_MST7323S		,
	I2C_NM_FRC_MST7398Q		, //usra4
	I2C_NM_FRC_SPIFLASH 	,
	I2C_NM_FRC_URSA5DDC		,
	I2C_NM_FRC_9459M		,
	I2C_NM_IO_EXP			,
	I2C_NM_HDMI_PS421		,
	I2C_NM_HDMI_PS421_EDID	,
	I2C_NM_RGB_SENSOR		,
	I2C_NM_SNSR_TEMP		,
	I2C_NM_FAN				,
	I2C_NM_PDP_MOD			,		/* 20 */
	I2C_NM_EYE_Q2			,
	I2C_NM_GREEN_EYE		,
	I2C_NM_A_TU             ,
	I2C_NM_D_TU             ,
	I2C_NM_S_TU             ,
	I2C_NM_LNB_TU           ,
	I2C_NM_HDMI_TDA19997    ,
	I2C_NM_HDCP			    ,
	I2C_NM_HDMI_SIL9287		,
	I2C_NM_HDMI_SIL9287_EDID,
	I2C_NM_HDMI_SIL9287_TMDX,
	I2C_NM_HDMI_SIL9287_CORE,		/* 30 */
	I2C_NM_HDMI_SIL9287_MHL ,

	I2C_NM_HDMI_SIL9587_B0	,
	I2C_NM_HDMI_SIL9587_66	,
	I2C_NM_HDMI_SIL9587_68	,
	I2C_NM_HDMI_SIL9587_50	,
	I2C_NM_HDMI_SIL9587_52	,
	I2C_NM_HDMI_SIL9587_54	,
	I2C_NM_HDMI_SIL9587_C0	,
	I2C_NM_HDMI_SIL9587_E0	,
	I2C_NM_HDMI_SIL9587_64	,
	I2C_NM_HDMI_SIL9587_90	,
	I2C_NM_HDMI_SIL9587_E6	,
	I2C_NM_HDMI_SIL9587_D0	,
	I2C_NM_HDMI_SIL9587_E8	,
	I2C_NM_HDMI_SIL9587_72	,
	
	I2C_NM_STA339			,
	I2C_NM_BUF16821 		,
	I2C_NM_HDMI_A_EDID		,
	I2C_NM_HDMI_B_EDID		,
	I2C_NM_HDMI_C_EDID		,			/* 36   -- Total count */
	I2C_NM_HDMI_D_EDID		,			/* 37   -- Total count */
	I2C_NM_LG5111			,			/* 38 */ /* LG5110 */
	I2C_NM_FRC_LG1120		,			/* 39 */ /* LG51121 */
	I2C_NM_FRC_LG1122		,
	I2C_NM_SHARP_TCON		,
	I2C_NM_C2DTO3D_LG1131	,			/*2Dto3D */
	I2C_NM_MAX9668	 		,
	I2C_NM_MAX9668B			,
	I2C_NM_MAX17139			,
	I2C_NM_LG5811			,
	I2C_NM_WIRELESS_DONGLE ,
//	I2C_NM_NTP3AMP			,
//	I2C_NM_NTP3AMP_1		,
	I2C_NM_NTPSNDBAR_M		,
	I2C_NM_NTPSNDBAR_S		,
	I2C_NM_NTP2AMP_M		,
	I2C_NM_NTP2AMP_W		,
	I2C_NM_TAS2AMP_M		,
	I2C_NM_TAS2AMP_W		,
	I2C_NM_TASSNDBAR_M		,
	I2C_NM_TASSNDBAR_S		,

	I2C_NM_TRIDFMT			,			/* 42 */ /* TRIFMT */
	I2C_NM_TCON_ODC			,			/* 43 */ /* TCON ODC */
	I2C_NM_TCON_ODC_LUT_L	,			/* 44 */ /* TCON ODC Low LUT */
	I2C_NM_TCON_ODC_LUT_H	,			/* 45 */ /* TCON ODC High LUT */
	I2C_NM_NVM_MONITOR	,				/*46 */ /* panel nvm */

	I2C_NM_DP_TX			,
	I2C_NM_DP_RX			,
	I2C_NM_DP_TX_H			,
	I2C_NM_DP_RX_H			,
	I2C_NM_LG8300_IND		,
	I2C_NM_LG8300_DIR		,
#if (PLATFORM_TYPE == LG_PLATFORM)
	I2C_NM_FRCII			,
	I2C_NM_NTP7000_1		,
	I2C_NM_CHB_TUA			,
	I2C_NM_CHB_TUD			,
#endif
	I2C_NM_PDP_MOD_2ND		,
	I2C_NM_NONE
};
int gI2cDevMaxCnt = sizeof(gI2cNameTable)/sizeof(char*);
#else
extern I2C_DESC_T	*gpI2cDescTable;
extern char*		gI2cNameTable[];
extern int			gI2cDevMaxCnt;
#endif

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T	DDI_I2C_Init     ( void );
extern API_STATE_T DDI_I2C_HW_Control (BOOLEAN enable); //balup_BBS
extern DTV_STATUS_T	DDI_I2C_Lock    ( I2C_DESC_T *pPort, BOOLEAN	isLocked );
extern DTV_STATUS_T	DDI_I2C_Read    ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			   	  UINT16 nBytes , UINT8 *pRxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_Write   ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			      UINT16 nBytes , UINT8 *pTxBuf   , UINT8 retry);

#if 0
extern DTV_STATUS_T	DDI_I2C_Read    ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
					  			   	  UINT16 nBytes , UINT8 *pRxBuf   , UINT8 retry);
extern DTV_STATUS_T	DDI_I2C_NormalWrite   ( I2C_DESC_T *pPort, UINT8 subAddrMode, UINT8 *pSubAddr,
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
#endif

extern DTV_STATUS_T	DDI_I2C_InitCommonIODev( void );
extern I2C_DESC_T	*DDI_I2C_AddDevice(char *pName, UINT32 portNum, UINT32 addr);
extern void			DDI_I2C_Debug(void);

#ifdef __cplusplus
}
#endif

#endif /* _I2C_DDI_H_ */
