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
 * $RCSfile: fm_ufs.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/18 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: f7742db8090e59e0db8d6cfe206313ba $
 *
 * Description: 
 *         This header file contains macros and types for File Manager UFS.
 *---------------------------------------------------------------------------*/

#ifndef _FM_UFS_H_
#define _FM_UFS_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_lnk_list.h"
#include "x_end_cnvt.h"
#include "fm_dev.h"
#include "u_fm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
typedef struct _FM_XENTRY_T FM_XENTRY_T;
typedef struct _FM_MOUNT_T  FM_MOUNT_T;
struct _FM_BUF_T;

/*
 *  UFS operations
 */
typedef INT32 (*fm_ufs_fct)(
    VOID            *pv_data);

typedef INT32 (*fm_mount_fct)(
    FM_MOUNT_T      *pt_mount,
    FM_XENTRY_T     *pt_dev_xent,
    VOID            *pv_data);

typedef struct _FM_UFS_OP_T
{
    fm_ufs_fct      pf_newfs;
    fm_ufs_fct      pf_chkfs;
    fm_ufs_fct      pf_mount;
    fm_ufs_fct      pf_umount;
} FM_UFS_OP_T;

/*
 *  Xentry operations
 */
typedef struct _FM_XENTRY_OP_T
{
    fm_ufs_fct      pf_lookup;
    fm_ufs_fct      pf_read;
    fm_ufs_fct      pf_write;
    fm_ufs_fct      pf_read_async;
    fm_ufs_fct      pf_write_async;
    fm_ufs_fct      pf_lseek;
    fm_ufs_fct      pf_close;
    fm_ufs_fct      pf_get_info;
    fm_ufs_fct      pf_set_info;
    fm_ufs_fct      pf_get_dir_info;
    fm_ufs_fct      pf_make_entry;
    fm_ufs_fct      pf_open_dir;
    fm_ufs_fct      pf_close_dir;
    fm_ufs_fct      pf_read_dir;
    fm_ufs_fct      pf_delete;
    fm_ufs_fct      pf_rename;
    fm_ufs_fct      pf_sync;
    fm_ufs_fct      pf_seek_dir;
    fm_ufs_fct      pf_free_fs_node;
} FM_XENTRY_OP_T;

/*
 *  Xentry type and flags
 */
struct _FM_XENTRY_T
{
    HANDLE_T        h_mtx;
    FM_DEVICE_T     *pt_dev;
    UINT32          ui4_mode;
    FM_MOUNT_T      *pt_mount;
    FM_MOUNT_T      *pt_mounted;
    FM_UFS_OP_T     *pt_ufs_ops;
    FM_XENTRY_OP_T  *pt_xent_ops;
    VOID            *pv_fs_node;
    UINT32          ui4_flags;
    UINT32          ui4_ref_cnt; /* if it is 0, purge it to LRU */
    UINT16          ui2_lock_cnt;
    UINT16          ui2_want_cnt;
    HANDLE_T        h_rw_lock;
    DLIST_ENTRY_T(_FM_XENTRY_T) t_qlink;

    DLIST_T(_FM_BUF_T) t_buf_list;

    /* Fields for name cache */
    struct _FM_XENTRY_T *pt_prnt_xent;
    CHAR                *ps_name;
    SIZE_T              z_name_size;
    SLIST_ENTRY_T(_FM_XENTRY_T) t_hlink;
};

#define FM_XENTRY_FLAG_FREE         0x00000000
#define FM_XENTRY_FLAG_BUSY         0x00000001
#define FM_XENTRY_FLAG_LRU          0x00000002
#define FM_XENTRY_FLAG_USED         0x00000003
#define FM_XENTRY_FLAG_DELETE       0x00000004
#define FM_XENTRY_FLAG_RENAME       0x00000008
#define FM_XENTRY_FLAG_INVAL        0x0000000C /* delete or rename */
#define FM_XENTRY_FLAG_LOCKED       0x10000000
#define FM_XENTRY_FLAG_MOUNT_ROOT   0x00000010
#define FM_XENTRY_FLAG_MOUNTED      0x00000020
#define FM_XENTRY_FLAG_UNLINKED     0x00000040

