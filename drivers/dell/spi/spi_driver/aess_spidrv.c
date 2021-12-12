/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * VSC 452 SPI driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSSPIDRV_C
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <asm/arch/map.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include "aess_spidrv.h"


/* enable to print out message */
#if 0
#define DEBUGP(format, args...) printk("SPIDrv: " format, ## args)
#else
#define DEBUGP(format, args...)
#endif


#define SPI_BASE_REG            (WPCM450_VA_FIU)
#define SPI_CODE_REG            (SPI_BASE_REG + 0x16)
#define SPI_ADDR0_REG           (SPI_BASE_REG + 0x17)
#define SPI_ADDR1_REG           (SPI_BASE_REG + 0x18)
#define SPI_ADDR2_REG           (SPI_BASE_REG + 0x19)
#define SPI_DATA0_REG           (SPI_BASE_REG + 0x1A)
#define SPI_DATA1_REG           (SPI_BASE_REG + 0x1B)
#define SPI_DATA2_REG           (SPI_BASE_REG + 0x1C)
#define SPI_DATA3_REG           (SPI_BASE_REG + 0x1D)
#define SPI_CTRL_STS_REG        (SPI_BASE_REG + 0x1E)
#define SPI_EXT_CTRL_STS_REG    (SPI_BASE_REG + 0x1F)

#define GCR_BASE_REG            WPCM450_VA_GCR
#define GCR_MFSEL1_REG          (GCR_BASE_REG+0x0C)

/* definitions for control and status register */
#define SPI_EXEC_DONE                0x80
#define SPI_EXEC                     0x80
#define SPI_DONE                     0x00
#define SPI_WR                       0x10
#define SPI_ADDR_3BYTE               0x08
#define SPI_DATA_1BYTE               0x01
#define SPI_DATA_2BYTE               0x02
#define SPI_DATA_4BYTE               0x04


#define DEV_NUM                   0x60
#define SPI_DEV_NUM_CLEAR         0x9F
/* definitions for GCR Multi Function Chip Select register */
#define GCR_SCSSEL_ENABLE12       0x18
#define GCR_SCSSEL_ENABLE3        0x3FFFFFDF

#define SPI_DRV_MAX_RETRY    0x20
#define SPI_TRANSACTION_DONE 0x0

/* Others*/
#define SHIFT_5_BITS   5
#define NUMBER_1       1
#define NUMBER_3       3
#define NUMBER_4       4
#define NUMBER_5       5
#define NUMBER_7       7
#define NUMBER_8       8
#define NUMBER_F       0xF



/* driver name will passed by insmod command */
static char *driver_name="aess_spidrv";

/* record the kernel spi initial state */
static int S_spi_init_state = 0;

dev_t spi_dev;
struct cdev *spi_cdev;

struct semaphore spi_sem;

static int aess_spi_wait_transaction_done(void)
{
  UINT8 u8WaitCount = 0;
  UINT8 u8Status = 0;
  
  while(1)
  {
    u8Status = ioread8(SPI_CTRL_STS_REG)&SPI_EXEC_DONE;
    if(SPI_DONE==u8Status)
    {
      break;
    }
    udelay(1);
    u8WaitCount++;
    DEBUGP("wiat for SPI transaction done\n");
    
    if( SPI_DRV_MAX_RETRY < u8WaitCount)
    {
      printk("Wait too long for SPI transaction!!!\n");
      return -EBUSY;;
    }  
  }
  return SPI_TRANSACTION_DONE;
}


