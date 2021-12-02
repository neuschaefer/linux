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
 * $RCSfile: rwl_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains Multiple-Reader Single-Writer lock related
 *         function implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "rwlock/rwl_api.h"
#include "rwlock/_rwl.h"

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _free_rwlock_resource
 *
 * Description: Internal API to free rwlock memory and resource. 
 *
 * Inputs:  - Handle to a read-write lock.
 *            Pointer to the rwlock structure.
 *
 * Outputs: - None.
 *
 * Returns: - None.
 *            
 ----------------------------------------------------------------------------*/
static VOID _free_rwlock_resource(HANDLE_T h_rw_lock,  MRSW_LOCK_T *pt_rw_lock)
{
    /*  Safe to remove the handle from handle control structure
        Anymore READ/WRITE/RELEASE lock would receive INVALID_HANDLE
    */
    handle_free(h_rw_lock, TRUE);
    
    /* Free the Read-Write lock data structure and the semaphores. */
    if ( x_sema_delete(pt_rw_lock->h_sem_wait_wr) != OSR_OK ||
         x_sema_delete(pt_rw_lock->h_sem_wait_rd) != OSR_OK )
    {
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_DELETE_WAIT_SEMAPHORE);
    }
    x_mem_free(pt_rw_lock);
}

/*-----------------------------------------------------------------------------
 * Name: rwl_create_lock
 *
 * Description: This API creates a Mutiple-Reader Single-Writer lock.
 *
 * Inputs:  - Pointer to a read-write lock handle.
 *
 * Outputs: - None.
 *
 * Returns: - RWLR_OK     A new lock is initialized successfully.
 *            RWLR_FAIL   Cannot create a lock.
 ----------------------------------------------------------------------------*/
