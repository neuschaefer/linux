/* linux/include/asm-arm/arch-wpcm450/regs-mem.h
 *
 * Copyright (c) 2004 Simtec Electronics <linux@simtec.co.uk>
 *		http://www.simtec.co.uk/products/SWLINUX/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 Memory Control register definitions
 *
 *  Changelog:
 *	29-Sep-2004  BJD  Initial include for Linux
 *      10-Mar-2005  LCVR Changed WPCM450_VA to WPCM450_VA
 *      04-Apr-2005  LCVR Added S3C2400 DRAM/BANKSIZE_MASK definitions
 *
*/

#ifndef __ASM_ARM_MEMREGS_H
#define __ASM_ARM_MEMREGS_H "$Id$"

#ifndef WPCM450_MEMREG
#define WPCM450_MEMREG(x) (WPCM450_VA_MEMCTRL + (x))
#endif

/* bus width, and wait state control */
#define WPCM450_BWSCON			WPCM450_MEMREG(0x0000)

/* bank zero config - note, pinstrapped from OM pins! */
#define WPCM450_BWSCON_DW0_16		(1<<1)
#define WPCM450_BWSCON_DW0_32		(2<<1)

/* bank one configs */
#define WPCM450_BWSCON_DW1_8		(0<<4)
#define WPCM450_BWSCON_DW1_16		(1<<4)
#define WPCM450_BWSCON_DW1_32		(2<<4)
#define WPCM450_BWSCON_WS1		(1<<6)
#define WPCM450_BWSCON_ST1		(1<<7)

/* bank 2 configurations */
#define WPCM450_BWSCON_DW2_8		(0<<8)
#define WPCM450_BWSCON_DW2_16		(1<<8)
#define WPCM450_BWSCON_DW2_32		(2<<8)
#define WPCM450_BWSCON_WS2		(1<<10)
#define WPCM450_BWSCON_ST2		(1<<11)

/* bank 3 configurations */
#define WPCM450_BWSCON_DW3_8		(0<<12)
#define WPCM450_BWSCON_DW3_16		(1<<12)
#define WPCM450_BWSCON_DW3_32		(2<<12)
#define WPCM450_BWSCON_WS3		(1<<14)
#define WPCM450_BWSCON_ST3		(1<<15)

/* bank 4 configurations */
#define WPCM450_BWSCON_DW4_8		(0<<16)
#define WPCM450_BWSCON_DW4_16		(1<<16)
#define WPCM450_BWSCON_DW4_32		(2<<16)
#define WPCM450_BWSCON_WS4		(1<<18)
#define WPCM450_BWSCON_ST4		(1<<19)

/* bank 5 configurations */
#define WPCM450_BWSCON_DW5_8		(0<<20)
#define WPCM450_BWSCON_DW5_16		(1<<20)
#define WPCM450_BWSCON_DW5_32		(2<<20)
#define WPCM450_BWSCON_WS5		(1<<22)
#define WPCM450_BWSCON_ST5		(1<<23)

/* bank 6 configurations */
#define WPCM450_BWSCON_DW6_8		(0<<24)
#define WPCM450_BWSCON_DW6_16		(1<<24)
#define WPCM450_BWSCON_DW6_32		(2<<24)
#define WPCM450_BWSCON_WS6		(1<<26)
#define WPCM450_BWSCON_ST6		(1<<27)

/* bank 7 configurations */
#define WPCM450_BWSCON_DW7_8		(0<<28)
#define WPCM450_BWSCON_DW7_16		(1<<28)
#define WPCM450_BWSCON_DW7_32		(2<<28)
#define WPCM450_BWSCON_WS7		(1<<30)
#define WPCM450_BWSCON_ST7		(1<<31)

