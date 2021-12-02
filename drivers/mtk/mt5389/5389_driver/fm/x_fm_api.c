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
 * $RCSfile: x_fm_api.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 34076dbf72c1e756a741024f89fac47f $
 *
 * Description: 
 *         This file contains File Manager exported API's.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "handle/handle.h"
#include "x_rm_dev_types.h"
#include "x_fm.h"
#include "fm_ufs.h"
#include "fm_blkdev.h"
#include "fm_rootfs.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static VOID _fm_copy_path(
    CHAR        *ps_dst,
    const CHAR  *ps_src);
/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _fm_copy_path
 *
 * Description: Copy path name from ps_src to ps_dst.
 *              Redundant '/', '.' and '..' are eliminated.
 *
 * Inputs:  ps_src
 *              Srouce string buffer.
 *
 * Outputs: ps_dst
 *              Target string buffer.
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_copy_path(
    CHAR            *ps_dst,
    const CHAR      *ps_src)
{
/* Only called from x_fm_set_dir_path */
    const CHAR *pc_src;
    CHAR *pc_dst;

    pc_src = ps_src;
    pc_dst = ps_dst;
    while (*pc_src != '\0')
    {
        *pc_dst = *pc_src;
        if (*pc_src == '/')
        {
            /* handle '/' */
            while (*pc_src == '/') pc_src++;
            pc_dst++;
        }
        else if (*pc_src == '.')
        {
            /* handle '.' */
            if (((*(pc_src + 1) == '/') || (*(pc_src + 1) == '\0')) &&
                ((pc_src == ps_src) || (*(pc_src - 1) == '/')))
            {
                pc_src++;
                if (pc_dst > ps_dst) pc_dst--;
            }
            /* handle '..' */
            else if (((*(pc_src + 1) == '.') && 
                      ((*(pc_src + 2) == '/') || (*(pc_src + 2) == '\0'))) &&
                     ((pc_src == ps_src) || (*(pc_src - 1) == '/')))
            {
                pc_src += 2;
                while ((pc_dst > ps_dst) && (*pc_dst != '/')) pc_dst--;
                if (pc_dst > ps_dst) pc_dst--;
                while ((pc_dst > ps_dst) && (*pc_dst != '/')) pc_dst--;
            }
            else
            {
                pc_src++;
                pc_dst++;
            }
        }
        else
        {
            pc_src++;
            pc_dst++;
        }
    }
    if (pc_dst == ps_dst)
    {
        *pc_dst++ = '/';
    }
    *pc_dst = '\0';
}

