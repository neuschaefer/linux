#include <linux/interrupt.h>
#include <linux/console.h>

void __init arch_init_irq(void)
{
	printk("arch_init_irq: TODO!!\n");
}

asmlinkage void plat_irq_dispatch(void)
{
	printk("plat_irq_dispatch: TODO!!\n");
}
