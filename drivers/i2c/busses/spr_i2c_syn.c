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
#include "spr_i2c_syn.h" 

u32 Slave_Offset;

t_I2C_Configure config;

/****************************************************************************/
/*	SPEAR_I2C_ConfigureUpdate					    */	
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine Sets the Configuration of I2C   Device.                */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - Received_Config: Pointer to Configuration Information.            */
/*                                                                          */
/* OUT : none                                                               */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/


void SPEAR_I2C_ConfigureUpdate(t_I2C_Configure *Received_Config)
{
	
	config.SlaveAddress = Received_Config->SlaveAddress; 
	config.slave_offset = Received_Config->slave_offset; 
	config. mode        = Received_Config-> mode ;
	config.operation    = Received_Config->operation ;
	config.tar_update   = Received_Config->tar_update ;
	config.FirstByte    = Received_Config->FirstByte ;
	config.TransferMode = Received_Config->TransferMode; 
	config.Speed 	    = Received_Config->Speed;	
	Slave_Offset 	    = Received_Config->slave_offset;  

}
EXPORT_SYMBOL(SPEAR_I2C_ConfigureUpdate);
/****************************************************************************/
/* NAME: 		SPEAR_I2C_Receive	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start receive x number of bytes.	It calls    */
/*      I2C_ReceiveBuffer of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - NoOfBytes: This is the number of characters to be Received.   */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the buffer where the data is stored*/
/*                                                                          */
/* RETURN:                                                                  */
/*        size_t                                                             */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/
size_t
SPEAR_I2C_Receive(unsigned char *RXBuffer,u32 NoOfBytes)
{
	size_t Bytes_Copied=0;
	u32 slave_offset;
	int i=0;
	
	slave_offset = Slave_Offset;
	printk("slave_offset in driver recieve func is %d\n",slave_offset);
	Bytes_Copied = I2C_ReceiveBuffer(&config,RXBuffer,NoOfBytes,slave_offset );

	return Bytes_Copied;	
}

EXPORT_SYMBOL(SPEAR_I2C_Receive);
/****************************************************************************/
/* NAME: 		SPEAR_I2C_Transmit	                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine allows to start transmit x number of bytes. It calls   */
/*      I2C_TransmitBuffer of HAL Layer				            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*      - NoOfBytes: This is the number of characters to be transmitted.   */
/*                                                                          */
/* OUT  :                                                                   */
/*      - Buffer: This is the pointer to the characters to be transmitted.  */
/*                                                                          */
/* RETURN:                                                                  */
/*        size_t                                                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: NA                                                           */
/****************************************************************************/

size_t
SPEAR_I2C_Transmit(unsigned char *TXBuffer,u32 NoOfBytes)
{
	size_t Bytes_Written;
	u32 slave_offset;
	int i=0;
	slave_offset = Slave_Offset;
	
	printk("slave_offset in driver Transmit func is %d\n",slave_offset);
	Bytes_Written = I2C_TransmitBuffer(&config,TXBuffer,NoOfBytes,slave_offset );
	
	return Bytes_Written;	
}	

EXPORT_SYMBOL(SPEAR_I2C_Transmit);
/****************************************************************************************/
/* NAME:           SPEAR_I2C_Write					    		*/
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
SPEAR_I2C_Write(struct file *fd, const char __user *Buf, size_t len, loff_t *f_pos) 
{
	size_t Bytes_Written;

	dev.TxBuffer =( unsigned char *)kmalloc(len,GFP_ATOMIC);
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_from_user(dev.TxBuffer,(u8 *)Buf,len))
	{
		return -1 ;	//in case of error
		
	}
	
	//Bytes_Written = I2C_TransmitBuffer(&config,dev.TxBuffer,len,slave_offset );
	Bytes_Written = SPEAR_I2C_Transmit(dev.TxBuffer,len);
		
	kfree(dev.TxBuffer);
	return Bytes_Written;	//in case of error

}

/****************************************************************************************/
/* NAME:           SPEAR_I2C__Read					    		*/
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
SPEAR_I2C_Read(struct file *fd, char __user *Buf, size_t len, loff_t *f_pos) 
{
	size_t Bytes_Copied;

	dev.RxBuffer =( unsigned char *)kmalloc(len,GFP_ATOMIC);
	
	Bytes_Copied = SPEAR_I2C_Receive(dev.RxBuffer,len);

	
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_to_user((u8 *)Buf,dev.RxBuffer,Bytes_Copied))
	{
		return -1;
	}
	
	kfree(dev.RxBuffer);
	return Bytes_Copied;	

}

