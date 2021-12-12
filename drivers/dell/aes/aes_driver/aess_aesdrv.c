/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 Kernel AES driver.
 *  
 * Copyright (C) 2006 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */

#define AESSAESDRV_C

#include <linux/errno.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/semaphore.h>
#include <asm/arch/map.h>
#include "aess_aesdrv.h"


/* Debugging Message */
#if 0
/* #define AES_DEBUG */
#define DEBUGP(fmt, args...) printk("AESDrv: " fmt, ## args)
#else
#define DEBUGP(fmt, args...)
#endif

/* enable a background timer to poll statuses of all initiated timer buses */
#if 0
#define AES_POLLING_STATUS
#define AES_POLLING_INTERVAL 500   /* 1 = 1 jiffies = 10 ms */
struct aes_list aes_polling;
#endif

#if 0
#define AES_MEASURE_BY_GPIO
#endif


#define WPCM450_AESREG(x) (WPCM450_VA_AES + (x))

#define WPCM450_AESKEYI     WPCM450_AESREG(0x000)
#define WPCM450_AESKEYO     WPCM450_AESREG(0x004)
#define WPCM450_AESDATI     WPCM450_AESREG(0x008)
#define WPCM450_AESDATO     WPCM450_AESREG(0x00C)
#define WPCM450_AESCTL      WPCM450_AESREG(0x010)

#define AES_RESET   0x08
#define AES_AVL     0x04
#define AES_RDY     0x02
#define AES_EXP     0x01


/* Linux device data structure */ 
dev_t aes_dev;
struct cdev *aesdrv_cdev; 

/* semaphore */
struct semaphore aes_sem;

/* driver name will passed by insmod command */
static char *driver_name="aess_aesdrv";

/* Init flag */
static UINT8 S_u8AESInitFlag = INIT_FAIL;


#ifdef AES_MEASURE_BY_GPIO
void aess_aes_set_gpio(int flag)
{
    static int gpio_init = 0;
    static int gpio_status[] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    if (!gpio_init)
    {
        /* GPIOE0~7 output low */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) = 0x00;
        
        /* GPIOE0~7 as push-pull type */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x18)) = 0x00;
        
        /* set GPIOE0~7 as output */
        *((UINT32 *) (WPCM450_VA_GPIO + 0x14)) = 0xFF;
        
        gpio_init = 1;
    }
    
    if (!gpio_status[flag])
    {
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) 
        = *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) | (1 << flag);
        
        gpio_status[flag] = 1;
    }
    else
    {
        *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) 
        = *((UINT32 *) (WPCM450_VA_GPIO + 0x1C)) & ~(1 << flag);
        
        gpio_status[flag] = 0;
    }
    
    return;
}
#endif


#ifdef AES_POLLING_STATUS
void aess_aes_polling_func(unsigned long data)
{
	/* DEBUGP("KN: polling...\n"); */
    
	/* timer interval is AES_POLLING_INTERVAL */
	aes_polling.expires = jiffies + AES_POLLING_INTERVAL;
	add_timer(&aes_polling);
	
	return;
}
#endif


