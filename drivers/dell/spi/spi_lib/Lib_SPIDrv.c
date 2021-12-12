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

#ifndef Lib_SPI_DRV_C
#define Lib_SPI_DRV_C

#include <asm/ioctl.h>                       /* For ioctl */
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "Lib_SPIDrv.h"

/* enable to print out message */
#if 0
#define DEBUGP(format, args...) printf("Lib_SPIDrv: " format, ## args)
#else
#define DEBUGP(format, args...)
#endif

static pthread_mutex_t S_SPIDrvMutex = PTHREAD_MUTEX_INITIALIZER;
static UINT8 S_u8SPIDrvOpenState = 0;
static UINT32 S_u32SPIDrvFD;


int SPIWriteRead(
                  /** access information */
                  sSPIDrvInfoType *psSPIDrvInfo
                )
{
    UINT8 d_path[64] ="/dev/";
    
    DEBUGP("SPIWriteRead\n");
    
    /* get semaphore */
    pthread_mutex_lock(&S_SPIDrvMutex);
    
    /* open if this is the first time to access spi module */
    if (!S_u8SPIDrvOpenState)
    {
        DEBUGP("open spi module\n");
        
        strcat(d_path, DRIVER_NAME);
        
        S_u32SPIDrvFD = open(d_path, O_RDWR, 0);
        
        if (S_u32SPIDrvFD < 0)
        {
            DEBUGP("S_u32SPIDrvFD < 0\n");
            
            /* release semaphore */
            pthread_mutex_unlock(&S_SPIDrvMutex);
            
            return (S_u32SPIDrvFD);
        }
        
        S_u8SPIDrvOpenState = 1;
    }
    
    if (ioctl(S_u32SPIDrvFD, AESS_SPIDRV_WR, (UINT32) psSPIDrvInfo))
    {
        DEBUGP("fail to access spi module\n");
        
        /* release semaphore */
        pthread_mutex_unlock(&S_SPIDrvMutex);
        
        return (Lib_SPI_STATUS_FAIL);
    }
    
    /* release semaphore */
    pthread_mutex_unlock(&S_SPIDrvMutex);
    
    DEBUGP("exit SPIWriteRead\n");
    
    return (Lib_SPI_STATUS_OK);
}


#endif      /* end of define Lib_SPI_DRV_C */

/* End of code */
