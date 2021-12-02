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
#include "cdb/_cdb.h"
#include "cdb/cdb_record.h"
#include "cdb/cdb_fs.h"
#include "util/x_sort_search.h"
#include "cl/zip_eng/x_cl_zip_eng.h"
#include "cdb/cdb_dbg.h"
#include "cdb/cdb_ser.h"
/*-----------------------------------------------------------------------------
  data declaraions
  ----------------------------------------------------------------------------*/
/* Database engine metadata */
static CDB_NAME_DB_REF_T* pt_name_db_obj=NULL;

static UINT16 ui2_do_max_count=0;   /* max number of database objects
                                       allowed */

static UINT16 ui2_do_count;       /* current number of database objects. */

/*---------------------------------------------------------------
 *   Autofree function for AEE
 *---------------------------------------------------------------*/
static INT32 cdb_handle_autofree
(
    HANDLE_T       h_handle,
    HANDLE_TYPE_T  e_type
)
{
    return handle_free(h_handle, FALSE);
}

static handle_autofree_fct   apf_autofree_fct[]=
{
     cdb_handle_autofree    
};

/*-----------------------------------------------------------------------------
 * Name: db_core_init 
 *
 * Description: This API initialize the core database engine
 *
 * Inputs:  - max number of database object allowed by the engine.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     Core DB is initialized successfully.
 *            DBR_FAIL   Cannot initialize the engine.
 *            DBR_INV_ARG ui2_max_num must be greater than 0.
 ----------------------------------------------------------------------------*/
