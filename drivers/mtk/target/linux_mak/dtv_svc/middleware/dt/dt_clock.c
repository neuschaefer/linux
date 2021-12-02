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
 * $RCSfile: dt_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Iolo Tsai $
 * $MD5HEX: eeeee16d218b478d12d6bd39898cde8d $
 *
 * Description:
 *         This file contain implementation of Date Time API's,
 *         which are exported to other Middleware components.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "dt/x_dt.h"
#include "os/inc/x_os.h"
#include "os/inc/os.h"
#include "dt/_dt.h"
#include "dt/dt_dbg.h"

#include "dt/dt_clock.h"

static DT_CLOCK_T   t_clock[MAX_NB_CUSTOM_CLOCKS_SUPPORT];

static INT32        i4_nb_clock_active = 0;

static HANDLE_T     _h_dt_lock_ = NULL_HANDLE;

static INT32        i4_init_flag = 0;

/*
  Function table to load and unload broadcast table processing.
*/
static const DT_CLOCK_BRDCST_API_T*         pt_clock_dvb_brdcst_api  = NULL;
static const DT_CLOCK_BRDCST_API_T*         pt_clock_atsc_brdcst_api = NULL;

/*-----------------------------------------------------------------------------
 * Name: dt_clock_handle_free_fct
 *
 * Description: This API is called whenever a DT clock handle
 *              is freed.
 *
 * Inputs:  h_obj         Contains the handle to core db client.
 *          e_type        Contains the core db client handle type and must be
 *                        set to CDB_CLIENT_HANDLE_T.
 *          pv_obj        References the client object structure.
 *          pv_tag        Ignored.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static BOOL dt_clock_handle_free_fct
(
    HANDLE_T       h_obj,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    BOOL           b_req_handle
)
{
    DT_CLOCK_CTRL_T*   pt_co ;

    /* If no client database object  has been pass into this
       simply return FALSE . */
    if ( ( pt_co = ( ( DT_CLOCK_CTRL_T*) pv_obj ) ) != NULL )
    {
        /* Abort if this is not the correct type. */
        if (e_type == DTT_CLOCK_T )
        {
            DBG_INFO(("{DT} freeing clock handle [%d] resource.\n", h_obj));
            /* protect the call to handle_delink. */
            /* Lock the database. */
            dt_grab_lock();

            if ( handle_delink(&(pt_co->pt_clock->t_link),h_obj) != HR_OK )
            {
                ABORT(DBG_CAT_HANDLE,DBG_ABRT_INV_STATE);
            }

            /*
              decrement the ref count. Remove the resource (control structure)
            */
            pt_co->pt_clock->i4_ref_count-- ;

            if ( pt_co->pt_clock->i4_ref_count == 0 )
            {
                DT_NFY_MSG_T      t_nfy_msg;

                DBG_INFO(("{DT} freeing clock [%s] resource.\n", pt_co->pt_clock->s_name));

                dt_clock_free_sync_src_resource(pt_co);

                /* send message to terminate the notification thread. */
                t_nfy_msg.t_msg_type = DT_CLOCK_EXIT_NOTIFICATION ;
                dt_clock_send_nfy_msg(pt_co, &t_nfy_msg);
                
                /* a clock has been deleted. decrement the active count. */
                i4_nb_clock_active-- ;
                
            }

            /*
              Remove the resource (control structure) for this handle. 
            */
            x_mem_free(pt_co);

            /* Unlock the database */
            dt_release_lock();
            
            return TRUE;
        }
        else
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: dt_clock_handle_parse_cb
 *
 * Description: This API is called by the handle_parse function to invoke each
 *    clock handle's notification function
 *
 * Inputs:  ui2_count      Contains the current count, starting at '0'.
 *          ui2_max_count  Contains the maximum count value.
 *          h_elem         Contains the handle referencing an client object.
 *          e_type         Contains the type.
 *          pv_obj         Contains the object pointer.
 *          pv_tag         Contains the tag value.
 *          pv_parse_data  Reason for calling notification function.
 * Outputs:
 *
 * Returns: HP_NEXT or HP_BREAK.
 ----------------------------------------------------------------------------*/
HP_TYPE_T dt_clock_handle_parse_cb
(
    UINT16         ui2_count,
    UINT16         ui2_max_count,
    HANDLE_T       h_elem,
    HANDLE_TYPE_T  e_type,
    VOID*          pv_obj,
    VOID*          pv_tag,
    VOID*          pv_parse_data
)
{
    HP_TYPE_T                  e_return;
    DT_CLOCK_CTRL_T*           pt_co;
    DT_CLOCK_HL_PARSE_DATA_T*  pt_parse_data;

    e_return = HP_NEXT;

    if (
        (e_type == DTT_CLOCK_T)                       &&
        ((pt_co = ((DT_CLOCK_CTRL_T*) pv_obj)) != NULL)  &&
        (pt_co->pf_nfy != NULL)
        )
    {
        pt_parse_data = (DT_CLOCK_HL_PARSE_DATA_T*) pv_parse_data;

        /* invoke the clock handle notification function. */
        pt_co->pf_nfy(h_elem,
                      pt_co->pv_tag,
                      pt_parse_data->e_cond,
                      pt_parse_data->t_delta);
    }
    return (e_return);
}


/*-----------------------------------------------------------------------------
 * Name: dt_clock_invoke_client_notify_fct
 *
 * Description: API that uses handle-link to invoke each handle's 
 *    notification function.
 *
 * Inputs:  -
 *   pt_clock:   structure to the clock.
 *   e_cond:     condition of clock.
 *   t_delta:    change  in  the value for the clock.
 *   
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
INT32 dt_clock_invoke_client_notify_fct
(
    DT_CLOCK_T*        pt_clock,
    DT_COND_T          e_cond,
    TIME_T             t_delta
)
{
    DT_CLOCK_HL_PARSE_DATA_T  t_parse_data;
    
    t_parse_data.e_cond  = e_cond;
    t_parse_data.t_delta = t_delta;

    /* protect the call to handle_parse function. */
    dt_clock_grab_sema(pt_clock->h_sema);
    
    /*
       Database object has been modified, notify the client so that it
       can take appropriate action.
    */
    handle_parse(&(pt_clock->t_link),dt_clock_handle_parse_cb,&t_parse_data);
    
    dt_clock_ungrab_sema(pt_clock->h_sema);

    return DTR_OK;
}

