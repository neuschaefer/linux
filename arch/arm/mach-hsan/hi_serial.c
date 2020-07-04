/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************
  Filename : serial_hsan.c
  Function : hsan serial device driver

  Date     : D2013_11_25

  Version  : V1.0
  Record   :
             create file.
******************************************************************************/

#include <mach/hi_hsan.h>

#ifdef __ARCH_OPS_DEFINE__
#endif

static hi_uint32 hi_uart_tx_empty(struct uart_port *pst_port)
{
    hi_uint32 ui_status = 0;

    HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_status);
    return (ui_status & HI_TEMT) ? TIOCSER_TEMT : 0;
}

static hi_void hi_uart_set_mctrl(struct uart_port *pst_port, hi_uint32 ui_mctrl)
{
}

static hi_uint32 hi_uart_get_mctrl(struct uart_port *pst_port)
{
    return 0;
}

static hi_void hi_uart_stop_tx(struct uart_port *pst_port)
{
    hi_uint32 ui_val = 0;

    HI_REG_READ( pst_port->membase + HI_UART_IER, ui_val );
    ui_val &= ~(HI_PTIME|HI_ETBEI);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, ui_val);
}

static hi_void hi_uart_start_tx(struct uart_port *pst_port)
{
    hi_uint32 ui_val = 0;

    HI_REG_READ( pst_port->membase + HI_UART_IER, ui_val );
    ui_val |= HI_ETBEI;
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, ui_val);
}

static hi_void hi_uart_stop_rx(struct uart_port *pst_port)
{
    hi_uint32 ui_val = 0;

    HI_REG_READ( pst_port->membase + HI_UART_IER, ui_val );
    ui_val &= ~(HI_ELSI|HI_ERBFI);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, ui_val);
}

static hi_void hi_uart_enable_ms(struct uart_port *pst_port)
{
}

static hi_void hi_uart_rx_chars(struct uart_port *pst_port)
{
    struct tty_struct *pst_tty      = pst_port->state->port.tty;
    hi_uint32          ui_status    = 0;
    hi_uint32          ui_ch        = 0;
    hi_uint32          ui_max_count = 256;
    hi_uint32          ui_flag      = 0;

    HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_status);
    if(!(ui_status & HI_DR))
    {
        HI_REG_READ( pst_port->membase + HI_UART_RBR, ui_ch);
    }

    while ((ui_status & HI_DR) && ui_max_count--)
    {
        HI_REG_READ( pst_port->membase + HI_UART_RBR, ui_ch);
        ui_ch  |= HI_UART_DUMMY_DR_RX;
        ui_flag = TTY_NORMAL;
        pst_port->icount.rx++;

        /* Note that the error handling code is out of the main execution path */
        if (unlikely(ui_ch & HI_UART_DR_ERR))
        {
            if (ui_ch & HI_UART_DR_BE)
            {
                ui_ch &= ~(HI_UART_DR_FE | HI_UART_DR_PE);
                pst_port->icount.brk++;
                if (uart_handle_break(pst_port))
                {
                    HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_status);
                    continue;
                }
            }
            else if (ui_ch & HI_UART_DR_PE)
            {
                pst_port->icount.parity++;
            }
            else if (ui_ch & HI_UART_DR_FE)
            {
                pst_port->icount.frame++;
            }
            else if (ui_ch & HI_UART_DR_OE)
            {
                pst_port->icount.overrun++;
            }

            ui_ch &= pst_port->read_status_mask;

            if (ui_ch & HI_UART_DR_BE)
            {
                ui_flag = TTY_BREAK;
            }
            else if (ui_ch & HI_UART_DR_PE)
            {
                ui_flag = TTY_PARITY;
            }
            else if (ui_ch & HI_UART_DR_FE)
            {
                ui_flag = TTY_FRAME;
            }
        }

        if (uart_handle_sysrq_char(pst_port, ui_ch & 255))
        {
            HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_status);
            continue;
        }

        uart_insert_char(pst_port, ui_ch, HI_UART_DR_OE, ui_ch, ui_flag);
        HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_status);
    }
    
    tty_flip_buffer_push(pst_tty);
}

