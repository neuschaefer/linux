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
#include "u_uc_str.h"

/*!
 * @addtogroup    groupMW_FM
 * @brief    This header file declares File Manager exported APIs.
 * 
 * @see groupMW
 *
 * @{
 * */

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*------------------------------------------------------------------*/
/*! @brief  The x_fm_mount function mount the file system on device ps_dev_path
 *          to the directory ps_mp_path. File Manager will automatically recognize
 *          the file system type.
 *  @param [in] h_dev_dir   -   Handle of the directory label relative to ps_dev_path.
 *  @param [in] ps_dev_path -   Path of the block/character device special file.
 *  @param [in] h_mp_dir    -   Handle of the directory label relative to ps_mp_path.
 *  @param [in] ps_mp_path  -   Path of the directory to mount(Mount point).
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_dev_path or ps_mp_path is NULL.
 *  @retval FMR_HANDLE          -   h_dev_dir or h_mp_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_dev_path or ps_mp_path is an invalid path.
 *                                  Or file system is not supported.
 *  @retval FMR_NO_ENTRY        -   ps_dev_path or ps_mp_path is not found.
 *  @retval FMR_BUSY            -   ps_dev_path is already mounted.
 *  @retval FMR_NOT_DIR         -   A component name in ps_dev_path or in ps_mp_path or
 *                                  ps_mp_path itself is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_dev_path or ps_mp_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_mount(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path);

/*------------------------------------------------------------------*/
/*! @brief  The x_fm_mount_ex function tries to mount specific file system on device
 *          ps_dev_path to the directory ps_mp_path.
 *  @param [in] h_dev_dir   -   Handle of the directory label relative to ps_dev_path.
 *  @param [in] ps_dev_path -   Path of the block/character device special file.
 *  @param [in] h_mp_dir    -   Handle of the directory label relative to ps_mp_path.
 *  @param [in] ps_mp_path  -   Path of the directory to mount(Mount point).
 *  @param [in] pt_mnt_parm -   File system mount parameter.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_dev_path or ps_mp_path or pt_mnt_parm is NULL.
 *  @retval FMR_HANDLE          -   h_dev_dir or h_mp_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_dev_path or ps_mp_path is an invalid path.
 *                                  Or file system is not supported.
 *  @retval FMR_NO_ENTRY        -   ps_dev_path or ps_mp_path is not found.
 *  @retval FMR_BUSY            -   ps_dev_path is already mounted.
 *  @retval FMR_NOT_DIR         -   A component name in ps_dev_path or in ps_mp_path or
 *                                  ps_mp_path itself is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_dev_path or ps_mp_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_mount_ex(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path,
    FM_MNT_PARM_T   *pt_mnt_parm);
/*------------------------------------------------------------------*/
/*! @brief  The x_fm_attach_file_system function tries to attach specific file system on device
 *          ps_dev_path to the directory ps_mp_path. It is only used in Linux turnkey.
 *  @param [in] h_dev_dir   -   Handle of the directory label relative to ps_dev_path.
 *  @param [in] ps_dev_path -   Path of the block/character device special file.
 *  @param [in] h_mp_dir    -   Handle of the directory label relative to ps_mp_path.
 *  @param [in] ps_mp_path  -   Path of the directory to mount(Mount point).
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_dev_path or ps_mp_path or pt_mnt_parm is NULL.
 *  @retval FMR_HANDLE          -   h_dev_dir or h_mp_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_dev_path or ps_mp_path is an invalid path.
 *                                  Or file system is not supported.
 *  @retval FMR_NO_ENTRY        -   ps_dev_path or ps_mp_path is not found.
 *  @retval FMR_BUSY            -   ps_dev_path is already mounted.
 *  @retval FMR_NOT_DIR         -   A component name in ps_dev_path or in ps_mp_path or
 *                                  ps_mp_path itself is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_dev_path or ps_mp_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_attach_file_system(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path);


/*------------------------------------------------------------------*/
/*! @brief  Detach a mounted file system.
 *  @param [in] h_dir    -   Handle of the directory label relative to ps_path.
 *  @param [in] ps_path  -   Mount point.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_umount(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

/*------------------------------------------------------------------*/
/*! @brief  Get a mounted file system information.
 *  @param [in] h_dir    -   Handle of the directory label relative to ps_path.
 *  @param [in] ps_path  -   A file/directory path name in the file system.
 *  @param [out] pt_fs_info -   Reference to file system information object.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path or pt_fs_info is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info);

/*------------------------------------------------------------------*/
/*! @brief  Create(Format) a file system on the specified device(partition).
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_dev      -   Device special file.
 *  @param [in] e_fs_type   -   File system type.
 *  @param [in] ps_dev      -   Private data for individual file system.
 *  @param [in] z_size      -   Size of pv_data in bytes.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_dev is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NO_ENTRY        -   ps_dev does not exist.
 *  @retval FMR_INVAL           -   The specified file system is not supported or ps_dev is not
 *                                  a special file for block device.
 *  @retval FMR_BUSY            -   The device is busy.
 *  @retval FMR_DEVICE_ERROR    -   Driver/device level I/O error.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_dev is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_dev is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_create_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_dev,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data,
    SIZE_T          z_size);

/*------------------------------------------------------------------*/
/*! @brief  Check the integrity of a file system.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   A file/directory path name in the file system.
 *  @param [in] b_auto_fix  -   Set to TRUE to fix it when the file system has errors.
 *  @param [out] None.
 *  @note  This API is not implemented now.
 *  @return Return the error code.
 *  @retval FMR_NOT_IMPLEMENT   -   Not implemented.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_check_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    BOOL            b_auto_fix);

/*------------------------------------------------------------------*/
/*! @brief  Create a new directory with specific access mode.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the entry to be created.
 *  @param [in] ui4_mode    -   Access permission of the new directory.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path does not exist.
 *  @retval FMR_EXIST           -   ps_path already exists.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_create_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

/*------------------------------------------------------------------*/
/*! @brief  Delete an empty directory.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the entry to be removed.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_DIR_NOT_EMPTY   -   The directory contains files or directories.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_delete_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

/*------------------------------------------------------------------*/
/*! @brief  Recursively delete a non-empty directory.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the entry to be removed.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_delete_dir_ex(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

/*------------------------------------------------------------------*/
/*! @brief  Open a directory.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the entry to be opened.
 *  @param [out] ph_dir     -   Handle of the opened directory.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_open_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir);

/*------------------------------------------------------------------*/
/*! @brief  Open a directory with offset hint.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the entry to be opened.
 *  @param [in] ui8_ofst    -   Offset hint.
 *  @param [out] ph_dir     -   Handle of the opened directory.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_open_dir_ex(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir,
    UINT64          ui8_ofst);

/*------------------------------------------------------------------*/
/*! @brief  Read entries in a directory.
 *  @param [in]  h_dir         -   Handle of an opened directory.
 *  @param [in]  ui4_count     -   Number of elements that pt_dir_entry can hold.
 *  @param [out] pt_dir_entry  -   Reference to an array of FM_DIR_ENTRY_T objects.
 *  @param [out] pui4_entries  -   The actual number of elements read in the pt_dir_entry.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pt_dir_entry is NULL or pui4_entries is NULL or ui4_count is 0..
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   h_dir is being closed.
 *  @retval FMR_NOT_DIR         -   h_dir is not a directory.
 *  @retval FMR_EOF             -   End of directory, no more entries.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_read_dir_entries(
    HANDLE_T        h_dir,
    FM_DIR_ENTRY_T  *pt_dir_entry,
    UINT32          ui4_count,
    UINT32          *pui4_entries);

/*------------------------------------------------------------------*/
/*! @brief Set a label to the specified directory. One can use this
 *         function to set a reference to a directory, so that the
 *         underneath UFS can start finding a file from this directory
 *         without re-searching from the root. It hence provides the
 *         similar ability to the current working directory, but it's
 *         more flexible.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of a directory to be set.
 *  @param [out] ph_dir     -   Reference to a handle of directory label.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_set_dir_path(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir);

/*------------------------------------------------------------------*/
/*! @brief  Get the path name of a directory label.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] pui4_len    -   Size of ps_path in bytes.
 *  @param [out] ps_path    -   The path name of directory label h_dir.
 *  @param [out] pui4_len   -   If return code is FMR_NOT_ENOUGH_SPACE, this value contains
 *                              the actual required buffer size in bytes.
 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   ps_path or pui4_len is NULL.
 *  @retval FMR_HANDLE           -   h_dir is an invalid handle.
 *  @retval FMR_NOT_ENOUGH_SPACE -   Length of ps_path is not large enough.
 *  @retval FMR_CORE             -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_dir_path(
    HANDLE_T        h_dir,
    CHAR            *ps_path,
    UINT32          *pui4_len);

/*------------------------------------------------------------------*/
/*! @brief  Delete a file.
 *  @param [in] h_dir       -   Handle of a directory label.
 *  @param [in] ps_path     -   Path name of the file to be removed.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_delete_file(
    HANDLE_T        h_dir,
    const CHAR      *ps_path);

/*------------------------------------------------------------------*/
/*! @brief  Change the name of a file/directory or move a file/directory.
 *  @param [in] h_old_dir    -   Handle of the source directory label.
 *  @param [in] ps_old_path  -   Path name of the source entry. 
 *  @param [in] h_new_dir    -   Handle of the destination directory label.
 *  @param [in] ps_new_path  -   Path name of the destination entry.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_old_path or ps_new_path is NULL.
 *  @retval FMR_HANDLE          -   h_old_dir or h_new_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_old_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   ps_old_path is not found.
 *  @retval FMR_EXIST           -   ps_new_path already exists.
 *  @retval FMR_NOT_DIR         -   A component name in ps_old_path or in ps_new_path is not
 *                                  a directory.
 *  @retval FMR_IS_DIR          -   ps_new_path exists and is a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Source and destination are not the same file system.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_rename(
    HANDLE_T        h_old_dir,
    const CHAR      *ps_old_path,
    HANDLE_T        h_new_dir,
    const CHAR      *ps_new_path);

/*------------------------------------------------------------------*/
/*! @brief  Open a file.
 *  @param [in]  h_dir      -   Handle of a directory label.
 *  @param [in]  ps_path    -   Path name of the file to be opened.
 *  @param [in]  ui4_flags  -   Flags for open. User can specify these flags by bitwise 'OR'
 *                              operation. The valid flags are as follows:
 *                              FM_READ_ONLY tells File Manager that the file is opened
 *                              for read only, and any write operation will get a FMR_PERM_DENY.
 *                              FM_WRITE_ONLY tells File Manager that the file is opend
 *                              for write only, and any read operation will get a FMR_PERM_DENY.
 *                              FM_READ_WRITE tells File Manager that the file is opened
 *                              for both read and write.
 *                              If none of above flags is selected, the default is read only.
 *                              If more than one is set, FM_READ_WRITE supersedes
 *                              FM_WRITE_ONLY and FM_WRITE_ONLY supersedes FM_READ_ONLY.
 *                              FM_OPEN_CREATE tells File Manager to create the specified
 *                              file if it does not exist.
 *                              FM_OPEN_TRUNC tells File Manager to truncate all contents of
 *                              the specified file. The starting offset will be zero
 *                              FM_OPEN_APPEND tells File Manager to set the starting offset
 *                              to the end of the file.
 *                              FM_OPEN_EXCLUDE tells File Manager to exclusively create a
 *                              file. It fails if the file already exists.
 *  @param [in]  ui4_mode   -   UNIX style access permission. The valid values are as follows:
 *                              FM_MODE_USR_READ
 *                              FM_MODE_USR_WRITE
 *                              FM_MODE_USR_EXEC
 *                              FM_MODE_GRP_READ
 *                              FM_MODE_GRP_WRITE
 *                              FM_MODE_GRP_EXEC
 *                              FM_MODE_OTH_READ
 *                              FM_MODE_OTH_WRITE
 *                              FM_MODE_OTH_EXEC
 *  @param [in]  b_ext_buf  -   Use the external buffer. If it is TRUE, File Manager will
 *                              NOT cache it. However, the buffer address must be aligned
 *                              to specific address due to the hardware's limitation.
 *  @param [out] ph_file    -   Handle of the opened file.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_open(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file);

/*------------------------------------------------------------------*/
/*! @brief  Open a file with offset hint, which will improve the looking-up performance.
 *  @param [in]  h_dir      -   Handle of a directory label.
 *  @param [in]  ps_path    -   Path name of the file to be opened.
 *  @param [in]  ui4_flags  -   Flags for open. User can specify these flags by bitwise 'OR'
 *                              operation. The valid flags are as follows:
 *                              FM_READ_ONLY tells File Manager that the file is opened
 *                              for read only, and any write operation will get a FMR_PERM_DENY.
 *                              FM_WRITE_ONLY tells File Manager that the file is opend
 *                              for write only, and any read operation will get a FMR_PERM_DENY.
 *                              FM_READ_WRITE tells File Manager that the file is opened
 *                              for both read and write.
 *                              If none of above flags is selected, the default is read only.
 *                              If more than one is set, FM_READ_WRITE supersedes
 *                              FM_WRITE_ONLY and FM_WRITE_ONLY supersedes FM_READ_ONLY.
 *                              FM_OPEN_CREATE tells File Manager to create the specified
 *                              file if it does not exist.
 *                              FM_OPEN_TRUNC tells File Manager to truncate all contents of
 *                              the specified file. The starting offset will be zero
 *                              FM_OPEN_APPEND tells File Manager to set the starting offset
 *                              to the end of the file.
 *                              FM_OPEN_EXCLUDE tells File Manager to exclusively create a
 *                              file. It fails if the file already exists.
 *  @param [in]  ui4_mode   -   UNIX style access permission. The valid values are as follows:
 *                              FM_MODE_USR_READ
 *                              FM_MODE_USR_WRITE
 *                              FM_MODE_USR_EXEC
 *                              FM_MODE_GRP_READ
 *                              FM_MODE_GRP_WRITE
 *                              FM_MODE_GRP_EXEC
 *                              FM_MODE_OTH_READ
 *                              FM_MODE_OTH_WRITE
 *                              FM_MODE_OTH_EXEC
 *  @param [in]  b_ext_buf  -   Use the external buffer. If it is TRUE, File Manager will
 *                              NOT cache it. However, the buffer address must be aligned
 *                              to specific address due to the hardware's limitation.
 *  @param [in] ui8_ofst    -   Offset hint.
 *  @param [out] ph_file    -   Handle of the opened file.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   The directory component in ps_path or ps_path itself does not exist.
 *  @retval FMR_NOT_DIR         -   The directory component in ps_path is not a directory.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_fm_open_ex(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file,
    UINT64          ui8_ofst);

/*------------------------------------------------------------------*/
/*! @brief  Close an opened file.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_INVAL           -   h_file is an file being closed.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_close(
    HANDLE_T        h_file);

/*------------------------------------------------------------------*/
/*! @brief  Read data from file to buffer.
 *  @param [in] h_file      -   Handle of an opened file.
 *  @param [in] pv_data     -   Buffer to hold read data.
 *  @param [in] ui4_count   -   Number of bytes to read.
 *  @param [out] pui4_read  -   Actual read bytes.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pv_data is NULL or pui4_read is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed.
 *  @retval FMR_EOF             -   Reaches EOF.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_read(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_read);

/*------------------------------------------------------------------*/
/*! @brief  Write data from buffer to file.
 *  @param [in] h_file       -   Handle of an opened file.
 *  @param [in] pv_data      -   Buffer of data to be written.
 *  @param [in] ui4_count    -   Number of bytes to write.
 *  @param [out] pui4_write  -   Actual written bytes.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pv_data is NULL or pui4_write is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed.
 *  @retval FMR_EOF             -   Reaches EOF.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_write(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_write);

/*------------------------------------------------------------------*/
/*! @brief  Asynchronously read data from the opened file.
 *  @param [in] h_file      -   Handle of an opened file.
 *  @param [in] pv_data     -   Buffer to hold read data.
 *  @param [in] ui4_count   -   Number of bytes to read.
 *  @param [in] ui1_pri     -   I/O priority.
 *  @param [in] pf_nfy_fct  -   Callback function while read is done.
 *  @param [in] pv_tag      -   Tag value, which will be returned in the callback.
 *  @param [out] None
 *  @return Return the error code
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pv_data is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed, driver not ready, etc.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_EOF             -   Reaches end of file.
 *  @retval FMR_ALIGNMENT       -   pv_data alignment error.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_read_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T        *ph_req);

/*------------------------------------------------------------------*/
/*! @brief  Asynchronously write data to the opened file.
 *  @param [in] h_file      -   Handle of an opened file.
 *  @param [in] pv_data     -   Reference to data to be written.
 *  @param [in] ui4_count   -   Number of bytes to write.
 *  @param [in] ui1_pri     -   I/O priority.
 *  @param [in] pf_nfy_fct  -   Callback function while write is done.
 *  @param [in] pv_tag      -   Tag value, which will be returned in the callback.
 *  @param [out] None
 *  @return Return the error code
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pv_data is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed, driver not ready, etc.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_EOF             -   Reaches end of file.
 *  @retval FMR_ALIGNMENT       -   pv_data alignment error.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_write_async(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T        *ph_req);

/*------------------------------------------------------------------*/
/*! @brief read a string from file.
 *  @param [in] h_file      -   Handle of an opened file.
 *  @param [in] pv_data     -   Buffer to hold read data.
 *  @param [in] ui4_count   -   Number of bytes to read.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success
 *  @retval FMR_ARG             -   pv_data is NULL or pui4_write is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed.
 *  @retval FMR_EOF             -   Reaches EOF.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_fgets(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count);

/*------------------------------------------------------------------*/
/*! @brief  Write a string to file.
 *  @param [in] h_file       -   Handle of an opened file.
 *  @param [in] pv_data      -   Buffer of data to be written.
 *  @return Return the error code.
 *  @retval > 0                 -   actual number of written bytes
 *  @retval FMR_ARG             -   pv_data is NULL or pui4_write is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed.
 *  @retval FMR_EOF             -   Reaches EOF.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_fputs(
    HANDLE_T        h_file,
    const VOID      *pv_data);

/*------------------------------------------------------------------*/
/*! @brief  Abort a async IO request
 *  @param [in] h_req - Handle of a async IO request
 *  @param [out] None
 *  @return Return the error code
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pv_data is NULL.
 *  @retval FMR_HANDLE          -   Fail to get the handle object.
 *  @retval FMR_INVAL           -   h_file does not reference to a file descriptor or it is being
 *                                  closed, driver not ready, etc.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_EOF             -   Reaches end of file.
 *  @retval FMR_ALIGNMENT       -   pv_data alignment error.
 *  @retval FMR_DEVICE_ERROR    -   Device read error.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_abort_async(
    HANDLE_T    h_req);

/*------------------------------------------------------------------*/
/*! @brief  Change the current postion of the opened file.
 *  @param [in] h_file      -   Handle of an opened file.
 *  @param [in] i8_offset   -   Offset from ui4_whence in bytes. Positive number means move
 *                              current position backward while negative number means move
 *                              forward.
 *  @param [in] ui1_whence  -   FM_SEEK_BGN tells File Manager to move current position to
 *                              i8_offset from the begging of the file. if i8_offset is
 *                              negative, current position is set to 0, that is the begging
 *                              of file.
 *                              FM_SEEK_CUR tells File Manager to move current position to
 *                              i8_offset from current position.
 *                              FM_SEEK_END tells File Manager to move current position to
 *                              i8_offset from the end of the file. If i8_offset is a
 *                              positive number, current position is set to the size of
 *                              the file, that is the end of file.
 *  @param [out] pui8_cur_pos    -   Current position.
 *  @return Return the error code
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_ARG     -   pui8_cur_pos is NULL.
 *  @retval FMR_HANDLE  -   h_file is an invalid handle.
 *  @retval FMR_INVAL   -   h_file is being closed or ui1_whence is not defined.
 *  @retval FMR_CORE    -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64          *pui8_cur_pos);

/*------------------------------------------------------------------*/
/*! @brief  Build index table for specified directory, which will improve the seeking-dir 
 *               performance when the directory has a large number of files or child-directories..
 *  @param [in] h_dir   -   Handle of an opened dir.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   h_dir is being closed.
 *  @retval FMR_NOT_DIR         -   h_dir is not a directory.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_build_dir_idx_tbl(
    HANDLE_T   h_dir);

/*------------------------------------------------------------------*/
/*! @brief  Change the current postion of the opened dir.
 *  @param [in] h_dir       -   Handle of an opened dir.
 *  @param [in] ui1_whence  -   FM_SEEK_BGN tells File Manager to move current position to
 *                              i8_offset from the begging of the file. if i8_offset is
 *                              negative, current position is set to 0, that is the begging
 *                              of file.
 *                              FM_SEEK_CUR tells File Manager to move current position to
 *                              i8_offset from current position.
 *                              FM_SEEK_END tells File Manager to move current position to
 *                              i8_offset from the end of the file. If i8_offset is a
 *                              positive number, current position is set to the size of
 *                              the file, that is the end of file.
 *  @param [in] i4_ent_off   -  Entry offset. It doesn't count in bytes, but count in entry.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   h_dir is being closed.
 *  @retval FMR_NOT_DIR         -   h_dir is not a directory.
 *  @retval FMR_EOF             -   End of directory, no more entries.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_seek_dir(
    HANDLE_T    h_dir,
    UINT8       ui1_whence,
    INT32       i4_ent_off);

/*------------------------------------------------------------------*/
/*! @brief  Change the current postion of the opened dir with a hint offset.
 *  @param [in] h_dir       -   Handle of an opened dir.
 *  @param [in] ui8_ofst    -   Entry offset counted in bytes.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   h_dir is being closed.
 *  @retval FMR_NOT_DIR         -   h_dir is not a directory.
 *  @retval FMR_EOF             -   End of directory, no more entries.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32  x_fm_seek_dir_ex(
    HANDLE_T    h_dir,
    UINT64      ui8_ofst);

/*------------------------------------------------------------------*/
/*! @brief  Tell if current position reaches the end of file.
 *  @param [in]  h_file -   Handle of an opened file.
 *  @param [out] pb_eof -   TRUE if it is at EOF, otherwise FALSE is returned.
 *  @note
 *  @see
 *  @return Return the error code.
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_ARG     -   pb_eof is NULL.
 *  @retval FMR_HANDLE  -   h_file is an invalid handle.
 *  @retval FMR_INVAL   -   File is beging closed.
 *  @retval FMR_CORE    -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof);

/*------------------------------------------------------------------*/
/*! @brief  Truncate a file to the specified size by name.
 *  @param [in] h_dir   -   Handle of a directory label
 *  @param [in] ps_path -   Path name of the file.
 *  @param [in] i8_len  -   New length of the file in bytes.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_trunc_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len);

/*------------------------------------------------------------------*/
/*! @brief  Truncate a file to the specified size by handle.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [in] i8_len  -   New length of the file in bytes.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_trunc_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len);

/*------------------------------------------------------------------*/
/*! @brief  Cut specified number of bytes from the beginning of a file by name.
 *  @param [in] h_dir   -   Handle of a directory label
 *  @param [in] ps_path -   Path name of the file.
 *  @param [in] i8_len  -   Number of bytes to be cut off.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_chop_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len);

/*------------------------------------------------------------------*/
/*! @brief  Truncate a file to the specified size by handle.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [in] i8_len  -   Number of bytes to be cut off.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_chop_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len);
        
/*------------------------------------------------------------------*/
/*! @brief  Extend a file to the specified size.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [in] i8_len  -   New length of the file in bytes.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_extend_file(
    HANDLE_T        h_file,
    INT64           i8_len);

/*------------------------------------------------------------------*/
/*! @brief  Sync the cached dirty data to file.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_sync_file(
    HANDLE_T        h_file);

/*------------------------------------------------------------------*/
/*! @brief  Get information about a file by name.
 *  @param [in]  h_dir   -   Handle of a directory label.
 *  @param [in]  ps_path -   Path name of the file.
 *  @param [out] pt_info -   Reference to the returned information about the file.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path or pt_info is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_info_by_name(
      HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info);

/*------------------------------------------------------------------*/
/*! @brief  Get information about a file by handle.
 *  @param [in]  h_dir   -   Handle of an opened file.
 *  @param [out] pt_info -   Reference to the returned information about the file.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   pt_info is NULL.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T  *pt_info);

/*------------------------------------------------------------------*/
/*! @brief  Get information about a dir.
 *  @param [in]  h_dir   -   Handle of a directory label.
 *  @param [in]  ps_path -   Path name of the dir.
 *  @param [out] pt_info -   Reference to the returned information about the dir.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path or pt_info is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_dir_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_DIR_INFO_T   *pt_info);

/*------------------------------------------------------------------*/
/*! @brief  Change the permission of a file/directory by name.
 *  @param [in]  h_dir      -   Handle of a directory label.
 *  @param [in]  ps_path    -   Path name of the file/dir.
 *  @param [in]  ui4_mode   -   Permission to be set.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   ps_path is not found.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_chmod_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

/*------------------------------------------------------------------*/
/*! @brief  Change the permission of a file/directory by handle.
 *  @param [in]  h_file     -   Handle of an opened file/dir
 *  @param [in]  ui4_mode   -   Permission to be set.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_HANDLE          -   h_file is an invalid handle.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_chmod_by_handle(
    HANDLE_T        h_file,
    UINT32          ui4_mode);

/*------------------------------------------------------------------*/
/*! @brief  This function is used to create a special file for a specific
 *          block or character device driver.
 *  @param [in]  h_dir      -   Handle of a directory label.
 *  @param [in]  ps_path    -   Path name of the entry to be created.
 *  @param [in]  ui4_mode   -   The permission and type of the new entry. Type must be
 *                              either FM_MODE_TYPE_BLK or FM_MODE_TYPE_CHR.
 *  @param [in]  ui2_dev    -   Type of the new device entry.
 *  @param [in]  ui2_unit   -   Unit id of the new device entry.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_INVAL           -   Parameters have invalid value.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_EXIST           -   ps_path already exists.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_NO_SPACE        -   File system/device has no more space.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
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
/*------------------------------------------------------------------*/
/*! @brief  Apply an advisory lock on an opened file.
 *  @param [in] h_file  -   Handle of an opened file.
 *  @param [in] ui4_op  -   File lock operations.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   Invalid arguments.
 *  @retval FMR_LOCK_FAIL       -   Lock Failed.
 *  @retval FMR_HANDLE          -   Invalid handle.
 *  @retval FMR_INVAL           -   This lock is in the process of being deleted.
 *  @retval RWLR_WOULD_BLOCK    -   This API will block on waiting for the lock.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_lock(
    HANDLE_T        h_file,
    UINT32          ui4_op);

/*------------------------------------------------------------------*/
/*! @brief  Remove an advisory lock on an opened file.
 *  @param [in]  h_file  -   Handle of an opened file.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_HANDLE  -   Invalid handle.
 *  @retval FMR_INVAL   -   This lock is in the process of being deleted.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_unlock(
    HANDLE_T        h_file);

/*------------------------------------------------------------------*/
/*! @brief  Check if a file is grabbed.
 *  @param [in]  h_file    -   Handle of an opened file.
 *  @param [in]  ui4_flag  -   File lock operations. Only FM_LOCK_READ and FM_LOCK_WRITE are
 *                             acceptable. FM_LOCK_NO_WAIT and FM_LOCK_RELEASE are ignored.
 *                             If FM_LOCK_READ and FM_LOCK_WRITE are set simultaneously, it
 *                             returns FMR_ARG.
 *  @param [out] pb_res    -   TRUE     -   If ui4_flag is FM_LOCK_WRITE, then the calling thread
 *                             has acquired the write lock to this file. If ui4_flag is
 *                             FM_LOCK_READ, then the calling thread or other threads have
 *                             acquired the read lock to this file.
 *                             FALSE    -   Not be grabbed.
 *  @return Return the error code.
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_ARG     -   Invalid arguments.
 *  @retval FMR_HANDLE  -   Invalid handle.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_grabbed(
    HANDLE_T        h_file,
    UINT32          ui4_flag,
    BOOL            *pb_res);

/*------------------------------------------------------------------*/
/*! @brief  Get partition number of the specific device.
 *  @param [in]  h_dir      -   Handle of a directory label.
 *  @param [in]  ps_path    -   Path name of the device.
 *  @param [out] pui4_count -   Referenced to the returned partition number.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path or pui4_count is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   ps_path is not found.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_get_part_ns(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32 *pui4_count);

/*------------------------------------------------------------------*/
/*! @brief  Get specific partition info of the specific device.
 *  @param [in]  h_dir           -   Handle of a directory label.
 *  @param [in]  ps_path         -   Path name of the device.
 *  @param [in]  ui4_part_idx    -   Partition index.
 *  @param [out] pt_part_info    -   Referenced to the returned partition info.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path or pt_part_info is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_INVAL           -   ps_path is an invalid path.
 *  @retval FMR_NO_ENTRY        -   ps_path is not found.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
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

/* Free all buffers in the h_file's partition. */
extern INT32 x_fm_recyc_buf_by_file_handle(
    HANDLE_T h_file);

