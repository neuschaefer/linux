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
 * $RCSfile: fm_linux.c,v $
 * $Revision: #2 $
 * $Date: 2012/05/23 $
 * $Author: hongjun.chu $
 *
 * Description: 
 *         This file implements Linux Core File System. It is mandatorily
 * required by FM.
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "os/inc/x_os.h"
#include "dbg/x_dbg.h"
#include "util/x_end_cnvt.h"
#include "unicode/x_uc_str.h"
#include "handle/x_handle.h"
#include "res_mngr/x_rm_dev_types.h"
#include "file_mngr/fm_util.h"
#include "file_mngr/fm_ufs.h"
#include "file_mngr/fm_blkdev.h"
#include "file_mngr/fm_buf.h"
#include "file_mngr/fm_rootfs.h"
#include "file_mngr/linuxfs/fm_linux.h"
#include <sys/mount.h>
#include <sys/vfs.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#if !defined(ANDROID) && !defined(__UCLIBC__)
#include <aio.h>
#endif
#include <time.h>
#include <string.h>

/* Define, Macro, Typedef */

/* Functions Declarations */
static INT32 linux_format(VOID* pv_data);
static INT32 linux_mount(VOID* pv_data);
static INT32 linux_umount(VOID* pv_data);

#define FM_SUPPORT_FAT_FORMAT

#ifdef FM_SUPPORT_FAT_FORMAT
#include "fm_fat.h"
#endif

static HANDLE_T gh_free_fd_list_sema = NULL_HANDLE;
static DLIST_T(_LINUX_FD_T) gt_free_fd_list;
static DLIST_T(_LINUX_MP_OPENED_FD_T)   gt_mp_open_fd_q;  

/* NTFS UFS operations */
static FM_UFS_OP_T t_linux_ufs_ops =
{
    linux_format,
    NULL,
    linux_mount,
    linux_umount
};

/* NTFS Xentry operations */
 FM_XENTRY_OP_T t_linux_xent_ops =
{
    linux_lookup,      /* pf_lookup */
    linux_read,        /* pf_read */
    linux_write,       /* pf_write */
#if !defined(ANDROID) && !defined(__UCLIBC__)
    linux_aread,       /* pf_read_async */
    linux_awrite,      /* pf_write_async */
#else
    NULL,
    NULL,
#endif
    linux_lseek,       /* pf_lseek */
    linux_close,       /* pf_close */
    linux_get_info,    /* pf_get_info */
    linux_set_info,    /* pf_set_info */
    linux_get_dir_info,/* pf_get_dir_info */
    linux_make_entry,  /* pf_make_entry */
    NULL,              /* pf_open_dir */
    NULL,       /* pf_close_dir */
    linux_read_dir,    /* pf_read_dir */
    linux_delete,      /* pf_delete */
    linux_rename,      /* pf_rename */
    linux_sync,        /* pf_sync */
    linux_seek_dir,    /* pf_seek_dir */
    linux_seek_dir_ex, /* pf_seek_dir_ex */
    linux_free_fs_node,/* pf_free_fs_node */
    linux_build_dir_idx_tbl,
    NULL
};

static CHAR *NTFS_3G_PATH = "/3rd/bin/ntfs-3g";
static CHAR *FSCK_FAT_PATH = "/3rd/bin/fsck.fat";
static CHAR *NTFS_FIX_PATH = "/3rd/bin/ntfsfix";
static CHAR *EXFAT_MOUNT_PATH = "/3rd/bin/mount.exfat-fuse";
static CHAR *EXFAT_FSCK_PATH = "/3rd/bin/exfatfsck";

BOOL file_exist_at(const CHAR *path)
{
    INT32 ret = 0;

    /* access will return 0 if file exist at @path. */
    ret = access(path, F_OK);

    return (ret == 0);
}


INT32 x_fm_linux_init(VOID* pv_data)
{
    FM_FS_DESC_T *pt_fs_desc = NULL;
    INT32 i4_ret = 0;

    /* Allocate a FM_FS_DESC_T structure */
    pt_fs_desc = x_mem_alloc(sizeof(FM_FS_DESC_T));
    if (pt_fs_desc == NULL)
    {
        return FMR_CORE;
    }

    /* Fill the structure */
    x_strcpy(pt_fs_desc->ps_name, "linuxfs");
    pt_fs_desc->e_fs_type  = FM_TYPE_LINUX;
    pt_fs_desc->i4_ref_cnt = 0;
    pt_fs_desc->pt_ufs_op  = &t_linux_ufs_ops;

    /* Register NTFS with the structure */
    i4_ret = fm_reg_fs(pt_fs_desc);
    if (i4_ret != FMR_OK)
    {
        x_mem_free(pt_fs_desc);
        return i4_ret;
    }

    if (x_sema_create(&gh_free_fd_list_sema,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        x_mem_free(pt_fs_desc);
        return FMR_CORE;
    }
    
    DLIST_INIT(&gt_free_fd_list);
    DLIST_INIT(&gt_mp_open_fd_q);

    return i4_ret;
}
/* if pt_xent != NULL, ps_name == pt_xent->ps_name */
CHAR* linux_generate_absolute_name(
    FM_XENTRY_T     *pt_prnt_xent,
    FM_XENTRY_T     *pt_xent,
    const CHAR      *ps_name)
{
    #define MAX_DIR_DEPTH 64
    FM_XENTRY_T     *pt_ptr = NULL;
    FM_XENTRY_T     *pt_mp_xent = NULL;
    CHAR            *ps_absolute_name = NULL;
    CHAR            ps_separator[2];
    const CHAR      *ps_comp_stack[MAX_DIR_DEPTH];/*enough?*/
    size_t          path_len = 0;
    size_t          separator_len;
    int i = 0;
    int stack_index = 0;
    /*DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));*/

    ps_separator[0] = '/';
    ps_separator[1] = '\0';
    separator_len = strlen(ps_separator);
    
    if (pt_prnt_xent != NULL && pt_prnt_xent->pt_mount != NULL)
    {    
        /*DBG_INFO(("[FM] %s: pt_prnt_xent->ps_name:%s \n ", __FUNCTION__,pt_prnt_xent->ps_name));*/
        /*DBG_INFO(("[FM] %s: pt_mount:%d \n ", __FUNCTION__,pt_prnt_xent->pt_mount));*/
        /*DBG_INFO(("[FM] %s: line:%d\n ", __FUNCTION__,__LINE__));*/
        pt_mp_xent = pt_prnt_xent->pt_mount->pt_mp_xent;
        /*DBG_INFO(("[FM] %s: line:%d\n ", __FUNCTION__,__LINE__));*/
    }
    else
    {
        if (pt_xent != NULL)
        {
            pt_mp_xent = pt_xent->pt_mount->pt_mp_xent;
        }
    }

    ps_comp_stack[i++] = ps_name;


    pt_ptr = pt_prnt_xent;
    /*DBG_INFO(("[FM] %s: ps_comp_stack[%d]=%s\n", __FUNCTION__,i-1,ps_comp_stack[i-1]));*/

    while(pt_ptr != NULL && i < MAX_DIR_DEPTH)
    {
        ps_comp_stack[i++] = pt_ptr->ps_name;
        
        /*DBG_INFO(("[FM] %s: ps_comp_stack[%d]=%s\n", __FUNCTION__,i-1,ps_comp_stack[i-1]));*/
        pt_ptr = pt_ptr->pt_prnt_xent;
    }

    pt_ptr = pt_mp_xent;
    while(pt_ptr != NULL && i < MAX_DIR_DEPTH)
    {
        ps_comp_stack[i++] = pt_ptr->ps_name;
        
        /*DBG_INFO(("[FM] %s: ps_comp_stack[%d]=%s\n", __FUNCTION__,i-1,ps_comp_stack[i-1]));*/
        pt_ptr = pt_ptr->pt_prnt_xent;
    }
    
    for (stack_index = i - 1; stack_index >= 0; stack_index--) 
    {
        path_len += strlen(ps_comp_stack[stack_index]) + separator_len;
    }
    path_len ++;


    /* Caller must free the buf */
    ps_absolute_name = (CHAR *)x_mem_alloc(path_len);
    if (ps_absolute_name == NULL)
    {
        return NULL;
    }
    ps_absolute_name[0] = '/';
    ps_absolute_name[1] = '\0';
    i--;
    for(;i>=0;i--)
    {
        if(x_strcmp(ps_comp_stack[i], ps_separator))
        {
            x_strcat(ps_absolute_name, ps_comp_stack[i]);
            if (i != 0)
            {
                x_strcat(ps_absolute_name, ps_separator);
            }
        }
    }
    
    DBG_INFO(("[FM] %s: ps_absolute_name=%s\n", __FUNCTION__,ps_absolute_name));
    return ps_absolute_name;
}

static LINUX_FD_T* _linux_acquire_fd_path_map()
{
    LINUX_FD_T* pt_fd = NULL;

    if (x_sema_lock(gh_free_fd_list_sema, X_SEMA_OPTION_WAIT) == OSR_OK)
    {
        if (!DLIST_IS_EMPTY(&gt_free_fd_list))
        {
            pt_fd = DLIST_HEAD(&gt_free_fd_list);
            DLIST_REMOVE(pt_fd, &gt_free_fd_list, t_link);
        }

        x_sema_unlock(gh_free_fd_list_sema);
    }
    
    if (pt_fd == NULL)
    {
        pt_fd = malloc(sizeof(LINUX_FD_T));
    }

    return pt_fd;
}


static LINUX_FD_INFO_T* _linux_submit_fd(LINUX_DENTRY_T * pt_ent, HANDLE_T h_file, INT32 i4_fd)
{
    LINUX_FD_INFO_T * pt_fd_info = NULL;

    if (pt_ent == NULL || h_file == NULL_HANDLE)
    {
        return NULL;
    }

    pt_fd_info = (LINUX_FD_INFO_T *)x_mem_alloc(sizeof(LINUX_FD_INFO_T));
    if (pt_fd_info == NULL)
    {
        return NULL;
    }

    pt_fd_info->h_file = h_file;
    pt_fd_info->i4_fd = i4_fd;
    pt_fd_info->ui8_aio_write_req_sent = 0;
    pt_fd_info->ui8_aio_write_req_recv = 0;
    DLIST_INSERT_TAIL(pt_fd_info, &pt_ent->t_fd_list, t_link);

    return pt_fd_info;
}

static LINUX_FD_INFO_T* _linux_find_fd_info_by_fd(LINUX_DENTRY_T * pt_ent, INT32 i4_fd)
{
    LINUX_FD_INFO_T * pt_fd_info;

    if (pt_ent == NULL || i4_fd <= 0)
    {
        return NULL;
    }

    DLIST_FOR_EACH(pt_fd_info, &pt_ent->t_fd_list, t_link)
    {
        if (pt_fd_info->i4_fd == i4_fd)
        {
            return pt_fd_info;
        }
    }

    return NULL;
}

static LINUX_FD_INFO_T* _linux_find_fd_info_by_fh(LINUX_DENTRY_T * pt_ent, HANDLE_T h_file)
{
    LINUX_FD_INFO_T * pt_fd_info;

    if (pt_ent == NULL || h_file == NULL_HANDLE || DLIST_IS_EMPTY(&pt_ent->t_fd_list))
    {
        return NULL;
    }

    DLIST_FOR_EACH(pt_fd_info, &pt_ent->t_fd_list, t_link)
    {
        if (pt_fd_info->h_file == h_file)
        {
            return pt_fd_info;
        }
    }

    return NULL;
}

static INT32 _linux_find_fd_by_fh(FM_XENTRY_T* pt_xent, HANDLE_T h_file, INT32* pi4_fd)
{
    LINUX_FD_INFO_T *pt_fd_info = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;

    if (pt_xent == NULL || h_file == NULL_HANDLE || pi4_fd == NULL)
    {
        ASSERT(0);
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_ldentry = (LINUX_DENTRY_T *)pt_xent->pv_fs_node;
    if (pt_ldentry == NULL)
    {
        ASSERT(0);
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    pt_fd_info = _linux_find_fd_info_by_fh(pt_ldentry, h_file);
    if (pt_fd_info == NULL)
    {
        /* Not find, create a new fd */
        CHAR* ps_absolute_name;

        ps_absolute_name = linux_generate_absolute_name(
            pt_xent->pt_prnt_xent,
            pt_xent,
            pt_xent->ps_name);

        if (ps_absolute_name == NULL)
        {
            ASSERT(0);
            
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            return FMR_CORE;
        }
        else
        {
            if (pt_xent->ui4_mode & FM_MODE_TYPE_FILE)
            {
                struct stat buf;
                INT32  i4_ret;
                
                DBG_INFO(("[FM] %s: line:%d, open(%s)\n", __FUNCTION__,__LINE__, ps_absolute_name));
                *pi4_fd = open(ps_absolute_name, O_RDWR, (pt_xent->ui4_mode & 0777));
                if (*pi4_fd < 0)
                {
                    if (errno == EROFS || errno == EACCES)
                    {
                        /* pathname refers to a file on a read-only filesystem and write access was requested. */
                        *pi4_fd = open(ps_absolute_name, O_RDONLY, (pt_xent->ui4_mode & 0777));
                        if (*pi4_fd < 0)
                        {
                            DBG_INFO(("[FM] %s: line:%d, open fail:%s\n", __FUNCTION__,__LINE__, strerror(errno)));
                            x_mem_free(ps_absolute_name);
                            return FMR_CORE;
                        }
                    }
                    else
                    {
                        DBG_INFO(("[FM] %s: line:%d, open fail:%s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        x_mem_free(ps_absolute_name);
                        return FMR_CORE;
                    }
                }
                DBG_INFO(("[FM] %s: line:%d, open(%s) ok, get fd:%ld\n", __FUNCTION__,__LINE__, ps_absolute_name, *pi4_fd));
                
                /* Get the attr */
                i4_ret = stat(ps_absolute_name, &buf);
                if (i4_ret < 0)
                {
                    i4_ret = FMR_CORE;
                    DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                    x_mem_free(ps_absolute_name);
                    return FMR_CORE;
                }
                pt_ldentry->ui8_size = buf.st_size;
                pt_ldentry->ui4_mode = buf.st_mode;

                /* submit the h_file<->i4_fd map */
                pt_fd_info = _linux_submit_fd(pt_ldentry, h_file, *pi4_fd);

            }
            else if (pt_xent->ui4_mode & FM_MODE_TYPE_DIR)
            {
                
                DBG_INFO(("[FM] %s: line:%d, opendir(%s)\n", __FUNCTION__,__LINE__, ps_absolute_name));
                *pi4_fd = (int)opendir(ps_absolute_name);
                if (*pi4_fd == 0)
                {
                    x_mem_free(ps_absolute_name);
                    
                    DBG_INFO(("[FM] %s: line:%d, open fail:%s\n", __FUNCTION__,__LINE__, strerror(errno)));
                    return FMR_CORE;
                }
                
                DBG_INFO(("[FM] %s: line:%d, open(%s) ok, get fd:%ld\n", __FUNCTION__,__LINE__, ps_absolute_name, *pi4_fd));
                /* submit the h_file<->i4_fd map */
                pt_fd_info = _linux_submit_fd(pt_ldentry, h_file, *pi4_fd);
            }
            else
            {
                x_mem_free(ps_absolute_name);
                
                DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
                return FMR_CORE;
            }
        }
        #if 1
        {
            LINUX_FD_T* pt_fd = NULL;
            LINUX_MP_OPENED_FD_T* pt_mpofd = NULL;

            DLIST_FOR_EACH(pt_mpofd, &gt_mp_open_fd_q, t_link)
            {
                if (strstr(ps_absolute_name, pt_mpofd->ps_mp_path))
                {
                    break;
                }
            }
            if (pt_mpofd != NULL)
            {
                pt_fd = _linux_acquire_fd_path_map();
                if(pt_fd != NULL)
                {
                    pt_fd->i4_fd = *pi4_fd;
                    pt_fd->b_file = (pt_xent->ui4_mode & FM_MODE_TYPE_DIR) ? (FALSE) : (TRUE);
                    	
                    if (x_sema_lock(pt_mpofd->h_mtx, X_SEMA_OPTION_WAIT) == OSR_OK)
                    {
                        DLIST_INSERT_HEAD(pt_fd, &pt_mpofd->t_fd_list, t_link);
                    
                        DBG_INFO(("[FM-Linux] Add %ld to %s's OPENED FD List\n", *pi4_fd, pt_mpofd->ps_mp_path));
                        
                        x_sema_unlock(pt_mpofd->h_mtx);
                    }
                }
            } 
        }
        #endif
        x_mem_free(ps_absolute_name);
    }
    else
    {
        *pi4_fd = pt_fd_info->i4_fd;
    }
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/

    return FMR_OK;
}


#if !defined(ANDROID) && !defined(__UCLIBC__)
void _linux_aio_callback(sigval_t sigval)
{
    INT32 i4_ret;
    FM_RW_ARG_T  *pt_rw_arg;
    struct aiocb *pt_aiocb;
    AIO_CB_TAG_T *pt_aiocb_tag;

    pt_aiocb_tag = (AIO_CB_TAG_T *)sigval.sival_ptr;
    pt_rw_arg = pt_aiocb_tag->pt_rw_arg;
    pt_aiocb  = pt_aiocb_tag->pt_aiocb;

    /* Did the request complete? */
    DBG_INFO(("_fm_aio_callback:paio_cb:%ld\n", (UINT32)pt_aiocb));

    if (FALSE == pt_aiocb_tag->b_read)
    {
        LINUX_DENTRY_T  *pt_ldentry = NULL;    
        LINUX_FD_INFO_T *pt_fd_info = NULL;
        
        pt_ldentry = (LINUX_DENTRY_T *)pt_rw_arg->pt_xent->pv_fs_node;
        
        pt_fd_info = _linux_find_fd_info_by_fd(pt_ldentry, pt_aiocb->aio_fildes);
        if (pt_fd_info != NULL)
        {
            pt_fd_info->ui8_aio_write_req_recv ++;
        }
    }
    
    i4_ret = aio_error( pt_aiocb );
    if (i4_ret != 0)
    {
        if (i4_ret == ECANCELED)
        {
            DBG_ERROR(("_fm_aio_callback:FM_ASYNC_COND_ABORT_OK.paio_cb:%ld,(%s)\n",(UINT32)pt_aiocb, strerror(errno)));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_ABORT_OK, 0);
        }
        else
        {
            DBG_ERROR(("_fm_aio_callback:FM_ASYNC_COND_FAIL.paio_cb:%ld,(%s)\n",(UINT32)pt_aiocb, strerror(errno)));
            pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_FAIL, 0);
        }
    }
    else
    {
        /* Request completed successfully, get the return status */
        i4_ret = aio_return( pt_aiocb );
        if (pt_aiocb_tag->b_read)
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_READ_OK,read_count:%d\n", i4_ret));
            if(i4_ret == 0)
            {
                pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_EOF, i4_ret);
            }
            else
            {
                pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_READ_OK, i4_ret);
            }

        }
        else
        {
            DBG_INFO(("_fm_aio_callback:FM_ASYNC_COND_WRITE_OK,write_count:%d\n", i4_ret));
            if(i4_ret == 0)
            {
                pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_EOF, i4_ret);
            }
            else
            {
                pt_rw_arg->pf_nfy_fct(pt_rw_arg->h_rw_req, pt_rw_arg->pv_tag, FM_ASYNC_COND_WRITE_OK, i4_ret);
            }
        }
    }
    x_handle_free(pt_rw_arg->h_rw_req);
    x_mem_free(pt_rw_arg);
    x_mem_free(pt_aiocb);
    x_mem_free(pt_aiocb_tag);
}
#endif


