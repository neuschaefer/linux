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
 * $RCSfile: cdb_fs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of CDB fileystem I/O functions.
 *         
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "util/x_end_cnvt.h"
#include "util/x_crc32.h"
#include "os/inc/x_os.h"
#include "rwlock/rwl_api.h"
#include "cdb/cdb_api.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_membuf.h"
#include "cdb/cdb_record.h"
#include "cdb/cdb_fs.h"
#include "cdb/cdb_io_buf.h"
#include "cdb/cdb_cmpres.h"
#include "file_mngr/x_fm.h"
#include "cdb/cdb_dbg.h"
/*-----------------------------------------------------------------------------
  Static function prototype declaraions
-------------------------------------------------------------------------*/
/*
  Utility function to read the beginning 4 bytes of data from a
  file.  Before calling this function, the file pointer must be
  pointing at the start of 4 byte magic number.  After return
  from this API, the file pointer position will remain the same.
*/
static INT32 db_get_beg_magic_num_for_write_ops
(
    HANDLE_T          h_file,
    UINT32*           pui4_beg_magic_num
);

/*
  Utility function to read the end magic and CRC values. Before
  calling this API, the file pointer is position at the start
  of data section.  The 'ui4_data_sz' indicates the number of
  bytes to skip in-order to read the end-magic number and CRC
  value.  After return from this API, the file pointer position
  will remain the same.
*/
static INT32 db_get_end_magic_num_for_read_ops
(
    HANDLE_T          h_file,
    UINT32            ui4_data_sz,
    UINT32*           pui4_end_magic_num
);

/*
  Utility function to write the CRC value and end-magic number. Before
  calling this API, the file pointer is position at end 
  of data section.  This API will then write out another 8 bytes (CRC
  and Magic number).

  After return from this API, the file pointer position will be advanced
  by 8 bytes.
*/
static INT32 db_write_file_magic_crc_num
(
    HANDLE_T          h_file,
    UINT32            ui4_magic,
    UINT32            ui4_crc 
);

static INT32 db_write_file_format_info
(
    CDB_DATA_OBJ_T*   pt_do,
    HANDLE_T          h_file,
    UINT32            ui4_magic,
    UINT32*           pui4_crc
);

static INT32 db_write_meta
(
    CDB_DATA_OBJ_T*      pt_do,
    HANDLE_T             h_file,
    db_endian_conv_fct   pf_conv,
    db_cdata_conv_fct    pf_cdata_conv,
    BOOL                 b_cmpres,
    UINT32*              pui4_crc
);


static INT32 db_write_data_sect
(
    CDB_DATA_OBJ_T*            pt_do,
    HANDLE_T                   h_file,
    db_endian_conv_fct         pf_conv,
    UINT32*                    pui4_crc,
    BOOL                       b_cmpress,
    BOOL                       b_write,
    SIZE_T*                    pz_data_len
);


static INT32 db_get_metadata_from_file
(  
    HANDLE_T             h_file,
    db_cdata_conv_fct    pf_cdata_conv,
    DB_FILE_META_DATA_T* pt_meta,
    UINT8*               pui1_file_ver,
    UINT32*              pui4_crc
);

static UINT16 db_get_meta_sect_size 
( 
    const CDB_DATA_OBJ_T*  pt_do
);

#if 0
static UINT32 db_get_data_sect_size 
( 
    const CDB_DATA_OBJ_T*  pt_do
);
#endif

static UINT32 db_get_data_sect_size_v2
( 
    CDB_DATA_OBJ_T*        pt_do,
    db_endian_conv_fct     pf_conv,
    BOOL                   b_cmpres 
);


static INT32 db_rec_to_big_endian
( 
    const CDB_DATA_OBJ_T*  pt_do,
    const UINT8*           pui1_rec,
    UINT8*                 pui1_buf,
    db_endian_conv_fct     pf_conv
);

static INT32 db_recfld_from_big_end_to_host_end
(
    CDB_DATA_OBJ_T*        pt_do,
    INT32                  i4_i,
    UINT8*                 pui1_in_mem_rec,
    DB_FILE_META_DATA_T*   pt_meta,
    INT32                  i4_j,
    const UINT8*           pui1_fsys_rec,
    db_endian_conv_fct     pf_conv
);

static INT32 db_parse_meta
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    UINT8                 ui1_cdb_ver,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
);

static INT32  db_free_metabuf
(
    DB_FILE_META_DATA_T* pt_meta
);

static
INT32  db_init_metabuf
(
    DB_FILE_META_DATA_T* pt_meta
);

static INT32 db_load_metadata_ver_1
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
);

static INT32 db_load_metadata_ver_2
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
);

static INT32 db_load_metadata_ver_3
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
);


static VOID _mem_buf_convert_old
(
    UINT32                ui4_buf_sz,
    UINT16                ui2_number,
    UINT32*               pui4_ary,
    UINT8*                pui1_buf,
    UINT32                ui4_check_sz,
    UINT32*               pui4_mem_used
);

static INT32 db_load_rec_from_file
(
    HANDLE_T              h_file,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    UINT32*               pui4_crc
);

static INT32  db_load_data_obj_from_file
(
    HANDLE_T              h_file,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    UINT32*               pui4_crc
);

static INT32 db_add_rec_to_db
(
    UINT8*                pui1_buf,
    DB_FILE_META_DATA_T*  pt_meta,
    HANDLE_T              h_cdb,
    CDB_DATA_OBJ_T*       pt_do,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    VOID*                 apv_recfld_addrs[],
    UINT8*                pui1_tmp
);

static
BOOL db_check_read_write_lock
(
    HANDLE_T         h_file
);

static
BOOL db_check_write_lock
(
    HANDLE_T         h_file
);

static INT32 db_fs_write_to_record_entry
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    const VOID*       apv_field_values[],
    BOOL              b_new_rec
);

static INT32 db_fs_write_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[]
);

/*-----------------------------------------------------------------------------
 * Name: db_call_client_write_fct
 *
 * Description: API that calls client's function that perform
 *    field modification before the field is write out to
 *    persistent storage.
 *
 * Inputs:  -
 *
 *    pv_rec_obj:      An opaque object that contains current
 *                     CDB record information.  
 *
 *    t_field_name:    Current CDB field name of the field in record being
 *                     processed
 *
 *    ui4_buf_len:     length of 'pui1_buf'
 * 
 * Outputs: -
 *
 *    pui1_buf:        buffer for converted field data in big-endian format. 
 *   
 *
 * Returns: -    
 *    0 :    No modification
 *    1 :    Write/modification success.
 *            
 ----------------------------------------------------------------------------*/
