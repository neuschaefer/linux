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
 * $RCSfile: cdb_cmpres.c,v $
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
#include "cdb/cdb_io_buf.h"
#include "file_mngr/x_fm.h"
#include "util/x_crc32.h"
#include "cdb/cdb_cmpres.h"

/*-----------------------------------------------------------------------------
 * Name: db_close_cmpres_eng
 *
 * Description: Close a handle compression engine.
 *
 * Inputs:  -
 *   s_cmpres_eng  Specify the name of the compression engine to
 *                 uncompress the data.
 *
 * Outputs: -
 *   ph_zip        Handle to the compressiong/decompression engine.
 *
 * Returns: -    
 *                           
 *            
 ----------------------------------------------------------------------------*/
INT32 db_close_cmpres_eng
(
    HANDLE_T          h_zip
)
{
    INT32             i4_rc = DBR_OK;

    if ( h_zip != NULL_HANDLE )
    {
        if ( x_cl_close(h_zip) != CLR_OK )
        {
            x_dbg_stmt("{CDB} Error: can not close compression engine handle [%d].\n",h_zip);
            i4_rc = DBR_FAIL;
        }
        else
        {
            DBG_INFO(("{CDB} compression engine handle [%d] closed.\n", h_zip));
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_open_cmpres_eng
 *
 * Description: Open a handle de-compression on the input data.
 *
 * Inputs:  -
 *   s_cmpres_eng  Specify the name of the compression engine to
 *                 uncompress the data.
 *
 * Outputs: -
 *   ph_zip        Handle to the compressiong/decompression engine.
 *
 * Returns: -    
 *                           
 *            
 ----------------------------------------------------------------------------*/
INT32 db_open_cmpres_eng
(
    const CHAR*       s_cmpres_eng,
    HANDLE_T*         ph_zip
)
{
    INT32             i4_rc    = DBR_FAIL;
    
    if ( ph_zip != NULL )
    {
        if ( x_cl_open_eng(s_cmpres_eng, NULL, NULL,
                           ph_zip)
             == CLR_OK
            )
        {
            DBG_INFO(("{CDB} De-compression engine [%s] opened [%d].\n",
                      s_cmpres_eng, *ph_zip));
            i4_rc = DBR_OK;
        }
        else
        {
            x_dbg_stmt("{CDB} De-compression engine [%s] not available\n",
                       s_cmpres_eng);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_uncmpres_data
 *
 * Description: Perform de-compression on the input data.
 *
 * Inputs:  -
 *   h_zip:        Handle to the compression engine.
 *   pt_io_buf:    Pointer to IO buffer structure that
 *                 contains compressed data on input.
 *   z_uncmp_data_len:   Data lenght of the uncompressed data.
 *
 * Outputs: -
 *   pt_io_uncmprs_buf:   Pointer to IO buffer structure that
 *                        contains de-compressed data on output.
 *
 *   pz_uncmp_data_len:   Length of data.
 *
 * Returns: -    
 *                           
 *            
 ----------------------------------------------------------------------------*/
INT32 db_uncmpres_data
(
    HANDLE_T          h_zip,
    DB_IO_BUF*        pt_io_buf,
    DB_IO_BUF*        pt_io_uncmprs_buf,
    SIZE_T            z_uncmp_data_len
)
{
    INT32             i4_rc    = DBR_OK;
    UINT8*            pui1_buf = NULL;
    SIZE_T            z_buf_len = 0 ;
    
    if ( h_zip == NULL_HANDLE )
    {
        return  DBR_FAIL;
    }
    
    /*
       Get the write pointer to the output buffer for
       writing out the uncompress data.
    */
    pui1_buf=db_get_write_ptr(pt_io_uncmprs_buf, (UINT32)z_uncmp_data_len);

    /* IO buffer's ui4_write_ptr contains length of compressed data. */
    
    if ( (pt_io_buf->ui4_write_ptr > 0)         &&
         (pui1_buf != NULL)
        )
    {
        z_buf_len = z_uncmp_data_len;
        /* De-compressed data. */
        if ( x_cl_decompress(h_zip,
                             pt_io_buf->pui1_buf,
                             pt_io_buf->ui4_write_ptr,
                             0,
                             CL_NULL_PARAM_TYPE,
                             NULL,
                             pui1_buf,
                             &z_buf_len,
                             NULL)
             == CLR_OK
            )
        {
            DBG_INFO(("{CDB} uncompressed data len: [%d bytes] compressed data len: [%d bytes]\n",
                      z_buf_len, pt_io_buf->ui4_write_ptr));

            if ( z_buf_len != z_uncmp_data_len )
            {
                /* Un-compressed data has a different length
                   than what is expected.
                */
                i4_rc = DBR_FAIL;
            }
            else
            {
                /* Success.
                   Clear the input buffer so that it can be resued.
                   for the next uncompress operation.

                   Reset the write buffer to the beginning.
                */
                pt_io_buf->ui4_write_ptr=0;
                i4_rc = DBR_OK;
            }
        }
        else
        {
            x_dbg_stmt("{CDB} Error: can not de-compress input data.\n");
            i4_rc = DBR_FAIL;
        }
    }
    else
    {
        x_dbg_stmt("{CDB} Error: can not de-compress input data.\n");
        
        i4_rc = DBR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_cmpres_data
 *
 * Description: Perform compression on the input data.
 *
 * Inputs:  -
 *   h_zip:       Handle to the compression engine.
 *   pt_io_buf:   Pointer to IO buffer structure that
 *                contains un-compressed data on input.
 *
 * Outputs: -
 *   pt_io_buf:   Pointer to IO buffer structure that
 *                contains compressed data on output.
 *
 *   pz_uncmp_data_len:   Length of data.
 *
 *   pz_cmp_data_len:     Length of compressed data.
 *
 * Returns: -    
 *                           
 *            
 ----------------------------------------------------------------------------*/
INT32 db_cmpres_data
(
    HANDLE_T          h_zip,
    DB_IO_BUF*        pt_io_buf,
    SIZE_T*           pz_uncmp_data_len,
    SIZE_T*           pz_cmp_data_len
)
{
    INT32             i4_rc = DBR_OK;

    /*
      Initialize the uncompress and compressed data len to
      the length obtained from db_get_cmpres_data_len_info
    */
    i4_rc = db_get_cmpres_data_len_info(h_zip,
                                        pt_io_buf,
                                        pz_uncmp_data_len,
                                        pz_cmp_data_len);
    if (i4_rc != DBR_OK)
    {
        return i4_rc;
    }

    if ( h_zip == NULL_HANDLE )
    {
        return  DBR_FAIL;   
    }
    
    if ( pt_io_buf->ui4_write_ptr > 0 )
    {
        DBG_INFO(("{CDB} Before compression: input buf size [%d bytes]\n",
                  pt_io_buf->ui4_write_ptr));
        
        /* get the compressed data len. */
        if ( x_cl_compress(h_zip,
                           pt_io_buf->pui1_buf,
                           pt_io_buf->ui4_write_ptr,
                           CL_NULL_PARAM_TYPE,
                           NULL,
                           pt_io_buf->pui1_buf,
                           pz_cmp_data_len)
             == CLR_OK
            )
        {
            DBG_INFO(("{CDB} uncompressed data len: [%d bytes] compressed data len: [%d bytes]\n",
                      *pz_uncmp_data_len, *pz_cmp_data_len));

            /* reset the IO buf valid data length to the compressed
               data len. */
            pt_io_buf->ui4_write_ptr = (UINT32)(*pz_cmp_data_len) ;
        }
        else
        {
            x_dbg_stmt("{CDB} Error: can not compress input data.\n");
            i4_rc = DBR_FAIL;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_cmpres_data_len_info
 *
 * Description: Compute the length of the compressed data and 
 *   uncompressed data from the IO buffer.
 *
 * Inputs:  -
 *   pt_io_buf:   Pointer to IO buffer structure.
 *
 * Outputs: -
 *   pz_uncmp_data_len:   Length of data
 *   pz_cmp_data_len:     Length of compressed data.
 *
 * Returns: -    
 *                           
 *            
 ----------------------------------------------------------------------------*/
INT32 db_get_cmpres_data_len_info
(
    HANDLE_T          h_zip,
    DB_IO_BUF*        pt_io_buf,
    SIZE_T*           pz_uncmp_data_len,
    SIZE_T*           pz_cmp_data_len
)
{
    INT32             i4_rc = DBR_OK;

    /* Initialize the uncompress and compressed data len to
       the length of data in the buffer.
    */
    *pz_uncmp_data_len = pt_io_buf->ui4_write_ptr ;
    *pz_cmp_data_len = pt_io_buf->ui4_write_ptr ;
    
    if ( h_zip == NULL_HANDLE )
    {
        return  DBR_FAIL;
    }
    
    if ( pt_io_buf->ui4_write_ptr > 0 )
    {
        DBG_INFO(("{CDB} Before computing compressed data len: input buf size [%d bytes]\n",
                  pt_io_buf->ui4_write_ptr));
        
        /* get the compressed data len. */
        if ( x_cl_get_compress_len(h_zip,
                                   pt_io_buf->pui1_buf,
                                   pt_io_buf->ui4_write_ptr,
                                   CL_NULL_PARAM_TYPE,
                                   NULL,
                                   pz_cmp_data_len)
             == CLR_OK
            )
        {
            DBG_INFO(("{CDB} uncompressed data len: [%d bytes] computed compressed data len: [%d bytes]\n",
                      *pz_uncmp_data_len, *pz_cmp_data_len));
        }
        else
        {
            x_dbg_stmt("{CDB} Error: can not get compress data length.\n");
            *pz_cmp_data_len = pt_io_buf->ui4_write_ptr ;
            i4_rc = DBR_FAIL;
        }
    }
    return i4_rc;
}
