/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by viresh Kumar  			*/
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
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <asm/arch/spr_major.h>
#include "spr_gpio_pl061.h"

#define SPEAR_GPIO_NAME 	"SPEAR_gpio"
#define SPEAR_GPIO_MAJOR	MAJOR_GPIO

#if SPEAR_GPIO_DEBUG
t_GPIOError IntTesting(t_IntInfo *);


void TestBinder0()
{
	printk("\n\nThis is the interrupt handler function of Pin 0....\n\n");
}
void TestBinder1()
{
	printk("\n\nThis is the interrupt handler function of Pin 1....\n\n");
}
void TestBinder2()
{
	printk("\n\nThis is the interrupt handler function of Pin 2....\n\n");
}
void TestBinder3()
{
	printk("\n\nThis is the interrupt handler function of Pin 3....\n\n");
}
void TestBinder4()
{
	printk("\n\nThis is the interrupt handler function of Pin 4....\n\n");
}
void TestBinder5()
{
	printk("\n\nThis is the interrupt handler function of Pin 5....\n\n");
}
void TestBinder6()
{
	printk("\n\nThis is the interrupt handler function of Pin 6....\n\n");
}
void TestBinder7()
{
	printk("\n\nThis is the interrupt handler function of Pin 7....\n\n");
}
#endif

//Spin Lock Variable
static spinlock_t SpinLocks[GPIO_NUM];

/*------------------------------------------------------------------------
 * a global structure representing the entire GPIO
 *----------------------------------------------------------------------*/


/****************************************************************************/
/* 			 GPIO_LockUnlock()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Allows to spin lock or Unlock a GPIO Device.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - GPIOId: Identify the GPIO Device	                            */
/*      - LockStatus: Lock Status, Lock or Unlock                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_GPIOError                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static t_GPIOError GPIO_LockUnlock(t_GPIOId GPIOId, t_LockStatus LockStatus)
{
	//Checking if GPIOId is correct.
	if((GPIOId < GPIO0) ||(GPIOId >= GPIO_NONE))
		return GPIO_INVALID_ID;

	//Select Lock or Unlock
	if(LockStatus == LOCK)
		spin_lock(&SpinLocks[GPIOId]);
	else if(LockStatus == UNLOCK)
		spin_unlock(&SpinLocks[GPIOId]);
	else
		return GPIO_INVALID_PARAMETER;

	return GPIO_OK;
}
/****************************************************************************/
/* NAME:           SPEAR_GPIO_ConfigDefault				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to configure the GPIO to the default settings.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError SPEAR_GPIO_ConfigDefault(t_GPIOId GPIOId)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Configuring GPIO
	Status = GPIO_ConfigDefault(GPIOId);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_GPIO_DirSet					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Set the direction of the Specified GPIO Pins.   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Dir: Dir of the pins to be set					    */
/*	Mask: Pins whose direction is required to be set 		    */
/* OUT : 								    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

t_GPIOError SPEAR_GPIO_DirSet(t_GPIOId GPIOId, t_GPIODir Dir, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Setting Direction of a pin
	Status = GPIO_DirSet(GPIOId, Dir, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}

/****************************************************************************/
/* NAME:           SPEAR_GPIO_DirGet					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Get the direction of the Specified GPIO Pins.   */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose direction is required				    */
/* OUT : 								    */
/*	Dir: Direction of the pin					    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError SPEAR_GPIO_DirGet(t_GPIOId GPIOId, t_GPIODir *Dir,uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Getting Direction of a pin
	Status = GPIO_DirGet(GPIOId, Dir, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_GPIO_ValSet					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Set the value of the Specified GPIO Pins.       */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose value is to be Set.				    */
/*	Val: Value of the pin to be set.				    */
/* OUT : 								    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

t_GPIOError SPEAR_GPIO_ValSet(t_GPIOId GPIOId, t_GPIOValue Val, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Setting Value of a pin
	Status = GPIO_ValSet(GPIOId, Val, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_GPIO_ValGet					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*	This API is used to Get the value at the Specified GPIO Pin.        */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : 								    */
