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
 * $RCSfile: nwl_read_api.c,v $
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

#include "nwl/_nwl.h"
#include "nwl/nwl_rec.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: nwl_open_object
 *
 * Description: Get a handle to the previously created NWL object.
 *
 * Inputs: 
 *   ui2_nwl_id:     specify the NWL id to open an handle to.
 *   ps_nwl_name:    specify the NWL name to open an handle to.
 *   pv_tag:         tag value 
 *   pf_nfy_fct:     notification function to be registered for callback.
 *
 * Outputs: 
 *   ph_nwl:         handle to the NWL object.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_open_object
(
    UINT16		    ui2_nwl_id,
    const CHAR*     ps_nwl_name,
    VOID*		    pv_tag,
    x_nwl_nfy_fct   pf_nfy_fct,
    HANDLE_T*		ph_nwl
)
{
    CHAR            ps_name[CDB_MAX_DATABASE_NAME_LEN+1];
    NWL_OBJ_T*      pt_nwl=NULL;
    INT32           i4_rc=NWLR_FAIL;
    HANDLE_T        h_cdb=NULL_HANDLE;
    
    /* create CDB name. */
    nwl_get_database_name(ps_nwl_name,ui2_nwl_id,ps_name);

    /* Create a NWL control block object. */
    pt_nwl=(NWL_OBJ_T*) x_mem_alloc(sizeof(NWL_OBJ_T));
    
    if ( pt_nwl != NULL )
    {
        /* Get a handle to the core database object.
           Note: the private tag field is used to
                 store the address of the NWL control block
                 structure.
        */
        i4_rc=db_get_handle_by_name(ps_name,
                                    (VOID*)pt_nwl,
                                    nwl_nfy_fct, /* Internal NWL-CDB
                                                    notification function.
                                                 */
                                    &h_cdb);
        
        if ( i4_rc == DBR_OK)
        {
            /*
              Allocate a handle for the NWL object.
            */
            if ( handle_alloc(NWLT_NETWORK_LIST,
                              (VOID*)pt_nwl,
                              pv_tag,
                              nwl_handle_free_fct,
                              ph_nwl)
                 == HR_OK )
            {
                /* initialize the NWL object. */
                pt_nwl->h_nwl=*ph_nwl;
                pt_nwl->h_cdb=h_cdb;
                pt_nwl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                  caller's notification
                                                  function.
                                               */
                pt_nwl->ui2_nwl_id=ui2_nwl_id;
                x_strncpy(pt_nwl->ps_nwl_name,ps_nwl_name,MAX_NWL_NAME_LEN-1);
                pt_nwl->ps_nwl_name[MAX_NWL_NAME_LEN-1]='\0';
                i4_rc=NWLR_OK;
            }
            else
            {
                handle_free(h_cdb, TRUE);
                x_mem_free(pt_nwl);
                i4_rc=NWLR_OUT_OF_HANDLE;
            }
        }
        else
        {
            x_mem_free(pt_nwl);
            /* translate the CDB error code to NWL error code */
            i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
        }
    }
    else
    {
        i4_rc = NWLR_OUT_OF_MEM ;
    }
    
    return i4_rc;
}