INT32 db_call_client_write_fct
(
    VOID*                  pv_rec_obj,
    DB_FIELD_T             t_field_name,
    UINT8*                 pui1_buf,
    UINT32                 ui4_buf_len
)
{
    CDB_OPAQUE_OBJ_T*      pt_obj;
    CDB_DATA_OBJ_T*        pt_do;
    INT32                  i4_rc=0;

    pt_obj = (CDB_OPAQUE_OBJ_T*) pv_rec_obj;
    pt_do = pt_obj->pt_do;

    if ( pt_do->pf_client_store_fct != NULL )
    {
        i4_rc = pt_do->pf_client_store_fct(pv_rec_obj,
                                           t_field_name,
                                           pui1_buf,
                                           ui4_buf_len);
    }

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_cdb_to_file
 *
 * Description: This API writes the CDB object to file.
 *
 * Inputs:  - pt_do:   Pointer to the CDB control block structure. 
 *
 * Outputs: - None.
 *
 * Returns: - Size of the meta data section.
 *
 ----------------------------------------------------------------------------*/

INT32 db_write_cdb_to_file
( 
    CDB_DATA_OBJ_T*     pt_do,
    HANDLE_T            h_file,
    db_endian_conv_fct  pf_conv,
    db_cdata_conv_fct   pf_cdata_conv,
    BOOL                b_cmpres
)
{
    UINT32  ui4_magic_num;
    INT32   i4_rc=DBR_FILE_WRITE_FAIL;
    UINT32  ui4_crc;
    BOOL    b_result;
    UINT32  ui4_obj_sz;
    
    /*
      check if write lock has been grabbed.
    */
    db_check_write_lock(h_file);

    if ( db_is_cdb_file_need_saving(pt_do, h_file, pf_conv, b_cmpres, &b_result, &ui4_obj_sz)
         == DBR_OK )
    {
        if ( b_result == FALSE )
        {
            UINT64   ui8_fpos;
            UINT64   ui8_x_fpos;
            
            /*
              Before returning, need to move the file pointer to end of
              this CDB object.  This is because the upper layer expect
              the file position will be at the end of object location
              in the file after call to write function.
            */
            /* ui8_fpos=db_persistent_obj_size(pt_do, pf_conv, b_cmpres); */

            ui8_fpos = ui4_obj_sz ;

            if (  x_fm_lseek(h_file, (INT64) ui8_fpos, FM_SEEK_CUR, &ui8_x_fpos) != FMR_OK )
            {
                /*
                  Fail to re-set to the original position of the file pointer,
                  return WRITE_FAIL so no write will be performed.
                */
                i4_rc = DBR_FILE_WRITE_FAIL;
                
                x_dbg_stmt("{CDB} Error: [db_write_cdb_to_file] can not set file position to end of CDB object: %lld",
                           ui8_fpos);

                
            }
            else
            {
                /* No file saving is needed. */
                DBG_INFO(("{CDB} [db_write_cdb_to_file] obj ref by handle (%d) in memory has not change, no write op is needed.\n",
                         pt_do->h_owner));
                
                i4_rc = DBR_OK;
            }
            return i4_rc;
        }
    }
    else
    {
        /*
           Some thing wrong with FILE ops, can not determine if
           writing operation is needed or not, so return write
           error status.
        */
        
        x_dbg_stmt("{CDB} Error: [db_write_cdb_to_file] can not determine the status of file for write ops\n");
        
        return i4_rc;
    }
 
    DBG_INFO(("{CDB} [db_write_cdb_to_file] obj ref by handle (%d) in memory has changed, write op is needed.\n",
             pt_do->h_owner));
    
    /*
      Read the first 4 bytes to get the current magic number from
      the CDB object store in file.
    */
    if ( db_get_beg_magic_num_for_write_ops(h_file, &ui4_magic_num)
         == DBR_OK
        )
    {
        /*
          The magic number should be toggled between one of the two. If not,
          this is a new position, so we can just select one.
        */
        if ( ui4_magic_num == DB_MAGIC_NUMBER        ||
             ui4_magic_num == ~DB_MAGIC_NUMBER
            )
        {
            /* flip of the magic number for over-write */
            ui4_magic_num = ~ui4_magic_num;
        }
        else
        {
            /* new write ops, just pick one. */
            ui4_magic_num = DB_MAGIC_NUMBER;
        }

        /*
          Initialized the CRC number
        */
        ui4_crc = 0xFFFFFFFF;
        
        /*
          Write file format info, meta data, and data section
        */
        if ( db_write_file_format_info(pt_do, h_file, ui4_magic_num, &ui4_crc)
             == DBR_OK
            )
        {
            if ( db_write_meta(pt_do, h_file, pf_conv, pf_cdata_conv, b_cmpres, &ui4_crc) == DBR_OK )
            {
                SIZE_T    z_data_len;
                
                if ( db_write_data_sect(pt_do, h_file, pf_conv, &ui4_crc, b_cmpres, TRUE, &z_data_len)
                     == DBR_OK )
                {
                    /*
                      write the end magic and CRC number to
                      indicate the write has completed successfully.
                    */
                    if ( db_write_file_magic_crc_num(h_file,
                                                     (~ui4_magic_num),
                                                     ui4_crc)
                         == DBR_OK )
                    {                      
                        DBG_INFO(("{CDB::FileIO} CRC value save to Filesystem: [%x]\n",
                                 ui4_crc));
                        i4_rc = DBR_OK;
                    }
                }
            }
        }
    }
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_get_meta_sect_size
 *
 * Description: This API compute the size (in bytes) of the metadata section
 *              in the CDB object to be written to file.
 *
 * Inputs:  - pt_do:   Pointer to the CDB control block structure. 
 *
 * Outputs: - None.
 *
 * Returns: - Size of the meta data section.
 *
 ----------------------------------------------------------------------------*/
static UINT16 db_get_meta_sect_size 
( 
    const CDB_DATA_OBJ_T*  pt_do
)
{
    UINT16  ui2_sz;
    UINT8   ui1_file_ver ;
    
    ui1_file_ver = CDB_FILE_VER ;
    
    switch (ui1_file_ver)
    {
        case 1 :
        {
            ui2_sz = (UINT16) (
                1 +                                     /* ui1_key_field_count */
                pt_do->ui1_key_field_count * 4 +        /* aui4_key_field_types[] */
                1 +                                     /* ui1_field_count */
                pt_do->ui1_field_count * (6 + 2 + 2 ) + /*
                                                          aui4_field_type,
                                                          aui2_field_element_count,
                                                          aui2_field_offset,
                                                          aui2_field_width
                                                        */
                2 +                                     /* ui2_max_count */
                2 +                                     /* ui2_record_count */
                2 +                                     /* ui2_record_width */
                4 +                                     /* MEMORY_POLICY enum */
                2 +                                     /* ui2_common_size */
                pt_do->ui2_common_size +                /* pui1_common_data[] */
                4 ) ;                                   /* ui4_data_size. */
            
            /* x_dbg_stmt("{CDB} metadata size (version 1): %d\n",ui2_sz); */
        }
        break;

        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        {
            ui2_sz = (UINT16) (
                4 +                                     /* ui4_db_version */
                1 +                                     /* ui1_key_field_count */
                pt_do->ui1_key_field_count * 4 +        /* aui4_key_field_types[] */
                1 +                                     /* ui1_field_count */
                pt_do->ui1_field_count * (6 + 2 + 2 ) + /*
                                                          aui4_field_type,
                                                          aui2_field_element_count,
                                                          aui2_field_offset,
                                                          aui2_field_width
                                                        */
                2 +                                     /* ui2_max_count */
                2 +                                     /* ui2_record_count */
                2 +                                     /* ui2_record_width */
                4 +                                     /* MEMORY_POLICY enum */
                2 +                                     /* ui2_common_size */
                pt_do->ui2_common_size +                /* pui1_common_data[] */
                4 ) ;                                   /* ui4_data_size. */

            /* x_dbg_stmt("{CDB} metadata size (version 2): %d\n",ui2_sz); */
        }
        break;

        default:
        {
            ui2_sz = 0 ;
        }
        break;
    }       
    
    return ui2_sz;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name: db_get_data_sect_size
 *
 * Description: This API compute the size (in bytes) of the record data 
 *              section in the CDB file.
 *
 * Inputs:  - pt_do:   Pointer to the CDB control block structure. 
 *
 * Outputs: - None.
 *
 * Returns: - Size of the record data section.
 *
 ----------------------------------------------------------------------------*/

UINT32 db_get_data_sect_size 
( 
    const CDB_DATA_OBJ_T*  pt_do
)
{
    UINT32                 ui4_sz = 0;
        
    ui4_sz = (pt_do->ui2_record_count * pt_do->ui2_record_width) ;
        

    if ( pt_do->pt_strbuf != NULL )
    {
        ui4_sz = ui4_sz + x_str_buf_persistent_sz(pt_do->pt_strbuf);
    }
    else
    {
        ui4_sz = ui4_sz + 8 ;
    }

    ui4_sz = ui4_sz + x_mem_buf_persistent_sz(pt_do->pt_membuf);
    
    return ui4_sz;
}
#endif

/*-----------------------------------------------------------------------------
 * Name: db_write_file_magic_crc_num
 *
 * Description: API to write the CDB file magic number and CRC value at
 *              end of the object.
 *              
 * Inputs:   
 *    h_file:   Handle to the file object to write to.
 *    ui4_magic File object magic number.
 *    ui4_crc   CRC value.
 *
 * Outputs: - None.
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
INT32 
db_write_file_magic_crc_num
(
    HANDLE_T          h_file,
    UINT32            ui4_magic,
    UINT32            ui4_crc
)
{
    
    INT32   i4_rc=DBR_FAIL;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[CDB_FILE_TAIL_MARKER_SIZE] ;
    
    /* convert to Big-Endian. */
    PUT_UINT32_TO_PTR_BIG_END(ui4_crc, &(aui1_buf[0]) );

    x_end_cnvt_to_big_32(&ui4_magic, 1, &(aui1_buf[4]) );
    
    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    
    if ( i4_rc == FMR_OK                    &&
         sizeof(aui1_buf) == ui4_byte_wrote
        )
    {
        /* Number of bytes written should equals number
           of bytes in the buffer.
        */
        i4_rc=DBR_OK;
    }
    else
    {
        i4_rc=DBR_FILE_WRITE_FAIL ;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_file_format_info
 *
 * Description: API to write the CDB file format info
 *              (in Big-Endian format).
 * Inputs:   
 *    pt_do:    Database object.
 *    ui4_magic Magic number to write to header section of the CDB object.
 *    h_file:   Handle to the file object to write to.
 *    pui4_crc: Input CRC32 value
 *
 * Outputs: -
 *    pui4_crc: CRC value computed over the file head data.
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_write_file_format_info
(
    CDB_DATA_OBJ_T*   pt_do,
    HANDLE_T          h_file,
    UINT32            ui4_magic,
    UINT32*           pui4_crc
)
{
    /*
      The file format info consists of total 5 bytes:

      UINT32:   magic number
      UINT8     CDB file version number
      UINT16    Meta data size
    */    
    
    UINT8   ui1_cdb_ver;
    UINT16  ui2_meta_data_sz;
    INT32   i4_rc=DBR_FAIL;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[CDB_FILE_HEADER_SIZE] ;
    UINT32  ui4_crc;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_magic, 1, &(aui1_buf[0]) );
    
    /* The current CDB library version number. */
    ui1_cdb_ver=CDB_FILE_VER;
    x_memcpy(&(aui1_buf[4]), &ui1_cdb_ver, 1);
    
    /* The metadata size. */
    ui2_meta_data_sz=db_get_meta_sect_size(pt_do);
    x_end_cnvt_to_big_16(&ui2_meta_data_sz, 1, &(aui1_buf[5]) );

    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    if ( i4_rc == FMR_OK )
    {
        /* Number of bytes written should equals number
           of bytes in the buffer.
        */
        if ( sizeof(aui1_buf) == ui4_byte_wrote )
        {
            /*
              Compute the CRC 32 value over the CDB file format buffer
              (e.g., file header data).
            */
            if ( pui4_crc != NULL )
            {
                ui4_crc = x_crc32_gen(aui1_buf, ui4_byte_wrote, (*pui4_crc));
                *pui4_crc = ui4_crc;
            }
            
            i4_rc=DBR_OK;
        }
        else
        {
            i4_rc=DBR_FILE_WRITE_FAIL ;
        }
    }
    else
    {
        i4_rc=DBR_FILE_WRITE_FAIL ;
    }
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_write_meta
 *
 * Description: API to write the CDB file format section and meta data
 *              section data in Big-Endian format.
 *             
 * Inputs:   
 *    pt_do:    Database object.
 *    h_file:   Handle to the file object to write to.
 *    pui4_crc: Initali CRC value to be used for computing over the
 *              meta data section.
 *
 * Outputs:
 *    pui4_crc: CRC value computed over the file head data.
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_write_meta
(
    CDB_DATA_OBJ_T*      pt_do,
    HANDLE_T             h_file,
    db_endian_conv_fct   pf_conv,
    db_cdata_conv_fct    pf_cdata_conv,
    BOOL                 b_cmpres,
    UINT32*              pui4_crc
)
{
    UINT16  ui2_i;
    UINT8*  pui1_buf   = NULL ;
    UINT8*  pui1_write = NULL ;
    UINT8*  pui1_begin = NULL ;
    INT32   i4_val;
    UINT32  ui4_sz;
    UINT32  ui4_val;
    INT32   i4_rc      = DBR_FAIL;
    UINT32  ui4_crc;
    UINT8   ui1_cdb_ver;    

    /* The current CDB library version number. */
    ui1_cdb_ver=CDB_FILE_VER;
    
    /* Create a memory buffer to write out the data. */
    ui4_sz = db_get_meta_sect_size(pt_do);

    pui1_buf = (UINT8*)x_mem_alloc(ui4_sz);
    if ( pui1_buf != NULL )
    {
        pui1_write=pui1_buf;

        /*
           For file version 2 and greater, we also write out
           the data version id.
        */
        if ( ui1_cdb_ver >= 2 )
        {
            /*
            x_dbg_stmt("{CDB} write out database version id (version 2): %d\n",
                       pt_do->ui4_db_version);
            */
            
            x_memcpy(pui1_write,&(pt_do->ui4_db_version),4);
            x_end_cnvt_to_big_32(pui1_write, 1, NULL);
            pui1_write += 4;
        }
        
        x_memcpy(pui1_write,&(pt_do->ui1_key_field_count),1);
        pui1_write += 1 ;

        pui1_begin = pui1_write;
        for ( ui2_i=0 ; ui2_i < pt_do->ui1_key_field_count ; ui2_i++ )
        {
            x_memcpy(pui1_write,&(pt_do->aui4_key_field_types[ui2_i]),4);
            pui1_write += 4;
        }
        x_end_cnvt_to_big_32(pui1_begin,pt_do->ui1_key_field_count,NULL);

        x_memcpy(pui1_write,&(pt_do->ui1_field_count),1);
        pui1_write += 1 ;

        pui1_begin = pui1_write;
        for ( ui2_i=0 ; ui2_i < pt_do->ui1_field_count ; ui2_i++ )
        {
            x_memcpy(pui1_write,&(pt_do->pt_fields[ui2_i].ui4_field_type),4);
            pui1_write += 4;
        }
        x_end_cnvt_to_big_32(pui1_begin,pt_do->ui1_field_count,NULL);

        pui1_begin = pui1_write;
        for ( ui2_i=0 ; ui2_i < pt_do->ui1_field_count ; ui2_i++ )
        {
            x_memcpy(pui1_write,
                     &(pt_do->pt_fields[ui2_i].ui2_field_element_count),2);
            pui1_write += 2;
        }
        x_end_cnvt_to_big_16(pui1_begin,pt_do->ui1_field_count,NULL);

        pui1_begin = pui1_write;
        for ( ui2_i=0 ; ui2_i < pt_do->ui1_field_count ; ui2_i++ )
        {
            x_memcpy(pui1_write,&(pt_do->ui2_fld_offset[ui2_i]),2);
            pui1_write += 2;
        }
        x_end_cnvt_to_big_16(pui1_begin,pt_do->ui1_field_count,NULL);        

        pui1_begin = pui1_write;
        for ( ui2_i=0 ; ui2_i < pt_do->ui1_field_count ; ui2_i++ )
        {
            x_memcpy(pui1_write,&(pt_do->ui2_fld_width[ui2_i]),2);
            pui1_write += 2;
        }
        x_end_cnvt_to_big_16(pui1_begin,pt_do->ui1_field_count,NULL);

        /* maximum record counts per memory block.*/
        x_memcpy(pui1_write,&(pt_do->ui2_max_count),2);
        x_end_cnvt_to_big_16(pui1_write,1,NULL);
        pui1_write += 2 ;

        /* Current number of record counts.*/
        x_memcpy(pui1_write,&(pt_do->ui2_record_count),2);
        x_end_cnvt_to_big_16(pui1_write,1,NULL);
        pui1_write += 2 ;

        /* Width of the record.*/
        x_memcpy(pui1_write,&(pt_do->ui2_record_width),2);
        x_end_cnvt_to_big_16(pui1_write,1,NULL);
        pui1_write += 2 ;
        
        /*
           Memory policy. Note: Although this field is an enum type, we
           will save it as 4 bytes integer value and will read it back as
           4 byte integer value.
        */
        i4_val=pt_do->t_memory->e_memory_policy;
        x_memcpy(pui1_write,&i4_val,4);
        x_end_cnvt_to_big_32(pui1_write,1,NULL);
        pui1_write += 4 ;
        
        /* Common data size. */
        x_memcpy(pui1_write,&(pt_do->ui2_common_size),2);
        x_end_cnvt_to_big_16(pui1_write,1,NULL);
        pui1_write += 2 ;

        /* Common data buffer. */
        if ( pt_do->ui2_common_size > 0 )
        {
            if ( pf_cdata_conv != NULL )
            {
                pf_cdata_conv(pt_do->pui1_common_data,
                              pt_do->ui2_common_size,
                              pui1_write);
            }
            else
            {
                x_memcpy(pui1_write,
                         pt_do->pui1_common_data,
                         pt_do->ui2_common_size);
            }
            pui1_write += pt_do->ui2_common_size;
        }

        /* Data section size. */
     
        ui4_val = db_get_data_sect_size_v2(pt_do, pf_conv, b_cmpres);
        
 
        DBG_INFO(("{CDB::FileIO} data section size: [%d bytes]\n", ui4_val));
         
        x_memcpy(pui1_write,&ui4_val,4);
        x_end_cnvt_to_big_32(pui1_write,1,NULL);

        /* ******************************************** */
        /* Write to the file handle. */
        /* ******************************************** */
        i4_val=x_fm_write(h_file,pui1_buf,ui4_sz,&ui4_val);
        if ( i4_val == FMR_OK )
        {
            /* Number of bytes written should equals number
               of bytes in the buffer.
            */
            if ( ui4_sz == ui4_val )
            {
                /*
                 Compute the CRC 32 value over the CDB metadata buffer.
                */
                if ( pui4_crc != NULL )
                {
                    ui4_crc = x_crc32_gen(pui1_buf, ui4_sz, (*pui4_crc));
                    *pui4_crc = ui4_crc;
                }
                
                i4_rc=DBR_OK;
            }
            else
            {
                i4_rc=DBR_FILE_WRITE_FAIL ;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
                */
            }
        }
        else
        {
            i4_rc=DBR_FILE_WRITE_FAIL ;
            /*
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
            */
        }
        /* Don't forget to free the temporary memory. */
        x_mem_free(pui1_buf);
    }
    else
    {
        i4_rc=DBR_OUT_OF_MEMORY ;
        /*
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_write_data_sect
 *
 * Description: Internal API to write out the data section.            
 *
 *
 * Inputs:
 *    pt_do:    database object.
 *    h_file:   field handle.
 *    pui4_crc: CRC value computed over the file head data.
 *            
 * Outputs:
 *     pui4_crc: CRC value computed over the file head data.
 *                          
 *
 * Returns: - DBR_OK:  success   
 *            DBR_FAIL: the specified field type is incorrect.   
 *            
 *            
 ----------------------------------------------------------------------------*/
#if 0
static INT32 db_write_data_sect_v2
(
    CDB_DATA_OBJ_T*    pt_do,
    HANDLE_T           h_file,
    db_endian_conv_fct pf_conv,
    UINT32*            pui4_crc
)
{
    UINT32              ui4_i=0;
    INT32               i4_rc;
    SIZE_T              z_uncmp_data_len;
    SIZE_T              z_cmp_data_len;
    CDB_MEM_CURSOR_T    t_mem_cursor;
    UINT8*              pui1_rec;
    DB_IO_BUF*          pt_io_buf;
    UINT8*              pui1_buf;
    HANDLE_T            h_zip = NULL_HANDLE;
    

    i4_rc = DBR_OK;

    db_open_cmpres_eng(pt_do->s_cmpres_eng, &h_zip);
    
    /* Write out CDB records one by one. */
    if ( pt_do->ui2_record_count > 0)
    {
        /*
           create a IO buffer (4K size)
        */
        i4_rc = db_create_io_buf(DB_IO_BUF_SZ, &pt_io_buf);

        if ( i4_rc == DBR_OK )
        {
            
            /*
              Set the memory cursor to the first record in the
              memory chunk.
            */
            db_set_cursor_start(pt_do->t_memory,&t_mem_cursor);
            
            while (1)
            {
                i4_rc=db_get_next_used_block(pt_do->t_memory,
                                             &t_mem_cursor,
                                             &pui1_rec);

                if ( i4_rc == DBR_OK && pui1_rec != NULL )
                {
                    /* Get the addr to write into the io buffer. */
                    pui1_buf=db_get_write_ptr(pt_io_buf,
                                              pt_do->ui2_record_width);
                    /*
                      If IO buffer is full, then flush the content of
                      IO buffer to file system.
                    */
                    if ( pui1_buf == NULL )
                    {
                        i4_rc = db_flush_io_buf(pt_io_buf, h_file, pui4_crc);
                        if ( i4_rc == DBR_OK )
                        {
                            pui1_buf=db_get_write_ptr(pt_io_buf,
                                                      pt_do->ui2_record_width);
                        }
                        else
                        {
                            /* encounter error while writing IO buffer
                               to file. Set the error condition and
                               break out the while loop.
                            */
                            i4_rc = DBR_FILE_WRITE_FAIL;
                            break;
                        }   
                    }
                    /*
                      Perform endian conversion.
                    */
                    db_rec_to_big_endian(pt_do, pui1_rec, pui1_buf, pf_conv);

                    /*
                      increment the record count write out to the
                      file IO buffer.
                    */
                    ui4_i++;
                }
                else
                {
                    /* No more records left, don'f forget to flush the
                       content of IO buffer to file.. */
                    i4_rc = db_flush_io_buf(pt_io_buf,h_file, pui4_crc);
            
                    /* Finish writing all records. */
                    break;
                }
            
            }

            /* Don't forget to delete IO buffer. */
            db_delete_io_buf(pt_io_buf);
        
        }
        
    }

    /*
       Check to make sure the number of record in the CDB and
       number of record written out is the same.
    */
    if ( i4_rc == DBR_OK  &&  ui4_i == pt_do->ui2_record_count )
    {
        i4_rc=DBR_OK;
    }
    else
    {
        i4_rc=DBR_FILE_WRITE_FAIL ;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }


    /*
      write out the string buffer.
    */
    if ( (i4_rc == DBR_OK)                &&
         (pt_do->pt_strbuf != NULL)
        )
    {
        UINT32           aui4_byte_to_copy[2];
        UINT32           aui4_big_end[2];
        UINT8*           aui1_addr_vec[2];
        UINT16           ui2_nb_of_str;
        
        /*
          CDB records loaded. Now we need to load the string
          object buffer.

          The string object buffer is stored as
          4 byte size, 4 bytes, n bytes of object string info,
          and n bytes of data.
        */
        x_str_buf_serialize(pt_do->pt_strbuf, aui4_byte_to_copy,
                            aui1_addr_vec);

        /***********************  cmpres string buf  ********************/
        i4_rc = db_create_io_buf(aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8,
                                 &pt_io_buf);
        if ( i4_rc != DBR_OK )
        {
            x_dbg_stmt("{CDB} Error:  no space in io_buffer.\n");
            return DBR_FILE_WRITE_FAIL ;
        }
        /***************   cmpres string buf  **********************/
        
        x_end_cnvt_to_big_32(aui4_byte_to_copy,2,aui4_big_end);
        
        i4_rc = x_fm_write(h_file,
                           aui4_big_end,
                           2*sizeof(UINT32),
                           &ui4_i);

        /****** cmpres string buf: write 8 bytes for string buf data *******/
        pui1_buf=db_get_write_ptr(pt_io_buf,8);
        if ( pui1_buf != NULL )
        {
            x_memcpy(pui1_buf,aui4_big_end,8);
        }
        else
        {
            x_dbg_stmt("{CDB} Error:  no space in io_buffer.\n");
            return DBR_FILE_WRITE_FAIL ;
        }
        /*****************************************/
        
        if ( (i4_rc == FMR_OK)                 &&
             (ui4_i == (2*sizeof(UINT32)))
            )
        {
            UINT16*    pui2_out_data = NULL;
            
            /*
            x_dbg_stmt("{CDB} write out string data: obj array sz: %d string buffer sz: %d\n",
                       aui4_byte_to_copy[0],
                       aui4_byte_to_copy[1]);
            */
            if ( aui4_byte_to_copy[0] > 0 )
            {
                if ( (pui2_out_data = x_mem_alloc(aui4_byte_to_copy[0])) != NULL )
                {
                    ui2_nb_of_str = aui4_byte_to_copy[0]/sizeof(UINT16);
                
                    /* write out the data, convert the internal
                       data array to Big endian. */
                    x_end_cnvt_to_big_16(aui1_addr_vec[0],
                                         ui2_nb_of_str,
                                         pui2_out_data);

                    i4_rc = x_fm_write(h_file,
                                       pui2_out_data,
                                       aui4_byte_to_copy[0],
                                       &ui4_i);

                    /**** cmpres string buf: string object location array ****/
                    pui1_buf=db_get_write_ptr(pt_io_buf,aui4_byte_to_copy[0]);
                    if ( pui1_buf != NULL )
                    {
                        x_memcpy(pui1_buf,pui2_out_data,aui4_byte_to_copy[0]);
                    }
                    else
                    {
                        x_dbg_stmt("{CDB::FileIO} Error:  no space in io_buffer.\n");
                        return DBR_FILE_WRITE_FAIL ;
                    }
                    /********* cmpres string buf  ***************/
        
                    x_mem_free(pui2_out_data);
                    
                    if ( i4_rc == FMR_OK    &&
                         ui4_i == aui4_byte_to_copy[0]
                        )
                    {
                        i4_rc = x_fm_write(h_file,
                                           aui1_addr_vec[1],
                                           aui4_byte_to_copy[1],
                                           &ui4_i);

                        
                    /****  cmpres string buf: string object location array  ******/
                    pui1_buf=db_get_write_ptr(pt_io_buf,aui4_byte_to_copy[1]);
                    if ( pui1_buf != NULL )
                    {
                        x_memcpy(pui1_buf,aui1_addr_vec[1],aui4_byte_to_copy[1]);
                    }
                    else
                    {
                        x_dbg_stmt("{CDB::FileIO} Error:  no space in io_buffer.\n");
                        return DBR_FILE_WRITE_FAIL ;
                    }
                
                    db_get_cmpres_data_len_info(h_zip,
                                                pt_io_buf,
                                                &z_uncmp_data_len,
                                                &z_cmp_data_len);
                    /************    cmpres string buf   ***************/
                        if ( i4_rc == FMR_OK     &&
                             ui4_i == aui4_byte_to_copy[1]
                            )
                        {
#if 0
                            x_str_buf_diagnostic(pt_do->pt_strbuf);
                            x_dbg_stmt("{CDB} string buffer saved to file.\n");
                            x_dbg_stmt("{CDB} ------------------------------\n");
#endif
                            
                            
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{CDB} Error: can not save string buffer to file.\n");
                            i4_rc = DBR_FILE_WRITE_FAIL ;
                        }
                    }
                    else
                    {
                        i4_rc = DBR_FILE_WRITE_FAIL ;
                    }
                }
                else
                {
                    i4_rc = DBR_FILE_WRITE_FAIL;
                }
            }
        }
        else
        {
            i4_rc = DBR_FILE_WRITE_FAIL ;
        }
    }


    /*
      write out the memory buffer.
    */
    if ( i4_rc == DBR_OK )
    {
        UINT32           aui4_byte_to_copy[2];
        UINT32           aui4_big_end[2];
        UINT8*           aui1_addr_vec[2];
        UINT16           ui2_nb_of_obj;
        
        /*
          CDB records and string buffer saved to file.
          Now write out the memory object buffer.

          The memory object buffer is stored as
          4 byte size, 4 bytes, n bytes of object info,
          and n bytes of data.
        */
        x_mem_buf_serialize(pt_do->pt_membuf, aui4_byte_to_copy,
                            aui1_addr_vec);

        x_end_cnvt_to_big_32(aui4_byte_to_copy,2,aui4_big_end);
        
        i4_rc = x_fm_write(h_file,
                           aui4_big_end,
                           2*sizeof(UINT32),
                           &ui4_i);
        
        if ( (i4_rc == FMR_OK)                 &&
             (ui4_i == (2*sizeof(UINT32)))
            )
        {
            UINT32*    pui4_out_data = NULL;
            
            /*
            x_dbg_stmt("{CDB} write out object data: obj array sz: %d memory buffer sz: %d\n",
                       aui4_byte_to_copy[0],
                       aui4_byte_to_copy[1]);
            */
            if ( aui4_byte_to_copy[0] > 0 )
            {
                if ( (pui4_out_data = x_mem_alloc(aui4_byte_to_copy[0])) != NULL )
                {
                    ui2_nb_of_obj = aui4_byte_to_copy[0]/sizeof(UINT32);
                
                    /* write out the data, convert the internal
                       data array to Big endian. */
                    x_end_cnvt_to_big_32(aui1_addr_vec[0],
                                         ui2_nb_of_obj,
                                         pui4_out_data);

                    i4_rc = x_fm_write(h_file,
                                       pui4_out_data,
                                       aui4_byte_to_copy[0],
                                       &ui4_i);

                    x_mem_free(pui4_out_data);
                    
                    if ( i4_rc == FMR_OK    &&
                         ui4_i == aui4_byte_to_copy[0]
                        )
                    {
                        i4_rc = x_fm_write(h_file,
                                           aui1_addr_vec[1],
                                           aui4_byte_to_copy[1],
                                           &ui4_i);

                        if ( i4_rc == FMR_OK     &&
                             ui4_i == aui4_byte_to_copy[1]
                            )
                        {
#if 0
                            x_mem_buf_diagnostic(pt_do->pt_membuf);
                            x_dbg_stmt("{CDB} memory buffer saved to file.\n");
                            x_dbg_stmt("{CDB} ------------------------------\n");
#endif
                            
                            
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{CDB} Error: can not save memory buffer to file.\n");
                            i4_rc = DBR_FILE_WRITE_FAIL ;
                        }
                    }
                    else
                    {
                        i4_rc = DBR_FILE_WRITE_FAIL ;
                    }
                }
                else
                {
                    i4_rc = DBR_FILE_WRITE_FAIL;
                }
            }
        }
        else
        {
            i4_rc = DBR_FILE_WRITE_FAIL ;
        }
    }

    db_close_cmpres_eng(h_zip);
    
    return i4_rc;
}
#endif
/*-----------------------------------------------------------------------------
 * Name: db_rec_to_big_endian
 *
 * Description: API to convert a record to Big-Endian format.
 *
 * Inputs:  -
 *    pt_do
 *    pui1_rec
 *    pf_conv:   caller provided endian conversion function.
 *
 * Outputs: -
 *    pui1_buf:  buffer containing converted data in big-endian format 
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_rec_to_big_endian
( 
    const CDB_DATA_OBJ_T*  pt_do,
    const UINT8*           pui1_rec,
    UINT8*                 pui1_buf,
    db_endian_conv_fct     pf_conv
)
{
    INT32           i4_rc=DBR_OK;
    INT32           i4_i;
    INT32           i4_n;
    INT32           i4_type;
    UINT8*          pui1_start;   /* Start of the record address. */
    
    /* Save the start of the input record address. */
    pui1_start=(UINT8*)pui1_rec;
    
    {
        /*
          Perform the endian conversion and compaction field by field.
        */
        for (i4_i=0; i4_i < pt_do->ui1_field_count; i4_i++ )
        {
            i4_type = (INT32) DB_GET_C_TYPE(pt_do->pt_fields[i4_i].ui4_field_type);
            
            switch ( i4_type )
            {
                case DBF_UINT8_T:
                case DBF_INT8_T:
                case DBF_CHAR_T:
                case DBF_ENUM8_T:
                case DBF_ARRAY_OF_UINT8_T:
                case DBF_ARRAY_OF_INT8_T:
                case DBF_ARRAY_OF_CHAR_T:
                case DBF_ARRAY_OF_ENUM8_T:
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }                    
                    /*
                       Single bytes, no endian conversion, so just copy
                       the field to io buffer.
                    */
                    i4_n=(INT32) (pt_do->pt_fields[i4_i].ui2_field_element_count) ;
                    x_memcpy(pui1_buf,
                             pui1_rec,
                             (SIZE_T)(1*i4_n));

                    /*
                       Call client registered store function to modify
                       field data being saved to the filesystem.
                    */
                    if ( pt_do->pf_client_store_fct != NULL )
                    {
                        CDB_OPAQUE_OBJ_T      t_rec_obj;

                        t_rec_obj.pt_do    = (CDB_DATA_OBJ_T*) pt_do ;
                        t_rec_obj.pui1_rec = pui1_start ;

                        db_call_client_write_fct(&t_rec_obj,
                                                 pt_do->pt_fields[i4_i].ui4_field_type,
                                                 pui1_buf,
                                                 (UINT32)(1*i4_n));
                    }
                    
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+1*i4_n;
                    pui1_rec=pui1_rec+1*i4_n;
                }
                break;

                case DBF_STRING_T:    /* string ID is stored as UINT16 */
                case DBF_MEMBUF_T:    /* object ID is also stored as UINT16 */
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }
                    /*
                       2 bytes, perform endian conversion.
                    */
                    i4_n= 1;
                    x_end_cnvt_to_big_16(pui1_rec, (SIZE_T)i4_n, pui1_buf);

                    /* Note: for string object or memory object, we currently
                       do not support client modifcation */
                    
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+2*i4_n;
                    pui1_rec=pui1_rec+2*i4_n;
                    
                }
                break;
                
                case DBF_UINT16_T:
                case DBF_INT16_T:
                case DBF_ENUM16_T:
                case DBF_UTF16_T:
                case DBF_ARRAY_OF_UINT16_T:
                case DBF_ARRAY_OF_INT16_T:
                case DBF_ARRAY_OF_ENUM16_T:
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }
                    /*
                       2 bytes, perform endian conversion.
                    */
                    i4_n= (INT32) (pt_do->pt_fields[i4_i].ui2_field_element_count) ;
                    x_end_cnvt_to_big_16(pui1_rec,(SIZE_T)i4_n,pui1_buf);

                    /*
                       Call client registered store function to modify
                       field data being saved to the filesystem.
                    */
                    if ( pt_do->pf_client_store_fct != NULL )
                    {
                        CDB_OPAQUE_OBJ_T      t_rec_obj;
                        INT32                 i4_is_modified = 0;

                        t_rec_obj.pt_do    = (CDB_DATA_OBJ_T*) pt_do ;
                        t_rec_obj.pui1_rec = pui1_start ;

                        i4_is_modified = db_call_client_write_fct(
                            &t_rec_obj,pt_do->pt_fields[i4_i].ui4_field_type,
                            pui1_buf, (UINT32)(2*i4_n));

                        /* covert to client's data to big endian. */
                        if ( i4_is_modified == 1 )
                        {
                            x_end_cnvt_to_big_16(pui1_buf,(SIZE_T)i4_n,pui1_buf);
                        }
                    }
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+2*i4_n;
                    pui1_rec=pui1_rec+2*i4_n;
                }
                break;
                
                case DBF_UINT32_T:
                case DBF_INT32_T:
                case DBF_ENUM32_T:
                case DBF_UTF32_T:
                case DBF_UNION_TAG_T:
                case DBF_ARRAY_OF_UINT32_T:
                case DBF_ARRAY_OF_INT32_T:
                case DBF_ARRAY_OF_ENUM32_T:
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }
                    /*
                       4 bytes, perform endian conversion.
                    */
                    i4_n= (INT32) (pt_do->pt_fields[i4_i].ui2_field_element_count);
                    x_end_cnvt_to_big_32(pui1_rec,(SIZE_T)i4_n,pui1_buf);

                    /*
                       Call client registered store function to modify
                       field data being saved to the filesystem.
                    */
                    if ( pt_do->pf_client_store_fct != NULL )
                    {
                        CDB_OPAQUE_OBJ_T      t_rec_obj;
                        INT32                 i4_is_modified = 0;

                        t_rec_obj.pt_do    = (CDB_DATA_OBJ_T*)pt_do ;
                        t_rec_obj.pui1_rec = pui1_start ;

                        i4_is_modified = db_call_client_write_fct(
                            &t_rec_obj,pt_do->pt_fields[i4_i].ui4_field_type,
                            pui1_buf, (UINT32)(4*i4_n));

                        /* covert to client's data to big endian. */
                        if ( i4_is_modified == 1 )
                        {
                            x_end_cnvt_to_big_32(pui1_buf,(SIZE_T)i4_n,pui1_buf);
                        }
                        
                    }
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+4*i4_n;
                    pui1_rec=pui1_rec+4*i4_n;
                }
                break;

                case DBF_UINT64_T:
                case DBF_INT64_T:
                case DBF_ARRAY_OF_UINT64_T:
                case DBF_ARRAY_OF_INT64_T:
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }
                    /*
                       8 bytes, perform endian conversion.
                    */
                    i4_n= (INT32) (pt_do->pt_fields[i4_i].ui2_field_element_count) ;
                    x_end_cnvt_to_big_64(pui1_rec,(SIZE_T)i4_n,pui1_buf);

                    /*
                       Call client registered store function to modify
                       field data being saved to the filesystem.
                    */
                    if ( pt_do->pf_client_store_fct != NULL )
                    {
                        CDB_OPAQUE_OBJ_T      t_rec_obj;
                        INT32                 i4_is_modified = 0;

                        t_rec_obj.pt_do    = (CDB_DATA_OBJ_T*) pt_do ;
                        t_rec_obj.pui1_rec = pui1_start ;

                        i4_is_modified = db_call_client_write_fct(
                            &t_rec_obj,pt_do->pt_fields[i4_i].ui4_field_type,
                            pui1_buf, (UINT32)(8*i4_n));
                        
                        /* covert to client's data to big endian. */
                        if ( i4_is_modified == 1 )
                        {
                            x_end_cnvt_to_big_64(pui1_buf,(SIZE_T)i4_n,pui1_buf);
                        }
                    }
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+8*i4_n;
                    pui1_rec=pui1_rec+8*i4_n;
                }
                break;                

                case DBF_UNION_T:
                {
                    DB_UNION_TAG_T  t_union_tag;
                    UINT8*          pui1_union_tag_rec;

                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }

                    /*
                      The previous record field (from the current UNION_T field)
                      must be a UNION_TAG_T.
                    */
                    if ( DB_GET_C_TYPE(pt_do->pt_fields[i4_i-1].ui4_field_type)
                        != DBF_UNION_TAG_T )
                    {
                        ABORT(DBG_CAT_INV_OP,DBG_ABRT_MISSING_UNION_TAG_FLD);
                    } 

                    /* Assertion check: for an UNION field, the caller
                       must provide a function to interpret the
                       content of the union, to convert to the proper
                       byte order.
                    */
                    if ( pf_conv == NULL )
                    {
                        ABORT(DBG_CAT_INV_OP,DBG_ABRT_NO_UNION_INTREP_FCT);
                    }
                    
                    /*
                      The previous field must be a UNION_TAG_T, get the
                      address of the UNION_TAG_T field (which is 4 bytes)
                      by decrementing the address of the UNION field by
                      4 bytes.
                    */
                    pui1_union_tag_rec= (UINT8*)pui1_rec - 4;
                    x_memcpy(&t_union_tag, pui1_union_tag_rec, 4);
                    
                    /*
                      call user provided function to interpret
                      union field.
                    */
                    pf_conv(
                        pt_do->pt_fields[i4_i].ui4_field_type,
                        t_union_tag,
                        pui1_rec,
                        pt_do->ui2_fld_width[i4_i],
                        pui1_buf);
                    /*
                        move the pointer to the next element of
                        this field.
                    */
                    pui1_buf=pui1_buf+pt_do->ui2_fld_width[i4_i];
                    pui1_rec=pui1_rec+pt_do->ui2_fld_width[i4_i];
                    
                }
                break;

                /*
                  Handle all other types, FLOAT32, FLOAT64,etc)
                */
                default:
                {
                    /*
                      Assertion check.
                    */
                    if ( pui1_rec != pui1_start+pt_do->ui2_fld_offset[i4_i] )
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_WRITE_FAIL);
                    }
                    /*
                      copy the field to the output buffer without endian
                      conversion.
                    */
                    i4_n=(INT32)(pt_do->ui2_fld_width[i4_i]);
                    x_memcpy(pui1_buf,
                             pui1_rec,
                             (SIZE_T)i4_n);


                    /*
                       Call client registered store function to modify
                       field data being saved to the filesystem.
                    */
                    if ( pt_do->pf_client_store_fct != NULL )
                    {
                        CDB_OPAQUE_OBJ_T      t_rec_obj;

                        t_rec_obj.pt_do    = (CDB_DATA_OBJ_T*) pt_do ;
                        t_rec_obj.pui1_rec = pui1_start ;

                        db_call_client_write_fct(&t_rec_obj,
                                                 pt_do->pt_fields[i4_i].ui4_field_type,
                                                 pui1_buf,
                                                 (UINT32)(i4_n));
                    }
                    
                    pui1_buf=pui1_buf+i4_n;
                    pui1_rec=pui1_rec+i4_n;
                }
                break;   
            }
        } /* End of loop that iterates through each record field. */
    }        
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_create_cdb_from_file
 *
 * Description: API to create a new CDB object from the data (file format, meta data, 
 *              and records) stored on a file.  It returns a CDB handle to the
 *              newly created database object.
 * Inputs:   
 *    pt_do:    Database object.
 *    h_file:   Handle to the file object to write to.
 *
 * Outputs: -
 *    ph_cdb:   Handle to the CDB object created
 *
 * Returns: - 
 *    DBR_FILE_READ_FAIL
 *-----------------------------------------------------------------------*/
