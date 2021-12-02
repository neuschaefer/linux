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
 * $RCSfile: msgq_lib.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This header file contains handle specific definitions, which are
 *         exported.
 *---------------------------------------------------------------------------*/


#include "os/inc/os.h"
#include "os/memory/mem_lib.h"
#include "os/semaphore/sema_lib.h"
#include "os/thread/thread_lib.h"
#include "os/timer/timer_lib.h"
#include "os/msgQ/msgq_lib.h"
#include "inc/common.h"

#ifdef SYS_MEM_CHK
#undef x_mem_alloc
#undef x_mem_calloc
#undef x_mem_realloc
#undef x_mem_free

extern VOID* x_mem_alloc (SIZE_T  z_size);

extern VOID* x_mem_calloc (UINT32  ui4_num_element,
                           SIZE_T  z_size_element);

extern VOID* x_mem_realloc (VOID*        pv_mem_block,
                            SIZE_T       z_new_size);

extern VOID x_mem_free (VOID*  pv_mem_block);
#endif

BOOL b_msg_q_inited = FALSE;      /* exported to mem_init() */

static OS_MSGQ_T *pt_msg_q_head = NULL;
static OS_MSGQ_T *pt_health_q   = NULL;
static HANDLE_T h_msg_q_sema_hdl = (HANDLE_T) NULL_HANDLE;
static BOOL g_b_health_check = TRUE;

#define MSG_Q_HEALTH_CHECK_DELAY          1024
#define MSG_Q_HEALTH_THRESHOLD            (64 * 1024)
#define MSG_Q_HEALTH_NUM_QUEUES_PER_CHECK  4

/*----------------------------------------------------------------------------
 * Function: msg_q_init()
 *
 * Description:
 *      this routine is called as part of os_init() and creates a semaphore
 *      to guard global msg Q link list.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      others (not OSR_OK): routine failed.
 *---------------------------------------------------------------------------*/
INT32
msg_q_init()
{
    INT32 i4_ret;

    if (h_msg_q_sema_hdl == NULL_HANDLE)
    {
        i4_ret = x_sema_create(&h_msg_q_sema_hdl,
                               X_SEMA_TYPE_BINARY,
                               X_SEMA_STATE_UNLOCK);
        if (i4_ret != 0)
        {
            return(i4_ret);
        }
    }

    b_msg_q_inited = TRUE;
    return(OSR_OK);
}


/*----------------------------------------------------------------------------
 * Function: msg_q_put_msgq_obj()
 *
 * Description:
 *      this routine is called to put a msg Q struct in global msg Q list.
 *      it performs
 *      1. check if msg Q mudule is initiated.
 *      2. guard global msg Q link list.
 *      3. go through global msg Q link list to check for existence.
 *      4. enqueue the pt_msg_q (msg Q struct) at the beginning of the list.
 *
 * Inputs:
 *      *pt_msg_q: pointer to a msgq struct.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : routine was successful.
 *      OSR_EXIST : msg Q exists.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
INT32
msg_q_put_msgq_obj(OS_MSGQ_T *pt_msg_q)
{
    OS_MSGQ_T *pt_tmp;
    INT32 i4_ret, i4_found = OSR_OK;

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret == OSR_OK)
    {
        GET_LINK_HEAD(pt_tmp, pt_msg_q_head);
        while (LINK_NOT_EMPTY(pt_tmp, NULL))
        {
            /* check for dup */
            if (x_strncmp(pt_tmp->s_qname, pt_msg_q->s_qname, MSGQ_NAME_LEN) == 0)
            {
                i4_found = OSR_EXIST;
                break;
            }

            GET_LINK_NEXT(pt_tmp, pt_tmp);
        }

        if (i4_found == OSR_OK)
        {
            /* put msg q object at the head anyway */
            SET_LINK_NEXT(pt_msg_q, pt_msg_q_head);
            SET_LINK_HEAD(pt_msg_q_head, pt_msg_q);
            pt_msg_q->i1_refcount = 1;
        }

        (VOID) x_sema_unlock(h_msg_q_sema_hdl);

        return(i4_found);
    }
    else
    {
        return(i4_ret);
    }
}


/*----------------------------------------------------------------------------
 * Function: msg_q_get_msgq_obj()
 *
 * Description:
 *      this routine called to get a msg Q struct by its name. it performs
 *      1. check if msg Q mudule is initiated.
 *      2. guard global msg Q link list.
 *      3. go through global msg Q link list to look for a msg Q based on
 *         "ps_name", up to 16 characters.
 *
 * Inputs:
 *      *ps_name: name of the msg Q.
 *
 * Outputs: -
 *
 * Returns:
 *      NULL : OS driver failed.
 *      non NULL : pointer to a msgq struct.
 *---------------------------------------------------------------------------*/
