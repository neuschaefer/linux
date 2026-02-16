/*
 * puma6_core.c
 * Description:
 * Architecture specific stuff.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2013-2014 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#ifdef CONFIG_HIGH_RES_TIMERS
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#endif
#include <linux/irq.h>
#include <asm/setup.h>
#include <asm/param.h>
#include <asm/mach-types.h>
#include <asm/io.h>
#include <asm/mach/map.h>
#include <asm/mach/arch.h>
#include <asm/mach/time.h>
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm-arm/arch-avalanche/puma6/puma6_cru_ctrl.h>  /* For CRU IDs */

void arch_init_irq(void);
extern void create_mapping(struct map_desc *md);
extern int puma_avalanche_setup(void);

static unsigned int no_linux_mem_size  = CONFIG_ARM_AVALANCHE_TOP_MEM_RESERVED;
static unsigned int no_linux_mem_last;
static unsigned int no_linux_mem_start;
static int          no_linux_mem_desc_idx = -1;

struct NO_OPERSYS_MEM_DESC_T no_OperSys_memory_desc[eNO_OperSys_END];

#ifdef CONFIG_HIGH_RES_TIMERS
static void config_timer2(u32);
#else
/* For VeryPrecise clock */
static int SC_timer_clk;
unsigned long long sc_calibrate_jf = 0;     /* Jiffies calibration */
unsigned long long frc2correct_load = 0;
static INT32 sc_timer_err_nsec;                  /* HW Timer error */
static void (*late_time_init_chain)(void) = NULL;

/* For Precise clock */
UINT32 sc_count_nsec = 0;
#endif

static struct map_desc puma6_io_desc[]  = {
#if CONFIG_ARM_AVALANCHE_TOP_MEM_RESERVED
    /* Note: This memory descriptor is set to be the FIRST intentionally !!! */
    /*       Do not change it's location !!!                                 */
    {
        .virtual    = NON_OS_RESERVED_VIRT,
        .pfn        = 0, // to be updated later
        .length     = 0, // to be updated later
        .type       = MT_MEMORY
    },
#endif
    {
        .virtual    = IO_VIRT,
        .pfn        = __phys_to_pfn(IO_PHY),
        .length     = IO_SIZE,
        .type       = MT_DEVICE
    },
    {
        .virtual    = MM_SPI_VIRT,
        .pfn        = __phys_to_pfn(MM_SPI_PHY),
        .length     = MM_SPI_SIZE,
        .type       = MT_DEVICE,
    },
    {
        .virtual    = SOC_IO_VIRT,
        .pfn        = __phys_to_pfn(SOC_IO_PHY),
        .length     = SOC_IO_SIZE,
        .type       = MT_DEVICE
    },
    {
        .virtual    = INTC_VIRT,
        .pfn        = __phys_to_pfn(INTC_PHY),
        .length     = INTC_SIZE,
        .type       =MT_DEVICE_NONSHARED
    },
};

static void __init puma6_map_io(void)
{
#if CONFIG_ARM_AVALANCHE_TOP_MEM_RESERVED
    no_linux_mem_desc_idx = 0;
    puma6_io_desc[ no_linux_mem_desc_idx ].pfn    = __phys_to_pfn(no_linux_mem_start);
    puma6_io_desc[ no_linux_mem_desc_idx ].length = no_linux_mem_size;
#endif
    iotable_init(puma6_io_desc, ARRAY_SIZE(puma6_io_desc));
    printk("Reserved %dk DSP memory starting from Physical 0x%p\n",
            (int)(no_linux_mem_size/1024),
            (void*)(no_linux_mem_start));
}


#define TIMER16_CNTRL_PRESCALE_ENABLE       0x8000
#define TIMER16_CNTRL_PRESCALE              0x003C
#define TIMER16_CNTRL_MODE                  0x0002

#define TIMER16_MINPRESCALE                 2
#define TIMER16_MAXPRESCALE                 8192
#define TIMER16_PRESCALE_DEFAULT       0x05
#define TIMER16_MIN_LOAD_VALUE              1
#define TIMER16_MAX_LOAD_VALUE              0xFFFF

#define MHZ                                 1000000

/* set min clock divisor to a little higher value
 * so that we are not close to the edge.
 * so multiply by factor 2
 */
