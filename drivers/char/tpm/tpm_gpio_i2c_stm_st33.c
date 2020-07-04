/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * STMicroelectronics TPM I2C Linux driver for TPM ST33ZP24
 * Copyright (C) 2009, 2010, 2014  STMicroelectronics
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
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * STMicroelectronics version 1.2.1, Copyright (C) 2014
 * STMicroelectronics comes with ABSOLUTELY NO WARRANTY.
 * This is free software, and you are welcome to redistribute it
 * under certain conditions.
 *
 * @Author: Christophe RICARD tpmsupport@st.com
 *
 * @File: tpm_stm_st33_i2c.c
 *
 * @Synopsis:
 *	09/15/2010:	First shot driver tpm_tis driver for
 *			 lpc is used as model.
 */

#include <linux/pci.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/freezer.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/sysfs.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>

#include <linux/platform_data/tpm_stm_st33.h>
#include "tpm.h"

#define TPM_ACCESS			0x0
#define TPM_STS				0x18
#define TPM_HASH_END			0x20
#define TPM_DATA_FIFO			0x24
#define TPM_HASH_DATA			0x24
#define TPM_HASH_START			0x28
#define TPM_INTF_CAPABILITY		0x14
#define TPM_INT_STATUS			0x10
#define TPM_INT_ENABLE			0x08

#define TPM_DUMMY_BYTE			0xAA
#define TPM_WRITE_DIRECTION		0x80
#define TPM_HEADER_SIZE			10
#define TPM_BUFSIZE			4096

#define LOCALITY0		0


enum stm33zp24_access {
	TPM_ACCESS_VALID = 0x80,
	TPM_ACCESS_ACTIVE_LOCALITY = 0x20,
	TPM_ACCESS_REQUEST_PENDING = 0x04,
	TPM_ACCESS_REQUEST_USE = 0x02,
};

enum stm33zp24_status {
	TPM_STS_VALID = 0x80,
	TPM_STS_COMMAND_READY = 0x40,
	TPM_STS_GO = 0x20,
	TPM_STS_DATA_AVAIL = 0x10,
	TPM_STS_DATA_EXPECT = 0x08,
};

enum stm33zp24_int_flags {
	TPM_GLOBAL_INT_ENABLE = 0x80,
	TPM_INTF_CMD_READY_INT = 0x080,
	TPM_INTF_FIFO_AVALAIBLE_INT = 0x040,
	TPM_INTF_WAKE_UP_READY_INT = 0x020,
	TPM_INTF_LOCALITY_CHANGE_INT = 0x004,
	TPM_INTF_STS_VALID_INT = 0x002,
	TPM_INTF_DATA_AVAIL_INT = 0x001,
};

enum tis_defaults {
	TIS_SHORT_TIMEOUT = 750,
	TIS_LONG_TIMEOUT = 2000,
};

struct tpm_stm_dev {
	struct tpm_chip *chip;
	u8 buf[TPM_BUFSIZE + 1];
	u32 intrs;
	int io_lpcpd;
};

unsigned int gpio_i2c_init(VOID);
unsigned int gpio_i2c_read( unsigned char *buffer, unsigned int len);
unsigned int gpio_i2c_write(unsigned char *data, unsigned int len);
/*
 * write8_reg
 * Send byte to the TIS register according to the ST33ZP24 I2C protocol.
 * @param: tpm_register, the tpm tis register where the data should be written
 * @param: tpm_data, the tpm_data to write inside the tpm_register
 * @param: tpm_size, The length of the data
 * @return: Returns negative errno, or else the number of bytes written.
 */
static int write8_reg(struct tpm_stm_dev *tpm_dev, u8 tpm_register,
		      u8 *tpm_data, u16 tpm_size)
{
	tpm_dev->buf[0] = tpm_register;
	memcpy(tpm_dev->buf + 1, tpm_data, tpm_size);
	return gpio_i2c_write(tpm_dev->buf, tpm_size + 1);
} /* write8_reg() */

/*
 * read8_reg
 * Recv byte from the TIS register according to the ST33ZP24 I2C protocol.
 * @param: tpm_register, the tpm tis register where the data should be read
 * @param: tpm_data, the TPM response
 * @param: tpm_size, tpm TPM response size to read.
 * @return: number of byte read successfully: should be one if success.
 */
