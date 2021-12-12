/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 SSPI driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSSSPIDRV_C
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <asm/arch/map.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include "aess_sspidrv.h"
#include "../../gpio/gpio_driver/aess_gpiodrv.h" 


/* enable to print out message */
#if 0
#define DEBUGP(format, args...) printk("SSPIDrv: " format, ## args)
#else
#define DEBUGP(format, args...)
#endif


/* since virtual memory should be in steps of 0x1000
   and SSPI Phy address is 0xB8000400, not in the boundary of 0x1000 
   so, refer UART's virtual address then add the offset 0x400 
   UART's phy address is 0xB8000000 */
#define SSPI_BASE_REG            (WPCM450_VA_UART + 0x400)
#define SSPI_DATA_REG            (SSPI_BASE_REG + 0x0) //8bits
#define SSPI_CTL1_REG           (SSPI_BASE_REG + 0x2) //16bits
#define SSPI_STAT_REG           (SSPI_BASE_REG + 0x4) //8bits

#define SSPI_GCR_BASE_REG            WPCM450_VA_GCR
#define SSPI_GCR_PDID_REG          (SSPI_GCR_BASE_REG+0x0)
#define SSPI_GCR_MFSEL1_REG          (SSPI_GCR_BASE_REG+0x0C)

/* since virtual memory should be in steps of 0x1000
   and Clock control register Phy address is 0xB8000200, not in the boundary of 0x1000 
   so, refer UART's virtual address then add the offset 0x200 
   UART's phy address is 0xB8000000 */
#define CLK_BASE_REG            (WPCM450_VA_UART + 0x200)
#define CLK_ENABLE_REG            (CLK_BASE_REG + 0x0) 
#define CLK_IPSRST_REG            (CLK_BASE_REG + 0x20) 

#define SSPI_GPIO_PORT_BASE_ADDR      WPCM450_VA_GPIO
#define SSPI_GPIO_REG_PORT0_DATAOUT_ADDR          (SSPI_GPIO_PORT_BASE_ADDR+0x1C)
#define SSPI_GPIO_REG_PORT1_DATAOUT_ADDR          (SSPI_GPIO_PORT_BASE_ADDR+0x34)


/* definitions for control and status register */
#define SSPI_ENABLE            0x01
#define SSPI_EIR               0x20
#define SSPI_EIW               0x40
#define SSPI_SCM               0x80
#define SSPI_SCIDL             0x100
#define SSPI_SCDV6_0           0x3F
#define SSPI_STAT_MASK					0x3
#define SSPI_BSY               0x01
#define SSPI_RBF               0x02


/* definitions for GCR Multi Function Chip Select register */
#define GCR_SSPISEL_ENABLE       0x80000000

#define SSPI_DRV_MAX_RETRY    5
#define SSPI_TRANSACTION_DONE 0x0
#define SSPI_RECEIVED_DONE 0x0

/* Others*/
#define SHIFT_9_BITS   9
#define SHIFT_7_BITS   7
#define NUMBER_1       1
#define NUMBER_3       3
#define NUMBER_4       4
#define NUMBER_5       5
#define NUMBER_7       7
#define NUMBER_8       8
#define NUMBER_F       0xF

/*SSPI Interrupt, shart with ADC*/
#define SSPI_HIB_INT 31

/* SSPI chip select signals by GPIOE15 */
#define SSPI_CHIP_SELECT_PIN 15

/* SSPI CMD/Data signals by GPIOL21 */
#define SSPI_CMD_DATA_SELECT_PIN 21

/* driver name will passed by insmod command */
static char *driver_name="aess_sspidrv";

/* record the kernel spi initial state */
static int S_sspi_init_state = 0;
#define Z2_VERSION_NUM       0x03926450


dev_t sspi_dev;
struct cdev *sspi_cdev;

struct semaphore sspi_sem;

#define SSPI_DRV_MAX_MSG_SIZE             512

/******************************************************************************
*   STRUCT      :   sSPIDrvInfoType
******************************************************************************/
/**
 *  @brief   structure of SPI driver information
 *
 *****************************************************************************/