static hi_void hi_uart_tx_chars(struct uart_port *pst_port)
{
    struct circ_buf *pst_xmit = &pst_port->state->xmit;
    hi_int32         i_count  = 0;
    hi_uint32        ui_val   = 0;

    if (pst_port->x_char)
    {
        do {
            HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_val);                
        }while ( (ui_val&0x20) != 0x20 );

        HI_REG_WRITE( pst_port->membase + HI_UART_THR, pst_port->x_char );
        pst_port->icount.tx++;
        pst_port->x_char = 0;
        return;
    }

    if ( uart_circ_empty(pst_xmit) || uart_tx_stopped(pst_port))
    {
        hi_uart_stop_tx(pst_port);
        return;
    }

    i_count = pst_port->fifosize >> 1;

    do {
        do {
            HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_val);                
        }while ( (ui_val&0x20) != 0x20 );

        HI_REG_WRITE( pst_port->membase + HI_UART_THR, pst_xmit->buf[pst_xmit->tail] );
        pst_xmit->tail = (pst_xmit->tail + 1) & (PAGE_SIZE - 1);
        pst_port->icount.tx++;
        if (uart_circ_empty(pst_xmit))
        {
            break;
        }
    } while (--i_count > 0);

    if (uart_circ_chars_pending(pst_xmit) < WAKEUP_CHARS)
    {
        uart_write_wakeup(pst_port);
    }
    if (uart_circ_empty(pst_xmit))
    {
        hi_uart_stop_tx(pst_port);
    }
}

static irqreturn_t hi_uart_intr(hi_int32 i_irq, hi_void *pv_dev_id)
{
    hi_int32          i_handled       = 0;
    hi_uint32         ui_int_id       = 0;
    hi_uint32         ui_status_usr   = 0;
    hi_uint32         ui_status       = 0;
    hi_uint32         ui_pass_counter = HI_ISR_PASS_LMT;
    struct uart_port *pst_port        = (struct uart_port*)pv_dev_id;
   
    spin_lock(&pst_port->lock);
    HI_REG_READ( pst_port->membase + HI_UART_IIR, ui_status);
    ui_int_id = 0x0F&ui_status;
    
    while (ui_int_id != HI_NOINTERRUPT)
    {
        if ((HI_RECEIVEAVA == ui_int_id) || (HI_RECTIMEOUT == ui_int_id) || (HI_RECEIVERR == ui_int_id))
        {
            hi_uart_rx_chars(pst_port);
        }else if (HI_MODEMSTA == ui_int_id){
            ;
        }
        else if(HI_TRANSEMP == ui_int_id)
        {
            hi_uart_tx_chars(pst_port);
        }
        else
        {
            HI_REG_READ( pst_port->membase + HI_UART_USR, ui_status_usr);
        }
        if (ui_pass_counter-- == 0)
        {
            break;
        }
        HI_REG_READ( pst_port->membase + HI_UART_IIR, ui_status);
        ui_int_id = 0x0F&ui_status;
        i_handled = 1;
    }    
    spin_unlock(&pst_port->lock);
    return IRQ_RETVAL(i_handled);
}

static hi_void hi_uart_break_ctl(struct uart_port *pst_port, hi_int32 i_break_state)
{
    hi_uint32  ui_lcr_h = 0;
    hi_ulong32 ul_flags = 0;

    spin_lock_irqsave(&pst_port->lock, ul_flags);
    
    do {
        HI_REG_READ( pst_port->membase + HI_UART_USR, ui_lcr_h);
    }while( ui_lcr_h&HI_UART_BUSY );

    HI_REG_READ( pst_port->membase + HI_UART_LCR, ui_lcr_h);
    if (i_break_state)
    {
        ui_lcr_h |= HI_UART_BREAK;
    }
    else
    {
        ui_lcr_h &= ~HI_UART_BREAK;
    }
    HI_REG_WRITE( pst_port->membase + HI_UART_LCR, ui_lcr_h );
    
    spin_unlock_irqrestore(&pst_port->lock, ul_flags);
}

static hi_int32 hi_uart_startup(struct uart_port *pst_port)
{
    hi_int32  i_ret  = 0;
    hi_uint32 ui_val = 0;

    /* disable FIFO and interrupt */
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, 0 );
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0 );
    HI_REG_READ( pst_port->membase + HI_UART_USR, ui_val);
    
    /* Allocate the IRQ */
    i_ret = request_irq(pst_port->irq, hi_uart_intr, 0, HI_UART_DRVNAME, pst_port );
    if (0 != i_ret)
    {
        return i_ret;
    }
    /* enable FIFO */
    spin_lock_irq(&pst_port->lock);
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0xb1);
    HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_val);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, HI_ERBFI|HI_ELSI );
    spin_unlock_irq(&pst_port->lock);
    return 0;
}

