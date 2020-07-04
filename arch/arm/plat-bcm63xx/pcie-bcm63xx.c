#if defined(CONFIG_BCM_KF_ARM_BCM963XX)
/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
*/
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/export.h>
#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <board.h>
#include <pmc_pcie.h>
#include <pmc_drv.h>
#include <shared_utils.h>

#if 0
#define DPRINT(x...)        printk(x)
#define TRACE() 						DPRINT("%s\n",__FUNCTION__)
#define TRACE_READ(x...)		printk(x)
#define TRACE_WRITE(x...)		printk(x)
#else
#undef DPRINT
#define DPRINT(x...)
#define TRACE()
#define TRACE_READ(x...)
#define TRACE_WRITE(x...)
#endif

/*PCI-E */
#define BCM_BUS_PCIE_ROOT							0
#if defined(PCIEH) && defined(PCIEH_1)
#define NUM_CORE	2
#else
#define NUM_CORE	1
#endif

extern unsigned long getMemorySize(void);
static int bcm63xx_pcie_get_baraddrsize_index(void);
static struct pci_bus *bcm63xx_pcie_scan_bus(int nr, struct pci_sys_data *sys);
static int bcm63xx_pcie_map_irq(const struct pci_dev *dev, u8 slot, u8 pin);
static int bcm63xx_pcie_setup(int nr, struct pci_sys_data *sys);
static void bcm63xx_pcie_phy_mode_config(int index);

/* calculate size dynamically according to the RAM
 * 0x01 ... 64KB
 * 0x02 ... 128KB
 * 0x03 ... 256KB ...
 * 0x0d ... 256MB ...
 * 0x14 ... 32GB 
 */
static int bcm63xx_pcie_get_baraddrsize_index(void)
{
	unsigned long memsize; /*in K units*/
	int i = 0;
	
	memsize = ((getMemorySize()) >> 10);
	DPRINT("getMemorySize() = %lu\n", getMemorySize());

	for ( i = 0; i < PCIE_MISC_RC_BAR_CONFIG_LO_SIZE_MAX; i++) {
		if ((64 * (1 << i)) >= memsize) {
			break;
		}
	}
	
	DPRINT("PCIE_MISC.RC_BAR1_CONFIG_LO.size = 0x%x\n", i + 1);
	return (i + 1);
}

static void bcm63xx_pcie_pcie_reset(int index, bool PowerOn)
{
#if defined(PCIE3_CORE)
	u32 val = __raw_readl(MISC_BASE+offsetof(Misc,miscPCIECtrl));

	TRACE();
	if(PowerOn) {
	  val &= ~(1<<index);
	  __raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	  mdelay(10);
	  bcm63xx_pcie_phy_mode_config(index);
	  mdelay(10);			  
	  val |= (1<<index);
	  __raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	  mdelay(10);
	} else {
		val &= ~(1<<index);
		__raw_writel(val, MISC_BASE+offsetof(Misc,miscPCIECtrl));
	}
	/* this is a critical delay */
	mdelay(500);
#endif
}

/*
 * PCIe host controller registers
 * one entry per port
 */

static struct resource bcm63xx_pcie_owin[NUM_CORE] = {
	{
	.name = "bcm63xx pcie0",
	.start = PCIEH_0_MEM_BASE,
	.end   = PCIEH_0_MEM_BASE+PCIEH_0_MEM_SIZE-1,
	.flags = IORESOURCE_MEM,
	},
#if defined(PCIEH_1)	
	{
	.name = "bcm63xx pcie1",
	.start = PCIEH_1_MEM_BASE,
	.end   = PCIEH_1_MEM_BASE+PCIEH_1_MEM_SIZE-1,
	.flags = IORESOURCE_MEM,
	},
#endif	
};

/*
 * Per port control structure
 */
static struct bcm63xx_pcie_port {
	unsigned char * __iomem regs;
	struct resource *owin_res;
	unsigned int irq;
	struct hw_pci hw_pci;

