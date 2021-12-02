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
 * $RCSfile: cdb_io_buf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation of file buffer I/O functions.
 *         
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cdb/_cdb.h"
#include "cdb/cdb_cmpres.h"
#include "cdb/cdb_io_buf.h"
#include "file_mngr/x_fm.h"
#include "util/x_crc32.h"
#include "util/x_end_cnvt.h"
#include "cdb/cdb_dbg.h"
/*-----------------------------------------------------------------------------
 * Name: db_create_io_buf
 *
 * Description: API to create a memory buffer for data record I/O.
 *              The CDB library will write records into this buffer 
 *              until a high-water is reached, and then the content
 *              of this buffer will be written to the filesystem.  
 *
 * Inputs:  - 
 *   ui4_io_buf_size:  size (in bytes) of the io buffer.
 *
 * Outputs: -
 *   ppt_io_buf:       pointer to the io_buf object created.
 *
 * Returns: -    
 *   DBR_OK:           IO buffer is created.            
 *   DBR_OUT_OF_MEMORY IO buffer can not be create due to lack of memory. *            
 ----------------------------------------------------------------------------*/
INT32 db_create_io_buf
(
    UINT32            ui4_io_buf_size,
    DB_IO_BUF**       ppt_io_buf
)
{
    INT32             i4_rc;
    DB_IO_BUF*        pt_io_buf=NULL;
    

    DBG_INFO(("{CDB} create IO buf with size of [%d bytes]\n",ui4_io_buf_size));

    *ppt_io_buf = NULL ;
    
    pt_io_buf   = (DB_IO_BUF*)x_mem_alloc(sizeof(DB_IO_BUF));

    if ( pt_io_buf != NULL )
    {
        pt_io_buf->pui1_buf=NULL;
        
        pt_io_buf->pui1_buf=(UINT8*)x_mem_alloc(ui4_io_buf_size);

        if ( pt_io_buf->pui1_buf != NULL )
        {
            pt_io_buf->ui4_total_space = ui4_io_buf_size;
            pt_io_buf->ui4_write_ptr = 0;
            pt_io_buf->ui4_read_ptr  = 0;
            
            x_memset(pt_io_buf->pui1_buf,0,pt_io_buf->ui4_total_space);
            
            
            *ppt_io_buf = pt_io_buf;

            i4_rc = DBR_OK;
        }
        else
        {
            db_delete_io_buf(pt_io_buf);
            i4_rc = DBR_OUT_OF_MEMORY;
            x_dbg_stmt("{CDB} Error: can not created IO buf with size of [%d bytes]\n",
                       ui4_io_buf_size);
        }
    }
    else
    {
        i4_rc = DBR_OUT_OF_MEMORY;
        x_dbg_stmt("{CDB} Error: can not created IO buf structure.\n");
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_delete_io_buf
 *
 * Description: API to free the memory allocated for IO buffer. 
 *
 * Inputs:  - 
 *   pt_io_buf:       pointer to the io_buf object to be freed.
 *
 * Outputs: -
 *   None
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_delete_io_buf
(
    DB_IO_BUF*        pt_io_buf
)
{
    if ( pt_io_buf != NULL )
    {
        if ( pt_io_buf->pui1_buf != NULL )
        {
            x_mem_free(pt_io_buf->pui1_buf);
        }
        x_mem_free(pt_io_buf);
        
        DBG_INFO(("{CDB} free IO buffer.\n"));

    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_clear_io_buf
 *
 * Description: API to clear all data in existing IO buffer and reset
 *    the write (input) pointers.
 *
 * Inputs:  - 
 *   pt_io_buf:       pointer to the io_buf object to be freed.
 *
 * Outputs: -
 *   None
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_clear_io_buf
(
    DB_IO_BUF*        pt_io_buf
)
{
    if ( pt_io_buf != NULL )
    {
        pt_io_buf->ui4_write_ptr = 0;
        pt_io_buf->ui4_read_ptr  = 0;
        
        x_memset(pt_io_buf->pui1_buf,0,pt_io_buf->ui4_total_space);
        
        DBG_INFO(("{CDB} clear IO buffer.\n"));
      
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_io_buf_capacity
 *
 * Description: API to return the total memory space allocatd by the
 *   IO buffer.
 *
 * Inputs:  - 
 *   pt_io_buf:       pointer to the io_buf object to be freed.
 *
 * Outputs: -
 *   None
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_get_io_buf_capacity
(
    DB_IO_BUF*        pt_io_buf
)
{
    if ( pt_io_buf != NULL )
    {
        return ((INT32) pt_io_buf->ui4_total_space);
    }
    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: db_get_io_buf_data_len
 *
 * Description: API to get the length of data in the IO buffer.
 *
 * Inputs:  - 
 *   pt_io_buf:       pointer to the io_buf object to be freed.
 *
 * Outputs: -
 *   None
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_get_io_buf_data_len
(
    DB_IO_BUF*        pt_io_buf
)
{
    if ( pt_io_buf != NULL )
    {
        return ((INT32) pt_io_buf->ui4_write_ptr);
    }
    return 0;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_write_ptr
 *
 * Description: API to get the write pointer to copy the CDB record into
 *              the memory buffer that location. 
 *              After this call, the write pointer is advanced by the
 *              number of bytes requested.
 *
 * Inputs:  - 
 *   pt_io_buf:   pointer to the io buffer object.
 *   ui4_size_to_write_to:  The size of data to write into the IO buffer.
 *                          If the free space left in IO buffer is smaller
 *                          than the size needed, a NULL pointer is returned.
 *
 * Outputs: -
 *   
 *
 * Returns: -    
 *   NULL:     If the specified 'size_to_write_to' is bigger than
 *             the amount of free space in the io_buffer object.
 *   
 *            
 ----------------------------------------------------------------------------*/
UINT8* db_get_write_ptr
( 
    DB_IO_BUF*  pt_io_buf,
    UINT32      ui4_size_to_write_to
)
{
    INT32    i4_free_space;
    UINT8*   pui1_addr=NULL;

    if ( pt_io_buf != NULL )
    {
        i4_free_space = (INT32)(pt_io_buf->ui4_total_space - pt_io_buf->ui4_write_ptr) ;

        if ( i4_free_space >= ((INT32) ui4_size_to_write_to) )
        {
            pui1_addr = pt_io_buf->pui1_buf + pt_io_buf->ui4_write_ptr ;
            /* advance the write ptr by the 'ui4_size_to_write_to' amount. */
            pt_io_buf->ui4_write_ptr = pt_io_buf->ui4_write_ptr + ui4_size_to_write_to;
        }
    }
    return pui1_addr;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_read_ptr
 *
 * Description: API to get the read pointer of data in the
 *     IO buf.  After return, the read pointer is move ahead by
 *     the request amount.  If no data is availabe, return NULL.
 *
 * Inputs:  - 
 *   pt_io_buf:   pointer to the io buffer object.
 *   ui4_size_to_read:  The size of data to write into the IO buffer.
 *                          If the free space left in IO buffer is smaller
 *                          than the size needed, a NULL pointer is returned.
 * Outputs: -
 *   pui4_byte_avail:   The size of data available to read.
 *   
 *
 * Returns: -    
 *   NULL:     If the specified 'size_to_write_to' is bigger than
 *             the amount of free space in the io_buffer object.
 *   
 *            
 ----------------------------------------------------------------------------*/
UINT8* db_get_read_ptr
( 
    DB_IO_BUF*  pt_io_buf,
    UINT32      ui4_byte_to_read,
    UINT32*     pui4_byte_available
)
{
    UINT8*   pui1_addr=NULL;
    *pui4_byte_available = 0;
    
    if ( (pt_io_buf->ui4_read_ptr + ui4_byte_to_read) <= pt_io_buf->ui4_write_ptr )
    {
        pui1_addr = pt_io_buf->pui1_buf + pt_io_buf->ui4_read_ptr  ;

        *pui4_byte_available = ui4_byte_to_read;
        
        /* advance the read ptr by the amount read. */
        pt_io_buf->ui4_read_ptr += (*pui4_byte_available) ;
    }
    else
    {
        /* return what's left in the IO buf  */
        if ( pt_io_buf->ui4_write_ptr > pt_io_buf->ui4_read_ptr )
        {
            pui1_addr = pt_io_buf->pui1_buf + pt_io_buf->ui4_read_ptr  ;

            *pui4_byte_available = pt_io_buf->ui4_write_ptr - pt_io_buf->ui4_read_ptr ;

            pt_io_buf->ui4_read_ptr += (*pui4_byte_available) ;
        }
    }
    return pui1_addr;
}

/*-----------------------------------------------------------------------------
 * Name: db_move_write_ptr
 *
 * Description: API to move the write ptr by a specified number of bytes.
 *
 * Inputs:  - 
 *   pt_io_buf:   pointer to the io buffer object.
 *   i4_offset:   amount of bytes to move the write ptr
 *   ui1_pos:     Whence to move the wrt pointer.
 *  
 * Outputs: - None.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/

INT32 db_move_write_ptr
(
    DB_IO_BUF*        pt_io_buf,
    INT32             i4_offset,
    DB_IO_BUF_POS     ui1_pos
)
{
    INT32  i4_new_wrt_ptr;
    
    switch (ui1_pos)
    {
        case DB_IO_SET:
        {
            if ( i4_offset < 0 )
            {
                pt_io_buf->ui4_write_ptr = 0;
            }
            else if ( ((INT32)i4_offset) > ((INT32)(pt_io_buf->ui4_total_space)) )
            {
                pt_io_buf->ui4_write_ptr = pt_io_buf->ui4_total_space;
            }
            else
            {
                pt_io_buf->ui4_write_ptr = (UINT32) i4_offset;
            }
        }
        break;

        case DB_IO_CUR:
        {
            i4_new_wrt_ptr= (INT32)(pt_io_buf->ui4_write_ptr + i4_offset);
            if ( i4_new_wrt_ptr < 0 )
            {
                pt_io_buf->ui4_write_ptr = 0;
            }
            else if ( ((INT32)i4_new_wrt_ptr) > ((INT32)pt_io_buf->ui4_total_space) )
            {
                pt_io_buf->ui4_write_ptr = pt_io_buf->ui4_total_space;
            }
            else
            {
                pt_io_buf->ui4_write_ptr = (UINT32)i4_new_wrt_ptr;
            }
        }
        break;

        case DB_IO_END:
        {
            i4_new_wrt_ptr=(INT32)(pt_io_buf->ui4_total_space - i4_offset);
            if ( i4_new_wrt_ptr < 0 )
            {
                pt_io_buf->ui4_write_ptr = 0;
            }
            else if ( ((INT32)i4_new_wrt_ptr) > ((INT32)pt_io_buf->ui4_total_space) )
            {
                pt_io_buf->ui4_write_ptr = pt_io_buf->ui4_total_space;
            }
            else
            {
                pt_io_buf->ui4_write_ptr = (UINT32)i4_new_wrt_ptr;
            }
        }
        break;

        default:
            /*
              Unknow IO buffer position command.
            */
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
            break;
    }
    return DBR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: db_flush_io_buf
 *
 * Description: API to write to IO buffer to the specified file handle.
 *              The flush occurs if the high-water mark is reached.
 *
 * Inputs:  - 
 *    pt_io_buf:   Pointer to IO buffer structure.
 *    h_file:      Handle to the file object.
 *    pui4_crc:    Input CRC32 value.
 *
 * Outputs: -
 *    pui4_crc:     CRC value computed from the data section to be flush out.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_flush_io_buf
(
    DB_IO_BUF*        pt_io_buf,
    HANDLE_T          h_file,
    UINT32*           pui4_crc
)
{
    UINT32  ui4_val;
    INT32   i4_rc;
    
    i4_rc=DBR_OK;

    if ( pt_io_buf->ui4_write_ptr > 0 )
    {
        i4_rc=x_fm_write(h_file,
                          pt_io_buf->pui1_buf,
                          pt_io_buf->ui4_write_ptr,
                          &ui4_val);
        if ( i4_rc == FMR_OK )
        {
            /* Number of bytes written should equals number
               of bytes in the buffer.
            */
            if (  ((UINT32)pt_io_buf->ui4_write_ptr) == ui4_val )
            {
                if ( pui4_crc != NULL )
                {
                    /*
                      compute the CRC32 value for the data section that is
                      flushed out.
                    */
                    ui4_val = x_crc32_gen(pt_io_buf->pui1_buf,
                                          (SIZE_T)pt_io_buf->ui4_write_ptr,
                                          (*pui4_crc));
                    *pui4_crc = ui4_val ;
                }

                /* Reset the write buffer. */
                pt_io_buf->ui4_write_ptr=0;
                
                x_memset(pt_io_buf->pui1_buf,0,pt_io_buf->ui4_total_space);
                
            }
            else
            {
                i4_rc = DBR_FILE_WRITE_FAIL;
            }
        }
        else
        {
            i4_rc = DBR_FILE_WRITE_FAIL;
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: db_flush_io_buf_cmpres
 *
 * Description: API to write to IO buffer to the specified file handle.
 * 
 *   This function also support compression of data before writing to
 *   filesystem, and compute the length of compressed data to be written
 *   out.
 *
 * Inputs:  - 
 *    pt_io_buf:   Pointer to IO buffer structure.
 *    h_file:      Handle to the file object.
 *    pui4_crc:    Input CRC32 value.
 *    b_write:     FALSE: Only compute how much data will be written out, but
 *                 don't actually write the data.
 *                 TRUE:  Write out the data.
 *    ui1_payload_type:  Caller specified tag to indicate the
 *                       type of data that is flused out.
 * Outputs: -
 *    pui4_crc:     CRC value computed from the data section to be flush out.
 *
 * Returns: -    
 *               
 *            
 *            
 ----------------------------------------------------------------------------*/
INT32 db_flush_io_buf_with_cmpres
(
    DB_IO_BUF*        pt_io_buf,
    HANDLE_T          h_file,
    UINT32*           pui4_crc,
    BOOL              b_cmpres,
    HANDLE_T          h_zip,
    SIZE_T*           pz_byte_output,
    BOOL              b_write,
    UINT8             ui1_payload_type
)
{
    UINT32            ui4_val;
    INT32             i4_rc;
    SIZE_T            z_uncmp_data_len;
    SIZE_T            z_cmp_data_len;
    UINT8             cmp_meta_data[9];
    
    
    i4_rc=DBR_OK;

    *pz_byte_output = 0 ;
    z_uncmp_data_len = pt_io_buf->ui4_write_ptr;
    z_cmp_data_len   = pt_io_buf->ui4_write_ptr;  

    if ( b_write )
    {
        if ( b_cmpres )
        {
            i4_rc = db_cmpres_data(h_zip,
                                   pt_io_buf,
                                   &z_uncmp_data_len,
                                   &z_cmp_data_len);
            cmp_meta_data[0] = 0;
            cmp_meta_data[0] = (UINT8)(DB_CMPRES_MASK | ui1_payload_type) ;
            x_end_cnvt_to_big_32(&z_cmp_data_len, 1,&(cmp_meta_data[1]));
            x_end_cnvt_to_big_32(&z_uncmp_data_len, 1, &(cmp_meta_data[5]));
        }
        else
        {
            cmp_meta_data[0] = 0;
            cmp_meta_data[0] = ui1_payload_type ;
            x_end_cnvt_to_big_32(&z_cmp_data_len, 1,&(cmp_meta_data[1]));
            x_end_cnvt_to_big_32(&z_uncmp_data_len, 1, &(cmp_meta_data[5]));
        }
        
        i4_rc=x_fm_write(h_file,
                         cmp_meta_data,
                         sizeof(cmp_meta_data),
                         &ui4_val);

        if ( (i4_rc == FMR_OK)   &&
             (ui4_val == sizeof(cmp_meta_data))
            )
        {
            *pz_byte_output += ui4_val;

            i4_rc=x_fm_write(h_file,
                             pt_io_buf->pui1_buf,
                             pt_io_buf->ui4_write_ptr, 
                             &ui4_val);

            if ( i4_rc == FMR_OK )
            {
                /* Number of bytes written should equals number
                   of bytes in the buffer.
                */
                if (  ui4_val == pt_io_buf->ui4_write_ptr )
                {
                    DBG_API(("{CDB} number of data bytes wrote to file: [%d bytes]\n", ui4_val));

                    *pz_byte_output += ui4_val;

                    DBG_INFO(("{CDB} Total number of bytes wrote to file: [%d bytes]\n", *pz_byte_output));
                    
                    if ( pui4_crc != NULL )
                    {
                        /*
                          compute the CRC32 value for the data section that is
                          flushed out.
                        */
                        ui4_val = x_crc32_gen(pt_io_buf->pui1_buf,
                                              (SIZE_T)pt_io_buf->ui4_write_ptr,
                                              (*pui4_crc));
                        *pui4_crc = ui4_val ;
                    }

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
                    x_dbg_stmt("{CDB} Write Error: data buffer [%d bytes] don't match data written [%d bytes]\n",
                               pt_io_buf->ui4_write_ptr, ui4_val);
                    i4_rc = DBR_FILE_WRITE_FAIL;
                }
            }
            else
            {
             
                x_dbg_stmt("{CDB} Write Error: can not write data [%d bytes] to file.\n",
                           pt_io_buf->ui4_write_ptr);                          
                i4_rc = DBR_FILE_WRITE_FAIL;
            }
        }
        else
        {
            x_dbg_stmt("{CDB} Write Error: can not write header (payload type, un-compres len, compres len to file\n");
            i4_rc = DBR_FILE_WRITE_FAIL;
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
            if ( i4_rc == DBR_OK )
            {
                *pz_byte_output =
                    z_cmp_data_len + sizeof(cmp_meta_data);
            }
            else
            {
                *pz_byte_output =
                    z_uncmp_data_len + sizeof(cmp_meta_data);
                /* can not get compress data len, change the flag to FALSE. */
                b_cmpres = FALSE;
            }
        }
        else
        {
            *pz_byte_output =
                z_uncmp_data_len + sizeof(cmp_meta_data);
        }

        /* Note: even this is non-write mode, we still need to clear the IO
           buffer, because the IO buffer will be re-used after it return
           to the caller to fill in more records for size computation.
        */
        /* Reset the write buffer. */
        pt_io_buf->ui4_write_ptr=0;

        /* clear out the buffer, we need to do this because for the
           UNION field in the CDB record, not all bytes are set by
           the endian conversion function, so there could be some
           left over data in the record from previous processing.
         */
        x_memset(pt_io_buf->pui1_buf,0,pt_io_buf->ui4_total_space);
        
        DBG_INFO(("{CDB} no write, just compute the output data len [%d bytes]\n",*pz_byte_output));
    }
    
    return i4_rc;
}
