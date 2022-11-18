#include <linux/io.h>

#include <asm/setup.h>

#define UART_BASE	((void __iomem *)KSEG1ADDR(0xbf540000))
#define UART_TXLVL	(UART_BASE + 0x0010)
#define UART_TXDATA	(UART_BASE + 0x0100)

#define TXLVL_MAX 64

void putc(char c)
{
	while (readw(UART_TXLVL) >= TXLVL_MAX)
		;
	writew(c, UART_TXDATA);
}
