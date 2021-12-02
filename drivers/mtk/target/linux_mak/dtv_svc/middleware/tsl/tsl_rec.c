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
 * $RCSfile: tsl_rec.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements function for specifying the TSL database name
 *    to be used for CDB, and for creating TSL record definition.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "tsl/_tsl.h"
#include "tsl/tsl_rec.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/


/**********************************************************************
   Following are private functions only known to the TSL library.
***********************************************************************/

/*-----------------------------------------------------------------------------
 * Name: tsl_db_rec_and_key_def
 *
 * Description: This function get the record field and key field
 * definitions that will be used to store record entry in CDB object.
 *
 *    The TSL record is defined by the following record fields:
 *
 *              <ui2_prev_rec_id>
 *              <ui2_next_rec_id>
 *
 *              <ui2_tsl_rec_id>
 *              <header>   bytes 
 *              <data>     bytes
 *              <s_nw_name> string
 *              <alternate tune info> bytes
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
 *   TSLR_OK       A valid TSL record definition is returned
 *   
 ----------------------------------------------------------------------------*/

#define  TSL_PRIVATE_FIELD_TYPES  (DB_FIELD_T) TSL_CDB_TYPE_PREV_REC_ID, \
                                  (DB_FIELD_T) TSL_CDB_TYPE_NEXT_REC_ID

#define  TSL_REC_FIELD_TYPES      (DB_FIELD_T) TSL_CDB_TYPE_REC_ID,      \
                                  (DB_FIELD_T) TSL_CDB_TYPE_HEADER_TAG,  \
                                  (DB_FIELD_T) TSL_CDB_TYPE_HEADER,      \
                                  (DB_FIELD_T) TSL_CDB_TYPE_DATA_TAG,    \
                                  (DB_FIELD_T) TSL_CDB_TYPE_DATA,        \
                                  (DB_FIELD_T) TSL_CDB_TYPE_NW_NAME, \
                                  (DB_FIELD_T) TSL_CDB_TYPE_ALT_TUNE_INFO_DATA_TAG, \
                                  (DB_FIELD_T) TSL_CDB_TYPE_ALT_TUNE_INFO_DATA,     \
                                  (DB_FIELD_T) TSL_CDB_TYPE_REC_REFERENCE_DATA_TAG, \
                                  (DB_FIELD_T) TSL_CDB_TYPE_REC_REFERENCE_DATA

/* Field types and  number of element per field. */
#define  TSL_PRIVATE_FIELD_INFO   {(DB_FIELD_T)TSL_CDB_TYPE_PREV_REC_ID,1},  \
                                  {(DB_FIELD_T)TSL_CDB_TYPE_NEXT_REC_ID,1}

#define  TSL_REC_FIELD_INFO  {(DB_FIELD_T)TSL_CDB_TYPE_REC_ID,1},                      \
                             {(DB_FIELD_T)TSL_CDB_TYPE_HEADER_TAG, 1},                 \
                             {(DB_FIELD_T)TSL_CDB_TYPE_HEADER,TSL_REC_HEADER_PADDING}, \
                             {(DB_FIELD_T)TSL_CDB_TYPE_DATA_TAG, 1},                   \
                             {(DB_FIELD_T)TSL_CDB_TYPE_DATA,TSL_REC_DATA_PADDING},     \
                             {(DB_FIELD_T)TSL_CDB_TYPE_NW_NAME,MAX_NW_NAME_LEN},       \
                             {(DB_FIELD_T)TSL_CDB_TYPE_ALT_TUNE_INFO_DATA_TAG,1},      \
                             {(DB_FIELD_T)TSL_CDB_TYPE_ALT_TUNE_INFO_DATA,TSL_REC_ALTERNATE_TUNE_INFO_PADDING}, \
                             {(DB_FIELD_T)TSL_CDB_TYPE_REC_REFERENCE_DATA_TAG,1},      \
                             {(DB_FIELD_T)TSL_CDB_TYPE_REC_REFERENCE_DATA,TSL_REC_REFERENCE_PADDING}

static DB_FIELD_T       aui4_db_recfld_type[TSL_DB_NUM_OF_FIELDS] =
{
    /*
      field types for private data for linked list of TSL record ID.
	 */
    TSL_PRIVATE_FIELD_TYPES,  

    /*
       Field types for the TSL record.
	*/
    TSL_REC_FIELD_TYPES
};


static DB_FIELD_INFO_T   at_db_recfld_info[TSL_DB_NUM_OF_FIELDS] =
{
    /* Private data for record link list. */
    TSL_PRIVATE_FIELD_INFO,
    
    /* Field Types for TSL record */   
    TSL_REC_FIELD_INFO
};

static DB_FIELD_T       aui4_db_keyfld_type[TSL_DB_NUM_OF_KEY_FIELDS] =
{
    (DB_FIELD_T) TSL_CDB_TYPE_REC_ID
};

