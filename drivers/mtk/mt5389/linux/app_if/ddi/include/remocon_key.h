/******************************************************************************
*	DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA.
*	Copyright(c) 1999 by LG Electronics Inc.
*
*	 All rights reserved. No part of this work may be reproduced, stored in a
*	 retrieval system, or transmitted by any means without prior written
*	 permission of LG Electronics Inc.
*****************************************************************************/

/** @file remocon_key.h
*
*	define key & key type(REMOTE/LOCAL/)
*
*	@author 	Kun-IL Lee(dreamer@lge.com)
*	@version	1.0
*	@date	  	1999.12.3
*	@note
*/

/*---------------------------------------------------------
    (Header File Guarder )
---------------------------------------------------------*/
#ifndef _REMOTE_KEY_H_
#define	_REMOTE_KEY_H_

/*---------------------------------------------------------
    Control 상수 정의
    (Control Constants)
---------------------------------------------------------*/

/*---------------------------------------------------------
    #include 파일들
    (File Inclusions)
---------------------------------------------------------*/
#include "branches.h"
#ifdef INCLUDE_ARCHITECTURE_1G
#include <linux/input.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------
    상수 정의
    (Constant Definitions)
    linux/input.h에 존재하는 키는 그대로 mapping하고
    아닌 경우는 중복을 막기 위해 lge_base위에 정의한다.
---------------------------------------------------------*/
#ifdef INCLUDE_ARCHITECTURE_1G
#define KEY_LG_BASE						KEY_MAX
#define	LGE_BASE(x)						(KEY_LG_BASE+(x))	/* x 는 0x1ff(511)개를 절대로 초과할 수 없다. 만약 초과시 linux/input.h 및 기타 수정 필수*/


/*	KEYs definition */
#define IR_KEY_POWER					KEY_POWER			/* 0x08 전원  */
#define IR_KEY_TV						KEY_TV				/* 0x0F TV */
#define IR_KEY_TV_VIDEO 				KEY_VIDEO_NEXT		/* 0x0B  TV/외부입력 */
#define IR_KEY_COMP_RGB_HDMI			LGE_BASE(0x98)		/*  멀티미디어 : 지원 안함 */
#define IR_KEY_STORE_POWER				LGE_BASE(0x33)				/* Store Power On */
#define IR_KEY_LOCAL_DIMMING			LGE_BASE(0x36)				/* Local Dimming Demo */

#define IR_KEY_0						KEY_0				/*  0x10 숫자 0 */
#define IR_KEY_1						KEY_1				/* 0x11 숫자 1 */
#define IR_KEY_2						KEY_2				/* 0x12  숫자 2 */
#define IR_KEY_3						KEY_3				/* 0x13  숫자 3 */
#define IR_KEY_4						KEY_4				/* 0x14  숫자 4 */
#define IR_KEY_5						KEY_5				/* 0x15  숫자 5 */
#define IR_KEY_6						KEY_6				/* 0x16  숫자 6 */
#define IR_KEY_7						KEY_7				/* 0x17  숫자 7 */
#define IR_KEY_8						KEY_8				/* 0x18  숫자 8 */
#define IR_KEY_9						KEY_9				/* 0x19  숫자 9 */
#define IR_KEY_DASH 					LGE_BASE(0x4C) 		/* 0x4C  Dash(-) */
#define IR_KEY_FLASHBACK				LGE_BASE(0x1A) 		/* 0x1A  이전채널 */

#define IR_KEY_CH_UP					KEY_CHANNELUP		/* 0x00  채널 + */
#define IR_KEY_CH_DOWN					KEY_CHANNELDOWN		/* 0x01  채널 - */
#define IR_KEY_VOL_UP					KEY_VOLUMEUP		/* 0x02  음량 + */
#define IR_KEY_VOL_DOWN 				KEY_VOLUMEDOWN		/* 0x03  음량 - */

#define IR_KEY_HOME 					LGE_BASE(0x7C)		/*  홈메뉴 */
#define IR_KEY_HOME_US 					LGE_BASE(0xC8)		/*  홈메뉴 */
#define IR_KEY_MUTE 					KEY_MUTE			/* 0x09 조용히 */
#define IR_KEY_FAVORITE 				KEY_FAVORITES		/* 0x1E 선호채널  */

#define IR_KEY_MENU 					KEY_MENU			/* 0x43 메뉴 */
#define IR_KEY_GRIDGUIDE				KEY_PROGRAM			/* 0xA9 전체방송 */
#define IR_KEY_INFO 					KEY_INFO			/* 0xAA 현재방송 */
#define IR_KEY_EXIT 					KEY_EXIT			/* 0x5B 취소 */

#define IR_KEY_UP_ARROW 				KEY_UP				/* 0x40 UP	 */
#define IR_KEY_DOWN_ARROW				KEY_DOWN			/* 0x41 DOWN  */
#define IR_KEY_LEFT_ARROW				KEY_LEFT			/* 0x07 MINUS */
#define IR_KEY_RIGHT_ARROW				KEY_RIGHT			/* 0x06 PLUS  */
#define IR_KEY_ENTER					KEY_ENTER			/* 0x44 확인   */

#define IR_KEY_RESLIST 					LGE_BASE(0x89)		/*  예약목록 */

#define IR_KEY_MARK 					LGE_BASE(0x7D)		/*  마크 */
#define IR_KEY_LIVETV					LGE_BASE(0x9E)		/* 0x9E Live TV */
#define IR_KEY_HCEC						LGE_BASE(0x7E)

//#define IR_KEY_INFORMATION			0x1B				/* Information(EU) */
#define IR_KEY_TELETEXT 				LGE_BASE(0x20)		/* Teletext(EU) */
#define IR_KEY_TEXTOPTION 				LGE_BASE(0x21)		/* Teletext(EU) */
#define IR_KEY_TEXTMODE					LGE_BASE(0x22)		/* Text mode(List) */
#define IR_KEY_TEXTMIX					LGE_BASE(0x24)		/* Text mix */
#define IR_KEY_TEXTSUBPAGE				LGE_BASE(0x26)		/* Text subpage(time) */
#define IR_KEY_BACK						KEY_PREVIOUS 		/* 0x28 Back */
//#define IR_KEY_BACK					KEY_BACK 			/* 0x28 Back */
#define IR_KEY_TEXTREVEAL				LGE_BASE(0x2A)		/* Text Reveal */
//#define IR_KEY_TEXTHOLD					0x65				/* ext Hold */
//hdcho
#define IR_KEY_FREEZE					LGE_BASE(0x65)		/* Freeze  */
#define IR_KEY_DA						LGE_BASE(0x50)		/* Digital/Analog TV */
#define IR_KEY_PRLIST					LGE_BASE(0x53)		/* Program List */
//#define IR_KEY_SUBTITLE				0x56				/* Subtitle ( It will be combine with 0x39) */
#define IR_KEY_DIGITALSETUP				LGE_BASE(0x5F)		/* Digital Setup */
#define IR_KEY_POSITION					LGE_BASE(0x62)		/* PIP Position/Text Update */
#define IR_KEY_SIZE						LGE_BASE(0x64)		/* Text Size */
#define IR_KEY_GUIDEPAL					LGE_BASE(0xAB)		/* Guide(For PAL TV) */
#define IR_KEY_TV_RADIO					LGE_BASE(0XF0)
#define IR_KEY_QMENU					LGE_BASE(0x45)

#define IR_KEY_RED						KEY_RED
#define IR_KEY_GREEN					KEY_GREEN
#define IR_KEY_YELLOW					KEY_YELLOW
#define IR_KEY_BLUE 					KEY_BLUE

#define IR_KEY_PIP						LGE_BASE(0x60)		/* 동시 화면 */
#define IR_KEY_PIP_CH_UP				IR_KEY_GREEN		/* 부화면+ */
#define IR_KEY_PIP_CH_DOWN				IR_KEY_RED			/* 부화면- */
#define IR_KEY_PIP_INPUT				IR_KEY_BLUE			/* 부입력 */
#define IR_KEY_SWAP						IR_KEY_YELLOW		/* 주부전환 */

#define IR_KEY_TIMER					LGE_BASE(0x0E)		/* 취침예약 */
#define IR_KEY_XD						LGE_BASE(0x92)		/* XD키  */
#define IR_KEY_ASPECT_RATIO 			LGE_BASE(0x79)		/* 화면크기 */

#define IR_KEY_SAP						LGE_BASE(0x0A)		/* 음성다중 */
#define IR_KEY_CC						LGE_BASE(0x39)		/* 자막방송 */
#define IR_KEY_EZPIC					LGE_BASE(0x4D)		/* 자동영상  */
#define IR_KEY_EZSOUND					LGE_BASE(0x52)		/* 자동음성 */

#define IR_KEY_ADJUST					LGE_BASE(0xCB)		/* 조정메뉴  */
#define IR_KEY_EJECT					LGE_BASE(0xCA)		/* M/C 꺼내기  */
#define IR_KEY_DWTS						LGE_BASE(0x67)		/* Delayed/Sync TS */

#define IR_KEY_MHEG						LGE_BASE(0x9F)		/* 데이터 방송 키 */
#ifndef _EMUL_WIN
#ifdef INCLUDE_ACAP
#define IR_KEY_ACAP						LGE_BASE(0x9F)		/* 데이터 방송 키 */
#endif
#else
#define IR_KEY_ACAP						LGE_BASE(0x9F)		/* 데이터 방송 키 */
#endif

#ifdef INCLUDE_MHP
#define IR_KEY_MHP						LGE_BASE(0x9F)		/* 데이터 방송 키 */
#endif

