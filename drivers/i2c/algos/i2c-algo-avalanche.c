/*
 *
 * i2c-along-avalanche.c
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

/** \file   i2c-algo-avalanche.c
    \brief  IIC algorithm code for Avalanche 
	
	FOR DOCUMENTATION OF FUNCTIONS: Refer file i2c-algo-avalanche.h
    \author     PSP TII
    \version    0.1
 */

#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/ioport.h>
#include <linux/errno.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-avalanche.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include "../busses/i2c-avalanche.h"

#ifdef CONFIG_I2C_DEBUG_ALGO
#define DEBUG_IIC_ALGO(fmt,arg...)  printk(fmt , ##arg)
#else
#define DEBUG_IIC_ALGO(fmt,arg...)
#endif

/* IIC Master Registers */
#define I2C_DATA_REG_HIGH            0x0
#define I2C_DATA_REG_LOW             0x4
#define I2C_CONFIGURATION            0x8
#define I2C_DATA_REGISTER            0xc
#define I2C_CLOCK_DIVIDER            0x10

/* Define the register masks for data high register.*/
#define I2C_READ_MODE_ENABLE         0x0300
#define I2C_WRITE_MODE_ENABLE        0x0200
#define I2C_END_BURST                0x0400

/*Define the register masks for I2C_CONFIGURATION register. */
#define I2C_SPIKE_FILTER_MASK        0x7000
#define I2C_INTERRUPT_ENABLE         0x0001
#define I2C_SPIKE_FILTER_BIT_POS     12

/* Define the register masks for read data register.*/
#define I2C_BUS_ERROR                0x8000
#define I2C_TRANSFER_COMPLETE        0x4000
/* This bit is high means that device is busy*/
#define I2C_WRITE_NOT_ALLOWED        0x2000  
#define I2C_MAX_RETRY_COUNT          0xFFFF
#define I2C_COMBINED_XFER    	     1
#define I2C_POLL_MILLI_SEC			 2	
/*for ioctl*/
#define AVALANCHE_I2C_SETCLK         0
#define AVALANCHE_I2C_GETCLK         1


#define get_clock(adap) adap->getclock(adap->data)
#define iic_out_word( reg, val) adap->setiic( reg, val)
#define iic_in_word( reg) adap->getiic( reg )

#if defined (CONFIG_MACH_PUMA5)
/* globals */
extern unsigned int avalanche_get_vbus_freq(void);
#endif

/* function definitions */
void iic_stop( struct i2c_algo_iic_data *adap )
{
    volatile unsigned int temp = 0;
    struct iic_avalanche *adap_priv_data = (struct iic_avalanche *)adap->data;
    DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
    temp = iic_in_word((unsigned long)( adap_priv_data->iic_base + 
                       I2C_DATA_REG_HIGH));
    iic_out_word((unsigned long) ( adap_priv_data->iic_base + 
                 I2C_DATA_REG_HIGH ), temp | I2C_END_BURST);
}

static int wait_for_bb( struct i2c_algo_iic_data *adap )
{
    struct iic_avalanche *adap_priv_data = (struct iic_avalanche *)adap->data;
    volatile unsigned int status = 0;
	unsigned long timeout = 0;
    unsigned int retry = 0;

i2c_retry:
   retry++;

	timeout = jiffies + msecs_to_jiffies(I2C_POLL_MILLI_SEC);
    do
    {
    	status = iic_in_word((unsigned long) (adap_priv_data->iic_base + 
                             I2C_DATA_REGISTER ));
		if(!( status & I2C_WRITE_NOT_ALLOWED ))
			return 0;
	}while(time_after(timeout, jiffies)) ;
    
    printk("***************************************************************************************\n");
    printk(" Timeout occured in function %s Try to recover (writing to data_low reg)\n",__FUNCTION__);
    printk("***************************************************************************************\n");
    
    /* Dummy write for WA */
    iic_out_word((unsigned long)(adap_priv_data->iic_base + I2C_DATA_REG_LOW),0xff);
    if (retry < 3)
    {
        goto i2c_retry;
    }

	printk(" Timeout occured in function %s\n",__FUNCTION__);
	return AVALANCHE_I2C_ERROR;
}


/* After issuing a transaction on the I2C bus, this function is called. 
 * In interrupt mode It puts this process to sleep until we get an interrupt
 * from the controller telling us that the transaction requested in complete.
 * In polling mode it checks for transfer complete bit of read data register.
 */