typedef struct
{
	
	/** Wait queue */
	wait_queue_head_t wq;

	/** Msg transmit buffer */
	UINT8 pu8MsgSendBuffer[SSPI_DRV_MAX_MSG_SIZE];
	
	/** Msg transmit data length */
	UINT32 u32MsgSendDataSize;
	
	/** Msg receive buffer */
	UINT8 pu8MsgRecBuffer[SSPI_DRV_MAX_MSG_SIZE];
	
	/** Msg receive data length */
	UINT32 u32MsgRecDataSize;
 	
	/** Current start tracking count */
	UINT16 u16CurIndex;

	/** Condition checking for wait queue */
	UINT8 wq_flag;

	/** write or read process, 0:write 1:read */
	UINT8 wr_flag;

} sSSPIDrvBufType;

sSSPIDrvBufType sspi_buffer;

static irqreturn_t aess_sspi_isr (int irq, void *dev_id, struct pt_regs *regs)
{
	UINT8 u8Status = 0;
	UINT8 u8Reading;
	
	u8Status = ioread8(SSPI_STAT_REG)&SSPI_STAT_MASK;
	
	DEBUGP("KN: aess_sspi_isr  u8Status = 0x%x \n", u8Status);
	switch(sspi_buffer.wr_flag)
	{
		case 0: //write process
			if (u8Status & SSPI_RBF)
			{
				//read data register for clear interrupt
		  	/* read a dummy data */
				u8Reading = ioread8(SSPI_DATA_REG);
				DEBUGP("KN: read data = 0x%x \n", u8Reading);
			}

			if ((u8Status & SSPI_BSY) == 0)
			{
				if (sspi_buffer.u16CurIndex < sspi_buffer.u32MsgSendDataSize)
				{
			  	/* write to SSPI */
					DEBUGP("KN: write data%d = 0x%x \n", sspi_buffer.u16CurIndex, sspi_buffer.pu8MsgSendBuffer[sspi_buffer.u16CurIndex]);
					iowrite8((UINT8) sspi_buffer.pu8MsgSendBuffer[sspi_buffer.u16CurIndex], 
							  (void *) SSPI_DATA_REG);
					sspi_buffer.u16CurIndex++;
				}
				else
				{
					/* nothing to do in write process */
					if (sspi_buffer.u16CurIndex == 0){
						DEBUGP("KN: do nothing in write processing \n");
						return IRQ_NONE;
					}
						
					/* init buf index */
					sspi_buffer.u16CurIndex = 0;
					
					/* change to read process */
					if (sspi_buffer.u32MsgRecDataSize > 0)
					{
						/* change to read process */
						sspi_buffer.wr_flag = 1; 
					}
					else
					{
						/* complete write read command, and disable SSPI */
						iowrite16(0, (void *) SSPI_CTL1_REG);
						DEBUGP("KN: disable sspi \n");

						/* set wq_flag to wake up thread */
						sspi_buffer.wq_flag = 1;
						/* wake up */
						wake_up_interruptible(&sspi_buffer.wq);
						/* since interrupt will coming after disable read/write interrupt,
					   	 so, change wr_flag to 2 and don't do anyting when come back this isr call back function*/
						sspi_buffer.wr_flag = 2; 
						break;
					}
				}
			}
			break;
		case 1: //read process
			if (u8Status & SSPI_RBF)
			{
				if (sspi_buffer.u16CurIndex < sspi_buffer.u32MsgRecDataSize)
				{
					/* read data from the register */
					u8Reading = ioread8(SSPI_DATA_REG);
					DEBUGP("KN: read data%d 0x%x \n", sspi_buffer.u16CurIndex, u8Reading);
					sspi_buffer.pu8MsgRecBuffer[sspi_buffer.u16CurIndex] = u8Reading;
					sspi_buffer.u16CurIndex++;
				}
				if (sspi_buffer.u16CurIndex == sspi_buffer.u32MsgRecDataSize)
				{
					/* nothing to do in read process */
					if (sspi_buffer.u16CurIndex == 0){
						DEBUGP("KN: do nothing in read processing \n");
						return IRQ_NONE;
					}

					/* complete write read command, and disable SSPI */
					iowrite16(0, (void *) SSPI_CTL1_REG);
					DEBUGP("KN: disable sspi \n");

					/* set wq_flag to wake up thread */
					sspi_buffer.wq_flag = 1;
					/* wake up */
					wake_up_interruptible(&sspi_buffer.wq);
					
					/* since interrupt will coming after disable read/write interrupt,
					   so, change wr_flag to 2 and don't do anyting when come back this isr call back function*/
					sspi_buffer.wr_flag = 2; 
					break;
				}
			}
			if ((u8Status & SSPI_BSY) == 0)
			{
				//write data register for clear interrupt
				/* write dummy data to the register */
				DEBUGP("KN: write data 0x0 \n");
				iowrite8((UINT8) 0x0, (void *) SSPI_DATA_REG);
			}
			break;
	}
	
	return IRQ_HANDLED;
}


