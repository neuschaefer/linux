/*
 * $RCSfile: aess_i2cdrv.h,v $
 * $Revision: 1.1.1.1 $
 * $Date: 2007/10/09 12:14:19 $
 * $Author: christsai $
 *
 * VSC 452 I2C driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#if !defined (AESSI2CDRV_H)
#define AESSI2CDRV_H

#ifdef AESSI2CDRV_C

/* ioctl definitions */
#define DRIVER_NAME "aess_i2cdrv"
#define DRIVER_MODULE "aess_i2cdrv.ko"
#define DRIVER_TYPE "CHAR"

/* IOCTL command */
#define AESS_I2CDRV_IOC_MAGIC       0xB7
#define AESS_I2CDRV_INIT            _IOWR(AESS_I2CDRV_IOC_MAGIC, 0, \
										  sI2CDrvBusInfoType)
#define AESS_I2CDRV_CONFIG          _IOWR(AESS_I2CDRV_IOC_MAGIC, 1, \
										  sI2CDrvBusInfoType)
#define AESS_I2CDRV_WR              _IOWR(AESS_I2CDRV_IOC_MAGIC, 2, \
										  sI2CDrvBufInfoType)
#define AESS_I2CDRV_GET_MSG         _IOWR(AESS_I2CDRV_IOC_MAGIC, 3, \
										  sI2CDrvBufInfoType)
#define AESS_I2CDRV_RESET           _IOWR(AESS_I2CDRV_IOC_MAGIC, 4, \
										  sI2CDrvBusInfoType)
#define AESS_I2CDRV_GET_STATUS      _IOWR(AESS_I2CDRV_IOC_MAGIC, 5, \
										  sI2CDrvBusInfoType)
#define AESS_I2CDRV_GET_HW_STATUS   _IOWR(AESS_I2CDRV_IOC_MAGIC, 6, \
										  sI2CDrvBusInfoType)
#define AESS_I2CDRV_CTRL_HW         _IOWR(AESS_I2CDRV_IOC_MAGIC, 7, \
										  sI2CDrvBusInfoType)


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


#endif   /* AESSI2CDRV_C */

#endif   /* AESSI2CDRV_H */
