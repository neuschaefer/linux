/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by Shiraz HAshim 					*/
/*   shiraz.hashim@st.com														*/
/*																				*/
/*   This program is free software; you can redistribute it and/or modify		*/
/*   it under the terms of the GNU General Public License as published by		*/
/*   the Free Software Foundation; either version 2 of the License, or			*/
/*   (at your option) any later version.										*/
/*																				*/
/*   This program is distributed in the hope that it will be useful,			*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 			*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				*/
/*   GNU General Public License for more details.								*/
/*																				*/
/*   You should have received a copy of the GNU General Public License			*/
/*   along with this program; if not, write to the								*/
/*   Free Software Foundation, Inc.,											*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.					*/
/********************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/arch/hardware.h>
#include <asm/arch/misc_regs.h>
#include <asm/io.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>
#include <asm/arch/spr_major.h>
#include "spr_arm2_enable.h"

#define DEVICE_NAME	"enbarm2"
#define ARM2                    0x00000004
#define SPEAR_ENBARM2_MAJOR MAJOR_ENBARM2

unsigned int threadx_text = THREADX_TEXT;
unsigned int mem 		  = MEM;

module_param (threadx_text, uint, S_IRUGO);
module_param (mem, uint, S_IRUGO);


volatile static int open_flag;
static char *writeptr;
static unsigned char *threadx_text_ptr;
static unsigned int threadx_mem_size;

static void Enable_ARM2_Core(void)
{
	volatile unsigned long val;

	/* Give reset */
	val = *((volatile unsigned long *)(PERIP1_SOF_RST));
	*((volatile unsigned long *)(PERIP1_SOF_RST)) = (val | ARM2);


	/* ARM2 CLOCK ENABLE */

	val = *((volatile unsigned long *)(PERIP1_CLK_ENB));
	*((volatile unsigned long *)(PERIP1_CLK_ENB)) = (val | ARM2);
	

	val = 0x80000007;
	*((volatile unsigned long *)(ICM1_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM2_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM3_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM4_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM5_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM6_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM7_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM8_ARB_CFG)) = val;
	*((volatile unsigned long *)(ICM9_ARB_CFG)) = val;



	/* Enable ARm2 */

	val = *((volatile unsigned long *)(PERIP1_SOF_RST));
	*((volatile unsigned long *)(PERIP1_SOF_RST)) = (val &  ~ARM2);


	/* return from this function */
	return;
}

static void Halt_ARM2_Core(void)
{

	volatile unsigned long val;

	/* Diable ARM2 clock */

	val = *((volatile unsigned long *)(PERIP1_CLK_ENB));
	*((volatile unsigned long *)(PERIP1_CLK_ENB)) = (val & ~ARM2);

	return;
}
	
static void Reboot_ARM2_Core(void)
{
	Halt_ARM2_Core();
	Enable_ARM2_Core();
}

static void Start_ARM2_Core(void)
{
	Enable_ARM2_Core();
}


static int SPEAR_ARM2_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long arg)
{
	int err=0;

	if (_IOC_TYPE(cmd) != SPEAR_EN_ARM2_IOC_BASE)
		return -ENOTTY;

	if (err) 
		return -EFAULT;

	switch(cmd)
	{
		case SPEAR_HALT_ARM2:
			Halt_ARM2_Core();
			break;
			
		case SPEAR_START_ARM2:
			Start_ARM2_Core();
			break;
			
		case SPEAR_REBOOT_ARM2:
			Reboot_ARM2_Core();
			break;

		default:
			printk("Invalid option \n");
			return -EFAULT;
	}

	return 0;
}

static int SPEAR_ARM2_Open(struct inode *inode, struct file *file)
{

	printk("\n Inside SPEAR_Arm2_Open\n");

	threadx_text_ptr = (unsigned char *)ioremap( threadx_text, threadx_mem_size);
	
	printk("\n threadx_text_ptr = %x\n",threadx_text_ptr);
		
	open_flag = 1;
	return 0;
}

static int SPEAR_ARM2_Release(struct inode *inode, struct file *file)
{
	iounmap( threadx_text_ptr);

	open_flag = 0;
	return 0;
}

static size_t SPEAR_ARM2_Write(struct file *file, const char __user *Buf, size_t len, loff_t *ppos) 
{
	int ret = -1;

	printk("Inside SPEAR_ARM2_Write \n");
	
	if(open_flag)
	{
		// only executed at first write operation

		if(len < THREADX_TEXT_OFFSET)
		{
			printk("len should be atleast:%d\n",THREADX_TEXT_OFFSET);
			return -EINVAL;
		}


		writeptr = (char *)(threadx_text_ptr);
		
		// first copy vectors
		ret = copy_from_user((char *)(THREADX_VEC), Buf, THREADX_VEC_SIZE);

		if(ret)
			printk("Not able to Write reset vector \n");

		//now start copying image
	
		if( len > THREADX_TEXT_OFFSET )
		{			
			ret = copy_from_user(writeptr, Buf + THREADX_TEXT_OFFSET, 
				(len - THREADX_TEXT_OFFSET)	 );

			if(ret)
				printk("Error in  Writing image \n");

			writeptr += (len - THREADX_TEXT_OFFSET);
		}
		open_flag=0;
	}
	else
	{
		/* copy remaining image */

		ret = copy_from_user(writeptr, Buf, len);
		if(ret)
			printk("Error in Writing image \n");
		writeptr += len;
	}

	if(!ret)
		return len;
	else
		return 0;

}

static struct file_operations SPEAR_ARM2_Fops =
{
	.owner	= THIS_MODULE,
	.ioctl	= SPEAR_ARM2_Ioctl,
	.open   = SPEAR_ARM2_Open, 
	.release= SPEAR_ARM2_Release,
	.write	= SPEAR_ARM2_Write
};

static struct miscdevice SPEAR_ARM2_dev =
{
	.minor  = MISC_DYNAMIC_MINOR,
	.name   = DEVICE_NAME,
	.fops   = &SPEAR_ARM2_Fops,
};


static int __init SPEAR_ARM2_Init(void)
{
	int ret;

	ret = register_chrdev(SPEAR_ENBARM2_MAJOR,DEVICE_NAME,&SPEAR_ARM2_Fops);

	if ( ret < 0)
	{
		printk(KERN_ERR DEVICE_NAME":unable to register character device\n");
	}

	threadx_mem_size = (mem * 1024 * 1024) - threadx_text;

	printk("Threadx text base=0x%x and size=0x%x\n", threadx_text, threadx_mem_size);

	return 0;

}
static void __exit SPEAR_ARM2_Exit(void)
{
	misc_deregister(&SPEAR_ARM2_dev);
	return;
}

module_init(SPEAR_ARM2_Init);
module_exit(SPEAR_ARM2_Exit);

MODULE_AUTHOR("ST Microelectronic");
MODULE_DESCRIPTION("SPEAr Code to reset ARM2");
MODULE_LICENSE("GPL");