static int read8_reg(struct tpm_stm_dev *tpm_dev, u8 tpm_register,
		    u8 *tpm_data, int tpm_size)
{
	u8 status = 0;
	u8 data;

	data = TPM_DUMMY_BYTE;
	status = write8_reg(tpm_dev, tpm_register, &data, 1);
	if (status == 2)
		status = gpio_i2c_read(tpm_data, tpm_size);
	return status;
} /* read8_reg() */

/*
 * I2C_WRITE_DATA
 * Send byte to the TIS register according to the ST33ZP24 I2C protocol.
 * @param: tpm_dev, the chip description
 * @param: tpm_register, the tpm tis register where the data should be written
 * @param: tpm_data, the tpm_data to write inside the tpm_register
 * @param: tpm_size, The length of the data
 * @return: number of byte written successfully: should be one if success.
 */
#define I2C_WRITE_DATA(tpm_dev, tpm_register, tpm_data, tpm_size) \
	(write8_reg(tpm_dev, tpm_register | \
	TPM_WRITE_DIRECTION, tpm_data, tpm_size))

/*
 * I2C_READ_DATA
 * Recv byte from the TIS register according to the ST33ZP24 I2C protocol.
 * @param: tpm_dev, the chip description
 * @param: tpm_register, the tpm tis register where the data should be read
 * @param: tpm_data, the TPM response
 * @param: tpm_size, tpm TPM response size to read.
 * @return: number of byte read successfully: should be one if success.
 */
#define I2C_READ_DATA(tpm_dev, tpm_register, tpm_data, tpm_size) \
	(read8_reg(tpm_dev, tpm_register, tpm_data, tpm_size))

/*
 * clear_interruption
 * clear the TPM interrupt register.
 * @param: tpm, the chip description
 * @return: the TPM_INT_STATUS value
 */
static u8 clear_interruption(struct tpm_stm_dev *tpm_dev)
{
	u8 interrupt;

	I2C_READ_DATA(tpm_dev, TPM_INT_STATUS, &interrupt, 1);
	I2C_WRITE_DATA(tpm_dev, TPM_INT_STATUS, &interrupt, 1);
	return interrupt;
} /* clear_interruption() */

/*
 * tpm_stm_i2c_cancel, cancel is not implemented.
 * @param: chip, the tpm_chip description as specified in driver/char/tpm/tpm.h
 */
static void tpm_stm_i2c_cancel(struct tpm_chip *chip)
{
	struct tpm_stm_dev *tpm_dev;
	u8 data;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	data = TPM_STS_COMMAND_READY;
	I2C_WRITE_DATA(tpm_dev, TPM_STS, &data, 1);
} /* tpm_stm_i2c_cancel() */

/*
 * tpm_stm_spi_status return the TPM_STS register
 * @param: chip, the tpm chip description
 * @return: the TPM_STS register value.
 */
static u8 tpm_stm_i2c_status(struct tpm_chip *chip)
{
	struct tpm_stm_dev *tpm_dev;
	u8 data;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	I2C_READ_DATA(tpm_dev, TPM_STS, &data, 1);
	return data;
} /* tpm_stm_i2c_status() */


/*
 * check_locality if the locality is active
 * @param: chip, the tpm chip description
 * @return: the active locality or -EACCESS.
 */
static int check_locality(struct tpm_chip *chip)
{
	struct tpm_stm_dev *tpm_dev;
	u8 data;
	u8 status;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	status = I2C_READ_DATA(tpm_dev, TPM_ACCESS, &data, 1);
	if (status && (data &
		(TPM_ACCESS_ACTIVE_LOCALITY | TPM_ACCESS_VALID)) ==
		(TPM_ACCESS_ACTIVE_LOCALITY | TPM_ACCESS_VALID))
		return chip->vendor.locality;

	return -EACCES;
} /* check_locality() */

/*
 * request_locality request the TPM locality
 * @param: chip, the chip description
 * @return: the active locality or EACCESS.
 */
static int request_locality(struct tpm_chip *chip)
{
	unsigned long stop;
	long ret;
	struct tpm_stm_dev *tpm_dev;
	u8 data;

	if (check_locality(chip) == chip->vendor.locality)
		return chip->vendor.locality;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	data = TPM_ACCESS_REQUEST_USE;
	ret = I2C_WRITE_DATA(tpm_dev, TPM_ACCESS, &data, 1);
	if (ret < 0)
		goto end;

	stop = jiffies + chip->vendor.timeout_a;

	/* Request locality is usually effective after the request */
	do {
		if (check_locality(chip) >= 0)
			return chip->vendor.locality;
		msleep(TPM_TIMEOUT);
	} while (time_before(jiffies, stop));
	ret = -EACCES;
end:
	return ret;
} /* request_locality() */

