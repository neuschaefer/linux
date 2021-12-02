/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file osa_config.h
 *
 *	System Call API configuration.
 *
 *	@author 	cskim
 *	@version	1.0
 *	@date		2005. 6. 1
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/
#ifndef _OSA_TIMER_H_
#define _OSA_TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif


#define	OSA_TIMER_IDX_BASE							0x10000
#define	OSA_TIMER_IDX_CECP_BASE						0x11000
#define	OSA_TIMER_IDX_PSIP_BASE						0x12000
#define	OSA_TIMER_IDX_SI_BASE						0x13000
#define	OSA_TIMER_IDX_UI_BASE						0x14000
#ifdef INCLUDE_MHP
#define	OSA_TIMER_IDX_MHP_BASE						0x15000
#endif

#ifdef INCLUDE_BML
#define	OSA_TIMER_IDX_BML_BASE						0x16000
#endif


#define	OTA_DDBEXIT_TIMER_ID						(OSA_TIMER_IDX_BASE    +  1)
#define	OTA_PROGRESS_TIMER_ID						(OSA_TIMER_IDX_BASE    +  2)
#define	CI_TIMER_ID									(OSA_TIMER_IDX_BASE    +  3)
#define	MICOM_TIMER_ID								(OSA_TIMER_IDX_BASE    +  4)
#define	OAD_PROGRESS_TIMER_ID						(OSA_TIMER_IDX_BASE    +  5)
#define	OAD_SRGFIL_TIMER_ID							(OSA_TIMER_IDX_BASE    +  6)
#define	PDPMODULE_INV_TIMER_ID						(OSA_TIMER_IDX_BASE    +  7)
#define	PDPMODULE_ORB_TIMER_ID						(OSA_TIMER_IDX_BASE    +  8)
#define	DIMMING_TIMER1_STATUS_UPDATE				(OSA_TIMER_IDX_BASE    +  9)
#define	PWM_INIT_END_TIMER_ID						(OSA_TIMER_IDX_BASE    + 10)
#define	PDP_USE_TIMER_ID							(OSA_TIMER_IDX_BASE    + 11)
#define	PATTERN_TEST_TIMER_ID1						(OSA_TIMER_IDX_BASE    + 12)
#define	PATTERN_TEST_TIMER_ID2						(OSA_TIMER_IDX_BASE    + 13)
#define	WB_MUTE_TIMER_ID							(OSA_TIMER_IDX_BASE    + 14)
#define	WB_ENABLE_TIMER_ID							(OSA_TIMER_IDX_BASE    + 15)
#define	TIMER_ID_VFD_INPUT_SOURCE_UPDATE			(OSA_TIMER_IDX_BASE    + 16)
#define	TIMER_ID_VFD_DVR_TIME_UPDATE				(OSA_TIMER_IDX_BASE    + 17)
#define	ACMS_TIMER_ID								(OSA_TIMER_IDX_BASE    + 18)
#define	AUTO_ARC_TIMER1_STATUS_UPDATE				(OSA_TIMER_IDX_BASE    + 19)
#define	ACC_TIMER1_ID								(OSA_TIMER_IDX_BASE    + 20)
#define	OSA_MICOM_TIMER_ID							(OSA_TIMER_IDX_BASE    + 21)
#define	VIDEO_VBI_SEND_DATA_TIMER_ID				(OSA_TIMER_IDX_BASE    + 22)
#define EMP_TIMER_AUDIO_ID							(OSA_TIMER_IDX_BASE    + 23)
#define	ACAP_RELEASE_KEY_TIMER_ID					(OSA_TIMER_IDX_BASE    + 24)
#define EMP_TIMER_VIDEO_ID							(OSA_TIMER_IDX_BASE    + 25)
#define PQ_XVYCC_STATUS_TIMER_ID					(OSA_TIMER_IDX_BASE    + 26)
#define PDP_IR_LED_TIMER_ID							(OSA_TIMER_IDX_BASE    + 27)
#define PDP_STBY_LED_TIMER_ID						(OSA_TIMER_IDX_BASE    + 28)
#define VIDEO_HDMI_INFO_PARSER_TIMER_ID				(OSA_TIMER_IDX_BASE    + 29)

#define ADCC_CLEARTIMER_ID		(OSA_TIMER_IDX_BASE    + 30)   // 1		/* in case that the caption_statement_data() has no CS, clear the screen */
#define ADCC_CHECKTIMER_ID		(OSA_TIMER_IDX_BASE    + 31)   // 2		/* in case that the signal status is bad, clear the screen */
#define ADCC_EXISTTIMER_ID		(OSA_TIMER_IDX_BASE    + 32)   // 3		/* timer fot the counter of the existence of the ADCC */
#define ADCC_STARTTIMER_ID		(OSA_TIMER_IDX_BASE    + 32)   // 3		/* timer for the start of ADCC */

