/*
 * sdhci-pltfm-kona.c Support for SDHCI KONA platform devices
 * Copyright (c) 2010 - 2012 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Supports:
 * SDHCI platform devices specific for KONA
 *
 * Inspired by sdhci-pci.c, by Pierre Ossman
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/highmem.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/mmc/host.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>

#include <mach/sdio_platform.h>

#include "sdhci-pltfm.h"
#include "sdhci.h"

#define SDHCI_SOFT_RESET            0x01000000
#define KONA_SDHOST_CORECTRL        0x8000
#define KONA_SDHOST_CD_PINCTRL      0x00000008
#define KONA_SDHOST_STOP_HCLK       0x00000004
#define KONA_SDHOST_RESET           0x00000002
#define KONA_SDHOST_EN              0x00000001

#define KONA_SDHOST_CORESTAT        0x8004
#define KONA_SDHOST_WP              0x00000002
#define KONA_SDHOST_CD_SW           0x00000001

#define KONA_SDHOST_COREIMR         0x8008
#define KONA_SDHOST_IP              0x00000001

#define KONA_SDHOST_COREISR         0x800C
#define KONA_SDHOST_COREIMSR        0x8010
#define KONA_SDHOST_COREDBG1        0x8014
#define KONA_SDHOST_COREGPO_MASK    0x8018

#define DEV_NAME                     "sdio"
#define MAX_DEV_NAME_SIZE            20

#define MAX_PROC_BUF_SIZE            256
#define MAX_PROC_NAME_SIZE           20
#define PROC_GLOBAL_PARENT_DIR       "sdhci-pltfm"
#define PROC_ENTRY_CARD_CTRL         "cardCtrl"
#define PROC_ENTRY_REGULATOR_CTRL    "regulator"
#define SD_DETECT_GPIO_DEBOUNCE_128MS	128

struct procfs {
	char name[MAX_PROC_NAME_SIZE];
	struct proc_dir_entry *parent;
};

struct sdio_dev {
	atomic_t initialized;
	struct device *dev;
	struct sdhci_host *host;
	unsigned long clk_hz;
	enum sdio_devtype devtype;
	int cd_gpio;
	int wp_gpio;
	struct sdio_wifi_gpio_cfg *wifi_gpio;
	struct procfs proc;

	struct clk *peri_clk;
	struct clk *sleep_clk;

	struct sdhci_ops ops;

	/* regulator that provides I/O voltage */
	struct regulator *vddo_regulator;
};

#if defined(CONFIG_MACH_BCM2850_FPGA) || defined(CONFIG_MACH_CAPRI_FPGA)
/* user can specify the clock when this driver is installed */
static unsigned int clock;
module_param(clock, uint, 0444);
#endif

#if defined(CONFIG_MACH_CAPRI_FPGA)
/* this is for FPGA also it looks awkward but it is
 * guaranteed to have at least 3 elements*/
static const unsigned long gClock[SDIO_MAX_NUM_DEVICES] = {
	/* set everything to 400 kHz by default for FPGA */
	400000,
	400000,
	400000,
	400000,
};
#endif

static struct proc_dir_entry *gProcParent;
static struct sdio_dev *gDevs[SDIO_MAX_NUM_DEVICES];
static struct sdio_dev *get_wifi_dev(void);

#ifdef CONFIG_MACH_KONA_FPGA
int sdhci_pltfm_clk_enable(struct sdhci_host *host, int enable) { }
#else
int sdhci_pltfm_clk_enable(struct sdhci_host *host, int enable);
#endif

static int sdhci_pltfm_regulator_init(struct platform_device *pdev,
		struct sdio_platform_cfg *hw_cfg);
static void sdhci_pltfm_regulator_term(struct platform_device *pdev);

#define DRIVER_NAME "sdio"
/*
static void sdhci_dumpregs(struct sdhci_host *host)
{
	printk(KERN_DEBUG DRIVER_NAME ": ============== REGISTER DUMP ==============\n");

	printk(KERN_DEBUG DRIVER_NAME ": Sys addr: 0x%08x | Version:  0x%08x\n",
		sdhci_readl(host, SDHCI_DMA_ADDRESS),
		sdhci_readw(host, SDHCI_HOST_VERSION));
	printk(KERN_DEBUG DRIVER_NAME ": Blk size: 0x%08x | Blk cnt:  0x%08x\n",
		sdhci_readw(host, SDHCI_BLOCK_SIZE),
		sdhci_readw(host, SDHCI_BLOCK_COUNT));
	printk(KERN_DEBUG DRIVER_NAME ": Argument: 0x%08x | Trn mode: 0x%08x\n",
		sdhci_readl(host, SDHCI_ARGUMENT),
		sdhci_readw(host, SDHCI_TRANSFER_MODE));
	printk(KERN_DEBUG DRIVER_NAME ": Present:  0x%08x | Host ctl: 0x%08x\n",
		sdhci_readl(host, SDHCI_PRESENT_STATE),
		sdhci_readb(host, SDHCI_HOST_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Power:    0x%08x | Blk gap:  0x%08x\n",
		sdhci_readb(host, SDHCI_POWER_CONTROL),
		sdhci_readb(host, SDHCI_BLOCK_GAP_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Wake-up:  0x%08x | Clock:    0x%08x\n",
		sdhci_readb(host, SDHCI_WAKE_UP_CONTROL),
		sdhci_readw(host, SDHCI_CLOCK_CONTROL));
	printk(KERN_DEBUG DRIVER_NAME ": Timeout:  0x%08x | Int stat: 0x%08x\n",
		sdhci_readb(host, SDHCI_TIMEOUT_CONTROL),
		sdhci_readl(host, SDHCI_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Int enab: 0x%08x | Sig enab: 0x%08x\n",
		sdhci_readl(host, SDHCI_INT_ENABLE),
		sdhci_readl(host, SDHCI_SIGNAL_ENABLE));
	printk(KERN_DEBUG DRIVER_NAME ": AC12 err: 0x%08x | Slot int: 0x%08x\n",
		sdhci_readw(host, SDHCI_ACMD12_ERR),
		sdhci_readw(host, SDHCI_SLOT_INT_STATUS));
	printk(KERN_DEBUG DRIVER_NAME ": Caps:     0x%08x | Max curr: 0x%08x\n",
		sdhci_readl(host, SDHCI_CAPABILITIES),
		sdhci_readl(host, SDHCI_MAX_CURRENT));

	if (host->flags & SDHCI_USE_ADMA)
		printk(KERN_DEBUG DRIVER_NAME ": ADMA Err: 0x%08x | ADMA Ptr: 0x%08x\n",
		       readl(host->ioaddr + SDHCI_ADMA_ERROR),
		       readl(host->ioaddr + SDHCI_ADMA_ADDRESS));

	printk(KERN_DEBUG DRIVER_NAME ": ===========================================\n");
}
*/