INT32 rwl_create_lock  (HANDLE_T* ph_rw_lock)
{
    MRSW_LOCK_T* pt_rw_lock ;
    /*
       Create the rw lock object.
    */
    pt_rw_lock = (MRSW_LOCK_T*) x_mem_alloc(sizeof(MRSW_LOCK_T));
    if ( pt_rw_lock != NULL ) 
    {
    
        /* Create the various database access semaphores. Simply */
        /* abort if anyone of them cannot be created.            */ 
        if (x_sema_create ( &(pt_rw_lock->h_sem_wait_wr), X_SEMA_TYPE_BINARY, 
                              X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_WR_WAIT_SEMAPHORE);
        }
    
        if (x_sema_create (&(pt_rw_lock->h_sem_wait_rd), X_SEMA_TYPE_BINARY, 
                             X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_RD_WAIT_SEMAPHORE);
        }


        /* Initialize counter variables. */
        pt_rw_lock->ui2_wr_access_cnt    = 0;
        pt_rw_lock->ui2_wr_wait_cnt      = 0;
        pt_rw_lock->ui2_rd_access_cnt    = 0;
        pt_rw_lock->ui2_rd_wait_cnt      = 0;
        pt_rw_lock->ui2_rd_inside_wr_cnt = 0;
        
        /* Initializ the write thread_id variable. */
        pt_rw_lock->h_wr_thread = NULL_HANDLE; 

        /* Set the lock delete status to False.  */
        pt_rw_lock->b_del_flag  = FALSE;
        
        /* create a handle and attach the rw_lock with the handle. */
        if ( handle_alloc( READ_WRITE_LOCK, (VOID*) pt_rw_lock,
                           NULL, rwl_handle_cb, ph_rw_lock ) != HR_OK )
        {
          ABORT(DBG_CAT_HANDLE,DBG_ABRT_OUT_OF_MEMORY);
        }
    } 
    else 
    {
      ABORT(DBG_CAT_MEMORY, DBG_ABRT_OUT_OF_MEMORY);
    }
    return RWLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rwl_read_lock
 *
 * Description: This API locks the shared variable for a read operation.
 *              I case one or more write operations are active, this API will
 *              simply place the current thread into a holding semaphore until
 *              the write operations have concluded. Once this API returnes,
 *              the shared variable is locked for read operations. Note that
 *              the read lock allows for multiple read accesses.
 *
 * Inputs:  - Handle to the read-write lock object.
 *
 * Outputs: - None.
 *
 * Returns: - RWLR_OK             Read-lock has been acquired. 
 *            RWLR_INVALID_HANDLE Invalid read-write lock handle.
 *            RWLR_DELETE_IN_PROGRESS This lock is being deleted.
 *            RWLR_WOULD_BLOCK        This API will block on waiting
 *                                    for the lock.
 ----------------------------------------------------------------------------*/
INT32 rwl_read_lock    (HANDLE_T h_rw_lock, RWL_OPTION_T e_option)
{
    BOOL            b_locked;
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    HANDLE_T        h_my_thread;
 
    e_handle_type = INV_HANDLE_TYPE;

    /* Get the current thread id value. */
    x_thread_self(&h_my_thread);
    
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state = x_crit_start ();
    
    /* Get the type and object reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock) ) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        
        /* Check to to see if the delete operation on this lock
           is in progress.  If the b_del_flag field in
           the Read-Write lock structure is not FALSE (e.g.,
           a delete request on this lock has been issued), then
           this thread will receive a condition indicating
           delete in progress.
        */
        if ( pt_rw_lock->b_del_flag != FALSE )
        {
            x_crit_end(t_state);
            return RWLR_DELETE_IN_PROGRESS;
        }
            
        b_locked = FALSE;
            
        while (! (b_locked) )
        {
            /* Allow read operation if no write operation is  */
            /* active. This means, multiple read operation on */
            /* the component database are allowed.            */
            if ( pt_rw_lock->ui2_wr_access_cnt == 0)
            {
                /* obtained read lock */
                pt_rw_lock->ui2_rd_access_cnt++;
                /*
                   Special check here: if rwlock is deleted, then
                   we should return a invalid status, so the
                   client will not proceed.
                */
                if ( pt_rw_lock->b_del_flag != FALSE )
                {
                    x_crit_end(t_state);
                    rwl_release_lock(h_rw_lock);
                    return RWLR_INVALID_HANDLE;
                }
                else
                {
                    x_crit_end (t_state);
                    b_locked = TRUE;
                }
            }
            else
            {
                /*
                  Assertion check.  Write thread can not be NULL
                */
                if ( pt_rw_lock->h_wr_thread == NULL_HANDLE )
                {
                    ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_LOCK_STATE);
                }
                /*
                  Special case: If the thread attempting the read
                  lock has alread obtain the write-lock, then the
                  read-lock will always succeed.
                */
                if ( pt_rw_lock->h_wr_thread == h_my_thread )
                {
                    pt_rw_lock->ui2_rd_inside_wr_cnt++;
                    x_crit_end (t_state);
                    b_locked = TRUE;
                }
                else
                {
                    /*
                      Normal case: where the thread reguesting the
                      read-lock is different than the write-lock
                      thread.
                    */
                    if ( e_option == RWL_OPTION_WAIT )
                    {    
                        /* Busy with a write access. The read operation */
                        /* must wait until the write one is finished.   */
                        pt_rw_lock->ui2_rd_wait_cnt++;

                        x_crit_end (t_state);
                        if ( x_sema_lock (pt_rw_lock->h_sem_wait_rd,
                                          X_SEMA_OPTION_WAIT) != OSR_OK )
                        {
                            ABORT(DBG_CAT_SEMAPHORE,
                                  DBG_ABRT_CANNOT_LOCK_WAIT_SEMAPHORE);
                        }
                        t_state = x_crit_start ();
                        pt_rw_lock->ui2_rd_wait_cnt--;
                    }
                    else
                    {
                        /* Don't wait for the lock. return immediately */
                        x_crit_end (t_state);
                        return RWLR_WOULD_BLOCK;
                    }
                }
            }
        }
    }
    else
    {
        x_crit_end(t_state);
        return RWLR_INVALID_HANDLE ;
    }
    return RWLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rwl_write_lock
 *
 * Description: This API locks the shared variable for a write / update
 *              operation. In the case where one or more read or
 *              another write operation is active, this API will 
 *              simply place the current thread into a holding 
 *              semaphore until the read or write operations have
 *              concluded. Once this API returnes, the component 
 *              database is locked for write operations.
 *
 * Inputs:  - Handle to the read-write lock object.
 *
 * Outputs: - None.
 *
 * Returns: - RWLR_OK             Write-lock has been acquired. 
 *            RWLR_INVALID_HANDLE Invalid read-write lock handle.
 *            RWLR_DELETE_IN_PROGRESS This lock is in the process of
 *                                    being deleted.
 ----------------------------------------------------------------------------*/
