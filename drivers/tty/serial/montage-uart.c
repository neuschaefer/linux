// SPDX-License-Identifier: GPL-2.0

#include <linux/serial_core.h>
#include <linux/tty_flip.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#define REG_TXLVL	0x0010	/* transmit FIFO level (max. 64) */
#define REG_RXLVL	0x0014	/* receive FIFO level (max. 64) */
#define REG_BAUDDIV	0x0018	/* clock divisor, integer part */
#define REG_BAUDFRAC	0x001c	/* clock divisor, fractional part (in steps of 1/16) */
#define REG_TX		0x0100	/* transmit data register */
#define REG_RX		0x0200	/* receive data register */

#define TXLVL_MAX	64
#define RXLVL_MAX	64

#define NUM_PORTS	8

struct montage_port {
	struct uart_port port;
	struct timer_list timer;
};

static struct montage_port *montage_uarts[NUM_PORTS];

#define to_montage_port(port)	container_of(port, struct montage_port, port)

#ifdef CONFIG_SERIAL_MONTAGE_CONSOLE
static struct console montage_console;
#endif

static struct uart_driver montage_driver = {
	.owner = THIS_MODULE,
	.driver_name = "montage",
	.dev_name = "ttyS",
	.major = 0,
	.minor = 0,
	.nr = NUM_PORTS,
#ifdef CONFIG_SERIAL_MONTAGE_CONSOLE
	.cons = &montage_console,
#endif
};

static bool montage_can_tx(struct uart_port *port)
{
	return ioread16(port->membase + REG_TXLVL) < TXLVL_MAX;
}

static void montage_putchar(struct uart_port *port, unsigned char ch)
{
	while (!montage_can_tx(port))
		cpu_relax();

	iowrite16(ch, port->membase + REG_TX);
}

static bool montage_can_rx(struct uart_port *port)
{
	return ioread16(port->membase + REG_RXLVL) > 0;
}

static int montage_rx(struct uart_port *port)
{
	return ioread16(port->membase + REG_RX) & 0xff;
}

static void montage_timer(struct timer_list *t)
{
	struct montage_port *uart = from_timer(uart, t, timer);
	struct uart_port *port = &uart->port;
	unsigned int flg = TTY_NORMAL;
	int ch;
	unsigned long status;

	while (montage_can_rx(port)) {
		ch = montage_rx(port);
		port->icount.rx++;

		if (uart_handle_sysrq_char(port, ch))
			continue;

		uart_insert_char(port, status, 0, ch, flg);
	}
	tty_flip_buffer_push(&port->state->port);

	mod_timer(&uart->timer, jiffies + uart_poll_timeout(port));
}

static unsigned int montage_tx_empty(struct uart_port *port)
{
	if (ioread16(port->membase + REG_TXLVL) == 0)
		return TIOCSER_TEMT;

	return 0;
}

static const char *montage_type(struct uart_port *port)
{
	return "montage-uart";
}

static int montage_startup(struct uart_port *port)
{
	pr_info("%s!\n", __func__);
	return 0;
}

static void montage_shutdown(struct uart_port *port)
{
	pr_info("%s!\n", __func__);
}

static const struct uart_ops montage_ops = {
	.tx_empty	= montage_tx_empty,
	.startup	= montage_startup,
	.shutdown	= montage_shutdown,
	.type		= montage_type,
};

static int montage_probe(struct platform_device *pdev)
{
	struct montage_port *uart;
	struct uart_port *port;
	int dev_id;

	dev_id = of_alias_get_id(pdev->dev.of_node, "serial");
	pr_info("montage_probe: dev_id=%d\n", dev_id);

	uart = devm_kzalloc(&pdev->dev, sizeof(struct montage_port), GFP_KERNEL);
	if (!uart)
		return -ENOMEM;

	port = &uart->port;

	port->membase = devm_platform_get_and_ioremap_resource(pdev, 0, NULL);
	if (IS_ERR(port->membase))
		return PTR_ERR(port->membase);

	port->dev = &pdev->dev;
	port->iotype = UPIO_MEM;
	port->flags = UPF_BOOT_AUTOCONF;
	port->ops = &montage_ops;
	port->regshift = 2;
	port->fifosize = 16;
	port->iobase = 1;
	port->type = PORT_UNKNOWN;
	port->line = dev_id;
	spin_lock_init(&port->lock);

	platform_set_drvdata(pdev, port);

	return uart_add_one_port(&montage_driver, &uart->port);
}

static int montage_remove(struct platform_device *pdev)
{
	struct uart_port *port = platform_get_drvdata(pdev);

	uart_remove_one_port(&montage_driver, port);
	return 0;
}

static const struct of_device_id montage_of_match[] = {
	{ .compatible = "montage,m88cs8001b-uart" },
	{}
};
MODULE_DEVICE_TABLE(of, montage_of_match);

static struct platform_driver montage_platform_driver = {
	.probe = montage_probe,
	.remove = montage_remove,
	.driver = {
		.name = "montage",
		.of_match_table = montage_of_match,
	},
};

#ifdef CONFIG_SERIAL_MONTAGE_CONSOLE
static void montage_console_write(struct console *co, const char *s,
	unsigned int count)
{
	struct montage_port *uart = montage_uarts[co->index];
	struct uart_port *port;
	unsigned long flags;

	port = &uart->port;

	spin_lock_irqsave(&port->lock, flags);
	uart_console_write(port, s, count, montage_putchar);
	spin_unlock_irqrestore(&port->lock, flags);
}

static int montage_console_setup(struct console *co, char *options)
{
	struct montage_port *uart = montage_uarts[co->index];
	struct uart_port *port;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	if (!uart)
		return -ENODEV;

	port = &uart->port;
	if (!port->membase)
		return -ENODEV;

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct console montage_console = {
	.name = "montage",
	.write = montage_console_write,
	.device = uart_console_device,
	.setup = montage_console_setup,
	.flags = CON_PRINTBUFFER,
	.index = -1,
	.data = &montage_driver,
};

static int __init montage_console_init(void)
{
	register_console(&montage_console);

	return 0;
}
console_initcall(montage_console_init);
#endif /* CONFIG_SERIAL_MONTAGE_CONSOLE */

static int __init montage_init(void)
{
	int res;

	res = uart_register_driver(&montage_driver);
	if (res)
		return res;

	res = platform_driver_register(&montage_platform_driver);
	if (res) {
		uart_unregister_driver(&montage_driver);
		return res;
	}

	return 0;
}

static void __exit montage_exit(void)
{
	platform_driver_unregister(&montage_platform_driver);
	uart_unregister_driver(&montage_driver);
}

module_init(montage_init);
module_exit(montage_exit);

MODULE_DESCRIPTION("Montage SoC serial driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:montage-uart");