/*
 * Get the base clock. Use central clock source for now. Not sure if different
 * clock speed to each dev is allowed
 */
static unsigned long sdhci_get_max_clk(struct sdhci_host *host)
{
	struct sdio_dev *dev = sdhci_priv(host);

	if (dev != NULL)
		return dev->clk_hz; 

	pr_err("unable to obtain sd max clock\n");
	return 0;
}

/*
 * Set the SD voltage through the regulator framework
 */
static int sdhci_set_voltage_signaling(struct sdhci_host *host, int voltage)
{
	int ret = 0;
	struct sdio_dev *dev = sdhci_priv(host);

	if (!dev || !dev->vddo_regulator)
		return -EFAULT;

	/* only allow valid SD volatges */
	if (voltage != 1800000 && voltage != 3300000)
		return -EINVAL;

	ret = regulator_set_voltage(dev->vddo_regulator, voltage, voltage);
	return ret;
}

static unsigned int sdhci_get_timeout_clock(struct sdhci_host *host)
{
	return sdhci_get_max_clk(host);
}

static int bcm_kona_sd_reset(struct sdio_dev *dev)
{
	struct sdhci_host *host = dev->host;
	unsigned int val;
#ifdef CONFIG_ARCH_CAPRI
	unsigned int tries = 10000;
#endif
	unsigned long timeout;

	/* Reset host controller by setting 'Software Reset for All' */
	sdhci_writeb(host, SDHCI_RESET_ALL, SDHCI_SOFTWARE_RESET);

	/* Wait for 100 ms max (100ms timeout is taken from sdhci.c) */
	timeout = jiffies + msecs_to_jiffies(100);

	while (sdhci_readb(host, SDHCI_SOFTWARE_RESET) & SDHCI_RESET_ALL) {
		if (time_is_before_jiffies(timeout)) {
			dev_err(dev->dev, "Error: sd host is in reset!!!\n");
			return -EFAULT;
		}
	}

	/* reset the host using the top level reset */
	val = sdhci_readl(host, KONA_SDHOST_CORECTRL);
	val |= KONA_SDHOST_RESET;
	sdhci_writel(host, val, KONA_SDHOST_CORECTRL);
	do {
		val = sdhci_readl(host, KONA_SDHOST_CORECTRL);
#ifdef CONFIG_ARCH_CAPRI
		if (--tries <= 0)
			break;
#endif
	} while (0 == (val & KONA_SDHOST_RESET));

	/* bring the host out of reset */
	val = sdhci_readl(host, KONA_SDHOST_CORECTRL);
	val &= ~KONA_SDHOST_RESET;

	/* Back-to-Back register write needs a delay of 1ms
	 * at bootup (min 10uS)
	 */
	udelay(1000);
	sdhci_writel(host, val, KONA_SDHOST_CORECTRL);

	return 0;
}

static int bcm_kona_sd_init(struct sdio_dev *dev)
{
	struct sdhci_host *host = dev->host;
	unsigned int val;

	/* enable the interrupt from the IP core */
	val = sdhci_readl(host, KONA_SDHOST_COREIMR);
	val |= KONA_SDHOST_IP;
	sdhci_writel(host, val, KONA_SDHOST_COREIMR);

	/*
	 * Enable DAT3 logic for card detection and enable the AHB clock to the
	 * host
	 */
	val = sdhci_readl(host, KONA_SDHOST_CORECTRL);
	val |= /*KONA_SDHOST_CD_PINCTRL | */ KONA_SDHOST_EN;

	/* Back-to-Back register write needs a delay of 1ms
	 * at bootup (min 10uS)
	 */
	udelay(1000);
	sdhci_writel(host, val, KONA_SDHOST_CORECTRL);

	return 0;
}

