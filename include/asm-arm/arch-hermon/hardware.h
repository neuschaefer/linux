/*
 *  linux/include/asm-arm/arch-hermon/hardware.h
 *
 *  This file contains the hardware definitions of the hermon
 *
 *  Copyright (C) 1999 ARM Limited.
 *  Copyright (C) 2005 American Megatrends Inc
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
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

#include <asm/sizes.h>
#include <asm/arch/platform.h>



/*****************************************************************
			IO Mapping

 All registers and flash spread across a range of 256MB 
 form 0xB0000000 to 0xBFFFFFFF 

 Add 0x30000000 to physical addr to map to virtual addresss
 range of 0xE0000000 to 0xEFFFFFFF

 Memory devices (Flash) is mapped to 0xF0000000

*****************************************************************/
#define IO_OFFSET	                0x30000000
#define MEM_ADDRESS(x)                  (((x) >> 4)| 0xF0000000)
#define IO_ADDRESS(x)                   ((x)+IO_OFFSET)
#define PHY_ADDRESS(x)                  ((x) << 4)

#endif
