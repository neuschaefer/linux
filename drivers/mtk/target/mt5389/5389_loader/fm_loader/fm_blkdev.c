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
 * $RCSfile: fm_blkdev.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/11 $
 * $Author: yingjoe.chen $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 0228d36d239d8d407d774b254b6b5be7 $
 *
 * Description: 
 *         This file implements the block device access methods.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "fm_util.h"
#include "fm_ufs.h"
#include "fm_blkdev.h"
#include "fm_buf.h"
#include "fm_rootfs.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#undef x_dbg_abort
#define x_dbg_abort(a, b)
#define x_dbg_ctrl_stmt(fmt...)
#define dbg_eval_dbg_level(a)   (0)
#undef DBG_INFO
#define DBG_INFO(fmt...)
#undef ASSERT
#define ASSERT(x)		((void)0)

#ifndef DEBUG_FM_BLKDEV

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_BLKDEV */

#define BLKDEV_DEF_SYNC_PRI (16)

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

static VOID _blkdev_sync_nfy_fct(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data);

static INT32 _blkdev_start_io(
    FM_RW_ARG_T     *pt_arg,
    BOOL            b_read);

static INT32 _blkdev_read_write(
    FM_RW_ARG_T     *pt_arg,
    BOOL            b_read);

extern INT32 FM_MUSB_Get(HANDLE_T hHandle, UINT32 u4GetType, VOID* pvData, UINT32 *pu4Size);
extern INT32 FM_MUSB_Set(HANDLE_T hHandle, UINT32 u4SetType, VOID* pvData, UINT32 u4Size);
extern INT32 FM_MUSB_Open(HANDLE_T* hHandle);

#ifdef CC_MTK_LOADER

#ifdef x_crit_start
#undef x_crit_start
#endif

#ifdef x_crit_end
#undef x_crit_end
#endif

#define x_sprintf                   SPrintf
#define x_sema_create(a, b, c)      (OSR_OK)
#define x_sema_delete(a)
#define x_crit_start()              ((CRIT_STATE_T)0)
#define x_crit_end(a)
#define x_dbg_stmt(fmt...)
#define x_rwl_create_lock(a)        (0)
#define x_rwl_read_lock(a, b)       (0)
#define x_rwl_read_lock_grabbed(a)  (0)
#define x_rwl_release_lock(a)       _GetZero()
#define x_rwl_write_lock(a, b)      (0)
#define x_rwl_write_lock_grabbed(a) (0)
#define x_sema_lock(a, b)           _GetZero()
#define x_sema_unlock(a)            _GetZero()

static UINT32 _GetZero()
{
    return 0;
}

#endif /* CC_MTK_LOADER */

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _blkdev_sync_nfy_fct
 *
 * Description: Block device callback function for synchronous read/write.
 *              This function is required because block device only has
 *              asynchronous read/write.
 *
 * Inputs:  pv_tag
 *              Reference to the read/write argument.
 *          e_async_cond
 *              Asynchronous notify condition.
 *          ui4_data
 *              Read/Written bytes if the operation was successful.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _blkdev_sync_nfy_fct(
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
    x_sema_unlock(pt_arg->h_bin_sema);
}

