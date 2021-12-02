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

/** @file hal_bench.h
 *  Benchmark function for HAL fetures.
 */

#ifndef __HAL_BENCH_H__
#define __HAL_BENCH_H__

/*  To enable the benchmark, uncomment the following macros and recompile.
    The report will be in /proc/hal_*

    Little awk script to convert output report to function/line-no.
    Please change the 'aaaddr2line' to addr2line program from toolchain.

    Run the script at command line, and paste the report then press ctrl-d.
    It will dump all 

gawk '{addr=sprintf("0x%x", strtonum($1)-0xbf02e000);   print addr }' |  \
  aaaddr2line -s -f -e mtk_mod.ko | gawk '{if(T) {printf("%s (%s)\n", T, \
  $1); T="";} else T=$1;}'
 */

// Benchmark critial sections length
//#define CC_ENABLE_CRITICAL_BENCH
// Benchmark cache flush size.
//#define CC_ENABLE_CACHE_FLUSH_BENCH


// Also report symbol.
// This need sprint_symbol() in chiling/kernel/linux-2.6.35/kernel/kallsyms.c
// However, it is EXPORT_SYMBOL_GPL and can only be used by GPL module, and
// we can't change that.
//#define LOOKUP_SYMBOL


#ifdef LOOKUP_SYMBOL
#define SYMBOL_REPORT_LENGTH "70"
#else
#define SYMBOL_REPORT_LENGTH "12"
#endif

#include <linux/proc_fs.h>
#include <linux/kallsyms.h>


extern struct proc_dir_entry proc_root;
#ifdef DO_HAL_BENCH_CRIT
#ifdef CC_ENABLE_CRITICAL_BENCH
#define CRITICAL_BENCH_SLOT   16
static int criticalBenchNum, criticalEnterTime;
static void *pLockHolder;
static struct timeval lockStartTime;
static struct lockHoldTime {
    void *pLockHolder;
    long holdTimeMicroSecond;
} lockStatisticsSlots[CRITICAL_BENCH_SLOT];

static void HalCriticalStartTimer(void *pRA)
{
    criticalEnterTime++;
    if (!pLockHolder)
    {
        pLockHolder = pRA;
        do_gettimeofday(&lockStartTime);
    }
}

static void HalCriticalEndTimer(void)
{
    struct timeval now;
    long holdTime;
    int i, insert = criticalBenchNum;

    do_gettimeofday(&now);
    holdTime = (now.tv_sec - lockStartTime.tv_sec) * 1000000 + 
               (now.tv_usec - lockStartTime.tv_usec);

    // Find previous log for same holder, also see where we should put this.
    for (i=0; i<criticalBenchNum; i++)
    {
        long slotHoldTime = lockStatisticsSlots[i].holdTimeMicroSecond;
        if (lockStatisticsSlots[i].pLockHolder == pLockHolder)
        {
            if (slotHoldTime < holdTime)
            {
                // Remove old statics.
                criticalBenchNum--;
                memcpy(&lockStatisticsSlots[i], &lockStatisticsSlots[i+1],
                        (criticalBenchNum - i) * sizeof(struct lockHoldTime));
                i--;
                continue;
            }
            else
            {
                // Already registered, ignore faster time
                holdTime = 0;
                break;
            }
        }
        if (insert > i && slotHoldTime < holdTime)
            insert = i;
    }

    if (holdTime && insert < CRITICAL_BENCH_SLOT)
    {
        if (criticalBenchNum < CRITICAL_BENCH_SLOT)
            criticalBenchNum++;
        memmove(&lockStatisticsSlots[insert+1], &lockStatisticsSlots[insert],
            (criticalBenchNum - insert - 1) * sizeof(struct lockHoldTime));
        lockStatisticsSlots[insert].pLockHolder = pLockHolder;
        lockStatisticsSlots[insert].holdTimeMicroSecond = holdTime;
    }

    pLockHolder=0;
}

static ssize_t _hal_critical_report_read(struct file *file, char __user *buf,
                                         size_t count, loff_t *ppos)
{
    unsigned char *tbuf;
    ssize_t ret, pos;
    int i;

    tbuf = kmalloc(4096, GFP_KERNEL|__GFP_REPEAT|__GFP_NOWARN);
    if (!tbuf)
        return 0;

    pos = sprintf(tbuf, "HalCriticalStart called %d\nTop %d critical holder:\n",
                  criticalEnterTime, criticalBenchNum);
    for (i=0; i<criticalBenchNum; i++)
    {
        char sym[KSYM_SYMBOL_LEN+40];

#ifdef LOOKUP_SYMBOL
        char sym1[KSYM_SYMBOL_LEN];

        sprint_symbol(sym1, (unsigned long)lockStatisticsSlots[i].pLockHolder);
        sprintf(sym, "0x%p (%s)", lockStatisticsSlots[i].pLockHolder, sym1);
#else
        sprintf(sym, "0x%p", lockStatisticsSlots[i].pLockHolder);
#endif
        pos += snprintf(tbuf+pos, 4096-pos, "  %-" SYMBOL_REPORT_LENGTH "s %ld\n",
                        sym,
                        lockStatisticsSlots[i].holdTimeMicroSecond);
    }

    ret = simple_read_from_buffer(buf, count, ppos, tbuf, pos);
    kfree(tbuf);
    return ret;
}

