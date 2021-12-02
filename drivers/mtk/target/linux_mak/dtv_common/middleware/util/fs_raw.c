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
 * $RCSfile: fs_raw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains implementation for the mini-manager to
 *         handle I/O operation to a RAW file.
 *        
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"
#include "util/x_sort_search.h"
#include "file_mngr/x_fm.h"
#include "util/fs_raw.h"
#include "rwlock/rwl_api.h"
#include "dbg/dbg.h"

/*-------------------------------------------------------------------------
  static function prototypes
  -------------------------------------------------------------------------*/
static
INT32 fs_raw_sort_meta_entries
(
    FS_RAW_META_T*   pt_meta
);

static
INT32 fs_raw_get_obj_space
(
    FS_RAW_META_T*   pt_meta,
    UINT32           ui4_obj_id,
    UINT32*          pui4_space_used,
    UINT32*          pui4_space_avail
);

/*
  Read the RAW file header info.
*/

static 
INT32 fs_raw_read_meta
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_raw_meta
);

/*
  Write the metadata header to RAW file.
*/
static 
INT32 fs_raw_write_meta
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_raw_meta
);

/*
  Return a file position in the raw file that is large enough to
  for the specified required space. It does not check if the object
  exists already, and it does not expand the file size limit.
*/
static
INT32 fs_raw_alloc
(
    HANDLE_T         h_file,
    UINT32           ui4_obj_id,
    UINT32           ui4_space,
    FS_RAW_META_T*   pt_meta,
    UINT32*          pui4_offset
);

static
INT32 fs_raw_expand_file_limit
(
    HANDLE_T          h_file,
    FS_RAW_META_T*    pt_meta,
    UINT32            ui4_space_to_incr
);

static
INT32 fsraw_fm_err_code
(
    INT32 i4_fm_error
);

static
INT32 fsraw_create_reg_file
(
    HANDLE_T         h_dir,
    const CHAR*      ps_fpath,
    UINT32*          pui4_sz_flag,
    UINT32*          pui4_size
);

static
BOOL fsraw_check_write_lock
(
    HANDLE_T         h_file
);

static
BOOL fsraw_check_read_write_lock
(
    HANDLE_T         h_file
);

/*-----------------------------------------------------------------------------
 * Name: fsraw_get_ctrl_end_marker_crc
 *
 * Description: This internal API extracts the end-magic-number and
 *              and CRC (current not implemented, reserve for future
 *              used) for the control block.
 *
 * Inputs:  -
 *   aui1_ctrl_block   Array of 1024 bytes from RAW partition control block.
 *
 * Outputs: -
 *   pui4_end_magic_num  Magic number at the end of control block.
 *   pui4_end_crc        Reserve for future used. CRC.
 *
 * Returns: - FSRAWR_OK     Success.
 *            FSRAWR_BAD_MAGIC   File is not a raw file.
 ----------------------------------------------------------------------------*/
static
INT32 fsraw_get_ctrl_end_marker_crc
(
    UINT8*  aui1_ctrl_block,
    UINT32* pui4_end_magic_num,
    UINT32* pui4_end_crc
)
{
    aui1_ctrl_block = aui1_ctrl_block + 1016 ;

    /* Get the end magic number. */
    x_end_cnvt_from_big_32(aui1_ctrl_block, 1, pui4_end_crc);
    
    /* Reserve for future used. */
    aui1_ctrl_block = aui1_ctrl_block + 4 ;
    x_end_cnvt_from_big_32(aui1_ctrl_block, 1, pui4_end_magic_num);

    return FSRAWR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fsraw_set_ctrl_end_marker_crc
 *
 * Description: This internal API sets the end-magic-number and
 *              and CRC (current not implemented, reserve for future
 *              used).
 *
 * Inputs:  -
 *   aui1_ctrl_block   Array of 1024 bytes from RAW partition control block.
 *   ui4_end_magic_num  Magic number at the end of control block.
 *
 * Outputs: -
 *   
 *   pui4_end_crc        Reserve for future used. CRC.
 *
 * Returns: - FSRAWR_OK     Success.
 *            FSRAWR_BAD_MAGIC   File is not a raw file.
 ----------------------------------------------------------------------------*/
static
INT32 fsraw_set_ctrl_end_marker_crc
(
    UINT8*  aui1_ctrl_block,
    UINT32  ui4_end_magic_num,
    UINT32  ui4_end_crc
)
{
    UINT32   ui4_crc=777;

    /*
      Write the end magic number to location 1016 bytes in control header
      block
    */
    aui1_ctrl_block = aui1_ctrl_block + 1016 ;
    x_end_cnvt_to_big_32(&ui4_crc, 1, aui1_ctrl_block);

    /* CRC value. Reserve for future used. */
    aui1_ctrl_block = aui1_ctrl_block + 4;
    x_end_cnvt_to_big_32(&ui4_end_magic_num, 1, aui1_ctrl_block);

    return FSRAWR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fsraw_toggle_magic_num
 *
 * Description: This internal API toggle the magic number to
 *              write to the file system. (This forces the NOR FLASH
 *              to flush out the bits.)
 *
 * Inputs:  -
 *   ui4_in_magic_num  Magic number
 *   
 * Outputs: -
 *   None
 *
 * Returns: - toggled magic number value.
 ----------------------------------------------------------------------------*/
static
UINT32 fsraw_toggle_magic_num
(
    UINT32  ui4_in_magic_num
)
{
    return (~ui4_in_magic_num);
}

/*-----------------------------------------------------------------------------
 * Name: fsraw_check_magic_num(
 *
 * Description: This internal API checks the end-magic-number and
 *              and beginning magic number for file corruption.
 *              Note: the magic number toggle each time a write
 *              operation is done (this is to force NOR FLASH to
 *              to flush the partition).
 *
 * Inputs:  -
 *   ui4_end_magic_num  Magic number at the end of control block.
 *   ui4_beg_magic_num  Magic number at the beg of control block.
 *
 * Outputs: -
 *
 * Returns: -
 *   TRUE:      Magic numbers matched.
 *   FALSE:     Magic numbers don't matched.
 ----------------------------------------------------------------------------*/
static
BOOL fsraw_check_magic_num
(
    UINT32  ui4_beg_magic_num,
    UINT32  ui4_end_magic_num
)
{
    BOOL  b_rc = FALSE;
    
    if ( ui4_beg_magic_num == ((UINT32)FSRAW_MAGIC)     &&
         ui4_end_magic_num == ((UINT32)FSRAW_MAGIC)
        )
    {
        b_rc = TRUE;
    }
    
    if ( ui4_beg_magic_num == ((UINT32)(~FSRAW_MAGIC))  &&
         ui4_end_magic_num == ((UINT32)(~FSRAW_MAGIC))
        )
    {
        b_rc = TRUE;
    }
    return b_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_read_meta 
 *
 * Description: This API read the meta header in a RAW file.
 *
 * Inputs:  - 
 *
 * Outputs: - Sorted listed
 *
 * Returns: - FSRAWR_OK     Success.
 *            FSRAWR_BAD_MAGIC   File is not a raw file.
 ----------------------------------------------------------------------------*/
static
INT32 fs_raw_read_meta
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta
)
{
    UINT64           ui8_pos;
    UINT8*           aui1_buf=NULL;
    UINT8*           pui1_ptr;
    UINT32           ui4_byte_read;
    INT32            i4_i;
    INT32            i4_rc=FSRAWR_FAIL;
    UINT32           ui4_end_magic;
    UINT32           ui4_end_crc;
    
    
    aui1_buf=x_mem_alloc(FSRAW_HEADER_SIZE);
    
    if ( aui1_buf != NULL )
    {
        /*
          Position the write pointer to the beginning of the file.
        */    
        x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_pos);
        /*
          Read the RAW file meta header into a byte buffer.
        */
        i4_rc=x_fm_read(h_file,aui1_buf,FSRAW_HEADER_SIZE,&ui4_byte_read);
        if ( i4_rc == FMR_OK                   &&
             ui4_byte_read==FSRAW_HEADER_SIZE
            )
        {
            pui1_ptr=aui1_buf;

            /*
              Get the end magic number.
            */
            fsraw_get_ctrl_end_marker_crc(aui1_buf,
                                          &ui4_end_magic,
                                          &ui4_end_crc);
        
            /*
              Get the beginning magic number.
            */
            x_end_cnvt_from_big_32(pui1_ptr, 1, &(pt_meta->ui4_magic));
            pui1_ptr += 4 ;

            /* check magic number. */
            if ( fsraw_check_magic_num(pt_meta->ui4_magic,
                                       ui4_end_magic) == TRUE
                )
            {
                /* Get the RAW file size. */
                x_end_cnvt_from_big_32(pui1_ptr, 1, &(pt_meta->ui4_raw_file_sz));
                pui1_ptr += 4;

                /* Get the file info: file type indicator and version id. */
                x_end_cnvt_from_big_32(pui1_ptr, 1, &(pt_meta->ui4_finfo));
                pui1_ptr +=4;
            
                /* Get the header size. */
                x_end_cnvt_from_big_16(pui1_ptr, 1, &(pt_meta->ui2_header_sz));
                pui1_ptr += 2;

                /* Get the maximum number of entry for object descriptor. */
                x_end_cnvt_from_big_16(pui1_ptr, 1, &(pt_meta->ui2_max_nb_obj));
                pui1_ptr += 2 ;

                /* Get the number of object entry. */
                x_end_cnvt_from_big_16(pui1_ptr, 1, &(pt_meta->ui2_nb_obj));
                pui1_ptr += 2 ;

                for ( i4_i = 0 ; i4_i < FSRAW_MAX_NUM_OBJ ; i4_i++ )
                {              
                    /*
                      Read the offset address of object.
                    */
                    x_end_cnvt_from_big_32(pui1_ptr,1,
                                           &(pt_meta->at_obj[i4_i].ui4_offset));
                    pui1_ptr += 4;

                    /*
                      Read the use flag and size of the object.
                    */
                    x_end_cnvt_from_big_32(pui1_ptr,1,
                                           &(pt_meta->at_obj[i4_i].ui4_obj_sz));   
                    pui1_ptr += 4;

                    /*
                      Read the object id
                    */
                    x_end_cnvt_from_big_32(pui1_ptr,1,
                                           &(pt_meta->at_obj[i4_i].ui4_obj_id));   
                    pui1_ptr += 4;
                }
                /*
                  Sort the meta data list.with the smallest
                  offset addresses first
                */
                i4_rc=fs_raw_sort_meta_entries(pt_meta);
            }
            else
            {
                i4_rc=FSRAWR_BAD_MAGIC;
            }
        }
        else
        {
            i4_rc=FSRAWR_READ_FAIL;
        }
        x_mem_free(aui1_buf);
    }
    else
    {
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEM);
    }   
    return i4_rc;
} 