static int aess_aesdrv_decrypt(sAESInfoType *psAESInfo)
{
    int i;
    UINT8 *pu8PlainText;
    UINT8 *pu8InputBlock;
    UINT8 *pu8OutputBlock;
    UINT8 *pu8CipherText;
    UINT8 *pu8InitVector;
    UINT8 *pu8InitKey;
    /* UINT8 u8Buffer[16]; */
    
    DEBUGP("aess_aesdrv_decrypt\n");
    
#ifdef AES_MEASURE_BY_GPIO
    aess_aes_set_gpio(1);
#endif
    
    pu8InitKey = psAESInfo->pu8InitKey;
    pu8InitVector = psAESInfo->pu8InitVector;
    pu8PlainText = psAESInfo->pu8Input;
    pu8CipherText = psAESInfo->pu8Output;
    
    /* check the key size setting */
    if (psAESInfo->u8KeySize != AES_DRV_128_BITS)
    {
        DEBUGP("key size is not support\n");
        
        /* Invalid argument */
        return -EINVAL;
    }
    
    /* operate before ECB engine */
    switch(psAESInfo->u8Mode)
    {
        /* case AES_DRV_MODE_ECB: */
        /* case AES_DRV_MODE_CBC: */
        /* case AES_DRV_MODE_CFB: */
        /* case AES_DRV_MODE_OFB: */
        /* case AES_DRV_MODE_CTR: */ 
            
        default:
            
            DEBUGP("mode is not support\n");
            
            /* Invalid argument */
            return -EINVAL;
    }
    
	/* get semaphore */
	if(down_interruptible(&aes_sem))
	{
		return -ERESTARTSYS;
	}
    
    /* enter ECB engine */
    
    /* check AES engine ready bit and AES expect data bit */
    if (ioread32(WPCM450_AESCTL) & (AES_RDY | AES_EXP))
    {
        for (i = 0; i < 16; i += 4)
        {
            /* feed key */
            iowrite32(*((UINT32 *) (pu8InitKey + i)), WPCM450_AESKEYI);
            
            /* feed plain data */
            iowrite32(*((UINT32 *) (pu8InputBlock + i)), WPCM450_AESDATI);
        }
        
        while (1)
        {
            /* wait until process is finished */
            if(ioread32(WPCM450_AESCTL) & AES_AVL)
            {
                break;
            }
        }
        
        /* retrieve cipher data from register */
        for (i = 0; i < 16; i += 4)
        {
            /* retrieve cipher data */
            *((UINT32 *) (pu8OutputBlock + i)) = ioread32(WPCM450_AESDATO);
        }
    }
    else
    {
        /* reset AES module */
        iowrite32(AES_RESET, WPCM450_AESCTL);
        
        DEBUGP("ECB engine is not ready\n");
    }
    
    /* exit ECB engine */
    
	/* release semaphore */
	up(&aes_sem);
    
    /* operate after ECB engine */
    switch(psAESInfo->u8Mode)
    {
        case AES_DRV_MODE_ECB:
            
            break;
            
        /* case AES_DRV_MODE_CBC: */
        /* case AES_DRV_MODE_CFB: */
        /* case AES_DRV_MODE_OFB: */
        /* case AES_DRV_MODE_CTR: */ 
            
    }
    
#ifdef AES_MEASURE_BY_GPIO
    aess_aes_set_gpio(1);
#endif
    
#ifdef AES_DEBUG
    for (i = 0; i < 16; i++)
    {
        DEBUGP("PT[%2d]=%3x IB[%2d]=%3x OB%2d]=%3x CT[%2d]=%3x IV[%2d]=%3x" \
               "IK[%2d]=%3x\n", 
               i, pu8PlainText[i], 
               i, pu8InputBlock[i],
               i, pu8OutputBlock[i],
               i, pu8CipherText[i],
               i, pu8InitVector[i], 
               i, pu8InitKey[i]);
    }
#endif
    
    return 0;
}


