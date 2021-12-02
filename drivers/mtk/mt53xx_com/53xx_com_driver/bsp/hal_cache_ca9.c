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

/** @file hal_cache.c
 *  Hardware cache abstraction rountines.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_bim.h"
#define __USE_XBIM_IRQS        // UGLY HACK! Make sure we don't include mach/irqs.h from kernel.

#include "x_hal_arm.h"
#include "hal.h"
#include "x_os.h"
#include "x_assert.h"
#include <asm/io.h>
#include <asm/outercache.h>
#include <linux/semaphore.h>
#include "x_linux.h"

#ifdef CONFIG_SMP
#include <linux/smp.h>
#endif

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define ICACHE_ENABLE               (1 << 12)
#define DCACHE_ENABLE               (1 << 2)
#define DSIZE_MASK                  0xf
#define DSIZE_SHIFT                 (6 + 12)
#define ISIZE_MASK                  0xf
#define ISIZE_SHIFT                 6

#define MAX_MULTILINE_SIZE          (256*1024*1024)  // 256MB

// Translate VA (Virtual Address) to MVA (Modified Virtual Address)
#define MVA(addr)                   ((addr) & 0xffffffe0)
#define ICACHE_LINE_SIZE            32
#define DCACHE_LINE_SIZE            32

// Cache parameters (in bytes)
#define DCACHE_LINE_SIZE_MSK        (DCACHE_LINE_SIZE - 1)

#define L1_WAY                      4
#define L1_SET                      256
#define L1_SIZE                     32*1024

// Translate VA (Virtual Address) to MVA (Modified Virtual Address)
#define L2MVA(addr)                 __PHYSICAL(addr)
#define CC_ENABLE_L2C

#ifdef CC_UBOOT
#error This hal should not be used by uboot.
#endif

#ifdef CONFIG_SMP
#define can_flush_all()             (!irqs_disabled())
#define DCACHE_FLUSH_THRESHOLD      (512*1024)           // Clear all cache is expansive.
#else
#define can_flush_all()             (1)
#define DCACHE_FLUSH_THRESHOLD      (256*1024)           // Clear all cache is expansive.
#endif

//-----------------------------------------------------------------------------
// Benchmark functions
//-----------------------------------------------------------------------------

// See mt53xx_com/53xx_com_driver/bsp/hal_bench.h for info on how to enable
// and use benchmark.
#define DO_HAL_BENCH_CACHE
#include "hal_bench.h"

//-----------------------------------------------------------------------------
// Cache
//-----------------------------------------------------------------------------

inline UINT32 HalGetDCacheLineSize(void)
{
    return DCACHE_LINE_SIZE;
}

BOOL HalIsICacheEnabled(void)
{
    register UINT32 r;

    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=g" (r));

    return ((r & ICACHE_ENABLE) ? TRUE : FALSE);
}

inline UINT32 HalGetICacheSize(void)
{
#if 0
    register UINT32 r;
    UINT32 u4SizeField, u4Size = 0;

    __asm__ ("MRC   p15, 0, %0, c0, c0, 1" : "=g" (r));

    u4SizeField = (r >> ISIZE_SHIFT) & ISIZE_MASK;
    if (u4SizeField >= 3 && u4SizeField<=8)
        u4Size = 1 << (u4SizeField - 1);
    else
        ASSERT(0);
    return u4Size * 1024;
#endif
    return L1_SIZE;
}

inline UINT32 HalGetICacheLineSize(void)
{
    return ICACHE_LINE_SIZE;
}

BOOL HalIsDCacheEnabled(void)
{
    register UINT32 r;

    __asm__ ("MRC   p15, 0, %0, c1, c0, 0" : "=g" (r));

    return ((r & DCACHE_ENABLE) ? TRUE : FALSE);
}

UINT32 HalGetDCacheSize(void)
{
    return L1_SIZE;
}

void HalEnableCaches(void)
{
    ASSERT(0);
}

void HalEnableDCache(void)
{
    ASSERT(0);
}

void HalDisableDCache(void)
{
    ASSERT(0);
}

void HalDisableCaches(void)
{
    ASSERT(0);
}

void HalSetInstTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable)
{
    // No TCM in armv7
    ASSERT(0);
}

void HalSetDataTCMReg(UINT32 u4BaseAddr, UINT32 u4Size, BOOL fgEnable)
{
    // No TCM in armv7
    ASSERT(0);
}

void HalInvalidateAllCaches(void)
{
    // Not support in Linux BSP
    ASSERT(0);
}

inline void HalFlushWriteBuffer(void)
{
    __asm__ ("dsb");
}


static inline void _ArmFlushInvalidateDCacheRange(UINT32 u4Addr, UINT32 u4Size)
{
    __asm__ volatile
            ("      add   %1, %1, %0                  \n"
             "      bic   %0, %0, %2-1        @MVA    \n"
             "      sub   %1, %1, %0                  \n"
             "1:                                      \n"
             "      MCR   p15, 0, %0, c7, c14, 1      \n"
             "      subs  %1, %1, %2                  \n"
             "      add   %0, %0, %2                  \n"
             "      bgt   1b                          \n"
             "      dsb                               \n"
             : "+r" (u4Addr), "+r" (u4Size) : "i" (DCACHE_LINE_SIZE));
}

static inline void _ArmInvalidateDCacheRange(UINT32 u4Addr, UINT32 u4Size)
{
    __asm__ volatile
            ("      add   %1, %1, %0                  \n"
             "      tst   %0, %2-1                    \n"
             "      bic   %0, %0, %2-1        @MVA    \n"
             "      mcrne p15, 0, %0, c7, c14, 1 @ Clean & Inv first \n"
             ""
             "      tst   %1, %2-1                    \n"
             "      bic   %1, %1, %2-1        @MVA    \n"
             "      mcrne p15, 0, %1, c7, c14, 1 @ Clean & Inv last \n"
             "      sub   %1, %0                      \n"
             "1:                                      \n"
             "      MCR   p15, 0, %0, c7, c6, 1       \n"
             "      subs  %1, %1, %2                  \n"
             "      add   %0, %0, %2                  \n"
             "      bgt   1b                          \n"
             "      dsb                               \n"
             : "+r" (u4Addr), "+r" (u4Size) : "i" (DCACHE_LINE_SIZE));
}

static inline void _ArmFlushDCacheRange(UINT32 u4Addr, UINT32 u4Size)
{
    __asm__ volatile
            ("      add   %1, %1, %0                  \n"
             "      bic   %0, %0, %2-1        @MVA    \n"
             "      sub   %1, %1, %0                  \n"
             "1:                                      \n"
             "      MCR   p15, 0, %0, c7, c10, 1      \n"
             "      subs  %1, %1, %2                  \n"
             "      add   %0, %0, %2                  \n"
             "      bgt   1b                          \n"
             "      dsb                               \n"
             : "+r" (u4Addr), "+r" (u4Size) : "i" (DCACHE_LINE_SIZE));
}

static inline void _L2InvalidateRange(UINT32 u4Addr, UINT32 u4Size)
{
#ifdef CC_ENABLE_L2C
    outer_cache.inv_range(u4Addr, u4Addr+u4Size);
#endif
}

static inline void _L2FlushRange(UINT32 u4Addr, UINT32 u4Size)
{
#ifdef CC_ENABLE_L2C
    outer_cache.clean_range(u4Addr, u4Addr+u4Size);
#endif
}

static inline void _L2FlushInvalidateRange(UINT32 u4Addr, UINT32 u4Size)
{
#ifdef CC_ENABLE_L2C
    outer_cache.flush_range(u4Addr, u4Addr+u4Size);
#endif
}

void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm__ ("MCR   p15, 0, %0, c7, c14, 1\n"
             "dsb   \n"
             : : "r" (u4Addr));

    u4Addr = L2MVA(u4Addr);
    _L2FlushInvalidateRange(u4Addr, 1);
}

void HalInvalidateDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm__ ("MCR     p15, 0, %0, c7, c6, 1\n"
             "dsb     \n"
             : : "r" (u4Addr));

    u4Addr = L2MVA(u4Addr);
    _L2InvalidateRange(u4Addr, 1);
}

void HalFlushDCacheSingleLine(UINT32 u4Addr)
{
    u4Addr = MVA(u4Addr);

    __asm__ ("MCR     p15, 0, %0, c7, c10, 1\n"
             "dsb     \n"
             : : "r" (u4Addr));

    u4Addr = L2MVA(u4Addr);
    _L2FlushRange(u4Addr, 1);
}


static void _FlushInvalidateDCache(void*data)
{
    register UINT32 sets, reg1, reg2, reg3;

    // Do it by set/way hard-coded to 4 way
    //sets = HalGetDCacheSize() / 32 / 4;
    sets = L1_SET;
    __asm__ volatile (
             "       dmb                              \n"
             "       mov  %0, %3         @ Sets to go \n"
             "1:                                      \n"
             "       mov  %1, #3         @ 4 Ways     \n"
             "2:                                      \n"
             "       mov  %2, %1, lsl #30             \n"
             "       orr  %2, %2, %0, lsl #5          \n"
             "       mcr  p15, 0, %2, c7, c14, 2  @ Clean & Inv  \n"
             "       subs %1, %1, #1                  \n" 
             "       bge  2b                          \n"
             "       subs %0, %0, #1                  \n"
             "       bge  1b                          \n"
             "       dsb                              \n"
             : "=r" (reg1), "=r" (reg2), "=r" (reg3) : "r" (sets-1));
}

static void _FlushDCache(void*data)
{
    register UINT32 sets, reg1, reg2, reg3;

    // Do it by set/way hard-coded to 4 way
    //sets = HalGetDCacheSize() / 32 / 4;
    sets = L1_SET;
    __asm__ volatile (
             "       dmb                              \n"
             "       mov  %0, %3         @ Sets to go \n"
             "1:                                      \n"
             "       mov  %1, #3         @ 4 Ways     \n"
             "2:                                      \n"
             "       mov  %2, %1, lsl #30             \n"
             "       orr  %2, %2, %0, lsl #5          \n"
             "       mcr  p15, 0, %2, c7, c10, 2  @ Clean\n"
             "       subs %1, %1, #1                  \n" 
             "       bge  2b                          \n"
             "       subs %0, %0, #1                  \n"
             "       bge  1b                          \n"
             "       dsb                              \n"
             : "=r" (reg1), "=r" (reg2), "=r" (reg3) : "r" (sets-1));
}

static inline void _L2FlushInvalidateAll(void)
{
#ifdef CC_ENABLE_L2C
    // to assure no L2C operation now
    while (L2C_READ32(L2C_REG_INVALIDATE_WAY) > 0) {}
    while (L2C_READ32(L2C_REG_CLEAN_WAY) > 0) {}
    while (L2C_READ32(L2C_REG_CLEAN_AND_INVALIDATE_WAY) > 0) {}

    L2C_WRITE32(L2C_REG_CLEAN_AND_INVALIDATE_WAY, 0xff);
    while (L2C_READ32(L2C_REG_CLEAN_AND_INVALIDATE_WAY) > 0) {}
    L2C_WRITE32(L2C_REG_SYNC, 0);
    while (L2C_READ32(L2C_REG_SYNC) > 0) {}
#endif
}

static inline void _L2FlushAll(void)
{
#ifdef CC_ENABLE_L2C
    // to assure no L2C operation now
    while (L2C_READ32(L2C_REG_INVALIDATE_WAY) > 0) {}
    while (L2C_READ32(L2C_REG_CLEAN_WAY) > 0) {}
    while (L2C_READ32(L2C_REG_CLEAN_AND_INVALIDATE_WAY) > 0) {}

    L2C_WRITE32(L2C_REG_CLEAN_WAY, 0xff);
    while (L2C_READ32(L2C_REG_CLEAN_WAY) > 0) {}
    L2C_WRITE32(L2C_REG_SYNC, 0);
    while (L2C_READ32(L2C_REG_SYNC) > 0) {}
#endif
}


#ifdef CONFIG_SMP
struct _CacheOpInfo
{
    atomic_t aReg, aFail;
    atomic_t aStep, aDone, aOut;
};

static atomic_t aCacheOpCpu = ATOMIC_INIT(-1);
static DEFINE_SPINLOCK(HalCacheOpLock);


/*
 *  Wait for all CPUs to gather.
 *
 *  Doing registration with IRQ disable with max try loops.
 *
 *  @param pOp Use aReg, aFail to check if all CPUs already gathers.
 *  @return 0 if fail. 1 if all CPUS is in sync.
 *   !!NOTE!!  if return 1, IRQ is also disabled.
 */
