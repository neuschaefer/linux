/*
 *
 * puma7_board_setup.c
 * Description:
 * puma7 board/soc initialization
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014-2016 Intel Corporation. All rights reserved.
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
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *    Intel Corporation
 *    2200 Mission College Blvd.
 *    Santa Clara, CA  97052
 *
 */


#include <pal.h>
#include <asm/arch/hardware.h>
#ifdef CONFIG_CACHE_L2X0
#include <asm/hardware/cache-l2x0.h>
#endif
#include <linux/proc_fs_macros.h>
#include <asm/uaccess.h>
#include <asm-arm/arch-avalanche/generic/sram_api.h>
#include <asm/system_misc.h>
#include <asm/outercache.h>
#include <linux/syscore_ops.h>
#include <../puma7/puma7_cru_ctrl.h>
#include <ctype.h>
#include <asm-arm/arch-avalanche/puma7/puma7_bootcfg_ctrl.h>

unsigned int cpu_freq = AVALANCHE_ARM_FREQ_DEFAULT;
unsigned long puma7_boardtype_id = 0; /* 0 - unknown boardtype */

#if 0
static int arm_pwr_state = 0;
static struct proc_dir_entry *gp_arm_pwr_state_file;
static struct proc_dir_entry *gp_ddr_pwr_state_file;
static struct proc_dir_entry *gp_ddr_sync_state_file;
static struct proc_dir_entry *gp_ddr_pwr_state_verbose_file;
#endif 

extern unsigned int system_rev;
extern void (*pm_power_off)(void);

/* reboot hook: */
extern int reboot_hook_proc_init(void);
extern int reboot_hook_is_set(void);
extern int reboot_hook_run(void);

/** \enum AVL_NS_REMAP_SIZE_T
    \brief Port remap window size
*/
typedef enum AVL_NS_REMAP_SIZE_tag
{
    AVL_NS_REMAP_SIZE_0K    = 0,
    AVL_NS_REMAP_SIZE_INV1, /* this is not supported by arm */
    AVL_NS_REMAP_SIZE_INV2, /* this is not supported by arm */
    AVL_NS_REMAP_SIZE_4K,
    AVL_NS_REMAP_SIZE_8K,

    /* like this it can go upto 2GB, but let us keep 8k as max*/
    AVL_NS_REMAP_SIZE_MAX
}AVL_NS_REMAP_SIZE_T;

#define AVL_NS_REMAP_BASE_MASK 0xFFFFF000 /* TBD - what should be the value?*/

void puma_restart(enum reboot_mode reboot_mode, const char *cmd);
/* void puma_power_off(void); */

/* This routine remaps the peripheral port remap register to the specified physical address*/
/* argument base should be aligned to the size of remapped region */
int avalanche_remap_non_shared(unsigned int phy_base, AVL_NS_REMAP_SIZE_T size)
{
    unsigned int region_size = 0;

    if((size >= AVL_NS_REMAP_SIZE_MAX) || (size == AVL_NS_REMAP_SIZE_INV1) || (size == AVL_NS_REMAP_SIZE_INV2))
        return (-1);

    /* calculate region_size from enum (second param) */
    region_size = ((1 << (size-1)) * (1024));

    /* align base address to the specified size */
    phy_base &= (~(region_size-1));

    /* mask out unwanted bits */
    phy_base &= AVL_NS_REMAP_BASE_MASK;

    /* set the size bit */
    phy_base |= size;

    /* update port remap h/w register */
    __asm__ ("mcr   p15, 0, %0, c15, c2, 4" : : "r" (phy_base): "cc");

    return (0);
}

static int __init set_boardtype_str (char *str)
{
    puma7_boardtype_id = simple_strtoul(str, NULL, 16);
    if (puma7_boardtype_id == PUMA7_UNKNOWN_BOARD_ID)
    {
        printk(KERN_ERR "Puma7 boardtype parsing failed - boardtype must be hex digit [%s].\n", str);
    }
    else
    {
        printk(KERN_INFO "Puma7 boardtype Id %lu \n",puma7_boardtype_id);
    }
    return 1;
}

__setup("boardtype=", set_boardtype_str);

#if 0
static int arm_proc_read_state (struct seq_file *m, void *v)
{
    seq_printf (m, "%d\n", arm_pwr_state);

    return 0;
}

static int arm_proc_write_state (struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    /* In Puma-7 we cant change the ARM clock rate. */
    return -EPERM; /* Operation not permitted */
}

DECLARE_PROCFS_ENTRY(arm_state, arm_proc_read_state, arm_proc_write_state)


int arm_create_pwr_state_proc (void)
{
    gp_arm_pwr_state_file =
        proc_create("avalanche/arm_state", 0644, NULL, &arm_state_proc_fops);

    return 0;
}

static int ddr_proc_read_state_verbose (struct seq_file *m, void *v)
{
    return -EPERM;
}

static int ddr_proc_read_sync_state (struct seq_file *m, void *v)
{
   return -EPERM;
}

static int ddr_proc_read_state (struct seq_file *m, void *v)
{
   return -EPERM;
}

