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
 * $RCSfile: cdb_record.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains API to obtain meta info for the internal
 *         CDB record.  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "rwlock/rwl_api.h"
#include "cdb/cdb_api.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_record.h"

/*-----------------------------------------------------------------------------
  data declaraions
  ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: db_get_fldtype_size
 *
 * Description: This API return the implicit size (in bytes) of a single 
 *              element of a field type. If the field type is an array type,
 *              It returns the size of an individual element.
 *              To get the total width of a field, you need
 *              to multiply size of element with the number of elements.
 *
 * Inputs:  - ui4_field_type:   Specify the Core Database field type.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK   success.
 *
 ----------------------------------------------------------------------------*/

UINT16 db_get_fldtype_size 
( 
    DB_FIELD_T ui4_field_type 
)
{
    UINT16  i2_sz=0;
    
    switch ( DB_GET_C_TYPE( ui4_field_type ) )
    {
        case DBF_UINT8_T :
        case DBF_INT8_T :
        case DBF_ENUM8_T:
        case DBF_CHAR_T :
        case DBF_ARRAY_OF_UINT8_T :
        case DBF_ARRAY_OF_INT8_T :
        case DBF_ARRAY_OF_ENUM8_T:
        case DBF_ARRAY_OF_CHAR_T :
        {
	        i2_sz = 1;
        }
        break;

        /*
	       Special case for UNION type:

	       The UNION field is considered as an array of 
           bytes.  Therefore, the size of individual element is 
           specified as 1 byte, and the number of element in the
           STRING indicates the total number of bytes for the
           given record field.
	    */
        case DBF_UNION_T:
        {
            i2_sz = 1;
        }
        break;

        /*
	       Special case for STRING/MEMBUF type:

	       The STRING/MEMBUF field is stored in the CDB record as a
           STRING ID or OBJECT ID.  The actual string/membuf is stored
           separately in a string buffer/membuf structure.  Thus width of
           the DBF_STRING_T / DBF_MEMBUF_T field is 2 bytes (e.g.,
           the size of String ID or Membuf ID).
        */
        case DBF_STRING_T :
        case DBF_MEMBUF_T:
        {
            i2_sz = 2;
        }
        break;

        case DBF_UINT16_T :
        case DBF_INT16_T :
        case DBF_ENUM16_T:
        case DBF_UTF16_T :
        case DBF_ARRAY_OF_UINT16_T :
        case DBF_ARRAY_OF_INT16_T :
        case DBF_ARRAY_OF_ENUM16_T:    
        case DBF_ARRAY_OF_UTF16_T :
        {
            i2_sz = 2;
        }
        break;

        case DBF_UINT32_T :
        case DBF_INT32_T :
        case DBF_ENUM32_T:    
        case DBF_UTF32_T :
        case DBF_UNION_TAG_T : 
        case DBF_ARRAY_OF_UTF32_T :    
        case DBF_FLOAT32_T :
        case DBF_ARRAY_OF_UINT32_T :
        case DBF_ARRAY_OF_INT32_T :
        case DBF_ARRAY_OF_ENUM32_T:    
        case DBF_ARRAY_OF_FLOAT32_T :
        {
            i2_sz = 4;
        }
        break;

        case DBF_UINT64_T :
        case DBF_INT64_T :
        case DBF_FLOAT64_T :
        case DBF_ARRAY_OF_UINT64_T :
        case DBF_ARRAY_OF_INT64_T :
        case DBF_ARRAY_OF_FLOAT64_T :
        {
            i2_sz = 8;
        }
        break;
        
        default:
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_UNKNOWN_FIELD_TYPE);
    }
    return i2_sz;
}

