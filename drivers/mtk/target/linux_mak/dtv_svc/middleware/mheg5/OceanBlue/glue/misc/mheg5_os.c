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
 * $RCSfile: mheg5_os.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all os related interfaces between the MHEG-5
 *         stack from OceanBlue and the OSAI.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "dbg/u_dbg.h"
#include "inc/x_common.h"
#include "inc/x_sys_name.h"
#include "handle/handle.h"
#include "handle/x_handle.h"
#include "handle/u_handle.h"
#include "os/inc/x_os.h"
#include "os/inc/u_os.h"
#include "mheg5/u_mheg5.h"

/* OceanBlue header files. */
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_types.h"
#include "mheg5/OceanBlue/mhegPortFuncs/inc/pm_os.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Abort stuff. */
#ifdef ABORT
#undef ABORT
#endif

#define ABORT(_cat, _val) DBG_ABORT ((DBG_MOD_MHEG_5 | _cat | _val))

#define DBG_ABRT_UNKNOWN_THREAD            ((UINT32) 0x00000000)
#define DBG_ABRT_INVALID_THREAD            ((UINT32) 0x00000001)
#define DBG_ABRT_NOT_SUPPORTED             ((UINT32) 0x00000002)
#define DBG_ABRT_CANNOT_DELETE_MSG_Q       ((UINT32) 0x00000003)
#define DBG_ABRT_CANNOT_READ_MSG_Q         ((UINT32) 0x00000004)
#define DBG_ABRT_CANNOT_WRITE_MSG_Q        ((UINT32) 0x00000005)
#define DBG_ABRT_CANNOT_LOCK_SEMA          ((UINT32) 0x00000006)
#define DBG_ABRT_CANNOT_UNLOCK_SEMA        ((UINT32) 0x00000007)
#define DBG_ABRT_CANNOT_DELETE_SEMA        ((UINT32) 0x00000008)
#define DBG_ABRT_CANNOT_DELETE_MUTEX_SEMA  ((UINT32) 0x00000009)
#define DBG_ABRT_CANNOT_LOCK_MUTEX_SEMA    ((UINT32) 0x0000000a)
#define DBG_ABRT_CANNOT_UNLOCK_MUTEX_SEMA  ((UINT32) 0x0000000b)
#define DBG_ABRT_CANNOT_ALLOCATE_MEMORY    ((UINT32) 0x0000000c)
#define DBG_ABRT_CANNOT_CREATE_SEMA        ((UINT32) 0x0000000d)
#define DBG_ABRT_CANNOT_CREATE_MUTEX       ((UINT32) 0x0000000e)

#define MAX_MHEG_5_THREAD_PRIORITY  16    /* MHEG-5 stack has 16 task priority levels. */

#define MSG_Q_SEND_DELAY  20              /* Retry sending a message every 20 msec. */

#define MSG_PRIORITY  128

#define THREAD_NAME  "mheg5_th_%d"
#define MSG_Q_NAME   "mheg5_mq_%d"

#define THREAD_NAME_MAX  SYS_NAME_LEN + 1
#define MSG_Q_NAME_MAX   SYS_NAME_LEN + 1


typedef enum _MHEG5_SEMA_TYPE_T
{
    MHEG5_SEMA_TYPE_BINARY = 0,
    MHEG5_SEMA_TYPE_MUTEX  = 1,
    MHEG5_SEMA_TYPE_COUNT  = 2
} MHEG5_SEMA_TYPE_T;

typedef struct _MHEG5_COUNT_SEMA_T
{
    HANDLE_T h_count_lock; /* Binary semaphore to protect accessing i4_count */
    HANDLE_T h_sema;       /* Binary semaphore to actual lock & unlock */
    INT64    i8_count;
} MHEG5_COUNT_SEMA_T;

typedef struct _MHEG5_BINARY_SEMA_T
{
    HANDLE_T h_sema;
} MHEG5_BINARY_SEMA_T;

