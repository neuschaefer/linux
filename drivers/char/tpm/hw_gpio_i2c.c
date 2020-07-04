/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  版权所有  : 2013-2020，华为技术有限公司
  文 件 名  : Hw_gpio_i2c.c
  版           本  : 1.0
  创建日期  : 2013-09-05
  描          述  :   该文件中使用2个GPIO来模拟I2C控制器.用一个GPIO来做时
                           钟线,另外一个GPIO来做数据线.
  函数列表  :
                        01. i2c_gpio_set_val        设置GPIO值
                        02. i2c_gpio_set_dir        设置GPIO方向,输入还是输出
                        03. gpio_i2c_start           向I2C SLAVE设备发送I2C启动的时序
                        04. gpio_i2c_stop            向I2C SLAVE设备发送I2C结束的时序
                        05. gpio_i2c_send_ack     发送ACK信号,如果是最后一个字节则发送NACK信号
                        06. gpio_get_value          获取SDA GPIO的值
                        07. gpio_i2c_receive_ack  接收ACK信号
                        08. gpio_i2c_send_byte    发送一个字节的数据
                        09. gpio_i2c_read_byte     接收一个字节的数据
                        10. gpio_i2c_write            向I2C 写数据
                        11. gpio_i2c_read             向I2C读数据
                        12. gpio_i2c_init               初始化GPIO SDA, GPIO SCL的值, 获取I2C的地址等.
  历史记录      :
   1.日    期   : 2013-09-05
     修改内容   : 完成初稿
   2.日    期   : 2013-10-20
     修改内容   : 增加ATMEL芯片的支持和升级
   3.日    期   : 2015-04-30
     修改内容   : 移植到HISI5610平台
*****************************************************************************/

/* Includes. */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/capability.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/string.h>

#include <linux/version.h>
#include <linux/slab.h>
#include "bhal.h"

#define GPIO_DELAY      3

#define GPIO_DIR_OUTPUT 1
#define GPIO_DIR_INPUT  0
#define GPIO_DATA_HIGH  1
#define GPIO_DATA_LOW   0

unsigned int g_sdaGpio = 81;
unsigned int g_sclGpio = 82;
unsigned char g_device_id = 0x13;

unsigned int g_rstGpio = 83;

extern int sd5610x_gpio_bit_write(unsigned int gpio, unsigned int level);
extern int sd5610x_gpio_bit_attr_set(unsigned int gpio, unsigned int mode);
extern int GetGpioKeyStatus(unsigned int gpio_no);
extern void sd5610x_iomux_set_gpio_mode(unsigned int gpio);


static void i2c_gpio_set_val(unsigned int gpio, int state)
{
    sd5610x_gpio_bit_write(gpio, state);

    return;
}

/* Toggle SCL by changing the direction of the pin. */
static void i2c_gpio_set_dir(unsigned int gpio, int state)
{
    sd5610x_gpio_bit_attr_set(gpio, state);

    return;
}

static void gpio_i2c_start(void)
{
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);
    udelay(GPIO_DELAY);
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);

    udelay(GPIO_DELAY);
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);
    udelay(GPIO_DELAY);

    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);
    udelay(GPIO_DELAY);

    return;
}

void gpio_i2c_stop(void)
{
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);
    udelay(GPIO_DELAY);

    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);

    udelay(GPIO_DELAY);
    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);
    udelay(GPIO_DELAY);

    return;
}

void gpio_i2c_send_ack(unsigned char ack)
{
    // 0 ACK   1 NACK
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);
    if (ack)
    {
        i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);
        udelay(GPIO_DELAY);
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
        udelay(GPIO_DELAY);
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);
        udelay(GPIO_DELAY);
    }
    else
    {
        i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);
        udelay(GPIO_DELAY);
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
        udelay(GPIO_DELAY);
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);
        udelay(GPIO_DELAY);
    }

    return;
}

int gpio_get_value(unsigned short bpGpio)
{
    return GetGpioKeyStatus(bpGpio);
}

