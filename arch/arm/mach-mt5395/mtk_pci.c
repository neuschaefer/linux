/*
 * arch/arm/mach-mt5365/mtk_pci.c
 *
 * PCI and PCIe functions for MTK System On Chip
 *
 * Maintainer: Max Liao <max.liao@mediatek.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */


#ifdef CONFIG_PCI

#include "mtk_pci.h"
#include <linux/delay.h>	/* for mdelay() */

static int MTK_pcie_rd_conf(struct pci_bus *bus, u32 devfn, int where,int size, u32 * val);

static int MTK_pcie_wr_conf(struct pci_bus *bus, u32 devfn, int where, int size, u32 val);

static struct pci_ops pcie_ops = 
{
    .read = MTK_pcie_rd_conf,
    .write = MTK_pcie_wr_conf,
};

static u32 _pci_u4ReadVal;
static u32 _pci_u4WriteVal;

static DEFINE_SPINLOCK(mtk_pcie_lock);

static int _pcie_rd_conf(u32 base, u32 bus, u32 devfn, int where, int size, u32 * val)
{   
    u32 u4Val;
    u32 u4ByteEnable;
    unsigned long flags;
    dma_addr_t dma_address;
    
    LOG("PCIE-R: bus=%d, dev=%d, func=%d, reg=0x%04X, size=%d, ", 
        bus, PCI_SLOT(devfn), PCI_FUNC(devfn), where, size);

    spin_lock_irqsave(&mtk_pcie_lock, flags);

    dma_address = dma_map_single(NULL, &_pci_u4ReadVal, sizeof(u32), DMA_FROM_DEVICE);

    writel(0, base + PLDA_RDMA_PCI_ADDR1);
    writel(CONF_BUS(bus) |CONF_DEV(PCI_SLOT(devfn)) |
               CONF_FUNC(PCI_FUNC(devfn)) |CONF_REG(where), base + PLDA_RDMA_PCI_ADDR0);
    writel((u32)dma_address, base + PLDA_RDMA_AHB_ADDR);
   
    if (size == 1)
    {
	u4ByteEnable = CONF_BYTE1;
    }
    else if (size == 2)
    {
	u4ByteEnable = CONF_BYTE2;
    }
    else if (size == 3)
    {
	u4ByteEnable = CONF_BYTE3;
    }
    else
    {
	u4ByteEnable = CONF_BYTE4;
    }

    u4ByteEnable <<= (where & 0x3);

    writel(CONF_SIZE | u4ByteEnable |CONF_RCMD | CONF_START, base + PLDA_RDMA_CONTROL);
    
    do
    {
        u4Val = readl(base + PLDA_RDMA_CONTROL);
        u4Val &= CONF_START;
    }while(u4Val == CONF_START);

    dma_unmap_single(NULL, dma_address, sizeof(u32), DMA_FROM_DEVICE);       
    
    if (size == 1)
    {
	*val = (_pci_u4ReadVal >> (8 * (where & 3))) & 0xFF;
    }
    else if (size == 2)
    {
	*val = (_pci_u4ReadVal >> (8 * (where & 3))) & 0xFFFF;
    }
    else if (size == 3)
    {
	*val = (_pci_u4ReadVal >> (8 * (where & 3))) & 0xFFFFFF;
    }
    else
    {
        *val = _pci_u4ReadVal;
    }

    LOG("val=0x%08X.\n", *val);

    spin_unlock_irqrestore(&mtk_pcie_lock, flags);
    
    return PCIBIOS_SUCCESSFUL;
}

