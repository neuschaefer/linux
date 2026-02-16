/*
 *
 * puma5_board_setup.c
 * Description:
 * puma5 board/soc initialization
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/
#include <asm-arm/arch-avalanche/generic/pal.h>
#include <asm/arch/hardware.h>
#ifdef CONFIG_ARM_AVALANCHE_VLYNQ
#include <asm/arch/generic/avalanche_vlynq_config.h>
#include <asm/arch/generic/pal_vlynq.h>
#endif
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <asm/system_misc.h>

#if defined(CONFIG_PUMA5_USBPHY_BYPASS_POR)
extern unsigned int system_rev;
#endif

void avalanche_set_vbus_freq(unsigned int new_vbus_freq);

unsigned int cpu_freq = AVALANCHE_ARM_FREQ_DEFAULT;

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

#if defined(CONFIG_MACH_PUMA5EVM)

char* puma5_boardtype;
int eth_reset_gpio_num = -1;
int ext_switch_reset_gpio = -1;
#ifdef CONFIG_ARM_EXTERNAL_SWITCH
extern PAL_Result switch_reset(int init);
#endif

static int __init set_boardtype_str (char *str)
{
    puma5_boardtype = &str[0];
    return 0;
}

early_param("boardtype", set_boardtype_str);


static int __init set_ext_switch_reset_gpio (char *str)
{

    if (!str)
    {
        return 0;
    }

    if (*str != '\0')
    {
        ext_switch_reset_gpio = simple_strtoul(str,NULL,0);
    }
    return 0;
}

early_param("ext_switch_reset_gpio", set_ext_switch_reset_gpio);


char *  puma5_usb_host_address;
static int __init usb_set_host_macstring(char *str)
{
    if(str[1])
    {
        puma5_usb_host_address = &str[1];
    }
    return 1;
}
__setup("usbhostaddr", usb_set_host_macstring);
EXPORT_SYMBOL(puma5_usb_host_address);


static void puma5_ext_phy_reset(int gpio_num)
{
    int delay;
    /* Get the External phy out of reset
     */
    /* Set the pin as GPIO pin */
    PAL_sysGpioCtrl(gpio_num, GPIO_PIN, GPIO_OUTPUT_PIN);
    /* Assert the reset pin by bringing it low */
    PAL_sysGpioOutBit(gpio_num, 0);
    /* According to datasheet minimum delay required is 20us put 1ms to be on safer side */
    udelay(1000);
    /* De-assert the reset */
    PAL_sysGpioOutBit(gpio_num, 1);

    /* 5ms recommended - udelay cannot take values more than 1000.
       mdelay() will not work at this point
     */
    for(delay = 0; delay < 5; delay++)
        udelay(1000);
}

#endif

#if defined(CONFIG_MACH_PUMA5EVM)
/* this function enables all the auxiliary GPIOs required for gmii */
static void puma5_gmii_enable(void)
{
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 19) /*aux_19*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* MII_TD[2] */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 20) /*aux_20*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* MII_TD[3] */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 21) /*aux_21*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* MII_TEN   */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 22) /*aux_22*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN);  /* MII_RD[2] */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 23) /*aux_23*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* MII_RD[3] */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 24) /*aux_24*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* MII_RCLK  */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 25) /*aux_25*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* MII_TCLK  */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 26) /*aux_26*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* MII_COL   */

    /* Set the Ethernet control register in the full duplex and giga bit mode enabled */
    /* TODO : Needs to confim this */
}
#endif

