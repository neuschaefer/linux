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
 * $RCSfile: lol_read_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements service list (TSL) database API for read/search
 *    operation: getting a handle, searching/iterating for specific
 *    TSL records.
 *   
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"

#include "lol/_lol.h"
#include "lol/lol_rec.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: lol_open_object
 *
 * Description: Get a handle to the previously created LOL object.
 *
 * Inputs: 
 *   ui2_lol_id:     specify the LOL id to open an handle to.
 *   ps_lol_name:    specify the LOL name to open an handle to.
 *   pv_tag:         tag value 
 *   pf_nfy_fct:     notification function to be registered for callback.
 *
 * Outputs: 
 *   ph_lol:         handle to the LOL object.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_open_object
(
    UINT16		    ui2_lol_id,
    const CHAR*     ps_lol_name,
    VOID*		    pv_tag,
    x_lol_nfy_fct   pf_nfy_fct,
    HANDLE_T*		ph_lol
)
{
    CHAR            ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    LOL_OBJ_T*      pt_lol=NULL;
    INT32           i4_rc=LOLR_FAIL;
    HANDLE_T        h_cdb=NULL_HANDLE;
    
    /* create CDB name. */
    lol_get_database_name(ps_lol_name,ui2_lol_id,ps_name);

    /* Create a LOL control block object. */
    pt_lol=(LOL_OBJ_T*) x_mem_alloc(sizeof(LOL_OBJ_T));
    
    if ( pt_lol != NULL )
    {
        /* Get a handle to the core database object.
           Note: the private tag field is used to
                 store the address of the LOL control block
                 structure.
        */
        i4_rc=db_get_handle_by_name(ps_name,
                                    (VOID*)pt_lol,
                                    lol_nfy_fct, /* Internal LOL-CDB
                                                    notification function.
                                                 */
                                    &h_cdb);
        
        if ( i4_rc == DBR_OK)
        {
            /*
              Allocate a handle for the LOL object.
            */
            if ( handle_alloc(LOLT_LARGE_OBJECT_LIST,
                              (VOID*)pt_lol,
                              pv_tag,
                              lol_handle_free_fct,
                              ph_lol)
                 == HR_OK )
            {
                /* initialize the LOL object. */
                pt_lol->h_lol=*ph_lol;
                pt_lol->h_cdb=h_cdb;
                pt_lol->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                  caller's notification
                                                  function.
                                               */
                pt_lol->ui2_lol_id=ui2_lol_id;
                x_strncpy(pt_lol->ps_lol_name,ps_lol_name,MAX_LOL_NAME_LEN-1);
                pt_lol->ps_lol_name[MAX_LOL_NAME_LEN-1]='\0';
                i4_rc=LOLR_OK;
            }
            else
            {
                handle_free(h_cdb, TRUE);
                x_mem_free(pt_lol);
                i4_rc=LOLR_OUT_OF_HANDLE;
            }
        }
        else
        {
            x_mem_free(pt_lol);
            /* translate the CDB error code to LOL error code */
            i4_rc=lol_cdb_to_lol_err_code(i4_rc);
        }
    }
    else
    {
        i4_rc = LOLR_OUT_OF_MEM ;
    }
    
    return i4_rc;
}