static UINT8 _fat_disksize_to_sectors_per_cluster(
    UINT64      ui4_dev_size,
    FAT_TYPE_T *pe_fat_type)
{
    UINT8  ui1_ret = 0;
    INT32 i;

    switch (*pe_fat_type)
    {
        case FAT_TYPE_ANY:
        case FAT_TYPE_12:

            /* we do not format disc in FAT 12 */

        case FAT_TYPE_16:
        {
            *pe_fat_type = FAT_TYPE_16;

            for (i = 0; i < (INT32)(sizeof(t_fat16_disk_tbl)/sizeof(FAT_DISKSIZE_TO_SECPERCLUS)); i++)
            {
                if (ui4_dev_size <= t_fat16_disk_tbl[i].ui4_disk_size)
                {
                    ui1_ret = t_fat16_disk_tbl[i].ui1_sec_per_clus;
                    break;
                }
            }
            
            if (ui1_ret != 0)
            {
                return ui1_ret;
            }
        }   

            /* once the given space is not suitable for FAT 16, we try 
               to format it in FAT 32.*/

        case FAT_TYPE_32:
        {
            *pe_fat_type = FAT_TYPE_32;

            for (i = 0; i < (INT32)(sizeof(t_fat32_disk_tbl)/sizeof(FAT_DISKSIZE_TO_SECPERCLUS)); i++)
            {
                if (ui4_dev_size <= t_fat32_disk_tbl[i].ui4_disk_size)
                {
                    ui1_ret = t_fat32_disk_tbl[i].ui1_sec_per_clus;
                    break;
                }
            }
            return ui1_ret;
        }
    }

    return 0;
}


static INT32 linux_format(
    VOID* pv_data)
{
    /* we now only support FAT format */
    FM_CREATE_FS_ARG_T *pt_arg;
    FM_XENTRY_T *pt_xent;
    FM_DEVICE_T *pt_dev;
    FAT_TYPE_T  e_fat_type = FAT_TYPE_ANY;
    HANDLE_T    h_sync_lock;
    UINT8  *pui1_bs = NULL;
    UINT8  *pui1_fat = NULL;
    ROOTFS_ENTRY_T  *pt_rent = NULL;
    UINT8  ui1_sec_per_clus;
    UINT8  ui1_num_fat = 2;
    UINT16 ui2_root_ent_cnt;
    UINT16 ui2_rsv_sec_cnt;
    UINT32 ui4_root_dir_sec;
    UINT32 ui4_fat_size;
    UINT32 ui4_disk_sec;
    UINT32 ui4_sec_size;
    UINT32 ui4_free_cnt;
    UINT32 ui4_tmp1, ui4_tmp2;
    UINT64 ui8_offset;
    UINT32 ui4_done;
    UINT32  i, j;
    INT32  i4_ret = FMR_OK;
    
    if (pv_data == NULL)
    {
        return FMR_ARG;
    }
    
    pt_arg = (FM_CREATE_FS_ARG_T *)pv_data;

    pt_xent    = pt_arg->pt_xent;
    h_sync_lock = pt_arg->h_sync_bin;

    if ((pt_xent == NULL) || (!IS_XENTRY_LOCKED(pt_xent)) || (h_sync_lock == NULL_HANDLE))
    {
        return FMR_INVAL;
    }
    
    pt_dev = pt_xent->pt_dev;
    pt_rent = ROOTFS_GET_ENT(pt_xent);

    /* Determine some parameters. */
    /* Here should be a visual device */
    if (IS_XENTRY_VIRT(pt_xent))
    {
        if (IS_XENTRY_BLK(pt_xent))
        {            
            /* Block device */
            ui4_sec_size = pt_dev->u_param.t_blkdev.ui4_sec_size;
            ui4_disk_sec = (UINT32)pt_dev->u_param.t_blkdev.ui8_sec_num;            
        }
        else
        {
            /* character device */
            ui4_sec_size = 512;
            ui4_disk_sec = (UINT32)(pt_dev->ui8_space >> 9);
        }
    }
    else
    {
        return FMR_INVAL;
    }

    if (ui4_sec_size == 512)
    {
        ui1_sec_per_clus = _fat_disksize_to_sectors_per_cluster(ui4_disk_sec, &e_fat_type);
        if ((ui1_sec_per_clus == 0) || (e_fat_type == FAT_TYPE_ANY))
        {
            return FMR_INVAL;
        }
    }
    else
    {
        if ((UINT64)(ui4_disk_sec * ui4_sec_size) < (UINT64)0x80000000)
        {
            e_fat_type = FAT_TYPE_16;
        }
        else
        {
            e_fat_type = FAT_TYPE_32;
        }
#if 0
        /* The disk formatted by this way will be most efficient, but the FAT size
           may be so large */
        ui1_sec_per_clus = 1;

        while(1)
        {
            if (e_fat_type == FAT_TYPE_16)
            {
                if (ui4_disk_sec / ui1_sec_per_clus <= 0xFFFF)
                {
                    break;
                }
                else
                {
                    ui1_sec_per_clus *= 2;
                }
            }
            else
            {
                if (ui4_disk_sec / ui1_sec_per_clus <= 0x0FFFFFFF)
                {
                    break;
                }
                else
                {
                    ui1_sec_per_clus *= 2;
                }
            }
        }
        if (ui1_sec_per_clus * ui4_sec_size >= 0x8000)
        {
            return FMR_INVAL;
        }
#else
        /* This is the simplest method, but the disk formatted by this method may not be so efficient */
        ui1_sec_per_clus = (UINT8)(((UINT32)0x8000) / ui4_sec_size);
#endif
    }

    ui2_root_ent_cnt = (UINT16)((e_fat_type == FAT_TYPE_32) ? 0 : 512);
    ui2_rsv_sec_cnt  = (UINT16)((e_fat_type == FAT_TYPE_32) ? 32 : 1);

    if(ui4_sec_size < 256)
    {
        return FMR_CORE;
    }
    
    /*
     *  Setup boot sector
     */
    pui1_bs = x_mem_alloc(ui4_sec_size * sizeof(UINT8));

    if (pui1_bs == NULL )
    {
        return FMR_CORE;
    }
    
    x_memset(pui1_bs, 0, ui4_sec_size);
    
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP0, 0xEB);
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP1, (UINT8)((e_fat_type == FAT_TYPE_32) ? 0x58 : 0x3C));
    FAT_SET_VAL8(pui1_bs, FAT_BS_JMP2, 0x90);
    x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_OEM_NAME), "MSWIN4.1", 8);

    FAT_SET_VAL16(pui1_bs, FAT_BS_BYTES_PER_SEC, ui4_sec_size);
    FAT_SET_VAL8(pui1_bs, FAT_BS_SEC_PER_CLUS, ui1_sec_per_clus);
    FAT_SET_VAL16(pui1_bs, FAT_BS_RSV_SEC_CNT, ui2_rsv_sec_cnt);
    FAT_SET_VAL8(pui1_bs,  FAT_BS_NUM_FAT, ui1_num_fat);
    FAT_SET_VAL16(pui1_bs, FAT_BS_ROOT_ENT_CNT, ui2_root_ent_cnt);
    FAT_SET_VAL8(pui1_bs,  FAT_BS_MEDIA, 0xF8);
    FAT_SET_VAL16(pui1_bs, FAT_BS_SEC_PER_TRK, 63);
    FAT_SET_VAL16(pui1_bs, FAT_BS_NUM_HEADS, 255);
    FAT_SET_VAL32(pui1_bs, FAT_BS_HIDDEN_SEC, 0);
    
    if ((e_fat_type == FAT_TYPE_32) || (ui4_disk_sec > 0xFFFF))
    {
        FAT_SET_VAL32(pui1_bs, FAT_BS_TOTAL_SEC32, ui4_disk_sec);
    }
    else
    {
        FAT_SET_VAL16(pui1_bs, FAT_BS_TOTAL_SEC16, ui4_disk_sec);
    }
    
    /*
     *  Determine fat size.
     */
    if (e_fat_type == FAT_TYPE_32)
    {
        ui4_root_dir_sec = (UINT32)(ui1_sec_per_clus * 2) /* sec_per_clus * root_clus */;
    }
    else
    {
        ui4_root_dir_sec = ((ui2_root_ent_cnt * 32) + (ui4_sec_size - 1)) / ui4_sec_size;
    }

    ui4_tmp1 = ui4_disk_sec - (ui2_rsv_sec_cnt + ui4_root_dir_sec);
    
    if (e_fat_type == FAT_TYPE_32)
    {
        ui4_tmp2 = (UINT32)((ui4_sec_size * ui1_sec_per_clus / 4) + ui1_num_fat);
    }
    else
    {
        ui4_tmp2 = (UINT32)((ui4_sec_size * ui1_sec_per_clus / 2) + ui1_num_fat);
    }

    ui4_fat_size = (ui4_tmp1 + (ui4_tmp2 - 1)) / ui4_tmp2;
    
    if (e_fat_type == FAT_TYPE_32)
    {
        FAT_SET_VAL32(pui1_bs, FAT_BS_FAT_SIZE32, ui4_fat_size);
        FAT_SET_VAL32(pui1_bs, FAT_BS_ROOT_CLUS, 2);
        FAT_SET_VAL16(pui1_bs, FAT_BS_FS_INFO, 1);
        FAT_SET_VAL16(pui1_bs, FAT_BS_BK_BOOT_SEC, 6);
        FAT_SET_VAL8(pui1_bs,  FAT_BS_BOOT_SIG32, 0x29);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_VOL_LAB32), "NO NAME    ", 11);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_FS_TYPE32), "FAT32   ", 8);
    }
    else
    {
        FAT_SET_VAL16(pui1_bs, FAT_BS_FAT_SIZE16, ui4_fat_size);
        FAT_SET_VAL8(pui1_bs,  FAT_BS_BOOT_SIG, 0x29);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_VOL_LAB), "NO NAME    ", 11);
        x_memcpy(FAT_OFFSET(pui1_bs, FAT_BS_FS_TYPE), "FAT16   ", 8);
    }

    pui1_bs[510] = 0x55;
    pui1_bs[511] = 0xAA;

    ui4_done = pwrite(pt_dev->h_driver, pui1_bs, ui4_sec_size, 0);
    if (ui4_done != ui4_sec_size)
    {
        goto EXIT;
    }

    /*
     *  Construct FAT32 FSInfo Sector
     */
    if (e_fat_type == FAT_TYPE_32)
    {
        x_memset(pui1_bs, 0, ui4_sec_size);
    
        pui1_bs[FAT_FSI_LEAD_SIG] = 0x52;
        pui1_bs[FAT_FSI_LEAD_SIG + 1] = 0x52;
        pui1_bs[FAT_FSI_LEAD_SIG + 2] = 0x61;
        pui1_bs[FAT_FSI_LEAD_SIG + 3] = 0x41;
        pui1_bs[FAT_FSI_STRUC_SIG] = 0x72;
        pui1_bs[FAT_FSI_STRUC_SIG + 1] = 0x72;
        pui1_bs[FAT_FSI_STRUC_SIG + 2] = 0x41;
        pui1_bs[FAT_FSI_STRUC_SIG + 3] = 0x61;

        ui4_free_cnt = (ui4_disk_sec -                 /* total sectors */
                        ui2_rsv_sec_cnt -               /* reserved sectors */
                        ui4_fat_size * ui1_num_fat -    /* fat sectors */
                        ui4_root_dir_sec) /             /* root dir sectors = 2 */
                        ui1_sec_per_clus;               /* sectors per cluster */

        pui1_bs[FAT_FSI_FREE_COUNT] = (UINT8)(FAT_UINT32(ui4_free_cnt));
        pui1_bs[FAT_FSI_NXT_FREE] = 0x03;
        pui1_bs[FAT_FSI_TRAIL_SIG] = 0x00;
        pui1_bs[FAT_FSI_TRAIL_SIG + 1] = 0x00;
        pui1_bs[FAT_FSI_TRAIL_SIG + 2] = 0x55;
        pui1_bs[FAT_FSI_TRAIL_SIG + 3] = 0xAA;

        ui4_done = pwrite(pt_dev->h_driver, pui1_bs, ui4_sec_size, ui4_sec_size);
        if (ui4_done != ui4_sec_size)
        {
            goto EXIT;
        }
    }

    /*
     *  Construct the FAT sectors
     */
    ui8_offset  = ui2_rsv_sec_cnt * ui4_sec_size;

    pui1_fat = x_mem_alloc(ui4_sec_size*1024);
    if(pui1_fat == NULL)
    {
        /* Allow the allocatation is NULL */
    }

    for (i = 0; i < ui1_num_fat; i++)
    {
        x_memset(pui1_bs, 0, ui4_sec_size);

        pui1_bs[0] = 0xF8;
        if (e_fat_type == FAT_TYPE_12)
        {
            pui1_bs[1] = pui1_bs[2] = 0xFF;
        }
        else if (e_fat_type == FAT_TYPE_16)
        {
            pui1_bs[1] = pui1_bs[2] = pui1_bs[3] = 0xFF;
        }
        else
        {
            pui1_bs[1] = pui1_bs[2] = 0xFF;
            pui1_bs[3] = 0x0f;
            FAT_SET_VAL32(pui1_bs, 4, 0xFFFFFFFF);
            pui1_bs[8] = pui1_bs[9] = pui1_bs[10] = 0xFF;
            pui1_bs[11] = 0x0f;
        }
        ui4_done = pwrite(pt_dev->h_driver, pui1_bs, ui4_sec_size, ui8_offset);
        if (ui4_done != ui4_sec_size)
        {
            goto EXIT;
        }
        
        ui8_offset += ui4_done;

        if (pui1_fat == NULL)
        {
            x_memset(pui1_bs, 0, ui4_sec_size);

            for (j = 1; j < ui4_fat_size; j++)
            {
                ui4_done = pwrite(pt_dev->h_driver, pui1_bs, ui4_sec_size, ui8_offset);
                if (ui4_done != ui4_sec_size)
                {
                    goto EXIT;
                }
            
                ui8_offset += ui4_done;
            }
        }
        else
        {
            /* Bulk write */
            UINT32  ui4_n;

            x_memset(pui1_fat,0,ui4_sec_size*1024);

            for (j = 1; j < ui4_fat_size; j+=1024)
            {
                if(ui4_fat_size - j >= 1024)
                {
                    ui4_n = 1024;
                }
                else
                {
                    ui4_n = ui4_fat_size - j; 
                }
                
                ui4_done = pwrite(pt_dev->h_driver, pui1_fat, ui4_n * ui4_sec_size, ui8_offset);
                if (ui4_done != ui4_n * ui4_sec_size)
                {
                    goto EXIT;
                }

                ui8_offset += ui4_done;
            }
        }
    }

    /*
     *  Construct the root sectors
     */
    x_memset(pui1_bs, 0, ui4_sec_size);
    for (i = 0; i < ui4_root_dir_sec; i++)
    {
        ui4_done = pwrite(pt_dev->h_driver, pui1_bs, ui4_sec_size, ui8_offset);
        if (ui4_done != ui4_sec_size)
        {
            goto EXIT;
        }
        
        ui8_offset += ui4_done;
    }