static int _pcie_wr_conf(u32 base, u32 bus, u32 devfn, int where, int size, u32 val)
{
    u32 u4Val;
    u32 u4ByteEnable;
    unsigned long flags;
    dma_addr_t dma_address;
    
    LOG("PCIE-W: bus=%d, dev=%d, func=%d, reg=0x%04X, size=%d, val=0x%08X.\n", 
        bus, PCI_SLOT(devfn), PCI_FUNC(devfn), where, size, val);

    spin_lock_irqsave(&mtk_pcie_lock, flags);

    _pci_u4WriteVal = val << (8 * (where & 3));
    dma_address = dma_map_single(NULL, &_pci_u4WriteVal, sizeof(u32), DMA_TO_DEVICE);
    writel(0, base + PLDA_WDMA_PCI_ADDR1);
    writel(CONF_BUS(bus) |CONF_DEV(PCI_SLOT(devfn)) |
               CONF_FUNC(PCI_FUNC(devfn)) |CONF_REG(where), base + PLDA_WDMA_PCI_ADDR0);
    writel((u32)dma_address, base + PLDA_WDMA_AHB_ADDR);

    if (size == 1)
    {
	u4ByteEnable = CONF_BYTE1;
    }
    else if (size == 2)
    {
	u4ByteEnable = CONF_BYTE2;
    }
    else if (size == 3)
    {
	u4ByteEnable = CONF_BYTE3;
    }
    else
    {
	u4ByteEnable = CONF_BYTE4;
    }

    u4ByteEnable <<= (where & 0x3);

    writel(CONF_SIZE | u4ByteEnable |CONF_WCMD | CONF_START, base + PLDA_WDMA_CONTROL);

    do
    {
        u4Val = readl(base + PLDA_WDMA_CONTROL);
        u4Val &= CONF_START;
    }while(u4Val == CONF_START);

    dma_unmap_single(NULL, dma_address, sizeof(u32), DMA_TO_DEVICE);

    spin_unlock_irqrestore(&mtk_pcie_lock, flags);
        
    return PCIBIOS_SUCCESSFUL;
}

static int _pcie_valid_config(int bus, int dev)
{
    // Root only support one slot.
    if (bus == 0)
    {
        return (dev == 0) ? 1:0;
    }

    return 1;
}

static irqreturn_t _pcie_irq (int irq, void *irqtag)
{
    unsigned long flags;
    u32 regval32;
    u32 u4IntStatus;
    int u4EventBit;
    u32 u4Val;

    if (irq != VECTOR_PCIE)
    {
        return IRQ_NONE;
    }
        
    /* IRQF_DISABLED doesn't work correctly with shared IRQs
     * when the first handler doesn't use it.  So let's just
     * assume it's never used.
     */
    local_irq_save(flags);

    // PCI-Express handle local interrupt status clear.
    regval32 = readl(PCIE_VIRT + PLDA_ISTATUS);

    u4IntStatus = regval32;
    if (INT_INTA & u4IntStatus)
    {
        generic_handle_irq(IRQ_PCIE_INTA);  
        u4IntStatus &= ~INT_INTA;
    }

    if (INT_INTB & u4IntStatus)
    {
        generic_handle_irq(IRQ_PCIE_INTB);  
        u4IntStatus &= ~INT_INTB;
    }
    
    if (INT_INTC & u4IntStatus)
    {
        generic_handle_irq(IRQ_PCIE_INTC);  
        u4IntStatus &= ~INT_INTC;        
    }
    
    if (INT_INTD & u4IntStatus)
    {
        generic_handle_irq(IRQ_PCIE_INTD);  
        u4IntStatus &= ~INT_INTD;        
    }

    if (INT_AER & u4IntStatus)
    {
        generic_handle_irq(IRQ_PCIE_AER);
        u4IntStatus &= ~INT_AER;                
    }

    if (INT_MSI & u4IntStatus)
    {    
#ifdef CONFIG_PCI_MSI       
        while ((u4Val = readl(PCIE_VIRT + PLDA_IMSISTATUS)) > 0)
        {
            u4EventBit = find_first_bit((const unsigned long *)&u4Val, 32);
            /* write back to clear bit */
            writel((1 << u4EventBit), PCIE_VIRT + PLDA_IMSISTATUS);
            generic_handle_irq(IRQ_PCIE_MSI + u4EventBit);
            u4Val = readl(PCIE_VIRT + PLDA_IMSISTATUS);
        }
#endif
        u4IntStatus &= ~INT_MSI;                
    }

    // Other Error.
    if (u4IntStatus & INT_ALL_ERR)
    {
        printk("PCIE Interrupt Error = 0x%08X.\n", regval32);
        PCI_ASSERT((u4IntStatus & INT_ALL_ERR) != 0);
    }
        
    writel(regval32, PCIE_VIRT + PLDA_ISTATUS);    

    local_irq_restore(flags);

    return IRQ_HANDLED;
}

static void __devinit rc_pci_fixup(struct pci_dev *dev)
{
    int i;

    LOG("rc_pci_fixup.\n");

    /*
     * Prevent enumeration of root complex.
     */
    if (dev->bus->parent == NULL && dev->devfn == 0)
    {

	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++)
	{
	    dev->resource[i].start = 0;
	    dev->resource[i].end = 0;
	    dev->resource[i].flags = 0;
	}
    }
}

DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_MEDIATEK, PCI_DEVICE_ID_MEDIATEK_MT5396, rc_pci_fixup);

