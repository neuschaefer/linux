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
 * $RCSfile: cm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains the implementation of the exported APIs.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "conn_mngr/x_cm.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/cm_handler_api.h"
#include "conn_mngr/cm_pipe_api.h"
#include "conn_mngr/cm_conn_api.h"
#include "conn_mngr/_cm.h"
#include "dbg/def_dbg_level_mw.h"
#include "dbg/dbg.h"

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static UINT16  ui2_cm_dbg_level = DBG_INIT_LEVEL_MW_CONN_MNGR;
static BOOL    b_init = FALSE;


/*-----------------------------------------------------------------------------
 * Name: cm_get_dbg_level
 *
 * Description: This API gets the debug level of the Connection Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: the Connection Manager debug level.
 ----------------------------------------------------------------------------*/
UINT16 cm_get_dbg_level (VOID)
{
    return ui2_cm_dbg_level;
}

/*-----------------------------------------------------------------------------
 * Name: cm_set_dbg_level
 *
 * Description: This API sets the debug level of the Connection Manager.
 *
 * Inputs:  ui2_dbg_level  Contains the new debug level.
 *
 * Outputs: -
 *
 * Returns: CMR_OK    The debug level was set successfully.
 *          CMR_FAIL  The debug level could not be set.
 ----------------------------------------------------------------------------*/
INT32 cm_set_dbg_level (UINT16  ui2_dbg_level)
{
    ui2_cm_dbg_level = ui2_dbg_level;
    
    return CMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: cm_condition_str
 *
 * Description: This API converts a condition value into a string.
 *
 * Inputs:  e_cond  Contains the condition.
 *
 * Outputs: -
 *
 * Returns: a pointer to a string representing the CM condition.
 ----------------------------------------------------------------------------*/
const CHAR* cm_condition_str (CM_COND_T  e_cond)
{
    const CHAR*  ps_str;
    
    switch (e_cond)
    {
        case CM_COND_CLOSED:          ps_str = "\"closed\"";          break;
        case CM_COND_OPENING:         ps_str = "\"opening\"";         break;
        case CM_COND_OPENED:          ps_str = "\"opened\"";          break;
        case CM_COND_CLOSING:         ps_str = "\"closing\"";         break;
        case CM_COND_DISCONNECTED:    ps_str = "\"disconnected\"";    break;
        case CM_COND_CONNECTING:      ps_str = "\"connecting\"";      break;
        case CM_COND_CONNECTED:       ps_str = "\"connected\"";       break;
        case CM_COND_DISCONNECTING:   ps_str = "\"disconnecting\"";   break;
        case CM_COND_CONNECTED_TRANS: ps_str = "\"connected trans\""; break;
        case CM_COND_CONNECTION_INFO: ps_str = "\"connection info\""; break;
        default:
            ps_str = "\"unknown CM condition\"";
    }
    
    return ps_str;
}

/*-----------------------------------------------------------------------------
 * Name: is_valid_pipe
 *
 * Description: This API checks whether a handle is a valid pipe handle.
 *
 * Inputs:  h_obj  Contains the handle to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is a handle to a pipe, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL is_valid_pipe (HANDLE_T h_obj)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_obj, & e_type);

    return (BOOL) ((i4_res == HR_OK) && (e_type == CMT_PIPE));
}

/*-----------------------------------------------------------------------------
 * Name: is_valid_obj
 *
 * Description: This API checks whether a handle is a valid pipe or connection
 *              handle.
 *
 * Inputs:  h_obj  Contains the handle to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is a handle to a pipe or to a connection, FALSE
 *          otherwise.
 ----------------------------------------------------------------------------*/
static BOOL is_valid_obj (HANDLE_T h_obj)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_obj, & e_type);

    return (BOOL) ((i4_res == HR_OK) &&
                   ((e_type == CMT_PIPE) || (e_type == CMT_CONNECTION)));
}

