/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *^M
 *                                                                            *^M
 *   This software/firmware and related documentation ("MediaTek Software")   *^M
 * are protected under international and related jurisdictions'copyright laws *^M
 * as unpublished works. The information contained herein is confidential and *^M
 * proprietary to MediaTek Inc. Without the prior written permission of       *^M
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *^M
 * MediaTek Software, and information contained herein, in whole or in part,  *^M
 * shall be strictly prohibited.                                              *^M
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *^M
 *                                                                            *^M
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *^M
 * AGREES TO THE FOLLOWING:                                                   *^M
 *                                                                            *^M
 *   1)Any and all intellectual property rights (including without            *^M
 * limitation, patent, copyright, and trade secrets) in and to this           *^M
 * Software/firmware and related documentation ("MediaTek Software") shall    *^M
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *^M
 * property rights (including without limitation, patent, copyright, and      *^M
 * trade secrets) in and to any modifications and derivatives to MediaTek     *^M
 * Software, whoever made, shall also remain the exclusive property of        *^M
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *^M
 * title to any intellectual property right in MediaTek Software to Receiver. *^M
 *                                                                            *^M
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *^M
 * representatives is provided to Receiver on an "AS IS" basis only.          *^M
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *^M
 * including but not limited to any implied warranties of merchantability,    *^M
 * non-infringement and fitness for a particular purpose and any warranties   *^M
 * arising out of course of performance, course of dealing or usage of trade. *^M
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *^M
 * software of any third party which may be used by, incorporated in, or      *^M
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *^M
 * such third parties for any warranty claim relating thereto.  Receiver      *^M
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *^M
 * from any third party all proper licenses contained in or delivered with    *^M
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *^M
 * releases made to Receiver's specifications or to conform to a particular   *^M
 * standard or open forum.                                                    *^M
 *                                                                            *^M
 *   3)Receiver further acknowledge that Receiver may, either presently       *^M
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *^M
 * development and the implementation, in accordance with Receiver's designs, *^M
 * of certain softwares relating to Receiver's product(s) (the "Services").   *^M
 * Except as may be otherwise agreed to in writing, no warranties of any      *^M
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *^M
 * to the Services provided, and the Services are provided on an "AS IS"      *^M
 * basis. Receiver further acknowledges that the Services may contain errors  *^M
 * that testing is important and it is solely responsible for fully testing   *^M
 * the Services and/or derivatives thereof before they are used, sublicensed  *^M
 * or distributed. Should there be any third party action brought against     *^M
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *^M
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *^M
 * mutually agree to enter into or continue a business relationship or other  *^M
 * arrangement, the terms and conditions set forth herein shall remain        *^M
 * effective and, unless explicitly stated otherwise, shall prevail in the    *^M
 * event of a conflict in the terms in any agreements entered into between    *^M
 * the parties.                                                               *^M
 *                                                                            *^M
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *^M
 * cumulative liability with respect to MediaTek Software released hereunder  *^M
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *^M
 * MediaTek Software at issue.                                                *^M
 *                                                                            *^M
 *   5)The transaction contemplated hereunder shall be construed in           *^M
 * accordance with the laws of Singapore, excluding its conflict of laws      *^M
 * principles.  Any disputes, controversies or claims arising thereof and     *^M
 * related thereto shall be settled via arbitration in Singapore, under the   *^M
 * then current rules of the International Chamber of Commerce (ICC).  The    *^M
 * arbitration shall be conducted in English. The awards of the arbitration   *^M
 * shall be final and binding upon both parties and shall be entered and      *^M
 * enforceable in any court of competent jurisdiction.                        *^M
 *---------------------------------------------------------------------------*/


#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/wait.h>
#include "x_assert.h"
#include "x_os.h"


#define EV_GRP_NAME_LEN 16

typedef struct os_ev_grp_light
{
    struct os_ev_grp_light *previous;
    struct os_ev_grp_light *next;
    wait_queue_head_t wq;
    EV_GRP_EVENT_T e_events;
    INT16 i2_refcount;
    CHAR s_name[EV_GRP_NAME_LEN + 1];
} OS_EV_GRP_LIGHT_T;


static OS_EV_GRP_LIGHT_T *s_ev_grp_list;

//static DEFINE_SPINLOCK(x_ev_grp_lock);
static DEFINE_RWLOCK(x_ev_grp_lock);