INT32 lol_read_common_data_from_object
(
    LOL_OBJ_T*        pt_lol,
    VOID*             ps_common,
    INT32             i4_common_size
)
{
    INT32      i4_rc;

    i4_rc=db_read_common(pt_lol->h_cdb,ps_common,(UINT16)i4_common_size);
    /* translate the CDB error code to LOL error code */
    i4_rc=lol_cdb_to_lol_err_code(i4_rc);
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_rec_from_obl
 *
 * Description: Retreive a record from LOL object.
 *
 * Inputs: 
 *   pt_lol:         specify the LOL object to read from.
 *   ui2_lol_rec_id: specify the record id of the record to read.
 *
 * Outputs: 
 *   pt_lol_rec:     pointer to the LOL record structure. On return,
 *                   this structure contains the data read from the
 *                   LOL record.
 *   pui4_ver_id     contains the current version value of LOL data object.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_get_rec_from_obj
(
    const LOL_OBJ_T*  	pt_lol,
    UINT16			    ui2_lol_rec_id,
    LOL_REC_T*			pt_lol_rec,
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
    VOID*               apv_recfld_addr[LOL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    UINT16              aui2_addr_len[LOL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;
    
    /* Set up the search parameter and address of the key value
       for the search.
    */
    ui1_key_count = 1;
    aui4_keyfld_type[0] = (DB_FIELD_T) LOL_CDB_TYPE_REC_ID ;
    apv_keyfld_addr[0] = &ui2_lol_rec_id;

    /* Specifies the fields to retrieve the record data. For
       LOL record, we extract all 10 fields of the data.
    */
    lol_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* We need to specify the ui2_data_len before setting up the addresses
     * because value of ui2_data_len will be saved to aui2_addr_len
     *                                      added by dyin on 2009/06/26
     */
    pt_lol_rec->ui2_data_len = (UINT16) sizeof(pt_lol_rec->aui1_data);

    /* Set up the addresses to copy the record data to. */
    lol_rec_fld_to_addr(pt_lol_rec, (const VOID**)apv_recfld_addr,
                        aui2_addr_len, 0, &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_rec_v2(pt_lol->h_cdb,
                           ui1_key_count,
                           aui4_keyfld_type,
                           apv_keyfld_addr,
                           (DB_SEARCH_T*)NULL,
                           ui1_fld_count,
                           pui4_recfld_type,
                           apv_recfld_addr,
                           aui2_addr_len,
                           0,
                           &ui4_db_version);
    /* dyin: we need to set ui2_data_len to the memory object length
     *       the memory buffer field must be the last one in definition
     */
    if (i4_rc == DBR_REC_NOT_FOUND)
    {
        pt_lol_rec->ui2_data_len = 0;
    }
    else
    {
        if (ui1_fld_count != LOL_NUM_OF_REC_FIELDS)
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_INVALID_RECORD_COUNT);
        }
        pt_lol_rec->ui2_data_len = aui2_addr_len[LOL_NUM_OF_REC_FIELDS-1];
    }

    if ( i4_rc == DBR_OK || i4_rc == DBR_REC_NOT_FOUND )
    {
        *pui4_ver_id = ui4_db_version;
    }
    /* translate the CDB error code to LOL error code */
    i4_rc=lol_cdb_to_lol_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_sort_rec_by_rec_id
 *
 * Description: Sort the LOL object by network id values.
 *
 * Inputs: 
 *   pt_lol:           Handle to lol.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of LOL list.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_sort_rec_by_rec_id
(
    const LOL_OBJ_T*  	pt_lol,
    UINT32*			    pui4_ver_id
)
{
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    INT32               i4_rc;
    
    /* specifies the sort key. */
    ui1_key_count = 1;
    aui4_keyfld_type[0]=(DB_FIELD_T)LOL_CDB_TYPE_REC_ID;
    /*
      convert LOL NULL version id to CDB null version id
    */
    if ( *pui4_ver_id == LOL_NULL_VER_ID )
    {
        *pui4_ver_id = CDB_NULL_VER_ID;
    }
    
    i4_rc=db_sort_rec(pt_lol->h_cdb,
                      ui1_key_count,
                      aui4_keyfld_type,
                      NULL,
                      TRUE,
                      pui4_ver_id);
    /* translate the CDB error code to LOL error code */
    return lol_cdb_to_lol_err_code(i4_rc);
}


INT32 lol_get_num_rec
(
    const LOL_OBJ_T*  	pt_lol,
    UINT16*             pui2_num_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    DB_INFO_T           t_db_info;

    /*
      Retrieve the database info from the CoreDB database.
    */
    i4_rc = db_get_info(pt_lol->h_cdb, &t_db_info);
    if ( i4_rc == DBR_OK )
    {
        *pui2_num_rec = (UINT16) t_db_info.ui4_record_count;
        *pui4_ver_id = t_db_info.ui4_db_version_id;
    }
    /* translate the CDB error code to LOL error code */
    return lol_cdb_to_lol_err_code(i4_rc);
}



/*-----------------------------------------------------------------------------
 * Name: lol_get_num_rec_by_flds
 *
 * Description: get the number of record for a specified field value.
 *
 * Inputs: 
 *   pt_lol:           Handle to lol.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of LOL list.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_get_num_rec_by_flds
(
    const LOL_OBJ_T*  	  pt_lol,
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
    i4_rc = db_get_num_of_rec(pt_lol->h_cdb,
                              ui1_key_count,
                              aui4_keyfld_type,
                              apv_keyfld_addr,
                              ae_cmp_param,
                              pui2_num_recs,
                              pui4_ver_id);
    
    /* translate the CDB error code to LOL error code */
    i4_rc=lol_cdb_to_lol_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_rec_by_flds
 *
 * Description: Get the field data from specified key field value.
 *
 * Inputs: 
 *   pt_lol:           pointer to LOL control block object.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of LOL list.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_get_rec_by_flds
(
    const LOL_OBJ_T*  	  pt_lol,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    LOL_REC_T*			  pt_lol_rec,
    UINT16			      ui2_idx,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[LOL_NUM_OF_REC_FIELDS];
    UINT16              aui2_addr_len[LOL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;
    
    /* Specifies the fields to retrieve the record data. For
       LOL record, we extract all fields of the data.
    */
    lol_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* We need to specify the ui2_data_len before setting up the addresses
     * because value of ui2_data_len will be saved to aui2_addr_len
     *                                      added by dyin on 2009/06/26
     */
    pt_lol_rec->ui2_data_len = (UINT16) sizeof(pt_lol_rec->aui1_data);

    /* Set up the addresses to copy the record data to. */
    lol_rec_fld_to_addr(pt_lol_rec,
                        (const VOID**)apv_recfld_addr,
                        aui2_addr_len,
                        0,
                        &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    i4_rc = db_read_rec_v2(pt_lol->h_cdb,
                           ui1_key_count,
                           aui4_keyfld_type,
                           apv_keyfld_addr,
                           ae_cmp_param,
                           ui1_fld_count,
                           pui4_recfld_type,
                           apv_recfld_addr,
                           aui2_addr_len,
                           ui2_idx,
                           pui4_ver_id);
    
    /* dyin: we need to set ui2_data_len to the memory object length
     *       the memory buffer field must be the last one in definition
     */
    if (i4_rc == DBR_REC_NOT_FOUND)
    {
        pt_lol_rec->ui2_data_len = 0;
    }
    else
    {
        if (ui1_fld_count != LOL_NUM_OF_REC_FIELDS)
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_INVALID_RECORD_COUNT);
        }
        pt_lol_rec->ui2_data_len = aui2_addr_len[LOL_NUM_OF_REC_FIELDS-1];
    }

    /* translate the CDB error code to LOL error code */
    i4_rc=lol_cdb_to_lol_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_read_rec_at_cursor_pos
 *
 * Description: Get the field data at the specified position in
 *              the sorted list.
 *
 * Inputs: 
 *   pt_lol:           pointer to LOL control block object.
 *   
 * Outputs:
 *   pt_lol_rec        ref a LOL record structure.
 *   pui4_ver_id       The current version id of LOL list.
 *
 * Returns: 
 *   LOLR_OK           LOL handle is returned.
 *   LOLR_OUT_OF_MEM   Not enough memory to create an LOL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 lol_read_rec_at_cursor_pos
(
    const LOL_OBJ_T*  	pt_lol,
    LOL_REC_T*			pt_lol_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;

    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[LOL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    UINT16              aui2_addr_len[LOL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;

    /* Specifies the fields to retrieve the record data. For
       LOL record, we extract all 4 fields of the data.
    */
    lol_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* We need to specify the ui2_data_len before setting up the addresses
     * because value of ui2_data_len will be saved to aui2_addr_len
     *                                      added by dyin on 2009/06/26
     */
    pt_lol_rec->ui2_data_len = (UINT16) sizeof(pt_lol_rec->aui1_data);

    /* Set up the addresses to copy the record data to. */
    lol_rec_fld_to_addr(pt_lol_rec,
                        (const VOID**)apv_recfld_addr,
                        aui2_addr_len,
                        0,
                        &i4_union_tag);
    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_sorted_rec_v2(pt_lol->h_cdb,
                                  ui1_fld_count,
                                  pui4_recfld_type,
                                  apv_recfld_addr,
                                  aui2_addr_len,
                                  &ui4_db_version);
    
    /* dyin: we need to set ui2_data_len to the memory object length
     *       the memory buffer field must be the last one in definition
     */
    if (i4_rc == DBR_REC_NOT_FOUND)
    {
        pt_lol_rec->ui2_data_len = 0;
    }
    else
    {
        if (ui1_fld_count != LOL_NUM_OF_REC_FIELDS)
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_INVALID_RECORD_COUNT);
        }
        pt_lol_rec->ui2_data_len = aui2_addr_len[LOL_NUM_OF_REC_FIELDS-1];
    }

    if ( i4_rc == DBR_OK || i4_rc == DBR_REC_NOT_FOUND )
    {
        *pui4_ver_id = ui4_db_version;
    }
    /* translate the CDB error code to LOL error code */
    i4_rc=lol_cdb_to_lol_err_code(i4_rc);
    
    return i4_rc;
}