/* memory set (rom, ram) */
#define WPCM450_BANKCON0		WPCM450_MEMREG(0x0004)
#define WPCM450_BANKCON1		WPCM450_MEMREG(0x0008)
#define WPCM450_BANKCON2		WPCM450_MEMREG(0x000C)
#define WPCM450_BANKCON3		WPCM450_MEMREG(0x0010)
#define WPCM450_BANKCON4		WPCM450_MEMREG(0x0014)
#define WPCM450_BANKCON5		WPCM450_MEMREG(0x0018)
#define WPCM450_BANKCON6		WPCM450_MEMREG(0x001C)
#define WPCM450_BANKCON7		WPCM450_MEMREG(0x0020)

/* bank configuration registers */

#define WPCM450_BANKCON_PMCnorm		(0x00)
#define WPCM450_BANKCON_PMC4		(0x01)
#define WPCM450_BANKCON_PMC8		(0x02)
#define WPCM450_BANKCON_PMC16		(0x03)

/* bank configurations for banks 0..7, note banks
 * 6 and 7 have differnt configurations depending on
 * the memory type bits */

#define WPCM450_BANKCON_Tacp2		(0x0 << 2)
#define WPCM450_BANKCON_Tacp3		(0x1 << 2)
#define WPCM450_BANKCON_Tacp4		(0x2 << 2)
#define WPCM450_BANKCON_Tacp6		(0x3 << 2)

#define WPCM450_BANKCON_Tcah0		(0x0 << 4)
#define WPCM450_BANKCON_Tcah1		(0x1 << 4)
#define WPCM450_BANKCON_Tcah2		(0x2 << 4)
#define WPCM450_BANKCON_Tcah4		(0x3 << 4)

#define WPCM450_BANKCON_Tcoh0		(0x0 << 6)
#define WPCM450_BANKCON_Tcoh1		(0x1 << 6)
#define WPCM450_BANKCON_Tcoh2		(0x2 << 6)
#define WPCM450_BANKCON_Tcoh4		(0x3 << 6)

#define WPCM450_BANKCON_Tacc1		(0x0 << 8)
#define WPCM450_BANKCON_Tacc2		(0x1 << 8)
#define WPCM450_BANKCON_Tacc3		(0x2 << 8)
#define WPCM450_BANKCON_Tacc4		(0x3 << 8)
#define WPCM450_BANKCON_Tacc6		(0x4 << 8)
#define WPCM450_BANKCON_Tacc8		(0x5 << 8)
#define WPCM450_BANKCON_Tacc10		(0x6 << 8)
#define WPCM450_BANKCON_Tacc14		(0x7 << 8)

#define WPCM450_BANKCON_Tcos0		(0x0 << 11)
#define WPCM450_BANKCON_Tcos1		(0x1 << 11)
#define WPCM450_BANKCON_Tcos2		(0x2 << 11)
#define WPCM450_BANKCON_Tcos4		(0x3 << 11)

#define WPCM450_BANKCON_Tacs0		(0x0 << 13)
#define WPCM450_BANKCON_Tacs1		(0x1 << 13)
#define WPCM450_BANKCON_Tacs2		(0x2 << 13)
#define WPCM450_BANKCON_Tacs4		(0x3 << 13)

#define WPCM450_BANKCON_SRAM		(0x0 << 15)
#define S3C2400_BANKCON_EDODRAM		(0x2 << 15)
#define WPCM450_BANKCON_SDRAM		(0x3 << 15)

/* next bits only for EDO DRAM in 6,7 */
#define S3C2400_BANKCON_EDO_Trdc1      (0x00 << 4)
#define S3C2400_BANKCON_EDO_Trdc2      (0x01 << 4)
#define S3C2400_BANKCON_EDO_Trdc3      (0x02 << 4)
#define S3C2400_BANKCON_EDO_Trdc4      (0x03 << 4)

/* CAS pulse width */
#define S3C2400_BANKCON_EDO_PULSE1     (0x00 << 3)
#define S3C2400_BANKCON_EDO_PULSE2     (0x01 << 3)

