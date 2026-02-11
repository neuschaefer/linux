/*****************************************************************************
 * Copyright 2006 - 2011 Broadcom Corporation.  All rights reserved.
 *
 * Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2, available at
 * http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
 *
 * Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a
 * license other than the GPL, without Broadcom's express prior written
 * consent.
 *****************************************************************************/

/*
 * Frameworks:
 *
 *    - SMP:          Fully supported.    Mutex protects critical section in read.
 *    - GPIO:         Fully supported.    No GPIOs are used.
 *    - MMU:          Fully supported.    Platform model with ioremap used.
 *    - Dynamic /dev: Fully supported.    Uses HW random number framework
 *    - Suspend:      Fully supported.    There is nothing to do. (see Power)
 *    - Clocks:       Fully supported.    Uses APB7 clock.
 *    - Power:        Fully supported.    Clocks are only on when data is requested.
 *
 */

/*
 * DESCRIPTION: The Kona random number generator (RNG) driver
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/hw_random.h>
#include <linux/delay.h>
#include <linux/mutex.h>
#include <linux/io.h>
#include <mach/rdb/brcm_rdb_rng.h>
#include <linux/clk.h>

DEFINE_MUTEX(lock);		/* lock for data access */

static atomic_t bus_is_probed;

static void *baseAddr;
static struct clk *clock;

static void ioclrbit32(void *addr, unsigned int bits)
{
	iowrite32(ioread32(addr) & ~bits, addr);
}

static void iosetbit32(void *addr, unsigned int bits)
{
	iowrite32(ioread32(addr) | bits, addr);
}

/**
*  @brief  set warmup cycle
*  @param  none
*  @return none
*  @note
*****************************************************************************/
static inline void chal_rng_inline_warmup(uint32_t cycles)
{
	cycles <<= RNG_STATUS_RNG_WARM_CNT_SHIFT;
	iowrite32(RNG_STATUS_RNG_WARM_CNT_MASK -
		  (cycles & RNG_STATUS_RNG_WARM_CNT_MASK),
		  baseAddr + RNG_STATUS_OFFSET);
	while ((ioread32(baseAddr + RNG_STATUS_OFFSET) &
		RNG_STATUS_RNG_WARM_CNT_MASK) != RNG_STATUS_RNG_WARM_CNT_MASK) ;
}

/**
*  @brief  Stop RNG block
*  @param  none
*  @return none
*  @note
*****************************************************************************/
static inline void chal_rng_inline_enable(void)
{
	iosetbit32(baseAddr + RNG_CTRL_OFFSET, RNG_CTRL_RNG_RBGEN_MASK);
}

/**
*  @brief  Stop RNG block
*  @param  none
*  @return none
*  @note
*****************************************************************************/
static inline void chal_rng_inline_disable(void)
{
	ioclrbit32(baseAddr + RNG_CTRL_OFFSET, RNG_CTRL_RNG_RBGEN_MASK);
}

/**
*  @brief  Enable/Disable RNG RBG2X
*  @param  enable   (in) 1 to enable, 0 to disable
*  @return none
*  @note
*****************************************************************************/
static inline void chal_rng_inline_rbg2x_enable(void)
{
	iosetbit32(baseAddr + RNG_CTRL_OFFSET, RNG_CTRL_RNG_RBG2X_MASK);
}

/**
*  @brief  Enable/Disable RNG INTERRUPT
*  @param  enable   (in) 1 to enable, 0 to disable
*  @return none
*  @note
*****************************************************************************/
static inline void chal_rng_inline_int_disable(void)
{
	iosetbit32(baseAddr + RNG_INT_MASK_OFFSET,
		   RNG_INT_MASK_RNG_INT_OFF_MASK);
}

/**
*  @brief  RNG get number of valid words available
*  @return Number of words available
*  @note
*****************************************************************************/
static inline uint32_t chal_rng_inline_get_valid_words(void)
{
	return ((ioread32(baseAddr + RNG_STATUS_OFFSET) &
		 RNG_STATUS_RND_VAL_MASK) >> RNG_STATUS_RND_VAL_SHIFT);
}

