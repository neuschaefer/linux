/* Created by Tony Li <tony.li@freescale.com>
 * Copyright (c) 2005 freescale semiconductor
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the  GNU General Public License along
 * with this program; if not, write  to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __PPC_SYSLIB_PPC83XX_PCI_H
#define __PPC_SYSLIB_PPC83XX_PCI_H

typedef struct immr_clk {
	u32 spmr; /* system PLL mode Register  */
	u32 occr; /* output clock control Register  */
	u32 sccr; /* system clock control Register  */
	u8 res0[0xF4];
} immr_clk_t;
#define SPMR_LBIUCM  0x80000000 /* LBIUCM  */
#define SPMR_DDRCM   0x40000000 /* DDRCM  */
#define SPMR_SVCOD   0x30000000 /* SVCOD  */
#define SPMR_SPMF    0x0F000000 /* SPMF  */
#define SPMR_CKID    0x00800000 /* CKID  */
#define SPMR_CKID_SHIFT 23
#define SPMR_COREPLL 0x007F0000 /* COREPLL  */
#define SPMR_CEVCOD  0x000000C0 /* CEVCOD  */
#define SPMR_CEPDF   0x00000020 /* CEPDF  */
#define SPMR_CEPMF   0x0000001F /* CEPMF  */

#define OCCR_PCICOE0 0x80000000 /* PCICOE0  */
#define OCCR_PCICOE1 0x40000000 /* PCICOE1  */
#define OCCR_PCICOE2 0x20000000 /* PCICOE2  */
#define OCCR_PCICOE3 0x10000000 /* PCICOE3  */
#define OCCR_PCICOE4 0x08000000 /* PCICOE4  */
#define OCCR_PCICOE5 0x04000000 /* PCICOE5  */
#define OCCR_PCICOE6 0x02000000 /* PCICOE6  */
#define OCCR_PCICOE7 0x01000000 /* PCICOE7  */
#define OCCR_PCICD0  0x00800000 /* PCICD0  */
#define OCCR_PCICD1  0x00400000 /* PCICD1  */
#define OCCR_PCICD2  0x00200000 /* PCICD2  */
#define OCCR_PCICD3  0x00100000 /* PCICD3  */
#define OCCR_PCICD4  0x00080000 /* PCICD4  */
#define OCCR_PCICD5  0x00040000 /* PCICD5  */
#define OCCR_PCICD6  0x00020000 /* PCICD6  */
#define OCCR_PCICD7  0x00010000 /* PCICD7  */
#define OCCR_PCI1CR  0x00000002 /* PCI1CR  */
#define OCCR_PCI2CR  0x00000001 /* PCI2CR  */

#define SCCR_TSEC1CM  0xc0000000 /* TSEC1CM  */
#define SCCR_TSEC1CM_SHIFT 30
#define SCCR_TSEC2CM  0x30000000 /* TSEC2CM  */
#define SCCR_TSEC2CM_SHIFT 28
#define SCCR_ENCCM    0x03000000 /* ENCCM  */
#define SCCR_ENCCM_SHIFT 24
#define SCCR_USBMPHCM 0x00c00000 /* USBMPHCM  */
#define SCCR_USBMPHCM_SHIFT 22
#define SCCR_USBDRCM  0x00300000 /* USBDRCM  */
#define SCCR_USBDRCM_SHIFT 20
#define SCCR_PCICM    0x00010000 /* PCICM  */

/*
 * Sequencer
 */
typedef struct immr_ios {
	u32	potar0;
	u8	res0[4];
	u32	pobar0;
	u8	res1[4];
	u32	pocmr0;
	u8	res2[4];
	u32	potar1;
	u8	res3[4];
	u32	pobar1;
	u8	res4[4];
	u32	pocmr1;
	u8	res5[4];
	u32	potar2;
	u8	res6[4];
	u32	pobar2;
	u8	res7[4];
	u32	pocmr2;
	u8	res8[4];
	u32	potar3;
	u8	res9[4];
	u32	pobar3;
	u8	res10[4];
	u32	pocmr3;
	u8	res11[4];
	u32	potar4;
	u8	res12[4];
	u32	pobar4;
	u8	res13[4];
	u32	pocmr4;
	u8	res14[4];
	u32	potar5;
	u8	res15[4];
	u32	pobar5;
	u8	res16[4];
	u32	pocmr5;
	u8	res17[4];
	u8	res18[0x60];
	u32	pmcr;
	u8	res19[4];
	u32	dtcr;
	u8	res20[4];
} immr_ios_t;
#define POTAR_TA_MASK	0x000fffff
#define POBAR_BA_MASK	0x000fffff
#define POCMR_EN	0x80000000
#define POCMR_IO	0x40000000 /* 0--memory space 1--I/O space */
#define POCMR_SE	0x20000000 /* streaming enable */
#define POCMR_DST	0x10000000 /* 0--PCI1 1--PCI2 */
#define POCMR_CM_MASK	0x000fffff

/*
 * PCI Controller Control and Status Registers
 */
typedef struct immr_pcictrl {
	u32	esr;
	u32	ecdr;
	u32	eer;
	u32	eatcr;
	u32	eacr;
	u32	eeacr;
	u32	edlcr;
	u32	edhcr;
	u32	gcr;
	u32	ecr;
	u32	gsr;
	u8	res0[12];
	u32	pitar2;
	u8	res1[4];
	u32	pibar2;
	u32	piebar2;
	u32	piwar2;
	u8	res2[4];
	u32	pitar1;
	u8	res3[4];
	u32	pibar1;
	u32	piebar1;
	u32	piwar1;
	u8	res4[4];
	u32	pitar0;
	u8	res5[4];
	u32	pibar0;
	u8	res6[4];
	u32	piwar0;
	u8	res7[132];
} immr_pcictrl_t;
#define PITAR_TA_MASK	0x000fffff
#define PIBAR_MASK	0xffffffff
#define PIEBAR_EBA_MASK	0x000fffff
#define PIWAR_EN	0x80000000
#define PIWAR_PF	0x20000000
#define PIWAR_RTT_MASK	0x000f0000
#define PIWAR_RTT_NO_SNOOP	0x00040000
#define PIWAR_RTT_SNOOP	0x00050000
#define PIWAR_WTT_MASK	0x0000f000
#define PIWAR_WTT_NO_SNOOP	0x00004000
#define PIWAR_WTT_SNOOP	0x00005000
#define PIWAR_IWS_MASK	0x0000003F
#define PIWAR_IWS_4K	0x0000000B
#define PIWAR_IWS_8K	0x0000000C
#define PIWAR_IWS_16K	0x0000000D
#define PIWAR_IWS_32K	0x0000000E
#define PIWAR_IWS_64K	0x0000000F
#define PIWAR_IWS_128K	0x00000010
#define PIWAR_IWS_256K	0x00000011
#define PIWAR_IWS_512K	0x00000012
#define PIWAR_IWS_1M	0x00000013
#define PIWAR_IWS_2M	0x00000014
#define PIWAR_IWS_4M	0x00000015
#define PIWAR_IWS_8M	0x00000016
#define PIWAR_IWS_16M	0x00000017
#define PIWAR_IWS_32M	0x00000018
#define PIWAR_IWS_64M	0x00000019
#define PIWAR_IWS_128M	0x0000001A
#define PIWAR_IWS_256M	0x0000001B
#define PIWAR_IWS_512M	0x0000001C
#define PIWAR_IWS_1G	0x0000001D
#define PIWAR_IWS_2G	0x0000001E

#endif /* __PPC_SYSLIB_PPC83XX_PCI_H */