#if defined (CONFIG_ARM_AVALANCHE_VLYNQ)
/* Set AUX GPIOs [10-18] in function mode [SCRUN, RX, TX]*/
static void puma5_vlynq_gpio_set (void)
{
    /* Ignore "Direction" parameter for Functional Pins */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 10) /*aux_10*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* VLYNQ_SCRUN */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 11) /*aux_11*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* VLYNQ_RXD0 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 12) /*aux_12*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* VLYNQ_RXD1 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 13) /*aux_13*/,FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* VLYNQ_RXD2 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 14) /*aux_14*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* VLYNQ_RXD3 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 15) /*aux_15*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* VLYNQ_TXD0 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 16) /*aux_16*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* VLYNQ_TXD1 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 17) /*aux_17*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* VLYNQ_TXD2 */
    PAL_sysGpioCtrl((AVALANCHE_MAX_PRIMARY_GPIOS + 18) /*aux_18*/,FUNCTIONAL_PIN, GPIO_INPUT_PIN);  /* VLYNQ_TXD3 */
}

static char* vlynq_clkdir_str;
static int __init vlynq_set_clkdir_str (char *str)
{
    vlynq_clkdir_str = &str[1];
    return 1;
}

__setup("vlynq", vlynq_set_clkdir_str);

static int g_is_sink_vlynq;
static int is_sink_vlynq (char* str)
{
    int i;
    char sink_str[] = {'s', 'i', 'n'};

    for (i = 0; i < 3; i++)
        if (sink_str[i] != str[i])
            return 0;

    return 1;
}

extern AV_VL_GETENV vl_cfg_getenv;
extern AV_VL_PRINTF vl_cfg_printf;
extern void vlynq_delay_wait(Uint32 count);

static char* vlynq_getenv (char* env)
{
    return (NULL);
}

int vlynq_dev_init(void)
{
    unsigned int reset_pol;

    //vl_cfg_getenv = prom_getenv;

    /* Put vlynq in reset and get it out of reset after a delay */

    reset_pol = 1;

    vl_cfg_getenv = (AV_VL_GETENV)vlynq_getenv;
    vl_cfg_printf = (AV_VL_PRINTF)printk;
    avalanche_vlynq_set_hw_reset_info(AVALANCHE_LOW_VLYNQ_CONTROL_BASE, 0 /* hop */,
                                      1/*reset bit.*/, reset_pol);


    if (!g_is_sink_vlynq)
    {
        PAL_sysVlynqInit( );

        /* Enable this block for VLYNQ->WLAN */
#if 0
        /* Get WLAN Out of reset - GPIO8 0-->1 */
        /* Set the pin as GPIO pin */
        PAL_sysGpioCtrl(8, GPIO_PIN, GPIO_OUTPUT_PIN);
        /* Assert the reset pin by bringing it low */
        PAL_sysGpioOutBit(8, 0);
        udelay(1000);
        /* De-assert the reset */
        PAL_sysGpioOutBit(8, 1);
#endif

        if (avalanche_vlynq_enumerate(
                *(unsigned int *)(AVALANCHE_LOW_VLYNQ_CONTROL_BASE + 0x40),
                AVALANCHE_LOW_VLYNQ_CONTROL_BASE,
                AVALANCHE_LOW_VLYNQ_MEM_MAP_BASE))
        {
            printk ("*** VLYNQ Enumeration failed.\n");
            return -1;
        }
    }

    return 0;
}

void vlynq_dev_deinit(void)
{
    extern REMOTE_VLYNQ_DEV_RESET_CTRL_FN p_remote_vlynq_dev_reset_ctrl;
    //extern void vlynq_cleanup_module(void);
    //printk ("*** Un-installing VLYNQ driver...\n");
    //driver is uninstalled from suspend function
    //vlynq_cleanup_module ();

    if(p_remote_vlynq_dev_reset_ctrl) {
        p_remote_vlynq_dev_reset_ctrl(0, IN_RESET);
    }

    /* Enable this block for VLYNQ->WLAN */
#if 0
    /* Put WLAN in reset - GPIO8 1-->0 */
    /* Assert the reset pin by bringing it low */
    PAL_sysGpioOutBit(8, 0);
    udelay(1000);
#endif
}

