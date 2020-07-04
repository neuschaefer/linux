/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_hsan.h
  版 本 号   : 初稿
  生成日期   : D2014_03_11

******************************************************************************/
#ifndef __HI_HSAN_H__
#define __HI_HSAN_H__

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/err.h>
#include <linux/threads.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/irq.h>
#include <linux/cache.h>
#include <linux/profile.h>
#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/platform_device.h>
#include <linux/list.h>
#include <linux/dma-mapping.h>
#include <linux/timex.h>
#include <linux/signal.h>
#include <linux/clk.h>
#include <linux/gfp.h>
#include <linux/clockchips.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/mbus.h>
#include <linux/console.h>
#include <linux/serial_core.h>
#include <linux/amba/bus.h>
#include <linux/cnt32_to_63.h>
#include <linux/kthread.h>
#include <linux/tty_flip.h>
#include <linux/skbuff.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include <asm/mach/pci.h>
#include <asm/setup.h>
#include <asm/smp_plat.h>
#include <asm/mach-types.h>
#include <asm/types.h>


#include <asm/sizes.h>
#include <asm/ptrace.h>
#include <asm/atomic.h>
#include <asm/irq.h>
#include <asm/pgtable.h>
#include <asm/pgalloc.h>
#include <asm/mmu_context.h>
#include <asm/tlbflush.h>
#include <asm/smp_scu.h>
#include <asm/smp_twd.h>
//#include <asm/localtimer.h>
#include <asm/cacheflush.h>
#include <asm/clkdev.h>
#include <asm/div64.h>

#include <asm/hardware/gic.h>

#include <mach/clkdev.h>

#include <mach/io.h>
#include <mach/smp.h>
#include <mach/hi_typedef.h>
#include <mach/hi_cache.h>
#include <mach/hi_com.h>
#include <mach/hi_edb.h>
#include <mach/hi_map.h>
#include <mach/hi_pcie.h>
#include <mach/hi_serial.h>
#include <mach/hi_wdg.h>
#include <mach/hi_pcie.h>
#include <mach/hi_netmap.h>

#endif /* __HI_HSAN_H__ */
