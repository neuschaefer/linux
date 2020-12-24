/* linux/include/asm-arm/arch-wpcm450/regs-nand.h
 *
 * Copyright (c) 2004,2005 Simtec Electronics <linux@simtec.co.uk>
 *		      http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 NAND register definitions
 *
 *  Changelog:
 *    18-Aug-2004    BJD     Copied file from 2.4 and updated
 *    01-May-2005    BJD     Added definitions for s3c2440 controller
*/

#ifndef __ASM_ARM_REGS_NAND
#define __ASM_ARM_REGS_NAND "$Id$"


#define WPCM450_NFREG(x) (x)

#define WPCM450_NFCONF  WPCM450_NFREG(0x00)
#define WPCM450_NFCMD   WPCM450_NFREG(0x04)
#define WPCM450_NFADDR  WPCM450_NFREG(0x08)
#define WPCM450_NFDATA  WPCM450_NFREG(0x0C)
#define WPCM450_NFSTAT  WPCM450_NFREG(0x10)
#define WPCM450_NFECC   WPCM450_NFREG(0x14)

#define S3C2440_NFCONT   WPCM450_NFREG(0x04)
#define S3C2440_NFCMD    WPCM450_NFREG(0x08)
#define S3C2440_NFADDR   WPCM450_NFREG(0x0C)
#define S3C2440_NFDATA   WPCM450_NFREG(0x10)
#define S3C2440_NFECCD0  WPCM450_NFREG(0x14)
#define S3C2440_NFECCD1  WPCM450_NFREG(0x18)
#define S3C2440_NFECCD   WPCM450_NFREG(0x1C)
#define S3C2440_NFSTAT   WPCM450_NFREG(0x20)
#define S3C2440_NFESTAT0 WPCM450_NFREG(0x24)
#define S3C2440_NFESTAT1 WPCM450_NFREG(0x28)
#define S3C2440_NFMECC0  WPCM450_NFREG(0x2C)
#define S3C2440_NFMECC1  WPCM450_NFREG(0x30)
#define S3C2440_NFSECC   WPCM450_NFREG(0x34)
#define S3C2440_NFSBLK   WPCM450_NFREG(0x38)
#define S3C2440_NFEBLK   WPCM450_NFREG(0x3C)

#define WPCM450_NFCONF_EN          (1<<15)
#define WPCM450_NFCONF_512BYTE     (1<<14)
#define WPCM450_NFCONF_4STEP       (1<<13)
#define WPCM450_NFCONF_INITECC     (1<<12)
#define WPCM450_NFCONF_nFCE        (1<<11)
#define WPCM450_NFCONF_TACLS(x)    ((x)<<8)
#define WPCM450_NFCONF_TWRPH0(x)   ((x)<<4)
#define WPCM450_NFCONF_TWRPH1(x)   ((x)<<0)

#define WPCM450_NFSTAT_BUSY        (1<<0)

#define S3C2440_NFCONF_BUSWIDTH_8	(0<<0)
#define S3C2440_NFCONF_BUSWIDTH_16	(1<<0)
#define S3C2440_NFCONF_ADVFLASH		(1<<3)
#define S3C2440_NFCONF_TACLS(x)		((x)<<12)
#define S3C2440_NFCONF_TWRPH0(x)	((x)<<8)
#define S3C2440_NFCONF_TWRPH1(x)	((x)<<4)

#define S3C2440_NFCONT_LOCKTIGHT	(1<<13)
#define S3C2440_NFCONT_SOFTLOCK		(1<<12)
#define S3C2440_NFCONT_ILLEGALACC_EN	(1<<10)
#define S3C2440_NFCONT_RNBINT_EN	(1<<9)
#define S3C2440_NFCONT_RN_FALLING	(1<<8)
#define S3C2440_NFCONT_SPARE_ECCLOCK	(1<<6)
#define S3C2440_NFCONT_MAIN_ECCLOCK	(1<<5)
#define S3C2440_NFCONT_INITECC		(1<<4)
#define S3C2440_NFCONT_nFCE		(1<<1)
#define S3C2440_NFCONT_ENABLE		(1<<0)

#define S3C2440_NFSTAT_READY		(1<<0)
#define S3C2440_NFSTAT_nCE		(1<<1)
#define S3C2440_NFSTAT_RnB_CHANGE	(1<<2)
#define S3C2440_NFSTAT_ILLEGAL_ACCESS	(1<<3)

#endif /* __ASM_ARM_REGS_NAND */

