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
 * $RCSfile: flm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all Favorites List Manager (FLM)
 *         public function implementations.
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/

#include "flm/flm.h"
#include "flm/x_flm.h"
#include "dbg/def_dbg_level_mw.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"


/*-------------------------------------------------------------------------
                    constants
 ------------------------------------------------------------------------*/

/* FLM Master list handle type */
#define FLM_MASTER_HANDLE (HT_GROUP_FLM + ((HANDLE_TYPE_T) + 2))

#ifndef DBG_INIT_LEVEL_MW_FLM
#define DBG_INIT_LEVEL_MW_FLM  DBG_LEVEL_ERROR
#endif
/*-------------------------------------------------------------------------
                    global variables
 ------------------------------------------------------------------------*/
/* This is only set TRUE if flm_init runs successfully. It is the only time
   that the value will be changed. */
static BOOL                b_flm_initialized = FALSE;

/* This is the handle to the list of favorites lists names.
   This will be NULLHANDLE only if flm_init has not been run or Favorites lists
   are to be turned off via flm_init */
static HANDLE_T            h_flm_master_list = NULL_HANDLE;

/* These two global variables will be set by system startup */
UINT16              ui2_max_num_fav_lists = 0;
UINT16              ui2_max_fav_size = 0;

static handle_autofree_fct apf_autofree_fcts[3];

#ifdef CLI_LVL_ALL

