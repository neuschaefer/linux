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
 *  @author		dhjung(dhjung77@lge.com)
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

//balup_090812
/*
	<GP2 model option> T3(S7) Part
	OPTION0 [GPIO42][G19]: FRC Support (LOW: not support FRC, HIGH: Support FRC)
	OPTION1 [GPIO11][C5] : LVDS type   (LOW: LVDS 			, HIGH: Mini LVDS)
	OPTION2 [GPIO14][F7] : DDR size	   (LOW: DDR 256MB      , HIGH: DDR 512MB)
	OPTION3 [GPIO32][B6] : Panel Res.  (LOW: HD(1366x768)   , HIGH: FHD(1920x1080)
*/


/**	GPIO Read functions */
typedef enum
{
	GPIO_RD_CHECK_CONNECT_HDMI	= 0,		///*	check connections of HDMI ports
	GPIO_RD_CHECK_CONNECT_S_VHS,		///*	check connections of S-VHS(S-VIDEO) ports
	GPIO_RD_CHECK_CONNECT_COMP,		///*	check connections of COMPonect ports
	GPIO_RD_CHECK_CONNECT_RGB,			///*	check connections of RGB ports
	GPIO_RD_CHECK_CONNECT_CVBS,		///*	check connections of CVBS(composite) ports
	GPIO_RD_CHECK_CONNECT_SCART,		///*	check connections of SCART ports
	GPIO_RD_CHECK_CONNECT_HP,			///*	check connections of Head Phone ports
	GPIO_RD_CHECK_SHORT_ANTENNA_5V,	///*	check shortage of ANTENNA 5V
	GPIO_RD_CHECK_FRC_SUPPORTED,		///*	check FRC SUPPORTED(TRUE/FALSE)
	GPIO_RD_CHECK_PDP_HD_TYPE,		///*	check PDP XGA/WXGA (TRUE/FALSE)
	GPIO_RD_CHECK_MODULE_TYPE,			///*    check MODULE TYPE(FHD/HD)
	GPIO_RD_CHECK_LVDS_TYPE,			///*    check LVDS TYPE(LVDS/Mini LVDS)
	GPIO_RD_CHECK_FRC1_SUPPORTED,		///*    check FRC support 1
	GPIO_RD_CHECK_GIP_SUPPORTED,		///*    check GIP Panel support
	GPIO_RD_CHECK_OLED,					///*    check OLED/LCD
	GPIO_RD_CHECK_T2_TUNER, 			///*	   check DVB-T2 TUNER
	GPIO_RD_CHECK_S_TUNER,				///*    check DVB-S TUNER
	GPIO_RD_CHECK_OPTIC,				///*    check Optic		
	GPIO_RD_CHECK_CPBOX,				///*    check CP-BOX	
	GPIO_RD_CHECK_3D_DEPTH_IC,			///*    check 3D Depth IC	
	GPIO_RD_CHECK_DDR_SIZE,				///*    check DDR SIZE (768MB or not)
	GPIO_RD_CHECK_EPI,					///*    check EPI		
//	GPIO_RD_CHECK_MHP,					///*    check MHP
	GPIO_RD_CHECK_USB1_OCD,				///*    check usb OCD
	GPIO_RD_CHECK_NUM_OF_HDMI,			///*	check number of HDMI
	GPIO_RD_CHECK_FAN_ALARM,			///*	check FAN alram
	GPIO_RD_CHECK_ERRPANEL_ON,			///*      check Panel error pin
	GPIO_RD_CHECK_MICOM_HWPORT,			///*  check micom hw model options  //balup_090908
	GPIO_RD_FMTUP_DATA,			// FMTUP
	GPIO_RD_M_RFMODULE_DD,				///* RFREMOTE update
#if (PLATFORM_TYPE == LG_PLATFORM)
	GPIO_RD_CHECK_REAR_USB_OCD,
#endif
	GPIO_RD_END
}	GPIO_FN_RD_TYPE_T;



