/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2008 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file tu_ext_drivers.h
 *
 *  external interface.
 *
 *  @author		Goldman(jhgold@lge.com)
 *  @version	1.0
 *  @date		2008.07.03
 *  @note		Additional information.
 *  @see		reference (file/function/URL/etc)
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/
#ifndef _TU_EXT_DRIVERS_H_
#define	_TU_EXT_DRIVERS_H_


#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	Control Constants
******************************************************************************/


/******************************************************************************
	File Inclusions
******************************************************************************/

#include "tu_common.h"

#include "sdec_ddi.h"


/******************************************************************************
 	Constant Definitions
******************************************************************************/


/******************************************************************************
	Macro Definitions
******************************************************************************/


/******************************************************************************
	Type Definitions
******************************************************************************/


/******************************************************************************
	Function Declaration
******************************************************************************/

extern	void 	TU_EXT_PowerOnReset(void);

extern	TU_RETURN_VALUE_T 	TU_I2C_Open(TU_I2C_CH_INDEX_T chIdx, TU_I2C_DEV_ID deviceID, TU_I2C_HANDLE *pHandle);
extern	TU_RETURN_VALUE_T 	TU_I2C_Close(TU_I2C_HANDLE handle);
extern	TU_RETURN_VALUE_T 	TU_I2C_Read (
									TU_I2C_HANDLE handle,
									UINT8 addrLength,
									UINT8 *pAddress,
									UINT16 dataLength,
									UINT8 *pData);
extern	TU_RETURN_VALUE_T 	TU_I2C_Write (
									TU_I2C_HANDLE handle,
									UINT8 addrLength,
									UINT8 *pAddress,
									UINT16 dataLength,
									UINT8 *pData);

extern	TU_RETURN_VALUE_T 	TU_I2C_FastRead (
									TU_I2C_HANDLE handle,
									UINT8 addrLength,
									UINT8 *pAddress,
									UINT16 dataLength,
									UINT8 *pData);
extern	TU_RETURN_VALUE_T 	TU_I2C_FastWrite (
									TU_I2C_HANDLE handle,
									UINT8 addrLength,
									UINT8 *pAddress,
									UINT16 dataLength,
									UINT8 *pData);

extern	TU_RETURN_VALUE_T	TU_EXT_VDP_CheckVideoSync(
									TUNER_NUMBER_T tunerNo,
									AUDIO_SIF_SOUNDSYSTEM_T audioSystem,
									TU_LOCK_STATE_T *pLockState);
extern	void					TU_EXT_VDP_SetHsyncDetectionForTuning(
							TUNER_NUMBER_T tunerNo,
							BOOLEAN bEnable);

extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_ResetDemod(TUNER_NUMBER_T tunerNo, UINT16 delayMs);

/* XC5000 Only: GPIO_WR_ENABLE_AGC_SPEED */
extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_ResetTuner(TUNER_NUMBER_T tunerNo, UINT16 delayMs);
extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_ResetSTuner(TUNER_NUMBER_T tunerNo, UINT16 delayMs);

extern	TU_RETURN_VALUE_T 	TU_EXT_GPIO_SetAGCSpeed(TUNER_NUMBER_T tunerNo, BOOLEAN bAGCSpeedON);
extern 	TU_RETURN_VALUE_T 	TU_EXT_GPIO_SetAttenuator(TUNER_NUMBER_T tunerNo, BOOLEAN bBoosterON);
extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_OutputControl(TUNER_NUMBER_T tunerNo, BOOLEAN bControl);

extern	TU_RETURN_VALUE_T 	TU_EXT_GPIO_Sharp8005_SetTUReset(TUNER_NUMBER_T tunerNo, BOOLEAN onOff);
extern	TU_RETURN_VALUE_T 	TU_EXT_GPIO_Sharp8005_SetRFSwitch(TUNER_NUMBER_T tunerNo, BOOLEAN onOff);
extern	TU_RETURN_VALUE_T 	TU_EXT_GPIO_Sharp8005_SetGAINSwitch(TUNER_NUMBER_T tunerNo, BOOLEAN onOff);

extern	TU_RETURN_VALUE_T 	TU_EXT_SetControl(TUNER_NUMBER_T tunerNo, TU_CONTROL_MODE_T controlMode, BOOLEAN bControl);
extern	TU_RETURN_VALUE_T 	TU_EXT_GetControl(TUNER_NUMBER_T tunerNo, TU_CONTROL_MODE_T controlMode, BOOLEAN *pbControl);

#if (PLATFORM_TYPE == LG_PLATFORM)
extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_BufferControl_FE(TUNER_NUMBER_T tunerNo, BOOLEAN bControl);
extern	TU_RETURN_VALUE_T	TU_EXT_GPIO_BufferControl_L9(TUNER_NUMBER_T tunerNo, BOOLEAN bControl);
#endif


#ifdef __cplusplus
}
#endif

#endif /* End of _TU_EXT_DRIVERS_H_ */

