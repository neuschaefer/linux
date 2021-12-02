/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 2008 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file demod_ddi.h
 *
 *  DEMOD Module DDI header file.
 *
 *  @author		Goldman(jhgold@lge.com)
 *  @version	1.0
 *  @date		2008.04.14
 *  @note
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _DEMOD_DDI_H_
#define	_DEMOD_DDI_H_

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

#include "branches.h"
#include "common.h"
#include "country.h"
#include "global_configurations.h"
#include "tu_ddi.h"

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

extern	DTV_STATUS_T	DDI_DEMOD_Probe(TUNER_NUMBER_T tunerNo, TU_I2C_CH_INDEX_T portI2C);
extern	DTV_STATUS_T	DDI_DEMOD_Initialize(TUNER_NUMBER_T tunerNo, TRANS_MEDIA_T transMedia);
extern	DTV_STATUS_T	DDI_DEMOD_ChangeTransMedia(TUNER_NUMBER_T tunerNo, TRANS_MEDIA_T transMedia);
extern	DTV_STATUS_T 	DDI_DEMOD_SetDemod(TU_PI_SET_DEMOD_T paramStruct);
extern	DTV_STATUS_T 	DDI_DEMOD_TunePostJob(TUNER_NUMBER_T tunerNo, TU_PO_POSTJOB_T *pParamOutput);
extern	DTV_STATUS_T 	DDI_DEMOD_SetOutputPath(TUNER_NUMBER_T tunerNo, BOOLEAN bWithCAM);
extern	DTV_STATUS_T 	DDI_DEMOD_CheckLockState(TUNER_NUMBER_T tunerNo, TU_LOCK_STATE_T *pLockState);
extern	DTV_STATUS_T 	DDI_DEMOD_CheckSignalState(TUNER_NUMBER_T tunerNo, TU_SIGNAL_STATE_T *pSignalState);
extern	DTV_STATUS_T 	DDI_DEMOD_CheckSpecialData(TUNER_NUMBER_T tunerNo, TU_SPECDATA_T *pSpecData);
extern	DTV_STATUS_T 	DDI_DEMOD_SetSpecialData(TUNER_NUMBER_T tunerNo, TU_SPECDATA_T specData);
extern	DTV_STATUS_T 	DDI_DEMOD_CheckFrequencyOffset(TUNER_NUMBER_T tunerNo, UINT32 *pFreqOffset);
extern	UINT16			DDI_DEMOD_GetCellID(TUNER_NUMBER_T tunerNo);
extern	DTV_STATUS_T	DDI_DEMOD_GetFWVersion(TUNER_NUMBER_T tunerNo, UINT32 *pFWVersion);
extern  UINT16          DDI_DEMOD_Monitor (TUNER_NUMBER_T tunerNo, TU_LOCK_STATE_T *pLockState);
extern	void 			DDI_DEMOD_DebugMenu (TUNER_NUMBER_T tunerNo);

extern 	DTV_STATUS_T DDI_DEMOD_ControlTSMode(TUNER_NUMBER_T tunerNo, BOOLEAN bIsSerial);
extern 	DTV_STATUS_T DDI_DEMOD_ControlOutput(TUNER_NUMBER_T tunerNo, BOOLEAN bEnableOutput);
#ifdef __cplusplus
}
#endif

#endif	/* _DEMOD_DDI_H_ */

