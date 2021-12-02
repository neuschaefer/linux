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
 * $RCSfile: lol_fs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file contains implementation of internal LOL file system related
 *    API's.  These API will be called by the exported LOL file system
 *    API: x_lol_fs_load, x_lol_fs_store, x_lol_fs_del, x_lol_fs_qry.
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

#include "lol/_lol.h"
#include "lol/lol_rec.h"
#include "file_mngr/x_fm.h"
#include "lol/lol_fs.h"
#include "lol/lol_fs_raw.h"
#include "lol/lol_dbg.h"
/*-------------------------------------------------------------------
                   static  functions declaration
 --------------------------------------------------------------------*/
static INT32 lol_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
);

static INT32 lol_write_file_header
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
);

static INT32 lol_write_metadata
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
    UINT32*            pui4_crc
);

static VOID lol_common_data_endconv_to_file
(
    const VOID*	pv_common_data,
    UINT16		ui2_size,
    VOID*  		pv_output
);

static INT32 lol_write_data
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
    UINT32*            pui4_crc
);

static INT32 lol_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_lol_name,
    UINT16           ui2_lol_id,    
    VOID*            pv_tag,
    x_lol_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_lol
);

static INT32 lol_sync_data_with_file
(
    HANDLE_T         h_lol,
    HANDLE_T         h_file
);

/*------------------------------------------------------------------
                    functions implementation
 -------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Name: lol_fs_del_object
 *
 * Description: This API deletes the specified LOL object 
 *              in the file system.
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Specified LOL object is deleted.
 *   LOLR_INV_HANDLE   Bad handle.
 *   LOLR_NOT_FOUND    Specified LOL object is not found.
 -----------------------------------------------------------------------*/
INT32 lol_fs_del_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_lol_path,
    UINT16          ui2_lol_id
)
{
    INT32           i4_rc;
    i4_rc=lol_fs_raw_del(h_dir,ps_lol_path, ui2_lol_id);
    return i4_rc;      
}

/*----------------------------------------------------------------------
 * Name: x_qry_persistent_objects
 *
 * Description: This API queries the directory or RAW file for
 *              LOL object in the persistent storage.
 *
 * Inputs: 
 *   ps_lol_path:   Path to RAW file.
 *   ui2_idx:       Iteration index to the LOL object.
 *
 * Outputs:
 *   pui4_lol_id:   The LOL object id found.
 *   ps_lol_name:   The LOL object name found.
 *
 * Returns: 
 *   LOLR_OK           All records are successfully removed. 
 *   LOLR_INV_HANDLE   Bad handle.
 -----------------------------------------------------------------------*/