#define IR_KEY_SCR_KEYBOARD				LGE_BASE(0x32)		/* screen keboard */
#define IR_KEY_AUTODEMO 				LGE_BASE(0x80)
#define IR_KEY_AUTOSCAN 				LGE_BASE(0x54)
#define IR_KEY_CHADDDEL 				LGE_BASE(0x55)

#define IR_KEY_PLAY 					KEY_PLAY			/* 0xB0 재생 */
#define IR_KEY_PAUSE					KEY_PAUSE			/* 0xBA  일시정지 */
#define IR_KEY_STOP 					KEY_STOP			/* 0xB1 정지 */
#define IR_KEY_REC						KEY_RECORD			/* 0xBD 녹화 */

#define IR_KEY_REW						KEY_REWIND   		/* 0x8F 되감기	*/
#define IR_KEY_FF						KEY_FASTFORWARD    	/* 0x8E 빨리감기 */
#define IR_KEY_GOTOPREV 				LGE_BASE(0xB2)		/* Skip Backward */
#define IR_KEY_GOTONEXT 				LGE_BASE(0xB3)		/* Skip Forward */

#ifdef INCLUDE_EMANUAL
#define IR_KEY_EMANUAL_PAGEPREV			LGE_BASE(0xB4)		/* e-manual page forward    */
#define IR_KEY_EMANUAL_PAGENEXT			LGE_BASE(0xB5)		/* e-manual page backword */
#endif

#ifdef INCLUDE_MOUSE
//#define IR_KEY_MOUSE_MENU				0x3A
#define IR_KEY_MOUSE_OK					BTN_MOUSE			// 0x29
#define IR_KEY_MOUSE_MCH				LGE_BASE(0x2B)		// ???
#define IR_KEY_PAIRING_START			LGE_BASE(0x93)
#define IR_KEY_PAIRING_STOP				LGE_BASE(0x94)
#endif

#define IR_KEY_3D 						LGE_BASE(0xDB)		//by hwangbos 20091214 IR_KEY_PAIRING_START	//0xDB				/* 3D on/off */
#define IR_KEY_3D_MODE					LGE_BASE(0xDC)		//by hwangbos 20091214 IR_KEY_PAIRING_STOP		//0xDC				/* 3D mode */
#define IR_KEY_3D_LR					LGE_BASE(0xDD)		/* 3D L/R */


//	Broadband 관련
#define IR_KEY_YAHOO					LGE_BASE(0x58)		/* YAHOO*/
#define IR_KEY_NETCAST					LGE_BASE(0x59)		/* NETCAST */

//Color Key 대응 by athens
//문제 가능성 ----------- Todo


#ifdef _EMUL_WIN
//Ch Edit에서 Color key대응
//Lock 메뉴에서 Block channel 에 Green color key 추가함
#define IR_KEY_CHEDIT_AUTO_SORT			IR_KEY_GREEN
//#define IR_KEY_CHEDIT_DEL				IR_KEY_GREEN
#define IR_KEY_CHEDIT_MOVE				IR_KEY_YELLOW
#define IR_KEY_CHEDIT_SKIP          	IR_KEY_BLUE

// Delete key
#define IR_KEY_LOCK_DEL					IR_KEY_RED
#endif

#define IR_KEY_MODE						IR_KEY_RED			/* R */
#define IR_KEY_TIMER_LIST				IR_KEY_BLUE			/* B */
#define IR_KEY_MANUAL_TIMER				IR_KEY_YELLOW		/* Y */
#define IR_KEY_DATE 					IR_KEY_GREEN		/* N */

/* TV extension key */
#define IR_KEY_PAGE_UP					IR_KEY_CH_UP
#define IR_KEY_PAGE_DOWN				IR_KEY_CH_DOWN
#define IR_KEY_PAGE_RIGHT				IR_KEY_SYNC
#define IR_KEY_PAGE_LEFT				IR_KEY_MV2START
#define IR_KEY_FORMAT_CHANGE			LGE_BASE(0x03D)	//	0x10E1

//#define IR_KEY_FREEZE					0x1010
#define IR_KEY_GUIDE					IR_KEY_GRIDGUIDE

/* DVR extension key */
#define IR_KEY_MV2START 				IR_KEY_GOTOPREV		/* 처음 보기 */
#define IR_KEY_SYNC 					IR_KEY_GOTONEXT		/* 끝 보기 */
#define IR_KEY_RESUME					IR_KEY_PLAY
#define IR_KEY_PAUSE_SLOWFWD			IR_KEY_PAUSE
#define IR_KEY_SLOW_FORWARD				LGE_BASE(0x01E)	//	0x11B0
#define IR_KEY_IREPLAY					IR_KEY_GOTOPREV
#define IR_KEY_SKIPFWD					IR_KEY_GOTONEXT
#define IR_KEY_TIME_SHIFT				LGE_BASE(0x01F)	//	0x11BB	/* 타임시프트 */

#define IR_KEY_RECLIST					IR_KEY_GREEN 		/* 한국향 dvr ready에서 green키를 reclist (녹화목록) 키로 이용함. */
#define IR_KEY_REPEAT					IR_KEY_BLUE 		/* 한국향 dvr ready에서 blue키를 repeat (구간반복) 키로 이용함. */	// IR_KEY_MARK
#ifdef INCLUDE_DVR_READY_EDT
#define IR_KEY_EDIT						IR_KEY_YELLOW 		/* GP2 dvr ready에서 YELLOW키를 edit (구간편집) 키로 이용함. */
#endif
#if 0 /* delete unused dvr key. by jihyeon@20080508 */
#define IR_KEY_RESREGIST				0x118A
#endif


/* 화면 밝기 */
#define IR_KEY_BRIGHTNESS_UP			KEY_BRIGHTNESSUP	/* 0xE0 화면밝기+ */
#define IR_KEY_BRIGHTNESS_DOWN			KEY_BRIGHTNESSDOWN	/* 0xE1 화면밝기- */

#define IR_KEY_GAMEMODE					LGE_BASE(0x30)		/* game mode - 070901 */

#define IR_KEY_AD						LGE_BASE(0x91)		/* Audio Description - 091109 */
#define IR_KEY_FAMILY					LGE_BASE(0x2E)		/* Family Mode - 	100323 */
#define IR_KEY_CRICKET					LGE_BASE(0x3A)		/* Cricket Mode - 	100323 */

//Japan Number Key - hojin.koh_110219 - Add Japan Model two byte key code definition.
#define IR_KEY_BS					LGE_BASE(0x100)
#define IR_KEY_BS_NUM_1				LGE_BASE(0x101)
#define IR_KEY_BS_NUM_2				LGE_BASE(0x102)
#define IR_KEY_BS_NUM_3				LGE_BASE(0x103)
#define IR_KEY_BS_NUM_4				LGE_BASE(0x104)
#define IR_KEY_BS_NUM_5				LGE_BASE(0x105)
#define IR_KEY_BS_NUM_6				LGE_BASE(0x106)
#define IR_KEY_BS_NUM_7				LGE_BASE(0x107)
#define IR_KEY_BS_NUM_8				LGE_BASE(0x108)
#define IR_KEY_BS_NUM_9				LGE_BASE(0x109)
#define IR_KEY_BS_NUM_10			LGE_BASE(0x10A)
#define IR_KEY_BS_NUM_11			LGE_BASE(0x10B)
#define IR_KEY_BS_NUM_12			LGE_BASE(0x10C)

#define IR_KEY_CS1					LGE_BASE(0x110)
#define IR_KEY_CS1_NUM_1			LGE_BASE(0x111)
#define IR_KEY_CS1_NUM_2			LGE_BASE(0x112)
#define IR_KEY_CS1_NUM_3			LGE_BASE(0x113)
#define IR_KEY_CS1_NUM_4			LGE_BASE(0x114)
#define IR_KEY_CS1_NUM_5			LGE_BASE(0x115)
#define IR_KEY_CS1_NUM_6			LGE_BASE(0x116)
#define IR_KEY_CS1_NUM_7			LGE_BASE(0x117)
#define IR_KEY_CS1_NUM_8			LGE_BASE(0x118)
#define IR_KEY_CS1_NUM_9			LGE_BASE(0x119)
#define IR_KEY_CS1_NUM_10			LGE_BASE(0x11A)
#define IR_KEY_CS1_NUM_11			LGE_BASE(0x11B)
#define IR_KEY_CS1_NUM_12			LGE_BASE(0x11C)

#define IR_KEY_CS2					LGE_BASE(0x120)
#define IR_KEY_CS2_NUM_1			LGE_BASE(0x121)
#define IR_KEY_CS2_NUM_2			LGE_BASE(0x122)
#define IR_KEY_CS2_NUM_3			LGE_BASE(0x123)
#define IR_KEY_CS2_NUM_4			LGE_BASE(0x124)
#define IR_KEY_CS2_NUM_5			LGE_BASE(0x125)
#define IR_KEY_CS2_NUM_6			LGE_BASE(0x126)
#define IR_KEY_CS2_NUM_7			LGE_BASE(0x127)
#define IR_KEY_CS2_NUM_8			LGE_BASE(0x128)
#define IR_KEY_CS2_NUM_9			LGE_BASE(0x129)
#define IR_KEY_CS2_NUM_10			LGE_BASE(0x12A)
#define IR_KEY_CS2_NUM_11			LGE_BASE(0x12B)
#define IR_KEY_CS2_NUM_12			LGE_BASE(0x12C)


