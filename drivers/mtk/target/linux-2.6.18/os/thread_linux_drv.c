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
/*-----------------------------------------------------------------------------
 * $RCSfile: thread_linux_drv.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/

/* Middleware header files */
#define __need_timespec

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/prctl.h>

#include "x_start_common.h"
#include "driver_os.h"
#include "sema_linux_drv.h"
#include "thread_linux_drv.h"

/* MHF Linux, -lzhang, Jun/30/2009 */
#include "include/ioctl_inc/ioctl_param.h"
#include "ioctl/adpt_ioctl.h"

#define SCHED_FAKE  (-1)
#define THREAD_STACK_SIZE 64*1024
/*TMP for test ndef*/
#ifdef LINUX_DBG_ENABLE         
    #define THREAD_DBG_INFO printf
#else
    #define THREAD_DBG_INFO(...) 
#endif

static INT32 i4_os_drv_pri_min;
static INT32 i4_os_drv_pri_max;
static VOID *pv_wait_sema_id = (VOID *) NULL;
static TH_WAIT_T **ppt_th_wait = (TH_WAIT_T **) NULL;
static pthread_key_t t_th_key_tag;  /* Used to store the thread specific tag value. */

extern INT32 x_dbg_stmt (const CHAR*  ps_format, ...);


void ioctl_thread_end()
{
   ADPT_IOCTL(IOCTL_THREAD_END, 0);
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_create()
 *
 * Description:
 *      this API creates a thread. it performs
 *      1. allocate memory for thread's wait struct to be used for suspend
 *         and resume.
 *      2. initialize thread's attributes and set scheduling to be realtime,
 *         round-robin (SCHED_RR).
 *      3. scale ui1_pri to fall in underlying OS priority range.
 *      4. call OS driver thread create routine passing the generic main
 *         function "os_drv_thread_main" and thread's wait struct as argument.
 *
 * Inputs:
 *      ps_name: name of this thread, null ('\0') terminated, up to 16
 *               characters excluding '\0' will be used.
 *      z_stacksize: stack size this thread will run on.
 *      ui1_pri: priority of this thread, 1- highest, 255 - lowest.
 *      ui2_flags: thread flag
 *      pf_main: main entry of this thread.
 *
 * Outputs:
 *      pph_th_id: pointer to hold returned thread id.
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NO_RESOURCE: no resources (memory).
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_create(VOID         **ppv_th_id,    /* thread id from driver */
                       const CHAR   *ps_name,
                       SIZE_T       z_stacksize,
                       UINT8        ui1_pri,        /* thread priority */
                       UINT16       ui2_flags,      /* thread flags */
                       VOID         *pv_tag,
                       x_os_drv_thread_main_fct pf_main)
{
    TH_WAIT_T *pt_wait;
    void *start_addr;
    pthread_attr_t t_th_attr;
    struct sched_param t_param;
    INT32 i4_sched_policy[3];
    INT32 i4_ret, i4_index, i4_rc;    

    pt_wait = (TH_WAIT_T *) malloc(sizeof(TH_WAIT_T));
    if (pt_wait == NULL)
    {   
        return(OSR_DRV_NO_RESOURCE);
    }
    
    x_strncpy (pt_wait->s_name, ps_name, MAX_NAME_LEN);
    pt_wait->s_name[MAX_NAME_LEN]=0;

    i4_ret = pthread_attr_init(&t_th_attr);
    if (i4_ret != 0)
    {   
        free(pt_wait);
        return(OSR_DRV_FAIL);
    }

    i4_ret = pthread_attr_setdetachstate(&t_th_attr, PTHREAD_CREATE_DETACHED);
    if (i4_ret != 0)
    {   
        free(pt_wait);
        return(OSR_DRV_FAIL);
    }
    
    /* create a semaphore to be used by suspend/resume */
    i4_ret = x_os_drv_sema_create(&pt_wait->pv_wait_id, X_SEMA_STATE_LOCK);
    if (i4_ret != OSR_DRV_OK)
    {   
        free(pt_wait);
        return(i4_ret);
    }

    /*
     * scale the passed in priority (ui1_pri) to be in the range of
     * min and max priorities the driver supports
     *
     * superuser can specify SCHED_RR or SCHED_FIFO (Realtime, round-robin)
     * otherwise, use default SCHED_OTHER, (set with pthread_attr_init())
     */

    pt_wait->pv_tag = pv_tag;
    pt_wait->ui1_in_pri = ui1_pri;
    pt_wait->ui1_flags = OS_DRV_THREAD_OK;

    pt_wait->pf_main = pf_main;
 
#ifndef CC_SCHED_RR_POLICY
    i4_sched_policy[0] = (INT32) SCHED_OTHER;
    i4_sched_policy[1] = (INT32) SCHED_RR;
#else
    i4_sched_policy[0] = (INT32) SCHED_RR;
    i4_sched_policy[1] = (INT32) SCHED_OTHER;
#endif
    i4_sched_policy[2] = (INT32) SCHED_FAKE;

    i4_index = 0;
    i4_ret = OSR_DRV_FAIL;

    while (i4_sched_policy[i4_index] != (INT32) SCHED_FAKE)
    {

/* Andrew Perepech 

Android does not implement pthread_attr_setinheritsched() and can 
schedule a thread without setting the attribute to PTHREAD_EXPLICIT_SCHED

*/      
#ifndef ANDROID
      
        i4_rc = pthread_attr_setinheritsched(&t_th_attr, PTHREAD_EXPLICIT_SCHED);
        if (i4_rc != 0)
        {
            THREAD_DBG_INFO(" SET INHER failed %x\n", i4_rc);
            ++i4_index;
            continue;
        }
#endif

        /*CHUN modified thread priority fix:
        There is no infor about the policy .
        Value <100 as the RR  
        TODO:Fixed next version to FIFO
       */  
        i4_rc = pthread_attr_setschedpolicy(&t_th_attr,
                                               (int) i4_sched_policy[i4_index]);
      
        if (i4_rc != 0)
        {
            ++i4_index;
            continue;
        }

        /*
         * set thread's priority which is coverted (scaled) to be in the range
         * of pri_min and pri_max of the underlying OS
         * if thread's scheduling policy is SCHED_OTHER (non realtime),
         * the priority is insignificant
         */

        if (i4_sched_policy[i4_index] != (INT32) SCHED_OTHER)
        {            
            DRV_THREAD_PRI(t_param.sched_priority,
                           ui1_pri,
                           i4_os_drv_pri_max,
                           i4_os_drv_pri_min);
           
        }
        else
        {
            t_param.sched_priority = 0;
        }

        i4_rc = pthread_attr_setschedparam(&t_th_attr, &t_param);
        if (i4_rc != 0)
        {
            ++i4_index;
            continue;
        }

        pt_wait->ui1_th_pri = t_param.sched_priority;


       
        /*
         * Linux does not seem to support setstackaddr
         * so, the passed in stacksize is useless
         */
        /*if(z_stacksize == 0)*/
        {
            z_stacksize = THREAD_STACK_SIZE;
        }
        THREAD_DBG_INFO(" ><><><><<< Thread zstack size %d\n", z_stacksize);

#if 0/* MHF Linux - Zheng Huang */
        start_addr = (void *)x_mem_alloc(z_stacksize);
#else
        start_addr = 0;/* = (void *)malloc(z_stacksize);*/
#endif
       
        if (start_addr != 0)
            pthread_attr_setstack(&t_th_attr, start_addr, z_stacksize);
        i4_rc = pthread_create((pthread_t *) ppv_th_id,
                                (pthread_attr_t *) &t_th_attr,
                                os_drv_thread_main,
                                (void *) pt_wait);

        if (i4_rc != 0)
        {
            ++i4_index;
            continue;
        }
        else
        {
            i4_ret = OSR_DRV_OK;    
            break;
        }
    }

    if (i4_ret != OSR_DRV_OK)
    {   
        x_os_drv_sema_delete(pt_wait->pv_wait_id);
        free(pt_wait);
        return(OSR_DRV_FAIL);
    }

    /* hash and queue the struct for later look up */
    i4_index = OS_DRV_TH_GET_HASHTBL_INDEX(*ppv_th_id);

    i4_ret = x_os_drv_sema_lock(pv_wait_sema_id, X_SEMA_OPTION_WAIT);
    if (i4_ret == OSR_DRV_OK)
    {
        SET_LINK_NEXT(pt_wait, ppt_th_wait[i4_index]);
        SET_LINK_HEAD(ppt_th_wait[i4_index], pt_wait);

        i4_ret = x_os_drv_sema_unlock(pv_wait_sema_id);
        if (i4_ret != OSR_DRV_OK)
        {
            SET_LINK_HEAD(ppt_th_wait[i4_index], pt_wait->pt_next);
            SET_LINK_NEXT(pt_wait, NULL);
        }
    }

    if (i4_ret != OSR_DRV_OK)
    {   
        /* when the created thread finally resumes, it checks if the */
        /* pt_wait->ui1_flags is OK before it actually calls the provided */
        /* main function */
        *((VOID **) ppv_th_id) = (VOID *) 0;        /* reset private tag */
        pt_wait->ui1_flags = OS_DRV_THREAD_ERROR;   /* info created thread */
    }

    /* unlock semaphore so that the created thread continues */
    (VOID) sem_post((sem_t *) (pt_wait->pv_wait_id));
    return(i4_ret);
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_exit()
 *
 * Description:
 *      this API exits a thread. it performs
 *      1. get thread id.
 *      2. guard the hash table that houses thread's wait struct.
 *      3. remove thread's wait struct from hash table.
 *      4. call underlying OS thread exit routine.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_exit()
{
    VOID *pv_ret;
    VOID *pv_th_id;             /* driver level thread id */
    TH_WAIT_T *pt_wait, *pt_prev;
    INT32 i4_ret, i4_index;
/*    pthread_attr_t attr;
    void *stackaddr;
    size_t stacksize;*/

    /*Kernel fake thread killing*/
    ioctl_thread_end();

    pv_th_id = (VOID *) pthread_self();

    pt_prev = NULL;
    i4_index = OS_DRV_TH_GET_HASHTBL_INDEX(pv_th_id);

    i4_ret = x_os_drv_sema_lock(pv_wait_sema_id, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_DRV_OK)
    {
        return;
    }

    /* remove thread's waitsema */
    GET_LINK_HEAD(pt_wait, ppt_th_wait[i4_index]);
    while (LINK_NOT_EMPTY(pt_wait, NULL))
    {
        if (pv_th_id == pt_wait->pv_th_id)
        {
            if (LINK_IS_EMPTY(pt_prev, NULL))
            {
                SET_LINK_HEAD(ppt_th_wait[i4_index], pt_wait->pt_next);
            }
            else
            {
                SET_LINK_NEXT(pt_prev, pt_wait->pt_next);
            }

            SET_LINK_NEXT(pt_wait, NULL);
            break;
        }

        pt_prev = pt_wait;
        GET_LINK_NEXT(pt_wait, pt_wait);
    }

    (VOID) x_os_drv_sema_unlock(pv_wait_sema_id);

    if (pt_wait != NULL)
    {
        (VOID) x_os_drv_sema_delete(pt_wait->pv_wait_id);
        free(pt_wait);
    }

    /* pthread_exit() will never return */
    /* v_ret can be used with pthread_join() from other threads */
    /*pthread_attr_getstack(&attr,&stackaddr, &stacksize);
    x_mem_free(stackaddr);*/
    pthread_exit((void *) &pv_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_delay()
 *
 * Description:
 *      this API allows a thread to give up CPU for a duration. it performs
 *      1. convert ui4_delay to seconds and microseconds.
 *      2. call nanosleep() to delay for the duration.
 *
 * Inputs: -
 *      ui4_delay: in millisecond (ms) the thread gives up CPU.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_delay(UINT32 ui4_delay)
{
    struct timespec req;
    struct timespec rem;
    struct timespec* a;
    struct timespec* b;
    extern int nanosleep();
    extern int sleep();

    if (ui4_delay == 0)
    {
        /* relinquish CPU */
#ifndef ANDROID
        pthread_yield();
#else
	sched_yield();
#endif
        return;
    }


    /*nanosleep can only be used less than 1000ms*/
    req.tv_sec = 0;
    req.tv_nsec = (ui4_delay % 1000) * 1000000;

    if (ui4_delay >= 1000)
    {
        int unsleep_second;

        unsleep_second = ui4_delay / 1000;
        while (1)
        {
            unsleep_second = sleep(unsleep_second);
            if (unsleep_second == 0)
            {
                break;
            }
        }
    }

    a = &req;
    b = &rem;
    
    while (nanosleep(a, b) && errno == EINTR)
    {
        struct timespec* tmp = a;
        a = b;
        b = tmp;
    }

    return;

}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_get_pri()
 *
 * Description:
 *      this API gets a thread's priority. it performs
 *      1. call underlying OS scheduling parameter routine.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs:
 *      *pui1_pri: pointer to hold returned thread priority.
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_get_pri(VOID *pv_th_id, UINT8 *pui1_pri)
{
    struct sched_param t_param;
    INT32 i4_policy, i4_ret;
    TH_WAIT_T *pt_wait;


    i4_ret = pthread_getschedparam((pthread_t) pv_th_id,
                                    (int *) &i4_policy,
                                    &t_param);
    if (i4_ret != 0)
    {
        return(OSR_DRV_FAIL);
    }

    if (t_param.sched_priority >= i4_os_drv_pri_min)
    {
        /* superuser to get proper returns */
        IN_THREAD_PRI(*pui1_pri,
                      t_param.sched_priority,
                      i4_os_drv_pri_max,
                      i4_os_drv_pri_min);
    }
    else
    {
        /* otherwise, it returns 0 as priority */
        /* so, return priority from saved */
        pt_wait = os_drv_thread_waitsema(pv_th_id);
        if (LINK_NOT_EMPTY(pt_wait, NULL))
        {
            *pui1_pri = pt_wait->ui1_in_pri;
        }
        else
        {
            *pui1_pri = (UINT8) 255;    /* random, dont care */
        }
    }

    return(OSR_DRV_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_set_pri()
 *
 * Description:
 *      this API sets a thread's priority. it performs
 *      1. scale ui1_pri to fall in underlying OS priority range.
 *      2. call underlying OS scheduling parameter routine to set priority.
 *      4. save ui1_pri in wait struct as back up use.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *      ui1_pri: new thread priority, 1-highest, 255-lowest.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_INV_ARG : invalid thread id, pv_th_id.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_set_pri(VOID *pv_th_id,
                        UINT8 ui1_pri)
{
    TH_WAIT_T *pt_wait;
    INT32 i4_ret, i4_index, i4_rc;
    INT32 i4_sched_policy[3];
    struct sched_param t_param;

    /*
     * set thread's priority which is coverted (scaled) to be in the range
     * of pri_min and pri_max of the underlying OS
     * if thread's scheduling policy is SCHED_OTHER (non realtime),
     * the priority is insignificant
     */
#ifndef CC_SCHED_RR_POLICY
    i4_sched_policy[0] = (INT32) SCHED_OTHER;
    i4_sched_policy[1] = (INT32) SCHED_RR;
#else
    i4_sched_policy[0] = (INT32) SCHED_RR;
    i4_sched_policy[1] = (INT32) SCHED_OTHER;
#endif
    i4_sched_policy[2] = (INT32) SCHED_FAKE;

    i4_index = 0;
    i4_ret = OSR_DRV_FAIL;

    while (i4_sched_policy[i4_index] != (INT32) SCHED_FAKE)
    {
        if (i4_sched_policy[i4_index] != (INT32) SCHED_OTHER)
        {
            DRV_THREAD_PRI(t_param.sched_priority,
                           ui1_pri,
                           i4_os_drv_pri_max,
                           i4_os_drv_pri_min);
            
        }
        else
        {
            t_param.sched_priority = 0;
        }

        i4_rc = pthread_setschedparam((pthread_t) pv_th_id,
                                       (int) i4_sched_policy[i4_index],
                                       &t_param);
        if (i4_rc != 0)
        {
            ++i4_index;
            continue;
        }
        else
        {
            i4_ret = OSR_DRV_OK;
            break;
        }
    }

    if (i4_ret != OSR_DRV_OK)
    {
        return(i4_ret);
    }

    /* look up thread's wait semaphore */
    pt_wait = os_drv_thread_waitsema(pv_th_id);
    if (LINK_IS_EMPTY(pt_wait, NULL))
    {
        return(OSR_DRV_INV_ARG);
    }
    pt_wait->ui1_th_pri = t_param.sched_priority;
    pt_wait->ui1_in_pri = ui1_pri;

    return(OSR_DRV_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_suspend()
 *
 * Description:
 *      this API suspends a thread. it performs
 *      1. get thread id.
 *      2. look up for thread's wait struct based on thread id.
 *      3. set thread wait to lock state.
 *      4. call underlying OS semop with opcode 0 which blocks thread since
 *         thread wait is in lock state.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
x_os_drv_thread_suspend()
{
    VOID *pv_th_id;
    TH_WAIT_T *pt_wait;
    INT32 i4_ret;


    /* get own driver level thread id */
    pv_th_id = (VOID *) pthread_self();

    /* look up thread's wait semaphore */
    pt_wait = os_drv_thread_waitsema(pv_th_id);
    if (LINK_NOT_EMPTY(pt_wait, NULL))
    {
        do {
            /* man page says sem_wait() always returns 0 */
            /* it is not true. so we still have to check errno */
            i4_ret = (INT32) sem_wait(pt_wait->pv_wait_id);

            if ((i4_ret < (INT32) 0) && (errno == EINTR))
            {
                continue;
            }
            else
            {
                break;
            }
        } while (TRUE);
    }
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_resume()
 *
 * Description:
 *      this API resumes a thread. it performs
 *      1. look up for thread's wait struct based on thread id.
 *      2. set thread wait to unlock state.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_INV_ARG : invalid thread id, pv_th_id.
 *      OSR_DRV_FAIL: OS driver related errors.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_resume(VOID *pv_th_id)
{
    TH_WAIT_T *pt_wait;
    INT32 i4_ret;
    INT32 i4_val;

    /* look up thread's wait semaphore */
    pt_wait = os_drv_thread_waitsema(pv_th_id);
    if (LINK_IS_EMPTY(pt_wait, NULL))
    {
        return(OSR_DRV_INV_ARG);
    }

    i4_ret = OSR_DRV_OK;

    /* sem_getval() always returns 0 */
    sem_getvalue((sem_t *) (pt_wait->pv_wait_id), (int *) &i4_val);
    if (i4_val < (INT32) 1)
    {
        i4_val = (INT32) sem_post((sem_t *) (pt_wait->pv_wait_id));

        if (i4_val < (INT32) 0)
        {
            i4_ret = OSR_DRV_FAIL;
        }
    }

    return(i4_ret);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_self()
 *
 * Description:
 *      this API gets the calling thread id. it performs
 *      1. call underlying OS thread self.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *      OSR_DRV_OK : routine was successful.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_self(VOID **ppv_th_id,
                     VOID **ppv_tag)
{

    (*ppv_th_id) = (VOID*) pthread_self();
    (*ppv_tag)   = (VOID*) pthread_getspecific (t_th_key_tag);

    return(OSR_DRV_OK);
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_stack_stats
 *
 * Description:
 *      This function returns the stack size and maximum used stack of a given
 *      thread once this operation is supported.
 *
 * Inputs
 *      pv_thread_id: The thread to get stack states
 *
 * Outputs:
 *      pz_alloc_stack: Pointer of the thread stack size
 *      pz_max_used_stack: Pointer of the maximum used stack size
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_FAIL : OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
x_os_drv_thread_stack_stats (VOID*    pv_th_id,
                             SIZE_T*  pz_alloc_stack,
                             SIZE_T*  pz_max_used_stack)
{
    return(OSR_DRV_INV_ARG);
}


/*----------------------------------------------------------------------------
 * Function: os_drv_thread_waitsema()
 *
 * Description:
 *      this API looks for thread's wait struct. it performs
 *      1. sema lock to guard hash table that houses thread's wait struct.
 *      2. go through the linked list for a wait struct that has pv_th_id.
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs: -
 *
 * Returns:
 *      non NULL : pointer to a thread's wait struct.
 *      NULL : not found.
 *---------------------------------------------------------------------------*/
TH_WAIT_T *
os_drv_thread_waitsema(VOID *pv_th_id)
{
    TH_WAIT_T *pt_wait;
    INT32 i4_ret, i4_index;

    i4_index = OS_DRV_TH_GET_HASHTBL_INDEX(pv_th_id);
    i4_ret = x_os_drv_sema_lock(pv_wait_sema_id, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_DRV_OK)
    {
        return(NULL);
    }

    GET_LINK_HEAD(pt_wait, ppt_th_wait[i4_index]);
    while (LINK_NOT_EMPTY(pt_wait, NULL))
    {
        if (pv_th_id == pt_wait->pv_th_id)
        {
            break;
        }

        GET_LINK_NEXT(pt_wait, pt_wait);
    }

    i4_ret = x_os_drv_sema_unlock(pv_wait_sema_id);
    if (i4_ret != OSR_DRV_OK)
    {
        return(NULL);
    }

    return(pt_wait);
}


/*----------------------------------------------------------------------------
 * Function: os_drv_thread_main()
 *
 * Description:
 *      this routine is the generic thread main entry is OS driver. it performs
 *      1. create a semaphore in wait struct to serve suspend/resume.
 *      2. hash and queue wait struct into hash table.
 *      3. call thread main entry passed in x_os_drv_thread_create().
 *
 * Inputs:
 *      pv_th_id: thread id returned via x_os_drv_thread_create().
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
void *
os_drv_thread_main(void *pv_arg)
{
    TH_WAIT_T *pt_wait = (TH_WAIT_T *) pv_arg;

    /* Store the thread specific tag value. */
    if (pthread_setspecific (t_th_key_tag, ((const void*) pt_wait->pv_tag)) != 0)
    {
        /* Very bad. Should never happen. */
        return (NULL);
    }

    /* wait until the calling thread finishes its operations */
    (VOID) sem_wait(pt_wait->pv_wait_id);

    /* the calling thread retruned not OK in API */
    if (pt_wait->ui1_flags != OS_DRV_THREAD_OK)
    {
        (VOID) x_os_drv_sema_delete(pt_wait->pv_wait_id);
        free(pt_wait);
        return(NULL);
    }

    pt_wait->pv_th_id = (VOID *) pthread_self();
    
#ifndef ANDROID
    prctl(PR_SET_NAME, (unsigned long)&pt_wait->s_name);
#else
    prctl(PR_SET_NAME, (unsigned long)&pt_wait->s_name, 0, 0, 0);
#endif

    (pt_wait->pf_main)();

    return(NULL);
}


/*----------------------------------------------------------------------------
 * Function: os_drv_thread_init()
 *
 * Description:
 *      this routine is called as part of os_drv_init(). it performs
 *      1. get underlying OS min. max. scheduling priorities.
 *      2. create a semaphore to guard thread hash table.
 *      3. allocate space for OS driver thread hash table.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_DRV_OK : routine was successful.
 *      OSR_DRV_NO_RESOURCE : no resource (memory).
 *      OSR_DRV_FAIL : OS driver related error.
 *---------------------------------------------------------------------------*/
INT32
os_drv_thread_init()
{
    INT32 i4_ret;
    SIZE_T z_size;

    i4_os_drv_pri_min = (INT32) sched_get_priority_min(SCHED_RR);
    i4_os_drv_pri_max = (INT32) sched_get_priority_max(SCHED_RR);

    if (pv_wait_sema_id == (VOID *) NULL)
    {
        i4_ret = x_os_drv_sema_create(&pv_wait_sema_id, X_SEMA_STATE_UNLOCK);
        if (i4_ret != OSR_DRV_OK)
        {
            return(INITR_FAIL);
        }
    }

    if (ppt_th_wait == (TH_WAIT_T **) NULL)
    {
        z_size = (SIZE_T) (OS_DRV_THREAD_WAIT_TBL_SIZE * sizeof(TH_WAIT_T *));
        ppt_th_wait = (TH_WAIT_T **) malloc(z_size);
        if (ppt_th_wait != (TH_WAIT_T **) NULL)
        {
            for (i4_ret = 0; i4_ret < OS_DRV_THREAD_WAIT_TBL_SIZE; i4_ret++)
            {
                ppt_th_wait[i4_ret] = (TH_WAIT_T *) NULL;
            }
        }
        else
        {
            x_os_drv_sema_delete(pv_wait_sema_id);
            pv_wait_sema_id = (VOID *) NULL;
            return(INITR_FAIL);
        }
    }

    /* Initialize the common PThread key, which is used to store thread specific data. */
    if (pthread_key_create (&t_th_key_tag, NULL) != 0)
    {
        return (INITR_FAIL);
    }

    return(INITR_OK);
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_thread_self_id()
 *
 * Description:
 *      this API gets the calling thread id.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *      OSR_DRV_OK : routine was successful.
 *---------------------------------------------------------------------------*/
VOID *
x_os_drv_thread_self_id()
{

    return( (VOID *) pthread_self() );
}


