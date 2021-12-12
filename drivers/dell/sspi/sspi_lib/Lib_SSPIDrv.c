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

#ifndef Lib_SSPI_DRV_C
#define Lib_SSPI_DRV_C

#include <asm/ioctl.h>                       /* For ioctl */
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#include "Lib_SSPIDrv.h"

/* enable to print out message */
#if 0
#define DEBUGP(format, args...) printf("Lib_SSPIDrv: " format, ## args)
#else
#define DEBUGP(format, args...)
#endif

static pthread_mutex_t S_SSPIDrvMutex = PTHREAD_MUTEX_INITIALIZER;
static UINT8 S_u8SSPIDrvOpenState = 0;
static UINT32 S_u32SSPIDrvFD;


int SSPIWriteRead(
                  /** access information */
                  sSSPIDrvInfoType *psSSPIDrvInfo
                )
{
    UINT8 d_path[64] ="/dev/";
    
    DEBUGP("SSPIWriteRead\n");
    
    /* get semaphore */
    pthread_mutex_lock(&S_SSPIDrvMutex);
    
    /* open if this is the first time to access spi module */
    if (!S_u8SSPIDrvOpenState)
    {
        DEBUGP("open sspi module\n");
        
        strcat(d_path, DRIVER_NAME);
        
        S_u32SSPIDrvFD = open(d_path, O_RDWR, 0);
        
        if (S_u32SSPIDrvFD < 0)
        {
            DEBUGP("S_u32SSPIDrvFD < 0\n");
            
            /* release semaphore */
            pthread_mutex_unlock(&S_SSPIDrvMutex);
            
            return (S_u32SSPIDrvFD);
        }
        
        S_u8SSPIDrvOpenState = 1;
    }
    
    if (ioctl(S_u32SSPIDrvFD, AESS_SSPIDRV_WR, (UINT32) psSSPIDrvInfo))
    {
        DEBUGP("fail to access spi module\n");
        
        /* release semaphore */
        pthread_mutex_unlock(&S_SSPIDrvMutex);
        
        return (Lib_SSPI_STATUS_FAIL);
    }
    
    /* release semaphore */
    pthread_mutex_unlock(&S_SSPIDrvMutex);
    
    DEBUGP("exit SPIWriteRead\n");
    
    return (Lib_SSPI_STATUS_OK);
}


#endif      /* end of define Lib_SPI_DRV_C */

/* End of code */
