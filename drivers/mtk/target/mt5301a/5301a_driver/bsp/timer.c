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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: timer.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file timer.c
 *  timer.c provides timer function from boot time.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_bim.h"
#include "x_timer.h"
#include "x_assert.h"
// #include "x_os.h"
#include "x_ckgen.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define TICKS_PER_SECOND            1

// Valid TIMER_ID: 0, 1, 2
//
#define TIMER_ID                    2

#define TIMER_VECTOR                (VECTOR_T0 + TIMER_ID)
#define TIMER_IRQ                   (1 << TIMER_VECTOR)
#define TIMER_LOW_REG               (REG_RW_TIMER0_LOW + (TIMER_ID * 8))
#define TIMER_HIGH_REG              (REG_RW_TIMER0_HIGH + (TIMER_ID * 8))
#define TIMER_LIMIT_LOW_REG         (REG_RW_TIMER0_LLMT + (TIMER_ID * 8))
#define TIMER_LIMIT_HIGH_REG        (REG_RW_TIMER0_HLMT + (TIMER_ID * 8))
#define TCTL_TIMER_EN               (TMR_CNTDWN_EN(TIMER_ID))
#define TCTL_TIMER_AL               (TMR_AUTOLD_EN(TIMER_ID))
#define MAX_TIMER_INTERVAL          (0xffffffff)


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MTK_PRELOADER
static BOOL _fgInited = FALSE;
static UINT32 _u4TimerInterval;
static UINT32 _u4SysClock;
#else /* CC_MTK_PRELOADER */
#define _u4TimerInterval    (GET_XTAL_CLK() / TICKS_PER_SECOND)
#endif /* CC_MTK_PRELOADER */


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
/** _ResetTimer() Reset timer
 */
//-----------------------------------------------------------------------------
static void _ResetTimer(void)
{
    // Reset timer value
//    BIM_WRITE32(TIMER_LOW_REG, MAX_TIMER_INTERVAL);
//    BIM_WRITE32(TIMER_HIGH_REG, MAX_TIMER_INTERVAL);

    // Clear timer interrupt
    VERIFY(BIM_ClearIrq(TIMER_VECTOR));
}
#endif /* CC_MTK_PRELOADER */


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** HAL_InitTimer() Timer initialization
 *  @retval TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
BOOL HAL_InitTimer(void)
{
#ifndef CC_MTK_PRELOADER
    if (_fgInited)
    {
        return TRUE;
    }
    _fgInited = TRUE;

    // Get system clock and timer interval
    _u4SysClock = GET_XTAL_CLK();
    _u4TimerInterval = _u4SysClock / TICKS_PER_SECOND;

    // Disable timer
    BIM_REG32(REG_RW_TIMER_CTRL) &= ~(TCTL_TIMER_EN | TCTL_TIMER_AL);

    // Setup timer interrupt interval
    BIM_WRITE32(TIMER_LIMIT_LOW_REG, MAX_TIMER_INTERVAL);
    BIM_WRITE32(TIMER_LIMIT_HIGH_REG, MAX_TIMER_INTERVAL);

    // Reset timer
    _ResetTimer();

    // Enable timer with auto-load
    BIM_REG32(REG_RW_TIMER_CTRL) |= (TCTL_TIMER_EN | TCTL_TIMER_AL);
#endif /* CC_MTK_PRELOADER */

    return TRUE;
}


#ifndef CC_MTK_PRELOADER
//-----------------------------------------------------------------------------
/** HAL_ResetTimer() Reset timer
 *  @retval TRUE on success, FALSE otherwise
 */
//-----------------------------------------------------------------------------
BOOL HAL_ResetTimer(void)
{
    if (!_fgInited)
    {
        // Should initialize timer first
        return HAL_InitTimer();
    }

    _ResetTimer();

    return TRUE;
}
#endif /* CC_MTK_PRELOADER */


//-----------------------------------------------------------------------------
/** HAL_GetRawTime() Get system ticks and clock cycles since startup
 *  @param pRawTime [out] - A pointer to RAW_TIME_T to receive raw time
 */
