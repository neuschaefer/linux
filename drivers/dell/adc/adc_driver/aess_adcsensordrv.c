/*
 * $RCSfile$
 * $Revision$
 * $Date$
 * $Author$
 *
 * WPCM450 On chip ADC sensor driver.
 *  
 * Copyright (C) 2007 Avocent Corp.
 *
 * This file is subject to the terms and conditions of the GNU 
 * General Public License. This program is distributed in the hope 
 * that it will be useful, but WITHOUT ANY WARRANTY; without even 
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
 * PURPOSE.  See the GNU General Public License for more details.
 */
#define AESSADCSENSORDRV_C

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/signal.h>
#include <linux/spinlock.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <asm/delay.h>
#include "aess_adcsensordrv.h"

//#define ADCSENSOR_DEBUG
#ifdef ADCSENSOR_DEBUG
	#define PDEBUG(fmt, args...) printk(fmt, ## args)
  
  static char *S_ADCChnlString[] =
  {
    "ADCI0", "ADCI1", "N/A", "VSB12", "VDD33", "N/A", "N/A", "ADCI2"
  };
#else
	#define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

/* driver name will passed by insmod command */
static char *driver_name = "aess_adcsensordrv"; 


/* Init flag */
static UINT8 S_u8init_flag = AESSADCSENSOR_NOT_INIT;
static dev_t dev;    
struct cdev *adcsensor_cdev;   

static int aess_adcsensor_open(struct inode *inode, struct file *filp)
{     
	if (S_u8init_flag == AESSADCSENSOR_NOT_INIT)
	{   
		PDEBUG("KN: Open the ADC Sensor Device!!!\n");

		/* Enable the ADC Module */  
		iowrite32((UINT32) WPCM450_ADCCON_ADC_EN, (void *) WPCM450_ADCCON);
		
		S_u8init_flag = AESSADCSENSOR_INIT_OK;
	}
    
	return (STATUS_OK);
}

static int aess_adcsensor_read(sADCSensorData *pADCSensorStruct)
{
	UINT8  u8ChannelNum = 0;
	UINT32 regtemp = 0;
	int cnt =0;
	
	u8ChannelNum = pADCSensorStruct->u8ADCChannelNum;   

	/* Select ADC channal */
	regtemp = ioread32((void *) WPCM450_ADCCON) ;
	regtemp &= ~WPCM450_ADCCON_MUXMASK;
        
	iowrite32((UINT32) (regtemp | WPCM450_ADCCON_ADCMUX(u8ChannelNum)), (void *) WPCM450_ADCCON);

	/* Activate convert the ADC input */
	regtemp = ioread32((void *) WPCM450_ADCCON);
	iowrite32((UINT32) (regtemp | WPCM450_ADCCON_ADC_CONV), (void *) WPCM450_ADCCON);

	/* Wait value */
	while((ioread32((void *) WPCM450_ADCCON) & WPCM450_ADCCON_ADC_CONV) != 0)
	{
		if (cnt < WPCM450_ADC_CONVERT_MAX_RETRY_CNT)
			cnt++;
		else
		{
			/* TBD: need to reset ADC module for clean ADC_CONV */
			printk("KN: ADC CONVERT FAIL!!\n");
			return (-EAGAIN);
		}
	}

	/* finish to convert */
	pADCSensorStruct->u32ADCReading 
	    = WPCM450_ADCCON_ADC_DATA_MASK(ioread32(WPCM450_ADCDATA));
			   
	PDEBUG("KN: [%d_%s] ADCReading=%ld [%ldmV]\n", 
		   u8ChannelNum, S_ADCChnlString[u8ChannelNum], pADCSensorStruct->u32ADCReading,
		   (pADCSensorStruct->u32ADCReading * 3300 / 1024));
    
	return 0;
			
}

