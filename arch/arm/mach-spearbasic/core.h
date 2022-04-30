/*
 *
 *  Copyright (C)2008  ST MICROELECTRONICS
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * AUTHOR :  Deepak Sikri <deepak.sikri@st.com> */

#ifndef __ASM_ARCH_spearbasic_H
#define __ASM_ARCH_spearbasic_H


extern void __init spearbasic_init(void);
extern void __init spearbasic_init_irq(void);
extern void __init spearbasic_map_io(void);
extern struct sys_timer spearbasic_timer;
extern unsigned int mmc_status(struct device *dev);

#define AMBA_DEVICE(name,busid,base,plat)						\
	static struct amba_device name##_device = {					\
		.dev		= {											\
			.coherent_dma_mask = ~0,							\
			.bus_id	= busid,									\
			.platform_data = plat,								\
		},														\
		.res		= {											\
			.start	= spearbasic_##base##_BASE,					\
			.end	= (spearbasic_##base##_BASE) + SZ_4K - 1,	\
			.flags	= IORESOURCE_MEM,							\
		},														\
		.dma_mask	= ~0,										\
		.irq		= base##_IRQ,								\
		/* .dma		= base##_DMA,*/								\
	}

#endif
