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
 * $RCSfile: cdb_sortlist.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains sorted list manager function
 *         implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_record.h"
#include "util/x_sort_search.h"

/*-----------------------------------------------------------------------------
  static function prototypes
----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 * Name: _db_select_sort_fct_to_use
 *
 * Description: This API selects a sort function to sort the CDB records.
 *    The priority of the sort function is:
 *
 *    1)  If the user provides a sort function address argument (not NULL),
 *        then this sort function is selected.
 *    2)  If a NULL sort function is provided, then we check if a global
 *        sort function for the data base object that is registered by
 *        caller. If the global sort function is registered, then this
 *        global sort function is selected.
 *    3)  If in (2), no global sort function is registered, then we select
 *        the default sort function implemented by the CDB library.
 *    
 * Inputs:
 *   pt_do:       Pointer to CDB database object.
 *   pt_sl:       Pointer to a sorted list structure.
 *   pf_sort_cmp  Specify the sort compare function.
 *   pv_tag_data: Caller provided private data to pass to sort compare
 *                function.
 *
 * Outputs:
 *   pt_sort_arg  Pointer to structure containing the sort compare function
 *                and private data to the sort compare function that are
 *                selected.
 *
 * Returns: -
 *   None
 --------------------------------------------------------------------------*/

typedef struct   _CDB_SORT_ARG_T_
{
    x_sort_cmp_fct    pf_sort_cmp;
    VOID*             pv_data;
} CDB_SORT_ARG_T;
    
VOID  _db_select_sort_fct_to_use
(
    CDB_DATA_OBJ_T*             pt_do,
    CDB_SORTED_LIST_T*          pt_sl,
    x_sort_cmp_fct              pf_sort_cmp,
    VOID*                       pv_tag_data,
    CDB_SORT_ARG_T*             pt_sort_arg
)
{
    x_sort_cmp_fct              pf_fct_selected = NULL;
    VOID*                       pv_data_selected = NULL;

    /*
       If the caller provide compare function, then
       use this compare function for sorting.
    */
    if ( pf_sort_cmp != NULL )
    {
        pf_fct_selected = pf_sort_cmp;
        pv_data_selected = pv_tag_data;
    }
    /*
        If a sort compare function has been registered
        with CDB database object, then use that
        function for sorting.
    */
    else if ( pt_do->pf_sort_cmp   != NULL   &&
              pt_do->pf_search_cmp != NULL
        )
    {
        pf_fct_selected = pt_do->pf_sort_cmp;
        pv_data_selected = pt_do->pv_tag_data;
    }
    else
    {
        /* sort the address list use default global fct,
           the pv_data to pass to the default global fct
           is teh SortList structure.
        */
        pf_fct_selected = db_compare_record;
        pv_data_selected = pt_sl;
    }

    pt_sort_arg->pf_sort_cmp = pf_fct_selected;
    pt_sort_arg->pv_data     = pv_data_selected;

    return;
}


/*-----------------------------------------------------------------------------
 * Name: db_create_sl_obj
 *
 * Description: This API allocates a sort list structure.
 *           
 * Inputs:  - pt_co:  pointer to the client object that that creates
 *                    this sortlist object.
 *            ui1_key_field_count:  number of key to sort the data records
 *            aui4_field_types:     the field types for sorting the
 *                                  database record.
 *
 * Outputs: - ppt_sl:  address to a sorted list structure.
 *
 * Returns: - DBR_OK     Success.
 *            DBR_OUT_OF_MEMORY     failed.
 ----------------------------------------------------------------------------*/
