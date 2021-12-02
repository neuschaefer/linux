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
 * $RCSfile: satl_fs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of internal SATL file system related
 *    API's.  These API will be called by the exported SATL file system
 *    API: x_satl_fs_load, x_satl_fs_store, x_satl_fs_del, x_satl_fs_qry.
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
#include "satl/satl.h"
#include "satl/_satl.h"
#include "satl/satl_rec.h"
#include "file_mngr/x_fm.h"
#include "satl/satl_fs.h"
#include "satl/satl_fs_raw.h"
#include "satl/satl_fs_union.h"
#include "satl/satl_dbg.h"
/*-------------------------------------------------------------------
                   static  functions declaration
 --------------------------------------------------------------------*/
static INT32 satl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
);

static INT32 satl_write_file_header
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
);

static INT32 satl_write_metadata
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
    UINT32*            pui4_crc
);

static VOID satl_common_data_endconv_to_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
);

static INT32 satl_write_data
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
    UINT32*            pui4_crc
);

static INT32 satl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_satl_name,
    UINT16           ui2_satl_id,   
    VOID*            pv_tag,
    x_satl_nfy_fct   pf_nfy_fct,
    HANDLE_T*        ph_satl
);

static INT32 satl_sync_data_with_file
(
    HANDLE_T         h_satl,
    HANDLE_T         h_file
);

/*------------------------------------------------------------------
                    functions implementation
 -------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Name: satl_fs_del_object
 *
 * Description: This API deletes the specified SATL object 
 *              in the file system.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Specified SATL object is deleted.
 *   SATLR_INV_HANDLE  Bad handle.
 *   SATLR_NOT_FOUND   Specified SATL object is not found.
 -----------------------------------------------------------------------*/
