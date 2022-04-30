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
#include "spr_wdt_sp805.h"


#define SPEAR_WDT_NAME		"WD Timer"
#define SPEAR_WDT_MAJOR		MAJOR_WDT

unsigned long WDT_CLOCK_TICK_RATE = 30000000;

//Spin Lock Variable
static spinlock_t WDT_Spin_Lock;


/****************************************************************************/
/* 			 WDT_LockUnlock()                 	            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Allows to spin lock or Unlock.			    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - LockStatus: Lock Status, Lock or Unlock                           */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static void WDT_LockUnlock(t_LockStatus LockStatus)
{
	//Select Lock or Unlock
	if(LockStatus == LOCK)
	{
		spin_lock(&WDT_Spin_Lock);
	}
	else if(LockStatus == UNLOCK)
	{
		spin_unlock(&WDT_Spin_Lock);
	}
}

/****************************************************************************/
/* 			 SPEAR_WDT_StartTimer()	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Starts the Timer. The Reset time for CPU is given in   */
/*      milliseconds.						            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - ResetTime: Reset Time in Milliseconds	                            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_WDTError SPEAR_WDT_StartTimer(uint ResetTime)
{
	t_WDTError Status;

	//START SPIN LOCK.
	WDT_LockUnlock(LOCK);

	//Configuring WDT
	Status = WDT_StartTimer(ResetTime);

	//END SPIN LOCK.
	WDT_LockUnlock(UNLOCK);

	return Status;
}


/****************************************************************************/
/* NAME: 		SPEAR_WDT_RefreshTimer                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Refresh the Timer.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_WDT_RefreshTimer(void)
{
	//START SPIN LOCK.
	WDT_LockUnlock(LOCK);

	//Configuring WDT
	WDT_RefreshTimer();

	//END SPIN LOCK.
	WDT_LockUnlock(UNLOCK);
}

/****************************************************************************/
/* NAME: 		SPEAR_WDT_StopTimer	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Stops the Timer.					    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
void SPEAR_WDT_StopTimer(void)
{
	//START SPIN LOCK.
	WDT_LockUnlock(LOCK);

	//Configuring WDT
	WDT_StopTimer();

	//END SPIN LOCK.
	WDT_LockUnlock(UNLOCK);
}


/****************************************************************************/
/* NAME: 		SPEAR_WDT_GetTimeLeft	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Returns the Time Left for the CPU to reset.	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : none                                                               */
/*                                                                          */
/* OUT : none                                                               */
/*      - ResetTime: Reset Time in Milliseconds	                            */
/*                                                                          */
/* RETURN:                                                                  */
/*        t_WDTError                   	                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
t_WDTError SPEAR_WDT_GetTimeLeft(uint *ResetTimeLeft)
{
	t_WDTError Status;

	//START SPIN LOCK.
	WDT_LockUnlock(LOCK);

	//Configuring WDT
	Status = WDT_GetTimeLeft(ResetTimeLeft);

	//END SPIN LOCK.
	WDT_LockUnlock(UNLOCK);

	return Status;
}


/****************************************************************************/
/* NAME:           SPEAR_WDT_IntHandler					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This is the interrupt handler routine.				    */
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
/*static irqreturn_t SPEAR_WDT_IntHandler(int Irq, void *Dev_id, struct pt_regs *Reg)
{
	printk("aya re\n");
	return IRQ_HANDLED;
}
*/
/****************************************************************************/
/* NAME:           SPEAR_WDT_Open					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine opens the char device				    */
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
static int SPEAR_WDT_Open(struct inode *inode, struct file *file) 
{
	return 0;
}



/****************************************************************************/
/* NAME:           SPEAR_WDT_Release					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device		      		    */
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
static int SPEAR_WDT_Release(struct inode *inode, struct file *file) 
{
	return 0;
}

/****************************************************************************************/
/* NAME:           SPEAR_WDT_IOCTL					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the file IOCTL operation of the Char Device WDT		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         Inode: index node of the device      		            		*/
/*         File : char device file name					    		*/
/*         Cmd  : command specifying the operation to be done               		*/
/*         Arg  : Parameters to be recieved/to send                         		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*----------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_WDT_IOCTL(struct inode *Inode, struct file *File, uint Cmd, unsigned long Buf)
{
	uint ResetTime;
	int err;

	switch(Cmd)
	{
		case WDTIOC_START_TIMER:
			err=copy_from_user(&ResetTime,(uint *)Buf, sizeof(ResetTime));
			return(SPEAR_WDT_StartTimer(ResetTime));
			break;
		case WDTIOC_REFRESH_TIMER:
			SPEAR_WDT_RefreshTimer();
			break;
		case WDTIOC_STOP_TIMER:
			SPEAR_WDT_StopTimer();
			break;
		default:
			return WDT_UNKNOWN_IOCTL_CMD;
	}
	return WDT_OK;
}


//Structure defining input output operations on the char device.
static struct file_operations SPEAR_WDT_Fops =
{
	.owner	= THIS_MODULE,
	.ioctl	= SPEAR_WDT_IOCTL,
	.open   = SPEAR_WDT_Open,
	.release= SPEAR_WDT_Release
};

#if 0
void WDT_TEST(void)
{
	SPEAR_WDT_StartTimer(5000);
	mdelay(3000);
	SPEAR_WDT_RefreshTimer();
}
#endif

/****************************************************************************/
/* NAME:           SPEAR_WDT_Init					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine registers the WDT driver.			 	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  : None                                                               */
/* OUT : None                                                               */
/*									    */
/* RETURN: t_WDTError							    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
static int __init SPEAR_WDT_Init(void)
{
	int Ret;

	Ret = register_chrdev(SPEAR_WDT_MAJOR, SPEAR_WDT_NAME, &SPEAR_WDT_Fops);

	if(Ret < 0)
	{
		printk(KERN_ERR SPEAR_WDT_NAME ": unable to register character device\n");
		return Ret;
	}

	spin_lock_init(&WDT_Spin_Lock);

/*	//Registering Interrupt Handler.
	Ret = request_irq(IRQ_BASIC_WDT, (void *)SPEAR_WDT_IntHandler, SA_INTERRUPT, SPEAR_WDT_NAME, NULL);
	if(Ret)
	{
		printk("\n\nFailed to Install spear WDT 0 Irq handler: %d\n\n", Ret);
	}
*/

//	WDT_TEST();


	return WDT_OK;
}

/****************************************************************************/
/* NAME:           SPEAR_WDT_Exit					    */
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
static void __exit SPEAR_WDT_Exit(void)
{
	//Unregisters the Driver
	unregister_chrdev(SPEAR_WDT_MAJOR, SPEAR_WDT_NAME);
}

module_init(SPEAR_WDT_Init);
module_exit(SPEAR_WDT_Exit);


//Export Symbols
EXPORT_SYMBOL(SPEAR_WDT_StartTimer);
EXPORT_SYMBOL(SPEAR_WDT_RefreshTimer);
EXPORT_SYMBOL(SPEAR_WDT_StopTimer);
EXPORT_SYMBOL(WDT_GetTimeLeft);
