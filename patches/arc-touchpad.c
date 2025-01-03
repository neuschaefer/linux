#include <linux/module.h>
#include <linux/init.h>
#include <linux/miscdevice.h> /* for struct miscdevice*/
#include <linux/interrupt.h>
#include <linux/fs.h> /* for iormap */
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/i2c.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/irqflags.h>
#include <linux/spinlock_types.h>
#include <linux/input.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h> 

#define I2C_BUS_ID 1
#define TOUCH_I2C_ADDR 0x0a
#define TOUCH_ADDR 0x01
#define TOUCH_NAME "arc_touch_pad"
#define IRQ_NAME "arc_touch"
#define IRQ_VALUE 116
#define MODULE_NAME "touch_stop"
#define DEV_BUFSIZE 1024
#define BUTTON_STATUS 8

static struct i2c_client *reg_client;
static struct i2c_adapter *adap;
static struct input_dev *button_dev;

static int touch_stop_major;
static int touch_stop_minor;
static dev_t touch_Dev;
static struct cdev *touch_cdev = NULL; 
static struct class *c1;
static int touch_stop_enable=0;  

ssize_t arc_touch_stop_read(struct file*, char __user*, size_t, loff_t*);
ssize_t arc_touch_stop(struct file*, const char __user *, size_t, loff_t*);

static struct i2c_board_info arc_touch_i2c_info =
{
	I2C_BOARD_INFO(TOUCH_NAME, TOUCH_I2C_ADDR),
};

static void reg_i2c_device(void)
{
	adap = i2c_get_adapter(I2C_BUS_ID);
	reg_client = i2c_new_device(adap, &arc_touch_i2c_info);
}

static void unreg_i2c_device(void)
{
	i2c_put_adapter(adap);
	i2c_unregister_device(reg_client);
}

static void touch_i2c_get(struct i2c_client *client, int get_address ,char* result)
{
	char get_buf[1]; //first byte is address to write. others are bytes to be written
	get_buf[0]=get_address;
    i2c_master_send(client, get_buf, 1); //reposition file pointer to register 0x29
	i2c_master_recv(client, result, 1); //read register 0x29 and save to rbuf
}

static void button_config(int check_value, int* button)
{
	if(check_value != 0)
	{
		if(check_value/128)
		{
			button[7] = 1;
			check_value = check_value % 128;
		}
		if(check_value/64)
		{
			button[6] = 1;
			check_value = check_value % 64;
		}
		if(check_value/32)
		{
			button[5] = 1;
			check_value = check_value % 32;
		}
		if(check_value/16)
		{
			button[4] = 1;
			check_value = check_value % 16;
		}
		if(check_value/8)
		{
			button[3] = 1;
			check_value = check_value % 8;
		}
		if(check_value/4)
		{
			button[2] = 1;
			check_value = check_value % 4;
		}
		if(check_value/2)
		{
			button[1] = 1;
			check_value = check_value % 2;
		}
		if(check_value)
		{
			button[0] = 1;
		}
		
	}
}

irqreturn_t arc_touch_irqhandler(int irq, void *dev_id)
{
	reg_i2c_device();
	return IRQ_WAKE_THREAD;
}

irqreturn_t arc_touch_threadhandler(int irq, void *dev_id)
{
	char r_buf[10]={0};
	int check_value=0;
	int button_status[BUTTON_STATUS]={0};
	int i;
	
	touch_i2c_get(reg_client, TOUCH_ADDR, r_buf);
	unreg_i2c_device();
	printk(KERN_WARNING"touch status %x\n",(int)r_buf[0]);
	check_value = (int)r_buf[0];
	button_config(check_value,button_status);
	
	if(button_status[0])
		input_report_key(button_dev, KEY_VOLUMEUP, 1);
	if(button_status[1])
		input_report_key(button_dev, KEY_MODE, 1);
	if(button_status[2])
		input_report_key(button_dev, KEY_VOLUMEDOWN, 1);
	if(button_status[3])
		input_report_key(button_dev, KEY_MUTE, 1);
	if(button_status[4])
		input_report_key(button_dev, KEY_VOLUMEUP, 0);
	if(button_status[5])
		input_report_key(button_dev, KEY_MODE, 0);
	if(button_status[6])
		input_report_key(button_dev, KEY_VOLUMEDOWN, 0);
	if(button_status[7])
		input_report_key(button_dev, KEY_MUTE, 0);
	
    input_sync(button_dev);

	return IRQ_HANDLED;
}

int arc_touch_stop_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char data[20] = {0};
    ssize_t ret = 0;

    printk("%s():\n", __FUNCTION__);

    if (*f_pos >= sizeof(data)) {
        goto out;
    }

    if (count > sizeof(data)) {
        count = sizeof(data);
    }
	
	if(touch_stop_enable == 1)
	{
		strcpy(data,"Touch stop\n");
	}
	else
	{
		strcpy(data,"Touch run\n");
	}

    if (copy_to_user(buf, data, count) < 0) {
        ret = -EFAULT;
        goto out;
    }
    *f_pos += count;
    ret = count;

out:
    return ret;
}

