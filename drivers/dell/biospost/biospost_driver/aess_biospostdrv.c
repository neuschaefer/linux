/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * BIOSPOST driver.
 *
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU
 * General Public License. This program is distributed in the hope
 * that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSBIOSPOSTDRV_C
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#include <asm/uaccess.h>
#include <asm/arch/hardware.h>
#include <asm/io.h>
#include <asm/arch/map.h>
#include <linux/spinlock.h>
#include <linux/kernel.h>
#include <asm/string.h>
#include "aess_biospostdrv.h"


//#define BIOSPOST_DEBUG 1
#define NEWDATA_OVERWRITE_OLDDATA_WHEN_OVERFLOW 1
#define POST_CODE_INTS_DISABLE_WHEN_TO_MANY_DUP 1
/* Debugging Message */
#ifdef BIOSPOST_DEBUG
#define DEBUG_MSG(fmt, args...) printk( "BIOSPOST: " fmt, ## args)
#else
#define DEBUG_MSG(fmt, args...)
#endif

/* driver name will passed by insmod command */
static char *driver_name="aess_biospostdrv";

/* Linux device data structure */
dev_t biospost_dev;
struct cdev *biospost_cdev;
static spinlock_t S_spinlock;
static UINT8 S_u8Open = 0;


static int aess_biospost_open(struct inode *inode, struct file *filp)
{
    /* Set the initial value for data structure */
    if(0==S_u8Open)
    {
	    spin_lock_init(&S_spinlock);
      S_u8Open = 1;
      memset(S_aBIOSPostData, 0, sizeof(sBIOSPostStruct)*IO_NUMBER);
    }
    return (STATUS_OK);
}

static int aess_biospost_close(struct inode* inode, struct file *flip)
{
  UINT8 u8Tmp = 0;
  UINT8 *pu8Data = (UINT8 *)flip->private_data;
  UINT8 i = 0;

  DEBUG_MSG("aess_BIOSPOST_close exit!\n");

  /*Check File ID*/
  if ( (&S_aBIOSPostData[FIFO_IOADDR0].u8FileID) != pu8Data  &&
    (&S_aBIOSPostData[FIFO_IOADDR1].u8FileID) != pu8Data)
  {
    /*illegal File ID*/
    return -EACCES;
  }

	u8Tmp = ioread8(BIOS_PS_REG_FIFO_ENABLE);

  if (NULL != pu8Data)
  {
    for(i = 0 ; i < IO_NUMBER; i++)
    {
     DEBUG_MSG("S_aBIOSPostData[%d].u8FileID is %x!\n", i, S_aBIOSPostData[i].u8FileID);

     /* disable IO ADDR */
     if(S_aBIOSPostData[i].u8FileID == pu8Data[0])
     {
       DEBUG_MSG("Disable IO Addr%d!\n", i);
       u8Tmp &= (~(FIFO_IOADDR_DISABLE >> i));
       S_aBIOSPostData[i].u8IOAddrInitFlag = IOADDRESS_NOINIT;

       if((&S_aBIOSPostData[i].u8FileID) != pu8Data)
       {
           /*AP uses the same file descriptor to access two I/O ports*
            *Reset u8FileID of an I/O port which was initialized last*
            *Reset u8FileID of the other I/O port which was first initialized *
            * when exiting for() loop*/
           S_aBIOSPostData[i].u8FileID = NO_FILE_ID;
       }
     }
    }

    /*Reset S_aBIOSPostData[].u8FileID, Then Set Private Data to NULL*/
    pu8Data[0] = NO_FILE_ID;
    pu8Data = NULL;
  }

  /* disable BISO Post Code interrupts*/
  if (IOADDRESS_NOINIT==S_aBIOSPostData[FIFO_IOADDR0].u8IOAddrInitFlag &&
    IOADDRESS_NOINIT==S_aBIOSPostData[FIFO_IOADDR1].u8IOAddrInitFlag)
  {
    DEBUG_MSG("Disable BIOS Post Code Interrupt!\n");
    u8Tmp &=FIFO_READY_INT_DISABLE;
    S_u8Open = 0;
  }
  iowrite8(u8Tmp, BIOS_PS_REG_FIFO_ENABLE);

  return 0;
}


