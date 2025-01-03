/*
 * Copyright (C) 2015 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/irqchip.h>
#include <linux/syscore_ops.h>
#include <soc/imx/revision.h>

#define IMR_NUM			4
#define GPC_MAX_IRQS            (IMR_NUM * 32)

#define GPC_IMR1_CORE0		0x30
#define GPC_IMR1_CORE1		0x40

#define GPC_CPU_PGC_SW_PUP_REQ  0xf0
#define GPC_PU_PGC_SW_PUP_REQ   0xf8
#define GPC_CPU_PGC_SW_PDN_REQ  0xfc

#define BM_CPU_PGC_SW_PDN_PUP_REQ_CORE1_A7      0x2
#define BM_GPC_PGC_PCG          0x1

#define GPC_PGC_C0              0x800
#define GPC_PGC_C0_PUPSCR       0x804
#define GPC_PGC_SCU_TIMING      0x890
#define GPC_PGC_C1              0x840

#define IMR_NUM                 4
#define GPC_MAX_IRQS            (IMR_NUM * 32)
#define GPC_LPCR_A7_BSC         0x0
#define GPC_LPCR_A7_AD          0x4
#define GPC_LPCR_M4             0x8
#define GPC_SLPCR               0x14
#define GPC_MLPCR               0x20
#define GPC_PGC_ACK_SEL_A7      0x24
#define GPC_MISC                0x2c
#define GPC_IMR1_CORE0          0x30
#define GPC_IMR1_CORE1          0x40
#define GPC_IMR1_M4             0x50
#define GPC_SLOT0_CFG           0xb0
#define GPC_PGC_CPU_MAPPING     0xec
#define GPC_CPU_PGC_SW_PUP_REQ  0xf0
#define GPC_PU_PGC_SW_PUP_REQ   0xf8
#define GPC_CPU_PGC_SW_PDN_REQ  0xfc
#define GPC_PU_PGC_SW_PDN_REQ   0x104
#define GPC_GTOR                0x124
#define GPC_PGC_C0              0x800
#define GPC_PGC_C0_PUPSCR       0x804
#define GPC_PGC_SCU_TIMING      0x890
#define GPC_PGC_C1              0x840
#define GPC_PGC_C1_PUPSCR       0x844
#define GPC_PGC_SCU             0x880
#define GPC_PGC_FM              0xa00
#define GPC_PGC_MIPI_PHY        0xc00
#define GPC_PGC_PCIE_PHY        0xc40
#define GPC_PGC_USB_OTG1_PHY    0xc80
#define GPC_PGC_USB_OTG2_PHY    0xcc0
#define GPC_PGC_USB_HSIC_PHY    0xd00


#define BM_LPCR_A7_BSC_IRQ_SRC_A7_WAKEUP        0x70000000
#define BM_LPCR_A7_BSC_CPU_CLK_ON_LPM           0x4000
#define BM_LPCR_A7_BSC_LPM1                     0xc
#define BM_LPCR_A7_BSC_LPM0                     0x3
#define BP_LPCR_A7_BSC_LPM1                     2
#define BP_LPCR_A7_BSC_LPM0                     0
#define BM_LPCR_M4_MASK_DSM_TRIGGER             0x80000000
#define BM_SLPCR_EN_DSM                         0x80000000
#define BM_SLPCR_RBC_EN                         0x40000000
#define BM_SLPCR_REG_BYPASS_COUNT               0x3f000000
#define BM_SLPCR_VSTBY                          0x4
#define BM_SLPCR_SBYOS                          0x2
#define BM_SLPCR_BYPASS_PMIC_READY              0x1
#define BM_SLPCR_EN_A7_FASTWUP_WAIT_MODE        0x10000
#define BM_LPCR_A7_AD_L2PGE                     0x10000
#define BM_LPCR_A7_AD_EN_C1_PUP                 0x800
#define BM_LPCR_A7_AD_EN_C1_IRQ_PUP             0x400
#define BM_LPCR_A7_AD_EN_C0_PUP                 0x200
#define BM_LPCR_A7_AD_EN_C0_IRQ_PUP             0x100
#define BM_LPCR_A7_AD_EN_PLAT_PDN               0x10
#define BM_LPCR_A7_AD_EN_C1_PDN                 0x8
#define BM_LPCR_A7_AD_EN_C1_WFI_PDN             0x4
#define BM_LPCR_A7_AD_EN_C0_PDN                 0x2
#define BM_LPCR_A7_AD_EN_C0_WFI_PDN             0x1

#define BM_CPU_PGC_SW_PDN_PUP_REQ_CORE1_A7      0x2
#define BM_GPC_PGC_PCG                          0x1
#define BM_GPC_PGC_CORE_PUPSCR                  0x7fff80

#define BM_GPC_PGC_ACK_SEL_A7_DUMMY_PUP_ACK     0x80000000
#define BM_GPC_PGC_ACK_SEL_A7_DUMMY_PDN_ACK     0x8000
#define BM_GPC_MLPCR_MEMLP_CTL_DIS              0x1

#define BP_LPCR_A7_BSC_IRQ_SRC                  28

#define MAX_SLOT_NUMBER                         10
#define A7_LPM_WAIT                             0x5
#define A7_LPM_STOP                             0xa

static void __iomem *gpc_base;

struct gpcv2_irqchip_data {
	struct raw_spinlock	rlock;
	void __iomem		*gpc_base;
	u32			wakeup_sources[IMR_NUM];
	u32			saved_irq_mask[IMR_NUM];
	u32			cpu2wakeup;
};

static struct gpcv2_irqchip_data *imx_gpcv2_instance;

static inline void imx_gpcv2_set_m_core_pgc(bool enable, u32 offset)
{
        u32 val = readl_relaxed(gpc_base + offset) & (~BM_GPC_PGC_PCG);

        if (enable)
                val |= BM_GPC_PGC_PCG;

        writel_relaxed(val, gpc_base + offset);
}

void imx_gpcv2_set_core1_pdn_pup_by_software(bool pdn)
{
	u32 offset = pdn ? GPC_CPU_PGC_SW_PDN_REQ : GPC_CPU_PGC_SW_PUP_REQ;
	u32 val;

	imx_gpcv2_set_m_core_pgc(true, GPC_PGC_C1);

	val = readl_relaxed(gpc_base + offset);
	val |= BM_CPU_PGC_SW_PDN_PUP_REQ_CORE1_A7;
	writel_relaxed(val, gpc_base + offset);

	while (((readl_relaxed(gpc_base + offset)) &
	       BM_CPU_PGC_SW_PDN_PUP_REQ_CORE1_A7) != 0);

	imx_gpcv2_set_m_core_pgc(false, GPC_PGC_C1);
}

/*
 * Interface for the low level wakeup code.
 */
