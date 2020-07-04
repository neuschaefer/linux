/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_hotplug.c
  版 本 号   : 初稿
  生成日期   : D2014_03_10

******************************************************************************/

#include <mach/hi_hsan.h>

extern volatile hi_int32 g_i_pen_release;
extern hi_void hi_l2cache_clean_all(hi_void);

static DECLARE_COMPLETION(g_st_cpu_killed);

static inline hi_void cpu_enter_lowpower(hi_void)
{
	hi_uint32 ui_value;

	hi_l2cache_clean_all();
	asm volatile(
	"	mcr	p15, 0, %1, c7, c5, 0\n"
	"	mcr	p15, 0, %1, c7, c10, 4\n"
	/*
	 * Turn off coherency
	 */
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	bic	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	"	mrc	p15, 0, %0, c1, c0, 0\n"
	"	bic	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	  : "=&r" (ui_value)
	  : "r" (0)
	  : "cc");
}

static inline hi_void cpu_leave_lowpower(hi_void)
{
	hi_uint32 ui_value;

	asm volatile(	"mrc	p15, 0, %0, c1, c0, 0\n"
	"	orr	%0, %0, #0x04\n"
	"	mcr	p15, 0, %0, c1, c0, 0\n"
	"	mrc	p15, 0, %0, c1, c0, 1\n"
	"	orr	%0, %0, #0x20\n"
	"	mcr	p15, 0, %0, c1, c0, 1\n"
	  : "=&r" (ui_value)
	  :
	  : "cc");
}

static inline hi_void platform_do_lowpower(hi_uint32 ui_cpu)
{
	/*
	 * there is no power-control hardware on this platform, so all
	 * we can do is put the core into WFI; this is safe as the calling
	 * code will have already disabled interrupts
	 */
	for (;;) {
		/*
		 * here's the WFI
		 */
		asm(".word	0xe320f003\n"
		    :
		    :
		    : "memory", "cc");

		if (g_i_pen_release == ui_cpu) {
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}

		/*
		 * getting here, means that we have come out of WFI without
		 * having been woken up - this shouldn't happen
		 *
		 * The trouble is, letting people know about this is not really
		 * possible, since we are currently running incoherently, and
		 * therefore cannot safely call printk() or anything else
		 */
		hi_debug("CPU%u: spurious wakeup call\n", ui_cpu);
	}
}

hi_int32 platform_cpu_kill(hi_uint32 ui_cpu)
{
	return 1;
}

/*
 * platform-specific code to shutdown a CPU
 *
 * Called with IRQs disabled
 */
hi_void platform_cpu_die(hi_uint32 ui_cpu)
{
	hi_uint32 this_cpu = hard_smp_processor_id();

	if (ui_cpu != this_cpu) {
		hi_debug(KERN_CRIT "Eek! platform_cpu_die running on %u, should be %u\n",
			    this_cpu, ui_cpu);
		BUG();
	}

	hi_printk(KERN_NOTICE "CPU%u: shutdown\n", ui_cpu);
	complete(&g_st_cpu_killed);

	/*
	 * we're ready for shutdown now, so do it
	 */
	cpu_enter_lowpower();
	platform_do_lowpower(ui_cpu);

	/*
	 * bring this CPU back into the world of cache
	 * coherency, and then restore interrupts
	 */
	cpu_leave_lowpower();
}

hi_int32 platform_cpu_disable(hi_uint32 ui_cpu)
{
	/*
	 * we don't allow CPU 0 to be shutdown (it is still too special
	 * e.g. clock tick interrupts)
	 */
	return ui_cpu == 0 ? -EPERM : 0;
}

