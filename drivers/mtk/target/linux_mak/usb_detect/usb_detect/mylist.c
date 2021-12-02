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
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <semaphore.h>

#include "mylist.h"

#define MY_LIST_LOGIC 0xBC190DF3

extern void (*pf_free) (void* pv_data);
extern int (*pf_cmp) (const void *pv1, const void *pv2);

void** my_list_toarray (handle_t h_handle, void *end);
int my_list_map (handle_t h_handle, int apply(void*, void*), void* pv_arg);
int my_list_create (handle_t *ph_handle);
int my_list_destroy (handle_t h_handle, void (*pf_free) (void* pv_data));
void* my_list_delete_head (handle_t h_handle);
void* my_list_delete (handle_t h_handle, const void *pv_data);
void* my_list_delete2 (handle_t h_handle, const void *pv_data, int (*pf_cmp) (const void *pv1, const void* pv2));
int my_list_add (handle_t h_handle, void *pv_data);
void *my_list_search (handle_t h_handle, const void *pv_data);
void *my_list_search2 (handle_t h_handle, const void *pv_data, int (*pf_cmp) (const void *pv1, const void* pv2));

struct st_my_list_head {
    unsigned int ui4_logic;
    void *pv_head;
    void *pv_tail;
    unsigned int i4_len;
    sem_t sem;
};

struct st_my_list {
    struct st_my_list *pt_next;
    struct st_my_list *pt_prev;
    void *pv_data;
};

static void _my_list_lock (sem_t *sem)
{
    sem_wait (sem);
}

static void _my_list_unlock (sem_t *sem)
{
    sem_post (sem);
}

void** my_list_toarray (handle_t h_handle, void *end)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    void **ppv_array = NULL;
    int len = 0;
    int i = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    len = pt_head->i4_len;
    if (len == 0)
    {
        return NULL;
    }

    ppv_array = malloc (sizeof (void*) * (len + 1));
    if (ppv_array == NULL)
    {
        printf ("[LIST] out of memory\n");
        return NULL;
    }

    i = 0;
    pt_list = pt_head->pv_head;
    while (pt_list != NULL)
    {
        if (i >= len)
        {
            /* need lock else may have bug */
            break;
        }
        ppv_array[i++] = pt_list->pv_data;
        pt_list = pt_list->pt_next;
    }

    ppv_array[len] = end;

    return ppv_array;
}

int my_list_toarray2 (handle_t h_handle, void **ppv_array, int len)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return 0;
    }

    if (len < 1)
    {
        return 0;
    }

    i = 0;
    pt_list = pt_head->pv_head;
    while (pt_list != NULL)
    {
        if (i >= len)
        {
            /* need lock else may have bug */
            break;
        }
        ppv_array[i++] = pt_list->pv_data;
        pt_list = pt_list->pt_next;
    }

    return i;
}

int my_list_map (handle_t h_handle, int apply(void*, void*), void* pv_arg)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i4_len = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return -1;
    }

    if (pt_head->i4_len == 0)
    {
        return 0;
    }

    i4_len = pt_head->i4_len;

    pt_list = pt_head->pv_head;

    while (pt_list != NULL && i4_len-- != 0)
    {
        struct st_my_list *pt_next = pt_list->pt_next;
        apply (pt_list->pv_data, pv_arg);
        pt_list = pt_next;
    }

    return 0;
}

int my_list_size (handle_t h_handle)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return 0;
    }

    return pt_head->i4_len;
}

int my_list_create (handle_t *ph_handle)
{
    struct st_my_list_head *pt_head = NULL;

    if (ph_handle == NULL)
    {
        return -1;
    }

    pt_head = (struct st_my_list_head*) malloc (sizeof (struct st_my_list_head));
    if (pt_head == NULL)
    {
        return -1;
    }

    pt_head->ui4_logic = MY_LIST_LOGIC;
    pt_head->pv_head = NULL;
    pt_head->pv_tail = NULL;
    pt_head->i4_len = 0;

    sem_init (&pt_head->sem, 0, 1);

    *ph_handle = (handle_t) pt_head;

    return 0;
}

