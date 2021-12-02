/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file pdpmodule_ddi.h
 *
 *	Definition of PDP Module's drivers
 *
 *	@author     unknown
 *	@version	0.1
 *	@date		2008.02.12
 *	@note
 *	@see
 */

/******************************************************************************
	Header File Guarder
******************************************************************************/

#ifndef _PDPMODULE_DDI_H_
#define _PDPMODULE_DDI_H_


/*==========================================================
	User Type Definition
==========================================================*/

// [LGE jykim 20071128 : Fresh White]
/**
 * Fresh White
 */
typedef struct FRESH_WHITE_DATA
{
	UINT8	pdpmoduleDCTC1;
	UINT8	pdpmoduleDCTC2;
	UINT8	pdpmoduleDCTC3;
	UINT8	pdpmoduleDCTC4;
	UINT8	pdpmoduleDCTC5;

}	PDPMODULE_FRESH_WHITE_DATA_T;

/**
 * GAMMA SET TABLE MODE.
 */
typedef enum
{
	PDPMODULE_GAMMA_TABLE1	= 0,
	PDPMODULE_GAMMA_TABLE2	= 1,
	PDPMODULE_GAMMA_TABLE3	= 2,
	PDPMODULE_GAMMA_TABLE4	= 3

}	PDPMODULE_GAMMA_CTRL_T;

/**
 * ISM(화면 잔상 방지) MODE.
 */
typedef enum
{
	PDPMODULE_ISM_OFF			= 0,
	PDPMODULE_ISM_MOVE			= 1,
//	PDPMODULE_ISM_FLIP			= 2,
	PDPMODULE_ISM_COLORWASH		=2,
	PDPMODULE_ISM_WHITEWASH	= 3

}	PDPMODULE_ISM_T;

/**
 * PS MODE.
 */
typedef enum
{
	PDPMODULE_PS_MODE0	= 0,
	PDPMODULE_PS_MODE1	= 1,
	PDPMODULE_PS_MODE2	= 2,
	PDPMODULE_PS_MODE3	= 3,
	PDPMODULE_PS_MODE4	= 4,
	PDPMODULE_PS_MODE5	= 5,
	PDPMODULE_PS_MODE6	= 6,
	PDPMODULE_PS_MODE7	= 7,
	PDPMODULE_PS_MODE8	= 8,
	PDPMODULE_PS_MODE9	= 9,
	PDPMODULE_PS_MODE10	= 10,
	PDPMODULE_PS_MODE11	= 11,
	PDPMODULE_PS_MODE12 = 12,
	PDPMODULE_PS_MODE13 = 13,
	PDPMODULE_PS_MODE14 = 14
}	PDPMODULE_PS_MODE_T;


/*==========================================================
	Function Prototype Definition
==========================================================*/
extern	void	DDI_PDPMODULE_SetPowerSaving(PDPMODULE_PS_MODE_T psMode, BOOLEAN bOnOff);
extern	void	DDI_PDPMODULE_SetPowerSave(PDPMODULE_PS_MODE_T psMode);

extern	DTV_STATUS_T	DDI_PDPMODULE_SetAVorPCMode(UINT8 avPcMode, BOOLEAN bIsPc);
extern	DTV_STATUS_T	DDI_PDPMODULE_SetOrbiter( UINT8 scrollMode, UINT8 orbiterTime,
													UINT8 orbiterPixel, UINT8 orbiterStep );
extern	DTV_STATUS_T	DDI_PDPMODULE_SetGammaMode(PDPMODULE_GAMMA_CTRL_T gammaMode);
extern	DTV_STATUS_T	DDI_PDPMODULE_SetISM(PDPMODULE_ISM_T ismMode, UINT8 ismCtrl);
extern	DTV_STATUS_T	DDI_PDPMODULE_ReadASICName(UINT8 *pReadASICName,UINT8 mode);
extern	DTV_STATUS_T	DDI_PDPMODULE_ReadMODULEName(UINT8 *pReadMODULEName, UINT8 mode);
extern	DTV_STATUS_T	DDI_PDPMODULE_ReadMODULERomVer(UINT8 *pReadRomVer, UINT8 mode);
extern	DTV_STATUS_T	DDI_PDPMODULE_ReadMODULETemp( float *pReadCurrTemp);

extern	BOOLEAN	DDI_PDPMODULE_SetInvTime(UINT8 invTime);
extern	void	DDI_PDPMODULE_SetFreshWhiteData(PDPMODULE_FRESH_WHITE_DATA_T freshWhiteReg);
extern	void	DDI_PDPMODULE_SetFreshWhite(UINT8 onOff);
extern	void	DDI_PDPMODULE_SetBright(UINT8 brMode);
extern	void	DDI_PDPMODULE_SetFMC(UINT8 bOnOff);
extern	void	DDI_PDPMODULE_SetDPS(UINT8 onOff);
extern	void	DDI_PDPMODULE_SetPatternGen(BOOLEAN bOnOff);
extern	BOOLEAN	DDI_PDPMODULE_GetPatternGen(void);

#endif	/* end of _PDPMODULE_DDI_H_ */