	bool enabled;		// link-up
	bool link;		// link-up
	bool saved;		// pci-state saved
} bcm63xx_pcie_ports[NUM_CORE] = {
	{
	.regs = (unsigned char * __iomem)PCIE_0_BASE, /* this is mapped address */
	.owin_res = & bcm63xx_pcie_owin[0],
	.irq = INTERRUPT_ID_PCIE0,
	.hw_pci = {
		.domain 	= 0,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= bcm63xx_pcie_setup,
		.scan 		= bcm63xx_pcie_scan_bus,
		.map_irq 	= bcm63xx_pcie_map_irq,
		},
	.enabled = 0,
	.link = 0,
	},
#if defined(PCIEH_1)	
	{
	.regs = (unsigned char * __iomem)PCIE_1_BASE,
	.owin_res = & bcm63xx_pcie_owin[1],
	.irq = INTERRUPT_ID_PCIE1,
	.hw_pci = {
		.domain 	= 1,
		.swizzle 	= pci_std_swizzle,
		.nr_controllers = 1,
		.setup 		= bcm63xx_pcie_setup,
		.scan 		= bcm63xx_pcie_scan_bus,
		.map_irq 	= bcm63xx_pcie_map_irq,
		},
	.enabled = 0,
	.link = 0,	
	},
#endif	
};

/* 
  Function pcie_mdio_read (phyad, regad)

   Parameters:
     phyad ... MDIO PHY address (typically 0!)
     regad ... Register address in range 0-0x1f

   Description:
     Perform PCIE MDIO read on specified PHY (typically 0), and Register.
     Access is through an indirect command/status mechanism, and timeout
     is possible. If command is not immediately complete, which would
     be typically the case, one more attempt is made after a 1ms delay.

   Return: 16-bit data item or 0xdead on MDIO timeout
*/
static uint16 bcm63xx_pcie_mdio_read (struct bcm63xx_pcie_port *port, uint16 phyad, uint16 regad) 
{
    unsigned char * __iomem regs = port->regs;
    int timeout;
    uint32 data;
    uint16 retval;
    volatile PcieBlk1000Regs *RcDLReg;

    RcDLReg = (PcieBlk1000Regs*)(regs+PCIEH_BLK_1000_REGS);

    /* Bit-20=1 to initiate READ, bits 19:16 is the phyad, bits 4:0 is the regad */
    data = 0x100000;
    data = data |((phyad & 0xf)<<16);
    data = data |(regad & 0x1F);

    RcDLReg->mdioAddr = data;
    /* critical delay */
    udelay(1000);

    timeout = 2;
    while (timeout-- > 0) {
        data = RcDLReg->mdioRdData;
        /* Bit-31=1 is DONE */
        if (data & 0x80000000)
            break;
        timeout = timeout - 1;
        udelay(1000);
    }

    if (timeout == 0) {
        retval = 0xdead;
    }else 
        /* Bits 15:0 is read data*/
        retval = (data&0xffff);

    return retval;
}

/* 
 Function pcie_mdio_write (phyad, regad, wrdata)

   Parameters:
     phyad ... MDIO PHY address (typically 0!)
     regad  ... Register address in range 0-0x1f
     wrdata ... 16-bit write data

   Description:
     Perform PCIE MDIO write on specified PHY (typically 0), and Register.
     Access is through an indirect command/status mechanism, and timeout
     is possible. If command is not immediately complete, which would
     be typically the case, one more attempt is made after a 1ms delay.

   Return: 1 on success, 0 on timeout
*/
static int bcm63xx_pcie_mdio_write (struct bcm63xx_pcie_port *port, uint16 phyad, uint16 regad, uint16 wrdata)
{
    unsigned char * __iomem regs = port->regs;
    int timeout;
    uint32 data;
    volatile PcieBlk1000Regs *RcDLReg;
    
    RcDLReg = (PcieBlk1000Regs*)(regs+PCIEH_BLK_1000_REGS);

    /* bits 19:16 is the phyad, bits 4:0 is the regad */
    data = ((phyad & 0xf) << 16);
    data = data | (regad & 0x1F);

    RcDLReg->mdioAddr = data;
    udelay(1000);

    /* Bit-31=1 to initial the WRITE, bits 15:0 is the write data */
    data = 0x80000000;
    data = data | (wrdata & 0xFFFF);

    RcDLReg->mdioWrData = data;
    udelay(1000);

    /* Bit-31=0 when DONE */
    timeout = 2;
    while (timeout-- > 0) {

        data = RcDLReg->mdioWrData;

        /* CTRL1 Bit-31=1 is DONE */
        if ((data & 0x80000000) == 0 )
            break;

        timeout = timeout - 1;
        udelay(1000);
    }

    if (timeout == 0){
        return 0;
    } else 
        return 1;
}