int my_list_destroy (handle_t h_handle, void (*pf_free)(void* pv_data))
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return -1;
    }

    /* call my_list_delete_head may have deal lock so not use lock() */
    /*_my_list_lock ();*/

    while (pt_head->i4_len != 0)
    {
        /* TODO */
        void *pv_data = my_list_delete_head (h_handle);
        if (pv_data != NULL && pf_free)
        {
            pf_free (pv_data);
        }
    }

    if (pt_head->i4_len == 0)
    {
        /*memset (pt_head, 0, sizeof (struct st_my_list_head));*/
        pt_head->ui4_logic = 0;
        free (pt_head);
    }

    /*_my_list_unlock ();*/

    return 0;
}

int my_list_add (handle_t h_handle, void *pv_data)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return -1;
    }

    if (pv_data == NULL)
    {
        return -1;
    }

    pt_list = (struct st_my_list*) malloc (sizeof (struct st_my_list));
    if (pt_list == NULL)
    {
        return -1;
    }

    memset (pt_list, 0, sizeof (struct st_my_list));

    pt_list->pv_data = pv_data;

    _my_list_lock (&pt_head->sem);

    /* */
    pt_list->pt_next = pt_head->pv_head;

    if (pt_head->pv_head)
    {
        ((struct st_my_list*)pt_head->pv_head)->pt_prev = pt_list;
    }

    pt_head->pv_head = pt_list;

    if (pt_head->pv_tail == NULL)
    {
        pt_head->pv_tail = pt_list;
    }

    ++pt_head->i4_len;

    _my_list_unlock (&pt_head->sem);

    return 0;
}

int my_list_add_head (handle_t h_handle, void *pv_data)
{
    return my_list_add (h_handle, pv_data);
}

int my_list_add_tail (handle_t h_handle, void *pv_data)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return -1;
    }

    if (pv_data == NULL)
    {
        return -1;
    }

    pt_list = (struct st_my_list*) malloc (sizeof (struct st_my_list));
    if (pt_list == NULL)
    {
        return -1;
    }

    memset (pt_list, 0, sizeof (struct st_my_list));

    pt_list->pv_data = pv_data;

    _my_list_lock (&pt_head->sem);

    /* */
    pt_list->pt_next = NULL;
    pt_list->pt_prev = pt_head->pv_tail;

    if (pt_head->pv_tail)
    {
        ((struct st_my_list*)pt_head->pv_tail)->pt_next = pt_list;
    }

    pt_head->pv_tail = pt_list;

    if (pt_head->pv_head == NULL)
    {
        pt_head->pv_head = pt_list;
    }

    ++pt_head->i4_len;

    _my_list_unlock (&pt_head->sem);

    return 0;
}

void* my_list_delete (handle_t h_handle, const void *pv_data)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i4_len = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    _my_list_lock (&pt_head->sem);

    i4_len = pt_head->i4_len;

    pt_list = pt_head->pv_head;

    while (pt_list != NULL && i4_len-- != 0)
    {
        if (pt_list->pv_data == pv_data)
        {
            if (pt_list->pt_prev != NULL)
            {
                pt_list->pt_prev->pt_next = pt_list->pt_next;
            }
            else
            {
                if (pt_head->pv_head == pt_list)
                {
                    pt_head->pv_head = pt_list->pt_next;
                }
            }

            if (pt_list->pt_next != NULL)
            {
                pt_list->pt_next->pt_prev = pt_list->pt_prev;
            }
            else
            {
                if (pt_head->pv_tail == pt_list)
                {
                    pt_head->pv_tail = pt_list->pt_prev;
                }
            }

            pt_list->pt_prev = NULL;
            pt_list->pt_next = NULL;
            pt_list->pv_data = NULL;

            free (pt_list);

            --pt_head->i4_len;

            _my_list_unlock (&pt_head->sem);

            return (void*) pv_data;
        }
        pt_list = pt_list->pt_next;
    }

    _my_list_unlock (&pt_head->sem);

    return NULL;
}

