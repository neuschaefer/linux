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
 * $RCSfile: satl_read_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements service list (SATL) database API for read/search
 *    operation: getting a handle, searching/iterating for specific
 *    SATL records.
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

/*-------------------------------------------------------------------------
                    static functin implementation
 --------------------------------------------------------------------------*/

static BOOL satl_cursor_is_not_valid(SATL_CURSOR_T* pt_in_cursor)
{
    INT32    i4_n_byte;
    INT32    i4_i;
    BOOL     b_result;
    UINT8*   pui1_addr;
    
    i4_n_byte = (INT32)sizeof(SATL_CURSOR_T);

    pui1_addr = (UINT8*) pt_in_cursor;

    b_result  = TRUE;
    
    for (i4_i = 0 ; i4_i < i4_n_byte ; i4_i++ )
    {
        if ( pui1_addr[i4_i] != SATL_UNSET_VAL )
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
 * Name: satl_open_object
 *
 * Description: Get a handle to the previously created SATL object.
 *
 * Inputs: 
 *   ui2_satl_id:
 *   ps_satl_name:
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK          SATL handle is returned.
 *   SATLR_OUT_OF_MEM  Not enough memory to create an SATL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 satl_open_object
(
    UINT16		    ui2_satl_id,
    const CHAR*     ps_satl_name,
    VOID*		    pv_tag,
    x_satl_nfy_fct  pf_nfy_fct,
    HANDLE_T*		ph_satl
)
{
    CHAR            ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    SATL_OBJ_T*     pt_satl=NULL;
    INT32           i4_rc=SATLR_FAIL;
    HANDLE_T        h_cdb=NULL_HANDLE;
    
    /* create CDB name. */
    satl_get_database_name(ps_satl_name,ui2_satl_id,ps_name);

    /* Create a SATL control block object. */
    pt_satl=(SATL_OBJ_T*) x_mem_alloc(sizeof(SATL_OBJ_T));
    
    if ( pt_satl != NULL )
    {
        /* Get a handle to the core database object.
           Note: the private tag field is used to
                 store the address of the SATL control block
                 structure.
        */
        i4_rc=db_get_handle_by_name(ps_name,
                                    (VOID*)pt_satl,
                                    satl_nfy_fct, /* Internal SATL-CDB
                                                    notification function.
                                                 */
                                    &h_cdb);
        
        if ( i4_rc == DBR_OK)
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
                                                  function.
                                               */
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
                handle_free(h_cdb, TRUE);
                x_mem_free(pt_satl);
                i4_rc=SATLR_OUT_OF_HANDLE;
            }
        }
        else
        {
            x_mem_free(pt_satl);
            /* translate the CDB error code to SATL error code */
            i4_rc=satl_cdb_to_satl_err_code(i4_rc);
        }
    }
    else
    {
        i4_rc = SATLR_OUT_OF_MEM ;
    }
    
    return i4_rc;
}

INT32 satl_read_common_data_from_object
(
    SATL_OBJ_T*       pt_satl,
    VOID*             ps_common,
    INT32             i4_common_size
)
{
    INT32      i4_rc;

    i4_rc=db_read_common(pt_satl->h_cdb,ps_common,(UINT16)i4_common_size);
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    return i4_rc;
}

INT32 satl_get_rec_from_obj
(
    const SATL_OBJ_T* 	pt_satl,
    UINT16			    ui2_satl_rec_id,
    SATL_REC_T*		pt_satl_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    UINT8               ui1_key_count;
    UINT8               ui1_fld_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    const  VOID*        apv_keyfld_addr[1];
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SATL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    DB_UNION_TAG_T      at_utag[SATL_NUM_UNION_FIELDS];
    
    /* Set up the search parameter and address of the key value
       for the search.
    */
    ui1_key_count = 1;
    aui4_keyfld_type[0] = (DB_FIELD_T)SATL_CDB_TYPE_REC_ID ;
    apv_keyfld_addr[0] = &ui2_satl_rec_id;

    /* Specifies the fields to retrieve the record data. For
       SATL record, we extract all 10 fields of the data.
    */
    satl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to copy the record data to. */
    satl_rec_fld_to_addr(pt_satl_rec,at_utag,(const VOID**)apv_recfld_addr,0);

    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_rec(pt_satl->h_cdb,
                        ui1_key_count,
                        aui4_keyfld_type,
                        apv_keyfld_addr,
                        (DB_SEARCH_T*)NULL,
                        ui1_fld_count,
                        pui4_recfld_type,
                        apv_recfld_addr,
                        0,
                        &ui4_db_version);
    if ( i4_rc == DBR_OK || i4_rc == DBR_REC_NOT_FOUND )
    {
        *pui4_ver_id = ui4_db_version;
    }
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    
    return i4_rc;
}