/*-----------------------------------------------------------------------------
 * Name: fs_raw_sort_cmp_fct 
 *
 * Description: This API compares two entries of the RAW file object
 *              list. It sorts the list based on the offset address
 *              of the object in the RAW file.
 *
 * Inputs:  - 
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 fs_raw_sort_cmp_fct 
(
    const VOID*  pv_elem_a,
    const VOID*  pv_elem_b,
    VOID*        pv_opt_param
)
{
    FS_RAW_OBJ_T*    pt_obj_a;
    FS_RAW_OBJ_T*    pt_obj_b;
    INT32            t_rc;

    pt_obj_a = (FS_RAW_OBJ_T*) pv_elem_a;
    pt_obj_b = (FS_RAW_OBJ_T*) pv_elem_b;

    /*
      We want the list of sort object entry such that the
      smaller offset address comes first.  The non-active
      entries, e.g., use flag equals 0, to be at the end of the list.
    */
    if ( FS_RAW_GET_OBJ_FLAG(pt_obj_a->ui4_obj_sz) >
         FS_RAW_GET_OBJ_FLAG(pt_obj_b->ui4_obj_sz)
       )
    {
        t_rc=RC_SMALLER;
    }
    else if ( FS_RAW_GET_OBJ_FLAG(pt_obj_a->ui4_obj_sz) <
              FS_RAW_GET_OBJ_FLAG(pt_obj_b->ui4_obj_sz)
        )
       
    {
        t_rc=RC_GREATER;
    }
    else
    {
        /* Compare the offset address. */
        if ( pt_obj_a->ui4_offset > pt_obj_b->ui4_offset )
        {
            t_rc=RC_GREATER;
        }
        else if ( pt_obj_a->ui4_offset < pt_obj_b->ui4_offset )
        {
            t_rc=RC_SMALLER;
        }
        else
        {
            /* two entries with same offset, for the case where
               both entries are non-active.
            */
            t_rc=RC_EQUAL;
        }
    }
    return t_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_sort_meta_entries
 *
 * Description: This API sorts the object entries in a RAW meta data
 *              header.
 *
 * Inputs:  - 
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
static
INT32 fs_raw_sort_meta_entries
(
    FS_RAW_META_T*   pt_meta
)
{
    INT32            i4_rc;
    INT32            i4_nb_obj;
    INT32            i4_elem_sz;
    FS_RAW_OBJ_T*    pt_obj;

    i4_nb_obj= (INT32) pt_meta->ui2_max_nb_obj;
    pt_obj=pt_meta->at_obj;
    i4_elem_sz= (INT32) sizeof(FS_RAW_OBJ_T);

    i4_rc=x_qsort(pt_obj,
                  (SIZE_T)i4_nb_obj,
                  (SIZE_T)i4_elem_sz,
                  fs_raw_sort_cmp_fct,
                  NULL);
    if ( i4_rc==SSR_OK )
    {
        i4_rc=FSRAWR_OK;
    }
    else
    {
        i4_rc=FSRAWR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_write_meta
 *
 * Description: API to write out the meta data  header.
 *              
 * Inputs:  - 
 *    h_file:   Handle to the RAW file.
 *    pt_meta:  Pointer to the meta data structure.
 *
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
static
INT32 fs_raw_write_meta
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta
)
{
    UINT64           ui8_pos;
    UINT8*           aui1_buf=NULL;
    UINT8*           pui1_ptr;
    UINT32           ui4_byte_wrote;
    INT32            i4_rc=FSRAWR_FAIL;
    INT32            i4_fm_rc;
    INT32            i4_i;
    UINT32           ui4_magic_num;

    if ( (aui1_buf=x_mem_alloc(FSRAW_HEADER_SIZE)) != NULL )
    {    
        i4_rc=FSRAWR_WRITE_FAIL;
        if ( (i4_fm_rc = x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_pos)) == FMR_OK )
        {
            /*
              Position the write pointer to the beginning of the file.
            */
            pui1_ptr=aui1_buf;

            /*
              toggle the magic number.
            */
            ui4_magic_num = fsraw_toggle_magic_num(pt_meta->ui4_magic);

            /*
              Pack the FS_RAW_META_T structure into an array of bytes
              with the specified endianess.
            */
            /*  beginning magic number. */
            x_end_cnvt_to_big_32(&(ui4_magic_num), 1, pui1_ptr);
            pui1_ptr += 4 ;
            
            /* RAW file size. */
            x_end_cnvt_to_big_32(&(pt_meta->ui4_raw_file_sz), 1, pui1_ptr);
            pui1_ptr += 4;
            /*
              file info: file type and version number.
            */
            x_end_cnvt_to_big_32(&(pt_meta->ui4_finfo), 1, pui1_ptr);
            pui1_ptr += 4 ;
            
            /* header size. */
            x_end_cnvt_to_big_16(&(pt_meta->ui2_header_sz), 1, pui1_ptr);
            pui1_ptr += 2;

            /* maximum number of entry for object descriptor. */
            x_end_cnvt_to_big_16(&(pt_meta->ui2_max_nb_obj), 1, pui1_ptr);
            pui1_ptr += 2 ;

            /* Number of object entry. */
            x_end_cnvt_to_big_16(&(pt_meta->ui2_nb_obj), 1, pui1_ptr);
            pui1_ptr += 2 ;

            for ( i4_i = 0 ; i4_i < FSRAW_MAX_NUM_OBJ ; i4_i++ )
            {
                /*
                  Read the offset address and the size of object.
                */
                x_end_cnvt_to_big_32(&(pt_meta->at_obj[i4_i].ui4_offset),1,
                                     pui1_ptr);
                pui1_ptr += 4;

                x_end_cnvt_to_big_32(&(pt_meta->at_obj[i4_i].ui4_obj_sz),1,
                                     pui1_ptr);
                pui1_ptr += 4;

                x_end_cnvt_to_big_32(&(pt_meta->at_obj[i4_i].ui4_obj_id),1,
                                     pui1_ptr);
                pui1_ptr += 4;
            }

            /*
              End magic number and CRC. Note: CRC is not implemented now,
              so we just use an arbitary value.
            */
            fsraw_set_ctrl_end_marker_crc(aui1_buf, ui4_magic_num, 777);
            
            /* Write out the buffer data. */
            if ( (i4_fm_rc = x_fm_write(h_file,
                            aui1_buf,
                            FSRAW_HEADER_SIZE,
                            &ui4_byte_wrote)) == FMR_OK         &&
                 ui4_byte_wrote == FSRAW_HEADER_SIZE
                )
            {
                i4_rc=FSRAWR_OK;
            }
            else
            {
                x_dbg_stmt("{FS_RAW} write failed, file-mngr error code: %d\n", i4_fm_rc);
                i4_rc = FSRAWR_WRITE_FAIL;
            }
        }
        else
        {
            x_dbg_stmt("{FS_RAW} seek failed, file-mngr error code: %d\n",i4_fm_rc);
        }
        
        x_mem_free(aui1_buf);
    }
    else
    {
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEM);
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_seek_obj
 *
 * Description: API to position the file read/write pointer at the start
 *              object in the RAW file.
 * Inputs:  -
 *   h_file:   handle to the RAW file.
 *   pt_meta:  pointer to the meta structure.
 *   ui4_obj_id:  Specifies the object ID to put the write/read pointer to.
 *
 * Outputs: -
 *   pui4_offset: the offset (from the beginning of the raw file) where
 *                the object is located.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_seek_obj
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta,
    UINT32           ui4_obj_id,
    UINT32*          pui4_offset,
    UINT32*          pui4_sz
)
{
    INT64            i8_offset;
    UINT64           ui8_pos;
    INT32            i4_i;
    INT32            i4_rc=FSRAWR_NOT_FOUND;
    
    /*
      check if read or write lock has been grabbed.
    */
    fsraw_check_read_write_lock(h_file);
    
    /*
       Read the RAW file meta data
    */
    *pui4_offset=0xFFFFFFFF;
    if ( fs_raw_read_meta(h_file, pt_meta) == FSRAWR_OK )
    {
        for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj) ; i4_i++ )
        {
            if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz)   &&
                 pt_meta->at_obj[i4_i].ui4_obj_id == ui4_obj_id )
            {            
                i8_offset = (INT64) FS_RAW_FILE_OFFSET(pt_meta,pt_meta->at_obj[i4_i].ui4_offset);

                i4_rc = x_fm_lseek(h_file, i8_offset, FM_SEEK_BGN, &ui8_pos);
                if ( i4_rc == FMR_OK && ui8_pos == ((UINT64)i8_offset) )
                {
                    *pui4_offset = (UINT32) i8_offset;
                    *pui4_sz     = (UINT32) FS_RAW_GET_OBJ_SIZE(
                        pt_meta->at_obj[i4_i].ui4_obj_sz);
                    
                    i4_rc = FSRAWR_OK ;
                }
                else
                {
                    i4_rc = FSRAWR_SEEK_FAIL ;
                }
                break;
            }
        }
    }
    else
    {
        i4_rc = FSRAWR_READ_FAIL;
    }
    return i4_rc;
}
/*-----------------------------------------------------------------------------
 * Name: fs_raw_seek_obj_by_idx
 *
 * Description: API to position the file read/write pointer at the start
 *              object in the RAW file by index count.
 * Inputs:  -
 *   h_file:   handle to the RAW file.
 *   pt_meta:  pointer to the meta structure.
 *   ui4_idx:  index count to put the write/read pointer to.
 *
 * Outputs: -
 *   pui4_obj_id: the object id corresponding to the index value.
 *   pui4_offset: the offset (from the beginning of the raw file) where
 *                the object is located.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_seek_obj_by_idx
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta,
    UINT16           ui2_idx,
    UINT32*          pui4_obj_id,
    UINT32*          pui4_offset,
    UINT32*          pui4_sz
)
{
    INT64            i8_offset;
    UINT64           ui8_pos;
    INT32            i4_rc;
    INT32            i4_i;
    UINT16           i2_count;

    /*
      check if read or write lock has been grabbed.
    */
    fsraw_check_read_write_lock(h_file);
    
    /*
       Read the RAW file meta data
    */
    *pui4_offset=0xFFFFFFFF;
    i4_rc=FSRAWR_NOT_FOUND;
    if ( fs_raw_read_meta(h_file, pt_meta) == FSRAWR_OK )
    {
        i2_count=0;
        *pui4_obj_id=0;
        for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj) ; i4_i++ )
        {
            /*
              Find an active object.
            */
            if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz) )
            {
                if ( ui2_idx == i2_count )
                {
                    i8_offset = (INT64) FS_RAW_FILE_OFFSET(pt_meta,pt_meta->at_obj[i4_i].ui4_offset);

                    i4_rc = x_fm_lseek(h_file,
                                       i8_offset,
                                       FM_SEEK_BGN,
                                       &ui8_pos);
                    if ( i4_rc == FMR_OK             &&
                         ui8_pos == (UINT64)i8_offset )
                    {
                        *pui4_obj_id = pt_meta->at_obj[i4_i].ui4_obj_id;
                        *pui4_offset = (UINT32) i8_offset;
                        *pui4_sz     = (UINT32) FS_RAW_GET_OBJ_SIZE(
                        pt_meta->at_obj[i4_i].ui4_obj_sz);
                        i4_rc=FSRAWR_OK ;
                    }
                    else
                    {
#ifdef DEBUG                        
                         x_dbg_stmt("{FS_RAW} file seek fail: %lld   sz: %d\n",
                                    i8_offset, pt_meta->at_obj[i4_i].ui4_obj_sz);
#endif                        
                        i4_rc = FSRAWR_SEEK_FAIL ;
                    }
                    break;
                }
                else
                {
                    /*
                      Increment the count value and go to the
                      next active object in the RAW file.
                    */
                    i2_count++;
                }
            }
        }
    }
    else
    {
        i4_rc = FSRAWR_READ_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_del_obj
 *
 * Description: API to delete the specified object in the RAW file.
 * Inputs:
 *   h_file:   handle to the raw file.
 *   pt_meta:  pointer to the meta
 *   ui4_obj_id:  obect id to be deleted from the RAW file.
 *
 * Outputs: None
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_del_obj
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta,
    UINT32           ui4_obj_id
)
{
    INT32            i4_rc;
    INT32            i4_i;

    /*
      check if write lock has been grabbed.
    */
    fsraw_check_write_lock(h_file);
        
    /*
       Read the RAW file meta data
    */
    i4_rc=FSRAWR_NOT_FOUND;
    if (  fs_raw_read_meta(h_file, pt_meta) == FSRAWR_OK )
    {
        /*
          Scan the RAW file to find the specified object
          to delete.
        */
        for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj) ; i4_i++ )
        {
            if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz)  &&
                 pt_meta->at_obj[i4_i].ui4_obj_id == ui4_obj_id )
            {
                pt_meta->at_obj[i4_i].ui4_offset = 0;
                pt_meta->at_obj[i4_i].ui4_obj_sz = 0;
                pt_meta->at_obj[i4_i].ui4_obj_id = 0;               
                pt_meta->ui2_nb_obj = (UINT16)(pt_meta->ui2_nb_obj - 1);
                /*
                  Update the metadata for the RAW file.
                */
                if ( fs_raw_write_meta(h_file, pt_meta) != FSRAWR_OK )
                {
                    i4_rc = FSRAWR_WRITE_FAIL;
                }
                else
                {
                    i4_rc = FSRAWR_OK;
                }
                break;
            }
        }
    }
    else
    {
        i4_rc = FSRAWR_READ_FAIL;
    }   
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_update_obj
 *
 * Description: API to update size for the existing object in the RAW file.
 * Inputs:
 *   h_file:   handle to the raw file.
 *   pt_meta:  pointer to the meta
 *   ui4_obj_id:  obect id to be deleted from the RAW file.
 *   ui4_new_obj_size:  new size required by the object.
 * Outputs: None
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
static INT32 fs_raw_update_obj
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta,
    UINT32           ui4_obj_id,
    UINT32           ui4_new_obj_size
)
{
    INT32            i4_rc;
    INT32            i4_i;

    /*
      check if write lock has been grabbed.
    */
    fsraw_check_write_lock(h_file);
        
    /*
       Read the RAW file meta data
    */
    i4_rc=FSRAWR_NOT_FOUND;
    /*
      Scan the metadata to find the specified object
      to update.
    */
    for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj) ; i4_i++ )
    {
        if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz)  &&
             pt_meta->at_obj[i4_i].ui4_obj_id == ui4_obj_id )
        {
#if 0            
            x_dbg_stmt("<FSRAW> prev object size: %d\n",
                       FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz));

            x_dbg_stmt("<FSRAW> new object size: %d\n",
                       ui4_new_obj_size);
