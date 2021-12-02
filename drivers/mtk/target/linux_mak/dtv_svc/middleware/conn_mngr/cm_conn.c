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
 * $RCSfile: cm_conn.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains the implementation of the connection-
 *         related APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "conn_mngr/cm_conn_api.h"
#include "conn_mngr/cm_handler_api.h"
#include "conn_mngr/_cm.h"
#include "dbg/dbg.h"


/*-----------------------------------------------------------------------------
 * Name: cm_connection_free_handle
 *
 * Description: This API is called when a connection handle is freed.
 *
 * Inputs:  h_conn        Contains the handle that should be freed.
 *          e_type        Contains the handle type.
 *          pv_obj        References the handle's object.
 *          pv_tag        References the handle's tag.
 *          b_req_handle  Is set to TRUE if the handle free was initiated with
 *                        this handle else FALSE.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle free is allowed, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL cm_connection_free_handle (HANDLE_T       h_conn,
                                       HANDLE_TYPE_T  e_type,
                                       VOID*          pv_obj,
                                       VOID*          pv_tag,
                                       BOOL           b_req_handle)
{
    if (! b_req_handle)
    {
        cm_handler_delink_conn(h_conn);
        
        x_mem_free(pv_obj);

        return TRUE;
    }
    else
    {
        /* free is not allowed */
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_is_pipe
 *
 * Description: This API checks whether a handle is a handle to a Connection
 *              Manager pipe.
 *
 * Inputs:  h_obj  Contains the handle to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is a handle to a Connection Manager pipe, FALSE
 *          otherwise.
 ----------------------------------------------------------------------------*/
static BOOL cm_is_pipe (HANDLE_T  h_obj)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_obj, & e_type);

    return (BOOL) ((i4_res == HR_OK) && (e_type == CMT_PIPE));
}

/*-----------------------------------------------------------------------------
 * Name: cm_conn_get_handler
 *
 * Description: This API returns the connection handler structure associated
 *              with a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: -
 *
 * Returns: a pointer to the connection handler structure, or NULL.
 ----------------------------------------------------------------------------*/
static CM_HANDLER_T* cm_conn_get_handler (HANDLE_T h_conn)
{
    CM_HANDLER_T*     pt_handler = NULL;
    CM_CONNECTION_T*  pt_conn;
    INT32             i4_res;

    i4_res = handle_get_obj(h_conn, (VOID**) & pt_conn);

    if (i4_res == HR_OK)
    {
        handle_get_obj(pt_conn->h_handler, (VOID**) & pt_handler);
    }

    return pt_handler;
}