static void bcm63xx_pcie_phy_mode_config(int index)
{
	struct bcm63xx_pcie_port* port;	
	port = &bcm63xx_pcie_ports[index];	

#if defined(RCAL_1UM_VERT)
	/*
	 * Rcal Calibration Timers
	 *   Block 0x1000, Register 1, bit 4(enable), and 3:0 (value)
	 */
	{
		int val = 0;
		uint16 data = 0; 
		if(GetRCalSetting(RCAL_1UM_VERT, &val)== kPMC_NO_ERROR) {
			printk("bcm63xx_pcie: setting resistor calibration value to 0x%x\n", val);
			bcm63xx_pcie_mdio_write(port, 0, 0x1f , 0x1000); 
			data = bcm63xx_pcie_mdio_read (port, 0, 1);
			data = ((data & 0xffe0) | (val & 0xf) | (1 << 4)); /*enable*/   		
			bcm63xx_pcie_mdio_write(port, 0, 1, data);
		}
	}
#endif

#if defined(PCIE3_CORE) 
	//printk("chipid:0x%x , chiprev:0x%x \n", kerSysGetChipId(), (UtilGetChipRev()));
	{
		printk("bcm63xx_pcie: applying serdes parameters\n");
		/*
		 * VCO Calibration Timers
		 * Workaround: 
		 * Block 0x3000, Register 0xB = 0x40
		 * Block 0x3000, Register 0xD = 7
		 * Notes: 
		 * -Fixed in 63148A0, 63381B0, 63138B0 but ok to write anyway
		 */ 
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x3000);
		bcm63xx_pcie_mdio_read (port, 0, 0x1f);  /* just to exericise the read */
		bcm63xx_pcie_mdio_write(port, 0, 0xB, 0x40);
		bcm63xx_pcie_mdio_write(port, 0, 0xD, 7);      

		/*	
		 * Reference clock output level
		 * Workaround:
		 * Block 0x2200, Register 3 = 0xaba4
		 * Note: 
		 * -Fixed in 63148A0, 63381B0, 63138B0 but ok to write anyway
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x2200);
		bcm63xx_pcie_mdio_write(port, 0, 3, 0xaba4);    

		/* 
		 * Tx Pre-emphasis
		 * Workaround:
		 * Block 0x4000, Register 0 = 0x1d20  // Gen1
		 * Block 0x4000, Register 1 = 0x12cd  // Gen1
		 * Block 0x4000, Register 3 = 0x0016  // Gen1, Gen2
		 * Block 0x4000, Register 4 = 0x5920  // Gen2
		 * Block 0x4000, Register 5 = 0x13cd  // Gen2
		 * Notes: 
		 * -Fixed in 63148A0, 63381B0, 63138B0 but ok to write anyway
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x4000);
		bcm63xx_pcie_mdio_write(port, 0, 0, 0x1D20);    
		bcm63xx_pcie_mdio_write(port, 0, 1, 0x12CD);
		bcm63xx_pcie_mdio_write(port, 0, 3, 0x0016);
		bcm63xx_pcie_mdio_write(port, 0, 4, 0x5920);
		bcm63xx_pcie_mdio_write(port, 0, 5, 0x13CD);

		/*
		 * Rx Signal Detect
		 * Workaround:
		 * Block 0x6000, Register 5 = 0x2c0d 
		 * Notes:
		 * -Fixed in 63148A0, 63381B0, 63138B0 but ok to write anyway
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x6000);
		bcm63xx_pcie_mdio_write(port, 0, 0x5, 0x2C0D);		

		/*
		 * Rx Jitter Tolerance
		 * Workaround:
		 * Block 0x7300, Register 3 = 0x190  // Gen1
		 * Block 0x7300, Register 9 = 0x194  // Gen2
		 * Notes:
		 * -Gen1 setting 63148A0, 63381B0, 63138B0 but ok to write anyway
		 * -Gen2 setting only in latest SerDes RTL  / future tapeouts
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x7300);
		bcm63xx_pcie_mdio_write(port, 0, 3, 0x190);
		bcm63xx_pcie_mdio_write(port, 0, 9, 0x194);

		/* 
		 * Gen2 Rx Equalizer
		 * Workaround:
		 * Block 0x6000 Register 7 = 0xf0c8  // Gen2
		 * Notes:
		 * -New setting only in latest SerDes RTL / future tapeouts
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x6000);
		bcm63xx_pcie_mdio_write(port, 0, 7, 0xf0c8);

		/*
		 * SSC Parameters
		 * Workaround:
		 * Block 0x1100, Register 0xA = 0xea3c  
		 * Block 0x1100, Register 0xB = 0x04e7
		 * Block 0x1100, Register 0xC = 0x0039 
		 * Block 0x2200, Register 5 = 0x5044    // VCO parameters for fractional mode, -175ppm
		 * Block 0x2200, Register 6 = 0xfef1    // VCO parameters for fractional mode, -175ppm
		 * Block 0x2200, Register 7 = 0xe818    // VCO parameters for fractional mode, -175ppm
		 * Notes:
		 * -Only need to apply these fixes when enabling Spread Spectrum Clocking (SSC), which would likely be a flash option
		 * -Block 0x1100 fixed in 63148A0, 63381B0, 63138B0 but ok to write anyway
		 */

		/*
		 * EP Mode PLL Bandwidth and Peaking
		 * Workaround:
		 * Block 0x2100, Register 0 = 0x5174
		 * Block 0x2100, Register 4 = 0x6023
		 * Notes:
		 * -Only needed for EP mode, but ok to write in RC mode too
		 * -New setting only in latest SerDes RTL / future tapeouts
		 */
		bcm63xx_pcie_mdio_write(port, 0, 0x1f, 0x2100);
		bcm63xx_pcie_mdio_write(port, 0, 0, 0x5174);
		bcm63xx_pcie_mdio_write(port, 0, 4, 0x6023);
	}
