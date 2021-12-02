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
#include <linux/vmalloc.h>
#include "x_assert.h"
#include "x_os.h"


#define MSGQ_NAME_LEN           16
#define MSGQ_MAX_DATA_SIZE      4095
#define MSGQ_MAX_MSGS           4095
#define MEM_ALLOC_SIZE_CHECK      (4 * PAGE_SIZE)

/* Support two priorities 
 * FIXME: assume most case uses only 1 priority, it consumes 2x memory, but saves list traversal time
 */
typedef struct os_msg_q_light
{
    struct os_msg_q_light *previous;
    struct os_msg_q_light *next;
    wait_queue_head_t read_wq;
    wait_queue_head_t write_wq;
    SIZE_T message_size;
    SIZE_T z_maxsize;
    UINT16 ui2_msg_count;
    INT16 i2_refcount;
    CHAR s_name[MSGQ_NAME_LEN + 1];
    UINT16 prio[2];
    BYTE *read[2];
    BYTE *write[2];
    BYTE *start[2];
    BYTE *end[2];
} OS_MSGQ_LIGHT_T;


static OS_MSGQ_LIGHT_T *s_msg_q_list;


static DEFINE_SPINLOCK(x_msg_lock);

static void msg_q_list_add(OS_MSGQ_LIGHT_T *pt_msg_q)
{
    if (s_msg_q_list != NULL)
    {
        pt_msg_q->previous = s_msg_q_list->previous;
        pt_msg_q->next = s_msg_q_list;
        s_msg_q_list->previous->next = pt_msg_q;
        s_msg_q_list->previous = pt_msg_q;
    }
    else
    {
        s_msg_q_list = pt_msg_q->next = pt_msg_q->previous = pt_msg_q;
    }
}


static void msg_q_list_remove(OS_MSGQ_LIGHT_T *pt_msg_q)
{
    if (pt_msg_q->previous == pt_msg_q)
    {
        s_msg_q_list = NULL;
    }
    else
    {
        pt_msg_q->previous->next = pt_msg_q->next;
        pt_msg_q->next->previous = pt_msg_q->previous;
        if (s_msg_q_list == pt_msg_q)
        {
            s_msg_q_list = pt_msg_q->next;
        }
    }
}


static OS_MSGQ_LIGHT_T *msg_q_find_obj(const CHAR *ps_name)
{
    OS_MSGQ_LIGHT_T *pt_msg_q = s_msg_q_list;
    if (pt_msg_q == NULL)
    {
        return NULL;
    }
    do
    {
        if (strncmp(pt_msg_q->s_name, ps_name, MSGQ_NAME_LEN) == 0)
        {
            return pt_msg_q;
        }
        pt_msg_q = pt_msg_q->next;
    } while (pt_msg_q != s_msg_q_list);

    return NULL;
}


INT32
x_msg_q_create(HANDLE_T     *ph_msg_hdl,
               const CHAR   *ps_name,
               SIZE_T       z_msg_size,
               UINT16       ui2_msg_count)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    BYTE *pt_data;
    SIZE_T message_size;
    unsigned long flags;
    unsigned long alloc_size;

    /* check arguments */
    if ((ps_name == NULL) || (ps_name[0] == '\0') || (ph_msg_hdl == NULL) ||
        (z_msg_size > MSGQ_MAX_DATA_SIZE) || (ui2_msg_count > MSGQ_MAX_MSGS) ||
        (z_msg_size == 0) || (ui2_msg_count == 0))
    {
        return OSR_INV_ARG;
    }

    message_size = sizeof(SIZE_T) + ((z_msg_size + 3) & ~3);
    pt_msg_q = kcalloc(1, sizeof(OS_MSGQ_LIGHT_T), GFP_KERNEL);
    alloc_size = message_size * (ui2_msg_count + 1);
    if (alloc_size > MEM_ALLOC_SIZE_CHECK)
    {
        pt_data = vmalloc(alloc_size);
    }
    else
    {
        pt_data = kcalloc(1, alloc_size, GFP_KERNEL);
    }
    if (pt_msg_q == NULL)
    {
        return OSR_NO_RESOURCE;
    }
    //FILL_CALLER(pt_msg_q);

    pt_msg_q->prio[0] = 0xFFFF;
    pt_msg_q->read[0] = pt_data;
    pt_msg_q->write[0] = pt_data;
    pt_msg_q->start[0] = pt_data;
    pt_msg_q->end[0] = pt_data + alloc_size;
    pt_msg_q->prio[1] = 0xFFFF;
    pt_msg_q->read[1] = NULL;
    pt_msg_q->write[1] = NULL;
    pt_msg_q->start[1] = NULL;
    pt_msg_q->end[1] = NULL;
    pt_msg_q->message_size = message_size;
    pt_msg_q->z_maxsize = z_msg_size;
    pt_msg_q->ui2_msg_count = ui2_msg_count;
    pt_msg_q->i2_refcount = 1;
    strncpy(pt_msg_q->s_name, ps_name, MSGQ_NAME_LEN);

    init_waitqueue_head(&pt_msg_q->read_wq);
    init_waitqueue_head(&pt_msg_q->write_wq);

    //local_irq_save(flags);
    spin_lock_irqsave(&x_msg_lock, flags);
    if (msg_q_find_obj(ps_name) != NULL)
    {
        //local_irq_restore(flags);
        spin_unlock_irqrestore(&x_msg_lock, flags);
        kfree(pt_msg_q);
        if (alloc_size > MEM_ALLOC_SIZE_CHECK)
        {
            vfree(pt_data);
        }
        else
        {
            kfree(pt_data);
        }
        return OSR_EXIST;
    }
    msg_q_list_add(pt_msg_q);
    //local_irq_restore(flags);
    spin_unlock_irqrestore(&x_msg_lock, flags);

    *ph_msg_hdl = (HANDLE_T)(pt_msg_q);
    return OSR_OK;
}