BOOL _fm_check_mbr(
    UINT8 *pui1_mbr)
{
    BOOL b_mbr = FALSE;
    
    if ((pui1_mbr[510] == 0x55) && (pui1_mbr[511] == 0xAA))
    {
        if (((!((pui1_mbr[0] == 0xEB) && (pui1_mbr[2] == 0x90))) &&
            (!(pui1_mbr[0] == 0xE9))) || 
            (!x_strncmp("iPod", (const CHAR *)(&pui1_mbr[71]), 4)))
        {
            b_mbr = TRUE;
        }
    }

    return b_mbr;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: x_fm_mount
 *
 * Description: The x_fm_mount function attaches the file system on device
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
INT32 x_fm_mount(
    HANDLE_T        h_dev_dir,
    const CHAR      *ps_dev_path,
    HANDLE_T        h_mp_dir,
    const CHAR      *ps_mp_path)
{
    FM_DIR_LABEL_T  *pt_dev_lbl  = NULL;
    FM_DIR_LABEL_T  *pt_mp_lbl   = NULL;
    FM_XENTRY_T     *pt_dev_xent = NULL;
    FM_XENTRY_T     *pt_mp_xent  = NULL;
    CHAR            *s_name;
    FM_LOOKUP_ARG_T t_lookup_arg;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dev_dir %d,"
               "ps_dev_path %s,"
               "h_mp_dir %d,"
               "ps_mp_path %s\n",
               __FUNCTION__,
               h_dev_dir,
               (ps_dev_path == NULL) ? "NULL" : ps_dev_path,
               h_mp_dir,
               (ps_mp_path == NULL) ? "NULL" : ps_mp_path));

    i4_ret = fm_get_dir_label(h_dev_dir, &pt_dev_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    i4_ret = fm_get_dir_label(h_mp_dir, &pt_mp_lbl);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    /*
     *  Get the xentry of the device node.
     */
    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dev_lbl,
                           ps_dev_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_dev_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_BLK(pt_dev_xent) && !IS_XENTRY_CHR(pt_dev_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto EXIT;
    }

    if (!IS_XENTRY_VIRT(pt_dev_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto EXIT;
    }
    
    if (IS_XENTRY_MOUNTED(pt_dev_xent))
    {
        i4_ret = FMR_BUSY;
        goto EXIT;
    }

    /*
     *  Get the xentry of the mount point.
     */
    i4_ret = fm_ufs_lookup(pt_mp_lbl,
                           ps_mp_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_mp_xent = t_lookup_arg.pt_xent;

    i4_ret = fm_ufs_mount(pt_dev_xent, pt_mp_xent);

EXIT:

    if (pt_mp_xent != NULL)
    {
        fm_xentry_unlock(pt_mp_xent);
    }
    if (pt_dev_xent != NULL)
    {
        fm_xentry_unlock(pt_dev_xent);
    }
    if (pt_mp_lbl != NULL)
    {
        fm_obj_unlock(pt_mp_lbl);
    }
    if (pt_dev_lbl != NULL)
    {
        fm_obj_unlock(pt_dev_lbl);
    }

    x_mem_free(s_name);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_umount
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
INT32 x_fm_umount(
    HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_XENTRY_T     *pt_xent    = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg;
    CHAR            *s_name;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    /*
     *  Get the xentry of path.
     */
    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_xent = t_lookup_arg.pt_xent;

    i4_ret = fm_ufs_umount(pt_xent, TRUE);

EXIT:

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    x_mem_free(s_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_fs_info
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
INT32 x_fm_get_fs_info(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FS_INFO_T    *pt_fs_info)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_XENTRY_T     *pt_xent    = NULL;
    FM_XENTRY_T     *pt_tmp_xent = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg = {0};
    CHAR            *s_name;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "pt_fs_info %x\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               (pt_fs_info == NULL) ? 0 : (UINT32)pt_fs_info));

    if (pt_fs_info == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }
 
    /*
     *  Get the xentry of the device node.
     */
    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_xent = t_lookup_arg.pt_xent;

    if (IS_XENTRY_MOUNTED(pt_xent))
    {   
        ASSERT(pt_xent->pt_mounted != NULL);

        pt_tmp_xent = pt_xent->pt_mounted->pt_root_xent;

        i4_ret = fm_xentry_lock(pt_tmp_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }

        ASSERT(pt_tmp_xent != NULL);
        ASSERT(pt_tmp_xent->ui4_flags & FM_XENTRY_FLAG_BUSY);
        ASSERT(pt_tmp_xent->ui4_flags & FM_XENTRY_FLAG_MOUNT_ROOT);

        *pt_fs_info = pt_tmp_xent->pt_mount->t_fs_info;

        fm_xentry_unlock(pt_tmp_xent);
    }
    else
    {
        *pt_fs_info = pt_xent->pt_mount->t_fs_info;
    }

EXIT:

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    x_mem_free(s_name);
   
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_create_fs
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
INT32 x_fm_create_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_dev,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data,
    SIZE_T          z_size)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_dev %s, "
               "e_fs_type %d\n",
               __FUNCTION__,
               h_dir,
               (ps_dev == NULL) ? "NULL" : ps_dev,
               e_fs_type));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_create_fs(pt_dir_lbl,
                              ps_dev,
                              e_fs_type,
                              pv_data);
    
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_check_fs
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
INT32 x_fm_check_fs(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    BOOL            b_auto_fix)
{
    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "b_auto_fix %d\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               b_auto_fix));

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_INVAL));
    return FMR_INVAL;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_create_dir
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
INT32 x_fm_create_dir(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    FM_MKENT_ARG_T  t_mkent_arg;
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "ui4_mode %d\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               ui4_mode));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    t_mkent_arg.ui4_mode = FM_MODE_TYPE_DIR | (ui4_mode & FM_MODE_PERM_MASK);
    
    i4_ret = fm_ufs_make_entry(pt_dir_lbl, ps_path, &t_mkent_arg);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_delete_dir
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
INT32 x_fm_delete_dir(
	HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_delete(pt_dir_lbl, ps_path, FM_MODE_TYPE_DIR);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_open_dir
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
INT32 x_fm_open_dir(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    if (ph_dir == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_open(pt_dir_lbl,
                         ps_path,
                         FM_READ_ONLY,
                         FM_MODE_TYPE_DIR,
                         ph_dir,
                         NULL);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_read_dir_entries
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
INT32 x_fm_read_dir_entries(
    HANDLE_T        h_dir,
    FM_DIR_ENTRY_T  *pt_dir_entry,
    UINT32          ui4_count,
    UINT32          *pui4_entries)
{
    FM_READ_DIR_ARG_T   t_arg;
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ui4_count %d\n",
               __FUNCTION__,
               h_dir,
               ui4_count));
    
    if ((pt_dir_entry == NULL) || (pui4_entries == NULL) || (ui4_count == 0))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_dir, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    if (!IS_XENTRY_DIR(pt_desc->pt_xent))
    {
        i4_ret = FMR_NOT_DIR;
        goto EXIT;
    }

    t_arg.pt_xent       = pt_desc->pt_xent;
    t_arg.pt_dir_ent    = pt_dir_entry;
    t_arg.ui4_ent_cnt   = ui4_count;
    t_arg.ui4_read_cnt  = 0;
    t_arg.ui8_offset    = pt_desc->ui8_offset;
    t_arg.h_sync_sema   = pt_desc->h_sync_sema;

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_READ_DIR(pt_desc->pt_xent)(&t_arg);
    
        fm_xentry_unlock(pt_desc->pt_xent);

        if (i4_ret == FMR_OK || i4_ret == FMR_EOF)
        {
            pt_desc->ui8_offset = t_arg.ui8_offset;
            *pui4_entries       = t_arg.ui4_read_cnt;
        }
    }

EXIT:

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_set_dir_path
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
INT32 x_fm_set_dir_path(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    HANDLE_T        *ph_dir)
{
    CHAR            *s_name;
    FM_LOOKUP_ARG_T t_lookup_arg;
    FM_DIR_LABEL_T  *pt_new_lbl;
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_XENTRY_T     *pt_xent    = NULL;
    UINT16          ui2_len;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    if (ph_dir == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    pt_new_lbl = fm_alloc_dir_label();
    if (pt_new_lbl == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT;
    }

    i4_ret = handle_alloc((HANDLE_TYPE_T)FMT_DIR_LABEL,
                          pt_new_lbl,
                          NULL,
                          fm_handle_free_fct,
                          ph_dir);
    if (i4_ret != HR_OK)
    {
        i4_ret = FMR_HANDLE;
        goto EXIT;
    }
    
    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    pt_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_DIR(pt_xent))
    {
        i4_ret = FMR_NOT_DIR;
        goto EXIT;
    }

    if (IS_XENTRY_MOUNTED(pt_xent))
    {
        FM_XENTRY_T *pt_root_xent;

        pt_root_xent = pt_xent->pt_mounted->pt_root_xent;
        i4_ret = fm_xentry_lock(pt_root_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        fm_xentry_unlock(pt_xent);
        pt_xent = pt_root_xent;
    }

    fm_xentry_ref(pt_xent);
    
    pt_new_lbl->pt_xent = pt_xent;
    if ((pt_dir_lbl == NULL) || (*ps_path == '/'))
    {
        _fm_copy_path(pt_new_lbl->ps_path, ps_path);
    }
    else
    {
        x_strcpy(pt_new_lbl->ps_path, pt_dir_lbl->ps_path);
        x_strcat(pt_new_lbl->ps_path, ps_path);
        _fm_copy_path(pt_new_lbl->ps_path, pt_new_lbl->ps_path);
    }
    
    ui2_len = x_strlen(pt_new_lbl->ps_path);
    if (pt_new_lbl->ps_path[ui2_len - 1] != '/')
    {
        pt_new_lbl->ps_path[ui2_len++] = '/';
        pt_new_lbl->ps_path[ui2_len] = '\0';
    }
    pt_new_lbl->e_type  = FM_TYPE_DIRLBL;
    pt_new_lbl->ui2_len = ui2_len;
    pt_new_lbl->h_ref_handle = *ph_dir;
    
    fm_obj_reg(pt_new_lbl, pt_xent->pt_mount);
    
EXIT:
    
    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }
    if (i4_ret != FMR_OK)
    {
        if (*ph_dir != NULL_HANDLE)
        {
            handle_free(*ph_dir, FALSE);
        }
        if (pt_new_lbl != NULL)
        {
            fm_free_dir_label(pt_new_lbl);
        }
    }
    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    x_mem_free(s_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_dir_path
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
INT32 x_fm_get_dir_path(
	HANDLE_T        h_dir,
    CHAR            *ps_path,
    UINT32          *pui4_len)
{
    FM_DIR_LABEL_T  *pt_dir_lbl;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d\n",
               __FUNCTION__,
               h_dir));
    
    if ((ps_path == NULL) || (pui4_len == NULL))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    if (h_dir == FM_ROOT_HANDLE)
    {
        if (*pui4_len <= 1)
        {
            *pui4_len = 2;

            DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_NOT_ENOUGH_SPACE));
            return FMR_NOT_ENOUGH_SPACE;
        }

        ps_path[0] = '/';
        ps_path[1] = '\0';

        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_OK));
        return FMR_OK;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    if (*pui4_len <= pt_dir_lbl->ui2_len)
    {
        *pui4_len = pt_dir_lbl->ui2_len + 1;
        i4_ret = FMR_NOT_ENOUGH_SPACE;
        goto EXIT;
    }

    x_strcpy(ps_path, pt_dir_lbl->ps_path);

EXIT:

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_delete_file
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
INT32 x_fm_delete_file(
	HANDLE_T        h_dir,
    const CHAR      *ps_path)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    if (ps_path == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_delete(pt_dir_lbl, ps_path, FM_MODE_TYPE_FILE);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_rename
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
INT32 x_fm_rename(
    HANDLE_T        h_old_dir,
    const CHAR      *ps_old_path,
    HANDLE_T        h_new_dir,
    const CHAR      *ps_new_path)
{
    FM_DIR_LABEL_T  *pt_old_lbl = NULL;
    FM_DIR_LABEL_T  *pt_new_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_old_dir %d, "
               "ps_old_path %s, "
               "h_new_dir %d, "
               "ps_new_path  %s\n",
               __FUNCTION__,
               h_old_dir,
               (ps_old_path == NULL) ? "NULL" : ps_old_path,
               h_new_dir,
               (ps_new_path == NULL) ? "NULL" : ps_new_path));

    i4_ret = fm_get_dir_label(h_old_dir, &pt_old_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_get_dir_label(h_new_dir, &pt_new_lbl);
    if (i4_ret != FMR_OK)
    {
        fm_obj_unlock(pt_old_lbl);
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_rename(pt_old_lbl, ps_old_path, pt_new_lbl, ps_new_path);

    fm_obj_unlock(pt_new_lbl);
    fm_obj_unlock(pt_old_lbl);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_open
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
INT32 x_fm_open(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    BOOL            b_ext_buf,
    HANDLE_T        *ph_file)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "ui4_flags %d, "
               "ui4_mode %d, "
               "b_ext_buf %d, ",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               ui4_flags,
               ui4_mode,
               b_ext_buf));

    if ((ps_path == NULL) || (ph_file == NULL))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }


    if (b_ext_buf)
    {
        ui4_flags |= FM_NO_CACHE;
    }
    else
    {
        ui4_flags &= ~FM_NO_CACHE;
    }

    i4_ret = fm_ufs_open(pt_dir_lbl,
                         ps_path,
                         ui4_flags,
                         ui4_mode,
                         ph_file,
                         NULL);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_close
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
INT32 x_fm_close(
    HANDLE_T        h_file)
{
    HANDLE_TYPE_T   e_obj;
    VOID            *pv_obj;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d\n",
               __FUNCTION__,
               h_file));

    if ((handle_get_type_obj(h_file, &e_obj, &pv_obj) != HR_OK))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_HANDLE));
        return FMR_HANDLE;
    }

    if (e_obj == FMT_FILE_DESC)
    {
        FM_FILE_DESC_T *pt_desc  = (FM_FILE_DESC_T *)pv_obj;
        FM_MOUNT_T     *pt_mount = pt_desc->pt_xent->pt_mount;

        i4_ret = fm_obj_lock(pt_desc);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }

        pt_desc->ui4_status |= FM_OBJ_CLOSING;

        i4_ret = FILE_OP_CLOSE(pt_desc)(pt_desc);

        if (i4_ret != FMR_OK)
        {
            pt_desc->ui4_status &= ~FM_OBJ_CLOSING;
            fm_obj_unlock(pt_desc);
            goto EXIT;
        }

        fm_obj_unreg(pt_desc, pt_mount);
        
        x_handle_free(h_file);
    }
    else if (e_obj == FMT_DIR_LABEL)
    {
        FM_DIR_LABEL_T *pt_lbl   = (FM_DIR_LABEL_T *)pv_obj;
        FM_MOUNT_T     *pt_mount = pt_lbl->pt_xent->pt_mount;

        i4_ret = fm_obj_lock(pt_lbl);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }

        pt_lbl->ui4_status |= FM_OBJ_CLOSING;

        i4_ret = fm_xentry_lock(pt_lbl->pt_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            pt_lbl->ui4_status &= ~FM_OBJ_CLOSING;
            fm_obj_unlock(pt_lbl);
            goto EXIT;
        }

        fm_xentry_rel(pt_lbl->pt_xent);
        fm_xentry_unlock(pt_lbl->pt_xent);

        fm_obj_unreg(pt_lbl, pt_mount);

        x_handle_free(h_file);
    }
    else
    {
        i4_ret = FMR_HANDLE;
    }

