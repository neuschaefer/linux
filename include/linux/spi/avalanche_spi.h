/*
 *
 * avalanche_spi.h
 * Description:
 * avalanche sppi controller header file
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


#ifndef __AVALANCHE_SPI_H_
#define __AVALANCHE_SPI_H_

#define AVALANCHE_SPI_INIT_SPMODE	0
#define AVALANCHE_SPI_SIZE			0xFF

/* Chip selects available on the spi IP */
#define AVALANCHE_SPI_CS0			0
#define AVALANCHE_SPI_CS1			1
#define AVALANCHE_SPI_CS2			2
#define AVALANCHE_SPI_CS3			3

#define AVALANCHE_SFI_MODE			0x01 
#define AVALANCHE_CORE_SPI_MODE		0x00	

#define  AVALANCHE_SPI_4_BYTE_ADDR_MODE 4
#define  AVALANCHE_SPI_3_BYTE_ADDR_MODE 3
/* Interrupt not used in MMSPI mode i,e each word transfer generates interrupt
 *  and it will degrade the performance 
 */

struct ctlr_cs_sel_t
{
	u8 cs;
	u8 pol;
};

struct avalanche_spi_platform_data {
	/* board specific information */
	u16 	initial_spmode;
	u16		bus_num; /* id for controller */
	u16		max_chipselect;
	int		(*activate_cs)( u8 cs, u8 polarity, 
                            struct ctlr_cs_sel_t *ctlr_cs_sel);
	int		(*deactivate_cs)( u8 cs, u8 polarity,
                              struct ctlr_cs_sel_t *ctlr_cs_sel);
	u32		sysclk;

    u16     addr_mode;
};

/* This structure captures the data required for SFI mode of operation,
 * infomation required by memory mapped set up registers  
 */
struct avalanche_sfi_dev_info_t
{
	u8 write_cmd;			/* Flash Specific WRITE CMD */
	u8 read_cmd;			/* Flash Specific READ/FAST_READ/DUAL_READ CMD */
	u16 mode; 				/* AVALANCHE_SFI_MODE or AVALANCHE_CORE_SPI_MODE */
	u16 dual_read;			/* 0 for Normal & Fast Read  1 for Dual read  */
	u16 num_dummy_bytes; 	/* 0 for Normal read 1 for Fast & Dual Read */
	u16 num_addr_bytes;     /* 3 bytes for most flashes */
	u32 sfi_base;           /* Memory Mapped address for givn CS */
	u16 initialized;        /* Used to avoid CMD initilization repetation */ 
	int (*sfi_transfer)(struct spi_device *spi, struct spi_transfer *t);
							/* Clinet Specific transfer function */
};

#define SFI_SET_MODE(spi, new_mode,old_mode)  {\
	struct avalanche_sfi_dev_info_t *sfi = \
                     (struct avalanche_sfi_dev_info_t*)(spi->controller_data);\
	old_mode = sfi->mode; \
	sfi->mode 	= new_mode;	\
}

/* Workaround for flash endless busy state */
void avalanche_spi_unlock_flash_busy_state( struct spi_device *spi );

#endif /* __AVALANCHE_SPI_H_ */