INT32 rwl_write_lock   (HANDLE_T h_rw_lock, RWL_OPTION_T e_option)
{
    BOOL            b_locked;
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    HANDLE_T        h_my_thread;
    
    e_handle_type = INV_HANDLE_TYPE;

    /* Get the current thread id value. */
    x_thread_self(&h_my_thread);
    
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state=x_crit_start();
    
    /* Get the type and object reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        /* Check to to see if the delete operation on this lock
           is in progress.  If the b_del_flag field in
           the Read-Write lock structure is not FALSE (e.g.,
           a delete request on this lock has been issued), then
           this thread will receive a condition indicating
           delete in progress.
        */
        if ( pt_rw_lock->b_del_flag != FALSE )
        {
            x_crit_end(t_state);
            return RWLR_DELETE_IN_PROGRESS;
        }
        
        b_locked = FALSE;

        while (! (b_locked))
        {
            /* Allow write operation ONLY if no other */
            /* read or write operation is active.     */
            if ((pt_rw_lock->ui2_rd_access_cnt == 0)  &&
                (pt_rw_lock->ui2_wr_access_cnt == 0))
            {
                /* obtained write lock and save the
                   current write thread. */
                pt_rw_lock->ui2_wr_access_cnt++;
                pt_rw_lock->h_wr_thread=h_my_thread; 
                /*
                   Special check here: if rwlock is deleted, then
                   we should return a invalid status, so the
                   client will not proceed.
                */
                if ( pt_rw_lock->b_del_flag != FALSE )
                {
                    x_crit_end(t_state);
                    rwl_release_lock(h_rw_lock);
                    return RWLR_INVALID_HANDLE;
                }
                else
                {
                    x_crit_end (t_state);
                    b_locked = TRUE;
                }
            }
            else
            {
                /*
                  Allow the thread that already acquire the write lock
                  to acquire the write lock again, if no read lock is
                  in effect.
                */
                if (  (pt_rw_lock->ui2_rd_access_cnt == 0)    &&
                      (pt_rw_lock->ui2_rd_inside_wr_cnt == 0) &&
                      (pt_rw_lock->ui2_wr_access_cnt >  0)    &&
                      (pt_rw_lock->h_wr_thread == h_my_thread)
                    )
                 {
                     /* obtained write lock */
                     pt_rw_lock->ui2_wr_access_cnt++;
                     /*
                       Special check here: if rwlock is deleted, then
                       we should return a invalid status, so the
                       client will not proceed.
                     */
                     if ( pt_rw_lock->b_del_flag != FALSE )
                     {
                         x_crit_end(t_state);
                         rwl_release_lock(h_rw_lock);
                         return RWLR_INVALID_HANDLE;
                     }
                     else
                     {
                         x_crit_end (t_state);
                         b_locked = TRUE;
                     }
                 }   
                 else
                 {
                     if ( e_option == RWL_OPTION_WAIT )
                     {                         
                         /* Some other READ or WRITE operation is active.
                            Simply */
                         /* put this thread into the waiting queue. */
                         pt_rw_lock->ui2_wr_wait_cnt++;

                         x_crit_end (t_state);              
                         
                         if ( x_sema_lock (pt_rw_lock->h_sem_wait_wr,
                                           X_SEMA_OPTION_WAIT) != OSR_OK )
                         {
                             ABORT(DBG_CAT_SEMAPHORE,
                                   DBG_ABRT_CANNOT_LOCK_WAIT_SEMAPHORE);
                         }
                         
                         t_state = x_crit_start ();

                         pt_rw_lock->ui2_wr_wait_cnt--;
                     }
                     else
                     {
                         /* Don't wait for the lock. return immediately */
                         x_crit_end (t_state);
                         return RWLR_WOULD_BLOCK;
                     }
                 }
            }
        }  /* end of while loop. */
    }
    else 
    {
        x_crit_end(t_state);
        return RWLR_INVALID_HANDLE ;
    }
    return RWLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rwl_release_lock
 *
 * Description: This API unlocks a previously locked shared variable. The
 *              shared variable was locked for a read or write operation.
 *
 * Inputs:  - Handle to the read-write lock object.
 *
 * Outputs: - None.
 *
 * Returns: - RWLR_OK               Read-Write lock has been released. 
 *            RWLR_INVALID_HANDLE   Invalid read-write lock handle.
 ----------------------------------------------------------------------------*/