EXIT:

    if (pui1_bs != NULL)
    {
        x_mem_free(pui1_bs);
    }

    if (pui1_fat != NULL)
    {
        x_mem_free(pui1_fat);
    }

    return i4_ret;
}


static INT32 linux_check_disk(
        char *ps_bin_path,
        char *ps_dev_name,
        char *ps_option,
        char *ps_direct_write)
{
    char *fsck_cmd_list[] = {
        ps_bin_path,
        ps_dev_name,
        ps_option,
        ps_direct_write,
        NULL
    };
    const int fsck_time_out = 15;

    if (!file_exist_at(ps_bin_path))
    {
        DBG_ERROR(("Error: %s not found, skip checking.", ps_bin_path));
        return 0;
    }
    
    x_dbg_stmt("Start checking disk by %s...\n", ps_bin_path);

    /* May be it should log to /val/log */
    return fm_spawn_with_time_out(fsck_cmd_list, "/tmp/check_disk_log", fsck_time_out, TRUE);
}

static BOOL linux_mount_ok(const char *ps_dev_name)
{
    CHAR cmd_buf[256];
    FILE* fp_mount_ret;
    int mounted_num;
    x_sprintf(cmd_buf, "mount |grep -c '^%s ' > /tmp/mount_result", ps_dev_name);

    system(cmd_buf);
    fp_mount_ret = fopen("/tmp/mount_result", "r");
    
    if (fp_mount_ret == NULL)
    {
        return FALSE;
    }
    fscanf(fp_mount_ret, "%d", &mounted_num);

    return (mounted_num == 1); 
}

static INT32 linux_mount_ntfs_3g(
        CHAR *ps_dev_name, 
        CHAR *ps_mp_name)
{
    INT32 ret;
    CHAR cmd_buf[256];
    linux_check_disk(NTFS_FIX_PATH, ps_dev_name, NULL, NULL);

    x_sprintf(cmd_buf, "busybox chrt -r 99 %s %s %s",
                    NTFS_3G_PATH,
                    ps_dev_name,
                    ps_mp_name);
    
    if (!file_exist_at("/sys/module/fuse"))
    {
        ret = system("insmod /basic/modules/fuse.ko");
        if (ret != 0)
        {
            DBG_ERROR(("Error:In %s: Can not load fuse.ko.\n", __FUNCTION__));
            return ret;
        }
    }

    INT32 i4_loop = 0;
	while(i4_loop < 3)
	{
	    ret = system(cmd_buf);

		x_dbg_stmt("[%s:%d]return value: %d, error[%d]: %s\n", __FUNCTION__, __LINE__, ret, errno, strerror(errno));
		if(ret == -1 || ret == 127)
		{
			x_dbg_stmt("system occur error\n");
			break;
		}
		else if(ret == 0 && errno == ECHILD)
		{
			x_dbg_stmt("there is no process, wait a second\n");
			sleep(1);
			i4_loop++;
		}
		else
		{
			break;
		}
		
	}
    
    if (linux_mount_ok(ps_dev_name))
    {
        DBG_INFO(("[FM] %s mounted on %s by %s",
            ps_dev_name,
            ps_mp_name,
            NTFS_3G_PATH));
        return FMR_OK;
    }
    else
    {
        return FMR_ERROR;
    }
}


static INT32 linux_mount_mtp(
        CHAR *ps_dev_name, 
        CHAR *ps_mp_name)
{
    INT32 ret;
    CHAR cmd_buf[256];
    CHAR *mtpfs_path = NULL;

    if(file_exist_at("/3rd/bin/mtpfs"))
    {
        mtpfs_path = "/3rd/bin/mtpfs";
    }
    else if(file_exist_at("/basic/mtpfs"))
    {
        mtpfs_path = "/basic/mtpfs";
    }
    else
    {
        return FMR_ACCESS;
    }

    x_sprintf(cmd_buf, "%s %s",
                    mtpfs_path,
                    ps_mp_name);
    
    if (!file_exist_at("/sys/module/fuse"))
    {
        ret = system("insmod /basic/modules/fuse.ko");
        if (ret != 0)
        {
            DBG_ERROR(("Error:In %s: Can not load fuse.ko.\n", __FUNCTION__));
            return ret;
        }
    }

    INT32 i4_loop = 0;
	while(i4_loop < 3)
	{
	    ret = system(cmd_buf);

		x_dbg_stmt("[%s:%d]return value: %d, error[%d]: %s\n", __FUNCTION__, __LINE__, ret, errno, strerror(errno));
		if(ret == -1 || ret == 127)
		{
			x_dbg_stmt("system occur error\n");
			break;
		}
		else if(ret == 0 && errno == ECHILD)
		{
			x_dbg_stmt("there is no process, wait a second\n");
			sleep(1);
			i4_loop++;
		}
		else
		{
			break;
		}
		
	}
    
    if (ret != 0)
    {
        DBG_ERROR(("[FM] Error:In %s: Can not mount mtp.\n", __FUNCTION__));
		x_dbg_stmt("[FM] Error:In %s: Can not mount mtp, return value: %d\n", __FUNCTION__, ret);
    }
    DBG_INFO(("[FM] %s mounted on %s by %s",
			ps_dev_name,
            ps_mp_name,
            mtpfs_path));
    
    return ret;
}

static INT32 linux_mount_exfat(
        CHAR *ps_dev_name, 
        CHAR *ps_mp_name)
{

    INT32 ret;
    CHAR cmd_buf[256];

    linux_check_disk(EXFAT_FSCK_PATH, ps_dev_name, NULL, NULL);

    x_sprintf(cmd_buf, "%s %s %s",
                    EXFAT_MOUNT_PATH,
                    ps_dev_name,
                    ps_mp_name);
    
    if (!file_exist_at("/sys/module/fuse"))
    {
        ret = system("insmod /basic/modules/fuse.ko");
        if (ret != 0)
        {
            DBG_ERROR(("Error:In %s: Can not load fuse.ko.\n", __FUNCTION__));
            return ret;
        }
    }

    INT32 i4_loop = 0;
	while(i4_loop < 3)
	{
	    ret = system(cmd_buf);

		x_dbg_stmt("[%s:%d]return value: %d, error[%d]: %s\n", __FUNCTION__, __LINE__, ret, errno, strerror(errno));
		if(ret == -1 || ret == 127)
		{
			x_dbg_stmt("system occur error\n");
			break;
		}
		else if(ret == 0 && errno == ECHILD)
		{
			x_dbg_stmt("there is no process, wait a second\n");
			sleep(1);
			i4_loop++;
		}
		else
		{
			break;
		}
		
	}
    
    DBG_INFO(("[FM] %s mounted on %s by %s",
            ps_dev_name,
            ps_mp_name,
            EXFAT_MOUNT_PATH));
    
    return ret;
}



