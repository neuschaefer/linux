/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * Lib 452 user space library for SPI driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#ifndef Lib_SSPI_DRV_H
#define Lib_SSPI_DRV_H

#ifdef Lib_SSPI_DRV_C

/* ioctl definitions */
#define DRIVER_MANDATORY
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

#endif  /* end of define Lib_SPI_DRV_C */


#define Lib_SSPI_STATUS_OK     0
#define Lib_SSPI_STATUS_FAIL   1


/******************************************************************************
*   STRUCT      :   sSSPIDrvInfoType
******************************************************************************/
/**
 *  @brief   structure of SPI driver information
 *
 *****************************************************************************/
typedef struct
{
		/** SPI Transaction process time */
		unsigned char u8ProcessTime;
    
    /** SPI bus mode */
    unsigned char u8Mode;
    
    /** SPI chip select */
    unsigned char u8ChipSelect;
    
    /** SPI clock speed */
    unsigned char u8Speed;
    
    /** Msg transmit buffer */
    unsigned char *pu8MsgSendBuffer;
    
    /** Msg transmit data length */
    unsigned long int u32MsgSendDataSize;
    
    /** Msg receive buffer */
    unsigned char *pu8MsgRecBuffer;
    
    /** Msg receive data length */
    unsigned long int u32MsgRecDataSize;
    
} sSSPIDrvInfoType;



/******************************************************************************
*   Enum      :   eSSPIDrvModeType
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
    /* SSPI_DRV_MODE_1, */
    
    /** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 0 */
    /* SSPI_DRV_MODE_2, */
    
    /** CPOL (clock polarity) = 1 ; CPHA (clock phase) = 1 */
    /* SSPI_DRV_MODE_3, */
    
    /** Max support mode */
    SSPI_DRV_MAX_MODE
} eSSPIDrvModeType;


/******************************************************************************
*   Enum      :   eSSPIDrvCSType
******************************************************************************/
/**
 *  @brief   SPI driver chip select enumeration type
 *
 *****************************************************************************/
typedef enum
{
    /** Chip select 0, SCS0_N */
    SSPI_DRV_CS_0 = 0,
    
    /** Chip select 1, SCS1_N */
    SSPI_DRV_CS_1,
    
    /** Chip select 2, SCS2_N */
    SSPI_DRV_CS_2,
    
    /** Chip select 3, SCS3_N */
    SSPI_DRV_CS_3,
    
    /** Max support chip select */
    SSPI_DRV_MAX_CS
} eSSPIDrvCSType;


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
extern int SSPIWriteRead(
                         /** access information */
                         sSSPIDrvInfoType *psSSPIDrvInfo
                       );


#endif  /* end of define Lib_SSPI_DRV_H */


/* End of code */