u32 imx_gpcv2_get_wakeup_source(u32 **sources)
{
	if (!imx_gpcv2_instance)
		return 0;

	if (sources)
		*sources = imx_gpcv2_instance->wakeup_sources;

	return IMR_NUM;
}

static int gpcv2_wakeup_source_save(void)
{
	struct gpcv2_irqchip_data *cd;
	void __iomem *reg;
	int i;

	cd = imx_gpcv2_instance;
	if (!cd)
		return 0;

	for (i = 0; i < IMR_NUM; i++) {
		reg = cd->gpc_base + cd->cpu2wakeup + i * 4;
		cd->saved_irq_mask[i] = readl_relaxed(reg);
		writel_relaxed(cd->wakeup_sources[i], reg);
	}

	return 0;
}

static void gpcv2_wakeup_source_restore(void)
{
	struct gpcv2_irqchip_data *cd;
	void __iomem *reg;
	int i;

	cd = imx_gpcv2_instance;
	if (!cd)
		return;

	for (i = 0; i < IMR_NUM; i++) {
		reg = cd->gpc_base + cd->cpu2wakeup + i * 4;
		writel_relaxed(cd->saved_irq_mask[i], reg);
	}
}

static struct syscore_ops imx_gpcv2_syscore_ops = {
	.suspend	= gpcv2_wakeup_source_save,
	.resume		= gpcv2_wakeup_source_restore,
};

static int imx_gpcv2_irq_set_wake(struct irq_data *d, unsigned int on)
{
	struct gpcv2_irqchip_data *cd = d->chip_data;
	unsigned int idx = d->hwirq / 32;
	unsigned long flags;
	void __iomem *reg;
	u32 mask, val;

	raw_spin_lock_irqsave(&cd->rlock, flags);
	reg = cd->gpc_base + cd->cpu2wakeup + idx * 4;
	mask = 1 << d->hwirq % 32;
	val = cd->wakeup_sources[idx];

	cd->wakeup_sources[idx] = on ? (val & ~mask) : (val | mask);
	raw_spin_unlock_irqrestore(&cd->rlock, flags);

	/*
	 * Do *not* call into the parent, as the GIC doesn't have any
	 * wake-up facility...
	 */

	return 0;
}