/*
 * Software emulation of the SD card insertion/removal. Set insert=1 for insert
 * and insert=0 for removal
 */
static int bcm_kona_sd_card_emulate(struct sdio_dev *dev, int insert)
{
	struct sdhci_host *host = dev->host;
	uint32_t val;
	unsigned long flags;

	/* this function can be called from various contexts including ISR */
	spin_lock_irqsave(&host->lock, flags);

#ifndef CONFIG_ARCH_ISLAND
	sdhci_pltfm_clk_enable(host, 1);
#endif
	/* Back-to-Back register write needs a delay of min 10uS.
	 * We keep 20uS
	 */
	udelay(20);
	val = sdhci_readl(host, KONA_SDHOST_CORESTAT);

	if (insert) {
		if (dev->wp_gpio >= 0) {
			int wp_status = gpio_get_value(dev->wp_gpio);

			if (wp_status)
				val |= KONA_SDHOST_WP;
			else
				val &= ~KONA_SDHOST_WP;
		}

		val |= KONA_SDHOST_CD_SW;
		sdhci_writel(host, val, KONA_SDHOST_CORESTAT);
	} else {
		val &= ~KONA_SDHOST_CD_SW;
		sdhci_writel(host, val, KONA_SDHOST_CORESTAT);
	}

	spin_unlock_irqrestore(&host->lock, flags);

	return 0;
}

static int
proc_card_ctrl_read(char *buffer, char **start, off_t off, int count,
		    int *eof, void *data)
{
	unsigned int len = 0;
	struct sdio_dev *dev = (struct sdio_dev *)data;
	struct sdhci_host *host = dev->host;

	if (off > 0)
		return 0;

	len += sprintf(buffer + len, "SD/MMC card is %s\n",
		       sdhci_readl(host,
				   KONA_SDHOST_CORESTAT) & KONA_SDHOST_CD_SW ?
		       "INSERTED" : "NOT INSERTED");

	return len;
}

static int
proc_card_ctrl_write(struct file *file, const char __user * buffer,
		     unsigned long count, void *data)
{
	int rc, insert;
	struct sdio_dev *dev = (struct sdio_dev *)data;
	unsigned char kbuf[MAX_PROC_BUF_SIZE];

	if (count > MAX_PROC_BUF_SIZE)
		count = MAX_PROC_BUF_SIZE;

	rc = copy_from_user(kbuf, buffer, count);
	if (rc) {
		pr_err("copy_from_user failed status=%d\n", rc);
		return -EFAULT;
	}

	if (sscanf(kbuf, "%d", &insert) != 1) {
		pr_err("echo <insert> > %s\n", PROC_ENTRY_CARD_CTRL);
		return count;
	}

	if (insert) {
		bcm_kona_sd_card_emulate(dev, 1);
		pr_info("Emulated card insert!\n");
	} else {
		bcm_kona_sd_card_emulate(dev, 0);
		pr_info("Emulated card remove!\n");
	}

	return count;
}

static int
proc_regulator_ctrl_read(char *buffer, char **start, off_t off, int count,
		int *eof, void *data)
{
	unsigned int len = 0;
	struct sdio_dev *dev = (struct sdio_dev *)data;
	struct sdhci_host *host = dev->host;

	if (off > 0)
		return 0;

	if (!host)
		return 0;

	len += sprintf(buffer + len, "Current registered regulators are %s %s\n",
			host->vmmc ? "vmmc" : "",
			dev->vddo_regulator ? "vddo" : "");

	return len;
}

/*
 * Initialize the proc entries
 */
static int proc_init(struct platform_device *pdev)
{
	int rc;
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct procfs *proc = &dev->proc;
	struct proc_dir_entry *proc_card_ctrl, *proc_regulator_ctrl;

	snprintf(proc->name, sizeof(proc->name), "%s%d", DEV_NAME, pdev->id);

	proc->parent = proc_mkdir(proc->name, gProcParent);
	if (proc->parent == NULL)
		return -ENOMEM;

	proc_card_ctrl = create_proc_entry(PROC_ENTRY_CARD_CTRL, 0644, proc->parent);
	if (proc_card_ctrl == NULL) {
		rc = -ENOMEM;
		goto proc_exit;
	}
	proc_card_ctrl->read_proc = proc_card_ctrl_read;
	proc_card_ctrl->write_proc = proc_card_ctrl_write;
	proc_card_ctrl->data = dev;

	proc_regulator_ctrl = create_proc_entry(PROC_ENTRY_REGULATOR_CTRL,
			0644, proc->parent);
	if (proc_regulator_ctrl == NULL) {
		rc = -ENOMEM;
		goto err_del_card_ctrl;
	}
	proc_regulator_ctrl->read_proc = proc_regulator_ctrl_read;
	proc_regulator_ctrl->write_proc = NULL;
	proc_regulator_ctrl->data = dev;

	return 0;

err_del_card_ctrl:
	remove_proc_entry(PROC_ENTRY_CARD_CTRL, proc->parent);

	return 0;
proc_exit:
	remove_proc_entry(proc->name, gProcParent);
	return rc;
}

/*
 * Terminate and remove the proc entries
 */
static void proc_term(struct platform_device *pdev)
{
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct procfs *proc = &dev->proc;

	remove_proc_entry(PROC_ENTRY_REGULATOR_CTRL, proc->parent);
	remove_proc_entry(PROC_ENTRY_CARD_CTRL, proc->parent);
	remove_proc_entry(proc->name, gProcParent);
}

