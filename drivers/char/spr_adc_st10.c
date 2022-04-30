/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by viresh Kumar  			*/
/*   viresh.kumar@st.com							*/
/*										*/
/*   This program is free software; you can redistribute it and/or modify	*/
/*   it under the terms of the GNU General Public License as published by	*/
/*   the Free Software Foundation; either version 2 of the License, or		*/
/*   (at your option) any later version.					*/
/*										*/
/*   This program is distributed in the hope that it will be useful,		*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 		*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		*/
/*   GNU General Public License for more details.				*/
/*										*/
/*   You should have received a copy of the GNU General Public License		*/
/*   along with this program; if not, write to the				*/
/*   Free Software Foundation, Inc.,						*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.			*/
/********************************************************************************/

/*------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------*/
#include <linux/serial_core.h>
#include <linux/tty.h>
#include <linux/serial.h>
#include <linux/console.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/timex.h>	
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <asm/arch/spr_major.h>
#include "spr_adc_st10.h"

#define SPEAR_ADC_NAME		"ADC"





//Storing the Current Status.
static t_ADCStatus ADCCurrentStatus=
{
	.CurrentOwner = NULL,
	.DeviceNumber = MKDEV(MAJOR_ADC, 0),
	.FlagArrived = 0,
	.IsConfigured = FALSE
};

static uint GDigitalVolt;

/****************************************************************************/
/* 			 ADC_GotoSleep()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called when adc device is required to wait for some */
/*      interval of time.                                                   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int ADC_GotoSleep()
{
	//Goto Sleep
	wait_event_interruptible(ADCCurrentStatus.WaitQueue,(ADCCurrentStatus.FlagArrived != 0));

	//if the flag arrived is 1, then it is normal wakeup.
	if(ADCCurrentStatus.FlagArrived == 1)
		ADCCurrentStatus.FlagArrived = 0;
	//if the flag arrived is 0, then it is not normal wakeup.
	else
	{
		ADCCurrentStatus.FlagArrived = 0;
		return ADC_ERROR;
	}

	return ADC_OK;
}

/****************************************************************************/
/* 			 ADC_WakeUp()	         	                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine is called to wakeup from sleep.			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void ADC_WakeUp()
{
	//Wake Up from Sleep 
	ADCCurrentStatus.FlagArrived = 1;
	wake_up_interruptible(&ADCCurrentStatus.WaitQueue);
}

/****************************************************************************/
/* 			 SPEAR_ADC_Acquire()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine acquired ADC Device.		    		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError SPEAR_ADC_Acquire(void *DeviceId)
{
	t_ADCError Status = ADC_BUSY;

	if(DeviceId == NULL)
		return ADC_NULL_POINTER;

	spin_lock(&ADCCurrentStatus.ADCSpinLock);

	//Checking that the requested ADC is Free or Busy.
	if(ADCCurrentStatus.CurrentOwner == NULL)
	{
		//If free then allocate it to the calling device. The calling device must free the ADC device after use.
		ADCCurrentStatus.CurrentOwner = DeviceId;
		Status = ADC_OK;
	}

	spin_unlock(&ADCCurrentStatus.ADCSpinLock);
	return Status;
}

/****************************************************************************/
/* 			 SPEAR_ADC_Free()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Releases a previously acquired ADC Device.		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError SPEAR_ADC_Free(void *DeviceId)
{
	t_ADCError Status = ADC_BUSY;

	if(DeviceId == NULL)
		return ADC_NULL_POINTER;

	spin_lock(&ADCCurrentStatus.ADCSpinLock);

	//Checking that the ADC Device is owned by the caller or somebody else.
	if(ADCCurrentStatus.CurrentOwner == DeviceId)
	{
		//Free device if current user has acquired it before.
		ADCCurrentStatus.CurrentOwner = NULL;
		Status = ADC_OK;
	}

	spin_unlock(&ADCCurrentStatus.ADCSpinLock);
	return Status;
}


/****************************************************************************/
/* 			 SPEAR_ADC_Configure()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to configure a ADC Device. It calls 	    */
/*      ADC_Configure of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - Config: Configuration Information.           	    		    */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError SPEAR_ADC_Configure(t_ADCConfig Config, void *DeviceId)
{
	t_ADCError Status = ADC_OK;

	if(DeviceId == NULL)
		return ADC_NULL_POINTER;

	spin_lock(&ADCCurrentStatus.ADCSpinLock);

	//Checking that the ADC Device is owned by the caller or somebody else.
	if(ADCCurrentStatus.CurrentOwner != DeviceId)
		return ADC_BUSY;

	spin_unlock(&ADCCurrentStatus.ADCSpinLock);

	//Configure ADC.
	if((Status = ADC_Configure(Config)) == ADC_OK)
		ADCCurrentStatus.IsConfigured = TRUE;
	return Status;
}