/* CAS pre-charge */
#define S3C2400_BANKCON_EDO_TCP1       (0x00 << 2)
#define S3C2400_BANKCON_EDO_TCP2       (0x01 << 2)

/* control column address select */
#define S3C2400_BANKCON_EDO_SCANb8     (0x00 << 0)
#define S3C2400_BANKCON_EDO_SCANb9     (0x01 << 0)
#define S3C2400_BANKCON_EDO_SCANb10    (0x02 << 0)
#define S3C2400_BANKCON_EDO_SCANb11    (0x03 << 0)

/* next bits only for SDRAM in 6,7 */
#define WPCM450_BANKCON_Trdc2		(0x00 << 2)
#define WPCM450_BANKCON_Trdc3		(0x01 << 2)
#define WPCM450_BANKCON_Trdc4		(0x02 << 2)

/* control column address select */
#define WPCM450_BANKCON_SCANb8		(0x00 << 0)
#define WPCM450_BANKCON_SCANb9		(0x01 << 0)
#define WPCM450_BANKCON_SCANb10		(0x02 << 0)

#define WPCM450_REFRESH			WPCM450_MEMREG(0x0024)
#define WPCM450_BANKSIZE		WPCM450_MEMREG(0x0028)
#define WPCM450_MRSRB6			WPCM450_MEMREG(0x002C)
#define WPCM450_MRSRB7			WPCM450_MEMREG(0x0030)

/* refresh control */

#define WPCM450_REFRESH_REFEN		(1<<23)
#define WPCM450_REFRESH_SELF		(1<<22)
#define WPCM450_REFRESH_REFCOUNTER	((1<<11)-1)

#define WPCM450_REFRESH_TRP_MASK	(3<<20)
#define WPCM450_REFRESH_TRP_2clk	(0<<20)
#define WPCM450_REFRESH_TRP_3clk	(1<<20)
#define WPCM450_REFRESH_TRP_4clk	(2<<20)

#define S3C2400_REFRESH_DRAM_TRP_MASK   (3<<20)
#define S3C2400_REFRESH_DRAM_TRP_1_5clk (0<<20)
#define S3C2400_REFRESH_DRAM_TRP_2_5clk (1<<20)
#define S3C2400_REFRESH_DRAM_TRP_3_5clk (2<<20)
#define S3C2400_REFRESH_DRAM_TRP_4_5clk (3<<20)

#define WPCM450_REFRESH_TSRC_MASK	(3<<18)
#define WPCM450_REFRESH_TSRC_4clk	(0<<18)
#define WPCM450_REFRESH_TSRC_5clk	(1<<18)
#define WPCM450_REFRESH_TSRC_6clk	(2<<18)
#define WPCM450_REFRESH_TSRC_7clk	(3<<18)


/* mode select register(s) */

#define  WPCM450_MRSRB_CL1		(0x00 << 4)
#define  WPCM450_MRSRB_CL2		(0x02 << 4)
#define  WPCM450_MRSRB_CL3		(0x03 << 4)

/* bank size register */
#define WPCM450_BANKSIZE_128M		(0x2 << 0)
#define WPCM450_BANKSIZE_64M		(0x1 << 0)
#define WPCM450_BANKSIZE_32M		(0x0 << 0)
#define WPCM450_BANKSIZE_16M		(0x7 << 0)
#define WPCM450_BANKSIZE_8M		(0x6 << 0)
#define WPCM450_BANKSIZE_4M		(0x5 << 0)
#define WPCM450_BANKSIZE_2M		(0x4 << 0)
#define WPCM450_BANKSIZE_MASK		(0x7 << 0)
#define S3C2400_BANKSIZE_MASK           (0x4 << 0)
#define WPCM450_BANKSIZE_SCLK_EN	(1<<4)
#define WPCM450_BANKSIZE_SCKE_EN	(1<<5)
#define WPCM450_BANKSIZE_BURST		(1<<7)

#endif /* __ASM_ARM_MEMREGS_H */
