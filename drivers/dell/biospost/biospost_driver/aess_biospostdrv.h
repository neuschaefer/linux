/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * BIOS Post Code driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
 
#ifndef AESSBIOSPOSTDRV_H
#define AESSBIOSPOSTDRV_H

#ifdef AESSBIOSPOSTDRV_C 
 

/* IOCTL command */
#define AESS_BIOSPOSTDRV_IOC_MAGIC    0xCF
#define AESS_BIOSPOSTDRV_INIT         _IOWR(AESS_BIOSPOSTDRV_IOC_MAGIC, 0, int)
#define AESS_BIOSPOSTDRV_READ         _IOWR(AESS_BIOSPOSTDRV_IOC_MAGIC, 1, int)
#define AESS_BIOSPOSTDRV_RESET        _IOWR(AESS_BIOSPOSTDRV_IOC_MAGIC, 2, int)


/* For temporary use, because no type.h now */
#define STATUS_OK     0
#define STATUS_FAIL   1
typedef unsigned char       UINT8;
typedef unsigned short int  UINT16;
typedef unsigned long int   UINT32;
typedef u64                 UINT64;

/* init flag */
#define INIT_OK                 0x0
#define INIT_FAIL               0x1
#define OPEN_OK                 0x0
#define OPEN_FAIL               0x1


/*BIOSPOST Interrupt*/
#define KCS_HIB_INT 0x9

  
/*BIOSPOST Register Basse Address*/
#define LPC_REG_BASE_ADDR          WPCM450_VA_KCS


/*BIOS POST Codes FIFO Registers*/
#define BIOS_POST_REG_BASE_ADDR         LPC_REG_BASE_ADDR
#define BIOS_PS_REG_FIFO_LADDR          (BIOS_POST_REG_BASE_ADDR+0x42)
#define BIOS_PS_REG_FIFO_MADDR          (BIOS_POST_REG_BASE_ADDR+0x44)
#define BIOS_PS_REG_FIFO_ENABLE         (BIOS_POST_REG_BASE_ADDR+0x46)
#define BIOS_PS_REG_FIFO_STATUS         (BIOS_POST_REG_BASE_ADDR+0x48)
#define BIOS_PS_REG_FIFO_DATA           (BIOS_POST_REG_BASE_ADDR+0x4A)
#define BIOS_PS_REG_FIFO_MISC_STATUS    (BIOS_POST_REG_BASE_ADDR+0x4C)



/* BIOS interface package and structure definition */
#define BIOSPOST_KFIFO_SIZE     0x400
#define BIOSPOST_KFIFO_SIZE_MINUS_1     (BIOSPOST_KFIFO_SIZE-1)
//#define BIOSPOST_KFIFO_SIZE_MINUS_1     (0x3FF)
#define IOADDRESS1_INIT     0x01
#define IOADDRESS2_INIT     0x02
#define IOADDRESS_NOINIT    0x0
#define NO_FILE_ID          0x0
#define FILE_ID1            0x1
#define FILE_ID2            0x2
#define IO_NUMBER           0x2

/* MICS */
#define SHIFT_0_BITS            0x00
#define SHIFT_1_BITS            0x01


/*BIOS regiser data*/
#define FIFO_READY_INT_ENABLE   0x08
#define FIFO_READY_INT_DISABLE  0xF7
#define FIFO_IOADDR0            0x0
#define FIFO_IOADDR1            0x01
#define FIFO_IOADDR_ENABLE      0x80
#define FIFO_IOADDR_DISABLE     0x80
#define FIFO_DATA_VALID         0x80
#define FIFO_OVERFLOW           0x20
#define FIFO_ADDR_DECODE        0x01
#define FIFO_MATCH_ADDR1        0x00
#define FIFO_MATCH_ADDR2        0x01

#define DUPLICATE_POST_CODE_COUNT 10
#define POST_CODE_INTS_NO_ACTION    0
#define POST_CODE_INTS_DISABLE         1

/******************************************************************************
*   STRUCT      :    sBIOSPostData
******************************************************************************/
/**
 *  @brief    BIOS Post Parameters definition from User space
 *
 *****************************************************************************/
typedef struct 
{

  /** Read data size Already*/
	UINT16 u16MaxReadLen;

	/** Read data size Already*/
	UINT16 u16CopyLen;    
	
	/** BIOS Post Codes FIFO address 2 LSB*/
	UINT8 u8BIOSPostAddressLSB;
	
	/** BIOS Post Codes FIFO address 2 MSB*/
	UINT8 u8BIOSPostAddressMSB;
	
	/** I/O Address Enable Flag*/
	UINT8 u8IOAddrEnFlag;

	/** Reserved*/
	UINT8 u8Reserved;

	/** Data buffer one byte len*/
	UINT8 *pu8Data;
		
} sBIOSPostInfo;

/******************************************************************************
*   STRUCT      :   sBIOSFIFOtruct
******************************************************************************/
/**
 *  @brief   Structure to BIOS FIFO driver related buffer, 
 *           used by linux driver.
 *
 *****************************************************************************/
typedef struct{

  /* data is input index*/
	UINT16 u16In;	

  /* data is output index*/
	UINT16 u16Out;	
	
	/* data is output index*/
	UINT16 u16Size;	
	
	/* data is output index*/
	UINT16 Reserved;	

	/* data buffer*/
	UINT8 au8Buffer[BIOSPOST_KFIFO_SIZE];

} sBIOSFIFOStruct;


/******************************************************************************
*   STRUCT      :   sBIOStruct
******************************************************************************/
/**
 *  @brief   Structure to BIOS driver related data parameter, 
 *           used by linux driver.
 *
 *****************************************************************************/
typedef struct 
{
	/** File ID*/
	UINT8 u8FileID;

	/** Record whether IO Address1 initialized*/
	UINT8  u8IOAddrInitFlag;

	/** BIOS Post Codes FIFO address 2 LSB*/
	UINT8 u8BIOSPostAddressLSB;
	
	/** BIOS Post Codes FIFO address 2 MSB*/
	UINT8 u8BIOSPostAddressMSB;


  /*FIFO for I/O Address1*/
	sBIOSFIFOStruct sIOAddrBuf;

} sBIOSPostStruct;

/* Declear BIOS Post Codes data structure */
static sBIOSPostStruct S_aBIOSPostData[IO_NUMBER];




/******************************************************************************
*   STRUCT      :   Function Prototype 
******************************************************************************/
/**
 *  @brief   Prototype for each private function.
 *
 *****************************************************************************/

static int aess_biospost_drv_init(sBIOSPostInfo *psInfo, struct file *flip);

static int aess_biospost_read(sBIOSPostInfo *psdata, struct file *flip);

static int aess_biospost_reset(sBIOSPostInfo *psdata, struct file *flip);

static int aess_biospost_clearall(void);

#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
static irqreturn_t aess_biospost_isr(int irq, void *dev_id);
#else
static irqreturn_t aess_biospost_isr(int irq, void *dev_id, struct pt_regs *regs);
#endif


#endif   /* AESSBIOSPOSTDRV_C */

#endif   /* AESSBIOSPOSTDRV_H */