/****************************************************************************************/
/* NAME:           SPEAR_I2C_Ioctl					    		*/
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
static int SPEAR_I2C_Ioctl(struct inode *inode, struct file *file, uint cmd, unsigned long *Buf)
{
	t_I2C_Configure configure;
	t_I2CError Status = I2C_OK ;
	
	//Copying Configuration Information from User Space to Kernel Space.
	if(copy_from_user(&configure,(struct t_I2C_Configure *)Buf, sizeof(configure))!=I2C_OK)
	
	{
		Status = EFAULT;
		goto out;
	}

	//Checking if Config Command is called.
	if(cmd == I2CIOC_CONFIG)
	{
		//Configuring I2C Device.
		 SPEAR_I2C_ConfigureUpdate(&configure);
	}
out:
	return Status;
}

/********************************************************************************/
/* NAME:           SPEAR_I2C_Open			                        */
/*------------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             	*/
/*      This routine opens the char device for I2C          		        */
/*                                                                          	*/
/* PARAMETERS:                                                              	*/
/* IN  :      inode:  for accessing file				        */
/*            fd :file opearation pointer		                        */
/* OUT :                                                                    	*/
/*                                                                          	*/
/* RETURN:     0 = SUCCESS		                                        */
/*             1 = FAILURE                                                  	*/
/*------------------------------------------------------------------------------*/

static int SPEAR_I2C_Open(struct inode *inode, struct file *fd)
{
	unsigned long		flags;
	int status = I2C_FAIL;
	

	spin_lock_irqsave (&dev.i2c_lock, flags);

	if (!dev.i2c_cdev_open) {
		dev.i2c_cdev_open = 1;
		fd->private_data = &dev;
		status = 0;
	}

	spin_unlock_irqrestore (&dev.i2c_lock, flags);

	DEBUG (dev, "SPEAR_I2C_Open returned %x\n", status);

	return status;
}

/****************************************************************************/
/* NAME:           SPEAR_I2C_Release                 	                     */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*      This routine releases the char device for i2c	                    */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file			            */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    */
/*             1 = FAILURE                                                  */
/*--------------------------------------------------------------------------*/
static int SPEAR_I2C_Release(struct inode *inode, struct file *file)
{
	unsigned long flags;       
     

	spin_lock_irqsave(&dev.i2c_lock, flags);

	dev.i2c_cdev_open = 0;

	spin_unlock_irqrestore(&dev.i2c_lock, flags);

	DEBUG(dev, "i2c_close\n");

	return 0;
}

static struct file_operations SPEAR_I2C_fops = 
{
	.owner 	= THIS_MODULE,
	.read 	= SPEAR_I2C_Read,
	.write 	= SPEAR_I2C_Write,
	.open 	= SPEAR_I2C_Open,
	.release= SPEAR_I2C_Release,
	.ioctl 	= SPEAR_I2C_Ioctl
};

/****************************************************************************/
/* NAME:      __exit SPEAR_I2C_Cleanup (void)	         		    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when I2C module is unloaded                        */
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

static void 
SPEAR_I2C_cleanup(void)
{
	cdev_del(&dev.i2c_cdev) ;
	spin_lock(&dev.i2c_lock);
	unregister_chrdev_region(g_I2C_devno, 1);

	spin_unlock(&dev.i2c_lock);
}

module_exit(SPEAR_I2C_cleanup);

/****************************************************************************/
/* NAME:  			SPEAR_I2C_Init 				    */
/*                                                			    */  
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:                                                             */
/*        initializes the I2C                                               */
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

static int __init SPEAR_I2C_Init(void)
{
	int status;
//	struct i2c_device dev;
	
	status = register_chrdev_region(g_I2C_devno, 1, "I2C");
	if (status)
	{
		printk( "register_chrdev_region %x\n", status);
		return status;
	}
	cdev_init(&dev.i2c_cdev, &SPEAR_I2C_fops);
	dev.i2c_cdev.owner = THIS_MODULE;

	status = cdev_add(&dev.i2c_cdev, g_I2C_devno, 1);
	if (status)
	{
		//If Error occurs while adding a cdev instance then exit.
		printk("***unable to register character device***\n");
		goto fail;
	}
	
	dev.TxBuffer = NULL;
	dev.RxBuffer = NULL;
	spin_lock_init(&dev.i2c_lock);

	//Initializing I2C Devices with the default settings.
	I2C_Init();


	return status;
fail:
		SPEAR_I2C_cleanup();
		return -1;	
}
EXPORT_SYMBOL(SPEAR_I2C_Init);

module_init(SPEAR_I2C_Init);



MODULE_AUTHOR("Rajeev Kumar");
MODULE_DESCRIPTION("SPEAR I2C  driver");
MODULE_SUPPORTED_DEVICE("/dev/i2c");
MODULE_LICENSE("GPL");