/*-----------------------------------------------------------------------------
 * Name: _blkdev_start_io
 *
 * Description: Synchronous/Asynchronous read/write.
 *
 * Inputs:  pt_arg
 *              Reference to the read/write argument.
 *          b_read
 *              Is this for read or write.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invalid data count or offset.
 *          FMR_ALIGNMENT
 *              Bad alignment of data buffer address.
 *          FMR_DEVICE_ERROR
 *              Device operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _blkdev_start_io(
    FM_RW_ARG_T     *pt_arg,
    BOOL            b_read)
{
    FM_DEVICE_T     *pt_dev;
    FM_IO_REQ_T     *pt_req;
    INT32           i4_ret;

    ASSERT(pt_arg != NULL);
    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(pt_arg->pt_xent->pt_dev != NULL);

    DEBUG_TRACE();

    pt_dev = pt_arg->pt_xent->pt_dev;

    if (((UINT32)pt_arg->pv_data % pt_dev->t_blk.ui4_align_bytes) != 0)
    {
        return FMR_ALIGNMENT;
    }
    if (((pt_arg->ui4_count  % pt_dev->t_blk.ui4_sec_size) != 0) ||
        ((pt_arg->ui8_offset % pt_dev->t_blk.ui4_sec_size) != 0))
    {
        return FMR_INVAL;
    }

    pt_req = x_mem_alloc(sizeof(FM_IO_REQ_T));
    if (pt_req == NULL)
    {
        return FMR_CORE;
    }

    x_memset((VOID *) pt_req, 0, sizeof(FM_IO_REQ_T));

    if (pt_arg->ui2_flag & FM_IO_SYNC)
    {
        if (x_sema_lock(pt_arg->h_bin_sema, X_SEMA_OPTION_WAIT) != OSR_OK)
        {
            x_mem_free(pt_req);
            return FMR_CORE;
        }
        pt_req->ui1_pri      = BLKDEV_DEF_SYNC_PRI;
        pt_req->pv_tag       = pt_arg;
        pt_req->pf_nfy_fct   = _blkdev_sync_nfy_fct;
    }
    else
    {
#if 0 // [5391 loader]
        pt_req->ui1_pri      = pt_arg->ui1_pri;
        pt_req->pv_tag       = pt_arg->pv_tag;
        pt_req->h_rw_req     = pt_arg->h_rw_req;
        pt_req->pf_nfy_fct   = pt_arg->pf_nfy_fct;
#else
        ASSERT(0);
#endif
    }

    pt_req->u_io.t_bio.pv_data          = pt_arg->pv_data;
    pt_req->u_io.t_bio.ui8_blk_num      = pt_arg->ui8_offset / pt_dev->t_blk.ui4_sec_size;
    pt_req->u_io.t_bio.ui4_count        = pt_arg->ui4_count;
    pt_req->u_io.t_bio.ui4_trans_count  = 0;
    pt_req->u_io.t_bio.b_ext_buf        = TRUE;
    
    pt_req->u_io.t_bio.t_nfy_info.pv_tag = 0;
    pt_req->u_io.t_bio.t_nfy_info.pf_blkdev_nfy = NULL;
    
    pt_req->z_len                       = sizeof(BLKDEV_IO_DATA_T);
    pt_req->ui4_done_byte               = pt_arg->ui4_done_bytes;
    pt_req->ui2_hold_cnt                = 0;
    
    if (b_read == TRUE)
    {
        /*
         *  Unlike synchronous read, RM uses SET function to read
         *  data asynchronously. So we use the FM_DEV_REQ_READ flag
         *  to distinguish the read/write operation.  
         */
        pt_req->ui1_type = FM_DEV_REQ_TYPE_SET|FM_DEV_REQ_TYPE_BLK|FM_DEV_REQ_READ;
        pt_req->ui4_op   = BLKDEV_SET_TYPE_READ;
    }
    else
    {
        pt_req->ui1_type = FM_DEV_REQ_TYPE_SET|FM_DEV_REQ_TYPE_BLK;
        pt_req->ui4_op   = BLKDEV_SET_TYPE_WRITE;
    }
        
    i4_ret = fm_dev_send_req(pt_dev, pt_req);

    if (i4_ret != FMR_OK)
    {
        x_mem_free(pt_req);
        if (pt_arg->ui2_flag & FM_IO_SYNC)
        {
            x_sema_unlock(pt_arg->h_bin_sema);
        }
    }
    else
    {
        if (pt_arg->ui2_flag & FM_IO_SYNC)
        {
            x_sema_lock(pt_arg->h_bin_sema, X_SEMA_OPTION_WAIT);
            x_sema_unlock(pt_arg->h_bin_sema);
            if (pt_arg->e_async_cond == FM_ASYNC_COND_FAIL)
            {
                i4_ret = FMR_DEVICE_ERROR;
            }
        }

        /* pt_req is free in fm_dev_async_thread() after I/O is done. */
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _blkdev_read_write
 *
 * Description: Synchronous/Asynchronous cached/non-cached read/write.
 *
 * Inputs:  pt_arg
 *              Reference to the read/write argument.
 *          b_read
 *              Is this for read or write.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Device operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
static INT32 _blkdev_read_write(
    FM_RW_ARG_T *pt_arg,
    BOOL        b_read)
{
    FM_DEVICE_T *pt_dev;
    UINT32      ui4_old_cnt;
    INT32       i4_ret;

    ASSERT(pt_arg != NULL);

    ASSERT(pt_arg->pt_xent != NULL);
    
    pt_dev = pt_arg->pt_xent->pt_dev;

    if ((pt_dev == NULL) || (pt_dev->h_driver == NULL_HANDLE))
    {
        return FMR_INVAL; /* not opened or closed */
    }
    if (pt_dev->ui8_space <= pt_arg->ui8_offset)
    {
        return FMR_EOF;
    }

    ui4_old_cnt = pt_arg->ui4_count;

    if ((pt_arg->ui8_offset + pt_arg->ui4_count) > pt_dev->ui8_space)
    {
        pt_arg->ui4_count = (UINT32)(pt_dev->ui8_space - pt_arg->ui8_offset);
    }

    ASSERT(pt_arg->ui2_flag & FM_IO_DIRECT);
    ASSERT(pt_arg->ui2_flag & FM_IO_SYNC);

#if 0    
    if (pt_arg->ui2_flag & FM_IO_DIRECT)
    {
        i4_ret = _blkdev_start_io(pt_arg, b_read);
    }
    else
    {
        i4_ret = fm_buf_read_write(pt_arg, b_read);
    }
#else
    i4_ret = _blkdev_start_io(pt_arg, b_read);
#endif

    pt_arg->ui4_count = ui4_old_cnt;

    return i4_ret;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: blkdev_open
 *
 * Description: Open a block device.
 *
 * Inputs:  pt_xent
 *              The xentry of the block device.
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
INT32 blkdev_open(
    FM_XENTRY_T         *pt_xent)
{
//    BLKDEV_NFY_INFO_T   t_nfy_info = {0};
    FM_DEVICE_T         *pt_dev = NULL;
    UINT16              ui2_drv_type;
    UINT16              ui2_drv_unit;
    SIZE_T              z_read;
    INT32               i4_ret;
    ROOTFS_ENTRY_T      *pt_rent = NULL;

    ASSERT(pt_xent != NULL);
/*
    t_get_arg.pt_xent = pt_xent;
    t_get_arg.pt_info = &t_file_info;

    i4_ret = XENTRY_OP_GET_INFO(pt_xent)(&t_get_arg);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }

    ui2_drv_type = t_get_arg.pt_info->ui4_inode & 0xffff;
    ui2_drv_unit = t_get_arg.pt_info->ui4_inode >> 16;
*/
    pt_rent = ROOTFS_GET_ENT(pt_xent);
    ui2_drv_type = (pt_rent->ui4_ent_id) & 0xffff;
    ui2_drv_unit = (pt_rent->ui4_ent_id) >> 16;
    
    i4_ret = fm_dev_open(pt_xent->ps_name,
                         ui2_drv_type,
                         ui2_drv_unit,
                         &pt_dev);
    if (i4_ret == FMR_EXIST)
    {
        goto DONE;
    }
    else if (i4_ret != FMR_OK)
    {
        goto FAIL;
    }
    
    /*
     *  Get device size.
     */

    z_read = sizeof(UINT64);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_SIZE, 
                         (VOID*)&pt_dev->t_blk.ui8_dev_size, 
                         (UINT32*)&z_read);

    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT64)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else

    i4_ret = rm_get(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_GET_TYPE_SIZE,
                    (VOID *)&pt_dev->t_blk.ui8_dev_size,
                    &z_read,
                    &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT64)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

    /*
     *  Get number of sectors.
     */

    z_read = sizeof(UINT64);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_NUMBER_OF_SECTORS, 
                         (VOID*)&pt_dev->t_blk.ui8_sec_num, 
                         (UINT32*)&z_read);
                         
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT64)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else

    i4_ret = rm_get(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_GET_TYPE_NUMBER_OF_SECTORS,
                    (VOID *)&pt_dev->t_blk.ui8_sec_num,
                    &z_read,
                    &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT64)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

    /*
     *  Get sector size.
     */

    z_read = sizeof(UINT32);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_SECTOR_SIZE, 
                         (VOID*)&pt_dev->t_blk.ui4_sec_size, 
                         (UINT32*)&z_read);             

    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else
    i4_ret = rm_get(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_GET_TYPE_SECTOR_SIZE,
                    (VOID *)&pt_dev->t_blk.ui4_sec_size,
                    &z_read,
                    &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

    /*
     *  Get max trans sectors.
     */

    z_read = sizeof(UINT32);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_MAX_TRANS_SECTORS, 
                         (VOID*)&pt_dev->t_blk.ui4_max_trans_sec, 
                         (UINT32*)&z_read); 

    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else

    i4_ret = rm_get(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_GET_TYPE_MAX_TRANS_SECTORS,
                    (VOID *)&pt_dev->t_blk.ui4_max_trans_sec,
                    &z_read,
                    &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

    /*
     *  Get number of requests that driver can handle simultaneously.
     */
     z_read = sizeof(UINT32);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_MAX_IO_REQUEST, 
                         (VOID*)&pt_dev->t_blk.ui4_max_io_req, 
                         (UINT32*)&z_read);
                         
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else

     i4_ret = rm_get(pt_dev->h_driver,
                     FALSE,
                     0,
                     BLKDEV_GET_TYPE_MAX_IO_REQUEST,
                     (VOID *)&pt_dev->t_blk.ui4_max_io_req,
                     &z_read,
                     &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

    /*
     *  Get alignment bytes.
     */

    z_read = sizeof(UINT32);

#if 1 /*  [Nathan] link with driver */  
#ifndef CC_USB_DISABLE
    i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                         BLKDEV_GET_TYPE_ALIGNMENT, 
                         (VOID*)&pt_dev->t_blk.ui4_align_bytes, 
                         (UINT32*)&z_read);
                         
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
#else

    i4_ret = rm_get(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_GET_TYPE_ALIGNMENT,
                    (VOID *)&pt_dev->t_blk.ui4_align_bytes,
                    &z_read,
                    &e_cond);
    if ((i4_ret != RMR_OK) || (z_read != sizeof(UINT32)))
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif

        /* Actually, Max did the same calculation for BLKDEV_GET_TYPE_SZIE, too.*/

    pt_dev->ui8_space = pt_dev->t_blk.ui8_sec_num * pt_dev->t_blk.ui4_sec_size;

    if (pt_dev->ui8_space == 0)
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
    ASSERT(pt_dev->ui8_space != 0);

    /*
     *  Setup the notification function for async operations.
     */
     