INT32 rwl_release_lock (HANDLE_T h_rw_lock)
{
    CRIT_STATE_T t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    HANDLE_T        h_my_thread;
    BOOL            b_crit = FALSE;

    BOOL            b_del_flag = FALSE;
    UINT16          ui2_wr_access_cnt = 0;
    UINT16          ui2_wr_wait_cnt   = 0;
    UINT16          ui2_rd_access_cnt = 0;
    UINT16          ui2_rd_wait_cnt   = 0;

#define RWLOCK_COPY_STATE_INFO  {b_del_flag=pt_rw_lock->b_del_flag ; ui2_wr_access_cnt=pt_rw_lock->ui2_wr_access_cnt ; ui2_wr_wait_cnt=pt_rw_lock->ui2_wr_wait_cnt ; ui2_rd_access_cnt = pt_rw_lock->ui2_rd_access_cnt ; ui2_rd_wait_cnt=pt_rw_lock->ui2_rd_wait_cnt;} 
    
    e_handle_type = INV_HANDLE_TYPE;

    /* Get the current thread id value. */
    x_thread_self(&h_my_thread);
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_type_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */    
    t_state = x_crit_start ();
    b_crit = TRUE;
    
    /* Get the type and object reference to the handle. */    
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        /* Assertion: If the read and write counters are both  */
        /* greater than '0' then there is a serious problem.       */
        if ((pt_rw_lock->ui2_rd_access_cnt > 0)  &&
            (pt_rw_lock->ui2_wr_access_cnt > 0))
        {
            ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_LOCK_STATE);
        }

        /*
          Special case:  the read-lock is being released from inside
          a write-lock, in the same thread context.
        */
        if (pt_rw_lock->ui2_rd_inside_wr_cnt > 0 )
        {
            /*
              Assertion check. There must already be
              a thread holding the write-lock.
            */
            if ( ! (pt_rw_lock->ui2_wr_access_cnt > 0) )
            {
                ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_LOCK_STATE);
            }
            
            /*
              Assertion check: the thread that is releaseing
              the read-lock enclosed by the write-lock must
              be the same thread.
            */
            if ( pt_rw_lock->h_wr_thread != h_my_thread )
            {
                ABORT (DBG_CAT_INV_OP, DBG_ABRT_INV_LOCK_STATE);
            }
            
            pt_rw_lock->ui2_rd_inside_wr_cnt--;
            b_crit = FALSE;
            x_crit_end (t_state);

            /*
               Don't wake up other pending read/write operation.
               We must keep the existing write-lock state.
            */
            return RWLR_OK;
        }

        /*
          Normal case: releasing read or write lock.
        */
        if (pt_rw_lock->ui2_rd_access_cnt > 0)
        {
            pt_rw_lock->ui2_rd_access_cnt--;
            
            /*
              Wake up any pending READ
            */
            if (pt_rw_lock->ui2_rd_wait_cnt > 0)
            {
                /* make local copy of rwlock state variables before leaving critical sect */
                RWLOCK_COPY_STATE_INFO ;
                    
                /* If some more read operations are pending, unlock them. */
                b_crit = FALSE;
                x_crit_end (t_state);
            
                if ( x_sema_unlock (pt_rw_lock->h_sem_wait_rd) != OSR_OK )
                {
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_UNLOCK_WAIT_SEMAPHORE);
                }
            }
            else
            {
                /* Wake up any pending WRITE. */
                /* Note: Only allow a write operation if no more */
                /* read operations are active.             */
                if ((pt_rw_lock->ui2_rd_access_cnt == 0) &&
                    (pt_rw_lock->ui2_wr_wait_cnt > 0))
                {
                    /* make local copy of rwlock state variables before leaving critical sect */
                    RWLOCK_COPY_STATE_INFO ;

                    /* Wake up any pending write lock operation. */
                    b_crit = FALSE;
                    x_crit_end (t_state);
                
                    if ( x_sema_unlock (pt_rw_lock->h_sem_wait_wr) != OSR_OK )
                    {
                        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_UNLOCK_WAIT_SEMAPHORE);
                    }
                }
            }
        }
        else
        {
            if ( pt_rw_lock->ui2_wr_access_cnt > 0 )
            {
                /*
                   Asserting check:
                   The thread performing write-unlock should be the
                   same thread that is currently holding the write lock.
                */
                if ( pt_rw_lock->h_wr_thread != h_my_thread )
                {                               
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_INV_LOCK_STATE);
                }
                
                pt_rw_lock->ui2_wr_access_cnt--;
                
                /*
                  Nullified the write-thread only if all the
                  last write lock has been released. 
                */
                if ( pt_rw_lock->ui2_wr_access_cnt == 0 )
                {
                    pt_rw_lock->h_wr_thread = NULL_HANDLE;
                }
                
                /*
                  Wake up any pending WRITE.
                */
                if ((pt_rw_lock->ui2_wr_access_cnt == 0) &&
                    (pt_rw_lock->ui2_wr_wait_cnt > 0))
                {
                    /* make local copy of rwlock state variables before leaving critical sect */
                    RWLOCK_COPY_STATE_INFO ;
                    
                    /* If some more write operations are pending, unlock them.*/
                    b_crit = FALSE;
                    x_crit_end (t_state);
                
                    if ( x_sema_unlock (pt_rw_lock->h_sem_wait_wr) != OSR_OK )
                    {
                        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_UNLOCK_WAIT_SEMAPHORE);
                    }
                }
                else
                {
                    /* Wake up any pending READ. */
                    /* Note: Only allow a read operation if no more */
                    /* write operations are active.           */
                    if ((pt_rw_lock->ui2_wr_access_cnt == 0) &&
                        (pt_rw_lock->ui2_rd_wait_cnt > 0))
                    {
                        /* make local copy of rwlock state variables before leaving critical sect */
                        RWLOCK_COPY_STATE_INFO ;

                        /* If some more read operations are pending, unlock them. */
                        b_crit = FALSE;
                        x_crit_end (t_state);
                    
                        if ( x_sema_unlock (pt_rw_lock->h_sem_wait_rd) != OSR_OK )
                        {
                            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_UNLOCK_WAIT_SEMAPHORE);
                        }
                    }
                }
            }
        }


        if ( b_crit == TRUE )
        {
            RWLOCK_COPY_STATE_INFO ;
            x_crit_end (t_state);
        }
        
        /*
          All read/write are completed, and no other read/write are
          pending, and it is now safe to free lock object and
          its resource.
        */
        if ( b_del_flag        != FALSE       &&
             ui2_wr_access_cnt == 0           &&
             ui2_wr_wait_cnt   == 0           &&
             ui2_rd_access_cnt == 0           &&
             ui2_rd_wait_cnt   == 0
            )
        {
            _free_rwlock_resource(h_rw_lock, pt_rw_lock);
        }
        else
        {
            /*
              Some other read/write are still pending or the
              lock is not marked for deletion, Continue for
              now. 
            */
        }
    }
    else
    {
        x_crit_end(t_state);
        return RWLR_INVALID_HANDLE ;
    }
    return RWLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rwl_delete_lock
 *
 * Description: This API delete the Read-Write locks. 
 *
 * Inputs:  - Handle to the read-write lock object.
 *
 * Outputs: - None.
 *
 * Returns: - RWLR_OK             Read-Write lock has been deleted. 
 *            RWLR_INVALID_HANDLE Invalid read-write lock handle.
 *            RWLR_DELETE_IN_PROGRESS This lock is in the process of
 *                                    being deleted.
 ----------------------------------------------------------------------------*/
