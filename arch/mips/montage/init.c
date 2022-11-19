#include <linux/console.h>

#include <asm/bootinfo.h>
#include <asm/prom.h>
#include <asm/time.h>

const char *get_system_type(void)
{
	return "Montage LZ SoC";
}

void __init prom_init(void)
{
	/* intentionally left empty */
}

void __init plat_mem_setup(void)
{
	void *dtb;

	dtb = get_fdt();
	if (dtb == NULL)
		panic("no dtb found");

	__dt_setup_arch(dtb);
}

void __init plat_time_init(void)
{
	mips_hpt_frequency = 297000000;  // not sure about that, let's see
	pr_info("plat_time_init: mips_hpt_frequency = %d\n", mips_hpt_frequency);
}