INT32 satl_fs_del_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16          ui2_satl_id
)
{
    INT32           i4_rc;
    i4_rc=satl_fs_raw_del(h_dir, ps_satl_path, ui2_satl_id);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_qry_persistent_objects
 *
 * Description: This API queries the directory or RAW file for
 *              SATL object in the persistent storage.
 *
 * Inputs: 
 *   ps_satl_path:  Path to RAW file.
 *   ui2_idx:       Iteration index to the SATL object.
 *
 * Outputs:
 *   pui4_satl_id:  The SATL object id found.
 *   ps_satl_name:  The SATL object name found.
 *
 * Returns: 
 *   SATLR_OK          All records are successfully removed. 
 *   SATLR_INV_HANDLE  Bad handle.
 -----------------------------------------------------------------------*/
INT32 satl_qry_persistent_objects
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_satl_id,
    CHAR*           ps_satl_name
)
{
    INT32           i4_rc;

    /*
       Access the RAW file to obtain the SATL object info.
    */
    i4_rc=satl_fs_raw_qry(h_dir,
                          ps_satl_path,
                          ui2_idx,
                          pui2_satl_id,
                          ps_satl_name);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_fs_load_object
 *
 * Description: This load a SATL object from a regular file from a 
 *              specified directory path or from a RAW file.
 *              
 *
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *   pt_satl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Record is successfully added. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *   SATLR_OUT_OF_MEM  Can not allocate memory, add record failed,.
 *   SATLR_INV_ARG     'pt_satl_rec' is null or the SATL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 satl_fs_load_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_satl_id,
    const CHAR*	    ps_satl_name,
    VOID*			pv_tag,
    x_satl_nfy_fct	pf_nfy,
    HANDLE_T*		ph_satl
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    SATL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SATL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SATL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=satl_fs_raw_get_read_hdle(h_dir,
                                    ps_satl_path,
                                    ui2_satl_id,
                                    &h_file);
    if ( i4_rc == SATLR_OK )
    {
        /* Read meta data of the SATL object. */
        if ( satl_read_file_info(h_file, &t_finfo, &ui4_crc) == SATLR_OK )
        {
            /* Assertiogn check, check the satl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_satl_id == ui2_satl_id )
            {
                /*
                  If the user did not provide a SATL name,
                  then use the SATL name stored in the
                  persistent storage.
                */
                if ( ps_satl_name == NULL )
                {
                    ps_satl_name = t_finfo.ps_satl_name;
                }

                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SATL file object loaded.

                  Create SATL handle from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=satl_create_handle_from_file(h_file,
                                                   ps_satl_name,
                                                   ui2_satl_id,
                                                   pv_tag,
                                                   pf_nfy,
                                                   ph_satl);
                if ( i4_rc == SATLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SATL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SATL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SATLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SATL} Read Error: fail CRC32 check.\n");
                            i4_rc = SATLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SATLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SATLR_OK )
                    {
                        /* clean up. */
                        x_satl_delete(*ph_satl);
                    }
                }
            }
            else
            {
                i4_rc = SATLR_FILE_READ_FAIL;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
        else
        {
            i4_rc = SATLR_FILE_READ_FAIL;
            /*
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
            */
        }
        /*
          close the file handle.
        */
        satl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  satl_fs_store_object
 *
 * Description: This function write an SATL object to a regular file or
 *              RAW file.
 *
 * Inputs: 
 *   h_satl          Handle to the SATL object.
 *   ui2_satl_rec_id Specifies the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK           Specified record is deleted. 
 *   SATLR_INV_HANDLE   Bad SATL handle.
 *   SATLR_INV_ARG      ui2_satl_rec_id is a NULL record id.
 *   SATLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the SATL object.
 *   
 -----------------------------------------------------------------------*/
INT32 satl_fs_store_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_satl_id   
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_SATL_NAME_LEN+1+16];
    HANDLE_T        h_file;
    HANDLE_T        h_satl;
    SATL_OBJ_T*     pt_satl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
    
    /*
       Get a SATL handle to the SATL object.
    */
    if ( (i4_rc=x_satl_open(ui2_satl_id, NULL, NULL, &h_satl)) == SATLR_OK )
    {
        i4_rc = SATLR_FILE_WRITE_FAIL; 
        /*
          Get the SATL control block object.
        */
        if ( (handle_get_type_obj(h_satl, &pe_type,((VOID**) &pt_satl))
              ==HR_OK)                                                &&
             (pe_type == SATLT_SATELLITE_LIST)                 &&
             (pt_satl != NULL)
            )
        {
            /*
              Lock the SATL object.
            */
            if ( satl_lock_object_for_read(pt_satl) == SATLR_OK )
            {
                db_clear_data_sect_sz_info(pt_satl->h_cdb);
                
                /*
                  Get the internal SATL name used for the storage
                  from the SATL id and SATL name.
                */
                satl_get_database_name(pt_satl->ps_satl_name,
                                       pt_satl->ui2_satl_id,
                                       ps_fname);
                /*
                  Open the RAW file for writing the SATL object.
                  Get the offset to location of the file where SATL object
                  will be written to.
                */
                if ( (i4_rc=satl_fs_raw_get_write_hdle(h_dir, ps_satl_path,
                                                       pt_satl, &h_file,
                                                       &ui4_beg_magic,
                                                       &b_new_pos))
                     == SATLR_OK
                    )
                {

                    DBG_API(("{SATL} start writing SATL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing SATL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=satl_is_write_to_file_needed(h_file,pt_satl,&b_write_flag))
                              == SATLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_API(("{SATL} no change in data, write ops not needed.\n"));
                            }
                            else
                            {
#ifdef DEBUG                                
                                x_dbg_stmt("{SATL} write data at existing persistent storage location\n");
#endif
                            }
                        }
                    }
                    else
                    {
                        /*
                          Writing SATL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
#ifdef DEBUG
                        x_dbg_stmt("{SATL} write data at new persistent storage location.\n");
#endif                        
                    }

                    if ( i4_rc == SATLR_OK     &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc = SATLR_FILE_WRITE_FAIL; 
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                    
                        if ( satl_write_file_header(h_file, pt_satl,
                                                    ui4_beg_magic, &ui4_crc) == SATLR_OK )
                        {
                            if ( satl_write_metadata(h_file, pt_satl, &ui4_crc) == SATLR_OK )
                            {
                                if ( satl_write_data(h_file, pt_satl, &ui4_crc) == SATLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);

                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        satl_write_obj_end_magic_crc(h_file,
                                                                     ui4_end_magic,
                                                                     ui4_crc) ;
                                }
                            }   
                        }
                    }
                    
                    /*
                      Don't forget to close the file handle.
                    */
                    if ( i4_rc == SATLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation. */
                        i4_rc = satl_fs_raw_close_hdle(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        satl_fs_raw_close_hdle(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_satl->h_cdb);
                }
                /*
                  don't forget to unlock the SATL
                */
                satl_unlock_object_for_read(pt_satl);
                
            }   /* Can not lock the SATL object. */
            
        }   /* Can not get the SATL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_satl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }
        
        /*
          Finish, don't forget to close the handle to the SATL object.
        */
        x_satl_close(h_satl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  satl_fs_store_object_given_file_handle
 *
 * Description: This function write an SATL object to a regular file or
 *              RAW file given the file handle..
 *
 * Inputs: 
 *   h_file          Handle to the file to write.
 *   ui2_satl_id      Specifies the SATL list ID to be stored. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK            Specified record is deleted. 
 *   SATLR_INV_HANDLE    Bad file handle.
 *   SATLR_NOT_FOUND     The specified SATL database object id (e.g. 
 *                      'ui2_satl_id') is not found.
 *   
 -----------------------------------------------------------------------*/
INT32 satl_fs_store_object_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_satl_id
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_SATL_NAME_LEN+1+16];
    HANDLE_T        h_satl;
    SATL_OBJ_T*      pt_satl;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a SATL handle to the SATL object.
    */
    if ( (i4_rc = x_satl_open(ui2_satl_id, NULL, NULL, &h_satl)) == SATLR_OK )
    {
        i4_rc= SATLR_FILE_WRITE_FAIL;
        /*
          Get the SATL control block object.
        */
        if ( (handle_get_type_obj(h_satl, &pe_type,((VOID**) &pt_satl))
              ==HR_OK)                                                &&
             (pe_type == SATLT_SATELLITE_LIST)                        &&
             (pt_satl  != NULL)
            )
        {
            /*
              Lock the SATL object.
            */
            if ( satl_lock_object_for_read(pt_satl) == SATLR_OK )
            {
                db_clear_data_sect_sz_info(pt_satl->h_cdb);
                
                /*
                  Get the internal SATL name used for the storage
                  from the SATL id and SATL name.
                */
                satl_get_database_name(pt_satl->ps_satl_name,
                                       pt_satl->ui2_satl_id,
                                       ps_fname);
                /*
                   Open the RAW file for writing SATL object.
                   Get the offset to location of the file where SATL object
                   will be written to.
                */
                if ( (i4_rc=satl_fs_raw_get_write_hdle_no_fm_open(
                                                      pt_satl, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == SATLR_OK
                    )
                {

                    DBG_API(("{SATL} start writing SATL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing SATL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=satl_is_write_to_file_needed(h_file,pt_satl,&b_write_flag))
                              == SATLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_INFO(("{SATL} [satl_fs_store_object] no change in data, write ops not needed.\n"));

                                /* x_dbg_stmt("{SATL} [satl_fs_store_object] no change in data, write ops not needed.\n"); */
                            }
                            else
                            {
                                DBG_INFO(("{SATL} [satl_fs_store_object] data changed, write ops is required.\n"));

                                /* x_dbg_stmt("{SATL} [satl_fs_store_object] data changed, write ops is required.\n"); */
                                
                            }
                            
                        }
                    }
                    else
                    {
                        /*
                          Writing SATL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == SATLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= SATLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( satl_write_file_header(h_file, pt_satl,
                                                    ui4_beg_magic, &ui4_crc) == SATLR_OK )
                        { 
                            if ( satl_write_metadata(h_file, pt_satl, &ui4_crc) == SATLR_OK )
                            {
                                if ( satl_write_data(h_file, pt_satl, &ui4_crc) == SATLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        satl_write_obj_end_magic_crc(h_file,
                                                                     ui4_end_magic,
                                                                     ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == SATLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = satl_fs_raw_close_hdle_no_fm_close(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        satl_fs_raw_close_hdle_no_fm_close(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_satl->h_cdb);
                } 
                
                /*
                  don't forget to unlock the SATL
                */
                satl_unlock_object_for_read(pt_satl);
                
            }  /* Can not lock the SATL object. */
            
        }   /* Can not get the SATL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_satl))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to SATL object. */
        x_satl_close(h_satl);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_write_obj_end_magic_crc
 *
 * Description: This API write the SATL file magic number and CRC values
 *                
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_satl     Pointer to an SATL handle object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK    Success.
 *
 -----------------------------------------------------------------------*/
static INT32 satl_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc	
)
{
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[SATL_FILE_TAIL_MARKER_SIZE] ;
    
    /* convert to Big-Endian. */
    PUT_UINT32_TO_PTR_BIG_END(ui4_crc, &(aui1_buf[0]) );

    
    x_end_cnvt_to_big_32(&ui4_end_magic, 1, &(aui1_buf[4]) );
    
    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    
    if ( i4_rc == FMR_OK                       &&
         ( sizeof(aui1_buf) == ui4_byte_wrote )
        )
    {
        /* Number of bytes written should equals number
           of bytes in the buffer.
        */
        i4_rc=SATLR_OK;

        DBG_API(("{SATL} [satl_write_obj_end_magic_crc]: SATL file end_magic_crc to filesystem.\n"));
    }
    else
    {
        i4_rc = SATLR_FILE_WRITE_FAIL;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_write_file_header
 *
 * Description: This API write the SATL file header. The file header
 *              consists of :
 *                    magic number (4 bytes)
 *                    file_version_number (1 byte)
 *                    metadata_size  (2 bytes)
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_satl     Pointer to an SATL handle object.
 *   ui4_beg_magic The beginning magic value.
 *   pui4_crc    Input CRC 32 value.
 * 
 * Outputs:
 *   pui4_crc:   CRC computed over the file head data.
 *
 * Returns: 
 *   SATLR_OK    Success.
 *
 -----------------------------------------------------------------------*/
static INT32 satl_write_file_header
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
)
{
    UINT8   ui1_satl_ver;
    UINT16  ui2_meta_data_sz;
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[SATL_FILE_HEADER_SIZE] ;
    UINT32  ui4_crc;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_beg_magic, 1, &(aui1_buf[0]) );
    
    /* The current SATL library version number. */
    ui1_satl_ver=SATL_FILE_VER;
    x_memcpy(&(aui1_buf[4]), &ui1_satl_ver, 1);

    /* The metadata size. */
    ui2_meta_data_sz=SATL_FILE_METADATA_SIZE;
    
    x_end_cnvt_to_big_16(&ui2_meta_data_sz, 1, &(aui1_buf[5]) );

    i4_rc=x_fm_write(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_wrote);
    
    if ( i4_rc == FMR_OK                       &&
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
            ui4_crc = x_crc32_gen(aui1_buf, SATL_FILE_HEADER_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{SATL} [satl_write_file_header]: SATL file header wrote to filesystem.\n"));
        i4_rc=SATLR_OK;
    }
    else
    {
        i4_rc = SATLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_write_metadata
 *
 * Description: This API write SATL metadata section to persistent storage.  
 *              The metadata consists of:
 *                     ui2_satl_id (2 bytes)
 *                     ps_satl_name (17 bytes)
 *                     ui4_data_size (4 bytes)
 *
 * Inputs: 
 *   h_satl      Handle to the persistent storage.
 *   pt_satl     Pointer to an SATL handle object.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 *
 * Outputs: 
 *   pui4_crc    CRC32 value computed over the SATL metadata section.
 *
 * Returns: 
 *   SATLR_OK       success
 *   
 *
 -----------------------------------------------------------------------*/
static INT32 satl_write_metadata
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_data_sz;
    UINT8           aui1_buf[SATL_FILE_METADATA_SIZE] ;
    UINT32          ui4_byte_wrote;
    UINT32          ui4_crc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;
        
    /* SATL id number. */
    x_end_cnvt_to_big_16(&(pt_satl->ui2_satl_id), 1, &(aui1_buf[0]));

    /* SATL name. */
    x_memcpy(&(aui1_buf[2]), pt_satl->ps_satl_name, MAX_SATL_NAME_LEN+1);

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_satl->h_cdb,
                                 (db_endian_conv_fct)satl_union_end_conv_to_file,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_data_sz = ui4_persistent_size;
        x_end_cnvt_to_big_32(&ui4_data_sz, 1, &(aui1_buf[19]));
    }
    else
    {
        return SATLR_FILE_WRITE_FAIL;
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
            ui4_crc = x_crc32_gen(aui1_buf, SATL_FILE_METADATA_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }
        
        DBG_API(("{SATL} [satl_write_metadata]: SATL file metadata wrote to filesystem.\n"));        
        
        i4_rc=SATLR_OK;
    }
    else
    {
        i4_rc = SATLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: satl_common_data_endconv_to_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored in CDB by the SATL library prior to writing out to
 *              the persistent storage.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK         SATL database is unlocked. 
 *   SATLR_INV_HANDLE Bad SATL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID satl_common_data_endconv_to_file
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
 * Name: satl_common_data_endconv_from_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored on persistent storage before return to result to
 *              structure in memory.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_satl	Handle to the SATL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK         SATL database is unlocked. 
 *   SATLR_INV_HANDLE Bad SATL handle.
 *   
 -----------------------------------------------------------------------*/
VOID satl_common_data_endconv_from_file
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
 * Name: satl_write_data
 *
 * Description: This function write out the data section of the SATL object.   
 *
 * Inputs: 
 *   pt_satl	Handle to the SATL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL is locked. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *
 -----------------------------------------------------------------------*/
static INT32 satl_write_data
(
    HANDLE_T           h_file,
    SATL_OBJ_T*        pt_satl,
    UINT32*            pui4_crc
) 
{
    INT32           i4_rc;

    i4_rc=db_save_to_file(pt_satl->h_cdb, h_file,
                          (db_endian_conv_fct)satl_union_end_conv_to_file,
                          satl_common_data_endconv_to_file);

    i4_rc = satl_cdb_to_satl_err_code(i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_read_file_info
 *
 * Description: This API read the SATL file header section and meta data 
 *              section from a specified FILE handle.
 * Inputs: 
 *   h_file:  SATL file handle.
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          SATL library is initialized successfully.
 *   
 -----------------------------------------------------------------------*/
INT32 satl_read_file_info
(
    HANDLE_T           h_file,
    SATL_FILE_INFO_T*  pt_finfo,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SATL_FILE_HEADER_SIZE+SATL_FILE_METADATA_SIZE];
    UINT32          ui4_crc;

    
    i4_rc=x_fm_read(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_read);
    if ( i4_rc == FMR_OK                     &&
        (SATL_FILE_HEADER_SIZE+SATL_FILE_METADATA_SIZE) == ui4_byte_read
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
        
        if ( pt_finfo->ui4_magic == SATL_MAGIC_NUMBER       ||
             pt_finfo->ui4_magic == (~SATL_MAGIC_NUMBER)
            )
        {
            /*
              Get the version number.
            */
            x_memcpy(&(pt_finfo->ui1_satl_ver), &(aui1_buf[4]), 1);

            /*
              Check the SATL file version number.
            */
            if ( pt_finfo->ui1_satl_ver == SATL_FILE_VER )
            {
                /*
                   Get the size of the metadata section.
                */
                x_end_cnvt_from_big_16(&(aui1_buf[5]),1,&(pt_finfo->ui2_meta_data_sz));

                /*
                  Get the satl id
                */
                x_end_cnvt_from_big_16(&(aui1_buf[7]),1,&(pt_finfo->ui2_satl_id));

                /*
                  Get the satl name.
                */
                x_memcpy(pt_finfo->ps_satl_name,&(aui1_buf[9]),MAX_SATL_NAME_LEN+1);

                /*
                  Get the data section size.
                */
                x_end_cnvt_from_big_32(&(aui1_buf[9+MAX_SATL_NAME_LEN+1]),
                                       1,&(pt_finfo->ui4_data_size));

                i4_rc=SATLR_OK;
            }
            else
            {
                DBG_API(("{SATL} Wrong SATL file version encountered: current software ver: %d  file ver %d.\n",
                        SATL_FILE_VER , pt_finfo->ui1_satl_ver ));
                
                i4_rc = SATLR_INV_FILE_VER;
            }
        }
        else
        {
            /*
              Wrong magic number.
            */
            i4_rc=SATLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = SATLR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_create_handle_from_file
 *
 * Description: This API create an SATL object by loading the paramters
 *              and records from persistent storage.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK            SATL library is initialized successfully.
 *   SATLR_OUT_OF_MEM    Not enough memory to create the SATL object.
 *   SATLR_INV_ARG       'ui2_max_num_satl_obj' is 0.
 *   SATLR_OUT_OF_HANDLE No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 satl_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_satl_name,
    UINT16           ui2_satl_id,   
    VOID*            pv_tag,
    x_satl_nfy_fct   pf_nfy_fct,
    HANDLE_T*        ph_satl
)
{
    CHAR               s_name[CDB_MAX_DATABASE_NAME_LEN+1];
    SATL_OBJ_T*        pt_satl=NULL;
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    HANDLE_T           h_cdb=NULL_HANDLE;
    INT32              i4_rc=SATLR_FAIL;
    
    /* create CDB name. */
    satl_get_database_name(ps_satl_name,ui2_satl_id,s_name);

    /* get the record definition. */
    satl_db_rec_and_key_def(&ui1_fld_count,
                            &pt_rec_fields,
                            &pui4_recfld_type,
                            &ui1_key_count,
                            &pui4_keyfld_type);
    
    satl_sema_lock();

    /* check if the input SATL name and id has not been used.
       then, add the name and id to the internal list. */

    if ( satl_check_id_name(ui2_satl_id,ps_satl_name) == TRUE &&
         satl_add_id_name(ui2_satl_id,ps_satl_name) == SATLR_OK
        )
    {
        /* Create a SATL object. */
        pt_satl=(SATL_OBJ_T*) x_mem_alloc(sizeof(SATL_OBJ_T));

        /* Create a core database object. */
        if ( pt_satl != NULL )
        {
            i4_rc=db_load_from_file(
                s_name,
                ui1_fld_count,
                pt_rec_fields,
                ui1_key_count,
                pui4_keyfld_type,
                h_file,
                /* Endian conversion function. */
                (db_endian_conv_fct)satl_union_end_conv_from_file,
                /* Common data endian conversion function. */
                (db_cdata_conv_fct)satl_common_data_endconv_from_file,
                (VOID*)(&pt_satl),
                satl_nfy_fct,
                &h_cdb);
            
            /* translate the CDB error code to SATL error code */
            i4_rc=satl_cdb_to_satl_err_code(i4_rc);
            if ( i4_rc == SATLR_OK )
            {
                /*
                  Allocate a handle for the SATL object.
                */
                if ( handle_alloc(SATLT_SATELLITE_LIST,
                                  (VOID*)pt_satl,
                                  pv_tag,
                                  satl_handle_free_fct,
                                  ph_satl)
                     == HR_OK )
                {
                    /* initialize the SATL object. */
                    pt_satl->h_satl=*ph_satl;
                    pt_satl->h_cdb=h_cdb;
                    pt_satl->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                      caller's notification
                                                      function. */
                    pt_satl->ui2_satl_id=ui2_satl_id;
                    x_strncpy(pt_satl->ps_satl_name,ps_satl_name,MAX_SATL_NAME_LEN);
                    /* Set the initial search parameter and cursor position
                       to unknown initial values
                       This will force sorted list to be created and search
                       to start from the first element of array.
                    */
                    x_memset(&(pt_satl->t_parm), SATL_UNSET_VAL,
                         sizeof(SATL_SEARCH_PARM_T));

                    x_memset(&(pt_satl->t_cursor), SATL_UNSET_VAL,
                         sizeof(SATL_CURSOR_T));

                    /*
                    x_memset(&(pt_satl->t_cmp_fct_info), 0,
                         sizeof(SATL_CMP_FCT_INFO_T));
                    */
                    
                    i4_rc=SATLR_OK;
                }
                else
                {
                    i4_rc=SATLR_OUT_OF_HANDLE;
                }
            }
        }
        else
        {
            i4_rc = SATLR_OUT_OF_MEM ;
        }

        if ( i4_rc != SATLR_OK )
        {
            /* Remove the id name pair. */
            satl_del_id(ui2_satl_id);
        }
    }
    else
    {
        i4_rc = SATLR_FAIL;
    }
    satl_sema_unlock();
    
    if ( i4_rc != SATLR_OK )
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,s_name);
        }
        if ( pt_satl != NULL )
        {
            x_mem_free(pt_satl);
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: satl_sync_data_with_file
 *
 * Description:
 *    This API create synchronize SATL object by loading the paramters
 *    and records from persistent storage. The existing data are over-
 *    written with data from the specified file.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   SATLR_OK            SATL library is initialized successfully.
 *   SATLR_OUT_OF_MEM    Not enough memory to create the SATL object.
 *   SATLR_INV_ARG       'ui2_max_num_satl_obj' is 0.
 *   SATLR_OUT_OF_HANDLE No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 satl_sync_data_with_file
(
    HANDLE_T         h_satl,
    HANDLE_T         h_file
)
{
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    INT32              i4_rc=SATLR_FAIL;
    SATL_OBJ_T*        pt_satl=NULL;
    HANDLE_TYPE_T      pe_type;

    if ( (handle_get_type_obj(h_satl, &pe_type,((VOID**) &pt_satl)) == HR_OK) &&
         (pe_type == SATLT_SATELLITE_LIST)                           &&
         (pt_satl != NULL)
        )
    {
        /* get the record definition. */
        satl_db_rec_and_key_def(&ui1_fld_count,
                                &pt_rec_fields,
                                &pui4_recfld_type,
                                &ui1_key_count,
                                &pui4_keyfld_type);
    
        /* Load the data from the file into CDB database object. */
        if ( pt_satl != NULL )
        {
            i4_rc=db_sync_data_with_file(pt_satl->h_cdb,
                                         ui1_fld_count,
                                         pt_rec_fields,
                                         ui1_key_count,
                                         pui4_keyfld_type,
                                         h_file,
                                         /* Endian conversion function. */
                                         (db_endian_conv_fct)
                                         satl_union_end_conv_from_file,
                                         /* Common data endian conversion
                                            function. */
                                         (db_cdata_conv_fct)
                                         satl_common_data_endconv_from_file
                );
            
            /* translate the CDB error code to SATL error code */
            i4_rc=satl_cdb_to_satl_err_code(i4_rc);
            if ( i4_rc == SATLR_OK )
            {
                DBG_API(("{SATL} SATL records are sync'd with file\n"));
            }
        }
        else
        {
            i4_rc = SATLR_INV_ARG ;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_satl))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = SATLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_persistent_size
 *
 * Description: This API returns the space needed to store
 *              a SATL object on the file system.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   size (in bytes) for storing the SATL object to file. The size includes
 *   the header (magic number, etc), metadata, and the CDB object which
 *   contains SATL records.
 *   0 if the SATL object size can not be determine.
 *   
 -----------------------------------------------------------------------*/
UINT32 satl_persistent_size
(
    SATL_OBJ_T*     pt_satl
)
{
    UINT32          ui4_sz = 0;
    INT32           i4_rc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_satl->h_cdb,
                                 (db_endian_conv_fct)satl_union_end_conv_to_file,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_sz = SATL_FILE_HEADER_SIZE + SATL_FILE_METADATA_SIZE +
            ui4_persistent_size + SATL_FILE_TAIL_MARKER_SIZE;
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
 * Name: satl_is_write_to_file_needed
 *
 * Description: This API compare the SATL object stored on filesystem
 *              with SATL object in memory. If they contains different
 *              data, then save (e.g., write operation) will be needed.
 *
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *   pt_satl_rec Pointer to the service record structure to be added. 
 *
 * Outputs:
 *   pb_rc:     TRUE  SATL object in memory and SATL object in filesystem
 *                    contains same data, no write operation is needed.
 *              FALSE SATL object in memory and SATL object in filesystem
 *                    contains different data, write operation is needed.
 *
 * Returns: 
 *   SATLR_OK:     
 *   SATLR_FILE_READ_FAIL:  Can not read the SATL object on filesystme.
 *
 -----------------------------------------------------------------------*/
INT32 satl_is_write_to_file_needed
(
    HANDLE_T        h_file,
    SATL_OBJ_T*     pt_satl,
    BOOL*           pb_rc
)
{
    UINT64          ui8_fpos;
    UINT64          ui8_end_fpos;
    INT32           i4_rc;
    SATL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    BOOL            b_result;
    INT32           i4_res;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;
    
    i4_rc  = SATLR_OK;
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

        /* Read meta data of the SATL object. */
        if ( satl_read_file_info(h_file, &t_finfo, &ui4_crc) == SATLR_OK )
        {
            /*
              Compare the SATL file header and metadata information
              with the SATL object in memory to see if they contain
              different data.
            */
            if ( t_finfo.ui1_satl_ver != SATL_FILE_VER )
            {
                DBG_INFO(("{SATL} [satl_is_write_to_file_needed] satl_ver diff\n"));
                
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_satl_id != pt_satl-> ui2_satl_id )
            {
                DBG_INFO(("{SATL} [satl_is_write_to_file_needed] satl_id diff\n"));
                *pb_rc = TRUE;
            }
            else if ( x_strcmp(t_finfo.ps_satl_name, pt_satl->ps_satl_name) != 0 )
            {
                DBG_INFO(("{SATL} [satl_is_write_to_file_needed] satl_name diff\n"));
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_meta_data_sz != SATL_FILE_METADATA_SIZE )
            {
                DBG_INFO(("{SATL} [satl_is_write_to_file_needed] satl_metadata size diff\n"));
                *pb_rc = TRUE;
            }
            else
            {
                /*
                  Check the data size between memory and size of data
                  stored on file.
                */            
                if ( db_get_persistent_size(pt_satl->h_cdb,
                                            (db_endian_conv_fct)satl_union_end_conv_to_file,
                                            &ui4_persistent_size,
                                            &ui4_ver_id)
                     == DBR_OK )
                {
                    DBG_INFO(("{SATL} size of SATL obj on file: [%d bytes], size of SATL data in memory: [%d bytes]\n",
                               ui4_persistent_size, t_finfo.ui4_data_size));                   
                                
                    if ( ui4_persistent_size != t_finfo.ui4_data_size )
                    {
                        DBG_API(("{SATL} data section size difference between SATL obj in memory and on filesystem\n"));
                        *pb_rc = TRUE;
                    }
                    /*
                      check if writing to CDB object is needed.
                    */
                    else if ( db_is_save_to_file_needed(
                                  pt_satl->h_cdb, h_file,
                                  (db_endian_conv_fct)satl_union_end_conv_to_file,
                                  NULL,
                                  &b_result) == DBR_OK
                        )
                    {                           
                        if ( b_result == TRUE )
                        {
                            DBG_API(("{SATL} writing CDB data to file system is needed.\n"));
                            *pb_rc = TRUE;
                        }
                    }
                    else
                    {
                        i4_rc = SATLR_FILE_READ_FAIL;
                    }
                }
                else
                {
                    i4_rc= SATLR_FILE_READ_FAIL;
                }
            }
        }
        else
        {
            DBG_API(("{SATL} can not read metadata info from satl object on the file system,"
                     " enable the write flag to write over the satl object in file. \n"));
            *pb_rc = TRUE;
        }

        /*
          Reset the file position.
        */
        if ( x_fm_lseek(h_file, (INT64)ui8_fpos,
                        FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
            )
        {
            i4_rc = SATLR_OK;
        }
        else
        {
            i4_rc = SATLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = SATLR_FILE_READ_FAIL;
        DBG_ERROR(("{SATL} [satl_is_write_to_file_needed] x_fm_lseek error: %d\n",i4_res));        
    }
    
    /*
      remove compiler warning.
    */
    i4_res=(i4_res+1);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_fs_sync_object
 *
 * Description:
 *   This API sync an existing SATL object with records
 *   from a persistent SATL object stored on file system.
 *
 * Inputs: 
 *   h_satl     Handle to the SATL object.
 *   pt_satl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          Record is successfully added. 
 *   SATLR_INV_HANDLE  Bad SATL handle.
 *   SATLR_OUT_OF_MEM  Can not allocate memory, add record failed,.
 *   SATLR_INV_ARG     'pt_satl_rec' is null or the SATL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 satl_fs_sync_object
(
    HANDLE_T		h_satl,
    HANDLE_T        h_dir,
    const CHAR*	    ps_satl_path,
    UINT16		    ui2_satl_id
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    SATL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SATL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SATL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=satl_fs_raw_get_read_hdle(h_dir,
                                    ps_satl_path,
                                    ui2_satl_id,
                                    &h_file);
    if ( i4_rc == SATLR_OK )
    {
        /* Read meta data of the SATL object. */
        if ( satl_read_file_info(h_file, &t_finfo, &ui4_crc) == SATLR_OK )
        {
            /* Assertiogn check, check the satl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_satl_id == ui2_satl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SATL file object loaded.

                  Sync SATL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=satl_sync_data_with_file(h_satl, h_file);
                if ( i4_rc == SATLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SATL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SATL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SATLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SATL} Read Error: fail CRC32 check.\n");
                            i4_rc = SATLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SATLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SATLR_OK )
                    {
                        /* clean up. */
                        x_satl_delete(h_satl);
                    }
                }
            }
            else
            {
                i4_rc = SATLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = SATLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        satl_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: satl_fs_sync_object_given_file_handle
 *
 * Description:
 *   This API sync an existing SATL object with records
 *   from a persistent SATL object stored on file system.
 *
 * Inputs: 
 *   h_satl      Handle to the SATL object.
 *   pt_satl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK           Record is successfully added. 
 *   SATLR_INV_HANDLE   Bad SATL handle.
 *   SATLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   SATLR_INV_ARG      'pt_satl_rec' is null or the SATL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 satl_fs_sync_object_given_file_handle
(
    HANDLE_T		h_satl,	
    HANDLE_T		h_file,	
    UINT16		    ui2_satl_id
)
{
    INT32           i4_rc;
    SATL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[SATL_FILE_TAIL_MARKER_SIZE];

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the SATL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=satl_fs_raw_get_read_hdle_no_fm_open(
                                   ui2_satl_id,
                                   &h_file);
    if ( i4_rc == SATLR_OK )
    {
        /* Read meta data of the SATL object. */
        if ( satl_read_file_info(h_file, &t_finfo, &ui4_crc) == SATLR_OK )
        {
            /* Assertiogn check, check the satl_id is the same as
               specified.
            */
            if (  t_finfo.ui2_satl_id == ui2_satl_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  SATL file object loaded.

                  Sync SATL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=satl_sync_data_with_file(h_satl, h_file);
                if ( i4_rc == SATLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, SATL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == SATL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = SATLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{SATL} Read Error: fail CRC32 check.\n");
                            i4_rc = SATLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = SATLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != SATLR_OK )
                    {
                        /* clean up. */
                        x_satl_delete(h_satl);
                    }
                }
            }
            else
            {
                i4_rc = SATLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = SATLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        satl_fs_raw_close_hdle_no_fm_close(h_file);
    }
    return i4_rc;
}