OS_MSGQ_T *
msg_q_get_msgq_obj(const CHAR *ps_name)
{
    OS_MSGQ_T *pt_tmp = NULL;

    if (x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT) == OSR_OK)
    {
        GET_LINK_HEAD(pt_tmp, pt_msg_q_head);
        while (LINK_NOT_EMPTY(pt_tmp, NULL))
        {
            if (x_strncmp(pt_tmp->s_qname, ps_name, MSGQ_NAME_LEN) == 0)
            {
                pt_tmp->i1_refcount++;
                break;
            }

            GET_LINK_NEXT(pt_tmp, pt_tmp);
        }

        (VOID) x_sema_unlock(h_msg_q_sema_hdl);
    }

    return(pt_tmp);
}


/*----------------------------------------------------------------------------
 * Function: msg_q_remove_msgq_obj()
 *
 * Description:
 *      this routine is called to remove a msg Q struct from global msg Q list.
 *      it performs
 *      1. check if msg Q mudule is initiated.
 *      2. guard global msg Q link list.
 *      3. decrement refcount of msg Q struct and return if non-zero.
 *      4. go through global msg Q link list to look for pt_msg_q and delink it.
 *      5. resume thread waiting on this msg Q and free msg Q struct.
 *
 * Inputs:
 *      *pt_msg_q: pointer to a msg Q struct.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_NOT_INIT : routine was successful.
 *      OSR_EXIST : msg Q esists.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
INT32
msg_q_remove_msgq_obj(OS_MSGQ_T *pt_msg_q)
{
    OS_MSGQ_T *pt_tmp, *pt_prev = NULL;
    INT32 i4_ret;
    BOOL b_last = FALSE;

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret == OSR_OK)
    {
        pt_msg_q->i1_refcount--;
        if (pt_msg_q->i1_refcount <= 0)     /* remove upon last deletion */
        {
            GET_LINK_HEAD(pt_tmp, pt_msg_q_head);
            while (LINK_NOT_EMPTY(pt_tmp, NULL))
            {
                if (pt_tmp == pt_msg_q)
                {
                    /* find the msg q object and delink it */
                    if (LINK_IS_EMPTY(pt_prev, NULL))
                    {
                        SET_LINK_HEAD(pt_msg_q_head, pt_tmp->pt_next);
                    }
                    else
                    {
                        SET_LINK_NEXT(pt_prev, pt_tmp->pt_next);
                    }

                    SET_LINK_NEXT(pt_msg_q, NULL);
                    i4_ret = OSR_OK;
                    b_last = TRUE;
                    break;
                }

                pt_prev = pt_tmp;
                GET_LINK_NEXT(pt_tmp, pt_tmp);
            }
        }

        if (pt_health_q == pt_msg_q)
        {
            pt_health_q = pt_health_q->pt_next;
        }
    
        (VOID) x_sema_unlock(h_msg_q_sema_hdl);

        if (b_last == TRUE)
        {
            x_mem_free(pt_msg_q);
        }

        return(OSR_OK);
    }
    else
    {
        return(i4_ret);
    }
}


