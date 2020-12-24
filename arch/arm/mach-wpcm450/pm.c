/* linux/arch/arm/mach-wpcm450/pm.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * WPCM450 Power Manager (Suspend-To-RAM) support
 *
 * See Documentation/arm/Winbond-WPCM450/Suspend.txt for more information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Parts based on arch/arm/mach-pxa/pm.c
 *
 * Thanks to Dimitry Andric for debugging
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
*/

#include <linux/config.h>
#include <linux/init.h>
#include <linux/suspend.h>
#include <linux/errno.h>
#include <linux/time.h>
#include <linux/interrupt.h>
#include <linux/crc32.h>
#include <linux/ioport.h>
#include <linux/delay.h>

#include <asm/hardware.h>
#include <asm/io.h>

#include <asm/arch/regs-serial.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-gpio.h>
#include <asm/arch/regs-mem.h>
#include <asm/arch/regs-irq.h>

#include <asm/mach/time.h>

#include "pm.h"

/* for external use */

unsigned long wpcm__pm_flags;

/* cache functions from arch/arm/mm/proc-arm926.S */

extern void arm926_flush_kern_cache_all(void);

#if 0//clyu
#define PFX "s3c24xx-pm: "

static struct sleep_save core_save[] = {
	SAVE_ITEM(wpcm450_LOCKTIME),
	SAVE_ITEM(wpcm450_CLKCON),

	/* we restore the timings here, with the proviso that the board
	 * brings the system up in an slower, or equal frequency setting
	 * to the original system.
	 *
	 * if we cannot guarantee this, then things are going to go very
	 * wrong here, as we modify the refresh and both pll settings.
	 */

	SAVE_ITEM(wpcm450_BWSCON),
	SAVE_ITEM(wpcm450_BANKCON0),
	SAVE_ITEM(wpcm450_BANKCON1),
	SAVE_ITEM(wpcm450_BANKCON2),
	SAVE_ITEM(wpcm450_BANKCON3),
	SAVE_ITEM(wpcm450_BANKCON4),
	SAVE_ITEM(wpcm450_BANKCON5),

	SAVE_ITEM(wpcm450_CLKDIVN),
	SAVE_ITEM(wpcm450_MPLLCON),
	SAVE_ITEM(wpcm450_UPLLCON),
	SAVE_ITEM(wpcm450_CLKSLOW),
	SAVE_ITEM(wpcm450_REFRESH),
};

/* this lot should be really saved by the IRQ code */
static struct sleep_save irq_save[] = {
	SAVE_ITEM(wpcm450_EXTINT0),
	SAVE_ITEM(wpcm450_EXTINT1),
	SAVE_ITEM(wpcm450_EXTINT2),
	SAVE_ITEM(wpcm450_EINFLT0),
	SAVE_ITEM(wpcm450_EINFLT1),
	SAVE_ITEM(wpcm450_EINFLT2),
	SAVE_ITEM(wpcm450_EINFLT3),
	SAVE_ITEM(wpcm450_EINTMASK),
	SAVE_ITEM(wpcm450_INTMSK)
};

static struct sleep_save gpio_save[] = {
	SAVE_ITEM(wpcm450_GPACON),
	SAVE_ITEM(wpcm450_GPADAT),

	SAVE_ITEM(wpcm450_GPBCON),
	SAVE_ITEM(wpcm450_GPBDAT),
	SAVE_ITEM(wpcm450_GPBUP),

	SAVE_ITEM(wpcm450_GPCCON),
	SAVE_ITEM(wpcm450_GPCDAT),
	SAVE_ITEM(wpcm450_GPCUP),

	SAVE_ITEM(wpcm450_GPDCON),
	SAVE_ITEM(wpcm450_GPDDAT),
	SAVE_ITEM(wpcm450_GPDUP),

	SAVE_ITEM(wpcm450_GPECON),
	SAVE_ITEM(wpcm450_GPEDAT),
	SAVE_ITEM(wpcm450_GPEUP),

	SAVE_ITEM(wpcm450_GPFCON),
	SAVE_ITEM(wpcm450_GPFDAT),
	SAVE_ITEM(wpcm450_GPFUP),

	SAVE_ITEM(wpcm450_GPGCON),
	SAVE_ITEM(wpcm450_GPGDAT),
	SAVE_ITEM(wpcm450_GPGUP),

