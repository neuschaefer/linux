/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Memory driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSMEMDRV_C

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>
#include <asm/memory.h>
#include <asm/arch/map.h>
#include "aess_memdrv.h"


/* Debugging Message */
#if 0
#define DEBUGP(fmt, args...) printk( "MEMDrv: " fmt, ## args)
#else
#define DEBUGP(fmt, args...)
#endif


/* driver name will passed by insmod command */
static char *driver_name="aess_memdrv"; 

/* Init flag */
static UINT8 S_u8init_flag = 0;

/* Used for protect write command */
struct rw_semaphore mem_drv_rw_sem;

/* Mutex to protect memory driver information structure */
static struct semaphore mem_drv_mutex;

/* linux device data structure */
dev_t mem_dev;
struct cdev *mem_cdev;

typedef struct 
{
    UINT32 u32Filp;         /* keep file structure */
	UINT32 u32VirtAddr;     /* virtual address */
	UINT16 u16MaxOffset;    /* max valid offset */
	UINT8 u8Status;         /* status */
} mem_info_type;

/* memory driver related information */
static mem_info_type mem_info[MEM_DRV_MAX_REGION_NUM];


static int aess_mem_open (struct inode *inode, struct file *filp)
{
    DEBUGP("aess_mem_open\n");
    
	if (S_u8init_flag == MEM_NOT_INIT)
	{
		printk(KERN_DEBUG "start init aess_mem_driver\n");
	    
    	/* clear memory driver structure */
    	memset(mem_info, 0, MEM_DRV_MAX_REGION_NUM * sizeof(mem_info_type));
	    
		init_rwsem(&mem_drv_rw_sem);
	    
		init_MUTEX(&mem_drv_mutex);
		
		S_u8init_flag = MEM_INIT_OK;
		
		printk(KERN_DEBUG "finish init aess_mem_driver\n");
	}
	
	return 0;
}


static int aess_mem_close(struct inode* inode, struct file *filp)
{
    int i;
    
	DEBUGP("aess_mem_close\n");
	
    /* get semaphore */
	if (down_interruptible(&mem_drv_mutex))
	{
	    return -ERESTARTSYS;
	}
	
	/* release resources that relate to this file */
    for (i = 0; i < MEM_DRV_MAX_REGION_NUM; i++)
    {
	    if (mem_info[i].u32Filp == (UINT32) filp)
	    {
	        DEBUGP("mem_info[%d] released\n", i);
	        
	        iounmap((void *) mem_info[i].u32VirtAddr);
	        
            mem_info[i].u32VirtAddr = 0;
            mem_info[i].u32Filp = 0;
            mem_info[i].u16MaxOffset = 0;
            mem_info[i].u8Status = 0;
	    }
    }
    
    /* release semaphore */
    up(&mem_drv_mutex);
	
	return 0;
}


static int aess_mem_read(sMemDrvInfoType *psMemDrvInfo)
{
	UINT16  u16i;
	
	UINT8  *u8ByteBuff;
	UINT16 *u16WordBuff;
	UINT32 *u32DWordBuff;
	
	void *pSrcAddr;
	
	DEBUGP("aess_mem_read\n");
	
	/* check the offset */
	if ((psMemDrvInfo->u16Offset 
	     + (psMemDrvInfo->u16DataSize << psMemDrvInfo->u8DataWidth)) 
	    > mem_info[psMemDrvInfo->u8ID].u16MaxOffset)
	{
	    return -EINVAL;
	}
	
	/* calculate the virtual address of source */
	pSrcAddr = (void *) (mem_info[psMemDrvInfo->u8ID].u32VirtAddr 
	                     + psMemDrvInfo->u16Offset);
	
	switch(psMemDrvInfo->u8DataWidth)
	{
		case MEM_DRV_BYTE:
		    
			u8ByteBuff = (UINT8 *) psMemDrvInfo->pDataPtr;
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
				*(u8ByteBuff + u16i) = ioread8(pSrcAddr + u16i);
			}
			
			break;
			
		case MEM_DRV_WORD:
		    
			u16WordBuff = (UINT16 *) psMemDrvInfo->pDataPtr;
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
				*(u16WordBuff + u16i) = ioread16(pSrcAddr + (u16i << 1));
			}
			break;
			
		case MEM_DRV_DWORD:
		    
			u32DWordBuff = (UINT32 *)psMemDrvInfo->pDataPtr;
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
				*(u32DWordBuff + u16i) = ioread32(pSrcAddr + (u16i << 2));
			}
			
			break;
			
		default:
			printk(KERN_ERR "Read():Data width error \n");
			return -EINVAL;
	}
	
	return 0;
}


