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
 * $RCSfile: nwl_fs_raw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements IO function for reading/writing/querying/deleting
 *    NWL object to a raw file (or a regular file). It uses fs_raw_*** API's
 *    to allocate write and read handle.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "file_mngr/x_fm.h"
#include "util/fs_raw.h"

#include "nwl/_nwl.h"
#include "nwl/nwl_fs.h"
#include "nwl/nwl_dbg.h"
/*-------------------------------------------------------------------------
            NWL file system related functions implementation
 --------------------------------------------------------------------------*/

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_get_magic_for_write_ops
 *
 * Description: This API to get the begin magic number to
 *              use for this write operation. Note: each time we
 *              write to the file, we will use the toggle the
 *              begin magic value to the negation of the
 *              previous value.  In addition, the begin and end
 *              magic number is negation of each other.
 *
 * Inputs: 
 *   h_file     Handle to the NWL file.
 *              After return from this API, the file handle is positioned
 *              at the file location where the NWL object will be
 *              written.
 *              
 *   ui4_obj_offset: the offset to the beginning of the write operation
 *                   for this NWL object.
 *
 * Outputs:
 *   pui4_beg_magic  Beginning magic number 
 *
 *   pb_new_pos:  Flag indicating if the file handle returned refers to an
 *                existing NWL object stored on filesystem.
 *
 *                TRUE: the location points to by the file handle is not
 *                      a NWL object.
 *                FALSE: the location points to by the file handle contains
 *                      an existing NWL object.
 *
 * Returns: 
 *   NWLR_OK      magic number is returned.
 *   
 -----------------------------------------------------------------------*/