#define WAIT_CPU_THRESHOLD 500
static int smpWaitForAllCPUs(struct _CacheOpInfo *pOp, unsigned long *pflag)
{
    unsigned long loops;
    int fail = 0;

    // Disable IRQ now.
    local_irq_save(*pflag);

    // Register here, and wait all others
    // Prevent dead-lock by waiting for at most WAIT_CPU_THRESHOLD
    atomic_sub(1, &pOp->aReg);

    loops = WAIT_CPU_THRESHOLD;
    while (loops-- && atomic_read(&pOp->aReg) && !atomic_read(&pOp->aFail))
    {
        // Allow others to access.
        __asm__ volatile(" nop; nop");
    }

    // Use spin-lock to make sure no race condition on aReg/aFail check.
    spin_lock(&HalCacheOpLock);
    if (atomic_read(&pOp->aReg))
        atomic_add(1, &pOp->aFail);
    fail = atomic_read(&pOp->aFail);
    spin_unlock(&HalCacheOpLock);

    if (fail)
    {
        atomic_sub(1, &pOp->aOut);
        local_irq_restore(*pflag);
        return 0;
    }

    return 1;
}

static void smpDoCacheOps(void (*func) (void *data))
{
    struct _CacheOpInfo opInfo;
    int numCpu = num_online_cpus();
    int force;

    while (1)
    {
        // Make sure only I can send cache ops to prevent deadlock.
        // Calling other IPI from other cores should be OK.
        // Must disable preemption, otherwise it might hang on signel CPU
        preempt_disable();
        force = 0;
        if (atomic_cmpxchg(&aCacheOpCpu, -1, smp_processor_id()) != -1)
        {
            if (atomic_read(&aCacheOpCpu) != smp_processor_id())
            {
                preempt_enable();
                continue;
            }

            // Cache flush ops interrupted by me on same CPU, OK to proceed.
            force=1;
        }

        atomic_set(&opInfo.aReg, numCpu);
        atomic_set(&opInfo.aFail, 0);
        atomic_set(&opInfo.aDone, numCpu);
        atomic_set(&opInfo.aOut, numCpu);
        atomic_set(&opInfo.aStep, 0);

        if (numCpu > 1)
            smp_call_function(func, &opInfo, 0);

        // Call op by myself.
        func(&opInfo);

        // Wait for all others to finish.
        while (atomic_read(&opInfo.aOut))
        {
            // Allow others to access.
            __asm__ volatile(" nop; nop");
        }

        // cache op done, clear
        if (!force)
            atomic_set(&aCacheOpCpu, -1);
        preempt_enable();

        // Retry if fail
        if (!atomic_read(&opInfo.aFail))
            break;
    };
}

