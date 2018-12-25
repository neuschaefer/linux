/*
 *  mma8452.c - Linux kernel modules for 3-Axis Orientation/Motion
 *  Detection Sensor 
 *
 *  Copyright (C) 2009-2010 Freescale Semiconductor Ltd.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/hwmon.h>
#include <linux/input-polldev.h>
#include <linux/device.h>
#include <linux/earlysuspend.h>

#include <mach/system.h>
#include <mach/hardware.h>
#include <mach/sys_config.h>

/*
 * Defines
 */
#define assert(expr)\
	if (!(expr)) {\
		printk(KERN_ERR "Assertion failed! %s,%d,%s,%s\n",\
			__FILE__, __LINE__, __func__, #expr);\
	}

#define MMA8452_DRV_NAME	"mma8452"
#define SENSOR_NAME 			MMA8452_DRV_NAME
#define MMA8452_I2C_ADDR	0x1C
#define MMA8452_ID			0x2A

#define POLL_INTERVAL_MAX	500
#define POLL_INTERVAL		100
#define INPUT_FUZZ		32
#define INPUT_FLAT		32
#define MODE_CHANGE_DELAY_MS	100

/* register enum for mma8452 registers */
static union{
	unsigned short dirty_addr_buf[2];
	const unsigned short normal_i2c[2];
}u_i2c_addr = {{0x00},};
static __u32 twi_id = 0;

#ifdef CONFIG_HAS_EARLYSUSPEND	
struct mma8452_data {
    struct i2c_client *client;
    struct early_suspend early_suspend;
};
static struct mma8452_data *mma8452_data;
#endif

enum {
	MMA8452_STATUS = 0x00,
	MMA8452_OUT_X_MSB,
	MMA8452_OUT_X_LSB,
	MMA8452_OUT_Y_MSB,
	MMA8452_OUT_Y_LSB,
	MMA8452_OUT_Z_MSB,
	MMA8452_OUT_Z_LSB,
	
	MMA8452_SYSMOD = 0x0B,
	MMA8452_INT_SOURCE,
	MMA8452_WHO_AM_I,
	MMA8452_XYZ_DATA_CFG,
	MMA8452_HP_FILTER_CUTOFF,
	
	MMA8452_PL_STATUS,
	MMA8452_PL_CFG,
	MMA8452_PL_COUNT,
	MMA8452_PL_BF_ZCOMP,
	MMA8452_PL_P_L_THS_REG,
	
	MMA8452_FF_MT_CFG,
	MMA8452_FF_MT_SRC,
	MMA8452_FF_MT_THS,
	MMA8452_FF_MT_COUNT,

	MMA8452_TRANSIENT_CFG = 0x1D,
	MMA8452_TRANSIENT_SRC,
	MMA8452_TRANSIENT_THS,
	MMA8452_TRANSIENT_COUNT,
	
	MMA8452_PULSE_CFG,
	MMA8452_PULSE_SRC,
	MMA8452_PULSE_THSX,
	MMA8452_PULSE_THSY,
	MMA8452_PULSE_THSZ,
	MMA8452_PULSE_TMLT,
	MMA8452_PULSE_LTCY,
	MMA8452_PULSE_WIND,
	
	MMA8452_ASLP_COUNT,
	MMA8452_CTRL_REG1,
	MMA8452_CTRL_REG2,
	MMA8452_CTRL_REG3,
	MMA8452_CTRL_REG4,
	MMA8452_CTRL_REG5,
	
	MMA8452_OFF_X,
	MMA8452_OFF_Y,
	MMA8452_OFF_Z,
	
	MMA8452_REG_END,
};

enum {
	MODE_2G = 0,
	MODE_4G,
	MODE_8G,
};

/* mma8452 status */
struct mma8452_status {
	u8 mode;
	u8 ctl_reg1;
};

static struct mma8452_status mma_status = {
	.mode 	= 0,
	.ctl_reg1	= 0
};

static struct input_polled_dev *mma8452_idev;
static struct device *hwmon_dev;
static struct i2c_client *mma8452_i2c_client;


static int gsensor_fetch_sysconfig_para(void)
{
	int ret = -1;
	int device_used = -1;
	__u32 twi_addr = 0;
	char name[I2C_NAME_SIZE];
	script_parser_value_type_t type = SCIRPT_PARSER_VALUE_TYPE_STRING;
		
	printk("========%s===================\n", __func__);
	 
	if(SCRIPT_PARSER_OK != (ret = script_parser_fetch("gsensor_para", "gsensor_used", &device_used, 1))){
	                pr_err("%s: script_parser_fetch err.ret = %d. \n", __func__, ret);
	                goto script_parser_fetch_err;
	}
	if(1 == device_used){
		if(SCRIPT_PARSER_OK != script_parser_fetch_ex("gsensor_para", "gsensor_name", (int *)(&name), &type, sizeof(name)/sizeof(int))){
			pr_err("%s: line: %d script_parser_fetch err. \n", __func__, __LINE__);
			goto script_parser_fetch_err;
		}
		if(strcmp(SENSOR_NAME, name)){
			pr_err("%s: name %s does not match SENSOR_NAME. \n", __func__, name);
			pr_err(SENSOR_NAME);
			//ret = 1;
			return ret;
		}
		if(SCRIPT_PARSER_OK != script_parser_fetch("gsensor_para", "gsensor_twi_addr", &twi_addr, sizeof(twi_addr)/sizeof(__u32))){
			pr_err("%s: line: %d: script_parser_fetch err. \n", name, __LINE__);
			goto script_parser_fetch_err;
		}
		u_i2c_addr.dirty_addr_buf[0] = twi_addr;
		u_i2c_addr.dirty_addr_buf[1] = I2C_CLIENT_END;
		printk("%s: after: gsensor_twi_addr is 0x%x, dirty_addr_buf: 0x%hx. dirty_addr_buf[1]: 0x%hx \n", \
			__func__, twi_addr, u_i2c_addr.dirty_addr_buf[0], u_i2c_addr.dirty_addr_buf[1]);

		if(SCRIPT_PARSER_OK != script_parser_fetch("gsensor_para", "gsensor_twi_id", &twi_id, 1)){
			pr_err("%s: script_parser_fetch err. \n", name);
			goto script_parser_fetch_err;
		}
		printk("%s: twi_id is %d. \n", __func__, twi_id);

		ret = 0;
		
	}else{
		pr_err("%s: gsensor_unused. \n",  __func__);
		ret = -1;
	}

	return ret;

script_parser_fetch_err:
	pr_notice("=========script_parser_fetch_err============\n");
	return ret;

}



/**
 * gsensor_detect - Device detection callback for automatic device creation
 * return value:  
 *                    = 0; success;
 *                    < 0; err
 */
int gsensor_detect(struct i2c_client *client, struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = client->adapter;
	
	if(twi_id == adapter->nr){
		pr_info("%s: Detected chip %s at adapter %d, address 0x%02x\n",
			 __func__, SENSOR_NAME, i2c_adapter_id(adapter), client->addr);

		strlcpy(info->type, SENSOR_NAME, I2C_NAME_SIZE);
		return 0;
	}else{
		return -ENODEV;
	}
}


//static ssize_t mma8452_value_show(struct device *dev,
//		struct device_attribute *attr, char *buf)
//{
//	int i;
//	s8 xyz[3]; 
//	s16 x, y, z;
//
//	for(i=0; i<3; i++)
//		mma7660_read_xyz(i, &xyz[i]);
//
//	/* convert signed 8bits to signed 16bits */
//	x = (((short)xyz[0]) << 8) >> 8;
//	y = (((short)xyz[1]) << 8) >> 8;
//	z = (((short)xyz[2]) << 8) >> 8;
//
//	return sprintf(buf, "x= %d y= %d z= %d\n", x, y, z);
//
//}

static ssize_t mma8452_enable_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{
    unsigned long data;
	int error;

	error = strict_strtoul(buf, 10, &data);
	
	if(error) {
		pr_err("%s strict_strtoul error\n", __FUNCTION__);
		goto exit;
	}

	if(data) {
		error = i2c_smbus_write_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1, mma_status.ctl_reg1);
		assert(error==0);
	} else {
		mma_status.ctl_reg1 = i2c_smbus_read_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1);
	    error = i2c_smbus_write_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1,mma_status.ctl_reg1 & 0xFE);
		assert(error==0);
	}

	return count;

