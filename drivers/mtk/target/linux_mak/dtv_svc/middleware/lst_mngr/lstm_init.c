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
 * $RCSfile: lst_mngr_init.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements initialization function of List Manager (lst_mngr)
 *    module.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "lst_mngr/lst_mngr.h"
#include "lst_mngr/_lstm.h"
#include "lst_mngr/lstm_def_db.h"

/* Internal static variable. */
static UINT16          _s_ui2_max_lst_obj_per_type = 0;

/* Internal array for the list handles. */
static LST_ID_HANDLE_T*  pt_lst_type_id_handle[LST_MNGR_NUM_LIST_TYPES] = {NULL};

/* Internal global lock. */
static HANDLE_T        h_lst_mngr_lock;

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_init
 *
 * Description: This function initializes the lst_mngr library. 
 *
 * Inputs: 
 *    ui2_max_num_of_lst_id:    Maximum number of list ID's that one list
 *                              type can manage.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           lst_mngr module is initialized successfully.
 *   LSTMR_INV_ARG      'ui2_max_num_of_lst_id' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 lst_mngr_init 
(
    UINT16           ui2_max_num_of_lst_id
)
{
    if ( ui2_max_num_of_lst_id == 0 )
    {
        ui2_max_num_of_lst_id = 8;
    }
    
    if ( ui2_max_num_of_lst_id > 0 )
    {
        INT32           ui4_i;

        /* allocate array for storing list handles for each list type. */
        for (ui4_i = 0; ui4_i < LST_MNGR_NUM_LIST_TYPES; ui4_i++)
        {
            pt_lst_type_id_handle[ui4_i] = (LST_ID_HANDLE_T*) x_mem_calloc (
                    ui2_max_num_of_lst_id,
                    sizeof(LST_ID_HANDLE_T));
            if ( pt_lst_type_id_handle[ui4_i] == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
            x_memset(pt_lst_type_id_handle[ui4_i], 0, 
                     sizeof(LST_ID_HANDLE_T) * ui2_max_num_of_lst_id);
        }

        _s_ui2_max_lst_obj_per_type = ui2_max_num_of_lst_id;

        if (x_sema_create( &(h_lst_mngr_lock), X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
        }

        lstm_default_db_init(ui2_max_num_of_lst_id);

#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
        /*
           Initialize CLI support
           */
        lst_mngr_cli_init();
#endif   
    }
    else
    {
        return LSTMR_INV_ARG ;
    }
    
    return LSTMR_OK;
}

/**********************************************************************
   Following are private functions only known to the lst_mngr library.
   The private functions are for adding new handle reference to list of
   specified type and ID, deleting handle reference, from the internal
   lst_mngr list handle array.  They are in this file because these private
   functions need to access the internal lst_mngr list handle arrays

***********************************************************************/

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_add_lst_type_id_handle
 *
 * Description: This function add new list ID/handle pair entry to the internal
 *              lst_mngr module. 
 *
 * Inputs: 
 *    e_type:           List type.
 *    ui2_lst_id:       List ID.
 *    h_handle:         List handle.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK           List ID/handle is added.
 *   LSTMR_OUT_OF_SPACE Can not add the new List ID/handle.
 ----------------------------------------------------------------------------*/
INT32 lst_mngr_add_lst_type_id_handle(LST_MNGR_LST_TYPE_T e_type,
                                      UINT16            ui2_lst_id,
                                      HANDLE_T          h_handle)
{
    INT16           ui2_i;

    if ( ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES )
    {
        return LSTMR_INV_ARG;
    }
    
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lst_obj_per_type ; ui2_i++ )
    {
        /* Look for an empty slot. */
        if ( pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id == LST_NULL_ID )
        {
            pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id = ui2_lst_id;
            pt_lst_type_id_handle[e_type][ui2_i].h_handle = h_handle;
            return LSTMR_OK;
        }
    }
    return LSTMR_OUT_OF_SPACE;
}

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_check_lst_type_id
 *
 * Description: This function check the list ID entry of specified type. 
 *
 * Inputs: 
 *    e_type:           List type.
 *    ui2_lst_id:       List ID.
 *
 * Outputs: None
 *
 * Returns: 
 *   TRUE      lst_mngr id is not currently in the database.
 *   FALSE     lst_mngr id already existed.
 ----------------------------------------------------------------------------*/
BOOL lst_mngr_check_lst_type_id(LST_MNGR_LST_TYPE_T e_type,
                                UINT16            ui2_lst_id)
{
    INT16 ui2_i;
    
    if ( ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES )
    {
        return FALSE;
    }

    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lst_obj_per_type ; ui2_i++ )
    {
        if ( pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id == ui2_lst_id )
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_del_type_id
 *
 * Description: This function deletes list ID/handle entry from the internal
 *              lst_mngr module. 
 *
 * Inputs: 
 *    e_type:           List type.
 *    ui2_lst_id:       List ID.
 *
 * Outputs: None
 *
 * Returns: 
 *   LSTMR_OK          List entry is deleted.
 *   LSTMR_NOT_FOUND   Specified List entry not found.
 ----------------------------------------------------------------------------*/
INT32 lst_mngr_del_type_id(LST_MNGR_LST_TYPE_T e_type,
                           UINT16            ui2_lst_id)
{
    INT16 ui2_i;
    INT32 i4_rc;

    if ( ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES )
    {
        return LSTMR_INV_ARG;
    }
    i4_rc = LSTMR_NOT_FOUND;
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lst_obj_per_type ; ui2_i++ )
    {
        if ( pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id == ui2_lst_id )
        {
            pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id = LST_NULL_ID;
            pt_lst_type_id_handle[e_type][ui2_i].h_handle   = NULL_HANDLE;
            i4_rc = LSTMR_OK;
            break;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_get_handle_by_type_id
 *
 * Description: This function search for specific List type & ID and return the
 *              list handle.
 *
 * Inputs: 
 *    e_type:           List type.
 *    ui2_lst_id:       List ID.
 *
 * Outputs:
 *    ph_handle:        List handle.
 *
 * Returns: 
 *   TRUE       specified lst_mngr_id is found.
 *   FALSE      specified lst_mngr id is not found.
 ----------------------------------------------------------------------------*/
BOOL lst_mngr_get_handle_by_type_id(LST_MNGR_LST_TYPE_T e_type,
                                    UINT16            ui2_lst_id,
                                    HANDLE_T*         ph_handle)
{
    INT16 ui2_i;

    /* 0 is not a valid value. */
    if ( ui2_lst_id == LST_NULL_ID || e_type >= LST_MNGR_NUM_LIST_TYPES )
    {
        return FALSE;
    }
    for ( ui2_i = 0 ; ((UINT16)ui2_i) < _s_ui2_max_lst_obj_per_type ; ui2_i++ )
    {
        if ( ui2_lst_id == pt_lst_type_id_handle[e_type][ui2_i].ui2_lst_id )
        {
            *ph_handle = pt_lst_type_id_handle[e_type][ui2_i].h_handle;
            return TRUE;
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_sema_lock
 *
 * Description: This function get the sema for the lst_mngr library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   None
 *   
 ----------------------------------------------------------------------------*/
VOID lst_mngr_sema_lock(VOID)
{
    if ( x_sema_lock( h_lst_mngr_lock, X_SEMA_OPTION_WAIT) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
}

/*-----------------------------------------------------------------------------
 * Name: lst_mngr_sema_unlock
 *
 * Description: This function release the sema for the lst_mngr library. 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    None.
 *
 * Returns: 
 *   None
 *   
 ----------------------------------------------------------------------------*/
VOID lst_mngr_sema_unlock(VOID)
{
    if ( x_sema_unlock( h_lst_mngr_lock) != OSR_OK )
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_UNLOCK_SEMAPHORE);
    }
}

