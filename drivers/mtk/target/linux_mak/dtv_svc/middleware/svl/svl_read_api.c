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
 * $RCSfile: svl_read_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements service list (SVL) database API for read/search
 *    operation: getting a handle, searching/iterating for specific
 *    SVL records.
 *   
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "svl/_svl.h"
#include "svl/svl_rec.h"
#include "svl/svl_dbg.h"
/*-------------------------------------------------------------------------
                    static functin implementation
 --------------------------------------------------------------------------*/

BOOL svl_cursor_is_not_valid(SVL_CURSOR_T* pt_in_cursor)
{
    INT32    i4_n_byte;
    INT32    i4_i;
    BOOL     b_result;
    UINT8*   pui1_addr;
    
    i4_n_byte = (INT32) sizeof(SVL_CURSOR_T);

    pui1_addr = (UINT8*) pt_in_cursor;

    b_result  = TRUE;
    
    for (i4_i = 0 ; i4_i < i4_n_byte ; i4_i++ )
    {
        if ( pui1_addr[i4_i] != SVL_UNSET_VAL )
        {
            b_result = FALSE;
            break;
        }
    }
    
    return b_result;
}
        

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: svl_open_object
 *
 * Description: Get a handle to the previously created SVL object.
 *
 * Inputs: 
 *   ui2_svl_id:
 *   ps_svl_name:
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_open_object
(
    UINT16		    ui2_svl_id,
    const CHAR*     ps_svl_name,
    VOID*		    pv_tag,
    x_svl_nfy_fct   pf_nfy_fct,
    HANDLE_T*		ph_svl
)
{
    CHAR            ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    SVL_OBJ_T*      pt_svl=NULL;
    INT32           i4_rc=SVLR_FAIL;
    HANDLE_T        h_cdb=NULL_HANDLE;
    
    /* create CDB name. */
    svl_get_database_name(ps_svl_name,ui2_svl_id,ps_name);

    /* Create a SVL control block object. */
    pt_svl=(SVL_OBJ_T*) x_mem_alloc(sizeof(SVL_OBJ_T));
    
    if ( pt_svl != NULL )
    {
        /* Get a handle to the core database object.
           Note: the private tag field is used to
                 store the address of the SVL control block
                 structure.
        */
        i4_rc=db_get_handle_by_name(ps_name,
                                    (VOID*)pt_svl,
                                    svl_nfy_fct, /* Internal SVL-CDB
                                                    notification function.
                                                 */
                                    &h_cdb);
        
        if ( i4_rc == DBR_OK)
        {
            /*
              Allocate a handle for the SVL object.
            */
            if ( handle_alloc(SVLT_SERVICE_LIST,
                              (VOID*)pt_svl,
                              pv_tag,
                              svl_handle_free_fct,
                              ph_svl)
                 == HR_OK )
            {
                /* initialize the SVL object. */
                pt_svl->h_svl=*ph_svl;
                pt_svl->h_cdb=h_cdb;
                pt_svl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                  caller's notification
                                                  function.
                                               */
                pt_svl->ui2_svl_id=ui2_svl_id;
                x_strncpy(pt_svl->ps_svl_name,ps_svl_name,MAX_SVL_NAME_LEN);

                /* Set the initial search parameter and cursor position
                   to unknown initial values
                   This will force sorted list to be created and search
                   to start from the first element of array.
                */
                x_memset(&(pt_svl->t_parm), SVL_UNSET_VAL,
                         sizeof(SVL_SEARCH_PARM_T));

                x_memset(&(pt_svl->t_cursor), SVL_UNSET_VAL,
                         sizeof(SVL_CURSOR_T));

                x_memset(&(pt_svl->t_cmp_fct_info), 0,
                         sizeof(SVL_CMP_FCT_INFO_T));
                
                i4_rc=SVLR_OK;
            }
            else
            {
                handle_free(h_cdb, TRUE);
                x_mem_free(pt_svl);
                i4_rc=SVLR_OUT_OF_HANDLE;
            }
        }
        else
        {
            x_mem_free(pt_svl);
            /* translate the CDB error code to SVL error code */
            i4_rc=svl_cdb_to_svl_err_code(i4_rc);
        }  
    }
    else
    {
        i4_rc = SVLR_OUT_OF_MEM ;
    }
    
    return i4_rc;
}


INT32 svl_get_db_version_id
(
    const SVL_OBJ_T*  	pt_svl,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    DB_INFO_T           t_db_info;

    /*
      Retrieve the database info from the CoreDB database.
    */
    i4_rc = db_get_info(pt_svl->h_cdb, &t_db_info);
    if ( i4_rc == DBR_OK )
    {
        *pui4_ver_id = t_db_info.ui4_db_version_id;
    }
    /* translate the CDB error code to SVL error code */
    return svl_cdb_to_svl_err_code(i4_rc);
}

