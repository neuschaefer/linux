/*
 *  8250_sunxi.c
 *
 *  Copyright (C) 1996-2003 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/module.h>
#include <linux/types.h>
#include <linux/tty.h>
#include <linux/serial_core.h>
#include <linux/errno.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/serial_reg.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <asm/io.h>
#include <asm/ecard.h>
#include <asm/string.h>
#include <linux/clk.h>

#include <mach/sys_config.h>
#include <mach/platform.h>
#include <mach/irqs.h>

#include "8250.h"
#if CONFIG_CHIP_ID==1123
#define MAX_PORTS	    8
#elif CONFIG_CHIP_ID==1125
#define MAX_PORTS	    4
#else
#error "Unknown chip ID for Serial"
#endif

static int sw_serial[MAX_PORTS];


#if 0
#define UART_MSG(fmt...)    printk("[uart]: "fmt)
#else
#define UART_MSG(fmt...)	do { } while (0)
#endif
/* Register base define */
#define UART_BASE       (0x01C28000)
#define UART_BASE_OS    (0x400)
#define UARTx_BASE(x)   (UART_BASE + (x) * UART_BASE_OS)
#define RESSIZE(res)    (((res)->end - (res)->start)+1)
#define OFFSET			0xf0000000
#define FCR_AW  0x0e1
#define REG_RD_U(fmt...)	__raw_readl(port_base_addr + fmt)
#define REG_WD_U(member,fmt...)	__raw_writel(member,port_base_addr + fmt)
struct sw_serial_port {
    struct uart_port    port;
    char                name[16];
    int                 port_no;
    int                 pin_num;
    u32                 pio_hdle;
    struct clk          *clk;
    u32                 sclk;
    struct resource     *mmres;
    u32                 irq;
    struct platform_device* pdev;
};
typedef struct backup_reg_def{
	u32 dll; 		/* 0x00	*/
	u32 dlh;		/* 0x04 */
	u32 ier;		/* 0x04 */
	u32 fcr;		/* 0x08 */
	u32 lcr;		/* 0x0C */
	u32 mcr;		/* 0x10 */
	u32 sch;		/* 0x1C */
	u32 halt;		/* 0xA4 */
} backup_reg_t;
#define	BACK_REG	backup_reg[port_num]
#define U_DLL		0X00
#define U_RX		0X00
#define U_DLH		0X04
#define U_IER		0X04
#define U_FCR		0X08
#define U_LCR		0X0c
#define U_MCR		0X10
#define U_SCH		0X1c
#define	U_USR		0X7c
#define U_HALT		0Xa4
static backup_reg_t backup_reg[MAX_PORTS];
void sunxi_8250_backup_reg(int port_num ,struct uart_port *port)
{
	unsigned long port_base_addr;
	port_base_addr = port->mapbase + OFFSET;
	UART_MSG("\nport_base_addr is %x \n port->mapbase is %x\n",port_base_addr,port->mapbase);
	BACK_REG.lcr	= REG_RD_U(U_LCR);
	BACK_REG.mcr	= REG_RD_U(U_MCR);
	BACK_REG.sch	= REG_RD_U(U_SCH);
	BACK_REG.halt	= REG_RD_U(U_HALT);
	BACK_REG.fcr	= FCR_AW;
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr & 0x7f,U_LCR);
	UART_MSG("\n1: lcr is %x  \n backup_reg.lcr is %x\n",REG_RD_U(U_LCR),BACK_REG.lcr);
	BACK_REG.ier	= REG_RD_U(U_IER);
	UART_MSG("\norgin: lcr is %x mcr is %x sch is %x halt is %x ier is %x\n",REG_RD_U(U_LCR),REG_RD_U(U_MCR),REG_RD_U(U_SCH),REG_RD_U(U_HALT),REG_RD_U(U_IER));
	UART_MSG("\nback: lcr is %x mcr is %x sch is %x halt is %x ier is %x\n",BACK_REG.lcr,BACK_REG.mcr,BACK_REG.sch,BACK_REG.halt,BACK_REG.ier);
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr | 0x80,U_LCR);
	BACK_REG.dll	= REG_RD_U(U_DLL);
	BACK_REG.dlh	= REG_RD_U(U_DLH);
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr,U_LCR);
	UART_MSG("\n2: lcr is %x  \n backup_reg.lcr is %x\n",REG_RD_U(U_LCR),BACK_REG.lcr);
}
void sunxi_8250_comeback_reg(int port_num,struct uart_port *port)
{
	unsigned long port_base_addr;
	port_base_addr = port->mapbase + OFFSET;
	REG_WD_U(BACK_REG.sch,U_SCH);
	REG_WD_U(BACK_REG.halt,U_HALT);
	REG_WD_U(BACK_REG.mcr,U_MCR);
	REG_WD_U(BACK_REG.fcr,U_FCR);
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr & 0x7f,U_LCR);
	UART_MSG("\n1: lcr is %x  \n backup_reg.lcr is %x\n",REG_RD_U(U_LCR),BACK_REG.lcr);
	REG_WD_U(BACK_REG.ier,U_IER);
	UART_MSG("\nback1: lcr is %x mcr is %x sch is %x halt is %x ier is %x\n",REG_RD_U(U_LCR),REG_RD_U(U_MCR),REG_RD_U(U_SCH),REG_RD_U(U_HALT),REG_RD_U(U_IER));
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr | 0x80,U_LCR);
	REG_WD_U(BACK_REG.dll,U_DLL);
	REG_WD_U(BACK_REG.dlh,U_DLH);
	while (REG_RD_U(U_USR)&1)
        REG_RD_U(U_RX);
	REG_WD_U(BACK_REG.lcr,U_LCR);
	UART_MSG("\nback1: lcr is %x mcr is %x sch is %x halt is %x ier is %x\n",REG_RD_U(U_LCR),REG_RD_U(U_MCR),REG_RD_U(U_SCH),REG_RD_U(U_HALT),REG_RD_U(U_IER));
	UART_MSG("\n3: lcr is %x  \n backup_reg.lcr is %x\n",REG_RD_U(U_LCR),BACK_REG.lcr);
}

