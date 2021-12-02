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


#include <linux/jiffies.h>
#include <linux/module.h>
#include <linux/param.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_timer.h"

#define TIMER_LIMIT             0xffffffff
#define TIMER_ID                1
#define TIMER_REG               (REG_RW_TIMER0_LOW + TIMER_ID * 8)

static VOID TimerHISR(unsigned long param);

static const UINT32 s_timer_res = 1000 / HZ;
static TIME_STAMP_T _arTimeStamp[MAX_SW_TIMESTAMP_SIZE];
static UINT32 _u4TimeStampSize = 0;
static struct timer_list os_tick_timer = TIMER_INITIALIZER(TimerHISR, 0, 0);

/* FIXME: ask why? MHF Linux - mcn03047 */ /* MHF Linux - jiang */
static wait_queue_head_t wt_tick;
static int tick_cond;

static int s_timer_inited=0;

static VOID TimerHISR(unsigned long param)
{
    UNUSED(param);
#if 1  /* MHF Linux - jiang */
    tick_cond++;
    wake_up_interruptible(&wt_tick);
#endif

    os_tick_timer.expires ++;
    add_timer(&os_tick_timer);
}

VOID x_polling_tick(void)
{
    wait_event_interruptible(wt_tick,(tick_cond > 0));
    if (tick_cond > 0)
       tick_cond--;
}

/* MHF Linux - jiang */

/*----------------------------------------------------------------------------
 * Function: x_os_drv_get_time()
 *
 * Description:
 *      to get system boot up time.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID x_os_drv_get_time(UINT32 *pui4Seconds, UINT32 *pui4Micros)
{
    HAL_TIME_T rTime;
    HAL_GetTime(&rTime);
    if ((pui4Seconds == NULL) || (pui4Micros == NULL))
    {
        return;
    }
    *pui4Seconds = rTime.u4Seconds;
    *pui4Micros = rTime.u4Micros;
}

//FIXME: ask user to stop use it
/*----------------------------------------------------------------------------
 * Function: os_get_sys_tick()
 *
 * Description:
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      Middleware tick count.
 *---------------------------------------------------------------------------*/