static CLI_EXEC_T at_cli_dbg_cmd[] = {
    {CLI_GET_DBG_LVL_STR, NULL, flm_get_dbg_level, NULL, CLI_GET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR, NULL, flm_set_dbg_level, NULL, CLI_SET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg[] = {
    {"flm", NULL, NULL, &(at_cli_dbg_cmd[0]), "Favorites List Manager commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};


#endif

UINT16     ui2_flm_debug_level = DBG_INIT_LEVEL_MW_FLM;

/*-------------------------------------------------------------------------
                    functions
 ------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Name: flm_handle_autofree
 *
 * Description: Frees the given FLM handle
 *
 * Inputs: -
 *    h      - FLM handle to free
 *    e_type - Type of handle
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    See handle_free
 *
 ------------------------------------------------------------------------*/
INT32 flm_handle_autofree(
    HANDLE_T      h,
    HANDLE_TYPE_T e_type
)
{
    return handle_free(h, FALSE);
}

/*-------------------------------------------------------------------------
 * Name: flm_nfy_handle_autofree
 *
 * Description: Frees the given FLM Notification handle
 *
 * Inputs: -
 *    h      - FLM Notification handle to free
 *    e_type - Type of handle
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    See handle_free
 *
 ------------------------------------------------------------------------*/
INT32 flm_nfy_handle_autofree(
    HANDLE_T      h,
    HANDLE_TYPE_T e_type
)
{
    return handle_free(h, FALSE);
}

/*-------------------------------------------------------------------------
 * Name: flm_init
 *
 * Description: This function initializes FLM globals
 *
 * Inputs: -
 *    ui2_num_fav_lists - maximum number of favorite lists allowed
 *    ui2_fav_size      - maximum number of entries in a favorites list
 *
 * Side Effects
 * These global variables are set:
 *    h_flm_master_list     - handle to the favorites master list
 *    ui2_max_num_fav_lists - maximum number of favorite lists allowed
 *    ui2_max_fav_size      - maximum number of entries in a favorites list
 *
 * Returns FLMR_OK only. If an error is encountered, an abort occurs.
 *
 * Exceptions:
 *    Aborts on any errors
 *
 ------------------------------------------------------------------------*/
INT32 flm_init(
    UINT16 ui2_num_fav_lists,
    UINT16 ui2_fav_size
)
{
    INT32                i4_result;
    DB_FIELD_INFO_T      t_field;
    FLM_MASTER_T*        pt_master;

    if (b_flm_initialized)
    {
        FLM_ABORT(DBG_CAT_INIT, 0);
    }

    b_flm_initialized = TRUE;

    ui2_max_num_fav_lists = ui2_num_fav_lists;
    ui2_max_fav_size = ui2_fav_size;
    h_flm_master_list = NULL_HANDLE;

    if (ui2_num_fav_lists == 0)
    {
        /* favorites lists not supported */
        ui2_max_fav_size = 0;
        return FLMR_OK;  
    }

    flm_new_master(&h_flm_master_list, &pt_master);

    i4_result = db_get_handle_by_name(
                   FLM_MASTER_LIST_NAME,
                   0,
                   NULL,
                   &(pt_master->h_db));

    /* create FLM master list database object if it doesn't exist */
    if (i4_result == DBR_NOT_FOUND)
    {
        t_field.ui4_field_type = (DB_FIELD_T) DB_MAKE_FIELD_TYPE(DBF_STRING_T, FLM_NAME_FIELD_T);
        t_field.ui2_field_element_count = FLM_NAME_LEN;
        i4_result = db_create( 
                  ui2_max_num_fav_lists,
                  DB_MEM_STATIC, 
                  FLM_MASTER_LIST_NAME,
                  1,
                  &t_field,
                  1,
                  &t_field.ui4_field_type,
                  (VOID*) pt_master,
                  NULL,
                  &(pt_master->h_db)
                );
    }
    if (i4_result != DBR_OK)
    {
        FLM_ABORT(DBG_CAT_INIT, 4);
    }

    /* register handle free functions */
    apf_autofree_fcts[0] = flm_handle_autofree;
    apf_autofree_fcts[1] = flm_nfy_handle_autofree;
    apf_autofree_fcts[2] = NULL;  /* never freed */
    i4_result = handle_set_autofree_tbl(
                   HT_GROUP_FLM,
                   apf_autofree_fcts
          );
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INIT, 5);
    }

#ifdef CLI_LVL_ALL
    /* register CLI stuff */
    x_cli_attach_cmd_tbl(at_cli_dbg, CLI_CAT_MW, CLI_GRP_NONE);
#endif

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_new_master
 *
 * Description: This allocates a new master list object
 *
 * Inputs: -
 *    ph_master  - pointer to handle (is this really necessary?)
 *    ppt_master - pointer to master list object
 *
 * Returns
 *    FLMR_OK
 *
 * Exceptions
 *    Aborts on any error.
 *
 ------------------------------------------------------------------------*/
INT32 flm_new_master(
    HANDLE_T*      ph_master,
    FLM_MASTER_T** ppt_master
)
{
    INT32                i4_result;
    FLM_MASTER_NOTIFY_T* pt_head;

    pt_head = x_mem_calloc(1, sizeof(FLM_MASTER_NOTIFY_T));
    if (pt_head == NULL)
    {
        FLM_ABORT(DBG_CAT_INIT, 1);
    }

    *ppt_master = x_mem_calloc(1, sizeof(FLM_MASTER_T));
    if (*ppt_master == NULL)
    {
        FLM_ABORT(DBG_CAT_INIT, 2);
    }

    i4_result = handle_alloc(FLM_MASTER_HANDLE,
                             (VOID*) *ppt_master,
                             0,
                             flm_master_free_callback,
                             ph_master);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INIT, 3);
    }

    /* add dummy head to master notification list */
    (*ppt_master)->pt_nfy_list = pt_head;

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_get_master_list
 *
 * Description: This function gets the handle to the master list.
 *              This is not a copy of the handle, but the real thing.
 *
 * Ouput
 *   ph_master_list - pointer to memory to put handle to master list
 *
 * Returns: - FLMR_OK
 *            FLMR_INV_ARG
 *
 * Exceptions
 *    Aborts if h_flm_master_list is NULL
 *
 ------------------------------------------------------------------------*/
INT32 flm_get_master_list
(
    HANDLE_T* ph_master_list
)
{
    FLM_ASSERT(h_flm_master_list, DBG_CAT_INV_OP, 0);

    if (ph_master_list == NULL)
    {
        return FLMR_INV_ARG;
    }

    *ph_master_list = h_flm_master_list;
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_add_to_master
 *
 * Description: This function adds a name to the master list
 *
 * Inputs: -
 *            ps_name - name of new favorites list
 *
 * Side Effects - Puts the name into the master list
 *
 * Returns: - FLMR_OK
 *            FLMR_INV_ARG
 *            FLMR_CANT_CREATE_LIST
 *
 * Exceptions:
 *    Aborts if h_flm_master_list is NULL
 *    Aborts if can't get master list object
 *    Aborts if can't unlock master list database
 *    flm_dispatch_nfys can abort
 *
 ------------------------------------------------------------------------*/
INT32 flm_add_to_master(
    const CHAR *ps_name
)
{
    INT32         i4_result;
    FLM_MASTER_T* pt_master;
    CRIT_STATE_T  t_old_level;

    FLM_ASSERT(h_flm_master_list, DBG_CAT_INV_OP, 1);

    if (ps_name == NULL)
    {
        return FLMR_INV_ARG;
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID **)&pt_master);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }
    
    /* add name to master list */
    i4_result = db_write_lock(pt_master->h_db);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    i4_result = db_write_rec(
                     pt_master->h_db,
                     (VOID *) &ps_name,
                     (VOID *) &ps_name,
                     DB_APPEND_RECORD
                  );
    if (i4_result != DBR_OK)
    {
        i4_result =  db_write_unlock(pt_master->h_db);
        return FLMR_CANT_CREATE_LIST;
    }
    i4_result =  db_write_unlock(pt_master->h_db);
    if (i4_result != DBR_OK)
    {
        FLM_ABORT(DBG_CAT_SEMAPHORE, 0);
    }

    /* call notification routines */
    flm_dispatch_nfys(FLM_UPDATED, FLM_RECORD_ADD);

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_del_from_master
 *
 * Description: Delete a name from the master list
 *
 * Inputs: -
 *    ps_name - name of favorites list to remove
 *
 * Side Effects - Deletes the name from the master list
 *
 * Returns: 
 *    FLMR_OK
 *    FLMR_INV_ARG
 *    FLMR_INV_HANDLE
 *    FLMR_REC_NOT_FOUND - Name of list is not in master list
 *
 * Exceptions:
 *    Aborts if h_flm_master_list is NULL
 *    Aborts if can't get master list object
 *    flm_dispatch_nfys can abort
 *
 ------------------------------------------------------------------------*/
INT32 flm_del_from_master(
    const CHAR *ps_name
)
{
    INT32         i4_result;
    DB_FIELD_T    t_search_field;
    FLM_MASTER_T* pt_master;
    CRIT_STATE_T  t_old_level;

    FLM_ASSERT(h_flm_master_list, DBG_CAT_INV_OP, 2);

    if (ps_name == NULL)
    {
        return FLMR_INV_ARG;
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID **)&pt_master);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }

    /* delete name from master list */
    i4_result = db_write_lock(pt_master->h_db);
    if (i4_result != DBR_OK)
    {
        return FLMR_INV_HANDLE;
    }

    /* delete it */
    t_search_field = (DB_FIELD_T) DB_MAKE_FIELD_TYPE(DBF_STRING_T, FLM_NAME_FIELD_T);
    i4_result = db_delete_rec(
                     pt_master->h_db,
                     1,
                     &t_search_field,
                     (VOID *) &ps_name,
                     0
                  );
    if (i4_result != DBR_OK)
    {
        db_write_unlock(pt_master->h_db);
        return db_to_flm_err(i4_result);
    }

    db_write_unlock(pt_master->h_db);

    /* call notification routines */
    flm_dispatch_nfys(FLM_UPDATED, FLM_RECORD_DEL);

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_get_check_list
 *
 * Description: This function makes sure the handle is of the proper
 *              type. If so, it returns a pointer to its data.
 *
 * All FLM APIs should call this if they are passed an FLM_HANDLE from the
 * outside world to make sure the handle is of the right type before any
 * FLM specific operations are carried out on it.
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 * Outputs:
 *    pt_list      - pointer to data for that favorites list
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_ARG
 *    FLMR_INV_HANDLE
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 flm_get_check_list
(
    HANDLE_T     h_fav_handle,
    FLM_LIST_T** pt_list
)
{
    INT32         i4_result;
    HANDLE_TYPE_T e_type;
    CRIT_STATE_T  t_old_level;

    t_old_level = x_crit_start();
    i4_result = handle_get_type_obj(h_fav_handle, &e_type, (VOID**)pt_list);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        if (i4_result == HR_INV_ARG)
        {
            return FLMR_INV_ARG;
        }
        else
        {
            return FLMR_INV_HANDLE;
        }
    }

    /* check handle type */
    if (e_type != FLM_HANDLE)
    {
#ifdef DEBUG
        FLM_ABORT(DBG_CAT_HANDLE, 1);
#endif
        return FLMR_INV_HANDLE;
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_free_callback
 *
 * Description: This function gets called when favorites handle
 *              is freed
 *
 * Inputs: -
 *     h_fav_handle - handle to favorites list
 *     e_type       - type of handle
 *     pv_obj       - data pointer for this handle
 *     pv_tag       - pointer to tag for this handle
 *     b_req_handle - whether this is a direct request to free the
 *                        handle.
 * Returns:
 *    TRUE if no errors
 *    FALSE if wrong type of handle
 *
 * Side Effects -
 *         The attached data structure is freed. The database object handle
 *         is freed.
 *
 * Exceptions:
 *         Aborts if handle is wrong type
 *
 ------------------------------------------------------------------------*/
BOOL flm_free_callback
(
    HANDLE_T      h_fav_handle,
    HANDLE_TYPE_T e_type,
    VOID*         pv_obj,
    VOID*         pv_tag,
    BOOL          b_req_handle
)
{
    FLM_LIST_T* pt_list;

    if (e_type != FLM_HANDLE)
    {
        FLM_ABORT(DBG_CAT_HANDLE, 2);
        return FALSE;
    }

    pt_list = (FLM_LIST_T *) pv_obj;
    if (pt_list != NULL)
    {
        if (pt_list->h_db != NULL_HANDLE)
        {
            x_handle_free(pt_list->h_db);
        }
        x_mem_free(pt_list);
    }
    return TRUE;
}

/*-------------------------------------------------------------------------
 * Name: flm_master_free_callback
 *
 * Description: This function gets called when favorites master list handle
 *              is freed
 *
 * Inputs: -
 *     h_fav_handle - handle to favorites master list
 *     e_type       - type of handle
 *     pv_obj       - data pointer for this handle
 *     pv_tag       - pointer to tag for this handle
 *     b_req_handle - whether this is a direct request to free the
 *                        handle.
 *
 * Side Effects -
 *         The attached data structure is freed. The database object handle
 *         is freed.
 *
 * Returns:
 *    TRUE if no errors
 *    FALSE if wrong type of handle or if b_req_handle is false
 *
 * Exceptions:
 *         Aborts if handle is wrong type
 *
 ------------------------------------------------------------------------*/
BOOL flm_master_free_callback
(
    HANDLE_T      h_fav_handle,
    HANDLE_TYPE_T e_type,
    VOID*         pv_obj,
    VOID*         pv_tag,
    BOOL          b_req_handle
)
{
    FLM_MASTER_T*        pt_master;
    FLM_MASTER_NOTIFY_T* pt_nfy;
    FLM_MASTER_NOTIFY_T* pt_next;

    if (e_type != FLM_MASTER_HANDLE)
    {
        FLM_ABORT(DBG_CAT_HANDLE, 3);
        return FALSE;
    }

    if (!b_req_handle)
    {
        return FALSE;
    }

    pt_master = (FLM_MASTER_T *) pv_obj;
    if (pt_master != NULL)
    {
        if (pt_master->h_db != NULL_HANDLE)
        {
            x_handle_free(pt_master->h_db);
        }
        
        /* free notification list */
        pt_nfy = pt_master->pt_nfy_list;
        while (pt_nfy)
        {
            /* save copy since x_handle_free deletes it */
            pt_next = pt_nfy->pt_next;

            x_handle_free(pt_nfy->h_notify);
            pt_nfy = pt_next;
        }

        x_mem_free(pt_master);
    }
    return TRUE;
}

/*-------------------------------------------------------------------------
 * Name: flm_free_notify_callback
 *
 * Description: This function gets called when favorites master list 
 *              notification handle is freed
 *
 * Inputs: -
 *         h_nfy_handle - handle for notification object
 *         e_type       - type of handle
 *         pv_obj       - data pointer for this handle
 *         pv_tag       - pointer to tag for this handle
 *         b_req_handle - whether this is a direct request to free the
 *                        handle.
 *
 * Side Effects -
 *         The attached data structure is unlinked from the master list
 *         object then freed.
 *
 * Returns:
 *    TRUE if no errors
 *    FALSE if wrong type of handle or if b_req_handle is false
 *
 * Exceptions:
 *         Aborts if handle is wrong type
 *
 ------------------------------------------------------------------------*/
BOOL flm_free_notify_callback
(
    HANDLE_T      h_nfy_handle,
    HANDLE_TYPE_T e_type,
    VOID*         pv_obj,
    VOID*         pv_tag,
    BOOL          b_req_handle
)
{
    FLM_MASTER_NOTIFY_T* pt_nfy;

    if (e_type != FLM_MASTER_NOTIFY_HANDLE)
    {
        FLM_ABORT(DBG_CAT_HANDLE, 4);
        return FALSE;
    }

    if (!b_req_handle)
    {
        return FALSE;
    }
    
    flm_del_notification(h_nfy_handle);

    pt_nfy = (FLM_MASTER_NOTIFY_T *) pv_obj;
    if (pt_nfy != NULL)
    {
        x_mem_free(pt_nfy);
    }
    return TRUE;
}

/*-------------------------------------------------------------------------
 * Name: flm_dispatch_nfys
 *
 * Description: This function calls all the notification functions
 *              with the given condition
 *
 * Inputs: -
 *         e_cond       - Condition of notification
 *         ui4_reason   - Reason of notification
 *
 * Returns:
 *    FLMR_OK
 *
 * Exceptions:
 *    Aborts if can't get master handle data
 *
 ------------------------------------------------------------------------*/
INT32 flm_dispatch_nfys(
    FLM_COND_T e_cond,
    UINT32     ui4_reason
)
{
    INT32                i4_result;
    FLM_MASTER_T*        pt_list;
    FLM_MASTER_NOTIFY_T* pt_notify;
    CRIT_STATE_T         t_old_level;


    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID**) &pt_list); 
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 24);
    }

    /* go through list calling all notification routines */
    pt_notify = pt_list->pt_nfy_list;
    while (pt_notify)
    {
        /* call the notification routine */
        if (pt_notify->pf_flm_notify != NULL)
        {
            pt_notify->pf_flm_notify(
                        pt_notify->h_notify,
                        e_cond,
                        ui4_reason,
                        pt_notify->pv_tag,
                        0
                        );

        }
        pt_notify = pt_notify->pt_next;
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_notify
 *
 * Description: This function gets called when a favorites list is locked
 *              and unlocked.
 *
 * Inputs: -
 *         h_db           - handle to the database object that caused the
 *                          notification
 *         e_cond         - condition of the notification
 *         ui4_reason     - reason for the notification
 *         pv_tag         - pointer to the FLM object belonging to this
 *                          database object
 *         ui4_db_version -
 *
 * Returns
 *    none
 *
 * Exceptions
 *    Aborts if could not get tag
 *
 ------------------------------------------------------------------------*/
VOID flm_notify
(
    HANDLE_T  h_db,
    DB_COND_T e_cond,
    UINT32    ui4_reason,
    VOID*     pv_tag,
    UINT32    ui4_db_version
)
{
    FLM_COND_T      e_flm_cond;
    UINT32          ui4_flm_reason;
    FLM_LIST_T*     pt_list;
    HANDLE_T        h_handle;
    INT32           i4_result;
    CRIT_STATE_T    t_old_level;
    pf_flm_nfy_func pf_nfy;

    if (pv_tag == NULL)
    {
        return;
    }

    pt_list = (FLM_LIST_T*) pv_tag;

    /* no notification function to call */
    if (pt_list->pf_flm_notify == NULL)
    {
        return;
    }

    /* Make sure this is the handle we think it is */
    if (pt_list->h_db != h_db)
    {
        return;
    }

    h_handle = pt_list->h_self;
    if (!x_handle_valid(h_handle))
    {
        return;
    }

    /* no reason to notify */
    if (ui4_reason == 0)
    {
        return;
    }

    t_old_level = x_crit_start();
    i4_result = x_handle_get_tag(h_handle, &pv_tag);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        /* should not get this if things are normal */
        FLM_ABORT(DBG_CAT_INV_OP, 13);
    }

    pf_nfy = pt_list->pf_flm_notify;

    if (e_cond == DB_CLOSED)
    {
        x_handle_free(h_handle);
    }

    /* translate DB reasons/conditions to FLM reasons/conditions */
    e_flm_cond = db_to_flm_condition(e_cond);
    ui4_flm_reason = db_to_flm_reason(ui4_reason);

    /* call client notification routine */
    if (pf_nfy)
    {
        pf_nfy(h_handle, e_flm_cond, ui4_flm_reason,
                            pv_tag, ui4_db_version);
    }
}