unsigned char gpio_i2c_receive_ack(void)
{
    unsigned char rc = 0;

    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
    udelay(GPIO_DELAY);

    if (gpio_get_value(g_sdaGpio))
    {
        rc = 1;
    }

    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);
    udelay(GPIO_DELAY);
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);
    udelay(GPIO_DELAY);

    return rc;
}

unsigned char gpio_i2c_send_byte(unsigned char send_byte)
{
    unsigned char rc = 0;
    unsigned char out_mask = 0x80;
    unsigned char value;
    unsigned char count = 8;

    while (count > 0)
    {
        value = ((send_byte & out_mask) ? 1 : 0);
        if (value == 1)
        {
            i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);
        }
        else
        {
            i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_LOW);
        }
        udelay(GPIO_DELAY);

        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
        udelay(GPIO_DELAY);

        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);

        out_mask >>= 1;
        count--;
    }

    udelay(GPIO_DELAY);
    rc = gpio_i2c_receive_ack();

    return rc;

}

void gpio_i2c_read_byte(unsigned char* buffer, unsigned char ack)
{
    unsigned char count = 0x08;
    unsigned char data = 0x00;
    unsigned char temp = 0;

    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_INPUT);

    while (count > 0)
    {
        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
        udelay(GPIO_DELAY);
        temp = gpio_get_value(g_sdaGpio);
        data <<= 1;
        if (temp)
        {
            data |= 0x01;
        }

        i2c_gpio_set_val(g_sclGpio, GPIO_DATA_LOW);
        udelay(GPIO_DELAY);
        count--;
    }

    gpio_i2c_send_ack(ack);//0 = ACK    1 = NACK
    *buffer = data;

    return;
}

unsigned int gpio_i2c_write(unsigned char* data, unsigned int len)
{
    unsigned char rc = 0;
    unsigned int i;
	
    gpio_i2c_start();

    rc |= gpio_i2c_send_byte(g_device_id << 1);
    if ((data == NULL) || (0 == len))
    {
        gpio_i2c_stop();
        return 0;
    }

    for (i = 0; i < len; i++)
    {
        rc |= gpio_i2c_send_byte(*data);
        data++;
    }

    gpio_i2c_stop();
    
    if (rc)
    {
        return -1;
    }

    return len;
}
EXPORT_SYMBOL(gpio_i2c_write);

unsigned int gpio_i2c_read( unsigned char* buffer, unsigned int len)
{
    unsigned char rc = 0;
    unsigned int i;


    gpio_i2c_start();
    rc |= gpio_i2c_send_byte((g_device_id << 1) | 0x01);
    if (rc)
    {
        gpio_i2c_stop();
        return -1;
    }

    for (i = 0; i < len; i++)
    {
        // 当是最后一个字节后发送NACK信号.
        gpio_i2c_read_byte(buffer++, !(len - i - 1));
    }

    gpio_i2c_stop();
     
    return len;

}
EXPORT_SYMBOL(gpio_i2c_read);

unsigned int gpio_i2c_init(VOID)
{
    unsigned int iRet = 0;

    sd5610x_iomux_set_gpio_mode(g_sdaGpio);
    sd5610x_iomux_set_gpio_mode(g_sclGpio);

    i2c_gpio_set_val(g_sdaGpio, GPIO_DATA_HIGH);
    i2c_gpio_set_dir(g_sdaGpio, GPIO_DIR_OUTPUT);
    i2c_gpio_set_val(g_sclGpio, GPIO_DATA_HIGH);
    i2c_gpio_set_dir(g_sclGpio, GPIO_DIR_OUTPUT);
    sd5610x_iomux_set_gpio_mode(g_rstGpio);
    i2c_gpio_set_dir(g_rstGpio, GPIO_DIR_OUTPUT);
    i2c_gpio_set_val(g_rstGpio, GPIO_DATA_LOW);
    msleep(10);
    i2c_gpio_set_val(g_rstGpio, GPIO_DATA_HIGH);
    msleep(50);
    
    return 0;
}
EXPORT_SYMBOL(gpio_i2c_init);