/*----------------------------------------------------------------------------
 * Function: msg_q_put_msg()
 *
 * Description:
 *      this routine is called to enqueue (send) a message to a msg Q.
 *      it performs
 *      1. check if too many messages already queued.
 *      2. copy message data to internal msg Q buffer.
 *      3. enqueue the message in ascending order or priority.
 *      4. resume thread waiting on msg Q.
 *
 * Inputs:
 *      pt_msg_q: pointer to a msg Q struct.
 *      pv_msg: pointer to message data.
 *      z_size: message size
 *      ui1_pri: message priority, 0-highest, 255-lowest.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_TOO_MANY : too many messages in msg Q.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
INT32
msg_q_put_msg(OS_MSGQ_T    *pt_msg_q,   /* msg q object */
              const VOID   *pv_msg,     /* msg data */
              SIZE_T       z_size,      /* msg size */
              UINT8        ui1_pri)
{
    OS_MSGQ_DATA_T *pt_msgdata, *pt_msg, *pt_prev, *pt_qbegin;
    OS_MSGQ_WAIT_T *pt_wchain;
    CHAR *pc_data;
    CRIT_STATE_T t_crit;
    UINT16 ui2_i, ui2_j, ui2_count;
    UINT8 ui1_p;

    t_crit = x_os_drv_crit_start();

    if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
        (pt_msg_q->ui2_qcount == 0))
    {
        if ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL)
        {
            pt_msg_q->pt_qwaittail = NULL;
        }
        /*x_os_drv_crit_end(t_crit);*/

        if (pt_wchain->z_bsize < z_size)
        {
            z_size = pt_wchain->z_bsize;
        }

        pt_wchain->z_dsize = z_size;
        x_memcpy(pt_wchain->pv_data, pv_msg, z_size);

        x_os_drv_crit_end(t_crit);        
        (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);

        return(OSR_OK);
    }

    pt_qbegin = (OS_MSGQ_DATA_T *) (pt_msg_q + 1);
    ui2_count = pt_msg_q->ui2_maxcount;
    if ((ui2_i = pt_msg_q->ui2_bufhead) < ui2_count)
    {
        pt_msgdata = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_i);
        pt_msg_q->ui2_bufhead = MSGQ_GET_MSG_INDEX(pt_msgdata->ui4_qdhdr);

        /*x_os_drv_crit_end(t_crit);*/

        pc_data = (CHAR *) (pt_qbegin + ui2_count);
        pc_data = (CHAR *) (pc_data + (pt_msg_q->z_maxsize * ui2_i));
 
        x_memcpy(pc_data, pv_msg, z_size);

        /*t_crit = x_os_drv_crit_start();*/
        if ((ui2_j = pt_msg_q->ui2_qhead) >= ui2_count)
        {
            pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, ui1_pri);
            pt_msg_q->ui2_qtail = pt_msg_q->ui2_qhead = ui2_i;
        }
        else
        {
            ui2_j = pt_msg_q->ui2_qtail;
            pt_msg = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_j);
            ui1_p = MSGQ_GET_PRI(pt_msg->ui4_qdhdr);
            if (ui1_pri >= ui1_p)
            {
                UINT16 ui2_s;

                ui2_s = MSGQ_GET_DATA_SIZE(pt_msg->ui4_qdhdr);
                pt_msg->ui4_qdhdr = MSGQ_SET_QDHDR(ui2_s, ui2_i, ui1_p);
                pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_count, ui1_pri);
                pt_msg_q->ui2_qtail = ui2_i;
            }
            else
            {
                pt_prev = NULL;
                ui2_j = pt_msg_q->ui2_qhead;
                while (ui2_j < ui2_count)
                {
                    pt_msg = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_j);
                    ui1_p = MSGQ_GET_PRI(pt_msg->ui4_qdhdr);
                    if (ui1_pri < ui1_p)
                    {
                        pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, ui1_pri);

                        if (!pt_prev)
                        {
                            pt_msg_q->ui2_qhead = ui2_i;
                        }
                        else
                        {
                            ui1_p = MSGQ_GET_PRI(pt_prev->ui4_qdhdr);
                            ui2_j = MSGQ_GET_DATA_SIZE(pt_prev->ui4_qdhdr);
                            pt_prev->ui4_qdhdr = MSGQ_SET_QDHDR(ui2_j, ui2_i, ui1_p);
                        }
                        break;
                    }
                    pt_prev = pt_msg;
                    ui2_j = MSGQ_GET_MSG_INDEX(pt_msg->ui4_qdhdr);
                }
            }
        }

        pt_msg_q->ui2_qcount++;

        if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
            ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL))
        {
            pt_msg_q->pt_qwaittail = NULL;
        }

        x_os_drv_crit_end(t_crit);
        if (pt_wchain)
        {
            (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);
        }

        return(OSR_OK);
    }
    else
    {
        x_os_drv_crit_end(t_crit);
        return(OSR_TOO_MANY);
    }
}

/*----------------------------------------------------------------------------
 * Function: msg_q_put_msg_to_front()
 *
 * Description:
 *      this routine is called to enqueue (send) a message to the front of a msg Q.
 *      it performs
 *      1. check if too many messages already queued.
 *      2. copy message data to internal msg Q buffer.
 *      3. enqueue the message in ascending order or priority.
 *      4. resume thread waiting on msg Q.
 *
 * Inputs:
 *      pt_msg_q: pointer to a msg Q struct.
 *      pv_msg: pointer to message data.
 *      z_size: message size
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_TOO_MANY : too many messages in msg Q.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
INT32
msg_q_put_msg_to_front(OS_MSGQ_T    *pt_msg_q,   /* msg q object */
                       const VOID   *pv_msg,     /* msg data */
                       SIZE_T       z_size)
{
    OS_MSGQ_DATA_T *pt_msgdata, *pt_qbegin;
    OS_MSGQ_WAIT_T *pt_wchain;
    CHAR *pc_data;
    CRIT_STATE_T t_crit;
    UINT16 ui2_i, ui2_j, ui2_count;
    
    t_crit = x_os_drv_crit_start();

    if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
        (pt_msg_q->ui2_qcount == 0))
    {
        if ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL)
        {
            pt_msg_q->pt_qwaittail = NULL;
        }
        x_os_drv_crit_end(t_crit);

        if (pt_wchain->z_bsize < z_size)
        {
            z_size = pt_wchain->z_bsize;
        }

        pt_wchain->z_dsize = z_size;
        x_memcpy(pt_wchain->pv_data, pv_msg, z_size);
        (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);

        return(OSR_OK);
    }

    pt_qbegin = (OS_MSGQ_DATA_T *) (pt_msg_q + 1);
    ui2_count = pt_msg_q->ui2_maxcount;
    if ((ui2_i = pt_msg_q->ui2_bufhead) < ui2_count)
    {
        pt_msgdata = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_i);
        pt_msg_q->ui2_bufhead = MSGQ_GET_MSG_INDEX(pt_msgdata->ui4_qdhdr);

        x_os_drv_crit_end(t_crit);

        pc_data = (CHAR *) (pt_qbegin + ui2_count);
        pc_data = (CHAR *) (pc_data + (pt_msg_q->z_maxsize * ui2_i));
 
        x_memcpy(pc_data, pv_msg, z_size);

        t_crit = x_os_drv_crit_start();
        if ((ui2_j = pt_msg_q->ui2_qhead) >= ui2_count)
        {
            pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, 0);
            pt_msg_q->ui2_qtail = pt_msg_q->ui2_qhead = ui2_i;
        }
        else
        {
            ui2_j = pt_msg_q->ui2_qhead;
            pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, 0);

            pt_msg_q->ui2_qhead = ui2_i;
        }

        pt_msg_q->ui2_qcount++;

        if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
            ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL))
        {
            pt_msg_q->pt_qwaittail = NULL;
        }

        x_os_drv_crit_end(t_crit);
        if (pt_wchain)
        {
            (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);
        }

        return(OSR_OK);
    }
    else
    {
        x_os_drv_crit_end(t_crit);
        return(OSR_TOO_MANY);
    }
}