static int aess_biospost_ioctl(struct inode * inode, struct file *flip,
						   unsigned int cmd, unsigned long arg)
{
	int err_check;
	sBIOSPostInfo *psInfo = (sBIOSPostInfo *) arg;

	/* Check the structure address is OK */
	if(!access_ok(VERIFY_WRITE, (void *) psInfo, sizeof(sBIOSPostInfo)))
	{
		printk(KERN_ERR "KN:struct access error!!\n");
		/* error occur */
		return -EFAULT;
	}

	switch(cmd)
	{
		case AESS_BIOSPOSTDRV_INIT:
			err_check = aess_biospost_drv_init(psInfo, flip);
			break;

		case AESS_BIOSPOSTDRV_READ:
			err_check = aess_biospost_read(psInfo, flip);
			break;
    case AESS_BIOSPOSTDRV_RESET:
      err_check = aess_biospost_reset(psInfo, flip);
      break;
		default:
			printk(KERN_ERR "KN:aess_BIOSPOST_ioctl, command error\n");
			err_check = -EINVAL;
	}

	/* 0->ok, minus->fail */
	return err_check;
}

static int aess_biospost_drv_init(sBIOSPostInfo *psInfo, struct file *flip)
{
  UINT8 u8Tmp = 0;
  UINT8 *pu8Data = (UINT8 *)flip->private_data;
  UINT8 u8IOIndex = psInfo->u8IOAddrEnFlag & 0x1;
  switch(u8IOIndex)
  {
    case FIFO_IOADDR0:
    case FIFO_IOADDR1:
        DEBUG_MSG("Enable IO Addr%x!\n",u8IOIndex);
        if(IOADDRESS_NOINIT== S_aBIOSPostData[u8IOIndex].u8IOAddrInitFlag)
        {
          S_aBIOSPostData[u8IOIndex].u8IOAddrInitFlag = IOADDRESS1_INIT;
          if(FIFO_IOADDR0==u8IOIndex)
          {
            S_aBIOSPostData[u8IOIndex].u8BIOSPostAddressLSB = 0x80;
            S_aBIOSPostData[u8IOIndex].u8BIOSPostAddressMSB = 0x00;
          }else
          {
          	/*Set BIOS POST Codes FIFO Address 2 LSB and MSB*/
            S_aBIOSPostData[u8IOIndex].u8BIOSPostAddressLSB = psInfo->u8BIOSPostAddressLSB;
            S_aBIOSPostData[u8IOIndex].u8BIOSPostAddressMSB = psInfo->u8BIOSPostAddressMSB;
            iowrite8(psInfo->u8BIOSPostAddressLSB,BIOS_PS_REG_FIFO_LADDR);
            iowrite8(psInfo->u8BIOSPostAddressMSB,BIOS_PS_REG_FIFO_MADDR);
          }

          if ( (&S_aBIOSPostData[FIFO_IOADDR0].u8FileID) != flip->private_data &&
              (&S_aBIOSPostData[FIFO_IOADDR1].u8FileID) != flip->private_data)
          {
            /*A new file ID*/
            S_aBIOSPostData[u8IOIndex].u8FileID = (FILE_ID1 << u8IOIndex);
            flip->private_data = &S_aBIOSPostData[u8IOIndex].u8FileID;
          }else
          {
            /*An application initialize two I/O address*/
            S_aBIOSPostData[u8IOIndex].u8FileID = pu8Data[0];
          }
          DEBUG_MSG("S_aBIOSPostData[0].u8FileID is %x!\n", S_aBIOSPostData[FIFO_IOADDR0].u8FileID);
        }else
        {
            /*Check File ID*/
            if ( (&S_aBIOSPostData[FIFO_IOADDR0].u8FileID) != flip->private_data &&
              (&S_aBIOSPostData[FIFO_IOADDR1].u8FileID) != flip->private_data)
            {
               /*The I/O address is initialized by other applicaton already.
                The current application has no permission to access this I/O
                address. The file ID is illeagal*/

               return -EACCES;
            }
        }
        break;
    default:
        /* IO Address invalid */
        return -EINVAL;
        break;
  }

  /*Enable FIFO Ready Interrupt and FIFO Capture of I/O address*/
  u8Tmp = ioread8(BIOS_PS_REG_FIFO_ENABLE)|FIFO_READY_INT_ENABLE;
  u8Tmp |= (FIFO_IOADDR_ENABLE >> psInfo->u8IOAddrEnFlag);
  iowrite8(u8Tmp, BIOS_PS_REG_FIFO_ENABLE);
	return STATUS_OK;
}

