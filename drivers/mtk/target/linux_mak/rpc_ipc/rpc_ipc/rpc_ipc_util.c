/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <sys/un.h>
#include <errno.h>
#include <time.h>
#include <errno.h>

#include "signal.h"

#include HEADER_COMMON_H

#include "_rpc_ipc_util.h"

#include "_ipc.h"


static OS_FNCT_T  t_os_fct_tbl = {NULL};

/* 
 * The total memory just record the usage, it might not
 * be the real memory used.
 */
static SIZE_T z_total_memory = 0;

VOID * ipc_os_traced_malloc(SIZE_T z_size)
{
    VOID * pv_ret;
    
    pv_ret = malloc(z_size);
    if(pv_ret != NULL)
    {
        z_total_memory ++;        
    }
    if((z_total_memory & 0xff) == 0)
    {
        IPC_LOG("MC M :%d\n", z_total_memory);
    }
    return pv_ret;
}

VOID * ipc_os_traced_calloc(SIZE_T z_num, SIZE_T z_size)
{
    VOID * pv_ret;
    pv_ret = calloc(z_num, z_size);
    if(pv_ret != NULL)
    {
        z_total_memory ++;
    }
    if((z_total_memory & 0xff) == 0)
    {
        IPC_LOG("MC C :%d\n", z_total_memory);
    }

    return pv_ret;
}

VOID ipc_os_traced_free(VOID * pv_addr)
{
    free(pv_addr);

    z_total_memory --;
    if((z_total_memory & 0xff) == 0)
    {
        IPC_LOG("MC F :%d\n", z_total_memory);
    }

}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Init with OS function table.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID * pv_log_key;
VOID rpcu_os_init(const OS_FNCT_T * pt_os_fcts)
{
    if(pt_os_fcts != NULL)
    {
        t_os_fct_tbl = *pt_os_fcts;
    }
    else
    {
        t_os_fct_tbl.pf_thread_create = NULL;
    }
    pv_log_key = rpcu_os_new_tls_key(NULL);
}


BOOL rpcu_tl_log_is_on()
{
    /*return (BOOL)rpcu_os_tls_get(pv_log_key);*/
    return (rpcu_os_tls_get(pv_log_key)? TRUE:FALSE);
}

BOOL rpcu_tl_log_start()
{
    BOOL b_on = rpcu_tl_log_is_on();
    rpcu_os_tls_set(pv_log_key, (VOID *)TRUE);
    return b_on;
}

VOID rpcu_tl_log_end(BOOL b_on)
{
    rpcu_os_tls_set(pv_log_key, (VOID *)(INT32)b_on);
}

