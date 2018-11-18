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
#include <asm/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/l3g4200dh.h>

MODULE_LICENSE("GPL");


/*
 * L3G4200DH Registers
 */
#define REG_WHO_AM_I		0x0f
#define REG_CTRL_REG1		0x20
#define REG_CTRL_REG2		0x21
#define REG_CTRL_REG3		0x22
#define REG_CTRL_REG4		0x23
#define REG_CTRL_REG5		0x24
#define REG_REFERENCE		0x25
#define REG_OUT_TEMP		0x26
#define REG_STATUS_REG		0x27
#define REG_OUTS		0xa8
#define REG_OUT_X_L		0x28
#define REG_OUT_X_H		0x29
#define REG_OUT_Y_L		0x2a
#define REG_OUT_Y_H		0x2b
#define REG_OUT_Z_L		0x2c
#define REG_OUT_Z_H		0x2d
#define REG_FIFO_CTRL_REG	0x2e
#define REG_FIFO_SRC_REG	0x2f
#define REG_INT1_CFG		0x30
#define REG_INT1_SRC		0x31
#define REG_INT1_TSH_XH		0x32
#define REG_INT1_TSH_XL		0x33
#define REG_INT1_TSH_YH		0x34
#define REG_INT1_TSH_YL		0x35
#define REG_INT1_TSH_ZH		0x36
#define REG_INT1_TSH_ZL		0x37
#define REG_INT1_DURATION	0x38


struct l3g4200dh_data {
	struct input_dev	*input_dev;
	struct work_struct	work;
	struct semaphore	sem;
	struct l3g4200dh_platform_data plat_data;
	s16			offset[3];
	s32			delay;
	int			open_count;
	s8			is_active;
};

static void set_power(s8 on);
static void l3g4200dh_set_active(struct l3g4200dh_data *gyro, s8 is_active);
static void l3g4200dh_start_measurement(struct l3g4200dh_data *gyro);
static void l3g4200dh_stop_measurement(struct l3g4200dh_data *gyro);
static void timer_func(unsigned long arg);

static DEFINE_TIMER(g_polling_timer, timer_func, 0, 0);
static struct i2c_client *g_client;

#define MAKE_S16(a, b) ( (s16) ((((a) << 8) & 0xff00) + ((b) & 0xff)) )

#define MIN_DELAY	(10)