	SAVE_ITEM(wpcm450_GPHCON),
	SAVE_ITEM(wpcm450_GPHDAT),
	SAVE_ITEM(wpcm450_GPHUP),

	SAVE_ITEM(wpcm450_DCLKCON),
};

#ifdef CONFIG_wpcm450_PM_DEBUG

#define SAVE_UART(va) \
	SAVE_ITEM((va) + wpcm450_ULCON), \
	SAVE_ITEM((va) + wpcm450_UCON), \
	SAVE_ITEM((va) + wpcm450_UFCON), \
	SAVE_ITEM((va) + wpcm450_UMCON), \
	SAVE_ITEM((va) + wpcm450_UBRDIV)

static struct sleep_save uart_save[] = {
	SAVE_UART(S3C24XX_VA_UART0),
	SAVE_UART(S3C24XX_VA_UART1),
#ifndef CONFIG_CPU_S3C2400
	SAVE_UART(S3C24XX_VA_UART2),
#endif
};

/* debug
 *
 * we send the debug to printascii() to allow it to be seen if the
 * system never wakes up from the sleep
*/

extern void printascii(const char *);

static void pm_dbg(const char *fmt, ...)
{
	va_list va;
	char buff[256];

	va_start(va, fmt);
	vsprintf(buff, fmt, va);
	va_end(va);

	printascii(buff);
}

static void wpcm450_pm_debug_init(void)
{
	unsigned long tmp = __raw_readl(wpcm450_CLKCON);

	/* re-start uart clocks */
	tmp |= wpcm450_CLKCON_UART0;
	tmp |= wpcm450_CLKCON_UART1;
	tmp |= wpcm450_CLKCON_UART2;

	__raw_writel(tmp, wpcm450_CLKCON);
	udelay(10);
}

#define DBG(fmt...) pm_dbg(fmt)
#else
#define DBG(fmt...) printk(KERN_DEBUG fmt)

#define wpcm450_pm_debug_init() do { } while(0)

static struct sleep_save uart_save[] = {};
#endif

#if defined(CONFIG_wpcm450_PM_CHECK) && CONFIG_wpcm450_PM_CHECK_CHUNKSIZE != 0

/* suspend checking code...
 *
 * this next area does a set of crc checks over all the installed
 * memory, so the system can verify if the resume was ok.
 *
 * CONFIG_wpcm450_PM_CHECK_CHUNKSIZE defines the block-size for the CRC,
 * increasing it will mean that the area corrupted will be less easy to spot,
 * and reducing the size will cause the CRC save area to grow
*/

#define CHECK_CHUNKSIZE (CONFIG_wpcm450_PM_CHECK_CHUNKSIZE * 1024)

static u32 crc_size;	/* size needed for the crc block */
static u32 *crcs;	/* allocated over suspend/resume */

typedef u32 *(run_fn_t)(struct resource *ptr, u32 *arg);

/* wpcm450_pm_run_res
 *
 * go thorugh the given resource list, and look for system ram
*/

static void wpcm450_pm_run_res(struct resource *ptr, run_fn_t fn, u32 *arg)
{
	while (ptr != NULL) {
		if (ptr->child != NULL)
			wpcm450_pm_run_res(ptr->child, fn, arg);

		if ((ptr->flags & IORESOURCE_MEM) &&
		    strcmp(ptr->name, "System RAM") == 0) {
			DBG("Found system RAM at %08lx..%08lx\n",
			    ptr->start, ptr->end);
			arg = (fn)(ptr, arg);
		}

		ptr = ptr->sibling;
	}
}

static void wpcm450_pm_run_sysram(run_fn_t fn, u32 *arg)
{
	wpcm450_pm_run_res(&iomem_resource, fn, arg);
}

static u32 *wpcm450_pm_countram(struct resource *res, u32 *val)
{
	u32 size = (u32)(res->end - res->start)+1;

	size += CHECK_CHUNKSIZE-1;
	size /= CHECK_CHUNKSIZE;

	DBG("Area %08lx..%08lx, %d blocks\n", res->start, res->end, size);

	*val += size * sizeof(u32);
	return val;
}