/*
 * release_locality release the active locality
 * @param: chip, the tpm chip description.
 */
static void release_locality(struct tpm_chip *chip)
{
	struct tpm_stm_dev *tpm_dev;
	u8 data;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);
	data = TPM_ACCESS_ACTIVE_LOCALITY;

	I2C_WRITE_DATA(tpm_dev, TPM_ACCESS, &data, 1);
}

/*
 * get_burstcount return the burstcount address 0x19 0x1A
 * @param: chip, the chip description
 * return: the burstcount.
 */
static int get_burstcount(struct tpm_chip *chip)
{
	unsigned long stop;
	int burstcnt, status;
	u8 tpm_reg, temp;
	struct tpm_stm_dev *tpm_dev;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	stop = jiffies + chip->vendor.timeout_d;
	do {
		tpm_reg = TPM_STS + 1;
		status = I2C_READ_DATA(tpm_dev, tpm_reg, &temp, 1);
		if (status < 0)
			goto end;

		tpm_reg = tpm_reg + 1;
		burstcnt = temp;
		status = I2C_READ_DATA(tpm_dev, tpm_reg, &temp, 1);
		if (status < 0)
			goto end;

		burstcnt |= temp << 8;
		if (burstcnt)
			return burstcnt;
		msleep(TPM_TIMEOUT);
	} while (time_before(jiffies, stop));

end:
	return -EBUSY;
} /* get_burstcount() */

static bool wait_for_tpm_stat_cond(struct tpm_chip *chip, u8 mask,
				bool check_cancel, bool *canceled)
{
	u8 status = chip->vendor.status(chip);

	*canceled = false;
	if ((status & mask) == mask)
		return true;
	if (check_cancel && chip->vendor.req_canceled == status) {
		*canceled = true;
		return true;
	}
	return false;
}

/*
 * interrupt_to_status
 * @param: irq_mask, the irq mask value to wait
 * @return: the corresponding tpm_sts value
 */
static u8 interrupt_to_status(u8 irq_mask)
{
	u8 status = 0;

	if ((irq_mask & TPM_INTF_STS_VALID_INT) == TPM_INTF_STS_VALID_INT)
		status |= TPM_STS_VALID;
	if ((irq_mask & TPM_INTF_DATA_AVAIL_INT) == TPM_INTF_DATA_AVAIL_INT)
		status |= TPM_STS_DATA_AVAIL;
	if ((irq_mask & TPM_INTF_CMD_READY_INT) == TPM_INTF_CMD_READY_INT)
		status |= TPM_STS_COMMAND_READY;

	return status;
} /* status_to_interrupt() */

/*
 * wait_for_stat wait for a TPM_STS value
 * @param: chip, the tpm chip description
 * @param: mask, the value mask to wait
 * @param: timeout, the timeout
 * @param: queue, the wait queue.
 * @param: check_cancel, does the command can be cancelled ?
 * @return: the tpm status, 0 if success, -ETIME if timeout is reached.
 */
static int wait_for_stat(struct tpm_chip *chip, u8 mask, unsigned long timeout,
			wait_queue_head_t *queue, bool check_cancel)
{
	unsigned long stop;
	int ret;
	bool canceled = false;
	bool condition;
	u32 cur_intrs;
	u8 interrupt, status;
	struct tpm_stm_dev *tpm_dev;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	/* check current status */
	status = tpm_stm_i2c_status(chip);
	if ((status & mask) == mask)
		return 0;

	stop = jiffies + timeout;

	if (chip->vendor.irq) {
		cur_intrs = tpm_dev->intrs;
		interrupt = clear_interruption(tpm_dev);
		enable_irq(chip->vendor.irq);

again:
		timeout = stop - jiffies;
		if ((long) timeout <= 0)
			return -1;

		ret = wait_event_interruptible_timeout(*queue,
					cur_intrs != tpm_dev->intrs, timeout);

		interrupt |= clear_interruption(tpm_dev);
		status = interrupt_to_status(interrupt);
		condition = wait_for_tpm_stat_cond(chip, mask,
						   check_cancel, &canceled);

		if (ret >= 0 && condition) {
			if (canceled)
				return -ECANCELED;
			return 0;
		}
		if (ret == -ERESTARTSYS && freezing(current)) {
			clear_thread_flag(TIF_SIGPENDING);
			goto again;
		}
		disable_irq_nosync(chip->vendor.irq);

	} else {
		do {
			msleep(TPM_TIMEOUT);
			status = chip->vendor.status(chip);
			if ((status & mask) == mask)
				return 0;
		} while (time_before(jiffies, stop));
	}

	return -ETIME;
} /* wait_for_stat() */

