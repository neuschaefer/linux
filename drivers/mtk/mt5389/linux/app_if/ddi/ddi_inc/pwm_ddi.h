/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file pwm_ddi.h
 *
 *  This header file declares the data types and DDI functions to control the gpio ports
 *
 *  @author		earnest
 *  @version	1.0
 *  @date		2007.08.01
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _PWM_DDI_H_
#define _PWM_DDI_H_

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "cmnio_ddi.h"

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/
#define	TEST_PWM_FUNCTION	//KWON_0723

#ifndef TEST_PWM_FUNCTION	//KWON_0717	기존 value 최종 확인 후 삭제해야 함.
#define PWM_VBR_A_INIT_VALUE			0
#define PWM_VBR_B_INIT_VALUE			0

#define PWM_VBR_A_INIT_END_VALUE    	0x7b
#define PWM_VBR_B_INIT_END_VALUE		0xff

#if (PLATFORM_TYPE == BCM_PLATFORM)
#define PWM_VBR_A_INIT_FREQUENCY_VALUE  53000	//150Hz
#define PWM_VBR_B_INIT_FREQUENCY_VALUE  120		//150Hz ->100hz
#define PWM_PORT_VBR_A					PWM_DEV_PIN0
#define PWM_PORT_VBR_B					PWM_DEV_PIN1
#else
#define PWM_VBR_A_INIT_FREQUENCY_VALUE  100		//150Hz
#define PWM_VBR_B_INIT_FREQUENCY_VALUE  100		//150Hz ->100hz
#define PWM_PORT_VBR_A					PWM_DEV_PIN0	//KWON_0819	PWM_DEV_PIN2
#define PWM_PORT_VBR_B					PWM_DEV_PIN1	//KWON_0819	PWM_DEV_PIN3
#endif

#else	//for #ifndef TEST_PWM_FUNCTION

#if (PLATFORM_TYPE == BCM_PLATFORM)
#define PWM_VBR_A_INIT_VALUE			0
#define PWM_VBR_B_INIT_VALUE			0

#define PWM_VBR_A_INIT_END_VALUE    	0x7b
#define PWM_VBR_B_INIT_END_VALUE		0xff

#define PWM_VBR_A_INIT_FREQUENCY_VALUE  53000	//150Hz
#define PWM_VBR_B_INIT_FREQUENCY_VALUE  120		//150Hz ->100hz
#define PWM_PORT_VBR_A					PWM_DEV_PIN0
#define PWM_PORT_VBR_B					PWM_DEV_PIN1
#else
#define PWM_VBR_A_INIT_VALUE			0x0f	//0xe9
#define PWM_VBR_B_INIT_VALUE			0xe9

#define PWM_VBR_A_INIT_FREQUENCY_VALUE	1	//KWON_0818 TEMP	100		//150Hz ->100hz
#define PWM_VBR_B_INIT_FREQUENCY_VALUE	1		//by hwangbos 100 -> 1		//150Hz

#define PWM_VBR_A_INIT_END_VALUE		0x08	//KWON_0819 TEMP	0xff
#define PWM_VBR_B_INIT_END_VALUE		0xe9	//by hwangbos 0x7b -> 0xe9 for temperal
#define PWM_PORT_VBR_A					PWM_DEV_PIN0	//KWON_0819	PWM_DEV_PIN3
#define PWM_PORT_VBR_B					PWM_DEV_PIN1	//KWON_0819	PWM_DEV_PIN2
#endif
#endif	//for #ifndef TEST_PWM_FUNCTION

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/
/**
 * PWM Configuration type
 */
typedef struct {
	UINT8			prescale;			///<00-1,01-2,10-4,11-8
	UINT8			tclkDivEn;			///<0=tclk 1-tclk/511
	UINT16			pwmPeriod;			///<10 bit value
	UINT8	        pwmDevClkSel;		///<only for PWM2/3 0-tclk/tclkby511 1-display hsync
	UINT8			pwmVreset;			///<only for PWM2/3 0-pwm count roll over continuosly 1:on every disp vsync edge
	UINT8			pwmHsyncPolarity;	///<only for PWM2/3 0-normal 1-invert
	UINT8			pwmVsyncPolarity;	///<only for PWM2/3 0-normal 1-invert
} PWM_CONFIG_T;

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
extern DTV_STATUS_T	DDI_PWM_Init (void);
//extern DTV_STATUS_T	DDI_PWM_SetPulseWidth (PWM_PIN_SEL_T portIndex, UINT16 data);
//extern DTV_STATUS_T	DDI_PWM_SetFrequency (PWM_PIN_SEL_T portIndex, UINT32 data);
//extern DTV_STATUS_T	DDI_PWM_SetDutyCycle (PWM_PIN_SEL_T portIndex, UINT16 data);
//extern DTV_STATUS_T	DDI_PWM_SetEnable (PWM_PIN_SEL_T portIndex, BOOLEAN data);
extern DTV_STATUS_T	DDI_PWM_SetVBRPortA (UINT16 data);
extern DTV_STATUS_T	DDI_PWM_SetVBRPortB (UINT16 data);
extern DTV_STATUS_T	DDI_PWM_SetPwmOnoff (BOOLEAN onoff);
extern void DDI_PWM_DEBUG (void);

#endif  /* _PWM_DDI_H_ */