static int vlynq_pwr_state = 0;
int vlynq_suspend (void)
{
    if (!vlynq_pwr_state)
    {
        vlynq_dev_deinit();
        udelay(1000);
        PAL_sysPowerCtrl((INT32)PSC_VLYNQ, PSC_SW_RST_DISABLE);
        vlynq_pwr_state = 1;
    }
    return 0;
}

int vlynq_resume(void)
{
    if (vlynq_pwr_state)
    {
        if (!vlynq_dev_init ())
        {
            vlynq_pwr_state = 0;
        }
    }
    return 0;
}

static int vlynq_proc_read_state (char *buf, char **start, off_t offset, int count, int *peof, void *data)
{
    int len;
    if (count < 3 || !buf)
        return -1;

    len = sprintf (buf, "%d\n", vlynq_pwr_state);
    *peof = 1;
    return len;
}

static int vlynq_proc_write_state (struct file *fp, const char *buf, unsigned long count, void *data)
{
    char local_buf[2+1];
    if (count > 3)
        return -EFAULT;

    copy_from_user(local_buf, buf, count);
    if (strncmp("0", local_buf, 1) == 0)
        vlynq_resume ();
    else if (strncmp("1", local_buf, 1) == 0)
        vlynq_suspend ();

    return count;
}

static struct proc_dir_entry *gp_pwr_state_file;
int vlynq_create_pwr_state_proc (void)
{
    gp_pwr_state_file =
        create_proc_entry("avalanche/vlynq_state", 0644, NULL);
    if (gp_pwr_state_file) {
        gp_pwr_state_file->read_proc = vlynq_proc_read_state;
        gp_pwr_state_file->write_proc = vlynq_proc_write_state;
    }

    return 0;
}

late_initcall(vlynq_create_pwr_state_proc);
#endif /* CONFIG_ARM_AVALANCHE_VLYNQ */

static int arm_pwr_state = 0;
static ssize_t arm_proc_read_state(struct seq_file *m, void *v)
{
    seq_printf(m, "%d\n", arm_pwr_state);
    return 0;
}

static ssize_t arm_proc_write_state (struct file *fp, const char __user *buf,
                                     size_t count, loff_t *f_pos)
{
    char local_buf[2+1];
    if (count > 3)
        return -EFAULT;

    copy_from_user(local_buf, buf, count);
    if ((strncmp("0", local_buf, 1) == 0) && (arm_pwr_state != 0)) {
        if (!PAL_sysClkcSetFreq (PAL_SYS_CLKC_ARM, 400000000))
            arm_pwr_state = 0;
    } else if ((strncmp("1", local_buf, 1) == 0) && (arm_pwr_state != 1)) {
        if (!PAL_sysClkcSetFreq (PAL_SYS_CLKC_ARM, 200000000))
            arm_pwr_state = 1;
    }
    return count;
}

static struct proc_dir_entry *gp_arm_pwr_state_file;

static int arm_proc_read_state_open(struct inode *inode, struct file *file)
{
    return single_open(file, arm_proc_read_state, NULL); 
}

static const struct file_operations arm_pwr_state_ops = {
    .open     = arm_proc_read_state_open,
    .read     = seq_read,
    .llseek   = seq_lseek,
    .release  = single_release,
    .write    = arm_proc_write_state
};
int arm_create_pwr_state_proc (void)
{
    gp_arm_pwr_state_file =
        proc_create("avalanche/arm_state", 0644, NULL, &arm_pwr_state_ops);
    return 0;
}

#define AVALANCHE_DDR_CONTROLLER_BASE   (0xE0000000)
#define SDRAM_REFRESH_CTRL_REG          (AVALANCHE_DDR_CONTROLLER_BASE + 0x0C)

void puma5_set_ddr_selfrefresh (int self_refresh)
{
    if (self_refresh)
        *(volatile unsigned int *)SDRAM_REFRESH_CTRL_REG |= (1<<31);
    else
        *(volatile unsigned int *)SDRAM_REFRESH_CTRL_REG &= ~(1<<31);
}

