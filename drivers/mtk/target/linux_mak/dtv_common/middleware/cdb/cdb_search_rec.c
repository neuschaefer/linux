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
 * $RCSfile: cdb_search_rec.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file implements CDB record search functions.  In the search
 *         operation, field of CDB record is compared with the specified
 *         search parameters. The search parameters (the key fields) consist
 *         of:
 *              1. number of search key fields
 *              2. field types of the specified key fields.
 *              3. address to the content of key field to compare with
 *                 the valus of record fields.
 *              4. optional comparson parameters.
 *
 *         
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

/*******************************************************************
            Static function declaration. 
********************************************************************  */
static
BOOL _db_keys_equal_recflds
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    UINT8		       ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[]
);

static
BOOL _db_cmp_keys_and_recflds
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    UINT8		       ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[]
);

static 
INT32 _db_cmp_key_rec
(
    const VOID*       pv_a,
    const VOID*       pv_b,
    DB_FIELD_T        t_fld_type,
    UINT16            ui2_width, 
    DB_CMP_OPCODE_T   e_opcode
);

/*-----------------------------------------------------------------------
  function implementation 
  -----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: db_find_rcd_linear_search
 *
 * Description: API to find the specific record in the memory buffer.
 *              Note: this find is performing a linear search. The function 
 *              needs to be protected by the Read-lock
 *.
 * Inputs:  - pt_do:    pointer to database object.
 *            ui1_keyfld_count:  number of fields that makes up the
 *                                  search parameters.
 *            aui4_keyfld_types:     field types of the search parameters.
 *            apv_keyfld_addrs:   addrs of for the search parameter values.
 *            at_keyfld_search_opts: array of search options for each
 *                                   specified key field.
 *
 * Outputs: - ppui1_record:  pointer to a variable containing the
 *                           record address.
 *
 * Returns: - DBR_OK:     record is found.   
 *            DBR_FAIL:   reach end of the memory buffer, no more
 *                        record match.
 *            
 ----------------------------------------------------------------------------*/
INT32 db_find_rcd_linear_search
(
    CDB_DATA_OBJ_T*     pt_do,
    UINT8**             ppui1_record, 
    UINT8		        ui1_keyfld_count,
    const DB_FIELD_T    aui4_keyfld_types[],
    const VOID*         apv_keyfld_addrs[],
    const DB_SEARCH_T   at_keyfld_search_opts[],
    CDB_MEM_CURSOR_T*   pt_mem_cursor
)
{
    while ( db_get_next_used_block(pt_do->t_memory,
                                   pt_mem_cursor,
                                   ppui1_record)
            == DBR_OK )
    {
		  /*
		    Default search (e.g., no search option specified)
		  */
		  if ( at_keyfld_search_opts == NULL )
		  {
			   if (_db_keys_equal_recflds(pt_do,
                                          (*ppui1_record),
                                          ui1_keyfld_count,
                                          aui4_keyfld_types,
                                          apv_keyfld_addrs
                       ) == TRUE
                   )
				{
				  /* Found a record that matches the search parameter. */
				  return DBR_OK;
				}
		  }
		  else
		  {
			   /*
			     Compare key with record using caller provided
				  paramters and evaluation function.
			   */
			   if ( _db_cmp_keys_and_recflds(pt_do,
                                             (*ppui1_record),
                                             ui1_keyfld_count,
                                             aui4_keyfld_types,
                                             apv_keyfld_addrs,
                                             at_keyfld_search_opts
                                         ) == TRUE
					)
				{
				    /* Found a record that matches the search parameter. */
				    return DBR_OK;
				}
		  }
	 }
    /* Iterate through all the used block, did not find a match. */
    return DBR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: _db_keys_equal_recflds
 *
 * Description: Default compare function to compares search key field values
 *              with record's field values. The default comparison
 *              operatoin is 'A==B'.
 *              Special case: if ui1_keyfld_count is 0, then any record 
 *                            is considered as a positive match.
 *
 * Inputs:  - pt_do:  database object.
 *            pui1_record:        record to be compared.
 *            ui1_keyfld_count:   count of the key parameters
 *            aui4_keyfld_types:  Array of key field types.
 *            apv_keyfld_addrs:   Array of addresses containing the key values.
 *
 * Outputs: - None.
 *
 * Returns: - TRUE: if the key values equals the field values of the record.
 *                  special case, if no keyfields are specified, e.g., 
 *                  the ui1_keyfld_count is 0, then return TRUE.
 *
 *            FALSE: 
 *     
 ----------------------------------------------------------------------------*/
static
BOOL _db_keys_equal_recflds
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    UINT8		       ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[]
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;
    BOOL       b_result;

    b_result  = TRUE;
    
    for ( ui1_i=0; ui1_i < ui1_keyfld_count; ui1_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_keyfld_types[ui1_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            /*
              If the field is a C-Lang string type (terminated
              by \0, then use x_str_buf_cmp_string()
            */
            if ( DB_GET_C_TYPE(aui4_keyfld_types[ui1_i]) == DBF_STRING_T )
            {
                STRING_ID_T     t_string_id;
                /*
                   If this is a string, then we call the mini-buffer
                   API to compare the string ID stored in this field
                   with the client provided string.
                */
                x_memcpy(&t_string_id,(pui1_record+ui2_offset),
                         ui2_width);

                if ( x_str_buf_cmp_string(pt_do->pt_strbuf,
                                          t_string_id,
                                          (const CHAR*)apv_keyfld_addrs[ui1_i])
                     != 0
                    )
                {
                    b_result = FALSE;
                }
#if 0
                if ( x_strcmp(apv_keyfld_addrs[ui1_i],
                              (CHAR*)(pui1_record+ui2_offset)) != 0 )
                {
                    b_result = FALSE;
                }
#endif                 
            }
            else
            {
                if ( x_memcmp(apv_keyfld_addrs[ui1_i],
                              (pui1_record+ui2_offset),
                              ui2_width) != 0 )
                {
                    b_result = FALSE;
                }
            }
        }
        else
        {
            /* Key field is not the right type. */
            b_result = FALSE;
        }
    }
    /*
       If no keyfld specified or all the records and key matched,
       then return TRUE.
    */
    return b_result;
}