static int aess_mem_write(sMemDrvInfoType *psMemDrvInfo)
{
	UINT16  u16i;
	
	UINT8  *u8ByteBuff;
	UINT16 *u16WordBuff;
	UINT32 *u32DWordBuff;
	
	void *pDestAddr;
	DEBUGP("aess_mem_write\n");
	
	/* check the offset */
	if ((psMemDrvInfo->u16Offset 
	     + (psMemDrvInfo->u16DataSize << psMemDrvInfo->u8DataWidth)) 
	    > mem_info[psMemDrvInfo->u8ID].u16MaxOffset)
	{
	    return -EINVAL;
	}
	
	/* calculate the virtual address of destination */
	pDestAddr = (void *) (mem_info[psMemDrvInfo->u8ID].u32VirtAddr 
	                      + psMemDrvInfo->u16Offset);
	
	switch(psMemDrvInfo->u8DataWidth)
	{
		case MEM_DRV_BYTE:
		    
			u8ByteBuff = (UINT8 *) psMemDrvInfo->pDataPtr;
			
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
				iowrite8(*(u8ByteBuff + u16i), pDestAddr + u16i);
			}
			
			break;
			
		case MEM_DRV_WORD:
		    
			u16WordBuff = (UINT16 *) psMemDrvInfo->pDataPtr;
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
				iowrite16(*(u16WordBuff + (u16i)), pDestAddr + (u16i << 1));
			}
			
			break;
			
		case MEM_DRV_DWORD:
		    
			u32DWordBuff = (UINT32 *) psMemDrvInfo->pDataPtr;
			
			for (u16i = 0; u16i < psMemDrvInfo->u16DataSize; u16i++)
			{
			    iowrite32(*(u32DWordBuff + (u16i)), pDestAddr + (u16i << 2));
			}
			
			break;
			
		default:
		    
			printk(KERN_ERR "Write():Data width error \n");
			
			return -EINVAL;
	}
	
	return 0;
}


static int aess_mem_request(struct file *filp, sMemDrvInfoType *psMemDrvInfo)
{
    int i;
    
    DEBUGP("aess_mem_request\n");
    
    /* get semaphore */
	if (down_interruptible(&mem_drv_mutex))
	{
	    return -ERESTARTSYS;
	}
    
    for (i = 0; i < MEM_DRV_MAX_REGION_NUM; i++)
    {
        if (mem_info[i].u8Status == 0)
        {
            mem_info[i].u32VirtAddr 
            = (UINT32) ioremap(psMemDrvInfo->u32BaseAddr, 
                               psMemDrvInfo->u16RegionSize);
            
            if (mem_info[i].u32VirtAddr == 0)
            {
                /* release semaphore */
                up(&mem_drv_mutex);
                
                return -ENOMEM;
            }
            
            psMemDrvInfo->u8ID = i;
            
            mem_info[i].u8Status = 1;
            mem_info[i].u16MaxOffset = psMemDrvInfo->u16RegionSize;
            mem_info[i].u32Filp = (UINT32) filp;
            
            DEBUGP("\n%d: u32BaseAddr=%lx, u16RegionSize=%x\n", 
                   i,
                   psMemDrvInfo->u32BaseAddr,
                   psMemDrvInfo->u16RegionSize);
            
            break;
        }
    }
    
    /* release semaphore */
    up(&mem_drv_mutex);
    
    return 0;
}


static int aess_mem_release(struct file *filp, sMemDrvInfoType *psMemDrvInfo)
{
    DEBUGP("aess_mem_release\n");
    
    /* check if index is valid */
    if (psMemDrvInfo->u8ID >= MEM_DRV_MAX_REGION_NUM)
    {
        return -EINVAL;
    }
    
    /* get semaphore */
	if (down_interruptible(&mem_drv_mutex))
	{
	    return -ERESTARTSYS;
	}
    
    if (mem_info[psMemDrvInfo->u8ID].u8Status == 1)
    {
        DEBUGP("mem_info[%d] released\n", psMemDrvInfo->u8ID);
        
        iounmap((void *) mem_info[psMemDrvInfo->u8ID].u32VirtAddr);
        
        mem_info[psMemDrvInfo->u8ID].u32VirtAddr = 0;
        mem_info[psMemDrvInfo->u8ID].u32Filp = 0;
        mem_info[psMemDrvInfo->u8ID].u16MaxOffset = 0;
        mem_info[psMemDrvInfo->u8ID].u8Status = 0;
    }
    else
    {
        /* release semaphore */
        up(&mem_drv_mutex);
        
        return -EINVAL;
    }
    
    /* release semaphore */
    up(&mem_drv_mutex);
    
    return 0;
}


