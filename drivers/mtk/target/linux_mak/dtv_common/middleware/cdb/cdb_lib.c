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
 * $RCSfile: cdb_lib.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains Multiple-Reader Single-Writer lock related
 *         function implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "rwlock/rwl_api.h"
#include "cdb/cdb_api.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_record.h"

/*-----------------------------------------------------------------------------
  data declaraions
  ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: db_delete_data_obj
 *
 * Description: Internal CDB API to delete a database object.
 *              points to the database object.
 *
 * Inputs:  - pt_do:  pointer to the internal CDB database object.
 *
 * Outputs: - None.
 *
 * Returns: - None.
 *
 ----------------------------------------------------------------------------*/
VOID db_delete_data_obj(CDB_DATA_OBJ_T* pt_do)
{
    if ( pt_do != NULL )
    {
        /* delete Read-Write lock */
        if ( pt_do->h_rwloc != NULL_HANDLE )
        {
            rwl_delete_lock(pt_do->h_rwloc);
        }

        /* free memory for the record definition
           and descriptions. */
        if ( pt_do->pt_fields != NULL )
        {
            x_mem_free(pt_do->pt_fields);
        }

        if ( pt_do->ui2_fld_offset != NULL)
        {
            x_mem_free(pt_do->ui2_fld_offset);
        }

        if ( pt_do->ui2_fld_width != NULL)
        {
            x_mem_free(pt_do->ui2_fld_width);
        }

        /* free the memory chunks for storing the records
           and memory manager. */
        if ( pt_do->t_memory != NULL )
        {
            db_delete_memory_manager(pt_do->t_memory);
        }

        /* free the mini-buffer for storing string. */
        if ( pt_do->pt_strbuf != NULL )
        {
            x_str_buf_free(pt_do->pt_strbuf);
        }

        /* free the mini-buffer for storing customer data. */
        if ( pt_do->pt_membuf != NULL )
        {
            x_mem_buf_free(pt_do->pt_membuf);
        }
        
        /* free memory for the common data section. */
        if ( pt_do->pui1_common_data != NULL )
        {
            x_mem_free(pt_do->pui1_common_data) ;
        }

        x_mem_free(pt_do);
    }
}

