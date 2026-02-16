/*
 * sdhci-puma7.c
 *
 *  GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2015-2017 Intel Corporation. All rights reserved.
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


/*
 * SDHCI support for Puma7 SoC
 *
 * 
 */

#include <asm/io.h>
#include <linux/mmc/host.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/proc_fs_macros.h>
#include <asm/uaccess.h>
#include "sdhci.h"
#include "sdhci-pltfm.h"

#define DRIVER_NAME "sdhci-puma7"

#define ISSPACE(c)  ((c) == ' ' || ((c) >= '\t' && (c) <= '\r'))


static struct sdhci_host *g_host;

static inline void sdhci_puma7_writeb(struct sdhci_host *host, u8 val, int reg)
{
    int align_reg;
    u32 align_val;

    /*
      In Puma7 we must write to HC Registers within width of 32 bits (4 bytes alingment) 
      Solution: Read the 32 bit register, modify the reqiured byte (8 bit), and write back 32 bits
    */

    /* align the register to 32 bit */
    align_reg = reg & 0xFFFFFFFC;      // Clear bit #0,#1

    // Read
    align_val = readl(host->ioaddr + align_reg);    
    
    // Modify 
    align_val = align_val &~ (((u32)0x000000FF) << ((reg & 0x00000003) << 3));  // Reset the 0,1,2 or 3 byte
    align_val = align_val | ((u32)val) << ((reg & 0x00000003) << 3);         // Add the 0,1,2 or 3 byte.

    // Do not write on initialization process - for ATOM - ARM Synchronization (avoid conflict)
    if ((host->flags & SDHCI_DISABLE_REGISTER_WRITE) != 0)
    {
        return;
    }

    // Write
    writel(align_val, host->ioaddr + align_reg);
}


static inline void sdhci_puma7_writew(struct sdhci_host *host, u16 val, int reg)
{
    int align_reg;
    u32 align_val;
    static u32 shadow_value;
    static u32 shadow_valid = 0;

    /* In Puma7 we must write to HC Registers within width of 32 bits (4 bytes alingment) 
      Solution: Read the 32 bit register, modify the High Word (16 bit), or Low Word, and write back 32 bits
    */

    /* In Puma7 A special case is wiriting to TRANSFER MODE register.
      Writing to TRANSFER MODE Register with Read/modify/write solution (as above), will trigger the 
      COMMAND Register, to send a message to eMMC card 
      Solution: write the value of TRANSFER MODE to 'a shadow' register, and next time the user write 
      to COMMAND Register, then write the shadow register to TRANSFER MODE register.
     
      Note: According to spec after each writing to TRANSFER MODE register, and user will also write
      to COMMAND Register. 
    */

    /* align the register to 32 bit */
    align_reg = reg & 0xFFFFFFFC;      // Clear bit #0,#1

    /* Save Transfer Mode to a shadow register */
    if (unlikely(reg == SDHCI_TRANSFER_MODE))
    {
        shadow_value = val;
        shadow_valid = 1;
        return;
    }

    /* Restore the Transfer Mode from a shadow register */
    if (unlikely((reg == SDHCI_COMMAND) && (shadow_valid == 1)))
    {
        // Read from shadow
        align_val = shadow_value;
        shadow_valid = 0;
    }
    else
    {
        // Read form Register
        align_val = readl(host->ioaddr + align_reg); 
    }

    // Modify 
    align_val = align_val &~ (((u32)0x0000FFFF) << ((reg & 0x00000003) << 3));  // Reset the first or second word (16 bits)
    align_val = align_val | ((u32)val) << ((reg & 0x00000003) << 3);            // Add the first or second word (16 bits)

    // Do not write on initialization process - for ATOM - ARM Synchronization (avoid conflict)
    if ((host->flags & SDHCI_DISABLE_REGISTER_WRITE) != 0)
    {
        return;
    }
    
    // Write
    writel(align_val, host->ioaddr + align_reg);
}

static inline void sdhci_puma7_writel(struct sdhci_host *host, u32 val, int reg)
{
    if ((host->flags & SDHCI_DISABLE_REGISTER_WRITE) != 0)
    {
        return;
    }
    // Write
    writel(val, host->ioaddr + reg);
}

static void sdhci_puma7_platform_init(struct sdhci_host *host)
{
    /* Puma7 platform specific configuration */
    host->mmc->caps |= MMC_CAP_8_BIT_DATA;          // Platfrom support 8 bits
    host->mmc->caps |= MMC_CAP_NONREMOVABLE;        // Card is not removable in this platform
    host->mmc->caps |= MMC_CAP_1_8V_DDR;            // Patform support DDR on 1.8V

    host->mmc->caps2 |= MMC_CAP2_HC_ERASE_SZ;       // Set because sdhci-acpi.c and sdhci-pci.c set it too
    // host->mmc->caps2 |= MMC_CAP2_POLL_R1B_BUSY;  // Can be remove - Not in use.
    host->mmc->caps2 |= MMC_CAP2_CACHE_CTRL;        // Enable card cache usage (if exist) (need to understand if it safe for power failure)
    host->mmc->caps2 |= MMC_CAP2_HS200_1_8V_SDR;    // Patform support 200Mhz SDR on 1.8V

	// host->mmc->pm_caps |= MMC_PM_KEEP_POWER;     // not in used

    //host->quirks2 |= SDHCI_QUIRK2_TUNING_POLL;    // not in used
    //host->quirks2 |= SDHCI_QUIRK2_PRESET_VALUE_BROKEN;  // Preset values are broken - do not use preset values - must identical to Atom
    host->quirks2 |= SDHCI_QUIRK2_HW_MUTEX;         // use hw_mutex

}