static int aess_biospost_read(sBIOSPostInfo *psInfo, struct file *flip)
{
  UINT32 u32Index = 0;
	UINT16 u16Len = 0;
	UINT16 i=0;
	UINT8 u8IOIndex = 0;

   /*Check File ID*/
   if ( (&S_aBIOSPostData[FIFO_IOADDR0].u8FileID) != flip->private_data &&
      (&S_aBIOSPostData[FIFO_IOADDR1].u8FileID) != flip->private_data)
   {
      /*illegal */
      printk("illegal File Descriptioin\n");
      return -EACCES;
   }
  u8IOIndex = psInfo->u8IOAddrEnFlag;

	switch(u8IOIndex)
	{
    case FIFO_IOADDR0:
    case FIFO_IOADDR1:

      /*Read data from KFIFO buffer*/
      if( S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size>0)
      {
        u16Len = min(S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size, psInfo->u16MaxReadLen);
        for(i=0; i< u16Len;i++)
        {
          /*If u16Out > BIOSPOST_KFIFO_SIZE, then u16Out -= BIOSPOST_KFIFO_SIZE*/
          S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out &= BIOSPOST_KFIFO_SIZE_MINUS_1;
          u32Index = S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out;

          /*Copy data from buffer*/
          psInfo->pu8Data[i] = S_aBIOSPostData[u8IOIndex].sIOAddrBuf.au8Buffer[u32Index];
          S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out++;
        }
        psInfo->u16CopyLen = u16Len;
        S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size -= u16Len;

      }else
      {
         psInfo->u16CopyLen = 0;
         if (0 == (ioread8(BIOS_PS_REG_FIFO_ENABLE) & FIFO_READY_INT_ENABLE))
         {
           // DF331275: iDRAC6 connectivity lost during RTC_Wake cycling
           // Return error status if interrupt is disabled so that app-level
           // can take action if necessary.
           return (STATUS_FAIL);
         }
      }
      break;
    default:
      return (STATUS_FAIL);
    break;
  }

	return (STATUS_OK);
}

static int aess_biospost_reset(sBIOSPostInfo *psInfo, struct file *flip)
{
  UINT8 u8IOIndex = 0;
  UINT8 u8Tmp = 0;

  /*Check File ID*/
  if ( (&S_aBIOSPostData[FIFO_IOADDR0].u8FileID) != flip->private_data &&
    (&S_aBIOSPostData[FIFO_IOADDR1].u8FileID) != flip->private_data)
  {
    /*illegal */
    return -EACCES;
  }

  u8IOIndex = psInfo->u8IOAddrEnFlag;
  switch(psInfo->u8IOAddrEnFlag)
  {
    case FIFO_IOADDR0:
    case FIFO_IOADDR1:
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In = 0;
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out = 0;
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size = 0;

      /*Enable FIFO Ready Interrupt and FIFO Capture of I/O address*/
      u8Tmp = ioread8(BIOS_PS_REG_FIFO_ENABLE)|FIFO_READY_INT_ENABLE;
      u8Tmp |= (FIFO_IOADDR_ENABLE >> psInfo->u8IOAddrEnFlag);
      iowrite8(u8Tmp, BIOS_PS_REG_FIFO_ENABLE);
      break;
    default:

      /* IO Address invalid */
      return -EINVAL;
      break;
  }
	return (STATUS_OK);
}

static int aess_biospost_clearall()
{
	/* Set the initial value for data structure */
	memset(S_aBIOSPostData, 0, sizeof(sBIOSPostStruct) * IO_NUMBER);

	/* Release IRQ */
	free_irq(KCS_HIB_INT, (void *) &S_aBIOSPostData);
	return (STATUS_OK);
}