INT32 db_create_cdb_from_file
(
    CHAR*              s_db_name,
    UINT8              ui1_count,
    DB_FIELD_INFO_T    at_field_info[],
    UINT8			   ui1_key_field_count,
    DB_FIELD_T         aui4_key_field_types[],
    HANDLE_T           h_file,
    db_endian_conv_fct pf_conv,
    db_cdata_conv_fct  pf_cdata_conv,
    VOID*              pv_tag,
    db_notify_fct      pf_nfy,
    HANDLE_T*          ph_cdb
)
{
    INT32                i4_rc;
    DB_FILE_META_DATA_T  t_meta;
    UINT32               ui4_byte_read;
    UINT8                aui1_buf[CDB_FILE_TAIL_MARKER_SIZE];
    UINT32               ui4_crc;
    UINT8                ui1_file_ver_read;
    
    /*
      check if read or write lock has been grabbed on the CDB file object
    */
    db_check_read_write_lock(h_file);

    db_init_metabuf(&t_meta);

    /*
      initialize the CRC seed value.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
       Get meta data section of
       CDB file.
    */
    i4_rc=db_get_metadata_from_file(h_file, pf_cdata_conv, &t_meta,
                                    &ui1_file_ver_read, &ui4_crc);
    
    if ( i4_rc == DBR_OK )
    {
        i4_rc=db_create(t_meta.ui2_max_count,
                        (DB_MEM_POLICY_T) t_meta.t_mem_policy,
                        s_db_name,
                        ui1_count,
                        at_field_info,
                        ui1_key_field_count,
                        aui4_key_field_types,
                        pv_tag,
                        pf_nfy,
                        ph_cdb);
        
        if ( i4_rc == DBR_OK )
        {
            db_write_lock(*ph_cdb);
            /* Load records from file */
            i4_rc=db_load_data_obj_from_file(h_file,
                                             &t_meta,
                                             ui1_file_ver_read,
                                             *ph_cdb,
                                             ui1_count,
                                             at_field_info,
                                             ui1_key_field_count,
                                             aui4_key_field_types,
                                             pf_conv,
                                             &ui4_crc);
            
            if ( i4_rc == DBR_OK )
            {
                if ( t_meta.ui2_common_sz > 0 )
                {
                    /* save the common data to the database
                       object. */
                    i4_rc=db_write_common(*ph_cdb,
                                          t_meta.pui1_common_data,
                                          t_meta.ui2_common_sz);
                }

                /*
                  Move the file pointer to the end of this CDB object.
                  By reading the end-magic number and CRC.
                */
                if ( x_fm_read(h_file, aui1_buf, CDB_FILE_TAIL_MARKER_SIZE,
                               &ui4_byte_read) == FMR_OK            &&
                     ui4_byte_read == CDB_FILE_TAIL_MARKER_SIZE
                    )
                {
                    /*
                      Perform CRC check if the file version is 2 or greater.
                    */
                    if ( ui1_file_ver_read >= 2 )
                    {
                        /* x_dbg_stmt("{CDB} perform CRC32 check.\n"); */
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        UINT32        ui4_crc_read;
                        x_memcpy(&ui4_crc_read, aui1_buf, 4);

                        DBG_INFO(("{CDB::FileIO} CRC value read from filesystem [%x]\n",
                                 ui4_crc_read));
                        
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            db_set_db_ver_num(*ph_cdb, t_meta.ui4_db_version);
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{CDB} Read Error: fail CRC32 check.\n"); 
                            i4_rc = DBR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        /* x_dbg_stmt("{CDB} old vesion of data file, skip CRC32 check.\n"); */
                        i4_rc = DBR_OK;
                    }
                }
                else
                {
                    i4_rc = DBR_FILE_READ_FAIL;
                }
            }
            
            db_write_unlock(*ph_cdb);
            /*
              If the loading of db records and retriving the
              common data operation failed, then we delete the
              DB object.
            */
            if ( i4_rc != DBR_OK )
            {
                db_delete(*ph_cdb,s_db_name);
            }
        }
    }

    /*
       Don't forget to clean up the buffers contained in the
       DB_FILE_META_DATA_T object.
    */
    db_free_metabuf(&t_meta);    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_metadata_from_file
 *
 * Description: API to read the CDB file format info (in Big-Endian format),
 *              verify that this is indeed a valid CDB file. Load the
 *              meta data section and then return the Meta data structure.
 *              .
 * Inputs:   
 *    h_file:   Handle to the file object to read from.
 *    pf_cdata_conv Data conversion interpretation function.
 *    pt_meta:  pointer to metadata object
 *    pui1_file_ver:  the file version of the data being read in.
 *    pui4_crc: Input CRC32 seed value.
 *
 * Outputs: -
 *    pui4_crc:  CRC32 value computed from the file head and metadata section.
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_get_metadata_from_file
(  
    HANDLE_T             h_file,
    db_cdata_conv_fct    pf_cdata_conv,
    DB_FILE_META_DATA_T* pt_meta,
    UINT8*               pui1_file_ver,
    UINT32*              pui4_crc
)
{
    /*
      The file format info consists of total 5 bytes:

      UINT32:   magic number
      UINT8     version number
      UINT16    Meta data size
    */    
    UINT32               ui4_magic;
    UINT32               ui4_end_magic;
    UINT8                ui1_cdb_ver;
    UINT16               ui2_meta_data_sz;
    INT32                i4_rc;
    UINT32               ui4_byte_read;
    UINT8                aui1_buf[CDB_FILE_HEADER_SIZE] ;
    UINT8*               pui1_buf   = NULL ;
    UINT32               ui4_val;

    i4_rc = DBR_FILE_READ_FAIL;
    
    if ( x_fm_read(h_file,aui1_buf,CDB_FILE_HEADER_SIZE,&ui4_byte_read)
         == FMR_OK
                      &&
         /*
            Number of bytes read should equals number
            of bytes in the buffer.
         */
         (CDB_FILE_HEADER_SIZE == ui4_byte_read)
        )
    {
        /*
          compute the CRC value from the bytes read.
        */
        if ( pui4_crc != NULL )
        {
            ui4_val = x_crc32_gen(aui1_buf,CDB_FILE_HEADER_SIZE,
                                  (*pui4_crc));
            *pui4_crc = ui4_val ;
        }
        
        x_memcpy(&ui4_magic,&(aui1_buf[0]),sizeof(ui4_magic));
        x_end_cnvt_from_big_32(&ui4_magic, 1, NULL);
            
        /*
          Check the beginning magic number, it has to be 
          DB_MAGIC_NUMBER or ~DB_MAGIC_NUMBER.
        */
        if ( ui4_magic == DB_MAGIC_NUMBER        ||
             ui4_magic == ~DB_MAGIC_NUMBER 
            )
        {
            x_memcpy(&ui1_cdb_ver,&(aui1_buf[4]),1);
            x_memcpy(&ui2_meta_data_sz, &(aui1_buf[5]),2);
            x_end_cnvt_from_big_16(&ui2_meta_data_sz, 1, NULL);

            *pui1_file_ver = ui1_cdb_ver;
            
            /* Allocate memory buffer to read in the
               meta data section from file.
            */
            pui1_buf=(UINT8*)x_mem_alloc(ui2_meta_data_sz);
            if ( pui1_buf != NULL )
            {
                if ( x_fm_read(h_file,
                               pui1_buf,
                               ui2_meta_data_sz,
                               &ui4_byte_read)         == FMR_OK   &&
                     ui4_byte_read == ui2_meta_data_sz
                    )
                {
                    /*
                      compute the CRC value from the bytes read.
                    */
                    if ( pui4_crc != NULL )
                    {
                        ui4_val = x_crc32_gen(pui1_buf, ui4_byte_read,
                                  (*pui4_crc));
                        *pui4_crc = ui4_val ;
                    }
                    
                    /*
                      Parse the meta data to get the description
                      of CDB object on file.
                    */
                    if ( db_parse_meta(pui1_buf,
                                       ui2_meta_data_sz,
                                       ui1_cdb_ver,
                                       pf_cdata_conv,
                                       pt_meta)  == DBR_OK
                        )
                    {
                        /*
                          Now we know the size of meta data size
                          and data section size. We can read the
                          end magic number to check for any
                          file corruption.  The file pointer
                          is point at the start of the data section,
                          so we need to pass in the size of
                          data section to skip.
                        */
                        if ( db_get_end_magic_num_for_read_ops(
                                 h_file,
                                 pt_meta->ui4_data_sz,
                                 &ui4_end_magic)
                             == DBR_OK )
                        {
                            /* check the end-magic number with beginning
                               magic number.
                            */
                            if ( (~ui4_end_magic) == ui4_magic )
                            {
#ifdef DEBUG
                                /*
                                x_dbg_stmt("{CDB} db_get_metadata_from_file: end-magic number check OK: %x.\n", ui4_end_magic );
                                */
#endif                                
                                i4_rc = DBR_OK;
                            }
                            else
                            {
                                x_dbg_stmt("{CDB::db_get_metadata_from_file} Read Error: fail end-magic number check.\n");
                            }
                        }
                    }
                }
                else
                {
                    i4_rc = DBR_FILE_READ_FAIL;
                    /*
                      ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                    */
                }
                /*
                  done with input buffer, free it.
                */
                x_mem_free(pui1_buf);
            }
            else
            {
                i4_rc = DBR_OUT_OF_MEMORY;
                /*
                  ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
        else
        {
            /*
              Wrong file magic number, file handle is not
              a CDB file.
            */
            i4_rc = DBR_FILE_READ_FAIL;
            /*
            x_dbg_stmt("{CDB} db_get_metadata_from_file: begin-magic number is invalid.\n");
            */
        }
    }
    else
    {
        /*
          File read error, or number of byte read error.
        */
        i4_rc = DBR_FILE_READ_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_parse_meta
 *
 * Description: API to parse CDB meta data section data in
 *              Big-Endian format.
 *             
 * Inputs:   
 *    pt_do:    Database object.
 *    h_file:   Handle to the file object to write to.
 *
 * Outputs: - None.
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_parse_meta
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    UINT8                 ui1_cdb_ver,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
)
{
    INT32           i4_rc  = DBR_FAIL;
    
    /*
      Ensure backward compatibility:
      Process the current and the previous version of meta data section
    */
    if ( ui1_cdb_ver <= CDB_FILE_VER )
    {
        switch (ui1_cdb_ver)
        {
            /*
              Handle version 1 of the CDB file format.
            */
            case 1:
            {
                /* x_dbg_stmt("{CDB} db_parse_meta: reading version 1 of CDB file.\n"); */
                /*
                  Load the meta data on file into the meta data
                  structure in memory.
                */
                i4_rc=db_load_metadata_ver_1(pui1_buf,
                                             ui2_meta_data_sz,
                                             pf_cdata_conv,
                                             pt_meta);
            }
            break;

            /*
              Handle version 2 of the CDB file format.
            */
            case 2:
            {
                if ( CDB_FILE_VER >= 3 )
                {
                    i4_rc = DBR_FILE_READ_FAIL;

                    x_dbg_stmt("{CDB} Read Error: Can not load version 2 of CDB file.\n");
                }
                else
                {
                    /*
                      Load the meta data on file into the meta data
                      structure in memory.
                    */
                    i4_rc=db_load_metadata_ver_2(pui1_buf,
                                                 ui2_meta_data_sz,
                                                 pf_cdata_conv,
                                                 pt_meta);
                }
            }
            break;

            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            {
                x_dbg_stmt("{CDB} Loading metadata (version %d)\n", ui1_cdb_ver);
                i4_rc=db_load_metadata_ver_3(pui1_buf,
                                             ui2_meta_data_sz,
                                             pf_cdata_conv,
                                             pt_meta);
            }
            break;
            
            /*
              In the future, if we have new version of CDB file, then
              new API will be implemented to load the meta data in file
              into the memory. 
            */
                    
            default:
            {
                i4_rc = DBR_FILE_READ_FAIL;
            }
            break;
        }
    }
    else
    {
        /*
          Does not know how to read this version of the meta data.
        */
        i4_rc = DBR_FILE_READ_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_init_metabuf
 *
 * Description: API to initialize the file meta-buffer structure befor use.
 *             
 * Inputs:   
 *    pt_meta:  Pointer to the metadata structure.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK
 *
 *-----------------------------------------------------------------------*/
static
INT32  db_init_metabuf
(
    DB_FILE_META_DATA_T* pt_meta
)
{
    if ( pt_meta != NULL )
    {
        pt_meta->ui4_db_version=0;
        pt_meta->ui1_keyfld_count=0;
        pt_meta->ui1_fld_count=0;
        pt_meta->ui2_max_count=0;
        pt_meta->ui2_rec_count=0;
        pt_meta->ui2_rec_width=0;
        pt_meta->ui2_common_sz=0;
        pt_meta->ui4_data_sz=0;

        pt_meta->pui4_keyfld_types=NULL;
        pt_meta->pt_fld_info=NULL;
        pt_meta->pui2_offset=NULL;
        pt_meta->pui2_width=NULL;
        pt_meta->pui1_common_data=NULL;
    }
    return DBR_OK;
}

    
/*-----------------------------------------------------------------------------
 * Name: db_alloc_metabuf
 *
 * Description: API to allocate mebuffers in the META DATA 
 *              structure for storing CDB meta data section
 *              in the file. 
 *
 *              The purpose of this function is to minimize
 *              memory fragmentation. We compute the space needed
 *              for storing the array fields in the META DATA
 *              structure, and make a single memory allocation,
 *              instead of 5 separate memory allocation.
 *
 *              
 *             
 * Inputs:   
 *    pt_meta:  Structure containing meta data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static
INT32  db_alloc_metabuf
(
    DB_FILE_META_DATA_T* pt_meta
)
{
    INT32  i4_sz;
    INT32  i4_loc_fld_info_ary;
    INT32  i4_loc_fld_offset;
    INT32  i4_loc_fld_width;
    INT32  i4_loc_common_data;
    INT32  i4_rc;

    /* size of keyfld type array. Add padding to align on 4 bytes */
    i4_sz = (pt_meta->ui1_keyfld_count * 4 + 3) & (~0x3);
 
    /* Set the location of record field_info array, this is aligned
       on 4 bytes
    */
    i4_loc_fld_info_ary = i4_sz ;
    /*
#ifdef DEBUG       
    x_dbg_stmt("fld info ary offset : %d\n",i4_sz);
#endif        
    */
    i4_sz += (pt_meta->ui1_fld_count * sizeof(DB_FIELD_INFO_T) + 3) & (~0x3);

    /* Set the start location of record field offset array. */
    i4_loc_fld_offset=i4_sz;
    /*
#ifdef DEBUG       
    x_dbg_stmt("fld offset ary offset : %d\n",i4_sz);
#endif      
    */
    i4_sz += (pt_meta->ui1_fld_count * 2 +3) & (~0x3);

    /* Set the start location of record field width array */
    i4_loc_fld_width=i4_sz;
    /*
#ifdef DEBUG      
    x_dbg_stmt("fld width ary offset : %d\n",i4_sz);
#endif     
    */
    i4_sz += (pt_meta->ui1_fld_count * 2 +3) & (~0x3);  

    /* Set the start location of the common data size. */
    i4_loc_common_data=i4_sz;
    /*
#ifdef DEBUG      
    x_dbg_stmt("common buffer offset : %d\n",i4_sz);
#endif    
    */
    i4_sz += (pt_meta->ui2_common_sz + 3) & (~0x3) ;

    /*
      Allocate the memory
    */
    /*
#ifdef DEBUG      
    x_dbg_stmt("Size of meta buffer allocated: %d\n",i4_sz);
#endif    
    */
    pt_meta->pui4_keyfld_types = NULL;
    
    pt_meta->pui4_keyfld_types=(UINT32*)x_mem_alloc((SIZE_T)i4_sz);

    if ( pt_meta->pui4_keyfld_types != NULL )
    {
        pt_meta->pt_fld_info= (DB_FIELD_INFO_T*)
            ( ((UINT8*)pt_meta->pui4_keyfld_types) + i4_loc_fld_info_ary ) ;

        pt_meta->pui2_offset= (UINT16*)
            ( ((UINT8*)pt_meta->pui4_keyfld_types) + i4_loc_fld_offset) ;

        pt_meta->pui2_width = (UINT16*)
            ( ((UINT8*)pt_meta->pui4_keyfld_types) + i4_loc_fld_width) ;
        
        pt_meta->pui1_common_data = (UINT8*)
            ( ((UINT8*)pt_meta->pui4_keyfld_types) + i4_loc_common_data) ;

        i4_rc = DBR_OK;
    }
    else
    {
        i4_rc = DBR_OUT_OF_MEMORY;
        /*
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_free_metabuf
 *
 * Description: API to free mebuffers in the META DATA 
 *              structure for storing CDB meta data section
 *              in the file. 
 *             
 * Inputs:   
 *    pt_meta:  Structure containing meta data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32  db_free_metabuf
(
    DB_FILE_META_DATA_T* pt_meta
)
{
    if ( pt_meta->pui4_keyfld_types != NULL )
    {
        x_mem_free(pt_meta->pui4_keyfld_types);
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_metadata_ver_1
 *
 * Description: API to parse decode CDB meta data section in
 *              file. This function handles version 1 of
 *              CDB meta data.  In future, if the CDB meta data
 *              section changes, then we will implement a new function
 *              to handle new CDB meta data section.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_load_metadata_ver_1
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
)
{
    UINT16  ui2_i;
    UINT8*  pui1_read = NULL ;
    INT32   i4_val;
    UINT32  ui4_val;
    INT32   i4_rc;

    UINT8*  pui1_keyfld_types;
    UINT8*  pui1_offset;
    UINT8*  pui1_width;
    UINT8*  pui1_fld_elem_count;
    UINT8*  pui1_fld_types;
    UINT8*  pui1_common_data;
    

    pui1_read=pui1_buf;
    x_memcpy(&(pt_meta->ui1_keyfld_count),pui1_read,1);
    pui1_read += 1 ;

    /* Save the address of keyfld array, and skip to the next fields.  */
    pui1_keyfld_types = pui1_read;
    pui1_read += ( 4 * pt_meta->ui1_keyfld_count);

    x_memcpy(&(pt_meta->ui1_fld_count),pui1_read,1);
    pui1_read += 1 ;


    pui1_fld_types = pui1_read;
    pui1_read += (4 * pt_meta->ui1_fld_count);

    pui1_fld_elem_count = pui1_read;
    pui1_read += (2 * pt_meta->ui1_fld_count);

    pui1_offset = pui1_read;
    pui1_read += (2 * pt_meta->ui1_fld_count);

    pui1_width = pui1_read;
    pui1_read += (2* pt_meta->ui1_fld_count);


    /* maximum record counts per memory block.*/
    x_memcpy(&(pt_meta->ui2_max_count),pui1_read,2);
    x_end_cnvt_from_big_16(&(pt_meta->ui2_max_count),1,NULL);
    pui1_read += 2 ;

    /* Current number of record counts.*/
    x_memcpy(&(pt_meta->ui2_rec_count),pui1_read,2);
    x_end_cnvt_from_big_16(&(pt_meta->ui2_rec_count),1,NULL);
    pui1_read += 2 ;

    /* Width of the record.*/
    x_memcpy(&(pt_meta->ui2_rec_width),pui1_read,2);
    x_end_cnvt_from_big_16(&(pt_meta->ui2_rec_width),1,NULL);
    pui1_read += 2 ;

    /*
       Memory policy. Note: Although this field is an enum type, we
       will save it as 4 bytes integer value and will read it back as
       4 byte integer value.
    */
    x_memcpy(&i4_val,pui1_read,4);
    x_end_cnvt_from_big_32(&i4_val,1,NULL);
    pt_meta->t_mem_policy=(DB_MEM_POLICY_T)i4_val;
    pui1_read += 4 ;

    /* Common data size. */
    x_memcpy(&(pt_meta->ui2_common_sz),pui1_read,2);
    x_end_cnvt_from_big_16(&(pt_meta->ui2_common_sz),1,NULL);
    pui1_read += 2 ;

    pui1_common_data = pui1_read;
    pui1_read += pt_meta->ui2_common_sz;

    /* Data section size. */
    x_memcpy(&ui4_val,pui1_read,4);
    x_end_cnvt_from_big_32(&ui4_val,1,NULL);
    pt_meta->ui4_data_sz=ui4_val;
    pui1_read += 4;

    /*
      Assertion check
    */
    if ( ((INT32)(pui1_read-pui1_buf)) != ((INT32)ui2_meta_data_sz) )
    {
        return DBR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }

    /*
       Now we have scan through the metafile buffer, and collect all
       the information needed allocate memory for storing
       field information such as field_types, file_width, field offset
    */
    i4_rc = db_alloc_metabuf(pt_meta);

    if ( i4_rc == DBR_OK )
    {
        x_memcpy(pt_meta->pui4_keyfld_types,
                 pui1_keyfld_types,
                 (SIZE_T)(4*pt_meta->ui1_keyfld_count));
        x_end_cnvt_from_big_32(pt_meta->pui4_keyfld_types,
                               pt_meta->ui1_keyfld_count,
                               NULL);

        for ( ui2_i = 0 ; ui2_i < pt_meta->ui1_fld_count ; ui2_i++ )
        {
            x_memcpy(&(pt_meta->pt_fld_info[ui2_i].ui4_field_type),
                     pui1_fld_types,
                     4);
            pui1_fld_types += 4;
            x_end_cnvt_from_big_32(&(pt_meta->pt_fld_info[ui2_i].ui4_field_type),
                                   1,NULL);    
        
        }    

        for ( ui2_i=0 ; ui2_i < pt_meta->ui1_fld_count ; ui2_i++ )
        {
            x_memcpy(&(pt_meta->pt_fld_info[ui2_i].ui2_field_element_count),
                     pui1_fld_elem_count,
                     2);
            pui1_fld_elem_count += 2;
            x_end_cnvt_from_big_16(
                &(pt_meta->pt_fld_info[ui2_i].ui2_field_element_count),
                1,NULL);
        }

        x_memcpy((pt_meta->pui2_offset),pui1_offset,
                 (SIZE_T)(2*pt_meta->ui1_fld_count));
        x_end_cnvt_from_big_16(pt_meta->pui2_offset,
                               pt_meta->ui1_fld_count,
                               NULL);
    
        x_memcpy((pt_meta->pui2_width),pui1_width,
                 (SIZE_T)(2*pt_meta->ui1_fld_count));
        x_end_cnvt_from_big_16(pt_meta->pui2_width,
                               pt_meta->ui1_fld_count,
                               NULL);

        /* Common data buffer. */
        if ( pt_meta->ui2_common_sz > 0 )
        {
            if ( pf_cdata_conv != NULL )
            {
                pf_cdata_conv(pui1_common_data,
                              pt_meta->ui2_common_sz,
                              pt_meta->pui1_common_data);
            }
            else
            {
                x_memcpy(pt_meta->pui1_common_data,
                         pui1_common_data,
                         pt_meta->ui2_common_sz);
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_metadata_ver_2
 *
 * Description: API to parse decode CDB meta data section in
 *              file. This function handles version 2 of
 *              CDB meta data.  In future, if the CDB meta data
 *              section changes, then we will implement a new function
 *              to handle new CDB meta data section.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_load_metadata_ver_2
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
)
{
    INT32   i4_rc;

    /*
      If CDB file version is 2 or greater,
      then read additional 4 bytes to get the database
      version id.
    */
    x_end_cnvt_from_big_32(pui1_buf, 1, &(pt_meta->ui4_db_version));

/*
    x_dbg_stmt("{CDB} database version id (version 2) read from file: %d\n",
               pt_meta->ui4_db_version);
*/    
    pui1_buf += 4 ;
    i4_rc = db_load_metadata_ver_1(pui1_buf, (UINT16)(ui2_meta_data_sz - 4),
                                   pf_cdata_conv, pt_meta);

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_metadata_ver_3
 *
 * Description: API to parse decode CDB meta data section in
 *              file. This function handles version 2 of
 *              CDB meta data.  In future, if the CDB meta data
 *              section changes, then we will implement a new function
 *              to handle new CDB meta data section.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns: - None.
 *
 *-----------------------------------------------------------------------*/
static INT32 db_load_metadata_ver_3
(
    UINT8*                pui1_buf,
    UINT16                ui2_meta_data_sz,
    db_cdata_conv_fct     pf_cdata_conv,
    DB_FILE_META_DATA_T*  pt_meta
)
{
    INT32   i4_rc;

    /*
     Metadata for CDB file version 3 and 4 are the same as in CDB file version 2.
    */
    i4_rc = db_load_metadata_ver_2(pui1_buf,ui2_meta_data_sz,pf_cdata_conv,pt_meta);

    if ( i4_rc != DBR_OK )
    {
        x_dbg_stmt("{CDB} Read Error: can not load metadata for database version 3 of the CDB object.\n");
    }
    
    return i4_rc;
}
    
/*-----------------------------------------------------------------------------
 * Name: _mem_buf_convert_old
 *
 * Description: API to convert old str_buf/mem_buf which uses 1-byte length
 *              to the new version which uses 2-byte length.
 *             
 * Inputs:   
 *    ui4_buf_sz:       buffer size
 *    ui2_total_num:    total number of strings/objects
 *    pui4_ary:         string/object ID array
 *    pui1_buf:         string buffer / memory buffer pointer
 *    ui4_check_sz:     size of output for assertion check
 *    
 * Outputs: - None.
 *    pui1_buf:         string buffer / memory buffer pointer
 *    pui4_mem_used:    pointer to the ui4_mem_used variable
 *
 * Returns:  
 *    None
 *   
 *-----------------------------------------------------------------------*/
static VOID _mem_buf_convert_old
(
    UINT32                ui4_buf_sz,
    UINT16                ui2_number,
    UINT32*               pui4_ary,
    UINT8*                pui1_buf,
    UINT32                ui4_check_sz,
    UINT32*               pui4_mem_used
)
{
    /*
     * The new mem_buf uses two bytes to store
     * object length. So we need to iterate through
     * the buffer read and expand the original one
     * byte length to two bytes.
     *
     * Decoupling needed?
     */
    UINT8*      pui1_tmp_buf;
    UINT32      ui4_read_ptr;
    UINT32      ui4_write_ptr;
    UINT16      ui2_length;
    UINT32      ui4_i;

    pui1_tmp_buf = (UINT8*) x_mem_alloc(ui4_buf_sz);
    if (pui1_tmp_buf == NULL)
    {
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
    }

    ui4_read_ptr = ui4_write_ptr = 0;
    for (ui4_i = 0; ui4_i < ui2_number; ui4_i++)
    {
        if (pui4_ary[ui4_i] != (UINT32) 0xFFFFFFFF)
        {
            ui4_read_ptr = pui4_ary[ui4_i];
            pui4_ary[ui4_i] = ui4_write_ptr;
            ui2_length = pui1_buf[ui4_read_ptr];
            PUT_UINT16_TO_PTR_BIG_END(ui2_length, &pui1_tmp_buf[ui4_write_ptr]);
            ui4_read_ptr++;
            ui4_write_ptr += 2;
            x_memcpy(&pui1_tmp_buf[ui4_write_ptr],
                     &pui1_buf[ui4_read_ptr],
                     ui2_length);
            ui4_write_ptr += ui2_length;
        }
    }

    *pui4_mem_used = ui4_write_ptr;

    /* Assertion check */
    if (ui4_check_sz != ui4_write_ptr)
    {
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_FILE_READ_FAIL);
    }

    x_memcpy(pui1_buf, pui1_tmp_buf, ui4_write_ptr);

    x_mem_free(pui1_tmp_buf);
}

/*-----------------------------------------------------------------------------
 * Name: db_load_rec_from_file
 *
 * Description: API to load CDB record data section from file.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_load_rec_from_file
(
    HANDLE_T              h_file,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    UINT32*               pui4_crc
)
{
    INT32             i4_rc;
    INT32             i4_rec_readed;
    INT32             i4_rec_per_io_buf;
    INT32             i4_sz;
    UINT32            ui4_i;
    INT32             i4_rec_remaining;
    DB_IO_BUF*        pt_io_buf;
    UINT8*            pui1_buf;
    UINT32            ui4_byte_read;
    UINT32            ui4_val;
    UINT16            ui2_count;
    
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    VOID**            apv_recfld_addrs=NULL;
    VOID*             apv_buf[16];
    UINT16            ui2_rec_width;
    UINT8*            pui1_tmp = NULL;
    UINT8             aui1_tmp[128];

    i4_rc = DBR_OK;
    
    if ( pt_meta->ui2_rec_count > 0 )
    {
        /*
          Get the width of the record in CPU memory.
        */
        if ( handle_get_type_obj(h_cdb, &pe_type, ((VOID**)&pt_co)) == HR_OK )
        {
#ifdef DEBUG
            if (pe_type != CDBT_COREDB)
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            pt_do=pt_co->p_data_obj;
            ui2_rec_width = pt_do->ui2_record_width;
        }
        else
        {
            return DBR_FILE_READ_FAIL;
        }

        /*
          Allocate a record buffer space. This working buffer will be
          use for destination of the input record buffer (read from
          file).  The content of fields read from the file will
          be converted (for any Endian conversion) into this working
          buffer.
        */
        if ( ui2_rec_width <= 128 )
        {
            /*
              If the record width is less than 128 bytes,
              then use the memory on the stack to avoid dynamic
              memory allocation.
            */
            pui1_tmp=aui1_tmp;
        }
        else
        {
            /*
              Allocate space for both tmp and work buffer.
            */
            pui1_tmp=(UINT8*)x_mem_alloc(ui2_rec_width);
            if ( pui1_tmp == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
            }
        }
        
        /*
          allocate space for the array of pointers to the fields.
          If number of fields is 16 or less, use the memory on
          the stack
        */
        if ( ui1_count <= 16 )
        {
            apv_recfld_addrs=apv_buf;
        }
        else
        {
            /*
              dynamic allocation.
            */
            apv_recfld_addrs = (VOID**)x_mem_alloc(ui1_count*sizeof(VOID*));
            if ( apv_recfld_addrs == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
            }
        }
        
        /*
          Create an IO buffer, and read multple records into
          the buffer.
        */
        i4_rc = db_create_io_buf(DB_IO_BUF_SZ, &pt_io_buf);

        if ( pui1_tmp          !=   NULL   &&
             apv_recfld_addrs  !=   NULL   &&
             i4_rc             ==   DBR_OK
            )
        {
            /*
              Determine the how many record (bytes) can be loaded in one read
              operation.
            */
            i4_rec_per_io_buf = ((INT32)(DB_IO_BUF_SZ/pt_meta->ui2_rec_width));

            /*
              Number of record in the CDB file to load.
            */
            i4_rec_remaining = (INT32)(pt_meta->ui2_rec_count);
        
            do
            {
                if ( i4_rec_remaining >= i4_rec_per_io_buf )
                {
                    i4_sz = i4_rec_per_io_buf * pt_meta->ui2_rec_width;
                    i4_rec_remaining = i4_rec_remaining - i4_rec_per_io_buf;
                }
                else
                {
                    i4_sz = i4_rec_remaining * pt_meta->ui2_rec_width;
                    i4_rec_remaining = 0;
                }
            
                /*
                  Get the write pointer to the IO buffer.
                */
                pui1_buf=db_get_write_ptr(pt_io_buf, (UINT32)i4_sz);
                if ( pui1_buf != NULL )
                {
                    i4_rc = x_fm_read(h_file, pui1_buf, (UINT32)i4_sz, &ui4_byte_read );

                    if ( (i4_rc==FMR_OK) && (ui4_byte_read == ((UINT32)i4_sz)) )
                    {
                        /*
                          compute the CRC32 value for the data section that read.
                        */
                        if ( pui4_crc != NULL )
                        {
                            ui4_val = x_crc32_gen(pui1_buf, ui4_byte_read, (*pui4_crc));
                            *pui4_crc = ui4_val ;
                        }
                        
                        i4_rec_readed = (INT32)(ui4_byte_read/pt_meta->ui2_rec_width);

                        for ( ui4_i=0; ((INT32)ui4_i) < i4_rec_readed ; ui4_i++ )
                        {
                            /*
                              Zero out the tmp buffer.
                            */
                            x_memset(pui1_tmp,0,pt_do->ui2_record_width);
                            i4_rc = db_add_rec_to_db(pui1_buf,
                                                     pt_meta,
                                                     h_cdb,
                                                     pt_do,
                                                     ui1_count,
                                                     at_field_info,
                                                     ui1_key_field_count,
                                                     aui4_key_field_types,
                                                     pf_conv,
                                                     apv_recfld_addrs,
                                                     pui1_tmp);
                            if ( i4_rc == DBR_OK )
                            {
                                /* process the next record. */
                                pui1_buf = pui1_buf + pt_meta->ui2_rec_width;
                            }
                            else
                            {
                                i4_rc = DBR_FILE_READ_FAIL;
                                break;
                                /*
                                  ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                                */
                            }   
                        }
                    
                        /*
                          Inner processing loop failed, exit the outer while
                          loop.
                        */
                        if ( i4_rc == DBR_FILE_READ_FAIL )
                        {
                            break;
                        }
                    }
                    else
                    {
                        i4_rc = DBR_FILE_READ_FAIL;
                        break;
                        /*
                          ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                        */
                    }
                    /*
                      Reset the write IO Buffer to the beginning, for the next
                      read from file.
                    */
                    db_move_write_ptr(pt_io_buf, 0, DB_IO_SET);
                }
                else
                {
                    i4_rc = DBR_FILE_READ_FAIL;
                    break;
                    /*
                      ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                    */
                }
            } while ( i4_rec_remaining > 0 ) ;
        }
        else
        {
            i4_rc = DBR_OUT_OF_MEMORY;
        }
        
        /*
          Done reading records.
          Free the IO buffer.
        */
        db_delete_io_buf(pt_io_buf);

        /*
           Load the string buffer from file
        */
        if ( i4_rc == DBR_OK )
        {
            UINT32           aui4_byte_to_copy[2];
            UINT32           aui4_big_end[2];
            UINT16           ui2_nb_of_str;

            /*
              CDB records loaded. Now we need to load the string
              object buffer.

              The string object buffer is stored as
              4 byte size, 4 bytes, n bytes of object string info,
              and n bytes of data.
            */
            i4_rc = x_fm_read(h_file,
                              aui4_big_end,
                              2*sizeof(UINT32),
                              &ui4_i);

            if ( (i4_rc == FMR_OK)                 &&
                 (ui4_i == (2*sizeof(UINT32)))
                )
            {
                x_end_cnvt_from_big_32(aui4_big_end,2,aui4_byte_to_copy);

                /* 
                x_dbg_stmt("{CDB} Read string data: obj array sz: %d string buffer sz: %d\n",
                           aui4_byte_to_copy[0],
                           aui4_byte_to_copy[1]);
                */

                if ( aui4_byte_to_copy[0] > 0 )
                {
                    if (ui1_file_ver_read >= 6)
                    {
                        ui2_nb_of_str = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));
                    }
                    else
                    {
                        ui2_nb_of_str = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT16));
                    }
                    
                    /*
                    x_dbg_stmt("{CDB} Free existing string buffer and load a new string buffer from file.\n");
                    */
                    /* free the existing string buffer and create a new
                       string buffer with the size read from file.
                    */
                    x_str_buf_free(pt_do->pt_strbuf);

                    if (ui1_file_ver_read >= 7)
                    {
                        x_str_buf_create(ui2_nb_of_str,
                                         aui4_byte_to_copy[1],
                                         &(pt_do->pt_strbuf));
                    }
                    else
                    {
                        /* old version uses 1-byte length. now we use 2 bytes */
                        x_str_buf_create(ui2_nb_of_str,
                                         aui4_byte_to_copy[1] + ui2_nb_of_str,
                                         &(pt_do->pt_strbuf));
                    }
                    
                    i4_rc = x_fm_read(h_file,
                                      (UINT8*)pt_do->pt_strbuf->pui4_ary,
                                      aui4_byte_to_copy[0],
                                      &ui4_i);
                    if ( i4_rc == FMR_OK    &&
                         ui4_i == aui4_byte_to_copy[0]
                        )
                    {        
                        /*
                           convert the data read into cpu memory order.
                        */
                        if (ui1_file_ver_read >= 6)
                        {
                            x_end_cnvt_from_big_32(pt_do->pt_strbuf->pui4_ary,
                                                   ui2_nb_of_str,
                                                   pt_do->pt_strbuf->pui4_ary);
                        }
                        else
                        {
                            UINT8*      pui1_tmp_buf;
                            pui1_tmp_buf = (UINT8*) x_mem_alloc(ui2_nb_of_str *
                                                                sizeof(UINT16));
                            if (pui1_tmp_buf == NULL)
                            {
                                ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
                            }

                            x_end_cnvt_from_big_16(pt_do->pt_strbuf->pui4_ary,
                                                   ui2_nb_of_str,
                                                   pui1_tmp_buf);

                            for (ui4_i = 0; ui4_i < ui2_nb_of_str; ui4_i++)
                            {
                                pt_do->pt_strbuf->pui4_ary[ui4_i] = *((UINT16*)(
                                        pui1_tmp_buf + ui4_i * sizeof(UINT16)));
                                if (pt_do->pt_strbuf->pui4_ary[ui4_i] == 0xFFFF)
                                {
                                    pt_do->pt_strbuf->pui4_ary[ui4_i] = 0xFFFFFFFF;
                                }
                            }

                            x_mem_free(pui1_tmp_buf);
                            ui4_i = aui4_byte_to_copy[0]; /* revert value */
                        }

                        i4_rc = x_fm_read(h_file,
                                          pt_do->pt_strbuf->pui1_str_buf,
                                          aui4_byte_to_copy[1],
                                          &ui4_i);

                        if ( i4_rc == FMR_OK     &&
                             ui4_i == aui4_byte_to_copy[1]
                            )
                        {
                            pt_do->pt_strbuf->ui2_nb_str = 0;

                            /* go through the input string offset array and find
                               out how many are valid string.
                            */
                            for ( ui2_count = 0 ;
                                  ui2_count < ui2_nb_of_str;
                                  ui2_count++ )
                            {
                                if ( pt_do->pt_strbuf->pui4_ary[ui2_count] != (UINT32)0xFFFFFFFF )
                                {
                                    pt_do->pt_strbuf->ui2_nb_str++;
                                }
                            }

                            if (ui1_file_ver_read >= 7)
                            {
                                pt_do->pt_strbuf->ui4_mem_used = aui4_byte_to_copy[1];
                            }
                            else
                            {
                                _mem_buf_convert_old(
                                        pt_do->pt_strbuf->ui4_str_buf_sz,
                                        ui2_nb_of_str,
                                        pt_do->pt_strbuf->pui4_ary,
                                        pt_do->pt_strbuf->pui1_str_buf,
                                        aui4_byte_to_copy[1] + pt_do->pt_strbuf->ui2_nb_str,
                                        &(pt_do->pt_strbuf->ui4_mem_used)
                                        );
                            }
                            
                            DBG_INFO(("{CDB} CDB object loaded from file: the internal string "));
                            DBG_INFO(("contains: %d of string entries and %d of valid string.\n",
                                      ui2_nb_of_str,pt_do->pt_strbuf->ui2_nb_str));
#if 0
                            x_str_buf_diagnostic(pt_do->pt_strbuf);
                            x_dbg_stmt("{CDB} string buffer loaded from file.\n");
                            x_dbg_stmt("{CDB} ------------------------------\n");
#endif                            
                            
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            i4_rc = DBR_FILE_READ_FAIL ;
                        }
                    }
                    else
                    {
                        i4_rc = DBR_FILE_READ_FAIL ;
                    }
                }
                
            }
            else
            {
                i4_rc = DBR_FILE_READ_FAIL ;
            }
        }      


        /*
           For version 4 and greater CDB file format,
           load the memory buffer from file.  
        */
        if ( ui1_file_ver_read >= 4  && i4_rc == DBR_OK )
        {
            UINT32           aui4_byte_to_copy[2];
            UINT32           aui4_big_end[2];
            UINT16           ui2_nb_of_obj;

            /*
              CDB records and string buffer loaded. Now load the memory
              buffer.

              The object buffer is stored as 4byte size, 4 bytes,
              n bytes of object string info, and n bytes of data.
            */
            i4_rc = x_fm_read(h_file,
                              aui4_big_end,
                              2*sizeof(UINT32),
                              &ui4_i);

            if ( (i4_rc == FMR_OK)                 &&
                 (ui4_i == (2*sizeof(UINT32)))
                )
            {
                x_end_cnvt_from_big_32(aui4_big_end,2,aui4_byte_to_copy);
                
                DBG_INFO(("{CDB} Read memory buffer data: obj array sz: %d memory buffer sz: %d\n",
                         aui4_byte_to_copy[0],aui4_byte_to_copy[1]));              

                if ( aui4_byte_to_copy[0] > 0 )
                {
                    ui2_nb_of_obj = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));
                    
                    /*
                    x_dbg_stmt("{CDB} Free existing string buffer and load a new string buffer from file.\n");
                    */
                    /* free the existing string buffer and create a new
                       string buffer with the size read from file.
                    */
                    x_mem_buf_free(pt_do->pt_membuf);
                    
                    if (ui1_file_ver_read >= 7)
                    {
                        x_mem_buf_create(ui2_nb_of_obj,
                                         aui4_byte_to_copy[1],
                                         &(pt_do->pt_membuf));
                    }
                    else
                    {
                        /* old version uses 1-byte length. now we use 2 bytes */
                        x_mem_buf_create(ui2_nb_of_obj,
                                         aui4_byte_to_copy[1] + ui2_nb_of_obj,
                                         &(pt_do->pt_membuf));
                    }
                    
                    i4_rc = x_fm_read(h_file,
                                      (UINT8*)pt_do->pt_membuf->pui4_ary,
                                      aui4_byte_to_copy[0],
                                      &ui4_i);
                    if ( i4_rc == FMR_OK    &&
                         ui4_i == aui4_byte_to_copy[0]
                        )
                    {        
                        /*
                           convert the data read into cpu memory order.
                        */
                        x_end_cnvt_from_big_32(pt_do->pt_membuf->pui4_ary,
                                               ui2_nb_of_obj,
                                               pt_do->pt_membuf->pui4_ary);

                        i4_rc = x_fm_read(h_file,
                                          pt_do->pt_membuf->pui1_mem_buf,
                                          aui4_byte_to_copy[1],
                                          &ui4_i);

                        if ( i4_rc == FMR_OK     &&
                             ui4_i == aui4_byte_to_copy[1]
                            )
                        {
                            pt_do->pt_membuf->ui2_nb_obj = 0;

                            /* go through the object offset array and find
                               out how many are valid object (e.g., address offset of
                               0xFFFFFFFF.)
                            */
                            for ( ui2_count = 0 ;
                                  ui2_count < ui2_nb_of_obj;
                                  ui2_count++ )
                            {
                                if ( pt_do->pt_membuf->pui4_ary[ui2_count] != ((UINT32)0xFFFFFFFF) )
                                {
                                    pt_do->pt_membuf->ui2_nb_obj++;
                                }
                            }
                            
                            if (ui1_file_ver_read >= 7)
                            {
                                pt_do->pt_membuf->ui4_mem_used = aui4_byte_to_copy[1];
                            }
                            else
                            {
                                _mem_buf_convert_old(
                                        pt_do->pt_membuf->ui4_obj_buf_sz,
                                        ui2_nb_of_obj,
                                        pt_do->pt_membuf->pui4_ary,
                                        pt_do->pt_membuf->pui1_mem_buf,
                                        aui4_byte_to_copy[1] + pt_do->pt_membuf->ui2_nb_obj,
                                        &(pt_do->pt_membuf->ui4_mem_used)
                                        );
                            }
                            
                            DBG_INFO(("{CDB} CDB object loaded from file: the membuf contains "));
                            DBG_INFO(("[%d] of object entries [%d], and [%d] of valid objects\n",
                                     ui2_nb_of_obj,pt_do->pt_membuf->ui2_ary_len, pt_do->pt_membuf->ui2_nb_obj));
                            
                            DBG_INFO(("{CDB} the memory size of buffer is [%d bytes], and memory used [%d bytes]",
                                     pt_do->pt_membuf->ui4_obj_buf_sz,pt_do->pt_membuf->ui4_mem_used));
                            
#if 0
                            x_mem_buf_diagnostic(pt_do->pt_membuf);
                            x_dbg_stmt("{CDB} memory buffer loaded from file.\n");
                            x_dbg_stmt("{CDB} ------------------------------\n");
#endif                            
                            
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            i4_rc = DBR_FILE_READ_FAIL ;
                        }
                    }
                    else
                    {
                        i4_rc = DBR_FILE_READ_FAIL ;
                    }
                }
                
            }
            else
            {
                i4_rc = DBR_FILE_READ_FAIL ;
            }
        }
            
        /*
           Don't forget to de-allocate the array of pointers to the fields.
        */
        if ( ui1_count > 16 && apv_recfld_addrs != NULL )
        {
            x_mem_free(apv_recfld_addrs);
        }
        if ( ui2_rec_width > 128 && pui1_tmp != NULL )
        {
            x_mem_free(pui1_tmp);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_add_rec_to_db
 *
 * Description: API to load CDB record data section from file.
 *             
 * Inputs:   
 *    pui1_buf: pointer to buffer containing a single CDB data record
 *              loaded from file system.
 *    pt_meta:  structure containing meta data descrbing
 *              the CDB object (record fields, field types, etc)
 *              read from filesystem.
 *    h_cdb:    Handle to the CDB object in memeoy to load the record
 *              from the file system.
 *    pt_do:    structure containing description of CDB object in memory.
 *
 *    ui1_count: Number of fields in the CDB object in memory.
 *
 *    ui1_field_info: Array containing record field description for the
 *                    CDB object in memory.
 *    ui1_key_field_count: number of key field in the CDB object in memory.
 *
 *    aui4_key_field_types: Field type of the key record field in the
 *                          CDB object in memory.
 *    pf_conv:  Endian conversion function for record fields.
 *    
 * Outputs: - None.
 *
 *    apv_recfld_addrs:  Array containing the address of the converted
 *                       record fields load from file system. This
 *                       array is pass into the CDB object in memory
 *                       to create the new record.
 *
 *    pui1_tmp: memory buffer containing the fields loaded from the
 *              file system.
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_add_rec_to_db
(
    UINT8*                pui1_buf,
    DB_FILE_META_DATA_T*  pt_meta,
    HANDLE_T              h_cdb,
    CDB_DATA_OBJ_T*       pt_do,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    VOID*                 apv_recfld_addrs[],
    UINT8*                pui1_tmp
)
{
    INT32       i4_rc=DBR_OK;
    INT32       i4_i;
    INT32       i4_j;
    VOID*       apv_keyfld_addrs[MAX_NUM_KEY_PARM_FIELDS];
    UINT8*      pui1_src_addr=NULL;
    UINT8*      pui1_dest_addr=NULL;

    /*
      Assertion check.  The meta info in CDB database object
      should be equaled to the record definition use to
      create the database object.
    */
    if ( pt_do->ui1_field_count !=     ui1_count           ||
         pt_do->ui1_key_field_count != ui1_key_field_count
        )
    {
        return DBR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    else
    {
        /*
          Assertion check: The width of record field type
          should be the same, otherwise we will ABORT.
        */
        for ( i4_i=0 ; i4_i < ui1_count ; i4_i++ )
        {
            if (
                pt_do->pt_fields[i4_i].ui4_field_type !=
                at_field_info[i4_i].ui4_field_type           ||
                pt_do->pt_fields[i4_i].ui2_field_element_count !=
                at_field_info[i4_i].ui2_field_element_count
                )
            {
                return DBR_FILE_READ_FAIL;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
    }
    
    /*
      pt_meta contains description of the data field stored in
      the pui1_buf (read in from stored file.)
    */
    for ( i4_i=0 ; i4_i < ui1_count ; i4_i++ )
    {
        /*
          Find the record field from the persistent storage, and
          then copy the field from the persistent storage record buffer
          to the temp internal work record buffer.
        */

        /*
          The addr on the temp work buffer to copy the record field
          to.
        */
        pui1_dest_addr = pui1_tmp + pt_do->ui2_fld_offset[i4_i];
        
        for ( i4_j = 0 ; i4_j < pt_meta->ui1_fld_count ; i4_j++ )
        {
            /*
              Find the location (offset) of the field in the
              record read back from the persistent storage.
            */
            if ( pt_meta->pt_fld_info[i4_j].ui4_field_type ==
                 pt_do->pt_fields[i4_i].ui4_field_type )
            {
                /*
                  Assertion check.

                  Given the same field type, the width of field in the
                  persistent storage should be smaller or equal to the
                  width of field in memory. For backward compatibility,
                  record field can be expand but not shrink.
                */
                if ( pt_meta->pui2_width[i4_j] > pt_do->ui2_fld_width[i4_i] )
                {
                    return DBR_FILE_READ_FAIL;
                    /*
                    ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                    */
                }
                    
                pui1_src_addr =  pui1_buf + pt_meta->pui2_offset[i4_j];

                /*
                   Perform the endian conversion and un-compaction
                   on the field to be copy from filesystem buffer
                   to memory buffer.
                */
                db_recfld_from_big_end_to_host_end(pt_do,
                                                   i4_i,
                                                   pui1_dest_addr,
                                                   pt_meta,
                                                   i4_j,
                                                   pui1_src_addr,
                                                   pf_conv);

                /*
                  Exit the inner loop.
                */
                break;
            }
        }
        /*
           Save the address of the record field.
        */
        apv_recfld_addrs[i4_i]=pui1_dest_addr;

        /*
          Find the next record field
        */
    }

    /*
      Find the key field address from the array of record field addresses.
    */
    for (i4_j=0; i4_j < ui1_key_field_count ; i4_j++ )
    {
        for (i4_i=0 ; i4_i < ui1_count ; i4_i++ )
        {
            if ( aui4_key_field_types[i4_j] ==
                 pt_do->pt_fields[i4_i].ui4_field_type )
            {
                apv_keyfld_addrs[i4_j]=apv_recfld_addrs[i4_i];
                break;
            }
        }
    }
    
    /*
      Add the records to the CDB database.
    */
    i4_rc = db_fs_write_rec(h_cdb,
                            (const VOID**) apv_keyfld_addrs,
                            (const VOID**) apv_recfld_addrs);
    
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_recfld_from_big_end_to_host_end
 *
 * Description: API to convert a record field from Big-Endian format to 
 *              the CPU memory format.
 *
 * Inputs:  -
 *    pt_do      Pointer to a CDB data object in memory.
 *    i4_i       Index to a specific field in the CDB data object in memory.
 *    pui1_in_mem_rec   output buffer of the converted host-endian data to be
 *               store in the CPU memory order.
 *    pt_meta    Pointer to a CDB data information load from filesystem.
 *    i4_j       index to a specific field in the CDB data object from filesystem.
 *    pui1_fsys_rec:  input buffer containing input field data (corresponding to 
 *               'i4_j') loaded from filesystem.
 *
 *    pf_conv:   caller provided UNION_T field endian conversion function.
 *
 * Outputs: -
 *    pui1_buf:  buffer containing converted data in the Big-endian format.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_recfld_from_big_end_to_host_end
(
    CDB_DATA_OBJ_T*        pt_do,
    INT32                  i4_i,
    UINT8*                 pui1_in_mem_rec,
    DB_FILE_META_DATA_T*   pt_meta,
    INT32                  i4_j,
    const UINT8*           pui1_fsys_rec,
    db_endian_conv_fct     pf_conv
)
{
    INT32           i4_rc=DBR_OK;
    
    /*
      Perform the endian conversion.
    */
    switch ( DB_GET_C_TYPE(pt_meta->pt_fld_info[i4_j].ui4_field_type) )
    {
        case DBF_UINT8_T:
        case DBF_INT8_T:
        case DBF_CHAR_T:
        case DBF_ENUM8_T:
        case DBF_ARRAY_OF_UINT8_T:
        case DBF_ARRAY_OF_INT8_T:
        case DBF_ARRAY_OF_CHAR_T:
        case DBF_ARRAY_OF_ENUM8_T:
        {
            /*
              Single bytes, no endian conversion, so just copy
              the field to io buffer.
            */
#ifdef DEBUG
           if ( (pt_meta->pui2_width[i4_j]) !=
                pt_meta->pt_fld_info[i4_j].ui2_field_element_count )
           {
               ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
           }          
#endif            
            x_memmove(pui1_in_mem_rec,
                      pui1_fsys_rec,
                      pt_meta->pt_fld_info[i4_j].ui2_field_element_count);
        }
        break;

        case DBF_STRING_T:     /* string ID is stored as UINT16 */
        case DBF_MEMBUF_T:     /* object ID is also stored as UINT16. */
        {        
            /*
              2 bytes, perform endian conversion.
            */
            x_end_cnvt_from_big_16(pui1_fsys_rec, 1, pui1_in_mem_rec);
        }
        break;
                
        case DBF_UINT16_T:
        case DBF_INT16_T:
        case DBF_ENUM16_T:
        case DBF_UTF16_T:
        case DBF_ARRAY_OF_UINT16_T:
        case DBF_ARRAY_OF_INT16_T:
        case DBF_ARRAY_OF_ENUM16_T:
        {
#ifdef DEBUG
           if ( (pt_meta->pui2_width[i4_j])/2 !=
                pt_meta->pt_fld_info[i4_j].ui2_field_element_count )
           {
               ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
           }          
#endif                    
            /*
              2 bytes, perform endian conversion.
            */
            x_end_cnvt_from_big_16(pui1_fsys_rec,
                                   pt_meta->pt_fld_info[i4_j].ui2_field_element_count,
                                   pui1_in_mem_rec);
        }
        break;
                
        case DBF_UINT32_T:
        case DBF_INT32_T:
        case DBF_ENUM32_T:
        case DBF_UTF32_T:
        case DBF_UNION_TAG_T:
        case DBF_ARRAY_OF_UINT32_T:
        case DBF_ARRAY_OF_INT32_T:
        case DBF_ARRAY_OF_ENUM32_T:
        {
#ifdef DEBUG
           if ( (pt_meta->pui2_width[i4_j])/4 !=
                pt_meta->pt_fld_info[i4_j].ui2_field_element_count )
           {
               ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
           }          
#endif                  
            /*
              4 bytes, perform endian conversion.
            */
            x_end_cnvt_from_big_32(pui1_fsys_rec,
                                   pt_meta->pt_fld_info[i4_j].ui2_field_element_count,
                                   pui1_in_mem_rec);
        }
        break;

        case DBF_UINT64_T:
        case DBF_INT64_T:
        case DBF_ARRAY_OF_UINT64_T:
        case DBF_ARRAY_OF_INT64_T:
        {
#ifdef DEBUG
           if ( (pt_meta->pui2_width[i4_j])/8 !=
                pt_meta->pt_fld_info[i4_j].ui2_field_element_count )
           {
               ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
           }          
#endif                  
            /*
              8 bytes, perform endian conversion.
            */
            x_end_cnvt_from_big_64(pui1_fsys_rec,
                                   pt_meta->pt_fld_info[i4_j].ui2_field_element_count,
                                   pui1_in_mem_rec);
        }
        break;                

        case DBF_UNION_T:
        {
            DB_UNION_TAG_T  t_union_tag;
            UINT8*          pui1_union_tag_rec;

            /*
              The previous record field (from the current UNION_T
              field)  must be a UNION_TAG_T.
            */
            if ( DB_GET_C_TYPE(pt_do->pt_fields[i4_i-1].ui4_field_type)
                 != DBF_UNION_TAG_T )
            {
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_MISSING_UNION_TAG_FLD);
            }

            /*
              The previous record field (from the current UNION_T
              field)  must be a UNION_TAG_T.
            */
            if ( DB_GET_C_TYPE(pt_meta->pt_fld_info[i4_j-1].ui4_field_type)
                 != DBF_UNION_TAG_T )
            {
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_MISSING_UNION_TAG_FLD);
            }

            /* Assertion check: for an UNION field, the caller
               must provide a function to interpret the
               content of the union, to convert to the proper
               byte order.
            */
            if ( pf_conv == NULL )
            {
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_NO_UNION_INTREP_FCT);
            }
                    
            /*
              The previous field must be a UNION_TAG_T, get the
              address of the UNION_TAG_T field (which is 4 bytes)
              by decrementing the address of the UNION field by
              4 bytes.
            */
            pui1_union_tag_rec= (UINT8*)(pui1_fsys_rec - 4);
            x_memcpy(&t_union_tag, pui1_union_tag_rec, 4);

            /* convert the 4 byte to host-endian. */
            x_end_cnvt_from_big_32(&t_union_tag, 1 , &t_union_tag);
                    
            /*
              call user provided function to interpret
              union field.
            */
            pf_conv(
                pt_meta->pt_fld_info[i4_j].ui4_field_type,
                t_union_tag,
                pui1_fsys_rec,
                pt_meta->pui2_width[i4_j],
                pui1_in_mem_rec);
        }
        break;

        /*
          Handle all other types, FLOAT32, FLOAT64,etc)
        */
        default:
        {
            /*
              copy the field to the output buffer without endian
              conversion.
            */
            x_memmove(pui1_in_mem_rec,
                      pui1_fsys_rec,
                      pt_meta->pui2_width[i4_j]);
        }
        break;
    }
    return i4_rc;
}


/*--------------------------------------------------------------------
 * Name: db_persisten_obj_size
 *
 * Description: API to return the size (bytes) that this CDB object is
 *              would required to write to a persistent storage.
 *
 * Inputs:  -
 *    pt_do:    Pointer to the CDB object to write to persistent storage.
 *
 * Outputs: - None.
 *
 * Returns: -   Size of CDB object when written to persistent storage.     
 *            
 ----------------------------------------------------------------------*/

UINT32 db_persistent_obj_size
(
    CDB_DATA_OBJ_T*        pt_do,
    db_endian_conv_fct     pf_conv,
    BOOL                   b_cmpres 
)
{
    UINT32         ui4_total_sz;
    UINT32         ui4_data_sect_sz;

    ui4_data_sect_sz = db_get_data_sect_size_v2(pt_do, pf_conv, b_cmpres);
    
    ui4_total_sz = CDB_FILE_HEADER_SIZE                        +
                   db_get_meta_sect_size(pt_do)                +
                   ui4_data_sect_sz  +
                   CDB_FILE_TAIL_MARKER_SIZE ;
#if 1
    DBG_INFO(("{CDB} persistent object size:: header size [%d bytes] + meta_data size [%d bytes]\n",
              CDB_FILE_HEADER_SIZE,db_get_meta_sect_size(pt_do)));
    DBG_INFO(("data section size [%d bytes] + File tail marker size: [%d bytes]\n",
               ui4_data_sect_sz, CDB_FILE_TAIL_MARKER_SIZE));
    DBG_INFO(("Total size: [%d bytes]\n", ui4_total_sz));
    
#endif
    return ui4_total_sz;    
}


/*-----------------------------------------------------------------------------
 * Name: db_check_read_write_lock
 *
 * Description: API to check if read or write lock has been grabbed.
 *
 * Inputs:  -
 *   h_file:   handle to the file.
 *
 * Outputs: - None.
 *                           
 * Returns: - 
 *   TRUE   The current thread has grabbed the write lock, or that
 *          read lock has been grabbed by some thread(s).
 * 
 *   FALSE  The current thread has the write lock, or read lock is
 *          grabbed.
 ----------------------------------------------------------------------------*/
static
BOOL db_check_read_write_lock
(
    HANDLE_T         h_file
)
{
    BOOL             b_result = FALSE;
    /*
      Check if the write lock is grabbed.
    */
    x_fm_grabbed(h_file, FM_LOCK_WRITE, &b_result);
    if ( b_result == FALSE )
    {
        /*
          Check if the read lock is grabbed.
        */
        x_fm_grabbed(h_file, FM_LOCK_READ, &b_result);
        if ( b_result == FALSE )
        {
            /* both read and write lock has not been grabbed. */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_INV_STATE);
        }
    }
    return b_result;
}

/*-----------------------------------------------------------------------------
 * Name: db_check_write_lock
 *
 * Description: API to check if write lock has been grabbed by the current
 *              thread.
 *
 * Inputs:  -
 *   h_file:   handle to the file.
 *
 * Outputs: - None.
 *                           
 * Returns: - 
 *   TRUE   The current thread has grabbed the write lock.
 * 
 *   FALSE  The current thread has not grab the write lock.
 ----------------------------------------------------------------------------*/
static
BOOL db_check_write_lock
(
    HANDLE_T         h_file
)
{
    BOOL             b_result = FALSE;
    /*
      Check if the write lock is grabbed.
    */
    x_fm_grabbed(h_file, FM_LOCK_WRITE, &b_result);
    if ( b_result == FALSE )
    {
        /* write lock has not been grabbed. */
        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INV_STATE);
    }
    return b_result;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_end_magic_num_for_read_ops
 *
 * Description: API to read the end magic number. NOTE: the file pointer
 *              must be pointed to the start of data section when this
 *              function is called.
 *              After return from this API, the file pointer will remain
 *              at the same currentllocation.
 *
 * Inputs:  -
 *   h_file:   handle to the file. It is position at the start of the
 *             data section of the CDB object.
 *   ui4_data_sz: the bytes to skip from the current location of the
 *                file pointer in order to read the end magic number
 *                and the CRC value.
 *
 * Outputs: -
 *   pui4_end_magic_num: pointer to a variable holding the end magic value.
 *                           
 * Returns: - 
 *   DBR_OK   The end-magic value is read.
 *   DBR_FILE_READ_FAIL   read operation failed.
 ----------------------------------------------------------------------------*/
INT32 db_get_end_magic_num_for_read_ops
(
    HANDLE_T          h_file,
    UINT32            ui4_data_sz,
    UINT32*           pui4_end_magic_num
)
{
    UINT64  ui8_fpos;
    UINT64  ui8_magic_fpos;
    UINT64  ui8_end_fpos;
    UINT32  ui4_byte_read;
    UINT8   aui1_buf[CDB_FILE_TAIL_MARKER_SIZE] ;
    UINT32  ui4_magic;
    UINT32  ui4_crc;
    INT32   i4_rc;
    
    
    ui4_magic = 0;
    *pui4_end_magic_num = 0;
    i4_rc = DBR_FILE_READ_FAIL;
    
    /*
      Remember the current locaiton of the File pointer.
    */
    if ( x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_fpos) == FMR_OK )
    {
        /*
          The position of the end-magic number is number of bytes
          of the data section from the current position.  Note:
          the current position is at the end of metadata section.
        */
        ui8_end_fpos = ui4_data_sz ;
        
        /*
          goto the end of CDB object to read the end-magic and crc
          value.
        */
        if ( x_fm_lseek(h_file, (INT64)ui8_end_fpos,
                        FM_SEEK_CUR, &ui8_magic_fpos) == FMR_OK
            )
        {
            if ( x_fm_read(h_file, aui1_buf, sizeof(aui1_buf),
                           &ui4_byte_read) == FMR_OK            &&
                 ui4_byte_read == sizeof(aui1_buf)
                )
            {
                /* convert from big endian. */
                x_end_cnvt_from_big_32(&(aui1_buf[0]), 1, &ui4_crc);
                x_end_cnvt_from_big_32(&(aui1_buf[4]), 1, &ui4_magic);

                /*
                   Don't forget to re-position the file pointer to
                   the original position.
                */
                if (  x_fm_lseek(h_file, (INT64)ui8_fpos,
                                 FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
                    )
                {
#ifdef DEBUG
                    /*
                    x_dbg_stmt("{CDB} db_get_end_magic_num_for_read_ops: reset to file position %lld. %lld \n",ui8_fpos, ui8_end_fpos );
                    */
#endif                    
                    *pui4_end_magic_num = ui4_magic;
                    i4_rc = DBR_OK;
                }
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_beg_magic_num_for_write_ops
 *
 * Description: API to read the beginning magic number. NOTE: the file 
 *              pointer must be pointed to the start of the CDB object.
 *              After return from this API, the file pointer will
 *              remain at the beginning of of magic number.
 *
 * Inputs:  -
 *   h_file:   handle to the file. It is position at the start of the
 *             of the CDB object.
 *
 * Outputs: -
 *   pui4_beg_magic_num: pointer to a variable containing the beginning
 *                       magic value..
 *                           
 * Returns: - 
 *   DBR_OK   The end-magic value is read.
 *   DBR_FILE_READ_FAIL   
 ----------------------------------------------------------------------------*/
INT32  db_get_beg_magic_num_for_write_ops
(
    HANDLE_T          h_file,
    UINT32*           pui4_beg_magic_num
)
{
    UINT64  ui8_fpos;
    UINT64  ui8_end_fpos;
    UINT32  ui4_byte_read;
    UINT8   aui1_buf[4] ;
    UINT32  ui4_magic;
    INT32   i4_rc;
    INT32   i4_frc;
    
    
    ui4_magic = 0;
    *pui4_beg_magic_num = 0;
    i4_rc = DBR_FILE_READ_FAIL;
    
    /*
      Remember the current locaiton of the File pointer.
    */
    if ( (i4_frc=x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_fpos)) == FMR_OK )
    {        
        /*
          Read the first 4 bytes
        */
        if ( (i4_frc=x_fm_read(h_file, aui1_buf, sizeof(aui1_buf),
                       &ui4_byte_read) ) == FMR_OK               &&
             ui4_byte_read == sizeof(aui1_buf)
            )
        {
            /* convert from big endian. */
            x_end_cnvt_from_big_32(&(aui1_buf[0]), 1, &ui4_magic);

            /*
              Don't forget to re-position the file pointer to
              the original position.
            */
            if (  x_fm_lseek(h_file, (INT64)ui8_fpos,
                             FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
                )
            {
                *pui4_beg_magic_num = ui4_magic;
                i4_rc = DBR_OK;
            }
        }
        else
        {
            /*
              For regular file system (e.g., RAM), the position of file pointer
              to start the write might already be extended beyon the current
              file limit.  So we need to check if the file position is byond
              end-of-file.  If it is beyond EOF, then we should just
              return 0 magic number.
            */
            if ( i4_frc == FMR_EOF )
            {
                /*
                  make sure the file position is at the start of the CDB
                  object.
                */
                if (  x_fm_lseek(h_file, (INT64)ui8_fpos,
                                 FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
                )
                {
                    *pui4_beg_magic_num = 0;
                    i4_rc = DBR_OK;
                }
            }
        }   
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_is_cdb_file_need_saving
 *
 * Description: API to check if the database stored on the file
 *              and the database in memory has same db version number
 *              and metadata info.  If the db version number or metadata
 *              is differenet, then return TRUE to indicate that writing
 *              to filesystem will be needed. 
 *
 * Inputs:  -
 *   pt_do:    CDB object in memory that will be store to the file.
 *
 *   h_file:   handle to the file. It is position at the start of the
 *             of the CDB object.
 *   
 *
 * Outputs: -
 *   pb_result: flag indicating if this CDB object need to be written
 *              to the file.
 *                           
 * Returns: - 
 *   DBR_OK   The operation is success.
 *   DBR_FILE_WRITE_FAIL   
 ----------------------------------------------------------------------------*/
INT32 db_is_cdb_file_need_saving
(
    CDB_DATA_OBJ_T*     pt_do,
    HANDLE_T            h_file,
    db_endian_conv_fct  pf_conv,
    BOOL                b_cmpres,
    BOOL*               pb_result,
    UINT32*             pui4_cdb_obj_sz_on_file
)
{
    UINT64              ui8_fpos;
    UINT64              ui8_x_fpos;
    UINT32              ui4_crc;
    INT32               i4_i;
    INT32               i4_rc;
    DB_FILE_META_DATA_T t_meta;
    UINT8               ui1_file_ver_read;

    *pui4_cdb_obj_sz_on_file = 0;
    
    /*
      Remember the current locaiton of the File pointer.
    */
    if ( x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_fpos) != FMR_OK )
    {
        /*
          Fail to get the current position of the file pointer,
          return WRITE_FAIL so no write will be performed.
        */
        *pb_result = FALSE;
        i4_rc = DBR_FILE_WRITE_FAIL;
        return i4_rc;
    }
    else
    {
#ifdef DEBUG
        /*
        x_dbg_stmt("{CDB} db_is_cdb_file_need_saving: file location for CDB object: %lld.\n", ui8_fpos );
        */
#endif        
    }

    
    /*
      initialize the CRC seed value and metadata buffer.
    */
    ui4_crc = 0xFFFFFFFF;
    db_init_metabuf(&t_meta);
    
    /*
       Get meta data section of CDB file object.
    */
    if ( db_get_metadata_from_file(h_file, NULL, &t_meta,
                                    &ui1_file_ver_read, &ui4_crc)
         != DBR_OK )
    {
        /*
           Can not get CDB metadata info, this could be a new file
           location or the file location is some other type of file
           that is not CDB file object, therefore write operation is
           needed.
           
           Before returning, reset the file pointer to the start of
           metadata section.  If there is an error reseting the file
           pointer, then return an error status and set write op flag
           to FALSE.
        */
        if (  x_fm_lseek(h_file, (INT64)ui8_fpos, FM_SEEK_BGN, &ui8_x_fpos) != FMR_OK )
        {
            /*
              Fail to re-set to the original position of the file pointer,
              return WRITE_FAIL so no write will be performed.
            */
            *pb_result = FALSE;
            i4_rc = DBR_FILE_WRITE_FAIL;
            
#ifdef DEBUG            
            x_dbg_stmt("{CDB} db_is_cdb_file_need_saving->Error can not reset file pointer to beginning of CDB object: %lld\n", ui8_fpos );
#endif
            
        }
        else
        {
            
#ifdef DEBUG
            /*
            x_dbg_stmt("{CDB} db_is_cdb_file_need_saving: reset file pointer to beginning of CDB object: %lld %lld\n", ui8_fpos, ui8_x_fpos );
            */
#endif            
            
            *pb_result = TRUE ;
            i4_rc = DBR_OK;
        }
        db_free_metabuf(&t_meta);    
        return i4_rc;
    }
    else
    {
        /*
          The file location contain an existing CDB object,
          reset the file pointer to the start of metadata section. If an
          error occurs during file pointer reset, return an error status.
        */
        if (  x_fm_lseek(h_file, (INT64)ui8_fpos, FM_SEEK_BGN, &ui8_x_fpos) != FMR_OK )
        {
            /*
              Fail to re-set to the original position of the file pointer,
              return WRITE_FAIL so no write will be performed.
            */
            *pb_result = FALSE;
            i4_rc = DBR_FILE_WRITE_FAIL;
            
#ifdef DEBUG           
            x_dbg_stmt("{CDB} db_is_cdb_file_need_saving->Error can not reset file pointer to beginning of CDB object: %lld\n", ui8_fpos );
#endif            
            db_free_metabuf(&t_meta);    
            return i4_rc;
        }
        else
        {
            
#ifdef DEBUG
            /*
            x_dbg_stmt("{CDB} db_is_cdb_file_need_saving: reset file pointer to beginning of CDB object: %lld %lld\n", ui8_fpos, ui8_x_fpos );
            */
#endif
            
            i4_rc = DBR_OK;
        }
    }
    

    /*
      Now check the header informaiton from the file with
      the database object to determine if writting to file
      is needed. Set default value to write to the file.
    */
    *pb_result = TRUE;
    
    if ( i4_rc ==  DBR_OK )
    {
        /****************************************************************************
           Compute the size of CDB object found on the file system. We
           compute this value here instead of calling db_persistent_obj_size()
           function to save some computation.   Note: the code below should
           sync with the code in db_persistent_obj_size() to ensure size
           computation is correct.
        *****************************************************************************/
        
        *pui4_cdb_obj_sz_on_file = CDB_FILE_HEADER_SIZE + db_get_meta_sect_size(pt_do) +
            t_meta.ui4_data_sz  + CDB_FILE_TAIL_MARKER_SIZE ;

        /*************************************************************************/
        
        if ( (ui1_file_ver_read  == CDB_FILE_VER)                      &&
             (t_meta.ui4_db_version == pt_do->ui4_db_version )         &&
             (t_meta.ui1_keyfld_count == pt_do->ui1_key_field_count )  &&
             (t_meta.ui1_fld_count == pt_do->ui1_field_count )         &&
             (t_meta.ui2_max_count == pt_do->ui2_max_count )           &&
             (t_meta.ui2_rec_count == pt_do->ui2_record_count )        &&
             (t_meta.ui2_rec_width == pt_do->ui2_record_width )        &&
             (t_meta.ui2_common_sz == pt_do->ui2_common_size )         /* &&
             (t_meta.ui4_data_sz == db_get_data_sect_size_v2(pt_do,pf_conv,b_cmpres) ) */
            )
        {
            UINT32    ui4_data_sect_sz = 0 ;

            ui4_data_sect_sz = db_get_data_sect_size_v2(pt_do,pf_conv,b_cmpres);
            
            /* check the data section size. */
            if ( t_meta.ui4_data_sz !=  ui4_data_sect_sz )
            {
                x_dbg_stmt("{CDB} object size in memory [%d bytes] differs from object size in file [%d bytes]\n",
                           ui4_data_sect_sz, t_meta.ui4_data_sz);
                
                *pb_result=TRUE;
                db_free_metabuf(&t_meta);    
                return i4_rc;
            }
            
            /*
              Compare the record field type, width, and offset.
            */
            for ( i4_i = 0 ; i4_i < t_meta.ui1_keyfld_count ; i4_i++ )
            {
                if ( t_meta.pui4_keyfld_types[i4_i] != pt_do->aui4_key_field_types[i4_i] )
                {
                    *pb_result=TRUE;
                    db_free_metabuf(&t_meta);    
                    return i4_rc;
                }
            }         

            /*
              Compare the record field type, width, and offset.
            */
            for ( i4_i = 0 ; i4_i < t_meta.ui1_fld_count ; i4_i++ )
            {

                if ( (t_meta.pt_fld_info[i4_i].ui4_field_type !=
                      pt_do->pt_fields[i4_i].ui4_field_type)                   ||
                     (t_meta.pt_fld_info[i4_i].ui2_field_element_count !=
                      pt_do->pt_fields[i4_i].ui2_field_element_count)          ||
                     (t_meta.pui2_offset[i4_i] != pt_do->ui2_fld_offset[i4_i]) ||
                     (t_meta.pui2_width[i4_i] != pt_do->ui2_fld_width[i4_i])
                    )
                {
                    *pb_result=TRUE;
                    db_free_metabuf(&t_meta);    
                    return i4_rc;
                }
            }

            /*
              Pass all comparison. The CDB object in memory
              and stored in the file is the same (e.g., not dirty), so
              no actual write ops is needed.
            */
            *pb_result = FALSE;

            DBG_INFO(("{CDB} CDB object in memory and in the file is the same (e.g., not dirty), so no actual write ops is needed.\n"));
            
            db_free_metabuf(&t_meta);    
            return i4_rc;
        }
    }
    db_free_metabuf(&t_meta);    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name:  db_fs_write_rec
 *
 * Description: This API write a record read from filesystem
 *   into the database object. For field
 *   that is of STRING_T type, it only copy the string_id, but it does
 *   not actually create the string in the string buffer. The reason
 *   for this is that hte string buffer will be created by reading
 *   in from the filesystem.
 *
 * Inputs:  - h_client:  client handle
 *            apv_key_field_values:  values for the key parameter. Note:
 *                                   If multiple fields make up a key
 *                                   parameter, the order of values
 *                                   has to be in the same order that
 *                                   key types are specified when
 *                                   the database was created.
 *            apv_field_values:  values of the record fields.
 *            ui1_write_mode: Specify if the record to be written is
 *                            a new record or update an existing
 *                            record.
 *
 * Outputs: - None.
 *
 * Returns: - DBR_OK     A new lock is initialized successfully.
 *            DBR_INV_HANDLE Bad handle.
 *            DBR_INV_ARG  key_field_values or field_values array is null.
 *            DBR_REC_NOT_FOUND The specified record can not be found.
 *            DBR_OUT_OF_MEMORY: can not allocate new memory.
 *           
 ----------------------------------------------------------------------------*/
static INT32 db_fs_write_rec
(
    HANDLE_T          h_client,
    const VOID*       apv_key_field_values[],
    const VOID*       apv_field_values[]
)
{
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;
    HANDLE_T          h_current_thread;
    UINT8*            pui1_record;
    INT32             i4_rc;
    BOOL              b_new_rec;
    
    i4_rc=DBR_FAIL;
    pui1_record=NULL;

    if ( apv_key_field_values != NULL && apv_field_values != NULL )
    {
        if ( (handle_get_type_obj ( h_client, &pe_type,
                                   ((VOID**) &pt_co)) == HR_OK)    &&
             (pe_type == CDBT_COREDB )                        
            )
        {
            /* Gets the current thread id value. Use the thread id
               to check if the write lock has been acquired.    */
            x_thread_self(&h_current_thread);
        
            if ( pt_co->h_write_thread == h_current_thread )
            {
                pt_do=pt_co->p_data_obj;
                
                /*
                  Get the next free block
                */
                i4_rc=db_get_next_freeblock(pt_do->t_memory,
                                            &(pui1_record));
                if ( i4_rc != DBR_OK )
                {   /* Out of memory. */
                    return DBR_OUT_OF_MEMORY;
                }
                else
                {
                    /*
                      Increment the number of records added.
                    */
                    pt_do->ui2_last_add_count++;
                    b_new_rec=TRUE;
                }
                    
                /*
                  copy the field values into the record block.
                */
                i4_rc=db_fs_write_to_record_entry(pt_do,
                                                  pui1_record,
                                                  apv_field_values,
                                                  b_new_rec);
            }
            else
            {
                /* No write lock or wrong thread is doing the
                   writing.
                */
                ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_NO_WRITE_LOCK);
            }
        }
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_client))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
            i4_rc=DBR_INV_HANDLE;
        }
    }
    else
    {
        i4_rc=DBR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_fs_write_to_record_entry
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
static INT32 db_fs_write_to_record_entry
(
    CDB_DATA_OBJ_T*   pt_do,
    UINT8*            pui1_record, 
    const VOID*       apv_field_values[],
    BOOL              b_new_rec
)
{
    UINT8      ui1_i;
    UINT16     ui2_offset;
    UINT16     ui2_width;

    
    for ( ui1_i=0; ui1_i < pt_do->ui1_field_count; ui1_i++ )
    {
        ui2_offset=pt_do->ui2_fld_offset[ui1_i];
        ui2_width=pt_do->ui2_fld_width[ui1_i];
        
        x_memcpy((pui1_record+ui2_offset),
                 apv_field_values[ui1_i],
                 ui2_width);
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
 * Name: db_get_data_sect_size_v2
 *
 * Description: This API compute the size (in bytes) of the record data 
 *              section in the CDB file.
 *
 * Inputs:  - pt_do:   Pointer to the CDB control block structure. 
 *
 * Outputs: - None.
 *
 * Returns: - Size of the record data section.
 *
 ----------------------------------------------------------------------------*/

UINT32 db_get_data_sect_size_v2
( 
    CDB_DATA_OBJ_T*        pt_do,
    db_endian_conv_fct     pf_conv,
    BOOL                   b_cmpres 
)
{
    INT32                  i4_rc;
    UINT32                 ui4_sz = 0;
    SIZE_T                 z_data_len = 0;
    BOOL                   b_recompute = TRUE;


    DBG_INFO(("{CDB} get CDB data section size (record, string buffer, memory buffer).\n"));
    
    /*
      Check if there is cached data section size information.
      If this information is available, use this instead of
      computing it again.
    */
    if (  (pt_do->t_dsect_info.b_valid_sect_sz == TRUE)   &&
          (pt_do->t_dsect_info.ui4_db_version == pt_do->ui4_db_version)  &&
          (pt_do->t_dsect_info.b_cmpres  == b_cmpres)
        )
    {
        if ( b_cmpres == TRUE )
        {
            if ( x_strcmp(pt_do->t_dsect_info.s_cmpres_eng,
                          pt_do->s_cmpres_eng)   == 0 )
            {
                b_recompute = FALSE;
                z_data_len = pt_do->t_dsect_info.ui4_data_len;
                i4_rc = DBR_OK;
            }
        }
    }

    /* Call write data section function to compute the size
       of the data section (which can be compressed) that will
       be written out to file system.
    */
    if ( b_recompute == TRUE )
    {
        i4_rc = db_write_data_sect(pt_do, NULL_HANDLE,
                               pf_conv, NULL,
                               b_cmpres,
                               FALSE,  /* don't write data */
                                   &z_data_len);
    }

    if ( i4_rc == DBR_OK )
    {
        ui4_sz = z_data_len;
    }

    DBG_INFO(("{CDB} CDB data section (record, string buf, customer data) [%d bytes].\n",
             ui4_sz));

    return ui4_sz;
}

/*-----------------------------------------------------------------------------
 * Name: db_write_data_sect
 *
 * Description: Internal API to write out the data section.            
 *
 *
 * Inputs:
 *    pt_do:    database object.
 *    h_file:   field handle.
 *    pui4_crc: CRC value computed over the file head data.
 *            
 * Outputs:
 *     pui4_crc: CRC value computed over the file head data.
 *    pz_data_len:  The length of data section                       
 *
 * Returns: - DBR_OK:  success   
 *            DBR_FAIL: the specified field type is incorrect.   
 *            
 *            
 ----------------------------------------------------------------------------*/
static INT32 db_write_data_sect
(
    CDB_DATA_OBJ_T*           pt_do,
    HANDLE_T                  h_file,
    db_endian_conv_fct        pf_conv,
    UINT32*                   pui4_crc,
    BOOL                      b_cmpres,
    BOOL                      b_write,
    SIZE_T*                   pz_data_len
)
{
    UINT32              ui4_i=0;
    INT32               i4_rc;
    SIZE_T              z_cmp_data_len  = 0 ;
    SIZE_T              z_output_len  = 0;
    CDB_MEM_CURSOR_T    t_mem_cursor;
    UINT8*              pui1_rec;
    DB_IO_BUF*          pt_io_buf = NULL;
    UINT8*              pui1_buf;
    HANDLE_T            h_zip = NULL_HANDLE;
    CHAR                s_cmpres_eng[CMPRES_ENG_NAME_LEN];
    
    i4_rc = DBR_OK;

    x_memset(s_cmpres_eng,0,CMPRES_ENG_NAME_LEN);
    if ( b_cmpres == TRUE )
    {
        /* If the compression engine name is not a valid one (e.g, not
           support by the system), then 'b_cmpres' flag will be changed
           to FALSE, so the data will be saved in un-compress format.

           Note: the name of 'compression engine' will be a null string
           (all \0) in the meta data in the file if no compression is
           applied to data.
        */
        if (db_open_cmpres_eng(pt_do->s_cmpres_eng, &h_zip) == DBR_OK )
        {
            x_strncpy(s_cmpres_eng, pt_do->s_cmpres_eng, CMPRES_ENG_NAME_LEN-1);
        }
        else
        {
            b_cmpres = FALSE;
        }
    }
    z_cmp_data_len += CMPRES_ENG_NAME_LEN ;

    
    if ( b_write )
    {
        UINT32      ui4_byte_wrote = 0;

        i4_rc=x_fm_write(h_file,
                         s_cmpres_eng,
                         CMPRES_ENG_NAME_LEN,
                         &ui4_byte_wrote);
        if ( (i4_rc          == FMR_OK)                 &&
             (ui4_byte_wrote == CMPRES_ENG_NAME_LEN)
            )
        {     
            i4_rc = DBR_OK;
        }
        else
        {
            i4_rc = DBR_FILE_WRITE_FAIL ;
        }
    }   


    ui4_i=0;
    /* Write out CDB records one by one. */
    if ( pt_do->ui2_record_count > 0)
    {
        
        DBG_INFO(("{CDB::FileIO} number of records: [%d]\n",
                 pt_do->ui2_record_count));
        
        /*
           create a IO buffer that should at least fit one record entry.
        */
        if ( pt_do->ui2_record_width > DB_IO_BUF_SZ )
        {
            i4_rc = db_create_io_buf(pt_do->ui2_record_width, &pt_io_buf);
        }
        else
        {
            i4_rc = db_create_io_buf(DB_IO_BUF_SZ, &pt_io_buf);
        }

        if ( i4_rc == DBR_OK )
        {
            
            /*
              Set the memory cursor to the first record in the
              memory chunk.
            */
            db_set_cursor_start(pt_do->t_memory,&t_mem_cursor);
         
            while (1)
            {
                i4_rc=db_get_next_used_block(pt_do->t_memory,
                                             &t_mem_cursor,
                                             &pui1_rec);
                
                if ( i4_rc == DBR_OK && pui1_rec != NULL )
                {
                    /* Get the addr to write into the io buffer. */
                    pui1_buf=db_get_write_ptr(pt_io_buf,
                                              pt_do->ui2_record_width);
                    /*
                      If IO buffer is full, then flush the content of
                      IO buffer to file system.
                    */
                    if ( pui1_buf == NULL )
                    {
                        i4_rc = db_flush_io_buf_with_cmpres(pt_io_buf,
                                                            h_file,
                                                            pui4_crc,
                                                            b_cmpres,
                                                            h_zip,
                                                            &z_output_len,
                                                            b_write,
                                                            DB_CMPRES_REC_DATA);                    
                        if ( i4_rc == DBR_OK )
                        {
                            z_cmp_data_len += z_output_len;
                            
                            pui1_buf=db_get_write_ptr(pt_io_buf,
                                                      pt_do->ui2_record_width);
                            if ( pui1_buf == NULL )
                            {
                                /* something is wrong. */
                                x_dbg_stmt("{CDB} Write Error: no space in IO buffer.\n");
                                i4_rc = DBR_FILE_WRITE_FAIL;
                                break;
                            }
                        }
                        else
                        {
                            /* encounter error while writing IO buffer
                               to file. Set the error condition and
                               break out the while loop.
                            */
                            i4_rc = DBR_FILE_WRITE_FAIL;
                            break;
                        }   
                    }
                    /*
                      Perform endian conversion.
                    */
                    db_rec_to_big_endian(pt_do, pui1_rec, pui1_buf, pf_conv);
                    /* x_memcpy(pui1_buf,pui1_rec,pt_do->ui2_record_width); */
                    

                    /*
                      increment the record count write out to the
                      file IO buffer.
                    */
                    ui4_i++;
                }
                else
                {
                    /* No more records left, don'f forget to flush the
                       content of IO buffer to file.. */
                    i4_rc = db_flush_io_buf_with_cmpres(pt_io_buf,
                                                        h_file,
                                                        pui4_crc,
                                                        b_cmpres,
                                                        h_zip,
                                                        &z_output_len,
                                                        b_write,
                                                        DB_CMPRES_REC_DATA);
                    if ( i4_rc == DBR_OK )
                    {
                        z_cmp_data_len += z_output_len;
                    }
                    else
                    {
                        i4_rc = DBR_FILE_WRITE_FAIL;
                    }
                    
                    /* Finish writing all records. */
                    break;
                }
            }          
        }
    }
    
    /*
       Check to make sure the number of record in the CDB and
       number of record written out is the same.
    */
    DBG_INFO(("{CDB::FileIO} total number of records processed: [%d]\n", ui4_i));

    if ( i4_rc == DBR_OK  &&  ui4_i == pt_do->ui2_record_count )
    {
        i4_rc=DBR_OK;
    }
    else
    {
        i4_rc=DBR_FILE_WRITE_FAIL ;
    }


    /*
      write out the string buffer.
    */
    if ( (i4_rc == DBR_OK)                    &&
         (pt_do->pt_strbuf != NULL)           &&
         (pt_do->pt_strbuf->ui2_nb_str > 0)
        )
    {
        UINT32           aui4_byte_to_copy[2];
        UINT32           aui4_big_end[2];
        UINT8*           aui1_addr_vec[2];
        UINT16           ui2_nb_of_str;
        
        /*
          CDB records loaded. Now we need to load the string
          object buffer.

          The string object buffer is stored as
          4 byte size, 4 bytes, n bytes of object string info,
          and n bytes of data.
        */
        x_str_buf_serialize(pt_do->pt_strbuf, aui4_byte_to_copy,
                            aui1_addr_vec);

        /***********************  cmpres string buf  ********************/
        if ( (aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8) > DB_IO_BUF_SZ )
        {
            /* delete existing IO buffer and create a new one. */
            db_delete_io_buf(pt_io_buf);
            
            i4_rc = db_create_io_buf(aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8,
                                     &pt_io_buf);
        }
        else
        {
            /* clear the existing IO buffer so we can re-use it. */
            i4_rc = db_clear_io_buf(pt_io_buf);
        }

        if ( i4_rc == DBR_OK )
        {
            x_end_cnvt_to_big_32(aui4_byte_to_copy,2,aui4_big_end);
            
            pui1_buf=db_get_write_ptr(pt_io_buf, 2*sizeof(UINT32));
            if ( pui1_buf != NULL )
            {
                x_memcpy(pui1_buf, aui4_big_end, 2*sizeof(UINT32));
                /*
                  x_dbg_stmt("{CDB} write out string data: obj array sz: %d string buffer sz: %d\n",
                       aui4_byte_to_copy[0],
                       aui4_byte_to_copy[1]);
                */
                if ( aui4_byte_to_copy[0] > 0 )
                {
                    /****  cmpres string buf: string object location array  ******/
                    pui1_buf = db_get_write_ptr(pt_io_buf, aui4_byte_to_copy[0]);
                    if ( pui1_buf != NULL )
                    {
                        ui2_nb_of_str = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));

                        /*
                           write out the string object location data, convert
                           the internal data array to Big endian.
                        */
                        x_end_cnvt_to_big_32(aui1_addr_vec[0],
                                             ui2_nb_of_str,
                                             pui1_buf);

                        /****  cmpres string buf: string data array  ******/
                        pui1_buf=db_get_write_ptr(pt_io_buf,aui4_byte_to_copy[1]);
                        if ( pui1_buf != NULL )
                        {
                            x_memcpy(pui1_buf,aui1_addr_vec[1],aui4_byte_to_copy[1]);
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{CDB} Write Error:  no space in io_buffer.\n");
                            i4_rc = DBR_FILE_WRITE_FAIL ;
                        }
                    }
                    else
                    {
                        x_dbg_stmt("{CDB} Write Error:  no space in io_buffer.\n");
                        i4_rc = DBR_FILE_WRITE_FAIL ;
                    }
                }

                if ( i4_rc == DBR_OK )
                {
                    i4_rc = db_flush_io_buf_with_cmpres(pt_io_buf,
                                                        h_file,
                                                        pui4_crc,
                                                        b_cmpres,
                                                        h_zip,
                                                        &z_output_len,
                                                        b_write,
                                                        DB_CMPRES_STRING_DATA);
                    if ( i4_rc == DBR_OK )
                    {
                        z_cmp_data_len += z_output_len;
                    }    
                }
            }
            else
            {
                x_dbg_stmt("{CDB} Write Error:  no space in IO_buffer.\n");
                i4_rc = DBR_FILE_WRITE_FAIL ;
            }
        }
        else
        {
            x_dbg_stmt("{CDB} Write Error: can not create IO buf for writing out string buffer.\n");
            i4_rc = DBR_FILE_WRITE_FAIL ;
        }      
    }


    /*
      write out the memory buffer (if there are data).
    */
    if ( (i4_rc == DBR_OK)                    &&
         (pt_do->pt_membuf != NULL)           &&
         (pt_do->pt_membuf->ui2_nb_obj > 0)
        )
    {
        UINT32           aui4_byte_to_copy[2];
        UINT32           aui4_big_end[2];
        UINT8*           aui1_addr_vec[2];
        UINT16           ui2_nb_of_obj;
        
        /*
          CDB records and string buffer saved to file.
          Now write out the memory object buffer.

          The memory object buffer is stored as
          4 byte size, 4 bytes, n bytes of object info,
          and n bytes of data.
        */
        x_mem_buf_serialize(pt_do->pt_membuf, aui4_byte_to_copy,
                            aui1_addr_vec);

        /***********************  memory buf  ********************/
        if ( (aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8) > DB_IO_BUF_SZ )
        {
            /* delete existing IO buffer and create a new one. */
            db_delete_io_buf(pt_io_buf);
            
            i4_rc = db_create_io_buf(aui4_byte_to_copy[0]+aui4_byte_to_copy[1]+8,
                                     &pt_io_buf);
        }
        else
        {
            /* clear the existing IO buffer so we can re-use it. */
            i4_rc = db_clear_io_buf(pt_io_buf);
        }
        
        if ( i4_rc == DBR_OK )
        {
            x_end_cnvt_to_big_32(aui4_byte_to_copy,2,aui4_big_end);

            pui1_buf=db_get_write_ptr(pt_io_buf, 2*sizeof(UINT32));
            if ( pui1_buf != NULL )
            {
                x_memcpy(pui1_buf, aui4_big_end, 2*sizeof(UINT32));

                /*
                   x_dbg_stmt("{CDB} write out object data: obj array sz: %d "
                              "memory buffer sz: %d\n",
                              aui4_byte_to_copy[0],
                              aui4_byte_to_copy[1]);
                */
                if ( aui4_byte_to_copy[0] > 0 )
                {
                    /****  memory buf: object location array  ******/
                    pui1_buf = db_get_write_ptr(pt_io_buf,aui4_byte_to_copy[0]);

                    if ( pui1_buf != NULL )
                    {
                        ui2_nb_of_obj = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));

                        /* write out the data, convert the internal
                           data array to Big endian.
                        */
                        x_end_cnvt_to_big_32(aui1_addr_vec[0],
                                             ui2_nb_of_obj,
                                             pui1_buf);

                        /****  memory buf: object data array  ******/
                        pui1_buf=db_get_write_ptr(pt_io_buf,aui4_byte_to_copy[1]);

                        if ( pui1_buf != NULL )
                        {
                            x_memcpy(pui1_buf, aui1_addr_vec[1],aui4_byte_to_copy[1]);
                            i4_rc = DBR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{CDB} Write Error:  no space in io_buffer.\n");
                            i4_rc = DBR_FILE_WRITE_FAIL ;
                        }
                    }
                    else
                    {
                        x_dbg_stmt("{CDB} Write Error:  no space in io_buffer.\n");
                        i4_rc = DBR_FILE_WRITE_FAIL ;
                    }
                }

                if ( i4_rc == DBR_OK )
                {
                    i4_rc = db_flush_io_buf_with_cmpres(pt_io_buf,
                                                        h_file,
                                                        pui4_crc,
                                                        b_cmpres,
                                                        h_zip,
                                                        &z_output_len,
                                                        b_write,
                                                        DB_CMPRES_CUSTOMER_DATA);
                    if ( i4_rc == DBR_OK )
                    {
                        z_cmp_data_len += z_output_len;
                    }    
#if 0
                    x_mem_buf_diagnostic(pt_do->pt_membuf);
                    x_dbg_stmt("{CDB} memory buffer saved to file.\n");
                    x_dbg_stmt("{CDB} ------------------------------\n");
#endif
                }
            }
            else
            {
                x_dbg_stmt("{CDB} Write Error:  no space in IO_buffer.\n");
                i4_rc = DBR_FILE_WRITE_FAIL ;
            }
        }
        else
        {
            x_dbg_stmt("{CDB} Write Error: can not create IO buf for writing out memory buffer.\n");
            i4_rc = DBR_FILE_WRITE_FAIL ;
        }
    }

    if ( i4_rc == DBR_OK )
    {
        *pz_data_len  = z_cmp_data_len ;
    }
    else
    {
        *pz_data_len = 0;
    }

    if ( b_cmpres )
    {
        DBG_INFO(("{CDB} Total size of CDB data section written: [%d bytes]\n", *pz_data_len));
    }

    /* save the compute the data section information.*/
    if ( i4_rc == DBR_OK )
    {
        pt_do->t_dsect_info.ui4_data_len = z_cmp_data_len ;
        pt_do->t_dsect_info.ui4_db_version = pt_do->ui4_db_version;
        pt_do->t_dsect_info.b_cmpres = b_cmpres;
        if ( b_cmpres == TRUE )
        {
            x_memset(pt_do->t_dsect_info.s_cmpres_eng, 0, CMPRES_ENG_NAME_LEN);
            x_strncpy(pt_do->t_dsect_info.s_cmpres_eng,
                      s_cmpres_eng,CMPRES_ENG_NAME_LEN-1);
        }
        else
        {
            x_memset(pt_do->t_dsect_info.s_cmpres_eng, 0, CMPRES_ENG_NAME_LEN);
        }
        pt_do->t_dsect_info.b_valid_sect_sz = TRUE;
    }
        
    /* Don't forget to delete IO buffer. */
    db_delete_io_buf(pt_io_buf);
        
    db_close_cmpres_eng(h_zip);
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_rec_from_io_buf
 *
 * Description: API to load CDB record data section from io buffer.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_load_rec_from_io_buf
(
    CDB_DATA_OBJ_T*       pt_do,
    DB_IO_BUF*            pt_io_buf,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv
)
{
    INT32             i4_rc;
    INT32             i4_rec_per_io_buf;
    UINT32            ui4_i;
    UINT8*            pui1_buf;
    
    VOID**            apv_recfld_addrs=NULL;
    VOID*             apv_buf[16];
    UINT16            ui2_rec_width;
    UINT8*            pui1_tmp = NULL;
    UINT8             aui1_tmp[128];

    i4_rc = DBR_OK;

    ui2_rec_width = pt_do->ui2_record_width;

    /*
       Allocate a working buffer space to store a single record .
       entry.  This working buffer will be used to store the 
       converted output (e.g., Endian conversion) from the record
       read from file. 
    */
    if ( ui2_rec_width <= 128 )
    {
        /*
           If the record width is less than 128 bytes,
           then use the memory on the stack to avoid dynamic
           memory allocation.
        */
        pui1_tmp=aui1_tmp;
    }
    else
    {
        /*
           Allocate space for both tmp and work buffer.
        */
        pui1_tmp=(UINT8*)x_mem_alloc(ui2_rec_width);
        if ( pui1_tmp == NULL )
        {
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
        }
    }
        
    /*
       allocate space for the array of pointers to the fields.
       If number of fields is 16 or less, use the memory on
       the stack
    */
    if ( ui1_count <= 16 )
    {
        apv_recfld_addrs=apv_buf;
    }
    else
    {
        /*
           dynamic allocation.
        */
        apv_recfld_addrs = (VOID**)x_mem_alloc(ui1_count*sizeof(VOID*));
        if ( apv_recfld_addrs == NULL )
        {
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
        }
    }

    if ( (pui1_tmp          !=   NULL)   &&
         (apv_recfld_addrs  !=   NULL)
        )
    {
         /*
            Determine the how many records are in IO buf.
          */
         i4_rec_per_io_buf = ((INT32)
                              ((db_get_io_buf_data_len(pt_io_buf))/pt_meta->ui2_rec_width));
 
         DBG_INFO(("{CDB} Number of CDB records in this IO buf.\n", i4_rec_per_io_buf));
         
         for ( ui4_i=0; ui4_i < ((UINT32)i4_rec_per_io_buf) ; ui4_i++ )
         {
             UINT32       ui4_byte_avail;
             
             pui1_buf = db_get_read_ptr(pt_io_buf,
                                        pt_meta->ui2_rec_width,
                                        &(ui4_byte_avail));
             if ( (pui1_buf != NULL )    &&
                  (ui4_byte_avail == pt_meta->ui2_rec_width)
                 )
             {
                 /*
                    Zero out the tmp buffer.
                 */
                 x_memset(pui1_tmp,0,pt_do->ui2_record_width);
                 i4_rc = db_add_rec_to_db(pui1_buf,
                                          pt_meta,
                                          h_cdb,
                                          pt_do,
                                          ui1_count,
                                          at_field_info,
                                          ui1_key_field_count,
                                          aui4_key_field_types,
                                          pf_conv,
                                          apv_recfld_addrs,
                                          pui1_tmp);
                 if ( i4_rc != DBR_OK )
                 {
                     break;
                 }
             }
             else
             {
                 i4_rc = DBR_FILE_READ_FAIL;
                 break;
             }
         }
    }
    else
    {
        i4_rc = DBR_OUT_OF_MEMORY;
    }
    
    /*
      Done reading records. clear IO buffer.
    */
    db_clear_io_buf(pt_io_buf);

    /*
       Don't forget to de-allocate the array of pointers to the fields.
    */
    if ( ui1_count > 16 && apv_recfld_addrs != NULL )
    {
        x_mem_free(apv_recfld_addrs);
    }
    if ( ui2_rec_width > 128 && pui1_tmp != NULL )
    {
        x_mem_free(pui1_tmp);
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_strbuf_from_io_buf
 *
 * Description: API to load CDB record data section from io buffer.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_load_strbuf_from_io_buf
(
    CDB_DATA_OBJ_T*       pt_do,
    DB_IO_BUF*            pt_io_buf,
    UINT8                 ui1_file_ver_read
)
{
    INT32             i4_rc;
    UINT8*            pui1_buf;
    UINT16            ui2_count;
    
    UINT32            aui4_byte_to_copy[2];
    UINT16            ui2_nb_of_str;
    UINT32            ui4_byte_avail;
    
    i4_rc = DBR_OK;    

    /*
       The string object buffer is stored as
       4 byte size, 4 bytes, n bytes of object string info,
       and n bytes of data.
    */
    pui1_buf = db_get_read_ptr(pt_io_buf,
                               2*sizeof(UINT32),
                               &(ui4_byte_avail));
    
    if ( (pui1_buf != NULL)                           &&
         (ui4_byte_avail == (2*sizeof(UINT32)))
        )
    {
        x_end_cnvt_from_big_32(pui1_buf,2,aui4_byte_to_copy);
        
        DBG_INFO(("{CDB} Read string data: obj array sz: %d string buffer sz: %d\n",
                   aui4_byte_to_copy[0],aui4_byte_to_copy[1]));

        if ( aui4_byte_to_copy[0] > 0 )
        {
            if (ui1_file_ver_read >= 6)
            {
                ui2_nb_of_str = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));
            }
            else
            {
                ui2_nb_of_str = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT16));
            }

            
            /* Free the existing string buffer and create a new
               string buffer with the size read from file.
            */
            x_str_buf_free(pt_do->pt_strbuf);

            if (ui1_file_ver_read >= 7)
            {
                x_str_buf_create(ui2_nb_of_str,
                                 aui4_byte_to_copy[1],
                                 &(pt_do->pt_strbuf));
            }
            else
            {
                /* old version uses 1-byte length. now we use 2 bytes */
                x_str_buf_create(ui2_nb_of_str,
                                 aui4_byte_to_copy[1] + ui2_nb_of_str,
                                 &(pt_do->pt_strbuf));
            }

            pui1_buf = db_get_read_ptr(pt_io_buf,
                                       aui4_byte_to_copy[0],
                                       &(ui4_byte_avail));
            if ( (pui1_buf != NULL)   &&
                 (pt_do->pt_strbuf != NULL ) &&
                 (((UINT32)ui4_byte_avail) == aui4_byte_to_copy[0])
                )
            {
                x_memcpy((UINT8*)pt_do->pt_strbuf->pui4_ary,
                         pui1_buf,
                         aui4_byte_to_copy[0]);
                
                /*
                   convert the data read into cpu memory order.
                */
                if (ui1_file_ver_read >= 6)
                {
                    x_end_cnvt_from_big_32(pt_do->pt_strbuf->pui4_ary,
                                           ui2_nb_of_str,
                                           pt_do->pt_strbuf->pui4_ary);
                }
                else
                {
                    UINT8*      pui1_tmp_buf;
                    pui1_tmp_buf = (UINT8*) x_mem_alloc(ui2_nb_of_str *
                                                        sizeof(UINT16));
                    if (pui1_tmp_buf == NULL)
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_FILE_READ_FAIL);
                    }

                    x_end_cnvt_from_big_16(pt_do->pt_strbuf->pui4_ary,
                                           ui2_nb_of_str,
                                           pui1_tmp_buf);

                    for (ui2_count= 0; ui2_count< ui2_nb_of_str;ui2_count++)
                    {
                        pt_do->pt_strbuf->pui4_ary[ui2_count] = *((UINT16*)(
                                        pui1_tmp_buf + ui2_count* sizeof(UINT16)));
                        if (pt_do->pt_strbuf->pui4_ary[ui2_count] == 0xFFFF)
                        {
                            pt_do->pt_strbuf->pui4_ary[ui2_count] = 0xFFFFFFFF;
                        }
                    }

                    x_mem_free(pui1_tmp_buf);
                }

                pui1_buf = db_get_read_ptr(pt_io_buf,
                                           aui4_byte_to_copy[1],
                                           &(ui4_byte_avail));

                if ( (pui1_buf != NULL)                         &&
                     (((UINT32)ui4_byte_avail) == aui4_byte_to_copy[1])
                    )
                {
                    x_memcpy(pt_do->pt_strbuf->pui1_str_buf,
                             pui1_buf,
                             aui4_byte_to_copy[1]);
                    
                    pt_do->pt_strbuf->ui2_nb_str = 0;

                    /* go through the input string offset array and find
                       out how many are valid string.
                    */
                    for ( ui2_count = 0 ;
                          ui2_count < ui2_nb_of_str;
                          ui2_count++ )
                    {
                        if ( pt_do->pt_strbuf->pui4_ary[ui2_count] != (UINT32)0xFFFFFFFF )
                        {
                            pt_do->pt_strbuf->ui2_nb_str++;
                        }
                    }

                    if (ui1_file_ver_read >= 7)
                    {
                        pt_do->pt_strbuf->ui4_mem_used = aui4_byte_to_copy[1];
                    }
                    else
                    {
                        _mem_buf_convert_old(
                                pt_do->pt_strbuf->ui4_str_buf_sz,
                                ui2_nb_of_str,
                                pt_do->pt_strbuf->pui4_ary,
                                pt_do->pt_strbuf->pui1_str_buf,
                                aui4_byte_to_copy[1] + pt_do->pt_strbuf->ui2_nb_str,
                                &(pt_do->pt_strbuf->ui4_mem_used)
                                );
                    }

                    DBG_INFO(("{CDB} String data loaded from file: nb of string: [%d] memory space used: [%d bytes]\n",
                              pt_do->pt_strbuf->ui2_nb_str, pt_do->pt_strbuf->ui4_mem_used));
#if 0
                    x_str_buf_diagnostic(pt_do->pt_strbuf);
                    x_dbg_stmt("{CDB} string buffer loaded from file.\n");
                    x_dbg_stmt("{CDB} ------------------------------\n");
#endif
                    i4_rc = DBR_OK;
                }
                else
                {
                    i4_rc = DBR_FILE_READ_FAIL ;
                }
            }
            else
            {
                i4_rc = DBR_FILE_READ_FAIL ;
            }
        }
        else
        {
            /* No string data, just return ok. */
            i4_rc = DBR_OK;
        }
    }
    else
    {
        i4_rc = DBR_FILE_READ_FAIL;
    }
    
    /*
      Done reading records. clear IO buffer.
    */
    db_clear_io_buf(pt_io_buf);

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_custm_data_from_io_buf
 *
 * Description: API to load customer data section from io buffer.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_load_custm_data_from_io_buf
(
    CDB_DATA_OBJ_T*       pt_do,
    DB_IO_BUF*            pt_io_buf,
    UINT8                 ui1_file_ver_read
)
{
    INT32             i4_rc = DBR_FAIL;
    UINT8*            pui1_buf;
    UINT16            ui2_count;
    UINT32            aui4_byte_to_copy[2];
    UINT16            ui2_nb_of_obj;
    UINT32            ui4_byte_avail;
    
    /*
       For version 4 and greater CDB file format,
       load the memory buffer from file.  
    */
    if ( ui1_file_ver_read >= 4 )
    {  
        /*
           CDB records and string buffer loaded. Now load the memory
           buffer.

           The object buffer is stored as 4byte size, 4 bytes,
           n bytes of object string info, and n bytes of data.
        */
        pui1_buf = db_get_read_ptr(pt_io_buf,
                                   2*sizeof(UINT32),
                                   &(ui4_byte_avail));

        if ( (pui1_buf != NULL)                           &&
             (ui4_byte_avail == (2*sizeof(UINT32)))
            )
        {
            x_end_cnvt_from_big_32(pui1_buf,2,aui4_byte_to_copy);
            
            DBG_INFO(("{CDB} Read memory buffer data: obj array sz: %d memory buffer sz: %d\n",
                     aui4_byte_to_copy[0], aui4_byte_to_copy[1]));

            if ( aui4_byte_to_copy[0] > 0 )
            {
                ui2_nb_of_obj = (UINT16)(aui4_byte_to_copy[0]/sizeof(UINT32));

                /* free the existing string buffer and create a new
                   string buffer with the size read from file.
                */
                x_mem_buf_free(pt_do->pt_membuf);

                if (ui1_file_ver_read >= 7)
                {
                    i4_rc = x_mem_buf_create(ui2_nb_of_obj,
                                     aui4_byte_to_copy[1],
                                     &(pt_do->pt_membuf));
                }
                else
                {
                    /* old version uses 1-byte length. now we use 2 bytes */
                    i4_rc = x_mem_buf_create(ui2_nb_of_obj,
                                     aui4_byte_to_copy[1] + ui2_nb_of_obj,
                                     &(pt_do->pt_membuf));
                }
                if ( i4_rc != MEMBUFR_OK )
                {
                    x_dbg_stmt("{CDB} Read Error: can not create memory buffer object.\n");
                    return DBR_FILE_READ_FAIL ;
                }
                i4_rc = DBR_FAIL; /* revert value */

                pui1_buf = db_get_read_ptr(pt_io_buf, aui4_byte_to_copy[0], &ui4_byte_avail);

                if ( (pui1_buf != NULL)       &&
                     (((UINT32)ui4_byte_avail) == aui4_byte_to_copy[0])
                    )
                {               
                    x_memcpy(pt_do->pt_membuf->pui4_ary, pui1_buf, aui4_byte_to_copy[0]);
                
                    /*
                      convert the data read into cpu memory order.
                    */
                    x_end_cnvt_from_big_32(pt_do->pt_membuf->pui4_ary,
                                           ui2_nb_of_obj,
                                           pt_do->pt_membuf->pui4_ary);

                    pui1_buf = db_get_read_ptr(pt_io_buf, aui4_byte_to_copy[1], &ui4_byte_avail);

                    if ( (pui1_buf != NULL)       &&
                         (((UINT32)ui4_byte_avail) == aui4_byte_to_copy[1])
                        )
                    {                       
                        x_memcpy(pt_do->pt_membuf->pui1_mem_buf, pui1_buf, aui4_byte_to_copy[1]);

                        pt_do->pt_membuf->ui2_nb_obj = 0;

                        /* go through the object offset array and find
                           out how many are valid object (e.g., address offset of
                           0xFFFFFFFF.)
                        */
                        for ( ui2_count = 0 ;
                              ui2_count < ui2_nb_of_obj;
                              ui2_count++ )
                        {
                            if ( pt_do->pt_membuf->pui4_ary[ui2_count] != ((UINT32)0xFFFFFFFF) )
                            {
                                pt_do->pt_membuf->ui2_nb_obj++;
                            }
                        }

                        
                        if (ui1_file_ver_read >= 7)
                        {
                            pt_do->pt_membuf->ui4_mem_used = aui4_byte_to_copy[1];
                        }
                        else
                        {
                            _mem_buf_convert_old(
                                    pt_do->pt_membuf->ui4_obj_buf_sz,
                                    ui2_nb_of_obj,
                                    pt_do->pt_membuf->pui4_ary,
                                    pt_do->pt_membuf->pui1_mem_buf,
                                    aui4_byte_to_copy[1] + pt_do->pt_membuf->ui2_nb_obj,
                                    &(pt_do->pt_membuf->ui4_mem_used)
                                    );
                        }

                        DBG_INFO(("{CDB} Memory buffer loaded from file: nb of objects: [%d] memory space used: [%d bytes]\n",
                              pt_do->pt_membuf->ui2_nb_obj, pt_do->pt_membuf->ui4_mem_used));
                            
#if 0
                        x_mem_buf_diagnostic(pt_do->pt_membuf);
                        x_dbg_stmt("{CDB} memory buffer loaded from file.\n");
                        x_dbg_stmt("{CDB} ------------------------------\n");
#endif
                        i4_rc = DBR_OK;
                    }
                    else
                    {
                        i4_rc = DBR_FILE_READ_FAIL ;
                    }
                }
                else
                {
                    i4_rc = DBR_FILE_READ_FAIL ;
                }
            }
            else
            {
                /* No customer data, just return ok. */
                i4_rc = DBR_OK;
            }
        }
        else
        {
            i4_rc = DBR_FILE_READ_FAIL ;
        }
    }            
    /*
      Done reading records. clear IO buffer.
    */
    db_clear_io_buf(pt_io_buf);

    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_data_sect_from_file
 *
 * Description: API to load compressed CDB record data section from file.
 *             
 * Inputs:   
 *    pui1_buf: Pointer to buffer containing meta data..
 *    ui2_meta_data_sz:   Size of metadata
 *    pf_cdata_conv:   Endian conversion function for common data.
 *    
 * Outputs: - None.
 *    pt_meta:  Structure containing decoded meta data field
 *
 * Returns:  
 *    DBR_OK:   records are loaded.
 *   
 *-----------------------------------------------------------------------*/
static INT32 db_load_data_sect_from_file
(
    HANDLE_T              h_file,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    UINT32*               pui4_crc
)
{
    INT32             i4_rc;
    DB_IO_BUF*        pt_io_buf         = NULL;
    DB_IO_BUF*        pt_io_uncmprs_buf = NULL;
    DB_IO_BUF*        pt_io_data        = NULL;
    UINT8*            pui1_buf;
    UINT32            ui4_byte_read;
    UINT32            ui4_val;
    
    CDB_DATA_OBJ_T*   pt_do; /* pointer to structure of a database object. */
    CDB_CLIENT_T*     pt_co; /* pointer to the client object. */
    HANDLE_TYPE_T     pe_type;

    UINT32            ui4_data_len;
    HANDLE_T          h_zip = NULL_HANDLE;
    CHAR              s_cmpres_eng[CMPRES_ENG_NAME_LEN];
    BOOL              b_input_data_cmpresd = FALSE;
    
    i4_rc = DBR_OK;

    /*
      Get the width of the record in CPU memory.
    */
    if ( handle_get_type_obj(h_cdb, &pe_type, ((VOID**)&pt_co)) == HR_OK )
    {
#ifdef DEBUG
        if (pe_type != CDBT_COREDB)
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        pt_do=pt_co->p_data_obj;
    }
    else
    {
        return DBR_FILE_READ_FAIL;
    }

    /* The data (compression engine name, CDB record, string buffer,
       customer data buffer) length.
    */
    ui4_data_len = pt_meta->ui4_data_sz;

    DBG_INFO(("{CDB} Processing input CDB data section-> length: [%d bytes]\n", ui4_data_len));
    
    i4_rc = DBR_FILE_READ_FAIL;

    /* Read the name of compression engine. */
    x_memset(s_cmpres_eng,0,CMPRES_ENG_NAME_LEN);
    if ( (x_fm_read(h_file, s_cmpres_eng, CMPRES_ENG_NAME_LEN, &ui4_byte_read) == FMR_OK) &&
         (ui4_byte_read == CMPRES_ENG_NAME_LEN)
        )
    {
        
        ui4_data_len -= ui4_byte_read ;
        
        /* If the 'compression engin name' consists of a NULL string,
           then no compression was used on the input data, so we
           by-pass the initializing the compression engine.
        */
        if ( x_strlen(s_cmpres_eng) == 0 )
        {
            b_input_data_cmpresd = FALSE;
            i4_rc = DBR_OK ;
        }
        else
        {
            /* open a handle to the compression engine. */
            if ( db_open_cmpres_eng(s_cmpres_eng, &h_zip) == DBR_OK )
            {
                b_input_data_cmpresd = TRUE;
                i4_rc = DBR_OK;
            }
            else
            {
                /* A 'compression engine name' is specified, but we can not
                   open a compression handle to that engine.  This is an error,
                   because the input data is compressed, but we don't have a 
                   way to un-compress it.
                */
                return DBR_FILE_READ_FAIL;
            }
        }
    }
    else
    {
        return DBR_FILE_READ_FAIL;
    }
    
#if 0
    if ( pt_meta->ui2_rec_count == 0 )
    {
        /* If no record stored in the file system, just return ok. */
        return i4_rc;
    }
#endif
    
    while ( (pt_meta->ui2_rec_count > 0)     &&
            (ui4_data_len > 0)               &&
            (i4_rc == DBR_OK) )
    {
        SIZE_T            z_uncmp_data_len;
        SIZE_T            z_cmp_data_len;
        UINT8             cmp_meta_data[9];
        BOOL              b_cmpres;
        UINT32            ui4_iobuf_sz;
        UINT32            ui4_uncmprs_iobuf_sz;
        
        /*
          Read the meta information about the compressed data section. The metadata are:
          cmpress flag (1 byte), cmp_data_Len (4 bytes), uncomp_data_len (4 bytes)
        */
        if ( (x_fm_read(h_file, cmp_meta_data, sizeof(cmp_meta_data), &ui4_byte_read) == FMR_OK) &&
             (sizeof(cmp_meta_data) == ui4_byte_read)
            )
        {
            ui4_data_len -= ui4_byte_read ;
            
            if ( cmp_meta_data[0] & DB_CMPRES_MASK )
            {
                b_cmpres = TRUE;
                if ( b_cmpres != b_input_data_cmpresd )
                {
                    x_dbg_stmt("{CDB} Read Error: input data is in compressed form but no compression engine specified.\n");
                    break;
                }
            }
            else
            {
                b_cmpres = FALSE;
            }

            x_end_cnvt_from_big_32(&(cmp_meta_data[1]), 1, &z_cmp_data_len);
            x_end_cnvt_from_big_32(&(cmp_meta_data[5]), 1, &z_uncmp_data_len);

            DBG_INFO(("{CDB} Input data section size [%d bytes] input sect uncmpres size [%d bytes]\n",
                      z_cmp_data_len, z_uncmp_data_len));
            
        }
        else
        {
            break;
        }

        /* round up to next 4 bytes */
        ui4_iobuf_sz = (((z_cmp_data_len+3)>>2)<<2);

        /* allocate IO buffer to read the compressed data and uncompress it. */
        if ( pt_io_buf != NULL )
        {
            db_clear_io_buf(pt_io_buf);
            if ( db_get_io_buf_capacity(pt_io_buf) < ((INT32)ui4_iobuf_sz) )
            {
                db_delete_io_buf(pt_io_buf);
                pt_io_buf = NULL;
                if ( db_create_io_buf(ui4_iobuf_sz, &pt_io_buf) != DBR_OK )
                {
                    break;
                }
            }
        }
        else
        {
            /* create IO buf */
            if ( db_create_io_buf(ui4_iobuf_sz, &pt_io_buf) != DBR_OK )
            {
                break;
            }
        }

        /* round up to the next 4 bytes.*/
        ui4_uncmprs_iobuf_sz = (((z_uncmp_data_len+3)>>2)<<2);
        
        /* allocate IO buffer to read the compressed data and uncompress it. */
        if ( pt_io_uncmprs_buf != NULL )
        {
            db_clear_io_buf(pt_io_uncmprs_buf);
            if ( (UINT32)(db_get_io_buf_capacity(pt_io_uncmprs_buf)) < ui4_uncmprs_iobuf_sz )
            {
                db_delete_io_buf(pt_io_uncmprs_buf);
                pt_io_uncmprs_buf = NULL;
                if ( db_create_io_buf(ui4_uncmprs_iobuf_sz, &pt_io_uncmprs_buf) != DBR_OK )
                {
                    break;
                }
            }
        }
        else
        {
            /* create IO buf for storing un-compressed data */
            if ( db_create_io_buf(ui4_uncmprs_iobuf_sz, &pt_io_uncmprs_buf) != DBR_OK )
            {
                break;
            }
        }
        
        /* Read the data. */
        /*
           Get the write pointer to the IO buffer.
        */
        pui1_buf=db_get_write_ptr(pt_io_buf, z_cmp_data_len);
        if ( pui1_buf != NULL )
        {
            if ( (x_fm_read(h_file, pui1_buf, z_cmp_data_len, &ui4_byte_read ) == FMR_OK)  &&
                 (ui4_byte_read == z_cmp_data_len )
                )
            {
                ui4_data_len -= ui4_byte_read ;
                /*
                  compute the CRC32 value for the data section that read.
                */
                if ( pui4_crc != NULL )
                {                    
                    ui4_val = x_crc32_gen(pui1_buf, ui4_byte_read, (*pui4_crc));
                    *pui4_crc = ui4_val ;
                }

                /* uncompress if needed. */
                if ( b_cmpres == TRUE )
                {
                    if ( db_uncmpres_data(h_zip,
                                          pt_io_buf,
                                          pt_io_uncmprs_buf,
                                          z_uncmp_data_len)
                         == DBR_OK
                        )
                    {
                        pt_io_data = pt_io_uncmprs_buf;
                        DBG_INFO(("{CDB} Success: data section uncompresssed.\n"));                   
                    }
                    else
                    {
                        x_dbg_stmt("{CDB} Read Error: can not un-compress input data section.\n");
                        break;   
                    } 
                }
                else
                {
                    pt_io_data = pt_io_buf;
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
                
        /* process the data section based on its types. */
        if ( cmp_meta_data[0] & DB_CMPRES_REC_DATA )
        {
      
            DBG_INFO(("{CDB} process CDB record section.\n"));
       
            /* process CDB record data. */            
            i4_rc = db_load_rec_from_io_buf(pt_do,
                                            pt_io_data,
                                            pt_meta,
                                            ui1_file_ver_read,
                                            h_cdb,
                                            ui1_count,
                                            at_field_info,
                                            ui1_key_field_count,
                                            aui4_key_field_types,
                                            pf_conv);
        }
        
        
        if ( cmp_meta_data[0] & DB_CMPRES_STRING_DATA )
        {
            DBG_INFO(("{CDB} process string buffer data.\n"));

            /*
              Load the string buffer from file, only if the newly
              create DB object contain string field.
            */
            if ( pt_do->pt_strbuf != NULL )
            {
                i4_rc = db_load_strbuf_from_io_buf(pt_do,
                                                   pt_io_data,
                                                   ui1_file_ver_read);
            }
            else
            {
                i4_rc = DBR_OK;
            }
        }

        if ( cmp_meta_data[0] & DB_CMPRES_CUSTOMER_DATA )
        {
            DBG_INFO(("{CDB} process memory buffer data.\n"));

            /*
              Load the string buffer from file, only if the newly
              create DB object contain customer private data field.
            */
            if ( pt_do->pt_membuf != NULL )
            {
                i4_rc = db_load_custm_data_from_io_buf(pt_do,
                                                       pt_io_data,
                                                       ui1_file_ver_read);
            }
            else
            {
                i4_rc = DBR_OK;
            }
        }
    }

    db_delete_io_buf(pt_io_buf);
    db_delete_io_buf(pt_io_uncmprs_buf);
    db_close_cmpres_eng(h_zip);
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_load_data_obj_from_file
 *
 * Description: API to create a new CDB object from the data (file format, meta data, 
 *              and records) stored on a file.  It returns a CDB handle to the
 *              newly created database object.
 *
 *              This function in term calls one of the two function which actually
 *              load the CDB object from file.  The newer version support
 *              compression, where as older version does not (it is kept for backward
 *              compatibility).
 * Inputs:   
 *    pt_do:    Database object.
 *    h_file:   Handle to the file object to write to.
 *
 * Outputs: -
 *    ph_cdb:   Handle to the CDB object created
 *
 * Returns: - 
 *    DBR_FILE_READ_FAIL
 *-----------------------------------------------------------------------*/
static INT32  db_load_data_obj_from_file
(
    HANDLE_T              h_file,
    DB_FILE_META_DATA_T*  pt_meta,
    UINT8                 ui1_file_ver_read,
    HANDLE_T              h_cdb,
    UINT8                 ui1_count,
    DB_FIELD_INFO_T       at_field_info[],
    UINT8			      ui1_key_field_count,
    DB_FIELD_T            aui4_key_field_types[],
    db_endian_conv_fct    pf_conv,
    UINT32*               pui4_crc
)
{
    INT32                 i4_rc ;
    
    if ( ui1_file_ver_read > 4 )
    {
        i4_rc = db_load_data_sect_from_file(h_file,
                                            pt_meta,
                                            ui1_file_ver_read,
                                            h_cdb,
                                            ui1_count,
                                            at_field_info,
                                            ui1_key_field_count,
                                            aui4_key_field_types,
                                            pf_conv,
                                            pui4_crc);
    }

    if ( ui1_file_ver_read <= 4 )
    {
        i4_rc = db_load_rec_from_file(h_file,
                                      pt_meta,
                                      ui1_file_ver_read,
                                      h_cdb,
                                      ui1_count,
                                      at_field_info,
                                      ui1_key_field_count,
                                      aui4_key_field_types,
                                      pf_conv,
                                      pui4_crc);
    }

    return i4_rc;
}

        