exit:
	return error;
}

static ssize_t mma8452_delay_store(struct device *dev,struct device_attribute *attr,
		const char *buf, size_t count)
{
   unsigned long data;
	int error;

	error = strict_strtoul(buf, 10, &data);
	if (error)
		return error;
	if (data > POLL_INTERVAL_MAX)
		data = POLL_INTERVAL_MAX;
	mma8452_idev->poll_interval = data;

	return count;
		}

static DEVICE_ATTR(enable, S_IRUGO|S_IWUSR|S_IWGRP,
		NULL, mma8452_enable_store);

//static DEVICE_ATTR(value, S_IRUGO|S_IWUSR|S_IWGRP,
//		mma8452_value_show, NULL);
		
static DEVICE_ATTR(delay, S_IRUGO|S_IWUSR|S_IWGRP,
		NULL, mma8452_delay_store);

static struct attribute *mma8452_attributes[] = {
//	&dev_attr_value.attr,
	&dev_attr_enable.attr,
	&dev_attr_delay.attr,
	NULL
};

static struct attribute_group mma8452_attribute_group = {
	.attrs = mma8452_attributes
};
/***************************************************************
 *
 * Initialization function
 */
static int mma8452_init_client(struct i2c_client *client)
{
	int result;

		mma_status.ctl_reg1 = 0x20;
		result = i2c_smbus_write_byte_data(client, MMA8452_CTRL_REG1, mma_status.ctl_reg1);
		assert(result==0);
		
		mma_status.mode	= MODE_2G;
		result = i2c_smbus_write_byte_data(client, MMA8452_XYZ_DATA_CFG, mma_status.mode);
		assert(result==0);
		
		mma_status.ctl_reg1 |= 0x01;
		result = i2c_smbus_write_byte_data(client, MMA8452_CTRL_REG1, mma_status.ctl_reg1);
		assert(result==0);
	
	mdelay(MODE_CHANGE_DELAY_MS);

	return result;
}