INT32 db_core_init   ( UINT16 ui2_max_num )
{
    UINT16  ui2_i;
    
    if ( ui2_max_num > 0 )
    {
        DBG_INFO(("{CDB} init cdb engine: max num of database objects: %d\n",ui2_max_num));
        
        /*
          CoreDb can only be initialized once.
        */
        if ( pt_name_db_obj == NULL )
        {
            pt_name_db_obj=(CDB_NAME_DB_REF_T*) x_mem_alloc(
                sizeof(CDB_NAME_DB_REF_T)*ui2_max_num);
            if ( pt_name_db_obj != NULL )
            {
                ui2_do_max_count=ui2_max_num;
                ui2_do_count=0;

                /* Initialize the name/database object array to null value. */
                for ( ui2_i = 0; ui2_i < ui2_do_max_count; ui2_i++ )
                {
                    pt_name_db_obj[ui2_i].pt_database_obj=NULL;
                }

                /*
                  Initialize a global lock for the
                  database engine.
                */
                db_create_internal_lock();

                /* register autofree function for the CDB handle. */
                if ( handle_set_autofree_tbl(HT_GROUP_CDB,
                                             apf_autofree_fct)
                     != HR_OK )
                {
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_CANNOT_SET_AUTOFREE);
                }
                
                /*
                  Initialize CLI support
                */
#ifdef CLI_LVL_ALL
                cdb_cli_init();
#endif
            }
            else
            {
                return DBR_FAIL;
            }
        }
        else
        {
            return DBR_FAIL;
        }
    }
    else
    {
        return DBR_INV_ARG;
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_create
 *
 * Description: This API creates a database object and client object that
 *              points to the database object.
 *
 * Inputs:  -  ui2_max_record:  maximum number of records for this
 *                              database object.
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
INT32 db_create
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
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    UINT16            ui2_i;
    INT32             i4_rc;

    /* Argument checking.*/
    if ( (ui2_max_record == 0)            ||
         (s_db_name == NULL  )            ||
         (ui1_count == 0     )            ||
         (at_field_info == NULL )         ||
         (ui1_key_field_count == 0 )      ||
         (ui1_key_field_count > MAX_NUM_KEY_PARM_FIELDS)  ||
         (aui4_key_field_types == NULL )  ||
         (ph_client == NULL )
        )
    {
        return DBR_INV_ARG;
    }

    if ( e_memory_policy != DB_MEM_STATIC     &&
         e_memory_policy != DB_MEM_DYNAMIC )
    {
        return DBR_INV_ARG;
    }

    i4_rc=DBR_OK;
    pt_do=NULL;
    pt_co=NULL;
    *ph_client = NULL_HANDLE;    
    
    /* Lock the database engine. */
    db_get_internal_lock();


    /*
      Do not allow duplicate database name,
      perform the check before we continue.
    */
    for ( ui2_i = 0; ui2_i < ui2_do_max_count ; ui2_i++ )
    {
        /*
          Check if the specified name has already been used.
        */
        if ( pt_name_db_obj[ui2_i].pt_database_obj != NULL  &&
             x_strncmp(pt_name_db_obj[ui2_i].name,
                       s_db_name,
                       CDB_MAX_DATABASE_NAME_LEN) == 0
           )
        {
            /* unlock the database. */
            db_release_internal_lock();
            return DBR_INV_NAME;
        }
    }
    
    /* check the resource limit. */
    if ( ui2_do_max_count > 0             &&
         ui2_do_count <=  ui2_do_max_count )
    {
        /*
          Create the database object.
        */
        pt_do = (CDB_DATA_OBJ_T*) x_mem_alloc(sizeof(CDB_DATA_OBJ_T));
        if ( pt_do != NULL )
        {
            pt_do->ui4_db_version       = 1;  /* version id start with 1.*/
            pt_do->ui2_max_count        = ui2_max_record;
            pt_do->ui2_record_count     = 0;
            pt_do->ui2_record_width     = 0;
            pt_do->ui1_key_field_count  = ui1_key_field_count;
            pt_do->pt_fields            = NULL;
            pt_do->ui2_fld_offset       = NULL;
            pt_do->ui2_fld_width        = NULL;
            pt_do->ui2_last_mod_count   = 0;
            pt_do->ui2_last_del_count   = 0;
            pt_do->ui2_last_add_count   = 0;
            pt_do->ui1_common_mod_flg   = 0;
            pt_do->pui1_common_data     = NULL;
            pt_do->ui2_common_size      = 0;
            pt_do->h_owner              = NULL_HANDLE;
            pt_do->pf_sort_cmp          = NULL;
            pt_do->pf_search_cmp        = NULL;
            pt_do->pf_client_store_fct  = NULL;
            pt_do->pv_tag_data          = NULL;
            pt_do->pt_strbuf            = NULL;
            pt_do->pt_membuf            = NULL;
            x_strcpy(pt_do->s_cmpres_eng, SN_CL_ZIP);
            
            for ( ui2_i = 0 ; ui2_i < (UINT16) ui1_key_field_count ; ui2_i++ )
            {
                pt_do->aui4_key_field_types[ui2_i] =
                    aui4_key_field_types[ui2_i];
            }
            pt_do->h_rwloc           = NULL_HANDLE;
        
            /* initialize a Read-Write lock */
            if ( rwl_create_lock(&(pt_do->h_rwloc)) == RWLR_OK )
            {
                /*
                  allocate memory to hold the record definition.
                  record definition consists of an array of field
                  description, offsets, and field width (in bytes).
                */
                pt_do->pt_fields=
                    (DB_FIELD_INFO_T*)x_mem_alloc(
                        sizeof(DB_FIELD_INFO_T) * ((SIZE_T) ui1_count));
            
                pt_do->ui2_fld_offset=
                    (UINT16*)x_mem_alloc(sizeof(UINT16) * ((SIZE_T) ui1_count));
            
                pt_do->ui2_fld_width=
                    (UINT16*)x_mem_alloc(sizeof(UINT16) * ((SIZE_T) ui1_count));
            
                if ( pt_do->pt_fields        != NULL  &&
                     pt_do->ui2_fld_offset   != NULL  &&
                     pt_do->ui2_fld_width    != NULL 
                    )
                {
                    /* Create the internal data record structure
                       from the record definition provided by
                       the client. */
                    if ( db_create_record_def(pt_do, ui1_count,
                                              at_field_info) == DBR_OK )
                    {
                        /* Initialize the memory buffer to store record. */
                        if ( db_memory_manager_init(&(pt_do->t_memory),
                                                    pt_do->ui2_record_width,
                                                    pt_do->ui2_max_count,
                                                    e_memory_policy) == DBR_OK )
                        {
                            /* Initialize the handle link structure. */
                            if ( handle_link_init( &(pt_do->t_link))
                                 != HR_OK )
                            {
                                i4_rc=DBR_FAIL;
                            }
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
                    i4_rc = DBR_OUT_OF_MEMORY;
                }
            }
            else
            {
                i4_rc = DBR_NO_WRITE_LOCK;
            }
        }
        else
        {
            i4_rc = DBR_OUT_OF_MEMORY;
        }
    }
    else
    {
        i4_rc = DBR_DB_LIMIT_EXCEED;
    }
    
    /* Next Create the client object. */
    if ( i4_rc == DBR_OK               &&
         db_create_client_obj(pt_do,&pt_co,pv_tag,pf_nfy)
         == DBR_OK )
    {
        /* Create a handle to the client object. */
        if ( handle_alloc(CDBT_COREDB, (VOID*)pt_co,
                          pv_tag, cdb_handle_free_cb,
                          ph_client) == HR_OK
            )
        {
            /* Database object and client object created, now update the
               core database meta data, and link the  client handle to the
               database object.
            */
    
            /*
              Link the client handle to the database object.
            */
            if ( handle_link (&(pt_do->t_link), *(ph_client)) == HR_OK )
            {
                /*
                  Save the database object and its name in the internal array.
                */
                for ( ui2_i = 0; ui2_i < ui2_do_max_count ; ui2_i++ )
                {
                    /*
                      Find a empty slot int database name array. Insert
                      the new database name and its handle.
                    */
                    if ( pt_name_db_obj[ui2_i].pt_database_obj==NULL )
                    {
                        pt_name_db_obj[ui2_i].pt_database_obj=pt_do;
                        
                        x_strncpy(pt_name_db_obj[ui2_i].name,
                                  s_db_name,
                                  CDB_MAX_DATABASE_NAME_LEN);
                        /* make sure the string is terminated by NULL. */
                        pt_name_db_obj[ui2_i].name[CDB_MAX_DATABASE_NAME_LEN]
                            = '\0';
                        
                        ui2_do_count++;

                        /*
                          Save the owner handle to this database object.
                        */
                        pt_do->h_owner=*(ph_client);

                        break;
                    }
                }
            }
            else
            {
                i4_rc=DBR_FAIL;
            }
        }
        else
        {
            i4_rc = DBR_OUT_OF_HANDLE;
        }
    }
    else
    {
        i4_rc = DBR_OUT_OF_MEMORY;
    }

    /* Unlock the database */
    db_release_internal_lock();
    
    if ( i4_rc != DBR_OK )
    {
        /* Free the client and database object. */
        db_delete_client_obj(pt_co);
        db_delete_data_obj(pt_do);

        DBG_ERROR(("{CDB} Can not create DB object [name: %s]\n", s_db_name)); 
    }
    else
    {
        DBG_API(("{CDB} DB object [name: %s] is created.\n",s_db_name));
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_get_handle_by_name
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
INT32 db_get_handle_by_name 
(
    CHAR*             s_db_name,
    VOID*             pv_tag,
    db_notify_fct     pf_nfy,
    HANDLE_T*         ph_client
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    UINT32            ui4_i;
    INT32             i4_rc;

    /* Argument checking.*/
    if ( (s_db_name == NULL  )            ||
         (ph_client == NULL )
        )
    {
        return DBR_INV_ARG;
    }
    
    /*
      Find the 'name' database object.
    */
    pt_do=NULL;
    pt_co=NULL;

    /* Lock the database. */
    db_get_internal_lock();
    
    for ( ui4_i = 0; ui4_i < ui2_do_max_count ; ui4_i++ )
    {
        if ( pt_name_db_obj[ui4_i].pt_database_obj != NULL &&
             x_strncmp(pt_name_db_obj[ui4_i].name,
                       s_db_name, CDB_MAX_DATABASE_NAME_LEN) == 0 )
        {
            pt_do=pt_name_db_obj[ui4_i].pt_database_obj;
            break;
        }
    }
     
    /* Next Create the client object. */
    if ( pt_do != NULL &&
         db_create_client_obj(pt_do,&pt_co,pv_tag,pf_nfy)
         == DBR_OK )
    {
        /* Create a handle to the client object. */
        if ( handle_alloc(CDBT_COREDB, (VOID*)pt_co,
                          pv_tag, cdb_handle_free_cb,
                          ph_client) == HR_OK
            )
        {
            /* Link the handle to the database object. */
            handle_link (&(pt_do->t_link), *(ph_client) );
            i4_rc=DBR_OK;
        }
        else
        {
            i4_rc = DBR_OUT_OF_HANDLE;
        }
    }
    else
    {
        i4_rc = DBR_NOT_FOUND;
    }
    
    /* Don't forget to unlock the database */
    db_release_internal_lock();
    
    if ( i4_rc != DBR_OK )
    {
        db_delete_client_obj(pt_co);
        DBG_ERROR(("{CDB} Can not create a handle to DB object [name: %s]\n", s_db_name)); 
    }
    else
    {
        DBG_API(("{CDB} Handle to DB object [handle: %d name: %s] is created\n", (*ph_client), s_db_name));
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_delete
 *
 * Description: This API delete a named database object.  
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

INT32 db_delete
(
    HANDLE_T          h_client,
    CHAR*             s_db_name
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    UINT32            ui4_i;
    INT32             i4_rc;
    HANDLE_T          h_myself;
    HANDLE_TYPE_T     pe_type;
    DB_COND_T         e_cond;
    HANDLE_T          h_rwloc;
    CRIT_STATE_T      t_state;

    /* Argument checking.*/
    if ( s_db_name == NULL )
    {
        return DBR_INV_ARG;
    }
    
    pt_do=NULL;
    pt_co=NULL;
    i4_rc=DBR_OK;

    /* Lock the core database. */
    db_get_internal_lock();

    /*
      Find the 'name' database object.
    */
    for ( ui4_i = 0; ui4_i < ui2_do_max_count ; ui4_i++ )
    {
        if ( pt_name_db_obj[ui4_i].pt_database_obj != NULL &&
             x_strncmp(pt_name_db_obj[ui4_i].name,
                       s_db_name, CDB_MAX_DATABASE_NAME_LEN) == 0 )
        {
            /* Remove the named database from the internal database. */
            pt_do=pt_name_db_obj[ui4_i].pt_database_obj;

            /* null out the database name. */
            pt_name_db_obj[ui4_i].name[0]='\0';
            pt_name_db_obj[ui4_i].pt_database_obj = NULL ;
            ui2_do_count--;
            break;
        }
    }
    /* Unlock the core database. */
    db_release_internal_lock();
    
    /* Delete the database object. */
    if ( pt_do != NULL  )
    {
        t_state=x_crit_start();
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                    ((VOID**) &pt_co)) == HR_OK) &&
             (pe_type == CDBT_COREDB)                            &&
             (pt_co->p_data_obj == pt_do )
            )
        {
            h_rwloc = pt_co->p_data_obj->h_rwloc;
            x_crit_end(t_state);
        
            /* Get the write lock for this database object. */
            if ( rwl_write_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
            {
                /*
                  Delete the read-write lock for the CDB. This
                  prevent other pending read/write operation
                  from proceeding.
                  Note: this call puts the RWL lock
                  in the deleted state, which prevent other thread
                  from getting the RWL lock again. The actual resource
                  for the RW Lock is not release until the last call
                  to rwl_release_lock() is called.
                */
                rwl_delete_lock(h_rwloc);
                
                /*
                  Notify all othe clients that this database object
                  is been deleted.
                */
                e_cond=DB_CLOSED;    
                h_myself=h_client;
                /*
                  Need to iterate through all the client's handle,
                  free the client handle, and notify the client
                  that its handle is no longer valid.
                */
                db_notify_all(h_myself,
                              pt_do,
                              e_cond,
                              DB_REASON_UNK,
                              pt_do->ui4_db_version);
                /*
                  Release the write lock.
                */
                if ( rwl_release_lock(h_rwloc) == RWLR_OK )
                {
                    i4_rc=DBR_OK;
                }
                else
                {
                    /*
                      Can not unlock, Abort.
                    */
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_UNLOCK_FAILED);
                }
                
                /* After we delete all other clients to this database object,
                   now we can delete the current (this) client handle and
                   its client object. */
                handle_free(h_myself, TRUE);
        
                /*
                  Ok to delete the database object
                */
                db_delete_data_obj(pt_do);

                DBG_API(("{CDB} DB object [DB name: %s, handle: %d] deleted\n", s_db_name, h_client));        
            }
            else
            {
                i4_rc=DBR_INV_HANDLE;
            }
        }
        else
        {
            /* Don't forget to end critical section. */
            x_crit_end(t_state);
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc = DBR_INV_HANDLE;
        }
    }
    else
    {
        /* Did not find the specified database. */
        i4_rc=DBR_NOT_FOUND;
    }    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_clean
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

INT32 db_clean
(
    HANDLE_T          h_client
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    DB_COND_T         e_cond;
    CRIT_STATE_T      t_state;
    UINT32            ui4_reason;
    
    pt_do=NULL;
    pt_co=NULL;
    i4_rc=DBR_OK;

    t_state=x_crit_start();
    if ( (handle_get_type_obj(h_client, &pe_type, (VOID**) &pt_co)) == HR_OK  &&
         (pe_type == CDBT_COREDB)                                             &&
         (pt_co->p_data_obj != NULL )
        )
    {
        x_crit_end(t_state);
        pt_do=pt_co->p_data_obj;
        /*
          Notify all othe clients that records for this
          database object are been deleted.
        */
        e_cond=DB_UPDATING;
        ui4_reason=DB_RECORD_DEL;
            
        db_notify_all(h_client,
                      pt_do,
                      e_cond,
                      ui4_reason,
                      pt_do->ui4_db_version);


        /* empty all records from the memory buffer. */
        db_remove_all_records(pt_do->t_memory);

        /* remove all string objects. */
        if ( pt_do->pt_strbuf != NULL )
        {
            x_str_buf_del_all_string(pt_do->pt_strbuf);
        }

        /* remove all client private data. */
        if ( pt_do->pt_membuf != NULL )
        {
            x_mem_buf_del_all_objects(pt_do->pt_membuf);
        }
       
        /* delete the internal sorted list object and set the
           sortlist object to NULL.
        */
        db_delete_sl_obj(pt_co->pt_sl);
        pt_co->pt_sl=NULL;
        
        /* Updated the internal state of the Database object:
           Increment version number, deleted record count
           and the record counts.
        */
        if (pt_do->ui2_record_count > 0) /* no incrm when cleaning empty DB */
        {
            pt_do->ui4_db_version = CDB_INCRM_VER(pt_do->ui4_db_version);
        }
        
        pt_do->ui2_last_del_count=pt_do->ui2_record_count;
        pt_do->ui2_record_count=0;

        /*
          Notify all other clients that this database object has
          been modified (all records deleted.)
        */
        e_cond=DB_UPDATED;
        ui4_reason=DB_RECORD_DEL;

        i4_rc=db_notify_all(h_client,
                            pt_do,
                            e_cond,
                            ui4_reason,
                            pt_do->ui4_db_version);

        DBG_API(("{CDB} All records from DB object [handle: %d] are removed.\n", h_client));      
    }
    else
    {
        /* Don't forget to end critical section. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_read_rec
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
 *            
 *
 * Outputs: - apv_fld_value_addr: array of pointer address to store the
 *                                  field values.
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

INT32 db_read_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[],
    UINT8             ui1_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    VOID*             apv_fld_value_addr[],
    UINT16            ui2_record_index,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T* pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*   pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T   pe_type;
    UINT8*          pui1_record; /* Address of the record memory block. */
    INT32           i4_rc;
    UINT16          ui2_count;
    BOOL            b_search_from_beginning;
    HANDLE_T        h_rwloc;
    CRIT_STATE_T    t_state;

    i4_rc = DBR_INV_ARG;
    
    /* Argument checking.*/
    if ( (ui1_count == 0 )                ||
         (aui4_fld_type_selected == NULL) ||
         (apv_fld_value_addr == NULL )    ||
         (pui4_db_version == NULL ) 
        )
    {
        return i4_rc;
    }

    /* IF ui1_keyfld_count is greater than 0, then key field array
       and key field values can not be NULL.
    */
    if ( ui1_keyfld_count > 0        &&
         ( (aui4_keyfld_types == NULL ) || (apv_keyfld_addrs == NULL ) )
        )
    {
        return i4_rc;
    }
    
    pui1_record=NULL;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )     
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /* Get the database object from the client handle object */
            pt_do=pt_co->p_data_obj;


            /*
              If this is the start of an iteration loop, then we
              don't need to check the version id, we just copy the
              current db versin id to the output argument, and set
              the flag to search the DB from the beginning.
            */
            if ( *pui4_db_version == CDB_NULL_VER_ID ||
                  ui2_record_index == 0
                )    
            {
                b_search_from_beginning = TRUE;
                *pui4_db_version = pt_do->ui4_db_version;
            }
            else
            {
                /*
                  The input version id is not NULL value, this
                  indicates that this call is in the iteration loop.
                  We need to check the version id to detect if
                  the database object has been
                  modified.  If version id don't match, then return
                  code indicating the database has
                  changed. 
                */
                if (  *pui4_db_version == pt_do->ui4_db_version )
                {
                    b_search_from_beginning = TRUE;
                }
                else
                {
                    /* Release read lock. */
                    if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                    {
                        /* Assertion Check:
                           Can not release Read lock, something wrong.
                        */
                        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                    }
                    /* return the current database version id
                       to the application. */
                    *pui4_db_version = pt_do->ui4_db_version;
                    return DBR_MODIFIED;
                }
            }

            /*
               Looking for the first record that matches the
               search param key and index value.
            */
            if ( b_search_from_beginning == TRUE )
            {
                /* Set the internal memory cursor to the first
                   record.
                */
                db_set_cursor_start(pt_do->t_memory,
                                    &(pt_co->t_mem_cursor));    
                
                /* perform linear search from the beginning of
                   the memory buffer. */
                ui2_count=0;
                while (TRUE)
                {
                    i4_rc=db_find_rcd_linear_search(pt_do,
                                                    &(pui1_record),
                                                    ui1_keyfld_count,
                                                    aui4_keyfld_types,
                                                    apv_keyfld_addrs,
                                                    at_keyfld_search_opts,
                                                    &(pt_co->t_mem_cursor));
                    if ( i4_rc==DBR_OK )
                    {
                        if ( ui2_count ==  ui2_record_index )
                        {
                            /* Found the record with the correct
                               index number
                            */
                            break;    
                        }
                        /* Increment the record index */
                        ui2_count++;
                    }
                    else
                    {
                        break;
                    }
                }

                /* If record found, then copy field values
                   to the user provided buffer. */
                if ( i4_rc==DBR_OK && pui1_record != NULL )
                {

                    /* copy the value. */
                    i4_rc=db_copy_field_values(pt_do,
                                               pui1_record,
                                               ui1_count,
                                               aui4_fld_type_selected,
                                               apv_fld_value_addr,
                                               NULL);   /* pedro */
                }
                else
                {
                    i4_rc=DBR_REC_NOT_FOUND;
                }
            }

            *pui4_db_version = pt_do->ui4_db_version;

            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
            /* Can not get READ Lock: 1) lock is deleted because
               this handle has been deleted.
            */
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);        
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_read_rec_v2
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

INT32 db_read_rec_v2
(
    HANDLE_T          h_client,
    UINT8		      ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[],
    UINT8             ui1_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    VOID*             apv_fld_value_addr[],
    UINT16            aui2_addr_len[],
    UINT16            ui2_record_index,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T* pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*   pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T   pe_type;
    UINT8*          pui1_record; /* Address of the record memory block. */
    INT32           i4_rc;
    UINT16          ui2_count;
    BOOL            b_search_from_beginning;
    HANDLE_T        h_rwloc;
    CRIT_STATE_T    t_state;

    i4_rc = DBR_INV_ARG;
    
    /* Argument checking.*/
    if ( (ui1_count == 0 )                ||
         (aui4_fld_type_selected == NULL) ||
         (apv_fld_value_addr == NULL )    ||
         (aui2_addr_len == NULL )          ||
         (pui4_db_version == NULL ) 
        )
    {
        return i4_rc;
    }

    /* IF ui1_keyfld_count is greater than 0, then key field array
       and key field values can not be NULL.
    */
    if ( ui1_keyfld_count > 0        &&
         ( (aui4_keyfld_types == NULL ) || (apv_keyfld_addrs == NULL ) )
        )
    {
        return i4_rc;
    }
    
    pui1_record=NULL;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )     
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /* Get the database object from the client handle object */
            pt_do=pt_co->p_data_obj;


            /*
              If this is the start of an iteration loop, then we
              don't need to check the version id, we just copy the
              current db versin id to the output argument, and set
              the flag to search the DB from the beginning.
            */
            if ( *pui4_db_version == CDB_NULL_VER_ID ||
                  ui2_record_index == 0
                )    
            {
                b_search_from_beginning = TRUE;
                *pui4_db_version = pt_do->ui4_db_version;
            }
            else
            {
                /*
                  The input version id is not NULL value, this
                  indicates that this call is in the iteration loop.
                  We need to check the version id to detect if
                  the database object has been
                  modified.  If version id don't match, then return
                  code indicating the database has
                  changed. 
                */
                if (  *pui4_db_version == pt_do->ui4_db_version )
                {
                    b_search_from_beginning = TRUE;
                }
                else
                {
                    /* Release read lock. */
                    if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                    {
                        /* Assertion Check:
                           Can not release Read lock, something wrong.
                        */
                        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                    }
                    /* return the current database version id
                       to the application. */
                    *pui4_db_version = pt_do->ui4_db_version;
                    return DBR_MODIFIED;
                }
            }

            /*
               Looking for the first record that matches the
               search param key and index value.
            */
            if ( b_search_from_beginning == TRUE )
            {
                /* Set the internal memory cursor to the first
                   record.
                */
                db_set_cursor_start(pt_do->t_memory,
                                    &(pt_co->t_mem_cursor));    
                
                /* perform linear search from the beginning of
                   the memory buffer. */
                ui2_count=0;
                while (TRUE)
                {
                    i4_rc=db_find_rcd_linear_search(pt_do,
                                                    &(pui1_record),
                                                    ui1_keyfld_count,
                                                    aui4_keyfld_types,
                                                    apv_keyfld_addrs,
                                                    at_keyfld_search_opts,
                                                    &(pt_co->t_mem_cursor));
                    if ( i4_rc==DBR_OK )
                    {
                        if ( ui2_count ==  ui2_record_index )
                        {
                            /* Found the record with the correct
                               index number
                            */
                            break;    
                        }
                        /* Increment the record index */
                        ui2_count++;
                    }
                    else
                    {
                        break;
                    }
                }

                /* If record found, then copy field values
                   to the user provided buffer. */
                if ( i4_rc==DBR_OK && pui1_record != NULL )
                {

                    /* copy the value. */
                    i4_rc=db_copy_field_values(pt_do,
                                               pui1_record,
                                               ui1_count,
                                               aui4_fld_type_selected,
                                               apv_fld_value_addr,
                                               aui2_addr_len);
                }
                else
                {
                    i4_rc=DBR_REC_NOT_FOUND;
                }
            }

            *pui4_db_version = pt_do->ui4_db_version;

            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
            /* Can not get READ Lock: 1) lock is deleted because
               this handle has been deleted.
            */
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);        
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_get_num_of_rec
 *
 * Description: This API returns the number record in the database that
 *              matches the search parameter.
 *
 * Inputs:  - h_client:  handle to CDB client
 *            ui1_keyfld_count:  number of fields that makes up the key
 *                                  parameters.
 *            aui4_keyfld_types: an array containing the field types of
 *                              the key parameters.
 *            apv_keyfld_addrs: an array of pointers to the values of
 *                              key parameters.
 *          
 * Outputs: - pui2_num_rec: pointer to a variable containing the number
 *                          of records found.
 *            pui4_db_version: pointer to a variable containing the
 *                             the current database version number.
 *
 * Returns: - DBR_OK     success, number of record is found.
 *            DBR_INV_ARG Input arguments are invalid, either because
 *                        the count variables are equal to 0 or the
 *                        pointer variables are null.
 *            DBR_INV_HANDLE  Bad handle or the database has been
 *                            deleted.
 ----------------------------------------------------------------------------*/

INT32 db_get_num_of_rec
(
    HANDLE_T          h_client,
    UINT8		      ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[],
    UINT16*           pui2_num_rec,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T* pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*   pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T   pe_type;
    UINT8*          pui1_record; /* Address of the record memory block. */
    INT32           i4_rc;
    UINT16          ui2_count;
    HANDLE_T        h_rwloc;
    CRIT_STATE_T    t_state;

    /* Argument checking.*/
    if ( (ui1_keyfld_count == 0 )        ||
         (aui4_keyfld_types == NULL )    ||
         (apv_keyfld_addrs == NULL )    ||
         (pui4_db_version == NULL ) 
        )
    {
        return DBR_INV_ARG;
    }
    
    pui1_record=NULL;
    i4_rc=DBR_INV_HANDLE;
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )     
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /* Get the database object from the client handle object */
            pt_do=pt_co->p_data_obj;
            
            /* Set the internal memory cursor to the first
               record.
            */
            db_set_cursor_start(pt_do->t_memory,
                                &(pt_co->t_mem_cursor));    
                
            /* perform linear search from the beginning of
               the memory buffer. */
            ui2_count=0;
            while (TRUE)
            {
                i4_rc=db_find_rcd_linear_search(pt_do,
                                                &(pui1_record),
                                                ui1_keyfld_count,
                                                aui4_keyfld_types,
                                                apv_keyfld_addrs,
                                                at_keyfld_search_opts,
                                                &(pt_co->t_mem_cursor));
                if ( i4_rc==DBR_OK )
                {
                    /* Increment the record index */
                    ui2_count++;
                }
                else
                {
                    /* No more record found. */
                    break;
                }
            }
            /*
              return the current database version id
              to the application.
            */
            *pui4_db_version=pt_do->ui4_db_version;
            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) == RWLR_OK )
            {
                i4_rc=DBR_OK;
                *pui2_num_rec=ui2_count;
            }
            else
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }
        }
        else
        {
            /* Can not unlock READ semaphore, the rwlock handle
               has been deleted.
            */
            i4_rc=DBR_INV_HANDLE;  
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
    }    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_read_sorted_rec
 *
 * Description: This API finds the next record from a sorted list
 *              and copies the selected fields from the record to
 *              client provides buffer. 
 *                         
 *
 * Inputs:  - h_client:  client handle.
 *            ui1_count: count of the selected output fields.
 *            aui4_fld_type_selected: field type of the selected
 *                                    output fields.
 *
 * Outputs: - apv_fld_value_addr: array of pointers to the memory
 *                                location for writing the field
 *                                values to.
 *            pui4_db_version:  pointer to the variable for storing the
 *                              version id of the database object.
 *
 * Returns: - DBR_OK     A record is read successfully.
 *            DBR_INV_HANDLE client handle is invalid. 
 *            DBR_INV_ARG Input arguments are invalid, either because
 *                        the count variables are equal to 0 or the
 *                        pointer variables are null.
 *            DBR_END_OF_REC reach end of records.
 *            DBR_MODIFIED  Database object has been modifed.
 *            DBR_NO_SORTED_LIST  missing internal sorted list,         
 ----------------------------------------------------------------------------*/