INT32 db_create_sl_obj
(
    CDB_CLIENT_T*               pt_co,
    UINT8                       ui1_key_field_count,
    const UINT32                aui4_field_types[],
    CDB_SORTED_LIST_T**         ppt_sl
)
{
    CDB_SORTED_LIST_T*   pt_sl;
    UINT8                ui1_i;
    
    *ppt_sl = NULL;

    /*
      Error check
    */
    if ( ui1_key_field_count > MAX_NUM_KEY_PARM_FIELDS )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INVALID_SORTED_LIST);
    }

    /*
      allocate memory for a sorted list object.
    */
    pt_sl=(CDB_SORTED_LIST_T*) x_mem_alloc(sizeof(CDB_SORTED_LIST_T));
    if ( pt_sl != NULL )
    {
        pt_sl->ui1_key_field_count = ui1_key_field_count;
        
        for ( ui1_i = 0 ; ui1_i < ui1_key_field_count ; ui1_i++ )
        {
            pt_sl->aui4_sort_param[ui1_i]=aui4_field_types[ui1_i];
        }
        pt_sl->ui4_version_id = 0;
        pt_sl->ui4_count = 0;
        pt_sl->pt_client = pt_co;
        pt_sl->pv_search_parm_rec = NULL;
        pt_sl->apui1_record_list  = NULL;
        pt_sl->pf_sort_cmp        = NULL;
        pt_sl->pv_tag_data        = NULL;
        *ppt_sl=pt_sl;
    }
    else
    {
        return DBR_OUT_OF_MEMORY;
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_delete_sl_obj
 *
 * Description: This API deletes the sorted list structure.
 *             
 * Inputs:  - pt_sl:  pointer to sorted list structure.
 *            
 *            
 *            
 *
 * Outputs: - None
 *
 * Returns: - DBR_OK     Success.
 *            
 ----------------------------------------------------------------------------*/
INT32 db_delete_sl_obj (
    CDB_SORTED_LIST_T*          pt_sl 
)
{
    if ( pt_sl != NULL )
    {
        if ( pt_sl->apui1_record_list != NULL )
        {
            x_mem_free(pt_sl->apui1_record_list);
        }
        if ( pt_sl->pv_search_parm_rec != NULL )
        {
            x_mem_free(pt_sl->pv_search_parm_rec);
        }
        x_mem_free(pt_sl);
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_update_sl_obj
 *
 * Description: This API update and resort the sorted list with the
 *              data records in the database object.
 *              The sorting will only take place if the key fields
 *              for the sorting have been changed, or the version
 *              id of the database object and version id of the sorted
 *              list are different.
 
 *              Note: this API must be protect by a Read lock.
 *         
 *
 * Inputs:  - pt_do:  pointer to the database object.
 *            pt_sl:  pointer to a sorted list structure.
 *            ui1_key_field_count:  number of field to used for sorting.
 *            aui4_key_field_types: the field types of the sorting parameter.
 *            pf_sort_cmp:   Use the caller provided sorted compare function.
 *            pv_tag_data:   Caller provided private data.
 *            b_forced:      Forec re-sorting of the list.
 *
 * Outputs: - None
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_update_sl_obj (
    CDB_DATA_OBJ_T*             pt_do,
    CDB_SORTED_LIST_T*          pt_sl,
    UINT8                       ui1_key_field_count,
    const DB_FIELD_T            aui4_key_field_types[],
    x_sort_cmp_fct              pf_sort_cmp,
    VOID*                       pv_tag_data,
    BOOL                        b_forced
)
{
    BOOL            b_update;
    BOOL            b_key_changed = FALSE;
    INT32           i4_rc;
    INT32           i4_i;
    CDB_SORT_ARG_T  t_sort_arg;

    /*
      Error check
    */
    if ( ui1_key_field_count > MAX_NUM_KEY_PARM_FIELDS )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INVALID_SORTED_LIST);
    }

    if ( b_forced == TRUE )
    {
        b_update = TRUE;
    }
    else
    {
        b_update = FALSE;
    }

    /*
      Determine which sort function and pv_data will be used.
    */
    _db_select_sort_fct_to_use(pt_do,
                               pt_sl,
                               pf_sort_cmp,
                               pv_tag_data,
                               &t_sort_arg);
    
    /*
      Check if updating and resorting is necessary.
    */
    if ( pt_sl->apui1_record_list == NULL || pt_sl->ui4_count == 0 )
    {
        /* sorted list is empty. */
        b_update = TRUE;
        /* x_dbg_stmt("<***** Resort because record list is empty.*****>\n"); */
    }
    else if ( pt_sl->ui1_key_field_count != ui1_key_field_count )
    {
        /* count of key field parameters has changed. */
        b_key_changed = TRUE;
        b_update = TRUE;
        /* x_dbg_stmt("<***** Resort because key field has changed.*****>\n"); */
    }
    else if ( db_same_field_types(ui1_key_field_count, aui4_key_field_types,
                                  pt_sl->aui4_sort_param) == FALSE )
    {
        /* key field types has changed. */
        b_key_changed = TRUE;
        b_update = TRUE;
        /* x_dbg_stmt("<***** Resort because search paramter has changed.*****>\n"); */
    }
    else if ( pt_sl->ui4_version_id != pt_do->ui4_db_version )
    {
        /* Update and resort the index list  if the sorted
           list structure and the database object are not in sync */
        b_update = TRUE;
        /* x_dbg_stmt("<***** Resort because data record has changed.*****>\n"); */
    }
    else if ( pt_sl->pf_sort_cmp != t_sort_arg.pf_sort_cmp )
    {
        /* Resort the list if the sort function has changed. */
        b_update = TRUE;
        /* x_dbg_stmt("<***** Resort because addr of sort function changed.*****>\n"); */
    }
    else if ( x_memcmp(pt_sl->pv_tag_data, t_sort_arg.pv_data, sizeof(VOID*))
              != 0 )
    {
        /* Resort if the tag data to the sort function has changed.  Note:
           We need to check the pv_tag_data because when SVL client register
           custom sort function, the function that is registered to CDB is
           the wrapper function. The actual function address is pass to
           the wrapper function as a structure, the first field of
           the structure is the client specified sort compare function.
        */
        b_update = TRUE;
        /* x_dbg_stmt("<***** TAG data for sort function changed.*****>\n"); */
    }

    i4_rc=DBR_OK;
    if ( b_update == TRUE )
    {   
        /* Re-Allocate the address array to store the memory block
           addresess.  */

        pt_sl->apui1_record_list = (UINT8**)
                x_mem_realloc((VOID*)(pt_sl->apui1_record_list),
                              pt_do->ui2_record_count * sizeof(UINT8*));

        if ( pt_sl->apui1_record_list != NULL )
        {
            /* Update the record count values. */
            pt_sl->ui4_count = pt_do->ui2_record_count ;

            /*
               Update with the new key parameter.
            */
            if ( b_key_changed == TRUE )
            {
                pt_sl->ui1_key_field_count = ui1_key_field_count;
                for ( i4_i=0; i4_i < ui1_key_field_count ; i4_i++ )
                {
                    pt_sl->aui4_sort_param[i4_i]=aui4_key_field_types[i4_i];
                }
            }
            
            /*
              Get the addresses of all records from the database object's
              memory buffer.
            */
            if ( db_get_used_list(pt_do->t_memory,
                                  pt_sl->ui4_count,
                                  pt_sl->apui1_record_list)
                 == DBR_OK )
            {

                /* sort the address list. */
                if ( x_qsort(pt_sl->apui1_record_list,
                             pt_sl->ui4_count,
                             sizeof(UINT8*),
                             t_sort_arg.pf_sort_cmp,
                             t_sort_arg.pv_data) == SSR_OK
                    )
                {
                    /* Update the version id value */
                    pt_sl->ui4_version_id = pt_do->ui4_db_version  ;

                    /* Update with the sort function address. */
                    pt_sl->pf_sort_cmp = t_sort_arg.pf_sort_cmp;
                    pt_sl->pv_tag_data = t_sort_arg.pv_data;

                    /* x_dbg_stmt("<***** Resort completed on the rec list .*****>\n"); */
                }
                else
                {
                    i4_rc = DBR_FAIL;
                }
            }
            else
            {
                i4_rc = DBR_FAIL;
            }
        }
        else
        {
            /* Out of memory... */
            i4_rc=DBR_OUT_OF_MEMORY;
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_copy_search_parm_to_rec
 *
 * Description: This API copy the search field values into the appropriate
 *    location in the record.  The record will contains holes where no fields
 *    data were written to.
 *              
 *             
 * Inputs:  - pt_sl:  pointer to sorted list structure.
 *            
 *            
 *            
 *
 * Outputs: - None
 *
 * Returns: - DBR_OK     Success.
 *            
 ----------------------------------------------------------------------------*/
INT32 db_copy_search_parm_to_rec (
    CDB_SORTED_LIST_T*     pt_sl,
    UINT8                  ui1_key_field_count,
    const DB_FIELD_T       aui4_key_field_types[],
    const VOID*            pv_search_parm_addr[]
)
{
    UINT16              ui2_record_width;
    CDB_DATA_OBJ_T*     pt_do;
    INT32               i4_i;
    UINT16              ui2_width;
    UINT16              ui2_offset;
    
    pt_do = pt_sl->pt_client->p_data_obj;
    ui2_record_width = pt_do->ui2_record_width;
    
    if ( pt_sl->pv_search_parm_rec == NULL )
    {
        pt_sl->pv_search_parm_rec = (UINT8*)x_mem_alloc(ui2_record_width);
    }
    
    if ( pt_sl->pv_search_parm_rec != NULL )
    {
        x_memset(pt_sl->pv_search_parm_rec, 0, ui2_record_width);
        
        for ( i4_i = 0 ; i4_i < pt_sl->ui1_key_field_count ; i4_i++ )
        {
            if ( db_get_fld_offset_width(pt_do,
                                         aui4_key_field_types[i4_i],
                                         &(ui2_offset),
                                         &(ui2_width) )
                 == DBR_OK
                )
            {
                x_memcpy(pt_sl->pv_search_parm_rec+ui2_offset,
                         pv_search_parm_addr[i4_i],
                         ui2_width);
            }
            else
            {
                return DBR_FAIL;
            }
        }
    }
    else
    {
        return DBR_FAIL;
    }
    return DBR_OK;
}