INT32 nwl_read_common_data_from_object
(
    NWL_OBJ_T*        pt_nwl,
    VOID*             ps_common,
    INT32             i4_common_size
)
{
    INT32      i4_rc;

    i4_rc=db_read_common(pt_nwl->h_cdb,ps_common,(UINT16)i4_common_size);
    /* translate the CDB error code to NWL error code */
    i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: nwl_get_rec_from_obl
 *
 * Description: Retreive a record from NWL object.
 *
 * Inputs: 
 *   pt_nwl:         specify the NWL object to read from.
 *   ui2_nwl_rec_id: specify the record id of the record to read.
 *
 * Outputs: 
 *   pt_nwl_rec:     pointer to the NWL record structure. On return,
 *                   this structure contains the data read from the
 *                   NWL record.
 *   pui4_ver_id     contains the current version value of NWL data object.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_get_rec_from_obj
(
    const NWL_OBJ_T*  	pt_nwl,
    UINT16			    ui2_nwl_rec_id,
    NWL_REC_T*			pt_nwl_rec,
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
    VOID*               apv_recfld_addr[NWL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    UINT16              aui2_addr_len[NWL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;
    
    /* Set up the search parameter and address of the key value
       for the search.
    */
    ui1_key_count = 1;
    aui4_keyfld_type[0] = (DB_FIELD_T) NWL_CDB_TYPE_REC_ID ;
    apv_keyfld_addr[0] = &ui2_nwl_rec_id;

    /* Specifies the fields to retrieve the record data. For
       NWL record, we extract all 10 fields of the data.
    */
    nwl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to copy the record data to. */
    nwl_rec_fld_to_addr(pt_nwl_rec, (const VOID**)apv_recfld_addr,
                        aui2_addr_len, 0, &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_rec(pt_nwl->h_cdb,
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
    /* translate the CDB error code to NWL error code */
    i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: nwl_sort_rec_by_network_id
 *
 * Description: Sort the NWL object by network id values.
 *
 * Inputs: 
 *   pt_nwl:           Handle to nwl.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of NWL list.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_sort_rec_by_network_id
(
    const NWL_OBJ_T*  	pt_nwl,
    UINT32*			    pui4_ver_id
)
{
    UINT8               ui1_key_count;
    DB_FIELD_T          aui4_keyfld_type[1];
    INT32               i4_rc;
    
    /* specifies the sort key. */
    ui1_key_count = 1;
    aui4_keyfld_type[0]=(DB_FIELD_T)NWL_CDB_TYPE_NETWORK_ID;
    /*
      convert NWL NULL version id to CDB null version id
    */
    if ( *pui4_ver_id == NWL_NULL_VER_ID )
    {
        *pui4_ver_id = CDB_NULL_VER_ID;
    }
    
    i4_rc=db_sort_rec(pt_nwl->h_cdb,
                      ui1_key_count,
                      aui4_keyfld_type,
                      NULL,
                      TRUE,
                      pui4_ver_id);
    /* translate the CDB error code to NWL error code */
    return nwl_cdb_to_nwl_err_code(i4_rc);
}


INT32 nwl_get_num_rec
(
    const NWL_OBJ_T*  	pt_nwl,
    UINT16*             pui2_num_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    DB_INFO_T           t_db_info;

    /*
      Retrieve the database info from the CoreDB database.
    */
    i4_rc = db_get_info(pt_nwl->h_cdb, &t_db_info);
    if ( i4_rc == DBR_OK )
    {
        *pui2_num_rec = (UINT16) t_db_info.ui4_record_count;
        *pui4_ver_id = t_db_info.ui4_db_version_id;
    }
    /* translate the CDB error code to NWL error code */
    return nwl_cdb_to_nwl_err_code(i4_rc);
}



/*-----------------------------------------------------------------------------
 * Name: nwl_get_num_rec_by_flds
 *
 * Description: get the number of record for a specified field value.
 *
 * Inputs: 
 *   pt_nwl:           Handle to nwl.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of NWL list.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_get_num_rec_by_flds
(
    const NWL_OBJ_T*  	  pt_nwl,
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
    i4_rc = db_get_num_of_rec(pt_nwl->h_cdb,
                              ui1_key_count,
                              aui4_keyfld_type,
                              apv_keyfld_addr,
                              ae_cmp_param,
                              pui2_num_recs,
                              pui4_ver_id);
    
    /* translate the CDB error code to NWL error code */
    i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: nwl_get_rec_by_flds
 *
 * Description: Get the field data from specified key field value.
 *
 * Inputs: 
 *   pt_nwl:           pointer to NWL control block object.
 *   
 * Outputs:
 *   pui4_ver_id       The current version id of NWL list.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_get_rec_by_flds
(
    const NWL_OBJ_T*  	  pt_nwl,
    UINT8                 ui1_key_count,
    const DB_FIELD_T      aui4_keyfld_type[],
    const VOID*           apv_keyfld_addr[],
    const DB_SEARCH_T  	  ae_cmp_param[],
    NWL_REC_T*			  pt_nwl_rec,
    UINT16			      ui2_idx,
    UINT32*			      pui4_ver_id 
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;
    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[NWL_NUM_OF_REC_FIELDS];
    UINT16              aui2_addr_len[NWL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;
    
    /* Specifies the fields to retrieve the record data. For
       NWL record, we extract all fields of the data.
    */
    nwl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to copy the record data to. */
    nwl_rec_fld_to_addr(pt_nwl_rec,
                        (const VOID**)apv_recfld_addr,
                        aui2_addr_len,
                        0,
                        &i4_union_tag);

    /*
      Retrieve the record from the CoreDB database.
    */
    i4_rc = db_read_rec(pt_nwl->h_cdb,
                        ui1_key_count,
                        aui4_keyfld_type,
                        apv_keyfld_addr,
                        ae_cmp_param,
                        ui1_fld_count,
                        pui4_recfld_type,
                        apv_recfld_addr,
                        ui2_idx,
                        pui4_ver_id);
    
    /* translate the CDB error code to NWL error code */
    i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: nwl_read_rec_at_cursor_pos
 *
 * Description: Get the field data at the specified position in
 *              the sorted list.
 *
 * Inputs: 
 *   pt_nwl:           pointer to NWL control block object.
 *   
 * Outputs:
 *   pt_nwl_rec        ref a NWL record structure.
 *   pui4_ver_id       The current version id of NWL list.
 *
 * Returns: 
 *   NWLR_OK           NWL handle is returned.
 *   NWLR_OUT_OF_MEM   Not enough memory to create an NWL handle.
 *   
 *
 ----------------------------------------------------------------------------*/
INT32 nwl_read_rec_at_cursor_pos
(
    const NWL_OBJ_T*  	pt_nwl,
    NWL_REC_T*			pt_nwl_rec,
    UINT32*			    pui4_ver_id
)
{
    INT32               i4_rc;
    UINT8               ui1_fld_count;

    DB_FIELD_INFO_T*    pt_rec_fields;
    DB_FIELD_T*         pui4_recfld_type;
    VOID*               apv_recfld_addr[NWL_NUM_OF_REC_FIELDS];
    UINT32              ui4_db_version;
    UINT16              aui2_addr_len[NWL_NUM_OF_REC_FIELDS];
    INT32               i4_union_tag;

    /* Specifies the fields to retrieve the record data. For
       NWL record, we extract all 4 fields of the data.
    */
    nwl_get_rec_def(&ui1_fld_count,&pt_rec_fields,&pui4_recfld_type);

    /* Set up the addresses to copy the record data to. */
    nwl_rec_fld_to_addr(pt_nwl_rec,
                        (const VOID**)apv_recfld_addr,
                        aui2_addr_len,
                        0,
                        &i4_union_tag);
    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_sorted_rec(pt_nwl->h_cdb,
                               ui1_fld_count,
                               pui4_recfld_type,
                               apv_recfld_addr,
                               &ui4_db_version);
    
    if ( i4_rc == DBR_OK || i4_rc == DBR_REC_NOT_FOUND )
    {
        *pui4_ver_id = ui4_db_version;
    }
    /* translate the CDB error code to NWL error code */
    i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
    
    return i4_rc;
}