typedef struct _PTHD_MAIN_ARG_T
{
    VOID *       pv_arg;
    os_main_fct  pf_main;
}PTHD_MAIN_ARG_T;

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Thread main function wrapper for pthread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static VOID * _pthread_main_wrapper(VOID * pv_arg)
{
    PTHD_MAIN_ARG_T * pt_arg = pv_arg;
    
    pt_arg->pf_main(pt_arg->pv_arg);
    
    RPC_FREE(pt_arg->pv_arg);
    RPC_FREE(pt_arg);  

    return NULL;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Create a thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_create_thread(
    VOID        (* pf_main)(VOID *),
    VOID *      pv_arg,
    SIZE_T      z_size,
    UINT8       ui1_prio,
    SIZE_T      z_stack_size)
{

    INT32 i4_ret;    
    
    if(t_os_fct_tbl.pf_thread_create == NULL)
    {
        pthread_t t_pth;
        PTHD_MAIN_ARG_T * pt_arg = RPC_MALLOC(sizeof(PTHD_MAIN_ARG_T));
        if(pt_arg == NULL)
        {
            return IPCR_OUTOFMEMORY;
        }
        pt_arg->pv_arg  = RPC_MALLOC(z_size);
        if(pt_arg->pv_arg  == NULL)
        {
            RPC_FREE(pt_arg);
            return IPCR_OUTOFMEMORY;
        }

        pt_arg->pf_main = pf_main;
        memcpy(pt_arg->pv_arg, pv_arg, z_size);  

        /* TODO: Fill the attributes */
        i4_ret = pthread_create(&t_pth,
            NULL,
                                _pthread_main_wrapper,
                                pt_arg);
        if(i4_ret != 0)
        {        
            RPC_FREE(pt_arg->pv_arg);
            RPC_FREE(pt_arg);
            return IPCR_OSERR;
        }
    
    }
    else
    {
        return t_os_fct_tbl.pf_thread_create(pf_main,
                                             z_stack_size,
                                             ui1_prio,
                                             z_size,
                                             pv_arg);
    }
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Create semaphore
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_sem_create(INT32 i4_value, VOID ** ppv_sem)
{
    sem_t * pt_sem;

    pt_sem = RPC_MALLOC(sizeof(sem_t));
    sem_init(pt_sem, 0, i4_value);

    *ppv_sem = pt_sem;

    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Delete semaphore.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID rpcu_os_sem_delete(VOID * pv_sem)
{
    sem_t * pt_sem;

    pt_sem = (sem_t *)pv_sem;

    sem_destroy(pt_sem);

    RPC_FREE(pt_sem);
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get real time to timespec structure.  If ui4_timeout is 
 * 0xffffffff, it will return IPCR_TIMEOUT.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
static INT32 _get_rtime(UINT32 ui4_timeout, struct timespec * pt_tm)
{
    if(ui4_timeout != 0xffffffff)
    {
        if(clock_gettime(CLOCK_REALTIME, pt_tm) == -1)
        {
            IPC_ERR("clock_gettime err\n");
            return IPCR_OSERR;
        }
        pt_tm->tv_sec  += ui4_timeout/1000;
        pt_tm->tv_nsec += ui4_timeout - pt_tm->tv_sec * 1000;
        return IPCR_TIMEOUT;
    }
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Lock semaphore.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_sem_lock(VOID * pv_sem)
{
    sem_t * pt_sem;
    INT32   i4_ret;

    IPC_ASSERT(pv_sem != NULL);

    pt_sem = (sem_t *)pv_sem;
    do
    {
        i4_ret = sem_wait(pt_sem);
        if(i4_ret < 0 && errno != EINTR)
        {
            return IPCR_OSERR;
        }
    }while(i4_ret < 0 /*&& errno == EINTR*/);
    
    return IPCR_OK;   
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Lock semaphore with timeout.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_sem_lock_timeout(VOID * pv_sem, UINT32 ui4_timeout)
{
    /* TODO: add timeout */
    sem_t * pt_sem;
    struct timespec t_tm;
    INT32   i4_ret;

    IPC_ASSERT(pv_sem != NULL);

    pt_sem = (sem_t *)pv_sem;

    i4_ret = _get_rtime(ui4_timeout, &t_tm);
    
    if(i4_ret == IPCR_TIMEOUT)
    {
        do{            
            i4_ret = sem_timedwait(pt_sem, &t_tm);
            if(i4_ret < 0)
            {
                if(errno == ETIMEDOUT)
                {
                    return IPCR_TIMEOUT;
                }
                else if(errno != EINTR)
                {
                    return IPCR_OSERR;
                }
            }
        }while(i4_ret < 0 /*&& errno == EINTR*/);
    }
    else if(i4_ret != IPCR_OSERR)
    {
        do
        {
            i4_ret = sem_wait(pt_sem);
            if(i4_ret < 0 && errno != EINTR)
            {
                return IPCR_OSERR;
            }
        }while(i4_ret < 0 /*&& errno == EINTR*/);
    }
    else
    {
        return i4_ret;
    }
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Unlock semaphore.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_sem_unlock(VOID * pv_sem)
{
    sem_t * pt_sem;
    pt_sem = (sem_t *)pv_sem;
    sem_post(pt_sem);

    return IPCR_OK;
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Create mutex.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_mutex_create(VOID ** ppv_mtx)
{
    pthread_mutexattr_t mtx_attr;
    pthread_mutex_t     * pt_mtx;

    pt_mtx = RPC_MALLOC(sizeof(pthread_mutex_t));
    if(pt_mtx == NULL)
    {
        return IPCR_OUTOFMEMORY;
    }
    pthread_mutexattr_init(&mtx_attr);
    pthread_mutexattr_settype(&mtx_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(pt_mtx, &mtx_attr);     
    pthread_mutexattr_destroy(&mtx_attr);

    *ppv_mtx = pt_mtx;
    
    return IPCR_OK;
}
/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Delete mutex.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID rpcu_os_mutex_delete(VOID * pv_mtx)
{
    pthread_mutex_t     * pt_mtx;
    
    pt_mtx = pv_mtx;

    pthread_mutex_destroy(pt_mtx);

    RPC_FREE(pt_mtx); 
   
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Lock mutex.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_mutex_lock(VOID * pv_mtx)
{
    pthread_mutex_lock(pv_mtx);
    
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Lock mutex with timeout.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_mutex_lock_timeout(VOID * pv_mtx, UINT32 ui4_timeout)
{
    pthread_mutex_t *   pt_mtx;
#ifndef ANDROID
    struct timespec     t_tm;
#endif  
    INT32               i4_ret;
    
    IPC_ASSERT(pv_mtx != NULL);

    pt_mtx = pv_mtx;

#ifndef ANDROID
    i4_ret = _get_rtime(ui4_timeout, &t_tm);
    
    if(i4_ret == IPCR_TIMEOUT)
    {
        i4_ret = pthread_mutex_timedlock(pt_mtx, &t_tm);
        if(i4_ret < 0)
        {
            if(errno == ETIMEDOUT)
            {
                return IPCR_TIMEOUT;
            }
            else if(errno != EINTR)
            {
                return IPCR_OSERR;
            }
        }
    }
    else if(i4_ret != IPCR_OSERR)
    {
        i4_ret = pthread_mutex_lock(pt_mtx);
        if(i4_ret < 0)
        {
            return IPCR_OSERR;
        }
    }
    else
    {
        return i4_ret;
    }
    
#else
    if(ui4_timeout != 0xffffffff)
    {
        i4_ret = pthread_mutex_lock_timeout_np(pt_mtx, ui4_timeout);
        if(i4_ret < 0)
        {
            if(errno == EBUSY)
            {
                return IPCR_TIMEOUT;
            }
            else if(errno != EINTR)
            {
               return IPCR_OSERR;
            }
        }
    }
    else
    {
        i4_ret = pthread_mutex_lock(pt_mtx);
        if(i4_ret < 0)
        {
            return IPCR_OSERR;
        }
    }
#endif  
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Unlock mutex.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_mutex_unlock(VOID * pv_mtx)
{
    pthread_mutex_unlock(pv_mtx);
    
    return IPCR_OK;
}



/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get current thread id.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID * rpcu_os_thread_self()
{
    return (VOID *)pthread_self();
}


/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Create Thread Local Storage key.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID * rpcu_os_new_tls_key(VOID (* pf_dtor)(VOID *))
{
    pthread_key_t * pt_key;
    
    pt_key = RPC_MALLOC(sizeof(pthread_key_t));
    if(pt_key == NULL)
    {
        return NULL;
    }
    
    if(pthread_key_create(pt_key, pf_dtor) != 0)
    {   
        RPC_FREE(pt_key);
        return NULL;
    }

    return (VOID *)pt_key;
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Delete TLS key.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID rpcu_os_delete_tls_key(VOID * pv_key)
{
    if(pv_key != NULL)
    {
        pthread_key_delete(*(pthread_key_t *)pv_key);
        RPC_FREE(pv_key);
    }
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Get value from TLS key for current thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
VOID * rpcu_os_tls_get(VOID * pv_key)
{
    if (pv_key == NULL)
    {
        return NULL;
    }

    return pthread_getspecific(*(pthread_key_t *)pv_key);
}

/*------------------------------------------------------------------------
 * Name:  
 *
 * Description: Set value to TLS key for current thread.
 *
 * Inputs: 
 * Outputs: - 
 * Returns: -
 -----------------------------------------------------------------------*/
INT32 rpcu_os_tls_set(VOID * pv_key, VOID * pv_val)
{
    if (pv_key == NULL)
    {
        return IPCR_NOT_OPENED;
    }

    pthread_setspecific(*(pthread_key_t *)pv_key, pv_val);
    return IPCR_OK;
}