/*-----------------------------------------------------------------------------
 * Name: _db_cmp_keys_and_recflds
 *
 * Description: API to compares search key field values with client
 *              specified operation or functions (if the field to be
 *              compared is an UNION type.).
 *
 *              Special case: if ui1_keyfld_count is 0, then any record 
 *                            is considered as a positive match.
 * 
 * Inputs:  - pt_do:             database object.
 *            pui1_record:       record to be compare.
 *            ui1_keyfld_count:  count of the key parameters
 *            aui4_keyfld_types: Array of key field types.
 *            apv_keyfld_addrs:  Array of addresses for the key values.
 *            at_keyfld_search_opts: Array of optional search/compare
 *                                   parameters.
 *
 * Outputs: - None.
 *
 * Returns: - TRUE: if the key values equals the field values of the record.
 *
 *            FALSE: 
 *                        
 ----------------------------------------------------------------------------*/
static
BOOL _db_cmp_keys_and_recflds
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    UINT8		       ui1_keyfld_count,
    const DB_FIELD_T  aui4_keyfld_types[],
    const VOID*       apv_keyfld_addrs[],
    const DB_SEARCH_T at_keyfld_search_opts[]
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;
    
    for ( ui1_i=0; ui1_i < ui1_keyfld_count; ui1_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_keyfld_types[ui1_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            /*
              Evaluate the optional parameter 
            */
            if ( DB_GET_C_TYPE(aui4_keyfld_types[ui1_i]) != DBF_UNION_T )
            {
				 /* 
                   If optional parameter is a 'equal' operation, 
                   then we can perfomr a memory compare. This save
                   some work on decoding the field values.
				 */
   				 if ( at_keyfld_search_opts[ui1_i].e_opcode == DB_OPS_EQUAL )
                 {
                     if ( DB_GET_C_TYPE(aui4_keyfld_types[ui1_i])
                          == DBF_STRING_T )
                     {
                         STRING_ID_T     t_string_id;
                         /*
                           If this is a string, then we call the mini-buffer
                           API to compare the string ID stored in this field
                           with the client provided string.
                         */
                         x_memcpy(&t_string_id,(pui1_record+ui2_offset),
                                  ui2_width);

                         if ( x_str_buf_cmp_string(pt_do->pt_strbuf,
                                                   t_string_id,
                                                   (const CHAR*)apv_keyfld_addrs[ui1_i])
                              != 0
                             )
                         {
                             return FALSE;
                         }
                     }
                     else
                     {
                         if ( x_memcmp(apv_keyfld_addrs[ui1_i],
                                       (pui1_record+ui2_offset),
                                       ui2_width) != 0 )
                         {
                             return FALSE;
                         }
                     }
                 }
                 else
                 {
						  /*
                       call the compare key and record field function
                       to use caller specified compare operation.
						  */    
                    if ( _db_cmp_key_rec(apv_keyfld_addrs[ui1_i],
                                         (pui1_record+ui2_offset),
                                         aui4_keyfld_types[ui1_i],
                                         ui2_width, 
                                         at_keyfld_search_opts[ui1_i].e_opcode
                                         ) != 0 
                       )
                    {
                        return FALSE;
                    }
                 }
            }
            else
            {
                /* The field to be compared is a UNION type.
                   We need to call the client provided function
                   to evaluate the search value with record
                   value.
                */
                if ( at_keyfld_search_opts[ui1_i].pf_fct != NULL )
                {
                    INT32            i4_result;
                    DB_UNION_TAG_T   e_tag_val;
                    
                    /*
                      Get union tag value of this UNION field
                    */
                    if ( db_get_union_tag_value(pt_do,
                                                pui1_record,
                                                aui4_keyfld_types[ui1_i],
                                                &(e_tag_val)) == TRUE )
                    {
                        
                        
                        i4_result=at_keyfld_search_opts[ui1_i].pf_fct(
                            apv_keyfld_addrs[ui1_i],
                            (pui1_record+ui2_offset),
                            e_tag_val,
                            (UINT32)ui2_width,
                            at_keyfld_search_opts[ui1_i].e_opcode,
                            at_keyfld_search_opts[ui1_i].pv_tag);

                        /*
                          Convert the return code from the compare 
                          function to TRUE or FALSE.
                        */
                        if ( i4_result != 0 )
                        {
                            return FALSE;
                        }
                    }
                    else
                    {
                        /*
                          No Union tag value found.
                        */
                        return FALSE;
                    }
                }
                else
                {
                    /*
                      The UNION compare function is NULL.  
					*/
                    return FALSE;
                }
            } /* End of UNION field compare */    
        }
        else
        {
            /* Key field is not the right type. */
            return FALSE;
        }
        /* continue on to the next key field and record field. */
    }
    /*
       If no key field is specified or record and key value matched,
       then return true.
    */
    return TRUE;
}