/*-----------------------------------------------------------------------------
 * Name: cm_connect_op
 *
 * Description: This API is called by x_cm_connect and performs the actual
 *              connection.
 *
 * Inputs:  h_obj           Contains a handle to an existing connection or to a
 *                          pipe.
 *          pt_connect_cmd  References an array of commands.
 *          pv_nfy_tag      References the tag passed to the notify function.
 *          pf_nfy          References the notify function.
 *
 * Outputs: ph_conn  Contains a handle to the connection.
 *          pe_cond  Contains the connection's condition (CONNECTING, etc)
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_connect_op (HANDLE_T             h_obj,
                     const CM_COMMAND_T*  pt_connect_cmd,
                     VOID*                pv_nfy_tag,
                     x_cm_nfy_fct         pf_nfy,
                     HANDLE_T*            ph_conn,
                     CM_COND_T*           pe_cond)
{
    CM_PIPE_T*           pt_pipe=NULL;
    CM_CONNECTION_T*     pt_parent_conn;
    HANDLE_T             h_cm_pipe;
    INT32                i4_hr_res, i4_res;
    BOOL                 b_root_connection;

    i4_res = CMR_OK;

    b_root_connection = cm_is_pipe(h_obj);

    if (b_root_connection)
    {
        h_cm_pipe = h_obj;

        pt_parent_conn = NULL;
    }
    else
    {
        i4_hr_res = handle_get_obj(h_obj, (VOID**) & pt_parent_conn);

        if (i4_hr_res == HR_OK)
        {
            h_cm_pipe = pt_parent_conn->h_pipe;
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }

    if (i4_res == CMR_OK)
    {
        i4_hr_res = handle_get_obj(h_cm_pipe, (VOID**) & pt_pipe);

        if ( (i4_hr_res == HR_OK)&&(pt_pipe!=NULL) )
        {
            const CHAR*  ps_src_type;
            const CHAR*  ps_src_name;

            {
                const CM_COMMAND_T*  pt_src_type_cmd;

                if (b_root_connection)
                {
                    ps_src_type = pt_pipe->s_src_type;
                }
                else
                {
                    ps_src_type = pt_parent_conn->s_src_type;
                }

                /* find src_type in connection commands */
                pt_src_type_cmd = cm_find_cmd(pt_connect_cmd, CONN_SRC_TYPE);

                if (pt_src_type_cmd != NULL)
                {
                    if ((ps_src_type[0] == '\0')
                        ||
                        cm_handler_are_equal(ps_src_type,
                                             pt_src_type_cmd->u.ps_name))
                    {
                        ps_src_type = pt_src_type_cmd->u.ps_name;

                        pt_connect_cmd++;
                    }
                    else
                    {
                        i4_res = CMR_INV_COMMAND;
                    }
                }

                if ((ps_src_type == NULL) || (ps_src_type[0] == '\0'))
                {
                    i4_res = CMR_INV_COMMAND;
                }
            }

            if (i4_res == CMR_OK)
            {
                const CM_COMMAND_T*  pt_src_name_cmd;

                /* find src_name in connection commands */
                pt_src_name_cmd = cm_find_cmd(pt_connect_cmd, CONN_SRC_NAME);

                if (pt_src_name_cmd != NULL)
                {
                    ps_src_name = pt_src_name_cmd->u.ps_name;
                    
                    if ((ps_src_name == NULL) || (ps_src_name[0] == '\0'))
                    {
                        i4_res = CMR_INV_COMMAND;
                    }
                }
                else if (b_root_connection)
                {
                    ps_src_name = pt_pipe->s_src_name;
                }
                else
                {
                    ps_src_name = pt_parent_conn->s_src_name;
                }
            }

            if (i4_res == CMR_OK)
            {
                CM_HANDLER_T* pt_handler;

                pt_handler = cm_handler_find_by_name(ps_src_type);

                if (pt_handler != NULL)
                {
                    CM_CONNECTION_T*  pt_conn=NULL;
                    HANDLE_T          h_conn;
                    
                    i4_res = handle_alloc_and_obj(CMT_CONNECTION,
                                                  pv_nfy_tag,
                                                  sizeof(CM_CONNECTION_T),
                                                  cm_connection_free_handle,
                                                  & h_conn,
                                                  (VOID**) & pt_conn);
                    
                    if ((i4_res == HR_OK)&&(pt_conn!=NULL))
                    {
                        cm_connect_fct  pf_connect;
                        HANDLE_T        h_parent_ch_conn;
                        CM_COND_T       e_cond;

                        x_memset(pt_conn, 0, sizeof(CM_CONNECTION_T));                             

                        pt_conn->h_pipe = h_cm_pipe;

                        if (b_root_connection)
                        {
                            pt_pipe->h_cm_conn = h_conn;

                            h_parent_ch_conn = NULL_HANDLE;
                        }
                        else
                        {
                            h_parent_ch_conn = pt_parent_conn->h_ch_conn;
                        }

                        pt_conn->e_cond     = CM_COND_DISCONNECTED;
                        pt_conn->pf_nfy     = pf_nfy;
                        pt_conn->b_root     = b_root_connection;
                        
                        cm_handler_link_conn(pt_handler, h_conn);
                        
                        x_strncpy(pt_conn->s_src_type,
                                  ps_src_type,
                                  CONN_SRC_TYPE_MAX_STRLEN + 1);

                        pt_conn->s_src_type[CONN_SRC_TYPE_MAX_STRLEN] = '\0';

                        if (ps_src_name != NULL)
                        {
                            x_strncpy(pt_conn->s_src_name,
                                      ps_src_name,
                                      CONN_SRC_NAME_MAX_STRLEN + 1);
                        }

                        pt_conn->s_src_name[CONN_SRC_NAME_MAX_STRLEN] = '\0';

                        pf_connect = pt_handler->t_fct_tbl.pf_connect;

                        i4_res = pf_connect(h_conn,
                                            & pt_conn->h_ch_conn,
                                            h_parent_ch_conn,
                                            pt_connect_cmd,
                                            & e_cond);

                        if (i4_res == CMR_OK)
                        {
                            CRIT_STATE_T  t_state;
                            
                            *ph_conn = h_conn;
                            
                            t_state = x_crit_start();
                            
                            /*
                              we only update the condition if it has not been
                              changed already by a call to cm_nfy
                            */
                            if (pt_conn->e_cond == CM_COND_DISCONNECTED)
                            {
                                pt_conn->e_cond = e_cond;
                            }
                            
                            x_crit_end(t_state);
                            
                            if (pe_cond != NULL)
                            {
                                *pe_cond = e_cond;
                            }
                        }
                        else
                        {
                            handle_free(h_conn, FALSE);
                        }
                    }
                    else
                    {
                        i4_res = CMR_FAIL;
                    }
                }
                else
                {
                    i4_res = CMR_NO_HANDLER;
                }
            }
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_disconnect_op
 *
 * Description: This API is called by x_cm_disconnect and performs the actual
 *              disconnection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: pe_cond  Contains the connection's condition (DISCONNECTING, etc)
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_disconnect_op (HANDLE_T    h_conn,
                        CM_COND_T*  pe_cond)
{
    CM_CONNECTION_T*  pt_conn;
    INT32             i4_res;

    i4_res = handle_get_obj(h_conn, (VOID**) & pt_conn);

    if (i4_res == HR_OK)
    {
        CM_HANDLER_T*  pt_handler;
        
        i4_res = handle_get_obj(pt_conn->h_handler, (VOID**) & pt_handler);

        if (i4_res == HR_OK)
        {
            i4_res = pt_handler->t_fct_tbl.pf_disconnect(pt_conn->h_ch_conn,
                                                         pe_cond);
            
            if (i4_res == CMR_OK)
            {
                CRIT_STATE_T  t_state;
                
                t_state = x_crit_start();
                
                if (cm_is_valid_connection (h_conn))
                {
                    if (pt_conn->e_cond != CM_COND_DISCONNECTED)
                    {
                        pt_conn->e_cond = *pe_cond;
                    }
                }
                
                x_crit_end(t_state);
                
                if (*pe_cond == CM_COND_DISCONNECTED)
                {
                    if (cm_is_valid_connection (h_conn))
                    {
                        handle_free(h_conn, FALSE);
                    }
                }
            }
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }
    else
    {
        i4_res = CMR_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_nfy
 *
 * Description: This API is called by connection handlers to notify the
 *              Connection Manager of changes to a connection.
 *
 * Inputs:  h_conn    Contains a handle to a connection.
 *          e_cond    Contains the notification condition.
 *          ui4_data  Contains additional data (depends on e_cond).
 *
 * Outputs: -
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Invalid argument.
 *          CMR_FAIL     Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 cm_nfy (HANDLE_T   h_conn,
              CM_COND_T  e_cond,
              UINT32     ui4_data)
{
    CM_CONNECTION_T*  pt_conn;
    HANDLE_TYPE_T     e_type;
    CRIT_STATE_T      t_state;
    INT32             i4_res;

    cm_lock();
    t_state = x_crit_start();
    
    i4_res = handle_get_type_obj(h_conn, & e_type, (VOID**) & pt_conn);
    
    if ((i4_res == HR_OK) && (e_type == CMT_CONNECTION) && (pt_conn != NULL))
    {
        VOID*  pv_nfy_tag;
        
        i4_res = x_handle_get_tag(h_conn, & pv_nfy_tag);
        
        if (i4_res == HR_OK)
        {
            CM_PIPE_T*    pt_pipe;
            x_cm_nfy_fct  pf_nfy;
            HANDLE_T      h_pipe;
            BOOL          b_close_pipe;
            
            if ((e_cond == CM_COND_CLOSED) ||
                (e_cond == CM_COND_OPENING) ||
                (e_cond == CM_COND_OPENED) ||
                (e_cond == CM_COND_CLOSING) ||
                (e_cond == CM_COND_DISCONNECTED) ||
                (e_cond == CM_COND_CONNECTING) ||
                (e_cond == CM_COND_CONNECTED) ||
                (e_cond == CM_COND_DISCONNECTING) ||
                (e_cond == CM_COND_CONNECTED_TRANS))
            {
                pt_conn->e_cond = e_cond;
            }
            
            pf_nfy       = pt_conn->pf_nfy;
            h_pipe       = pt_conn->h_pipe;
            b_close_pipe = pt_conn->b_close_pipe;
            
            if ((e_cond == CM_COND_DISCONNECTED)
                &&
                (handle_get_type_obj(h_pipe,
                                     & e_type,
                                     (VOID**) & pt_pipe) == HR_OK)
                &&
                (e_type == CMT_PIPE))
            {
                pt_pipe->h_cm_conn = NULL_HANDLE;
            }
            
            x_crit_end(t_state);
            
            DBG_INFO((DBG_PREFIX"Nfy conn %d (%s, tag 0x%08x, data %d)\n",
                      h_conn,
                      cm_condition_str(e_cond),
                      pv_nfy_tag,
                      ui4_data));
            
            pf_nfy(h_conn, e_cond, pv_nfy_tag, ui4_data);
            
            if (e_cond == CM_COND_DISCONNECTED)
            {
                if (b_close_pipe)
                {
                    cm_pipe_rm_close(h_pipe, TRUE, & e_cond);
                }
                
                handle_free(h_conn, FALSE);
            }
            
            i4_res = CMR_OK;
        }
        else
        {
            x_crit_end(t_state);
            
            i4_res = CMR_FAIL;
        }
    }
    else
    {
        x_crit_end(t_state);
        
        i4_res = CMR_INV_HANDLE;
    }

    cm_unlock();

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_op
 *
 * Description: This API gets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to a connection.
 *          e_ctrl        Contains the control code.
 *
 * Outputs: pv_ctrl_data  Contains information about the connection.
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Invalid argument.
 *          CMR_FAIL     Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 cm_get_op (HANDLE_T        h_conn,
                 CM_CTRL_TYPE_T  e_ctrl,
                 VOID*           pv_ctrl_data)
{
    INT32 i4_res;

    if (CM_CTRL_IS_FOR_HANDLER(e_ctrl))
    {
        CM_HANDLER_T*     pt_handler;
        CM_CONNECTION_T*  pt_conn;

        pt_handler = cm_conn_get_handler(h_conn);

        if ((pt_handler != NULL) &&
            (pt_handler->t_fct_tbl.pf_get != NULL) &&
            (handle_get_obj(h_conn, (VOID**) & pt_conn) == HR_OK))
        {
            i4_res = pt_handler->t_fct_tbl.pf_get(pt_conn->h_ch_conn,
                                                  e_ctrl,
                                                  pv_ctrl_data);
        }
        else
        {
            i4_res = CMR_NO_HANDLER;
        }
    }
    else
    {
        CM_CONNECTION_T* pt_conn;

        if (handle_get_obj(h_conn, (VOID**) & pt_conn) == HR_OK)
        {
            i4_res = CMR_OK;

            switch (e_ctrl)
            {
                case CM_CTRL_COND:
                {
                    CRIT_STATE_T  t_state;

                    t_state = x_crit_start();

                    *((CM_COND_T*) pv_ctrl_data) = pt_conn->e_cond;

                    x_crit_end(t_state);
                }
                break;

                case CM_CTRL_RM_PIPE:
                {
                    CM_PIPE_T*  pt_pipe;

                    if (handle_get_obj(pt_conn->h_pipe,
                                       (VOID**) & pt_pipe)
                        == HR_OK)
                    {
                        CRIT_STATE_T  t_state;

                        t_state = x_crit_start();

                        ((CM_CTRL_RM_PIPE_T*) pv_ctrl_data)->h_rm_pipe =
                            pt_pipe->h_rm_pipe;

                        x_crit_end(t_state);
                    }
                    else
                    {
                        i4_res = CMR_FAIL;
                    }
                }
                break;

                default:
                    /* unrecognized */
                    i4_res = CMR_INV_ARG;
                    break;
            }
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_set_op
 *
 * Description: This API sets some attributes of a connection.
 *
 * Inputs:  h_conn        Contains a handle to a connection.
 *          e_ctrl        Contains the control code.
 *          pv_ctrl_data  References a data structure which content depends on
 *                        e_ctrl.
 *
 * Outputs: pv_ctrl_data  Contains values that may have been modified.
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Invalid argument.
 *          CMR_FAIL     Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 cm_set_op (HANDLE_T        h_conn,
                 CM_CTRL_TYPE_T  e_ctrl,
                 VOID*           pv_ctrl_data)
{
    INT32 i4_res;

    if (CM_CTRL_IS_FOR_HANDLER(e_ctrl))
    {
        CM_HANDLER_T*     pt_handler;
        CM_CONNECTION_T*  pt_conn;

        pt_handler = cm_conn_get_handler(h_conn);

        if ((pt_handler != NULL) &&
            (pt_handler->t_fct_tbl.pf_set != NULL) &&
            (handle_get_obj(h_conn, (VOID**) & pt_conn) == HR_OK))
        {
            i4_res = pt_handler->t_fct_tbl.pf_set(pt_conn->h_ch_conn,
                                                  e_ctrl,
                                                  pv_ctrl_data);
        }
        else
        {
            i4_res = CMR_NO_HANDLER;
        }
    }
    else
    {
        i4_res = CMR_OK;

        switch (e_ctrl)
        {
            default:
                /* unrecognized */
                i4_res = CMR_INV_ARG;
                break;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_cond_op
 *
 * Description: This API returns the condition of a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: pe_cond  Contains the connection condition.
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_get_cond_op (HANDLE_T    h_conn,
                      CM_COND_T*  pe_cond)
{
    CM_CONNECTION_T*  pt_conn;
    INT32             i4_res;

    i4_res = handle_get_obj(h_conn, (VOID **) & pt_conn);

    if (i4_res == HR_OK)
    {
        CRIT_STATE_T  t_state;

        t_state = x_crit_start();

        *pe_cond = pt_conn->e_cond;

        x_crit_end(t_state);

        i4_res = CMR_OK;
    }
    else
    {
        i4_res = CMR_FAIL;
    }

    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: cm_connection_get_pipe
 *
 * Description: This API returns the handle to the pipe the connection belongs
 *              to.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: -
 *
 * Returns: a pipe handle, or NULL_HANDLE.
 ----------------------------------------------------------------------------*/
HANDLE_T cm_connection_get_pipe (HANDLE_T  h_conn)
{
    CM_CONNECTION_T*  pt_conn;
    HANDLE_T          h_cm_pipe;
    INT32             i4_res;

    i4_res = handle_get_obj(h_conn, (VOID**) & pt_conn);

    if (i4_res == HR_OK)
    {
        h_cm_pipe = pt_conn->h_pipe;
    }
    else
    {
        h_cm_pipe = NULL_HANDLE;
    }

    return h_cm_pipe;
}

/*-----------------------------------------------------------------------------
 * Name: cm_connection_get_handler_conn
 *
 * Description: This API returns the handle to the handler's connection handle.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: -
 *
 * Returns: a handler connection handle, or NULL_HANDLE.
 ----------------------------------------------------------------------------*/
HANDLE_T cm_connection_get_handler_conn (HANDLE_T  h_conn)
{
    CM_CONNECTION_T*  pt_conn;
    HANDLE_T          h_ch_conn;
    INT32             i4_res;
    
    DBG_INFO((DBG_PREFIX"cm_connection_get_handler_conn(%d)\n", h_conn));
    
    i4_res = handle_get_obj(h_conn, (VOID**) & pt_conn);
    
    if (i4_res == HR_OK)
    {
        DBG_INFO((DBG_PREFIX"cm_connection_get_handler_conn - %p\n",
                  & pt_conn->h_ch_conn));
        
        h_ch_conn = pt_conn->h_ch_conn;
    }
    else
    {
        h_ch_conn = NULL_HANDLE;
    }
    
    return h_ch_conn;
}

/*-----------------------------------------------------------------------------
 * Name: cm_connection_set_handler_conn
 *
 * Description: This API sets the handle to the handler's connection handle.
 *
 * Inputs:  h_cm_conn  Contains a handle to a CM connection.
 *          h_ch_conn  Contains a handle to a handler connection.
 *
 * Outputs: -
 *
 * Returns: CMR_OK       The handle was set successfully.
 *          CMR_INV_ARG  The CM connection handle is invalid.
 ----------------------------------------------------------------------------*/
INT32 cm_connection_set_handler_conn (HANDLE_T  h_cm_conn,
                                      HANDLE_T  h_ch_conn)
{
    CM_CONNECTION_T*  pt_conn;
    INT32             i4_res;
    
    DBG_INFO((DBG_PREFIX"cm_connection_set_handler_conn(%d, %d)\n",
              h_cm_conn, h_ch_conn));
    
    i4_res = handle_get_obj(h_cm_conn, (VOID**) & pt_conn);
    
    if (i4_res == HR_OK)
    {
        DBG_INFO((DBG_PREFIX"cm_connection_set_handler_conn - %p\n",
                  & pt_conn->h_ch_conn));
        
        pt_conn->h_ch_conn = h_ch_conn;
        
        i4_res = CMR_OK;
    }
    else
    {
        i4_res = CMR_INV_ARG;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_connection_get_src_name
 *
 * Description: This API returns the name of the connection's source component.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: ps_src_name  Contains the name of the source component.
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_connection_get_src_name (HANDLE_T  h_conn,
                                  CHAR*     ps_src_name)
{
    CM_CONNECTION_T*  pt_conn;
    INT32             i4_res;

    i4_res = handle_get_obj(h_conn, (VOID**) & pt_conn);

    i4_res = ((i4_res == HR_OK) && (ps_src_name != NULL)) ? CMR_OK : CMR_FAIL;

    if (i4_res == CMR_OK)
    {
        x_strcpy(ps_src_name, pt_conn->s_src_name);
    }

    return i4_res;
}