static int aess_sspi_wr(sSSPIDrvInfoType *psSSPIDrvInfo)
{
	unsigned char u8TmpBuf;
	int     i32Result = 0;
	int i;
	unsigned char clock_divider;
	
	
	DEBUGP("KN: aess_sspi_wr\n");
	
	/* check the arguments */
	if (psSSPIDrvInfo->u8Mode >= SSPI_DRV_MAX_MODE)
	{
		/* operation not permitted */
		return -EPERM;
	}
	
	DEBUGP("KN: u8ProcessTime=%d u8Mode=%d u8ChipSelect=%d u8Speed=%d\n", 
		   psSSPIDrvInfo->u8ProcessTime, psSSPIDrvInfo->u8Mode,
		   psSSPIDrvInfo->u8ChipSelect, psSSPIDrvInfo->u8Speed);
	
	/* initiate local data struture */
	
	/* initiate wait queue */
	init_waitqueue_head(&sspi_buffer.wq);
	/* initiate buffer index */
	sspi_buffer.u16CurIndex = 0;
	/* write process firest */
	sspi_buffer.wr_flag = 0;
	/* clear the flag of condition checking */
	sspi_buffer.wq_flag = 0;
	
	if (copy_from_user ((void *) &sspi_buffer.pu8MsgSendBuffer[0],
						(void *) psSSPIDrvInfo->pu8MsgSendBuffer,
						psSSPIDrvInfo->u32MsgSendDataSize))
	{
		/* release the chip select */
		DEBUGP("KN: release the chip select \n");
		u8TmpBuf = SET_GPIO_OUTPUT_HIGH;
		aess_gpio_commander(SSPI_CHIP_SELECT_PIN, GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
		
		/* disable SSPI interface */
		DEBUGP("KN: disable SSPI interface \n");
		iowrite16(0, (void *) SSPI_CTL1_REG);
		
		return -EFAULT;
	}
	
	sspi_buffer.u32MsgSendDataSize = psSSPIDrvInfo->u32MsgSendDataSize;
	sspi_buffer.u32MsgRecDataSize = psSSPIDrvInfo->u32MsgRecDataSize;
	
	/* convert clock divider */
	clock_divider = ((psSSPIDrvInfo->u8Speed/2) - 1) & SSPI_SCDV6_0;

	/* set the speed and chip select */
	u8TmpBuf = SET_GPIO_OUTPUT_LOW;
	aess_gpio_commander(SSPI_CHIP_SELECT_PIN, GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
	DEBUGP("KN: GPIO port 0 data = 0x%x \n", ioread32(SSPI_GPIO_REG_PORT0_DATAOUT_ADDR));
	
	
	/* set Command/Data select */
	if (0 == psSSPIDrvInfo->u8ChipSelect) {
		u8TmpBuf = SET_GPIO_OUTPUT_LOW; 
	} else { 
		u8TmpBuf = SET_GPIO_OUTPUT_HIGH; 
	}
	aess_gpio_commander(SSPI_CMD_DATA_SELECT_PIN, GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
	DEBUGP("KN: GPIO port 1 data = 0x%x \n", ioread32(SSPI_GPIO_REG_PORT1_DATAOUT_ADDR));
	
	/* enable SSPI interface */
	DEBUGP("KN: Enable SSPI \n");
	iowrite16((UINT16) (SSPI_ENABLE | SSPI_EIR | SSPI_EIW | (clock_divider << SHIFT_9_BITS) | (psSSPIDrvInfo->u8Mode << SHIFT_7_BITS)), 
			  (void *) SSPI_CTL1_REG);
	DEBUGP("KN: SSPI_CTL1_REG = 0x%x \n", ioread16(SSPI_CTL1_REG));
	
	
	/* push task go to sleep and wait for SSPI transaction completed */
	DEBUGP("KN: wait_event_interruptible_timeout \n");
	if (0 == wait_event_interruptible_timeout(sspi_buffer.wq, 
											  sspi_buffer.wq_flag == 1, 
											  psSSPIDrvInfo->u8ProcessTime))
	{
		DEBUGP("KN: timeout for write read processing \n");
		sspi_buffer.wr_flag = 2;
		i32Result =  -EBUSY;
	}
	else
	{
		if ((psSSPIDrvInfo->u32MsgRecDataSize > 0) && (sspi_buffer.wq_flag == 1))
		{
			DEBUGP("KN: copy data to user space, len = %d\n", psSSPIDrvInfo->u32MsgRecDataSize);
			for (i = 0; i < psSSPIDrvInfo->u32MsgRecDataSize; i++){
				psSSPIDrvInfo->pu8MsgRecBuffer[i] = sspi_buffer.pu8MsgRecBuffer[i];
			}
		}
	}

	/* release the chip select */
	DEBUGP("KN: release the chip select \n");
	u8TmpBuf = SET_GPIO_OUTPUT_HIGH;
	aess_gpio_commander(SSPI_CHIP_SELECT_PIN, GPIO_WRITE, NORMAL_OUTPUT, (void *)&u8TmpBuf);
	
	/* disable SSPI interface */
	DEBUGP("KN: disable SSPI interface \n");
	iowrite16(0, (void *) SSPI_CTL1_REG);
	
	/* clear the flag of condition checking */
	sspi_buffer.wq_flag = 0;

	return i32Result;
}

int aess_sspi_io(sSSPIDrvInfoType *psSSPIDrvInfo)
{
	int result;
	
	DEBUGP("aess_sspi_io\n");
	
	/* if sspi interface is not initiated */
	if (!S_sspi_init_state)
	{
		DEBUGP("init sspi\n");
		
		/* initiate semaphores */
		init_MUTEX(&sspi_sem);
		
		/* sspi interface is initiated */
		S_sspi_init_state = 1;
	}
	
	/* get semaphore */
	if(down_interruptible(&sspi_sem))
	{
		DEBUGP("bus busy!\n");
		
		/* device busy */
		return -EBUSY;
	}
	
	/* invoke the handling function */
	result = aess_sspi_wr(psSSPIDrvInfo);
	
	/* release semaphore */
	up(&sspi_sem);
	
	return result;
}
EXPORT_SYMBOL_GPL(aess_sspi_io);


static int aess_sspi_ioctl(struct inode * inode, struct file *flip, 
						  unsigned int cmd, unsigned long arg)
{
	int result;
	sSSPIDrvInfoType *psSSPIDrvInfo;
	
	DEBUGP("KN: aess_spi_ioctl\n");
	
	switch (cmd)
	{
		case AESS_SSPIDRV_WR:
			
			DEBUGP("KN: AESS_SPIDRV_WR\n");
			
			/* */
			psSSPIDrvInfo = (sSSPIDrvInfoType *) arg;
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psSSPIDrvInfo, 
						  sizeof(sSSPIDrvInfoType)))
			{
				DEBUGP("KN: struct access error!\n");
				
				/* bad address */
				return -EFAULT;
			}
			
			/* get semaphore */
			if(down_interruptible(&sspi_sem))
			{
				DEBUGP("KN: bus busy!\n");
				
				/* device busy */
				return -EBUSY;
			}
			
			/* invoke the handling function */
			result = aess_sspi_wr(psSSPIDrvInfo);
			
			/* release semaphore */
			up(&sspi_sem);
			
			break;
			
		default:
			
			DEBUGP("KN: default\n");
			
			result = -ENOIOCTLCMD;
			
			break;
	}
	
	return result;
}


static int aess_sspi_release(struct inode *inode, struct file *filp)
{
	DEBUGP("aess_sspi_release\n");
	return 0;
}


static int aess_sspi_open(struct inode *inode, struct file *filp)
{
  UINT32 u32Data = 0;
	DEBUGP("aess_sspi_open\n");
	
	/* if spi interface is not initiated */
	if (!S_sspi_init_state)
	{
		/* initiate semaphores */
		init_MUTEX(&sspi_sem);
		
		/* spi interface is initiated */
		S_sspi_init_state = 1;
		  
		/* set Pin function from GPIO to nSPICS */
    DEBUGP("Set pin function\n");
    
    /*Select SSPI option. In Z2*/
    u32Data = (ioread32(SSPI_GCR_MFSEL1_REG)|GCR_SSPISEL_ENABLE);
    iowrite32(u32Data, SSPI_GCR_MFSEL1_REG);
    DEBUGP("After setting CGR_MFSEL1_REG %x\n",ioread32(SSPI_GCR_MFSEL1_REG));    

    DEBUGP("SSPI_GCR_PDID_REG %x\n",ioread32(SSPI_GCR_PDID_REG));    
    DEBUGP("CLK_ENABLE_REG %x\n",ioread32(CLK_ENABLE_REG));    
    DEBUGP("CLK_IPSRST_REG %x\n",ioread32(CLK_IPSRST_REG));   
	}
	
	return 0;
}


struct file_operations aess_sspi_fops = {
	.owner = THIS_MODULE,
	.open = aess_sspi_open,
	.release = aess_sspi_release,
	.ioctl = aess_sspi_ioctl,
};


int __init aess_sspi_init(void)
{
	int result;
	
	DEBUGP("init aess_sspi_module\n");
	
	if (ioread32(SSPI_GCR_PDID_REG) < Z2_VERSION_NUM)
	{
		DEBUGP("This version 0x%x don't support SSPI\n", ioread32(SSPI_GCR_PDID_REG));
		return 0;
	}
	
	/* allocate a char device */
	sspi_cdev = cdev_alloc();
	sspi_cdev->owner = THIS_MODULE;
	sspi_cdev->ops = &aess_sspi_fops;
	
	/* allocate a device number */
	result = alloc_chrdev_region(&sspi_dev, 0, 1, driver_name);
	
	DEBUGP("Major number %d\n", MAJOR(sspi_dev));
	
	if (result)
	{
		printk(KERN_ERR "KN: Registering the character device failed with %d\n", 
			   MAJOR(sspi_dev));
		
		return result;
	}
	
	/* add a char device */
	cdev_add(sspi_cdev, sspi_dev, 1);
	printk("mknod /dev/aess_sspidrv c %d 0\n", MAJOR(sspi_dev));

	/* disable SSPI before request irq */
	iowrite16(0, (void *) SSPI_CTL1_REG);
	if (request_irq(SSPI_HIB_INT, (irq_handler_t)aess_sspi_isr, IRQF_SHARED|IRQF_SAMPLE_RANDOM, "SSPI", &S_sspi_init_state)) {
		return -EBUSY;
	}

	return 0;
}


void __exit aess_sspi_exit(void)
{
	DEBUGP("exit aess_sspi_module\n");
	
	/* release the char device */
	cdev_del(sspi_cdev);
	
	/* release the device number */
	unregister_chrdev_region(sspi_dev, 1);
	free_irq(SSPI_HIB_INT, (void *) NULL);
	
	return;
}

MODULE_DESCRIPTION("AESS SSPI Driver");
MODULE_AUTHOR("Justin Lee <justin.lee@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_sspi_init);
module_exit(aess_sspi_exit);