static DB_FIELD_T       aui4_tsl_recfld_type[TSL_NUM_OF_REC_FIELDS] =
{
    /*
       Field types for the TSL record.
	*/
    TSL_REC_FIELD_TYPES
};

static DB_FIELD_INFO_T   at_tsl_recfld_info[TSL_NUM_OF_REC_FIELDS] =
{   
    /* Field Types for TSL record */   
    TSL_REC_FIELD_INFO
};

INT32 tsl_db_rec_and_key_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type,
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* Record field definition. */
    *pui1_fld_count=TSL_DB_NUM_OF_FIELDS;
    *ppt_rec_fields=at_db_recfld_info;
    *ppui4_recfld_type=aui4_db_recfld_type;

    /* Key field definition. */
    *pui1_key_count=TSL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    
    return TSLR_OK;
}

INT32 tsl_get_rec_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type
)
{
    /* Record field definition. */
    *pui1_fld_count=TSL_NUM_OF_REC_FIELDS;
    *ppt_rec_fields=at_tsl_recfld_info;
    *ppui4_recfld_type=aui4_tsl_recfld_type;
    return TSLR_OK;
}

INT32 tsl_get_key_def
(
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* Key field definition. */
    *pui1_key_count=TSL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    
    return TSLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: tsl_rec_fld_to_addr
 *
 * Description: This function returns the address of each field for the
 *    specified TSL record structure. 
 *
 * Inputs: 
 *    pt_tsl_rec:  pointer to the record structure.
 *          
 *    i2_loc:      This value is used as offset to the index location in the
 *                 'apv_rec_fld_addr' array to begind writing the TSL record
 *                 field address.
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s)
 *
 * Returns: 
 *   TRUE      TSL ID/Name is not currently in the database..
 *   FALSE     Invalid TSL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 tsl_rec_fld_to_addr
(
    const TSL_REC_T*       pt_tsl_rec,
    const DB_UNION_TAG_T*  at_utag,
    const VOID*            apv_rec_fld_addr[],
    INT16                  i2_loc
)
{    
    apv_rec_fld_addr[i2_loc+0]=&(pt_tsl_rec->ui2_tsl_rec_id);
    apv_rec_fld_addr[i2_loc+1]=&(at_utag[0]);
    apv_rec_fld_addr[i2_loc+2]=&(pt_tsl_rec->uheader);
    apv_rec_fld_addr[i2_loc+3]=&(at_utag[1]);
    apv_rec_fld_addr[i2_loc+4]=&(pt_tsl_rec->udata);
    apv_rec_fld_addr[i2_loc+5]=&(pt_tsl_rec->s_nw_name);
    apv_rec_fld_addr[i2_loc+6]=&(at_utag[2]);
    apv_rec_fld_addr[i2_loc+7]=&(pt_tsl_rec->utune);
    apv_rec_fld_addr[i2_loc+8]=&(at_utag[3]);
    apv_rec_fld_addr[i2_loc+9]=&(pt_tsl_rec->uref);
    
    return TSLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: tsl_get_db_fld_addr
 *
 * Description: This function returns the an array of addresses for
 *    the private data field and TSL record fields.
 *
 * Inputs:
 *    pt_rec_id_link: pointer to the private data.
 *    pt_tsl_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s). The size of
 *                       of this array must be number of record fields +
 *                       the number of private fields.
 *
 * Returns: 
 *   TRUE      TSL ID/Name is not currently in the database..
 *   FALSE     Invalid TSL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 tsl_get_db_fld_addr
(
    const TSL_REC_ID_LINK_T*  pt_rec_id_link,
    const TSL_REC_T*          pt_tsl_rec,
    const DB_UNION_TAG_T*     at_utag,
    const VOID*               apv_rec_fld_addr[]
)
{ 
    /* copy the address of private data to the address array. */  
    apv_rec_fld_addr[0]=&(pt_rec_id_link->ui2_prev);
    apv_rec_fld_addr[1]=&(pt_rec_id_link->ui2_next);
   
    /* copy the TSL record field address to the address array, with
       starting location of address of array at 2. */
    tsl_rec_fld_to_addr(pt_tsl_rec,
                        at_utag,
                        apv_rec_fld_addr,
                        TSL_NUM_PRIVATE_FIELDS);

    return TSLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: tsl_get_db_key_addr
 *
 * Description: This function returns the address of key field for the
 *    specified TSL record structure. 
 *
 * Inputs: 
 *    pt_tsl_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_key_fld_addr:  Address of the key field(s)
 *
 * Returns: 
 *   TRUE      TSL ID/Name is not currently in the database..
 *   FALSE     Invalid TSL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 tsl_get_db_key_addr
(
    const TSL_REC_T*   pt_tsl_rec,
    const VOID*        apv_key_fld_addr[]
)
{
    apv_key_fld_addr[0]=&(pt_tsl_rec->ui2_tsl_rec_id);
    return TSLR_OK;
}