static hi_void hi_uart_shutdown(struct uart_port *pst_port)
{
    hi_uint32 ui_val = 0;

    /* disable/clear all interrupts */
    spin_lock_irq(&pst_port->lock);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, 0 );
    HI_REG_READ( pst_port->membase + HI_UART_LSR, ui_val);
    spin_unlock_irq(&pst_port->lock);

    /* Free the interrupt */
    free_irq(pst_port->irq, pst_port);
    
    /* disable break condition and fifos */
    do {
        HI_REG_READ( pst_port->membase + HI_UART_USR, ui_val);
    }while( ui_val&HI_UART_BUSY );
    
    HI_REG_READ( pst_port->membase + HI_UART_LCR, ui_val);    
    ui_val &= ~(HI_UART_BREAK);
    HI_REG_WRITE( pst_port->membase + HI_UART_LCR, ui_val );
    /* disable fifo*/
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0);
}

static hi_void hi_uart_set_termios(struct uart_port *pst_port, struct ktermios *pst_termios, struct ktermios *pst_old)
{
    hi_uint32  ui_lcr_h  = 0;
    hi_uint32  ui_old_cr = 0;
    hi_uint32  ui_baud   = 0;
    hi_uint32  ui_quot   = 0;
    hi_ulong32 ul_flags  = 0;    

    /* Ask the core to calculate the divisor for us. */
    ui_baud = uart_get_baud_rate(pst_port, pst_termios, pst_old, 0, pst_port->uartclk/16);
    ui_quot = pst_port->uartclk / (16 * ui_baud);
    
    switch ( pst_termios->c_cflag & CSIZE )
    {
        case CS5:
            ui_lcr_h = HI_UART_5bit;
            break;
        case CS6:
            ui_lcr_h = HI_UART_6bit;
            break;
        case CS7:
            ui_lcr_h = HI_UART_7bit;
            break;
        case CS8:
            ui_lcr_h = HI_UART_8bit;
            break;
        default: // CS8
            ui_lcr_h = HI_UART_8bit;
            break;
    }
    
    if (pst_termios->c_cflag & CSTOPB)
    {
        ui_lcr_h |= HI_UART_STOP;
    }
    if (pst_termios->c_cflag & PARENB)
    {
        ui_lcr_h |= HI_UART_PEN;
        if (!(pst_termios->c_cflag & PARODD))
        {
            ui_lcr_h |= HI_UART_EPS;
        }
    }

    spin_lock_irqsave(&pst_port->lock, ul_flags);
    
    /* Update the per-pst_port timeout. */
    uart_update_timeout(pst_port, pst_termios->c_cflag, ui_baud);
    pst_port->read_status_mask = HI_UART_DR_OE | 255;
    if (pst_termios->c_iflag & INPCK)
    {
        pst_port->read_status_mask |= HI_UART_DR_FE | HI_UART_DR_PE;
    }
    if (pst_termios->c_iflag & (BRKINT | PARMRK))
    {
        pst_port->read_status_mask |= HI_UART_DR_BE;
    }

    /* Characters to ignore */
    pst_port->ignore_status_mask = 0;
    if (pst_termios->c_iflag & IGNPAR)
    {
        pst_port->ignore_status_mask |= HI_UART_DR_FE | HI_UART_DR_PE;
    }
    if (pst_termios->c_iflag & IGNBRK)
    {
        pst_port->ignore_status_mask |= HI_UART_DR_BE;
        /* If we're ignoring parity and break indicators, ignore overruns too (for real raw support). */
        if (pst_termios->c_iflag & IGNPAR)
        {
            pst_port->ignore_status_mask |= HI_UART_DR_OE;
        }
    }

    /* Ignore all characters if CREAD is not set. */
    if ((pst_termios->c_cflag & CREAD) == 0)
    {
        pst_port->ignore_status_mask |= HI_UART_DUMMY_DR_RX;
    }
    
    /* Enable DLL and DLH */
    HI_REG_READ(  pst_port->membase + HI_UART_LCR, ui_old_cr);
    ui_old_cr |= HI_UART_DLAB;
    HI_REG_WRITE( pst_port->membase + HI_UART_LCR, ui_old_cr );

    /* Set baud rate */
    HI_REG_WRITE( pst_port->membase + HI_UART_DLH, ((ui_quot&0xFF00)>>8));
    HI_REG_WRITE( pst_port->membase + HI_UART_DLL, (ui_quot & 0xFF));

    ui_old_cr &= ~HI_UART_DLAB;
    HI_REG_WRITE( pst_port->membase + HI_UART_LCR, ui_old_cr );
    HI_REG_WRITE( pst_port->membase + HI_UART_LCR, ui_lcr_h );

    spin_unlock_irqrestore(&pst_port->lock, ul_flags);
}