//static void sw_serial_print_reg(struct sw_serial_port *sport)
//{
//    int i;
//    
//    for (i=0; i<32; i+=4)
//    {
//        if (!(i&0xf))
//            eayly_printk("\n0x%08x : ", i);
//        eayly_printk("%08x ", readl(sport->port.membase + i));
//    }
//}

static int sw_serial_get_resource(struct sw_serial_port *sport)
{
    char name[16];
    struct clk *pclk = NULL;
    char uart_para[16];
    int ret;

    /* get register base */
    sport->mmres = platform_get_resource(sport->pdev, IORESOURCE_MEM, 0);
    if (!sport->mmres) {
        ret = -ENODEV;
        goto err_out;
    }

    /* get clock */
    pclk = clk_get(&sport->pdev->dev, "apb1");
    if (IS_ERR(pclk)) {
        ret = PTR_ERR(pclk);
        goto iounmap;
    }

    sport->sclk = clk_get_rate(pclk);
    clk_put(pclk);

    sprintf(name, "apb_uart%d", sport->port_no);
    sport->clk = clk_get(&sport->pdev->dev, name);
    if (IS_ERR(sport->clk)) {
        ret = PTR_ERR(sport->clk);
        goto iounmap;
    }
    clk_enable(sport->clk);

    /* get irq */
    sport->irq = platform_get_irq(sport->pdev, 0);
    if (sport->irq == 0) {
        ret = -EINVAL;
        goto free_pclk;
    }

    /* get gpio resource */
    sprintf(uart_para, "uart_para%d", sport->port_no);
    sport->pio_hdle = gpio_request_ex(uart_para, NULL);
    if (!sport->pio_hdle) {
        ret = -EINVAL;
        goto free_pclk;
    }
    return 0;

free_pclk:
    clk_put(sport->clk);
iounmap:
err_out:
    return ret;
}

static int sw_serial_put_resource(struct sw_serial_port *sport)
{
    clk_disable(sport->clk);
    clk_put(sport->clk);
    gpio_release(sport->pio_hdle, 1);
    return 0;
}

static int sw_serial_get_config(struct sw_serial_port *sport, u32 uart_id)
{
    char uart_para[16] = {0};
    int ret;

    sprintf(uart_para, "uart_para%d", uart_id);
    ret = script_parser_fetch(uart_para, "uart_port", &sport->port_no, sizeof(int));
    if (ret)
        return -1;
    if (sport->port_no != uart_id)
        return -1;
    ret = script_parser_fetch(uart_para, "uart_type", &sport->pin_num, sizeof(int));
    if (ret)
        return -1;

    return 0;
}

static void
sw_serial_pm(struct uart_port *port, unsigned int state,
          unsigned int oldstate)
{
    struct sw_serial_port *up = (struct sw_serial_port *)port;

    if (!state)
        clk_enable(up->clk);
    else
        clk_disable(up->clk);
}