static struct sdhci_ops sdhci_puma7_ops = {
    .write_b = sdhci_puma7_writeb,
    .write_w = sdhci_puma7_writew,
    .write_l = sdhci_puma7_writel,
    .platform_init = sdhci_puma7_platform_init,

};

struct sdhci_pltfm_data sdhci_puma7_pdata = {
	.ops = &sdhci_puma7_ops,
	.quirks = // SDHCI_QUIRK_BROKEN_CARD_DETECTION |
              // SDHCI_QUIRK_NO_CARD_NO_RESET      |
              // SDHCI_QUIRK_DELAY_AFTER_POWER     |
              // SDHCI_QUIRK_BROKEN_DMA            |
              // SDHCI_QUIRK_BROKEN_ADMA           |
              // SDHCI_QUIRK_32BIT_DMA_ADDR        |
              // SDHCI_QUIRK_32BIT_DMA_SIZE        |
              // SDHCI_QUIRK_32BIT_ADMA_SIZE       |  
              // SDHCI_QUIRK_NO_HISPD_BIT          |
              // SDHCI_QUIRK_FORCE_1_BIT_DATA      |
              0,
};


static int sdhci_puma7_probe(struct platform_device *pdev)
{
    int ret  = 0;
    ret = sdhci_pltfm_register(pdev, &sdhci_puma7_pdata, 0);
    g_host = platform_get_drvdata(pdev);
    return ret;
}

static int sdhci_puma7_remove(struct platform_device *pdev)
{
    return sdhci_pltfm_unregister(pdev);
}

static struct platform_driver sdhci_puma7_driver = {
	.driver		= {
		.name	= "sdhci-puma7",
		.owner	= THIS_MODULE,
		.pm	= SDHCI_PLTFM_PMOPS,
	},
	.probe		= sdhci_puma7_probe,
	.remove		= sdhci_puma7_remove,
};

module_platform_driver(sdhci_puma7_driver);

MODULE_DESCRIPTION("SDHCI driver for Puma7");
MODULE_AUTHOR("Intel Corporation");
MODULE_LICENSE("GPL v2");



static int sdhci_puma7_proc_dump_regs_read (struct seq_file *m, void *v)
{
    if (g_host == NULL)
    {
        printk(KERN_ERR"g_host is NULL\n");
        return 0;
    }
    sdhci_dump_status(g_host);

    return 0;
}

DECLARE_PROCFS_READ_ENTRY(sdhci_puma7_dump_regs, sdhci_puma7_proc_dump_regs_read)

/**************************************************************************/
/*! \fn static int sdhci_puma7_proc_debug_read(struct seq_file *m, void *v)
 **************************************************************************
 *  \brief Debug.
 *
 *  \return 0.
 **************************************************************************/
static int sdhci_puma7_proc_debug_read(struct seq_file *m, void *v)
{
    seq_printf(m, "Debug read \n");
    
    return 0;
}


/**************************************************************************/
/*! \fn static int sdhci_puma7_proc_debug_write(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
 **************************************************************************
 *  \brief Debug.
 * 
 *  \return 0.
 **************************************************************************/
static int sdhci_puma7_proc_debug_write(struct file *fp, const char __user *buf, size_t count, loff_t *f_pos)
{
    char kbuf[64];
              
    int cnt = count;

    /* boundary check for input buffer */
    if (count > (64 - 1)) 
    {
        printk(KERN_ERR "Failed to write - count is too long (count=%d)\n", count);
        return -EINVAL;
    }

    /* Copy from user buffer to kernel buffer*/
    if (copy_from_user(&kbuf, buf, count) > 0)
    {
        printk(KERN_ERR "Failed to write - failed to copy from user\n");
        return -EFAULT;
    }

    /* Trim trailing white space */
    while (ISSPACE(kbuf[cnt-1]))
    {
        cnt--;
    }

    /* Set Null terminator */
    kbuf[cnt] = 0;

    /* Command length should be one charecter only. '1' or '0' */
    if (strlen(kbuf) != 1) 
    {
        printk(KERN_ERR "Invalid command.\n");
        return -EINVAL;
    }

    /* Parse opcode in buffer */
    if (kbuf[0] == '0')
    {
        printk("Debug write 0\n");
    }
    else if (kbuf[0] == '1')
    {
        printk("Debug write 1 \n");
    }
    else if (kbuf[0] == '2')
    {
        printk("Debug write 2 \n");
    }
    else
    {
        printk(KERN_ERR "Invalid command.\n");
        return -EINVAL;
    }

    return count;
}

DECLARE_PROCFS_ENTRY(sdhci_puma7_debug, sdhci_puma7_proc_debug_read, sdhci_puma7_proc_debug_write)



int sdhci_puma7_proc_init (void)
{
    proc_mkdir("mmc",NULL);
    proc_create("mmc/dump_regs", 0644, NULL, &sdhci_puma7_dump_regs_proc_fops);
    proc_create("mmc/debug", 0644, NULL, &sdhci_puma7_debug_proc_fops);
    pr_info(DRIVER_NAME ": /proc/mmc/dump_regs added. *\n");

    return 0;
}


fs_initcall(sdhci_puma7_proc_init);

