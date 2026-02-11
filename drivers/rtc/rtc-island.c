/* drivers/rtc/rtc-bcmhana.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * BCMHANA RTC Driver
 */

/*
 * Frameworks:
 *
 *    - SMP:          Fully supported.    Locking is in place where necessary.
 *    - GPIO:         Fully supported.    No GPIOs are used.
 *    - MMU:          Partiall done.      CHAL layer is broken needs interface like kona keypad
 *    - Dynamic /dev: Not applicable.
 *    - Suspend:      Partially implemented.
 *    - Clocks:       Done.
 *    - Power:        Not done.
 *
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/rtc.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/errno.h>

#include <chal/chal_rtc.h>

struct island_rtc {
	CHAL_RTC_HANDLE_t *handle;
	struct rtc_device *dev;
	unsigned int irq1;
	unsigned int irq2;
	unsigned int set;
	struct clk *clock;
};
struct island_rtc *rtc;
CHAL_RTC_HANDLE_t foo;

static unsigned int epoch = 1970;
static DEFINE_SPINLOCK(island_rtc_lock);

/* IRQ Handlers */

/*
 * RTC IRQ hanlder. This routine is invoked when a RTC oneshot timer completes
 */
static irqreturn_t rtc_alm_isr(int irq, void *data)
{
	struct island_rtc *rdev = data;

	/* Disable alarm interrupts because they are oneshot */
	chal_rtc_intDisable(rtc->handle, CHAL_RTC_INT_MATCH);
	chal_rtc_intStatusClr(rtc->handle, CHAL_RTC_INT_MATCH);
	if (chal_rtc_matchInterruptValGet(rtc->handle)) {
		pr_debug("%s: oneshot interrupted\n", __func__);
		rtc_update_irq(rdev->dev, 1, RTC_AF | RTC_IRQF);
	}
	rtc->set = 0;
	return IRQ_HANDLED;
}

/* Update control registers */
static int island_rtc_setaie(struct device *dev, unsigned int to)
{
	pr_debug("%s: aie=%d\n", __func__, to);

	spin_lock_irq(&island_rtc_lock);

	if (to) {
		chal_rtc_intEnable(rtc->handle, CHAL_RTC_INT_MATCH);
	} else {
		chal_rtc_intDisable(rtc->handle, CHAL_RTC_INT_MATCH);
	}
	spin_unlock_irq(&island_rtc_lock);
        return 0;
}

/* Time read/write */

static int island_rtc_gettime(struct device *dev, struct rtc_time *rtc_tm)
{
	unsigned int epoch_sec, elapsed_sec;

	epoch_sec = mktime(epoch, 1, 1, 0, 0, 0);

	/* CHAL/Hardware issue: Read until we get a non-zero value. */
        while (!(elapsed_sec = chal_rtc_secGet(rtc->handle)))

	pr_debug("%s: epoch_sec=%u, elapsed_sec=%u\n", __func__, epoch_sec,
		 elapsed_sec);
	rtc_time_to_tm(epoch_sec + elapsed_sec, rtc_tm);

	pr_debug("read time 0x%02x.0x%02x.0x%02x 0x%02x/0x%02x/0x%02x\n",
		 rtc_tm->tm_year, rtc_tm->tm_mon, rtc_tm->tm_mday,
		 rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);

	return 0;
}

static int island_rtc_settime(struct device *dev, struct rtc_time *time)
{
	unsigned int epoch_sec, current_sec;

	epoch_sec = mktime(epoch, 1, 1, 0, 0, 0);
	current_sec =
	    mktime(time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
		   time->tm_hour, time->tm_min, time->tm_sec);

	chal_rtc_secSet(rtc->handle, current_sec - epoch_sec);
	chal_rtc_ctrlSet(rtc->handle, CHAL_RTC_CTRL_RUN);

	pr_debug("%s: current_sec=%u, epoch_sec=%u\n", __func__, current_sec,
		 epoch_sec);

	pr_debug("set time %02d.%02d.%02d %02d/%02d/%02d\n", time->tm_year,
		 time->tm_mon, time->tm_mday, time->tm_hour, time->tm_min,
		 time->tm_sec);

	return 0;
}