#endif
            /*
              Write the new object size back to file system only
              if there is a difference.
            */
            if ( FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz) !=
                 ui4_new_obj_size
                )
            {
                FS_RAW_SET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz,ui4_new_obj_size); 
                /*
                  Update the metadata for the RAW file.
                */
                if ( fs_raw_write_meta(h_file, pt_meta) != FSRAWR_OK )
                {
                    i4_rc = FSRAWR_WRITE_FAIL;
                }
                else
                {
                    i4_rc = FSRAWR_OK;
                }
            }
            else
            {
                /*
                  prev object size and new object size is the same,
                  no write is need, just return OK status.
                */
                i4_rc = FSRAWR_OK;
            }
            break;
        }
    }  
    return i4_rc;
}

/*--------------------------------------------------------------------------
 * Name: fs_raw_get_obj_space
 *
 * Description: API to compute the space between the specified object's
 *              start address and the next object's start address.
 *              If this the last active object, then
 *              the space is computed from this object to the end of
 *              RAW file.
 *              It also return the space currently used by th object.
 *
 * Inputs:  - 
 *   pt_meta:  pointer to the raw file meta data.  The metadata's
 *             object descriptor table must be sorted.
 *   ui4_obj_id:  obect id to be deleted from the RAW file.
 *
 * Outputs: -
 *   pui4_space_used: space used by the object at it current location
 *                    in the RAW file.
 *   pui4_space_avail: space avaiable between this object's start address
 *                     to next object's start address (or end of the RAW
 *                     file).  In another word, this total space that
 *                     is available for this object to grow to at its
 *                     current location.
 * Returns: - None
 *            
 --------------------------------------------------------------------------*/