#define TIMER16_MAX_CLK_DIVISOR (TIMER16_MAX_LOAD_VALUE * TIMER16_MAXPRESCALE)
#define TIMER16_MIN_CLK_DIVISOR (TIMER16_MIN_LOAD_VALUE * TIMER16_MINPRESCALE * 2)

typedef struct {
                        u32 ctrl_reg;   /* Timer Control Register */
                        u32 load_reg;   /* Timer Load value register */
                        u32 count_reg;  /* Timer count register */
                        u32 intr_reg;   /* Timer Interrupt register */

} puma_timer_regs_t;

typedef enum
{
    TIMER16_MODE_ONESHOT  = 0,
    TIMER16_MODE_AUTOLOAD = 2
} puma_timer_mode;

typedef enum
{
    TIMER16_STATUS_STOP = 0,
    TIMER16_STATUS_START
} puma_timer_status;


#ifdef CONFIG_HIGH_RES_TIMERS
/****************************************************************************
 * FUNCTION: puma_config_timer
 ****************************************************************************
 * Description: The routine is called to configure the timer mode and
 *              time period (in micro seconds).
 ***************************************************************************/
int puma_config_timer(u32 base_address,u32 refclk_freq,
                        puma_timer_mode mode, u32 usec)
{
    volatile puma_timer_regs_t *p_timer;
    u32 prescale;
    u32 count;
    u32 refclk_mhz = (refclk_freq / MHZ);

    if ((base_address == 0) || (usec == 0))
        return -1;

    if ((mode != TIMER16_CNTRL_ONESHOT) && (mode != TIMER16_CNTRL_AUTOLOAD))
        return -1;

    /* The min time period is 1 usec and since the reference clock freq is always going
        to be more than "min" divider value, minimum value is not checked.
        Check the max time period that can be derived from the timer in micro-seconds
    */
    if (usec > ((TIMER16_MAX_CLK_DIVISOR) / refclk_mhz)) {
        return -1;      /* input argument speed, out of range */
    }

    p_timer = (puma_timer_regs_t *) (base_address);
    count = refclk_mhz * usec;

    for (prescale = 0; prescale < 12; prescale++) {
        count = count >> 1;
        if (count <= TIMER16_MAX_LOAD_VALUE) {
                break;
        }
    }

    printk("load_value = %d prescale = %d\n", count, prescale);
    /*write the load counter value */
    p_timer->load_reg = count;

    /* write prescalar and mode to control reg */
    p_timer->ctrl_reg =
    mode | TIMER16_CNTRL_PRESCALE_ENABLE | (prescale << 2);

    return 0;
}

static void config_timer2(u32 base_address)
{
   volatile puma_timer_regs_t *p_timer;

   p_timer = (puma_timer_regs_t *) (base_address);

   /*write the load counter value */
   p_timer->load_reg = TIMER16_MAX_LOAD_VALUE;

   /* write prescalar and mode to control reg */
   p_timer->ctrl_reg = TIMER16_CNTRL_AUTOLOAD
           | TIMER16_CNTRL_PRESCALE_ENABLE |(TIMER16_PRESCALE_DEFAULT << 2);
}

/****************************************************************************
 * FUNCTION: puma_timer_ctrl
 ****************************************************************************
 * Description: The routine is called to start/stop the timer
 *
 ***************************************************************************/
void puma_timer_ctrl(u32 base_address, puma_timer_status status)
{
    volatile puma_timer_regs_t *p_timer;

    if (base_address) {
        p_timer = (puma_timer_regs_t *) (base_address);

        if (status == TIMER16_CTRL_START) {
            p_timer->ctrl_reg |= TIMER16_CTRL_START;
        } else {
            p_timer->ctrl_reg &= ~(TIMER16_CTRL_START);
        }
    }

}

/****************************************************************************
 * FUNCTION: puma_timer_read
 ****************************************************************************
 * Description: The routine is called to read the current value of timer.
 *
 ***************************************************************************/

static cycle_t timer_read(void)
{
   volatile puma_timer_regs_t *p_timer;
   u32 timer_value;

   p_timer = (puma_timer_regs_t *)(AVALANCHE_TIMER2_BASE);
   timer_value = (TIMER16_MAX_LOAD_VALUE - (p_timer->count_reg &0xffff));

   return (cycles_t)timer_value;
}


/*
 * clocksource
 */
