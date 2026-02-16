/*
  GPL LICENSE SUMMARY

  Copyright(c) 2008-2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.

  Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/
/*
 *
 * puma6_board_setup.c
 * Description:
 * puma6 board/soc initialization
 *
 *
 */
#include <pal.h>
#include <asm/arch/hardware.h>
#include <linux/proc_fs_macros.h>
#include <asm/uaccess.h>
#include <asm-arm/arch-avalanche/generic/sram_api.h>
#include <asm/system_misc.h>
#include <puma6_cru_ctrl.h>
#include "p_unit_api.h"
#include <ctype.h>

unsigned int cpu_freq = AVALANCHE_ARM_FREQ_DEFAULT;
unsigned long puma6_boardtype_id = 0; /* 0 - unknown boardtype */

static int arm_pwr_state = 0;
static struct proc_dir_entry *gp_arm_pwr_state_file;
static struct proc_dir_entry *gp_ddr_pwr_state_file;
static struct proc_dir_entry *gp_ddr_sync_state_file;
static struct proc_dir_entry *gp_ddr_pwr_state_verbose_file;

extern unsigned int system_rev;

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

#define AVL_NS_REMAP_BASE_MASK 0xFFFFF000

void avalanche_restart(char mode, const char *cmd);

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
    puma6_boardtype_id = simple_strtoul(str, NULL, 16);
    if (puma6_boardtype_id == PUMA6_UNKNOWN_BOARD_ID)
    {
        printk(KERN_ERR "Puma6 boardtype parsing failed - boardtype must be hex digit [%s].\n", str);
    }
    else
    {
        printk(KERN_INFO "Puma6 boardtype Id %lu \n",puma6_boardtype_id);
    }
    return 1;
}

__setup("boardtype=", set_boardtype_str);

static int arm_proc_read_state (struct seq_file *m, void *v)
{
    seq_printf (m, "%d\n", arm_pwr_state);

    return 0;
}

static int arm_proc_write_state (struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    /* In Puma-6 we cant change the ARM clock rate. */
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
    /* In Puma-6 we cant change the DDR regs. */
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



void avalanche_soc_platform_init(void)
{
     unsigned int paramVal_silicon;
     unsigned int paramVal_boardType;
     /***************************************************************
     * Boot config initialization - NOTE *** MUST BE DONE FIRST
     **************************************************************/
     PAL_sysBootCfgCtrl_init();

     /***************************************************************
     * Get entities on the board out of reset
     **************************************************************/
     PAL_sysResetCtrl(AVALANCHE_UART0_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_UART1_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_I2C_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_TIMER1_RESET, OUT_OF_RESET);
     PAL_sysResetCtrl(AVALANCHE_TIMER2_RESET, OUT_OF_RESET);

     PAL_sysResetCtrl(AVALANCHE_PP_RESET, OUT_OF_RESET);

     /*SPI driver initialize SPCR1 register which requires TDM to be out of reset ( TDM0 is not used )*/
     PAL_sysResetCtrl(CRU_NUM_TDM00, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM01, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM10, OUT_OF_RESET);
     PAL_sysResetCtrl(CRU_NUM_TDM11, OUT_OF_RESET);

    /* Check silicon stepping */
    SRAM_Get_Boot_Param(SILICON_STEPPING_ID, &paramVal_silicon);
    SRAM_Get_Boot_Param(BOARD_TYPE_ID, &paramVal_boardType);
    if ((paramVal_silicon >= SILICON_STEPPING_ID_C_0) &&
	((paramVal_boardType == PUMA6_BL_BOARD_ID) || (paramVal_boardType == PUMA6_FM_BOARD_ID)))
    {
         PAL_sysResetCtrl(AVALANCHE_I2C_DMA_RESET, OUT_OF_RESET); /* new i2c_dma controller */
    }
 
     /***************************************************************
     * Initialize clock controller
     **************************************************************/
     /* Initialize clock controller */
     PAL_sysClkcInit(NULL);

     cpu_freq = PAL_sysClkcGetFreq(PAL_SYS_CLKC_ARM);


     /***************************************************************
     * Boot config - Setup IOs
     **************************************************************/
     PAL_sysBootCfgCtrl_DocsisIo_UART0(BOOTCFG_IO_ENABLE);  /* Open UART-0 IOs */
     PAL_sysBootCfgCtrl_DocsisIo_UART1(BOOTCFG_IO_ENABLE);  /* Open UART-1 IOs */
     PAL_sysBootCfgCtrl_DocsisIo_IIC(BOOTCFG_IO_ENABLE);    /* Open I2C IOs */
     PAL_sysBootCfgCtrl_DocsisIo_US_AMP(BOOTCFG_IO_ENABLE);


    /***************************************************************
     * Programming ARM Peripheral Port remap register to map Interrupt controller
     * Userspace mmap on non-shared region will not work if this is not done
     ***************************************************************/
    if(avalanche_remap_non_shared(INTC_PHY, AVL_NS_REMAP_SIZE_4K) == 0)
        printk("Initialized Peripheral Port Remap Register to base : 0x%08x\n",  INTC_PHY);
    else
        printk("Peripheral Port Remap Register Initialization Failed at base : 0x%08x\n",  INTC_PHY);

#if 0 /* Should be review on GPIO modification task for puma6 - OMER*/
    /* Open Power LED */
    PAL_sysGpioCtrl( 0, GPIO_PIN, GPIO_OUTPUT_PIN);
#endif
    printk("Puma-6 system_rev = %d -- docsis ip rev = %d\n",system_rev, avalanche_get_chip_version_info());

    arm_pm_restart = avalanche_restart;
}

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

/*
 * Puma6 uses watch dog for resetting the device
 * NOTE: parameter mode is not used in Puma6
 */
void avalanche_system_reset(PAL_SYS_SYSTEM_RST_MODE_T mode)
{
#if 0
/* WDT PAL layer is initialized in watchdog character driver
 * if WDT character driver was compiled out then it should be
 * initialized before triggering reset
 */

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
#else
    p_unit_reset_soc();
#endif

    return ;
}

void avalanche_restart(char mode, const char *cmd)
{
	local_irq_disable();
	avalanche_system_reset((PAL_SYS_SYSTEM_RST_MODE_T)0);
}