/***************************************************************
*
* read sensor data from mma8452
*
***************************************************************/ 				
static int mma8452_read_data(short *x, short *y, short *z) {
	u8	tmp_data[7];



	if (i2c_smbus_read_i2c_block_data(mma8452_i2c_client,MMA8452_OUT_X_MSB,7,tmp_data) < 7) {
		dev_err(&mma8452_i2c_client->dev, "i2c block read failed\n");
			return -3;
	}

	*x = ((tmp_data[0] << 8) & 0xff00) | tmp_data[1];
	*y = ((tmp_data[2] << 8) & 0xff00) | tmp_data[3];
	*z = ((tmp_data[4] << 8) & 0xff00) | tmp_data[5];

	*x = (short)(*x) >> 4;
	*y = (short)(*y) >> 4;
	*z = (short)(*z) >> 4;


	if (mma_status.mode == MODE_4G){
		(*x)=(*x)<<1;
		(*y)=(*y)<<1;
		(*z)=(*z)<<1;
	}
	else if (mma_status.mode == MODE_8G){
		(*x)=(*x)<<2;
		(*y)=(*y)<<2;
		(*z)=(*z)<<2;
	}

	return 0;
}

static void report_abs(void)
{
	short x,y,z;
	int result;
	
	do{
		result=i2c_smbus_read_byte_data(mma8452_i2c_client, MMA8452_STATUS);
	} while (!(result & 0x08));		/* wait for new data */

	if (mma8452_read_data(&x,&y,&z) != 0) {
		//DBG("mma8452 data read failed\n");
		return;
	}
	
	input_report_abs(mma8452_idev->input, ABS_X, x);
	input_report_abs(mma8452_idev->input, ABS_Y, y);
	input_report_abs(mma8452_idev->input, ABS_Z, z);
	input_sync(mma8452_idev->input);
}

static void mma8452_dev_poll(struct input_polled_dev *dev)
{
	report_abs();
} 

#ifdef CONFIG_HAS_EARLYSUSPEND	

static void mma8452_suspend(struct early_suspend *h)
{
	int result;
    struct mma8452_data *mma8452_data = container_of(h, struct mma8452_data, early_suspend);
    mma8452_data = i2c_get_clientdata(mma8452_i2c_client);
	mma_status.ctl_reg1 = i2c_smbus_read_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1);
	result = i2c_smbus_write_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1,mma_status.ctl_reg1 & 0xFE);
	assert(result==0);
	return ;
}

static void mma8452_resume(struct early_suspend *h) //(struct i2c_client *client)
{
	int result;

    struct mma8452_data *mma8452_data = container_of(h, struct mma8452_data, early_suspend);
    mma8452_data = i2c_get_clientdata(mma8452_i2c_client);

	result = i2c_smbus_write_byte_data(mma8452_i2c_client, MMA8452_CTRL_REG1, mma_status.ctl_reg1);
	assert(result==0);
	return ;
}
#endif