EXIT:

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_read
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
INT32 x_fm_read(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_read)
{
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;
    FM_FILE_INFO_ARG_T  t_get_arg;
    FM_FILE_INFO_T t_info;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_count %d\n",
               __FUNCTION__,
               h_file,
               ui4_count));
    
    if ((pv_data == NULL) || (pui4_read == NULL) || (ui4_count == 0))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    /* modified by riccardo
         1. To prevent the read is out of the size of file
         2. To check if this read meets the end of file
         {
      */
    t_get_arg.pt_xent = pt_desc->pt_xent;
    t_get_arg.pt_info = &t_info;

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);

    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_GET_INFO(pt_desc->pt_xent)(&t_get_arg);
        fm_xentry_unlock(pt_desc->pt_xent);
    }
    else
    {
        return i4_ret;
    }

    if (t_info.ui8_size < pt_desc->ui8_offset + ui4_count)
    {
        ui4_count = t_info.ui8_size - pt_desc->ui8_offset;
    }

    i4_ret = FILE_OP_READ(pt_desc)(pt_desc,
                                   pv_data,
                                   ui4_count,
                                   pui4_read);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_write
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
INT32 x_fm_write(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_write)
{
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_count %d\n",
               __FUNCTION__,
               h_file,
               ui4_count));
    
    if ((pv_data == NULL) || (pui4_write == NULL) || (ui4_count == 0))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = FILE_OP_WRITE(pt_desc)(pt_desc,
                                    (VOID *)pv_data,
                                    ui4_count,
                                    pui4_write);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
  * Name: x_fm_abort_async
  *
  * Description: Abort async-ed r/w erquest
  *
  * Input: h_req
  *             Handle of the r/w request, or handle of file.
  *
  * Output: FMR_OK
  *                 Success!
  *             FMR_INVAL
  *                 No such request in queue.
  *             FMR_CANCELING
  *                 Caller will get a e_cond in callback function.
  *
 ----------------------------------------------------------------------------*/