/*
 *  File system descriptor
 */
typedef struct _FM_FS_DESC_T
{
    CHAR             ps_name[16];
    FM_FS_TYPE_T     e_fs_type;
    INT32            i4_ref_cnt;
    FM_UFS_OP_T      *pt_ufs_op;
    SLIST_ENTRY_T(_FM_FS_DESC_T) t_link;
} FM_FS_DESC_T;

/*
 *  Partition structure.
 */
typedef struct _FM_PART_T
{
    UINT8   ui1_drv;
    UINT8   ui1_start_chs[3]; /* starting CHS for INT 13 */
    UINT8   ui1_type;
    UINT8   ui1_end_chs[3];   /* skip ending CHS for INT 13 */
    UINT32  ui4_start_lba;
    UINT32  ui4_sectors;

    UINT32  ui4_part_idx;
    BOOL    b_entried;

    BOOL    b_mnt;

    CHAR    ps_part_name[32];
    CHAR    ps_part_path[32];
    CHAR    ps_mnt_path[32];

    FM_MNT_CB_T t_mnt_cb;
} FM_PART_T;

typedef struct _FM_PART_TBL_ENT_T
{
    FM_PART_T  t_part;
    
    struct _FM_PART_TBL_ENT_T *pt_next;
    struct _FM_PART_TBL_ENT_T *pt_prev;
} FM_PART_TBL_ENT_T;

typedef struct _FM_PART_TBL_T
{
    UINT32  ui4_parts;
    FM_PART_TBL_ENT_T  t_prm[4];
} FM_PART_TBL_T;

/*
 *  Mount structure
 */
struct _FM_MOUNT_T
{
    FM_FS_DESC_T    *pt_fs_desc;
    FM_XENTRY_T     *pt_mp_xent;
    FM_XENTRY_T     *pt_root_xent;
    FM_XENTRY_T     *pt_dev_xent;
    SLIST_ENTRY_T(_FM_MOUNT_T) t_link;

    FM_FS_INFO_T    t_fs_info;
    FM_PART_T       t_part_info;
    VOID            *pv_priv;
    UINT32          ui4_flags;
#define FM_MNT_FLAG_ROOTFS 0x00000001

    HANDLE_T        h_sync_sema;
    HANDLE_T        h_lock_mtx;

    /* Opened file desc and dir label in this file system */
    DLIST_T(_FM_MTX_OBJ_T) t_opened_obj_q;
};

/*
 *  Lookup flags
 */
#define FM_UFS_CREATE       ((UINT16) 0x0001)
#define FM_UFS_LOOKUP       ((UINT16) 0x0002)
#define FM_UFS_DELETE       ((UINT16) 0x0004)
#define FM_UFS_RENAME       ((UINT16) 0x0008)
#define FM_UFS_LOCK_LEAF    ((UINT16) 0x0001)
#define FM_UFS_LOCK_PARENT  ((UINT16) 0x0002)

/*
 *  Xentry related macros
 */
#define IS_XENTRY_BUSY(pxe)     \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_BUSY)
    
#define IS_XENTRY_LRU(pxe)      \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_LRU)
    
#define IS_XENTRY_USED(pxe)     \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_USED)
    
#define IS_XENTRY_INVAL(pxe)    \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_INVAL)
    
#define IS_XENTRY_LOCKED(pxe)   \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_LOCKED)
    
#define IS_XENTRY_MOUNT_ROOT(pxe) \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_MOUNT_ROOT)
    
#define IS_XENTRY_MOUNTED(pxe)  \
    ((pxe)->ui4_flags & FM_XENTRY_FLAG_MOUNTED)
    
#define IS_XENTRY_FILE(pxe)     \
    (FM_IS_FILE((pxe)->ui4_mode))
    
#define IS_XENTRY_DIR(pxe)      \
    (FM_IS_DIR((pxe)->ui4_mode))
    
