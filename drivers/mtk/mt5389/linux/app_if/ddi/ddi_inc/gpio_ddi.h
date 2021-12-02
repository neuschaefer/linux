/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file gpio_ddi.h
 *
 *  This header file declares the data types and DDI functions to control the gpio ports
 *
 *  @author		dhjung
 *  @version	0.1
 *  @date		2008.02.12
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _GPIO_DDI_H_
#define _GPIO_DDI_H_

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/


/** GPIO Group Index */
#define	_GPIO_GROUP_CMNIO				1
#define	_GPIO_GROUP_MICOM				2
#define	_GPIO_GROUP_IOEXP				3
#define	_GPIO_GROUP_MAX 				4
#define _GPIO_GROUP_SHIFT				12
#define _GPIO_GROUP_SET( x )			( ((x) & 0xF) << _GPIO_GROUP_SHIFT )
#define	GPIO_GROUP_GET( x ) 			( ((x) >> _GPIO_GROUP_SHIFT) & 0xF )

#define GPIO_GROUP_CMNIO				_GPIO_GROUP_SET( _GPIO_GROUP_CMNIO )
#define GPIO_GROUP_MICOM				_GPIO_GROUP_SET( _GPIO_GROUP_MICOM )
#define GPIO_GROUP_IOEXP				_GPIO_GROUP_SET( _GPIO_GROUP_IOEXP )

/**	GPIO Type(Direction) */
#define	_GPIO_TYPE_WR					1
#define _GPIO_TYPE_RD					2
#define _GPIO_TYPE_RW					(_GPIO_TYPE_WR | _GPIO_TYPE_RD)
#define _GPIO_TYPE_SHIFT				8
#define _GPIO_TYPE_SET( x )				( ((x) & 0xF) << _GPIO_TYPE_SHIFT )

#define	GPIO_TYPE_WR					_GPIO_TYPE_SET( _GPIO_TYPE_WR )
#define GPIO_TYPE_RD					_GPIO_TYPE_SET( _GPIO_TYPE_RD )
#define GPIO_TYPE_RW					_GPIO_TYPE_SET( _GPIO_TYPE_RW )

/** GPIO Index Type */
#define GPIO_CMNIO_WR  					(GPIO_GROUP_CMNIO + GPIO_TYPE_WR)
#define GPIO_CMNIO_RD  					(GPIO_GROUP_CMNIO + GPIO_TYPE_RD)
#define GPIO_CMNIO_RW  					(GPIO_GROUP_CMNIO + GPIO_TYPE_RW)
#define GPIO_MICOM_WR					(GPIO_GROUP_MICOM + GPIO_TYPE_WR)
#define GPIO_MICOM_RD					(GPIO_GROUP_MICOM + GPIO_TYPE_RD)
#define GPIO_MICOM_RW					(GPIO_GROUP_MICOM + GPIO_TYPE_RW)
#define GPIO_IOEXP_WR					(GPIO_GROUP_IOEXP + GPIO_TYPE_WR)
#define GPIO_IOEXP_RD					(GPIO_GROUP_IOEXP + GPIO_TYPE_RD)
#define GPIO_IOEXP_RW					(GPIO_GROUP_IOEXP + GPIO_TYPE_RW)


#define	GPIO_HDMI_SEL_NONE				0xff

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/**	GPIO Read functions */
typedef enum
{
	GPIO_RD_CHECK_CONNECT_HDMI	= 0,	///*	check connections of HDMI ports
	GPIO_RD_CHECK_CONNECT_S_VHS,		///*	check connections of S-VHS(S-VIDEO) ports
	GPIO_RD_CHECK_CONNECT_COMP,			///*	check connections of COMPonect ports
	GPIO_RD_CHECK_CONNECT_RGB,			///*	check connections of RGB ports
	GPIO_RD_CHECK_CONNECT_CVBS,			///*	check connections of CVBS(composite) ports
	GPIO_RD_CHECK_CONNECT_SCART,		///*	check connections of SCART ports
	GPIO_RD_CHECK_CONNECT_HP,			///*	check connections of Head Phone ports
	GPIO_RD_CHECK_SHORT_ANTENNA_5V,		///*	check shortage of ANTENNA 5V
	GPIO_RD_CHECK_DISPLAY_TYPE,			///*	check DIAPLAY TYPE(LCD or PDP)
	GPIO_RD_CHECK_MODULE_TYPE,			///*	check MODULE TYPE(FHD/WXGA/XGA)
	GPIO_RD_CHECK_NUM_OF_HDMI,			///*	check number of HDMI
	GPIO_RD_CHECK_FAN_ALARM,			///*	check FAN alram
	GPIO_RD_GET_DSUB_SDA,				///*	get level에 DSUB SDA

}	GPIO_FN_RD_TYPE_T;