/* wpcm450_pm_prepare_check
 *
 * prepare the necessary information for creating the CRCs. This
 * must be done before the final save, as it will require memory
 * allocating, and thus touching bits of the kernel we do not
 * know about.
*/

static void wpcm450_pm_check_prepare(void)
{
	crc_size = 0;

	wpcm450_pm_run_sysram(wpcm450_pm_countram, &crc_size);

	DBG("wpcm450_pm_prepare_check: %u checks needed\n", crc_size);

	crcs = kmalloc(crc_size+4, GFP_KERNEL);
	if (crcs == NULL)
		printk(KERN_ERR "Cannot allocated CRC save area\n");
}

static u32 *wpcm450_pm_makecheck(struct resource *res, u32 *val)
{
	unsigned long addr, left;

	for (addr = res->start; addr < res->end;
	     addr += CHECK_CHUNKSIZE) {
		left = res->end - addr;

		if (left > CHECK_CHUNKSIZE)
			left = CHECK_CHUNKSIZE;

		*val = crc32_le(~0, phys_to_virt(addr), left);
		val++;
	}

	return val;
}

/* wpcm450_pm_check_store
 *
 * compute the CRC values for the memory blocks before the final
 * sleep.
*/

static void wpcm450_pm_check_store(void)
{
	if (crcs != NULL)
		wpcm450_pm_run_sysram(wpcm450_pm_makecheck, crcs);
}

/* in_region
 *
 * return TRUE if the area defined by ptr..ptr+size contatins the
 * what..what+whatsz
*/

static inline int in_region(void *ptr, int size, void *what, size_t whatsz)
{
	if ((what+whatsz) < ptr)
		return 0;

	if (what > (ptr+size))
		return 0;

	return 1;
}

static u32 *wpcm450_pm_runcheck(struct resource *res, u32 *val)
{
	void *save_at = phys_to_virt(wpcm450_sleep_save_phys);
	unsigned long addr;
	unsigned long left;
	void *ptr;
	u32 calc;

	for (addr = res->start; addr < res->end;
	     addr += CHECK_CHUNKSIZE) {
		left = res->end - addr;

		if (left > CHECK_CHUNKSIZE)
			left = CHECK_CHUNKSIZE;

		ptr = phys_to_virt(addr);

		if (in_region(ptr, left, crcs, crc_size)) {
			DBG("skipping %08lx, has crc block in\n", addr);
			goto skip_check;
		}

		if (in_region(ptr, left, save_at, 32*4 )) {
			DBG("skipping %08lx, has save block in\n", addr);
			goto skip_check;
		}

		/* calculate and check the checksum */

		calc = crc32_le(~0, ptr, left);
		if (calc != *val) {
			printk(KERN_ERR PFX "Restore CRC error at "
			       "%08lx (%08x vs %08x)\n", addr, calc, *val);

			DBG("Restore CRC error at %08lx (%08x vs %08x)\n",
			    addr, calc, *val);
		}

	skip_check:
		val++;
	}

	return val;
}

/* wpcm450_pm_check_restore
 *
 * check the CRCs after the restore event and free the memory used
 * to hold them
*/

static void wpcm450_pm_check_restore(void)
{
	if (crcs != NULL) {
		wpcm450_pm_run_sysram(wpcm450_pm_runcheck, crcs);
		kfree(crcs);
		crcs = NULL;
	}
}

#else

#define wpcm450_pm_check_prepare() do { } while(0)
#define wpcm450_pm_check_restore() do { } while(0)
#define wpcm450_pm_check_store()   do { } while(0)
#endif

/* helper functions to save and restore register state */

void wpcm450_pm_do_save(struct sleep_save *ptr, int count)
{
	for (; count > 0; count--, ptr++) {
		ptr->val = __raw_readl(ptr->reg);
		DBG("saved %p value %08lx\n", ptr->reg, ptr->val);
	}
}

/* wpcm450_pm_do_restore
 *
 * restore the system from the given list of saved registers
 *
 * Note, we do not use DBG() in here, as the system may not have
 * restore the UARTs state yet
*/

void wpcm450_pm_do_restore(struct sleep_save *ptr, int count)
{
	for (; count > 0; count--, ptr++) {
		printk(KERN_DEBUG "restore %p (restore %08lx, was %08x)\n",
		       ptr->reg, ptr->val, __raw_readl(ptr->reg));

		__raw_writel(ptr->val, ptr->reg);
	}
}

