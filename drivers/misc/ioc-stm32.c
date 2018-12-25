/*
 * Timer device implementation for SGI UV platform.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 2009 Silicon Graphics, Inc.  All rights reserved.
 *
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/ioctl.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/vmalloc.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/dma-mapping.h>
#include <linux/regulator/consumer.h>

#define IOC_NAME "ioc-stm32"

#define ERR(s...) printk(KERN_ERR IOC_NAME ": " s)
#define INF(s...) printk(KERN_ERR IOC_NAME ": " s)
#define DBG(s...) printk(KERN_ERR IOC_NAME ": " s)
//#define DBG(s...)

#define IOC_RO_ATTRIBUTE(name, mode) \
    static struct kobj_attribute name##_attribute = __ATTR(name, mode, name##_show, NULL);

#define IOC_WO_ATTRIBUTE(name, mode) \
    static struct kobj_attribute name##_attribute = __ATTR(name, mode, NULL, name##_store);

#define IOC_RW_ATTRIBUTE(name, mode) \
    static struct kobj_attribute name##_attribute = __ATTR(name, mode, name##_show, name##_store);


#define SPI_BUFFER_SIZE 65536

static struct spi_device *ioc_spi_device = NULL;
static struct kobject *ioc_kobj = NULL;

static struct regulator *ldo4 = NULL;

static int spi_registered = 0;

static dma_addr_t buf_phys;
static void *buf_virt;

static struct ioc_context {

	int enabled;

} gioc;

static char *hexdump(void *data, int len)
{
	static char buf[132];
	unsigned char *p = (unsigned char *) data;
	int i;

	if (len > 64) len = 64;
	for (i=0; i<len; i++) sprintf(buf+i*2, "%02x", *p++);
	return buf;
}

static char *blockdump(void *data, int len)
{
	static char buf[64];

	strcpy(buf, hexdump(data, 8));
	buf[16] = buf[17] = '.';
	strcpy(buf+18, hexdump(data+(len-8), 8));
	return buf;
}

static int ioc_set_enabled(int en)
{
	if (en) {
		if (! regulator_is_enabled(ldo4)) regulator_enable(ldo4);
	} else {
		if (regulator_is_enabled(ldo4)) regulator_disable(ldo4);
	}
	gioc.enabled = en;
}

static ssize_t ioc_write(struct file * file, const char __user * buf,
                        size_t count, loff_t *ppos)
{
	return count;
}

static ssize_t ioc_read(struct file * file, char __user * buf,
                        size_t count, loff_t *ppos)
{
	return count;
}

static long ioc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {

	  default:
		ret = -ENOSYS;
		break;

	}

	return ret;
}


static ssize_t enabled_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return scnprintf(buf, PAGE_SIZE, "%d\n", 1);
}

static ssize_t enabled_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int en = simple_strtol(buf, NULL, 10);
    DBG("enabled=%d\n", en);
    ioc_set_enabled(en);
    return count;
}

IOC_RW_ATTRIBUTE(enabled, 0644);

static struct attribute *ioc_sysfs_attrs[] = {

    &enabled_attribute.attr,
    NULL

};

static struct attribute_group ioc_sysfs_group = {

    .attrs = ioc_sysfs_attrs,

};

static const struct file_operations ioc_fops = {
	.owner = THIS_MODULE,
	.read = ioc_read,
	.write = ioc_write,
	.unlocked_ioctl = ioc_ioctl,
};

static struct miscdevice ioc_miscdev = {
	235 /*MISC_DYNAMIC_MINOR*/,
	IOC_NAME,
	&ioc_fops
};

static int ioc_probe(struct spi_device *spi) {

	ioc_spi_device = spi;
	ioc_kobj = &spi->dev.kobj;

	ldo4 = regulator_get(NULL, "axp20_hdmi");
	if (IS_ERR(ldo4)) {
		ERR("could not get ldo4 regulator\n");
		return -ENODEV;
	}

	buf_virt = dma_alloc_coherent(NULL, SPI_BUFFER_SIZE, &buf_phys, GFP_KERNEL);
	if (! buf_virt) {
		ERR("failed to allocate memory\n");
		return -ENOMEM;
	}

	if (misc_register(&ioc_miscdev)) {
		ERR("failed to register device\n");
		return -ENODEV;
	}

	sysfs_create_group(ioc_kobj, &ioc_sysfs_group);

	ioc_set_enabled(1);

	INF("registered\n");
	return 0;
}

static int ioc_remove(struct spi_device *spi)
{
	ioc_set_enabled(0);

	sysfs_remove_group(ioc_kobj, &ioc_sysfs_group);

	misc_deregister(&ioc_miscdev);

	dma_free_coherent(NULL, SPI_BUFFER_SIZE, &buf_phys, GFP_KERNEL);

	regulator_put(ldo4);

	ioc_kobj = NULL;
	ioc_spi_device = NULL;

	return 0;
}

static const struct spi_device_id ioc_device_id[] = {
        { IOC_NAME, 0 },
        {}
};
MODULE_DEVICE_TABLE(spi, ioc_device_id);

static struct spi_driver ioc_spi_driver = {
        .id_table = ioc_device_id,
        .driver = {
                .name   = IOC_NAME,
                .bus = &spi_bus_type,
                .owner = THIS_MODULE,
        },
        .probe = ioc_probe,
        .remove = ioc_remove,
};

static int __init ioc_init(void) {

        int ret = spi_register_driver(&ioc_spi_driver);
        if (ret) {
                ERR("spi driver registration failed: %d\n", ret);
                spi_registered = 0;
        } else {
                spi_registered = 1;
        }
        return ret;
}

void ioc_exit(void)
{
        if (spi_registered) {
                spi_unregister_driver(&ioc_spi_driver);
                spi_registered = 0;
        }
}

module_init(ioc_init);
module_exit(ioc_exit);

