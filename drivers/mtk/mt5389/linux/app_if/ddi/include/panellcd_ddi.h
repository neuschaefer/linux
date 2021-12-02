/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file panellcd_ddi.h
 *
 * This source file defines the DDI functions related to LCD module Device
 * Driver control
 *
 *  @author	Lee, Byung Gul[ashton](lachesis@lge.com)
 *  @author	Seol, SeongWoon(sw.seol@lge.com)
 *  @version	0.2
 *  @date		2009.05.19
 *  @date		2009.07.01
 *  @date		2010.01.19
 *  @note
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_PANELLCD_DDI_H_
#define	_PANELLCD_DDI_H_

/******************************************************************************
    (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM // modified by dilly97 : fix include
#include "common.h"
#else
#include "common_win32.h"
#endif
#include "i2c_ddi.h"
#include "global_configurations.h"
#include "pql_ddi.h"

/******************************************************************************
    (Macro Definitions)
******************************************************************************/

/******************************************************************************
    (Local Constant Definitions)
******************************************************************************/
#define READ_OPERATION		0
#define WRITE_OPERATION		1

/******************************************************************************
    (Local Type Definitions)
******************************************************************************/

/******************************************************************************
    (Global Type Definitions)
******************************************************************************/
/**
 * VCOM PGAMMA PANEL Enum.
*/
typedef enum {
	VCOM_PGAMMA_PANEL_37INCH	= 0,
	VCOM_PGAMMA_PANEL_42INCH,
	VCOM_PGAMMA_PANEL_47INCH,
	VCOM_PGAMMA_PANEL_55INCH,
	VCOM_PGAMMA_PANEL_72INCH,
	VCOM_PGAMMA_PANEL_60HZ,
	VCOM_PGAMMA_PANEL_SIZE_MAX
} VCOM_PGAMMA_PANEL_SIZE_T;


/**
 * LOCALDIMMING DemoMode Type Enum.
*/
typedef enum {
	LOCALDIMMING_DEMOTYPE_LINEDEMO	= 0,
	LOCALDIMMING_DEMOTYPE_STOREDEMO,
	LOCALDIMMING_DEMOTYPE_SIZE_MAX
} LDIM_DEMO_TYPE_T;

#if (PLATFORM_TYPE == LG_PLATFORM)
/**
 * LOCALDIMMING DemoMode Type Enum.
*/
typedef enum {
	LOCALDIMMING_VERTYPE_VERSION 	= 1,
	LOCALDIMMING_VERTYPE_INCH,
	LOCALDIMMING_VERTYPE_SIZE_MAX
} LDIM_VERSIONDEMO_TYPE_T;
#endif

/******************************************************************************
    (Extern Variables & Function Prototype Declarations)
******************************************************************************/

/******************************************************************************
    (Static Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    (Local Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    (Global Variables & Function Prototypes Declarations)
******************************************************************************/
/**
 * pointer struct for Tcon function.
*/
typedef struct PANELLCD_TCON_DDI_CHIP
{
	void	(*pfnInitTcon) (void);
	void	(*pfnDebugTcon) (void);
	void	(*pfnInitVcomPGamma) (void);
	void	(*pfnWriteVcom) (UINT8 addr, UINT16 value);
	void	(*pfnReadVcom) (UINT8 addr, UINT16 *pValue);
	void	(*pfnWriteVcomToNVRAM) (UINT8 addr, UINT16 value);
	void	(*pfnWritePGamma) (UINT8 addr, UINT16 value);
	void	(*pfnReadPGamma) (UINT8 addr, UINT16 *pValue);
	void	(*pfnWritePGammaToNVRAM) (UINT8 addr, UINT16 value);
	void	(*pfnSet3DOptimize) (SINT8 mode);
	void	(*pfnInitPMIC)(void);
	void	(*pfnDebugPMIC)(void);
	BOOLEAN	(*pfnSetODCTable) (UINT8 mode);
	BOOLEAN	(*pfnSetWBColorTemp) (PQL_COLOR_TEMP_T cscPcCtData);
} PANELLCD_TCON_DDI_CHIP_T;

