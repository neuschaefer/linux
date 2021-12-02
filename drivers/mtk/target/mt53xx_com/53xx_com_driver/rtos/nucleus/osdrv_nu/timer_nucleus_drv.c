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
 *
 * Description:
 *
 *---------------------------------------------------------------------------*/

/* Middleware header files */
#include "driver_os.h"
#include "timer_nucleus_drv.h"
#include "nucleus.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_os.h"

//---------------------------------------------------------------------------
// Config

#define USE_HW_TIMER            0


//---------------------------------------------------------------------------
// Constant definitions

#define OS_TIMER_NAME           "OSDRV"
#define OS_TIMER_ID             1

#if USE_HW_TIMER

// Valid TIMER_ID: 0, 1, 2
//
#define TIMER_ID                1

// Timer register definitions
#define TIMER_VECTOR            (VECTOR_T0 + TIMER_ID)
#define TIMER_IRQ               (1 << TIMER_VECTOR)
#define TIMER_REG               (REG_RW_TIMER0_LOW + TIMER_ID * 8)
#define TIMER_LIMIT_REG         (REG_RW_TIMER0_LLMT + TIMER_ID * 8)
#define TCTL_TIMER_EN           (TMR0_CNTDWN_EN << (TIMER_ID * 8))
#define TCTL_TIMER_AL           (TMR0_AUTOLD_EN << (TIMER_ID * 8))

#define TIMER_LIMIT             0xffffffff

#endif /* USE_HW_TIMER */

//---------------------------------------------------------------------------
// Static variables

static NU_TIMER _rTimer;
static UINT32 _u4TickRes;
static UINT32 _u4TimeStampSize = 0;
static TIME_STAMP_T _arTimeStamp[MAX_SW_TIMESTAMP_SIZE];

//---------------------------------------------------------------------------
// Static functions

//---------------------------------------------------------------------------
// Timer HISR, called by Nucleus kernel in HISR context
// This function is just to tick the timer library
// Input
//      uId: The timer ID
// Output
//      -
// Return
//      -
//---------------------------------------------------------------------------
static VOID TimerHISR(UNSIGNED uId)
{
    UNUSED(uId);

    // Invoke OS tick function
    x_os_drv_tick();
}

//---------------------------------------------------------------------------
// Public functions

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
    return _u4TickRes;
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
#if USE_HW_TIMER
    UINT32 u4Ticks;

    u4Ticks = BIM_READ32(TIMER_REG);
    return TIMER_LIMIT - u4Ticks;
#else /* USE_HW_TIMER */
    HAL_RAW_TIME_T rRawTime;

    HAL_GetRawTime(&rRawTime);
    return rRawTime.u4Cycles;
#endif /* USE_HW_TIMER */
}


/*----------------------------------------------------------------------------
 * Function: os_drv_timer_init()
 *
 * Description:
 *      Initialize timer library
 *      1. Create a timer, to get timer tick regularly
 *
 * Inputs: -
 *
 * Outputs: -
 *
 * Returns: -
 *---------------------------------------------------------------------------*/
INT32
os_drv_timer_init(void)
{
    UINT32 u4Left;

    // Create a timer HISR to tick OSAI
    x_memset(&_rTimer, 0, sizeof(NU_TIMER));
    if (NU_Create_Timer(&_rTimer, OS_TIMER_NAME, TimerHISR, OS_TIMER_ID, 1, 1,
        NU_ENABLE_TIMER) != NU_SUCCESS)
    {
        return OSR_DRV_FAIL;
    }

    // Compute tick period in millisecond
    _u4TickRes = 1000 / NU_PLUS_Ticks_Per_Second;        /* 1000 ms = 1 sec */
    u4Left = 1000 - (_u4TickRes * NU_PLUS_Ticks_Per_Second);

    //lint --e(774) Boolean within 'if' always evaluates to False
    if (u4Left >= (NU_PLUS_Ticks_Per_Second >> 1))  /* round up */
    {
        ++_u4TickRes;
    }

#if USE_HW_TIMER
    // Initialize hardware timer for fine tick
    BIM_WRITE32(TIMER_REG, 0);                  // Reset timer value
    BIM_WRITE32(TIMER_LIMIT_REG, TIMER_LIMIT);  // Set timer limit
    BIM_REG32(REG_RW_TIMER_CTRL) |= (TCTL_TIMER_EN | TCTL_TIMER_AL); // Enable timer with auto-reload
#endif /* USE_HW_TIMER */

    // Debug, reset HAL timer to sync timings between driver and middleware
    if (!HAL_ResetTimer())
    {
        return FALSE;
    }

    return OSR_DRV_OK;
}

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

