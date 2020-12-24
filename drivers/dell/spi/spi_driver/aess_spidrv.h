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

#if !defined (AESSSPIDRV_H)
#define AESSSPIDRV_H


#ifdef AESSSPIDRV_C

/* ioctl definitions */
#define DRIVER_NAME "aess_spidrv"
#define DRIVER_MODULE "aess_spidrv.ko"
#define DRIVER_TYPE "CHAR"

/* IOCTL command */
#define AESS_SPIDRV_IOC_MAGIC       0xBC
#define AESS_SPIDRV_WR              _IOWR(AESS_SPIDRV_IOC_MAGIC, 1, \
										                sSPIDrvInfoType)

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

#endif   /* AESSSPIDRV_C */


/******************************************************************************
*   STRUCT      :   sSPIDrvInfoType
******************************************************************************/
/**
 *  @brief   structure of SPI driver information
 *
 *****************************************************************************/
typedef struct
{
	/** SPI bus number */
	UINT8 u8Number;
	
	/** SPI bus mode */
	UINT8 u8Mode;
	
	/** SPI chip select */
	UINT8 u8ChipSelect;
	
	/** Reserved for feature used*/
	UINT8 u8Speed;
	
	/** Msg transmit buffer */
	UINT8 *pu8MsgSendBuffer;
	
	/** Msg transmit data length */
	UINT32 u32MsgSendDataSize;
	
	/** Msg receive buffer */
	UINT8 *pu8MsgRecBuffer;
	
	/** Msg receive data length */
	UINT32 u32MsgRecDataSize;
 	
} sSPIDrvInfoType;


/******************************************************************************
*   Enum      :   eSPIDrvBusType
******************************************************************************/
/**
 *  @brief   SPI driver bus enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Bus number 0 */
	SPI_DRV_BUS_0 = 0,
	
	/** Bus number 1 */
	/*(SPI_DRV_BUS_1, */
	
	/** Bus number 2 */
	/* SPI_DRV_BUS_2,*/ 
	
	/** Bus number 3 */
	/* SPI_DRV_BUS_3,*/
	
	/** Max bus number */
	SPI_DRV_MAX_BUS
} eSPIDrvBusType;


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
	SPI_DRV_MODE_0 = 0,
	
	/** CPOL (clock polarity) = 0 ; CPHA (clock phase) = 1 */
	/* SPI_DRV_MODE_1, */
	
	/** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 0 */
	/* SPI_DRV_MODE_2, */
	
	/** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 1 */
	/* SPI_DRV_MODE_3, */
	
	/** Max support mode */
	SPI_DRV_MAX_MODE
} eSPIDrvModeType;


/******************************************************************************
*   Enum      :   eSPIDrvCSType
******************************************************************************/
/**
 *  @brief   SPI driver chip select enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Chip select 0, SCS0_N */
	SPI_DRV_CS_0 = 0,
	
	/** Chip select 1, SCS1_N */
	SPI_DRV_CS_1,
	
	/** Chip select 2, SCS2_N */
	SPI_DRV_CS_2,
	
	/** Chip select 3, SCS3_N */
	SPI_DRV_CS_3,
	
	/** Max support chip select */
	SPI_DRV_MAX_CS
} eSPIDrvCSType;
#ifndef AESSSPIDRV_C

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
extern int aess_spi_io(sSPIDrvInfoType *psSPIDrvInfo);

#endif   /* AESSSPIDRV_C */

#endif   /* AESSSPIDRV_H */
