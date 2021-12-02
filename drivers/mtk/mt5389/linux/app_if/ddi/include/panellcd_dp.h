/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/
/** @file panellcd_dp.h
 *
 *  The Common Functions for Module
 *  Display Port setting
 *
 *  @author Park. Jae Han (jaehan.park@lge.com)
 *  @version	 0.2
 *  @date		 2009.05.19
 *  @note
 *  @see

 */

#ifndef _PANELLCD_DP_H_
#define _PANELLCD_DP_H_

#include "common.h"

//------------------------------------------------------------------------------
//  Definition
//------------------------------------------------------------------------------
#define 		DP_ACK						0x0C

#define		HOST_REG_ADDR				0xE4 
#define		DDC2BI_DST_ADDR			0x6C

#define		DDC2BI_DST_ADDR4RX		0x6C		//RX Board for test,  TX : 0x6E 
#define		DDC2BI_DST_ADDR4TX		0x6E 
#define		DDC2BI_SRC_ADDR			0x51 

#define		GPROBE_CMD_CODE			0xC2
#define		GPROBE_CMD_PAGE			0x0000

#define		CMD_FAST_WR_FLASH		0x10 
#define		CMD_APPS_TEST				0x12 
#define		BURST_RAM_WR				0x13 
#define		CMD_FLASH_ERASE			0x19 
#define		CMD_RAM_EXECUTE			0x1D 
#define		CMD_RESET					0x20	//force excution in internal rom  
#define		CMD_READ_REG				0x06 
#define		CMD_DEBUG_ON				0x09 
#define		CMD_DEBUG_OFF				0x0A 

#define		ISP_DRIVER_RAM_ADDR 		0x500
#define		ISP_DRIVER_ADDR			0x82000000UL
#define		ISP_DRIVER_SIZE_RX			0x1450//0x1443//0x1433
#define		ISP_DRIVER_SIZE_TX			0x1450//0x1431//0x1433


#define		ISP_CODE_START			0xAC000UL
#define		ISP_CODE_ADDR				0x84000000UL
#define		ISP_CODE_SIZE				0x14000UL

#define		DP_RX						1
#define		DP_TX						0
#define		DP_ALL						2
#define		DP_UNKNOWN				0xff

#define		H_CMD_VERSION				0x04
#define		H_CMD_SSCG				0x38
#define		H_CMD_GPIO_DIR			0x3E
#define		H_CMD_GPIO_OUT			0x3F
#define		H_CMD_STOP_WDOG			0x01
#define		H_CMD_DPRX_MS_DTG_MODE	0x70
#define		H_CMD_ICP1_HS_VS_REGEN	0x8E


//------------------------------------------------------------------------------
//  Structure
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Functions
//------------------------------------------------------------------------------

/**
 * DP reponse type.
 *
 * @see
*/
typedef struct RespType
{
	UINT8 len;
	UINT8 cmd;
	UINT8 chk;
}RespType;

/**
 * DP Error Status type.
 *
 * @see
*/
typedef enum
{                        // Error status
   DP_FALSE = 0,
   DP_TRUE  = 1,
   DP_OK    = 2,          // PDR7021
   DP_ERR_SEND = 3,       // Error in sending data
   DP_ERR_RCV,            // Error in receiving data
   DP_ERR_TMO,            // Timeout error
   DP_ERR_PARAM,          // Error in input parameters
   DP_ERR_READ,           // Error in reading
   DP_ERR_WRITE,          // Error in writing
   DP_ERR_COMMAND,        // API command failed
   DP_ERR_POINTER,        // Pointer not intialized
   DP_ERR_OVERFLOW,       // Input parameter vlaues over limit
   DP_ERR_MEMORY,         // Out of SDRAM memory
   DP_ERR_NO_DATA,        // IR FIFO queue is empty
   DP_ERR_BUSY,           // Device busy
   DP_ERR_LIMIT,          // Reach limit of position
   DP_TMR_ACTIVE,         // Timer is active
   DP_TMR_TIMED_OUT,      // Timer is timed out
   DP_TMR_STOPPED,     // Timer is stopped
   DP_CONTINUE,        // DP_OK to DP_CONTINUE execution
   DP_ABORT,           // DP_ABORT execution
   DP_ABORTED,         // execution was DP_ABORTed
   DP_LOST_FEATURES,   // auto adjust lost features
   DP_FAIL,            // something failed
   DP_ERR_2WIRE_NOACK,  // 2-Wire slave device returned no-ack during write operation
   DP_ERR_I2C_NOACK  //37
}DP_RET_STAT;


DTV_STATUS_T DDI_PANELLCD_DP_RegReadByHost(UINT16 addr, UINT8 RxTx, UINT8 *buffer);
DTV_STATUS_T DDI_PANELLCD_DP_DownloadByUSB(UINT8* pBuffer, ULONG datasize, UINT8 dltype);
DTV_STATUS_T DDI_PANELLCD_DP_FRC_LOCALDIMM_GPIO_Set(void);
DTV_STATUS_T DDI_PANELLCD_DP_FRC_Reset(UINT8 ctrl);
DTV_STATUS_T DDI_PANELLCD_DP_LocalDimming_Reset(UINT8 ctrl);
DTV_STATUS_T DDI_PANELLCD_DP_Set_ICP1(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_Set_RX_MS_DTG_MODE(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_RX_Set_SpreadSpectrum(BOOLEAN bEnable);
DTV_STATUS_T DDI_PANELLCD_DP_Lock_Init(void);

#endif /* _PANELLCD_LOCALDIMMING_DP_H_ */