static int aess_spi_write(sSPIDrvInfoType *psSPIDrvInfo)
{
	UINT32 u32SendIndex = 0;
	UINT32 u32RemainingLen = 0;
	UINT8  u8Status = 0;
	UINT8   i = 0;
	UINT8   u8Len = 0;
	int     i32Result = 0;
  
  DEBUGP("aess_spi_write()\n");

  /*Wait for SPI transaction done*/
  i32Result = aess_spi_wait_transaction_done();
  if(SPI_TRANSACTION_DONE != i32Result)
  {
    return i32Result;
  }

  if (NUMBER_8 <psSPIDrvInfo->u32MsgSendDataSize)
  {
    /*Page write and activate the nSPICS signal*/
    u8Status = (UINT8)((~(NUMBER_1 << psSPIDrvInfo->u8ChipSelect))&NUMBER_F);
    
    /*Bits 7~4 are reserved bits, don't set 1 to these bits. Otherwise, page write fails*/
    iowrite8(u8Status, SPI_EXT_CTRL_STS_REG);
    DEBUGP("ECTS is %x\n",ioread8(SPI_EXT_CTRL_STS_REG));
  }
  
  u32RemainingLen = psSPIDrvInfo->u32MsgSendDataSize;
  u32SendIndex = 0;

  while(0 < u32RemainingLen)
  {
    if ( NUMBER_7 < u32RemainingLen)
    {
	    /* write code register */
      iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_CODE_REG));
     
      for (i=0;i<NUMBER_3;i++)
      {
		    /*ADDR2 must be send first*/
		    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_ADDR2_REG-i));
		  }
		  for (i=0;i<NUMBER_4;i++)
		  {
		    /*DATA0 must be send first*/
		    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_DATA0_REG+i));
		  }  
		  u32RemainingLen -= 8;
		  
		  /*Set WR, A_SIZE, D_SIZE bits*/
		  u8Status = (ioread8(SPI_CTRL_STS_REG) & DEV_NUM)|
		           SPI_WR | SPI_ADDR_3BYTE |SPI_DATA_4BYTE;
		  
		}else if((0 < u32RemainingLen)&&(u32RemainingLen < NUMBER_8))
		{	 
      switch (u32RemainingLen)
      {
        case 1:
   		    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_CODE_REG));
          
          /*Set WR bits*/
		      u8Status = (ioread8(SPI_CTRL_STS_REG) & DEV_NUM)|SPI_WR;
          break;
        case 2:
          iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_CODE_REG));
   		    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_DATA0_REG));
   		    
   		    /*Set WR, D_SIZE bits*/
   		    u8Status = ((ioread8(SPI_CTRL_STS_REG) & DEV_NUM)|
		           SPI_WR | SPI_DATA_1BYTE);
          break;
        case 3:
          iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_CODE_REG));
   		    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_DATA0_REG));
          iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_DATA1_REG));

   		    /*Set WR, D_SIZE bits*/
   		    u8Status = ((ioread8(SPI_CTRL_STS_REG) & DEV_NUM)| SPI_WR | SPI_DATA_2BYTE);
          break;
        case 4:
        case 5:
        case 6:
        case 7:

          iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_CODE_REG));
      
          for (i = 0 ; i < NUMBER_3 ; i++)
          {
		        /*ADDR2 must be send first*/
		        iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_ADDR2_REG-i));
		      }
		      u8Len = u32RemainingLen - NUMBER_4;
		      for (i=0;i<u8Len;i++)
		      {
		        /*DATA0 must be send first*/
		        iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32SendIndex++],(SPI_DATA0_REG+i));
		      }  

   		    /*Set WR, A_SIZE, D_SIZE bits*/
		      u8Status = (ioread8(SPI_CTRL_STS_REG)&DEV_NUM)|SPI_WR 
		               | SPI_ADDR_3BYTE | u8Len;
          break;
        default:
          printk("\n\nInvalid length\n\n");
          if (  NUMBER_8 < psSPIDrvInfo->u32MsgSendDataSize)
          {
            /*Release the nSPICS signal*/
            iowrite8(NUMBER_F, SPI_EXT_CTRL_STS_REG);  
          }
          return -EIO;
      }
   		u32RemainingLen = 0;
    }

    /*Start Transaction*/
    u8Status |= SPI_EXEC;
    DEBUGP("W=>u8Status is %x\n",u8Status);
    iowrite8(u8Status,SPI_CTRL_STS_REG);
    
    /*Wait for SPI transaction done*/
    i32Result = aess_spi_wait_transaction_done();
    if(SPI_TRANSACTION_DONE != i32Result)
    {
      if (NUMBER_8 <psSPIDrvInfo->u32MsgSendDataSize)
      {
        /*Release the nSPICS signal*/
        iowrite8(NUMBER_F, SPI_EXT_CTRL_STS_REG);  
      }
      return i32Result;
    }
  }
	
  if ( NUMBER_8 <psSPIDrvInfo->u32MsgSendDataSize)
  {
    /*Release the nSPICS signal*/
    iowrite8(NUMBER_F, SPI_EXT_CTRL_STS_REG);  
  }
  return 0; 			  
}

