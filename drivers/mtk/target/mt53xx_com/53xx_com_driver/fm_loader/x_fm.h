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
 * $RCSfile: x_fm.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/11 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 018d446a5d5eb2522b2487754a37aac2 $
 *
 * Description: 
 *         This header file contains File Manager exported functions.
 *---------------------------------------------------------------------------*/

#ifndef _X_FM_H_
#define _X_FM_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_fm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 x_fm_mount(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path);

extern INT32 x_fm_umount(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

extern INT32 x_fm_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info);

extern INT32 x_fm_create_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_dev,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data,
    SIZE_T          z_size);

extern INT32 x_fm_check_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    BOOL            b_auto_fix);

extern INT32 x_fm_create_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

extern INT32 x_fm_delete_dir(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path);

extern INT32 x_fm_open_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir);

extern INT32 x_fm_read_dir_entries(
	  HANDLE_T        h_dir,
    FM_DIR_ENTRY_T  *pt_dir_entry,
    UINT32          ui4_count,
    UINT32          *pui4_entries);

extern INT32 x_fm_set_dir_path(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir);

extern INT32 x_fm_get_dir_path(
	  HANDLE_T        h_dir,
    CHAR            *ps_path,
    UINT32          *pui4_len);

extern INT32 x_fm_delete_file(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path);

extern INT32 x_fm_rename(
	  HANDLE_T        h_old_dir,
    const CHAR      *ps_old_path,
    HANDLE_T        h_new_dir,
    const CHAR      *ps_new_path);

extern INT32 x_fm_open(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file);

extern INT32 x_fm_close(
    HANDLE_T        h_file);

extern INT32 x_fm_read(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_read);

extern INT32 x_fm_write(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_write);

extern INT32 x_fm_read_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req);

extern INT32 x_fm_write_async(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req);

extern INT32    x_fm_abort_async(
    HANDLE_T    h_req);
    
extern INT32    x_fm_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64          *pui8_cur_pos);

extern INT32    x_fm_seek_dir(
    HANDLE_T    h_dir,
    UINT8           ui1_whence,
    INT32           i4_range);

extern INT32 x_fm_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof);

extern INT32 x_fm_trunc_by_name(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len);

extern INT32 x_fm_trunc_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len);

extern INT32 x_fm_chop_by_name(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len);

extern INT32 x_fm_chop_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len);
        
extern INT32 x_fm_extend_file(
    HANDLE_T        h_file,
    INT64           i8_len);

extern INT32 x_fm_sync_file(
    HANDLE_T        h_file);

extern INT32 x_fm_get_info_by_name(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info);

extern INT32 x_fm_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T  *pt_info);

extern INT32 x_fm_get_dir_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_DIR_INFO_T   *pt_info);

extern INT32 x_fm_chmod_by_name(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

extern INT32 x_fm_chmod_by_handle(
    HANDLE_T        h_file,
    UINT32          ui4_mode);

extern INT32 x_fm_make_entry(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode,
    UINT16          ui2_dev,
    UINT16          ui2_unit);

extern INT32 x_fm_delete_entry(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

extern INT32 x_fm_lock(
    HANDLE_T        h_file,
    UINT32          ui4_op);

extern INT32 x_fm_unlock(
    HANDLE_T        h_file);

extern INT32 x_fm_grabbed(
    HANDLE_T        h_file,
    UINT32          ui4_flag,
    BOOL            *pb_res);

extern INT32 x_fm_get_part_ns(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32 *pui4_count);

extern INT32 x_fm_get_part_info(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32  ui4_part_idx,
    FM_PART_INFO_T *pt_part_info);

extern INT32 x_fm_parse_drive(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    const VOID *pv_data);

extern INT32 x_fm_release_drive(
    HANDLE_T h_dir,
    const CHAR *ps_path);

extern INT32 x_fm_set_time_by_name(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    TIME_T t_access_time,
    TIME_T t_modified_time);

#endif /* _X_FM_H */