/*----------------------------------------------------------------------------
 * Function: msg_q_put_msg_2_parts()
 *
 * Description:
 *      this routine is called to enqueue (send) a message to a msg Q. The
 *      2 input buffers will be concatenated into one single message. 
 *      it performs
 *      1. check if too many messages already queued.
 *      2. copy message data to internal msg Q buffer.
 *      3. enqueue the message in ascending order or priority.
 *      4. resume thread waiting on msg Q.
 *
 * Inputs:
 *      pt_msg_q: pointer to a msg Q struct.
 *      pv_msg_1: pointer to message data (1st part).
 *      z_size_1: 1st-part data size
 *      pv_msg_2: pointer to message data (2nd part).
 *      z_size_2: 2nd-part data size
 *      ui1_pri: message priority, 0-highest, 255-lowest.
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_TOO_MANY : too many messages in msg Q.
 *      OSR_FAIL : OS driver failed.
 *---------------------------------------------------------------------------*/
static INT32
msg_q_put_msg_2_parts(OS_MSGQ_T*   pt_msg_q,   /* msg q object */
                      const VOID*  pv_msg_1,   /* msg data 1 */
                      SIZE_T       z_size_1,   /* msg size 1 */
                      const VOID*  pv_msg_2,   /* msg data 2 */
                      SIZE_T       z_size_2,   /* msg size 2 */
                      UINT8        ui1_pri)
{
    OS_MSGQ_DATA_T *pt_msgdata, *pt_msg, *pt_prev, *pt_qbegin;
    OS_MSGQ_WAIT_T *pt_wchain;
    CHAR *pc_data;
    SIZE_T z_size;
    CRIT_STATE_T t_crit;
    UINT16 ui2_i, ui2_j, ui2_count;
    UINT8 ui1_p;

    if (!pv_msg_2)
    {
        z_size_2 = 0;
    }
    z_size = z_size_1 + z_size_2;

    t_crit = x_os_drv_crit_start();

    if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
        (pt_msg_q->ui2_qcount == 0))
    {
        if ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL)
        {
            pt_msg_q->pt_qwaittail = NULL;
        }
        /*x_os_drv_crit_end(t_crit);*/

        if (pt_wchain->z_bsize < z_size)
        {
            z_size = pt_wchain->z_bsize;
            z_size_2 = (z_size > z_size_1) ? (z_size - z_size_1) : 0;
        }
        pt_wchain->z_dsize = z_size;
        x_memcpy(pt_wchain->pv_data, pv_msg_1, z_size_1);

        if (z_size_2)
        {
            x_memcpy(((UINT8 *) pt_wchain->pv_data) + z_size_1, pv_msg_2, z_size_2);
        }

        x_os_drv_crit_end(t_crit);        
        (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);

        return(OSR_OK);
    }

    pt_qbegin = (OS_MSGQ_DATA_T *) (pt_msg_q + 1);
    ui2_count = pt_msg_q->ui2_maxcount;
    if ((ui2_i = pt_msg_q->ui2_bufhead) < ui2_count)
    {
        pt_msgdata = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_i);
        pt_msg_q->ui2_bufhead = MSGQ_GET_MSG_INDEX(pt_msgdata->ui4_qdhdr);

        /*x_os_drv_crit_end(t_crit);*/

        pc_data = (CHAR *) (pt_qbegin + ui2_count);
        pc_data = (CHAR *) (pc_data + (pt_msg_q->z_maxsize * ui2_i));
 
        x_memcpy(pc_data, pv_msg_1, z_size_1);

        if (z_size_2)
        {
            x_memcpy(pc_data + z_size_1, pv_msg_2, z_size_2);
        }

        /*t_crit = x_os_drv_crit_start();*/
        if ((ui2_j = pt_msg_q->ui2_qhead) >= ui2_count)
        {
            pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, ui1_pri);
            pt_msg_q->ui2_qtail = pt_msg_q->ui2_qhead = ui2_i;
        }
        else
        {
            ui2_j = pt_msg_q->ui2_qtail;
            pt_msg = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_j);
            ui1_p = MSGQ_GET_PRI(pt_msg->ui4_qdhdr);
            if (ui1_pri >= ui1_p)
            {
                UINT16 ui2_s;

                ui2_s = MSGQ_GET_DATA_SIZE(pt_msg->ui4_qdhdr);
                pt_msg->ui4_qdhdr = MSGQ_SET_QDHDR(ui2_s, ui2_i, ui1_p);
                pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_count, ui1_pri);
                pt_msg_q->ui2_qtail = ui2_i;
            }
            else
            {
                pt_prev = NULL;
                ui2_j = pt_msg_q->ui2_qhead;
                while (ui2_j < ui2_count)
                {
                    pt_msg = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_j);
                    ui1_p = MSGQ_GET_PRI(pt_msg->ui4_qdhdr);
                    if (ui1_pri < ui1_p)
                    {
                        pt_msgdata->ui4_qdhdr = MSGQ_SET_QDHDR(z_size, ui2_j, ui1_pri);

                        if (!pt_prev)
                        {
                            pt_msg_q->ui2_qhead = ui2_i;
                        }
                        else
                        {
                            ui1_p = MSGQ_GET_PRI(pt_prev->ui4_qdhdr);
                            ui2_j = MSGQ_GET_DATA_SIZE(pt_prev->ui4_qdhdr);
                            pt_prev->ui4_qdhdr = MSGQ_SET_QDHDR(ui2_j, ui2_i, ui1_p);
                        }
                        break;
                    }
                    pt_prev = pt_msg;
                    ui2_j = MSGQ_GET_MSG_INDEX(pt_msg->ui4_qdhdr);
                }
            }
        }

        pt_msg_q->ui2_qcount++;

        if (((pt_wchain = pt_msg_q->pt_qwaithead) != NULL) &&
            ((pt_msg_q->pt_qwaithead = pt_wchain->pt_next) == NULL))
        {
            pt_msg_q->pt_qwaittail = NULL;
        }

        x_os_drv_crit_end(t_crit);
        if (pt_wchain)
        {
            (VOID) x_sema_unlock(((OS_THREAD_T *) pt_wchain->pt_thread)->h_sema_hdl);
        }

        return(OSR_OK);
    }
    else
    {
        x_os_drv_crit_end(t_crit);
        return(OSR_TOO_MANY);
    }
}

