/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2007 by Rajeev Kumar 			*/
/*   rajeev-dlh kumar@st.com							*/
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
#include "../busses/spr_i2c_syn.h"


//#define ODD_OFFSET
#define SPEAR_EEPROM_BASE 'S'
#define EEPROMIOC_CONFIG _IOWR(SPEAR_EEPROM_BASE,1,t_I2C_Configure)

#define SPEAR_EEPROM_NAME "M24CXX_I2C"
static const int g_I2C_devno_m24cxx_eeprom = MKDEV(241,0);
extern u32 Slave_Offset; 

t_I2C_Configure M24CXX_Config;

struct M24CXX_device
{
	spinlock_t eeprom_lock;
	struct cdev eeprom_cdev;
	u8	eeprom_cdev_open;
	u8	*TxBuffer;
	u8 	*RxBuffer; 
};

static	struct M24CXX_device eepromdev;

/****************************************************************************************/
/* NAME:           SPEAR_M24CXX_Write					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application to send data					*/
/*                                                                          		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :      										*/
/*            fd    :file opearation pointer		                    		*/
/*            buf   :data to write         		                    		*/
/*            len   : data length           		                    		*/
/*            f_pos :offset pointer       		                    		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:      bytes wrote on SUCCESS		                                	*/
/*	       	   -1 on FAILURE					    		*/


/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/


ssize_t
SPEAR_M24CXX_Write(struct file *fd, const char __user *Buf, size_t len, loff_t *f_pos) 
{
	unsigned int	Bytes_Written;
	unsigned int 	Bytes_Wrote1=0;
	unsigned int 	Bytes_Wrote2=0;
	unsigned int  	BytesToBeSend =0;
	unsigned int    OddBytes=0;
	unsigned int 	eeprom_offset=0;
	char *temp;

	eeprom_offset = M24CXX_Config.slave_offset;
	printk(" M24CXX_Config.slave_offset in Write is %x\n", M24CXX_Config.slave_offset);
	eepromdev.TxBuffer =(unsigned  char *)kmalloc(len,GFP_ATOMIC);
	
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_from_user(eepromdev.TxBuffer,(u8 *)Buf,len))
	{
		return -1 ;	//in case of error
		
	}
	if(len>MAX_BUFSIZE)
	{
		printk("len excedded maximum buffer size\n");
		return -1;
	}
	
//	printk("eeprom_offset->Slave_Offset is %0x %0x\n",eeprom_offset,Slave_Offset);
	temp = eepromdev.TxBuffer;
	
	BytesToBeSend = len;

#ifdef ODD_OFFSET
	OddBytes = eeprom_offset%8;
	
	if( OddBytes!=0 && len>OddBytes )
	{
		OddBytes = BLK_SIZE - OddBytes;
		Bytes_Wrote1 = SPEAR_I2C_Transmit(temp,OddBytes);
		BytesToBeSend -= OddBytes ;
	
		Slave_Offset  += OddBytes; 
		temp = (eepromdev.TxBuffer + OddBytes);

	}
	
	
	Bytes_Wrote2 = SPEAR_I2C_Transmit(temp ,BytesToBeSend);
	Bytes_Written = Bytes_Wrote1 + Bytes_Wrote2;
#else
	Bytes_Written = SPEAR_I2C_Transmit(temp ,BytesToBeSend);
#endif
	kfree(eepromdev.TxBuffer);
	return Bytes_Written;	

}
/****************************************************************************************/
/* NAME:           SPEAR_M24CXX_Read					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application to send data					*/
/*                                                                          		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :      										*/
/*            fd    :file opearation pointer		                    		*/
/*            buf   :data to write         		                    		*/
/*            len   : data length           		                    		*/
/*            f_pos :offset pointer       		                    		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:      bytes_copied  on SUCCESS	                                	*/
/*	       	   -1 on FAILURE					    		*/


/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
ssize_t
SPEAR_M24CXX_Read(struct file *fd, char __user *Buf, size_t len, loff_t *f_pos) 
{
	 unsigned int Bytes_Copied=0;
	unsigned int  Bytes_Copied1=0;
	unsigned int  Bytes_Copied2=0;
	unsigned int  BytesToBeReceived = 0;
	unsigned int   OddBytes = 0;
	unsigned int eeprom_offset = 0;
	char *temp;
	eepromdev.RxBuffer =( unsigned char *)kmalloc(len,GFP_ATOMIC);
	
	temp = eepromdev.RxBuffer;
		
	eeprom_offset = M24CXX_Config.slave_offset;
	Slave_Offset = M24CXX_Config.slave_offset;
	BytesToBeReceived = len;
	

#ifdef ODD_OFFSET
	OddBytes = eeprom_offset%8;
	if(OddBytes!=0)
	{
		OddBytes = BLK_SIZE - OddBytes;
		Bytes_Copied1 = SPEAR_I2C_Receive(temp,OddBytes);
		BytesToBeReceived -= OddBytes ;
		Slave_Offset  += OddBytes;
	        temp = (eepromdev.RxBuffer + OddBytes);
		printk("*********************************\n");	

	}

	Bytes_Copied2 = SPEAR_I2C_Receive(temp,BytesToBeReceived);
	Bytes_Copied = Bytes_Copied1 + Bytes_Copied2;
#else	
	Bytes_Copied = SPEAR_I2C_Receive(temp,BytesToBeReceived);
#endif
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_to_user((u8 *)Buf,eepromdev.RxBuffer,Bytes_Copied))
	{
		return -1;
	}
	
	kfree(eepromdev.RxBuffer);
	return Bytes_Copied;	

}