/*-------------------------------------------------------------------------
 * Name: flm_add_notification
 *
 * Description: Add a notification object to the master list's
 *              notification list.
 *
 * Inputs:
 *    pt_notify - pointer to notification object
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_ARG - pt_notify is NULL
 *    FLMR_INV_HANDLE - handle to master list is NULL
 *
 * Exceptions
 *    Aborts if could not get master list object
 *
 ------------------------------------------------------------------------*/
INT32 flm_add_notification
(
    FLM_MASTER_NOTIFY_T* pt_notify
)
{
    INT32         i4_result;
    FLM_MASTER_T* pt_master_list;
    CRIT_STATE_T  t_old_level;

    if (pt_notify == NULL)
    {
        return FLMR_INV_ARG;
    }

    if (h_flm_master_list == NULL_HANDLE)
    {
        return FLMR_INV_HANDLE;
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID*) &pt_master_list); 
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        /* should not get this if things are normal */
        FLM_ABORT(DBG_CAT_INV_OP, 14);
    }

    /* add this to the master list notification queue */
    t_old_level = x_crit_start();
    pt_notify->pt_next = pt_master_list->pt_nfy_list->pt_next;
    pt_master_list->pt_nfy_list->pt_next = pt_notify;
    x_crit_end(t_old_level);
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_del_notification
 *
 * Description: Delete a notification object from the master list's
 *              notification list.
 *
 * Inputs: -
 *    h_notify - handle identifying notification object to delete
 *               from master list's notification list.
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_ARG - pt_notify is NULL
 *    FLMR_INV_HANDLE - handle to master list is NULL
 *
 * Exceptions
 *    Aborts if can't get master list object
 *
 ------------------------------------------------------------------------*/
