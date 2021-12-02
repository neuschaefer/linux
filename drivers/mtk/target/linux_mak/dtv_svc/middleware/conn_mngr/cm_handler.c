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
 * $RCSfile: cm_handler.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains the handler-related types and APIs
 *         definitions.
 *---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "conn_mngr/cm_handler_api.h"
#include "conn_mngr/cm_conn_api.h"
#include "conn_mngr/cm.h"
#include "conn_mngr/_cm.h"
#include "dbg/dbg.h"


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

static HANDLE_T       h_semaphore;
static CM_HANDLER_T*  pt_handlers = NULL;
static UINT16         ui2_num_total_handlers = 0;
static BOOL           b_init = FALSE;


/*-----------------------------------------------------------------------------
 * Name: cm_lock
 *
 * Description: This API locks the Connection Manager. This API will abort if
 *              the semaphore cannot be acquired.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_lock (VOID)
{
    INT32 i4_res;
    
    i4_res = x_sema_lock(h_semaphore,
                         X_SEMA_OPTION_WAIT);

    if (i4_res != OSR_OK)
    {
        ABORT(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_CM);
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_unlock
 *
 * Description: This API unlocks the Connection Manager. This API will abort if
 *              the semaphore cannot be released.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_unlock (VOID)
{
    INT32 i4_res;
    
    i4_res = x_sema_unlock(h_semaphore);
    
    if (i4_res != OSR_OK)
    {
        ABORT(DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_CM);
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_list_handlers
 *
 * Description: This API lists the handlers registered with the Connection
 *              Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_list_handlers (VOID)
{
    UINT16  ui2_i;
    
    x_dbg_stmt(DBG_PREFIX"Handlers:\n");
    
    cm_lock();
    
    for (ui2_i = 0; ui2_i < ui2_num_total_handlers; ui2_i++)
    {
        if (pt_handlers[ui2_i].ps_names != NULL)
        {
            x_dbg_stmt(DBG_PREFIX" %s\n", pt_handlers[ui2_i].ps_names);
        }
    }
    
    cm_unlock();
}

/*-----------------------------------------------------------------------------
 * Name: cm_list_connections
 *
 * Description: This API lists the connection for each handler registered with
 *              the Connection Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_list_connections (VOID)
{
    UINT16  ui2_i;
    
    x_dbg_stmt(DBG_PREFIX"Connections:\n");
    
    cm_lock();
    
    for (ui2_i = 0; ui2_i < ui2_num_total_handlers; ui2_i++)
    {
        if (pt_handlers[ui2_i].ps_names != NULL)
        {
            HANDLE_T  h_cm_conn;
            
            h_cm_conn = pt_handlers[ui2_i].h_cm_conn;
            
            while (h_cm_conn != NULL_HANDLE)
            {
                CM_CONNECTION_T*  pt_conn = NULL;
                HANDLE_TYPE_T     e_type;
                INT32             i4_res;
                
                i4_res = handle_get_type_obj(h_cm_conn,
                                             & e_type,
                                             (VOID**) & pt_conn);
                
                if ((i4_res == HR_OK)
                    &&
                    (e_type == CMT_CONNECTION)
                    &&
                    (pt_conn != NULL))
                {
                    x_dbg_stmt(DBG_PREFIX
                               " %d (pipe %d, type:\"%s\", src:\"%s\", %s)\n",
                               h_cm_conn,
                               pt_conn->h_pipe,
                               pt_conn->s_src_type,
                               pt_conn->s_src_name,
                               cm_condition_str(pt_conn->e_cond));
                    
                    h_cm_conn = pt_conn->h_next;
                }
                else
                {
                    h_cm_conn = NULL_HANDLE;
                }
            }
        }
    }
    
    cm_unlock();
}

/*-----------------------------------------------------------------------------
 * Name: is_valid_handler
 *
 * Description: This API checks whether a handle is a valid handler handle.
 *
 * Inputs:  h_handler  Contains the handle to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if the handle is a handle to a connection handler, FALSE
 *          otherwise.
 ----------------------------------------------------------------------------*/
