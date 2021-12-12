/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Kernel AES driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
 
#ifndef AESSAESDRV_H
#define AESSAESDRV_H

#ifdef AESSAESDRV_C 

/* IOCTL command */
#define AESS_ENCRPDRV_IOC_MAGIC 0xC2
#define AESS_AESDRV_ENCRYPT     _IOWR(AESS_ENCRPDRV_IOC_MAGIC, 0, sAESInfoType)
#define AESS_AESDRV_DECRYPT     _IOWR(AESS_ENCRPDRV_IOC_MAGIC, 1, sAESInfoType)
#define AESS_AESDRV_TEST        _IOWR(AESS_ENCRPDRV_IOC_MAGIC, 2, int)

/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

/* init flag */
#define INIT_OK                 0x0
#define INIT_FAIL               0x1


/******************************************************************************
*   Enum      :   eAESDrvKeySizeType
******************************************************************************/
/**
 *  @brief   AES driver key size enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** 128 bits */
	AES_DRV_128_BITS = 0,
	
	/** Cipher block chaining, CBC mode */
	AES_DRV_192_BITS,
	
	/** Cipher feedback, CFB mode */
	AES_DRV_256_BITS
	
} eAESDrvKeySizeType;


/******************************************************************************
*   Enum      :   eAESDrvModeType
******************************************************************************/
/**
 *  @brief   AES driver mode enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Electronic code book, ECB mode */
	AES_DRV_MODE_ECB = 0,
	
	/** Cipher block chaining, CBC mode */
	AES_DRV_MODE_CBC,
	
	/** Cipher feedback, CFB mode */
	AES_DRV_MODE_CFB,
	
	/** Output feedback, OFB mode */
	AES_DRV_MODE_OFB,
	
	/** Counter, CTR mode */
	AES_DRV_MODE_CTR,
	
} eAESDrvModeType;


/******************************************************************************
*   STRUCT      :   sAESInfo
******************************************************************************/
/**
 *  @brief   Structure to AES driver related data parameter.
 *
 *****************************************************************************/
typedef struct 
{
    /** Initial key */
    UINT8 *pu8InitKey;
    
    /** Initial vector*/
    UINT8 *pu8InitVector;
    
    /** Pointer to the source buffer */
    UINT8 *pu8Input;
    
    /** Pointer to the target buffer */
    UINT8 *pu8Output;
    
    /** Mode option */
    UINT8 u8Mode;
    
    /** Key size option */
    UINT8 u8KeySize;
    
} sAESInfoType;


#else

			 
#endif   /* AESSAESDRV_C */

#endif   /* AESSEVENTHANDLER_H */
