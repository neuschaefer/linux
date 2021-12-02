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
 * $RCSfile: fm_dev.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/1 $
 * $SWAuthor: Yuchien Chen $
 * $MD5HEX: 04174ea8a26b68310a9f56fe2da00cf4 $
 *
 * Description: 
 *         This file implements the device asynchronous priority I/O.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "fm_dev.h"
#include "fm_ufs.h"
#include "x_rm_dev_types.h"
#include "handle/handle.h"
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#ifndef DEBUG_FM_DEV

#undef  DEBUG_TRACE
#define DEBUG_TRACE()

#define REQ_INSERT_PEND(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_pend_req_q, t_qlink)
    
#define REQ_INSERT_BUSY(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_busy_req_q, t_qlink)
    
#define REQ_INSERT_DONE(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_done_req_q, t_qlink)
    
#define REQ_INSERT_ABORT(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_abort_req_q, t_qlink)
    
#define REQ_REMOVE_PEND(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_pend_req_q, t_qlink)
    
#define REQ_REMOVE_BUSY(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_busy_req_q, t_qlink)
    
#define REQ_REMOVE_DONE(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_done_req_q, t_qlink)
    
#define REQ_REMOVE_ABORT(pt_req, pt_dev)   \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_abort_req_q, t_qlink)

#else /* !DEBUG_FM_DEV */

#define REQ_INSERT_PEND(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_pend_req_q, t_qlink);  \
    FM_DEV_SET_REQ_STATE((pt_req), FM_DEV_REQ_PEND);    \
    (pt_dev)->aui4_num[FM_DEV_REQ_PEND]++
    
#define REQ_INSERT_BUSY(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_busy_req_q, t_qlink);  \
    FM_DEV_SET_REQ_STATE((pt_req), FM_DEV_REQ_BUSY);    \
    (pt_dev)->aui4_num[FM_DEV_REQ_BUSY]++
    
#define REQ_INSERT_DONE(pt_req, pt_dev)    \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_done_req_q, t_qlink);  \
    FM_DEV_SET_REQ_STATE((pt_req), FM_DEV_REQ_DONE);    \
    (pt_dev)->aui4_num[FM_DEV_REQ_DONE]++
    
#define REQ_INSERT_ABORT(pt_req, pt_dev)   \
    DLIST_INSERT_TAIL((pt_req), &(pt_dev)->t_abort_req_q, t_qlink); \
    FM_DEV_SET_REQ_STATE((pt_req), FM_DEV_REQ_ABORT);   \
    (pt_dev)->aui4_num[FM_DEV_REQ_ABORT]++
    
#define REQ_REMOVE_PEND(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_pend_req_q, t_qlink);   \
    (pt_dev)->aui4_num[FM_DEV_REQ_PEND]--
    
#define REQ_REMOVE_BUSY(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_busy_req_q, t_qlink);   \
    (pt_dev)->aui4_num[FM_DEV_REQ_BUSY]--
    
#define REQ_REMOVE_DONE(pt_req, pt_dev)    \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_done_req_q, t_qlink);   \
    (pt_dev)->aui4_num[FM_DEV_REQ_DONE]--
    
#define REQ_REMOVE_ABORT(pt_req, pt_dev)   \
    DLIST_REMOVE((pt_req), &(pt_dev)->t_abort_req_q, t_qlink);  \
    (pt_dev)->aui4_num[FM_DEV_REQ_ABORT]--

#endif /* !DEBUG_FM_DEV */

#define _fm_dev_lock_devlist() \
    x_sema_lock(h_dev_list_mtx, X_SEMA_OPTION_WAIT)
    
#define _fm_dev_unlock_devlist() \
    x_sema_unlock(h_dev_list_mtx)

/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/
static SLIST_T(_FM_DEVICE_T)    t_dev_list;
static HANDLE_T                 h_dev_list_mtx = NULL_HANDLE;
static BOOL                     b_inited       = FALSE;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

static VOID _fm_dev_io_thread(
    FM_DEVICE_T *pt_dev);

static VOID _fm_dev_async_thread(
    VOID        *pv_data);


