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
 * $RCSfile: nwl_fs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of internal NWL file system related
 *    API's.  These API will be called by the exported NWL file system
 *    API: x_nwl_fs_load, x_nwl_fs_store, x_nwl_fs_del, x_nwl_fs_qry.
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

#include "nwl/_nwl.h"
#include "nwl/nwl_rec.h"
#include "file_mngr/x_fm.h"
#include "nwl/nwl_fs.h"
#include "nwl/nwl_fs_raw.h"
#include "nwl/nwl_dbg.h"
/*-------------------------------------------------------------------
                   static  functions declaration
 --------------------------------------------------------------------*/
static INT32 nwl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
);

static INT32 nwl_write_file_header
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
);

static INT32 nwl_write_metadata
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
    UINT32*            pui4_crc
);

static VOID nwl_common_data_endconv_to_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
);

static INT32 nwl_write_data
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
    UINT32*            pui4_crc
);

static INT32 nwl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_nwl_name,
    UINT16           ui2_nwl_id,    
    VOID*            pv_tag,
    x_nwl_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_nwl
);

static INT32 nwl_sync_data_with_file
(
    HANDLE_T         h_nwl,
    HANDLE_T         h_file
);

/*------------------------------------------------------------------
                    functions implementation
 -------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Name: nwl_fs_del_object
 *
 * Description: This API deletes the specified NWL object 
 *              in the file system.
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Specified NWL object is deleted.
 *   NWLR_INV_HANDLE   Bad handle.
 *   NWLR_NOT_FOUND    Specified NWL object is not found.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_del_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16          ui2_nwl_id
)
{
    INT32           i4_rc;
    i4_rc=nwl_fs_raw_del(h_dir,ps_nwl_path, ui2_nwl_id);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_qry_persistent_objects
 *
 * Description: This API queries the directory or RAW file for
 *              NWL object in the persistent storage.
 *
 * Inputs: 
 *   ps_nwl_path:   Path to RAW file.
 *   ui2_idx:       Iteration index to the NWL object.
 *
 * Outputs:
 *   pui4_nwl_id:   The NWL object id found.
 *   ps_nwl_name:   The NWL object name found.
 *
 * Returns: 
 *   NWLR_OK           All records are successfully removed. 
 *   NWLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 nwl_qry_persistent_objects
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_nwl_id,
    CHAR*           ps_nwl_name	
)
{
    INT32           i4_rc;

    /*
       Access the RAW file to obtain the NWL object info.
    */
    i4_rc=nwl_fs_raw_qry(h_dir,
                         ps_nwl_path,
                         ui2_idx,
                         pui2_nwl_id,
                         ps_nwl_name);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_load_object
 *
 * Description: This load a NWL object from a regular file from a 
 *              specified directory path or from a RAW file.
 *              
 *
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *   NWLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   NWLR_INV_ARG      'pt_nwl_rec' is null or the NWL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_load_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_nwl_id,
    const CHAR*	    ps_nwl_name,
    VOID*			pv_tag,
    x_nwl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_nwl	
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    NWL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[NWL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the NWL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=nwl_fs_raw_get_read_hdle(h_dir,
                                   ps_nwl_path,
                                   ui2_nwl_id,
                                   &h_file);
    if ( i4_rc == NWLR_OK )
    {
        /* Read meta data of the NWL object. */
        if ( nwl_read_file_info(h_file, &t_finfo, &ui4_crc) == NWLR_OK )
        {
            /* Assertiogn check, check the nwl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_nwl_id == ui2_nwl_id )
            {
                /*
                  If the user did not provide a NWL name,
                  then use the NWL name stored in the
                  persistent storage.
                */
                if ( ps_nwl_name == NULL )
                {
                    ps_nwl_name = t_finfo.ps_nwl_name;
                }

                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  NWL file object loaded.

                  Create NWL handle from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=nwl_create_handle_from_file(h_file,
                                                  ps_nwl_name,
                                                  ui2_nwl_id,
                                                  pv_tag,
                                                  pf_nfy,
                                                  ph_nwl);
                if ( i4_rc == NWLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, NWL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == NWL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = NWLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{NWL} Read Error: fail CRC32 check.\n");
                            i4_rc = NWLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = NWLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != NWLR_OK )
                    {
                        /* clean up. */
                        x_nwl_delete(*ph_nwl);
                    }
                }
            }
            else
            {
                i4_rc = NWLR_FILE_READ_FAIL;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
        else
        {
            i4_rc = NWLR_FILE_READ_FAIL;
            /*
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
            */
        }
        /*
          close the file handle.
        */
        nwl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  nwl_fs_store_object
 *
 * Description: This function write an NWL object to a regular file or
 *              RAW file.
 *
 * Inputs: 
 *   h_nwl           Handle to the NWL object.
 *   ui2_nwl_rec_id  Specifies the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK            Specified record is deleted. 
 *   NWLR_INV_HANDLE    Bad NWL handle.
 *   NWLR_INV_ARG       ui2_nwl_rec_id is a NULL record id.
 *   NWLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the NWL object.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_fs_store_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_nwl_id	   
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_NWL_NAME_LEN+1+16];
    HANDLE_T        h_file;
    HANDLE_T        h_nwl;
    NWL_OBJ_T*      pt_nwl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a NWL handle to the NWL object.
    */
    if ( (i4_rc = x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl)) == NWLR_OK )
    {
        i4_rc= NWLR_FILE_WRITE_FAIL;
        /*
          Get the NWL control block object.
        */
        if ( (handle_get_type_obj(h_nwl, &pe_type,((VOID**) &pt_nwl))
              ==HR_OK)                                                &&
             (pe_type == NWLT_NETWORK_LIST)                           &&
             (pt_nwl  != NULL)
            )
        {
            /*
              Lock the NWL object.
            */
            if ( nwl_lock_object_for_read(pt_nwl) == NWLR_OK )
            {
                db_clear_data_sect_sz_info(pt_nwl->h_cdb);
                
                /*
                  Get the internal NWL name used for the storage
                  from the NWL id and NWL name.
                */
                nwl_get_database_name(pt_nwl->ps_nwl_name,
                                      pt_nwl->ui2_nwl_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing NWL object.
                   Get the offset to location of the file where NWL object
                   will be written to.
                */
                if ( (i4_rc=nwl_fs_raw_get_write_hdle(h_dir, ps_nwl_path,
                                                      pt_nwl, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == NWLR_OK
                    )
                {

                    DBG_API(("{NWL} start writing NWL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing NWL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=nwl_is_write_to_file_needed(h_file,pt_nwl,&b_write_flag))
                              == NWLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_API(("{NWL} [nwl_fs_store_object] no change in data, write ops not needed.\n"));
                            }
                        }
                    }
                    else
                    {
                        /*
                          Writing NWL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == NWLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= NWLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( nwl_write_file_header(h_file, pt_nwl,
                                                   ui4_beg_magic, &ui4_crc) == NWLR_OK )
                        { 
                            if ( nwl_write_metadata(h_file, pt_nwl, &ui4_crc) == NWLR_OK )
                            {
                                if ( nwl_write_data(h_file, pt_nwl, &ui4_crc) == NWLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        nwl_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == NWLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = nwl_fs_raw_close_hdle(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        nwl_fs_raw_close_hdle(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_nwl->h_cdb);
                } 
                
                /*
                  don't forget to unlock the NWL
                */
                nwl_unlock_object_for_read(pt_nwl);
                
            }  /* Can not lock the NWL object. */
            
        }   /* Can not get the NWL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_nwl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to NWL object. */
        x_nwl_close(h_nwl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  nwl_fs_store_object_given_file_handle
 *
 * Description: This function write an NWL object to a regular file or
 *              RAW file given the file handle..
 *
 * Inputs: 
 *   h_file          Handle to the file to write.
 *   ui2_nwl_id      Specifies the NWL list ID to be stored. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK            Specified record is deleted. 
 *   NWLR_INV_HANDLE    Bad file handle.
 *   NWLR_NOT_FOUND     The specified NWL database object id (e.g. 
 *                      'ui2_nwl_id') is not found.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_fs_store_object_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_nwl_id
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_NWL_NAME_LEN+1+16];
    HANDLE_T        h_nwl;
    NWL_OBJ_T*      pt_nwl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a NWL handle to the NWL object.
    */
    if ( (i4_rc = x_nwl_open(ui2_nwl_id, NULL, NULL, &h_nwl)) == NWLR_OK )
    {
        i4_rc= NWLR_FILE_WRITE_FAIL;
        /*
          Get the NWL control block object.
        */
        if ( (handle_get_type_obj(h_nwl, &pe_type,((VOID**) &pt_nwl))
              ==HR_OK)                                                &&
             (pe_type == NWLT_NETWORK_LIST)                           &&
             (pt_nwl  != NULL)
            )
        {
            /*
              Lock the NWL object.
            */
            if ( nwl_lock_object_for_read(pt_nwl) == NWLR_OK )
            {
                db_clear_data_sect_sz_info(pt_nwl->h_cdb);
                
                /*
                  Get the internal NWL name used for the storage
                  from the NWL id and NWL name.
                */
                nwl_get_database_name(pt_nwl->ps_nwl_name,
                                      pt_nwl->ui2_nwl_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing NWL object.
                   Get the offset to location of the file where NWL object
                   will be written to.
                */
                if ( (i4_rc=nwl_fs_raw_get_write_hdle_no_fm_open(
                                                      pt_nwl, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == NWLR_OK
                    )
                {

                    DBG_API(("{NWL} start writing NWL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing NWL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=nwl_is_write_to_file_needed(h_file,pt_nwl,&b_write_flag))
                              == NWLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_INFO(("{NWL} [nwl_fs_store_object] no change in data, write ops not needed.\n"));

                                /* x_dbg_stmt("{NWL} [nwl_fs_store_object] no change in data, write ops not needed.\n"); */
                            }
                            else
                            {
                                DBG_INFO(("{NWL} [nwl_fs_store_object] data changed, write ops is required.\n"));

                                /* x_dbg_stmt("{NWL} [nwl_fs_store_object] data changed, write ops is required.\n"); */
                                
                            }
                            
                        }
                    }
                    else
                    {
                        /*
                          Writing NWL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == NWLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= NWLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( nwl_write_file_header(h_file, pt_nwl,
                                                   ui4_beg_magic, &ui4_crc) == NWLR_OK )
                        { 
                            if ( nwl_write_metadata(h_file, pt_nwl, &ui4_crc) == NWLR_OK )
                            {
                                if ( nwl_write_data(h_file, pt_nwl, &ui4_crc) == NWLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        nwl_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == NWLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = nwl_fs_raw_close_hdle_no_fm_close(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        nwl_fs_raw_close_hdle_no_fm_close(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_nwl->h_cdb);
                } 
                
                /*
                  don't forget to unlock the NWL
                */
                nwl_unlock_object_for_read(pt_nwl);
                
            }  /* Can not lock the NWL object. */
            
        }   /* Can not get the NWL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_nwl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to NWL object. */
        x_nwl_close(h_nwl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_write_file_magic_num
 *
 * Description: This API write the NWL file magic number and CRC value.
 *                    
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_nwl      Pointer to an NWL handle object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 nwl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
)
{
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[NWL_FILE_TAIL_MARKER_SIZE] ;
    
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
        i4_rc=NWLR_OK;

        DBG_API(("{NWL} [nwl_write_obj_end_magic_crc]: NWL file end_magic_crc to filesystem.\n"));
        
    }
    else
    {
        i4_rc = NWLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_write_file_header
 *
 * Description: This API write the NWL file header. The file header
 *              consists of :
 *                    magic number (4 bytes)
 *                    file_version_number (1 byte)
 *                    metadata_size  (2 bytes)
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_nwl      Pointer to an NWL handle object.
 *   ui4_beg_magic The beginning magic value.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 *
 * Outputs:
 *   pui4_crc    CRC32 value computed over the NWL metadata section. 
 *
 * Returns: 
 *   NWLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 nwl_write_file_header
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
)
{
    UINT8   ui1_nwl_ver;
    UINT16  ui2_meta_data_sz;
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[NWL_FILE_HEADER_SIZE] ;
    UINT32  ui4_crc;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_beg_magic, 1, &(aui1_buf[0]) );
    
    /* The current NWL library version number. */
    ui1_nwl_ver=NWL_FILE_VER;
    x_memcpy(&(aui1_buf[4]), &ui1_nwl_ver, 1);

    /* The metadata size. */
    ui2_meta_data_sz=NWL_FILE_METADATA_SIZE;
    
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
            ui4_crc = x_crc32_gen(aui1_buf, NWL_FILE_HEADER_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{NWL} [nwl_write_file_header]: NWL file header wrote to filesystem.\n"));
                
        i4_rc=NWLR_OK;
    }
    else
    {
        i4_rc = NWLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_write_metadata
 *
 * Description: This API write NWL metadata section to persistent storage.  
 *              The metadata consists of:
 *                     ui2_nwl_id  (2 bytes)
 *                     ps_nwl_name (17 bytes)
 *                     ui4_data_size (4 bytes)
 *
 * Inputs: 
 *   h_nwl       Handle to the NWL object.
 *   pt_nwl      Pointer to an NWL handle object.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 * Outputs: None
  *   pui4_crc    CRC32 value computed over the NWL metadata section.  
 *
 * Returns: 
 *   NWLR_OK        success
 *   
 *
 -----------------------------------------------------------------------*/

static INT32 nwl_write_metadata
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_data_sz;
    UINT8           aui1_buf[NWL_FILE_METADATA_SIZE] ;
    UINT32          ui4_byte_wrote;
    UINT32          ui4_crc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;
    INT32           i4_pos;
    
        
    /* NWL id number. */
    i4_pos=0;
    x_end_cnvt_to_big_16(&(pt_nwl->ui2_nwl_id), 1, &(aui1_buf[i4_pos]));
    i4_pos += 2;

    /* NWL name. */
    x_memcpy(&(aui1_buf[i4_pos]), pt_nwl->ps_nwl_name, MAX_NWL_NAME_LEN);
    i4_pos += (MAX_NWL_NAME_LEN+1);

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_nwl->h_cdb,
                                 (db_endian_conv_fct)nwl_data_conv_to_big_end,
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
        return NWLR_FILE_WRITE_FAIL;
        
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
            ui4_crc = x_crc32_gen(aui1_buf, NWL_FILE_METADATA_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{NWL} [nwl_write_metadata]: NWL file metadata wrote to filesystem.\n"));
                
        i4_rc=NWLR_OK;
    }
    else
    {
        i4_rc = NWLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: nwl_common_data_endconv_to_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored in CDB by the NWL library prior to writing out to
 *              the persistent storage.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK          NWL database is unlocked. 
 *   NWLR_INV_HANDLE  Bad NWL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID nwl_common_data_endconv_to_file
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
 * Name: nwl_common_data_endconv_from_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored on persistent storage before return to result to
 *              structure in memory.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_nwl	Handle to the NWL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK          NWL database is unlocked. 
 *   NWLR_INV_HANDLE  Bad NWL handle.
 *   
 -----------------------------------------------------------------------*/
VOID nwl_common_data_endconv_from_file
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
 * Name: nwl_write_data
 *
 * Description: This function write out the data section of the NWL object.   
 *
 * Inputs: 
 *   pt_nwl	Handle to the NWL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL is locked. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *
 -----------------------------------------------------------------------*/
static INT32 nwl_write_data
(
    HANDLE_T           h_file,
    NWL_OBJ_T*         pt_nwl,
    UINT32*            pui4_crc
) 
{
    INT32           i4_rc;

    i4_rc=db_save_to_file(pt_nwl->h_cdb, h_file,
                          (db_endian_conv_fct)nwl_data_conv_to_big_end,
                          nwl_common_data_endconv_to_file);
    
    i4_rc = nwl_cdb_to_nwl_err_code(i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_read_file_info
 *
 * Description: This API read the NWL file header section and meta data 
 *              section from a specified FILE handle.
 * Inputs: 
 *   h_file:  NWL file handle.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL file info is read successfully.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_read_file_info
(
    HANDLE_T           h_file,
    NWL_FILE_INFO_T*   pt_finfo,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[NWL_FILE_HEADER_SIZE+NWL_FILE_METADATA_SIZE];
    UINT32          ui4_crc;

    i4_rc=x_fm_read(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_read);
    if ( i4_rc == FMR_OK                     &&
         (NWL_FILE_HEADER_SIZE+NWL_FILE_METADATA_SIZE) == ui4_byte_read
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

        DBG_API(("{NWL} read the beginning magic value: %x\n",
                   pt_finfo->ui4_magic));

        if ( pt_finfo->ui4_magic == NWL_MAGIC_NUMBER    ||
             pt_finfo->ui4_magic == (~NWL_MAGIC_NUMBER) 
            )
        {
            /*
              Get the version number.
            */
            x_memcpy(&(pt_finfo->ui1_nwl_ver), &(aui1_buf[4]), 1);

            DBG_API(("{NWL} version number read from file object is: %d "
                     "current file version in memory is %d\n",
                     pt_finfo->ui1_nwl_ver, NWL_FILE_VER));
            /*
              Check the NWL file version number. 
            */
            if ( pt_finfo->ui1_nwl_ver == NWL_FILE_VER )
            {
                /*
                  Get the size of the metadata section.
                */
                x_end_cnvt_from_big_16(&(aui1_buf[5]),1,&(pt_finfo->ui2_meta_data_sz));

                /*
                  Get the nwl id
                */
                x_end_cnvt_from_big_16(&(aui1_buf[7]),1,&(pt_finfo->ui2_nwl_id));

                /*
                  Get the nwl name.
                */
                x_memcpy(pt_finfo->ps_nwl_name,&(aui1_buf[9]),MAX_NWL_NAME_LEN);

                /*
                  Get the data section size.
                */
                x_end_cnvt_from_big_32(&(aui1_buf[9+MAX_NWL_NAME_LEN+1]),
                                       1,&(pt_finfo->ui4_data_size));
                i4_rc=NWLR_OK;
            }
            else
            {
                DBG_API(("{NWL} Wrong NWL file version encountered: current software ver: %d  file ver %d.\n",
                        NWL_FILE_VER , pt_finfo->ui1_nwl_ver ));
                
                
                i4_rc = NWLR_INV_FILE_VER;
            }
        }
        else
        {
            DBG_API(("{NWL} bad file magic value: the correct magic value is %x or %x\n",
                     NWL_MAGIC_NUMBER, (~NWL_MAGIC_NUMBER)));

            /*
              Wrong magic number.
            */
            i4_rc=NWLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = NWLR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: nwl_create_handle_from_file
 *
 * Description: This API create an NWL object by loading the paramters
 *              and records from persistent storage.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   NWLR_OK             NWL library is initialized successfully.
 *   NWLR_OUT_OF_MEM     Not enough memory to create the NWL object.
 *   NWLR_INV_ARG        'ui2_max_num_nwl_obj' is 0.
 *   NWLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 nwl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_nwl_name,
    UINT16           ui2_nwl_id,    
    VOID*            pv_tag,
    x_nwl_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_nwl
)
{
    CHAR               s_name[CDB_MAX_DATABASE_NAME_LEN+1];
    NWL_OBJ_T*         pt_nwl=NULL;
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    HANDLE_T           h_cdb=NULL_HANDLE;
    INT32              i4_rc=NWLR_FAIL;
    
    /* create CDB name. */
    nwl_get_database_name(ps_nwl_name,ui2_nwl_id,s_name);

    /* get the record definition. */
    nwl_db_rec_and_key_def(&ui1_fld_count,
                           &pt_rec_fields,
                           &pui4_recfld_type,
                           &ui1_key_count,
                           &pui4_keyfld_type);
    
    nwl_sema_lock();

    /* check if the input NWL name and id has not been used.
       then, add the name and id to the internal list. */

    if ( nwl_check_id_name(ui2_nwl_id,ps_nwl_name) == TRUE &&
         nwl_add_id_name(ui2_nwl_id,ps_nwl_name) == NWLR_OK
        )
    {
        /* Create a NWL object. */
        pt_nwl=(NWL_OBJ_T*) x_mem_alloc(sizeof(NWL_OBJ_T));

        /* Create a core database object. */
        if ( pt_nwl != NULL )
        {
            i4_rc=db_load_from_file(
                s_name,
                ui1_fld_count,
                pt_rec_fields,
                ui1_key_count,
                pui4_keyfld_type,
                h_file,
                /* Endian conversion function. */
                (db_endian_conv_fct)nwl_data_conv_from_big_end,
                /* Common data endian conversion function. */
                (db_cdata_conv_fct)nwl_common_data_endconv_from_file,
                (VOID*)(&pt_nwl),
                nwl_nfy_fct,
                &h_cdb);
            
            /* translate the CDB error code to NWL error code */
            i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
            if ( i4_rc == NWLR_OK )
            {

                DBG_API(("{NWL} NWL's cdb object loaded.\n"));

                /*
                  Allocate a handle for the NWL object.
                */
                if ( handle_alloc(NWLT_NETWORK_LIST,
                                  (VOID*)pt_nwl,
                                  pv_tag,
                                  nwl_handle_free_fct,
                                  ph_nwl)
                     == HR_OK )
                {
                    /* initialize the NWL object. */
                    pt_nwl->h_nwl=*ph_nwl;
                    pt_nwl->h_cdb=h_cdb;
                    pt_nwl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                      caller's notification
                                                      function. */
                    pt_nwl->ui2_nwl_id=ui2_nwl_id;
                    x_strncpy(pt_nwl->ps_nwl_name,ps_nwl_name,
                              MAX_NWL_NAME_LEN);

                    /* Set the initial search parameter and cursor position
                       to unknown initial values
                       This will force sorted list to be created and search
                       to start from the first element of array.
                    */
                    x_memset(&(pt_nwl->t_parm), NWL_UNSET_VAL,
                         sizeof(NWL_SEARCH_PARM_T));

                    x_memset(&(pt_nwl->t_cursor), NWL_UNSET_VAL,
                         sizeof(NWL_CURSOR_T));

#if 0
                    /* right now, we don't have any customize sort/compare
                       function associated with NWL handle object.
                    */
                    x_memset(&(pt_nwl->t_cmp_fct_info), 0,
                         sizeof(NWL_CMP_FCT_INFO_T));
#endif                    
                
                    i4_rc=NWLR_OK;

                    DBG_API(("{NWL} NWL object created from file\n"));
                }
                else
                {
                    i4_rc=NWLR_OUT_OF_HANDLE;
                }
            }
        }
        else
        {
            i4_rc = NWLR_OUT_OF_MEM ;
        }

        if ( i4_rc != NWLR_OK )
        {
            /* Remove the id name pair. */
            nwl_del_id(ui2_nwl_id);
        }
    }
    else
    {
        i4_rc = NWLR_FAIL;
    }
    nwl_sema_unlock();
    
    if ( i4_rc != NWLR_OK )
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,s_name);
        }
        if ( pt_nwl != NULL )
        {
            x_mem_free(pt_nwl);
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: nwl_sync_data_with_file
 *
 * Description:
 *    This API create synchronize NWL object by loading the paramters
 *    and records from persistent storage. The existing data are over-
 *    written with data from the specified file.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   NWLR_OK             NWL library is initialized successfully.
 *   NWLR_OUT_OF_MEM     Not enough memory to create the NWL object.
 *   NWLR_INV_ARG        'ui2_max_num_nwl_obj' is 0.
 *   NWLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 nwl_sync_data_with_file
(
    HANDLE_T         h_nwl,
    HANDLE_T         h_file
)
{
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    INT32              i4_rc=NWLR_FAIL;
    NWL_OBJ_T*         pt_nwl=NULL;
    HANDLE_TYPE_T      pe_type;

    if ( (handle_get_type_obj(h_nwl, &pe_type,((VOID**) &pt_nwl)) == HR_OK) &&
         (pe_type == NWLT_NETWORK_LIST)                                     &&
         (pt_nwl  != NULL)
        )
    {
        /* get the record definition. */
        nwl_db_rec_and_key_def(&ui1_fld_count,
                               &pt_rec_fields,
                               &pui4_recfld_type,
                               &ui1_key_count,
                               &pui4_keyfld_type);
    
        /* Load the data from the file into CDB database object. */
        if ( pt_nwl != NULL )
        {
            i4_rc=db_sync_data_with_file(pt_nwl->h_cdb,
                                         ui1_fld_count,
                                         pt_rec_fields,
                                         ui1_key_count,
                                         pui4_keyfld_type,
                                         h_file,
                                         /* Endian conversion function. */
                                         (db_endian_conv_fct)
                                         nwl_data_conv_from_big_end,
                                         /* Common data endian conversion
                                            function. */
                                         (db_cdata_conv_fct)
                                         nwl_common_data_endconv_from_file
                );
            
            /* translate the CDB error code to NWL error code */
            i4_rc=nwl_cdb_to_nwl_err_code(i4_rc);
            if ( i4_rc == NWLR_OK )
            {
                DBG_API(("{NWL} NWL records are sync'd with file\n"));
            }
        }
        else
        {
            i4_rc = NWLR_INV_ARG ;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_nwl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = NWLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_persistent_size
 *
 * Description: This API returns the space needed to store
 *              a NWL object on the file system.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   size (in bytes) for storing the NWL object to file. The size includes
 *   the header (magic number, etc), metadata, and the CDB object which
 *   contains NWL records.
 *   0 if the NWL object size can not be determine.
 *   
 -----------------------------------------------------------------------*/
UINT32 nwl_persistent_size
(
    NWL_OBJ_T*      pt_nwl
)
{
    UINT32          ui4_sz;
    INT32           i4_rc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_nwl->h_cdb,
                                 (db_endian_conv_fct)nwl_data_conv_to_big_end,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_sz = NWL_FILE_HEADER_SIZE + NWL_FILE_METADATA_SIZE +
            ui4_persistent_size + NWL_FILE_TAIL_MARKER_SIZE ;
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
 * Name: nwl_is_write_to_file_needed
 *
 * Description: This API compare the NWL object stored on filesystem
 *              with NWL object in memory. If they contains different
 *              data, then save (e.g., write operation) will be needed.
 *
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs:
 *   pb_rc:     TRUE  NWL object in memory and NWL object in filesystem
 *                    contains same data, no write operation is needed.
 *              FALSE NWL object in memory and NWL object in filesystem
 *                    contains different data, write operation is needed.
 *
 * Returns: 
 *   NWLR_OK:      
 *   NWLR_FILE_READ_FAIL:   Can not read the NWL object on filesystme.
 *
 -----------------------------------------------------------------------*/
INT32 nwl_is_write_to_file_needed
(
    HANDLE_T        h_file,
    NWL_OBJ_T*      pt_nwl,
    BOOL*           pb_rc
)
{
    UINT64          ui8_fpos;
    UINT64          ui8_end_fpos;
    INT32           i4_rc;
    NWL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    BOOL            b_result;
    INT32           i4_res;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    i4_rc  = NWLR_OK;
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

        /* Read meta data of the NWL object. */
        if ( nwl_read_file_info(h_file, &t_finfo, &ui4_crc) == NWLR_OK )
        {
            /*
              Compare the NWL file header and metadata information
              with the NWL object in memory to see if they contain
              different data.
            */
            if ( t_finfo.ui1_nwl_ver != NWL_FILE_VER )
            {
                DBG_INFO(("{NWL} [nwl_is_write_to_file_needed] nwl_ver diff\n"));
                
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_nwl_id != pt_nwl->ui2_nwl_id )
            {
                DBG_INFO(("{NWL} [nwl_is_write_to_file_needed] nwl_id diff\n"));
                *pb_rc = TRUE;
            }
            else if ( x_strcmp(t_finfo.ps_nwl_name, pt_nwl->ps_nwl_name) != 0 )
            {
                DBG_INFO(("{NWL} [nwl_is_write_to_file_needed] nwl_name diff\n"));
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_meta_data_sz != NWL_FILE_METADATA_SIZE )
            {
                DBG_INFO(("{NWL} [nwl_is_write_to_file_needed] nwl_metadata size diff\n"));
                *pb_rc = TRUE;
            }
            else
            {
                /*
                  Check the data size between memory and size of data
                  stored on file.
                */
                if ( db_get_persistent_size(pt_nwl->h_cdb,
                                            (db_endian_conv_fct)nwl_data_conv_to_big_end,
                                            &ui4_persistent_size,
                                            &ui4_ver_id)
                     == DBR_OK )
                {

                    DBG_INFO(("{NWL} size of NWL obj on file: [%d bytes], size of NWL data in memory: [%d bytes]\n",
                               ui4_persistent_size, t_finfo.ui4_data_size));                   
                    
                    if ( ui4_persistent_size != t_finfo.ui4_data_size )
                    {
                         DBG_INFO(("{NWL} data sect size difference between NWL obj in memory [%d bytes] and on filesystem [%d bytes]\n", ui4_persistent_size , t_finfo.ui4_data_size ));
                         
                        *pb_rc = TRUE;
                       
                    }
                    /*
                      check if writing to CDB object is needed.
                    */
                    else if ( db_is_save_to_file_needed(
                                  pt_nwl->h_cdb, h_file,
                                  (db_endian_conv_fct)nwl_data_conv_to_big_end,
                                  NULL,
                                  &b_result) == DBR_OK
                        )
                    {
                        if ( b_result == TRUE )
                        {
                            DBG_API(("{NWL} writing CDB data to file system is needed.\n"));
                            *pb_rc = TRUE;
                        }
                    }
                    else
                    {
                        i4_rc = NWLR_FILE_READ_FAIL;
                    }
                }
                else
                {
                    i4_rc= NWLR_FILE_READ_FAIL;
                }
            }
        }
        else
        {
            DBG_API(("{NWL} can not read metadata info from nwl object on the file system,"
                     " enable the write flag to write over the nwl object in file. \n"));

            *pb_rc = TRUE;
            
        }
        
        /*
          Reset the file position.
        */
        if ( x_fm_lseek(h_file, (INT64) ui8_fpos,
                        FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
            )
        {
            i4_rc = NWLR_OK;
        }
        else
        {
            i4_rc = NWLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = NWLR_FILE_READ_FAIL;
        DBG_ERROR(("{NWL} [nwl_is_write_to_file_needed] x_fm_lseek error: %d\n",i4_res));
    }
    
    /* remove warning. */
    i4_res=(i4_res+1);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_sync_object
 *
 * Description:
 *   This API sync an existing NWL object with records
 *   from a persistent NWL object stored on file system.
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *   NWLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   NWLR_INV_ARG      'pt_nwl_rec' is null or the NWL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_sync_object
(
    HANDLE_T		h_nwl,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_nwl_id
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    NWL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[NWL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the NWL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=nwl_fs_raw_get_read_hdle(h_dir,
                                   ps_nwl_path,
                                   ui2_nwl_id,
                                   &h_file);
    if ( i4_rc == NWLR_OK )
    {
        /* Read meta data of the NWL object. */
        if ( nwl_read_file_info(h_file, &t_finfo, &ui4_crc) == NWLR_OK )
        {
            /* Assertiogn check, check the nwl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_nwl_id == ui2_nwl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  NWL file object loaded.

                  Sync NWL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=nwl_sync_data_with_file(h_nwl, h_file);
                if ( i4_rc == NWLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, NWL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == NWL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = NWLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{NWL} Read Error: fail CRC32 check.\n");
                            i4_rc = NWLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = NWLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != NWLR_OK )
                    {
                        /* clean up. */
                        x_nwl_delete(h_nwl);
                    }
                }
            }
            else
            {
                i4_rc = NWLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = NWLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        nwl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_sync_object_given_file_handle
 *
 * Description:
 *   This API sync an existing NWL object with records
 *   from a persistent NWL object stored on file system.
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *   NWLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   NWLR_INV_ARG      'pt_nwl_rec' is null or the NWL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_sync_object_given_file_handle
(
    HANDLE_T		h_nwl,	
    HANDLE_T		h_file,	
    UINT16		    ui2_nwl_id
)
{
    INT32           i4_rc;
    NWL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[NWL_FILE_TAIL_MARKER_SIZE];

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the NWL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=nwl_fs_raw_get_read_hdle_no_fm_open(
                                   ui2_nwl_id,
                                   &h_file);
    if ( i4_rc == NWLR_OK )
    {
        /* Read meta data of the NWL object. */
        if ( nwl_read_file_info(h_file, &t_finfo, &ui4_crc) == NWLR_OK )
        {
            /* Assertiogn check, check the nwl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_nwl_id == ui2_nwl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  NWL file object loaded.

                  Sync NWL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=nwl_sync_data_with_file(h_nwl, h_file);
                if ( i4_rc == NWLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, NWL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == NWL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = NWLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{NWL} Read Error: fail CRC32 check.\n");
                            i4_rc = NWLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = NWLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != NWLR_OK )
                    {
                        /* clean up. */
                        x_nwl_delete(h_nwl);
                    }
                }
            }
            else
            {
                i4_rc = NWLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = NWLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        nwl_fs_raw_close_hdle_no_fm_close(h_file);
    }
    return i4_rc;
}
