/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 On chip peci sensor driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#include <asm/ioctl.h>
#if !defined (AESSPECISENSORDRV_H)
#define AESSPECISENSORDRV_H

#ifdef AESSPECISENSORDRV_C

/* ioctl definitions */
#define AESS_PECIDRV_IOC_MAGIC  0xC8
#define AESS_PECIDRV_R          _IOR(AESS_PECIDRV_IOC_MAGIC, 0, int)
#define AESS_PECIDRV_Q          _IOR(AESS_PECIDRV_IOC_MAGIC, 1, int)
#define AESS_PECIDRV_COMMAND    _IOR(AESS_PECIDRV_IOC_MAGIC, 2, int)

/* For temporary use, because no type.h now */
#define STATUS_OK     0
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;

/* init status */
#define NOT_INIT   0
#define INIT_DONE  1

#define MAX_CPU_NUMBER   4
#define DOMAIN_0         0
#define DOMAIN_1         1
#define DOMAIN_0_CMD     0x01
#define DOMAIN_1_CMD     0x02

#define DOMAIN_NUM_MASK             0xf
#define PROCESS_NUM_MASK            0xf0
#define GET_PROCESS_NUM             4

#define PECI_READING_VALUE_MASK     0xFFFF
#define SHIFT_FRACTIONAL            6

/* Clock Control Register Address */
#define CLA_BA                  (WPCM450_VA_CLK)
#define REG_CLKEN               (CLA_BA + 0x00)

/* AIC  Register Address */
#define AIC_BA                  (WPCM450_VA_AIC)
#define AIC_SCR6                (AIC_BA + 0x18)

/* PECI Register Address */
//#define PECI_BA                 (WPCM450_VA_PECI)
#define PECI_BA                 (WPCM450_VA_UART + 0x200)
#define REG_PECI_CTL_STS        (PECI_BA + 0x00)
#define REG_PECI_RD_LENGTH      (PECI_BA + 0x04)
#define REG_PECI_ADDR           (PECI_BA + 0x08)
#define REG_PECI_CMD            (PECI_BA + 0x0C)
#define REG_PECI_WR_LENGTH      (PECI_BA + 0x1C)
#define REG_PECI_DAT_INOUT0     (PECI_BA + 0x40)
#define REG_PECI_DAT_INOUT1     (PECI_BA + 0x44)
#define REG_PECI_DAT_INOUT2     (PECI_BA + 0x48)
#define REG_PECI_DAT_INOUT3     (PECI_BA + 0x4C)
#define REG_PECI_DAT_INOUT4     (PECI_BA + 0x50)
#define REG_PECI_DAT_INOUT5     (PECI_BA + 0x54)
#define REG_PECI_DAT_INOUT6     (PECI_BA + 0x58)
#define REG_PECI_DAT_INOUT7     (PECI_BA + 0x5C)
#define REG_PECI_DAT_INOUT8     (PECI_BA + 0x60)
#define REG_PECI_DAT_INOUT9     (PECI_BA + 0x64)
#define REG_PECI_DAT_INOUTA     (PECI_BA + 0x68)
#define REG_PECI_DAT_INOUTB     (PECI_BA + 0x6C)
#define REG_PECI_DAT_INOUTC     (PECI_BA + 0x70)
#define REG_PECI_DAT_INOUTD     (PECI_BA + 0x74)
#define REG_PECI_DAT_INOUTE     (PECI_BA + 0x78)
#define REG_PECI_DAT_INOUTF     (PECI_BA + 0x7C)

#define MASK_PECI_WR_LENGTH     0x1F
#define MASK_PECI_RD_LENGTH     0x1F

#define PECI_CLOCK_ENABLED_BIT  6

/******************************************************************************
*   Enum      :   ePECIDrvControlStatus
******************************************************************************/
/**
 *  @brief   PECI driver control status enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Start/Busy */
	START_BUSY = 0,
	
	/** Done */
	DONE,
	
	/** Reserver Bit */
	RESERVER1,
	
	/** CRC Error */
	CRC_ERR,
	
	/** Abort Error */
	ABRT_ERR,
	
	/** Reserver Bit */
	RESERVER2,
	
	/** Done Enable */
	DONE_EN,
	
	/** Reserver Bit */
	RESERVER3
} ePECIDrvControlStatus;