static int __init MTK_pcie_setup(struct pci_sys_data *sys)
{
    struct resource *res;
    unsigned long flags;
    u32 regval32;
    u8 regval8;

    spin_lock_irqsave(&mtk_pcie_lock, flags);

    /* Adjust TX/RX data latch edge for bitfile1103 */
    writel(0x10, PCIE_PHY_BASE + PCIE_TX_RX_PHASE_SEL);

    /* Active MAC/PHY for bitfile1103 */
    writel(0xF, PCIE_PHY_BASE + PCIE_RESET_CTRL);

    /* Delay for link setup */
    mdelay(100);
    
    /* Check link up from phy register. 
         Note: device must insert on pci-express. 
         No device inserted will fail at checking link up status.
    */
    regval8 = readb(PCIE_PHY_BASE + PCIE_LTSSM_OFFSET) & 0x1F;    
    regval32 = readl(PCIE_VIRT + PLDA_INFO);

    if (regval32 != 0x0152)
    {
        printk("PCI-Express id check fail, status = 0x%04X.\n", regval32);
        return 0;
    }

#if 1

    /* LTSSM = 0x0F means L0 state. */
    if (regval8 != 0x0F)
    {
        printk("PCI-Express not Link up, status = 0x%02X.\n", regval8);
        return 0;        
    }

#else
//++jip test --> Used to find correct setting
    u8 PHYdelay=0, i, j;

    for(j=0; j<=0xf; j++)
    {
    // set BK here to set PHY reg 2, from 0x00-->0xff
    	printk("\n*******PHYdelay = 0x%02X.\n", PHYdelay);
    	for(i=0; i<=0xf; i++)
    	{
    		//write clock combinatination
    		writel(i<<4, PCIE_PHY_BASE + PCIE_TX_RX_PHASE_SEL);
    		mdelay(100);    		
    		writel(0xF, PCIE_PHY_BASE + PCIE_RESET_CTRL);
    		printk("TX RX PHASE sel = 0x%04X.\n", i<<4);
    		//read link status
    		mdelay(100);
    		regval8 = readb(PCIE_PHY_BASE + PCIE_LTSSM_OFFSET) & 0x1F;			
    		//print message
    		printk("Link status, status = 0x%04X.\n", regval8);
    		writel(0x0, PCIE_PHY_BASE + PCIE_RESET_CTRL);
    		mdelay(100);    		
    	}
    	PHYdelay += 0x11;
    }
    writel(0xF, PCIE_PHY_BASE + PCIE_RESET_CTRL);	
//++jip test --> To find correct setting
#endif

    // Init vendor/device id.
    writel(0x539614C3, PCIE_VIRT + PLDA_DV);
    writel(0x539614C3, PCIE_VIRT + PLDA_SUB);
    writel((PCI_CLASS_BRIDGE_PCI << 16), PCIE_VIRT + PLDA_CREV);    

    // AHB-PCI: window 0: from virtual 0xF020,0000 ~ 0xF03F,FFFF, size = 2Mbytes.
    writel(0xF0200055, PCIE_VIRT + PLDA_AHBPCI0_BASE0);
    writel(0x00000000, PCIE_VIRT + PLDA_AHBPCI0_BASE1);
    
    // AHB-PCI: window 1: reserve for future used.
    writel(0x00000000, PCIE_VIRT + PLDA_AHBPCI1_BASE0);
    writel(0x00000000, PCIE_VIRT + PLDA_AHBPCI1_BASE1);

    // PCI-AHB: window 0: from physical 0 ~ 0x7FFF,FFFF, size = 0x8000,0000
    writel(0x000000DF, PCIE_VIRT + PLDA_PCIAHB0_BASE);
    // PCI-AHB: window 1: reserve for future used.
    writel(0x00000000, PCIE_VIRT + PLDA_PCIAHB1_BASE);

    // MSI addresss configure.
    writel(PLDA_MSI_ADDR_SETTING, PCIE_VIRT + PLDA_IMSIADDR);

    // Clear local processor interrupts.
    regval32 = readl(PCIE_VIRT + PLDA_ISTATUS);
    writel(regval32, PCIE_VIRT + PLDA_ISTATUS);    

    // Turn on interrupt mask.
    regval32 = INT_INTA | INT_INTB |INT_INTC|INT_INTD|INT_AER|INT_MSI;
    writel(regval32, PCIE_VIRT + PLDA_IMASK);

    // ASPM configure.
    writel(0xFFFFFFFF, PCIE_VIRT + PLDA_ASPM_CONF);

    // Note: PLDA init must set type1 0x18 register before any configuration register read/write.    
    (void)_pcie_wr_conf(PCIE_VIRT, 0, 0, 0x18, 4, 0x00FF0100);
   
    spin_unlock_irqrestore(&mtk_pcie_lock, flags);
     
    /*
     * Request resources.
     */
    res = kzalloc(sizeof(struct resource) * 1, GFP_KERNEL);
    if (!res)
    	panic("pcie_setup unable to alloc resources");
    /*
     * IORESOURCE_IO
     */
    sys->resource[0] = NULL;

    /*
     * IORESOURCE_MEM
     */
    res[0].name = "PCIe Memory Space";
    res[0].flags = IORESOURCE_MEM;
    res[0].start = PCIE_MMIO_BASE;
    res[0].end = res[0].start + PCIE_BRIDGE_LENGTH - 1;
    if (request_resource(&iomem_resource, &res[0]))
    	panic("Request PCIe Memory resource failed\n");
    sys->resource[1] = &res[0];

    sys->resource[2] = NULL;
    sys->io_offset = 0;
    sys->mem_offset = 0;

    MTK_CreateTestProcFs();

    // Setup pci-express interrupt.
    if (request_irq(VECTOR_PCIE, &_pcie_irq, IRQF_SAMPLE_RANDOM, "PCIe-Interrupt", NULL) != 0) 
    {
    	LOG("request interrupt %d failed\n", VECTOR_PCIE);
    }

    return 1;
}