static struct clocksource clocksource_puma = {
        .name           = "timer16bit",
        .rating         = 300,
        .read           = timer_read,
        .mask           = CLOCKSOURCE_MASK(16),
        .shift          = 16,
        .is_continuous  = 1,
};

static void puma_timer_set_next_event(unsigned long cycles,
                                    struct clock_event_device *evt)
{
   volatile puma_timer_regs_t *p_timer;
   p_timer = (puma_timer_regs_t *)(AVALANCHE_TIMER0_BASE);

   /* First stop the timer */
   p_timer->ctrl_reg &= ~(TIMER16_CTRL_START);
   /* Load the value being passed */
        p_timer->load_reg = cycles;

   /* Now start the timer */
   p_timer->ctrl_reg |= TIMER16_CTRL_START;
}

static void puma_timer_set_mode(enum clock_event_mode mode,
                              struct clock_event_device *evt)
{
   volatile puma_timer_regs_t *p_timer;
   p_timer = (puma_timer_regs_t *)(AVALANCHE_TIMER0_BASE);

   switch (mode) {
        case CLOCK_EVT_PERIODIC:
            /* write mode to control reg */
            p_timer->ctrl_reg |= TIMER16_CNTRL_AUTOLOAD;
            break;
        case CLOCK_EVT_ONESHOT:
            /* write mode to control reg */
            p_timer->ctrl_reg &= ~(TIMER16_CNTRL_AUTOLOAD);
            break;
        case CLOCK_EVT_SHUTDOWN:
            /* stop the timer */
            p_timer->ctrl_reg &= ~(TIMER16_CTRL_START);
            break;
   }
}

/*
 * clockevent
 */
static struct clock_event_device clockevent_puma = {
        .name           = "timer16bit",
        .capabilities   = CLOCK_CAP_NEXTEVT | CLOCK_CAP_TICK |
                          CLOCK_CAP_UPDATE,
        .shift          = 32,
        .set_next_event = puma_timer_set_next_event,
        .set_mode       = puma_timer_set_mode,
        .event_handler  = NULL,
};
#endif

#ifdef CONFIG_HIGH_RES_TIMERS
static irqreturn_t puma_timer0_interrupt(int irq, void *dev_id,
                                            struct pt_regs *regs)
{

        clockevent_puma.event_handler(regs);
        return IRQ_HANDLED;
}
#else
static irqreturn_t
puma_timer0_interrupt(int irq, void *dev_id)
{
    timer_tick();
    return IRQ_HANDLED;
}
#endif

static struct irqaction puma_timer0_irq = {
   .name = "Puma6 Timer Tick",
        .flags          = IRQF_DISABLED | IRQF_TIMER,
        .handler        = puma_timer0_interrupt
};


#ifdef CONFIG_HIGH_RES_TIMERS
/*
 * Set up timer interrupt, and return the current time in seconds.
 */
