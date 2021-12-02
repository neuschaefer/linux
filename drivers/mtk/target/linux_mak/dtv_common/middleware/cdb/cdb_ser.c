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
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains in-memory serialization / deserialization related
 *         function implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_ser.h"
#include "cdb/cdb_io_buf.h"
#include "cdb/cdb_cmpres.h"
#include "cdb/cdb_fs.h"
#include "cdb/cdb_record.h"

/*-----------------------------------------------------------------------------
  data declaraions
  ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: db_serialize
 *
 * Description: API to serialize CDB object into a sequence of bytes in memory,
 *              either compressed or not. The output memory is dynamically
 *              allocated.
 *
 * Inputs:  - pt_do:        pointer to the internal CDB database object.
 *          - b_compress:   controls whether or not to compress data
 * 
 * Outputs: - ppv_mem_dump: pointer to the serialization output.
 *          - pui4_len:     pointer to the output buffer length
 *
 * Returns: 
 *    DBR_OK
 *    DBR_INV_ARG
 *    DBR_FAIL
 *    DBR_OUT_OF_MEMORY
 ----------------------------------------------------------------------------*/
INT32 db_serialize
(
    CDB_DATA_OBJ_T*     pt_do,
    BOOL                b_compress,
    VOID**              ppv_mem_dump,
    UINT32*             pui4_len
)
{
    DB_MEM_DUMP_META_DATA_T*    pt_meta;
    DB_MEM_DUMP_COMPR_META_T    t_cmp_meta = {1}; /* end mark */
    CDB_MEM_CURSOR_T            t_mem_cursor;
    DB_IO_BUF*                  pt_io_buf = NULL;
    UINT8*                      pui1_output = NULL;
    UINT8*                      pui1_write_ptr;
    UINT8*                      pui1_rec;
    UINT8*                      pui1_buf;
    HANDLE_T                    h_zip = NULL_HANDLE;
    INT32                       i4_rc;
    UINT32                      ui4_rec_count = 0;
    UINT32                      ui4_crc32;
    SIZE_T                      z_output_len;
    UINT32                      ui4_meta_len;  /*do not include length of common data*/

    if (pt_do == NULL || ppv_mem_dump == NULL || pui4_len == NULL)
    {
        return DBR_INV_ARG;
    }

    do
    {
        /* first prepare for metadata and field definitions */
        ui4_meta_len = (sizeof(DB_MEM_DUMP_META_DATA_T) +
                     (sizeof(DB_FIELD_T) *
                      pt_do->ui1_key_field_count) +
                     (sizeof(DB_FIELD_INFO_T) *
                      pt_do->ui1_field_count));
        *pui4_len = ui4_meta_len + pt_do->ui2_common_size;
        pui1_output = (UINT8 *) x_mem_alloc(*pui4_len);
        if (pui1_output == NULL)
        {
            i4_rc = DBR_OUT_OF_MEMORY;
            *pui4_len = 0;
            break;
        }

        pt_meta = (DB_MEM_DUMP_META_DATA_T *) pui1_output;

        pt_meta->ui4_db_version   = pt_do->ui4_db_version;
        pt_meta->ui1_keyfld_count = pt_do->ui1_key_field_count;
        pt_meta->ui1_fld_count    = pt_do->ui1_field_count;
        pt_meta->ui2_common_sz    = pt_do->ui2_common_size;
        pt_meta->ui4_blocksize    = pt_do->ui2_record_width;
        pt_meta->ui4_block_count  = pt_do->ui2_max_count;
        pt_meta->ui2_record_count = pt_do->ui2_record_count;
        pt_meta->e_mem_policy     = pt_do->t_memory->e_memory_policy;

        if (pt_do->ui1_key_field_count <= MAX_NUM_KEY_PARM_FIELDS)
        {
            x_memcpy(pui1_output + sizeof(DB_MEM_DUMP_META_DATA_T),
                     pt_do->aui4_key_field_types,
                     sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count);
        }
        else
        {
            ABORT(DBG_CAT_INV_OP, DBG_ABRT_INV_STATE);
        }

        x_memcpy(pui1_output + sizeof(DB_MEM_DUMP_META_DATA_T) +
                 sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count,
                 pt_do->pt_fields,
                 sizeof(DB_FIELD_INFO_T) * pt_do->ui1_field_count);

        if (pt_do->ui2_common_size > 0)
        {
            x_memcpy(pui1_output + ui4_meta_len,
                     pt_do->pui1_common_data,
                     pt_do->ui2_common_size);
        }

        /* now initialize compress engine and IO buffer */
        if (b_compress)
        {
            if (db_open_cmpres_eng(pt_do->s_cmpres_eng, &h_zip) != DBR_OK)
            {
                b_compress = FALSE;
            }
        }
        
        if (pt_do->ui2_record_width > DB_IO_BUF_SZ)
        {
            i4_rc = db_create_io_buf(pt_do->ui2_record_width, &pt_io_buf);
        }
        else
        {
            i4_rc = db_create_io_buf(DB_IO_BUF_SZ, &pt_io_buf);
        }

        if (i4_rc != DBR_OK)
        {
            break;
        }

        /* dump the record data */
        db_set_cursor_start(pt_do->t_memory,&t_mem_cursor);
     
        while (1)
        {
            i4_rc = db_get_next_used_block(pt_do->t_memory,
                                           &t_mem_cursor,
                                           &pui1_rec);
            
            if (i4_rc == DBR_OK && pui1_rec != NULL)
            {
                /* Get the addr to write into the io buffer. */
                pui1_buf=db_get_write_ptr(pt_io_buf,
                                          pt_do->ui2_record_width);
                /*
                  If IO buffer is full, then flush the content of
                  IO buffer to memory.
                */
                if (pui1_buf == NULL)
                {
                    i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                               NULL,
                                                               b_compress,
                                                               h_zip,
                                                               0,
                                                               &z_output_len,
                                                               FALSE,
                                                               DB_CMPRES_REC_DATA);
                    if ( i4_rc == DBR_OK )
                    {
                        pui1_output = (UINT8 *) x_mem_realloc(pui1_output,
                                                              *pui4_len + z_output_len);
                        if (pui1_output == NULL)
                        {
                            i4_rc = DBR_OUT_OF_MEMORY;
                            break;
                        }
                        pui1_write_ptr = pui1_output + *pui4_len;
                        *pui4_len += z_output_len;
                        
                        i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                                   pui1_write_ptr,
                                                                   b_compress,
                                                                   h_zip,
                                                                   z_output_len,
                                                                   &z_output_len,
                                                                   TRUE,
                                                                   DB_CMPRES_REC_DATA);
                        if (i4_rc != DBR_OK)
                        {
                            x_dbg_stmt("{CDB} Dump Error: failed to compress.\n");
                            break;
                        }

                        pui1_buf=db_get_write_ptr(pt_io_buf,
                                                  pt_do->ui2_record_width);
                        if ( pui1_buf == NULL )
                        {
                            /* something is wrong. */
                            x_dbg_stmt("{CDB} Dump Error: no space in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }   
                }

                x_memcpy(pui1_buf, pui1_rec, pt_do->ui2_record_width);
                ui4_rec_count++;
            }
            else
            {
                /* No more records left, don'f forget to flush the
                   content of IO buffer to file.. */
                i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                           NULL,
                                                           b_compress,
                                                           h_zip,
                                                           0,
                                                           &z_output_len,
                                                           FALSE,
                                                           DB_CMPRES_REC_DATA);
                if ( i4_rc == DBR_OK )
                {
                    pui1_output = (UINT8 *) x_mem_realloc(pui1_output,
                                                          *pui4_len + z_output_len);
                    if (pui1_output == NULL)
                    {
                        i4_rc = DBR_OUT_OF_MEMORY;
                        break;
                    }
                    pui1_write_ptr = pui1_output + *pui4_len;
                    *pui4_len += z_output_len;

                    i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                               pui1_write_ptr,
                                                               b_compress,
                                                               h_zip,
                                                               z_output_len,
                                                               &z_output_len,
                                                               TRUE,
                                                               DB_CMPRES_REC_DATA);
                    if (i4_rc != DBR_OK)
                    {
                        x_dbg_stmt("{CDB} Dump Error: failed to compress.\n");
                        break;
                    }
                }
                else
                {
                    break;
                }
                
                /* Finish writing all records. */
                break;
            }
        }
        if (i4_rc != DBR_OK)
        {
            break;
        }
        else if (ui4_rec_count != pt_do->ui2_record_count)
        {
            x_dbg_stmt("{CDB} Dump Error: record count not match.\n");
            i4_rc = DBR_FAIL;
            break;
        }

        /* dump the string buffer & memory buffer */
        if (pt_do->pt_strbuf != NULL && pt_do->pt_strbuf->ui2_nb_str > 0)
        {
            UINT32           aui4_byte_to_copy[2];
            UINT8*           aui1_addr_vec[2];

            /*
               CDB records dumped. Now we need to dump the string
               object buffer.

               The string object buffer is stored as
               4 byte size, 4 bytes, n bytes of object string info,
               and n bytes of data.
               */
            x_str_buf_serialize(pt_do->pt_strbuf, aui4_byte_to_copy,
                                aui1_addr_vec);

            if ((aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8) >
                (UINT32) db_get_io_buf_capacity(pt_io_buf))
            {
                /* delete existing IO buffer and create a new one. */
                db_delete_io_buf(pt_io_buf);

                i4_rc = db_create_io_buf(aui4_byte_to_copy[0]+
                                         aui4_byte_to_copy[1]+8,
                                         &pt_io_buf);
            }
            else
            {
                /* clear the existing IO buffer so we can re-use it. */
                i4_rc = db_clear_io_buf(pt_io_buf);
            }
            if (i4_rc != DBR_OK)
            {
                break;
            }

            pui1_buf=db_get_write_ptr(pt_io_buf, 2 * sizeof(UINT32));
            if ( pui1_buf == NULL )
            {
                x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                i4_rc = DBR_FAIL;
                break;
            }
            x_memcpy(pui1_buf, aui4_byte_to_copy, 2*sizeof(UINT32));

            if ( aui4_byte_to_copy[0] > 0 )
            {
                pui1_buf = db_get_write_ptr(pt_io_buf, aui4_byte_to_copy[0]);
                if ( pui1_buf == NULL )
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                    i4_rc = DBR_FAIL;
                    break;
                }
                x_memcpy(pui1_buf, aui1_addr_vec[0], aui4_byte_to_copy[0]);

                pui1_buf = db_get_write_ptr(pt_io_buf, aui4_byte_to_copy[1]);
                if ( pui1_buf == NULL )
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                    i4_rc = DBR_FAIL;
                    break;
                }

                x_memcpy(pui1_buf, aui1_addr_vec[1], aui4_byte_to_copy[1]);
            }

            /* now compress the io buffer and output */
            i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                       NULL,
                                                       b_compress,
                                                       h_zip,
                                                       0,
                                                       &z_output_len,
                                                       FALSE,
                                                       DB_CMPRES_REC_DATA);
            if ( i4_rc == DBR_OK )
            {
                pui1_output = (UINT8 *) x_mem_realloc(pui1_output,
                                                      *pui4_len + z_output_len);
                if (pui1_output == NULL)
                {
                    i4_rc = DBR_OUT_OF_MEMORY;
                    break;
                }
                pui1_write_ptr = pui1_output + *pui4_len;
                *pui4_len += z_output_len;

                i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                           pui1_write_ptr,
                                                           b_compress,
                                                           h_zip,
                                                           z_output_len,
                                                           &z_output_len,
                                                           TRUE,
                                                           DB_CMPRES_STRING_DATA);
                if (i4_rc != DBR_OK)
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to compress.\n");
                    break;
                }
            }
            else
            {
                break;
            }
        } /* end dump string buffer */

        if (pt_do->pt_membuf != NULL && pt_do->pt_membuf->ui2_nb_obj > 0)
        {
            UINT32           aui4_byte_to_copy[2];
            UINT8*           aui1_addr_vec[2];

            /*
              CDB records and string buffer dumped to memory.
              Now serialize the memory object buffer.

              The memory object buffer is stored as
              4 byte size, 4 bytes, n bytes of object info,
              and n bytes of data.
            */
            x_mem_buf_serialize(pt_do->pt_membuf, aui4_byte_to_copy,
                                aui1_addr_vec);

            if ((aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8) >
                (UINT32) db_get_io_buf_capacity(pt_io_buf))
            {
                /* delete existing IO buffer and create a new one. */
                db_delete_io_buf(pt_io_buf);

                i4_rc = db_create_io_buf(aui4_byte_to_copy[0]+
                                         aui4_byte_to_copy[1]+8,
                                         &pt_io_buf);
            }
            else
            {
                /* clear the existing IO buffer so we can re-use it. */
                i4_rc = db_clear_io_buf(pt_io_buf);
            }
            if (i4_rc != DBR_OK)
            {
                break;
            }

            pui1_buf=db_get_write_ptr(pt_io_buf, 2 * sizeof(UINT32));
            if ( pui1_buf == NULL )
            {
                x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                i4_rc = DBR_FAIL;
                break;
            }
            x_memcpy(pui1_buf, aui4_byte_to_copy, 2*sizeof(UINT32));

            if ( aui4_byte_to_copy[0] > 0 )
            {
                pui1_buf = db_get_write_ptr(pt_io_buf, aui4_byte_to_copy[0]);
                if ( pui1_buf == NULL )
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                    i4_rc = DBR_FAIL;
                    break;
                }
                x_memcpy(pui1_buf, aui1_addr_vec[0], aui4_byte_to_copy[0]);

                pui1_buf = db_get_write_ptr(pt_io_buf, aui4_byte_to_copy[1]);
                if ( pui1_buf == NULL )
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to get write pointer.\n");
                    i4_rc = DBR_FAIL;
                    break;
                }

                x_memcpy(pui1_buf, aui1_addr_vec[1], aui4_byte_to_copy[1]);
            }

            /* now compress the io buffer and output */
            i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                       NULL,
                                                       b_compress,
                                                       h_zip,
                                                       0,
                                                       &z_output_len,
                                                       FALSE,
                                                       DB_CMPRES_REC_DATA);
            if ( i4_rc == DBR_OK )
            {
                pui1_output = (UINT8 *) x_mem_realloc(pui1_output,
                                                      *pui4_len + z_output_len);
                if (pui1_output == NULL)
                {
                    i4_rc = DBR_OUT_OF_MEMORY;
                    break;
                }
                pui1_write_ptr = pui1_output + *pui4_len;
                *pui4_len += z_output_len;

                i4_rc = db_flush_io_buf_with_cmpres_to_mem(pt_io_buf,
                                                           pui1_write_ptr,
                                                           b_compress,
                                                           h_zip,
                                                           z_output_len,
                                                           &z_output_len,
                                                           TRUE,
                                                           DB_CMPRES_CUSTOMER_DATA);
                if (i4_rc != DBR_OK)
                {
                    x_dbg_stmt("{CDB} Dump Error: failed to compress.\n");
                    break;
                }
            }
            else
            {
                break;
            }
        } /* end dump memory buffer */

        /* now write end mark generate the CRC */
        pui1_output = (UINT8 *) x_mem_realloc(pui1_output,
                                              *pui4_len +
                                              sizeof(DB_MEM_DUMP_COMPR_META_T) +
                                              4);
        if (pui1_output == NULL)
        {
            i4_rc = DBR_OUT_OF_MEMORY;
            break;
        }
        pui1_write_ptr = pui1_output + *pui4_len;
        x_memcpy(pui1_write_ptr, &t_cmp_meta, sizeof(DB_MEM_DUMP_COMPR_META_T));
        pui1_write_ptr += sizeof(DB_MEM_DUMP_COMPR_META_T);
        *pui4_len += sizeof(DB_MEM_DUMP_COMPR_META_T);

        ui4_crc32 = x_crc32_gen(pui1_output, *pui4_len, 0xFFFFFFFF);
        PUT_UINT32_TO_PTR_BIG_END(ui4_crc32, pui1_write_ptr);

        *pui4_len += 4;

        i4_rc = DBR_OK;
    } while (0);

    if (i4_rc == DBR_OK)
    {
        *ppv_mem_dump = pui1_output;
    }
    else
    {
        if (pui1_output != NULL)
        {
            x_mem_free(pui1_output);
        }
        *ppv_mem_dump = NULL;
        *pui4_len = 0;
    }

    db_delete_io_buf(pt_io_buf);

    if (b_compress)
    {
        db_close_cmpres_eng(h_zip);
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_deserialize
 *
 * Description: API to deserialize CDB object from a previous memory dump.
 *              The memory shall be freed outside.
 *
 * Inputs:  - pt_do:        pointer to the internal CDB database object.
 *          - pv_mem_dump:  pointer to the previous serialization output.
 *          - ui4_len:      memory dump buffer length
 *          - b_check_meta: indicates pt_do whether or not required to have the
 *                          same record definition to deserialize successfully.
 * 
 * Outputs: - None
 *
 * Returns: 
 *    DBR_OK
 *    DBR_INV_ARG
 *    DBR_FAIL
 ----------------------------------------------------------------------------*/
INT32 db_deserialize
(
    CDB_DATA_OBJ_T*     pt_do,
    VOID*               pv_mem_dump,
    UINT32              ui4_len,
    BOOL                b_check_meta
)
{
    DB_MEM_DUMP_META_DATA_T*    pt_meta;
    DB_MEM_DUMP_COMPR_META_T    t_cmp_meta;
    DB_IO_BUF*                  pt_io_buf         = NULL;
    DB_IO_BUF*                  pt_io_uncmprs_buf = NULL;
    DB_IO_BUF*                  pt_io_data        = NULL;
    UINT8*                      pui1_input = (UINT8 *) pv_mem_dump;
    UINT8*                      pui1_write_ptr;
    UINT8*                      pui1_rec;
    UINT8*                      pui1_buf;
    HANDLE_T                    h_zip = NULL_HANDLE;
    INT32                       i4_rc;
    UINT32                      ui4_rec_count;
    UINT32                      ui4_byte_avail;
    UINT32                      ui4_data_len;
    BOOL                        b_compress = FALSE;

    if (pt_do == NULL || pv_mem_dump == NULL)
    {
        return DBR_INV_ARG;
    }

    if (x_crc32_gen(pui1_input, ui4_len, 0xFFFFFFFF) != 0)
    {
        x_dbg_stmt("{CDB} Load Error: CRC failed.\n");
        return DBR_FAIL;
    }

    do
    {
        /* first load metadata and field definitions */

        pt_meta = (DB_MEM_DUMP_META_DATA_T *) pui1_input;

        /* check field definition the same or not */
        i4_rc = DBR_FAIL;
        do
        {
            if (pt_meta->ui1_keyfld_count != pt_do->ui1_key_field_count)
            {
                break;
            }
            if (pt_meta->ui1_fld_count != pt_do->ui1_field_count)
            {
                break;
            }
            pui1_buf = pui1_input + sizeof(DB_MEM_DUMP_META_DATA_T);
            if (x_memcmp(pui1_buf, pt_do->aui4_key_field_types,
                         sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count))
            {
                break;
            }
            pui1_buf += sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count;
            if (x_memcmp(pui1_buf, pt_do->pt_fields,
                         sizeof(DB_FIELD_INFO_T) * pt_do->ui1_field_count))
            {
                break;
            }

            i4_rc = DBR_OK;
        } while (0);

        if (i4_rc != DBR_OK)
        {
            if (b_check_meta)
            {
                x_dbg_stmt("{CDB} Load Error: CDB metadata not the same as given one.\n");
                break;
            }
            else
            {
                DBG_INFO(("{CDB} Overriding field definition.\n"));

                pt_do->ui1_key_field_count = pt_meta->ui1_keyfld_count;
                pt_do->ui1_field_count     = pt_meta->ui1_fld_count;
                pt_do->ui2_record_width    = (UINT16) pt_meta->ui4_blocksize;

                pt_do->pt_fields = (DB_FIELD_INFO_T *) 
                                   x_mem_realloc(pt_do->pt_fields,
                                                 sizeof(DB_FIELD_INFO_T) *
                                                 pt_do->ui1_field_count);
                if (pt_do->pt_fields == NULL)
                {
                    ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
                }

                pt_do->ui2_fld_offset
                    = (UINT16 *) x_mem_realloc(pt_do->ui2_fld_offset,
                                               sizeof(UINT16) *
                                               pt_do->ui1_field_count);
            
                pt_do->ui2_fld_width
                    = (UINT16 *) x_mem_realloc(pt_do->ui2_fld_width,
                                               sizeof(UINT16) *
                                               pt_do->ui1_field_count);

                pui1_buf = pui1_input + sizeof(DB_MEM_DUMP_META_DATA_T);
                if (pt_do->ui1_key_field_count <= MAX_NUM_KEY_PARM_FIELDS)
                {
                    x_memcpy(pt_do->aui4_key_field_types, pui1_buf, 
                             sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count);
                }
                else
                {
                    ABORT(DBG_CAT_INV_OP, DBG_ABRT_INV_STATE);
                }

                pui1_buf += sizeof(DB_FIELD_T) * pt_do->ui1_key_field_count;
                x_memcpy(pt_do->pt_fields, pui1_buf, 
                         sizeof(DB_FIELD_INFO_T) * pt_do->ui1_field_count);

                i4_rc = db_create_record_def(pt_do, pt_do->ui1_field_count,
                                          pt_do->pt_fields);
                if (i4_rc != DBR_OK)
                {
                    x_dbg_stmt("{CDB} Load Error: failed to create record definition.\n");
                    break;
                }
            }
        }
        else
        {
            if ((UINT32) pt_do->ui2_record_width != pt_meta->ui4_blocksize)
            {
                x_dbg_stmt("{CDB} Load Error: CDB record width not the same as given one.\n");
                i4_rc = DBR_FAIL;
                break;
            }
        }

        if (pt_do->t_memory == NULL ||
            pt_do->t_memory->e_memory_policy != pt_meta->e_mem_policy ||
            pt_do->t_memory->ui4_blocksize != pt_meta->ui4_blocksize ||
            pt_do->t_memory->ui4_block_count != pt_meta->ui4_block_count)
        {
            pt_do->ui2_max_count       = (UINT16) pt_meta->ui4_block_count;
            db_delete_memory_manager(pt_do->t_memory);
            i4_rc = db_memory_manager_init(&(pt_do->t_memory),
                                        pt_do->ui2_record_width,
                                        pt_do->ui2_max_count,
                                        pt_meta->e_mem_policy);
            if (i4_rc != DBR_OK)
            {
                x_dbg_stmt("{CDB} Load Error: failed to init memory manager.\n");
                break;
            }
        }
        else
        {
            if ((UINT32) pt_do->ui2_max_count != pt_meta->ui4_block_count)
            {
                x_dbg_stmt("{CDB} Load Error: CDB max count not the same as given one.\n");
                i4_rc = DBR_FAIL;
                break;
            }
            db_remove_all_records(pt_do->t_memory);
        }

        if ( pt_do->pt_strbuf != NULL )
        {
            x_str_buf_del_all_string(pt_do->pt_strbuf);
        }

        if ( pt_do->pt_membuf != NULL )
        {
            x_mem_buf_del_all_objects(pt_do->pt_membuf);
        }

        if (pt_meta->ui2_common_sz > 0)
        {
            pt_do->pui1_common_data
                = (UINT8 *) x_mem_realloc(pt_do->pui1_common_data,
                                          pt_meta->ui2_common_sz);
            if (pt_do->pui1_common_data == NULL)
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEMORY);
            }

            x_memcpy(pt_do->pui1_common_data,
                     pui1_input + (sizeof(DB_MEM_DUMP_META_DATA_T) +
                                   (sizeof(DB_FIELD_T) *
                                    pt_do->ui1_key_field_count) +
                                   (sizeof(DB_FIELD_INFO_T) *
                                    pt_do->ui1_field_count)),
                     pt_meta->ui2_common_sz);
        }
        else if (pt_do->pui1_common_data != NULL)
        {
            x_mem_free(pt_do->pui1_common_data);
            pt_do->pui1_common_data = NULL;
        }

		pt_do->ui4_db_version      = CDB_INCRM_VER(pt_do->ui4_db_version);
        pt_do->ui2_common_size     = pt_meta->ui2_common_sz;
        pt_do->ui2_record_count    = pt_meta->ui2_record_count;

        /* now initialize compress engine and IO buffer */
        if (db_open_cmpres_eng(pt_do->s_cmpres_eng, &h_zip) != DBR_OK)
        {
            b_compress = FALSE;
        }
        else
        {
            b_compress = TRUE;
        }

        ui4_data_len = ui4_len - (sizeof(DB_MEM_DUMP_META_DATA_T) +
                                   (sizeof(DB_FIELD_T) *
                                    pt_do->ui1_key_field_count) +
                                   (sizeof(DB_FIELD_INFO_T) *
                                    pt_do->ui1_field_count) +
                                   pt_do->ui2_common_size) - 4;
        pui1_buf = pui1_input + (sizeof(DB_MEM_DUMP_META_DATA_T) +
                                   (sizeof(DB_FIELD_T) *
                                    pt_do->ui1_key_field_count) +
                                   (sizeof(DB_FIELD_INFO_T) *
                                    pt_do->ui1_field_count) +
                                   pt_do->ui2_common_size);
        while (ui4_data_len > 0)
        {
            x_memcpy(&t_cmp_meta, pui1_buf, sizeof(DB_MEM_DUMP_COMPR_META_T));
            ui4_data_len -= sizeof(DB_MEM_DUMP_COMPR_META_T);
            pui1_buf += sizeof(DB_MEM_DUMP_COMPR_META_T);
            if (t_cmp_meta.ui1_end_flag)
            {
                break;
            }
            if (t_cmp_meta.ui1_compr_flag && !b_compress)
            {
                x_dbg_stmt("{CDB} Load Error: compress engine error.\n");
                i4_rc = DBR_FAIL;
                break;
            }
            if ( pt_io_buf != NULL )
            {
                db_clear_io_buf(pt_io_buf);
                if ((UINT32) db_get_io_buf_capacity(pt_io_buf) <
                    t_cmp_meta.ui4_cmp_data_len)
                {
                    db_delete_io_buf(pt_io_buf);
                    pt_io_buf = NULL;
                    i4_rc = db_create_io_buf(t_cmp_meta.ui4_cmp_data_len,
                                             &pt_io_buf);
                    if ( i4_rc != DBR_OK )
                    {
                        break;
                    }
                }
            }
            else
            {
                /* create IO buf */
                i4_rc = db_create_io_buf(t_cmp_meta.ui4_cmp_data_len,
                                         &pt_io_buf);
                if ( i4_rc != DBR_OK )
                {
                    break;
                }
            }
            if ( pt_io_uncmprs_buf != NULL )
            {
                db_clear_io_buf(pt_io_uncmprs_buf);
                if ((UINT32) db_get_io_buf_capacity(pt_io_uncmprs_buf) <
                    t_cmp_meta.ui4_uncmp_data_len)
                {
                    db_delete_io_buf(pt_io_uncmprs_buf);
                    pt_io_uncmprs_buf = NULL;
                    i4_rc = db_create_io_buf(t_cmp_meta.ui4_uncmp_data_len,
                                             &pt_io_uncmprs_buf);
                    if ( i4_rc != DBR_OK )
                    {
                        break;
                    }
                }
            }
            else
            {
                /* create IO buf for storing un-compressed data */
                i4_rc = db_create_io_buf(t_cmp_meta.ui4_uncmp_data_len,
                                         &pt_io_uncmprs_buf);
                if ( i4_rc != DBR_OK )
                {
                    break;
                }
            }
            pui1_write_ptr = db_get_write_ptr(pt_io_buf,
                                              t_cmp_meta.ui4_cmp_data_len);
            if ( pui1_write_ptr == NULL )
            {
                /* something is wrong. */
                x_dbg_stmt("{CDB} Load Error: no space in IO buffer.\n");
                i4_rc = DBR_FAIL;
                break;
            }
            x_memcpy(pui1_write_ptr, pui1_buf, t_cmp_meta.ui4_cmp_data_len);
            ui4_data_len -= t_cmp_meta.ui4_cmp_data_len;
            pui1_buf += t_cmp_meta.ui4_cmp_data_len;
            if (t_cmp_meta.ui1_compr_flag)
            {
                i4_rc = db_uncmpres_data(h_zip,
                                         pt_io_buf,
                                         pt_io_uncmprs_buf,
                                         t_cmp_meta.ui4_uncmp_data_len);
                if (i4_rc != DBR_OK)
                {
                    x_dbg_stmt("{CDB} Load Error: decompress failed.\n");
                    break;
                }
                pt_io_data = pt_io_uncmprs_buf;
            }
            else
            {
                pt_io_data = pt_io_buf;
            }
            
            switch (t_cmp_meta.ui1_payload_type)
            {
                case DB_CMPRES_REC_DATA:
                    {
                        ui4_rec_count =
                            (UINT32) db_get_io_buf_data_len(pt_io_data) /
                            pt_do->ui2_record_width;
                        while (ui4_rec_count-- > 0)
                        {
                            pui1_rec = db_get_read_ptr(pt_io_data,
                                                       pt_do->ui2_record_width,
                                                       &(ui4_byte_avail));
                            if (pui1_rec == NULL ||
                                ui4_byte_avail != pt_do->ui2_record_width)
                            {
                                x_dbg_stmt("{CDB} Load Error: no enough space for record in IO buffer.\n");
                                i4_rc = DBR_FAIL;
                                break;
                            }
                            i4_rc = db_get_next_freeblock(pt_do->t_memory,
                                                          &pui1_write_ptr);
                            if (i4_rc != DBR_OK)
                            {
                                break;
                            }
                            
                            x_memcpy(pui1_write_ptr, pui1_rec,
                                     pt_do->ui2_record_width);
                        }

                    }
                    break;

                case DB_CMPRES_STRING_DATA:
                    {
                        UINT32*          pui4_byte_to_copy;
                        UINT8*           aui1_addr_vec[2];

                        pui4_byte_to_copy
                            = (UINT32 *) db_get_read_ptr(pt_io_data,
                                                         2 * sizeof(UINT32),
                                                         &(ui4_byte_avail));

                        if (pui4_byte_to_copy == NULL ||
                            ui4_byte_avail != 2 * sizeof(UINT32))
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for string buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        aui1_addr_vec[0] = db_get_read_ptr(pt_io_data,
                                                   pui4_byte_to_copy[0],
                                                   &(ui4_byte_avail));
                        if (aui1_addr_vec[0] == NULL ||
                            ui4_byte_avail != pui4_byte_to_copy[0])
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for string buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        aui1_addr_vec[1] = db_get_read_ptr(pt_io_data,
                                                   pui4_byte_to_copy[1],
                                                   &(ui4_byte_avail));
                        if (aui1_addr_vec[1] == NULL ||
                            ui4_byte_avail != pui4_byte_to_copy[1])
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for string buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }

                        x_str_buf_free(pt_do->pt_strbuf);
                        pt_do->pt_strbuf =
                            x_str_buf_create_from_byte_stream(pui4_byte_to_copy,
                                                              aui1_addr_vec);
                        if (pt_do->pt_strbuf == NULL)
                        {
                            x_dbg_stmt("{CDB} Load Error: failed to create string buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        i4_rc = DBR_OK;
                    }
                    break;

                case DB_CMPRES_CUSTOMER_DATA:
                    {
                        UINT32*          pui4_byte_to_copy;
                        UINT8*           aui1_addr_vec[2];

                        pui4_byte_to_copy
                            = (UINT32 *) db_get_read_ptr(pt_io_data,
                                                         2 * sizeof(UINT32),
                                                         &(ui4_byte_avail));

                        if (pui4_byte_to_copy == NULL ||
                            ui4_byte_avail != 2 * sizeof(UINT32))
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for memory buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        aui1_addr_vec[0] = db_get_read_ptr(pt_io_data,
                                                   pui4_byte_to_copy[0],
                                                   &(ui4_byte_avail));
                        if (aui1_addr_vec[0] == NULL ||
                            ui4_byte_avail != pui4_byte_to_copy[0])
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for memory buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        aui1_addr_vec[1] = db_get_read_ptr(pt_io_data,
                                                   pui4_byte_to_copy[1],
                                                   &(ui4_byte_avail));
                        if (aui1_addr_vec[1] == NULL ||
                            ui4_byte_avail != pui4_byte_to_copy[1])
                        {
                            x_dbg_stmt("{CDB} Load Error: no enough space for memory buffer in IO buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }

                        x_mem_buf_free(pt_do->pt_membuf);
                        pt_do->pt_membuf =
                            x_mem_buf_create_from_byte_stream(pui4_byte_to_copy,
                                                              aui1_addr_vec);
                        if (pt_do->pt_membuf == NULL)
                        {
                            x_dbg_stmt("{CDB} Load Error: failed to create memory buffer.\n");
                            i4_rc = DBR_FAIL;
                            break;
                        }
                        i4_rc = DBR_OK;
                    }
                    break;

                default:
                    x_dbg_stmt("{CDB} Load Error: unknown payload type %d.\n",
                               t_cmp_meta.ui1_payload_type);
                    i4_rc = DBR_FAIL;
                    break;
            }
        }
        if (i4_rc != DBR_OK)
        {
            break;
        }

        if (ui4_data_len != 0)
        {
            x_dbg_stmt("{CDB} Load Error: wrong data length.\n");
            i4_rc = DBR_FAIL;
            break;
        }

        if (pt_do->t_memory->ui4_record_count != pt_do->ui2_record_count)
        {
            x_dbg_stmt("{CDB} Dump Error: record count not match.\n");
            i4_rc = DBR_FAIL;
            break;
        }
    } while (0);


    db_delete_io_buf(pt_io_buf);
    db_delete_io_buf(pt_io_uncmprs_buf);

    if (b_compress)
    {
        db_close_cmpres_eng(h_zip);
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_flush_io_buf_with_cmpres_to_mem
 *
 * Description: API to write to IO buffer to the specified memory location.
 * 
 *   This function also support compression of data before writing to
 *   memory, and compute the length of compressed data to be written
 *   out.
 *
 *   This function is placed here instead of cdb_io_buf because it uses
 *   DB_MEM_DUMP_COMPR_META_T for compression metadata. It differs from the
 *   function in cdb_io_buf also on that CRC value is not calculated, and the
 *   output buffer length of free bytes is also given. Also the function will
 *   not proceed if the compression failed, and in no write mode, io buffer will
 *   not be cleared.
 *
 * Inputs:  - 
 *    pt_io_buf:   Pointer to IO buffer structure.
 *    pui1_buf:    Pointer to the output buffer.
 *    b_write:     FALSE: Only compute how much data will be written out, but
 *                 don't actually write the data.
 *                 TRUE:  Write out the data.
 *    z_buffer_len: length of the output buffer
 *    h_zip:       handle to the compress engine
 *    ui1_payload_type:  Caller specified tag to indicate the
 *                       type of data that is flused out.
 * Outputs: -
 *    pz_byte_output:   Bytes that have been flushed out.
 *
 * Returns: -    
 *    DBR_BUF_TOO_SMALL Output buffer too small            
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_flush_io_buf_with_cmpres_to_mem
(
    DB_IO_BUF*        pt_io_buf,
    UINT8*            pui1_buf,
    BOOL              b_cmpres,
    HANDLE_T          h_zip,
    SIZE_T            z_buffer_len,
    SIZE_T*           pz_byte_output,
    BOOL              b_write,
    UINT8             ui1_payload_type
)
{
    INT32             i4_rc;
    SIZE_T            z_uncmp_data_len;
    SIZE_T            z_cmp_data_len;
    DB_MEM_DUMP_COMPR_META_T    t_cmp_meta = {0};
    
    
    i4_rc=DBR_OK;

    *pz_byte_output  = 0 ;
    z_uncmp_data_len = pt_io_buf->ui4_write_ptr;
    z_cmp_data_len   = pt_io_buf->ui4_write_ptr;  

    if (z_uncmp_data_len == 0)
    {
        *pz_byte_output = 0;
        return DBR_OK;
    }

    if (pui1_buf == NULL)
    {
        b_write = FALSE;
    }

    if ( b_write )
    {
        if ( b_cmpres )
        {
            i4_rc = db_cmpres_data(h_zip,
                                   pt_io_buf,
                                   &z_uncmp_data_len,
                                   &z_cmp_data_len);

        }

        if (i4_rc == DBR_OK)
        {
            if (z_buffer_len >= sizeof(DB_MEM_DUMP_COMPR_META_T)+z_cmp_data_len)
            {
                t_cmp_meta.ui1_end_flag     = (UINT8) FALSE;
                t_cmp_meta.ui1_compr_flag   = (UINT8) b_cmpres;
                t_cmp_meta.ui1_payload_type = ui1_payload_type;
                t_cmp_meta.ui4_cmp_data_len = (UINT32) z_cmp_data_len;
                t_cmp_meta.ui4_uncmp_data_len = (UINT32) z_uncmp_data_len;

                x_memcpy(pui1_buf,&t_cmp_meta,sizeof(DB_MEM_DUMP_COMPR_META_T));
                pui1_buf += sizeof(DB_MEM_DUMP_COMPR_META_T);
                x_memcpy(pui1_buf, pt_io_buf->pui1_buf, z_cmp_data_len);
                *pz_byte_output =
                    sizeof(DB_MEM_DUMP_COMPR_META_T) + z_cmp_data_len;

                /* Reset the write buffer. */
                pt_io_buf->ui4_write_ptr=0;

                /* clear out the buffer, we need to do this because for the
                   UNION field in the CDB record, not all bytes are set by
                   the endian conversion function, so there could be some
                   left over data in the record from previous processing.
                   */
                x_memset(pt_io_buf->pui1_buf,0,pt_io_buf->ui4_total_space);
            }
            else
            {
                i4_rc = DBR_BUF_TOO_SMALL;
                /* note that the content in the IO buffer have been altered. */
            }
        }
    }
    else
    {
        if ( b_cmpres )
        {
            i4_rc = db_get_cmpres_data_len_info(h_zip,
                                                pt_io_buf,
                                                &z_uncmp_data_len,
                                                &z_cmp_data_len);
        }
        if ( i4_rc == DBR_OK )
        {
            *pz_byte_output = z_cmp_data_len + sizeof(DB_MEM_DUMP_COMPR_META_T);
        }
    }
    
    return i4_rc;
}