static int ddr_pwr_state = 0;
static int ddr_pwr_state_verbose_show (struct seq_file *m, void *v)
{
    seq_printf(m, "Power state: %s (%d)\nDDR ASYNCH refresh: %s\n",
               ddr_pwr_state ? "Self refresh" : "Normal", ddr_pwr_state,
               IS_DDR_ASYNCH ? "Enabled" : "Disabled");
    return 0;
}

static int ddr_sync_state_show (struct seq_file *m, void *v)
{
    seq_printf(m, "%s\n", IS_DDR_ASYNCH ? "Async" : "Sync");
    return 0;
}

static int ddr_pwr_state_show (struct seq_file *m, void *v)
{
    seq_printf(m, "%d\n", ddr_pwr_state);
    return 0;
}

static int ddr_pwr_state_write (struct file *filp, const char __user *buf,
                                 size_t count, loff_t *f_pos)
{
    char local_buf[2+1];
    if (count > 3)
        return -EFAULT;

    copy_from_user(local_buf, buf, count);
    if ((strncmp("0", local_buf, 1) == 0) && (ddr_pwr_state != 0)) {
        /* Set DDR in normal mode */
        puma5_set_ddr_selfrefresh (0);
        ddr_pwr_state = 0;
    } else if ((strncmp("1", local_buf, 1) == 0) && (ddr_pwr_state != 1)) {
        /* Set DDR in low power mode (self refresh) */
        puma5_set_ddr_selfrefresh (1);
        ddr_pwr_state = 1;
    }
    return count;
}

static struct proc_dir_entry *gp_ddr_pwr_state_file;
static struct proc_dir_entry *gp_ddr_sync_state_file;
static struct proc_dir_entry *gp_ddr_pwr_state_verbose_file;

static int ddr_pwr_state_open(struct inode *inode, struct file *file)
{
    return single_open(file, ddr_pwr_state_show, NULL); 
}

static const struct file_operations ddr_pwr_state_ops = {
    .open    = ddr_pwr_state_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release,
    .write   = ddr_pwr_state_write
};

static int ddr_sync_state_open(struct inode *inode, struct file *file)
{
    return single_open(file, ddr_sync_state_show, NULL); 
}

static const struct file_operations ddr_sync_state_ops = {
    .open    = ddr_sync_state_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release,
};

static int ddr_pwr_state_verbose_open(struct inode *inode, struct file *file)
{
    return single_open(file, ddr_pwr_state_verbose_show, NULL); 
}

static const struct file_operations ddr_pwr_state_verbose_ops = {
    .open    = ddr_pwr_state_verbose_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release,
};

int ddr_create_pwr_state_proc (void)
{
    gp_ddr_pwr_state_file =
        proc_create("avalanche/ddr_state", 0644, NULL,
                    &ddr_pwr_state_ops);

    gp_ddr_pwr_state_verbose_file =
        proc_create("avalanche/ddr_state_verbose", 0444, NULL,
                    &ddr_pwr_state_verbose_ops);

    gp_ddr_sync_state_file =
        proc_create("avalanche/ddr_sync_state", 0444, NULL,
                    &ddr_sync_state_ops);
    return 0;
}

late_initcall(ddr_create_pwr_state_proc);
late_initcall(arm_create_pwr_state_proc);

