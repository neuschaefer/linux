/*
 * Copyright (c) 2005 Cisco Systems.  All rights reserved.
 * Roland Dreier <rolandd@cisco.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/init.h>

#include <asm/reg.h>
#include <asm/io.h>
#include <asm/ibm44x.h>

#include "ppc440spe_pcie.h"

static int
pcie_read_config(struct pci_bus *bus, unsigned int devfn, int offset,
		     int len, u32 *val)
{
	struct pci_controller *hose = bus->sysdata;
#ifdef CONFIG_PCIE_ENDPOINT
	/*
	 * Endpoint can not generate upstream(remote) config cycles.
	 */
	return PCIBIOS_DEVICE_NOT_FOUND;
#endif
	if (!((PCI_SLOT(devfn) == 1) && (PCI_FUNC(devfn) == 0)))
		return PCIBIOS_DEVICE_NOT_FOUND;
	/*
	 * 440SPE rev B implements only one function per port
	 */
	if (ppc440spe_revB())
		devfn = 0;

	offset += devfn << 12;

	/*
	 * Note: the caller has already checked that offset is
	 * suitably aligned and that len is 1, 2 or 4.
	 */
	switch (len) {
	case 1:
		*val = in_8(hose->cfg_data + offset);
		break;
	case 2:
		*val = in_le16(hose->cfg_data + offset);
		break;
	default:
		*val = in_le32(hose->cfg_data + offset);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static int
pcie_write_config(struct pci_bus *bus, unsigned int devfn, int offset,
		      int len, u32 val)
{
	struct pci_controller *hose = bus->sysdata;
#ifdef CONFIG_PCIE_ENDPOINT
	/*
	 * Endpoint can not generate upstream(remote) config cycles.
	 */
	return PCIBIOS_DEVICE_NOT_FOUND;
#endif

	if (!((PCI_SLOT(devfn) == 1) && (PCI_FUNC(devfn) == 0)))
		return PCIBIOS_DEVICE_NOT_FOUND;
	/*
	 * 440SPE rev B implements only one function per port
	 */
	if (ppc440spe_revB())
		devfn = 0;

	offset += devfn << 12;

	switch (len) {
	case 1:
		out_8(hose->cfg_data + offset, val);
		break;
	case 2:
		out_le16(hose->cfg_data + offset, val);
		break;
	default:
		out_le32(hose->cfg_data + offset, val);
		break;
	}
	return PCIBIOS_SUCCESSFUL;
}

static struct pci_ops pcie_pci_ops =
{
	.read  = pcie_read_config,
	.write = pcie_write_config
};

enum {
	PTYPE_ENDPOINT		= 0x0,
	PTYPE_LEGACY_ENDPOINT	= 0x1,
	PTYPE_ROOT_PORT		= 0x4,

	LNKW_X1			= 0x1,
	LNKW_X4			= 0x4,
	LNKW_X8			= 0x8
};

int
ppc440spe_revB(void)
{
	if (mfspr(SPRN_PVR) == 0x53421891)
		return 1;
	else
		return 0;
}

/*
 * Set up UTL registers
 */
static void
ppc440spe_setup_utl(u32 port)
{
	void __iomem *utl_base;

	/*
	 * Map UTL at 0xc_1000_n000
	 */
	switch (port) {
	case 0:
		mtdcr(DCRN_PEGPL_REGBAH(PCIE0), 0x0000000c);
		mtdcr(DCRN_PEGPL_REGBAL(PCIE0), 0x10000000);
		mtdcr(DCRN_PEGPL_REGMSK(PCIE0), 0x00007001);
		mtdcr(DCRN_PEGPL_SPECIAL(PCIE0), 0x68782800);
		break;

	case 1:
		mtdcr(DCRN_PEGPL_REGBAH(PCIE1), 0x0000000c);
		mtdcr(DCRN_PEGPL_REGBAL(PCIE1), 0x10001000);
		mtdcr(DCRN_PEGPL_REGMSK(PCIE1), 0x00007001);
		mtdcr(DCRN_PEGPL_SPECIAL(PCIE1), 0x68782800);
		break;

	case 2:
		mtdcr(DCRN_PEGPL_REGBAH(PCIE2), 0x0000000c);
		mtdcr(DCRN_PEGPL_REGBAL(PCIE2), 0x10002000);
		mtdcr(DCRN_PEGPL_REGMSK(PCIE2), 0x00007001);
		mtdcr(DCRN_PEGPL_SPECIAL(PCIE2), 0x68782800);
		break;
	}
	utl_base = ioremap64(0xc10000000ull + 0x1000 * port, 0x100);
	
	/*
	 * Set buffer allocations and then assert VRB and TXE.
	 */
	out_be32(utl_base + PEUTL_OUTTR,   0x08000000);
	out_be32(utl_base + PEUTL_INTR,    0x02000000);
	out_be32(utl_base + PEUTL_OPDBSZ,  0x10000000);
	out_be32(utl_base + PEUTL_PBBSZ,   0x53000000);
	out_be32(utl_base + PEUTL_IPHBSZ,  0x08000000);
	out_be32(utl_base + PEUTL_IPDBSZ,  0x10000000);
	out_be32(utl_base + PEUTL_RCIRQEN, 0x00f00000);
	out_be32(utl_base + PEUTL_PCTL,    0x80800066);

	iounmap(utl_base);
}

static int check_error(void)
{
	u32 valPE0, valPE1, valPE2;
	int err = 0;

	/* SDR0_PEGPLLLCT1 reset */
	if (!(valPE0 = SDR_READ(PESDR0_PLLLCT1) & 0x01000000)) {
		/*
		 * the PCIe core was probably already initialised
		 * by firmware - let's re-reset RCSSET regs
		 */
		pr_debug("PCIE: SDR0_PLLLCT1 already reset.\n");
		SDR_WRITE(PESDR0_RCSSET, 0x01010000);
		SDR_WRITE(PESDR1_RCSSET, 0x01010000);
		SDR_WRITE(PESDR2_RCSSET, 0x01010000);
	}

	valPE0 = SDR_READ(PESDR0_RCSSET);
	valPE1 = SDR_READ(PESDR1_RCSSET);
	valPE2 = SDR_READ(PESDR2_RCSSET);

	/* SDR0_PExRCSSET rstgu */
	if ( !(valPE0 & 0x01000000) ||
	     !(valPE1 & 0x01000000) ||
	     !(valPE2 & 0x01000000)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET rstgu error\n");
		err = -1;
	}

	/* SDR0_PExRCSSET rstdl */
	if ( !(valPE0 & 0x00010000) ||
	     !(valPE1 & 0x00010000) ||
	     !(valPE2 & 0x00010000)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET rstdl error\n");
		err = -1;
	}

	/* SDR0_PExRCSSET rstpyn */
	if ( (valPE0 & 0x00001000) ||
	     (valPE1 & 0x00001000) ||
	     (valPE2 & 0x00001000)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET rstpyn error\n");
		err = -1;
	}

	/* SDR0_PExRCSSET hldplb */
	if ( (valPE0 & 0x10000000) ||
	     (valPE1 & 0x10000000) ||
	     (valPE2 & 0x10000000)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET hldplb error\n");
		err = -1;
	}

	/* SDR0_PExRCSSET rdy */
	if ( (valPE0 & 0x00100000) ||
	     (valPE1 & 0x00100000) ||
	     (valPE2 & 0x00100000)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET rdy error\n");
		err = -1;
	}

	/* SDR0_PExRCSSET shutdown */
	if ( (valPE0 & 0x00000100) ||
	     (valPE1 & 0x00000100) ||
	     (valPE2 & 0x00000100)) {
		printk(KERN_INFO "PCIE:  SDR0_PExRCSSET shutdown error\n");
		err = -1;
	}
	return err;
}

/*
 * Initialize PCI Express core as described in User Manual
 */
static int
ppc440spe_init_pcie(void)
{
	int time_out = 20;

	/* Set PLL clock receiver to LVPECL */
	SDR_WRITE(PESDR0_PLLLCT1, SDR_READ(PESDR0_PLLLCT1) | 1 << 28);

	if (check_error())
		return -1;

	if (!(SDR_READ(PESDR0_PLLLCT2) & 0x10000)) {
		printk(KERN_INFO "PCIE: PESDR_PLLCT2 resistance calibration "
				"failed (0x%08x)\n",
		       SDR_READ(PESDR0_PLLLCT2));
		return -1;
	}

	/* De-assert reset of PCIe PLL, wait for lock */
	SDR_WRITE(PESDR0_PLLLCT1, SDR_READ(PESDR0_PLLLCT1) & ~(1 << 24));
	udelay(3);

	while(time_out) {
		if (!(SDR_READ(PESDR0_PLLLCT3) & 0x10000000)) {
			time_out--;
			udelay(1);
		} else
			break;
	}
	if (!time_out) {
		printk(KERN_INFO "PCIE: VCO output not locked\n");
		return -1;
	}
	
	pr_debug("PCIE initialization OK\n");

	return 0;
}

/*
 *  Yucca board as End point and root point setup
 *                    and
 *    testing inbound and out bound windows
 *
 *  YUCCA board can be plugged into another yucca board or you can get PCI-E
 *  cable which can be used to setup loop back from one port to another port.
 *  Please rememeber that unless there is a endpoint plugged in to root port it
 *  will not initialize. It is the same in case of endpoint , unless there is
 *  root port attached it will not initialize.
 *
 *  In this release of software all the PCI-E ports are configured as either
 *  endpoint or rootpoint.In future we will have support for selective ports
 *  setup as endpoint and root point in single board.
 *
 *  Once your board came up as root point , you can verify by reading
 *  /proc/bus/pci/devices. Where you can see the configuration registers
 *  of end point device attached to the port.
 *
 *  Enpoint cofiguration can be verified by connecting Yucca board to any
 *  host or another yucca board. Then try to scan the device. In case of
 *  linux use "lspci" or appripriate os command.
 *  
 *  To verify the inbound and outbound windows on yucca to yucca configuration
 *  windows already configured for memory region 0. On root point side memory
 *  map the 36 bit address value 0x4 0000 0000(SRAM) then do the read write to
 *  the memory mapped address. On endpoint board memory map the 0x4 0000 0000
 *  read the data to verify if writes happened or not.For inbound window 
 *  verificatio do the reverse way of write and read .
 */
int ppc440spe_init_pcie_root_or_endport(u32 port)
{
	static int core_init;
	int attempts;
	u32 val = 0;

	if (!core_init) {
		if(ppc440spe_init_pcie())
			return -1;
		++core_init;
	}

	/*
	 * Initialize various parts of the PCI Express core for our port:
	 *
	 * - Set as a root port and enable max width
	 *   (PXIE0 -> X8, PCIE1 and PCIE2 -> X4).
	 * - Set up UTL configuration.
	 * - Increase SERDES drive strength to levels suggested by AMCC.
	 * - De-assert RSTPYN, RSTDL and RSTGU.
	 *
	 * NOTICE for revB chip: PESDRn_UTLSET2 is not set - we leave it with
	 * default setting 0x11310000. The register has new fields,
	 * PESDRn_UTLSET2[LKINE] in particular: clearing it leads to PCIE core
	 * hang.
	 */

#ifdef CONFIG_PCIE_ENDPOINT
#define PTYPE_TYPE PTYPE_LEGACY_ENDPOINT
#else
#define PTYPE_TYPE PTYPE_ROOT_PORT
#endif
	switch (port) {
	case 0:
		SDR_WRITE(PESDR0_DLPSET, 1 << 24 | PTYPE_TYPE << 20 | LNKW_X8 << 12); 

		SDR_WRITE(PESDR0_UTLSET1, 0x20222222);
		if (!ppc440spe_revB())
			SDR_WRITE(PESDR0_UTLSET2, 0x11000000);
		SDR_WRITE(PESDR0_HSSL0SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL1SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL2SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL3SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL4SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL5SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL6SET1, 0x35000000);
		SDR_WRITE(PESDR0_HSSL7SET1, 0x35000000);

		SDR_WRITE(PESDR0_RCSSET,
			  (SDR_READ(PESDR0_RCSSET) & ~(1 << 24 | 1 << 16)) | 1 << 12);
		break;

	case 1:
		SDR_WRITE(PESDR1_DLPSET, 1 << 24 | PTYPE_TYPE << 20 | LNKW_X4 << 12); 

		SDR_WRITE(PESDR1_UTLSET1, 0x20222222);
		if (!ppc440spe_revB())
			SDR_WRITE(PESDR1_UTLSET2, 0x11000000);
		SDR_WRITE(PESDR1_HSSL0SET1, 0x35000000);
		SDR_WRITE(PESDR1_HSSL1SET1, 0x35000000);
		SDR_WRITE(PESDR1_HSSL2SET1, 0x35000000);
		SDR_WRITE(PESDR1_HSSL3SET1, 0x35000000);

		SDR_WRITE(PESDR1_RCSSET,
			  (SDR_READ(PESDR1_RCSSET) & ~(1 << 24 | 1 << 16)) | 1 << 12);
		break;

	case 2:
		SDR_WRITE(PESDR2_DLPSET, 1 << 24 | PTYPE_TYPE << 20 | LNKW_X4 << 12); 

		SDR_WRITE(PESDR2_UTLSET1, 0x20222222);
		if (!ppc440spe_revB())
			SDR_WRITE(PESDR2_UTLSET2, 0x11000000);
		SDR_WRITE(PESDR2_HSSL0SET1, 0x35000000);
		SDR_WRITE(PESDR2_HSSL1SET1, 0x35000000);
		SDR_WRITE(PESDR2_HSSL2SET1, 0x35000000);
		SDR_WRITE(PESDR2_HSSL3SET1, 0x35000000);

		SDR_WRITE(PESDR2_RCSSET,
			  (SDR_READ(PESDR2_RCSSET) & ~(1 << 24 | 1 << 16)) | 1 << 12);
		break;
	}
	mdelay(1000);

	switch (port) {
	case 0:
		val = SDR_READ(PESDR0_RCSSTS);
		break;
		
	case 1:
		val = SDR_READ(PESDR1_RCSSTS);
		break;
		
	case 2:
		val = SDR_READ(PESDR2_RCSSTS);
		break;
	}

	if (val & (1 << 20)) {
		printk(KERN_WARNING "PGRST for PCIE%d failed %08x\n",
				port, val);
		return -1;
	}

	/*
	 * Verify link is up
	 */
	val = 0;
	switch (port) {
	case 0:
		val = SDR_READ(PESDR0_LOOP);
		break;
	case 1:
		val = SDR_READ(PESDR1_LOOP);
		break;
	case 2:
		val = SDR_READ(PESDR2_LOOP);
		break;
	}
	if (!(val & 0x00001000)) {
		printk(KERN_INFO "PCIE: link is not up for port %d.\n", port);
		return -1;
	}

	/*
	 * Setup UTL registers - but only on revA!
	 * We use default settings for revB chip.
	 */
	if (!ppc440spe_revB())
		ppc440spe_setup_utl(port);

	/*
	 * We map PCI Express configuration access into the 512MB regions.
	 *
	 * NOTICE: revB is very strict about PLB real addressess and ranges to
	 * be mapped for config space; it seems to only work with d_nnnn_nnnn
	 * range (hangs the core upon config transaction attempts when set
	 * otherwise) while revA uses c_nnnn_nnnn.
	 *
	 * For revA:
	 *     PCIE0: 0xc_4000_0000
	 *     PCIE1: 0xc_8000_0000
	 *     PCIE2: 0xc_c000_0000
	 *
	 * For revB:
	 *     PCIE0: 0xd_0000_0000
	 *     PCIE1: 0xd_2000_0000
	 *     PCIE2: 0xd_4000_0000
	 */
	switch (port) {
	case 0:
		if (ppc440spe_revB()) {
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE0), 0x0000000d);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE0), 0x00000000);
		} else {
			/* revA */
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE0), 0x0000000c);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE0), 0x40000000);
		}
		mtdcr(DCRN_PEGPL_CFGMSK(PCIE0), 0xe0000001); /* 512MB region, valid */
		break;

	case 1:
		if (ppc440spe_revB()) {
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE1), 0x0000000d);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE1), 0x20000000);
		} else {
			/* revA */
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE1), 0x0000000c);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE1), 0x80000000);
		}
		mtdcr(DCRN_PEGPL_CFGMSK(PCIE1), 0xe0000001); /* 512MB region, valid */
		break;

	case 2:
		if (ppc440spe_revB()) {
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE2), 0x0000000d);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE2), 0x40000000);
		} else {
			/* revA */
			mtdcr(DCRN_PEGPL_CFGBAH(PCIE2), 0x0000000c);
			mtdcr(DCRN_PEGPL_CFGBAL(PCIE2), 0xc0000000);
		}
		mtdcr(DCRN_PEGPL_CFGMSK(PCIE2), 0xe0000001); /* 512MB region, valid */
		break;
	}

	/*
	 * Check for VC0 active and assert RDY.
	 */
	attempts = 10;
	switch (port) {
	case 0:
		while(!(SDR_READ(PESDR0_RCSSTS) & (1 << 16))) {
			if (!(attempts--)) {
				printk(KERN_WARNING "PCIE0: VC0 not active\n");
				return -1;
			}
			mdelay(1000);
		}
		SDR_WRITE(PESDR0_RCSSET, SDR_READ(PESDR0_RCSSET) | 1 << 20);
		break;
	case 1:
		while(!(SDR_READ(PESDR1_RCSSTS) & (1 << 16))) {
			if (!(attempts--)) {
				printk(KERN_WARNING "PCIE1: VC0 not active\n");
				return -1;
			}
			mdelay(1000);
		}

		SDR_WRITE(PESDR1_RCSSET, SDR_READ(PESDR1_RCSSET) | 1 << 20);
		break;
	case 2:
		while(!(SDR_READ(PESDR2_RCSSTS) & (1 << 16))) {
			if (!(attempts--)) {
				printk(KERN_WARNING "PCIE2: VC0 not active\n");
				return -1;
			}
			mdelay(1000);
		}

		SDR_WRITE(PESDR2_RCSSET, SDR_READ(PESDR2_RCSSET) | 1 << 20);
		break;
	}

	mdelay(100);

	return 0;
}