int arc_touch_stop_write(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	char *data;
	int data_value;
    ssize_t ret = 0;

    printk("%s():\n", __FUNCTION__);

    data = kzalloc(sizeof(char) * DEV_BUFSIZE, GFP_KERNEL);
    if (!data) {
        return -ENOMEM;
    }

    if (count > DEV_BUFSIZE) {
        count = DEV_BUFSIZE;
    }

    if (copy_from_user(data, buf, count) < 0) {
        ret = -EFAULT;
        goto out;
    }
    
	kstrtol(data,10,&data_value);
	printk("%s(): %d\n", __FUNCTION__, data_value);
	if(data_value == 1 && touch_stop_enable != 1)
	{
		touch_stop_enable = 1;
		disable_irq(IRQ_VALUE);
		printk("The touch pad stop running, you can upgrade the MCU.");
	}
	else if(data_value == 0 && touch_stop_enable != 0)
	{
		touch_stop_enable = 0;
		enable_irq(IRQ_VALUE);
		printk("The touch pad is running normally.");
	}
	else
	{
		ret = -EFAULT;
        goto out;
	}
    *f_pos += count;
    ret = count;
out:
    kfree(data);
    return ret;
}

int arc_touch_stop_release(struct inode *inode, struct file *filp)
{
	printk("arc_touch_stop_release\n");
	return 0;
}

int arc_touch_stop_open(struct inode *inode, struct file *filp)
{
	printk("arc_touch_stop_open\n");
	return 0;
}

int arc_touch_open(struct inode *node, struct file *filp)
{
	printk(KERN_WARNING"open ...!\n");
	
	return 0;
}

struct file_operations arc_touch_fops = 
{
	.open = arc_touch_open,
};

struct file_operations touch_stop_fops =
{
	.read = arc_touch_stop_read,
	.write = arc_touch_stop_write,
	.open = arc_touch_stop_open,
	.release = arc_touch_stop_release,
};

struct miscdevice arc_touch_miscdev = //定義一個misdevice結構
{
	.minor = 200,
	.name = IRQ_NAME,
	.fops = &arc_touch_fops,//這裡arc_touch_fops是一個struct file_operations結構
};



static int arc_touch_init(void)
{
	int err;
	
	misc_register(&arc_touch_miscdev);//註冊一個混雜裝置驅動裝置
	
	//由高電平產生中斷 IRQF_TRIGGER_HIGH
	
	if( (err = request_threaded_irq(IRQ_VALUE, arc_touch_irqhandler, arc_touch_threadhandler,  IRQF_TRIGGER_RISING | IRQF_ONESHOT | IRQF_NO_SUSPEND, IRQ_NAME, NULL)) < 0 )//註冊中斷處理程式 5個引數
	{
		 printk(KERN_WARNING"err = %d\n", err);
		 goto irq_err;
	}
	button_dev = input_allocate_device();
	if (!button_dev) {
			printk(KERN_ERR "button.c: Not enough memory\n");
			err = -ENOMEM;
			goto irq_err;
	}

	__set_bit(EV_KEY, button_dev->evbit);
	__set_bit(KEY_VOLUMEUP, button_dev->keybit);
	__set_bit(KEY_MODE, button_dev->keybit);
	__set_bit(KEY_VOLUMEDOWN, button_dev->keybit);
	__set_bit(KEY_MUTE, button_dev->keybit);
	button_dev->name = TOUCH_NAME;
	
	err = alloc_chrdev_region(&touch_Dev, 0, 1, MODULE_NAME);
	if (err) {
        printk("can't alloc chrdev\n");
        return err;
    }
	
	touch_stop_major = MAJOR(touch_Dev);
    touch_stop_minor = MINOR(touch_Dev);
	printk("register chrdev(%d,%d)\n", touch_stop_major, touch_stop_minor);
	
	touch_cdev= kzalloc(sizeof(struct cdev), GFP_KERNEL);
	cdev_init(touch_cdev,&touch_stop_fops);
	err = cdev_add(touch_cdev, MKDEV(touch_stop_major, touch_stop_minor),1);
	if (err < 0) {
        printk("add chr dev failed\n");
        goto failed;
    }
	c1 = class_create(THIS_MODULE, "touch");
    device_create(c1, NULL, touch_Dev, NULL, MODULE_NAME);

	//device_create(c1, NULL, touch_Dev, NULL, MODULE_NAME);
 
	err = input_register_device(button_dev);
	if (err) {
			printk(KERN_ERR "button.c: Failed to register device\n");
			goto err_free_dev;
	}

	return err;

err_free_dev:
        input_free_device(button_dev);
		
irq_err:
		misc_deregister(&arc_touch_miscdev);
failed:
    if (touch_cdev) {
        kfree(touch_cdev);
        touch_cdev = NULL;
    }		
	return -1;
}

static void arc_touch_exit(void)
{
	free_irq(IRQ_VALUE, NULL);//登出中斷 這裡irqnumber引數暫時用一個變數來表示（中斷號）
	input_unregister_device(button_dev);
	misc_deregister(&arc_touch_miscdev);//登出一個混雜裝置驅動
	unregister_chrdev_region(MKDEV(201, 5),1);
	
	printk(KERN_WARNING"key up!");	
}


module_init(arc_touch_init);
module_exit(arc_touch_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("key driver");