/* ***************************************************************
 Macro for performing C-language ops on two integral scalar value. 
***************************************************************   */
/*
  Evaluate a and b using the specified operation, 
  Store the result in the  _result_ variable. 
*/
#define EVAL_INTEGRAL_REC(_a_,_b_,_ops_,_result_,_type_cast_)   \
do                                         \
{                                          \
    switch(_ops_)                          \
    {                                      \
        case DB_OPS_BITWISE_AND:           \
        {                                  \
            _result_=(_type_cast_)((_a_) & (_b_));  \
        }                                  \
        break;                             \
                                           \
        default:                           \
            _result_=0;                    \
    }                                      \
} while(0)

/*-----------------------------------------------------------------------------
 * Name: _db_cmp_key_rec
 *
 * Description: API to compare two fields from records. 
 *             
 * Inputs:  - 
 *    pv_a:        Pointer to the first record field.
 *    pv_b:        Pointer to the second record field.
 *    t_fld_type:  Specifies the field type of the record field.
 *    ui2_width:   Width (in bytes) of the record field. 
 *
 * Outputs: - None.
 *
 * Returns: 
 *    0:           Key field and record field results in a positive match.
 *    1:           negative match.
 *            
 ----------------------------------------------------------------------------*/
static 
INT32 _db_cmp_key_rec
(
    const VOID*       pv_a,
    const VOID*       pv_b,
    DB_FIELD_T        t_fld_type,
    UINT16            ui2_width, 
    DB_CMP_OPCODE_T   e_opcode
) 
{
    UINT8*                      pui1_a;
    UINT8*                      pui1_b;
    INT32                       i4_result;
    

    /* Temporary variables to copy the record field value to.  Need 
       to do this in-order to ensure memory alignment for the C-lang
       intrinsic types. 
    */
    CHAR             c1_a,    c1_b;
    UINT8            ui1_a,   ui1_b;
    UINT16           ui2_a,   ui2_b;
    UINT32           ui4_a,   ui4_b;
    INT8             i1_a,    i1_b;
    INT16            i2_a,    i2_b;
    INT32            i4_a,    i4_b;
    FLOAT            f4_a,    f4_b;
    
    UTF16_T          utf2_a,  utf2_b;            
    UTF32_T          utf4_a,  utf4_b;

    
    pui1_a = (UINT8*) pv_a ;
    pui1_b = (UINT8*) pv_b ;
    i4_result = 0;

    switch ( DB_GET_C_TYPE(t_fld_type) )
    {
        case DBF_UINT8_T :
        {
            x_memcpy(&ui1_a, pui1_a, ui2_width);
            x_memcpy(&ui1_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(ui1_a,ui1_b,e_opcode,i4_result,INT32);
        }
        break;

        case DBF_UINT16_T :
        {
            x_memcpy(&ui2_a, pui1_a, ui2_width);
            x_memcpy(&ui2_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(ui2_a,ui2_b,e_opcode,i4_result,INT32);
        }
        break;

        case DBF_UTF16_T :
        {
            x_memcpy(&utf2_a, pui1_a, ui2_width);
            x_memcpy(&utf2_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(utf2_a,utf2_b,e_opcode,i4_result,INT32);
        }
        break;

        case DBF_UINT32_T :
        {
            x_memcpy(&ui4_a, pui1_a, ui2_width);
            x_memcpy(&ui4_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(ui4_a,ui4_b,e_opcode,i4_result,INT32);
        }
        break;                    

        case DBF_UTF32_T :
        {
            x_memcpy(&utf4_a, pui1_a, ui2_width);
            x_memcpy(&utf4_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(utf4_a,utf4_b,e_opcode,i4_result,INT32);
        }
        break;
                    
        case DBF_INT8_T :
        case DBF_ENUM8_T:
        {
            x_memcpy(&i1_a, pui1_a, ui2_width);
            x_memcpy(&i1_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(i1_a,i1_b,e_opcode,i4_result,INT32);
        }
        break;
                    
        case DBF_INT16_T :
        case DBF_ENUM16_T:
        {
            x_memcpy(&i2_a, pui1_a, ui2_width);
            x_memcpy(&i2_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(i2_a,i2_b,e_opcode,i4_result,INT32);
        }
        break;
                    
        case DBF_INT32_T :
        case DBF_ENUM32_T:
        {
            x_memcpy(&i4_a, pui1_a, ui2_width);
            x_memcpy(&i4_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(i4_a,i4_b,e_opcode,i4_result,INT32);
        }
        break;                    
                    
        case DBF_CHAR_T :
        {
            x_memcpy(&c1_a, pui1_a, ui2_width);
            x_memcpy(&c1_b, pui1_b, ui2_width);
            EVAL_INTEGRAL_REC(c1_a,c1_b,e_opcode,i4_result,INT32);
        }
        break;                    

        case DBF_FLOAT32_T :
        {
            x_memcpy(&f4_a, pui1_a, ui2_width);
            x_memcpy(&f4_b, pui1_b, ui2_width);
            /*
              Currently, the 'bitwise and' does not apply to
              float, so return negative match.
            */
            i4_result=-1;
        }
        break;

        default:
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_UNKNOWN_FIELD_TYPE);
    }
    
    if ( i4_result > 0 )
    {  
        /* If the evaluation of two record fields resulted in a positive 
           value, e.g., BITWISE AND, BITWISE OR, etc, then it is considered
           that record field and key field is a match.  We convert the 
           result to represent a 'match'.  
        */
        i4_result=0;
    }
    else
    {   
        /* Result is not a matched. */
		  i4_result = 1;
    }
    return i4_result;
}