/*----------------------------------------------------------------------------
 * Function: msg_q_send_2_parts()
 *
 * Description:
 *      this API sends a message to a message queue. The message is made of 2
 *      parts, which will be concatenated into one single message.
 *
 * Inputs:
 *      h_msg_hdl: msg Q handle returned via x_msg_q_create() or
 *                 x_msg_q_attach().
 *      pv_msg_1: pointer to the 1st part of the message data.
 *      z_size_1: size in bytes of "pv_msg_1".
 *      pv_msg_2: pointer to the 2nd part of the message data.
 *      z_size_2: size in bytes of "pv_msg_2".
 *      ui1_pri: priority of this sending message, 0 (highest) - 255 (lowest).
 *
 * Outputs: -
 *
 * Returns:
 *      OSR_OK : routine was successful. 
 *      OSR_NOT_INIT : message Q module has not been initiated. 
 *      OSR_INV_ARG: an error in the arguments of this API.
 *      OSR_TOO_BIG: the sending message is bigger than allowed, specified
 *                   in x_msg_q_create().
 *      OSR_TOO_MANY: too many messages queued in the msg Q, more that allowed,
 *                    specified in x_msg_q_create().
 *      OSR_INV_HANDLE: an error in handle operation.
 *---------------------------------------------------------------------------*/