#endif
    return;
}

static struct bcm63xx_pcie_port *bcm63xx_pcie_bus2port(struct pci_bus *bus)
{
	struct pci_sys_data *sys = bus->sysdata;
	return sys->private_data;
}

static void bcm63xx_pcie_config_select(unsigned char * __iomem regs, u32 bus_no, u32 dev_no, u32 func_no)
{
	/* set device bus/func/func */
#if defined(UBUS2_PCIE)
  __raw_writel((bus_no<<PCIE_EXT_CFG_BUS_NUM_SHIFT)|(dev_no <<PCIE_EXT_CFG_DEV_NUM_SHIFT)|(func_no<<PCIE_EXT_CFG_FUNC_NUM_SHIFT),
  			regs+PCIEH_PCIE_EXT_CFG_REGS+offsetof(PcieExtCfgRegs,index));

#endif    
}
    
static u32 __iomem bcm63xx_pcie_config_offset_aligned(int bus_no, int where)
{
   if(bus_no == BCM_BUS_PCIE_ROOT ) {
        /* access offset 0 */
        return where&~3;
    } else {
        /* access offset */
        return (PCIEH_DEV_OFFSET+where)&~3;
    }	
}


static int bcm63xx_pcie_valid_config(int bus_no, int dev_no)
{
	/* to tune more ?*/
    if (bus_no == BCM_BUS_PCIE_ROOT ) {
        /* bridge */
        return (dev_no == 0); /*otherwise will loop for the rest of the device*/
    } else {
       	return (dev_no == 0); /*otherwise will loop for the rest of the device*/
    }
    return 0;	
}	

