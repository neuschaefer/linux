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
 * $RCSfile: cfg.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains public Configuration Manager (CFG) functions
 *-----------------------------------------------------------------*/

#include "config/cfg.h"
#include "handle/handle.h"

#include "dbg/def_dbg_level_mw.h"
#include "cli/u_cli.h"
#include "cli/x_cli.h"

#ifndef DBG_INIT_LEVEL_MW_CFG
#define DBG_INIT_LEVEL_MW_CFG  DBG_LEVEL_ERROR
#endif

/*------------------------------------------------------------
                 Global Variables
 -----------------------------------------------------------*/

static BOOL               b_cfg_init = FALSE;
static HANDLE_T           h_cfg_lock;        /* lock for CFG Manager */

static CFG_LIST_T*        pt_configs;

static UINT16             ui2_cfg_num;

#ifdef CLI_LVL_ALL

static CLI_EXEC_T at_cli_dbg_cmd[] = {
    {CLI_GET_DBG_LVL_STR, NULL, cfg_get_dbg_level, NULL, CLI_GET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR, NULL, cfg_set_dbg_level, NULL, CLI_SET_DBG_LVL_HELP_STR, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_dbg[] = {
    {"cfg", NULL, NULL, &(at_cli_dbg_cmd[0]), "Configuration Manager commands", CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

#endif

UINT16     ui2_cfg_debug_level = DBG_INIT_LEVEL_MW_CFG;

/*------------------------------------------------------------
                 Functions
 -----------------------------------------------------------*/
UINT16 db_to_cfg_field(CFG_FIELD_T e_field, UINT16 ui1_count)
/* UINT16 db_to_cfg_field(CFG_FIELD_T e_field, UINT8 ui1_count) */
{
    switch (e_field)
    {
    case CFG_8BIT_T:
        if (ui1_count >1)
        {
            return DBF_ARRAY_OF_INT8_T;
        }
        else
        {
            return DBF_INT8_T;
        }
    case CFG_16BIT_T:
        if (ui1_count >1)
        {
            return DBF_ARRAY_OF_INT16_T;
        }
        else
        {
            return DBF_INT16_T;
        }
    case CFG_32BIT_T:
        if (ui1_count >1)
        {
            return DBF_ARRAY_OF_INT32_T;
        }
        else
        {
            return DBF_INT32_T;
        }
    case CFG_64BIT_T:
        if (ui1_count >1)
        {
            return DBF_ARRAY_OF_INT64_T;
        }
        else
        {
            return DBF_INT64_T;
        }
    case CFG_NONE:
    default:
        return DBF_UNKNOWN;
    }
}

/*---------------------------------------------------------------------
 * Name: db_to_cfg_err
 *
 * Description: translates a database error to a configuration error
 *
 * Inputs: -
 *         i4_db_error - Database error
 *
 * Returns: CFG error
 *
 --------------------------------------------------------------------*/
INT32 db_to_cfg_err(
                   INT32 i4_db_error
                   )
{
    INT32 i4_cfg_error;

    switch (i4_db_error)
    {
    case DBR_OK:
        i4_cfg_error = CFGR_OK;
        break;
    case DBR_INV_ARG:
        i4_cfg_error = CFGR_INV_ARG;
        break;
    case DBR_INV_HANDLE:
        i4_cfg_error = CFGR_INV_HANDLE;
        break;
    case DBR_OUT_OF_MEMORY:
        i4_cfg_error = CFGR_OUT_OF_MEM;
        break;
    case DBR_OUT_OF_HANDLE:
        i4_cfg_error = CFGR_OUT_OF_HANDLE;
        break;
    case DBR_NO_WRITE_LOCK:
        i4_cfg_error = CFGR_NO_WRITE_LOCK;
        break;
    case DBR_REC_NOT_FOUND:
        i4_cfg_error = CFGR_REC_NOT_FOUND;
        break;
    case DBR_WRITE_LOCKED:
        i4_cfg_error = CFGR_WRITE_LOCKED;
        break;
    case DBR_END_OF_REC:
        i4_cfg_error = CFGR_END_OF_ITERATION;
        break;
    case DBR_MODIFIED:
        i4_cfg_error = CFGR_MODIFIED;
        break;
    case DBR_DB_LIMIT_EXCEED:
        i4_cfg_error = CFGR_LIMIT_EXCEEDED;
        break;
    case DBR_INV_NAME:
        i4_cfg_error = CFGR_EXIST;
        break;
    case DBR_FILE_READ_FAIL:
        i4_cfg_error = CFGR_FILE_READ_FAIL;
        break;
    case DBR_FILE_WRITE_FAIL:
        i4_cfg_error = CFGR_FILE_WRITE_FAIL;
        break;
    default:
        i4_cfg_error = CFGR_UNKNOWN;
        break;
    }
    return i4_cfg_error;
}

/*---------------------------------------------------------------------
 * Name: fm_to_cfg_err
 *
 * Description: translates a file manager error to a configuration error
 *
 * Inputs: -
 *         i4_fm_error - File Manager error
 *
 * Returns: CFG error
 *
 --------------------------------------------------------------------*/
INT32 fm_to_cfg_err(
                   INT32 i4_fm_error
                   )
{
    INT32 i4_cfg_err;

    switch (i4_fm_error)
    {
    case FMR_OK:
        i4_cfg_err = CFGR_OK;
        break;
    case FMR_ARG:
        i4_cfg_err = CFGR_INV_ARG;
        break;
    case FMR_HANDLE:
        i4_cfg_err = CFGR_INV_HANDLE;
        break;
    case FMR_INVAL:
        i4_cfg_err = CFGR_INV_HANDLE;
        break;
    case FMR_CORE:
        i4_cfg_err = CFGR_CORE;
        break;
    case FMR_EXIST:
        i4_cfg_err = CFGR_EXIST;
        break;
    case FMR_NO_ENTRY:
        i4_cfg_err = CFGR_NO_SUCH_FILE;
        break;
    case FMR_NOT_DIR:
        i4_cfg_err = CFGR_NOT_DIR;
        break;
    case FMR_NOT_INIT:
        i4_cfg_err = CFGR_UNKNOWN;
        break;
    case FMR_DEVICE_ERROR:
        i4_cfg_err = CFGR_DEVICE_ERROR;
        break;
    case FMR_NO_SUCH_DEVICE:
        i4_cfg_err = CFGR_NO_SUCH_DEVICE;
        break;
    case FMR_NOT_ENOUGH_SPACE:
    case FMR_FILE_SYSTEM_FULL:
        i4_cfg_err = CFGR_NO_FREE_SPACE;
        break;
    case FMR_NAME_TOO_LONG:
        i4_cfg_err = CFGR_NAME_TOO_LONG;
        break;
    default:
        i4_cfg_err = CFGR_UNKNOWN;
        break;
    }
    return i4_cfg_err;
}

/*-------------------------------------------------------------------------
 * Name: fsraw_to_cfg_err
 *
 * Description: translates a raw file system error to a config manager error
 *
 * Inputs: -
 *         i4_fsraw_error - Raw File system error
 *
 * Returns: CFG error
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 fsraw_to_cfg_err(
                      INT32 i4_fsraw_error
                      )
{
    INT32 i4_cfg_error;

    switch (i4_fsraw_error)
    {
    case FSRAWR_OK:
        i4_cfg_error = CFGR_OK;
        break;
    case FSRAWR_OUT_OF_SPACE:
        i4_cfg_error = CFGR_FILESYS_FULL;
        break;
    case FSRAWR_NOT_FOUND:
        i4_cfg_error = CFGR_NO_SUCH_FILE;
        break;
    case FSRAWR_INV_ARG:
        i4_cfg_error = CFGR_INV_ARG;
        break;
    default:
        i4_cfg_error = CFGR_UNKNOWN;
        break;
    }
    return i4_cfg_error;
}

/*-----------------------------------------------------------------------
--
 * Name: cfg_notify_handle_autofree
 *
 * Description: 
 *    Frees a configuration notification handle
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ------------------------------------------------------------------------
*/
static INT32 cfg_notify_handle_autofree(
                                       HANDLE_T      h_handle,
                                       HANDLE_TYPE_T e_type
                                       )
{
    return handle_free(h_handle, FALSE);
}

/*-----------------------------------------------------------------------
--
 * Name: cfg_handle_autofree
 *
 * Description: 
 *    Frees a configuration handle
 *
 * Inputs:  h_handle  Contains the handle to delete
 *          e_type    Contains the type of the handle.
 *
 * Outputs: -
 *
 * Returns: HR_OK  Routine successful.
 ------------------------------------------------------------------------
*/
static INT32 cfg_handle_autofree(
                                 HANDLE_T      h_handle,
                                 HANDLE_TYPE_T e_type
                                )
{
    return handle_free(h_handle, FALSE);
}

/*---------------------------------------------------------------------
 * Name: cfg_init
 *
 * Description: Initiates everything required for Configuation Manager
 *
 * Returns: CFG error
 *    CFGR_OK
 *    CFGR_CANT_CREATE_LOCK
 *
 --------------------------------------------------------------------*/
INT32 cfg_init(
              VOID
              )
{
    INT32 i4_result;
    static handle_autofree_fct apf_autofree_fcts[2] =
    {
        cfg_handle_autofree,
        cfg_notify_handle_autofree
    };

    if (b_cfg_init)
    {
        CFG_ABORT(DBG_CAT_INIT, 6);
    }

    b_cfg_init = TRUE;

    pt_configs = NULL;

    ui2_cfg_num = 2; /* named config IDs start with 2 */

    /*** do any needed initialization here ***/
    i4_result = rwl_create_lock(&h_cfg_lock);
    if (i4_result != RWLR_OK)
    {
        /*** should be an abort ***/
        return CFGR_CANT_CREATE_LOCK;
    }
#if 0
    i4_result = rwl_create_lock(&h_cfg_nfy_lock);
    if (i4_result != RWLR_OK)
    {
        /*** should be an abort ***/
        return CFGR_CANT_CREATE_LOCK;
    }
#endif
    i4_result = handle_set_autofree_tbl(HT_GROUP_CFG, apf_autofree_fcts);
    if (i4_result != HR_OK)
    {
        ABORT(DBG_CAT_INV_OP, 1);
    }

#ifdef CLI_LVL_ALL
    /* register CLI stuff */
    x_cli_attach_cmd_tbl(at_cli_dbg, CLI_CAT_MW, 0);
#endif

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_get_read_lock
 *
 * Description: Gets read lock
 *
 * Returns: 
 *    CFGR_OK
 *    CFGR_NO_READ_LOCK
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_read_lock(VOID)
{
    INT32 i4_result;

    i4_result = rwl_read_lock(h_cfg_lock, RWL_OPTION_WAIT);
    if (i4_result != RWLR_OK)
    {
        return CFGR_NO_READ_LOCK;
    }
    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_get_write_lock
 *
 * Description: Gets write lock
 *
 * Returns: 
 *    CFGR_OK
 *    CFGR_NO_WRITE_LOCK
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_write_lock(VOID)
{
    INT32 i4_result;

    i4_result = rwl_write_lock(h_cfg_lock, RWL_OPTION_WAIT);
    if (i4_result != RWLR_OK)
    {
        return CFGR_NO_WRITE_LOCK;
    }
    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_unlock
 *
 * Description: Releases lock
 *
 * Returns: 
 *    CFGR_OK
 *    CFGR_CANT_UNLOCK
 *
 --------------------------------------------------------------------*/
INT32 cfg_unlock(VOID)
{
    INT32 i4_result;

    i4_result = rwl_release_lock(h_cfg_lock);
    if (i4_result != RWLR_OK)
    {
        return CFGR_CANT_UNLOCK;
    }
    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_get_field_size
 *
 * Description: Gets the base field type in bytes
 *
 * Inputs:
 *    ui4_field_type
 *
 * Outputs:
 *    pz_size
 *
 * Returns: 
 *    CFGR_OK
 *    CFGR_INV_ARG
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_field_size(
                        UINT32      ui4_field_type,
                        SIZE_T*     pz_size
                        )
{
    switch (DB_GET_C_TYPE(ui4_field_type))
    {
    case DBF_INT8_T:
    case DBF_ARRAY_OF_INT8_T:
        *pz_size = 1;
        break;
    case DBF_INT16_T:
    case DBF_ARRAY_OF_INT16_T:
        *pz_size = 2;
        break;
    case DBF_INT32_T:
    case DBF_ARRAY_OF_INT32_T:
        *pz_size = 4;
        break;
    case DBF_INT64_T:
    case DBF_ARRAY_OF_INT64_T:
        *pz_size = 8;
        break;
    case CFG_NONE:
    default:
        *pz_size = 0;
        return CFGR_INV_ARG;
    }
    return CFGR_OK;
}

/*-----------------------------------------------------------------
 * Name: cfg_get_group_obj
 *
 * Description: get the information of a settings group
 *
 * Inputs: -
 *    pt_list     - configuration list record
 *    ui2_group   - group ID of setting
 *
 * Outputs:
 *    ppt_group_list - pointer to the vaiable to hold a pointer to
 *                  the group information
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *
 ----------------------------------------------------------------*/
INT32 cfg_get_group_obj(
                       CFG_LIST_T*        pt_list,
                       UINT16             ui2_group,
                       CFG_GROUP_LIST_T** ppt_group_list
                       )
{
    CFG_GROUP_LIST_T* pt_glist;

    if (pt_list == NULL)
    {
        pt_list = pt_configs;
    }

    if (pt_list == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    if (pt_list->t_groups.pt_next_group == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    for (pt_glist = pt_list->t_groups.pt_next_group; pt_glist; pt_glist = pt_glist->pt_next_group)
    {
        if (pt_glist->ui2_group == ui2_group)
        {
            *ppt_group_list = pt_glist;
            return CFGR_OK;
        }
    }

    return CFGR_REC_NOT_FOUND;
}

/*-----------------------------------------------------------------
 * Name: cfg_get_rec_obj
 *
 * Description: get the information of a settings record
 *
 * Inputs: -
 *    ui2_id   - ID of setting
 * Outputs:
 *    ppt_rec_list - pointer to the vaiable to hold a pointer to
 *                  the information
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *
 ----------------------------------------------------------------*/
INT32 cfg_get_rec_obj(
                     CFG_LIST_T*      pt_list,
                     UINT16           ui2_id,
                     CFG_REC_LIST_T** ppt_rec_list
                     )
{
    CFG_GROUP_LIST_T* pt_glist;
    CFG_REC_LIST_T*   pt_reclist;
    INT32             i4_result;

    i4_result = cfg_get_group_obj(pt_list, CFG_GET_GROUP(ui2_id), &pt_glist);
    if (i4_result != CFGR_OK)
    {
        return i4_result;
    }

    if (pt_glist->pt_next_rec == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    for (pt_reclist = pt_glist->pt_next_rec; pt_reclist; pt_reclist = pt_reclist->pt_next)
    {
        if (pt_reclist->ui2_id == ui2_id)
        {
            *ppt_rec_list = pt_reclist;
            return CFGR_OK;
        }
    }

    return CFGR_REC_NOT_FOUND;
}

/*-----------------------------------------------------------------
 * Name: cfg_add_setting
 *
 * Description: Add a setting to the master list
 *
 * Inputs: -
 *    pt_list  - config list record
 *    ui2_id   - ID of setting
 *    h_db     - pointer to database object for setting
 *    ui4_field_type = CDB field type for setting
 *    ui2_num
 *
 * Returns:
 *     CFGR_OK
 *     CFGR_OUT_OF_MEM
 *
 ----------------------------------------------------------------*/
INT32 cfg_add_setting(
                     CFG_LIST_T* pt_list,
                     UINT16      ui2_id,
                     HANDLE_T    h_db,
                     DB_FIELD_T  ui4_field_type,
                     UINT16      ui2_num
                     )
{
    CFG_GROUP_LIST_T* pt_group;
    UINT16            ui2_group;
    CFG_REC_LIST_T*   pt_rec;
    BOOL              b_found = FALSE;

    ui2_group = CFG_GET_GROUP(ui2_id);

    if (pt_list == NULL)
    {
        pt_list = pt_configs;
    }

    if (pt_list == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    /* search if this group exists */
    for (pt_group = pt_list->t_groups.pt_next_group; pt_group; pt_group=pt_group->pt_next_group)
    {
        if (pt_group->ui2_group == ui2_group)
        {
            b_found = TRUE;
            break;
        }
    }

    /* if not, create it */
    if (!b_found)
    {
        CFG_GROUP_LIST_T* pt_new_group;

        pt_new_group = (CFG_GROUP_LIST_T*) x_mem_calloc(1, sizeof(CFG_GROUP_LIST_T));
        if (pt_new_group == NULL)
        {
            return CFGR_OUT_OF_MEM;
        }

        pt_new_group->ui2_group = ui2_group;

        /* add to list of groups */
        if (pt_list->t_groups.pt_next_group)
        {
            pt_new_group->pt_next_group = pt_list->t_groups.pt_next_group;
        }
        pt_list->t_groups.pt_next_group = pt_new_group;
        pt_group = pt_new_group;
    }

    /* search if this setting exists */
    b_found = FALSE;
    for (pt_rec = pt_group->pt_next_rec; pt_rec; pt_rec = pt_rec->pt_next)
    {
        if (pt_rec->ui2_id == ui2_id)
        {
            b_found = TRUE;
            break;
        }
    }

    if (!b_found)
    {
        pt_rec = (CFG_REC_LIST_T*) x_mem_calloc(1, sizeof(CFG_REC_LIST_T));
        if (pt_rec == NULL)
        {
            return CFGR_OUT_OF_MEM;
        }
        /* attach to group node */
        pt_rec->pt_next = pt_group->pt_next_rec;
        pt_group->pt_next_rec = pt_rec;
    }

    /* now add the record */
    pt_rec->ui2_id = ui2_id;
    pt_rec->h_db = h_db;
    pt_rec->ui4_field_type = ui4_field_type;
    pt_rec->ui2_num = ui2_num;

    return CFGR_OK;
}

/*-----------------------------------------------------------------
 * Name: cfg_purge_recs
 *
 * Description: delete all config settings from CDB
 *
 * Inputs:
 *    pt_list - config list to purge
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_INV_HANDLE
 *    CFGR_REC_NOT_FOUND
 *
 -------------------------------------------------------------------*/
INT32 cfg_purge_recs(CFG_LIST_T* pt_list)
{
    CFG_GROUP_LIST_T* pt_group;
    CFG_GROUP_LIST_T* pt_group_temp;
    CFG_REC_LIST_T*   pt_rec;
    CFG_NFY_OBJ_T*    pt_nfy;

    if (pt_list == NULL)
    {
        pt_list = pt_configs;
    }

    if (pt_list == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    /* for each group */
    pt_group = pt_list->t_groups.pt_next_group;
    while (pt_group)
    {
        /* for each ID */
        pt_rec = pt_group->pt_next_rec;
        while (pt_rec)
        {
            CFG_REC_LIST_T* pt_del;
            CHAR            s_name[CFG_NAME_LEN];

            /* delete db entry */
            CFG_MAKE_NAME(s_name, pt_list, pt_rec->ui2_id);

            /* we might be deleting after an unsuccessful load so
               check that the handle is valid. If not, ignore and
               make sure we release the rest of the structures */
            if (x_handle_valid(pt_rec->h_db))
            {
                db_delete(pt_rec->h_db, s_name);
            }

            /* delete record object */
            pt_del = pt_rec;
            pt_rec = pt_rec->pt_next;
            x_mem_free(pt_del);
        }

        pt_group->pt_next_rec = NULL;
        pt_group_temp = pt_group;
        pt_group = pt_group->pt_next_group;

        x_mem_free(pt_group_temp);
    }
    pt_list->t_groups.pt_next_group = NULL;

    /* delete notification list */
    for (pt_nfy = pt_list->t_nfy_list.pt_next;pt_nfy; pt_nfy = pt_nfy->pt_next)
    {
        x_handle_free(pt_nfy->h_nfy);
    }

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_alloc_nfy_obj
 *
 * Description: 
 *   Allocate a notify object and its handle.
 *
 * Outputs: -
 *    ph_notify
 *    ppt_nfy_obj
 *
 * Returns
 *    CFGR_OK
 *    CFGR_INV_ARG
 *    CFGR_OUT_OF_MEM
 *    CFGR_OUT_OF_HANDLE
 *
 --------------------------------------------------------------------*/
INT32 cfg_alloc_nfy_obj(
                       CFG_LIST_T*     pt_config,
                       HANDLE_T*       ph_notify,
                       CFG_NFY_OBJ_T** ppt_nfy_obj
                       )
{
    INT32         i4_result;

    if (ph_notify == NULL|| ppt_nfy_obj == NULL)
    {
        return CFGR_INV_ARG;
    }

    if (pt_config == NULL)
    {
        pt_config = pt_configs;
    }

    if (pt_config == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    *ppt_nfy_obj = (CFG_NFY_OBJ_T*)x_mem_calloc(1, sizeof(CFG_NFY_OBJ_T));
    if (*ppt_nfy_obj == NULL)
    {
        return CFGR_OUT_OF_MEM;
    }

    i4_result = handle_alloc(
                            CFG_NOTIFY_HANDLE,
                            (VOID*) *ppt_nfy_obj,
                            NULL,
                            cfg_free_notify_callback,
                            ph_notify
                            );
    if (i4_result != HR_OK)
    {
        x_mem_free(*ppt_nfy_obj);
        return CFGR_OUT_OF_HANDLE;
    }

    cfg_get_write_lock();

    /* insert into config list's list */
    (*ppt_nfy_obj)->pt_next = pt_config->t_nfy_list.pt_next;
    pt_config->t_nfy_list.pt_next = *ppt_nfy_obj;

    (*ppt_nfy_obj)->h_nfy = *ph_notify;

    cfg_unlock();
    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_dispatch_nfys
 *
 * Description: 
 *   Call all notification functions for the group
 *
 * Inputs: -
 *    pt_group
 *    e_condition
 *    ui2_id
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_INV_ARG
 *
 --------------------------------------------------------------------*/
INT32 cfg_dispatch_nfys(
                       CFG_LIST_T* pt_list,
                       CFG_COND_T  e_condition,
                       UINT16      ui2_id
                       )
{
    CFG_NFY_OBJ_T* pt_nfy;
    UINT16 ui2_group;

    if (pt_list == NULL)
    {
        pt_list = pt_configs;
    }

    if (pt_list == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    ui2_group = CFG_GET_GROUP(ui2_id);

    /* go through notification table */
    cfg_get_read_lock();
    for (pt_nfy = pt_list->t_nfy_list.pt_next; pt_nfy; pt_nfy = pt_nfy->pt_next)
    {
        if (pt_nfy->ui2_group == ui2_group && pt_nfy->pf_nfy)
        {
            pt_nfy->pf_nfy(
                           pt_nfy->h_nfy, 
                           e_condition,
                           pt_nfy->pv_tag, 
                           ui2_id
                           );
        }
    }
    cfg_unlock();

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_free_notify_callback
 *
 * Description: 
 *   Free a notify object and its handle.
 *
 * Inputs: -
 *    h_nfy_handle
 *    e_type
 *    pv_obj
 *    pv_tag
 *    b_req_handle
 *
 * Returns: 
 *    TRUE - always
 *
 --------------------------------------------------------------------*/
BOOL cfg_free_notify_callback(
                             HANDLE_T      h_nfy_handle,
                             HANDLE_TYPE_T e_type,
                             VOID*         pv_obj,
                             VOID*         pv_tag,
                             BOOL          b_req_handle
                             )
{
    CFG_LIST_T* pt_list;

    for (pt_list = pt_configs; pt_list; pt_list = pt_list->pt_next)
    {
        CFG_NFY_OBJ_T* pt_nfy;

        pt_nfy = &(pt_list->t_nfy_list); 
        while (pt_nfy->pt_next)
        {
            if(pt_nfy->pt_next->h_nfy == h_nfy_handle)
            {
                CFG_NFY_OBJ_T* pt_temp;
                pt_temp = pt_nfy->pt_next;
                pt_nfy->pt_next = pt_temp->pt_next;
                x_mem_free(pt_temp);
            }
            else
            {
                pt_nfy = pt_nfy->pt_next;
            }
        }
    }
    return TRUE;
}

/*---------------------------------------------------------------------
 * Name: cfg_calc_size
 *
 * Description: Calculate amount of space needed to store config
 *              settings for raw device.
 *
 * Outputs:
 *    pz_size           - pointer to variable to hold size
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_INV_HANDLE
 *    CFGR_INV_ARG
 *
 --------------------------------------------------------------------*/
INT32 cfg_calc_size(
                   CFG_LIST_T* pt_list,
                   SIZE_T*     pz_size
                   )
{
    INT32             i4_result;
    SIZE_T            z_size;
    CFG_GROUP_LIST_T* pt_group;
    CFG_REC_LIST_T*   pt_setting;
    SIZE_T            z_name_len;
    UINT32            ui4_persistent_size;
    UINT32            ui4_ver_id;
    
    z_size = 0;

    z_name_len = x_strlen(pt_list->as_name);
    if (z_name_len != 0)
    {
        z_size = z_name_len + 1; /* name len + name */
    }

    z_size += sizeof(UINT16);    /* number of groups */

    /* for each group */
    for (pt_group = pt_list->t_groups.pt_next_group;
        pt_group;
        pt_group = pt_group->pt_next_group)
    {
        /* number of settings */
        z_size += sizeof(UINT16);

        /* for each setting */
        for (pt_setting = pt_group->pt_next_rec;
            pt_setting; 
            pt_setting = pt_setting->pt_next)
        {
            /* setting ID, */
            z_size += sizeof(UINT16);

            /* field type, */
            z_size += sizeof(UINT32);

            /* num of elements */
            z_size += sizeof(UINT16);

            i4_result = db_get_persistent_size(pt_setting->h_db,
                                               (db_endian_conv_fct)NULL,
                                               &ui4_persistent_size,
                                               &ui4_ver_id);
            
            if (i4_result != DBR_OK)
            {
                return db_to_cfg_err(i4_result);
            }

            /* add in size that database takes */
            z_size += ui4_persistent_size;
        }
    }
    *pz_size = z_size;
    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_store_settings
 *
 * Description: 
 *
 * Inputs: -
 *    h_dir
 *    ps_pathname
 *    h_config
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_CANT_CREATE_FILE
 *    CFGR_NO_SUCH_FILE
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_EXIST
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 cfg_store_settings(
                        HANDLE_T h_dir,
                        CHAR*    ps_pathname,
                        HANDLE_T h_config
                        )
{
    INT32             i4_result;
    CFG_GROUP_LIST_T* pt_group;
    CFG_REC_LIST_T*   pt_setting;
    UINT16            ui2_num_groups;
    UINT16            ui2_num_settings;
    UINT32            ui4_bytes_written;
    HANDLE_T          h_file;
    UINT32            ui4_flags;
    UINT32            ui4_mode;
    FS_RAW_META_T     t_meta;
    UINT32            ui4_offset;
    SIZE_T            z_size;
    UINT64            ui8_position;
    CFG_LIST_T*       pt_list;
    CFG_OBJ_T*        pt_obj;
    UINT8             ui1_name_len;
    HANDLE_TYPE_T     e_type;

    if (h_config != NULL_HANDLE)
    {
        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }

        pt_list = pt_obj->pt_config;
    }
    else
    {
        pt_list = pt_configs;
    }

    if (pt_list == NULL)
    {
        return CFGR_REC_NOT_FOUND;
    }

    /* do prep necessary for raw file save */
    if (!fs_raw_is_inited(h_dir, ps_pathname))
    {
        i4_result = fs_raw_init(h_dir, ps_pathname);
        if (i4_result != FSRAWR_OK)
        {
            return CFGR_CANT_CREATE_FILE;
        }
    }

    /* open the file/device */
    ui4_flags = FM_READ_WRITE|FM_OPEN_CREATE;
    ui4_mode = 0660;
    i4_result = x_fm_open(h_dir, ps_pathname, ui4_flags, ui4_mode,
                          FALSE, &h_file);
    if (i4_result != FMR_OK)
    {
        return fm_to_cfg_err(i4_result);
    }

    i4_result = x_fm_lock(h_file, FM_LOCK_WRITE);
    if (i4_result != FMR_OK)
    {
        x_fm_close(h_file);
        return fm_to_cfg_err(i4_result);
    }

    /* calculate size */
    i4_result = cfg_calc_size(pt_list, &z_size);
    if (i4_result != CFGR_OK)
    {
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        return i4_result;
    }

    /* fs_raw_realloc */
    i4_result = fs_raw_realloc(
                              h_file,
                              (UINT32) FS_RAW_MAKE_OBJ_ID(CFG_GROUP_ID, pt_list->ui2_id),
                              z_size,
                              &t_meta,
                              &ui4_offset
                              );
    if (i4_result != FSRAWR_OK)
    {
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        if (i4_result == FSRAWR_OUT_OF_SPACE)
        {
            return CFGR_NO_FREE_SPACE;
        }
        return CFGR_CANT_CREATE_FILE;
    }

    /* x_fm_lseek */
    i4_result = x_fm_lseek(h_file, (INT64)ui4_offset, FM_SEEK_BGN, &ui8_position);
    if (i4_result != FMR_OK)
    {
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        return CFGR_CANT_CREATE_FILE;
    }

    /* write name and length if not 0 */
    ui1_name_len = (UINT8) x_strlen(pt_list->as_name);
    if (ui1_name_len != 0)
    {
        /* write name length */
        i4_result = x_fm_write(h_file,
                               &ui1_name_len,
                               sizeof(ui1_name_len), 
                               &ui4_bytes_written);
        if (i4_result != FMR_OK)
        {
            x_fm_unlock(h_file);
            x_fm_close(h_file);
            return fm_to_cfg_err(i4_result);
        }
    
        /* write name */
        i4_result = x_fm_write(h_file,
                               pt_list->as_name,
                               ui1_name_len, 
                               &ui4_bytes_written);
        if (i4_result != FMR_OK)
        {
            x_fm_unlock(h_file);
            x_fm_close(h_file);
            return fm_to_cfg_err(i4_result);
        }
    }

    /* get # of groups */
    /* write # of groups to file */
    ui2_num_groups = 0;
    for (pt_group = pt_list->t_groups.pt_next_group;
        pt_group; 
        pt_group = pt_group->pt_next_group)
    {
        ++ui2_num_groups;
    }

    ui2_num_groups = CONV_STD_END_TO_UINT16(ui2_num_groups);
    i4_result = x_fm_write(h_file,
                           &ui2_num_groups,
                           sizeof(ui2_num_groups), 
                           &ui4_bytes_written);
    if (i4_result != FMR_OK)
    {
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        return fm_to_cfg_err(i4_result);
    }

    /* for all groups */
    for (pt_group = pt_list->t_groups.pt_next_group;
        pt_group; 
        pt_group = pt_group->pt_next_group)
    {
        /* get # of settings in group */
        ui2_num_settings = 0;
        for (pt_setting = pt_group->pt_next_rec;
            pt_setting;
            pt_setting = pt_setting->pt_next)
        {
            ++ui2_num_settings;
        }

        /* write # of settings in group to file */
        ui2_num_settings = CONV_STD_END_TO_UINT16(ui2_num_settings);
        i4_result = x_fm_write(h_file, 
                               &ui2_num_settings, 
                               sizeof(ui2_num_settings),
                               &ui4_bytes_written);
        if (i4_result != FMR_OK)
        {
            x_fm_unlock(h_file);
            x_fm_close(h_file);
            return fm_to_cfg_err(i4_result);
        }

        /* for all settings */
        for (pt_setting = pt_group->pt_next_rec;
            pt_setting;
            pt_setting = pt_setting->pt_next)
        {
            UINT16 ui2_id;
            UINT16 ui2_num;
            UINT32 ui4_field_type;

            /* write ID */
            ui2_id = CONV_STD_END_TO_UINT16(pt_setting->ui2_id);
            i4_result = x_fm_write(h_file, 
                                   &ui2_id, 
                                   sizeof(ui2_id),
                                   &ui4_bytes_written);
            if (i4_result != FMR_OK)
            {
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }

            /* write field type */
            ui4_field_type = CONV_STD_END_TO_UINT32(pt_setting->ui4_field_type);
            i4_result = x_fm_write(h_file, 
                                   &ui4_field_type, 
                                   sizeof(ui4_field_type),
                                   &ui4_bytes_written);
            if (i4_result != FMR_OK)
            {
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }

            /* write number of field type */
            ui2_num = CONV_STD_END_TO_UINT16(pt_setting->ui2_num);
            i4_result = x_fm_write(h_file, 
                                   &ui2_num,
                                   sizeof(ui2_num),
                                   &ui4_bytes_written);
            if (i4_result != FMR_OK)
            {
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }

            i4_result = db_save_to_file(
                                       pt_setting->h_db,
                                       h_file,
                                       NULL,
                                       NULL
                                       );
            if (i4_result != DBR_OK)
            {
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return db_to_cfg_err(i4_result);
            }
        }
    }
    x_fm_unlock(h_file);
    x_fm_close(h_file);

    return CFGR_OK;
}

/*---------------------------------------------------------------------
 * Name: cfg_load_settings
 *
 * Description: 
 *
 * Inputs:
 *    h_dir
 *    ps_pathname
 *    ps_config_name
 *
 * Outputs:
 *    ph_config
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_SUCH_FILE
 *    CFGR_CANT_OPEN_FILE
 *    CFGR_OUT_OF_MEM
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_EXIST
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 cfg_load_settings(
                       HANDLE_T    h_dir,
                       CHAR*       ps_pathname,
                       const CHAR* ps_config_name,
                       HANDLE_T*   ph_config
                       )
{
    INT32             i4_result;
    UINT16            ui2_group;
    UINT16            ui2_settings;
    UINT32            ui4_bytes_read;
    UINT16            ui2_num_groups;
    UINT16            ui2_num_settings;
    DB_FIELD_INFO_T   t_field_info[2];
    UINT32            ui4_flags;
    UINT32            ui4_mode;
    HANDLE_T          h_file;
    CFG_LIST_T*       pt_cfg_list;
    CFG_OBJ_T*        pt_obj;
    CHAR*             ps_fake_name = "fake_name";

    if (ph_config != NULL)
    {
        /* allocate handle and object for caller */
        i4_result = cfg_alloc_handle(ph_config, NULL, &pt_obj);
        if (i4_result)
        {
            return i4_result;
        }

        /* see if a config by this name is already in RAM */
        i4_result = cfg_find_config(ps_config_name, &pt_cfg_list);
        if (i4_result != CFGR_OK)
        {
            /* there must be a config in RAM */
            x_handle_free(*ph_config);
            return i4_result;
        }
        pt_obj->pt_config = pt_cfg_list;
    }
    else
    {
        pt_cfg_list = pt_configs;
    }

    if (pt_cfg_list == NULL)
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        return CFGR_REC_NOT_FOUND;
    }

    /* do prep necessary for raw file load */
    if (!fs_raw_is_inited(h_dir, ps_pathname))
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        return CFGR_INV_FILE_PATH;
    }

    /* open the file */
    ui4_flags = FM_READ_ONLY;
    ui4_mode = 0440;
    i4_result = x_fm_open(
                         h_dir,
                         ps_pathname, 
                         ui4_flags,
                         ui4_mode,
                         FALSE,
                         &h_file);
    if (i4_result != FMR_OK)
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        return fm_to_cfg_err(i4_result);
    }

    /* get read lock */
    i4_result = x_fm_lock(h_file, FM_LOCK_READ);
    if (i4_result != FMR_OK)
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        x_fm_close(h_file);
        return fm_to_cfg_err(i4_result);
    }

    /* seek to CFG object */
    i4_result = cfg_fs_find_config(h_file, ps_config_name);

    if (i4_result)
    {
        /* allegedly should never cause an error */
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        return fsraw_to_cfg_err(i4_result);
    }

    /* get # of groups */
    i4_result = x_fm_read(h_file, 
                          &ui2_num_groups, 
                          sizeof(ui2_num_groups), 
                          &ui4_bytes_read);
    if (i4_result != FMR_OK)
    {
        if (ph_config != NULL)
        {
            x_handle_free(*ph_config);
        }
        x_fm_unlock(h_file);
        x_fm_close(h_file);
        return fm_to_cfg_err(i4_result);
    }
    ui2_num_groups = CONV_STD_END_TO_UINT16(ui2_num_groups);

    /* for all groups */
    for (ui2_group = 0;ui2_group < ui2_num_groups; ++ui2_group)
    {
        UINT16    ui2_rec_id;

        /* read in num of settings in group */
        i4_result = x_fm_read(h_file, 
                              &(ui2_num_settings), 
                              sizeof(ui2_num_settings),
                              &ui4_bytes_read);
        if (i4_result != FMR_OK)
        {
            if (ph_config != NULL)
            {
                x_handle_free(*ph_config);
            }
            x_fm_unlock(h_file);
            x_fm_close(h_file);
            return fm_to_cfg_err(i4_result);
        }
        ui2_num_settings = CONV_STD_END_TO_UINT16(ui2_num_settings);

        t_field_info[0].ui4_field_type =  (DB_FIELD_T) CFG_KEY_FIELD_TYPE;
        t_field_info[0].ui2_field_element_count = 1;

        for (ui2_settings = 0; ui2_settings < ui2_num_settings; ++ui2_settings)
        {
            HANDLE_T        h_db;
            CFG_REC_LIST_T* pt_rec;
            BOOL            b_keep;
            UINT32          ui4_key_field_type;
            UINT32          ui4_data_field_type;
            UINT32          ui4_version;
            UINT8           ui1_key = 0;
            VOID*           apv_keyfield_addr[1];

            /* read in ID */
            i4_result = x_fm_read(h_file, 
                                  &ui2_rec_id,
                                  sizeof(ui2_rec_id),
                                  &ui4_bytes_read);
            if (i4_result != FMR_OK)
            {
                if (ph_config != NULL)
                {
                    x_handle_free(*ph_config);
                }
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }
            ui2_rec_id = CONV_STD_END_TO_UINT16(ui2_rec_id);

            /* read in field type */
            i4_result = x_fm_read(h_file,
                                  &(t_field_info[1].ui4_field_type),
                                  sizeof(t_field_info[1].ui4_field_type), 
                                  &ui4_bytes_read);
            if (i4_result != FMR_OK)
            {
                if (ph_config != NULL)
                {
                    x_handle_free(*ph_config);
                }
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }
            t_field_info[1].ui4_field_type = CONV_STD_END_TO_UINT32(t_field_info[1].ui4_field_type);

            /* read in number of field type */
            i4_result = x_fm_read(h_file, 
                                  &(t_field_info[1].ui2_field_element_count),
                                  sizeof(t_field_info[1].ui2_field_element_count), 
                                  &ui4_bytes_read);
            if (i4_result != FMR_OK)
            {
                if (ph_config != NULL)
                {
                    x_handle_free(*ph_config);
                }
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return fm_to_cfg_err(i4_result);
            }
            t_field_info[1].ui2_field_element_count = CONV_STD_END_TO_UINT16(t_field_info[1].ui2_field_element_count);

            i4_result =  cfg_get_rec_obj(pt_cfg_list, ui2_rec_id, &pt_rec);
            if (i4_result)
            {
                if (ph_config != NULL)
                {
                    x_handle_free(*ph_config);
                }
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return (i4_result);
            }

            /* check if compatible type/count */
            /* if not compatible, mark to skip it */
            b_keep = TRUE;

            if ((pt_rec->ui4_field_type != t_field_info[1].ui4_field_type) ||
                (pt_rec->ui2_num != t_field_info[1].ui2_field_element_count))
            {
                b_keep = FALSE;
            }

            /* make sure there isn't any */
            db_delete(h_db, ps_fake_name);

            i4_result = db_load_from_file(
                                         ps_fake_name,
                                         2,
                                         t_field_info,
                                         1,
                                         &(t_field_info[0].ui4_field_type),
                                         h_file,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         &h_db);  

            if (i4_result != DBR_OK)
            {
                if (ph_config != NULL)
                {
                    x_handle_free(*ph_config);
                }
                x_fm_unlock(h_file);
                x_fm_close(h_file);
                return db_to_cfg_err(i4_result);
            }

            /* update record object with new settings */
            if (b_keep)
            {
                UINT8*  pui1_buff;
                SIZE_T  z_len;

                i4_result = cfg_get_field_size(pt_rec->ui4_field_type,
                                               &z_len);
                if (i4_result != CFGR_OK)
                {
                    if (ph_config != NULL)
                    {
                        x_handle_free(*ph_config);
                    }
                    x_fm_unlock(h_file);
                    x_fm_close(h_file);
                    db_delete(h_db, ps_fake_name);
                    return i4_result;
                }

                z_len *= pt_rec->ui2_num;
                pui1_buff = (UINT8*)x_mem_calloc(1, z_len);
                if (pui1_buff == NULL)
                {
                    if (ph_config != NULL)
                    {
                        x_handle_free(*ph_config);
                    }
                    x_fm_unlock(h_file);
                    x_fm_close(h_file);
                    db_delete(h_db, ps_fake_name);
                    return i4_result;
                }

                ui4_key_field_type = (UINT32) CFG_KEY_FIELD_TYPE;
                ui4_data_field_type = pt_rec->ui4_field_type;
                apv_keyfield_addr[0] = &ui1_key;
                ui4_version = CDB_NULL_VER_ID;
                i4_result = db_read_rec(h_db,
                                        1,
                                        &ui4_key_field_type,
                                        (const VOID**) apv_keyfield_addr,
                                        NULL,
                                        1,
                                        &ui4_data_field_type,
                                        (VOID**) &pui1_buff,
                                        0,
                                        &ui4_version);
                if (i4_result != DBR_OK)
                {
                    if (ph_config != NULL)
                    {
                        x_handle_free(*ph_config);
                    }
                    x_fm_unlock(h_file);
                    x_fm_close(h_file);
                    x_mem_free(pui1_buff);
                    db_delete(h_db, ps_fake_name);
                    return db_to_cfg_err(i4_result);
                }

                if (ph_config != NULL)
                {
                    i4_result = x_cfg_set(*ph_config,
                                          ui2_rec_id,
                                          pui1_buff,
                                          z_len);
                }
                else
                {
                    i4_result = x_cfg_set(NULL_HANDLE,
                                          ui2_rec_id,
                                          pui1_buff,
                                          z_len);
                }

                if (i4_result != CFGR_OK)
                {
                    if (ph_config != NULL)
                    {
                        x_handle_free(*ph_config);
                    }
                    x_fm_unlock(h_file);
                    x_fm_close(h_file);
                    x_mem_free(pui1_buff);
                    db_delete(h_db, ps_fake_name);
                    return i4_result;
                }

                x_mem_free(pui1_buff);

                /* since we loaded into a fake db, we need to
                   update the real db with the version number */
                db_set_db_ver_num(pt_rec->h_db, ui4_version);

                /* send loaded notifications */
                i4_result =  cfg_dispatch_nfys(pt_cfg_list, CFG_LOADED, ui2_rec_id);
                if (i4_result != CFGR_OK)
                {
                    if (ph_config != NULL)
                    {
                        x_handle_free(*ph_config);
                    }
                    x_fm_unlock(h_file);
                    x_fm_close(h_file);
                    db_delete(h_db, ps_fake_name);
                    return i4_result;
                }
            }

            db_delete(h_db, ps_fake_name);
        }
    }

    x_fm_unlock(h_file);
    x_fm_close(h_file);
    return CFGR_OK;
}

#ifdef CLI_LVL_ALL
/*---------------------------------------------------------------------
 * Name: cfg_get_dbg_level
 *
 * Description: 
 *
 * Inputs: -
 *    argc     - number of args
 *    pps_argv - pointer to array of args
 *
 * Returns: -
 *    CFG_OK - success
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_dbg_level(
    INT32 i4_argc,
    const CHAR** pps_argv
    )
{
    INT32 i4_result;

    i4_result = x_cli_show_dbg_level(ui2_cfg_debug_level);
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: cfg_set_dbg_level
 *
 * Description: 
 *
 * Inputs: -
 *    argc     - number of args
 *    pps_argv - pointer to array of args
 *
 * Returns: -
 *    CFG_OK - success
 *
 --------------------------------------------------------------------*/
INT32 cfg_set_dbg_level(
    INT32 i4_argc,
    const CHAR** pps_argv
    )
{
    INT32  i4_result;
    UINT16 ui2_i;

    i4_result = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_i);
    if (i4_result == CLIR_OK)
    {
        ui2_cfg_debug_level = ui2_i;
    }
    return CFGR_OK;
}
#endif

/*-------------------------------------------------------------------------
 * Name: cfg_free_callback
 *
 * Description: This function gets called when a configuration handle
 *              is freed
 *
 * Inputs: -
 *     h_fav_handle - handle to configuration
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
 *         The attached data structure is freed. The configuration
 *         is NOT freed.
 *
 * Exceptions:
 *         Aborts if handle is wrong type
 *
 ------------------------------------------------------------------------*/
BOOL cfg_free_callback
(
    HANDLE_T      h_fav_handle,
    HANDLE_TYPE_T e_type,
    VOID*         pv_obj,
    VOID*         pv_tag,
    BOOL          b_req_handle
)
{
    CFG_OBJ_T* pt_obj;

    if (e_type != CFG_HANDLE)
    {
        CFG_ABORT(DBG_CAT_HANDLE, 2);
        return FALSE;
    }

    pt_obj = (CFG_OBJ_T *) pv_obj;
    if (pt_obj != NULL)
    {
        x_mem_free(pt_obj);
    }
    return TRUE;
}

/*-------------------------------------------------------------------------
 * Name: cfg_alloc_handle
 *
 * Description: allocates a handle to a configuration list
 *
 * Inputs:
 *         pv_tag       - allowed to be NULL?
 *
 * Outputs:
 *         ph_list      - handle to config list
 *         ppt_list     - pointer to pointer to list object data
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_OUT_OF_MEM
 *    CFGR_OUT_OF_HANDLE
 *    CFGR_INV_ARG       - ppt_list or ph_list are NULL
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_alloc_handle(
    HANDLE_T*    ph_config,
    VOID*        pv_tag,
    CFG_OBJ_T**  ppt_obj
    )
{
    INT32 i4_result;

    if (ppt_obj == NULL || ph_config == NULL )
    {
        return CFGR_INV_ARG;
    }

    *ppt_obj = (CFG_OBJ_T*)x_mem_calloc(1, sizeof(CFG_OBJ_T));
    if (*ppt_obj == NULL)
    {
        return CFGR_OUT_OF_MEM;
    }

    i4_result = handle_alloc(CFG_HANDLE,
                             (VOID*) *ppt_obj,
                             pv_tag,
                             cfg_free_callback,
                             ph_config);
    if (i4_result != HR_OK)
    {
        x_mem_free(*ppt_obj);
        *ph_config = NULL_HANDLE;
        return CFGR_OUT_OF_HANDLE;
    }

    return CFGR_OK;
}

/*-------------------------------------------------------------------------
 * Name: cfg_create_config
 *
 * Description: creates a configuration
 *
 * Inputs:
 *         ps_config_name - Name to attach to configuration
 *
 * Outputs:
 *         ppt_list       - pointer to variable to hold list pointer
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_OUT_OF_MEM
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_create_config(const CHAR* ps_config_name, CFG_LIST_T** ppt_list)
{
    CFG_LIST_T* pt_list;

    pt_list = (CFG_LIST_T*)x_mem_calloc(1, sizeof(CFG_LIST_T));
    if (pt_list == NULL)
    {
        return CFGR_OUT_OF_MEM;
    }

    

    if (ps_config_name == NULL)
    {
        pt_list->ui2_id = 1;
    }
    else
    {
        pt_list->ui2_id = ui2_cfg_num++;
        if (ui2_cfg_num == 0)
        {
            x_mem_free(pt_list);
            return CFGR_INV_NAME;
        }

        /* only copy the ps_config_name to pt_list structure if it is
           NULL.  The previous code depending on the behavior of
           x_strcpy(..) to check for NULL parameter.
        */
        x_strcpy(pt_list->as_name, ps_config_name);
    }

    /* set persistent storage APIs function address to NULL */
    x_memset(&(pt_list->t_file_fct_tbl), 0, sizeof(CFG_FILE_API_T));
    
    pt_list->pt_next = pt_configs;
    pt_configs = pt_list;
    if (ppt_list != NULL)
    {
        *ppt_list = pt_list;
    }
    return CFGR_OK;
}

/*-------------------------------------------------------------------------
 * Name: cfg_del_config
 *
 * Description: deletes a configuration
 *
 * Inputs:
 *         pt_cfg       - pointer to configuration to remove
 *
 * Returns:
 *    CFGR_OK
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_del_config(CFG_LIST_T* pt_cfg)
{
    CFG_LIST_T*       pt_list;

    if (pt_cfg)
    {
        if (pt_configs == pt_cfg)
        {
            pt_configs = NULL;
        }
        else
        {
            for(pt_list = pt_configs; pt_list; pt_list = pt_list->pt_next)
            {
                /* find the config so we can remove it from the list */
                if (pt_list->pt_next == pt_cfg)
                {
                    pt_list->pt_next = pt_cfg->pt_next;
                }
            }
        }

        /* now free associated config */
        cfg_purge_recs(pt_cfg);

        x_mem_free(pt_cfg);
    }
    return CFGR_OK;
}

/*-------------------------------------------------------------------------
 * Name: cfg_find_config
 *
 * Description: finds a configuration
 *
 * Inputs:
 *         ps_config_name - Name of configuration to find
 *         ppt_list       - pointer to variable to return pointer in
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_find_config(const CHAR* ps_config_name, CFG_LIST_T** ppt_list)
{
    CFG_LIST_T* pt_list;

    for (pt_list = pt_configs; pt_list; pt_list = pt_list->pt_next)
    {
        if (x_strcmp(pt_list->as_name, ps_config_name) == 0)
        {
            *ppt_list = pt_list;
            return CFGR_OK;
        }
    }
    return CFGR_REC_NOT_FOUND;    
}

/*-------------------------------------------------------------------------
 * Name: cfg_fs_find_config
 *
 * Description: finds a configuration in a raw file and leaves file
 *              position at the point just beyond the config name
 *
 * Inputs:
 *         h_file         - handle to file
 *         ps_config_name - Name of configuration to find
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_fs_find_config(HANDLE_T h_file, const CHAR* ps_config_name)
{
    UINT16        ui2_i;
    UINT32        ui4_offset;
    UINT32        ui4_size;
    UINT32        ui4_bytes_read;
    FS_RAW_META_T t_meta;
    UINT8         ui1_len;
    CHAR          ps_name[CFG_NAME_LEN];
    INT32         i4_result;

    if (ps_config_name == NULL)
    {
        /* move file pointer */
        i4_result = fs_raw_seek_obj(h_file, &t_meta,
                                    (UINT32) FS_RAW_MAKE_OBJ_ID(CFG_GROUP_ID, 1),
                                    &ui4_offset,
                                    &ui4_size);
        return fsraw_to_cfg_err(i4_result);
    }
    else
    {
        for (ui2_i = 2; ui2_i < ui2_cfg_num; ++ui2_i)
        {
            /* move file pointer */
            i4_result = fs_raw_seek_obj(h_file, &t_meta,
                                        (UINT32) FS_RAW_MAKE_OBJ_ID(CFG_GROUP_ID, ui2_i),
                                        &ui4_offset,
                                        &ui4_size);
            if (i4_result == FSRAWR_NOT_FOUND)
            {
                continue;
            }

            if (i4_result)
            {
                return fsraw_to_cfg_err(i4_result);
            }
    
            /* read name length */
            i4_result = x_fm_read(h_file, 
                                  &ui1_len, 
                                  sizeof(ui1_len), 
                                  &ui4_bytes_read);
            if (i4_result)
            {
                return fm_to_cfg_err(i4_result);
            }
    
            if (ui1_len >= CFG_NAME_LEN)
            {
                return CFGR_INV_NAME;
            }
    
            /* read name */
            i4_result = x_fm_read(h_file, 
                                  &ps_name, 
                                  ui1_len, 
                                  &ui4_bytes_read);
            if (i4_result)
            {
                return fm_to_cfg_err(i4_result);
            }
    
            ps_name[ui1_len] = '\0';
            if (x_strcmp(ps_name , ps_config_name) == 0)
            {
                return CFGR_OK;
            }
        }
    }
    return CFGR_NO_SUCH_FILE;
}

/*-------------------------------------------------------------------------
 * Name: cfg_fs_del_config
 *
 * Description: finds a configuration in a raw file and deletes it
 *
 * Inputs:
 *         h_file         - handle to file
 *         ps_config_name - Name of configuration to find
 *
 * Returns:
 *    CFGR_OK
 *    CFGR_REC_NOT_FOUND
 *
 * Exceptions:
 *    None
 *
 ------------------------------------------------------------------------*/
INT32 cfg_fs_del_config(HANDLE_T h_file, const CHAR* ps_config_name)
{
    UINT16        ui2_i;
    UINT32        ui4_offset;
    UINT32        ui4_size;
    UINT32        ui4_bytes_read;
    FS_RAW_META_T t_meta;
    UINT8         ui1_len;
    CHAR          ps_name[CFG_NAME_LEN];
    INT32         i4_result;
    UINT32        ui4_id;

    if (ps_config_name == NULL)
    {
        ui4_id = (UINT32) FS_RAW_MAKE_OBJ_ID(CFG_GROUP_ID, 1);
        i4_result = fs_raw_del_obj(h_file, &t_meta, ui4_id);
        return fsraw_to_cfg_err(i4_result);
    }
    else
    {
        for (ui2_i = 2; ui2_i < ui2_cfg_num; ++ui2_i)
        {
            ui4_id = (UINT32) FS_RAW_MAKE_OBJ_ID(CFG_GROUP_ID, ui2_i);
    
            /* move file pointer */
            i4_result = fs_raw_seek_obj(h_file,
                                        &t_meta,
                                        ui4_id,
                                        &ui4_offset,
                                        &ui4_size);
            
            if (i4_result == FSRAWR_NOT_FOUND)
            {
                continue;
            }

            if (i4_result)
            {
                return fsraw_to_cfg_err(i4_result);
            }
    
            /* read name length */
            i4_result = x_fm_read(h_file, 
                                  &ui1_len, 
                                  sizeof(ui1_len), 
                                  &ui4_bytes_read);
            if (i4_result)
            {
                return fm_to_cfg_err(i4_result);
            }
    
            if (ui1_len >= CFG_NAME_LEN)
            {
                return CFGR_INV_NAME;
            }
    
            /* read name */
            i4_result = x_fm_read(h_file, 
                                  &ps_name, 
                                  ui1_len, 
                                  &ui4_bytes_read);
            if (i4_result)
            {
                return fm_to_cfg_err(i4_result);
            }
    
            ps_name[ui1_len] = '\0';
            if (x_strcmp(ps_name , ps_config_name) == 0)
            {
                i4_result = fs_raw_del_obj(h_file, &t_meta, ui4_id);
                return fsraw_to_cfg_err(i4_result);
            }
        }
    }
    return CFGR_NO_SUCH_FILE;    
}



/*---------------------------------------------------------------------
 * Name: cfg_get_custom_fct_tbl
 *
 * Description: 
 *    Get the application registered function table for performing
 *    IO operation to persistent storage.
 *
 * Inputs:
 *    ps_config_name:   Specified the config name to search for the
 *                      registered function table. If NULL then,
 *                      the default configuration is used.
 *  
 * Outputs:
 *    pt_fct_tbl        Pointer to a caller registered function table
 *                      containing custom file IO operation.
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_SUCH_FILE
 *    CFGR_CANT_OPEN_FILE
 *    CFGR_OUT_OF_MEM
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_EXIST
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_custom_fct_tbl
(
    const CHAR*       ps_config_name,
    CFG_FILE_API_T*   pt_fct_tbl
)
{
    INT32             i4_result = CFGR_OK;
    CFG_LIST_T*       pt_cfg_list = NULL;

    x_memset(pt_fct_tbl,0,sizeof(CFG_FILE_API_T));

    if ( ps_config_name == NULL )
    {
        pt_cfg_list = pt_configs;
    }
    else
    {
        /* See if a config by this name is already in RAM */
        i4_result = cfg_find_config(ps_config_name, &pt_cfg_list);
    }
    
    if ( (i4_result   == CFGR_OK)                          &&
         (pt_cfg_list != NULL )                            &&
         (pt_cfg_list->t_file_fct_tbl.pf_load  != NULL )   &&
         (pt_cfg_list->t_file_fct_tbl.pf_store != NULL )
        )
    {
        x_memcpy(pt_fct_tbl,
                 &(pt_cfg_list->t_file_fct_tbl),
                 sizeof(CFG_FILE_API_T));
    }   
    
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: cfg_get_config_from_handle
 *
 * Description: 
 *    Get the config list structure from the specified handle.
 *
 * Inputs:
 *    h_config:   Specified the handle to config database.
 *  
 * Outputs:
 *    ppt_cfg_list  Ref to a pointer to the CFG_LIST_T structure
 *                  associate with the specified handle.
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_NO_SUCH_FILE
 *    CFGR_CANT_OPEN_FILE
 *    CFGR_OUT_OF_MEM
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *    CFGR_NOT_DIR
 *    CFGR_CORE
 *    CFGR_NAME_TOO_LONG
 *    CFGR_EXIST
 *    CFGR_DEVICE_ERROR
 *
 --------------------------------------------------------------------*/
INT32 cfg_get_config_from_handle
(
    HANDLE_T          h_config,
    CFG_LIST_T**      ppt_cfg_list
)
{
    INT32             i4_result = CFGR_OK;
    CFG_OBJ_T*        pt_obj = NULL;
    HANDLE_TYPE_T     e_type;
    CRIT_STATE_T      t_state;

    *ppt_cfg_list = NULL;
    
    if (h_config != NULL_HANDLE)
    {
        t_state=x_crit_start(); 
        i4_result = handle_get_type_obj(h_config, &e_type, (VOID**) &pt_obj);
        if (i4_result != HR_OK || (e_type != CFG_HANDLE) || (pt_obj == NULL))
        {
            x_crit_end(t_state);
#ifdef DEBUG
            if(x_handle_valid(h_config))
            {
                CFG_ABORT(DBG_CAT_HANDLE, 1);
            }
#endif
            return CFGR_INV_HANDLE;
        }

        *ppt_cfg_list = pt_obj->pt_config;
        x_crit_end(t_state);
    }
    else
    {
        *ppt_cfg_list = pt_configs;
    }
    return i4_result;
}

/*---------------------------------------------------------------------
 * Name: cfg_load_settings_by_custom_fct
 *
 * Description: 
 *
 * Inputs:
 *    h_dir            directory handle.
 *    ps_pathname      pathname to the persistent storage location.
 *    ps_config_name   configuration name to load
 *    pt_fct_tbl       client specified function table.
 *
 * Outputs:
 *    ph_config        handle to the config database create from
 *                     persistent storage.
 *
 * Returns: -
 *    CFGR_OK
 *    CFGR_CANT_OPEN_FILE
 *    CFGR_OUT_OF_MEM
 *    CFGR_INV_ARG
 *    CFGR_INV_HANDLE
 *
 *
 --------------------------------------------------------------------*/

#define CFG_FREE_PTR_HANDLE(_ph_hdle_) {if(_ph_hdle_ != NULL){x_handle_free(*_ph_hdle_);}}

INT32 cfg_load_settings_by_custom_fct
(
    HANDLE_T               h_dir,
    CHAR*                  ps_pathname,
    const CHAR*            ps_config_name,
    const CFG_FILE_API_T*  pt_fct_tbl,
    HANDLE_T*              ph_config
)
{
    INT32                  i4_result;
#if 0
    HANDLE_T               h_config = NULL_HANDLE;
    CFG_LIST_T*            pt_cfg_list = NULL;
    CFG_OBJ_T*             pt_obj = NULL;


    /* custom load function is reponsible for allocating
       a new handle.
    */
    if (ph_config != NULL)
    {
        /* allocate handle and object for caller */
        i4_result = cfg_alloc_handle(ph_config, NULL, &pt_obj);
        if (i4_result)
        {
            return i4_result;
        }

        /* see if a config by this name is already in RAM */
        i4_result = cfg_find_config(ps_config_name, &pt_cfg_list);
        if (i4_result != CFGR_OK)
        {
            /* there must be a config in RAM */
            CFG_FREE_PTR_HANDLE(ph_config);
            return i4_result;
        }
        pt_obj->pt_config = pt_cfg_list;
        h_config = *ph_config;
    }
#endif
    
    DBG_API(("{CFG} load config data to config name [%s] using registered function.\n",ps_config_name));

    i4_result = pt_fct_tbl->pf_load(h_dir,
                                    ps_pathname,
                                    ps_config_name,
                                    ph_config,
                                    pt_fct_tbl->pv_load_tag);
#if 0
    if (i4_result != CFGR_OK)
    {
        CFG_FREE_PTR_HANDLE(ph_config);
    }
#endif
    return i4_result;
}