static int __devinit
sw_serial_probe(struct platform_device *dev)
{
    struct sw_serial_port *sport;
	int ret;
    UART_MSG("this coming sw_serial_probe\n");
	sport = kzalloc(sizeof(struct sw_serial_port), GFP_KERNEL);
	if (!sport)
		return -ENOMEM;
    sport->port_no  = dev->id;
    sport->pdev     = dev;

    ret = sw_serial_get_config(sport, dev->id);
    if (ret) {
        UART_MSG(KERN_ERR "Failed to get config information\n");
        goto free_dev;
    }

    ret = sw_serial_get_resource(sport);
    if (ret) {
        UART_MSG(KERN_ERR "Failed to get resource\n");
        goto free_dev;
    }
    platform_set_drvdata(dev, sport);

    sport->port.irq     = sport->irq;
    sport->port.fifosize= 64;
	sport->port.regshift= 2;
    sport->port.iotype  = UPIO_DWAPB32;
    sport->port.flags   = UPF_IOREMAP | UPF_BOOT_AUTOCONF;
    sport->port.uartclk = sport->sclk;
    sport->port.pm      = sw_serial_pm;
    sport->port.dev     = &dev->dev;

    sport->port.mapbase = sport->mmres->start;
    sw_serial[sport->port_no] = serial8250_register_port(&sport->port);
    UART_MSG("serial probe %d, membase %p irq %d mapbase 0x%08x\n", 
             dev->id, sport->port.membase, sport->port.irq, sport->port.mapbase);
	UART_MSG("sport->pdev is %x \n &sport->pdev is %x",sport->pdev,&sport->pdev);
	UART_MSG("pdev.dev is %x \n &pdev.dev is %x",sport->pdev->dev,&sport->pdev->dev);
	UART_MSG("dev.dev is %x \n &dev.dev is %x",dev->dev,&dev->dev);
	return 0;
free_dev:
    kfree(sport);
    sport = NULL;
    return ret;
}

static int __devexit sw_serial_remove(struct platform_device *dev)
{
    struct sw_serial_port *sport = platform_get_drvdata(dev);

	UART_MSG("serial remove\n");
	serial8250_unregister_port(sw_serial[sport->port_no]);
	sw_serial[sport->port_no] = 0;
	sw_serial_put_resource(sport);

	kfree(sport);
	sport = NULL;
	return 0;
}
static int sw_serial_suspend(struct platform_device *dev, pm_message_t state)
{
	int i;
	struct uart_port *port;
	UART_MSG("aw_resume uart suspend\n");
	UART_MSG("&dev->dev is 0x%x\n",&dev->dev);
#if 0
		volatile __u32 loop_flag = 1;
		while(1 == loop_flag);

#endif
	for (i = 0; i < MAX_PORTS; i++) {
		port=(struct uart_port *)get_ports(i);
		if (port->type != PORT_UNKNOWN){
		UART_MSG("type is 0x%x  PORT_UNKNOWN is 0x%x\n",port->type,PORT_UNKNOWN);
		UART_MSG("port.dev is 0x%x  &dev->dev is 0x%x\n",port->dev,&dev->dev);
		}

		if ((port->type != PORT_UNKNOWN)&& (port->dev == &dev->dev)){
			UART_MSG(" SUPER_STANDBY ");
			//serial8250_suspend_port(i);
				UART_MSG(" SUPER_STANDBY ");
				sunxi_8250_backup_reg(i,port);
			}
	}

	return 0;
}

static int sw_serial_resume(struct platform_device *dev)
{
	struct uart_port *port;
	int i;
	UART_MSG("aw_resume SUPER_STANDBY resume\n");
	UART_MSG("&dev->dev is 0x%x\n",&dev->dev);
#if 0
		volatile __u32 loop_flag = 1;
		while(1 == loop_flag);

#endif	
	for (i = 0; i < MAX_PORTS; i++) {
		port=(struct uart_port *)get_ports(i);
		if (port->type != PORT_UNKNOWN){
		UART_MSG("type is 0x%x  PORT_UNKNOWN is 0x%x\n",port->type,PORT_UNKNOWN);
		UART_MSG("port.dev is 0x%x  &dev->dev is 0x%x\n",port->dev,&dev->dev);
		}
		if ((port->type != PORT_UNKNOWN) && (port->dev == &dev->dev)){
				UART_MSG(" SUPER_STANDBY ");
				sunxi_8250_comeback_reg(i,port);
			//serial8250_resume_port(i);

	}
	}
#if 0
		while(1 == loop_flag);

#endif	
	return 0;
}

static struct platform_driver sw_serial_driver = {
    .probe  	= sw_serial_probe,
    .remove 	= sw_serial_remove,
	.suspend	= sw_serial_suspend,
	.resume		= sw_serial_resume,
    .driver = {
        .name    = "sunxi-uart",
        .owner    = THIS_MODULE,
    },
};

