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
 * $RCSfile: lol_rec.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains definitions and descriptions of LOL record
 *    fields that are used by CDB module, and for storing LOL record
 *    in memory.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"
#include "cdb/cdb_api.h"
#include "lol/lol.h"
#include "lol/_lol.h"
#include "lol/lol_rec.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/


/**********************************************************************
   Following are private functions only known to the LOL module.
***********************************************************************/

static DB_FIELD_INFO_T  at_db_recfld_info[LOL_DB_NUM_OF_FIELDS] =
{
    /* field types private data for record link list. */
    { (DB_FIELD_T)LOL_CDB_TYPE_PREV_REC_ID,    1 },
    { (DB_FIELD_T)LOL_CDB_TYPE_NEXT_REC_ID,    1 },

    /* LOL record field type */  /* number of element per field. */
    { (DB_FIELD_T)LOL_CDB_TYPE_REC_ID,         1 },
    { (DB_FIELD_T)LOL_CDB_TYPE_DATA_TAG,      1 },
    { (DB_FIELD_T)LOL_CDB_TYPE_DATA_BUF,       MAX_LARGE_OBJECT_LEN },
};

static DB_FIELD_T     aui4_db_recfld_type[LOL_DB_NUM_OF_FIELDS] =
{
    /*
       private data for linked list of LOL record ID.
	*/
    (DB_FIELD_T)LOL_CDB_TYPE_PREV_REC_ID,
    (DB_FIELD_T)LOL_CDB_TYPE_NEXT_REC_ID,

    /*
       Field types for the LOL record.
	*/
    (DB_FIELD_T)LOL_CDB_TYPE_REC_ID,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_TAG,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_BUF,
    
};

static DB_FIELD_T        aui4_db_keyfld_type[LOL_DB_NUM_OF_KEY_FIELDS] =
{
    (DB_FIELD_T)LOL_CDB_TYPE_REC_ID
};

static DB_FIELD_INFO_T  at_lol_recfld_info[LOL_NUM_OF_REC_FIELDS] =
{
    /* LOL record field type */    /* Number of element per field. */
    { (DB_FIELD_T)LOL_CDB_TYPE_REC_ID,         1 },
    { (DB_FIELD_T)LOL_CDB_TYPE_DATA_TAG,      1 },
    { (DB_FIELD_T)LOL_CDB_TYPE_DATA_BUF,       MAX_LARGE_OBJECT_LEN },
};

static DB_FIELD_T        aui4_lol_recfld_type[LOL_NUM_OF_REC_FIELDS] =
{
    /*
       Field types for the LOL record.
	 */
    (DB_FIELD_T)LOL_CDB_TYPE_REC_ID,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_TAG,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_BUF,
};

static DB_FIELD_T     aui4_prv_and_lol_recfld_type[]=
{
    /*
       private data for linked list of LOL record ID.
	*/
    (DB_FIELD_T)LOL_CDB_TYPE_PREV_REC_ID,
    (DB_FIELD_T)LOL_CDB_TYPE_NEXT_REC_ID,
    /*
       Field types for the LOL record.
	 */
    (DB_FIELD_T)LOL_CDB_TYPE_REC_ID,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_TAG,
    (DB_FIELD_T)LOL_CDB_TYPE_DATA_BUF,
};

/*-----------------------------------------------------------------------------
 * Name: lol_db_rec_and_key_def
 *
 * Description: This function get the LOL record field and key field
 * definitions that will be used to store LOL record entry in CDB object.
 *
 *    The CDB record for the LOL data is defined by the following 
 *    record fields:
 *          field                               data type
 *       <ui2_prev_rec_id>                       UINT16
 *       <ui2_next_rec_id>                       UINT16
 *
 *       <ui2_lol_rec_id>                        UINT16
 *       <ui2_nw_id>                             UINT16
 *       <ui4_nw_mask>                           UINT32     
 *       <ui1_nit_ver>                           UINT8
 *       <s_nw_name<[MAX_NETWORK_NAME_LEN]>      String_ID (UINT16) 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    pui1_fld_count:  number of fields in the record.  
 *    ppt_rec_fields:  pointer to an array containing field info structure.
 *                     each field info structure element contains the
 *                     field Type and the number of element for that field
 *                     type.
 *    ppui4_recfld_type: pointer to an array of record field type.
 *    pui1_key_count:  number of the keys in the record.
 *    ppui4_keyfld_type: pointer to an array of key field type.
 *
 * Returns: 
 *   LOLR_OK       A valid LOL record definition is returned
 *   
 ----------------------------------------------------------------------------*/