/*--------------------------------------------------------------------------
 * Name: dt_clock_free_sync_src_resource
 *
 * Description -    This API frees the clock's resource such 
 *     Table root handle.
 *
 * Input 
 *
 *   pt_clock        Pointer to a clock structure.
 *
 *
 * Output -- None.
 *
 * Return
 *
 *   DTR_WILL_NOTIFY:   
 *                  
 * 
 --------------------------------------------------------------------------*/
INT32 dt_clock_free_sync_src_resource
(
    DT_CLOCK_CTRL_T* pt_co
)
{
    INT32          i4_rc=DTR_OK;

    switch ( pt_co->pt_clock->e_src_type )
    {
        case DT_DVB_API:
        {
            if ( pt_clock_dvb_brdcst_api != NULL )
            {
                i4_rc=pt_clock_dvb_brdcst_api->pf_clock_unload_brdcst_time_info(pt_co);
                pt_co->pt_clock->e_src_type = DT_SRC_TYPE_UNKNOWN ;
            }        
        }
        break;

        case DT_ATSC_API:
        {
            if ( pt_clock_atsc_brdcst_api != NULL )
            {
                i4_rc=pt_clock_atsc_brdcst_api->pf_clock_unload_brdcst_time_info(pt_co);
                pt_co->pt_clock->e_src_type = DT_SRC_TYPE_UNKNOWN ;
                        
            }
        }
        break;

        default:
        {
        }
        break;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------
 * Name:  dt_create_lock
 *
 * Description: Internal API to create an binary semaphore lock. this
 *    binary semaphre lock (acts like a global lock) is used
 *    for controlling access to the shared variables, for
 *    example: clock name array
 *
 * Inputs:  - none
 *
 * Outputs: - none.
 *
 * Returns: none
 ----------------------------------------------------------------------------*/
VOID dt_create_lock(VOID)
{
    /* Create the clock semaphores. Simply */
    /* abort if semahore cannot be created.            */
    if (x_sema_create( &(_h_dt_lock_), X_SEMA_TYPE_MUTEX,
                       X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  dt_grab_lock
 *
 * Description: Internal API to grab lock
 *
 * Inputs:  - none
 *
 * Outputs: - none.
 *
 * Returns: none
 *
 ----------------------------------------------------------------------------*/
VOID dt_grab_lock(VOID)
{
    /* lock database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_lock( _h_dt_lock_, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  dt_release_lock
 *
 * Description: Internal API to unlock.
 *
 * Inputs:  none
 *
 * Outputs: none.
 *
 * Returns: none
 *
 ----------------------------------------------------------------------------*/
VOID dt_release_lock(VOID)
{
    /* un-lock the database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_unlock( _h_dt_lock_ ) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}


/*-----------------------------------------------------------------------------
 * Name:  dt_clock_grab_sema
 *
 * Description: API to grep a clock's semaphore.
 *
 * Inputs:  h_dt_sema:   handle to clock's semaphore.
 *
 * Outputs: none.
 *
 * Returns: none
 *
 ----------------------------------------------------------------------------*/
VOID dt_clock_grab_sema(HANDLE_T   h_dt_sema)
{
    if ( x_sema_lock( h_dt_sema, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  dt_clock_ungrab_sema
 *
 * Description: API to release a clock's semaphore.
 *
 * Inputs:  h_dt_sema:   handle to clock's semaphore.
 *
 * Outputs: none.
 *
 * Returns: none
 *
 ----------------------------------------------------------------------------*/
VOID dt_clock_ungrab_sema(HANDLE_T   h_dt_sema)
{
    if ( x_sema_unlock( h_dt_sema) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
    return;
}
/*----------------------------------------------------------------------
 * Name: x_dt_clock_init
 *
 * Description:  Initialize software clock component            
 *
 * Input:
 *    t_tz_offset:   timezone offset in number of seconds.
 *
 * Output:
 *    None
 *
 * Returns:  VOID.
------------------------------------------------------------------------*/
VOID dt_clock_init(VOID)
{
    INT32  i4_i;
    
    if ( i4_init_flag == 0  )
    {
        dt_create_lock();
        for ( i4_i = 0 ; i4_i < MAX_NB_CUSTOM_CLOCKS_SUPPORT ; i4_i++ )
        {
            x_memset(&(t_clock[i4_i]), 0, sizeof(DT_CLOCK_T));
        }
        i4_init_flag = 1 ;
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: x_dt_clock_create
 *
 * Description:  Initialize software clock component            
 *
 * Input:
 *    s_clock_name  Name of the clock to create.
 *    pf_nfy        Notification function.
 *    pv_tag        private tag pass to notification function.
 *
 * Output:
 *    ph_clock   Handle to the specified clock.
 *
 * Returns:  VOID.
------------------------------------------------------------------------*/
INT32 x_dt_clock_create
(
    CHAR*              s_clock_name,
    x_dt_nfy_fct       pf_nfy,
    VOID*              pv_tag,
    HANDLE_T*          ph_clock
)
{
    INT32              i4_i ;
    INT32              i4_rc = DTR_FAIL;
    DT_CLOCK_CTRL_T*   pt_clock_ctrl ;
    UINT16             ui2_clock_id;
    
    
    if ( ph_clock == NULL )
    {
        return DTR_INV_ARG;
    }

    /* check the resource limit. */
    if ( i4_init_flag == 1                           &&
         i4_nb_clock_active < MAX_NB_CUSTOM_CLOCKS_SUPPORT     )
    {
        dt_grab_lock();

        /* check for duplicate name. */
        for ( i4_i = 0 ; i4_i < MAX_NB_CUSTOM_CLOCKS_SUPPORT ; i4_i++)
        {
            if ( x_strcmp(t_clock[i4_i].s_name, s_clock_name) == 0 )
            {
                DBG_ERROR(("{DT} create clock error: name [%s] is already used.\n", s_clock_name));
                dt_release_lock();
                return DTR_FAIL;
            }
        }
                        
        /* create clock-handle ctrl struct. */
        pt_clock_ctrl = (DT_CLOCK_CTRL_T*)x_mem_alloc(sizeof(DT_CLOCK_CTRL_T));
        if (  pt_clock_ctrl != NULL )
        {
            /* allocate handle. */
            if ( handle_alloc(DTT_CLOCK_T,
                              (VOID*)pt_clock_ctrl,
                              pv_tag,
                              dt_clock_handle_free_fct,
                              ph_clock)
                 == HR_OK )
            {
                /* find a free DT_CLOCK_T structure */
                for ( i4_i = 0 ; i4_i < MAX_NB_CUSTOM_CLOCKS_SUPPORT ; i4_i++)
                {
                    if ( x_strlen(t_clock[i4_i].s_name) == 0 )
                    {
                        /*
                          Reserve a os system clock id
                        */
                        if ( os_reserve_sys_td(&ui2_clock_id) == OSR_OK )
                        {
                            TIME_T  t_utc_sec;
                            
                            /* Initialize the custom clock
                               with time from RTC if it is available.
                            */
                            if ( dt_rtc_get_handle() == DTR_OK )
                            {
                                /* Get the RTC time. */
                                if ( dt_rtc_get_time(&t_utc_sec) == DTR_OK )
                                {
                                    /*
                                      Set the user time clock.
                                    */
                                    os_set_sys_td(t_utc_sec, 0,
                                                  ui2_clock_id);
                                    t_clock[i4_i].t_run_cond = DT_IS_RUNNING ;
                                    t_clock[i4_i].t_sync_cond = DT_FREE_RUNNING;
                                    
                                }
                            }
                            else
                            {
                                t_clock[i4_i].t_run_cond = DT_NOT_RUNNING ;
                                t_clock[i4_i].t_sync_cond = DT_NO_SYNC_SRC;
                            }
                            
                            t_clock[i4_i].ui2_clock_id = ui2_clock_id ;

                            x_strncpy(t_clock[i4_i].s_name,
                                      s_clock_name,
                                      MAX_CLOCK_NAME_LEN);

                            /* Initialize the handle link structure. */
                            handle_link_init( &(t_clock[i4_i].t_link) );

                            /*
                               Initialize the semaphore for protecting the change
                               of synchronization source and its internal state.
                            */
                            if (  x_sema_create(&(t_clock[i4_i].h_sema),
                                                X_SEMA_TYPE_MUTEX,
                                                X_SEMA_STATE_UNLOCK) != OSR_OK
                                )
                            {
                                ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
                            }
                            
                            /*
                              Link the client handle to the database object.
                            */
                            if ( handle_link(&(t_clock[i4_i].t_link), *(ph_clock))
                                 == HR_OK )
                            {
                                /* Save Clock structure in the clock 
                                   handle control structure.
                                */
                                pt_clock_ctrl->pt_clock = &(t_clock[i4_i]);
                                pt_clock_ctrl->pf_nfy   = pf_nfy;
                                pt_clock_ctrl->pv_tag   = pv_tag;
                                pt_clock_ctrl->h_clock  = *ph_clock ;

                                /* create a notification thread. */
                                dt_clock_init_nfy(pt_clock_ctrl);

                                pt_clock_ctrl->pt_clock->i4_ref_count++;

                                /* a new clock is now activated. */
                                i4_nb_clock_active++;
                                
                                i4_rc = DTR_OK;
                            }
                            else
                            {
                                i4_rc=DTR_FAIL;
                            }
                            
                        }
                        break;
                    }
                }

                /* clean up if fail. */
                if ( i4_rc != DTR_OK )
                {
                    x_mem_free(pt_clock_ctrl);
                    handle_free(*ph_clock, TRUE);
                }
            }
            else
            {
                x_mem_free(pt_clock_ctrl);
                i4_rc = DTR_OUT_OF_HANDLE;
            }
        }
        else
        {
            i4_rc = DTR_OUT_OF_MEM;
        }
        
        dt_release_lock();
    }
    return i4_rc ;
}

/*----------------------------------------------------------------------
 * Name: x_dt_clock_get_handle
 *
 * Description:  Get the handle to a software clock component.           
 *
 * Input:
 *    s_clock_name   the name of clock to obtain a handle to.
 *    pf_nfy         Notification function.
 *    pv_tag         private tag pass to notification function.
 * Output:
 *    ph_clock       Handle to the specified clock.
 *
 * Returns:  VOID.
------------------------------------------------------------------------*/
INT32 x_dt_clock_get_handle
(
    CHAR*              s_clock_name,
    x_dt_nfy_fct       pf_nfy,
    VOID*              pv_tag,
    HANDLE_T*          ph_clock
)
{
    INT32              i4_i ;
    INT32              i4_rc = DTR_FAIL;
    DT_CLOCK_CTRL_T*   pt_clock_ctrl ;
    
    if ( ph_clock == NULL )
    {
        return DTR_INV_ARG;
    }

    /* check the resource limit. */
    if ( i4_init_flag == 1                           &&
         i4_nb_clock_active < MAX_NB_CUSTOM_CLOCKS_SUPPORT     )
    {
        dt_grab_lock();
        
        /* create clock-handle ctrl struct. */
        pt_clock_ctrl = (DT_CLOCK_CTRL_T*)x_mem_alloc(sizeof(DT_CLOCK_CTRL_T));
        if (  pt_clock_ctrl != NULL )
        {
            /* allocate handle. */
            if ( handle_alloc(DTT_CLOCK_T,
                              (VOID*)pt_clock_ctrl,
                              pv_tag,
                              dt_clock_handle_free_fct,
                              ph_clock)
                 == HR_OK )
            {
                /* find the DT_CLOCK_T structure */
                for ( i4_i = 0 ; i4_i < MAX_NB_CUSTOM_CLOCKS_SUPPORT ; i4_i++)
                {
                    if ( x_strcmp(t_clock[i4_i].s_name, s_clock_name) == 0 )
                    {
                        /*
                           Link the client handle to the database object.
                        */
                        if ( handle_link(&(t_clock[i4_i].t_link), *(ph_clock))
                             == HR_OK )
                        {
                            /* Save Clock structure in the clock 
                               handle control structure.
                            */
                            pt_clock_ctrl->pt_clock = &(t_clock[i4_i]);
                            pt_clock_ctrl->pf_nfy   = pf_nfy;
                            pt_clock_ctrl->pv_tag   = pv_tag;
                            pt_clock_ctrl->h_clock  = *ph_clock ;
                            pt_clock_ctrl->pt_clock->i4_ref_count++;
                            i4_rc = DTR_OK;
                        }
                        else
                        {
                            i4_rc=DTR_FAIL;
                        }
                        break;
                    }
                }
                /* clean up if fail. */
                if ( i4_rc != DTR_OK )
                {
                    x_mem_free(pt_clock_ctrl);
                    handle_free(*ph_clock, TRUE);
                }
            }
            else
            {
                x_mem_free(pt_clock_ctrl);
                i4_rc = DTR_OUT_OF_HANDLE;
            }
        }
        else
        {
            i4_rc = DTR_OUT_OF_MEM;
        }
        
        dt_release_lock();
    }
    return i4_rc ;
}

/*--------------------------------------------------------------------------
 * Name: _dt_clock_get_cond_unprotected
 *
 * Description -    This API get the clock running condition.
 *    If the clock is not running, then DT_NOT_RUNNING condition is
 *    returned.  If the DT is running, then sync source condition is
 *    returned.
 *                  
 *    This static function must be called inside critical section
 *    for protection.
 *
 * Input:
 *    pt_co:     ref to clock handle control structure.
 *
 * Output:
 *    None        
 *
 * Return
 *   clock condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *   DT_NOT_RUNNING, DT_NO_SYNC_SRC.          
 * 
 --------------------------------------------------------------------------*/
static DT_COND_T  _dt_clock_get_cond_unprotected
(
    DT_CLOCK_CTRL_T*     pt_co
)
{
    DT_COND_T            t_cx;
    
    if ( pt_co->pt_clock->t_run_cond == DT_NOT_RUNNING )
    {
        t_cx = DT_NOT_RUNNING;
    }
    else
    {
        t_cx = pt_co->pt_clock->t_sync_cond;
    }
    return  t_cx;
}

/*--------------------------------------------------------------------------
 * Name: dt_clock_get_utc_and_cond
 *
 * Description -    This API get the clock running condition and
 *     the UTC seconds.  This is an atomic operation
 *
 * Input:
 *   pt_co:          ref to a clock handle control structure.
 * Output:
 *
 *   pui2_milli_sec  pointer to variable holding the current millisecond
 *                   value.
 *
 *   pt_cond         pointer to variable containing the system clock
 *                   condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *                   DT_NOT_RUNNING 
 *
 * Return
 *   UTC sec         the current UTC seconds.
 --------------------------------------------------------------------------*/
TIME_T dt_clock_get_utc_and_cond
(
    DT_CLOCK_CTRL_T*   pt_co,
    UINT16*            pui2_milli_sec, 
    DT_COND_T*         pt_cond
)
{
    CRIT_STATE_T       t_state;
    TIME_T             t_utc;
    DT_COND_T          t_cx;

    /* Getting the UTC second and system clock condition must be
       atomic
    */
    t_state=x_crit_start();
    
    t_cx  = _dt_clock_get_cond_unprotected(pt_co);
    t_utc =  os_get_sys_td(pui2_milli_sec, pt_co->pt_clock->ui2_clock_id);
    
    x_crit_end(t_state);

    if ( pt_cond != NULL )
    {
        *pt_cond  = t_cx ;
    }
    return t_utc;
}


/*-----------------------------------------------------------------------------
 * Name: _dt_clock_set_cond_unprotected
 *
 * Description: This API set the clock and sync source
 *     condition.  Note this function needs to called inside
 *     critical condition for proctection.
 *
 * Inputs:
 *   pt_co      pointer to clock handle control structure.
 *   t_cond     Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING, DT_NOT_RUNNING,
 *              DT_IS_RUNNING, DT_SYNC_DISCONT, DT_NO_SYNC_SRC.
 *
 * Outputs: None
 *
 * Returns:
 *
 *   DTR_WILL_NOTIFY:   If the DT is running, and the sync source
 *                      condition's value has changed, then Datetime
 *                      lib will need to notify the clients.
 *
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 ---------------------------------------------------------------------------*/
static INT32 _dt_clock_set_cond_unproctected
(
    DT_CLOCK_CTRL_T*   pt_co,
    DT_COND_T          t_cond
)
{
    INT32              i4_rc;
    
    i4_rc = DTR_OK;
    if ( t_cond == DT_NOT_RUNNING || t_cond == DT_IS_RUNNING )
    {
        pt_co->pt_clock->t_run_cond = t_cond ;
    }
    else
    {
        if ( pt_co->pt_clock->t_sync_cond != t_cond )
        {
            /* If the 't_cond' is DT_SYNC_DISCONT, then we
               will convert the condition to DT_SYNC_RUNNING,
               but will return DTR_WILL_NOTIFY so the client
               is notified.
               
               This way if we detect multiple discontinuity
               consectively, we will continue to notify client with
               DT_SYNC_DISCONT.

               Note:  DT_SYNC_DISCONT is only used for notify the
               client, but will not be saved as an internal
               condition.
            */
            if ( t_cond == DT_SYNC_DISCONT )
            {
                t_cond = DT_SYNC_RUNNING;
            }
            pt_co->pt_clock->t_sync_cond = t_cond;
            i4_rc = DTR_WILL_NOTIFY;

            /*
               If this clock is being sync but it is in the
               'not running state' then we also need to change
               its state to 'running'.

               This condition can happen if RTC fail to run
               during at boot up, thus the first time the
               software clocks are set is when the broadcast
                tables are received.
            */
            if ( (t_cond == DT_SYNC_RUNNING) &&
                 (pt_co->pt_clock->t_run_cond == DT_NOT_RUNNING) )
            {
                pt_co->pt_clock->t_run_cond = DT_IS_RUNNING;
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: dt_clock_set_cond
 *
 * Description: This API set the clock and sync source condition
 *
 * Inputs:
 *   pt_co      pointer to clock handle control structure.
 *
 *   t_cond     Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING, DT_NOT_RUNNING,
 *              DT_IS_RUNNING, DT_SYNC_DISCONT, DT_NO_SYNC_SRC.
 *
 * Outputs: None
 *
 * Returns:
 *
 *   DTR_WILL_NOTIFY:   If the DT is running, and the sync source condition's value
 *                      has changed, then Datetime lib will need to
 *                      notify the clients.
 *
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 ---------------------------------------------------------------------------*/
INT32 dt_clock_set_cond
(
    DT_CLOCK_CTRL_T*   pt_co,
    DT_COND_T          t_cond
)
{
    CRIT_STATE_T       t_state;
    INT32              i4_rc;
    
    t_state=x_crit_start();

    i4_rc = _dt_clock_set_cond_unproctected (pt_co, t_cond);
    
    x_crit_end(t_state);
    
    return i4_rc;
}


/*--------------------------------------------------------------------------
 * Name: dt_clock_set_utc_and_cond
 *
 * Description -    This API set the clock condition and the UTC seconds
 *                  This is an atomic operation
 *
 * Input 
 *
 *   t_sec          The UTC seconds to set the sysetm clock.
 *
 *   ui2_milli_sec  Millisecond to set the system clock.
 *
 *   t_cond         Condition: DT_FREE_RUNNING, DT_SYNC_RUNNING,
 *                  DT_NOT_RUNNING 
 *
 * Output -- None.
 *
 * Return
 *
 *   DTR_WILL_NOTIFY:   The condition is set and the value condition has
 *                      has changed, therefore Datetime lib will need to
 *                      notify the clients. 
 *   DTR_OK:            The condition is set, but the value has not changed
 *                      from the previous condition, therefore notification
 *                      to clients is not needed. 
 *                  
 * 
 --------------------------------------------------------------------------*/
INT32 dt_clock_set_utc_and_cond
(
    DT_CLOCK_CTRL_T*   pt_co,
    TIME_T             t_sec,
    UINT16             ui2_milli_sec,
    DT_COND_T          t_cond
)
{
    CRIT_STATE_T   t_state;
    INT32          i4_rc;

    i4_rc = DTR_OK;
    
    /* Setting the UTC second and system clock condition must be
       atomic
    */
    t_state=x_crit_start();

    i4_rc = _dt_clock_set_cond_unproctected (pt_co, t_cond);
    
    os_set_sys_td(t_sec, ui2_milli_sec, pt_co->pt_clock->ui2_clock_id);
    
    x_crit_end(t_state);

#if 0    
    /* Setting of Real-Time clock only when this operation is
       setting the user clock condition and time.
       
       Not this operation is done outside the critical section.
    */
    if ( ui4_clock == ((INT32)DT_CLOCK_USER_UTC) )
    {
        dt_rtc_set_time(t_sec);
    }
#endif
    
    return i4_rc;
}

/*-----------------------------------------------------------------

* Name: x_dt_clock_get_utc
 *
 * Description: Get the current broadcast clock time in UTC time frame,
 *              the result is  returned as the number of seconds from
 *              a default epoch of 00:00:00Z, Jan 1, 1970.
 *
 *              If the broadcast time has not bee set, then NULL_TIME is
 *              returned.
 *
 * Inputs:
 *    h_clock           Handle to the clock.
 *
 * Outputs:
 *    pui2_milli_sec	Optional parameter to store the number of
 *                      milliseconds (0 to 999). If the parameter is NULL,
 *                      milliseconds is ignored.
 *
 *    pt_dt_cond	    The condition of the system clock: DT_NOT_RUNNING,
 *                      DT_FREE_RUNNING, DT_SYNC_RUNNING.  If NULL, then
 *                      condition code is ignored.
 *
 * Returns:
 *    The number of seconds since default epoch for the current
 *    UTC time.
----------------------------------------------------------------*/
TIME_T x_dt_clock_get_utc
(
    HANDLE_T         h_clock,
    UINT16*          pui2_milli_sec,
    DT_COND_T*       pt_dt_cond
)
{
    CRIT_STATE_T     t_state;
    TIME_T           t_utc;
    HANDLE_TYPE_T    e_type;
    DT_CLOCK_CTRL_T* pt_co;

    t_utc = 0 ;
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_clock, &e_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (e_type == DTT_CLOCK_T)                            &&
         (pt_co->pt_clock != NULL )
        )     
    {
        x_crit_end(t_state);
        
        t_utc = dt_clock_get_utc_and_cond(pt_co,
                                          pui2_milli_sec,
                                          pt_dt_cond);
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
#endif
    }
    
    return t_utc ;
}


/*------------------------------------------------------------------------
 * Name: x_dt_clock_set_utc
 *
 * Description: Set the clock time in UTC time frame.
 *
 * Input:
 *    h_clock           Handle to the clock.
 *    t_sec:  The number of seconds since the default epoch, 00:00:00Z,
 *            Jan 1, 1970.
 *
 *    ui2_milli_sec  Optional parameter to set the number of milliseconds
 *                   (0 to 999).
 * Output:
 *    None
 *
 * Returns
 *    DTR_OK:  Success
 *    DTR_FAIL:  Error on setting time on the Real-Time-Clock.
 *    DTR_NO_RTC: No Real-Time-Clock on the system platform.
------------------------------------------------------------------------*/
INT32 x_dt_clock_set_utc
(
    HANDLE_T         h_clock,
    TIME_T           t_sec,
    UINT16           ui2_milli_sec
)
{
    CRIT_STATE_T     t_state;
    HANDLE_TYPE_T    e_type;
    DT_CLOCK_CTRL_T* pt_co;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_clock, &e_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (e_type == DTT_CLOCK_T)                            &&
         (pt_co->pt_clock != NULL )
        )     
    {
        x_crit_end(t_state);

        os_set_sys_td(t_sec, ui2_milli_sec, pt_co->pt_clock->ui2_clock_id);
        dt_clock_set_cond (pt_co, DT_IS_RUNNING);
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
#endif
    }
    /*
      Call the user time clock to set the current time.
    */
    return DTR_OK;
}


/*--------------------------------------------------------------------------
 * Name: x_dt_clock_set_sync_src
 *
 * Description: This API sets the synchronization source for the current
 *              time.
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
INT32 x_dt_clock_set_sync_src
(
    HANDLE_T                    h_clock,
    DT_SYNC_SRC_TYPE_T          t_sync_src_type,
    DT_SRC_TYPE_DESC_T          t_src_type_desc,
    const VOID*                 pv_src_info
)    
{
    CRIT_STATE_T     t_state;
    HANDLE_TYPE_T    e_type;
    DT_CLOCK_CTRL_T* pt_co;
    INT32            i4_rc = DTR_FAIL;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_clock, &e_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (e_type == DTT_CLOCK_T)                            &&
         (pt_co->pt_clock != NULL )
        )     
    {
        x_crit_end(t_state);
        /*
           Determine the source type to obtain the current system time,
           and call the corresponding table engine.
        */
        switch (t_sync_src_type)
        {
            case DT_SYNC_SRC_NONE:
            {
                dt_clock_free_sync_src_resource(pt_co);
            }
            break;
            
            case DT_SYNC_WITH_DVB_TDT:
            case DT_SYNC_WITH_DVB_TOT:
            case DT_SYNC_WITH_DVB_TDT_OR_TOT:
            case DT_SYNC_WITH_DVB_TOT_OR_TDT:
            {
                switch (t_src_type_desc)
                {
                    /*
                      For MPEG2 Broadcast source or connection handle,
                      set up the parameters to connect to the tbm manager
                      for DVB TDT/TOT table.
                    */
                    case DT_SRC_TYPE_MPEG_2_BRDCST:
                    case DT_SRC_TYPE_CONN_HANDLE:
                    {
                        if ( pv_src_info != NULL )
                        {
                            /* t_tbm_src_type = TM_SRC_TYPE_MPEG_2_BRDCST; */
                        
                            /*
                              Set up connection and callback function
                              with DVB engine.
                            */
                            if ( pt_clock_dvb_brdcst_api != NULL )
                            {
                                i4_rc = pt_clock_dvb_brdcst_api->pf_clock_load_brdcst_time_info(
                                    pt_co,
                                    t_sync_src_type,
                                    t_src_type_desc, /* t_tbm_src_type,*/
                                    pv_src_info);

                                pt_co->pt_clock->e_src_type = DT_DVB_API;
                            }
                        }
                    }
                    break;
        
                    default:
                    {
                        /*
                          Currently, we only support DVB TDT and TOT table
                          from MPEG2 broadcast.
                        */
                        i4_rc = DTR_FAIL;
                    }
                    break;
                }
            }
            break;
        
            /* For cable out-of-band STT data. */
            case DT_SYNC_WITH_SCTE_STT:
            {
                switch (t_src_type_desc)
                {
                    /*
                      For OOB Cable source, the format type can be
                      anything.
                    */
                    case DT_SRC_TYPE_FORMAT_UNKNOWN:
                    {
                        /*
                          For Cable OOB data source,
                          the 'pv_src_info' should be set to NULL.
                        */
                        if ( pv_src_info == NULL )
                        {
                            /*
                              For the table manager, we need to specify
                              MPEG_2, OOB source type.
                            */
                        
                            /* t_tbm_src_type = TM_SRC_TYPE_MPEG_2_OOB; */
                        
                            /*
                              Set up connection and callback function
                              with ATSC engine.
                            */
                            if ( pt_clock_atsc_brdcst_api != NULL )
                            {
                                i4_rc = pt_clock_atsc_brdcst_api->pf_clock_load_brdcst_time_info(
                                    pt_co,
                                    t_sync_src_type,
                                    t_src_type_desc, /* t_tbm_src_type,*/
                                    pv_src_info);

                                pt_co->pt_clock->e_src_type = DT_ATSC_API;
                            }
                        }
                        else
                        {
                            DBG_ERROR(("{DT} Error: tuner name parameter, pv_src_info, should be set to NULL for Cable OOB connection.\n"));
                        }
                    }
                    break;
        
                    default:
                    {
                        i4_rc = DTR_FAIL;
                    }
                    break;
                }
            }
            break;
            
            case DT_SYNC_WITH_ATSC_STT:
            {
                switch (t_src_type_desc)
                {
                    /*
                      For MPEG2 Broadcast source, set up the parameters
                      to connect to the tbm manager for ATSC STT table.
                    */
                    case DT_SRC_TYPE_MPEG_2_BRDCST:
                    case DT_SRC_TYPE_CONN_HANDLE:
                    {
                        if ( pv_src_info != NULL )
                        {
                            /*
                              Set up connection and callback function
                              with ATSC engine.
                            */
                            if ( pt_clock_atsc_brdcst_api != NULL )
                            {
                                i4_rc = pt_clock_atsc_brdcst_api->pf_clock_load_brdcst_time_info(
                                    pt_co,
                                    t_sync_src_type,
                                    t_src_type_desc,  /* t_tbm_src_type */
                                    pv_src_info);

                                pt_co->pt_clock->e_src_type = DT_ATSC_API;
                                
                            }
                        }
                    }
                    break;
        
                    default:
                    {
                        /*
                          Currently, we only support ATSC STT table from
                          MPEG2 broadcast.
                        */
                        i4_rc = DTR_FAIL;
                    }
                    break;
                }
            }
            break;
            
            default:
            {
                /* unknown synchronization source, no action needed. */
                i4_rc = DTR_OK;
            }
            break;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
#endif
    }

    return i4_rc;
}

/*****************************************************************
 * Name: dt_reg_brdcst_functions
 *
 * Description:  This function registers DVB function table.
 *
 * Inputs:
 *   pt_table     DVB function table processing API's.
 *
 * Outputs: -
 *   VOID
 *
 * Returns:
 *   DTR_OK      conversion is successful.
 *   DTR_FAIL
 *****************************************************************/
INT32 dt_clock_reg_brdcst_functions
(
    DT_TIME_SRC_TYPE_T             e_src_type,
    const DT_CLOCK_BRDCST_API_T*   pt_table
)
{
    INT32   i4_rc = DTR_OK;

    switch ( e_src_type )
    {
        case DT_DVB_API:
        {
            pt_clock_dvb_brdcst_api = pt_table;
        }
        break;

        case DT_ATSC_API:
        {
            pt_clock_atsc_brdcst_api = pt_table;
        }
        break;

        default:
        {
        }
        break;
    }
    
    return i4_rc;
}





