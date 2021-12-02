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
 * $RCSfile: fm_buf.c,v $
 * $Revision: #2 $
 * $Date: 2012/06/11 $
 * $Author: yingjoe.chen $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/2 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 9136bddef0a6437bdc61af581c31d9d8 $
 *
 * Description: 
 *         This file implements the buffer cache system.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_handle.h"
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

#ifndef DEBUG_FM_BUF

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#endif /* DEBUG_FM_BUF */

typedef struct _FM_BUF_REQ_T
{
    FM_RW_ARG_T *pt_rw_req;
} FM_BUF_REQ_T;

typedef enum _FM_BUF_THREAD_STATE
{
    FM_BUF_THREAD_INIT = 0,
    FM_BUF_THREAD_RUN,
    FM_BUF_THREAD_KILL,
    FM_BUF_THREAD_OVER
} FM_BUF_THREAD_STATE;

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/
#ifdef DEBUG_FM_BUF
static UINT32 ui4_lru_cnt = 0;
#endif /* DEBUG_FM_BUF */

#if 0
static BOOL     b_inited        = FALSE;
static UINT32   ui4_free_spc    = FM_MAX_BUF_SPACE;
static UINT16   ui2_used_blk    = 0;
static UINT16   ui2_dirty_buf   = 0;
static HANDLE_T h_buf_cache_mtx = NULL_HANDLE;
static HANDLE_T h_buf_thrd_mtx  = NULL_HANDLE;
static HANDLE_T h_buf_thrd      = NULL_HANDLE;
static HANDLE_T h_buf_timer     = NULL_HANDLE;
static DLIST_T(_FM_BUF_T)       t_lru_buf_q;
static DLIST_T(_FM_BUF_T)       t_free_buf_q;
static DLIST_T(_FM_BUF_T)       t_dirty_buf_q;
static DLIST_T(_FM_BUF_T)       t_retry_buf_q;
static FM_BUF_THREAD_STATE      e_buf_thrd_state = FM_BUF_THREAD_INIT;
#endif

#ifdef DEBUG_FM_BUF

static BOOL b_cache_locked;

#define _fm_buf_lock_cache() \
    do { \
        x_sema_lock(h_buf_cache_mtx, X_SEMA_OPTION_WAIT); \
        b_cache_locked = TRUE; \
    } while (0)
    
#define _fm_buf_unlock_cache() \
    do { \
        b_cache_locked = FALSE; \
        x_sema_unlock(h_buf_cache_mtx); \
    } while (0)

