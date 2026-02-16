/*
 *
 * i2c-avalanche.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/


/** \file   i2c-avalanche.c
    \brief  i2c driver for TI IIC adaptiors

    FOR DOCUMENTATION OF FUNCTIONS: Refer file i2c-avalanche.h
    \author     PSP TII
    \version    0.1
 */

#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/i2c.h>
#include<linux/i2c-algo-avalanche.h>
#include "i2c-avalanche.h"

#if defined(CONFIG_I2C_DEBUG_BUS)
#define DEBUG_IIC_BUS(fmt,arg...)  printk(fmt , ##arg)
#else
#define DEBUG_IIC_BUS(fmt,arg...)
#endif

#if defined (CONFIG_MACH_PUMA5)
/* global defines */
extern unsigned int avalanche_get_vbus_freq(void);
#endif

static int clock = CONFIG_I2C_AVALANCHE_CLOCK;
static int polling_mode = CONFIG_I2C_AVALANCHE_FORCE_POLLED_MODE;
static int spike_filter = CONFIG_I2C_AVALANCHE_SPIKE_FILTER;


static struct iic_avalanche iic_avalanche_priv_data =  
{
    AVALANCHE_IIC_REGS_BASE,      /* Base address */	
    LNXINTNUM(AVALANCHE_I2C_INT), /* Interrupt*/
    0,	        	              /* Our address on the i2c bus */
	CONFIG_I2C_AVALANCHE_FORCE_POLLED_MODE,
	CONFIG_I2C_AVALANCHE_CLOCK,
	CONFIG_I2C_AVALANCHE_SPIKE_FILTER,
	NULL,
	0,
	0,
};



static void iic_avalanche_setiic( unsigned long reg_addr,unsigned int val )
{
    *(volatile unsigned long *)reg_addr = val;
}

static unsigned int iic_avalanche_getiic( unsigned long reg_addr )
{
    volatile unsigned int val = 0;
	val = *(volatile unsigned long *)reg_addr;
    return( val );
}

static int iic_avalanche_getown(void *data )
{
    struct iic_avalanche *avalanche_priv_data =(struct iic_avalanche *)data;
    return( avalanche_priv_data->iic_own );
}

static int iic_avalanche_getclock(void *data )
{
    struct iic_avalanche *avalanche_priv_data  = (struct iic_avalanche *)data;
    return( avalanche_priv_data->clock);
}

static irqreturn_t iic_avalanche_handler( int this_irq, void *dev_id)
{
	volatile unsigned int status = 0;

    /* Read any I2C master register to clear the interrupt*/
    status = iic_avalanche_getiic((unsigned long )
    		 (iic_avalanche_priv_data.iic_base + 0xc ));	

	if(!( status & 0x4000 ) )
	{
		wake_up_interruptible(iic_avalanche_priv_data.iic_wait);
	}

    return IRQ_HANDLED;
}

static void iic_avalanche_waitforpin( void *data ) 
{
	long timeout = 0;
    /* If interrupts are enabled (which they are), then put the process to
     * sleep.  This process will be awakened by two events -- either the
     * the I2C peripheral interrupts or the timeout expires. (timeout is set to 50msec)
     */
	timeout = interruptible_sleep_on_timeout(iic_avalanche_priv_data.iic_wait,
                                             (5 * (HZ/100)));
	if(timeout == 0)
	{
		DEBUG_IIC_BUS("No I2C interrupt for a long period (timeout \
                       = %d jiffies)\n", (int)timeout);
	}
    *(int*)data = (timeout)?0:(AVALANCHE_I2C_ERROR);
}

/* Request our interrupt line and register its associated handler. */
static int iic_hw_resrc_init(void)
{
    unsigned long flags =0;
	if(! iic_avalanche_priv_data.polling_mode )
	{
		if( iic_avalanche_priv_data.iic_irq > 0 )
    	{
	       if( request_irq( iic_avalanche_priv_data.iic_irq,
                           iic_avalanche_handler, flags, "TI IIC", NULL ) < 0 )
	       {
				printk("ERROR request_irq failed in iic_hw_resrc_init\n" );
				return AVALANCHE_I2C_ERROR;
		   }
    	   else 
	       {
    	        DEBUG_IIC_BUS("Enabled IIC IRQ %d\n", \
                               iic_avalanche_priv_data.iic_irq);
	       }
    	}
	}
	return 0;
}
void iic_avalanche_release(void)
{
    if(!iic_avalanche_priv_data.polling_mode)
	{	
	    if( iic_avalanche_priv_data.iic_irq > 0 )
    	{
        	free_irq( iic_avalanche_priv_data.iic_irq, 0 );
		}
	}
}

static struct i2c_algo_iic_data iic_avalanche_data = {
    NULL,
    iic_avalanche_setiic,
    iic_avalanche_getiic,
    iic_avalanche_getown,
    iic_avalanche_getclock,
    iic_avalanche_waitforpin,
	80,    /* udelay waits */
    80,    /* mdelay waits */
    100,   /* timeout      */
};