/******************************************************************************
*   Enum      :   ePECIDrvErrorType
******************************************************************************/
/**
 *  @brief   PECI driver error enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** Successful */
	PECI_DRV_OK = 0,
	
	/** General failure */
	PECI_DRV_FAIL,
	
	/** Memory allocation failed */
	PECI_DRV_MEM_FAIL,
	
	/** Memory pool creation failed */
	PECI_DRV_POOL_FAIL,
	
	/** Queue creation failed */
	PECI_DRV_Q_FAIL,
	
	/** Event flags group creation failed */
	PECI_DRV_FLAGS_FAIL,
	
	/** Semaphore creation failed */
	PECI_DRV_SEM_FAIL,
	
	/** Timer creation failed */
	PECI_DRV_TIMER_FAIL
} ePECIDrvErrorType;

/******************************************************************************
*   Enum      :   ePECIDrvErrorStatusType
******************************************************************************/
/**
 *  @brief   PECI driver error status enumeration type
 *
 *****************************************************************************/
typedef enum
{
	/** No errors */
	PECI_DRV_ERROR_NONE = 0,
	
	/** ABORT error */
	PECI_DRV_ERROR_ABORT,
	
	/** CRC error */
	PECI_DRV_ERROR_CRC,
	
	/** Timeout error */
	PECI_DRV_ERROR_TIMEOUT
} ePECIDrvErrorStatusType;

/******************************************************************************
*   Enum      :   ePECIDrvStateType
******************************************************************************/
/**
 *  @brief   PECI driver state machine enumeration
 *
 *****************************************************************************/
typedef enum
{
	/** Idle state */
	PECI_DRV_IDLE_STATE,
	
	/** Transaction state */
	PECI_DRV_TRANS_STATE
} ePECIDrvStateType;

#define PECI_DATA_INOUT_REG_NUMBER  16
volatile static UINT32 REG_PECI_DAT_INOUT[PECI_DATA_INOUT_REG_NUMBER] =
                {
                    (UINT32) REG_PECI_DAT_INOUT0,
                    (UINT32) REG_PECI_DAT_INOUT1,
                    (UINT32) REG_PECI_DAT_INOUT2,
                    (UINT32) REG_PECI_DAT_INOUT3,
                    (UINT32) REG_PECI_DAT_INOUT4,
                    (UINT32) REG_PECI_DAT_INOUT5,
                    (UINT32) REG_PECI_DAT_INOUT6,
                    (UINT32) REG_PECI_DAT_INOUT7,
                    (UINT32) REG_PECI_DAT_INOUT8,
                    (UINT32) REG_PECI_DAT_INOUT9,
                    (UINT32) REG_PECI_DAT_INOUTA,
                    (UINT32) REG_PECI_DAT_INOUTB,
                    (UINT32) REG_PECI_DAT_INOUTC,
                    (UINT32) REG_PECI_DAT_INOUTD,
                    (UINT32) REG_PECI_DAT_INOUTE,
                    (UINT32) REG_PECI_DAT_INOUTF                                                                                                                                                                                                          
                };

/******************************************************************************
*   STRUCT      :   sPECISensorData
******************************************************************************/
/**
 *  @brief   Structure to PECI Sensor driver related data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* Set PECI  Processor domain number */
	UINT8 u8ProcessorDomainNum;     

	/* Set PECI reading value */
	UINT8 u8PECIReading;     

	/* Reserved Byte 1 */
	UINT8 u8Reserved1;
	
	/* Reserved Byte 2  */
	UINT8 u8Reserved2;	
			
} sPECISensorData;

/******************************************************************************
*   STRUCT      :   sPECICommandData
******************************************************************************/
/**
 *  @brief   Structure to PECI driver command data parameter.
 *
 *****************************************************************************/
typedef struct
{
	/* Set PECI Client Address */
	volatile UINT8 u8ClientAddress;     

	/* Set PECI Write Length */
	volatile UINT8 u8WriteLength;     

	/* Set PECI Read Length */
	volatile UINT8 u8ReadLength;     
	
	/* Set PECI Command Code */
	volatile UINT8 u8CommandCode;     	

	/* Set PECI Command Data */
	volatile UINT8 u8CommandData[PECI_DATA_INOUT_REG_NUMBER];   
			
} sPECICommandData;

/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/
 
static int aess_pecisensor_read(sPECISensorData *pPECISensorStruct);
static int aess_pecisensor_query(sPECISensorData *pPECISensorStruct);
static int aess_pecisensor_command(sPECICommandData *pPECICommandData);

#endif   /* AESSpeciSENSORDRV_C */

#endif   /* AESSpeciSENSORDRV_H */