/* wpcm450_pm_do_restore_core
 *
 * similar to wpcm450_pm_do_restore_core
 *
 * WARNING: Do not put any debug in here that may effect memory or use
 * peripherals, as things may be changing!
*/

static void wpcm450_pm_do_restore_core(struct sleep_save *ptr, int count)
{
	for (; count > 0; count--, ptr++) {
		__raw_writel(ptr->val, ptr->reg);
	}
}

/* wpcm450_pm_show_resume_irqs
 *
 * print any IRQs asserted at resume time (ie, we woke from)
*/

static void wpcm450_pm_show_resume_irqs(int start, unsigned long which,
					unsigned long mask)
{
	int i;

	which &= ~mask;

	for (i = 0; i <= 31; i++) {
		if ((which) & (1L<<i)) {
			DBG("IRQ %d asserted at resume\n", start+i);
		}
	}
}

/* wpcm450_pm_check_resume_pin
 *
 * check to see if the pin is configured correctly for sleep mode, and
 * make any necessary adjustments if it is not
*/

static void wpcm450_pm_check_resume_pin(unsigned int pin, unsigned int irqoffs)
{
	unsigned long irqstate;
	unsigned long pinstate;
	int irq = wpcm450_gpio_getirq(pin);

	if (irqoffs < 4)
		irqstate = s3c_irqwake_intmask & (1L<<irqoffs);
	else
		irqstate = s3c_irqwake_eintmask & (1L<<irqoffs);

	pinstate = wpcm450_gpio_getcfg(pin);
	pinstate >>= wpcm450_GPIO_OFFSET(pin)*2;

	if (!irqstate) {
		if (pinstate == 0x02)
			DBG("Leaving IRQ %d (pin %d) enabled\n", irq, pin);
	} else {
		if (pinstate == 0x02) {
			DBG("Disabling IRQ %d (pin %d)\n", irq, pin);
			wpcm450_gpio_cfgpin(pin, 0x00);
		}
	}
}

/* wpcm450_pm_configure_extint
 *
 * configure all external interrupt pins
*/

static void wpcm450_pm_configure_extint(void)
{
	int pin;

	/* for each of the external interrupts (EINT0..EINT15) we
	 * need to check wether it is an external interrupt source,
	 * and then configure it as an input if it is not
	*/

	for (pin = wpcm450_GPF0; pin <= wpcm450_GPF7; pin++) {
		wpcm450_pm_check_resume_pin(pin, pin - wpcm450_GPF0);
	}

	for (pin = wpcm450_GPG0; pin <= wpcm450_GPG7; pin++) {
		wpcm450_pm_check_resume_pin(pin, (pin - wpcm450_GPG0)+8);
	}
}

#define any_allowed(mask, allow) (((mask) & (allow)) != (allow))

/* wpcm450_pm_enter
 *
 * central control for sleep/resume process
*/

