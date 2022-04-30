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

#include <linux/init.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/sysdev.h>
#include <linux/interrupt.h>

#include <asm/system.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/leds.h>
#include <asm/mach-types.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <asm/hardware/icst307.h>

#include <asm/mach/arch.h>
#include <asm/mach/flash.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <asm/mach/map.h>
#include <asm/mach/mmc.h>

#include "core.h"
#include "clock.h"


#if 0
static struct map_desc spearbasic_io_desc[] __initdata = {
	{ IO_ADDRESS(SPEARBASIC_ICM1_ICM2_BASE), 	SPEARBASIC_ICM1_ICM2_BASE 	 , 	SPEARBASIC_ICM1_ICM2_SIZE ,  MT_DEVICE },
	{ IO_ADDRESS(SPEARBASIC_ICM4_BASE),  		SPEARBASIC_ICM4_BASE	 , 			SPEARBASIC_ICM4_SIZE ,  		MT_DEVICE },
	{ IO_ADDRESS(SPEARBASIC_ML1_2_ICM3_BASE), 	SPEARBASIC_ML1_2_ICM3_BASE 	 , 	SPEARBASIC_ML1_2_ICM3_SIZE , MT_DEVICE },
//Ashwini - Adding a entry for a shared memory area starting from 32MB 
//	{  IPC_MEMORY_ADDRESS(IPC_MEMORY_START), IPC_MEMORY_START,  IPC_MEMORY_SIZE, MT_DEVICE}
};

#endif

static struct map_desc spearbasic_io_desc[] __initdata = {
{
		 .virtual        =  IO_ADDRESS(SPEARBASIC_ICM1_ICM2_BASE),
	         .pfn            =  __phys_to_pfn(SPEARBASIC_ICM1_ICM2_BASE),
        	 .length         =  SPEARBASIC_ICM1_ICM2_SIZE,
	         .type           =  MT_DEVICE
        }, {
	 	.virtual        =  IO_ADDRESS(SPEARBASIC_ICM4_BASE),
                .pfn            = __phys_to_pfn(SPEARBASIC_ICM4_BASE),
                .length         = SPEARBASIC_ICM4_SIZE,
                .type           = MT_DEVICE
        }, {
		 .virtual        =  IO_ADDRESS(SPEARBASIC_ML1_2_ICM3_BASE),
                .pfn            = __phys_to_pfn(SPEARBASIC_ML1_2_ICM3_BASE),
                .length         = SPEARBASIC_ML1_2_ICM3_SIZE,
                .type           = MT_DEVICE
        }
};

void __init spearbasic_map_io(void)
{
	iotable_init(spearbasic_io_desc, ARRAY_SIZE(spearbasic_io_desc));
}