static int wait_for_pin( struct i2c_algo_iic_data *adap, int *status )
{
    struct iic_avalanche *adap_priv_data = (struct iic_avalanche *)adap->data;
    volatile unsigned int complete = 0;
	unsigned long timeout = 0;

    DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);

	if(( (! adap_priv_data->polling_mode) && (adap_priv_data->iic_irq > 0) ))
	{
		/* interrupt mode, wait for interrupt */
       	adap->waitforpin((void*)status);
	}
	else
	{
		timeout = jiffies + msecs_to_jiffies(I2C_POLL_MILLI_SEC);
	    do
		{
	 	    complete = iic_in_word((unsigned long) (adap_priv_data->iic_base +
    	  	                       I2C_DATA_REGISTER));
			if(complete & I2C_TRANSFER_COMPLETE)
				goto i2c_comp;
	    }while(time_after(timeout, jiffies));
	
		printk(" Timeout occured in function %s\n",__FUNCTION__);
		*status = AVALANCHE_I2C_ERROR;
		return AVALANCHE_I2C_ERROR;
	}
i2c_comp:
	*status = wait_for_bb(adap);
    return 0;
}


void reg_dump(unsigned long iic_base)
{
#ifdef I2C_DEBUG_ALGO    
    printk("\n--------------------------------------------------------\n");
    printk("data reg high = 0x%04X\n",*(unsigned int*)
                                      (iic_base + I2C_DATA_REG_HIGH));
    printk("data reg low  = 0x%04X\n",*(unsigned int*)
                                      (iic_base + I2C_DATA_REG_LOW));
    printk("configuration = 0x%04X\n",*(unsigned int*) 
                                      (iic_base + I2C_CONFIGURATION));
    printk("clock_divider = 0x%04X\n",*(unsigned int*) 
                                      (iic_base + I2C_CLOCK_DIVIDER));
    printk("data_register = 0x%04X\n",*(unsigned int*) 
                                      (iic_base + I2C_DATA_REGISTER));
    printk("--------------------------------------------------------\n");
#endif

}

static int iic_init( struct i2c_algo_iic_data *adap )
{
    struct iic_avalanche *adap_priv_data = NULL; 
    volatile unsigned int temp = 0;
    int clk_divider = 0;
    int iic_clock = 0; 
    int vbus_clock = 0;

	/* initialize i2c */
	DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
    if(( adap != NULL )&& ( adap->data != NULL ) )
    {
        DEBUG_IIC_ALGO(" Assigning the adap_priv_data in function %s\n", \
                         __FUNCTION__);
        adap_priv_data = adap->data;
        iic_clock = adap_priv_data->clock;
    }
    else 
    {	
        printk(" Received NULL argument in function %s Exiting\n",__FUNCTION__);
        return AVALANCHE_I2C_ERROR;
    }

    /* Check  bus busy, wait for the write_allowed bit to become low. 
     * The I2C module cannot be configured till this bit is low. After
     * retrying for 9 clock cycles,the h/w will clear the write allowed
     * bit and will also set the error bit.
     */

    DEBUG_IIC_ALGO("check the bus status \n");
	if( wait_for_bb(adap) != 0 )
	{
		 printk("Device found busy Exiting initializatio\n");
         printk(" Initialization failed \n");
         return AVALANCHE_I2C_ERROR;
	}

    DEBUG_IIC_ALGO("enabling interrupt bit in Master \n");
	/* enable interrupt bit always */
    temp = iic_in_word((unsigned long)(adap_priv_data->iic_base + 
        	               I2C_CONFIGURATION ));
    iic_out_word((unsigned long) (adap_priv_data->iic_base + 
    	              I2C_CONFIGURATION),temp |I2C_INTERRUPT_ENABLE);

    /* Set spike filter value */
    DEBUG_IIC_ALGO("Setting spike filter value\n");
	/* Reset the spike filter val */
    temp = iic_in_word((unsigned long)(adap_priv_data->iic_base + I2C_CONFIGURATION ));
    iic_out_word( (unsigned long) ( adap_priv_data->iic_base + I2C_CONFIGURATION), 
               temp & (~I2C_SPIKE_FILTER_MASK) );
	/* update new spike filter val */
    temp = iic_in_word((unsigned long)(adap_priv_data->iic_base + I2C_CONFIGURATION ));
    iic_out_word( (unsigned long) ( adap_priv_data->iic_base + I2C_CONFIGURATION), 
               temp |(adap_priv_data->spike_filter) << I2C_SPIKE_FILTER_BIT_POS);

	/* calculate the clock divider */
#if defined (CONFIG_MACH_PUMA5)
	vbus_clock = avalanche_get_vbus_freq();
#else /* CONFIG_MACH_PUMA6  For Puma-6 SoC */
    vbus_clock =  PAL_sysClkcGetFreq(PAL_SYS_CLKC_I2C);
#endif
    if(vbus_clock == 0) 
    {
#ifdef CONFIG_MACH_PUMA5_VOLCANO
        vbus_clock = 10000000;
#else
        vbus_clock = 60000000;
#endif
    }
    clk_divider = vbus_clock/iic_clock;

    /* set the clock divider register */
    iic_out_word(adap_priv_data->iic_base+I2C_CLOCK_DIVIDER,clk_divider);
	
	DEBUG_IIC_ALGO("Register values after Init is \n");
	reg_dump(adap_priv_data->iic_base);

	return 0;
}