/*-----------------------------------------------------------------------------
 * Name:  svl_sort_rec_by_channel
 *
 * Description: This API calls the CDB sort function to sort the records.
 *              The order which SVL record are sorted are:
 *
 *              1. By default, it is sorted by the numeric value of the
 *                 channel number in SVL record.
 *
 *              2. If the user registered a global sort compare function,
 *                 then that sorting function will be called to determine
 *                 the order of SVL records.
 *
 *              3. If the user registere a per-handle sort compare function,
 *                 then this function will be used to determine the order
 *                 of SVL records.
 * Inputs: 
 *   pt_svl:    ref to SVL object.
 *
 * Outputs:
 *   pui4_ver_id: version id of the CDB database.
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_sort_rec_by_channel
(
    const SVL_OBJ_T*  	pt_svl,
    BOOL                b_forced,
    UINT32*			    pui4_ver_id
)
{
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    INT32               i4_rc;
    
    /* specifies the sort key. */
    ui1_key_count =1;
    aui4_keyfld_type[0]= (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID;
    /*
      convert SVL NULL version id to CDB null version id
    */
    if ( *pui4_ver_id == SVL_NULL_VER_ID )
    {
        *pui4_ver_id = CDB_NULL_VER_ID;
    }

    if ( pt_svl->t_cmp_fct_info.pf_sort_cmp  != NULL )
    {
        i4_rc = db_sort_rec_by_client_fct(pt_svl->h_cdb,
                                          svl_2_cdb_sort_cmp_fct_wrapper,
                                          (VOID*)&(pt_svl->t_cmp_fct_info),
                                          TRUE,
                                          b_forced,
                                          pui4_ver_id);        
    }
    else if ( db_has_global_cmp_fcts(pt_svl->h_cdb) == TRUE )
    {
        i4_rc = db_sort_rec_by_client_fct(pt_svl->h_cdb,
                                          NULL,
                                          NULL,
                                          TRUE,
                                          b_forced,
                                          pui4_ver_id);        
    } 
    else
    {
        i4_rc=db_sort_rec(pt_svl->h_cdb,
                          ui1_key_count,
                          aui4_keyfld_type,
                          NULL,
                          TRUE,
                          pui4_ver_id);
    }
    
    /* translate the CDB error code to SVL error code */
    return svl_cdb_to_svl_err_code(i4_rc);
}

INT32 svl_read_rec_from_sort_list
(
    SVL_OBJ_T*      	pt_svl,
    SVL_SEARCH_PARM_T*  pt_parm,
    UINT16              ui2_idx,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    INT16               i2_rec_to_skip;
    DB_CTRL_FLAG_T      t_ctl;
    INT32               i4_union_tag;

    /*
       Optimization:

       we determine if we can start the search from
       the last location or the search starts from the first
       record of sorted list.

       If the ui2_idx is 0, or the version ID is null, or the current
       search parameters is different than the last search parameters,
       then we must start the search from the beginning.

       Else, we set the cursor to the last found record position, and
       start the search from there.
    */
    if ( ui2_idx == 0                       ||
         *pui4_ver_id == SVL_NULL_VER_ID    ||
         svl_cursor_is_not_valid(&(pt_svl->t_cursor)) == TRUE         ||
         svl_cmp_search_parm(pt_parm, &(pt_svl->t_parm)) == FALSE
        )
    {
        /*
          Sorted the records by channel number.
        */  
        if(*pui4_ver_id == SVL_NULL_VER_ID)
        {
            DBG_INFO(("[!!!NOTE:]%s with version=0,maybe casue iterate slow down\n",__FUNCTION__));
        }
        
        i4_rc=svl_sort_rec_by_channel(pt_svl, FALSE, pui4_ver_id);
        if ( i4_rc != SVLR_OK )
        {
            /* IF sort list can not be re-sorted/created. */
            return i4_rc;
        }
        
        /* Start the search from the beginning of the sorted list. */
        db_set_cursor(pt_svl->h_cdb,DB_FIRST_RECORD,DB_NULL_RECORD);
        /*
           set the direction of iteration.
        */
        t_ctl=DB_NEXT_RECORD;
        /*
           If the the idex we are looking for is greater than 0, then
           we need know how many records to to skip.
        */
        i2_rec_to_skip= (INT16) ui2_idx;    
    }
    else
    {
        /* Use the cursor position from the last read position. */
        db_set_cursor(pt_svl->h_cdb,DB_GOTO_RECORD,pt_svl->t_cursor.t_pos);

        /*
           Determine the direction that we need to move the iteration
           cursor, and the number of record to skip as we search in that
           direction.
        */
        i2_rec_to_skip = (INT16) (ui2_idx - pt_svl->t_cursor.ui2_rec_idx) ;
        if ( i2_rec_to_skip > 0 )
        {
            t_ctl=DB_NEXT_RECORD;
        }
        else
        {
            t_ctl=DB_PREV_RECORD;
            /*
              We have the direction of search, change the
              the number of skip to positive number.
            */
            i2_rec_to_skip = i2_rec_to_skip * -1;
        }
    }
    
    /*
       Specifies the fields to retrieve the record data. For
       SVL record, we extract all 10 fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    i4_union_tag = 0; /* try to fix Klocwork issue */
    /* Set up the addresses to receive the SVL record data. */
    svl_rec_fld_to_addr(pt_svl_rec,
                        (const VOID**)apv_recfld_addr,
                        NULL,
                        0,
                        &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    while ( 1 )
    {
        i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                   ui1_fld_count,
                                   pui4_recfld_type,
                                   apv_recfld_addr,
                                   pui4_ver_id);
        
        if ( i4_rc == DBR_OK )
        {
            
            /* pedro tsai. assertion check */
            if ( ((INT32)pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type) !=
                 i4_union_tag )
            {
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL );
            }
    
            /* compare the SVL record and the search param values. */
            if ( svl_cmp_rec_search_parm(pt_parm,pt_svl_rec) == TRUE )
            {
          
                if ( i2_rec_to_skip == 0 )
                {
                    /* Save the search parameter record found. */
                    svl_save_search_parm_and_cursor(pt_svl, pt_parm, ui2_idx);
                    return SVLR_OK;
                }
                i2_rec_to_skip--;
            }
        }
        else
        {
            if ( i4_rc == DBR_END_OF_REC )
            {
                i4_rc=SVLR_REC_NOT_FOUND;
            }
            else
            {
                /* translate the CDB error code to SVL error code */
                i4_rc=svl_cdb_to_svl_err_code(i4_rc);
               
            }
            /* break out the while loop. */
            break;
        }
            
        /* Move the cursor to the the next or prev record.  */
        i4_rc=db_set_cursor(pt_svl->h_cdb,t_ctl, DB_NULL_RECORD);
        if ( i4_rc == DBR_END_OF_REC )
        {
            i4_rc=SVLR_REC_NOT_FOUND;
            break;
        }
    }
    return i4_rc;
}


