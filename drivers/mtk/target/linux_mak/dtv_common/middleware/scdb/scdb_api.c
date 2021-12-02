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
 * $RCSfile: scdb_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains Stream Component DataBase related
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
#include "scdb/x_scdb_api.h"
#include "scdb/scdb_api.h"
#include "scdb/_scdb.h"
#include "conn_mngr/cm.h"

/*----------------------------------------------------------------------
     function  implementation
  ----------------------------------------------------------------------*/
static INT32 scdb_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     scdb_handle_autofree    
};

/*-----------------------------------------------------------------------------
 * Name: scdb_init
 *
 * Description: This API initialize SCDB module.
 *
 * Inputs:  - None.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK     SCDB database is initialized.
 *            SCDBR_FAIL   Cannot create database object and handle.
 ----------------------------------------------------------------------------*/
INT32 scdb_init
(
    VOID
)
{
    /* register autofree function for the SCDB handle. */
    if ( handle_set_autofree_tbl(HT_GROUP_SCDB,
                                 apf_autofree_fct)
         != HR_OK )
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
    }
    return SCDBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_create
 *
 * Description: This API creates a stream component database object and
 *              returns handle to the stream component database object.
 *
 * Inputs:  -  h_connection: Handle to the connection object that 
 *                           contains the stream components.
 *             pt_common_data: Pointer to shared metadata about all 
 *                             the stream components contained in the 
 *                             connection.
 *
 * Outputs: - ph_scdb: A SCDB handle to the database object.
 *
 * Returns: - SCDBR_OK     A new database object and a client handle to
 *                       the database object are initialized successfully.
 *            SCDBR_FAIL   Cannot create database object and handle.
 *            SCDBR_OUT_OF_HANDLE:  no more handle.
 *            SCDBR_OUT_OF_MEM: can not allocate internal scdb structure.
 *            SCDBR_INV_ARG:  pt_scdb is null.
 ----------------------------------------------------------------------------*/