/*
 * PCIe config cycles are done by programming the PCIE_CONF_ADDR register
 * and then reading the PCIE_CONF_DATA register. Need to make sure these
 * transactions are atomic.
 */

static int bcm63xx_pciebios_read(struct pci_bus *bus, u32 devfn, int where,
			int size, u32 *val)
{
	struct bcm63xx_pcie_port *port = bcm63xx_pcie_bus2port(bus);
	unsigned char * __iomem regs = port->regs;
	u32 __iomem offset;
	int busno = bus->number;
	int slot = PCI_SLOT(devfn);
	int fn = PCI_FUNC(devfn);
  u32 data;

	TRACE();

  TRACE_READ("R device (bus)%d/(slot)%d/func(%d) at %d size %d, val=0x%x\n", busno, slot, fn, where, size, *val);		
	if (bcm63xx_pcie_valid_config(busno, PCI_SLOT(devfn)) == 0){
		*val = -1;
		TRACE_READ("not valid config\n");
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
		
	offset = (u32)regs + bcm63xx_pcie_config_offset_aligned(busno,where);
	
	if (((size == 2) && (where & 1)) ||((size == 4) && (where & 3))) {
		 BUG_ON(1);
		 return PCIBIOS_BAD_REGISTER_NUMBER;
	}

	bcm63xx_pcie_config_select(regs, busno,slot,fn);

  data = __raw_readl(offset);

  TRACE_READ("reading 0x%x @ 0x%x\n", data, offset);
    
  if (data == 0xdeaddead) {
		*val = -1;
		return PCIBIOS_DEVICE_NOT_FOUND;
  }
  if (size == 1)
     *val = (data >> ((where & 3) << 3)) & 0xff;
  else if (size == 2)
     *val = (data >> ((where & 3) << 3)) & 0xffff;
  else
     *val = data;

  TRACE_READ("val= 0x%x\n", *val);        	
  return PCIBIOS_SUCCESSFUL;
}
	

static int bcm63xx_pciebios_write(struct pci_bus *bus, u32 devfn,
			int where, int size, u32 val)
{
	struct bcm63xx_pcie_port *port = bcm63xx_pcie_bus2port(bus);
	unsigned char * __iomem regs = port->regs;
	u32 __iomem offset;
	int busno = bus->number;
	int slot = PCI_SLOT(devfn);
	int fn = PCI_FUNC(devfn);
  u32 data;
	
	TRACE();

  TRACE_WRITE("W device (bus)%d/(slot)%d/func(%d) at %d size %d, val=0x%x\n", busno, slot, fn, where, size, val);
	if (bcm63xx_pcie_valid_config(busno, PCI_SLOT(devfn)) == 0)
	{
		TRACE_WRITE("not valid config\n");
		return PCIBIOS_DEVICE_NOT_FOUND;
	}
		
	bcm63xx_pcie_config_select(regs, busno,slot,fn);
	offset = (u32)regs + bcm63xx_pcie_config_offset_aligned(busno,where);
	
	if (((size == 2) && (where & 1)) ||((size == 4) && (where & 3))) {
		 BUG_ON(1);
		 return PCIBIOS_BAD_REGISTER_NUMBER;
	}

  data = __raw_readl(offset);
  DPRINT("reading 0x%x @ 0x%x\n", data, offset);
  if (size == 1)
		data = (data & ~(0xff << ((where & 3) << 3))) | (val << ((where & 3) << 3));
	else if (size == 2)
		data = (data & ~(0xffff << ((where & 3) << 3))) | (val << ((where & 3) << 3));
  else
  	data = val;

  TRACE_WRITE("writing 0x%x @ 0x%x\n", data, offset);
	__raw_writel(data, offset);

  return PCIBIOS_SUCCESSFUL;
}


static struct pci_ops bcm63xx_pcie_ops = {
	.read   = bcm63xx_pciebios_read,
	.write  = bcm63xx_pciebios_write
};

static struct bcm63xx_pcie_port *bcm63xx_pcie_sysdata2port(struct pci_sys_data *sysdata)
{
	unsigned port;
	TRACE();
	port = sysdata->domain;
	BUG_ON(port >= ARRAY_SIZE(bcm63xx_pcie_ports));
	return & bcm63xx_pcie_ports[port];
}

static void bcm63xx_hw_pcie_setup(struct bcm63xx_pcie_port *port)
{
#if defined(UBUS2_PCIE)
	unsigned char * __iomem regs = port->regs;
	TRACE();
		
	__raw_writel(PCIE_CPU_INTR1_PCIE_INTD_CPU_INTR | PCIE_CPU_INTR1_PCIE_INTC_CPU_INTR |PCIE_CPU_INTR1_PCIE_INTB_CPU_INTR |PCIE_CPU_INTR1_PCIE_INTA_CPU_INTR,
				regs+PCIEH_CPU_INTR1_REGS+offsetof(PcieCpuL1Intr1Regs,maskClear));
				/*&((PcieCpuL1Intr1Regs*)(regs+PCIEH_CPU_INTR1_REGS))->maskClear);*/
            
  /* setup outgoing mem resource window */
	__raw_writel((port->owin_res->end & PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_LIMIT_MASK)
    	 	|((port->owin_res->start >> PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_LIMIT_SHIFT) << PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_LIMIT_BASE_SHIFT),
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,cpu_2_pcie_mem_win0_base_limit));
 				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->cpu_2_pcie_mem_win0_base_limit);
    	 																								
  __raw_writel((port->owin_res->start & PCIE_MISC_CPU_2_PCI_MEM_WIN_LO_BASE_ADDR_MASK),
  			regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,cpu_2_pcie_mem_win0_lo));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->cpu_2_pcie_mem_win0_lo);
   	 
  /* setup incoming DDR memory BAR(1) */
