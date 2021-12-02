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


#include "os.h"
#include "x_linux.h"
#include "x_hal_arm.h"
#include "../thread/thread_lib.h"
#include "../timer/timer_lib.h"
#include "msgq_lib.h"

BOOL b_msg_q_inited = FALSE;      /* exported to mem_init() */

static OS_MSGQ_T *pt_msg_q_head = NULL;
static HANDLE_T h_msg_q_sema_hdl = (HANDLE_T) 0;


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

    if (h_msg_q_sema_hdl == 0)
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

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    GET_LINK_HEAD(pt_tmp, pt_msg_q_head);
    while (LINK_NOT_EMPTY(pt_tmp, NULL))
    {
        /* check for dup */
        i4_ret = x_strncmp(pt_tmp->s_qname,
                           pt_msg_q->s_qname,
                           MSGQ_NAME_LEN);
        if (i4_ret == 0)
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

    i4_ret = x_sema_unlock(h_msg_q_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    return(i4_found);
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
    OS_MSGQ_T *pt_tmp;
    INT32 i4_ret;

    if (b_thread_inited != TRUE)
    {
        return(NULL);
    }

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(NULL);
    }

    GET_LINK_HEAD(pt_tmp, pt_msg_q_head);
    while (LINK_NOT_EMPTY(pt_tmp, NULL))
    {
        i4_ret = x_strncmp(pt_tmp->s_qname, ps_name, MSGQ_NAME_LEN);
        if (i4_ret == 0)
        {
            pt_tmp->i1_refcount++;
            break;
        }

        GET_LINK_NEXT(pt_tmp, pt_tmp);
    }

    i4_ret = x_sema_unlock(h_msg_q_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
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
    CRIT_STATE_T t_crit;
    BOOL b_last = FALSE;

    if (b_thread_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

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

    i4_ret = x_sema_unlock(h_msg_q_sema_hdl);
    if (i4_ret != OSR_OK)
    {
        OS_DBG_ABORT(DBG_CAT_SEMAPHORE, DBG_SEMA_UNLOCK_FAIL);
    }

    if (b_last == TRUE)
    {
        t_crit = x_crit_start();
        if (LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL))
        {
            pt_msg_q->pt_qwaithead->pt_msg_q = NULL;
            SET_LINK_HEAD(pt_msg_q->pt_qwaithead, NULL);
            msg_q_resume_thread(pt_msg_q);
        }
        x_crit_end(t_crit);

        x_mem_free(pt_msg_q);
    }

    return(OSR_OK);
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
    OS_MSGQ_DATA_T *pt_msgdata, *pt_msg, *pt_prev;
    CHAR *pc_data;
    CRIT_STATE_T t_crit;
    INT32 i4_ret;

    i4_ret = OSR_OK;

    t_crit = x_crit_start();

    /* get a data buffer */
    GET_LINK_HEAD(pt_msgdata, pt_msg_q->pt_bufhead);
    if (LINK_NOT_EMPTY(pt_msgdata, NULL))
    {
        SET_LINK_HEAD(pt_msg_q->pt_bufhead, pt_msgdata->pt_next);
    }
    else
    {
        if (LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL))
        {
            msg_q_resume_thread(pt_msg_q);
        }

        i4_ret = OSR_TOO_MANY;
    }
    x_crit_end(t_crit);

    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    /* store this msg in the msgdata struct */
    pc_data = (CHAR *) (pt_msgdata + 1);
    x_memcpy(pc_data, pv_msg, z_size);
    pt_msgdata->z_size = z_size;
    pt_msgdata->ui1_pri = ui1_pri;

    pt_prev = NULL;

    t_crit = x_crit_start();

    /* go through queues descending in priority */
    /* locate qlist based on msg pri.  */
    GET_LINK_HEAD(pt_msg, pt_msg_q->pt_qhead);
    while (LINK_NOT_EMPTY(pt_msg, NULL))
    {
        if (ui1_pri < pt_msg->ui1_pri)
        {
            break;
        }
        pt_prev = pt_msg;
        GET_LINK_NEXT(pt_msg, pt_msg);
    }

    /* add pt_msgdata between pt_prev and pt_msg */
    SET_LINK_NEXT(pt_msgdata, pt_msg);
    if (LINK_IS_EMPTY(pt_prev, NULL))
    {
        SET_LINK_HEAD(pt_msg_q->pt_qhead, pt_msgdata);
    }
    else
    {
        SET_LINK_NEXT(pt_prev, pt_msgdata);
    }
    pt_msg_q->ui2_qcount++;

    /* resume suspended thread waiting for message */
    if (LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL))
    {
        msg_q_resume_thread(pt_msg_q);
    }

    x_crit_end(t_crit);

    return(OSR_OK);
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
    OS_MSGQ_DATA_T *pt_msgdata, *pt_msg, *pt_prev;
    CHAR *pc_data;
    CRIT_STATE_T t_crit;
    INT32 i4_ret;

    i4_ret = OSR_OK;

    t_crit = x_crit_start();

    /* get a data buffer */
    GET_LINK_HEAD(pt_msgdata, pt_msg_q->pt_bufhead);
    if (LINK_NOT_EMPTY(pt_msgdata, NULL))
    {
        SET_LINK_HEAD(pt_msg_q->pt_bufhead, pt_msgdata->pt_next);
    }
    else
    {
        if (LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL))
        {
            msg_q_resume_thread(pt_msg_q);
        }

        i4_ret = OSR_TOO_MANY;
    }
    x_crit_end(t_crit);

    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    /* store this msg in the msgdata struct */
    pc_data = (CHAR *) (pt_msgdata + 1);
    x_memcpy(pc_data, pv_msg_1, z_size_1);

    if ((pv_msg_2 != NULL) && (z_size_2 != 0))
    {
        x_memcpy(pc_data + z_size_1, pv_msg_2, z_size_2);
    }
    else
    {
        z_size_2 = 0;
    }

    pt_msgdata->z_size = (z_size_1 + z_size_2);
    pt_msgdata->ui1_pri = ui1_pri;

    pt_prev = NULL;

    t_crit = x_crit_start();

    /* go through queues descending in priority */
    /* locate qlist based on msg pri.  */
    GET_LINK_HEAD(pt_msg, pt_msg_q->pt_qhead);
    while (LINK_NOT_EMPTY(pt_msg, NULL))
    {
        if (ui1_pri < pt_msg->ui1_pri)
        {
            break;
        }
        pt_prev = pt_msg;
        GET_LINK_NEXT(pt_msg, pt_msg);
    }

    /* add pt_msgdata between pt_prev and pt_msg */
    SET_LINK_NEXT(pt_msgdata, pt_msg);
    if (LINK_IS_EMPTY(pt_prev, NULL))
    {
        SET_LINK_HEAD(pt_msg_q->pt_qhead, pt_msgdata);
    }
    else
    {
        SET_LINK_NEXT(pt_prev, pt_msgdata);
    }
    pt_msg_q->ui2_qcount++;

    /* resume suspended thread waiting for message */
    if (LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL))
    {
        msg_q_resume_thread(pt_msg_q);
    }

    x_crit_end(t_crit);

    return(OSR_OK);
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
    INT32 i4_ret;

    if ((pv_msg_1 == NULL) || (z_size_1 == 0))
    {
        return(OSR_INV_ARG);
    }

    if (b_msg_q_inited != TRUE)
    {
        return(OSR_NOT_INIT);
    }

    /* check if h_msg_hdl has the right type and get its object */
    i4_ret = os_check_handle(h_msg_hdl, HT_GROUP_OS_MSGQ, (VOID *) &pt_msg_q);
    if (i4_ret != OSR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    if ((z_size_1 + z_size_2) > pt_msg_q->z_maxsize)
    {
        return(OSR_TOO_BIG);
    }


    /* enqueue this message */
    i4_ret = msg_q_put_msg_2_parts(pt_msg_q,
                                   pv_msg_1, z_size_1,
                                   pv_msg_2, z_size_2,
                                   ui1_pri);

    if (i4_ret != OSR_OK)
    {
        return(i4_ret);
    }

    return(OSR_OK);
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
msg_q_get_msg(OS_MSGQ_POLL_T *pt_poll,
              VOID           *pv_msgdata,
              SIZE_T         *pz_size,
              UINT16         *pui2_index,
              MSGQ_OPTION_T  e_option,
              UINT32         ui4_time)
{
    OS_MSGQ_T  *pt_msg_q;       /* msg q object */
    OS_MSGQ_DATA_T *pt_msg;     /* msg data */
    OS_MSGQ_WAIT_T *pt_wchain;
    OS_THREAD_T *pt_thread;    /* thread object */
    CHAR *pc_data;
    INT32 i4_n, i4_i, i4_polling, i4_ret;
    UINT32 ui4_delay, ui4_elapsed = 0;
    CRIT_STATE_T t_crit;

#define MSGQ_TIMEOUT_INTERVAL   50      /* millisecond */

    i4_ret = os_check_handle(pt_poll->h_th_hdl,
                             HT_GROUP_OS_THREAD,
                             (VOID *) &pt_thread);
    if (i4_ret != HR_OK)
    {
        return(OSR_INV_HANDLE);
    }

    i4_polling = 0;     /* polling not yet started */

    do
    {
        pt_msg = NULL;
        pt_msg_q = NULL;
        pt_wchain = (OS_MSGQ_WAIT_T *) &(pt_poll->t_wchain);

        t_crit = x_crit_start();

        for (i4_n = 0, i4_i = 0; i4_n < pt_poll->i1_count; i4_n++, pt_wchain++)
        {
            pt_msg_q = pt_wchain->pt_msg_q;     /* msg q object */
            if (pt_msg_q == NULL)
            {
                continue;
            }

            if ((LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL)) &&
                (pt_msg_q->pt_qwaithead->pt_poll != pt_poll))
            {
                /* support one receive at any given time */
                *pui2_index = i4_n;
                x_crit_end(t_crit);
                return(OSR_BUSY);
            }

            ++i4_i;     /* # of active (not deleted) msg q objects */

            if (pt_msg_q->ui2_qcount <= 0)   /* this msg q has no msgs */
            {
                continue;
            }

            GET_LINK_HEAD(pt_msg, pt_msg_q->pt_qhead);
            if (LINK_NOT_EMPTY(pt_msg, NULL))
            {
                SET_LINK_HEAD(pt_msg_q->pt_qhead, pt_msg->pt_next);
                pt_msg_q->ui2_qcount--;
                break;
            }
        }

        if (pt_msg != NULL)
        {
            /* post polling clean up */
            if (i4_polling)
            {
                msg_q_poll_cleanup(pt_poll);
            }
            x_crit_end(t_crit);

            pc_data = (CHAR *) (pt_msg + 1);
            x_memcpy(pv_msgdata, pc_data, pt_msg->z_size);
            *pz_size = pt_msg->z_size;          /* msg data size */
            *pui2_index = (UINT16) i4_n;        /* returned index */

            t_crit = x_crit_start();
            SET_LINK_NEXT(pt_msg, pt_msg_q->pt_bufhead);
            SET_LINK_HEAD(pt_msg_q->pt_bufhead, pt_msg);
            x_crit_end(t_crit);

            return(OSR_OK);
        }

        /* do not release protection. otherwise, once released, */
        /* a thread which does a msgq_send comes along and eventually does a */
        /* resume thread will finds no thread waiting. after send, if the */
        /* thread does a msgq_receive could be suspended. */
        /* then, the original thread comes back and eventually does a */
        /* sema_lock() and suspend itself */
        /* both threads are then is suspension mode */
        /* x_crit_end(t_crit); */

        /* so, cannot satisfy the receive in this round */
        if ((e_option != X_MSGQ_OPTION_WAIT) || (i4_i == 0))
        {
            /* no active msg q objects, post polling clean up */
            if (i4_polling)
            {
                msg_q_poll_cleanup(pt_poll);
            }

            x_crit_end(t_crit);
            return(OSR_NO_MSG);
        }

        /* block if so specified */
        if ((i4_polling == 0) && (ui4_time == 0))
        {
            i4_polling = 1;
            pt_wchain = (OS_MSGQ_WAIT_T *) &(pt_poll->t_wchain);
            for (i4_n = 0; i4_n < pt_poll->i1_count; i4_n++, pt_wchain++)
            {
                pt_msg_q = (OS_MSGQ_T *) pt_wchain->pt_msg_q;
                if (LINK_NOT_EMPTY(pt_msg_q, NULL))
                {
                    if ((LINK_NOT_EMPTY(pt_msg_q->pt_qwaithead, NULL)) &&
                        (pt_msg_q->pt_qwaithead->pt_poll != pt_poll))
                    {
                        /* support one receive at any given time */
                        /* undo wait */
                        pt_wchain = (OS_MSGQ_WAIT_T *) &(pt_poll->t_wchain);
                        for (i4_i = 0; i4_i < i4_n; i4_i++, pt_wchain++)
                        {
                            pt_msg_q = (OS_MSGQ_T *) pt_wchain->pt_msg_q;
                            if (LINK_NOT_EMPTY(pt_msg_q, NULL))
                            {
                                SET_LINK_HEAD(pt_msg_q->pt_qwaithead, NULL);
                            }
                        }

                        *pui2_index = i4_n;
                        x_crit_end(t_crit);
                        return(OSR_BUSY);
                    }
                    else
                    {
                        SET_LINK_NEXT(pt_wchain, NULL);
                        SET_LINK_HEAD(pt_msg_q->pt_qwaithead, pt_wchain);
                    }
                }
            }
        }
        x_crit_end(t_crit);

        if ((ui4_time == 0) && (ui4_elapsed == 0))   /* wait forever */
        {
            i4_ret = x_sema_lock(pt_thread->h_sema_hdl, X_SEMA_OPTION_WAIT);
        }
        else
        {
            if (ui4_elapsed < ui4_time)
            {
                ui4_delay = ui4_time - ui4_elapsed;

                if (ui4_delay > (UINT32) MSGQ_TIMEOUT_INTERVAL)
                {
                    ui4_delay = (UINT32) MSGQ_TIMEOUT_INTERVAL;
                }

                (VOID) x_sema_lock_timeout(pt_thread->h_sema_hdl, ui4_delay);

                ui4_elapsed += ui4_delay;
                i4_ret = OSR_OK;
            }
            else
            {
                i4_ret = OSR_TIMEOUT;
            }
        }

        if (i4_ret != OSR_OK)
        {
            return(i4_ret);
        }
    } while (1);
}