UINT32
os_get_sys_tick(void)
{
    return(jiffies);
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_get_fine_tick
 *
 * Description:
 *      This function returns the current tick value of hardware timer running
 *      at bus clock speed
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      The current tick value of hardware timer
 *---------------------------------------------------------------------------*/
UINT32
x_os_drv_get_fine_tick(VOID)
{
    UINT32 u4Ticks;

    u4Ticks = BIM_READ32(TIMER_REG);
    return TIMER_LIMIT - u4Ticks;
}


/*----------------------------------------------------------------------------
 * Function: x_os_drv_set_timestamp()
 *
 * Description:
 *      to get system boot up time.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
VOID x_os_drv_set_timestamp(CHAR *szString)
{
    if (_u4TimeStampSize >= MAX_SW_TIMESTAMP_SIZE)
    {
        _u4TimeStampSize = MAX_SW_TIMESTAMP_SIZE - 1;
    }
    _arTimeStamp[_u4TimeStampSize].u4TimeStamp = BIM_READ32(REG_RW_TIMER2_LOW);
    _arTimeStamp[_u4TimeStampSize].szString = szString;
    _u4TimeStampSize++;
}

/*----------------------------------------------------------------------------
 * Function: x_os_drv_get_timestamp()
 *
 * Description:
 *      to get system boot up timestamp list.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
TIME_STAMP_T * x_os_drv_get_timestamp(UINT32 *pu4Size)
{
    *pu4Size = _u4TimeStampSize;
    return _arTimeStamp;
}


typedef struct os_timer_light
{
    struct timer_list timer;
    x_os_timer_cb_fct pf_callback;
    VOID *pv_tag;
    BOOL fg_active;
    unsigned long interval;
} OS_TIMER_LIGHT_T;



/*----------------------------------------------------------------------------
 * Function: x_os_drv_get_tick_period()
 *
 * Description:
 *      this API returns tick's resolution is millisecond (ms). it performs
 *      1. get underlying OS HZ definition.
 *      2. round up resolution.
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns:
 *      tick's resolution is millisecond (ms).
 *---------------------------------------------------------------------------*/
UINT32
x_os_drv_get_tick_period()
{
    return s_timer_res;
}

static void TimerProc(unsigned long id)
{
    OS_TIMER_LIGHT_T *pt_timer;

    pt_timer = (OS_TIMER_LIGHT_T *)(id);
    pt_timer->pf_callback((HANDLE_T)(pt_timer), pt_timer->pv_tag);

    if (pt_timer->interval != 0)
    {
        pt_timer->timer.expires += pt_timer->interval;
        add_timer(&pt_timer->timer);
    }
    else
    {
        pt_timer->fg_active = FALSE;
    }
}


INT32 x_timer_create (HANDLE_T *ph_timer)
{
    OS_TIMER_LIGHT_T *pt_timer;

    if (ph_timer == NULL)
    {
        return OSR_INV_ARG;
    }

    pt_timer = kcalloc(1, sizeof(OS_TIMER_LIGHT_T), GFP_KERNEL);
    if (pt_timer == NULL)
    {
        return OSR_NO_RESOURCE;
    }
    //FILL_CALLER(pt_timer);

    init_timer(&pt_timer->timer);

    *ph_timer = (HANDLE_T)(pt_timer);
    return OSR_OK;
}


INT32 x_timer_start (HANDLE_T           h_timer,
                     UINT32             ui4_delay,
                     TIMER_FLAG_T       e_flags,
                     x_os_timer_cb_fct  pf_callback,
                     VOID*              pv_tag)
{
    OS_TIMER_LIGHT_T *pt_timer;
    unsigned long j;

    if ((pf_callback == NULL) || (ui4_delay == 0) ||
        ((e_flags != X_TIMER_FLAG_ONCE) && (e_flags != X_TIMER_FLAG_REPEAT)))
    {
        return OSR_INV_ARG;
    }

    j = jiffies;
    pt_timer = (OS_TIMER_LIGHT_T *)(h_timer);
    pt_timer->timer.expires = j + ui4_delay / s_timer_res;
    pt_timer->timer.function = &TimerProc;
    pt_timer->timer.data = (unsigned long)pt_timer;
    if (e_flags == X_TIMER_FLAG_ONCE)
    {
        pt_timer->interval = 0;
    }
    else
    {
        pt_timer->interval = ui4_delay / s_timer_res;
    }

    pt_timer->pf_callback = pf_callback;
    pt_timer->pv_tag = pv_tag;
    pt_timer->fg_active = TRUE;
    add_timer(&pt_timer->timer);

    return OSR_OK;
}


INT32 x_timer_stop (HANDLE_T  h_timer)
{
    OS_TIMER_LIGHT_T *pt_timer;

    pt_timer = (OS_TIMER_LIGHT_T *)(h_timer);
    if (pt_timer->fg_active)
    {
        del_timer_sync(&pt_timer->timer);
        pt_timer->fg_active = FALSE;
        pt_timer->timer.expires -= jiffies;
    }
    return OSR_OK;
}


INT32 x_timer_delete (HANDLE_T  h_timer)
{
    OS_TIMER_LIGHT_T *pt_timer;

    pt_timer = (OS_TIMER_LIGHT_T *)(h_timer);
    if (pt_timer->fg_active)
    {
        del_timer_sync(&pt_timer->timer);
    }
    kfree(pt_timer);
    return OSR_OK;
}


INT32 x_timer_resume (HANDLE_T  h_timer)
{
    OS_TIMER_LIGHT_T *pt_timer;

    pt_timer = (OS_TIMER_LIGHT_T *)(h_timer);
    if (!pt_timer->fg_active)
    {
        pt_timer->timer.expires += jiffies;
        pt_timer->fg_active = TRUE;
        add_timer(&pt_timer->timer);
    }

    return OSR_OK;
}


UINT32 x_os_get_sys_tick (VOID)
{
    return jiffies;
}


UINT32 x_os_get_sys_tick_period (VOID)
{
    return s_timer_res;
}


INT32 os_timer_init (VOID)
{
    if (s_timer_inited == 1)
    {
        return OSR_OK;
    }

    s_timer_inited = 1;

#if 1       /* MHF Linux - jiang */
    init_waitqueue_head(&wt_tick);
    tick_cond = 0;
#endif

    os_tick_timer.expires = jiffies + 1;
    add_timer(&os_tick_timer);

    return OSR_OK;
}

INT32
_timer_init(void)
{
    return os_timer_init();
}

