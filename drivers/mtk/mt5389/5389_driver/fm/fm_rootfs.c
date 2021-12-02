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
 * $RCSfile: fm_rootfs.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: ead66f3efb7f6ab251403e8a2ecfa170 $
 *
 * Description: 
 *         This file implements the root file system.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_os.h"
#include "fm_util.h"
#include "fm_rootfs.h"
#include "fm_blkdev.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef DEBUG_FM_ROOTFS

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_ROOTFS */

/*-----------------------------------------------------------------------------
                    functions declaraions
 ----------------------------------------------------------------------------*/

static VOID _rootfs_add_entry(
    ROOTFS_ENTRY_T      *pt_parent,
    ROOTFS_ENTRY_T      *pt_child);

static VOID _rootfs_del_entry(
    ROOTFS_ENTRY_T      *pt_child);

static ROOTFS_ENTRY_T *_rootfs_lookup_entry(
    ROOTFS_ENTRY_T      *pt_parent,
    const CHAR          *ps_name);

static FM_XENTRY_T *_rootfs_make_xentry(
    FM_XENTRY_T         *pt_this_xent,
    ROOTFS_ENTRY_T      *pt_rent);


static INT32 rootfs_mount(VOID      *pv_data);
static INT32 rootfs_umount(VOID     *pv_data);
static INT32 rootfs_lookup(VOID     *pv_data);
static INT32 rootfs_make_entry(VOID *pv_data);
static INT32 rootfs_read(VOID       *pv_data);
static INT32 rootfs_write(VOID      *pv_data);
static INT32 rootfs_lseek(VOID      *pv_data);
static INT32 rootfs_close(VOID      *pv_data);
static INT32 rootfs_get_info(VOID   *pv_data);
static INT32 rootfs_set_info(VOID   *pv_data);
static INT32 rootfs_get_dir_info(VOID   *pv_data);
static INT32 rootfs_read_dir(VOID   *pv_data);
static INT32 rootfs_delete(VOID     *pv_data);
static INT32 rootfs_rename(VOID     *pv_data);
static INT32 rootfs_sync(VOID       *pv_data);
static INT32 rootfs_inval(VOID      *pv_data);

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

static FM_UFS_OP_T t_rootfs_ufs_ops = 
{
    NULL,
    NULL,
    rootfs_mount,
    rootfs_umount
};

static FM_XENTRY_OP_T t_rootfs_xent_ops =
{
    rootfs_lookup,
    rootfs_read,
    rootfs_write,
    rootfs_inval,
    rootfs_inval,
    rootfs_lseek,
    rootfs_close,
    rootfs_get_info,
    rootfs_set_info,
    rootfs_get_dir_info,
    rootfs_make_entry,
    NULL,
    NULL,
    rootfs_read_dir,
    rootfs_delete,
    rootfs_rename,
    rootfs_sync,
    rootfs_lseek,
    NULL,
};

static FM_XENTRY_OP_T t_blkdev_xent_ops =
{
    rootfs_lookup,
    blkdev_read,
    blkdev_write,
    blkdev_read,
    blkdev_write,
    blkdev_lseek,
    blkdev_close,
    rootfs_get_info,
    rootfs_set_info,
    rootfs_get_dir_info,
    rootfs_make_entry,
    NULL,
    NULL,
    rootfs_read_dir,
    rootfs_delete,
    rootfs_rename,
    blkdev_sync,
    blkdev_lseek,
    NULL
};

static ROOTFS_ENTRY_T  t_root_rent, *pt_root_rent;
static UINT32 ui4_free_size;
static UINT32 ui4_used_size;
static UINT32 ui4_free_block;
static UINT32 ui4_used_block;
static UINT32 ui4_meta_size;
static UINT32 ui4_meta_block;

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#ifdef DEBUG_FM_ROOTFS

VOID PRINT_FS_INFO(VOID)
{
    DBG_INFO(("\n"
                "[FM] ROOTFS info:\n"
                "     \tTotal bytes:%u\n"
                "     \tFree bytes:%u\n"
                "     \tUsed bytes:%u\n"
                "     \tTotal blocks:%u\n"
                "     \tFree blocks:%u\n"
                "     \tUsed blocks:%u\n\n",
                ROOTFS_SIZE,
                (int)ui4_free_size,
                (int)ui4_used_size,
                ROOTFS_BLOCK_NUM,
                (int)ui4_free_block,
                (int)ui4_used_block));
}

#else

#define PRINT_FS_INFO()

#endif /* DEBUG_FM_ROOTFS */