#define IR_KEY_TER					LGE_BASE(0x130)
#define IR_KEY_TER_NUM_1			LGE_BASE(0x131)
#define IR_KEY_TER_NUM_2			LGE_BASE(0x132)
#define IR_KEY_TER_NUM_3			LGE_BASE(0x133)
#define IR_KEY_TER_NUM_4			LGE_BASE(0x134)
#define IR_KEY_TER_NUM_5			LGE_BASE(0x135)
#define IR_KEY_TER_NUM_6			LGE_BASE(0x136)
#define IR_KEY_TER_NUM_7			LGE_BASE(0x137)
#define IR_KEY_TER_NUM_8			LGE_BASE(0x138)
#define IR_KEY_TER_NUM_9			LGE_BASE(0x139)
#define IR_KEY_TER_NUM_10			LGE_BASE(0x13A)
#define IR_KEY_TER_NUM_11			LGE_BASE(0x13B)
#define IR_KEY_TER_NUM_12			LGE_BASE(0x13C)

#define IR_KEY_3DIGIT_INPUT			LGE_BASE(0x23)	//Japan Only - newly added
#define IR_KEY_BML_DATA				LGE_BASE(0x9F)	//Japan Only - newly added
#define IR_KEY_JAPAN_DISPLAY		LGE_BASE(0xAC) 	//Japan Only - newly added


#ifdef INCLUDE_DVR_READY
/* T-CON */
#define IR_KEY_TCON_UP					0x30F0	/* T-CON Virtual Key */
#define IR_KEY_TCON_UPRIGHT				0x30F1	/* T-CON Virtual Key */
#define IR_KEY_TCON_RIGHT				0x30F2	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWNRIGHT			0x30F3	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWN				0x30F4	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWNLEFT			0x30F5	/* T-CON Virtual Key */
#define IR_KEY_TCON_LEFT				0x30F6	/* T-CON Virtual Key */
#define IR_KEY_TCON_UPLEFT				0x30F7	/* T-CON Virtual Key */
#endif

#define IR_KEY_LEFT_SCROLL_1ST			LGE_BASE(0xA0) 	/* T-CON 좌1*/
#define IR_KEY_LEFT_SCROLL_2ND			LGE_BASE(0xA1) 	/* T-CON 좌2*/
#define IR_KEY_LEFT_SCROLL_3RD			LGE_BASE(0xA2) 	/* T-CON 좌3*/
#define IR_KEY_LEFT_SCROLL_4TH			LGE_BASE(0xA3) 	/* T-CON 좌4*/
#define IR_KEY_LEFT_SCROLL_5TH			LGE_BASE(0xA7) 	/* T-CON 좌5*/
#define IR_KEY_RIGHT_SCROLL_1ST			LGE_BASE(0xB4) 	/* T-CON 우1*/
#define IR_KEY_RIGHT_SCROLL_2ND			LGE_BASE(0xB5) 	/* T-CON 우2*/
#define IR_KEY_RIGHT_SCROLL_3RD			LGE_BASE(0xB7) 	/* T-CON 좌3*/
#define IR_KEY_RIGHT_SCROLL_4TH			LGE_BASE(0xB9) 	/* T-CON 좌4*/
#define IR_KEY_RIGHT_SCROLL_5TH			LGE_BASE(0xBB) 	/* T-CON 좌5*/

/* factory key */
#define IR_KEY_EYE_Q					LGE_BASE(0x95)
#define IR_KEY_IN_STOP         			LGE_BASE(0xFA)
#define IR_KEY_IN_START        			LGE_BASE(0xFB)
#define IR_KEY_P_CHECK         			LGE_BASE(0xFC)
#define IR_KEY_HDMI_CHECK				LGE_BASE(0xA6) //[081020 leekyungho] : IR_KEY_HDMI_CHECK(0xA6) 기능 UI 대응
#define IR_KEY_S_CHECK         			LGE_BASE(0xFD)
#define IR_KEY_POWERONLY   				LGE_BASE(0xFE)
#define IR_KEY_ADJ	   					LGE_BASE(0xFF)
#define IR_KEY_EZ_ADJUST        		IR_KEY_ADJ
#define IR_KEY_FRONT_AV	   				LGE_BASE(0x51)
#define IR_KEY_FMODE_INIT				LGE_BASE(0x27)
#define IR_KEY_FMODE_START				LGE_BASE(0xEA)
#define IR_KEY_FMODE_F1					LGE_BASE(0xEB)
#define IR_KEY_IN_TIME          		LGE_BASE(0x26)
#define IR_KEY_LAMP_RESET				LGE_BASE(0x35)				/* for KOR model */
#define IR_KEY_DISPMODE_READY			LGE_BASE(0xEC)				/* for KOR model */
#define IR_KEY_DISPMODE					IR_KEY_AUTODEMO
#define IR_KEY_BLUETOOTH				LGE_BASE(0x1F)
#define IR_KEY_USB_CHECK				LGE_BASE(0xEE)				/* 081106 swbyun : IR_KEY_USB_CHECK(0xEE) 기능 UI 대응 */
#define IR_KEY_USB2_CHECK				LGE_BASE(0xA8)				/* stonedef - 091121 : USB 2 port JPEG, MP3 자동 play */

#define IR_KEY_TILT						LGE_BASE(0xF9)				/* Module Pattern Generation */

/* DMS 요구사항에 의해 추가된 Key Code로
    생산 line에서 사용하는 별도의 Remocon이 있음
    Test를 위해서 별도의 Key code를 사용해야함.*/
#define IR_KEY_POWERSAVING_TEST			LGE_BASE(0xFB)				/* DMS 요구사항 반영 */

/* Factory Mode */
#define IR_VIRKEY_SHOWMSG_ADJUST		0x2021
#define IR_VIRKEY_CHANGE_SOURCE_AV1		0x2022
#define IR_VIRKEY_CHANGE_SOURCE_COMP1	0x2023
#define IR_VIRKEY_CHANGE_SOURCE_RGB 	0x2024
#define IR_VIRKEY_ADJUST_UPD			0x2025
#define IR_VIRKEY_ADJUST_AD 			0x2026				/* ADC 조정을 위해 사용 */
#define IR_VIRKEY_ADAVOSD				0x2027

/* front key */
//#define IR_FRONTKEY_AV					0x51				/* 조정 remocon key - 생산시 front AV로 전환하는데 사용 */
#define IR_FRONTKEY_INPUT_SELECT		LGE_BASE(0x1F6)
#define IR_FRONTKEY_MENU				LGE_BASE(0x1F5)
#define IR_FRONTKEY_ENTER				LGE_BASE(0x1F7)
#define IR_FRONTKEY_CH_UP				LGE_BASE(0x1F3)
#define IR_FRONTKEY_CH_DOWN				LGE_BASE(0x1F4)
#define IR_FRONTKEY_VOL_UP				LGE_BASE(0x1F1)
#define IR_FRONTKEY_VOL_DOWN			LGE_BASE(0x1F2)
#define IR_FRONTKEY_TVGUIDE 			IR_FRONTKEY_INPUT_SELECT
#define IR_FRONTKEY_POWER				LGE_BASE(0x1F8)

#define IR_KEY_MULTICHANNEL				LGE_BASE(0x03E)	//	0x10FA
#define IR_KEY_WIDGET					LGE_BASE(0x03F)	//	0x10FB
#define IR_KEY_SETUP					LGE_BASE(0x040)	//	0x10FC
#define IR_KEY_MULTIMEDIA				LGE_BASE(0x041)	//	0x10FD
#define IR_KEY_FLASHGAME				LGE_BASE(0x010)	//	0x10FE
#define IR_KEY_PHOTOLIST				LGE_BASE(0x011)	//	0x10FF
#define IR_KEY_MUSICLIST				LGE_BASE(0x012)	//	0x1100
#define IR_KEY_MOVIELIST				LGE_BASE(0x013)	//	0x1101
#define IR_KEY_DIRECTOSD				LGE_BASE(0x014)	//	0x1102
#define IR_KEY_EMANUEL					LGE_BASE(0x015)	//	0x1103
#define IR_KEY_BT_MENU					LGE_BASE(0x016)	//	0x1104
#define IR_KEY_VIR_INPUTLIST			LGE_BASE(0x017)	//	0x1105
#define IR_KEY_GAME_REAL_SWING			LGE_BASE(0x018)	//	0x1106
#define IR_KEY_GAME_SUDO_SWING			LGE_BASE(0x019)	//	0x1107
#define IR_KEY_CUBEMENU					LGE_BASE(0x01A)	//	0x1108
#define IR_KEY_CONTENTSLINK				IR_KEY_FLASHGAME
#define IR_KEY_SIMPLINK					LGE_BASE(0x01B)	//	0x110a
#define IR_KEY_MYMEDIA					LGE_BASE(0x01C)	//	0x110b