INT32
x_msg_q_attach(HANDLE_T     *ph_msg_hdl,
               const CHAR   *ps_name)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    unsigned long flags;

    /* arguments check */
    if ((ps_name == NULL) || (ps_name[0] == '\0') ||
        (ph_msg_hdl == NULL_HANDLE))
    {
        return OSR_INV_ARG;
    }

    //local_irq_save(flags);
    spin_lock_irqsave(&x_msg_lock, flags);
    pt_msg_q = msg_q_find_obj(ps_name);
    if (pt_msg_q == NULL)
    {
        //local_irq_restore(flags);
        spin_unlock_irqrestore(&x_msg_lock, flags);
        return OSR_NOT_EXIST;
    }

    pt_msg_q->i2_refcount++;
    //local_irq_restore(flags);
    spin_unlock_irqrestore(&x_msg_lock, flags);

    *ph_msg_hdl = (HANDLE_T)(pt_msg_q);

    return OSR_OK;
}


INT32
x_msg_q_delete(HANDLE_T h_msg_hdl)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    INT16 i2_refcount;
    unsigned long flags;
    unsigned long alloc_size;

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(h_msg_hdl);

    //local_irq_save(flags);
    spin_lock_irqsave(&x_msg_lock, flags);
    i2_refcount = --pt_msg_q->i2_refcount;
    if (i2_refcount > 0)
    {
        //local_irq_restore(flags);
        spin_unlock_irqrestore(&x_msg_lock, flags);
        return OSR_OK;
    }
    msg_q_list_remove(pt_msg_q);
    //local_irq_restore(flags);
    spin_unlock_irqrestore(&x_msg_lock, flags);

    alloc_size = pt_msg_q->message_size * (pt_msg_q->ui2_msg_count + 1);
    if (pt_msg_q->start[0])
    {
        if (alloc_size > MEM_ALLOC_SIZE_CHECK)
        {
            vfree(pt_msg_q->start[0]);
        }
        else
        {
            kfree(pt_msg_q->start[0]);
        }
    }

    if (pt_msg_q->start[1])
    {
        if (alloc_size > MEM_ALLOC_SIZE_CHECK)
        {
            vfree(pt_msg_q->start[1]);
        }
        else
        {
            kfree(pt_msg_q->start[1]);
        }
    }

    kfree(pt_msg_q);
    return OSR_OK;
}


