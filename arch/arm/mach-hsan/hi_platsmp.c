/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: smp.c
  功能描述: SMP多核管理接口
  版本描述: V1.0

  创建日期: D2013_10_25

  修改记录: 
            生成初稿.
******************************************************************************/

#include <mach/hi_hsan.h>

extern hi_int32 hi_secondary_startup(hi_void);

volatile hi_int32 g_i_pen_release = -1;

static hi_iomem *hi_scu_base_addr(hi_void)
{
    return (hi_iomem*)(hi_io_address(HI_REG_BASE_A9_PERI + HI_REG_A9_PERI_SCU));
}

static hi_void __cpuinit hi_write_pen_release(hi_int32 i_val)
{
	g_i_pen_release = i_val;
	smp_wmb();

	__cpuc_flush_dcache_area((hi_void *)&g_i_pen_release, sizeof(g_i_pen_release));
	outer_clean_range(__pa(&g_i_pen_release), __pa(&g_i_pen_release + 1));
}

static DEFINE_SPINLOCK(g_st_boot_lock);

/******************************************************************************
  函数功能:  Perform platform specific initialisation of the specified CPU.
  输入参数:  ui_cpu : CPU号
  输出参数:  无
  函数返回:  
  函数备注:  
******************************************************************************/
hi_void platform_secondary_init(hi_uint32 ui_cpu)
{
    hi_debug("cpu:%d pen_release:%d",ui_cpu,g_i_pen_release);

    trace_hardirqs_off();

    /*
     * if any interrupts are already enabled for the primary
     * core (e.g. timer irq), then they will not have been enabled
     * for us: do so
     */
    gic_secondary_init(0);
    /*
     * let the primary processor know we're out of the
     * pen, then head off into the C entry point
     */
    hi_write_pen_release(-1);

    /*
     * Synchronise with the boot thread.
     */
    spin_lock(&g_st_boot_lock);
    spin_unlock(&g_st_boot_lock);
}

hi_int32 boot_secondary(hi_uint32 ui_cpu, struct task_struct *pst_idle)
{
    hi_ulong32 ul_timeout = 0;

    hi_debug("cpu:%d pen_release:%d\n",ui_cpu,g_i_pen_release);


	/*
	 * Set synchronisation state between this boot processor
	 * and the secondary one
	 */
    spin_lock(&g_st_boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting g_i_pen_release.
	 *
	 * Note that "g_i_pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
    hi_write_pen_release(cpu_logical_map(ui_cpu));

	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */
	smp_cross_call(cpumask_of(ui_cpu));
	
	ul_timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, ul_timeout)) {
		smp_rmb();
		if (g_i_pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&g_st_boot_lock);

    hi_debug("cpu:%d pen_release:%d",ui_cpu,g_i_pen_release);

	return g_i_pen_release != -1 ? -ENOSYS : 0;
}

/******************************************************************************
  函数功能:  Initialise the CPU possible map early - this describes the CPUs
                             which may be present or become present in the system
  输入参数:  无
  输出参数:  无
  函数返回:  
  函数备注:  
******************************************************************************/
hi_void smp_init_cpus(hi_void)
{
	hi_iomem *scu_base  = hi_scu_base_addr();
	hi_uint32 i         = 0;
	hi_uint32 ui_ncores = 0;

	ui_ncores = scu_base ? scu_get_core_count(scu_base) : 1;

	/* sanity check */
	if (ui_ncores == 0) {
		hi_printk(KERN_ERR
		       "hsan: strange CM count of 0? Default to 1\n");

		ui_ncores = 1;
        hi_trace();
	}

	if (ui_ncores > NR_CPUS) {
		printk(KERN_WARNING
		       "hsan: no. of cores (%d) greater than configured "
		       "maximum of %d - clipping\n",
		       ui_ncores, NR_CPUS);
		ui_ncores = NR_CPUS;
        hi_trace();
	}

	for (i = 0; i < ui_ncores; i++)
	{
		set_cpu_possible(i, true);
    }

    set_smp_cross_call(gic_raise_softirq);
}

/******************************************************************************
  函数功能:  Prepare machine for booting other CPUs
  输入参数:  无
  输出参数:  无
  函数返回:  
  函数备注:  
******************************************************************************/
hi_void platform_smp_prepare_cpus(hi_uint32 ui_max_cpus)
{
    scu_enable(hi_scu_base_addr());

    /* Write the address of secondary startup into the system-wide flags register. The BootMonitor waits
            until it receives a soft interrupt, and then the secondary CPU branches to this address. */
    HI_REG_WRITE( hi_io_address(HI_REG_BASE_SCTL+ HI_REG_SC_SC_TESTREG1), virt_to_phys(hi_secondary_startup));

    hi_trace();
}