INT32 flm_del_notification
(
    HANDLE_T h_notify
)
{
    INT32                i4_result;
    FLM_MASTER_T*        pt_master_list;
    FLM_MASTER_NOTIFY_T* pt;
    CRIT_STATE_T         t_old_level;

    if (h_notify == NULL_HANDLE)
    {
        return FLMR_INV_ARG;
    }

    if (h_flm_master_list == NULL_HANDLE)
    {
        return FLMR_INV_HANDLE;
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID*) &pt_master_list); 
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        /* should not get this if things are normal */
        FLM_ABORT(DBG_CAT_INV_OP, 15);
    }

    if (pt_master_list->pt_nfy_list == NULL)
    {
        /* nothing to delete */
        return FLMR_OK;
    }
    
    /* delete this handle from the master list notification queue */
    t_old_level = x_crit_start();
    for(pt = pt_master_list->pt_nfy_list; pt; pt = pt->pt_next)
    {
        if (pt->pt_next->h_notify == h_notify)
        {
            pt->pt_next = pt->pt_next->pt_next;
        }
    }
    x_crit_end(t_old_level);

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: fsraw_to_flm_err
 *
 * Description: translates a raw file system error to a favorites list error
 *
 * Inputs: -
 *         i4_fsraw_error - Raw File system error
 *
 * Returns: FLM error
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 fsraw_to_flm_err
(
    INT32 i4_fsraw_error
)
{
    INT32 i4_flm_error;

    switch(i4_fsraw_error)
    {
    case FSRAWR_OK:
        i4_flm_error = FLMR_OK;
        break;
    case FSRAWR_OUT_OF_SPACE:
        i4_flm_error = FLMR_FILESYS_FULL;
        break;
    case FSRAWR_NOT_FOUND:
        i4_flm_error = FLMR_FILE_NOT_FOUND;
        break;
    case FSRAWR_INV_ARG:
        i4_flm_error = FLMR_INV_ARG;
        break;
    default:
        i4_flm_error = FLMR_UNKNOWN;
        break;
    }
    return i4_flm_error;
}

