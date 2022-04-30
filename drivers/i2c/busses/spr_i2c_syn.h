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


#ifndef SPEAR_I2C_SYN_H
#define SPEAR_I2C_SYN_H

#include <asm/arch/spear.h>
#include <asm/arch/hardware.h>
#include <linux/types.h>
#include <asm/arch/hcl_defs.h>
#include "spr_i2c_syn_hal.h"

#define SPEAR_I2C_BASE 'S'
#define I2CIOC_CONFIG _IOWR(SPEAR_I2C_BASE,1,t_I2C_Configure)

static const char driver_name[] = "SPEARPlus_I2C";
static const int g_I2C_devno = MKDEV(240,0);


#ifdef DEBUG
#undef DEBUG
#define DEBUG(dev,fmt,args...) \
	xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define DEBUG(dev,fmt,args...) \
	do { } while (0)
#endif /* DEBUG */


struct i2c_device
{
	spinlock_t i2c_lock;
	struct cdev i2c_cdev;
	u8	i2c_cdev_open;
	u8	*TxBuffer;
	u8 	*RxBuffer; 
};

static	struct i2c_device dev;
#define MAX_BUFSIZE 512

void SPEAR_I2C_ConfigureUpdate(t_I2C_Configure *Received_Config);
size_t SPEAR_I2C_Receive(unsigned char  *RXBuffer,u32 NoOfBytes);
size_t SPEAR_I2C_Transmit(unsigned char *TXBuffer,u32 NoOfBytes);




 #endif //SPEAR_I2C_SYN_H