extern INT32 x_fm_recyc_buf(
    HANDLE_T h_dir,
    const CHAR *ps_path);

/*------------------------------------------------------------------*/
/*! @brief  Erase the flash(NOR or NAND) partition specified by the ps_path.
 *  @param [in] ps_path             -   Path of the device node, like "/dev/nor_2".
 *  @param [in] pui1_pattern        -   Pattern.
 *  @param [in] ui4_pattern_size    -   Size of the pattern.
 *  @param [out] None.
 *  @note   The device node must be closed before calling this API.
 *  @return Return the error code.
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_ARG     -   ps_path or pui1_pattern is NULL, or ui4_pattern_size is 0.
 *  @retval FMR_BUSY    -   The device is being opened.
 *  @retval FMR_INVAL   -   Not a NAND or NOR flash, or the ui4_pattern_size is larger than the sector size.
 *  @retval Others      -   Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_erase_flash_part(
    const CHAR*      ps_path,
    UINT8*           pui1_pattern,
    UINT32           ui4_pattern_size);

/*------------------------------------------------------------------*/
/*! @brief  Do IO control for special device.
 *  @param [in] h_dev   -   Device handle, got from x_fm_open().
 *  @param [in] e_type  -   IO control type.
 *  @param [in] pv_data -   IO control type data.
 *  @param [out] None
 *  @note   (1)Once the user enabled(disabled) the flash write protection, the user is 
 *             responsible for disabling(enabling) that before close the device handle. 
 *          (2)It is suggested to use the write protection only when upgrade 
 *             the image bin.
 *  @return Return the error code.
 *  @retval FMR_OK      -   Success.
 *  @retval FMR_ARG     -   h_dev or pv_data is NULL, or e_type is invalid.
 *  @retval FMR_BUSY    -   The device is being opened by other users.
 *  @retval FMR_INVAL   -   Not a NAND or NOR flash.
 *  @retval Others      -   Internal error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_io_ctrl(
    HANDLE_T h_dev,
    FM_IO_CTRL_COMMAND e_type,
    VOID* pv_data);

/*------------------------------------------------------------------*/
/*! @brief  Set attribute for a file/dir on disk by its name.
 *  @param [in] h_dir_lbl   -   Handle of a directory label
 *  @param [in] ps_path -   Path name of the file/dir.
 *  @param [in] e_set_type  -   Attribute set type.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_fm_set_attr_on_disk_by_name(
    HANDLE_T                   h_dir_lbl,
    const CHAR*                ps_path,
    FM_ATTR_ON_DISK_SET_TYPE_T e_set_type);

/*------------------------------------------------------------------*/
/*! @brief  Set attribute for a file/dir on disk by its opened handle.
 *  @param [in] h_file   -   Handle of the file/dir
 *  @param [in] e_set_type  -   Attribute set type.
 *  @param [out] None.
 *  @return Return the error code.
 *  @retval FMR_OK              -   Success.
 *  @retval FMR_ARG             -   ps_path is NULL.
 *  @retval FMR_HANDLE          -   h_dir is an invalid handle.
 *  @retval FMR_NAME_TOO_LONG   -   Length of ps_path is too long.
 *  @retval FMR_PERM_DENY       -   Permission deny.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_fm_set_attr_on_disk_by_handle(
    HANDLE_T                   h_file,
    FM_ATTR_ON_DISK_SET_TYPE_T e_set_type);


/*------------------------------------------------------------------*/
/*! @brief  API for Sync Copy/Paste file;
 *  @param [in] h_from_dir_lbl    -   Handle of the source directory label relative to ps_src_path.
 *  @param [in] ps_src_path      -   A file path name in the file system.
 *  @param [in] h_to_dir_lbl       -   Handle of the dest directory label relative to ps_dest_path.
 *  @param [in] ps_dest_path    -   Dest file path
 *  @param [in] pf_nfy_fct        -   Callback function to notify the progress
 *  @return Return the error code.
 *  @retval FMR_OK                -   Success.
 *  @retval FMR_ARG              -   ps_src_path or ps_dest_path is NULL.
 *  @retval FMR_HANDLE         -   h_from_dir_lbl or h_to_dir_lbl is an invalid handle.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_copy_file(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct);

/*------------------------------------------------------------------*/
/*! @brief  API for Async Copy/Paste file;
 *  @param [in] h_from_dir_lbl    -   Handle of the source directory label relative to ps_src_path.
 *  @param [in] ps_src_path      -   A file path name in the file system.
 *  @param [in] h_to_dir_lbl       -   Handle of the dest directory label relative to ps_dest_path.
 *  @param [in] ps_dest_path    -   Dest file path
 *  @param [in] pf_nfy_fct        -   Callback function to notify the progress
 *  @param [in] pv_tag             -   User's tag
 *  @param [out] ph_req           -   Request handle
 *  @return Return the error code.
 *  @retval FMR_OK                -   Success.
 *  @retval FMR_ARG              -   ps_src_path or ps_dest_path is NULL.
 *  @retval FMR_HANDLE         -   h_from_dir_lbl or h_to_dir_lbl is an invalid handle.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_async_copy_file(
    HANDLE_T       h_from_dir_lbl,
    CHAR*          ps_src_path,
    HANDLE_T       h_to_dir_lbl,
    CHAR*          ps_dest_path,
    x_fm_copy_nfy  pf_nfy_fct,
    VOID*          pv_tag,
    HANDLE_T*      ph_req);

/*------------------------------------------------------------------*/
/*! @brief  API for Sync Copy/Paste dir. Note: This API is not implemented now.
 *  @param [in] h_from_dir_lbl    -   Handle of the source directory label relative to ps_src_path.
 *  @param [in] ps_src_path      -   A directory path name in the file system.
 *  @param [in] h_to_dir_lbl       -   Handle of the dest directory label relative to ps_dest_path.
 *  @param [in] ps_dest_path    -   Dest dir path
 *  @param [in] pf_nfy_fct        -   Callback function to notify the progress
 *  @return Return the error code.
 *  @retval FMR_OK                -   Success.
 *  @retval FMR_ARG              -   ps_src_path or ps_dest_path is NULL.
 *  @retval FMR_HANDLE         -   h_from_dir_lbl or h_to_dir_lbl is an invalid handle.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_copy_dir(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct);

/*------------------------------------------------------------------*/
/*! @brief  API for Async Copy/Paste dir. Note: This API is not implemented now.
 *  @param [in] h_from_dir_lbl    -   Handle of the source directory label relative to ps_src_path.
 *  @param [in] ps_src_path      -   A directory path name in the file system.
 *  @param [in] h_to_dir_lbl       -   Handle of the dest directory label relative to ps_dest_path.
 *  @param [in] ps_dest_path    -   Dest dir path
 *  @param [in] pf_nfy_fct        -   Callback function to notify the progress
 *  @param [in] pv_tag             -   User's tag
 *  @param [out] ph_req           -   Request handle
 *  @return Return the error code.
 *  @retval FMR_OK                -   Success.
 *  @retval FMR_ARG              -   ps_src_path or ps_dest_path is NULL.
 *  @retval FMR_HANDLE         -   h_from_dir_lbl or h_to_dir_lbl is an invalid handle.
 *  @retval FMR_CORE            -   OSAI error.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_async_copy_dir(
    HANDLE_T      h_from_dir_lbl,
    CHAR*         ps_src_path,
    HANDLE_T      h_to_dir_lbl,
    CHAR*         ps_dest_path,
    x_fm_copy_nfy pf_nfy_fct,
    VOID*         pv_tag,
    HANDLE_T*     ph_req);


/*------------------------------------------------------------------*/
/*! @brief  API for querying volume label in FAT, as FAT doesn't specify the default encoding type in its volume label string
 *  @param [in] e_mul_byte_type    -   Default encoding type used by FAT
 *  @param [out] None
 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_set_default_encoding_type(
    E_MUL_BYTE_TYPE e_mul_byte_type);


/*------------------------------------------------------------------*/
/*! @brief  Get the volume label of a mounted file systeml.
 *  @param [in] h_dir    -   Handle of the directory label relative to ps_path.
 *  @param [in] ps_path  -   A file/directory path name in the file system.
 *  @param [in] pui4_len    -   Size of ps_path in bytes.
 *  @param [out] ps_path    -   The volume label string, note:it is an UTF8 string with null ('\0') terminated
 *  @param [out] pui4_len   -   If return code is FMR_NOT_ENOUGH_SPACE, this value contains
 *                              the actual required buffer size in bytes.
 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   ps_path or pui4_len is NULL.
 *  @retval FMR_HANDLE           -   h_dir is an invalid handle.
 *  @retval FMR_NOT_ENOUGH_SPACE -   Length of ps_path is not large enough.
 *  @retval FMR_CORE             -   OSAI error.
 */