static struct i2c_adapter iic_avalanche_adapter = {
	.owner     = THIS_MODULE,
	.class     = I2C_CLASS_ALL, 
	.algo      = NULL,           
	.algo_data = &iic_avalanche_data,
	.name      = "TI IIC adapter",
};


/* Called when the module is loaded.  This function starts the
 * cascade of calls up through the heirarchy of i2c modules
 * (i.e. up to the algorithm layer and into to the core layer)
 */
static int iic_avalanche_init( void )
{
	int ret = 0;

    DEBUG_IIC_BUS("Initialize Avalanche IIC adapter module\n");

	iic_avalanche_priv_data.clock			= clock;
	iic_avalanche_priv_data.polling_mode	= polling_mode;
	iic_avalanche_priv_data.spike_filter	= spike_filter;

	DEBUG_IIC_BUS("values of the parameters is \n clock = %d\n polling_mode"
				" = %d\n spike_filter = %d\n",iic_avalanche_priv_data.clock,
     			iic_avalanche_priv_data.polling_mode,
                iic_avalanche_priv_data.spike_filter);

	/* Validate mode of the Driver */
	if( iic_avalanche_priv_data. polling_mode != 0 && 
		iic_avalanche_priv_data.polling_mode != 1 )
	{
        printk("Invalid mode defaulting to Interrupt mode\n");
 	    iic_avalanche_priv_data.polling_mode = 0; 
	}
    /* Validate clock frequency */ 
   	if( iic_avalanche_priv_data.clock < AVALANCHE_I2C_MIN_CLOCK || 
        iic_avalanche_priv_data.clock > AVALANCHE_I2C_MAX_CLOCK )  
    {
        printk("Invalid clock defaulting to 400KHz\n");
        iic_avalanche_priv_data.clock = AVALANCHE_I2C_MAX_CLOCK;
    }
    /* Validate spike_filter */ 
   	if(	iic_avalanche_priv_data.spike_filter < 0 ||
		iic_avalanche_priv_data.spike_filter > 7  )
	{
        printk("Invalid spike filter defaulting to 0\n");
 	    iic_avalanche_priv_data.spike_filter = 0; 
	}
	iic_avalanche_priv_data.iic_wait = kmalloc( sizeof(wait_queue_head_t),
												GFP_KERNEL);

	if(iic_avalanche_priv_data.iic_wait == NULL) 
	{
		printk("TI I2C Bus : kmalloc failed\n");
		return -ENOMEM;	
	}
	else
	{
	    init_waitqueue_head(iic_avalanche_priv_data.iic_wait);
	}
	/*initialize the data lock */
	mutex_init(&(iic_avalanche_priv_data.avalanche_i2c_lock));

	iic_avalanche_data.data = (void *)&iic_avalanche_priv_data;

    if( iic_hw_resrc_init() == 0 )
    {
        DEBUG_IIC_BUS("calling add bus\n");
        if( i2c_avalanche_add_bus(&iic_avalanche_adapter) < 0)
        {
			ret = -ENODEV;
			goto i2c_avalanche_bus_fail;
        }
	}  
    else 
    {
		ret = -ENODEV;
		goto i2c_avalanche_bus_fail;
	}
    //DEBUG_IIC_BUS("Found device at %#x irq %d.\n",
    //         (unsigned int)iic_avalanche_priv_data.iic_base,(unsigned int)iic_avalanche_priv_data.iic_irq);
	return ret;

i2c_avalanche_bus_fail:
	iic_avalanche_release();
	kfree(iic_avalanche_priv_data.iic_wait);
	return ret;
}

static void iic_avalanche_exit( void )
{
    i2c_del_adapter(&iic_avalanche_adapter);
    iic_avalanche_release();
	kfree(iic_avalanche_priv_data.iic_wait);
}

module_init(iic_avalanche_init);
module_exit(iic_avalanche_exit);


MODULE_AUTHOR("Texas Instruments India");
MODULE_DESCRIPTION("TI I2C bus adapter");
MODULE_LICENSE("GPL");

module_param(spike_filter,int,S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(spike_filter,
                 " \n\t\tFor noisy environments, selectable spike filtering is available for I2C data and clock signals.The spike filter evaluates the incoming signal for a selected number of chip clocks Signal stability can be evaluated for up to eight chip clock cycles <Range: 0 to 7>");

module_param(polling_mode,int,S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(clock,
                 "\n\t\tSet I2C Clock frequency in Hz: 100KHz (Standard Mode) or 400KHz(Fast Mode) <Range: 1000 to 400000>");

module_param(clock,int,S_IRUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(polling_mode,
                 "\n\t\tSet Polling_mode=1 for  polling mode, for interrupt mode set polling_mode=0 <Range:0/1> ");
