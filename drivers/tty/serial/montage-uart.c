// SPDX-License-Identifier: GPL-2.0

#include <linux/serial_core.h>
#include <linux/tty_flip.h>
#include <linux/of.h>
#include <linux/of_irq.h>
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

struct montage_uart {
	struct uart_port port;
	struct timer_list timer;
};

static struct montage_uart *montage_uarts[NUM_PORTS];

#define to_montage_uart(port)	container_of(port, struct montage_uart, port)

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

#if 0
static void montage_timer(struct timer_list *t)
{
	struct montage_uart *uart = from_timer(uart, t, timer);
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
#endif

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

static irqreturn_t montage_irq_handler(int irq, void *priv)
{
	struct uart_port *port = priv;
	unsigned int flg = TTY_NORMAL;
	unsigned long status;
	int ch;

	while (montage_can_rx(port)) {
		ch = montage_rx(port);
		port->icount.rx++;

		if (uart_handle_sysrq_char(port, ch))
			continue;

		uart_insert_char(port, status, 0, ch, flg);
	}
	tty_flip_buffer_push(&port->state->port);

	return IRQ_HANDLED;
}

static int montage_startup(struct uart_port *port)
{
	pr_info("%s: [0x08] = %08x, [0x24] = %08x\n", __func__,
			readl(port->membase + 0x08), readl(port->membase + 0x24));

	return request_irq(port->irq, montage_irq_handler, IRQF_TRIGGER_HIGH, "montage-uart", port);
}

static void montage_shutdown(struct uart_port *port)
{
	pr_info("%s!\n", __func__);
}

static void montage_config_port(struct uart_port *port, int type)
{
	pr_info("%s lol\n", __func__);
	port->type = PORT_MONTAGE;
}

static void montage_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	pr_info("%s lol\n", __func__);
}

static void montage_set_termios(struct uart_port *port, struct ktermios *new,
				const struct ktermios *old)
{
	pr_info("%s lol\n", __func__);
}

static void montage_start_tx(struct uart_port *port)
{
	struct circ_buf *xmit = &port->state->xmit;
	unsigned char ch;

	// TODO: deal with backpressure a little better

	if (unlikely(port->x_char)) {
		montage_putchar(port, port->x_char);
		port->icount.tx++;
		port->x_char = 0;
	} else if (!uart_circ_empty(xmit)) {
		while (xmit->head != xmit->tail) {
			ch = xmit->buf[xmit->tail];
			xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
			port->icount.tx++;
			montage_putchar(port, ch);
		}
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);
}

static void montage_stop_tx(struct uart_port *port)
{
	pr_info("%s lol\n", __func__);
}

static const struct uart_ops montage_ops = {
	.tx_empty	= montage_tx_empty,
	.startup	= montage_startup,
	.shutdown	= montage_shutdown,
	.type		= montage_type,
	.config_port	= montage_config_port,
	.set_mctrl	= montage_set_mctrl,
	.set_termios	= montage_set_termios,
	.start_tx	= montage_start_tx,
	.stop_tx	= montage_stop_tx,
};

static int montage_probe(struct platform_device *pdev)
{
	struct montage_uart *uart;
	struct uart_port *port;
	struct resource *res_mem;
	int dev_id;
	int irq;

	dev_id = of_alias_get_id(pdev->dev.of_node, "serial");

	uart = devm_kzalloc(&pdev->dev, sizeof(struct montage_uart), GFP_KERNEL);
	if (!uart)
		return -ENOMEM;

	port = &uart->port;


	res_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res_mem)
		return -ENODEV;

	irq = of_irq_get(pdev->dev.of_node, 0);
	if (irq < 0)
		return irq;

	port->irq = irq;
	port->dev = &pdev->dev;
	port->iotype = UPIO_MEM;
	port->mapbase = res_mem->start;
	port->mapsize = resource_size(res_mem);
	port->membase = devm_ioremap(&pdev->dev, port->mapbase, port->mapsize);
	port->flags = UPF_BOOT_AUTOCONF;
	port->ops = &montage_ops;
	port->regshift = 2;
	port->fifosize = 16;
	port->iobase = 1;
	port->type = PORT_UNKNOWN;
	port->line = dev_id;
	port->fifosize = TXLVL_MAX;
	spin_lock_init(&port->lock);

	platform_set_drvdata(pdev, port);

	montage_uarts[dev_id] = uart;
	pr_info("%s: montage_driver=%px, port=%px\n", __func__, &montage_driver, &uart->port);
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
	struct montage_uart *uart = montage_uarts[co->index];
	struct uart_port *port = &uart->port;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	uart_console_write(port, s, count, montage_putchar);
	spin_unlock_irqrestore(&port->lock, flags);
}

static int montage_console_setup(struct console *co, char *options)
{
	struct montage_uart *uart = montage_uarts[co->index];
	struct uart_port *port = &uart->port;
	int baud = 115200;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	pr_info("%s: entry, idx=%u, uart = %px\n", __func__, co->index, uart);
	if (!uart)
		return -ENODEV;

	port = &uart->port;
	if (!port->membase)
		return -ENODEV;

	pr_info("%s: [0x08] = %08x, [0x24] = %08x\n", __func__,
			readl(port->membase + 0x08), readl(port->membase + 0x24));

	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

static struct console montage_console = {
	.name = "ttyS",
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