/*----------------------------------------------------------------------------
 * Function: msg_q_poll_cleanup()
 *
 * Description:
 *      this routine is called to clean waitchain from a previous receive
 *      call, msg_q_get_msg(). it performs
 *      1. go through each waitchain in pt_poll for valid msg Q and delink
 *         the waitchain.
 *
 * Inputs:
 *      pt_poll: pointer to poll struct containg msg Q's to receive message from
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
msg_q_poll_cleanup(OS_MSGQ_POLL_T *pt_poll)
{
    OS_MSGQ_T *pt_msg_q;    /* msg q object */
    OS_MSGQ_WAIT_T *pt_wchain;
    OS_MSGQ_WAIT_T *pt_qwait, *pt_prev;
    INT32 i4_n;

    /* get waitchain of this polling */
    pt_wchain = (OS_MSGQ_WAIT_T *) &(pt_poll->t_wchain);
    i4_n = pt_poll->i1_count;
    while (i4_n-- > 0)
    {
        pt_msg_q = pt_wchain->pt_msg_q;     /* msg q object */
        if (LINK_NOT_EMPTY(pt_msg_q, NULL))
        {
            /* this msg q obj is active */
            pt_prev = NULL;
            GET_LINK_HEAD(pt_qwait, pt_msg_q->pt_qwaithead); /* waichain head */
            while (LINK_NOT_EMPTY(pt_qwait, NULL))
            {
                if (pt_wchain == pt_qwait)
                {
                    /* found and remove it from waitchain */
                    if (LINK_IS_EMPTY(pt_prev, NULL))   /* remove head */
                    {
                        SET_LINK_HEAD(pt_msg_q->pt_qwaithead, pt_qwait->pt_next);
                    }
                    else
                    {
                        SET_LINK_NEXT(pt_prev, pt_qwait->pt_next);
                    }
                }
                pt_prev = pt_qwait;
                GET_LINK_NEXT(pt_qwait, pt_qwait);
            }
        }
        ++pt_wchain;
    }
}


