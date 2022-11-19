#include <linux/interrupt.h>
#include <linux/console.h>
#include <linux/irqchip.h>
#include <linux/of_address.h>

#include <asm/irq_cpu.h>

#define MONTAGE_FOO(x)		((x) * 4)

#define MONTAGE_NUM_IRQS	48

struct montage_intc {
	int irq;
	void __iomem *regs;
	struct irq_domain *domain;
};

static struct montage_intc *intc;

static void montage_intc_irq_unmask(struct irq_data *d)
{
	pr_info("%s: hwirq=%lu\n", __func__, irqd_to_hwirq(d));
}

static void montage_intc_irq_mask(struct irq_data *d)
{
	pr_info("%s: hwirq=%lu\n", __func__, irqd_to_hwirq(d));
}

static struct irq_chip montage_intc_irq_chip = {
	.name		= "montage intc",
	.irq_unmask	= montage_intc_irq_unmask,
	.irq_mask	= montage_intc_irq_mask,
	//.irq_mask_ack	= montage_intc_irq_mask,
};

static int montage_intc_map(struct irq_domain *d, unsigned int irq, irq_hw_number_t hw)
{
	pr_info("%s!\n", __func__);
	irq_set_chip_and_handler(irq, &montage_intc_irq_chip, handle_level_irq);

	return 0;
}

static const struct irq_domain_ops irq_domain_ops = {
	.xlate = irq_domain_xlate_onecell,
	.map = montage_intc_map,
};

static void montage_intc_irq_handler(struct irq_desc *desc)
{
	pr_info("%s!\n", __func__);
}

static void montage_intc_init_hw(void)
{
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
	pr_info("montage_of_init: regs = %px\n", intc->regs);

	montage_intc_init_hw();

	intc->domain = irq_domain_add_linear(np, MONTAGE_NUM_IRQS, &irq_domain_ops, intc);

	irq_set_chained_handler_and_data(intc->irq, montage_intc_irq_handler, intc->domain);

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