static int MTK_pcie_rd_conf(struct pci_bus *bus, u32 devfn, int where,
			    int size, u32 * val)
{
    unsigned long flags;
    int ret;

    /*
    LOG("MTK_pcie_rd_conf: bus=%d, dev=%d, func=%d, reg=0x%08X, size=%d.\n", 
        bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn), where, size);
    */

    if (_pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0)
    {
	*val = 0xffffffff;
	return PCIBIOS_DEVICE_NOT_FOUND;
    }

    spin_lock_irqsave(&mtk_pcie_lock, flags);
    ret = _pcie_rd_conf(PCIE_VIRT, bus->number, devfn, where, size, val);
    spin_unlock_irqrestore(&mtk_pcie_lock, flags);

    return ret;
}

static int MTK_pcie_wr_conf(struct pci_bus *bus, u32 devfn,
			    int where, int size, u32 val)
{
    unsigned long flags;
    int ret;

    /*
    LOG("MTK_pcie_wr_conf: bus=%d, dev=%d, func=%d, reg=0x%08X, size=%d, val=0x%08X.\n", 
        bus->number, PCI_SLOT(devfn), PCI_FUNC(devfn), where, size, val);
    */

    if (_pcie_valid_config(bus->number, PCI_SLOT(devfn)) == 0)
    {
	return PCIBIOS_DEVICE_NOT_FOUND;
    }

    spin_lock_irqsave(&mtk_pcie_lock, flags);
    ret = _pcie_wr_conf(PCIE_VIRT, bus->number, devfn, where, size, val);
    spin_unlock_irqrestore(&mtk_pcie_lock, flags);

    return ret;
}

int __init MTK_pci_sys_setup(int nr, struct pci_sys_data *sys)
{
    int ret = 0;

    LOG("MTK_pci_sys_setup: %d.\n", nr);

    if (nr == 0)
    {
	ret = MTK_pcie_setup(sys);
    }
    return ret;
}

struct pci_bus __init *MTK_pci_sys_scan(int nr, struct pci_sys_data *sys)
{
    struct pci_bus *bus;

    LOG("MTK_pci_sys_scan.\n");

    if (nr == 0)
    {
	bus = pci_scan_bus(sys->busnr, &pcie_ops, sys);
    }
    else
    {
	bus = NULL;
	PCI_ASSERT(0);
    }

    return bus;
}

int MTK_pci_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
{
    int irq = -1;

    LOG("MTK_pci_map_irq.\n");

    switch (pin) 
    {
        case 1: 
            irq = IRQ_PCIE_INTA;
            break;
        case 2: 
            irq = IRQ_PCIE_INTB;
            break;
        case 3: 
            irq = IRQ_PCIE_INTC;
            break;
        case 4: 
            irq = IRQ_PCIE_INTD;
            break;
        default: 
            break;;
    }

    LOG("PCIe map irq: %04d:%02x:%02x.%02x slot %d, pin %d, irq: %d\n",
        pci_domain_nr(dev->bus), dev->bus->number, PCI_SLOT(dev->devfn),
        PCI_FUNC(dev->devfn), slot, pin, irq);

    return irq;
}

#endif				/* CONFIG_PCI */