static int aess_aesdrv_encrypt(sAESInfoType *psAESInfo)
{
    int i;
    UINT8 *pu8PlainText;
    UINT8 *pu8InputBlock;
    UINT8 *pu8OutputBlock;
    UINT8 *pu8CipherText;
    UINT8 *pu8InitVector;
    UINT8 *pu8InitKey;
    UINT8 u8Buffer[16];
    
    DEBUGP("aess_aesdrv_encrypt\n");
    
#ifdef AES_MEASURE_BY_GPIO
    aess_aes_set_gpio(0);
#endif
    
    pu8InitKey = psAESInfo->pu8InitKey;
    pu8InitVector = psAESInfo->pu8InitVector;
    pu8PlainText = psAESInfo->pu8Input;
    pu8CipherText = psAESInfo->pu8Output;
    
    /* check the key size setting */
    if (psAESInfo->u8KeySize != AES_DRV_128_BITS)
    {
        DEBUGP("key size is not support\n");
        
        /* Invalid argument */
        return -EINVAL;
    }
    
    /* operate before ECB engine */
    switch(psAESInfo->u8Mode)
    {
        case AES_DRV_MODE_CBC:
            
            pu8InputBlock = &u8Buffer[0];
            pu8OutputBlock = pu8CipherText;
            
            /* plain text xor initial vector */
            for (i = 0; i < 16; i += 4)
            {
                *((UINT32 *) (pu8InputBlock + i)) 
                = *((UINT32 *) (pu8PlainText + i)) 
                  ^ *((UINT32 *) (pu8InitVector + i));
            }
            
            break;
            
        case AES_DRV_MODE_ECB:
            
            pu8InputBlock = pu8PlainText;
            pu8OutputBlock = pu8CipherText;
            
            break;
            
        /* case AES_DRV_MODE_CFB: */
        /* case AES_DRV_MODE_OFB: */
        /* case AES_DRV_MODE_CTR: */ 
            
        default:
            
            DEBUGP("mode is not support\n");
            
            /* Invalid argument */
            return -EINVAL;
    }
    
	/* get semaphore */
	if(down_interruptible(&aes_sem))
	{
		return -ERESTARTSYS;
	}
    
    /* enter ECB engine */
    
    /* check AES engine ready bit and AES expect data bit */
    if (ioread32(WPCM450_AESCTL) & (AES_RDY | AES_EXP))
    {
        for (i = 0; i < 16; i += 4)
        {
            /* feed key */
            iowrite32(*((UINT32 *) (pu8InitKey + i)), WPCM450_AESKEYI);
            
            /* feed plain data */
            iowrite32(*((UINT32 *) (pu8InputBlock + i)), WPCM450_AESDATI);
        }
        
        while (1)
        {
            /* wait until process is finished */
            if(ioread32(WPCM450_AESCTL) & AES_AVL)
            {
                break;
            }
        }
        
        /* retrieve cipher data from register */
        for (i = 0; i < 16; i += 4)
        {
            /* retrieve cipher data */
            *((UINT32 *) (pu8OutputBlock + i)) = ioread32(WPCM450_AESDATO);
        }
    }
    else
    {
        /* reset AES module */
        iowrite32(AES_RESET, WPCM450_AESCTL);
        
        DEBUGP("ECB engine is not ready\n");
    }
    
    /* exit ECB engine */
    
	/* release semaphore */
	up(&aes_sem);
    
    /* operate after ECB engine */
    switch(psAESInfo->u8Mode)
    {
        case AES_DRV_MODE_CBC:
            
            /* copy output data to init vector */
            for (i = 0; i < 16; i += 4)
            {
                *((UINT32 *) (pu8InitVector + i)) 
                = *((UINT32 *) (pu8OutputBlock + i));
            }
            
            break;
            
        case AES_DRV_MODE_ECB:
            
            break;
            
        /* case AES_DRV_MODE_CFB: */
        /* case AES_DRV_MODE_OFB: */
        /* case AES_DRV_MODE_CTR: */ 
            
    }
    
#ifdef AES_MEASURE_BY_GPIO
    aess_aes_set_gpio(0);
#endif
    
#ifdef AES_DEBUG
    for (i = 0; i < 16; i++)
    {
        DEBUGP("PT[%2d]=%3x IB[%2d]=%3x OB%2d]=%3x CT[%2d]=%3x IV[%2d]=%3x" \
               "IK[%2d]=%3x\n", 
               i, pu8PlainText[i], 
               i, pu8InputBlock[i],
               i, pu8OutputBlock[i],
               i, pu8CipherText[i],
               i, pu8InitVector[i], 
               i, pu8InitKey[i]);
    }
#endif
    
    return 0;
}   