#if defined(INCLUDE_MOUSE) || defined(INCLUDE_MOUSE_SP)
#define	SP_TASK_TIMER_ID							(OSA_TIMER_IDX_BASE	   + 33)
#define	SP_SETTING_TIMER_ID							(OSA_TIMER_IDX_BASE    + 34)
#define	SP_FEEDBACK_TIMER_ID						(OSA_TIMER_IDX_BASE    + 35)
#define	SP_UI_TIMER_ID								(OSA_TIMER_IDX_BASE    + 36)
#define	UI_CURSOR_TIMER_ID							(OSA_TIMER_IDX_BASE    + 37)
#define	UI_GESTURECH_TIMER_ID						(OSA_TIMER_IDX_BASE    + 38)
#define	UI_VOLUME_TIMER_ID							(OSA_TIMER_IDX_BASE    + 39)
#endif

#define	LCD_INV_ON_TIMER_ID							(OSA_TIMER_IDX_BASE    + 40)
#define	OTA_WRITE_TIMER_ID						(OSA_TIMER_IDX_BASE    +  41)

#ifdef INCLUDE_STREAM
#define EME_TIMER_PLAY_START						(OSA_TIMER_IDX_BASE    + 45)
#endif

#ifdef INCLUDE_NETFLIX
#define ASF_TIMER_PLAY_ID							(OSA_TIMER_IDX_BASE	+ 50)
#endif

#ifdef INCLUDE_NSU
#define	NSU_PROGRESS_TIMER_ID						(OSA_TIMER_IDX_BASE    + 70)
#define	NSU_WRITE_EXIT_TIMER_ID						(OSA_TIMER_IDX_BASE    + 71)
#endif

#ifdef INCLUDE_VUDU_NATIVE
#define VUDU_TIMER_ID_START							(OSA_TIMER_IDX_BASE		+ 72)
#define VUDU_TIMER_ID_END								(OSA_TIMER_IDX_BASE		+ 73)
#endif

#ifdef INCLUDE_USB
#define USBUPDATE_PROGRESS_TIMER_ID					(OSA_TIMER_IDX_BASE + 74)
#endif

#ifdef INCLUDE_ADDON
#define 	ADDON_RELEASE_KEY_TIMER_ID				(OSA_TIMER_IDX_BASE    + 80)
#endif

#define 	VDEC_TIMER_ID							(OSA_TIMER_IDX_BASE    + 81)
#define 	VDEC_LIPSYNCON_ID						(OSA_TIMER_IDX_BASE    + 82)
#define 	ADEC_TIMER_ID							(OSA_TIMER_IDX_BASE    + 83)
#define	MHP_RELEASE_KEY_TIMER_ID					(OSA_TIMER_IDX_BASE    + 84)
#define	POWERSEQ_EXT_PWM_TIMER_ID					(OSA_TIMER_IDX_BASE    + 85)
#ifdef INCLUDE_BT_BRCM
#define BT_UPLOAD_TIMER_ID	(OSA_TIMER_IDX_BASE + 86)
#endif

#if 1 //def INCLUDE_TRIDTV
#define FMT_HDMI_INFO_PARSER_TIMER_ID				(OSA_TIMER_IDX_BASE    + 87) /*tridfmt */
#endif

#define DISEQC_MOTOR_TIMERID 						(OSA_TIMER_IDX_BASE    + 88)

#define FMT_SWU_TIMER_ID							(OSA_TIMER_IDX_BASE    + 89)

#define	UI_EV_TIMER_ID								(OSA_TIMER_IDX_UI_BASE +  1)
#define	UI_CB_TIMER_ID								(OSA_TIMER_IDX_UI_BASE +  2)

#ifdef INCLUDE_MDNS
#ifdef INCLUDE_1Q
#define MDNS_TIMER_ID								(OSA_TIMER_IDX_BASE    + 90)
#else
#define MDNS_TIMER_ID								(OSA_TIMER_IDX_BASE    + 86)
#endif
#endif

#ifdef INCLUDE_1Q
#define ML_TIMER_ID								(OSA_TIMER_IDX_BASE    + 91)
#endif

#ifdef INCLUDE_BML
#define	BML_RELEASE_KEY_TIMER_ID					(OSA_TIMER_IDX_BASE    + 92)
#endif
#define BSI_CURSOR_TIMER_ID									(OSA_TIMER_IDX_BASE    + 93)

#ifdef INCLUDE_JCAP
#define JCAP_MANAGEMENT_TIMER_ID					(OSA_TIMER_IDX_BASE    + 94)
#endif /* INCLUDE_JCAP */

#ifdef INCLUDE_BML
#define	BML_DNS_QUERY_1_TIMER_ID					(OSA_TIMER_IDX_BASE    + 95)
#define	BML_DNS_QUERY_2_TIMER_ID					(OSA_TIMER_IDX_BASE    + 96)
#define	BML_DNS_QUERY_3_TIMER_ID					(OSA_TIMER_IDX_BASE    + 97)
#define	BML_DNS_QUERY_4_TIMER_ID					(OSA_TIMER_IDX_BASE    + 98)
#endif

#ifdef __cplusplus
}
#endif

#endif /* end of _OSA_TIMER_H_ */

