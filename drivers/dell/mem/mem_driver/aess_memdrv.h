/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Memory driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#ifndef AESSMEMDRV_H
#define AESSMEMDRV_H

#ifdef AESSMEMDRV_C

/* ioctl definitions */
#define AESS_MEMDRV_IOC_MAGIC    0xB4
#define AESS_MEMDRV_RW           _IOWR(AESS_MEMDRV_IOC_MAGIC, 0, int)
#define AESS_MEMDRV_READ         _IOWR(AESS_MEMDRV_IOC_MAGIC, 1, int)
#define AESS_MEMDRV_WRITE        _IOWR(AESS_MEMDRV_IOC_MAGIC, 2, int)
#define AESS_MEMDRV_REQUEST      _IOWR(AESS_MEMDRV_IOC_MAGIC, 3, int)
#define AESS_MEMDRV_RELEASE      _IOWR(AESS_MEMDRV_IOC_MAGIC, 4, int)


#define MEM_READ      0
#define MEM_WRITE     1

/* define the limit of memory driver */
#define MEM_DRV_MAX_REGION_NUM      10
#define MEM_DRV_MAX_REGION_SIZE     65535

/* init flag */
#define MEM_NOT_INIT 0
#define MEM_INIT_OK  1

/* type define */
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;


/******************************************************************************
*   Enum      :   eMemDrvDataWidthType
******************************************************************************/
/**
 *  @brief   memory driver data width enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Byte */
	MEM_DRV_BYTE = 0,
	
	/** Word */
	MEM_DRV_WORD,
	
	/** Double word */
	MEM_DRV_DWORD
	
} eMemDrvDataWidthType;


/******************************************************************************
*   STRUCT      :   sMemDrvInfoType
******************************************************************************/
/**
 *  @brief   Structure to memory driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* physical base address of request region */
	UINT32 u32BaseAddr;
    
	/* size of request region in bytes */
	UINT16 u16RegionSize;
    
	/* offset of base address in bytes */
	UINT16 u16Offset;
    
	/* read/write data buffer pointer */
	void *pDataPtr;
    
	/* data size to read/write */
	UINT16 u16DataSize;
    
	/* read/write data width */
	UINT8 u8DataWidth;
	
	/* identifier */
	UINT8 u8ID;
	
} sMemDrvInfoType;


/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/


#endif   /* AESSMEMDRV_C */

#endif   /* AESSMEMDRV_H */