/*
 * SD card detection interrupt handler
 */
static irqreturn_t sdhci_pltfm_cd_interrupt(int irq, void *dev_id)
{
	struct sdio_dev *dev = (struct sdio_dev *)dev_id;

	/* card insert */
	if (gpio_get_value_cansleep(dev->cd_gpio) == 0)
		bcm_kona_sd_card_emulate(dev, 1);
	else			/* card removal */
		bcm_kona_sd_card_emulate(dev, 0);

	return IRQ_HANDLED;
}

int sdhci_pltfm_clk_enable(struct sdhci_host *host, int enable)
{
#if defined(CONFIG_ARCH_SAMOA) || defined(CONFIG_MACH_CAPRI_FPGA)
	return 0;
#else
	int ret = 0;
	struct sdio_dev *dev = sdhci_priv(host);
	BUG_ON(!dev);
	if (enable) {
		/* peripheral clock */
		ret = clk_enable(dev->peri_clk);
		if (ret)
			return ret;
	} else {
		clk_disable(dev->peri_clk);
	}
	return ret;
#endif
}

#ifdef CONFIG_BRCM_UNIFIED_DHD_SUPPORT
static void kona_sdio_status_notify_cb(int card_present, void *dev_id)
{
	struct sdhci_host *host;
	int rc;
	struct sdio_dev *dev;

	pr_debug("%s: ENTRY\n", __func__);

	rc = wifi_sdio_is_initialized();
	if (rc <= 0) {
		pr_err("%s: CARD IS NOT INITIALIZED\n", __func__);
		return;
	}
	dev = get_wifi_dev();

	pr_debug("%s: DEV=%p\n", __func__, dev);

	host = dev_id;
	if (host == NULL) {
		pr_err("%s: Invalid host structure pointer\n", __func__);
		return;
	}
	pr_debug("%s: CALL EMULATION=%p\n", __func__, dev);
	if (card_present)
		bcm_kona_sd_card_emulate(dev, 1);
	else
		bcm_kona_sd_card_emulate(dev, 0);

	pr_debug("%s: EMULATION DONE=%p\n", __func__, dev);
	/*
	 * TODO: The required implementtion to check the status of the card
	 * etc
	 */

	/* Call the core function to rescan on the given host controller */
	pr_debug("%s: MMC_DETECT_CHANGE\n", __func__);

	mmc_detect_change(host->mmc, 100);

	pr_debug("%s: MMC_DETECT_CHANGE DONE\n", __func__);
}
#endif

static int __devinit sdhci_pltfm_probe(struct platform_device *pdev)
{
	struct sdhci_host *host;
	struct sdio_dev *dev;
	struct resource *iomem;
	struct sdio_platform_cfg *hw_cfg;
	char devname[MAX_DEV_NAME_SIZE];
	int ret;

	pr_debug("%s: ENTRY\n", __func__);

	BUG_ON(pdev == NULL);

	if (pdev->dev.platform_data == NULL) {
		dev_err(&pdev->dev, "platform_data missing\n");
		ret = -EFAULT;
		goto err;
	}

	pr_debug("%s: GET PLATFORM DATA\n", __func__);

	hw_cfg = (struct sdio_platform_cfg *)pdev->dev.platform_data;
	if (hw_cfg->devtype >= SDIO_DEV_TYPE_MAX) {
		dev_err(&pdev->dev, "unknown device type\n");
		ret = -EFAULT;
		goto err;
	}

	pr_debug("%s: GET PLATFORM RESOURCES\n", __func__);

	iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!iomem) {
		ret = -ENOMEM;
		goto err;
	}

	/* Some PCI-based MFD need the parent here */
	if (pdev->dev.parent != &platform_bus)
		host =
		    sdhci_alloc_host(pdev->dev.parent, sizeof(struct sdio_dev));
	else
		host = sdhci_alloc_host(&pdev->dev, sizeof(struct sdio_dev));
	if (IS_ERR(host)) {
		ret = PTR_ERR(host);
		goto err;
	}

	pr_debug("%s: ALLOC HOST\n", __func__);

	host->hw_name = "bcm_kona_sd";
	host->irq = platform_get_irq(pdev, 0);
	host->quirks = SDHCI_QUIRK_NO_CARD_NO_RESET
	    | SDHCI_QUIRK_BROKEN_TIMEOUT_VAL
	    | SDHCI_QUIRK_32BIT_DMA_ADDR
	    | SDHCI_QUIRK_32BIT_DMA_SIZE | SDHCI_QUIRK_32BIT_ADMA_SIZE;

	pr_debug("%s: GET IRQ\n", __func__);

	if (hw_cfg->flags & KONA_SDIO_FLAGS_DEVICE_NON_REMOVABLE)
		host->mmc->caps |= MMC_CAP_NONREMOVABLE;

	if (!request_mem_region(iomem->start, resource_size(iomem),
				mmc_hostname(host->mmc))) {
		dev_err(&pdev->dev, "cannot request region\n");
		ret = -EBUSY;
		goto err_free_host;
	}

	host->ioaddr = ioremap(iomem->start, resource_size(iomem));
	if (!host->ioaddr) {
		dev_err(&pdev->dev, "failed to remap registers\n");
		ret = -ENOMEM;
		goto err_free_mem_region;
	}

	pr_debug("%s: MEM and IO REGION OKAY\n", __func__);

	dev = sdhci_priv(host);
	dev->dev = &pdev->dev;
	dev->host = host;
	dev->devtype = hw_cfg->devtype;

	host->ops = &dev->ops;
	dev->ops.get_max_clk = sdhci_get_max_clk;
	dev->ops.get_timeout_clock = sdhci_get_timeout_clock;

	dev->cd_gpio = hw_cfg->cd_gpio;
	dev->wp_gpio = hw_cfg->wp_gpio;
	if (dev->devtype == SDIO_DEV_TYPE_WIFI)
		dev->wifi_gpio = &hw_cfg->wifi_gpio;

	pr_debug("%s: DEV TYPE %x\n", __func__, dev->devtype);

	gDevs[pdev->id] = dev;

	platform_set_drvdata(pdev, dev);

	snprintf(devname, sizeof(devname), "%s%d", DEV_NAME, pdev->id);

	/* enable clocks */