static s32 _i2c_read_byte(struct i2c_client *client, u8 command)
{
	s32 err;

	err = i2c_smbus_read_byte_data(client, command);
	if (err < 0) {
		pr_err("i2c_smbus_read_byte_data: %d\n", err);
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


#define DEF_REG_SHOW(reg) \
static ssize_t reg##_show(struct device *dev, struct device_attribute *attr, char* buf) \
{ \
	return snprintf(buf, 8, "%02x\n", (u8)_i2c_read_byte(g_client, REG_##reg)); \
}

#define DEF_REG_STORE(reg) \
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

#define DIAG_REG_RO(reg) \
DEF_REG_SHOW(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, NULL)

#define DIAG_REG_RW(reg) \
DEF_REG_SHOW(reg) \
DEF_REG_STORE(reg) \
static DEVICE_ATTR(reg, S_IWUSR | S_IRUGO, reg##_show, reg##_store)


DIAG_REG_RO(WHO_AM_I);
DIAG_REG_RW(CTRL_REG1);
DIAG_REG_RW(CTRL_REG2);
DIAG_REG_RW(CTRL_REG3);
DIAG_REG_RW(CTRL_REG4);
DIAG_REG_RW(CTRL_REG5);
DIAG_REG_RW(REFERENCE);
DIAG_REG_RO(OUT_TEMP);
DIAG_REG_RO(STATUS_REG);
DIAG_REG_RO(OUT_X_L);
DIAG_REG_RO(OUT_X_H);
DIAG_REG_RO(OUT_Y_L);
DIAG_REG_RO(OUT_Y_H);
DIAG_REG_RO(OUT_Z_L);
DIAG_REG_RO(OUT_Z_H);
DIAG_REG_RW(FIFO_CTRL_REG);
DIAG_REG_RO(FIFO_SRC_REG);
DIAG_REG_RW(INT1_CFG);
DIAG_REG_RO(INT1_SRC);
DIAG_REG_RW(INT1_TSH_XH);
DIAG_REG_RW(INT1_TSH_XL);
DIAG_REG_RW(INT1_TSH_YH);
DIAG_REG_RW(INT1_TSH_YL);
DIAG_REG_RW(INT1_TSH_ZH);
DIAG_REG_RW(INT1_TSH_ZL);
DIAG_REG_RO(INT1_DURATION);


static ssize_t is_active_show(struct device *dev, struct device_attribute *attr, char* buf)
{
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);
	return snprintf(buf, 8, "%x\n", gyro->is_active);
}
static DEVICE_ATTR(is_active, S_IRUGO, is_active_show, NULL);


static int initialize_device_files(struct device* dev)
{
	int i;
	int err = 0;
	static struct device_attribute* attrs[] = {
		&dev_attr_WHO_AM_I,
		&dev_attr_CTRL_REG1,
		&dev_attr_CTRL_REG2,
		&dev_attr_CTRL_REG3,
		&dev_attr_CTRL_REG4,
		&dev_attr_CTRL_REG5,
		&dev_attr_REFERENCE,
		&dev_attr_OUT_TEMP,
		&dev_attr_STATUS_REG,
		&dev_attr_OUT_X_L,
		&dev_attr_OUT_X_H,
		&dev_attr_OUT_Y_L,
		&dev_attr_OUT_Y_H,
		&dev_attr_OUT_Z_L,
		&dev_attr_OUT_Z_H,
		&dev_attr_FIFO_CTRL_REG,
		&dev_attr_FIFO_SRC_REG,
		&dev_attr_INT1_CFG,
		&dev_attr_INT1_SRC,
		&dev_attr_INT1_TSH_XH,
		&dev_attr_INT1_TSH_XL,
		&dev_attr_INT1_TSH_YH,
		&dev_attr_INT1_TSH_YL,
		&dev_attr_INT1_TSH_ZH,
		&dev_attr_INT1_TSH_ZL,
		&dev_attr_INT1_DURATION,
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

static void set_power(s8 on)
{
	if (on) {
		_i2c_write_byte(g_client, REG_CTRL_REG1, 0x0f);
	} else {
		_i2c_write_byte(g_client, REG_CTRL_REG1, 0x00);
	}
}


static void l3g4200dh_set_active(struct l3g4200dh_data *gyro, s8 is_active)
{
	is_active = is_active ? 1 : 0;
	if (gyro->is_active == is_active) {
		return ;
	}
	if (is_active) {
		/* POWER ON */
		set_power(1);
		l3g4200dh_start_measurement(gyro);
		gyro->is_active = 1;
	} else {
		gyro->is_active = 0;
		l3g4200dh_stop_measurement(gyro);
		/* POWER OFF */
		set_power(0);
	}
	return ;
}


static int read_data(struct l3g4200dh_data *gyro, s32* px, s32* py, s32* pz)
{
	s32 err;
	s32 x, y, z;
	s32 xx, yy, zz;
	u8 values[6];
	s8* R;

	err = i2c_smbus_read_i2c_block_data(g_client, REG_OUTS, sizeof values, values);
	if (err < 0) {
		goto out;
	}
	x = MAKE_S16(values[1], values[0]);
	y = MAKE_S16(values[3], values[2]);
	z = MAKE_S16(values[5], values[4]);

	x += gyro->offset[0];
	y += gyro->offset[1];
	z += gyro->offset[2];
/*
 * Swap the axes if necessory
 */
	R = gyro->plat_data.transformation_matrix;
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
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);

	err = data = _i2c_read_byte(g_client, REG_STATUS_REG);
	if (err < 0) {
		goto out;
	}
	if ((data & 0x08) == 0) {
		goto out;
	}

	err = read_data(gyro, &x, &y, &z);
	if (err < 0) {
		goto out;
	}

	input_report_abs(gyro->input_dev, ABS_X, x);
	input_report_abs(gyro->input_dev, ABS_Y, y);
	input_report_abs(gyro->input_dev, ABS_Z, z);
	input_sync(gyro->input_dev);
out:
	return ;
}


static void timer_func(unsigned long arg)
{
	struct l3g4200dh_data *gyro = (struct l3g4200dh_data*)arg;

	if (!work_pending(&gyro->work)) {
		schedule_work(&gyro->work);
	}

	g_polling_timer.expires = jiffies + msecs_to_jiffies(gyro->delay);
	g_polling_timer.data = (unsigned long)gyro;
	add_timer(&g_polling_timer);
}

static void l3g4200dh_start_measurement(struct l3g4200dh_data *gyro)
{
	pr_debug("called\n");
	_i2c_write_byte(g_client, REG_CTRL_REG1, 0x0f);
	_i2c_write_byte(g_client, REG_CTRL_REG4, 0xa0);

	g_polling_timer.expires = jiffies + msecs_to_jiffies(1);
	g_polling_timer.data = (unsigned long)gyro;
	add_timer(&g_polling_timer);
}

static void l3g4200dh_stop_measurement(struct l3g4200dh_data *gyro)
{
	pr_debug("called\n");
	del_timer_sync(&g_polling_timer);

	cancel_work_sync(&gyro->work);
}


static int l3g4200dh_open(struct inode *inode, struct file* filp)
{
	int retval;
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);

	retval = 0;
	if (down_interruptible(&gyro->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	pr_debug("called: %d\n", gyro->open_count);
	{
		gyro->open_count++;
		if (gyro->open_count == 1) {
			gyro->delay = MIN_DELAY;
		}
	}
	up(&gyro->sem);
out:
	return retval;
}


static int l3g4200dh_release(struct inode *inode, struct file* filp)
{
	int retval;
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);

	retval = 0;
	if (down_interruptible(&gyro->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	pr_debug("called: %d\n\n", gyro->open_count);
	{
		gyro->open_count--;
		if (gyro->open_count < 1) {
			gyro->open_count = 0;
			l3g4200dh_set_active(gyro, 0);
		}
	}
	up(&gyro->sem);
out:
	return retval;
}


static long l3g4200dh_ioctl(
		struct file* filp, unsigned int cmd, unsigned long arg)
{
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);
	int	retval;

	retval = -ENOTTY;
	if (_IOC_TYPE(cmd) != L3G4200DH_IOC_MAGIC) {
		goto out;
	}
	if (_IOC_NR(cmd) >= L3G4200DH_IOC_MAXNR) {
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

	if (down_interruptible(&gyro->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}

	{
		s8 prev;
		s32 n;
		s16 data[3];
		switch (cmd) {
			case L3G4200DH_IOC_GET_ACTIVE:
				n = gyro->is_active;
				pr_debug("L3G4200DH_IOC_GET_ACTIVE: %d\n", n);
				retval = copy_to_user(
							(void __user*)arg, &n, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				break;
			case L3G4200DH_IOC_SET_ACTIVE:
				retval = copy_from_user(
							&n, (void __user*)arg, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("L3G4200DH_IOC_SET_ACTIVE: %d, %df\n",
							n, gyro->is_active);
				l3g4200dh_set_active(gyro, n);
				break;
			case L3G4200DH_IOC_GET_OFS_DATA:
				memcpy(&data[0], gyro->offset, sizeof(gyro->offset));
				retval = copy_to_user(
							(void __user*)arg, data, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("L3G4200DH_IOC_GET_OFS_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)gyro->offset[0],
							(u16)gyro->offset[1],
							(u16)gyro->offset[2]);
				break;
			case L3G4200DH_IOC_SET_OFS_DATA:
				retval = copy_from_user(
							data, (void __user*)arg, sizeof(data));
				if (retval != 0) {
					goto out_locked;
				}
				prev = gyro->is_active;
				l3g4200dh_set_active(gyro, 0);
				memcpy(gyro->offset, &data[0], sizeof(gyro->offset));
				pr_debug("L3G4200DH_IOC_SET_OFS_DATA: "
							"0x%04x, 0x%04x, 0x%04x\n",
							(u16)gyro->offset[0],
							(u16)gyro->offset[1],
							(u16)gyro->offset[2]);
				l3g4200dh_set_active(gyro, prev);
				break;
			case L3G4200DH_IOC_GET_DELAY:
				retval = copy_to_user(
							(void __user*)arg, &gyro->delay, sizeof(gyro->delay));
				if (retval != 0) {
					goto out_locked;
				}
				pr_debug("L3G4200DH_IOC_GET_DELAY: %d\n", gyro->delay);
				break;
			case L3G4200DH_IOC_SET_DELAY:
				l3g4200dh_stop_measurement(gyro);
				retval = copy_from_user(
							&n, (void __user*)arg, sizeof(n));
				if (retval != 0) {
					goto out_locked;
				}
				if (n < MIN_DELAY) {
					n = MIN_DELAY;
				}
				gyro->delay = n;
				prev = gyro->is_active;
				l3g4200dh_set_active(gyro, 0);
				pr_debug("L3G4200DH_IOC_SET_DELAY: %d\n", gyro->delay);
				l3g4200dh_set_active(gyro, prev);
				break;
			default:
				goto out_locked;
		}
	}
	retval = 0;
out_locked:
	up(&gyro->sem);

out:
	return retval;
}


static struct file_operations l3g4200dh_fops = {
	.owner = THIS_MODULE,
	.open = l3g4200dh_open,
	.release = l3g4200dh_release,
	.unlocked_ioctl = l3g4200dh_ioctl,
};

static struct miscdevice l3g4200dh_dev = {
	MISC_DYNAMIC_MINOR,
	"l3g4200dh",
	&l3g4200dh_fops,
};


static int l3g4200dh_probe(struct i2c_client *client,
					const struct i2c_device_id *id)
{
	s32 data;
	int err;
	int retval;
	struct l3g4200dh_data *gyro;

	pr_info("probe: %s\n", client->name);
	retval = 0;

	data = _i2c_read_byte(client, REG_WHO_AM_I);
	if (data != 0xd3) {
		pr_err("unknown device: %2x\n", data);
		retval = -ENODEV;
		goto err;
	}

	gyro = kzalloc(sizeof *gyro, GFP_KERNEL);
	if (gyro == NULL) {
		pr_err("kzalloc()\n");
		retval = -ENOMEM;
		goto err_kzalloc;
	}

	if (client->dev.platform_data == NULL) {
		s8 E[9] = { 1, 0, 0, 
			    0, 1, 0, 
			    0, 0, 1 };
		memcpy(gyro->plat_data.transformation_matrix, E, 
			sizeof gyro->plat_data.transformation_matrix);
	} else {
		memcpy(&gyro->plat_data, client->dev.platform_data,
			sizeof (struct l3g4200dh_platform_data));
	}
	sema_init(&gyro->sem, 1);
	INIT_WORK(&gyro->work, work_func);

	gyro->input_dev = input_allocate_device();
	if (gyro->input_dev == NULL) {
		pr_err("input_allocate_device()\n");
		retval = -ENOMEM;
		goto err_input_allocate_device;
	}

	gyro->input_dev->name = "l3g4200dh";
	set_bit(EV_ABS, gyro->input_dev->evbit);
	input_set_abs_params(gyro->input_dev, ABS_X, -32768, 32767, 0, 0);
	input_set_abs_params(gyro->input_dev, ABS_Y, -32768, 32767, 0, 0);
	input_set_abs_params(gyro->input_dev, ABS_Z, -32768, 32767, 0, 0);

	err = input_register_device(gyro->input_dev);
	if (err) {
		pr_err("input_alloc_device(): %d\n", err);
		retval = err;
		goto err_input_register_device;
	}

	err = misc_register(&l3g4200dh_dev);
	if (err) {
		pr_err("misc_register(): %d\n", err);
		retval = err;
		goto err_misc_register;
	}

	i2c_set_clientdata(client, gyro);
	g_client = client;

	err = initialize_device_files(&client->dev);
	if (err < 0) {
		pr_err("initialize_device_files (%d)\n", err);
	}

	return retval;

err_misc_register:
err_input_register_device:
	input_unregister_device(gyro->input_dev);
err_input_allocate_device:
	kfree(gyro);
err_kzalloc:
err:
	return retval;
}

static int l3g4200dh_remove(struct i2c_client *client)
{
	struct l3g4200dh_data *gyro = i2c_get_clientdata(client);

	pr_debug("called\n");
	misc_deregister(&l3g4200dh_dev);
	input_unregister_device(gyro->input_dev);
	i2c_set_clientdata(client, NULL);
	g_client = NULL;
	kfree(gyro);

	return 0;
}

#ifdef CONFIG_PM

static int l3g4200dh_suspend(struct device* dev)
{
	int retval;
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);

	pr_debug("called\n");
	retval = 0;
	if (down_interruptible(&gyro->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	{
		if (gyro->open_count > 0) {
			l3g4200dh_stop_measurement(gyro);
			set_power(0);
		}
	}
	up(&gyro->sem);
out:
	return retval;
}

static int l3g4200dh_resume(struct device* dev)
{
	int retval;
	struct l3g4200dh_data *gyro = i2c_get_clientdata(g_client);

	pr_debug("called\n");
	retval = 0;
	if (down_interruptible(&gyro->sem)) {
		retval = -ERESTARTSYS;
		goto out;
	}
	{
		if (gyro->open_count > 0) {
			l3g4200dh_set_active(gyro, gyro->is_active);
		}
	}
	up(&gyro->sem);
out:
	return retval;
}

static struct dev_pm_ops l3g4200dh_pm_ops = {
	.suspend = l3g4200dh_suspend,
	.resume = l3g4200dh_resume,
};

#define L3G4200DH_PM_OPS (&l3g4200dh_pm_ops)

#else /* CONFIG_PM */
#define L3G4200DH_PM_OPS NULL


#endif /* CONFIG_PM */

static struct i2c_device_id l3g4200dh_idtable[] = {
	{ "l3g4200dh", 0 },
	{ }
};

MODULE_DEVICE_TABLE(i2c, l3g4200dh_idtable);

static struct i2c_driver l3g4200dh_driver = {
	.driver = {
		.name = "l3g4200dh",
		.owner = THIS_MODULE,
		.pm = L3G4200DH_PM_OPS,
	},
	.probe = l3g4200dh_probe,
	.remove = l3g4200dh_remove,
	.id_table = l3g4200dh_idtable,
};

static int __init l3g4200dh_driver_init(void)
{
	int res;

	res = i2c_add_driver(&l3g4200dh_driver);

	return res;
}

static void __exit l3g4200dh_driver_exit(void)
{
	i2c_del_driver(&l3g4200dh_driver);
}

module_init(l3g4200dh_driver_init);
module_exit(l3g4200dh_driver_exit);

