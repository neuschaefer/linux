/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_mach.c
  版 本 号   : 初稿
  生成日期   : D2014_03_10

******************************************************************************/

#include <mach/hi_hsan.h>

hi_iomem *gic_cpu_base_addr = HI_NULL;
static hi_uint64 g_ull_timer_tsc = 0;

#ifdef __TIMER_SOURCE__
#endif


static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, HI_REG_BASE_A9_PERI+HI_REG_SC_LOCAL_TIMER, HI_IRQ_LOCALTIMER);

static hi_void __init hi_timer_twd_init(hi_void)
{
    hi_int32 i_err = twd_local_timer_register(&twd_local_timer);
    
    if (i_err)
    {
        pr_err("twd_local_timer_register failed %d\n", i_err);
    }

    hi_trace();
}

static irqreturn_t hi_timer_tsc_interrupt(hi_int32 i_irq, hi_void *pv_dev)
{
    hi_uint32  ui_value = 0;

    HI_REG_READ( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_RIS, ui_value );
    if( ui_value&0x1 )
    {
        g_ull_timer_tsc++;
        HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_INTCLR, ~0);
    }
    return IRQ_HANDLED;
}

static cycle_t hi_timer_source_get_cycles(struct clocksource *cs)
{
    hi_uint32 ui_val = 0;
    HI_REG_READ(HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_VALUE, ui_val);
    return ~ui_val;
}

static struct irqaction g_st_timer_tsc_irq =
{
    .name       = "tsc timer:1",
    .flags      = IRQF_SHARED | IRQF_TIMER,
    .handler    = hi_timer_tsc_interrupt,
};

static struct clocksource g_st_timer_source =
{
    .name       = "sourcetimer:1",
    .rating     = 200,
    .read       = hi_timer_source_get_cycles,
    .mask       = CLOCKSOURCE_MASK(32),
    .shift      = 20,
    .flags      = CLOCK_SOURCE_IS_CONTINUOUS,
};

static hi_void hi_timer_source_init(hi_void)
{
    // set timer1
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_CONTROL, 0                );
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_RELOAD , 0xFFFFFFFF       );
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_VALUE  , 0xFFFFFFFF       );
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_CONTROL, HI_TIMER_CONTROL );
	// reg source
	g_st_timer_source.mult = clocksource_khz2mult(HI_BUSCLK_TO_TIMER_CLK_KHZ, g_st_timer_source.shift);
	clocksource_register(&g_st_timer_source);
    setup_irq( HI_INTNR_TIMER_0_1, &g_st_timer_tsc_irq );

	hi_trace();
}

#ifdef __TIMER_COMP__
#endif

static irqreturn_t hi_timer_comp_interrupt(hi_int32 i_irq, hi_void *pv_dev_id)
{
    struct clock_event_device *pst_evt  = pv_dev_id;
    hi_uint32 ui_val = 0;

    HI_REG_READ(HI_CFG_TIMER0_VABASE+HI_REG_TIMER_RIS, ui_val);
    if ( ui_val & 0x1 )
    {
        HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_INTCLR, ~0 );
        pst_evt->event_handler(pst_evt);
    }
    return IRQ_HANDLED;
}

static hi_void hi_timer_comp_set_mode(enum clock_event_mode em_mode, struct clock_event_device *pst_evt)
{
    hi_uint32 ui_val    = 0;
    
    switch(em_mode)
    {
        case CLOCK_EVT_MODE_PERIODIC:
            HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, 0 );
            HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_RELOAD , HI_BUSCLK_TO_TIMER_RELOAD );
            HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, HI_TIMER_CONTROL );
            break;
        case CLOCK_EVT_MODE_ONESHOT:
            HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, (HI_TIMER_32BIT |HI_TIMER_ONESHOT) );
            break;
        case CLOCK_EVT_MODE_UNUSED:
        case CLOCK_EVT_MODE_SHUTDOWN:
        default:
            HI_REG_READ(  HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, ui_val );
            ui_val &= ~HI_TIMER_ENABLE;
            HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, ui_val );
            break;
    }
}

static hi_int32 hi_timer_comp_set_next_event( hi_ulong32 ul_evt, struct clock_event_device *pst_evt)
{
    hi_uint32 ui_val = 0;

    HI_REG_READ(  HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, ui_val );
    ui_val &=~(HI_TIMER_ENABLE | HI_TIMER_INTMASK);
    HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, ui_val );
    //
    HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_RELOAD, ul_evt );
    HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL, HI_TIMER_ONE_CONTROL );
    return 0;
}

static struct clock_event_device g_st_timer_comp_event =
{
    .name     = "comptimer0",
    .features = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.rating	  = 250,
    .shift    = 32,
};

static struct irqaction g_st_timer_comp_irq =
{
    .name     = "comptimer:0",
    .flags    = IRQF_SHARED | IRQF_DISABLED | IRQF_TIMER,
    .handler  = hi_timer_comp_interrupt,
    .dev_id   = &g_st_timer_comp_event,
};

