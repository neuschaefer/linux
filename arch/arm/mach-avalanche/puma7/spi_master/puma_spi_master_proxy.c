/*
 *
 * puma_spi_master_proxy.c
 * Description:
 * User mode SPI device interface.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014 Intel Corporation.
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

#include <linux/fs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master.h>
#include <asm-arm/arch-avalanche/puma7/spi_master/puma_spi_master_proxy.h>


struct spi_proxy_private_data {
    u8 tx_buffer[MAX_TRANSFER];
    u8 rx_buffer[MAX_TRANSFER];
    unsigned chipselect;
};


static int spidev_open(struct inode *inode, struct file *filp)
{
    struct spi_proxy_private_data *priv_data;
    unsigned minor_number;
    int ret = 0;

    minor_number = MINOR(inode->i_rdev);
    ret = puma_spi_device_open(minor_number);
    if (ret != 0) {
        goto out;
    }

    priv_data = kzalloc(sizeof(struct spi_proxy_private_data), GFP_KERNEL);
    if (priv_data == NULL) {
        puma_spi_device_close(minor_number);
        ret = -ENOMEM;
        goto out;
    }

    priv_data->chipselect = minor_number;
    filp->private_data = priv_data;

out:
    return ret;
}

static ssize_t spidev_read(struct file *filp, char __user *buf, size_t count,
                           loff_t *f_pos)
{
    struct spi_proxy_private_data *priv_data = filp->private_data;
    int ret = 0;

    if (count > MAX_TRANSFER) {
        ret = -EMSGSIZE;
        goto out;
    }

    ret = puma_spi_device_transfer(priv_data->chipselect, NULL, priv_data->rx_buffer, count);
    if (ret != 0) {
        goto out;
    }

    if (copy_to_user(buf, priv_data->rx_buffer, count)) {
        ret = -EFAULT;
        goto out;
    }

    ret = count;

out:
    return ret;
}

static int spidev_release(struct inode *inode, struct file *filp)
{
    struct spi_proxy_private_data *priv_data = filp->private_data;
    int ret = 0;

    ret = puma_spi_device_close(priv_data->chipselect);

    kfree(filp->private_data);
    filp->private_data = NULL;

    return ret;
}

static long spidev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct puma_spi_ioctl_transfer transfer;
    struct spi_proxy_private_data *priv_data = filp->private_data;
    char *tx_buf = NULL;
    char *rx_buf = NULL;
    int ret = 0;

	/* Check type and command number */
	if (_IOC_TYPE(cmd) != SPI_IOCTL_MAGIC) {
        ret = -ENOTTY;
        goto out;
    }
    if (_IOC_SIZE(cmd) != sizeof(struct puma_spi_ioctl_transfer)) {
        ret = -EINVAL;
        goto out;
    }

    if (copy_from_user(&transfer, (void __user *)arg, sizeof(struct puma_spi_ioctl_transfer))) {
        ret = -EFAULT;
        goto out;
    }

    if (transfer.len > MAX_TRANSFER) {
        ret = -EMSGSIZE;
        goto out;
    }

    if (transfer.tx_buf) {
        tx_buf = priv_data->tx_buffer;
        if (copy_from_user(priv_data->tx_buffer, transfer.tx_buf, transfer.len)) {
            ret = -EFAULT;
            goto out;
        }
    }
    if (transfer.rx_buf) {
        rx_buf = priv_data->rx_buffer;
    }

    if (!tx_buf && !rx_buf) {
        /* No buffers specified */
        ret = -EINVAL;
        goto out;
    }

    ret = puma_spi_device_transfer(priv_data->chipselect, tx_buf, rx_buf, transfer.len);
    if ((ret == 0) && (rx_buf)) {
        if (copy_to_user(transfer.rx_buf, priv_data->rx_buffer, transfer.len)) {
            ret = -EFAULT;
            goto out;
        }
    }

out:
    return ret;
}

static ssize_t spidev_write(struct file *filp, const char __user *buf,
                            size_t count, loff_t *f_pos)
{
    struct spi_proxy_private_data *priv_data = filp->private_data;
    int ret = 0;

    if (count > MAX_TRANSFER) {
        ret = -EMSGSIZE;
        goto out;
    }

    if (copy_from_user(priv_data->tx_buffer, buf, count)) {
        ret = -EFAULT;
        goto out;
    }

    ret = puma_spi_device_transfer(priv_data->chipselect, priv_data->tx_buffer, NULL, count);
    if (ret != 0) {
        goto out;
    }

    ret = count;

out:
    return ret;
}


static const struct file_operations spidev_fops = {
    .owner = THIS_MODULE,

    .open           = spidev_open,
    .read           = spidev_read,
    .release        = spidev_release,
    .unlocked_ioctl = spidev_ioctl,
    .write          = spidev_write
};


int puma_spi_proxy_init(void)
{
    return register_chrdev(PUMA_SPI_MASTER_MAJOR, "puma_spi", &spidev_fops);
}
