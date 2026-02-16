/*
 *
 * entry-macro.S  
 * Description:
 * isr entry point
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
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

#include <mach/hardware.h>

		.macro	disable_fiq
		.endm

		.macro  get_irqnr_preamble, base, tmp
		mov     \base, #INTC_VIRT
		.endm

		.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp
		ldr     \irqstat, [\base, #0x280] /* Status of Int 0-31 */
		ldr     \irqnr, [\base, #0x80] /*get irq # from global priority idx reg*/
		and	\irqnr, \irqnr, #0xff     /* keep bits 0-9 */
		cmp     \irqstat, #0x0
		bne	44f
		ldr     \irqstat, [\base, #0x284] /* Status of Int 32-63*/
		cmp \irqstat, #0x0
		bne 44f
		ldr     \irqstat, [\base, #0x288] /* Status of Int 64-95*/
		cmp \irqstat, #0x0
44:
		.endm


		.macro  arch_ret_to_user, tmp1, tmp2
		.endm