#if 1 /*  [Nathan] link with driver */  

#if 0
    t_nfy_info.pv_tag = pt_dev;
    t_nfy_info.pf_blkdev_nfy = (x_blkdev_nfy_fct)fm_dev_nfy_fct;

    i4_ret = FM_MUSB_Set(pt_dev->h_driver, 
                         BLKDEV_SET_TYPE_NFY_FCT, 
                         (VOID*)&t_nfy_info, 
                         sizeof(BLKDEV_NFY_INFO_T));

    if (i4_ret != RMR_OK)
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }    
#endif
#else

    t_nfy_info.pv_tag = pt_dev;
    t_nfy_info.pf_blkdev_nfy = (x_blkdev_nfy_fct)fm_dev_nfy_fct;
    i4_ret = rm_set(pt_dev->h_driver,
                    FALSE,
                    0,
                    BLKDEV_SET_TYPE_NFY_FCT,
                    (VOID *)&t_nfy_info,
                    sizeof(BLKDEV_NFY_INFO_T),
                    &e_cond);
    if (i4_ret != RMR_OK)
    {
        i4_ret = FMR_DEVICE_ERROR;
        goto FAIL;
    }
#endif
    DBG_INFO(("[FM] Device:%s\n"
                "     Size:%lld bytes\n"
                "     Sector: %lld\n"
                "     Sector size: %d bytes\n"
                "     Max trans sectors: %d\n"
                "     Max io req: %d\n"
                "     Alignment : %d bytes\n",
                pt_xent->ps_name,
                pt_dev->t_blk.ui8_dev_size,
                pt_dev->t_blk.ui8_sec_num,
                (int)pt_dev->t_blk.ui4_sec_size,
                (int)pt_dev->t_blk.ui4_max_trans_sec,
                (int)pt_dev->t_blk.ui4_max_io_req,
                (int)pt_dev->t_blk.ui4_align_bytes));