INT32 lol_qry_persistent_objects
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_lol_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_lol_id,
    CHAR*           ps_lol_name	
)
{
    INT32           i4_rc;

    /*
       Access the RAW file to obtain the LOL object info.
    */
    i4_rc=lol_fs_raw_qry(h_dir,
                         ps_lol_path,
                         ui2_idx,
                         pui2_lol_id,
                         ps_lol_name);
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_fs_load_object
 *
 * Description: This load a LOL object from a regular file from a 
 *              specified directory path or from a RAW file.
 *              
 *
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Record is successfully added. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *   LOLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   LOLR_INV_ARG      'pt_lol_rec' is null or the LOL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 lol_fs_load_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_lol_path,
    UINT16		    ui2_lol_id,
    const CHAR*	    ps_lol_name,
    VOID*			pv_tag,
    x_lol_nfy_fct	pf_nfy,
    HANDLE_T*		ph_lol	
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    LOL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[LOL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the LOL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=lol_fs_raw_get_read_hdle(h_dir,
                                   ps_lol_path,
                                   ui2_lol_id,
                                   &h_file);
    if ( i4_rc == LOLR_OK )
    {
        /* Read meta data of the LOL object. */
        if ( lol_read_file_info(h_file, &t_finfo, &ui4_crc) == LOLR_OK )
        {
            /* Assertiogn check, check the lol_id is the same as
               specified.
            */
            if (  t_finfo.ui2_lol_id == ui2_lol_id )
            {
                /*
                  If the user did not provide a LOL name,
                  then use the LOL name stored in the
                  persistent storage.
                */
                if ( ps_lol_name == NULL )
                {
                    ps_lol_name = t_finfo.ps_lol_name;
                }

                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  LOL file object loaded.

                  Create LOL handle from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=lol_create_handle_from_file(h_file,
                                                  ps_lol_name,
                                                  ui2_lol_id,
                                                  pv_tag,
                                                  pf_nfy,
                                                  ph_lol);
                if ( i4_rc == LOLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, LOL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == LOL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = LOLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{LOL} Read Error: fail CRC32 check.\n");
                            i4_rc = LOLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = LOLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != LOLR_OK )
                    {
                        /* clean up. */
                        x_lol_delete(*ph_lol);
                    }
                }
            }
            else
            {
                i4_rc = LOLR_FILE_READ_FAIL;
                /*
                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                */
            }
        }
        else
        {
            i4_rc = LOLR_FILE_READ_FAIL;
            /*
            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
            */
        }
        /*
          close the file handle.
        */
        lol_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  lol_fs_store_object
 *
 * Description: This function write an LOL object to a regular file or
 *              RAW file.
 *
 * Inputs: 
 *   h_lol           Handle to the LOL object.
 *   ui2_lol_rec_id  Specifies the service record to be deleted. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK            Specified record is deleted. 
 *   LOLR_INV_HANDLE    Bad LOL handle.
 *   LOLR_INV_ARG       ui2_lol_rec_id is a NULL record id.
 *   LOLR_REC_NOT_FOUND The specified record to be deleted is not in 
 *                      the LOL object.
 *   
 -----------------------------------------------------------------------*/
INT32 lol_fs_store_object
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_lol_path,
    UINT16		    ui2_lol_id	   
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_LOL_NAME_LEN+1+16];
    HANDLE_T        h_file;
    HANDLE_T        h_lol;
    LOL_OBJ_T*      pt_lol;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a LOL handle to the LOL object.
    */
    if ( (i4_rc = x_lol_open(ui2_lol_id, NULL, NULL, &h_lol)) == LOLR_OK )
    {
        i4_rc= LOLR_FILE_WRITE_FAIL;
        /*
          Get the LOL control block object.
        */
        if ( (handle_get_type_obj(h_lol, &pe_type,((VOID**) &pt_lol))
              ==HR_OK)                                                &&
             (pe_type == LOLT_LARGE_OBJECT_LIST)                           &&
             (pt_lol  != NULL)
            )
        {
            /*
              Lock the LOL object.
            */
            if ( lol_lock_object_for_read(pt_lol) == LOLR_OK )
            {
                db_clear_data_sect_sz_info(pt_lol->h_cdb);
                
                /*
                  Get the internal LOL name used for the storage
                  from the LOL id and LOL name.
                */
                lol_get_database_name(pt_lol->ps_lol_name,
                                      pt_lol->ui2_lol_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing LOL object.
                   Get the offset to location of the file where LOL object
                   will be written to.
                */
                if ( (i4_rc=lol_fs_raw_get_write_hdle(h_dir, ps_lol_path,
                                                      pt_lol, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == LOLR_OK
                    )
                {

                    DBG_API(("{LOL} start writing LOL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing LOL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=lol_is_write_to_file_needed(h_file,pt_lol,&b_write_flag))
                              == LOLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_API(("{LOL} [lol_fs_store_object] no change in data, write ops not needed.\n"));
                            }
                        }
                    }
                    else
                    {
                        /*
                          Writing LOL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == LOLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= LOLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( lol_write_file_header(h_file, pt_lol,
                                                   ui4_beg_magic, &ui4_crc) == LOLR_OK )
                        { 
                            if ( lol_write_metadata(h_file, pt_lol, &ui4_crc) == LOLR_OK )
                            {
                                if ( lol_write_data(h_file, pt_lol, &ui4_crc) == LOLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        lol_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == LOLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = lol_fs_raw_close_hdle(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        lol_fs_raw_close_hdle(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_lol->h_cdb);
                } 
                
                /*
                  don't forget to unlock the LOL
                */
                lol_unlock_object_for_read(pt_lol);
                
            }  /* Can not lock the LOL object. */
            
        }   /* Can not get the LOL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_lol))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to LOL object. */
        x_lol_close(h_lol);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name:  lol_fs_store_object_given_file_handle
 *
 * Description: This function write an LOL object to a regular file or
 *              RAW file given the file handle..
 *
 * Inputs: 
 *   h_file          Handle to the file to write.
 *   ui2_lol_id      Specifies the LOL list ID to be stored. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK            Specified record is deleted. 
 *   LOLR_INV_HANDLE    Bad file handle.
 *   LOLR_NOT_FOUND     The specified LOL database object id (e.g. 
 *                      'ui2_lol_id') is not found.
 *   
 -----------------------------------------------------------------------*/
INT32 lol_fs_store_object_given_file_handle
(
    HANDLE_T        h_file,
    UINT16          ui2_lol_id
)
{
    INT32           i4_rc;
    CHAR            ps_fname[MAX_LOL_NAME_LEN+1+16];
    HANDLE_T        h_lol;
    LOL_OBJ_T*      pt_lol;
    HANDLE_TYPE_T   pe_type;
    UINT32          ui4_beg_magic;
    UINT32          ui4_end_magic;
    UINT32          ui4_crc;
    BOOL            b_new_pos;
    BOOL            b_write_flag;
        
    /*
       Get a LOL handle to the LOL object.
    */
    if ( (i4_rc = x_lol_open(ui2_lol_id, NULL, NULL, &h_lol)) == LOLR_OK )
    {
        i4_rc= LOLR_FILE_WRITE_FAIL;
        /*
          Get the LOL control block object.
        */
        if ( (handle_get_type_obj(h_lol, &pe_type,((VOID**) &pt_lol))
              ==HR_OK)                                                &&
             (pe_type == LOLT_LARGE_OBJECT_LIST)                      &&
             (pt_lol  != NULL)
            )
        {
            /*
              Lock the LOL object.
            */
            if ( lol_lock_object_for_read(pt_lol) == LOLR_OK )
            {
                db_clear_data_sect_sz_info(pt_lol->h_cdb);
                
                /*
                  Get the internal LOL name used for the storage
                  from the LOL id and LOL name.
                */
                lol_get_database_name(pt_lol->ps_lol_name,
                                      pt_lol->ui2_lol_id,
                                      ps_fname);
                /*
                   Open the RAW file for writing LOL object.
                   Get the offset to location of the file where LOL object
                   will be written to.
                */
                if ( (i4_rc=lol_fs_raw_get_write_hdle_no_fm_open(
                                                      pt_lol, &h_file,
                                                      &ui4_beg_magic,
                                                      &b_new_pos))
                     == LOLR_OK
                    )
                {

                    DBG_API(("{LOL} start writing LOL object to file.\n"));

                    if ( b_new_pos == FALSE )
                    {
                        /*
                          If over-writing existing LOL object, we will
                          performing additional check to determine if writing
                          to file system is needed.
                        */
                        if ( (i4_rc=lol_is_write_to_file_needed(h_file,pt_lol,&b_write_flag))
                              == LOLR_OK
                            )
                        {
                            if ( b_write_flag == FALSE )
                            {
                                DBG_INFO(("{LOL} [lol_fs_store_object] no change in data, write ops not needed.\n"));

                                /* x_dbg_stmt("{LOL} [lol_fs_store_object] no change in data, write ops not needed.\n"); */
                            }
                            else
                            {
                                DBG_INFO(("{LOL} [lol_fs_store_object] data changed, write ops is required.\n"));

                                /* x_dbg_stmt("{LOL} [lol_fs_store_object] data changed, write ops is required.\n"); */
                                
                            }
                            
                        }
                    }
                    else
                    {
                        /*
                          Writing LOL object to new file location, set the write_flag
                          to TRUE.
                        */
                        b_write_flag = TRUE;
                    }

                    if ( i4_rc == LOLR_OK      &&
                         b_write_flag == TRUE
                        )
                    {
                        i4_rc= LOLR_FILE_WRITE_FAIL;
                        /*
                          Init the CRC 32 seed value to all 1's bit.
                        */
                        ui4_crc = 0xFFFFFFFF;
                 
                        if ( lol_write_file_header(h_file, pt_lol,
                                                   ui4_beg_magic, &ui4_crc) == LOLR_OK )
                        { 
                            if ( lol_write_metadata(h_file, pt_lol, &ui4_crc) == LOLR_OK )
                            {
                                if ( lol_write_data(h_file, pt_lol, &ui4_crc) == LOLR_OK )
                                {
                                    ui4_end_magic = (~ui4_beg_magic);
                                
                                    /* write end magic and CRC values. */
                                    i4_rc =
                                        lol_write_obj_end_magic_crc(h_file,
                                                                    ui4_end_magic,
                                                                    ui4_crc) ;
                                }
                            }
                        }
                    }
                    
                    /*
                       Don't forget to close the file handle.
                    */
                    if ( i4_rc == LOLR_OK )
                    {
                        /* close the file and  returns the return code
                           from file close operation */
                        i4_rc = lol_fs_raw_close_hdle_no_fm_close(h_file);
                    }
                    else
                    {
                        /* there was error, close the file but return
                           the original error error.
                        */
                        lol_fs_raw_close_hdle_no_fm_close(h_file);
                    }

                    db_clear_data_sect_sz_info(pt_lol->h_cdb);
                } 
                
                /*
                  don't forget to unlock the LOL
                */
                lol_unlock_object_for_read(pt_lol);
                
            }  /* Can not lock the LOL object. */
            
        }   /* Can not get the LOL control block from the handle. */
        else
        {
#ifdef DEBUG
            if(x_handle_valid(h_lol))
            {
                ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
            }
#endif
        }

        /* Finish, don't forget to close the handle to LOL object. */
        x_lol_close(h_lol);
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: lol_write_file_magic_num
 *
 * Description: This API write the LOL file magic number and CRC value.
 *                    
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_lol      Pointer to an LOL handle object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 lol_write_obj_end_magic_crc
(
    HANDLE_T           h_file,
    UINT32             ui4_end_magic,
	UINT32             ui4_crc
)
{
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[LOL_FILE_TAIL_MARKER_SIZE] ;
    
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
        i4_rc=LOLR_OK;

        DBG_API(("{LOL} [lol_write_obj_end_magic_crc]: LOL file end_magic_crc to filesystem.\n"));
        
    }
    else
    {
        i4_rc = LOLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_write_file_header
 *
 * Description: This API write the LOL file header. The file header
 *              consists of :
 *                    magic number (4 bytes)
 *                    file_version_number (1 byte)
 *                    metadata_size  (2 bytes)
 *
 * Inputs: 
 *   h_file      Handle to the persistent storage.
 *   pt_lol      Pointer to an LOL handle object.
 *   ui4_beg_magic The beginning magic value.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 *
 * Outputs:
 *   pui4_crc    CRC32 value computed over the LOL metadata section. 
 *
 * Returns: 
 *   LOLR_OK     Success.
 *
 -----------------------------------------------------------------------*/
static INT32 lol_write_file_header
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
	UINT32             ui4_beg_magic,
    UINT32*            pui4_crc
)
{
    UINT8   ui1_lol_ver;
    UINT16  ui2_meta_data_sz;
    INT32   i4_rc;
    UINT32  ui4_byte_wrote;
    UINT8   aui1_buf[LOL_FILE_HEADER_SIZE] ;
    UINT32  ui4_crc;
    
    /* convert to Big-Endian. */
    x_end_cnvt_to_big_32(&ui4_beg_magic, 1, &(aui1_buf[0]) );
    
    /* The current LOL library version number. */
    ui1_lol_ver=LOL_FILE_VER;
    x_memcpy(&(aui1_buf[4]), &ui1_lol_ver, 1);

    /* The metadata size. */
    ui2_meta_data_sz=LOL_FILE_METADATA_SIZE;
    
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
            ui4_crc = x_crc32_gen(aui1_buf, LOL_FILE_HEADER_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{LOL} [lol_write_file_header]: LOL file header wrote to filesystem.\n"));
                
        i4_rc=LOLR_OK;
    }
    else
    {
        i4_rc = LOLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_write_metadata
 *
 * Description: This API write LOL metadata section to persistent storage.  
 *              The metadata consists of:
 *                     ui2_lol_id  (2 bytes)
 *                     ps_lol_name (17 bytes)
 *                     ui4_data_size (4 bytes)
 *
 * Inputs: 
 *   h_lol       Handle to the LOL object.
 *   pt_lol      Pointer to an LOL handle object.
 *   pui4_crc    Input CRC32 value to be used to compute over the metadata
 *               section.
 * Outputs: None
  *   pui4_crc    CRC32 value computed over the LOL metadata section.  
 *
 * Returns: 
 *   LOLR_OK        success
 *   
 *
 -----------------------------------------------------------------------*/

static INT32 lol_write_metadata
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_data_sz;
    UINT8           aui1_buf[LOL_FILE_METADATA_SIZE] ;
    UINT32          ui4_byte_wrote;
    UINT32          ui4_crc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;
    INT32           i4_pos;
    
        
    /* LOL id number. */
    i4_pos=0;
    x_end_cnvt_to_big_16(&(pt_lol->ui2_lol_id), 1, &(aui1_buf[i4_pos]));
    i4_pos += 2;

    /* LOL name. */
    x_memcpy(&(aui1_buf[i4_pos]), pt_lol->ps_lol_name, MAX_LOL_NAME_LEN);
    i4_pos += (MAX_LOL_NAME_LEN+1);

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_lol->h_cdb,
                                 (db_endian_conv_fct)lol_data_conv_to_big_end,
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
        return LOLR_FILE_WRITE_FAIL;
        
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
            ui4_crc = x_crc32_gen(aui1_buf, LOL_FILE_METADATA_SIZE, (*pui4_crc));
            *pui4_crc = ui4_crc;
        }

        DBG_API(("{LOL} [lol_write_metadata]: LOL file metadata wrote to filesystem.\n"));
                
        i4_rc=LOLR_OK;
    }
    else
    {
        i4_rc = LOLR_FILE_WRITE_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
        */
    }
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: lol_common_data_endconv_to_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored in CDB by the LOL library prior to writing out to
 *              the persistent storage.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK          LOL database is unlocked. 
 *   LOLR_INV_HANDLE  Bad LOL handle.
 *   
 -----------------------------------------------------------------------*/
static VOID lol_common_data_endconv_to_file
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
 * Name: lol_common_data_endconv_from_file
 *
 * Description: This function perform endian conversion of the common data
 *              stored on persistent storage before return to result to
 *              structure in memory.  Currently, the common
 *              data contains the min and max record id values in the
 *              database.
 *
 * Inputs: 
 *   h_lol	Handle to the LOL object.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK          LOL database is unlocked. 
 *   LOLR_INV_HANDLE  Bad LOL handle.
 *   
 -----------------------------------------------------------------------*/
VOID lol_common_data_endconv_from_file
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
 * Name: lol_write_data
 *
 * Description: This function write out the data section of the LOL object.   
 *
 * Inputs: 
 *   pt_lol	Handle to the LOL object. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL is locked. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *
 -----------------------------------------------------------------------*/
static INT32 lol_write_data
(
    HANDLE_T           h_file,
    LOL_OBJ_T*         pt_lol,
    UINT32*            pui4_crc
) 
{
    INT32           i4_rc;

    i4_rc=db_save_to_file(pt_lol->h_cdb, h_file,
                          (db_endian_conv_fct)lol_data_conv_to_big_end,
                          lol_common_data_endconv_to_file);
    
    i4_rc = lol_cdb_to_lol_err_code(i4_rc);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_read_file_info
 *
 * Description: This API read the LOL file header section and meta data 
 *              section from a specified FILE handle.
 * Inputs: 
 *   h_file:  LOL file handle.
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           LOL file info is read successfully.
 *   
 -----------------------------------------------------------------------*/
INT32 lol_read_file_info
(
    HANDLE_T           h_file,
    LOL_FILE_INFO_T*   pt_finfo,
    UINT32*            pui4_crc
)
{
    INT32           i4_rc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[LOL_FILE_HEADER_SIZE+LOL_FILE_METADATA_SIZE];
    UINT32          ui4_crc;

    i4_rc=x_fm_read(h_file,aui1_buf,sizeof(aui1_buf),&ui4_byte_read);
    if ( i4_rc == FMR_OK                     &&
         (LOL_FILE_HEADER_SIZE+LOL_FILE_METADATA_SIZE) == ui4_byte_read
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

        DBG_API(("{LOL} read the beginning magic value: %x\n",
                   pt_finfo->ui4_magic));

        if ( pt_finfo->ui4_magic == LOL_MAGIC_NUMBER    ||
             pt_finfo->ui4_magic == (~LOL_MAGIC_NUMBER) 
            )
        {
            /*
              Get the version number.
            */
            x_memcpy(&(pt_finfo->ui1_lol_ver), &(aui1_buf[4]), 1);

            DBG_API(("{LOL} version number read from file object is: %d "
                     "current file version in memory is %d\n",
                     pt_finfo->ui1_lol_ver, LOL_FILE_VER));
            /*
              Check the LOL file version number. 
            */
            if ( pt_finfo->ui1_lol_ver == LOL_FILE_VER )
            {
                /*
                  Get the size of the metadata section.
                */
                x_end_cnvt_from_big_16(&(aui1_buf[5]),1,&(pt_finfo->ui2_meta_data_sz));

                /*
                  Get the lol id
                */
                x_end_cnvt_from_big_16(&(aui1_buf[7]),1,&(pt_finfo->ui2_lol_id));

                /*
                  Get the lol name.
                */
                x_memcpy(pt_finfo->ps_lol_name,&(aui1_buf[9]),MAX_LOL_NAME_LEN);

                /*
                  Get the data section size.
                */
                x_end_cnvt_from_big_32(&(aui1_buf[9+MAX_LOL_NAME_LEN+1]),
                                       1,&(pt_finfo->ui4_data_size));
                i4_rc=LOLR_OK;
            }
            else
            {
                DBG_API(("{LOL} Wrong LOL file version encountered: current software ver: %d  file ver %d.\n",
                        LOL_FILE_VER , pt_finfo->ui1_lol_ver ));
                
                
                i4_rc = LOLR_INV_FILE_VER;
            }
        }
        else
        {
            DBG_API(("{LOL} bad file magic value: the correct magic value is %x or %x\n",
                     LOL_MAGIC_NUMBER, (~LOL_MAGIC_NUMBER)));

            /*
              Wrong magic number.
            */
            i4_rc=LOLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = LOLR_FILE_READ_FAIL;
        /*
        ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
        */
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: lol_create_handle_from_file
 *
 * Description: This API create an LOL object by loading the paramters
 *              and records from persistent storage.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   LOLR_OK             LOL library is initialized successfully.
 *   LOLR_OUT_OF_MEM     Not enough memory to create the LOL object.
 *   LOLR_INV_ARG        'ui2_max_num_lol_obj' is 0.
 *   LOLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 lol_create_handle_from_file
(
    HANDLE_T         h_file,
    const CHAR*      ps_lol_name,
    UINT16           ui2_lol_id,    
    VOID*            pv_tag,
    x_lol_nfy_fct    pf_nfy_fct,
    HANDLE_T*        ph_lol
)
{
    CHAR               s_name[CDB_MAX_DATABASE_NAME_LEN+1];
    LOL_OBJ_T*         pt_lol=NULL;
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    HANDLE_T           h_cdb=NULL_HANDLE;
    INT32              i4_rc=LOLR_FAIL;
    
    /* create CDB name. */
    lol_get_database_name(ps_lol_name,ui2_lol_id,s_name);

    /* get the record definition. */
    lol_db_rec_and_key_def(&ui1_fld_count,
                           &pt_rec_fields,
                           &pui4_recfld_type,
                           &ui1_key_count,
                           &pui4_keyfld_type);
    
    lol_sema_lock();

    /* check if the input LOL name and id has not been used.
       then, add the name and id to the internal list. */

    if ( lol_check_id_name(ui2_lol_id,ps_lol_name) == TRUE &&
         lol_add_id_name(ui2_lol_id,ps_lol_name) == LOLR_OK
        )
    {
        /* Create a LOL object. */
        pt_lol=(LOL_OBJ_T*) x_mem_alloc(sizeof(LOL_OBJ_T));

        /* Create a core database object. */
        if ( pt_lol != NULL )
        {
            i4_rc=db_load_from_file(
                s_name,
                ui1_fld_count,
                pt_rec_fields,
                ui1_key_count,
                pui4_keyfld_type,
                h_file,
                /* Endian conversion function. */
                (db_endian_conv_fct)lol_data_conv_from_big_end,
                /* Common data endian conversion function. */
                (db_cdata_conv_fct)lol_common_data_endconv_from_file,
                (VOID*)(&pt_lol),
                lol_nfy_fct,
                &h_cdb);
            
            /* translate the CDB error code to LOL error code */
            i4_rc=lol_cdb_to_lol_err_code(i4_rc);
            if ( i4_rc == LOLR_OK )
            {

                DBG_API(("{LOL} LOL's cdb object loaded.\n"));

                /*
                  Allocate a handle for the LOL object.
                */
                if ( handle_alloc(LOLT_LARGE_OBJECT_LIST,
                                  (VOID*)pt_lol,
                                  pv_tag,
                                  lol_handle_free_fct,
                                  ph_lol)
                     == HR_OK )
                {
                    /* initialize the LOL object. */
                    pt_lol->h_lol=*ph_lol;
                    pt_lol->h_cdb=h_cdb;
                    pt_lol->pf_nfy_fct=pf_nfy_fct; /* save the pointer to the
                                                      caller's notification
                                                      function. */
                    pt_lol->ui2_lol_id=ui2_lol_id;
                    x_strncpy(pt_lol->ps_lol_name,ps_lol_name,
                              MAX_LOL_NAME_LEN);

                    /* Set the initial search parameter and cursor position
                       to unknown initial values
                       This will force sorted list to be created and search
                       to start from the first element of array.
                    */
                    x_memset(&(pt_lol->t_parm), LOL_UNSET_VAL,
                         sizeof(LOL_SEARCH_PARM_T));

                    x_memset(&(pt_lol->t_cursor), LOL_UNSET_VAL,
                         sizeof(LOL_CURSOR_T));

#if 0
                    /* right now, we don't have any customize sort/compare
                       function associated with LOL handle object.
                    */
                    x_memset(&(pt_lol->t_cmp_fct_info), 0,
                         sizeof(LOL_CMP_FCT_INFO_T));
#endif                    
                
                    i4_rc=LOLR_OK;

                    DBG_API(("{LOL} LOL object created from file\n"));
                }
                else
                {
                    i4_rc=LOLR_OUT_OF_HANDLE;
                }
            }
        }
        else
        {
            i4_rc = LOLR_OUT_OF_MEM ;
        }

        if ( i4_rc != LOLR_OK )
        {
            /* Remove the id name pair. */
            lol_del_id(ui2_lol_id);
        }
    }
    else
    {
        i4_rc = LOLR_FAIL;
    }
    lol_sema_unlock();
    
    if ( i4_rc != LOLR_OK )
    {
        if ( h_cdb != NULL_HANDLE )
        {
            db_delete(h_cdb,s_name);
        }
        if ( pt_lol != NULL )
        {
            x_mem_free(pt_lol);
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: lol_sync_data_with_file
 *
 * Description:
 *    This API create synchronize LOL object by loading the paramters
 *    and records from persistent storage. The existing data are over-
 *    written with data from the specified file.
 *
 * Inputs: 
 *    
 *
 * Outputs: 
 *
 * Returns: 
 *   LOLR_OK             LOL library is initialized successfully.
 *   LOLR_OUT_OF_MEM     Not enough memory to create the LOL object.
 *   LOLR_INV_ARG        'ui2_max_num_lol_obj' is 0.
 *   LOLR_OUT_OF_HANDLE  No handle resource.
 ----------------------------------------------------------------------------*/
static INT32 lol_sync_data_with_file
(
    HANDLE_T         h_lol,
    HANDLE_T         h_file
)
{
    UINT8              ui1_fld_count=0;
    DB_FIELD_INFO_T*   pt_rec_fields=NULL;
    DB_FIELD_T*        pui4_recfld_type=NULL;
    UINT8              ui1_key_count=0;
    DB_FIELD_T*        pui4_keyfld_type=NULL;
    INT32              i4_rc=LOLR_FAIL;
    LOL_OBJ_T*         pt_lol=NULL;
    HANDLE_TYPE_T      pe_type;

    if ( (handle_get_type_obj(h_lol, &pe_type,((VOID**) &pt_lol)) == HR_OK) &&
         (pe_type == LOLT_LARGE_OBJECT_LIST)                                     &&
         (pt_lol  != NULL)
        )
    {
        /* get the record definition. */
        lol_db_rec_and_key_def(&ui1_fld_count,
                               &pt_rec_fields,
                               &pui4_recfld_type,
                               &ui1_key_count,
                               &pui4_keyfld_type);
    
        /* Load the data from the file into CDB database object. */
        if ( pt_lol != NULL )
        {
            i4_rc=db_sync_data_with_file(pt_lol->h_cdb,
                                         ui1_fld_count,
                                         pt_rec_fields,
                                         ui1_key_count,
                                         pui4_keyfld_type,
                                         h_file,
                                         /* Endian conversion function. */
                                         (db_endian_conv_fct)
                                         lol_data_conv_from_big_end,
                                         /* Common data endian conversion
                                            function. */
                                         (db_cdata_conv_fct)
                                         lol_common_data_endconv_from_file
                );
            
            /* translate the CDB error code to LOL error code */
            i4_rc=lol_cdb_to_lol_err_code(i4_rc);
            if ( i4_rc == LOLR_OK )
            {
                DBG_API(("{LOL} LOL records are sync'd with file\n"));
            }
        }
        else
        {
            i4_rc = LOLR_INV_ARG ;
        }
    }
    else
    {
#ifdef DEBUG
        if(x_handle_valid(h_lol))
        {
            ABORT (DBG_CAT_HANDLE, DBG_ABRT_INVALID_HANDLE_TYPE);
        }
#endif
        i4_rc = LOLR_INV_HANDLE;
    }
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_persistent_size
 *
 * Description: This API returns the space needed to store
 *              a LOL object on the file system.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   size (in bytes) for storing the LOL object to file. The size includes
 *   the header (magic number, etc), metadata, and the CDB object which
 *   contains LOL records.
 *   0 if the LOL object size can not be determine.
 *   
 -----------------------------------------------------------------------*/
UINT32 lol_persistent_size
(
    LOL_OBJ_T*      pt_lol
)
{
    UINT32          ui4_sz;
    INT32           i4_rc;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    /* Size of CDB object. */
    i4_rc=db_get_persistent_size(pt_lol->h_cdb,
                                 (db_endian_conv_fct)lol_data_conv_to_big_end,
                                 &ui4_persistent_size,
                                 &ui4_ver_id);
    if ( i4_rc == DBR_OK )
    {
        ui4_sz = LOL_FILE_HEADER_SIZE + LOL_FILE_METADATA_SIZE +
            ui4_persistent_size + LOL_FILE_TAIL_MARKER_SIZE ;
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
 * Name: lol_is_write_to_file_needed
 *
 * Description: This API compare the LOL object stored on filesystem
 *              with LOL object in memory. If they contains different
 *              data, then save (e.g., write operation) will be needed.
 *
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to the service record structure to be added. 
 *
 * Outputs:
 *   pb_rc:     TRUE  LOL object in memory and LOL object in filesystem
 *                    contains same data, no write operation is needed.
 *              FALSE LOL object in memory and LOL object in filesystem
 *                    contains different data, write operation is needed.
 *
 * Returns: 
 *   LOLR_OK:      
 *   LOLR_FILE_READ_FAIL:   Can not read the LOL object on filesystme.
 *
 -----------------------------------------------------------------------*/
INT32 lol_is_write_to_file_needed
(
    HANDLE_T        h_file,
    LOL_OBJ_T*      pt_lol,
    BOOL*           pb_rc
)
{
    UINT64          ui8_fpos;
    UINT64          ui8_end_fpos;
    INT32           i4_rc;
    LOL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    BOOL            b_result;
    INT32           i4_res;
    UINT32          ui4_persistent_size;
    UINT32          ui4_ver_id;

    i4_rc  = LOLR_OK;
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

        /* Read meta data of the LOL object. */
        if ( lol_read_file_info(h_file, &t_finfo, &ui4_crc) == LOLR_OK )
        {
            /*
              Compare the LOL file header and metadata information
              with the LOL object in memory to see if they contain
              different data.
            */
            if ( t_finfo.ui1_lol_ver != LOL_FILE_VER )
            {
                DBG_INFO(("{LOL} [lol_is_write_to_file_needed] lol_ver diff\n"));
                
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_lol_id != pt_lol->ui2_lol_id )
            {
                DBG_INFO(("{LOL} [lol_is_write_to_file_needed] lol_id diff\n"));
                *pb_rc = TRUE;
            }
            else if ( x_strcmp(t_finfo.ps_lol_name, pt_lol->ps_lol_name) != 0 )
            {
                DBG_INFO(("{LOL} [lol_is_write_to_file_needed] lol_name diff\n"));
                *pb_rc = TRUE;
            }
            else if ( t_finfo.ui2_meta_data_sz != LOL_FILE_METADATA_SIZE )
            {
                DBG_INFO(("{LOL} [lol_is_write_to_file_needed] lol_metadata size diff\n"));
                *pb_rc = TRUE;
            }
            else
            {
                /*
                  Check the data size between memory and size of data
                  stored on file.
                */
                if ( db_get_persistent_size(pt_lol->h_cdb,
                                            (db_endian_conv_fct)lol_data_conv_to_big_end,
                                            &ui4_persistent_size,
                                            &ui4_ver_id)
                     == DBR_OK )
                {

                    DBG_INFO(("{LOL} size of LOL obj on file: [%d bytes], size of LOL data in memory: [%d bytes]\n",
                               ui4_persistent_size, t_finfo.ui4_data_size));                   
                    
                    if ( ui4_persistent_size != t_finfo.ui4_data_size )
                    {
                         DBG_INFO(("{LOL} data sect size difference between LOL obj in memory [%d bytes] and on filesystem [%d bytes]\n", ui4_persistent_size , t_finfo.ui4_data_size ));
                         
                        *pb_rc = TRUE;
                       
                    }
                    /*
                      check if writing to CDB object is needed.
                    */
                    else if ( db_is_save_to_file_needed(
                                  pt_lol->h_cdb, h_file,
                                  (db_endian_conv_fct)lol_data_conv_to_big_end,
                                  NULL,
                                  &b_result) == DBR_OK
                        )
                    {
                        if ( b_result == TRUE )
                        {
                            DBG_API(("{LOL} writing CDB data to file system is needed.\n"));
                            *pb_rc = TRUE;
                        }
                    }
                    else
                    {
                        i4_rc = LOLR_FILE_READ_FAIL;
                    }
                }
                else
                {
                    i4_rc= LOLR_FILE_READ_FAIL;
                }
            }
        }
        else
        {
            DBG_API(("{LOL} can not read metadata info from lol object on the file system,"
                     " enable the write flag to write over the lol object in file. \n"));

            *pb_rc = TRUE;
            
        }
        
        /*
          Reset the file position.
        */
        if ( x_fm_lseek(h_file, (INT64) ui8_fpos,
                        FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
            )
        {
            i4_rc = LOLR_OK;
        }
        else
        {
            i4_rc = LOLR_FILE_READ_FAIL;
        }
    }
    else
    {
        i4_rc = LOLR_FILE_READ_FAIL;
        DBG_ERROR(("{LOL} [lol_is_write_to_file_needed] x_fm_lseek error: %d\n",i4_res));
    }
    
    /* remove warning. */
    i4_res=(i4_res+1);
    
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_fs_sync_object
 *
 * Description:
 *   This API sync an existing LOL object with records
 *   from a persistent LOL object stored on file system.
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Record is successfully added. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *   LOLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   LOLR_INV_ARG      'pt_lol_rec' is null or the LOL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 lol_fs_sync_object
(
    HANDLE_T		h_lol,	
    HANDLE_T        h_dir,
    const CHAR*	    ps_lol_path,
    UINT16		    ui2_lol_id
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    LOL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[LOL_FILE_TAIL_MARKER_SIZE];

    h_file = NULL_HANDLE;

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the LOL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=lol_fs_raw_get_read_hdle(h_dir,
                                   ps_lol_path,
                                   ui2_lol_id,
                                   &h_file);
    if ( i4_rc == LOLR_OK )
    {
        /* Read meta data of the LOL object. */
        if ( lol_read_file_info(h_file, &t_finfo, &ui4_crc) == LOLR_OK )
        {
            /* Assertiogn check, check the lol_id is the same as
               specified.
            */
            if (  t_finfo.ui2_lol_id == ui2_lol_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  LOL file object loaded.

                  Sync LOL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=lol_sync_data_with_file(h_lol, h_file);
                if ( i4_rc == LOLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, LOL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == LOL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = LOLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{LOL} Read Error: fail CRC32 check.\n");
                            i4_rc = LOLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = LOLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != LOLR_OK )
                    {
                        /* clean up. */
                        x_lol_delete(h_lol);
                    }
                }
            }
            else
            {
                i4_rc = LOLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = LOLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        lol_fs_raw_close_hdle(h_file);
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: lol_fs_sync_object_given_file_handle
 *
 * Description:
 *   This API sync an existing LOL object with records
 *   from a persistent LOL object stored on file system.
 *
 * Inputs: 
 *   h_lol      Handle to the LOL object.
 *   pt_lol_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   LOLR_OK           Record is successfully added. 
 *   LOLR_INV_HANDLE   Bad LOL handle.
 *   LOLR_OUT_OF_MEM   Can not allocate memory, add record failed,.
 *   LOLR_INV_ARG      'pt_lol_rec' is null or the LOL record id is a invalid.
 -----------------------------------------------------------------------*/
INT32 lol_fs_sync_object_given_file_handle
(
    HANDLE_T		h_lol,	
    HANDLE_T		h_file,	
    UINT16		    ui2_lol_id
)
{
    INT32           i4_rc;
    LOL_FILE_INFO_T t_finfo;
    UINT32          ui4_crc;
    UINT32          ui4_byte_read;
    UINT8           aui1_buf[LOL_FILE_TAIL_MARKER_SIZE];

    /*
      Initialize the CRC seed value to all 1's.
    */
    ui4_crc = 0xFFFFFFFF;
    
    /*
      Access the RAW file to obtain the LOL object info
      and the file handle to read from the RAW file.
      The file handle created is position at the
      location of CDB object.
    */
    i4_rc=lol_fs_raw_get_read_hdle_no_fm_open(
                                   ui2_lol_id,
                                   &h_file);
    if ( i4_rc == LOLR_OK )
    {
        /* Read meta data of the LOL object. */
        if ( lol_read_file_info(h_file, &t_finfo, &ui4_crc) == LOLR_OK )
        {
            /* Assertiogn check, check the lol_id is the same as
               specified.
            */
            if (  t_finfo.ui2_lol_id == ui2_lol_id )
            {
                /*
                  The file read pointer is now at the
                  position at the CDB file object is inside
                  LOL file object loaded.

                  Sync LOL handle with data from the persistent
                  storage, and load all the records from
                  the CDB object.
                */
                i4_rc=lol_sync_data_with_file(h_lol, h_file);
                if ( i4_rc == LOLR_OK )
                {
                    /*
                       reading the end-magic number and CRC.
                    */
                    if ( x_fm_read(h_file, aui1_buf, LOL_FILE_TAIL_MARKER_SIZE,
                                   &ui4_byte_read) == FMR_OK            &&
                         ui4_byte_read == LOL_FILE_TAIL_MARKER_SIZE
                        )
                    {
                        /*
                          check the CRC value. The first 4 bytes of aui1_buf is
                          the CRC value.
                        */
                        if ( x_crc32_gen(aui1_buf, 4, ui4_crc ) == 0 )
                        {
                            i4_rc = LOLR_OK;
                        }
                        else
                        {
                            x_dbg_stmt("{LOL} Read Error: fail CRC32 check.\n");
                            i4_rc = LOLR_FILE_READ_FAIL;
                        }
                    }
                    else
                    {
                        i4_rc = LOLR_FILE_READ_FAIL;
                    }

                    if ( i4_rc != LOLR_OK )
                    {
                        /* clean up. */
                        x_lol_delete(h_lol);
                    }
                }
            }
            else
            {
                i4_rc = LOLR_FILE_READ_FAIL;
            }
        }
        else
        {
            i4_rc = LOLR_FILE_READ_FAIL;
        }
        /*
          close the file handle.
        */
        lol_fs_raw_close_hdle_no_fm_close(h_file);
    }
    return i4_rc;
}
