/*
 * Copyright (C) 2010 Mediatek Inc
 * LH Hsiao <lh.hsiao@mediatek.com>
 *
 * This file is heavily based on relaview platform, almost a copy.
 *
 * Copyright (C) 2002 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/clockchips.h>

#include <asm/irq.h>
#include <asm/smp_twd.h>
#include <asm/localtimer.h>
#include <mach/platform.h>
/*
 * Setup the local clock events for a CPU.
 */
void __cpuinit local_timer_setup(struct clock_event_device *evt)
{
	/* Setup the local timer base */
	twd_base = (void __iomem *)MPCORE_GIT_VIRT; 
	
	evt->irq = IRQ_LOCALTIMER;
	twd_timer_setup(evt);
}
