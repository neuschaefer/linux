/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_ycproc.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _DRV_LDMSPI_H_
#define _DRV_LDMSPI_H_

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_io.h"
#include "panel.h"
#include "x_ckgen.h"

#define LDM_BUS_CLOCK 144
#define MAX_SPI_CMD_SIZE 512
#define HSYNC_SRC SrcPWM2
#define CONTRAST_SKIN_CURVE_NUM 33

typedef enum
{
    E_SPI_PAGE_WRITE,
    E_SPI_READ_FAULT,
    E_SPI_LIGHT_UP,
    E_SPI_LOG_OFF,
    E_SPI_FAULT_REPAIR
} ENUM_SPI_COMMAND_SET;

typedef enum
{
    E_SPI_NORMAL_STATUS=0,
    E_SPI_OPEN_STATUS=1,
    E_SPI_SHORT_STATUS=2,
    E_SPI_UNCERTAIN_SHORT_STATUS=3,
    E_SPI_SHORT_TOLERANCE=180   
} ENUM_SPI_FAULT_STATUS_SET;

typedef enum
{
    E_SPI_LGD_PROTOCOL,
    E_SPI_AMS_PROTOCOL        
} ENUM_SPI_PROTOCOL_SET;

BOOL bDrvLcdimSpiInit(void);
void vDrvLcdimSpiCommandGen(UINT8 u1CommandSet);
void vDrvLcdimSpiGetSystemInfo(void);
void vDrvLcdimSpiSetLoopAllOnOff(UINT8 u1OnOff);
void vDrvLcdimSpiSetGlobalDimming(UINT16 u2Duty, UINT8 u1Freq);
void vDrvLcdimSpi1BWrite(UINT8 u1Addr, UINT8 u1Data);
void vDrvLcdimSpi1BRead(UINT8 u1Addr);
void vDrvLcdimSpiSendDataSWMode(UINT8 *u1DataArray, UINT16 u2Datalen);
void vDrvLcdimSpiSendDataHWMode(UINT8 *u1DataArray, UINT16 u2Datalen);
UINT8 u1DrvLcdimSpiRWProtect(void);
UINT8 vDrvLcdimSpiGetGlobalDimming(void);
UINT16 u2DrvLcdimSpiGetBlockNum(void);
UINT32 vDrvLcdimSpiWaitReady(UINT32 u4WaitCount);

//-----------------------------------------------------------
//   Function Prototype For Command Set
//-----------------------------------------------------------
void SPI_NON_DEF_CMD_0_PARAM(void);
void SPI_NON_DEF_CMD_1_PARAM(UINT8 u1Param1);
void SPI_NON_DEF_CMD_2_PARAM(UINT8 u1Param1, UINT8 u1Param2);

typedef struct sLdmSpiFuncSet {    
	void (*vSpiBatchReadCmd)(void);
	void (*vSpiBatchWriteCmd)(void);
	void (*vSpiLightUpCmd)(void);
	void (*vSpiLogOffCmd)(void);
	void (*vSpiRepairCmd)(void);	
	void (*vSpiRead1BCmd)(UINT8);
	void (*vSpiWrite1BCmd)(UINT8, UINT8);	
} sLdmSpiCommandSet;

//-----------------------------------------------------------
//                Driver For AMS Driver IC
//-----------------------------------------------------------
void vDrvAmsBatchReadCmd(void);
void vDrvAmsBatchWriteCmd(void);
void vDrvAmsLightUpCmd(void);
void vDrvAmsLogOffCmd(void);
void vDrvAmsRepairCmd(void);
void vDrvAmsRead1BCmd(UINT8 u1Addr);
void vDrvAmsWrite1BCmd(UINT8 u1Addr, UINT8 u1Data);

void vDrvAmsSetFaultState(UINT8 u1BlkIdx, UINT8 u1FaultStatus);
UINT8 u1DrvAmsGetFaultState(void);
UINT16 u2DrvGetTotalBadBlocks(void);
void vDrvAmsSetScanning(void);
//-----------------------------------------------------------
//                Driver For LGD Driver IC
//-----------------------------------------------------------
void vDrvLgdBatchWriteCmd(void);
void vDrvLgdLightUpCmd(void);

#endif //_DRV_LDMSPI_H_
