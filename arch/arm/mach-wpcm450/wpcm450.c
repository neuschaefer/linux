/* linux/arch/arm/mach-wpcm450/wpcm450.c
 *
 * Copyright (c) 2006 winbond Electronics
 *	shirley yu <clyu2@winbond.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *    22-08-2006     clyu     Created file
 *    30-12-2006     zswan    Change file
*/
  
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <linux/serial_core.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-serial.h>
#include <asm/arch/regs-gcr.h>


#include "wpcm450.h"
#include "cpu.h"
#include "clock.h"

/* Initial IO mappings */

static struct map_desc wpcm450_iodesc[] __initdata = {
#ifdef AVI_REMOVE
	IODESC_ENT(EMC),
	IODESC_ENT(USBOHCIHOST),
	IODESC_ENT(LCD),
#endif // AVI_REMOVE
	IODESC_ENT(TMR),
#ifdef AVI_REMOVE
	IODESC_ENT(ADC),
	
	/************************/
	/*zswan add it for map!********************/
	/*i am sorry ,i don't map other register!*/
	/*****************************************/
	
	IODESC_ENT(GDMA),
	IODESC_ENT(USBDEV),
	IODESC_ENT(DMA),
	IODESC_ENT(FMI),
	IODESC_ENT(ACTL),
	IODESC_ENT(ATAPI),
#endif // AVI_REMOVE
	
	IODESC_ENT(GPIO),
#ifdef AVI_REMOVE
	IODESC_ENT(RTC),
	IODESC_ENT(SC),
	IODESC_ENT(GE),
	IODESC_ENT(I2C),
	IODESC_ENT(PWM),
	IODESC_ENT(KPI),
	IODESC_ENT(PS2),
	IODESC_ENT(ADC),
#endif // AVI_REMOVE
	IODESC_ENT(GCR),
//	IODESC_ENT(CLK),
#ifdef AVI_REMOVE
	IODESC_ENT(EBI),
	IODESC_ENT(PCI),
#endif // AVI_REMOVE
	
	//IODESC_ENT(WATCHDOG),
};

static struct resource wpcm_uart0_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART0,
		.end   = WPCM450_PA_UART0 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART0,
		.end   = IRQ_UART0,
		.flags = IORESOURCE_IRQ,
	}

};

static struct resource wpcm_uart1_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	}
#ifndef __iDRAC__
	,
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	},
#endif
};
#ifdef __iDRAC__

static struct resource wpcm_uart2_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
};

static struct resource wpcm_uart3_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	}
};

static struct resource wpcm_uart4_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	}
};
static struct resource wpcm_uart5_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	}
};
static struct resource wpcm_uart6_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	}
};
#endif
/* our uart devices */

static struct platform_device wpcm_uart0 = {
	.name		  = "wpcm450-uart",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(wpcm_uart0_resource),
	.resource	  = wpcm_uart0_resource,
};


static struct platform_device wpcm_uart1 = {
	.name		  = "wpcm450-uart",
	.id		  = 1,
	.num_resources	  = ARRAY_SIZE(wpcm_uart1_resource),
	.resource	  = wpcm_uart1_resource,
};

#ifdef __iDRAC__

static struct platform_device wpcm_uart2 = {
	.name		  = "wpcm450-uart",
	.id		  = 2,
	.num_resources	  = ARRAY_SIZE(wpcm_uart2_resource),
	.resource	  = wpcm_uart2_resource,
};

static struct platform_device wpcm_uart3 = {
	.name		  = "wpcm450-uart",
	.id		  = 3,
	.num_resources	  = ARRAY_SIZE(wpcm_uart3_resource),
	.resource	  = wpcm_uart3_resource,
};

static struct platform_device wpcm_uart4 = {
	.name		  = "wpcm450-uart",
	.id		  = 4,
	.num_resources	  = ARRAY_SIZE(wpcm_uart4_resource),
	.resource	  = wpcm_uart4_resource,
};

static struct platform_device wpcm_uart5 = {
	.name		  = "wpcm450-uart",
	.id		  = 5,
	.num_resources	  = ARRAY_SIZE(wpcm_uart5_resource),
	.resource	  = wpcm_uart5_resource,
};


static struct platform_device wpcm_uart6 = {
	.name		  = "wpcm450-uart",
	.id		  = 6,
	.num_resources	  = ARRAY_SIZE(wpcm_uart6_resource),
	.resource	  = wpcm_uart6_resource,
};

#endif
static struct platform_device *uart_devices[] __initdata = {
	&wpcm_uart0,
    &wpcm_uart1,
#ifdef __iDRAC__	
	&wpcm_uart2,
	&wpcm_uart3,
	&wpcm_uart4,
	&wpcm_uart5,
	&wpcm_uart6,
#endif	
};

static int wpcm450_uart_count = 0;

/* uart registration process */