static INT32 linux_mount(
    VOID* pv_data)
{
    FM_MOUNT_FS_ARG_T  *pt_mount_arg = NULL;
    FM_XENTRY_T        *pt_dev_xent = NULL;
    FM_MOUNT_T         *pt_mount = NULL;
    CHAR *ps_dev_name = NULL;
    CHAR *ps_mp_name = NULL;
    FM_FS_TYPE_T       e_fs_type = FM_TYPE_INVAL;
    INT32              i4_ret;

    DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_mount_arg = (FM_MOUNT_FS_ARG_T*)pv_data;

    if ((pt_mount_arg->pt_dev_xent == NULL) ||
        (pt_mount_arg->pt_mount == NULL))
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_dev_xent = pt_mount_arg->pt_dev_xent;
    pt_mount    = pt_mount_arg->pt_mount;

    ps_dev_name = linux_generate_absolute_name(
        pt_mount_arg->pt_dev_xent->pt_prnt_xent,
        pt_mount_arg->pt_dev_xent,
        pt_mount_arg->pt_dev_xent->ps_name);
    if (ps_dev_name == NULL)
    {
        return FMR_CORE;
    }
    
    ps_mp_name = linux_generate_absolute_name(
        pt_mount->pt_mp_xent->pt_prnt_xent,
        pt_mount->pt_mp_xent,
        pt_mount->pt_mp_xent->ps_name);
    if (ps_mp_name == NULL)
    {
        x_mem_free(ps_dev_name);
        return FMR_CORE;
    }
    /* Linux mount */
    if (pt_mount_arg->pt_mnt_parm->b_attach_mounted == FALSE)
    {
        int  errno1 = 0;
        int  errno2 = 0;
        int  errno3 = 0;
        BOOL b_remounting = FALSE;
        unsigned long mount_times = 0;
        //unsigned long mount_flags = 0 | MS_NOATIME |MS_RDONLY;
        unsigned long mount_flags = 0 | MS_NOATIME;
        
        x_dbg_stmt("[FM-Linux]Try to mount %s to %s...\n",ps_dev_name, ps_mp_name);

        do
        {
            if(strncmp(ps_mp_name, "/mnt/usb/usbdev", strlen("/mnt/usb/usbdev")) == 0)
            {
                i4_ret = linux_mount_mtp(ps_dev_name, ps_mp_name);
                break;
            }
            /* TODO: We can get file system type by fdisk -l. 
             * Detect file system type first. 
             */
            linux_check_disk(FSCK_FAT_PATH, ps_dev_name, "-a", "-w");
            
            DBG_INFO(("[FM-Linux]Try to mount %s to %s(FS: vfat, Flags:0X%X)...\n",ps_dev_name, ps_mp_name, mount_flags));
            i4_ret = mount(
                        ps_dev_name,
                        ps_mp_name,
                        "vfat",
                        mount_flags,
                        "iocharset=utf8,shortname=mixed,errors=continue");
                        
            e_fs_type = FM_TYPE_FAT;
            errno1 = errno;
            DBG_INFO(("errno=%d\n", errno1));
            
            if (i4_ret != 0 && errno1 != EBUSY && errno1 != EACCES && errno1 != EROFS)
            {
                DBG_INFO(("[FM-Linux]Mount %s to %s failed with vfat as %s...\n",ps_dev_name, ps_mp_name, strerror(errno)));
                DBG_INFO(("[FM-Linux]Try to mount %s to %s(FS: ntfs, Flags:0X%X)...\n",ps_dev_name, ps_mp_name, mount_flags));


                if (file_exist_at(NTFS_3G_PATH) 
                    && file_exist_at("/basic/modules/fuse.ko"))
                {
                    i4_ret = linux_mount_ntfs_3g(ps_dev_name, ps_mp_name);
                }
                else
                {
                    i4_ret = mount(
                                ps_dev_name,
                                ps_mp_name,
                                "ntfs",
                                mount_flags,
                                "nls=utf8");
                }
                
                e_fs_type = FM_TYPE_NTFS;
                errno2 = errno;
                
                DBG_INFO(("errno=%d\n", errno2));
            }

            /*try to mount file system as exfat*/
            if (i4_ret != 0 && errno2 != EBUSY && errno2 != EACCES && errno2 != EROFS)
            {
                DBG_INFO(("[FM-Linux]Try to mount %s to %s(FS: exfat, Flags:0X%X)...\n",ps_dev_name, ps_mp_name, mount_flags));

                if (file_exist_at(EXFAT_MOUNT_PATH) 
                    && file_exist_at("/basic/modules/fuse.ko"))
                {
                    i4_ret = linux_mount_exfat(ps_dev_name, ps_mp_name);
                }
                e_fs_type = FM_TYPE_FAT64;
                errno3 = errno;
                
                DBG_INFO(("errno=%d\n", errno3));
            }

            mount_times++;

            if (i4_ret == 0)
            {
                break;
            }
            else if (errno1 == EBUSY || errno2 == EBUSY || errno3 == EBUSY || b_remounting == TRUE)
            {
                struct timespec req;
                req.tv_sec = 0;
                req.tv_nsec = 100000000;
                
                DBG_INFO(("[FM-Linux]Mount %s to %s failed as %s, sleep a while, current system tick:%lu\n",ps_dev_name, ps_mp_name, strerror(errno), x_os_get_sys_tick()));
                
                nanosleep(&req, NULL);
                
                DBG_INFO(("[FM-Linux]Wake up, try to remount %s to %s again...current system tick:%lu\n",ps_dev_name, ps_mp_name, x_os_get_sys_tick()));

                mount_flags |= MS_REMOUNT;
                b_remounting = TRUE;
            }
            else if (errno1 == EACCES || errno2 == EACCES || errno1 == EROFS || errno2 == EROFS)
            {
                mount_flags |= MS_RDONLY;
            }
            else
            {
                break;
            }
        }while(i4_ret != 0 && mount_times < 2);
        /* Failed */
        if (i4_ret != 0)
        {
            x_dbg_stmt("[FM-Linux]Mount %s to %s failed as %s\n",ps_dev_name, ps_mp_name, strerror(errno));

            switch (errno)
            {
                case EINVAL:
                    i4_ret = FMR_INVAL;
                    break;
                case ENOENT:
                    i4_ret = FMR_NO_ENTRY;
                    break;
                case EBUSY:
                    i4_ret = FMR_BUSY;
                    break;
                case ENOTDIR:
                    i4_ret = FMR_NOT_DIR;
                    break;
                case ENAMETOOLONG:
                    i4_ret = FMR_NAME_TOO_LONG;
                    break;
                default:
                    i4_ret = FMR_CORE;
                    break;
            }
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
            if (ps_dev_name != NULL)
            {
                x_mem_free(ps_dev_name);
                ps_dev_name = NULL;
            }
            if (ps_mp_name != NULL)
            {
                x_mem_free(ps_mp_name);
                ps_mp_name = NULL;
            }
            
            return i4_ret;
        }
    }
    /*else*/
    {
        struct statfs* pt_fs_info;
        LINUX_DENTRY_T* pt_ldentry;

        x_dbg_stmt("[FM-Linux]Mount %s to %s successfully.\n",ps_dev_name, ps_mp_name);

        pt_fs_info = x_mem_alloc(sizeof(struct statfs));
        if (pt_fs_info == NULL)
        { 
            umount2(ps_mp_name, MNT_FORCE);
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        pt_ldentry = x_mem_alloc(sizeof(LINUX_DENTRY_T));
        if (pt_ldentry == NULL)
        {
            umount2(ps_mp_name, MNT_FORCE);
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            x_mem_free(pt_fs_info);
            i4_ret = FMR_CORE;
            goto FAIL;
        }

        /* Successful, get file system info */
        
        DBG_INFO(("Get file system info!\n"));
        i4_ret = statfs(ps_mp_name, pt_fs_info);
        if (i4_ret < 0)
        {
            umount2(ps_mp_name, MNT_FORCE);
            DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
            x_mem_free(pt_fs_info);
            x_mem_free(pt_ldentry);
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("Set mount structure!\n"));

        /* Set mount structure  */
        #if 0
        x_dbg_stmt("%s FS info:\n\t"
                   "type of filesystem:0x%x\n\t"
                   "optimal transfer block size:%d\n\t"
                   "total data blocks in file system:%d\n\t"
                   "free blocks in fs:%d\n\t"
                   "free blocks avail to non-superuser:%d\n\t"
                   "total file nodes in file system:%d\n\t"
                   "free file nodes in fs:%d\n\t"
                   "file system id:0x%x\n\t"
                   "maximum length of filenames:%d\n",
                   __FUNCTION__,
                   pt_fs_info->f_type,
                   pt_fs_info->f_bsize,
                   pt_fs_info->f_blocks,
                   pt_fs_info->f_bfree,
                   pt_fs_info->f_bavail,
                   pt_fs_info->f_files,
                   pt_fs_info->f_ffree,
                   pt_fs_info->f_fsid,
                   pt_fs_info->f_namelen);
        #endif
        pt_mount->t_fs_info.e_type = e_fs_type;
        pt_mount->t_fs_info.ui8_blk_size     = pt_fs_info->f_bsize;
        pt_mount->t_fs_info.ui8_total_blk    = pt_fs_info->f_blocks;
        pt_mount->t_fs_info.ui8_free_blk     = pt_fs_info->f_bavail;
        pt_mount->t_fs_info.ui8_avail_blk    = pt_fs_info->f_bavail;
        pt_mount->t_fs_info.ui8_files        = pt_fs_info->f_files;
        pt_mount->t_fs_info.ui2_max_name_len = pt_fs_info->f_namelen;
		
        if(pt_dev_xent->pt_dev == NULL)
        {
            pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_READ_WRITE;
        }
        else
        {
            switch(pt_dev_xent->pt_dev->t_blk.ui4_access_type)
            {
                case 0:
                    pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_RW_DENY;
                    break;
                case 1:
                    pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_READ_WRITE;
                    break;
                case 2:
                    pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_WRITE_ONLY;
                    break;
                case 3:
                    pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_READ_ONLY;
                    break;
                default:
                    pt_mount->t_fs_info.e_rw_attr = FM_FS_RW_ATTR_READ_WRITE;
                    break;
            }
        }

        pt_mount->pv_priv = pt_fs_info;

        DLIST_INIT(&(pt_ldentry->t_fd_list));

        pt_mount->pt_root_xent->pv_fs_node  = pt_ldentry;
        pt_mount->pt_root_xent->pt_ufs_ops  = &t_linux_ufs_ops;
        pt_mount->pt_root_xent->pt_xent_ops = &t_linux_xent_ops;
#if 1
        do
        {
            LINUX_MP_OPENED_FD_T* pt_mpofd = NULL;

            pt_mpofd  = malloc(sizeof(LINUX_MP_OPENED_FD_T));
            if (pt_mpofd != NULL)
            {
                memset(pt_mpofd, 0, sizeof(LINUX_MP_OPENED_FD_T));
               
                if (x_sema_create(&pt_mpofd->h_mtx,
                                  X_SEMA_TYPE_MUTEX,
                                  X_SEMA_STATE_UNLOCK) != OSR_OK)
                {
                    free(pt_mpofd);
                    break;
                }
                
                pt_mpofd->ps_mp_path = strdup(ps_mp_name);
                DLIST_INIT(&(pt_mpofd->t_fd_list));

                DBG_INFO(("[FM-Linux] Add %s to global MP OPENED FD List\n", ps_mp_name));

                DLIST_INSERT_TAIL(pt_mpofd, &gt_mp_open_fd_q, t_link);
            }
        }while(0);
#endif
        /* Mount has done! */

        i4_ret = FMR_OK;
    FAIL:
        if (ps_dev_name != NULL)
        {
            x_mem_free(ps_dev_name);
        }
        if (ps_mp_name != NULL)
        {
            x_mem_free(ps_mp_name);
        }
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
        return i4_ret;
    }
}

static INT32 linux_umount(
    VOID* pv_data)
{
    FM_UMOUNT_FS_ARG_T  *pt_umount_arg = NULL;
    FM_MOUNT_T          *pt_mount = NULL;
    struct statfs       *pt_fs_info = NULL;
    LINUX_DENTRY_T      *pt_ldentry = NULL;
    CHAR                *ps_mp = NULL;
    INT32               i4_ret;
    UINT32              ui4_umount_times;
    DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_umount_arg = (FM_UMOUNT_FS_ARG_T *)pv_data;

    if (pt_umount_arg->pt_mount == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));

        return FMR_ARG;
    }

    pt_mount = pt_umount_arg->pt_mount;

    if (pt_mount->pt_root_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    ps_mp = linux_generate_absolute_name(
        pt_mount->pt_mp_xent->pt_prnt_xent, 
        NULL, 
        pt_mount->pt_mp_xent->ps_name);
    if (ps_mp == NULL)
    {
        return FMR_NO_ENTRY;
    }
#if 1
    {
        LINUX_FD_T* pt_fd = NULL;
        LINUX_MP_OPENED_FD_T* pt_mpofd = NULL;

        DBG_INFO(("[FM-Linux]: before umounting %s, close all the opened fd in this mount\n", ps_mp));

        DLIST_FOR_EACH(pt_mpofd, &gt_mp_open_fd_q, t_link)
        {
            if (strcmp(ps_mp, pt_mpofd->ps_mp_path) == 0)
            {
                break;
            }
        }
        if (pt_mpofd != NULL)
        {
            HANDLE_T h_mtx = pt_mpofd->h_mtx;
            if (x_sema_lock(h_mtx, X_SEMA_OPTION_WAIT) == OSR_OK)
            {
                while(!DLIST_IS_EMPTY(&pt_mpofd->t_fd_list))
                {
                    pt_fd = DLIST_HEAD(&pt_mpofd->t_fd_list);
                    if (pt_fd != NULL)
                    {
                        DBG_INFO(("[FM-Linux]: close %ld in this mount\n", pt_fd->i4_fd));
                        if (pt_fd->b_file == TRUE)
                        {
#if !defined(ANDROID) && !defined(__UCLIBC__)
                            aio_cancel(pt_fd->i4_fd, NULL);
#endif
                            fsync(pt_fd->i4_fd);
                            i4_ret = close(pt_fd->i4_fd);
                            if (i4_ret < 0)
                            {
                                DBG_INFO(("[FM-Linux]close %ld failed as (%s)\n",pt_fd->i4_fd, strerror(errno)));
                            }
                        }
                        else
                        {
                            i4_ret = closedir((DIR *)pt_fd->i4_fd);
                            if (i4_ret < 0)
                            {
                                DBG_INFO(("[FM-Linux]closedir %ld failed as (%s)\n",pt_fd->i4_fd, strerror(errno)));
                            }
                        }
                        
                        DLIST_REMOVE(pt_fd, &pt_mpofd->t_fd_list, t_link);

                        if (x_sema_lock(gh_free_fd_list_sema, X_SEMA_OPTION_WAIT) == OSR_OK)
                        {
                            DLIST_INSERT_HEAD(pt_fd, &gt_free_fd_list, t_link);
                            x_sema_unlock(gh_free_fd_list_sema);
                        }
                        else
                        {
                            free(pt_fd);
                            pt_fd = NULL;
                        }
                    }
                }
                
                DLIST_REMOVE(pt_mpofd, &gt_mp_open_fd_q, t_link);
                free(pt_mpofd->ps_mp_path);
                free(pt_mpofd);
                
                x_sema_delete(h_mtx);
            }
        } 
     }
    #endif

    /* Linux Umount() */
    x_dbg_stmt("[FM-Linux]: Try to umount %s\n", ps_mp);
    ui4_umount_times = 0;
    do
    {
        /*
                For Linux2.0, two processes have mounnted the device, so one process may fail to umount the device as
                another device still holds some file/dir.
                Wait untill another process closes all opened file/dir.
             */
        i4_ret = umount2(ps_mp, MNT_FORCE);
        ui4_umount_times++;
        
        if (i4_ret < 0)
        {
            if (errno == ENOENT || errno == EINVAL)
            {
                /* The device is already umounted in another device */
                DBG_INFO(("[FM-Linux]: umount %s failed as \"%s\", ignore it...\n", ps_mp, strerror(errno)));
                x_dbg_stmt("[FM-Linux]: umount %s successfully...\n", ps_mp);
                x_mem_free(ps_mp);
                ps_mp = NULL;

                break;
            }
            else if (errno == EBUSY)
            {
                /* Another process still holds some file/dir, wait */
                struct timespec req;
                req.tv_sec = 0;
                req.tv_nsec = 100000000;

                DBG_INFO(("[FM-Linux]: umount %s failed as \"%s\", sleep a while and then umount again...\n", ps_mp, strerror(errno)));

                nanosleep(&req, NULL);

                continue;
            }
            else
            {
                x_dbg_stmt("[FM-Linux]: umount %s failed as \"%s\"...\n", ps_mp, strerror(errno));
                
                x_mem_free(ps_mp);
                ps_mp = NULL;
                
                return FMR_CORE;
            }
        }
        else
        {
            x_dbg_stmt("[FM-Linux]: umount %s successfully...\n", ps_mp);
            
            x_mem_free(ps_mp);
            ps_mp = NULL;
            break;
        }
    }while(ui4_umount_times<40);

    if (ps_mp != NULL)
    {
        x_mem_free(ps_mp);
        ps_mp = NULL;
    }
    
    pt_ldentry = LINUX_GET_DENTRY(pt_mount->pt_root_xent);
    if (pt_ldentry == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    /* Free Root node */
    x_mem_free(pt_ldentry);
    pt_mount->pt_root_xent->pv_fs_node = NULL;

    pt_fs_info = (struct statfs*)pt_mount->pv_priv;
    x_mem_free(pt_fs_info);
    pt_mount->pv_priv = NULL;
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));

    /* Umount has done! */
    return FMR_OK;
}