/*------------------------------------------------------------------*/

extern INT32 x_fm_get_volume_label(
        HANDLE_T        h_dir,
        const CHAR*     ps_path,
        CHAR*           ps_vol_label,
        UINT32*         pui4_len);

#ifdef LINUX_TURNKEY_SOLUTION
/*------------------------------------------------------------------*/
/*! @brief  Remount a mounted partition to read-write.
 *  @param [in] ps_mp_path  -   A file/directory path name in the file system.

 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   ps_mp_path is NULL.
 *  @retval FMR_ERROR            -   mount(2) system call failed.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_remount_rw(const CHAR *ps_mp_path);

/*------------------------------------------------------------------*/
/*! @brief  Remount a mounted partition to read-only.
 *  @param [in] ps_mp_path  -   A file/directory path name in the file system.

 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   ps_mp_path is NULL.
 *  @retval FMR_ERROR            -   mount(2) system call failed.
 */
/*------------------------------------------------------------------*/

extern INT32 x_fm_remount_ro(const CHAR *ps_mp_path);

/*-----------------------------------------------------------------------------*/
/*! @brief  search all matched files in a directory and its sub-directory
 *  @param [in] ps_dir_path  -   the searched directory path
 *  @param [in] ps_keyword  -   the string to matched

 *  @param [out] pt_result  -   the pointer of search result structure

 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   parameter is invalid.
 */
 /*----------------------------------------------------------------------------*/