#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
static irqreturn_t aess_biospost_isr (int irq, void *dev_id)
#else
static irqreturn_t aess_biospost_isr (int irq, void *dev_id, struct pt_regs *regs)
#endif
{
#if POST_CODE_INTS_DISABLE_WHEN_TO_MANY_DUP
    UINT8 u8DisableISR;
    UINT32 i;
#endif
    UINT8 u8Tmp = 0;
    UINT32 u32Flag = 0;
    UINT32 u32Index = 0;
    UINT8 u8IOIndex= 0;
    UINT8 u8Data = 0;
    static UINT8 S_u8ISRFlag = 0;
    DEBUG_MSG("BIOS Post Code ISR\n");

    S_u8ISRFlag = 0;

    spin_lock_irqsave(&S_spinlock, u32Flag);
		u8Tmp = ioread8(BIOS_PS_REG_FIFO_STATUS);
		while(FIFO_DATA_VALID&u8Tmp)
		{
      u8IOIndex = (u8Tmp&FIFO_ADDR_DECODE);

 	    /*Read data from FIFO to clear interrupt*/
  	  u8Data = ioread8(BIOS_PS_REG_FIFO_DATA);
  	  //printk("Data in BIOS H/W FIFO is %x\n",u8Data);

      #if NEWDATA_OVERWRITE_OLDDATA_WHEN_OVERFLOW
      if(S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size == BIOSPOST_KFIFO_SIZE)
      {
       /*New data overwrite old data.*/
       S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out &= BIOSPOST_KFIFO_SIZE_MINUS_1;
       S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Out++;
       S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size--;
      }
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In &= BIOSPOST_KFIFO_SIZE_MINUS_1;
      u32Index = S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In;
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.au8Buffer[u32Index]= u8Data;
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In++;
      S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size++;


      #else
      /*New data is skipped.*/
      if(S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size < BIOSPOST_KFIFO_SIZE)
      {
        S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In &= BIOSPOST_KFIFO_SIZE_MINUS_1;
        u32Index = S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In;
        S_aBIOSPostData[u8IOIndex].sIOAddrBuf.au8Buffer[u32Index]= u8Data;
        S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16In++;
        S_aBIOSPostData[u8IOIndex].sIOAddrBuf.u16Size++;

      }
      #endif

      // The following code block prevents iDRAC crashes when BIOS
      // goes into a spin loop sending a constant stream of error POST
      // codes, such as "CPU configuration error."
      #if POST_CODE_INTS_DISABLE_WHEN_TO_MANY_DUP
      if (u32Index > DUPLICATE_POST_CODE_COUNT)
      {
          u8DisableISR = POST_CODE_INTS_DISABLE;
          for (i=u32Index; i >= u32Index - DUPLICATE_POST_CODE_COUNT; i--)
          {
              if (u8Data != S_aBIOSPostData[u8IOIndex].sIOAddrBuf.au8Buffer[i-1])
              {
                  u8DisableISR = POST_CODE_INTS_NO_ACTION;
                  break;  // Last 10 POST Codes are not all equal
              }
          }

          if (u8DisableISR == POST_CODE_INTS_DISABLE)
          {
              DEBUG_MSG("Disabling POST CODE interrupts\n");
              u8Tmp = ioread8(BIOS_PS_REG_FIFO_ENABLE) & ~(FIFO_READY_INT_ENABLE);
              u8Tmp &= ~(FIFO_IOADDR_ENABLE >> u8IOIndex);
              iowrite8(u8Tmp, BIOS_PS_REG_FIFO_ENABLE);
          }
      }
      #endif

      if(u8Tmp&FIFO_OVERFLOW)
      {
          DEBUG_MSG("BIOS Post Codes FIFO Overflow!!!\n");
      }
      u8Tmp = ioread8(BIOS_PS_REG_FIFO_STATUS);
      S_u8ISRFlag =1;
 		}

    spin_unlock_irqrestore(&S_spinlock, u32Flag);

		if(1==S_u8ISRFlag)
		{
		  return IRQ_HANDLED;
	  }else
		{
		  return IRQ_NONE;
		}
}

struct file_operations aess_biospost_fops = {
	.open = aess_biospost_open,
	.ioctl = aess_biospost_ioctl,
	.release = aess_biospost_close,
};

int __init aess_biospost_init(void)
{
	int result;
	biospost_cdev = cdev_alloc();

	biospost_cdev->ops = &aess_biospost_fops;
	result = alloc_chrdev_region(&biospost_dev, 0, 1, driver_name);

	if (result < 0) {
		printk (KERN_ERR "KN:Registering the BIOSPOST device failed with %d\n", MAJOR(biospost_dev));
		return result;
	}

	cdev_add(biospost_cdev, biospost_dev, 1);

	/* Register IRQ*/
	if (request_irq(KCS_HIB_INT, aess_biospost_isr, IRQF_SHARED|IRQF_SAMPLE_RANDOM , "BIOSPOST",

		(void *) &S_aBIOSPostData))
	{
		return -EBUSY;
	}

	printk("mknod /dev/aess_biospostdrv c %d 0\n", MAJOR(biospost_dev));
	return (STATUS_OK);
}

void __exit aess_biospost_exit(void)
{
	aess_biospost_clearall();
	DEBUG_MSG("aess_BIOSPOST_exit\n");
	cdev_del(biospost_cdev);
	unregister_chrdev_region(biospost_dev, 1);
	return;
}

MODULE_DESCRIPTION("AESS BIOSPOST Driver");
MODULE_AUTHOR("Jason Hung <jason.hung@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_biospost_init);
module_exit(aess_biospost_exit);