INT32 rwl_delete_lock   (HANDLE_T h_rw_lock)
{
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;

    e_handle_type = INV_HANDLE_TYPE;
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state=x_crit_start();
    
    /* Get the reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        /* If the b_del_flag field in the Read-Write lock
           structure is not FALSE, then some other thread has
           call the delete lock operation on this lock. If
           this is the case, then we return with a status indicating
           this condition.
        */    
        if ( pt_rw_lock->b_del_flag == FALSE )
        {
            /* Change the b_del_flag to TRUE value. This 
               prevents any other client from making new READ or WRITE
               requests. */
            pt_rw_lock->b_del_flag = TRUE;
        }
        else
        {
            x_crit_end(t_state);
            return RWLR_DELETE_IN_PROGRESS;
        }
            
        /*
          If all othe read/write has completed and none are pending, then
          it is safe to remove the lock object.

        */
        if ( pt_rw_lock->ui2_wr_access_cnt == 0 &&
             pt_rw_lock->ui2_wr_wait_cnt   == 0 &&
             pt_rw_lock->ui2_rd_access_cnt == 0 &&
             pt_rw_lock->ui2_rd_wait_cnt   == 0
            )
        {
            x_crit_end (t_state);
            _free_rwlock_resource(h_rw_lock,  pt_rw_lock);
        }
        else
        {
            /*
              If there are pending and existing read/write operations, then
              the lock can not be free yet.  Just continue now and
              at some future point the resource for the rwlock will be free
              when the last thread calls the release_lock api.
            */
            x_crit_end(t_state);
        }   
    }
    else 
    {
        x_crit_end(t_state);
        return RWLR_INVALID_HANDLE ;
    }
    return RWLR_OK;
}