#if 0
/* obsoleted */
VOID
msg_q_delete(OS_MSGQ_T *pt_msg_q)
{
    INT32 i4_ret;

    i4_ret = x_sema_lock(h_msg_q_sema_hdl, X_SEMA_OPTION_WAIT);
    if (i4_ret != OSR_OK)
    {
        return;
    }

    pt_msg_q->i1_refcount--;

    if (pt_msg_q->i1_refcount <= 0)
    {
        msg_q_remove_msgq_obj(pt_msg_q);
        x_mem_free(pt_msg_q);
    }

    (VOID) x_sema_unlock(h_msg_q_sema_hdl);
}


VOID
msg_q_free_msg(OS_MSGQ_T *pt_msg_q)
{
#if 0
    OS_MSGQ_LIST_T *pt_qlist, *pt_qtmp;
    OS_MSGQ_DATA_T *pt_msg, *pt_msg_tmp;

    /* when the last msg q delete is called */
    /* pt_msg_q will be freed anyway. */
    /* so, this routine may not get called. */
    /* but, in case there is a need, we have it. */
    /* remove pri. queue chain */
    GET_LINK_HEAD(pt_qlist, pt_msg_q->pt_qhead);    /* queue chain */
    SET_LINK_HEAD(pt_msg_q->pt_qhead, NULL);
    while (pt_qlist != NULL)
    {
        /* remove all msgs associated with this pri. queue */
        GET_LINK_HEAD(pt_msg, pt_qlist->pt_head);
        SET_LINK_HEAD(pt_qlist->pt_head, NULL);
        SET_LINK_TAIL(pt_qlist->pt_tail, NULL);

        while (LINK_NOT_EMPTY(pt_msg, NULL))
        {
            pt_msg_tmp = pt_msg;
            GET_LINK_NEXT(pt_msg, pt_msg->pt_next);

            /* free this OS_MSGQ_LIST_T back to pool */
            SET_LINK_NEXT(pt_msg_tmp, pt_msg_q->pt_bufhead);
            SET_LINK_HEAD(pt_msg_q->pt_bufhead, pt_msg_tmp);
        }
        pt_qlist->ui2_count = 0;
        pt_qtmp = pt_qlist;
        GET_LINK_NEXT(pt_qlist, pt_qlist->pt_next);

        /* free this OS_MSGQ_LIST_T back to pool */
        SET_LINK_NEXT(pt_qtmp, pt_msg_q->pt_qbufhead);
        SET_LINK_HEAD(pt_msg_q->pt_qbufhead, pt_qtmp);
    }
#endif
}
#endif


/*----------------------------------------------------------------------------
 * Function: msg_q_resume_thread()
 *
 * Description:
 *      this routine is called to resume a thread waiting on a msg Q.
 *      it performs
 *      1. check if there is a waitchain entry in the msg Q and call
 *         thread resume passing thread handle in the waitchain.
 *
 * Inputs:
 *      pt_msg_q: pointer to a msg Q struct.
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID
msg_q_resume_thread(OS_MSGQ_T *pt_msg_q)
{
    OS_MSGQ_WAIT_T *pt_qwait;
    OS_THREAD_T *pt_thread;    /* thread object */
    INT32 i4_ret;

    GET_LINK_HEAD(pt_qwait, pt_msg_q->pt_qwaithead);

    if ((LINK_NOT_EMPTY(pt_qwait, NULL)) &&
        (LINK_NOT_EMPTY(pt_qwait->pt_msg_q, NULL)))
    {
        i4_ret = os_check_handle(pt_qwait->pt_poll->h_th_hdl,
                                 HT_GROUP_OS_THREAD,
                                 (VOID *) &pt_thread);
        if (i4_ret == HR_OK)
        {
            (VOID) x_sema_unlock(pt_thread->h_sema_hdl);
        }
    }
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