/****************************************************************************/
/* NAME: 		SPEAR_ADC_Convert	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine converts a analog voltage to digital voltage. 	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :	                                                            */
/*      - DeviceId: DeviceId of Caller Driver Or In case of User Application*/
/*                  it is pointer to the current task_struct.               */
/* OUT :                                                                    */
/*      - DigitalVolt: Output Digital Data 	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_ADCError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_ADCError SPEAR_ADC_Convert(uint *DigitalVolt, void *DeviceId)
{
	t_ADCError Status = ADC_OK;

	if(DeviceId == NULL)
		return ADC_NULL_POINTER;

	//Checking that the ADC Device is owned by the caller or somebody else.
	spin_lock(&ADCCurrentStatus.ADCSpinLock);

	if(ADCCurrentStatus.CurrentOwner != DeviceId)
		return ADC_BUSY;

	spin_unlock(&ADCCurrentStatus.ADCSpinLock);

	//Check if ADC is configured or not.
	if(ADCCurrentStatus.IsConfigured == FALSE)
		return ADC_NOT_CONFIGURED;

	//Start Conversion.
	if((Status = ADC_StartConversion()) == ADC_OK)
	{
		//Goto Sleep 
		Status = ADC_GotoSleep();
	}

	*DigitalVolt = GDigitalVolt;

	return Status;
}

/****************************************************************************/
/* NAME:   		SPEAR_ADC_IntHandler				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 					                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static irqreturn_t SPEAR_ADC_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg)
{
	ADC_GetConvertedData(&GDigitalVolt);
	ADC_WakeUp();

	return IRQ_HANDLED;
}

/****************************************************************************/
/* NAME:           SPEAR_ADC_Open					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine opens the char device for adc          		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   none                                                             */
/*                                                                          */
/* OUT :                                                                    */
/*       None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*       int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ADC_Open(struct inode *inode, struct file *file)
{
	t_ADCError Status = ADC_OK;

	//Acquiring a ADC Device.
	Status = SPEAR_ADC_Acquire(current);

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_ADC_Release					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for adc       		    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :   none                                                             */
/*                                                                          */
/* OUT :                                                                    */
/*       None                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*       int	                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int SPEAR_ADC_Release(struct inode *inode, struct file *file)
{
	t_ADCError Status;

	//Freeing ADC Device previously acquired.
	Status = SPEAR_ADC_Free(current);

	ADCCurrentStatus.IsConfigured = FALSE;

	return Status;
}



/****************************************************************************************/
/* NAME:           SPEAR_ADC_Read					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file Read operation of the Char Device ADC		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         len: Number of units to be Read                                 		*/
/* OUT :                                                                    		*/
/*         Buf: Data Read                                         			*/
/*         ppos: Number of units Read                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_ADC_Read(struct file *file, char __user *Buf, size_t len, loff_t *ppos)
{
	t_ADCError Status;
	uint DigitalVolt;

	//Calling Receive API.
	if((Status = SPEAR_ADC_Convert(&DigitalVolt, current)) == ADC_OK)
	{
		copy_to_user(Buf, &DigitalVolt, sizeof(uint));
	}

	if(Status == ADC_OK)
		return 1;
	else
		return Status;
}

/****************************************************************************************/
/* NAME:           SPEAR_ADC_Ioctl					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the IOCTL operation of the Char Device ADC. IOCTL is    */
/*      used to configure ADC Device.                                       		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         inode: index node of the device      		            		*/
/*         file : char device file name					    		*/
/*         cmd  : command specifying the operation to be done               		*/
/*         arg  : Parameters to be recieved/to send                         		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_ADC_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long Buf)
{
	t_ADCConfig ADCConfigure;
	t_ADCError Status = ADC_OK;

	if(cmd == ADCIOC_CONFIG)
	{
		//Copying Configuration Information from User Space to Kernel Space.
		copy_from_user(&ADCConfigure,(struct t_ADCConfig *)Buf, sizeof(ADCConfigure));

		//Configuring ADC Device.
		Status = SPEAR_ADC_Configure(ADCConfigure, current);
	}

	return Status;
}


static struct file_operations SPEAR_ADC_Fops =
{
	.owner	= THIS_MODULE,
	.open   = SPEAR_ADC_Open, 
	.release= SPEAR_ADC_Release,
	.read	= SPEAR_ADC_Read,
	.ioctl	= SPEAR_ADC_Ioctl
};

/****************************************************************************/
/* NAME:  			SPEAR_ADC_Exit 				    */
/*                                                 			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*                                                                          */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: t_adc_error :  ADC error code	                            */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void SPEAR_ADC_Exit(void)
{
	cdev_del(&ADCCurrentStatus.Cdev);

	unregister_chrdev_region(ADCCurrentStatus.DeviceNumber, 1);

	//Disable ADC
	ADC_Disable();
}