static int aess_adcsensor_ioctl(struct inode * inode, struct file *filp, 
						  unsigned int cmd, unsigned long arg)
{
	int err_check;
	sADCSensorData *pADCSensorStruct = (sADCSensorData *) arg;
	
	/* Check the structure address is OK */
	if(!access_ok(VERIFY_READ, (void *) pADCSensorStruct, sizeof(sADCSensorData)))
	{
		PDEBUG("KN: buffer access error!!\n");
		/* error occur */
		return -EFAULT;
	}     

	if(!access_ok(VERIFY_WRITE, (void *) pADCSensorStruct, sizeof(sADCSensorData)))
	{
		PDEBUG("KN: struct access error!!\n");
		/* error occur */
		return -EFAULT;
	}
	
	switch(cmd)
	{   
		case AESS_ADCDRV_R:

			switch (pADCSensorStruct->u8ADCChannelNum) 
			{
				case WPCM450_ADC_CHNL0_ADCI0:
				case WPCM450_ADC_CHNL1_ADCI1:
				case WPCM450_ADC_CHNL3_VSB12:
				case WPCM450_ADC_CHNL4_VDD33:
				case WPCM450_ADC_CHNL7_ADCI2:
					err_check = aess_adcsensor_read(pADCSensorStruct);  
				 	break;

				case WPCM450_ADC_CHNL2_NA:
				case WPCM450_ADC_CHNL5_NA:
				case WPCM450_ADC_CHNL6_NA:
					err_check = aess_adcsensor_read(pADCSensorStruct);  
				 	break;
					
				default:
					printk("KN: aess_adcsensor_ioctl, Unsupport channel number [%d]!\n", pADCSensorStruct->u8ADCChannelNum);
					err_check = -ENODEV;
					break;
			}

			 break;
			
		default:
			printk("KN: aess_adcsensor_ioctl, command error!!! \n");
			err_check = -EINVAL;
	}
	
	/* 0->ok, minus->fail */
	return err_check;
}

static int aess_adcsensor_release(struct inode *inode, struct file *filp)
{   
	UINT32 regtemp = 0;
    
	regtemp = ioread32((void *) WPCM450_ADCCON);
    
	/* Disable the ADC Module */  
	iowrite32((UINT32) (regtemp & ~WPCM450_ADCCON_ADC_EN), (void *) WPCM450_ADCCON);

	S_u8init_flag = AESSADCSENSOR_NOT_INIT;

	return (STATUS_OK);
}

struct file_operations aess_adcsensor_fops = {
	.open = aess_adcsensor_open, 
	.ioctl = aess_adcsensor_ioctl,
	.release = aess_adcsensor_release,
};

int __init aess_adcsensor_init(void)
{
	int result;
	adcsensor_cdev = cdev_alloc();
	
	PDEBUG("KN: Init_aess_adcsensor_module Sucess!!! \n");
	adcsensor_cdev->ops = &aess_adcsensor_fops;
	result = alloc_chrdev_region(&dev, 0, 1, driver_name);

	printk("mknod /dev/aess_adcsensordrv c %d 0\n", MAJOR(dev));

	if (result < 0) {
		PDEBUG ("KN: Registering the character device failed with %d\n", MAJOR(dev));
		return result;
	}
		   
	cdev_add(adcsensor_cdev, dev, 1);
	
#if 0
	/* TEST CODE */
	{
	int i=0;
	UINT32 regtemp = 0;
	sADCSensorData pADCSensorStruct;

	regtemp = ioread32((void *) WPCM450_ADCCON);
	printk("WPCM450_ADCCON=0x%lx\n", regtemp);

	pADCSensorStruct.u32ADCReading = 0;
	
	aess_adcsensor_open(NULL,NULL);

		for (i=0; i<8; i++)
		{
			#if 0
			if ((i == 2) || (i == 5) || (i == 6))
				continue;
			#endif
			
			pADCSensorStruct.u8ADCChannelNum = i;

			aess_adcsensor_read(&pADCSensorStruct);
		}		
	}
#endif

	return (STATUS_OK);
}

void __exit aess_adcsensor_exit(void)
{
   cdev_del(adcsensor_cdev);
   unregister_chrdev_region(dev,1);
   PDEBUG ("KN: aess_adcsensor_exit \n");
	
   return;
}

MODULE_DESCRIPTION("AESS ADC Sensor Driver");
MODULE_AUTHOR("Watt Wu <watt.wu@avocent.com>");
MODULE_LICENSE("GPL");
module_param(driver_name, charp, S_IRUGO);
module_init(aess_adcsensor_init);
module_exit(aess_adcsensor_exit);