static int ddr_proc_write_state (struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    /* In Puma-7 we cant change the DDR regs. */
    return -EPERM; /* Operation not permitted */
}

DECLARE_PROCFS_ENTRY(ddr_state, ddr_proc_read_state, ddr_proc_write_state)
DECLARE_PROCFS_ENTRY(ddr_state_verbose, ddr_proc_read_state_verbose, NULL)
DECLARE_PROCFS_ENTRY(ddr_sync_state, ddr_proc_read_sync_state, NULL)

int ddr_create_pwr_state_proc (void)
{
    gp_ddr_pwr_state_file =
        proc_create("avalanche/ddr_state", 0644, NULL, &ddr_state_proc_fops);

    gp_ddr_pwr_state_verbose_file =
        proc_create("avalanche/ddr_state_verbose", 0444, NULL,
                    &ddr_state_verbose_proc_fops);

    gp_ddr_sync_state_file =
        proc_create("avalanche/ddr_sync_state", 0444, NULL,
                    &ddr_sync_state_proc_fops);
    return 0;
}

late_initcall(ddr_create_pwr_state_proc);
late_initcall(arm_create_pwr_state_proc);
#endif

int puma7_proc_init (void)
{
	reboot_hook_proc_init();
	return 0;
}
fs_initcall(puma7_proc_init);

/* Boot sequance:
    1. start_kernel()@/src/init/main.c calling to ->
    2. setup_arch()@/src/arch/arm/kernel/setup.c calling to ->
    3. init_early (a callback, which is initialize to puma_avalanche_setup,
       by MACHINE_START macro at src\arch\arm\mach-avalanche\puma7\puma7_core.c or puma6/puma5)
    4. puma_avalanche_setup, then calling to this function (avalanche_soc_platform_init)
*/
void avalanche_soc_platform_init(void)
{
     /* unsigned int paramVal_silicon; */
     /* unsigned int paramVal_boardType; */
     /***************************************************************
     * Boot config initialization - NOTE *** MUST BE DONE FIRST
     **************************************************************/
     PAL_sysBootCfgCtrl_init();

     PAL_sysBootCfgCtrl_Endianess();

     /* Select SAI for RCE tarnsaction to PMC over IOSF-SB (Direct) */
     /* For B0 stepping - may be removed */
     PAL_sysBootCfgCtrl_SetRCE_SAI();

     /***************************************************************
     * Get entities on the board out of reset
     **************************************************************/
     PAL_sysResetCtrl(AVALANCHE_UART0_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_UART1_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_I2C_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_TIMER1_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_TIMER2_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_GPIO_RESET_BIT, OUT_OF_RESET);

     PAL_sysResetCtrl(AVALANCHE_PP_RESET, OUT_OF_RESET);

     /*puma7 TDM out of reset*/
     PAL_sysResetCtrl(CRU_NUM_TDM00, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM01, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM10, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM11, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM20, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM21, OUT_OF_RESET);

// TBD - add /* Check silicon stepping */
     /***************************************************************
     * Initialize clock controller
     **************************************************************/
     /* Initialize clock controller */
     PAL_sysClkcInit(NULL);

     cpu_freq = PAL_sysClkcGetFreq(PAL_SYS_CLKC_ARM);

    /***************************************************************
     * Programming ARM Peripheral Port remap register to map Interrupt controller
     * Userspace mmap on non-shared region will not work if this is not done
     ***************************************************************/
    if(avalanche_remap_non_shared(INTC_PHY, AVL_NS_REMAP_SIZE_4K) == 0)
        printk("Initialized Peripheral Port Remap Register to base : 0x%08x\n",  INTC_PHY);
    else
        printk("Peripheral Port Remap Register Initialization Failed at base : 0x%08x\n",  INTC_PHY);

    printk("Puma-7 system_rev = %d -- docsis ip rev = %d\n", system_rev, avalanche_get_chip_version_info());

    /* Callback definition for the last function to be called during system restart process */
    arm_pm_restart = puma_restart;

    /* Callback definition for the last function to be called during system shutdown process */
    /* pm_power_off = puma_power_off; */

    /* Enabling clocks for MoCA */
    PAL_sysBootCfgCtrl_MoCAConfigClkMux(BOOTCFG_IO_ENABLE);
}

void l2x0_shutdown(void)
{
    unsigned long flags;

    printk(KERN_EMERG "Shutting down L2cache\n");
    /* disable interrupts */
    local_irq_save(flags);  
    outer_disable();
    /* enable interrupts */
    local_irq_restore(flags);
}

static struct syscore_ops l2x0_syscore_ops = {
     .shutdown = l2x0_shutdown,
};