static struct resource sw_uart_res[8][2] = {
    {/* uart0 resource */
        {.start = UARTx_BASE(0),      .end = UARTx_BASE(0) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART0, .end = SW_INT_IRQNO_UART0,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart1 resource */
        {.start = UARTx_BASE(1),      .end = UARTx_BASE(1) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART1, .end = SW_INT_IRQNO_UART1,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart2 resource */
        {.start = UARTx_BASE(2),      .end = UARTx_BASE(2) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART2, .end = SW_INT_IRQNO_UART2,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart3 resource */
        {.start = UARTx_BASE(3),      .end = UARTx_BASE(3) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART3, .end = SW_INT_IRQNO_UART3,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart4 resource */
        {.start = UARTx_BASE(4),      .end = UARTx_BASE(4) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART4, .end = SW_INT_IRQNO_UART4,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart5 resource */
        {.start = UARTx_BASE(5),      .end = UARTx_BASE(5) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART5, .end = SW_INT_IRQNO_UART5,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart6 resource */
        {.start = UARTx_BASE(6),      .end = UARTx_BASE(6) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART6, .end = SW_INT_IRQNO_UART6,           .flags = IORESOURCE_IRQ}, /*irq */
    },
    {/* uart7 resource */
        {.start = UARTx_BASE(7),      .end = UARTx_BASE(7) + UART_BASE_OS - 1, .flags = IORESOURCE_MEM}, /*base*/
        {.start = SW_INT_IRQNO_UART7, .end = SW_INT_IRQNO_UART7,           .flags = IORESOURCE_IRQ}, /*irq */
    },
};

struct platform_device sw_uart_dev[] = {
    [0] = {.name = "sunxi-uart", .id = 0, .num_resources = ARRAY_SIZE(sw_uart_res[0]), .resource = &sw_uart_res[0][0], .dev = {}},
    [1] = {.name = "sunxi-uart", .id = 1, .num_resources = ARRAY_SIZE(sw_uart_res[1]), .resource = &sw_uart_res[1][0], .dev = {}},
    [2] = {.name = "sunxi-uart", .id = 2, .num_resources = ARRAY_SIZE(sw_uart_res[2]), .resource = &sw_uart_res[2][0], .dev = {}},
    [3] = {.name = "sunxi-uart", .id = 3, .num_resources = ARRAY_SIZE(sw_uart_res[3]), .resource = &sw_uart_res[3][0], .dev = {}},
    [4] = {.name = "sunxi-uart", .id = 4, .num_resources = ARRAY_SIZE(sw_uart_res[4]), .resource = &sw_uart_res[4][0], .dev = {}},
    [5] = {.name = "sunxi-uart", .id = 5, .num_resources = ARRAY_SIZE(sw_uart_res[5]), .resource = &sw_uart_res[5][0], .dev = {}},
    [6] = {.name = "sunxi-uart", .id = 6, .num_resources = ARRAY_SIZE(sw_uart_res[6]), .resource = &sw_uart_res[6][0], .dev = {}},
    [7] = {.name = "sunxi-uart", .id = 7, .num_resources = ARRAY_SIZE(sw_uart_res[7]), .resource = &sw_uart_res[7][0], .dev = {}},
};

static int uart_used;
static int __init sw_serial_init(void)
{
    int ret;
    int i;
    int used = 0;
    char uart_para[16];

    memset(sw_serial, 0, sizeof(sw_serial));
    uart_used = 0;
    for (i=0; i<MAX_PORTS; i++, used=0) {
        sprintf(uart_para, "uart_para%d", i);
        ret = script_parser_fetch(uart_para, "uart_used", &used, sizeof(int));
        if (ret)
            UART_MSG("failed to get uart%d's used information\n", i);
        if (used) {
            uart_used |= 1 << i;
            platform_device_register(&sw_uart_dev[i]);
        }
    }

    if (uart_used) {
        UART_MSG("used uart info.: 0x%02x\n", uart_used);
        ret = platform_driver_register(&sw_serial_driver);
        return ret;
    }

	return 0;
}

static void __exit sw_serial_exit(void)
{
    if (uart_used)
	    platform_driver_unregister(&sw_serial_driver);
}

MODULE_AUTHOR("Aaron.myeh<leafy.myeh@Reuuimllatech.com>");
MODULE_DESCRIPTION("SUNXI 8250-compatible serial port expansion card driver");
MODULE_LICENSE("GPL");

module_init(sw_serial_init);
module_exit(sw_serial_exit);