void avalanche_soc_platform_init(void)
{
    PAL_SYS_Puma5Init clkc;

/***************************************************************
 * Power Up/Down required modules IOPDCR
 **************************************************************/
#if defined(CONFIG_MACH_PUMA5EVM)
    unsigned int psc_pid = 0;

    /* Initialize PSC base  */
    #define AVL_PSC_NO_CCM_CONTROL 0
    if(PAL_sysPscInit(AVALANCHE_PSC_BASE, AVL_PSC_NO_CCM_CONTROL, &psc_pid) == 0)
        printk("Power & Sleep Controller @ 0x%08x Initialized [id-0x%08x]\n",AVALANCHE_PSC_BASE, psc_pid);
    else
        printk("Power & Sleep Controller @ 0x%08x Initialization failed\n", AVALANCHE_PSC_BASE);

#if defined(CONFIG_PUMA5_USBPHY_BYPASS_POR)
     /* by the usb phy POR - only for PG2 devices */
    if (system_rev == AVALANCHE_PG_REV2){
        printk("USB PHY POR Bypass enabled\n");
        *(u32 *)USB_PHY_POR_ADDR = 0x80;
        mdelay(10);
    }
#endif

    /* Disable the CLK_OUT0 as 25MHz clock to external phy is
     * supplied by external oscillator
     */
    avalanche_setIOPowerMode(IOPM_CLKOUT0, AVALANCHE_IO_POWER_DOWN);

    /* see chip level document for information in IOPDCR bits */
    avalanche_setIOPowerMode(IOPM_UART, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_TDM_CODEC, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_GPIO, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_I2C, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_GMII, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_OOB, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_EPGA, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_BBU, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_ASYNC_EMIF, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_AGC, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_TAGC, AVALANCHE_IO_POWER_UP);
    avalanche_setIOPowerMode(IOPM_EXT_INT, AVALANCHE_IO_POWER_UP);


/***************************************************************
 * Get entities on the board out of reset
 **************************************************************/

    PAL_sysResetCtrl((INT32)PSC_GROUP1, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_UART1, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_GPIO, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_TIMER0, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_TIMER1, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_TIMER2, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_MMAP_SPI, OUT_OF_RESET);
    PAL_sysResetCtrl((INT32)PSC_EMIF3E_VRST, OUT_OF_RESET);

    /* Bring CPI4 out of reset - for clean implementation, reset and then
     * unreset the module */
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK0, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK1, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK2, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK3, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK4, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK5, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK6, IN_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK7, IN_RESET);
    udelay (1000);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK0, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK1, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK2, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK3, OUT_OF_RESET);

     /* set the reference clock and initialize clock controller */
    clkc.refclk_inp = REFCLK_FREQ;
    PAL_sysClkcInit(&clkc);
     /* set USB clock speed to 24 Mhz */
    if(PAL_sysClkcSetFreq(PAL_SYS_CLKC_USB, AVALANCHE_USB_CLOCK_SPEED) < 0)
             printk("%s:USB clock set failed\n", __FUNCTION__);

    PAL_sysResetCtrl ((INT32)PSC_SR_CLK4, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK5, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK6, OUT_OF_RESET);
    PAL_sysResetCtrl ((INT32)PSC_SR_CLK7, OUT_OF_RESET);
    /* bring USB-PHY out of reset */
    PAL_sysResetCtrl ((INT32)PSC_USB_PHY, OUT_OF_RESET);
    /* bring Performance monitor module out of reset */
    PAL_sysResetCtrl ((INT32)PSC_PERF_MON, OUT_OF_RESET);

    udelay (1000);


#else /* CONFIG_MACH_PUMA5_VOLCANO */
    /* NWSS is already active because the bootloader was using it */
    /* so put it in reset mode and then bring it out of reset */
    PAL_sysResetCtrl( AVALANCHE_NWSS_RESET_BIT, IN_RESET );
    udelay(1000);
    /* take everything out of reset */
    PAL_sysResetCtrl( AVALANCHE_NWSS_RESET_BIT, OUT_OF_RESET );
    PAL_sysResetCtrl(AVALANCHE_CPMAC_ETH0_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_MDIO_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_GPIO_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_UART0_RESET_BIT, OUT_OF_RESET);
    PAL_sysResetCtrl(AVALANCHE_UART1_RESET_BIT, OUT_OF_RESET);
#endif /*CONFIG_MACH_PUMA5EVM */

/***************************************************************
 * Initialize clock controller
 **************************************************************/
/* initialize vbus frequency */
    avalanche_set_vbus_freq(PAL_sysClkcGetFreq(CLKC_VBUS));
    cpu_freq = PAL_sysClkcGetFreq(CLKC_ARM);

