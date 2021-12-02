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
#include <linux/param.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/thread_info.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif
#include "x_assert.h"
#include "x_os.h"


#define INVALID_OWNER ((struct thread_info *)(NULL))


typedef struct os_sema_light
{
    SEMA_TYPE_T e_type;
    struct semaphore sem;
    struct thread_info *owner;
    INT16 i2_selfcount;
} OS_SEMA_LIGHT_T;


INT32 x_sema_create (HANDLE_T*    ph_sema_hdl,
                     SEMA_TYPE_T  e_type,
                     UINT32       ui4_init_value)
{
    OS_SEMA_LIGHT_T *pt_sema;

    /* check arguments */
    if ((ph_sema_hdl == NULL) ||
        ((e_type != X_SEMA_TYPE_BINARY) && (e_type != X_SEMA_TYPE_MUTEX) &&
         (e_type != X_SEMA_TYPE_COUNTING)))
    {
        return OSR_INV_ARG;
    }

    if (((e_type == X_SEMA_TYPE_BINARY) || (e_type == X_SEMA_TYPE_MUTEX)) &&
         (ui4_init_value != X_SEMA_STATE_LOCK) &&
         (ui4_init_value != X_SEMA_STATE_UNLOCK))
    {
        return OSR_INV_ARG;
    }

    if ((e_type == X_SEMA_TYPE_COUNTING) &&
        (((INT32) ui4_init_value) < ((INT32) X_SEMA_STATE_LOCK)))
    {
        return OSR_INV_ARG;
    }

    pt_sema = kcalloc(1, sizeof(OS_SEMA_LIGHT_T), GFP_KERNEL);
    if (pt_sema == NULL)
    {
        return OSR_NO_RESOURCE;
    }
    //FILL_CALLER(pt_sema);

    pt_sema->e_type = e_type;
    sema_init(&pt_sema->sem, ui4_init_value);

    if (e_type == X_SEMA_TYPE_MUTEX)
    {
        if (ui4_init_value == X_SEMA_STATE_LOCK)
        {
            pt_sema->owner = current_thread_info();
            pt_sema->i2_selfcount++;
        }
        else
        {
            pt_sema->owner = INVALID_OWNER;
            pt_sema->i2_selfcount = 0;
        }
    }

    *ph_sema_hdl = (HANDLE_T)(pt_sema);
    return OSR_OK;
}


INT32 x_sema_delete (HANDLE_T  h_sema_hdl)
{
    OS_SEMA_LIGHT_T *pt_sema = (OS_SEMA_LIGHT_T *)(h_sema_hdl);

    kfree(pt_sema);
    return OSR_OK;
}


INT32 x_sema_lock (HANDLE_T       h_sema_hdl,
                   SEMA_OPTION_T  e_option)
{
    OS_SEMA_LIGHT_T *pt_sema = (OS_SEMA_LIGHT_T *)(h_sema_hdl);

    if (e_option != X_SEMA_OPTION_WAIT && e_option != X_SEMA_OPTION_NOWAIT)
    {
        return OSR_INV_ARG;
    }

    if (pt_sema->e_type == X_SEMA_TYPE_MUTEX)
    {
        if (pt_sema->owner != current_thread_info())
        {
            if (e_option == X_SEMA_OPTION_NOWAIT)
            {
                if (down_trylock(&pt_sema->sem) != 0)
                {
                    return OSR_WOULD_BLOCK;
                }
            }
            else
            {
                down(&pt_sema->sem);
            }
            pt_sema->owner = current_thread_info();
        }
        pt_sema->i2_selfcount++;
        return OSR_OK;
    }
    else
    {
        if (e_option == X_SEMA_OPTION_NOWAIT)
        {
            if (down_trylock(&pt_sema->sem) != 0)
            {
                return OSR_WOULD_BLOCK;
            }
        }
        else
        {
            down(&pt_sema->sem);
        }

        return OSR_OK;
    }
}


INT32 x_sema_lock_timeout (HANDLE_T  h_sema_hdl,
                           UINT32    ui4_time)
{
    static const int quantum_ms = 1000 / HZ;
    OS_SEMA_LIGHT_T *pt_sema = (OS_SEMA_LIGHT_T *)(h_sema_hdl);
    int ret;

    if (ui4_time == 0)
    {
        INT32 i4 = x_sema_lock(h_sema_hdl, X_SEMA_OPTION_NOWAIT);
        return i4 != OSR_WOULD_BLOCK ? i4 : OSR_TIMEOUT;
    }

    if (pt_sema->e_type == X_SEMA_TYPE_MUTEX)
    {
        if (pt_sema->owner != current_thread_info())
        {
            ret = down_timeout(&pt_sema->sem, ui4_time / quantum_ms);
            if (ret != 0)
            {
                goto err;
            }
            pt_sema->owner = current_thread_info();
        }
        pt_sema->i2_selfcount++;
        return OSR_OK;
    }
    else
    {
        ret = down_timeout(&pt_sema->sem, ui4_time / quantum_ms);
        if (ret != 0)
        {
            goto err;
        }

        return OSR_OK;
    }

err:
    switch (ret)
    {
    case -ETIME:
        return OSR_TIMEOUT;

    default:
        return OSR_FAIL;
    }
}


INT32 x_sema_unlock (HANDLE_T  h_sema_hdl)
{
    OS_SEMA_LIGHT_T *pt_sema = (OS_SEMA_LIGHT_T *)(h_sema_hdl);

    if (pt_sema->e_type == X_SEMA_TYPE_MUTEX)
    {
        if (pt_sema->owner != current_thread_info())
        {
            return OSR_FAIL;
        }
        pt_sema->i2_selfcount--;
        if (pt_sema->i2_selfcount != 0)
        {
            return OSR_OK;
        }
        pt_sema->owner = INVALID_OWNER;

        up(&pt_sema->sem);
        return OSR_OK;
    }
    else
    {
        up(&pt_sema->sem);
        return OSR_OK;
    }
}


INT32 os_sema_init (VOID)
{
    return OSR_OK;
}


INT32 os_cli_show_sema_all (INT32        i4_argc,
                            const CHAR   **pps_argv)
{
    return OSR_OK;
}