INT32 lol_db_rec_and_key_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type,
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* DB Record field type and size definition. */
    *pui1_fld_count=LOL_DB_NUM_OF_FIELDS;
    *ppt_rec_fields=at_db_recfld_info;

    /* DB record field type. */
    *ppui4_recfld_type=aui4_db_recfld_type;

    /* DB Key field definition. */
    *pui1_key_count=LOL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    
    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_rec_def
 *
 * Description: This function get the LOL record field definitions
 *    that are defined in fields in LOL_REC_T structure.
 *
 *    The CDB record for the LOL data is defined by the following 
 *    record fields:
 *          field                               data type
 *
 *       <ui2_lol_rec_id>                        UINT16
 *       <ui2_nw_id>                             UINT16
 *       <ui4_nw_mask>                           UINT32     
 *       <ui1_nit_ver>                           UINT8
 *       <s_nw_name<[MAX_NETWORK_NAME_LEN]>      String_ID (UINT16) 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    pui1_fld_count:  number of fields in the record.  
 *    ppt_rec_fields:  pointer to an array containing field info structure.
 *                     each field info structure element contains the
 *                     field Type and the number of element for that field
 *                     type.
 *    ppui4_recfld_type: pointer to an array of record field type.
 *
 * Returns: 
 *   LOLR_OK       A valid LOL record definition is returned
 *   
 ----------------------------------------------------------------------------*/
