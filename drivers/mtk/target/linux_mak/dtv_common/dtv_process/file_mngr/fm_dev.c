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
 * $RCSfile: fm_dev.c,v $
 * $Revision: #2 $
 * $Date: 2012/05/23 $
 * $Author: hongjun.chu $
 *
 * Description: 
 *         This file implements the device asynchronous priority I/O.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/common.h"
#include "res_mngr/x_rm_dev_types.h"
#include "res_mngr/rm.h"
#include "res_mngr/x_rm.h"
#include "file_mngr/linuxfs/fm_linux.h"
#include "file_mngr/fm_dev.h"
#include "file_mngr/fm_ufs.h"
#include "dm/x_dm_dlna.h"
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
#include <string.h>

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
extern VOID* x_dm_smb_get_device(UINT32 ui4_id);


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/

#if defined(DLNA_SUPPORT)

static VOID _fm_dev_async_thread(
    VOID        *pv_data);

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
#ifdef DLNA_SUPPORT
    if (pt_dev->e_drv_type == DRVT_DLNA_DMS)
    {
        fm_dlna_io_thread(pt_dev);
    }
#endif
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_pend_req_q));
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_busy_req_q));
    ASSERT(DLIST_IS_EMPTY(&pt_dev->t_done_req_q));

    x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);

    pt_dev->ui1_thread_state = FM_DEV_THREAD_OVER;

    x_sema_unlock(pt_dev->h_async_thrd_mtx);

    x_thread_exit();
}
#endif