static int __init avalanche_soc_platform_init_late(void)
{

    /***************************************************************
     * Initialize l2cache controller
     **************************************************************/
#ifdef CONFIG_CACHE_L2X0
    {
        bool                    l2cache_enabled;
        PAL_SYS_RESET_CTRL_T    l2cache_reset;

        unsigned int aux_mask, aux_val;

        /* The following two settings must be enabled by UEFI for L2$ to work */
        l2cache_enabled = PAL_sysBootCfgCtrl_IsCpuL2CacheEnabled();
        l2cache_reset = PAL_sysGetResetStatus(CRU_NUM_ARM11_L2);

        if(l2cache_enabled == false)
        {
            printk("Puma7 L2C: Not enabled in boot config!\n");
        }
        else if(l2cache_reset != OUT_OF_RESET)
        {
            printk("Puma7 L2C: CRU is not taken out of reset!, l2cache_reset state=%d\n", l2cache_reset);
        }
        else
        {
            /*
                L2cache Configuration
                ---------------------
                [30]    ; Early BRESP enable
                [27]    ; NS Interrupt access control
                [26]    ; NS Lockdown enable
                [25]    ; Random replacement
                [23]    ; [24:23] Write allocate override (ex: 2 = force write allocate)
                [22]    ; Shared override enable
                [21]    ; Parity enable
                [20]    ; Event bus enable
                [19:17] ; Way size - 001 for 16KB
                [16]    ; Associativity - 1 for 16 ways
                [12]    ; Exclusive cache enable
                [11]    ; Store buffer device limitation enable
                [10]    ; SO and DEV read priority enable
                [0 ]    ; Full line of zero enable
            */
            aux_val = (L310_AUX_CTRL_ASSOCIATIVITY_16 |
                       L2C_AUX_CTRL_WAY_SIZE(1));

            aux_mask = ~(L310_AUX_CTRL_FULL_LINE_ZERO |
                         L310_AUX_CTRL_ASSOCIATIVITY_16 |
                         L2C_AUX_CTRL_WAY_SIZE_MASK |
                         L310_AUX_CTRL_CACHE_REPLACE_RR |
                         L310_AUX_CTRL_EARLY_BRESP);

            l2x0_init((void*)AVALANCHE_L2CACHE_BASE, aux_val, aux_mask);
            register_syscore_ops(&l2x0_syscore_ops);
            printk("Puma7 L2C: L2cache init DONE\n");
        }
    }
#endif  /* CONFIG_CACHE_L2X0 */

    return 0;

}
early_initcall(avalanche_soc_platform_init_late);

#ifdef CONFIG_AVALANCHE_INTC_PACING
unsigned int avalanche_get_intc_input_freq(void)
{
    return PAL_sysClkcGetFreq(PAL_SYS_CLKC_INTC);
}
#endif

void avalanche_processor_idle(void)
{
    cpu_do_idle();
    return ;
}


#if 0
/*
 * Puma7 OS power off flow ends here - on shutdown send
 * MBOX message to PMC and write to RCE shut down register.
 */
void puma_power_off( void )
{
    printk(KERN_EMERG "Power Off: Send message and write to RCE register\n");
    /* MBOX Send delayed message */
    /* Write to RCE Register */
}
#endif

/*
 * Puma7 OS fatal error reset flow ends here - on shutdown
 * Set watchdog timer for immediate reset.
 */
void avalanche_system_reset(void)
{
    if (reboot_hook_is_set() == 1)
    {
        /* Reboot Hook -
           In case of graceful reboot, the userspace application sets a reboot hook
           before calling to reboot() (before killing the init process).

           To set a reboot hook the user needs to write to /proc/avalanche/reboot_hook.
           More information in "puma7_reboot_hook.c".
        */
        reboot_hook_run();

#if defined(CONFIG_ARM_AVALANCHE_WDTIMER)
        /* disable watchdog timer */
        printk("Reboot: Disable WDT.\n");
        PAL_sysWdtimerCtrl(AVALANCHE_WDT_DISABLE_VALUE);
#endif

        printk("Reboot: system reset.\n");
        while (1);
    }

    /* If reboot hook is not set then initiate a WatchDog Reset */
    printk("Reboot: system reset.\n");
    local_irq_disable();

#if !defined(CONFIG_ARM_AVALANCHE_WDTIMER)
    /* get the Watchdog timer module out of reset */
    PAL_sysResetCtrl(AVALANCHE_WDT_RESET, OUT_OF_RESET);

    /* Initialize watchdog timer */
    PAL_sysWdtimerInit(AVALANCHE_WATCHDOG_TIMER_BASE,
                            PAL_sysClkcGetFreq(PAL_SYS_CLKC_WDT));
#endif

    /* disable watchdog timer */
    if(PAL_sysWdtimerCtrl(AVALANCHE_WDT_DISABLE_VALUE))
        return;

    /* set timer (in msecs) */
    if(PAL_sysWdtimerSetPeriod(AVALANCHE_WDT_RESET_MARGIN))
        return;

    /* enable watchdog timer */
    if(PAL_sysWdtimerCtrl(AVALANCHE_WDT_ENABLE_VALUE))
        return;

    /* kick is required to reload the new timeout margin */
    PAL_sysWdtimerKick();

    while (1);
}

void puma_restart(enum reboot_mode reboot_mode, const char *cmd)
{
	avalanche_system_reset();
}
