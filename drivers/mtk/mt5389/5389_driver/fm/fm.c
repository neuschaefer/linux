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
 * $RCSfile: fm.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Yuchien Chen | Riccardo Sun $
 * $MD5HEX: a6fbdec1a3e068835c045ab0cf718927 $
 *
 * Description: 
 *         This file contains File Manager internal functions.
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_end_cnvt.h"
#include "handle/handle.h"
#include "x_os.h"
#include "x_rwlock.h"
#include "fm_util.h"
#include "fm_ufs.h"
#include "fm_rootfs.h"
#include "fm_blkdev.h"
#include "fm_buf.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef DEBUG_FM_UFS

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_UFS */


#define XENTRY_HASH_SIZE (64)   /* must be power of 2 */
#define XENTRY_MAX_NUM   (64)

typedef struct _FM_XENTRY_HASH_T
{
    SLIST_T(_FM_XENTRY_T) *pt_hash_tbl;
} FM_XENTRY_HASH_T;
 
#define FM_ROOT_FS_NAME "rootfs"

#ifdef DEBUG_FM_UFS
VOID _fm_dump_xentry(FM_XENTRY_T *pt_xent)
{
    if ((pt_xent) != NULL)
    {
        DBG_INFO(("[FM] \"%s\", mode=%o, ref=%d, flag=0x%x, lock=%d, want=%d\n",
                   (char *)(pt_xent)->ps_name,
                   (int)(pt_xent)->ui4_mode,
                   (int)(pt_xent)->ui4_ref_cnt,
                   (int)(pt_xent)->ui4_flags,
                   (int)(pt_xent)->ui2_lock_cnt,
                   (int)(pt_xent)->ui2_want_cnt));
    }
}

VOID _fm_dump_hash(FM_XENTRY_HASH_T *pt_hash)
{
    FM_XENTRY_T *pt_xent;
    int i;
    
    for (i = 0; i < XENTRY_HASH_SIZE; i++)
    {
        DBG_INFO(("[FM] [ %d ]\n", i));
        SLIST_FOR_EACH(pt_xent, &(pt_hash->pt_hash_tbl[i]), t_hlink)
        {
            _fm_dump_xentry(pt_xent);
        }
        DBG_INFO(("[FM] ==========\n"));
    }
}
#else
#define _fm_dump_xentry(a)
#define _fm_dump_hash(a)
#endif /* DEBUG_FM_UFS */


#define FM_GET_PARTITION_1(part, mbr)       \
    (part)->ui1_drv  = mbr[446];            \
    (part)->ui1_start_chs[0] = mbr[447];    \
    (part)->ui1_start_chs[1] = mbr[448];    \
    (part)->ui1_start_chs[2] = mbr[449];    \
    (part)->ui1_type = mbr[450];            \
    (part)->ui1_end_chs[0] = mbr[451];      \
    (part)->ui1_end_chs[1] = mbr[452];      \
    (part)->ui1_end_chs[2] = mbr[453];      \
    x_end_cnvt_from_little_32(&mbr[454], 1, &(part)->ui4_start_lba); \
    x_end_cnvt_from_little_32(&mbr[458], 1, &(part)->ui4_sectors);
    
#define FM_GET_PARTITION_2(part, mbr)       \
    (part)->ui1_drv  = mbr[446+16];            \
    (part)->ui1_start_chs[0] = mbr[447+16];    \
    (part)->ui1_start_chs[1] = mbr[448+16];    \
    (part)->ui1_start_chs[2] = mbr[449+16];    \
    (part)->ui1_type = mbr[450+16];            \
    (part)->ui1_end_chs[0] = mbr[451+16];      \
    (part)->ui1_end_chs[1] = mbr[452+16];      \
    (part)->ui1_end_chs[2] = mbr[453+16];      \
    x_end_cnvt_from_little_32(&mbr[454+16], 1, &(part)->ui4_start_lba); \
    x_end_cnvt_from_little_32(&mbr[458+16], 1, &(part)->ui4_sectors);
    
#define FM_GET_PARTITION_3(part, mbr)       \
    (part)->ui1_drv  = mbr[446+32];            \
    (part)->ui1_start_chs[0] = mbr[447+32];    \
    (part)->ui1_start_chs[1] = mbr[448+32];    \
    (part)->ui1_start_chs[2] = mbr[449+32];    \
    (part)->ui1_type = mbr[450+32];            \
    (part)->ui1_end_chs[0] = mbr[451+32];      \
    (part)->ui1_end_chs[1] = mbr[452+32];      \
    (part)->ui1_end_chs[2] = mbr[453+32];      \
    x_end_cnvt_from_little_32(&mbr[454+32], 1, &(part)->ui4_start_lba); \
    x_end_cnvt_from_little_32(&mbr[458+32], 1, &(part)->ui4_sectors);
    
#define FM_GET_PARTITION_4(part, mbr)       \
    (part)->ui1_drv  = mbr[446+48];            \
    (part)->ui1_start_chs[0] = mbr[447+48];    \
    (part)->ui1_start_chs[1] = mbr[448+48];    \
    (part)->ui1_start_chs[2] = mbr[449+48];    \
    (part)->ui1_type = mbr[450+48];            \
    (part)->ui1_end_chs[0] = mbr[451+48];      \
    (part)->ui1_end_chs[1] = mbr[452+48];      \
    (part)->ui1_end_chs[2] = mbr[453+48];      \
    x_end_cnvt_from_little_32(&mbr[454+48], 1, &(part)->ui4_start_lba); \
    x_end_cnvt_from_little_32(&mbr[458+48], 1, &(part)->ui4_sectors);

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

static UINT32 _fm_xentry_hash_val(
    const CHAR      *ps_name,
    VOID            *pv_data);

static INT32 _fm_xentry_hash_init(
    VOID);

static VOID _fm_xentry_busy(
    FM_XENTRY_T     *pt_xent);

static VOID _fm_xentry_lru(
    FM_XENTRY_T     *pt_xent);

static VOID _fm_xentry_rehash(
    FM_XENTRY_T     *pt_xent);

static INT32 _fm_acquire_xentry(
    const CHAR      *ps_name,
    FM_XENTRY_T     *pt_prnt_xent,
    FM_XENTRY_T     **ppt_xent);

static INT32 _fm_alloc_mount(
    FM_XENTRY_T     *pt_mp_xent,
    FM_MOUNT_T      **ppt_mount);

static INT32 _fm_free_mount(
    FM_MOUNT_T      *pt_mount);

static INT32 _fm_mount_fs(
    FM_MOUNT_T      *pt_mount,
    FM_XENTRY_T     *pt_dev_xent,
    FM_FS_DESC_T    *pt_fs_desc,
    BOOL            b_root);

static INT32 _fm_umount_fs(
    FM_MOUNT_T      *pt_mount);

static INT32 _fm_ufs_read(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_done);

static INT32 _fm_ufs_write(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_done);

static INT32 _fm_ufs_async_read(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req,
    HANDLE_T    h_file);

static INT32 _fm_ufs_async_write(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req,
    HANDLE_T    h_file);

static INT32 _fm_ufs_close(
    VOID            *pv_data);

static INT32 _fm_ufs_flock(
    FM_FILE_DESC_T  *pt_desc,
    UINT32          ui4_lock_op);

static INT32 _fm_inval_mounted_xent(
    FM_MOUNT_T      *pt_mount,
    BOOL            b_enforce);

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/

static SLIST_T(_FM_FS_DESC_T)   t_fs_list;
static SLIST_T(_FM_MOUNT_T)     t_mount_list;
static DLIST_T(_FM_XENTRY_T)    t_free_xentry_q; /* entry removed */
static DLIST_T(_FM_XENTRY_T)    t_lru_xentry_q;  /* entry no more reference */
static DLIST_T(_FM_XENTRY_T)    t_busy_xentry_q; /* entry in use */
static FM_XENTRY_HASH_T         t_xentry_hash;
static BOOL                     b_fm_init_ok      = FALSE;
static FM_FS_DESC_T             *pt_rootfs_desc   = NULL;
static FM_XENTRY_T              *pt_root_xent     = NULL;
static FM_MOUNT_T               *pt_root_mount    = NULL;
static HANDLE_T                 h_name_cache_mtx  = NULL_HANDLE;
static HANDLE_T                 h_opened_obj_mtx  = NULL_HANDLE;
static UINT32                   ui4_xentry_num    = 0;
static UINT16                   ui2_fm_dbg_level  = DBG_INIT_LEVEL;

FM_FILE_OP_T ufs_file_ops =
{
    _fm_ufs_read, 
    _fm_ufs_write,
    _fm_ufs_async_read,
    _fm_ufs_async_write,
    _fm_ufs_close,
    _fm_ufs_flock
};

#define _fm_xentry_cache_lock() \
    x_sema_lock(h_name_cache_mtx, X_SEMA_OPTION_WAIT)
    
#define _fm_xentry_cache_unlock() \
    x_sema_unlock(h_name_cache_mtx)

static VOID _fm_raw_async_nfy_fct(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    FM_RW_ARG_T *pt_arg;

    ASSERT(pv_tag != NULL);

    pt_arg = (FM_RW_ARG_T *)pv_tag;
    pt_arg->ui4_done_bytes = ui4_data;

    pt_arg->e_async_cond = e_async_cond;

    if (pt_arg->pf_nfy_fct)
    {
        if (pt_arg->ui1_aborted == TRUE)
        {
            pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                               pt_arg->pv_tag, 
                               FM_ASYNC_COND_ABORT_FAIL, 
                               pt_arg->ui4_done_bytes);
        }
        else
        {
            pt_arg->pf_nfy_fct(pt_arg->h_rw_req, 
                               pt_arg->pv_tag, 
                               e_async_cond, 
                               pt_arg->ui4_done_bytes);
        }
    }

    x_handle_free(h_req);
    x_mem_free(pt_arg);
}

/*-----------------------------------------------------------------------------
 * Name: _fm_name_hash_val
 *
 * Description: Calculate the hash value of a file name. The hash value is
 *              normalized to XENTRY_HASH_SIZE.
 *
 * Inputs:  ps_name
 *              String name.
 *          pv_data
 *              The reference to parent directory related stuff.
 *
 * Outputs: -
 *
 * Returns: Hash value, between 0 and (XENTRY_HASH_SIZE - 1)
 ----------------------------------------------------------------------------*/