#define IS_XENTRY_BLK(pxe)      \
    (FM_IS_BLK((pxe)->ui4_mode))
    
#define IS_XENTRY_CHR(pxe)      \
    (FM_IS_CHR((pxe)->ui4_mode))
    
#define IS_XENTRY_LNK(pxe)      \
    (FM_IS_LINK((pxe)->ui4_mode))

#define IS_XENTRY_TRUE(pxe)      \
    (FM_IS_TRUE((pxe)->ui4_mode))

#define IS_XENTRY_VIRT(pxe)      \
    (FM_IS_VIRT((pxe)->ui4_mode))

#define XENTRY_OP_LOOKUP(xent)                  \
    (*((xent)->pt_xent_ops->pf_lookup))
    
#define XENTRY_OP_READ(xent)                    \
    (*((xent)->pt_xent_ops->pf_read))
    
#define XENTRY_OP_WRITE(xent)                   \
    (*((xent)->pt_xent_ops->pf_write))
    
#define XENTRY_OP_CLOSE(xent)                   \
    (*((xent)->pt_xent_ops->pf_close))
    
#define XENTRY_OP_READ_ASYNC(xent)              \
    (*((xent)->pt_xent_ops->pf_read_async))
    
#define XENTRY_OP_WRITE_ASYNC(xent)             \
    (*((xent)->pt_xent_ops->pf_write_async))
    
#define XENTRY_OP_LSEEK(xent)                   \
    (*((xent)->pt_xent_ops->pf_lseek))

#define XENTRY_OP_SEEK_DIR(xent)        \
    (*((xent)->pt_xent_ops->pf_seek_dir))
    
#define XENTRY_OP_GET_INFO(xent)                \
    (*((xent)->pt_xent_ops->pf_get_info))
    
#define XENTRY_OP_SET_INFO(xent)                \
    (*((xent)->pt_xent_ops->pf_set_info))

#define XENTRY_OP_GET_DIR_INFO(xent)                \
    (*((xent)->pt_xent_ops->pf_get_dir_info))

#define XENTRY_OP_MAKE_ENTRY(xent)              \
    (*((xent)->pt_xent_ops->pf_make_entry))
    
#define XENTRY_OP_READ_DIR(xent)                \
    (*((xent)->pt_xent_ops->pf_read_dir))
    
#define XENTRY_OP_DELETE(xent)                  \
    (*((xent)->pt_xent_ops->pf_delete))
    
#define XENTRY_OP_RENAME(xent)                  \
    (*((xent)->pt_xent_ops->pf_rename))
    
#define XENTRY_OP_SYNC(xent)                    \
    (*((xent)->pt_xent_ops->pf_sync))
    
/*
 *  Xentry operation arguments
 */
typedef struct _FM_LOOKUP_ARG_T
{
    FM_XENTRY_T     *pt_prnt_xent;
    FM_XENTRY_T     *pt_xent;
    CHAR            *ps_name;
} FM_LOOKUP_ARG_T;

typedef struct _FM_MKENT_ARG_T
{
    FM_XENTRY_T     *pt_this_xent;
    const CHAR      *ps_name;
    UINT32          ui4_mode;
    UINT16          ui2_dev;
    UINT16          ui2_unit;
    FM_XENTRY_T     *pt_new_xent;
} FM_MKENT_ARG_T;

typedef struct _FM_RW_ARG_T
{
    FM_XENTRY_T     *pt_xent;
    VOID            *pv_data;
    UINT32          ui4_count;
    UINT64          ui8_offset;
    UINT32          ui4_done_bytes;
    UINT16          ui2_flag;
#define FM_IO_SYNC          ((UINT16) 0x0001)
#define FM_IO_DIRECT        ((UINT16) 0x0002)
#define FM_IO_INVAL         ((UINT16) 0x8000)
#define FM_IO_RW_END    ((UINT16) 0x0800)
    /* for sync */
    HANDLE_T        h_bin_sema;

    /* for async */
    UINT8           ui1_pri;
    x_fm_async_fct  pf_nfy_fct;
    VOID            *pv_tag;
    FM_ASYNC_COND_T e_async_cond;
    HANDLE_T    h_rw_req;
    HANDLE_T    h_file;
    
    UINT8       ui1_all_req_sent;
    UINT32      ui4_req_cnt;
    UINT32      ui4_req_ok_cnt;
    UINT32      ui4_req_fail_cnt;
    UINT8       ui1_aborted;

} FM_RW_ARG_T;