INT32 linux_lookup(
    VOID* pv_data)
{
    FM_LOOKUP_ARG_T *pt_lookup_arg = NULL;
    FM_XENTRY_T     *pt_xent = NULL;
    CHAR            *ps_absolute_name = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;
    struct stat buf;
    INT32           i4_ret;
    /*DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_lookup_arg = (FM_LOOKUP_ARG_T *)pv_data;

    if(pt_lookup_arg->pt_prnt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    if(pt_lookup_arg->ps_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* Get the absolute path name */
    ps_absolute_name = linux_generate_absolute_name(
        pt_lookup_arg->pt_prnt_xent,
        pt_lookup_arg->pt_xent,
        pt_lookup_arg->ps_name);

    /*DBG_INFO(("Get the absolute path name:%s\n",ps_absolute_name));*/

    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*DBG_INFO(("access:%s\n",ps_absolute_name));*/

    /* Check the existence first*/
    i4_ret = access(ps_absolute_name, F_OK);
    if (i4_ret < 0)
    {
        i4_ret = FMR_NO_ENTRY;
        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
        goto FAIL;
    }
    /*DBG_INFO(("stat:%s\n",ps_absolute_name));*/

    /* Get the attr */
    i4_ret = stat(ps_absolute_name, &buf);
    if (i4_ret < 0)
    {
        i4_ret = FMR_CORE;
        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
        goto FAIL;
    }
    /*DBG_INFO(("fm_get_new_xentry:%s\n",pt_lookup_arg->ps_name));*/

    /* Acquire a new xentry and fill it */
    pt_xent = fm_get_new_xentry(pt_lookup_arg->ps_name);
    if (pt_xent == NULL)
    {
        i4_ret = FMR_CORE;
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto FAIL;
    }
    /*DBG_INFO(("Allocate dentry\n");*/
    pt_ldentry = x_mem_alloc(sizeof(LINUX_DENTRY_T));
    if (pt_ldentry == NULL)
    {
        i4_ret = FMR_CORE;
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto FAIL;
    }

    /*DBG_INFO(("Init fd list\n"));*/
    DLIST_INIT(&(pt_ldentry->t_fd_list));
    pt_ldentry->ui8_size = buf.st_size;
    /*DBG_INFO(("Setup1 pt_xent\n"));*/

    pt_xent->ui4_mode = 0;

    if (S_ISDIR(buf.st_mode))
    {
        pt_xent->ui4_mode |= FM_MODE_TYPE_DIR;
    }
    else if (S_ISREG(buf.st_mode))
    {
        pt_xent->ui4_mode |= FM_MODE_TYPE_FILE;
    }
    pt_xent->ui4_mode |= (buf.st_mode & 0777);

    /*DBG_INFO(("Setup2 pt_xent\n"));*/

    if (pt_lookup_arg->pt_prnt_xent != NULL)
    {
        /*DBG_INFO(("Setup3.1 pt_xent:%ld,pt_prnt_xent:%ld\n",pt_xent,pt_lookup_arg->pt_prnt_xent));*/
        pt_xent->pt_mount     = pt_lookup_arg->pt_prnt_xent->pt_mount;
        pt_xent->pt_ufs_ops   = pt_lookup_arg->pt_prnt_xent->pt_ufs_ops;
        pt_xent->pv_fs_node   = pt_ldentry;
        pt_xent->pt_prnt_xent = pt_lookup_arg->pt_prnt_xent;
        pt_xent->pt_xent_ops  = pt_lookup_arg->pt_prnt_xent->pt_xent_ops;
        if(pt_lookup_arg->pt_prnt_xent->pt_mount != NULL &&
           pt_lookup_arg->pt_prnt_xent->pt_mount->pt_dev_xent != NULL)
        { 
            
            /*DBG_INFO(("Setup3.1 pt_lookup_arg->pt_prnt_xent->pt_mount:%ld\n",pt_lookup_arg->pt_prnt_xent->pt_mount));*/
            pt_xent->pt_dev       = pt_lookup_arg->pt_prnt_xent->pt_mount->pt_dev_xent->pt_dev;
        }
        else
        {
            pt_xent->pt_dev = NULL;
        }
    }
    else
    {
        /*DBG_INFO(("Setup3.2 pt_xent\n"));*/
        pt_xent->pt_mount     = NULL;
        pt_xent->pt_ufs_ops   = &t_linux_ufs_ops;
        pt_xent->pv_fs_node   = pt_ldentry;
        pt_xent->pt_prnt_xent = NULL;
        pt_xent->pt_xent_ops  = &t_linux_xent_ops;
        pt_xent->pt_dev       = NULL;
    }
    /*DBG_INFO(("Setup3.3 pt_xent\n"));*/
    pt_lookup_arg->pt_xent = pt_xent;

    /*DBG_INFO(("Setup end pt_xent\n"));*/

    /* Don't forget to free the ps_absolute_name */
    if (NULL != ps_absolute_name)
    {
        x_mem_free(ps_absolute_name);
    }

    /* Lookup has done! */
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
    
    return FMR_OK;

FAIL:
    x_mem_free(ps_absolute_name);
    return i4_ret;;
}

INT32 linux_read(
    VOID* pv_data)
{
    LINUX_DENTRY_T  *pt_ldentry = NULL;    
    LINUX_FD_INFO_T *pt_fd_info = NULL;
    FM_RW_ARG_T     *pt_arg = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    /*DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    /* read */
    pt_ldentry = (LINUX_DENTRY_T *)pt_arg->pt_xent->pv_fs_node;
    pt_fd_info = _linux_find_fd_info_by_fd(pt_ldentry, i4_fd);
    if (pt_fd_info == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    
    i4_ret = read(i4_fd, pt_arg->pv_data, pt_arg->ui4_count);
    
    if (i4_ret == -1)
    {
        switch (errno)
        {
            case EFAULT:
                
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
                return FMR_EOF;
            case EIO:
                
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_DEVICE_ERROR));
                return FMR_DEVICE_ERROR;
            default:
                
                DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                return FMR_CORE;
        }
    }
    else if (i4_ret == EOVERFLOW)
    {
        pt_arg->ui4_done_bytes = i4_ret;
        x_dbg_stmt("Read over valid range. Read count = %d", i4_ret);
        return FMR_OK;
    }
    else
    {
        pt_arg->ui4_done_bytes = i4_ret;

        if (i4_ret == 0)
        {
            
            /*DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));*/
            return FMR_EOF;
        }
        else
        {
            
            /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
            return FMR_OK;
        }
    }
}

#if !defined(ANDROID) && !defined(__UCLIBC__)
INT32 linux_aread(
    VOID* pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    FM_RW_ARG_T     *pt_new_arg = NULL;
    struct aiocb    *pt_aiocb = NULL;
    AIO_CB_TAG_T    *pt_aiocb_tag = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    pt_ldentry = (LINUX_DENTRY_T *)pt_arg->pt_xent->pv_fs_node;
    DBG_INFO(("[FM] %s: line:%d,ofst:%llu, size:%llu\n",__FUNCTION__,__LINE__,pt_arg->ui8_offset,pt_ldentry->ui8_size));
    if(pt_arg->ui8_offset >= pt_ldentry->ui8_size)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
        return FMR_EOF;
    }

    pt_aiocb = (struct aiocb *)x_mem_alloc(sizeof(struct aiocb));
    if (pt_aiocb == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memset(pt_aiocb, 0, sizeof(struct aiocb));

    pt_new_arg = (FM_RW_ARG_T *)x_mem_alloc(sizeof(FM_RW_ARG_T));
    if (pt_new_arg == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_mem_free(pt_aiocb);
        return FMR_CORE;
    }
    x_memcpy(pt_new_arg, pt_arg, sizeof(FM_RW_ARG_T));

    pt_aiocb_tag = (AIO_CB_TAG_T *)x_mem_alloc(sizeof(AIO_CB_TAG_T));
    if (pt_aiocb_tag == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        return FMR_CORE;
    }

    /* Setup aiocb structure */
    pt_aiocb->aio_fildes = i4_fd;
    pt_aiocb->aio_buf = pt_arg->pv_data;
    pt_aiocb->aio_nbytes = pt_arg->ui4_count;
    pt_aiocb->aio_reqprio = 0;
    pt_aiocb->aio_offset = /*lseek(i4_fd, 0, SEEK_CUR);*/pt_arg->ui8_offset;
    pt_aiocb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    pt_aiocb->aio_sigevent.sigev_notify_function = _linux_aio_callback;
    pt_aiocb->aio_sigevent.sigev_notify_attributes = NULL;
    pt_aiocb->aio_sigevent.sigev_value.sival_ptr = pt_aiocb_tag;

    pt_aiocb_tag->b_read = TRUE;
    pt_aiocb_tag->pt_rw_arg = pt_new_arg;
    pt_aiocb_tag->pt_aiocb = pt_aiocb;

    i4_ret = aio_read(pt_aiocb);
    if (i4_ret != 0)
    {
        DBG_ERROR(("aio read:paio_cb:%ld init fail %s\n",(UINT32)pt_aiocb, strerror(errno)));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        x_mem_free(pt_aiocb_tag);
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*lseek(i4_fd, pt_arg->ui4_count, SEEK_CUR);*/
    DBG_INFO(("send aio read:paio_cb:%ld\n",(UINT32)pt_aiocb));
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
    return FMR_OK;

}
#endif

INT32 linux_write(VOID *pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    pt_ldentry = (LINUX_DENTRY_T *)pt_arg->pt_xent->pv_fs_node;

    /* quick return if write 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        
        return FMR_CORE;
    }

    /*if file is read-only, return*/
    if((pt_ldentry->ui4_mode & S_IWUSR) == 0)
    {
        DBG_ERROR(("[FM] %s: line: %d, %s is read only\n", __FUNCTION__, __LINE__, pt_arg->pt_xent->ps_name));
        return FMR_ACCESS;
    }

    /* write */
    i4_ret = write(i4_fd, pt_arg->pv_data, pt_arg->ui4_count);
    if (i4_ret == -1)
    {
        switch (errno)
        {
            case EFAULT:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
                return FMR_EOF;
            case EIO:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_DEVICE_ERROR));
                return FMR_DEVICE_ERROR;
            case ENOSPC:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_NOT_ENOUGH_SPACE));
                return FMR_NOT_ENOUGH_SPACE;   
            default:
                DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                return FMR_CORE;
        }
    }
    else
    {
        pt_arg->ui4_done_bytes = i4_ret;
        if(pt_arg->ui8_offset + pt_arg->ui4_done_bytes > pt_ldentry->ui8_size)
        {
            pt_ldentry->ui8_size = pt_arg->ui8_offset + pt_arg->ui4_done_bytes;
        }
        if (i4_ret == 0)
        {
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
            return FMR_EOF;
        }
        else
        {
            
            DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
            return FMR_OK;
        }
    }
}