/**	GPIO Write functions */
typedef enum
{
	GPIO_WR_RESET_TUNER			= 0,
	GPIO_WR_RESET_AUDIO,
	GPIO_WR_ENABLE_CI,
//	GPIO_WR_ENABLE_LGS8G13,
//	GPIO_WR_ENABLE_HD2812,
	GPIO_WR_ENABLE_AGC_SPEED,
	GPIO_WR_ENABLE_ANTENNA_BOOSTER,
	GPIO_WR_ENABLE_ANTENNA_5V,
	GPIO_WR_SELECT_SCART_8_PIN,
	GPIO_WR_SELECT_HDMI_HOT_PLUG,
	GPIO_WR_SELECT_HDMI,
	GPIO_WR_SELECT_MNT_V_OUT,
//	GPIO_WR_SELECT_LCD_BIT,
#if (PLATFORM_TYPE == BCM_PLATFORM)
	GPIO_WR_RESET_AUDIO_AMP,
#else
	GPIO_WR_RESET_EYEQ,
#endif
	GPIO_WR_RESET_FRC,
	GPIO_WR_ENABLE_FRC_RESET,
	GPIO_WR_ENABLE_FRC_1_3PLL,
	GPIO_WR_ENABLE_FRC_1_8VF,
	GPIO_WR_ENABLE_FRC_1_3VF,
	GPIO_WR_ENABLE_FRC_2_5VF,
	GPIO_WR_ENABLE_FRC_3_3VF,
	GPIO_WR_SET_DSUB_SCL,
	GPIO_WR_SET_DSUB_SDA,
	GPIO_WR_ENABLE_EMP_MODE,
	GPIO_WR_MUTE_AUDIO_MNT_OUT,
	GPIO_WR_MUTE_AUDIO_TV_OUT,
	GPIO_WR_MUTE_AUDIO_AMP,
	GPIO_WR_MUTE_AUDIO_HP,			///*	mute audio Head Phone
	GPIO_WR_ENABLE_LCD_DISPLAY,
	GPIO_WR_ENABLE_PDP_DISPLAY,
	GPIO_WR_ENABLE_AI,				// 필요한지 확인 요
	GPIO_WR_ENABLE_MODULE_PORT,
	GPIO_WR_ENABLE_LCD_POWER,		///*	enable LCD Display panel power
	GPIO_WR_ENABLE_WRITE_PROTECT,
	GPIO_WR_ENABLE_RGB_EDID_WRITE_PROTECT,
	GPIO_WR_CONTROL_PDP_VAVS,
	GPIO_WR_CONTROL_PDP_RELAY,
	GPIO_WR_CONTROL_VIDEO_SWITCH,
	GPIO_WR_ENABLE_BT,           	///*	enable BT(BluetTooth)
	GPIO_WR_ENABLE_OPC,          	///*	enable OPC(Optimal Power Control)
	GPIO_WR_SELECT_BT_AUDIO_SWITCH,      	///*	select HP / BT MONO switch
	GPIO_WR_ENBALE_LED_DEMO,		///* enable LED backlight Demo Mode

} GPIO_FN_WR_TYPE_T;

/**	SCART PIN #8 CONTROL type */
typedef enum {

	SCART_PIN8_RF					= 0,
	SCART_PIN8_4_3					= 1,
	SCART_PIN8_16_9					= 2,
	SCART_PIN8_NOCTRL				= 0xff

} SCART_PIN8_CTRL_T;


/**	GPIO GET FUNCTION POINTER TYPE */
typedef DTV_STATUS_T	(*GPIO_GET_FUNC_PTR_T)( UINT32, UINT8* );
/**	GPIO SET FUNCTION POINTER TYPE */
typedef DTV_STATUS_T	(*GPIO_SET_FUNC_PTR_T)( UINT32, UINT8  );

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T DDI_GPIO_Init(void);
extern DTV_STATUS_T DDI_GPIO_DynamicPortAlloc(void);
extern DTV_STATUS_T DDI_GPIO_GetControl(UINT8 gpioFnType, UINT8 ctrl1, UINT8 ctrl2, UINT8 *pData);
extern DTV_STATUS_T DDI_GPIO_SetControl(UINT8 gpioFnType, UINT8 ctrl1, UINT8 ctrl2);
extern void 		DDI_GPIO_Debug(void);

#endif  /* _GPIO_DDI_H_ */