INT32 lol_get_rec_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type
)
{
    /* Record field definition. */
    *pui1_fld_count=LOL_NUM_OF_REC_FIELDS;
    *ppt_rec_fields=at_lol_recfld_info;
    *ppui4_recfld_type=aui4_lol_recfld_type;
    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_prv_and_lolrec_def
 *
 * Description: This function get the private fields used by LOL database
 *    and LOL record field definitions that are defined in fields in
 *    LOL_REC_T structure.
 *
 *    The private fields and the LOL record data is defined by the following 
 *    record fields:
 *          field                               data type
 *       <ui2_prev_lol_rec_id>                   UINT16
 *       <ui2_next_lol_rec_id>                   UINT16
 *
 *       <ui2_lol_rec_id>                        UINT16
 *       <ui2_nw_id>                             UINT16
 *       <ui4_nw_mask>                           UINT32     
 *       <ui1_nit_ver>                           UINT8
 *       <s_nw_name<[MAX_NETWORK_NAME_LEN]>      String_ID (UINT16) 
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    pui1_fld_count:  number of fields in the record.  
 *    ppui4_recfld_type:  pointer to an array containing field info structure.
 *                     each field info structure element contains the
 *                     field Type and the number of element for that field
 *                     type.
 *    ppui4_recfld_type: pointer to an array of record field type.
 *
 * Returns: 
 *   LOLR_OK       A valid LOL record definition is returned
 *   
 ----------------------------------------------------------------------------*/
INT32 lol_get_prv_and_lolrec_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_T**       ppui4_recfld_type
)
{
    /* Record field definition for both private field and
       LOL_REC_T fields.
    */
    *pui1_fld_count=LOL_NUM_PRIVATE_FIELDS + LOL_NUM_OF_REC_FIELDS;
    *ppui4_recfld_type=aui4_prv_and_lol_recfld_type;
    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_key_def
 *
 * Description: This function get the CDB data type for the index field
 *    (e.g., key field) for LOL record..
 *
 *    The key field for the CDB record of the LOL data is defined by
 *    the following record fields:
 *
 *          field                               data type
 *
 *       <ui2_lol_rec_id>                        UINT16
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    pui1_key_count:  number of key fields in the record.  
 *    
 *    ppui4_keyfld_typee: pointer to an array of record field type.
 *
 * Returns: 
 *   LOLR_OK       A valid LOL record definition is returned
 *   
 ----------------------------------------------------------------------------*/
INT32 lol_get_key_def
(
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* Key field definition. */
    *pui1_key_count=LOL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_rec_fld_to_addr
 *
 * Description: This function returns the address of each field for the
 *    specified LOL_REC_T structure. 
 *
 * Inputs: 
 *    pt_lol_rec:  pointer to the LOL record structure.
 *    i2_i:        index location in the 'apv_rec_fld_addr' array to begin
 *                 writing the LOL record field address.  This offset is used
 *                 to skip over private fields.
 *
 *    pui4_buf     Pointer to a INT32 buffer for storing UNION_TAG value.
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s)
 *    aui2_addr_len: Optional array to store the size of the address location
 *                   in 'apv_rec_fld_addr'.
 *
 * Returns: 
 *   TRUE      LOL ID/Name is not currently in the database..
 *   FALSE     Invalid LOL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 lol_rec_fld_to_addr
(
    const LOL_REC_T*   pt_lol_rec,
    const VOID*        apv_rec_fld_addr[],
    UINT16             aui2_addr_len[],
    UINT16             ui2_i,
    INT32*             p_union_tag
)
{
    /*  this field is not used right now. */
    *p_union_tag = 0;
    
    apv_rec_fld_addr[ui2_i+0]=&(pt_lol_rec->ui2_lol_rec_id);
    apv_rec_fld_addr[ui2_i+1]=&(pt_lol_rec->ui4_data_tag);
    apv_rec_fld_addr[ui2_i+2]=&(pt_lol_rec->aui1_data);

    if ( aui2_addr_len != NULL )
    {
        aui2_addr_len[ui2_i+0] = (UINT16)sizeof(pt_lol_rec->ui2_lol_rec_id);
        aui2_addr_len[ui2_i+1] = (UINT16)sizeof(pt_lol_rec->ui4_data_tag);
        aui2_addr_len[ui2_i+2] = (UINT16)(pt_lol_rec->ui2_data_len);
    }
    
    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_db_fld_addr
 *
 * Description: This function returns the an array of addresses for
 *    the private data field and LOL record fields.
 *
 * Inputs:
 *    pt_rec_id_link: pointer to the private data.
 *    pt_lol_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s). The size of
 *                       of this array must be number of record fields +
 *                       the number of private fields.
 *
 * Returns: 
 *   TRUE      LOL ID/Name is not currently in the database..
 *   FALSE     Invalid LOL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 lol_get_db_fld_addr
(
    const LOL_REC_ID_LINK_T*  pt_rec_id_link,
    const LOL_REC_T*          pt_lol_rec,
    const VOID*               apv_rec_fld_addr[],
    UINT16                    aui2_addr_len[],
    INT32*                    p_union_tag
)
{ 
    /* copy the address of private data to the address array. */  
    apv_rec_fld_addr[0]=&(pt_rec_id_link->ui2_prev);
    apv_rec_fld_addr[1]=&(pt_rec_id_link->ui2_next);

    aui2_addr_len[0]=2;
    aui2_addr_len[1]=2;
    
    /* copy the LOL record field address to the address array, with
       starting location of address of array at 2. */
    lol_rec_fld_to_addr(pt_lol_rec,apv_rec_fld_addr, aui2_addr_len,
                        LOL_NUM_PRIVATE_FIELDS, p_union_tag);

    return LOLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: lol_get_db_key_addr
 *
 * Description: This function returns the address of key field for the
 *    specified LOL record structure. 
 *
 * Inputs: 
 *    pt_lol_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_key_fld_addr:  Address of the key field(s)
 *
 * Returns: 
 *   LOLR_OK     LOL key field address is returned.
 *   
 ----------------------------------------------------------------------------*/
INT32 lol_get_db_key_addr
(
    const LOL_REC_T*   pt_lol_rec,
    const VOID*        apv_key_fld_addr[]
)
{
    apv_key_fld_addr[0]=&(pt_lol_rec->ui2_lol_rec_id);
    return LOLR_OK;
}


/*----------------------------------------------------------------------
 * Name: lol_data_conv_to_big_end
 *
 * Description: This API is called by the CDB to perform endian
 *              conversion on the union component of the data record
 *              field, when performing writing operation
 *              to the file system (to big-endian)
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK    Conversion of LOL_REC_T fields to big-end is success.
 *   
 -----------------------------------------------------------------------*/
VOID lol_data_conv_to_big_end
(
   DB_FIELD_T	 	 t_fld_type,
   DB_UNION_TAG_T	 t_union_tag,
   const VOID*	     pv_input,
   UINT16		     ui2_size,
   VOID*             pv_output    
)
{
    x_dbg_stmt("<lol_data_conv_to_big_end> this function should not be called since no LOL fields contain union data. \n");
#if 0
    /*
       Currently, this function does notthing because all LOL_REC_T fields
       are explicited defined, so the custom conversion to big-end is needed.
    */
    UINT8*           pui1_input;
    UINT8*           pui1_output;
    
    if ( t_fld_type == LOL_CDB_TYPE_DATA )
    {
        switch (t_union_tag)
        {
           
        }
    }
#endif    
    return;
}

/*----------------------------------------------------------------------
 * Name: lol_data_conv_from_big_end
 *
 * Description: This API is called by the CDB to perform endian
 *              conversion on the union component of the data record
 *              field, when performing reading operation
 *              from the file system (big-endian) to cpu memory order.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL library is initialized successfully.
 *   
 -----------------------------------------------------------------------*/
VOID lol_data_conv_from_big_end
(
   DB_FIELD_T	 	 t_fld_type,
   DB_UNION_TAG_T	 t_union_tag,
   const VOID*	     pv_input,
   UINT16		     ui2_size,
   VOID*             pv_output    
)
{

    x_dbg_stmt("<lol_data_conv_from_big_end> this function should not be called since no LOL fields contain union data. \n");
    
#if 0    
    /*
       Currently, this function does notthing because all LOL_REC_T fields
       are explicited defined, so the custom conversion to big-end is needed.
    */
    
    UINT8*           pui1_input;
    UINT8*           pui1_output;
    
    if ( t_fld_type == LOL_CDB_TYPE_DATA )
    {
        switch (t_union_tag)
        {
          
        }
    }
#endif
    
    return;
}
