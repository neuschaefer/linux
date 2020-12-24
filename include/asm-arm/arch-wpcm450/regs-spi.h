/* linux/include/asm-arm/arch-wpcm450/regs-spi.h
 *
 * Copyright (c) 2004 Fetron GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 SPI register definition
 *
 *  Changelog:
 *    20-04-2004     KF      Created file
 *    04-10-2004     BJD     Removed VA address (no longer mapped)
 *			     tidied file for submission
 *    03-04-2005     LCVR    Added S3C2400_SPPIN_nCS definition
 */

#ifndef __ASM_ARCH_REGS_SPI_H
#define __ASM_ARCH_REGS_SPI_H


#define WPCM450_SPCON	(0x00)

#define WPCM450_SPCON_SMOD_DMA	  (2<<5)	/* DMA mode */
#define WPCM450_SPCON_SMOD_INT	  (1<<5)	/* interrupt mode */
#define WPCM450_SPCON_SMOD_POLL   (0<<5)	/* polling mode */
#define WPCM450_SPCON_ENSCK	  (1<<4)	/* Enable SCK */
#define WPCM450_SPCON_MSTR	  (1<<3)	/* Master/Slave select
						   0: slave, 1: master */
#define WPCM450_SPCON_CPOL_HIGH	  (1<<2)	/* Clock polarity select */
#define WPCM450_SPCON_CPOL_LOW	  (0<<2)	/* Clock polarity select */

#define WPCM450_SPCON_CPHA_FMTB	  (1<<1)	/* Clock Phase Select */
#define WPCM450_SPCON_CPHA_FMTA	  (0<<1)	/* Clock Phase Select */

#define WPCM450_SPCON_TAGD	  (1<<0)	/* Tx auto garbage data mode */


#define WPCM450_SPSTA	 (0x04)

#define WPCM450_SPSTA_DCOL	  (1<<2)	/* Data Collision Error */
#define WPCM450_SPSTA_MULD	  (1<<1)	/* Multi Master Error */
#define WPCM450_SPSTA_READY	  (1<<0)	/* Data Tx/Rx ready */


#define WPCM450_SPPIN	 (0x08)

#define WPCM450_SPPIN_ENMUL	  (1<<2)	/* Multi Master Error detect */
#define WPCM450_SPPIN_RESERVED	  (1<<1)
#define S3C2400_SPPIN_nCS     	  (1<<1)	/* SPI Card Select */
#define WPCM450_SPPIN_KEEP	  (1<<0)	/* Master Out keep */


#define WPCM450_SPPRE	 (0x0C)
#define WPCM450_SPTDAT	 (0x10)
#define WPCM450_SPRDAT	 (0x14)

#endif /* __ASM_ARCH_REGS_SPI_H */