//-----------------------------------------------------------------------------
void HAL_GetRawTime(HAL_RAW_TIME_T* pRawTime)
{
    UINT32 u4NewHigh, u4OldHigh, u4Low;

    ASSERT(pRawTime != NULL);

    u4OldHigh = BIM_READ32(TIMER_HIGH_REG);
    u4Low = BIM_READ32(TIMER_LOW_REG);
    u4NewHigh = BIM_READ32(TIMER_HIGH_REG);

    if (u4OldHigh != u4NewHigh)
    {
        pRawTime->u4Ticks = ~((u4Low < 0x10000000) ? u4OldHigh : u4NewHigh);
    }
    else
    {
        pRawTime->u4Ticks = ~u4OldHigh;
    }
    pRawTime->u4Cycles = ~u4Low;
}


//-----------------------------------------------------------------------------
/** HAL_RawToTime() Convert RAW_TIME_T to TIME_T
 *  @param pRawTime [in]  - Pointer to RAW_TIME_T, source
 *  @param pTime    [out] - Pointer to TIME_T, destination
 */
//-----------------------------------------------------------------------------
void HAL_RawToTime(const HAL_RAW_TIME_T* pRawTime, HAL_TIME_T* pTime)
{
    if ((pRawTime != NULL) && (pTime != NULL))
    {
        UINT64 u8Cycles;

        u8Cycles = pRawTime->u4Ticks;
        u8Cycles = u8Cycles << 32;
        u8Cycles += pRawTime->u4Cycles;
        pTime->u4Seconds = (UINT32)(u8Cycles / (UINT64)_u4TimerInterval);
        pTime->u4Micros = (UINT32)(((u8Cycles % (UINT64)_u4TimerInterval) * 1000000L) / _u4TimerInterval);
    }
}


//-----------------------------------------------------------------------------
/** HAL_GetTime() Get system time from startup
 *  @param pTime    [out] - Pointer to TIME_T to store system time
 */
//-----------------------------------------------------------------------------
void HAL_GetTime(HAL_TIME_T* pTime)
{
    HAL_RAW_TIME_T rt;

    HAL_GetRawTime(&rt);
    HAL_RawToTime(&rt, pTime);
}


//-----------------------------------------------------------------------------
/** HAL_GetDeltaTime() Get delta time of two time stamps
 *  @param pResult  [out] - The result
 *  @param pOlder   [in]  - The older time
 *  @param pNewer   [in]  - The newer time
 */
//-----------------------------------------------------------------------------
void HAL_GetDeltaTime(HAL_TIME_T* pResult, HAL_TIME_T* pT0,
    HAL_TIME_T* pT1)
{
    HAL_TIME_T* pNewer;
    HAL_TIME_T* pOlder;

    ASSERT(pResult != NULL);
    ASSERT(pT0 != NULL);
    ASSERT(pT1 != NULL);
    ASSERT((pT0->u4Micros < 1000000) && (pT1->u4Micros < 1000000));

    // Decide which one is newer
    if ((pT0->u4Seconds > pT1->u4Seconds) ||
        ((pT0->u4Seconds == pT1->u4Seconds) &&
        (pT0->u4Micros > pT1->u4Micros)))
    {
        pNewer = pT0;
        pOlder = pT1;
    }
    else
    {
        pNewer = pT1;
        pOlder = pT0;
    }

    if (pNewer->u4Seconds > pOlder->u4Seconds)
    {
        pResult->u4Seconds = pNewer->u4Seconds - pOlder->u4Seconds;
        if (pNewer->u4Micros >= pOlder->u4Micros)
        {
            pResult->u4Micros = pNewer->u4Micros - pOlder->u4Micros;
        }
        else
        {
            pResult->u4Micros = (1000000 + pNewer->u4Micros)
                - pOlder->u4Micros;
            pResult->u4Seconds--;
        }
    }
    else
    {
        // pNewer->u4Secons == pOlder->u4Seconds
        ASSERT(pNewer->u4Micros >= pOlder->u4Micros);
        pResult->u4Seconds = 0;
        pResult->u4Micros = pNewer->u4Micros - pOlder->u4Micros;
    }
}

