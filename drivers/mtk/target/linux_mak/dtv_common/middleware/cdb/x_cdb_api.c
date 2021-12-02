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
 * $RCSfile: cdb_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains exported CDB API implementations.
 *---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
  include files
  ------------------------------------------------------------------------*/

#include "cdb/cdb_api.h"

/*-----------------------------------------------------------------
 * Function implementation.
 *----------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_db_create
 *
 * Description: This API creates a database object and client object that
 *              points to the database object.
 *
 * Inputs:  -  ui2_max_record:  maximum number of records for this
 *                              database object.(STATIC), or it is
 *                              the chunk size when internal
 *                              memory is increased (DYNAMIC)
 *             e_memory_policy: memory policy (STATIC or DYNAMIC)
 *             s_db_name:       database name.
 *             ui1_count:       count of the record fields
 *             at_field_info:   record field description array
 *             aui4_key_field_count: count of number of field that
 *                                   makes up the key parameter.
 *             aui4_key_field_types: array of field types that makes up
 *                                   the key parameter.
 *             pv_tag:          a tag specified by the client
 *             pf_nfy:          a notification provide by the client. This
 *                              notification funciton is called to notify
 *                              the registered client any changes in the
 *                              database object.
 *        
 *
 * Outputs: - ph_client: A client handle to the database object.
 *
 * Returns: - DBR_OK     A new database object and a client handle to
 *                       the database object are initialized successfully.
 *            DBR_FAIL   Cannot create database object and handle.
 *            DBR_INV_ARG Input arguments are invalid, either because
 *                        the count variables are equal to 0 or the
 *                        pointer variables are null.
 *            DBR_OUT_OF_MEMORY Out of memory.
 *            DBR_OUT_OF_HANDLE Out of handle.
 *            DBR_DB_LIMIT_EXCEED Can not create anymore database object,
 *                                because we have reach the resouce limit.
 *            DBR_INV_NAME The Specified name 's_db_name' has been used by
 *                             another CDB object.
 ----------------------------------------------------------------------------*/
INT32 x_db_create
(
    UINT16            ui2_max_record,
    DB_MEM_POLICY_T   e_memory_policy,
    CHAR*             s_db_name,
    UINT8             ui1_count,
    DB_FIELD_INFO_T   at_field_info[],
    UINT8			  ui1_key_field_count,
    DB_FIELD_T        aui4_key_field_types[],
    VOID*             pv_tag,
    db_notify_fct     pf_nfy,
    HANDLE_T*         ph_client
)
{
    return db_create(ui2_max_record,
                     e_memory_policy,
                     s_db_name,
                     ui1_count,
                     at_field_info,
                     ui1_key_field_count,
                     aui4_key_field_types,
                     pv_tag,
                     pf_nfy,
                     ph_client);

}

/*-----------------------------------------------------------------------------
 * Name:  x_db_delete
 *
 * Description: This API delete a named database object all the handles
 *              (e.g, the owner that create the database and other
 *              client handles that point to this database.)
 *
 * Inputs:  - h_client:   handle to the database object.
 *            s_db_name:  specifies the name of the database object.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK          Specified database object is deleted.
 *            DBR_NOT_FOUND   Cannot find the specified database object.
 *            DBR_INV_HANDLE  Bad handle.
 *            DBR_INV_ARG     s_db_name is null.
 ----------------------------------------------------------------------------*/
