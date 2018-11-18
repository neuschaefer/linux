/*
 * tc35892.c - tc35892 ioexpander driver
 *
 * Copyright (C) 2010-2012 Sony Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/i2c.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <media/tc35892.h>
#include <linux/crc16.h>
#include <mach/gpio.h>
#include <linux/err.h>
#include <../gpio-names.h>
#include <mach/tegra_odm_fuses.h>
#include <media/tegra_camera.h>


struct tc35892_info {
    struct i2c_client *i2c_client;
    struct tc35892_platform_data *pdata;
};

#define MODELID_EVT 0x1c
#define MODELID_DVT 0x18

#define TC35892_TABLE_WAIT_MS 0
#define TC35892_TABLE_END     1
#define TC35892_MAX_RETRIES   3

#define REG_KBDSIZE         0x03
#define REG_KBDDEDCFG       0x04
#define REG_IOCFG           0xA7
#define REG_IOPC0           0xAA
#define REG_IOPC1           0xAC
#define REG_IOPC2           0xAE
#define REG_GPIODATA1       0xC2
#define REG_GPIODATA1MASK   0xC3
#define REG_GPIODATA2       0xC4
#define REG_GPIODATA2MASK   0xC5
#define REG_GPIODIR1        0xC7
#define REG_GPIODIR2        0xC8

#define REG_RSTINTCLR       0x84
#define REG_CLKMODE         0x88
#define REG_RSTCTRL         0x82
#define REG_GPIODIR0        0xC6
#define REG_GPIODATA0       0xC0
#define REG_GPIODATA0MASK   0xC1

static int tc35892_read8(struct i2c_client *client, u8 addr, u8 *val)
{
    int err;
    struct i2c_msg msg[2];

    if (!client->adapter)
        return -ENODEV;

    msg[0].addr = client->addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &addr;

    msg[1].addr = client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].len = 1;
    msg[1].buf = val;


    err = i2c_transfer(client->adapter, msg, 2);

    if (err != 2) {
        printk("%s read reg 0x%04x error\n",__func__, addr);
        return -EINVAL;
    }
//    printk(">>>>>>>addr=%#x,val=%#x\n",addr,*val);
    return 0;
}
static int tc35892_read16(struct i2c_client *client, u8 addr, u16 *val)
{
    int err;
    struct i2c_msg msg[2];
    u8 data[2];

    if (!client->adapter)
        return -ENODEV;

    msg[0].addr = client->addr;
    msg[0].flags = 0;
    msg[0].len = 1;
    msg[0].buf = &addr;

    msg[1].addr = client->addr;
    msg[1].flags = I2C_M_RD;
    msg[1].len = 2;
    msg[1].buf = data;


    err = i2c_transfer(client->adapter, msg, 2);

    if (err != 2) {
        printk("%s read reg 0x%04x error\n",__func__, addr);
        return -EINVAL;
    }
    *val=data[0] | data[1]<<8;
 //   printk(">>>>>>>addr=%#x,val=%#x\n",addr,*val);

    return 0;
}

static int tc35892_write8(struct i2c_client *client, u8 addr, u8 val)
{
    int err;
    struct i2c_msg msg;
    unsigned char data[2];
    int retry = 0;

//    printk("%s ,addr=%#x,val=%#x\n",__func__,addr,val);

    if (!client->adapter)
        return -ENODEV;

    data[0] = addr;
    data[1] = val;

    msg.addr = client->addr;
    msg.flags = 0;
    msg.len = 2;
    msg.buf = data;

    do {
        err = i2c_transfer(client->adapter, &msg, 1);
        if (err == 1)
            return 0;
        retry++;
        pr_err("tc35892: i2c transfer failed, retrying %x %x\n",
                addr, val);
        msleep(3);
    } while (retry <= TC35892_MAX_RETRIES);

    return err;
}

static int tc35892_write16(struct i2c_client *client, u8 addr, u16 val)
{
    int err;
    struct i2c_msg msg;
    unsigned char data[3];
    int retry = 0;

//    printk("%s ,addr=%#x,val=%#x\n",__func__,addr,val);

    if (!client->adapter)
        return -ENODEV;

    data[0] = addr;
    data[1] = (u8)(val & 0x00FF);
    data[2] = (u8)(val >> 8);        


    msg.addr = client->addr;
    msg.flags = 0;
    msg.len = 3;
    msg.buf = data;

    do {
        err = i2c_transfer(client->adapter, &msg, 1);
        if (err == 1)
            return 0;
        retry++;
        pr_err("tc35892: i2c transfer failed, retrying %x %x\n",
                addr, val);
        msleep(3);
    } while (retry <= TC35892_MAX_RETRIES);

    return err;
}


static int tc35892_io_init(struct i2c_client *client)
{
    tc35892_write8(client, REG_RSTINTCLR, 0x01);
    tc35892_write8(client, REG_CLKMODE, 0x01);
    tc35892_write8(client, REG_RSTCTRL, 0x1F);
    tc35892_write8(client, REG_RSTCTRL, 0x00);

    tc35892_write8(client, REG_IOCFG, 0xF8);    // set BALLCFG to 0, let the KPY8 use GPIO16

    tc35892_write8(client, REG_GPIODIR0, 0xFF);
    tc35892_write8(client, REG_GPIODIR1, 0xFF);
    tc35892_write8(client, REG_GPIODIR2, 0xFF);

    tc35892_write8(client, REG_GPIODATA0MASK, 0xFF);
    tc35892_write8(client, REG_GPIODATA1MASK, 0xFF);
    tc35892_write8(client, REG_GPIODATA2MASK, 0xFF);

    return 0;
}

static long tc35892_powerOnCamera(__u32 cameraIndex, struct tc35892_info *info)
{
    int status = tc35892_io_init(info->i2c_client);
    if(status)
    {
        return -EINVAL;
    }

    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0808); // set CAM_RST_VGA to high
    tc35892_write16(info->i2c_client, REG_GPIODATA2, 0x0101); // set 1.8V_CAM_VGA_EN to high
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x8080); // set 2.8V_CAM_VGA_EN to high

    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x4040); // set 1.2V_CAM_5M_EN to high
    mdelay(1);
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x2020); // set 1.8V_CAM_5M_EN to high
    mdelay(1);
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x1010); // set 2.8V_CAM_5M_EN to high
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0800); // set CAM_RST_VGA to low
    mdelay(1);
    info->pdata->vcm_power_on();                              // VCM is  power on

    msleep(100);
    tegra_camera_enable_mclk();

    mdelay(1);

    if ( 0 == cameraIndex) {
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0202); // set CAM_RESET_5M to high
        mdelay(6);
        udelay(700);    
    } else if ( 1 == cameraIndex) {
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0808); // set CAM_RST_VGA to high
    }

    return 0;
}

static long tc35892_powerOffCamera(__u32 cameraIndex, struct tc35892_info *info)
{

    if ( 0 == cameraIndex) {
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0100); // set CAM_STANDBY_5M to Low
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0200); // set CAM_RESET_5M to low    
    } else if ( 1 == cameraIndex) {
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0800); // set CAM_RST_VGA to low
    }

    tegra_camera_disable_mclk();
    info->pdata->vcm_power_off();          // VCM is  power off

    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x1000); // set 2.8V_CAM_5M_EN to low
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x2000); // set 1.8V_CAM_5M_EN to low
    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x4000); // set 1.2V_CAM_5M_EN to low

    tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x8000); // set 2.8V_CAM_VGA_EN to low
    tc35892_write16(info->i2c_client, REG_GPIODATA2, 0x0100); // set 1.8V_CAM_VGA_EN to low

    return 0;
}

static long tc35892_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct tc35892_info *info = file->private_data;

    __u32 cameraIndex   = arg;    
    printk("%s\n", __func__);    

    switch (cmd) {
    case TC35892_IOCTL_POWERON:
        return tc35892_powerOnCamera(cameraIndex, info);
    case TC35892_IOCTL_POWEROFF:
        return tc35892_powerOffCamera(cameraIndex, info);
    default:
        return -EINVAL;
    }
    return 0;
}

static struct tc35892_info *info;

int tc35892_set_standby(void)
{
        tc35892_write16(info->i2c_client, REG_GPIODATA1, 0x0101); // set CAM_STANDBY_5M to high
        return 0;
}

static int tc35892_open(struct inode *inode, struct file *file)
{
    u8 data=0;
    pr_info("%s\n", __func__);
    file->private_data = info;
//    if (info->pdata && info->pdata->reset)
//        info->pdata->reset(); 
    return 0;
}

int tc35892_release(struct inode *inode, struct file *file)
{
    pr_info("%s\n", __func__);
    file->private_data = NULL;
    return 0;
}


static const struct file_operations tc35892_fileops = {
    .owner = THIS_MODULE,
    .open = tc35892_open,
    .unlocked_ioctl = tc35892_ioctl,
    .release = tc35892_release,
};

static struct miscdevice tc35892_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "tc35892",
    .fops = &tc35892_fileops,
};

static int tc35892_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int err;
    int status = 0;    

    pr_info("tc35892: probing %s addr=%x.\n", client->name, client->addr);
    info = kzalloc(sizeof(struct tc35892_info), GFP_KERNEL);

    if (!info) {
        pr_err("tc35892: Unable to allocate memory!\n");
        return -ENOMEM;
    }

    err = misc_register(&tc35892_device);
    if (err) {
        pr_err("tc35892: Unable to register misc device!\n");
        kfree(info);
        return err;
    }

    info->pdata = client->dev.platform_data;
    info->i2c_client = client;

    i2c_set_clientdata(client, info);

    if (info->pdata && info->pdata->reset)
        info->pdata->reset(); 

    status = tc35892_io_init(info->i2c_client);
    if(status)
    {
        return -EINVAL;
    }

    return 0;
}

static int tc35892_remove(struct i2c_client *client)
{
    info = i2c_get_clientdata(client);
    misc_deregister(&tc35892_device);
    kfree(info);
    return 0;
}

static const struct i2c_device_id tc35892_id[] = {
    { "tc35892", 0 },
    { },
};

MODULE_DEVICE_TABLE(i2c, tc35892_id);

static struct i2c_driver tc35892_i2c_driver = {
    .driver = {
        .name = "tc35892",
        .owner = THIS_MODULE,
    },
    .probe = tc35892_probe,
    .remove = tc35892_remove,
    .id_table = tc35892_id,
};

static int __init tc35892_init(void)
{
    pr_info("tc35892 I/O Expander driver loading\n");
    return i2c_add_driver(&tc35892_i2c_driver);
}

static void __exit tc35892_exit(void)
{
    i2c_del_driver(&tc35892_i2c_driver);
}

module_init(tc35892_init);
module_exit(tc35892_exit);