/*	Mask: Pin whose Value is Required				    */
/* OUT : 								    */
/*	Val: Value at the pin.						    */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_GPIOError SPEAR_GPIO_ValGet(t_GPIOId GPIOId, t_GPIOValue *Val, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Getting Value of a pin
	Status = GPIO_ValGet(GPIOId, Val, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}

/********************************************************************************/
/* NAME:           SPEAR_GPIO_EdgeConfig				        */
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to configure Edge Interrupt on the specified GPIO Pins. 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	IntEdge: Type of interrupt to be set Single Edge or Both Edge.	    	*/
/*	IntSingleEdge : Rising or Falling Edge interrupt in case of Single Edge	*/
/*	Mask: Pin required to be set for Edge Interrupt			    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError SPEAR_GPIO_EdgeConfig(t_GPIOId GPIOId, t_GPIOIntEdgeConfig IntEdge, t_GPIOIntSingleEdgeConfig IntSingleEdge, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Configuring GPIO for Interrupts.
	Status = GPIO_EdgeConfig(GPIOId, IntEdge, IntSingleEdge, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO_LevelConfig					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to configure Level Interrupt on the specified GPIO Pins. 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	IntLevel: Type of interrupt to be set Low Level or High Level.	    	*/
/*	Mask: Pin required to be set for Level Interrupt			*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError SPEAR_GPIO_LevelConfig(t_GPIOId GPIOId, t_GPIOIntLevelConfig IntLevel, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Configuring GPIO for Interrupts.
	Status = GPIO_LevelConfig(GPIOId, IntLevel, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO_IntEnable						*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Enable Interrupt on the specified GPIO Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be set for Interrupts				*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError SPEAR_GPIO_IntEnable(t_GPIOId GPIOId, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Enabling Interrupts.
	Status = GPIO_IntEnable(GPIOId, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO_IntDisable					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Disable Interrupt on the specified GPIO Pins.	 	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be Disabled for Interrupts			*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError SPEAR_GPIO_IntDisable(t_GPIOId GPIOId, uint Mask)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Disabling Interrupts.
	Status = GPIO_IntDisable(GPIOId, Mask);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO_BindIntHandler					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This API is used to Bind Interrupt Handler with the specified GPIO Pins.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/*	Mask: Pin required to be Cleared for Interrupts				*/
/*	HandlerPtr: Pointer to Interrupt handler to be registered		*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: t_GPIOError							    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
t_GPIOError SPEAR_GPIO_BindIntHandler(t_GPIOId GPIOId, uint Mask, t_GPIOIntHandler HandlerPtr)
{
	t_GPIOError Status;

	//START SPIN LOCK.
	if((Status = GPIO_LockUnlock(GPIOId, LOCK)) != GPIO_OK)
		return Status;

	//Binding Interrupt Handlers.
	Status = GPIO_BindIntHandler(GPIOId, Mask, HandlerPtr);

	//END SPIN LOCK.
	GPIO_LockUnlock(GPIOId, UNLOCK);

	return Status;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO0_IntHandler					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function of GPIO 0 to be registered to the kernel.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
static irqreturn_t SPEAR_GPIO0_IntHandler(int Irq, void *dev_id, struct pt_regs *regs)
{
#if SPEAR_GPIO_DEBUG
	printk("This is GPIO 0 Interrupt Handler.\n");
#endif
	GPIO_IntHandler(GPIO0);
	return IRQ_HANDLED;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO1_IntHandler					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function of GPIO 1 to be registered to the kernel.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
static irqreturn_t SPEAR_GPIO1_IntHandler(int Irq, void *dev_id, struct pt_regs *regs)
{
#if SPEAR_GPIO_DEBUG
	printk("This is GPIO 1 Interrupt Handler.\n");
#endif
	GPIO_IntHandler(GPIO1);
	return IRQ_HANDLED;
}


/********************************************************************************/
/* NAME:           SPEAR_GPIO2_IntHandler					*/
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/* This is Interrupt Handler function of GPIO 2 to be registered to the kernel.	*/
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  : 								    	*/
/* OUT : 								    	*/
/*									    	*/
/* RETURN: 								    	*/
/*------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           	*/
/********************************************************************************/
#ifdef CONFIG_ARCH_SPEARPLUS
static irqreturn_t SPEAR_GPIO2_IntHandler(int Irq, void *dev_id, struct pt_regs *regs)
{
#if SPEAR_GPIO_DEBUG
	printk("This is GPIO 2 Interrupt Handler.\n");
#endif
	GPIO_IntHandler(GPIO2);
	return IRQ_HANDLED;
}
#endif


/****************************************************************************/
/* NAME:           SPEAR_GPIO_Open					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine opens the char device for gpio          		    */
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

static int SPEAR_GPIO_Open(struct inode *inode, struct file *file) 
{
	return 0;
}



/****************************************************************************/
/* NAME:           SPEAR_GPIO_Release					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for gpio       		    */
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
static int SPEAR_GPIO_Release(struct inode *inode, struct file *file) 
{
	return 0;
}



/****************************************************************************************/
/* NAME:           SPEAR_GPIO_Read					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file Read operation of the Char Device GPIO		*/
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
static int SPEAR_GPIO_Read(struct file *file, char __user *Buf, size_t len, loff_t *ppos)
{
	t_GPIOPinInfo PinData;
	uint Success;

	copy_from_user(&PinData,(struct t_GPIOPinInfo*)Buf, sizeof(PinData));
	Success = SPEAR_GPIO_ValGet(PinData.GPIOId, (t_GPIOValue *)(&(PinData.PinInfo.PinVal)), (size_t)(PinData.PinMask));
	if(!Success)
	{
		copy_to_user((char *)Buf,&PinData,sizeof(PinData));
	}
	return Success;
}



/****************************************************************************************/
/* NAME:           SPEAR_GPIO_Write					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file write operation of the Char Device GPIO	*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         file: Char device file name                                         		*/
/*         Buf: Data to be written                                         		*/
/*         len: Number of units to be written                                 		*/
/* OUT :                                                                    		*/
/*         ppos: Number of units written                                      		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_GPIO_Write(struct file *file, const char __user *Buf, size_t len, loff_t *ppos) 
{
	t_GPIOPinInfo PinData;

	copy_from_user(&PinData,(struct t_GPIOPinInfo*)Buf, sizeof(PinData));
	return(SPEAR_GPIO_ValSet(PinData.GPIOId, (t_GPIOValue)(PinData.PinInfo.PinVal), (size_t)PinData.PinMask));
}


/****************************************************************************************/
/* NAME:           SPEAR_GPIO_IOCTL					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file IOCTL operation of the Char Device GPIO	*/
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
/*----------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_GPIO_IOCTL(struct inode *inode, struct file *file, uint cmd, unsigned long Buf)
{
	t_GPIOPinInfo PinData;
	uint Success;

#if SPEAR_GPIO_DEBUG
	t_IntInfo IntData;
#endif

	copy_from_user(&PinData,(struct t_GPIOPinInfo*)Buf, sizeof(PinData));
	switch(cmd)
	{
		case GPIOIOC_CONFIGDEFAULT:
			return(SPEAR_GPIO_ConfigDefault(PinData.GPIOId));
			break;
		case GPIOIOC_DIRSET:
			return(SPEAR_GPIO_DirSet(PinData.GPIOId, (t_GPIODir)(PinData.PinInfo.PinDir),PinData.PinMask));
			break;
		case GPIOIOC_DIRGET: 
			Success = SPEAR_GPIO_DirGet(PinData.GPIOId, (t_GPIODir *)&(PinData.PinInfo.PinDir),PinData.PinMask);
			if(!Success)
			{
				copy_to_user((char *)Buf,&PinData,sizeof(PinData));
			}
			return Success;
			break;
#if SPEAR_GPIO_DEBUG
		case GPIOIOC_INT_TESTING:
			copy_from_user(&IntData,(struct t_GPIOPinInfo*)Buf, sizeof(IntData));
			return IntTesting((t_IntInfo *)(&IntData));
			break; 

#endif
		default:
			return GPIO_UNKNOWN_ERROR;
	}
}

#if SPEAR_GPIO_DEBUG
void Handler(void)
{
	printk("IntHandler Called.\n");
}

void TEST_KERNEL(void)
{
	t_GPIODir Dir = OUT;
	t_GPIOValue Val = SET;
	t_GPIOId GPIOId = GPIO0;

//Setting and Getting Direction.
	if(!SPEAR_GPIO_DirSet(GPIOId, OUT, GPIO_PIN4))
		printk("DirSet for Pin 4\n");
	if(!SPEAR_GPIO_DirSet(GPIOId, IN, GPIO_PIN5))
		printk("DirSet for Pin 5\n");

	if(!SPEAR_GPIO_DirGet(GPIOId, &Dir,GPIO_PIN4))
		printk("DirGet for Pin 4: %d\n", Dir);
	if(!SPEAR_GPIO_DirGet(GPIOId, &Dir,GPIO_PIN5))
		printk("DirGet for Pin 5: %d\n", Dir);

//Setting and Getting Value.
	if(!SPEAR_GPIO_ValSet(GPIOId, Val, GPIO_PIN4))
		printk("ValSet for Pin 4\n");
	if(SPEAR_GPIO_ValSet(GPIOId, Val, GPIO_PIN5))
		printk("ValSet Error for Pin 5\n");

	if(SPEAR_GPIO_ValGet(GPIOId, &Val, GPIO_PIN4))
		printk("ValGet Error for Pin 4: \n");
	if(!SPEAR_GPIO_ValGet(GPIOId, &Val, GPIO_PIN5))
		printk("ValGet for Pin 5: %d\n", Val);

//Setting Interrupts.
	if(!SPEAR_GPIO_BindIntHandler(GPIOId, GPIO_PIN4, Handler))
		printk("Handler for Pin 4\n");
	if(!SPEAR_GPIO_BindIntHandler(GPIOId, GPIO_PIN5, Handler))
		printk("Handler for Pin 5\n");

	if(!SPEAR_GPIO_LevelConfig(GPIOId, HIGH_LEVEL, GPIO_PIN4))
		printk("IntSet for Pin 4\n");
	if(!SPEAR_GPIO_LevelConfig(GPIOId, HIGH_LEVEL, GPIO_PIN5))
		printk("IntSet for Pin 5\n");

//	if(!SPEAR_GPIO_IntEnable(GPIOId, GPIO_PIN4))
//		printk("IntEnable for Pin 4\n");
	if(!SPEAR_GPIO_IntEnable(GPIOId, GPIO_PIN5))
		printk("IntEnable for Pin 5\n");

}
#endif

//Structure defining input output operations on the char device.
static struct file_operations SPEAR_GPIO_Fops =
{
	.owner	= THIS_MODULE,
	.write	= SPEAR_GPIO_Write,
	.read	= SPEAR_GPIO_Read,
	.ioctl	= SPEAR_GPIO_IOCTL,
	.open   = SPEAR_GPIO_Open, 
	.release= SPEAR_GPIO_Release
};


#ifdef CONFIG_ARCH_SPEARBASIC
/****************************************************************************/
/* NAME:  EnableRasInterrupt_gpio                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the GPIO of the RAS. In SPEArBasic we have */
/*      GPIO in RAS. So first RAS need to be enabled and then Base address  */
/*      of RAS need to be mapped.                                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*				                                    					    */
/* RETURN: None                             							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  EnableRasInterrupt_gpio(void)
{
    unsigned int va_ras_register = 0;
    unsigned int intr_status     = 0;

    ///////////////////////////////////////////////////////////////////////
    //SYNOPSIS                                                           //
    //ioremap - map bus memory into CPU space                            //
    //    void * ioremap (unsigned long offset, unsigned long size);     //
    //Arguments:                                                         //
    //  offset -  bus address of the memory                              //
    //  size - size of the resource to map                               //
    ///////////////////////////////////////////////////////////////////////
      
    va_ras_register = ioremap(0x50000054, 4);// in 2nd arg is ioremap, 4 is the pagesize
    
    //Read Current Interrupt status and then Set bit no:8 and then write again
	intr_status = readl( va_ras_register );
    intr_status |= 0x100; 
	writel(intr_status, va_ras_register );

    printk("spearbasic RAS gpio Interrupt enable\n");
} 


/****************************************************************************/
/* NAME:  EnablePhotoFrameMode_gpio                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the GPIO of the RAS. In SPEArBasic we have */
/*      GPIO in RAS. So first RAS need to be enabled and then Base address  */
/*      of RAS need to be mapped.                                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*				                                    					    */
/* RETURN: None                             							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  EnablePhotoFrameMode_gpio(void)
{
    unsigned int va_ras1 = 0;
	unsigned int va_ras2 = 0;
    uint *BaseAddress, *TempBaseAddress;
	t_GPIOId GPIOId;

    ///////////////////////////////////////////////////////////////////////
    //SYNOPSIS                                                           //
    //ioremap - map bus memory into CPU space                            //
    //    void * ioremap (unsigned long offset, unsigned long size);     //
    //Arguments:                                                         //
    //  offset -  bus address of the memory                              //
    //  size - size of the resource to map                               //
    ///////////////////////////////////////////////////////////////////////

#if 0	
    va_ras1 = ioremap(SPEARBASIC_RAS1, 4);// in 2nd arg is ioremap, 4 is the pagesize
    va_ras2 = ioremap(SPEARBASIC_RAS2, 4);
    
    //Enable RAS for SPEArBasic since GPIO is present in RAS
	writel(0x4, va_ras1 );
	writel(0x2, va_ras2 );

    printk("spearbasic RAS gpio enable\n");
#endif

    //Map physical addess 
	for(GPIOId = GPIO0; GPIOId < GPIO_NONE; GPIOId += (t_GPIOId)1)
    {
        BaseAddress = GetBaseAddress(GPIOId);
        TempBaseAddress = ioremap(*BaseAddress, 4);
        *BaseAddress = TempBaseAddress;
    }

}
 
/****************************************************************************/
/* NAME:  DisablePhotoFrameMode_gpio                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      In SPEArBasic we have GPIO in RAS. So first Base address of gpio of */
/*        RAS need to be unmapped and then registers of RAS need to be      */
/*        unmapped.                                                         */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									                                        */
/* RETURN: None     							                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void  DisablePhotoFrameMode_gpio(void)
{
    uint *BaseAddress;
	t_GPIOId GPIOId;

    ///////////////////////////////////////////////////////////////////////
    //SYNOPSIS                                                           //
    //iounmap - unmap CPU space of bus memory                            //
    //    void * iounmap (unsigned long offset);                         //
    //Arguments:                                                         //
    //  offset -  bus address of the memory                              //
    ///////////////////////////////////////////////////////////////////////
 
    for(GPIOId = GPIO0; GPIOId < GPIO_NONE; GPIOId += (t_GPIOId)1)
    {
        BaseAddress = GetBaseAddress(GPIOId);
        iounmap(*BaseAddress);
    }
     
#if 0	
    iounmap(SPEARBASIC_RAS1);
    iounmap(SPEARBASIC_RAS2);
#endif
	
    printk("spearbasic RAS gpio disable\n");
}
#endif

/****************************************************************************/
/* NAME:           SPEAR_GPIO_Init					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Initializes the GPIO to the Default Configurations	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_GPIOError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_GPIO_Init(void)
{
	int Ret;
	t_GPIOError Status;
	t_GPIOId GPIOId;


	Ret = register_chrdev(SPEAR_GPIO_MAJOR, SPEAR_GPIO_NAME, &SPEAR_GPIO_Fops);
	if(Ret < 0)
	{
		printk(KERN_ERR SPEAR_GPIO_NAME ": unable to register character device\n");
		return Ret;
	}

	//Initializing spin locks.
	for(GPIOId = GPIO0;GPIOId<GPIO_NONE;GPIOId += 1)
		spin_lock_init(&SpinLocks[GPIOId]);

#ifdef CONFIG_ARCH_SPEARBASIC
    EnablePhotoFrameMode_gpio();    
    EnableRasInterrupt_gpio();
#endif

	//Initializing GPIO Devices.
	Status = GPIO_Init();

#ifdef CONFIG_ARCH_SPEARBASIC
    Ret = request_irq( IRQ_GEN_RAS_1, (void *)SPEAR_GPIO0_IntHandler, SA_INTERRUPT, SPEAR_GPIO_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install sbasic GPIO 0 Irq handler: %d\n\n", Ret);
	}
    
    Ret = request_irq( IRQ_BASIC_GPIO, (void *)SPEAR_GPIO1_IntHandler, SA_INTERRUPT, SPEAR_GPIO_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install sbasic GPIO 1 Irq handler: %d\n\n", Ret);
	}
#endif

#ifdef CONFIG_ARCH_SPEARPLUS
	//Registering Interrupt Handlers.
	Ret = request_irq(IRQ_APPL_GPIO, (void *)SPEAR_GPIO0_IntHandler, SA_INTERRUPT, SPEAR_GPIO_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus GPIO 0 Irq handler: %d\n\n", Ret);
	}

	Ret = request_irq(IRQ_BASIC_GPIO, (void *)SPEAR_GPIO1_IntHandler, SA_INTERRUPT, SPEAR_GPIO_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus GPIO 1 Irq handler: %d\n\n", Ret);
	}

	Ret = request_irq(IRQ_LOCAL_GPIO, (void *)SPEAR_GPIO2_IntHandler, SA_INTERRUPT, SPEAR_GPIO_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install splus GPIO 2 Irq handler: %d\n\n", Ret);
	}
#endif

	return Status;
}

/****************************************************************************/
/* NAME:           SPEAR_GPIO_Exit					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine performs the finishing tasks                           */
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

static void __exit SPEAR_GPIO_Exit(void)
{
	unregister_chrdev(SPEAR_GPIO_MAJOR, SPEAR_GPIO_NAME);

#ifdef CONFIG_ARCH_SPEARBASIC
    DisablePhotoFrameMode_gpio();    
#endif
}


//Module Init and Exit.
module_init(SPEAR_GPIO_Init);
module_exit(SPEAR_GPIO_Exit);

//Export Symbols
EXPORT_SYMBOL(SPEAR_GPIO_ConfigDefault);
EXPORT_SYMBOL(SPEAR_GPIO_DirSet);
EXPORT_SYMBOL(SPEAR_GPIO_DirGet);
EXPORT_SYMBOL(SPEAR_GPIO_ValSet);
EXPORT_SYMBOL(SPEAR_GPIO_ValGet);
EXPORT_SYMBOL(SPEAR_GPIO_EdgeConfig);
EXPORT_SYMBOL(SPEAR_GPIO_LevelConfig);
EXPORT_SYMBOL(SPEAR_GPIO_IntEnable);
EXPORT_SYMBOL(SPEAR_GPIO_IntDisable);
EXPORT_SYMBOL(SPEAR_GPIO_BindIntHandler);

#if SPEAR_GPIO_DEBUG
t_GPIOError IntTesting(t_IntInfo *Buf)
{
	t_GPIOIntHandler Handler = NULL;
	switch(Buf->Command)
	{
		case Int_Enable:
			return SPEAR_GPIO_IntEnable(Buf->GPIOId, Buf->PinMask);
			break;
		case Int_Disable:
			return SPEAR_GPIO_IntDisable(Buf->GPIOId, Buf->PinMask);
			break;
		case Int_Bind:
			switch(Buf->PinMask)
			{
				case GPIO_PIN0: Handler = TestBinder0; 
					break;
				case GPIO_PIN1: Handler = TestBinder1; 
					break;
				case GPIO_PIN2: Handler = TestBinder2; 
					break;
				case GPIO_PIN3: Handler = TestBinder3; 
					break;
				case GPIO_PIN4: Handler = TestBinder4; 
					break;
				case GPIO_PIN5: Handler = TestBinder5; 
					break;
				case GPIO_PIN6: Handler = TestBinder6; 
					break;
				case GPIO_PIN7: Handler = TestBinder7; 
					break;
			}
			return SPEAR_GPIO_BindIntHandler(Buf->GPIOId, Buf->PinMask, Handler);
			break;
		case Int_Clear:
			return GPIO_IntClear(Buf->GPIOId, Buf->PinMask);
			break;
		case Int_Config:
			switch(Buf->IntType)
			{
				case EDGE:
					return SPEAR_GPIO_EdgeConfig(Buf->GPIOId, Buf->EdgeType, Buf->SEdgeType, Buf->PinMask);
					break;
				case LEVEL:
					return SPEAR_GPIO_LevelConfig(Buf->GPIOId, Buf->LevelType, Buf->PinMask);
					break;
				default:
					printk("Incorrect Interrupt Type.\n");
					break;
			}
			return 12;
			break;
		case Reg_Read:
			return 0;
			break;
	}
	return 0;
}
#endif
