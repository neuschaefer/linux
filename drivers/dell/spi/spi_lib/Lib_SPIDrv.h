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

#ifndef Lib_SPI_DRV_H
#define Lib_SPI_DRV_H

#ifdef Lib_SPI_DRV_C

/* ioctl definitions */
#define DRIVER_MANDATORY
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

#endif  /* end of define Lib_SPI_DRV_C */


#define Lib_SPI_STATUS_OK     0
#define Lib_SPI_STATUS_FAIL   1


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
    unsigned char u8Number;
    
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
    /* SPI_DRV_BUS_1, */
    
    /** Bus number 2 */
    /* SPI_DRV_BUS_2, */
    
    /** Bus number 3 */
    /* SPI_DRV_BUS_3, */
    
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


/******************************************************************************
*   Enum      :   eSPIDrvSpeedType
******************************************************************************/
/**
 *  @brief   SPI driver speed enumeration type
 *
 *****************************************************************************/
typedef enum
{
    /** Clock speed 1.56 MHz */
    SPI_DRV_SPEED_1DOT56 = 0,
    
    /** Clock speed 12.5 MHz */
    SPI_DRV_SPEED_12DOT5,
    
    /** Max support clock speed */
    SPI_DRV_MAX_SPEED
} eSPIDrvSpeedType;


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
extern int SPIWriteRead(
                         /** access information */
                         sSPIDrvInfoType *psSPIDrvInfo
                       );


#endif  /* end of define Lib_SPI_DRV_H */


/* End of code */