extern INT32 FM_MUSB_Get(HANDLE_T hHandle, UINT32 u4GetType, VOID* pvData, UINT32 *pu4Size);
extern INT32 FM_MUSB_Set(HANDLE_T hHandle, UINT32 u4SetType, VOID* pvData, UINT32 u4Size);
extern INT32 FM_MUSB_Open(HANDLE_T* hHandle);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: _fm_dev_io_thread
 *
 * Description: The thread to handle the asynchronous read/write requests for
 *              non-flash devices.
 *
 * Inputs:  pt_dev
 *              Reference to the device object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_dev_io_thread(
    FM_DEVICE_T     *pt_dev)
{
    FM_IO_REQ_T *pt_req, *pt_req_abort, *pt_req_next;
    INT32       i4_ret;
    VOID *pv_obj;
    HANDLE_TYPE_T   e_obj_type;
    SIZE_T      z_msg_size;
    UINT16      ui2_q_idx;
    UINT16      ui2_resend_num = 0;
    FM_RW_ARG_T    *pt_arg;
    UNUSED(ui2_resend_num);
    while (1)
    {

        x_sema_lock(pt_dev->h_queue_mtx, X_SEMA_OPTION_WAIT);

        /*
         *  Receive done messages. Move all I/O requests from
         *  busy queue to done queue.
         */
        while (1)
        {
            z_msg_size = sizeof(FM_IO_REQ_T *);
            i4_ret = x_msg_q_receive(&ui2_q_idx,
                                     &pt_req,
                                     &z_msg_size,
                                     &pt_dev->h_done_msgq,
                                     1,
                                     X_MSGQ_OPTION_NOWAIT);
            if (i4_ret != OSR_OK)
            {
                break;
            }
            
            REQ_REMOVE_BUSY(pt_req, pt_dev);
            REQ_INSERT_DONE(pt_req, pt_dev);
        }
        
#ifdef DEBUG_FM_DEV
        DBG_INFO(("[FM] pend:%d busy:%d done:%d abort:%d\n",
                   (int)pt_dev->aui4_num[1],
                   (int)pt_dev->aui4_num[2],
                   (int)pt_dev->aui4_num[3],
                   (int)pt_dev->aui4_num[4]));
#endif /* DEBUG_FM_DEV */

        if (DLIST_IS_EMPTY(&pt_dev->t_pend_req_q) &&
            DLIST_IS_EMPTY(&pt_dev->t_done_req_q) &&
            DLIST_IS_EMPTY(&pt_dev->t_abort_req_q))
        {
            if (pt_dev->ui1_thread_state != FM_DEV_THREAD_RUN)
            {
                break;
            }

            x_sema_unlock(pt_dev->h_queue_mtx);
            x_sema_lock(pt_dev->h_ctrl_mtx, X_SEMA_OPTION_WAIT);
            continue;
        }
              
        while (DLIST_IS_EMPTY(&pt_dev->t_pend_req_q) != TRUE
            || DLIST_IS_EMPTY(&pt_dev->t_abort_req_q) != TRUE)
        {
        
            /* to remove requests if there is an abort request */
            while( (pt_req_abort = DLIST_HEAD(&pt_dev->t_abort_req_q)) != NULL)
            {
                REQ_REMOVE_ABORT(pt_req_abort, pt_dev);
                handle_get_type_obj(pt_req_abort->h_rw_req, &e_obj_type, &pv_obj);

                pt_req = pt_dev->t_pend_req_q.pt_head;
                while( pt_req)
                {
                    pt_req_next = pt_req->t_qlink.pt_next;
                    pt_arg = (FM_RW_ARG_T*)pt_req->pv_tag;

                    if ( (e_obj_type == FMT_FILE_DESC && pt_arg->h_file == pt_req_abort->h_rw_req)
                        || ((e_obj_type == FMT_ASYNC_READ || e_obj_type == FMT_ASYNC_WRITE)
                                && pt_arg->h_rw_req == pt_req_abort->h_rw_req))
                    {
                        DBG_INFO(("abort in fm_dev.c, pt_req: %x\n", pt_req));
                        pt_req->ui4_done_byte = 0;
                        pt_req->e_cond = FM_ASYNC_COND_ABORT_OK;
                        REQ_REMOVE_PEND(pt_req, pt_dev);
                        REQ_INSERT_DONE(pt_req, pt_dev);
                        pt_arg->ui1_aborted = TRUE;
                    }
                    pt_req = pt_req_next;
                }
                pt_req_abort->e_cond = FM_ASYNC_COND_ABORT_OK;
                pt_req_abort->ui4_done_byte = 0;
                REQ_INSERT_DONE(pt_req_abort, pt_dev);
            }

            if ( (pt_req = DLIST_HEAD(&pt_dev->t_pend_req_q)) != NULL)
            {
                REQ_REMOVE_PEND(pt_req, pt_dev);

                if (pt_req->ui4_done_byte != 0)
                {
                    if (pt_req->ui1_type & FM_DEV_REQ_READ)
                    {
                        pt_req->e_cond = FM_ASYNC_COND_READ_OK;
                    }
                    else
                    {
                        pt_req->e_cond = FM_ASYNC_COND_WRITE_OK;
                    }

                    REQ_INSERT_DONE(pt_req, pt_dev);

                    continue;
                }
                
                /*
                 *  XXX, don't exceed the maximum acceptable requests.
                 */
                REQ_INSERT_BUSY(pt_req, pt_dev);
                ui2_resend_num = 0;

                while( 1)
                {
                    if ((pt_req->ui1_type & FM_DEV_REQ_OP_MASK) == FM_DEV_REQ_TYPE_GET)
                    {
#if 1 /*  [Nathan] link with driver */
                        i4_ret = FM_MUSB_Get(pt_dev->h_driver, 
                                             pt_req->ui4_op, 
                                             pt_req, 
                                             (UINT32*)&pt_req->z_len);
#else
                        i4_ret = rm_get(pt_dev->h_driver,
                                        FALSE,
                                        0,
                                        pt_req->ui4_op,
                                        (VOID *)pt_req,
                                        &pt_req->z_len,
                                        &e_cond);
#endif
                    }
                    else
                    {
#if 1 /*  [Nathan] link with driver */
                        i4_ret = FM_MUSB_Set(pt_dev->h_driver, 
                                             pt_req->ui4_op, 
                                             pt_req, 
                                             pt_req->z_len);
#else
                        i4_ret = rm_set(pt_dev->h_driver,
                                        FALSE,
                                        0,
                                        pt_req->ui4_op,
                                        (VOID *)pt_req,
                                        pt_req->z_len,
                                        &e_cond);
#endif
                    }
#if 1 /*  [Nathan] link with driver */
                    if (i4_ret == 0)
                    {
                        ui2_resend_num = 0;
                        break;
                    }
                    else
                    {
                        ASSERT(0);
                    }
#else
                    if ( i4_ret == RMR_DRV_SET_FAILED || i4_ret == RMR_DRV_ERROR)
                    {
                        x_sema_unlock(pt_dev->h_queue_mtx);
                        x_thread_delay(50);
                        x_sema_lock(pt_dev->h_queue_mtx, X_SEMA_OPTION_WAIT);

                        if (ui2_resend_num++ > 5)
                        {
                            pt_req->e_cond = FM_ASYNC_COND_FAIL;
                            REQ_REMOVE_BUSY(pt_req, pt_dev);
                            REQ_INSERT_DONE(pt_req, pt_dev);
                            break;
                        }
                        else
                            continue;
                    }
                    else if (i4_ret != RMR_ASYNC_NFY)
                    {
                        pt_req->e_cond = FM_ASYNC_COND_FAIL;
                        REQ_REMOVE_BUSY(pt_req, pt_dev);
                        REQ_INSERT_DONE(pt_req, pt_dev);
                        break;
                    }
                    else
                    {
                        ui2_resend_num = 0;
                        break;
                    }
#endif
                    /*}*/
                }
            }
        }

        while ((pt_req = DLIST_HEAD(&pt_dev->t_done_req_q)) != NULL)
        {
            REQ_REMOVE_DONE(pt_req, pt_dev);
            
            if (pt_req->pf_nfy_fct != NULL)
            {
                pt_req->pf_nfy_fct(
                                    pt_req->h_rw_req,
                                    pt_req->pv_tag,
                                    pt_req->e_cond,
                                    pt_req->ui4_done_byte);
            }
            x_mem_free(pt_req);
        }

        x_sema_unlock(pt_dev->h_queue_mtx);
    }
    
    /* pt_dev->h_queue_mtx still locked here */

    /*
     *  Wait for requests that have been issued to the device driver
     *  before leaving.
     */
    while (DLIST_HEAD(&pt_dev->t_busy_req_q) != NULL)
    {
        x_sema_unlock(pt_dev->h_queue_mtx);
        x_thread_delay(1);
        x_sema_lock(pt_dev->h_queue_mtx, X_SEMA_OPTION_WAIT);
    }

    /* pt_dev->h_queue_mtx still locked here */

    while ((pt_req = DLIST_HEAD(&pt_dev->t_done_req_q)) != NULL)
    {
        REQ_REMOVE_DONE(pt_req, pt_dev);
        
        if (pt_req->pf_nfy_fct != NULL)
        {
            pt_req->pf_nfy_fct( pt_req->h_rw_req,
                                            pt_req->pv_tag,
                                            pt_req->e_cond,
                                            pt_req->ui4_done_byte);
        }
        x_mem_free(pt_req);
    }

    x_sema_unlock(pt_dev->h_queue_mtx);
}