static void smpFlushInvalidateDCache(void *data)
{
    struct _CacheOpInfo *pOp = (struct _CacheOpInfo*)data;
    unsigned long flags;

    // Wait CPUs to gather
    if (!smpWaitForAllCPUs(pOp, &flags))
        return;

    // Now all workers are here, we can finally start working.
    _FlushDCache(0);
    atomic_sub(1, &pOp->aDone);
    while (atomic_read(&pOp->aDone))
    {
        // Allow others to access.
        __asm__ volatile(" nop; nop");
    }

    // CPU 0 will flush L2, others just wait for it to complete.
    if (smp_processor_id() == 0)
    {
        _L2FlushInvalidateAll();
        atomic_add(1, &pOp->aStep);
    }
    else
    {
        while (!atomic_read(&pOp->aStep))
        {
            // Allow others to access.
            __asm__ volatile(" nop; nop");
        }
    }

    // Final step.
    _FlushInvalidateDCache(0);

    // Restore IRQ.
    atomic_sub(1, &pOp->aOut);
    local_irq_restore(flags);
}

static inline void __HalFlushInvalidateDCache(void)
{
    //ASSERT(!irqs_disabled() && "Calling HalFlushInvalidateDCache() with IRQ disabled.");
    smpDoCacheOps(smpFlushInvalidateDCache);
}