/**
 * pointer struct for Localdimming function.
*/
typedef struct PANELLCD_LOCALDIMMING_DDI_CHIP
{
	DTV_STATUS_T			(*pfnInitLocalDimming) 							(void);
	DTV_STATUS_T			(*pfnWriteRegisterLocalDimming)					(UINT8 *addr, UINT8 num, UINT8 *data);
	DTV_STATUS_T			(*pfnReadRegisterLocalDimming)					(UINT8 *addr, UINT8 num, UINT8 *data);
	DTV_STATUS_T			(*pfnCtrlEnableLocalDimming)					(BOOLEAN bCtrl);
	DTV_STATUS_T			(*pfnCtrlEnablePixelCompensationLocalDimming)	(BOOLEAN bCtrl);
	DTV_STATUS_T			(*pfnCtrlDemoModeLocalDimming)					(LDIM_DEMO_TYPE_T bType, BOOLEAN bCtrl);
	DTV_STATUS_T			(*pfnCtrlStoreLuxUpLocalDimming)				(BOOLEAN bCtrl);
	DTV_STATUS_T			(*pfnSetLUTLocalDimming)						(UINT8 lutTableIndex);
	DTV_STATUS_T			(*pfnCtrl3DTVModeLocalDimming)					(UINT8 ModuleType, BOOLEAN bCtrl, BOOLEAN bReverse);
	DTV_STATUS_T			(*pfnUpdateLocalDimmingFirmware)				(void);
	DTV_STATUS_T			(*pfnReadLEDVersionLocalDimming)				(UINT8 *data);	
	DTV_STATUS_T			(*pfnReadLEDcrcLocalDimming)					(UINT8 *data);	
	DTV_STATUS_T			(*pfnGetBplLocalDimming)						(UINT16 *pBPL);	
	DTV_STATUS_T			(*pfnDebugLocalDimming)							(void);
} PANELLCD_LOCALDIMMING_DDI_CHIP_T;

/******************************************************************************
	(Function Definitions)
******************************************************************************/
DTV_STATUS_T DDI_PANELLCD_TCON_Init(void);
DTV_STATUS_T DDI_PANELLCD_TCON_WriteVcom(UINT8 addr, UINT16 value);
DTV_STATUS_T DDI_PANELLCD_TCON_ReadVcom(UINT8 addr, UINT16 *pValue);
DTV_STATUS_T DDI_PANELLCD_TCON_WriteVcomToNVRAM(UINT8 addr, UINT16 value);
DTV_STATUS_T DDI_PANELLCD_TCON_WritePGamma(UINT8 addr, UINT16 value);
DTV_STATUS_T DDI_PANELLCD_TCON_ReadPGamma(UINT8 addr, UINT16 *pValue);
DTV_STATUS_T DDI_PANELLCD_TCON_WritePGammaToNVRAM(UINT8 addr, UINT16 value);

DTV_STATUS_T DDI_PANELLCD_TCON_SetODC_LUTable(UINT8 mode);
DTV_STATUS_T DDI_PANELLCD_TCON_SetWBColorTemp(PQL_COLOR_TEMP_T cscPcCtData);
DTV_STATUS_T DDI_PANELLCD_TCON_3DOptimize(SINT8 mode);

DTV_STATUS_T DDI_PANELLCD_DP_RX_Set_SpreadSpectrum(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_Set_ICP1(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_Set_RX_MS_DTG_MODE(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_RegReadByHost(UINT16 addr, UINT8 RxTx, UINT8 *buffer);
DTV_STATUS_T DDI_PANELLCD_DP_DownloadByUSB(UINT8* pBuffer, ULONG datasize, UINT8 dltype);
DTV_STATUS_T DDI_PANELLCD_DP_Lock_Init(void);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_Ctrl3DTVMode(BOOLEAN bCtrl);

// External Local Dimming IC
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_Init(void);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_CtrlLocalDimming(BOOLEAN bCtrl);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_CtrlPixelCompensation(BOOLEAN bCtrl);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_CtrlDemoMode(LDIM_DEMO_TYPE_T bType, BOOLEAN bCtrl);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_CtrlStoreLuxUp(BOOLEAN bCtrl);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_SetLUT(UINT8 nMode);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_UpdateFirmware(void);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_ReadLEDVersion(UINT8 *data);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_ReadLEDCrc(UINT8 *data);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_GetBPL(UINT16 *pBPL);

DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_DisableIC(void);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_Reset(UINT16 wDelayms);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_DownloadByUSB(UINT16 u16Size, UINT8 *pBuffer);
DTV_STATUS_T DDI_PANELLCD_LOCALDIMMING_ReadVersion(UINT8 index, UINT8 *pVersion);

void DDI_PANELLCD_LOCALDIMMING_Debug(void);

// Internal Local Dimming
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_Initialize(void);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_EnableLocalDimming(BOOLEAN bCtrl);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_EnablePixelCompensation(BOOLEAN bCtrl);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_CtrlDemoMode(BOOLEAN bType, BOOLEAN bCtrl);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_CtrlStoreDimmingLuxUp(BOOLEAN bCtrl);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_SetLUT(UINT8 lutTableIndex);
DTV_STATUS_T DDI_VIDEO_LOCALDIMMING_Debug(void);

void DDI_PANELLCD_LOCALDIMMING_LG1122_Control3DSyncDelay(UINT16 u3DSyncDelay);
void DDI_PANELLCD_LOCALDIMMING_LG1122_Command_APL_On(void);

#endif /* _PANELLCD_DDI_H_ */
