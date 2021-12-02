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
 * $RCSfile: svl_fs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of internal SVL file system related
 *    API's.  These API will be called by the exported SVL file system
 *    API: x_svl_fs_load, x_svl_fs_store, x_svl_fs_del, x_svl_fs_qry.
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/
#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "util/x_end_cnvt.h"
#include "util/x_crc32.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "svl/svl.h"
#include "svl/_svl.h"
#include "svl/svl_rec.h"
#include "file_mngr/x_fm.h"
#include "svl/svl_fs.h"
#include "svl/svl_fs_raw.h"
#include "svl/svl_dbg.h"
/*-------------------------------------------------------------------
                   static  functions declaration
 --------------------------------------------------------------------*/
static INT32 svl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
);

static INT32 svl_write_file_header
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
);

static INT32 svl_write_metadata
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
    UINT32*            pui4_crc
);

static VOID svl_common_data_endconv_to_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
);

static INT32 svl_write_data
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
    UINT32*            pui4_crc
);

static INT32 svl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_svl_name,
    UINT16           ui2_svl_id,    
    VOID*            pv_tag,
    x_svl_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_svl
);

static INT32 svl_sync_data_with_file
(
    HANDLE_T         h_svl,
    HANDLE_T         h_file
);

/*------------------------------------------------------------------
                    functions implementation
 -------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Name: svl_fs_del_object
 *
 * Description: This API deletes the specified SVL object 
 *              in the file system.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Specified SVL object is deleted.
 *   SVLR_INV_HANDLE   Bad handle.
 *   SVLR_NOT_FOUND    Specified SVL object is not found.
 -----------------------------------------------------------------------*/