typedef struct _FM_LSEEK_ARG_T
{
    FM_XENTRY_T     *pt_xent;
    INT64           i8_offset;
    UINT8           ui1_whence;
    UINT64          ui8_cur_pos;
} FM_LSEEK_ARG_T;

typedef struct _FM_SEEK_DIR_ARG_T
{
    FM_XENTRY_T *pt_xent;
    INT32               i4_ent_off;
    UINT8               ui1_whence;
    UINT64              ui8_cur_pos;
} FM_SEEK_DIR_ARG_T;

typedef struct _FM_FILE_INFO_ARG_T
{
    FM_XENTRY_T     *pt_prnt_xent;
    FM_XENTRY_T     *pt_xent;
    FM_FILE_INFO_T  *pt_info;
} FM_FILE_INFO_ARG_T;

typedef struct _FM_DIR_INFO_ARG_T
{
    FM_XENTRY_T     *pt_prnt_xent;
    FM_XENTRY_T     *pt_xent;
    FM_DIR_INFO_T   *pt_info;
} FM_DIR_INFO_ARG_T;

#define FM_SET_SIZE_NOVAL ((UINT64)-1)
#define FM_SET_TIME_NOVAL ((UINT32)-1)
#define FM_SET_MODE_NOVAL ((UINT32)-1)

typedef struct _FM_READ_DIR_ARG_T
{
    FM_XENTRY_T     *pt_xent;
    FM_DIR_ENTRY_T  *pt_dir_ent;
    UINT32          ui4_ent_cnt;
    UINT32          ui4_read_cnt;
    UINT64          ui8_offset;
    HANDLE_T        h_sync_sema;
} FM_READ_DIR_ARG_T;

typedef struct _FM_DELETE_ARG_T
{
    FM_XENTRY_T     *pt_prnt_xent;
    FM_XENTRY_T     *pt_xent;
} FM_DELETE_ARG_T;

typedef struct _FM_RENAME_ARG_T
{
    FM_XENTRY_T     *pt_src_prnt_xent;
    FM_XENTRY_T     *pt_xent;
    FM_XENTRY_T     *pt_dst_prnt_xent;
    const CHAR      *ps_name;
} FM_RENAME_ARG_T;

typedef struct _FM_CREATE_FS_ARG_T
{
    FM_XENTRY_T     *pt_xent;
    HANDLE_T         h_sync_bin;
} FM_CREATE_FS_ARG_T;

typedef struct _FM_MOUNT_FS_ARG_T
{
    FM_MOUNT_T      *pt_mount;
    FM_XENTRY_T     *pt_dev_xent;
    VOID            *pv_data;
} FM_MOUNT_FS_ARG_T;

typedef struct _FM_UMOUNT_FS_ARG_T
{
    FM_MOUNT_T      *pt_mount;
    VOID            *pv_data;
} FM_UMOUNT_FS_ARG_T;

/*
 *  File descriptor structure
 */
typedef struct _FM_MTX_OBJ_T   FM_MTX_OBJ_T;
typedef struct _FM_FILE_DESC_T FM_FILE_DESC_T;

typedef INT32 (*fm_file_op_fct)(
    VOID            *pv_data);

typedef INT32 (*fm_file_rw_fct)(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_done);

typedef INT32 (*fm_file_arw_fct)(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req,
    HANDLE_T    h_file);

typedef INT32 (*fm_file_lock_fct)(
    FM_FILE_DESC_T  *pt_desc,
    UINT32          ui4_lock_op);

