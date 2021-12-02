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
#include "dt/dt_clock.h"


/* ----------------------------------------------------------------------
  Typedefs   
------------------------------------------------------------------------- */

/* --------------------------------------------------------------
  Function implementation.
---------------------------------------------------------------- */

/*--------------------------------------------------------------------------
 * Name:  dt_clock_nfy_fct_thread_main
 *
 * Description: DT thread main function for invoking 
 *     client's notification function.
 * Input  -
 *    
 *
 * Return - VOID
 *          
 --------------------------------------------------------------------------*/
static VOID  dt_clock_nfy_fct_thread_main(VOID*  pv_arg)
{
    SIZE_T             z_size;
    INT32              i4_rc;
    DT_NFY_MSG_T       t_msg;
    UINT16             ui2_index;
    DAY_SAV_TIME_T     t_new_dst_state;
    DT_CLOCK_T*        pt_clock;
    
    /* pv_arg contains the address of pt_clock structure. */
    pt_clock = *((DT_CLOCK_T**)pv_arg);


    pt_clock->b_running = TRUE;
    z_size = sizeof(DT_NFY_MSG_T);

    DBG_INFO(("{DT} clock [%s] nfy thread started: msg q handle: [%d]\n",pt_clock->s_name, pt_clock->h_dt_nfy_msg_queue));
    
    while ( (pt_clock->b_running == TRUE)                      &&
            (pt_clock->h_dt_nfy_msg_queue != NULL_HANDLE)
        )
    {
        i4_rc = x_msg_q_receive(&ui2_index,
                                &t_msg,
                                &z_size,
                                &(pt_clock->h_dt_nfy_msg_queue),
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
                        DBG_INFO(("{DT} clock [%s] notify client with day-light-saving changed. %d\n",
                                  pt_clock->s_name,t_new_dst_state));
                        
                        dt_clock_invoke_client_notify_fct(pt_clock, DT_DAY_LGT_SAV_CHANGED,(TIME_T) 0);
                    }
                }
                break;

                case DT_TZ_OFFSET_CHANGED_NOTIFICATION:
                {
                    DBG_INFO(("{DT} clock [%s] local timezone offset changed notification.\n",pt_clock->s_name));

                    dt_clock_invoke_client_notify_fct(pt_clock, DT_TZ_OFFSET_CHANGED, (TIME_T) 0);
                }
                break;

                case DT_ANALOG_TUNER_NOTIFICATION:
                {
                    DT_ANALOG_BRDCST_API_T* pt_table = NULL ;

                    DBG_INFO(("{DT} clock [%s] process analog tuner notificatin.\n",pt_clock->s_name));

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

                case DT_CLOCK_EXIT_NOTIFICATION:
                {
                    pt_clock->b_running = FALSE;
                }
                break;

                case DT_CLOCK_FREE_RUNNING_NOTIFICATION:
                {
                    dt_clock_invoke_client_notify_fct(pt_clock,DT_FREE_RUNNING,(TIME_T) 0);
                }
                break;

                case DT_CLOCK_SYNC_RUNNING_NOTIFICATION:
                {
                    dt_clock_invoke_client_notify_fct(pt_clock,DT_SYNC_RUNNING,(TIME_T) 0);
                }
                break;

                case DT_CLOCK_SYNC_DISCONT_NOTIFICATION:
                {
                    dt_clock_invoke_client_notify_fct(pt_clock, DT_SYNC_DISCONT, (TIME_T) t_msg.ui32_val);
                }
                break;

                case DT_CLOCK_NO_SYNC_SRC_NOTIFICATION:
                {
                    dt_clock_invoke_client_notify_fct(pt_clock, DT_NO_SYNC_SRC, (TIME_T) 0);
                }
                break;

                default:
                {
                    DBG_INFO(("{DT} clock [%s] unknown msg type [%d] received\n", pt_clock->s_name,t_msg.t_msg_type));  
                }
                break;
            }
        }
    }

    /*
      Existing DT notification thread.
      Free resource.
    */
    x_msg_q_delete(pt_clock->h_dt_nfy_msg_queue);

    os_release_sys_td(pt_clock->ui2_clock_id);

    /* delete clock semaphore. */
    x_sema_delete(pt_clock->h_sema);
    
    DBG_INFO(("{DT} clock [%s] notification thread terminated\n",pt_clock->s_name));

    /*
      Thread exiting, so now we can null out the clock structure.
    */
    x_memset(pt_clock,0, sizeof(DT_CLOCK_T));
    
    return;
}