/*------------------------------------------------------------------------
 * Name: rwl_read_lock_grabbed
 *
 * Description:  This API checks if the read lock is currently active.
 *               The API returns TRUE, if one or more threads have the
 *               read lock on 'h_rw_lock' handle.  It returns FALSE if no
 *               threads has the read lock on 'h_rw_lock' handle.  
 *
 * Inputs:
 *   h_rw_lock:  Handle to the read-write lock object.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   TRUE        Some thread(s) have grabbed the read lock on the 
 *               specified rwlock handle. 
 *
 *   FALSE       No thread(s) has the read lock on the specified rwlock
 *               handle.
 ------------------------------------------------------------------------*/
BOOL  rwl_read_lock_grabbed(HANDLE_T  h_rw_lock)
{
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    BOOL            b_result;

    e_handle_type = INV_HANDLE_TYPE;
    b_result      = FALSE;
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state=x_crit_start();
    
    /* Get the reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        if ( (pt_rw_lock->ui2_rd_access_cnt > 0 )  ||
             (pt_rw_lock->ui2_rd_inside_wr_cnt > 0 )
            )
        {
            b_result = TRUE;
        }
    }
    
    x_crit_end(t_state);

    return b_result;
}


/*-------------------------------------------------------------------------
 * Name: rwl_write_lock_grabbed
 *
 * Description: This API checks if the write lock on 'h_rw_lock' handle
 *              has been acquired by the caller thread.    The API returns
 *              TRUE only if the current thread has acquired the write lock.
 *              The API returns FALSE if another thread has acquired the
 *              write lock on 'h_rw_lock', or that no write lock on
 *              'h_rw_lock' handle is currently active.
 *
 * Inputs:
 *   h_rw_lock: Handle to the read-write lock object.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   TRUE	    The current thread context (e.g., the thread that calls
 *              this API) has acquired the write lock on specified rwlock
 *              handle. 
 *
 *   FALSE      The current thread context (e.g., the thread that calls
 *              this API) does not have the write lock on the specified
 *              rwlock handle.
 *
 ------------------------------------------------------------------------*/
BOOL  rwl_write_lock_grabbed(HANDLE_T h_rw_lock)
{
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    BOOL            b_result;
    HANDLE_T        h_my_thread;

    /* Get the current thread id value. */
    x_thread_self(&h_my_thread);
    
    e_handle_type = INV_HANDLE_TYPE;
    b_result      = FALSE;
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state=x_crit_start();
    
    /* Get the reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        if ( (pt_rw_lock->h_wr_thread == h_my_thread) &&
             (pt_rw_lock->ui2_wr_access_cnt > 0)
            )
        {
            b_result = TRUE;
        }
    }
    
    x_crit_end(t_state);

    return b_result;
}


/*-------------------------------------------------------------------------
 * Name: rwl_info
 *
 * Description: This API print out information on thread that is waiting
 *              for read-lock or write-lock on the specified rwlock.
 *              Note: this function only does something with CLI_LVL_ALL
 *              turns on.
 *
 * Inputs:
 *   h_rw_lock: Handle to the read-write lock object.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   VOID
 *
 ------------------------------------------------------------------------*/
VOID  rwl_info(HANDLE_T h_rw_lock)
{
    
#ifdef CLI_LVL_ALL
    
    CRIT_STATE_T    t_state;
    HANDLE_TYPE_T   e_handle_type;
    MRSW_LOCK_T     *pt_rw_lock ;
    HANDLE_T        h_thread = NULL_HANDLE;
    CHAR            ac_thread_name[36];
    CHAR*           ac_rw = "read and write access";
    CHAR*           ac_r  = "read access";
    CHAR*           ac_w  = "write access";
    CHAR*           s_ptr = "";

    UINT16          ui2_wr_access_cnt = 0;
    UINT16          ui2_wr_wait_cnt   = 0;
    UINT16          ui2_rd_access_cnt = 0;
    UINT16          ui2_rd_wait_cnt   = 0;
    HANDLE_T        h_sem_wait_wr     = NULL_HANDLE; 
    HANDLE_T        h_sem_wait_rd     = NULL_HANDLE;
    HANDLE_T        h_wr_thread       = NULL_HANDLE;
    
    e_handle_type = INV_HANDLE_TYPE;
    /*
      Start the crtical section here.  If we start the crtical section
      after handle_get_obj(), there is a chance that a context switch
      take place, and that the object will become null (due to delete
      in progress) between the time we get the obj and when obj is used.
    */
    t_state=x_crit_start();
    /* Get the reference to the handle. */
    if ( (handle_get_type_obj (h_rw_lock,
                               &e_handle_type,
                               (VOID**) (&pt_rw_lock)) == HR_OK) &&
         (e_handle_type == READ_WRITE_LOCK)
        )
    {
        /* make local copy of rwlock state variables before leaving critical sect */
        ui2_wr_access_cnt   =  pt_rw_lock->ui2_wr_access_cnt ;
        ui2_wr_wait_cnt     =  pt_rw_lock->ui2_wr_wait_cnt ;
        ui2_rd_access_cnt   =  pt_rw_lock->ui2_rd_access_cnt ;
        ui2_rd_wait_cnt     =  pt_rw_lock->ui2_rd_wait_cnt ;
        h_sem_wait_wr       =  pt_rw_lock->h_sem_wait_wr ;
        h_sem_wait_rd       =  pt_rw_lock->h_sem_wait_rd ;
        h_wr_thread         =  pt_rw_lock->h_wr_thread ;

        x_crit_end(t_state);

        ac_thread_name[0]='\0';
        if ( ui2_rd_wait_cnt > 0 || ui2_wr_wait_cnt > 0  )
        {
            h_thread = NULL_HANDLE;

            if ( ui2_rd_wait_cnt > 0  && ui2_wr_wait_cnt > 0 )
            {
                s_ptr = ac_rw;
            }
            else if ( ui2_rd_wait_cnt > 0 )
            {
                s_ptr = ac_r;
            }
            else if ( ui2_wr_wait_cnt > 0 )
            {
                s_ptr = ac_w;
            }

            if ( ui2_wr_access_cnt > 0 )
            {
                /* get the owner of 'h_sem_wait_wr' semaphore. */
                sema_cli_get_owner(h_sem_wait_wr, &h_thread, ac_thread_name); 
                if ( h_thread != NULL_HANDLE )
                {
                    x_dbg_stmt("Waiting for %s on RWL (%d) which has write lock owned by thread [%d] <%s>\n",
                               s_ptr, h_rw_lock, h_thread, ac_thread_name);
                }
                else
                {
                    x_dbg_stmt("Waiting for %s on RWL (%d) which has write lock owned by thread [%d]\n",
                               s_ptr, h_rw_lock, h_wr_thread);
                }
            }
            else
            {
                if  ( ui2_rd_access_cnt > 0 )
                {
                    h_thread = NULL_HANDLE;
                    /* get the owner of 'h_sem_wait_rd' semaphore. */
                    sema_cli_get_owner(h_sem_wait_rd, &h_thread, ac_thread_name);
                    if ( h_thread != NULL_HANDLE )
                    {
                        x_dbg_stmt("Waiting for %s on RWL (%d) which has read lock owned by thread [%d] <%s>\n",
                                   s_ptr, h_rw_lock, h_thread, ac_thread_name);
                    }
                    else
                    {
                        x_dbg_stmt("Waiting for %s on RWL (%d) which has read lock enabled.\n",
                                   s_ptr, h_rw_lock);
                    }
                }
            }
        }
    }
    else
    {
        x_crit_end(t_state);
        x_dbg_stmt("Handle value [%d] is not a valid Rwlock handle.\n",  h_rw_lock);
    }
#endif  /* CLI_LVL_ALL */
    
    return ;
}