/*-----------------------------------------------------------------------------
 * Name:  db_create_internal_lock
 *
 * Description: Internal API to create an binary semaphore lock. this
 *              binary semaphre lock (acts like a global lock) is used
 *              for controlling access to the shared variables, for
 *              example: tlink, and database object and name array
 *
 *
 * Inputs:  - None
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
static HANDLE_T  _h_db_lock_ = NULL_HANDLE;

VOID db_create_internal_lock()
{
    /* Create the database object semaphores. Simply */
    /* abort if semahore cannot be created.            */
    if (x_sema_create( &(_h_db_lock_), X_SEMA_TYPE_MUTEX,
                       X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_CREATE_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  db_get_internal_lock
 *
 * Description: Internal API to lock the binary semaphore lock. This
 *              is to control access to the shared variables, for
 *              example: tlink, and database object and name array
 *
 *
 * Inputs:  - None
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
VOID db_get_internal_lock()
{
    /* lock database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_lock( _h_db_lock_, X_SEMA_OPTION_WAIT) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  db_release_internal_lock
 *
 * Description: Internal API to unlock the binary semaphore lock. This
 *              is to control access to the shared variables, for
 *              example: tlink, and database object and name array
 *
 *
 * Inputs:  - None
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
VOID db_release_internal_lock()
{
    /* un-lock the database object semaphores. Simply */
    /* abort if semaphore unlock fails.            */
    if (x_sema_unlock( _h_db_lock_ ) != OSR_OK)
    {
        ABORT (DBG_CAT_SEMAPHORE, DBG_ABRT_CANNOT_LOCK_SEMAPHORE);
    }
    return;
}
#if 0
/*-----------------------------------------------------------------------------
 * Name: _db_compare_field_t
 *
 * Description: Internal API to compare two field type and determine which
 *              field type should come first in the record structure. We want
 *              the internal record fields in descending order, i.e., 8 bytes ->
 *              8 bytes array -> 4 byte -> 4 byte array, etc. Therefore,
 *              the  comparison assumes 8 byte < 8 byte array < 4 byte < 4 byte array
 *              < 2 byte < 2 byte array < 1 byte < 1 byte array.
 * Inputs:  -
 *
 * Outputs: - None.
 *
 * Returns: -    -1  if pv_elem_A is 'Less' than pv_elem_B.
 *                0  if pv_elem_A is 'Equal' to pv_elem_B.
 *                1  if pv_elem_A is 'Greater' than pv_elem_B.
 *
 ----------------------------------------------------------------------------*/
INT32 _db_compare_field_t(
    VOID*		pv_elem_A,
    VOID*		pv_elem_B,
    VOID*       pv_array)
{
    UINT8              ui1_A;
    UINT8              ui1_B;
    DB_FIELD_INFO_T*   pt_fields;
    INT32              rc;
    INT32              ui4_size_A;
    INT32              ui4_size_B;
    BOOL               b_A_is_array;
    BOOL               b_B_is_array;
    INT32              ui4_ctype_A;
    INT32              ui4_ctype_B;

/*
  Deference the index values used to access the data element array.
  The index value used here is UINT8
*/
    ui1_A = *( (UINT8*) pv_elem_A);
    ui1_B = *( (UINT8*) pv_elem_B);
    pt_fields=(DB_FIELD_INFO_T*) pv_array;

    ui4_size_A = db_get_fldtype_size(pt_fields[ui1_A].ui4_field_type);
    ui4_size_B = db_get_fldtype_size(pt_fields[ui1_B].ui4_field_type);

    if ( ui4_size_A > ui4_size_B )
    {
        rc = -1 ;
    }
    if ( ui4_size_A ==  ui4_size_B )
    {
        b_A_is_array = DB_IS_FIELD_T_ARRAY(pt_fields[ui1_A].ui4_field_type);
        b_B_is_array = DB_IS_FIELD_T_ARRAY(pt_fields[ui1_B].ui4_field_type);

        if ( b_A_is_array && ( ! b_B_is_array ) )
        {
            rc = -1;
        }

        if ( b_A_is_array &&  b_B_is_array  )
        {
            ui4_ctype_A = DB_GET_C_TYPE(pt_fields[ui1_A].ui4_field_type);
            ui4_ctype_B = DB_GET_C_TYPE(pt_fields[ui1_B].ui4_field_type);
            if ( ui4_ctype_A == DBF_STRING_T  && (! ui4_ctype_B == DBF_STRING_T ) )
            {
                rc = 1;
            }
            if ( ( ! ui4_ctype_A == DBF_STRING_T ) &&
                 ( ui4_ctype_B == DBF_STRING_T ) )
            {
                rc = -1;
            }

            rc = 0;
        }

        if ( ( ! b_A_is_array ) && b_B_is_array )
        {
            rc = 1;
        }
    }
    if ( ui4_size_A < ui4_size_B )
    {
        rc = 1 ;
    }
    return rc;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: db_create_client_obj
 *
 * Description: Internal API to create a client object and attach the.
 *              client object to the database object.
 *
 * Inputs:  - pt_data_obj:  database object that this client object is
 *                          accessing.
 *
 *            ppt_client:   pointer to a pointer variable storing
 *                          the client object created.
 *
 *            pv_tag:       tag value passed in by the caller.
 *
 *            pf_nfy:       notification implemented by the caller. The
 *                          function is called when the database object
 *                          is modified for any reasons.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OUT_OF_MEMORY: Out of memory.
 *            DBR_OK: client object is created.
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_create_client_obj(
    CDB_DATA_OBJ_T*		pt_data_obj,
    CDB_CLIENT_T**		ppt_client,
    VOID*               pv_tag,
    db_notify_fct       pf_nfy
)
{
    CDB_CLIENT_T*   pt_co;

    *ppt_client =   NULL;
    pt_co = (CDB_CLIENT_T*) x_mem_alloc(sizeof(CDB_CLIENT_T));
    if ( pt_co != NULL )
    {
        pt_co->ui4_cursor = DB_NULL_RECORD;
        pt_co->p_data_obj = pt_data_obj;
        pt_co->pt_sl = NULL;
        pt_co->pv_tag = pv_tag;
        pt_co->h_write_thread = NULL_HANDLE;
        pt_co->pf_notify = pf_nfy;

        /* copy the client object address to the return parameter. */
        *ppt_client = pt_co;
    }
    else
    {
        return DBR_OUT_OF_MEMORY ;
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_delete_client_obj
 *
 * Description: Internal API to delete a client object.
 *
 * Inputs:  - pt_co:  pointer to the CDB client control structure.
 *
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK
 *
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_delete_client_obj
(
    CDB_CLIENT_T*		pt_co
)
{
    if ( pt_co != NULL )
    {
        if ( pt_co->pt_sl != NULL )
        {
            db_delete_sl_obj(pt_co->pt_sl);
        }
        x_mem_free(pt_co);
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_extract_key_field_values
 *
 * Description: API to extract out the fields values for the
 *              key field from a record.
 *
 *
 * Inputs:  - pt_do: database object
 *            apv_field_values: buffers containing field value to copy from.
 *
 *
 * Outputs: - apv_key_field_values: buffers to copy the field values to.
 *
 * Returns: - DBR_OK:  success.
 *            DBR_FAIL: key field types is not the correct type.
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_extract_key_field_values
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            apv_field_values[],
    UINT8*            apv_key_field_values[]
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;


    for ( ui1_i=0; ui1_i < pt_do->ui1_key_field_count; ui1_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     pt_do->aui4_key_field_types[ui1_i],
                                     &(ui2_offset),
                                     &(ui2_width))
             == DBR_OK )
        {
            x_memcpy(apv_key_field_values[ui1_i],
                     apv_field_values[ui1_i],
                     ui2_width);
        }
        else
        {
            return DBR_FAIL;
        }
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_allocate_key_field_buffers
 *
 * Description: API to allocate space for storing key field values.
 *
 *
 *
 * Inputs:  - pt_do database object
 *            ui1_key_field_count:    number of buffers
 *            aui4_key_field_types:   the field types for each of the buffer
 *                                    to be allocated.
 *
 * Outputs: - papv_key_field_values:  pointer to an address of UINT8
 *                                    addresses.
 *
 * Returns: - DBR_OK:  success
 *            DBR_FAIL: the specified field type is incorrect.
 *                      (e.g., key field type specified is not the
 *                       right type. )
 *
 ----------------------------------------------------------------------------*/
INT32 db_allocate_key_field_buffers
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8             ui1_key_field_count,
    DB_FIELD_T        aui4_key_field_types[],
    UINT8*            *papv_key_field_values[]
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;
    UINT8**    apv_key_field_values;

    apv_key_field_values = (*papv_key_field_values);

    for ( ui1_i=0; ui1_i < ui1_key_field_count; ui1_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_key_field_types[ui1_i],
                                     &(ui2_offset),
                                     &(ui2_width))
             == DBR_OK )
        {
            apv_key_field_values[ui1_i] = (UINT8*) x_mem_alloc(ui2_width);
            if (  apv_key_field_values[ui1_i] == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
        }
        else
        {
            return DBR_FAIL;
        }
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_deallocate_key_field_buffers
 *
 * Description: API to free space for storing key field values.
 *
 *
 *
 * Inputs:  - pt_do database object
 *            pui1_record:  memory block to store the record
 *
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_deallocate_key_field_buffers
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8             ui1_key_field_count,
    UINT8*            apv_key_field_values[]
)
{
    UINT8      ui1_i;

    for ( ui1_i=0; ui1_i < ui1_key_field_count; ui1_i++ )
    {
        if ( apv_key_field_values[ui1_i] != NULL )
        {
            x_mem_free(apv_key_field_values[ui1_i]);
        }
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_notify_all
 *
 * Description: API to notify all the clients that database has been modified.
 *              Note: this API must be protect by the read-write lock.
 *
 * Inputs:  - h_client
 *            pt_do
 *            e_cond
 *            ui4_reason
 *            ui4_db_version
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_notify_all
(
    HANDLE_T          h_client,
    CDB_DATA_OBJ_T*   pt_do,
    DB_COND_T         e_cond,
    UINT32            ui4_reason,
    UINT32            ui4_db_version
    )
{
    CDB_HL_PARSE_DATA_T  t_parse_data;

    t_parse_data.h_self = h_client;
    t_parse_data.e_cond = e_cond;
    t_parse_data.ui4_reason = ui4_reason;
    t_parse_data.ui4_db_version = ui4_db_version;

    /* protect the call to handle_parse function. */
    db_get_internal_lock();

    /*
      Check the reason for the notification and called the
      appropriate callback function.
    */
    if ( e_cond == DB_CLOSED )
    {
        /* Database object is being freed, notify the client and free
           the client handle to the database object.
        */
        handle_parse(&(pt_do->t_link),cdb_handle_parse_delete_handle,
                     &t_parse_data);
    }
    else
    {
        /*
          Database object has been modified, notify the client so that it
          can take appropriate action.
        */
        handle_parse(&(pt_do->t_link),cdb_handle_parse_cb,&t_parse_data);
    }

    /* Don't forget to unlock */
    db_release_internal_lock();

    return DBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: db_compare_record
 *
 * Description: API to compare two records. The first and second arguments
 *              contains the addresses to the record. The third argument
 *              (sorted list structure) specifies the parameters for comparison,
 *              e.g., the field types of the records.
 *
 * Inputs:  -
 *
 * Outputs: - None.
 *
 * Returns: -
 *
 *
 *
 ----------------------------------------------------------------------------*/
INT32 db_compare_record
(
    const VOID*    pv_a,
    const VOID*    pv_b,
    VOID*          pv_obj
)
{
    CDB_DATA_OBJ_T*             pt_do;
    CDB_CLIENT_T*               pt_co;
    CDB_SORTED_LIST_T*          pt_sl;
    UINT8*                      pui1_a;
    UINT8*                      pui1_b;
    INT32                       i4_i;
    UINT16                      ui2_offset;
    UINT16                      ui2_width;
    INT32                       i4_result;
/* Temporary variables. */
    CHAR                        c1_a, c1_b;
    UINT8                       ui1_a, ui1_b;
    UINT16                      ui2_a, ui2_b;
    UINT32                      ui4_a, ui4_b;
    INT8                        i1_a, i1_b;
    INT16                       i2_a, i2_b;
    INT32                       i4_a, i4_b;
    float                       f4_a, f4_b;
    CHAR*                       s_a;
    CHAR*                       s_b;


    pt_sl=(CDB_SORTED_LIST_T*)  pv_obj;
    pt_co=pt_sl->pt_client;
    pt_do=pt_co->p_data_obj;
    pui1_a = *( (UINT8**) pv_a ) ;
    pui1_b = *( (UINT8**) pv_b );
    i4_result = -1;

    /*
      Compare the key fields, first determine the size and offset
      of the fields, then copy the fields to two temporary
      variables for comparison.
    */
    for ( i4_i = 0 ; i4_i < pt_sl->ui1_key_field_count ; i4_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     pt_sl->aui4_sort_param[i4_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            switch ( DB_GET_C_TYPE(pt_sl->aui4_sort_param[i4_i] ) )
            {
                case DBF_UINT8_T :
                {
                    x_memcpy(&ui1_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&ui1_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(ui1_a,ui1_b,i4_result);
                }
                break;

                case DBF_UINT16_T :
                {
                    x_memcpy(&ui2_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&ui2_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(ui2_a,ui2_b,i4_result);
                }
                break;

                case DBF_UINT32_T :
                {
                    x_memcpy(&ui4_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&ui4_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(ui4_a,ui4_b,i4_result);
                }
                break;

                case DBF_INT8_T :
                case DBF_ENUM8_T:
                {
                    x_memcpy(&i1_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&i1_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(i1_a,i1_b,i4_result);
                }
                break;

                case DBF_INT16_T :
                case DBF_ENUM16_T:
                {
                    x_memcpy(&i2_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&i2_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(i2_a,i2_b,i4_result);
                }
                break;

                case DBF_INT32_T :
                case DBF_ENUM32_T:
                {
                    x_memcpy(&i4_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&i4_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(i4_a,i4_b,i4_result);
                }
                break;

                case DBF_CHAR_T :
                {
                    x_memcpy(&c1_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&c1_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(c1_a,c1_b,i4_result);
                }
                break;

                case DBF_FLOAT32_T :
                {
                    x_memcpy(&f4_a, pui1_a+ui2_offset, ui2_width);
                    x_memcpy(&f4_b, pui1_b+ui2_offset, ui2_width);
                    COMPARE_TWO_TYPE(f4_a,f4_b,i4_result);
                }
                break;

                case DBF_STRING_T :
                {
                    /* compare null-terminate ISO-8 char string. */
                    s_a=(CHAR*) (pui1_a+ui2_offset);
                    s_b=(CHAR*) (pui1_b+ui2_offset);
                    i4_result=x_strncmp(s_a, s_b, (SIZE_T)(ui2_width-1));
                }
                break;

                default:
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_UNKNOWN_FIELD_TYPE);
            }
        }
        else
        {
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_INVALID_SORTED_LIST);
        }
        /*
          If the result of the comparsion for this field is not 0, then
          we can exit the loop, otherwise we need to compare the next
          field.
        */
        if ( i4_result != 0 )
        {
            break;
        }
    }
    return i4_result;
}

/*-----------------------------------------------------------------------------
 * Name: db_copy_data
 *
 * Description: API to copy records and common data from one CDB object
 *     to another object.
 *              
 *
 * Inputs:
 *    t_from:  database object to read the record from.
 *    t_to:    database object to copy the records to.
 * 
 * Outputs: - None.
 *
 * Returns: 
 *    DBR_OK
 *    DBR_FAIL
 ----------------------------------------------------------------------------*/
INT32   db_copy_data
(
    CDB_DATA_OBJ_T*    pt_from,
    CDB_DATA_OBJ_T*    pt_to
)
{
    INT32              i4_rc;
    INT32              i4_i;
    UINT32             ui4_rec_copied;

    /*
      Evaluate the metadata of 2 CDB object to determine if copy operation
      should proceed.
    */
    if ( pt_from->ui1_field_count  == pt_to->ui1_field_count      &&
         pt_from->ui2_record_width == pt_to->ui2_record_width
        )
    {
        for (i4_i=0; i4_i < pt_from->ui1_field_count; i4_i++ )
        {
            if ( pt_from->pt_fields[i4_i].ui4_field_type !=
                 pt_to->pt_fields[i4_i].ui4_field_type )
            {
                return DBR_FAIL;
            }

            if ( pt_from->pt_fields[i4_i].ui2_field_element_count !=
                 pt_to->pt_fields[i4_i].ui2_field_element_count )
            {
                return DBR_FAIL;
            }
        }

        for (i4_i=0; i4_i < pt_from->ui1_field_count; i4_i++ )
        {
            if ( pt_from->ui2_fld_offset[i4_i] != pt_to->ui2_fld_offset[i4_i] )
            {
                return DBR_FAIL;
            }
        }

        for (i4_i=0; i4_i < pt_from->ui1_field_count; i4_i++ )
        {
            if ( pt_from->ui2_fld_width[i4_i] != pt_to->ui2_fld_width[i4_i] )
            {
                return DBR_FAIL;
            }
        }

        /*
          copy the string buffer.
        */
        if ( pt_from->pt_strbuf != NULL )
        {
            if ( pt_to->pt_strbuf != NULL )
            {
                x_str_buf_free(pt_to->pt_strbuf);
                pt_to->pt_strbuf = NULL;
            }
            pt_to->pt_strbuf=x_str_buf_dup(pt_from->pt_strbuf);
            if ( pt_to->pt_strbuf == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
        }

        /*
          copy the memory buffer if one is available.
        */
        if ( pt_from->pt_membuf != NULL )
        {
            if ( pt_to->pt_membuf != NULL )
            {
                x_mem_buf_free(pt_to->pt_membuf);
                pt_to->pt_membuf = NULL;
            }
            pt_to->pt_membuf=x_mem_buf_dup(pt_from->pt_membuf);
            if ( pt_to->pt_membuf == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }
        }
        
        /*
          copy record from pt_from database object to pt_to database object.
        */
        i4_rc = db_copy_mem_obj(pt_from->t_memory, pt_to->t_memory,
                                &ui4_rec_copied);
        if ( i4_rc == DBR_OK )
        {
            /* update record count and version id. */
            pt_to->ui2_last_del_count = pt_to->ui2_record_count ;
            pt_to->ui2_record_count = (UINT16)ui4_rec_copied;
            pt_to->ui2_last_add_count = pt_to->ui2_record_count ;
            CDB_INCRM_VER(pt_to->ui4_db_version);

            /* select the largest of 2 version ID to use. This
               is to prevent problem with version ID in memory smaller
               than the version ID in the filesystme. */
            if ( pt_from->ui4_db_version > pt_to->ui4_db_version )
            {
                pt_to->ui4_db_version = pt_from->ui4_db_version ;
            }
                
            /*
              copy the common data if needed.
            */
            if ( pt_from->ui2_common_size > 0 )
            {
                pt_to->pui1_common_data = 
                    (UINT8*) x_mem_realloc(pt_to->pui1_common_data,
                                           (SIZE_T) pt_from->ui2_common_size);

                if ( pt_to->pui1_common_data != NULL )
                {
                    x_memcpy(pt_to->pui1_common_data,
                             pt_from->pui1_common_data,
                             (SIZE_T)pt_from->ui2_common_size);

                    pt_to->ui2_common_size = pt_from->ui2_common_size;
                }
                else
                {
                    i4_rc = DBR_OUT_OF_MEMORY;
                }
            }
        }
    }
    else
    {
        i4_rc = DBR_FAIL;
    }

    
    return i4_rc;
}


        
    
