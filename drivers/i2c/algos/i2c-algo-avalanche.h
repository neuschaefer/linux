/*
 *
 * i2c-algo-avalanche.h
 * Description:
 * Defines of the Avalanche IIC algo priveate data etc.
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


#ifndef AVALANCHE_I2C_ALGO_H
#define AVALANCHE_I2C_ALGO_H


struct i2c_algo_iic_data {
    void *data;     /* private data for lowlevel routines   */
    void (*setiic)(unsigned long reg_addr, unsigned int val);
    unsigned int(*getiic)(unsigned long reg_addr);
    int  (*getown)( void *data );
    int  (*getclock)(void *data );
    void (*waitforpin)(void *data );
    /* local settings */
    int udelay;
    int mdelay;
    int timeout;
};

int i2c_avalanche_add_bus(struct i2c_adapter *);
int i2c_avalanche_del_bus(struct i2c_adapter *);

#endif /* AVALANCHE_I2C_ALGO_H */

