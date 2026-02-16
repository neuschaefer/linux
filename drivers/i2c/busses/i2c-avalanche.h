/*
 *
 * i2c-avalanche.h 
 * Description:
 * Defines of the Avalanche IIC adapter data, etc.
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


#ifndef I2C_ADAP_AVALANCHE_H
#define I2C_ADAP_AVALANCHE_H

#include <linux/i2c.h>
#include <asm-arm/arch-avalanche/generic/pal.h>

#define I2C_HW_AVALANCHE_ID             160
#define AVALANCHE_I2C_MIN_CLOCK         1000
#define AVALANCHE_I2C_MAX_CLOCK         400000

#define AVALANCHE_I2C_ERROR            -1

/* This struct contains the hw-dependent values of TI  IIC  */

struct iic_avalanche {
    unsigned long iic_base;
    int iic_irq;
    int iic_own;
	int polling_mode;
	int clock;
	int spike_filter;
	wait_queue_head_t *iic_wait;
    int transfer_complete;
    unsigned int status;
	struct mutex avalanche_i2c_lock; 
};

typedef struct iic_avalanche iic_avalanche_t;
#endif /* I2C_ADAP_AVALANCHE_H */
