/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 SPI driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#if !defined (AESSSSPIDRV_H)
#define AESSSSPIDRV_H


#ifdef AESSSSPIDRV_C

/* ioctl definitions */
#define DRIVER_NAME "aess_sspidrv"
#define DRIVER_MODULE "aess_sspidrv.ko"
#define DRIVER_TYPE "CHAR"

/* IOCTL command */
#define AESS_SSPIDRV_IOC_MAGIC       0xC5
#define AESS_SSPIDRV_WR              _IOWR(AESS_SSPIDRV_IOC_MAGIC, 1, \
										                sSSPIDrvInfoType)

/* type define */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;
typedef u64                 UINT64;


/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/

#endif   /* AESSSSPIDRV_C */


/******************************************************************************
*   STRUCT      :   sSPIDrvInfoType
******************************************************************************/
/**
 *  @brief   structure of SPI driver information
 *
 *****************************************************************************/
typedef struct
{
	/** SPI Transaction process time */
	UINT8 u8ProcessTime;
	
	/** SPI bus mode */
	UINT8 u8Mode;
	
	/** SPI chip select */
	UINT8 u8ChipSelect;
	
	/** SPI clock speed */
	UINT8 u8Speed;
	
	/** Msg transmit buffer */
	UINT8 *pu8MsgSendBuffer;
	
	/** Msg transmit data length */
	UINT32 u32MsgSendDataSize;
	
	/** Msg receive buffer */
	UINT8 *pu8MsgRecBuffer;
	
	/** Msg receive data length */
	UINT32 u32MsgRecDataSize;
 	
} sSSPIDrvInfoType;



/******************************************************************************
*   Enum      :   eSPIDrvModeType
******************************************************************************/
/**
 *  @brief   SPI driver mode enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** CPOL (clock polarity) = 0 ; CPHA (clock phase) = 0 */
	SSPI_DRV_MODE_0 = 0,
	
	/** CPOL (clock polarity) = 0 ; CPHA (clock phase) = 1 */
	SSPI_DRV_MODE_1,
	
	/** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 0 */
	SSPI_DRV_MODE_2,
	
	/** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 1 */
	SSPI_DRV_MODE_3,
	
	/** Max support mode */
	SSPI_DRV_MAX_MODE
} eSSPIDrvModeType;


#ifndef AESSSSPIDRV_C

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
extern int aess_sspi_io(sSSPIDrvInfoType *psSSPIDrvInfo);

#endif   /* AESSSSPIDRV_C */

#endif   /* AESSSSPIDRV_H */