/*-----------------------------------------------------------------------------
 * Name: db_create_record_def
 *
 * Description: Internal CDB API to create an internal record definition
 *              (offset and width) from client specified record definition.
 *              The internal record structure (e.g., fields layout) might
 *              be different than the record definition provided by the
 *              client. 
 *
 * Inputs:   
 *    pt_do:         Database object.
 *    ui1_count:     Number of field in the data record definition.
 *    at_field_info: Array of DB_FIELD_INFO_T structures.  The DB_FIELD_INFO
 *                   structure contains the field type and field element
 *                   count for each record field.
 * Outputs: - None.
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
INT32 
db_create_record_def
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8             ui1_count,
    DB_FIELD_INFO_T   at_field_info[]
)
{
    UINT16  ui2_i;
    UINT16  ui2_sz;
    UINT32  ui4_ctype;
    
    pt_do->ui2_record_width=0;
    pt_do->ui1_field_count=ui1_count;
        
    /* copy the fields into the internal data record definition.*/
    for ( ui2_i = 0 ; ui2_i < (UINT16) pt_do->ui1_field_count; ui2_i++ )
    {
        pt_do->pt_fields[ui2_i].ui4_field_type =
            at_field_info[ui2_i].ui4_field_type;
        pt_do->pt_fields[ui2_i].ui2_field_element_count =
            at_field_info[ui2_i].ui2_field_element_count;

        /* Save the byte offset for each field. */
        pt_do->ui2_fld_offset[ui2_i]=pt_do->ui2_record_width;
            
        /* compute the width of a record entry =>
           individual element size * number of elements
        */
        ui2_sz= db_get_fldtype_size(pt_do->pt_fields[ui2_i].ui4_field_type);
        if ( DB_IS_FIELD_T_ARRAY(pt_do->pt_fields[ui2_i].ui4_field_type) )
        {
            ui2_sz= ui2_sz * pt_do->pt_fields[ui2_i].ui2_field_element_count;
        }
        else
        {
            ui4_ctype = DB_GET_C_TYPE(pt_do->pt_fields[ui2_i].ui4_field_type);
            if ( ui4_ctype == DBF_UNION_T )
            {
                ui2_sz= ui2_sz *
                    pt_do->pt_fields[ui2_i].ui2_field_element_count;
            }
            else if ( ui4_ctype == DBF_STRING_T )
            {
                /* for the string type or membuf type, we will only store
                   the STRING ID that references the actual string
                   or membuf entry. The element count is the maximum number of char
                   allow in the string or the maximum size of the membuf entry.
                */
                if ( pt_do->pt_strbuf == NULL &&
                     x_str_buf_create(0,0,&(pt_do->pt_strbuf)) != STRBUFR_OK  )
                {
                    return DBR_FAIL;
                }
            }
            else if ( ui4_ctype == DBF_MEMBUF_T )
            {
                /* for the membuf type, we will only store OBJECT ID that
                   references the actual membuf entry. The element count is
                   the maximum size allowed for each membuf entry.
               
                   If there record field contains MEMBUF data type
                   entry then create a memory buffer to store .
                */
                if ( pt_do->pt_membuf == NULL &&
                     x_mem_buf_create(0,0,&(pt_do->pt_membuf)) != MEMBUFR_OK )
                {
                    return DBR_FAIL;
                }
            }
            else  /* all other types, set element count to 1. */
            {
                pt_do->pt_fields[ui2_i].ui2_field_element_count=1;
                ui2_sz= ui2_sz *
                    pt_do->pt_fields[ui2_i].ui2_field_element_count;
            }
        }

        /* Sum the width of record up to this field. */
        pt_do->ui2_record_width = pt_do->ui2_record_width + ui2_sz ;
            
        /* Save the individual field width */
        pt_do->ui2_fld_width[ui2_i]=ui2_sz;

        /*
        x_dbg_stmt("field %d  type: %d width: %d offset: %d elem_count: %d\n",
                   ui2_i,
                   ui4_ctype,
                   pt_do->ui2_fld_width[ui2_i],
                   pt_do->ui2_fld_offset[ui2_i],
                   pt_do->pt_fields[ui2_i].ui2_field_element_count );
        */
        
    }
    return DBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: db_get_fld_offset_width
 *
 * Description: Internal API to get the byte offset and the width
 *              (in bytes) for a field within a record.              
 *
 *
 * Inputs:  - pt_do:  database object.
 *            ui4_fld:  field type descriptor
 *            
 * Outputs: - pui2_offset:  the byte offset in record byte array
 *                          for the specified field.
 *            pui2_width:   the width (in bytes) of the specified
 *                          field.
 *
 * Returns: - DBR_OK:  success   
 *            DBR_FAIL: the specified field type is incorrect.   
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 
db_get_fld_offset_width
(
    CDB_DATA_OBJ_T*  pt_do,
    DB_FIELD_T       ui4_fld,
    UINT16*          pui2_offset,
    UINT16*          pui2_width
)
{
    UINT8   ui1_i;

    for ( ui1_i = 0 ; ui1_i < pt_do->ui1_field_count ; ui1_i++ )
    {
        if ( ui4_fld == pt_do->pt_fields[ui1_i].ui4_field_type )
        {
            *pui2_offset = pt_do->ui2_fld_offset[ui1_i];
            *pui2_width  = pt_do->ui2_fld_width[ui1_i];
            return DBR_OK;
        }
    }
    return DBR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: db_copy_field_values
 *
 * Description: API to copy the field values from the record
 *              to a client provided buffer. Note: No check is
 *              done on the client provided buffer. If the client
 *              selected field is not a valid field, no copy is
 *              done.  This is "copy A to B" operation.
 *
 * Inputs:  - 
 *   pt_do:     database object.
 *   pui1_record:  the record containing the fields.
 *   ui1_count:    Number of fields selected.
 *   aui4_field_type_selected:  The field types of record fields selected.
 *
 * Outputs: -
 *   apv_field_value_addr:  Array of address to copy the record field to.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_copy_field_values
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    UINT8		      ui1_count,
    const DB_FIELD_T  aui4_field_type_selected[],
    VOID*             apv_field_value_addr[],
    UINT16            aui2_addr_len[]
)
{
    INT32      i4_rc = DBR_OK ;
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;

    for ( ui1_i=0; ui1_i < ui1_count; ui1_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_field_type_selected[ui1_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            if ( DB_GET_C_TYPE(aui4_field_type_selected[ui1_i])
                 == DBF_STRING_T )
            {
                STRING_ID_T     t_string_id;
                /* the string size is set to maximum len of the
                   a STRING object. On return, this value is replaced
                   with the actual len of string.
                */
                SIZE_T          z_str_sz = 255;
                
                /* If this is a string, then get the actual string
                   from the string ID stored in this field.
                */
                x_memcpy(&t_string_id,(pui1_record+ui2_offset),
                         ui2_width);

                if ( aui2_addr_len != NULL )
                {
                    z_str_sz = aui2_addr_len[ui1_i];
                }
                
                if ( x_str_buf_get_string(pt_do->pt_strbuf,
                                          t_string_id,
                                          (CHAR*) apv_field_value_addr[ui1_i],
                                          &z_str_sz)
                     != STRBUFR_OK  )
                {
                    x_memset(apv_field_value_addr[ui1_i],0,1);
                    x_dbg_stmt("{CDB} db_copy_field_values, get string id: %d failed\n",
                               t_string_id);
                    
                    /* return DBR_FAIL; */
                }

                if ( aui2_addr_len != NULL )
                {
                    aui2_addr_len[ui1_i] = (UINT16)z_str_sz;
                }
            }
            else if ( DB_GET_C_TYPE(aui4_field_type_selected[ui1_i])
                      == DBF_MEMBUF_T )
            {
                OBJECT_ID_T     t_object_id;
                SIZE_T          z_obj_sz;
                
                /* If this is a object, then get the actual object
                   data from the object ID stored in this field.
                */
                x_memcpy(&t_object_id,(pui1_record+ui2_offset),
                         ui2_width);

                if ( aui2_addr_len != NULL )
                {
                    z_obj_sz = aui2_addr_len[ui1_i];
                }
                else
                {
                    continue;
                }
                
                i4_rc = x_mem_buf_get_object(pt_do->pt_membuf,
                                             t_object_id,
                                             apv_field_value_addr[ui1_i],
                                             &z_obj_sz);

                if ( i4_rc == MEMBUFR_OK )
                {
                    i4_rc = DBR_OK ;
                }
                else if ( i4_rc == MEMBUFR_INPUT_BUF_TOO_SMALL )
                {
                    i4_rc = DBR_BUF_TOO_SMALL;
                }
                else
                {
                    i4_rc = DBR_FAIL;
                }

                if ( aui2_addr_len != NULL )
                {
                    aui2_addr_len[ui1_i] = (UINT16)z_obj_sz;
                } 
            }
            else
            {
                /* check input buffer space and copy only up to the space
                   provided by the caller. */
                if ( aui2_addr_len != NULL )
                {
                    if ( ui2_width > aui2_addr_len[ui1_i] )
                    {
                        ui2_width = aui2_addr_len[ui1_i];
                    }
                    else
                    {
                        aui2_addr_len[ui1_i] = ui2_width;
                    }
                }
                
                x_memcpy(apv_field_value_addr[ui1_i],
                         (pui1_record+ui2_offset),
                         ui2_width);
            }
        }
        else
        {
            i4_rc = DBR_FAIL;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_union_tag_value
 *
 * Description: API to get the UNION Tag value for the specified
 *              UNION record field.  Note: for each UNION record
 *              field, the caller must provide a UNION-TAG record
 *              field.  The UNION-TAG record field must be
 *              record preceding the UNION record field that it is
 *              relatedd to. 
 *
 * Inputs:  - 
 *   pt_do:       Database object.
 *   pui1_record: record entry.
 *   ui4_keyfld_type:  field type for the UNION field.
 *   
 * Outputs: -
 *   pe_tag_val:  Tag value for the specified UNION field.
 *
 * Returns: -    
 *   TRUE:     If the specified UNION-TAG value is found.            
 *   FALSE:    The specified UNION-TAG is not found.         
 *            
 ----------------------------------------------------------------------------*/
BOOL
db_get_union_tag_value
( 
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record,
    DB_FIELD_T        ui4_keyfld_type,
    DB_UNION_TAG_T*   pe_tag_val
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;
     
    for ( ui1_i=1; ui1_i < pt_do->ui1_field_count; ui1_i++ )
    {
        if ( pt_do->pt_fields[ui1_i].ui4_field_type ==
				 ui4_keyfld_type )
        {
            /* 
              The preceding record should be the UNION TAG field.
		   	*/
            if ( DB_GET_C_TYPE(pt_do->pt_fields[ui1_i-1].ui4_field_type) 
                 == DBF_UNION_TAG_T 
					)
            {
                ui2_offset = pt_do->ui2_fld_offset[ui1_i-1];
                ui2_width  = pt_do->ui2_fld_width[ui1_i-1];
                x_memcpy(pe_tag_val,
                        (pui1_record+ui2_offset),
                        ui2_width);
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: db_is_record_update_needed
 *
 * Description: API to detemine if the data to update the record field
 *   is the same (x_memcmp) as existing data.  
 *           
 *
 * Inputs:  - 
 *    pt_do:            database object
 *    pui1_record:      memory block of the record fields to be updated
 *    apv_field_values: array of addresses pointing to the record
 *                      field values.
 *
 * Outputs: - None.
 *
 * Returns: -    
 *   TRUE:     Data differ so update is needed.
 *   FALSE:    Data is the same, no update is needed.
 ----------------------------------------------------------------------------*/
static BOOL db_is_record_update_needed
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record,
    const VOID*       apv_field_values[],
    const UINT16      aui2_addr_len[]
)
{
    UINT8             ui1_i;
    UINT16            ui2_offset;
    UINT16            ui2_width;
    
    for ( ui1_i=0; ui1_i < pt_do->ui1_field_count; ui1_i++ )
    {
        ui2_offset=pt_do->ui2_fld_offset[ui1_i];
        ui2_width=pt_do->ui2_fld_width[ui1_i];

        if ( DB_GET_C_TYPE(pt_do->pt_fields[ui1_i].ui4_field_type)
             == DBF_STRING_T
            )
        {
            STRING_ID_T     t_string_id;
            /*
              Compare the existing string ID with input
              string
            */
            x_memcpy(&t_string_id,
                     (pui1_record+ui2_offset),
                     ui2_width);
            if ( STRBUFR_OK !=
                 x_str_buf_cmp_string(pt_do->pt_strbuf,
                                      t_string_id,
                                      (const CHAR*) apv_field_values[ui1_i])
                )
            {
                return TRUE;
            }
        }
        else if ( DB_GET_C_TYPE(pt_do->pt_fields[ui1_i].ui4_field_type)
                  == DBF_MEMBUF_T
                )
        {
            OBJECT_ID_T     t_object_id;
            x_memcpy(&t_object_id,
                     (pui1_record+ui2_offset),
                     ui2_width);
            
            if ( aui2_addr_len == NULL || MEMBUFR_OK !=
                 x_mem_buf_cmp_object(pt_do->pt_membuf,
                                      t_object_id,
                                      apv_field_values[ui1_i],
                                      aui2_addr_len[ui1_i])
                )
            {
                return TRUE;
            }
        } 
        else
        {
            if ( 0 != x_memcmp((pui1_record+ui2_offset),
                               apv_field_values[ui1_i],
                               ui2_width)
                )
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_to_record_entry
 *
 * Description: API to copy the record field values from a client
 *              provided buffer to the database memory buffer. The input
 *              values must be in the same order as specified
 *              by the client when creating the database object.
 *
 * Inputs:  - 
 *    pt_do:            database object
 *    pui1_record:      memory block to store the record fields.
 *    apv_field_values: array of addresses pointing to the record
 *                      field values.
 *    aui2_addr_len:    Specify the length (in bytes) of data in the buffer 
 *                      ref'd by the address entry in 'apv_field_values'
 *    b_new_rec:        flag to indicate if this write create a new
 *                      record.  If this new record, we update the
 *                      internal statistic in the database object.
 *
 * Outputs: - None.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_write_to_record_entry
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    const VOID*       apv_field_values[],
    const UINT16      aui2_addr_len[],
    BOOL              b_new_rec
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;

    if ( b_new_rec != TRUE )
    {
        /*
          Compare existing record to see if update is needed.
        */
        if ( db_is_record_update_needed(pt_do,
                                        pui1_record,
                                        apv_field_values,
                                        aui2_addr_len)
             == FALSE
            )
        {
            DBG_API(("[CDB] [db_write_to_record_entry] is not needed.\n"));
            if (pt_do->ui2_last_mod_count > 0)
            {
                pt_do->ui2_last_mod_count--;
            }
            return DBR_OK;
        }
    }

    for ( ui1_i=0; ui1_i < pt_do->ui1_field_count; ui1_i++ )
    {
        ui2_offset=pt_do->ui2_fld_offset[ui1_i];
        ui2_width=pt_do->ui2_fld_width[ui1_i];
        
        if ( DB_GET_C_TYPE(pt_do->pt_fields[ui1_i].ui4_field_type)
             == DBF_STRING_T
            )
        {
            STRING_ID_T     t_string_id; 
            /*
               If this is a string, then  save the
               actual string to the mini-buffer, and write
               the STRING ID to the record field.

               IF the string has the len of 0, no write
               to the mini-buffer will occur, the t_string_id
               returned will be 0.
            */
            if ( b_new_rec == FALSE )
            { 
                /*
                  Delete the existing string
                */
                x_memcpy(&t_string_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                if ( t_string_id != STR_BUF_NULL_STR_ID )
                {
                    x_str_buf_del_string(pt_do->pt_strbuf,
                                         t_string_id);
                }
            }
            
            if ( x_str_buf_write_string(pt_do->pt_strbuf,
                                        (const CHAR*)apv_field_values[ui1_i],
                                        &t_string_id)
                 == STRBUFR_OK
                )
            {
                /* Save the string ID */
                x_memcpy((pui1_record+ui2_offset),
                         &t_string_id,
                         ui2_width);
            }
            else
            {
                t_string_id = STR_BUF_NULL_STR_ID ;
                x_memcpy((pui1_record+ui2_offset),
                         &t_string_id,
                         ui2_width);
                DBG_ERROR(("{CDB} out of memory, can not store string for CDB record in string buffer.\n"));
            }
        }
        else if ( DB_GET_C_TYPE(pt_do->pt_fields[ui1_i].ui4_field_type)
                  == DBF_MEMBUF_T )
        {
            OBJECT_ID_T     t_object_id; 
            /*
               If this is a client private data, then save the
               data to the mini-buffer, and write
               the Object ID to the record field.

               IF the client data has the len of 0 , then no data
               will be written to the mini-buffer, the t_object_id
               returned will be 0.
            */
            if ( aui2_addr_len == NULL )
            {
                x_dbg_stmt("{CDB} Data length in the user provided buffer missing.\n");
                continue;
            }
                
            if ( b_new_rec == FALSE )
            { 
                /*
                  Delete the existing object
                */
                x_memcpy(&t_object_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                
                if ( t_object_id != MEM_BUF_NULL_OBJ_ID )
                {
                    x_mem_buf_del_object(pt_do->pt_membuf,
                                         t_object_id);
                }
            }
            
            if ( x_mem_buf_write_object(pt_do->pt_membuf,
                                        apv_field_values[ui1_i],
                                        aui2_addr_len[ui1_i],
                                        &t_object_id)
                 == MEMBUFR_OK
                )
            {
                /* Save the object ID */
                x_memcpy((pui1_record+ui2_offset),
                         &t_object_id,
                         ui2_width);
            }
            else
            {
                t_object_id = MEM_BUF_NULL_OBJ_ID ;
                x_memcpy((pui1_record+ui2_offset),
                         &t_object_id,
                         ui2_width);
                DBG_ERROR(("{CDB} out of memory, can not store object for CDB record in string buffer.\n"));
            }
        }
        else
        {
            if ( aui2_addr_len != NULL )
            {
                if ( aui2_addr_len[ui1_i] < ui2_width )
                {
                    ui2_width=aui2_addr_len[ui1_i];
                }
            }
            x_memcpy((pui1_record+ui2_offset),
                     apv_field_values[ui1_i],
                     ui2_width);
        }
    }
    /*
      Update the version number.
    */
    pt_do->ui4_db_version = CDB_INCRM_VER(pt_do->ui4_db_version);
    
    /*
       If this is a new record, then increment the
       total number of record.
    */
    if ( b_new_rec )
    {
        pt_do->ui2_record_count++;
    }
    
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_is_update_record_fld_needed
 *
 * Description: API to determine if the selected record fields 
 *    to be updated contains the same data as new data.
 *
 * Inputs:  - 
 *    pt_do:            database object
 *    pui1_record:      memory block of the record fields.
 *    apv_fld_addrs:    array of addresses pointing to the record
 *                      field values.
 *    aui2_addr_len:    Specify the length (in bytes) of data in the buffer 
 *                      ref'd by the address entry in 'apv_fld_addrs'
 * 
 * Outputs: - None.
 *
 * Returns:    
 *   TRUE:     Data differ so update is needed.
 *   FALSE:    Data is the same, no update is needed.           
 ----------------------------------------------------------------------------*/
static BOOL db_is_update_record_fld_needed
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record,
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_addrs[],
    const UINT16      aui2_addr_len[]
)
{
    INT32      i4_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;

    for ( i4_i=0; i4_i < ui1_fld_count; i4_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_fld_type_selected[i4_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            if ( DB_GET_C_TYPE(aui4_fld_type_selected[i4_i])
                 == DBF_STRING_T
                )
            {
                STRING_ID_T     t_string_id;
                x_memcpy(&t_string_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                
                if ( STRBUFR_OK !=
                     x_str_buf_cmp_string(pt_do->pt_strbuf,
                                          t_string_id,
                                          (const CHAR*)apv_fld_addrs[i4_i])
                    )
                {
                    return TRUE;
                }
                
            }
            else if ( DB_GET_C_TYPE(aui4_fld_type_selected[i4_i])
                        == DBF_MEMBUF_T )
            {
                OBJECT_ID_T     t_object_id;
                x_memcpy(&t_object_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                
                if ( MEMBUFR_OK !=
                     x_mem_buf_cmp_object(pt_do->pt_membuf,
                                          t_object_id,
                                          apv_fld_addrs[i4_i],
                                          aui2_addr_len[i4_i])
                    )
                {
                    return TRUE;
                }
            } 
            else
            {
                if ( 0 != x_memcmp((pui1_record+ui2_offset),
                                   apv_fld_addrs[i4_i],
                                   ui2_width)
                    )
                {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}   

/*-----------------------------------------------------------------------------
 * Name: db_update_record_fld
 *
 * Description: API to write to selected record fields in internal
 *              database memory buffer with values from a client
 *              provided buffer. This API is used to update/change
 *              selected field within a record.
 *
 * Inputs:  - 
 *    pt_do:            database object
 *    pui1_record:      memory block to store the record fields.
 *    apv_field_values: array of addresses pointing to the record
 *                      field values.
 *    aui2_addr_len:    Specify the length (in bytes) of data in the buffer 
 *                      ref'd by the address entry in 'apv_field_addrs'
 *  
 * Outputs: - None.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/

INT32 db_update_record_fld
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record,
    UINT8             ui1_fld_count,
    const DB_FIELD_T  aui4_fld_type_selected[],
    const VOID*       apv_fld_addrs[],
    const UINT16      aui2_addr_len[]
)
{
    INT32      i4_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;


    if ( db_is_update_record_fld_needed(pt_do,
                                        pui1_record,
                                        ui1_fld_count,
                                        aui4_fld_type_selected,
                                        apv_fld_addrs,
                                        aui2_addr_len)
         == FALSE
        )
    {
        DBG_API(("[CDB] [db_update_record_fld] is not needed.\n"));
        if (pt_do->ui2_last_mod_count > 0)
        {
            pt_do->ui2_last_mod_count--;
        }
        return DBR_OK;
    }
    
    for ( i4_i=0; i4_i < ui1_fld_count; i4_i++ )
    {
        if ( db_get_fld_offset_width(pt_do,
                                     aui4_fld_type_selected[i4_i],
                                     &(ui2_offset), &(ui2_width))
             == DBR_OK )
        {
            if ( DB_GET_C_TYPE(aui4_fld_type_selected[i4_i])
                 == DBF_STRING_T
                )
            {
                STRING_ID_T     t_string_id;

                /*
                  Delete the existing string
                */
                x_memcpy(&t_string_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                if ( t_string_id != STR_BUF_NULL_STR_ID )
                {
                    x_str_buf_del_string(pt_do->pt_strbuf,
                                         t_string_id);
                }
                /*
                  Save the actual string to the mini-buffer,
                  and write the STRING ID to the record field.
                */
                if ( x_str_buf_write_string(pt_do->pt_strbuf,
                                            (const CHAR*)apv_fld_addrs[i4_i],
                                            &t_string_id)
                     == STRBUFR_OK
                    )
                {
                    x_memcpy((pui1_record+ui2_offset),
                             &t_string_id,
                             ui2_width);
                }
                else
                {
                    t_string_id = STR_BUF_NULL_STR_ID ;
                    x_memcpy((pui1_record+ui2_offset),
                             &t_string_id,
                             ui2_width);
                    DBG_ERROR(("{CDB} out of memory, can not store string for CDB record in string buffer.\n"));
                }
            }
            else if ( DB_GET_C_TYPE(aui4_fld_type_selected[i4_i])
                      == DBF_MEMBUF_T )
            {
                OBJECT_ID_T     t_object_id; 
                /*************************************************
                  If this is a client private data, then save the
                  data to the mini-buffer, and write
                  the Object ID to the record field.
                **************************************************/

                /*************************************************************
                  If the addr_len array is NULL , then no data
                  will be written to the mini-buffer, the t_object_id
                  returned will be NULL.
                **************************************************************/
                if ( aui2_addr_len == NULL )
                {
                    continue;
                }
                
                /*
                  Delete the existing object
                */
                x_memcpy(&t_object_id,
                         (pui1_record+ui2_offset),
                         ui2_width);
                
                if ( t_object_id != MEM_BUF_NULL_OBJ_ID )
                {
                    x_mem_buf_del_object(pt_do->pt_membuf,
                                         t_object_id);
                }

                /*************************************************************
                  If the new client data has the len of 0 , then no data
                  will be written to the mini-buffer, the t_object_id
                  returned will be MEM_BUF_NULL_OBJ_ID.
                **************************************************************/
                if ( x_mem_buf_write_object(pt_do->pt_membuf,
                                            apv_fld_addrs[i4_i],
                                            aui2_addr_len[i4_i],
                                            &t_object_id)
                     == MEMBUFR_OK )
                {
                    /* Save the object ID */
                    x_memcpy((pui1_record+ui2_offset),
                             &t_object_id,
                             ui2_width);
                }
                else
                {
                    t_object_id = MEM_BUF_NULL_OBJ_ID ;
                    x_memcpy((pui1_record+ui2_offset),
                             &t_object_id,
                             ui2_width);
                    DBG_ERROR(("{CDB} out of memory, can not store object for CDB record in string buffer.\n"));
                }  
            }
            else
            {
                x_memcpy((pui1_record+ui2_offset),
                         apv_fld_addrs[i4_i],
                         ui2_width);
            }
        }
        else
        {
            return DBR_FAIL;
        }
    }
    /*
      Update the version number.
    */
    pt_do->ui4_db_version = CDB_INCRM_VER(pt_do->ui4_db_version);
    
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_same_field_types
 *
 * Description: API to compare two field type arrays that are
 *              used to define a key parameter. If field types 
 *              matches between two arrays, then return TRUE,
 *              else return FALSE.
 *
 * Inputs:  - 
 *
 * Outputs: - None.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
BOOL db_same_field_types
( 
    UINT8		       ui1_count,
    const DB_FIELD_T  aui4_field_type_A[],
    const DB_FIELD_T  aui4_field_type_B[]
)
{
    INT32      i4_i;

    for ( i4_i=0; i4_i < ui1_count; i4_i++ )
    {
        if ( aui4_field_type_A[i4_i] != aui4_field_type_B[i4_i] )
        {
            return FALSE;
        }
    }
    return TRUE;
}