#if defined(CONFIG_MACH_PUMA5EVM)
    /* set GMII clock to 125Mhz */
    if(PAL_sysClkcSetFreq(PAL_SYS_CLKC_GMII, AVALANCHE_GMII_CLOCK_SPEED) < 0)
        printk("%s:GMII Clock set failed\n", __FUNCTION__);
/***************************************************************
 * Initialize Board GPIO mode and program GMII
 * -pin muxing-
 **************************************************************/

#if (CONFIG_AVALANCHE_CONSOLE_PORT > 0)
    /*  pin-mux for second uart, GPIOs 10,11,12,13 should be in functional mode
        pin direction (last param) doesnt matter since we are in functional mode
        GPIO 12 is also used as Link Active LED, so if second UART is used then
        Link Active LED cannot be used */
    PAL_sysGpioCtrl( 10, FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* UART-B_TD */
    PAL_sysGpioCtrl( 11, FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* UART-B_RD */
    PAL_sysGpioCtrl( 12, FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* UART-B_CTS */
    PAL_sysGpioCtrl( 13, FUNCTIONAL_PIN, GPIO_OUTPUT_PIN); /* UART-B_RTS */
#endif

    /* Initialize GMII module */

    /* Determine ethernet reset gpio pin as per the per the environment variable
     * setting for board type. Currently supported values for 'boardtype' are:
     * 'tnetc550' and 'tnetc950'.
     */
    {
        typedef struct
        {
            char *boardtype;
            int eth_reset_gpio_num;

        } gmii_gpio_t;

        static const gmii_gpio_t gmii_gpio[] =
        {
            { "tnetc550", EXTPHY_RESET_TNETC550_GPIO_NUM },  /* Default */
            { "tnetc950", EXTPHY_RESET_TNETC950_GPIO_NUM },
            { "tnetc958", EXTPHY_RESET_TNETC958_GPIO_NUM },
            { "tnetc958ext", EXTPHY_RESET_TNETC958_GPIO_NUM },
            { "tnetc552", EXTPHY_RESET_TNETC552_GPIO_NUM },
            { NULL, 0 } /* Last */
        };

        if( !puma5_boardtype )
        {
            eth_reset_gpio_num = gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].eth_reset_gpio_num;
            puma5_boardtype = gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].boardtype;
            printk ("WARN: boardtype variable was %s, using default (%s)\n", "not set", gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].boardtype );
        }
        else
        {
            int i = 0;

            while( gmii_gpio[i].boardtype != NULL )
            {
                if( strcmp( puma5_boardtype, gmii_gpio[i].boardtype ) == 0 )
                {
                    eth_reset_gpio_num = gmii_gpio[i].eth_reset_gpio_num;
                    break;
                }
                i++;
            }

            if(eth_reset_gpio_num == -1)
            {
                printk ("WARN: boardtype variable was %s [%s], using default (%s)\n", "set incorrectly", puma5_boardtype, gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].boardtype );
                eth_reset_gpio_num  = gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].eth_reset_gpio_num;
                puma5_boardtype     = gmii_gpio[EXTPHY_RESET_DEFAULT_INDEX].boardtype;
            }
        }

        /* for tnetc958, power up clockout0 */
        if( (eth_reset_gpio_num == EXTPHY_RESET_TNETC958_GPIO_NUM) && ( strcmp( puma5_boardtype, "tnetc958" ) == 0 ) )
        {
            avalanche_setIOPowerMode( IOPM_CLKOUT0, AVALANCHE_IO_POWER_UP );
        }

        printk( "Board type: %s\n", puma5_boardtype );

#ifdef CONFIG_ARM_EXTERNAL_SWITCH
        switch_reset(1);
#endif
        puma5_ext_phy_reset(eth_reset_gpio_num);
        puma5_gmii_enable();
    }
#endif /* CONFIG_MACH_PUMA5EVM */

