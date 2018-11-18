/*
 * Copyright (C) 2011-2012 Sony Corporation
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <linux/i2c.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/ami304.h>

MODULE_LICENSE("GPL");


/*
 * AMI304 Registers
 */
#define REG_INFO	0x0d
#define REG_WIA		0x0f
#define REG_DATAX	0x10
#define REG_DATAY	0x12
#define REG_DATAZ	0x14
#define REG_INS1	0x16
#define REG_STAT1	0x18
#define REG_INL		0x1a
#define REG_CNTL1	0x1b
#define REG_CNTL2	0x1c
#define REG_CNTL3	0x1d
#define REG_INC1	0x1e
#define REG_B0X		0x20
#define REG_B0Y		0x22
#define REG_B0Z		0x24
#define REG_ITHR1	0x26
#define REG_PRET	0x30
#define REG_CNTL5	0x40
#define REG_CNTL4	0x5c
#define REG_DELAYX	0x68
#define REG_DELAYY	0x6e
#define REG_DELAYZ	0x74
#define REG_OFFX	0x6c
#define REG_OFFY	0x72
#define REG_OFFZ	0x78
#define REG_VER		0xe8
#define REG_SN		0xea

struct ami304_data {
	struct input_dev	*input_dev;
	struct work_struct	work;
	struct semaphore	sem;
	struct ami304_platform_data plat_data;
	s16			b0[3];
	s16			offset[3];
	s32			delay;
	s32			open_count;
	s8			is_active;
};


static void set_power(s8 on);
static void ami304_set_active(struct ami304_data *mag, s8 is_active);
static void ami304_start_measurement(struct ami304_data *magnetometer);
static void ami304_stop_measurement(struct ami304_data *magnetometer);
static void timer_func(unsigned long arg);

static DEFINE_TIMER(g_polling_timer, timer_func, 0, 0);
static struct i2c_client *g_client;

#define MIN_DELAY	(16)

static s32 _i2c_read_byte(struct i2c_client *client, u8 command)
{
	s32 err;

	err = i2c_smbus_read_byte_data(client, command);
	if (err < 0) {
		pr_err("i2c_smbus_read_byte_data: %d\n", err);
	}
	return err;
}


static s32 _i2c_read_word(struct i2c_client *client, u8 command)
{
	s32 err;

	err = i2c_smbus_read_word_data(client, command);
	if (err < 0) {
		pr_err("i2c_smbus_read_word_data: %d\n", err);
	}
	return err;
}


static s32 _i2c_write_byte(struct i2c_client *client, u8 command, u8 value)
{
	s32 err;

	err = i2c_smbus_write_byte_data(client, command, value);
	if (err < 0) {
		pr_err("i2c_smbus_write_byte_data: %d\n", err);
	}
	return err;
}

static s32 _i2c_write_word(struct i2c_client *client, u8 command, u16 value)
{
	s32 err;

	err = i2c_smbus_write_word_data(client, command, value);
	if (err < 0) {
		pr_err("i2c_smbus_write_word_data: %d\n", err);
	}
	return err;
}


