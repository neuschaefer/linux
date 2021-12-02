/*
 *  linux/include/asm-arm/arch-mt5391/uncompress.h
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */

#ifndef __ARCH_ARM_UNCOMPRESS_H
#define __ARCH_ARM_UNCOMPRESS_H

#define MT5391_UART0_DR  (*(volatile unsigned char *)0xF000c000)
#define MT5391_UART0_ST  (*(volatile unsigned int *) 0xF000c04C)

/*
 * This does not append a newline
 */
static inline void putc(int c)
{
    /* wait for empty slot */
    while ((MT5391_UART0_ST & 0x1f00) == 0);

    MT5391_UART0_DR = c;

    if (c == '\n') {
        while ((MT5391_UART0_ST & 0x1f00) == 0);

        MT5391_UART0_DR = '\r';
    }
}

static inline void flush(void)
{
    /* wait for empty slot */
    while ((MT5391_UART0_ST & 0x1f00) == 0);
}

/*
 * nothing to do
 */
#define arch_decomp_setup()

#define arch_decomp_wdog()

#endif /* __ARCH_ARM_UNCOMPRESS_H */