INT32
x_msg_q_send(HANDLE_T   h_msg_hdl,
             const VOID *pv_msg,
             SIZE_T     z_size,
             UINT8      ui1_pri)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    BYTE *write;
    unsigned long flags;
    unsigned long i=0xFFFFFFFF;

    if ((pv_msg == NULL) || (z_size == 0))
    {
        return OSR_INV_ARG;
    }

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(h_msg_hdl);

    if (z_size > pt_msg_q->z_maxsize)
    {
        return OSR_TOO_BIG;
    }

    if (pt_msg_q->prio[0] == (UINT16) ui1_pri)
    {
        i = 0;
    } else if (pt_msg_q->prio[1] == (UINT16) ui1_pri)
    {
        i = 1;
    } else {
        if (pt_msg_q->prio[0] == 0xFFFF)
        {
            pt_msg_q->prio[0] = (UINT16) ui1_pri;
            i = 0;
        } 

	if (i == 0xFFFFFFFF)
	{
            if (pt_msg_q->prio[1] == 0xFFFF)
	    {
                BYTE *pt_data;
                unsigned long alloc_size;

		/* first time, create data memory */
                alloc_size = pt_msg_q->message_size * (pt_msg_q->ui2_msg_count + 1);
                if (alloc_size > MEM_ALLOC_SIZE_CHECK)
                {
                    pt_data = vmalloc(alloc_size);
                }
                else
                {
                    pt_data = kcalloc(1, alloc_size, GFP_KERNEL|__GFP_REPEAT);
                }
		if (pt_data == NULL)
                {
                    return OSR_NO_RESOURCE;
                }
                pt_msg_q->prio[1] = (UINT16) ui1_pri;
		pt_msg_q->read[1] = pt_data;
		pt_msg_q->write[1] = pt_data;
		pt_msg_q->start[1] = pt_data;
		pt_msg_q->end[1] = pt_data + alloc_size;
	    }
	    i = 1;
	}
    }

    if (i == 0xFFFFFFFF)
    {
            panic("msg_q only supports two priorities. already in use: %d, %d, now pass %d\n", pt_msg_q->prio[0], pt_msg_q->prio[1], ui1_pri);
    }
    //local_irq_save(flags);
    spin_lock_irqsave(&x_msg_lock, flags);
    write = pt_msg_q->write[i] + pt_msg_q->message_size;
    if (write == pt_msg_q->end[i])
    {
        write = pt_msg_q->start[i];
    }
    if (write == pt_msg_q->read[i])
    {
        //local_irq_restore(flags);
        spin_unlock_irqrestore(&x_msg_lock, flags);
        return OSR_TOO_MANY;
    }
    *(SIZE_T *)(pt_msg_q->write[i]) = z_size;
    memcpy(pt_msg_q->write[i] + sizeof(SIZE_T), pv_msg, z_size);
    pt_msg_q->write[i] = write;
    wake_up_all(&pt_msg_q->write_wq);
    //local_irq_restore(flags);
    spin_unlock_irqrestore(&x_msg_lock, flags);

    return OSR_OK;
}


INT32
x_msg_q_receive(UINT16          *pui2_index,
                VOID            *pv_msg,
                SIZE_T          *pz_size,
                HANDLE_T        *ph_msgq_hdl_list,
                UINT16          ui2_msgq_hdl_count,
                MSGQ_OPTION_T   e_option)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    BYTE *read;
    SIZE_T z_size;
    unsigned long i=0;

    /* check arguments */
    if (e_option != X_MSGQ_OPTION_WAIT && e_option != X_MSGQ_OPTION_NOWAIT)
    {
        return OSR_INV_ARG;
    }

    if ((pui2_index == NULL) || (pv_msg == NULL) ||
        (pz_size == NULL) || (*pz_size == 0) ||
        (ph_msgq_hdl_list == NULL) || (ui2_msgq_hdl_count == 0))
    {
        return OSR_INV_ARG;
    }

    if (ui2_msgq_hdl_count != 1)
    {
        panic("x_msg_q_receive() not supports multiple handle count!\n");
        return OSR_NOT_SUPPORT;
    }

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(ph_msgq_hdl_list[0]);
    if ((pt_msg_q->read[0] == pt_msg_q->write[0]) &&
	(pt_msg_q->read[1] == pt_msg_q->write[1]))
    {
        if (e_option == X_MSGQ_OPTION_NOWAIT)
        {
            return OSR_NO_MSG;
        }
        wait_event(pt_msg_q->write_wq, ((pt_msg_q->read[0] != pt_msg_q->write[0]) || (pt_msg_q->read[1] != pt_msg_q->write[1])));
    }

    if (pt_msg_q->prio[0] < pt_msg_q->prio[1])
    {
        i = 0;
    }
    else
    {
        i = 1;
    }

    /* check high priority has data or not */
    if (pt_msg_q->read[i] == pt_msg_q->write[i])
    {
        i = 1 - i;
    }

    /* if there is no data on the other queue, return */
    if (pt_msg_q->read[i] == pt_msg_q->write[i])
    {
        panic("x_msg_q_receive(): not support multiple threads call msg_q_receive.\n");
        return OSR_NO_MSG;
    }

    read = pt_msg_q->read[i];
    z_size = *(SIZE_T *)(read);
    if (z_size > *pz_size)
    {
        z_size = *pz_size;
    }
    memcpy(pv_msg, read + sizeof(SIZE_T), z_size);
    read += pt_msg_q->message_size;
    if (read == pt_msg_q->end[i])
    {
        read = pt_msg_q->start[i];
    }
    pt_msg_q->read[i] = read;
    wake_up_all(&pt_msg_q->read_wq);

    *pui2_index = 0;
    *pz_size = z_size;

    return OSR_OK;
}


