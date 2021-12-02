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
/*----------------------------------------------------------------------------*
 * $RCSfile: c_fm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: a27cebe0fbe15d93e4806e8f05bf9b45 $
 *
 * Description: 
 *         This file contains File Manager exported API's for AEE.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "file_mngr/x_fm.h"
#include "aee/aee.h"
#include "unicode/u_uc_str.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
                                                                               
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: c_fm_mount
 *
 * Description: The c_fm_mount function attaches the file system on device
 *              ps_dev to the directory ps_path. File Manager will 
 *              automatically recognize the file system type.
 *
 * Inputs:  h_dev_dir
 *              Handle of the directory label relative to ps_dev_path.
 *          ps_dev_path
 *              Path of the block device special file.
 *          h_mp_dir
 *              Handle of the directory label relative to ps_mp_path.
 *          ps_mp_path
 *              Path of the directory to mount. (Mount point)
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_dev_path or ps_mp_path is NULL.
 *          FMR_HANDLE
 *              h_dev_dir or h_mp_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_dev_path or ps_mp_path is an invalid path.
 *              Or file system is not supported.
 *          FMR_NO_ENTRY
 *              ps_dev_path or ps_mp_path is not found.
 *          FMR_BUSY
 *              ps_dev_path is already mounted.
 *          FMR_NOT_DIR
 *              A component name in ps_dev_path or in ps_mp_path or ps_mp_path
 *              itself is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_dev_path or ps_mp_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_mount(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path)
{
    return x_fm_mount(h_dev_dir,
                      ps_dev_path,
                      h_mp_dir,
                      ps_mp_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_mount_ex
 *
 * Description: The c_fm_mount function attaches the file system on device
 *              ps_dev to the directory ps_path. File Manager will 
 *              automatically recognize the file system type.
 *
 * Inputs:  h_dev_dir
 *              Handle of the directory label relative to ps_dev_path.
 *          ps_dev_path
 *              Path of the block device special file.
 *          h_mp_dir
 *              Handle of the directory label relative to ps_mp_path.
 *          ps_mp_path
 *              Path of the directory to mount. (Mount point)
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_dev_path or ps_mp_path is NULL.
 *          FMR_HANDLE
 *              h_dev_dir or h_mp_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_dev_path or ps_mp_path is an invalid path.
 *              Or file system is not supported.
 *          FMR_NO_ENTRY
 *              ps_dev_path or ps_mp_path is not found.
 *          FMR_BUSY
 *              ps_dev_path is already mounted.
 *          FMR_NOT_DIR
 *              A component name in ps_dev_path or in ps_mp_path or ps_mp_path
 *              itself is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_dev_path or ps_mp_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_mount_ex(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path,
    FM_MNT_PARM_T   *pt_mnt_parm)
{
    return x_fm_mount_ex(h_dev_dir,
                         ps_dev_path,
                         h_mp_dir,
                         ps_mp_path,
                         pt_mnt_parm);
}

INT32 c_fm_attach_file_system(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path)
{
    return x_fm_attach_file_system(h_dev_dir,
                      ps_dev_path,
                      h_mp_dir,
                      ps_mp_path);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_umount
 *
 * Description: Detach a mounted file system.
 *
 * Inputs: h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Mount point.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 ----------------------------------------------------------------------------*/