//-----------------------------------------------------------------------------
/** HAL_GetDeltaTime() Get delta time of two time stamps
 *  @param pResult  [out] - The result
 *  @param pOlder   [in]  - The older time
 *  @param pNewer   [in]  - The newer time
 */
//-----------------------------------------------------------------------------
void HAL_GetDeltaRawTime(HAL_RAW_TIME_T* pResult, const HAL_RAW_TIME_T* pT0,
    const HAL_RAW_TIME_T* pT1)
{
	/*
    HAL_RAW_TIME_T* pNewer;
    HAL_RAW_TIME_T* pOlder;
	*/
	UINT64 u8New;
	UINT64 u8Old;
	UINT64 u8Result;
	
    ASSERT(pResult != NULL);
    ASSERT(pT0 != NULL);
    ASSERT(pT1 != NULL);
    
    u8New     = ((UINT64)pT1->u4Ticks << 32) | pT1->u4Cycles;
    u8Old     = ((UINT64)pT0->u4Ticks << 32) | pT0->u4Cycles;
    u8Result  = u8New - u8Old;
    pResult->u4Ticks  = u8Result >> 32;
    pResult->u4Cycles = u8Result & 0xffffffff;
    
    /*
    ASSERT((pT0->u4Cycles < _u4TimerInterval) &&
        (pT1->u4Cycles < _u4TimerInterval));

    if ((pT0->u4Ticks > pT1->u4Ticks) ||
        ((pT0->u4Ticks == pT1->u4Ticks) && (pT0->u4Cycles > pT1->u4Cycles)))
    {
        pNewer = pT0;
        pOlder = pT1;
    }
    else
    {
        pNewer = pT1;
        pOlder = pT0;
    }

    if (pNewer->u4Ticks > pOlder->u4Ticks)
    {
        pResult->u4Ticks = pNewer->u4Ticks - pOlder->u4Ticks;
        if (pNewer->u4Cycles >= pOlder->u4Cycles)
        {
            pResult->u4Cycles = pNewer->u4Cycles - pOlder->u4Cycles;
        }
        else
        {
            pResult->u4Cycles = (_u4TimerInterval + pNewer->u4Cycles)
                - pOlder->u4Cycles;
            pResult->u4Ticks--;
        }
    }
    else
    {
        // pNewer->u4Secons == pOlder->u4Seconds
        ASSERT(pNewer->u4Cycles >= pOlder->u4Cycles);
        pResult->u4Ticks = 0;
        pResult->u4Cycles = pNewer->u4Cycles - pOlder->u4Cycles;
    }
    */
}

//-----------------------------------------------------------------------------
/** HAL_Delay_us() delay X micro seconds
 *  @param u4Micro
 */
//-----------------------------------------------------------------------------
void HAL_Delay_us(UINT32 u4Micros)
{
    HAL_TIME_T rOrgTime, rNewTime, rDiffTime;

    // This function cannot delay more than 10 seconds.
    ASSERT(u4Micros < 10000000);

    if (u4Micros > 10000000)
    {
        u4Micros = 10000000;
    }

    HAL_GetTime(&rOrgTime);
    do {
        HAL_GetTime(&rNewTime);
        HAL_GetDeltaTime(&rDiffTime, &rOrgTime, &rNewTime);
    } while (((1000000*rDiffTime.u4Seconds)+rDiffTime.u4Micros) < u4Micros);
}

#if defined(CC_MTK_PRELOADER) && defined(__KERNEL__)
/* Replacement (=dummy) for GNU/Linux undefined reference on gcc preloader */
static void raise(void)
{
    return;
}

static void __aeabi_unwind_cpp_pr0(void)
{
    return;
}
#endif