#define DSC_IR_KEY_PWRON				LGE_BASE(0xC4)
#define DSC_IR_KEY_PWROFF				LGE_BASE(0xC5)
#define DSC_IR_KEY_ARC4X3				LGE_BASE(0x76)
#define DSC_IR_KEY_ARC16X9				LGE_BASE(0x77)
#define DSC_IR_KEY_ARCZOOM				LGE_BASE(0xAF)
#define DSC_IR_KEY_TV					LGE_BASE(0xD6)
//#define DSC_IR_KEY_CATV				0xF0				/* 임시 DSC key 할당(CATV,DTV,CADTV,HDMI2 ) */
#define DSC_IR_KEY_DTV					LGE_BASE(0xF1)
#define DSC_IR_KEY_CADTV				LGE_BASE(0xF2)
#define DSC_IR_KEY_VIDEO1				LGE_BASE(0x5A)
#define DSC_IR_KEY_VIDEO2				LGE_BASE(0xD0)
#define DSC_IR_KEY_VIDEO3				LGE_BASE(0xD1)
#define DSC_IR_KEY_COMP1				LGE_BASE(0xBF)
#define DSC_IR_KEY_COMP2				LGE_BASE(0xD4)
#define DSC_IR_KEY_COMP3				LGE_BASE(0xD9)				/* stonedef - 091121, Component 3 Hotkey */
#define DSC_IR_KEY_RGBPC				LGE_BASE(0xD5)
#define DSC_IR_KEY_RGBDTV				LGE_BASE(0xD7)
#define DSC_IR_KEY_RGBDVI				LGE_BASE(0xC6)
#define DSC_IR_KEY_HDMI1				LGE_BASE(0xCE)
#define DSC_IR_KEY_HDMI2				LGE_BASE(0xCC)
//기판 모드 대응 see _SUMODE_CircuitModeKeyHandler()
#define DSC_IR_KEY_HDMI3				LGE_BASE(0xE9)
#define DSC_IR_KEY_HDMI4				LGE_BASE(0xDA)
#define DSC_IR_KEY_VOL30				LGE_BASE(0x85)				/* VOL  30 */
#define DSC_IR_KEY_VOL50				LGE_BASE(0x86)				/* VOL  50 */
#define DSC_IR_KEY_VOL80				LGE_BASE(0x84)				/* VOL  80 */
#define DSC_IR_KEY_VOL100				LGE_BASE(0x87)				/* VOL 100 */
#define DSC_IR_KEY_SUBSTRATE			LGE_BASE(0x88)				/* 기판 Mode 진입 */
#define DSC_IR_KEY_MULTI_PIP			LGE_BASE(0x70)				/* 기판 Mode 초기화 기능 추가 */
#define DSC_IR_KEY_WB_MODE  			LGE_BASE(0x69)				/* WB Mode 진입 */
#define DSC_IR_KEY_POWERONLY_OFF		LGE_BASE(0xF8)				/* PowerOnly 해제 */

/* 아래 키 들은 자가 진단을 위해 사용함*/
#define DSC_IR_KEY_SELF_DIAG_00			LGE_BASE(0x68)				/* 자가 진단 Group 1, 2, 3 */
#define DSC_IR_KEY_SELF_DIAG_10			LGE_BASE(0x6A)				/* 자가 진단 Group 1*/
#define DSC_IR_KEY_SELF_DIAG_20			LGE_BASE(0x6D)				/* 자가 진단 Group 2*/
#define DSC_IR_KEY_SELF_DIAG_30			LGE_BASE(0x6E)				/* 자가 진단 Group 3*/
#define DSC_IR_KEY_SELF_DIAG_11			LGE_BASE(0x6F)				/* 자가 진단 AV */
#define DSC_IR_KEY_SELF_DIAG_12			LGE_BASE(0xA4)				/* 자가 진단 component */
#define DSC_IR_KEY_SELF_DIAG_13			LGE_BASE(0xA5)				/* 자가 진단 RGB */
#define DSC_IR_KEY_SELF_DIAG_21			LGE_BASE(0xAD)				/* 자가 진단 HDMI1 */
#define DSC_IR_KEY_SELF_DIAG_22			LGE_BASE(0xBC)				/* 자가 진단 HDMI2 */
#define DSC_IR_KEY_SELF_DIAG_23			LGE_BASE(0xBE)				/* 자가 진단 HDMI3 */
#define DSC_IR_KEY_SELF_DIAG_24			LGE_BASE(0x31)				/* 자가 진단 HDMI4 */
#define DSC_IR_KEY_SELF_DIAG_25			LGE_BASE(0x25)				/* 자가 진단 HDMI5 */
#define DSC_IR_KEY_SELF_DIAG_31			LGE_BASE(0x31)				/* 자가 진단 RF - 사용 안함  */
#define DSC_IR_KEY_SELF_DIAG_32			IR_KEY_SCR_KEYBOARD /*자가 진단 DTV - 사용 안함 */

#ifdef INCLUDE_HOTEL_MODE
#define IR_KEY_AUTOCONFIGURE			LGE_BASE(0x99)				/* Auto Configuration*/
#endif

#ifdef INCLUDE_WIRELESS_READY
/************************************************************
	Virtual Keys for media box
************************************************************/
// TODO: 추후 media box 터치키가 추가되면 여기서 할 것.
#define WL_VIRKEY_AV1					0x4010
#define WL_VIRKEY_AV2					0x4011
#define WL_VIRKEY_COMP1					0x4012
#define WL_VIRKEY_COMP2					0x4013
#define WL_VIRKEY_RGB					0x4014
#define WL_VIRKEY_HDMI1					0x4015
#define WL_VIRKEY_HDMI2					0x4016
#define WL_VIRKEY_HDMI3					0x4017
#define WL_VIRKEY_HDMI4					0x4018
#endif

/************************************************************
	Virtual Keys for Commercial
************************************************************/
#define IR_VIRKEY_ASPECTRATIO			0x5000

#define IR_VIRKEY_CONTRAST				0x5004
#define IR_VIRKEY_BRIGHTNESS			0x5005
#define IR_VIRKEY_COLOR					0x5006
#define IR_VIRKEY_SHARPNESS				0x5007
#define IR_VIRKEY_COLORTEMP				0x5008
#define IR_VIRKEY_TINT					0x5009
#define IR_VIRKEY_TREBLE				0x500A
#define IR_VIRKEY_BASS					0x500B
#define IR_VIRKEY_BALANCE				0x500C

#define IR_VIRKEY_DVIPC					0x500D
#define IR_VIRKEY_DTV					0x500E

#define IR_VIRKEY_PIP					0x500F
#define IR_VIRKEY_DW					0x5010

#define IR_VIRKEY_ISM_METHOD			0x5011
#define IR_VIRKEY_ORBITER_TIME			0x5012
#define IR_VIRKEY_ORBITER_PIXEL			0x5013
#define IR_VIRKEY_KEY_LOCK				0x5014
#define IR_VIRKEY_OSD_SELECT			0x5015

#define IR_VIRKEY_ANTENNA				0x5016
#define IR_VIRKEY_CABLE					0x5017
#define IR_VIRKEY_LOWPOWER				0x5019
#define IR_VIRKEY_ADDDELETE				0x5020
#define IR_VIRKEY_EYEQGREEN				0x5021
#define IR_VIRKEY_VOLUME				0x5022
#define IR_VIRKEY_AUTOCONFIG			0x5023

#define IR_VIRKEY_RED					0x5028
#define IR_VIRKEY_BLUE					0x5029
#define IR_VIRKEY_GREEN					0x5030
#define IR_VIRKEY_EQ					0x5031
#define IR_VIRKEY_BACKLIGHT				0x5032

#ifdef USE_OAD
#define IR_VIRKEY_OAD					0x5026
#endif
#define IR_VIRKEY_BAD_CABLE_SIGNAL		0x5027
/*hdcho Text hold 기능 수행위한 key*/
#define IR_VIRKEY_TEXTHOLD				0x5028
/*ymseo Rec Key valid check 없이 instant rec 진입을 위한 key*/
#define IR_VIRKEY_REC					0x5033

#ifdef INCLUDE_HOTEL_MODE
#define IR_VIRKEY_3D_ONOFF				0x5034
#define IR_VIRKEY_3D_3Dto2D				0x5035
#define IR_VIRKEY_3D_2Dto3D				0x5036
#define IR_VIRKEY_3DOPT_LR				0x5037
#define IR_VIRKEY_3DOPT_DEPTH			0x5038
#define IR_VIRKEY_3DOPT_VIEWPOINT		0x5039
#define IR_VIRKEY_3DOPT_OPTIMIZATION	0x5040
#define IR_VIRKEY_3DOPT_SIZE			0x5041
#define IR_VIRKEY_3DOPT_BALANCE			0x5042
#endif

//hojin.koh_110219 - Add Japan Model two byte key code definition.
//IR_KEY_HOTELMODE, IR_KEY_HOTELMODE_READY 를 Virtual Key Code로 변경.
#define IR_KEY_HOTELMODE				LGE_BASE(0x03B) 	//	0x05CF
#define IR_KEY_HOTELMODE_READY			LGE_BASE(0x03C) 	//	0x0523


//--------------------------------------------------//
//			Magic Motion Remote RF Key 				//
//--------------------------------------------------//
//#define RF_KEY_NUM 						14

#define MM_RF_KEY_NONE					0x0000
#define MM_RF_KEY_POWER					0x0080
#define MM_RF_KEY_MUTE					0x0001
#define MM_RF_KEY_OK					0x0002
#define MM_RF_KEY_VOL_UP				0x0004
#define MM_RF_KEY_VOL_DOWN				0x0008
#define MM_RF_KEY_CH_UP					0x0010
#define MM_RF_KEY_CH_MULTI				0x0020
#define MM_RF_KEY_CH_DOWN				0x0040

#define MM_RF_KEY_UP					0x00F1
#define MM_RF_KEY_DOWN					0x00F2
#define MM_RF_KEY_LEFT					0x00F3
#define MM_RF_KEY_RIGHT					0x00F4
#define MM_RF_KEY_MENU					0x00F5

//--------------------------------------------------//
//			Magic Motion Remote UI Key 				//
//--------------------------------------------------//
#define RF_KEY_NONE 					0x0000
#define RF_KEY_MUTE						0x0100
#define RF_KEY_OK						0x0200
#define RF_KEY_VOL_UP 					0x0400
#define RF_KEY_VOL_DOWN 				0x0800
#define RF_KEY_CH_UP 					0x1000
#define RF_KEY_CH_MULTI					0x2000
#define RF_KEY_CH_DOWN 					0x4000
#define RF_KEY_MENU						0xF500