INT32
msg_q_send_2_parts(HANDLE_T     h_msg_hdl,      /* msg q handle */
                   const VOID*  pv_msg_1,
                   SIZE_T       z_size_1,
                   const VOID*  pv_msg_2,
                   SIZE_T       z_size_2,
                   UINT8        ui1_pri)
{
    OS_MSGQ_T *pt_msg_q;    /* msg q object */
    HANDLE_TYPE_T e_type;

    if (pv_msg_1 && z_size_1)
    {
        if ((handle_get_type_obj(h_msg_hdl, &e_type, (VOID **) &pt_msg_q) == HR_OK) &&
            (e_type == HT_GROUP_OS_MSGQ))
        {
            if ((z_size_1 + z_size_2) <= pt_msg_q->z_maxsize)
            {
                return( msg_q_put_msg_2_parts(pt_msg_q, pv_msg_1, z_size_1,
                                              pv_msg_2, z_size_2, ui1_pri) );
            }
            else
            {
                return(OSR_TOO_BIG);
            }
        }
        else
        {
            return(OSR_INV_HANDLE);
        }
    }
    else
    {
        return(OSR_INV_ARG);
    }
}

/*----------------------------------------------------------------------------
 * Function: msg_q_get_msg()
 *
 * Description:
 *      this routine is called to get(receive) a message from a msg Q.
 *      it performs
 *      1. go through waitchain in pt_poll for valid msg Q that has not had
 *         thread waiting on.
 *      2. if msg Q has messages, get the first one from head and adjust 
 *         msg Q message head, otherwise, move the next waitchain in pt_poll.
 *      3. if no message at all and e_option is NOWAIT, returns no message.
 *      4. enqueue each waitchain in pt_poll to its msg Q and suspend the
 *         thread. 
 *
 * Inputs:
 *      pt_poll: pointer to poll struct containg msg Q's to receive message from
 *      pv_msgdata: pointer to receive buffer.
 *      pz_size: pointer contains size of pv_msgdata.
 *      e_option: either X_MSGQ_OPTION_WAIT or X_MSGQ_OPTION_NOWAIT.
 *
 * Outputs: -
 *      pv_msgdata: pointer to received message.
 *      pz_size: pointer contains size of received message.
 *      pui2_index: pointer contains index to waitchain table in pt_poll.
 *
 * Returns:
 *      OSR_OK : routine was successful.
 *      OSR_BUSY : other thread waiting on a msg Q.
 *      OSR_FAIL : OS driver failed.
 *      OSR_INV_HANDLE : invalid handle.
 *---------------------------------------------------------------------------*/
INT32
msg_q_get_msg(VOID           *pv_msgdata,
              SIZE_T         *pz_size,
              MSGQ_OPTION_T  e_option,
              OS_MSGQ_T      *pt_msg_q,
              UINT32         ui4_time)
{
    OS_MSGQ_DATA_T *pt_msg, *pt_qbegin;
    OS_THREAD_T *pt_thread;
    CHAR *pc_data;
    OS_MSGQ_WAIT_T t_wchain;
    INT32 i4_polling, i4_ret;
    CRIT_STATE_T t_crit;
    UINT16 ui2_i, ui2_j;
    UINT16 ui2_count;

    i4_ret = OSR_OK;
    i4_polling = 0;
    pt_qbegin = (OS_MSGQ_DATA_T *) (pt_msg_q + 1);
    ui2_count = pt_msg_q->ui2_maxcount;
    do
    {
        t_crit = x_os_drv_crit_start();
        if (pt_msg_q->ui2_qcount)
        {
            if ((ui2_j = pt_msg_q->ui2_qhead) < ui2_count)
            {
                pt_msg_q->ui2_qcount--;
                pt_msg = (OS_MSGQ_DATA_T *) (pt_qbegin + ui2_j);
                pt_msg_q->ui2_qhead = MSGQ_GET_MSG_INDEX(pt_msg->ui4_qdhdr);
                ui2_i = MSGQ_GET_DATA_SIZE(pt_msg->ui4_qdhdr);
                /*x_os_drv_crit_end(t_crit);*/

                pc_data = (CHAR *) (pt_qbegin + ui2_count);
                pc_data = (CHAR *) (pc_data + (pt_msg_q->z_maxsize * ui2_j));
                x_memcpy(pv_msgdata, pc_data, ui2_i);
                *(SIZE_T *) pz_size = (SIZE_T) ui2_i;

                /*t_crit = x_os_drv_crit_start();*/
                ui2_i = pt_msg_q->ui2_bufhead;
                pt_msg->ui4_qdhdr = MSGQ_INIT_QDHDR(ui2_i);
                pt_msg_q->ui2_bufhead = ui2_j;
                x_os_drv_crit_end(t_crit);

                return(OSR_OK);
            }
        }
        else if (e_option == X_MSGQ_OPTION_NOWAIT)
        {
            x_os_drv_crit_end(t_crit);
            return(OSR_NO_MSG);
        }
        else if (i4_ret != OSR_OK)
        {
            x_os_drv_crit_end(t_crit);
            return(i4_ret);
        }

        if (!i4_polling)
        {
            VOID *pv_th_id;

            i4_polling = 1;
            (VOID) x_os_drv_thread_self(&pv_th_id,
                                        (VOID**) &pt_thread);
            if (pt_thread == NULL)
            {
                x_os_drv_crit_end(t_crit);
                return(OSR_FAIL);
            }
            t_wchain.pt_thread = (VOID *) pt_thread;
            t_wchain.pt_next = NULL;
            t_wchain.pv_data = pv_msgdata;
            t_wchain.z_bsize = *(SIZE_T *) pz_size;
            t_wchain.z_dsize = 0;
        }

        if (pt_msg_q->pt_qwaithead == NULL)
        {
            pt_msg_q->pt_qwaittail = pt_msg_q->pt_qwaithead = &t_wchain;
        }
        else
        {
            pt_msg_q->pt_qwaittail->pt_next = &t_wchain;
            pt_msg_q->pt_qwaittail = &t_wchain;
        }
        x_os_drv_crit_end(t_crit);

        if (!ui4_time)
        {
            i4_ret = x_sema_lock(pt_thread->h_sema_hdl, X_SEMA_OPTION_WAIT);
        }
        else
        {
            i4_ret = x_sema_lock_timeout(pt_thread->h_sema_hdl, ui4_time);
        }

        if (t_wchain.z_dsize)
        {
            *(SIZE_T *) pz_size = t_wchain.z_dsize;
            return(OSR_OK);
        }
        else
        {
            OS_MSGQ_WAIT_T *pt_c, *pt_p;

            pt_p = NULL;
            t_crit = x_os_drv_crit_start();

            if (t_wchain.z_dsize)
            {
                *(SIZE_T *) pz_size = t_wchain.z_dsize;
                x_os_drv_crit_end(t_crit);
                return(OSR_OK);
            }

            pt_c = pt_msg_q->pt_qwaithead;
            while (pt_c)
            {
                if (pt_c == &t_wchain)
                {
                    if (!pt_p)
                    {
                        pt_msg_q->pt_qwaithead = pt_c->pt_next;
                    }
                    else
                    {
                        pt_p->pt_next = pt_c->pt_next;
                    }
                    if (pt_c->pt_next == NULL)
                    {
                        pt_msg_q->pt_qwaittail = pt_p;
                    }
                    break;
                }
                pt_p = pt_c;
                pt_c = pt_c->pt_next;
            }
            x_os_drv_crit_end(t_crit);
            return(i4_ret);
        }
    } while (1);
}