static void __init puma6_timer_init(void)
{
   int timer_clk;
   int scale;

   /* get the input clock frequency */
   timer_clk = PAL_sysClkcGetFreq(PAL_SYS_CLKC_TIMER0);

   /* timer 1 - enable timer and auto load, and go off every 1 ms */
   PAL_sysResetCtrl(AVALANCHE_TIMER0_RESET, OUT_OF_RESET);
   puma_config_timer(AVALANCHE_TIMER0_BASE, timer_clk,
               TIMER16_CNTRL_AUTOLOAD,
               (int)((1.0 / (float)(HZ)) * 1000000.0));
   puma_timer_ctrl(AVALANCHE_TIMER0_BASE, TIMER16_CTRL_START);
   setup_irq(AVALANCHE_TIMER_0_INT, &puma_timer0_irq);

   PAL_sysResetCtrl(AVALANCHE_TIMER2_RESET, OUT_OF_RESET);
   config_timer2(AVALANCHE_TIMER2_BASE);
   puma_timer_ctrl(AVALANCHE_TIMER2_BASE, TIMER16_CTRL_START);

   /* Get the scaler value for dividing timer frequency */
   scale = (2*(1<<TIMER16_PRESCALE_DEFAULT));

        /* setup clocksource */
        clocksource_puma.mult = clocksource_hz2mult(timer_clk/scale,
                                     clocksource_puma.shift);
   printk("Mult = %u\n", clocksource_puma.mult);
   if (clocksource_register(&clocksource_puma))
        printk(KERN_ERR "%s: can't register clocksource!\n",
           clocksource_puma.name);

   /* setup clockevent */
   clockevent_puma.mult = div_sc(timer_clk/scale, NSEC_PER_SEC,
                                         clockevent_puma.shift);
   clockevent_puma.max_delta_ns =
            clockevent_delta2ns(0xfffe, &clockevent_puma);
   clockevent_puma.min_delta_ns =
           clockevent_delta2ns(1, &clockevent_puma);

   register_global_clockevent(&clockevent_puma);

   printk("Puma6 Timer0 & Timer2 initialized\n");
}
#else
/* For VeryPrecise clock */
static int FRC_init = 0;
extern void (*__initdata late_time_init)(void);
static void late_time_init_f(void)
{
    unsigned long long timer0_phase;
    unsigned long flags;
    unsigned long frc_hi;
    unsigned long frc_lo;

    printk("INITIAL_JIFFIES %lu\n", INITIAL_JIFFIES);
    printk("jiffies %lu\n", jiffies);

    /* Sync the tick timer to the FRC */

    local_irq_save(flags);  

    do
    {
        timer0_phase = PAL_sysTimer16GetNsAfterTick(AVALANCHE_TIMER0_BASE, sc_count_nsec);
    } while (timer0_phase > sc_count_nsec); /* Exit close to the reload, there is a whole tick to achieve this */
    /* Now timer0 is around the reload area */
    /* Now start FRC */
    /* Reset the free-running clock */
    FREE_RUNNING_COUNTER_ENABLE();
    FREE_RUNNING_COUNTER_RESET();

    frc_hi = FREE_RUNNING_COUNTER_H_GET();
    frc_lo = FREE_RUNNING_COUNTER_L_GET();

    local_irq_restore(flags);

    printk("Sync FRC (hi %lu lo %lu) to timer0 (%llu)\n", frc_hi, frc_lo, timer0_phase);

    /* Calc frc2correct_load: recalc when total err > 100 uSec */
    /* err per tick * num ticks = 100Usec
            nticks = 100 uSec / sc_timer_err_nsec
            nticks = 100000 / sc_timer_err_nsec
            frc = nticks * 10mSec * frc timer_clk
            frc = 100000 / sc_timer_err_nsec * 0.01 * frc timer_clk
            frc = 100000 / HZ * frc timer_clk / sc_timer_err_nsec
       */
    /* Since we requested a negative err, make the err positive */
    sc_timer_err_nsec = -sc_timer_err_nsec;
    /* Change the timer_clk to frc freq */
    SC_timer_clk *= 4;
    frc2correct_load = 100000 / HZ;
    frc2correct_load *= SC_timer_clk;
    frc2correct_load += sc_timer_err_nsec/2;
    do_div(frc2correct_load, sc_timer_err_nsec);
    //printk("sc_timer_err_nsec %d, frc2correct_load %llu\n", sc_timer_err_nsec, frc2correct_load);

    FRC_init = 1;

    if (late_time_init_chain != NULL)
    {
        late_time_init_chain();
    }
}

static void __init puma6_timer_init(void)
{
    /* get the input clock frequency */
    SC_timer_clk = PAL_sysClkcGetFreq(PAL_SYS_CLKC_TIMER0);

    /* timer 1 - enable timer and auto load, and go off every 1 ms */
    PAL_sysResetCtrl(AVALANCHE_TIMER0_RESET, OUT_OF_RESET);
    sc_timer_err_nsec = -1;
    PAL_sysTimer16SetParams(AVALANCHE_TIMER0_BASE, SC_timer_clk,
            TIMER16_CNTRL_AUTOLOAD, (int)((1.0/(float)(HZ)) * 1000000.0), &sc_timer_err_nsec);
    PAL_sysTimer16Ctrl(AVALANCHE_TIMER0_BASE, TIMER16_CTRL_START);

    /* For VeryPrecise clock */
    /* It is too early to start the FRC, start later */
    late_time_init_chain = late_time_init;
    late_time_init = late_time_init_f;

    /* For Precise clock */
    sc_count_nsec = PAL_sysTimer16GetNSecPerCount(AVALANCHE_TIMER0_BASE, NSEC_PER_SEC / HZ);
    printk("Precise sched_clock: load %d, nsec/count %d\n", PAL_sysTimer16GetLoad(AVALANCHE_TIMER0_BASE), sc_count_nsec);

    setup_irq(AVALANCHE_TIMER_0_INT, &puma_timer0_irq);

    printk("Puma6 Timer0 initialized\n");
}
#endif