typedef struct _FM_FILE_OP_T
{
    fm_file_rw_fct   pf_read;
    fm_file_rw_fct   pf_write;
    fm_file_arw_fct  pf_async_read;
    fm_file_arw_fct  pf_async_write;
    fm_file_op_fct   pf_close;
    fm_file_lock_fct pf_flock;
} FM_FILE_OP_T;

typedef enum
{
    FM_TYPE_XENTRY = 1,
    FM_TYPE_DIRLBL,
    FM_TYPE_SOCKET          /* reserved for further extension */
} FM_ENTRY_TYPE_T;

typedef VOID (*fm_obj_free_fct)(VOID *pv_obj);

struct _FM_MTX_OBJ_T
{
    HANDLE_T        h_mtx;
    HANDLE_T        h_ref_handle;
    UINT16          ui2_lock_cnt;
    UINT16          ui2_want_cnt;
    UINT32          ui4_status;
    fm_obj_free_fct pf_free;
    FM_ENTRY_TYPE_T e_type;
    DLIST_ENTRY_T(_FM_MTX_OBJ_T) t_obj_link;
};

#define FM_OBJ_CLOSING  ((UINT32) 0x00000001)
#define FM_OBJ_LOCKED   ((UINT32) 0x00000002)

#define IS_OBJ_CLOSING(obj) \
        (((obj)->ui4_status & FM_OBJ_CLOSING) != 0)
        
#define IS_OBJ_LOCKED(obj) \
        (((obj)->ui4_status & FM_OBJ_LOCKED) != 0)

/*
 *  File descriptor and directory label
 */
struct _FM_FILE_DESC_T
{
    HANDLE_T        h_mtx;
    HANDLE_T        h_ref_handle;
    UINT16          ui2_lock_cnt;
    UINT16          ui2_want_cnt;
    UINT32          ui4_status;
    fm_obj_free_fct pf_free;
    FM_ENTRY_TYPE_T e_type;
    DLIST_ENTRY_T(_FM_MTX_OBJ_T) t_obj_link;

    /* Cannot modify above entries. It must be the same as FM_MTX_OBJ_T */
    
    FM_XENTRY_T     *pt_xent;
    FM_FILE_OP_T    *pt_file_ops;
    UINT32          ui4_flags;
    UINT64          ui8_offset;
    HANDLE_T        h_sync_sema; /* for synchronous I/O */
    UINT32          ui4_flock_cnt;
};

#define FILE_OP_READ(pf) \
    (*((pf)->pt_file_ops->pf_read))
#define FILE_OP_WRITE(pf) \
    (*((pf)->pt_file_ops->pf_write))
#define FILE_OP_READ_ASYNC(pf) \
    (*((pf)->pt_file_ops->pf_async_read))
#define FILE_OP_WRITE_ASYNC(pf) \
    (*((pf)->pt_file_ops->pf_async_write))
#define FILE_OP_CLOSE(pf) \
    (*((pf)->pt_file_ops->pf_close))
#define FILE_OP_FLOCK(pf) \
    (*((pf)->pt_file_ops->pf_flock))

/*
 *  Directory label
 */
typedef struct _FM_DIR_LABEL_T
{
    HANDLE_T        h_mtx;
    HANDLE_T        h_ref_handle;
    UINT16          ui2_lock_cnt;
    UINT16          ui2_want_cnt;
    UINT32          ui4_status;
    fm_obj_free_fct pf_free;
    FM_ENTRY_TYPE_T e_type;
    DLIST_ENTRY_T(_FM_MTX_OBJ_T) t_obj_link;

    /* Cannot modify above entries. It must be the same as FM_MTX_OBJ_T */

    FM_XENTRY_T     *pt_xent;
    CHAR            *ps_path; /* includes endding '/' */
    UINT16          ui2_len;  /* strlen(ps_path) */
} FM_DIR_LABEL_T;

/*
 *  File system init function prototype.
 *  FS_INIT_FCT_DECLARE is used for individual file system to declare its
 *  init function. FS_INIT_FCT_REF is used by UFS (fm.c) to reference to a
 *  file system's init function.
 */
