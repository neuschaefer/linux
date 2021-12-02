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
 * $RCSfile: dsm_thread.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains Date Time library function for
 *         interfacing with the RTC driver.
 *---------------------------------------------------------------------------*/

#include "dsm/_dsm.h"
#include "dsm/dsm_nfy.h"
#include "dbg/u_dbg.h"
static VOID pf_dsm_main_fct(VOID*  pv_arg);

/*--------------------------------------------------------------------------
 * Name: dsm_init
 *
 * Description - Initalize the DSM and start the DSM notification dispatch
 *               thread.
 *
 * Input  - t_sec:  current system time as the number of seconds
 *                  since 00:00:00Z, Jan 1, 1970.
 *
 * Return - DTR_OK:  system current time is set.
 *          DTR_FAIL: Can not set current time on the RTC component.
 *          DTR_NO_RTC: No RTC component is registered with resource
 *                      manager.
 --------------------------------------------------------------------------*/
static HANDLE_T     h_dsm_main_thread = NULL_HANDLE;

static CHAR*        s_dsm_thread_name = DSM_DEFAULT_THREAD_NAME;

static SIZE_T       z_dsm_thread_stack_sz = DSM_DEFAULT_STACK_SZ;

static UINT8        ui1_dsm_thread_priority = DSM_DEFAULT_PRIORITY;

static CHAR*        s_dsm_msg_q_name = DSM_MSG_QUEUE_NAME;

static UINT16       z_msg_size = DSM_DEFAULT_MSG_SIZE;

static UINT16       ui2_msg_count = DSM_DEFAULT_NUM_OF_MSG;

static HANDLE_T     h_msg_queue = NULL_HANDLE;

/*
  Global Semaphore Handle for the DSM module to control access to
  shared DSM resources and notification handles.
*/
static HANDLE_T     h_global_sema = NULL_HANDLE;


/*-------------------------------------------------------------------------
     Autofree functions implementation (called by AEE to
     free handle resources).
 --------------------------------------------------------------------------*/
static INT32 dsm_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     dsm_handle_autofree    
};

/*----------------------------------------------------------------------
 * Name: dsm_grab_sema
 *
 * Description: This function locks the SVL object.   The function blocks 
 *              until the lock is acquired.
 *
 * Inputs: 
 *   h_sema     Handle to the semaphore object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   None
 *
 -----------------------------------------------------------------------*/