static void ev_grp_list_add(OS_EV_GRP_LIGHT_T *pt_ev_grp)
{
    if (s_ev_grp_list != NULL)
    {
        pt_ev_grp->previous = s_ev_grp_list->previous;
        pt_ev_grp->next = s_ev_grp_list;
        s_ev_grp_list->previous->next = pt_ev_grp;
        s_ev_grp_list->previous = pt_ev_grp;
    }
    else
    {
        s_ev_grp_list = pt_ev_grp->next = pt_ev_grp->previous = pt_ev_grp;
    }
}


static void ev_grp_list_remove(OS_EV_GRP_LIGHT_T *pt_ev_grp)
{
    if (pt_ev_grp->previous == pt_ev_grp)
    {
        s_ev_grp_list = NULL;
    }
    else
    {
        pt_ev_grp->previous->next = pt_ev_grp->next;
        pt_ev_grp->next->previous = pt_ev_grp->previous;
        if (s_ev_grp_list == pt_ev_grp)
        {
            s_ev_grp_list = pt_ev_grp->next;
        }
    }
}


static OS_EV_GRP_LIGHT_T *ev_grp_find_obj(const CHAR *ps_name)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp = s_ev_grp_list;
    if (pt_ev_grp == NULL)
    {
        return NULL;
    }
    do
    {
        if (strncmp(pt_ev_grp->s_name, ps_name, EV_GRP_NAME_LEN) == 0)
        {
            return pt_ev_grp;
        }
        pt_ev_grp = pt_ev_grp->next;
    } while (pt_ev_grp != s_ev_grp_list);

    return NULL;
}


INT32
x_ev_group_create(HANDLE_T           *ph_hdl,
                  const CHAR         *ps_name,
                  EV_GRP_EVENT_T     e_init_events)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    unsigned long flags;

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_hdl == NULL))
    {
        return OSR_INV_ARG;
    }

    pt_ev_grp = kcalloc(1, sizeof(OS_EV_GRP_LIGHT_T), GFP_KERNEL);
    if (pt_ev_grp == NULL)
    {
        return OSR_NO_RESOURCE;
    }
    //FILL_CALLER(pt_ev_grp);

    pt_ev_grp->e_events = e_init_events;
    pt_ev_grp->i2_refcount = 1;
    strncpy(pt_ev_grp->s_name, ps_name, EV_GRP_NAME_LEN);
    pt_ev_grp->s_name[EV_GRP_NAME_LEN] = 0;

    init_waitqueue_head(&pt_ev_grp->wq);

    //local_irq_save(flags);
    write_lock_irqsave(&x_ev_grp_lock, flags);
    if (ev_grp_find_obj(ps_name) != NULL)
    {
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_ev_grp_lock, flags);
        kfree(pt_ev_grp);
        return OSR_EXIST;
    }

    ev_grp_list_add(pt_ev_grp);
    *ph_hdl = (HANDLE_T)(pt_ev_grp);

    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_ev_grp_lock, flags);
    
    return OSR_OK;
}


INT32
x_ev_group_attach(HANDLE_T     *ph_hdl,
                  const CHAR   *ps_name)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    unsigned long flags;

    /* arguments check */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_hdl == NULL))
    {
        return OSR_INV_ARG;
    }

    //local_irq_save(flags);
    write_lock_irqsave(&x_ev_grp_lock, flags);
    pt_ev_grp = ev_grp_find_obj(ps_name);
    if (pt_ev_grp == NULL)
    {
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_ev_grp_lock, flags);
        return OSR_NOT_EXIST;
    }
    
    pt_ev_grp->i2_refcount++;
    *ph_hdl = (HANDLE_T)(pt_ev_grp);
    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_ev_grp_lock, flags);

    return OSR_OK;
}


INT32
x_ev_group_delete(HANDLE_T    h_hdl)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    INT16 i2_refcount;
    unsigned long flags;

    pt_ev_grp = (OS_EV_GRP_LIGHT_T *)(h_hdl);

    //local_irq_save(flags);
    write_lock_irqsave(&x_ev_grp_lock, flags);
    i2_refcount = --pt_ev_grp->i2_refcount;
    if (i2_refcount > 0)
    {
        //local_irq_restore(flags);
        write_unlock_irqrestore(&x_ev_grp_lock, flags);
        return OSR_OK;
    }

    ev_grp_list_remove(pt_ev_grp);
    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_ev_grp_lock, flags);

    kfree(pt_ev_grp);
    return OSR_OK;
}