#if !defined(ANDROID) && !defined(__UCLIBC__)
INT32 linux_awrite(VOID* pv_data)
{
    FM_RW_ARG_T     *pt_arg = NULL;
    FM_RW_ARG_T     *pt_new_arg = NULL;
    struct aiocb    *pt_aiocb = NULL;
    AIO_CB_TAG_T    *pt_aiocb_tag = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;    
    LINUX_FD_INFO_T *pt_fd_info = NULL;
    INT32           i4_fd;
    INT32           i4_ret = 0;
    DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    /* Get corresponding inode */
    pt_arg = (FM_RW_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* quick return if read 0 bytes */
    if (pt_arg->ui4_count == 0)
    {
        pt_arg->ui4_done_bytes = 0;
        
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }    
    pt_ldentry = (LINUX_DENTRY_T *)pt_arg->pt_xent->pv_fs_node;
    /*
    if(pt_arg->ui8_offset >= pt_ldentry->ui8_size)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_EOF));
        return FMR_EOF;
    }
    */
    
    pt_fd_info = _linux_find_fd_info_by_fd(pt_ldentry, i4_fd);
    if (pt_fd_info == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    pt_aiocb = (struct aiocb *)x_mem_alloc(sizeof(struct aiocb));
    if (pt_aiocb == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memset(pt_aiocb, 0, sizeof(struct aiocb));

    pt_new_arg = (FM_RW_ARG_T *)x_mem_alloc(sizeof(FM_RW_ARG_T));
    if (pt_new_arg == NULL)
    {
        x_mem_free(pt_aiocb);
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    x_memcpy(pt_new_arg, pt_arg, sizeof(FM_RW_ARG_T));

    pt_aiocb_tag = (AIO_CB_TAG_T *)x_mem_alloc(sizeof(AIO_CB_TAG_T));
    if (pt_aiocb_tag == NULL)
    {
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    /* Setup aiocb structure */
    pt_aiocb->aio_fildes = i4_fd;
    pt_aiocb->aio_buf = pt_arg->pv_data;
    pt_aiocb->aio_nbytes = pt_arg->ui4_count;
    pt_aiocb->aio_reqprio = 0;
    pt_aiocb->aio_offset = /*lseek(i4_fd, 0, SEEK_CUR);*/pt_arg->ui8_offset;
    pt_aiocb->aio_sigevent.sigev_notify = SIGEV_THREAD;
    pt_aiocb->aio_sigevent.sigev_notify_function = _linux_aio_callback;
    pt_aiocb->aio_sigevent.sigev_notify_attributes = NULL;
    pt_aiocb->aio_sigevent.sigev_value.sival_ptr = pt_aiocb_tag;

    pt_aiocb_tag->b_read = FALSE;
    pt_aiocb_tag->pt_rw_arg = pt_new_arg;
    pt_aiocb_tag->pt_aiocb = pt_aiocb;

    pt_fd_info->ui8_aio_write_req_sent ++;

    i4_ret = aio_write(pt_aiocb);
    if (i4_ret != 0)
    {
        pt_fd_info->ui8_aio_write_req_sent --;
        DBG_ERROR(("aio write:paio_cb:%ld init fail %s\n",(UINT32)pt_aiocb, strerror(errno)));
        x_mem_free(pt_aiocb);
        x_mem_free(pt_new_arg);
        x_mem_free(pt_aiocb_tag);
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*lseek(i4_fd, pt_arg->ui4_count, SEEK_CUR);*/
    DBG_INFO(("send aio write:paio_cb:%ld\n",(UINT32)pt_aiocb));
    
    if(pt_arg->ui8_offset + pt_arg->ui4_count > pt_ldentry->ui8_size)
    {
        pt_ldentry->ui8_size = pt_arg->ui8_offset + pt_arg->ui4_count;
    }

    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
    return FMR_OK;

}
#endif


INT32 linux_lseek(VOID *pv_data)
{
    FM_LSEEK_ARG_T  *pt_arg;
    INT32           i4_fd;
    int             whence;
    INT32           i4_ret;
    off_t           cur_pos;
    /*DBG_INFO(("[FM] %s: enter %d \n", __FUNCTION__, pv_data));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    /*DBG_INFO(("[FM] %s: line:%d\n",__FUNCTION__,__LINE__);*/
    pt_arg = (FM_LSEEK_ARG_T *)pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    /*DBG_INFO(("[FM] %s: line:%d,pt_arg->pt_xent=%d, pt_arg->h_file=%d\n",__FUNCTION__,__LINE__,pt_arg->pt_xent,pt_arg->h_file);*/
    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    /*DBG_INFO(("[FM] %s: line:%d,pt_arg->ui1_whence=%d\n",__FUNCTION__,__LINE__,pt_arg->ui1_whence));*/
    switch (pt_arg->ui1_whence)
    {
        case FM_SEEK_BGN:
            whence = SEEK_SET;
            break;

        case FM_SEEK_CUR:
            whence = SEEK_CUR;
            break;

        case FM_SEEK_END:
            whence = SEEK_END;
            break;

        default:
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
            return FMR_INVAL;
    }
    /*DBG_INFO(("[FM] %s: line:%d,lseek:pt_arg->i8_offset=%d\n",__FUNCTION__,__LINE__,pt_arg->i8_offset));*/
    cur_pos = lseek(i4_fd, (off_t)pt_arg->i8_offset, whence);
    if (cur_pos == (off_t)(-1))
    {
        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
        return FMR_CORE;
    }
    /*DBG_INFO(("CUR OFST:%d \n",i4_ret));*/

    pt_arg->ui8_cur_pos = (UINT64)cur_pos;
    
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
    return FMR_OK;
}

INT32 linux_seek_dir(VOID *pv_data)
{
    FM_SEEK_DIR_ARG_T   *pt_arg = NULL;
    INT32   i4_fd;
    INT32   i4_ret;
    int base = 0;
    int i = 0;
    struct dirent *dirent;
    /*DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    pt_arg = (FM_SEEK_DIR_ARG_T*) pv_data;
    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    if (!IS_XENTRY_DIR(pt_arg->pt_xent))
    {
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_NOT_DIR));
        return FMR_NOT_DIR;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_dir, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    if (pt_arg->ui1_whence == FM_SEEK_BGN)
    {
        base = pt_arg->i4_ent_off;
        pt_arg->ui4_cur_entry_pos = 0;
        rewinddir((DIR *)i4_fd);
    }
    else if (pt_arg->ui1_whence == FM_SEEK_CUR)
    {
        base = pt_arg->i4_ent_off;
    }
    else
    {
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    while(i++ < base)
    {
        dirent = readdir((DIR *)i4_fd);
        if(NULL == dirent)
        {
            break;
        }
        pt_arg->ui4_cur_entry_pos++;
        pt_arg->ui8_cur_byte_pos = dirent->d_off;
    }
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
    return FMR_OK;
}

INT32 linux_seek_dir_ex(VOID *pv_data)
{
    FM_SEEK_DIR_EX_ARG_T   *pt_arg;
    DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));

    ASSERT(pv_data != NULL);
    pt_arg = (FM_SEEK_DIR_EX_ARG_T *) pv_data;

    ASSERT(pt_arg->pui8_ofst != NULL);
    *(pt_arg->pui8_ofst) = pt_arg->ui8_ofst;
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));

    return FMR_OK;
}

INT32 linux_read_dir(VOID *pv_data)
{
    FM_READ_DIR_ARG_T   *pt_arg = NULL;
    FM_XENTRY_T         *pt_xent = NULL;
    FM_DIR_ENTRY_T      *pt_dir_ent = NULL;
    struct dirent *dirent;
    struct stat buf;
    CHAR     *ps_absolute_name = NULL;
    INT32    i4_fd;
    UINT32   ui4_to_read_cnt;
    INT32    i4_ret = FMR_OK;
    int i;
    /*DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    pt_arg = (FM_READ_DIR_ARG_T *)pv_data;

    if (pt_arg->pt_xent == NULL || pt_arg->pt_dir_ent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    if (!IS_XENTRY_LOCKED(pt_arg->pt_xent))
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }

    /* Directory xentry */
    pt_xent = pt_arg->pt_xent;
    /* Reference to an array of FM_DIR_ENTRY_T objects. */
    pt_dir_ent = pt_arg->pt_dir_ent;

    if (!IS_XENTRY_DIR(pt_xent))
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_NOT_DIR));
        return FMR_NOT_DIR;
    }

    i4_ret = _linux_find_fd_by_fh(pt_arg->pt_xent, pt_arg->h_dir, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    /* to read 0 entry, just return OK. */
    ui4_to_read_cnt = pt_arg->ui4_ent_cnt;
    if (ui4_to_read_cnt == 0)
    {
        pt_arg->ui4_read_cnt = 0;
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    pt_arg->ui4_read_cnt = 0;
    for (i=0;i<ui4_to_read_cnt;i++)
    {
        dirent = readdir((DIR *)i4_fd);
        if(NULL == dirent)
        {
            DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
            return FMR_EOF;
        }
        pt_dir_ent[i].ui1_len = dirent->d_reclen;
        pt_dir_ent[i].ui8_dirent_ofst = dirent->d_off;
        x_strcpy(pt_dir_ent[i].s_name, dirent->d_name);

        pt_dir_ent[i].ui4_dirent_len =
            pt_dir_ent[i].ui8_dirent_ofst - pt_arg->ui8_byte_offset;
        pt_arg->ui4_entry_offset ++;
        pt_arg->ui8_byte_offset = pt_dir_ent[i].ui8_dirent_ofst;

        /* We need the absolute path name here */
        ps_absolute_name = linux_generate_absolute_name(pt_xent, NULL, dirent->d_name);
        if (ps_absolute_name == NULL)
        {
            DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
            return FMR_CORE;
        }
        DBG_INFO(("[FM] %s: line:%d,dirent->dame:%s \n", __FUNCTION__,__LINE__, ps_absolute_name));
        i4_ret = stat(ps_absolute_name, &buf);
        if (i4_ret < 0)
        {
            DBG_ERROR(("[FM] %s: line:%d, return %d, %s\n", __FUNCTION__,__LINE__, FMR_CORE, strerror(errno)));
            x_mem_free(ps_absolute_name);
            #if 0
            return FMR_CORE;
            #else
            i--;
            pt_arg->ui4_entry_offset --;
            continue;
            #endif
        }
        x_mem_free(ps_absolute_name);
        ps_absolute_name = NULL;

        pt_dir_ent[i].t_file_info.ui4_mode = 0;
        if (S_ISDIR(buf.st_mode))
        {
            pt_dir_ent[i].t_file_info.ui4_mode |= FM_MODE_TYPE_DIR;
        }
        else if (S_ISREG(buf.st_mode))
        {
            pt_dir_ent[i].t_file_info.ui4_mode |= FM_MODE_TYPE_FILE;
        }
        pt_dir_ent[i].t_file_info.ui4_mode |= (buf.st_mode & 0777);

        pt_dir_ent[i].t_file_info.ui4_inode    = buf.st_ino;/* unused */
        pt_dir_ent[i].t_file_info.ui4_uid      = buf.st_uid;
        pt_dir_ent[i].t_file_info.ui4_gid      = buf.st_gid;
        pt_dir_ent[i].t_file_info.ui8_size     = buf.st_size;
        pt_dir_ent[i].t_file_info.ui8_offset   = 0;
        pt_dir_ent[i].t_file_info.ui4_blk_size = buf.st_blksize;
        pt_dir_ent[i].t_file_info.ui8_blk_cnt  = buf.st_blocks;
        pt_dir_ent[i].t_file_info.ui8_create_time = buf.st_ctime;
        pt_dir_ent[i].t_file_info.ui8_access_time = buf.st_atime;
        pt_dir_ent[i].t_file_info.ui8_modify_time = buf.st_mtime;
        pt_dir_ent[i].t_file_info.ui4_start_lba = 0;/* unused */
        pt_arg->ui4_read_cnt++;
    }
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
    return FMR_OK;
}

INT32 linux_get_info(VOID *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg = NULL;
    CHAR* ps_absolute_name;
    struct stat buf;
    INT32  i4_ret;
    /*DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    pt_arg = (FM_FILE_INFO_ARG_T  *)pv_data;

    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    /*DBG_INFO(("[FM] %s: get path-name,pt_arg->pt_prnt_xent:%d,pt_arg->pt_xent->ps_name:%s \n", __FUNCTION__,pt_arg->pt_prnt_xent,pt_arg->pt_xent->ps_name));*/
    ps_absolute_name = linux_generate_absolute_name(
        pt_arg->pt_prnt_xent,
        pt_arg->pt_xent,
        pt_arg->pt_xent->ps_name);
    /*DBG_INFO(("[FM] %s: path-name:%s\n", __FUNCTION__,ps_absolute_name));*/

    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    else
    {
        /*DBG_INFO(("[FM] %s: stat(%s) \n", __FUNCTION__,ps_absolute_name));*/
        i4_ret = stat(ps_absolute_name, &buf);
        if (i4_ret < 0)
        {
            x_mem_free(ps_absolute_name);
            DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
            return FMR_CORE;
        }
        /*DBG_INFO(("[FM] %s: stat(%s) got\n", __FUNCTION__,ps_absolute_name));*/

        pt_arg->pt_info->ui4_mode = 0;
        if (S_ISDIR(buf.st_mode))
        {
            pt_arg->pt_info->ui4_mode |= FM_MODE_TYPE_DIR;
        }
        else if (S_ISREG(buf.st_mode))
        {
            pt_arg->pt_info->ui4_mode |= FM_MODE_TYPE_FILE;
        }
        pt_arg->pt_info->ui4_mode |= (buf.st_mode & 0777);

        pt_arg->pt_info->ui4_inode    = buf.st_ino;/* unused */
        pt_arg->pt_info->ui4_uid      = buf.st_uid;
        pt_arg->pt_info->ui4_gid      = buf.st_gid;
        pt_arg->pt_info->ui8_size     = buf.st_size;
        pt_arg->pt_info->ui8_offset   = 0;
        pt_arg->pt_info->ui4_blk_size = buf.st_blksize;
        pt_arg->pt_info->ui8_blk_cnt  = buf.st_blocks;
        pt_arg->pt_info->ui8_create_time = buf.st_ctime;
        pt_arg->pt_info->ui8_access_time = buf.st_atime;
        pt_arg->pt_info->ui8_modify_time = buf.st_mtime;
        pt_arg->pt_info->ui4_start_lba = 0;/* unused */
    #if 0
        x_dbg_stmt("create time: %lld\n", pt_arg->pt_info->ui8_create_time);
        x_dbg_stmt("access time: %lld\n", pt_arg->pt_info->ui8_access_time);
        x_dbg_stmt("modify time: %lld\n", pt_arg->pt_info->ui8_modify_time);
    #endif

        x_mem_free(ps_absolute_name);
    }
    /*DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));*/
    return FMR_OK;
}

INT32 linux_set_info(VOID *pv_data)
{
    FM_FILE_INFO_ARG_T  *pt_arg = NULL;
    CHAR* ps_absolute_name = NULL;
    LINUX_DENTRY_T  *pt_ldentry = NULL;
    INT32  i4_ret;
    /*DBG_INFO(("[FM] %s: enter \n", __FUNCTION__));*/

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    pt_arg = (FM_FILE_INFO_ARG_T  *)pv_data;

    if (pt_arg->pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    /*DBG_INFO(("[FM] %s: get path-name,pt_arg->pt_prnt_xent:%d,pt_arg->pt_xent->ps_name:%s \n", __FUNCTION__,pt_arg->pt_prnt_xent,pt_arg->pt_xent->ps_name));*/
    ps_absolute_name = linux_generate_absolute_name(
        pt_arg->pt_prnt_xent,
        pt_arg->pt_xent,
        pt_arg->pt_xent->ps_name);
    /*DBG_INFO(("[FM] %s: path-name:%s\n", __FUNCTION__,ps_absolute_name));*/

    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    else
    {
        /* handle truncate / extend */
        if (pt_arg->pt_info->ui8_size != FM_SET_SIZE_NOVAL)
        {
            struct stat buf;

            i4_ret = stat(ps_absolute_name, &buf);
            if (i4_ret < 0)
            {
                x_mem_free(ps_absolute_name);
                DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                return FMR_CORE;
            }
            
            if (S_ISREG(buf.st_mode))
            {
                #if 1
                /* It cost too much time if we use truncate to extend file size, it could be caused by the zero-filling the extended area.  */
                if (pt_arg->pt_info->ui8_size != buf.st_size)
                {
                    INT32 i4_fd;
                    
                    i4_fd = open(ps_absolute_name, O_WRONLY, (pt_arg->pt_xent->ui4_mode & 0777));
                    if (i4_ret < 0)
                    {
                        x_mem_free(ps_absolute_name);
                        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        return FMR_CORE;
                    }
                    
                    DBG_ERROR(("!!!!!!%s Before ftruncate(new size:%llu) at sys_tick:%lu\n", __FUNCTION__,pt_arg->pt_info->ui8_size, x_os_get_sys_tick()));
                    
                    i4_ret = ftruncate(i4_fd, pt_arg->pt_info->ui8_size);
                    
                    DBG_ERROR(("!!!!!!%s After ftruncate(new size:%llu) at sys_tick:%lu\n", __FUNCTION__,pt_arg->pt_info->ui8_size, x_os_get_sys_tick()));
                    if (i4_ret < 0)
                    {
                        close(i4_fd);
                        x_mem_free(ps_absolute_name);
                        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        return FMR_CORE;
                    }

                    /*fsync(i4_fd);*/

                    /* Update dentry size */
                    
                    pt_ldentry = (LINUX_DENTRY_T *)pt_arg->pt_xent->pv_fs_node;
                    pt_ldentry->ui8_size = pt_arg->pt_info->ui8_size;

                    close(i4_fd);
                }
                #else
                if (pt_arg->pt_info->ui8_size < buf.st_size)
                {
                    INT32 i4_fd;
                    
                    i4_fd = open(ps_absolute_name, O_WRONLY, (pt_arg->pt_xent->ui4_mode & 0777));
                    if (i4_ret < 0)
                    {
                        x_mem_free(ps_absolute_name);
                        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        return FMR_CORE;
                    }
                    
                    i4_ret = ftruncate(i4_fd, pt_arg->pt_info->ui8_size);
                    if (i4_ret < 0)
                    {
                        close(i4_fd);
                        x_mem_free(ps_absolute_name);
                        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        return FMR_CORE;
                    }

                    fsync(i4_fd);

                    close(i4_fd);
                }
                else if (pt_arg->pt_info->ui8_size > buf.st_size)
                {
                    INT32 i4_fd;
                    UINT64 ui8_expand;
                    SIZE_T z_buf_size;
                    VOID* pv_buf = NULL;
                    
                    i4_fd = open(ps_absolute_name, O_WRONLY|O_APPEND, (pt_arg->pt_xent->ui4_mode & 0777));
                    if (i4_ret < 0)
                    {
                        x_mem_free(ps_absolute_name);
                        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                        return FMR_CORE;
                    }

                    ui8_expand = pt_arg->pt_info->ui8_size - buf.st_size;
                    z_buf_size = 0x00200000;

                    pv_buf = x_mem_calloc(1, z_buf_size);

                    while(!pv_buf)
                    {
                        z_buf_size = z_buf_size/2;
                        
                        pv_buf = x_mem_calloc(1, z_buf_size);
                    }

                    while(ui8_expand > z_buf_size)
                    {
                        if (write(i4_fd, pv_buf, z_buf_size) != z_buf_size)
                        {
                            fsync(i4_fd);
                            close(i4_fd);
                            x_mem_free(pv_buf);
                            x_mem_free(ps_absolute_name);
                            DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                            return FMR_CORE; 
                        }
                        ui8_expand -= z_buf_size;
                    }
                    if (ui8_expand > 0)
                    {
                        z_buf_size = ui8_expand;
                        
                        if (write(i4_fd, pv_buf, z_buf_size) != z_buf_size)
                        {
                            fsync(i4_fd);
                            close(i4_fd);
                            x_mem_free(pv_buf);
                            x_mem_free(ps_absolute_name);
                            DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                            return FMR_CORE; 
                        }
                    }
                    x_mem_free(pv_buf);
                    fsync(i4_fd);
                    close(i4_fd);
                }
                #endif

                
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
                x_mem_free(ps_absolute_name);
                return FMR_OK;
            }
            else
            {
                x_mem_free(ps_absolute_name);
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
                return FMR_INVAL;
            }
        }
        else
        {
            x_mem_free(ps_absolute_name);
            return FMR_OK;
        }
    }
}

static INT32 linux_check_file_type(
    CHAR *ps_dir_path, 
    struct dirent *dir_entry,
    FM_DIR_INFO_T *pt_info)
{
    struct stat stat_buf;
    CHAR *ps_full_path;
    size_t path_len = x_strlen(ps_dir_path) + x_strlen(dir_entry->d_name) + 2;

    ps_full_path = (CHAR *)x_mem_alloc(path_len);

    if (ps_full_path == NULL)
    {
        DBG_ERROR(("Error: Not enough Memory. %s().\n", __func__));
        return FMR_ERROR;
    }

    ps_full_path[0]='\0';
    x_strcat(ps_full_path, ps_dir_path);
    x_strcat(ps_full_path, "/");
    x_strcat(ps_full_path, dir_entry->d_name);

    if (stat(ps_full_path, &stat_buf) == -1)
    {
        DBG_ERROR(("Error: Stat fail.. %s().\n", __func__));
        x_mem_free(ps_full_path);
        return FMR_ERROR;
    }

    switch (stat_buf.st_mode & S_IFMT)
    {
        case S_IFDIR:
            DBG_INFO(("Unknown file %s is Dir.\n", ps_full_path));
            pt_info->ui4_dir_num++;
            break;
        case S_IFREG:
            DBG_INFO(("Unknown file %s is File.\n", ps_full_path));
            pt_info->ui4_file_num++;
            break;
        default:
            DBG_INFO(("Unknown file %s is not a Dir or file.\n", ps_full_path));
    }

    x_mem_free(ps_full_path);
    return FMR_OK;
}


INT32 linux_get_dir_info(VOID *pv_data)
{
    FM_DIR_INFO_ARG_T   *pt_arg = NULL;
    FM_DIR_INFO_T       *pt_info = NULL;
    CHAR* ps_absolute_name;
    INT32   i4_ret;
    int i = 0;
    struct dirent **namelist;
    INT32 i4_total;

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: NULL Argument\n", __FUNCTION__));
        return FMR_ARG;
    }

    pt_arg = (FM_DIR_INFO_ARG_T*) pv_data;
    if (pt_arg->pt_xent == NULL || pt_arg->pt_info == NULL)
    {
        DBG_ERROR(("[FM] %s: Wrong xent or info\n", __FUNCTION__));
        return FMR_INVAL;
    }
    if (!IS_XENTRY_DIR(pt_arg->pt_xent))
    {
        DBG_ERROR(("[FM] %s: Not a DIR.\n", __FUNCTION__));
        return FMR_NOT_DIR;
    }
    pt_info = pt_arg->pt_info;

    ps_absolute_name = linux_generate_absolute_name(
        pt_arg->pt_xent->pt_prnt_xent,
        pt_arg->pt_xent,
        pt_arg->pt_xent->ps_name);

    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: Can not get absolute name.\n", __FUNCTION__));
        return FMR_CORE;
    }

    pt_info->ui4_dir_num = 0;
    pt_info->ui4_file_num = 0;
    i4_total = scandir(ps_absolute_name, &namelist, NULL, NULL);
    if (i4_total < 0)
    {
        x_mem_free(ps_absolute_name);
        DBG_ERROR(("[FM] %s: scandir return %s\n", __FUNCTION__, strerror(errno)));
        i4_ret = FMR_CORE;
    }
    else
    {
        i4_ret = FMR_OK;
        for (i = 0; i < i4_total; i++)
        {
            #if 0
            if (namelist[i]->d_type == DT_REG)
            {
                pt_info->ui4_file_num ++;
            }
            else if (namelist[i]->d_type == DT_DIR)
            {
                pt_info->ui4_dir_num ++;
            }
            else if (namelist[i]->d_type == DT_UNKNOWN)
            {
                linux_check_file_type(ps_absolute_name, namelist[i], pt_info);
            }
            #else
            if ((namelist[i]->d_type == DT_REG) ||
                (namelist[i]->d_type == DT_DIR) ||
                (namelist[i]->d_type == DT_UNKNOWN))
            {
                linux_check_file_type(ps_absolute_name, namelist[i], pt_info);
            }
            #endif
            
            free(namelist[i]);
        }
        free(namelist);

        x_mem_free(ps_absolute_name);
        DBG_INFO(("[FM]%s:total:%d, dir:%d, file:%d\n",__FUNCTION__, 
                    i4_total, pt_info->ui4_dir_num, pt_info->ui4_file_num));
    }
    return i4_ret;
}


INT32 linux_delete(VOID *pv_data)
{
    FM_DELETE_ARG_T *pt_del_arg = NULL;
    FM_XENTRY_T     *pt_prnt_xent = NULL;
    FM_XENTRY_T     *pt_xent = NULL;
    LINUX_DENTRY_T  *pt_parent_ent, *pt_ent;
    CHAR* ps_absolute_name;
    INT32 i4_ret;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    ASSERT(pv_data != NULL);

    pt_del_arg   = (FM_DELETE_ARG_T *)pv_data;
    pt_prnt_xent = (FM_XENTRY_T *)pt_del_arg->pt_prnt_xent;
    pt_xent      = (FM_XENTRY_T *)pt_del_arg->pt_xent;

    ASSERT(pt_prnt_xent != NULL);
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_prnt_xent));
    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    if (IS_XENTRY_MOUNT_ROOT(pt_xent))
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }

    pt_ent = (LINUX_DENTRY_T *)pt_xent->pv_fs_node;
    pt_parent_ent = (LINUX_DENTRY_T *)pt_prnt_xent->pv_fs_node;

    ASSERT(pt_ent != NULL);
    ASSERT(pt_parent_ent != NULL);
    /* Close all the opened fd first */
    /*linux_free_fs_node(pt_ent);*/
    while(!DLIST_IS_EMPTY(&pt_ent->t_fd_list))
    {
        LINUX_FD_INFO_T* pt_fd_info = DLIST_HEAD(&pt_ent->t_fd_list);
        DLIST_REMOVE(pt_fd_info, &pt_ent->t_fd_list, t_link);
        DBG_INFO(("[FM] %s: line:%d,close:%d\n", __FUNCTION__,__LINE__,pt_fd_info->i4_fd));
#if !defined(ANDROID) && !defined(__UCLIBC__)
        aio_cancel(pt_fd_info->i4_fd, NULL);
#endif
        fsync(pt_fd_info->i4_fd);
        close(pt_fd_info->i4_fd);
        DBG_INFO(("[FM] %s: line:%d\n", __FUNCTION__,__LINE__));
        x_mem_free(pt_fd_info);
    }

    ps_absolute_name = linux_generate_absolute_name(
       pt_del_arg->pt_prnt_xent,
       pt_del_arg->pt_xent,
       pt_del_arg->pt_xent->ps_name);

    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    else
    {
        if (IS_XENTRY_FILE(pt_xent))
        {
            i4_ret = remove(ps_absolute_name);
            DBG_INFO(("[FM] %s: line:%d, remove(%s) return %s\n", __FUNCTION__,__LINE__, ps_absolute_name,strerror(errno)));
        }
        else if (IS_XENTRY_DIR(pt_xent))
        {
            i4_ret = rmdir(ps_absolute_name);
            DBG_INFO(("[FM] %s: line:%d, rmdir(%s) return %s\n", __FUNCTION__,__LINE__, ps_absolute_name,strerror(errno)));
        }
        else
        {
            i4_ret = FMR_CORE;
            DBG_ERROR(("[FM] %s: line:%d, %s is not a file or dir\n", __FUNCTION__,__LINE__, ps_absolute_name));
        }
        
        if (i4_ret < 0)
        {
            switch (errno)
            {
                case EACCES:
                    i4_ret = FMR_ACCESS;
                    break;
                case EPERM:
                    i4_ret = FMR_PERM_DENY;
                    break;
                case EBUSY:
                    i4_ret = FMR_BUSY;
                    break;
                case ENOENT:
                    i4_ret = FMR_NO_ENTRY;
                    break;
                case ENOTDIR:
                    i4_ret = FMR_NOT_DIR;
                    break;
                case ENOTEMPTY:
                    i4_ret = FMR_DIR_NOT_EMPTY;
                    break;
                case ENAMETOOLONG:
                    i4_ret = FMR_NAME_TOO_LONG;
                    break;
                default:
                    i4_ret = FMR_CORE;
                    break;
            }
        }
        else
        {
            x_mem_free(pt_xent->pv_fs_node);
            pt_xent->pv_fs_node = NULL;
        }
    }
    x_mem_free(ps_absolute_name);
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
    return i4_ret;
}

INT32 linux_rename(VOID *pv_data)
{
    FM_RENAME_ARG_T *pt_rename_arg = NULL;
    CHAR            *ps_absolute_src_name = NULL;
    CHAR            *ps_absolute_dest_name = NULL;
    INT32           i4_ret = FMR_OK;

    if (pv_data == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }

    
    pt_rename_arg = (FM_RENAME_ARG_T *)pv_data;
	
    if ( pt_rename_arg->pt_src_prnt_xent == NULL ||
         pt_rename_arg->pt_xent == NULL          ||
         pt_rename_arg->pt_dst_prnt_xent == NULL ||
         pt_rename_arg->ps_name == NULL )
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
	
    ps_absolute_src_name = linux_generate_absolute_name(
		   pt_rename_arg->pt_src_prnt_xent,
		   pt_rename_arg->pt_xent,
		   pt_rename_arg->pt_xent->ps_name);
    if (ps_absolute_src_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    ps_absolute_dest_name = linux_generate_absolute_name(
		   pt_rename_arg->pt_dst_prnt_xent,
		   NULL,
		   pt_rename_arg->ps_name);
	
    if (ps_absolute_dest_name == NULL)
    {
        if(ps_absolute_src_name != NULL)
        {
            x_mem_free(ps_absolute_src_name);
            ps_absolute_src_name = NULL;
        }
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    DBG_INFO(("The absolute source name: %s\n", ps_absolute_src_name));
    DBG_INFO(("The absolute destimation name: %s\n", ps_absolute_dest_name));

    i4_ret = rename(ps_absolute_src_name, ps_absolute_dest_name);

    if(ps_absolute_src_name != NULL)
    {
        x_mem_free(ps_absolute_src_name);
        ps_absolute_src_name = NULL;
    }
    if(ps_absolute_dest_name != NULL)
    {
        x_mem_free(ps_absolute_dest_name);
        ps_absolute_dest_name = NULL;
    }
	
    if (i4_ret == -1)
    {
        switch (errno)
        {
            case EISDIR:                
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
                return FMR_ARG;
				
            case EXDEV:             
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_FILE_SYSTEM_OTHER));
                return FMR_FILE_SYSTEM_OTHER;
						
            case ENOTEMPTY:
            case EEXIST:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_DIR_NOT_EMPTY));
                return FMR_DIR_NOT_EMPTY;

            case EBUSY:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_BUSY));
                return FMR_BUSY;

            case ENOTDIR:
                DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_NOT_DIR));
                return FMR_NOT_DIR;

            default:
                DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
                return FMR_CORE;
        }
    }
    
    return FMR_OK;
}