/*--------------------------------------------------------------------------
 * Name: dt_clock_send_nfy_msg
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
INT32 dt_clock_send_nfy_msg
(
    DT_CLOCK_CTRL_T*   pt_co,
    DT_NFY_MSG_T*      pt_msg
)
{
    INT32           i4_rc = DTR_OK;

    DBG_INFO(("{DT} clock [%s] send msg to DT notification thread.\n",pt_co->pt_clock->s_name));
    
    if ( pt_co->pt_clock->h_dt_nfy_msg_queue != NULL_HANDLE                        &&
         x_msg_q_send(pt_co->pt_clock->h_dt_nfy_msg_queue, pt_msg,
                      sizeof(DT_NFY_MSG_T),
                      DT_NFY_THREAD_PRIORITY) == OSR_OK
        )
    {
        i4_rc=DTR_OK;
    }
    else
    {
        DBG_INFO(("{DT} Error: clock [%s] can not send msg to DT notification thread.\n",pt_co->pt_clock->s_name));
        i4_rc=DTR_FAIL;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name: dt_clock_init_nfy
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
INT32 dt_clock_init_nfy(DT_CLOCK_CTRL_T*  pt_co)
{
    INT32           i4_rc ;
    DT_CLOCK_T*     pt_clock_addr;
    
    i4_rc = DTR_FAIL;
    CHAR   s_thread_name[MAX_CLOCK_NAME_LEN+3];
    CHAR   s_msg_q_name[MAX_CLOCK_NAME_LEN+3];
    

    /* store the address of pt_clock structure to a local ptr variable */
    pt_clock_addr =  pt_co->pt_clock;

    x_sprintf(s_thread_name,"DT_%d\0", pt_co->h_clock);
    x_sprintf(s_msg_q_name,"DT_%d\0", pt_co->h_clock);
    
    /*
       Create the semaphore to control access to the
       shared internal variables.
    */
    if ( (x_msg_q_create(&(pt_co->pt_clock->h_dt_nfy_msg_queue),
                         s_msg_q_name,
                         DT_NFY_THREAD_MSG_SZ,
                         DT_NFY_THREAD_MSG_CNT) == OSR_OK)         &&
         (x_thread_create(&(pt_co->pt_clock->h_dt_nfy_thread),
                          s_thread_name,
                          DT_NFY_THREAD_STACK_SZ,
                          DT_NFY_THREAD_PRIORITY,
                          dt_clock_nfy_fct_thread_main,
                          sizeof(VOID*),               /* size of content in local ptr variable.
                                                          this is 4 bytes.
                                                        */
                          &pt_clock_addr) == OSR_OK)   /* pass the addr of local ptr
                                                          variable to
                                                          the thread main function. */
        )
    {
        DBG_INFO(("{DT} clock [%s] nfy thread init: msg q handle: %d\n", pt_co->pt_clock->s_name, pt_co->pt_clock->h_dt_nfy_msg_queue));
        i4_rc = DTR_OK;
    }
    else
    {
        ABORT(DBG_CAT_HANDLE,DBG_ABRT_OUT_OF_RESOURCE);
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dt_clock_notify
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
VOID dt_clock_notify
(
    DT_CLOCK_CTRL_T*     pt_co,
    DT_COND_T            t_cond,
    VOID*                pv_data
)
{
    DT_NFY_MSG_T         t_msg;

    /* convert DT_COND_T to notification code. */
    if (  t_cond == DT_FREE_RUNNING )
    {
        t_msg.t_msg_type = DT_CLOCK_FREE_RUNNING_NOTIFICATION;
    }
    else if ( t_cond == DT_SYNC_RUNNING )
    {
        t_msg.t_msg_type = DT_CLOCK_SYNC_RUNNING_NOTIFICATION;
    }
    else if ( t_cond == DT_SYNC_DISCONT )
    {
        t_msg.t_msg_type = DT_CLOCK_SYNC_DISCONT_NOTIFICATION;
    }
    else if ( t_cond == DT_NO_SYNC_SRC )
    {
        t_msg.t_msg_type = DT_CLOCK_NO_SYNC_SRC_NOTIFICATION;
    }
    else if ( t_cond == DT_DAY_LGT_SAV_CHANGED )
    {
        t_msg.t_msg_type = DT_DAY_LGT_SAV_CHANGED_NOTIFICATION;
    }
    else if ( t_cond ==  DT_TZ_OFFSET_CHANGED )
    {
        t_msg.t_msg_type = DT_TZ_OFFSET_CHANGED_NOTIFICATION;
    }
    
    t_msg.ui32_val     = (UINT32) pv_data;

    /* only send the notification message to clock notification thread when
       it is running.
    */
    if ( pt_co->pt_clock->b_running  == TRUE )
    {
        dt_clock_send_nfy_msg(pt_co,&t_msg);
    }
    
    return;
}
    