/*-----------------------------------------------------------------------------
 * Name: _fm_dev_async_thread
 *
 * Description: A thread to handle asynchronous I/O for device driver.
 *              Each device has one such thread. The thread manages I/O
 *              requests with a priority queue.
 *
 * Inputs:  pv_data
 *              Reference to the device object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static VOID _fm_dev_async_thread(
    VOID    *pv_data)
{
    FM_DEVICE_T *pt_dev;
    
    ASSERT(pv_data != NULL);

    pt_dev = *(FM_DEVICE_T **)pv_data;

    /* Make the caller resume */
    pt_dev->ui1_thread_state = FM_DEV_THREAD_RUN;
#if 0
    if ((pt_dev->e_drv_type == DRVT_NOR_FLASH) ||
        (pt_dev->e_drv_type == DRVT_NAND_FLASH))
    {
        ASSERT(0);
    }
    else
#endif
    {
        _fm_dev_io_thread(pt_dev);
    }
    
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_pend_req_q));
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_busy_req_q));
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_done_req_q));

    x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);

    pt_dev->ui1_thread_state = FM_DEV_THREAD_OVER;

    x_sema_unlock(pt_dev->h_async_thrd_mtx);

    x_thread_exit();
}


/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: fm_dev_open
 *
 * Description: Open the device driver, init the priority queue and create
 *              the I/O thread. After that, add it to the device list. Each
 *              driver is exactly opened once. If the specified driver was
 *              opened before, it would add the reference count only.
 *
 * Inputs:  ps_name
 *              Driver's name.
 *          e_drv_type
 *              Driver component type.
 *              It could be one of the following values:
 *              DRVT_EEPROM, DRVT_NOR_FLASH, DRVT_NAND_FLASH, DRVT_MEM_CARD,
 *              DRVT_HARD_DISK.
 *          ui2_uint
 *              Driver unit number.
 *
 * Outputs: ppt_dev
 *              Reference to the device object.
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_NO_SUCH_DEVICE
 *              Cannot open the specified driver component.
 *          FMR_DEVICE_ERROR
 *              Cannot set the notification function.
 *          FMR_CORE
 *              OSAI error.
 ----------------------------------------------------------------------------*/
