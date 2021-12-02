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
 * $RCSfile: cm_pipe.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains the implementation of the pipe-related APIs
 *         (open, close, free, notifications, etc).
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "conn_mngr/cm_pipe_api.h"
#include "conn_mngr/_cm.h"
#include "res_mngr/rm.h"


/*-----------------------------------------------------------------------------
 * Name: cm_pipe_free_handle
 *
 * Description: This API is called when a pipe handle is freed
 *
 * Inputs:  h_pipe        Contains the pipe handle that should be freed.
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
static BOOL cm_pipe_free_handle (HANDLE_T       h_pipe,
                                 HANDLE_TYPE_T  e_type,
                                 VOID*          pv_obj,
                                 VOID*          pv_tag,
                                 BOOL           b_req_handle)
{
    if (! b_req_handle)
    {
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
 * Name: cm_nfy_from_rm
 *
 * Description: This API is called by the Resource Manager to notify the
 *              Connection Manager of a change of state on a pipe.
 *
 * Inputs:  h_rm_pipe   Contains the RM pipe handle of interest.
 *          ui1_port    Contains the port number.
 *          e_nfy_cond  Contains the notify condition.
 *          pv_nfy_tag  Contains the tag value (specified in rm_open_comp).
 *          ui4_data    Contains additional data.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID cm_nfy_from_rm(HANDLE_T   h_rm_pipe,
                           UINT8      ui1_port,
                           RM_COND_T  e_nfy_cond,
                           VOID*      pv_nfy_tag,
                           UINT32     ui4_data)
{
    CM_PIPE_T*  pt_pipe;
    VOID*       pv_pipe_nfy_tag;
    CM_COND_T   e_cm_cond;
    INT32       i4_res;

    cm_lock();

    pt_pipe = (CM_PIPE_T*) pv_nfy_tag;
    
    i4_res = x_handle_get_tag(pt_pipe->h_cm_pipe,
                              & pv_pipe_nfy_tag);

    if (i4_res == HR_OK)
    {
        if (cm_cond_from_rm_cond(& e_cm_cond, e_nfy_cond))
        {
            pt_pipe->pf_nfy(pt_pipe->h_cm_pipe,
                            e_cm_cond,
                            pv_pipe_nfy_tag,
                            ui4_data);
        }
        
        switch (e_cm_cond)
        {
            case CM_COND_CLOSED:
                handle_free(pt_pipe->h_cm_pipe, FALSE);
                break;
            default:
                break;
        }
    }

    cm_unlock();

}

/*-----------------------------------------------------------------------------
 * Name: cm_open_pipe_op
 *
 * Description: This API is called by x_cm_open_pipe and performs the actual
 *              opening of the pipe.
 *
 * Inputs:  pt_pipe_cmd   References the array of commands.
 *          ui1_priority  Contains the priority of the pipeline.
 *          ui4_flags     Contains the pipeline flags.
 *          pv_nfy_tag    References.the tag passed to the notify function.
 *          pf_nfy        References the notify function.
 *
 * Outputs: ph_pipe  Contains the handle to the new pipeline.
 *          pe_cond  Contains the pipeline's condition (OPENED, etc).
 *
 * Returns: CMR_OK    Routine successful (the pipe has been created)
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_open_pipe_op (const CM_COMMAND_T*  pt_pipe_cmd,
                       UINT8                ui1_priority,
                       UINT32               ui4_flags,
                       VOID*                pv_nfy_tag,
                       x_cm_nfy_fct         pf_nfy,
                       HANDLE_T*            ph_pipe,
                       CM_COND_T*           pe_cond)
{
    const CHAR*  ps_src_name = NULL;
    const CHAR*  ps_snk_name = NULL;
    CM_PIPE_T*   pt_pipe = NULL;
    HANDLE_T     h_cm_pipe;
    INT32        i4_res;

    if (pt_pipe_cmd != NULL)
    {
        const CM_COMMAND_T*  pt_cmd;
        
        pt_cmd = cm_find_cmd(pt_pipe_cmd, CONN_SRC_NAME);
        
        if (pt_cmd != NULL)
        {
            ps_src_name = pt_cmd->u.ps_name;
        }
        
        pt_cmd = cm_find_cmd(pt_pipe_cmd, CONN_SNK_NAME);
        
        if (pt_cmd != NULL)
        {
            ps_snk_name = pt_cmd->u.ps_name;
        }
    }
    
    i4_res = handle_alloc_and_obj(CMT_PIPE,
                                  pv_nfy_tag,
                                  sizeof(CM_PIPE_T),
                                  cm_pipe_free_handle,
                                  & h_cm_pipe,
                                  (VOID**) & pt_pipe);
    
    if ( (i4_res == HR_OK)&&(pt_pipe!=NULL) )
    {
        HANDLE_T   h_rm_pipe;
        RM_COND_T  e_rm_cond;
        
        i4_res = rm_open_pipe(ps_src_name,
                              ps_snk_name,
                              ui1_priority,
                              ui4_flags,
                              pt_pipe, /* the tag is the CM pipe */
                              cm_nfy_from_rm,
                              & h_rm_pipe,
                              & e_rm_cond);
        
        if (i4_res == RMR_OK)
        {
            pt_pipe->h_rm_pipe  = h_rm_pipe;
            pt_pipe->h_cm_pipe  = h_cm_pipe;
            pt_pipe->pf_nfy     = pf_nfy;

            if (ps_src_name != NULL)
            {
                x_strncpy(pt_pipe->s_src_name,
                          ps_src_name,
                          CONN_SRC_NAME_MAX_STRLEN + 1);
            }

            if (pt_pipe_cmd != NULL)
            {
                const CM_COMMAND_T*  pt_cmd;
                
                pt_cmd = cm_find_cmd(pt_pipe_cmd, CONN_SRC_TYPE);
                
                if (pt_cmd != NULL)
                {
                    x_strncpy(pt_pipe->s_src_type,
                              pt_cmd->u.ps_name,
                              CONN_SRC_TYPE_MAX_STRLEN + 1);
                }
            }
            
            cm_cond_from_rm_cond(pe_cond, e_rm_cond);

            *ph_pipe = h_cm_pipe;
            
            i4_res = CMR_OK;
        }
        else
        {
            handle_free(h_cm_pipe, FALSE);

            i4_res = CMR_FAIL;
        }
    }
    else
    {
        i4_res = CMR_FAIL;
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pipe_rm_close
 *
 * Description: This API is closes the component and deletes the pipe
 *
 * Inputs:  h_cm_pipe  Contains the CM pipe handle.
 *          b_nfy_app  Specifies whether the application should be notified.
 *
 * Outputs: pe_cond  Contains the pipeline's condition (OPENED, etc).
 *
 * Returns: CMR_OK          Routine successful.
 *          CMR_INV_HANDLE  Invalid handle.
 *          CMR_FAIL        Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_pipe_rm_close (HANDLE_T    h_cm_pipe,
                        BOOL        b_nfy_app,
                        CM_COND_T*  pe_cond)
{
    CM_PIPE_T*    pt_pipe;
    CRIT_STATE_T  t_state;
    INT32         i4_res;
    
    t_state = x_crit_start();
    
    i4_res = handle_get_obj(h_cm_pipe, (VOID**) & pt_pipe);

    if (i4_res == HR_OK)
    {
        VOID*      pv_pipe_nfy_tag;
        HANDLE_T   h_rm_pipe;
        RM_COND_T  e_rm_cond;
        
        h_rm_pipe = pt_pipe->h_rm_pipe;
        
        x_handle_get_tag(h_cm_pipe,
                         & pv_pipe_nfy_tag);
        
        x_crit_end(t_state);
        
        i4_res = rm_close(h_rm_pipe, TRUE, & e_rm_cond);

        if (i4_res == RMR_OK)
        {
            if (e_rm_cond == RM_COND_CLOSED)
            {
                if (b_nfy_app)
                {
                    pt_pipe->pf_nfy(h_cm_pipe,
                                    CM_COND_CLOSED,
                                    pv_pipe_nfy_tag, 0);
                }
                
                handle_free(h_cm_pipe, FALSE);
                
                *pe_cond = CM_COND_CLOSED;
            }
            else
            {
                *pe_cond = CM_COND_CLOSING;
            }
            
            i4_res = CMR_OK;
        }
        else
        {
            i4_res = CMR_FAIL;
        }
    }
    else
    {
        x_crit_end(t_state);

        i4_res = CMR_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_close_pipe_op
 *
 * Description: This API is called by x_cm_close_pipe and performs the actual
 *              closing of the pipe.
 *
 * Inputs:  h_pipe  Contains a handle to the pipe to close.
 *
 * Outputs: pe_cond  Contains the pipeline's condition (CLOSED, etc).
 *
 * Returns: CMR_OK    Routine successful.
 *          CMR_FAIL  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 cm_close_pipe_op (HANDLE_T    h_pipe,
                        CM_COND_T*  pe_cond)
{
    CRIT_STATE_T  t_state;
    CM_PIPE_T*    pt_pipe;
    INT32         i4_res;

    t_state = x_crit_start();
    
    i4_res = handle_get_obj(h_pipe, (VOID**) & pt_pipe);

    if (i4_res == HR_OK)
    {
        CM_CONNECTION_T*  pt_conn;
        HANDLE_T          h_cm_conn;
        CM_COND_T         e_cm_cond;
        
        h_cm_conn = pt_pipe->h_cm_conn;
        
        if (handle_get_obj(h_cm_conn, (VOID**) & pt_conn) == HR_OK)
        {
            pt_conn->b_close_pipe = TRUE;
        }
        
        x_crit_end(t_state);
        
        i4_res = cm_disconnect_op(h_cm_conn, & e_cm_cond);

        if (((i4_res == CMR_OK) && (e_cm_cond == CM_COND_DISCONNECTED))
            ||
            (i4_res == CMR_INV_HANDLE))
        {
            i4_res = cm_pipe_rm_close(h_pipe, FALSE, pe_cond);
            
            if (i4_res == CMR_OK)
            {
                if (*pe_cond == CM_COND_CLOSED)
                {
                    handle_free(h_pipe, FALSE);
                }
                
                i4_res = CMR_OK;
            }
            else
            {
                i4_res = CMR_FAIL;
            }
        }
        else if (i4_res == CMR_OK)
        {
            *pe_cond = CM_COND_CLOSING;
        }
    }
    else
    {
        x_crit_end(t_state);
        
        i4_res = CMR_INV_HANDLE;
    }

    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_pipe_get_rm_pipe
 *
 * Description: This API returns a handle to a Resource Manager pipe given a
 *              handle to a Connection Manager pipe.
 *
 * Inputs:  h_cm_pipe  Contains a handle to a Connection Manager pipe.
 *
 * Outputs: -
 *
 * Returns: handle to a Resource Manager pipe, or NULL_HANDLE.
 ----------------------------------------------------------------------------*/
HANDLE_T cm_pipe_get_rm_pipe (HANDLE_T  h_cm_pipe)
{
    CM_PIPE_T*  pt_pipe;
    HANDLE_T    h_rm_pipe;
    INT32       i4_res;
    
    i4_res = handle_get_obj(h_cm_pipe, (VOID**) & pt_pipe);

    if (i4_res == HR_OK)
    {
        h_rm_pipe = pt_pipe->h_rm_pipe;
    }
    else
    {
        h_rm_pipe = NULL_HANDLE;
    }
    
    return h_rm_pipe;
}