extern INT32    x_fm_abort_async(HANDLE_T h_req)
{
    /*
    * 1. h_req contains FMT_ASYNC_READ/FMT_ASYNC_WRITE obj
    *
    *
    *
    * 2. h_req contains FMT_FILE_DESC obj.
    *
    */
    INT32   i4_ret = FMR_OK;
    FM_FILE_DESC_T *pt_file_desc;
    FM_DEVICE_T *pt_dev;
    HANDLE_TYPE_T   e_type;
    VOID*   pv_obj;
    FM_IO_REQ_T *pt_new_req;
    
    if ( NULL == (pt_new_req = (FM_IO_REQ_T*)x_mem_alloc(sizeof(FM_IO_REQ_T))))
    {
        return FMR_CORE;
    }

    x_memset((VOID *) pt_new_req, 0, sizeof(FM_IO_REQ_T));

    if ( HR_OK != handle_get_type_obj(h_req, &e_type, &pv_obj))
    {
        return FMR_CORE;
    }

    if ( e_type != FMT_ASYNC_READ
    && e_type != FMT_ASYNC_WRITE
    && e_type != FMT_FILE_DESC)
        return FMR_INVAL;

    pt_new_req->e_cond = FM_ASYNC_COND_ABORT_FAIL;
    pt_new_req->pv_tag = NULL;
    pt_new_req->ui1_pri = 127;
    pt_new_req->ui1_type = FM_DEV_REQ_ABORT;
    pt_new_req->pf_nfy_fct = NULL;
    pt_new_req->h_rw_req = h_req;

    if ( e_type != FMT_FILE_DESC)
    {
        pt_dev = ((FM_XENTRY_T*)pv_obj)->pt_dev;
    }
    else
    {
        pt_file_desc = (FM_FILE_DESC_T*)pv_obj;
        pt_dev = pt_file_desc->pt_xent->pt_dev;
    }
    
    i4_ret = fm_dev_send_req(pt_dev, pt_new_req);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_read_async
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
INT32 x_fm_read_async(
    HANDLE_T        h_file,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req)
{
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_count %d, "
               "ui1_pri %d\n",
               __FUNCTION__,
               h_file,
               ui4_count,
               ui1_pri));
    
    if (pv_data == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    if (ui4_count == 0)
    {
        return FMR_INVAL;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = FILE_OP_READ_ASYNC(pt_desc)(pt_desc,
                                         pv_data,
                                         ui4_count,
                                         ui1_pri,
                                         pf_nfy_fct,
                                         pv_tag,
                                         ph_req,
                                         h_file);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_write_async
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
INT32 x_fm_write_async(
    HANDLE_T        h_file,
    const VOID      *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req)
{
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_count %d, "
               "ui1_pri %d\n",
               __FUNCTION__,
               h_file,
               ui4_count,
               ui1_pri));
    
    if (pv_data == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = FILE_OP_WRITE_ASYNC(pt_desc)(pt_desc,
                                          (VOID *)pv_data,
                                          ui4_count,
                                          ui1_pri,
                                          pf_nfy_fct,
                                          pv_tag,
                                          ph_req,
                                          h_file);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_lseek
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
INT32 x_fm_lseek(
    HANDLE_T        h_file,
    INT64           i8_offset,
    UINT8           ui1_whence,
    UINT64          *pui8_cur_pos)
{
    FM_LSEEK_ARG_T  t_arg;
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret = FMR_OK;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "i8_offset %lld, "
               "ui1_whence %d\n",
               __FUNCTION__,
               h_file,
               i8_offset,
               ui1_whence));
    
    if (pui8_cur_pos == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    if (i8_offset <= 0)
    {
        if (ui1_whence == FM_SEEK_BGN)
        {
            pt_desc->ui8_offset = 0;
            goto EXIT; /* done */
        }
        else if (ui1_whence == FM_SEEK_CUR)
        {
            if (pt_desc->ui8_offset + i8_offset > 0)
            {
                pt_desc->ui8_offset += i8_offset;
            }
            else
            {
                pt_desc->ui8_offset = 0;
            }
            goto EXIT; /* done */
        }
    }

    t_arg.pt_xent     = pt_desc->pt_xent;
    t_arg.i8_offset   = i8_offset;
    t_arg.ui1_whence  = ui1_whence;
    t_arg.ui8_cur_pos = pt_desc->ui8_offset;

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
            
    i4_ret = XENTRY_OP_LSEEK(pt_desc->pt_xent)(&t_arg);
            
    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset = t_arg.ui8_cur_pos;
    }
    
EXIT:

    if (i4_ret == FMR_OK)
    {
        *pui8_cur_pos = pt_desc->ui8_offset;
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_seek_dir
 *
 * Description: Change the current postion of the opened dir.
 *
 * Inputs:  h_dir
 *              Handle of an opened dir.
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
 *          i4_ent_off
 *              entry offset. It doesn't count in bytes, but count in entry.
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
 INT32  x_fm_seek_dir(
    HANDLE_T    h_dir,
    UINT8   ui1_whence,
    INT32   i4_ent_off)
{
    INT32   i4_ret;
    FM_FILE_DESC_T  *pt_desc;
    FM_SEEK_DIR_ARG_T t_arg;
    /* error prevention */
    ASSERT( TRUE == x_handle_valid(h_dir));
    ASSERT( (ui1_whence == FM_SEEK_BGN) || (ui1_whence == FM_SEEK_CUR));

    /* get descriptor of this dir */
    i4_ret = fm_get_file_desc(h_dir, &pt_desc);
    if (i4_ret != FMR_OK)
        return i4_ret;

    /* reset current offset */
    if ( ui1_whence == FM_SEEK_BGN)
        pt_desc->ui8_offset = 0;

    t_arg.pt_xent     = pt_desc->pt_xent;
    t_arg.i4_ent_off = i4_ent_off;
    t_arg.ui1_whence  = ui1_whence;
    t_arg.ui8_cur_pos = pt_desc->ui8_offset;

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
            
    i4_ret = XENTRY_OP_SEEK_DIR(pt_desc->pt_xent)(&t_arg);
            
    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset = t_arg.ui8_cur_pos;
    }    

EXIT:
    fm_obj_unlock( pt_desc);
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_feof
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
INT32 x_fm_feof(
    HANDLE_T        h_file,
    BOOL            *pb_eof)
{
    FM_FILE_INFO_ARG_T  t_get_arg;
    FM_FILE_INFO_T      t_info;
    FM_FILE_DESC_T      *pt_desc;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d\n",
               __FUNCTION__,
               h_file));
    
    if (pb_eof == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }
    
    t_get_arg.pt_xent = pt_desc->pt_xent;
    t_get_arg.pt_info = &t_info;
    
    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_GET_INFO(pt_desc->pt_xent)(&t_get_arg);
        fm_xentry_unlock(pt_desc->pt_xent);

        if (i4_ret == FMR_OK)
        {
            /*
             *  pt_desc->ui8_offset may exceed the file size,
             *  that's because one can use x_fm_extend_file or
             *  x_fm_lseek to extend a file. The actual file 
             *  size will grow only after a write operation is
             *  issued.
             */
            *pb_eof = (pt_desc->ui8_offset >= t_info.ui8_size) ? TRUE : FALSE;
        }
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_trunc_by_name
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
INT32 x_fm_trunc_by_name(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_FILE_INFO_T  t_info;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "i8_len %lld\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               i8_len));
    
    if (ps_path == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    RTC_GetTimeDate(&ui8_time);

    t_info.ui4_mode        = FM_SET_MODE_NOVAL;
    t_info.ui8_size        = i8_len;
    t_info.ui8_offset      = FM_SET_SIZE_NOVAL;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    i4_ret = fm_ufs_lookup_set_info(pt_dir_lbl, ps_path, &t_info);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_trunc_by_handle
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
INT32 x_fm_trunc_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    FM_FILE_INFO_ARG_T  t_set_arg;
    FM_FILE_INFO_T  t_info;
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "i8_len %lld\n",
               __FUNCTION__,
               h_file,
               i8_len));

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    RTC_GetTimeDate(&ui8_time);

    t_info.ui4_mode        = FM_SET_MODE_NOVAL;
    t_info.ui8_size        = i8_len;
    t_info.ui8_offset      = FM_SET_SIZE_NOVAL;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    t_set_arg.pt_prnt_xent = pt_desc->pt_xent->pt_prnt_xent;
    t_set_arg.pt_xent = pt_desc->pt_xent;
    t_set_arg.pt_info = &t_info; 

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_SET_INFO(pt_desc->pt_xent)(&t_set_arg);
        fm_xentry_unlock(pt_desc->pt_xent);
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_chop_by_name
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
INT32 x_fm_chop_by_name(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    INT64           i8_len)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_FILE_INFO_T  t_info;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "i8_len %lld\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               i8_len));
    
    if (ps_path == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    RTC_GetTimeDate(&ui8_time);

    t_info.ui4_mode        = FM_SET_MODE_NOVAL;
    t_info.ui8_size        = FM_SET_SIZE_NOVAL;
    t_info.ui8_offset      = i8_len;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    i4_ret = fm_ufs_lookup_set_info(pt_dir_lbl, ps_path, &t_info);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_chop_by_handle
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
INT32 x_fm_chop_by_handle(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    FM_FILE_INFO_ARG_T  t_set_arg;
    FM_FILE_INFO_T  t_info;
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "i8_len %lld\n",
               __FUNCTION__,
               h_file,
               i8_len));

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    RTC_GetTimeDate(&ui8_time);
    
    t_info.ui4_mode        = FM_SET_MODE_NOVAL;
    t_info.ui8_size        = FM_SET_SIZE_NOVAL;
    t_info.ui8_offset      = i8_len;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    t_set_arg.pt_prnt_xent = pt_desc->pt_xent->pt_prnt_xent;
    t_set_arg.pt_xent = pt_desc->pt_xent;
    t_set_arg.pt_info = &t_info; 

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_SET_INFO(pt_desc->pt_xent)(&t_set_arg);
        fm_xentry_unlock(pt_desc->pt_xent);
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}
        