static int island_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	unsigned int epoch_sec, elapsed_sec, alarm_elapsed_sec;
	chal_rtc_TIME_t alm_reg_secs;
	struct rtc_time *alm_tm = &alrm->time;
	alrm->enabled = chal_rtc_intIsEnabled(rtc->handle, CHAL_RTC_INT_MATCH);

	epoch_sec = mktime(epoch, 1, 1, 0, 0, 0);
	elapsed_sec = chal_rtc_secGet(rtc->handle);

	alm_reg_secs = chal_rtc_matchInterruptValGet(rtc->handle);

	/* Handle carry over */
	if ((elapsed_sec & 0x0ffff) > alm_reg_secs) {
		elapsed_sec += 0x10000;
	}
	elapsed_sec &= ~0xffff;	/* clear lower 16 bits for 16-bit alarm match register below */
	alarm_elapsed_sec = elapsed_sec + alm_reg_secs;
	pr_debug
	    ("%s: epoch_sec=%u, elapsed_sec=%u, alm_reg_secs=%lu=0x%lx, alarm_elapsed_sec=%u=0x%x\n",
	     __func__, epoch_sec, elapsed_sec, (unsigned long)alm_reg_secs,
	     (unsigned long)alm_reg_secs, alarm_elapsed_sec, alarm_elapsed_sec);

	rtc_time_to_tm(epoch_sec + alarm_elapsed_sec, alm_tm);
	pr_debug("read alarm %02x %02x.%02x.%02x %02x/%02x/%02x\n",
		 alrm->enabled, alm_tm->tm_year, alm_tm->tm_mon,
		 alm_tm->tm_mday, alm_tm->tm_hour, alm_tm->tm_min,
		 alm_tm->tm_sec);

	return 0;
}

static int island_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	unsigned int epoch_sec, elapsed_sec;
	struct rtc_time *time = &alrm->time;
	chal_rtc_TIME_t alm_secs;

	pr_debug("%s: %d, %02x/%02x/%02x %02x.%02x.%02x\n",
		 __func__, alrm->enabled, time->tm_mday & 0xff,
		 time->tm_mon & 0xff, time->tm_year & 0xff,
		 time->tm_hour & 0xff, time->tm_min & 0xff, time->tm_sec);

	epoch_sec = mktime(epoch, 1, 1, 0, 0, 0);
	elapsed_sec = chal_rtc_secGet(rtc->handle);
	alm_secs =
	    mktime(time->tm_year + 1900, time->tm_mon + 1, time->tm_mday,
		   time->tm_hour, time->tm_min, time->tm_sec);

	pr_debug("%s: epoch_sec=%u, elapsed_sec=%u, alm_secs=%lu\n", __func__,
		 epoch_sec, elapsed_sec, (unsigned long)alm_secs);

	spin_lock_irq(&island_rtc_lock);

	chal_rtc_intDisable(rtc->handle, CHAL_RTC_INT_MATCH);
	chal_rtc_intStatusClr(rtc->handle, CHAL_RTC_INT_MATCH);

	chal_rtc_matchInterruptValSet(rtc->handle, alm_secs);

	if (alrm->enabled) {
		chal_rtc_intEnable(rtc->handle, CHAL_RTC_INT_MATCH);
	}

	rtc->set = 1;
	spin_unlock_irq(&island_rtc_lock);

	return 0;
}

static int island_rtc_proc(struct device *dev, struct seq_file *seq)
{
	seq_printf(seq, "\nperiodic timer: 0x%x\n",
		   chal_rtc_readReg(rtc->handle, RTC_PERIODIC_TIMER_ADDR));
	seq_printf(seq, "match register: 0x%x\n",
		   chal_rtc_readReg(rtc->handle, RTC_MATCH_REGISTER_ADDR));
	seq_printf(seq, "clear intr register: 0x%x\n",
		   chal_rtc_readReg(rtc->handle, RTC_CLEAR_INTR_ADDR));
	seq_printf(seq, "intr status register: 0x%x\n",
		   chal_rtc_readReg(rtc->handle, RTC_INTERRUPT_STATUS_ADDR));
	seq_printf(seq, "control addr register: 0x%x\n",
		   chal_rtc_readReg(rtc->handle, RTC_CONTROL_ADDR));
	return 0;
}

static const struct rtc_class_ops island_rtcops = {
	.proc = island_rtc_proc,
	.read_time = island_rtc_gettime,
	.read_alarm = island_rtc_getalarm,
	.set_time = island_rtc_settime,
	.set_alarm = island_rtc_setalarm,
	.alarm_irq_enable = island_rtc_setaie,
};

static void island_rtc_enable(struct platform_device *pdev, int en)
{
	if (!en) {
		chal_rtc_ctrlSet(rtc->handle, CHAL_RTC_CTRL_STOP);
	} else {
		chal_rtc_ctrlSet(rtc->handle, CHAL_RTC_CTRL_RUN);
	}
}

static int __exit island_rtc_remove(struct platform_device *dev)
{
	rtc_device_unregister(rtc->dev);
	device_init_wakeup(&dev->dev, 0);

	platform_set_drvdata(dev, NULL);

	island_rtc_setaie(NULL, 0);

	free_irq(rtc->irq2, rtc);
	free_irq(rtc->irq1, rtc);

	clk_disable(rtc->clock);
	clk_put(rtc->clock);

	return 0;
}