static int wpcm450_pm_enter(suspend_state_t state)
{
	unsigned long regs_save[16];
	unsigned long tmp;

	/* ensure the debug is initialised (if enabled) */

	wpcm450_pm_debug_init();

	DBG("wpcm450_pm_enter(%d)\n", state);

	if (state != PM_SUSPEND_MEM) {
		printk(KERN_ERR PFX "error: only PM_SUSPEND_MEM supported\n");
		return -EINVAL;
	}

	/* check if we have anything to wake-up with... bad things seem
	 * to happen if you suspend with no wakeup (system will often
	 * require a full power-cycle)
	*/

	if (!any_allowed(s3c_irqwake_intmask, s3c_irqwake_intallow) &&
	    !any_allowed(s3c_irqwake_eintmask, s3c_irqwake_eintallow)) {
		printk(KERN_ERR PFX "No sources enabled for wake-up!\n");
		printk(KERN_ERR PFX "Aborting sleep\n");
		return -EINVAL;
	}

	/* prepare check area if configured */

	wpcm450_pm_check_prepare();

	/* store the physical address of the register recovery block */

	wpcm450_sleep_save_phys = virt_to_phys(regs_save);

	DBG("wpcm450_sleep_save_phys=0x%08lx\n", wpcm450_sleep_save_phys);

	/* ensure at least GESTATUS3 has the resume address */

	__raw_writel(virt_to_phys(wpcm450_cpu_resume), wpcm450_GSTATUS3);

	DBG("GSTATUS3 0x%08x\n", __raw_readl(wpcm450_GSTATUS3));
	DBG("GSTATUS4 0x%08x\n", __raw_readl(wpcm450_GSTATUS4));

	/* save all necessary core registers not covered by the drivers */

	wpcm450_pm_do_save(gpio_save, ARRAY_SIZE(gpio_save));
	wpcm450_pm_do_save(irq_save, ARRAY_SIZE(irq_save));
	wpcm450_pm_do_save(core_save, ARRAY_SIZE(core_save));
	wpcm450_pm_do_save(uart_save, ARRAY_SIZE(uart_save));

	/* set the irq configuration for wake */

	wpcm450_pm_configure_extint();

	DBG("sleep: irq wakeup masks: %08lx,%08lx\n",
	    s3c_irqwake_intmask, s3c_irqwake_eintmask);

	__raw_writel(s3c_irqwake_intmask, wpcm450_INTMSK);
	__raw_writel(s3c_irqwake_eintmask, wpcm450_EINTMASK);

	/* ack any outstanding external interrupts before we go to sleep */

	__raw_writel(__raw_readl(wpcm450_EINTPEND), wpcm450_EINTPEND);

	/* flush cache back to ram */

	arm920_flush_kern_cache_all();

	wpcm450_pm_check_store();

	/* send the cpu to sleep... */

	__raw_writel(0x00, wpcm450_CLKCON);  /* turn off clocks over sleep */

	wpcm450_cpu_suspend(regs_save);

	/* restore the cpu state */

	cpu_init();

	/* unset the return-from-sleep flag, to ensure reset */

	tmp = __raw_readl(wpcm450_GSTATUS2);
	tmp &= wpcm450_GSTATUS2_OFFRESET;
	__raw_writel(tmp, wpcm450_GSTATUS2);

	/* restore the system state */

	wpcm450_pm_do_restore_core(core_save, ARRAY_SIZE(core_save));
	wpcm450_pm_do_restore(gpio_save, ARRAY_SIZE(gpio_save));
	wpcm450_pm_do_restore(irq_save, ARRAY_SIZE(irq_save));
	wpcm450_pm_do_restore(uart_save, ARRAY_SIZE(uart_save));

	wpcm450_pm_debug_init();

	/* check what irq (if any) restored the system */

	DBG("post sleep: IRQs 0x%08x, 0x%08x\n",
	    __raw_readl(wpcm450_SRCPND),
	    __raw_readl(wpcm450_EINTPEND));

	wpcm450_pm_show_resume_irqs(IRQ_EINT0, __raw_readl(wpcm450_SRCPND),
				    s3c_irqwake_intmask);

	wpcm450_pm_show_resume_irqs(IRQ_EINT4-4, __raw_readl(wpcm450_EINTPEND),
				    s3c_irqwake_eintmask);

	DBG("post sleep, preparing to return\n");

	wpcm450_pm_check_restore();

	/* ok, let's return from sleep */

	DBG("wpcm450 PM Resume (post-restore)\n");
	return 0;
}

/*
 * Called after processes are frozen, but before we shut down devices.
 */
static int wpcm450_pm_prepare(suspend_state_t state)
{
	return 0;
}

/*
 * Called after devices are re-setup, but before processes are thawed.
 */
static int wpcm450_pm_finish(suspend_state_t state)
{
	return 0;
}

/*
 * Set to PM_DISK_FIRMWARE so we can quickly veto suspend-to-disk.
 */
static struct pm_ops wpcm450_pm_ops = {
	.pm_disk_mode	= PM_DISK_FIRMWARE,
	.prepare	= wpcm450_pm_prepare,
	.enter		= wpcm450_pm_enter,
	.finish		= wpcm450_pm_finish,
};

/* wpcm450_pm_init
 *
 * Attach the power management functions. This should be called
 * from the board specific initialisation if the board supports
 * it.
*/

int __init wpcm450_pm_init(void)
{
	printk("wpcm450 Power Management, (c) 2004 Simtec Electronics\n");

	pm_set_ops(&wpcm450_pm_ops);
	return 0;
}

#endif