/*----------------------------------------------------------------------------
 * Function: msg_q_free_cb()
 *
 * Description:
 *      this routine is called via handle_free(). it performs
 *      1. check e_type is MSGQ.
 *      2. check timer flag to make sure it comes from proper path of
 *         calling handle_free() and then free timer struct object.
 *
 * Inputs:
 *      h_hdl : handle.
 *      e_type : handle type.
 *      pv_obj : handle object.
 *      pv_tag : handle private tag.
 *      b_req_hdl : TRUE or FALSE.
 *
 * Outputs: -
 *
 * Returns:
 *      TRUE : routine was successful. 
 *      FALSE : routine failed.
 *---------------------------------------------------------------------------*/
BOOL
msg_q_free_cb(HANDLE_T       h_hdl,
              HANDLE_TYPE_T  e_type,
              VOID *         pv_obj,
              VOID *         pv_tag,
              BOOL           b_req_hdl)
{
    OS_MSGQ_T *pt_msg_q;    /* msg q object */
    INT32 i4_ret;

    if (e_type != HT_GROUP_OS_MSGQ)
    {
        return(FALSE);
    }

    pt_msg_q = (OS_MSGQ_T *) pv_obj;

    if (pt_msg_q->e_flags & X_MSGQ_FLAG_CLOSE)
    {
        i4_ret = handle_delink(&pt_msg_q->t_hdl_link, h_hdl);
        if (i4_ret != HR_OK)
        {
            return(FALSE);
        }
        return(TRUE);
    }
    else
    {
        return(FALSE);
    }
}