static int __devinit island_rtc_probe(struct platform_device *dev)
{
	void __iomem *base;
	struct resource *res;
	int ret;

	/* We only accept one device, and it must have an id of -1 */
	if (dev->id != -1)
		return -ENODEV;

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENODEV;
		goto err_out;
	}

	rtc = devm_kzalloc(&dev->dev, sizeof(struct island_rtc), GFP_KERNEL);
	if (!rtc) {
		ret = -ENOMEM;
		goto err_out;
	}

        rtc->clock = clk_get(&dev->dev, dev->dev.platform_data);
	if (rtc->clock < 0) {
		ret = -ENXIO;
		goto err_out;
	}

	rtc->irq1 = platform_get_irq(dev, 0);
	if (rtc->irq1 < 0) {
		ret = -ENXIO;
		goto err_out;
	}
	rtc->irq2 = platform_get_irq(dev, 1);
	if (rtc->irq2 < 0) {
		ret = -ENXIO;
		goto err_out;
	}

	base = ioremap(res->start, resource_size(res));
	if (!base) {
		ret = -ENOMEM;
		goto err_out;
	}

	rtc->handle = &foo;
	rtc->set = 0;
	chal_rtc_init(rtc->handle, (uint32_t)base, RTC_BASE_ADDR );

	island_rtc_enable(dev, 1);

	device_init_wakeup(&dev->dev, 1);

	chal_rtc_intDisable(rtc->handle, CHAL_RTC_INT_MATCH);
	chal_rtc_intDisable(rtc->handle, CHAL_RTC_INT_PER);
	chal_rtc_intStatusClr(rtc->handle, CHAL_RTC_INT_MATCH);
	chal_rtc_intStatusClr(rtc->handle, CHAL_RTC_INT_PER);

	rtc->dev =
	    rtc_device_register("island", &dev->dev, &island_rtcops,
				THIS_MODULE);

	if (IS_ERR(rtc->dev)) {
		ret = PTR_ERR(rtc);
		pr_debug("cannot attach rtc\n");
		goto err_device_unregister;
	}

	/* I think there might be a memory corruption bug in the RTC framework.   */
        /* When device_register is called in the bod og rtc_device_register parts */
        /* of the rtc_device staructure are corrupted.                            */
        rtc->dev->irq_task = NULL;

        platform_set_drvdata(dev, rtc->dev);

	ret = request_irq(rtc->irq1, rtc_alm_isr, IRQF_NO_SUSPEND, "island_rtc_alarm", rtc);
	if (ret) {
		dev_printk(KERN_ERR, &rtc->dev->dev,
			   "cannot register IRQ%d for alarm rtc\n",
			   rtc->irq1);
		goto err_irq;
	}

	chal_rtc_ctrlSet(rtc->handle, CHAL_RTC_CTRL_RUN);
	clk_enable(rtc->clock);

	printk(KERN_INFO "RTC: driver initialized properly\n");

	return 0;

 err_irq:
	free_irq(rtc->irq1, rtc);
	free_irq(rtc->irq2, rtc);
 err_device_unregister:
	iounmap(base);
 err_out:
	return ret;
}

#ifdef CONFIG_PM
static int island_rtc_suspend(struct platform_device *dev, pm_message_t msg)
{
        struct island_rtc *rtc = platform_get_drvdata(dev);
	if (!rtc->set) {
		island_rtc_enable(dev, 0);
		clk_disable(rtc->clock);
	}
        return 0;
}

static int island_rtc_resume(struct platform_device *dev)
{
        struct island_rtc *rtc = platform_get_drvdata(dev);
	if (!rtc->set) {
		clk_enable(rtc->clock);
		island_rtc_enable(dev, 1);
	}
	return 0;
}
#else
#define island_rtc_suspend    NULL
#define island_rtc_resume     NULL
#endif

static struct platform_driver island_rtcdrv = {
	.remove = __exit_p(island_rtc_remove),
	.suspend        = island_rtc_suspend,
        .resume         = island_rtc_resume,
	.driver = {
		.name = "island-rtc",
		.owner = THIS_MODULE,
	},
};

static char __initdata banner[] =
    "Island RTC Driver, (c) 2011 Broadcom Corporation\n";

static int __init island_rtc_init(void)
{
	printk(banner);
	return platform_driver_probe(&island_rtcdrv, island_rtc_probe);
}

static void __exit island_rtc_exit(void)
{
	platform_driver_unregister(&island_rtcdrv);
}

module_init(island_rtc_init);
module_exit(island_rtc_exit);

MODULE_DESCRIPTION("Broadcom Island RTC Driver");
MODULE_AUTHOR("Broadcom Corporation");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:island-rtc");