static const hi_char8 *hi_uart_type(struct uart_port *pst_port)
{
    return ( HI_UART_PORT_AMBA == pst_port->type ) ? HI_UART_DEVNAME : HI_NULL;
}

static hi_void hi_uart_release_port(struct uart_port *pst_port)
{
	release_mem_region(pst_port->mapbase, SZ_4K );
}

static hi_int32 hi_uart_request_port(struct uart_port *pst_port)
{
	return request_mem_region(pst_port->mapbase, SZ_4K, HI_UART_DEVNAME) != HI_NULL ? 0 : -EBUSY;
}

static hi_void hi_uart_config_port(struct uart_port *pst_port, hi_int32 i_flags)
{
    if ( i_flags & UART_CONFIG_TYPE ) 
    {
        pst_port->type = HI_UART_PORT_AMBA;
        hi_uart_request_port(pst_port);
    }
}

static hi_int32 hi_uart_verify_port(struct uart_port *pst_port, struct serial_struct *pst_ser)
{
    hi_int32 i_ret = 0;

    if ((HI_UART_PORT_UNKNOWN != pst_ser->type) && (HI_UART_PORT_AMBA != pst_ser->type))
    {
        i_ret = -EINVAL;
    }
    if ((0 > pst_ser->irq) || (NR_IRQS <= pst_ser->irq))
    {
        i_ret = -EINVAL;
    }
    return i_ret;
}

static struct uart_ops g_st_uart_ops =
{
    .tx_empty       = hi_uart_tx_empty,
    .set_mctrl      = hi_uart_set_mctrl,
    .get_mctrl      = hi_uart_get_mctrl,
    .stop_tx        = hi_uart_stop_tx,
    .start_tx       = hi_uart_start_tx,
    .stop_rx        = hi_uart_stop_rx,
    .enable_ms      = hi_uart_enable_ms,
    .break_ctl      = hi_uart_break_ctl,
    .startup        = hi_uart_startup,
    .shutdown       = hi_uart_shutdown,
    .set_termios    = hi_uart_set_termios,
    .type           = hi_uart_type,
    .release_port	= hi_uart_release_port,
    .request_port	= hi_uart_request_port,
    .config_port    = hi_uart_config_port,
    .verify_port    = hi_uart_verify_port,
};

struct uart_port g_st_uart_port[] =
{
    {
		.membase	= (hi_void*)hi_io_address(HI_REG_BASE_UART0),
		.mapbase	= HI_REG_BASE_UART0,
		.iotype		= HI_UPIO_MEM,
		.irq		= HI_INTNR_UART0,
		.uartclk	= CONFIG_HSAN_BUSCLK,
		.fifosize	= 16,
		.ops		= &g_st_uart_ops,
		.flags		= HI_UPF_BOOT_AUTOCONF,
		.line		= 0,
    },
    {
		.membase	= (hi_void*)hi_io_address(HI_REG_BASE_UART1),
		.mapbase	= HI_REG_BASE_UART1,
		.iotype		= HI_UPIO_MEM,
		.irq		= HI_INTNR_UART1,
		.uartclk	= CONFIG_HSAN_BUSCLK,
		.fifosize	= 16,
		.ops		= &g_st_uart_ops,
		.flags		= HI_UPF_BOOT_AUTOCONF,
		.line		= 1,
    },
};

static hi_void hi_uart_console_put_char(struct uart_port *pst_port, hi_int32 i_ch)
{
    hi_uint32 ui_stat = 0x00;

    ui_stat = *(volatile hi_uint32*)(pst_port->membase + HI_UART_LSR);
    while(!( (ui_stat & (1<<5)) && (ui_stat & (1 << 6))))
    {
        barrier();
        ui_stat = *(volatile hi_uint32*)(pst_port->membase + HI_UART_LSR);
    }
    *(volatile hi_uint32*)pst_port->membase = i_ch&0xFF;
}