/* Utility functions */
int iic_sendbytes( struct i2c_adapter *i2c_adap,const char *buf,
                   int count,int xfer_type )
{

    struct i2c_algo_iic_data *adap = i2c_adap->algo_data;
    struct iic_avalanche *adap_priv_data =adap->data;
    int status = 0;
    int wrcount;
	for( wrcount = 0; wrcount <count; wrcount++ )
    {
        iic_out_word((unsigned long)(adap_priv_data->iic_base + 
   	                 I2C_DATA_REG_LOW), buf[wrcount]);
       	DEBUG_IIC_ALGO("-----------------I2C data written to I2C_DATA_REG_LOW \
                        is =%x --------------\n", buf[wrcount]);

        wait_for_pin(adap, &status);
   	    if( status < 0 ) 
       	{
           	printk(" ERROR IN FUNCTION %s status =0x%x\n",__FUNCTION__,status);
            reg_dump(adap_priv_data->iic_base);
			break;
		}
	}
	return status;
}


int iic_readbytes( struct i2c_adapter *i2c_adap, char *buf, int count,
                   int xfer_type)
{
    struct i2c_algo_iic_data *adap = i2c_adap->algo_data;
    struct iic_avalanche *adap_priv_data =adap->data;
	volatile unsigned int data = 0;
    int status = 0;
    int rdcount;

    DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
    for( rdcount = 0; rdcount <count; rdcount++ )
    {
		/* Dummy write for read */
      	iic_out_word((unsigned long)(adap_priv_data->iic_base + 
           	         I2C_DATA_REG_LOW),0xff );

        wait_for_pin(adap, &status);
	    if( status < 0 )
   	   	{
       	   	reg_dump(adap_priv_data->iic_base);
           	printk(" ERROR IN FUNCTION %s\n",__FUNCTION__);
			break;
		}
		data = iic_in_word((unsigned long)(adap_priv_data->iic_base + 
							I2C_DATA_REGISTER ));
   		buf[rdcount] = 0xff & data;
        DEBUG_IIC_ALGO("i2c data = %x read\n",buf[rdcount]);
    }
    return status;
}

/* Whenever we initiate a transaction, the first byte clocked
 * onto the bus after the start condition is the address (7 bit) of the
 * device we want to talk to.  This function manipulates the address specified
 * so that it makes sense to the hardware when written to the I2C peripheral.
 * Note: 10 BIT ADDRESSES ARE NOT SUPPORTED hardware.
 */
static int avalanche_iic_doAddress( struct i2c_algo_iic_data *adap,
                                    struct i2c_msg *msg, int retries)
{
    struct iic_avalanche *adap_priv_data =adap->data;
    unsigned int addr;

    DEBUG_IIC_ALGO("\n In functin %s with the address %x for read/write = 1/0 \
                     %d\n",__FUNCTION__, msg->addr, msg->flags);

    /* Ten bit addresses not supported */
    if ( msg->flags & I2C_M_TEN ) 
    {
        printk("\nTen bit address not supported by Hardware\n");
		return AVALANCHE_I2C_ERROR;
    }
    else
    {
        addr = msg->addr;
        if( msg->len )
        {
            if( msg->flags & I2C_M_RD )
			{
           	    addr |= ( I2C_READ_MODE_ENABLE );
			}
			else
			{
                addr |= ( I2C_WRITE_MODE_ENABLE );
			}			
        }
        else 
        {
           /* For emulating 0 byte transfer its always safe to read */
            addr |= ( I2C_READ_MODE_ENABLE );
            DEBUG_IIC_ALGO("Message length is zero\n");
        }

        DEBUG_IIC_ALGO(" writing the address %x to the Data High register \
                    in function %s\n",addr,__FUNCTION__);

        iic_out_word((unsigned long)adap_priv_data->iic_base + 
                      I2C_DATA_REG_HIGH, addr);
    }
    return 0;
}


