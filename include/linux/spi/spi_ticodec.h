/*
 *
 * spi_ticodec.h
 * Description:
 * code spi header file
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


#ifndef __TI_CODEC_SPI_H_
#define __TI_CODEC_SPI_H_

struct ti_ctlr_cs_sel_t
{
	u8 cs;
	u8 pol;
};

#define SPI_3_WIRE 0x40

struct ti_codec_spi_platform_data {
	/* board specific information */
	u16 	initial_spmode;
	u16		bus_num; /* id for controller */
	u16		max_chipselect;
	int		(*activate_cs)( u8 cs, u8 polarity, 
                            struct ti_ctlr_cs_sel_t *ctlr_cs_sel);
	int		(*deactivate_cs)( u8 cs, u8 polarity,
                              struct ti_ctlr_cs_sel_t *ctlr_cs_sel);
	u32		sysclk;
};

#endif /* __TI_CODEC_SPI_H_ */
