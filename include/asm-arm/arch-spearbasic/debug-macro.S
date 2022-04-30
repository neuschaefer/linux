/* 
 *
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/


		.macro	addruart,rx
		 mrc     p15, 0, \rx, c1, c0
                tst     \rx, #1                 @ MMU enabled?
                moveq   \rx,      #0xD0000000   
                movne   \rx,      #IO_ADDRESS(0xD0000000)   @ virtual base 
		.endm

		.macro	senduart,rd,rx
		strb	\rd, [\rx, #0x0]	@ ASC_TX_BUFFER
		.endm

		.macro	waituart,rd,rx
1001:		ldr	\rd, [\rx, #0x18]	@ ASC_STATUS
		tst	\rd, #1 << 5		@ TX_FULL
		bne	1001b
		.endm

		.macro	busyuart,rd,rx
1001:		ldr	\rd, [\rx, #0x18]	@ ASC_STATUS
		tst	\rd, #1 << 7		@ TX_EMPTY
		beq	1001b
		.endm