extern INT32 x_fm_search(
    const char *path,
    const char *keyword,
    FM_SEARCH_RESULT_T *pt_result);


/*-----------------------------------------------------------------------------*/
/*! @brief  search all matched files in a directory and its sub-directory async
 *  @param [in] ps_dir_path  -   the searched directory path
 *  @param [in] ps_keyword  -   the string to matched
 *  @param [in] pf_search_nfy  -   notify function

 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 *  @retval FMR_ARG              -   parameter is invalid.
 */
 /*----------------------------------------------------------------------------*/
extern INT32 x_fm_async_search(
    const char *ps_dir_path,
    const char *ps_keyword,
    x_fm_search_nfy pf_search_nfy,
    FM_SEARCH_RESULT_T *pt_result);


/*-----------------------------------------------------------------------------*/
/*! @brief  when search async, this function can stop search
  *  @return Return the error code.
  */
 /*----------------------------------------------------------------------------*/
extern VOID x_fm_search_stop(VOID);

/*-----------------------------------------------------------------------------*/
/*! @brief  Release the memory of search result structure
  *  @param [in] pt_result    the pointer point to search result structure
  */
 /*----------------------------------------------------------------------------*/
extern VOID x_fm_search_result_free(FM_SEARCH_RESULT_T *pt_result);