#ifndef CONFIG_HIGH_RES_TIMERS
/* For VeryPrecise clock */
/* Calc sched_clock from free running 64 bit counter */
unsigned long long notrace avalanche_clock_vp(void)
{
    unsigned long long ns = 0;
    /* Re-Read till hi is equal */
    UINT32 prev_hi = 0;
    UINT32 curr_hi = 0;
    UINT32 curr_lo = 0;
    static unsigned long long frc2correct = 0;
    unsigned long long curr_frc = 0;
    static long long err_crct_ns = 0;

    if (FRC_init == 0)
    {
        return (unsigned long long)(jiffies - INITIAL_JIFFIES) * (NSEC_PER_SEC / HZ);
    }

    do
    {
        prev_hi = curr_hi;
        /* Do lo before hi so when hi==0 a single read is enough */
        curr_lo = FREE_RUNNING_COUNTER_L_GET();
        curr_hi = FREE_RUNNING_COUNTER_H_GET();
    } while (prev_hi != curr_hi);
    curr_frc = (unsigned long long )(curr_hi) << 32 | (unsigned long long )(curr_lo);
    /* The base clock is 450MHz, so normalize to nSec */
    /* ns = cnt*tick_ns = cnt*(10^9/freq_hz) = cnt*(10^9 / 450MHz) = cnt*(1000/450) = cnt * 20 / 9 */
    /* Don't worry about wrap/overflow, 64bit * nSec ==~ 6000 years */
    ns = curr_frc;
    ns *= 20;
    ns += 5; /* Round up the division */
    do_div(ns, 9);

    if (frc2correct_load != 0)
    {
        if (curr_frc > frc2correct)
        {
            /* Reload */
            frc2correct = curr_frc + frc2correct_load;
            /* Recalc :
                correction == number of ticks * err per tick
                frc = nticks * 10mSec * frc timer_clk
                nticks = frc / 10mSec / frc timer_clk
                nticks = frc * HZ / frc timer_clk
                */
            err_crct_ns = curr_frc * HZ + SC_timer_clk/2;
            do_div(err_crct_ns, SC_timer_clk);
            err_crct_ns *= sc_timer_err_nsec;
            //printk("Recalculated err_crct_ns %lld: curr_frc %lld, next frc %lld\n", err_crct_ns, curr_frc, frc2correct);
        }
        ns += err_crct_ns;
    }

    /* Normalize using calibration value */
    ns -= sc_calibrate_jf;

    return ns;
}
#endif

/* This API is used to allocate specified size of memory for the
 * specified module (DSP) from the reserved region. Currently size
 * of reserved region is decided at compile time using Kconfig variable
*/
int avalanche_alloc_no_OperSys_memory(AVALANCHE_NO_OPERSYS_MOD_T mod,
                                        unsigned int size,
                                        unsigned int *phys_start)
{
    unsigned int cookie;
    int ret = 0;

    if(mod >= eNO_OperSys_END)
        return -1;

    PAL_osProtectEntry(PAL_OSPROTECT_INTERRUPT, &cookie);

    /* 32-bit align */
    size = ((size + 0x3) & (~0x3));

    /* we do not have that much reserved memory */
    if(size > no_linux_mem_size)
    {
        ret = -1;
        goto topmem_done;
    }

    switch(mod)
    {
        case eNO_OperSys_VDSP:
            /* If the memory was already allocated then simply return it */
            if(no_OperSys_memory_desc[mod].reserved)
            {
                *phys_start = no_OperSys_memory_desc[mod].phys_start;
                goto topmem_done;
            }

            no_linux_mem_last  -= size;
            *phys_start           = no_linux_mem_last;
            no_OperSys_memory_desc[mod].reserved = 1;
            no_OperSys_memory_desc[mod].phys_start = no_linux_mem_last;
            no_linux_mem_size  -= size;

            break;

        case eNO_OperSys_VoiceNI:
        {
            unsigned int    mask, padding;
            mask = size - 1;
            padding = no_linux_mem_start & mask;

            if(padding+size > no_linux_mem_size)
            {
                printk("%s:%d\n", __FUNCTION__,__LINE__);
                ret = -1;
                break;
            }
            if (padding)
            {
                no_linux_mem_start &= ~mask;
                no_linux_mem_start +=  size;
                no_linux_mem_size  -=  padding;
            }
            *phys_start                            = no_linux_mem_start;
            no_linux_mem_start += size;
            no_linux_mem_size  -= size;
            break;
        }

        default:
            ret = -1;
    }

topmem_done:
    PAL_osProtectExit(PAL_OSPROTECT_INTERRUPT, cookie);

    return ret;
}
EXPORT_SYMBOL(avalanche_alloc_no_OperSys_memory);