/* Prepares the controller for a transaction (clearing status
 * registers, data buffers, etc), and then calls either iic_readbytes or
 * iic_sendbytes to do the actual transaction.
 *
 * Before we issue a transaction, we should
 * verify that the bus is not busy or in some unknown state.
 */
static int iic_xfer( struct i2c_adapter *i2c_adap, struct i2c_msg msgs[],
                     int num)
{
    struct i2c_algo_iic_data *adap = i2c_adap->algo_data;
    struct iic_avalanche *adap_priv_data = ( struct iic_avalanche*)adap->data;
    struct i2c_msg *pmsg;
    int msg = 0;
    int status = 0;
	
	mutex_lock(&(adap_priv_data->avalanche_i2c_lock));
    for( msg = 0; msg < num; msg++ )
    {
	     wait_for_bb(adap);
    	if( wait_for_bb(adap) < 0 ) 
	    {
    	    printk("iic_xfer: Timeout waiting for host not busy In function %s\n", __FUNCTION__);
			status = -EIO;
			goto i2c_avalanche_algo_term;
		}
        pmsg = &msgs[msg];
        /* Load address */
        status = avalanche_iic_doAddress(adap, pmsg, i2c_adap->retries);

        if( status < 0  )
        {
			status = -EIO;
			goto i2c_avalanche_algo_term;
        }
        if( pmsg->flags & I2C_M_RD ) 
        {
            DEBUG_IIC_ALGO( " This one is a read calling iic_readbytes function\n");
            status = iic_readbytes( i2c_adap, pmsg->buf, pmsg->len, 
                                    I2C_COMBINED_XFER);
        	if(status < 0)
            {
                DEBUG_IIC_ALGO("Read Failed status \n");    
    		   	iic_stop(adap);
                break;
            }
        }
        else if(!( pmsg->flags & I2C_M_RD ) )
        {
            DEBUG_IIC_ALGO("This one is a write calling iic_sendbytes function\n");
            status = iic_sendbytes( i2c_adap, pmsg->buf, pmsg->len, 
                                    I2C_COMBINED_XFER);
            if( status < 0 )
            {
               DEBUG_IIC_ALGO("Write Failed status \n");    
    		   iic_stop(adap);
               break;
            }
        }
        iic_stop(adap);
    }
i2c_avalanche_algo_term:
	mutex_unlock(&(adap_priv_data->avalanche_i2c_lock));
	return (status)?(AVALANCHE_I2C_ERROR):msg;
}



static unsigned int iic_func(struct i2c_adapter *adap)
{
    DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
    return( I2C_FUNC_I2C | I2C_FUNC_SMBUS_EMUL );
}


static struct i2c_algorithm iic_algo = {
    .master_xfer = iic_xfer,    /* master_xfer	 */
    .smbus_xfer  = NULL,        /* smbus_xfer	 */
    .functionality  = iic_func, /* functionality */
};

/* Registering functions to load algorithms at runtime */
int i2c_avalanche_add_bus(struct i2c_adapter *adap)
{
    struct i2c_algo_iic_data *iic_adap = adap->algo_data;
	DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
	/* register new adapter to i2c module... */
	adap->retries = I2C_MAX_RETRY_COUNT ;
	adap->timeout = I2C_MAX_RETRY_COUNT;
	adap->algo    = &iic_algo;
	if( iic_init(iic_adap) != 0)
    {
        return AVALANCHE_I2C_ERROR;
    }
	if( i2c_add_adapter(adap) != 0)
    {
        return AVALANCHE_I2C_ERROR;
    }
    return 0;
}


int i2c_avalanche_del_bus(struct i2c_adapter *adap)
{
    DEBUG_IIC_ALGO(" In function %s\n", __FUNCTION__);
    i2c_del_adapter(adap);
    DEBUG_IIC_ALGO("In function %s i2c-algo-ti: adapter \
               unregistered: %s\n",__FUNCTION__,adap->name);
    return 0;
}

EXPORT_SYMBOL(i2c_avalanche_add_bus);
EXPORT_SYMBOL(i2c_avalanche_del_bus);
MODULE_LICENSE("GPL");