#endif /* LINUX_TURNKEY_SOLUTION */

/*------------------------------------------------------------------*/
/*! @brief  Set notify function for partition status change.
 *  @param [in] pf_nfy           -   Function pointer of callback function.
 *  @param [in] ui4_tag          -   Tag for callback identify.
 *  @return Return the error code.
 *  @retval FMR_NFY_SETTED       -   Callback function is already set.
 *  @retval FMR_OK               -   Success.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_partition_reg_nfy (x_fm_partition_status_nfy pf_nfy, UINT32 ui4_tag);

/*------------------------------------------------------------------*/
/*! @brief  Function to notify file manager about parition status change.
 *  @param [in] e_partition      -   Mounted partition.
 *  @return Return the error code.
 *  @retval FMR_OK               -   Success.
 */
/*------------------------------------------------------------------*/
extern INT32 x_fm_partition_ready (FS_PARTITION_T e_partition);



#if (defined(FM_SUPPORT_DYNAMIC_NTFS) && defined(DUAL_NTFS_BANK))
extern INT32 x_fm_get_ntfs_mount_path(
    CHAR            *ps_path,
    UINT32          *pui4_len);

extern INT32 x_fm_toggle_ntfs_load(VOID);
#endif

/*! @}  */

#endif /* _X_FM_H */