#if 0 //defined(CONFIG_CPU_LITTLE_ENDIAN)
	__raw_writel(PCIE_RC_CFG_VENDOR_SPECIFIC_REG1_ENDIAN_MODE_BYTE_ALIGN,
				regs+PCIEH_RC_CFG_VENDOR_REGS+offsetof(PcieRcCfgVendorRegs,specificReg1));
				//&((PcieRcCfgVendorRegs*)(regs + PCIEH_RC_CFG_VENDOR_REGS))->specificReg1);
#endif
	__raw_writel((DDR_UBUS_ADDRESS_BASE & PCIE_MISC_RC_BAR_CONFIG_LO_MATCH_ADDRESS_MASK)| bcm63xx_pcie_get_baraddrsize_index(),
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,rc_bar1_config_lo));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->rc_bar1_config_lo);


	__raw_writel(PCIE_MISC_UBUS_BAR_CONFIG_ACCESS_EN,
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,ubus_bar1_config_remap));
				//&((PcieMiscRegs*)(regs+PCIEH_MISC_REGS))->ubus_bar1_config_remap);

  /* set device bus/func/func -no need*/
  /* setup class code, as bridge */       
  __raw_writel((__raw_readl(regs+PCIEH_BLK_428_REGS+offsetof(PcieBlk428Regs,idVal3))& PCIE_IP_BLK428_ID_VAL3_REVISION_ID_MASK) | (PCI_CLASS_BRIDGE_PCI << 8),
  			regs+PCIEH_BLK_428_REGS+offsetof(PcieBlk428Regs,idVal3));
 
  /* disable bar0 size -no need*/

	/* disable data bus error for enumeration */
	__raw_writel(__raw_readl(regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl))|PCIE_MISC_CTRL_CFG_READ_UR_MODE,
				regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl));  
#endif