/*-------------------------------------------------------------------------
 * Name: fm_to_flm_err
 *
 * Description: translates a file manager error to a favorites list error
 *
 * Inputs: -
 *         i4_fm_error - File Manager error
 *
 * Returns: FLM error
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 fm_to_flm_err
(
    INT32 i4_fm_error
)
{
    INT32 i4_flm_error;

    switch(i4_fm_error)
    {
    case FMR_OK:
        i4_flm_error = FLMR_OK;
        break;
    case FMR_ARG:
    case FMR_INVAL:
        i4_flm_error = FLMR_INV_ARG;
        break;
    case FMR_HANDLE:
        i4_flm_error = FLMR_INV_HANDLE;
        break;
    case FMR_DEVICE_ERROR:
        i4_flm_error = FLMR_DEV_ERR;
        break;
    case FMR_CORE:
        i4_flm_error = FLMR_OS_ERR;
        break;
    case FMR_PERM_DENY:
        i4_flm_error = FLMR_PERM_DENY;
        break;
    case FMR_EOF:
        i4_flm_error = FLMR_EOF;
        break;
    case FMR_FILE_SYSTEM_FULL:
        i4_flm_error = FLMR_FILESYS_FULL;
        break;
    case FMR_NOT_DIR:
    case FMR_NO_ENTRY:
    case FMR_NAME_TOO_LONG:
    case FMR_NO_SUCH_DEVICE:
        i4_flm_error = FLMR_INV_FILE_PATH;
        break;
    default:
        i4_flm_error = FLMR_UNKNOWN;
        break;
    }
    return i4_flm_error;
}

/*-------------------------------------------------------------------------
 * Name: db_to_flm_err
 *
 * Description: translates a database error to a favorites list error
 *
 * Inputs: -
 *         i4_db_error - Database error
 *
 * Returns: FLM error
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 db_to_flm_err
(
    INT32 i4_db_error
)
{
    INT32 i4_flm_error;

    switch(i4_db_error)
    {
    case DBR_OK:
        i4_flm_error = FLMR_OK;
        break;
    case DBR_INV_ARG:
        i4_flm_error = FLMR_INV_ARG;
        break;
    case DBR_INV_HANDLE:
        i4_flm_error = FLMR_INV_HANDLE;
        break;
    case DBR_OUT_OF_MEMORY:
        i4_flm_error = FLMR_OUT_OF_MEM;
        break;
    case DBR_OUT_OF_HANDLE:
        i4_flm_error = FLMR_OUT_OF_HANDLE;
        break;
    case DBR_NO_WRITE_LOCK:
        i4_flm_error = FLMR_NO_WRITE_LOCK;
        break;
    case DBR_NOT_FOUND:
        i4_flm_error = FLMR_LIST_NOT_FOUND;
        break;
    case DBR_REC_NOT_FOUND:
        i4_flm_error = FLMR_REC_NOT_FOUND;
        break;
    case DBR_WRITE_LOCKED:
        i4_flm_error = FLMR_WRITE_LOCKED;
        break;
    case DBR_END_OF_REC:
        i4_flm_error = FLMR_END_OF_ITERATION;
        break;
    case DBR_MODIFIED:
        i4_flm_error = FLMR_MODIFIED;
        break;
    case DBR_DB_LIMIT_EXCEED:
        i4_flm_error = FLMR_LIMIT_EXCEED;
        break;
    case DBR_INV_NAME:
        i4_flm_error = FLMR_INV_NAME;
        break;
    case DBR_FILE_READ_FAIL:
        i4_flm_error = FLMR_FILE_READ_FAIL;
        break;
    case DBR_FILE_WRITE_FAIL:
        i4_flm_error = FLMR_FILE_WRITE_FAIL;
        break;
    default:
        i4_flm_error = FLMR_UNKNOWN;
        break;
    }
    return i4_flm_error;
}

/*-------------------------------------------------------------------------
 * Name: db_to_flm_condition
 *
 * Description: translates a database notify condition to a favorites
 *              list notify condition
 *
 * Inputs: -
 *         e_db_cond - Database condition
 *
 * Returns: FLM condition
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
FLM_COND_T db_to_flm_condition
(
    DB_COND_T e_db_cond
)
{
    switch(e_db_cond)
    {
    case DB_UPDATING:
        return FLM_UPDATING;
    case DB_UPDATED:
        return FLM_UPDATED;
    case DB_CLOSED:
        return FLM_CLOSED;
    default:
        return FLM_NO_COND;
    }
}

/*-------------------------------------------------------------------------
 * Name: db_to_flm_reason
 *
 * Description: translates a database notify reason to a favorites
 *              list notify reason
 *
 * Inputs:
 *         ui4_db_reason - Database reason
 *
 * Returns: FLM reason
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
UINT32 db_to_flm_reason
(
    UINT32 ui4_db_reason
)
{
    UINT32 ui4_flm_reason = 0;

    if (ui4_db_reason & DB_REASON_UNK)
    {
        ui4_flm_reason |= FLM_REASON_UNK;
    }
    if (ui4_db_reason & DB_RECORD_ADD)
    {
        ui4_flm_reason |= FLM_RECORD_ADD;
    }
    if (ui4_db_reason & DB_RECORD_DEL)
    {
        ui4_flm_reason |= FLM_RECORD_DEL;
    }
    if (ui4_db_reason & DB_RECORD_MOD)
    {
        ui4_flm_reason |= FLM_RECORD_MOD;
    }
    return ui4_flm_reason;
}

/*-------------------------------------------------------------------------
 * Name: flm_move_up
 *
 * Description: moves entries in a favorites list up
 *
 * Inputs: -
 *         h_db         - handle to favorites list database object
 *         ui2_position - position to start moving
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_HANDLE
 *    FLMR_INV_ARG
 *    FLMR_FAIL
 *
 * Exceptions:
 *    Aborts if no write lock or wrong thread is writing.
 *
 ------------------------------------------------------------------------*/