#if defined(CONFIG_MACH_BCM2850_FPGA) || defined(CONFIG_MACH_CAPRI_FPGA)
	if (clock) {		/* clock override */
		dev->clk_hz = clock;
	} else {
		dev->clk_hz = gClock[dev->devtype];
	}
#else
	/* peripheral clock */
	dev->peri_clk = clk_get(&pdev->dev, "peri_clk");
	if (IS_ERR_OR_NULL(dev->peri_clk)) {
		ret = -EINVAL;
		goto err_unset_pltfm;
	}
	ret = clk_set_rate(dev->peri_clk, hw_cfg->peri_clk_rate);
	if (ret)
		goto err_peri_clk_put;

	/* sleep clock */
	dev->sleep_clk = clk_get(&pdev->dev, "sleep_clk");
	if (IS_ERR_OR_NULL(dev->sleep_clk)) {
		ret = -EINVAL;
		goto err_peri_clk_put;
	}

	ret = clk_enable(dev->sleep_clk);
	if (ret) {
		dev_err(&pdev->dev, "failed to enable sleep clock for %s\n",
			devname);
		goto err_sleep_clk_put;
	}

	ret = sdhci_pltfm_clk_enable(host, 1);
	if (ret) {
		dev_err(&pdev->dev, "failed to initialize core clock for %s\n",
			devname);
		goto err_sleep_clk_disable;
	}
	dev->clk_hz = clk_get_rate(dev->peri_clk);