INT32 db_read_sorted_rec
(
    HANDLE_T	 	   h_client,
    UINT8		       ui1_count,
    DB_FIELD_T         aui4_fld_type_selected[],
    VOID*              apv_fld_value_addr[],
    UINT32*            pui4_db_version
)
{
    CDB_DATA_OBJ_T*    pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*      pt_co; /* pointer to the client object. */
    CDB_SORTED_LIST_T* pt_sl; /* pointer to the sorted list object.  */
    HANDLE_TYPE_T      pe_type;
    UINT8*             pui1_record;
    INT32              i4_rc;
    HANDLE_T           h_rwloc;
    CRIT_STATE_T       t_state;    

    /* Argument checking.*/
    if ( (ui1_count == 0     )             ||
         (aui4_fld_type_selected == NULL ) ||
         (apv_fld_value_addr == NULL )   ||
         (pui4_db_version == NULL )
        )
    {
        return DBR_INV_ARG;
    }
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /*
               Get the database object from the client handle
               object
            */
            pt_do=pt_co->p_data_obj;
            
            /* check if the data object contains record data. */
            if ( pt_do->ui2_record_count > 0 )
            {
                /* Get the sorted list object. */
                pt_sl=pt_co->pt_sl;
            
                /*
                  Get the sorted list object. Client need to
                  generate a sorted list and sorted list
                  needs to be up to date.
                */
                        
                if ( pt_sl != NULL )
                {
                            
                    /* Check if the database has changed since the
                       sorted list was last sorted. */
                    if ( pt_sl->ui4_version_id ==
                         pt_do->ui4_db_version )
                    {
                        /*
                          Sorted list is up to date, start the
                          iteration.
                        */

                        /* check if the cursor is pointing at a
                           valid record. */
                        if ( pt_co->ui4_cursor != DB_NULL_RECORD &&
                             pt_co->ui4_cursor <= pt_do->ui2_record_count )
                        { 
                            /*
                              Get the address of the record from the
                              entry pointed to by the cursor. */
                            pui1_record=
                                pt_sl->apui1_record_list[pt_co->ui4_cursor-1];
                                
                            /* copy the record. */
                            i4_rc = db_copy_field_values(pt_do,
                                                         pui1_record,
                                                         ui1_count,
                                                         aui4_fld_type_selected,
                                                         apv_fld_value_addr,
                                                         NULL);  /* pedro */
                        }
                        else
                        {
                            i4_rc=DBR_END_OF_REC;
                        }
                    }
                    else
                    {
                    
                        /*
                          Tell the client that database object
                          has been modified, therefore it needs
                          to update the sorted list
                          and reset the cursor and start the
                          iteration process again.
                        */
                        i4_rc=DBR_MODIFIED;
                    }
                }
                else
                {
                    /*
                      No sorted list object specified by the client.
                      Client needs to call db_sort() first.
                    */
                    i4_rc=DBR_NO_SORTED_LIST;
                }
            }
            else
            {
                /* empty records. */
                i4_rc=DBR_END_OF_REC;
            }
            
            /* return the database version id to the caller. */
            *pui4_db_version=pt_do->ui4_db_version;
            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
#if 0            
            /* Assertion Check:
               Can not obtain Read lock, something wrong. */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_read_sorted_rec_v2
 *
 * Description: This API finds the next record from a sorted list
 *              and copies the selected fields from the record to
 *              client provides buffer. 
 *                         
 *
 * Inputs:  - h_client:  client handle.
 *            ui1_count: count of the selected output fields.
 *            aui4_fld_type_selected: field type of the selected
 *                                    output fields.
 *            aui2_addr_len:  Specify the buffer space for the buffer ref'd by
 *                            the address entry in 'apv_fld_value_addr'
 *
 * Outputs: - apv_fld_value_addr: array of pointers to the memory
 *                                location for writing the field
 *                                values to.
 *            aui2_addr_len:  On return, it contains the data len copied into
 *                            the space ref'd by the the address entry in
 *                            'apv_fld_value_addr'.
 *            pui4_db_version:  pointer to the variable for storing the
 *                              version id of the database object.
 *
 * Returns: - DBR_OK     A record is read successfully.
 *            DBR_INV_HANDLE client handle is invalid. 
 *            DBR_INV_ARG Input arguments are invalid, either because
 *                        the count variables are equal to 0 or the
 *                        pointer variables are null.
 *            DBR_END_OF_REC reach end of records.
 *            DBR_MODIFIED  Database object has been modifed.
 *            DBR_NO_SORTED_LIST  missing internal sorted list,         
 ----------------------------------------------------------------------------*/
INT32 db_read_sorted_rec_v2
(
    HANDLE_T	 	   h_client,
    UINT8		       ui1_count,
    DB_FIELD_T         aui4_fld_type_selected[],
    VOID*              apv_fld_value_addr[],
    UINT16             aui2_addr_len[],
    UINT32*            pui4_db_version
)
{
    CDB_DATA_OBJ_T*    pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*      pt_co; /* pointer to the client object. */
    CDB_SORTED_LIST_T* pt_sl; /* pointer to the sorted list object.  */
    HANDLE_TYPE_T      pe_type;
    UINT8*             pui1_record;
    INT32              i4_rc;
    HANDLE_T           h_rwloc;
    CRIT_STATE_T       t_state;    

    /* Argument checking.*/
    if ( (ui1_count == 0     )             ||
         (aui4_fld_type_selected == NULL ) ||
         (apv_fld_value_addr == NULL )     ||
         (aui2_addr_len == NULL )          ||
         (pui4_db_version == NULL )
        )
    {
        return DBR_INV_ARG;
    }
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /*
               Get the database object from the client handle
               object
            */
            pt_do=pt_co->p_data_obj;
            
            /* check if the data object contains record data. */
            if ( pt_do->ui2_record_count > 0 )
            {
                /* Get the sorted list object. */
                pt_sl=pt_co->pt_sl;
            
                /*
                  Get the sorted list object. Client need to
                  generate a sorted list and sorted list
                  needs to be up to date.
                */
                        
                if ( pt_sl != NULL )
                {
                            
                    /* Check if the database has changed since the
                       sorted list was last sorted. */
                    if ( pt_sl->ui4_version_id ==
                         pt_do->ui4_db_version )
                    {
                        /*
                          Sorted list is up to date, start the
                          iteration.
                        */

                        /* check if the cursor is pointing at a
                           valid record. */
                        if ( pt_co->ui4_cursor != DB_NULL_RECORD &&
                             pt_co->ui4_cursor <= pt_do->ui2_record_count )
                        { 
                            /*
                              Get the address of the record from the
                              entry pointed to by the cursor. */
                            pui1_record=
                                pt_sl->apui1_record_list[pt_co->ui4_cursor-1];
                                
                            /* copy the record. */
                            i4_rc = db_copy_field_values(pt_do,
                                                         pui1_record,
                                                         ui1_count,
                                                         aui4_fld_type_selected,
                                                         apv_fld_value_addr,
                                                         aui2_addr_len);
                        }
                        else
                        {
                            i4_rc=DBR_END_OF_REC;
                        }
                    }
                    else
                    {
                    
                        /*
                          Tell the client that database object
                          has been modified, therefore it needs
                          to update the sorted list
                          and reset the cursor and start the
                          iteration process again.
                        */
                        i4_rc=DBR_MODIFIED;
                    }
                }
                else
                {
                    /*
                      No sorted list object specified by the client.
                      Client needs to call db_sort() first.
                    */
                    i4_rc=DBR_NO_SORTED_LIST;
                }
            }
            else
            {
                /* empty records. */
                i4_rc=DBR_END_OF_REC;
            }
            
            /* return the database version id to the caller. */
            *pui4_db_version=pt_do->ui4_db_version;
            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
#if 0            
            /* Assertion Check:
               Can not obtain Read lock, something wrong. */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_set_cursor 
 *
 * Description: This API is called to set or move the internal
 *              cursor.  
 *
 * Inputs:  - h_client:   handle to the client object.
 *            e_position: specifies how the cursor should be moved.
 *            e_rec_idx:  specifies the record position in the internal
 *                        sorted list.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK:     Operation is successful.
 *            DBR_INV_HANDLE:  Bad handle.
 *            DBR_INV_ARG: the specified e_position is not recognized.
 *            DBR_END_OF_REC: The cursor is at the end (first or last record
 *                            position. The cursor can not be moved in the specified
 *                            direction.
 ----------------------------------------------------------------------------*/


INT32  db_set_cursor
(
    HANDLE_T          h_client,
    DB_CTRL_FLAG_T    e_position,
    DB_RECORD_POS_T   t_rec_idx
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    UINT16            ui2_record_count;
    INT32             i4_rc;

    i4_rc=DBR_OK;
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {       
        ui2_record_count=pt_co->p_data_obj->ui2_record_count;
        
        /* end critical section. */
        x_crit_end(t_state);
        /*
          Note: valid cursor position starts from 1 (first record)
          to total number of records. 0 and number_of_record+1 are
          used as sentinel values.
        */
        switch ( e_position )
        {
            case DB_FIRST_RECORD:
            {
                if ( ui2_record_count > 0 )
                {
                    pt_co->ui4_cursor=1;
                }
                else
                {
                    pt_co->ui4_cursor = DB_NULL_RECORD;
                }
            }
            break;

            case DB_LAST_RECORD :
            {
                if ( ui2_record_count > 0 )
                {
                    pt_co->ui4_cursor=ui2_record_count;
                }
                else
                {
                    pt_co->ui4_cursor = DB_NULL_RECORD;
                }
            }
            break;

            case DB_NEXT_RECORD:
            {
                if ( pt_co->ui4_cursor < ui2_record_count )
                {
                   /* increment the cursor. */
                    pt_co->ui4_cursor++;
                }
                else
                {
                    /* we reach the end of the sorted list. */
                    i4_rc=DBR_END_OF_REC;
                }
            }
            break;

            case DB_PREV_RECORD:
            {
                if ( pt_co->ui4_cursor > 1 )
                {
                    /* decrement the cursor. */
                    pt_co->ui4_cursor--;
                }
                else
                {
                    /* we reach the end of the sorted list. */
                    i4_rc=DBR_END_OF_REC;
                }
            }
            break;

            case DB_GOTO_RECORD:
            {
                pt_co->ui4_cursor=t_rec_idx;
            }
            break;

            default:
            {
                i4_rc=DBR_INV_ARG;
            }
            break;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_cursor 
 *
 * Description: This API gets the position of the cursor 
 *              pointing at some entry in the sorted list. 
 *     
 * Inputs:  - h_client:   handle to the client object.
 *
 * Outputs: - pt_rec_ix:  On return, pointer to the current 
 *                        record index location. 
 *
 * Returns: - DBR_OK:     Operation is successful.
 *            DBR_INV_HANDLE:  Bad handle.
 *            DBR_INV_ARG: the specified e_position is not recognized.
 ----------------------------------------------------------------------------*/


INT32  db_get_cursor
(
    HANDLE_T          h_client,
    DB_RECORD_POS_T*  pt_rec_idx
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    INT32             i4_rc;

    i4_rc=DBR_OK;
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {   
        /* end critical section. */
        x_crit_end(t_state);
        *pt_rec_idx=pt_co->ui4_cursor;
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  
 *
 * Description: This API returns TRUE if the cursor is not already at
 *              either end points (first or last entry) of the sorted
 *              list. 
 *
 *
 * Inputs:  - h_client:  handle to the database client.
 *            e_position: The direction which the cursor will advance.
 * 
 * Outputs: - None.
 *
 * Returns: - TRUE     cursor is not near the ends of the database entries.
 *            FALSE    cursor is at the ends of the database entries.
 ----------------------------------------------------------------------------*/
BOOL db_has_more_entry
(
    HANDLE_T	 	  h_client,
    DB_CTRL_FLAG_T 	  e_position
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    UINT16            ui2_record_count;
    BOOL              b_result;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )                        
        )
    {
        ui2_record_count=pt_co->p_data_obj->ui2_record_count;

        /* end critical section. */
        x_crit_end(t_state);
        
        /*
          Check if the cursor is greater than the first and
          less than last entry of the sorted list.
        */
        switch ( e_position )
        {
            case DB_NEXT_RECORD:
            {
                if ( pt_co->ui4_cursor < ui2_record_count )
                {
                    b_result=TRUE;
                }
                else
                {
                    b_result=FALSE;
                }
            }
            break;

            case DB_PREV_RECORD:
            {
                if ( pt_co->ui4_cursor > 1 )
                {
                    b_result=TRUE;
                }
                else
                {
                    b_result=FALSE;
                }
            }
            break;

            default:
            {
                b_result=FALSE;
            }
            break;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        b_result=FALSE;
    }
    return b_result;
}

/*-----------------------------------------------------------------------------
 * Name:  db_get_info
 *
 * Description: This API returns information about the database object
 *              this client to the database object.
 *
 * Inputs:  - h_client:  handle to the client of the database object.
 *
 * Outputs: - pt_db_info: pointer to a info structure.
 *
 * Returns: - DBR_OK     A new lock is initialized successfully.
 *            DBR_INV_HANDLE Bad handle or the database is no longer valid.
 *            DBR_INV_ARG  pt_db_info is null.
 ----------------------------------------------------------------------------*/


INT32 db_get_info
(
    HANDLE_T          h_client,
    DB_INFO_T*        pt_db_info
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;
    
    if ( pt_db_info != NULL )
    {
        t_state=x_crit_start();
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                    ((VOID**) &pt_co)) == HR_OK) &&
             (pe_type    == CDBT_COREDB)                         &&
             (pt_co->p_data_obj != NULL )
            )
        {
            h_rwloc = pt_co->p_data_obj->h_rwloc;
            x_crit_end(t_state);
            
            /* Get the Read Lock */
            if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
            {               
                /*
                  Information about the client sorted list.
                */
                if ( pt_co->pt_sl != NULL )
                {
                    pt_db_info->ui4_client_version_id=
                        pt_co->pt_sl->ui4_version_id;
                }
                else
                {
                    pt_db_info->ui4_client_version_id =
                        CDB_NULL_VER_ID;
                }      

                /*
                  Information about the database object.
                */
                pt_do=pt_co->p_data_obj;

                pt_db_info->ui4_db_version_id=pt_do->ui4_db_version;
                pt_db_info->ui4_max_record=pt_do->ui2_max_count;
                pt_db_info->ui4_record_count=pt_do->ui2_record_count;
                pt_db_info->ui2_record_width=pt_do->ui2_record_width;
                pt_db_info->ui2_last_del_count=pt_do->ui2_last_del_count;
                pt_db_info->ui2_last_mod_count=pt_do->ui2_last_mod_count;
                pt_db_info->ui2_last_add_count=pt_do->ui2_last_add_count;
                if ( pt_do->pt_strbuf != NULL )
                {
                    pt_db_info->ui4_str_buf_allocated = pt_do->pt_strbuf->ui4_str_buf_sz;
                    pt_db_info->ui4_str_buf_used    = pt_do->pt_strbuf->ui4_mem_used;
                    pt_db_info->ui2_nb_str_obj_allocated = pt_do->pt_strbuf->ui2_ary_len;
                    pt_db_info->ui2_nb_str_obj_used    =pt_do->pt_strbuf->ui2_nb_str;
                    
                }
                else
                {
                    pt_db_info->ui4_str_buf_allocated = 0 ;
                    pt_db_info->ui4_str_buf_used    = 0;
                    pt_db_info->ui2_nb_str_obj_allocated = 0;
                    pt_db_info->ui2_nb_str_obj_used = 0;
                }
                
                /* Release read lock. */
                if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                {
                    /* Assertion Check:
                       Can not release Read lock, something wrong.
                    */
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                }

                i4_rc=DBR_OK;
            }
            else
            {
#if 0                
                /* Assertion Check:
                   Can not obtain Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
                i4_rc=DBR_INV_HANDLE;
            }            
        }
        else
        {
            /* don't forget to call x_crit_end. */
            x_crit_end(t_state);
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_get_persistent_size
 *
 * Description: This API returns size about the database object
 *    when it is stored on the file system.
 *
 * Inputs:  - h_client:  handle to the client of the database object.
 *
 * Outputs: - pt_db_info: pointer to a info structure.
 *
 * Returns: - DBR_OK     A new lock is initialized successfully.
 *            DBR_INV_HANDLE Bad handle or the database is no longer valid.
 *            DBR_INV_ARG  pt_db_info is null.
 ----------------------------------------------------------------------------*/


INT32 db_get_persistent_size
(
    HANDLE_T               h_client,
    db_endian_conv_fct     pf_conv,
    UINT32*                pui4_persistent_size,
    UINT32*                pui4_ver_id
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;
            
    if ( pui4_persistent_size == NULL  || pui4_ver_id == NULL )
    {
        return DBR_INV_ARG;
    }
    *pui4_persistent_size = 0;
    *pui4_ver_id          = 0;
    
    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type    == CDBT_COREDB)                         &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
            
        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /*
              Information about the database object.
            */
            pt_do=pt_co->p_data_obj;

            *pui4_ver_id = pt_do->ui4_db_version;
            
            *pui4_persistent_size=db_persistent_obj_size(pt_do,pf_conv,TRUE);

            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
            i4_rc=DBR_OK;
        }
        else
        {
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_lock 
 *
 * Description: This API locks database for write operation.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A write lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/


INT32 db_write_lock
(
    HANDLE_T h_client
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    DB_COND_T         e_cond;
    UINT32            ui4_reason;    
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        if ( rwl_write_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {   
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);

            pt_do=pt_co->p_data_obj;
            
            pt_co->h_write_thread = h_current_thread;
            /*
              Clear the counter for number of records
              added, deleted, and added, and flag
              for modifying common data.
            */
            pt_do->ui2_last_del_count=0;
            pt_do->ui2_last_mod_count=0;
            pt_do->ui2_last_add_count=0;
            pt_do->ui1_common_mod_flg=0;

            /*
              Notify all othe clients that this database object
              is being modified.
            */
            e_cond=DB_UPDATING;
            ui4_reason=DB_REASON_UNK;
                
            db_notify_all(
                h_client,
                pt_do,
                e_cond,
                ui4_reason,
                pt_do->ui4_db_version
                );
            
            
            i4_rc=DBR_OK;
        }
        else
        {
#if 0            
            /*
              Can not lock, Abort.
            */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_write_unlock
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


INT32 db_write_unlock
(
    HANDLE_T h_client
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    UINT32            ui4_reason = 0;
    DB_COND_T         e_cond;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    UINT32            ui4_ver_id;

    i4_rc=DBR_INV_HANDLE;
    if ( (handle_get_type_obj ( h_client, &pe_type,
                               ((VOID**) &pt_co)) == HR_OK) &&
         pe_type == CDBT_COREDB                             &&
         (pt_co->p_data_obj != NULL )
        )
    {   
        /*
          Assertion check.  It is an error if the write unlock is
          called before the write lock being called.

          ABORT is is kind restrictive check, but 
          it forces us to check for programming error.

          Need to investigate if we can do debug build and production
          build via compiler directive.
        */
        if ( pt_co->h_write_thread != NULL_HANDLE )
        {
            /*
              set thread id to NULL
            */
            pt_co->h_write_thread = NULL_HANDLE;            
        }
        else
        {
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
        }

        h_rwloc = pt_co->p_data_obj->h_rwloc;
        /*
          We need to get the information from the database object
          before releasing the rwloc.
        */
        pt_do=pt_co->p_data_obj;
        
        /*
          Determine the modification to the database (e.g., records
          added, deleted, or modified. 
        */
        if ( pt_do->ui2_last_del_count > 0 )
        {
            ui4_reason = ui4_reason | DB_RECORD_DEL;
        }
        if ( pt_do->ui2_last_mod_count > 0 )
        {
            ui4_reason = ui4_reason | DB_RECORD_MOD ;
        }
        if ( pt_do->ui2_last_add_count > 0 )
        {
            ui4_reason = ui4_reason | DB_RECORD_ADD ;
        }
        if ( pt_do->ui1_common_mod_flg > 0 )
        {
            ui4_reason = ui4_reason | DB_COMMON_MOD ;
        }
        ui4_ver_id = pt_do->ui4_db_version;
            
        /*
          Notify all othe clients that this database object
          has been changed.  Note, db_notify_all() is called
          in protected state, e.g., we release the read-write
          lock after we return from the db_notify_all().
        */
        e_cond=DB_UPDATED;
                
        db_notify_all(
            h_client,
            pt_do,
            e_cond,
            ui4_reason,
            ui4_ver_id
            );
        if ( rwl_release_lock(h_rwloc) == RWLR_OK )
        {
            i4_rc=DBR_OK;
        }
        else
        {         
            /*
              Can not unlock, Abort.
            */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_UNLOCK_FAILED);
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_write_rec
 *
 * Description: This API write a record into the database object.
 *
 * Inputs:  - h_client:  client handle
 *            apv_key_field_values:  values for the key parameter. Note:
 *                                   If multiple fields make up a key
 *                                   parameter, the order of values
 *                                   has to be in the same order that
 *                                   key types are specified when
 *                                   the database was created.
 *            apv_field_values:  values of the record fields.
 *            ui1_write_mode: Specify if the record to be written is
 *                            a new record or update an existing
 *                            record.
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


INT32 db_write_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[],
    DB_CTRL_FLAG_T    ui1_write_mode
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    UINT8*            pui1_record;
    INT32             i4_rc;
    BOOL              b_new_rec;
    
    i4_rc=DBR_FAIL;
    pui1_record=NULL;

    if ( apv_key_field_values != NULL && apv_field_values != NULL )
    {
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                   ((VOID**) &pt_co)) == HR_OK)    &&
             (pe_type == CDBT_COREDB )                        
            )
        {
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);
        
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;
                switch ( ui1_write_mode )
                {
                    case DB_APPEND_RECORD:
                    {
                        /*
                          Get the next free block
                        */
                        i4_rc=db_get_next_freeblock(pt_do->t_memory,
                                                    &(pui1_record));
                        if ( i4_rc != DBR_OK )
                        {   /* Out of memory. */
                            return DBR_OUT_OF_MEMORY;
                        }
                        else
                        {
                            /*
                              Increment the number of records added.
                            */
                            pt_do->ui2_last_add_count++;
                            b_new_rec=TRUE;
                        }
                    }
                    break;

                    case DB_UPDATE_RECORD:
                    case DB_MODIFY_RECORD:
                    {
                        /*
                          Search the database object for the specific record.
                          Start the search from the beginning of the
                          the memory buffer.
                        */
                        db_set_cursor_start(pt_do->t_memory,
                                            &(pt_co->t_mem_cursor));
                        
                        /* find the record. */
                        db_find_rcd_linear_search(
                            pt_do,
                            &(pui1_record),
                            pt_do->ui1_key_field_count,
                            pt_do->aui4_key_field_types,
                            apv_key_field_values,
                            (DB_SEARCH_T*)NULL,
                            &(pt_co->t_mem_cursor)
                        );
                        /*
                          If record does not exist, allocate a new
                          record block.
                        */
                        if ( pui1_record == NULL )
                        {
                            if ( ui1_write_mode == DB_MODIFY_RECORD )
                            {
                                /*
                                  Specified record does not exist.
                                */
                                return DBR_REC_NOT_FOUND;
                            }
                            else
                            {
                                /*
                                  Allocate a new free block
                                */
                                i4_rc=db_get_next_freeblock(pt_do->t_memory,
                                                        &(pui1_record));
                                if ( i4_rc != DBR_OK )
                                {   /* Out of memory. */
                                    return DBR_OUT_OF_MEMORY;
                                }
                                else
                                {
                                    /*
                                       Increment the number of records added.
                                    */
                                    pt_do->ui2_last_add_count++;
                                    b_new_rec=TRUE;
                                }
                            }
                        }
                        else
                        {
                            /*
                              Increment the number of records modified.
                            */
                            pt_do->ui2_last_mod_count++;
                            b_new_rec=FALSE;
                        }
                    }
                    break;

                    default:
                        return DBR_INV_ARG;
                }
                /*
                  copy the field values into the record block.
                */
                i4_rc=db_write_to_record_entry(pt_do,
                                               pui1_record,
                                               apv_field_values,
                                               NULL,
                                               b_new_rec);
            }
            else
            {
                /* No write lock or wrong thread is doing the
                   writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
            }
        }
        else
        {
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_write_rec_v2
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
INT32 db_write_rec_v2
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[],
    const UINT16      aui2_addr_len[],
    DB_CTRL_FLAG_T    ui1_write_mode
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    UINT8*            pui1_record;
    INT32             i4_rc;
    BOOL              b_new_rec;
    
    i4_rc=DBR_FAIL;
    pui1_record=NULL;

    if ( apv_key_field_values != NULL && apv_field_values != NULL )
    {
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                   ((VOID**) &pt_co)) == HR_OK)    &&
             (pe_type == CDBT_COREDB )                        
            )
        {
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);
        
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;
                switch ( ui1_write_mode )
                {
                    case DB_APPEND_RECORD:
                    {
                        /*
                          Get the next free block
                        */
                        i4_rc=db_get_next_freeblock(pt_do->t_memory,
                                                    &(pui1_record));
                        if ( i4_rc != DBR_OK )
                        {   /* Out of memory. */
                            return DBR_OUT_OF_MEMORY;
                        }
                        else
                        {
                            /*
                              Increment the number of records added.
                            */
                            pt_do->ui2_last_add_count++;
                            b_new_rec=TRUE;
                        }
                    }
                    break;

                    case DB_UPDATE_RECORD:
                    case DB_MODIFY_RECORD:
                    {
                        /*
                          Search the database object for the specific record.
                          Start the search from the beginning of the
                          the memory buffer.
                        */
                        db_set_cursor_start(pt_do->t_memory,
                                            &(pt_co->t_mem_cursor));
                        
                        /* find the record. */
                        db_find_rcd_linear_search(
                            pt_do,
                            &(pui1_record),
                            pt_do->ui1_key_field_count,
                            pt_do->aui4_key_field_types,
                            apv_key_field_values,
                            (DB_SEARCH_T*)NULL,
                            &(pt_co->t_mem_cursor)
                        );
                        /*
                          If record does not exist, allocate a new
                          record block.
                        */
                        if ( pui1_record == NULL )
                        {
                            if ( ui1_write_mode == DB_MODIFY_RECORD )
                            {
                                /*
                                  Specified record does not exist.
                                */
                                return DBR_REC_NOT_FOUND;
                            }
                            else
                            {
                                /*
                                  Allocate a new free block
                                */
                                i4_rc=db_get_next_freeblock(pt_do->t_memory,
                                                        &(pui1_record));
                                if ( i4_rc != DBR_OK )
                                {   /* Out of memory. */
                                    return DBR_OUT_OF_MEMORY;
                                }
                                else
                                {
                                    /*
                                       Increment the number of records added.
                                    */
                                    pt_do->ui2_last_add_count++;
                                    b_new_rec=TRUE;
                                }
                            }
                        }
                        else
                        {
                            /*
                              Increment the number of records modified.
                            */
                            pt_do->ui2_last_mod_count++;
                            b_new_rec=FALSE;
                        }
                    }
                    break;

                    default:
                        return DBR_INV_ARG;
                }
                /*
                  copy the field values into the record block.
                */
                i4_rc=db_write_to_record_entry(pt_do,
                                               pui1_record,
                                               apv_field_values,
                                               aui2_addr_len,
                                               b_new_rec);
            }
            else
            {
                /* No write lock or wrong thread is doing the
                   writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
            }
        }
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_update_rec
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
INT32 db_update_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_addrs[]
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    UINT8*            pui1_record;
    INT32             i4_rc;
    
    i4_rc=DBR_FAIL;
    pui1_record=NULL;

    if ( apv_key_field_values   != NULL     &&
         aui4_fld_type_selected != NULL     &&
         apv_fld_addrs          != NULL )
    {
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                   ((VOID**) &pt_co)) == HR_OK)    &&
             (pe_type == CDBT_COREDB )                        
            )
        {
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);
        
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;
                /*
                   Search the database object for the specific record.
                   Start the search from the beginning of the
                   the memory buffer.
                */
                db_set_cursor_start(pt_do->t_memory,
                                    &(pt_co->t_mem_cursor));
                /* find the record. */
                db_find_rcd_linear_search(
                    pt_do,
                    &(pui1_record),
                    pt_do->ui1_key_field_count,
                    pt_do->aui4_key_field_types,
                    apv_key_field_values,
                    (DB_SEARCH_T*)NULL,
                    &(pt_co->t_mem_cursor)
                    );

                if ( pui1_record != NULL )
                {
                    /*
                      Increment the number of records modified.
                    */
                    pt_do->ui2_last_mod_count++;
                    /*
                      update the selected field values with new values.
                    */
                    i4_rc=db_update_record_fld(pt_do,
                                               pui1_record,
                                               ui1_fld_count,
                                               aui4_fld_type_selected,
                                               apv_fld_addrs,
                                               NULL);
                    if ( i4_rc != DBR_OK && pt_do->ui2_last_mod_count > 0)
                    {
                        pt_do->ui2_last_mod_count--;
                    }
                }
                else
                {
                    /*
                      Specified record does not exist.
                    */
                    i4_rc=DBR_REC_NOT_FOUND;
                }
            }
            else
            {
                /* No write lock or wrong thread is doing the
                   writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
            }
        }
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_update_rec_v2
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
INT32 db_update_rec_v2
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_addrs[],
    const UINT16      aui2_addr_len[]
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    UINT8*            pui1_record;
    INT32             i4_rc;
    
    i4_rc=DBR_FAIL;
    pui1_record=NULL;

    if ( apv_key_field_values   != NULL     &&
         aui4_fld_type_selected != NULL     &&
         apv_fld_addrs          != NULL     &&
         aui2_addr_len          != NULL
        )
    {
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                   ((VOID**) &pt_co)) == HR_OK)    &&
             (pe_type == CDBT_COREDB )                        
            )
        {
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);
        
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;
                /*
                   Search the database object for the specific record.
                   Start the search from the beginning of the
                   the memory buffer.
                */
                db_set_cursor_start(pt_do->t_memory,
                                    &(pt_co->t_mem_cursor));
                /* find the record. */
                db_find_rcd_linear_search(
                    pt_do,
                    &(pui1_record),
                    pt_do->ui1_key_field_count,
                    pt_do->aui4_key_field_types,
                    apv_key_field_values,
                    (DB_SEARCH_T*)NULL,
                    &(pt_co->t_mem_cursor)
                    );

                if ( pui1_record != NULL )
                {
                    /*
                      Increment the number of records modified.
                    */
                    pt_do->ui2_last_mod_count++;
                    /*
                      update the selected field values with new values.
                    */
                    i4_rc=db_update_record_fld(pt_do,
                                               pui1_record,
                                               ui1_fld_count,
                                               aui4_fld_type_selected,
                                               apv_fld_addrs,
                                               aui2_addr_len);
                    if ( i4_rc != DBR_OK && pt_do->ui2_last_mod_count > 0)
                    {
                        pt_do->ui2_last_mod_count--;
                    }
                }
                else
                {
                    /*
                      Specified record does not exist.
                    */
                    i4_rc=DBR_REC_NOT_FOUND;
                }
            }
            else
            {
                /* No write lock or wrong thread is doing the
                   writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
            }
        }
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_sort_rec
 *
 * Description: This API creates create a sorted list of database records. 
 *
 * Inputs:  - h_client: handle to the client object.
 *            ui1_key_field_count: count of the key fields
 *            aui4_key_field_types: key field types
 *            at_sort_opts:         Sort options.
 *            b_blocking_mode: WAIT or NOWAIT.
 *
 * Outputs: - pui4_db_version:    version number of the database.
 *
 * Returns: - DBR_OK     
 *            DBR_WRITE_LOCKED:   Another client is performing write operation
 *                                on the database.
 *            DBR_INV_HANDLE:     h_client is not a valid handle.
 *            DBR_INV_ARG:        ui1_key_field_count is 0 or the input field
 *                                types are not scalar types, or the pointer
 *                                to the pui4_db_version is null.
 ----------------------------------------------------------------------------*/
INT32 db_sort_rec
(
    HANDLE_T          h_client,
    UINT8             ui1_key_field_count,
    const DB_FIELD_T  aui4_key_field_types[],
    const DB_SORT_T   at_sort_opts[],
    BOOL              b_blocking_mode,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T*     pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*       pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T       pe_type;
    INT32               i4_rc;
    RWL_OPTION_T        e_option;
    HANDLE_T            h_rwloc;
    CRIT_STATE_T        t_state;
    
    /* Arg checking. */
    if ( ui1_key_field_count  != 0         &&
         aui4_key_field_types != NULL      &&
         pui4_db_version != NULL )
    {
        /* We sorted the record using non-array fields. */
        for ( i4_rc = 0 ; i4_rc < ui1_key_field_count; i4_rc++ )
        {
            if ( DB_IS_FIELD_T_ARRAY(aui4_key_field_types[i4_rc]) == TRUE )
            {
                return DBR_INV_ARG;
            }
        }
    }
    else
    {
        return DBR_INV_ARG;
    }
    
    /* blocking or non-blocking */
    if ( b_blocking_mode == TRUE )
    {
        e_option = RWL_OPTION_WAIT;
    }
    else
    {
        e_option = RWL_OPTION_NOWAIT;
    }

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                               ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                           &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        if ( (i4_rc=rwl_read_lock(h_rwloc, e_option)) == RWLR_OK )
        {
            pt_do=pt_co->p_data_obj;

            if ( pt_do->ui2_record_count > 0 )
            {
                
                /* Create a sorted list structure using the specified
                   key parameters . */
                if ( pt_co->pt_sl == NULL )
                {
                    if ( db_create_sl_obj(pt_co,
                                          ui1_key_field_count,
                                          aui4_key_field_types,
                                          &(pt_co->pt_sl) )
                         != DBR_OK
                        )
                    {
                        /* Can not create a new sorted list object,
                           release the write lock and return error code.
                        */
                        if ( rwl_release_lock(h_rwloc) == RWLR_OK )
                        {
                            return DBR_OUT_OF_MEMORY;
                        }
                        else
                        {
                            /*  Can not unlock, Abort.*/
                            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                        }
                    }
                }
            
                /* Update and re-sort the list if the
                   database has changed or if the key parameters
                   has changed.
                */
                i4_rc = db_update_sl_obj(pt_do,
                                         pt_co->pt_sl,
                                         ui1_key_field_count,
                                         aui4_key_field_types,
                                         NULL,NULL,FALSE);
                if ( i4_rc == DBR_OK )
                {
                    /* Return to the client the database object version */
                    *pui4_db_version = pt_do->ui4_db_version ;
                }
            }
            else
            {
                i4_rc = DBR_END_OF_REC;
            }

            /* release the write lock and return error code. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /*  Can not unlock, Abort.*/
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }
        }
        else
        {
            /*
              Did not get the write lock,  depending on the
              request mode, it is either an error (for blocking mode),
              or because another thread has the write lock
              already.
            */
            if (  b_blocking_mode == TRUE )
            {
#if 0               
                /*
                  Blocking mode: Can not lock, Abort.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
                i4_rc=DBR_INV_HANDLE;
            }
            else
            {
                /*
                  Non-blocking mode.  return the status to the
                  caller.
                */
                if ( i4_rc == RWLR_WOULD_BLOCK )
                {
                    i4_rc=DBR_WRITE_LOCKED;
                }
                else
                {
#if 0                    
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
                    i4_rc = DBR_INV_HANDLE;
                }
            }
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);        
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }     
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_sort_rec_by_client_fct
 *
 * Description: This API creates create a sorted list of database records
 *              using client defined sort compare function.
 *
 * Inputs:  - h_client: handle to the client object.
 *            pf_sort_cmp:  Client provided sort compare function.
 *            pv_tag_data:     Tag data to the sort compare function.
 *            b_blocking_mode: WAIT or NOWAIT.
 *
 * Outputs: - pui4_db_version:    version number of the database.
 *
 * Returns: - DBR_OK     
 *            DBR_WRITE_LOCKED:   Another client is performing write operation
 *                                on the database.
 *            DBR_INV_HANDLE:     h_client is not a valid handle.
 *            DBR_INV_ARG:        pui4_db_version is null.
 ----------------------------------------------------------------------------*/
INT32 db_sort_rec_by_client_fct
(
    HANDLE_T          h_client,
    x_sort_cmp_fct    pf_sort_cmp,
    VOID*             pv_tag_data,
    BOOL              b_blocking_mode,
    BOOL              b_forced,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T*     pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*       pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T       pe_type;
    INT32               i4_rc;
    RWL_OPTION_T        e_option;
    HANDLE_T            h_rwloc;
    CRIT_STATE_T        t_state;
    
    /* Arg checking. */
    if ( pui4_db_version == NULL )
    {
        return DBR_INV_ARG;
    }
    
    /* blocking or non-blocking */
    if ( b_blocking_mode == TRUE )
    {
        e_option = RWL_OPTION_WAIT;
    }
    else
    {
        e_option = RWL_OPTION_NOWAIT;
    }

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                               ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                           &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        if ( (i4_rc=rwl_read_lock(h_rwloc, e_option)) == RWLR_OK )
        {
            pt_do=pt_co->p_data_obj;

            if ( pt_do->ui2_record_count > 0 )
            {
                
                /* Create a sorted list structure using the specified
                   key parameters . */
                if ( pt_co->pt_sl == NULL )
                {
                    if ( db_create_sl_obj(pt_co,
                                          0,
                                          NULL,
                                          &(pt_co->pt_sl) )
                         != DBR_OK
                        )
                    {
                        /* Can not create a new sorted list object,
                           release the write lock and return error code.
                        */
                        if ( rwl_release_lock(h_rwloc) == RWLR_OK )
                        {
                            return DBR_OUT_OF_MEMORY;
                        }
                        else
                        {
                            /*  Can not unlock, Abort.*/
                            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                        }
                    }
                }
            
                /* Update and re-sort the list if the
                   database has changed or if the key parameters
                   has changed.
                */
                i4_rc = db_update_sl_obj(pt_do,
                                         pt_co->pt_sl,
                                         0,
                                         NULL,
                                         pf_sort_cmp,
                                         pv_tag_data,
                                         b_forced);
                if ( i4_rc == DBR_OK )
                {
                    /* Return to the client the database object version */
                    *pui4_db_version = pt_do->ui4_db_version ;
                }
            }
            else
            {
                i4_rc = DBR_END_OF_REC;
            }

            /* release the write lock and return error code. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /*  Can not unlock, Abort.*/
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }
        }
        else
        {
            /*
              Did not get the write lock,  depending on the
              request mode, it is either an error (for blocking mode),
              or because another thread has the write lock
              already.
            */
            if (  b_blocking_mode == TRUE )
            {
                i4_rc=DBR_INV_HANDLE;
            }
            else
            {
                /*
                  Non-blocking mode.  return the status to the
                  caller.
                */
                if ( i4_rc == RWLR_WOULD_BLOCK )
                {
                    i4_rc=DBR_WRITE_LOCKED;
                }
                else
                {
                    i4_rc = DBR_INV_HANDLE;
                }
            }
        }
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);        
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }     
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_find_sorted_rec
 *
 * Description: This API searchs for a specific record the sorted list.
 *              It return a cursor which can be used to access that
 *              record. 
 *
 * Inputs:  -
 *   h_client:             handle to the client object.
 *   ui1_key_field_count:  count of the key fields.
 *   aui4_key_field_types: key field types.
 *   apv_keyfld_addrs:     Array of addresses contain values of search keys.
 *   at_sort_opts:         Optional sort options [contains function to
 *                         interpret key fields that are of UNION type.]
 *   ui2_record_index:     This index specifies a particular record.
 *                         If multiple record matches the search parameters,
 *                                  
 *
 * Outputs: -
 *   pt_rec_pos:         location of record in the sorted list.
 *   pui4_db_version:    version number of the database.
 *
 * Returns: - DBR_OK     
 *            DBR_WRITE_LOCKED:   Another client is performing write operation
 *                                on the database.
 *            DBR_REC_NOT_FOUND:  The specified record can not be found.
 *            DBR_INV_HANDLE:     h_client is not a valid handle.
 *            DBR_INV_ARG:        ui1_key_field_count is 0 or the input field
 *                                types are not scalar types, or the pointer
 *                                to the pui4_db_version is null.
 ----------------------------------------------------------------------------*/
INT32 db_find_sorted_rec
(
    HANDLE_T          h_client,
    UINT8             ui1_key_field_count,
    const DB_FIELD_T  aui4_key_field_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SORT_T   at_sort_opts[],
    UINT16            ui2_record_index,
    DB_RECORD_POS_T*  pt_rec_pos,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T*     pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*       pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T       pe_type;
    INT32               i4_rc;
    HANDLE_T            h_rwloc;
    CRIT_STATE_T        t_state;
    INT32               i4_result;
    
    /* Arg checking. */
    if ( ui1_key_field_count  != 0         &&
         aui4_key_field_types != NULL      &&
         apv_keyfld_addrs     != NULL      &&
         pt_rec_pos           != NULL      &&
         pui4_db_version      != NULL )
    {
        t_state=x_crit_start();
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                    ((VOID**) &pt_co)) == HR_OK) &&
             (pe_type == CDBT_COREDB)                           &&
             (pt_co->p_data_obj != NULL )
        )
        {
            h_rwloc = pt_co->p_data_obj->h_rwloc;
            x_crit_end(t_state);
            
            if ( (i4_rc=rwl_read_lock(h_rwloc, RWL_OPTION_WAIT)) == RWLR_OK )
            {
                /* Get the database object from the client handle object */
                pt_do=pt_co->p_data_obj;

                /*
                   If the sorted list has not been created, then
                   return an error. 
                */
                if ( pt_co->pt_sl != NULL )
                {
                    /* check if the specified key fields to search
                       the sort list is the same as the key fields
                       that was used to sorted the list.
                    */
                    if ( db_same_field_types(ui1_key_field_count,
                                             aui4_key_field_types,
                                             pt_co->pt_sl->aui4_sort_param)
                         == TRUE
                        )
                    {
                        /*
                          Update and re-sort the list if the
                          database has changed.
                        */
                        db_update_sl_obj(pt_do,
                                         pt_co->pt_sl,
                                         ui1_key_field_count,
                                         aui4_key_field_types,
                                         NULL, NULL, FALSE);
                        /* Return to the client the database object
                           version */
                        *pui4_db_version = pt_do->ui4_db_version ;

                        /* convert the arrays of search parameters
                           into a internal record representation.
                        */
                        i4_rc=db_copy_search_parm_to_rec(pt_co->pt_sl,
                                                         ui1_key_field_count,
                                                         aui4_key_field_types,
                                                         apv_keyfld_addrs);
                        if ( i4_rc == DBR_OK )
                        {
                            /* Find the record location.  */
                            i4_rc=x_binary_search(
                                pt_co->pt_sl->apui1_record_list,
                                pt_co->pt_sl->ui4_count,
                                sizeof(UINT8*),
                                db_compare_record,
                                &(pt_co->pt_sl->pv_search_parm_rec),
                                (INT32) ui2_record_index,
                                pt_co->pt_sl,
                                &i4_result
                                );
                            if ( i4_rc == SSR_OK )
                            {
                                /* The record index starts with 1 to N index.
                                   We reserve 0 for the NULL record index.
                                */
                                *pt_rec_pos= (DB_RECORD_POS_T)(i4_result+1);
                                i4_rc = DBR_OK;
                            }
                            else
                            {
                                i4_rc = DBR_REC_NOT_FOUND;
                            }
                        }
                        else
                        {
                            i4_rc=DBR_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc=DBR_FAIL;
                    }     
                }
                else
                {
                    i4_rc=DBR_FAIL;
                }
                /* release the read lock. */
                if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                {
                    /*  Can not unlock, Abort.*/
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                }
            }
            else
            {
                /*
                  Did not get the write lock,  the CDB handle is no
                  longer valid. 
                */
                i4_rc=DBR_INV_HANDLE;
            }
        }
        else
        {
            /* don't forget to call x_crit_end. */
            x_crit_end(t_state);        
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        return DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_find_sorted_rec_and_neighbors
 *
 * Description: This API searchs for a specific record the sorted list.
 *              It returns a cursor which can be used to access that
 *              record.
 *
 *              It also return the cursors to the next and prev element
 *              in the sorted list bracketing the found record.
 *
 *              If the record does not exist, the next and prev cursor
 *              points to the elements bracketing the location where the
 *              specified record should be located if it exists in the
 *              sorted list.
 *
 * Inputs:  -
 *   h_client:             handle to the client object.
 *   ui1_key_field_count:  count of the key fields.
 *   aui4_key_field_types: key field types.
 *   apv_keyfld_addrs:     Array of addresses contain values of search keys.
 *   at_sort_opts:         Optional sort options [contains function to
 *                         interpret key fields that are of UNION type.]
 *   ui2_record_index:     This index specifies a particular record.
 *                         If multiple record matches the search parameters,
 *                                  
 *
 * Outputs: -
 *   pt_rec_pos:         location of record in the sorted list.
 *   pt_rec_prev:        prev record in the sorted list.
 *   pt_rec_next:        next record in the sorted list.
 *
 *   pui4_db_version:    version number of the database.
 *
 * Returns: - DBR_OK     
 *            DBR_WRITE_LOCKED:   Another client is performing write operation
 *                                on the database.
 *            DBR_REC_NOT_FOUND:  The specified record can not be found.
 *            DBR_INV_HANDLE:     h_client is not a valid handle.
 *            DBR_INV_ARG:        ui1_key_field_count is 0 or the input field
 *                                types are not scalar types, or the pointer
 *                                to the pui4_db_version is null.
 ----------------------------------------------------------------------------*/
INT32 db_find_sorted_rec_and_neighbors
(
    HANDLE_T          h_client,
    UINT8             ui1_key_field_count,
    const DB_FIELD_T  aui4_key_field_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SORT_T   at_sort_opts[],
    UINT16            ui2_record_index,
    DB_RECORD_POS_T*  pt_rec_pos,
    DB_RECORD_POS_T*  pt_prev_rec,
    DB_RECORD_POS_T*  pt_next_rec,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T*     pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*       pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T       pe_type;
    INT32               i4_rc;
    HANDLE_T            h_rwloc;
    CRIT_STATE_T        t_state;
    INT32               i4_result = -1;
    INT32               i4_prev   = -1;
    INT32               i4_next   = -1;
    
    /* Arg checking. */
    if ( ui1_key_field_count  != 0         &&
         aui4_key_field_types != NULL      &&
         apv_keyfld_addrs     != NULL      &&
         pt_rec_pos           != NULL      &&
         pui4_db_version      != NULL )
    {
        t_state=x_crit_start();
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                    ((VOID**) &pt_co)) == HR_OK) &&
             (pe_type == CDBT_COREDB)                           &&
             (pt_co->p_data_obj != NULL )
        )
        {
            h_rwloc = pt_co->p_data_obj->h_rwloc;
            x_crit_end(t_state);
            
            if ( (i4_rc=rwl_read_lock(h_rwloc, RWL_OPTION_WAIT)) == RWLR_OK )
            {
                /* Get the database object from the client handle object */
                pt_do=pt_co->p_data_obj;

                /*
                   If the sorted list has not been created, then
                   return an error. 
                */
                if ( pt_co->pt_sl != NULL )
                {
                    /* check if the specified key fields to search
                       the sort list is the same as the key fields
                       that was used to sorted the list.
                    */
                    if ( db_same_field_types(ui1_key_field_count,
                                             aui4_key_field_types,
                                             pt_co->pt_sl->aui4_sort_param)
                         == TRUE
                        )
                    {
                        /*
                          Update and re-sort the list if the
                          database has changed.
                        */
                        db_update_sl_obj(pt_do,
                                         pt_co->pt_sl,
                                         ui1_key_field_count,
                                         aui4_key_field_types,
                                         NULL, NULL, FALSE);
                        /* Return to the client the database object
                           version */
                        *pui4_db_version = pt_do->ui4_db_version ;

                        /* convert the arrays of search parameters
                           into a internal record representation.
                        */
                        i4_rc=db_copy_search_parm_to_rec(pt_co->pt_sl,
                                                         ui1_key_field_count,
                                                         aui4_key_field_types,
                                                         apv_keyfld_addrs);
                        if ( i4_rc == DBR_OK )
                        {
                            /* Find the record location.  */
                            i4_rc=x_bin_search(
                                pt_co->pt_sl->apui1_record_list,
                                pt_co->pt_sl->ui4_count,
                                sizeof(UINT8*),
                                db_compare_record,
                                &(pt_co->pt_sl->pv_search_parm_rec),
                                (INT32) ui2_record_index,
                                pt_co->pt_sl,
                                &i4_result,
                                &i4_prev,
                                &i4_next
                                );
                            if ( i4_rc == SSR_OK )
                            {
                                /* The record index starts with 1 to N index.
                                   We reserve 0 for the NULL record index.
                                */
                                *pt_rec_pos=(DB_RECORD_POS_T)(i4_result+1);
                                i4_rc = DBR_OK;
                            }
                            else if ( i4_rc == SSR_NOT_FOUND )
                            {
                                *pt_rec_pos = DB_NULL_RECORD;
                                i4_rc = DBR_REC_NOT_FOUND;
                            }
                            else
                            {
                                i4_rc = DBR_FAIL;
                            }
                            /*
                              convert the neighbor indices into record
                              position cursor.
                            */
                            if ( pt_prev_rec != NULL    &&
                                 pt_next_rec != NULL
                                )
                            {
                                if ( i4_prev != -1 )
                                {
                                    *pt_prev_rec = (DB_RECORD_POS_T)(i4_prev + 1);
                                }
                                else
                                {
                                    *pt_prev_rec = DB_NULL_RECORD;
                                }
                                
                                if ( i4_next != -1 )
                                {
                                    *pt_next_rec = (DB_RECORD_POS_T)(i4_next + 1) ;
                                }
                                else
                                {
                                    *pt_next_rec = DB_NULL_RECORD;
                                }
                            }
                        }
                        else
                        {
                            i4_rc=DBR_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc=DBR_FAIL;
                    }     
                }
                else
                {
                    i4_rc=DBR_FAIL;
                }
                /* release the read lock. */
                if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                {
                    /*  Can not unlock, Abort.*/
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                }
            }
            else
            {
                /*
                  Did not get the write lock,  the CDB handle is no
                  longer valid. 
                */
                i4_rc=DBR_INV_HANDLE;
            }
        }
        else
        {
            /* don't forget to call x_crit_end. */
            x_crit_end(t_state);        
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        return DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_find_sorted_rec_and_neighbors_by_client_fct
 *
 * Description: This API searchs for a specific record the sorted list.
 *              It returns a cursor which can be used to access that
 *              record.
 *
 *              It also return the cursors to the next and prev element
 *              in the sorted list bracketing the found record.
 *
 *              If the record does not exist, the next and prev cursor
 *              points to the elements bracketing the location where the
 *              specified record should be located if it exists in the
 *              sorted list.
 *
 * Inputs:  -
 *   h_client:             handle to the client object.
 *   pv_param_to_find      contains value to look for in the database record.
 *   pf_search_cmp         client provided search compare function.
 *   pv_tag_data           client provided tag to pass to the compare function.
 *   ui2_record_index      Index used to find a particular record when multiple
 *                         record matches the search parameters,
 *                                  
 *
 * Outputs: -
 *   pt_rec_pos:         location of record in the sorted list.
 *   pt_rec_prev:        prev record in the sorted list.
 *   pt_rec_next:        next record in the sorted list.
 *
 *   pui4_db_version:    version number of the database.
 *
 * Returns: - DBR_OK     
 *            DBR_WRITE_LOCKED:   Another client is performing write operation
 *                                on the database.
 *            DBR_REC_NOT_FOUND:  The specified record can not be found.
 *            DBR_INV_HANDLE:     h_client is not a valid handle.
 *            DBR_INV_ARG:        ui1_key_field_count is 0 or the input field
 *                                types are not scalar types, or the pointer
 *                                to the pui4_db_version is null.
 ----------------------------------------------------------------------------*/
INT32 db_find_sorted_rec_and_neighbors_by_client_fct
(
    HANDLE_T          h_client,
    VOID*             pv_param_to_find,
    x_search_cmp_fct  pf_search_cmp,
    VOID*             pv_tag_data,
    UINT16            ui2_record_index,
    DB_RECORD_POS_T*  pt_rec_pos,
    DB_RECORD_POS_T*  pt_prev_rec,
    DB_RECORD_POS_T*  pt_next_rec,
    UINT32*           pui4_db_version
)
{
    CDB_DATA_OBJ_T*     pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*       pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T       pe_type;
    INT32               i4_rc;
    HANDLE_T            h_rwloc;
    CRIT_STATE_T        t_state;
    INT32               i4_result = -1;
    INT32               i4_prev   = -1;
    INT32               i4_next   = -1;
    
    /* Arg checking. */
    if ( pt_rec_pos           != NULL      &&
         pui4_db_version      != NULL )
    {
        t_state=x_crit_start();
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                    ((VOID**) &pt_co)) == HR_OK) &&
             (pe_type == CDBT_COREDB)                           &&
             (pt_co->p_data_obj != NULL )
        )
        {
            h_rwloc = pt_co->p_data_obj->h_rwloc;
            x_crit_end(t_state);
            
            if ( (i4_rc=rwl_read_lock(h_rwloc, RWL_OPTION_WAIT)) == RWLR_OK )
            {
                /* Get the database object from the client handle object */
                pt_do=pt_co->p_data_obj;

                /*
                   If the sorted list has not been created, then
                   return an error. 
                */
                if ( pt_co->pt_sl != NULL )
                {
                    /*
                       Return to the client the database object
                       version
                    */
                    *pui4_db_version = pt_do->ui4_db_version ;

                    i4_rc = SSR_NOT_FOUND;
                    
                    if ( pf_search_cmp != NULL )
                    {
                        /* Find the record location using client provide
                           search compare function.  */
                        i4_rc=x_bin_search(
                            pt_co->pt_sl->apui1_record_list,
                            pt_co->pt_sl->ui4_count,
                            sizeof(UINT8*),
                            pf_search_cmp,
                            pv_param_to_find,
                            (INT32) ui2_record_index,
                            pv_tag_data,
                            &i4_result,
                            &i4_prev,
                            &i4_next
                            );
                    }
                    else if ( pt_do->pf_search_cmp != NULL )
                    {
                        /* Find the record location using client provide
                           global search compare function.  */
                        i4_rc=x_bin_search(
                            pt_co->pt_sl->apui1_record_list,
                            pt_co->pt_sl->ui4_count,
                            sizeof(UINT8*),
                            pt_do->pf_search_cmp,
                            pv_param_to_find,
                            (INT32) ui2_record_index,
                            pt_do->pv_tag_data,
                            &i4_result,
                            &i4_prev,
                            &i4_next
                            );
                    }
                    
                    if ( i4_rc == SSR_OK )
                    {
                        /* The record index starts with 1 to N index.
                           We reserve 0 for the NULL record index.
                        */
                        *pt_rec_pos=(DB_RECORD_POS_T)(i4_result+1);
                        i4_rc = DBR_OK;
                    }
                    else if ( i4_rc == SSR_NOT_FOUND )
                    {
                        *pt_rec_pos = DB_NULL_RECORD;
                        i4_rc = DBR_REC_NOT_FOUND;
                    }
                    else
                    {
                        i4_rc = DBR_FAIL;
                    }
                    
                    /*
                      convert the neighbor indices into record
                      position cursor.
                    */
                    if ( pt_prev_rec != NULL    &&
                         pt_next_rec != NULL
                        )
                    {
                        if ( i4_prev != -1 )
                        {
                            *pt_prev_rec = (DB_RECORD_POS_T)(i4_prev + 1);
                        }
                        else
                        {
                            *pt_prev_rec = DB_NULL_RECORD;
                        }
                                
                        if ( i4_next != -1 )
                        {
                            *pt_next_rec = (DB_RECORD_POS_T)(i4_next + 1) ;
                        }
                        else
                        {
                            *pt_next_rec = DB_NULL_RECORD;
                        }
                    }
                }
                else
                {
                    i4_rc=DBR_FAIL;
                }
                /* release the read lock. */
                if ( rwl_release_lock(h_rwloc) != RWLR_OK )
                {
                    /*  Can not unlock, Abort.*/
                    ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
                }
            }
            else
            {
                /*
                  Did not get the write lock,  the CDB handle is no
                  longer valid. 
                */
                i4_rc=DBR_INV_HANDLE;
            }
        }
        else
        {
            /* don't forget to call x_crit_end. */
            x_crit_end(t_state);        
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        return DBR_INV_ARG;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_delete_rec
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

INT32 db_delete_rec
(
    HANDLE_T          h_client,
    UINT8		       ui1_key_field_count,
    const DB_FIELD_T  aui4_key_field_types[],
    const VOID*       apv_key_field_values[],
    UINT16		       ui2_record_index
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_T          h_current_thread;
    HANDLE_TYPE_T     pe_type;
    UINT8*            pui1_record;
    UINT16            ui2_count;
    UINT8             ui1_i;

    /* Arg checking. */
    if ( ui1_key_field_count == 0        ||
         aui4_key_field_types == NULL    ||
         apv_key_field_values ==NULL
        )
    {
        return DBR_INV_ARG;
    }
    
    i4_rc = DBR_OK;
    if ( (handle_get_type_obj ( h_client, &pe_type,
                               ((VOID**) &pt_co)) == HR_OK)     &&
         pe_type == CDBT_COREDB
        )
    {
        /* Gets the current thread id value. Use the thread id
           to check if the write lock has been acquired.    */
        x_thread_self(&h_current_thread);

        if ( pt_co->h_write_thread == h_current_thread )
        {
            /* Get the database object from the client handle object */
            pt_do=pt_co->p_data_obj;
            
            /*
              Search the database object for the specific record.
              Start the search from the beginning of the
              the memory buffer.
            */
            db_set_cursor_start(pt_do->t_memory,
                                &(pt_co->t_mem_cursor));

            
            /*  Set the counter to 0 and start the linear search to
                find the record to be deleted.
            */
            ui2_count=0;
            while (TRUE)
            {
                if ( db_find_rcd_linear_search(pt_do,
                                               &(pui1_record),
                                               ui1_key_field_count,
                                               aui4_key_field_types,
                                               apv_key_field_values,
                                               (DB_SEARCH_T*)NULL,
                                               &(pt_co->t_mem_cursor))==DBR_OK
                    )
                {
                    /*
                      The record is found, then check the counter value
                      with the specified rec index.
                    */
                    if ( ui2_count == ui2_record_index )
                    {
                        /* before freeing the memory block, we
                           need to check for the field containing
                           STRING field, so we can delete the
                           string object from the mini-buffer.
                        */
                        for ( ui1_i = 0 ;
                              ui1_i < pt_do->ui1_field_count ;
                              ui1_i++ )
                        {
                            if (  DB_GET_C_TYPE(
                                      pt_do->pt_fields[ui1_i].ui4_field_type)
                                  == DBF_STRING_T
                                )
                            {
                                STRING_ID_T   t_string_id;
                                UINT16     ui2_offset;
                                UINT16     ui2_width;

                                ui2_offset = pt_do->ui2_fld_offset[ui1_i];
                                ui2_width  = pt_do->ui2_fld_width[ui1_i];
                                
                                x_memcpy(&t_string_id,
                                         pui1_record+ui2_offset,
                                         ui2_width);

                                if ( t_string_id != STR_BUF_NULL_STR_ID )
                                {
                                    x_str_buf_del_string(pt_do->pt_strbuf,
                                                         t_string_id);
                                }
                            }
                            else if (  DB_GET_C_TYPE(
                                           pt_do->pt_fields[ui1_i].ui4_field_type)
                                       == DBF_MEMBUF_T
                                )
                            {
                                OBJECT_ID_T   t_object_id;
                                UINT16     ui2_offset;
                                UINT16     ui2_width;

                                ui2_offset = pt_do->ui2_fld_offset[ui1_i];
                                ui2_width  = pt_do->ui2_fld_width[ui1_i];
                                
                                x_memcpy(&t_object_id,
                                         pui1_record+ui2_offset,
                                         ui2_width);

                                if ( t_object_id != MEM_BUF_NULL_OBJ_ID )
                                {
                                    x_mem_buf_del_object(pt_do->pt_membuf,
                                                         t_object_id);
                                }
                            }
                        }

                        /* proceed to free up the memory 
                           block
                  
                           Note: The memory cursor contains
                           information points to this record block.
                        */
                        db_free_memory_block(pt_do->t_memory,
                                             &(pt_co->t_mem_cursor),
                                             pui1_record);
                        /*
                          The database is modified, so we need to
                          update the version number.
                        */
                        pt_do->ui4_db_version = CDB_INCRM_VER(pt_do->ui4_db_version);

                        /*
                          Decrement the total number of record.
                        */
                        pt_do->ui2_record_count--;
                        
                        /*
                          Increment deleted records count.
                        */
                        pt_do->ui2_last_del_count++;
        
                        break;                    
                    }
                    else
                    {
                        /* Increment the count and continue to
                           look for next match.
                        */
                        ui2_count++;
                    }
                }
                else
                {
                    /* Did not find the specified record for deletion. */
                    i4_rc=DBR_REC_NOT_FOUND;
                    break;
                }
            } /* End of while loop */
        }
        else
        {
            /* Assertion Check: client did not first acquire the
               WRITE lock when calling this function. */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_common 
 *
 * Description: This API write the common data (i.e., metadata that applies
 *              to all the records of a database object) into a internal
 *              memory buffer of database object.
 *
 * Inputs:  - h_client:  handle to the database object client.
 *            pv_common_data: pointer to the common data region.
 *            ui2_size:   size(bytes) of the common data region.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     Common data is written to database.
 *            DBR_INV_HANDLE  Bad database handle
 *            DBR_OUT_OF_MEMORY   Can not allocate memory to store common
 *                                data.
 *            DBR_INV_ARG:  pv_common_data is null.
 ----------------------------------------------------------------------------*/
INT32 db_write_common
(
    HANDLE_T	 	  h_client,
    VOID*		      pv_common_data,
    UINT16		      ui2_size 
    )
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;

    i4_rc=DBR_INV_ARG;
    if ( pv_common_data != NULL         &&
         ui2_size > 0
        )
    {
        if ( (handle_get_type_obj(h_client,
                                 &pe_type, ((VOID**) &pt_co)) == HR_OK)  &&
             (pe_type == CDBT_COREDB)                           
            )
        {
            /*
              Gets the current thread id value. Use the thread id
              to check if the write lock has been acquired.
            */
            x_thread_self(&h_current_thread);
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;

                if ( (pt_do->ui2_common_size == ui2_size) &&
                     (x_memcmp(pt_do->pui1_common_data, pv_common_data,
                               (SIZE_T)ui2_size) == 0) )
                {
                    /* Common data is the same. No need to write */
                    i4_rc=DBR_OK;
                }
                else if ( (pt_do->pui1_common_data =
                           (UINT8*) x_mem_realloc(pt_do->pui1_common_data,
                                                  (SIZE_T) ui2_size))
                          != NULL )
                {
                    x_memcpy(pt_do->pui1_common_data,
                             pv_common_data,
                             (SIZE_T)ui2_size);
                    pt_do->ui2_common_size=ui2_size;
                    pt_do->ui1_common_mod_flg=1;

                    /*
                      Update the version number.
                    */
                    pt_do->ui4_db_version = CDB_INCRM_VER(pt_do->ui4_db_version);
    
                    i4_rc=DBR_OK;
                }
                else
                {
                    i4_rc=DBR_OUT_OF_MEMORY;
                }
            }
            else
            {
                /*
                  No write lock or wrong thread is doing the
                  writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);            
            }
        }
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_read_common 
 *
 * Description: This API copies the common data for the database object
 *              to a buffer provided by the application.
 *
 * Inputs:  - h_client:   client object handle.
 *            pv_common_data:  address to a buffer provided by the application.
 *            ui2_size:   size of application provided buffer.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK          common data is read.
 *            DBR_INV_HANDLE  bad handle.
 *            DBR_INV_ARG     pv_common_data is NULL.
 *
 ----------------------------------------------------------------------------*/
INT32 db_read_common
(
    HANDLE_T	 	h_client,
    VOID*		    pv_common_data,
    UINT16		    ui2_size 
    )
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;    
    CRIT_STATE_T      t_state;
    
    if ( pv_common_data == NULL )
    {
        return DBR_INV_ARG;
    }

    t_state=x_crit_start();    
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        /*
          Gets the read-lock for this database object.
        */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
             pt_do=pt_co->p_data_obj;
             if ( ui2_size >= pt_do->ui2_common_size )
             {
                 x_memcpy(pv_common_data,
                          pt_do->pui1_common_data,
                          (SIZE_T)pt_do->ui2_common_size);
                 i4_rc=DBR_OK;
             }
             else
             {
                 i4_rc=DBR_OUT_OF_MEMORY;
             }
             /* Release read lock. */
             if ( rwl_release_lock(h_rwloc) != RWLR_OK )
             {
                 /* Assertion Check:
                    Can not release Read lock, something wrong.
                 */
                 ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
             }           
        }
        else
        {
#if 0            
            /*
              Assertion check, can not get the READ lock.
            */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_common_size
 *
 * Description: This API return the size of the common data.
 *
 * Inputs:  - h_client:   handle to the client object.
 *
 * Outputs: - pui2_size:  pointer to a variable holding the
 *                        size of the common data.
 *
 * Returns: - DBR_OK    common data size is obtained.
 *            DBR_INV_HANDLE: Bad handle (or database has been deleted.)
 *            DBR_INV_ARG:  pui2_size is Null.
 ----------------------------------------------------------------------------*/

extern INT32 db_common_size
(
    HANDLE_T         h_client,
    UINT16*          pui2_size 
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    CRIT_STATE_T      t_state;

    /* Arg check. */
    if ( pui2_size == NULL )
    {
        return DBR_INV_ARG;
    }
    
    t_state=x_crit_start();    
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /*
          Gets the read-lock for this database object.
        */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            *pui2_size = pt_co->p_data_obj->ui2_common_size;
            i4_rc=DBR_OK;

            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
#if 0            
            /*
              Assertion check, can not get the READ lock.
            */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_save_to_file
 *
 * Description: write the CDB object to file.
 *
 * Inputs:  - 
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK   
 *            DBRR_FAIL 
 ----------------------------------------------------------------------------*/


INT32 db_save_to_file
(
    HANDLE_T           h_cdb,
    HANDLE_T           h_file,
    db_endian_conv_fct pf_conv,
    db_cdata_conv_fct  pf_cdata_conv
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    CRIT_STATE_T      t_state;
    
    t_state=x_crit_start();    
    if ( (handle_get_type_obj ( h_cdb, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL)                         
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /*
          Gets the read-lock for this database object.
        */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            /*
              Write the content of CDB object to the specified
              file handle.
            */
            
            i4_rc=db_write_cdb_to_file(pt_co->p_data_obj,
                                       h_file,
                                       pf_conv,
                                       pf_cdata_conv,
                                       TRUE);

            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_cdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }

    if ( i4_rc == DBR_OK )
    {
        DBG_INFO(("{CDB} Save to file [handle: %d  file handle: %d]\n",h_cdb, h_file));
    }
    else
    {
        DBG_ERROR(("{CDB} Save to file failed [handle: %d file handle: %d err_code: %d]\n",
                   h_cdb,h_file,i4_rc));
    }
            
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_load_from_file
 *
 * Description: 
 *
 * Inputs:  - 
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     database is sucessfully create from file.
 *            DBR_FAIL   Cannot load from file due to internal error.
 *            DBR_INV_NAME:  the specified database name is already been used.
 ----------------------------------------------------------------------------*/

INT32 db_load_from_file
(
    CHAR*              s_db_name,
    UINT8              ui1_count,
    DB_FIELD_INFO_T    at_field_info[],
    UINT8			   ui1_key_field_count,
    DB_FIELD_T         aui4_key_field_types[],
    HANDLE_T           h_file_handle,
    db_endian_conv_fct pf_conv,
    db_cdata_conv_fct  pf_cdata_conv,
    VOID*              pv_tag,
    db_notify_fct      pf_nfy,
    HANDLE_T*          ph_client
)
{
    UINT16    ui2_i;
    INT32     i4_rc=DBR_OK;

    /*
      Check arguments
    */
    if ( s_db_name == NULL               ||
         x_strlen(s_db_name) == 0        ||
         ui1_count == 0                  ||
         at_field_info == NULL           ||
         ui1_key_field_count == 0        ||
         aui4_key_field_types == NULL    ||
         ph_client == NULL
        )
    {
        return DBR_INV_ARG;
    }
    
    /* Lock the database engine. */
    db_get_internal_lock();

    /*
      Check if the s_db_name has not been used in the database.
    */
    for ( ui2_i = 0; ui2_i < ui2_do_max_count ; ui2_i++ )
    {
        /*
           Look for specified db_name in the existing CDB engine.
        */
        if ( pt_name_db_obj[ui2_i].pt_database_obj != NULL &&
             x_strncmp(pt_name_db_obj[ui2_i].name, s_db_name,
                       CDB_MAX_DATABASE_NAME_LEN) == 0
            )
        {
            i4_rc=DBR_INV_NAME;
        }
    }

    /* Unlock the database */
    db_release_internal_lock();

    /*
      Continue to load the database from file.
    */
    if ( i4_rc == DBR_OK )
    {
        i4_rc=db_create_cdb_from_file(s_db_name,
                                      ui1_count,
                                      at_field_info,
                                      ui1_key_field_count,
                                      aui4_key_field_types,
                                      h_file_handle,
                                      pf_conv,
                                      pf_cdata_conv,
                                      pv_tag,
                                      pf_nfy,
                                      ph_client);
    }

    if ( i4_rc == DBR_OK )
    {
        DBG_API(("{CDB} load from file [handle: %d  file handle: %d name: %s]\n",
                 *ph_client, h_file_handle,s_db_name));
    }
    else
    {
        DBG_ERROR(("{CDB} load from file failed [file handle: %d name: %s err_code: %d]\n",
                   h_file_handle,s_db_name,i4_rc));
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_set_db_ver_num
 *
 * Description: This API set the database version number. The version number
 *              is incremented when the database is modified.
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

INT32 db_set_db_ver_num
(
    HANDLE_T          h_client,
    UINT32            ui4_ver_num
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    
    pt_co=NULL;
    i4_rc=DBR_OK;

    t_state=x_crit_start();
    if ( (handle_get_type_obj(h_client, &pe_type, (VOID**) &pt_co)) == HR_OK  &&
         (pe_type == CDBT_COREDB)                                             &&
         (pt_co->p_data_obj != NULL )
        )
    {
        /*
           Updated the internal state of the Database object:
           set version number.
        */
        pt_co->p_data_obj->ui4_db_version = ui4_ver_num ;
        x_crit_end(t_state);
    }
    else
    {
        /* Don't forget to end critical section. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = DBR_INV_HANDLE;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_reg_cmp_fcts
 *
 * Description: This API regiser CDB database compare functions to be
 *              used for sorting and searching operation of CDB records.
 *              If no function is registered, then the default CDB
 *              sort and search compare functions are used.
 *
 * Inputs:  -
 *   h_client:   handle to CDB database object.
 *
 *   pf_sort_cmp: Sort compare function called by CDB for sorting record
 *                operation.
 *
 *   pf_search_cmp: Search compare function called by CDB for searching
 *                  record operation.
 *
 *   pv_tag_data:  A reference to the client's private data to be pass
 *                 to the sort compare and search compare function.
 *                 This data must be dynamically allocated. Client must
 *                 not free this data, the CDB library will free this
 *                 data when the compare function is un-registered.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK          All records from the specified database
 *                            object are deleted.
 *            DBR_INV_HANDLE  Bad handle.
 *
 ----------------------------------------------------------------------------*/
INT32 db_reg_cmp_fcts
(
    HANDLE_T           h_client,
    x_sort_cmp_fct     pf_sort_cmp,
    x_search_cmp_fct   pf_search_cmp,
    VOID*              pv_tag_data
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    VOID*             pv_tmp_tag;
    HANDLE_T          h_current_thread = NULL_HANDLE;
    
    pt_co=NULL;
    i4_rc=DBR_OK;

    t_state=x_crit_start();
    if ( (handle_get_type_obj(h_client, &pe_type, (VOID**) &pt_co)) == HR_OK  &&
         (pe_type == CDBT_COREDB)                                             &&
         (pt_co->p_data_obj != NULL )
        )
    {
        x_crit_end(t_state);

        /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
        x_thread_self(&h_current_thread);
#ifndef ANDROID 
        if ( pt_co->h_write_thread == h_current_thread )
#endif
	{
        
            if (
                 (pf_sort_cmp != NULL && pf_search_cmp != NULL) ||
                 (pf_sort_cmp == NULL && pf_search_cmp == NULL )
               )
            {
                /*
                  Attach compare functions with CDB database object.
                */
                pt_co->p_data_obj->pf_sort_cmp   = pf_sort_cmp ;
                
                pt_co->p_data_obj->pf_search_cmp = pf_search_cmp;

                /*
                  Attach the tag data, 
                */
                pv_tmp_tag = pt_co->p_data_obj->pv_tag_data ;
                pt_co->p_data_obj->pv_tag_data = pv_tag_data;
                x_crit_end(t_state);

                /*
                  make sure the previous pv_tag
                  data is deleted.
                */
                if ( pv_tmp_tag != NULL )
                {
                    x_mem_free(pv_tmp_tag);
                }

                pt_co->p_data_obj->ui4_db_version = CDB_INCRM_VER(pt_co->p_data_obj->ui4_db_version);
                i4_rc = DBR_OK;
            }
            else
            {
                x_crit_end(t_state);
                i4_rc = DBR_INV_ARG;
            }
        }
#ifndef ANDROID
        else
        {
            /* No write lock or wrong thread is doing the writing.*/
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
        }
#endif
    }
    else
    {
        /* Don't forget to end critical section. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = DBR_INV_HANDLE;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_unreg_cmp_fcts
 *
 * Description: This API regiser CDB database compare functions to be
 *              used for sorting and searching operation of CDB records.
 *              If no function is registered, then the default CDB
 *              sort and search compare functions are used.
 *
 * Inputs:  -
 *   h_client:   handle to CDB database object.
 *
 *   pf_sort_cmp: Sort compare function called by CDB for sorting record
 *                operation.
 *
 *   pf_search_cmp: Search compare function called by CDB for searching
 *                  record operation.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK          All records from the specified database
 *                            object are deleted.
 *            DBR_INV_HANDLE  Bad handle.
 *
 ----------------------------------------------------------------------------*/
INT32 db_unreg_cmp_fcts
(
    HANDLE_T           h_client
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    CRIT_STATE_T      t_state;
    
    pt_co=NULL;
    i4_rc=DBR_OK;

    t_state=x_crit_start();
    if ( (handle_get_type_obj(h_client, &pe_type, (VOID**) &pt_co)) == HR_OK  &&
         (pe_type == CDBT_COREDB)                                             &&
         (pt_co->p_data_obj != NULL )
        )
    {
        x_crit_end(t_state);
        
        /*
          Un-attach compare functions with CDB database object.
        */
        pt_co->p_data_obj->pf_sort_cmp   = NULL ;
            
        pt_co->p_data_obj->pf_search_cmp = NULL;

        /*
          Free the tag data, 
        */
        if ( pt_co->p_data_obj->pv_tag_data != NULL )
        {
            x_mem_free(pt_co->p_data_obj->pv_tag_data) ;

            pt_co->p_data_obj->pv_tag_data = NULL ;
        }
            
        i4_rc = DBR_OK;
    }
    else
    {
        /* Don't forget to end critical section. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = DBR_INV_HANDLE;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_has_global_cmp_fcts
 *
 * Description: This API gets the reference to the database object
 *
 * Inputs:  - h_client:      handle to the database client
 *            
 *
 * Outputs: - apv_fld_value_addr: array of pointer address to store the
 *                                  field values.
 *
 * Returns: - DBR_OK         A record is read successfully.
 *            DBR_INV_HANDLE Bad handle or the database has been
 *                           deleted.
 *            DBR_INV_ARG    Input arguments are invalid, either because
 *                           the count variables are equal to 0 or the
 *                           pointer variables are null.
 *          
 ----------------------------------------------------------------------------*/

BOOL db_has_global_cmp_fcts
(
    HANDLE_T          h_client
)
{
    CDB_CLIENT_T*   pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T   pe_type;
    CRIT_STATE_T    t_state;
    BOOL            b_result;

    b_result = FALSE;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )     
    {
        if ( pt_co->p_data_obj->pf_sort_cmp    != NULL &&
             pt_co->p_data_obj->pf_search_cmp  != NULL
            )
        {
            b_result = TRUE;
        }
        x_crit_end(t_state);
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        
    }    
    return b_result;
}

/*-----------------------------------------------------------------------------
 * Name:  db_paser_rec_data
 *
 * Description: This API parse a specific record entry (raw bytes) into
 *              an array of address provided by the caller.
 *
 * Inputs:  - pv_cdb_database_obj  pointer to structure of a database object
 *
 *            pui1_record    Address of the record memory block. 
 *            
 *            ui1_count:     number of value to be extracted.
 *
 *            aui4_fld_type_selected: field types to be obtained.
 *            
 *            ui2_record_index:   index number of the records matching the
 *                                search paramters.
 *            
 *
 * Outputs: - apv_fld_value_addr: array of pointer address to store the
 *                                  field values.
 *
 * Returns: - DBR_OK         A record is read successfully.
 *            DBR_INV_HANDLE Bad handle or the database has been
 *                           deleted.
 *            DBR_INV_ARG    Input arguments are invalid, either because
 *                           the count variables are equal to 0 or the
 *                           pointer variables are null.
 *          
 ----------------------------------------------------------------------------*/


INT32 db_parse_rec_data
(
    VOID*             pv_cdb_database_obj,
    UINT8*            pui1_record,
    UINT8             ui1_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    VOID*             apv_fld_value_addr[]
)
{
    CDB_DATA_OBJ_T*   pt_do;
    INT32             i4_rc;

    i4_rc = DBR_INV_ARG;
    
    /* Argument checking.*/
    if ( (ui1_count == 0 )                 ||
         (aui4_fld_type_selected == NULL ) ||
         (apv_fld_value_addr == NULL )     ||
         (pui1_record == NULL )            ||
         (pv_cdb_database_obj == NULL )   
        )
    {
        return i4_rc;
    }
    else
    {
        pt_do = (CDB_DATA_OBJ_T*) pv_cdb_database_obj;
        /* copy the CDB field to user provided address */
        i4_rc=db_copy_field_values(pt_do,
                                   pui1_record,
                                   ui1_count,
                                   aui4_fld_type_selected,
                                   apv_fld_value_addr,
                                   NULL);   /* pedro */
    } 
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_get_database_obj
 *
 * Description: This API gets the reference to the database object
 *
 * Inputs:  - h_client:      handle to the database client
 *            
 *
 * Outputs: - apv_fld_value_addr: array of pointer address to store the
 *                                  field values.
 *
 * Returns: - DBR_OK         A record is read successfully.
 *            DBR_INV_HANDLE Bad handle or the database has been
 *                           deleted.
 *            DBR_INV_ARG    Input arguments are invalid, either because
 *                           the count variables are equal to 0 or the
 *                           pointer variables are null.
 *          
 ----------------------------------------------------------------------------*/

INT32 db_get_database_obj
(
    HANDLE_T          h_client,
    VOID**            ppt_cdb_database_obj
)
{
    CDB_DATA_OBJ_T* pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*   pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T   pe_type;
    INT32           i4_rc;
    CRIT_STATE_T    t_state;
    

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )     
    {
        pt_do=pt_co->p_data_obj;
        x_crit_end(t_state);
        
        *ppt_cdb_database_obj = pt_do ;
        i4_rc = DBR_OK;
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);        
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_is_save_to_file_needed
 *
 * Description: API to check if the CDB object referenced by handle 'h_cdb'
 *              and the CDB object stored on file system specified by the
 *              file handle location 'h_file' contain the different data
 *              If the data are different, then the writing to file handle
 *              will be needed in order save the content of CDB object.
 * 
 *              The algorithm only compare the CDB version number, and
 *              metadata between CDB object in the memory and CDB object
 *              filesytem.  
 *
 * Inputs:
 *   h_cdb:     Handle referenc the CDB object.
 *   h_file:    File handle to the CDB object store on the file system.
 *   pf_conv:   Endian conversion function to convert Big-endian data
 *              stored in file to endian order in CPU for each field of
 *              CDB record.
 *   pf_cdata_conv:  function to convert common data from big-endian format
 *                   stored in file to endian order in CPU.
 *
 * Outputs: -
 *   pb_result: TRUE  save-to-file operation needed.
 *              FALSE save-to-file operatin not needed.
 *
 * Returns: - DBR_OK   
 *            DBRR_FAIL 
 ----------------------------------------------------------------------------*/

INT32 db_is_save_to_file_needed
(
    HANDLE_T           h_cdb,
    HANDLE_T           h_file,
    db_endian_conv_fct pf_conv,
    db_cdata_conv_fct  pf_cdata_conv,
    BOOL*              pb_result
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    INT32             i4_rc;
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    CRIT_STATE_T      t_state;
    UINT32            ui4_obj_sz;
    
    t_state=x_crit_start();    
    if ( (handle_get_type_obj ( h_cdb, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL)                         
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        /*
          Gets the read-lock for this database object.
        */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            i4_rc=db_is_cdb_file_need_saving(pt_co->p_data_obj,
                                             h_file,
                                             pf_conv,
                                             TRUE,
                                             pb_result,
                                             &ui4_obj_sz);
            
            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_cdb))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_read_lock 
 *
 * Description: This API locks database for read operation.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/

INT32 db_read_lock
(
    HANDLE_T h_client
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {
            i4_rc=DBR_OK;
        }
        else
        {
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_read_unlock 
 *
 * Description: This API unlocks database for read operation.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is released successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/


INT32 db_read_unlock
(
    HANDLE_T h_client
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);
        
        if ( rwl_release_lock(h_rwloc) == RWLR_OK )
        {
            i4_rc=DBR_OK;
        }
        else
        {
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  db_sync_data_with_file
 *
 * Description: This API syncs the records and common data contains in
 *    the current CDB with records and common data from file system.
 *    The existing records in CDB handle are deleted, and then re-populate
 *    with records from read from the specified file.
 *
 *    If the CDB handle and CDB object on the file system don't have
 *    same meta-data, then operation failed.  Only compatible CDB objects,
 *    (e.g., same record definition) can be sync'd.  
 *
 * Inputs:  - 
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     database is sucessfully create from file.
 *            DBR_FAIL   Cannot load from file due to internal error.
 *            DBR_INV_NAME:  the specified database name is already been used.
 ----------------------------------------------------------------------------*/

INT32 db_sync_data_with_file
(
    HANDLE_T           h_cdb,
    UINT8              ui1_count,
    DB_FIELD_INFO_T    at_field_info[],
    UINT8			   ui1_key_field_count,
    DB_FIELD_T         aui4_key_field_types[],
    HANDLE_T           h_file_handle,
    db_endian_conv_fct pf_conv,
    db_cdata_conv_fct  pf_cdata_conv
)
{
    UINT16             ui2_i;
    UINT16             ui2_idx;
    INT32              i4_rc=DBR_OK;
    CHAR               s_db_name[CDB_MAX_DATABASE_NAME_LEN+1];
    HANDLE_T           h_tmp_cdb;
    
    HANDLE_TYPE_T      pe_type;
    CRIT_STATE_T       t_state;
    CDB_CLIENT_T*      pt_co;
    CDB_DATA_OBJ_T*    pt_do_to;
    CDB_DATA_OBJ_T*    pt_do_from;
    
    /*
      Check arguments
    */
    if ( ui1_count == 0                  ||
         at_field_info == NULL           ||
         ui1_key_field_count == 0        ||
         aui4_key_field_types == NULL    
        )
    {
        return DBR_INV_ARG;
    }
    
    /* Lock the database engine. */
    db_get_internal_lock();

    
    for ( ui2_idx = 0 ; ui2_idx < 16 ; ui2_idx++ )
    {
        /* create a temporary name. */
        x_sprintf(s_db_name,"_from_file_%d\0", ui2_idx);

        i4_rc = DBR_OK;
        /*
           Check if the s_db_name has not been used in the database.
        */
        for ( ui2_i = 0; ui2_i < ui2_do_max_count ; ui2_i++ )
        {
            /*
              Look for specified db_name in the existing CDB engine.
            */
            if ( pt_name_db_obj[ui2_i].pt_database_obj != NULL &&
                 x_strncmp(pt_name_db_obj[ui2_i].name, s_db_name,
                           CDB_MAX_DATABASE_NAME_LEN) == 0
                )
            {
                i4_rc=DBR_INV_NAME;
            }
        }
        
        if ( i4_rc == DBR_OK )
        {
            break;
        }
    }

    /* Unlock the database */
    db_release_internal_lock();

    /*
      Load the database from file.
    */
    if ( i4_rc == DBR_OK )
    {
        i4_rc=db_create_cdb_from_file(s_db_name,
                                      ui1_count,
                                      at_field_info,
                                      ui1_key_field_count,
                                      aui4_key_field_types,
                                      h_file_handle,
                                      pf_conv,
                                      pf_cdata_conv,
                                      NULL,
                                      NULL,
                                      &h_tmp_cdb);

        if ( i4_rc == DBR_OK )
        {
            /* Lock the CDB handle for read (source) and write (destination)
               operation.
            */
            db_read_lock(h_tmp_cdb);
            db_write_lock(h_cdb);

            pt_do_to = NULL ;
            pt_do_from = NULL;

            /* Get the database object from the CDB handle. */
            t_state=x_crit_start();
            if ( (handle_get_type_obj ( h_cdb, &pe_type,
                                        ((VOID**) &pt_co)) == HR_OK) &&
                 (pe_type == CDBT_COREDB)                             &&
                 (pt_co->p_data_obj != NULL )
                )
            {
                x_crit_end(t_state);
                pt_do_to=pt_co->p_data_obj;
            }
            else
            {
                x_crit_end(t_state);
#ifdef DEBUG
                if(x_handle_valid(h_cdb))
                {
                    ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
                }
#endif
            }

            /*
              Get the database object from the CDB handle create from file.
            */
            t_state=x_crit_start();
            if ( (handle_get_type_obj ( h_tmp_cdb, &pe_type,
                                        ((VOID**) &pt_co)) == HR_OK) &&
                 (pe_type == CDBT_COREDB)                             &&
                 (pt_co->p_data_obj != NULL )
                )
            {
                x_crit_end(t_state);
                pt_do_from=pt_co->p_data_obj;
            }
            else
            {
                x_crit_end(t_state);
#ifdef DEBUG
                if(x_handle_valid(h_tmp_cdb))
                {
                    ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
                }
#endif
            }

            if ( pt_do_to !=  NULL && pt_do_from != NULL )
            {
                i4_rc = db_copy_data(pt_do_from, pt_do_to);
            }
            
            db_write_unlock(h_cdb);
            
            db_read_unlock(h_tmp_cdb);

            /* delete temporary CDB handle. */
            db_delete(h_tmp_cdb, s_db_name);
        }
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_clear_data_sect_sz_info
 *
 * Description: This API clear data section size information
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/

INT32 db_clear_data_sect_sz_info
(
    HANDLE_T h_client
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    INT32             i4_rc = DBR_OK;
    CRIT_STATE_T      t_state;
    CDB_DATA_OBJ_T*   pt_do;
    

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        pt_do = pt_co->p_data_obj;
        x_crit_end(t_state);

        pt_do->t_dsect_info.b_valid_sect_sz = FALSE;
        pt_do->t_dsect_info.ui4_data_len = 0 ;
        pt_do->t_dsect_info.ui4_db_version = 0;
        pt_do->t_dsect_info.b_cmpres = FALSE;
        x_memset(pt_do->t_dsect_info.s_cmpres_eng, 0, CMPRES_ENG_NAME_LEN);
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_set_data_version_id
 *
 * Description: This API clear data section size information
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/

INT32 db_set_data_version_id
(
    HANDLE_T h_client,
    UINT32   ui4_version_id
    
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    INT32             i4_rc = DBR_OK;
    CRIT_STATE_T      t_state;
    CDB_DATA_OBJ_T*   pt_do;
    HANDLE_T          h_current_thread;
    

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        x_crit_end(t_state);
        
        /*
           Gets the current thread id value. Use the thread id
           to check if the write lock has been acquired.
        */
        x_thread_self(&h_current_thread);

        if ( pt_co->h_write_thread == h_current_thread )
        {
            pt_do = pt_co->p_data_obj;
            pt_do->ui4_db_version = ui4_version_id;
        }
        else
        {
            /* No write lock or wrong thread is doing the
               writing.
            */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
        }   
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_set_client_persistent_write_fct
 *
 * Description: This API register a write fucntion to covert/modify
 *              a specific CDB record field data before it is written
 *              out to persistent storage.
 *
 *              The registered function will be called when each field
 *              of a CDB record is written to persistent storage.
 *
 * Inputs:  - h_client:  handle to the client object.
 *
 * Outputs: - None.
 
 * Returns: - DBR_OK     A read lock is acquired successfully.
 *            DBR_INV_HANDLE Bad handle (or the database object is deleted.)
 ----------------------------------------------------------------------------*/

INT32 db_reg_persistent_write_fct
(
    HANDLE_T h_client,
    db_write_field_conv_fct    pf_client_persistent_write_fct
)
{
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    INT32             i4_rc = DBR_OK;
    CRIT_STATE_T      t_state;
    CDB_DATA_OBJ_T*   pt_do;

    t_state=x_crit_start();
    
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type == CDBT_COREDB)                            &&
         (pt_co->p_data_obj != NULL )
        )
    {
        x_crit_end(t_state);

        pt_do = pt_co->p_data_obj;
        pt_do->pf_client_store_fct = pf_client_persistent_write_fct;
        
    }
    else
    {
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_field_from_rec
 *
 * Description: API to read a field from a record
 *
 * Inputs:
 *    pv_rec_obj:      An opaque object that contains current
 *                     CDB record information.  
 *
 *    t_field_name:    CDB field name that specifies the field data to
 *                     be retrieved.
 *
 *    pui1_buf:        Output buffer to store field data
 *
 *    ui4_buf_len:     The length of the output buffer.
 *    
 *
 * Outputs: -
 *    pui1_buf:        buffer containing the data from the specified field
 *                     in the CDB record.
 *
 * Returns: -    
 *     DBR_OK    
 *     DBR_FAIL 
 *            
 ----------------------------------------------------------------------------*/
INT32 db_get_field_from_rec
(
    VOID*                  pv_rec_obj,
    DB_FIELD_T             t_field_name,
    UINT8*                 pui1_buf,
    UINT32                 ui4_buf_len
)
{
    CDB_OPAQUE_OBJ_T*      pt_obj;
    CDB_DATA_OBJ_T*        pt_do;
    UINT8*                 pui1_rec;
    INT32                  i4_rc=DBR_OK;
    UINT8		           ui1_count;
    DB_FIELD_T             aui4_field_type_selected[1];
    VOID*                  apv_field_value_addr[1];
    UINT16                 aui2_addr_len[1];

    pt_obj = (CDB_OPAQUE_OBJ_T*) pv_rec_obj;
    pt_do = pt_obj->pt_do;
    pui1_rec = pt_obj->pui1_rec;

    ui1_count = 1;
    aui4_field_type_selected[0] = t_field_name;
    apv_field_value_addr[0] = (VOID*) pui1_buf;
    aui2_addr_len[0] = (UINT16) ui4_buf_len;

    i4_rc = db_copy_field_values(pt_do,
                                 pui1_rec,
                                 ui1_count,
                                 (const DB_FIELD_T*) aui4_field_type_selected,
                                 apv_field_value_addr,
                                 aui2_addr_len);

    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_dump_to_mem
 *
 * Description: This API dumps the database object to a newly allocated memory
 *              location, which could be later used for db_load_from_mem.
 *
 * Inputs:  - h_client:     handle to the client of the database object.
 *          - b_compress:   controls whether or not to compress data
 *
 * Outputs: - ppv_mem_dump: pointer to the serialization output.
 *          - pui4_len:     pointer to the output buffer length
 *
 * Returns: - DBR_OK        Database has been dumped successfully.
 *            DBR_INV_HANDLE Bad handle or the database is no longer valid.
 *            DBR_INV_ARG   ppv_mem_dump or pui4_len is null.
 *            DBR_FAIL      Error has occured in dumping the database.
 *            DBR_OUT_OF_MEMORY Failed to allocated memory.
 ----------------------------------------------------------------------------*/

INT32 db_dump_to_mem
(
    HANDLE_T          h_client,
    BOOL              b_compress,
    VOID**            ppv_mem_dump,
    UINT32*           pui4_len
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type    == CDBT_COREDB)                         &&
         (pt_co->p_data_obj != NULL )
       )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);

        /* Get the Read Lock */
        if ( rwl_read_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {               
            pt_do=pt_co->p_data_obj;

            i4_rc=db_serialize(pt_do, b_compress, ppv_mem_dump, pui4_len);

            /* Release read lock. */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /* Assertion Check:
                   Can not release Read lock, something wrong.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
            }           
        }
        else
        {
#if 0                
            /* Assertion Check:
               Can not obtain Read lock, something wrong.
               */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }            
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_load_from_mem
 *
 * Description: This API restores the database content from a previous memory
 *              dump generated by db_dump_to_mem.
 *
 * Inputs:  - h_client:     handle to the client of the database object.
 *          - pv_mem_dump:  pointer to the previous memory dump.
 *          - ui4_len:      memory dump buffer length.
 *
 * Outputs: - None
 *
 * Returns: - DBR_OK        Database has been restored successfully.
 *            DBR_INV_HANDLE Bad handle or the database is no longer valid.
 *            DBR_INV_ARG   pv_mem_dump or pui4_len is null.
 *            DBR_FAIL      Error has occured in loading the database.
 ----------------------------------------------------------------------------*/

INT32 db_load_from_mem
(
    HANDLE_T          h_client,
    VOID*             pv_mem_dump,
    UINT32            ui4_len
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_rwloc;
    INT32             i4_rc;
    CRIT_STATE_T      t_state;

    t_state=x_crit_start();
    if ( (handle_get_type_obj ( h_client, &pe_type,
                                ((VOID**) &pt_co)) == HR_OK) &&
         (pe_type    == CDBT_COREDB)                         &&
         (pt_co->p_data_obj != NULL )
       )
    {
        h_rwloc = pt_co->p_data_obj->h_rwloc;
        x_crit_end(t_state);

        /* Get the Write Lock */
        if ( rwl_write_lock(h_rwloc, RWL_OPTION_WAIT) == RWLR_OK )
        {               
            pt_do=pt_co->p_data_obj;

            i4_rc=db_deserialize(pt_do, pv_mem_dump, ui4_len, TRUE);

            /*
              Release the write lock.
            */
            if ( rwl_release_lock(h_rwloc) != RWLR_OK )
            {
                /*
                  Can not unlock, Abort.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_UNLOCK_FAILED);
            }
        }
        else
        {
#if 0                
            /* Assertion Check:
               Can not obtain Read lock, something wrong.
               */
            ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
#endif
            i4_rc=DBR_INV_HANDLE;
        }            
    }
    else
    {
        /* don't forget to call x_crit_end. */
        x_crit_end(t_state);
#ifdef DEBUG
        if(x_handle_valid(h_client))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc=DBR_INV_HANDLE;
    }
    return i4_rc;
}