INT32
x_msg_q_receive_timeout(UINT16          *pui2_index,
                        VOID            *pv_msg,
                        SIZE_T          *pz_size,
                        HANDLE_T        *ph_msgq_hdl_list,
                        UINT16          ui2_msgq_hdl_count,
                        UINT32          ui4_time)
{
    static const int quantum_ms = 1000 / HZ;
    OS_MSGQ_LIGHT_T *pt_msg_q;
    BYTE *read;
    SIZE_T z_size;
    int ret;
    unsigned long i;

    if ((pui2_index == NULL) || (pv_msg == NULL) ||
        (pz_size == NULL) || (*pz_size == 0) ||
        (ph_msgq_hdl_list == NULL) || (ui2_msgq_hdl_count == 0))
    {
        return OSR_INV_ARG;
    }

    if (ui2_msgq_hdl_count != 1)
    {
        panic("x_msg_q_receive_timeout() not supports multiple handle count!\n");
        return OSR_NOT_SUPPORT;
    }

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(ph_msgq_hdl_list[0]);
    if ((pt_msg_q->read[0] == pt_msg_q->write[0]) &&
        (pt_msg_q->read[1] == pt_msg_q->write[1]))
    {
        ret = wait_event_timeout(pt_msg_q->write_wq, ((pt_msg_q->read[0] != pt_msg_q->write[0]) || (pt_msg_q->read[1] != pt_msg_q->write[1])), ui4_time / quantum_ms);
        if (ret == 0)
        {
            return OSR_TIMEOUT;
        }
    }

    if (pt_msg_q->prio[0] < pt_msg_q->prio[1])
    {
        i = 0;
    }
    else
    {
        i = 1;
    }

    /* check high priority has data or not */
    if (pt_msg_q->read[i] == pt_msg_q->write[i])
    {
        i = 1 - i;
    }

    /* if there is no data on the other queue, return */
    if (pt_msg_q->read[i] == pt_msg_q->write[i])
    {
        panic("x_msg_q_receive(): check it.\n");
        return OSR_NO_MSG;
    }

    read = pt_msg_q->read[i];
    z_size = *(SIZE_T *)(read);
    if (z_size > *pz_size)
    {
        z_size = *pz_size;
    }
    memcpy(pv_msg, read + sizeof(SIZE_T), z_size);
    read += pt_msg_q->message_size;
    if (read == pt_msg_q->end[i])
    {
        read = pt_msg_q->start[i];
    }
    pt_msg_q->read[i] = read;
    wake_up_all(&pt_msg_q->read_wq);

    *pui2_index = 0;
    *pz_size = z_size;

    return OSR_OK;
}


INT32
x_msg_q_num_msgs(HANDLE_T   h_msg_hdl,
                 UINT16     *pui2_num_msgs)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;
    SIZE_T messages, total_messages=0;
    unsigned long i;

    if (pui2_num_msgs == NULL)
    {
        return OSR_INV_ARG;
    }

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(h_msg_hdl);

    for (i=0; i<2; i++)
    {
        messages = pt_msg_q->write[i] - pt_msg_q->read[i];
	if (pt_msg_q->write[i] < pt_msg_q->read[i])
        {
	    messages += pt_msg_q->end[i] - pt_msg_q->start[i];
        }
	messages /= pt_msg_q->message_size;
	total_messages += messages;
    }
    *pui2_num_msgs = (UINT16)(total_messages);

    return OSR_OK;
}


INT32
x_msg_q_get_max_msg_size(HANDLE_T   h_msg_hdl,
                         SIZE_T     *pz_max_size)
{
    OS_MSGQ_LIGHT_T *pt_msg_q;

    if (pz_max_size == NULL)
    {
        return OSR_INV_ARG;
    }

    pt_msg_q = (OS_MSGQ_LIGHT_T *)(h_msg_hdl);
    *pz_max_size = pt_msg_q->z_maxsize;

    return OSR_OK;
}


INT32
msg_q_init(VOID)
{
    return OSR_OK;
}