int ppc440spe_setup_pcie(struct pci_controller *hose, u32 port)
{
	void __iomem *mbase;
	int attempts = 0;

	if (ppc440spe_revB()) {
		/*
		 * NOTICE: revB is very strict about PLB real addressess and
		 * sizes to be mapped for config space; it hangs the core upon
		 * config transaction attempt if not set to 0xd_0010_0000,
		 * 0xd_2010_0000, 0xd_4010_0000 respectively.
		 */
		hose->cfg_data = ioremap64(0xd00100000ull + port * 0x20000000,
					0x400);

		/* for accessing Local Config space we need to set A[35] */
		mbase = ioremap64(0xd10000000ull + port * 0x20000000, 0x400);
	} else {
		/* revA */

		/*
		 * Map 16MB, which is enough for 4 bits of bus #
		 */
		hose->cfg_data = ioremap64(0xc40100000ull + port * 0x40000000,
					0x01000000);
		mbase = ioremap64(0xc50000000ull + port * 0x40000000, 0x1000);
	}

	hose->ops = &pcie_pci_ops;

#ifndef CONFIG_PCIE_ENDPOINT
	/*
	 * Set bus numbers on our root port
	 */
		out_8(mbase + PCI_PRIMARY_BUS, 0);
		out_8(mbase + PCI_SECONDARY_BUS, 1);
		out_8(mbase + PCI_SUBORDINATE_BUS, 1);
#endif
	/*
	 * Set up outbound translation to hose->mem_space from PLB
	 * addresses at an offset of 0xd_0000_0000.  We set the low
	 * bits of the mask to 11 to turn off splitting into 8
	 * subregions and to enable the outbound translation.
	 * POMs are set different for root and endpoints to 
	 * different window ranges fron inbound and out bound transactions.
	 */
#ifdef CONFIG_PCIE_ENDPOINT
	out_le32(mbase + PECFG_POM0LAH, 0x0000fff8);
	out_le32(mbase + PECFG_POM0LAL, 0x0001fff8);
#else
	out_le32(mbase + PECFG_POM0LAH, 0);
	out_le32(mbase + PECFG_POM0LAL, hose->mem_space.start );
#endif

	switch (port) {
	case 0:
		mtdcr(DCRN_PEGPL_OMR1BAH(PCIE0),  0x0000000d);
		mtdcr(DCRN_PEGPL_OMR1BAL(PCIE0),  hose->mem_space.start);
		mtdcr(DCRN_PEGPL_OMR1MSKH(PCIE0), 0x7fffffff);
		mtdcr(DCRN_PEGPL_OMR1MSKL(PCIE0),
		      ~(YUCCA_PCIE_MEM_SIZE - 1) | 3);
		break;
	case 1:
		mtdcr(DCRN_PEGPL_OMR1BAH(PCIE1),  0x0000000d);
		mtdcr(DCRN_PEGPL_OMR1BAL(PCIE1),  hose->mem_space.start);
		mtdcr(DCRN_PEGPL_OMR1MSKH(PCIE1), 0x7fffffff);
		mtdcr(DCRN_PEGPL_OMR1MSKL(PCIE1),
		      ~(YUCCA_PCIE_MEM_SIZE - 1) | 3);
		break;
	case 2:
		mtdcr(DCRN_PEGPL_OMR1BAH(PCIE2),  0x0000000d);
		mtdcr(DCRN_PEGPL_OMR1BAL(PCIE2),  hose->mem_space.start);
		mtdcr(DCRN_PEGPL_OMR1MSKH(PCIE2), 0x7fffffff);
		mtdcr(DCRN_PEGPL_OMR1MSKL(PCIE2),
		      ~(YUCCA_PCIE_MEM_SIZE - 1) | 3);
		break;
	}

	/* Set up 16GB inbound memory window at 0 */
#ifndef CONFIG_PCIE_ENDPOINT
	out_le32(mbase + PECFG_BAR0HMPA, 0x7fffffc);
	out_le32(mbase + PECFG_BAR0LMPA, 0);
#else
	out_le32(mbase + PECFG_BAR0HMPA, 0xfffffffc);
	out_le32(mbase + PECFG_BAR0LMPA, 0xffff0000);
#endif
	out_le32(mbase + PCI_BASE_ADDRESS_0, 0);
	out_le32(mbase + PCI_BASE_ADDRESS_1, 0);

	/*
	 *  SRAM location 0x4 0000 0000 is target for inbound transefers
	 */
	out_le32(mbase + PECFG_PIM0LAL, 0x00000000);
	out_le32(mbase + PECFG_PIM0LAH, 0x00000004);
	out_le32(mbase + PECFG_PIM1LAL, 0x00000000);
	out_le32(mbase + PECFG_PIM1LAH, 0x00000004);
	out_le32(mbase + PECFG_PIM01SAH, 0xffff0000);
	out_le32(mbase + PECFG_PIM01SAL, 0x00000000);
	out_le32(mbase + PECFG_PIMEN, 0x1);

	/* Enable I/O, Mem, and Busmaster cycles */
	out_le16(mbase + PCI_COMMAND,
		 in_le16(mbase + PCI_COMMAND) |
		 PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER);
	attempts = 10;
#ifndef CONFIG_PCIE_ENDPOINT
	printk(KERN_INFO"PCIE:%d successfully set as rootpoint\n",port);
	switch(port )
	{
		case 0:
			out_le16(mbase + 0x200,0xaaa0);
			out_le16(mbase + 0x202,0xbed0);
			break;
		case 1:
			out_le16(mbase + 0x200,0xaaa1);
			out_le16(mbase + 0x202,0xbed1);
			break;
		case 2:
			out_le16(mbase + 0x200,0xaaa2);
			out_le16(mbase + 0x202,0xbed2);
			break;
		default:
			out_le16(mbase + 0x200,0xaaa3);
			out_le16(mbase + 0x202,0xbed3);
	}
#else
	switch (port) {
	case 0:
		while(!(SDR_READ(PESDR0_RCSSTS) & (1 << 8))) {
			if (!(attempts--)) {
				printk(KERN_INFO "PCIE0: BMEN is not active\n");
				return -1;
			}
			mdelay(1000);
		}
		break;
	case 1:
		while(!(SDR_READ(PESDR1_RCSSTS) & (1 << 8))) {
			if (!(attempts--)) {
				printk(KERN_WARNING "PCIE1: BEMEN not active\n");
				return -1;
			}
			mdelay(1000);
		}
		break;
	case 2:
		while(!(SDR_READ(PESDR2_RCSSTS) & (1 << 8))) {
			if (!(attempts--)) {
				printk(KERN_WARNING "PCIE2: BMEN is not active\n");
				return -1;
			}
			mdelay(1000);
		}
		break;
	}
	printk(KERN_INFO"PCIE:%d successfully set as endpoint\n",port);
	switch(port)
	{
		case 0:
			out_le16(mbase + 0x200,0xeee0);
			out_le16(mbase + 0x202,0xfed0);
			break;
		case 1:
			out_le16(mbase + 0x200,0xeee1);
			out_le16(mbase + 0x202,0xfed1);
			break;
		case 2:
			out_le16(mbase + 0x200,0xeee2);
			out_le16(mbase + 0x202,0xfed2);
			break;
		default :
			out_le16(mbase + 0x200,0xeee3);
			out_le16(mbase + 0x202,0xfed3);
	}
#endif
	printk(KERN_INFO"vendor-id 0x%x\n",in_le16(mbase+0x0));
	printk(KERN_INFO"device-id 0x%x\n",in_le16(mbase+0x2));
	/*
	 * This code works as is with yucca plugged in another yucca board or any endpoint device 
	 * pugged in yucca board as root point device.
	 * If you want to change configuration according to your configuration . Here are few gotch's.
	 * There could be hangs due to different reasons
	 * 1 -- It could be that endpoint is not initialyzed. So always initialize the endpoint first.
	 * 2 -- May be your POM and BARs are not set properly
	 * 3 -- careful with masks which decide window sizes
	 * Here is the test code which used for testing sram read . This test dumps remote and local sram 
	 * locations which help in comparing visually.
	 * u32 *rsram,*lsram;
	 * int i=0;
	 * lsram = ioremap64(0x400000000 ,0x200); 
     * #ifndef CONFIG_PCIE_ENDPOINT
	 *    rsram = ioremap64(0xd00000000 + hose->mem_space.start,0x200);
	 *	 if(port == 1)
	 *		 for(i=0;i<20;i+=4) 
	 *			 printk(KERN_INFO"endp sram 0x%x root sram 0x%x\n",*(rsram+i),*(lsram+i));
     *#else
	 *    rsram = ioremap64(0xd00000000 + hose->mem_space.start,0x200);
	 *    for(i=0;i<20;i+=4) 
	 *		 printk(KERN_INFO"rootp sram 0x%x endp sram 0x%x\n",*(rsram+i),*(lsram+i));
     * #endif
    */

	iounmap(mbase);
	return 0;
}
