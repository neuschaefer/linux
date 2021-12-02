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
 * $RCSfile: hal.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file hal.c
 *  Hardware abstraction rountines.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_bim.h"
#define __USE_XBIM_IRQS        // UGLY HACK! Make sure we don't include mach/irqs.h from kernel.

#include "hal.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include <asm/io.h>
#include <linux/semaphore.h>
#include "x_linux.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MMU_ENABLE                  (1 << 0)

#ifdef CC_UBOOT
#error This hal should not be used by uboot.
#endif

extern spinlock_t mt53xx_bim_lock;

//-----------------------------------------------------------------------------
// Benchmark functions
//-----------------------------------------------------------------------------

// See mt53xx_com/53xx_com_driver/bsp/hal_bench.h for info on how to use
// these.
#define DO_HAL_BENCH_CRIT
#define DO_HAL_BENCH_INIT
#include "hal_bench.h"

void __init HalInitBenchmark(void)
{
    _HalInitBenchmark();
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

// Sanity check.
#if defined(HAL_CRIT_FORCE_FUNCTION) && defined(HAL_CRIT_FORCE_INLINE_IRQ_RESTORE)
#error Both HAL_CRIT_FORCE_FUNCTION and HAL_CRIT_FORCE_INLINE_IRQ_RESTORE are set.
#endif

#if defined(CONFIG_SMP) || defined(HAL_CRIT_FORCE_FUNCTION)
static DEFINE_SPINLOCK(HalGlobalLock);
static struct task_struct *pHalLockOwner;
static int iHalLockLevel;


UINT32 HalCriticalStart(void)
{
    unsigned long flags;
    if (!spin_trylock_irqsave(&HalGlobalLock, flags))
    {
        if (get_current() == pHalLockOwner)
        {
            // Locked, but I am owner.
            iHalLockLevel++;
            return flags;
        }
        spin_lock_irqsave(&HalGlobalLock, flags);
    }

    pHalLockOwner = get_current();
    HalCriticalStartTimer(__builtin_return_address(0));
    return (UINT32)flags;
}

#if !defined(HAL_CRIT_NEED_DO_CRITICAL_END)
void HalCriticalEnd(UINT32 u4Flags)
{
    unsigned long flags = (unsigned long)u4Flags;
    if (iHalLockLevel)
    {
        iHalLockLevel--;
        return;
    }

    pHalLockOwner = 0;
    HalCriticalEndTimer();
    spin_unlock_irqrestore(&HalGlobalLock, flags);
}
#endif /* !HAL_CRIT_NEED_DO_CRITICAL_END */
#endif /* SMP || HAL_CRIT_FORCE_FUNCTION */

#ifdef HAL_CRIT_NEED_DO_CRITICAL_END
BOOL DoHalCriticalEnd(void)
{
    if (iHalLockLevel)
    {
        iHalLockLevel--;
        return 0;
    }

    pHalLockOwner = 0;
    HalCriticalEndTimer();
    spin_unlock(&HalGlobalLock);
    return 1;
}
#endif /* HAL_CRIT_NEED_DO_CRITICAL_END */

UINT32 HalCountLeadingZero(UINT32 r1)
{
    register UINT32 r0;

    __asm__ ("CLZ     %0, %1" : "=r" (r0) : "r" (r1));
    return r0;
}

//-----------------------------------------------------------------------------
/** BIM_GetOscillator() Get ring oscillator raw value.
 *  @retval return the ring oscillator raw value.
 */
//-----------------------------------------------------------------------------
UINT32 BIM_GetOscillator()
{
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_CLR);
    BIM_WRITE32(REG_RW_ROSCCTL, ROSC_SEL);
    BIM_WRITE32(REG_RW_ROSCCTL, (ROSC_SEL | ROSC_TRIGGER));
    x_thread_delay(10);
    return BIM_READ32(REG_RO_ROSCCNT);
}

//-----------------------------------------------------------------------------
/** BIM_LockModifyReg32() Read/and/or/Write register with lock held 
 *  to prevent modify register from other threads.
 */
//-----------------------------------------------------------------------------
void BIM_LockModifyReg32(UINT32 reg, UINT32 mask, UINT32 setVal)
{
    UINT32 u4State, u4Val;

    spin_lock_irqsave(&mt53xx_bim_lock, u4State);
    u4Val = BIM_READ32(reg);
    u4Val &= mask;
    u4Val |= setVal;
    BIM_WRITE32(reg, u4Val);
    spin_unlock_irqrestore(&mt53xx_bim_lock, u4State);
}

void HalDisableMMU(void)
{
    ASSERT(0);
}

BOOL HalIsMMUEnabled(void)
{
    register UINT32 r;

    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=g" (r));

    return ((r & MMU_ENABLE) ? TRUE : FALSE);
}

/* refer to arch/arm/include/asm/proc-fns.h */
UINT32 HalCpuGetPhyPgd(void)
{
    unsigned long pg;
    __asm__("mrc        p15, 0, %0, c2, c0, 0"  : "=r" (pg) : : "cc");
    pg &= ~0x3fff;
    return pg;
}

UINT32 HalGetMMUTableAddress(void)
{
    return HalCpuGetPhyPgd();
}