void ComInit (void)
{
    
    unsigned int u32Data = 0;
    
    /*Global and Serial Ports Switch setting*/
    u32Data = __raw_readl(WPCM450_GCR_MFSEL1);
    /* Disable Host Serial Port 1 for GPIOs. (chtsai) */
    __raw_writel (((u32Data&(~0x400))|0xa00),WPCM450_GCR_MFSEL1); /* Enable UART0, UART1 pin function */
    //printk("\n\n\nGCR_MFSSEL1 is %x\n\n\n",__raw_readl(WPCM450_GCR_MFSEL1));
    
    
    u32Data = __raw_readl(WPCM450_GCR_SPSWC);
    __raw_writel ((u32Data|0x1),WPCM450_GCR_SPSWC);/*Set Serial Port Mode to 2*/
   //printk("\n\n\nWPCM450_GCR_SPSWC is %x\n\n\n",__raw_readl(WPCM450_GCR_SPSWC));
 
	    /*Init UART0*/
    __raw_writeb (0x80, WPCM450_VA_UART0 + WPCM450_COM_LCR); /* select divisor latch registers */
    
    
    __raw_writeb (/*6*/ 0xb, WPCM450_VA_UART0 + WPCM450_COM_LSB);  /* Baud rate = 115200 : 15,000,000/(16*(5+2))  */
    //__raw_writeb (/*6*/ 76, WPCM450_VA_UART0 + WPCM450_COM_LSB);  /* Baud rate = 115200 : 15,000,000/(16*(5+2))  */
    //__raw_writel (COM_LSB, 96);	  /* Baud rate = 9600   : 15,000,000/(16*(96+2)) */
    __raw_writeb (0, WPCM450_VA_UART0 + WPCM450_COM_MSB);

    __raw_writeb (0x03, WPCM450_VA_UART0 + WPCM450_COM_LCR); /* none parity, 8 data bits, 1 stop bits */
    
    __raw_writeb (0x01, WPCM450_VA_UART0 + WPCM450_COM_IER); /* RDA interrupt source on / LOOP disable */
    __raw_writeb (0x00, WPCM450_VA_UART0 + WPCM450_COM_MCR);

    __raw_writeb (0x80+0x20, WPCM450_VA_UART0 + WPCM450_COM_TOR); /* Timeout if more than ??? bits xfer time */

    __raw_writeb (0x8F, WPCM450_VA_UART0 + WPCM450_COM_FCR); /* 8-byte FIFO trigger level, reset Tx and Rx FIFO */



    /*Init UART1*/
    //__raw_writeb (0x80, WPCM450_VA_UART1 + WPCM450_COM_LCR); /* select divisor latch registers */
    
    //__raw_writeb (0xB, WPCM450_VA_UART1 + WPCM450_COM_LSB);  /* Baud rate = 115200 : 15,000,000/(16*(b+2))  */
    __raw_writeb (76, WPCM450_VA_UART1 + WPCM450_COM_LSB);  /* Baud rate = 19200 : 15,000,000/(16*(76+2))  */
    printk("\n\nWPCM450_VA_UART1 + WPCM450_COM_LSB is 0x%x\n\n",ioread8(WPCM450_VA_UART1 + WPCM450_COM_LSB));
    __raw_writeb (0, WPCM450_VA_UART1 + WPCM450_COM_MSB);

    __raw_writeb (0x03, WPCM450_VA_UART1 + WPCM450_COM_LCR); /* none parity, 8 data bits, 1 stop bits */
    
    __raw_writeb (0x01, WPCM450_VA_UART1 + WPCM450_COM_IER); /* RDA interrupt source on / LOOP disable */
    __raw_writeb (0x00, WPCM450_VA_UART1 + WPCM450_COM_MCR);

    __raw_writeb (0x80+0x20, WPCM450_VA_UART1 + WPCM450_COM_TOR); /* Timeout if more than ??? bits xfer time */

    __raw_writeb (0x8F, WPCM450_VA_UART1 + WPCM450_COM_FCR); /* 8-byte FIFO trigger level, reset Tx and Rx FIFO */


 

} /* end ComInit */



void __init wpcm450_init_uarts(struct wpcm450_uartcfg *cfg, int no)
{
	struct platform_device *platdev;
	int uart;

	for (uart = 0; uart < no; uart++, cfg++) {
		platdev = uart_devices[cfg->hwport];

		wpcm450_uart_devs[uart] = platdev;
		platdev->dev.platform_data = cfg;
	}

	wpcm450_uart_count = uart;
	
	ComInit();
}

/* wpcm450_map_io
 *
 * register the standard cpu IO areas, and any passed in from the
 * machine specific initialisation.
*/

void __init wpcm450_map_io(struct map_desc *mach_desc, int mach_size)
{
	/* register our io-tables */

	iotable_init(wpcm450_iodesc, ARRAY_SIZE(wpcm450_iodesc));
	iotable_init(mach_desc, mach_size);
}

void __init wpcm450_init_clocks(int xtal)
{
	//s3c24xx_setup_clocks(xtal, fclk, hclk, pclk);
}

int __init wpcm450_init(void)
{
	printk("WPCM450: Initialising architecture\n");

	return platform_add_devices(wpcm450_uart_devs, wpcm450_uart_count);
}