/***************************************************************
 * Vlynq initialization
 **************************************************************/
#if defined (CONFIG_ARM_AVALANCHE_VLYNQ)
    avalanche_setIOPowerMode(IOPM_VLYNQ, AVALANCHE_IO_POWER_UP);
    puma5_vlynq_gpio_set ();

    /* Set VLYNQ clock to source/sink as per the environment variable setting.
     * Actually te bit in BootCFG register sould have been set/reset as per
     * hardware configuration but still relying on the env variable method. If
     * we find out that the bits are really set by h/w, we can remove this code.
     */
    {
        volatile unsigned int *bootcfg_reg = (unsigned int*)AVALANCHE_DCL_BOOTCR;
        printk ("Default VLYNQ clock is set as %s\n",
                (*bootcfg_reg & (1<<6)) ? "'source'" : "'sink'");

        if (!vlynq_clkdir_str)
            printk ("WARN: Env variable for VLYNQ clock direction not set\n"
                    "Assuming 'source' vlynq.\n"
                    "Pass 'vlynq=src' or 'vlynq=sin' to kernel,"
                    "any other value is assumed as 'src'.\n");


        if (vlynq_clkdir_str && is_sink_vlynq (vlynq_clkdir_str))
        {
            g_is_sink_vlynq = 1;
            *bootcfg_reg &= ~(1<<6);
        }
        else
            *bootcfg_reg |= (1<<6);

        printk ("VLYNQ clock is set as %s\n",
                (*bootcfg_reg & (1<<6)) ? "'source'" : "'sink'");
    }

    vlynq_dev_init();
#endif

/***************************************************************
 * Programming ARM Peripheral Port remap register to map Interrupt controller
 * Userspace mmap on non-shared region will not work if this is not done
 ***************************************************************/
    if(avalanche_remap_non_shared(INTC_PHY, AVL_NS_REMAP_SIZE_4K) == 0)
        printk("Initialized Peripheral Port Remap Register to base : 0x%08x\n",  INTC_PHY);
    else
        printk("Peripheral Port Remap Register Initialization Failed at base : 0x%08x\n",  INTC_PHY);

    /* Power LED */
    PAL_sysGpioCtrl( 0, GPIO_PIN, GPIO_OUTPUT_PIN);


    arm_pm_restart = avalanche_restart;
}

#ifdef CONFIG_AVALANCHE_INTC_PACING
unsigned int avalanche_get_intc_input_freq(void)
{
    return PAL_sysClkcGetFreq(CLKC_SYS);
}
#endif

void avalanche_processor_idle(void)
{
    cpu_do_idle();
    return ;
}


/*
 * Puma5 uses watch dog for resetting the device
 * NOTE: parameter mode is not used in Puma5
 */
void avalanche_system_reset(PAL_SYS_SYSTEM_RST_MODE_T mode)
{
/* Puma5 volcano RTLs doesnt have watchdog support */
#if defined(CONFIG_MACH_PUMA5EVM)

/* WDT PAL layer is initialized in watchdog character driver
 * if WDT character driver was compiled out then it should be
 * initialized before triggering reset
 */
#if !defined(CONFIG_ARM_AVALANCHE_WDTIMER)
    /* get the Watchdog timer module out of reset */
    PAL_sysResetCtrl(AVALANCHE_WDT_RESET, OUT_OF_RESET);

    /* Initialize watchdog timer */
    PAL_sysWdtimerInit(AVALANCHE_WATCHDOG_TIMER_BASE,
                            PAL_sysClkcGetFreq(CLKC_VBUS));
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

/* volcano uses yamuna reset controller IP */
#else
    PAL_sysSystemReset(RESET_SOC_WITH_MEMCTRL);
#endif
    return ;
}

void avalanche_restart(char mode, const char *cmd)
{
    local_irq_disable();
    avalanche_system_reset((PAL_SYS_SYSTEM_RST_MODE_T)0);
}