/*
 * recv_data receive data
 * @param: chip, the tpm chip description
 * @param: buf, the buffer where the data are received
 * @param: count, the number of data to receive
 * @return: the number of bytes read from TPM FIFO.
 */
static int recv_data(struct tpm_chip *chip, u8 *buf, size_t count)
{
	int size = 0, burstcnt, len, ret;
	struct tpm_stm_dev *tpm_dev;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	while (size < count &&
	       wait_for_stat(chip,
			     TPM_STS_DATA_AVAIL | TPM_STS_VALID,
			     chip->vendor.timeout_c,
			     &chip->vendor.read_queue, true) == 0) {
		burstcnt = get_burstcount(chip);
		if (burstcnt < 0)
			return burstcnt;
		len = min_t(int, burstcnt, count - size);
		ret = I2C_READ_DATA(tpm_dev, TPM_DATA_FIFO, buf + size, len);
		if (ret < 0)
			return ret;

		size += len;
	}
	return size;
}

/*
 * tpm_ioserirq_handler the serirq irq handler
 * @param: irq, the tpm chip description
 * @param: dev_id, the description of the chip
 * @return: the status of the handler.
 */
static irqreturn_t tpm_ioserirq_handler(int irq, void *dev_id)
{
	struct tpm_chip *chip = dev_id;
	struct tpm_stm_dev *tpm_dev;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	tpm_dev->intrs++;
	wake_up_interruptible(&chip->vendor.read_queue);
	disable_irq_nosync(chip->vendor.irq);

	return IRQ_HANDLED;
} /* tpm_ioserirq_handler() */


/*
 * tpm_stm_i2c_send send TPM commands through the I2C bus.
 *
 * @param: chip, the tpm_chip description as specified in driver/char/tpm/tpm.h
 * @param: buf,	the buffer to send.
 * @param: count, the number of bytes to send.
 * @return: In case of success the number of bytes sent.
 *			In other case, a < 0 value describing the issue.
 */
static int tpm_stm_i2c_send(struct tpm_chip *chip, unsigned char *buf,
			    size_t len)
{
	u32 status, i, size, ordinal;
	int burstcnt = 0;
	int ret;
	u8 data;
	struct tpm_stm_dev *tpm_dev;

	if (!chip)
		return -EBUSY;
	if (len < TPM_HEADER_SIZE)
		return -EBUSY;

	tpm_dev = (struct tpm_stm_dev *)TPM_VPRIV(chip);

	ret = request_locality(chip);
	if (ret < 0)
		return ret;

	status = tpm_stm_i2c_status(chip);
	if ((status & TPM_STS_COMMAND_READY) == 0) {
		tpm_stm_i2c_cancel(chip);
		if (wait_for_stat
		    (chip, TPM_STS_COMMAND_READY, chip->vendor.timeout_b,
		     &chip->vendor.read_queue, false) < 0) {
			ret = -ETIME;
			goto out_err;
		}
	}

	for (i = 0; i < len - 1;) {
		burstcnt = get_burstcount(chip);
		if (burstcnt < 0)
			return burstcnt;
		size = min_t(int, len - i - 1, burstcnt);
		ret = I2C_WRITE_DATA(tpm_dev, TPM_DATA_FIFO, buf + i, size);
		if (ret < 0)
			goto out_err;

		i += size;
	}

	status = tpm_stm_i2c_status(chip);
	if ((status & TPM_STS_DATA_EXPECT) == 0) {
		ret = -EIO;
		goto out_err;
	}

	ret = I2C_WRITE_DATA(tpm_dev, TPM_DATA_FIFO, buf + len - 1, 1);
	if (ret < 0)
		goto out_err;

	status = tpm_stm_i2c_status(chip);
	if ((status & TPM_STS_DATA_EXPECT) != 0) {
		ret = -EIO;
		goto out_err;
	}

	data = TPM_STS_GO;
	I2C_WRITE_DATA(tpm_dev, TPM_STS, &data, 1);

	if (chip->vendor.irq) {
		ordinal = be32_to_cpu(*((__be32 *) (buf + 6)));

		ret = wait_for_stat(chip, TPM_STS_DATA_AVAIL | TPM_STS_VALID,
				  tpm_calc_ordinal_duration(chip, ordinal),
				  &chip->vendor.read_queue, false);
		if (ret < 0)
			goto out_err;
	}

	return len;
out_err:
	tpm_stm_i2c_cancel(chip);
	release_locality(chip);
	return ret;
}