/*-----------------------------------------------------------------------------
 * Name: is_valid_open_pipe_command
 *
 * Description: This API checks whether a command set is valid for an open pipe
 *              operation.
 *
 * Inputs:  pt_pipe_cmd  References the array to validate.
 *
 * Outputs: -
 *
 * Returns: TRUE if the command set is valid, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL is_valid_open_pipe_command (const CM_COMMAND_T*  pt_pipe_cmd)
{
    BOOL b_is_valid = TRUE;

    if (pt_pipe_cmd != NULL)
    {
        CM_COMMAND_CODE_T e_code;
        UINT32            ui4_count;
        BOOL              b_src_type, b_src_name, b_snk_name;

        ui4_count = 0;

        b_src_type = b_src_name = b_snk_name = FALSE;

        while ((e_code = pt_pipe_cmd->e_code) != CONN_END)
        {
            switch (e_code)
            {
                case CONN_SRC_TYPE:
                    if ((! b_src_type) &&
                        (ui4_count == 0) &&
                        (pt_pipe_cmd->u.ps_name != NULL))
                    {
                        b_src_type = TRUE;
                    }
                    else
                    {
                        b_is_valid = FALSE;
                    }
                    break;

                case CONN_SRC_NAME:
                    if ((! b_src_name) &&
                        (pt_pipe_cmd->u.ps_name != NULL))
                    {
                        b_src_name = TRUE;
                    }
                    else
                    {
                        b_is_valid = FALSE;
                    }
                    break;

                case CONN_SNK_NAME:
                    if ((! b_snk_name) &&
                        (pt_pipe_cmd->u.ps_name != NULL))
                    {
                        b_snk_name = TRUE;
                    }
                    else
                    {
                        b_is_valid = FALSE;
                    }
                    break;

                default:
                    /* command for specific handler is valid */
                    b_is_valid = (((UINT32) e_code & 0x80000000U) != 0);
                    break;
            }

            if (! b_is_valid)
            {
                break;
            }

            pt_pipe_cmd++;
            ui4_count++;
        }
    }

    return b_is_valid;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pipe_handle_autofree
 *
 * Description: This API closes the pipe.
 *
 * Inputs:  h_handle  Contains the handle to delete.
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 *          -1     Routine failed.
 ----------------------------------------------------------------------------*/