#endif

	ret = sdhci_pltfm_regulator_init(pdev, hw_cfg);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to initialize regulator for %s\n",
				devname);
			goto err_term_clk;
	}

	ret = bcm_kona_sd_reset(dev);
	if (ret)
		goto err_term_regulator;

	ret = bcm_kona_sd_init(dev);
	if (ret)
		goto err_reset;

	if (hw_cfg->is_8bit)
		host->mmc->caps |= MMC_CAP_8_BIT_DATA;

	/* Note that sdhci_add_host calls --> mmc_add_host, which in turn
	 * checks for the flag MMC_PM_IGNORE_PM_NOTIFY before registering a PM
	 * notifier for the specific instance of SDIO host controller. For
	 * WiFi case, we don't want to get notified, becuase then from there
	 * mmc_power_off is called which will reset the Host registers that
	 * needs to be re-programmed by starting SDIO handsake again. We want
	 * to prevent this in case of WiFi. So enable MMC_PM_IGNORE_PM_NOTIFY
	 * flag, so that notifier never gets registered.
	 */
	if (dev->devtype == SDIO_DEV_TYPE_WIFI) {
		/* The Wireless LAN drivers call the API sdio_get_host_pm_caps
		 * to know the PM capabilities of the driver, which would
		 * return pm_caps. While the internal code decides based on
		 * pm_flags, the pm_caps also should reflect the same.
		 */
		host->mmc->pm_caps =
		    MMC_PM_KEEP_POWER | MMC_PM_IGNORE_PM_NOTIFY;
		host->mmc->pm_flags =
		    MMC_PM_KEEP_POWER | MMC_PM_IGNORE_PM_NOTIFY;
	}

	/* Enable 1.8V DDR operation for e.MMC */
	if (dev->devtype == SDIO_DEV_TYPE_EMMC)
		host->mmc->caps |= MMC_CAP_1_8V_DDR;

	/*
	 * Temporary UHS support only for eMMC as it does not need volatge level
	 * adjustment. The SD card will not be able to support UHS until the PMU
	 * regulator framework is integrated
	 */
	if (hw_cfg->tmp_uhs)
		host->mmc->caps |= MMC_CAP_1_8V_DDR | MMC_CAP_UHS_DDR50;

	ret = sdhci_add_host(host);
	if (ret)
		goto err_reset;

	ret = proc_init(pdev);
	if (ret)
		goto err_rm_host;

	/* if device is eMMC, emulate card insert right here */
	if (dev->devtype == SDIO_DEV_TYPE_EMMC) {
		ret = bcm_kona_sd_card_emulate(dev, 1);
		if (ret) {
			dev_err(&pdev->dev,
				"unable to emulate card insertion\n");
			goto err_proc_term;
		}
		pr_info("%s: card insert emulated!\n", devname);
	} else if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0) {
		ret = gpio_request(dev->cd_gpio, "sdio cd");

		if (ret < 0) {
			dev_err(&pdev->dev, "Unable to request GPIO pin %d\n",
				dev->cd_gpio);
			goto err_proc_term;
		}
		gpio_direction_input(dev->cd_gpio);

		/* Set debounce for SD Card detect to maximum value (128ms)
		 *
		 * NOTE-1: If gpio_set_debounce() returns error we still
		 * continue with the default debounce value set. Another reason
		 * for doing this is that on rhea-ray boards the SD Detect GPIO
		 * is on GPIO Expander and gpio_set_debounce() will return error
		 * and if we return error from here, then probe() would fail and
		 * SD detection would always fail.
		 *
		 * NOTE-2: We also give a msleep() of the "debounce" time here
		 * so that we give enough time for the debounce to stabilize
		 * before we read the gpio value in gpio_get_value_cansleep().
		 */
		ret =
		    gpio_set_debounce(dev->cd_gpio,
				      (SD_DETECT_GPIO_DEBOUNCE_128MS * 1000));
		if (ret < 0) {
			dev_err(&pdev->dev, "%s: gpio set debounce failed."
				"default debounce value assumed\n", __func__);
		}

		/* Sleep for 128ms to allow debounce to stabilize */
		msleep(SD_DETECT_GPIO_DEBOUNCE_128MS);

		/* request irq for cd_gpio after the gpio debounce is
		 * stabilized, otherwise, some bogus gpio interrupts might be
		 * triggered.
		 */
		ret = request_threaded_irq(gpio_to_irq(dev->cd_gpio),
					   NULL,
					   sdhci_pltfm_cd_interrupt,
					   IRQF_TRIGGER_FALLING |
					   IRQF_TRIGGER_RISING |
					   IRQF_NO_SUSPEND, "sdio cd", dev);
		if (ret) {
			dev_err(&pdev->dev,
				"Unable to request card detection irq=%d"
				" for gpio=%d\n",
				gpio_to_irq(dev->cd_gpio), dev->cd_gpio);
			goto err_free_cd_gpio;
		}

		if (dev->wp_gpio >= 0) {
			ret = gpio_request(dev->wp_gpio, "sdio wp");
			if (ret < 0) {
				dev_err(&pdev->dev, "Unable to request WP pin %d\n", dev->wp_gpio);
				dev->wp_gpio = -1;
			} else {
				gpio_direction_input(dev->wp_gpio);
			}
		}

		/*
		 * Since the card detection GPIO interrupt is configured to be
		 * edge sensitive, check the initial GPIO value here, emulate
		 * only if the card is present
		 */
		if (gpio_get_value_cansleep(dev->cd_gpio) == 0)
			bcm_kona_sd_card_emulate(dev, 1);
	}
#ifdef CONFIG_BRCM_UNIFIED_DHD_SUPPORT
	if ((dev->devtype == SDIO_DEV_TYPE_WIFI) &&
	    (hw_cfg->register_status_notify != NULL)) {
		hw_cfg->register_status_notify(kona_sdio_status_notify_cb,
					       host);
	}
	pr_debug("%s: CALL BACK IS REGISTERED\n", __func__);

#endif

	atomic_set(&dev->initialized, 1);
#ifndef CONFIG_ARCH_ISLAND
	sdhci_pltfm_clk_enable(host, 0);
#endif

	pr_info("%s: initialized properly\n", devname);

	return 0;

err_free_cd_gpio:
	if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0)
		gpio_free(dev->cd_gpio);

err_proc_term:
	proc_term(pdev);

err_rm_host:
	sdhci_remove_host(host, 0);

err_reset:
	bcm_kona_sd_reset(dev);

err_term_regulator:
	sdhci_pltfm_regulator_term(pdev);

err_term_clk:
	sdhci_pltfm_clk_enable(host, 0);

#ifndef CONFIG_MACH_BCM2850_FPGA
err_sleep_clk_disable:
	clk_disable(dev->sleep_clk);

err_sleep_clk_put:
	clk_put(dev->sleep_clk);

err_peri_clk_put:
	clk_put(dev->peri_clk);
#endif

err_unset_pltfm:
	platform_set_drvdata(pdev, NULL);
	iounmap(host->ioaddr);

err_free_mem_region:
	release_mem_region(iomem->start, resource_size(iomem));

err_free_host:
	sdhci_free_host(host);

err:
	pr_err("Probing of sdhci-pltfm %d failed: %d\n", pdev->id,
	       ret);
	return ret;
}

static void sdhci_pltfm_shutdown(struct platform_device *pdev)
{
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct sdhci_host *host = dev->host;
	int dead;
	u32 scratch;

	atomic_set(&dev->initialized, 0);
	gDevs[pdev->id] = NULL;

	if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0) {
		free_irq(gpio_to_irq(dev->cd_gpio), dev);
		gpio_free(dev->cd_gpio);
		if (dev->wp_gpio >= 0) {
			gpio_free(dev->wp_gpio);
			dev->wp_gpio = -1;
		}
	}

	proc_term(pdev);

	sdhci_pltfm_clk_enable(host, 1);
	dead = 0;
	scratch = readl(host->ioaddr + SDHCI_INT_STATUS);
	if (scratch == (u32) -1)
		dead = 1;
	sdhci_remove_host(host, dead);

	sdhci_pltfm_clk_enable(host, 0);