/*-----------------------------------------------------------------------------
 * Name: x_fm_extend_file
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
INT32 x_fm_extend_file(
    HANDLE_T        h_file,
    INT64           i8_len)
{
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_sync_file
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
INT32 x_fm_sync_file(
    HANDLE_T        h_file)
{
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d\n",
               __FUNCTION__,
               h_file));

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_SYNC(pt_desc->pt_xent)(pt_desc->pt_xent);

        fm_xentry_unlock(pt_desc->pt_xent);
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_info_by_name
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
INT32 x_fm_get_info_by_name(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info)
{

    CHAR                *s_name;
    FM_LOOKUP_ARG_T     t_lookup_arg;
    FM_FILE_INFO_ARG_T  t_get_arg;
    FM_DIR_LABEL_T      *pt_dir_lbl = NULL;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    if (pt_info == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    t_get_arg.pt_xent = t_lookup_arg.pt_xent;
    t_get_arg.pt_info = pt_info;

    i4_ret = XENTRY_OP_GET_INFO(t_lookup_arg.pt_xent)(&t_get_arg);
    
    fm_xentry_unlock(t_lookup_arg.pt_xent);

EXIT:

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    x_mem_free(s_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_info_by_handle
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
INT32 x_fm_get_info_by_handle(
    HANDLE_T        h_file,
    FM_FILE_INFO_T  *pt_info)
{
    FM_FILE_INFO_ARG_T  t_get_arg;
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d\n",
               __FUNCTION__,
               h_file));
    
    if (pt_info == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }
    
    t_get_arg.pt_xent = pt_desc->pt_xent;
    t_get_arg.pt_info = pt_info;

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_GET_INFO(pt_desc->pt_xent)(&t_get_arg);
        fm_xentry_unlock(pt_desc->pt_xent);
    }

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_dir_info
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
INT32 x_fm_get_dir_info(
	  HANDLE_T        h_dir,
    const CHAR      *ps_path,
    FM_DIR_INFO_T   *pt_info)
{

    CHAR                *s_name;
    FM_LOOKUP_ARG_T     t_lookup_arg;
    FM_DIR_INFO_ARG_T  t_get_arg;
    FM_DIR_LABEL_T      *pt_dir_lbl = NULL;
    INT32               i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path));

    if (pt_info == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    if (IS_XENTRY_MOUNTED(t_lookup_arg.pt_xent))
    {   
        FM_XENTRY_T *pt_tmp_xent;
    
        ASSERT(t_lookup_arg.pt_xent->pt_mounted != NULL);

        pt_tmp_xent = t_lookup_arg.pt_xent->pt_mounted->pt_root_xent;

        fm_xentry_lock(pt_tmp_xent, X_SEMA_OPTION_WAIT);

        t_get_arg.pt_xent = pt_tmp_xent;
        t_get_arg.pt_info = pt_info;

        i4_ret = XENTRY_OP_GET_DIR_INFO(pt_tmp_xent)(&t_get_arg);

        fm_xentry_unlock(pt_tmp_xent);
    }
    else
    {
        t_get_arg.pt_xent = t_lookup_arg.pt_xent;
        t_get_arg.pt_info = pt_info;

        i4_ret = XENTRY_OP_GET_DIR_INFO(t_lookup_arg.pt_xent)(&t_get_arg);
    }
   
    fm_xentry_unlock(t_lookup_arg.pt_xent);

EXIT:

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    x_mem_free(s_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_chmod_by_name
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
INT32 x_fm_chmod_by_name(
	HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    FM_FILE_INFO_T  t_info;
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "ui4_mode %d\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               ui4_mode));

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    RTC_GetTimeDate(&ui8_time);

    t_info.ui4_mode        = (ui4_mode & FM_MODE_PERM_MASK);
    t_info.ui8_size        = FM_SET_SIZE_NOVAL;
    t_info.ui8_offset      = FM_SET_SIZE_NOVAL;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    i4_ret = fm_ufs_lookup_set_info(pt_dir_lbl, ps_path, &t_info);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_chmod_by_handle
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
INT32 x_fm_chmod_by_handle(
    HANDLE_T        h_file,
    UINT32          ui4_mode)
{
    FM_FILE_INFO_ARG_T  t_set_arg;
    FM_FILE_INFO_T  t_info;
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;
    UINT64          ui8_time;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_mode %d\n",
               __FUNCTION__,
               h_file,
               ui4_mode));
    
    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    if ((pt_desc->ui4_flags & FM_ACCESS_MODE) == FM_READ_ONLY)
    {
        i4_ret = FMR_PERM_DENY;
        goto EXIT;
    }

    RTC_GetTimeDate(&ui8_time);

    t_info.ui4_mode        = (ui4_mode & FM_MODE_PERM_MASK);
    t_info.ui8_size        = FM_SET_SIZE_NOVAL;
    t_info.ui8_offset      = FM_SET_SIZE_NOVAL;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = ui8_time;
    t_info.ui8_modify_time = ui8_time;

    t_set_arg.pt_prnt_xent = pt_desc->pt_xent->pt_prnt_xent;
    t_set_arg.pt_xent = pt_desc->pt_xent;
    t_set_arg.pt_info = &t_info; 

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret == FMR_OK)
    {
        i4_ret = XENTRY_OP_SET_INFO(pt_desc->pt_xent)(&t_set_arg);
        fm_xentry_unlock(pt_desc->pt_xent);
    }

EXIT:

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_make_entry
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
INT32 x_fm_make_entry(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode,
    UINT16          ui2_dev,
    UINT16          ui2_unit)
{
    FM_MKENT_ARG_T  t_mkent_arg;
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "ui4_mode %d, "
               "ui2_dev %d, "
               "ui2_unit %d\n",
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               ui4_mode,
               ui2_dev,
               ui2_unit));
    
    if (ps_path == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    /* modified by riccardo {*/
    if (FM_IS_BLK(ui4_mode) == FM_IS_CHR(ui4_mode))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_INVAL));
        return FMR_INVAL;
    }

    switch (ui2_dev)
    {
#if 0
        case FM_DRVT_EEPROM:
            ui2_dev = DRVT_EEPROM;
            break;
        case FM_DRVT_NOR_FLASH:
            ui2_dev = DRVT_NOR_FLASH;
            break;
        case FM_DRVT_NAND_FLASH:
            ui2_dev = DRVT_NAND_FLASH;
            break;
#endif
        case FM_DRVT_MEM_CARD:
            ui2_dev = DRVT_MEM_CARD;
            break;
        case FM_DRVT_HARD_DISK:
            ui2_dev = DRVT_HARD_DISK;
            break;
        case FM_DRVT_USB_MASS_STORAGE:
            ui2_dev = DRVT_USB_MASS_STORAGE;
            break;
        case FM_DRVT_OPTICAL_DISC:
            ui2_dev = DRVT_OPTICAL_DISC;
            break;        
        default:
            return FMR_INVAL;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    t_mkent_arg.ui4_mode = ui4_mode;
    t_mkent_arg.ui2_dev  = ui2_dev;
    t_mkent_arg.ui2_unit = ui2_unit;
    
    i4_ret = fm_ufs_make_entry(pt_dir_lbl, ps_path, &t_mkent_arg);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_delete_entry
 *
 * Description: This function is used to delete a special file for a specific
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
INT32 x_fm_delete_entry(
    HANDLE_T        h_dir,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_dir %d, "
               "ps_path %s, "
               "ui4_mode %d, "
               __FUNCTION__,
               h_dir,
               (ps_path == NULL) ? "NULL" : ps_path,
               ui4_mode));
    
    if (ps_path == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    /* modified by riccardo {*/
    if (FM_IS_BLK(ui4_mode) == FM_IS_CHR(ui4_mode))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_INVAL));
        return FMR_INVAL;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    i4_ret = fm_ufs_delete(pt_dir_lbl, ps_path, ui4_mode);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_lock
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
INT32 x_fm_lock(
    HANDLE_T        h_file,
    UINT32          ui4_op)
{
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_op %d\n",
               __FUNCTION__,
               h_file,
               ui4_op));

    if (ui4_op & FM_LOCK_READ)
    {
        if (ui4_op & (FM_LOCK_WRITE|FM_LOCK_RELEASE))
        {
            DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
            return FMR_ARG;
        }
    }
    else if (ui4_op & FM_LOCK_WRITE)
    {
        if (ui4_op & (FM_LOCK_READ|FM_LOCK_RELEASE))
        {
            DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
            return FMR_ARG;
        }
    }
    else
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }
    
    i4_ret = FILE_OP_FLOCK(pt_desc)(pt_desc, ui4_op);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: x_fm_unlock
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
INT32 x_fm_unlock(
    HANDLE_T        h_file)
{
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d\n",
               __FUNCTION__,
               h_file));

    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }
    
    i4_ret = FILE_OP_FLOCK(pt_desc)(pt_desc, FM_LOCK_RELEASE);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_grabbed
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
INT32 x_fm_grabbed(
    HANDLE_T        h_file,
    UINT32          ui4_flag,
    BOOL            *pb_res)
{
    FM_FILE_DESC_T  *pt_desc;
    INT32           i4_ret;

    DBG_API(("[FM] %s: "
               "h_file %d, "
               "ui4_flag %d\n",
               __FUNCTION__,
               h_file,
               ui4_flag));

    if (pb_res == NULL)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    if (((ui4_flag & FM_LOCK_READ) && (ui4_flag & FM_LOCK_WRITE)) ||
        ((ui4_flag & (FM_LOCK_READ|FM_LOCK_WRITE)) == 0))
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_ARG));
        return FMR_ARG;
    }
    
    i4_ret = fm_get_file_desc(h_file, &pt_desc);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }
    
    ui4_flag &= ~(FM_LOCK_NO_WAIT|FM_LOCK_RELEASE);

    *pb_res = fm_ufs_is_file_locked(pt_desc, ui4_flag);

    fm_obj_unlock(pt_desc);

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, FMR_OK));
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_partition_ns
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
 *          FMR_ARG
 *              Invalid argument.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/