#define DEF_REG8_SHOW(reg) \
static ssize_t reg##_show(struct device *dev, struct device_attribute *attr, char* buf) \
{ \
	return snprintf(buf, 8, "%02x\n", (u8)_i2c_read_byte(g_client, REG_##reg)); \
}


#define DEF_REG8_STORE(reg) \
static ssize_t reg##_store(struct device *dev, struct device_attribute *attr, \
		const char* buf, size_t count) \
{ \
	s32 err; \
	unsigned long ul = simple_strtoul(buf, NULL, 0); \
	if (ul > 0xff) { \
		return -EINVAL; \
	}\
	err = _i2c_write_byte(g_client, REG_##reg, ul); \
	return err < 0 ? err : count; \
}

#define DEF_REG16_SHOW(reg) \
static ssize_t reg##_show(struct device *dev, struct device_attribute *attr, char* buf) \
{ \
	return snprintf(buf, 8, "%04x\n", (u8)_i2c_read_word(g_client, REG_##reg)); \
}

#define DEF_REG16_STORE(reg) \
static ssize_t reg##_store(struct device *dev, struct device_attribute *attr, \
		const char* buf, size_t count) \
{ \
	s32 err; \
	unsigned long ul = simple_strtoul(buf, NULL, 0); \
	if (ul > 0xffff) { \
		return -EINVAL; \
	}\
	err = _i2c_write_word(g_client, REG_##reg, ul); \
	return err < 0 ? err : count; \
}

#define DIAG_REG8_RO(reg) \
DEF_REG8_SHOW(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, NULL)

#define DIAG_REG8_RW(reg) \
DEF_REG8_SHOW(reg) \
DEF_REG8_STORE(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, reg##_store)

#define DIAG_REG16_RO(reg) \
DEF_REG16_SHOW(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, NULL)

#define DIAG_REG16_RW(reg) \
DEF_REG16_SHOW(reg) \
DEF_REG16_STORE(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, reg##_store)


DIAG_REG16_RO(INFO);
DIAG_REG8_RO(WIA);
DIAG_REG16_RO(DATAX);
DIAG_REG16_RO(DATAY);
DIAG_REG16_RO(DATAZ);
DIAG_REG8_RO(INS1);
DIAG_REG8_RO(STAT1);
DIAG_REG8_RO(INL);
DIAG_REG8_RW(CNTL1);
DIAG_REG8_RW(CNTL2);
DIAG_REG8_RW(CNTL3);
DIAG_REG8_RW(INC1);
DIAG_REG16_RW(B0X);
DIAG_REG16_RW(B0Y);
DIAG_REG16_RW(B0Z);
DIAG_REG16_RW(ITHR1);
DIAG_REG8_RW(PRET);
DIAG_REG16_RW(CNTL4);
DIAG_REG8_RW(DELAYX);
DIAG_REG8_RW(DELAYY);
DIAG_REG8_RW(DELAYZ);
DIAG_REG16_RW(OFFX);
DIAG_REG16_RW(OFFY);
DIAG_REG16_RW(OFFZ);
DIAG_REG16_RO(VER);
DIAG_REG16_RO(SN);


static ssize_t is_active_show(struct device *dev, struct device_attribute *attr, char* buf)
{
	struct ami304_data *mag = i2c_get_clientdata(g_client);
	return snprintf(buf, 8, "%x\n", mag->is_active);
}
static DEVICE_ATTR(is_active, S_IRUGO, is_active_show, NULL);


static int initialize_device_files(struct device* dev)
{
	int i;
	int err = 0;
	static struct device_attribute* attrs[] = {
		&dev_attr_INFO,
		&dev_attr_WIA,
		&dev_attr_DATAX,
		&dev_attr_DATAY,
		&dev_attr_DATAZ,
		&dev_attr_INS1,
		&dev_attr_STAT1,
		&dev_attr_INL,
		&dev_attr_CNTL1,
		&dev_attr_CNTL2,
		&dev_attr_CNTL3,
		&dev_attr_INC1,
		&dev_attr_B0X,
		&dev_attr_B0Y,
		&dev_attr_B0Z,
		&dev_attr_ITHR1,
		&dev_attr_PRET,
		&dev_attr_CNTL4,
		&dev_attr_DELAYX,
		&dev_attr_DELAYY,
		&dev_attr_DELAYZ,
		&dev_attr_OFFX,
		&dev_attr_OFFY,
		&dev_attr_OFFZ,
		&dev_attr_VER,
		&dev_attr_SN,
		&dev_attr_is_active,
	};
	for (i = 0; i < ARRAY_SIZE(attrs); ++i) {
		err = device_create_file(dev, attrs[i]);
		if (err) {
			pr_err("device_create_file: %d\n", err);
			break;
		}
	}
	return err;
}


static int read_data(struct ami304_data *mag, s32* px, s32* py, s32* pz)
{
	s32 err;
	s32 x, y, z;
	s32 xx, yy, zz;
	s8* R;

	err = _i2c_read_word(g_client, REG_DATAX);
	x = (s16)err;
	if (err < 0) {
		goto out;
	}
	err = _i2c_read_word(g_client, REG_DATAY);
	y = (s16)err;
	if (err < 0) {
		goto out;
	}
	err = _i2c_read_word(g_client, REG_DATAZ);
	z = (s16)err;
	if (err < 0) {
		goto out;
	}
/*
 * Swap the axes if necessory
 */
	R = mag->plat_data.transformation_matrix;
	xx = R[0] * x + R[1] * y + R[2] * z;
	yy = R[3] * x + R[4] * y + R[5] * z;
	zz = R[6] * x + R[7] * y + R[8] * z;

	*px = xx;
	*py = yy;
	*pz = zz;
	err = 0;
out:
	return err;
}


static void work_func(struct work_struct *work)
{
	s32 err;
	s32 x,y,z;
	s32 data;
	struct ami304_data *mag = i2c_get_clientdata(g_client);

	err = data = _i2c_read_byte(g_client, REG_STAT1);
	if (err < 0) {
		goto out;
	}
	if ((data & 0x40) == 0) {
		goto out;
	}

	err = read_data(mag, &x, &y, &z);
	if (err < 0) {
		goto out;
	}

	input_report_abs(mag->input_dev, ABS_X, x);
	input_report_abs(mag->input_dev, ABS_Y, y);
	input_report_abs(mag->input_dev, ABS_Z, z);
	input_sync(mag->input_dev);
out:
	return;
}


static void set_power(s8 on)
{
	if (on) {
		_i2c_write_byte(g_client, REG_CNTL1, 0x82);
	} else {
		_i2c_write_byte(g_client, REG_CNTL1, 0x00);
	}
}


static void timer_func(unsigned long arg)
{
	struct ami304_data *mag = (struct ami304_data*)arg;

	if (!work_pending(&mag->work)) {
		schedule_work(&mag->work);
	}

	g_polling_timer.expires = jiffies + msecs_to_jiffies(mag->delay);
	g_polling_timer.data = (unsigned long)mag;
	add_timer(&g_polling_timer);
}


static void ami304_start_measurement(struct ami304_data *mag)
{
	pr_debug("called\n");
	_i2c_write_byte(g_client, REG_CNTL1, 0x82);
	_i2c_write_byte(g_client, REG_CNTL2, 0x08);

	_i2c_write_word(g_client, REG_OFFX, mag->offset[0]);
	_i2c_write_word(g_client, REG_OFFY, mag->offset[1]);
	_i2c_write_word(g_client, REG_OFFZ, mag->offset[2]);
	_i2c_write_word(g_client, REG_B0X, mag->b0[0]);
	_i2c_write_word(g_client, REG_B0Y, mag->b0[1]);
	_i2c_write_word(g_client, REG_B0Z, mag->b0[2]);
	_i2c_write_byte(g_client, REG_CNTL1, 0x82);
	_i2c_write_byte(g_client, REG_CNTL1, 0x80);

	g_polling_timer.expires = jiffies + msecs_to_jiffies(1);
	g_polling_timer.data = (unsigned long)mag;
	add_timer(&g_polling_timer);
}


static void ami304_stop_measurement(struct ami304_data *mag)
{
	pr_debug("called\n");
	del_timer_sync(&g_polling_timer);

	cancel_work_sync(&mag->work);
}


static void ami304_set_active(struct ami304_data *mag, s8 is_active)
{
	is_active = is_active ? 1 : 0;
	if (mag->is_active == is_active) {
		return ;
	}
	if (is_active) {
		/* POWER ON */
		set_power(1);
		ami304_start_measurement(mag);
		mag->is_active = 1;
	} else {
		mag->is_active = 0;
		ami304_stop_measurement(mag);
		/* POWER OFF */
		set_power(0);
	}
	return ;
}


static void do_calibration(struct ami304_data *mag)
{
	int i;
	int k;
	s32 data;
	s32 v;
	int fine_dat;
	int diff_dat[3];
	u16 wk_dat[3];
	u16 off_dat[3];
	static const int OFF_REG[3] = { REG_OFFX, REG_OFFY, REG_OFFZ };
	static const int DAT_REG[3] = { REG_DATAX, REG_DATAY, REG_DATAZ };

	pr_debug("Calibrating...\n");

	set_power(1);

	/* Software Reset */
	_i2c_write_byte(g_client, REG_CNTL3, 0x80);

	_i2c_write_byte(g_client, REG_CNTL1, 0x82);
	_i2c_write_byte(g_client, REG_CNTL2, 0x08);

	/* B0 load */
	_i2c_write_byte(g_client, REG_CNTL3, 0x10);
	_i2c_write_byte(g_client, REG_CNTL3, 0x00);

	/* Gyro measurement mode */
	_i2c_write_word(g_client, REG_CNTL4, 0x0001);
	for (i = 0; i < 3; ++i) {
		off_dat[i] = 0;
		diff_dat[i] = 9999;
		wk_dat[i] = (u16)_i2c_read_word(g_client, OFF_REG[i]);
	}
	for (fine_dat = 0; fine_dat < 64; ++fine_dat) {
		for (i = 0; i < 3; ++i) {
			wk_dat[i] = (wk_dat[i] & 0xffc0) | fine_dat;
			_i2c_write_word(g_client, OFF_REG[i], wk_dat[i]);
		}

		_i2c_write_byte(g_client, REG_CNTL3, 0x40);
		for (k = 0; k < 10000; ++k) {
			data = _i2c_read_byte(g_client, REG_STAT1);
			if (data & 0x40) {
				break;
			}
			msleep(1);
		}
		for (i = 0; i < 3; ++i) {
			v = (s16)_i2c_read_word(g_client, DAT_REG[i]);
			v = abs(v);
			if (diff_dat[i] > v) {
				off_dat[i] = wk_dat[i];
				diff_dat[i] = v;
			}
		}
	}
	for (i = 0; i < 3; ++i) {
		_i2c_write_word(g_client, OFF_REG[i], off_dat[i]);
		mag->offset[i] = off_dat[i];
	}

	_i2c_write_byte(g_client, REG_CNTL5, 0x03);

	_i2c_write_byte(g_client, REG_DELAYX, 0xfa);
	_i2c_write_byte(g_client, REG_DELAYY, 0xfa);
	_i2c_write_byte(g_client, REG_DELAYZ, 0xfa);

	_i2c_write_byte(g_client, REG_CNTL3, 0x40);
	for (k = 0; k < 10000; ++k) {
		data = _i2c_read_byte(g_client, REG_STAT1);
		if (data & 0x40) {
			break;
		}
		msleep(1);
	}
	mag->b0[0] = (s16)_i2c_read_word(g_client, REG_DATAX);
	mag->b0[1] = (s16)_i2c_read_word(g_client, REG_DATAY);
	mag->b0[2] = (s16)_i2c_read_word(g_client, REG_DATAZ);

	pr_debug("OFFX: 0x%04x\n", (u16)_i2c_read_word(g_client, REG_OFFX));
	pr_debug("OFFY: 0x%04x\n", (u16)_i2c_read_word(g_client, REG_OFFY));
	pr_debug("OFFZ: 0x%04x\n", (u16)_i2c_read_word(g_client, REG_OFFZ));
	pr_debug("\nB0X: %d (0x%04x)\nB0Y: %d (0x%04x)\nB0Z: %d (0x%04x)\n",
		mag->b0[0], (u16)mag->b0[0],
		mag->b0[1], (u16)mag->b0[1],
		mag->b0[2], (u16)mag->b0[2]);

	/* Software Reset */
	_i2c_write_byte(g_client, REG_CNTL3, 0x80);

	set_power(0);
}


static int ami304_open(struct inode *inode, struct file* filp)
{
	struct ami304_data *mag = i2c_get_clientdata(g_client);
	int retval;

	retval = 0;
	if (down_interruptible(&mag->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	pr_debug("called: %d\n", mag->open_count);
	{
		mag->open_count++;
		if (mag->open_count == 1) {
			mag->delay = MIN_DELAY;
		}
	}
	up(&mag->sem);
out:
	return retval;
}


static int ami304_release(struct inode *inode, struct file* filp)
{
	struct ami304_data *mag = i2c_get_clientdata(g_client);
	int retval;

	retval = 0;
	if (down_interruptible(&mag->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	pr_debug("called: %d\n", mag->open_count);
	{
		mag->open_count--;
		if (mag->open_count < 1) {
			mag->open_count = 0;
			ami304_set_active(mag, 0);
		}
	}
	up(&mag->sem);
out:
	return retval;
}

static long ami304_ioctl(
		struct file* filp, unsigned int cmd, unsigned long arg)
{
	struct ami304_data *mag = i2c_get_clientdata(g_client);
	int	 retval;

	retval = -ENOTTY;
	if (_IOC_TYPE(cmd) != AMI304_IOC_MAGIC) {
		goto out;
	}
	if (_IOC_NR(cmd) >= AMI304_IOC_MAXNR) {
		goto out;
	}
	if (_IOC_DIR(cmd) & _IOC_READ) {
		if (!access_ok(VERIFY_READ,
				(void __user *)arg, _IOC_SIZE(cmd))) {
			retval = -EFAULT;
			goto out;
		}
	} else if (_IOC_DIR(cmd) & _IOC_WRITE) {
		if (!access_ok(VERIFY_WRITE,
					(void __user *)arg, _IOC_SIZE(cmd))) {
			retval = -EFAULT;
			goto out;
		}
	}

	if (down_interruptible(&mag->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}

	{
		s8 prev;
		s32 n;
		s16 data[3];
		s16 calib_data[6];
		switch (cmd) {
			case AMI304_IOC_GET_ACTIVE:
				n = mag->is_active;
				pr_debug("AMI304_IOC_GET_ACTIVE: %d\n", n);
				retval = copy_to_user(
							(void __user*)arg, &n, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				break;
			case AMI304_IOC_SET_ACTIVE:
				retval = copy_from_user(
							&n, (void __user*)arg, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("AMI304_IOC_SET_ACTIVE: %d, %d\n",
							n, mag->is_active);
				ami304_set_active(mag, n);
				break;
			case AMI304_IOC_DO_CALIBRATION:
				prev = mag->is_active;
				ami304_set_active(mag, 0);
				do_calibration(mag);
				memcpy(&calib_data[0], mag->offset, sizeof(mag->offset));
				memcpy(&calib_data[3], mag->b0, sizeof(mag->b0));
				retval = copy_to_user(
							(void __user*)arg, calib_data, sizeof(calib_data));
				ami304_set_active(mag, prev);
				if (retval != 0) {
					goto out_locked;
				}
				break;
			case AMI304_IOC_GET_OFS_DATA:
				memcpy(&data[0], mag->offset, sizeof(mag->offset));
				retval = copy_to_user(
							(void __user*)arg, data, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("AMI304_IOC_GET_OFS_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)mag->offset[0],
							(u16)mag->offset[1],
							(u16)mag->offset[2]);
				break;
			case AMI304_IOC_SET_OFS_DATA:
				retval = copy_from_user(
							data, (void __user*)arg, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				prev = mag->is_active;
				ami304_set_active(mag, 0);
				memcpy(mag->offset, &data[0], sizeof(mag->offset));
				pr_debug("AMI304_IOC_SET_OFS_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)mag->offset[0],
							(u16)mag->offset[1],
							(u16)mag->offset[2]);
				ami304_set_active(mag, prev);
				break;
			case AMI304_IOC_GET_B0_DATA:
				memcpy(&data[0], mag->b0, sizeof(mag->b0));
				retval = copy_to_user(
							(void __user*)arg, data, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("AMI304_IOC_GET_B0_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)mag->b0[0],
							(u16)mag->b0[1],
							(u16)mag->b0[2]);
				break;
			case AMI304_IOC_SET_B0_DATA:
				retval = copy_from_user(
							data, (void __user*)arg, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				prev = mag->is_active;
				ami304_set_active(mag, 0);
				memcpy(mag->b0, &data[0], sizeof(mag->b0));
				pr_debug("AMI304_IOC_SET_B0_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)mag->b0[0],
							(u16)mag->b0[1],
							(u16)mag->b0[2]);
				ami304_set_active(mag, prev);
				break;
			case AMI304_IOC_GET_DELAY:
				retval = copy_to_user(
							(void __user*)arg, &mag->delay, sizeof(mag->delay));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("AMI304_IOC_GET_DELAY: %d\n", mag->delay);
				break;
			case AMI304_IOC_SET_DELAY:
				retval = copy_from_user(
							&n, (void __user*)arg, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				if (n < MIN_DELAY) {
					n = MIN_DELAY;
				}
				mag->delay = n;
				prev = mag->is_active;
				ami304_set_active(mag, 0);
				pr_debug("AMI304_IOC_SET_DELAY: %d\n", mag->delay);
				ami304_set_active(mag, prev);
				break;
			default:
				goto out_locked;
		}
	}
	retval = 0;
out_locked:
	up(&mag->sem);

out:
	return retval;
}


static struct file_operations ami304_fops = {
	.owner = THIS_MODULE,
	.open = ami304_open,
	.release = ami304_release,
	.unlocked_ioctl = ami304_ioctl,
};

static struct miscdevice ami304_dev = {
	MISC_DYNAMIC_MINOR,
	"ami304",
	&ami304_fops,
};


static int ami304_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	s32 data;
	int err;
	int retval;
	struct ami304_data *mag;

	pr_info("probe: %s\n", client->name);
	retval = 0;

	data = _i2c_read_byte(client, REG_WIA);
	if (data != 0x47) {
		pr_err("unknown device: %2x\n", data);
		retval = -ENODEV;
		goto err;
	}

	mag = kzalloc(sizeof *mag, GFP_KERNEL);
	if (mag == NULL) {
		pr_err("kzalloc()\n");
		retval = -ENOMEM;
		goto err_kzalloc;
	}

	if (client->dev.platform_data == NULL) {
		s8 E[9] = { 1, 0, 0, 
			    0, 1, 0, 
			    0, 0, 1 };
		memcpy(mag->plat_data.transformation_matrix, E, 
			sizeof mag->plat_data.transformation_matrix);
	} else {
		memcpy(&mag->plat_data, client->dev.platform_data,
			sizeof (struct ami304_platform_data));
	}
	sema_init(&mag->sem, 1);
	INIT_WORK(&mag->work, work_func);

	mag->input_dev = input_allocate_device();
	if (mag->input_dev == NULL) {
		pr_err("input_allocate_device()\n");
		retval = -ENOMEM;
		goto err_input_allocate_device;
	}

	mag->input_dev->name = "ami304";
	set_bit(EV_ABS, mag->input_dev->evbit);
	input_set_abs_params(mag->input_dev, ABS_X, -32768, 32767, 0, 0);
	input_set_abs_params(mag->input_dev, ABS_Y, -32768, 32767, 0, 0);
	input_set_abs_params(mag->input_dev, ABS_Z, -32768, 32767, 0, 0);

	err = input_register_device(mag->input_dev);
	if (err) {
		pr_err("input_alloc_device(): %d\n", err);
		retval = err;
		goto err_input_register_device;
	}

	err = misc_register(&ami304_dev);
	if (err) {
		pr_err("misc_register(): %d\n", err);
		retval = err;
		goto err_misc_register;
	}

	i2c_set_clientdata(client, mag);
	g_client = client;

	/* POWER ON */
	_i2c_write_byte(g_client, REG_CNTL1, 0x80);
	_i2c_write_byte(g_client, REG_CNTL3, 0x80); /* SW Reset */
	_i2c_write_byte(g_client, REG_CNTL1, 0x82);

	/* B0 load */
	_i2c_write_byte(g_client, REG_CNTL3, 0x10);
	_i2c_write_byte(g_client, REG_CNTL3, 0x00);

	/* Read Default Offset Value */
	mag->offset[0] = _i2c_read_word(g_client, REG_OFFX);
	mag->offset[1] = _i2c_read_word(g_client, REG_OFFY);
	mag->offset[2] = _i2c_read_word(g_client, REG_OFFZ);
	pr_debug("Initial OFFX: 0x%04x\n", (u16)mag->offset[0]);
	pr_debug("Initial OFFY: 0x%04x\n", (u16)mag->offset[1]);
	pr_debug("Initial OFFZ: 0x%04x\n", (u16)mag->offset[2]);
	mag->b0[0] = _i2c_read_word(g_client, REG_B0X);
	mag->b0[1] = _i2c_read_word(g_client, REG_B0Y);
	mag->b0[2] = _i2c_read_word(g_client, REG_B0Z);
	pr_debug("Initial B0X: 0x%04x\n", (u16)mag->b0[0]);
	pr_debug("Initial B0Y: 0x%04x\n", (u16)mag->b0[1]);
	pr_debug("Initial B0Z: 0x%04x\n", (u16)mag->b0[2]);

	/* POWER OFF */
	_i2c_write_byte(g_client, REG_CNTL1, 0x00);

	err = initialize_device_files(&client->dev);
	if (err < 0) {
		pr_err("initialize_device_files (%d)\n", err);
	}

	return retval;

err_misc_register:
err_input_register_device:
	input_unregister_device(mag->input_dev);
err_input_allocate_device:
	kfree(mag);
err_kzalloc:
err:
	return retval;
}

static int ami304_remove(struct i2c_client *client)
{
	struct ami304_data *mag = i2c_get_clientdata(client);

	pr_debug("called\n");
	misc_deregister(&ami304_dev);
	input_unregister_device(mag->input_dev);
	i2c_set_clientdata(client, NULL);
	g_client = NULL;
	kfree(mag);

	return 0;
}

#ifdef CONFIG_PM

static int ami304_suspend(struct device* dev)
{
	int retval;
	struct ami304_data *mag = i2c_get_clientdata(g_client);

	pr_debug("called\n");
	retval = 0;
	if (down_interruptible(&mag->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	{
		if (mag->open_count > 0) {
			ami304_stop_measurement(mag);
			set_power(0);
		}
	}
	up(&mag->sem);
out:
	return retval;
}

static int ami304_resume(struct device* dev)
{
	int retval;
	struct ami304_data *mag = i2c_get_clientdata(g_client);

	pr_debug("called\n");
	retval = 0;
	if (down_interruptible(&mag->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	{
		if (mag->open_count > 0) {
			ami304_set_active(mag, mag->is_active);
		}
	}
	up(&mag->sem);
out:
	return retval;
}

static struct dev_pm_ops ami304_pm_ops = {
	.suspend = ami304_suspend,
	.resume = ami304_resume,
};

#define AMI304_PM_OPS (&ami304_pm_ops)

#else /* CONFIG_PM */
#define AMI304_PM_OPS NULL

#endif /* CONFIG_PM */

MODULE_DEVICE_TABLE(i2c, ami304_idtable);

static struct i2c_device_id ami304_idtable[] = {
	{ "ami304", 0 },
	{ }
};

static struct i2c_driver ami304_driver = {
	.driver = {
		.name = "ami304",
		.owner = THIS_MODULE,
		.pm = AMI304_PM_OPS,
	},
	.probe = ami304_probe,
	.remove = ami304_remove,
	.id_table = ami304_idtable,
};

static int __init ami304_driver_init(void)
{
	int res;

	res = i2c_add_driver(&ami304_driver);

	return res;
}

static void __exit ami304_driver_exit(void)
{
	i2c_del_driver(&ami304_driver);
}

module_init(ami304_driver_init);
module_exit(ami304_driver_exit);

