#include <linux/interrupt.h>
#include <linux/console.h>
#include <linux/irqchip.h>
#include <linux/of_address.h>

#include <asm/irq_cpu.h>

#define MONTAGE_MODE(x)		(0x00 + (x) * 4)
#define MONTAGE_MASK(x)		(0x60 + (x) * 4)
#define MONTAGE_CAUSE		0x68
#define MONTAGE_ACK		0x6c

#define MONTAGE_NUM_IRQS	48

struct montage_intc {
	int irq;
	void __iomem *regs;
	struct irq_domain *domain;
};

static struct montage_intc *intc;

static void montage_intc_init_hw(void)
{
	// Mask all interrupts
	writel(0xffffffff, intc->regs + MONTAGE_MASK(0));
	writel(0xffffffff, intc->regs + MONTAGE_MASK(1));
}

static void montage_intc_set_mode(int hwirq, int mode)
{
	int index = hwirq / 8;
	int shift = (hwirq % 8) * 4;
	u32 val;

	val = readl(intc->regs + MONTAGE_MODE(index));
	val &= ~(0xf << shift);
	val |=  mode << shift;
	writel(val, intc->regs + MONTAGE_MODE(index));
}

static void montage_intc_irq_unmask(struct irq_data *d)
{
	int hwirq = irqd_to_hwirq(d);
	int index = hwirq / 32;
	int shift = hwirq % 32;
	u32 val;

	pr_info("%s: hwirq=%lu\n", __func__, irqd_to_hwirq(d));

	pr_info("%s: skip!\n", __func__);
	return;

	montage_intc_set_mode(hwirq, 3);

	val = readl(intc->regs + MONTAGE_MASK(index));
	val &= ~BIT(shift);
	writel(val, intc->regs + MONTAGE_MASK(index));
}

static void montage_intc_irq_mask(struct irq_data *d)
{
	int hwirq = irqd_to_hwirq(d);
	int index = hwirq / 32;
	int shift = hwirq % 32;
	u32 val;

	pr_info("%s: hwirq=%lu\n", __func__, irqd_to_hwirq(d));

	val = readl(intc->regs + MONTAGE_MASK(index));
	val |= BIT(shift);
	writel(val, intc->regs + MONTAGE_MASK(index));
}

static void montage_intc_ack(void)
{
	writel(1, intc->regs + MONTAGE_ACK);
	while (readl(intc->regs + MONTAGE_ACK) != 0)
		cpu_relax();
}

static void montage_intc_irq_ack(struct irq_data *d)
{
	pr_info("%s: hwirq=%lu\n", __func__, irqd_to_hwirq(d));

	montage_intc_ack();
}

static struct irq_chip montage_intc_irq_chip = {
	.name		= "montage intc",
	.irq_unmask	= montage_intc_irq_unmask,
	.irq_mask	= montage_intc_irq_mask,
	.irq_ack	= montage_intc_irq_ack,
};

static int montage_intc_map(struct irq_domain *d, unsigned int irq, irq_hw_number_t hw)
{
	pr_info("%s %d!\n", __func__, d->hwirq);
	irq_set_chip_and_handler(irq, &montage_intc_irq_chip, handle_level_irq);

	return 0;
}

static const struct irq_domain_ops irq_domain_ops = {
	.xlate = irq_domain_xlate_onecell,
	.map = montage_intc_map,
};

static void montage_intc_irq_handler(struct irq_desc *desc)
{
	unsigned int hwirq;
	u32 cause;

	cause = readl(intc->regs + MONTAGE_CAUSE);
	pr_info("%s: cause=%d\n", __func__, cause);

	hwirq = cause;
	generic_handle_domain_irq(intc->domain, hwirq);
}

static int __init montage_intc_of_init(struct device_node *np,
				       struct device_node *parent)
{
	intc = kzalloc(sizeof(*intc), GFP_KERNEL);
	if (!intc)
		return -ENOMEM;

	intc->irq = irq_of_parse_and_map(np, 0);
	if (!intc->irq) {
		pr_err("Failed to get INTC IRQ");
		return -EINVAL;
	}

	intc->regs = of_iomap(np, 0);
	if (!intc->regs) {
		pr_err("Failed to map Montage interrupt controller registers\n");
		kfree(intc);
		return -ENOMEM;
	}
	pr_info("%s: regs = %px\n", __func__, intc->regs);

	montage_intc_init_hw();

	intc->domain = irq_domain_add_linear(np, MONTAGE_NUM_IRQS, &irq_domain_ops, intc);

	irq_set_chained_handler_and_data(intc->irq, montage_intc_irq_handler, intc->domain);

	montage_intc_ack();

	return 0;
}

static struct of_device_id __initdata of_irq_ids[] = {
	{ .compatible = "mti,cpu-interrupt-controller", .data = mips_cpu_irq_of_init },
	{ .compatible = "montage,m88cs8001b-intc", .data = montage_intc_of_init },
	{},
};

void __init arch_init_irq(void)
{
	of_irq_init(of_irq_ids);
}