static int __devinit mma8452_probe(struct i2c_client *client,
				   const struct i2c_device_id *id)
{
	int result;
	struct input_dev *idev;
	struct i2c_adapter *adapter;
 
	mma8452_i2c_client = client;
	adapter = to_i2c_adapter(client->dev.parent);
	result = i2c_check_functionality(adapter,
					 I2C_FUNC_SMBUS_BYTE |
					 I2C_FUNC_SMBUS_BYTE_DATA);
	assert(result);
	
	printk(KERN_INFO "check mma8452 chip ID\n");
	result = i2c_smbus_read_byte_data(client, MMA8452_WHO_AM_I);

	if (MMA8452_ID != (result)) {	//compare the address value 
		dev_err(&client->dev,"read chip ID 0x%x is not equal to 0x%x!\n", result,MMA8452_ID);
		printk(KERN_INFO "read chip ID failed\n");
		result = -EINVAL;
		goto err_detach_client;
	}

	/* Initialize the MMA8452 chip */
	result = mma8452_init_client(client);
	assert(result==0);

	hwmon_dev = hwmon_device_register(&client->dev);
	assert(!(IS_ERR(hwmon_dev)));

	dev_info(&client->dev, "build time %s %s\n", __DATE__, __TIME__);
  

	/*input poll device register */
	mma8452_idev = input_allocate_polled_device();
	if (!mma8452_idev) {
		dev_err(&client->dev, "alloc poll device failed!\n");
		result = -ENOMEM;
		return result;
	}
	mma8452_idev->poll = mma8452_dev_poll;
	mma8452_idev->poll_interval = POLL_INTERVAL;
	mma8452_idev->poll_interval_max = POLL_INTERVAL_MAX;
	idev = mma8452_idev->input;
	idev->name = MMA8452_DRV_NAME;
	idev->id.bustype = BUS_I2C;
	idev->evbit[0] = BIT_MASK(EV_ABS);

	input_set_abs_params(idev, ABS_X, -8192, 8191, INPUT_FUZZ, INPUT_FLAT);
	input_set_abs_params(idev, ABS_Y, -8192, 8191, INPUT_FUZZ, INPUT_FLAT);
	input_set_abs_params(idev, ABS_Z, -8192, 8191, INPUT_FUZZ, INPUT_FLAT);
	result = input_register_polled_device(mma8452_idev);
	if (result) {
		dev_err(&client->dev, "register poll device failed!\n");
		return result;
	}
	result = sysfs_create_group(&mma8452_idev->input->dev.kobj, &mma8452_attribute_group);
#ifdef CONFIG_HAS_EARLYSUSPEND	
	printk("==register_early_suspend =\n");
	mma8452_data = kzalloc(sizeof(*mma8452_data), GFP_KERNEL);
	if (mma8452_data == NULL) {
		result = -ENOMEM;
		goto err_alloc_data_failed;
	}

	mma8452_data->early_suspend.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 3;	
	mma8452_data->early_suspend.suspend = mma8452_suspend;
	mma8452_data->early_suspend.resume	= mma8452_resume;
	register_early_suspend(&mma8452_data->early_suspend);
#endif

	return result;
err_alloc_data_failed:	
err_detach_client:
	return result;
}

static int __devexit mma8452_remove(struct i2c_client *client)
{
	int result;
	mma_status.ctl_reg1 = i2c_smbus_read_byte_data(client, MMA8452_CTRL_REG1);
	result = i2c_smbus_write_byte_data(client, MMA8452_CTRL_REG1,mma_status.ctl_reg1 & 0xFE);
	assert(result==0);

	hwmon_device_unregister(hwmon_dev);
#ifdef CONFIG_HAS_EARLYSUSPEND	
	 unregister_early_suspend(&mma8452_data->early_suspend);	
#endif
	return result;
}


static const struct i2c_device_id mma8452_id[] = {
	{ MMA8452_DRV_NAME, 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mma8452_id);

static struct i2c_driver mma8452_driver = {
	.class = I2C_CLASS_HWMON,
	.driver = {
		.name	= MMA8452_DRV_NAME,
		.owner	= THIS_MODULE,
	},
//	.suspend = mma8452_suspend,
//	.resume	= mma8452_resume,
	.probe	= mma8452_probe,
	.remove	= __devexit_p(mma8452_remove),
	.id_table = mma8452_id,
	.address_list	= u_i2c_addr.normal_i2c,
};

static int __init mma8452_init(void)
{
	/* register driver */
	int res;
	printk("======%s=========. \n", __func__);
	if(gsensor_fetch_sysconfig_para()){
		printk("%s: err.\n", __func__);
		return -1;
	}

	printk("%s: after fetch_sysconfig_para:  normal_i2c: 0x%hx. normal_i2c[1]: 0x%hx \n", \
	__func__, u_i2c_addr.normal_i2c[0], u_i2c_addr.normal_i2c[1]);
	mma8452_driver.detect = gsensor_detect;
	res = i2c_add_driver(&mma8452_driver);
	if (res < 0){
		printk(KERN_INFO "add mma8452 i2c driver failed\n");
		return -ENODEV;
	}
	printk(KERN_INFO "add mma8452 i2c driver\n");

	return (res);
}

static void __exit mma8452_exit(void)
{
	printk(KERN_INFO "remove mma8452 i2c driver.\n");
	i2c_del_driver(&mma8452_driver);
}

MODULE_AUTHOR("Chen Gang <gang.chen@freescale.com>");
MODULE_DESCRIPTION("MMA8452 3-Axis Orientation/Motion Detection Sensor driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.1");

module_init(mma8452_init);
module_exit(mma8452_exit);