/*
 * tpm_stm_i2c_recv received TPM response through the I2C bus.
 * @param: chip, the tpm_chip description as specified in driver/char/tpm/tpm.h.
 * @param: buf,	the buffer to store datas.
 * @param: count, the number of bytes to send.
 * @return: In case of success the number of bytes received.
 *	    In other case, a < 0 value describing the issue.
 */
static int tpm_stm_i2c_recv(struct tpm_chip *chip, unsigned char *buf,
			    size_t count)
{
	int size = 0;
	int expected;

	if (!chip)
		return -EBUSY;

	if (count < TPM_HEADER_SIZE) {
		size = -EIO;
		goto out;
	}

	size = recv_data(chip, buf, TPM_HEADER_SIZE);
	if (size < TPM_HEADER_SIZE) {
		dev_err(chip->dev, "Unable to read header\n");
		goto out;
	}

	expected = be32_to_cpu(*(__be32 *)(buf + 2));
	if (expected > count) {
		size = -EIO;
		goto out;
	}

	size += recv_data(chip, &buf[TPM_HEADER_SIZE],
			expected - TPM_HEADER_SIZE);
	if (size < expected) {
		dev_err(chip->dev, "Unable to read remainder of result\n");
		size = -ETIME;
		goto out;
	}

out:
	chip->vendor.cancel(chip);
	release_locality(chip);
	return size;
}

static const struct file_operations tpm_st33_i2c_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.read = tpm_read,
	.write = tpm_write,
	.open = tpm_open,
	.release = tpm_release,
};

static DEVICE_ATTR(pubek, S_IRUGO, tpm_show_pubek, NULL);
static DEVICE_ATTR(pcrs, S_IRUGO, tpm_show_pcrs, NULL);
static DEVICE_ATTR(enabled, S_IRUGO, tpm_show_enabled, NULL);
static DEVICE_ATTR(active, S_IRUGO, tpm_show_active, NULL);
static DEVICE_ATTR(owned, S_IRUGO, tpm_show_owned, NULL);
static DEVICE_ATTR(temp_deactivated, S_IRUGO, tpm_show_temp_deactivated, NULL);
static DEVICE_ATTR(caps, S_IRUGO, tpm_show_caps_1_2, NULL);
static DEVICE_ATTR(cancel, S_IWUSR | S_IWGRP, NULL, tpm_store_cancel);

static struct attribute *stm_tpm_attrs[] = {
	&dev_attr_pubek.attr,
	&dev_attr_pcrs.attr,
	&dev_attr_enabled.attr,
	&dev_attr_active.attr,
	&dev_attr_owned.attr,
	&dev_attr_temp_deactivated.attr,
	&dev_attr_caps.attr,
	&dev_attr_cancel.attr, NULL,
};

static struct attribute_group stm_tpm_attr_grp = {
	.attrs = stm_tpm_attrs
};

static const struct tpm_vendor_specific st_i2c_tpm = {
	.send = tpm_stm_i2c_send,
	.recv = tpm_stm_i2c_recv,
	.cancel = tpm_stm_i2c_cancel,
	.status = tpm_stm_i2c_status,
	.req_complete_mask = TPM_STS_DATA_AVAIL | TPM_STS_VALID,
	.req_complete_val = TPM_STS_DATA_AVAIL | TPM_STS_VALID,
	.req_canceled = TPM_STS_COMMAND_READY,
	.attr_group = &stm_tpm_attr_grp,
	.miscdev = {.fops = &tpm_st33_i2c_fops,},

};

#ifdef CONFIG_PM_SLEEP
/*
 * tpm_stm_i2c_pm_suspend suspend the TPM device
 * @param: client, the i2c_client drescription (TPM I2C description).
 * @param: mesg, the power management message.
 * @return: 0 in case of success.
 */
static int tpm_stm_i2c_pm_suspend(struct device *dev)
{
	struct st33zp24_platform_data *pin_infos = dev->platform_data;
	pm_message_t msg;
	int ret = 0;

	if (gpio_is_valid(pin_infos->io_lpcpd))
		gpio_set_value(pin_infos->io_lpcpd, 0);
	else
		ret = tpm_pm_suspend(dev, msg);

	return ret;
} /* tpm_stm_i2c_suspend() */