INT32 linux_make_entry(VOID *pv_data)
{
    FM_MKENT_ARG_T  *pt_arg = NULL;
    FM_MOUNT_T      *pt_mount = NULL;
    FM_XENTRY_T     *pt_prnt_xent = NULL;
    LINUX_DENTRY_T  *pt_prnt_dent = NULL;
    LINUX_DENTRY_T  *pt_dent = NULL;
    const CHAR      *ps_name = NULL;
    struct stat buf;
    CHAR* ps_absolute_name;
    UINT32          ui4_mode = 0;
    INT32           i4_ret;
    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));

    ASSERT(pv_data != NULL);

    pt_arg = (FM_MKENT_ARG_T *)pv_data;

    ASSERT(pt_arg->pt_this_xent != NULL);
    ASSERT(pt_arg->pt_this_xent->pt_mount != NULL);
    ASSERT(pt_arg->ps_name      != NULL);
    ASSERT(IS_XENTRY_DIR(pt_arg->pt_this_xent));

    pt_prnt_xent = pt_arg->pt_this_xent;
    pt_mount = pt_prnt_xent->pt_mount;
    ps_name  = pt_arg->ps_name;

    pt_prnt_dent = (LINUX_DENTRY_T *)pt_prnt_xent->pv_fs_node;
    ui4_mode = pt_arg->ui4_mode;

    ps_absolute_name = linux_generate_absolute_name(pt_prnt_xent, NULL, ps_name);
    if (ps_absolute_name == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }

    switch (ui4_mode & FM_MODE_TYPE_MASK)
    {
        case FM_MODE_TYPE_DIR:
            {
                DBG_INFO(("[FM] %s: mkdir:%s\n", __FUNCTION__, ps_absolute_name));
                i4_ret = mkdir(ps_absolute_name, ui4_mode);
                if (i4_ret == 0)
                {
                    i4_ret = FMR_OK;
                    if ((ui4_mode & 0022) != 0)
                    {
                        chmod(ps_absolute_name, (ui4_mode & FM_MODE_PERM_MASK));
                    }
                }
                else
                {
                    switch (errno)
                    {
                        case ENAMETOOLONG:
                            i4_ret = FMR_NAME_TOO_LONG;
                            break;
                        case ENOENT:
                            i4_ret = FMR_NO_ENTRY;
                            break;
                        case EEXIST:
                            i4_ret = FMR_EXIST;
                            break;
                        case ENOTDIR:
                            i4_ret = FMR_NOT_DIR;
                            break;
                        default:
                            i4_ret = FMR_CORE;
                            break;
                    }
                }
            }
            break;

        case FM_MODE_TYPE_FILE:
            {
                INT32 i4_fd;
                DBG_INFO(("[FM] %s: creat:%s\n", __FUNCTION__, ps_absolute_name));
                i4_fd = creat(ps_absolute_name, (O_RDWR | O_CREAT | O_EXCL));

                if (i4_fd == -1)
                {
                    switch (errno)
                    {
                        case ENOENT:
                            i4_ret = FMR_NO_ENTRY;
                            break;
                        case EEXIST:
                            i4_ret = FMR_EXIST;
                            break;
                        case ENOTDIR:
                            i4_ret = FMR_NOT_DIR;
                            break;
                        case ENAMETOOLONG:
                            i4_ret = FMR_NAME_TOO_LONG;
                            break;
                        default:
                            i4_ret = FMR_CORE;
                            break;
                    }
                }
                else
                {
                    /* Just create, need not open */
                    close(i4_fd);
                    i4_ret = FMR_OK;
                }
            }
            break;

        case FM_MODE_TYPE_BLK:
            {
                DBG_INFO(("[FM] %s: mknode(blk):%s\n", __FUNCTION__, ps_absolute_name));
                i4_ret = mknod(ps_absolute_name, FM_MODE_TYPE_BLK, ((pt_arg->ui2_dev & 0xFFF) << 20) | (pt_arg->ui2_unit & 0xFFFFF));
                if (i4_ret == -1)
                {
                    switch (errno)
                    {
                        case EEXIST:
                            i4_ret = FMR_EXIST;
                            break;
                        case ENAMETOOLONG:
                            i4_ret = FMR_NAME_TOO_LONG;
                            break;
                        default:
                            i4_ret = FMR_CORE;
                            break;
                    }
                }
                else
                {
                    i4_ret = FMR_OK;
                }
            }
            break;

        case FM_MODE_TYPE_CHR:
            {
                DBG_INFO(("[FM] %s: mknode(chr):%s\n", __FUNCTION__, ps_absolute_name));
                i4_ret = mknod(ps_absolute_name, FM_MODE_TYPE_CHR, ((pt_arg->ui2_dev & 0xFFF) << 20) | (pt_arg->ui2_unit & 0xFFFFF));
                if (i4_ret == -1)
                {
                    switch (errno)
                    {
                        case EEXIST:
                            i4_ret = FMR_EXIST;
                            break;
                        case ENAMETOOLONG:
                            i4_ret = FMR_NAME_TOO_LONG;
                            break;
                        default:
                            i4_ret = FMR_CORE;
                            break;
                    }
                }
                else
                {
                    i4_ret = FMR_OK;
                }
            }
            break;

        default:
            i4_ret = FMR_CORE;
            break;
    }
    if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }
    i4_ret = stat(ps_absolute_name, &buf);
    if (i4_ret < 0)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %s\n", __FUNCTION__,__LINE__, strerror(errno)));
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    /* Make xentry */
    pt_arg->pt_new_xent = fm_get_new_xentry(ps_name);
    if (pt_arg->pt_new_xent == NULL)
    {
        i4_ret = FMR_CORE;
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto FAIL;
    }
    pt_dent = x_mem_alloc(sizeof(LINUX_DENTRY_T));
    if (pt_dent == NULL)
    {
        i4_ret = FMR_CORE;
        
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
        goto FAIL;
    }
    DLIST_INIT(&(pt_dent->t_fd_list));

    pt_arg->pt_new_xent->ui4_mode = 0;

    if (S_ISDIR(buf.st_mode))
    {
        pt_arg->pt_new_xent->ui4_mode |= FM_MODE_TYPE_DIR;
    }
    else if (S_ISREG(buf.st_mode))
    {
        pt_arg->pt_new_xent->ui4_mode |= FM_MODE_TYPE_FILE;
    }
    pt_arg->pt_new_xent->ui4_mode |= (buf.st_mode & 0777);

    pt_arg->pt_new_xent->pt_mount     = pt_prnt_xent->pt_mount;
    pt_arg->pt_new_xent->pt_ufs_ops   = pt_prnt_xent->pt_ufs_ops;
    pt_arg->pt_new_xent->pv_fs_node   = pt_dent;
    pt_arg->pt_new_xent->pt_prnt_xent = pt_prnt_xent;
    pt_arg->pt_new_xent->pt_xent_ops  = pt_prnt_xent->pt_xent_ops;
    pt_arg->pt_new_xent->pt_dev       = pt_prnt_xent->pt_mount->pt_dev_xent->pt_dev;

    i4_ret = FMR_OK;