/*-----------------------------------------------------------------------------
 * Name: _rootfs_add_entry
 *
 * Description: Add an entry to the file system.
 *
 * Inputs:  pt_parent
 *              Parent directory entry where the new entry will be put.
 *          pt_child
 *              New directory entry to be added.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _rootfs_add_entry(
    ROOTFS_ENTRY_T *pt_parent,
    ROOTFS_ENTRY_T *pt_child)
{
    ASSERT(pt_parent != NULL);
    ASSERT(pt_child  != NULL);
    
    pt_child->pt_parent = pt_parent;
    
    if (pt_parent->pt_child != NULL)
    {
        pt_child->pt_next = pt_parent->pt_child;
        pt_child->pt_prev = NULL;
        pt_parent->pt_child->pt_prev = pt_child;
    }

    pt_parent->pt_child = pt_child;
}


/*-----------------------------------------------------------------------------
 * Name: _rootfs_del_entry
 *
 * Description: Remove a directory entry.
 *
 * Inputs:  pt_child
 *              Directory entry to be removed.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _rootfs_del_entry(
    ROOTFS_ENTRY_T *pt_child)
{
    ROOTFS_ENTRY_T *pt_parent;

    ASSERT(pt_child  != NULL);
    
    pt_parent = (ROOTFS_ENTRY_T *)pt_child->pt_parent;

    ASSERT(pt_parent != NULL);
    
    if (pt_child->pt_next != NULL)
    {
        pt_child->pt_next->pt_prev = pt_child->pt_prev;
    }

    if (pt_child->pt_prev != NULL)
    {
        pt_child->pt_prev->pt_next = pt_child->pt_next;
    }

    if (pt_parent->pt_child == pt_child)
    {
        pt_parent->pt_child = pt_child->pt_next;
    }

    pt_child->pt_next = pt_child->pt_prev = NULL;
}


/*-----------------------------------------------------------------------------
 * Name: _rootfs_lookup_entry
 *
 * Description: Lookup an entry by name in a directory.
 *
 * Inputs:  pt_parent
 *              The directory where to search.
 *          ps_name
 *              Entry name to be searched.
 *
 * Outputs: -
 *
 * Returns: The reference to the entry object on success.
 *          Otherwise, NULL is returned.
 ----------------------------------------------------------------------------*/
static ROOTFS_ENTRY_T *_rootfs_lookup_entry(
    ROOTFS_ENTRY_T  *pt_parent,
    const CHAR      *ps_name)
{
    ROOTFS_ENTRY_T  *pt_child;

    ASSERT(pt_parent != NULL);
    ASSERT(ps_name   != NULL);

    for (pt_child = pt_parent->pt_child;
         pt_child != NULL;
         pt_child = pt_child->pt_next)
    {
        if (!x_strcmp(pt_child->ps_name, ps_name))
        {
            break;
        }
    }

    return pt_child;
}


/*-----------------------------------------------------------------------------
 * Name: _rootfs_make_xentry
 *
 * Description: Create an xentry to associate the rootfs entry.
 *
 * Inputs:  pt_prnt_xent
 *              The reference to xentry of the parent directory.
 *          pt_rent
 *              The reference to rootfs entry.
 *
 * Outputs: -
 *
 * Returns: The reference to the new xentry object on success.
 *          Otherwise, NULL is returned.
 ----------------------------------------------------------------------------*/
