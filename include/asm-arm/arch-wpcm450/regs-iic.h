/* linux/include/asm-arm/arch-wpcm450/regs-iic.h
 *
 * Copyright (c) 2004 Simtec Electronics <linux@simtec.co.uk>
 *		http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 I2C Controller
 *
 *  Changelog:
 *	03-Oct-2004  BJD  Initial include for Linux
 *	08-Nov-2004  BJD  Added S3C2440 filter register
*/

#ifndef __ASM_ARCH_REGS_IIC_H
#define __ASM_ARCH_REGS_IIC_H __FILE__

/* see wpcm450x user guide, v1.1, section 9 (p447) for more info */

#define WPCM450_IICREG(x) (x)

#define WPCM450_IICCON    WPCM450_IICREG(0x00)
#define WPCM450_IICSTAT   WPCM450_IICREG(0x04)
#define WPCM450_IICADD    WPCM450_IICREG(0x08)
#define WPCM450_IICDS     WPCM450_IICREG(0x0C)
#define S3C2440_IICLC	  WPCM450_IICREG(0x10)

#define WPCM450_IICCON_ACKEN		(1<<7)
#define WPCM450_IICCON_TXDIV_16		(0<<6)
#define WPCM450_IICCON_TXDIV_512	(1<<6)
#define WPCM450_IICCON_IRQEN		(1<<5)
#define WPCM450_IICCON_IRQPEND		(1<<4)
#define WPCM450_IICCON_SCALE(x)		((x)&15)
#define WPCM450_IICCON_SCALEMASK	(0xf)

#define WPCM450_IICSTAT_MASTER_RX	(2<<6)
#define WPCM450_IICSTAT_MASTER_TX	(3<<6)
#define WPCM450_IICSTAT_SLAVE_RX	(0<<6)
#define WPCM450_IICSTAT_SLAVE_TX	(1<<6)
#define WPCM450_IICSTAT_MODEMASK	(3<<6)

#define WPCM450_IICSTAT_START		(1<<5)
#define WPCM450_IICSTAT_BUSBUSY		(1<<5)
#define WPCM450_IICSTAT_TXRXEN		(1<<4)
#define WPCM450_IICSTAT_ARBITR		(1<<3)
#define WPCM450_IICSTAT_ASSLAVE		(1<<2)
#define WPCM450_IICSTAT_ADDR0		(1<<1)
#define WPCM450_IICSTAT_LASTBIT		(1<<0)

#define WPCM450_IICLC_SDA_DELAY0	(0 << 0)
#define WPCM450_IICLC_SDA_DELAY5	(1 << 0)
#define WPCM450_IICLC_SDA_DELAY10	(2 << 0)
#define WPCM450_IICLC_SDA_DELAY15	(3 << 0)
#define WPCM450_IICLC_SDA_DELAY_MASK	(3 << 0)

#define WPCM450_IICLC_FILTER_ON		(1<<2)

#endif /* __ASM_ARCH_REGS_IIC_H */