INT32 x_db_delete
(
    HANDLE_T          h_client,
    CHAR*             s_db_name
)
{
    return db_delete(h_client,s_db_name);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_clean
 *
 * Description: This API delete all records from a specified database object.  
 *
 * Inputs:  - h_client:   handle to the database object.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK          All records from the specified database
 *                            object are deleted.
 *            DBR_INV_HANDLE  Bad handle.
 *
 ----------------------------------------------------------------------------*/
INT32 x_db_clean
(
    HANDLE_T          h_client
)
{
    return db_clean(h_client);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_get_handle_by_name
 *
 * Description: This API returns a new client handle to a database.
 *
 * Inputs:  - s_db_name:   Name of the database object.
 *            pv_tag:      A private tag value for this client.
 *            pv_nfy:      Notification for this client.
 *      
 * Outputs: - ph_client:   A handle to the database object.
 *
 * Returns: - DBR_OK     A new handle to the specified database object is
 *                       created.
 *            DBR_NOT_FOUND    The specified database object does not existed.
 *            DBR_OUT_OF_HANDLE   Cannot create a new handle to the
 *                                database object.
 *            DBR_INV_ARG  s_db_name or ph_client is null.
 ----------------------------------------------------------------------------*/
INT32 x_db_get_handle_by_name
(
    CHAR*             s_db_name,
    VOID*             pv_tag,
    db_notify_fct     pf_nfy,
    HANDLE_T*         ph_client
)
{
    return db_get_handle_by_name(s_db_name,pv_tag,pf_nfy,ph_client);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_read_rec
 *
 * Description: This API reads a specific record from the database record.
 *
 * Inputs:  - h_client:      handle to the database client
 *            ui1_keyfld_count: number of fields that make up the search
 *                              parameters.
 *            aui4_keyfld_types: field types that make up the search paramters.
 *            apv_keyfld_addrs:  an array of pointers to the content of the
 *                               search parameters.
 *            ui1_count:         number of value to be extracted
 *            aui4_fld_type_selected: field types to be obtained.
 *            
 *            ui2_record_index:   index number of the records matching the
 *                                search paramters.
 *            aui2_addr_len:  Specify the buffer space for the buffer ref'd by
 *                            the address entry in 'apv_fld_value_addr'
 *
 * Outputs: - apv_fld_value_addr: array of pointer address to store the
 *                                  field values.
 *            aui2_addr_len:  On return, it contains the data len copied into
 *                            the space ref'd by the the address entry in
 *                            'apv_fld_value_addr'.
 *            pui4_db_version:    pointer to current database version id.
 *
 * Returns: - DBR_OK         A record is read successfully.
 *            DBR_INV_HANDLE Bad handle or the database has been
 *                           deleted.
 *            DBR_INV_ARG    Input arguments are invalid, either because
 *                           the count variables are equal to 0 or the
 *                           pointer variables are null.
 *            DBR_REC_NOT_FOUND  No record matching the searching parameters
 *                               was found.
 *            DBR_MODIFIED   When reading the database using idx value
 *                           greater than 0, e.g, iterating through the
 *                           records, the database has been modified
 *                           by another client.
 ----------------------------------------------------------------------------*/
INT32 x_db_read_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[],
    UINT8             ui1_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    VOID*             apv_fld_value_buffer[],
    UINT16            aui2_buffer_len[],
    UINT16            ui2_record_index,
    UINT32*           pui4_db_version
)
{
    return db_read_rec_v2(h_client,
                          ui1_keyfld_count,
                          aui4_keyfld_types,
                          apv_keyfld_addrs,
                          at_keyfld_search_opts,
                          ui1_count,
                          aui4_fld_type_selected,
                          apv_fld_value_buffer,
                          aui2_buffer_len,
                          ui2_record_index,
                          pui4_db_version); 
}


/*-----------------------------------------------------------------------------
 * Name:  x_db_update_rec
 *
 * Description: This API updates fields of an existing record in the
 *              database object.  The record to be updated is specified
 *              by the key fields, and it is an error if the specified
 *              record can not be found.
 *
 * Inputs:  -
 *    h_client:  client handle
 *    apv_key_field_values:  values for the key parameter. Note:
 *                           If multiple fields make up a key
 *                           parameter, the order of values
 *                           has to be in the same order that
 *                           key types are specified when
 *                           the database was created.
 *    ui1_fld_count: Number of record fields to be updated.
 *    aui4_fld_type_selected: Specify field type of the record fields to
 *                            be updated.
 *    apv_fld_addrs:  Address of the record fields. The content
 *                    of the address will be copy into the
 *                    corresponding field in the database record.
 *    aui2_addr_len:  Specify the length (in bytes) of data in the buffer 
 *                    ref'd by the address entry in 'apv_field_addrs'
 *    
 * Outputs: - None.
 *
 * Returns: - DBR_OK     A new lock is initialized successfully.
 *            DBR_INV_HANDLE Bad handle.
 *            DBR_INV_ARG  key_field_values or field_values array is null.
 *            DBR_REC_NOT_FOUND The specified record can not be found.
 *            DBR_FAIL   The specified field types does not match the
 *                       record's field types.
 ----------------------------------------------------------------------------*/
INT32 x_db_update_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_buffer[],
    const UINT16      aui2_buffer_len[]
)
{
    return db_update_rec_v2(h_client,
                            apv_key_field_values,
                            ui1_fld_count,
                            aui4_fld_type_selected,
                            apv_fld_buffer,
                            aui2_buffer_len);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_write_rec
 *
 * Description: This API write a record into the database object.
 *
 * Inputs:  -
 *  h_client:  client handle
 *  apv_key_field_values:  values for the key parameter. Note:
 *                         If multiple fields make up a key
 *                         parameter, the order of values
 *                         has to be in the same order that
 *                         key types are specified when
 *                         the database was created.
 *  apv_field_values:  values of the record fields.
 *
 *  aui2_addr_len:  Specify the length (in bytes) of data in the buffer 
 *                  ref'd by the address entry in 'apv_field_values'
 *
 *  ui1_write_mode: Specify if the record to be written is
 *                  a new record or update an existing
 *                  record.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     A new lock is initialized successfully.
 *            DBR_INV_HANDLE Bad handle.
 *            DBR_INV_ARG  key_field_values or field_values array is null.
 *            DBR_REC_NOT_FOUND The specified record can not be found.
 *            DBR_OUT_OF_MEMORY: can not allocate new memory.
 *           
 ----------------------------------------------------------------------------*/
INT32 x_db_write_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[],
    const UINT16      aui2_addr_len[],
    DB_CTRL_FLAG_T    ui1_write_mode
)
{
    return db_write_rec_v2(h_client,
                           apv_key_field_values,
                           apv_field_values,
                           aui2_addr_len,
                           ui1_write_mode);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_delete_rec
 *
 * Description: This API deletes a specific records that matches the 
 *              specified search key parameter (field types and
 *              field values)
 *
 * Inputs:  - h_client:  handle to the client object.
 *            ui1_key_field_count:  number of fields that makes up the key
 *            aui4_key_field_types: array of field types that makes up the
 *                                  key.
 *            apv_key_field_values: field values for the key parameter.
 *            ui2_record_index:     The number of record matching the
 *                                  search parameter to delete.  For
 *                                  example: 0 means delete the first
 *                                  record that matches the key parameter,
 *                                  2 means delete third records that
 *                                  matches the key paramter.
 * Outputs: - None.
 *
 * Returns: - DBR_OK     Record is successfully deleted.
 *            DBR_REC_NOT_FOUND The specified record can not be found.
 *            DBR_INV_HANLDE:  The specified handle is not valid.
 *            DBR_INV_ARG:     key_field_count = 0, or key field types array
 *                             or key_field_values array is NULL.
 ----------------------------------------------------------------------------*/
INT32 x_db_delete_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_key_field_count,
    const DB_FIELD_T  aui4_field_types[],  
    const VOID*       apv_field_values[],     
    UINT16		      ui2_record_index 
)
{
    return db_delete_rec(h_client,
                         ui1_key_field_count,
                         aui4_field_types,
                         apv_field_values,
                         ui2_record_index);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_write_unlock
 *
 * Description: This API locks the database before write operations.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     The write ock is released successfully.
 *            DBR_INV_HANDLE Bad handle.
 ----------------------------------------------------------------------------*/
INT32 x_db_write_lock
(
    HANDLE_T h_client
)
{
    return db_write_lock(h_client);
}

/*-----------------------------------------------------------------------------
 * Name:  x_db_write_unlock
 *
 * Description: This API unlocks the database after write operations.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     The write ock is released successfully.
 *            DBR_INV_HANDLE Bad handle.
 ----------------------------------------------------------------------------*/
INT32 x_db_write_unlock
(
    HANDLE_T h_client
)
{
    return db_write_unlock(h_client);
}

/*-----------------------------------------------------------------------------
 * Name: x_db_read_lock 
 *
 * Description: This API locks database for read operation.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/
INT32 x_db_read_lock
(
    HANDLE_T h_client
)
{
    return db_read_lock(h_client);
}

/*-----------------------------------------------------------------------------
 * Name: x_db_read_unlock 
 *
 * Description: This API unlocks database for read operation.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is released successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/
INT32 x_db_read_unlock
(
    HANDLE_T h_client
)
{
    return db_read_unlock(h_client);
}


