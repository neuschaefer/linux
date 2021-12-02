/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 1999 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file LCDmodule_ddi.h
 *
 *	Definition of LCD Module's drivers
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

#ifndef _LCDMODULE_DDI_H_
#define _LCDMODULE_DDI_H_


/*==========================================================
	User Type Definition
==========================================================*/

typedef enum {
	VCOM_PGAMMA_PANEL_37INCH	= 0,
	VCOM_PGAMMA_PANEL_42INCH,
	VCOM_PGAMMA_PANEL_47INCH,
	VCOM_PGAMMA_PANEL_SIZE_MAX
} VCOM_PGAMMA_PANEL_SIZE_T;


/*==========================================================
	Function Prototype Definition
==========================================================*/
extern DTV_STATUS_T DDI_LCDMODULE_InitVcomPGamma(void);
extern DTV_STATUS_T DDI_LCDMODULE_SetVcomPGamma(UINT8 addr, UINT16 value);
extern DTV_STATUS_T DDI_LCDMODULE_GetVcomPGamma(UINT8 addr, UINT16 *pValue);
extern DTV_STATUS_T DDI_LCDMODULE_SetVcomPGammaToNVRAM(UINT8 addr, UINT16 value);


#endif	/* end of _LCDMODULE_DDI_H_ */