static int aess_mem_ioctl(struct inode * inode, struct file *filp, 
						  unsigned int cmd, unsigned long arg)
{
	int err_check;
	sMemDrvInfoType *psMemDrvInfo = (sMemDrvInfoType *) arg;
    
	DEBUGP("aess_mem_ioctl\n");
	
	/* Check the structure address is OK */
	if(!access_ok(VERIFY_WRITE, (void *) psMemDrvInfo, sizeof(sMemDrvInfoType)))
	{
		printk("struct access error\n");
		/* error occur */
		return -EFAULT;
	}
	
	switch(cmd)
	{
#if 0
		case AESS_MEMDRV_RW:
		    
			if (psMemDrvInfo->u8Command == MEM_READ)
			{
				printk(KERN_DEBUG "Start Read process\n");
				
				/* Check the buffer address exist in the structure is OK */
				if(!access_ok(VERIFY_READ, psMemDrvInfo->pDataPtr,
							  sizeof(UINT8)))
				{
					printk("buffer access error\n");
					/* error occur */
					return -EFAULT;
				}
				
				down_read(&mem_drv_rw_sem);
				err_check = aess_mem_read(psMemDrvInfo);
				up_read(&mem_drv_rw_sem);
				
				printk(KERN_DEBUG "Finish Read process\n");
			}
			else if (psMemDrvInfo->u8Command == MEM_WRITE)
			{
				/* Use read/write semaphore to protect the write command */
				printk(KERN_DEBUG "Start write process\n");
				
				/* Check the buffer address exist in the structure is OK */
				if(!access_ok(VERIFY_WRITE, psMemDrvInfo->pDataPtr,
							  sizeof(UINT8)))
				{
					printk("buffer access error\n");
					/* error occur */
					return -EFAULT;
				}
				
				down_write(&mem_drv_rw_sem);
				err_check = aess_mem_write(psMemDrvInfo); 
				up_write(&mem_drv_rw_sem);
				
				printk(KERN_DEBUG "Finish write process\n");
			}
			else
			{
				printk(KERN_ERR "AESS_MEMDRV_RW, command error\n");
				err_check = -EINVAL;
			}
			
			break;
#endif
		    
		case AESS_MEMDRV_READ:
		    
			/* Check the buffer address exist in the structure is OK */
			if(!access_ok(VERIFY_READ, psMemDrvInfo->pDataPtr,
						  sizeof(UINT8)))
			{
				printk("buffer access error\n");
				/* error occur */
				return -EFAULT;
			}
		    
			down_read(&mem_drv_rw_sem);
			err_check = aess_mem_read(psMemDrvInfo);
			up_read(&mem_drv_rw_sem);
		    
		    break;
		    
		case AESS_MEMDRV_WRITE:
		    
			/* Check the buffer address exist in the structure is OK */
			if(!access_ok(VERIFY_WRITE, psMemDrvInfo->pDataPtr,
						  sizeof(UINT8)))
			{
				printk("buffer access error\n");
				/* error occur */
				return -EFAULT;
			}
		    
			down_write(&mem_drv_rw_sem);
			err_check = aess_mem_write(psMemDrvInfo); 
			up_write(&mem_drv_rw_sem);
		    
		    break;
		    
		case AESS_MEMDRV_REQUEST:
		    
		    err_check = aess_mem_request(filp, psMemDrvInfo);
		    
		    break;
		    
		case AESS_MEMDRV_RELEASE:
		    
		    err_check = aess_mem_release(filp, psMemDrvInfo);
		    
		    break;
			
		default:
		    
			printk(KERN_ERR "aess_mem_ioctl, command error\n");
			err_check = -EINVAL;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}


struct file_operations aess_mem_fops = {
	.open = aess_mem_open, 
	.ioctl = aess_mem_ioctl,
	.release = aess_mem_close, 
};


int __init aess_mem_init(void)
{
	int result;
    
    DEBUGP("aess_mem_init\n");
    
	mem_cdev = cdev_alloc();
	mem_cdev->ops = &aess_mem_fops;
	
	result = alloc_chrdev_region(&mem_dev, 0, 1, driver_name);
    
	if (result < 0) 
	{
		printk (KERN_ERR "Registering the character device failed with %d\n", 
		        MAJOR(mem_dev));
		
		return result;
	}
	
	cdev_add(mem_cdev, mem_dev, 1);
	
    /* show this message for memory driver install shell used */
    printk("mknod /dev/aess_memdrv c %d 0\n", MAJOR(mem_dev));
	
	return 0;
}


static void __exit aess_mem_exit(void)
{
    int i;
    
	DEBUGP("aess_memdrv_exit\n");
	
	cdev_del(mem_cdev);
	unregister_chrdev_region(mem_dev, 1);
	
    for (i = 0; i < MEM_DRV_MAX_REGION_NUM; i++)
    {
        if (mem_info[i].u8Status == 1)
        {
            iounmap((void *) mem_info[i].u32VirtAddr);
            
            mem_info[i].u32VirtAddr = 0;
            mem_info[i].u32Filp = 0;
            mem_info[i].u16MaxOffset = 0;
            mem_info[i].u8Status = 0;
        }
    }
	
	return;
}


MODULE_DESCRIPTION("AESS Memory Driver");
MODULE_AUTHOR("Justin Lee <justin.lee@avocent.com>");
MODULE_LICENSE("GPL");
module_init(aess_mem_init);
module_exit(aess_mem_exit);
module_param(driver_name, charp, S_IRUGO);