INT32
x_ev_group_set_event(HANDLE_T           h_hdl,
                     EV_GRP_EVENT_T     e_events,
                     EV_GRP_OPERATION_T e_op)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    unsigned long flags;

    if (e_op == X_EV_OP_CLEAR)
    {
        e_events = (EV_GRP_EVENT_T)(0);
        e_op = X_EV_OP_AND;
    }
    else if (e_op != X_EV_OP_AND && e_op != X_EV_OP_OR)
    {
        return OSR_INV_ARG;
    }

    pt_ev_grp = (OS_EV_GRP_LIGHT_T *)(h_hdl);

    //local_irq_save(flags);
    write_lock_irqsave(&x_ev_grp_lock, flags);
    if (e_op == X_EV_OP_AND)
    {
        pt_ev_grp->e_events &= e_events;
    }
    else
    {
        pt_ev_grp->e_events |= e_events;
    }
    wake_up_all(&pt_ev_grp->wq);
    //local_irq_restore(flags);
    write_unlock_irqrestore(&x_ev_grp_lock, flags);

    return OSR_OK;
}


static INT32
x_ev_group_wait_event_impl(HANDLE_T              h_hdl,
                           EV_GRP_EVENT_T        e_events_req,
                           EV_GRP_EVENT_T        *pe_events_got,
                           EV_GRP_OPERATION_T    e_op,
                           BOOL                  wait)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    EV_GRP_EVENT_T e_events;
    unsigned long flags;

    if (pe_events_got == NULL ||
        (e_op != X_EV_OP_AND && e_op != X_EV_OP_OR && e_op != X_EV_OP_AND_CONSUME && e_op != X_EV_OP_OR_CONSUME))
    {
        return OSR_INV_ARG;
    }

    pt_ev_grp = (OS_EV_GRP_LIGHT_T *)(h_hdl);
    *pe_events_got = 0;

    if (wait)
    {
        while (TRUE)
        {
            if (e_op == X_EV_OP_AND || e_op == X_EV_OP_AND_CONSUME)
            {
                wait_event(pt_ev_grp->wq, (pt_ev_grp->e_events & e_events_req) == e_events_req);
            }
            else
            {
                wait_event(pt_ev_grp->wq, (pt_ev_grp->e_events & e_events_req) != (EV_GRP_EVENT_T)(0));
            }
            if(e_op == X_EV_OP_AND ||e_op == X_EV_OP_OR)
            {
                read_lock_irqsave(&x_ev_grp_lock, flags);
                e_events = pt_ev_grp->e_events & e_events_req;
                if((e_op == X_EV_OP_AND && e_events == e_events_req) 
                    ||(e_op == X_EV_OP_OR && e_events != (EV_GRP_EVENT_T)(0)))
                {
                    read_unlock_irqrestore(&x_ev_grp_lock, flags);
                    break;
                }
                read_unlock_irqrestore(&x_ev_grp_lock, flags);
            }
            else
            {
                write_lock_irqsave(&x_ev_grp_lock, flags);
                e_events = pt_ev_grp->e_events & e_events_req;
                if((e_op == X_EV_OP_AND_CONSUME && e_events == e_events_req)
                    ||(e_op == X_EV_OP_OR_CONSUME && e_events != (EV_GRP_EVENT_T)(0)))
                {
                    pt_ev_grp->e_events &= ~e_events_req;
                    write_unlock_irqrestore(&x_ev_grp_lock, flags);
                    break;
                }
                write_unlock_irqrestore(&x_ev_grp_lock, flags);
            }
        }
    }
    else
    {
        if(e_op == X_EV_OP_AND ||e_op == X_EV_OP_OR)
        {
            read_lock_irqsave(&x_ev_grp_lock, flags);
            e_events = pt_ev_grp->e_events & e_events_req;
            if((e_op == X_EV_OP_AND && e_events != e_events_req)
                || (e_op == X_EV_OP_OR && e_events ==  (EV_GRP_EVENT_T)(0)))
            {
                read_unlock_irqrestore(&x_ev_grp_lock, flags);
                return OSR_TIMEOUT;
            }
            read_unlock_irqrestore(&x_ev_grp_lock, flags);
        }
        else
        {
            write_lock_irqsave(&x_ev_grp_lock, flags);
            e_events = pt_ev_grp->e_events & e_events_req;
            if((e_op == X_EV_OP_AND_CONSUME && e_events != e_events_req)
                    ||(e_op == X_EV_OP_OR_CONSUME && e_events == (EV_GRP_EVENT_T)(0)))
            {
                write_unlock_irqrestore(&x_ev_grp_lock, flags);
                return OSR_TIMEOUT;
            }
            pt_ev_grp->e_events &= ~e_events_req;
            write_unlock_irqrestore(&x_ev_grp_lock, flags);
        }
    }

    *pe_events_got = e_events;
    return OSR_OK;
}