INT32 x_fm_get_part_ns(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32 *pui4_count)
{
    INT32 i4_ret = FMR_OK;

    FM_XENTRY_T *pt_xent = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg;
    FM_DIR_LABEL_T  *pt_dir_lbl  = NULL;
    CHAR *ps_name = NULL;

    ROOTFS_ENTRY_T  *pt_rent = NULL;

    FM_PART_TBL_T *pt_part_tbl = NULL;
    HANDLE_TYPE_T t_handle_type;

    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (ps_name == NULL)
    {
        return FMR_CORE;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto exit;
    }

    t_lookup_arg.ps_name = ps_name;

    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);

    if (i4_ret != FMR_OK)
    {
        goto exit;
    }

    if (pui4_count == NULL)
    {
        i4_ret = FMR_ARG;
        goto exit;
    }

    pt_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_TRUE(pt_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    pt_rent = ROOTFS_GET_ENT(pt_xent);

    ASSERT(pt_rent != NULL);

    if (pt_rent->u.h_part_tbl == NULL_HANDLE)
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    i4_ret = handle_get_type_obj(pt_rent->u.h_part_tbl,
                                 &t_handle_type,
                                  (VOID **)&pt_part_tbl);

    if ((i4_ret != HR_OK) || 
        (pt_part_tbl == NULL) || 
        ((t_handle_type != FMT_PART_TBL)))
    {
        i4_ret = FMR_HANDLE;
        goto exit;
    }

    *pui4_count = pt_part_tbl->ui4_parts;

exit:

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    x_mem_free(ps_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_get_partition_info
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
 *          FMR_ARG
 *              Invalid argument.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_get_part_info(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    UINT32  ui4_part_idx,
    FM_PART_INFO_T *pt_part_info)
{
    INT32 i4_cnt;
    INT32 i4_ret = FMR_OK;

    FM_XENTRY_T *pt_xent = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg = {0};
    FM_DIR_LABEL_T  *pt_dir_lbl  = NULL;
    CHAR *ps_name = NULL;

    ROOTFS_ENTRY_T  *pt_rent = NULL;

    FM_PART_TBL_T *pt_part_tbl = NULL;
    HANDLE_TYPE_T t_handle_type;

    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (ps_name == NULL)
    {
        return FMR_CORE;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto exit;
    }

    t_lookup_arg.ps_name = ps_name;

    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);

    if (i4_ret != FMR_OK)
    {
        goto exit;
    }

    if (pt_part_info == NULL)
    {
        i4_ret = FMR_ARG;
        goto exit;
    }

    pt_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_TRUE(pt_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    pt_rent = ROOTFS_GET_ENT(pt_xent);

    if (pt_rent->u.h_part_tbl == NULL_HANDLE)
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    i4_ret = handle_get_type_obj(pt_rent->u.h_part_tbl,
                                 &t_handle_type,
                                  (VOID **)&pt_part_tbl);

    if ((i4_ret != HR_OK) || 
        (pt_part_tbl == NULL) || 
        ((t_handle_type != FMT_PART_TBL)))
    {
        i4_ret = FMR_HANDLE;
        goto exit;
    }

    if (ui4_part_idx >= pt_part_tbl->ui4_parts)
    {
        i4_ret = FMR_ARG;
        goto exit;
    }

    for (i4_cnt = 0; i4_cnt < 4; i4_cnt++)
    {
        FM_PART_TBL_ENT_T *pt_ent = &(pt_part_tbl->t_prm[i4_cnt]);

        for (; pt_ent != NULL; pt_ent = pt_ent->pt_next)
        {
            FM_PART_T *pt_part = &(pt_ent->t_part);

            if ((pt_part->b_entried == TRUE) && (ui4_part_idx == pt_part->ui4_part_idx))
            {
                pt_part_info->b_mnt = pt_part->b_mnt;

                x_strcpy(pt_part_info->ps_part_name, pt_part->ps_part_name);
                x_strcpy(pt_part_info->ps_part_path, pt_part->ps_part_path);

                if (pt_part->b_mnt == TRUE)
                {
                    x_strcpy(pt_part_info->ps_mnt_path, pt_part->ps_mnt_path);
                }
                
                goto exit;
            }
        }
    }

    i4_ret = FMR_INVAL;

exit:

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    x_mem_free(ps_name);

    return i4_ret;    
}

/*-----------------------------------------------------------------------------
 * Name: x_fm_parse_partition
 *
 * Description: 
 *
 * Inputs:  ph_partitions
 *                  handle of partitions.
 *              s_dev_name
 *                  device node name.
 *              pb_mbr
 *                  indicator of BMR. True: there is a MBR, FALSE: none of MBR.
 *              pui4_cnt
 *                  partition counts.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_SUCH_DEVICE
 *              Cannot open the specified driver component.
 *          FMR_DEVICE_ERROR
 *              Set or get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 x_fm_parse_drive(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    const VOID *pv_data)
{
    INT32 i4_ret;
    INT32 i4_cnt;

    UINT32 ui4_part_cnt = 0;

    FM_RW_ARG_T t_io = {0};
    FM_XENTRY_T *pt_xent = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg = {0};
    FM_DIR_LABEL_T  *pt_dir_lbl  = NULL;
    CHAR *ps_name = NULL;

    FM_PART_TBL_T *pt_part_tbl = NULL;
    UINT8 *pui1_mbr = NULL;
    UINT8 *pui1_mbr_buf = NULL;

    HANDLE_T h_sync_sema = NULL_HANDLE;
    HANDLE_T h_part_tbl = NULL_HANDLE;

    ROOTFS_ENTRY_T  *pt_rent = NULL;

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto exit;
    }

    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (ps_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = ps_name;

    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);

    if (i4_ret != FMR_OK)
    {
        goto exit;
    }
                           
    pt_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_TRUE(pt_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    pt_part_tbl = (FM_PART_TBL_T *)x_mem_alloc(sizeof(FM_PART_TBL_T));

    if (pt_part_tbl == NULL)
    {
        i4_ret = FMR_CORE;
        goto exit;
    }

    x_memset(pt_part_tbl, 0, sizeof(FM_PART_TBL_T));

    i4_ret = blkdev_open(pt_xent);

    if (i4_ret != FMR_OK)
    {
        x_mem_free((VOID *)pt_part_tbl);
        goto exit;
    }

    pui1_mbr_buf = IS_XENTRY_BLK(pt_xent)?
                   x_mem_alloc(sizeof(UINT8) * 
                               (pt_xent->pt_dev->t_blk.ui4_sec_size + 
                               pt_xent->pt_dev->t_blk.ui4_align_bytes)) :
                   x_mem_alloc(sizeof(UINT8) * 512);

    if (pui1_mbr_buf == NULL)
    {
        x_mem_free((VOID *)pt_part_tbl);

        if (IS_XENTRY_BLK(pt_xent))
        {
            blkdev_close(pt_xent);
        }
        else
        {
            ASSERT(0);
        }

        i4_ret = FMR_CORE;
        goto exit;
    }

    if (IS_XENTRY_BLK(pt_xent))
    {
        x_memset((VOID *)pui1_mbr_buf, 
                 0, 
                 pt_xent->pt_dev->t_blk.ui4_sec_size + 
                 pt_xent->pt_dev->t_blk.ui4_align_bytes);

        pui1_mbr = pui1_mbr_buf + 
                (pt_xent->pt_dev->t_blk.ui4_align_bytes -
                (((UINT32)pui1_mbr_buf) % pt_xent->pt_dev->t_blk.ui4_align_bytes));
    }
    else
    {
        x_memset((VOID *)pui1_mbr_buf, 0, 512);

        pui1_mbr = pui1_mbr_buf;
    }

    if (x_sema_create(&h_sync_sema, 
                      X_SEMA_TYPE_BINARY, 
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free((VOID *)pt_part_tbl);

        if (IS_XENTRY_BLK(pt_xent))
        {
            blkdev_close(pt_xent);
        }
        else
        {
            ASSERT(0);
        }

        i4_ret = FMR_CORE;
        goto exit;
    }

    t_io.pt_xent        = pt_xent;
    t_io.pv_data        = pui1_mbr;
    t_io.ui8_offset     = 0;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC | FM_IO_DIRECT;
    t_io.h_bin_sema     = h_sync_sema;
    t_io.ui4_count      = IS_XENTRY_BLK(pt_xent) ?
                          pt_xent->pt_dev->t_blk.ui4_sec_size :
                          512;

    if (handle_alloc(FMT_PART_TBL, 
                     pt_part_tbl, 
                     NULL, 
                     fm_handle_free_fct, 
                     &h_part_tbl) != HR_OK)
    {
        x_mem_free((VOID *)pt_part_tbl);

        if (IS_XENTRY_BLK(pt_xent))
        {
            blkdev_close(pt_xent);
        }
        else
        {
            ASSERT(0);
        }

        i4_ret = FMR_HANDLE;
        goto exit;
    }

    /* here, should parse the partitions
      * 0. Check MBR
      * 1. 4 primary partitions
      * 2. unlimited extended partitions
      */

    if (IS_XENTRY_BLK(pt_xent))
    {
        i4_ret = blkdev_read(&t_io);
    }
    else
    {
        ASSERT(0);
    }

    if (i4_ret != FMR_OK)
    {
        x_handle_free(h_part_tbl);
        goto exit;
    }    

    if (_fm_check_mbr(pui1_mbr))
    {
        /* There is a mbr */
        /* Parse the 4 primary partitions */

        ui4_part_cnt = 0;

        for(i4_cnt = 0; i4_cnt < 4; i4_cnt++)
        {
            FM_PART_TBL_ENT_T *pt_tbl_ent;
            FM_PART_T *pt_part;

            pt_tbl_ent = &(pt_part_tbl->t_prm[i4_cnt]);

            pt_tbl_ent->pt_next = NULL;
            pt_tbl_ent->pt_prev = NULL;

            pt_part = &(pt_tbl_ent->t_part);
            
            pt_part->ui1_drv  = pui1_mbr[446 + i4_cnt * 16];            
            pt_part->ui1_start_chs[0] = pui1_mbr[447 + i4_cnt * 16];    
            pt_part->ui1_start_chs[1] = pui1_mbr[448 + i4_cnt * 16];    
            pt_part->ui1_start_chs[2] = pui1_mbr[449 + i4_cnt * 16];    
            pt_part->ui1_type = pui1_mbr[450 + i4_cnt * 16];            
            pt_part->ui1_end_chs[0] = pui1_mbr[451 + i4_cnt * 16];      
            pt_part->ui1_end_chs[1] = pui1_mbr[452 + i4_cnt * 16];      
            pt_part->ui1_end_chs[2] = pui1_mbr[453 + i4_cnt * 16];      

            x_end_cnvt_from_little_32(&pui1_mbr[454 + i4_cnt * 16], 
                                      1, 
                                      &(pt_part->ui4_start_lba)); 

            x_end_cnvt_from_little_32(&pui1_mbr[458 + i4_cnt * 16], 
                                      1, 
                                      &(pt_part->ui4_sectors));

            if ((pt_part->ui1_type != 0x0) &&
                (pt_part->ui1_type != 0x5) &&
                (pt_part->ui1_type != 0xF))
            {
                pt_part->ui4_part_idx = ui4_part_cnt;
                pt_part->b_entried = TRUE;

                ui4_part_cnt++;
            }

            if ((pt_part->ui1_type == 0x5) ||
                (pt_part->ui1_type == 0xF))
            {
                UINT64 ui8_offset;

                ui8_offset = IS_XENTRY_BLK(pt_xent) ?
                            ((UINT64)pt_part->ui4_start_lba * 
                                    pt_xent->pt_dev->t_blk.ui4_sec_size) :
                            (UINT64)1;

                i4_ret = fm_get_logical_partitions(pt_tbl_ent, 
                                                   pt_xent,
                                                   ui8_offset, 
                                                   &ui4_part_cnt);

                if (i4_ret != FMR_OK)
                {
                    x_handle_free(h_part_tbl);

                    if (IS_XENTRY_BLK(pt_xent))
                    {
                        blkdev_close(pt_xent);
                    }
                    else
                    {
                        ASSERT(0);
                    }
                    goto exit;
                }
            }
        }
    }
    else
    {
        FM_PART_TBL_ENT_T *pt_tbl_ent = NULL;
        FM_PART_T *pt_part = NULL;

        pt_tbl_ent = &(pt_part_tbl->t_prm[0]);

        pt_tbl_ent->pt_next = NULL;
        pt_tbl_ent->pt_prev = NULL;

        pt_part = &(pt_tbl_ent->t_part);

        pt_part->ui1_drv  = 0x80;   
        pt_part->ui1_start_chs[0] = 0;
        pt_part->ui1_start_chs[1] = 0;
        pt_part->ui1_start_chs[2] = 0;

        pt_part->ui1_type = 0x0;   

        pt_part->ui1_end_chs[0] = 0;
        pt_part->ui1_end_chs[1] = 0;
        pt_part->ui1_end_chs[2] = 0;
        pt_part->ui4_start_lba = 0;
        pt_part->ui4_sectors = (UINT32)pt_xent->pt_dev->t_blk.ui8_sec_num;

        pt_part->b_entried = TRUE;

        pt_part->ui4_part_idx = 0;
        pt_part->b_mnt = FALSE;

        ui4_part_cnt = 1;        
    }

    if (IS_XENTRY_BLK(pt_xent))
    {
        blkdev_close(pt_xent);
    }
    else
    {
        ASSERT(0);
    }

    pt_part_tbl->ui4_parts = ui4_part_cnt;
   
    pt_rent = ROOTFS_GET_ENT(pt_xent);

    pt_rent->u.h_part_tbl = h_part_tbl;

    for (i4_cnt = 0; i4_cnt < 4; i4_cnt++)
    {
        FM_PART_TBL_ENT_T *pt_ent = &(pt_part_tbl->t_prm[i4_cnt]);

        for (; pt_ent != NULL; pt_ent = pt_ent->pt_next)
        {
            FM_PART_T *pt_part = &(pt_ent->t_part);

            if (pt_part->b_entried == TRUE)
            {
                FM_MKENT_ARG_T  t_mk_arg = {0};
                ROOTFS_ENTRY_T  *pt_vrent = NULL;
                CHAR  ps_part_path[32];

                x_memset(ps_name, 0, sizeof(4 * (FM_MAX_FILE_LEN + 1)));
                x_sprintf(ps_name, "%s_%d", ps_path, pt_part->ui4_part_idx);
                                       
                t_mk_arg.ui4_mode = IS_XENTRY_BLK(pt_xent)?
                                    (FM_MODE_DEV_VIRT | FM_MODE_TYPE_BLK | 0666):
                                    (FM_MODE_DEV_VIRT | FM_MODE_TYPE_CHR | 0666);

                t_mk_arg.ui2_dev = (UINT16)(pt_rent->ui4_ent_id & 0x0000ffff);
                t_mk_arg.ui2_unit = (UINT16)((pt_rent->ui4_ent_id & 0xffff0000) >> 16) ;

                i4_ret = fm_ufs_make_entry(pt_dir_lbl, ps_name, &t_mk_arg);                

                if (i4_ret != FMR_OK)
                {
                    x_handle_free(h_part_tbl);
                    goto exit;
                }

                fm_get_xentry_full_path(t_mk_arg.pt_new_xent, 
                                        ps_part_path, 
                                        32);

                x_strcpy(pt_ent->t_part.ps_part_name, t_mk_arg.pt_new_xent->ps_name);
                x_strcpy(pt_ent->t_part.ps_part_path, ps_part_path);

                if (pv_data == NULL)
                {
                    pt_ent->t_part.t_mnt_cb.pf_mnt_fct = NULL;
                    pt_ent->t_part.t_mnt_cb.pv_tag = NULL;
                }
                else
                {
                    x_memcpy(&(pt_ent->t_part.t_mnt_cb), 
                             (const VOID *) pv_data, 
                             sizeof(FM_MNT_CB_T));
                }

                pt_vrent = ROOTFS_GET_ENT(t_mk_arg.pt_new_xent);

                ASSERT(pt_vrent->u.pt_part == NULL);

                pt_vrent->u.pt_part = &(pt_ent->t_part);
            }
        }
    }
   
exit:

    if (pui1_mbr_buf != NULL)
    {
        x_mem_free(pui1_mbr_buf);
    }

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    if (h_sync_sema != NULL_HANDLE)
    {
        x_sema_delete(h_sync_sema);
    }

    x_mem_free(ps_name);

    return i4_ret;    
}

INT32 x_fm_release_drive(
    HANDLE_T h_dir,
    const CHAR *ps_path)
{
    INT32 i4_cnt;
    INT32 i4_ret = FMR_OK;

    FM_XENTRY_T *pt_xent = NULL;
    FM_LOOKUP_ARG_T t_lookup_arg = {0};
    FM_DIR_LABEL_T  *pt_dir_lbl  = NULL;
    CHAR *ps_name = NULL;

    ROOTFS_ENTRY_T  *pt_rent = NULL;

    FM_PART_TBL_T *pt_part_tbl = NULL;
    HANDLE_TYPE_T t_handle_type;

    ps_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (ps_name == NULL)
    {
        return FMR_CORE;
    }

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto exit;
    }

    t_lookup_arg.ps_name = ps_name;

    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);

    if (i4_ret != FMR_OK)
    {
        goto exit;
    }
                           
    pt_xent = t_lookup_arg.pt_xent;

    if (!IS_XENTRY_TRUE(pt_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    pt_rent = ROOTFS_GET_ENT(pt_xent);

    if (pt_rent->u.h_part_tbl == NULL_HANDLE)
    {
        i4_ret = FMR_INVAL; /* invalid device type */
        goto exit;
    }

    i4_ret = handle_get_type_obj(pt_rent->u.h_part_tbl,
                                 &t_handle_type,
                                  (VOID **)&pt_part_tbl);

    if ((i4_ret != HR_OK) || 
        (pt_part_tbl == NULL) || 
        ((t_handle_type != FMT_PART_TBL)))
    {
        i4_ret = FMR_HANDLE;
        goto exit;
    }

    for (i4_cnt = 0; i4_cnt < 4; i4_cnt++)
    {
        FM_PART_TBL_ENT_T *pt_ent = &(pt_part_tbl->t_prm[i4_cnt]);

        for (; pt_ent != NULL; pt_ent = pt_ent->pt_next)
        {
            FM_PART_T *pt_part = NULL;
            UINT32 ui4_mode;

            pt_part = &(pt_ent->t_part);

            if (pt_part->b_entried == TRUE)
            {
                ui4_mode = FM_MODE_DEV_VIRT | 
                           FM_MODE_TYPE_BLK | 
                           0666;

                i4_ret = fm_ufs_delete(pt_dir_lbl, 
                                       pt_part->ps_part_name, 
                                       ui4_mode);

                if (i4_ret != FMR_OK)
                {
                    if (i4_ret == FMR_BUSY)
                    {
                        goto exit;
                    }
                    else
                    {
                        ASSERT(i4_ret == FMR_NO_ENTRY);
                        i4_ret = FMR_OK;
                    }
                }

                pt_part->b_entried = FALSE;
            }
        }
    }

    x_handle_free(pt_rent->u.h_part_tbl);

exit:

    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    x_mem_free(ps_name);

    return i4_ret;
}

INT32 x_fm_set_time_by_name(
    HANDLE_T h_dir,
    const CHAR *ps_path,
    TIME_T t_access_time,
    TIME_T t_modified_time)
{
    FM_FILE_INFO_T  t_info;
    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    INT32           i4_ret;

    i4_ret = fm_get_dir_label(h_dir, &pt_dir_lbl);
    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        return i4_ret;
    }

    t_info.ui4_mode        = FM_SET_MODE_NOVAL;
    t_info.ui8_size        = FM_SET_SIZE_NOVAL;
    t_info.ui8_offset      = FM_SET_SIZE_NOVAL;
    t_info.ui8_create_time = FM_SET_TIME_NOVAL;
    t_info.ui8_access_time = t_access_time;
    t_info.ui8_modify_time = t_modified_time;

    i4_ret = fm_ufs_lookup_set_info(pt_dir_lbl, ps_path, &t_info);

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

    DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
    return i4_ret;
}