void* my_list_delete_tail (handle_t h_handle)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    void *pv_data;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    _my_list_lock (&pt_head->sem);

    pt_list = pt_head->pv_tail;

    if (pt_list == NULL)
    {
        _my_list_unlock (&pt_head->sem);
        return NULL;
    }

    pv_data = pt_list->pv_data;

    if (pt_head->pv_head == pt_list)
    {
        pt_head->pv_head = NULL;
        pt_head->pv_tail = NULL;
        pt_head->i4_len = 0;
    }
    else
    {
        pt_list->pt_prev->pt_next = pt_list->pt_next;
        pt_head->pv_tail = pt_list->pt_prev;
        --pt_head->i4_len;
    }

    free (pt_list);

    _my_list_unlock (&pt_head->sem);

    return pv_data;
}

void* my_list_delete2 (handle_t h_handle, const void *pv_data, int (*pf_cmp) (const void*, const void*))
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i4_len = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    _my_list_lock (&pt_head->sem);

    i4_len = pt_head->i4_len;

    pt_list = pt_head->pv_head;

    while (pt_list != NULL && i4_len-- != 0)
    {
        int i4_cmp = -1;

        if (pf_cmp == NULL)
        {
            if (pt_list->pv_data == pv_data)
            {
                i4_cmp = 0;
            }
        }
        else
        {
            i4_cmp = pf_cmp (pt_list->pv_data, pv_data);
        }

        /*if (pt_list->pv_data == pv_data)*/
        if (i4_cmp == 0)
        {
            void *pv_tmp = pt_list->pv_data;

            if (pt_list->pt_prev != NULL)
            {
                pt_list->pt_prev->pt_next = pt_list->pt_next;
            }
            else
            {
                if (pt_head->pv_head == pt_list)
                {
                    pt_head->pv_head = pt_list->pt_next;
                }
            }

            if (pt_list->pt_next != NULL)
            {
                pt_list->pt_next->pt_prev = pt_list->pt_prev;
            }
            else
            {
                if (pt_head->pv_tail == pt_list)
                {
                    pt_head->pv_tail = pt_list->pt_prev;
                }
            }

            pt_list->pt_prev = NULL;
            pt_list->pt_next = NULL;
            pt_list->pv_data = NULL;

            free (pt_list);

            --pt_head->i4_len;

            _my_list_unlock (&pt_head->sem);

            return pv_tmp;
        }
        pt_list = pt_list->pt_next;
    }

    _my_list_unlock (&pt_head->sem);

    return NULL;
}

void* my_list_delete_head (handle_t h_handle)
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    if (pt_head->pv_head != NULL)
    {
        return my_list_delete (h_handle, ((struct st_my_list*)(pt_head->pv_head))->pv_data);
    }

    return NULL;
}

void* my_list_search (handle_t h_handle, const void *pv_data)
{
#if 0
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i4_len = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    i4_len = pt_head->i4_len;

    pt_list = pt_head->pv_head;

    while (pt_list != NULL && i4_len-- != 0)
    {
        if (pt_list->pv_data == pv_data)
        {
            return pv_data;
        }
        pt_list = pt_list->pt_next;
    }

    return NULL;
#endif
    return my_list_search2 (h_handle, pv_data, NULL);
}

void* my_list_search2 (handle_t h_handle, const void *pv_data, int (*pf_cmp) (const void*, const void*))
{
    struct st_my_list_head *pt_head = (struct st_my_list_head*) h_handle;
    struct st_my_list *pt_list = NULL;
    int i4_len = 0;

    if (pt_head == NULL || pt_head->ui4_logic != MY_LIST_LOGIC)
    {
        return NULL;
    }

    if (pt_head->i4_len == 0)
    {
        return NULL;
    }

    _my_list_lock (&pt_head->sem);

    i4_len = pt_head->i4_len;

    pt_list = pt_head->pv_head;

    while (pt_list != NULL && i4_len-- != 0)
    {
        int i4_cmp = -1;

        if (pf_cmp == NULL)
        {
            if (pt_list->pv_data == pv_data)
            {
                i4_cmp = 0;
            }
        }
        else
        {
            i4_cmp = pf_cmp (pt_list->pv_data, pv_data);
        }

        /*if (pt_list->pv_data == pv_data)*/
        if (i4_cmp == 0)
        {
            _my_list_unlock (&pt_head->sem);
            return pt_list->pv_data;
        }
        pt_list = pt_list->pt_next;
    }

    _my_list_unlock (&pt_head->sem);

    return NULL;
}

