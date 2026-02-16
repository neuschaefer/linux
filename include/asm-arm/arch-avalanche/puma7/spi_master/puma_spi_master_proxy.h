/*
 *
 * puma_spi_master_proxy.h
 * Description:
 * Puma SPI proxy ioctl declaration.
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
#ifndef _PUMA_SPI_MASTER_PROXY_H
#define _PUMA_SPI_MASTER_PROXY_H

#define PUMA_SPI_MASTER_MAJOR 153


struct puma_spi_ioctl_transfer {
    const unsigned char *tx_buf;
    unsigned char *rx_buf;
    unsigned len;
};


#define SPI_IOCTL_MAGIC         'I'
#define PUMA_SPI_IOCTL_TRANSFER _IOW(SPI_IOCTL_MAGIC, 0, struct puma_spi_ioctl_transfer)

#endif