static BOOL is_valid_handler(HANDLE_T h_handler)
{
    HANDLE_TYPE_T  e_type;
    INT32          i4_res;

    i4_res = x_handle_get_type(h_handler, & e_type);

    return (BOOL) ((i4_res == HR_OK) && (e_type == CMT_HANDLER));
}

/*-----------------------------------------------------------------------------
 * Name: is_valid_name
 *
 * Description: This API checks whether a handle is a valid handler handle.
 *
 * Inputs:  ps_names  References a string that contains all the names to check.
 *
 * Outputs: -
 *
 * Returns: TRUE if all the names are valid, FALSE othewise.
 ----------------------------------------------------------------------------*/
static BOOL is_valid_name(const CHAR* ps_names)
{
    while (TRUE)
    {
        CHAR c_char;
        
        c_char = *ps_names++;

        /* the first character has to be a letter */
        if (! ((c_char >= 'a' && c_char <= 'z') ||
               (c_char >= 'A' && c_char <= 'Z')))
        {
            return FALSE;
        }

        /* following characters can be letters, digits or underscore */
        while (((c_char = *ps_names++) != ',') && (c_char != '\0'))
        {
            if (! ((c_char >= 'a' && c_char <= 'z') ||
                   (c_char >= 'A' && c_char <= 'Z') ||
                   (c_char >= '0' && c_char <= '9') ||
                   (c_char == '_')))
            {
                return FALSE;
            }
        }

                
        if (c_char == '\0')
        {
            return TRUE;
        }
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_match_name
 *
 * Description: This API checks if a name matches one of the handler's names.
 *
 * Inputs:  ps_names    References a string that contains all the names to
 *                      check against.
 *          ps_name     References a string that contains one name.
 *          z_name_len  Contains the length of the ps_name string.
 *
 * Outputs: -
 *
 * Returns: TRUE if ps_name matches one of the names.
 ----------------------------------------------------------------------------*/
static BOOL cm_handler_match_name (const CHAR*  ps_names,
                                   const CHAR*  ps_name,
                                   SIZE_T       z_name_len)
{
    SIZE_T z_len = 0;
    BOOL   b_res;
    
    while (TRUE)
    {
        CHAR c_char;
        
        c_char = ps_names[z_len];
        
        if (c_char == '\0' || c_char == ',')
        {
            if ((z_len == z_name_len) && (x_memcmp(ps_name, ps_names, z_name_len) == 0))
            {
                b_res = TRUE;
                break;
            }
            
            if (c_char == '\0')
            {
                b_res = FALSE;
                break;
            }
            
            ps_names += z_len + 1;
            
            z_len = 0;
        }
        else
        {
            z_len++;
        }
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_find_by_name_and_len
 *
 * Description: This API finds a specific connection handler given a name and a
 *              name length.
 *
 * Inputs:  ps_name     References a string that contains the name of the
 *                      connection handler to look for.
 *          z_name_len  Contains the length of the name.
 *
 * Outputs: -
 *
 * Returns: a pointer to the connection handler structure if it has been found,
 *          or NULL.
 ----------------------------------------------------------------------------*/
static CM_HANDLER_T* cm_handler_find_by_name_and_len(const CHAR*  ps_name,
                                                     SIZE_T       z_name_len)
{
    UINT16 ui2_i;

    for (ui2_i = 0; ui2_i < ui2_num_total_handlers; ui2_i++)
    {
        const CHAR* ps_names;

        ps_names = pt_handlers[ui2_i].ps_names;
        
        if (ps_names != NULL)
        {
            if (cm_handler_match_name(ps_names,
                                      ps_name, z_name_len))
            {
                return & pt_handlers[ui2_i];
            }
        }
    }
    
    return NULL;

}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_find_by_name
 *
 * Description: This API finds a specific connection handler given a name.
 *
 * Inputs:  ps_name  References a string that contains the name of the
 *                   connection handler to look for.
 *
 * Outputs: -
 *
 * Returns: a pointer to the connection handler structure if it has been found,
 *          or NULL.
 ----------------------------------------------------------------------------*/
CM_HANDLER_T* cm_handler_find_by_name(const CHAR*  ps_name)
{
    SIZE_T  z_name_len;

    z_name_len = x_strlen(ps_name);

    return cm_handler_find_by_name_and_len(ps_name, z_name_len);
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_exists_by_name
 *
 * Description: This API determines whether or not a connection handler exists
 *              given a name and a name length.
 *
 * Inputs:  ps_name     References a string that contains the name of the
 *                      connection handler to look for.
 *          z_name_len  Contains the length of the name.
 *
 * Outputs: -
 *
 * Returns: TRUE if the connection handler exists, FALSE otherwise.
 ----------------------------------------------------------------------------*/
static BOOL cm_handler_exists_by_name(const CHAR*  ps_name,
                                      SIZE_T       z_name_len)
{
    return (cm_handler_find_by_name_and_len(ps_name, z_name_len) != NULL);
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_are_equal
 *
 * Description: This API determines whether or not 2 names represent a same
 *              connection handler.
 *
 * Inputs:  ps_name_1   References a string that contains the first name.
 *          ps_name_2   References a string that contains the second name.
 *
 * Outputs: -
 *
 * Returns: TRUE if the 2 names represent the same connection handler.
 ----------------------------------------------------------------------------*/
BOOL cm_handler_are_equal (const CHAR*  ps_name_1,
                           const CHAR*  ps_name_2)
{
    BOOL  b_res;

    b_res = (x_strcmp(ps_name_1, ps_name_2) == 0);
    
    if (! b_res)
    {
        CM_HANDLER_T*  pt_handler;

        pt_handler = cm_handler_find_by_name(ps_name_1);

        if (pt_handler != NULL)
        {
            SIZE_T  z_name_len;

            z_name_len = x_strlen(ps_name_2);
            
            b_res = cm_handler_match_name(pt_handler->ps_names,
                                          ps_name_2, z_name_len);
        }
    }

    return b_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_link_conn
 *
 * Description: This API links a connection to a connection handler.
 *
 * Inputs:  pt_handler  References the handler structure.
 *          h_cm_conn   Contains the connection handle.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_handler_link_conn(CM_HANDLER_T*  pt_handler,
                          HANDLE_T       h_cm_conn)
{
    CM_CONNECTION_T*  pt_conn;
    CRIT_STATE_T      t_state;
    INT32             i4_res;
    
    t_state = x_crit_start();
    
    i4_res = handle_get_obj(h_cm_conn, (VOID**) & pt_conn);
    
    DBG_ASSERT((i4_res == HR_OK) && (pt_conn->h_handler == NULL_HANDLE),
               DBG_ABRT_CANNOT_GET_HANDLE_OBJ);
    
    pt_conn->h_next = pt_handler->h_cm_conn;
    
    pt_conn->h_handler = pt_handler->h_handler;
    
    pt_handler->h_cm_conn = h_cm_conn;
    
    x_crit_end(t_state);
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_delink_conn
 *
 * Description: This API delinks a connection from the handler
 *
 * Inputs:  h_handler  Contains the handle to the connection handler.
 *          h_cm_conn  Contains the connection handle.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID cm_handler_delink_conn (HANDLE_T  h_cm_conn)
{
    CM_CONNECTION_T*  pt_conn;
    CM_HANDLER_T*     pt_handler;
    HANDLE_T*         ph_cm_conn;
    HANDLE_T          h_next_cm_conn;
    CRIT_STATE_T      t_state;
    INT32             i4_res;
    
    t_state = x_crit_start();
    
    i4_res = handle_get_obj(h_cm_conn, (VOID**) & pt_conn);
    
    DBG_ASSERT(i4_res == HR_OK, DBG_ABRT_CANNOT_GET_HANDLE_OBJ);
    
    i4_res = handle_get_obj(pt_conn->h_handler, (VOID**) & pt_handler);
    
    DBG_ASSERT(i4_res == HR_OK, DBG_ABRT_CANNOT_GET_HANDLE_OBJ);
    
    pt_conn->h_handler = NULL_HANDLE;
    
    ph_cm_conn = & pt_handler->h_cm_conn;
    
    h_next_cm_conn = pt_conn->h_next;
    
    while (*ph_cm_conn != NULL_HANDLE)
    {
        if (*ph_cm_conn == h_cm_conn)
        {
            *ph_cm_conn = h_next_cm_conn;
            break;
        }
        
        i4_res = handle_get_obj(*ph_cm_conn, (VOID**) & pt_conn);
        
        DBG_ASSERT(i4_res == HR_OK, DBG_ABRT_CANNOT_GET_HANDLE_OBJ);
        
        ph_cm_conn = & pt_conn->h_next;
    }
    
    x_crit_end(t_state);
}

/*-----------------------------------------------------------------------------
 * Name: cm_handler_free_handle
 *
 * Description: This API is called when a connection handler handle is freed.
 *
 * Inputs:  h_handle      Contains the handle that should be freed.
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
static BOOL cm_handler_free_handle (HANDLE_T       h_handle,
                                    HANDLE_TYPE_T  e_type,
                                    VOID*          pv_obj,
                                    VOID*          pv_tag,
                                    BOOL           b_req_handle)
{
    if (! b_req_handle)
    {
        CM_HANDLER_T* pt_handler;
        
        pt_handler = (CM_HANDLER_T*) pv_obj;
        
        x_mem_free(pt_handler->ps_names);
        
        pt_handler->ps_names = NULL;
        
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*-----------------------------------------------------------------------------
 * Name: cm_reg_handler
 *
 * Description: This API registers a connection handler.
 *
 * Inputs:  ps_names    References a string that contains all the names for the
 *                      connection handler.
 *          pt_fct_tbl  References the connection handler's function table.
 *
 * Outputs: ph_handler  Contains a handle to the connection handler.
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Invalid argument.
 *          CMR_FAIL     Routine unsuccessful (other reason)
 ----------------------------------------------------------------------------*/
INT32 cm_reg_handler (const CHAR*                  ps_names,
                      const CM_HANDLER_FCT_TBL_T*  pt_fct_tbl,
                      HANDLE_T*                    ph_handler)
{
    INT32 i4_res;

    if (! b_init)
    {
        i4_res = CMR_NOT_INIT;
    }
    else if ((ps_names == NULL)                  ||
             (pt_fct_tbl == NULL)                ||
             (pt_fct_tbl->pf_connect == NULL)    ||
             (pt_fct_tbl->pf_disconnect == NULL))
    {
        i4_res = CMR_INV_ARG;
    }
    else if (! is_valid_name(ps_names))
    {
        i4_res = CMR_INV_NAME;
    }
    else
    {
        const CHAR*  ps_name;
        SIZE_T       z_len;

        ps_name = ps_names;
        z_len   = 0;

        cm_lock();
        
        while (TRUE)
        {
            CHAR c_char;
            
            c_char = ps_name[z_len];

            if (c_char == ',' || c_char == '\0')
            {
                BOOL b_handler_exists;

                b_handler_exists = cm_handler_exists_by_name(ps_name, z_len);
                
                if (b_handler_exists)
                {
                    i4_res = CMR_ALREADY_EXISTS;
                    break;
                }
                
                if (c_char == '\0')
                {
                    i4_res = CMR_OK;
                    break;
                }
                
                ps_name += z_len + 1;
                
                z_len = 0;
            }
            else
            {
                z_len++;
            }
        }

        if (i4_res == CMR_OK)
        {
            UINT16 ui2_i;
            
            for (ui2_i = 0; ui2_i < ui2_num_total_handlers; ui2_i++)
            {
                if (pt_handlers[ui2_i].ps_names == NULL)
                {
                    break;
                }
            }
            
            if (ui2_i < ui2_num_total_handlers)
            {
                CHAR *ps_names_dup;
                
                ps_names_dup = x_strdup(ps_names);
                
                if (ps_names_dup != NULL)
                {
                    i4_res = handle_alloc(CMT_HANDLER,
                                          & pt_handlers[ui2_i],
                                          NULL,
                                          cm_handler_free_handle,
                                          ph_handler);
                    
                    if (i4_res == HR_OK)
                    {
                        pt_handlers[ui2_i].ps_names = ps_names_dup;
                        
                        pt_handlers[ui2_i].t_fct_tbl = *pt_fct_tbl;

                        pt_handlers[ui2_i].h_handler = *ph_handler;
                        
                        i4_res = CMR_OK;
                    }
                    else
                    {
                        x_mem_free(ps_names_dup);
                        
                        i4_res = CMR_OUT_OF_HANDLES;
                    }
                }
                else
                {
                    i4_res = CMR_NOT_ENOUGH_MEM;
                }
            }
            else
            {
                i4_res = CMR_OUT_OF_HANDLERS;
            }
        }

        cm_unlock();
    }
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_unreg_handler
 *
 * Description: This API unregisters a connection handler.
 *
 * Inputs:  h_handler  Contains a handle to the connection handler.
 *          b_force    Contains a boolean that specifies whether all the
 *                     connections on that handler should be closed.
 *
 * Outputs: -
 *
 * Returns: CMR_OK       Routine successful.
 *          CMR_INV_ARG  Invalid argument.
 *          CMR_FAIL     Routine unsuccessful (other reason)
 ----------------------------------------------------------------------------*/
INT32 cm_unreg_handler (HANDLE_T  h_handler,
                        BOOL      b_force)
{
    INT32 i4_res;
    
    cm_lock();
    
    if (is_valid_handler(h_handler))
    {
        CM_HANDLER_T*  pt_handler;
        
        i4_res = handle_get_obj(h_handler, (VOID**) & pt_handler);
        
        if (i4_res == HR_OK)
        {
            if (b_force)
            {
                HANDLE_T  h_cm_conn;

                h_cm_conn = pt_handler->h_cm_conn;

                while (h_cm_conn != NULL_HANDLE)
                {
                    CM_CONNECTION_T*  pt_conn;
                    
                    i4_res = handle_get_obj(h_cm_conn,
                                            (VOID**) & pt_conn);
                    
                    if (i4_res == HR_OK)
                    {
                        x_cm_nfy_fct  pf_nfy;
                        VOID*         pv_tag;
                        CM_COND_T     e_cond;
                        
                        pf_nfy = pt_conn->pf_nfy;
                        
                        x_handle_get_tag(h_cm_conn, & pv_tag);
                        
                        cm_disconnect_op(h_cm_conn, & e_cond);
                        
                        if (e_cond == CM_COND_DISCONNECTED)
                        {
                            pf_nfy(h_cm_conn, CM_COND_DISCONNECTED,
                                   pv_tag, 0);
                        }
                    }
                    else
                    {
                        i4_res = CMR_FAIL;
                        break;
                    }
                    
                    h_cm_conn = pt_conn->h_next;
                }
            }
            
            x_mem_free(pt_handler->ps_names);
            pt_handler->ps_names = NULL;
            
            i4_res = CMR_OK;
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
    
    cm_unlock();
    
    return i4_res;
}

/*-----------------------------------------------------------------------------
 * Name: cm_init_handlers
 *
 * Description: This API initializes the connection manager handlers.
 *
 * Inputs:  ui2_num_handlers  Contains the maximum number of handlers supported
 *                            by the Connection Manager. This API will abort in
 *                            case of failure.
 *
 * Outputs: -
 *
 * Returns: CMR_OK  Routine successful.
 ----------------------------------------------------------------------------*/
INT32 cm_init_handlers (UINT16 ui2_num_handlers)
{
    INT32 i4_res;

    i4_res = x_sema_create(& h_semaphore,
                           X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK);
    
    if (i4_res != OSR_OK)
    {
        ABORT(DBG_CAT_SEMAPHORE,
              DBG_ABRT_CANNOT_CREATE_CM_SEMAPHORE);
    }
    
    pt_handlers = (CM_HANDLER_T *) x_mem_calloc(ui2_num_handlers,
                                                sizeof(CM_HANDLER_T));
    
    if (pt_handlers == NULL)
    {
        ABORT(DBG_CAT_MEMORY,
              DBG_ABRT_CANNOT_ALLOCATE_HANDLERS_MEM);
    }

    ui2_num_total_handlers = ui2_num_handlers;
    
    b_init = TRUE;

    i4_res = CMR_OK;

    return i4_res;
}