#define RF_KEY_UP						0x0001
#define RF_KEY_DOWN						0x0002
#define RF_KEY_DRAG						0x0004
#define RF_KEY_REPEAT					0x0008

#define RF_KEY_MUTE_UP					(RF_KEY_MUTE | RF_KEY_UP)
#define RF_KEY_MUTE_DOWN				(RF_KEY_MUTE | RF_KEY_DOWN)

#define RF_KEY_OK_DOWN					(RF_KEY_OK | RF_KEY_DOWN)
#define RF_KEY_OK_UP					(RF_KEY_OK | RF_KEY_UP)
#define RF_KEY_OK_DRAG					(RF_KEY_OK | RF_KEY_DRAG)
#define RF_KEY_OK_REPEAT				(RF_KEY_OK | RF_KEY_DRAG | RF_KEY_REPEAT)
#define RF_KEY_VOL_UP_DOWN				(RF_KEY_VOL_UP | RF_KEY_DOWN)
#define RF_KEY_VOL_UP_UP				(RF_KEY_VOL_UP | RF_KEY_UP)

#define RF_KEY_VOL_DOWN_DOWN			(RF_KEY_VOL_DOWN | RF_KEY_DOWN)
#define RF_KEY_VOL_DOWN_UP				(RF_KEY_VOL_DOWN | RF_KEY_UP)

#define RF_KEY_CH_UP_DOWN				(RF_KEY_CH_UP | RF_KEY_DOWN)
#define RF_KEY_CH_UP_UP					(RF_KEY_CH_UP | RF_KEY_UP)

#define RF_KEY_CH_MULTI_DOWN			(RF_KEY_CH_MULTI | RF_KEY_DOWN)
#define RF_KEY_CH_MULTI_UP				(RF_KEY_CH_MULTI | RF_KEY_UP)

#define RF_KEY_CH_DOWN_DOWN				(RF_KEY_CH_DOWN | RF_KEY_DOWN)
#define RF_KEY_CH_DOWN_UP				(RF_KEY_CH_DOWN | RF_KEY_UP)

#define CURSOR_ON_INIT					(0x8000 | 0x0000)
#define RF_KEY_GESTURECH_DOWN			(0x8000 | RF_KEY_DOWN)
#define RF_KEY_GESTURECH_UP

#else //INCLUDE_ARCHITECTURE_1G


/*	KEYs definition */
#define IR_KEY_POWER					0x08				/*  전원  */
#define IR_KEY_TV						0x0F				/*  TV */
#define IR_KEY_TV_VIDEO 				0x0B				/*  TV/외부입력 */
#define IR_KEY_COMP_RGB_HDMI			0x98				/*  멀티미디어 : 지원 안함 */
#define IR_KEY_STORE_POWER				0x33				/* Store Power On */
#define IR_KEY_LOCAL_DIMMING			0x36				/* Local Dimming Demo */

#define IR_KEY_0						0x10				/*  숫자 0 */
#define IR_KEY_1						0x11				/*  숫자 1 */
#define IR_KEY_2						0x12				/*  숫자 2 */
#define IR_KEY_3						0x13				/*  숫자 3 */
#define IR_KEY_4						0x14				/*  숫자 4 */
#define IR_KEY_5						0x15				/*  숫자 5 */
#define IR_KEY_6						0x16				/*  숫자 6 */
#define IR_KEY_7						0x17				/*  숫자 7 */
#define IR_KEY_8						0x18				/*  숫자 8 */
#define IR_KEY_9						0x19				/*  숫자 9 */
#define IR_KEY_DASH 					0x4C				/*  Dash(-) */
#define IR_KEY_FLASHBACK				0x1A				/*  이전채널 */

#define IR_KEY_CH_UP					0x00				/*  채널 + */
#define IR_KEY_CH_DOWN					0x01				/*  채널 - */
#define IR_KEY_VOL_UP					0x02				/*  음량 + */
#define IR_KEY_VOL_DOWN 				0x03				/*  음량 - */

#define IR_KEY_HOME 					0x7C				/*  홈메뉴 */
#define IR_KEY_HOME_US 					0xC8				/*  홈메뉴 */
#define IR_KEY_MUTE 					0x09				/*  조용히 */
#define IR_KEY_FAVORITE 				0x1E				/*  선호채널  */

#define IR_KEY_MENU 					0x43				/*  메뉴 */
#define IR_KEY_GRIDGUIDE				0xA9				/*  전체방송 */
#define IR_KEY_INFO 					0xAA				/*  현재방송 */
#define IR_KEY_EXIT 					0x5B				/*  취소 */

#define IR_KEY_UP_ARROW 				0x40				/*  UP	 */
#define IR_KEY_DOWN_ARROW				0x41				/*  DOWN  */
#define IR_KEY_LEFT_ARROW				0x07				/*  MINUS */
#define IR_KEY_RIGHT_ARROW				0x06				/*  PLUS  */
#define IR_KEY_ENTER					0x44				/*  확인   */

#define IR_KEY_RESLIST 					0x89				/*  예약목록 */

#define IR_KEY_MARK 					0x7D				/*  마크 */
#define IR_KEY_LIVETV					0x9E				/*  Live TV */
#define IR_KEY_HCEC						0x7E

//#define IR_KEY_INFORMATION			0x1B				/* Information(EU) */
#define IR_KEY_TELETEXT 				0x20				/* Teletext(EU) */
#define IR_KEY_TEXTOPTION 				0x21				/* Teletext(EU) */
#define IR_KEY_TEXTMODE					0x22				/* Text mode(List) */
#define IR_KEY_TEXTMIX					0x24				/* Text mix */
#define IR_KEY_TEXTSUBPAGE				0x26				/* Text subpage(time) */
#define IR_KEY_BACK						0x28				/* Back */
#define IR_KEY_TEXTREVEAL				0x2A				/* Text Reveal */
//#define IR_KEY_TEXTHOLD					0x65				/* ext Hold */
//hdcho
#define IR_KEY_FREEZE					0x65				/* Freeze  */
#define IR_KEY_DA						0x50				/* Digital/Analog TV */
#define IR_KEY_PRLIST					0x53				/* Program List */
//#define IR_KEY_SUBTITLE				0x56				/* Subtitle ( It will be combine with 0x39) */
#define IR_KEY_DIGITALSETUP				0x5F				/* Digital Setup */
#define IR_KEY_POSITION					0x62				/* PIP Position/Text Update */
#define IR_KEY_SIZE						0x64				/* Text Size */
#define IR_KEY_GUIDEPAL					0xAB				/* Guide(For PAL TV) */
#define IR_KEY_TV_RADIO					0XF0
#define IR_KEY_QMENU					0x45

#define IR_KEY_PIP						0x60				/* 동시 화면 */
#define IR_KEY_PIP_CH_UP				0x71				/* 부화면+ */
#define IR_KEY_PIP_CH_DOWN				0x72				/* 부화면- */
#define IR_KEY_PIP_INPUT				0x61				/* 부입력 */

#define IR_KEY_TIMER					0x0E				/* 취침예약 */
#define IR_KEY_XD						0x92				/* XD키  */
#define IR_KEY_ASPECT_RATIO 			0x79				/* 화면크기 */
#define IR_KEY_SWAP 					0x63				/* 주부전환 */

#define IR_KEY_SAP						0x0A				/* 음성다중 */
#define IR_KEY_CC						0x39				/* 자막방송 */
#define IR_KEY_EZPIC					0x4D				/* 자동영상  */
#define IR_KEY_EZSOUND					0x52				/* 자동음성 */

#define IR_KEY_ADJUST					0xCB				/* 조정메뉴  */
#define IR_KEY_EJECT					0xCA				/* M/C 꺼내기  */
#define IR_KEY_DWTS						0x67				/* Delayed/Sync TS */

#define IR_KEY_MHEG						0x9F				/* 데이터 방송 키 */
#ifndef _EMUL_WIN
#ifdef INCLUDE_ACAP
#define IR_KEY_ACAP						0x9F				/* 데이터 방송 키 */
#endif
#else
#define IR_KEY_ACAP						0x9F				/* 데이터 방송 키 */
#endif

#ifdef INCLUDE_MHP
#define IR_KEY_MHP						0x9F				/* 데이터 방송 키 */
#endif

#define IR_KEY_SCR_KEYBOARD				0x32				/* screen keboard */
#define IR_KEY_AUTODEMO 				0x80
#define IR_KEY_AUTOSCAN 				0x54
#define IR_KEY_CHADDDEL 				0x55

#define IR_KEY_PLAY 					0xB0				/* 재생 */
#define IR_KEY_PAUSE					0xBA				/* 일시정지 */
#define IR_KEY_STOP 					0xB1				/* 정지 */
#define IR_KEY_REC						0xBD				/* 녹화 */

#define IR_KEY_REW						0x8F   			/* 되감기	*/
#define IR_KEY_FF						0x8E           	/* 빨리감기 */
#define IR_KEY_GOTOPREV 				0xB2				/* Skip Backward */
#define IR_KEY_GOTONEXT 				0xB3				/* Skip Forward */

#ifdef INCLUDE_EMANUAL
#define IR_KEY_EMANUAL_PAGEPREV			0xB4				/* e-manual page forward    */
#define IR_KEY_EMANUAL_PAGENEXT			0xB5				/* e-manual page backword */
#endif

#ifdef INCLUDE_MOUSE
//#define IR_KEY_MOUSE_MENU				0x3A
#define IR_KEY_MOUSE_OK					0x29
#define IR_KEY_MOUSE_MCH				0x2B
#define IR_KEY_PAIRING_START			0x93
#define IR_KEY_PAIRING_STOP				0x94
#endif