INT32 svl_get_num_rec_by_flds
(
    const SVL_OBJ_T*  	  pt_svl,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    UINT16*			      pui2_num_recs,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    /*
      Retrieve the number of records from the CoreDB database.
    */
    i4_rc = db_get_num_of_rec(pt_svl->h_cdb,
                              ui1_key_count,
                              aui4_keyfld_type,
                              apv_keyfld_addr,
                              ae_cmp_param,
                              pui2_num_recs,
                              pui4_ver_id);
    
    /* translate the CDB error code to SVL error code */
    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
    
    return i4_rc;
}

INT32 svl_get_rec_by_flds
(
    const SVL_OBJ_T*  	  pt_svl,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    SVL_REC_T*			  pt_svl_rec,
    UINT16			      ui2_idx,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;
    
    /* Specifies the fields to retrieve the record data. For
       SVL record, we extract all fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    i4_union_tag = 0; /* try to fix Klocwork issue */
    /* Set up the addresses to copy the record data to. */
    svl_rec_fld_to_addr(pt_svl_rec,
                        (const VOID**)apv_recfld_addr,
                        NULL,
                        0,
                        &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    i4_rc = db_read_rec(pt_svl->h_cdb,
                        ui1_key_count,
                        aui4_keyfld_type,
                        apv_keyfld_addr,
                        ae_cmp_param,
                        ui1_fld_count,
                        pui4_recfld_type,
                        apv_recfld_addr,
                        ui2_idx,
                        pui4_ver_id);

    if ( i4_rc == DBR_OK )
    {
        /* pedro tsai, assertion check */
        if ( ((INT32)pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type) != i4_union_tag )
        {
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL );
        }
    }
    
    /* translate the CDB error code to SVL error code */
    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  svl_get_client_private_data
 *
 * Description: This API 'read's' the client's private data associatd with
 *    the specified SVL record.
 *
 *
 * Inputs: 
 *   pt_svl:           Ref to SVL object.
 *   ui1_key_count:    Number of key field for this SVL record.
 *   aui4_keyfld_type: Key field type.
 *   apv_keyfld_addr:  Addr(s) containing the key value for looking up the
 *                     specific record.
 *   ac_cmp_param:     Search parameters (opcode for compare, union compare functio,
 *                     union type value, and tag value.)
 *   ui2_idx:          Iteration idx.
 *
 * Outputs:
 *   pt_client_data:   Pointer to structure containing client data.
 *   pui4_ver_id:      Version id of the CDB database.
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_get_client_private_data
(
    const SVL_OBJ_T*  	  pt_svl,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    SVL_CLIENT_DATA_T*	  pt_client_data,
    UINT16			      ui2_idx,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;  
    DB_FIELD_T          pui4_recfld_type[SVL_NUM_CLIENT_DATA_FIELDS];
    VOID*               apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    UINT16              aui2_buf_len[SVL_NUM_OF_REC_FIELDS];
    
    /*
      check the client data structure
    */
    if ( pt_client_data->ui1_buf_len == 0    ||
         pt_client_data->pui1_data_buf == NULL
        )
    {
        return SVLR_INV_ARG;
    }
    else
    {
        x_memset(pt_client_data->pui1_data_buf, 0, pt_client_data->ui1_buf_len);
        pt_client_data->ui1_data_len = 0;
    }
    
    /* Specifies the fields to retrieve the record data. For
       client's private data, we extract fields
       SVL_CDB_TYPE_CLIENT_DATA_TAG and SVL_CDB_TYPE_CLIENT_DATA.
    */
    ui1_fld_count = SVL_NUM_CLIENT_DATA_FIELDS ;
    pui4_recfld_type[0] = (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA_TAG;
    pui4_recfld_type[1] = (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA;

    
    /* Set up the addresses to copy the record data to. */
    apv_recfld_addr[0]=&(pt_client_data->ui2_data_tag);
    apv_recfld_addr[1]=(pt_client_data->pui1_data_buf);

    /* Set the size of data buf in apv_recfld_addr[]. */
    aui2_buf_len[0]=2;
    aui2_buf_len[1]=pt_client_data->ui1_buf_len;
    
    /*
      Retrieve the record from the CoreDB database.
    */
    i4_rc = db_read_rec_v2(pt_svl->h_cdb,
                           ui1_key_count,
                           aui4_keyfld_type,
                           apv_keyfld_addr,
                           ae_cmp_param,
                           ui1_fld_count,
                           pui4_recfld_type,
                           apv_recfld_addr,
                           aui2_buf_len,
                           ui2_idx,
                           pui4_ver_id);
    if ( i4_rc == SVLR_REC_NOT_FOUND )
    {
        pt_client_data->ui1_data_len = 0;
    }
    else
    {
        pt_client_data->ui1_data_len = (UINT8) aui2_buf_len[1];
    }
    
    /* translate the CDB error code to SVL error code */
    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: svl_find_chan_netmask
 *
 * Description: This API find the record with specified channel number and
 *              netmask value, and return its cursor position.
 *
 * Inputs: 
 *   pt_svl:  SVL object.
 *   pt_parm: The search parameter containing the channel number and
 *            netmask value.
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_find_chan_netmask
(
    SVL_OBJ_T*      	pt_svl,
    SVL_SEARCH_PARM_T*  pt_parm,
    SVL_CURSOR_T*       pt_cursor
)
{
    UINT8               ui1_fld_count;
    SVL_REC_T           t_svl_rec;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[2];  /* only two field is readed. */
    INT32               i4_rc;
    DB_CTRL_FLAG_T      t_ctl;
    
    /* Get the field type definition of channel id and network mask.
       Set up the receiver addresses to get the channel id and network
       mask value.
    */
    svl_get_chan_nwmask_def(&ui1_fld_count,&pui4_recfld_type);

    /* Set up the addresses to receive the SVL channel and netmask
       data. */
    svl_chan_nwmask_fld_to_addr(&t_svl_rec,(const VOID**)apv_recfld_addr);

    /*
      Retrieve the record from the CoreDB database.
    */
    while ( 1 )
    {
        i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                   ui1_fld_count,
                                   pui4_recfld_type,
                                   apv_recfld_addr,
                                   &(pt_cursor->ui4_ver_id));
        
        if ( i4_rc == DBR_OK )
        {   
            /* compare the SVL record and the search param values. */
            if ( svl_cmp_rec_search_parm(pt_parm,&t_svl_rec) == TRUE )
            {
                /* Save the cursor position. */
                svl_save_search_parm_and_cursor(pt_svl,pt_parm,0);
                pt_cursor->t_pos=pt_svl->t_cursor.t_pos;
                return SVLR_OK;
            }
        }
        else
        {
            if ( i4_rc == DBR_END_OF_REC )
            {
                i4_rc=SVLR_REC_NOT_FOUND;
            }
            else
            {
                /* translate the CDB error code to SVL error code */
                i4_rc=svl_cdb_to_svl_err_code(i4_rc);
               
            }
            break;
        }
            
        /* Continue searching, move the cursor to the the next or prev record.  */
        if ( pt_parm->u.t_chan.ui4_channel_num >=  t_svl_rec.uheader.t_rec_hdr.ui4_channel_id )
        {
            t_ctl=DB_NEXT_RECORD;
        }
        else
        {
            t_ctl=DB_PREV_RECORD;
        }
        db_set_cursor(pt_svl->h_cdb, t_ctl, DB_NULL_RECORD);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: svl_iterate_from_sorted_list
 *
 * Description: This API returns the next/prev SVL record from the
 *              the current record location with the specified channel
 *              number.  If the specified channel number is not found,
 *              Then we return the nearest neighboring record.
 *
 *              For example:
 *              SVL records list with following channel number:
 *
 *              2.1 2.2  4.1 4.2 4.3 10.1 10.2 10.3 15.1 15.2
 *
 *              If caller specifies channel 2.2 with CHANNEL_PLUS argument,
 *              API returns  4.1
 *
 *              If caller specifies channel 2.2 with CHANNEL_MINUS argument,
 *              API returns  2.1
 *
 *              If caller specifies channel 3.1 with CHANNEL_PLUS argument,
 *              API returns  4.1
 *
 *              If caller specifies channel 3.1 with CHANNEL_MINUS argument,
 *              API returns  2.2
 *
 *              For the boundary cases:
 *
 *              If caller specifies channel 17.1 with CHANNEL_PLUS argument,
 *              API returns END_OF_ITERATION
 *
 *              If caller specifies channel 17.1 with CHANNEL_MINUS argument,
 *              API returns 15.2
 *
 *              If caller specifies channel  1.1 with CHANNEL_PLUS argument,
 *              API returns 2.1
 *
 *              If caller specifies channel 1.1 with CHANNEL_MINUS argument,
 *              API returns END_OF_ITERATION
 *
 *
 *              Note:
 *              The internal svl record is sorted by channel number.
 *
 * Inputs: 
 *   pt_svl:  SVL object.
 *   pt_parm: The search parameter containing the channel number and
 *            netmask value.
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_iterate_from_sorted_list
(
    SVL_OBJ_T*      	pt_svl,
    SVL_SEARCH_PARM_T*  pt_parm,
    SVL_ITERATE_T		e_iter_dir,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const VOID*         apv_keyfld_addr[1];
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    DB_CTRL_FLAG_T      t_ctl = DB_FIRST_RECORD;
    UINT32              ui4_ver_id;
    DB_RECORD_POS_T     t_rec_pos;
    DB_RECORD_POS_T     t_prev_rec;
    DB_RECORD_POS_T     t_next_rec;
    INT32               i4_union_tag;
    INT32               i4_reason;
    
    /*
      Specifies the fields to retrieve the record data. For
      SVL record, we extract all 10 fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count, &pt_rec_fields, &pui4_recfld_type);
    
    i4_union_tag = 0; /* try to fix Klocwork issue */
    /* Set up the addresses to receive the SVL record data. */
    svl_rec_fld_to_addr(pt_svl_rec,
                        (const VOID**)apv_recfld_addr,
                        NULL,
                        0,
                        &i4_union_tag);

    /*
      Specifies the sorting parameter definition.
    */
    ui1_key_count=1;
    aui4_keyfld_type[0]= (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID;
    apv_keyfld_addr[0]=&(pt_parm->u.t_chan.ui4_channel_num);
    
    ui4_ver_id = SVL_NULL_VER_ID;
    
    while (1)
    {
        /*
          Sorted the records by channel number.
        */
        if (
            (*pui4_ver_id == SVL_NULL_VER_ID)                          ||
            (svl_cursor_is_not_valid(&(pt_svl->t_cursor)) == TRUE )    ||
            (svl_cmp_search_parm(pt_parm, &(pt_svl->t_parm)) == FALSE) ||
            (e_iter_dir == SVL_CHANNEL_SMALLEST)                       ||
            (e_iter_dir == SVL_CHANNEL_LARGEST)
            )
        {
            i4_rc=svl_sort_rec_by_channel(pt_svl, FALSE, &ui4_ver_id);
            if ( i4_rc != SVLR_OK )
            {
                /* IF sort list can not be re-sorted/created. */
                break;
            }
        }
        
        switch (e_iter_dir)
        {
            case SVL_CHANNEL_SMALLEST:
            {
                /* Start the search from the first record of the
                   sorted list. */
                db_set_cursor(pt_svl->h_cdb,DB_FIRST_RECORD,DB_NULL_RECORD);
                t_ctl=DB_NEXT_RECORD;
                i4_reason = SVLR_OK;
            }
            break;

            case SVL_CHANNEL_LARGEST:
            {
                /* Start the search from the last record of the sorted list.*/
                db_set_cursor(pt_svl->h_cdb,DB_LAST_RECORD,DB_NULL_RECORD);
                t_ctl=DB_PREV_RECORD;
                i4_reason = SVLR_OK;
            }
            break;

            case SVL_CHANNEL_PLUS:
            case SVL_CHANNEL_MINUS:
            {
                /*
                      Find the position of the record with specified channel number.
                */ 
                if ( pt_svl->t_cmp_fct_info.pf_search_cmp != NULL )
                {
                    i4_rc = db_find_sorted_rec_and_neighbors_by_client_fct(
                        pt_svl->h_cdb,
                        (VOID*)pt_parm->u.t_chan.ui4_channel_num,
                        svl_2_cdb_search_cmp_fct_wrapper,
                        (VOID*)&(pt_svl->t_cmp_fct_info),
                        0,
                        &t_rec_pos,
                        &t_prev_rec,
                        &t_next_rec,
                        &ui4_ver_id);
                }
                else if ( db_has_global_cmp_fcts(pt_svl->h_cdb) == TRUE )
                {
                    i4_rc = db_find_sorted_rec_and_neighbors_by_client_fct(
                        pt_svl->h_cdb,
                        (VOID*)pt_parm->u.t_chan.ui4_channel_num,
                        NULL,
                        NULL,
                        0,
                        &t_rec_pos,
                        &t_prev_rec,
                        &t_next_rec,
                        &ui4_ver_id);
                }
                else
                {               
                    i4_rc=db_find_sorted_rec_and_neighbors(
                        pt_svl->h_cdb,
                        ui1_key_count,
                        aui4_keyfld_type,
                        apv_keyfld_addr,
                        NULL,
                        0,
                        &t_rec_pos,
                        &t_prev_rec,
                        &t_next_rec,
                        &ui4_ver_id);
                }
                
                
                if (i4_rc==DBR_OK )
                {
                    /*
                       Go to the specific record
                    */
                    db_set_cursor(pt_svl->h_cdb,DB_GOTO_RECORD,t_rec_pos);
                    /*
                      Set the iteration direction.
                    */
                    if ( e_iter_dir == SVL_CHANNEL_PLUS )
                    {
                        t_ctl=DB_NEXT_RECORD;
                    }
                    else
                    {
                        t_ctl=DB_PREV_RECORD;
                    }
                    /* Move to the next record. */
                    i4_rc=db_set_cursor(pt_svl->h_cdb,t_ctl, DB_NULL_RECORD);
                    /*
                      Check if we are not at end of list.
                    */
                    if ( i4_rc == DBR_END_OF_REC )
                    {
                        return SVLR_END_OF_ITERATION;
                    }

                    
                    {
                    /*
                       To avoid duplicate channel number, the following code 
                       skip over records with the same channel number.

                       Note:

                       if 2 or more records has the same channel number, but other fields in
                       the record contains different data, then there could be case where
                       inconsistency will result:
                       
                       for example:

	                   record    chan number      other data in the record (for example: netmask)

	                   1           100               .... 
                       2           111               0x0A
                       3           111               0x0B
                       4           111               0x0C
                       5           121               ....
                       6           124               ....


                       In one case, if iterating from large channel to small channel (channel minus),
                       then after 121, you will get record with chan 111 (with 0x0C netmask).

                       But, if iterating from small channel to large channel (channel plus), the next
                       record after 100, you will get record with chan 111 (with 0x0A netmask).

                    */
                        UINT8               ui1_count  = 1;
                        DB_FIELD_T          ui4_recfld_type = (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID;
                        VOID*               apv_channel_num_addr[1];
                        UINT32              ui4_channel_num ;

                        apv_channel_num_addr[0] = &ui4_channel_num ;

                        /* Read this record */
                        i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                                   ui1_count,
                                                   &ui4_recfld_type,
                                                   apv_channel_num_addr,
                                                   &ui4_ver_id);
                        
                        while ( (i4_rc == DBR_OK)  )
                        {
                            if (ui4_channel_num == pt_parm->u.t_chan.ui4_channel_num )
                            {
                                
                                DBG_ERROR(("<WARNNING> duplicate channel number [%d] found in the service list.\n",ui4_channel_num));

                                /* move to next */
                                i4_rc=db_set_cursor(pt_svl->h_cdb, t_ctl, DB_NULL_RECORD);
                                 
                                if ( i4_rc ==  DBR_END_OF_REC )
                                {
                                    return SVLR_END_OF_ITERATION;;
                                }
                                i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                                           ui1_count,
                                                           &ui4_recfld_type,
                                                           apv_channel_num_addr,
                                                           &ui4_ver_id);
                                if ( i4_rc != DBR_OK )
                                {
                                    return SVLR_FAIL;
                                }   
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                   
                    /*
                      using caller specified record to iterate
                    */
                    i4_reason = SVLR_OK;
                }
                else if (i4_rc==DBR_REC_NOT_FOUND)
                {
                    /*
                      The specified record is not found, try to
                      use the nearest record.
                    */
                    if ( e_iter_dir == SVL_CHANNEL_MINUS )
                    {
                        if ( t_prev_rec != DB_NULL_RECORD )
                        {
                            db_set_cursor(pt_svl->h_cdb, DB_GOTO_RECORD,
                                          t_prev_rec);
                            /*
                              Set the iteration direction, we need to
                              do this because we need to look for the
                              prev record until netmask paramter matches.
                            */
                            t_ctl=DB_PREV_RECORD;
                        }
                        else
                        {
                            return SVLR_END_OF_ITERATION;
                        }
                    }
                    

                    if ( e_iter_dir == SVL_CHANNEL_PLUS )
                    {
                        if ( t_next_rec != DB_NULL_RECORD )
                        {
                            db_set_cursor(pt_svl->h_cdb, DB_GOTO_RECORD,
                                      t_next_rec);
                            /*
                              Set the iteration direction, we need to
                              do this because we need to look for the
                              next record until netmask paramter matches.
                            */
                            t_ctl=DB_NEXT_RECORD;
                        }
                        else
                        {
                            return SVLR_END_OF_ITERATION;
                        }
                    }
                    /*
                      using the nearest record to user specified record
                      to iterate.
                    */
                    i4_reason = SVLR_NEAREST_CHAN_MATCH;
                }
                else
                {
                    /* Error from finding record from sortlist. */
                    return SVLR_FAIL;
                }
            }
            break;

            default:
            {
                return SVLR_INV_ARG;
            }
        }
        
        /*
          Retrieve the record from the CoreDB database, compare each
          record with the selected bitmask.
        */
        while ( 1 )
        {
            i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                       ui1_fld_count,
                                       pui4_recfld_type,
                                       apv_recfld_addr,
                                       &ui4_ver_id);
        
            if ( i4_rc == DBR_OK )
            {
                /* pedro tsai. assertion check. */
                if ( ((INT32)pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type) !=
                     i4_union_tag )
                {
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL );
                }
                
                /* compare the SVL record and the search param values. */
                if ( svl_cmp_rec_nwmask_parm(pt_parm,pt_svl_rec) == TRUE )
                {
                    /* save the new database version id. */
                    *pui4_ver_id = ui4_ver_id;
                    
                    return i4_reason;
                }
            }
            else
            {
                if ( i4_rc == DBR_END_OF_REC )
                {
                    return SVLR_END_OF_ITERATION;
                }
                else if ( i4_rc == DBR_MODIFIED )
                {

                    /*
                      If the search is starting from beginning,
                      then we try again.
                    */
                    if ( *pui4_ver_id == SVL_NULL_VER_ID )
                    {
                        /*
                          break out the inner loop, re-try
                          again. 
                        */
                        break;
                    }
                    else
                    {
                        /*
                          search is inside the loop, exit and let the
                          caller handle this.
                        */
                        return SVLR_MODIFIED;
                    }
                }
                else
                {
                    /* translate the CDB error code to SVL error code */
                    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
                    return i4_rc;
                }
            }
            /* Move the cursor to the the next or prev record.  */
            i4_rc=db_set_cursor(pt_svl->h_cdb,t_ctl, DB_NULL_RECORD);
            if ( i4_rc == DBR_END_OF_REC )
            {
                return SVLR_END_OF_ITERATION;
            }
        }
        
    } /* End of outer while loop. */
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: svl_iterate_from_customized_sorted_list
 *
 * Description: This API returns the next/prev SVL record from the
 *              the current record location with the specified channel
 *              number.  If the specified channel number is not found,
 *              Then we return the nearest neighboring record.
 *
 *              For example:
 *              SVL records list with following channel number:
 *
 *              2.1 2.2  4.1 4.2 4.3 10.1 10.2 10.3 15.1 15.2
 *
 *              If caller specifies channel 2.2 with CHANNEL_PLUS argument,
 *              API returns  4.1
 *
 *              If caller specifies channel 2.2 with CHANNEL_MINUS argument,
 *              API returns  2.1
 *
 *              If caller specifies channel 3.1 with CHANNEL_PLUS argument,
 *              API returns  4.1
 *
 *              If caller specifies channel 3.1 with CHANNEL_MINUS argument,
 *              API returns  2.2
 *
 *              For the boundary cases:
 *
 *              If caller specifies channel 17.1 with CHANNEL_PLUS argument,
 *              API returns END_OF_ITERATION
 *
 *              If caller specifies channel 17.1 with CHANNEL_MINUS argument,
 *              API returns 15.2
 *
 *              If caller specifies channel  1.1 with CHANNEL_PLUS argument,
 *              API returns 2.1
 *
 *              If caller specifies channel 1.1 with CHANNEL_MINUS argument,
 *              API returns END_OF_ITERATION
 *
 *
 *              Note:
 *              The internal svl record is sorted by channel number.
 *
 * Inputs: 
 *   pt_svl:  SVL object.
 *   pt_parm: The search parameter containing the channel number and
 *            netmask value.
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK           SVL handle is returned.
 *   SVLR_OUT_OF_MEM   Not enough memory to create an SVL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 svl_iterate_from_customized_sorted_list
(
    SVL_OBJ_T*      	pt_svl,
    SVL_SEARCH_PARM_T*  pt_parm,
    SVL_ITERATE_T		e_iter_dir,
    SVL_REC_T*			pt_svl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SVL_NUM_OF_REC_FIELDS];
    DB_CTRL_FLAG_T      t_ctl = DB_FIRST_RECORD;
    UINT32              ui4_ver_id;
    DB_RECORD_POS_T     t_rec_pos;
    DB_RECORD_POS_T     t_prev_rec;
    DB_RECORD_POS_T     t_next_rec;
    INT32               i4_union_tag;
    INT32               i4_reason;
    BOOL                b_flag = TRUE;
    
    /*
      Specifies the fields to retrieve the record data. For
      SVL record, we extract all 10 fields of the data.
    */
    svl_get_rec_def(&ui1_fld_count, &pt_rec_fields, &pui4_recfld_type);
    
    i4_union_tag = 0; /* try to fix Klocwork issue */
    /* Set up the addresses to receive the SVL record data. */
    svl_rec_fld_to_addr(pt_svl_rec,
                        (const VOID**)apv_recfld_addr,
                        NULL,
                        0,
                        &i4_union_tag);
    
    ui4_ver_id = SVL_NULL_VER_ID;
    
    while (b_flag==TRUE)
    {
        switch (e_iter_dir)
        {
            case SVL_CHANNEL_SMALLEST:
            {
                /* Start the search from the first record of the
                   sorted list. */
                db_set_cursor(pt_svl->h_cdb,DB_FIRST_RECORD,DB_NULL_RECORD);
                t_ctl=DB_NEXT_RECORD;
                i4_reason = SVLR_OK;
            }
            break;

            case SVL_CHANNEL_LARGEST:
            {
                /* Start the search from the last record of the sorted list.*/
                db_set_cursor(pt_svl->h_cdb,DB_LAST_RECORD,DB_NULL_RECORD);
                t_ctl=DB_PREV_RECORD;
                i4_reason = SVLR_OK;
            }
            break;

            case SVL_CHANNEL_PLUS:
            case SVL_CHANNEL_MINUS:
            {
                /*
                      Find the position of the record with specified channel number.
                */ 
                if ( pt_svl->t_cmp_fct_info.pf_search_cmp != NULL )
                {
                    i4_rc = db_find_sorted_rec_and_neighbors_by_client_fct(
                        pt_svl->h_cdb,
                        (VOID*)pt_parm->pv_data,
                        svl_2_cdb_search_cmp_fct_wrapper,
                        (VOID*)&(pt_svl->t_cmp_fct_info),
                        0,
                        &t_rec_pos,
                        &t_prev_rec,
                        &t_next_rec,
                        &ui4_ver_id);
                }
                else if ( db_has_global_cmp_fcts(pt_svl->h_cdb) == TRUE )
                {
                    i4_rc = db_find_sorted_rec_and_neighbors_by_client_fct(
                        pt_svl->h_cdb,
                        (VOID*)pt_parm->pv_data,
                        NULL,
                        NULL,
                        0,
                        &t_rec_pos,
                        &t_prev_rec,
                        &t_next_rec,
                        &ui4_ver_id);
                }
                else
                {
                    i4_rc = DBR_FAIL;
                    DBG_ERROR(("{CDB} Error: No custom sort function registered.\n"));
                }
                
                if (i4_rc==DBR_OK )
                {
                    /*
                       Go to the specific record
                    */
                    db_set_cursor(pt_svl->h_cdb,DB_GOTO_RECORD,t_rec_pos);
                    /*
                      Set the iteration direction.
                    */
                    if ( e_iter_dir == SVL_CHANNEL_PLUS )
                    {
                        t_ctl=DB_NEXT_RECORD;
                    }
                    else
                    {
                        t_ctl=DB_PREV_RECORD;
                    }
                    /* Move to the next record. */
                    i4_rc=db_set_cursor(pt_svl->h_cdb,t_ctl, DB_NULL_RECORD);
                    /*
                      Check if we are not at end of list.
                    */
                    if ( i4_rc == DBR_END_OF_REC )
                    {
                        return SVLR_END_OF_ITERATION;
                    }
                    /*
                      using caller specified record to iterate
                    */
                    i4_reason = SVLR_OK;
                }
                else if (i4_rc==DBR_REC_NOT_FOUND)
                {
                    /*
                      The specified record is not found, try to
                      use the nearest record.
                    */
                    if ( e_iter_dir == SVL_CHANNEL_MINUS )
                    {
                        if ( t_prev_rec != DB_NULL_RECORD )
                        {
                            db_set_cursor(pt_svl->h_cdb, DB_GOTO_RECORD,
                                          t_prev_rec);
                            /*
                              Set the iteration direction, we need to
                              do this because we need to look for the
                              prev record until netmask paramter matches.
                            */
                            t_ctl=DB_PREV_RECORD;
                        }
                        else
                        {
                            return SVLR_END_OF_ITERATION;
                        }
                    }
                    

                    if ( e_iter_dir == SVL_CHANNEL_PLUS )
                    {
                        if ( t_next_rec != DB_NULL_RECORD )
                        {
                            db_set_cursor(pt_svl->h_cdb, DB_GOTO_RECORD,
                                      t_next_rec);
                            /*
                              Set the iteration direction, we need to
                              do this because we need to look for the
                              next record until netmask paramter matches.
                            */
                            t_ctl=DB_NEXT_RECORD;
                        }
                        else
                        {
                            return SVLR_END_OF_ITERATION;
                        }
                    }
                    /*
                      using the nearest record to user specified record
                      to iterate.
                    */
                    i4_reason = SVLR_NEAREST_CHAN_MATCH;
                }
                else
                {
                    /* Error from finding record from sortlist. */
                    return SVLR_FAIL;
                }
            }
            break;

            default:
            {
                return SVLR_INV_ARG;
            }
        }
        
        /*
          Retrieve the record from the CoreDB database, compare each
          record with the selected bitmask.
        */
        while ( 1 )
        {
            i4_rc = db_read_sorted_rec(pt_svl->h_cdb,
                                       ui1_fld_count,
                                       pui4_recfld_type,
                                       apv_recfld_addr,
                                       &ui4_ver_id);
        
            if ( i4_rc == DBR_OK )
            {
                /* pedro tsai. assertion check. */
                if ( ((INT32)pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type) !=
                     i4_union_tag )
                {
                    ABORT(DBG_CAT_INV_OP,DBG_ABRT_DB_OPERATION_FAIL );
                }
                
                /* compare the SVL record and the search param values. */
                if ( svl_cmp_rec_nwmask_parm(pt_parm,pt_svl_rec) == TRUE )
                {
                    /* save the new database version id. */
                    *pui4_ver_id = ui4_ver_id;
                    
                    return i4_reason;
                }
            }
            else
            {
                if ( i4_rc == DBR_END_OF_REC )
                {
                    return SVLR_END_OF_ITERATION;
                }
                else if ( i4_rc == DBR_MODIFIED )
                {

                    /*
                      If the search is starting from beginning,
                      then we try again.
                    */
                    if ( *pui4_ver_id == SVL_NULL_VER_ID )
                    {
                        /*
                          break out the inner loop, re-try
                          again. 
                        */
                        break;
                    }
                    else
                    {
                        /*
                          search is inside the caller's loop, exit and let the
                          caller handle this.
                        */
                        b_flag = FALSE;
                        return SVLR_MODIFIED;
                    }
                }
                else
                {
                    /* translate the CDB error code to SVL error code */
                    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
                    return i4_rc;
                }
            }
            /* Move the cursor to the the next or prev record.  */
            i4_rc=db_set_cursor(pt_svl->h_cdb,t_ctl, DB_NULL_RECORD);
            if ( i4_rc == DBR_END_OF_REC )
            {
                return SVLR_END_OF_ITERATION;
            }
        }
        
    } /* End of outer while loop. */
    return i4_rc;
}


INT32 svl_get_multi_lang
(
    const SVL_OBJ_T*  	  pt_svl,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    SVL_MULTI_LANG_T *	  pt_multi_lang,
    UINT16			      ui2_idx,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;  
    DB_FIELD_T          pui4_recfld_type[SVL_NUM_MULTI_LANG_FIELDS];
    VOID*               apv_recfld_addr[SVL_NUM_MULTI_LANG_FIELDS];
    UINT16              aui2_buf_len[SVL_NUM_MULTI_LANG_FIELDS];
    
    /*
      check the client data structure
    */
    if ( pt_multi_lang->ui1_buf_len == 0    ||
         pt_multi_lang->pui1_data_buf == NULL
        )
    {
        return SVLR_INV_ARG;
    }
    else
    {
        x_memset(pt_multi_lang->pui1_data_buf, 0, pt_multi_lang->ui1_buf_len);
        pt_multi_lang->ui1_data_len = 0;
    }
    
    /* Specifies the fields to retrieve the record data. For
       client's private data, we extract fields
       SVL_CDB_TYPE_CLIENT_DATA_TAG and SVL_CDB_TYPE_CLIENT_DATA.
    */
    ui1_fld_count = SVL_NUM_MULTI_LANG_FIELDS ;
    pui4_recfld_type[0] = (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG_TAG;
    pui4_recfld_type[1] = (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG;

    
    /* Set up the addresses to copy the record data to. */
    apv_recfld_addr[0]=&(pt_multi_lang->ui2_data_tag);
    apv_recfld_addr[1]=(pt_multi_lang->pui1_data_buf);

    /* Set the size of data buf in apv_recfld_addr[]. */
    aui2_buf_len[0]=2;
    aui2_buf_len[1]=pt_multi_lang->ui1_buf_len;
    
    /*
      Retrieve the record from the CoreDB database.
    */
    i4_rc = db_read_rec_v2(pt_svl->h_cdb,
                           ui1_key_count,
                           aui4_keyfld_type,
                           apv_keyfld_addr,
                           ae_cmp_param,
                           ui1_fld_count,
                           pui4_recfld_type,
                           apv_recfld_addr,
                           aui2_buf_len,
                           ui2_idx,
                           pui4_ver_id);
    if ( i4_rc == SVLR_REC_NOT_FOUND )
    {
        pt_multi_lang->ui1_data_len = 0;
    }
    else
    {
        pt_multi_lang->ui1_data_len = (UINT8) aui2_buf_len[1];
    }
    
    /* translate the CDB error code to SVL error code */
    i4_rc=svl_cdb_to_svl_err_code(i4_rc);
    
    return i4_rc;
}