void HalFlushInvalidateDCache(void)
{
    HalCacheFlushSizeRecord(__builtin_return_address(0), 1*1024*1024*1024);
    __HalFlushInvalidateDCache();
}

static void smpFlushDCache(void *data)
{
    struct _CacheOpInfo *pOp = (struct _CacheOpInfo*)data;
    unsigned long flags;

    // Wait CPUs to gather
    if (!smpWaitForAllCPUs(pOp, &flags))
        return;

    // Now all workers are here, we can finally start working.
    _FlushDCache(0);
    atomic_sub(1, &pOp->aDone);
    while (atomic_read(&pOp->aDone))
    {
        // Allow others to access.
        __asm__ volatile(" nop; nop");
    }

    // CPU 0 will flush L2, others just wait for it to complete.
    if (smp_processor_id() == 0)
    {
        _L2FlushAll();
        atomic_add(1, &pOp->aStep);
    }
    else
    {
        while (!atomic_read(&pOp->aStep))
        {
            // Allow others to access.
            __asm__ volatile(" nop; nop");
        }
    }

    // Restore IRQ.
    atomic_sub(1, &pOp->aOut);
    local_irq_restore(flags);
}

static inline void __HalFlushDCache(void)
{
    //ASSERT(!irqs_disabled() && "Calling HalFlushDCache() with IRQ disabled.");
    smpDoCacheOps(smpFlushDCache);
}