INT32 satl_sort_rec_by_ts_id
(
    const SATL_OBJ_T* 	pt_satl,
    UINT32*			    pui4_ver_id
)
{
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    INT32               i4_rc;
    
    /* specifies the sort key. */
    ui1_key_count =1;
    aui4_keyfld_type[0]=(DB_FIELD_T)SATL_CDB_TYPE_REC_ID;
    /*
      convert SATL NULL version id to CDB null version id
    */
    if ( *pui4_ver_id == SATL_NULL_VER_ID )
    {
        *pui4_ver_id = CDB_NULL_VER_ID;
    }
    
    i4_rc=db_sort_rec(pt_satl->h_cdb,
                      ui1_key_count,
                      aui4_keyfld_type,
                      NULL,
                      TRUE,
                      pui4_ver_id);
    /* translate the CDB error code to SATL error code */
    return satl_cdb_to_satl_err_code(i4_rc);
}

/*-----------------------------------------------------------------------------
 * Name: satl_read_rec_from_sort_list
 *
 * Description: Read SATL record from the sorted list by specified
 *    search parameters
 *
 * Inputs: 
 *   pt_satl:
 *   ps_satl_name:
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK          SATL handle is returned.
 *   SATLR_OUT_OF_MEM  Not enough memory to create an SATL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 satl_read_rec_from_sort_list
(
    SATL_OBJ_T*     	pt_satl,
    SATL_SEARCH_PARM_T* pt_parm,
    UINT16              ui2_idx,
    SATL_REC_T*		pt_satl_rec,
    UINT32*			    pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SATL_NUM_OF_REC_FIELDS];
    INT16               i2_rec_to_skip;
    DB_CTRL_FLAG_T      t_ctl;
    DB_UNION_TAG_T      at_utag[SATL_NUM_UNION_FIELDS];

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
         *pui4_ver_id == SATL_NULL_VER_ID   ||
         satl_cursor_is_not_valid(&(pt_satl->t_cursor)) == TRUE         ||
         satl_cmp_search_parm(pt_parm, &(pt_satl->t_parm)) == FALSE
        )
    {
        /*
          Sorted the records by satellite id.
        */
        i4_rc=satl_sort_rec_by_ts_id(pt_satl, pui4_ver_id);
        if ( i4_rc != SATLR_OK )
        {
            /* sort list can not be re-sorted/created. */
            return i4_rc;
        }
        
        /* Start the search from the beginning of the sorted list. */
        db_set_cursor(pt_satl->h_cdb,DB_FIRST_RECORD,DB_NULL_RECORD);
        /*
           set the direction of iteration.
        */
        t_ctl=DB_NEXT_RECORD;
        /*
           If the the idex we are looking for is greater than 0, then
           we need know how many records to to skip.
        */
        i2_rec_to_skip=(INT16)ui2_idx;    
    }
    else
    {
        /* Use the cursor position from the last read position. */
        db_set_cursor(pt_satl->h_cdb,DB_GOTO_RECORD,pt_satl->t_cursor.t_pos);

        /*
           Determine the direction that we need to move the iteration
           cursor, and the number of record to skip as we search in that
           direction.
        */
        i2_rec_to_skip = (INT16)(ui2_idx - pt_satl->t_cursor.ui2_rec_idx) ;
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
       SATL record, we extract all 10 fields of the data.
    */
    satl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to receive the SATL record data. */
    satl_rec_fld_to_addr(pt_satl_rec,
                         at_utag, 
                         (const VOID**)apv_recfld_addr,
                         0);

    /*
      Retrieve the record from the CoreDB database.
    */
    while ( 1 )
    {
        i4_rc = db_read_sorted_rec(pt_satl->h_cdb,
                                   ui1_fld_count,
                                   pui4_recfld_type,
                                   apv_recfld_addr,
                                   pui4_ver_id);
        
        if ( i4_rc == DBR_OK )
        {      
    
            /* compare the SATL record and the search param values. */
            if ( satl_cmp_rec_search_parm(pt_parm,pt_satl_rec) == TRUE )
            {
                if ( i2_rec_to_skip == 0 )
                {
                    /* Save the search parameter record found. */
                    satl_save_search_parm_and_cursor(pt_satl, pt_parm, ui2_idx);
                    return SATLR_OK;
                }
                i2_rec_to_skip--;
            }
        }
        else
        {
            if ( i4_rc == DBR_END_OF_REC )
            {
                i4_rc=SATLR_REC_NOT_FOUND;
            }
            else
            {
                /* translate the CDB error code to SATL error code */
                i4_rc=satl_cdb_to_satl_err_code(i4_rc);
               
            }
            /* break out the while loop. */
            break;
        }
            
        /* Move the cursor to the the next or prev record.  */
        i4_rc=db_set_cursor(pt_satl->h_cdb,t_ctl, DB_NULL_RECORD);
        if ( i4_rc == DBR_END_OF_REC )
        {
            i4_rc=SATLR_REC_NOT_FOUND;
            break;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_get_num_rec_by_flds
 *
 * Description: Get the number of SATL records that match specified 
 *    search parameters
 *
 * Inputs: 
 *   pt_satl:
 *   ui1_key_count:
 *   pv_tag:
 *   pf_nfy_fct:
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK          SATL handle is returned.
 *   SATLR_OUT_OF_MEM  Not enough memory to create an SATL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 satl_get_num_rec_by_flds
(
    const SATL_OBJ_T* 	  pt_satl,
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
    i4_rc = db_get_num_of_rec(pt_satl->h_cdb,
                              ui1_key_count,
                              aui4_keyfld_type,
                              apv_keyfld_addr,
                              ae_cmp_param,
                              pui2_num_recs,
                              pui4_ver_id);
    
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    
    return i4_rc;
}

INT32 satl_get_num_rec
(
    const SATL_OBJ_T* 	pt_satl,
    UINT16*             pui2_num_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    DB_INFO_T           t_db_info;

    /*
      Retrieve the database info from the CoreDB database.
    */
    i4_rc = db_get_info(pt_satl->h_cdb, &t_db_info);
    if ( i4_rc == DBR_OK )
    {
        *pui2_num_rec = (UINT16) t_db_info.ui4_record_count;
        *pui4_ver_id = t_db_info.ui4_db_version_id;
    }
    /* translate the CDB error code to SATL error code */
    return satl_cdb_to_satl_err_code(i4_rc);
}



INT32 satl_read_rec_at_cursor_pos
(
    const SATL_OBJ_T* 	pt_satl,
    SATL_REC_T*		pt_satl_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;

    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[SATL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    DB_UNION_TAG_T      at_utag[SATL_NUM_UNION_FIELDS];

    /* Specifies the fields to retrieve the record data. For
       SATL record, we extract all 10 fields of the data.
    */
    satl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to copy the record data to. */
    satl_rec_fld_to_addr(pt_satl_rec,at_utag,(const VOID**)apv_recfld_addr,0);

    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_sorted_rec(pt_satl->h_cdb,
                               ui1_fld_count,
                               pui4_recfld_type,
                               apv_recfld_addr,
                               &ui4_db_version);
    
    if ( i4_rc == DBR_OK || i4_rc == DBR_REC_NOT_FOUND )
    {
        *pui4_ver_id = ui4_db_version;
    }
    /* translate the CDB error code to SATL error code */
    i4_rc=satl_cdb_to_satl_err_code(i4_rc);
    
    return i4_rc;
}