struct file_operations _hal_critical_report_operations =
{
    .read = _hal_critical_report_read,
};
#else
#define HalCriticalStartTimer(a)
#define HalCriticalEndTimer()
#endif /* CC_ENABLE_CRITICAL_BENCH */
#endif /* DO_HAL_BENCH_CRIT */


#ifdef DO_HAL_BENCH_CACHE
#ifdef CC_ENABLE_CACHE_FLUSH_BENCH
#define CACHE_BENCH_SLOT   32
static DEFINE_SPINLOCK(cacheBenchLock);
static int cacheBenchNum;
static long long cacheTotalSize;
static struct FlushSizeInfo
{
    void *pOwner;
    UINT32 uSize;
    long long llTotal;
} cacheBenchInfos[CACHE_BENCH_SLOT];

static void HalCacheFlushSizeRecord(void *pRA, UINT32 size)
{
    int i, insert = -1;
    long long total = size;
    unsigned long flags;

    spin_lock_irqsave(&cacheBenchLock, flags);
    size = (size + DCACHE_LINE_SIZE-1) & (~(DCACHE_LINE_SIZE-1));
    cacheTotalSize += size;

    for (i=0; i<cacheBenchNum; i++)
    {
        if (cacheBenchInfos[i].pOwner == pRA)
        {
            if (insert < 0)
            {
                // Just add size and return
                cacheBenchInfos[i].llTotal += size;
                size = 0;
            }
            else
            {
                // Remove this one.
                total += cacheBenchInfos[i].llTotal;
                cacheBenchNum--;
                memcpy(&cacheBenchInfos[i], &cacheBenchInfos[i+1],
                        (cacheBenchNum - i) * sizeof(struct FlushSizeInfo));
            }
            break;
        }

        if (insert < 0 && cacheBenchInfos[i].uSize < size)
            insert = i;
    }

    if (size && insert < 0 && cacheBenchNum < CACHE_BENCH_SLOT)
        insert = cacheBenchNum;

    if (insert >= 0)
    {
        if (cacheBenchNum < CACHE_BENCH_SLOT)
            cacheBenchNum++;
        memmove(&cacheBenchInfos[insert+1], &cacheBenchInfos[insert],
            (cacheBenchNum - insert - 1) * sizeof(struct FlushSizeInfo));
        cacheBenchInfos[insert].pOwner = pRA;
        cacheBenchInfos[insert].uSize = size;
        cacheBenchInfos[insert].llTotal = total;
    }
    spin_unlock_irqrestore(&cacheBenchLock, flags);
}

static ssize_t _hal_cache_report_read(struct file *file, char __user *buf,
                                      size_t count, loff_t *ppos)
{
    unsigned char *tbuf;
    ssize_t ret, pos;
    int i;
    unsigned long flags;

    tbuf = kmalloc(4096, GFP_KERNEL|__GFP_REPEAT|__GFP_NOWARN);
    if (!tbuf)
        return 0;

    spin_lock_irqsave(&cacheBenchLock, flags);
    pos = sprintf(tbuf, "CacheFlush total %lld\nTop %d caller:\n",
                  cacheTotalSize, cacheBenchNum);
    for (i=0; i<cacheBenchNum; i++)
    {
        char sym[KSYM_SYMBOL_LEN+40];

#ifdef LOOKUP_SYMBOL
        char sym1[KSYM_SYMBOL_LEN];

        sprint_symbol(sym1, (unsigned long)cacheBenchInfos[i].pOwner);
        sprintf(sym, "0x%p (%s)", cacheBenchInfos[i].pOwner, sym1);
#else
        sprintf(sym, "0x%p", cacheBenchInfos[i].pOwner);
#endif
        pos += snprintf(tbuf+pos, 4096-pos, "  %-" SYMBOL_REPORT_LENGTH "s %-16ld %-16lld\n",
                        sym,
                        cacheBenchInfos[i].uSize,
                        cacheBenchInfos[i].llTotal);
    }

    ret = simple_read_from_buffer(buf, count, ppos, tbuf, pos);
    kfree(tbuf);
    spin_unlock_irqrestore(&cacheBenchLock, flags);
    return ret;
}

struct file_operations _hal_cache_report_operations =
{
    .read = _hal_cache_report_read,
};
#else
static void HalCacheFlushSizeRecord(void *pRA, UINT32 size)
{
}
#endif /* CC_ENABLE_CACHE_FLUSH_BENCH */
#endif /* DO_HAL_BENCH_CACHE */


#ifdef DO_HAL_BENCH_INIT
extern struct file_operations _hal_critical_report_operations;
extern struct file_operations _hal_cache_report_operations;
static void _HalInitBenchmark(void)
{
    struct proc_dir_entry * proc_file __attribute__((unused)) = 0;

#ifdef CC_ENABLE_CRITICAL_BENCH
    proc_file = create_proc_entry("hal_critical", S_IFREG | S_IWUSR, NULL);
    if (proc_file)
    {
        proc_file->proc_fops = &_hal_critical_report_operations;
    }
#endif

#ifdef CC_ENABLE_CACHE_FLUSH_BENCH
    proc_file = create_proc_entry("hal_cache", S_IFREG | S_IWUSR, NULL);
    if (proc_file)
    {
        proc_file->proc_fops = &_hal_cache_report_operations;
    }
#endif
}
#endif /* DO_HAL_BENCH_INIT */

#endif /* __HAL_BENCH_H__ */