DONE:
 
    pt_xent->pt_dev = pt_dev;

    return FMR_OK;

FAIL:
    
    if (pt_dev != NULL)
    {
        fm_dev_close(pt_dev);
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: blkdev_read
 *
 * Description: Read data from the block device.
 *
 * Inputs:  pv_data
 *              Reference to the read argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 blkdev_read(VOID *pv_data)
{
    return _blkdev_read_write((FM_RW_ARG_T *)pv_data, TRUE);
}


/*-----------------------------------------------------------------------------
 * Name: blkdev_write
 *
 * Description: Write data to the block device.
 *
 * Inputs:  pv_data
 *              Reference to the write argument.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invaid device.
 *          FMR_EOF
 *              Cannot read over the device size.
 *          FMR_DEVICE_ERROR
 *              Get operation error.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 blkdev_write(VOID *pv_data)
{
    return _blkdev_read_write((FM_RW_ARG_T *)pv_data, FALSE);
}


/*-----------------------------------------------------------------------------
 * Name: blkdev_lseek
 *
 * Description: Change the current position of an opened block device.
 *
 * Inputs:  pv_data
 *              The reference to FM_LSEEK_ARG_T object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 ----------------------------------------------------------------------------*/
INT32 blkdev_lseek(VOID *pv_data)
{
    FM_LSEEK_ARG_T  *pt_arg;
    UINT64 ui8_dev_size;
    
    DEBUG_TRACE();

    ASSERT(pv_data != NULL);

    pt_arg = (FM_LSEEK_ARG_T *)pv_data;
    
    ASSERT(pt_arg->pt_xent != NULL);
    ASSERT(pt_arg->pt_xent->pt_dev != NULL);

    ui8_dev_size = pt_arg->pt_xent->pt_dev->ui8_space;
    
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
            ui8_new_off = pt_arg->i8_offset + ui8_dev_size;
        }

        if (ui8_new_off > ui8_dev_size)
        {
            ui8_new_off = ui8_dev_size;
        }

        pt_arg->ui8_cur_pos = ui8_new_off;
    }
    else
    {
        ASSERT(pt_arg->ui1_whence == FM_SEEK_END);

        if (pt_arg->i8_offset + (INT64)ui8_dev_size > 0)
        {
            pt_arg->ui8_cur_pos = pt_arg->i8_offset + ui8_dev_size;
        }
        else
        {
            pt_arg->ui8_cur_pos = 0;
        }
    }
    
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: blkdev_sync
 *
 * Description: Sync pending I/O.
 *
 * Inputs:  pv_data
 *              Reference to xentry of the block device.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