/**
*  @brief  RNG get value
*  @param  pBuffer (in) Buffer to read data into
*  @param  len (in) bytes of the data to be read
*  @return Number of bytes actually read
*  @note
*****************************************************************************/
static inline uint32_t chal_rng_inline_get_random_number(void)
{
	while (chal_rng_inline_get_valid_words() == 0) ;

	return ioread32(baseAddr + RNG_DATA_OFFSET);
}

/**
 *  @brief  Start RNG block
 *  @param  none
 *  @return none
 *  @note
 *****************************************************************************/
static void chal_rng_inline_start(void)
{
	chal_rng_inline_int_disable();
	chal_rng_inline_enable();
	chal_rng_inline_rbg2x_enable();
	chal_rng_inline_warmup(0xfff);
}

static int kona_rng_read(struct hwrng *rng, void *data, size_t max, bool wait)
{
	int ready, i;

	if (!atomic_read(&bus_is_probed))
		return -ENODEV;

	clk_enable(clock);

	mutex_lock(&lock);

	chal_rng_inline_start();

	ready = chal_rng_inline_get_valid_words();

	while (!ready && wait) {
		/*
		 * RNG produces data fast enough.  We *could* use the RNG IRQ, but
		 * that'd be higher overhead ... so why bother?
		 */
		udelay(10);
		ready = chal_rng_inline_get_valid_words();
	}

	for (i = 0; i < (max / 4) && i < ready; ++i) {
		((unsigned int *)data)[i] = chal_rng_inline_get_random_number();
	}

	chal_rng_inline_disable();

	mutex_unlock(&lock);

//	clk_disable(clock);

	return i * 4;
}

static struct hwrng kona_rng_ops = {
	.name = "bcmkona",
	.read = kona_rng_read,
};

static int __init kona_rng_probe(struct platform_device *pdev)
{
	struct resource *res;
	int ret;

	/* We only accept one device, and it must have an id of -1 */
	if (pdev->id != -1)
		return -ENODEV;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		ret = -ENODEV;
		goto err_out;
	}

	baseAddr = ioremap(res->start, resource_size(res));
	if (!baseAddr) {
		ret = -ENOMEM;
		goto err_out;
	}

	atomic_set(&bus_is_probed, 0);

        clock = clk_get(&pdev->dev, pdev->dev.platform_data);
	if (clock < 0) {
		ret = -ENXIO;
		goto err_out;
	}

	/* register to the Linux RNG framework */
	ret = hwrng_register(&kona_rng_ops);
	if (ret)
		goto err_register;

	printk(KERN_INFO "RNG: Driver initialized\n");

	atomic_set(&bus_is_probed, 1);

	return 0;

 err_register:
	iounmap(baseAddr);
 err_out:
	return ret;
}

static int __devexit kona_rng_remove(struct platform_device *pdev)
{
	atomic_set(&bus_is_probed, 0);
	hwrng_unregister(&kona_rng_ops);
	clk_put(clock);
	return 0;
}

static struct platform_driver kona_rng_driver = {
	.driver = {
		   .name = "kona_rng",
		   .owner = THIS_MODULE,
		   },
	.remove = __devexit_p(kona_rng_remove),
};

static char banner[] __initdata = KERN_INFO "Broadcom KONA RNG Driver\n";

static int __init kona_rng_init(void)
{
	printk(banner);
	return platform_driver_probe(&kona_rng_driver, kona_rng_probe);
}

static void __exit kona_rng_exit(void)
{
	platform_driver_unregister(&kona_rng_driver);
}

module_init(kona_rng_init);
module_exit(kona_rng_exit);

MODULE_AUTHOR("Broadcom");
MODULE_DESCRIPTION("RNG Device Driver");
MODULE_LICENSE("GPL");