#if defined(PCIE3_CORE)
	/* Misc performance addition */
	__raw_writel(__raw_readl(regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl))
								|PCIE_MISC_CTRL_MAX_BURST_SIZE_128B
								|PCIE_MISC_CTRL_BURST_ALIGN
								|PCIE_MISC_CTRL_PCIE_IN_WR_COMBINE
								|PCIE_MISC_CTRL_PCIE_RCB_MPS_MODE
								|PCIE_MISC_CTRL_PCIE_RCB_64B_MODE,
							  regs+PCIEH_MISC_REGS+offsetof(PcieMiscRegs,misc_ctrl));
#endif
}

static int bcm63xx_pcie_setup(int nr, struct pci_sys_data *sys)
{
	struct bcm63xx_pcie_port *port = bcm63xx_pcie_sysdata2port(sys);

	TRACE();
	BUG_ON(request_resource(&iomem_resource, port->owin_res));

 	pci_add_resource_offset(&sys->resources, port->owin_res, sys->mem_offset); 
	/* pcibios_init_hw will add resource offset */
	sys->private_data = port;
	bcm63xx_hw_pcie_setup(port);

	return 1;
}

static struct pci_bus *
bcm63xx_pcie_scan_bus(int nr, struct pci_sys_data *sys)
{
	TRACE();	
	return pci_scan_root_bus(NULL, sys->busnr, &bcm63xx_pcie_ops, sys, &sys->resources);
}


static int bcm63xx_pcie_map_irq(const struct pci_dev *dev, u8 slot,
	u8 pin)
{
	struct bcm63xx_pcie_port *port = bcm63xx_pcie_bus2port(dev->bus);
	TRACE();  
	return port->irq;
}

#define BCM4360_D11AC_SROMLESS_ID	0x4360
#define BCM4360_D11AC_ID	0x43a0
#define BCM4360_D11AC2G_ID	0x43a1
#define BCM4360_D11AC5G_ID	0x43a2
#define BCM4352_D11AC_ID	0x43b1
#define BCM4352_D11AC2G_ID	0x43b2
#define BCM4352_D11AC5G_ID	0x43b3
#define BCM43602_CHIP_ID	0xaa52
#define BCM43602_D11AC_ID	0x43ba
#define BCM43602_D11AC2G_ID	0x43bb
#define BCM43602_D11AC5G_ID	0x43bc

#define IS_DEV_AC3X3(d) (((d) == BCM4360_D11AC_ID) || \
	                 ((d) == BCM4360_D11AC2G_ID) || \
	                 ((d) == BCM4360_D11AC5G_ID) || \
	                 ((d) == BCM4360_D11AC_SROMLESS_ID) || \
	                 ((d) == BCM43602_D11AC_ID) || \
	                 ((d) == BCM43602_D11AC2G_ID) || \
	                 ((d) == BCM43602_D11AC5G_ID) || \
	                 ((d) == BCM43602_CHIP_ID))

#define IS_DEV_AC2X2(d) (((d) == BCM4352_D11AC_ID) ||	\
	                 ((d) == BCM4352_D11AC2G_ID) || \
	                 ((d) == BCM4352_D11AC5G_ID))

static void bcm63xx_pcie_fixup_mps(struct pci_dev *dev)
{
#if defined(PCIE3_CORE)	
	if (dev->vendor == 0x14e4) {
		if (IS_DEV_AC3X3(dev->device) || IS_DEV_AC2X2(dev->device)) {
			/* set 4360 specific tunables
			 * wlan driver will set mps but cannot populate to RC, 
			 * fake/hijack it so linux sw can sync it up
			 */
			dev->pcie_mpss = 2;
		}
	}
#endif	
}

static void bcm63xx_pcie_fixup_final(struct pci_dev *dev)
{
#if defined(PCIE3_CORE)
	pcie_bus_config = PCIE_BUS_SAFE;

	bcm63xx_pcie_fixup_mps(dev);

	/* sync-up mps */
	if (dev->bus && dev->bus->self) {
		pcie_bus_configure_settings(dev->bus, dev->bus->self->pcie_mpss);
	}
#endif	
}
DECLARE_PCI_FIXUP_FINAL(PCI_ANY_ID, PCI_ANY_ID, bcm63xx_pcie_fixup_final);


