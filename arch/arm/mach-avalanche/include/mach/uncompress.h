/*
 * linux/include/asm-arm/arch-shark/uncompress.h
 * by Alexander Schulz
 *
 * derived from:
 * linux/include/asm-arm/arch-ebsa285/uncompress.h
 * Copyright (C) 1996,1997,1998 Russell King
 */

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by Texas Instruments
 * to do the following:
 * Explanation of modification.
 *  avalanche changes
 *  
 *
 * THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
 * "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
 * OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 * PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
 * DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
 * COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
 * See The GNU General Public License for more details.
 *
 * These changes are covered under version 2 of the GNU General Public License,
 * dated June 1991.
 */


#include <mach/memory.h>
#include <mach/puma.h>
#include <mach/hardware.h>

#if (CONFIG_AVALANCHE_CONSOLE_PORT == 0)
#define AVALANCHE_UART_REGS_BASE AVALANCHE_UART0_REGS_BASE
#endif
#if (CONFIG_AVALANCHE_CONSOLE_PORT == 1)
#define AVALANCHE_UART_REGS_BASE AVALANCHE_UART1_REGS_BASE
#endif
#if (CONFIG_AVALANCHE_CONSOLE_PORT == 2)
#define AVALANCHE_UART_REGS_BASE AVALANCHE_UART2_REGS_BASE
#endif



#define UART_REG_THR   (AVALANCHE_UART_REGS_BASE + 3)
#define UART_REG_LSR   (AVALANCHE_UART_REGS_BASE + 3 + 0x14)

#define putc(x) avl_putc(x)

static inline void avl_putc(int c)
{
	volatile unsigned char *thr = (volatile unsigned char *)(IO_VIRT2PHY(UART_REG_THR));
    volatile unsigned char *lsr = (volatile unsigned char *)(IO_VIRT2PHY(UART_REG_LSR));

	while((*lsr & 0x20) == 0);
		*thr = c;
}

/*
 * This does not append a newline
 */
static void avl_putstr(const char *s)
{
	while (*s) {
		avl_putc(*s);
		if (*s == '\n')
			avl_putc('\r');
		s++;
	}
}

static inline void flush(void)
{
}

#ifdef DEBUG
static void putn(unsigned long z)
{
	int i;
	char x;

	avl_putc('0');
	avl_putc('x');
	for (i=0;i<8;i++) {
		x='0'+((z>>((7-i)*4))&0xf);
		if (x>'9') x=x-'0'+'A'-10;
		avl_putc(x);
	}
}

static void putr()
{
	avl_putc('\n');
	avl_putc('\r');
}
#endif

/*
 * nothing to do
 */
#define arch_decomp_setup()
#define arch_decomp_wdog()