void HalFlushDCache(void)
{
    HalCacheFlushSizeRecord(__builtin_return_address(0), 1*1024*1024*1024);
    __HalFlushDCache();
}
#else
static void __HalFlushInvalidateDCache(void)
{
    unsigned long flags;

    local_irq_save(flags);
    _FlushDCache(0);
    _L2FlushInvalidateAll();
    _FlushInvalidateDCache(0);
    local_irq_restore(flags);
}

void HalFlushInvalidateDCache(void)
{
    HalCacheFlushSizeRecord(__builtin_return_address(0), 1*1024*1024*1024);
    __HalFlushInvalidateDCache();
}

static void __HalFlushDCache(void)
{
    unsigned long flags;

    local_irq_save(flags);
    _FlushDCache(0);
    _L2FlushAll();
    local_irq_restore(flags);
}

void HalFlushDCache(void)
{
    HalCacheFlushSizeRecord(__builtin_return_address(0), 1*1024*1024*1024);
    __HalFlushDCache();
}
#endif /* CONFIG_SMP */

//-----------------------------------------------------------------------------
/** HalInvalidateDCacheMultipleLine() Invalidate multiple line of D-cache
 *  @param u4Addr  [in] - The start address to be invalidated
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    HalCacheFlushSizeRecord(__builtin_return_address(0), u4Size);
    if (u4Size == 0)
    {
        return;
    }
    else
    {
        // Clean L2, for invalidate we should do L2 first
        UINT32 u4PA = __PHYSICAL(u4Addr);
        _L2InvalidateRange(u4PA, u4Size);

        // Clean ARM L1
        _ArmInvalidateDCacheRange(u4Addr, u4Size);

        // For CA9, we enabled exclusive cache, which need this extra
        // invalidate to make sure data are invalidated.
        _L2InvalidateRange(u4PA, u4Size);
    }
}

//-----------------------------------------------------------------------------
/** HalIOMMUInvalidateDCacheMultipleLine() Invalidate multiple line of D-cache
 *  @param u4VirAddr  [in] - The start virtual address to be invalidated
 *  @param u4PhyAddr  [in] - The start physical address to be invalidated
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalIOMMUInvalidateDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    if (u4Size == 0)
    {
        return;
    }
    else
    {
        // Clean L2, for invalidate we should do L2 first
        _L2InvalidateRange(u4PhyAddr, u4Size);

        // Clean ARM L1
        _ArmInvalidateDCacheRange(u4VirAddr, u4Size);

        // For CA9, we enabled exclusive cache, which need this extra
        // invalidate to make sure data are invalidated.
        _L2InvalidateRange(u4PhyAddr, u4Size);
    }
}

//-----------------------------------------------------------------------------
/** HalFlushDCacheMultipleLine() Flush (clean) multiple line of D-cache
 *  @param u4Addr  [in] - The start address to be flushed
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalFlushDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    HalCacheFlushSizeRecord(__builtin_return_address(0), u4Size);
    if (u4Size == 0)
    {
        return;
    }
    else if (u4Size > DCACHE_FLUSH_THRESHOLD && can_flush_all())
    {
        __HalFlushDCache();
    }
    else
    {
        // Clean ARM L1
        _ArmFlushDCacheRange(u4Addr, u4Size);

        // Clean L2
        u4Addr = __PHYSICAL(u4Addr);
        _L2FlushRange(u4Addr, u4Size);
    }
}

//-----------------------------------------------------------------------------
/** HalIOMMUFlushDCacheMultipleLine() Flush (clean) multiple line of D-cache
 *  @param u4VirAddr  [in] - The start virtual address to be flushed
 *  @param u4PhyAddr  [in] - The start physical address to be flushed
 *  @param u4Size     [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalIOMMUFlushDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    if (u4Size == 0)
    {
        return;
    }
    else if (u4Size > DCACHE_FLUSH_THRESHOLD && can_flush_all())
    {
        __HalFlushDCache();
    }
    else
    {
        // Clean ARM L1
        _ArmFlushDCacheRange(u4VirAddr, u4Size);

        // Clean L2
        _L2FlushRange(u4PhyAddr, u4Size);
    }
}

//-----------------------------------------------------------------------------
/** HalFlushInvalidateDCacheMultipleLine() Flush (clean) and invalidate multiple \n
 *                  line of D-cache
 *  @param u4Addr  [in] - The start address to be flushed
 *  @param u4Size  [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalFlushInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    HalCacheFlushSizeRecord(__builtin_return_address(0), u4Size);
    if (u4Size == 0)
    {
        return;
    }
    else if (u4Size > DCACHE_FLUSH_THRESHOLD && can_flush_all())
    {
        __HalFlushInvalidateDCache();
    }
    else
    {
        UINT32 u4PAddr;

        // Flush ARM L1
        _ArmFlushDCacheRange(u4Addr, u4Size);

        // FlushInvalidate L2
        u4PAddr = __PHYSICAL(u4Addr);
        _L2FlushInvalidateRange(u4PAddr, u4Size);

        // FlushInvalidate ARM L1
        _ArmFlushInvalidateDCacheRange(u4Addr, u4Size);
    }
}

//-----------------------------------------------------------------------------
/** HalIOMMUFlushInvalidateDCacheMultipleLine() Flush (clean) and invalidate multiple \n
 *                  line of D-cache
 *  @param u4VirAddr  [in] - The start virtual address to be flushed
 *  @param u4PhyAddr  [in] - The start physical address to be flushed
 *  @param u4Size     [in] - The memory size to be invalidated
 */
//-----------------------------------------------------------------------------
void HalIOMMUFlushInvalidateDCacheMultipleLine(UINT32 u4VirAddr, UINT32 u4PhyAddr, UINT32 u4Size)
{
    ASSERT(u4Size <= MAX_MULTILINE_SIZE && "Called with strange size");
    if (u4Size == 0)
    {
        return;
    }
    else if (u4Size > DCACHE_FLUSH_THRESHOLD && can_flush_all())
    {
        __HalFlushInvalidateDCache();
    }
    else
    {
        // Flush ARM L1
        _ArmFlushDCacheRange(u4VirAddr, u4Size);

        // FlushInvalidate L2
        _L2FlushInvalidateRange(u4PhyAddr, u4Size);

        // FlushInvalidate ARM L1
        _ArmFlushInvalidateDCacheRange(u4VirAddr, u4Size);
    }
}