static int aess_spi_read(sSPIDrvInfoType *psSPIDrvInfo)
{
  UINT32 u32Index = 0;
	UINT8  u8Status = 0;
	int    i32Result = 0;
	
  DEBUGP("aess_spi_read()\n");

  /*Check whether SPI transaction is done*/

  i32Result = aess_spi_wait_transaction_done();
  if(SPI_TRANSACTION_DONE != i32Result)
  {
    return i32Result;
  }

  /*Only keep DEV_NUM bits, other bits are cleared*/
  u8Status = ioread8(SPI_CTRL_STS_REG)&DEV_NUM; 
  
  /*Set Code & Addr register*/
  u32Index = 0;
  iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32Index++],(SPI_CODE_REG));

  if(NUMBER_4 == psSPIDrvInfo->u32MsgSendDataSize)
  {
    /*ADDR2 send first*/
    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32Index++],(SPI_ADDR2_REG));
    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32Index++],(SPI_ADDR1_REG));
    iowrite8(psSPIDrvInfo->pu8MsgSendBuffer[u32Index++],(SPI_ADDR0_REG));

    /*Set A_Size bit*/
    u8Status |= SPI_ADDR_3BYTE;    
  }
  
  /*Set Data size and start SPI transaction*/
  u8Status |= (SPI_EXEC|psSPIDrvInfo->u32MsgRecDataSize);
  DEBUGP("u8Status is 0x%x\n",u8Status);
  iowrite8(u8Status,SPI_CTRL_STS_REG);
      
  /*Wait for SPI transaction done*/
  i32Result = aess_spi_wait_transaction_done();
  if(SPI_TRANSACTION_DONE != i32Result)
  {
    return i32Result;
  }

 	/* read from SPI */
	for (u32Index = 0; u32Index < psSPIDrvInfo->u32MsgRecDataSize; u32Index++)
	{
    psSPIDrvInfo->pu8MsgRecBuffer[u32Index]  = ioread8((SPI_DATA0_REG+u32Index));
  }
	return 0;
}
static int aess_spi_wr(sSPIDrvInfoType *psSPIDrvInfo)
{

	UINT8 u8Status = 0;
	int result = 0;
		
	DEBUGP("aess_spi_wr\n");
	
	/* check the arguments */
	if ((psSPIDrvInfo->u8Number >= SPI_DRV_MAX_BUS)
		|| (psSPIDrvInfo->u8Mode >= SPI_DRV_MAX_MODE)
		|| (psSPIDrvInfo->u8ChipSelect >= SPI_DRV_MAX_CS))
	{
		/* operation not permitted */
		DEBUGP("Invalid arguments\n");
		DEBUGP("Bus number is %x\n",psSPIDrvInfo->u8Number);
		DEBUGP("SPI mode is %x\n",psSPIDrvInfo->u8Mode);
		DEBUGP("Chip select is %x\n",psSPIDrvInfo->u8ChipSelect);
		return -EPERM;
	}

	/* set Device Number */
	DEBUGP("Chip Selection is %x\n",psSPIDrvInfo->u8ChipSelect);
	u8Status = ioread8(SPI_CTRL_STS_REG)&SPI_DEV_NUM_CLEAR;
	u8Status |= ((UINT8)(psSPIDrvInfo->u8ChipSelect << SHIFT_5_BITS));
  iowrite8(u8Status, SPI_CTRL_STS_REG);
  
 	DEBUGP("SPI_CTRL_STS_REG %x\n",ioread8(SPI_CTRL_STS_REG));

	
	/*Check Write or Read operation*/
	if((0 < psSPIDrvInfo->u32MsgSendDataSize) && (0 == psSPIDrvInfo->u32MsgRecDataSize))
	{
     DEBUGP("Call aess_spi_write()\n");

     /*Write operation*/
    result = aess_spi_write(psSPIDrvInfo);
  }else if ( ((NUMBER_1 == psSPIDrvInfo->u32MsgSendDataSize)|(NUMBER_4 == psSPIDrvInfo->u32MsgSendDataSize)) && 
      ((0< psSPIDrvInfo->u32MsgRecDataSize)&&(NUMBER_5 > psSPIDrvInfo->u32MsgRecDataSize)))
  {
    DEBUGP("Call aess_spi_read()\n");
  	/*Read operation*/
    result = aess_spi_read(psSPIDrvInfo);
 	}else
 	{
     DEBUGP("Invalid data size\n");
		/* Invalid Arguments */
		return -EINVAL;
  }	
	
  return result;
}