#ifndef CONFIG_MACH_BCM2850_FPGA
	clk_disable(dev->sleep_clk);
	clk_put(dev->sleep_clk);
	clk_put(dev->peri_clk);
#endif

	sdhci_pltfm_regulator_term(pdev);
}

static int __devexit sdhci_pltfm_remove(struct platform_device *pdev)
{
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct sdhci_host *host = dev->host;
	struct resource *iomem = platform_get_resource(pdev, IORESOURCE_MEM, 0);

	sdhci_pltfm_shutdown(pdev);

	platform_set_drvdata(pdev, NULL);
	kfree(dev);
	iounmap(host->ioaddr);
	release_mem_region(iomem->start, resource_size(iomem));
	sdhci_free_host(host);

	return 0;
}

#ifdef CONFIG_PM
static int sdhci_pltfm_suspend(struct platform_device *pdev, pm_message_t state)
{
	int ret;
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct sdhci_host *host = dev->host;

#if 0
	if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0)
		free_irq(gpio_to_irq(dev->cd_gpio), dev);
#endif

	ret = sdhci_suspend_host(host, state);
	if (ret) {
		dev_err(&pdev->dev, "Unable to suspend sdhci host err=%d\n",
			ret);
		return ret;
	}

	if (dev->vddo_regulator)
		regulator_disable(dev->vddo_regulator);

	return 0;
}

static int sdhci_pltfm_resume(struct platform_device *pdev)
{
	int ret;
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct sdhci_host *host = dev->host;

	if (dev->vddo_regulator) {
		ret= regulator_enable(dev->vddo_regulator);
		if (ret) {
			dev_err(&pdev->dev, "Unable to enable vddo regulator\n");
			return ret;
		}

		/* set voltage back to 3.3 V */
		ret = regulator_set_voltage(dev->vddo_regulator,
				3300000, 3300000);
		if (ret < 0) {
			dev_err(&pdev->dev, "Unable to set vddo regulator to 3.3V\n");
			return ret;
		}
	}

	ret = sdhci_resume_host(host);
	if (ret) {
		dev_err(&pdev->dev, "Unable to resume sdhci host err=%d\n",
			ret);
		return ret;
	}
#if 0
	if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0) {
		ret =
		    request_irq(gpio_to_irq(dev->cd_gpio),
				sdhci_pltfm_cd_interrupt,
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				"sdio cd", dev);
		if (ret) {
			dev_err(&pdev->dev, "Unable to request card detection "
				"irq=%d for gpio=%d\n",
				gpio_to_irq(dev->cd_gpio), dev->cd_gpio);
			return ret;
		}
	}
#endif

#ifndef CONFIG_MMC_UNSAFE_RESUME
	/*
	 * card state might have been changed during system suspend.
	 * Need to sync up only if MMC_UNSAFE_RESUME is not enabled
	 */
	if (dev->devtype == SDIO_DEV_TYPE_SDMMC && dev->cd_gpio >= 0) {
		if (gpio_get_value_cansleep(dev->cd_gpio) == 0)
			bcm_kona_sd_card_emulate(dev, 1);
		else
			bcm_kona_sd_card_emulate(dev, 0);
	}
#endif
	return 0;
}
#else
#define sdhci_pltfm_suspend NULL
#define sdhci_pltfm_resume NULL
#endif /* CONFIG_PM */

static struct platform_driver sdhci_pltfm_driver = {
	.driver = {
		   .name = "sdhci",
		   .owner = THIS_MODULE,
		   },
	.probe = sdhci_pltfm_probe,
	.remove = __devexit_p(sdhci_pltfm_remove),
	.shutdown = sdhci_pltfm_shutdown,
	.suspend = sdhci_pltfm_suspend,
	.resume = sdhci_pltfm_resume,
};

static int __init sdhci_drv_init(void)
{
	int rc;

	gProcParent = proc_mkdir(PROC_GLOBAL_PARENT_DIR, NULL);
	if (gProcParent == NULL) {
		pr_err("%s: sdhci platform procfs install failed\n", __func__);
		return -ENOMEM;
	}

	rc = platform_driver_register(&sdhci_pltfm_driver);
	if (rc < 0) {
		pr_err("%s: sdhci_drv_init failed\n", __func__);
		remove_proc_entry(PROC_GLOBAL_PARENT_DIR, NULL);
		return rc;
	}

	return 0;
}

static void __exit sdhci_drv_exit(void)
{
	remove_proc_entry(PROC_GLOBAL_PARENT_DIR, NULL);
	platform_driver_unregister(&sdhci_pltfm_driver);
}

fs_initcall(sdhci_drv_init);
module_exit(sdhci_drv_exit);

MODULE_DESCRIPTION("Secure Digital Host Controller Interface platform driver");
MODULE_AUTHOR("Mocean Laboratories <info@mocean-labs.com>");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:sdhci");

static struct sdio_dev *get_wifi_dev(void)
{
	int dev;

	for (dev = 0; dev < SDIO_MAX_NUM_DEVICES; dev++) {
		if (gDevs[dev] != NULL) {
			if (gDevs[dev]->devtype == SDIO_DEV_TYPE_WIFI)
				return gDevs[dev];
		}
	}

