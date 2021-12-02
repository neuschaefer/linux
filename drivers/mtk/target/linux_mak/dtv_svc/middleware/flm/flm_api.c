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
 * $RCSfile: flm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains all Favorites List Manager (FLM)
 *         function implementations.
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
                    include files
 ------------------------------------------------------------------------*/

#include "flm.h"

/*-------------------------------------------------------------------------
 * Name: x_flm_create
 *
 * Description: This API creates a favorites list
 *
 * Inputs: 
 *    ps_name        - name of favorites list
 *    pv_tag         - Pointer to tag
 *    pf_notify_func - Pointer to notification function
 *
 * Outputs:
 *    ph_fav_handle - handle of favorites list
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_ARG       - ps_name or ph_fav_handle are NULL or
 *                         strlen(ps_name) is too long.
 *    FLMR_OUT_OF_MEM    - out of memory
 *    FLMR_OUT_OF_HANDLE - out of handles
 *    FLMR_LIMIT_EXCEED  - Too many favorites lists.
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/

INT32 x_flm_create
(
    CHAR*            ps_name,
    VOID*            pv_tag,
    pf_flm_nfy_func  pf_flm_notify,
    HANDLE_T*        ph_fav_handle
)
{
    INT32           i4_result;
    UINT16          ui2_numlists;
    DB_FIELD_INFO_T at_field_info[2];
    FLM_LIST_T*     pt_list;

    DBG_API(("{FLM} x_flm_create: ps_name %s pv_tag %d pf_flm_notify 0x%x ph_fav_handle 0x%x\n",
             ps_name, pv_tag, pf_flm_notify, ph_fav_handle));

    /* perform sanity checks on arguments */
    if (ps_name == NULL || ph_fav_handle == NULL)
    {
        return FLMR_INV_ARG;
    }

    if (x_strlen(ps_name) >= FLM_NAME_LEN)
    {
        return FLMR_INV_ARG;
    }

    i4_result = x_flm_get_num_lists(&ui2_numlists);
    if (ui2_numlists > x_flm_get_max_lists())
    {
        return FLMR_LIMIT_EXCEED;
    }
    
    i4_result = flm_alloc_handle(ph_fav_handle, pv_tag, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    at_field_info[0].ui4_field_type = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    at_field_info[0].ui2_field_element_count = 1;
    at_field_info[1].ui4_field_type = (DB_FIELD_T) FLM_DATA_FIELD_TYPE;
    at_field_info[1].ui2_field_element_count = 1;

    i4_result = db_create(
                  x_flm_get_max_size(),
                  DB_MEM_STATIC,
                  ps_name,
                  2,
                  at_field_info,
                  1,
                  &at_field_info[0].ui4_field_type,
                  (VOID*) pt_list,
                  flm_notify,
                  &(pt_list->h_db)
                );

    if (i4_result != DBR_OK)
    {
        x_handle_free(*ph_fav_handle);
        *ph_fav_handle = NULL_HANDLE;
        return db_to_flm_err(i4_result);
    }

    i4_result = flm_add_to_master(ps_name);
    if (i4_result != FLMR_OK)
    {
        /* delete database */
        db_delete(pt_list->h_db, ps_name);
        pt_list->h_db = NULL_HANDLE;
        x_handle_free(*ph_fav_handle);
        return i4_result;
    }
    pt_list->pf_flm_notify = pf_flm_notify;
    pt_list->h_self = *ph_fav_handle;
    x_strcpy(pt_list->as_name, ps_name);
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_delete
 *
 * Description: This API deletes a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *
 * Outputs: -
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - ps_name is NULL
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_delete
(
    HANDLE_T h_fav_handle
)
{
    INT32       i4_result;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_delete: h_fav_handle %d\n", h_fav_handle));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    i4_result = db_delete(pt_list->h_db, pt_list->as_name);
    pt_list->h_db = NULL_HANDLE;
    if (i4_result != DBR_OK)
    {
        if (i4_result == DBR_NOT_FOUND)
        {
            /* if it wasn't found, then it's already gone! Success! */
            x_handle_free(h_fav_handle);
            return FLMR_OK;
        }
        return db_to_flm_err(i4_result);
    }
    i4_result = flm_del_from_master(pt_list->as_name);

    x_handle_free(h_fav_handle);

    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_max_lists
 *
 * Description: Gets the maximum number of favorites lists allowed
 *
 * Returns: Maximum number of favorites lists allowed.
 *
 * Exceptions: None
 ------------------------------------------------------------------------*/
UINT16 x_flm_get_max_lists(VOID)
{
    DBG_API(("{FLM} x_flm_get_max_lists\n"));

    return ui2_max_num_fav_lists;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_max_size
 *
 * Description: Gets the maximum number of entries allowed in a favorites list
 *
 * Returns: Maximum number of entries allowed in a favorites list
 *
 * Exceptions: None
 ------------------------------------------------------------------------*/
UINT16 x_flm_get_max_size(VOID)
{
    DBG_API(("{FLM} x_flm_get_max_size\n"));

    return ui2_max_fav_size;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_num_lists
 *
 * Description: This API gets the number of existing favorites lists
 *
 * Inputs:  -
 *
 * Outputs: -
 *    pui2_numlists - pointer to variable to hold number
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - pui2_numlists is NULL
 *
 * Exceptions
 *    Aborts on any other error.
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_num_lists(
    UINT16* pui2_numlists
)
{
    INT32         i4_result;
    DB_INFO_T     db_info;
    HANDLE_T      h_flm_master_list;
    FLM_MASTER_T* pt_master;
    CRIT_STATE_T  t_old_level;

    DBG_API(("{FLM} x_flm_get_num_lists: pui2_numlists 0x%x\n", pui2_numlists));

    if (pui2_numlists == NULL)
    {
        return FLMR_INV_ARG;
    }

    /* open master list */
    i4_result = flm_get_master_list(&h_flm_master_list);
    if (i4_result != FLMR_OK)
    {
        FLM_ABORT(DBG_CAT_HANDLE, 0);
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID **) &pt_master);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_HANDLE, 1);
    }

    /* count number of records in master list */
    i4_result = db_get_info(pt_master->h_db, &db_info);
    if (i4_result != DBR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }
    *pui2_numlists = (UINT16) db_info.ui4_record_count;

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_name_len
 *
 * Description: This API gets the length of the name of the specified
 *              favorites list
 *
 * Inputs:  -
 *    ui2_listnum  - list number
 *
 * Outputs: -
 *    pt_length - pointer to variable to hold name length
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ui2_listnum or pui1_length is not valid
 *    FLMR_REC_NOT_FOUND - no such list
 *
 * Exceptions
 *    Aborts if h_flm_master_list is NULL
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_name_len(
    UINT16  ui2_listnum,
    SIZE_T* pz_length
)
{
    INT32         i4_result;
    UINT32        ui4_db_version;
    DB_FIELD_T    t_field;
    CHAR          s_name[FLM_NAME_LEN+1];
    HANDLE_T      h_flm_master_list;
    FLM_MASTER_T* pt_master;
    CRIT_STATE_T  t_old_level;
    const VOID*   apv_field_value_addr[1];

    DBG_API(("{FLM} x_flm_get_name_len: ui2_listnum %d pz_length 0x%x\n", ui2_listnum, pz_length));

    if (ui2_listnum > x_flm_get_max_lists() || pz_length == NULL)
    {
        return FLMR_INV_ARG;
    }

    /* default just in case of error */
    s_name[0] = '\0';

    /* look name up in master list */
    i4_result = flm_get_master_list(&h_flm_master_list);
    if (i4_result != FLMR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID **) &pt_master);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }

    /* set position to specified record */
    i4_result = db_set_cursor(pt_master->h_db, DB_GOTO_RECORD, ui2_listnum);
    if (i4_result != DBR_OK)
    {
        return FLMR_REC_NOT_FOUND;
    }

    t_field = (DB_FIELD_T) DB_MAKE_FIELD_TYPE(DBF_STRING_T, FLM_NAME_FIELD_T);
    ui4_db_version = FLM_NULL_VER_ID;

    /* read the name */
    apv_field_value_addr[0] = s_name;
    i4_result = db_read_rec(
                    pt_master->h_db,
                    0,
                    NULL,
                    NULL,
                    NULL,
                    1,
                    &t_field,
                    (VOID*) &apv_field_value_addr,
                    0,
                    &ui4_db_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    *pz_length = x_strlen(s_name);
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_name
 *
 * Description: This API gets the name of the specified
 *              favorites list
 *
 * Inputs:  -
 *    ui2_listnum  - list number
 *    pui4_version
 *
 * Outputs: -
 *    ps_name - pointer to string to hold name
 *    pui4_version
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ui2_listnum or ps_name is not valid
 *    FLMR_REC_NOT_FOUND - no such list
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions
 *    Aborts if h_flm_master_list is NULL
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_name(
    UINT16  ui2_listnum,
    CHAR*   ps_name,
    UINT32* pui4_master_version
)
{
    INT32         i4_result;
    HANDLE_T      h_flm_master_list;
    FLM_MASTER_T* pt_master;
    DB_FIELD_T    t_field;
    CRIT_STATE_T  t_old_level;

    DBG_API(("{FLM} x_flm_get_name: ui2_listnum %d ps_name %s pui4_master_version 0x%x\n",
             ui2_listnum, ps_name, pui4_master_version));

    if (ps_name == NULL || pui4_master_version == NULL)
    {
        return FLMR_INV_ARG;
    }

    if (ui2_listnum > x_flm_get_max_lists())
    {
        return FLMR_INV_ARG;
    }

    /* default just in case of error */
    *ps_name = '\0';

    /* look name up in master list */
    i4_result = flm_get_master_list(&h_flm_master_list);
    if (i4_result != FLMR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_flm_master_list, (VOID **) &pt_master);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 0);
    }

    /* set position to specified record */
    i4_result = db_set_cursor(pt_master->h_db, DB_FIRST_RECORD, 0);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    /* read the name */
    t_field = (DB_FIELD_T) DB_MAKE_FIELD_TYPE(DBF_STRING_T, FLM_NAME_FIELD_T);

    i4_result = db_read_rec(
                    pt_master->h_db,
                    0,
                    NULL,
                    NULL,
                    NULL,
                    1,
                    &t_field,
                    (VOID*) &ps_name,
                    ui2_listnum,
                    pui4_master_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_open
 *
 * Description: This API opens a favorites list
 *
 * Inputs:  -
 *    ps_name - name of favorites list
 *    pv_tag - Pointer to tag
 *    pf_notify_func - Pointer to notification function
 *
 * Outputs: -
 *    ph_fav_handle - handle of favorites list
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_ARG - ph_fav_handle or ps_name are NULL
 *    FLMR_OUT_OF_MEM - 
 *    FLMR_OUT_OF_HANDLE - 
 *    FLMR_LIST_NOT_FOUND
 *
 * Exceptions
 *    None
 *
 ------------------------------------------------------------------------*/

INT32 x_flm_open
(
    CHAR*           ps_name,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_fav_handle
)
{
    INT32       i4_result;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_open: ps_name %s pv_tag %d pf_flm_notify 0x%x ph_fav_handle 0x%x\n",
             ps_name, pv_tag, pf_flm_notify, ph_fav_handle));

    if (ph_fav_handle == NULL || ps_name == NULL)
    {
        return FLMR_INV_ARG;
    }

    if (x_strlen(ps_name) >= FLM_NAME_LEN)
    {
        return FLMR_INV_ARG;
    }

    i4_result = flm_alloc_handle(ph_fav_handle, pv_tag, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    i4_result = db_get_handle_by_name(
                    ps_name,
                    (VOID *) pt_list,
                    flm_notify,
                    &(pt_list->h_db)
                );
    if (i4_result != DBR_OK)
    {
        x_handle_free(*ph_fav_handle);
        *ph_fav_handle = NULL_HANDLE;
        return db_to_flm_err(i4_result);
    }

    pt_list->pf_flm_notify = pf_flm_notify;
    pt_list->h_self = *ph_fav_handle;
    x_strcpy(pt_list->as_name, ps_name);
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_close
 *
 * Description: This API closes a favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_close
(
    HANDLE_T h_fav_handle
)
{
    INT32       i4_result;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_close: h_fav_handle %d\n", h_fav_handle));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    i4_result = x_handle_free(h_fav_handle);
    if (i4_result != HR_OK)
    {
        return FLMR_INV_HANDLE;
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_num_recs
 *
 * Description: This API gets the number of recs in a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle of favorites list
 *
 * Outputs: -
 *    pui2_num_recs - Pointer to memory to receive number of recs
 *    pui4_fav_version - Pointer to memory to receive version of favorites list.
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_num_recs or pui4_fav_version are not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_num_recs(
    HANDLE_T h_fav_handle,
    UINT16*  pui2_num_recs,
    UINT32*  pui4_fav_version
)
{
    FLM_LIST_T*  pt_list;
    DB_INFO_T    db_info;
    INT32        i4_result;
    CRIT_STATE_T t_old_level;

    DBG_API(("{FLM} x_flm_get_num_recs: h_fav_handle %d pui2_num_recs 0x%x pui4_fav_version 0x%x\n",
             h_fav_handle, pui2_num_recs, pui4_fav_version));

    if (h_fav_handle == NULL_HANDLE)
    {
        return FLMR_INV_HANDLE;
    }

    if (pui2_num_recs == NULL || pui4_fav_version == NULL)
    {
        return FLMR_INV_ARG;
    }

    t_old_level = x_crit_start();
    i4_result = handle_get_obj(h_fav_handle, (VOID*) &pt_list);
    x_crit_end(t_old_level);
    if (i4_result != HR_OK)
    {
#ifdef DEBUG
        FLM_ABORT(DBG_CAT_HANDLE, 1);
#endif
        return FLMR_INV_HANDLE;
    }

    /* do a db_get_info */
    i4_result = db_get_info(pt_list->h_db, &db_info);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }
    *pui2_num_recs = (UINT16) db_info.ui4_record_count;
    *pui4_fav_version = db_info.ui4_db_version_id;

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_lock
 *
 * Description: This API locks the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_lock
(
    HANDLE_T h_fav_handle
)
{
    INT32       i4_result;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_lock: h_fav_handle %d\n", h_fav_handle));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    i4_result = db_write_lock(pt_list->h_db);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_unlock
 *
 * Description: This API unlocks the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle of favorites list
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_unlock
(
    HANDLE_T h_fav_handle
)
{
    INT32       i4_result;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_unlock: h_fav_handle %d\n", h_fav_handle));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    i4_result = db_write_unlock(pt_list->h_db);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_position
 *
 * Description: This API gets the current position of the specified
 *              service in a favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *    pt_service - Pointer to service to look for
 *    ui2_record_index - If there are more than one records that
 *                match, this selects between them. 0 = first,
 *                1 = second, etc.
 *    pui4_version
 *
 * Outputs: -
 *    pui2_position - pointer to a variable to retrieve position
 *    pui4_version
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_position or pui4_version is not valid
 *    FLMR_REC_NOT_FOUND - record not found
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_position
(
    HANDLE_T     h_fav_handle,
    UINT16*      pui2_position,
    FLM_REC_T*   pt_service,
    UINT16       ui2_record_index,
    UINT32*      pui4_version
)
{
    INT32       i4_result;
    DB_FIELD_T  t_data_field;
    DB_FIELD_T  t_search_field;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_get_position: h_fav_handle %d pui2_position 0x%x pt_service 0x%x ui2_record_index %d pui4_version 0x%x\n",
             h_fav_handle, pui2_position, pt_service, ui2_record_index, pui4_version));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    if (pui4_version == NULL || pt_service == NULL || pui2_position == NULL)
    {
        return FLMR_INV_ARG;
    }
   
    /* search for the record with specified data */
    t_search_field = (DB_FIELD_T) FLM_DATA_FIELD_TYPE;

    /* find record and return the index field */
    t_data_field = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;

    i4_result = db_read_rec(
                    pt_list->h_db,
                    1,
                    &t_search_field,
                    (VOID*)&pt_service,
                    NULL,
                    1,
                    &t_data_field,
                    (VOID*) &pui2_position,
                    ui2_record_index,
                    pui4_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_add_rec
 *
 * Description: This API adds the specified service to the specified
 *              favorites list
 *
 * Inputs:
 *    h_fav_handle  - handle to favorites list
 *    pt_rec        - pointer to service to add
 *    pui2_position - where to add service
 *
 * Outputs:
 *    pui2_position - where service was actually added
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_HANDLE    - handle is not valid
 *    FLMR_INV_ARG       - ui2_flags is not valid
 *    FLMR_LIMIT_EXCEED  - no free position in list
 *    FLMR_REC_NOT_FOUND - corrupted list
 *    FLMR_OUT_OF_MEM    - 
 *
 * Exceptions:
 *    Aborts if an error happened as records were being moved down
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_add_rec(
    HANDLE_T   h_fav_handle,
    FLM_REC_T* pt_rec,
    UINT16*    pui2_position
)
{
    INT32       i4_result;
    DB_INFO_T   db_info;
    UINT16      ui2_num_records;
    FLM_LIST_T* pt_list;
    VOID*       apv_keyfield_addr[1];
    const VOID* apv_field_value_addr[2];

    DBG_API(("{FLM} x_flm_add_rec: h_fav_handle %d pt_rec 0x%x pui2_position 0x%x\n",
             h_fav_handle, pt_rec, pui2_position));

    if (pt_rec == NULL || pui2_position == NULL)
    {
        return FLMR_INV_ARG;
    }

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    /* if list is already full, error */
    /* count number of records in master list */
    i4_result = db_get_info(pt_list->h_db, &db_info);
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    ui2_num_records = (UINT16) db_info.ui4_record_count;
    if ( ((UINT32)(ui2_num_records+1)) > ((UINT32) x_flm_get_max_size()) )
    {
        return FLMR_LIMIT_EXCEED;
    }

    if (ui2_num_records > 0)
    {
        if (*pui2_position >= ui2_num_records)
        {
            /* if position is beyond the end, add it to the end */
            *pui2_position = ui2_num_records;
        }
        else
        {
            /* move any other records down to make room */
            i4_result = flm_move_down(pt_list->h_db, *pui2_position,
                                 ui2_num_records);
            if (i4_result != FLMR_OK)
            {
                FLM_ABORT(DBG_CAT_INV_OP, 10);
            }
        }
    }
    else
    {
        *pui2_position = 0;
    }
    apv_keyfield_addr[0] = pui2_position;
    apv_field_value_addr[0] = pui2_position;
    apv_field_value_addr[1] = pt_rec;

    /* write new rec into current record */
    i4_result = db_write_rec(
                   pt_list->h_db,
                   (VOID*) apv_keyfield_addr,
                   (VOID*) apv_field_value_addr,
                   DB_APPEND_RECORD
                 );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_del_rec
 *
 * Description: This API deletes the specified rec from the specified
 *              favorites list
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_position - position to delete
 *
 * Outputs:
 *
 * Returns:
 *    FLMR_OK            - on success
 *    FLMR_INV_HANDLE    - handle is not valid
 *    FLMR_INV_ARG       - ui2_position is not valid
 *    FLMR_REC_NOT_FOUND - corrupted list
 *
 * Exceptions
 *    Aborts if an error happened while moving records up
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_del_rec(
    HANDLE_T h_fav_handle,
    UINT16   ui2_position
)
{
    INT32       i4_result;
    DB_FIELD_T  t_index_field;
    UINT16*     pt_index;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_del_rec: h_fav_handle %d ui2_position %d\n",
             h_fav_handle, ui2_position));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    /* delete it */
    t_index_field = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    pt_index = &ui2_position;
    i4_result = db_delete_rec(
                  pt_list->h_db,
                  1,
                  &t_index_field,
                  (VOID *) &pt_index,
                  0
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    i4_result = flm_move_up(pt_list->h_db, ui2_position);
    if (i4_result != FLMR_OK)
    {
        FLM_ABORT(DBG_CAT_INV_OP, 11);
    }
    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_get_rec
 *
 * Description: This API gets the service from the specified position in
 *              the specified favorites list
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_position - position to get rec of
 *    pui4_version
 *
 * Outputs:
 *    pt_rec - pointer to variable to store the rec in.
 *    pui4_version
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - t_rec is not valid
 *    FLMR_REC_NOT_FOUND - record not found
 *    FLMR_MODIFIED - favorites list version is different from expected
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_get_rec(
    HANDLE_T   h_fav_handle,
    UINT16     ui2_position,
    FLM_REC_T* pt_rec,
    UINT32*    pui4_version
)
{
    INT32       i4_result;
    DB_FIELD_T  at_data_field[1];
    DB_FIELD_T  at_search_field[1];
    FLM_LIST_T* pt_list;
    VOID*       apv_keyfield_addr[1];
    VOID*       apv_field_value_addr[1];

    DBG_API(("{FLM} x_flm_get_rec: h_fav_handle %d ui2_position %d pt_rec 0x%x pui4_version 0x%x\n",
             h_fav_handle, ui2_position, pt_rec, pui4_version));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    if (pt_rec == NULL || pui4_version == NULL)
    {
        return FLMR_INV_ARG;
    }

    /* search for the record with specified index */
    at_search_field[0] = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    apv_keyfield_addr[0] = &ui2_position;

    /* find record and return the data field */
    at_data_field[0] = (DB_FIELD_T) FLM_DATA_FIELD_TYPE;
    apv_field_value_addr[0] = pt_rec;

    i4_result = db_read_rec(
                    pt_list->h_db,
                    1,
                    at_search_field,
                    (VOID*) apv_keyfield_addr,
                    NULL,
                    1,
                    at_data_field,
                    (VOID*) apv_field_value_addr,
                    0,
                    pui4_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_swap_rec
 *
 * Description: This API swaps the positions of the specified recs
 *              of the specified favorites list
 *
 * Inputs:  -
 *    h_fav_handle - handle to favorites list
 *    ui2_position1 - position of one of the recs to be swapped
 *    ui2_position2 - position of one of the recs to be swapped
 *
 * Outputs: -
 *
 * Returns: -
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_swap_rec(
    HANDLE_T h_fav_handle,
    UINT16   ui2_position1,
    UINT16   ui2_position2
)
{
    INT32       i4_result;
    FLM_REC_T*  pt_rec1;
    FLM_REC_T*  pt_rec2;
    FLM_REC_T   t_rec1;
    FLM_REC_T   t_rec2;
    UINT16*     pui2_index1;
    UINT16*     pui2_index2;
    UINT32      ui4_db_version;
    DB_FIELD_T  t_data_field;
    DB_FIELD_T  t_search_field;
    FLM_LIST_T* pt_list;

    DBG_API(("{FLM} x_flm_swap_rec: h_fav_handle %d ui2_position1 %d ui2_position2 %d\n",
             h_fav_handle, ui2_position1, ui2_position2));

    i4_result = flm_get_check_list(h_fav_handle, &pt_list);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    /* search for first record */
    t_search_field = (DB_FIELD_T) FLM_INDEX_FIELD_TYPE;
    pui2_index1 = &ui2_position1;
    pui2_index2 = &ui2_position2;

    t_data_field = (DB_FIELD_T) FLM_DATA_FIELD_TYPE;
    pt_rec1 = &t_rec1;
    ui4_db_version = FLM_NULL_VER_ID;

    i4_result = db_read_rec(
                    pt_list->h_db,
                    1,
                    &t_search_field,
                    (VOID*)&pui2_index1,
                    NULL,
                    1,
                    &t_data_field,
                    (VOID*) &pt_rec1,
                    0,
                    &ui4_db_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    /* if record exists but positions are the same */
    /* then we are done! */
    if (ui2_position1 == ui2_position2)
    {
        return FLMR_OK;
    }

    /* read record 2 */
    pt_rec2 = &t_rec2;
    i4_result = db_read_rec(
                    pt_list->h_db,
                    1,
                    &t_search_field,
                    (VOID*)&pui2_index2,
                    NULL,
                    1,
                    &t_data_field,
                    (VOID*) &pt_rec2,
                    0,
                    &ui4_db_version
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    /* update record 1 */
    i4_result = db_update_rec(
                   pt_list->h_db,
                   (VOID *) &pui2_index1,
                   1,
                   &t_data_field,
                   (VOID *) &pt_rec2
                 );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    /* update record 2 */
    i4_result = db_update_rec(
                   pt_list->h_db,
                   (VOID *) &pui2_index2,
                   1,
                   &t_data_field,
                   (VOID *) &pt_rec1
                );
    if (i4_result != DBR_OK)
    {
        return db_to_flm_err(i4_result);
    }

    return FLMR_OK;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_move_rec
 *
 * Description: This API moves the specified rec from one position
 *              of the specified favorites list to another.
 *
 * Inputs:
 *    h_fav_handle - handle to favorites list
 *    ui2_from - original position of rec to be moved
 *    pui2_to - requested resting place of rec
 *
 * Outputs:
 *    pui2_to - final resting place of rec
 *
 * Returns:
 *    FLMR_OK - on success
 *    FLMR_INV_HANDLE - handle is not valid
 *    FLMR_INV_ARG - pui2_to is not valid
 *    FLMR_REC_NOT_FOUND - no such record
 *    FLMR_LIMIT_EXCEED- no free position in list
 *
 * Exceptions:
 ------------------------------------------------------------------------*/
INT32 x_flm_move_rec(
    HANDLE_T h_fav_handle,
    UINT16   ui2_from,
    UINT16*  pui2_to
)
{
    INT32     i4_result;
    FLM_REC_T t_rec;
    UINT32    ui4_version;

    DBG_API(("{FLM} x_flm_move_rec: h_fav_handle %d ui2_from %d pui2_to 0x%x\n",
             h_fav_handle, ui2_from, pui2_to));

    if (pui2_to == NULL)
    {
        return FLMR_INV_ARG;
    }

    /*** let user pass in? ***/
    ui4_version = FLM_NULL_VER_ID;

    /* read data for record */
    i4_result = x_flm_get_rec(h_fav_handle, ui2_from, &t_rec, &ui4_version);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    /* delete the record from the old spot */
    i4_result =  x_flm_del_rec(h_fav_handle, ui2_from);
    if (i4_result != FLMR_OK)
    {
        return i4_result;
    }

    /* insert the record in the new (modified) spot */
    return x_flm_add_rec(h_fav_handle, &t_rec, pui2_to);
}

/*-------------------------------------------------------------------------
 * Name: x_flm_master_change_reg
 *
 * Description: This API returns a handle to register notification
 *              of the changing of the master list.
 *
 * Inputs:  -
 *    pv_tag         - Pointer to tag
 *    pf_flm_notify  - Pointer to notification function
 *
 * Outputs: -
 *    ph_notify_handle  - pointer to storage for handle to notification
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *
 * Exceptions
 *    Aborts if could not get master list object
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_master_change_reg(
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_notify_handle
)
{
    INT32 i4_result;
    FLM_MASTER_NOTIFY_T* pt_notify;

    DBG_API(("{FLM} x_flm_master_change_reg: pv_tag %d pf_flm_notify 0x%x ph_notify_handle 0x%x\n",
             pv_tag, pf_flm_notify, ph_notify_handle));

    if (ph_notify_handle == NULL || pf_flm_notify == NULL)
    {
        return FLMR_INV_ARG;
    }

    /*
     * Keeps tag and notify pointer
     * when master list is changed (notified), the master
     * notification callback will go through the list and also
     * call any of these user notification routines with their tags.
     *
     * For the user to "unregister" for notification, all they have
     * to do is x_handle_free on the handle. flm_free_notify will
     * remove the appropriate rec (matching tag).
     */

    /* allocate a structure for master notify object */
    pt_notify = x_mem_calloc(1, sizeof(FLM_MASTER_NOTIFY_T));
    if (pt_notify == NULL)
    {
        return FLMR_OUT_OF_MEM;
    }

    /* allocate a handle */
    i4_result = handle_alloc(FLM_MASTER_NOTIFY_HANDLE,
                             pt_notify,
                             0,
                             flm_free_notify_callback,
                             ph_notify_handle);
    if (i4_result != HR_OK)
    {
        x_mem_free(pt_notify);
        return FLMR_OUT_OF_HANDLE;
    }

    /* stick handle, tag, and notify routine in object */
    pt_notify->h_notify = *ph_notify_handle;
    pt_notify->pv_tag = pv_tag;
    pt_notify->pf_flm_notify = pf_flm_notify;

    /* add to linked list in master list object */
    i4_result = flm_add_notification(pt_notify);

    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_fs_load
 *
 * Description: This API loads a specific favorites list
 *              from persistant storage.
 *
 * Inputs:  -
 *    h_file        - handle to file to load from
 *    ps_pathname   - name of list to load
 *    pv_tag        - client data for notification
 *    pf_flm_notify - client notification function
 *
 * Outputs: -
 *    ph_list - Pointer to handle for loaded favorites list
 *
 * Returns
 *    FLMR_OK
 *    FLMR_INV_ARG          - h_dir is not a valid handle
 *    FLMR_LIMIT_EXCEED     - too many favorites lists in CDB
 *    FLMR_LIST_NOT_FOUND   - could not find pv_listname list
 *    FLMR_INV_FILE         - file is not FLM file
 *    FLMR_OUT_OF_HANDLE    - out of handles
 *    FLMR_OUT_OF_MEM       - out of memory
 *    FLMR_CANT_CREATE_LIST - error trying to create list
 *    FLMR_EOF
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_INVAL_FILE_PATH
 *    FLMR_PERM_DENY       - don't have permission to access file
 *    FLMR_REC_NOT_FOUND   - if an existing record is deleted from
 *                           CDB but name wasn't in master list.
 *
 * Exceptions
 *   Aborts on unlock error
 *   Aborts on close error
 *   Aborts if can't create raw read lock
 *   Aborts if can't unlock raw read lock
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_fs_load(
    HANDLE_T        h_dir,
    CHAR*           ps_pathname,
    CHAR*           ps_listname,
    VOID*           pv_tag,
    pf_flm_nfy_func pf_flm_notify,
    HANDLE_T*       ph_list
)
{
    DBG_API(("{FLM} x_flm_fs_load: h_dir %d ps_pathname %s ps_listname %s pv_tag %d pf_flm_notify 0x%x ph_list 0x%x\n",
             h_dir, ps_pathname, ps_listname, pv_tag, pf_flm_notify, ph_list));

    return flm_load_raw_list(h_dir, ps_pathname, ps_listname,
                         pv_tag, pf_flm_notify, ph_list);
}

/*-------------------------------------------------------------------------
 * Name: x_flm_fs_store
 *
 * Description: This API stores a favorites list to persistant storage.
 *
 * Inputs:
 *    h_dir       - handle to directory to store to
 *    ps_pathname - name of file to store to
 *    h_list      - handle to favorites list to store
 *
 * Outputs:
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_CANT_CREATE_FILE
 *    FLMR_CANT_OPEN_FILE
 *    FLMR_NO_WRITE_LOCK
 *    FLMR_INV_ARG
 *    FLMR_INV_HANDLE
 *    FLMR_REC_NOT_FOUND
 *    FLMR_LIST_NOT_FOUND
 *    FLMR_OUT_OF_MEM
 *    FLMR_OUT_OF_HANDLE
 *    FLMR_DEV_ERR
 *    FLMR_OS_ERR
 *    FLMR_INV_FILE_PATH
 *    FLMR_PERM_DENY
 *
 flm_store_list
 *
 * Exceptions:
 *    Aborts if h_flm_master_list is NULL
 *    Aborts if can't get master object
 *    Aborts if can't get master object info
 *    Aborts if can't get list name
 *    Aborts if can't open list
 *    Aborts if can't unlock files
 *    Aborts if can't close files
 *    Aborts if can't create semaphore
 *    Aborts if raw device error
 *    Aborts if no memory for raw device header
 *    Aborts on file write errors for file into/metadata
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_fs_store(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    HANDLE_T h_list
)
{
    DBG_API(("{FLM} x_flm_fs_store: h_dir %d ps_pathname %s h_list %d\n",
             h_dir, ps_pathname, h_list));

    return flm_store_raw_list(h_dir, ps_pathname, h_list);
}


/*-------------------------------------------------------------------------
 * Name: x_flm_fs_del
 *
 * Description: This API deletes
 *
 * Inputs:
 *    h_dir       - handle to directory to delete from
 *    ps_pathname - name of file to delete from
 *    ps_listname - name of list to delete??
 *
 * Outputs:
 *
 * Returns:
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_fs_del(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    CHAR*    ps_listname
)
{
    INT32         i4_result;
    UINT16 ui2_i;
    UINT16 ui2_limit;
    UINT32 ui4_id;
    BOOL   b_found = FALSE;
    CHAR   s_listname[FLM_NAME_LEN+1];

    DBG_API(("{FLM} x_flm_fs_del: h_dir %d ps_pathname %s ps_listname %s\n",
             h_dir, ps_pathname, ps_listname));

    i4_result = FLMR_OK;

    if (ps_listname == NULL)
    {
        return FLMR_INV_ARG;
    }

    ui2_limit = x_flm_get_max_lists();
    /* iterate over FLM lists and find a match */
    for (ui2_i = 0; ui2_i < ui2_limit; ++ui2_i)
    {
        i4_result = flm_raw_fs_qry(h_dir, 
                                   ps_pathname, 
                                   ui2_i, 
                                   s_listname,
                                   &ui4_id);
        if (i4_result)
        {
            break;
        }

        if (x_strncmp(ps_listname, s_listname, FLM_NAME_LEN) == 0)
        {
            i4_result = flm_raw_del(h_dir, ps_pathname, ui4_id);
            if (i4_result == FLMR_OK)
            {
                b_found = TRUE;
            }
            break;
        }
    }

    if (b_found)
    {
        return FLMR_OK;
    }

    return i4_result;
}

/*-------------------------------------------------------------------------
 * Name: x_flm_fs_qry
 *
 * Description: This API queries
 *
 * Inputs:  -
 *    h_dir       - handle to directory to query
 *    ps_pathname - name of file to query
 *    ui2_index   - index (0=first, 1=second,...)
 *
 * Outputs: -
 *    ps_listname - name of list at index
 *
 * Returns:
 *    FLMR_OK
 *    FLMR_INV_ARG
 *    FLMR_LIST_NOT_FOUND
 *
 * Exceptions:
 *
 ------------------------------------------------------------------------*/
INT32 x_flm_fs_qry(
    HANDLE_T h_dir,
    CHAR*    ps_pathname,
    UINT16   ui2_index,
    CHAR*    ps_listname
)
{
    UINT32        ui4_id;

    DBG_API(("{FLM} x_flm_fs_qry: h_dir %d ps_pathname %s ui2_index %d ps_listname %s\n",
             h_dir, ps_pathname, ui2_index, ps_listname));

    if (ps_listname == NULL)
    {
        return FLMR_INV_ARG;
    }

    return flm_raw_fs_qry(
                   h_dir,
                   ps_pathname,
                   ui2_index,
                   ps_listname,
                   &ui4_id
                 );
}