static VOID _fm_buf_dump()
{
    FM_BUF_T *pt_buf;

    DBG_INFO(("[FM] Free buffer space:%d\n", (int)ui4_free_spc));
    DBG_INFO(("[FM] Dirty:\n"));
    DLIST_FOR_EACH(pt_buf, &t_dirty_buf_q, t_vlink)
    {
        DBG_INFO(("[FM] \t %s: %xh-%xh)\n",
                   pt_buf->pt_xent->ps_name,
                   (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                   (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size));
    }

    DBG_INFO(("[FM] Clean:\n"));
    DLIST_FOR_EACH(pt_buf, &t_lru_buf_q, t_vlink)
    {
        DBG_INFO(("[FM] \t %s: %xh-%xh)\n",
                   pt_buf->pt_xent->ps_name,
                   (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                   (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size));
    }
}

#else

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

UINT8 *pui1_fm_buffer;

#endif /* CC_MTK_LOADER */

#define _fm_buf_lock_cache() \
    x_sema_lock(h_buf_cache_mtx, X_SEMA_OPTION_WAIT)
    
#define _fm_buf_unlock_cache() \
    x_sema_unlock(h_buf_cache_mtx)

#endif /* DEBUG_FM_BUF */

#define _fm_buf_pause_buf_daemon() \
    x_sema_lock(h_buf_thrd_mtx, X_SEMA_OPTION_WAIT)
    
#define _fm_buf_resume_buf_daemon() \
    x_sema_unlock(h_buf_thrd_mtx)
#if 0
/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

static INT32 _fm_buf_lock_buf(
    FM_BUF_T        *pt_buf,
    SEMA_OPTION_T   e_opt);

static INT32 _fm_buf_unlock_buf(
    FM_BUF_T        *pt_buf);

static VOID _fm_buf_daemon_thread(
    VOID            *pv_data);

static VOID _fm_buf_wakeup_daemon(
    HANDLE_T        h_timer,
    VOID            *pv_tag);

static FM_BUF_T *_fm_buf_acquire(
    FM_XENTRY_T     *pt_xent,
    UINT64          ui8_lbn,
    UINT32          ui4_size);

static VOID _fm_buf_add_cache(
    FM_XENTRY_T     *pt_xent,
    FM_BUF_T        *pt_buf);

static VOID _fm_buf_release(
    FM_BUF_T        *pt_buf);

static INT32 _fm_buf_start_io(
    FM_BUF_T        *pt_buf,
    BOOL            b_read);

static INT32 _fm_buf_read_write_cache(
    FM_RW_ARG_T     *pt_rw_req,
    BOOL            b_read);

static INT32 _fm_buf_load_data(
    FM_BUF_T        *pt_buf);

static INT32 _fm_buf_flush_data(
    FM_BUF_T        *pt_buf);


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _fm_buf_lock_buf
 *
 * Description: Lock a buffer block.
 *
 * Inputs:  pt_buf
 *              Reference to the buffer block.
 *          e_opt
 *              Lock option.
 *
 * Outputs: -
 *
 * Returns: It returns the result of x_sema_lock().
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_lock_buf(
    FM_BUF_T        *pt_buf,
    SEMA_OPTION_T   e_opt)
{
    INT32    i4_ret;

    ASSERT(pt_buf != NULL);

    if (IS_BUF_INVAL(pt_buf))
    {
        return FMR_INVAL;
    }
    pt_buf->ui2_want++;

    i4_ret = x_sema_lock(pt_buf->h_mtx, e_opt);

    pt_buf->ui2_want--;

    if (i4_ret == OSR_WOULD_BLOCK)
    {
        return FMR_BUSY;
    }
    
    if (i4_ret != OSR_OK)
    {
        return FMR_CORE;
    }

    if (IS_BUF_INVAL(pt_buf))
    {
        _fm_buf_release(pt_buf);
        x_sema_unlock(pt_buf->h_mtx);

        return FMR_INVAL;
    }
    
    pt_buf->ui2_flags |= FM_BUF_FLAG_LOCKED;

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_unlock_buf
 *
 * Description: Unlock a buffer block.
 *
 * Inputs:  pt_buf
 *              Reference to the buffer block.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_unlock_buf(
    FM_BUF_T        *pt_buf)
{
    INT32 i4_ret;

    ASSERT(pt_buf != NULL);
    ASSERT(IS_BUF_LOCKED(pt_buf));

    pt_buf->ui2_flags &= ~FM_BUF_FLAG_LOCKED;

    i4_ret = x_sema_unlock(pt_buf->h_mtx);

    return (i4_ret == OSR_OK) ? FMR_OK : FMR_CORE;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_daemon_thread
 *
 * Description: Dirty buffer flush daemon. This daemon periodically flush
 *              dirty buffer blocks into device. The period is set to
 *              10 seconds by default, and it's defined by FM_BUF_FLUSH_TIME
 *              constant.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: Never return.
 ----------------------------------------------------------------------------*/
static VOID _fm_buf_daemon_thread(VOID *pv_data)
{
    INT32 i4_ret;

    e_buf_thrd_state = FM_BUF_THREAD_RUN;

    while (e_buf_thrd_state == FM_BUF_THREAD_RUN)
    {
        FM_BUF_T *pt_buf;

        x_thread_suspend();

        x_sema_lock(h_buf_thrd_mtx, X_SEMA_OPTION_WAIT);

        _fm_buf_lock_cache();

#ifdef DEBUG_FM_BUF
        _fm_buf_dump();
#endif /* DEBUG_FM_BUF */

        while ((pt_buf = DLIST_HEAD(&t_dirty_buf_q)) != NULL)
        {
            DLIST_REMOVE(pt_buf, &t_dirty_buf_q, t_vlink);
            ui2_dirty_buf--;

#ifdef DEBUG_FM_BUF
            DBG_INFO("[FM] Dirty buffers: %d\n", ui2_dirty_buf);
#endif /* DEBUG_FM_BUF */
            
            _fm_buf_unlock_cache();

            i4_ret = _fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_WAIT);
            
            /*
             *  pt_buf cannot be invalid, because fm_buf_reclaim
             *  cannot be running right now.
             */
            ASSERT(i4_ret != FMR_INVAL);

            if (i4_ret != FMR_OK)
            {
                /*
                 *  FMR_CORE. In such case, the only thing I can do
                 *  is to put this dirty buffer into the retry queue.
                 *  The buffers in the retry queue will be flushed
                 *  only when fm_buf_flush or fm_buf_reclaim is called.
                 */
                DBG_ERROR(("[FM] ERR: Fail to lock buffer: %s: %xh-%xh, error=%d\n",
                             pt_buf->pt_xent->ps_name,
                             (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                             (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size,
                             (int)i4_ret));
            }
            else
            {
                i4_ret = _fm_buf_flush_data(pt_buf);

                if (i4_ret != FMR_OK)
                {
                    DBG_ERROR(("[FM] ERR: Fail to flush buffer: %s: %xh-%xh, error=%d\n",
                                 pt_buf->pt_xent->ps_name,
                                 (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                                 (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size,
                                 (int)i4_ret));
                }
            }

            if (i4_ret != FMR_OK)
            {
                _fm_buf_lock_cache();
                pt_buf->ui2_flags |= FM_BUF_FLAG_RETRY;
                DLIST_INSERT_TAIL(pt_buf, &t_retry_buf_q, t_vlink);
                _fm_buf_unlock_cache();
            }

            _fm_buf_unlock_buf(pt_buf);

            _fm_buf_lock_cache();
        }

        _fm_buf_unlock_cache();

        x_sema_unlock(h_buf_thrd_mtx);
    }

    e_buf_thrd_state = FM_BUF_THREAD_OVER;
    
    x_thread_exit();
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_wakeup_daemon
 *
 * Description: Wakeup buffer flush daemon.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_buf_wakeup_daemon(
    HANDLE_T  h_timer,
    VOID      *pv_tag)
{
    if (h_buf_thrd != NULL_HANDLE)
    {
        x_thread_resume(h_buf_thrd);
    }
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_alloc_new
 *
 * Description: Allocate a new block and lock it.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: Reference to FM_BUF_T object.
 ----------------------------------------------------------------------------*/
static FM_BUF_T *_fm_buf_alloc_new(VOID)
{
    FM_BUF_T *pt_buf;

    pt_buf = x_mem_calloc(1, sizeof(FM_BUF_T));
    if (pt_buf == NULL)
    {
        DEBUG_TRACE();
        return NULL;
    }

    if (x_sema_create(&pt_buf->h_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }
    
    if (x_sema_create(&pt_buf->h_sync,
                      X_SEMA_TYPE_BINARY,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }
    
    if (_fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_WAIT) != FMR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }
    
    ui2_used_blk++;
    
    if (ui2_used_blk >= FM_MAX_BUF_BLOCK)
    {
        DBG_INFO(("[FM] WARNING! buffer block reaches maximum\n"));
    }

    return pt_buf;

FAIL:

    if (pt_buf->h_sync != NULL_HANDLE)
    {
        x_sema_delete(pt_buf->h_sync);
    }
    if (pt_buf->h_mtx != NULL_HANDLE)
    {
        x_sema_delete(pt_buf->h_mtx);
    }

    x_mem_free(pt_buf);

    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: _fm_buf_acquire
 *
 * Description: Acquire a locked buffer block with specified size.
 *              First, it tries to find a usable block in the free list.
 *              If it is not found and the total number of buffer blocks
 *              does not exceed the limit, then create a new one. Otherwise,
 *              try to get a block from the LRU list. If it still cannot
 *              find a buffer block to use, flush dirty buffers and try again.
 *
 * Inputs:  pt_xent
 *              Reference to xentry.
 *          ui8_lbn
 *              Logic block number to access in the file.
 *          ui4_size
 *              Size of this buffer. Must be a multiple of block size.
 *
 * Outputs: -
 *
 * Returns: Reference to FM_BUF_T object.
 ----------------------------------------------------------------------------*/
static FM_BUF_T *_fm_buf_acquire(
    FM_XENTRY_T *pt_xent,
    UINT64 ui8_lbn,
    UINT32 ui4_size)
{
    FM_BUF_T    *pt_buf = NULL;
    UINT32      ui4_align;

    ASSERT(pt_xent         != NULL);
    ASSERT(pt_xent->pt_dev != NULL);
    ASSERT((ui4_size % FM_BUF_BLOCK_SIZE) == 0);

    while (pt_buf == NULL)
    {
        _fm_buf_lock_cache();
        
        DLIST_FOR_EACH(pt_buf, &t_free_buf_q, t_vlink)
        {
            if (_fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_NOWAIT) == FMR_OK)
            {
                if ((pt_buf->ui4_size == 0) && (ui4_free_spc < ui4_size))
                {
                    _fm_buf_unlock_buf(pt_buf);
                    continue;
                }

                DLIST_REMOVE(pt_buf, &t_free_buf_q, t_vlink);
                break;
            }
        }

        if ((pt_buf == NULL) &&
            (ui2_used_blk < FM_MAX_BUF_BLOCK) &&
            (ui4_free_spc >= ui4_size))
        {
            pt_buf = _fm_buf_alloc_new();
        }

        if (pt_buf == NULL)
        {
            DLIST_FOR_EACH(pt_buf, &t_lru_buf_q, t_vlink)
            {
                if (_fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_NOWAIT) == FMR_OK)
                {
                    DLIST_REMOVE(pt_buf, &t_lru_buf_q, t_vlink);
                    break;
                }
            }
            
            if (pt_buf == NULL)
            {
                /*
                 *  Flush all dirty and try again.
                 */
                _fm_buf_unlock_cache();

                x_thread_resume(h_buf_thrd);

                x_thread_delay(1);
                
                continue;
            }

            ASSERT(pt_buf->pt_xent != NULL);
            DLIST_REMOVE(pt_buf, &pt_buf->pt_xent->t_buf_list, t_hlink);
        }

        ASSERT(IS_BUF_LOCKED(pt_buf));

        ui4_align = (IS_XENTRY_BLK(pt_xent)) ? pt_xent->pt_dev->t_blk.ui4_align_bytes : 0;
        
        if ((pt_buf->ui4_size != ui4_size) || (pt_buf->ui4_align < ui4_align))
        {
            if (pt_buf->pv_alloc != NULL)
            {
                ui4_free_spc += pt_buf->ui4_size;
                x_mem_free(pt_buf->pv_alloc);
                pt_buf->pv_alloc = NULL;
                pt_buf->ui4_size = 0;
                pt_buf->ui4_align = 0;
            }

            if (ui4_free_spc >= ui4_size)
            {
                pt_buf->pv_alloc = x_mem_alloc(ui4_size + ui4_align);
            }

            if ((ui4_free_spc < ui4_size) || (pt_buf->pv_alloc == NULL))
            {
                /*
                 *  Not enough buffer space, flush dirties and try again.
                 */
                DLIST_INSERT_TAIL(pt_buf, &t_free_buf_q, t_vlink);

                _fm_buf_unlock_cache();
                _fm_buf_unlock_buf(pt_buf);

                pt_buf = NULL;

                x_thread_resume(h_buf_thrd);

                x_thread_delay(100);
                
                continue;
            }

            ui4_free_spc -= ui4_size;

            /*
             *  The alignment must be power of 2.
             */
            pt_buf->pv_data   = FM_DEV_ALIGN_ADDR(pt_buf->pv_alloc, ui4_align);
            pt_buf->ui4_size  = ui4_size;
            pt_buf->ui4_align = ui4_align;
        }

        _fm_buf_unlock_cache();

        ASSERT(IS_XENTRY_BLK(pt_xent) || IS_XENTRY_CHR(pt_xent));
        pt_buf->pt_xent      = pt_xent;
        pt_buf->ui8_lbn      = ui8_lbn;
        pt_buf->ui4_sec_size = FM_BUF_BLOCK_SIZE;
        pt_buf->pt_rw_req    = NULL;
    }

    return pt_buf;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_add_cache
 *
 * Description: Add a buffer block to associated xentry. The buffer cache list
 *              of an xentry is sorted by logic block number in ascending
 *              order.
 *
 * Inputs:  pt_xent
 *              Reference to xentry.
 *          pt_buf
 *              Reference to buffer block.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_buf_add_cache(
    FM_XENTRY_T *pt_xent,
    FM_BUF_T    *pt_buf)
{
    ASSERT(pt_xent != NULL);
    ASSERT(pt_buf  != NULL);
#if 0
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
#endif
    ASSERT(IS_BUF_LOCKED(pt_buf));

    if (DLIST_IS_EMPTY(&pt_xent->t_buf_list))
    {
        DLIST_INSERT_HEAD(pt_buf, &pt_xent->t_buf_list, t_hlink);
    }
    else 
    {
        FM_BUF_T *pt_next_buf, *pt_last_buf;

        DLIST_FOR_EACH(pt_next_buf, &pt_xent->t_buf_list, t_hlink)
        {
            ASSERT(pt_buf->ui8_lbn != pt_next_buf->ui8_lbn);

            pt_last_buf = pt_next_buf;

            if (pt_buf->ui8_lbn < pt_next_buf->ui8_lbn)
            {
                DLIST_INSERT_BEFORE(pt_buf, pt_next_buf, &pt_xent->t_buf_list, t_hlink);
                break;
            }
        }
        if (pt_next_buf == NULL)
        {
            DLIST_INSERT_AFTER(pt_buf, pt_last_buf, &pt_xent->t_buf_list, t_hlink);
        }
    }

#ifdef DEBUG_FM_BUF
    ui4_lru_cnt++;
#endif /* DEBUG_FM_BUF */

}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_release
 *
 * Description: Release the buffer block space and put it back to the free
 *              list.
 *
 * Inputs:  pt_buf
 *              Reference to buffer block.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_buf_release(
    FM_BUF_T    *pt_buf)
{
    ASSERT(pt_buf != NULL);
    ASSERT(IS_BUF_LOCKED(pt_buf));
    
    pt_buf->ui2_flags |= FM_BUF_FLAG_INVAL;
    if (pt_buf->ui2_want == 0)
    {
        _fm_buf_lock_cache();

        ui4_free_spc += pt_buf->ui4_size;
        x_mem_free(pt_buf->pv_alloc);
        pt_buf->pv_alloc   = NULL;
        pt_buf->ui4_size   = 0;
        pt_buf->ui2_flags &= ~(FM_BUF_FLAG_INVAL|FM_BUF_FLAG_DROP|FM_BUF_FLAG_IO);
        DLIST_INSERT_TAIL(pt_buf, &t_free_buf_q, t_vlink);

        _fm_buf_unlock_cache();
    }
    else
    {
         DBG_INFO(("[FM] buffer: %s: %xh-%xh is wanted\n",
                    pt_buf->pt_xent->ps_name,
                    (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                    (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size));
    }
}


/*-----------------------------------------------------------------------------
 * Name: _buf_read_write
 *
 * Description: Synchronous/Asynchronous read/write a buffer.
 *
 * Inputs: pt_buf
 *          Reference to the buffer.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Otherwise
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _buf_read_write(
    FM_BUF_T        *pt_buf)
{
    FM_RW_ARG_T t_io;
    INT32       i4_ret;

    ASSERT(pt_buf != NULL);
    ASSERT(pt_buf->pt_xent != NULL);
    ASSERT(pt_buf->pv_data != NULL);
    ASSERT(pt_buf->pt_xent->pt_dev != NULL);
    
    t_io.pt_xent        = pt_buf->pt_xent;
    t_io.pv_data        = pt_buf->pv_data;
    t_io.ui4_count      = pt_buf->ui4_size;
    t_io.ui8_offset     = FM_BUF_ADDR(pt_buf->ui8_lbn);
    t_io.ui4_done_bytes = 0;

    if ((t_io.ui8_offset + (UINT64)t_io.ui4_count) >
        pt_buf->pt_xent->pt_dev->ui8_space)
    {
        t_io.ui4_count = (UINT32)(pt_buf->pt_xent->pt_dev->ui8_space - 
                                  t_io.ui8_offset);
    }

    if ((pt_buf->ui2_flags & FM_BUF_FLAG_ASYNC) == 0)
    {
        t_io.ui2_flag   = FM_IO_SYNC|FM_IO_DIRECT;
        t_io.h_bin_sema = pt_buf->h_sync;
    }
    else
    {
        if (pt_buf->ui2_flags & FM_BUF_FLAG_IO_DONE)
        {
            /*
             *  For asynchronous read/write the data may have
             *  been ready in the buffer and hence no need to
             *  perform any I/O. We just let the device's I/O
             *  thread to send the notification of the I/O
             *  completion.
             */
            t_io.ui4_done_bytes = pt_buf->ui4_size;
        }

        t_io.ui2_flag   = FM_IO_DIRECT;
        t_io.ui1_pri    = pt_buf->ui1_pri;
        t_io.pf_nfy_fct = fm_buf_done_nfy;
        t_io.pv_tag     = pt_buf;
    }

    if (pt_buf->ui2_flags & FM_BUF_FLAG_READ)
    {
        ASSERT(pt_buf->pt_xent->pt_xent_ops != NULL);
        ASSERT(pt_buf->pt_xent->pt_xent_ops->pf_read != NULL);

        i4_ret = XENTRY_OP_READ(pt_buf->pt_xent)(&t_io);
    }
    else
    {
        ASSERT(pt_buf->pt_xent->pt_xent_ops != NULL);
        ASSERT(pt_buf->pt_xent->pt_xent_ops->pf_write != NULL);

        i4_ret = XENTRY_OP_WRITE(pt_buf->pt_xent)(&t_io);
    }

    if (((pt_buf->ui2_flags & FM_BUF_FLAG_ASYNC) == 0) &&
        (i4_ret == FMR_OK))
    {
        ASSERT(t_io.ui4_done_bytes == t_io.ui4_count);
        
        pt_buf->ui4_resid = 0;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_load_data
 *
 * Description: Load data from device to buffer.
 *
 * Inputs: pt_buf
 *              Reference to buffer block
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_load_data(
    FM_BUF_T    *pt_buf)
{
    ASSERT(pt_buf != NULL);
    ASSERT(IS_BUF_LOCKED(pt_buf));

    pt_buf->ui2_flags |= (FM_BUF_FLAG_READ | FM_BUF_FLAG_IO);
    pt_buf->ui4_resid = pt_buf->ui4_size;

    return _buf_read_write(pt_buf);
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_flush_data
 *
 * Description: Synchronous flush data from buffer to device. pt_buf must
 *              be dirty. It will be locked and removed from the dirty queue.
 *
 * Inputs:  pt_buf
 *              Reference to a dirty buffer.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_flush_data(
    FM_BUF_T    *pt_buf)
{
    INT32 i4_ret;

    ASSERT(pt_buf != NULL);
    ASSERT(IS_BUF_DIRTY(pt_buf));
    ASSERT(IS_BUF_LOCKED(pt_buf));

    pt_buf->ui2_flags |= FM_BUF_FLAG_IO;
    pt_buf->ui2_flags &= ~(FM_BUF_FLAG_READ | FM_BUF_FLAG_ASYNC);
    pt_buf->ui4_resid = pt_buf->ui4_size;

    i4_ret = _buf_read_write(pt_buf);

    if (i4_ret != FMR_OK)
    {
        pt_buf->ui2_flags &= ~FM_BUF_FLAG_IO;
        return i4_ret;
    }

    if (pt_buf->ui2_flags & FM_BUF_FLAG_DROP)
    {
        _fm_buf_release(pt_buf);
    }
    else
    {
        /* Add this buffer to lru list. */

        _fm_buf_lock_cache();

        pt_buf->ui2_flags &= ~(FM_BUF_FLAG_IO | FM_BUF_FLAG_DIRTY);
        DLIST_INSERT_TAIL(pt_buf, &t_lru_buf_q, t_vlink);

        _fm_buf_unlock_cache();
    }

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_start_io
 *
 * Description: Start to read/write a buffer synchronously/asynchronously.
 *
 * Inputs:  pt_buf
 *              Reference to the buffer to perform I/O.
 *          b_read
 *              TRUE for read, and FALSE for write.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_start_io(
    FM_BUF_T    *pt_buf,
    BOOL        b_read)
{
    FM_RW_ARG_T *pt_rw_req;
    INT32 i4_ret = FMR_OK;

    ASSERT(pt_buf != NULL);
    ASSERT(pt_buf->pt_rw_req != NULL);

    pt_rw_req = pt_buf->pt_rw_req;

    if (b_read == TRUE)
    {
        if (pt_rw_req->ui2_flag & FM_IO_SYNC)
        {
            pt_buf->ui2_flags &= ~FM_BUF_FLAG_ASYNC;
            i4_ret = _fm_buf_load_data(pt_buf);
            if (i4_ret == FMR_OK)
            {
                x_memcpy(pt_buf->pv_copy_addr,
                         (CHAR *)pt_buf->pv_data + pt_buf->ui4_copy_off,
                         pt_buf->ui4_copy_size);

                pt_buf->ui2_flags &= ~(FM_BUF_FLAG_IO | FM_BUF_FLAG_DIRTY);
                        
                _fm_buf_lock_cache();
                DLIST_INSERT_TAIL(pt_buf, &t_lru_buf_q, t_vlink);
                _fm_buf_unlock_cache();

                _fm_buf_unlock_buf(pt_buf);

                pt_rw_req->ui4_done_bytes += pt_buf->ui4_copy_size;
            }
        }
        else
        {
            pt_buf->ui2_flags |= FM_BUF_FLAG_ASYNC;
            pt_buf->ui2_flags &= ~FM_BUF_FLAG_COPY_IN;
            i4_ret = _fm_buf_load_data(pt_buf);
        }
    }
    else
    {
        if (pt_rw_req->ui2_flag & FM_IO_SYNC)
        {
            if (pt_buf->ui4_copy_size != pt_buf->ui4_size)
            {
                pt_buf->ui2_flags &= ~FM_BUF_FLAG_ASYNC;
                i4_ret = _fm_buf_load_data(pt_buf);
            }
            if (i4_ret == FMR_OK)
            {
                x_memcpy((CHAR *)pt_buf->pv_data + pt_buf->ui4_copy_off,
                         pt_buf->pv_copy_addr,
                         pt_buf->ui4_copy_size);

                pt_buf->ui2_flags &= ~FM_BUF_FLAG_IO;
                pt_buf->ui2_flags |= FM_BUF_FLAG_DIRTY;

                _fm_buf_lock_cache();

                DLIST_INSERT_TAIL(pt_buf, &t_dirty_buf_q, t_vlink);
                ui2_dirty_buf++;

                _fm_buf_unlock_cache();

                _fm_buf_unlock_buf(pt_buf);

                pt_rw_req->ui4_done_bytes += pt_buf->ui4_copy_size;
            }
        }
        else
        {
            pt_buf->ui2_flags |= (FM_BUF_FLAG_ASYNC | FM_BUF_FLAG_COPY_IN);
            i4_ret = _fm_buf_load_data(pt_buf);
        }
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_read_write_cache
 *
 * Description: Perform read/write operation. For read operation, if the 
 *              desired data hits the cache, then just copy from cache
 *              to specified address. If it misses, allocate a new buffer
 *              block and issue a device read operation. For write operation,
 *              it either updates the cache content or allocate a new
 *              buffer block and issue a write operation.
 *
 * Inputs:  pt_rw_req
 *              Reference to FM_RW_ARG_t object.
 *          b_read
 *              To indicate if it is a read or write operation.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
static INT32 _fm_buf_read_write_cache(
    FM_RW_ARG_T *pt_rw_req,
    BOOL        b_read)
{
    FM_XENTRY_T *pt_xent;
    FM_BUF_T    *pt_buf;
    FM_BUF_T    *pt_new_buf;
    UINT32      ui4_new_size;
    UINT32      ui4_resid;
    UINT64      ui8_data_laddr;
    UINT64      ui8_data_llbn;
    UINT64      ui8_data_rlbn;
    UINT64      ui8_buf_llbn;
    UINT64      ui8_buf_rlbn;
    CHAR        *pc_data;
    SIZE_T      z_copy;
    INT32       i4_ret;
    
    ASSERT(pt_rw_req != NULL);
    ASSERT(pt_rw_req->pt_xent != NULL);
    ASSERT(pt_rw_req->pv_data != NULL);

    pc_data        = pt_rw_req->pv_data;
    pt_xent        = pt_rw_req->pt_xent;
    ui4_resid      = pt_rw_req->ui4_count;
    ui8_data_laddr = pt_rw_req->ui8_offset;
    ui8_data_rlbn  = FM_BUF_LBN(ui8_data_laddr + ui4_resid - 1) + 1;
    ui8_data_llbn  = FM_BUF_LBN(ui8_data_laddr);
    ui8_buf_llbn   = 0;

    ASSERT(IS_XENTRY_LOCKED(pt_xent));

    /*
     *  case 0:
     *          buf
     *          data    |======|
     *  case 1:
     *          buf     |******|
     *          data      |==|
     *  case 2:
     *          buf     |******|
     *          data       |======|
     *  case 3:
     *          buf     |******|
     *          data              |=====|
     *  case 4:
     *          buf        |****|
     *          data    |==========|
     *  case 5:
     *          buf       |******|
     *          data    |======|
     *  case 6
     *          buf           |***|
     *          data    |===|
     */

    /*
     *  Check if it could be found in the cache list.
     */
    pt_buf = DLIST_TAIL(&pt_xent->t_buf_list);
    if (pt_buf == NULL)
    {
        goto SKIP_SEEK;
    }
    if (ui8_data_llbn >= (pt_buf->ui8_lbn + (pt_buf->ui4_size / FM_BUF_BLOCK_SIZE)))
    {
        pt_buf = NULL;
        goto SKIP_SEEK;
    }

    pt_buf = DLIST_HEAD(&pt_xent->t_buf_list);
    if (pt_buf != NULL)
    {
        if (pt_buf->ui8_lbn >= ui8_data_rlbn)
        {
            ui8_buf_llbn = pt_buf->ui8_lbn; /* used for read-ahead */
            goto SKIP_SEEK;
        }
    }

    /*
     *  Start seek in the cache list.
     */
    DLIST_FOR_EACH(pt_buf, &pt_xent->t_buf_list, t_hlink)
    {
        ui8_buf_llbn = pt_buf->ui8_lbn;
        ui8_buf_rlbn = pt_buf->ui8_lbn + (pt_buf->ui4_size / FM_BUF_BLOCK_SIZE);
        ui8_data_llbn  = FM_BUF_LBN(ui8_data_laddr);
        if (ui8_data_llbn >= ui8_buf_llbn)
        {
            if (ui8_data_llbn < ui8_buf_rlbn)
            {
                /* case 1, case 2 */

                if ((i4_ret = _fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_WAIT)) != FMR_OK)
                {
                    return i4_ret;
                }
                
                z_copy = (SIZE_T)((ui8_data_rlbn <= ui8_buf_rlbn) ?
                                  ui4_resid :
                                  FM_BUF_ADDR(ui8_buf_rlbn) - ui8_data_laddr);

                if (pt_rw_req->ui2_flag & FM_IO_SYNC)
                {
                    if (b_read)
                    {
                        x_memcpy(pc_data,
                                 (CHAR *)pt_buf->pv_data + (ui8_data_laddr - FM_BUF_ADDR(pt_buf->ui8_lbn)),
                                 z_copy);

                        if (!IS_BUF_DIRTY(pt_buf))
                        {
                            _fm_buf_lock_cache();
                    
                            /* move to tail to perform the lru behavior */
                            DLIST_REMOVE(pt_buf, &t_lru_buf_q, t_vlink);
                            DLIST_INSERT_TAIL(pt_buf, &t_lru_buf_q, t_vlink);

                            _fm_buf_unlock_cache();
                        }
                    }
                    else
                    {
                        x_memcpy((CHAR *)pt_buf->pv_data + (ui8_data_laddr - FM_BUF_ADDR(pt_buf->ui8_lbn)),
                                 pc_data,
                                 z_copy);
                        if (!IS_BUF_DIRTY(pt_buf))
                        {
                            _fm_buf_lock_cache();

                            DLIST_REMOVE(pt_buf, &t_lru_buf_q, t_vlink);
                            pt_buf->ui2_flags |= FM_BUF_FLAG_DIRTY;
                            DLIST_INSERT_TAIL(pt_buf, &t_dirty_buf_q, t_vlink);
                            ui2_dirty_buf++;

                            _fm_buf_unlock_cache();
                        }
                    }

                    _fm_buf_unlock_buf(pt_buf);

                    pt_rw_req->ui4_done_bytes += z_copy;
                }
                else
                {
                    /* 
                     *  Async read/write, the required data is covered
                     *  by the cache, no need to perform any I/O then.
                     *  We mark this buffer as IO_DONE, and issue it
                     *  to the underneath device so that the device
                     *  I/O thread can invoke the callback later on.
                     */
                    _fm_buf_lock_cache();

                    if (IS_BUF_DIRTY(pt_buf))
                    {
                        DLIST_REMOVE(pt_buf, &t_dirty_buf_q, t_vlink);
                        ui2_dirty_buf--;
                    }
                    else
                    {
                        DLIST_REMOVE(pt_buf, &t_lru_buf_q, t_vlink);
                    }

                    _fm_buf_unlock_cache();

                    pt_buf->ui2_flags    |= FM_BUF_FLAG_IO_DONE;
                    pt_buf->ui1_pri       = pt_rw_req->ui1_pri;
                    pt_buf->pt_rw_req     = pt_rw_req;
                    pt_buf->pv_copy_addr  = pc_data;
                    pt_buf->ui4_copy_off  = ui8_data_laddr - FM_BUF_ADDR(pt_buf->ui8_lbn);
                    pt_buf->ui4_copy_size = z_copy;

                    i4_ret = _fm_buf_start_io(pt_buf, b_read);
                    if (i4_ret != FMR_OK)
                    {
                        _fm_buf_unlock_buf(pt_buf);
                        goto FAIL;
                    }
                }

                pc_data        += z_copy;
                ui8_data_laddr += z_copy;
                ui4_resid      -= z_copy;
            }
            else
            {
                /* case 3 */
                
                /* do nothing, check next buffer */
            }
        }
        else
        {
            if (ui8_data_rlbn > ui8_buf_llbn)
            {
                /* case 4, case 5 */
                
                ui4_new_size = (UINT32)(ui8_buf_llbn - ui8_data_llbn) << FM_BUF_BLOCK_SHIFT;
                if (ui4_new_size > FM_BUF_CHUNK_SIZE)
                {
                    ui4_new_size = FM_BUF_CHUNK_SIZE;
                }

                z_copy = ui4_new_size - FM_BUF_OFFSET(ui8_data_laddr);

                pt_new_buf = _fm_buf_acquire(pt_xent, ui8_data_llbn, ui4_new_size);
                if (pt_new_buf == NULL)
                {
                    i4_ret = FMR_CORE;
                    goto FAIL;
                }

                _fm_buf_add_cache(pt_xent, pt_new_buf);

                if (((pt_rw_req->ui2_flag & FM_IO_SYNC) == 0) &&
                    (b_read == FALSE) &&
                    (z_copy == ui4_new_size))
                {
                    /*
                     *  The size to write will cover the entire buffer,
                     *  therefore no need to perform an extra read operation.
                     *  Set the IO_DONE.
                     */
                    pt_new_buf->ui2_flags |= FM_BUF_FLAG_IO_DONE;
                }
                
                pt_new_buf->ui1_pri       = pt_rw_req->ui1_pri;
                pt_new_buf->pt_rw_req     = pt_rw_req;
                pt_new_buf->pv_copy_addr  = pc_data;
                pt_new_buf->ui4_copy_off  = FM_BUF_OFFSET(ui8_data_laddr);
                pt_new_buf->ui4_copy_size = z_copy;

                i4_ret = _fm_buf_start_io(pt_new_buf, b_read);
                if (i4_ret != FMR_OK)
                {
                    DLIST_REMOVE(pt_new_buf, &pt_xent->t_buf_list, t_hlink);
                    _fm_buf_release(pt_new_buf);
                    _fm_buf_unlock_buf(pt_new_buf);
                    goto FAIL;
                }
                
                pc_data        += z_copy;
                ui8_data_laddr += z_copy;
                ui4_resid      -= z_copy;

                /*
                 *  pt_new_buf had been inserted, change pt_buf
                 *  so that it will use this pt_buf in next iteration.
                 */
                pt_buf = pt_new_buf;
            }
            else
            {
                /* case 6 */
                break;
            }
        }

        if (ui4_resid == 0)
        {
            break;
        }
    }

SKIP_SEEK:

    while (ui4_resid != 0)
    {
        ui8_data_llbn = FM_BUF_LBN(ui8_data_laddr);
        ui4_new_size  = (UINT32)(ui8_data_rlbn - ui8_data_llbn) * FM_BUF_BLOCK_SIZE;
        
        if (ui4_new_size > FM_BUF_CHUNK_SIZE)
        {
            ui4_new_size = FM_BUF_CHUNK_SIZE;
            z_copy = FM_BUF_CHUNK_SIZE - FM_BUF_OFFSET(ui8_data_laddr);
        }
        else
        {
            UINT64 ui8_new_size;

            if (pt_buf == NULL)
            {
                /* we're in the tail of buffer chain, perform read-ahead if possible */

                ROOTFS_ENTRY_T *pt_rent;
                UINT64  ui8_partition_end ;

                pt_rent = ROOTFS_GET_ENT(pt_xent);

                if (FM_MODE_DEV_VIRT ==  (pt_rent->ui4_mode & FM_MODE_DEV_MASK))
                {
                    if (IS_XENTRY_BLK(pt_xent))
                    {
                        ui8_partition_end = ((UINT64)(pt_rent->u.pt_part->ui4_sectors +
                                             pt_rent->u.pt_part->ui4_start_lba)) * 
                                             pt_xent->pt_dev->u_param.t_blkdev.ui4_sec_size;
                    }
                    else
                    {
                        ui8_partition_end = ((UINT64)(pt_rent->u.pt_part->ui4_sectors +
                                             pt_rent->u.pt_part->ui4_start_lba)) * 512;
                    }
                    
                    ui8_new_size = ui8_partition_end - FM_BUF_ADDR(ui8_data_llbn);
                }
                else
                {
                    ui8_new_size = pt_xent->pt_dev->ui8_space - FM_BUF_ADDR(ui8_data_llbn);
                }

                /* Align to 512 bytes */
                if ((ui8_new_size & ((UINT64)(FM_BUF_BLOCK_SIZE - 1))) != 0)
                {
                    ui8_new_size = (ui8_new_size + FM_BUF_BLOCK_SIZE) >> FM_BUF_BLOCK_SHIFT;
                    ui8_new_size <<= FM_BUF_BLOCK_SHIFT;
                }
            }
            else
            {
                /* case 6 */
                /* we can perform read-ahead as well, but notice that
                 * the it cannot overlap the next buffer in chain */
                ui8_new_size = (ui8_buf_llbn - ui8_data_llbn) * FM_BUF_BLOCK_SIZE;
            }
            if (ui8_new_size > FM_BUF_CHUNK_SIZE)
            {
                ui4_new_size = FM_BUF_CHUNK_SIZE;
            }
            else
            {
                ui4_new_size = (UINT32)ui8_new_size;
            }
            
            z_copy = (SIZE_T)ui4_resid;
        }

        ASSERT(ui4_new_size >= z_copy);

        pt_new_buf = _fm_buf_acquire(pt_xent, ui8_data_llbn, ui4_new_size);
        if (pt_new_buf == NULL)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }

        _fm_buf_add_cache(pt_xent, pt_new_buf);

        if (((pt_rw_req->ui2_flag & FM_IO_SYNC) == 0) &&
            (b_read == FALSE) &&
            (z_copy == ui4_new_size))
        {
            /*
             *  The size to write will cover the entire buffer,
             *  therefore no need to perform an extra read operation.
             *  Set the IO_DONE.
             */
            pt_new_buf->ui2_flags |= FM_BUF_FLAG_IO_DONE;
        }

        pt_new_buf->ui1_pri       = pt_rw_req->ui1_pri;
        pt_new_buf->pt_rw_req     = pt_rw_req;
        pt_new_buf->pv_copy_addr  = pc_data;
        pt_new_buf->ui4_copy_off  = FM_BUF_OFFSET(ui8_data_laddr);
        pt_new_buf->ui4_copy_size = z_copy;

        i4_ret = _fm_buf_start_io(pt_new_buf, b_read);
        if (i4_ret != FMR_OK)
        {
            DLIST_REMOVE(pt_new_buf, &pt_xent->t_buf_list, t_hlink);
            _fm_buf_release(pt_new_buf);
            _fm_buf_unlock_buf(pt_new_buf);
            goto FAIL;
        }

        pc_data        += z_copy;
        ui8_data_laddr += z_copy;
        ui4_resid      -= z_copy;
    }

    
    if (pt_rw_req->ui2_flag & FM_IO_SYNC)
    {
        x_sema_unlock(pt_rw_req->h_bin_sema);
    }

    return FMR_OK;

FAIL:

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _fm_buf_done_nfy
 *
 * Description: Callback while I/O request is done.
 *
 * Inputs:  pv_tag
 *              Referecne to buffer block.
 *          e_async_cond
 *              I/O complete condition.
 *          ui4_data
 *              Number of done bytes.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
VOID fm_buf_done_nfy(
    HANDLE_T    h_req,
    VOID            *pv_tag,
    FM_ASYNC_COND_T e_async_cond,
    UINT32          ui4_data)
{
    FM_BUF_T        *pt_buf;
    FM_RW_ARG_T     *pt_rw_req;
    UINT32          ui4_copy_size;
    BOOL            b_dirty = FALSE;
    
    ASSERT(pv_tag != NULL);
    
    pt_buf        = (FM_BUF_T *)pv_tag;
    pt_rw_req     = pt_buf->pt_rw_req;
    ui4_copy_size = pt_buf->ui4_copy_size;
    
    ASSERT(IS_BUF_LOCKED(pt_buf));
    ASSERT(pt_buf->ui2_flags & FM_BUF_FLAG_ASYNC);
    ASSERT(ui4_data <= pt_buf->ui4_size);
    ASSERT(ui4_data <= pt_buf->ui4_resid);

    pt_buf->ui4_resid -= ui4_data;
    
    if (pt_buf->pv_copy_addr != NULL)
    {
        if (pt_buf->ui2_flags & FM_BUF_FLAG_COPY_IN)
        {
            /* Copy data in to buffer cache */
            x_memcpy((CHAR *)pt_buf->pv_data + pt_buf->ui4_copy_off,
                     pt_buf->pv_copy_addr,
                     ui4_copy_size);
            b_dirty = TRUE;
        }
        else
        {
            /* Copy data out from buffer cache */ 
            x_memcpy(pt_buf->pv_copy_addr,
                     (CHAR *)pt_buf->pv_data + pt_buf->ui4_copy_off,
                     ui4_copy_size);
        }
    }

    if (pt_buf->ui4_resid == 0)
    {
        if (pt_buf->ui2_flags & FM_BUF_FLAG_DROP)
        {
            _fm_buf_release(pt_buf);
        }
        else
        {
            _fm_buf_lock_cache();

            pt_buf->ui2_flags &= ~FM_BUF_FLAG_IO;
            /*
             *  If the buffer is dirty (just updated), put it in dirty list.
             *  Else if it is an IO_DONE buffer (no I/O is performed) and
             *  it was originally dirty, put it back in dirty list.
             */
            if ((b_dirty) ||
                ((pt_buf->ui2_flags & FM_BUF_FLAG_IO_DONE) && (IS_BUF_DIRTY(pt_buf))))
            {
                pt_buf->ui2_flags |= FM_BUF_FLAG_DIRTY;
                DLIST_INSERT_TAIL(pt_buf, &t_dirty_buf_q, t_vlink);
                ui2_dirty_buf++;
            }
            else
            {
                pt_buf->ui2_flags &= ~FM_BUF_FLAG_DIRTY;
                DLIST_INSERT_TAIL(pt_buf, &t_lru_buf_q, t_vlink);
            }

            _fm_buf_unlock_cache();
        }

        _fm_buf_unlock_buf(pt_buf);
    }

    if (pt_rw_req != NULL)
    {
        pt_rw_req->ui4_done_bytes += ui4_copy_size;

        if (pt_rw_req->ui4_done_bytes == pt_rw_req->ui4_count)
        {
            if (pt_rw_req->ui2_flag & FM_IO_SYNC)
            {
                x_sema_unlock(pt_rw_req->h_bin_sema);
            }
            else if (pt_rw_req->pf_nfy_fct != NULL)
            {
                if (e_async_cond != FM_ASYNC_COND_FAIL)
                {
                    if (pt_buf->ui2_flags & FM_BUF_FLAG_COPY_IN)
                    {
                        e_async_cond = FM_ASYNC_COND_WRITE_OK;
                    }
                    else
                    {
                        e_async_cond = FM_ASYNC_COND_READ_OK;
                    }
                }
                
                pt_rw_req->pf_nfy_fct(pt_rw_req->h_rw_req,
                                      pt_rw_req->pv_tag,
                                      e_async_cond,
                                      pt_rw_req->ui4_count);
                x_mem_free(pt_rw_req);
            }
        }
    }
}

#endif
/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: fm_buf_read_write
 *
 * Description: Read/write function for upper layer file system.
 *
 * Inputs:  pt_rw_req
 *              Reference to FM_RW_ARG_T object.
 *          b_read
 *              Indicate if it is read or write operation.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
INT32 fm_buf_read_write(
    FM_RW_ARG_T     *pt_rw_req,
    BOOL            b_read)
{
    FM_RW_ARG_T     *pt_new_req;
    INT32           i4_ret = FMR_OK;
    ROOTFS_ENTRY_T  *pt_rent;
    UINT32 ui4_ofst_shift;

    ASSERT(pt_rw_req != NULL);
    ASSERT(pt_rw_req->pt_xent != NULL);
    ASSERT(pt_rw_req->pt_xent->pt_dev != NULL);
    ASSERT(pt_rw_req->pt_xent->pt_dev->h_driver != NULL_HANDLE);
    ASSERT(pt_rw_req->ui4_done_bytes == 0);
    ASSERT(pt_rw_req->pt_xent->pt_dev->ui8_space > pt_rw_req->ui8_offset);

    if ((pt_rw_req->ui2_flag & FM_IO_SYNC) == 0)
    {
#if 0 // [5391 loader]
        pt_new_req = x_mem_alloc(sizeof(FM_RW_ARG_T));
        if (pt_new_req == NULL)
        {
            return FMR_CORE;
        }
        *pt_new_req = *pt_rw_req;

        pt_rw_req = pt_new_req;
#else
        ASSERT(0);
#endif
    }

    pt_rent = ROOTFS_GET_ENT(pt_rw_req->pt_xent);

    if (IS_XENTRY_BLK(pt_rw_req->pt_xent))
    {
        if (IS_XENTRY_VIRT(pt_rw_req->pt_xent))   /* virtual device needs offset */
        {
            pt_rw_req->ui8_offset += 
                ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * 
                pt_rw_req->pt_xent->pt_dev->t_blk.ui4_sec_size;
        }
    }
    else if (IS_XENTRY_CHR(pt_rw_req->pt_xent))
    {
        if (IS_XENTRY_VIRT(pt_rw_req->pt_xent))   /* virtual device needs offset */
        {
            pt_rw_req->ui8_offset += 
                ((UINT64)pt_rent->u.pt_part->ui4_start_lba) * 512;
        }
    }
    else
    {
        ASSERT(0);
    }

#if 0 // [5391 loader]
    i4_ret = _fm_buf_read_write_cache(pt_rw_req, b_read);
#else

  /*
        copied logic from _buf_read_write
  */

    if (pt_rw_req->ui2_flag & FM_IO_DIRECT)
    {
        pt_new_req = pt_rw_req;
    }
    else
    {
        pt_new_req = x_mem_alloc(sizeof(FM_RW_ARG_T));

        if (pt_new_req == NULL)
        {
            return FMR_CORE;
        }

        *pt_new_req = *pt_rw_req;

        pt_new_req->pv_data = pui1_fm_buffer;
 
        pt_new_req->ui8_offset = 
            (UINT64)((pt_new_req->ui8_offset / pt_new_req->pt_xent->pt_dev->t_blk.ui4_sec_size)
                * pt_new_req->pt_xent->pt_dev->t_blk.ui4_sec_size);

        ui4_ofst_shift = pt_rw_req->ui8_offset - pt_new_req->ui8_offset;
        
        pt_new_req->ui4_count += ui4_ofst_shift;
        
        pt_new_req->ui4_count = 
            ((pt_new_req->ui4_count / pt_new_req->pt_xent->pt_dev->t_blk.ui4_sec_size)
            + ((TRUE == (pt_new_req->ui4_count % pt_new_req->pt_xent->pt_dev->t_blk.ui4_sec_size != 0))?(1):(0))) 
                * pt_new_req->pt_xent->pt_dev->t_blk.ui4_sec_size;

        ASSERT(pt_new_req->ui4_count <= (128 * 1024));

        pt_new_req->ui2_flag |= FM_IO_DIRECT;
    }

    if ((pt_new_req->ui8_offset + (UINT64)pt_new_req->ui4_count) >
        pt_new_req->pt_xent->pt_dev->ui8_space)
    {
        pt_new_req->ui4_count = (UINT32)(pt_new_req->pt_xent->pt_dev->ui8_space - 
                                  pt_new_req->ui8_offset);
    }

    ASSERT(pt_new_req->ui2_flag & FM_IO_DIRECT);
    ASSERT(pt_new_req->ui2_flag & FM_IO_SYNC);

    if (b_read)
    {
        ASSERT(pt_new_req->pt_xent->pt_xent_ops != NULL);
        ASSERT(pt_new_req->pt_xent->pt_xent_ops->pf_read != NULL);

        i4_ret = XENTRY_OP_READ(pt_new_req->pt_xent)(pt_new_req);
    }
    else
    {
#if 0
        ASSERT(pt_rw_req->pt_xent->pt_xent_ops != NULL);
        ASSERT(pt_rw_req->pt_xent->pt_xent_ops->pf_write != NULL);

        i4_ret = XENTRY_OP_WRITE(pt_new_req->pt_xent)(pt_new_req);
#else
        ASSERT(0);
#endif
    }

    ASSERT(pt_new_req->ui4_done_bytes == pt_new_req->ui4_count);

    if (!(pt_rw_req->ui2_flag & FM_IO_DIRECT))
    {
        ui4_ofst_shift = pt_rw_req->ui8_offset - pt_new_req->ui8_offset;
    
        x_memcpy((void *) pt_rw_req->pv_data, 
                 (const void *) ((UINT32)pt_new_req->pv_data + ui4_ofst_shift), 
                 (SIZE_T) pt_rw_req->ui4_count);

        x_mem_free(pt_new_req);
    }

#endif
    /*
    if (pt_rw_req->e_async_cond == FM_ASYNC_COND_FAIL)
    {
        i4_ret = FMR_DEVICE_ERROR;
    }
    */

    return i4_ret;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name: fm_buf_flush
 *
 * Description: Flush dirty buffers of the specified xentry.
 *
 * Inputs:  pt_xent
 *              Reference to xentry.
 *          b_sync
 *              Indicate if it is synchronous or asynchronous operation.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
INT32 fm_buf_flush(FM_XENTRY_T *pt_xent, BOOL b_sync)
{
    FM_BUF_T *pt_buf;
    INT32    i4_ret  = FMR_OK;
    
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(IS_XENTRY_BLK(pt_xent) || IS_XENTRY_CHR(pt_xent));

    _fm_buf_pause_buf_daemon();

    if (ui2_dirty_buf > 0)
    {
        DLIST_FOR_EACH(pt_buf, &pt_xent->t_buf_list, t_hlink)
        {
            i4_ret = _fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_WAIT);
            if (i4_ret != FMR_OK)
            {
                break;
            }

            if (IS_BUF_DIRTY(pt_buf))
            {
                _fm_buf_lock_cache();

                if (IS_BUF_RETRY(pt_buf))
                {
                    DLIST_REMOVE(pt_buf, &t_retry_buf_q, t_vlink);
                    pt_buf->ui2_flags &= ~FM_BUF_FLAG_RETRY;
                }
                else
                {
                    DLIST_REMOVE(pt_buf, &t_dirty_buf_q, t_vlink);
                    ui2_dirty_buf--;
                }

                _fm_buf_unlock_cache();

                i4_ret = _fm_buf_flush_data(pt_buf);
            
                if (i4_ret != FMR_OK)
                {
                    DBG_ERROR(("[FM] ERR: Fail to flush buffer: %s: %xh-%xh\n",
                                 pt_buf->pt_xent->ps_name,
                                 (int)FM_BUF_ADDR(pt_buf->ui8_lbn),
                                 (int)FM_BUF_ADDR(pt_buf->ui8_lbn) + (int)pt_buf->ui4_size));

                    x_sema_unlock(pt_buf->h_sync);
                
                    _fm_buf_lock_cache();

                    DLIST_INSERT_TAIL(pt_buf, &t_dirty_buf_q, t_vlink);
                    ui2_dirty_buf++;
                
                    _fm_buf_unlock_cache();
                }
            }

            _fm_buf_unlock_buf(pt_buf);

            if (i4_ret != FMR_OK)
            {
                break;
            }
        }
    }

    _fm_buf_resume_buf_daemon();

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_buf_reclaim
 *
 * Description: Purge all used buffer blocks of the specified xentry. It is
 *              usually called when the xentry is no more referenced.
 *
 * Inputs:  pt_xent
 *              Reference to xentry.
 *          b_sync
 *              Indicate if it is synchronous or asynchronous operation.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          Others
 *              Fail.
 ----------------------------------------------------------------------------*/
VOID fm_buf_reclaim(FM_XENTRY_T *pt_xent, BOOL b_sync)
{
    FM_BUF_T *pt_buf;
    INT32    i4_ret;
    
    ASSERT(pt_xent != NULL);
    ASSERT(IS_XENTRY_LOCKED(pt_xent));
    ASSERT(IS_XENTRY_BLK(pt_xent) || IS_XENTRY_CHR(pt_xent));

    _fm_buf_pause_buf_daemon();

    while ((pt_buf = DLIST_HEAD(&pt_xent->t_buf_list)) != NULL)
    {
        i4_ret = _fm_buf_lock_buf(pt_buf, X_SEMA_OPTION_WAIT);
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR(("[FM] ERR: Fail to lock buffer\n"));
            break;
        }

        DLIST_REMOVE(pt_buf, &pt_xent->t_buf_list, t_hlink);

        _fm_buf_lock_cache();

        ASSERT(!IS_BUF_INVAL(pt_buf));
        
        if (!IS_BUF_DIRTY(pt_buf))
        {
            DLIST_REMOVE(pt_buf, &t_lru_buf_q, t_vlink);
        }
        else if (IS_BUF_RETRY(pt_buf))
        {
            DLIST_REMOVE(pt_buf, &t_retry_buf_q, t_vlink);
            pt_buf->ui2_flags &= ~(FM_BUF_FLAG_RETRY|FM_BUF_FLAG_DIRTY);
        }
        else
        {
            DLIST_REMOVE(pt_buf, &t_dirty_buf_q, t_vlink);
            pt_buf->ui2_flags &= ~FM_BUF_FLAG_DIRTY;
            ui2_dirty_buf--;
        }

        _fm_buf_unlock_cache();

        _fm_buf_release(pt_buf);

        _fm_buf_unlock_buf(pt_buf);
    }

    _fm_buf_resume_buf_daemon();
}


/*-----------------------------------------------------------------------------
 * Name: fm_buf_init
 *
 * Description: Initialize the buffer cache system.
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
#endif

INT32 fm_buf_init(VOID)
{
#if 0
    INT32 i4_ret;

    if (b_inited == TRUE)
    {
        return FMR_OK;
    }

    DLIST_INIT(&t_lru_buf_q);
    DLIST_INIT(&t_free_buf_q);
    DLIST_INIT(&t_dirty_buf_q);
    DLIST_INIT(&t_retry_buf_q);

    i4_ret = x_sema_create(&h_buf_cache_mtx,
                           X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }

    i4_ret = x_sema_create(&h_buf_thrd_mtx,
                           X_SEMA_TYPE_MUTEX,
                           X_SEMA_STATE_UNLOCK);
    if (i4_ret != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }

    i4_ret = x_timer_create(&h_buf_timer);
    if (i4_ret != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }

    i4_ret = x_timer_start(h_buf_timer,
                           FM_BUF_FLUSH_TIME,
                           X_TIMER_FLAG_REPEAT,
                           _fm_buf_wakeup_daemon,
                           NULL);
    if (i4_ret != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }

    i4_ret = x_thread_create(&h_buf_thrd,
                             "fm_buf_thread",
                             (SIZE_T) 1024,
                             128,
                             _fm_buf_daemon_thread,
                             sizeof(FM_BUF_THREAD_STATE),
                             &e_buf_thrd_state);
    if (i4_ret != OSR_OK)
    {
        DEBUG_TRACE();
        goto FAIL;
    }

    while (e_buf_thrd_state == FM_BUF_THREAD_INIT)
    {
        x_thread_delay(1);
    }

    b_inited = TRUE;

    return FMR_OK;

FAIL:

    if (h_buf_timer != NULL_HANDLE)
    {
        x_timer_delete(h_buf_timer);
        h_buf_timer = NULL_HANDLE;
    }
    if (h_buf_thrd_mtx != NULL_HANDLE)
    {
        x_timer_delete(h_buf_thrd_mtx);
        h_buf_thrd_mtx = NULL_HANDLE;
    }
    if (h_buf_cache_mtx != NULL_HANDLE)
    {
        x_sema_delete(h_buf_cache_mtx);
        h_buf_cache_mtx = NULL_HANDLE;
    }

    return FMR_CORE;
#else

    pui1_fm_buffer = x_mem_alloc(128 * 1024 + 32);

    if (pui1_fm_buffer == NULL)
    {
        return FMR_CORE;
    }

    pui1_fm_buffer = (UINT8 *)((((UINT32)pui1_fm_buffer / 32) + 1) * 32);

    return FMR_OK;

#endif
}

