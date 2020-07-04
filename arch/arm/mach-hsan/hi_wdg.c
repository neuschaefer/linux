/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  文件名称: watchdog.c
  功能描述: 硬件看门狗驱动
  版本描述: V1.0

  创建日期: D2011_09_30

  修改记录: 
            生成初稿.
******************************************************************************/

#include <mach/hi_hsan.h>

static hi_crg_reg_s *g_pst_crg_reg = HI_NULL;
static DEFINE_PER_CPU( hi_uint32, hi_last_feed_time);
static hi_uint32 g_ui_wdg_en = HI_ENABLE;
static hi_uint32 g_ui_wdg_init = HI_DISABLE;

static hi_void hi_wdg_en(hi_uint32 ui_en)
{
    if ( 0 != ui_en )
    {
        // enable
        HI_REG_WRITE(&g_pst_crg_reg->ui_crg_sc_perctrl8, 0x4F4E5452);
        HI_REG_WRITE(&g_pst_crg_reg->ui_crg_sc_perctrl8, 0x12345610);
    }
    else
    {
        // disable
        HI_REG_WRITE(&g_pst_crg_reg->ui_crg_sc_perctrl8, 0xABCD5610);
        HI_REG_WRITE(&g_pst_crg_reg->ui_crg_sc_perctrl8, 0xED574447);
    }
}

static hi_void hi_wdg_clear(hi_void)
{
    HI_REG_WRITE(&g_pst_crg_reg->ui_crg_wdg_init_cfg, 0x55AA5A5A);
    HI_REG_WRITE(&g_pst_crg_reg->ui_crg_wdg_init_cfg, 0xAA55A5A5);
}

static hi_void hi_wdg_set_reset_time(hi_uint32 ui_time)
{
    hi_crg_sc_perctrl4_u un_reg_crg_sc_perctrl4;

    /*检查ui_time范围*/
    if ((ui_time < HI_WDG_MIN_INTERVAL) || (ui_time > HI_WDG_MAX_INTERVAL))
    {
        ui_time = HI_WDG_DFT_RST_TIME;
    }

    HI_REG_READ (&g_pst_crg_reg->ui_crg_sc_perctrl4, un_reg_crg_sc_perctrl4.ui_value);
    un_reg_crg_sc_perctrl4.st_bits.ui_wdg_rst_time_cfg = ui_time-1;
    
    HI_REG_WRITE(&g_pst_crg_reg->ui_crg_sc_perctrl4, un_reg_crg_sc_perctrl4.ui_value);
}

#ifdef __WDG__
#endif

typedef void (*watchdog_reboot_callback)(void);

watchdog_reboot_callback g_pv_watchdog_reboot_callback = NULL;

void watchdog_reboot_reg(watchdog_reboot_callback pv_callback)
{
    g_pv_watchdog_reboot_callback = pv_callback;

    printk("REGISTER g_pv_watchdog_reboot_callback.\n");
}

EXPORT_SYMBOL(watchdog_reboot_reg);

// by cpu hard interrupt to call
hi_void hi_wdg_check(hi_void)
{
    hi_uint32 ui_inter   = 0;
    hi_uint32 ui_cpu     = 0;
    hi_uchar8 uc_cmd[64] = {0};

    if ( unlikely((HI_ENABLE != g_ui_wdg_en) || (HI_ENABLE != g_ui_wdg_init) ) )
    {
        return;
    }
    
    // check per monitor    
    for ( ui_cpu=0; ui_cpu<NR_CPUS; ui_cpu++ )
    {
        ui_inter = per_cpu(hi_last_feed_time,ui_cpu);
        // early 
        if ( unlikely(0 == ui_inter) )
        {
            return;
        }
        if ( (jiffies - ui_inter) > HI_WDG_CHECK_MAX_TIME )
        {			
			if (g_pv_watchdog_reboot_callback != NULL)
            {
				g_pv_watchdog_reboot_callback();
            }
			
            g_ui_wdg_en = HI_DISABLE;
            smp_wmb();
            hi_dumpall();
            hi_sprintf(uc_cmd,"cpu:%u watchdog timeout=%u0ms",ui_cpu,(hi_uint32)(jiffies - ui_inter));
            arch_reset(0,uc_cmd);
            return;
        }
    }
    return;
}

static hi_int32 hi_wdg_task_entry( hi_void *pv_data )
{
    hi_ulong32 ul_timeout = (HI_WDG_DFT_FEED_TIME*HZ);
    hi_uint32 *pui_stamp  = &__get_cpu_var(hi_last_feed_time);
    hi_uint32  ui_cpu     = smp_processor_id();
    
    while( HI_ENABLE )
    {  
        // check time
        if ( (jiffies - *pui_stamp) >= ul_timeout )
        {
            // only monitor cpu to clear hard wdg tick
            if ( ( HI_WDG_MONITOR_CPUID == ui_cpu ) && ( HI_ENABLE == g_ui_wdg_en ))
            {
                hi_wdg_clear();
            }
            // refresh feed time
            *pui_stamp = jiffies;
            smp_wmb();
        }
        // go to wait
        set_current_state(TASK_INTERRUPTIBLE);  
        schedule_timeout(ul_timeout);
    }
    return 0;
}

#ifdef __WDG_INIT__
#endif

hi_int32 hi_wdg_init(hi_void)
{
    hi_uint32           ui_cpu   = 0;
    struct task_struct *pst_task = HI_NULL;

    /*物理地址映射到虚拟内存*/
    g_pst_crg_reg = (hi_crg_reg_s*)hi_io_address(HI_REG_BASE_CRG);

    if ( HI_ENABLE != g_ui_wdg_en )
    {
        hi_wdg_en(HI_DISABLE);
        hi_printk("\nkernel watchdog disable");
        return 0;
    }
    // set wdg
    hi_wdg_en(HI_ENABLE);
    hi_wdg_set_reset_time(HI_WDG_DFT_RST_TIME);
    hi_wdg_clear();

    // kthread monitor
    for ( ui_cpu=0; ui_cpu<NR_CPUS; ui_cpu++ )
    {
        // init
        per_cpu(hi_last_feed_time,ui_cpu) = jiffies;
        pst_task = kthread_create(hi_wdg_task_entry,HI_NULL,"cpu%d:watchdog",ui_cpu); 
        if ( IS_ERR(pst_task) )
        {
            hi_printk("\nkernel wdtchdog init fail!!!");
            return -1;
        }
        kthread_bind(pst_task,ui_cpu);
        wake_up_process(pst_task);
    }
    g_ui_wdg_init = HI_ENABLE;
    hi_printk("\nkernel watchdog enable");
    return 0;
}

late_initcall(hi_wdg_init);

#ifdef __WDG_CMDLINE__
#endif

static hi_int32 hi_wdg_option( hi_char8 *pc_str )
{
    get_option(&pc_str, &g_ui_wdg_en);
    return HI_ENABLE;
}
early_param("wdg", hi_wdg_option);