static hi_void hi_timer_comp_init(hi_void)
{
    // event
    g_st_timer_comp_event.set_mode       = hi_timer_comp_set_mode;
    g_st_timer_comp_event.set_next_event = hi_timer_comp_set_next_event;

    g_st_timer_comp_event.mult           = div_sc(HI_BUSCLK_TO_TIMER_CLK_KHZ, NSEC_PER_SEC, g_st_timer_comp_event.shift);
    g_st_timer_comp_event.max_delta_ns   = clockevent_delta2ns(0xFFFFFFFF, &g_st_timer_comp_event);
    g_st_timer_comp_event.min_delta_ns   = clockevent_delta2ns(0xF, &g_st_timer_comp_event);
    g_st_timer_comp_event.cpumask        = cpumask_of(0);

    clockevents_register_device(&g_st_timer_comp_event);
    setup_irq( HI_INTNR_TIMER_0_1, &g_st_timer_comp_irq );
   
    hi_trace();
}

#ifdef __ARCH_MACH__
#endif

static hi_void hi_local_timer_init(hi_void)
{
    hi_uint32 ui_value = 0;
    
    /* enable of timer 0 timer 1 timer 2 */
    HI_REG_READ(  hi_io_address(HI_REG_BASE_SCTL), ui_value);
	HI_REG_WRITE( hi_io_address(HI_REG_BASE_SCTL), ui_value | HI_TIMER_CLK_SOURCE );
	
    /* clear all */
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER_CONTROL , 0 );
	HI_REG_WRITE( HI_CFG_TIMER0_VABASE + HI_REG_TIMER1_CONTROL, 0 );
	HI_REG_WRITE( HI_CFG_TIMER2_VABASE + HI_REG_TIMER_CONTROL , 0 );
	HI_REG_WRITE( HI_CFG_TIMER2_VABASE + HI_REG_TIMER1_CONTROL, 0 );

    /* source timer init */
    hi_timer_source_init();
    
    /* comp timer init */
    hi_timer_comp_init();
    
#ifdef CONFIG_SMP
    /* setup twd local timer */
    hi_timer_twd_init();
#endif

    hi_printk("\nHSAN:apb bus clk is %u",hi_get_apbclk());
}

static struct sys_timer g_st_sys_timer =
{
    .init = hi_local_timer_init,
};

static hi_void hi_machine_init(hi_void)
{
    hi_acp_init();
    hi_trace();

    hi_l2cache_init();
    hi_trace();
}

static hi_void hi_gic_init_irq(hi_void)
{
    //gic_cpu_base_addr = (hi_iomem *)HI_GIC_CPU_BASE;
    gic_init(0, HI_IRQ_LOCALTIMER, HI_GIC_DIST_BASE, HI_GIC_CPU_BASE);
    
    hi_trace();
}

static struct map_desc g_st_map_io_desc[] __initdata = {
    { 
        .virtual    =  hi_io_address(HI_REG_BASE_SCTL),
        .pfn        = __phys_to_pfn(HI_REG_BASE_SCTL),
        .length     = HI_REG_IOSIZE_SCTL,
        .type       = MT_DEVICE
    },{
       .virtual     =  hi_io_address(HI_REG_BASE_UART0),
        .pfn        = __phys_to_pfn(HI_REG_BASE_UART0),
        .length     = HI_REG_IOSIZE_UART0,
        .type       = MT_DEVICE
    },{
       .virtual     =  hi_io_address(HI_REG_BASE_UART1),
        .pfn        = __phys_to_pfn(HI_REG_BASE_UART1),
        .length     = HI_REG_IOSIZE_UART1,
        .type       = MT_DEVICE
    },{
       .virtual     =  hi_io_address(HI_REG_BASE_TIMER01),
        .pfn        = __phys_to_pfn(HI_REG_BASE_TIMER01),
        .length     =  HI_REG_IOSIZE_TIMER01,
        .type       = MT_DEVICE
    },{
       .virtual     =  hi_io_address(HI_REG_BASE_TIMER23),
        .pfn        = __phys_to_pfn(HI_REG_BASE_TIMER23),
        .length     =  HI_REG_IOSIZE_TIMER23,
        .type       = MT_DEVICE
    },{
       .virtual     =  hi_io_address(HI_REG_BASE_A9_PERI),
        .pfn        = __phys_to_pfn(HI_REG_BASE_A9_PERI),
        .length     =  HI_REG_IOSIZE_A9_PERI,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_L2CACHE),
        .pfn        = __phys_to_pfn(HI_REG_BASE_L2CACHE),
        .length     = HI_REG_IOSIZE_L2CACHE,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_CRG),
        .pfn        = __phys_to_pfn(HI_REG_BASE_CRG),
        .length     =  HI_REG_IOSIZE_CRG,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_IOMUX),
        .pfn        = __phys_to_pfn(HI_REG_BASE_IOMUX),
        .length     =  HI_REG_IOSIZE_MUX,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO0),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO0),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO1),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO1),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO2),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO2),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO3),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO3),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO4),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO4),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO5),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO5),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO6),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO6),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },{
        .virtual    =  hi_io_address(HI_REG_BASE_GPIO7),
        .pfn        = __phys_to_pfn(HI_REG_BASE_GPIO7),
        .length     =  HI_REG_IOSIZE_GPIO,
        .type       = MT_DEVICE
    },
};

static hi_void hi_map_io(hi_void)
{
    iotable_init(g_st_map_io_desc, ARRAY_SIZE(g_st_map_io_desc));
    hi_printk("\nHSAN:mapping io");
}

MACHINE_START(HSAN, "hsan")
    .atag_offset    = 0x100,
    .map_io         = hi_map_io,
    .init_irq       = hi_gic_init_irq,
    .timer          = &g_st_sys_timer,
    .handle_irq     = gic_handle_irq,
    .init_machine   = hi_machine_init,
    .restart        = arch_reset,
MACHINE_END

