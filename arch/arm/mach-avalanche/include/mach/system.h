/*
 *
 * system.h 
 * Description:
 * reset/idle arch hooks
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


#ifndef __ASM_ARCH_SYSTEM_H
#define __ASM_ARCH_SYSTEM_H

#include <mach/io.h>
#include <asm-arm/arch-avalanche/generic/pal.h>
extern void avalanche_system_reset(PAL_SYS_SYSTEM_RST_MODE_T mode);
extern void avalanche_processor_idle(void);

static void arch_reset(char mode, const char *cmd  )
{
	local_irq_disable();
	avalanche_system_reset((PAL_SYS_SYSTEM_RST_MODE_T)0);	
}

static inline void arch_idle(void)
{
	avalanche_processor_idle();
}

#endif