typedef struct _MHEG5_MUTEX_SEMA_T
{
    HANDLE_T h_mutex;
} MHEG5_MUTEX_SEMA_T;

typedef struct _MHEG5_SEMA_T
{
    MHEG5_SEMA_TYPE_T e_type;
    
    union
    {
        MHEG5_BINARY_SEMA_T t_binary;
        MHEG5_MUTEX_SEMA_T  t_mutex;
        MHEG5_COUNT_SEMA_T  t_count;
    } u;
} MHEG5_SEMA_T;


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static UINT32  ui4_tick_period;  /* Tick period. */

static UINT16 ui2_last_thread_idx = 0;
static UINT16 ui2_last_msg_q_idx  = 0;

const static UINT8 aui1_priority_cnvt [MAX_MHEG_5_THREAD_PRIORITY] =
{
    255, 248, 240, 232, 224, 216, 208, 200,
    175, 150, 125, 100,  75,  50,  25,   0
};


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSInit
 *
 * Description: This API is called to initialize the OSAI.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSInit (void)
{
    ui4_tick_period = x_os_get_sys_tick_period ();
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSGetClockMilliseconds
 *
 * Description: This API returns the current tick count multiplied by the time
 *              per tick.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The current free running clock in ms.
 ----------------------------------------------------------------------------*/
U32BIT DVP_OSGetClockMilliseconds (void)
{
    return ((U32BIT) (x_os_get_sys_tick () * ui4_tick_period));
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSGetClockDiff
 *
 * Description: This API returns the difference between the current tick count
 *              multiplied by the time per tick and argument 'timestamp'. This
 *              API will also handle roll-over.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: The time differences.
 ----------------------------------------------------------------------------*/
#if 0
U32BIT DVP_OSGetClockDiff (U32BIT  timestamp)
{
    UINT32  ui4_curr_t;
    UINT32  ui4_delta_t;


    ui4_curr_t  = (UINT32) DVP_OSGetClockMilliseconds ();
    ui4_delta_t = 0;

    if (ui4_curr_t >= ((UINT32) timestamp))
    {
        ui4_delta_t = ui4_curr_t - ((UINT32) timestamp);
    }
    else
    {
        ui4_delta_t = 0xffffffff - (((UINT32) timestamp) - ui4_delta_t);
    }

    return (U32BIT)(ui4_delta_t);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  DVP_OSCreateTask
 *
 * Description: This API creates a new task (or thread).
 *
 * Inputs:  function  References the entry point of the task or thread.
 *          param     References the task's parameter list.
 *          stack     Indicates the size of the task stack.
 *          priority  Priority of the created task.
 *          name      Name of the created task.
 *
 * Outputs: -
 *
 * Returns: A reference (or handle) to the created task or NULL.
 ----------------------------------------------------------------------------*/
void* DVP_OSCreateTask (void    (*function) (void*),
                        void*   param,
                        U32BIT  stack,
                        U8BIT   priority,
                        U8BIT*  name)
{
    BOOL     b_done;
    CHAR     ps_thread_name [THREAD_NAME_MAX];
    VOID*    pv_arg;
    INT32    i4_status;
    HANDLE_T h_thread;


    h_thread = NULL_HANDLE;

    if (priority < MAX_MHEG_5_THREAD_PRIORITY)
    {
        pv_arg = (VOID*) &(param);
        b_done = FALSE;

        while (! (b_done))
        {
            /* Create unique thread name. */
            x_sprintf (&(ps_thread_name [0]), THREAD_NAME, ui2_last_thread_idx);

            i4_status = x_thread_create (
                &h_thread,
            #if 0
                &(ps_thread_name [0]),
            #else
                (const CHAR*) &name[0],
            #endif
                ((SIZE_T) stack*2),
                aui1_priority_cnvt [priority],
                function,
                sizeof (VOID*),
                pv_arg);

            if (i4_status != OSR_EXIST)
            {
                /* Either successfull or some error other than OSR_EXIST. */
                b_done = TRUE;

                if (i4_status != OSR_OK)
                {
                    /* Bad news, some other error. Well, simply return a NULL_HANDLE. */
                    h_thread = NULL_HANDLE;
                }
            }

            ui2_last_thread_idx++;
        }
    }

    return ((void*) h_thread);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSDestroyTask
 *
 * Description: This API destroyes a task (or thread).
 *
 * Inputs:  task_id  References the task (or handle).
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSDestroyTask (void*  task_id)
{
    HANDLE_T  h_thread;


    if (x_thread_self (&h_thread) != OSR_OK)
    {
        ABORT (DBG_CAT_THREAD, DBG_ABRT_UNKNOWN_THREAD);
    }

    if ((NULL != task_id) && (((HANDLE_T) task_id) != h_thread))
    {
        ABORT (DBG_CAT_THREAD, DBG_ABRT_INVALID_THREAD);
    }

    x_thread_exit ();
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSTaskDelay
 *
 * Description: This API delays a task by a specific amount of time.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSTaskDelay (U16BIT  timeout)
{
    x_thread_delay ((UINT32) timeout);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSCreateQueue
 *
 * Description: This API creates a message queue.
 *
 * Inputs:  msg_size  Indicates the maximum size of a message.
 *          msg_max   Indicates the maximum number of messages.
 *
 * Outputs: -
 *
 * Returns: A reference (or handle) to the created message queue or NULL.
 ----------------------------------------------------------------------------*/
void* DVP_OSCreateQueue (U16BIT  msg_size,
                         U16BIT  msg_max)
{
    BOOL     b_done;
    CHAR     ps_msg_q_name [MSG_Q_NAME_MAX];
    INT32    i4_status;
    HANDLE_T h_msg_q;


    b_done = FALSE;

    while (! (b_done))
    {
        x_sprintf (&(ps_msg_q_name [0]), MSG_Q_NAME, ui2_last_msg_q_idx);

        i4_status = x_msg_q_create (&h_msg_q, &(ps_msg_q_name [0]), ((SIZE_T) msg_size), ((UINT16) msg_max));

        if (i4_status != OSR_EXIST)
        {
            /* Either successfull or some error other than OSR_EXIST. */
            b_done = TRUE;

            if (i4_status != OSR_OK)
            {
                /* Bad news, some other error. Well, simply return a NULL_HANDLE. */
                h_msg_q = NULL_HANDLE;
            }
        }

        ui2_last_msg_q_idx++;
    }

    return ((void*) h_msg_q);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSDeleteQueue
 *
 * Description: This API deletes a message queue.
 *
 * Inputs:  queue  References a previoulsy created message queue.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSDeleteQueue (void*  queue)
{
    if (x_handle_valid ((HANDLE_T) queue))
    {
        if (x_msg_q_delete ((HANDLE_T) queue) != OSR_OK)
        {
            ABORT (DBG_CAT_MESSAGE, DBG_ABRT_CANNOT_DELETE_MSG_Q);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSReadQueue
 *
 * Description: This API reads a message from the queue. If argument 'timeout'
 *              is set to TIMEOUT_NOW then a quick check is being performed
 *              and if no message is available, the API immediately returns.
 *              If argument 'timeout' is set to TIMEOUT_NEVER then this API
 *              will wait forever for a message to arrive.
 *
 * Inputs:  queue     References a previoulsy created message queue.
 *          msg_size  Contains the message size.
 *          timeout   Contains TIMEOUT_NOW, TIMEOUT_NEVER or an actual
 *                    timeout value.
 *
 * Outputs: msg  Contains the received message if this API returns TRUE.
 *
 * Returns: TRUE if a message was read else FALSE, which indicates timeout.
 ----------------------------------------------------------------------------*/
BOOLEAN DVP_OSReadQueue (void*   queue,
                         void*   msg,
                         U16BIT  msg_size,
                         U16BIT  timeout)
{
    BOOL     b_return = FALSE;
    INT32    i4_status;
    SIZE_T   z_msg_size;
    UINT16   ui2_idx;
    HANDLE_T h_msg_q;


    z_msg_size = (SIZE_T) msg_size;
    h_msg_q    = (HANDLE_T) queue;

    /* to avoid the error value from NUCLEUS */
    if (timeout > 0 && timeout < 5)
    {
        timeout = 5;
    }

    switch (timeout)
    {
        case TIMEOUT_NOW:
            i4_status = x_msg_q_receive (&ui2_idx, msg, &z_msg_size, &h_msg_q, 1, X_MSGQ_OPTION_NOWAIT);
            break;

        case TIMEOUT_NEVER:
            i4_status = x_msg_q_receive (&ui2_idx, msg, &z_msg_size, &h_msg_q, 1, X_MSGQ_OPTION_WAIT);
            break;

        default:
            i4_status = x_msg_q_receive_timeout (&ui2_idx, msg, &z_msg_size, &h_msg_q, 1, ((UINT32) timeout));
            break;
    }

    /* And determine return value. */
    switch (i4_status)
    {
        case OSR_WOULD_BLOCK:
        case OSR_TIMEOUT:
            b_return = FALSE;
            break;

        case OSR_OK:
            b_return = TRUE;
            break;

        default:
            /* Very bad. Something is definitelly not right. */
            /*ABORT (DBG_CAT_MESSAGE, DBG_ABRT_CANNOT_READ_MSG_Q);*/
            break;
    }

    return ((BOOLEAN) b_return);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSWriteQueue
 *
 * Description: This API writes a message to the queue. If argument 'timeout'
 *              is set to TIMEOUT_NOW then a quick write is being performed
 *              and if not succesfull, the API immediately returns. If
 *              argument 'timeout' is set to TIMEOUT_NEVER then this API will
 *              try forever (until successfull) to write the message.
 *
 * Inputs:  queue     References a previoulsy created message queue.
 *          msg       References the actual message.
 *          msg_size  Contains the message size.
 *          timeout   Contains TIMEOUT_NOW, TIMEOUT_NEVER or an actual
 *                    timeout value.
 *
 * Outputs: -
 *
 * Returns: TRUE if a message was written else FALSE, which indicates timeout.
 ----------------------------------------------------------------------------*/
BOOLEAN DVP_OSWriteQueue (void*   queue,
                          void*   msg,
                          U16BIT  msg_size,
                          U16BIT  timeout)
{
    BOOL   b_return;
    BOOL   b_done;
    INT32  i4_status;


    b_done  = FALSE;

    while (! (b_done))
    {
        if ((i4_status = x_msg_q_send (((HANDLE_T) queue), msg, ((SIZE_T) msg_size), MSG_PRIORITY)) == OSR_OK)
        {
            b_done= TRUE;
        }
        else
        {
            if (i4_status == OSR_TOO_MANY)
            {
                /* Hm, message queue is full. Do the delay thing. */
                switch (timeout)
                {
                    case TIMEOUT_NOW:
                        b_done = TRUE;
                        break;

                    case TIMEOUT_NEVER:
                        x_thread_delay (MSG_Q_SEND_DELAY);
                        break;

                    default:
                        if (timeout >= MSG_Q_SEND_DELAY)
                        {
                            x_thread_delay (MSG_Q_SEND_DELAY);

                            timeout -= MSG_Q_SEND_DELAY;
                        }
                        else
                        {
                            x_thread_delay ((UINT32) timeout);

                            timeout = 0;
                        }
                        break;
                }
            }
            else
            {
                /* Very bad. Something is definitelly not right. */
                ABORT (DBG_CAT_MESSAGE, DBG_ABRT_CANNOT_WRITE_MSG_Q);
            }
        }
    }

    b_return = (i4_status == OSR_OK);

    return ((BOOLEAN) b_return);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSCreateSemaphore
 *
 * Description: This API creates a binary semaphore.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference (or handle) to the created semaphore or NULL.
 ----------------------------------------------------------------------------*/
void* DVP_OSCreateSemaphore (void)
{
    MHEG5_SEMA_T* pt_sema = NULL;

    pt_sema = x_mem_calloc(1, sizeof(MHEG5_SEMA_T));
    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_MEMORY, DBG_ABRT_CANNOT_ALLOCATE_MEMORY);
        return NULL;
    }
    
    pt_sema->e_type = MHEG5_SEMA_TYPE_BINARY;

    if (x_sema_create (&(pt_sema->u.t_binary.h_sema), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        x_mem_free(pt_sema);
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMA);
        return NULL;
    }

    return ((void*) pt_sema);
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSSemaphoreWait
 *
 * Description: This API waits on a semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSSemaphoreWait (void*  semaphore)
{
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)semaphore;

    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_BINARY)
    {
        if (x_sema_lock ((pt_sema->u.t_binary.h_sema), X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
        }
    }
    else if (pt_sema->e_type == MHEG5_SEMA_TYPE_COUNT)
    {
        MHEG5_COUNT_SEMA_T* pt_count_sema = (MHEG5_COUNT_SEMA_T*)(&(pt_sema->u.t_count));
        
        if (x_sema_lock (pt_count_sema->h_count_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
        }
        
        /*x_dbg_stmt("DVP_OSSemaphoreWait() -> count:%lld --\n", pt_count_sema->i8_count);*/
        pt_count_sema->i8_count --;

        if (pt_count_sema->i8_count < (INT64)0)
        {
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
            
            if (x_sema_lock (pt_count_sema->h_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
            }
        }
        else
        {        
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
        }
    }
    else /* Mutex should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
    }
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSSemaphoreWaitTimeout
 *
 * Description: This API waits on a semaphore with timeout.
 *
 * Inputs:  semaphore  References the previoulsy created semaphore.
 *          timeout    Contains the timeout in msec.
 *
 * Outputs: -
 *
 * Returns: TRUE if the semaphore was locked or FALSE if the timeout expired.
 ----------------------------------------------------------------------------*/
BOOLEAN DVP_OSSemaphoreWaitTimeout (void*   semaphore,
                                    U16BIT  timeout)
{
    BOOL b_return = FALSE;
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)semaphore;

    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
    }
   
    if (pt_sema->e_type == MHEG5_SEMA_TYPE_BINARY)
    {
        switch (x_sema_lock_timeout ((pt_sema->u.t_binary.h_sema), ((UINT32) timeout)))
        {
            case OSR_OK:
                b_return = TRUE;
                break;

            case OSR_TIMEOUT:
                b_return = FALSE;
                break;

            default:
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
                break;
        }

        return ((BOOLEAN) b_return);
    }
    else if (pt_sema->e_type == MHEG5_SEMA_TYPE_COUNT)
    {
        MHEG5_COUNT_SEMA_T* pt_count_sema = (MHEG5_COUNT_SEMA_T*)(&(pt_sema->u.t_count));
        
        if (x_sema_lock (pt_count_sema->h_count_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
        }
        
        /*x_dbg_stmt("DVP_OSSemaphoreWaitTimeout() -> count:%lld --\n", pt_count_sema->i8_count);*/
        pt_count_sema->i8_count --;

        if (pt_count_sema->i8_count < (INT64)0)
        {
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
            
            switch (x_sema_lock_timeout (pt_count_sema->h_sema, ((UINT32) timeout)))
            {
                case OSR_OK:
                    b_return = TRUE;
                    break;
            
                case OSR_TIMEOUT:
                    b_return = FALSE;
                    break;
            
                default:
                    ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
                    break;
            }

            if (b_return == FALSE)
            {
                if (x_sema_lock (pt_count_sema->h_count_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
                {
                    ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
                }
                
                /*x_dbg_stmt("DVP_OSSemaphoreWaitTimeout() TimeOut!! -> count:%lld ++\n", pt_count_sema->i8_count);*/
                pt_count_sema->i8_count ++;
                
                if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
                {
                    ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
                }
            }
            
            return ((BOOLEAN) b_return);
        }
        else
        {        
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
            
            return ((BOOLEAN) TRUE);
        }
    }
    else /* Mutex should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
        
        return ((BOOLEAN) b_return);
    }
    
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSSemaphoreSignal
 *
 * Description: This API signals a semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSSemaphoreSignal (void*  semaphore)
{
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)semaphore;

    if (pt_sema == NULL)
    {
        /* Very bad. Something is definitelly not right. */
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_BINARY)
    {
        if (x_sema_unlock (pt_sema->u.t_binary.h_sema) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
        }
    }
    else if (pt_sema->e_type == MHEG5_SEMA_TYPE_COUNT)
    {
        MHEG5_COUNT_SEMA_T* pt_count_sema = (MHEG5_COUNT_SEMA_T*)(&(pt_sema->u.t_count));

        if (x_sema_lock (pt_count_sema->h_count_lock, X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            /* Very bad. Something is definitelly not right. */
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMA);
        }
        
        if (pt_count_sema->i8_count < (INT64)0)
        {
            /*x_dbg_stmt("DVP_OSSemaphoreSignal() -> count:%lld ++\n", pt_count_sema->i8_count);*/
            pt_count_sema->i8_count ++;
            
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }

            if (x_sema_unlock (pt_count_sema->h_sema) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
        }
        else
        {
            /*x_dbg_stmt("DVP_OSSemaphoreSignal() -> count:%lld ++\n", pt_count_sema->i8_count);*/
            pt_count_sema->i8_count ++;
            
            if (x_sema_unlock (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
            }
        }
    }
    else /* Mutex should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMA);
    }
}


/*-----------------------------------------------------------------------------
 * Name:  DVP_OSInitCountSemaphore
 *
 * Description: As per discussions with OceanBlue, this API does not need to
 *              be support.
 *
 * Inputs:  semaphore  References the previoulsy created semaphore.
 *          value      Contains the new count value.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
#if 0
void DVP_OSInitCountSemaphore (void*   semaphore,
                               U32BIT  value)
{
    ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_NOT_SUPPORTED);
}
#endif

/*-----------------------------------------------------------------------------
 * Name:  DVP_OSCreateCountSemaphore
 *
 * Description: This API creates a counting semaphore.
 *
 * Inputs:  value  Contains the initial value of the counting semaphore.
 *
 * Outputs: -
 *
 * Returns: A reference (or handle) to the created semaphore or NULL.
 ----------------------------------------------------------------------------*/
void* DVP_OSCreateCountSemaphore (U32BIT  value)
{
    MHEG5_SEMA_T* pt_sema = NULL;

    pt_sema = x_mem_calloc(1, sizeof(MHEG5_SEMA_T));
    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_MEMORY, DBG_ABRT_CANNOT_ALLOCATE_MEMORY);
        return NULL;
    }
    
    pt_sema->e_type = MHEG5_SEMA_TYPE_COUNT;

    pt_sema->u.t_count.i8_count = (INT64)value;
    
    if (x_sema_create (&(pt_sema->u.t_count.h_count_lock), X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_sema);
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMA);
        return NULL;
    }

    if (x_sema_create (&(pt_sema->u.t_count.h_sema), X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) != OSR_OK)
    {
        x_mem_free(pt_sema);
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_MUTEX);
        return NULL;
    }
    
    return ((void*) pt_sema);
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSDeleteSemaphore
 *
 * Description: This API deletes a semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSDeleteSemaphore (void*  semaphore)
{
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)semaphore;

    if (pt_sema == NULL)
    {
        /* Very bad. Something is definitelly not right. */
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_BINARY)
    {
        if (x_handle_valid (pt_sema->u.t_binary.h_sema))
        {
            if (x_sema_delete (pt_sema->u.t_binary.h_sema) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_SEMA);
            }
        }
        
        x_mem_free(pt_sema);
    }    
    else if (pt_sema->e_type == MHEG5_SEMA_TYPE_COUNT)
    {
        MHEG5_COUNT_SEMA_T* pt_count_sema = (MHEG5_COUNT_SEMA_T*)(&(pt_sema->u.t_count));

        if (x_handle_valid (pt_count_sema->h_sema))
        {
            if (x_sema_delete (pt_count_sema->h_sema) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_SEMA);
            }
        }
        
        if (x_handle_valid (pt_count_sema->h_count_lock))
        {
            if (x_sema_delete (pt_count_sema->h_count_lock) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_SEMA);
            }
        }
        
        x_mem_free(pt_sema);
    }
    else /* Mutex should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_SEMA);
    }
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSCreateMutex
 *
 * Description: This API creates a mutex type semaphore.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference (or handle) to the created mutex semaphore or NULL.
 ----------------------------------------------------------------------------*/
void* DVP_OSCreateMutex (void)
{
    MHEG5_SEMA_T* pt_sema = NULL;

    pt_sema = x_mem_calloc(1, sizeof(MHEG5_SEMA_T));
    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_MEMORY, DBG_ABRT_CANNOT_ALLOCATE_MEMORY);
        return NULL;
    }
    
    pt_sema->e_type = MHEG5_SEMA_TYPE_MUTEX;

    if (x_sema_create (&(pt_sema->u.t_mutex.h_mutex), X_SEMA_TYPE_MUTEX, X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_sema);        
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_MUTEX);
        return NULL;
    }
    
    return ((void*) pt_sema);
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSDeleteMutex
 *
 * Description: This API deletes a mutex type semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created mutex semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSDeleteMutex (void*  mutex)
{
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)mutex;

    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_MUTEX_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_MUTEX)
    {
        if (x_handle_valid (pt_sema->u.t_mutex.h_mutex))
        {
            if (x_sema_delete (pt_sema->u.t_mutex.h_mutex) != OSR_OK)
            {
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_MUTEX_SEMA);
            }
        }
        
        x_mem_free(pt_sema);
    }
    else /* Binary & Count should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_DELETE_MUTEX_SEMA);
    }
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSMutexLock
 *
 * Description: This API locks a mutex type semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created mutex semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSMutexLock (void*  mutex)
{
    
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)mutex;

    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_MUTEX_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_MUTEX)
    {
        if (x_sema_lock ((pt_sema->u.t_mutex.h_mutex), X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_MUTEX_SEMA);
        }
    }
    else /* Binary & Count should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_MUTEX_SEMA);
    }
}



/*-----------------------------------------------------------------------------
 * Name:  DVP_OSMutexUnlock
 *
 * Description: This API unlocks a mutex type semaphore.
 *
 * Inputs:  semaphore  References the previoulsy created mutex semaphore.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
void DVP_OSMutexUnlock (void*  mutex)
{
    MHEG5_SEMA_T* pt_sema = (MHEG5_SEMA_T*)mutex;

    if (pt_sema == NULL)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_MUTEX_SEMA);
    }

    if (pt_sema->e_type == MHEG5_SEMA_TYPE_MUTEX)
    {
        if (x_sema_unlock (pt_sema->u.t_mutex.h_mutex) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_MUTEX_SEMA);
        }
    }
    else /* Binary & Count should not be here */
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_MUTEX_SEMA);
    }
}


/*
void DBG_DebugPrintf(char* fmt, ...)
{
}
void DBG_WarningPrintf(char* fmt, ...)
{
}
void DBG_InfoPrintf(char* fmt, ...)
{
}
void DBG_ErrorPrintf(char* fmt, ...)
{
}
*/

