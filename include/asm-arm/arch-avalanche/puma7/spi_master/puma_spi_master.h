/*
 *
 * puma_spi_master.h
 * Description:
 * SPI utility header.
 *
 *
 * GPL LICENSE SUMMARY
 *
 *  Copyright(c) 2014 Intel Corporation.
 *
 *  This program is free software; you can redistribute it and/or modify 
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 *  General Public License for more details.
 *
 *
 *  You should have received a copy of the GNU General Public License 
 *  along with this program; if not, write to the Free Software 
 *  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *  The full GNU General Public License is included in this distribution 
 *  in the file called LICENSE.GPL.
 *
 *  Contact Information:
 *  Intel Corporation
 *  2200 Mission College Blvd.
 *  Santa Clara, CA  97052
 */

#include <linux/platform_device.h>
#ifndef _PUMA_SPI_MASTER_H
#define _PUMA_SPI_MASTER_H

#define MAX_CHIPSELECT 8
#define MAX_TRANSFER 128
#define DEFAULT_SPEED_HZ 40000000

struct puma_spi_device
{
    u8 chipselect;
    unsigned speed_hz;
    u8 end_delay;
    u8 start_delay;
    u8 lsb;
    u8 tx_neg;
    u8 rx_neg;
};

int puma_alloc_spi_device(struct puma_spi_device *device);

/* Initialize SPI proxy device driver. It handles read/write/ioctl for user-mode access */
extern int puma_spi_proxy_init(void);

/* Initialize SPI /proc interface */
extern int puma_spi_proc_init(void);

int puma_spi_device_open(unsigned chipselect);
int puma_spi_device_close(unsigned chipselect);
int puma_spi_device_transfer(unsigned chipselect, const u8 *tx, u8 *rx,
                             unsigned len);

int puma_spi_master_init(struct platform_device *master_device);

#endif