/*
 * tpm_stm_i2c_pm_resume resume the TPM device
 * @param: client, the i2c_client drescription (TPM I2C description).
 * @return: 0 in case of success.
 */
static int tpm_stm_i2c_pm_resume(struct device *dev)
{
	struct tpm_chip *chip = dev_get_drvdata(dev);
	struct st33zp24_platform_data *pin_infos = dev->platform_data;

	int ret = 0;

	if (gpio_is_valid(pin_infos->io_lpcpd)) {
		gpio_set_value(pin_infos->io_lpcpd, 1);
		ret = wait_for_stat(chip,
				TPM_STS_VALID, chip->vendor.timeout_b,
				&chip->vendor.read_queue, false);
	} else {
		ret = tpm_pm_resume(dev);
		if (!ret)
			tpm_do_selftest(chip);
	}
	return ret;
} /* tpm_stm_i2c_pm_resume() */
#endif

static struct platform_driver tpm_stm_i2c_drv = {
	.driver = {
		.name = TPM_ST33_I2C,
		.owner = THIS_MODULE,
	},
#ifdef CONFIG_PM_SLEEP
	.suspend = tpm_stm_i2c_pm_suspend,
	.resume = tpm_stm_i2c_pm_resume,
#endif
};

static struct platform_device *pdev;

static void __exit tpm_stm_i2c_exit(void)
{
	struct tpm_chip *chip = dev_get_drvdata(&pdev->dev);
	
	platform_driver_unregister(&tpm_stm_i2c_drv);
	tpm_remove_hardware(chip->dev);
	platform_device_unregister(pdev);
}

static int tpm_stm_i2c_startup(struct tpm_chip *chip)
{
	int ret;
	u8 intmask = 0;
	struct st33zp24_platform_data *platform_data;
	struct tpm_stm_dev *tpm_dev;

	tpm_dev = devm_kzalloc(&pdev->dev, sizeof(struct tpm_stm_dev),
			       GFP_KERNEL);
	if (!tpm_dev)
		return -ENOMEM;

	TPM_VPRIV(chip) = tpm_dev;

	chip->vendor.timeout_a = msecs_to_jiffies(TIS_SHORT_TIMEOUT);
	chip->vendor.timeout_b = msecs_to_jiffies(TIS_LONG_TIMEOUT);
	chip->vendor.timeout_c = msecs_to_jiffies(TIS_SHORT_TIMEOUT);
	chip->vendor.timeout_d = msecs_to_jiffies(TIS_SHORT_TIMEOUT);

	chip->vendor.locality = LOCALITY0;

    tpm_stm_startup(chip);
    tpm_stm_selftest(chip);
	tpm_stm_physicpresence(chip, 0x20);
	tpm_stm_physicpresence(chip, 0x08);
	tpm_stm_enable(chip);
	tpm_stm_active(chip);
	tpm_get_timeouts(chip);
	tpm_do_selftest(chip);

	return 0;
_tpm_clean_answer:
	dev_info(chip->dev, "TPM I2C initialisation fail\n");
	return ret;
}

static int __init tpm_stm_i2c_init(void)
{
	int rc = 0;
	struct  tpm_chip *chip;

	rc = platform_driver_register(&tpm_stm_i2c_drv);
	if (rc)
		return rc;

    gpio_i2c_init();
	pdev = platform_device_register_simple(TPM_ST33_I2C, -1, NULL, 0);
	if (IS_ERR(pdev)) {
		rc = PTR_ERR(pdev);
		goto err_rel_reg;
	}

	if (!(chip = tpm_register_hardware(&pdev->dev, &st_i2c_tpm))) {
		rc = -ENODEV;
		goto err_unreg_dev;
	}

    rc = tpm_stm_i2c_startup(chip);
	if (rc < 0) {
		goto err_unstartup;
	}
	
	return 0;

err_unstartup:
	tpm_remove_hardware(chip->dev);
err_unreg_dev:
	platform_device_unregister(pdev);
err_rel_reg:
	platform_driver_unregister(&tpm_stm_i2c_drv);
	return rc;
}
module_init(tpm_stm_i2c_init);
module_exit(tpm_stm_i2c_exit);

MODULE_AUTHOR("Christophe Ricard (tpmsupport@st.com)");
MODULE_DESCRIPTION("STM TPM I2C ST33 Driver");
MODULE_VERSION("1.2.1");
MODULE_LICENSE("GPL");