/**	GPIO Write functions */
typedef enum
{
	GPIO_WR_RESET_DEMOD			= 0,
	GPIO_WR_RESET_AUDIO,
	GPIO_WR_ENABLE_CI,
	GPIO_WR_ENABLE_AGC_SPEED,
	GPIO_WR_ENABLE_ANTENNA_BOOSTER,
	GPIO_WR_ENABLE_ANTENNA_5V,
	GPIO_WR_SELECT_SCART_8_PIN,
	GPIO_WR_SELECT_HDMI_HOT_PLUG,
	GPIO_WR_SELECT_HDMI,
	GPIO_WR_SELECT_MNT_V_OUT,
	GPIO_WR_RESET_EYEQ,
	GPIO_WR_RESET_FRC,
	GPIO_WR_ENABLE_FRC_RESET,
	GPIO_WR_ENABLE_FRC_1_3PLL,
	GPIO_WR_ENABLE_FRC_1_8VF,
	GPIO_WR_ENABLE_FRC_1_3VF,
	GPIO_WR_ENABLE_FRC_2_5VF,
	GPIO_WR_ENABLE_FRC_3_3VF,
	GPIO_WR_ENABLE_EMP_MODE,
	GPIO_WR_MUTE_AUDIO_MNT_OUT,		///*	mute audio MNT out(SC2)
	GPIO_WR_MUTE_AUDIO_TV_OUT,		///*	mute audio TV out(SC1)
	GPIO_WR_MUTE_AUDIO_AMP,
	GPIO_WR_MUTE_AUDIO_HP,			///*	mute audio Head Phone
	GPIO_WR_MUTE_VIDEO_TV_OUT,		///* 	mute video TV out(SC1)
	GPIO_WR_ENABLE_LCD_DISPLAY,
	GPIO_WR_ENABLE_PDP_DISPLAY,
	GPIO_WR_ENABLE_AI,				// 필요한지 확인 요
	GPIO_WR_ENABLE_MODULE_PORT,
	GPIO_WR_ENABLE_LCD_POWER,	///*	enable LCD Display panel power
	GPIO_WR_ENABLE_WRITE_PROTECT,
	GPIO_WR_ENABLE_RGB_EDID_WRITE_PROTECT,
	GPIO_WR_CONTROL_PDP_VAVS,
	GPIO_WR_CONTROL_PDP_5VONOFF,
	GPIO_WR_CONTROL_PDP_LED_W1,
	GPIO_WR_CONTROL_PDP_LED_W2,
	GPIO_WR_CONTROL_PDP_RELAY,
	GPIO_WR_CONTROL_VIDEO_SWITCH,
	GPIO_WR_ENABLE_BT,           	///*	enable BT(BluetTooth)
	GPIO_WR_RESET_BT,				///*	reset BT(BlueTooth)
	GPIO_WR_VREGCTRL_BT, // streamerj 091222
	GPIO_WR_ENABLE_OPC,          	///*	enable OPC(Optimal Power Control)
	GPIO_WR_SELECT_BT_AUDIO_SWITCH,      	///*	select HP / BT MONO switch
	GPIO_WR_ENBALE_LED_DEMO,		///* enable LED backlight Demo Mode
	GPIO_WR_ENABLE_SERIAL_TS_BUFFER,		///* TS buffer control for PDP DVB only
	GPIO_WR_ENABLE_POWEREN,			// enable power en for LCD ATSC only
	GPIO_WR_ENABLE_LED_G, 			// control LED_G
	GPIO_WR_ENABLE_LED_R,			// control LED_R
	GPIO_WR_ENABLE_LNA2, 			// control LED_G
	GPIO_WR_ENABLE_RF_SWITCH,			// control LED_R
	GPIO_WR_ENABLE_USB_CTRL,			// control USB_CTL
	GPIO_WR_RESET_USB_HUB,			// Reset USB HUB
	GPIO_WR_TUNER_RESET, 			// XC5000 Reset
	GPIO_WR_STUNER_RESET,			// DVB-S tuner reset
	GPIO_WR_LVDS_SELECT, 			// control LVDS SELECT
	GPIO_WR_LED_LOCALDIMMING_RESET, 			// control LED Local Dimming IC Reset
	GPIO_WR_WIRELES_READY_SWITCH,
	GPIO_WR_WIRELES_READY_POWER, //balup
	GPIO_WR_DPM, //balup_090907
	GPIO_WR_HCONV,
	GPIO_WR_CHB_RESET, 			// Channel Browser Reset
	GPIO_WR_CHB_BOOSTER_CTRL,	// CHB Booster Ctrl
	GPIO_WR_TW9910_RESET, 			// TW9910 Reset
	GPIO_WR_CHB_TUNER_RESET,	// Reset Tuner for CHB
	GPIO_WR_M_RFMODULE_RESET,
	GPIO_WR_RF_SWITCH_CTL_CHB,
	GPIO_WR_SCART_MUTE_CTRL,
	GPIO_WR_NAND_WP_CTRL,//hjkoh_091027
	GPIO_WR_SELECT_CI,	// CI_SEL (dreamer.lee@lge.com)
	GPIO_WR_FMTUP_NCS,		// FMTUP
	GPIO_WR_FMTUP_DCLK,	// FMTUP
	GPIO_WR_FMTUP_ASDI,	// FMTUP
	GPIO_WR_LG8300_RESET, // LG8300 Reset
	//GPIO_WR_USB_POWER,  // control USB power ON
	GPIO_WR_CONTROL_3D,
	GPIO_WR_CONTROL_FPGA_DOWNPORT,
	GPIO_WR_SWITCH_MOTMOD_RS232,
	GPIO_WR_M_RFMODULE_DC,
	GPIO_WR_M_RFMODULE_DD,


#if (PLATFORM_TYPE == LG_PLATFORM)
	GPIO_WR_OPT_P,		//Mini LVDS control  //index 61
	GPIO_WR_OPT_N,		//Mini LVDS control
	GPIO_WR_3D_RESET,
	GPIO_WR_3D_POWER_EN,
	GPIO_WR_REAR_USB_CTRL,
	GPIO_WR_ENABLE_USB2_CTRL,
	GPIO_WR_ARC_CTRL,	// Audio Return Channel for HDMI
	GPIO_WR_OPTIC_CTRL,
	GPIO_WR_FE_TSOUT_BUF_CTRL,
	GPIO_WR_L9_TSOUT_BUF_CTRL,
	GPIO_WR_ENABLE_HDMI_RESET,

#endif
	GPIO_WR_END,
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
extern DTV_STATUS_T DDI_GPIO_SetDefault(void);

extern DTV_STATUS_T DDI_GPIO_GetControl(UINT8 gpioFnType, UINT8 ctrl1, UINT8 ctrl2, UINT8 *pData);
extern DTV_STATUS_T DDI_GPIO_SetControl(UINT8 gpioFnType, UINT8 ctrl1, UINT8 ctrl2);
#if 0
extern DTV_STATUS_T DDI_GPIO_SetStandbyMute(BOOLEAN bOn);
#endif
extern void 		DDI_GPIO_Debug(void);

//extern void DDI_GPIO_FPGADownPortControl(UINT8 b_enable); //balup_FPGA

#if 0
#if	(PLATFORM_TYPE == BCM_PLATFORM)
typedef void (*DDI_GPIO_CALLBACK_T) (UINT32 index);
extern DTV_STATUS_T DDI_CMNIO_GPIO_BCM_EnableIRQ( UINT32 index, DDI_GPIO_CALLBACK_T callback );
#endif
#endif

#endif  /* _GPIO_DDI_H_ */