static VOID dsm_grab_sema
(
    HANDLE_T           h_sema
) 
{
    if ( h_sema != NULL_HANDLE     &&
         x_sema_lock( h_sema, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
}

VOID DSM_GRAB_SEMA(VOID)
{
    dsm_grab_sema(h_global_sema);
}

/*----------------------------------------------------------------------
 * Name: dsm_ungrab_sema
 *
 * Description: This function unlocks the semaphore. 
 *
 * Inputs: 
 *   h_sema     Handle to the semaphore object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID dsm_ungrab_sema
(
    HANDLE_T           h_sema
) 
{
    if ( h_sema != NULL_HANDLE     &&
         x_sema_unlock( h_sema) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
    return; 
}

VOID DSM_UNGRAB_SEMA(VOID)
{
    dsm_ungrab_sema(h_global_sema);
}

/*--------------------------------------------------------------------------
 * Name: dsm_init
 *
 * Description: This API start the DSM dispatch thread.
 *
 * Input  -
 *    t_sync_src_type:  Specifies the types of synchronization source.
 *
 *    t_src_type_desc:  Specifies the source characteristic.
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source. 
 *
 * Return - DTR_OK:   synchronization source is set.
 *          DTR_FAIL: Can not set the source of the synchronization for
 *                    the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_init(DSM_CONFIG_T*  pt_config)
{
    INT32              i4_rc;
    
    /*
      Create the semaphore to control access to the
      shared internal variables.
    */
    if ( h_global_sema == NULL_HANDLE                           &&
         x_sema_create(&(h_global_sema), X_SEMA_TYPE_MUTEX,
                       X_SEMA_STATE_UNLOCK) == OSR_OK )
    {
        /*
          Scan and register all the tuner drivers with
          DSM.
        */
        i4_rc = DSMR_OK;
        if ( dsm_tuner_init() != DSMR_OK )
        {
            i4_rc = DSMR_FAIL;
        }

        if ( dsm_avc_init() != DSMR_OK )
        {
            i4_rc = DSMR_FAIL;
        }

        /* register autofree function for the DSM handle. */
        if ( handle_set_autofree_tbl(HT_GROUP_DSM,
                                     apf_autofree_fct)
             != HR_OK )
        {
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
        }
    }
    else
    {
        x_dbg_stmt("{DSM} Initialization failed: Can not create semaphore.\n");
        return DSMR_FAIL;
    }
    
    if (  h_dsm_main_thread == NULL_HANDLE    &&
          h_msg_queue       == NULL_HANDLE
        )
    {
        /*
          Set the DSM thread stack size, msg queue size, priority, and
          number of msg parameter.  If pt_config is NULL or specified
          input parameter is 0, use the default values. 
        */
        if ( pt_config != NULL )
        {
            if ( pt_config->z_msg_size > 0  )
            {
                z_msg_size = pt_config->z_msg_size;
            }
            if ( pt_config->ui2_msg_count > 0 )
            {
                ui2_msg_count = pt_config->ui2_msg_count;
            }
            if ( pt_config->z_stack_sz > 0 )
            {
                z_dsm_thread_stack_sz = pt_config->z_stack_sz;
            }
            if ( pt_config->ui1_thread_priority > 0 )
            {
                ui1_dsm_thread_priority = pt_config->ui1_thread_priority;
            }
        }
        
        /*
           Create the msg queue.
        */
        if ( x_msg_q_create(&h_msg_queue,
                            s_dsm_msg_q_name,
                            z_msg_size,
                            ui2_msg_count)
             == OSR_OK
        )
        {
            DBG_INFO(("{DSM} DSM msq queue id: %d\n",h_msg_queue));
            /*
              Create DSM thread
            */
            if ( x_thread_create(&h_dsm_main_thread,
                                 s_dsm_thread_name,
                                 z_dsm_thread_stack_sz,
                                 ui1_dsm_thread_priority,
                                 pf_dsm_main_fct,
                                 sizeof(HANDLE_T),
                                 &h_msg_queue) == OSR_OK )
            {
                DBG_INFO(("{DSM} creating dsm thread %d, stack sz: %d "
                          "priority: %d\n",
                          h_dsm_main_thread,
                          z_dsm_thread_stack_sz,
                          ui1_dsm_thread_priority));
    
#ifdef CLI_LVL_ALL
                /*
                   Initialize CLI support
                */
                dsm_cli_init();
#endif        
                i4_rc = DSMR_OK;
            }
            else
            {
                x_dbg_stmt("{DSM} Error: Can not create dsm main thread.\n");
                x_msg_q_delete(h_msg_queue);
                h_msg_queue = NULL_HANDLE;
                i4_rc = DSMR_FAIL;
            }
        }
        else
        {
            x_dbg_stmt("{DSM} Error: Can not create dsm message queue.\n");
            i4_rc = DSMR_FAIL;
        }
    }
    else
    {
        x_dbg_stmt("{DMS} dsm_init(): already initialized.\n");
        i4_rc = DSMR_FAIL;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: x_dsm_stop
 *
 * Description: This API free up DSM resource and terminate the DSM
 *              thread.
 *
 * Input  -
 *    t_sync_src_type:  Specifies the types of synchronization source.
 *
 *    t_src_type_desc:  Specifies the source characteristic.
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source. 
 *
 * Return - DTR_OK:   synchronization source is set.
 *          DTR_FAIL: Can not set the source of the synchronization for
 *                    the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 x_dsm_stop
(
    VOID
)
{
    INT32           i4_rc;
    DSM_MSG_T       t_msg;

    t_msg.e_msg_code = DSM_MSG_CODE_EXIT ;
    i4_rc = dsm_send_msg(&t_msg);

    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: dsm_send_msg
 *
 * Description: This API sends the message to the DSM dispatch thread.
 *
 * Input  -
 *    t_msg
 *
 *    
 *
 * Return -
 *    DTR_OK:   synchronization source is set.
 *    DTR_FAIL: Can not set the source of the synchronization for
 *              the current time. 
 *          
 --------------------------------------------------------------------------*/
INT32 dsm_send_msg
(
    DSM_MSG_T*   pt_msg
)
{
    INT32           i4_rc;

    i4_rc = DSMR_OK;

    if ( h_msg_queue != NULL_HANDLE                        &&
         x_msg_q_send(h_msg_queue, pt_msg, sizeof(DSM_MSG_T),
                      ui1_dsm_thread_priority) == OSR_OK
        )
    {
        DBG_INFO(("{DSM} send msg to DSM dispatch thread.\n"));
        i4_rc=DSMR_OK;
    }
    else
    {
        DBG_INFO(("{DSM} Error: can not send msg to DSM dispatch thread.\n"));
        i4_rc=DSMR_FAIL;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name:  pf_dsm_main_fct
 *
 * Description: DSM thread for dispatching notification to upper
 *              layer.
 *
 * Input  -
 *    t_sync_src_type:  Specifies the types of synchronization source.
 *
 *    t_src_type_desc:  Specifies the source characteristic.
 *
 *    pv_src_info:      Pointer to a structure containing information
 *                      about the synchronization source. 
 *
 * Return - DTR_OK:   synchronization source is set.
 *          DTR_FAIL: Can not set the source of the synchronization for
 *                    the current time. 
 *          
 --------------------------------------------------------------------------*/
static VOID  pf_dsm_main_fct(VOID*  pv_arg)
{

    /* Wait for DSM event message. */
    HANDLE_T           h_queue;
    DSM_MSG_T          t_msg;
    SIZE_T             z_size;
    INT32              i4_rc;
    UINT16             ui2_index;
    BOOL               b_running;
    DSM_COMP_T*        pt_dsm_comp = NULL;

    b_running = TRUE;
    
    h_queue = *((HANDLE_T*) pv_arg);

    z_size = sizeof(DSM_MSG_T);

    DBG_INFO(("{DSM} msg loop started, monitoring msq queue: %d\n",h_queue));
    
    while ( b_running )
    {
        i4_rc = x_msg_q_receive(&ui2_index,
                                 &t_msg,
                                 &z_size,
                                 &h_queue,
                                 1,
                                 X_MSGQ_OPTION_WAIT);

        if ( i4_rc == OSR_OK             &&
             sizeof(DSM_MSG_T) == z_size
            )
        {
            switch (t_msg.e_msg_code)
            {
                case DSM_MSG_CODE_AVC:
                {
                    DBG_INFO(("{DSM} process msq: avc\n"));
                }
                break;

                case DSM_MSG_CODE_TUNER:
                {
                    DBG_INFO(("{DSM} process msq: tuner\n"));
                }
                break;

                case DSM_MSG_CODE_1394:
                {
                    DBG_INFO(("{DSM} process msg: 1394\n"));
                }
                break;

                case DSM_MSG_DEV_NFY:
                {
                    DBG_INFO(("{DSM} send notification to device client handles.\n"));
                }
                break;

                case DSM_MSG_AVC_COMBI_DRVT_NFY:
                {
                    DBG_INFO(("{DSM} process AVC combi driver notifications.\n"));

                    dsm_avc_combi_proc_client_nfy(&t_msg);
                    
                }
                break;

                case DSM_MSG_AVC_SCART_DRVT_NFY:
                {
                    DBG_INFO(("{DSM} process AVC SCART driver notifications.\n"));

                    dsm_avc_scart_proc_client_nfy(&t_msg);
                    
                }
                break;

                

                case DSM_MSG_RM_HANDLE_CLOSED:
                {
                    DBG_INFO(("{DSM} RM handle closed, send notification to client handles and then close the client handle.\n"));

                    /*
                      Free the DSM component structure for this RM handle.
                    */
                    pt_dsm_comp=dsm_get_ref_to_comp(t_msg.h_dsm_comp) ;
                    dsm_close_comp(pt_dsm_comp);
                    
                }
                break;

                case DSM_MSG_RM_TVD_NFY:
                {
                    avc_rm_tv_decoder_nfy_msg_hdlr(
                        t_msg.u.t_rm_tvd_nfy.h_comp,
                        t_msg.u.t_rm_tvd_nfy.ui1_port,
                        t_msg.u.t_rm_tvd_nfy.e_nfy_cond,
                        t_msg.u.t_rm_tvd_nfy.pv_tag,
                        t_msg.u.t_rm_tvd_nfy.ui4_data );
                }
                break;

                case DSM_MSG_TVD_DRV_NFY:
                {
                    tv_decoder_drv_nfy_msg_hdlr(
                        t_msg.u.t_tvd_nfy.pv_nfy_tag,
                        t_msg.u.t_tvd_nfy.e_nfy_cond,
                        t_msg.u.t_tvd_nfy.ui4_data_1,
                        t_msg.u.t_tvd_nfy.ui4_data_2 );
                }
                break;

                case DSM_MSG_TV_DECODER_PLAYED:
                {
                    DBG_INFO(("{DSM} TV decoder started playing, "
                              "send notification to client that signal monitoring started.\n"));
                    /* notify the client monitoring has started. */
                    pt_dsm_comp=dsm_get_ref_to_comp(t_msg.h_dsm_comp) ;
                    if ( pt_dsm_comp != NULL )
                    {
                        dsm_avc_combi_proc_tv_decoder_nfy(
                            pt_dsm_comp,
                            DSM_COND_DEV_MONITOR_START);
                    }
                    
                }
                break;

                case DSM_MSG_TV_DECODER_STOPPED:
                {
                    DBG_INFO(("{DSM} TV decoder stopped playing, "
                              "send notification to client that video signal cleared.\n"));
                    /* notify the client monitoring has started. */
                    pt_dsm_comp=dsm_get_ref_to_comp(t_msg.h_dsm_comp) ;
                    if ( pt_dsm_comp != NULL )
                    {
                        ((DSM_AVC_COMBI_T*)(pt_dsm_comp->pv_private))->ui4_video_signal = 0;
                        dsm_avc_combi_proc_tv_decoder_nfy(
                            pt_dsm_comp,
                            DSM_COND_DEV_VIDEO_SIG_STATUS);
                    }
                    
                }
                break;
                                
                case DSM_MSG_TV_DECODER_MASTER:
                {
                    pt_dsm_comp=dsm_get_ref_to_comp(t_msg.h_dsm_comp) ;
                    if ( pt_dsm_comp != NULL )
                    {
                        INT32   i4_ret;
                        
                        i4_ret = dsm_avc_combi_enable_tv_decoder(pt_dsm_comp);
                        if ( i4_ret == DSMR_OK )
                        {
                            DBG_INFO(("{DSM} regain TV decoder, re-connect and play the TV decoder.\n"));
                        }
                        else if ( i4_ret == DSMR_ASYNC_NFY )
                        {
                            DBG_INFO(("{DSM} regain TV decoder, re-connect and play the TV decoder async.\n"));
                        }
                        else
                        {
                            DBG_ERROR(("{DSM} can not re-connect and enable play TV decoder.\n"));
                        }
                    }
                }
                break;

                case DSM_MSG_TV_DECODER_SLAVE:
                {
                    pt_dsm_comp=dsm_get_ref_to_comp(t_msg.h_dsm_comp) ;
                    if ( pt_dsm_comp != NULL )
                    {
                        DBG_INFO(("{DSM} loss control of TV decoder, "
                              "check if signal monitoring can continue.\n"));
                        if ( dsm_avc_combi_monitor_with_tv_decoder(
                                 pt_dsm_comp) != DSMR_OK )
                        {
                            DBG_INFO(("{DSM} AVC combi lost monitor of signal, "
                                      "send notification to client.\n"));
                            /* notify the client monitoring has stopped. */
                            dsm_avc_combi_proc_tv_decoder_nfy(
                                pt_dsm_comp,
                                DSM_COND_DEV_MONITOR_STOP);
                        }
                        else
                        {
                            DBG_INFO(("{DSM} AVC combi [%d] can continue to "
                                  "monitor signal.\n",
                                  pt_dsm_comp->h_rm_hdle));
                        }
                    }
                }
                break;

                case DSM_MSG_CODE_EXIT:
                {
                    DBG_INFO(("{DSM} process msg: exit\n"));
                    b_running = FALSE;
                }
                break;
                
                default:
                {
                    DBG_INFO(("{DSM} process msg: unknow msg\n"));  
                }
            }
        }
        else
        {
            b_running = FALSE;
        }
    }

    /*
      Existing DSM main thread.
      Free resource.
    */
    if ( x_msg_q_delete(h_queue) == OSR_OK )
    {
        DBG_INFO(("{DSM} DSM message queue deleted.\n"));

        h_queue = NULL_HANDLE;
        h_dsm_main_thread = NULL_HANDLE;
    }
    else
    {
        DBG_INFO(("{DSM} Error: can not delete DSM message queue.\n"));
    }

    if ( h_global_sema != NULL_HANDLE )
    {
        if ( x_sema_delete(h_global_sema) == OSR_OK )
        {
            DBG_INFO(("{DSM} Semaphore deleted.\n"));
            h_global_sema = NULL_HANDLE;

            /*
              Free RM resource handles and client handle.
            */
            dsm_tuner_close();
            
            dsm_avc_close();
            
        }
        else
        {
            DBG_ERROR(("{DSM} Error: can not delete semaphore.\n"));
        }
    }   

    DBG_INFO(("{DSM} main thread terminated.\n"));
    return;
}