static
INT32 fs_raw_get_obj_space
(
    FS_RAW_META_T*   pt_meta,
    UINT32           ui4_obj_id,
    UINT32*          pui4_space_used,
    UINT32*          pui4_space_avail
)
{
    INT32            i4_rc;
    UINT32           ui4_end;
    INT32            i4_i;
    
    /*
      Scan the RAW file to find the specified object.
    */
    i4_rc=FSRAWR_NOT_FOUND;
    *pui4_space_avail=0;
    *pui4_space_used=0;
    for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj) ; i4_i++ )
    {
        if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz)  &&
             pt_meta->at_obj[i4_i].ui4_obj_id == ui4_obj_id )
        {
            *pui4_space_used =
                FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz);
            /*
              check if the next object is an active object.
            */
            if ( i4_i < (pt_meta->ui2_max_nb_obj-1)        &&
                 FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i+1].ui4_obj_sz)
                )
            {
                ui4_end = pt_meta->at_obj[i4_i+1].ui4_offset;
            }
            else
            {
                ui4_end = pt_meta->ui4_raw_file_sz - pt_meta->ui2_header_sz;
            }

            if ( ui4_end > pt_meta->at_obj[i4_i].ui4_offset )
            {
                *pui4_space_avail = ui4_end -
                    pt_meta->at_obj[i4_i].ui4_offset ;
                i4_rc=FSRAWR_OK;
            }
            else
            {
                i4_rc = FSRAWR_FAIL;
                /*
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_INV_STATE);
                */
            }
            break;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_compact
 *
 * Description: API to compact all the objects in the RAW file. After
 *              compaction, all the free space should be contiuguous.
 *
 * Inputs:  - 
 *
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
#define FS_RAW_TMP_BUF_SZ      4096
static
INT32 fs_raw_compact
(
    HANDLE_T         h_file,
    FS_RAW_META_T*   pt_meta,
    UINT32*          pui4_free_space
)
{
    INT32            i4_rc;
    INT32            i4_i;
    UINT32           ui4_byte_wrote;
    UINT32           ui4_byte_remain;
    UINT32           ui4_byte_to_read;
    UINT32           ui4_byte_read;
    UINT32           ui4_write_ptr;
    UINT32           ui4_read_ptr;
    UINT32           ui4_new_obj_addr;
    UINT8*           pui1_buf=NULL;
    UINT64           ui8_pos;

    *pui4_free_space = 0;
    /*
      Read the metadata from RAW file.
    */
    i4_rc=fs_raw_read_meta(h_file, pt_meta);
    if ( i4_rc == FSRAWR_OK )
    {
        if ( i4_rc == FSRAWR_OK )
        {
            ui4_write_ptr=0;
            /*
              Allocate temp buffer to copy the block.
            */
            pui1_buf=x_mem_alloc(FS_RAW_TMP_BUF_SZ);
            if ( pui1_buf == NULL )
            {
                ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEM);
            }
            
            for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj); i4_i++ )
            {
                if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz) )
                {
                    /* If the data block is less than the
                       write pointer, then shift the block to the
                       write pointer.
                    */
                    if ( ui4_write_ptr <
                         pt_meta->at_obj[i4_i].ui4_offset )
                    {
                        /*
                          The new offset for the block to be moved.
                        */
                        ui4_new_obj_addr=ui4_write_ptr;
                        
                        ui4_read_ptr=pt_meta->at_obj[i4_i].ui4_offset;
                        ui4_byte_remain=
                            FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz);                       
                        do
                        {
                            x_fm_lseek(h_file,
                                       (INT64) FS_RAW_FILE_OFFSET(pt_meta,ui4_read_ptr),
                                       FM_SEEK_BGN,
                                       &ui8_pos);

                            
                            if ( i4_rc == FMR_OK &&
                                 ui8_pos == FS_RAW_FILE_OFFSET(pt_meta,
                                                               ui4_read_ptr)
                                )
                            {
                                ui4_byte_to_read =
                                    ( FS_RAW_TMP_BUF_SZ >
                                      ui4_byte_remain ) ?
                                    ui4_byte_remain :
                                    FS_RAW_TMP_BUF_SZ ;
                                
                                i4_rc=x_fm_read(h_file,
                                                pui1_buf,
                                                ui4_byte_to_read,
                                                &ui4_byte_read);
                                if ( i4_rc == FMR_OK &&
                                     ui4_byte_read == ui4_byte_to_read )
                                {
                                    /* position the write pointer */
                                    i4_rc=x_fm_lseek(h_file,
                                                     (INT64) FS_RAW_FILE_OFFSET(pt_meta,ui4_write_ptr),
                                                     FM_SEEK_BGN,
                                                     &ui8_pos);

                                    
                                    if ( i4_rc==FMR_OK &&
                                         ui8_pos == FS_RAW_FILE_OFFSET(
                                                         pt_meta,
                                                         ui4_write_ptr) )
                                    {
                                        i4_rc=x_fm_write(h_file,
                                                         pui1_buf,
                                                         ui4_byte_read,
                                                         &ui4_byte_wrote);
                                        if ( i4_rc == FMR_OK   &&
                                            ui4_byte_wrote == ui4_byte_read )
                                        {
                                            /*
                                              re-compute the bytes remaining
                                            */
                                            ui4_byte_remain =
                                                ui4_byte_remain -
                                                ui4_byte_wrote;
                                            /*
                                              move the read and write pointer
                                              to the next read/write position.
                                            */
                                            ui4_write_ptr += ui4_byte_wrote;
                                            ui4_read_ptr += ui4_byte_read;
                                        }
                                        else
                                        {
                                            x_dbg_stmt("{FS_RAW} file write failed, file mngr error code: %d",
                                                       i4_rc);
                                            
                                            ABORT(DBG_CAT_IO,
                                                  DBG_ABRT_WRITE_FAIL);
                                        }
                                    }
                                    else
                                    {
                                        ABORT(DBG_CAT_IO,DBG_ABRT_SEEK_FAIL);
                                    }
                                }
                                else
                                {
                                    ABORT(DBG_CAT_IO,DBG_ABRT_READ_FAIL);
                                }
                            }
                            else
                            {
                                ABORT(DBG_CAT_IO,DBG_ABRT_SEEK_FAIL);
                            }
                        } while (ui4_byte_remain > 0 ) ;
                        /*
                          update the meta data with new start address
                          of this block that  has been moved.
                        */
                        pt_meta->at_obj[i4_i].ui4_offset=ui4_new_obj_addr;
                    }
                    else
                    {
                        /* Assertion check: if the write_ptr is greater
                           than the read ptr, something is wrong.
                        */
                       if ( ui4_write_ptr >
                            pt_meta->at_obj[i4_i].ui4_offset )
                       {
                           ABORT(DBG_CAT_IO,DBG_ABRT_INV_STATE);
                       }

                       /*
                         Move the write pointer to the end of
                         this object.
                       */
                       if ( ui4_write_ptr ==
                            pt_meta->at_obj[i4_i].ui4_offset )
                       {
                           ui4_write_ptr = ui4_write_ptr +
                               FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz);                          
                       }
                    }
                }
                else
                {
                    /* Assertion check: Move all the active objects
                       into a contigous location in the raw file.
                       Any obj entries higher than the number of
                       used entries must have the used flag set to 0.
                    */
                    if ( (i4_i) >= ((INT32)pt_meta->ui2_nb_obj) )
                    {
                        break;
                    }
                    else
                    {
                        ABORT(DBG_CAT_IO,DBG_ABRT_INV_STATE);
                    }
                }
            }  /* End of the for loop */
            x_mem_free(pui1_buf);
        }
        else
        {
            ABORT(DBG_CAT_IO,DBG_ABRT_INV_STATE);
        }
    }
    else
    {
        ABORT(DBG_CAT_IO,DBG_ABRT_READ_FAIL);
    }
    /*
      Write the new meta data back into RAW file.
    */
    i4_rc = fs_raw_write_meta(h_file, pt_meta);
    if ( i4_rc != FSRAWR_OK )
    {
        ABORT(DBG_CAT_IO,DBG_ABRT_WRITE_FAIL);
    }

    /* compute the free space. */
    if ( pt_meta->ui4_raw_file_sz >
         (  pt_meta->ui2_header_sz +
            pt_meta->at_obj[pt_meta->ui2_nb_obj-1].ui4_offset +
            FS_RAW_GET_OBJ_SIZE(
                pt_meta->at_obj[pt_meta->ui2_nb_obj-1].ui4_obj_sz)
         )
       )
    {
        *pui4_free_space = pt_meta->ui4_raw_file_sz - 
        ( pt_meta->ui2_header_sz +
          pt_meta->at_obj[pt_meta->ui2_nb_obj-1].ui4_offset +
          FS_RAW_GET_OBJ_SIZE(
              pt_meta->at_obj[pt_meta->ui2_nb_obj-1].ui4_obj_sz)
        );
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_alloc
 *
 * Description: API to allocate a block of contiguous space within
 *              the RAW file.  This function does not check if the
 *              specified object id is already used.
 *
 * Inputs:  -
 *   h_file:       handle to the raw file.
 *   ui4_obj_id:   object id.
 *   ui4_space:    the size of contiguous block to allocate.
 *
 * Outputs: -
 *   pt_meta:      Pointer to the meta structure.
 *   
 *   pui4_offset:  the position, e.g., bytes offset from the beginning
 *                 in RAW file for allocate space.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
static
INT32 fs_raw_alloc
(
    HANDLE_T         h_file,
    UINT32           ui4_obj_id,
    UINT32           ui4_space,
    FS_RAW_META_T*   pt_meta,
    UINT32*          pui4_offset
)
{
    INT32            i4_rc;
    INT32            i4_i;
    INT32            i4_new;
    INT32            i4_start_pos;
    UINT32           ui4_free_space;
    UINT32           ui4_total_free;
    
    /*
      Read the metadata from RAW file.
    */
    i4_rc=fs_raw_read_meta(h_file, pt_meta);
    if ( i4_rc == FSRAWR_OK )
    {
        if ( pt_meta->ui2_nb_obj == pt_meta->ui2_max_nb_obj )
        {
            /* no more slot available. */
            i4_rc=FSRAWR_OUT_OF_SPACE;
        }
        else
        {
            ui4_total_free=0;
            /* iterate through the blocks and look for a block
               to fit the request file size.
            */
            i4_start_pos = 0;
            for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj); i4_i++ )
            {
                /*
                  Check for the space between the end of
                  this block and start of the next block.
                */
                if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz) )
                {
                    ui4_free_space=
                        pt_meta->at_obj[i4_i].ui4_offset -
                        i4_start_pos;
                    /*
                      check if the space between two
                      used block is big enough to fit
                      the new data in.
                    */
                    if ( ui4_free_space >= ui4_space )
                    {
                        /* update the metadata. */
                        i4_new=(INT32)pt_meta->ui2_nb_obj;                      
                        pt_meta->at_obj[i4_new].ui4_offset = 
                            (UINT32) i4_start_pos ;
                        
                        FS_RAW_SET_OBJ_SIZE(pt_meta->at_obj[i4_new].ui4_obj_sz,ui4_space);                        
                        pt_meta->at_obj[i4_new].ui4_obj_id =
                            ui4_obj_id;
                        pt_meta->ui2_nb_obj = (UINT16)(pt_meta->ui2_nb_obj
                                                       + 1);

                        /* compute the offset from the beginning
                           of the RAW file.
                        */

                        *pui4_offset = FS_RAW_FILE_OFFSET(pt_meta,pt_meta->at_obj[i4_new].ui4_offset);
                        i4_rc=FSRAWR_OK;
                        break;
                    }
                    else
                    {
                        /*
                          Keep track of the empty space
                          skip over because it is not
                          big enought to fit the new data
                          block.
                        */
                        ui4_total_free += ui4_free_space;
                    }
                }
                /*
                  If next block is not used, then check the
                  space between the end of this block and
                  end of the RAW file
                */
                else
                {
                    /*
                      Assertion check: we already sort the
                      list of block entries, the number of
                      used block must equals to the number of
                      block checked so far.
                    */
                    if ( (i4_i) != ((INT32)pt_meta->ui2_nb_obj) )
                    {
                        ABORT(DBG_CAT_INV_OP,DBG_ABRT_INV_STATE);
                    }
                    /* Compute the space between end of
                       this block to the end of the RAW file.
                    */
                    ui4_free_space =
                        (pt_meta->ui4_raw_file_sz -
                         pt_meta->ui2_header_sz) -
                        i4_start_pos;
                    if ( ui4_free_space >=  ui4_space )
                    {
                        /* update the metadata. */
                        i4_new=(INT32)pt_meta->ui2_nb_obj;                     
                        pt_meta->at_obj[i4_new].ui4_offset =
                            (UINT32)i4_start_pos ;
                        
                        FS_RAW_SET_OBJ_SIZE(pt_meta->at_obj[i4_new].ui4_obj_sz,ui4_space);                       
                        pt_meta->at_obj[i4_new].ui4_obj_id =
                            ui4_obj_id;
                        pt_meta->ui2_nb_obj = (UINT16)(pt_meta->ui2_nb_obj
                                                       + 1);

                        /* compute the offset from the beginning
                           of the RAW file.
                        */
                        *pui4_offset = FS_RAW_FILE_OFFSET(pt_meta,pt_meta->at_obj[i4_new].ui4_offset);
                        
                        i4_rc=FSRAWR_OK;
                        break;
                    }
                    else
                    {
                        /*
                          Now we have scan all the block, if we still
                          have not find a free space big enough to
                          store the data, now see if we need to
                          compact the block.
                        */
                        /*
                          Compute the total amount of free space
                          left in the RAW file.
                        */
                        ui4_total_free += ui4_free_space;
                        if ( ui4_total_free >= ui4_space )
                        {
                            /*
                              Compact the blocks and
                              try again.
                            */
                            fs_raw_compact(
                                h_file,
                                pt_meta,
                                &ui4_free_space);

                            /*
                              Check the sum of blocks of free space
                              before compaction and after compaction
                              must be the same.
                            */
                            if ( ui4_free_space == ui4_total_free )
                            {
                                i4_rc=fs_raw_alloc(
                                    h_file,
                                    ui4_obj_id,
                                    ui4_space,
                                    pt_meta,
                                    pui4_offset);
                                break;
                            }
                            else
                            {
                                ABORT(DBG_CAT_IO,DBG_ABRT_INV_STATE);
                            }
                        }
                        else
                        {
                            i4_rc=FSRAWR_OUT_OF_SPACE;
                            break;
                        }    
                    }
                }
                /*
                  move to the end address of this used object.
                  Re-set the start position for the next iteration.
                */
                i4_start_pos=(INT32)(pt_meta->at_obj[i4_i].ui4_offset +
                                     FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz));
            }
        }  /* End of finding position in the RAW file to write. */        
    }
    if ( i4_rc == FSRAWR_OK )
    {
        i4_rc = fs_raw_write_meta(h_file, pt_meta);
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_realloc
 *
 * Description: API to allocate a block of contiguous space within
 *              the RAW file for the specified object id.
 *
 *              If the specified object id exists, and there is sufficent
 *              space available at the existing location to accomodate the
 *              requested space, then the current position of the object
 *              within the raw file is returned. If there is not sufficent
 *              space available at the current object's location, but
 *              sufficient free space is available at another location in
 *              the raw file (through defragmentation if necessary), then
 *              existing object is deleted, the defragmentation is performed
 *              to consolidate the free space, and the new location of a
 *              contiguous block in the raw file large enough to accomodate
 *              the requested space  is returned.
 *
 *              If specified object id does not existed, then search
 *              through the raw file, perform defragmentation if necessary
 *              and return the start location of a contiguous space to
 *              hold the requested space.
 *             
 *
 * Inputs:  -
 *   h_file:       handle to the raw file.
 *   ui4_obj_id:   object id.
 *   ui4_space_requested:  the size of contiguous block to allocate.
 *
 * Outputs: -
 *   pt_meta:      Pointer to the meta structure.
 *   
 *   pui4_offset:  the position, e.g., bytes offset from the beginning
 *                 in RAW file for allocate space.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_realloc
(
    HANDLE_T         h_file,
    UINT32           ui4_obj_id,
    UINT32           ui4_space_requested,
    FS_RAW_META_T*   pt_meta,
    UINT32*          pui4_offset
)
{
    INT32           i4_rc;
    UINT32          ui4_space_avail;
    UINT32          ui4_space_used;
    UINT32          ui4_total_free_space;
    UINT32          ui4_total_used_space;
    UINT32          ui4_space_to_incr;
    UINT32          ui4_obj_sz;
    UINT64          ui8_pos;

    /*
      check if write lock has been grabbed.
    */
    fsraw_check_write_lock(h_file);
    
    *pui4_offset=0xFFFFFFFF;
    /*
      Determine if the specified object exists in the RAW file.
    */
    i4_rc= FSRAWR_NOT_FOUND;
    if ( fs_raw_seek_obj(h_file, pt_meta, ui4_obj_id, pui4_offset, &ui4_obj_sz)
         == FSRAWR_OK )
    {
        /*
          The object is found, we need determine if enough space
          is avaiable for over-writing this object at its current
          location in the RAW file.
        */
        fs_raw_get_obj_space(pt_meta,
                             ui4_obj_id,
                             &ui4_space_used,
                             &ui4_space_avail);
            
        if ( ui4_space_avail >= ui4_space_requested )
        {
            /*
              Update the metadata block with the new space used
              by the object.
            */
            fs_raw_update_obj(h_file, pt_meta, ui4_obj_id, ui4_space_requested);

            i4_rc = x_fm_lseek(h_file, (INT64)(*pui4_offset), FM_SEEK_BGN, &ui8_pos);
            if ( i4_rc == FMR_OK  )
            {
                /* Done, the h_file is at the location for the
                   write operation.
                */
                i4_rc = FSRAWR_OK;
            }
            else
            {
                i4_rc = FSRAWR_SEEK_FAIL ;
            }
        }
        else
        {
            /* Since there is not enough space to write in the
               existing location of the object, check if we can
               delete the existing  object first, compact the raw
               file to consolidate the space, and then write the
               object to another location in the RAW file.
            */
            fs_raw_get_space_info(pt_meta, &ui4_total_free_space, &ui4_total_used_space);
            if ( (ui4_total_free_space+ui4_space_used) >=
                 ui4_space_requested )
            {
                /*
                  Delete the object at its existing location,
                  so we can compact the free space.
                  Set the return code to object not found.
                */
                fs_raw_del_obj(h_file, pt_meta, ui4_obj_id);
            }
            else
            {
                /* We don't have enough space, see if the file
                   limit can be expaned.
                */
                ui4_space_to_incr=ui4_space_requested-ui4_total_free_space
                    -ui4_space_used;
                if ( fs_raw_expand_file_limit(h_file,
                                              pt_meta,
                                              ui4_space_to_incr)
                     == FSRAWR_OK
                    )
                {
                    /*
                       Delete the object at its existing location,
                       There now should be enough space to store
                       the object.
                    */
                    fs_raw_del_obj(h_file, pt_meta, ui4_obj_id);
                }
                else
                {
                    /* No space to fit the update this object. */
                    i4_rc = FSRAWR_OUT_OF_SPACE;
                }
            }
        }
    }
    else
    {
        /* Adding a new object to raw file, we first determine
           if there is sufficient space to accomodate the object.
           We will calculate how much more space is need and
           then try to xpand the storage space if possible.
           If file limit can not be expanded, then i4_rc is
           set to OUT_OF_SPACE status.
        */
        fs_raw_get_space_info(pt_meta, &ui4_total_free_space, &ui4_total_used_space);
        if ( ui4_total_free_space < ui4_space_requested )
        {
            ui4_space_to_incr=ui4_space_requested-ui4_total_free_space;
            if ( fs_raw_expand_file_limit(h_file,
                                          pt_meta,
                                          ui4_space_to_incr) != FSRAWR_OK
                )
            {
                i4_rc=FSRAWR_OUT_OF_SPACE;
            }
        }
    }   
        
    if ( i4_rc == FSRAWR_NOT_FOUND )
    {
        /*
          No object found (or specified object deleted to free up
          space for compaction) . Look for a location where we
          can write the new SVL object.
        */
        i4_rc=fs_raw_alloc(h_file,
                           ui4_obj_id,
                           ui4_space_requested,
                           pt_meta,
                           pui4_offset);
    }
    return i4_rc;
}

static INT32 fs_raw_zero_file_system(HANDLE_T h_file)
{
    FM_FILE_INFO_T t_info;
    INT32 i4_rc = 0;
    FM_FLASH_PART_BULK_WRITE_T t_ioctl_info;

    i4_rc = x_fm_get_info_by_handle(h_file, &t_info);
    if (i4_rc != FMR_OK)
    {
        x_dbg_stmt("{FS_RAW} Error:fs_raw_zero_file_system(0x%x),can not get file info.\n",(int)h_file);
        return FSRAWR_FAIL;
    }
    
     /* block device */
    if (!FM_IS_BLK(t_info.ui4_mode))
    {
        x_dbg_stmt("{FS_RAW} Error:fs_raw_zero_file_system(0x%x), not block device.\n",(int)h_file);
        return FSRAWR_INV_ARG;
    }

    t_ioctl_info.ui1_value = 0xff;
    t_ioctl_info.ui4_offset = 0;
    t_ioctl_info.ui4_size = (UINT32)t_info.ui8_size;

    i4_rc = x_fm_io_ctrl(h_file,FM_IO_CTRL_FLASH_BULK_WRITE, &t_ioctl_info);
    if(i4_rc != FMR_OK)
    {
        x_dbg_stmt("{FS_RAW} Error:fs_raw_zero_file_system(0x%x), erase block device error.\n",(int)h_file);
        return FSRAWR_FAIL;
    }

    return FSRAWR_OK;

}
/*-----------------------------------------------------------------------------
 * Name: fs_raw_init
 *
 * Description: API to initialize the RAW file.
 *
 * Inputs:  - 
 *   ps_file_path:    File pathname to the raw file.
 *   ui2_max_nb_obj:  Maximum number of objects that can be stored in
 *                    this raw file.
 *
 * Outputs: - None.
 *
 * Returns: - 
 *   FSRAWR_OK    raw file is inited.
 *   FSRAWR_FAIL  error opening 'ps_file_path'. File does not exit or
 *                it is an invalid pathname.
 *   FSRAWR_OUT_OF_SPACE:  The raw file does not contain enough space to
 *                     store the header for the meta data of raw file, plus
 *                     some minimum space for the data.
 ----------------------------------------------------------------------------*/

#define  FSRAW_INIT_BUF_SIZE      (16*1024)

INT32 fs_raw_init
(
    HANDLE_T            h_dir,
    const CHAR*         ps_file_path
)
{
    INT32            i4_rc;
    //INT32            i4_fm_rc;
    FS_RAW_META_T    t_meta;
    HANDLE_T         h_file;
    UINT32           ui4_sz_flag;
    UINT32           ui4_sz;
    //INT32            i4_byte_left;
    //UINT32           ui4_byte_wrote;
    //UINT32           ui4_byte_to_write;
    //UINT8*           pui1_zero_buf = NULL;
    FS_RAW_FTYPE     e_ftype;
    
    i4_rc = fs_raw_qry_fpath(h_dir, ps_file_path, &e_ftype, &ui4_sz_flag, &ui4_sz);
    /*
      If the file does not exist, then try to create a new file.
      This only work if the filepath specified resided on a regular
      file system.
    */
    if ( i4_rc == FSRAWR_NOT_FOUND )
    {
        i4_rc=fsraw_create_reg_file(h_dir,
                                    ps_file_path,
                                    &ui4_sz_flag,
                                    &ui4_sz);
        e_ftype=FSRAW_FTYPE_REGULAR;
    }
 
    if ( i4_rc == FSRAWR_OK )
    {
        i4_rc=x_fm_open(h_dir,ps_file_path,FM_READ_WRITE,0644,FALSE,&h_file);
        if ( i4_rc == FMR_OK )
        {
            if ( fs_raw_write_lock(h_file) == FSRAWR_OK )
            {
                /*
                   Zero out the FS_RAW_META_T structure.
                */
                x_memset(&t_meta, 0, sizeof(t_meta));   
                
                /*
                  Set the fields of meta data sturcture
                  to initial values.
                */
                t_meta.ui4_magic        = FSRAW_MAGIC;
                FS_RAW_SET_FILE_FLAG(t_meta.ui4_finfo,ui4_sz_flag);
                FS_RAW_SET_FILE_VER(t_meta.ui4_finfo,FSRAW_NULL_VER);
                t_meta.ui2_max_nb_obj   = FSRAW_MAX_NUM_OBJ;
                t_meta.ui2_header_sz    = FSRAW_HEADER_SIZE;
                t_meta.ui4_raw_file_sz  = ui4_sz;
            
                /*
                  Check that the RAW file has at least sufficient space
                  to store the meta header + some padding.
                */
                if ( ((UINT32)(t_meta.ui2_header_sz+FSRAW_MIN_DATA_SIZE))
                     <= t_meta.ui4_raw_file_sz )
                {
#if 0                    
                    /*
                      zero out the filesystem.
                    */
                    {

                        i4_rc = FSRAWR_OK;

                        i4_byte_left = (INT32)t_meta.ui4_raw_file_sz ;
                        
                        /*
                          write out 0 to the file system partition.
                        */
                        pui1_zero_buf = x_mem_alloc(FSRAW_INIT_BUF_SIZE);
                        if ( pui1_zero_buf != NULL )
                        {
                            /*
                               Set the write pattern (0 or 0xFF) based on
                               storage type.
                            */
                            if ( e_ftype == FSRAW_FTYPE_NOR )
                            {
                                x_memset(pui1_zero_buf, 0xFF, FSRAW_INIT_BUF_SIZE);
                            }
                            else
                            {
                                x_memset(pui1_zero_buf, 0, FSRAW_INIT_BUF_SIZE);
                            }
                            
                            ui4_byte_to_write = FSRAW_INIT_BUF_SIZE;

                            while (i4_byte_left > 0)
                            {
                                if ( ((UINT32)i4_byte_left) < ui4_byte_to_write )
                                {
                                    ui4_byte_to_write = (UINT32)i4_byte_left;
                                }
                                
                                if ( (i4_fm_rc=x_fm_write(h_file, pui1_zero_buf, ui4_byte_to_write,
                                                &ui4_byte_wrote)) == FMR_OK               &&
                                     ui4_byte_wrote == ui4_byte_to_write
                                    )
                                {
                                    i4_byte_left -= ui4_byte_wrote;
                                }
                                else
                                {
                                    x_dbg_stmt("{FS_RAW} Can not initialize storage: file-mngr error code: [%d]\n",
                                               i4_fm_rc);
                                    i4_rc = FSRAWR_FAIL;
                                    break;
                                }
                            }
                            x_mem_free(pui1_zero_buf);
                        }
                        else
                        {
                            i4_rc = FSRAWR_FAIL;
                        }
                    }
#endif

                    if ( i4_rc == FSRAWR_OK )
                    {
                        i4_rc = fs_raw_write_meta(h_file, &t_meta);
                        if ( i4_rc == FSRAWR_OK )
                        {
                            x_dbg_stmt("{FS_RAW} initialize storage [%s], size is [%d] bytes.\n",
                                       ps_file_path, t_meta.ui4_raw_file_sz );
                        }
                        else
                        {
                            x_dbg_stmt("{FS_RAW} retry to initialize storage [%s].\n",ps_file_path);
                            i4_rc = fs_raw_zero_file_system(h_file);
                            if(i4_rc == FSRAWR_OK)
                            {
                                i4_rc = fs_raw_write_meta(h_file, &t_meta);
                                if ( i4_rc == FSRAWR_OK )
                                {
                                    x_dbg_stmt("{FS_RAW} [Retry]initialize storage [%s] success.\n",ps_file_path);
                                }
                            }                            
                        }
                    }
                }
                else
                {
                    i4_rc = FSRAWR_OUT_OF_SPACE ;
                }
                    
                /* Unlock the file. */
                fs_raw_unlock(h_file);
            }
            else
            {
                x_dbg_stmt("{FS_RAW} Error: can not lock the filesystem %s for initialization.\n",
                           ps_file_path);
            }
            
            x_fm_close(h_file);

        }
        else
        {
            /* Translate FM error to FSRAW error. */
            i4_rc=fsraw_fm_err_code(i4_rc);    
        }
    }  
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_is_inited
 *
 * Description: API to check if a specified RAW file has been inited.
 *              This check is done by examing the mini-control block of
 *              the raw file system (fisrt 1024 bytes), and see if
 *              the beginning and ending magic marker is valid. 
 * Inputs:  - 
 *   ps_file_path:  File pathname to the raw file.
 *   
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
BOOL fs_raw_is_inited
(
    HANDLE_T            h_dir,
    const CHAR*         ps_file_path
)
{
    BOOL             b_rc;
    HANDLE_T         h_file;
    UINT32           ui4_magic;
    UINT32           ui4_end_magic;
    UINT32           ui4_beg_magic;
    UINT32           ui4_byte_read;
    UINT64           ui8_pos;

    b_rc=FALSE;
    if (  x_fm_open(h_dir,
                    ps_file_path,
                    FM_READ_WRITE,
                    0644,
                    FALSE,
                    &h_file) == FMR_OK )
    {
        if ( fs_raw_read_lock(h_file) == FSRAWR_OK )
        {                
            /*
              Read the begining magic number of the control block. 
            */
            if ( x_fm_read(h_file, &(ui4_magic),
                           sizeof(ui4_magic), &ui4_byte_read)    == FMR_OK  &&
                 ui4_byte_read == sizeof(ui4_magic) )
            {
                x_end_cnvt_from_big_32(&ui4_magic, 1, &ui4_beg_magic);
                /*
                  Read the end magic number of the control block,
                  which is located at 1020 bytes.
                */
                if ( x_fm_lseek(h_file, 1020,
                                FM_SEEK_BGN, &ui8_pos)           == FMR_OK  &&
                     x_fm_read(h_file, &(ui4_magic),
                               sizeof(ui4_magic),&ui4_byte_read) == FMR_OK  &&
                     ui4_byte_read == sizeof(ui4_magic)
                    )
                {
                    x_end_cnvt_from_big_32(&ui4_magic, 1, &ui4_end_magic);
                    /*
                      Compare the magic numbers to determine if this
                      raw file has been initialized.
                    */
                    if ( fsraw_check_magic_num(ui4_beg_magic,
                                               ui4_end_magic) == TRUE )
                    {
                        b_rc = TRUE;
                    }
                }
            }
            /* Unlock the file. */
            fs_raw_unlock(h_file);
        }
        x_fm_close(h_file);
    }
    return b_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_is_inited_ex
 *
 * Description: API to check if a specified RAW file has been inited.
 *              This check is done by examing the mini-control block of
 *              the raw file system (fisrt 1024 bytes), and see if
 *              the beginning and ending magic marker is valid. 
 * Inputs:  - 
 *   ps_file_path:  File pathname to the raw file.
 *   
 * Outputs: -
 *   ph_file:       File handle of the opened file if RAW file has been inited.
 *                  If RAW file has not been inited, the file handle is invalid.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
BOOL fs_raw_is_inited_ex
(
    HANDLE_T            h_dir,
    const CHAR*         ps_file_path,
    UINT32              ui4_flags,
    HANDLE_T*           ph_file
)
{
    BOOL             b_rc;
    HANDLE_T         h_file;
    UINT32           ui4_magic;
    UINT32           ui4_end_magic;
    UINT32           ui4_beg_magic;
    UINT32           ui4_byte_read;
    UINT64           ui8_pos;

    b_rc=FALSE;
    if (  x_fm_open(h_dir,
                    ps_file_path,
                    ui4_flags,
                    0644,
                    FALSE,
                    &h_file) == FMR_OK )
    {
        if ( fs_raw_read_lock(h_file) == FSRAWR_OK )
        {                
            /*
              Read the begining magic number of the control block. 
            */
            if ( x_fm_read(h_file, &(ui4_magic),
                           sizeof(ui4_magic), &ui4_byte_read)    == FMR_OK  &&
                 ui4_byte_read == sizeof(ui4_magic) )
            {
                x_end_cnvt_from_big_32(&ui4_magic, 1, &ui4_beg_magic);
                /*
                  Read the end magic number of the control block,
                  which is located at 1020 bytes.
                */
                if ( x_fm_lseek(h_file, 1020,
                                FM_SEEK_BGN, &ui8_pos)           == FMR_OK  &&
                     x_fm_read(h_file, &(ui4_magic),
                               sizeof(ui4_magic),&ui4_byte_read) == FMR_OK  &&
                     ui4_byte_read == sizeof(ui4_magic)
                    )
                {
                    x_end_cnvt_from_big_32(&ui4_magic, 1, &ui4_end_magic);
                    /*
                      Compare the magic numbers to determine if this
                      raw file has been initialized.
                    */
                    if ( fsraw_check_magic_num(ui4_beg_magic,
                                               ui4_end_magic) == TRUE )
                    {
                        b_rc = TRUE;
                    }
                }
            }
            /* Unlock the file. */
            fs_raw_unlock(h_file);
        }
        if (b_rc == TRUE && ph_file != NULL)
        {
            *ph_file = h_file;
        }
        else
        {
            x_fm_close(h_file);
        }
    }
    return b_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_get_space_info
 *
 * Description: API retruns the free space available in the RAW file and
 *              space used by all the data object in the raw file.
 *
 * Inputs:  - 
 *   ps_file_path:  File pathname to the raw file.
 *   
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_get_space_info
(
    FS_RAW_META_T*   pt_meta,
    UINT32*          pui4_free_space,
    UINT32*          pui4_used_space
)
{
    UINT32           ui4_total_space_used;
    INT32            i4_i;
    /*
      Read the metadata from RAW file.
    */
    *pui4_free_space = 0 ;
    ui4_total_space_used = 0 ; 
    /*
      Loop through each object try and sum up the space
      of each active object.
    */
    for ( i4_i=0; i4_i < ((INT32)pt_meta->ui2_max_nb_obj); i4_i++ )
    {
        if ( FS_RAW_OBJ_IS_ACTIVE(pt_meta->at_obj[i4_i].ui4_obj_sz) )
        {
            ui4_total_space_used = ui4_total_space_used +
                FS_RAW_GET_OBJ_SIZE(pt_meta->at_obj[i4_i].ui4_obj_sz);
        }
    }

    *pui4_used_space = ui4_total_space_used ;
    
    /*
      subtract off the header size and total space used to the
      the left-over free space.
    */
    if ( pt_meta->ui4_raw_file_sz >=
         (pt_meta->ui2_header_sz + ui4_total_space_used )
        )
    {
        *pui4_free_space = pt_meta->ui4_raw_file_sz -
            ( pt_meta->ui2_header_sz + ui4_total_space_used );
    }
    
    return FSRAWR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_qry_fpath
 *
 * Description: API to query the type of file for the storage. If the file
 *              is a raw file, the the size of file is fixed. If the file
 *              is a regular file, then size is dynamic, it can grows until
 *              no more free space is available in the file system. 
 *
 * Inputs:  - 
 *   ps_file_path:  File pathname to the raw file.
 *   
 * Outputs: -
 *   pui4_flag:  contains the file flag (whether the size is fixed or
 *               can be extended). 
 *   pui4_size:  Size of the file. 
 * Returns: -
 *   FSRAWR_OK:  success
 *   FSRAWR_NOT_FOUND:      Specified dir and/or file does not exist.
 *   FSRAWR_INV_FILE_PATH:  can not open the specified path. The reason
 *                          might be due to bad directory path, or
 *                          lack of permission.
 *            
 ----------------------------------------------------------------------------*/

INT32 fs_raw_qry_fpath
(
    HANDLE_T         h_dir,
    const CHAR*      ps_fpath,
    FS_RAW_FTYPE*    pe_ftype,
    UINT32*          pui4_sz_flag,
    UINT32*          pui4_size
)
{
    INT32            i4_rc;
    FM_FILE_INFO_T   t_file;
    HANDLE_T         h_file;
    UINT64           ui8_pos;
    
    *pui4_sz_flag = 0;
    *pui4_size=0;
    *pe_ftype = FSRAW_FTYPE_UNKNOWN;

    i4_rc = x_fm_get_info_by_name(h_dir, ps_fpath, &t_file);
    
    if ( i4_rc == FMR_OK )
    {
        if ( FM_IS_FILE(t_file.ui4_mode) )
        /*
          File is on a regular filesystem.
        */
        {       
            *pui4_sz_flag = FSRAW_DYNAM_FILE_SZ;
            *pui4_size=(UINT32)t_file.ui8_size;
            /*
              Dynamic size. If the file size is samller than
              the minimum require size, then set the file
              limit to the initial default initial value.
              Later, the file limit can be increased as needed. 
            */
            if ( *pui4_size < (FSRAW_MIN_DATA_SIZE + FSRAW_HEADER_SIZE) )
            {
                *pui4_size = FSRAW_MIN_DATA_SIZE + FSRAW_HEADER_SIZE ;
            }
            *pe_ftype = FSRAW_FTYPE_REGULAR;
            i4_rc = FSRAWR_OK;
        }
        /*
          File is on RAW partition.
        */
        else if ( FM_IS_CHR(t_file.ui4_mode) || FM_IS_BLK(t_file.ui4_mode) )
        {
            *pui4_sz_flag = FSRAW_FIXED_FILE_SZ;
            /*
              Get the raw file size.  Use the x_fm_lseek()
            */
            if ( x_fm_open(h_dir,
                           ps_fpath,
                           FM_READ_ONLY,
                           0644,
                           FALSE,
                           &h_file) == FMR_OK )
            {
                if ( x_fm_lseek(h_file, 0, FM_SEEK_END, &ui8_pos)
                     == FMR_OK )
                {
                    *pui4_size=(UINT32)ui8_pos;
                    *pe_ftype = FSRAW_FTYPE_NOR;
                    i4_rc = FSRAWR_OK;
                }
                else
                {
                    i4_rc = FSRAWR_FAIL;
                }
                x_fm_close(h_file);
            }
            else
            {
                i4_rc = FSRAWR_FAIL;
            }
        }
        else
        {
            i4_rc = FSRAWR_INV_FILE_PATH;
        }
    }
    else
    {
        if ( i4_rc == FMR_NO_ENTRY )
        {
            i4_rc = FSRAWR_NOT_FOUND;
        }
    }  
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_expand_file_limit
 *
 * Description: API to increase the file limit. 
 *
 * Inputs:  - 
 *   h_file:  File handle to the raw file.
 *   
 * Outputs: -
 *   pui4_flag:  contains the file flag (whether the size is fixed or
 *               can be extended). 
 *   pui4_size:  Size of the file. 
 * Returns: -
 *
 *            
 ----------------------------------------------------------------------------*/
static
INT32 fs_raw_expand_file_limit
(
    HANDLE_T          h_file,
    FS_RAW_META_T*    pt_meta,
    UINT32            ui4_space_to_incr
)
{
    INT32        i4_rc;

    i4_rc=FSRAWR_FAIL;
    /*
      Read the metadata from RAW file.
    */
    if ( fs_raw_read_meta(h_file, pt_meta) == FSRAWR_OK )
    {
        /* if the file is on a RAW partition, then
           we can not increase the limit.
        */
        if ( FS_RAW_GET_FILE_FLAG(pt_meta->ui4_finfo)
             == FSRAW_DYNAM_FILE_SZ
            )
        {
            /* expand the file. */
            /*[DTV00211318]Update the call to x_fe_extend_file for the reason that the 
                        implement of this interface has been changed
                    */
            if ( x_fm_extend_file(h_file, (INT64)(pt_meta->ui4_raw_file_sz + ui4_space_to_incr)) == FMR_OK )
            {
                /*
                  Update the size of file of file.
                */
                pt_meta->ui4_raw_file_sz += ui4_space_to_incr;
                if ( fs_raw_write_meta(h_file, pt_meta) != FSRAWR_OK )
                {
                    ABORT(DBG_CAT_IO,DBG_ABRT_WRITE_FAIL);
                }
                /*
#ifdef DEBUG                       
                x_dbg_stmt("File limit increased, new size is: %d\n",
                           pt_meta->ui4_raw_file_sz);
#endif                              
                */
                i4_rc = FSRAWR_OK;
            }
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_read_lock
 *
 * Description: API to lock the raw file for read operation.
 *
 * Inputs:  - 
 *   h_file:  File handle to the raw file.
 *   
 * Outputs: -
 *   None.
 *
 * Returns: -
 *
 *            
 ----------------------------------------------------------------------------*/

INT32 fs_raw_read_lock
(
    HANDLE_T          h_file
)
{
    INT32             i4_rc=FSRAWR_OK;
    if ( x_fm_lock(h_file, FM_LOCK_READ) != FMR_OK )
    {
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_LOCK_FAIL);  
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_write_lock
 *
 * Description: API to lock the raw file for write operation.
 *
 * Inputs:  - 
 *   h_file:  File handle to the raw file.
 *   
 * Outputs: -
 *   None.
 *
 * Returns: -
 *
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_write_lock
(
    HANDLE_T          h_file
)
{
    INT32             i4_rc=FSRAWR_OK;
    if ( x_fm_lock(h_file, FM_LOCK_WRITE) != FMR_OK )
    {
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_LOCK_FAIL);
    } 
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_unlock
 *
 * Description: API to unlock (read or write) the raw file.
 *
 * Inputs:  - 
 *   h_file:  File handle to the raw file.
 *   
 * Outputs: -
 *   None.
 *
 * Returns: -
 *
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_unlock
(
    HANDLE_T          h_file
)
{
    INT32             i4_rc=FSRAWR_OK;
    if ( x_fm_unlock(h_file) != FMR_OK )
    {
        ABORT(DBG_CAT_SEMAPHORE,DBG_ABRT_LOCK_FAIL);
    }    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fsraw_fm_err_code
 *
 * Description: API to translate File Manager error code to FSRAW error code.
 *             
 *
 * Inputs:  - i4_fm_error: File Manager API error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: - tanslated FSRAWR error code.   
 *                        
 ----------------------------------------------------------------------------*/
static
INT32 fsraw_fm_err_code(INT32 i4_fm_error)
{
    INT32 i4_rc;
    
    switch (i4_fm_error)
    {
        case FMR_OK:
            i4_rc=FSRAWR_OK;
            break;

        case FMR_ARG:
            i4_rc=FSRAWR_INV_ARG;
            break;

        case FMR_NO_ENTRY:
            i4_rc=FSRAWR_NOT_FOUND;
            break;
            
        case FMR_NAME_TOO_LONG:
            i4_rc=FSRAWR_INV_FILE_PATH;
            break;
            
        case FMR_INVAL:
            i4_rc=FSRAWR_INV_FILE_PATH;
            break;
            
        case FMR_NO_SUCH_DEVICE:
            i4_rc=FSRAWR_INV_FILE_PATH;
            break;
            
        default:
            i4_rc=FSRAWR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fsraw_create_reg_file
 *
 * Description: API to create a new file with a initial default size for I/O.
 *              Assuming the path given resolved into a regular file system.
 *
 * Inputs:  -
 *   h_dir:   directory handle where the new file is created.
 *   ps_fpath: specified the pathname of the new file.
 *   pui4_sz_flag:  Flag indicating this file is a variable size.
 *   pui4_size:     The current size of the file.
 *
 * Outputs: - None.
 *                           
 * Returns: - 
 *   FSRAWR_OK   Success
 *   FSRAWR_FAIL Can not create a new file.
 ----------------------------------------------------------------------------*/
static
INT32 fsraw_create_reg_file
(
    HANDLE_T         h_dir,
    const CHAR*      ps_fpath,
    UINT32*          pui4_sz_flag,
    UINT32*          pui4_size
)
{
    INT32            i4_rc;
    HANDLE_T         h_file;
    UINT64           ui8_pos;
    UINT32           ui4_wrote;
    FS_RAW_FTYPE     e_ftype;

    i4_rc = FSRAWR_FAIL;
    
    if ( x_fm_open(h_dir,
                   ps_fpath,
                   FM_OPEN_CREATE | FM_READ_WRITE,
                   0644,
                   FALSE,
                   &h_file) == FMR_OK
        )
    {
        if ( fs_raw_write_lock(h_file) == FSRAWR_OK )
        { 
            /*
              write 4 bytes at the end of this file,
              this will set the file to an initial default size.
            */
            if ( x_fm_lseek(h_file,
                            (INT64)(FSRAW_MIN_DATA_SIZE+FSRAW_HEADER_SIZE-sizeof(i4_rc)),
                            FM_SEEK_BGN,
                            &ui8_pos)    == FMR_OK                    &&
                 x_fm_write(h_file,
                            &i4_rc,
                            sizeof(i4_rc),
                            &ui4_wrote)   == FMR_OK                   &&
                 (ui4_wrote) == sizeof(i4_rc)
                )
            {
                
                /* Unlock the file. */
                fs_raw_unlock(h_file);
                x_fm_close(h_file);
                i4_rc=fs_raw_qry_fpath(h_dir,ps_fpath,&e_ftype,pui4_sz_flag,pui4_size);
            }
            else
            {
                /* Unlock the file. */
                fs_raw_unlock(h_file);
                /* seek or write fail, close and delete the file.  */
                x_fm_close(h_file);
                x_fm_delete_file(h_dir,ps_fpath);
            }
        }
        
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fsraw_check_read_write_lock
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
BOOL fsraw_check_read_write_lock
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
 * Name: fsraw_check_write_lock
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
BOOL fsraw_check_write_lock
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
 * Name: fs_raw_seek_beg_magic_for_write_ops
 *
 * Description: API to read the beginning magic value (first 4 bytes)
 *              at the location of an object. Note: if this is a new location,
 *              then the it would return 0 for the beginning magic number. 
 *
 *              On return from this API, the file pointer will
 *              be positioned at the beginning of the object.
 *
 * Inputs:  -
 *   h_file:   handle to the file. On return, It is positioned at the
 *             start of the object on the file system.
 *
 *   ui4_obj_offset: The byte offset of the object within the file.
 *
 *
 * Outputs: -
 *   pui4_beg_magic_num: pointer to a variable containing the beginning
 *                       magic value..
 *                           
 * Returns: - 
 *   FSRAWR_OK   The begining-magic value is read.
 *   FSRAWR_READ_FAIL  File system read operation fail.
 ----------------------------------------------------------------------------*/
INT32 fs_raw_seek_beg_magic_for_write_ops
(
    HANDLE_T          h_file,
    UINT32            ui4_obj_offset,
    UINT32*           pui4_beg_magic_num
)
{
    UINT64  ui8_fpos;
    UINT32  ui4_byte_read;
    UINT8   aui1_buf[4] ;
    UINT32  ui4_magic;
    INT32   i4_rc;
    INT32   i4_frc;
    
    
    ui4_magic = 0;
    *pui4_beg_magic_num = 0;
    i4_rc = FSRAWR_READ_FAIL;
    
    /*
      check if read or write lock has been grabbed.
    */
    fsraw_check_read_write_lock(h_file);
    
    /*
      Set the File pointer to the start of the object in the file.
    */
    i4_frc=x_fm_lseek(h_file, (INT64)ui4_obj_offset, FM_SEEK_BGN, &ui8_fpos);
    
    if ( i4_frc == FMR_OK   &&
         ((UINT64)ui4_obj_offset) == ui8_fpos )
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
              the start of the object position.
            */
            if (  x_fm_lseek(h_file, (INT64)ui4_obj_offset,
                             FM_SEEK_BGN, &ui8_fpos) == FMR_OK
                )
            {
                *pui4_beg_magic_num = ui4_magic;
                i4_rc = FSRAWR_OK;
#ifdef DEBUG
                x_dbg_stmt("{FS_RAW} object position in raw file: %d begin magic at that location: %x\n",
                       ui4_obj_offset, *pui4_beg_magic_num);
#endif                    
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
                if (  x_fm_lseek(h_file, (INT64)ui4_obj_offset,
                                 FM_SEEK_BGN, &ui8_fpos) == FMR_OK
                )
                {
                    *pui4_beg_magic_num = 0;
                    i4_rc = FSRAWR_OK;
#ifdef DEBUG
                    x_dbg_stmt("{FS_RAW} object position in raw file: %d begin magic at that location: %x\n",
                       ui4_obj_offset, *pui4_beg_magic_num);
#endif   
                }
            }
        }   
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_get_obj_beg_end_magic_num
 *
 * Description: API to find the the beginning and ending magic number of
 *              an existing object.
 *
 *              After return from this API, the file pointer will be
 *              positioned at the beginning of the object.
 *
 * Inputs:  -
 *   h_file:   handle to the file. It is position at the start of the
 *             object.  
 *
 *   ui4_sz:   Specifies the size (in bytes) of the object.
 *
 * Outputs: -
 *
 *   pui4_beg_magic_num: pointer to a variable holding the begining
 *                       magic value.
 *
 *   pui4_end_magic_num: pointer to a variable holding the end magic
 *                       value.
 *                           
 * Returns: - 
 *   DBR_OK   The end-magic value is read.
 *   DBR_FILE_READ_FAIL   read operation failed.
 ----------------------------------------------------------------------------*/
INT32 fs_raw_get_obj_beg_end_magic_num
(
    HANDLE_T          h_file,
    UINT32            ui4_sz,
    UINT32*           pui4_beg_magic_num,
    UINT32*           pui4_end_magic_num
)
{
    UINT64  ui8_fpos;
    UINT64  ui8_magic_fpos;
    UINT64  ui8_end_fpos;
    UINT32  ui4_byte_read;
    UINT8   aui1_buf[4];
    UINT32  ui4_beg_magic;
    UINT32  ui4_end_magic;
    INT32   i4_rc;
    
    
    ui4_beg_magic = 0;
    ui4_end_magic = 0;
    *pui4_end_magic_num = 0;
    *pui4_beg_magic_num = 0;
    
    i4_rc = FSRAWR_READ_FAIL;
    
    /*
      Remember the current location of the File pointer.
    */
    if ( x_fm_lseek(h_file, 0, FM_SEEK_CUR, &ui8_fpos) == FMR_OK )
    {
        /* x_dbg_stmt("{FS_RAW} read begin magic at %lld\n",ui8_fpos); */
        
        /*
          Read the first 4 bytes
        */
        if ( x_fm_read(h_file, aui1_buf, sizeof(aui1_buf),
                       &ui4_byte_read)  == FMR_OK               &&
             ui4_byte_read == sizeof(aui1_buf)
            )
        {
            /* convert from big endian. */
            x_end_cnvt_from_big_32(&(aui1_buf[0]), 1, &ui4_beg_magic);

            /*
              Compute the position of the end-magic number. The
              end-magic is size of the object - 8 bytes
              from the current position.  Note:
              the current position is 4 bytes from the beginning of
              of the object
            */
            ui8_end_fpos = ui4_sz - 8 ;
        
            /*
              goto the end of object to read the end-magic
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
                    x_end_cnvt_from_big_32(&(aui1_buf[0]), 1, &ui4_end_magic);
                    *pui4_beg_magic_num = ui4_beg_magic;
                    *pui4_end_magic_num = ui4_end_magic;

                    /* x_dbg_stmt("{FS_RAW} read end magic at %lld\n",ui8_magic_fpos); */
                    /*
                      Don't forget to re-position the file pointer to
                      the original position.
                    */
                    if (  x_fm_lseek(h_file, (INT64)ui8_fpos,
                                     FM_SEEK_BGN, &ui8_end_fpos) == FMR_OK
                        )
                    {
                        i4_rc = FSRAWR_OK;
                    }
                }
            }
        }
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: fs_raw_finfo
 *
 * Description: API print out info about the RAW file 
 *
 * Inputs:  - 
 *   h_file:  Handle to the file 
 *   
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_get_finfo
(
    HANDLE_T         h_dir,
    const CHAR*      ps_fpath
)
{
    HANDLE_T          h_file;
    INT32             i4_rc;
    FS_RAW_META_T     t_meta;
    UINT32            ui4_free_space;
    UINT32            ui4_used_space;
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
    INT32             i4_i;
#endif    
    
    i4_rc=x_fm_open(h_dir, ps_fpath, FM_READ_ONLY, 0644, FALSE, &h_file);
    if ( i4_rc == FMR_OK )
    {
        if ( fs_raw_read_lock(h_file) == FSRAWR_OK )
        { 
            /*
              Get the metadata for this raw file.
            */
            if ( fs_raw_read_meta(h_file, &t_meta) == FSRAWR_OK )
            {
                /*
                  compute the free space and used spaced
                */
                fs_raw_get_space_info(&t_meta,
                                      &ui4_free_space,
                                      &ui4_used_space);
            
                /* Unlock the file. */
                fs_raw_unlock(h_file);
                i4_rc=FSRAWR_OK;
#if defined(CLI_LVL_ALL) || defined(CLI_LVL_GUEST) || defined(CLI_STRESS_MODE)
                /*
                  print out information about the raw file.
                */
                x_dbg_stmt("Raw file info for %s \n", ps_fpath);
                x_dbg_stmt("    total size:                            %d bytes\n", t_meta.ui4_raw_file_sz);
                x_dbg_stmt("    meta data header size:                 %d bytes\n", t_meta.ui2_header_sz) ;
                x_dbg_stmt("    max nb of objects that can be stored:  %d\n", t_meta.ui2_max_nb_obj) ;
                x_dbg_stmt("    nb of data objects:                    %d\n", t_meta.ui2_nb_obj) ;
                x_dbg_stmt("    total space used by data objects:      %d bytes\n", ui4_used_space);
                x_dbg_stmt("    total free space:                      %d bytes\n", ui4_free_space);
                x_dbg_stmt("    percent of space used:                 %d %\n",
                           (int)((ui4_used_space*100)/(t_meta.ui4_raw_file_sz-t_meta.ui2_header_sz)));
                
                for ( i4_i=0; (i4_i < t_meta.ui2_max_nb_obj) && (i4_i < FSRAW_MAX_NUM_OBJ); i4_i++ )
                {
                    if ( FS_RAW_OBJ_IS_ACTIVE(t_meta.at_obj[i4_i].ui4_obj_sz) )
                    {
                        /* If the data block is less than the
                           write pointer, then shift the block to the
                           write pointer.
                        */
                        x_dbg_stmt("[%d] File obj id [%d] position in filesys: [%d], size: [%d bytes]\n",
                                   i4_i,
                                   t_meta.at_obj[i4_i].ui4_obj_id,
                                   t_meta.at_obj[i4_i].ui4_offset,
                                   FS_RAW_GET_OBJ_SIZE(t_meta.at_obj[i4_i].ui4_obj_sz));
                    }
                }       
#endif                
            }
            else
            {
                /* Unlock the file. */
                fs_raw_unlock(h_file);
                i4_rc = FSRAWR_FAIL;
            }
        }
        else
        {
            i4_rc = FSRAWR_FAIL;
        }
        /* close the file handle.  */
        x_fm_close(h_file); 
    }
    else
    {
        i4_rc = FSRAWR_INV_FILE_PATH ;
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: fs_raw_copy
 *
 * Description: API copy RAW file to another path
 *
 * Inputs:  - 
 *   h_file:  Handle to the file 
 *   
 * Outputs: - None.
 *
 * Returns: - None
 *            
 ----------------------------------------------------------------------------*/
INT32 fs_raw_copy
(
    HANDLE_T         h_dir,
    const CHAR*      ps_fpath,
    HANDLE_T         h_dir_dst,
    const CHAR*      ps_fpath_dst
)
{
    HANDLE_T          h_file;
    HANDLE_T          h_file_dst;
    INT32             i4_rc = FSRAWR_OK;
    FS_RAW_META_T     t_meta;
    UINT32            ui4_bytes_left;
    UINT32            ui4_bytes_to_read;
    UINT32            ui4_bytes_read;
    UINT32            ui4_bytes_wrote;
    UINT32            ui4_buf_size = 16384;
    UINT64            ui8_pos;
    UINT8*            pui1_buffer;
    
    i4_rc=x_fm_open(h_dir, ps_fpath, FM_READ_ONLY, 0644, FALSE, &h_file);
    if ( i4_rc == FMR_OK )
    {
        if ( fs_raw_read_lock(h_file) == FSRAWR_OK )
        { 
            /*
              Get the metadata for this raw file.
            */
            if ( fs_raw_read_meta(h_file, &t_meta) == FSRAWR_OK )
            {
                x_fm_lseek(h_file, 0, FM_SEEK_BGN, &ui8_pos);
                i4_rc=x_fm_open(h_dir_dst, ps_fpath_dst, FM_WRITE_ONLY | FM_OPEN_CREATE, 0644, FALSE, &h_file_dst);
                if ( i4_rc == FMR_OK )
                {
                    fs_raw_write_lock(h_file_dst);
                    ui4_bytes_left = t_meta.ui4_raw_file_sz;

                    pui1_buffer = (UINT8 *) x_mem_alloc(ui4_buf_size);
                    if (pui1_buffer == NULL)
                    {
                        ABORT(DBG_CAT_MEMORY,DBG_ABRT_OUT_OF_MEM);
                    }

                    while (ui4_bytes_left > 0)
                    {
                        ui4_bytes_to_read = ui4_bytes_left > ui4_buf_size ? ui4_buf_size : ui4_bytes_left;
                        i4_rc = x_fm_read(h_file, pui1_buffer, ui4_bytes_to_read, &ui4_bytes_read);
                        if ( i4_rc == FMR_OK/* && ui4_bytes_read == ui4_bytes_to_read */)
                        {
                            if (ui4_bytes_read == 0)
                            {
                                break;
                            }
                            if (ui4_bytes_read != ui4_bytes_to_read)
                            {
                            }
                            i4_rc = x_fm_write(h_file_dst,
                                               pui1_buffer,
                                               ui4_bytes_read,
                                               &ui4_bytes_wrote);
                            if ( i4_rc == FMR_OK && ui4_bytes_wrote == ui4_bytes_read)
                            {
                                ui4_bytes_left -= ui4_bytes_read;
                            }
                            else
                            {
                                i4_rc = FSRAWR_FAIL;
                                break;
                            }
                        }
                        else
                        {
                            i4_rc = FSRAWR_FAIL;
                            break;
                        }
                    }
                    x_mem_free(pui1_buffer);

                    fs_raw_unlock(h_file_dst);
                    x_fm_close(h_file_dst);
                    /* Unlock the file. */
                    fs_raw_unlock(h_file);
                }
                else
                {
                    /* Unlock the file. */
                    fs_raw_unlock(h_file);
                    i4_rc = FSRAWR_FAIL;
                }
            }
            else
            {
                /* Unlock the file. */
                fs_raw_unlock(h_file);
                i4_rc = FSRAWR_FAIL;
            }
        }
        else
        {
            i4_rc = FSRAWR_FAIL;
        }
        /* close the file handle.  */
        x_fm_close(h_file); 
    }
    else
    {
        i4_rc = FSRAWR_INV_FILE_PATH ;
    }
    
    if (i4_rc != FSRAWR_OK)
    {
        x_dbg_stmt("fs_raw_copy failed with %d.\r\n", i4_rc);
    }
    else
    {
        x_dbg_stmt("Done.\r\n");
    }
    return i4_rc;
}