	return NULL;
}

int wifi_sdio_is_initialized(void)
{
	struct sdio_dev *dev;

	dev = get_wifi_dev();
	if (dev == NULL)
		return 0;

	return atomic_read(&dev->initialized);
}
EXPORT_SYMBOL(wifi_sdio_is_initialized);

struct mmc_card *wifi_sdio_get_card(void)
{
	int rc;
	struct sdio_dev *dev;

	rc = wifi_sdio_is_initialized();
	if (rc <= 0)
		return NULL;

	dev = get_wifi_dev();
	if (dev == NULL)
		return NULL;

	return dev->host->mmc->card;
}
EXPORT_SYMBOL(wifi_sdio_get_card);

struct sdio_wifi_gpio_cfg *wifi_sdio_get_gpio(void)
{
	int rc;
	struct sdio_dev *dev;

	rc = wifi_sdio_is_initialized();
	if (rc <= 0)
		return NULL;

	dev = get_wifi_dev();
	if (dev == NULL)
		return NULL;

	return dev->wifi_gpio;
}
EXPORT_SYMBOL(wifi_sdio_get_gpio);

int wifi_sdio_card_emulate(int insert)
{
	int rc;
	struct sdio_dev *dev;

	rc = wifi_sdio_is_initialized();
	if (rc <= 0)
		return -EAGAIN;

	dev = get_wifi_dev();
	if (dev == NULL)
		return -EAGAIN;

	return bcm_kona_sd_card_emulate(dev, insert);
}
EXPORT_SYMBOL(wifi_sdio_card_emulate);

int sdio_stop_clk(enum sdio_devtype devtype, int insert)
{
	int rc;
	struct sdio_dev *dev;
	struct sdhci_host *host;

	rc = wifi_sdio_is_initialized();
	if (rc <= 0)
		return -EFAULT;

#ifndef CONFIG_ARCH_ISLAND
	dev = get_wifi_dev();
	host = dev->host;

	sdhci_pltfm_clk_enable(host, insert);
#endif
	return 0;
}
EXPORT_SYMBOL(sdio_stop_clk);

static int sdhci_pltfm_regulator_init(struct platform_device *pdev,
		struct sdio_platform_cfg *hw_cfg)
{
	int ret;
	struct sdio_dev *dev = platform_get_drvdata(pdev);
	struct sdhci_host *host = dev->host;
	char devname[MAX_DEV_NAME_SIZE];

	snprintf(devname, sizeof(devname), "%s%d", DEV_NAME, pdev->id);

	/* VMMC */
	host->vmmc = regulator_get(&pdev->dev, "vmmc");
	if (IS_ERR(host->vmmc)) {
		pr_err("Unable to get vmmc regulator, err: %ld\n",
				PTR_ERR(host->vmmc));
		host->vmmc = NULL;
		return 0;
	}

	ret = regulator_enable(host->vmmc);
	if (ret < 0) {
		printk(KERN_ERR "Unable to enable vmmc regulator\n");
		goto err_put_vmmc_reg;
	}

	printk(KERN_INFO "Found and enabled vmmc regulator for %s\n", devname);

	/* VDDO */
	dev->vddo_regulator = regulator_get(&pdev->dev, "vddo");
	if (IS_ERR(dev->vddo_regulator)) {
		pr_err("Unable to get vddo regulator, err: %ld\n",
				PTR_ERR(dev->vddo_regulator));
		dev->vddo_regulator = NULL;
		ret = 0;
		goto err_disable_vmmc_reg;
	}

	ret = regulator_enable(dev->vddo_regulator);
	if (ret < 0) {
		printk(KERN_ERR "Unable to enable vddo regulator\n");
		goto err_put_vddo_reg;
	}

	/* set to 3.3V by default */
	ret = regulator_set_voltage(dev->vddo_regulator, 3300000, 3300000);
	if (ret < 0) {
		printk(KERN_ERR "Unable to set vddo regulator to 3.3V\n");
		goto err_disable_vddo_reg;
	}

	/* now vddo regulator is initialized properly, register the callback */
	if (hw_cfg->need_regulator_uhs)
		dev->ops.set_voltage_signaling = sdhci_set_voltage_signaling;

	printk(KERN_INFO "Found and enabled vddo regulator for %s\n", devname);

	return 0;

err_disable_vddo_reg:
	if (dev->vddo_regulator)
		regulator_disable(dev->vddo_regulator);

err_put_vddo_reg:
	if (dev->vddo_regulator) {
		regulator_put(dev->vddo_regulator);
		dev->vddo_regulator = NULL;
	}

err_disable_vmmc_reg:
	if (host->vmmc)
		regulator_disable(host->vmmc);

err_put_vmmc_reg:
	if (host->vmmc) {
		regulator_put(host->vmmc);
		host->vmmc = NULL;
	}

	return ret;
}

static void sdhci_pltfm_regulator_term(struct platform_device *pdev)
{
	struct sdio_dev *dev = platform_get_drvdata(pdev);

	if (dev->vddo_regulator) {
		dev->ops.set_voltage_signaling = NULL;
		regulator_disable(dev->vddo_regulator);
		regulator_put(dev->vddo_regulator);
		dev->vddo_regulator = NULL;
	}

	/* host vmmc is already disabled in the sdhci core driver */
}
