/*
 *
 * puma5_spi.h 
 * Description:
 * puma5 spi platform data
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
#ifndef __PUMA5_SPI__

#define __PUMA5_SPI___ 

#define AVALANCHE_SPI_BUS_NUM				(0)
#define AVALANCHE_SPI_MAX_CHIPSELECT		(4)
#define AVALANCHE_DEFAULT_SYS_CLK			(8*1000*1000)


#ifdef CONFIG_SPI_TI_CODEC
#define AVALANCHE_CODEC_SPI_BUS_NUM			(1)
#define AVALANCHE_CODEC_SPI_MAX_CHIPSELECT	(2)

#define CODEC_SPI_CS_SEL_REG 				((volatile UINT32 *)(IO_BASE + 0x08611B3C))
#define TI_CODEC_SPI_DEF_SPCR1				(0x81080001)
#endif /* CONFIG_SPI_TI_CODEC */


#endif /*__PUMA5_SPI___ */      