static
INT32 nwl_fs_raw_get_magic_for_write_ops
(
    HANDLE_T  h_file,
    UINT32    ui4_obj_offset,
    UINT32*   pui4_beg_magic,
    BOOL*     pb_new_pos
)
{
    INT32     i4_rc;
    UINT32    ui4_magic;
    
    if ( fs_raw_seek_beg_magic_for_write_ops(h_file,
                                             ui4_obj_offset,
                                             &ui4_magic)
         == FSRAWR_OK
        )
    {
        /*
          If the magic value is not recognized, then it means
          this write position is at a new location. In that case,
          we pick a default MAGIC number to start.
        */
        if ( ui4_magic != NWL_MAGIC_NUMBER  &&
             ui4_magic != (~NWL_MAGIC_NUMBER)
            )
        {
            *pui4_beg_magic = NWL_MAGIC_NUMBER;
            *pb_new_pos     = TRUE;
        }
        else
        {
            /* Flip the magic value. */
            *pui4_beg_magic = (~ui4_magic);
            *pb_new_pos     = FALSE;
        }
        i4_rc = NWLR_OK;
    }
    else
    {
        i4_rc = NWLR_FILE_READ_FAIL ;
    }
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_close_hdle
 *
 * Description: This API unlock and close the file handle. 
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL file.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           file handle is closed.
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_close_hdle
(
    HANDLE_T  h_file
)
{
    INT32 i4_rc;

    fs_raw_unlock(h_file);
    i4_rc=x_fm_close(h_file);

    /* Translate FM err code to NWL err code. */
    i4_rc=nwl_fm_to_nwl_err_code(i4_rc);

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_close_hdle_no_fm_close
 *
 * Description: This API unlock and close the file handle. 
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL file.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           file handle is successfully closed. 
 *   
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_close_hdle_no_fm_close
(
    HANDLE_T  h_file
)
{
    INT32 i4_rc;

    i4_rc=fs_raw_unlock(h_file);

    /* Translate FS_RAW err code to NWL err code. */
    i4_rc=nwl_fsraw_to_nwl_err_code(i4_rc);

    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_get_read_hdle
 *
 * Description: This API return a file handle to the RAW file, the read
 *              pointer is position at the NWL object specified by
 *              the ui2_nwl_id.
 *
 *              This API will check the begin and end NWL magic number
 *              to detect any corruption.
 *
 * Inputs: 
 *   ps_nwl_path  Path to the RAW file.
 *   ui2_nwl_id   Specify the NWL id to load
 *
 * Outputs: 
 *   ph_file:     File handle to the NWL object on the RAW file.
 *
 * Returns: 
 *   NWLR_OK           File handle is returned.
 *   NWLR_NOT_FOUND    NWL object not found.
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_get_read_hdle
(
    HANDLE_T           h_dir,
    const CHAR*	       ps_nwl_path,
    UINT16		       ui2_nwl_id,
    HANDLE_T*		   ph_file	
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;
    UINT32          ui4_offset;
    UINT32          ui4_obj_sz;
    UINT32          ui4_beg_magic_num;
    UINT32          ui4_end_magic_num;

    *ph_file=NULL_HANDLE;

    if ( fs_raw_is_inited(h_dir, ps_nwl_path) == FALSE )
    {
        return NWLR_INV_FILE_PATH;
    }
    /*
      Open the RAW file.
    */
    i4_rc=x_fm_open(h_dir,
                    ps_nwl_path,
                    FM_READ_ONLY,
                    0644,
                    FALSE,
                    &h_file);
    if ( i4_rc == FMR_OK                          &&
         fs_raw_read_lock(h_file) == FSRAWR_OK
        )
    {
        /*
          Set the file offset to the NWL object found.
        */
        i4_rc=fs_raw_seek_obj(h_file, &t_meta,
                              (UINT32)(FS_RAW_MAKE_OBJ_ID(NWL_GROUP_ID,ui2_nwl_id)),
                              &ui4_offset, &ui4_obj_sz);
        if (  i4_rc == FSRAWR_OK )
        {

            DBG_API(("{NWL} locating object for read operation> object id: %d size: %d offset: %d\n",
                       ui2_nwl_id, ui4_obj_sz, ui4_offset));

            /*
              Check the begin and ending magic number for the NWL object.
              The begin magic number should be equal to the
              negation of the end-magic number.
            */
            i4_rc=fs_raw_get_obj_beg_end_magic_num(h_file,
                                                   ui4_obj_sz,
                                                   &ui4_beg_magic_num,
                                                   &ui4_end_magic_num);
            if ( i4_rc == FSRAWR_OK )
            {

                DBG_API(("{NWL} begin_magic: %x end_magic: %x\n",
                           ui4_beg_magic_num, ui4_end_magic_num));

                if ( (
                         ui4_beg_magic_num == NWL_MAGIC_NUMBER  ||
                         ui4_beg_magic_num == (~NWL_MAGIC_NUMBER)
                     )   &&
                     ( ui4_beg_magic_num == (~ui4_end_magic_num) )
                   )
                {
                    /*
                      The file read pointer is now at the start
                      position of the NWL object is inside
                      RAW file.  Save the RAW file handle.
                    */
                    *ph_file = h_file;
                    i4_rc=NWLR_OK;
                    
                }
                else
                {
                    /* Bad magic, this NWL object is corrupted. */
                    /* Don't forget to close the file handle. */
                    nwl_fs_raw_close_hdle(h_file);
                    i4_rc = NWLR_FILE_READ_FAIL;
                }
            }
            else
            {
                /* Translate FSRAW err code to NWL err code. */
                i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            
                /* Error condition occur:
                   Don't forget to close the file handle. */
                nwl_fs_raw_close_hdle(h_file);
            }    
        }
        else
        {
            /* Translate FSRAW err code to NWL err code. */
            i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            
            /* Error condition occur:
               Don't forget to close the file handle. */
            nwl_fs_raw_close_hdle(h_file);
        }
    }
    else
    {
        /* Translate FM err code to NWL err code. */
        i4_rc = nwl_fm_to_nwl_err_code(i4_rc);
    }        
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_get_read_hdle_no_fm_open
 *
 * Description: This API return a file handle to the RAW file, the read
 *              pointer is position at the NWL object specified by
 *              the ui2_nwl_id.
 *
 *              This API will check the begin and end NWL magic number
 *              to detect any corruption.
 *
 * Inputs: 
 *   ps_nwl_path  Path to the RAW file.
 *   ui2_nwl_id   Specify the NWL id to load
 *
 * Outputs: 
 *   ph_file:     File handle to the NWL object on the RAW file.
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_NOT_FOUND    NWL object not found.
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_get_read_hdle_no_fm_open
(
    UINT16		       ui2_nwl_id,
    HANDLE_T*		   ph_file	
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;
    UINT32          ui4_offset;
    UINT32          ui4_obj_sz;
    UINT32          ui4_beg_magic_num;
    UINT32          ui4_end_magic_num;

    if (*ph_file == NULL_HANDLE)
    {
        return NWLR_INV_HANDLE;
    }
    h_file = *ph_file;

    if ( (i4_rc=fs_raw_read_lock(h_file)) == FSRAWR_OK )
    {
        /*
          Set the file offset to the NWL object found.
        */
        i4_rc=fs_raw_seek_obj(h_file, &t_meta,
                              (UINT32) FS_RAW_MAKE_OBJ_ID(NWL_GROUP_ID,ui2_nwl_id),
                              &ui4_offset, &ui4_obj_sz);
        if (  i4_rc == FSRAWR_OK )
        {

            DBG_API(("{NWL} locating object for read operation> object id: %d size: %d offset: %d\n",
                       ui2_nwl_id, ui4_obj_sz, ui4_offset));

            /*
              Check the begin and ending magic number for the NWL object.
              The begin magic number should be equal to the
              negation of the end-magic number.
            */
            i4_rc=fs_raw_get_obj_beg_end_magic_num(h_file,
                                                   ui4_obj_sz,
                                                   &ui4_beg_magic_num,
                                                   &ui4_end_magic_num);
            if ( i4_rc == FSRAWR_OK )
            {

                DBG_API(("{NWL} begin_magic: %x end_magic: %x\n",
                           ui4_beg_magic_num, ui4_end_magic_num));

                if ( (
                         ui4_beg_magic_num == NWL_MAGIC_NUMBER  ||
                         ui4_beg_magic_num == (~NWL_MAGIC_NUMBER)
                     )   &&
                     ( ui4_beg_magic_num == (~ui4_end_magic_num) )
                   )
                {
                    /*
                      The file read pointer is now at the start
                      position of the NWL object is inside
                      RAW file.  Save the RAW file handle.
                    */
                    *ph_file = h_file;
                    i4_rc=NWLR_OK;
                    
                }
                else
                {
                    /* Bad magic, this NWL object is corrupted. */
                    /* Don't forget to close the file handle. */
                    nwl_fs_raw_close_hdle_no_fm_close(h_file);
                    i4_rc = NWLR_FILE_READ_FAIL;
                }
            }
            else
            {
                /* Translate FSRAW err code to NWL err code. */
                i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            
                /* Error condition occur:
                   Don't forget to close the file handle. */
                nwl_fs_raw_close_hdle_no_fm_close(h_file);
            }    
        }
        else
        {
            /* Translate FSRAW err code to NWL err code. */
            i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            
            /* Error condition occur:
               Don't forget to close the file handle. */
            nwl_fs_raw_close_hdle_no_fm_close(h_file);
        }
    }
    else
    {
        /* Translate FS_RAW err code to NWL err code. */
        i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
    }        
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_get_write_hdle
 *
 * Description: This API return a file handle to the RAW file, the read
 *              pointer is position at the NWL object specified by
 *              the ui2_nwl_id.
 *             
 *
 * Inputs: 
 *   ps_nwl_path  Path to the RAW file.
 *   ui2_nwl_id   Specify the NWL id to load
 *
 * Outputs: 
 *   ph_file:     File handle to the NWL object on the RAW file.
 *   pui4_beg_magic: The beginning magic number to be used for the write
 *                   operation.
 *   pb_new_pos:  Flag indicating if the file handle returned ref to an
 *                existing NWL object stored on filesystem.
 *                TRUE: the location points to by the file handle is not
 *                      a NWL object.
 *                FALSE: the location points to by the file handle contains
 *                      an existing NWL object.
 *
 * Returns: 
 *   NWLR_OK           File write handle is returned 
 *   NWLR_NOT_FOUND    NWL object not found.
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_get_write_hdle
(
    HANDLE_T           h_dir,
    const CHAR*	       ps_nwl_path,
    NWL_OBJ_T*         pt_nwl,
    HANDLE_T*		   ph_file,
	UINT32*            pui4_beg_magic,
    BOOL*              pb_new_pos
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;
    UINT16          ui2_nwl_id;
    UINT32          ui4_obj_space_needed;
    UINT32          ui4_write_offset;

    ui2_nwl_id = pt_nwl->ui2_nwl_id;
    *ph_file=NULL_HANDLE;

    if ( fs_raw_is_inited(h_dir, ps_nwl_path) == FALSE )
    {
        if ( fs_raw_init(h_dir, ps_nwl_path) != FSRAWR_OK )
        {
            return NWLR_INV_FILE_PATH;
        }
    }
    /*
      Open the RAW file.
    */
    i4_rc=x_fm_open(h_dir,
                    ps_nwl_path,
                    FM_OPEN_CREATE | FM_READ_WRITE,
                    0644,
                    FALSE,
                    &h_file);

    if ( i4_rc == FMR_OK                       &&
         fs_raw_write_lock(h_file) == FSRAWR_OK
       )
    {
        /*
          Get the size of the NWL object to write to
          file
        */
        ui4_obj_space_needed=nwl_persistent_size(pt_nwl);
        

        DBG_API(("{NWL} object spaced required: %d bytes\n",ui4_obj_space_needed));


        if ( ui4_obj_space_needed > 0 )
        {   
            /* Get file offset position within the RAW file to write the
               NWL object.
            */
            i4_rc=fs_raw_realloc(h_file,
                                 (UINT32)(FS_RAW_MAKE_OBJ_ID(NWL_GROUP_ID,ui2_nwl_id)),
                                 ui4_obj_space_needed,
                                 &t_meta,
                                 &ui4_write_offset);
            if ( i4_rc == FSRAWR_OK )
            {
                /*
                  Get the magic number to be used for this
                  write operation, and position the file pointer
                  to the position for write operation.
                */
                if ( nwl_fs_raw_get_magic_for_write_ops(h_file,
                                                        ui4_write_offset,
                                                        pui4_beg_magic,
                                                        pb_new_pos)
                     == NWLR_OK
                    )
                {
                    *ph_file=h_file;
                    i4_rc=NWLR_OK;
                }
                else
                {
                    /*
                      File I/O error occur:
                      Don't forget to close the file handle.
                    */
                    nwl_fs_raw_close_hdle(h_file);
                    
                    i4_rc = NWLR_FILE_WRITE_FAIL;
                    /*
                    ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
                    */
                }
            }
            else
            {
                /*
                   Error condition occur:
                   Don't forget to close the file handle.
                */
                nwl_fs_raw_close_hdle(h_file);
                
                /* Translate FM err code to NWL err code. */
                i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            }
        }
        else
        {
            /*
               Object size is 0, return error code and 
               don't forget to close the file handle.
            */
            nwl_fs_raw_close_hdle(h_file);
            
            i4_rc = NWLR_FILE_WRITE_FAIL;
        }
    }
    else
    {
        /* Translate FM err code to NWL err code. */
        i4_rc = nwl_fm_to_nwl_err_code(i4_rc);
    }        
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_get_write_hdle_no_fm_open
 *
 * Description: This API return a file handle to the RAW file, the read
 *              pointer is position at the NWL object specified by
 *              the ui2_nwl_id.
 *             
 *
 * Inputs: 
 *   ps_nwl_path  Path to the RAW file.
 *   ui2_nwl_id   Specify the NWL id to load
 *
 * Outputs: 
 *   ph_file:     File handle to the NWL object on the RAW file.
 *   pui4_beg_magic: The beginning magic number to be used for the write
 *                   operation.
 *   pb_new_pos:  Flag indicating if the file handle returned ref to an
 *                existing NWL object stored on filesystem.
 *                TRUE: the location points to by the file handle is not
 *                      a NWL object.
 *                FALSE: the location points to by the file handle contains
 *                      an existing NWL object.
 *
 * Returns: 
 *   NWLR_OK           Record is successfully added. 
 *   NWLR_NOT_FOUND    NWL object not found.
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_get_write_hdle_no_fm_open
(
    NWL_OBJ_T*         pt_nwl,
    HANDLE_T*          ph_file,
    UINT32*            pui4_beg_magic,
    BOOL*              pb_new_pos
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;
    UINT16          ui2_nwl_id;
    UINT32          ui4_obj_space_needed;
    UINT32          ui4_write_offset;

    ui2_nwl_id = pt_nwl->ui2_nwl_id;
    if (*ph_file == NULL_HANDLE)
    {
        return NWLR_INV_HANDLE;
    }
    h_file = *ph_file;

    if ( (i4_rc=fs_raw_write_lock(h_file)) == FSRAWR_OK )
    {
        /*
          Get the size of the NWL object to write to
          file
        */
        ui4_obj_space_needed=nwl_persistent_size(pt_nwl);
        

        DBG_API(("{NWL} object spaced required: %d bytes\n",ui4_obj_space_needed));


        if ( ui4_obj_space_needed > 0 )
        {   
            /* Get file offset position within the RAW file to write the
               NWL object.
            */
            i4_rc=fs_raw_realloc(h_file,
                                 (UINT32)FS_RAW_MAKE_OBJ_ID(NWL_GROUP_ID,ui2_nwl_id),
                                 ui4_obj_space_needed,
                                 &t_meta,
                                 &ui4_write_offset);
            if ( i4_rc == FSRAWR_OK )
            {
                /*
                  Get the magic number to be used for this
                  write operation, and position the file pointer
                  to the position for write operation.
                */
                if ( nwl_fs_raw_get_magic_for_write_ops(h_file,
                                                        ui4_write_offset,
                                                        pui4_beg_magic,
                                                        pb_new_pos)
                     == NWLR_OK
                    )
                {
                    *ph_file=h_file;
                    i4_rc=NWLR_OK;
                }
                else
                {
                    /*
                      File I/O error occur:
                      Don't forget to close the file handle.
                    */
                    nwl_fs_raw_close_hdle_no_fm_close(h_file);
                    
                    i4_rc = NWLR_FILE_WRITE_FAIL;
                    /*
                    ABORT(DBG_CAT_IO,DBG_ABRT_FILE_WRITE_FAIL);
                    */
                }
            }
            else
            {
                /*
                   Error condition occur:
                   Don't forget to close the file handle.
                */
                nwl_fs_raw_close_hdle_no_fm_close(h_file);
                
                /* Translate FM err code to NWL err code. */
                i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
            }
        }
        else
        {
            /*
               Object size is 0, return error code and 
               don't forget to close the file handle.
            */
            nwl_fs_raw_close_hdle_no_fm_close(h_file);
            
            i4_rc = NWLR_FILE_WRITE_FAIL;
        }
    }
    else
    {
        /* Translate FS_RAW err code to NWL err code. */
        i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
    }        
    return i4_rc;
}

/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_del
 *
 * Description: This API delete an NWL object from the RAW file.
 *             
 *
 * Inputs: 
 *   ps_nwl_path  Path to the RAW file.
 *   ui2_nwl_id   Specify the NWL id to delete.
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           NWL object on file system is deleted 
 *   NWLR_NOT_FOUND    NWL object specified is not found on file
 *                     system location
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_del
(
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_nwl_id
)
{
    INT32           i4_rc;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;

    if ( fs_raw_is_inited(h_dir, ps_nwl_path) == FALSE )
    {
        return NWLR_INV_FILE_PATH;
    }
    
    /*
      Open the RAW file.
    */
    i4_rc=x_fm_open(h_dir,
                    ps_nwl_path,
                    FM_READ_WRITE,
                    0644,
                    FALSE,
                    &h_file);

    if ( i4_rc == FMR_OK                    &&
         fs_raw_write_lock(h_file) == FSRAWR_OK
        )
    {
        i4_rc = fs_raw_del_obj(h_file, &t_meta,
                               (UINT32)(FS_RAW_MAKE_OBJ_ID(NWL_GROUP_ID,ui2_nwl_id)));

        /* Translate FSRAW err code to NWL err code. */
        i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);

        /* close the file handle. */
        nwl_fs_raw_close_hdle(h_file);
    }
    else
    {
        /* Translate FM err code to NWL err code. */
        i4_rc = nwl_fm_to_nwl_err_code(i4_rc);
    }    
    return i4_rc;
}


/*----------------------------------------------------------------------
 * Name: nwl_fs_raw_qry
 *
 * Description: This query the NWL ID and NAME of a NWL object store
 *              on the raw file.   
 *
 * Inputs: 
 *   h_nwl      Handle to the NWL object.
 *   pt_nwl_rec Pointer to the service record structure to be added. 
 *
 * Outputs: None
 *
 * Returns: 
 *   NWLR_OK           Query is success. 
 *   NWLR_INV_HANDLE   Bad NWL handle.
 *   NWLR_OUT_OF_MEM   Can not allocate memory, add record failed.
 *   NWLR_INV_ARG      'pui2_nwl_id' or 'ps_nwl_name' is a NULL.
 *   NWLR_INV_FILE_PATH specified h_dir and ps_nwl_path is not a valid RAW
 *                      file.
 -----------------------------------------------------------------------*/
INT32 nwl_fs_raw_qry (
    HANDLE_T        h_dir,
    const CHAR*	    ps_nwl_path,
    UINT16		    ui2_idx,
    UINT16*	   	    pui2_nwl_id,
    CHAR*           ps_nwl_name	
)
{
    INT32           i4_rc;
    UINT32          ui4_obj_id;
    UINT32          ui4_offset;
    UINT32          ui4_obj_sz;
    UINT16          ui2_count;
    UINT16          ui2_nb_matched;
    HANDLE_T        h_file;
    FS_RAW_META_T   t_meta;
    NWL_FILE_INFO_T t_finfo;
    
    if ( fs_raw_is_inited(h_dir, ps_nwl_path) == FALSE )
    {
        return NWLR_INV_FILE_PATH;
    }
    
    /*
      Open the RAW file.
    */
    i4_rc=x_fm_open(h_dir,
                    ps_nwl_path,
                    FM_READ_ONLY,
                    0644,
                    FALSE,
                    &h_file);

    if ( i4_rc == FMR_OK                    &&
         fs_raw_read_lock(h_file) == FSRAWR_OK 
        )
    {
        ui2_count=0;
        ui2_nb_matched=0;
        do
        { 
            /*
              Set the file offset to the NWL object found.
            */
            i4_rc=fs_raw_seek_obj_by_idx(h_file,
                                         &t_meta,
                                         ui2_count,
                                         &ui4_obj_id,
                                         &ui4_offset,
                                         &ui4_obj_sz);
            if ( i4_rc == FSRAWR_OK )
            {
                if ( FS_RAW_GET_MODULE_ID(ui4_obj_id) == NWL_GROUP_ID )
                {
                    if ( ui2_nb_matched == ui2_idx )
                    {
                        /* Read meta data of the NWL object. */
                        if ( nwl_read_file_info(h_file, &t_finfo, NULL) == NWLR_OK )
                        {
                            /* Assertion check, object id and nwl id must be the same. */
                            if (  FS_RAW_GET_ID(ui4_obj_id) == t_finfo.ui2_nwl_id )
                            {
                                *pui2_nwl_id=t_finfo.ui2_nwl_id;
                                x_strncpy(ps_nwl_name, t_finfo.ps_nwl_name,MAX_NWL_NAME_LEN);
                                i4_rc = NWLR_OK;
                                break;
                            }
                            else
                            {
                                i4_rc = NWLR_FILE_READ_FAIL;
                                break;
                                
                                /*
                                ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                                */
                            }
                        }
                        else
                        {
                            i4_rc = NWLR_FILE_READ_FAIL;
                            break;
                            
                            /*
                            ABORT(DBG_CAT_IO,DBG_ABRT_FILE_READ_FAIL);
                            */
                        }
                    }
                    else
                    {
                        ui2_nb_matched++;
                    }
                }
                ui2_count++;
            }
            else
            {
                /* Translate FSRAW err code to NWL err code. */
                i4_rc = nwl_fsraw_to_nwl_err_code(i4_rc);
                break;
            }
        } while (1);
        nwl_fs_raw_close_hdle(h_file);
    }
    else
    {
        /* Translate FM err code to NWL err code. */
        i4_rc = nwl_fm_to_nwl_err_code(i4_rc);
    }
    return i4_rc;
}