#define IR_KEY_3D 						0xDB	//by hwangbos 20091214 IR_KEY_PAIRING_START	//0xDB				/* 3D on/off */
#define IR_KEY_3D_MODE					0xDC	//by hwangbos 20091214 IR_KEY_PAIRING_STOP		//0xDC				/* 3D mode */
#define IR_KEY_3D_LR					0xDD					/* 3D L/R */


//	Broadband 관련
#define IR_KEY_YAHOO					0x58	/* NETCAST */
#define IR_KEY_NETCAST					0x59	/* YAHOO */

//Color Key 대응 by athens
#define IR_KEY_RED						IR_KEY_PIP_CH_DOWN
#define IR_KEY_GREEN					IR_KEY_PIP_CH_UP
#define IR_KEY_YELLOW					IR_KEY_SWAP
#define IR_KEY_BLUE 					IR_KEY_PIP_INPUT

#ifdef _EMUL_WIN
//Ch Edit에서 Color key대응
//Lock 메뉴에서 Block channel 에 Green color key 추가함
#define IR_KEY_CHEDIT_AUTO_SORT			IR_KEY_GREEN
//#define IR_KEY_CHEDIT_DEL				IR_KEY_GREEN
#define IR_KEY_CHEDIT_MOVE				IR_KEY_YELLOW
#define IR_KEY_CHEDIT_SKIP          	IR_KEY_BLUE

// Delete key
#define IR_KEY_LOCK_DEL					IR_KEY_RED
#endif

#define IR_KEY_MODE						IR_KEY_RED			/* R */
#define IR_KEY_TIMER_LIST				IR_KEY_BLUE			/* B */
#define IR_KEY_MANUAL_TIMER				IR_KEY_YELLOW		/* Y */
#define IR_KEY_DATE 					IR_KEY_GREEN		/* N */

/* TV extension key */
#define IR_KEY_PAGE_UP					IR_KEY_CH_UP
#define IR_KEY_PAGE_DOWN				IR_KEY_CH_DOWN
#define IR_KEY_PAGE_RIGHT				IR_KEY_SYNC
#define IR_KEY_PAGE_LEFT				IR_KEY_MV2START
#define IR_KEY_FORMAT_CHANGE			0x10E1
//#define IR_KEY_FREEZE					0x1010
#define IR_KEY_GUIDE					IR_KEY_GRIDGUIDE

/* DVR extension key */
#define IR_KEY_MV2START 				IR_KEY_GOTOPREV		/* 처음 보기 */
#define IR_KEY_SYNC 					IR_KEY_GOTONEXT		/* 끝 보기 */
#define IR_KEY_RESUME					IR_KEY_PLAY
#define IR_KEY_PAUSE_SLOWFWD			IR_KEY_PAUSE
#define IR_KEY_SLOW_FORWARD				0x11B0
#define IR_KEY_IREPLAY					IR_KEY_GOTOPREV
#define IR_KEY_SKIPFWD					IR_KEY_GOTONEXT
#define IR_KEY_TIME_SHIFT				0x11BB				/* 타임시프트 */

#define IR_KEY_RECLIST					IR_KEY_GREEN 		/* 한국향 dvr ready에서 green키를 reclist (녹화목록) 키로 이용함. */
#define IR_KEY_REPEAT					IR_KEY_BLUE 		/* 한국향 dvr ready에서 blue키를 repeat (구간반복) 키로 이용함. */	// IR_KEY_MARK
#ifdef INCLUDE_DVR_READY_EDT
#define IR_KEY_EDIT						IR_KEY_YELLOW 		/* GP2 dvr ready에서 YELLOW키를 edit (구간편집) 키로 이용함. */
#endif
#if 0 /* delete unused dvr key. by jihyeon@20080508 */
#define IR_KEY_RESREGIST				0x118A
#endif


/* 화면 밝기 */
#define IR_KEY_BRIGHTNESS_UP			0xE0				/* 화면밝기+ */
#define IR_KEY_BRIGHTNESS_DOWN			0xE1				/* 화면밝기- */

#define IR_KEY_GAMEMODE					0x30				/* game mode - 070901 */

#define IR_KEY_AD						0x91				/* Audio Description - 091109 */
#define IR_KEY_FAMILY					0x2E				/* Family Mode - 	100323 */
#define IR_KEY_CRICKET					0x3A				/* Cricket Mode - 	100323 */

//Japan Number Key - hojin.koh_110219 - Add Japan Model two byte key code definition.
#define IR_KEY_BS						0xDB00
#define IR_KEY_BS_NUM_1				0xDB01
#define IR_KEY_BS_NUM_2				0xDB02
#define IR_KEY_BS_NUM_3				0xDB03
#define IR_KEY_BS_NUM_4				0xDB04
#define IR_KEY_BS_NUM_5				0xDB05
#define IR_KEY_BS_NUM_6				0xDB06
#define IR_KEY_BS_NUM_7				0xDB07
#define IR_KEY_BS_NUM_8				0xDB08
#define IR_KEY_BS_NUM_9				0xDB09
#define IR_KEY_BS_NUM_10				0xDB0A
#define IR_KEY_BS_NUM_11				0xDB0B
#define IR_KEY_BS_NUM_12				0xDB0C

#define IR_KEY_CS1					0xDB10
#define IR_KEY_CS1_NUM_1				0xDB11
#define IR_KEY_CS1_NUM_2				0xDB12
#define IR_KEY_CS1_NUM_3				0xDB13
#define IR_KEY_CS1_NUM_4				0xDB14
#define IR_KEY_CS1_NUM_5				0xDB15
#define IR_KEY_CS1_NUM_6				0xDB16
#define IR_KEY_CS1_NUM_7				0xDB17
#define IR_KEY_CS1_NUM_8				0xDB18
#define IR_KEY_CS1_NUM_9				0xDB19
#define IR_KEY_CS1_NUM_10			0xDB1A
#define IR_KEY_CS1_NUM_11			0xDB1B
#define IR_KEY_CS1_NUM_12			0xDB1C

#define IR_KEY_CS2					0xDB20
#define IR_KEY_CS2_NUM_1				0xDB21
#define IR_KEY_CS2_NUM_2				0xDB22
#define IR_KEY_CS2_NUM_3				0xDB23
#define IR_KEY_CS2_NUM_4				0xDB24
#define IR_KEY_CS2_NUM_5				0xDB25
#define IR_KEY_CS2_NUM_6				0xDB26
#define IR_KEY_CS2_NUM_7				0xDB27
#define IR_KEY_CS2_NUM_8				0xDB28
#define IR_KEY_CS2_NUM_9				0xDB29
#define IR_KEY_CS2_NUM_10			0xDB2A
#define IR_KEY_CS2_NUM_11			0xDB2B
#define IR_KEY_CS2_NUM_12			0xDB2C


#define IR_KEY_TER					0xDB30
#define IR_KEY_TER_NUM_1				0xDB31
#define IR_KEY_TER_NUM_2				0xDB32
#define IR_KEY_TER_NUM_3				0xDB33
#define IR_KEY_TER_NUM_4				0xDB34
#define IR_KEY_TER_NUM_5				0xDB35
#define IR_KEY_TER_NUM_6				0xDB36
#define IR_KEY_TER_NUM_7				0xDB37
#define IR_KEY_TER_NUM_8				0xDB38
#define IR_KEY_TER_NUM_9				0xDB39
#define IR_KEY_TER_NUM_10			0xDB3A
#define IR_KEY_TER_NUM_11			0xDB3B
#define IR_KEY_TER_NUM_12			0xDB3C

#define IR_KEY_3DIGIT_INPUT			0x23 //Japan Only - newly added
#define IR_KEY_BML_DATA				0x9F //Japan Only - newly added
#define IR_KEY_JAPAN_DISPLAY		0xAC //Japan Only - newly added


#ifdef INCLUDE_DVR_READY
/* T-CON */
#define IR_KEY_TCON_UP					0x30F0	/* T-CON Virtual Key */
#define IR_KEY_TCON_UPRIGHT				0x30F1	/* T-CON Virtual Key */
#define IR_KEY_TCON_RIGHT				0x30F2	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWNRIGHT			0x30F3	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWN				0x30F4	/* T-CON Virtual Key */
#define IR_KEY_TCON_DOWNLEFT			0x30F5	/* T-CON Virtual Key */
#define IR_KEY_TCON_LEFT				0x30F6	/* T-CON Virtual Key */
#define IR_KEY_TCON_UPLEFT				0x30F7	/* T-CON Virtual Key */
#endif

#define IR_KEY_LEFT_SCROLL_1ST			0xA0 	/* T-CON 좌1*/
#define IR_KEY_LEFT_SCROLL_2ND			0xA1 	/* T-CON 좌2*/
#define IR_KEY_LEFT_SCROLL_3RD			0xA2 	/* T-CON 좌3*/
#define IR_KEY_LEFT_SCROLL_4TH			0xA3 	/* T-CON 좌4*/
#define IR_KEY_LEFT_SCROLL_5TH			0xA7 	/* T-CON 좌5*/
#define IR_KEY_RIGHT_SCROLL_1ST			0xB4 	/* T-CON 우1*/
#define IR_KEY_RIGHT_SCROLL_2ND			0xB5 	/* T-CON 우2*/
#define IR_KEY_RIGHT_SCROLL_3RD			0xB7 	/* T-CON 좌3*/
#define IR_KEY_RIGHT_SCROLL_4TH			0xB9 	/* T-CON 좌4*/
#define IR_KEY_RIGHT_SCROLL_5TH			0xBB 	/* T-CON 좌5*/