INT32 flm_move_up(
    HANDLE_T h_db,
    UINT16   ui2_position
)
{
    INT32      i4_result;
    DB_FIELD_T t_index_field;
    UINT16     ui2_i;
    UINT16*    pt_index;
    UINT16     ui2_new_index;
    VOID*      apv_keyfield_addr[1];

   /* move everything after it up */
    t_index_field = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    pt_index = &ui2_new_index;

    apv_keyfield_addr[0] = &ui2_i;

    for (ui2_i = (UINT16)(ui2_position+1); ;++ui2_i)
    {
        ui2_new_index = (UINT16) (ui2_i - 1);
        i4_result = db_update_rec(
               h_db,
               (VOID *) &apv_keyfield_addr,
               1,
               &t_index_field,
               (VOID *) &pt_index
             );

        if (i4_result == DBR_REC_NOT_FOUND)
        {
            break;
        }

        if (i4_result != DBR_OK)
        {
            return db_to_flm_err(i4_result);
        }
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_move_down
 *
 * Description: moves entries in a favorites list down
 *
 * Inputs: -
 *         h_db         - handle to favorites list database object
 *         ui2_position - position to start moving
 *         ui2_last     - starting last position of list
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_HANDLE
 *    FLMR_INV_ARG
 *    FLMR_FAIL
 *
 * Exceptions:
 *    Aborts if no write lock or wrong thread is writing.
 *
 ------------------------------------------------------------------------*/
INT32 flm_move_down(
    HANDLE_T h_db,
    UINT16   ui2_position,
    UINT16   ui2_last
)
{
    INT32      i4_result;
    DB_FIELD_T t_index_field;
    UINT16     ui2_i;
    UINT16*    pt_index;
    UINT16     ui2_new_index;
    VOID*      apv_keyfield_addr[1];

   /* move any other records down to make room */
    t_index_field = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    pt_index = &ui2_new_index;
    apv_keyfield_addr[0] = &ui2_i;

    for (ui2_i = (UINT16)(ui2_last-1); ui2_i >= ui2_position; --ui2_i)
    {
        ui2_new_index = (UINT16)(ui2_i + 1);

        if (ui2_new_index == 0)
        {
           break;
        }

        i4_result = db_update_rec(
                      h_db,
                      (VOID *) apv_keyfield_addr,
                      1,
                      &t_index_field,
                      (VOID *) &pt_index
                    );
        if (i4_result == DBR_REC_NOT_FOUND)
        {
            break;
        }
        if (i4_result != DBR_OK)
        {
            return db_to_flm_err(i4_result);
        }
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_alloc_handle
 *
 * Description: allocates a handle to a favorites list
 *
 * Inputs:
 *         pv_tag       - allowed to be NULL?
 *
 * Outputs:
 *         ph_list      - handle to favorites list
 *         ppt_list     - pointer to pointer to list object data
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *    FLMR_INV_ARG       - ppt_list or ph_list are NULL
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 flm_alloc_handle(
    HANDLE_T*    ph_list,
    VOID*        pv_tag,
    FLM_LIST_T** ppt_list
)
{
    INT32 i4_result;

    if (ppt_list == NULL || ph_list == NULL )
    {
        return FLMR_INV_ARG;
    }

    *ppt_list = x_mem_calloc(1, sizeof(FLM_LIST_T));
    if (*ppt_list == NULL)
    {
        return FLMR_OUT_OF_MEM;
    }

    i4_result = handle_alloc(FLM_HANDLE,
                             (VOID*) *ppt_list,
                             pv_tag,
                             flm_free_callback,
                             ph_list);
    if (i4_result != HR_OK)
    {
        x_mem_free(*ppt_list);
        *ph_list = NULL_HANDLE;
        return FLMR_OUT_OF_HANDLE;
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_signature_good
 *
 * Description: Checks that the given signature is correct for an FLM file.
 *
 * Inputs:
 *    pac_signature - pointer to a 4 character array
 *
 * Returns:
 *    TRUE  - signature is correct for an FLM file
 *    FALSE - file is not an FLM file.
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
BOOL flm_signature_good(
    CHAR* pac_signature
)
{
    if (pac_signature[0] == 'F' &&
        pac_signature[1] == 'L' &&
        pac_signature[2] == 'M' &&
        pac_signature[3] == '1')
    {
        return TRUE;
    }
    return FALSE;
}

/*-------------------------------------------------------------------------
 * Name: flm_read_listname
 *
 * Description: Reads a list name into the provided buffer.
 *              Assumes the file pointer is at the correct place.
 *
 * Inputs:
 *    h_file - Handle to the file to read from
 *
 * Outputs:
 *    ps_listname - pointer to the buffer to read the name into.
 *
 * Returns:
 *   FLMR_DEV_ERR
 *   FLMR_OS_ERR
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 flm_read_listname(
    HANDLE_T h_file,
    CHAR*    ps_listname
)
{
    INT32  i4_result;
    UINT32 ui4_bytes_read;

    i4_result = x_fm_read(
                      h_file,
                      ps_listname,
                      FLM_NAME_LEN,
                      &ui4_bytes_read
                   );
    return fm_to_flm_err(i4_result);
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_open_read_lock
 *
 * Description: Opens a raw device and gets a read lock on it.
 *
 * Inputs:
 *    h_dir       - Handle of directory
 *    ps_pathname - Path to device
 *
 * Outputs:
 *    ph_file - pointer to variable to hold the handle
 *
 * Returns:
 *
 * Exceptions:
 *    Aborts if it can't get read lock
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_open_read_lock(
    HANDLE_T       h_dir,
    CHAR*          ps_pathname,
    HANDLE_T*      ph_file
)
{
    INT32 i4_result;

    if (!fs_raw_is_inited(h_dir, ps_pathname))
    {
        return FLMR_INV_FILE_PATH;
    }

    i4_result = x_fm_open(
                   h_dir,
                   ps_pathname,
                   FM_READ_ONLY,
                   0644,
                   FALSE,
                   ph_file
                 );
    if (i4_result != FMR_OK)
    {
        return fm_to_flm_err(i4_result);
    }

    /* get read lock on file */
    fs_raw_read_lock(*ph_file);

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_open_write_lock
 *
 * Description: Opens a raw device and gets a write lock on it.
 *
 * Inputs:
 *    h_dir       - Handle of directory
 *    ps_pathname - Path to device
 *
 * Outputs:
 *    ph_file - pointer to variable to hold the handle
 *
 * Returns:
 *
 * Exceptions:
 *    Aborts if it can't get write lock
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_open_write_lock(
    HANDLE_T       h_dir,
    CHAR*          ps_pathname,
    HANDLE_T*      ph_file
)
{
    INT32 i4_result;

    if (!fs_raw_is_inited(h_dir, ps_pathname))
    {
        i4_result = fs_raw_init(h_dir, ps_pathname);
        if (i4_result)
        {
            return fsraw_to_flm_err(i4_result);
        }
    }

    i4_result = x_fm_open(
                   h_dir,
                   ps_pathname,
                   FM_READ_WRITE,
                   0664,
                   FALSE,
                   ph_file
                 );
    if (i4_result != FMR_OK)
    {
        return fm_to_flm_err(i4_result);
    }

    /* get write lock on file */
    fs_raw_write_lock(*ph_file);

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_unlock_close
 *
 * Description: Releases any raw fs lock and closes the file.
 *
 * Inputs:
 *    h_file  - Handle to file
 *
 * Outputs:
 *    None
 *
 * Returns:
 *
 * Exceptions
 *    Aborts if unlock could not be done
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_unlock_close(
    HANDLE_T h_file
)
{
    INT32 i4_result;

    fs_raw_unlock(h_file);
    i4_result = x_fm_close(h_file);
    return fm_to_flm_err(i4_result);
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_del
 *
 * Description: 
 *
 * Inputs:
 *    h_dir       - Handle of directory
 *    ps_pathname - Path to device
 *    ui4_id      - Raw FS ID of object to delete
 *
 * Outputs:
 *    None
 *
 * Returns:
 *
 * Exceptions
 *    Aborts if can't get raw fs write lock on device.
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_del(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    UINT32   ui4_id
)
{
    INT32           i4_result;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;

    i4_result = flm_raw_open_write_lock(h_dir, ps_pathname, 
                                 &h_file);
    if (i4_result)
    {
        return i4_result;
    }

    i4_result = fs_raw_del_obj(h_file, &t_meta, ui4_id);

    flm_raw_unlock_close(h_file);

    return fsraw_to_flm_err(i4_result);
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_fs_qry
 *
 * Description: This API queries
 *
 * Inputs:  -
 *    h_dir       - handle to directory to query
 *    ps_pathname - name of file to query
 *    ui2_index   - index (0=first, 1=second,...)
 *
 * Outputs: -
 *    ps_listname - name of list to query about
 *    pui4_id     - pointer to object ID for raw file
 *
 * Returns:
 *
 * Exceptions:
 *    Aborts if can't get raw fs write lock on device.
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_fs_qry(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    UINT16   ui2_index,
    CHAR*    ps_listname,
    UINT32*  pui4_id
)
{
    INT32         i4_result;
    UINT16        ui2_i;
    UINT16        ui2_module;
    UINT32        ui4_offset;
    UINT32        ui4_sz;
    FS_RAW_META_T t_meta;
    HANDLE_T      h_file;

    i4_result = flm_raw_open_read_lock(h_dir, ps_pathname, 
                                 &h_file);
    if (i4_result)
    {
        return i4_result;
    }

    for (ui2_i = 0; ui2_i < FSRAW_MAX_NUM_OBJ; ++ui2_i)
    {
        i4_result = fs_raw_seek_obj_by_idx(
                       h_file,
                       &t_meta,
                       ui2_i,
                       pui4_id,
                       &ui4_offset,
                       &ui4_sz
                 );
        if (i4_result)
        {
            flm_raw_unlock_close(h_file);
            return FLMR_LIST_NOT_FOUND;
        }
        ui2_module = (UINT16) FS_RAW_GET_MODULE_ID(*pui4_id);
        if (ui2_module == FLM_GROUP_ID)
        {
            if (ui2_index == 0)
            {
                /* grab listname */
                i4_result = flm_read_listname(h_file, ps_listname);

                flm_raw_unlock_close(h_file);
                return i4_result;
            }
            --ui2_index;
        }
    }
    flm_raw_unlock_close(h_file);
    return FLMR_LIST_NOT_FOUND;
}

/*-------------------------------------------------------------------------
 * Name: flm_find_id
 *
 * Description: Searches the file for an existing or unused raw fs object ID
 *
 * Inputs:
 *    h_file      - handle of device
 *    pt_listname - pointer to name of list
 *    pt_meta     - pointer to raw fs metadata structure
 *
 * Outputs:
 *    pui4_id     - pointer to variable to hold ID, if any
 *    pui4_offset - pointer to variable to hold offset, if any
 *
 * Returns:
 *    FLMR_OK           - A free ID was found
 *    FLMR_FILESYS_FULL - No free ID found
 *
 * Exceptions
 *
 ------------------------------------------------------------------------*/
INT32 flm_find_id(
    HANDLE_T       h_file,
    CHAR*          ps_listname,
    FS_RAW_META_T* pt_meta,
    UINT32*        pui4_id,
    UINT32*        pui4_offset
)
{
    INT32  i4_result;
    UINT16 ui2_i;
    UINT32 ui4_id;
    UINT32 ui4_sz;

    for (ui2_i = 1; ui2_i < 256; ++ui2_i)
    {
        /* if object doesn't exist, use it */
        ui4_id = (UINT32) FS_RAW_MAKE_OBJ_ID(FLM_GROUP_ID, ui2_i);
        i4_result = fs_raw_seek_obj(h_file, pt_meta, ui4_id, pui4_offset, &ui4_sz);
        if (i4_result == FSRAWR_NOT_FOUND)
        {
            *pui4_id = ui4_id;
            return FLMR_OK;
        }
        else if (i4_result == FLMR_OK)
        {
            CHAR s_tmp[FLM_NAME_LEN+1];

            i4_result = flm_read_listname(h_file, s_tmp);
            if (i4_result)
            {
                return i4_result;
            }

            if (x_strncmp(ps_listname, s_tmp, FLM_NAME_LEN) == 0)
            {
                *pui4_id = ui4_id;
                return FLMR_OK;
            }
        }
        else
            return fsraw_to_flm_err(i4_result);
    }
    return FLMR_FILESYS_FULL;
}

/*-------------------------------------------------------------------------
 * Name: flm_store_raw_list
 *
 * Description: This API stores the specified favorites list to a
 *              raw device.
 *
 * Inputs:  -
 *    h_dir       - handle of directory to store to
 *    ps_filename - name of device file to store to
 *    h_list      - handle of list to store
 *
 * Returns:
 *    FLMR_CANT_CREATE_FILE
 *    FLMR_INV_FILE_PATH
 *    FLMR_INV_HANDLE
 *    FLMR_INV_ARG - ps_filename is NULL
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_PERM_DENY
 *    FLMR_REC_NOT_FOUND
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *    FLMR_LIST_NOT_FOUND
 *
 * Exception:
 *    Aborts if can't get list name
 *    Aborts if can't open list
 *    Aborts if can't unlock files
 *    Aborts if can't close files
 *    Aborts if can't create semaphore
 *    Aborts if raw device error
 *    Aborts if no memory for raw device header
 *    Aborts if h_flm_master_list is NULL
 *    Aborts if can't get master object
 *    Aborts if can't get master object info
 *
 ------------------------------------------------------------------------*/
INT32 flm_store_raw_list(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    HANDLE_T h_list
)
{
    INT32         i4_result;
    FS_RAW_META_T t_meta;
    HANDLE_T      h_file;
    HANDLE_T      h_db;
    UINT32        ui4_id;
    UINT32        ui4_offset;
    UINT32        ui4_size;
    UINT32        ui4_written;
    UINT64        ui8_position;
    FLM_LIST_T*   pt_list;
    UINT32        ui4_persistent_size;
    UINT32        ui4_ver_id;


    i4_result = handle_get_obj(h_list, (VOID*) &pt_list);
    h_db = pt_list->h_db;

    i4_result = flm_raw_open_write_lock(h_dir, ps_pathname, 
                                 &h_file);
    if (i4_result)
    {
        return i4_result;
    }

    /* find an ID that works */
    i4_result = flm_find_id(h_file, pt_list->as_name, &t_meta, &ui4_id, &ui4_offset);
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return i4_result;
    }

    /* calc size */
    i4_result = db_get_persistent_size(h_db,
                                       (db_endian_conv_fct)NULL,
                                       &ui4_persistent_size,
                                       &ui4_ver_id);
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return db_to_flm_err(i4_result);
    }

    ui4_size = (FLM_NAME_LEN) + ui4_persistent_size;

    /* allocate */
    i4_result = fs_raw_realloc(h_file, 
                               ui4_id,
                               ui4_size,
                               &t_meta,
                               &ui4_offset
                              );
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return fsraw_to_flm_err(i4_result);
    }

    /* seek */
    i4_result = x_fm_lseek(h_file,
                           (INT64)ui4_offset,
                           FM_SEEK_BGN,
                           &ui8_position);
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return fsraw_to_flm_err(i4_result);
    }

    /* write name */
    i4_result = x_fm_write(h_file, pt_list->as_name, FLM_NAME_LEN, &ui4_written);
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return fm_to_flm_err(i4_result);
    }

    /* write records */
    i4_result = db_save_to_file(h_db, h_file, NULL, NULL);
    flm_raw_unlock_close(h_file);

    return db_to_flm_err(i4_result);
}

/*-------------------------------------------------------------------------
 * Name: flm_raw_seek_by_name
 *
 * Description: Moves the seek point of the file past the name
 *              that matches ps_listname, if any
 *
 * Inputs:
 *    h_file      - Handle of device to search
 *    ps_listname - pointer to name of list to search for
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_LIST_NOT_FOUND - list was not found on device
 *
 * Exceptions
 *
 ------------------------------------------------------------------------*/
INT32 flm_raw_seek_by_name(
    HANDLE_T h_file,
    CHAR*    ps_listname
)
{
    INT32         i4_result;
    UINT16        ui2_i;
    UINT32        ui4_id;
    UINT32        ui4_offset;
    UINT32        ui4_sz;
    UINT32        ui4_read;
    FS_RAW_META_T t_meta;
    CHAR          s_name[FLM_NAME_LEN+1];

    for (ui2_i = 1; ui2_i < 256; ++ui2_i)
    {
        ui4_id = (UINT32) FS_RAW_MAKE_OBJ_ID(FLM_GROUP_ID, ui2_i);
        i4_result = fs_raw_seek_obj(h_file, &t_meta, ui4_id, &ui4_offset, &ui4_sz);
        if (i4_result)
        {
            return FLMR_LIST_NOT_FOUND;
        }

        /* read in listname */
        i4_result = x_fm_read(
                         h_file,
                         &s_name,
                         FLM_NAME_LEN,
                         &ui4_read
                      );
        if (i4_result)
        {
            return fm_to_flm_err(i4_result);
        }

        if (x_strncmp(s_name, ps_listname, FLM_NAME_LEN) == 0)
        {
            return FLMR_OK;
        }
    }

    return FLMR_LIST_NOT_FOUND;
}

/*-------------------------------------------------------------------------
 * Name: flm_load_raw_list
 *
 * Description: This API loads the specified favorites lists from
 *              raw device
 *
 * Inputs:
 *    h_dir         - handle of directory to load from
 *    ps_filename   - pointer to name of device to load lists from
 *    ps_listname   - pointer to name of list to load
 *    pv_tag        - pointer to tag
 *    pf_flm_notify - pointer to notification function
 *
 * Outputs:
 *    ph_list       - pointer to variable to hold handle to list loaded
 *
 * Returns:
 *    FLMR_INV_ARG         - h_dir is not a valid handle
 *    FLMR_CANT_OPEN_FILE
 *    FLMR_INV_FILE        - file is not FLM file
 *    FLMR_INV_FILE_PATH
 *    FLMR_PERM_DENY       - don't have permission to access file
 *    FLMR_OUT_OF_MEM    - out of memory
 *    FLMR_OUT_OF_HANDLE - out of handles
 *    FLMR_EOF
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_LIMIT_EXCEED    - too many lists or too many records in
 *                           a list
 *    FLMR_REC_NOT_FOUND   - corrupted list during move down
 *
 * Exceptions:
 *   Aborts on close error
 *   Aborts on unlock error
 *   Aborts if can't create raw read lock
 *   Aborts if can't unlock raw read lock
 *
 ------------------------------------------------------------------------*/
INT32 flm_load_raw_list(
    HANDLE_T        h_dir,
    CHAR*           ps_pathname,
    CHAR*           ps_listname,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_list
)
{
    INT32           i4_result;
    HANDLE_T        h_file;
    DB_FIELD_INFO_T at_field_info[2];
    FLM_LIST_T*     pt_list;


    i4_result = flm_raw_open_read_lock(h_dir, ps_pathname, 
                                 &h_file);
    if (i4_result)
    {
        return i4_result;
    }

    /* search for ps_listname */
    i4_result = flm_raw_seek_by_name(h_file, ps_listname);
    if (i4_result)
    {
        flm_raw_unlock_close(h_file);
        return i4_result;
    }

    /* allocate new list handle, object */
    i4_result = flm_alloc_handle(ph_list, pv_tag, &pt_list);

    /* load favorites list */
    at_field_info[0].ui4_field_type = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    at_field_info[0].ui2_field_element_count = 1;
    at_field_info[1].ui4_field_type = (DB_FIELD_T) FLM_DATA_FIELD_TYPE;
    at_field_info[1].ui2_field_element_count = 1;

    i4_result = db_load_from_file(ps_listname, 2, at_field_info,
                                1, &at_field_info[0].ui4_field_type,
                                h_file, NULL, NULL,
                                (VOID*) &pt_list,
                                flm_notify,
                                &(pt_list->h_db));

    flm_raw_unlock_close(h_file);

    if (i4_result)
    {
        x_handle_free(*ph_list);
        return db_to_flm_err(i4_result);
    }
    
    flm_add_to_master(ps_listname);

    pt_list->pf_flm_notify = pf_flm_notify;
    pt_list->h_self = *ph_list;
    x_strcpy(pt_list->as_name, ps_listname);
    return FLMR_OK;
}

#ifdef CLI_LVL_ALL
/*---------------------------------------------------------------------
 * Name: flm_get_dbg_level
 *
 * Description: 
 *
 * Inputs: -
 *    argc     - number of args
 *    pps_argv - pointer to array of args
 *
 * Returns: -
 *    FLMR_OK
 *
 --------------------------------------------------------------------*/
INT32 flm_get_dbg_level(
    INT32 i4_argc,
    const CHAR** pps_argv
    )
{
    INT32 i4_result;

    i4_result = x_cli_show_dbg_level(ui2_flm_debug_level);
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: flm_set_dbg_level
 *
 * Description: 
 *
 * Inputs: -
 *    argc     - number of args
 *    pps_argv - pointer to array of args
 *
 * Returns: -
 *    FLMR_OK
 *
 --------------------------------------------------------------------*/
INT32 flm_set_dbg_level(
    INT32 i4_argc,
    const CHAR** pps_argv
    )
{
    INT32  i4_result;
    UINT16 ui2_i;

    i4_result = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_i);
    if (i4_result == CLIR_OK)
    {
        ui2_flm_debug_level = ui2_i;
    }
    return FLMR_OK;
}
#endif