INT32 scdb_create
(
    HANDLE_T              h_connection,
    const SCDB_COMMON_T*  pt_common_data,
    HANDLE_T*             ph_scdb 
    )
{
    INT32           i4_rc;
    CHAR            name[CDB_MAX_DATABASE_NAME_LEN+1];
    HANDLE_TYPE_T   pe_type = INV_HANDLE_TYPE;
    SCDB_OBJ_T*     pt_scdb=NULL;
    HANDLE_T        h_cdb=NULL_HANDLE;

    /* variables to store SCDB record definition. */
    DB_FIELD_INFO_T*  pt_rec_fields;
    DB_FIELD_T*       pt_recfld_type;
    UINT8             ui1_fld_count;
    DB_FIELD_T*       pi4_keyfld_type;
    UINT8             ui1_key_count;
    /*
      Temporary buffer to hold the common data.
    */
    UINT8           aui1_membuf[SCDB_COMMON_DATA_SIZE ];
    
    if ( ph_scdb == NULL ||
         pt_common_data == NULL
        )
    {
        return SCDBR_INV_ARG;
    }

    /* check input arguments. */
    if ( h_connection != NULL_HANDLE                          &&
         x_handle_get_type(h_connection,&pe_type) == HR_OK    &&    
         (pe_type == CMT_CONNECTION)  
        )
    {
        /*
          Get the SCDB record definition and the key definition.
        */
        scdb_rec_def(&pt_rec_fields,
                     &pt_recfld_type,
                     &ui1_fld_count,
                     &pi4_keyfld_type,
                     &ui1_key_count);

        /* Get the name of the SCDB database for this connection. */
        scdb_database_name(h_connection, name);

        /* copy SCDB_COMMON_T structure to memory buffer. */
        scdb_cmn_struct_to_mem(pt_common_data,aui1_membuf,
                               SCDB_COMMON_DATA_SIZE) ;
        
        /* Create a SCDB object. */
        pt_scdb=(SCDB_OBJ_T*) x_mem_alloc(sizeof(SCDB_OBJ_T));
        
        /* Create a core database object. */
        if ( pt_scdb != NULL )
        {
            if ( (i4_rc=db_create(8,
                                  DB_MEM_DYNAMIC,
                                  name,
                                  ui1_fld_count,
                                  pt_rec_fields,
                                  ui1_key_count,
                                  pi4_keyfld_type, /* Key types description.*/
                                  (VOID*)(&pt_scdb), /* Use Private Tag field
                                                        to store addr of SCDB
                                                        handle control
                                                        structure. */
                                  scdb_nfy_fct,  /* Notification fcnt */
                                  &h_cdb))
                 == DBR_OK )
            {
                        
                /* save common data for this SCDB object, note: we need to
                   lock the database before we can call the the write API.
                */
                if ( db_write_lock(h_cdb) == DBR_OK )
                {
                    if ( db_write_common(h_cdb,
                                         aui1_membuf,
                                         (UINT16)SCDB_COMMON_DATA_SIZE) == DBR_OK )
                    {
                        /*
                          unlock the core database.
                        */
                        db_write_unlock(h_cdb);
                        
                        
                        /*
                          allocate a SCDB handle to the SCDB handle control
                          structure. Note: the private tag field is used to
                          store the address of the pt_scdb structure. 
                        */
                        if ( handle_alloc(SCDBT_STREAM_DB,
                                          (VOID*)pt_scdb,
                                          0,   /* No tag value
                                                  for the
                                                  creator. */
                                          scdb_handle_free_cb,
                                          ph_scdb)
                             == HR_OK )
                        {
                            /* Save the SCDB object. */
                            pt_scdb->h_scdb=*ph_scdb;

                            /* Save the handle to the CDB object. */
                            pt_scdb->h_cdb=h_cdb;

                            /*
                              For the handle of this SCDB database
                              object, we save the 'h_connection'
                              value.  The 'h_connection' is used
                              to check for the special permission for
                              the creation handle to delete and modify
                              records in the SCDB.

                              Note: For other clients handle to this
                              SCDB database, this field is NULL_HANDLE,
                              which means it does not have the permission
                              to modify the SCDB database object.
                            */
                            pt_scdb->h_creator=h_connection;

                            /*
                              For the creator of the
                              this SCDB database, we register a
                              NULL client notification function.
                              Note: for other client of this
                              SCDB database, the client will
                              need to provide a notification
                              function.
                            */
                            pt_scdb->pf_nfy_fct = NULL;

                            /*
                               Set the inital compare function info
                            */
                            x_memset(&(pt_scdb->t_cmp_fct_info), 0,
                                     sizeof(SCDB_CMP_FCT_INFO_T));

                            i4_rc = SCDBR_OK;
                        }
                        else
                        {
                            i4_rc=SCDBR_OUT_OF_HANDLE;
                        }
                    }
                    else
                    {
                        /*
                          unlock the database.
                        */
                        db_write_unlock(h_cdb);
                        i4_rc = SCDBR_FAIL;
                    }
                }
                else
                {
                    /* Assertion check. */
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
                }
            }
            else
            {
                i4_rc=scdb_error_code(i4_rc);
            }
        }
        else
        {
            i4_rc=SCDBR_OUT_OF_MEM;
        }
    }
    else
    {
#ifdef DEBUG
        if ( (h_connection != NULL_HANDLE) &&
             x_handle_valid(h_connection)
            )
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=SCDBR_INV_HANDLE;
    }
    
    /*
      If unable to create SCDB handle, remove resource that was used during the
      init process.
    */
    if ( i4_rc != SCDBR_OK )
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,name);
        }
        
        if ( pt_scdb != NULL )
        {
            x_mem_free(pt_scdb);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  scdb_delete
 *
 * Description: This API deletes a SCDB database object.
 *
 * Inputs:  - h_scdb:  handle of the SCDB database object.
 *            
 *      
 * Outputs: - None
 *
 * Returns: - SCDBR_OK     The database object associate with the specified
 *                        handle is freed.
 *            SCDBR_NO_PERMISSION:  No permission to delete the SCDB database.
 *            SCDBR_INV_HANDLE:  Bad handle.
 *            SCDBR_NOT_FOUND:  Specified database object is not found.  
 ----------------------------------------------------------------------------*/

INT32 scdb_delete
(
    HANDLE_T             h_scdb
)
{
    INT32                i4_rc;
    HANDLE_TYPE_T        pe_type;
    SCDB_OBJ_T*          pt_scdb;
    CHAR                 name[CDB_MAX_DATABASE_NAME_LEN+1];
    
    if ( (handle_get_type_obj(h_scdb,
                              &pe_type,
                              ((VOID**) &pt_scdb)) == HR_OK)    &&
         (pe_type == SCDBT_STREAM_DB)                           &&
         (pt_scdb != NULL)
        )
    {
        if ( pt_scdb->h_creator != NULL_HANDLE )
        {
            /* Get the name of the CDB database for this SCDB handle. */
            scdb_database_name(pt_scdb->h_creator, name);

            /* Delete the named CDB object. */
            i4_rc=db_delete(pt_scdb->h_cdb, name);

            /* translate the CDB error code to SCDB error code */
            i4_rc=scdb_error_code(i4_rc);

            /* Don't forget to delete the handle to this SCDB */
            handle_free(h_scdb, TRUE);
        }
        else
        {
            i4_rc = SCDBR_NO_PERMISSION;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_scdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SCDBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  scdb_add_rec
 *
 * Description: This API adds a new record to the database object.
 *
 *              Note: The SCDB database must be locked prior to
 *              calling this API.
 *
 * Inputs:  - h_scdb:  handle to the SCDB database.
 *            pt_stream_desc: pointer to the stream descriptor
 *            pt_scdb_rec:    pointer to the scdb record
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK          Specified record is added
 *            SCDBR_OUT_OF_MEM  Can not add record, no memory left.
 *            SCDBR_INV_HANDLE  bad handle.
 *            SCDBR_INV_ARG     'pt_stream_desc' or 'pt_scdb_rec' is null.
 *            SCDBR_NO_PERMISSION  client handle 'h_scdb' is not the creator
 *                                 of the SCDB database.
 ----------------------------------------------------------------------------*/
INT32 scdb_add_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,
    const SCDB_REC_T*         pt_scdb_rec
)
{
    INT32                i4_rc;
    HANDLE_TYPE_T        pe_type;
    SCDB_OBJ_T*          pt_scdb;

    if ( pt_stream_desc != NULL && pt_scdb_rec != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb) ) == HR_OK)  &&
             (pe_type == SCDBT_STREAM_DB)                          &&
             (pt_scdb != NULL)
            )
        {
            if ( pt_scdb->h_creator != NULL_HANDLE )
            {
                i4_rc=scdb_write_scdb_rec(pt_scdb->h_cdb,
                                          pt_stream_desc,
                                          pt_scdb_rec,
                                          DB_APPEND_RECORD);
            }
            else
            {
                i4_rc=SCDBR_NO_PERMISSION;
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  scdb_del_rec
 *
 * Description: This API deletes a specific record from the database record.
 *
 *              Note: The SCDB database must be locked prior to
 *              calling this API.
 *
 * Inputs:  - h_scdb:      handle to the SCDB object
 *            pt_stream_desc: pointer to a stream component structure 
 *                            specifying the SCDB record to be deleted.
 *                               
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK     Specified record is deleted.
 *            SCDBR_NO_PERMISSION  The client handle 'h_scdb'
 *                                 is not the creator of this SCDB database. 
 *            SCDBR_INV_HANDLE   Bad handle.
 *            SCDBR_REC_NOT_FOUND  The specified record is not found.
 *            SCDBR_INV_ARG      pt_stream_desc is NULL.
 ----------------------------------------------------------------------------*/

INT32 scdb_del_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_desc
)
{
    INT32                i4_rc;
    HANDLE_TYPE_T        pe_type;
    SCDB_OBJ_T*          pt_scdb;

    /* variables to store SCDB record definition. */
    DB_FIELD_INFO_T*     pt_rec_fields;
    DB_FIELD_T*          pt_recfld_type;
    UINT8                ui1_fld_count;
    DB_FIELD_T*          pui4_keyfld_type;
    UINT8                ui1_key_count;    

    /* Array to store field values addresses */
    const VOID*          apv_key_field_addr[2];
    
    if ( pt_stream_desc != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb) ) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                           &&
             (pt_scdb != NULL) )
        {
            if ( pt_scdb->h_creator != NULL_HANDLE )
            {
                /*
                  Get record definition and the key definition.
                */
                scdb_rec_def(&pt_rec_fields,
                             &pt_recfld_type,
                             &ui1_fld_count,
                             &pui4_keyfld_type,
                             &ui1_key_count);

                apv_key_field_addr[0]=(STREAM_TYPE_T*)
                    &(pt_stream_desc->e_type);

                apv_key_field_addr[1]=(VOID*)
                    &(pt_stream_desc->pv_stream_tag);
            
                /* delete record. */
                i4_rc=db_delete_rec(pt_scdb->h_cdb,
                                    ui1_key_count,
                                    pui4_keyfld_type,
                                    apv_key_field_addr,
                                    0);
                i4_rc=scdb_error_code(i4_rc);
            }
            else
            {
                i4_rc=SCDBR_NO_PERMISSION;
            }        
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  scdb_update_rec
 *
 * Description: This API update an existing SCDB. If the record does not
 *              exist, then a new record is created. 
 *
 *              Note: The SCDB database must be locked prior to
 *              calling this API.
 *
 * Inputs:  - h_scdb         handle to the SCDB object
 *            pt_stream_desc pointer to a stream component structure 
 *                           specifying key (stream type and stream tag)
 *                           of the SCDB record to be updated.
 *            pt_scdb_rec    pointer to the SCDB record structure.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK            Record is updated
 *            SCDBR_NO_PERMISSION The client handle 'h_scdb'
 *                                is not the creator of this SCDB database.
 *            SCDBR_INV_HANDLE    Bad handle
 *            SCDBR_INV_ARG       'pt_stream_desc' or 'pt_scdb_rce' is NULL.
 ----------------------------------------------------------------------------*/

INT32 scdb_update_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,
    const SCDB_REC_T*         pt_scdb_rec 
)
{
    INT32                i4_rc;
    HANDLE_TYPE_T        pe_type;
    SCDB_OBJ_T*          pt_scdb;

    if ( pt_stream_desc != NULL && pt_scdb_rec != NULL )
    {  
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb) ) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                           &&
             (pt_scdb != NULL)
            )
        {
            if ( pt_scdb->h_creator != NULL_HANDLE )
            {
                i4_rc=scdb_write_scdb_rec(pt_scdb->h_cdb,
                                          pt_stream_desc,
                                          pt_scdb_rec,
                                          DB_UPDATE_RECORD);
            }
            else
            {
                i4_rc=SCDBR_NO_PERMISSION;
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}



/*-----------------------------------------------------------------------------
 * Name:  scdb_set_common
 *
 * Description: This API sets the shared metadata about the records
 *              stored in the SCDB database.
 *
 * Inputs:  -
 *   h_scdb:  handle to the SCDB.
 *   pt_scdb_common: pointer to a structure containing SCDB
 *                   common data.
 * Outputs: - None.
 *
 * Returns: -
 *   SCDBR_OK    Common data is modified.
 *   SCDBR_INV_HANDLE  Bad SCDB handle.
 *   SCDBR_INV_ARG  "pt_scdb_common" is a NULL pointer
 *   SCDBR_NO_PERMISSION The client handle 'h_scdb'
 *                       does not have the write permission to
 *                       SCDB database.
 ----------------------------------------------------------------------------*/

INT32 scdb_set_common
(
    HANDLE_T			 h_scdb,
    const SCDB_COMMON_T* pt_scdb_common
)
{
    INT32                i4_rc;
    HANDLE_TYPE_T        pe_type;
    SCDB_OBJ_T*          pt_scdb;
    /*
      Temporary buffer to hold the common data.
    */
    UINT8                aui1_membuf[SCDB_COMMON_DATA_SIZE];

    if ( pt_scdb_common != NULL )
    {  
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb) ) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                           &&
             (pt_scdb != NULL)
            )
        {
            if ( pt_scdb->h_creator != NULL_HANDLE )
            {
                /* copy SCDB_COMMON_T structure to memory buffer. */
                scdb_cmn_struct_to_mem(pt_scdb_common,
                                       aui1_membuf,
                                       SCDB_COMMON_DATA_SIZE);
                db_write_common(pt_scdb->h_cdb,
                                aui1_membuf,
                                (UINT16)SCDB_COMMON_DATA_SIZE);
                i4_rc=SCDBR_OK;
            }
            else
            {
                i4_rc=SCDBR_NO_PERMISSION;
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}



/*-----------------------------------------------------------------------------
 * Name:   scdb_lock
 *
 * Description: This API lock the SCDB database for write/modify
 *              operation.
 *
 * Inputs:  - h_scdb:   SCDB handle.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK     Database is locked
 *            SCDBR_NO_PERMISSION: This handle is not allowed to lock the SCDB
 *                                 database.
 *            SCDBR_INV_HANDLE:  bad handle.
 ----------------------------------------------------------------------------*/


INT32 scdb_lock
(
    HANDLE_T            h_scdb 
)
{
    HANDLE_TYPE_T       pe_type;
    SCDB_OBJ_T*         pt_scdb;    
    INT32               i4_rc;

    i4_rc=SCDBR_FAIL;
    
    if ( (handle_get_type_obj(h_scdb,
                              &pe_type,
                              ((VOID**) &pt_scdb)) == HR_OK)   &&
         (pe_type == SCDBT_STREAM_DB)                          &&
         (pt_scdb != NULL)
        )
    {
        if ( pt_scdb->h_creator != NULL_HANDLE )
        {
            if ( db_write_lock(pt_scdb->h_cdb) == DBR_OK )
            {
                i4_rc=SCDBR_OK;
            }
            else
            {
                /* Assertion check. */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
            }
        }
        else
        {
            i4_rc=SCDBR_NO_PERMISSION;
        }        
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_scdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=SCDBR_INV_HANDLE;
    }    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  scdb_unlock
 *
 * Description: This API unlocks the SCDB database after the write/modify
 *              operation.
 *              
 * Inputs:  - h_scdb:   SCDB handle.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK     SCDB database is unlocked.
 *            SCDBR_NO_PERMISSION: This handle is not allowed to unlock 
 *                                 the SCDB database.
 *            SCDBR_INV_HANDLE:  bad handle.
 ----------------------------------------------------------------------------*/

INT32 scdb_unlock
(
    HANDLE_T            h_scdb 
)
{
    HANDLE_TYPE_T       pe_type;
    SCDB_OBJ_T*         pt_scdb;       
    INT32               i4_rc;

    i4_rc = SCDBR_FAIL;
    
    if ( (handle_get_type_obj(h_scdb,
                              &pe_type,
                              ((VOID**) &pt_scdb)) == HR_OK)   &&
         (pe_type == SCDBT_STREAM_DB)                          &&
         (pt_scdb != NULL)
        )
    {
        if ( pt_scdb->h_creator != NULL_HANDLE )
        {    
            if ( db_write_unlock(pt_scdb->h_cdb) == DBR_OK )
            {
                i4_rc=SCDBR_OK;
            }
            else
            {
                /* Assertion check. */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
            }
        }
        else
        {
            i4_rc=SCDBR_NO_PERMISSION;
        }        
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_scdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=SCDBR_INV_HANDLE;
    }        
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  x_scdb_open
 *
 * Description: This API returns a handle to the SCDB object for a
 *              specified connection. 
 *
 * Inputs:  -  h_connection:   Specifies the connection handle that was 
 *                             used to create the SCDB object.
 *             pv_tag:         Private tag value associated with the 
 *                             calling client.
 *             pf_nfy:         Client notification function.
 *
 * Outputs: - ph_scdb:  pointer to a SCDB handle.
 *
 * Returns: - SCDBR_OK     A new handle is created
 *            SCDBR_OUT_OF_HANDLE:  No more handle available.
 *            SCDBR_INV_HANDLE: bad 'h_connection' handle.
 *            SCDBR_OUT_OF_MEM:     Out of memory.
 *            SCDBR_INV_ARG:    ph_scdb is NULL.
 *            SCDBR_NOT_FOUND:  Specified database object is not found.  
 ----------------------------------------------------------------------------*/
INT32 x_scdb_open
(
    HANDLE_T        h_connection,
    HANDLE_T*       ph_scdb,
    VOID*           pv_tag,
    x_scdb_nfy_fct  pf_nfy
)
{
    HANDLE_TYPE_T       pe_type = INV_HANDLE_TYPE;
    INT32               i4_rc;    
    CHAR                name[CDB_MAX_DATABASE_NAME_LEN+1];
    SCDB_OBJ_T*         pt_scdb=NULL;
    HANDLE_T            h_cdb=NULL_HANDLE;
    
    i4_rc=SCDBR_INV_ARG;

    if ( ph_scdb == NULL )
    {
        return i4_rc;
    }
    
    /* check input arguments. */
    if ( (h_connection != NULL_HANDLE)                          &&
         (x_handle_get_type(h_connection,&pe_type) == HR_OK)    &&
         (pe_type == CMT_CONNECTION)
        )
    {
        /* Create a SCDB object. */
        pt_scdb=(SCDB_OBJ_T*) x_mem_alloc(sizeof(SCDB_OBJ_T));
        
        /* Create a core database object. */
        if ( pt_scdb != NULL )
        {
            /* Get the name of the SCDB database for this connection. */
            scdb_database_name(h_connection, name);

            /* Get the handle to the CoreDB database object with the
               specified name. Note: the private tag field is used to
               store the address of the pt_scdb structure. 
            */
            if ( (i4_rc=db_get_handle_by_name(name,
                                              (VOID*)pt_scdb,
                                              scdb_nfy_fct,
                                              &h_cdb))
                 == DBR_OK)
            {
                /*  allocate a SCDB handle to the SCDB handle control
                    structure. 
                */
                if ( handle_alloc(SCDBT_STREAM_DB,
                                  (VOID*)pt_scdb,
                                  pv_tag,
                                  scdb_handle_free_cb,
                                  ph_scdb)
                     == HR_OK )
                {
                    /* Save the SCDB object. */
                    pt_scdb->h_scdb=*ph_scdb;

                    /* Save the handle to the CDB object. */
                    pt_scdb->h_cdb=h_cdb;

                    /*
                       Set this field is NULL_HANDLE, which means
                       this handle does not have the permission
                       to modify the SCDB database object.
                    */
                    pt_scdb->h_creator=NULL_HANDLE;

                    /*
                       Register (save) client's notification
                       function with SCDB object.
                    */
                    pt_scdb->pf_nfy_fct = pf_nfy;

                    /*
                       Set the inital compare function info
                    */
                    x_memset(&(pt_scdb->t_cmp_fct_info), 0,
                             sizeof(SCDB_CMP_FCT_INFO_T));

                    i4_rc = SCDBR_OK;
                }
                else
                {
                    i4_rc=SCDBR_OUT_OF_HANDLE;
                }
            }
            else
            {
                i4_rc=scdb_error_code(i4_rc);
            }
        }
        else
        {
            i4_rc=SCDBR_OUT_OF_MEM;
            
        }
    }
    else
    {
#ifdef DEBUG
        if ( (h_connection != NULL_HANDLE) &&
             x_handle_valid(h_connection)
            )
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SCDBR_INV_HANDLE;
    }
    
    /*
      If unable to create SCDB handle, remove resource that was used during the
      init process.
    */
    if ( i4_rc != SCDBR_OK )
    {
        if ( pt_scdb != NULL )
        {
            x_mem_free(pt_scdb);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  x_scdb_close
 *
 * Description: This API frees the specified handle to the SCDB database.
 *
 * Inputs:  - h_scdb:  handle to be freed.
 *
 * Outputs: - None.
 *
 * Returns: - SCDBR_OK    SCDB handle and its resource are free.
 *            SCDBR_INV_HANDLE  Bad SCDB handle.
 ----------------------------------------------------------------------------*/

INT32 x_scdb_close
(
    HANDLE_T        h_scdb
)
{
    INT32              i4_rc;
    HANDLE_TYPE_T      pe_type;
    SCDB_OBJ_T*        pt_scdb;       
    
    if ( (handle_get_type_obj(h_scdb,
                              &pe_type,
                              ((VOID**) &pt_scdb)) == HR_OK)   &&
         (pe_type == SCDBT_STREAM_DB)                          &&
         (pt_scdb != NULL)
        )
    {
        handle_free(h_scdb,TRUE);
        i4_rc=SCDBR_OK;
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_scdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=SCDBR_INV_HANDLE;
    }
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name:  x_scdb_get_common
 *
 * Description: This API get the shared metadata about the records stored
 *              in the SCDB database object.
 *
 * Inputs: 
 *   h_scdb:  handle to the SCDB object.
 *
 * Outputs: 
 *   pt_scdb_common: pointer to a structure containing SCDB common data.
 *
 * Returns: -
 *   SCDBR_OK          Common data is copied.
 *   SCDBR_INV_HANDLE  Bad SCDB handle.
 --------------------------------------------------------------------------*/
INT32 x_scdb_get_common
(
    HANDLE_T            h_scdb,
    SCDB_COMMON_T*      pt_scdb_common 
)
{
    HANDLE_TYPE_T       pe_type;
    SCDB_OBJ_T*         pt_scdb;     
    INT32               i4_rc;
    /*
      Buffer to hold the common data.
    */
    UINT8           aui1_membuf[SCDB_COMMON_DATA_SIZE ];

    if ( pt_scdb_common != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb)) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                          &&
             (pt_scdb != NULL)
            )
        {   
            /*
              Copy the common data into a memory buffer.
            */
            if ( (i4_rc=db_read_common(pt_scdb->h_cdb,
                                       aui1_membuf,
                                       (UINT16)SCDB_COMMON_DATA_SIZE))
                 == DBR_OK )
            {
                /* convert memory buffer data into SCDB_COMMON_T structure. */
                if ( scdb_mem_2_cmn_struct(aui1_membuf,
                                           pt_scdb_common,
                                           SCDB_COMMON_DATA_SIZE)
                     == SCDBR_OK )
                {
                    i4_rc=SCDBR_OK;
                }
                else
                {
                    i4_rc=SCDBR_FAIL;
                }
            }
            else
            {
                i4_rc=scdb_error_code(i4_rc);
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  x_scdb_get_rec
 *
 * Description: This API gets a record with a specified stream type
 *              and stream_id value from the database.
 *
 * Inputs:  -
 *    h_scdb          handle to the SCDB object.
 *    pt_stream_desc  pointer to the structure specifying the
 *                    stream type and stream id.
 *
 * Outputs: -
 *    pt_scdb_rec  pointer to the structure containing the
 *                 stream component data.
 *
 * Returns: -
 *    SCDBR_OK:             A SCDB_REC_T structure is returned
 *    SCDBR_INV_HANDLE: Bad 'h_scdb' handle.
 *    SCDBR_INVA_ARG:       'pt_stream_descr' or 'pt_scdb_rec' 
 *                          is NULL.
 *    SCDBR_REC_NOT_FOUND:  Specified record is not found.
 *
 ----------------------------------------------------------------------------*/
INT32 x_scdb_get_rec
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,
    SCDB_REC_T*               pt_scdb_rec
)
{
    HANDLE_TYPE_T       pe_type;
    SCDB_OBJ_T*         pt_scdb;
    INT32               i4_rc;
    
    if ( pt_scdb_rec != NULL && pt_stream_desc != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb)) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                          &&
             (pt_scdb != NULL)
            )
        {
            if ( pt_stream_desc->pv_stream_tag != 0 )
            {
                i4_rc=scdb_read_cdb_rec(pt_scdb->h_cdb,
                                        pt_stream_desc,
                                        pt_scdb_rec,
                                        0);
            }
            else
            {
                /*
                  Look for the default stream type.
                */
                i4_rc = scdb_get_default_rec_by_stream_type(h_scdb,
                                                            pt_stream_desc,
                                                            pt_scdb_rec);
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  x_scdb_get_rec_by_idx
 *
 * Description: This API gets a record from the SCDB database object. The
 *              record is specified by the stream_type_t value and an index
 *              value.  If the specified 'stream_type_t' is ST_UNKONWN,
 *              then SCDB record matching any stream type is returned to
 *              the caller.
 *              To get the next record, the index counter 'ui2_idx'
 *              is incremented. 
 *  
 *
 * Inputs:  - h_scdb          client handle 
 *            e_stream_type   specify the stream type.
 *            ui2_idx         index value for selecting the record that
 *                            match the specified stream type. 0 being 
 *                            the first match, 1 the second match, etc.
 *            
 * Outputs: - pt_stream_desc: pointer to a stream descriptor structure
 *                            holding the stream type and stream id of
 *                            the record returned.
 *
 *            pt_scdb_rec:    pointer to a SCDB record.
 *            pui4_db_version: current version number of the database.
 *
 * Returns: - SCDBR_OK:              A SCDB_REC_T structure is returned
 *            SCDBR_INV_HANDLE:      Bad 'h_scdb' handle.
 *            SCDBR_INVALIE_ARG:     'pt_stream_descr' or 'pt_scdb_rec' 
 *                                   is NULL.
 *            SCDBR_REC_NOT_FOUND:   Specified record is not found.
 *            SCDBR_DB_MODIFIED:     Between read operations of the
 *                                   stream component database using
 *                                   different idx value, the
 *                                   records has been modified.  The
 *                                   application should re-starts
 *                                   the read operation with idx=0
 ----------------------------------------------------------------------------*/
INT32 x_scdb_get_rec_by_idx
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16              ui2_idx,
    STREAM_COMP_ID_T*   pt_stream_desc,
    SCDB_REC_T*         pt_scdb_rec,
    UINT32*             pui4_db_version
)
{
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    SCDB_OBJ_T*       pt_scdb;

    if ( pt_scdb_rec != NULL && pt_stream_desc != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb)) == HR_OK)   &&
             (pe_type == SCDBT_STREAM_DB)                          &&
             (pt_scdb != NULL)
            )
        {
            if (pt_scdb->t_cmp_fct_info.pf_sort_cmp == NULL)
            {
                i4_rc=scdb_read_next_cdb_rec(pt_scdb->h_cdb,
                                             e_stream_type,
                                             pt_stream_desc,
                                             pt_scdb_rec,
                                             ui2_idx,
                                             pui4_db_version);
            }
            else
            {
                i4_rc=scdb_read_sorted_cdb_rec(pt_scdb->h_cdb,
                                               &(pt_scdb->t_cmp_fct_info),
                                               e_stream_type,
                                               pt_stream_desc,
                                               pt_scdb_rec,
                                               ui2_idx,
                                               pui4_db_version);
            }
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  x_scdb_get_num_recs
 *
 * Description: This API returns the number of records matching the specified
 *              stream type.
 *
 * Inputs:  - h_scdb:  handle to the SCDB database.
 *            e_stream_type:  specify the stream type.
 *
 * Outputs: - pui2_num_recs:  pointer to a variable holding the result.
 *            pui4_db_version: current version number of the SCDB database.
 *
 * Returns: - SCDBR_OK          Number of records found is returned
 *            SCDBR_INV_ARG     'pui2_num_recs' is a NULL pointer.
 *            SCDBR_INV_HANDLE   Bad 'h_scdb' handle. 
 ----------------------------------------------------------------------------*/
INT32 x_scdb_get_num_recs
(
    HANDLE_T            h_scdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_db_version    
)
{
    INT32               i4_rc;
    HANDLE_TYPE_T     pe_type;
    SCDB_OBJ_T*       pt_scdb;

    if ( pui2_num_recs != NULL )
    {
        if ( (handle_get_type_obj(h_scdb,
                                  &pe_type,
                                  ((VOID**) &pt_scdb) ) == HR_OK)  &&
             (pe_type == SCDBT_STREAM_DB)                          &&
             (pt_scdb != NULL)
            )
        {
            i4_rc=scdb_get_num_of_cdb_rec(pt_scdb->h_cdb,
                                          e_stream_type,
                                          pui2_num_recs,
                                          pui4_db_version);
        }
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_scdb))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=SCDBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=SCDBR_INV_ARG;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: x_scdb_reg_cmp_fcts
 *
 * Description: This function register compare functions for
 *              sorting operation.
 *
 *              The compare function are SCDB handle specific.
 *
 * Inputs: 
 *   h_scdb       Handle to the SCDB object.
 *
 *   b_global     TRUE:  compare functions are global scope.
 *                FALSE: compare functions are SCDB handle specific.
 *
 *   pf_sort_cmp  Compare function used for sorting.
 *
 *   pv_tag
 *
 * Outputs:
 *   None.
 *
 * Returns: 
 *   SCDBR_OK         Version id is returned. 
 *   SCDBR_INV_HANDLE Bad 'h_scdb' handle.
 *   SCDBR_INV_ARG    'pf_sort_cmp' is a NULL pointer.
 -----------------------------------------------------------------------*/
INT32 x_scdb_reg_cmp_fcts
(
    HANDLE_T               h_scdb,
    VOID*                  pv_tag,
    x_scdb_sort_cmp        pf_sort_cmp
)
{
    INT32               i4_rc;
    SCDB_OBJ_T*         pt_scdb;
    HANDLE_TYPE_T       pe_type;

    if ( (handle_get_type_obj(h_scdb,&pe_type,((VOID**) &pt_scdb)) == HR_OK) &&
         (pe_type == SCDBT_STREAM_DB)                                        &&
         (pt_scdb  != NULL)
        )
    {
        /*
           Attached the compare functions with SCDB object.
        */
        pt_scdb->t_cmp_fct_info.pf_sort_cmp   = pf_sort_cmp;
        pt_scdb->t_cmp_fct_info.pv_tag        = pv_tag;

        i4_rc = db_get_database_obj(pt_scdb->h_cdb,
                                    &(pt_scdb->t_cmp_fct_info.pt_cdb_database));
        if ( i4_rc == DBR_OK )
        {
            i4_rc = SCDBR_OK;
        }
        else
        {
            i4_rc = SCDBR_FAIL;
            pt_scdb->t_cmp_fct_info.pf_sort_cmp   = NULL;
            pt_scdb->t_cmp_fct_info.pv_tag    = NULL;
        }

        if (pt_scdb->t_cmp_fct_info.pf_sort_cmp != NULL)
        {
            UINT32      ui4_version = SCDB_NULL_VER_ID;

            scdb_sort_cdb_rec(pt_scdb->h_cdb,
                              &(pt_scdb->t_cmp_fct_info),
                              TRUE,
                              &ui4_version);
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_scdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SCDBR_INV_HANDLE;
    }

    return i4_rc;
}