INT32 blkdev_sync(VOID *pv_data)
{
//    FM_XENTRY_T *pt_xent;
    INT32       i4_ret = FMR_OK;
    
//    ASSERT(pv_data != NULL);

//    pt_xent = (FM_XENTRY_T *)pv_data;

//    ASSERT(IS_XENTRY_LOCKED(pt_xent));
//    ASSERT(pt_xent->pt_dev != NULL);
    
//    i4_ret = fm_buf_flush(pt_xent, TRUE);
#if 0
    if (pt_xent->pt_dev->e_drv_type == DRVT_NAND_FLASH)
    {
        /*
         *  The flash has buffers to handle the erase condition.
         *  Need to flush them as well.
         */
        pt_xent->pt_dev->b_flush_dirty = TRUE;

        x_sema_lock(pt_xent->pt_dev->h_flash_wait_flush, X_SEMA_OPTION_WAIT);
        /* replaced by riccardo. {
        x_thread_resume(pt_xent->pt_dev->h_thread);
        */
        x_sema_unlock(pt_xent->pt_dev->h_ctrl_mtx);
        /* } */
        x_sema_lock(pt_xent->pt_dev->h_flash_wait_flush, X_SEMA_OPTION_WAIT);
        x_sema_unlock(pt_xent->pt_dev->h_flash_wait_flush);
    }
#endif
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: blkdev_close
 *
 * Description: Close an opened block device.
 *
 * Inputs:  pv_data
 *              Reference to xentry of the block device.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invalid pt_dev
 ----------------------------------------------------------------------------*/
INT32 blkdev_close(VOID *pv_data)
{
    FM_XENTRY_T     *pt_xent;
//    INT32           i4_ret;
    
    ASSERT(pv_data != NULL);

    pt_xent = (FM_XENTRY_T *)pv_data;

    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(pt_xent->pt_dev != NULL);

    /* Synchronously flush all dirty buffers before closing driver */
#if 0
    i4_ret = fm_buf_flush(pt_xent, TRUE);
    if (i4_ret != FMR_OK)
    {
        return i4_ret;
    }
#endif
    /*
     *  For memory card, the device may not present any more.
     *  Drop all cached buffers.
     *
     *  This is a quick solution, and the better way is to
     *  perform this operation by hotswap handling. In addition,
     *  not only memory card but also other hotswappable devices
     *  should perform this operation.
     */
#if 0
    fm_buf_reclaim(pt_xent, TRUE);
#endif

    return fm_dev_close(pt_xent->pt_dev);
}