typedef FM_FS_DESC_T *(FS_INIT_FCT_T)(VOID *pv_data);

#define FS_INIT_FCT_DECLARE(fs) \
    FM_FS_DESC_T *fs##_init(VOID *pv_data)

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
                                                                               
extern FM_FILE_OP_T ufs_file_ops;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 fm_get_xentry_full_path(
    FM_XENTRY_T *pt_xent,
    CHAR *ps_path,
    UINT32 ui4_size);

extern INT32 fm_get_logical_partitions(
    FM_PART_TBL_ENT_T *pt_prm_ent,
    FM_XENTRY_T * pt_xent, 
    UINT64 ui8_offset, 
    UINT32 * pui4_cnt);

extern INT32 fm_reg_fs(
    FM_FS_DESC_T    *pt_fs_desc);

extern INT32 fm_xentry_lock(
    FM_XENTRY_T     *pt_xent,
    SEMA_OPTION_T   e_opt);

extern INT32 fm_xentry_unlock(
    FM_XENTRY_T     *pt_xent);

extern VOID fm_xentry_ref(
    FM_XENTRY_T     *pt_xent);

extern VOID fm_xentry_rel(
    FM_XENTRY_T     *pt_xent);

extern INT32 fm_obj_lock(
    VOID            *pv_obj);

extern INT32 fm_obj_unlock(
    VOID            *pv_obj);

extern VOID fm_obj_reg(
    VOID            *pv_obj,
    FM_MOUNT_T      *pt_mount);

extern VOID fm_obj_unreg(
    VOID            *pv_obj,
    FM_MOUNT_T      *pt_mount);

extern FM_FILE_DESC_T *fm_alloc_file_desc(
    VOID);

extern VOID fm_free_file_desc(
    VOID            *pv_obj);

extern INT32 fm_get_file_desc(
    HANDLE_T        h_file,
    FM_FILE_DESC_T  **ppt_desc);

extern FM_DIR_LABEL_T *fm_alloc_dir_label(
    VOID);

extern VOID fm_free_dir_label(
    VOID            *pv_obj);

extern INT32 fm_get_dir_label(
    HANDLE_T        h_dir,
    FM_DIR_LABEL_T  **ppt_lbl);

extern BOOL fm_handle_free_fct(
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID            *pv_obj,
    VOID            *pv_tag,
    BOOL            b_req_handle);

extern FM_XENTRY_T *fm_get_new_xentry(
    const CHAR      *ps_name);

extern VOID fm_xentry_reclaim(
    FM_XENTRY_T     *pt_xent);

extern INT32 fm_ufs_lookup(
    FM_DIR_LABEL_T  *pt_start_dir,
    const CHAR      *ps_name,
    UINT16          ui2_op,
    UINT16          ui2_flag,
    FM_LOOKUP_ARG_T *pt_arg);

extern INT32 fm_ufs_open(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    HANDLE_T        *ph_file,
    FM_FILE_DESC_T  **ppt_file_desc);

extern INT32 fm_ufs_make_entry(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_MKENT_ARG_T  *pt_mk_ent_arg);

extern INT32 fm_ufs_delete(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    UINT32          ui4_mode);

extern INT32 fm_ufs_rename(
    FM_DIR_LABEL_T  *pt_src_lbl,
    const CHAR      *ps_src_path,
    FM_DIR_LABEL_T  *pt_dst_lbl,
    const CHAR      *ps_dst_path);

extern INT32 fm_ufs_lookup_set_info(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info);

extern INT32 fm_ufs_mount(
    FM_XENTRY_T     *pt_dev_xent,
    FM_XENTRY_T     *pt_mp_xent);

extern INT32 fm_ufs_umount(
    FM_XENTRY_T     *pt_xent,
    BOOL            b_enforce);

extern INT32 fm_ufs_create_fs(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data);

extern BOOL fm_ufs_is_file_locked(
    FM_FILE_DESC_T  *pt_desc,
    UINT32          ui4_lock_op);

extern INT32 fm_init(VOID);

#endif /* _FM_UFS_H */