static FM_XENTRY_T *_rootfs_make_xentry(
    FM_XENTRY_T     *pt_prnt_xent,
    ROOTFS_ENTRY_T  *pt_rent)
{
    FM_XENTRY_T *pt_xent;
    ROOTFS_ENTRY_T **ppt_rent;

    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_rent      != NULL);

    ppt_rent = x_mem_alloc(sizeof(ROOTFS_ENTRY_T *));
    if (ppt_rent == NULL)
    {
        return NULL;
    }
    *ppt_rent = pt_rent;

    pt_xent = fm_get_new_xentry(pt_rent->ps_name);
    if (pt_xent != NULL)
    {
        pt_xent->ui4_mode     = pt_rent->ui4_mode;
        pt_xent->pt_mount     = pt_prnt_xent->pt_mount;
        pt_xent->pt_ufs_ops   = pt_prnt_xent->pt_ufs_ops;
        pt_xent->pv_fs_node   = ppt_rent;
        pt_xent->pt_prnt_xent = pt_prnt_xent;

        if (IS_XENTRY_BLK(pt_xent))
        {
            pt_xent->pt_xent_ops = &t_blkdev_xent_ops;
        }
        else if (IS_XENTRY_CHR(pt_xent))
        {
            ASSERT(0);
        }
        else
        {
            pt_xent->pt_xent_ops = pt_prnt_xent->pt_xent_ops;
        }
    }

    return pt_xent;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_mount
 *
 * Description: Rootfs mount function.
 *
 * Inputs:  pt_mount
 *              The reference to mount object.
 *          pt_dev_xent
 *              Xentry of the device. No use for rootfs.
 *          pv_data
 *              No use.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_mount(
    VOID            *pv_data)
{
    FM_MOUNT_FS_ARG_T *pt_mount_arg;
    FM_MOUNT_T        *pt_mount;
    ROOTFS_ENTRY_T    **ppt_rent;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_mount_arg = (FM_MOUNT_FS_ARG_T *)pv_data;

    ASSERT(pt_mount_arg->pt_mount != NULL);

    pt_mount = pt_mount_arg->pt_mount;

    ppt_rent = x_mem_alloc(sizeof(ROOTFS_ENTRY_T *));
    if (ppt_rent == NULL)
    {
        return FMR_CORE;
    }
    *ppt_rent = pt_root_rent;

    pt_mount->t_fs_info.e_type           = FM_TYPE_ROOTFS;
    pt_mount->t_fs_info.ui8_blk_size     = (UINT64)ROOTFS_BLOCK_SIZE;
    pt_mount->t_fs_info.ui8_total_blk    = (UINT64)ROOTFS_BLOCK_NUM;
    pt_mount->t_fs_info.ui8_free_blk     = (UINT64)ROOTFS_BLOCK_NUM;
    pt_mount->t_fs_info.ui8_avail_blk    = (UINT64)ROOTFS_BLOCK_NUM;
    pt_mount->t_fs_info.ui8_files        = 0;
    pt_mount->t_fs_info.ui2_max_name_len = ROOTFS_FILENAME_MAX;
    
    pt_mount->pt_root_xent->pv_fs_node   = ppt_rent;
    pt_mount->pt_root_xent->pt_ufs_ops   = &t_rootfs_ufs_ops;
    pt_mount->pt_root_xent->pt_xent_ops  = &t_rootfs_xent_ops;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_umount
 *
 * Description: Cannot umount the rootfs.
 *
 * Inputs: pv_data
 *              No use.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *
 ----------------------------------------------------------------------------*/
static INT32 rootfs_umount(VOID *pv_data)
{
    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rootfs_lookup
 *
 * Description: Lookup a name in the directory.
 *
 * Inputs:  pv_data
 *              The reference to FM_LOOKUP_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_ENTRY
 *              Not found.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_lookup(VOID *pv_data)
{
    FM_LOOKUP_ARG_T *pt_arg;
    ROOTFS_ENTRY_T  *pt_this_rent;
    ROOTFS_ENTRY_T  *pt_new_rent;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LOOKUP_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_prnt_xent != NULL);
    ASSERT(pt_arg->ps_name      != NULL);
    
    pt_this_rent = ROOTFS_GET_ENT(pt_arg->pt_prnt_xent);

    pt_new_rent = _rootfs_lookup_entry(pt_this_rent, pt_arg->ps_name);
    if (pt_new_rent == NULL)
    {
        return FMR_NO_ENTRY;
    }

    if ( pt_arg->pt_xent == NULL)
    {
        pt_arg->pt_xent = _rootfs_make_xentry(pt_arg->pt_prnt_xent,
                                          pt_new_rent);
              
        if (pt_arg->pt_xent == NULL)
        {
            return FMR_CORE;
        }
    }
    else
    {/* There is already a pt_xent, we don't have to make it. */
        
    }
        
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_make_entry
 *
 * Description: Make a new entry.
 *
 * Inputs:  pv_data
 *              The reference to FM_MKENT_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NAME_TOO_LONG
 *              Name is too long.
 *          FMR_FILE_SYSTEM_FULL
 *              There is no space for new entry.
 *          FMR_EXIST
 *              The entry already exists.
 *          FMR_INVAL
 *              Invalid entry type.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_make_entry(VOID *pv_data)
{
    FM_MKENT_ARG_T  *pt_arg;
    ROOTFS_ENTRY_T  *pt_this_rent;
    ROOTFS_ENTRY_T  *pt_new_rent;
    INT32           i4_ret;
    UINT64          ui8_time;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_MKENT_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_this_xent != NULL);
    ASSERT(pt_arg->ps_name      != NULL);
    ASSERT(IS_XENTRY_DIR(pt_arg->pt_this_xent));

    if (x_strlen(pt_arg->ps_name) > ROOTFS_FILENAME_MAX)
    {
        return FMR_NAME_TOO_LONG;
    }

    if (ui4_free_block < ui4_meta_block)
    {
        return FMR_FILE_SYSTEM_FULL;
    }
    
    pt_this_rent = ROOTFS_GET_ENT(pt_arg->pt_this_xent);
    ASSERT(pt_this_rent != NULL);

    ASSERT(_rootfs_lookup_entry(pt_this_rent, pt_arg->ps_name) == NULL);

    pt_new_rent = x_mem_calloc(1, sizeof(ROOTFS_ENTRY_T));
    if (pt_new_rent == NULL)
    {
        return FMR_CORE;
    }

    x_strncpy(pt_new_rent->ps_name, pt_arg->ps_name, ROOTFS_FILENAME_MAX);
    pt_new_rent->ui4_mode = pt_arg->ui4_mode;
    
    switch (pt_arg->ui4_mode & FM_MODE_TYPE_MASK)
    {
        case FM_MODE_TYPE_DIR:
            pt_new_rent->z_file_size = ui4_meta_size;
            /* Fall through */
        case FM_MODE_TYPE_FILE:
            pt_new_rent->ui4_ent_id = (UINT32)pt_new_rent;
            break;

        case FM_MODE_TYPE_BLK:
        case FM_MODE_TYPE_CHR:
            pt_new_rent->z_file_size = ui4_meta_size;
            pt_new_rent->ui4_ent_id = (((UINT32)pt_arg->ui2_dev & 0x0000ffff) |
                                       ((UINT32)pt_arg->ui2_unit << 16));
            break;

        case FM_MODE_TYPE_LINK:
        default:
            DBG_ERROR(("[FM] ERR: Invalid file type\n"));
            i4_ret = FMR_INVAL;
            goto FAIL;
    }

    RTC_GetTimeDate(&ui8_time);

    pt_new_rent->ui8_create_time = ui8_time;
    pt_new_rent->ui8_access_time = pt_new_rent->ui8_create_time;
    pt_new_rent->ui8_modify_time = pt_new_rent->ui8_create_time;

    pt_arg->pt_new_xent = _rootfs_make_xentry(pt_arg->pt_this_xent,
                                              pt_new_rent);

    if (pt_arg->pt_new_xent == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    if (FM_IS_BLK(pt_arg->ui4_mode))
    {
        if (FM_IS_VIRT(pt_arg->ui4_mode))
        {
            pt_new_rent->u.pt_part = NULL;
        }
        else if (FM_IS_TRUE(pt_arg->ui4_mode))
        {
            pt_new_rent->u.h_part_tbl = NULL_HANDLE;
        }
    }

    _rootfs_add_entry(pt_this_rent, pt_new_rent);

    ui4_free_size  -= ui4_meta_size;
    ui4_used_size  += ui4_meta_size;
    ui4_free_block -= ui4_meta_block;
    ui4_used_block += ui4_meta_block;
    
    return FMR_OK;

FAIL:

    x_mem_free(pt_new_rent);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_write
 *
 * Description: Write data to a file entry.
 *
 * Inputs:  pv_data
 *              The reference to FM_RW_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_FILE_SYSTEM_FULL
 *              No more space.
 *          FMR_IO_FAIL
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_write(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg;
    ROOTFS_ENTRY_T  *pt_rent;
    SIZE_T          z_alloc;
    UINT32          ui4_more_bytes;
    UINT64          ui8_time;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_RW_ARG_T *)pv_data;
    
    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);
    
    ASSERT(pt_rent != NULL);

    if ((pt_arg->ui8_offset >= ROOTFS_SIZE) ||
        (pt_arg->ui4_count > ui4_free_size))
    {
        return FMR_FILE_SYSTEM_FULL;
    }

    z_alloc = CAL_BLOCK_SIZE((SIZE_T)pt_arg->ui8_offset +
                             (SIZE_T)pt_arg->ui4_count);
    if (z_alloc > pt_rent->z_alloc_size)
    {
        z_alloc -= pt_rent->z_alloc_size;
    }
    else
    {
        z_alloc = 0;
    }
    
    if (z_alloc > (ui4_free_block * ROOTFS_BLOCK_SIZE))
    {
        return FMR_FILE_SYSTEM_FULL;
    }

    if (z_alloc != 0)
    {
        CHAR *pc_buf;

        pc_buf = (CHAR *)x_mem_realloc(pt_rent->pv_data,
                                       pt_rent->z_alloc_size + z_alloc);
        if (pc_buf == NULL)
        {
            return FMR_CORE;
        }
        x_memset(pc_buf + pt_rent->z_alloc_size, 0, z_alloc);
        pt_rent->pv_data = (VOID *)pc_buf;
        pt_rent->z_alloc_size += z_alloc;
        
        ui4_free_block -= (z_alloc / ROOTFS_BLOCK_SIZE);
        ui4_used_block += (z_alloc / ROOTFS_BLOCK_SIZE);
    }

    x_memcpy(((CHAR *)pt_rent->pv_data) + pt_arg->ui8_offset,
             pt_arg->pv_data,
             (SIZE_T)pt_arg->ui4_count);

    if (pt_arg->ui8_offset + pt_arg->ui4_count > pt_rent->z_file_size)
    {
        ui4_more_bytes = pt_arg->ui8_offset + pt_arg->ui4_count -
                         pt_rent->z_file_size;

        pt_rent->z_file_size += (SIZE_T)ui4_more_bytes;
        ui4_free_size        -= ui4_more_bytes;
        ui4_used_size        += ui4_more_bytes;
    }

    pt_arg->ui4_done_bytes = pt_arg->ui4_count;

    RTC_GetTimeDate(&ui8_time);

    pt_rent->ui8_modify_time = ui8_time;
    pt_rent->ui8_access_time = pt_rent->ui8_modify_time;

    PRINT_FS_INFO();

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_read
 *
 * Description: Read data from a file.
 *
 * Inputs:  pv_data
 *              The reference to FM_RW_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_EOF
 *              Reaches end of file.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_read(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg;
    ROOTFS_ENTRY_T  *pt_rent;
    UINT32          ui4_read_bytes;
    INT32           i4_ret;
    UINT64          ui8_time;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_RW_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);

    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);

    ASSERT(pt_rent != NULL);

    
    if (pt_arg->ui8_offset >= pt_rent->z_file_size)
    {
        return FMR_EOF;
    }

    if (pt_rent->z_file_size - pt_arg->ui8_offset >= pt_arg->ui4_count)
    {
        ui4_read_bytes = pt_arg->ui4_count;
        i4_ret = FMR_OK;
    }
    else
    {
        ui4_read_bytes = pt_rent->z_file_size - pt_arg->ui8_offset;
        i4_ret = FMR_EOF;
    }

    x_memcpy(pt_arg->pv_data,
             ((CHAR *)pt_rent->pv_data) + pt_arg->ui8_offset,
             ui4_read_bytes);

    pt_arg->ui4_done_bytes = ui4_read_bytes;

    RTC_GetTimeDate(&ui8_time);

    pt_rent->ui8_access_time = ui8_time;

    PRINT_FS_INFO();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_lseek
 *
 * Description: Change the current position of an opened file.
 *
 * Inputs:  pv_data
 *              The reference to FM_LSEEK_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_FILE_TOO_LARGE
 *              Exceed the maximum file size.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_lseek(VOID *pv_data)
{
    FM_LSEEK_ARG_T  *pt_arg;
    ROOTFS_ENTRY_T  *pt_rent;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LSEEK_ARG_T *)pv_data;
    
    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);
    
    ASSERT(pt_rent != NULL);
    
    if (pt_arg->i8_offset > 0)
    {
        UINT64 ui8_new_off;

        if (pt_arg->ui1_whence == FM_SEEK_BGN)
        {
            ui8_new_off = pt_arg->i8_offset;
        }
        else if (pt_arg->ui1_whence == FM_SEEK_CUR)
        {
            ui8_new_off = pt_arg->i8_offset + pt_arg->ui8_cur_pos;
        }
        else
        {
            ui8_new_off = pt_arg->i8_offset + pt_rent->z_file_size;
        }

        if (ui8_new_off >= ROOTFS_SIZE)
        {
            return FMR_FILE_TOO_LARGE;
        }

        pt_arg->ui8_cur_pos = ui8_new_off;
    }
    else
    {
        ASSERT(pt_arg->ui1_whence == FM_SEEK_END);

        if (pt_arg->i8_offset + (INT64)pt_rent->z_file_size > 0)
        {
            pt_arg->ui8_cur_pos = pt_arg->i8_offset + pt_rent->z_file_size;
        }
        else
        {
            pt_arg->ui8_cur_pos = 0;
        }
    }
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_close
 *
 * Description: Close an opened file. We just update the access time.
 *
 * Inputs:  pv_data
 *              The reference to FM_XENTRY_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_close(VOID *pv_data)
{
    DEBUG_TRACE();

    /* Nothing to do */
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_get_info
 *
 * Description: Get the information of an entry.
 *
 * Inputs:  pv_data
 *              The reference to FM_FILE_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_get_info(VOID *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg;
    ROOTFS_ENTRY_T      *pt_rent;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_FILE_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);

    ASSERT(pt_rent != NULL);

    pt_arg->pt_info->ui4_inode    = pt_rent->ui4_ent_id;
    pt_arg->pt_info->ui4_mode     = pt_rent->ui4_mode;
    pt_arg->pt_info->ui4_uid      = 0;
    pt_arg->pt_info->ui4_gid      = 0;
    pt_arg->pt_info->ui8_size     = pt_rent->z_file_size;
    pt_arg->pt_info->ui8_offset   = 0;
    pt_arg->pt_info->ui4_blk_size = pt_rent->z_alloc_size;
    pt_arg->pt_info->ui8_blk_cnt  = pt_rent->z_alloc_size / ROOTFS_BLOCK_SIZE;
    pt_arg->pt_info->ui8_create_time = pt_rent->ui8_create_time;
    pt_arg->pt_info->ui8_access_time = pt_rent->ui8_access_time;
    pt_arg->pt_info->ui8_modify_time = pt_rent->ui8_modify_time;

    /* we should add for the case of virtual devices */

    if (FM_IS_TRUE(pt_rent->ui4_mode))
    {
        pt_arg->pt_info->ui8_size = pt_arg->pt_xent->pt_dev->ui8_space;
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_set_info
 *
 * Description: Set the properties of an entry.
 *
 * Inputs:  pv_data
 *              The reference to FM_FILE_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_set_info(VOID *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg;
    FM_FILE_INFO_T      *pt_info;
    ROOTFS_ENTRY_T      *pt_rent;
    SIZE_T              z_new_size;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_FILE_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);

    ASSERT(pt_rent != NULL);
    
    pt_info = pt_arg->pt_info;

    ASSERT(pt_info != NULL);

    /* set mode */
    if (pt_info->ui4_mode != FM_SET_MODE_NOVAL)
    {
        pt_rent->ui4_mode = (pt_rent->ui4_mode & FM_MODE_TYPE_MASK) |
                            (pt_info->ui4_mode & FM_MODE_PERM_MASK);
    }
    
    /* handle chop / truncate / extend */
    if ((pt_info->ui8_offset != FM_SET_SIZE_NOVAL) ||
        (pt_info->ui8_size   != FM_SET_SIZE_NOVAL))
    {
        UINT64  ui8_offset;

        if (pt_info->ui8_offset != FM_SET_SIZE_NOVAL)
        {
            z_new_size = (pt_info->ui8_offset > (UINT64)pt_rent->z_file_size) ?
                          0 :
                          pt_rent->z_file_size - (SIZE_T)pt_info->ui8_offset;
            ui8_offset = pt_info->ui8_offset;
        }
        else
        {
            z_new_size = pt_info->ui8_size;
            ui8_offset = 0;
        }
        
        if (z_new_size != pt_rent->z_file_size)
        {
            SIZE_T  z_alloc = 0;
            CHAR    *pc_buf = NULL;

            if (z_new_size != 0)
            {
                z_alloc = CAL_BLOCK_SIZE(z_new_size);
                pc_buf = (CHAR *)x_mem_alloc(z_alloc);
                if (pc_buf == NULL)
                {
                    return FMR_CORE;
                }

                x_memcpy(pc_buf,
                         (char *)pt_rent->pv_data +
                         (SIZE_T)ui8_offset,
                         (z_new_size < pt_rent->z_file_size) ?
                         z_new_size : pt_rent->z_file_size);
            }
        
            x_mem_free(pt_rent->pv_data);
            pt_rent->pv_data = (VOID *)pc_buf;

            ui4_free_size  += (pt_rent->z_file_size - z_new_size);
            ui4_used_size  -= (pt_rent->z_file_size - z_new_size);
            ui4_free_block += (pt_rent->z_alloc_size - z_alloc)/ROOTFS_BLOCK_SIZE;
            ui4_used_block -= (pt_rent->z_alloc_size - z_alloc)/ROOTFS_BLOCK_SIZE;

            pt_rent->z_file_size  = z_new_size;
            pt_rent->z_alloc_size = z_alloc;
        }
    }

    if (pt_info->ui8_create_time != FM_SET_TIME_NOVAL)
    {
        pt_rent->ui8_create_time = pt_info->ui8_create_time;
    }
    
    if (pt_info->ui8_access_time != FM_SET_TIME_NOVAL)
    {
        pt_rent->ui8_access_time = pt_info->ui8_access_time;
    }

    if (pt_info->ui8_modify_time != FM_SET_TIME_NOVAL)
    {
        pt_rent->ui8_modify_time = pt_info->ui8_modify_time;
    }

    PRINT_FS_INFO();

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rootfs_get_dir_info
 *
 * Description: Get the information of an entry.
 *
 * Inputs:  pv_data
 *              The reference to FM_DIR_INFO_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_get_dir_info(VOID *pv_data)
{
    FM_DIR_INFO_ARG_T  *pt_arg;
    ROOTFS_ENTRY_T      *pt_rent;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_arg = (FM_DIR_INFO_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);

    ASSERT(pt_rent != NULL);

    pt_arg->pt_info->ui4_dir_num = 2;/* "." and ".." */
    pt_arg->pt_info->ui4_file_num = 0;

    pt_rent = pt_rent->pt_child;

    while(pt_rent)
    {
        pt_arg->pt_info->ui4_dir_num++;
        pt_rent = pt_rent->pt_next;
        /* so far, there is no posibility to create a "FILE" in rootfs */
    }

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rootfs_read_dir
 *
 * Description: Read directory entries.
 *
 * Inputs:  pv_data
 *              The reference to FM_READ_DIR_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_EOF
 *              Reaches end of file.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_read_dir(VOID *pv_data)
{
    FM_READ_DIR_ARG_T   *pt_arg;
    FM_DIR_ENTRY_T      *pt_dir_ent;
    ROOTFS_ENTRY_T      *pt_rent;
    ROOTFS_ENTRY_T      *pt_child_rent;
    UINT64              ui8_offset;
    UINT32              ui4_read_cnt = 0;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_READ_DIR_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_arg->pt_xent));
    
    pt_rent = ROOTFS_GET_ENT(pt_arg->pt_xent);
    
    ASSERT(pt_rent != NULL);
    
    pt_dir_ent = pt_arg->pt_dir_ent;

    ASSERT(pt_dir_ent != NULL);

    ui8_offset = pt_arg->ui8_offset;
    
    if ( pt_arg->ui4_ent_cnt >= 2)
    {
        if ( pt_arg->ui8_offset == 0)
        { /* there are 2 rooms for ".", and ".." */
            x_strcpy(pt_dir_ent->s_name, ".");
            pt_dir_ent->ui1_len = x_strlen(".");
            
            pt_dir_ent++;
            ui4_read_cnt++;

            x_strcpy(pt_dir_ent->s_name, "..");
            pt_dir_ent->ui1_len = x_strlen( "..");

            pt_dir_ent++;
            ui4_read_cnt++;            
        }
        else if ( pt_arg->ui8_offset == 1)
        {
            x_strcpy(pt_dir_ent->s_name, "..");
            pt_dir_ent->ui1_len = x_strlen( "..");

            pt_dir_ent++;
            ui4_read_cnt++;
            ui8_offset--;
        }
        else
        {
            ui8_offset -= 2;
        }
    }
    else if ( pt_arg->ui4_ent_cnt >= 1)
    {
        if ( pt_arg->ui8_offset == 0)
        {
            x_strcpy(pt_dir_ent->s_name, ".");
            pt_dir_ent->ui1_len = x_strlen(".");
            
            pt_dir_ent++;
            ui4_read_cnt++;
        }
        else if ( pt_arg->ui8_offset == 1)
        {
            x_strcpy(pt_dir_ent->s_name, "..");
            pt_dir_ent->ui1_len = x_strlen( "..");

            pt_dir_ent++;
            ui4_read_cnt++;
            ui8_offset--;
        }
        else
        {
            ui8_offset -= 2;
        }
    }
    else
    { /* zero pt_arg->ui4_ent_cnt? */
        return FMR_ARG;
    }
    
    for (pt_child_rent = pt_rent->pt_child;
         (pt_child_rent != NULL) && (ui4_read_cnt < pt_arg->ui4_ent_cnt);
         pt_child_rent = pt_child_rent->pt_next)
    {
        if (ui8_offset != 0)
        {
            ui8_offset--;
            /* skip the offset entries */
            continue;
        }

        x_strncpy(pt_dir_ent->s_name, pt_child_rent->ps_name, ROOTFS_FILENAME_MAX);
        pt_dir_ent->ui1_len = x_strlen(pt_child_rent->ps_name);

        pt_dir_ent->t_file_info.ui4_inode    = pt_child_rent->ui4_ent_id;
        pt_dir_ent->t_file_info.ui4_mode     = pt_child_rent->ui4_mode;
        pt_dir_ent->t_file_info.ui4_uid      = 0;
        pt_dir_ent->t_file_info.ui4_gid      = 0;
        pt_dir_ent->t_file_info.ui8_size     = pt_child_rent->z_file_size;
        pt_dir_ent->t_file_info.ui8_offset   = 0;
        pt_dir_ent->t_file_info.ui4_blk_size = pt_child_rent->z_alloc_size;
        pt_dir_ent->t_file_info.ui8_blk_cnt  = pt_child_rent->z_alloc_size / ROOTFS_BLOCK_SIZE;
        pt_dir_ent->t_file_info.ui8_create_time = pt_child_rent->ui8_create_time;
        pt_dir_ent->t_file_info.ui8_access_time = pt_child_rent->ui8_access_time;
        pt_dir_ent->t_file_info.ui8_modify_time = pt_child_rent->ui8_modify_time;
        
        pt_dir_ent++;
        ui4_read_cnt++;
    }

    pt_arg->ui4_read_cnt = ui4_read_cnt;
    pt_arg->ui8_offset  += ui4_read_cnt;
    
    return (ui4_read_cnt == 0) ? FMR_EOF : FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_delete
 *
 * Description: Remove a file/directory.
 *
 * Inputs:  pv_data
 *              The reference to FM_DELETE_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_DIR_NOT_EMPTY
 *              The directory is not empty.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_delete(VOID *pv_data)
{
    FM_DELETE_ARG_T *pt_del_arg;
    FM_XENTRY_T     *pt_xent;
    ROOTFS_ENTRY_T  *pt_rent;

    DEBUG_TRACE();

    ASSERT(pv_data != NULL);
    
    pt_del_arg   = (FM_DELETE_ARG_T *)pv_data;
    pt_xent      = (FM_XENTRY_T *)pt_del_arg->pt_xent;

    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    pt_rent = ROOTFS_GET_ENT(pt_xent);

    ASSERT(pt_rent != NULL);

    if (FM_IS_DIR(pt_rent->ui4_mode))
    {
        if (pt_rent->pt_child != NULL)
        {
            return FMR_DIR_NOT_EMPTY;
        }
    }

    _rootfs_del_entry(pt_rent);

    if (pt_xent->ui4_ref_cnt == 0)
    {
        SIZE_T z_free;

        z_free = pt_rent->z_alloc_size + ui4_meta_size;
        ui4_free_size  += (z_free);
        ui4_used_size  -= (z_free);
        ui4_free_block += (z_free / ROOTFS_BLOCK_SIZE);
        ui4_used_block -= (z_free / ROOTFS_BLOCK_SIZE);

        x_mem_free(pt_rent->pv_data);
        x_mem_free(pt_rent);
    }
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_rename
 *
 * Description: Rename a file/directory.
 *
 * Inputs:  pv_data
 *              The reference to FM_DELETE_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NAME_TOO_LONG
 *              The target name is too long.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_rename(VOID *pv_data)
{
    FM_RENAME_ARG_T *pt_rename_arg;
    ROOTFS_ENTRY_T  *pt_src_prnt_rent;
    ROOTFS_ENTRY_T  *pt_dst_prnt_rent;
    ROOTFS_ENTRY_T  *pt_rent;
    const CHAR      *ps_name;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_rename_arg = (FM_RENAME_ARG_T *)pv_data;

    ASSERT(pt_rename_arg->pt_src_prnt_xent != NULL);
    ASSERT(pt_rename_arg->pt_dst_prnt_xent != NULL);
    ASSERT(pt_rename_arg->pt_xent          != NULL);
    ASSERT(pt_rename_arg->ps_name          != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_src_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_dst_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_rename_arg->pt_xent));

    pt_src_prnt_rent = ROOTFS_GET_ENT(pt_rename_arg->pt_src_prnt_xent);
    pt_dst_prnt_rent = ROOTFS_GET_ENT(pt_rename_arg->pt_dst_prnt_xent);
    pt_rent = ROOTFS_GET_ENT(pt_rename_arg->pt_xent);
    ps_name = pt_rename_arg->ps_name;

    if (x_strlen(ps_name) > ROOTFS_FILENAME_MAX)
    {
        return FMR_NAME_TOO_LONG;
    }

    if (pt_src_prnt_rent != pt_dst_prnt_rent)
    {
        _rootfs_del_entry(pt_rent);
        _rootfs_add_entry(pt_dst_prnt_rent, pt_rent);
    }
    x_strcpy(pt_rent->ps_name, ps_name);

    return FMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: rootfs_sync
 *
 * Description: -
 *
 * Inputs:  pv_data              
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 ----------------------------------------------------------------------------*/
static INT32 rootfs_sync(VOID *pv_data)
{
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: rootfs_inval
 *
 * Description: Return FMR_INVAL to indicate this is an invalid file system
 *              operation.
 *
 * Inputs:  pv_data              
 *
 * Outputs: -
 *
 * Returns: FMR_INVAL
 *              Invalid function.
 ----------------------------------------------------------------------------*/
static INT32 rootfs_inval(VOID *pv_data)
{
    return FMR_INVAL;
}


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: rootfs_init
 *
 * Description: Initialize the rootfs and return the FM_FS_DESC_T structure.
 *
 * Inputs:  pv_data
 *              File system private init data.
 *
 * Outputs: -
 *
 * Returns: Reference to an object of FM_FS_DESC_T type on success.
 *          Otherwise NULL is returned.
 ----------------------------------------------------------------------------*/
FM_FS_DESC_T *rootfs_init(VOID *pv_data)
{
    FM_FS_DESC_T *pt_fs_desc = NULL;
    UINT64 ui8_time;

    DEBUG_TRACE();

    pt_fs_desc = x_mem_alloc(sizeof(FM_FS_DESC_T));
    if (pt_fs_desc != NULL)
    {
        x_strcpy(pt_fs_desc->ps_name, "rootfs");
        pt_fs_desc->e_fs_type  = FM_TYPE_ROOTFS;
        pt_fs_desc->i4_ref_cnt = 0;
        pt_fs_desc->pt_ufs_op  = &t_rootfs_ufs_ops;

        ui4_free_size  = ROOTFS_SIZE;
        ui4_used_size  = 0;
        ui4_free_block = ROOTFS_BLOCK_NUM;
        ui4_used_block = 0;
        ui4_meta_size = CAL_BLOCK_SIZE(sizeof(ROOTFS_ENTRY_T));
        ui4_meta_block = (ui4_meta_size / ROOTFS_BLOCK_SIZE);

        RTC_GetTimeDate(&ui8_time);

        pt_root_rent = &t_root_rent;
        x_memset(pt_root_rent, 0, sizeof(ROOTFS_ENTRY_T));
        x_strcpy(pt_root_rent->ps_name, "/");
        pt_root_rent->ui4_ent_id      = (UINT32)pt_root_rent;
        pt_root_rent->ui4_mode        = (FM_MODE_TYPE_DIR | 0755);
        pt_root_rent->ui8_create_time = ui8_time;
        pt_root_rent->ui8_access_time = pt_root_rent->ui8_create_time;
        pt_root_rent->ui8_modify_time = pt_root_rent->ui8_create_time;
    }

    return pt_fs_desc;
}