/****************************************************************************/
/* NAME:  			SPEAR_ADC_Init 				    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        initializes the adc                                               */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: 						                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/


static int __init SPEAR_ADC_Init(void)
{
	int Ret;
	t_ADCError Status = ADC_OK;

	//Registering Character Device.
	Ret = register_chrdev_region(ADCCurrentStatus.DeviceNumber, 1, SPEAR_ADC_NAME);
	if(Ret < 0)
	{
		printk(KERN_ERR SPEAR_ADC_NAME ": unable to register character device\n");
		return Ret;
	}

	ADCCurrentStatus.Cdev.owner = THIS_MODULE;

	cdev_init(&ADCCurrentStatus.Cdev, &SPEAR_ADC_Fops);
	Ret = cdev_add(&(ADCCurrentStatus.Cdev), ADCCurrentStatus.DeviceNumber, 1);
	if (Ret)
	{
		//If Error occurs while adding a cdev instance then exit.
		printk(KERN_ERR SPEAR_ADC_NAME ": unable to register character device\n");
		SPEAR_ADC_Exit();
		return -EINVAL;
	}

	//Registering Interrupt Handlers.
#ifdef CONFIG_ARCH_SPEARPLUS
	Ret = request_irq(IRQ_APPL_ADC, (void *)SPEAR_ADC_IntHandler, SA_INTERRUPT, SPEAR_ADC_NAME, NULL);
#endif

#ifdef CONFIG_ARCH_SPEARBASIC
	Ret = request_irq(IRQ_ADC, (void *)SPEAR_ADC_IntHandler, SA_INTERRUPT, SPEAR_ADC_NAME, NULL);
#endif

	if(Ret)
	{
		printk("\n\nFailed to Install spear ADC 0 Irq handler: %d\n\n", Ret);
	}

	//Initialize wait queue heads.
	init_waitqueue_head(&(ADCCurrentStatus.WaitQueue));

	//Enable ADC
	ADC_Enable();

	return Status;
}

module_init(SPEAR_ADC_Init);
module_exit(SPEAR_ADC_Exit);

MODULE_AUTHOR("ST Microelectronic");
MODULE_DESCRIPTION("SPEAR generic serial port driver");
MODULE_SUPPORTED_DEVICE("ttyS");
MODULE_LICENSE("GPL");
