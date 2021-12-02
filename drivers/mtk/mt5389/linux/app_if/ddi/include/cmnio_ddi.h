/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file cmnio_ddi.h
 *
 *	This header file defines the driver functions related to i2c Driver
 *
 *	@author 	dhjung (dhjung77@lge.com)
 *	@version	0.9
 *	@date		2007.02.12
 *	@note		2007.04.28 modified by earnest for chaplin Chip
 *	@see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _CMNIO_DDI_H_
#define _CMNIO_DDI_H_

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <osa_api.h>
#include <common.h>

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

#define I2C_OK				 0
#define I2C_ERROR			-1

#if (PLATFORM_TYPE == BCM_PLATFORM)
#define OFFSET_PWM_DUTY_DIRECT		10000
#endif


/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/
#define PWM_OUT_EN	0x01	/* pwm out enable */
#define PWM_DB_EN	0x02	/* pwm double buffer enable */
#define PWM_VRST_EN	0x04	/* pwm v-sync reset enable */
#define PWM_VDB_EN	0x08	/* pwm v-sync double buffer enable */
#define PWM_VAR_EN	0x10	/* pwm variable freq mode enable */

/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

/* CMNIO I2C */

/**
 * type definition about i2c speed
 *
*/
typedef enum
{
	I2C_MODE_NORMAL		= 	0,		/**< normal speed	: 100K	*/
	I2C_MODE_FAST		=	1,		/**< fast speed 	: 400K	*/
	I2C_MODE_SLOW		=	2,		/**< slow speed 	:  50K	*/

}	I2C_MODE_T;

/* CMIO GPIO */
/**
 * type definition about gpio group
 *
*/
typedef enum
{
	GPIO_GROUP_GPIO = 1,	/**< gpio group */
	GPIO_GROUP_GPI,			/**< gpi group */
	GPIO_GROUP_GPO			/**< gpo group */

}	GPIO_GROUP_T;

/**
* type definition about gpio high low
*
*/
typedef enum
{
	GPIO_LOW  = 0,			/**< gpio low */
	GPIO_HIGH 				/**< gpio high */
} GPIO_VALUE_T;

/* CMNIO PWM */

/**
* type definition about PWM device's number
*
*/
typedef enum {
	PWM_DEV_PIN0	= 0,	/**< PWM# 0 */
	PWM_DEV_PIN1,			/**< PWM# 1 */
	PWM_DEV_PIN2,			/**< PWM# 2 */
	PWM_DEV_PIN3,			/**< PWM# 3 */
	PWM_DEV_PIN4,			/**< PWM# 3 */
	PWM_DEV_MAX,			/**< PWM# MAX */
} PWM_PIN_SEL_T;

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
/* CMNIO I2C */
extern DTV_STATUS_T DDI_CMNIO_I2C_Init (void);
extern DTV_STATUS_T DDI_CMNIO_I2C_Read (UINT8 ChNum, I2C_MODE_T transMode, UINT8 slaveAddr, UINT32 subAddrMode,
									   UINT8 *subAddr, UINT16 nBytes, UINT8 *rxBuf, UINT32 retry);
extern DTV_STATUS_T DDI_CMNIO_I2C_Write(UINT8 ChNum, I2C_MODE_T transMode, UINT8 slaveAddr, UINT32 subAddrMode,
								 	   UINT8 *subAddr, UINT16 nBytes, UINT8 *txBuf, UINT32 retry);

/* CMNIO GPIO */
extern DTV_STATUS_T	DDI_CMNIO_GPIO_Init (void);
extern DTV_STATUS_T	DDI_CMNIO_GPIO_SetPortDirection(UINT32 portIndex, BOOLEAN direction);
extern DTV_STATUS_T	DDI_CMNIO_GPIO_SetPortDirection_SP(UINT32 portIndex, BOOLEAN direction); //sungwoo.han 1234
extern DTV_STATUS_T	DDI_CMNIO_GPIO_SetOutputPort(UINT32 portIndex, UINT8 data);


#if 1 /* Jason Lim, for fast I2C */
extern DTV_STATUS_T DDI_CMNIO_GPIO_I2C_ReadWriteByte(UINT32 portIndex_sda, UINT32 portIndex_scl, BOOLEAN bWrite, UINT8 *pData);
#endif


extern DTV_STATUS_T	DDI_CMNIO_GPIO_GetInputPort(UINT32 portIndex, UINT8 *pData);
extern DTV_STATUS_T	DDI_CMNIO_GPIO_SetOutputPortArray(UINT32 numArray, UINT32 portArray[], UINT8 dataArray[]);
extern DTV_STATUS_T	DDI_CMNIO_GPIO_GetInputPortArray(UINT32 numArray, UINT32 portArray[], UINT8 dataArray[]);

/* CMNIO PWM */
extern DTV_STATUS_T DDI_CMNIO_PWM_Init (void);
extern DTV_STATUS_T DDI_CMNIO_PWM_SetFrequency(PWM_PIN_SEL_T pwmIndex, UINT32 data);
extern DTV_STATUS_T DDI_CMNIO_PWM_SetDutyCycle (PWM_PIN_SEL_T pwmIndex, UINT32 data);
#if (PLATFORM_TYPE == LG_PLATFORM)
extern DTV_STATUS_T DDI_CMNIO_PWM_SetEnable (PWM_PIN_SEL_T pwmIndex, UINT8 data);
#else
extern DTV_STATUS_T DDI_CMNIO_PWM_SetEnable (PWM_PIN_SEL_T pwmIndex, BOOLEAN data);
#endif

extern DTV_STATUS_T DDI_CMNIO_PWM_SetPulseWidth (PWM_PIN_SEL_T pwmIndex, UINT32 data);

void DDI_BSC_IIC_Port_Enable(void);
void DDI_BSC_IIC_Port_Disable(void);



#if(PLATFORM_TYPE == BCM_PLATFORM)
//extern DTV_STATUS_T DDI_CMNIO_PWM_SetDutyCycleDirect(PWM_PIN_SEL_T pwmIndex, UINT32 data);
//extern UINT32 DDI_CMNIO_PWM_GetFrequency(void);
#endif

#endif /* _CMNIO_DDI_H_ */
