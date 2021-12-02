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
 * $RCSfile: scdb_lib.c,v $
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
#include "cdb/cdb_api.h"
#include "scdb/_scdb.h"


/*-----------------------------------------------------------------------------
  data declaraions
  ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: scdb_cmn_struct_to_mem
 *
 * Description: API to copy the common data structure to a memory buffer.
 *
 * Inputs:  - pt_common:     pointer to the common structure.
 *            
 *            ui4_size_buf:  size of the buffer.
 *
 * Outputs: - pui1_buffer:   pointer to the memory buffer.
 *
 * Returns: - SCDBR_OK
 *            
 *            
 ----------------------------------------------------------------------------*/

/* Macro to get the size and offset of a member field in a C-structure. */
#define GET_OFFSET_AND_SIZE(_type_,_struct_obj_,_memb_,_offset_,_size_of_field_)  \
do { _offset_ = offsetof(_type_,_memb_) ;  _size_of_field_ = sizeof(_struct_obj_->_memb_); } while (0)

INT32 scdb_cmn_struct_to_mem
(
    const SCDB_COMMON_T*          pt_common,        /* source */
    UINT8*                        pui1_buffer,      /* destination */
    SIZE_T                        t_size_buf
)
{
    SIZE_T       t_size;
    
    /* copy the structure to a memory buffer. */
    t_size = sizeof(SCDB_COMMON_T);

    if ( pt_common     !=  NULL               &&
         pui1_buffer   !=  NULL               &&
         t_size_buf    >=  t_size )
    {
        x_memcpy(pui1_buffer,pt_common, t_size_buf );
    }
    else
    {
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_ARRAY_OUT_OF_BOUND);
    }
    
    return SCDBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_mem_2_cmn_struct
 *
 * Description: API to create common data structure from memory buffer.
 *
 * Inputs:  - pt_common:     pointer to the common structure.
 *            pui1_buffer:   pointer to the memory buffer.
 *            ui4_size_buf:  size of the buffer.
 *
 * Outputs: - None.
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_mem_2_cmn_struct
(
    UINT8*                  pui1_buffer,            /* source */
    SCDB_COMMON_T*          pt_common,              /* destination */
    SIZE_T                  t_size_buf            /* size of source buffer. */
)
{
    SIZE_T       t_size;

    /* copy the memory buffer content to common data structure. */
    t_size = sizeof(SCDB_COMMON_T);

    if ( pt_common      != NULL       &&
         pui1_buffer    != NULL       &&
         t_size         >= t_size_buf )
    {
        x_memcpy(pt_common, pui1_buffer, t_size_buf );
    }
    else
    {
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_ARRAY_OUT_OF_BOUND);
    }
    return SCDBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_data_struct_to_addr
 *
 * Description: API to convert the fields in SCDB union structure into
 *              an array of addresses.  
 *
 * Inputs:  - pt_stream_desc:  key field (stream) for the record.
 *            pt_scdb_rec:  pointer to the SCDB record.
 *
 * Outputs: - apv_key_field_values: address(s) of the key field.
 *            apv_field_values:     addresses(s) of record fields. 
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_data_struct_to_addr
(
    const STREAM_COMP_ID_T*   pt_stream_desc,
    const SCDB_REC_T*         pt_scdb_rec,
    VOID*                     apv_key_field_values[],
    VOID*                     apv_field_values[]
)
{
    /* Make copy the address of key fields. */
    apv_key_field_values[0]=(STREAM_TYPE_T*)&(pt_stream_desc->e_type);

    apv_key_field_values[1]=(STREAM_TYPE_T*)&(pt_stream_desc->pv_stream_tag);    

    /*
      Build the list of addresses for the input record fields.
      Note: the order of the input record fields must be in the
      same order as the specified in the record definition.

      <stream_type> <stream_tag> <stream_data>
    */
    
    /* Field 0 of the record is the stream type. */
    apv_field_values[0]=(STREAM_TYPE_T*)&(pt_stream_desc->e_type);

    /* Field 1 of the record is the stream tag. */
    apv_field_values[1]=(VOID*)&(pt_stream_desc->pv_stream_tag);


    /* Field 2 of the record is the stream data. */
    apv_field_values[2]=(SCDB_REC_T*)(pt_scdb_rec);

    return SCDBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_rec_def
 *
 * Description: API to return SCDB record definition and the key field array
 *              for the record definition.
 *
 * Inputs:  - None.
 *
 * Outputs: - ppt_rec_field: pointer to an array of DB_FIELD_INFO_T structure
 *                           that describes the record.
 *            pi4_fld_count: pointer to variable holding the count of field in
 *                           the SCDB record.
 *            ppi4_key_fields: pointer to an array of DB_FIELD_T that describes
 *                             the key make-up. 
 *            pi4_key_count: pointer to variable holding the count of key
 *                           fields.
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

/*
   This Record field descrption array and Record key description array

   SCDB record consists of 3 fields:

   <stream_type> <stream_tag> <stream_data>

   The stream_data field is an unsigned byt byte buffer, it stores
   (currently) either SCDB_AUDIO_MPEG_T or SCDB_VIDEO_MPEG_T
   structure.

   For SCDB_AUDIO_MPEG_T structure, it contains the following
   fields:
   
   <pid> <language_description> <data_description> <bool flag>

   For SCDB_VIDEO_MPEG_T structure, it contains

   <pid> <data_description> <bool flag>   

*/
#define SCDB_NUM_OF_REC_FIELDS         3
#define SCDB_NUM_OF_KEY_FIELDS         2

static DB_FIELD_INFO_T         at_rec_fields[SCDB_NUM_OF_REC_FIELDS];
static DB_FIELD_T        aui4_db_recfld_type[SCDB_NUM_OF_REC_FIELDS];   
static DB_FIELD_T        aui4_db_stream_type[SCDB_NUM_OF_KEY_FIELDS];


INT32 scdb_rec_def
(
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type,
    UINT8*             pui1_fld_count,
    DB_FIELD_T**       ppui4_keyfld_type,
    UINT8*             pui1_key_count
)
{
    /* create key field type definition. */
    aui4_db_stream_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE;
    aui4_db_stream_type[1]=(DB_FIELD_T)CDB_TYPE_STREAM_TAG;
    
    /*
      Create a record definition:
      
       <stream_type> <stream_tag> <stream_data>

       <stream_type>:  DBF_ENUMXX_T,  count= 1.

       <stream_tag>:   DBF_UINT32_T,  count= 1

       <stream_data>:  DBF_UINT8_T,   count= sizeof(SCDB_REC_T)
    */
    
    /* Stream type and count (size). */
    at_rec_fields[0].ui4_field_type = aui4_db_stream_type[0];
    aui4_db_recfld_type[0] = at_rec_fields[0].ui4_field_type;
    at_rec_fields[0].ui2_field_element_count = 1 ;
    
    /* Stream Tag type and count (size) */
    at_rec_fields[1].ui4_field_type = aui4_db_stream_type[1];
    aui4_db_recfld_type[1] = at_rec_fields[1].ui4_field_type;
    at_rec_fields[1].ui2_field_element_count = 1;

    /* Stream data type and count (size)  */
    at_rec_fields[2].ui4_field_type = (DB_FIELD_T)CDB_TYPE_STREAM_DATA ;
    aui4_db_recfld_type[2] = at_rec_fields[2].ui4_field_type;
    at_rec_fields[2].ui2_field_element_count = (UINT16)sizeof(SCDB_REC_T);

    /* Assigned the rec definition array, key field array, and count values
       to the output variables.
    */
    *ppt_rec_fields=at_rec_fields;
    *ppui4_recfld_type=aui4_db_recfld_type;
    *pui1_fld_count=SCDB_NUM_OF_REC_FIELDS;
    *ppui4_keyfld_type=aui4_db_stream_type;
    *pui1_key_count=SCDB_NUM_OF_KEY_FIELDS;

    return SCDBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_write_scdb_rec
 *
 * Description: API to write a rec (append or update mode )
 *              to the SCDB database.
 *
 * Inputs:  - h_cdb:  handle to the CDB database object.
 *            pt_stream_desc: pointer to the stream descriptor
 *            pt_scdb_rec:    pointer to the scdb record
 *
 * Outputs: - None.
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_write_scdb_rec
(
    HANDLE_T                  h_cdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,   /* Array of const pointers */
    const SCDB_REC_T*         pt_scdb_rec,
    DB_CTRL_FLAG_T            e_option
)
{
    INT32            i4_rc;
    VOID*            apv_key_field_values[SCDB_NUM_OF_KEY_FIELDS];
    VOID*            apv_field_values[SCDB_NUM_OF_REC_FIELDS];
    /*
       Convert SCDB record struct to array of addresss.
    */
    if ( scdb_data_struct_to_addr(pt_stream_desc,
                                  pt_scdb_rec,
                                  apv_key_field_values,
                                  apv_field_values)
         == SCDBR_OK
        )  
    {
        i4_rc = db_write_rec(h_cdb,
                             (const VOID**)apv_key_field_values,
                             (const VOID**)apv_field_values,
                             e_option);
        
        /* translate the CDB error code to SCDB error code */
        i4_rc=scdb_error_code(i4_rc);
    }
    else
    {
        i4_rc=SCDBR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_read_cdb_rec
 *
 * Description: API to read a rec (append or update mode )
 *              from the SCDB database.
 *
 * Inputs:  - h_cdb:  handle to the CDB database object.
 *            pt_stream_desc: pointer to the stream descriptor
 *            ui2_rec_index:  iteration index
 *
 * Outputs: - pt_scdb_rec:    pointer to the scdb record
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_read_cdb_rec
(
    HANDLE_T                  h_cdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,   
    SCDB_REC_T*               pt_scdb_rec,
    UINT16                    ui2_rec_index
)
{
    INT32               i4_rc;
    /* Key fields and record info. */
    DB_FIELD_T          ui4_recfld_type[SCDB_NUM_OF_REC_FIELDS];
    UINT8               ui1_fld_count;
    DB_FIELD_T          ui4_keyfld_type[SCDB_NUM_OF_KEY_FIELDS];
    UINT8               ui1_key_count;
    
    UINT32              ui4_db_version;    
    const VOID*         apv_key_field_values[SCDB_NUM_OF_KEY_FIELDS];
    VOID*               apv_field_values[SCDB_NUM_OF_REC_FIELDS];
    
    /*
      Specify the searching parameters: <stream_type> and <stream_tag>
    */
    apv_key_field_values[0]=(STREAM_TYPE_T*) (&(pt_stream_desc->e_type));
    apv_key_field_values[1]=(VOID*) (&(pt_stream_desc->pv_stream_tag));

    ui4_keyfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE;
    ui4_keyfld_type[1]=(DB_FIELD_T)CDB_TYPE_STREAM_TAG;
            
    ui1_key_count=2;
    
    /*
       Convert SCDB record struct to an addresss.  We are only
       extracting 1 field <stream_data> from the SCDB record.
    */
    apv_field_values[0]=(SCDB_REC_T*)(pt_scdb_rec);
    ui4_recfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_DATA ;
    ui1_fld_count=1;
        
    /*
      Retrieve the record from the CoreDB database.
    */
    ui4_db_version=CDB_NULL_VER_ID;
    i4_rc = db_read_rec(h_cdb,
                        ui1_key_count,
                        ui4_keyfld_type,
                        apv_key_field_values,
                        (DB_SEARCH_T*) NULL,
                        ui1_fld_count,
                        ui4_recfld_type,
                        apv_field_values,
                        ui2_rec_index,
                        &ui4_db_version);
        
    /* translate the CDB error code to SCDB error code */
    i4_rc=scdb_error_code(i4_rc);
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: scdb_read_next_cdb_rec
 *
 * Description: API to read next rec from the CDB database.
 *
 * Inputs:  - h_cdb:  handle to the CDB database object.
 *            pt_stream_desc: pointer to the stream descriptor
 *            ui2_rec_index:  iteration index
 *
 * Outputs: - pt_scdb_rec:    pointer to the scdb record
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_read_next_cdb_rec
(
    HANDLE_T                  h_cdb,
    STREAM_TYPE_T             e_stream_type,
    STREAM_COMP_ID_T*         pt_stream_desc,   
    SCDB_REC_T*               pt_scdb_rec,
    UINT16                    ui2_rec_index,
    UINT32*                   pui4_db_version
)
{
    INT32               i4_rc;
    /* Key fields and record info. */
    DB_FIELD_T          ui4_recfld_type[SCDB_NUM_OF_REC_FIELDS];
    UINT8               ui1_fld_count;
    DB_FIELD_T          ui4_keyfld_type[SCDB_NUM_OF_KEY_FIELDS];
    UINT8               ui1_key_count;
    
    const VOID*         apv_key_field_values[SCDB_NUM_OF_KEY_FIELDS];
    VOID*               apv_field_values[SCDB_NUM_OF_REC_FIELDS];
    
    /*
      Specify the searching parameters: <stream_type>
    */
    apv_key_field_values[0]=(STREAM_TYPE_T*) (&e_stream_type);
    ui4_keyfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE;
    ui1_key_count=1;
    
    /*
       Convert SCDB record struct to an addresss.  We need to
       extract 3 field: <stream_type> <stream_tag> <stream_data>
       from the SCDB record.
    */
    apv_field_values[0]=(STREAM_TYPE_T*)(&(pt_stream_desc->e_type));
    ui4_recfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE ;
    
    apv_field_values[1]=(VOID*)(&(pt_stream_desc->pv_stream_tag));
    ui4_recfld_type[1]=(DB_FIELD_T)CDB_TYPE_STREAM_TAG ;    

    apv_field_values[2]=(SCDB_REC_T*)(pt_scdb_rec);
    ui4_recfld_type[2]=(DB_FIELD_T)CDB_TYPE_STREAM_DATA ;

    ui1_fld_count=3;

    if ( e_stream_type == ST_UNKNOWN )
    {
        /* match any record. */
        ui1_key_count=0;
    }
    
    /*
      Retrieve the record from the CoreDB database.
    */        
    i4_rc = db_read_rec(h_cdb,
                        ui1_key_count,
                        ui4_keyfld_type,
                        apv_key_field_values,
                        (DB_SEARCH_T*) NULL,
                        ui1_fld_count,
                        ui4_recfld_type,
                        apv_field_values,
                        ui2_rec_index,
                        pui4_db_version);
        
    /* translate the CDB error code to SCDB error code */
    i4_rc=scdb_error_code(i4_rc);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_get_num_of_cdb_rec
 *
 * Description: API to get the number of records for a specified streamtype
 *              in the SCDB database. If specified streamtype is 'ST_UNKNOWN',
 *              then all records in the SCDB are counted.
 *
 * Inputs:  - h_cdb:  handle to the CDB database object.
 *            e_stream_type: specify the stream type
 *
 * Outputs: - pui2_num_rec: pointer to a variable that holds the
 *                          number of records found.
 *            pui4_db_version: the version number of database.
 *
 * Returns: -    SCDBR_OK:   success
 *               SCDBR_INV_HANDLE:  bad CDB handle.
 ----------------------------------------------------------------------------*/

INT32 scdb_get_num_of_cdb_rec
(
    HANDLE_T            h_cdb,
    STREAM_TYPE_T       e_stream_type,
    UINT16*             pui2_num_recs,
    UINT32*             pui4_db_version
)
{
    INT32               i4_rc;
    /* Key info. */

    DB_FIELD_T          aui4_keyfld_type[SCDB_NUM_OF_KEY_FIELDS];
    UINT8               ui1_key_count;
    const VOID*         apv_key_field_values[SCDB_NUM_OF_KEY_FIELDS];
    DB_INFO_T           t_db_info ;
    
    if ( e_stream_type == ST_UNKNOWN )
    {
        /* 'ST_UNKNOWN' matches any record, so return all the
           records in the database.
        */
        i4_rc=db_get_info(h_cdb,&t_db_info);
        if ( i4_rc==DBR_OK )
        {
            *pui2_num_recs= (UINT16) t_db_info.ui4_record_count;
        }
        else
        {
            *pui2_num_recs=0;
        }
    }
    else
    {
        /*
           Specify the searching parameters: <stream_type>
        */
        apv_key_field_values[0]=(STREAM_TYPE_T*) (&e_stream_type);
        aui4_keyfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE;
        ui1_key_count=1;

        i4_rc=db_get_num_of_rec(h_cdb,
                                ui1_key_count,
                                aui4_keyfld_type,
                                apv_key_field_values,
                                (DB_SEARCH_T*) NULL,
                                pui2_num_recs,
                                pui4_db_version);
    }
        
    /* translate the CDB error code to SCDB error code */
    i4_rc=scdb_error_code(i4_rc);
    
    return i4_rc;
}

/*------------------------------------------------------------------
 * Name:  scdb_2_cdb_sort_cmp_fct_wrapper
 *
 * Description: Compare function registered with CDB database object.
 *              When this function is called by CDB internally to
 *              perform sorting operation, it will then invoke the
 *              SCDB sort compare function.
 *
 * Inputs:
 *   pv_elem_a  CDB record 1 for comparison.
 *
 *   pv_elem_b  CDB record 2 for comparison.
 *
 *   pv_data    Referece to the SCDB private structure containing
 *              the actual compare function.
 *
 * Outputs:
 *   None.
 *
 * Returns:
 *   SCDBR_OK         Version id is returned.
 *   SCDBR_INV_HANDLE Bad 'h_scdb' handle.
 *   SCDBR_INV_ARG    'pf_sort_cmp' or 'pf_search_cmp' is a NULL pointer.
 *
 *
--------------------------------------------------------------------*/
static INT32  scdb_2_cdb_sort_cmp_fct_wrapper
(
    const VOID*  pv_elem_a,
    const VOID*  pv_elem_b,
    VOID*        pv_data
)
{
    STREAM_COMP_ID_T  t_stream_desc_1;
    SCDB_REC_T        t_scdb_rec_1;
    STREAM_COMP_ID_T  t_stream_desc_2;
    SCDB_REC_T        t_scdb_rec_2;
    UINT8*            pui1_a;
    UINT8*            pui1_b;
    DB_FIELD_INFO_T*  pt_rec_fields;
    DB_FIELD_T*       pui4_recfld_type;
    UINT8             ui1_fld_count;
    UINT8             ui1_key_count;
    DB_FIELD_T*       pui4_keyfld_type;
    VOID*             apv_key_field_values[SCDB_NUM_OF_KEY_FIELDS];
    VOID*             apv_recfld_addr[SCDB_NUM_OF_REC_FIELDS];
    INT32             i4_rc;

    pui1_a = *( (UINT8**) pv_elem_a ) ;
    pui1_b = *( (UINT8**) pv_elem_b );

    /* Specifies the fields to retrieve the record data. For
       SCDB record, we extract all fields of the data.
    */
    scdb_rec_def(&pt_rec_fields,
                 &pui4_recfld_type,
                 &ui1_fld_count,
                 &pui4_keyfld_type,
                 &ui1_key_count);

    /*
      call the SCDB sort compare function.
    */
    if ( pv_data != 0 )
    {
        SCDB_CMP_FCT_INFO_T*    pt_cmp_fct_tag = NULL ;

        pt_cmp_fct_tag = (SCDB_CMP_FCT_INFO_T*) pv_data;

        /* Set up the addresses to copy the record data to. */
        scdb_data_struct_to_addr(&t_stream_desc_1,
                                 &t_scdb_rec_1,
                                 apv_key_field_values,
                                 apv_recfld_addr);
        /*
          convert the byte data to SCDB record.
        */
        db_parse_rec_data(pt_cmp_fct_tag->pt_cdb_database,
                          (UINT8*) pui1_a,
                          ui1_fld_count,
                          pui4_recfld_type,
                          apv_recfld_addr);

        /* Set up the addresses to copy the second record data. */
        scdb_data_struct_to_addr(&t_stream_desc_2,
                                 &t_scdb_rec_2,
                                 apv_key_field_values,
                                 apv_recfld_addr);
        /*
          convert the byte data to SCDB record.
        */
        db_parse_rec_data(pt_cmp_fct_tag->pt_cdb_database,
                          (UINT8*) pui1_b,
                          ui1_fld_count,
                          pui4_recfld_type,
                          apv_recfld_addr);

        i4_rc = pt_cmp_fct_tag->pf_sort_cmp(&t_scdb_rec_1, &t_scdb_rec_2,
                                            (pt_cmp_fct_tag->pv_tag));
    }
    else
    {
        i4_rc = 0;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_sort_cdb_rec
 *
 * Description: API to update sort list of the CDB database.
 *
 * Inputs:  - h_cdb:            handle to the CDB database object.
 *            pt_cmp_fct_info:  compare function info
 *            b_forced:         force sort or only when version change
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/

INT32 scdb_sort_cdb_rec
(
    HANDLE_T                  h_cdb,
    SCDB_CMP_FCT_INFO_T*      pt_cmp_fct_info,
    BOOL                      b_forced,
    UINT32*                   pui4_db_version
)
{
    INT32           i4_rc;

    i4_rc = db_sort_rec_by_client_fct(h_cdb,
                                      scdb_2_cdb_sort_cmp_fct_wrapper,
                                      pt_cmp_fct_info,
                                      TRUE,
                                      b_forced,
                                      pui4_db_version);
    if ( i4_rc == DBR_END_OF_REC )
    {
        i4_rc = SCDBR_REC_NOT_FOUND;
    }
    else
    {
        i4_rc = scdb_error_code(i4_rc);
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_read_sorted_cdb_rec
 *
 * Description: API to read sorted rec from the CDB database.
 *
 * Inputs:  - h_cdb:            handle to the CDB database object.
 *            pt_cmp_fct_info:  compare function info
 *            e_stream_type:    stream type to match
 *            ui2_rec_index:    iteration index
 *
 * Outputs: - pt_scdb_rec:      pointer to the scdb record
 *            pt_stream_desc:   pointer to the stream descriptor
 *            pui4_db_version:  pointer to the database version
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/

INT32 scdb_read_sorted_cdb_rec
(
    HANDLE_T                  h_cdb,
    SCDB_CMP_FCT_INFO_T*      pt_cmp_fct_info,
    STREAM_TYPE_T             e_stream_type,
    STREAM_COMP_ID_T*         pt_stream_desc,
    SCDB_REC_T*               pt_scdb_rec,
    UINT16                    ui2_rec_index,
    UINT32*                   pui4_db_version
)
{
    INT32               i4_rc;
    DB_FIELD_T          ui4_recfld_type[SCDB_NUM_OF_REC_FIELDS];
    UINT8               ui1_fld_count;
    VOID*               apv_field_values[SCDB_NUM_OF_REC_FIELDS];

    i4_rc = db_read_lock(h_cdb);
    if ( i4_rc != DBR_OK )
    {
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_READ_LOCK_FAILED);
    }

    /*
      Sort the records by client compare function.
    */
    i4_rc = scdb_sort_cdb_rec(h_cdb, pt_cmp_fct_info, FALSE, pui4_db_version);
    if ( i4_rc != SCDBR_OK )
    {
        /* sort list can not be re-sorted/created. */
        db_read_unlock(h_cdb);
        return i4_rc;
    }

    /* Start the search from the beginning of the sorted list. */
    db_set_cursor(h_cdb, DB_FIRST_RECORD, DB_NULL_RECORD);

    /*
       Convert SCDB record struct to an addresss.  We need to
       extract 3 field: <stream_type> <stream_tag> <stream_data>
       from the SCDB record.
    */
    apv_field_values[0]=(STREAM_TYPE_T*)(&(pt_stream_desc->e_type));
    ui4_recfld_type[0]=(DB_FIELD_T)CDB_TYPE_STREAM_TYPE ;

    apv_field_values[1]=(VOID*)(&(pt_stream_desc->pv_stream_tag));
    ui4_recfld_type[1]=(DB_FIELD_T)CDB_TYPE_STREAM_TAG ;

    apv_field_values[2]=(SCDB_REC_T*)(pt_scdb_rec);
    ui4_recfld_type[2]=(DB_FIELD_T)CDB_TYPE_STREAM_DATA ;

    ui1_fld_count=3;

    /*
      Retrieve the record from the CoreDB database.
    */
    while ( 1 )
    {
        i4_rc = db_read_sorted_rec(h_cdb,
                                   ui1_fld_count,
                                   ui4_recfld_type,
                                   apv_field_values,
                                   pui4_db_version);

        if ( i4_rc == DBR_OK )
        {
            if ( e_stream_type == ST_UNKNOWN ||
                 e_stream_type == pt_stream_desc->e_type )
            {
                if (ui2_rec_index == 0)
                {
                    /* Matching one found */
                    i4_rc = SCDBR_OK;
                    break;
                }
                ui2_rec_index--;
            }
        }
        else
        {
            if ( i4_rc == DBR_END_OF_REC )
            {
                i4_rc=SCDBR_REC_NOT_FOUND;
            }
            else
            {
                /* translate the CDB error code to SCDB error code */
                i4_rc=scdb_error_code(i4_rc);

            }
            /* break out the while loop. */
            break;
        }

        /* Move the cursor to the the next or prev record.  */
        i4_rc=db_set_cursor(h_cdb, DB_NEXT_RECORD, DB_NULL_RECORD);
        if ( i4_rc == DBR_END_OF_REC )
        {
            i4_rc=SCDBR_REC_NOT_FOUND;
            break;
        }
    }

    db_read_unlock(h_cdb);

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_error_code
 *
 * Description: API to translate CDB error code to SCDB error code.
 *             
 *
 * Inputs:  - i4_cdb_error: CDB error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_error_code(INT32 i4_cdb_error)
{
    INT32 i4_rc;
    
    switch (i4_cdb_error)
    {
        case DBR_OK:
            i4_rc=SCDBR_OK;
            break;

        case DBR_FAIL:
            i4_rc=SCDBR_FAIL;
            break;
            
        case DBR_OUT_OF_MEMORY:
            i4_rc=SCDBR_OUT_OF_MEM;
            break;

        case DBR_INV_HANDLE:
            i4_rc=SCDBR_INV_HANDLE;
            break;

        case DBR_OUT_OF_HANDLE:
            i4_rc=SCDBR_OUT_OF_HANDLE;
            break;

        case DBR_NO_WRITE_LOCK:
            i4_rc=SCDBR_NO_WRITE_LOCK;
            break;

        case DBR_NOT_FOUND:
            i4_rc=SCDBR_NOT_FOUND;
            break;
            
        case DBR_REC_NOT_FOUND:
            i4_rc=SCDBR_REC_NOT_FOUND;
            break;

        case DBR_INV_ARG:
            i4_rc=SCDBR_INV_ARG;
            break;

        case DBR_MODIFIED:
            i4_rc=SCDBR_DB_MODIFIED;
            break;

        case DBR_DB_LIMIT_EXCEED:
            i4_rc=SCDBR_DB_LIMIT_EXCEED;
            break;
            
        default:
            i4_rc=SCDBR_FAIL;
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: scdb_database_name
 *
 * Description: API to translate connection handle to CDB database name.
 *             
 *
 * Inputs:  - i4_cdb_error: CDB error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/

INT32 scdb_database_name(HANDLE_T h_connection, CHAR name[])
{
    INT32 i4_rc;
    
    if ( name != NULL )
    {
        x_sprintf(name,"SCDB_%d\0",(UINT32) h_connection);
        i4_rc=SCDBR_OK;
    }
    else
    {
        i4_rc=SCDBR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: scdb_get_default_rec_by_stream_type
 *
 * Description: API to search SCDB database for the default record for
 *              a specific stream type (e.g. ST_VIDEO, ST_AUDIO, etc).
 *             
 * Inputs: 
 *   h_scdb:    handle to SCDB database.
 *
 *   pt_stream_desc:  structure for the stream component description.
 *
 * Outputs:
 *   pt_scdb_rec:  pointer to the SCDB record.                  
 *
 * Returns: -    
 *                        
 ----------------------------------------------------------------------------*/
INT32  scdb_get_default_rec_by_stream_type
(
    HANDLE_T                  h_scdb,
    const STREAM_COMP_ID_T*   pt_stream_desc,
    SCDB_REC_T*               pt_scdb_rec
)
{
    
    /* Find the default stream type. */
    INT32               i4_rc;
    UINT16              ui2_idx;
    STREAM_COMP_ID_T    t_stream_desc_found;
    UINT32              ui4_db_version;
    BOOL                b_found;

    ui2_idx = 0;
    ui4_db_version = 0;
    b_found  = FALSE;
    i4_rc    = SCDBR_REC_NOT_FOUND;
                
    /*
      Search through the specified stream type and
      find the one with default flag set.
    */
    while ( x_scdb_get_rec_by_idx(h_scdb,
                                  pt_stream_desc->e_type,
                                  ui2_idx,
                                  &t_stream_desc_found,
                                  pt_scdb_rec,
                                  &ui4_db_version)
            == SCDBR_OK &&
            (t_stream_desc_found.e_type == pt_stream_desc->e_type)
        )
    {
        switch(pt_scdb_rec->e_rec_type)
        {
            case SCDB_REC_TYPE_AUDIO_MPEG:
            {
                if ( pt_scdb_rec->u.t_audio_mpeg.b_default == TRUE )
                {
                    b_found = TRUE;
                }
            }
            break;

            case SCDB_REC_TYPE_VIDEO_MPEG:
            {
                if ( pt_scdb_rec->u.t_video_mpeg.b_default == TRUE )
                {
                    b_found = TRUE;
                }
            }
            break;

            case SCDB_REC_TYPE_CLOSED_CAPTION:
            {
                if ( pt_scdb_rec->u.t_atsc_cc.b_default == TRUE )
                {
                    b_found = TRUE;
                }
            }
            break;

            case SCDB_REC_TYPE_AUDIO_ANALOG:
            {
                if ( pt_scdb_rec->u.t_audio_analog.b_default == TRUE )
                {
                    b_found = TRUE;
                }
            }
            break;

            case SCDB_REC_TYPE_VIDEO_ANALOG:
            {
                if ( pt_scdb_rec->u.t_video_analog.b_default == TRUE )
                {
                    b_found = TRUE;
                }
            }
            break;
                        
            default:
                break;
        }

        if ( b_found == TRUE )
        {
            i4_rc = SCDBR_OK;
            break;
        }
        else
        {
            /* try next SCDB record. */
            ui2_idx++;
        }
    }

    return i4_rc;            
}