static INT32  cm_pipe_handle_autofree (HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type)
{
    CM_COND_T  e_cond;
    INT32      i4_res;

    i4_res = x_cm_close_pipe(h_handle, & e_cond);

    i4_res = (i4_res == CMR_OK) ? HR_OK : -1;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_conn_handle_autofree
 *
 * Description: This API disconnects a connection.
 *
 * Inputs:  h_handle  Contains the handle to delete.
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 *          -1     Routine failed.
 ----------------------------------------------------------------------------*/
static INT32  cm_conn_handle_autofree (HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type)
{
    CM_COND_T  e_cond;
    INT32      i4_res;

    i4_res = x_cm_disconnect(h_handle, & e_cond);

    i4_res = (i4_res == CMR_OK) ? HR_OK : -1;

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_close_pipe
 *
 * Description: This API closes a pipe (previously open with x_cm_open_pipe).
 *
 * Inputs:  h_pipe  Contains a handle to the pipe to close.
 *
 * Outputs: pe_cond  Contains the pipe condition (CLOSED, CLOSING)
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid pipe handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_close_pipe (HANDLE_T    h_pipe,
                       CM_COND_T*  pe_cond)
{
    INT32  i4_res;
    
    DBG_API((DBG_PREFIX"Calling x_cm_close_pipe(handle %d, ...)\n",
             h_pipe));
    
    cm_lock();
    
    if (! is_valid_pipe(h_pipe))
    {
        DBG_ERROR((DBG_PREFIX"Invalid pipe handle (%d) in x_cm_close_pipe\n",
                   h_pipe));
        
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_COND_T  e_cond;
        
        i4_res = cm_close_pipe_op(h_pipe, & e_cond);
        
        if (pe_cond != NULL)
        {
            *pe_cond = e_cond;
        }
        
        DBG_INFO((DBG_PREFIX"x_cm_close_pipe(%d, ...) returns %d (cond %s)\n",
                  h_pipe, i4_res,
                  (i4_res == CMR_OK) ? cm_condition_str(e_cond) : "n/a"));
    }
    
    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_connect
 *
 * Description: This API initiates a connection.
 *
 * Inputs:  h_obj           Contains a handle to an existing connection or to a
 *                          pipe.
 *          pt_connect_cmd  References the commands set for the connect
 *                          operation.
 *          pv_nfy_tag      References the tag passed to the notify function.
 *          pf_nfy          References the notify function.
 *
 * Outputs: ph_conn  Contains the connection handle.
 *          pe_cond  Contains the connection condition (CONNECTING, etc)
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_connect (HANDLE_T             h_obj,
                    const CM_COMMAND_T*  pt_connect_cmd,
                    VOID*                pv_nfy_tag,
                    x_cm_nfy_fct         pf_nfy,
                    HANDLE_T*            ph_conn,
                    CM_COND_T*           pe_cond)
{
    INT32 i4_res;

    DBG_API((DBG_PREFIX"Calling x_cm_connect(handle %d, ...)\n",
             h_obj));
    
    cm_lock();
    
    if (pf_nfy == NULL || ph_conn == NULL)
    {
        DBG_ERROR((DBG_PREFIX"Invalid argument in x_cm_connect\n"));
        
        i4_res = CMR_INV_ARG;
    }
    else if (! is_valid_obj(h_obj))
    {
        DBG_ERROR((DBG_PREFIX"Invalid handle in x_cm_connect\n"));
        
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_COND_T  e_cond;
        
        i4_res = cm_connect_op(h_obj,
                               pt_connect_cmd,
                               pv_nfy_tag,
                               pf_nfy,
                               ph_conn,
                               & e_cond);
        
        if (pe_cond != NULL)
        {
            *pe_cond = e_cond;
        }
        
        DBG_INFO((DBG_PREFIX"x_cm_connect(%d, ...) returns %d (conn %d %s)\n",
                  h_obj, i4_res, *ph_conn,
                  (i4_res == CMR_OK) ? cm_condition_str(e_cond) : "n/a"));
    }
    
    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_disconnect
 *
 * Description: This API performs a disconnection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: pe_cond  Contains the connection condition (DISCONNECTING, etc)
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_disconnect (HANDLE_T    h_conn,
                       CM_COND_T*  pe_cond)
{
    INT32 i4_res;

    DBG_API((DBG_PREFIX"Calling x_cm_disconnect(handle %d, ...)\n",
             h_conn));
    
    cm_lock();
    
    if (! cm_is_valid_connection(h_conn))
    {
        DBG_ERROR((DBG_PREFIX"Invalid connection (%d) in x_cm_disconnect\n",
                   h_conn));
        
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_COND_T  e_cond;
        
        i4_res = cm_disconnect_op(h_conn,
                                  & e_cond);
        
        if (pe_cond != NULL)
        {
            *pe_cond = e_cond;
        }
        
        DBG_INFO((DBG_PREFIX"x_cm_disconnect(%d, ...) returns %d (cond %s)\n",
                  h_conn, i4_res,
                  (i4_res == CMR_OK) ? cm_condition_str(e_cond) : "n/a"));
    }

    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_get
 *
 * Description: This API gets information about a connection.
 *
 * Inputs:  h_conn        Contains a handle to a connection.
 *          e_ctrl        Contains the control code.
 *
 * Outputs: pv_ctrl_data  Contains information about the connection.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_get (HANDLE_T        h_conn,
                CM_CTRL_TYPE_T  e_ctrl,
                VOID*           pv_ctrl_data)
{
    INT32 i4_res;

    cm_lock();
    
    if ((pv_ctrl_data == NULL) ||
        (CM_CTRL_IS_PRIVATE(e_ctrl)))
    {
        i4_res = CMR_INV_ARG;
    }
    else if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        i4_res = cm_get_op(h_conn, e_ctrl, pv_ctrl_data);
    }

    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_set
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
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_set (HANDLE_T        h_conn,
                CM_CTRL_TYPE_T  e_ctrl,
                VOID*           pv_ctrl_data)
{
    INT32 i4_res;

    cm_lock();
    
    if (CM_CTRL_IS_PRIVATE(e_ctrl))
    {
        i4_res = CMR_INV_ARG;
    }
    else if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        i4_res = cm_set_op(h_conn, e_ctrl, pv_ctrl_data);
    }

    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_get_cond
 *
 * Description: This API returns the condition of a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: pe_cond  Contains the connection condition.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid connection handle.
 *          CMR_FAIL        Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 x_cm_get_cond (HANDLE_T    h_conn,
                     CM_COND_T*  pe_cond)
{
    INT32 i4_res;

    cm_lock();
    
    if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        i4_res = cm_get_cond_op(h_conn, pe_cond);
    }

    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_open_pipe
 *
 * Description: This API opens a pipe.
 *
 * Inputs:  pt_pipe_cmd   References the commands set for the open pipe
 *                        operation.
 *          ui1_priority  Contains the pipe priority.
 *          ui4_flags     Contains the pipe flags.
 *          pv_nfy_tag    References the tag passed to the notify function.
 *          pf_nfy        References the notify function.
 *
 * Outputs: ph_pipe  Contains the pipe handle.
 *          pe_cond  Contains the pipe condition (CLOSED, CLOSING)
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid pipe handle.
 *          CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_open_pipe (const CM_COMMAND_T*  pt_pipe_cmd,
                      UINT8                ui1_priority,
                      UINT32               ui4_flags,
                      VOID*                pv_nfy_tag,
                      x_cm_nfy_fct         pf_nfy,
                      HANDLE_T*            ph_pipe,
                      CM_COND_T*           pe_cond)
{
    INT32 i4_res;

    DBG_API((DBG_PREFIX"Calling x_cm_open_pipe(...)\n"));

    cm_lock();
    
    if (pf_nfy == NULL || ph_pipe == NULL)
    {
        DBG_ERROR((DBG_PREFIX"Invalid argument in x_cm_open_pipe\n"));
        
        i4_res = CMR_INV_ARG;
    }
    else if (! is_valid_open_pipe_command(pt_pipe_cmd))
    {
        DBG_ERROR((DBG_PREFIX"Invalid commands in x_cm_open_pipe\n"));
        
        i4_res = CMR_INV_COMMAND;
    }
    else
    {
        CM_COND_T  e_cond;
        
        i4_res = cm_open_pipe_op(pt_pipe_cmd,
                                 ui1_priority,
                                 ui4_flags | RM_PIPE_FLAG_ALLOW_SLAVE_SET_OP,
                                 pv_nfy_tag,
                                 pf_nfy,
                                 ph_pipe,
                                 & e_cond);
        
        if (pe_cond != NULL)
        {
            *pe_cond = e_cond;
        }
        
        DBG_INFO((DBG_PREFIX"x_cm_open_pipe(...) returns %d (cond %s)\n",
                  i4_res,
                  (i4_res == CMR_OK) ? cm_condition_str(e_cond) : "n/a"));
    }

    cm_unlock();
    
    return (i4_res);
}

/*-----------------------------------------------------------------------------
 * Name: x_cm_store_pre_conn_info
 *
 * Description: This API store the current svc info into mw eeprom.
 *
 * Inputs:  b_tv_src           Indicate the last src is dtv src or not
 *             ui1_svl_lst        Contains last svl list id.
 *             ui4_channel_id  Contains the last channel id user selected
 *
 * Outputs: -.
 *
 * Returns: CMR_OK          Routine successful.
 *              CMR_FAIL        Routine unsuccessful (other reason).
 ----------------------------------------------------------------------------*/
INT32 x_cm_store_pre_conn_info(BOOL   b_tv_src, 
                                     UINT8  ui1_svl_lst, 
                                     UINT32 ui4_channel_id)
{
    return cm_store_pre_conn_info(b_tv_src, 
                                  ui1_svl_lst, 
                                  ui4_channel_id);
}

/*-----------------------------------------------------------------------------
 * Name: cm_attach_instance
 *
 * Description: This API ataches "instance" data to a connection.
 *
 * Inputs:  h_conn       Contains a handle to a connection.
 *          pv_instance  References the "instance" data.
 *
 * Outputs: -.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid connection handle.
 ----------------------------------------------------------------------------*/
INT32 cm_attach_instance (HANDLE_T  h_conn,
                          VOID*     pv_instance)
{
    INT32 i4_res;

    if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_CONNECTION_T* pt_conn;

        i4_res = handle_get_obj(h_conn,
                                (VOID **) & pt_conn);

        if (i4_res == HR_OK)
        {
            CRIT_STATE_T t_state;

            t_state = x_crit_start();

            pt_conn->pv_instance = pv_instance;

            x_crit_end(t_state);

            i4_res = CMR_OK;
        }
        else
        {
            i4_res = CMR_INV_HANDLE;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_detach_instance
 *
 * Description: This API detaches the "instance" data attached to a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: ppv_instance  Contains a pointer to the "instance" data.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 ----------------------------------------------------------------------------*/
INT32 cm_detach_instance (HANDLE_T  h_conn,
                          VOID**    ppv_instance)
{
    INT32 i4_res;

    if (ppv_instance == NULL)
    {
        i4_res = CMR_INV_ARG;
    }
    else if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_CONNECTION_T* pt_conn;

        i4_res = handle_get_obj(h_conn,
                                (VOID **) & pt_conn);

        if (i4_res == HR_OK)
        {
            CRIT_STATE_T t_state;

            t_state = x_crit_start();

            *ppv_instance = pt_conn->pv_instance;
            pt_conn->pv_instance = NULL;

            x_crit_end(t_state);

            i4_res = CMR_OK;
        }
        else
        {
            i4_res = CMR_INV_HANDLE;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_get_instance
 *
 * Description: This API returns the "instance" data attached to a connection.
 *
 * Inputs:  h_conn  Contains a handle to a connection.
 *
 * Outputs: ppv_instance  Contains a pointer to the "instance" data.
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_ARG     Invalid argument.
 *          CMR_INV_HANDLE  Invalid connection handle.
 ----------------------------------------------------------------------------*/
INT32 cm_get_instance (HANDLE_T  h_conn,
                       VOID**    ppv_instance)
{
    INT32 i4_res;

    if (ppv_instance == NULL)
    {
        i4_res = CMR_INV_ARG;
    }
    else if (! cm_is_valid_connection(h_conn))
    {
        i4_res = CMR_INV_HANDLE;
    }
    else
    {
        CM_CONNECTION_T* pt_conn;

        i4_res = handle_get_obj(h_conn,
                                (VOID **) & pt_conn);

        if (i4_res == HR_OK)
        {
            CRIT_STATE_T t_state;

            t_state = x_crit_start();

            *ppv_instance = pt_conn->pv_instance;

            x_crit_end(t_state);

            i4_res = CMR_OK;
        }
        else
        {
            i4_res = CMR_INV_HANDLE;
        }
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_init
 *
 * Description: This API initializes the Connection Manager.
 *
 * Inputs:  ui2_num_handlers  Contains the maximum number of handlers supported
 *                            by the Connection Manager.
 *
 * Outputs: -
 *
 * Returns: CMR_OK            Routine successful.
 *          CMR_ALREADY_INIT  The Connection Manager is already initialized.
 *          CMR_INV_HANDLE    Invalid argument.
 ----------------------------------------------------------------------------*/
INT32 cm_init (UINT16 ui2_num_handlers)
{
    CRIT_STATE_T  t_state;
    INT32         i4_res;
    BOOL          b_already_init;

    t_state = x_crit_start();

    b_already_init = b_init;
    b_init = TRUE;

    x_crit_end(t_state);

    if (b_already_init)
    {
        i4_res = CMR_ALREADY_INIT;
    }
    else if (ui2_num_handlers == 0)
    {
        i4_res = CMR_INV_ARG;
    }
    else
    {
        static handle_autofree_fct  apf_autofree_fcts[] =
            {
                NULL, /* cm_handler_handle_autofree */
                cm_pipe_handle_autofree,
                cm_conn_handle_autofree
            };

        i4_res = handle_set_autofree_tbl(HT_GROUP_CM,
                                         apf_autofree_fcts);

        if (i4_res != HR_OK)
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_CANNOT_SET_AUTOFREE);
        }

        i4_res = cm_init_handlers(ui2_num_handlers);

        if (i4_res != CMR_OK)
        {
            b_init = FALSE;
        }
        else
        {
            cm_cli_init();
        }
    }

    cm_util_init();

    return i4_res;
}

HANDLE_T x_cm_pipe_get_rm_pipe (HANDLE_T h_cm_pipe)
{
    HANDLE_T h_rm_pipe = NULL_HANDLE;
    
    cm_lock();
    
    h_rm_pipe = cm_pipe_get_rm_pipe (h_cm_pipe);
    
    cm_unlock();

    return h_rm_pipe;
}

HANDLE_T x_cm_connection_get_pipe (HANDLE_T  h_conn)
{
    HANDLE_T h_cm_pipe = NULL_HANDLE;
    
    cm_lock();
    
    h_cm_pipe = cm_connection_get_pipe (h_conn);
    
    cm_unlock();
    
    return h_cm_pipe;
}