static int bcm63xx_pcie_link_up(int index)
{		
	struct bcm63xx_pcie_port* port=&bcm63xx_pcie_ports[index];
	TRACE();

	port->link = __raw_readl(port->regs+PCIEH_BLK_1000_REGS+offsetof(PcieBlk1000Regs,dlStatus)) & PCIE_IP_BLK1000_DL_STATUS_PHYLINKUP_MASK;
	if(port->link) {
		printk("PCIE port %d link-up\n", index);
	}
		
	return port->link;
}

/* save pci configuration for all devices in domain */
static void bcm63xx_pcie_domain_save(int domain)
{
	struct pci_dev *dev = 0;

	for_each_pci_dev(dev) {
		if (pci_domain_nr(dev->bus) == domain) {
			pci_save_state(dev);
		}
	}
}

/* restore pci configuration for all devices in domain */
static void bcm63xx_pcie_domain_restore(int domain)
{
	struct pci_dev *dev = 0;

	for_each_pci_dev(dev) {
		if (pci_domain_nr(dev->bus) == domain) {
			/* expected all to have state saved */
			if (!dev->state_saved) {
				printk("%s %x:%x %s\n", __func__,
					dev->vendor, dev->device, "not saved");
				continue;
			}
			pci_restore_state(dev);
			dev->state_saved = TRUE; // mark state as still valid
			pci_reenable_device(dev);
		}
	}
}

/* pcie reinit without pci_common_init */
void bcm63xx_pcie_aloha(int hello)
{
	int i;

	if (!hello) {
		/* goodbye */
		for (i = 0; i < NUM_CORE; i++) {
			bcm63xx_pcie_pcie_reset(i, FALSE);
			pmc_pcie_power_down(i);
		}
		return;
	}

	/* pcie ports, domain 0/1 */
	for (i = 0; i < NUM_CORE; i++) {
		struct bcm63xx_pcie_port *port = &bcm63xx_pcie_ports[i];

		/* skip ports with link down */
		if (!port->enabled)
			continue;

		if (!port->saved) {
			/* first time: port powered, link up */
			port->saved = TRUE;

			/* save pci configuration for domain devices */
			bcm63xx_pcie_domain_save(i);
		} else {
			/* power port and check for link */
			pmc_pcie_power_up(i);
			bcm63xx_pcie_pcie_reset(i, TRUE);
			port->enabled = bcm63xx_pcie_link_up(i);
			if (!port->enabled) {
				/* power off ports without link */
				bcm63xx_pcie_pcie_reset(i, FALSE);
				pmc_pcie_power_down(i);
			} else {
				/* redo setup (previously done during bus scan) */
				bcm63xx_hw_pcie_setup(port);

				/* restore pci configuration for domain devices */
				bcm63xx_pcie_domain_restore(i);
			}
		}
	}
}
EXPORT_SYMBOL(bcm63xx_pcie_aloha);

static int bcm63xx_pcie_init(void)
{
	int i;
	bool shutdown;
	TRACE();
  
	/* pcie ports, domain 1/2 */
	for (i = 0; i < NUM_CORE; i++) {
		shutdown = TRUE;
		if (kerSysGetPciePortEnable(i)) {
			pmc_pcie_power_up(i);
			bcm63xx_pcie_pcie_reset(i, TRUE);
			bcm63xx_pcie_ports[i].enabled =1;			
			if(bcm63xx_pcie_link_up(i)) {
				pci_common_init(&(bcm63xx_pcie_ports[i].hw_pci));
				shutdown = FALSE;
	  	}
	  }
		if(shutdown) {
			/* power off ports without link */
			printk("PCIE port %d power-down\n", i);
			bcm63xx_pcie_pcie_reset(i, FALSE);
			pmc_pcie_power_down(i);
		}
	}
	return 0;
}
subsys_initcall(bcm63xx_pcie_init);
#endif