INT32 c_fm_umount(
    HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    return x_fm_umount(h_dir, ps_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_fs_info
 *
 * Description: Get a mounted file system information.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              A file/directory path name in the file system.
 *
 * Outputs: pt_fs_info
 *              Reference to file system information object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_fs_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NO_ENTRY
 *              The directory component in ps_path or ps_path does not exist.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NOT_DIR
 *              The directory component in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info)
{
    return x_fm_get_fs_info(h_dir, ps_path, pt_fs_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_create_fs
 *
 * Description: Create(Format) a file system on the specified device.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_dev
 *              Device special file.
 *          e_fs_type
 *              File system type.
 *          pv_data
 *              Private data for individual file system.
 *          z_size
 *              Size of pv_data in bytes.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_dev is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NO_ENTRY
 *              ps_dev does not exist.
 *          FMR_INVAL
 *              The specified file system is not supported or ps_dev is not
 *              a special file for block device.
 *          FMR_BUSY
 *              The device is busy.
 *          FMR_DEVICE_ERROR
 *              Driver/device level I/O error.
 *          FMR_NOT_DIR
 *              The directory component in ps_dev is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_dev is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_create_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_dev,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data,
    SIZE_T          z_size)
{
    return x_fm_create_fs(h_dir,
                          ps_dev,
                          e_fs_type,
                          pv_data,
                          z_size);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_check_fs
 *
 * Description: Check the integrity of a file system.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              A file/directory path name in the file system.
 *          b_auto_fix
 *              Set to TRUE to fix it when the file system has errors.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 ----------------------------------------------------------------------------*/
INT32 c_fm_check_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    BOOL            b_auto_fix)
{
    return x_fm_check_fs(h_dir, ps_path, b_auto_fix);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_create_dir
 *
 * Description: Create a new directory with ui4_mode.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              name or a path. Note that if the first character of
 *              ps_path is '/', h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *          ui4_mode
 *              Access permission of the new directory.
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_EXIST
 *              ps_path already exists.
 *          FMR_NOT_DIR
 *              A component name in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_create_dir(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    return x_fm_create_dir(h_dir, ps_path, ui4_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_delete_dir
 *
 * Description: Delete an empty directory.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the entry to be removed.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path or ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_DIR_NOT_EMPTY
 *              The directory contains file or directory.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_delete_dir(
	HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    return x_fm_delete_dir(h_dir, ps_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_delete_dir_ex
 *
 * Description: Delete a non-empty directory.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the entry to be removed.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path or ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_DIR_NOT_EMPTY
 *              The directory contains file or directory.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/

INT32 c_fm_delete_dir_ex(
	HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    return x_fm_delete_dir_ex(h_dir, ps_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_open_dir
 *
 * Description: Open a directory.
 *
 * Inputs:  h_dir
 *              Handle of the directory label.
 *          ps_path
 *              Path name of the directory to be opened.
 *
 * Outputs: ph_dir
 *              Handle of the opened directory.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path or ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_open_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir)
{
    HANDLE_T  h_aux = NULL_HANDLE;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return FMR_HANDLE;
    }

    i4_ret = x_fm_open_dir(h_dir, ps_path, ph_dir);
    if (i4_ret == FMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_dir);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_open_dir_ex
 *
 * Description: Open a directory.
 *
 * Inputs:  h_dir
 *              Handle of the directory label.
 *          ps_path
 *              Path name of the directory to be opened.
 *
 * Outputs: ph_dir
 *              Handle of the opened directory.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path or ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_open_dir_ex(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir,
    UINT64          ui8_ofst)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return FMR_HANDLE;
    }

    i4_ret = x_fm_open_dir_ex(h_dir, ps_path, ph_dir, ui8_ofst);
    if (i4_ret == FMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_dir);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_read_dir_entries
 *
 * Description: Read entries in a directory.
 *
 * Inputs:  h_dir
 *              Handle of an opened directory.
 *          ui4_count
 *              Number of elements that pt_dir_entry can hold.
 *
 * Outputs:
 *          pt_dir_entry
 *              Reference to an array of FM_DIR_ENTRY_T objects.
 *          pui4_entries
 *              The actual number of elements read in the pt_dir_entry.
 *
 * Returns: FMR_OK
 *              success.
 *          FMR_ARG
 *              pt_dir_entry is NULL or pui4_entries is NULL or ui4_count is 0.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              h_dir is being closed.
 *          FMR_NOT_DIR
 *              h_dir is not a directory.
 *          FMR_EOF
 *              End of File, no more entries.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_read_dir_entries(
    HANDLE_T        h_dir,
    FM_DIR_ENTRY_T  *pt_dir_entry,
    UINT32          ui4_count,
    UINT32          *pui4_entries)
{
    return x_fm_read_dir_entries(h_dir,
                                 pt_dir_entry,
                                 ui4_count,
                                 pui4_entries);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_set_dir_path
 *
 * Description: Set a label to the specified directory. One can use this
 *              function to set a reference to a directory, so that the
 *              underneath UFS can start finding a file from this directory
 *              without re-searching from the root. It hence provides the
 *              similar ability to the current working directory, but it's
 *              more flexible.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of a directory to be set.
 *
 * Outputs: ph_dir
 *              Reference to a handle of directory label.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL or ph_dir is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_set_dir_path(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir)
{
    HANDLE_T  h_aux = NULL_HANDLE;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return FMR_HANDLE;
    }

    i4_ret = x_fm_set_dir_path(h_dir, ps_path, ph_dir);
    if (i4_ret == FMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_dir);
    }
    else
    {
        aee_release_handle_resource(1);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_dir_path
 *
 * Description: Get the path name of a directory label.
 *
 * Inputs:  h_dir
 *              The directory label.          
 *          pui4_len
 *              Size of ps_path in bytes;
 *
 * Outputs: ps_path
 *              The path name of directory label h_dir.
 *          pui4_len
 *              If return code is FMR_NOT_ENOUGH_SPACE, this value contains
 *              the actual required buffer size in bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL or pui4_len is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NOT_ENOUGH_SPACE
 *              Length of ps_path is not large enough.
 *          FMR_CORE
 *              Fail to lock the directory entry.
 ----------------------------------------------------------------------------*/
INT32 c_fm_get_dir_path(
	HANDLE_T        h_dir,
    CHAR            *ps_path,
    UINT32          *pui4_len)
{
    return x_fm_get_dir_path(h_dir, ps_path, pui4_len);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_delete_file
 *
 * Description: Delete a file.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the file to be removed.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path or ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_delete_file(
	HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    return x_fm_delete_file(h_dir, ps_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_rename
 *
 * Description: Change the name of a file/directory or move a file/directory.
 *
 * Inputs:  h_old_dir
 *              Handle of the source directory label.
 *          ps_old_path
 *              Path name of the source entry. 
 *          h_new_dir
 *              Handle of the destination directory label.
 *          ps_new_path
 *              Path name of the destination entry.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_old_path or ps_new_path is NULL.
 *          FMR_HANDLE
 *              h_old_dir or h_new_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_old_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_old_path is not found.
 *          FMR_EXIST
 *              ps_new_path already exists.
 *          FMR_NOT_DIR
 *              A component name in ps_old_path or in ps_new_path is not
 *              a directory.
 *          FMR_IS_DIR
 *              ps_new_path exists and is a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of source or destination path is too long.
 *          FMR_OTHER_FS
 *              Source and destination are not the same file system.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_rename(
    HANDLE_T        h_old_dir,
    const CHAR      *ps_old_path,
    HANDLE_T        h_new_dir,
    const CHAR      *ps_new_path)
{
    return x_fm_rename(h_old_dir,
                       ps_old_path,
                       h_new_dir,
                       ps_new_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_open
 *
 * Description: 
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the file to be opened.
 *          ui4_flags
 *              Flags for open. User can specify these flags by bitwise 'OR'
 *              operation. The valid flags are as follows:
 *              FM_READ_ONLY tells File Manager that the file is opened
 *              for read only, and any write operation will get a FMR_PERM_DENY.
 *              FM_WRITE_ONLY tells File Manager that the file is opend
 *              for write only, and any read operation will get a FMR_PERM_DENY.
 *              FM_READ_WRITE tells File Manager that the file is opened
 *              for both read and write.
 *              If none of above flags is selected, the default is read only.
 *              If more than one is set, FM_READ_WRITE supersedes
 *              FM_WRITE_ONLY and FM_WRITE_ONLY supersedes FM_READ_ONLY.
 *              FM_OPEN_CREATE tells File Manager to create the specified
 *              file if it does not exist.
 *              FM_OPEN_TRUNC tells File Manager to truncate all contents of
 *              the specified file. The starting offset will be zero
 *              FM_OPEN_APPEND tells File Manager to set the starting offset
 *              to the end of the file.
 *              FM_OPEN_EXCLUDE tells File Manager to exclusively create a
 *              file. It fails if the file already exists.
 *              
 *          ui4_mode
 *              UNIX style access permission. The valid values are as
 *              follows:
 *              FM_MODE_USR_READ
 *              FM_MODE_USR_WRITE
 *              FM_MODE_USR_EXEC
 *              FM_MODE_GRP_READ
 *              FM_MODE_GRP_WRITE
 *              FM_MODE_GRP_EXEC
 *              FM_MODE_OTH_READ
 *              FM_MODE_OTH_WRITE
 *              FM_MODE_OTH_EXEC
 *          b_ext_buf
 *              Use the external buffer. If it is TRUE, File Manager will
 *              NOT cache it. However, the buffer address must be aligned
 *              to specific address due to the hardware's limitation.
 *
 * Outputs: ph_file
 *              Handle of the opened file.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_old_path or ps_new_path is NULL.
 *          FMR_HANDLE
 *              h_old_dir or h_new_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_old_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_old_path is not found.
 *          FMR_EXIST
 *              ps_new_path already exists.
 *          FMR_NOT_DIR
 *              A component name in ps_old_path or in ps_new_path is not
 *              a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of source or destination path is too long.
 *          FMR_OTHER_FS
 *              Source and destination are not the same file system.
 *          FMR_DEVICE_ERROR
 *              Device open error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_open(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file)
{
    HANDLE_T  h_aux = NULL_HANDLE;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return FMR_HANDLE;
    }

    i4_ret = x_fm_open(h_dir,
                       ps_path,
                       ui4_flags,
                       ui4_mode,
                       b_ext_buf,
                       ph_file);
    if (i4_ret == FMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_file);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_open_ex
 *
 * Description: 
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the file to be opened.
 *          ui4_flags
 *              Flags for open. User can specify these flags by bitwise 'OR'
 *              operation. The valid flags are as follows:
 *              FM_READ_ONLY tells File Manager that the file is opened
 *              for read only, and any write operation will get a FMR_PERM_DENY.
 *              FM_WRITE_ONLY tells File Manager that the file is opend
 *              for write only, and any read operation will get a FMR_PERM_DENY.
 *              FM_READ_WRITE tells File Manager that the file is opened
 *              for both read and write.
 *              If none of above flags is selected, the default is read only.
 *              If more than one is set, FM_READ_WRITE supersedes
 *              FM_WRITE_ONLY and FM_WRITE_ONLY supersedes FM_READ_ONLY.
 *              FM_OPEN_CREATE tells File Manager to create the specified
 *              file if it does not exist.
 *              FM_OPEN_TRUNC tells File Manager to truncate all contents of
 *              the specified file. The starting offset will be zero
 *              FM_OPEN_APPEND tells File Manager to set the starting offset
 *              to the end of the file.
 *              FM_OPEN_EXCLUDE tells File Manager to exclusively create a
 *              file. It fails if the file already exists.
 *              
 *          ui4_mode
 *              UNIX style access permission. The valid values are as
 *              follows:
 *              FM_MODE_USR_READ
 *              FM_MODE_USR_WRITE
 *              FM_MODE_USR_EXEC
 *              FM_MODE_GRP_READ
 *              FM_MODE_GRP_WRITE
 *              FM_MODE_GRP_EXEC
 *              FM_MODE_OTH_READ
 *              FM_MODE_OTH_WRITE
 *              FM_MODE_OTH_EXEC
 *          b_ext_buf
 *              Use the external buffer. If it is TRUE, File Manager will
 *              NOT cache it. However, the buffer address must be aligned
 *              to specific address due to the hardware's limitation.
 *
 * Outputs: ph_file
 *              Handle of the opened file.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_old_path or ps_new_path is NULL.
 *          FMR_HANDLE
 *              h_old_dir or h_new_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_old_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_old_path is not found.
 *          FMR_EXIST
 *              ps_new_path already exists.
 *          FMR_NOT_DIR
 *              A component name in ps_old_path or in ps_new_path is not
 *              a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of source or destination path is too long.
 *          FMR_OTHER_FS
 *              Source and destination are not the same file system.
 *          FMR_DEVICE_ERROR
 *              Device open error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_open_ex(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file,
    UINT64          ui8_ofst)
{
    HANDLE_T  h_aux;
    INT32     i4_ret;

    if (aee_grab_handle_resource(AEE_FLAG, 1, &h_aux) != AEER_OK)
    {
        return FMR_HANDLE;
    }

    i4_ret = x_fm_open_ex(h_dir,
                          ps_path,
                          ui4_flags,
                          ui4_mode,
                          b_ext_buf,
                          ph_file,
                          ui8_ofst);

    if (i4_ret == FMR_OK)
    {
        handle_link_to_aux(h_aux, *ph_file);
    }
    else
    {
        aee_release_handle_resource(1);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_close
 *
 * Description: Close an opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is an file being closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_close(
    HANDLE_T        h_file)
{
    return x_fm_close(h_file);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_read
 *
 * Description: Read data from file to buffer.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          pv_data
 *              Buffer to hold read data.
 *          ui4_count
 *              Size of pv_data in bytes.
 *
 * Outputs: pui4_read
 *              Actual read bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_read is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed.
 *          FMR_EOF
 *              Reaches EOF.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_read(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_read)
{
    return x_fm_read(h_file,
                     pv_data,
                     ui4_count,
                     pui4_read);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_write
 *
 * Description: Write data from buffer to file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          pv_data
 *              Buffer of data to be written.
 *          ui4_count
 *              Number of bytes to be written.
 *
 * Outputs: pui4_write
 *              Actual written bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL or pui4_write is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_write(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_write)
{
    return x_fm_write(h_file,
                      pv_data,
                      ui4_count,
                      pui4_write);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_read_async
 *
 * Description: Asynchronously read data from the opened file.
 *
 * Inputs:  h_file
 *              Handle of the opened file.
 *          pv_data
 *              Reference to a buffer to hold the read data.
 *          ui4_count
 *              Number of bytes to read.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while read is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_read_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req    )
{
    return x_fm_read_async(h_file,
                           pv_data,
                           ui4_count,
                           ui1_pri,
                           pf_nfy_fct,
                           pv_tag,
                           ph_req);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_write_async
 *
 * Description: Asynchronously write data to the opened file.
 *
 * Inputs:  h_file
 *              Handle of the opened file.
 *          pv_data
 *              Reference to data to be written.
 *          ui4_count
 *              Number of bytes to write.
 *          ui1_pri
 *              I/O priority.
 *          pf_nfy_fct
 *              Callback function while write is done.
 *          pv_tag
 *              Tag value, which will be returned in the callback.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_write_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req)
{
    return x_fm_write_async(h_file,
                            pv_data,
                            ui4_count,
                            ui1_pri,
                            pf_nfy_fct,
                            pv_tag,
                            ph_req);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_fgets
 *
 * Description: read a string from the opened file.
 *
 * Inputs:  h_file
 *              Handle of the opened file.
 *          pv_data
 *              Reference to a buffer to hold the read data.
 *          ui4_count
 *              Number of bytes to read.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device read error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/

INT32 c_fm_fgets(
	HANDLE_T 		h_file,
	void 			*pv_data,
	UINT32 			ui4_count)
{
	return x_fm_fgets(h_file,
					  pv_data, 
					  ui4_count);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_fputs
 *
 * Description: write a string to the opened file.
 *
 * Inputs:  h_file
 *              Handle of the opened file.
 *          pv_data
 *              Reference to data to be written.
 *
 * Outputs: -
 *
 * Returns: > 0
 *              Success, actual number of written bytes
 *          FMR_ARG
 *              pv_data is NULL.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              h_file does not reference to a file descriptor or it is being
 *              closed, driver not ready, etc.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device write error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_fputs(
	HANDLE_T	h_file,
	const VOID	*pv_data)
{
	return x_fm_fputs(h_file,
					  pv_data);
}


INT32 c_fm_abort_async(
    HANDLE_T    h_req)
{
    return x_fm_abort_async(h_req);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_lseek
 *
 * Description: Change the current postion of the opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          i8_offset
 *              Offset from ui4_whence in bytes. Positive number means move
 *              current position backward while negative number means move
 *              forward.
 *          ui1_whence
 *              FM_SEEK_BGN tells File Manager to move current position to
 *              i8_offset from the begging of the file. if i8_offset is
 *              negative, current position is set to 0, that is the begging
 *              of file.
 *              FM_SEEK_CUR tells File Manager to move current position to
 *              i8_offset from current position.
 *              FM_SEEK_END tells File Manager to move current position to
 *              i8_offset from the end of the file. If i8_offset is a
 *              positive number, current position is set to the size of
 *              the file, that is the end of file.
 *
 * Outputs: pui8_cur_pos
 *              Current position.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64          *pui8_cur_pos)
{
    return x_fm_lseek(h_file,
                      i8_offset,
                      ui1_whence,
                      pui8_cur_pos);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_build_dir_idx_tbl
 *
 * Description: Build index table for specified directory
 *
 * Inputs:  h_dir
 *              Handle of an opened dir.
 *       
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_build_dir_idx_tbl(HANDLE_T   h_dir)
{
    return x_fm_build_dir_idx_tbl(h_dir);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_seek_dir
 *
 * Description: Change the current postion of the opened dir
 *
 * Inputs:  h_dir
 *              Handle of an opened file.
 *          ui1_whence
 *              FM_SEEK_BGN tells File Manager to move current position to
 *              i8_offset from the begging of the file. if i8_offset is
 *              negative, current position is set to 0, that is the begging
 *              of file.
 *              FM_SEEK_CUR tells File Manager to move current position to
 *              i8_offset from current position.
 *              FM_SEEK_END tells File Manager to move current position to
 *              i8_offset from the end of the file. If i8_offset is a
 *              positive number, current position is set to the size of
 *              the file, that is the end of file.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_seek_dir(
    HANDLE_T        h_dir,
    UINT8           ui1_whence,
    INT32           i4_range)
{
    return x_fm_seek_dir(h_dir, ui1_whence, i4_range);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_seek_dir
 *
 * Description: Change the current postion of the opened dir.
 *
 * Inputs:  h_dir
 *              Handle of an opened dir.
 *          ui8_ofst
 *              Entry offset counted in bytes.
 *              
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pui8_cur_pos is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed or ui1_whence is not defined.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/


INT32 c_fm_seek_dir_ex(
    HANDLE_T        h_dir,
    UINT64          ui8_ofst)
{
    return x_fm_seek_dir_ex(h_dir, ui8_ofst);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_feof
 *
 * Description: Tell if current position reaches the end of file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: pb_eof
 *              TRUE if it is at EOF, otherwise FALSE is returned.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              pb_eof is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              file is beging closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof)
{
    return x_fm_feof(h_file, pb_eof);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_trunc_by_name
 *
 * Description: Truncate a file to the specified size.
 *
 * Inputs:  h_dir
 *              Handle of a directory label
 *          ps_path
 *              Path name of the file.
 *          i8_len
 *              New length of the file in bytes.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_trunc_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len)
{
    return x_fm_trunc_by_name(h_dir, ps_path, i8_len);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_trunc_by_handle
 *
 * Description: Truncate an opened file to the specified size.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          i8_len
 *              New length of the file in bytes.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_trunc_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    return x_fm_trunc_by_handle(h_file, i8_len);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_chop_by_name
 *
 * Description: Cut specified number of bytes from the beginning of a file.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *          ps_path
 *              Path name of the file to be chopped.
 *          i8_len
 *              Number of bytes to be cut off.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_chop_by_name(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len)
{
    return x_fm_chop_by_name(h_dir, ps_path, i8_len);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_chop_by_handle
 *
 * Description: Cut off specified number of bytes from the beginning of an
 *              opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          i8_len
 *              Number of bytes to be cut off.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              h_file is being closed.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_chop_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    return x_fm_chop_by_handle(h_file, i8_len);
}
        
/*-----------------------------------------------------------------------------
 * Name: c_fm_extend_file
 *
 * Description: 
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
INT32 c_fm_extend_file(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    return x_fm_extend_file(h_file, i8_len);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_sync_file
 *
 * Description: 
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns:
 *
 ----------------------------------------------------------------------------*/
INT32 c_fm_sync_file(
    HANDLE_T        h_file)
{
    return x_fm_sync_file(h_file);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_info_by_name
 *
 * Description: Get information about a file/dir.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path will be created.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the file/dir.  
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_get_info_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info)
{
    return x_fm_get_info_by_name(h_dir, ps_path, pt_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_info_by_handle
 *
 * Description: Get information about an opened file/dir.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the file.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              h_file is NULL_HANDLE or pt_info is NULL.
 *          FMR_HANDLE
 *              h_file is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T  *pt_info)
{
    return x_fm_get_info_by_handle(h_file, pt_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_dir_info
 *
 * Description: Get information about a dir.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path will be created.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *
 * Outputs: pt_info
 *              Reference to the returned information about the dir.  
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pt_info is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_get_dir_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_DIR_INFO_T   *pt_info)
{
    return x_fm_get_dir_info(h_dir, ps_path, pt_info);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_chmod_by_name
 *
 * Description: Change the permission of a file/directory.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path starts.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be changed mode. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *          ui4_mode
 *              Permission to be set.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_INVAL
 *              ps_path is an invalid path.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_chmod_by_name(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    return x_fm_chmod_by_name(h_dir, ps_path, ui4_mode);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_chmod_by_handle
 *
 * Description: Change the permission of an opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          ui4_mode
 *              Permission to be set.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              h_file is NULL_HANDLE
 *          FMR_INVAL
 *              h_file is an invalid handle.
 *          FMR_PERM_DENY
 *              h_file is opened for read only.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_chmod_by_handle(
    HANDLE_T        h_file,
    UINT32          ui4_mode)
{
    return x_fm_chmod_by_handle(h_file, ui4_mode);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_make_entry
 *
 * Description: This function is used to create a special file for a specific
 *              block or character device driver.
 *
 * Inputs:  h_dir
 *              Handle of a directory where the ps_path will be created.
 *              FM_ROOT_HANDLE can be used to indicate the root directory.
 *              NULL_HANDLE is equivalent to FM_ROOT_HANDLE.
 *          ps_path
 *              Path name of the entry to be created. It can be a single
 *              file name or a path. Note that if the first character of
 *              ps_path is '/', then h_dir will be ignored and ps_path is
 *              regarded as absolute path. This argument cannot be NULL.
 *          ui4_mode
 *              The permission and type of the new entry. Type must be
 *              either FM_MODE_TYPE_BLK or FM_MODE_TYPE_CHR.
 *          ui2_dev
 *              Type of the new device entry.
 *          ui2_unit
 *              Unit id of the new device entry.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path is NULL.
 *          FMR_INVAL
 *              Parameters have invalid value.
 *          FMR_HANDLE
 *              h_dir is an invalid handle.
 *          FMR_EXIST
 *              ps_path already exists.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_NO_SPACE
 *              File system/device has no more space.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 c_fm_make_entry(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode,
    UINT16          ui2_dev,
    UINT16          ui2_unit)
{
    return x_fm_make_entry(h_dir,
                           ps_path,
                           ui4_mode,
                           ui2_dev,
                           ui2_unit);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_lock
 *
 * Description: Apply an advisory lock on an opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          ui4_op
 *              File lock operations.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              Invalid arguments.
 *          FMR_LOCK_FAIL
 *              Lock Failed.
 *          FMR_HANDLE
 *              Invalid handle.
 *          FMR_INVAL
 *              This lock is in the process of being deleted.
 *          RWLR_WOULD_BLOCK:
 *              This API will block on waiting for the lock.
 ----------------------------------------------------------------------------*/
INT32 c_fm_lock(
    HANDLE_T        h_file,
    UINT32          ui4_op)
{
    return x_fm_lock(h_file, ui4_op);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_unlock
 *
 * Description: Remove an advisory lock on an opened file.
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_LOCK_FAIL
 *              Lock Failed.
 *          FMR_HANDLE
 *              Invalid handle.
 *          FMR_INVAL
 *              This lock is in the process of being deleted.
 *          RWLR_WOULD_BLOCK:
 *              This API will block on waiting for the lock.
 ----------------------------------------------------------------------------*/
INT32 c_fm_unlock(
    HANDLE_T        h_file)
{
    return x_fm_unlock(h_file);
}


/*-----------------------------------------------------------------------------
 * Name: c_fm_grabbed
 *
 * Description: 
 *
 * Inputs:  h_file
 *              Handle of an opened file.
 *          ui4_flag
 *              File lock operations. Only FM_LOCK_READ and FM_LOCK_WRITE are
 *              acceptable. FM_LOCK_NO_WAIT and FM_LOCK_RELEASE are ignored.
 *              If FM_LOCK_READ and FM_LOCK_WRITE are set simultaneously, it
 *              returns FMR_ARG.
 *
 * Outputs: pb_res
 *              TRUE - If ui4_flag is FM_LOCK_WRITE, then the calling thread
 *              has acquired the write lock to this file. If ui4_flag is
 *              FM_LOCK_READ, then the calling thread or other threads have
 *              acquired the read lock to this file.
 *              False -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              Invalid arguments.
 *          FMR_HANDLE
 *              Invalid handle.
 ----------------------------------------------------------------------------*/
INT32 c_fm_grabbed(
    HANDLE_T        h_file,
    UINT32          ui4_flag,
    BOOL            *pb_res)
{
    return x_fm_grabbed(h_file, ui4_flag, pb_res);
}
/*-----------------------------------------------------------------------------
 * Name: c_fm_get_partition_ns
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32   c_fm_get_part_ns(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32 *pui4_count)
{
    return x_fm_get_part_ns(
                    h_dir,
                    ps_path,
                    pui4_count);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_get_partition_ns
 *
 * Description: 
 *
 * Inputs: 
 *          h_partition
 *              handle of partition table   
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32   c_fm_get_part_info(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32  ui4_part_idx,
    FM_PART_INFO_T *pt_part_info)
{
    return x_fm_get_part_info(
                    h_dir,
                    ps_path,
                    ui4_part_idx,
                    pt_part_info);
}

INT32   c_fm_recyc_buf(
    HANDLE_T h_dir,
    const CHAR *ps_path)
{
    return x_fm_recyc_buf(
                    h_dir,
                    ps_path);
}

/*-----------------------------------------------------------------------------
 * Name: c_fm_erase_flash_part
 *
 * Description: Erase the flash(NOR or NAND) partition specified by the ps_path.
 *              NOTE: The device node must be closed before calling this API.
 *
 * Inputs:  ps_path
 *              Path of the device node, like "/dev/nor_2".
 *          pui1_pattern
 *              Pattern.
 *          ui4_pattern_size.
 *              Size of the pattern.
 *
 * Outputs: NONE
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              ps_path or pui1_pattern is NULL, or ui4_pattern_size is 0.
 *          FMR_BUSY
 *              The device is being opened.
 *          FMR_INVAL
 *              Not a NAND or NOR flash, or the ui4_pattern_size is larger than the sector size.
 *          Others
 *              Internal error.
 ----------------------------------------------------------------------------*/

INT32 c_fm_erase_flash_part(
    const CHAR*      ps_path,
    UINT8*           pui1_pattern,
    UINT32           ui4_pattern_size)
{
    return x_fm_erase_flash_part(
                ps_path,
                pui1_pattern,
                ui4_pattern_size);
}
/*-----------------------------------------------------------------------------
 * Name: c_fm_io_ctrl
 *
 * Description: Do IO control for special device.
 *              NOTE: 
 *              (1)Once the user enabled(disabled) the flash write protection, the user is 
 *                 responsible for disabling(enabling) that before close the device handle. 
 *              (2)It is suggested to use the write protection only when upgrade 
 *                 the image bin.
 *
 * Inputs:  h_dev
 *              Device handle, got from c_fm_open().
 *          e_type
 *              IO control type.
 *          pv_data.
 *              IO control type data.
 *
 * Outputs: NONE
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_ARG
 *              h_dev or pv_data is NULL, or e_type is invalid.
 *          FMR_BUSY
 *              The device is being opened by other users.
 *          FMR_INVAL
 *              Not a NAND or NOR flash.
 *          Others
 *              Internal error.
 ----------------------------------------------------------------------------*/

INT32 c_fm_io_ctrl(
    HANDLE_T h_dev,
    FM_IO_CTRL_COMMAND e_type,
    VOID* pv_data)
{
    return x_fm_io_ctrl(h_dev, e_type, pv_data);
}

INT32 c_fm_copy_file(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct)
{
    return x_fm_copy_file(
          h_from_dir_lbl,
          ps_src_path,
          h_to_dir_lbl,
          ps_dest_path,
          pf_nfy_fct);
}

INT32 c_fm_async_copy_file(
    HANDLE_T       h_from_dir_lbl,
    CHAR*          ps_src_path,
    HANDLE_T       h_to_dir_lbl,
    CHAR*          ps_dest_path,
    x_fm_copy_nfy  pf_nfy_fct,
    VOID*          pv_tag,
    HANDLE_T*      ph_req)
{
    return x_fm_async_copy_file(
           h_from_dir_lbl,
           ps_src_path,
           h_to_dir_lbl,
           ps_dest_path,
           pf_nfy_fct,
           pv_tag,
           ph_req);
}

INT32 c_fm_copy_dir(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct)
{
    return x_fm_copy_dir(
          h_from_dir_lbl,
          ps_src_path,
          h_to_dir_lbl,
          ps_dest_path,
          pf_nfy_fct);
}

INT32 c_fm_async_copy_dir(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct,
    VOID*         pv_tag,
    HANDLE_T*     ph_req)
{
    return x_fm_async_copy_dir(
           h_from_dir_lbl,
           ps_src_path,
           h_to_dir_lbl,
           ps_dest_path,
           pf_nfy_fct,
           pv_tag,
           ph_req);
}


INT32 c_fm_set_default_encoding_type(
    E_MUL_BYTE_TYPE e_mul_byte_type)
{
    return x_fm_set_default_encoding_type(e_mul_byte_type);
}

INT32 c_fm_get_volume_label(
        HANDLE_T        h_dir,
        const CHAR*     ps_path,
        CHAR*           ps_vol_label,
        UINT32*         pui4_len)
{
    return x_fm_get_volume_label(h_dir, ps_path, ps_vol_label, pui4_len);
}

INT32 c_fm_partition_reg_nfy (x_fm_partition_status_nfy pf_nfy, UINT32 ui4_tag)
{
    return x_fm_partition_reg_nfy (pf_nfy, ui4_tag);
}

INT32 c_fm_remount_fs_rw (const CHAR* ps_path)
{
    return x_fm_remount_rw(ps_path);
}

INT32 c_fm_remount_fs_ro (const CHAR* ps_path)
{
    return x_fm_remount_ro(ps_path);
}