INT32 fm_dev_open(
    CHAR            *ps_name,
    DRV_TYPE_T      e_drv_type,
    UINT16          ui2_unit,
    FM_DEVICE_T     **ppt_dev)
{
    FM_DEVICE_T     *pt_dev = NULL;
    HANDLE_T        h_driver = NULL_HANDLE;
    INT32           i4_ret;

    ASSERT(ppt_dev != NULL);
    ASSERT(
#if 0
           (e_drv_type == DRVT_EEPROM)      ||
           (e_drv_type == DRVT_NOR_FLASH)   ||
           (e_drv_type == DRVT_NAND_FLASH)  ||
#endif
           (e_drv_type == DRVT_MEM_CARD)    ||
           (e_drv_type == DRVT_HARD_DISK)   ||
           (e_drv_type == DRVT_USB_MASS_STORAGE) ||
           (e_drv_type == DRVT_OPTICAL_DISC));

    if (b_inited == FALSE)
    {
        if (fm_dev_init() != FMR_OK)
        {
            return FMR_CORE;
        }
    }

    _fm_dev_lock_devlist();

    SLIST_FOR_EACH(pt_dev, &t_dev_list, t_llink)
    {
        if ((pt_dev->e_drv_type == e_drv_type) &&
            (pt_dev->ui2_unit == ui2_unit))
        {
            pt_dev->ui2_ref_cnt++;
            *ppt_dev = pt_dev;
            _fm_dev_unlock_devlist();
            return FMR_EXIST;
        }
    }

    _fm_dev_unlock_devlist();

    /*
     *  Open device driver in SINGLE mode.
     */
#if 1 /*  [Nathan] link with driver */
    i4_ret = FM_MUSB_Open(&h_driver);
    if (i4_ret != RMR_OK)
    {
        i4_ret = FMR_NO_SUCH_DEVICE;
        goto FAIL;
    }    
#else
    i4_ret = rm_open_comp(NULL_HANDLE,
                          OBJ_COMP_REL_IGNORE,
                          e_drv_type,
                          ui2_unit,
                          NULL,
                          CTRL_TYPE_SHARED,
                          NULL,
                          NULL,
                          NULL,
                          _fm_dev_rm_nfy_fct,
                          &h_driver,
                          &e_cond);
    if (i4_ret != RMR_OK)
    {
        i4_ret = FMR_NO_SUCH_DEVICE;
        goto FAIL;
    }
#endif
    /*
     *  Check if the memory card exists.
     */
#if 1 /*  [Nathan] link with driver */
    if (e_drv_type == DRVT_MEM_CARD || e_drv_type == DRVT_USB_MASS_STORAGE)
    {
        BLKDEV_STATUS_T e_status;    
        SIZE_T z_read;
        
        i4_ret = FM_MUSB_Get(h_driver, BLKDEV_GET_TYPE_STATUS, (VOID*)&e_status, (UINT32*)&z_read);
        if (i4_ret != RMR_OK)
        {
            i4_ret = FMR_NO_SUCH_DEVICE;
            goto FAIL;
        }
        if (e_status != BLKDEV_STATUS_READY)
        {
            i4_ret = FMR_DEVICE_ERROR;
            goto FAIL;
        }        
    }
#else
    if (e_drv_type == DRVT_MEM_CARD || e_drv_type == DRVT_USB_MASS_STORAGE)
    {
        BLKDEV_STATUS_T e_status;
        SIZE_T z_read;

        z_read = sizeof(BLKDEV_STATUS_T);
        i4_ret = rm_get(h_driver,
                        FALSE,
                        0,
                        BLKDEV_GET_TYPE_STATUS,
                        (VOID *)&e_status,
                        &z_read,
                        &e_cond);
        if (i4_ret != RMR_OK)
        {
            i4_ret = FMR_NO_SUCH_DEVICE;
            goto FAIL;
        }
        if (e_status != BLKDEV_STATUS_READY)
        {
            i4_ret = FMR_DEVICE_ERROR;
            goto FAIL;
        }
    }
#endif
    /*
     *  Allocate a new device.
     */
    pt_dev = x_mem_calloc(1, sizeof(FM_DEVICE_T));
    if (pt_dev == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    /*
     *  Check if the device supports asynchronous mode.
     */
#if 0
    if (e_drv_type == DRVT_EEPROM)
    {
        UINT16            ui2_idx;
        UINT16            ui2_cmp_id;
        UINT32            ui4_cmp_flags;
        
        for (ui2_idx = 0; ui2_idx < 32; ui2_idx++)
        {
            i4_ret = rm_get_comp_info(e_drv_type,
                                      ui2_idx,
                                      &ui2_cmp_id,
                                      NULL,
                                      NULL,
                                      &ui4_cmp_flags);
            if ((i4_ret == RMR_OK) && (ui2_cmp_id == ui2_unit))
            {
                if (ui4_cmp_flags &
                    (CHRDEV_FLAG_ASYNC_READ|CHRDEV_FLAG_ASYNC_WRITE))
                {
                    pt_dev->ui2_flags |= FM_DEV_FLAG_ASYNC;
                }
                break;
            }
        }
    }
    else
#endif
    {
        pt_dev->ui2_flags |= FM_DEV_FLAG_ASYNC;
    }

    if (x_sema_create(&pt_dev->h_queue_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    if (x_sema_create(&pt_dev->h_async_thrd_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    i4_ret = x_msg_q_create(&pt_dev->h_done_msgq,
                            ps_name,
                            sizeof(FM_IO_REQ_T *),
                            256);
    if (i4_ret != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }

    DLIST_INIT(&pt_dev->t_pend_req_q);
    DLIST_INIT(&pt_dev->t_busy_req_q);
    DLIST_INIT(&pt_dev->t_done_req_q);
    DLIST_INIT(&pt_dev->t_abort_req_q);

    /* added by riccardo {*/
    i4_ret = x_sema_create(&pt_dev->h_ctrl_mtx, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    if ( i4_ret != OSR_OK)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }
                    
    /* } */
#if 0
    if ((e_drv_type == DRVT_NOR_FLASH) || (e_drv_type == DRVT_NAND_FLASH))
    {
        if (x_sema_create(&pt_dev->h_flash_wait_flush,
                          X_SEMA_TYPE_BINARY,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        if (x_sema_create(&pt_dev->h_flash_wait_io,
                          X_SEMA_TYPE_BINARY,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DLIST_INIT(&pt_dev->t_flash_cache);
    }
#endif
    pt_dev->h_driver         = h_driver;
    pt_dev->e_drv_type       = e_drv_type;
    pt_dev->ui2_unit         = ui2_unit;
    pt_dev->ui2_ref_cnt++;

    if (pt_dev->ui2_flags & FM_DEV_FLAG_ASYNC)
    {
        pt_dev->ui1_thread_state = FM_DEV_THREAD_INIT;

        i4_ret = x_thread_create(&pt_dev->h_thread,
                                 ps_name,
                                 (SIZE_T) 1024,
                                 128,
                                 _fm_dev_async_thread,
                                 sizeof(FM_DEVICE_T *),
                                 (VOID *)&pt_dev);
        if (i4_ret != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }

        while (pt_dev->ui1_thread_state == FM_DEV_THREAD_INIT)
        {
            x_thread_delay(1);
        }
    }
    
    *ppt_dev = pt_dev;

    _fm_dev_lock_devlist();

    SLIST_INSERT_HEAD(pt_dev, &t_dev_list, t_llink);

    _fm_dev_unlock_devlist();

    return FMR_OK;
    
FAIL:

    if (pt_dev != NULL)
    {
#if 0
        if ((e_drv_type == DRVT_NOR_FLASH) || (e_drv_type == DRVT_NAND_FLASH))
        {
            if (pt_dev->h_flash_wait_io != NULL_HANDLE)
            {
                x_sema_delete(pt_dev->h_flash_wait_io);
            }
            if (pt_dev->h_flash_wait_flush != NULL_HANDLE)
            {
                x_sema_delete(pt_dev->h_flash_wait_flush);
            }
        }
#endif
        if (pt_dev->h_done_msgq != NULL_HANDLE)
        {
            x_msg_q_delete(pt_dev->h_done_msgq);
        }
        if (pt_dev->h_async_thrd_mtx != NULL_HANDLE)
        {
            x_sema_delete(pt_dev->h_async_thrd_mtx);
        }
        if (pt_dev->h_queue_mtx != NULL_HANDLE)
        {
            x_sema_delete(pt_dev->h_queue_mtx);
        }
        x_mem_free(pt_dev);
    }
#if 0 /*  [Nathan] link with driver */
    if (h_driver != NULL_HANDLE)
    {
        rm_close(h_driver, FALSE, &e_cond);
    }
#endif 
    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: fm_dev_close
 *
 * Description: Close the opened device.
 *
 * Inputs:  pt_dev
 *              Reference to the device object.
 *
 * Outputs: -
 *
 * Returns: FMR_OK
 *              Success.
 *          FMR_INVAL
 *              Invalid pt_dev
 ----------------------------------------------------------------------------*/
INT32 fm_dev_close(
    FM_DEVICE_T     *pt_dev)
{   
    ASSERT(pt_dev != NULL);
    ASSERT(b_inited == TRUE);
    
    if (pt_dev->h_driver == NULL_HANDLE)
    {
        return FMR_INVAL;
    }

    _fm_dev_lock_devlist();

    if (pt_dev->ui2_ref_cnt > 0)
    {
        if (--pt_dev->ui2_ref_cnt == 0)
        {
            SLIST_REMOVE(pt_dev, t_llink);

            if (pt_dev->ui2_flags & FM_DEV_FLAG_ASYNC)
            {
                x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);

                pt_dev->ui1_thread_state = FM_DEV_THREAD_KILL;
                /* x_thread_resume(pt_dev->h_thread); */
                x_sema_unlock(pt_dev->h_ctrl_mtx);

                while (pt_dev->ui1_thread_state != FM_DEV_THREAD_OVER)
                {
                    x_sema_unlock(pt_dev->h_async_thrd_mtx);
                    x_thread_delay(1);
                    x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);
                }

                x_sema_unlock(pt_dev->h_async_thrd_mtx);

                /*
                 *  Thread had been over. It's safe to free resources now.
                 */
            }
#if 0 /*  [Nathan] link with driver */
            rm_close(pt_dev->h_driver, FALSE, &e_cond);
#endif
#if 0
            if ((pt_dev->e_drv_type == DRVT_NOR_FLASH) ||
                (pt_dev->e_drv_type == DRVT_NAND_FLASH))
            {
                x_sema_delete(pt_dev->h_flash_wait_io);
                x_sema_delete(pt_dev->h_flash_wait_flush);
            }
#endif
            x_msg_q_delete(pt_dev->h_done_msgq);
            x_sema_delete(pt_dev->h_async_thrd_mtx);
            x_sema_delete(pt_dev->h_queue_mtx);
            x_sema_delete(pt_dev->h_ctrl_mtx);

#if 0
            if (pt_dev->pt_sector_tbl != NULL)
            {
                x_mem_free(pt_dev->pt_sector_tbl);
            }
#endif
            x_mem_free(pt_dev);
        }
    }

    _fm_dev_unlock_devlist();

    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_dev_send_req
 *
 * Description: Send an asynchronous request to the device.
 *
 * Inputs:  pt_dev
 *              Reference to the device object.
 *          pt_new_req
 *              Reference to the request.
 *
 * Outputs: -
 *
 * Returns: FM_OK
 *              Success.
 *          FMR_DEVICE_ERROR
 *              Device I/O thread is being closed.
 ----------------------------------------------------------------------------*/
INT32 fm_dev_send_req(
    FM_DEVICE_T     *pt_dev,
    FM_IO_REQ_T     *pt_new_req)
{
    FM_IO_REQ_T     *pt_req;
    BOOL            b_done = FALSE;

    ASSERT(pt_dev != NULL);
    ASSERT(pt_new_req != NULL);

    if (pt_dev->ui1_thread_state != FM_DEV_THREAD_RUN)
    {
        return FMR_DEVICE_ERROR;
    }
#ifdef DEBUG_FM_DEV
    FM_DEV_SET_REQ_STATE(pt_new_req, FM_DEV_REQ_INVAL);
#endif /* DEBUG_FM_DEV */
    x_sema_lock(pt_dev->h_queue_mtx, X_SEMA_OPTION_WAIT);

    if ( pt_new_req->ui1_type & FM_DEV_REQ_ABORT)
    {
        REQ_INSERT_ABORT(pt_new_req, pt_dev);
    }
    else
    {
        DLIST_FOR_EACH(pt_req, &pt_dev->t_pend_req_q, t_qlink)
        {
            pt_req->ui2_hold_cnt++;
            if ((pt_req->ui2_hold_cnt > 100) && (pt_req->ui1_pri > 0))
            {
                pt_req->ui1_pri--;
                pt_req->ui2_hold_cnt = 0;
            }
            if (pt_new_req->ui1_pri >= pt_req->ui1_pri)
            {
                continue;
            }

            if (b_done == FALSE)
            {
                DLIST_INSERT_BEFORE(pt_new_req,
                                    pt_req,
                                    &pt_dev->t_pend_req_q,
                                    t_qlink);
#ifdef DEBUG_FM_DEV
                FM_DEV_SET_REQ_STATE(pt_new_req, FM_DEV_REQ_PEND);
                pt_dev->aui4_num[FM_DEV_REQ_PEND]++;
#endif /* DEBUG_FM_DEV */

                b_done = TRUE;
            }
        }
        if (b_done == FALSE)
        {
            REQ_INSERT_PEND(pt_new_req, pt_dev);
        }
        
    }
    

    /* x_thread_resume(pt_dev->h_thread);*/

    x_sema_unlock(pt_dev->h_queue_mtx);
    x_sema_unlock(pt_dev->h_ctrl_mtx);
    return FMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: fm_dev_nfy_fct
 *
 * Description: Callback function used in rm_open_comp. It is called when
 *              a driver finishes the I/O operation.
 *
 * Inputs:  pv_nfy_tag
 *              Tag value that was set while registering this function.
 *              It is used to keep reference to the device structure.
 *          e_nfy_cond
 *              Notify condition.
 *          pv_data
 *              Reference to the request object.
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
VOID fm_dev_nfy_fct(
    VOID        *pv_nfy_tag,
    INT32       e_nfy_cond,
    VOID        *pv_data)
{
    FM_IO_REQ_T *pt_req;
    FM_DEVICE_T *pt_dev;
    INT32       i4_ret;
    
    ASSERT(pv_nfy_tag != NULL);
    ASSERT(pv_data    != NULL);

    pt_req = (FM_IO_REQ_T *)pv_data;
    pt_dev = (FM_DEVICE_T *)pv_nfy_tag;
#ifdef DEBUG_FM_DEV
    ASSERT(pt_req->ui2_state == FM_DEV_REQ_BUSY);
#endif /* DEBUG_FM_DEV */

    pt_req->e_cond = (FM_ASYNC_COND_T)e_nfy_cond;
    switch (pt_req->ui1_type & FM_DEV_REQ_DATA_MASK)
    {
        case FM_DEV_REQ_TYPE_BLK:
            pt_req->ui4_done_byte = ((BLKDEV_IO_DATA_T *)pt_req)->ui4_trans_count;
            break;
        case FM_DEV_REQ_TYPE_CHRL:
#if 0
            pt_req->ui4_done_byte = ((CHRDEV_LONG_DATA_INFO_T *)pt_req)->ui4_len;
            break;
#endif
        case FM_DEV_REQ_TYPE_CHRS:
        default:
            ASSERT(0);
    }

    i4_ret = x_msg_q_send(pt_dev->h_done_msgq,
                          (VOID *)&pt_req,
                          sizeof(FM_IO_REQ_T *),
                          0);
    if (i4_ret != OSR_OK)
    {
        DBG_ERROR(("[FM] ERR: PANIC! Send msg fail\n"));
    }
    x_sema_unlock(pt_dev->h_ctrl_mtx);
    /* x_thread_resume(pt_dev->h_thread); */
}


/*-----------------------------------------------------------------------------
 * Name: fm_dev_init
 *
 * Description: Init the device list and mutex.
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
INT32 fm_dev_init(VOID)
{
    if (b_inited == TRUE)
    {
        return FMR_OK;
    }

    SLIST_INIT(&t_dev_list);

    if (x_sema_create(&h_dev_list_mtx,
                      X_SEMA_TYPE_MUTEX,
                      X_SEMA_STATE_UNLOCK) != OSR_OK)
    {
        return FMR_CORE;
    }

    b_inited = TRUE;

    return FMR_OK;
}