/* factory key */
#define IR_KEY_EYE_Q					0x95
#define IR_KEY_IN_STOP         			0xFA
#define IR_KEY_IN_START        			0xFB
#define IR_KEY_P_CHECK         			0xFC
#define IR_KEY_HDMI_CHECK				0xA6//[081020 leekyungho] : IR_KEY_HDMI_CHECK(0xA6) 기능 UI 대응
#define IR_KEY_S_CHECK         			0xFD
#define IR_KEY_POWERONLY   				0xFE
#define IR_KEY_ADJ	   					0xFF
#define IR_KEY_EZ_ADJUST        		IR_KEY_ADJ
#define IR_KEY_FRONT_AV	   				0x51
#define IR_KEY_FMODE_INIT				0x27
#define IR_KEY_FMODE_START				0xEA
#define IR_KEY_FMODE_F1					0xEB
#define IR_KEY_IN_TIME          		0x26
#define IR_KEY_LAMP_RESET				0x35				/* for KOR model */
#define IR_KEY_DISPMODE_READY			0xEC				/* for KOR model */
#define IR_KEY_DISPMODE					IR_KEY_AUTODEMO
#define IR_KEY_BLUETOOTH				0x1F
#define IR_KEY_USB_CHECK				0xEE				/* 081106 swbyun : IR_KEY_USB_CHECK(0xEE) 기능 UI 대응 */
#define IR_KEY_USB2_CHECK				0xA8				/* stonedef - 091121 : USB 2 port JPEG, MP3 자동 play */

#define IR_KEY_TILT						0xF9				/* Module Pattern Generation */

/* DMS 요구사항에 의해 추가된 Key Code로
    생산 line에서 사용하는 별도의 Remocon이 있음
    Test를 위해서 별도의 Key code를 사용해야함.*/
#define IR_KEY_POWERSAVING_TEST			0xFB				/* DMS 요구사항 반영 */

/* Factory Mode */
#define IR_VIRKEY_SHOWMSG_ADJUST		0x2021
#define IR_VIRKEY_CHANGE_SOURCE_AV1		0x2022
#define IR_VIRKEY_CHANGE_SOURCE_COMP1	0x2023
#define IR_VIRKEY_CHANGE_SOURCE_RGB 	0x2024
#define IR_VIRKEY_ADJUST_UPD			0x2025
#define IR_VIRKEY_ADJUST_AD 			0x2026				/* ADC 조정을 위해 사용 */
#define IR_VIRKEY_ADAVOSD				0x2027

/* front key */
//#define IR_FRONTKEY_AV					0x51				/* 조정 remocon key - 생산시 front AV로 전환하는데 사용 */
#define IR_FRONTKEY_INPUT_SELECT		0x10F6
#define IR_FRONTKEY_MENU				0x10F5
#define IR_FRONTKEY_ENTER				0x10F7
#define IR_FRONTKEY_CH_UP				0x10F3
#define IR_FRONTKEY_CH_DOWN				0x10F4
#define IR_FRONTKEY_VOL_UP				0x10F1
#define IR_FRONTKEY_VOL_DOWN			0x10F2
#define IR_FRONTKEY_TVGUIDE 			IR_FRONTKEY_INPUT_SELECT
#define IR_FRONTKEY_POWER				0x10F8

#define IR_KEY_MULTICHANNEL				0x10FA
#define IR_KEY_WIDGET					0x10FB
#define IR_KEY_SETUP					0x10FC
#define IR_KEY_MULTIMEDIA				0x10FD
#define IR_KEY_FLASHGAME				0x10FE
#define IR_KEY_PHOTOLIST				0x10FF
#define IR_KEY_MUSICLIST				0x1100
#define IR_KEY_MOVIELIST				0x1101
#define IR_KEY_DIRECTOSD				0x1102
#define IR_KEY_EMANUEL					0x1103
#define IR_KEY_BT_MENU					0x1104
#define IR_KEY_VIR_INPUTLIST			0x1105
#define IR_KEY_GAME_REAL_SWING			0x1106
#define IR_KEY_GAME_SUDO_SWING			0x1107
#define IR_KEY_CUBEMENU					0x1108
#define IR_KEY_CONTENTSLINK				IR_KEY_FLASHGAME
#define IR_KEY_SIMPLINK					0x110a
#define IR_KEY_MYMEDIA					0x110b
#define DSC_IR_KEY_PWRON				0xC4
#define DSC_IR_KEY_PWROFF				0xC5
#define DSC_IR_KEY_ARC4X3				0x76
#define DSC_IR_KEY_ARC16X9				0x77
#define DSC_IR_KEY_ARCZOOM				0xAF
#define DSC_IR_KEY_TV					0xD6
//#define DSC_IR_KEY_CATV				0xF0				/* 임시 DSC key 할당(CATV,DTV,CADTV,HDMI2 ) */
#define DSC_IR_KEY_DTV					0xF1
#define DSC_IR_KEY_CADTV				0xF2
#define DSC_IR_KEY_VIDEO1				0x5A
#define DSC_IR_KEY_VIDEO2				0xD0
#define DSC_IR_KEY_VIDEO3				0xD1
#define DSC_IR_KEY_COMP1				0xBF
#define DSC_IR_KEY_COMP2				0xD4
#define DSC_IR_KEY_COMP3				0xD9				/* stonedef - 091121, Component 3 Hotkey */
#define DSC_IR_KEY_RGBPC				0xD5
#define DSC_IR_KEY_RGBDTV				0xD7
#define DSC_IR_KEY_RGBDVI				0xC6
#define DSC_IR_KEY_HDMI1				0xCE
#define DSC_IR_KEY_HDMI2				0xCC
//기판 모드 대응 see _SUMODE_CircuitModeKeyHandler()
#define DSC_IR_KEY_HDMI3				0xE9
#define DSC_IR_KEY_HDMI4				0xDA
#define DSC_IR_KEY_VOL30				0x85				/* VOL  30 */
#define DSC_IR_KEY_VOL50				0x86				/* VOL  50 */
#define DSC_IR_KEY_VOL80				0x84				/* VOL  80 */
#define DSC_IR_KEY_VOL100				0x87				/* VOL 100 */
#define DSC_IR_KEY_SUBSTRATE			0x88				/* 기판 Mode 진입 */
#define DSC_IR_KEY_MULTI_PIP			0x70				/* 기판 Mode 초기화 기능 추가 */
#define DSC_IR_KEY_WB_MODE  			0x69				/* WB Mode 진입 */
#define DSC_IR_KEY_POWERONLY_OFF		0xF8				/* PowerOnly 해제 */

/* 아래 키 들은 자가 진단을 위해 사용함*/
#define DSC_IR_KEY_SELF_DIAG_00			0x68				/* 자가 진단 Group 1, 2, 3 */
#define DSC_IR_KEY_SELF_DIAG_10			0x6A				/* 자가 진단 Group 1*/
#define DSC_IR_KEY_SELF_DIAG_20			0x6D				/* 자가 진단 Group 2*/
#define DSC_IR_KEY_SELF_DIAG_30			0x6E				/* 자가 진단 Group 3*/
#define DSC_IR_KEY_SELF_DIAG_11			0x6F				/* 자가 진단 AV */
#define DSC_IR_KEY_SELF_DIAG_12			0xA4				/* 자가 진단 component */
#define DSC_IR_KEY_SELF_DIAG_13			0xA5				/* 자가 진단 RGB */
#define DSC_IR_KEY_SELF_DIAG_21			0xAD				/* 자가 진단 HDMI1 */
#define DSC_IR_KEY_SELF_DIAG_22			0xBC				/* 자가 진단 HDMI2 */
#define DSC_IR_KEY_SELF_DIAG_23			0xBE				/* 자가 진단 HDMI3 */
#define DSC_IR_KEY_SELF_DIAG_24			0x31				/* 자가 진단 HDMI4 */
#define DSC_IR_KEY_SELF_DIAG_25			0x25				/* 자가 진단 HDMI5 */
#define DSC_IR_KEY_SELF_DIAG_31			0x31				/* 자가 진단 RF - 사용 안함  */
#define DSC_IR_KEY_SELF_DIAG_32			IR_KEY_SCR_KEYBOARD /*자가 진단 DTV - 사용 안함 */

#ifdef INCLUDE_HOTEL_MODE
#define IR_KEY_AUTOCONFIGURE			0x99				/* Auto Configuration*/
#endif

#ifdef INCLUDE_WIRELESS_READY
/************************************************************
	Virtual Keys for media box
************************************************************/
// TODO: 추후 media box 터치키가 추가되면 여기서 할 것.
#define WL_VIRKEY_AV1					0x4010
#define WL_VIRKEY_AV2					0x4011
#define WL_VIRKEY_COMP1					0x4012
#define WL_VIRKEY_COMP2					0x4013
#define WL_VIRKEY_RGB					0x4014
#define WL_VIRKEY_HDMI1					0x4015
#define WL_VIRKEY_HDMI2					0x4016
#define WL_VIRKEY_HDMI3					0x4017
#define WL_VIRKEY_HDMI4					0x4018
#endif

/************************************************************
	Virtual Keys for Commercial
************************************************************/
#define IR_VIRKEY_ASPECTRATIO			0x5000

#define IR_VIRKEY_CONTRAST				0x5004
#define IR_VIRKEY_BRIGHTNESS			0x5005
#define IR_VIRKEY_COLOR					0x5006
#define IR_VIRKEY_SHARPNESS				0x5007
#define IR_VIRKEY_COLORTEMP				0x5008
#define IR_VIRKEY_TINT					0x5009
#define IR_VIRKEY_TREBLE				0x500A
#define IR_VIRKEY_BASS					0x500B
#define IR_VIRKEY_BALANCE				0x500C