/*----------------------------------------------------------------------------
 * Function: msg_q_health_check
 *
 * Description:
 *      this routine pauses or resumes health checking.
 *
 * Inputs:
 *      b_enable  Specifies whether health checking should be enabled or not.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID msg_q_health_check (BOOL  b_check)
{
    g_b_health_check = b_check;
}

/*----------------------------------------------------------------------------
 * Function: msg_q_health_check_main
 *
 * Description:
 *      this routine is the health checking daemon's main function.
 *
 * Inputs:
 *      pv_arg: ignored
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
static VOID msg_q_health_check_main (VOID*  pv_arg)
{
    UINT32  ui4_counter = 0;
    
    while (TRUE)
    {
        if (g_b_health_check)
        {
            UINT32  ui4_i;
            INT32   i4_res;
            
            if (++ui4_counter == 0)
            {
                ui4_counter++;
            }
            
            i4_res = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
            
            if (i4_res == OSR_OK)
            {
                for (ui4_i = 0;
                     ui4_i < MSG_Q_HEALTH_NUM_QUEUES_PER_CHECK;
                     ui4_i++)
                {
                    if (pt_health_q == NULL)
                    {
                        if ((pt_health_q = pt_msg_q_head) == NULL)
                        {
                            break;
                        }
                    }
                    
                    if (pt_health_q->b_health_check)
                    {
                        CRIT_STATE_T  t_state;
                        
                        t_state = x_crit_start();
                        
                        if ((pt_health_q->ui2_qcount == 0)
                            /*&&
                              (pt_health_q->pt_qwaithead != NULL)*/)
                        {
                            pt_health_q->ui4_old_recv_counter =
                                pt_health_q->ui4_recv_counter;
                            
                            pt_health_q->ui4_health_counter = ui4_counter;
                            
                            x_crit_end(t_state);
                        }
                        else if (pt_health_q->ui4_recv_counter
                                 !=
                                 pt_health_q->ui4_old_recv_counter)
                        {
                            pt_health_q->ui4_old_recv_counter =
                                pt_health_q->ui4_recv_counter;
                            
                            pt_health_q->ui4_health_counter = ui4_counter;
                            
                            x_crit_end(t_state);
                        }
                        else
                        {
                            UINT32  ui4_diff;
                            
                            ui4_diff  = ui4_counter;
                            ui4_diff -= pt_health_q->ui4_health_counter;
                            
                            x_crit_end(t_state);
                            
                            ui4_diff *= MSG_Q_HEALTH_CHECK_DELAY;
                            
                            if (ui4_diff > MSG_Q_HEALTH_THRESHOLD)
                            {
                                /* unhealthy - abort */
                                
                                DBG_ERROR(("Dead queue \"%s\" (%u ms)\n",
                                           pt_health_q->s_qname, ui4_diff));
                                
                                DBG_ABORT(DBG_MOD_OS);
                            }
                            else
                            {
                                DBG_INFO(("No activity on \"%s\", %u ms "
                                          "(%u,%p,%u,%u)\n",
                                          pt_health_q->s_qname, ui4_diff,
                                          pt_health_q->ui2_qcount,
                                          pt_health_q->pt_qwaithead,
                                          pt_health_q->ui4_recv_counter,
                                          pt_health_q->ui4_old_recv_counter));
                            }
                        }
                    }
                    
                    pt_health_q = pt_health_q->pt_next;
                }
                
                x_sema_unlock(h_msg_q_sema_hdl);
            }
        }
        
        x_thread_delay(MSG_Q_HEALTH_CHECK_DELAY);
    }
}

/*----------------------------------------------------------------------------
 * Function: x_msg_q_health_check
 *
 * Description: This API enables or disables health checking on a specific
 *              message queue.
 *
 * Inputs: ps_name   References the name of the queue.
 *         b_enable  Specifies whether health checking should be enabled
 *
 * Outputs: -
 *
 * Returns: OSR_OK                Heath check set successfully.
 *          OSR_NOT_EXIST         No message queue with such name.
 *          Other negative value  Other error.
 *---------------------------------------------------------------------------*/
INT32 x_msg_q_health_check (const CHAR*  ps_name,
                            BOOL         b_enable)
{
    INT32  i4_res;
    
    i4_res = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    
    if (i4_res == OSR_OK)
    {
        OS_MSGQ_T*  pt_queue;
        
        i4_res = OSR_NOT_EXIST;
        
        GET_LINK_HEAD(pt_queue, pt_msg_q_head);
        
        while (LINK_NOT_EMPTY(pt_queue, NULL))
        {
            i4_res = x_strncmp(pt_queue->s_qname, ps_name, MSGQ_NAME_LEN);
            
            if (i4_res == 0)
            {
                pt_queue->b_health_check = b_enable;
                
                i4_res = OSR_OK;
                
                break;
            }
            
            GET_LINK_NEXT(pt_queue, pt_queue);
        }
        
        x_sema_unlock(h_msg_q_sema_hdl);
    }
    
    return i4_res;
}

/*----------------------------------------------------------------------------
 * Function: x_msg_q_health_check_start
 *
 * Description: This API starts the health checking daemon.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: OSR_OK                Daemon started successfully.
 *          Other negative value  Other error.
 *---------------------------------------------------------------------------*/
INT32 x_msg_q_health_check_start (VOID)
{
    HANDLE_T  h_thread;
    INT32     i4_res;
    
    i4_res = x_thread_create(& h_thread,
                             MSG_Q_HEALTH_CHECK_THREAD_NAME,
                             MSG_Q_HEALTH_CHECK_THREAD_STACK_SIZE,
                             (UINT8) MSG_Q_HEALTH_CHECK_THREAD_PRIORITY,
                             msg_q_health_check_main,
                             0,
                             NULL);
    
    return i4_res;
}
