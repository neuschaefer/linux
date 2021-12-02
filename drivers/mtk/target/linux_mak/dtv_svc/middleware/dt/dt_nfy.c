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
 * $RCSfile: dt_nfy.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains function to notify clients that the
 *         system time has been synchronized with the current time
 *         value from the STT/TOT/TDT data.
 *---------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "dbg/x_dbg.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "_dt.h"
#include "dt/dt_dbg.h"
/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

typedef struct _DT_CTRL_BLK_T_
{
    x_dt_nfy_fct          pf_nfy;
    HANDLE_T              h_client;
} DT_CTRL_BLK_T ;


/*
  Internal variables.
*/
static INT32        i4_max_num_clients = 0;

/*
  Array for storing handle to the notification.
*/
static DT_CTRL_BLK_T* pt_nfy_ctl = NULL;


/*
  Notification module's Semaphore handle, Thread handle, and Message queue
  handle.
*/
static HANDLE_T      h_dt_nfy_sema = NULL_HANDLE;

static HANDLE_T      h_dt_nfy_thread = NULL_HANDLE ;

static HANDLE_T      h_dt_nfy_msg_queue = NULL_HANDLE;

static CHAR*         s_dt_msg_q_name = "dt_nfy_msq";

static CHAR*         s_dt_nfy_thread_name = "dt_nfy_task";

static SIZE_T        z_dt_nfy_thread_stack_sz = 10240;

static UINT8         ui1_dt_nfy_thread_priority = 128;

static SIZE_T        z_dt_nfy_msg_sz = sizeof(DT_NFY_MSG_T);

static UINT16        ui2_dt_nfy_msg_cnt = 64;

/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */

static VOID dt_grab_nfy_sema(VOID)
{
    if ( x_sema_lock( h_dt_nfy_sema, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

static VOID dt_ungrab_nfy_sema(VOID)
{
    if ( x_sema_unlock( h_dt_nfy_sema) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
    return;
}

/* -------------------------------------------------------------------------
      Autofree function for the Datetime library notification handle.
 -------------------------------------------------------------------------- */
static INT32 dt_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     dt_handle_autofree    
};


/*-----------------------------------------------------------------------------
 * Name: dt_handle_free_cb
 *
 * Description: This API is called whenever handle_free() function 
 *              is called.
 *
 * Inputs:  h_hdl         Contains the handle to core db client.
 *          e_type        Contains the core db client handle type and must be
 *                        set to CDB_CLIENT_HANDLE_T.
 *          pv_obj        References the client object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: None
 *
 * Returns: TRUE:   handle control block resource is freed. The handle
 *                  library should continue with freeing of this handle.
 *          FALSE:  handle control block resource is not free. The
 *                  handle library should not delete this handle.
 ---------------------------------------------------------------------------*/
static BOOL dt_handle_free_cb
(
    HANDLE_T       h_hdl,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    INT32          i4_i;
    BOOL           b_rc;

    b_rc=FALSE;
    
    /* Abort if this is not the correct type. */
    if (e_type == DTT_NFY_T )
    {
        /* Lock the internal handle control block. */
        dt_grab_nfy_sema();
        
        i4_i = (INT32) pv_obj;

        /*
          un-Register the client notification and free the handle
        */
        if (i4_i >= 0                           &&
            i4_i < i4_max_num_clients
            )
        {
            if ( pt_nfy_ctl[i4_i].h_client == h_hdl )
            {
                /*
                  Found the allocated the handle, remove the
                  callback function and handle value.
                */
                pt_nfy_ctl[i4_i].pf_nfy   = (x_dt_nfy_fct)NULL;
                pt_nfy_ctl[i4_i].h_client = NULL_HANDLE;
                
                b_rc=TRUE;
            }
        }        

        /* Unlock */
        dt_ungrab_nfy_sema();
    }
    else
    {
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
    }
    
    return b_rc;
}


/*--------------------------------------------------------------------------
 * Name:  pf_dt_nfy_thread
 *
 * Description: DT thread for invoking client's notification function.
 *     
 * Input  -
 *    
 *
 * Return - VOID
 *          
 --------------------------------------------------------------------------*/
static VOID  pf_dt_nfy_thread(VOID*  pv_arg)
{
    SIZE_T             z_size;
    INT32              i4_rc;
    DT_NFY_MSG_T       t_msg;
    UINT16             ui2_index;
    BOOL               b_running;
    DAY_SAV_TIME_T     t_new_dst_state;

    b_running = TRUE;

    z_size = sizeof(DT_NFY_MSG_T);

    DBG_INFO(("{DT} nfy thread started: [%d]\n",h_dt_nfy_msg_queue));
    
    /* x_dbg_stmt("{DT} nfy thread started: [%d]\n",h_dt_nfy_msg_queue); */
    
    while ( b_running   &&
            (h_dt_nfy_msg_queue != NULL_HANDLE)
        )
    {
        i4_rc = x_msg_q_receive(&ui2_index,
                                &t_msg,
                                &z_size,
                                &h_dt_nfy_msg_queue,
                                1,
                                X_MSGQ_OPTION_WAIT);

        if ( i4_rc == OSR_OK             &&
             sizeof(DT_NFY_MSG_T) == z_size
            )
        {
            switch (t_msg.t_msg_type)
            {
                case DT_DAY_LGT_SAV_CHANGED_NOTIFICATION:
                {   
                    t_new_dst_state = (DAY_SAV_TIME_T) t_msg.ui32_val;

                    if ( dt_update_dst(t_new_dst_state) == TRUE )
                    {
                        DBG_INFO(("{DT} nfy thread: notify client with day-light-saving changed. %d\n",
                                  t_new_dst_state));
                        
                        dt_nfy_all_clients(DT_DAY_LGT_SAV_CHANGED,
                                           (TIME_T) 0);
                    }
                }
                break;

                case DT_TZ_OFFSET_CHANGED_NOTIFICATION:
                {
                    DBG_INFO(("{DT} nfy thread: local timezone offset changed notification.\n"));

                    dt_nfy_all_clients(DT_TZ_OFFSET_CHANGED, (TIME_T) 0);
                }
                break;

                case DT_ANALOG_TUNER_NOTIFICATION:
                {
                    DT_ANALOG_BRDCST_API_T* pt_table = NULL ;

                    DBG_INFO(("{DT} nfy thread: process analog tuner notificatin.\n"));

                    pt_table = dt_get_analog_brdcst_functions();

                    if ( (pt_table != NULL)                               &&
                         (pt_table->pf_analog_tuner_nfy_handler != NULL)
                        )
                    {
                        pt_table->pf_analog_tuner_nfy_handler(
                            t_msg.u.t_anal_tuner_info.h_obj,
                            t_msg.u.t_anal_tuner_info.ui1_port,
                            t_msg.u.t_anal_tuner_info.e_nfy_cond,
                            t_msg.u.t_anal_tuner_info.pv_tag,
                            t_msg.u.t_anal_tuner_info.ui4_data);
                    }
                    else
                    {
                        DBG_INFO(("{DT} analog tuner notification fct not registered.\n"));
                    }
                }
                break;
                
                default:
                {
                    DBG_INFO(("{DT} nfy thread: unknown msg received\n"));  
                }
                break;
            }
        }
    }

    /*
      Existing DT notification thread.
      Free resource.
    */
    if ( x_msg_q_delete(h_dt_nfy_msg_queue) == OSR_OK )
    {
        h_dt_nfy_msg_queue = NULL_HANDLE;
    }
    else
    {
        DBG_ERROR(("{DT} can not delete DT message queue.\n"));
    }

    if ( x_sema_delete(h_dt_nfy_sema) == OSR_OK )
    {
        h_dt_nfy_sema = NULL_HANDLE;
    }
    else
    {
        DBG_ERROR(("{DT} can not delete DT notification semaphore.\n"));
    }

    h_dt_nfy_thread = NULL_HANDLE;
    DBG_INFO(("{DT} notification thread terminated.\n"));
    return;
}

/*--------------------------------------------------------------------------
 * Name: dt_send_nfy_msg
 *
 * Description: This API sends the message to the DT notification
 *   thread.
 *
 * Input  -
 *    pt_msg:           Pointer to the DT msg structure.   
 *
 * Return -
 *    DTR_OK:   message sent.
 *    DTR_FAIL: Un-able to send notification msg to DT notification thread. 
 *          
 --------------------------------------------------------------------------*/
INT32 dt_send_nfy_msg
(
    DT_NFY_MSG_T*   pt_msg
)
{
    INT32           i4_rc = DTR_OK;

    if ( h_dt_nfy_msg_queue != NULL_HANDLE                        &&
         x_msg_q_send(h_dt_nfy_msg_queue, pt_msg, sizeof(DT_NFY_MSG_T),
                      ui1_dt_nfy_thread_priority) == OSR_OK
        )
    {
        DBG_INFO(("{DT} send msg to DT notification thread.\n"));
        /* x_dbg_stmt("{DT} send msg to DT notification thread.\n"); */
        i4_rc=DTR_OK;
    }
    else
    {
        DBG_INFO(("{DT} Error: can not send msg to DT notification thread.\n"));
        /* x_dbg_stmt("{DT} Error: can not send msg to DT notification thread.\n");*/
        
        i4_rc=DTR_FAIL;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name: dt_init_nfy
 *
 * Description -   This function initializes the internal structure to
 *                 register client's notification function that will
 *                 be called when the system clock is synchronized with
 *                 the current time.
 *
 * Input 
 *   i4_num        Specifies the number of clients that can regiester
 *                 a notification function.
 *                 
 *
 * Output -- None.
 *
 * Return 
 *   DTR_OK:       The registeration mechanism has been initialized.
 * 
 *   DTR_FAIL      Internal error.
 * 
 --------------------------------------------------------------------------*/
INT32 dt_init_nfy(INT32  i4_num)
{
    INT32     i4_rc ;

    i4_rc = DTR_FAIL;
    
    if (
         pt_nfy_ctl         == NULL            &&
         i4_max_num_clients == 0               && 
         i4_num > 0
        )
    {
        /*
          allocate array to store notification function and the tag.
        */
        pt_nfy_ctl = (DT_CTRL_BLK_T*) x_mem_alloc(sizeof(DT_CTRL_BLK_T) * i4_num);
            
        if ( pt_nfy_ctl != NULL )
        {
            /*
              Create the semaphore to control access to the
              shared internal variables.
            */
            if ( (x_sema_create(&(h_dt_nfy_sema),
                                X_SEMA_TYPE_MUTEX,
                                X_SEMA_STATE_UNLOCK) == OSR_OK)          &&
                 (x_msg_q_create(&h_dt_nfy_msg_queue,
                                 s_dt_msg_q_name,
                                 z_dt_nfy_msg_sz,
                                 ui2_dt_nfy_msg_cnt) == OSR_OK)         &&
                 (x_thread_create(&h_dt_nfy_thread,
                                  s_dt_nfy_thread_name,
                                  z_dt_nfy_thread_stack_sz,
                                  ui1_dt_nfy_thread_priority,
                                  pf_dt_nfy_thread,
                                  sizeof(HANDLE_T),
                                  &h_dt_nfy_msg_queue) == OSR_OK)
                )
                
            {   
                x_memset(pt_nfy_ctl, 0,
                         sizeof(DT_CTRL_BLK_T) * i4_num);

                i4_max_num_clients = i4_num;

                /* register autofree function for the DT notification
                   handle. */
                if ( handle_set_autofree_tbl(HT_GROUP_DT,
                                             apf_autofree_fct)
                     != HR_OK )
                {
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
                }
                
                DBG_INFO(("{DT} Init nfy function successful.\n"));

                /* x_dbg_stmt("{DT} Init nfy function successful.\n"); */
                

                i4_rc = DTR_OK;
            }
            else
            {
                if ( h_dt_nfy_sema != NULL_HANDLE )
                {
                    x_sema_delete(h_dt_nfy_sema);
                }

                if ( h_dt_nfy_msg_queue != NULL_HANDLE )
                {
                    x_msg_q_delete(h_dt_nfy_msg_queue);
                }

                x_mem_free(pt_nfy_ctl);
                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
            }
        }
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dt_nfy_all_clients
 *
 * Description - This API notifies registered client of the condition
 *               change of the system time value: (DT_FREE_RUNNING,
 *               DT_SYNC_RUNNING, DT_SYNC_DISCONT)
 *
 * Input 
 *   t_dt_cond      Condition of system time clock.              
 *
 *   t_delta        Indicate the jump (positive or negative) in the system
 *                  clock time received from STT.
 *
 *                  The discontinuity condition can occurs if the stream
 *                  being playout is re-winded, so the STT suddenly has very
 *                  large negative jump in time.  The 't_dt_cond' is set
 *                  to DT_SYNC_DISCONT.
 *
 * Output -- None.
 *
 * Return 
 *   None
 * 
 --------------------------------------------------------------------------*/
VOID dt_nfy_all_clients(DT_COND_T t_dt_cond, TIME_T t_delta)
{
    INT32                  i4_i;
    VOID*                  pv_tag;

    if (
         pt_nfy_ctl != NULL                       &&
         i4_max_num_clients  >  0
        )
    {
        dt_grab_nfy_sema();

         DBG_INFO(("{DT} Notification: system clock condition: %d delta time: %lld\n",t_dt_cond, t_delta));

        /*
          call the each handle notification function.
        */
        for (i4_i=0; i4_i < i4_max_num_clients ; i4_i++ )
        {
            /*
               Get the handle value.
            */
            if ( pt_nfy_ctl[i4_i].h_client != NULL_HANDLE )
            {
                /* Get the client's private tag value. */
                if ( x_handle_get_tag (pt_nfy_ctl[i4_i].h_client,
                                       &pv_tag) == HR_OK
                    )
                {
                    /*
                      call the notification function.
                    */
                    pt_nfy_ctl[i4_i].pf_nfy(
                        pt_nfy_ctl[i4_i].h_client,
                        pv_tag,
                        t_dt_cond,
                        t_delta);
                }
                else
                {
                    ABORT (DBG_CAT_HANDLE,DBG_ABRT_INVALID_HANDLE_TAG);
                }
            }
        }

        dt_ungrab_nfy_sema();
    }
    return;
}
    
/*--------------------------------------------------------------------------
 * Name: x_dt_reg_nfy_fct
 *
 * Description -   This API registers a notification function that will
 *                 be called by the DateTime library when the system clock
 *                 is synchronized with the current time value from the STT,
 *                 TDT, or TOT, or when the system clock switches to a free
 *                 running state.
 *
 *                 The notification function will be called in the section
 *                 manager callback context.
 *
 *                 Note:  Up to a total of 16 notification functions can be
 *                 registered.
 *
 * Input 
 *   pf_nfy        Notification function provided by the caller.  This
 *                 notification function is called when the system clock
 *                 is synchronized with the current time value from STT,
 *                 or TOT, or TDT.
 *
 *   pv_tag        A tag value provided by the caller. This tag value will
 *                 be passed to the caller provided notification function.
 *
 * Output
 *   ph_dt	       Handle reference to the client's notification function.	
 *
 * Return 
 *   DTR_OK:          The notification function is registered.
 *                    
 *   DTR_INV_ARG      pf_nfy or ph_dt is NULL.
 *                   
 *   DTR_FAIL         Internal error: can not registered the notification
 *                    function.
 *   DTR_OUT_OF_HANDLE  Can not allocate handle.
 --------------------------------------------------------------------------*/
INT32 x_dt_reg_nfy_fct
(
    x_dt_nfy_fct   pf_nfy,
    VOID*          pv_tag,
    HANDLE_T*      ph_dt
)
{
    INT32          i4_rc;
    INT32          i4_i;

    if ( pf_nfy != NULL   &&
         ph_dt  != NULL
        )
    {
        *ph_dt = NULL_HANDLE;
        
        /* Grab the semaphore. */
        dt_grab_nfy_sema();

        /*
          Register the client notification and create a handle
        */
        i4_rc=DTR_FAIL;
        for (i4_i=0; i4_i < i4_max_num_clients ; i4_i++ )
        {
            if ( pt_nfy_ctl[i4_i].pf_nfy == NULL )
            {
                /*
                  Found an empty slot, allocate the
                  handle.  Note: the handle object control
                  block is just the index to the static
                  handle control block array.
                */
                if ( handle_alloc(DTT_NFY_T, (VOID*)(i4_i), pv_tag,
                                  dt_handle_free_cb, ph_dt)
                     == HR_OK
                    )
                {
                    pt_nfy_ctl[i4_i].pf_nfy=pf_nfy;
                    pt_nfy_ctl[i4_i].h_client=*ph_dt;
                    i4_rc=DTR_OK;
                    break;
                }
                else
                {
                    i4_rc=DTR_OUT_OF_HANDLE;
                    break;
                }
            }
        }
            
        /* Release the semaphore. */
        dt_ungrab_nfy_sema();
    }
    else
    {
        i4_rc = DTR_INV_ARG;
    }
    return i4_rc;
}