#define IR_VIRKEY_DVIPC					0x500D
#define IR_VIRKEY_DTV					0x500E

#define IR_VIRKEY_PIP					0x500F
#define IR_VIRKEY_DW					0x5010

#define IR_VIRKEY_ISM_METHOD			0x5011
#define IR_VIRKEY_ORBITER_TIME			0x5012
#define IR_VIRKEY_ORBITER_PIXEL			0x5013
#define IR_VIRKEY_KEY_LOCK				0x5014
#define IR_VIRKEY_OSD_SELECT			0x5015

#define IR_VIRKEY_ANTENNA				0x5016
#define IR_VIRKEY_CABLE					0x5017
#define IR_VIRKEY_LOWPOWER				0x5019
#define IR_VIRKEY_ADDDELETE				0x5020
#define IR_VIRKEY_EYEQGREEN				0x5021
#define IR_VIRKEY_VOLUME				0x5022
#define IR_VIRKEY_AUTOCONFIG			0x5023

#define IR_VIRKEY_RED					0x5028
#define IR_VIRKEY_BLUE					0x5029
#define IR_VIRKEY_GREEN					0x5030
#define IR_VIRKEY_EQ					0x5031
#define IR_VIRKEY_BACKLIGHT				0x5032

#ifdef USE_OAD
#define IR_VIRKEY_OAD					0x5026
#endif
#define IR_VIRKEY_BAD_CABLE_SIGNAL		0x5027
/*hdcho Text hold 기능 수행위한 key*/
#define IR_VIRKEY_TEXTHOLD				0x5028
/*ymseo Rec Key valid check 없이 instant rec 진입을 위한 key*/
#define IR_VIRKEY_REC					0x5033

#ifdef INCLUDE_HOTEL_MODE
#define IR_VIRKEY_3D_ONOFF				0x5034
#define IR_VIRKEY_3D_3Dto2D				0x5035
#define IR_VIRKEY_3D_2Dto3D				0x5036
#define IR_VIRKEY_3DOPT_LR				0x5037
#define IR_VIRKEY_3DOPT_DEPTH			0x5038
#define IR_VIRKEY_3DOPT_VIEWPOINT		0x5039
#define IR_VIRKEY_3DOPT_OPTIMIZATION	0x5040
#define IR_VIRKEY_3DOPT_SIZE			0x5041
#define IR_VIRKEY_3DOPT_BALANCE			0x5042
#endif

//hojin.koh_110219 - Add Japan Model two byte key code definition.
//IR_KEY_HOTELMODE, IR_KEY_HOTELMODE_READY 를 Virtual Key Code로 변경.
#define IR_KEY_HOTELMODE					0x05CF
#define IR_KEY_HOTELMODE_READY			0x0523


//--------------------------------------------------//
//			Magic Motion Remote RF Key 				//
//--------------------------------------------------//
//#define RF_KEY_NUM 						14

#define MM_RF_KEY_NONE					0x0000
#define MM_RF_KEY_POWER					0x0080
#define MM_RF_KEY_MUTE					0x0001
#define MM_RF_KEY_OK					0x0002
#define MM_RF_KEY_VOL_UP				0x0004
#define MM_RF_KEY_VOL_DOWN				0x0008
#define MM_RF_KEY_CH_UP					0x0010
#define MM_RF_KEY_CH_MULTI				0x0020
#define MM_RF_KEY_CH_DOWN				0x0040

#define MM_RF_KEY_UP					0x00F1
#define MM_RF_KEY_DOWN					0x00F2
#define MM_RF_KEY_LEFT					0x00F3
#define MM_RF_KEY_RIGHT					0x00F4
#define MM_RF_KEY_MENU					0x00F5

//--------------------------------------------------//
//			Magic Motion Remote UI Key 				//
//--------------------------------------------------//
#define RF_KEY_NONE 					0x0000
#define RF_KEY_MUTE						0x0100
#define RF_KEY_OK						0x0200
#define RF_KEY_VOL_UP 					0x0400
#define RF_KEY_VOL_DOWN 				0x0800
#define RF_KEY_CH_UP 					0x1000
#define RF_KEY_CH_MULTI					0x2000
#define RF_KEY_CH_DOWN 					0x4000
#define RF_KEY_MENU						0xF500

#define RF_KEY_UP						0x0001
#define RF_KEY_DOWN						0x0002
#define RF_KEY_DRAG						0x0004
#define RF_KEY_REPEAT					0x0008

#define RF_KEY_MUTE_UP					(RF_KEY_MUTE | RF_KEY_UP)
#define RF_KEY_MUTE_DOWN				(RF_KEY_MUTE | RF_KEY_DOWN)

#define RF_KEY_OK_DOWN					(RF_KEY_OK | RF_KEY_DOWN)
#define RF_KEY_OK_UP					(RF_KEY_OK | RF_KEY_UP)
#define RF_KEY_OK_DRAG					(RF_KEY_OK | RF_KEY_DRAG)
#define RF_KEY_OK_REPEAT				(RF_KEY_OK | RF_KEY_DRAG | RF_KEY_REPEAT)
#define RF_KEY_VOL_UP_DOWN				(RF_KEY_VOL_UP | RF_KEY_DOWN)
#define RF_KEY_VOL_UP_UP				(RF_KEY_VOL_UP | RF_KEY_UP)

#define RF_KEY_VOL_DOWN_DOWN			(RF_KEY_VOL_DOWN | RF_KEY_DOWN)
#define RF_KEY_VOL_DOWN_UP				(RF_KEY_VOL_DOWN | RF_KEY_UP)

#define RF_KEY_CH_UP_DOWN				(RF_KEY_CH_UP | RF_KEY_DOWN)
#define RF_KEY_CH_UP_UP					(RF_KEY_CH_UP | RF_KEY_UP)

#define RF_KEY_CH_MULTI_DOWN			(RF_KEY_CH_MULTI | RF_KEY_DOWN)
#define RF_KEY_CH_MULTI_UP				(RF_KEY_CH_MULTI | RF_KEY_UP)

#define RF_KEY_CH_DOWN_DOWN				(RF_KEY_CH_DOWN | RF_KEY_DOWN)
#define RF_KEY_CH_DOWN_UP				(RF_KEY_CH_DOWN | RF_KEY_UP)

#define CURSOR_ON_INIT					(0x8000 | 0x0000)
#define RF_KEY_GESTURECH_DOWN			(0x8000 | RF_KEY_DOWN)
#define RF_KEY_GESTURECH_UP
#endif //INCLUDE_ARCHITECTURE_1G



/*---------------------------------------------------------
    매크로 함수 정의
    (Macro Definitions)
---------------------------------------------------------*/
/*	Key Type 확인
*/
#define IS_KEY_TYPE_REMOTE( x )		(((x) == KEY_TYPE_REMOTE_SINGLE) || ((x) == KEY_TYPE_REMOTE_REPEAT))
#define IS_KEY_TYPE_LOCAL( x )		(((x) == KEY_TYPE_LOCAL_SINGLE ) || ((x) == KEY_TYPE_LOCAL_REPEAT ))
#define	IS_KEY_TYPE_SINGLE( x )		(((x) == KEY_TYPE_REMOTE_SINGLE) || ((x) == KEY_TYPE_LOCAL_SINGLE ))
#define	IS_KEY_TYPE_REPEAT( x )		(((x) == KEY_TYPE_REMOTE_REPEAT) || ((x) == KEY_TYPE_LOCAL_REPEAT ))

#ifdef INCLUDE_REMOTESVC
#define	IS_KEY_TYPE_REMOTESVC( x )	(((x) == KEY_TYPE_REMOTESVC))
#endif

/*---------------------------------------------------------
    Type 정의
    (Type Definitions)
---------------------------------------------------------*/

/**
 * 	KEY(event) Type
 *
 * @see	the 2nd paramter of UI_KEY_RESULT_T UI_MAIN_SendKeyToUI( UINT32 key, UINT32 keyType );
 */
/**
 * bk1472(최배권 책임 comment -
 * enumeration type에서는 platform 별 system 별 분기가 필요 없도록
 * 작성 바랍니다.
 */
typedef enum
{
	/*	KEY TYPE	definition */
	KEY_TYPE_REMOTE_SINGLE		=	0x0000,	/**< KEY TYPE:	remocom single	*/
	KEY_TYPE_REMOTE_REPEAT		=	0x0001,	/**< KEY TYPE:	remocom repeat	*/
	KEY_TYPE_LOCAL_SINGLE		=	0x0002,	/**< KEY TYPE:	local single	*/
	KEY_TYPE_LOCAL_REPEAT		=	0x0003,	/**< KEY TYPE:	local repeat	*/
	KEY_TYPE_COMMAND			=	0x0004, /**< KEY TYPE:	RS232C command*/
#ifdef INCLUDE_BUILT_IN_LOG
	KEY_TYPE_RF_SINGLE			= 	0x0005, /**< KEY TYPE:	RF single */
	KEY_TYPE_RF_REPEAT			= 	0x0006, /**< KEY TYPE:	RF single */
#endif

#ifdef INCLUDE_MOUSE
	KEY_TYPE_MOUSE_OFF_SINGLE	=   0x0007, /**< KEY TYPE:	mouse off IR Key single */
#endif

#ifdef INCLUDE_REMOTESVC
	KEY_TYPE_REMOTESVC			=   0x0008, /**< KEY TYPE:	for remote service */
#endif

	KEY_TYPE_LAST							/**< KEY TYPE:	last value		*/

}	KEY_TYPE_T;

/*---------------------------------------------------------
    함수 선언
    (Function Declaration)
---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif  /* _REMOTE_KEY_H_ */