INT32 svl_fs_del_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16          ui2_svl_id
)
{
    INT32           i4_rc;
    i4_rc=svl_fs_raw_del(h_dir,ps_svl_path, ui2_svl_id);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_qry_persistent_objects
 *
 * Description: This API queries the directory or RAW file for
 *              SVL object in the persistent storage.
 *
 * Inputs: 
 *   ps_svl_path:   Path to RAW file.
 *   ui2_idx:       Iteration index to the SVL object.
 *
 * Outputs:
 *   pui4_svl_id:   The SVL object id found.
 *   ps_svl_name:   The SVL object name found.
 *
 * Returns: 
 *   SVLR_OK           All records are successfully removed. 
 *   SVLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 svl_qry_persistent_objects
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_svl_id,
    CHAR*           ps_svl_name	
)
{
    INT32           i4_rc;

    /*
       Access the RAW file to obtain the SVL object info.
    */
    i4_rc=svl_fs_raw_qry(h_dir,
                         ps_svl_path,
                         ui2_idx,
                         pui2_svl_id,
                         ps_svl_name);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_fs_load_object
 *
 * Description: This load a SVL object from a regular file from a 
 *              specified directory path or from a RAW file.
 *              
 *
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SVLR_INV_ARG      'pt_svl_rec' is null or the SVL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 svl_fs_load_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id,
    const CHAR*	    ps_svl_name,
    VOID*			pv_tag,
    x_svl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_svl	
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    SVL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SVL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SVL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=svl_fs_raw_get_read_hdle(h_dir,
                                   ps_svl_path,
                                   ui2_svl_id,
                                   &h_file);
    if ( i4_rc == SVLR_OK )
    {
        /* Read meta data of the SVL object. */
        if ( svl_read_file_info(h_file, &t_finfo, &ui4_crc) == SVLR_OK )
        {
            /* Assertiogn check, check the svl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_svl_id == ui2_svl_id )
            {
                /*
                  If the user did not provide a SVL name,
                  then use the SVL name stored in the
                  persistent storage.
                */
                if ( ps_svl_name == NULL )
                {
                    ps_svl_name = t_finfo.ps_svl_name;
                }

                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SVL file object loaded.

                  Create SVL handle from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=svl_create_handle_from_file(h_file,
                                                  ps_svl_name,
                                                  ui2_svl_id,
                                                  pv_tag,
                                                  pf_nfy,
                                                  ph_svl);
                if ( i4_rc == SVLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SVL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SVL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SVLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SVL} Read Error: fail CRC32 check.\n");
                            i4_rc = SVLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SVLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SVLR_OK )
                    {
                        /* clean up. */
                        x_svl_delete(*ph_svl);
                    }
                }
            }
            else
            {
                i4_rc = SVLR_FILE_READ_FAIL;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
        else
        {
            i4_rc = SVLR_FILE_READ_FAIL;
            /*
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
            */
        }
        /*
          close the file handle.
        */
        svl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  svl_fs_store_object
 *
 * Description: This function write an SVL object to a regular file or
 *              RAW file.
 *
 * Inputs: 
 *   h_svl           Handle to the SVL object.
 *   ui2_svl_rec_id  Specifies the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK            Specified record is deleted. 
 *   SVLR_INV_HANDLE    Bad SVL handle.
 *   SVLR_INV_ARG       ui2_svl_rec_id is a NULL record id.
 *   SVLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the SVL object.
 *   
 -----------------------------------------------------------------------*/
INT32 svl_fs_store_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id	   
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_SVL_NAME_LEN+1+16];
    HANDLE_T        h_file;
    HANDLE_T        h_svl;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a SVL handle to the SVL object.
    */
    if ( (i4_rc = x_svl_open(ui2_svl_id, NULL, NULL, &h_svl)) == SVLR_OK )
    {
        i4_rc= SVLR_FILE_WRITE_FAIL;
        /*
          Get the SVL control block object.
        */
        if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl))
              ==HR_OK)                                                &&
             (pe_type == SVLT_SERVICE_LIST)                           &&
             (pt_svl  != NULL)
            )
        {
            /*
              Lock the SVL object.
            */
            if ( svl_lock_object_for_read(pt_svl) == SVLR_OK )
            {
                db_clear_data_sect_sz_info(pt_svl->h_cdb);
                
                /*
                  Get the internal SVL name used for the storage
                  from the SVL id and SVL name.
                */
                svl_get_database_name(pt_svl->ps_svl_name,
                                      pt_svl->ui2_svl_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing SVL object.
                   Get the offset to location of the file where SVL object
                   will be written to.
                */
                if ( (i4_rc=svl_fs_raw_get_write_hdle(h_dir, ps_svl_path,
                                                      pt_svl, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == SVLR_OK
                    )
                {

                    DBG_API(("{SVL} start writing SVL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing SVL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=svl_is_write_to_file_needed(h_file,pt_svl,&b_write_flag))
                              == SVLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_INFO(("{SVL} [svl_fs_store_object] no change in data, write ops not needed.\n"));

                                /* x_dbg_stmt("{SVL} [svl_fs_store_object] no change in data, write ops not needed.\n"); */
                            }
                            else
                            {
                                DBG_INFO(("{SVL} [svl_fs_store_object] data changed, write ops is required.\n"));

                                /* x_dbg_stmt("{SVL} [svl_fs_store_object] data changed, write ops is required.\n"); */
                                
                            }
                            
                        }
                    }
                    else
                    {
                        /*
                          Writing SVL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == SVLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= SVLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( svl_write_file_header(h_file, pt_svl,
                                                   ui4_beg_magic, &ui4_crc) == SVLR_OK )
                        { 
                            if ( svl_write_metadata(h_file, pt_svl, &ui4_crc) == SVLR_OK )
                            {
                                if ( svl_write_data(h_file, pt_svl, &ui4_crc) == SVLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        svl_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == SVLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = svl_fs_raw_close_hdle(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        svl_fs_raw_close_hdle(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_svl->h_cdb);
                } 
                
                /*
                  don't forget to unlock the SVL
                */
                svl_unlock_object_for_read(pt_svl);
                
            }  /* Can not lock the SVL object. */
            
        }   /* Can not get the SVL control block from the handle. */
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_svl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to SVL object. */
        x_svl_close(h_svl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  svl_fs_store_object_given_file_handle
 *
 * Description: This function write an SVL object to a regular file or
 *              RAW file given the file handle..
 *
 * Inputs: 
 *   h_file          Handle to the file to write.
 *   ui2_svl_id      Specifies the SVL list ID to be stored. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK            Specified record is deleted. 
 *   SVLR_INV_HANDLE    Bad file handle.
 *   SVLR_NOT_FOUND     The specified SVL database object id (e.g. 
 *                      'ui2_svl_id') is not found.
 *   
 -----------------------------------------------------------------------*/
INT32 svl_fs_store_object_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_svl_id
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_SVL_NAME_LEN+1+16];
    HANDLE_T        h_svl;
    SVL_OBJ_T*      pt_svl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a SVL handle to the SVL object.
    */
    if ( (i4_rc = x_svl_open(ui2_svl_id, NULL, NULL, &h_svl)) == SVLR_OK )
    {
        i4_rc= SVLR_FILE_WRITE_FAIL;
        /*
          Get the SVL control block object.
        */
        if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl))
              ==HR_OK)                                                &&
             (pe_type == SVLT_SERVICE_LIST)                           &&
             (pt_svl  != NULL)
            )
        {
            /*
              Lock the SVL object.
            */
            if ( svl_lock_object_for_read(pt_svl) == SVLR_OK )
            {
                db_clear_data_sect_sz_info(pt_svl->h_cdb);
                
                /*
                  Get the internal SVL name used for the storage
                  from the SVL id and SVL name.
                */
                svl_get_database_name(pt_svl->ps_svl_name,
                                      pt_svl->ui2_svl_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing SVL object.
                   Get the offset to location of the file where SVL object
                   will be written to.
                */
                if ( (i4_rc=svl_fs_raw_get_write_hdle_no_fm_open(
                                                      pt_svl, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == SVLR_OK
                    )
                {

                    DBG_API(("{SVL} start writing SVL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing SVL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=svl_is_write_to_file_needed(h_file,pt_svl,&b_write_flag))
                              == SVLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_INFO(("{SVL} [svl_fs_store_object] no change in data, write ops not needed.\n"));

                                /* x_dbg_stmt("{SVL} [svl_fs_store_object] no change in data, write ops not needed.\n"); */
                            }
                            else
                            {
                                DBG_INFO(("{SVL} [svl_fs_store_object] data changed, write ops is required.\n"));

                                /* x_dbg_stmt("{SVL} [svl_fs_store_object] data changed, write ops is required.\n"); */
                                
                            }
                            
                        }
                    }
                    else
                    {
                        /*
                          Writing SVL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == SVLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= SVLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( svl_write_file_header(h_file, pt_svl,
                                                   ui4_beg_magic, &ui4_crc) == SVLR_OK )
                        { 
                            if ( svl_write_metadata(h_file, pt_svl, &ui4_crc) == SVLR_OK )
                            {
                                if ( svl_write_data(h_file, pt_svl, &ui4_crc) == SVLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        svl_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == SVLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = svl_fs_raw_close_hdle_no_fm_close(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        svl_fs_raw_close_hdle_no_fm_close(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_svl->h_cdb);
                } 
                
                /*
                  don't forget to unlock the SVL
                */
                svl_unlock_object_for_read(pt_svl);
                
            }  /* Can not lock the SVL object. */
            
        }   /* Can not get the SVL control block from the handle. */
        else
        {
#ifdef DEBUG
            if (x_handle_valid(h_svl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to SVL object. */
        x_svl_close(h_svl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_write_file_magic_num
 *
 * Description: This API write the SVL file magic number and CRC value.
 *                    
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_svl      Pointer to an SVL handle object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 svl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
)
{
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[SVL_FILE_TAIL_MARKER_SIZE] ;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_crc,       1, &(aui1_buf[0]) );
    x_end_cnvt_to_big_32(&ui4_end_magic, 1, &(aui1_buf[4]) );
    
    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    
    if ( i4_rc == FMR_OK                  &&
         ( sizeof(aui1_buf) == ui4_byte_wrote )
        )
    {
        /* Number of bytes written should equals number
           of bytes in the buffer.
        */
        i4_rc=SVLR_OK;

        DBG_API(("{SVL} [svl_write_obj_end_magic_crc]: SVL file end_magic_crc to filesystem.\n"));
        
    }
    else
    {
        i4_rc = SVLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_write_file_header
 *
 * Description: This API write the SVL file header. The file header
 *              consists of :
 *                    magic number (4 bytes)
 *                    file_version_number (1 byte)
 *                    metadata_size  (2 bytes)
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_svl      Pointer to an SVL handle object.
 *   ui4_beg_magic The beginning magic value.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 *
 * Outputs:
 *   pui4_crc    CRC32 value computed over the SVL metadata section. 
 *
 * Returns: 
 *   SVLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 svl_write_file_header
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
)
{
    UINT8   ui1_svl_ver;
    UINT16  ui2_meta_data_sz;
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[SVL_FILE_HEADER_SIZE] ;
    UINT32  ui4_crc;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_beg_magic, 1, &(aui1_buf[0]) );
    
    /* The current SVL library version number. */
    ui1_svl_ver=SVL_FILE_VER;
    x_memcpy(&(aui1_buf[4]), &ui1_svl_ver, 1);

    /* The metadata size. */
    ui2_meta_data_sz=SVL_FILE_METADATA_SIZE;
    
    x_end_cnvt_to_big_16(&ui2_meta_data_sz, 1, &(aui1_buf[5]) );

    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);

    if ( i4_rc == FMR_OK                  &&
         /*
           Number of bytes written should equals number
           of bytes in the buffer.
         */
         ( sizeof(aui1_buf) == ui4_byte_wrote )
        )
    {
        /*
          Compute the CRC 32 value over the header data buffer.
        */
        if ( pui4_crc != NULL )
        {
            ui4_crc = x_crc32_gen(aui1_buf, SVL_FILE_HEADER_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{SVL} [svl_write_file_header]: SVL file header wrote to filesystem.\n"));
                
        i4_rc=SVLR_OK;
    }
    else
    {
        i4_rc = SVLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_write_metadata
 *
 * Description: This API write SVL metadata section to persistent storage.  
 *              The metadata consists of:
 *                     ui2_svl_id  (2 bytes)
 *                     ps_svl_name (17 bytes)
 *                     ui4_data_size (4 bytes)
 *
 * Inputs: 
 *   h_svl       Handle to the SVL object.
 *   pt_svl      Pointer to an SVL handle object.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 * Outputs: None
  *   pui4_crc    CRC32 value computed over the SVL metadata section.  
 *
 * Returns: 
 *   SVLR_OK        success
 *   
 *
 -----------------------------------------------------------------------*/

static INT32 svl_write_metadata
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_data_sz;
    UINT8           aui1_buf[SVL_FILE_METADATA_SIZE] ;
    UINT32          ui4_byte_wrote;
    UINT32          ui4_crc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;
    INT32           i4_pos;
    
        
    /* SVL id number. */
    i4_pos=0;
    x_end_cnvt_to_big_16(&(pt_svl->ui2_svl_id), 1, &(aui1_buf[i4_pos]));
    i4_pos += 2;

    /* SVL name. */
    x_memcpy(&(aui1_buf[i4_pos]), pt_svl->ps_svl_name, MAX_SVL_NAME_LEN+1);
    i4_pos += (MAX_SVL_NAME_LEN+1);

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_svl->h_cdb,
                                 (db_endian_conv_fct)svl_data_conv_to_big_end,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_data_sz = ui4_persistent_size;
        x_end_cnvt_to_big_32(&ui4_data_sz, 1, &(aui1_buf[i4_pos]));
        i4_pos += 4;
    }
    else
    {
        return SVLR_FILE_WRITE_FAIL;
        
        /* Some thing is wrong, we are unable to get CDB object information. */
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    
    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    
    if ( i4_rc == FMR_OK                        &&
         /*
            Number of bytes written should equals number
            of bytes in the buffer.
         */
         ( sizeof(aui1_buf) == ui4_byte_wrote )
        )
    {
        /*
          Compute the CRC 32 value over the header data buffer.
        */
        if ( pui4_crc != NULL )
        {
            ui4_crc = x_crc32_gen(aui1_buf, SVL_FILE_METADATA_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{SVL} [svl_write_metadata]: SVL file metadata wrote to filesystem.\n"));
                
        i4_rc=SVLR_OK;
    }
    else
    {
        i4_rc = SVLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: svl_common_data_endconv_to_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored in CDB by the SVL library prior to writing out to
 *              the persistent storage.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID svl_common_data_endconv_to_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
)
{
    if ( x_sys_end_is_big() == TRUE )
    {
        x_memcpy(pv_output, pv_common_data, ui2_size);
    }
    else
    {
        /*
          Perform the endian conversion field by field.
        */
        x_end_cnvt_to_big_16(pv_common_data, 2, pv_output);
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: svl_common_data_endconv_from_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored on persistent storage before return to result to
 *              structure in memory.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_svl	Handle to the SVL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK          SVL database is unlocked. 
 *   SVLR_INV_HANDLE  Bad SVL handle.
 *   
 -----------------------------------------------------------------------*/
VOID svl_common_data_endconv_from_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
)
{
    if ( x_sys_end_is_big() == TRUE )
    {
        x_memcpy(pv_output, pv_common_data, ui2_size);
    }
    else
    {
        /*
          Perform the endian conversion field by field.
        */
        x_end_cnvt_from_big_16(pv_common_data, 2, pv_output);
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: svl_write_data
 *
 * Description: This function write out the data section of the SVL object.   
 *
 * Inputs: 
 *   pt_svl	Handle to the SVL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL is locked. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *
 -----------------------------------------------------------------------*/
static INT32 svl_write_data
(
    HANDLE_T           h_file,
    SVL_OBJ_T*         pt_svl,
    UINT32*            pui4_crc
) 
{
    INT32           i4_rc;

    i4_rc=db_save_to_file(pt_svl->h_cdb, h_file,
                          (db_endian_conv_fct)svl_data_conv_to_big_end,
                          svl_common_data_endconv_to_file);
    
    i4_rc = svl_cdb_to_svl_err_code(i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_read_file_info
 *
 * Description: This API read the SVL file header section and meta data 
 *              section from a specified FILE handle.
 * Inputs: 
 *   h_file:  SVL file handle.
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL file info is read successfully.
 *   
 -----------------------------------------------------------------------*/
INT32 svl_read_file_info
(
    HANDLE_T           h_file,
    SVL_FILE_INFO_T*   pt_finfo,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SVL_FILE_HEADER_SIZE+SVL_FILE_METADATA_SIZE];
    UINT32          ui4_crc;

    i4_rc=x_fm_read(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_read);
    if ( i4_rc == FMR_OK                     &&
         (SVL_FILE_HEADER_SIZE+SVL_FILE_METADATA_SIZE) == ui4_byte_read
        )
    {
        /*
          compute the CRC value from the data read.
        */
        if ( pui4_crc != NULL )
        {
             ui4_crc = x_crc32_gen(aui1_buf, ui4_byte_read, (*pui4_crc));
             *pui4_crc = ui4_crc;
        }
        
        /* check magic number. */
        x_end_cnvt_from_big_32(&(aui1_buf[0]), 1, &(pt_finfo->ui4_magic));

        DBG_API(("{SVL} read the beginning magic value: %x\n",
                   pt_finfo->ui4_magic));

        if ( pt_finfo->ui4_magic == SVL_MAGIC_NUMBER    ||
             pt_finfo->ui4_magic == (~SVL_MAGIC_NUMBER) 
            )
        {
            /*
              Get the version number.
            */
            x_memcpy(&(pt_finfo->ui1_svl_ver), &(aui1_buf[4]), 1);

            DBG_API(("{SVL} version number read from file object is: %d "
                     "current file version in memory is %d\n",
                     pt_finfo->ui1_svl_ver, SVL_FILE_VER));
            /*
              Check the SVL file version number. 
            */
            if ( pt_finfo->ui1_svl_ver == SVL_FILE_VER )
            {
                /*
                  Get the size of the metadata section.
                */
                x_end_cnvt_from_big_16(&(aui1_buf[5]),1,&(pt_finfo->ui2_meta_data_sz));

                /*
                  Get the svl id
                */
                x_end_cnvt_from_big_16(&(aui1_buf[7]),1,&(pt_finfo->ui2_svl_id));

                /*
                  Get the svl name.
                */
                x_memcpy(pt_finfo->ps_svl_name,&(aui1_buf[9]),MAX_SVL_NAME_LEN+1);

                /*
                  Get the data section size.
                */
                x_end_cnvt_from_big_32(&(aui1_buf[9+MAX_SVL_NAME_LEN+1]),
                                       1,&(pt_finfo->ui4_data_size));
                i4_rc=SVLR_OK;
            }
            else
            {
                DBG_API(("{SVL} Wrong SVL file version encountered: current software ver: %d  file ver %d.\n",
                        SVL_FILE_VER , pt_finfo->ui1_svl_ver ));
                
                
                i4_rc = SVLR_INV_FILE_VER;
            }
        }
        else
        {
            DBG_API(("{SVL} bad file magic value: the correct magic value is %x or %x\n",
                     SVL_MAGIC_NUMBER, (~SVL_MAGIC_NUMBER)));

            /*
              Wrong magic number.
            */
            i4_rc=SVLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = SVLR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: svl_create_handle_from_file
 *
 * Description: This API create an SVL object by loading the paramters
 *              and records from persistent storage.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK             SVL library is initialized successfully.
 *   SVLR_OUT_OF_MEM     Not enough memory to create the SVL object.
 *   SVLR_INV_ARG        'ui2_max_num_svl_obj' is 0.
 *   SVLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 svl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_svl_name,
    UINT16           ui2_svl_id,    
    VOID*            pv_tag,
    x_svl_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_svl
)
{
    CHAR               s_name[CDB_MAX_DATABASE_NAME_LEN+1];
    SVL_OBJ_T*         pt_svl=NULL;
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    HANDLE_T           h_cdb=NULL_HANDLE;
    INT32              i4_rc=SVLR_FAIL;
    
    /* create CDB name. */
    svl_get_database_name(ps_svl_name,ui2_svl_id,s_name);

    /* get the record definition. */
    svl_db_rec_and_key_def(&ui1_fld_count,
                           &pt_rec_fields,
                           &pui4_recfld_type,
                           &ui1_key_count,
                           &pui4_keyfld_type);
    
    svl_sema_lock();

    /* check if the input SVL name and id has not been used.
       then, add the name and id to the internal list. */

    if ( svl_check_id_name(ui2_svl_id,ps_svl_name) == TRUE &&
         svl_add_id_name(ui2_svl_id,ps_svl_name) == SVLR_OK
        )
    {
        /* Create a SVL object. */
        pt_svl=(SVL_OBJ_T*) x_mem_alloc(sizeof(SVL_OBJ_T));

        /* Create a core database object. */
        if ( pt_svl != NULL )
        {
            i4_rc=db_load_from_file(
                s_name,
                ui1_fld_count,
                pt_rec_fields,
                ui1_key_count,
                pui4_keyfld_type,
                h_file,
                /* Endian conversion function. */
                (db_endian_conv_fct)svl_data_conv_from_big_end,
                /* Common data endian conversion function. */
                (db_cdata_conv_fct)svl_common_data_endconv_from_file,
                (VOID*)(&pt_svl),
                svl_nfy_fct,
                &h_cdb);
            
            /* translate the CDB error code to SVL error code */
            i4_rc=svl_cdb_to_svl_err_code(i4_rc);
            if ( i4_rc == SVLR_OK )
            {

                DBG_API(("{SVL} SVL's cdb object loaded.\n"));

                /* register a persistent write function to control
                   how certain field values are modified prior
                   to storing on the file system.
                */
                db_reg_persistent_write_fct(h_cdb,
                                            svl_write_persistent_data);

                /*
                  Allocate a handle for the SVL object.
                */
                if ( handle_alloc(SVLT_SERVICE_LIST,
                                  (VOID*)pt_svl,
                                  pv_tag,
                                  svl_handle_free_fct,
                                  ph_svl)
                     == HR_OK )
                {
                    /* initialize the SVL object. */
                    pt_svl->h_svl=*ph_svl;
                    pt_svl->h_cdb=h_cdb;
                    pt_svl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                      caller's notification
                                                      function. */
                    pt_svl->ui2_svl_id=ui2_svl_id;
                    x_strncpy(pt_svl->ps_svl_name,ps_svl_name,
                              MAX_SVL_NAME_LEN);

                    /* Set the initial search parameter and cursor position
                       to unknown initial values
                       This will force sorted list to be created and search
                       to start from the first element of array.
                    */
                    x_memset(&(pt_svl->t_parm), SVL_UNSET_VAL,
                         sizeof(SVL_SEARCH_PARM_T));

                    x_memset(&(pt_svl->t_cursor), SVL_UNSET_VAL,
                         sizeof(SVL_CURSOR_T));

                    x_memset(&(pt_svl->t_cmp_fct_info), 0,
                         sizeof(SVL_CMP_FCT_INFO_T));
                
                    i4_rc=SVLR_OK;

                    DBG_API(("{SVL} SVL object created from file\n"));
                }
                else
                {
                    i4_rc=SVLR_OUT_OF_HANDLE;
                }
            }
        }
        else
        {
            i4_rc = SVLR_OUT_OF_MEM ;
        }

        if ( i4_rc != SVLR_OK )
        {
            /* Remove the id name pair. */
            svl_del_id(ui2_svl_id);
        }
    }
    else
    {
        i4_rc = SVLR_FAIL;
    }
    svl_sema_unlock();
    
    if ( i4_rc != SVLR_OK )
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,s_name);
        }
        if ( pt_svl != NULL )
        {
            x_mem_free(pt_svl);
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: svl_sync_data_with_file
 *
 * Description:
 *    This API create synchronize SVL object by loading the paramters
 *    and records from persistent storage. The existing data are over-
 *    written with data from the specified file.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   SVLR_OK             SVL library is initialized successfully.
 *   SVLR_OUT_OF_MEM     Not enough memory to create the SVL object.
 *   SVLR_INV_ARG        'ui2_max_num_svl_obj' is 0.
 *   SVLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 svl_sync_data_with_file
(
    HANDLE_T         h_svl,
    HANDLE_T         h_file
)
{
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    INT32              i4_rc=SVLR_FAIL;
    SVL_OBJ_T*         pt_svl=NULL;
    HANDLE_TYPE_T      pe_type;

    if ( (handle_get_type_obj(h_svl, &pe_type,((VOID**) &pt_svl)) == HR_OK) &&
         (pe_type == SVLT_SERVICE_LIST)                                     &&
         (pt_svl  != NULL)
        )
    {
        /* get the record definition. */
        svl_db_rec_and_key_def(&ui1_fld_count,
                               &pt_rec_fields,
                               &pui4_recfld_type,
                               &ui1_key_count,
                               &pui4_keyfld_type);
    
        /* Load the data from the file into CDB database object. */
        if ( pt_svl != NULL )
        {
            i4_rc=db_sync_data_with_file(pt_svl->h_cdb,
                                         ui1_fld_count,
                                         pt_rec_fields,
                                         ui1_key_count,
                                         pui4_keyfld_type,
                                         h_file,
                                         /* Endian conversion function. */
                                         (db_endian_conv_fct)
                                         svl_data_conv_from_big_end,
                                         /* Common data endian conversion
                                            function. */
                                         (db_cdata_conv_fct)
                                         svl_common_data_endconv_from_file
                );
            
            /* translate the CDB error code to SVL error code */
            i4_rc=svl_cdb_to_svl_err_code(i4_rc);
            if ( i4_rc == SVLR_OK )
            {
                DBG_API(("{SVL} SVL records are sync'd with file\n"));
            }
        }
        else
        {
            i4_rc = SVLR_INV_ARG ;
        }
    }
    else
    {
#ifdef DEBUG
        if (x_handle_valid(h_svl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SVLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_persistent_size
 *
 * Description: This API returns the space needed to store
 *              a SVL object on the file system.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   size (in bytes) for storing the SVL object to file. The size includes
 *   the header (magic number, etc), metadata, and the CDB object which
 *   contains SVL records.
 *   0 if the SVL object size can not be determine.
 *   
 -----------------------------------------------------------------------*/
UINT32 svl_persistent_size
(
    SVL_OBJ_T*      pt_svl
)
{
    UINT32          ui4_sz;
    INT32           i4_rc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_svl->h_cdb,
                                 (db_endian_conv_fct)svl_data_conv_to_big_end,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_sz = SVL_FILE_HEADER_SIZE + SVL_FILE_METADATA_SIZE +
            ui4_persistent_size + SVL_FILE_TAIL_MARKER_SIZE ;
    }
    else
    {
        ui4_sz = 0;
        /* Some thing is wrong, we are unable to get CDB object information. */
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return ui4_sz;
}

/*----------------------------------------------------------------------
 * Name: svl_is_write_to_file_needed
 *
 * Description: This API compare the SVL object stored on filesystem
 *              with SVL object in memory. If they contains different
 *              data, then save (e.g., write operation) will be needed.
 *
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs:
 *   pb_rc:     TRUE  SVL object in memory and SVL object in filesystem
 *                    contains same data, no write operation is needed.
 *              FALSE SVL object in memory and SVL object in filesystem
 *                    contains different data, write operation is needed.
 *
 * Returns: 
 *   SVLR_OK:      
 *   SVLR_FILE_READ_FAIL:   Can not read the SVL object on filesystme.
 *
 -----------------------------------------------------------------------*/
INT32 svl_is_write_to_file_needed
(
    HANDLE_T        h_file,
    SVL_OBJ_T*      pt_svl,
    BOOL*           pb_rc
)
{
    UINT64          ui8_fpos;
    UINT64          ui8_end_fpos;
    INT32           i4_rc;
    SVL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    BOOL            b_result;
    INT32           i4_res;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    i4_rc  = SVLR_OK;
    *pb_rc = FALSE;
    i4_res = 0;
    
    /*
      Remember the current location of the File pointer.
    */
    if ( (i4_res=x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_fpos)) == FMR_OK )
    {
        /*
          Initialize the CRC seed value to all 1's.
        */
        ui4_crc = 0xFFFFFFFF;

        /* Read meta data of the SVL object. */
        if ( svl_read_file_info(h_file, &t_finfo, &ui4_crc) == SVLR_OK )
        {
            /*
              Compare the SVL file header and metadata information
              with the SVL object in memory to see if they contain
              different data.
            */
            if ( t_finfo.ui1_svl_ver != SVL_FILE_VER )
            {
                DBG_INFO(("{SVL} [svl_is_write_to_file_needed] svl_ver diff\n"));
                
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_svl_id != pt_svl-> ui2_svl_id )
            {
                DBG_INFO(("{SVL} [svl_is_write_to_file_needed] svl_id diff\n"));
                *pb_rc = TRUE;
            }
            else if ( x_strcmp(t_finfo.ps_svl_name, pt_svl->ps_svl_name) != 0 )
            {
                DBG_INFO(("{SVL} [svl_is_write_to_file_needed] svl_name diff\n"));
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_meta_data_sz != SVL_FILE_METADATA_SIZE )
            {
                DBG_INFO(("{SVL} [svl_is_write_to_file_needed] svl_metadata size diff\n"));
                *pb_rc = TRUE;
            }
            else
            {
                /*
                  Check the data size between memory and size of data
                  stored on file.
                */
                if ( db_get_persistent_size(pt_svl->h_cdb,
                                            (db_endian_conv_fct)svl_data_conv_to_big_end,
                                            &ui4_persistent_size,
                                            &ui4_ver_id)
                     == DBR_OK )
                {

                    DBG_INFO(("{SVL} size of SVL obj on file: [%d bytes], size of SVL data in memory: [%d bytes]\n",
                               ui4_persistent_size, t_finfo.ui4_data_size));                   
                    
                    if ( ui4_persistent_size != t_finfo.ui4_data_size )
                    {
                        DBG_API(("{SVL} data section size difference between SVL obj in memory and on filesystem\n"));
                        *pb_rc = TRUE;
                    }
                    /*
                      check if writing to CDB object is needed.
                    */
                    else if ( db_is_save_to_file_needed(
                                  pt_svl->h_cdb, h_file,
                                  (db_endian_conv_fct)svl_data_conv_to_big_end,
                                  NULL,
                                  &b_result) == DBR_OK
                        )
                    {
                        if ( b_result == TRUE )
                        {
                            DBG_API(("{SVL} writing CDB data to file system is needed.\n"));
                            *pb_rc = TRUE;
                        }
                    }
                    else
                    {
                        i4_rc = SVLR_FILE_READ_FAIL;
                    }
                }
                else
                {
                    i4_rc= SVLR_FILE_READ_FAIL;
                }
            }
        }
        else
        {
            DBG_API(("{SVL} can not read metadata info from svl object on the file system,"
                     " enable the write flag to write over the svl object in file. \n"));

            *pb_rc = TRUE;
            
        }
        
        /*
          Reset the file position.
        */
        if ( x_fm_lseek(h_file, (INT64) ui8_fpos,
                        FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
            )
        {
            i4_rc = SVLR_OK;
        }
        else
        {
            i4_rc = SVLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = SVLR_FILE_READ_FAIL;
        DBG_ERROR(("{SVL} [svl_is_write_to_file_needed] x_fm_lseek error: %d\n",i4_res));
    }
    
    /* remove warning. */
    i4_res=(i4_res+1);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_fs_sync_object
 *
 * Description:
 *   This API sync an existing SVL object with records
 *   from a persistent SVL object stored on file system.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SVLR_INV_ARG      'pt_svl_rec' is null or the SVL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 svl_fs_sync_object
(
    HANDLE_T		h_svl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_svl_path,
    UINT16		    ui2_svl_id
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    SVL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SVL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SVL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=svl_fs_raw_get_read_hdle(h_dir,
                                   ps_svl_path,
                                   ui2_svl_id,
                                   &h_file);
    if ( i4_rc == SVLR_OK )
    {
        /* Read meta data of the SVL object. */
        if ( svl_read_file_info(h_file, &t_finfo, &ui4_crc) == SVLR_OK )
        {
            /* Assertiogn check, check the svl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_svl_id == ui2_svl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SVL file object loaded.

                  Sync SVL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=svl_sync_data_with_file(h_svl, h_file);
                if ( i4_rc == SVLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SVL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SVL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SVLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SVL} Read Error: fail CRC32 check.\n");
                            i4_rc = SVLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SVLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SVLR_OK )
                    {
                        /* clean up. */
                        x_svl_delete(h_svl);
                    }
                }
            }
            else
            {
                i4_rc = SVLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = SVLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        svl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: svl_fs_sync_object_given_file_handle
 *
 * Description:
 *   This API sync an existing SVL object with records
 *   from a persistent SVL object stored on file system.
 *
 * Inputs: 
 *   h_svl      Handle to the SVL object.
 *   pt_svl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           Record is successfully added. 
 *   SVLR_INV_HANDLE   Bad SVL handle.
 *   SVLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SVLR_INV_ARG      'pt_svl_rec' is null or the SVL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 svl_fs_sync_object_given_file_handle
(
    HANDLE_T		h_svl,	
    HANDLE_T		h_file,	
    UINT16		    ui2_svl_id
)
{
    INT32           i4_rc;
    SVL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SVL_FILE_TAIL_MARKER_SIZE];

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SVL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=svl_fs_raw_get_read_hdle_no_fm_open(
                                   ui2_svl_id,
                                   &h_file);
    if ( i4_rc == SVLR_OK )
    {
        /* Read meta data of the SVL object. */
        if ( svl_read_file_info(h_file, &t_finfo, &ui4_crc) == SVLR_OK )
        {
            /* Assertiogn check, check the svl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_svl_id == ui2_svl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SVL file object loaded.

                  Sync SVL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=svl_sync_data_with_file(h_svl, h_file);
                if ( i4_rc == SVLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SVL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SVL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SVLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SVL} Read Error: fail CRC32 check.\n");
                            i4_rc = SVLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SVLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SVLR_OK )
                    {
                        /* clean up. */
                        x_svl_delete(h_svl);
                    }
                }
            }
            else
            {
                i4_rc = SVLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = SVLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        svl_fs_raw_close_hdle_no_fm_close(h_file);
    }
    return i4_rc;
}