unsigned int avalanche_no_OperSys_memory_phys_to_virt( unsigned int pAddr )
{
    if ( -1 == no_linux_mem_desc_idx )
    {
        printk("%s:%d\n", __FUNCTION__,__LINE__);
        return (unsigned int)-1;
    }

    if ( (pAddr <  __pfn_to_phys(puma6_io_desc[ no_linux_mem_desc_idx ].pfn)) ||
         (pAddr > (__pfn_to_phys(puma6_io_desc[ no_linux_mem_desc_idx ].pfn) +
                                 puma6_io_desc[ no_linux_mem_desc_idx ].length )))
    {
        printk("%s:%d\n", __FUNCTION__,__LINE__);
        return (unsigned int)-1;
    }

    return ( pAddr
             - __pfn_to_phys(puma6_io_desc[ no_linux_mem_desc_idx ].pfn)
             +               puma6_io_desc[ no_linux_mem_desc_idx ].virtual );
}
EXPORT_SYMBOL(avalanche_no_OperSys_memory_phys_to_virt);

unsigned int avalanche_no_OperSys_memory_virt_to_phys( unsigned int vAddr )
{
    if ( -1 == no_linux_mem_desc_idx )
    {
        printk("%s:%d\n", __FUNCTION__,__LINE__);
        return (unsigned int)-1;
    }

    if ( (vAddr <  puma6_io_desc[ no_linux_mem_desc_idx ].virtual) ||
         (vAddr > (puma6_io_desc[ no_linux_mem_desc_idx ].virtual) +
                   puma6_io_desc[ no_linux_mem_desc_idx ].length ))
    {
        printk("%s:%d\n", __FUNCTION__,__LINE__);
        return (unsigned int)-1;
    }

    return ( vAddr
             -               puma6_io_desc[ no_linux_mem_desc_idx ].virtual
             + __pfn_to_phys(puma6_io_desc[ no_linux_mem_desc_idx ].pfn) );
}
EXPORT_SYMBOL(avalanche_no_OperSys_memory_virt_to_phys);

/* variable used by vlynq */
unsigned int avalanche_mem_size;
EXPORT_SYMBOL(avalanche_mem_size);

static void __init puma6_fixup(struct tag *tag,
         char **cmdline, struct meminfo *mi)
{
    for (; tag->hdr.size; tag = tag_next(tag)) {
        if (tag->hdr.tag == ATAG_MEM) {
            unsigned long size  =   tag->u.mem.size;
            unsigned long start =   tag->u.mem.start;

            size -= start & ~PAGE_MASK;

            /* This variable is used by DSP memory allocation API */
            no_linux_mem_last = start + size;

            size -= no_linux_mem_size;

            mi->bank[mi->nr_banks].start = PAGE_ALIGN(start);
            mi->bank[mi->nr_banks].size  = size & PAGE_MASK;
            mi->bank[mi->nr_banks].highmem = PHYS_TO_NID(start);

            /* dsp memory starts where normal memory ends */
            no_linux_mem_start = start + size;
            avalanche_mem_size += size;
            mi->nr_banks += 1;
            break;
        }
    }
}

/* boot_params:
 *  This is the location from where Linux should pick the Atag list
 *  created by the bootloader (mostly uboot)
 */
MACHINE_START(PUMA6, "puma6")
   /* .phys_io     = IO_START, */
   /* .io_pg_offst = ((IO_BASE) >> 18) & 0xfffc, */
    .atag_offset    =   CONFIG_ARM_AVALANCHE_KERNEL_PARAMS_OFFSET,
    .map_io         =   puma6_map_io,
    .init_irq       =   arch_init_irq,
    .init_early     =   puma_avalanche_setup,
    .init_time      =   puma6_timer_init,
    .fixup          =   puma6_fixup,
MACHINE_END

