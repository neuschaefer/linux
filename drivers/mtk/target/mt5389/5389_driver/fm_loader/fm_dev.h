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
 * $RCSfile: fm_dev.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/17 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 10e7e856968f7491d10a51c4885c5897 $
 *
 * Description: 
 *          This header file contains types and function prototype for
 *          device access.
 *---------------------------------------------------------------------------*/

#ifndef _FM_DEV_H_
#define _FM_DEV_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_lnk_list.h"
#include "x_blk_dev.h"
#include "u_fm.h"
#include "x_rm.h"
#include "fm_util.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define FM_DEV_ALIGN_ADDR(addr, align) \
    (((align) == 0) ? (CHAR *)addr : (CHAR *)(((UINT32)addr + align) & ~(align - 1)))

#define FM_DEV_SYNC_READ    ((UINT8) 1)
#define FM_DEV_SYNC_WRITE   ((UINT8) 2)
#define FM_DEV_ASYNC_READ   ((UINT8) 4)
#define FM_DEV_ASYNC_WRITE  ((UINT8) 8)

#define FM_DEV_FLAG_WAIT    ((UINT16) 0x0001)
#define FM_DEV_FLAG_RUN     ((UINT16) 0x0002)
#define FM_DEV_FLAG_ABORT   ((UINT16) 0x0004)

typedef struct _FM_IO_REQ_T
{
    union
    {
        BLKDEV_IO_DATA_T            t_bio;
#if 0
        CHRDEV_LONG_DATA_INFO_T     t_cio_long;
        CHRDEV_SHORT_DATA_INFO_T    t_cio_short;
        BLKDEV_SECTOR_RANGE_INFO_T  t_brng;
        CHRDEV_SECTOR_RANGE_INFO_T  t_crng;
#endif
    } u_io;

    SIZE_T          z_len;
    UINT32          ui4_done_byte;
    UINT8           ui1_type;
#define FM_DEV_REQ_TYPE_BLK     ((UINT8) 0x01)
#define FM_DEV_REQ_TYPE_CHRL    ((UINT8) 0x02)
#define FM_DEV_REQ_TYPE_CHRS    ((UINT8) 0x04)
#define FM_DEV_REQ_TYPE_ERASE   ((UINT8) 0x08)
#define FM_DEV_REQ_DATA_MASK    ((UINT8) 0x0f)
#define FM_DEV_REQ_TYPE_GET     ((UINT8) 0x10)
#define FM_DEV_REQ_TYPE_SET     ((UINT8) 0x20)
#define FM_DEV_REQ_OP_MASK      ((UINT8) 0x30)
#define FM_DEV_REQ_READ         ((UINT8) 0x40)
#define FM_DEV_REQ_ABORT        ((UINT8)0x80)
    
    UINT16          ui2_hold_cnt;
    UINT32          ui4_op;
    FM_ASYNC_COND_T e_cond;
    UINT8           ui1_pri;
    VOID            *pv_tag;
    x_fm_async_fct  pf_nfy_fct;

    DLIST_ENTRY_T(_FM_IO_REQ_T) t_qlink;

#ifdef DEBUG_FM_DEV
    UINT16          ui2_state;
#define FM_DEV_REQ_INVAL ((UINT16) 0)
#define FM_DEV_REQ_PEND  ((UINT16) 1)
#define FM_DEV_REQ_BUSY  ((UINT16) 2)
#define FM_DEV_REQ_DONE  ((UINT16) 3)
#define FM_DEV_REQ_ABORT ((UINT16) 4)
#define FM_DEV_SET_REQ_STATE(req, state) \
        ((req)->ui2_state = (state))
#endif /* DEBUG_FM_DEV */

    HANDLE_T    h_rw_req;
    /* XXX, watch dog */
} FM_IO_REQ_T;

typedef enum
{
    FM_DEV_TYPE_INVAL = 0,
    FM_DEV_TYPE_BLK,
    FM_DEV_TYPE_CHR
} FM_DEV_TYPE_T;

struct _FM_DEVICE_T
{
    HANDLE_T        h_driver;
    DRV_TYPE_T      e_drv_type;
    UINT16          ui2_flags;
#define FM_DEV_FLAG_ASYNC ((UINT16) 0x0001)

    UINT16          ui2_unit;
    UINT16          ui2_ref_cnt;
    union
    {
        struct blk_param_t
        {
            UINT64 ui8_dev_size;
            UINT64 ui8_sec_num;
            UINT32 ui4_sec_size;
            UINT32 ui4_max_trans_sec;
            UINT32 ui4_max_io_req;
            UINT32 ui4_align_bytes;

            BLKDEV_NFY_INFO_T        t_nfy_info;
        } t_blkdev;
#if 0
        struct chr_param_t
        {
            UINT32 ui4_dev_size;
            UINT32 ui4_sec_num;
            UINT32 ui4_max_rsync;
            UINT32 ui4_max_wsync;

            CHRDEV_NFY_INFO_T        t_nfy_info;
        } t_chrdev;
#endif
    } u_param;
#define t_chr u_param.t_chrdev
#define t_blk u_param.t_blkdev

    UINT64          ui8_space;
    HANDLE_T        h_thread;
    HANDLE_T        h_async_thrd_mtx;
    UINT8           ui1_thread_state;
#define FM_DEV_THREAD_INIT      ((UINT8) 0)
#define FM_DEV_THREAD_RUN       ((UINT8) 1)
#define FM_DEV_THREAD_KILL      ((UINT8) 2)
#define FM_DEV_THREAD_OVER      ((UINT8) 3)

    HANDLE_T        h_done_msgq;
    HANDLE_T        h_queue_mtx;
    DLIST_T(_FM_IO_REQ_T) t_pend_req_q;
    DLIST_T(_FM_IO_REQ_T) t_busy_req_q;
    DLIST_T(_FM_IO_REQ_T) t_done_req_q;
    DLIST_T(_FM_IO_REQ_T) t_abort_req_q;
#ifdef DEBUG_FM_DEV
    UINT32          aui4_num[5];
#endif /* DEBUG_FM_DEV */

    SLIST_ENTRY_T(_FM_DEVICE_T) t_llink;
#if 0
    /* used for NOR/NAND Flash */
    FLASH_SECTOR_TBL_INFO_T *pt_sector_tbl;
    DLIST_T(_FLASH_SECTOR_CACHE_T) t_flash_cache; /* For flash I/O cache */
    HANDLE_T    h_flash_wait_flush;
    HANDLE_T    h_flash_wait_io;
#endif
    UINT32      ui4_cached_size;
    BOOL        b_flush_dirty;

    /* added by riccardo */
    HANDLE_T    h_ctrl_mtx;
};

typedef struct _FM_DEVICE_T FM_DEVICE_T;

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 fm_dev_open(
    CHAR            *ps_name,
    DRV_TYPE_T      e_drv_type,
    UINT16          ui2_unit,
    FM_DEVICE_T     **ppt_dev);
extern INT32 fm_dev_close(
    FM_DEVICE_T     *pt_dev);

extern INT32 fm_dev_send_req(
    FM_DEVICE_T     *pt_dev,
    FM_IO_REQ_T     *pt_new_req);

extern VOID fm_dev_nfy_fct(
    VOID        *pv_nfy_tag,
    INT32       e_nfy_cond,
    VOID        *pv_data);

extern INT32 fm_dev_init(VOID);

#endif /* _FM_DEV_H */