static FM_DEV_TYPE_T _fm_dev_get_device_type(char *full_path)
{
    FM_DEV_TYPE_T e_device_type = FM_DEV_TYPE_INVAL;
    
    ASSERT(full_path != NULL);

    if (strstr(full_path, "mtdsdm"))
    {
        e_device_type = FM_DEV_TYPE_SDM;
    }
    else if (strstr(full_path, "mtdblock"))
    {
        e_device_type = FM_DEV_TYPE_MTDBLOCK;
    }
    else if (strstr(full_path, "mtd"))
    {
        if(strstr(full_path,"ro"))
        {
            e_device_type = FM_DEV_TYPE_MTDRO;
        }
        else
        {
            e_device_type = FM_DEV_TYPE_MTD;
        }
    }

    DBG_INFO(("[FM]%s is TYPE (%d) \n", full_path, e_device_type));

    return e_device_type;  
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
    VOID           *pv_xent,
    DRV_TYPE_T      e_drv_type,
    UINT16          ui2_unit,
    FM_DEVICE_T     **ppt_dev)
{
    FM_XENTRY_T     *pt_xent = NULL;
    FM_DEVICE_T     *pt_dev = NULL;
    HANDLE_T        h_driver = NULL_HANDLE;
    INT32           i4_ret;
    UINT32          ui4_access_type = 0;
    FM_DEV_TYPE_T   e_device_type = FM_DEV_TYPE_INVAL;

    ASSERT(ppt_dev != NULL);
    ASSERT((e_drv_type == DRVT_EEPROM)      ||
           (e_drv_type == DRVT_NOR_FLASH)   ||
           (e_drv_type == DRVT_NAND_FLASH)  ||
           (e_drv_type == DRVT_MEM_CARD)    ||
           (e_drv_type == DRVT_HARD_DISK)   ||
           (e_drv_type == DRVT_USB_MASS_STORAGE) ||
           (e_drv_type == DRVT_OPTICAL_DISC) ||
           (e_drv_type == DRVT_DLNA_DMS) ||
           (e_drv_type == DRVT_SMB));
    DBG_INFO(("[FM] %s enter\n",__FUNCTION__));

    if (b_inited == FALSE)
    {
        if (fm_dev_init() != FMR_OK)
        {
            
            DBG_INFO(("[FM] %s fm_dev_init fail\n",__FUNCTION__));
            return FMR_CORE;
        }
    }
    pt_xent = (FM_XENTRY_T*)pv_xent;

    _fm_dev_lock_devlist();

    SLIST_FOR_EACH(pt_dev, &t_dev_list, t_llink)
    {
        if ((pt_dev->e_drv_type == e_drv_type) &&
            (pt_dev->ui2_unit == ui2_unit))
        {
            pt_dev->ui2_ref_cnt++;
            *ppt_dev = pt_dev;
            _fm_dev_unlock_devlist();
            
            DBG_INFO(("[FM] %s this device is already existed\n",__FUNCTION__));
            return FMR_EXIST;
        }
    }

    _fm_dev_unlock_devlist();
#ifdef DLNA_SUPPORT
    if (e_drv_type == DRVT_DLNA_DMS)
    {
        h_driver = (HANDLE_T)x_dm_dlna_get_device((UINT32)ui2_unit);

        if (h_driver)
        {
            i4_ret = FMR_OK;
        }
        else
        {
            i4_ret = FMR_NO_SUCH_DEVICE;
            goto FAIL;
        }
    }
    else
#endif
#ifdef SMB_SUPPORT
    if (e_drv_type == DRVT_SMB)
    {
		printf ("[smb..] %s, dm_smb_get_device\n", __FUNCTION__);
        h_driver = (HANDLE_T)x_dm_smb_get_device((UINT32)ui2_unit);

        if (h_driver)
        {
            i4_ret = FMR_OK;
        }
        else
        {
            i4_ret = FMR_NO_SUCH_DEVICE;
            goto FAIL;
        }
    }
    else
#endif
    {
        CHAR* ps_name = NULL;
        INT32 i4_fd;

        ps_name = linux_generate_absolute_name(
            pt_xent->pt_prnt_xent,
            pt_xent,
            pt_xent->ps_name);

        if (ps_name == NULL)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        
        DBG_INFO(("[FM] %s device name:%s\n",__FUNCTION__,ps_name));
        
        i4_fd = open(ps_name, O_RDWR, 0777);
        if (i4_fd < 0)
        {
            DBG_INFO(("[FM] %s Open %s fail %s\n",__FUNCTION__,ps_name, strerror(errno)));
            i4_fd = open(ps_name, O_RDONLY, 0777);
            if (i4_fd < 0)
            {
                DBG_INFO(("[FM] %s Open %s fail %s\n",__FUNCTION__,ps_name, strerror(errno)));
                i4_fd = open(ps_name, O_WRONLY, 0777);
                if (i4_fd < 0)
                {
                    ui4_access_type = 0;
                    DBG_INFO(("[FM] %s Open %s fail %s\n",__FUNCTION__,ps_name, strerror(errno)));
                }
                else
                {
                    ui4_access_type = 2;
                }
            }
            else
            {
                ui4_access_type = 3;
            }
        }
        else
        {
            ui4_access_type = 1;
        }
        e_device_type = _fm_dev_get_device_type(ps_name);
        x_mem_free(ps_name);

        if (i4_fd == -1)
        {
            switch (errno)
            {
                case ENOENT:
                    i4_ret = FMR_NO_SUCH_DEVICE;
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
            
            DBG_INFO(("[FM] %s Open fail %s\n",__FUNCTION__, strerror(errno)));
            goto FAIL;
        }

        h_driver = i4_fd;
        
        DBG_INFO(("[FM] %s Open success. fd=%d\n",__FUNCTION__, i4_fd));
    }

    /*
     *  Allocate a new device.
     */
    pt_dev = x_mem_calloc(1, sizeof(FM_DEVICE_T));
    if (pt_dev == NULL)
    {
        i4_ret = FMR_CORE;
        goto FAIL;
    }
    if (e_drv_type == DRVT_USB_MASS_STORAGE)
    {
        pt_dev->t_blk.ui4_access_type = ui4_access_type;
    }
#ifdef DLNA_SUPPORT
    if (e_drv_type == DRVT_DLNA_DMS)
    {
        pt_dev->ui2_flags |= FM_DEV_FLAG_ASYNC;

        DBG_INFO(("[FM] %s Create h_queue_mtx\n",__FUNCTION__));

        if (x_sema_create(&pt_dev->h_queue_mtx,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Create h_async_thrd_mtx\n",__FUNCTION__));

        if (x_sema_create(&pt_dev->h_async_thrd_mtx,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Create h_done_msgq\n",__FUNCTION__));

        i4_ret = x_msg_q_create(&pt_dev->h_done_msgq,
                                pt_xent->ps_name,
                                sizeof(FM_IO_REQ_T *),
                                256);
        if (i4_ret != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Init 4 queues\n",__FUNCTION__));

        DLIST_INIT(&pt_dev->t_pend_req_q);
        DLIST_INIT(&pt_dev->t_busy_req_q);
        DLIST_INIT(&pt_dev->t_done_req_q);
        DLIST_INIT(&pt_dev->t_abort_req_q);

        DBG_INFO(("[FM] %s Create h_ctrl_mtx\n",__FUNCTION__));

        i4_ret = x_sema_create(&pt_dev->h_ctrl_mtx, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if ( i4_ret != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
    }
#endif
#ifdef SMB_SUPPORT
#if 0
    if (e_drv_type == DRVT_SMB)
    {
        pt_dev->ui2_flags |= FM_DEV_FLAG_ASYNC;

        DBG_INFO(("[FM] %s Create h_queue_mtx\n",__FUNCTION__));

        if (x_sema_create(&pt_dev->h_queue_mtx,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Create h_async_thrd_mtx\n",__FUNCTION__));

        if (x_sema_create(&pt_dev->h_async_thrd_mtx,
                          X_SEMA_TYPE_MUTEX,
                          X_SEMA_STATE_UNLOCK) != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Create h_done_msgq\n",__FUNCTION__));

        i4_ret = x_msg_q_create(&pt_dev->h_done_msgq,
                                pt_xent->ps_name,
                                sizeof(FM_IO_REQ_T *),
                                256);
        if (i4_ret != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
        DBG_INFO(("[FM] %s Init 4 queues\n",__FUNCTION__));

        DLIST_INIT(&pt_dev->t_pend_req_q);
        DLIST_INIT(&pt_dev->t_busy_req_q);
        DLIST_INIT(&pt_dev->t_done_req_q);
        DLIST_INIT(&pt_dev->t_abort_req_q);

        DBG_INFO(("[FM] %s Create h_ctrl_mtx\n",__FUNCTION__));

        i4_ret = x_sema_create(&pt_dev->h_ctrl_mtx, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
        if ( i4_ret != OSR_OK)
        {
            i4_ret = FMR_CORE;
            goto FAIL;
        }
    }
#endif
#endif


    pt_dev->h_driver         = h_driver;
    pt_dev->e_drv_type       = e_drv_type;
    pt_dev->e_device_type    = e_device_type;
    pt_dev->ui2_unit         = ui2_unit;
    pt_dev->ui2_ref_cnt++;

    if (pt_dev->ui2_flags & FM_DEV_FLAG_ASYNC)
    {
        DBG_INFO(("[FM] %s Create async thread\n",__FUNCTION__));
        i4_ret = OSR_OK;
        pt_dev->ui1_thread_state = FM_DEV_THREAD_INIT;
#ifdef DLNA_SUPPORT

        if (pt_dev->e_drv_type == DRVT_DLNA_DMS)
        {
            i4_ret = x_thread_create(&pt_dev->h_thread,
                                     pt_xent->ps_name,
                                     FM_DLNA_DEV_THREAD_DEFAULT_STACK_SIZE,
                                     FM_DLNA_DEV_THREAD_DEFAULT_PRIORITY,
                                     _fm_dev_async_thread,
                                     sizeof(FM_DEVICE_T *),
                                     (VOID *)&pt_dev);
        }
        else
#endif
#ifdef SMB_SUPPORT
#if 0

        if (pt_dev->e_drv_type == DRVT_SMB)
        {
            i4_ret = x_thread_create(&pt_dev->h_thread,
                                     pt_xent->ps_name,
                                     32 * 1024,
                                     230,
                                     _fm_dev_async_thread,
                                     sizeof(FM_DEVICE_T *),
                                     (VOID *)&pt_dev);
        }
        else
#endif
#endif
        {
            pt_dev->ui1_thread_state = FM_DEV_THREAD_RUN;
        }

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
    DBG_INFO(("[FM] %s exit with success\n",__FUNCTION__));

    return FMR_OK;
    
FAIL:

    if (pt_dev != NULL)
    {
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
        if (pt_dev->h_ctrl_mtx != NULL_HANDLE)
        {
            x_sema_delete(pt_dev->h_queue_mtx);
        }
        x_mem_free(pt_dev);
    }
    if (h_driver != NULL_HANDLE)
    {
        if (e_drv_type == DRVT_DLNA_DMS || e_drv_type == DRVT_SMB)
        {
        }
        else
        {
            close(h_driver);
        }
    }
    
    DBG_INFO(("[FM] %s exit with fail%d\n",__FUNCTION__, i4_ret));
    return i4_ret;
}


INT32 fm_dev_kill_thread(FM_DEVICE_T *pt_dev)
{
    if ((pt_dev->ui2_flags & FM_DEV_FLAG_ASYNC) && (pt_dev->ui1_thread_state == FM_DEV_THREAD_RUN))
    {
        x_dbg_stmt("[FM] %s.\n",__FUNCTION__);
        x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);

        pt_dev->ui1_thread_state = FM_DEV_THREAD_KILL;
        /* x_thread_resume(pt_dev->h_thread); */
        x_sema_unlock(pt_dev->h_ctrl_mtx);

        x_dbg_stmt("[FM] %s: waiting for thread over.\n", __FUNCTION__);
        while (pt_dev->ui1_thread_state != FM_DEV_THREAD_OVER)
        {
            
            x_sema_unlock(pt_dev->h_async_thrd_mtx);
            x_thread_delay(1);
            x_sema_lock(pt_dev->h_async_thrd_mtx, X_SEMA_OPTION_WAIT);
        }
        x_dbg_stmt("[FM] %s: thread killed.\n", __FUNCTION__);

        x_sema_unlock(pt_dev->h_async_thrd_mtx);

        /*
         *  Thread had been over. It's safe to free resources now.
         */
    }
    return FMR_OK;
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

    DBG_INFO(("[FM] %s: enter\n ", __FUNCTION__));
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

            fm_dev_kill_thread(pt_dev);

            if (pt_dev->e_drv_type == DRVT_DLNA_DMS || pt_dev->e_drv_type == DRVT_SMB)
            {
                pt_dev->h_driver = NULL_HANDLE;
            }
            else
            {
                DBG_INFO(("[FM] %s: line:%d,close(%d)\n", __FUNCTION__,__LINE__,pt_dev->h_driver));
                close(pt_dev->h_driver);
                DBG_INFO(("[FM] %s: line:%d %s\n", __FUNCTION__,__LINE__, strerror(errno)));
            }
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
            if (pt_dev->h_ctrl_mtx != NULL_HANDLE)
            {
                x_sema_delete(pt_dev->h_queue_mtx);
            }

            if (pt_dev->pt_sector_tbl != NULL)
            {
                int i;
                x_mem_free(pt_dev->pt_sector_tbl->pt_sector_rng);
                for (i=0;i<pt_dev->pt_sector_tbl->ui4_num_entries;i++)
                {
                    x_mem_free(pt_dev->pt_sector_tbl->pt_sector_info[i].pb_bad);
                }
                    
                x_mem_free(pt_dev->pt_sector_tbl->pt_sector_info);
                x_mem_free(pt_dev->pt_sector_tbl);
            }

            x_mem_free(pt_dev);
        }
    }

    _fm_dev_unlock_devlist();
    DBG_INFO(("[FM] %s: exit\n ", __FUNCTION__));

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
            pt_req->ui4_done_byte = ((CHRDEV_LONG_DATA_INFO_T *)pt_req)->ui4_len;
            break;
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