static int aess_aesdrv_ioctl(struct inode * inode, struct file *flip, 
							   unsigned int cmd, unsigned long arg)
{   
	int err_check;
	sAESInfoType *psAESInfo = (sAESInfoType *) arg;
    
	if (S_u8AESInitFlag != INIT_OK)
	{
		/* Return Operation not permitted, need init first */
		return -EPERM;
	}
	  
	switch(cmd)
	{
		case AESS_AESDRV_ENCRYPT:
		    
	        /* Check the structure address is OK */
	        if (!access_ok(VERIFY_WRITE, (void *) psAESInfo, 
	                       sizeof(sAESInfoType)))
	        {
	        	/* error occur */
	        	return -EFAULT;
	        }
		    
			/* Pass the add command to command handler */
			err_check = aess_aesdrv_encrypt(psAESInfo);
			
			break;
			
		case AESS_AESDRV_DECRYPT:
		    
	        /* Check the structure address is OK */
	        if (!access_ok(VERIFY_WRITE, (void *) psAESInfo, 
	                       sizeof(sAESInfoType)))
	        {
	        	/* error occur */
	        	return -EFAULT;
	        }
		    
			/* Pass the add command to command handler */
			err_check = aess_aesdrv_decrypt(psAESInfo);
			
			break;
			
#ifdef AES_MEASURE_BY_GPIO
		case AESS_AESDRV_TEST:
		    
			/* Pass the add command to command handler */
			aess_aes_set_gpio((int) arg);
			
			err_check = 0;
			
			break;
#endif
			
		default:
		    
			err_check = -EINVAL;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}


static int aess_aesdrv_open(struct inode *inode, struct file *filp)
{
    DEBUGP("KN: aess_aesdrv_open\n");
    
    if (S_u8AESInitFlag != INIT_OK)
    {
        DEBUGP("KN: first time to open\n");
        
        /* reset AES module */
        iowrite32(AES_RESET, WPCM450_AESCTL);
        
		/* initiate semaphores */
		init_MUTEX(&aes_sem);
        
        S_u8AESInitFlag = INIT_OK;
    }
    
    return 0;
}

static int aess_aesdrv_release(struct inode* inode, struct file *flip)
{
    DEBUGP("KN: aess_aesdrv_release\n");
    
    return 0;
}


struct file_operations aess_aesdrv_fops = {
	.open = aess_aesdrv_open,
	.ioctl = aess_aesdrv_ioctl,
	.release = aess_aesdrv_release,
};


int __init aess_aesdrv_init(void)
{
	int result;
	
	DEBUGP("aess_aesdrv_init\n");
	
	aesdrv_cdev = cdev_alloc();
    
	aesdrv_cdev->ops = &aess_aesdrv_fops;
	result = alloc_chrdev_region(&aes_dev, 0, 1, driver_name);
    
	if (result < 0) {
		return result;
	}
    
	cdev_add(aesdrv_cdev, aes_dev, 1);
	
    /* show this message for aes driver install shell used */
    printk("mknod /dev/aess_aesdrv c %d 0\n", MAJOR(aes_dev));
	
#ifdef AES_POLLING_STATUS
    /* initiate polling timer */
    aes_polling.data = 0;
    aes_polling.function = &aess_aes_polling_func;
    
    /* timer interval is AES_POLLING_INTERVAL */
    aes_polling.expires = jiffies + AES_POLLING_INTERVAL;
    
    init_timer(&aes_polling);
	add_timer(&aes_polling);
#endif
    
	return (STATUS_OK);
}


static void __exit aess_aesdrv_exit(void)
{
	DEBUGP("aess_aesdrv_exit\n");
	
	/* Release the character device related resource */
	cdev_del(aesdrv_cdev);
	unregister_chrdev_region(aes_dev, 1);
	
#ifdef AES_POLLING_STATUS
	del_timer(&aes_polling);
#endif
}


MODULE_DESCRIPTION("AESS AES Driver");
MODULE_AUTHOR("Justin Lee <justin.lee@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_aesdrv_init);
module_exit(aess_aesdrv_exit);
