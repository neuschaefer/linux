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
 * $RCSfile: satl_write_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements service list (SATL) database API for write
 *    operation: creating, deleting, removing SATL record, and for locking
 *    unlocking the SATL handle.
 *   
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "satl/satl.h"
#include "satl/_satl.h"
#include "satl/satl_rec.h"
#include "satl/satl_rec_id.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: satl_get_curr_rec_link_info
 *
 * Description: This API finds the location in internal linklist of
 *    the specified record.  The function returns the prev and next
 *    record ids (e.g, record id of the neighboring records). 
 *
 * Inputs:
 *    pt_satl:     SATL object.
 *    pt_satl_rec: pointer to the record structure.
 *
 * Outputs:
 *     pt_rec_id_link: pointer to the private data containing prev and next
 *                    record ids.
 *    
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
static INT32 satl_get_curr_rec_link_info
(
    const SATL_OBJ_T*         pt_satl,
    const SATL_REC_T*         pt_satl_rec,
    SATL_REC_ID_LINK_T*       pt_rec_id_link
)
{
    UINT32			    ui4_ver_id ;
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_T          ui4_recfld_type[3];
    VOID*               apv_recfld_addr[3];
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const VOID*         apv_keyfld_addr[1];
    UINT16              ui2_satl_rec_id;
    INT32               rc;
    /*
       determine the record's neighbors are.
    */
    pt_rec_id_link->ui2_prev = 0;
    pt_rec_id_link->ui2_next = 0;
    pt_rec_id_link->ui2_curr = 0;
    
    ui4_ver_id=SATL_NULL_VER_ID;
    ui1_fld_count=3;
    /*
      specify the private fields to retrieve.
    */
    ui4_recfld_type[0]=(DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID;
    ui4_recfld_type[1]=(DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID;
    ui4_recfld_type[2]=(DB_FIELD_T) SATL_CDB_TYPE_REC_ID;

    /*
      specify the addresses to write the data to.
    */
    apv_recfld_addr[0]=&(pt_rec_id_link->ui2_prev);
    apv_recfld_addr[1]=&(pt_rec_id_link->ui2_next);
    apv_recfld_addr[2]=&(pt_rec_id_link->ui2_curr);

    ui2_satl_rec_id = pt_satl_rec->ui2_satl_rec_id;

    ui1_key_count        = 1;
    aui4_keyfld_type[0]  = (DB_FIELD_T) SATL_CDB_TYPE_REC_ID ;
    apv_keyfld_addr[0]   = &ui2_satl_rec_id;

    /*
    x_dbg_stmt("{SATL} look for SATL rec id : %d\n", ui2_satl_rec_id);
    */
    rc= db_read_rec(pt_satl->h_cdb,
                    ui1_key_count,
                    aui4_keyfld_type,
                    apv_keyfld_addr,
                    NULL,
                    ui1_fld_count,
                    ui4_recfld_type,
                    apv_recfld_addr,
                    0,
                    &ui4_ver_id);

    if ( rc == DBR_OK )
    {
        /*
        x_dbg_stmt("{SATL} SATL rec id using linear find: prev: %d curr: %d  next: %d  \n",
                   pt_rec_id_link->ui2_prev,
                   pt_rec_id_link->ui2_curr,
                   pt_rec_id_link->ui2_next);
        */
        i4_rc = SATLR_DUPLICATE_REC;
    }
    else
    {
        i4_rc = SATLR_REC_NOT_FOUND;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_create_object
 *
 * Description: 
 *
 * Inputs: 
 *    
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL library is initialized successfully.
 *   SATLR_OUT_OF_MEM  Not enough memory to initialize the SATL library.
 *   SATLR_INV_ARG     'ui2_max_num_satl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 satl_create_object
(
    const CHAR*      ps_satl_name,
    UINT16           ui2_satl_id,
    SATL_MEM_POLICY_T e_mem_policy,
    UINT16           ui2_rec_sz,    
    VOID*            pv_tag,
    x_satl_nfy_fct   pf_nfy_fct,
    HANDLE_T*        ph_satl
)
{
    CHAR               ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    SATL_OBJ_T*        pt_satl=NULL;
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    HANDLE_T           h_cdb=NULL_HANDLE;
    INT32              i4_rc=SATLR_FAIL;
    DB_MEM_POLICY_T    e_cdb_mem_policy = DB_MEM_DYNAMIC;

    /* create CDB name. */
    satl_get_database_name(ps_satl_name,ui2_satl_id,ps_name);

    /* get the record definition. */
    satl_db_rec_and_key_def(&ui1_fld_count,
                           &pt_rec_fields,
                           &pui4_recfld_type,
                           &ui1_key_count,
                           &pui4_keyfld_type);
    
    /* Create a SATL object. */
    pt_satl=(SATL_OBJ_T*) x_mem_alloc(sizeof(SATL_OBJ_T));

    if ( e_mem_policy == SATL_MEM_STATIC )
    {
        e_cdb_mem_policy = DB_MEM_STATIC ;
    }
    else if ( e_mem_policy == SATL_MEM_DYNAMIC )
    {
        e_cdb_mem_policy = DB_MEM_DYNAMIC;
    }
    else
    {
        e_cdb_mem_policy = DB_MEM_STATIC;
    }
    
        
    /* Create a core database object. */
    if ( pt_satl != NULL )
    {
        i4_rc=db_create(ui2_rec_sz,
                        e_cdb_mem_policy,
                        ps_name,
                        ui1_fld_count,
                        pt_rec_fields,
                        ui1_key_count,
                        pui4_keyfld_type, /* Key types description.*/
                        (VOID*)(&pt_satl), /* Use Private Tag field
                                             to store addr of SATL
                                             handle control
                                             structure. */
                        satl_nfy_fct,     /* Internal SATL-CDB Notification
                                             function */
                        &h_cdb);
        
        /* translate the CDB error code to SATL error code */
        i4_rc=satl_cdb_to_satl_err_code(i4_rc);
             
        if ( i4_rc == SATLR_OK )
        {
            /*
              Allocate a handle for the SATL object.
            */
            if ( handle_alloc(SATLT_SATELLITE_LIST,
                              (VOID*)pt_satl,
                              pv_tag,
                              satl_handle_free_fct,
                              ph_satl)
                 == HR_OK )
            {
                /* initialize the SATL object. */
                pt_satl->h_satl=*ph_satl;
                pt_satl->h_cdb=h_cdb;
                pt_satl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                  caller's notification
                                                  function. */
                pt_satl->ui2_satl_id=ui2_satl_id;
                x_strncpy(pt_satl->ps_satl_name,ps_satl_name,MAX_SATL_NAME_LEN);

                /* Set the initial search parameter and cursor position
                   to unknown initial values
                   This will force sorted list to be created and search
                   to start from the first element of array.
                */
                x_memset(&(pt_satl->t_parm), SATL_UNSET_VAL,
                         sizeof(SATL_SEARCH_PARM_T));

                x_memset(&(pt_satl->t_cursor), SATL_UNSET_VAL,
                         sizeof(SATL_CURSOR_T));

                /*
                x_memset(&(pt_satl->t_cmp_fct_info), 0,
                         sizeof(SATL_CMP_FCT_INFO_T));
                */

                
                i4_rc=SATLR_OK;
            }
            else
            {
                i4_rc=SATLR_OUT_OF_HANDLE;
            }
        }
    }
    else
    {
        i4_rc = SATLR_OUT_OF_MEM ;
    }
    

    if ( i4_rc == SATLR_OK )
    {
        /*
          Initialize the internal variables for keeping
          track of the minimum and maximum SATL record id values.
        */
        satl_lock_object(pt_satl);
        satl_reset_rec_id(pt_satl);
        satl_unlock_object(pt_satl);
    }
    else
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,ps_name);
        }
        if ( pt_satl != NULL )
        {
            x_mem_free(pt_satl);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_delete_object
 *
 * Description: Delete SATL object and associated handle. 
 *
 * Inputs: 
 *    SATL_OBJ_T*     pt_satl;
 *  
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL library is initialized successfully.
 *   SATLR_OUT_OF_MEM  Not enough memory to initialize the SATL library.
 *   SATLR_INV_ARG     'ui2_max_num_satl_obj' is 0.
 *
 ----------------------------------------------------------------------------*/
INT32 satl_delete_object
(
    SATL_OBJ_T*   	pt_satl
)
{
    CHAR            ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    INT32           i4_rc;

  
    /* Get the name of the CDB database for this SATL handle. */
    satl_get_database_name(pt_satl->ps_satl_name,
                           pt_satl->ui2_satl_id,
                           ps_name);
    /* Delete the named CDB object. */
    i4_rc=db_delete(pt_satl->h_cdb, ps_name);

    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);

    /* Don't forget to delete the handle to this SATL */
    handle_free(pt_satl->h_satl, TRUE);

    return i4_rc;    
}

/*-----------------------------------------------------------------------------
 * Name: satl_clean_object
 *
 * Description: Remove all records from the specified SATL object.
 *
 * Inputs: 
 *   pt_satl: specified SATL object.
 *  
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          records are removed successfully.
 *   SATLR_INV_HANDLE  invalid cdb handle is passed into the db_clean() API.
 ----------------------------------------------------------------------------*/
INT32 satl_clean_object
(
    SATL_OBJ_T*   	pt_satl
)
{
    INT32 i4_rc;

    i4_rc = db_write_lock(pt_satl->h_cdb);

    if ( i4_rc == DBR_OK )
    {
        /* Clean the CDB object. */
        i4_rc=db_clean(pt_satl->h_cdb);
        if ( i4_rc == DBR_OK )
        {
            satl_reset_rec_id(pt_satl);

            /* Set the initial search parameter and cursor position
               to unknown initial values
               This will force sorted list to be created and search
               to start from the first element of array.
            */
            x_memset(&(pt_satl->t_parm), SATL_UNSET_VAL,
                     sizeof(SATL_SEARCH_PARM_T));
            
            x_memset(&(pt_satl->t_cursor), SATL_UNSET_VAL,
                     sizeof(SATL_CURSOR_T));
        }
        else
        {
            /* Assertion check. */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
        }
        db_write_unlock(pt_satl->h_cdb);
    }
    else
    {
        /* Assertion check. */
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
    }
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    return i4_rc;
}



INT32 satl_write_common_data_to_object
(
    SATL_OBJ_T*       pt_satl,
    VOID*             ps_common,
    INT32             ui4_common_size
)
{
    INT32      i4_rc;

    i4_rc=db_write_common(pt_satl->h_cdb,ps_common,(UINT16)ui4_common_size);
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: satl_write_rec_to_object
 *
 * Description: Write a record to the specified SATL object.
 *
 * Inputs: 
 *    pt_satl: specified SATL object.
 *    pt_satl_rec: Record to be written to the SATL object.
 *    ui1_write_mode: Write mode (APPEND, UPDATE, or APPEND_OR_UPDATE).
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          records are removed successfully.
 *   SATLR_INV_HANDLE  invalid cdb handle is passed into the db_clean() API.
 ----------------------------------------------------------------------------*/
INT32 satl_write_rec_to_object
(
    SATL_OBJ_T*        pt_satl,
    const SATL_REC_T*  pt_satl_rec,
    UINT8              ui1_write_mode
)
{
    const VOID*        apv_key_fld_addr[SATL_DB_NUM_OF_KEY_FIELDS];
    const VOID*        apv_rec_fld_addr[SATL_DB_NUM_OF_FIELDS];
    DB_CTRL_FLAG_T     e_option;
    INT32              i4_rc;
    SATL_REC_ID_LINK_T t_link_info;
    DB_UNION_TAG_T     at_utag[SATL_NUM_UNION_FIELDS];

    /*
      Because there is only one field in the LNB/DiSEqC union component,
      we just set the tag value to 1.
    */
    at_utag[0]=1;

    at_utag[1]=1;

    /* get the addresses of the satl record fields. */
    satl_get_db_fld_addr(&t_link_info, pt_satl_rec, at_utag, apv_rec_fld_addr);

    /* get the addresses of the key field. */
    satl_get_db_key_addr(pt_satl_rec, apv_key_fld_addr);


    /*
      Determine what kind of write operation should be
      performed. 
    */
    if ( ui1_write_mode == SATL_MODIFY_RECORD )
    {
        /*
          If this write operation is to an existing record, then
          get the neighbor record ids of the specified SATL record.
          Use the following function.
        */
        i4_rc=satl_get_curr_rec_link_info(pt_satl,pt_satl_rec,&t_link_info);
        if ( i4_rc == SATLR_DUPLICATE_REC )
        {
            /*
              x_dbg_stmt("[SATL] request to modify record %d ok\n", t_link_info.ui2_curr); */
            e_option=DB_MODIFY_RECORD;
        }
        else
        {
            /*
              x_dbg_stmt("[SATL] request to modify record %d but the record do not exist.\n",
               t_link_info.ui2_curr);
            */
            /* Error, specified record don't exist. */
            return SATLR_REC_NOT_FOUND ;
        }
    }
    else
    {
        /*
          If this write operation is for creating an new record, then
          determine the neighbor record ids of the new SATL record.  
        */
        i4_rc=satl_get_rec_link_info(pt_satl,pt_satl_rec,&t_link_info);

        if ( i4_rc == SATLR_OK || i4_rc == SATLR_DUPLICATE_REC )
        {
            /*
              convert the SATL write option flag to CoreDB write
              option flag.
            */
            if ( ui1_write_mode == SATL_APPEND_RECORD )
            {
                if ( i4_rc == SATLR_DUPLICATE_REC )
                {
                    /*
                      An existing record with the same key field value
                      is found. Note: We don't allow duplicate records
                      (e.g., record with the same key value) in SATL
                      database object.
                    */
                    return i4_rc;
                }
                else
                {
                    e_option=DB_APPEND_RECORD;
                }
            }
            else if ( ui1_write_mode == SATL_UPDATE_RECORD )
            {
                if ( i4_rc == SATLR_DUPLICATE_REC )
                {
                    /* Record exist, so this will be a modify operation. */
                    e_option=DB_MODIFY_RECORD;
                }
                else
                {
                    /* Record does not exist, add to the SATL database. */
                    e_option=DB_APPEND_RECORD;
                }
            }
            else
            {
                /* unknow write mode. */
                return SATLR_INV_ARG;
            }
        }
        else
        {
            /*
              can not get link information.
            */
            /*
              x_dbg_stmt("[SATL] can not get SATL record link information.\n");
            */
            return i4_rc;
        }
    }

    i4_rc=db_write_rec(pt_satl->h_cdb,
                       apv_key_fld_addr,
                       apv_rec_fld_addr,
                       e_option);

    /*
    x_dbg_stmt("[SATL] wrote link info: prev: %d curr: %d next: %d\n",
               t_link_info.ui2_prev,
               t_link_info.ui2_curr,
               t_link_info.ui2_next);
    */
    
    /*
      If this is a new record, then we need to update the link
      info.
    */
    if ( i4_rc == DBR_OK && e_option == DB_APPEND_RECORD )
    {
        /* Update the neighbor record's next and prev pointer. */
        i4_rc=satl_update_new_rec_link_info(pt_satl,&t_link_info);
    }
    else
    {
        /* translate the CDB error code to SATL error code */
        i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    }
    return i4_rc;
}

INT32 satl_del_rec_from_object
(
    SATL_OBJ_T*        pt_satl,
    UINT16             ui2_satl_rec_id
)
{
    INT32              i4_rc;
    UINT8              ui1_fld_count;
    UINT8              ui1_key_count;
    DB_FIELD_T         aui4_recfld_type[3];
    VOID*              apv_recfld_addr[3];     
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    const VOID*        apv_keyfld_addr[SATL_DB_NUM_OF_KEY_FIELDS];
    SATL_REC_ID_LINK_T t_rec_id_link;
    UINT32             ui4_db_version;    

    /*
      specify the private fields to retrieve.
    */
    ui1_fld_count=3;
    aui4_recfld_type[0]=(DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID;
    aui4_recfld_type[1]=(DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID;
    aui4_recfld_type[2]=(DB_FIELD_T) SATL_CDB_TYPE_REC_ID;

    /*
      specify the addresses to write the data to.
    */
    apv_recfld_addr[0]=&(t_rec_id_link.ui2_prev);
    apv_recfld_addr[1]=&(t_rec_id_link.ui2_next);
    apv_recfld_addr[2]=&(t_rec_id_link.ui2_curr);
    
    satl_get_key_def(&ui1_key_count,&pui4_keyfld_type);
    apv_keyfld_addr[0]=&ui2_satl_rec_id;

    /* Read the record linklist info. */
    i4_rc = db_read_rec(pt_satl->h_cdb,
                        ui1_key_count,
                        pui4_keyfld_type,
                        apv_keyfld_addr,
                        (DB_SEARCH_T*)NULL,
                        ui1_fld_count,
                        aui4_recfld_type,
                        apv_recfld_addr,
                        0,
                        &ui4_db_version);
    
    if ( i4_rc == DBR_OK )
    {
        i4_rc=db_delete_rec(pt_satl->h_cdb,
                            ui1_key_count,
                            pui4_keyfld_type,
                            apv_keyfld_addr,
                            0);
        if ( i4_rc == DBR_OK )
        {
            /* Update the linklist info for neighboring records of
               deleted record.
            */
            satl_update_deleted_rec_link_info(pt_satl, &t_rec_id_link);
        }
        else
        {
            /* translate the CDB error code to SATL error code */
            i4_rc=satl_cdb_to_satl_err_code(i4_rc);
            return i4_rc;
        }
    }
    else
    {
        /* translate the CDB error code to SATL error code */
        i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_lock_object
 *
 * Description: Lock the specified SATL object for write operation
 *
 * Inputs: 
 *    pt_satl: specified SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          object is locked successfully.
 *   SATLR_FAIL        internal error.
 ----------------------------------------------------------------------------*/
INT32 satl_lock_object
(
    SATL_OBJ_T*        pt_satl
)
{
    INT32       i4_rc;
    i4_rc=satl_cdb_to_satl_err_code(db_write_lock(pt_satl->h_cdb));
    if ( i4_rc == SATLR_OK )
    {
        return i4_rc;
    }
    else
    {
        /* Assertion check. */
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_unlock_object
 *
 * Description: Unlock the specified SATL object for write operation
 *
 * Inputs: 
 *    pt_satl: specified SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          object is unlocked successfully.
 *   SATLR_FAIL        internal error.
 ----------------------------------------------------------------------------*/
INT32 satl_unlock_object
(
    SATL_OBJ_T*        pt_satl
)
{
    INT32       i4_rc;
    i4_rc=satl_cdb_to_satl_err_code(db_write_unlock(pt_satl->h_cdb));
    if ( i4_rc == SATLR_OK )
    {
        return i4_rc;
    }
    else
    {
        /* Assertion check. */
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_WRITE_LOCK_FAILED);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_lock_object_for_read
 *
 * Description: Lock the specified SATL object for read operation
 *
 * Inputs: 
 *    pt_satl: specified SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          object is locked successfully.
 *   SATLR_FAIL        internal error.
 ----------------------------------------------------------------------------*/
INT32 satl_lock_object_for_read
(
    SATL_OBJ_T*        pt_satl
)
{
    INT32       i4_rc;
    i4_rc=satl_cdb_to_satl_err_code(db_read_lock(pt_satl->h_cdb));
    if ( i4_rc == SATLR_OK )
    {
        return i4_rc;
    }
    else
    {
        /* Assertion check. */
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_unlock_object_for_read
 *
 * Description: Unlock the specified SATL object for read operation
 *
 * Inputs: 
 *    pt_satl: specified SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          object is unlocked successfully.
 *   SATLR_FAIL        internal error.
 ----------------------------------------------------------------------------*/
INT32 satl_unlock_object_for_read
(
    SATL_OBJ_T*        pt_satl
)
{
    INT32       i4_rc;
    i4_rc=satl_cdb_to_satl_err_code(db_read_unlock(pt_satl->h_cdb));
    if ( i4_rc == SATLR_OK )
    {
        return i4_rc;
    }
    else
    {
        /* Assertion check. */
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_get_rec_link_info
 *
 * Description: This API finds the location in internal linklist where
 *    new record can be inserted.  The function returns the prev and next
 *    record ids (e.g, record id of the neighboring records). 
 *
 * Inputs:
 *    pt_satl:     SATL object.
 *    pt_satl_rec: pointer to the record structure.
 *
 * Outputs:
 *     pt_rec_id_link: pointer to the private data containing prev and next
 *                    record ids.
 *    
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 satl_get_rec_link_info
(
    const SATL_OBJ_T*         pt_satl,
    const SATL_REC_T*         pt_satl_rec,
    SATL_REC_ID_LINK_T*       pt_rec_id_link
)
{
    UINT32			    ui4_ver_id ;
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_T          ui4_recfld_type[3];
    VOID*               apv_recfld_addr[3];
    UINT16              ui2_largest_rec_id;
    UINT16              ui2_smallest_rec_id;
    

    satl_get_largest_rec_id(pt_satl, &ui2_largest_rec_id);
    satl_get_smallest_rec_id(pt_satl, &ui2_smallest_rec_id);

    /*
       If the list is current empty, the new record is the head and tail.
    */
    if ( ui2_largest_rec_id   ==  SATL_NULL_REC_ID &&
         ui2_smallest_rec_id  ==  SATL_NULL_REC_ID )
    {
        pt_rec_id_link->ui2_prev = SATL_NULL_REC_ID ;
        pt_rec_id_link->ui2_next = SATL_NULL_REC_ID ;
        pt_rec_id_link->ui2_curr = pt_satl_rec->ui2_satl_rec_id;
        return SATLR_OK;
    }

    /*
       If the record id is greater than the current largest record id
       value, then add the new record at the tail.
    */
    if ( pt_satl_rec->ui2_satl_rec_id > ui2_largest_rec_id )
    {
        pt_rec_id_link->ui2_prev = ui2_largest_rec_id;
        pt_rec_id_link->ui2_next = SATL_NULL_REC_ID;
        pt_rec_id_link->ui2_curr = pt_satl_rec->ui2_satl_rec_id;
        return SATLR_OK;
    }

    /*
       If the record id is smaller than the current smallest record id
       value, then add the new record at the head.
    */
    if ( pt_satl_rec->ui2_satl_rec_id < ui2_smallest_rec_id )
    {
        pt_rec_id_link->ui2_prev = SATL_NULL_REC_ID;
        pt_rec_id_link->ui2_next = ui2_smallest_rec_id;
        pt_rec_id_link->ui2_curr = pt_satl_rec->ui2_satl_rec_id;
        return SATLR_OK;
    }    

    /*
       Else we need to walk through the link list and determine
       where the  new record's neighbors are.
    */
    ui4_ver_id=SATL_NULL_VER_ID;
    i4_rc=satl_sort_rec_by_ts_id(pt_satl,&ui4_ver_id);
    if ( i4_rc == SATLR_OK )
    {
        /* Start the search from the beginning of the sorted list. */
        db_set_cursor(pt_satl->h_cdb,DB_FIRST_RECORD,DB_NULL_RECORD);
        
        /*
          specify the private fields to retrieve.
        */
        ui4_recfld_type[0]=(DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID;
        ui4_recfld_type[1]=(DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID;
        ui4_recfld_type[2]=(DB_FIELD_T) SATL_CDB_TYPE_REC_ID;

        /*
          specify the addresses to write the data to.
        */
        apv_recfld_addr[0]=&(pt_rec_id_link->ui2_prev);
        apv_recfld_addr[1]=&(pt_rec_id_link->ui2_next);
        apv_recfld_addr[2]=&(pt_rec_id_link->ui2_curr);
    }    
    else
    {
        return i4_rc;
    }

    ui1_fld_count=3;
    while (1)
    {
        i4_rc = db_read_sorted_rec(pt_satl->h_cdb,
                                   ui1_fld_count,
                                   ui4_recfld_type,
                                   apv_recfld_addr,
                                   &ui4_ver_id);        
        if ( i4_rc == DBR_OK )
        {
            /* check the value of record id with the specified record id. */
            if ( pt_satl_rec->ui2_satl_rec_id == pt_rec_id_link->ui2_curr )
            {
                i4_rc = SATLR_DUPLICATE_REC;
                break;
            }
            else
            {
                if ( pt_satl_rec->ui2_satl_rec_id > pt_rec_id_link->ui2_curr &&
                     pt_satl_rec->ui2_satl_rec_id < pt_rec_id_link->ui2_next )
                {
                    pt_rec_id_link->ui2_prev = pt_rec_id_link->ui2_curr;
                    pt_rec_id_link->ui2_curr = pt_satl_rec->ui2_satl_rec_id;
                    i4_rc = SATLR_OK;
                    break;
                }
            }
            /*
              Goto next record.
            */
            db_set_cursor(pt_satl->h_cdb,DB_NEXT_RECORD,DB_NULL_RECORD);
        }
        else
        {

            if ( i4_rc == DBR_MODIFIED )
            {
                /* Re-set the cursor position and re-start. */
                ui4_ver_id=SATL_NULL_VER_ID;
                i4_rc=satl_sort_rec_by_ts_id(pt_satl,&ui4_ver_id);
                if ( i4_rc == SATLR_OK )
                {
                    /* Start the search from the beginning of the
                      sorted list. */
                    db_set_cursor(pt_satl->h_cdb,
                                  DB_FIRST_RECORD,DB_NULL_RECORD);
                }
                else
                {
                    break;
                }
            }
            else
            {
                i4_rc = SATLR_FAIL;
                break;
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_update_new_rec_link_info
 *
 * Description: This function updates the prev and next record ids 
 *    of the neighboring records of the specified record. The specified
 *    record is a new entry.
 *
 * Inputs:
 *    pt_satl:        SATL object.
 *    pt_rec_id_link: Structure containing private data for the
 *                    specified new record and its prev and next record ids.
 *
 * Outputs:
 *    None
 *    
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 satl_update_new_rec_link_info
(
    SATL_OBJ_T*                pt_satl,
    const SATL_REC_ID_LINK_T*  pt_rec_id_link
)
{
    INT32               i4_rc;
    UINT16              ui2_largest_rec_id;
    UINT16              ui2_smallest_rec_id;
    

    satl_get_largest_rec_id(pt_satl, &ui2_largest_rec_id);
    satl_get_smallest_rec_id(pt_satl, &ui2_smallest_rec_id);

    /*
       If the list is current empty, the new record is the head and tail.
    */
    if ( ui2_largest_rec_id   ==  SATL_NULL_REC_ID &&
         ui2_smallest_rec_id  ==  SATL_NULL_REC_ID )
    {
        satl_set_smallest_rec_id(pt_satl, pt_rec_id_link->ui2_curr);
        satl_set_largest_rec_id(pt_satl, pt_rec_id_link->ui2_curr);
        return SATLR_OK;
    }

    /*
       If the record id is greater than the current largest record id
       value, then add the new record at the tail.
    */
    if ( pt_rec_id_link->ui2_curr > ui2_largest_rec_id )
    {
        /* Update the previous tail record*/
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       ui2_largest_rec_id,
                                       pt_rec_id_link->ui2_curr,
                                       (DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID);

        if ( i4_rc == SATLR_OK )
        {
            /* Update the largest record id. */
            satl_set_largest_rec_id(pt_satl, pt_rec_id_link->ui2_curr);
        }
        else
        {
            /* Assertion check, if update link info fail, then
               somthing is wrong, just abort.
            */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
        }
        return i4_rc;
    }

    /*
       If the record id is smaller than the current smallest record id
       value, then add the new record at the head.
    */
    if ( pt_rec_id_link->ui2_curr  < ui2_smallest_rec_id )
    {
        /* Update the previous head record. */
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       ui2_smallest_rec_id,
                                       pt_rec_id_link->ui2_curr,
                                       (DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID);

        if ( i4_rc == SATLR_OK )
        {
            /* Update the smallest record id. */
            satl_set_smallest_rec_id(pt_satl, pt_rec_id_link->ui2_curr);
        }
        else
        {
            /* Assertion check, if update link info fail, then
               somthing is wrong, just abort.
            */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
        }
        return i4_rc;
    }    

    /*
       Else we need to update new record's neighbors are.
    
       Update the previous record's next field and next record's prev field.
    */
    i4_rc=satl_write_rec_link_data(pt_satl,
                                   pt_rec_id_link->ui2_prev,
                                   pt_rec_id_link->ui2_curr,
                                   (DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID);
    if ( i4_rc == SATLR_OK )
    {
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       pt_rec_id_link->ui2_next,
                                       pt_rec_id_link->ui2_curr,
                                       (DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID);
    }
    else
    {
        /* Assertion check, if update link info fail, then
           somthing is wrong, just abort.
        */
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
    }
    return i4_rc;
}
    
/*-----------------------------------------------------------------------------
 * Name: satl_update_deleted_rec_link_info
 *
 * Description: This function updates the prev and next record ids 
 *    of the neighboring records of the specified record. The specified
 *    record has just been deleted.
 *
 * Inputs:
 *    pt_satl:        SATL object.
 *    pt_rec_id_link: Structure containing private data for the
 *                    specified new record and its prev and next record ids.
 *
 * Outputs:
 *    None
 *    
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 satl_update_deleted_rec_link_info
(
    SATL_OBJ_T*                pt_satl,
    const SATL_REC_ID_LINK_T*  pt_rec_id_link
)
{
    INT32               i4_rc;
    UINT16              ui2_largest_rec_id;
    UINT16              ui2_smallest_rec_id;
    

    satl_get_largest_rec_id(pt_satl, &ui2_largest_rec_id);
    satl_get_smallest_rec_id(pt_satl, &ui2_smallest_rec_id);


    /*
      Special case, if the deleted record was the last record
      in the database, then its record id is equal to the
      largest and smallest record id.  We reset the min and max
      record id, but don't need to update the link info.
    */
    if ( pt_rec_id_link->ui2_curr == ui2_largest_rec_id    &&
         pt_rec_id_link->ui2_curr == ui2_smallest_rec_id )
    {
        /*
          Reset the Min and Max record id value.
        */
        satl_reset_rec_id(pt_satl);
        return SATLR_OK;
    }
    
    /*
       If the deleted record id is equal to the current largest record id
       value, then update the tail.
    */
    if ( pt_rec_id_link->ui2_curr == ui2_largest_rec_id )
    {
        /* Update the tail's prev record */
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       pt_rec_id_link->ui2_prev,
                                       SATL_NULL_REC_ID,
                                       (DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID);
        
        if ( i4_rc == SATLR_OK )
        {
            /* Update the largest record id. */
            satl_set_largest_rec_id(pt_satl, pt_rec_id_link->ui2_prev);
        }
        else
        {
            /* Assertion check, if update link info fail, then
               somthing is wrong, just abort.
            */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
        }
        return i4_rc;
    }

    /*
       If the deleted record id is equal to the current smallest record id
       value, then update the head.
    */
    if ( pt_rec_id_link->ui2_curr == ui2_smallest_rec_id )
    {
        /* Update the head's next record. */
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       pt_rec_id_link->ui2_next,
                                       SATL_NULL_REC_ID,
                                       (DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID);

        if ( i4_rc == SATLR_OK )
        {
            /* Update the smallest record id. */
            satl_set_smallest_rec_id(pt_satl, pt_rec_id_link->ui2_next);
        }
        else
        {
            /* Assertion check, if update link info fail, then
               somthing is wrong, just abort.
            */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
        }
        return i4_rc;
    }    

    /*
       Else we need to update new record's neighbors are.
       Update the this record's prev neighbor record's next pointer,
       and then this record's next neighbor record's prev pointer.
    */
    i4_rc=satl_write_rec_link_data(pt_satl,
                                   pt_rec_id_link->ui2_prev,
                                   pt_rec_id_link->ui2_next,
                                   (DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID);

    if ( i4_rc == SATLR_OK )
    {
        i4_rc=satl_write_rec_link_data(pt_satl,
                                       pt_rec_id_link->ui2_next,
                                       pt_rec_id_link->ui2_prev,
                                       (DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID);
    }
    else
    {
        /* Assertion check, if update link info fail, then
           somthing is wrong, just abort.
        */
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL);
    }
    return i4_rc;
}
    
    
/*-----------------------------------------------------------------------------
 * Name: satl_write_rec_link_data
 *
 * Description: This function write the prev,or next record ids 
 *    of the specified record. The specified record must existed.
 *
 * Inputs:
 *    pt_satl:      SATL object.
 *    ui2_rec_id:   Structure containing private data for the
 *                  specified new record and its prev and next record ids.
 *    ui4_fld_type: Specify the private data (prev, curr, or next) to be
 *                  modified.
 *
 * Outputs:
 *    None
 *    
 * Returns: 
 *   TRUE      SATL ID/Name is not currently in the database..
 *   FALSE     Invalid SATL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 satl_write_rec_link_data
(
    const SATL_OBJ_T*          pt_satl,
    UINT16                     ui2_rec_id,
    UINT16                     ui2_neighbor_id,
    DB_FIELD_T                 ui4_fld_type
)
{
    INT32               i4_rc;
    const VOID*         apv_keyfld_addrs[SATL_DB_NUM_OF_KEY_FIELDS];
    UINT8               ui1_fld_count;
    DB_FIELD_T          aui4_recfld_type[1];
    const VOID*         apv_recfld_addr[1];

    /* Set up the key field address of the record to be updated */
    apv_keyfld_addrs[0]=&(ui2_rec_id);
    
    switch (ui4_fld_type)
    {
        case SATL_CDB_TYPE_PREV_REC_ID:
        {
            ui1_fld_count = 1;
            aui4_recfld_type[0]=(DB_FIELD_T) SATL_CDB_TYPE_PREV_REC_ID;
            apv_recfld_addr[0]=&(ui2_neighbor_id);
            i4_rc=db_update_rec(pt_satl->h_cdb,
                                apv_keyfld_addrs,
                                ui1_fld_count,
                                aui4_recfld_type,
                                apv_recfld_addr);
        }
        break;

        case SATL_CDB_TYPE_NEXT_REC_ID:
        {
            ui1_fld_count = 1;
            aui4_recfld_type[0]=(DB_FIELD_T) SATL_CDB_TYPE_NEXT_REC_ID;
            apv_recfld_addr[0]=&(ui2_neighbor_id);
            i4_rc=db_update_rec(pt_satl->h_cdb,
                                apv_keyfld_addrs,
                                ui1_fld_count,
                                aui4_recfld_type,
                                apv_recfld_addr);
        }
        break;
        
        default:
            return SATLR_FAIL;
    }
    
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    return i4_rc;
}
    