INT32
x_ev_group_wait_event(HANDLE_T              h_hdl,
                      EV_GRP_EVENT_T        e_events_req,
                      EV_GRP_EVENT_T        *pe_events_got,
                      EV_GRP_OPERATION_T    e_op)
{
    return x_ev_group_wait_event_impl(h_hdl, e_events_req, pe_events_got, e_op, TRUE);
}



INT32
x_ev_group_wait_event_timeout(HANDLE_T              h_hdl,
                              EV_GRP_EVENT_T        e_events_req,
                              EV_GRP_EVENT_T        *pe_events_got,
                              EV_GRP_OPERATION_T    e_op,
                              UINT32                ui4_time)
{
    static const int quantum_ms = 1000 / HZ;
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    EV_GRP_EVENT_T e_events;
    unsigned long flags;
    int ret;

    if (ui4_time == 0)
    {
        return x_ev_group_wait_event_impl(h_hdl, e_events_req, pe_events_got, e_op, FALSE);
    }
    if (ui4_time == 0xFFFFFFFFU)
    {
        return x_ev_group_wait_event_impl(h_hdl, e_events_req, pe_events_got, e_op, TRUE);
    }
    if (pe_events_got == NULL ||
        (e_op != X_EV_OP_AND && e_op != X_EV_OP_OR && e_op != X_EV_OP_AND_CONSUME && e_op != X_EV_OP_OR_CONSUME))
    {
        return OSR_INV_ARG;
    }


    pt_ev_grp = (OS_EV_GRP_LIGHT_T *)(h_hdl);
    *pe_events_got = 0;

    while (TRUE)
    {
        if (e_op == X_EV_OP_AND || e_op == X_EV_OP_AND_CONSUME)
        {
            ret = wait_event_timeout(pt_ev_grp->wq, (pt_ev_grp->e_events & e_events_req) == e_events_req, ui4_time / quantum_ms);
        }
        else
        {
            ret = wait_event_timeout(pt_ev_grp->wq, (pt_ev_grp->e_events & e_events_req) != (EV_GRP_EVENT_T)(0), ui4_time / quantum_ms);
        }
        if (ret == 0)
        {
            return OSR_TIMEOUT;
        }
        if(e_op == X_EV_OP_AND ||e_op == X_EV_OP_OR)
        {
            read_lock_irqsave(&x_ev_grp_lock, flags);
            e_events = pt_ev_grp->e_events & e_events_req;
            if((e_op == X_EV_OP_AND && e_events == e_events_req) 
                    ||(e_op == X_EV_OP_OR && e_events != (EV_GRP_EVENT_T)(0)))
            {
                read_unlock_irqrestore(&x_ev_grp_lock, flags);
                break;
            }
            read_unlock_irqrestore(&x_ev_grp_lock, flags);
        }
        else
        {
            write_lock_irqsave(&x_ev_grp_lock, flags);
            e_events = pt_ev_grp->e_events & e_events_req;
            if((e_op == X_EV_OP_AND_CONSUME && e_events == e_events_req) 
                    ||(e_op == X_EV_OP_OR_CONSUME && e_events != (EV_GRP_EVENT_T)(0)))
            {
                pt_ev_grp->e_events &= ~e_events_req;
                write_unlock_irqrestore(&x_ev_grp_lock, flags);
                break;
            }
            write_unlock_irqrestore(&x_ev_grp_lock, flags);
        }
    }

    *pe_events_got = e_events;
    return OSR_OK;
}




INT32
x_ev_group_get_info(HANDLE_T          h_hdl,
                    EV_GRP_EVENT_T    *pe_cur_events,
                    UINT8             *pui1_num_thread_waiting,
                    CHAR              *ps_ev_group_name,
                    CHAR              *ps_first_wait_thread)
{
    OS_EV_GRP_LIGHT_T *pt_ev_grp;
    unsigned long flags;

    if ((pe_cur_events == NULL) ||
        (pui1_num_thread_waiting == NULL) ||
        (ps_ev_group_name == NULL) ||
        (ps_first_wait_thread == NULL))
    {
        return OSR_INV_ARG;
    }

    pt_ev_grp = (OS_EV_GRP_LIGHT_T *)(h_hdl);
    *pe_cur_events = 0;

    //local_irq_save(flags);
    read_lock_irqsave(&x_ev_grp_lock, flags);
    *pe_cur_events = pt_ev_grp->e_events;
    //local_irq_restore(flags);
    read_unlock_irqrestore(&x_ev_grp_lock, flags);

    // TODO: fill pui1_num_thread_waiting, ps_ev_group_name, ps_first_wait_thread

    // return OSR_OK;
    return OSR_INV_ARG;
}



INT32
ev_grp_init(VOID)
{
    return OSR_OK;
}