/****************************************************************************************/
/* NAME:           SPEAR_M24CXX_Ioctl					    		*/
/*--------------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             		*/
/*      This routine implements the IOCTL operation of the Char Device I2C. IOCTL is    */
/*      used to configure I2C Device.                                       		*/
/*                                                                          		*/
/* PARAMETERS:                                                              		*/
/* IN  :                                                                		*/
/*         inode: index node of the device      		            		*/
/*         fd	: file descriptor associated with the file.		    		*/
/*         cmd  : command specifying the operation to be done               		*/
/*         arg  : Parameters to be recieved/to send                         		*/
/* OUT :                                                                    		*/
/*                                                                          		*/
/* RETURN:                                                                  		*/
/*       int	                                                            		*/
/*--------------------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           		*/
/****************************************************************************************/
static int SPEAR_M24CXX_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long *Buf)
{
	int Status = I2C_FAIL;
	
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_from_user(&M24CXX_Config,(struct t_I2C_Configure *)Buf, sizeof(M24CXX_Config))!=I2C_OK)
	{
		Status = EFAULT;
		goto out;
	}

	//Checking if Config Command is called.
	if(cmd == EEPROMIOC_CONFIG)
	{
		//Configuring I2C Device.
		 SPEAR_I2C_ConfigureUpdate(&M24CXX_Config);
	}
out:
	return Status;
}

/********************************************************************************/
/* NAME:           SPEAR_M24CXX_Open			                        */
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/*      This routine opens the char device for eeprom          		        */
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :      inode:  for accessing file				        */
/*            fd :file opearation pointer		                        */
/* OUT :                                                                    	*/
/*                                                                          	*/
/* RETURN:     0 = SUCCESS		                                        */
/*             1 = FAILURE                                                  	*/
/*------------------------------------------------------------------------------*/

static int SPEAR_M24CXX_Open(struct inode *inode, struct file *fd)
{
	unsigned long		flags;
	int status = I2C_FAIL;
	

	spin_lock_irqsave (&eepromdev.eeprom_lock, flags);

	if (!eepromdev.eeprom_cdev_open) {
		eepromdev.eeprom_cdev_open = 1;
		fd->private_data = &eepromdev;
		status = 0;
	}

	spin_unlock_irqrestore (&eepromdev.eeprom_lock, flags);

	printk("SPEAR_M24CXX_Open returned %x\n", status);

	return status;
}

/****************************************************************************/
/* NAME:           SPEAR_M24CXX_Release                 	                     */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for eeprom	                    */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file			            */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    */
/*             1 = FAILURE                                                  */
/*--------------------------------------------------------------------------*/
static int SPEAR_M24CXX_Release(struct inode *inode, struct file *file)
{
	unsigned long flags;       
     

	spin_lock_irqsave(&eepromdev.eeprom_lock, flags);

	eepromdev.eeprom_cdev_open = 0;

	spin_unlock_irqrestore(&eepromdev.eeprom_lock, flags);

	DEBUG(dev, "eeprom_close\n");

	return 0;
}

static struct file_operations SPEAR_M24CXX_fops = 
{
	.owner 	= THIS_MODULE,
	.read 	= SPEAR_M24CXX_Read,
	.write 	= SPEAR_M24CXX_Write,
	.open 	= SPEAR_M24CXX_Open,
	.release= SPEAR_M24CXX_Release,
	.ioctl 	= SPEAR_M24CXX_Ioctl
};

/****************************************************************************/
/* NAME:      __exit SPEAR_M24CXX_Cleanup (void)	         		    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when M24CXX module is unloaded                        */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void __exit
SPEAR_M24CXX_cleanup(void)
{
	cdev_del(&eepromdev.eeprom_cdev) ;
	spin_lock(&eepromdev.eeprom_lock);
	unregister_chrdev_region(g_I2C_devno_m24cxx_eeprom, 1);

	spin_unlock(&eepromdev.eeprom_lock);
}

module_exit(SPEAR_M24CXX_cleanup);

/****************************************************************************/
/* NAME:  			SPEAR_M24CXX_Init 				    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        initializes the eeprom                                               */
/* PARAMETERS:                                                              */
/*                                                                          */
/* OUT :   None                                                             */
/*                                                                          */
/*                                                                          */
/* RETURN: static int					                    */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

static int __init SPEAR_M24CXX_Init(void)
{
	int status;
	status = register_chrdev_region(g_I2C_devno_m24cxx_eeprom, 1,SPEAR_EEPROM_NAME );
	if (status)
	{
		printk( "register_chrdev_region %x\n", status);
		return status;
	}
	cdev_init(&eepromdev.eeprom_cdev, &SPEAR_M24CXX_fops);
	eepromdev.eeprom_cdev.owner = THIS_MODULE;

	status = cdev_add(&eepromdev.eeprom_cdev, g_I2C_devno_m24cxx_eeprom, 1);
	if (status)
	{
		//If Error occurs while adding a cdev instance then exit.
		printk("***unable to register character device***\n");
		goto fail;
	}
	
	eepromdev.TxBuffer = NULL;
	eepromdev.RxBuffer = NULL;
	spin_lock_init(&eepromdev.eeprom_lock);

	//Initializing I2C Devices with the default settings.
	//status= SPEAR_I2C_Init();


	return status;
fail:
		SPEAR_M24CXX_cleanup();
		return -1;	
}

module_init(SPEAR_M24CXX_Init);



MODULE_AUTHOR("Rajeev Kumar");
MODULE_DESCRIPTION("SPEAR EEPROM  driver");
MODULE_SUPPORTED_DEVICE("/dev/eeprom-i2c");
MODULE_LICENSE("GPL");