static void imx_gpcv2_irq_unmask(struct irq_data *d)
{
	struct gpcv2_irqchip_data *cd = d->chip_data;
	void __iomem *reg;
	u32 val;

	raw_spin_lock(&cd->rlock);
	reg = cd->gpc_base + cd->cpu2wakeup + d->hwirq / 32 * 4;
	val = readl_relaxed(reg);
	val &= ~(1 << d->hwirq % 32);
	writel_relaxed(val, reg);
	raw_spin_unlock(&cd->rlock);

	irq_chip_unmask_parent(d);
}

static void imx_gpcv2_irq_mask(struct irq_data *d)
{
	struct gpcv2_irqchip_data *cd = d->chip_data;
	void __iomem *reg;
	u32 val;

	raw_spin_lock(&cd->rlock);
	reg = cd->gpc_base + cd->cpu2wakeup + d->hwirq / 32 * 4;
	val = readl_relaxed(reg);
	val |= 1 << (d->hwirq % 32);
	writel_relaxed(val, reg);
	raw_spin_unlock(&cd->rlock);

	irq_chip_mask_parent(d);
}

static struct irq_chip gpcv2_irqchip_data_chip = {
	.name			= "GPCv2",
	.irq_eoi		= irq_chip_eoi_parent,
	.irq_mask		= imx_gpcv2_irq_mask,
	.irq_unmask		= imx_gpcv2_irq_unmask,
	.irq_set_wake		= imx_gpcv2_irq_set_wake,
	.irq_retrigger		= irq_chip_retrigger_hierarchy,
	.irq_set_type		= irq_chip_set_type_parent,
#ifdef CONFIG_SMP
	.irq_set_affinity	= irq_chip_set_affinity_parent,
#endif
};

static int imx_gpcv2_domain_translate(struct irq_domain *d,
				      struct irq_fwspec *fwspec,
				      unsigned long *hwirq,
				      unsigned int *type)
{
	if (is_of_node(fwspec->fwnode)) {
		if (fwspec->param_count != 3)
			return -EINVAL;

		/* No PPI should point to this domain */
		if (fwspec->param[0] != 0)
			return -EINVAL;

		*hwirq = fwspec->param[1];
		*type = fwspec->param[2];
		return 0;
	}

	return -EINVAL;
}

static int imx_gpcv2_domain_alloc(struct irq_domain *domain,
				  unsigned int irq, unsigned int nr_irqs,
				  void *data)
{
	struct irq_fwspec *fwspec = data;
	struct irq_fwspec parent_fwspec;
	irq_hw_number_t hwirq;
	unsigned int type;
	int err;
	int i;

	err = imx_gpcv2_domain_translate(domain, fwspec, &hwirq, &type);
	if (err)
		return err;

	if (hwirq >= GPC_MAX_IRQS)
		return -EINVAL;

	for (i = 0; i < nr_irqs; i++) {
		irq_domain_set_hwirq_and_chip(domain, irq + i, hwirq + i,
				&gpcv2_irqchip_data_chip, domain->host_data);
	}

	parent_fwspec = *fwspec;
	parent_fwspec.fwnode = domain->parent->fwnode;
	return irq_domain_alloc_irqs_parent(domain, irq, nr_irqs,
					    &parent_fwspec);
}

static const struct irq_domain_ops gpcv2_irqchip_data_domain_ops = {
	.translate	= imx_gpcv2_domain_translate,
	.alloc		= imx_gpcv2_domain_alloc,
	.free		= irq_domain_free_irqs_common,
};

static int __init imx_gpcv2_irqchip_init(struct device_node *node,
			       struct device_node *parent)
{
	struct irq_domain *parent_domain, *domain;
	struct gpcv2_irqchip_data *cd;
	int i, val;

	if (!parent) {
		pr_err("%pOF: no parent, giving up\n", node);
		return -ENODEV;
	}

	parent_domain = irq_find_host(parent);
	if (!parent_domain) {
		pr_err("%pOF: unable to get parent domain\n", node);
		return -ENXIO;
	}

	cd = kzalloc(sizeof(struct gpcv2_irqchip_data), GFP_KERNEL);
	if (!cd) {
		pr_err("kzalloc failed!\n");
		return -ENOMEM;
	}

	raw_spin_lock_init(&cd->rlock);