static hi_void hi_uart_console_write(struct console *pst_cons, const hi_char8 *pc_str, hi_uint32 ui_cnt)
{
    struct uart_port *pst_port = g_st_uart_port + pst_cons->index;
    uart_console_write(pst_port, pc_str, ui_cnt, hi_uart_console_put_char);
}

static hi_int32 hi_uart_console_setup(struct console *pst_cons, hi_char8 *pc_options)
{
    struct uart_port *pst_port = g_st_uart_port + pst_cons->index;
	hi_int32 i_baud   = CONFIG_HSAN_KCONSOLE_BAUD;
	hi_int32 i_bits   = 8;
	hi_int32 i_parity = 'n';
	hi_int32 i_flow   = 'n';
	
    hi_debug("UART: setup console(%d) mapbase(0x%x).",pst_cons->index,pst_port->mapbase);
    hi_debug("i_baud=%d, i_parity=%d, i_bits=%d, i_flow=%d",i_baud, i_parity, i_bits, i_flow);
    return uart_set_options(pst_port, pst_cons, i_baud, i_parity, i_bits, i_flow);
}

struct uart_driver g_st_uart_driver;

static struct console g_st_uart_console =
{
    .name           = HI_UART_DEVNAME,
    .write          = hi_uart_console_write,
    .setup          = hi_uart_console_setup,
    .device         = uart_console_device,
    .flags          = CON_PRINTBUFFER,
#if ( CONFIG_HSAN_KCONSOLE == 0 )
    .index          = 0,
#else
    .index          = 1,
#endif
    .data		    = &g_st_uart_driver,
};

static hi_int32 hi_uart_console_init(hi_void)
{
	register_console(&g_st_uart_console);	
	return 0;
}
console_initcall(hi_uart_console_init);

#ifdef __MODULE_INIT__
#endif

struct uart_driver g_st_uart_driver =
{
    .owner          = THIS_MODULE,
    .driver_name    = HI_UART_DRVNAME,
    .dev_name       = HI_UART_DEVNAME,
    .major          = HI_UART_MAJOR,    //204
    .minor          = HI_UART_MINOR,    //64
    .nr             = HI_UART_NR,
    .cons           = &g_st_uart_console,
};

#if 1
#define HI_REG_BASE_IOMUX               0x14900000
#define HI_BIT_SET(reg,bit)             ((reg) |= (bit)) 
#define HI_BIT_CLR(reg,bit)             ((reg) &= (~(bit)))
#define HI_BIT10_1 (0x00000001 << 10)
#define HI_BIT09_1 (0x00000001 << 9)
#define HI_BIT00_1 (0x00000001)
static void config_uart_gpio()
{
    unsigned int*  pui_reg_gpiosel = 0;
    unsigned int*  pui_reg_iosel   = 0;

    pui_reg_gpiosel = (unsigned int *)ioremap(HI_REG_BASE_IOMUX + 0x1ac,4);
    pui_reg_iosel = (unsigned int *)ioremap(HI_REG_BASE_IOMUX + 0x198,4);

    /*使能UART0 用于DECT控制通信*/
    HI_BIT_CLR(*(pui_reg_gpiosel), HI_BIT09_1);
    HI_BIT_CLR(*(pui_reg_gpiosel), HI_BIT10_1);
    HI_BIT_SET(*(pui_reg_iosel), HI_BIT00_1);
}
#endif

static hi_int32 hi_uart_init(hi_void)
{
    hi_int32  i_ret  = 0;

    hi_printk(KERN_INFO "serial: hisi uart driver\n");
	
	config_uart_gpio();

    // 注册驱动
    i_ret = uart_register_driver(&g_st_uart_driver);
    if (0 != i_ret)
    {
        hi_trace();
        return i_ret;
    }

    uart_add_one_port(&g_st_uart_driver, &g_st_uart_port[0]);

    hi_trace();
    return 0;
}

static hi_void hi_uart_exit(hi_void)
{
    uart_remove_one_port(&g_st_uart_driver, &g_st_uart_port[0]);
	uart_unregister_driver(&g_st_uart_driver);
    hi_trace();
}

module_init(hi_uart_init);
module_exit(hi_uart_exit);

EXPORT_SYMBOL(g_st_uart_driver);
EXPORT_SYMBOL(g_st_uart_port);
MODULE_DESCRIPTION("hsan serial port driver");
MODULE_AUTHOR("hisilicon Inc.");
MODULE_LICENSE("GPL");