static UINT32 _fm_xentry_hash_val(
    const CHAR      *ps_name,
    VOID            *pv_data)
{
    const CHAR *pc;
    UINT32 ui4_val = 0;

    ASSERT(ps_name != NULL);

    for (pc = ps_name; *pc != '\0'; pc++)
    {
        ui4_val *= 7;
        ui4_val += ((UINT32)(*pc) * 13) + 1;
    }

    ui4_val += ((UINT32)pv_data >> 8);
    ui4_val &= (XENTRY_HASH_SIZE - 1);

    return ui4_val;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_xentry_hash_init
 *
 * Description: Initialize the xentry hash.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_xentry_hash_init(VOID)
{
    UINT32 i;

    t_xentry_hash.pt_hash_tbl =
        x_mem_alloc(sizeof(*t_xentry_hash.pt_hash_tbl) * XENTRY_HASH_SIZE);

    if (t_xentry_hash.pt_hash_tbl == NULL)
    {
        return FMR_CORE;
    }

    for (i = 0; i < XENTRY_HASH_SIZE; i++)
    {
        SLIST_INIT(&(t_xentry_hash.pt_hash_tbl[i]));
    }
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_xentry_busy
 *
 * Description: Move the xentry to busy queue and put it in the hash chain if
 *              it is not cached yet.
 *
 * Inputs:  pt_xent
 *              The xentry to be moved.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_xentry_busy(
    FM_XENTRY_T     *pt_xent)
{
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(!IS_XENTRY_BUSY(pt_xent));

    _fm_xentry_cache_lock();

    if (!IS_XENTRY_USED(pt_xent))
    {
        UINT32 ui4_val;
        
        ui4_val = _fm_xentry_hash_val(pt_xent->ps_name, pt_xent->pt_prnt_xent);
        SLIST_INSERT_HEAD(pt_xent,
                          &(t_xentry_hash.pt_hash_tbl[ui4_val]),
                          t_hlink);
    }
    else
    {
        DLIST_REMOVE(pt_xent, &t_lru_xentry_q, t_qlink);
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_LRU;
    }
    pt_xent->ui4_flags |= FM_XENTRY_FLAG_BUSY;
    DLIST_INSERT_TAIL(pt_xent, &t_busy_xentry_q, t_qlink);

    _fm_xentry_cache_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: _fm_xentry_lru
 *
 * Description: Move the xentry to the lru queue and put it in the hash chain.
 *              if it is not cached yet.
 *
 * Inputs:  pt_xent
 *              The xentry to be moved.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_xentry_lru(
    FM_XENTRY_T     *pt_xent)
{
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(!IS_XENTRY_LRU(pt_xent));

    _fm_xentry_cache_lock();

    if (!IS_XENTRY_USED(pt_xent))
    {
        UINT32 ui4_val;

        ui4_val = _fm_xentry_hash_val(pt_xent->ps_name, pt_xent->pt_prnt_xent);
        SLIST_INSERT_HEAD(pt_xent,
                          &(t_xentry_hash.pt_hash_tbl[ui4_val]),
                          t_hlink);
    }
    else
    {
        DLIST_REMOVE(pt_xent, &t_busy_xentry_q, t_qlink);
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_BUSY;
    }
    pt_xent->ui4_flags |= FM_XENTRY_FLAG_LRU;
    DLIST_INSERT_TAIL(pt_xent, &t_lru_xentry_q, t_qlink);

    _fm_xentry_cache_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: _fm_xentry_rehash
 *
 * Description: Rehash the specified xentry. This functions is used by rename.
 *              Rename may change the name or its parent directory.
 *
 * Inputs:  pt_xent
 *              Xentry to be rehashed.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

static VOID _fm_xentry_rehash(
    FM_XENTRY_T     *pt_xent)
{
    UINT32 ui4_val;
    
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(IS_XENTRY_USED(pt_xent));
    
    ui4_val = _fm_xentry_hash_val(pt_xent->ps_name, pt_xent->pt_prnt_xent);

    _fm_xentry_cache_lock();

    SLIST_REMOVE(pt_xent, t_hlink);
    SLIST_INSERT_HEAD(pt_xent,
                      &(t_xentry_hash.pt_hash_tbl[ui4_val]),
                      t_hlink);
    
    _fm_xentry_cache_unlock();
}

/*-----------------------------------------------------------------------------
 * Name: _fm_acquire_xentry
 *
 * Description: Find an xentry and lock it in the cache by name and parent
 *              xentry.
 *
 * Inputs:  ps_name
 *              Path component name.
 *          pt_prnt_xent
 *              Xentry of the parent directory.
 *
 * Outputs: ppt_xent
 *              The reference to the xentry if it was found.
 *
 * Returns: FMR_OK
 *              Success and found.
 *          FMR_NO_ENTRY
 *              No entry named ps_name is found.
 *          FMR_INVAL
 *              The desired entry is being renamed or deleted.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_acquire_xentry(
    const CHAR      *ps_name,
    FM_XENTRY_T     *pt_prnt_xent,
    FM_XENTRY_T     **ppt_xent)
{
    FM_XENTRY_T     *pt_xent = NULL;
    BOOL            b_found  = FALSE;
    UINT32          ui4_val;
    INT32           i4_ret;

    ASSERT(ps_name != NULL);
    ASSERT(pt_prnt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_prnt_xent));
    
    ui4_val = _fm_xentry_hash_val(ps_name, pt_prnt_xent);

    while (1)
    {
        i4_ret = FMR_NO_ENTRY;

        _fm_xentry_cache_lock();

        SLIST_FOR_EACH(pt_xent,
                       &(t_xentry_hash.pt_hash_tbl[ui4_val]),
                       t_hlink)
        {
            if ((x_strcmp(pt_xent->ps_name, ps_name) == 0) &&
                (pt_xent->pt_prnt_xent == pt_prnt_xent))
            {
                /*
                 *  To avoid deadlock, we use nowait lock here.
                 */
                i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_NOWAIT);
            
                if (i4_ret == FMR_BUSY)
                {
                    _fm_xentry_cache_unlock();
                    x_thread_delay(1);
                }
                else
                {
                    b_found = TRUE;
                }
                break; /* break SLIST_FOR_EACH */
            }
        }
        if ((pt_xent == NULL) || (b_found == TRUE))
        {
            break; /* break while(1) */
        }
    }

    *ppt_xent = pt_xent;

    _fm_xentry_cache_unlock();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_alloc_mount
 *
 * Description: Allocate a mount object.
 *
 * Inputs:  pt_mp_xent
 *              Mount point xentry.
 *
 * Outputs: ppt_mount
 *              The reference to the address of allocated mount object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OS error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_alloc_mount(
    FM_XENTRY_T     *pt_mp_xent,
    FM_MOUNT_T      **ppt_mount)
{
    FM_MOUNT_T *pt_mount;

    ASSERT(ppt_mount != NULL);

    pt_mount = x_mem_calloc(1, sizeof(FM_MOUNT_T));
    if (pt_mount == NULL)
    {
        return FMR_CORE;
    }

    if (x_sema_create(&pt_mount->h_sync_sema,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_mount);
        return FMR_CORE;
    }
    if (x_sema_create(&pt_mount->h_lock_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_sema_delete(pt_mount->h_sync_sema);
        x_mem_free(pt_mount);
        return FMR_CORE;
    }

    DLIST_INIT(&pt_mount->t_opened_obj_q);
    pt_mount->pt_mp_xent = pt_mp_xent; 
    *ppt_mount = pt_mount;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_free_mount
 *
 * Description: Free a mount object.
 *
 * Inputs:  pt_mount
 *              The reference to the address of allocated mount object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
static INT32 _fm_free_mount(
    FM_MOUNT_T      *pt_mount)
{
    ASSERT(pt_mount != NULL);

    x_sema_delete(pt_mount->h_lock_mtx);
    x_sema_delete(pt_mount->h_sync_sema);
    x_mem_free(pt_mount);

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_mount_fs
 *
 * Description: Moumnt a file system on the device.
 *
 * Inputs:  pt_mount
 *              The reference to the mount object.
 *          pt_dev_xent
 *              Xentry for the device.
 *          pt_fs_desc
 *              File system descriptor.
 *          b_root
 *              Indicate if this is mount for root.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_BUSY
 *              Mount point has already been mounted.
 *          FMR_NOT_DIR
 *              Mount point is not a directory.
 *          FMR_CORE
 *              OS error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_mount_fs(
    FM_MOUNT_T      *pt_mount,
    FM_XENTRY_T     *pt_dev_xent,
    FM_FS_DESC_T    *pt_fs_desc,
    BOOL            b_root)
{
    FM_MOUNT_FS_ARG_T t_mount_arg;
    FM_XENTRY_T *pt_xent = NULL;
    INT32 i4_ret;

    ASSERT(pt_mount             != NULL);
    ASSERT(pt_fs_desc           != NULL);
    ASSERT(pt_mount->pt_mp_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_mount->pt_mp_xent));

    if (IS_XENTRY_MOUNTED(pt_mount->pt_mp_xent))
    {
        DBG_ERROR(("[FM] ERR: Already mounted\n"));
        return FMR_BUSY;
    }
    if (!IS_XENTRY_DIR(pt_mount->pt_mp_xent))
    {
        DBG_ERROR(("[FM] ERR: Not dir\n"));
        return FMR_NOT_DIR;
    }

    if (b_root == TRUE)
    {
        /* root mount */
        /* It is the special case that pt_mp_xent == pt_root_xent */
        pt_mount->pt_root_xent = pt_mount->pt_mp_xent;
    }
    else
    {
        ASSERT(pt_dev_xent != NULL);
        
        /* allocate a new root xentry relative to this mount */
        pt_xent = fm_get_new_xentry("/");
        if (pt_xent == NULL)
        {
            return FMR_CORE;
        }
        if (fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT) != FMR_OK)
        {
            pt_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
            fm_xentry_reclaim(pt_xent);
            return FMR_CORE;
        }

        pt_xent->ui4_mode |= (FM_MODE_TYPE_DIR | 0755);
        pt_mount->pt_dev_xent  = pt_dev_xent;
        pt_mount->pt_root_xent = pt_xent;
        pt_mount->pt_root_xent->pt_dev = pt_dev_xent->pt_dev;
    }
    
    pt_mount->pt_fs_desc    = pt_fs_desc;

    t_mount_arg.pt_mount    = pt_mount;
    t_mount_arg.pt_dev_xent = pt_dev_xent;
    t_mount_arg.pv_data     = NULL;

    i4_ret = (*pt_fs_desc->pt_ufs_op->pf_mount)(&t_mount_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    pt_mount->pt_mp_xent->pt_mounted   = pt_mount;
    pt_mount->pt_mp_xent->ui4_flags   |= FM_XENTRY_FLAG_MOUNTED;
    pt_mount->pt_root_xent->pt_mount   = pt_mount;
    pt_mount->pt_root_xent->ui4_flags |= FM_XENTRY_FLAG_MOUNT_ROOT;
    
    fm_xentry_ref(pt_mount->pt_mp_xent);
    fm_xentry_ref(pt_mount->pt_root_xent);

    if (b_root == TRUE)
    {
        pt_mount->ui4_flags |= FM_MNT_FLAG_ROOTFS; /* cannot be unmounted */
    }

EXIT:
    
    if (pt_xent != NULL)
    {
        if (i4_ret != FMR_OK)
        {
            pt_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
            fm_xentry_reclaim(pt_xent);
        }
        fm_xentry_unlock(pt_xent);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_umount_fs
 *
 * Description: Unmoumnt a file system.
 *
 * Inputs:  pt_mount
 *              The reference to the mount object. Release root xentry and
 *              mount point xentry.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_BUSY
 *              Mount point has already been mounted.
 *          FMR_NOT_DIR
 *              Mount point is not a directory.
 *          FMR_CORE
 *              OS error.
 ----------------------------------------------------------------------------*/

static INT32 _fm_umount_fs(
    FM_MOUNT_T      *pt_mount)
{
    FM_UMOUNT_FS_ARG_T t_umount_arg;
    FM_FS_DESC_T    *pt_fs_desc;
    FM_XENTRY_T     *pt_mp_xent = NULL;
    FM_XENTRY_T     *pt_root_xent = NULL;
    FM_MTX_OBJ_T    *pt_mtx_obj;
    INT32           i4_ret;

    if (pt_mount == NULL)
    {
        return FMR_CORE;
    }
        
    if ((pt_mount->pt_fs_desc == NULL) ||
        (pt_mount->pt_mp_xent == NULL) ||
        (pt_mount->pt_root_xent == NULL))
    {
        return FMR_CORE;
    }

    if ((!IS_XENTRY_MOUNTED(pt_mount->pt_mp_xent)) ||
        (!IS_XENTRY_MOUNT_ROOT(pt_mount->pt_root_xent)))
    {
        return FMR_CORE;
    }

    pt_fs_desc = pt_mount->pt_fs_desc;

    if (fm_xentry_lock(pt_mount->pt_mp_xent, X_SEMA_OPTION_WAIT) != FMR_OK)
    {
        return FMR_CORE;
    }
    pt_mp_xent = pt_mount->pt_mp_xent;
    
    if (fm_xentry_lock(pt_mount->pt_root_xent, X_SEMA_OPTION_WAIT) != FMR_OK)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }
    pt_root_xent = pt_mount->pt_root_xent;

    /*
     *  Call individual file system's umount function.
     */
    t_umount_arg.pt_mount = pt_mount;
    t_umount_arg.pv_data  = NULL;

    i4_ret = (*pt_fs_desc->pt_ufs_op->pf_umount)(&t_umount_arg);
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }

    /*
     *  Release all opened files and directory label in this mount.
     */
    x_sema_lock(pt_mount->h_lock_mtx, X_SEMA_OPTION_WAIT);

    while ((pt_mtx_obj = DLIST_HEAD(&pt_mount->t_opened_obj_q)) != NULL)
    {
        FM_XENTRY_T  *pt_del_xent = NULL;

        ASSERT((pt_mtx_obj->e_type == FM_TYPE_XENTRY) ||
               (pt_mtx_obj->e_type == FM_TYPE_DIRLBL));

        i4_ret = fm_obj_lock(pt_mtx_obj);
        ASSERT(i4_ret == FMR_OK);

        DLIST_REMOVE(pt_mtx_obj, &pt_mount->t_opened_obj_q, t_obj_link);

        if (pt_mtx_obj->e_type == FM_TYPE_XENTRY)
        {
            pt_del_xent = ((FM_FILE_DESC_T *)pt_mtx_obj)->pt_xent;
        }
        else if (pt_mtx_obj->e_type == FM_TYPE_DIRLBL)
        {
            pt_del_xent = ((FM_DIR_LABEL_T *)pt_mtx_obj)->pt_xent;
        }
        ASSERT(pt_del_xent != NULL);
        
        if (pt_del_xent != NULL)
        {
            if (!IS_XENTRY_MOUNT_ROOT(pt_del_xent))
            {
                ASSERT(IS_XENTRY_INVAL(pt_del_xent));
                
                fm_xentry_rel(pt_del_xent);
                if (pt_del_xent->ui4_ref_cnt == 0)
                {
                    pt_del_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
                    fm_xentry_reclaim(pt_del_xent);
    /* modified by riccardo { */
                    fm_xentry_unlock(pt_del_xent);

                }
    /*            
                fm_xentry_unlock(pt_del_xent);
    */
    /* } */
            }
            else
            {
                ASSERT(pt_del_xent == pt_root_xent);
                fm_xentry_rel(pt_del_xent);
            }
        }

        x_handle_free(pt_mtx_obj->h_ref_handle);
    }

    x_sema_unlock(pt_mount->h_lock_mtx);

    /*
     *  Release mount root and mount point xentries.
     */
    pt_mp_xent->ui4_flags &= ~FM_XENTRY_FLAG_MOUNTED;
    fm_xentry_rel(pt_mp_xent);
    fm_xentry_rel(pt_root_xent);
    ASSERT(pt_root_xent->ui4_ref_cnt == 0);
    pt_root_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
    fm_xentry_reclaim(pt_root_xent);

FAIL:

    if (pt_root_xent != NULL)
    {
        fm_xentry_unlock(pt_root_xent);
    }
    if (pt_mp_xent != NULL)
    {
        fm_xentry_unlock(pt_mp_xent);
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_read
 *
 * Description: Read data of an xentry.
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          ui4_count
 *              Bytes to read.
 *
 * Outputs: pv_data
 *              Buffer to hold the read data.
 *          pui4_done
 *              Actual read bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_INVAL
 *              Invalid file descriptor or xentry.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_IO_FAIL
 *              Device driver I/O error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_ufs_read(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_done)
{
    FM_RW_ARG_T t_io;
    INT32       i4_ret;

    DEBUG_TRACE();

    ASSERT(pt_desc   != NULL);
    ASSERT(pv_data   != NULL);
    ASSERT(pui4_done != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(IS_XENTRY_BUSY(pt_desc->pt_xent));
    ASSERT(pt_desc->pt_xent->pt_xent_ops != NULL);
    ASSERT(pt_desc->pt_xent->pt_xent_ops->pf_read != NULL);

    *pui4_done = 0;

    if ((pt_desc->ui4_flags & FM_ACCESS_MODE) == FM_WRITE_ONLY)
    {
        return FMR_PERM_DENY;
    }
    
    t_io.pt_xent        = pt_desc->pt_xent;
    t_io.pv_data        = pv_data;
    t_io.ui4_count      = ui4_count;
    t_io.ui8_offset     = pt_desc->ui8_offset;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC;
    t_io.h_bin_sema     = pt_desc->h_sync_sema;
    if (pt_desc->ui4_flags & FM_NO_CACHE)
    {
        t_io.ui2_flag |= FM_IO_DIRECT;
    }

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    
    i4_ret = XENTRY_OP_READ(pt_desc->pt_xent)(&t_io);

    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset += t_io.ui4_done_bytes;
        *pui4_done = t_io.ui4_done_bytes;
    }
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_write
 *
 * Description: Write data to an xentry.
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          pv_data
 *              Data to be written.
 *          ui4_count
 *              Bytes to read.
 *
 * Outputs: 
 *          pui4_done
 *              Actual read bytes.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_INVAL
 *              Invalid file descriptor or xentry.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_IO_FAIL
 *              Device driver I/O error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_ufs_write(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT32          *pui4_done)
{
    FM_RW_ARG_T t_io;
    INT32       i4_ret;

    DEBUG_TRACE();

    ASSERT(pt_desc   != NULL);
    ASSERT(pv_data   != NULL);
    ASSERT(pui4_done != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(IS_XENTRY_BUSY(pt_desc->pt_xent));
    ASSERT(pt_desc->pt_xent->pt_xent_ops != NULL);
    ASSERT(pt_desc->pt_xent->pt_xent_ops->pf_write != NULL);
    
    if ((pt_desc->ui4_flags & FM_ACCESS_MODE) == FM_READ_ONLY)
    {
        return FMR_PERM_DENY;
    }

    if (pt_desc->ui4_flags & FM_OPEN_APPEND)
    {
        FM_FILE_INFO_ARG_T t_get_arg;
        FM_FILE_INFO_T     t_file_info;

        /* 
         *  Set pf_file_desc->ui8_offset to file size
         */
        t_get_arg.pt_xent = pt_desc->pt_xent;
        t_get_arg.pt_info = &t_file_info;

        i4_ret = XENTRY_OP_GET_INFO(pt_desc->pt_xent)(&t_get_arg);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }

        pt_desc->ui8_offset = t_file_info.ui8_size;
    }
    
    t_io.pt_xent        = pt_desc->pt_xent;
    t_io.pv_data        = pv_data;
    t_io.ui4_count      = ui4_count;
    t_io.ui8_offset     = pt_desc->ui8_offset;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC;
    t_io.h_bin_sema     = pt_desc->h_sync_sema;
    if (pt_desc->ui4_flags & FM_NO_CACHE)
    {
        t_io.ui2_flag |= FM_IO_DIRECT;
    }

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    
    i4_ret = XENTRY_OP_WRITE(pt_desc->pt_xent)(&t_io);
/*
    if (i4_ret == FMR_OK)
    {
        FM_FILE_INFO_ARG_T t_file_info_arg = {0};
        FM_FILE_INFO_T t_file_info = {0};

        UINT64 ui8_time;

        RTC_GetTimeDate(&ui8_time);
    
        t_file_info.ui4_mode = FM_SET_MODE_NOVAL;
        t_file_info.ui8_size = FM_SET_SIZE_NOVAL;
        t_file_info.ui8_offset = FM_SET_SIZE_NOVAL;
        t_file_info.ui8_create_time = FM_SET_TIME_NOVAL;
        t_file_info.ui8_access_time = FM_SET_TIME_NOVAL;
        t_file_info.ui8_modify_time = ui8_time;

        t_file_info_arg.pt_xent = pt_desc->pt_xent;
        t_file_info_arg.pt_prnt_xent = pt_desc->pt_xent->pt_prnt_xent;
        t_file_info_arg.pt_info = &t_file_info;

        XENTRY_OP_SET_INFO(pt_desc->pt_xent)(&t_file_info_arg);
    }
*/
    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset += t_io.ui4_done_bytes;
        *pui4_done = t_io.ui4_done_bytes;
    }

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_rw_handle_free_nfy
 *
 *
----------------------------------------------------------------------------*/
BOOL _fm_rw_handle_free_nfy (HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 VOID*          pv_obj,
                                 VOID*          pv_tag,
                                 BOOL           b_req_handle)
{
    return TRUE;
}
/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_async_read
 *
 * Description: Asynchronously read data from an xentry.
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          ui4_count
 *              Number of bytes to read.
 *          ui1_pri
 *              Priority.
 *          pf_nfy_fct
 *              Callback after read is done.
 *          pv_tag
 *              Tag value, that will be returned in the callback.
 *
 * Outputs: pv_data
 *              Buffer to hold the read data. Note that the space cannot be
 *              freed before File Manager invokes pf_nfy_fct.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_INVAL
 *              Invalid file descriptor or xentry, device driver not opened or
 *              has been closed, invalid size or offset, etc.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device driver error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_ufs_async_read(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req,
    HANDLE_T    h_file)
{
    FM_RW_ARG_T t_aio;
    INT32       i4_ret;

    DEBUG_TRACE();

    ASSERT(pt_desc    != NULL);
    ASSERT(pv_data    != NULL);
    ASSERT(pf_nfy_fct != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(IS_XENTRY_BUSY(pt_desc->pt_xent));
    ASSERT(pt_desc->pt_xent->pt_xent_ops != NULL);
    ASSERT(pt_desc->pt_xent->pt_xent_ops->pf_read_async != NULL);
    
    if ((pt_desc->ui4_flags & FM_ACCESS_MODE) == FM_WRITE_ONLY)
    {
        return FMR_PERM_DENY;
    }
    
    t_aio.pt_xent        = pt_desc->pt_xent;
    t_aio.pv_data        = pv_data;
    t_aio.ui4_count      = ui4_count;
    t_aio.ui8_offset     = pt_desc->ui8_offset;
    t_aio.ui4_done_bytes = 0;
    t_aio.ui2_flag       = 0; /* ASYNC */
    t_aio.ui1_pri        = ui1_pri;
    t_aio.pf_nfy_fct     = pf_nfy_fct;
    t_aio.pv_tag         = pv_tag;
    t_aio.e_async_cond   = FM_ASYNC_COND_READ_OK;
    if ( HR_OK != 
        handle_alloc(FMT_ASYNC_READ, 
                                pt_desc->pt_xent, 
                                NULL, 
                                fm_handle_free_fct, 
                                ph_req))
    {
        return FMR_CORE;
    }
    
    t_aio.h_rw_req = *ph_req;
    t_aio.h_file    = h_file;

    if (pt_desc->ui4_flags & FM_NO_CACHE)
    {
        t_aio.ui2_flag |= FM_IO_DIRECT;
    }

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        handle_free( *ph_req, TRUE);
        return i4_ret;
    }

    if ((IS_XENTRY_TRUE(pt_desc->pt_xent)) || (IS_XENTRY_VIRT(pt_desc->pt_xent)))
    {
        FM_RW_ARG_T *pt_new_arg = NULL;

        pt_new_arg = (FM_RW_ARG_T*) x_mem_alloc(sizeof(FM_RW_ARG_T));

        if (pt_new_arg == NULL)
        {
            return FMR_CORE;
        }

        x_memcpy((VOID *) pt_new_arg, (const VOID *) &t_aio, sizeof(FM_RW_ARG_T));

        t_aio.pv_tag       = pt_new_arg;
        t_aio.pf_nfy_fct   = _fm_raw_async_nfy_fct;
    }

    i4_ret = XENTRY_OP_READ_ASYNC(pt_desc->pt_xent)(&t_aio);

    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset += ui4_count;
    }
    else
    {
        handle_free(*ph_req, TRUE);
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_async_write
 *
 * Description: Asynchronously write data to an xentry.
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          pv_data
 *              Reference to data to write. Note that the space cannot be
 *              freed before File Manager invokes pf_nfy_fct.
 *          ui4_count
 *              Number of bytes to write.
 *          ui1_pri
 *              Priority.
 *          pf_nfy_fct
 *              Callback after write is done.
 *          pv_tag
 *              Tag value, that will be returned in the callback.          
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_INVAL
 *              Invalid file descriptor or xentry, device driver not opened or
 *              has been closed, invalid size or offset, etc.
 *          FMR_EOF
 *              Reaches end of file.
 *          FMR_ALIGNMENT
 *              pv_data alignment error.
 *          FMR_DEVICE_ERROR
 *              Device driver error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_ufs_async_write(
    FM_FILE_DESC_T  *pt_desc,
    VOID            *pv_data,
    UINT32          ui4_count,
    UINT8           ui1_pri,
    x_fm_async_fct  pf_nfy_fct,
    VOID            *pv_tag,
    HANDLE_T    *ph_req,
    HANDLE_T    h_file)
{
    FM_RW_ARG_T t_aio;
    INT32       i4_ret;

    DEBUG_TRACE();

    ASSERT(pt_desc    != NULL);
    ASSERT(pv_data    != NULL);
    ASSERT(pf_nfy_fct != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(IS_XENTRY_BUSY(pt_desc->pt_xent));
    ASSERT(pt_desc->pt_xent->pt_xent_ops != NULL);
    ASSERT(pt_desc->pt_xent->pt_xent_ops->pf_write_async != NULL);
    
    if ((pt_desc->ui4_flags & FM_ACCESS_MODE) == FM_READ_ONLY)
    {
        return FMR_PERM_DENY;
    }

    if (pt_desc->ui4_flags & FM_OPEN_APPEND)
    {
        FM_FILE_INFO_ARG_T t_get_arg;
        FM_FILE_INFO_T     t_file_info;

        /* 
         *  Set pf_file_desc->ui8_offset to file size
         */
        t_get_arg.pt_xent = pt_desc->pt_xent;
        t_get_arg.pt_info = &t_file_info;

        i4_ret = XENTRY_OP_GET_INFO(pt_desc->pt_xent)(&t_get_arg);
        if (i4_ret != FMR_OK)
        {
            return i4_ret;
        }

        pt_desc->ui8_offset = t_file_info.ui8_size;
    }

    t_aio.pt_xent        = pt_desc->pt_xent;
    t_aio.pv_data        = pv_data;
    t_aio.ui4_count      = ui4_count;
    t_aio.ui8_offset     = pt_desc->ui8_offset;
    t_aio.ui4_done_bytes = 0;
    t_aio.ui2_flag       = 0; /* ASYNC */
    t_aio.ui1_pri        = ui1_pri;
    t_aio.pf_nfy_fct     = pf_nfy_fct;
    t_aio.pv_tag         = pv_tag;
    t_aio.e_async_cond   = FM_ASYNC_COND_WRITE_OK;
    if (pt_desc->ui4_flags & FM_NO_CACHE)
    {
        t_aio.ui2_flag |= FM_IO_DIRECT;
    }

    i4_ret = fm_xentry_lock(pt_desc->pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    i4_ret = XENTRY_OP_WRITE_ASYNC(pt_desc->pt_xent)(&t_aio);
/*
    if (i4_ret == FMR_OK)
    {
        FM_FILE_INFO_ARG_T t_file_info_arg = {0};
        FM_FILE_INFO_T t_file_info = {0};
        UINT64 ui8_time;

        RTC_GetTimeDate(&ui8_time);

        t_file_info.ui4_mode = FM_SET_MODE_NOVAL;
        t_file_info.ui8_size = FM_SET_SIZE_NOVAL;
        t_file_info.ui8_offset = FM_SET_SIZE_NOVAL;
        t_file_info.ui8_create_time = FM_SET_TIME_NOVAL;
        t_file_info.ui8_access_time = FM_SET_TIME_NOVAL;
        t_file_info.ui8_modify_time = ui8_time;

        t_file_info_arg.pt_xent = pt_desc->pt_xent;
        t_file_info_arg.pt_prnt_xent = pt_desc->pt_xent->pt_prnt_xent;
        t_file_info_arg.pt_info = &t_file_info;

        XENTRY_OP_SET_INFO(pt_desc->pt_xent)(&t_file_info_arg);
    }
*/
    fm_xentry_unlock(pt_desc->pt_xent);

    if (i4_ret == FMR_OK)
    {
        pt_desc->ui8_offset += ui4_count;
    }
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_close
 *
 * Description: Close an opened file. It calls individual file system's
 *              close function and release the opened xentry.
 *
 * Inputs:  pv_data
 *              The reference to FM_XENTRY_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              pt_xent is an invalid entry, which is either renamed or
 *              removed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _fm_ufs_close(VOID *pv_data)
{
    FM_FILE_DESC_T  *pt_desc;
    FM_XENTRY_T     *pt_xent;
    FM_DELETE_ARG_T t_del_arg;
    INT32           i4_ret;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_desc = (FM_FILE_DESC_T *)pv_data;

    ASSERT(pt_desc->pt_xent != NULL);

    pt_xent = pt_desc->pt_xent;

    i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    i4_ret = XENTRY_OP_CLOSE(pt_xent)(pt_xent);

    if (i4_ret == FMR_OK)
    {
        fm_xentry_rel(pt_xent);
        /* check if it should be deleted */
        if ( pt_xent->ui4_ref_cnt == 0
            && pt_xent->ui4_flags & FM_XENTRY_FLAG_UNLINKED)
        {
            i4_ret = fm_xentry_lock(pt_xent->pt_prnt_xent, X_SEMA_OPTION_WAIT);
            if ( i4_ret != FMR_OK)
            {
                goto EXIT;
            }
            t_del_arg.pt_xent = pt_xent;
            t_del_arg.pt_prnt_xent = pt_xent->pt_prnt_xent;
            i4_ret = XENTRY_OP_DELETE(pt_xent)(&t_del_arg);
            fm_xentry_unlock(pt_xent->pt_prnt_xent);
            if (i4_ret == FMR_OK)
            {
                pt_xent->ui4_flags |= FM_XENTRY_FLAG_DELETE;
                pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_UNLINKED;
                fm_xentry_reclaim(pt_xent);
            }
            
        }

        while (pt_desc->ui4_flock_cnt > 0)
        {
            x_rwl_release_lock(pt_xent->h_rw_lock);
            pt_desc->ui4_flock_cnt--;
        }

        pt_desc->pt_xent = NULL;
    }

    if ((IS_XENTRY_DIR(pt_xent)) && (pt_xent->pt_xent_ops->pf_close_dir != NULL))
    {
        pt_xent->pt_xent_ops->pf_close_dir(pt_desc);
    }

EXIT:
    fm_xentry_unlock(pt_xent);
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_ufs_flock
 *
 * Description: Apply or remove an advisory lock on an opened file.
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          ui4_lock_op
 *              File lock operations.
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
static INT32 _fm_ufs_flock(
    FM_FILE_DESC_T  *pt_desc,
    UINT32          ui4_lock_op)
{
    FM_XENTRY_T  *pt_xent;
    RWL_OPTION_T e_rwl_opt;
    INT32        i4_ret;

    DEBUG_TRACE();

    ASSERT(pt_desc != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(pt_desc->pt_xent->h_rw_lock != NULL_HANDLE);
    ASSERT(ui4_lock_op & (FM_LOCK_READ|FM_LOCK_WRITE|FM_LOCK_RELEASE));

    e_rwl_opt = (ui4_lock_op & FM_LOCK_NO_WAIT) ?
        RWL_OPTION_NOWAIT : RWL_OPTION_WAIT;

    pt_xent = pt_desc->pt_xent;

    if (ui4_lock_op & FM_LOCK_WRITE)
    {
        i4_ret = x_rwl_write_lock(pt_xent->h_rw_lock, e_rwl_opt);
    }
    else if (ui4_lock_op & FM_LOCK_READ)
    {
        i4_ret = x_rwl_read_lock(pt_xent->h_rw_lock, e_rwl_opt);
    }
    else
    {
        ASSERT(ui4_lock_op & FM_LOCK_RELEASE);

        if (pt_desc->ui4_flock_cnt == 0)
        {
            return FMR_OK;
        }
        i4_ret = x_rwl_release_lock(pt_xent->h_rw_lock);
    }

    switch (i4_ret)
    {
        case RWLR_OK:
            if (ui4_lock_op & FM_LOCK_RELEASE)
            {
                ASSERT(pt_desc->ui4_flock_cnt > 0);

                pt_desc->ui4_flock_cnt--;
            }
            else
            {
                pt_desc->ui4_flock_cnt++;
            }

            i4_ret = FMR_OK;
            break;
        case RWLR_FAIL:
            i4_ret = FMR_LOCK_FAIL;
            break;
        case RWLR_INVALID_HANDLE:
            i4_ret = FMR_HANDLE;
            break;
        case RWLR_DELETE_IN_PROGRESS:
            i4_ret = FMR_INVAL;
            break;
        case RWLR_WOULD_BLOCK:
            i4_ret = FMR_WOULD_BLOCK;
            break;
        default:
            ASSERT(0);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_inval_mounted_xent
 *
 * Description: Invalid all xentries assoicated to this mount, except mount
 *              root and mount point.
 *
 * Inputs:  pt_mount
 *              Reference to the mount structure
 *          b_enforce
 *              Enfore to umount.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/

static INT32 _fm_inval_mounted_xent(
    FM_MOUNT_T      *pt_mount,
    BOOL            b_enforce)
{
    FM_XENTRY_T *pt_xent;
    FM_XENTRY_T *pt_next_xent;
    INT32 i4_ret = FMR_OK;
    
    _fm_xentry_cache_lock();

    for (pt_xent = t_lru_xentry_q.pt_head;
         pt_xent != NULL;
         pt_xent = pt_next_xent)
    {
        pt_next_xent = pt_xent->t_qlink.pt_next;

        while ((i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_NOWAIT)) ==
               FMR_BUSY)
        {
            _fm_xentry_cache_unlock();
            x_thread_delay(1);
            _fm_xentry_cache_lock();
        }
        ASSERT(i4_ret != FMR_CORE);
        if (i4_ret == FMR_OK)
        {
            if (pt_xent->pt_mount == pt_mount)
            {
                pt_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
                fm_xentry_reclaim(pt_xent);
            }
            fm_xentry_unlock(pt_xent);
        }
    }

    for (pt_xent = t_busy_xentry_q.pt_head;
         pt_xent != NULL;
         pt_xent = pt_next_xent)
    {
        pt_next_xent = pt_xent->t_qlink.pt_next;

        if (pt_xent->pt_mount != pt_mount)
        {
            continue;
        }

        while ((i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_NOWAIT)) ==
               FMR_BUSY)
        {
            _fm_xentry_cache_unlock();
            x_thread_delay(1);
            _fm_xentry_cache_lock();
        }
        ASSERT(i4_ret != FMR_CORE);
        if (i4_ret == FMR_OK)
        {
            if (pt_xent->pt_mount == pt_mount)
            {
                /*
                 *  It is an opened file in this file system.
                 *  If we're not enforced to perform umount, return FMR_BUSY.
                 *  Otherwise, reclaim this entry and notify application that
                 *  the handle becomes invalid.
                 */
                
                if (b_enforce == FALSE)
                {
                    if (!IS_XENTRY_MOUNT_ROOT(pt_xent))
                    {
                        i4_ret = FMR_BUSY;
                    }
                }
                else
                {
                    if (IS_XENTRY_MOUNTED(pt_xent))
                    {
                        /*
                         *  XXX
                         *  Recursively umount.
                         */
                        ASSERT(0);

                        DBG_ERROR(("[FM] ERR: PANIC! Recursively umount.\n"));
                        i4_ret = FMR_INVAL;
                    }

                    /*
                     *  The mount root will be freed in _fm_umount_fs().
                     */
                    if (!IS_XENTRY_MOUNT_ROOT(pt_xent))
                    {
                        pt_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL;
                        continue; /* no unlock */
                    }
                }
            }

            fm_xentry_unlock(pt_xent);

            if (i4_ret != FMR_OK)
            {
                break; /* Not enforced umount */
            }
        }
    }
    
    _fm_xentry_cache_unlock();

    return i4_ret;
}

INT32 fm_get_xentry_full_path(
    FM_XENTRY_T *pt_xent,
    CHAR *ps_path,
    UINT32 ui4_size)
{
    FM_XENTRY_T *pt_xent_ptr = NULL;
    CHAR ps_buf[32];

    x_memset((VOID *) ps_path, 0, ui4_size);
    x_strcpy(ps_path, (const CHAR *) pt_xent->ps_name);

    for (pt_xent_ptr = pt_xent->pt_prnt_xent; 
         pt_xent_ptr != NULL; 
         pt_xent_ptr = pt_xent_ptr->pt_prnt_xent)
    {
        x_memset((VOID *) ps_buf, 0, 32);

        if (*(pt_xent_ptr->ps_name) == '/')
        {
            x_strcpy(ps_buf, (const CHAR *) pt_xent_ptr->ps_name);
        }
        else
        {
            x_strcpy(ps_buf, (const CHAR *) pt_xent_ptr->ps_name);
            ps_buf[x_strlen((const CHAR*)pt_xent_ptr->ps_name)] = '/';
        }
        
        x_strcat(ps_buf, (const CHAR *) ps_path);
        x_strcpy(ps_path, (const CHAR *) ps_buf);

        ASSERT(x_strlen(ps_path) < ui4_size);
        ASSERT(x_strlen(ps_buf) < 32);
    }

    return FMR_OK;
}
/* Before invoke this function, please lock cache list. */
BOOL    _fm_is_child_busy( FM_XENTRY_T* pt_prnt_xent)
{
    BOOL    b_busy = FALSE;
    FM_XENTRY_T *pt_xent = NULL;

    DLIST_FOR_EACH(pt_xent, &t_busy_xentry_q, t_qlink)
    {
        if (pt_xent->pt_prnt_xent == pt_prnt_xent)
        {
            b_busy = TRUE;
            return b_busy;
        }
    }
        
    DLIST_FOR_EACH(pt_xent, &t_lru_xentry_q, t_qlink)
    {
        if (pt_xent->pt_prnt_xent == pt_prnt_xent)
        {
            b_busy = TRUE;
            return b_busy;
        }
    }
    return b_busy;
}
/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: fm_reg_fs
 *
 * Description: Register a new file system.
 *
 * Inputs:  pt_fs_desc
 *              Reference to file system descriptor.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              File Manager does not init.
 ----------------------------------------------------------------------------*/
INT32 fm_reg_fs(
    FM_FS_DESC_T *pt_fs_desc)
{
    if (b_fm_init_ok == FALSE)
    {
        return FMR_INVAL;
    }

    if (pt_fs_desc != NULL)
    {
        FM_FS_DESC_T *pt_desc;
    
        SLIST_FOR_EACH(pt_desc, &t_fs_list, t_link)
        {
            if (pt_desc->e_fs_type == pt_fs_desc->e_fs_type)
            {
                return FMR_EXIST;
            }
        }
    
        SLIST_INSERT_HEAD(pt_fs_desc, &t_fs_list, t_link);
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_xentry_lock
 *
 * Description: Lock the xentry.
 *
 * Inputs:  pt_xent
 *              The reference to the xentry to be locked.
 *          e_opt
 *              lock option, which can be X_SEMA_OPTION_WAIT or
 *              X_SEMA_OPTION_NOWAIT.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              pt_xent is an invalid entry, which is either renamed or
 *              removed.
 *          FMR_BUSY
 *              It's already locked by another task, and will cause the
 *              calling thread blocked.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_xentry_lock(
    FM_XENTRY_T     *pt_xent,
    SEMA_OPTION_T   e_opt)
{
    CRIT_STATE_T t_crit;
    INT32        i4_ret;

    ASSERT(pt_xent != NULL);

    t_crit = x_crit_start();

    if (IS_XENTRY_INVAL(pt_xent))
    {
        x_crit_end(t_crit);
        return FMR_INVAL;
    }

    pt_xent->ui2_want_cnt++;

    x_crit_end(t_crit);
/*
    _show_xent_sema_info(pt_xent, 1);
*/
    i4_ret = x_sema_lock(pt_xent->h_mtx, e_opt);

    pt_xent->ui2_want_cnt--;

    if (i4_ret == OSR_WOULD_BLOCK)
    {
        return FMR_BUSY;
    }
    if (i4_ret != OSR_OK)
    {
        return FMR_CORE;
    }
    if (IS_XENTRY_INVAL(pt_xent))
    {
        fm_xentry_reclaim(pt_xent);
        x_sema_unlock(pt_xent->h_mtx);

        return FMR_INVAL;
    }

    pt_xent->ui2_lock_cnt++;
    pt_xent->ui4_flags |= FM_XENTRY_FLAG_LOCKED;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_xentry_unlock
 *
 * Description: Unlock the xentry.
 *
 * Inputs:  pt_xent
 *              The reference to the xentry to be unlocked.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_xentry_unlock(
    FM_XENTRY_T *pt_xent)
{
    INT32 i4_ret;
    
    ASSERT(pt_xent != NULL);
    ASSERT(pt_xent->ui2_lock_cnt > 0);
    
    if (--pt_xent->ui2_lock_cnt == 0)
    {
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_LOCKED;
    }
/*
    _show_xent_sema_info(pt_xent, 0);
*/
    i4_ret = x_sema_unlock(pt_xent->h_mtx);

    return (i4_ret == OSR_OK) ? FMR_OK : FMR_CORE;
}


/*-----------------------------------------------------------------------------
 * Name: fm_xentry_ref
 *
 * Description: Reference an entry. The pt_xent will be add to the busy list
 *              and increase the reference count by 1.
 *
 * Inputs:  pt_xent
 *              A reference to xentry. It must be locked.
 *
 * Outputs: -
 *
 * Returns: -
 *          
 ----------------------------------------------------------------------------*/
VOID fm_xentry_ref(
    FM_XENTRY_T *pt_xent)
{
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    pt_xent->ui4_ref_cnt++;

    if (!IS_XENTRY_BUSY(pt_xent))
    {
        _fm_xentry_busy(pt_xent);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fm_xentry_rel
 *
 * Description: Dereference to a file. The reference count will be decreased
 *              by 1. If is is no longer referenced, it will be moved to the
 *              LRU list.
 *
 * Inputs:  pt_xent
 *              The reference to xentry. It must be locked.
 *
 * Outputs: -
 *
 * Returns: -
 *          
 ----------------------------------------------------------------------------*/
VOID fm_xentry_rel(
    FM_XENTRY_T *pt_xent)
{
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    if (pt_xent->ui4_ref_cnt > 0)
    {
        if (--pt_xent->ui4_ref_cnt == 0)
        {
            _fm_xentry_lru(pt_xent);
        }
    }
}


/*-----------------------------------------------------------------------------
 * Name: fm_obj_lock
 *
 * Description: Lock the object to prevent from concurrent reusage. The
 *              object may be a file descriptor or a directory label.
 *
 * Inputs:  pv_obj
 *              The reference to file descriptor or directory label object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              The object is closing and going to be free.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_obj_lock(
    VOID *pv_obj)
{
    FM_MTX_OBJ_T *pt_mtx_obj;
    INT32 i4_ret;

    ASSERT(pv_obj != NULL);

    pt_mtx_obj = (FM_MTX_OBJ_T *)pv_obj;
    
    if (pt_mtx_obj->ui4_status & FM_OBJ_CLOSING)
    {
        return FMR_INVAL;
    }

    pt_mtx_obj->ui2_want_cnt++;
    
    i4_ret = x_sema_lock(pt_mtx_obj->h_mtx, X_SEMA_OPTION_WAIT);
    
    pt_mtx_obj->ui2_want_cnt--;
    
    if (i4_ret != OSR_OK)
    {
        return FMR_CORE;
    }

    if (pt_mtx_obj->ui4_status & FM_OBJ_CLOSING)
    {
        x_sema_unlock(pt_mtx_obj->h_mtx);
        pt_mtx_obj->pf_free(pt_mtx_obj);
        return FMR_INVAL;
    }

    pt_mtx_obj->ui2_lock_cnt++;
    pt_mtx_obj->ui4_status |= FM_OBJ_LOCKED;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_obj_unlock
 *
 * Description: Unlock the object of file descriptor or directory label.
 *
 * Inputs:  pv_obj
 *              The reference to file descriptor or directory label object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_obj_unlock(VOID *pv_obj)
{
    FM_MTX_OBJ_T *pt_mtx_obj;
    INT32 i4_ret;

    if (pv_obj == NULL)
    {
        return FMR_OK;
    }

    pt_mtx_obj = (FM_MTX_OBJ_T *)pv_obj;
    
    if (--pt_mtx_obj->ui2_lock_cnt == 0)
    {
        pt_mtx_obj->ui4_status &= ~FM_OBJ_LOCKED;
    }

    i4_ret = x_sema_unlock(pt_mtx_obj->h_mtx);

    return (i4_ret == OSR_OK) ? FMR_OK : FMR_CORE;
}


/*-----------------------------------------------------------------------------
 * Name: fm_obj_reg
 *
 * Description: Register this object so that we can invalidate it once the
 *              file system is unexpected umounted.
 *
 * Inputs:  pv_obj
 *              The reference to file descriptor or directory label object.
 *          pt_mount
 *              Reference to the mount structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_obj_reg(
    VOID        *pv_obj,
    FM_MOUNT_T  *pt_mount)
{
    FM_MTX_OBJ_T *pt_mtx_obj;

    ASSERT(pv_obj != NULL);
    ASSERT(pt_mount != NULL);

    pt_mtx_obj = (FM_MTX_OBJ_T *)pv_obj;

    x_sema_lock(pt_mount->h_lock_mtx, X_SEMA_OPTION_WAIT);

    DLIST_INSERT_HEAD(pt_mtx_obj, &pt_mount->t_opened_obj_q, t_obj_link);
    
    x_sema_unlock(pt_mount->h_lock_mtx);
}


/*-----------------------------------------------------------------------------
 * Name: fm_obj_unreg
 *
 * Description: Unregister this object.
 *
 * Inputs:  pv_obj
 *              The reference to file descriptor or directory label object.
 *          pt_mount
 *              Reference to the mount structure.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_obj_unreg(
    VOID        *pv_obj,
    FM_MOUNT_T  *pt_mount)
{
    FM_MTX_OBJ_T *pt_mtx_obj;

    ASSERT(pv_obj != NULL);
    ASSERT(pt_mount != NULL);

    pt_mtx_obj = (FM_MTX_OBJ_T *)pv_obj;
    
    x_sema_lock(pt_mount->h_lock_mtx, X_SEMA_OPTION_WAIT);
    
    DLIST_REMOVE(pt_mtx_obj, &pt_mount->t_opened_obj_q, t_obj_link);
    
    x_sema_unlock(pt_mount->h_lock_mtx);
}


/*-----------------------------------------------------------------------------
 * Name: fm_alloc_file_desc
 *
 * Description: Free the file descriptor object.
 *
 * Inputs:  pv_obj
 *              The reference to the file descriptor object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
FM_FILE_DESC_T *fm_alloc_file_desc(VOID)
{
    FM_FILE_DESC_T  *pt_desc;

    pt_desc = x_mem_calloc(1, sizeof(FM_FILE_DESC_T));

    if (pt_desc == NULL)
    {
        return NULL;
    }

    if (x_sema_create(&pt_desc->h_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_desc);
        return NULL;
    }
    
    if (x_sema_create(&pt_desc->h_sync_sema,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_sema_delete(pt_desc->h_mtx);
        x_mem_free(pt_desc);
        return NULL;
    }

    pt_desc->pf_free = fm_free_file_desc;

    return pt_desc;
}

/*-----------------------------------------------------------------------------
 * Name: fm_file_desc_free
 *
 * Description: Free the file descriptor object.
 *
 * Inputs:  pv_obj
 *              The reference to the file descriptor object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fm_free_file_desc(VOID *pv_obj)
{
    FM_FILE_DESC_T *pt_desc;

    ASSERT(pv_obj != NULL);

    pt_desc = (FM_FILE_DESC_T *)pv_obj;
    if (pt_desc->ui2_want_cnt == 0)
    {
        x_sema_delete(pt_desc->h_mtx);
        x_sema_delete(pt_desc->h_sync_sema);
        x_mem_free(pt_desc);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fm_get_file_desc
 *
 * Description: Get the file descriptor with lock.
 *
 * Inputs:  h_file
 *              Handle of a file descriptor.
 *
 * Outputs: ppt_desc
 *              The reference to address of FM_FILE_DESC_T object.
 *
 * Returns: FMR_OK  
 *              Success.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              Invalid object type or object is being closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_get_file_desc(
    HANDLE_T        h_file,
    FM_FILE_DESC_T  **ppt_desc)
{
    HANDLE_TYPE_T   e_obj;

    if (handle_get_type_obj(h_file, &e_obj, (VOID**)ppt_desc) != HR_OK)
    {
        return FMR_HANDLE;
    }
    if (e_obj != FMT_FILE_DESC)
    {
        return FMR_INVAL;
    }
    
    return fm_obj_lock(*ppt_desc);
}


/*-----------------------------------------------------------------------------
 * Name: fm_alloc_dir_label
 *
 * Description: Allocate new directory label object.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: Reference to the allocated directory label object.
 ----------------------------------------------------------------------------*/
FM_DIR_LABEL_T *fm_alloc_dir_label(VOID)
{
    FM_DIR_LABEL_T  *pt_lbl;
    
    pt_lbl = x_mem_calloc(1, sizeof(FM_DIR_LABEL_T) + FM_MAX_PATH_LEN + 1);
    if (pt_lbl == NULL)
    {
        return NULL;
    }
    
    if (x_sema_create(&pt_lbl->h_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_lbl);
        return NULL;
    }

    pt_lbl->ps_path = (CHAR *)(pt_lbl + 1);
    pt_lbl->pf_free = fm_free_dir_label;

    return pt_lbl;
}


/*-----------------------------------------------------------------------------
 * Name: fm_dir_label_free
 *
 * Description: Free the directory label object.
 *
 * Inputs:  pv_obj
 *              The reference to the directory label object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fm_free_dir_label(VOID *pv_obj)
{
    FM_DIR_LABEL_T *pt_lbl;

    ASSERT(pv_obj != NULL);

    pt_lbl = (FM_DIR_LABEL_T *)pv_obj;
    if (pt_lbl->ui2_want_cnt == 0)
    {
        x_sema_delete(pt_lbl->h_mtx);
        x_mem_free(pt_lbl);
    }
}


/*-----------------------------------------------------------------------------
 * Name: fm_get_dir_label
 *
 * Description: Get the direcotry label with lock.
 *
 * Inputs:  h_dir
 *              Handle of a directory label.
 *
 * Outputs: ppt_lbl
 *              The reference to address of FM_DIR_LABEL_T object.
 *
 * Returns: FMR_OK  
 *              Success.
 *          FMR_HANDLE
 *              Fail to get the handle object.
 *          FMR_INVAL
 *              Invalid object type or object is being closed.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_get_dir_label(
    HANDLE_T        h_dir,
    FM_DIR_LABEL_T  **ppt_lbl)
{
    HANDLE_TYPE_T   e_obj;
    
    ASSERT(ppt_lbl != NULL);

    if (h_dir == FM_ROOT_HANDLE)
    {
        *ppt_lbl = NULL;
        return FMR_OK;
    }

    if (handle_get_type_obj(h_dir, &e_obj, (VOID**)ppt_lbl) != HR_OK)
    {
        return FMR_HANDLE;
    }
    
    if (e_obj != FMT_DIR_LABEL)
    {
        return FMR_INVAL;
    }

    return fm_obj_lock(*ppt_lbl);
}
/*-----------------------------------------------------------------------------
 * Name: fm_partition_table_free
 *
 * Description: Free the directory label object.
 *
 * Inputs:  pv_obj
 *              The reference to the directory label object.
 *
 * Outputs: -
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
VOID fm_part_tbl_free(VOID *pv_obj)
{
    FM_PART_TBL_ENT_T *pt_ent_free;
    FM_PART_TBL_ENT_T *pt_ent_current;

    FM_PART_TBL_T *pt_table;

    UINT32 ui4_cnt;

    pt_table = (FM_PART_TBL_T*)pv_obj;

    if (pt_table != NULL)
    {
        for(ui4_cnt = 0; ui4_cnt < 4; ui4_cnt++)
        {
            pt_ent_current = (FM_PART_TBL_ENT_T*)pt_table->t_prm[ui4_cnt].pt_next;

            while(pt_ent_current)
            {
                pt_ent_free = pt_ent_current;
                pt_ent_current = (FM_PART_TBL_ENT_T*)pt_ent_current->pt_next;
                x_mem_free(pt_ent_free);
            }
        }

        x_mem_free(pt_table);
    }    
}
/*-----------------------------------------------------------------------------
 * Name: fm_handle_free_fct
 *
 * Description: Free allocated resources.
 *
 * Inputs:   h_handle
 *           e_type
 *          *pv_obj
 *          *pv_tag
 *           b_req_handle
 *
 * Outputs: -
 *
 * Returns: Always return TRUE.
 ----------------------------------------------------------------------------*/
BOOL fm_handle_free_fct(
    HANDLE_T        h_handle,
    HANDLE_TYPE_T   e_type,
    VOID            *pv_obj,
    VOID            *pv_tag,
    BOOL            b_req_handle)
{
    if (b_req_handle == TRUE)
    {
        switch (e_type)
        {
            case FMT_FILE_DESC:
                fm_free_file_desc(pv_obj);
                break;

            case FMT_DIR_LABEL:
                fm_free_dir_label(pv_obj);
                break;
            case FMT_ASYNC_READ:
            case FMT_ASYNC_WRITE:
                break;
            case FMT_PART_TBL:
                fm_part_tbl_free(pv_obj);
                break;
        }
    }

    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: fm_get_new_xentry
 *
 * Description: Get a new xentry to associate to an entry on the file system.
 *              It first gets from free list. If there is no free xentry and 
 *              the number of all xentry has does not reach the limit, then
 *              allocate a new one. Otherwise, try to get it from the LRU list.
 *
 * Inputs:  ps_name
 *              The name of a file or a directory component.
 *
 * Outputs: -
 *
 * Returns: The reference to FM_XENTRY_T object on success.
 *          Otherwise, NULL is retured.
 ----------------------------------------------------------------------------*/
FM_XENTRY_T *fm_get_new_xentry(const CHAR *ps_name)
{
    FM_XENTRY_T *pt_xent = NULL;
    
    ASSERT(ps_name != NULL);

    _fm_xentry_cache_lock();

    if (!DLIST_IS_EMPTY(&t_free_xentry_q))
    {
        pt_xent = DLIST_HEAD(&t_free_xentry_q);
        DLIST_REMOVE(pt_xent, &t_free_xentry_q, t_qlink);

        ASSERT(pt_xent->ui4_ref_cnt == 0);
        ASSERT(!IS_XENTRY_INVAL(pt_xent));
        ASSERT(!IS_XENTRY_LOCKED(pt_xent));
    }
    else if (ui4_xentry_num < XENTRY_MAX_NUM)
    {
        pt_xent = x_mem_calloc(1, sizeof(FM_XENTRY_T));

        if (pt_xent != NULL)
        {
            INT32 i4_ret;

            if (x_sema_create(&pt_xent->h_mtx,
                              X_SEMA_TYPE_MUTEX,
                              X_SEMA_STATE_UNLOCK) == OSR_OK)
            {
                ui4_xentry_num++;

                if (ui4_xentry_num >= XENTRY_MAX_NUM)
                {
                    DBG_ERROR(("[FM] ERR: WARNING! xentry is full.\n"));
                }
            }
            else
            {
                x_mem_free(pt_xent);
                pt_xent = NULL;
            }

            /* create read/write lock */
            i4_ret = x_rwl_create_lock(&pt_xent->h_rw_lock);
            if (i4_ret != RWLR_OK)
            {
                x_sema_delete(pt_xent->h_mtx);
                x_mem_free(pt_xent);
                pt_xent = NULL;
            }
        }
    }
    
    if ((pt_xent == NULL) && (!DLIST_IS_EMPTY(&t_lru_xentry_q)))
    {
        DLIST_FOR_EACH(pt_xent, &t_lru_xentry_q, t_qlink)
        {
            if (!IS_XENTRY_LOCKED(pt_xent) && !IS_XENTRY_INVAL(pt_xent))
            {
                /* We could remove a xentry from lru even it's dir, we however need
                to make sure that there is none of its children is in busy list.
                */
                if (_fm_is_child_busy(pt_xent) == TRUE)
                {
                    continue;
                }

                DLIST_REMOVE(pt_xent, &t_lru_xentry_q, t_qlink); /* detach lru link */
                SLIST_REMOVE(pt_xent, t_hlink); /* detach hash link */
                if (pt_xent->pv_fs_node != NULL)
                {
                    if (pt_xent->pt_xent_ops->pf_free_fs_node != NULL)
                    {
                        pt_xent->pt_xent_ops->pf_free_fs_node(pt_xent->pv_fs_node);
                    }
                    else
                    {
                        x_mem_free(pt_xent->pv_fs_node);
                    }

                    pt_xent->pv_fs_node = NULL;
                }
                break;
            }
        }
    }

    _fm_xentry_cache_unlock();

    if (pt_xent != NULL)
    {
        SIZE_T      z_len;

        ASSERT(!IS_XENTRY_LOCKED(pt_xent));
        ASSERT(!IS_XENTRY_INVAL(pt_xent));
        
        z_len = x_strlen(ps_name) + 1;
        if (pt_xent->z_name_size < z_len)
        {
            CHAR *ps_new_name;

            ps_new_name = x_mem_realloc(pt_xent->ps_name, z_len);
            if (ps_new_name == NULL)
            {
                pt_xent = NULL;
                goto EXIT;
            }
            pt_xent->ps_name = ps_new_name;
            pt_xent->z_name_size = z_len;
        }

        x_strcpy(pt_xent->ps_name, ps_name);
        pt_xent->pt_prnt_xent = NULL;
        ASSERT(pt_xent->h_mtx != NULL_HANDLE);
        pt_xent->pt_dev       = NULL;
        pt_xent->ui4_mode     = 0;
        pt_xent->pt_mount     = NULL;
        pt_xent->pt_mounted   = NULL;
        pt_xent->pt_ufs_ops   = NULL;
        pt_xent->pt_xent_ops  = NULL;
        pt_xent->pv_fs_node   = NULL;
        pt_xent->ui4_ref_cnt  = 0;
        pt_xent->ui4_flags    = 0;
        pt_xent->ui2_lock_cnt = 0;

        SLIST_INIT(&pt_xent->t_buf_list);
    }
    
EXIT:

    return pt_xent;
}


/*-----------------------------------------------------------------------------
 * Name: fm_xentry_reclaim
 *
 * Description: Detach from the hash chain, and put it back to the free queue.
 *
 * Inputs:  pt_xentry
 *              The xentry to be moved.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_xentry_reclaim(
    FM_XENTRY_T     *pt_xent)
{
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LRU(pt_xent) || IS_XENTRY_INVAL(pt_xent));
    
    _fm_xentry_cache_lock();

    if (IS_XENTRY_LRU(pt_xent))
    {
        SLIST_REMOVE(pt_xent, t_hlink);
        DLIST_REMOVE(pt_xent, &t_lru_xentry_q, t_qlink);
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_LRU;
    }
    if (pt_xent->ui2_want_cnt == 0)
    {
        if (pt_xent->pv_fs_node != NULL)
        {
            if (pt_xent->pt_xent_ops->pf_free_fs_node != NULL)
            {
                pt_xent->pt_xent_ops->pf_free_fs_node(pt_xent->pv_fs_node);
            }
            else
            {
                x_mem_free(pt_xent->pv_fs_node);
            }

            pt_xent->pv_fs_node = NULL;
        }
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_INVAL;
        pt_xent->pt_dev       = NULL;
        pt_xent->pt_mount     = NULL;
        pt_xent->pt_mounted   = NULL;
        pt_xent->pt_prnt_xent = NULL;
        pt_xent->pt_ufs_ops   = NULL;
        pt_xent->pt_xent_ops  = NULL;
        DLIST_INSERT_TAIL(pt_xent, &t_free_xentry_q, t_qlink);
    }
    
    _fm_xentry_cache_unlock();
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_lookup
 *
 * Description: 
 *
 * Inputs:  ps_name
 *              The path to be lookup. It cannot be NULL.
 *          pt_start_dir
 *              
 *          ui2_op
 *              FM_UFS_CREATE
 *              FM_UFS_LOOKUP
 *              FM_UFS_DELETE
 *              FM_UFS_RENAME
 *          ui2_flag
 *              FM_UFS_LOCK_LEAF
 *              FM_UFS_LOCK_PARENT
 *              FM_UFS_WANT_PARENT
 *              FM_UFS_NO_CACHE
 *
 * Outputs: pt_arg
 *              If the directory entry is found, ppt_xent will contain
 *              the information about the xentry. It cannot be NULL.
 *
 * Returns: FMR_OK  
 *              Success.
 *          FMR_NO_ENTRY
 *              One of the directory component cannot be found.
 *          FMR_NOT_DIR
 *              The directory component is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Path exceeds FM_MAX_PATH_LEN.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
typedef enum
{
    FM_LOOKUP_NAME = 0,
    FM_LOOKUP_DOT,
    FM_LOOKUP_DOTDOT
} FM_LOOKUP_TYPE;

INT32 fm_ufs_lookup(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_name,
    UINT16          ui2_op,
    UINT16          ui2_flag,
    FM_LOOKUP_ARG_T *pt_arg)
{
    FM_LOOKUP_TYPE  e_lookup_type;
    FM_XENTRY_T     *pt_xent = NULL;
    FM_XENTRY_T     *pt_prnt_xent = NULL;

    CHAR            *ps_path = NULL;
    CHAR            *ps_comp = NULL;
    CHAR            *ps_next_comp = NULL;
    CHAR            *ps_last_comp = NULL;
    INT32           i4_ret = FMR_OK;

    ASSERT(pt_arg  != NULL);

    ps_path = (CHAR*)x_mem_alloc(sizeof(CHAR) * (FM_MAX_PATH_LEN + 1));
    
    if (ps_path == NULL)
    {
        return FMR_CORE;
    }

    ps_path[FM_MAX_PATH_LEN] = '\0';

    if (pt_dir_lbl == NULL)
    {
        if ((ps_name == NULL) || (*ps_name  == '\0'))
        {
            i4_ret = FMR_NO_ENTRY;
            goto EXIT;
        }

        if (x_strlen(ps_name) > FM_MAX_PATH_LEN)
        {
            i4_ret = FMR_NAME_TOO_LONG;
            goto EXIT;
        }

        pt_prnt_xent = pt_root_xent;
        x_strcpy(ps_path, ps_name);
        ps_comp = ps_path;
    }
    else
    {
        UINT32 ui4_strlen = (ps_name == NULL) ? 0 : x_strlen(ps_name);

        if ((pt_dir_lbl->ui2_len + ui4_strlen) >
            FM_MAX_PATH_LEN)
        {
            i4_ret = FMR_NAME_TOO_LONG;
            goto EXIT;
        }

        pt_prnt_xent = pt_dir_lbl->pt_xent;

        if ((ps_name == NULL) || (*ps_name  == '\0'))
        {
            pt_xent = pt_prnt_xent;
        }

        for (ps_comp = ps_path, ps_next_comp = pt_dir_lbl->ps_path;
             *ps_next_comp != '\0';
             ps_comp++, ps_next_comp++)
        {
            *ps_comp = *ps_next_comp;
        }
        *ps_comp = '\0';

        if (ps_name != NULL)
        {
            x_strcat(ps_comp, ps_name);
        }
    }
    
    /*
     *  1. Handle . and ..
     *  2. Lookup the cache.
     *  3. If it is not found in the cache, use the mounted file system's
     *     lookup function.
     */
    for (ps_last_comp = NULL; *ps_comp != '\0'; ps_comp = ps_next_comp)
    {
        BOOL b_comp_is_dir = FALSE;

        /*
         *  follow through the mount point.
         */
        if (IS_XENTRY_MOUNTED(pt_prnt_xent))
        {
            ASSERT(pt_prnt_xent->pt_mounted != NULL);
            pt_prnt_xent = pt_prnt_xent->pt_mounted->pt_root_xent;
            ASSERT(pt_prnt_xent != NULL);
            ASSERT(pt_prnt_xent->ui4_flags & FM_XENTRY_FLAG_BUSY);
            ASSERT(pt_prnt_xent->ui4_flags & FM_XENTRY_FLAG_MOUNT_ROOT);
        }

        /*
         *  Get the component name to search.
         */
        for (; *ps_comp == '/'; ps_comp++); /* skip contiguous '/' */
        if (*ps_comp == '\0')
        {
            pt_xent = pt_prnt_xent; /* no more component name */
            break;
        }
        if (ps_last_comp != NULL)
        {
            i4_ret = FMR_NO_ENTRY;
            goto EXIT;
        }
        for (ps_next_comp = ps_comp;
             (*ps_next_comp != '/') && (*ps_next_comp != '\0');
             ps_next_comp++);
        if ((*ps_comp == '.') && ((ps_next_comp - ps_comp) == 1))
        {
            e_lookup_type = FM_LOOKUP_DOT;
        }
        else if ((*ps_comp == '.') && (*(ps_comp + 1) == '.') && ((ps_next_comp - ps_comp) == 2))
        {
            e_lookup_type = FM_LOOKUP_DOTDOT;
        }
        else
        {
            e_lookup_type = FM_LOOKUP_NAME;
        }

        if (*ps_next_comp == '/')
        {
            b_comp_is_dir = TRUE;
            *ps_next_comp++ = '\0';
        }

        /* Now we get a component name */

        if (e_lookup_type == FM_LOOKUP_DOT)
        {
            pt_xent = pt_prnt_xent;
            *(ps_next_comp - 1) = '/';
            if (b_comp_is_dir == TRUE)
            {
                *(ps_next_comp - 2) = '/';
            }
            continue;
        }
        else if (e_lookup_type == FM_LOOKUP_DOTDOT)
        {
            if (pt_prnt_xent->ui4_flags & FM_XENTRY_FLAG_MOUNT_ROOT)
            {
                ASSERT(pt_prnt_xent->pt_mount != NULL);

                /* set pt_parent xentry to the mount point xentry */
                pt_prnt_xent = pt_prnt_xent->pt_mount->pt_mp_xent;
                ASSERT(pt_prnt_xent != NULL);
                ASSERT(pt_prnt_xent->ui4_flags & FM_XENTRY_FLAG_BUSY);
                ASSERT(pt_prnt_xent->ui4_flags & FM_XENTRY_FLAG_MOUNTED);
            }

            /*
             *  Replace '..' and the last directory name and with '/'.
             */
            *(--ps_next_comp) = '/';
            *(--ps_next_comp) = '/';
            if (b_comp_is_dir == TRUE)  *(--ps_next_comp) = '/';
            while ((ps_next_comp >= ps_path) && 
                   ((*ps_next_comp == '/') || (*ps_next_comp == '\0')))
            {
                *ps_next_comp-- = '/';
            }
            while ((ps_next_comp >= ps_path) &&
                   (*ps_next_comp != '/') && (*ps_next_comp != '\0'))
            {
                *ps_next_comp-- = '/';
            }
            while (ps_next_comp >= ps_path)
            {
                if (*ps_next_comp == '\0') *ps_next_comp = '/';
                ps_next_comp--;
            }
            ps_next_comp = ps_path;

            pt_prnt_xent = pt_root_xent;
            pt_xent = NULL;

            continue;
        }

        i4_ret = fm_xentry_lock(pt_prnt_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            i4_ret = FMR_NO_ENTRY;
            goto EXIT;
        }
        
        i4_ret = _fm_acquire_xentry(ps_comp, pt_prnt_xent, &pt_xent);

        if (i4_ret == FMR_CORE)
        {
            fm_xentry_unlock(pt_prnt_xent);
            goto EXIT;
        }
        
        if (i4_ret != FMR_OK) /* FMR_NO_ENTRY or FMR_INVAL */
        {
            FM_LOOKUP_ARG_T t_this_arg = {0};

            t_this_arg.pt_prnt_xent = pt_prnt_xent;
            t_this_arg.ps_name      = ps_comp;

            i4_ret = XENTRY_OP_LOOKUP(pt_prnt_xent)(&t_this_arg);
            
            if (i4_ret != FMR_OK)
            {
                if ((i4_ret != FMR_NO_ENTRY) || (ui2_op != FM_UFS_CREATE) ||
                    (*ps_next_comp != '\0'))
                {
                    fm_xentry_unlock(pt_prnt_xent);
                    goto EXIT;
                }

                ui2_flag &= ~FM_UFS_LOCK_PARENT;
                ps_last_comp = ps_comp;
                break;
            }

            pt_xent = t_this_arg.pt_xent;

            ASSERT(pt_xent != NULL);
            ASSERT(!IS_XENTRY_LOCKED(pt_xent));
            ASSERT(!IS_XENTRY_USED(pt_xent));
            ASSERT(!IS_XENTRY_INVAL(pt_xent));

            /* Add this xentry to cache */
            i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
            _fm_xentry_lru(pt_xent);
        }
        else
        {
            /* Perform LRU operatoin */
            if (IS_XENTRY_LRU(pt_xent))
            {
                 DLIST_REMOVE(pt_xent, &t_lru_xentry_q, t_qlink);
                 DLIST_INSERT_TAIL(pt_xent, &t_lru_xentry_q, t_qlink);
            }
        }

        ASSERT(pt_xent != NULL);
        
        if ((b_comp_is_dir == TRUE) && !IS_XENTRY_DIR(pt_xent))
        {
            fm_xentry_unlock(pt_xent);
            fm_xentry_unlock(pt_prnt_xent);
            i4_ret = FMR_NOT_DIR;
            goto EXIT;
        }

        fm_xentry_unlock(pt_xent);
        fm_xentry_unlock(pt_prnt_xent);
        pt_prnt_xent = pt_xent;
    }

    switch (ui2_op)
    {
        case FM_UFS_CREATE:
            if ((ps_last_comp == NULL) && (pt_xent != NULL))
            {
                pt_prnt_xent = pt_xent->pt_prnt_xent;
            }
            if (ui2_flag & FM_UFS_LOCK_PARENT)
            {
                if (pt_prnt_xent != NULL)
                {
                    i4_ret = fm_xentry_lock(pt_prnt_xent, X_SEMA_OPTION_WAIT);
                    if (i4_ret != FMR_OK)
                    {
                        goto EXIT;
                    }
                }
            }
            pt_arg->pt_prnt_xent = pt_prnt_xent;
            
            if (ps_last_comp == NULL)
            {
                /*
                 *  The last component name in the path is found.
                 *  If FM_UFS_LOCK_LEAF is set, lock it.
                 */
                if ((ui2_flag & FM_UFS_LOCK_LEAF) && (pt_xent != NULL))
                {
                    i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
                    if ((i4_ret == FMR_OK) && (!IS_XENTRY_USED(pt_xent)))
                    {
                        fm_xentry_unlock(pt_xent);
                        i4_ret = FMR_NO_ENTRY;
                    }
                    if (i4_ret != FMR_OK)
                    {
                        if ((ui2_flag & FM_UFS_LOCK_PARENT) && (pt_prnt_xent != NULL))
                        {
                            fm_xentry_unlock(pt_prnt_xent);
                        }
                        goto EXIT;
                    }
                }
                pt_arg->pt_xent = pt_xent;
            }
            else
            {
                pt_arg->pt_xent = NULL;
                x_strcpy(pt_arg->ps_name, ps_last_comp);
            }

            i4_ret = FMR_OK;
            
            break;

        case FM_UFS_LOOKUP:
            if (pt_xent == NULL)
            {
                i4_ret = FMR_CORE;
                goto EXIT;
            }            

            pt_prnt_xent = pt_xent->pt_prnt_xent;

            if (ui2_flag & FM_UFS_LOCK_PARENT)
            {
                if (pt_prnt_xent != NULL)
                {
                    i4_ret = fm_xentry_lock(pt_prnt_xent, X_SEMA_OPTION_WAIT);
                    if (i4_ret != FMR_OK)
                    {
                        goto EXIT;
                    }
                }
            }
            
            if (ui2_flag & FM_UFS_LOCK_LEAF)
            {
                i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
                if ((i4_ret == FMR_OK) && (!IS_XENTRY_USED(pt_xent)))
                {
                    fm_xentry_unlock(pt_xent);
                    i4_ret = FMR_NO_ENTRY;
                }
                if (i4_ret != FMR_OK)
                {
                    if ((ui2_flag & FM_UFS_LOCK_PARENT) && (pt_prnt_xent != NULL)) 
                    {
                        fm_xentry_unlock(pt_prnt_xent);
                    }
                    goto EXIT;
                }
            }

            pt_arg->pt_prnt_xent = pt_prnt_xent;
            pt_arg->pt_xent = pt_xent;

            i4_ret = FMR_OK;

            break;
        
        case FM_UFS_DELETE:
        case FM_UFS_RENAME:
            ASSERT(ui2_flag & FM_UFS_LOCK_PARENT);
            ASSERT(ui2_flag & FM_UFS_LOCK_LEAF);

            if (pt_xent == NULL)
            {
                i4_ret = FMR_CORE;
                goto EXIT;
            }            

            pt_prnt_xent = pt_xent->pt_prnt_xent;

            ASSERT(pt_prnt_xent != NULL);

            i4_ret = fm_xentry_lock(pt_prnt_xent, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }

            i4_ret = fm_xentry_lock(pt_xent, X_SEMA_OPTION_WAIT);
            if ((i4_ret == FMR_OK) && (!IS_XENTRY_USED(pt_xent)))
            {
                fm_xentry_unlock(pt_xent);
                i4_ret = FMR_NO_ENTRY;
            }
            if (i4_ret != FMR_OK)
            {
                fm_xentry_unlock(pt_prnt_xent);
                goto EXIT;
            }
            
            pt_arg->pt_prnt_xent = pt_prnt_xent;
            pt_arg->pt_xent = pt_xent;
            
            i4_ret = FMR_OK;

            break;
    }
    
EXIT:

    x_mem_free(ps_path);
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_make_entry
 *
 * Description: Create a new entry on individual file system.
 *
 * Inputs:  pt_dir_lbl
 *              Directory label.
 *          ps_name
 *              Name of the new entry.
 *          pt_mk_ent_arg
 *              The referecne to FM_MKENT_ARG_T object.
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_make_entry(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_MKENT_ARG_T  *pt_mk_ent_arg)
{
    CHAR            *s_name;
    FM_LOOKUP_ARG_T t_lookup_arg;
    FM_XENTRY_T     *pt_prnt_xent = NULL;
    INT32           i4_ret;

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_CREATE,
                           FM_UFS_LOCK_PARENT,
                           &t_lookup_arg);

    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_prnt_xent = t_lookup_arg.pt_prnt_xent;

    if (t_lookup_arg.pt_xent != NULL)
    {
        i4_ret = FMR_EXIST;
        goto EXIT;
    }

    pt_mk_ent_arg->pt_this_xent = pt_prnt_xent;
    pt_mk_ent_arg->ps_name      = t_lookup_arg.ps_name;
    
    i4_ret = XENTRY_OP_MAKE_ENTRY(pt_prnt_xent)(pt_mk_ent_arg);

    if (i4_ret == FMR_OK)
    {
        FM_XENTRY_T *pt_dst_xent = pt_mk_ent_arg->pt_new_xent;

        ASSERT(pt_dst_xent != NULL);
        ASSERT(!IS_XENTRY_LOCKED(pt_dst_xent));
        ASSERT(!IS_XENTRY_USED(pt_dst_xent));
        ASSERT(!IS_XENTRY_INVAL(pt_dst_xent));

        i4_ret = fm_xentry_lock(pt_dst_xent, X_SEMA_OPTION_WAIT);
        if (i4_ret == FMR_OK)
        {
            _fm_xentry_lru(pt_dst_xent);
            fm_xentry_unlock(pt_dst_xent);
        }
    }

EXIT:

    if  (pt_prnt_xent != NULL)
    {
        fm_xentry_unlock(pt_prnt_xent);
    }

    x_mem_free(s_name);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_open
 *
 * Description: Open a file/directory.
 *
 * Inputs:  pt_dir_lbl
 *              Directory label.
 *          ps_path
 *              File/directory path name to be opened.
 *          ui4_flags
 *              Open flags. Valid flags are list as follows:
 *              FM_ACCESS_MODE
 *              FM_READ_ONLY
 *              FM_WRITE_ONLY
 *              FM_READ_WRITE
 *              FM_OPEN_CREATE
 *              FM_OPEN_TRUNC
 *              FM_OPEN_APPEND
 *              FM_OPEN_EXCLUDE
 *          ui4_mode
 *              UNIX like access mode. Valid values are list as follows:
 *              FM_MODE_USR_READ
 *              FM_MODE_USR_WRITE
 *              FM_MODE_USR_EXEC
 *              FM_MODE_GRP_READ
 *              FM_MODE_GRP_WRITE
 *              FM_MODE_GRP_EXEC
 *              FM_MODE_OTH_READ
 *              FM_MODE_OTH_WRITE
 *              FM_MODE_OTH_EXEC
 *              
 * Outputs: ph_file
 *              The reference to the handle of opened file/directory.
 *          ppt_file_desc
 *              The reference to the address of FM_FILE_DESC_T object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_HANDLE
 *              Fail to allocate new handle.
 *          FMR_INVAL
 *              Invalid arguemnts.
 *          FMR_NO_ENTRY
 *              ps_path is not found.
 *          FMR_NOT_DIR
 *              A component name in ps_path is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Length of ps_path is too long.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_open(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    UINT32          ui4_flags,
    UINT32          ui4_mode,
    HANDLE_T        *ph_file,
    FM_FILE_DESC_T  **ppt_file_desc)
{
    CHAR    *s_name;
    FM_LOOKUP_ARG_T t_lookup_arg;
    FM_FILE_DESC_T  *pt_file_desc  = NULL;
    FM_XENTRY_T     *pt_open_xent  = NULL;
    FM_XENTRY_T     *pt_prnt_xent  = NULL;
    INT32           i4_ret;

    ASSERT(ph_file != NULL);

    pt_file_desc = fm_alloc_file_desc();
    if (pt_file_desc == NULL)
    {
        return FMR_CORE;
    }

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    i4_ret = handle_alloc((HANDLE_TYPE_T)FMT_FILE_DESC,
                          pt_file_desc,
                          NULL,
                          fm_handle_free_fct,
                          ph_file);
    if (i4_ret != HR_OK)
    {
        i4_ret = FMR_HANDLE;
        goto EXIT;
    }

    t_lookup_arg.ps_name = s_name;

    if (ui4_flags & FM_OPEN_CREATE)
    {
        if ( ui4_mode & (FM_MODE_TYPE_DIR | FM_MODE_TYPE_BLK | FM_MODE_TYPE_CHR | FM_MODE_TYPE_LINK) )
        {
            i4_ret = FMR_INVAL;
            goto EXIT;
        }
        
        i4_ret = fm_ufs_lookup(pt_dir_lbl,
                               ps_path,
                               FM_UFS_CREATE,
                               FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                               &t_lookup_arg);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }

        pt_prnt_xent = t_lookup_arg.pt_prnt_xent;
        
        if (t_lookup_arg.pt_xent != NULL)
        {
            /* exist */
            pt_open_xent = t_lookup_arg.pt_xent;

            if (ui4_flags & FM_OPEN_EXCLUDE)
            {
                i4_ret = FMR_EXIST;
                goto EXIT;
            }
            if (IS_XENTRY_DIR(pt_open_xent))
            {
                i4_ret = FMR_IS_DIR;
                goto EXIT;
            }

            fm_xentry_unlock(pt_prnt_xent);

            ui4_flags &= ~FM_OPEN_CREATE;
        }
        else
        {
            /* create it */
            FM_MKENT_ARG_T t_mkent_arg;

            t_mkent_arg.pt_this_xent = pt_prnt_xent;
            t_mkent_arg.ps_name      = t_lookup_arg.ps_name;
            t_mkent_arg.ui4_mode     = ui4_mode | FM_MODE_TYPE_FILE;
            
            i4_ret = XENTRY_OP_MAKE_ENTRY(pt_prnt_xent)(&t_mkent_arg);
            
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
            
            i4_ret = fm_xentry_lock(t_mkent_arg.pt_new_xent, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }

            pt_open_xent = t_mkent_arg.pt_new_xent;
            ui4_flags &= ~(FM_OPEN_TRUNC|FM_OPEN_APPEND);
        }
    }
    else
    {
        i4_ret = fm_ufs_lookup(pt_dir_lbl,
                               ps_path,
                               FM_UFS_LOOKUP,
                               FM_UFS_LOCK_LEAF,
                               &t_lookup_arg);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        pt_prnt_xent = t_lookup_arg.pt_prnt_xent;
        pt_open_xent = t_lookup_arg.pt_xent;
    }
    
    ASSERT(pt_open_xent->pt_mount     != NULL);
    ASSERT(pt_open_xent->pt_ufs_ops   != NULL);
    ASSERT(pt_open_xent->pt_xent_ops  != NULL);
    ASSERT(pt_open_xent->pv_fs_node   != NULL);
    ASSERT(pt_open_xent->ui2_lock_cnt == 1);

    if (pt_open_xent->ui4_flags & FM_XENTRY_FLAG_UNLINKED)    
    {        
        i4_ret = FMR_DELETING_ENTRY;
        goto EXIT;
    }

    if (IS_XENTRY_DIR(pt_open_xent))
    {
        if (((ui4_flags & FM_ACCESS_MODE) != FM_READ_ONLY) ||
            (ui4_flags & (FM_OPEN_TRUNC | FM_OPEN_APPEND)))
        {
            i4_ret = FMR_ACCESS;
            goto EXIT;
        }
        if (!FM_IS_DIR(ui4_mode))
        {
            i4_ret = FMR_IS_DIR;
            goto EXIT;
        }
        if (IS_XENTRY_MOUNTED(pt_open_xent))
        {
            FM_XENTRY_T *pt_root_xent;

            pt_root_xent = pt_open_xent->pt_mounted->pt_root_xent;
            i4_ret = fm_xentry_lock(pt_root_xent, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }
            fm_xentry_unlock(pt_open_xent);
            pt_open_xent = pt_root_xent;
        }

        pt_file_desc->ui8_offset = 0;
    }
    else if (IS_XENTRY_FILE(pt_open_xent))
    {
        if (FM_IS_DIR(ui4_mode))
        {
            i4_ret = FMR_NOT_DIR;
            goto EXIT;
        }
        if (ui4_flags & FM_OPEN_TRUNC)
        {
            FM_FILE_INFO_ARG_T   t_set_arg;
            FM_FILE_INFO_T       t_info;
            UINT64               ui8_time;

            /*
             *  Discard data on file system and set 
             *  pf_file_desc->ui8_offset to 0.
             */

            RTC_GetTimeDate(&ui8_time);

            t_info.ui4_mode   = FM_SET_MODE_NOVAL;
            t_info.ui8_size   = 0;
            t_info.ui8_offset = 0;
            t_info.ui8_create_time = FM_SET_TIME_NOVAL;
            t_info.ui8_access_time = ui8_time;
            t_info.ui8_modify_time = ui8_time;
            
            t_set_arg.pt_prnt_xent = pt_prnt_xent;
            t_set_arg.pt_xent = pt_open_xent;
            t_set_arg.pt_info = &t_info;

            i4_ret = XENTRY_OP_SET_INFO(pt_open_xent)(&t_set_arg);
            if (i4_ret != FMR_OK)
            {
                goto EXIT;
            }

            pt_file_desc->ui8_offset = 0;
        }
    }
    else if (IS_XENTRY_BLK(pt_open_xent))
    {
        ASSERT(IS_XENTRY_TRUE(pt_open_xent)); /* be sure this is a true device */

        if (IS_XENTRY_MOUNTED(pt_open_xent) && 
            ((ui4_flags & FM_ACCESS_MODE) != FM_READ_ONLY))
        {
            i4_ret = FMR_ACCESS;
            goto EXIT;
        }
        i4_ret = blkdev_open(pt_open_xent);
        if (i4_ret != FMR_OK)
        {
            goto EXIT;
        }
        ui4_flags &= ~(FM_OPEN_TRUNC | FM_OPEN_APPEND);
    }
    else if (IS_XENTRY_CHR(pt_open_xent))
    {
        ASSERT(0);
    }
    
    fm_xentry_ref(pt_open_xent);

    pt_file_desc->e_type       = FM_TYPE_XENTRY;
    pt_file_desc->pt_xent      = pt_open_xent;
    pt_file_desc->pt_file_ops  = &ufs_file_ops;
    pt_file_desc->ui4_flags    = ui4_flags;
    pt_file_desc->h_ref_handle = *ph_file;

    if (IS_XENTRY_DIR(pt_open_xent) && (pt_open_xent->pt_xent_ops->pf_open_dir != NULL))
    {
        pt_open_xent->pt_xent_ops->pf_open_dir(pt_file_desc);
    }
    
    fm_obj_reg(pt_file_desc, pt_open_xent->pt_mount);
    
    if (ppt_file_desc != NULL)
    {
        *ppt_file_desc = pt_file_desc;
    }

EXIT:

    if (pt_open_xent != NULL)
    {
        fm_xentry_unlock(pt_open_xent);
    }
    if ((pt_prnt_xent != NULL) && (ui4_flags & FM_OPEN_CREATE))
    {
        /* only create operation will lock parent xent */
        fm_xentry_unlock(pt_prnt_xent);
    }
    if (i4_ret != FMR_OK)
    {
        if (*ph_file != NULL_HANDLE)
        {
            handle_free(*ph_file, FALSE);
        }

        fm_free_file_desc(pt_file_desc);
    }

    x_mem_free(s_name);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_delete
 *
 * Description: Lookup and delete a file/directory.
 *
 * Inputs:  pt_dir_lbl
 *              The reference to a directory label.
 *          ps_path
 *              The file/directory to be deleted.
 *          ui4_mode
 *              Type of the entry to be removed. 
 *              FM_MODE_TYPE_DIR or other types.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_ENTRY
 *              One of the directory component cannot be found.
 *          FMR_NOT_DIR
 *              The directory component or ps_path is not a directory.
 *          FMR_IS_DIR
 *              ps_path is a directory.
 *          FMR_NAME_TOO_LONG
 *              Path exceeds FM_MAX_PATH_LEN.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_delete(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    UINT32          ui4_mode)
{
    CHAR            *s_name;
    FM_LOOKUP_ARG_T t_lookup_arg;
    FM_DELETE_ARG_T t_del_arg;
    FM_XENTRY_T     *pt_prnt_xent = NULL;
    FM_XENTRY_T     *pt_xent      = NULL;
    INT32           i4_ret;

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_DELETE,
                           FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }
    pt_prnt_xent = t_lookup_arg.pt_prnt_xent;
    pt_xent      = t_lookup_arg.pt_xent;

    if (IS_XENTRY_BUSY(pt_xent))
    {
        i4_ret = FMR_BUSY;
        pt_xent->ui4_flags |= FM_XENTRY_FLAG_UNLINKED;
        goto EXIT;
    }

    if (FM_IS_DIR(ui4_mode))
    {
        if (!IS_XENTRY_DIR(pt_xent))
        {
            i4_ret = FMR_NOT_DIR;
            goto EXIT;
        }

        if (IS_XENTRY_MOUNTED(pt_xent))
        {
            i4_ret = FMR_BUSY;
            pt_xent->ui4_flags |= FM_XENTRY_FLAG_UNLINKED;
            goto EXIT;
        }
    }
    else
    {
        if (IS_XENTRY_DIR(pt_xent))
        {
            i4_ret = FMR_IS_DIR;
            goto EXIT;
        }
    }

    t_del_arg.pt_prnt_xent = pt_prnt_xent;
    t_del_arg.pt_xent      = pt_xent;
    
    i4_ret = XENTRY_OP_DELETE(pt_xent)(&t_del_arg);

    if (i4_ret == FMR_OK)
    {
        pt_xent->ui4_flags |= FM_XENTRY_FLAG_DELETE;
        pt_xent->ui4_flags &= ~FM_XENTRY_FLAG_UNLINKED;
        fm_xentry_reclaim(pt_xent);
    }
    
EXIT:
    
    if (pt_xent != NULL)
    {
        fm_xentry_unlock(pt_xent);
    }
    if (pt_prnt_xent != NULL)
    {
        fm_xentry_unlock(pt_prnt_xent);
    }

    x_mem_free(s_name);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_rename
 *
 * Description: Rename a file/directory.
 *
 * Inputs:  pt_src_lbl
 *              The reference to old directory label.
 *          pt_old_path
 *              Old file/directory path name.
 *          pt_dst_lbl
 *              The reference to new directory label.
 *          pt_new_path
 *              New file/directory path name.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_ENTRY
 *              One of the source/destination directory component cannot
 *              be found.
 *          FMR_NOT_DIR
 *              The directory component of ps_src_path or ps_dst_path 
 *              is not a directory.
 *          FMR_IS_DIR
 *              ps_dst_path is a directory.
 *          FMR_NAME_TOO_LONG
 *              Path exceeds FM_MAX_PATH_LEN.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_FILE_SYSTEM_OTHER
 *              Source and destination are mounted on different file system.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_rename(
    FM_DIR_LABEL_T  *pt_src_lbl,
    const CHAR      *ps_src_path,
    FM_DIR_LABEL_T  *pt_dst_lbl,
    const CHAR      *ps_dst_path)
{
    CHAR            *s_src_name;
    CHAR            *s_dst_name;
    FM_LOOKUP_ARG_T t_src_lookup;
    FM_LOOKUP_ARG_T t_dst_lookup;
    FM_RENAME_ARG_T t_rename_arg;
    FM_XENTRY_T     *pt_src_xent = NULL;
    FM_XENTRY_T     *pt_src_prnt = NULL;
    FM_XENTRY_T     *pt_dst_xent = NULL;
    FM_XENTRY_T     *pt_dst_prnt = NULL;
    INT32           i4_ret;

    s_src_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_src_name == NULL)
    {
        return FMR_CORE;
    }

    s_dst_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_dst_name == NULL)
    {
        x_mem_free(s_src_name);
        return FMR_CORE;
    }

    t_src_lookup.ps_name = s_src_name;
    t_dst_lookup.ps_name = s_dst_name;

    i4_ret = fm_ufs_lookup(pt_src_lbl,
                           ps_src_path,
                           FM_UFS_RENAME,
                           FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                           &t_src_lookup);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    pt_src_xent = t_src_lookup.pt_xent;
    pt_src_prnt = t_src_lookup.pt_prnt_xent;
    if (IS_XENTRY_MOUNTED(pt_src_xent))
    {
        i4_ret = FMR_BUSY;
        goto EXIT;
    }
    
    i4_ret = fm_ufs_lookup(pt_dst_lbl,
                           ps_dst_path,
                           FM_UFS_CREATE,
                           FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                           &t_dst_lookup);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    pt_dst_xent = t_dst_lookup.pt_xent;
    pt_dst_prnt = t_dst_lookup.pt_prnt_xent;

    if (pt_dst_xent == NULL)
    {
        SIZE_T z_len;
        
        /*
         *  New entry is not found.
         *  Check if the source and target are in the same file system.
         *  Return FMR_OTHER_FILE_SYSTEM if they are not, otherwise call
         *  file system's rename function.
         */
        if (t_dst_lookup.pt_prnt_xent->pt_mount !=
            t_src_lookup.pt_prnt_xent->pt_mount)
        {
            i4_ret = FMR_FILE_SYSTEM_OTHER;
            goto EXIT;
        }
        
        z_len = x_strlen(t_dst_lookup.ps_name) + 1;
        if (pt_src_xent->z_name_size < z_len)
        {
            CHAR *ps_new_name;

            ps_new_name = x_mem_realloc(pt_src_xent->ps_name, z_len);
            if (ps_new_name == NULL)
            {
                i4_ret = FMR_CORE;
                goto EXIT;
            }
            pt_src_xent->ps_name = ps_new_name;
            pt_src_xent->z_name_size = z_len;
        }
            
        t_rename_arg.pt_src_prnt_xent = pt_src_prnt;
        t_rename_arg.pt_dst_prnt_xent = pt_dst_prnt;
        t_rename_arg.pt_xent = pt_src_xent;
        t_rename_arg.ps_name = t_dst_lookup.ps_name;

        i4_ret = XENTRY_OP_RENAME(pt_src_xent)(&t_rename_arg);
        if (i4_ret == FMR_OK)
        {
            pt_src_xent->pt_prnt_xent = pt_dst_prnt;
            x_strcpy(pt_src_xent->ps_name, t_dst_lookup.ps_name);
            _fm_xentry_rehash(pt_src_xent);
        }
    }
    else
    {
        /* New entry is found */
        if (pt_dst_xent == pt_src_xent)
        {
            i4_ret = FMR_OK;
        }
        else if (IS_XENTRY_DIR(pt_dst_xent))
        {
            i4_ret = FMR_IS_DIR;
        }
        else
        {
            i4_ret = FMR_EXIST;
        }
        fm_xentry_unlock(pt_dst_xent);
    }

EXIT:

    if (pt_dst_prnt != NULL)
    {
        fm_xentry_unlock(pt_dst_prnt);
    }
    if (pt_src_xent != NULL)
    {
        fm_xentry_unlock(pt_src_xent);
    }
    if (pt_src_prnt != NULL)
    {
        fm_xentry_unlock(pt_src_prnt);
    }

    x_mem_free(s_src_name);
    x_mem_free(s_dst_name);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_set_info
 *
 * Description: Lookup a path and set the information if it was found.
 *
 * Inputs:  pt_dir_lbl
 *              The reference to a directory label.
 *          ps_path
 *              Path name of the file/directory.
 *          pt_info
 *              The reference to the information to be set.
 *
 * Outputs:
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_ENTRY
 *              One of the directory component cannot be found.
 *          FMR_NOT_DIR
 *              The directory component is not a directory.
 *          FMR_NAME_TOO_LONG
 *              Path exceeds FM_MAX_PATH_LEN.
 *          FMR_PERM_DENY
 *              Permission deny.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_lookup_set_info(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_FILE_INFO_T  *pt_info)
{
    CHAR                *s_name;
    FM_LOOKUP_ARG_T     t_lookup_arg;
    FM_FILE_INFO_ARG_T  t_set_arg;
    INT32               i4_ret;

    s_name = (CHAR*)x_mem_alloc(4 * (FM_MAX_FILE_LEN + 1));
    
    if (s_name == NULL)
    {
        return FMR_CORE;
    }

    t_lookup_arg.ps_name = s_name;
    i4_ret = fm_ufs_lookup(pt_dir_lbl,
                           ps_path,
                           FM_UFS_LOOKUP,
                           FM_UFS_LOCK_PARENT|FM_UFS_LOCK_LEAF,
                           &t_lookup_arg);
    if (i4_ret != FMR_OK)
    {
        goto EXIT;
    }

    t_set_arg.pt_prnt_xent = t_lookup_arg.pt_prnt_xent;
    t_set_arg.pt_xent = t_lookup_arg.pt_xent;
    t_set_arg.pt_info = pt_info;

    i4_ret = XENTRY_OP_SET_INFO(t_lookup_arg.pt_xent)(&t_set_arg);
    
    fm_xentry_unlock(t_lookup_arg.pt_xent);
    fm_xentry_unlock(t_lookup_arg.pt_prnt_xent);
    
EXIT:

    x_mem_free(s_name);

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_init
 *
 * Description: Initialize File Manager.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: FMR_OK  
 *              Success.
 *          FMR_BUSY
 *              The mount point for root file system is already mounted.
 *          FMR_NOT_DIR
 *              The mount point for root file system is not a directory.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_init(VOID)
{
    INT32           i4_ret;

    FM_DIR_LABEL_T  *pt_dir_lbl = NULL;
    FM_MKENT_ARG_T  t_mkent_arg;

    if (b_fm_init_ok == TRUE)
    {
        return FMR_OK;
    }

    i4_ret = fm_buf_init();
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    SLIST_INIT(&t_fs_list);
    SLIST_INIT(&t_mount_list);
    DLIST_INIT(&t_free_xentry_q);
    DLIST_INIT(&t_lru_xentry_q);
    DLIST_INIT(&t_busy_xentry_q);
    i4_ret = _fm_xentry_hash_init();
    if (i4_ret != FMR_OK)
    {
        goto EXIT_1;
    }

    i4_ret = x_sema_create(&h_name_cache_mtx,
                           X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto EXIT_1;
    }

    i4_ret = x_sema_create(&h_opened_obj_mtx,
                           X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto EXIT_1;
    }

    /*
     *  Initializie root file system.
     */
    pt_rootfs_desc = rootfs_init(NULL);
    if (pt_rootfs_desc == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT_1;
    }

    SLIST_INSERT_HEAD(pt_rootfs_desc, &t_fs_list, t_link);
    
    /*
     *  Create root xentry.
     */
    pt_root_xent = fm_get_new_xentry("/");
    if (pt_root_xent == NULL)
    {
        i4_ret = FMR_CORE;
        goto EXIT_1;
    }
    i4_ret = fm_xentry_lock(pt_root_xent, X_SEMA_OPTION_WAIT);
    if (i4_ret != FMR_OK)
    {
        goto EXIT_1;
    }
    
    pt_root_xent->ui4_mode |= (FM_MODE_TYPE_DIR | 0755);
    pt_root_xent->ui4_ref_cnt++;
    
    /*
     *  Mount the root xentry to rootfs.
     */
    i4_ret = _fm_alloc_mount(pt_root_xent, &pt_root_mount);
    if (i4_ret != FMR_OK)
    {
        fm_xentry_unlock(pt_root_xent);
        goto EXIT_2;
    }
    i4_ret = _fm_mount_fs(pt_root_mount, NULL, pt_rootfs_desc, TRUE);
    if (i4_ret != FMR_OK)
    {
        fm_xentry_unlock(pt_root_xent);
        goto EXIT_2;
    }
    SLIST_INSERT_HEAD(pt_root_mount, &t_mount_list, t_link);

    fm_xentry_unlock(pt_root_xent);

    i4_ret = fm_get_dir_label(FM_ROOT_HANDLE, &pt_dir_lbl);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto EXIT_3;
    }

    t_mkent_arg.ui4_mode = FM_MODE_TYPE_DIR | (0666 & FM_MODE_PERM_MASK);
    
    i4_ret = fm_ufs_make_entry(pt_dir_lbl, "/mnt", &t_mkent_arg);

    if (i4_ret != FMR_OK)
    {
        DBG_API(("[FM] %s: return %d\n", __FUNCTION__, i4_ret));
        goto EXIT_3;
    }

    b_fm_init_ok = TRUE;

EXIT_3:

    if (pt_dir_lbl != NULL)
    {
        fm_obj_unlock(pt_dir_lbl);
    }

EXIT_2:

    if (i4_ret != FMR_OK)
    {
        if (pt_root_mount != NULL)
        {
            _fm_free_mount(pt_root_mount);
            pt_root_mount = NULL;
        }
    }

EXIT_1:

    if (i4_ret != FMR_OK)
    {
        if (pt_root_xent != NULL)
        {
            x_mem_free(pt_root_xent->ps_name);
            x_mem_free(pt_root_xent);
            pt_root_xent = NULL;
        }
        if (h_opened_obj_mtx != NULL_HANDLE)
        {
            x_sema_delete(h_opened_obj_mtx);
            h_opened_obj_mtx = NULL_HANDLE;
        }
        if (h_name_cache_mtx != NULL_HANDLE)
        {
            x_sema_delete(h_name_cache_mtx);
            h_name_cache_mtx = NULL_HANDLE;
        }
        if (t_xentry_hash.pt_hash_tbl != NULL)
        {
            x_mem_free(t_xentry_hash.pt_hash_tbl);
            t_xentry_hash.pt_hash_tbl = NULL;
        }
    }
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_create_fs
 *
 * Description: Create a specified file system on a device.
 *
 * Inputs:  pt_dir_lbl
 *              Directory label.
 *          ps_path
 *              Device path.
 *          e_fs_type
 *              File system type.
 *          pv_data
 *              Additional data for individual file system.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_create_fs(
    FM_DIR_LABEL_T  *pt_dir_lbl,
    const CHAR      *ps_path,
    FM_FS_TYPE_T    e_fs_type,
    const VOID      *pv_data)
{
    FM_CREATE_FS_ARG_T  t_arg;
    FM_LOOKUP_ARG_T     t_lookup_arg;
    FM_FS_DESC_T        *pt_fs_desc;
    FM_XENTRY_T         *pt_xent;
    CHAR  *s_name;
    INT32 i4_ret;

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
        return i4_ret;
    }

    pt_xent = t_lookup_arg.pt_xent;

    ASSERT(pt_xent->pt_mount     != NULL);
    ASSERT(pt_xent->pt_ufs_ops   != NULL);
    ASSERT(pt_xent->pt_xent_ops  != NULL);
    ASSERT(pt_xent->pv_fs_node   != NULL);
    ASSERT(pt_xent->ui2_lock_cnt == 1);

    t_arg.pt_xent = pt_xent;
    if (x_sema_create(&t_arg.h_sync_bin,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto EXIT_1;
    }

    if (!IS_XENTRY_VIRT(pt_xent))
    {
        i4_ret = FMR_INVAL; /* invalid device path */
        goto EXIT_2;
    }
    
    if (IS_XENTRY_BLK(pt_xent))
    {
        i4_ret = blkdev_open(pt_xent);
    }
    else
    if (IS_XENTRY_CHR(pt_xent))
    {
        ASSERT(0);
    }
    else
    {
        i4_ret = FMR_INVAL; /* invalid device path */
        goto EXIT_2;
    }
    if (i4_ret != FMR_OK)
    {
        goto EXIT_2;
    }
    
    fm_xentry_ref(pt_xent);

    SLIST_FOR_EACH(pt_fs_desc, &t_fs_list, t_link)
    {
        if (pt_fs_desc->e_fs_type == e_fs_type)
        {
            break;
        }
    }
    
    if (pt_fs_desc == NULL)
    {
        i4_ret = FMR_INVAL; /* invalid file system type */
        goto EXIT_3;
    }
            
    i4_ret = pt_fs_desc->pt_ufs_op->pf_newfs(&t_arg);

EXIT_3:

    fm_xentry_rel(pt_xent);
    if (IS_XENTRY_BLK(pt_xent))
    {
        blkdev_close(pt_xent);
    }
    else
    {
        ASSERT(0);
    }

EXIT_2:

    x_sema_delete(t_arg.h_sync_bin);

EXIT_1:

    fm_xentry_unlock(pt_xent);

    x_mem_free(s_name);

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: fm_ufs_mount
 *
 * Description: Mount a directory to a file system.
 *
 * Inputs:  pt_dev_xent
 *              Reference to the device xentry.
 *          pt_mp_xent
 *              Reference to the mount point directory xentry.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_mount(
    FM_XENTRY_T     *pt_dev_xent,
    FM_XENTRY_T     *pt_mp_xent)
{
    FM_FS_DESC_T    *pt_fs_desc = NULL;
    FM_MOUNT_T      *pt_mount = NULL;

    INT32 i4_ret;
    ROOTFS_ENTRY_T  *pt_rent = NULL;

    CHAR ps_path[32];

    ASSERT(pt_dev_xent != NULL);
    ASSERT(pt_mp_xent  != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_dev_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_mp_xent));

    i4_ret = _fm_alloc_mount(pt_mp_xent, &pt_mount);
    if (i4_ret != FMR_OK)
    {
        return FMR_CORE;
    }

    ASSERT(IS_XENTRY_VIRT(pt_dev_xent));  /* be sure this is a virtual device */

    if (IS_XENTRY_BLK(pt_dev_xent))
    {
        i4_ret = blkdev_open(pt_dev_xent);
        if (i4_ret == FMR_OK)
        {
             pt_mount->t_fs_info.ui4_alignment = pt_dev_xent->pt_dev->t_blk.ui4_align_bytes;
             pt_mount->t_fs_info.ui4_min_blk_size = pt_dev_xent->pt_dev->t_blk.ui4_sec_size;
        }
    }
    else
    {
        ASSERT(0);
    }
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }

    /*
     *  Try to mount that device with each individual non-root file system.
     */

    i4_ret = FMR_INVAL;

    fm_get_xentry_full_path(pt_mp_xent, ps_path, 32);

    pt_rent = ROOTFS_GET_ENT(pt_dev_xent);
    x_memcpy(&pt_mount->t_part_info, &pt_rent->u.pt_part, sizeof(FM_PART_T));

    SLIST_FOR_EACH(pt_fs_desc, &t_fs_list, t_link)
    {
        if (pt_fs_desc->e_fs_type == FM_TYPE_ROOTFS)
        {
            continue;
        }

        i4_ret = _fm_mount_fs(pt_mount, pt_dev_xent, pt_fs_desc, FALSE);

        if (i4_ret == FMR_OK)
        {
            /* Prevent from multiple mounts on the same device */
            pt_dev_xent->ui4_flags |= FM_XENTRY_FLAG_MOUNTED;

            fm_xentry_ref(pt_dev_xent);

            if (pt_dev_xent->ui4_mode & FM_MODE_DEV_VIRT)
            {
                pt_rent->pt_mp_xent = pt_mp_xent;
            }

            x_strcpy(pt_rent->u.pt_part->ps_mnt_path, ps_path);
            pt_rent->u.pt_part->b_mnt = TRUE;

            if (pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct != NULL)
            {
                pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct(FM_MNT_OK,
                                                        pt_dev_xent->ps_name,
                                                        pt_rent->u.pt_part->ui4_part_idx,
                                                        ps_path,
                                                        pt_rent->u.pt_part->t_mnt_cb.pv_tag);
            }

            return FMR_OK;
        }
    }

    if (pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct != NULL)
    {
        pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct(FM_MNT_FAIL,
                                                pt_dev_xent->ps_name,
                                                pt_rent->u.pt_part->ui4_part_idx,
                                                ps_path,
                                                pt_rent->u.pt_part->t_mnt_cb.pv_tag);
    }

    /* Mount fails, no available file system */
    if (IS_XENTRY_BLK(pt_dev_xent))
    {
        blkdev_close(pt_dev_xent);
    }
    else
    {
        ASSERT(0);
    }

FAIL:

    _fm_free_mount(pt_mount);
    
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_umount
 *
 * Description: Unmount mounted file system.
 *
 * Inputs:  pt_xent
 *              Reference to xentry in that file system, including the mount
 *              point itself.
 *          b_enforce
 *              Enforce to perform the unmount operation or not.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 fm_ufs_umount(
    FM_XENTRY_T     *pt_xent,
    BOOL            b_enforce)
{
    FM_MOUNT_T      *pt_mount;
    FM_XENTRY_T     *pt_dev_xent, *pt_prnt_xent;

    FM_DELETE_ARG_T t_del_arg;
    ROOTFS_ENTRY_T  *pt_rent;

    CHAR ps_path[32];

    INT32 i4_ret;

    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    
    if (IS_XENTRY_MOUNTED(pt_xent))
    {
        pt_mount = pt_xent->pt_mounted;
        pt_xent->pt_mounted = NULL;
    }
    else
    {
        pt_mount = pt_xent->pt_mount;
        pt_xent->pt_mount = NULL;
    }

    ASSERT(pt_mount != NULL);

    if (pt_mount->ui4_flags & FM_MNT_FLAG_ROOTFS)
    {
        return FMR_BUSY;
    }

    pt_dev_xent = pt_mount->pt_dev_xent;
    
    ASSERT(pt_dev_xent != NULL);

    if (!IS_XENTRY_MOUNTED(pt_dev_xent))
    {
        return FMR_OK;
    }

    if (pt_mount->ui4_flags & FM_MNT_FLAG_ROOTFS)
    {
        return FMR_BUSY; /* root cannot be unmount */
    }

    /*
     *  Release all associated entries, except root xentry
     *  and mount point xentry.
     */
    i4_ret = _fm_inval_mounted_xent(pt_mount, b_enforce);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
    
    /*
     *  Call individual file system's private unmount function.
     *  Release root xentry and mount point xentry.
     *  Invalidate all opened file descriptor and directory label.
     */
    i4_ret = _fm_umount_fs(pt_mount);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    _fm_free_mount(pt_mount);

    /*
     *  Release the device node.
     */
    pt_prnt_xent = pt_dev_xent->pt_prnt_xent;
    if (pt_prnt_xent != NULL
      && (i4_ret = fm_xentry_lock(pt_prnt_xent, X_SEMA_OPTION_WAIT)) != FMR_OK)
    {
        return i4_ret;
    }
    if ((i4_ret = fm_xentry_lock(pt_dev_xent, X_SEMA_OPTION_WAIT)) != FMR_OK)
    {
        if (pt_prnt_xent != NULL)
        {
            fm_xentry_unlock(pt_prnt_xent);
        }
        return i4_ret;
    }

    ASSERT(IS_XENTRY_VIRT(pt_dev_xent));  /* be sure this is a virtual device */

    if (IS_XENTRY_BLK(pt_dev_xent))
    {
        blkdev_close(pt_dev_xent);
    }
    else
    {
        ASSERT(0);
    }

    pt_dev_xent->ui4_flags &= ~FM_XENTRY_FLAG_MOUNTED;

    fm_xentry_rel(pt_dev_xent);

    fm_get_xentry_full_path(pt_xent, ps_path, 32);

    pt_rent = ROOTFS_GET_ENT(pt_dev_xent);
    pt_rent->u.pt_part->b_mnt = FALSE;

    if (pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct != NULL)
    {
        pt_rent->u.pt_part->t_mnt_cb.pf_mnt_fct(FM_MNT_UMNT,
                                                pt_dev_xent->ps_name,
                                                pt_rent->u.pt_part->ui4_part_idx,
                                                ps_path,
                                                pt_rent->u.pt_part->t_mnt_cb.pv_tag);
    }

    if ((pt_dev_xent->ui4_ref_cnt == 0) &&
         (pt_dev_xent->ui4_flags & FM_XENTRY_FLAG_UNLINKED))
    {
        if ( i4_ret != FMR_OK)
        {
            goto exit;
        }

        t_del_arg.pt_xent = pt_dev_xent;
        t_del_arg.pt_prnt_xent = pt_dev_xent->pt_prnt_xent;

        i4_ret = XENTRY_OP_DELETE(pt_dev_xent)(&t_del_arg);

        if (i4_ret == FMR_OK)
        {
            pt_dev_xent->ui4_flags |= FM_XENTRY_FLAG_DELETE;
            pt_dev_xent->ui4_flags &= ~FM_XENTRY_FLAG_UNLINKED;
            fm_xentry_reclaim(pt_dev_xent);
        }
        
    }

exit:

    /* pt_dev_xent->ui4_flags |= FM_XENTRY_FLAG_INVAL; */
    if (pt_prnt_xent != NULL)
    {
        fm_xentry_unlock(pt_prnt_xent);
    }
    fm_xentry_unlock(pt_dev_xent);
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_ufs_is_locked
 *
 * Description:
 *
 * Inputs:  pt_desc
 *              The reference to a file descriptor.
 *          ui4_lock_op
 *              File lock operations.
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
BOOL fm_ufs_is_file_locked(
    FM_FILE_DESC_T  *pt_desc,
    UINT32          ui4_lock_op)
{
    DEBUG_TRACE();

    ASSERT(pt_desc != NULL);
    ASSERT(pt_desc->pt_xent != NULL);
    ASSERT(pt_desc->pt_xent->h_rw_lock != NULL_HANDLE);
    
    return (ui4_lock_op == FM_LOCK_READ) ?
        x_rwl_read_lock_grabbed(pt_desc->pt_xent->h_rw_lock) :
        x_rwl_write_lock_grabbed(pt_desc->pt_xent->h_rw_lock);
}
/*-----------------------------------------------------------------------------
 * Name: fm_get_logical_partitions
 *
 * Description:
 *
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 fm_get_logical_partitions(
    FM_PART_TBL_ENT_T *pt_prm_ent,
    FM_XENTRY_T *pt_xent,  
    UINT64 ui8_offset,
    UINT32 *pui4_cnt)
{
    INT32 i4_ret = FMR_OK;
    UINT32 ui4_extend_sectors = 0;
    UINT64 ui8_absolute_offset = ui8_offset;
    UINT64 ui8_relative_offset = 0;
    UINT8 *pui1_ebr_buf, *pui1_ebr = NULL;
    HANDLE_T h_sync_sema = NULL_HANDLE;
    FM_RW_ARG_T t_io;
    FM_PART_TBL_ENT_T  *pt_old_ent, *pt_new_ent = pt_prm_ent;

    ASSERT(IS_XENTRY_TRUE(pt_xent));  /* be sure this is a virtual device */

    pui1_ebr_buf = IS_XENTRY_BLK(pt_xent) ?
                   x_mem_alloc(pt_xent->pt_dev->t_blk.ui4_sec_size + 
                               pt_xent->pt_dev->t_blk.ui4_align_bytes) :
                   x_mem_alloc(sizeof(UINT8) * 512);

    if (pui1_ebr_buf == NULL)
    {
        i4_ret = FMR_CORE;
        goto exit;
    }

    if (IS_XENTRY_BLK(pt_xent))
    {
        x_memset(pui1_ebr_buf, 
                 0, 
                 pt_xent->pt_dev->t_blk.ui4_sec_size + 
                 pt_xent->pt_dev->t_blk.ui4_align_bytes);
        
        pui1_ebr = pui1_ebr_buf + 
                   (pt_xent->pt_dev->t_blk.ui4_align_bytes -
                   (((UINT32)pui1_ebr_buf) % pt_xent->pt_dev->t_blk.ui4_align_bytes));
    }
    else
    {
        x_memset((VOID *)pui1_ebr_buf, 0, 512);

        pui1_ebr = pui1_ebr_buf;
    }

    if (x_sema_create(&h_sync_sema, 
                      X_SEMA_TYPE_BINARY, 
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto exit;
    }


    t_io.pt_xent        = pt_xent;
    t_io.pv_data        = pui1_ebr;
    t_io.ui8_offset     = 0;
    t_io.ui4_done_bytes = 0;
    t_io.ui2_flag       = FM_IO_SYNC | FM_IO_DIRECT;
    t_io.h_bin_sema     = h_sync_sema;
    t_io.ui4_count      = IS_XENTRY_BLK(pt_xent) ?
                          pt_xent->pt_dev->t_blk.ui4_sec_size :
                          512;

    do
    {
        t_io.ui8_offset = ui8_absolute_offset + ui8_relative_offset;
        t_io.ui4_done_bytes = 0;

        if (IS_XENTRY_BLK(pt_xent))
        {
            i4_ret = blkdev_read(&t_io);
        }
        else
        {
            ASSERT(0);
        }

        if (i4_ret == FMR_OK)
        {
            FM_PART_T *pt_part = &(pt_new_ent->t_part);
        
            pt_part->ui1_drv  = pui1_ebr[446];            
            pt_part->ui1_start_chs[0] = pui1_ebr[447];    
            pt_part->ui1_start_chs[1] = pui1_ebr[448];    
            pt_part->ui1_start_chs[2] = pui1_ebr[449];    
            pt_part->ui1_type = pui1_ebr[450];            
            pt_part->ui1_end_chs[0] = pui1_ebr[451];      
            pt_part->ui1_end_chs[1] = pui1_ebr[452];      
            pt_part->ui1_end_chs[2] = pui1_ebr[453];
            
            x_end_cnvt_from_little_32(&(pui1_ebr[454]), 
                                      1, 
                                      &(pt_part->ui4_start_lba)); 

            x_end_cnvt_from_little_32(&(pui1_ebr[458]), 
                                      1, 
                                      &(pt_part->ui4_sectors));

            if (IS_XENTRY_BLK(pt_xent))
            {
                pt_part->ui4_start_lba += 
                          (UINT32)((ui8_absolute_offset + ui8_relative_offset) / 
                                    pt_xent->pt_dev->t_blk.ui4_sec_size);
            }
            else
            {
                pt_part->ui4_start_lba += 
                          (UINT32)(ui8_absolute_offset + ui8_relative_offset);
            }

            pt_part->ui4_part_idx = *pui4_cnt;
            pt_part->b_entried = TRUE;

            *pui4_cnt = *pui4_cnt + 1;

            /* 2nd entry of EBR */
            if (pui1_ebr[450 + 16] != 0)
            { 
                /* There are still partitions */

                x_end_cnvt_from_little_32(&(pui1_ebr[454 + 16]), 
                                          1, 
                                          &ui4_extend_sectors);

                if (IS_XENTRY_BLK(pt_xent))
                {
                    ui8_relative_offset = (UINT64)ui4_extend_sectors * 
                                          pt_xent->pt_dev->t_blk.ui4_sec_size;
                }
                else
                {
                     ui8_relative_offset = (UINT64)ui4_extend_sectors * 1;
                }

                pt_old_ent = pt_new_ent;
                pt_new_ent = (FM_PART_TBL_ENT_T *)
                                  x_mem_alloc(sizeof(FM_PART_TBL_ENT_T));

                if (pt_new_ent == NULL)
                {
                    i4_ret = FMR_CORE;
                    goto exit;
                }

                x_memset((VOID *) pt_new_ent, 0, sizeof(FM_PART_TBL_ENT_T));

                pt_old_ent->pt_next = (FM_PART_TBL_ENT_T *)pt_new_ent;
                pt_new_ent->pt_prev = (FM_PART_TBL_ENT_T *)pt_old_ent;
                pt_new_ent->pt_next = NULL;
            }
            else
            {
                break;
            }
        }      
        else
        {
            goto exit;
        }
    } while(1);
   
exit:

    if (pui1_ebr_buf != NULL)
    {
        x_mem_free(pui1_ebr_buf);
    }

    if (h_sync_sema != NULL_HANDLE)
    {
        x_sema_delete(h_sync_sema);
    }

    return i4_ret;    

}
/*-----------------------------------------------------------------------------
 * Name: fm_get_dbg_level
 *
 * Description: This API returns the current setting of debug level.
 *
 * Inputs: -  
 *
 * Outputs: -
 *
 * Returns: The current setting of debug level.
 ----------------------------------------------------------------------------*/ 
UINT16 fm_get_dbg_level(VOID)
{
    return (b_fm_init_ok) ? ui2_fm_dbg_level : 0;
}


/*-----------------------------------------------------------------------------
 * Name: fm_set_dbg_level
 *
 * Description: This API changes the setting of debug level to new one.
 *
 * Inputs: ui2_level            The new setting of debug level.  
 *
 * Outputs: -
 *
 * Returns: TRUE            Routine successful.
 *          FALSE           Routine failed.
 ----------------------------------------------------------------------------*/ 
BOOL fm_set_dbg_level(UINT16 ui2_level)
{
    if (!b_fm_init_ok)
    {
        return FALSE;
    }

    ui2_fm_dbg_level = ui2_level;
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: fm_dump_xentries
 *
 * Description: This API dumps all the UFS entries in the Hyper Terminal.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/ 
VOID fm_dump_xentries(VOID)
{
    INT32 i4_cnt;
    FM_XENTRY_T *pt_xent = NULL;

#if 0
    FM_XENTRY_T *pt_xent_ptr = NULL;
    CHAR ps_entry_name_buf1[256];
    CHAR ps_entry_name_buf2[256];
#endif

    for (i4_cnt = 0; i4_cnt < XENTRY_HASH_SIZE; i4_cnt++)
    {

        SLIST_FOR_EACH(pt_xent, &(t_xentry_hash.pt_hash_tbl[i4_cnt]), t_hlink)
        {
#if 0
            x_memset((VOID *) ps_entry_name_buf1, 0, 256);
            x_strcpy(ps_entry_name_buf1, (const CHAR *) pt_xent->ps_name);

            for (pt_xent_ptr = pt_xent->pt_prnt_xent; pt_xent_ptr != NULL; 
              pt_xent_ptr = pt_xent_ptr->pt_prnt_xent)
            {
                x_memset((VOID *) ps_entry_name_buf2, 0, 256);

                if (*(pt_xent_ptr->ps_name) == '/')
                {
                    x_strcpy(ps_entry_name_buf2, (const CHAR *) pt_xent_ptr->ps_name);
                }
                else
                {
                    x_strcpy(ps_entry_name_buf2, (const CHAR *) pt_xent_ptr->ps_name);
                    ps_entry_name_buf2[x_strlen((const CHAR*)pt_xent_ptr->ps_name)] = '/';
                }
                
                x_strcat(ps_entry_name_buf2, (const CHAR *) ps_entry_name_buf1);
                x_strcpy(ps_entry_name_buf1, (const CHAR *) ps_entry_name_buf2);
            }
#endif
            x_dbg_stmt("entry:%s %d\n", pt_xent->ps_name, pt_xent->ui4_ref_cnt);
        }
    }
}