	cd->gpc_base = of_iomap(node, 0);
	if (!cd->gpc_base) {
		pr_err("fsl-gpcv2: unable to map gpc registers\n");
		kfree(cd);
		return -ENOMEM;
	}
	gpc_base = cd->gpc_base;

	domain = irq_domain_add_hierarchy(parent_domain, 0, GPC_MAX_IRQS,
				node, &gpcv2_irqchip_data_domain_ops, cd);
	if (!domain) {
		iounmap(cd->gpc_base);
		kfree(cd);
		return -ENOMEM;
	}
	irq_set_default_host(domain);

	/* Initially mask all interrupts */
	for (i = 0; i < IMR_NUM; i++) {
		writel_relaxed(~0, cd->gpc_base + GPC_IMR1_CORE0 + i * 4);
		writel_relaxed(~0, cd->gpc_base + GPC_IMR1_CORE1 + i * 4);
		cd->wakeup_sources[i] = ~0;
	}

	/* Let CORE0 as the default CPU to wake up by GPC */
	cd->cpu2wakeup = GPC_IMR1_CORE0;

	/*
	 * Due to hardware design failure, need to make sure GPR
	 * interrupt(#32) is unmasked during RUN mode to avoid entering
	 * DSM by mistake.
	 */
	writel_relaxed(~0x1, cd->gpc_base + cd->cpu2wakeup);

	/* only external IRQs to wake up LPM and core 0/1 */
	val = readl_relaxed(gpc_base + GPC_LPCR_A7_BSC);
	val |= BM_LPCR_A7_BSC_IRQ_SRC_A7_WAKEUP;
	writel_relaxed(val, gpc_base + GPC_LPCR_A7_BSC);

	/* mask m4 dsm trigger if M4 NOT enabled */
	/* m4 is disabled during boot up
	 * use "if (!imx_src_is_m4_enabled())" in future
	 */
	writel_relaxed(readl_relaxed(gpc_base + GPC_LPCR_M4) |
		BM_LPCR_M4_MASK_DSM_TRIGGER, gpc_base + GPC_LPCR_M4);

	/* set mega/fast mix in A7 domain */
	writel_relaxed(0x1, gpc_base + GPC_PGC_CPU_MAPPING);
	/* set SCU timing */
	writel_relaxed((0x59 << 10) | 0x5B | (0x2 << 20),
		gpc_base + GPC_PGC_SCU_TIMING);

	/* set C0/C1 power up timming per design requirement */
	val = readl_relaxed(gpc_base + GPC_PGC_C0_PUPSCR);
	val &= ~BM_GPC_PGC_CORE_PUPSCR;
	val |= (0x1A << 7);
	writel_relaxed(val, gpc_base + GPC_PGC_C0_PUPSCR);

	val = readl_relaxed(gpc_base + GPC_PGC_C1_PUPSCR);
	val &= ~BM_GPC_PGC_CORE_PUPSCR;
	val |= (0x1A << 7);
	writel_relaxed(val, gpc_base + GPC_PGC_C1_PUPSCR);

	val = readl_relaxed(gpc_base + GPC_SLPCR);
	val &= ~(BM_SLPCR_EN_DSM);

	/* m4 is disabled during boot up
	 * use "if (!imx_src_is_m4_enabled())" in future
	 */
	val &= ~(BM_SLPCR_VSTBY | BM_SLPCR_RBC_EN |
		 BM_SLPCR_SBYOS | BM_SLPCR_BYPASS_PMIC_READY);

	val |= BM_SLPCR_EN_A7_FASTWUP_WAIT_MODE;
	writel_relaxed(val, gpc_base + GPC_SLPCR);

	if (imx_get_soc_revision() == IMX_CHIP_REVISION_1_0) {
		/* disable memory low power mode */
		val = readl_relaxed(gpc_base + GPC_MLPCR);
		val |= BM_GPC_MLPCR_MEMLP_CTL_DIS;
		writel_relaxed(val, gpc_base + GPC_MLPCR);
	}

	imx_gpcv2_instance = cd;
	register_syscore_ops(&imx_gpcv2_syscore_ops);

	/*
	 * Clear the OF_POPULATED flag set in of_irq_init so that
	 * later the GPC power domain driver will not be skipped.
	 */
	of_node_clear_flag(node, OF_POPULATED);
	return 0;
}

IRQCHIP_DECLARE(imx_gpcv2, "fsl,imx7d-gpc", imx_gpcv2_irqchip_init);