int aess_spi_io(sSPIDrvInfoType *psSPIDrvInfo)
{
	int result;
	
	DEBUGP("aess_spi_io\n");
	
	/* if spi interface is not initiated */
	if (!S_spi_init_state)
	{
		DEBUGP("init spi\n");
		
		/* initiate semaphores */
		init_MUTEX(&spi_sem);
		
		/* spi interface is initiated */
		S_spi_init_state = 1;
	}
	
	/* get semaphore */
	if(down_interruptible(&spi_sem))
	{
		DEBUGP("bus busy!\n");
		
		/* device busy */
		return -EBUSY;
	}
	
	/* invoke the handling function */
	result = aess_spi_wr(psSPIDrvInfo);
	
	/* release semaphore */
	up(&spi_sem);
	
	return result;
}
EXPORT_SYMBOL_GPL(aess_spi_io);


static int aess_spi_ioctl(struct inode * inode, struct file *flip, 
						  unsigned int cmd, unsigned long arg)
{
	int result;
	sSPIDrvInfoType *psSPIDrvInfo;
	
	DEBUGP("aess_spi_ioctl\n");
	
	switch (cmd)
	{
		case AESS_SPIDRV_WR:
			
			DEBUGP("AESS_SPIDRV_WR\n");
			
			psSPIDrvInfo = (sSPIDrvInfoType *) arg;
			
			
			/* Check the structure address is OK */
			if(!access_ok(VERIFY_WRITE, (void *) psSPIDrvInfo, 
						  sizeof(sSPIDrvInfoType)))
			{
				DEBUGP("struct access error!\n");
				
				/* bad address */
				return -EFAULT;
			}
			
			/* get semaphore */
			if(down_interruptible(&spi_sem))
			{
				DEBUGP("bus busy!\n");
				
				/* device busy */
				return -EBUSY;
			}
			
			/* invoke the handling function */
			result = aess_spi_wr(psSPIDrvInfo);
			
			/* release semaphore */
			up(&spi_sem);
			
			break;
			
		default:
			
			DEBUGP("default\n");
			
			result = -ENOIOCTLCMD;
			
			break;
	}
	
	return result;
}


static int aess_spi_release(struct inode *inode, struct file *filp)
{
	DEBUGP("aess_spi_release\n");
	
	return 0;
}


static int aess_spi_open(struct inode *inode, struct file *filp)
{
  UINT32 u32Data = 0;
	DEBUGP("aess_spi_open\n");
	
	/* if spi interface is not initiated */
	if (!S_spi_init_state)
	{
		/* initiate semaphores */
		init_MUTEX(&spi_sem);
		
		/* spi interface is initiated */
		S_spi_init_state = 1;
		  
		/* set Pin function from GPIO to nSPICS */
    DEBUGP("Set pin function\n");
    
    /*Clear all nSPICS selection*/
    u32Data = (ioread32(GCR_MFSEL1_REG)|GCR_SCSSEL_ENABLE12)&GCR_SCSSEL_ENABLE3;
    DEBUGP("Before setting CGR_MFSEL1_REG %x\n",ioread32(GCR_MFSEL1_REG));    
    iowrite32(u32Data, GCR_MFSEL1_REG);
    DEBUGP("After setting CGR_MFSEL1_REG %x\n",ioread32(GCR_MFSEL1_REG));    
	}
	
	return 0;
}


struct file_operations aess_spi_fops = {
	.owner = THIS_MODULE,
	.open = aess_spi_open,
	.release = aess_spi_release,
	.ioctl = aess_spi_ioctl,
};


int __init aess_spi_init(void)
{
	int result;
	
	DEBUGP("init aess_spi_module\n");
	
	/* allocate a char device */
	spi_cdev = cdev_alloc();
	spi_cdev->owner = THIS_MODULE;
	spi_cdev->ops = &aess_spi_fops;
	
	/* allocate a device number */
	result = alloc_chrdev_region(&spi_dev, 0, 1, driver_name);
	
	DEBUGP("Major number %d\n", MAJOR(spi_dev));
	
	if (result)
	{
		printk(KERN_ERR "KN: Registering the character device failed with %d\n", 
			   MAJOR(spi_dev));
		
		return result;
	}
	
	/* add a char device */
	cdev_add(spi_cdev, spi_dev, 1);
	printk("mknod /dev/aess_spidrv c %d 0\n", MAJOR(spi_dev));
	return 0;
}


void __exit aess_spi_exit(void)
{
	DEBUGP("exit aess_spi_module\n");
	
	/* release the char device */
	cdev_del(spi_cdev);
	
	/* release the device number */
	unregister_chrdev_region(spi_dev, 1);
	
	return;
}

MODULE_DESCRIPTION("AESS SPI Driver");
MODULE_AUTHOR("Justin Lee <justin.lee@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_spi_init);
module_exit(aess_spi_exit);