FAIL:
    
    if (NULL != ps_absolute_name)
    {
        x_mem_free(ps_absolute_name);
    }
    
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
    return i4_ret;
}



INT32 linux_close(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent = NULL;
    LINUX_DENTRY_T  *pt_dent = NULL;
    LINUX_FD_INFO_T *pt_fd_info;
    HANDLE_T h_file;

    pt_xent = (FM_XENTRY_T*) pv_data;
    DBG_INFO(("[FM] %s: enter, pt_xent:%d\n ", __FUNCTION__, pt_xent));

    if (pt_xent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_ARG));
        return FMR_ARG;
    }
    DBG_INFO(("[FM] %s: line:%d\n", __FUNCTION__,__LINE__));

    h_file = pt_xent->h_ref_to_close;
    if (h_file == NULL_HANDLE)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    DBG_INFO(("[FM] %s: line:%d\n", __FUNCTION__,__LINE__));

    pt_dent = (LINUX_DENTRY_T *)pt_xent->pv_fs_node;
    if (pt_dent == NULL)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_INVAL));
        return FMR_INVAL;
    }
    DBG_INFO(("[FM] %s: line:%d,pt_dent:%d, h_file:%d\n", __FUNCTION__,__LINE__, pt_dent, h_file));

    pt_fd_info = _linux_find_fd_info_by_fh(pt_dent, h_file);
    if (pt_fd_info == NULL)
    {
        DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
        return FMR_OK;
    }    
    DBG_INFO(("[FM] %s: line:%d,pt_fd_info->i4_fd:%d\n", __FUNCTION__,__LINE__,pt_fd_info->i4_fd));
    DLIST_REMOVE(pt_fd_info, &pt_dent->t_fd_list, t_link);
    DBG_INFO(("[FM] %s: line:%d,pt_fd_info->i4_fd:%d\n", __FUNCTION__,__LINE__,pt_fd_info->i4_fd));

    switch (pt_xent->ui4_mode & FM_MODE_TYPE_MASK)
    {
        case FM_MODE_TYPE_DIR:
        {
            DBG_INFO(("%s, closedir(%d)...\n",__FUNCTION__, pt_fd_info->i4_fd));
            if (closedir((DIR *)(pt_fd_info->i4_fd)) < 0)
            {
                DBG_INFO(("%s, closedir(%d) failed as %s \n",__FUNCTION__, pt_fd_info->i4_fd, strerror(errno)));
            }
        }
        break;

        case FM_MODE_TYPE_FILE:
        {
            DBG_INFO(("%s, close(%d)...\n",__FUNCTION__, pt_fd_info->i4_fd));
#if !defined(ANDROID) && !defined(__UCLIBC__)
            aio_cancel(pt_fd_info->i4_fd, NULL);
#endif
            fsync(pt_fd_info->i4_fd);
            if (close(pt_fd_info->i4_fd) < 0)
            {
                DBG_INFO(("%s, close(%d) failed as %s \n",__FUNCTION__, pt_fd_info->i4_fd, strerror(errno)));
            }
        }
        break;

        default:
            break;
    }

#if 1
    {
        LINUX_FD_T* pt_fd = NULL;
        LINUX_FD_T* pt_nxt_fd = NULL;
        LINUX_MP_OPENED_FD_T* pt_mpofd = NULL;

        CHAR* ps_absolute_name;

        ps_absolute_name = linux_generate_absolute_name(
            pt_xent->pt_prnt_xent,
            pt_xent,
            pt_xent->ps_name);

        if (ps_absolute_name != NULL)
        {
            DBG_INFO(("%s Close %s\n", __FUNCTION__, ps_absolute_name));
            DLIST_FOR_EACH(pt_mpofd, &gt_mp_open_fd_q, t_link)
            {
                if (strstr(ps_absolute_name, pt_mpofd->ps_mp_path))
                {
                    break;
                }
            }
            
            if (pt_mpofd != NULL)
            {
                if (x_sema_lock(pt_mpofd->h_mtx, X_SEMA_OPTION_WAIT) == OSR_OK)
                {
                    for (pt_fd = pt_mpofd->t_fd_list.pt_head;
                         pt_fd != NULL;
                         pt_fd = pt_nxt_fd)
                    {
                        pt_nxt_fd = pt_fd->t_link.pt_next;
                        
                        if (pt_fd->i4_fd == pt_fd_info->i4_fd)
                        {
                            DBG_INFO(("%s, remove(%d) from the opened fd list..\n",__FUNCTION__, pt_fd_info->i4_fd));
                            DLIST_REMOVE(pt_fd, &pt_mpofd->t_fd_list, t_link);
                            
                            if (x_sema_lock(gh_free_fd_list_sema, X_SEMA_OPTION_WAIT) == OSR_OK)
                            {
                                DLIST_INSERT_HEAD(pt_fd, &gt_free_fd_list, t_link);
                                x_sema_unlock(gh_free_fd_list_sema);
                            }
                            else
                            {
                                free(pt_fd);
                                pt_fd = NULL;
                            }
                        }
                    }
                    x_sema_unlock(pt_mpofd->h_mtx);
                }
            }
            x_mem_free(ps_absolute_name);
            ps_absolute_name = NULL;
        }
    }
#endif
    x_mem_free(pt_fd_info);
    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_OK));
    return FMR_OK;

}


INT32 linux_sync(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent = NULL;
    INT32           i4_ret = FMR_OK;
    INT32           i4_fd = -1;

    ASSERT(pv_data != NULL);

    pt_xent = (FM_XENTRY_T *)pv_data;
    
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(pt_xent->pt_mount != NULL);

    i4_ret = _linux_find_fd_by_fh(pt_xent, pt_xent->h_file, &i4_fd);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, FMR_CORE));
        return FMR_CORE;
    }
    
    i4_ret = fsync(i4_fd);

    if (i4_ret != 0)
    {
        i4_ret = FMR_ERROR;
    }
    else
    {
        i4_ret = FMR_OK;
    }

    DBG_INFO(("[FM] %s: line:%d, return %d\n", __FUNCTION__,__LINE__, i4_ret));
    return FMR_OK;
}


INT32 linux_free_fs_node(VOID *pv_data)
{
    LINUX_DENTRY_T  *pt_ent;
    LINUX_FD_INFO_T *pt_fd_info;

    ASSERT(pv_data != NULL);
    pt_ent = (LINUX_DENTRY_T *)pv_data;
    DBG_INFO(("[FM] %s: enter pt_ent=%d\n", __FUNCTION__,pt_ent));

    while(!DLIST_IS_EMPTY(&pt_ent->t_fd_list))
    {
        pt_fd_info = DLIST_HEAD(&pt_ent->t_fd_list);
        DLIST_REMOVE(pt_fd_info, &pt_ent->t_fd_list, t_link);
        DBG_INFO(("[FM] closing:%d...\n", pt_fd_info->i4_fd));
#if !defined(ANDROID) && !defined(__UCLIBC__)
        aio_cancel(pt_fd_info->i4_fd, NULL);
#endif
        fsync(pt_fd_info->i4_fd);
        close(pt_fd_info->i4_fd);
        x_mem_free(pt_fd_info);
    }
    x_mem_free(pt_ent);
    DBG_INFO(("[FM] %s: return %d\n", __FUNCTION__, FMR_OK));
    return FMR_OK;
}

INT32 linux_build_dir_idx_tbl(VOID *pv_data)
{
    DBG_INFO(("[FM] %s: enter\n", __FUNCTION__));
    return FMR_NOT_IMPLEMENT;
}


