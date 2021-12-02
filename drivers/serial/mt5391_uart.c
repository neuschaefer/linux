
#if defined(CONFIG_SERIAL_MT5391_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/platform_device.h>

#include <asm/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/hardware.h>

#include "mt5391_uart.h"

#define DISABLE_MODEM_CONTROL

/* support uart0(debug port), uart1 and uart 2*/
#define UART_NR                 3

#define SERIAL_MT5391_MAJOR     204
#define SERIAL_MT5391_MINOR     16

#define MT5391_ISR_PASS_LIMIT   256

/*
 * Port
 */
struct mt5391_uart_port
{
    struct uart_port port;
    int nport;
    unsigned int old_status;
    unsigned int tx_stop;
    unsigned int rx_stop;
    unsigned int ms_enable;
    unsigned int baud, datalen, stopbit, parity;

    unsigned int    (*fn_read_allow)  (void);
    unsigned int    (*fn_write_allow) (void);
    void            (*fn_int_enable)  (int enable);
    void            (*fn_empty_int_enable) (int enable);
    unsigned int    (*fn_read_byte)   (void);
    void            (*fn_write_byte)  (unsigned int byte);
    void            (*fn_flush)       (void);
    void            (*fn_get_top_err) (int *p_parity, int *p_end, int *p_break);
};
static struct mt5391_uart_port _mt5391_uart_ports[];

/*
 * Locals
 */
static void _mt5391_uart_stop_tx(struct uart_port *port);

/*
 * HW functions of mt5391 uart
 */
#define UART_READ8(REG)             (((REG) < UART2_BASE) ?                    \
                                     __raw_readb(RS232_VIRT + (REG)) :         \
                                     __raw_readb(PDWNC_VIRT + (REG)))
#define UART_READ16(REG)            (((REG) < UART2_BASE) ?                    \
                                     __raw_readw(RS232_VIRT + (REG)) :         \
                                     __raw_readw(PDWNC_VIRT + (REG)))
#define UART_READ32(REG)            (((REG) < UART2_BASE) ?                    \
                                     __raw_readl(RS232_VIRT + (REG)) :         \
                                     __raw_readl(PDWNC_VIRT + (REG)))
#define UART_WRITE8(VAL, REG)       (((REG) < UART2_BASE) ?                    \
                                     __raw_writeb(VAL, RS232_VIRT + (REG)) :   \
                                     __raw_writeb(VAL, PDWNC_VIRT + (REG)))
#define UART_WRITE16(VAL, REG)      (((REG) < UART2_BASE) ?                    \
                                     __raw_writew(VAL, RS232_VIRT + (REG)) :   \
                                     __raw_writew(VAL, PDWNC_VIRT + (REG)))
#if 0
#define UART_WRITE32(VAL, REG)      (((REG) < UART2_BASE) ?                    \
                                     __raw_writel(VAL, RS232_VIRT + (REG)) :   \
                                     __raw_writel(VAL, PDWNC_VIRT + (REG))) 
#endif

static inline void UART_WRITE32(unsigned int val, unsigned int reg)
{
    if (reg < UART2_BASE)
    {
        __raw_writel(val, RS232_VIRT + reg);
    }
    else
    {
        __raw_writel(val, PDWNC_VIRT + reg);
    }
}


#define CKGEN_READ32(REG)            __raw_readl(CKGEN_VIRT + (REG))
#define CKGEN_WRITE32(VAL, REG)      __raw_writel(VAL, CKGEN_VIRT + (REG))

/*
 * Macros
 */
#define UART_REG_BITCLR(BITS, REG)      UART_WRITE32(UART_READ32(REG) & ~(BITS), REG)
#define UART_REG_BITSET(BITS, REG)      UART_WRITE32(UART_READ32(REG) | (BITS), REG)

#define PINMUX0_OFFSET          (0x0400)
#define PINMUX0_WRITE(value)            CKGEN_WRITE32((value), PINMUX0_OFFSET)
#define PINMUX0_REG()                   CKGEN_READ32(PINMUX0_OFFSET)

/*
 * HW functions
 */

static unsigned int _mt5391_u0_trans_mode_on(void)
{
    return UART_READ32(UART0_STATUS_OLD) & U0_TRANSPARENT;
}

static unsigned int _mt5391_u2_trans_mode_on(void)
{
    return UART_READ32(UART2_STATUS_OLD) & U2_TRANSPARENT;
}

static void _mt5391_uart_flush(int port)
{
    unsigned int u4Reg;
    unsigned long flags;
    
    switch (port)
    {
    case UART_PORT_0:
        u4Reg = UART0_BUFCTRL;
        break;
    case UART_PORT_1:
        u4Reg = UART1_BUFCTRL;
        break;
    case UART_PORT_2:
        u4Reg = UART2_BUFCTRL;
        break;
    default:
        return;
    }

    local_irq_save(flags);

    UART_REG_BITSET((CLEAR_TBUF | CLEAR_RBUF), u4Reg);

    local_irq_restore(flags);
}

static void _mt5391_u0_set_trans_mode(unsigned int on)
{
    if (on > 0)
    {
        UART_WRITE32(0xE2, UART0_STATUS_OLD);
    }
    else
    {
        UART_WRITE32(0, UART0_STATUS_OLD);
    }
}

static void _mt5391_u2_set_trans_mode(unsigned int on)
{
    if (on > 0)
    {
        UART_WRITE32(0xE2, UART2_STATUS_OLD);
    }
    else
    {
        UART_WRITE32(0, UART2_STATUS_OLD);
    }
}

static void _UART_HwWaitTxBufClear(unsigned int u4Port)
{
    unsigned int u4RegAddr;

    switch (u4Port)
    {
    case UART_PORT_0:
        u4RegAddr = UART0_STATUS;
        break;

    case UART_PORT_1:
        u4RegAddr = UART1_STATUS;
        break;

    case UART_PORT_2:
        u4RegAddr = UART2_STATUS;
        break;

    default:
        return;
    }

    while ((UART_READ32(u4RegAddr) & UART_WRITE_ALLOW) != UART_WRITE_ALLOW);
}

static void _mt5391_uart_hwinit(int port)
{
    unsigned int u4Tmp;
    unsigned long flags;

    local_irq_save(flags);

    if (port == 2)
    {
        /* Change PD UART control to ARM */
        u4Tmp = __raw_readl(PDWNC_VIRT + 0x178);
        u4Tmp &= ~(0x01000000);
        __raw_writel(u4Tmp, PDWNC_VIRT + 0x178);
    }
    
    if (port == 0)
    {
        /* disable uart 0 interrupt */
        u4Tmp = UART_READ32(UART_INT_EN);
        u4Tmp &= ~U0_INTMSK;
        UART_WRITE32(u4Tmp, UART_INT_EN);
    }

    if (port == 1)
    {
        /* disable uart 1 interrupt */
        u4Tmp = UART_READ32(UART_INT_EN);
        u4Tmp &= ~U1_INTMSK;
        UART_WRITE32(u4Tmp, UART_INT_EN);
    }

    if (port == 2)
    {
        /* disable uart2 interrupt */
        u4Tmp = UART_READ32(UART2_INT_EN);
        u4Tmp &= ~U2_INTMSK;
        UART_WRITE32(u4Tmp, UART2_INT_EN);
    }

    if (port == 0)
    {
        /* config uart mode */
        u4Tmp = UART_READ32(UART_RS232_MODE);
        u4Tmp &= ~DBG_MODE_MSK;
        u4Tmp |= DBG_RS232_ACTIVE;
        UART_WRITE32(u4Tmp, UART_RS232_MODE);
    }

    if (port == 2)
    {
        u4Tmp = UART_READ32(UART2_RS232_MODE);
        u4Tmp &= ~DBG_MODE_MSK;
        u4Tmp |= DBG_RS232_ACTIVE;
        UART_WRITE32(u4Tmp, UART2_RS232_MODE);
    }

    if (port == 0)
    {
        // Config normal mode buffer size
        if (UART_READ32(UART0_BUFFER_SIZE) != 0)
        {
            _mt5391_u0_set_trans_mode(0);
            UART_WRITE32(0, UART0_BUFFER_SIZE);
            _mt5391_u0_set_trans_mode(1);
        }
    }

    if (port == 2)
    {
        if (UART_READ32(UART2_BUFFER_SIZE) != 0)
        {
            _mt5391_u2_set_trans_mode(0);
            UART_WRITE32(0, UART2_BUFFER_SIZE);
            _mt5391_u2_set_trans_mode(1);
        }

    }

    if (port == 0)
    {
        // Change to transparent mode if necessary
        if (!_mt5391_u0_trans_mode_on())
        {
            _mt5391_u0_set_trans_mode(1);
        }
    }

    if (port == 2)
    {
        if (!_mt5391_u2_trans_mode_on())
        {
            _mt5391_u2_set_trans_mode(1);
        }
    }
    
    /* set buad rate to 115200 ? or leave them as the boot settings */
    /*
    UART_WRITE32(0, UART0_COMMCTRL);
    UART_WRITE32(0, UART1_COMMCTRL);
    */

    if (port == 1)
    {
        // Set uart 1 initial settings
        UART_WRITE32(0, UART1_DMAW_CTRL);
    }
    
    /*
     * rx data timeout = 15 * 1/27M sec
     * trigger level 26
     * flush rx, tx buffer
     */
    if (port == 0)
    {
        UART_WRITE32(BUFCTRL_INIT, UART0_BUFCTRL);
        _UART_HwWaitTxBufClear(UART_PORT_0);
    }
    if (port == 1)
    {
        UART_WRITE32(BUFCTRL_INIT, UART1_BUFCTRL);
        _UART_HwWaitTxBufClear(UART_PORT_1);
    }
    if (port == 2)
    {
        UART_WRITE32(BUFCTRL_INIT, UART2_BUFCTRL);
        _UART_HwWaitTxBufClear(UART_PORT_2);
    }
    
    // Clear interrupt status
    if (port == 0)
    {
        u4Tmp = ~U0_INTMSK;
        UART_WRITE32(u4Tmp, UART_INT_ID);
    }
    if (port == 1)
    {
        u4Tmp = ~U1_INTMSK;
        UART_WRITE32(u4Tmp, UART_INT_ID);
    }
    if (port == 2)
    {
        u4Tmp = ~U2_INTMSK;
        UART_WRITE32(u4Tmp, UART2_INT_ID);
    }

    if (port == 0)
    {
        // Disable modem control by default
        UART_WRITE32(0, UART0_MODEM);
    }
    if (port == 1)
    {
        // Disable modem control by default
        UART_WRITE32(0, UART1_MODEM);
    }

    if (port == 1)
    {
        // Open uart1 Tx/Rx pinmux
        PINMUX0_WRITE(PINMUX0_REG() & (unsigned int) (~0x30000));
        PINMUX0_WRITE(PINMUX0_REG() | 0x10000);
    }

    if (port == 2)
    {
        // Open uart2 Tx/Rx pinmux
        PINMUX0_WRITE(PINMUX0_REG() & (unsigned int) (~0xc0000));
        PINMUX0_WRITE(PINMUX0_REG() | 0x80000);
    }

    local_irq_restore(flags);
}

void _mt5391_uart_set(unsigned int uCOMMCTRL, int baud, int datalen, int stop, int parity)
{
    if (uCOMMCTRL == UART2_COMMCTRL)
    {
        baud = 115200;
    }
    else
    {
        switch (baud)
        {
        case 115200:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_115200), uCOMMCTRL);            
            break;
        case 230400:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_230400), uCOMMCTRL);
            break;
        case 460800:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_460800), uCOMMCTRL);
            break;
        case 921600:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_921600), uCOMMCTRL);
            break;
        case 57600:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_57600), uCOMMCTRL);
            break;
        case 38400:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_38400), uCOMMCTRL);
            break;
        case 19200:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_19200), uCOMMCTRL);
            break;
        case 9600:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_9600), uCOMMCTRL);
            break;
        case 4800:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_4800), uCOMMCTRL);
            break;
        case 2400:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_2400), uCOMMCTRL);
            break;
        case 1200:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_1200), uCOMMCTRL);
            break;
        case 300:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_300), uCOMMCTRL);
            break;
        case 110:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_110), uCOMMCTRL);
            break;
        default:
            break;
        }
    }

    /* datalen & stop */
    if (stop == 5)          // 1.5 bits
    {
        if (datalen != 5)
        {
            goto lc_set_parity;
        }
        UART_REG_BITCLR(UBIT_MASK, uCOMMCTRL);
        UART_REG_BITSET(UBIT_5_1_5, uCOMMCTRL);
    }
    else
    {
        UART_REG_BITCLR(STOP_BIT_MASK, uCOMMCTRL);
        UART_REG_BITSET((stop == 2) ? STOP_BIT_2 : STOP_BIT_1, uCOMMCTRL);
        switch (datalen)
        {
        case 8:
            UART_REG_BITCLR(DATA_BIT_MASK, uCOMMCTRL);
            UART_REG_BITSET(DATA_BIT_8, uCOMMCTRL);
            break;
        case 7:
            UART_REG_BITCLR(DATA_BIT_MASK, uCOMMCTRL);
            UART_REG_BITSET(DATA_BIT_7, uCOMMCTRL);
            break;
        case 6:
            UART_REG_BITCLR(DATA_BIT_MASK, uCOMMCTRL);
            UART_REG_BITSET(DATA_BIT_6, uCOMMCTRL);
            break;
        case 5:
            UART_REG_BITCLR(DATA_BIT_MASK, uCOMMCTRL);
            UART_REG_BITSET(DATA_BIT_5, uCOMMCTRL);
            break;
        default:
            break;
        }
    }

lc_set_parity:
    switch (parity)
    {
    case 0:
        UART_REG_BITCLR(PARITY_MASK, uCOMMCTRL);
        UART_REG_BITSET(PARITY_NONE, uCOMMCTRL);
        break;
    case 1:
        UART_REG_BITCLR(PARITY_MASK, uCOMMCTRL);
        UART_REG_BITSET(PARITY_ODD, uCOMMCTRL);
        break;
    case 2:
        UART_REG_BITCLR(PARITY_MASK, uCOMMCTRL);
        UART_REG_BITSET(PARITY_EVEN, uCOMMCTRL);
        break;
    default:
        break;
    }
}

static void _mt5391_uart_get(unsigned int uCOMMCTRL, int *p_baud, int *p_datalen, int *p_stop, int *p_parity)
{
    int baud, datalen, stop, parity;

    switch (GETBAUD(UART_READ32(uCOMMCTRL)))
    {
    case BAUD_X1:
        baud = 115200;
        break;
    case BAUD_X2:
        baud = 230400;
        break;
    case BAUD_X4:
        baud = 460800;
        break;
    case BAUD_X8:
        baud = 921600;
        break;
    case BAUD_57600:
        baud = 57600;
        break;
    case BAUD_38400:
        baud = 38400;
        break;
    case BAUD_19200:
        baud = 19200;
        break;
    case BAUD_9600:
        baud = 9600;
        break;
    case BAUD_4800:
        baud = 4800;
        break;
    case BAUD_2400:
        baud = 2400;
        break;
    case BAUD_1200:
        baud = 1200;
        break;
    case BAUD_300:
        baud = 300;
        break;
    case BAUD_110:
        baud = 110;
        break;
    default:
        baud = 115200;
        break;
    }

    switch (UART_READ32(uCOMMCTRL) & UBIT_MASK)
    {
    case UBIT_8_1:
        datalen = 8;
        stop    = 1;
        break;
    case UBIT_8_2:
        datalen = 8;
        stop    = 2;
        break;
    case UBIT_7_1:
        datalen = 7;
        stop    = 1;
        break;
    case UBIT_7_2:
        datalen = 7;
        stop    = 2;
        break;
    case UBIT_6_1:
        datalen = 6;
        stop    = 1;
        break;
    case UBIT_6_2:
        datalen = 6;
        stop    = 2;
        break;
    case UBIT_5_1:
        datalen = 5;
        stop    = 1;
        break;
    case UBIT_5_1_5:
        datalen = 5;
        stop    = 5;
        break;
    default:
        datalen = 8;
        stop    = 1;
        break;
    }

    switch (UART_READ32(uCOMMCTRL) & PARITY_MASK)
    {
    case PARITY_NONE:
        parity = 0;
        break;
    case PARITY_ODD:
        parity = 1;
        break;
    case PARITY_EVEN:
        parity = 2;
        break;
    default:
        parity = 0;
        break;
    }

    if (uCOMMCTRL == UART2_COMMCTRL)
    {
        baud = 115200;
    }

    *p_baud = baud;
    *p_datalen = datalen;
    *p_stop = stop;
    *p_parity = parity;
}

/*
 * uart member functions
 */

static unsigned int _mt5391_u0_read_allow(void)
{
    //return UART_READ32(UART0_STATUS_OLD) & U0_RD_ALLOW;
    if (_mt5391_u0_trans_mode_on())
    {
        return GET_RX_DATA_SIZE(UART_READ32(UART0_STATUS));
    }
    else
    {
        //return UART_READ32(UART0_BUFFER_SIZE) + 1;
        return 1;
    }
}

static unsigned int _mt5391_u0_write_allow(void)
{
    //return UART_READ32(UART0_STATUS_OLD) & U0_WR_ALLOW;
    if (_mt5391_u0_trans_mode_on())
    {
        return GET_TX_EMPTY_SIZE(UART_READ32(UART0_STATUS));
    }
    else
    {
        //return 0;
        return 1;
    }
}

static void _mt5391_u0_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        UART_REG_BITSET(U0_INTALL, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U0_INTALL, UART_INT_EN);
    }
    local_irq_restore(flags);
}

static void _mt5391_u0_empty_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        UART_REG_BITSET(U0_TBUF, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U0_TBUF, UART_INT_EN);
    }
    local_irq_restore(flags);
}

static unsigned int _mt5391_u0_read_byte(void)
{
    unsigned int i4Ret;

    i4Ret = UART_READ32(UART0_DATA_BYTE);

    return i4Ret;
}

static void _mt5391_u0_write_byte(unsigned int byte)
{
    
    UART_WRITE32(byte, UART0_DATA_BYTE);
}

static void _mt5391_u0_flush(void)
{
    _mt5391_uart_flush(UART_PORT_0);
    _UART_HwWaitTxBufClear(UART_PORT_0);
}

static void _mt5391_u0_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART0_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART0_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART0_STATUS) & ERR_BREAK)  ? 1 : 0;
}

static unsigned int _mt5391_u1_read_allow(void)
{
    return GET_RX_DATA_SIZE(UART_READ32(UART1_STATUS));
}

static unsigned int _mt5391_u1_write_allow(void)
{
    return GET_TX_EMPTY_SIZE(UART_READ32(UART1_STATUS));
}

static void _mt5391_u1_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        UART_REG_BITSET(U1_INTALL, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U1_INTALL, UART_INT_EN);
    }
    local_irq_restore(flags);
}

static void _mt5391_u1_empty_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        UART_REG_BITSET(U1_TBUF, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U1_TBUF, UART_INT_EN);
    }
    local_irq_restore(flags);
}

static unsigned int _mt5391_u1_read_byte(void)
{
    unsigned int i4Ret;
    i4Ret = UART_READ32(UART1_DATA_BYTE);

    return i4Ret;
}

static void _mt5391_u1_write_byte(unsigned int byte)
{
    UART_WRITE32(byte, UART1_DATA_BYTE);
}

static void _mt5391_u1_flush(void)
{
    _mt5391_uart_flush(UART_PORT_1);
    _UART_HwWaitTxBufClear(UART_PORT_1);
}

static void _mt5391_u1_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART1_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART1_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART1_STATUS) & ERR_BREAK)  ? 1 : 0;
}

static unsigned int _mt5391_u2_read_allow(void)
{
    if (_mt5391_u2_trans_mode_on())
    {
        return GET_RX_DATA_SIZE(UART_READ32(UART2_STATUS));
    }
    else
    {
        return 1;
    }
}

static unsigned int _mt5391_u2_write_allow(void)
{
    if (_mt5391_u2_trans_mode_on())
    {
        return GET_TX_EMPTY_SIZE(UART_READ32(UART2_STATUS));
    }
    else
    {
        return 1;
    }
}

static void _mt5391_u2_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        unsigned int u4Reg;
        u4Reg = __raw_readl(PDWNC_VIRT + 0x44);
        u4Reg |= 0x200;
        __raw_writel(u4Reg, PDWNC_VIRT + 0x44);
        UART_REG_BITSET(U2_INTALL, UART2_INT_EN);
    }
    else
    {
        /* FIXME: disable PDWNC interrupt ? */
        UART_REG_BITCLR(U2_INTALL, UART2_INT_EN);
    }
    local_irq_restore(flags);
}

static void _mt5391_u2_empty_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
    if (enable)
    {
        UART_REG_BITSET(U2_TBUF, UART2_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U2_TBUF, UART2_INT_EN);
    }
    local_irq_restore(flags);
}

static unsigned int _mt5391_u2_read_byte(void)
{
    unsigned int i4Ret;
    i4Ret = UART_READ32(UART2_DATA_BYTE);

    return i4Ret;
}

static void _mt5391_u2_write_byte(unsigned int byte)
{
    UART_WRITE32(byte, UART2_DATA_BYTE);
}

static void _mt5391_u2_flush(void)
{
    _mt5391_uart_flush(UART_PORT_2);
    _UART_HwWaitTxBufClear(UART_PORT_2);
}

static void _mt5391_u2_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART2_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART2_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART2_STATUS) & ERR_BREAK)  ? 1 : 0;
}

/*
 * interrupt handling
 */
static void _mt5391_uart_rx_chars(struct mt5391_uart_port *mport)
{
    struct tty_struct *tty;
    int max_count;
    unsigned int data_byte;
    unsigned int flag;
    int err_parity, err_end, err_break;
#if 0
    int err_overrun, err_status;
#endif
    
    if (!mport || !(&mport->port) || !(mport->port.state))
    {
        return;
    }

    tty = mport->port.state->port.tty;

    if (!tty)
    {
        return;
    }

    max_count = RX_BUF_SIZE;

    if (((mport->nport == 0) && !(_mt5391_u0_trans_mode_on())) ||
        ((mport->nport == 2) && !(_mt5391_u2_trans_mode_on())))
    {
        max_count = 1;
    }

    while (max_count > 0)
    {
        max_count--;

        /* check status */
        if (mport->fn_read_allow())
        {
            /* in mt5391, process error before read byte */
            mport->fn_get_top_err(&err_parity, &err_end, &err_break);

            /* read the byte */
            data_byte = mport->fn_read_byte();
            mport->port.icount.rx++;
            flag = TTY_NORMAL;

            /* error handling routine */
            if (err_break)
            {
                mport->port.icount.brk++;
                if (uart_handle_break(&mport->port))
                {
                    continue;
                }
                flag = TTY_BREAK;
            }
            else if (err_parity)
            {
                mport->port.icount.parity++;
                flag = TTY_PARITY;
            }
            else if (err_end)
            {
                mport->port.icount.frame++;
                flag = TTY_FRAME;
            }
        }
        else
        {
            break;
        }

        if (uart_handle_sysrq_char(&mport->port, data_byte))
        {
            continue;
        }

#if 1
        //2.6.18. no overrun support, set status and mask to 0
        uart_insert_char(&mport->port, 0, 0, data_byte, flag);

        //2.6.10
        //tty_insert_flip_char(tty, data_byte, flag);
        /*
        if (OVERRUN_HAPPENED)
        {
            tty_insert_flip_char(tty, 0, TTY_OVERRUN);
        }
        */
#else
        if (mport->nport == 0)
        {
            err_overrun = (UART_READ32(UART_INT_ID) & U0_OVRUN) ? 1 : 0;
        }
        else if (mport->nport == 1)
        {
            err_overrun = (UART_READ32(UART_INT_ID) & U1_OVRUN) ? 1 : 0;
        }
        else if (mport->nport == 2)
        {
            err_overrun = (UART_READ32(UART2_INT_ID) & U2_OVRUN) ? 1 : 0;
        }
        else
        {
            err_overrun = 0;
        }

        err_status = (err_break << 3) | (err_parity << 2) | (err_end << 1) | err_overrun;
        uart_insert_char(&mport->port, err_status, 1, data_byte, flag);
#endif // if 0
    }

    tty_flip_buffer_push(tty);
    //tty_schedule_flip(tty);
}

static void _mt5391_uart_tx_chars(struct mt5391_uart_port *mport)
{
    struct uart_port *port;
    struct circ_buf *xmit;
    int count;

    if (!mport || !(&mport->port) || !mport->port.state)
    {
        return;
    }
    
    port = &mport->port;
    xmit = &port->state->xmit;    

    if (!xmit)
    {
        return;
    }

    /* deal with x_char first */
    if (port->x_char)
    {
        /* make sure we have room to write */
        while (!mport->fn_write_allow())
        {
            barrier();
        }
        mport->fn_write_byte(port->x_char);
        port->icount.tx++;
        port->x_char = 0;
        return;
    }

    /* stop tx if circular buffer is empty or this port is stopped */
    if (uart_circ_empty(xmit) || uart_tx_stopped(port))
    {
        _mt5391_uart_stop_tx(port);
        return;
    }

    count = TX_BUF_SIZE;

    while ((count > 0) &&
           (mport->fn_write_allow()) &&
           !(uart_circ_empty(xmit)))
    {
        barrier();
        count--;
        mport->fn_write_byte(xmit->buf[xmit->tail]);
        xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
        port->icount.tx++;
    }
    
    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
    {
        uart_write_wakeup(port);
    }

    if (uart_circ_empty(xmit))
    {
        _mt5391_uart_stop_tx(port);
    }
}

static void _mt5391_uart_modem_state(struct mt5391_uart_port *mport)
{
    struct uart_port *port;
    unsigned int status, delta;
    unsigned int uMS;

    if (!mport || !(&mport->port))
    {
        return;
    }

    port = &mport->port;

    switch (mport->nport)
    {
    case 0:
        uMS = UART0_MODEM;
        break;
    case 1:
        uMS = UART1_MODEM;
        break;
    default:
        return;
    } 
        
    status = UART_READ32(uMS);        /* actually, support uart 1 only */
    status &= MDM_DSR | MDM_CTS | MDM_DCD;

    delta = status ^ mport->old_status;


    if (!delta)
    {
        return;
    }

    if (mport->ms_enable)
    {
        if (delta & MDM_DCD)
        {
            uart_handle_dcd_change(port, status & MDM_DCD);
        }
        if (delta & MDM_DSR)
        {
            port->icount.dsr++;
        }
        if (delta & MDM_CTS)
        {
            uart_handle_cts_change(port, status & MDM_CTS);
        }

        wake_up_interruptible(&port->state->port.delta_msr_wait);
    }

    mport->old_status = status;
}

/*
 * uart ops
 */

/* called from thread context */
static unsigned int _mt5391_uart_tx_empty(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned int count;
    unsigned long flags;

    if (!port)
    {
        return 0;
    }

    mport = (struct mt5391_uart_port *)port;

    spin_lock_irqsave(&port->lock, flags);
    count = mport->fn_write_allow();
    spin_unlock_irqrestore(&port->lock, flags);
    
    return ((count == UART_FIFO_SIZE) ? TIOCSER_TEMT : 0);
}

static void _mt5391_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
    struct mt5391_uart_port *mport;
    unsigned int regval;
    unsigned int uMS;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;
    if (!mport->ms_enable)
    {
        return;
    }
    
    switch (mport->nport)
    {
    case 0:
        uMS = UART0_MODEM;
        break;
    case 1:
        uMS = UART1_MODEM;
        break;
    default:
        return;
    } 

    local_irq_save(flags);
    
    regval = UART_READ32(uMS);

    if (mctrl & TIOCM_RTS)
    {
        regval |= MDM_RTS;
    }
    else
    {
        regval &= ~MDM_RTS;
    }

    if (mctrl & TIOCM_DTR)
    {
        regval |= MDM_DTR;
    }
    else
    {
        regval &= ~MDM_DTR;
    }

    UART_WRITE32(regval, uMS);

    local_irq_restore(flags);
}

static unsigned int _mt5391_uart_get_mctrl(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned int modem_status;
    unsigned int result = 0;
    unsigned int uMS;

    if (!port)
    {
        return 0;
    }

    mport = (struct mt5391_uart_port *)port;

    if (!mport->ms_enable)
    {
        return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
    }
    
    switch (mport->nport)
    {
    case 0:
        uMS = UART0_MODEM;
        break;
    case 1:
        uMS = UART1_MODEM;
        break;
    default:
        // no modem control lines
        return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
    } 

    modem_status = UART_READ32(uMS);
    if (modem_status & MDM_DCD)
    {
        result |= TIOCM_CAR;
    }
    if (modem_status & MDM_DSR)
    {
        result |= TIOCM_DSR;
    }
    if (modem_status & MDM_CTS)
    {
        result |= TIOCM_CTS;
    }

    return result;
}

static void _mt5391_uart_stop_tx(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;
    
    //mport->fn_empty_int_enable(0);
    local_irq_save(flags);
    mport->tx_stop = 1;
    local_irq_restore(flags);
}

static void _mt5391_uart_start_tx(struct uart_port *port)
{
    //struct circ_buf *xmit = &port->info->xmit;
    struct mt5391_uart_port *mport;
    unsigned long flags;

    //mport->fn_empty_int_enable(1);

    if (!port)
    {
        return;
    }
    
    mport = (struct mt5391_uart_port *)port;

    local_irq_save(flags);
    
    if (mport->fn_write_allow())
    {
        _mt5391_uart_tx_chars(mport);
    }
    mport->tx_stop = 0;

    local_irq_restore(flags);
}


static void _mt5391_uart_stop_rx(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;

    local_irq_save(flags);
    mport->rx_stop = 1;
    local_irq_restore(flags);
}

static void _mt5391_uart_enable_ms(struct uart_port *port)
{
    struct mt5391_uart_port *mport;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;

#ifdef DISABLE_MODEM_CONTROL
    mport->ms_enable = 0;
#endif // DISABLE_MODEM_CONTROL
}

static void _mt5391_uart_break_ctl(struct uart_port *port, int break_state)
{
    struct mt5391_uart_port *mport;
    unsigned int regval;
    unsigned long flags;
    unsigned int uCOMMCTRL;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;
    
    switch (mport->nport)
    {
    case 0:
        uCOMMCTRL = UART0_COMMCTRL;
        break;
    case 1:
        uCOMMCTRL = UART1_COMMCTRL;
        break;
    case 2:
        uCOMMCTRL = UART2_COMMCTRL;
        break;
    default:
        return;
    } 

    local_irq_save(flags);
    regval = UART_READ32(uCOMMCTRL);
    if (break_state == -1)
    {
        regval |= CONTROL_BREAK;
    }
    else
    {
        regval &= ~CONTROL_BREAK;
    }
    UART_WRITE32(regval, uCOMMCTRL);
    local_irq_restore(flags);
}

static int _u0_irq_allocated=0, _u1_irq_allocated=0, _u2_irq_allocated=0;
static irqreturn_t _mt5391_uart0_interrupt(int irq, void *dev_id);
static irqreturn_t _mt5391_uart1_interrupt(int irq, void *dev_id);
static irqreturn_t _mt5391_uart2_interrupt(int irq, void *dev_id);

static int _mt5391_uart_startup(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned long flags;
    int retval;
    int u0irq, u1irq, u2irq;
    
    if (!port)
    {
        return -1;
    }

    mport = (struct mt5391_uart_port *)port;
  
    /* FIXME: hw init? */
    _mt5391_uart_hwinit(mport->nport);

    if (mport->nport == 0)
    {
        _u0_irq_allocated ++;
    }
    else if (mport->nport == 1)
    {
        _u1_irq_allocated ++;
    }
    else
    {
        _u2_irq_allocated ++;
    }
    u0irq = _u0_irq_allocated;
    u1irq = _u1_irq_allocated;
    u2irq = _u2_irq_allocated;

    if ((mport->nport == 0) && (u0irq == 1))
    {
        /* allocate irq, u0/u1 share the same interrupt number */
        retval = request_irq(port->irq, _mt5391_uart0_interrupt, IRQF_SHARED,
                             "MT5391 Serial 0", port);
        if (retval)
        {
            _u0_irq_allocated --;
            return retval;
        }
    }

    if ((mport->nport == 1) && (u1irq == 1))
    {
        /* allocate irq, u0/u1 share the same interrupt number */
        retval = request_irq(port->irq, _mt5391_uart1_interrupt, IRQF_SHARED,
                             "MT5391 Serial 1", port);
        if (retval)
        {
            _u1_irq_allocated --;
            return retval;
        }
    }

    if ((mport->nport == 2) && (u2irq == 1))
    {
        /* allocate irq, u0/u1 share the same interrupt number */
        retval = request_irq(port->irq, _mt5391_uart2_interrupt, IRQF_SHARED,
                             "MT5391 Serial 2", port);
        if (retval)
        {
            _u2_irq_allocated --;
            return retval;
        }
    }

    /* enable interrupt */
    mport->fn_empty_int_enable(1);
    mport->fn_int_enable(1);

    local_irq_save(flags);
    if (!(__raw_readl(BIM_VIRT + REG_RW_IRQEN) & (1 << port->irq)))
    {
        __raw_writel(__raw_readl(BIM_VIRT + REG_RW_IRQEN) | (1 << port->irq),
                     BIM_VIRT + REG_RW_IRQEN);
    }
    local_irq_restore(flags);
    return 0;
}

static void _mt5391_uart_shutdown(struct uart_port *port)
{
    struct mt5391_uart_port *mport;
    unsigned long flags;
    int u0irq, u1irq, u2irq;
        
    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;

    /* 
     * FIXME: disable BIM IRQ enable bit if all ports are shutdown?
     * PDWNC may need BIM interrupt
     */
    if (mport->nport == 0)
    {
        if (_u0_irq_allocated > 0)
        {
            _u0_irq_allocated --;
        }
    }
    else if (mport->nport == 1)
    {
        if (_u1_irq_allocated > 0)
        {
            _u1_irq_allocated --;
        }
    }
    else
    {
        if (_u2_irq_allocated > 0)
        {
            _u2_irq_allocated --;
        }
    }
    u0irq = _u0_irq_allocated;
    u1irq = _u1_irq_allocated;
    u2irq = _u2_irq_allocated;
    
    if ((mport->nport == 0) && (u0irq == 0))
    {
        /* disable interrupt and disable port */
        mport->fn_empty_int_enable(0);
        mport->fn_int_enable(0);
        free_irq(port->irq, port);
    }
    
    if ((mport->nport == 1) && (u1irq == 0))
    {
        /* disable interrupt and disable port */
        mport->fn_empty_int_enable(0);
        mport->fn_int_enable(0);
        free_irq(port->irq, port);
    }

    if (((mport->nport == 0) || (mport->nport == 1)) &&
        (u0irq == 0) && (u1irq == 0))
    {
        local_irq_save(flags);
        __raw_writel(__raw_readl(BIM_VIRT + REG_RW_IRQEN) & ~(1 << port->irq),
                     BIM_VIRT + REG_RW_IRQEN);
        local_irq_restore(flags);
    }

    if ((mport->nport == 2) && (u2irq == 0))
    {
        /* disable interrupt and disable port */
        mport->fn_empty_int_enable(0);
        mport->fn_int_enable(0);
        
        local_irq_save(flags);
        //FIXME
        //__raw_writel(__raw_readl(BIM_VIRT + REG_RW_IRQEN) & ~(1 << port->irq),
        //             BIM_VIRT + REG_RW_IRQEN);
        local_irq_restore(flags);

        free_irq(port->irq, port);
    }

    /* reset uart port */
    _mt5391_uart_hwinit(mport->nport);
}

static unsigned char _mt5391_uart_cal_rxd_timeout(unsigned int baud)
{
    unsigned int wait_time;

    wait_time = 1000000 / (baud / 8);

    if (wait_time < (64 * 128 / 30))
    {
        return (wait_time * 30 / 128);
    } else if (wait_time < (64 * 512 / 30))
    {
        return (0x40 | (wait_time * 30 / 512));
    } else if (wait_time < (64 * 30 / 2048))
    {
        return (0x80 | (wait_time * 30 / 2048));
    }else if (wait_time < (64 * 8192 / 30))
    {
        return (0xC0 | (wait_time * 30 /8192));
    }
    else
    {
        return 0xF;
    }
}

static void _mt5391_uart_set_termios(struct uart_port *port,
    struct ktermios *termios, struct ktermios *old)
{
    struct mt5391_uart_port *mport;
    unsigned long flags;
    int baud;
    int datalen;
    int parity = 0;
    int stopbit = 1;
    unsigned int uCOMMCTRL, uMS;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;
            
    switch (mport->nport)
    {
    case 0:
        uCOMMCTRL = UART0_COMMCTRL;
        uMS = UART0_MODEM;
        break;
    case 1:
        uCOMMCTRL = UART1_COMMCTRL;
        uMS = UART1_MODEM;
        break;
    case 2:
        uCOMMCTRL = UART2_COMMCTRL;
        uMS = UART0_MODEM;
        break;
    default:
        return;
    } 

    /* calculate baud rate */
    baud = (int)uart_get_baud_rate(port, termios, old, 0, port->uartclk);
    /* datalen : default 8 bit */
    switch (termios->c_cflag & CSIZE)
    {
    case CS5:
        datalen = 5;
        break;
    case CS6:
        datalen = 6;
        break;
    case CS7:
        datalen = 7;
        break;
    case CS8:
    default:
        datalen = 8;
        break;
    }

    /* stopbit : default 1 */
    if (termios->c_cflag & CSTOPB)
    {
        stopbit = 2;
    }
    /* parity : default none */
    if (termios->c_cflag & PARENB)
    {
        if (termios->c_cflag & PARODD)
        {
            parity = 1; /* odd */
        }
        else
        {
            parity = 2; /* even */
        }
    }

    //_mt5391_uart_flush(mport->nport);
    if ((mport->baud != baud) ||
        (mport->datalen != datalen) ||
        (mport->stopbit != stopbit) ||
        (mport->parity != parity))
    {
        unsigned int wait_time;
        if (mport->baud != 0)
        {
            wait_time = 1000000 / (mport->baud / 8);
        }
        else
        {
            wait_time = 1000000 / (baud / 8);
        }

        udelay(wait_time * 2);

        mport->baud = baud;
        mport->datalen = datalen;
        mport->stopbit = stopbit;
        mport->parity = parity;
    }
    
    /* lock from here */
    spin_lock_irqsave(&port->lock, flags);

    /* update per port timeout */
    uart_update_timeout(port, termios->c_cflag, baud);
    /* read status mask */
    if (termios->c_iflag & INPCK)
    {
        /* frame error, parity error */
        port->read_status_mask |= UST_FRAME_ERROR | UST_PARITY_ERROR;
    }
    if (termios->c_iflag & (BRKINT | PARMRK))
    {
        /* break error */
        port->read_status_mask |= UST_BREAK_ERROR;
    }
    /* status to ignore */
    port->ignore_status_mask = 0;
    if (termios->c_iflag & IGNPAR)
    {
        port->ignore_status_mask |= UST_FRAME_ERROR | UST_PARITY_ERROR;
    }
    if (termios->c_iflag & IGNBRK)
    {
        port->ignore_status_mask |= UST_BREAK_ERROR;
        if (termios->c_iflag & IGNPAR)
        {
            port->ignore_status_mask |= UST_OVRUN_ERROR;
        }
    }
    if ((termios->c_cflag & CREAD) == 0)
    {
        // dummy read
        port->ignore_status_mask |= UST_DUMMY_READ;
    }
    
    _mt5391_uart_set(uCOMMCTRL, baud, datalen, stopbit, parity);
    
    /* change rxd timeout period */
    switch (mport->nport)
    {
    case 0:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART0_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt5391_uart_cal_rxd_timeout(baud)) , UART0_BUFCTRL);
        break;
    case 1:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART1_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt5391_uart_cal_rxd_timeout(baud)) , UART1_BUFCTRL);
        break;
    case 2:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART2_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt5391_uart_cal_rxd_timeout(baud)) , UART2_BUFCTRL);
        break;
    default:
        break;
    }
    
    /* hw rts/cts ? */
#ifndef DISABLE_MODEM_CONTROL
    if ((mport->ms_enable) && (mport->nport != 2) && (termios->c_cflag & CRTSCTS))
    {
        UART_REG_BITSET(MDM_HW_RTS | MDM_HW_CTS, uMS);
    }
#endif // DISABLE_MODEM_CONTROL
    
    /* disable modem status interrupt */
    mport->ms_enable = 0;
    
#ifndef DISABLE_MODEM_CONTROL
    if ((mport->nport != 2) && (UART_ENABLE_MS(port, termios->c_cflag)))
    {
        // enable modem status interrupt
        mport->ms_enable = 1;
    }
#endif // DISABLE_MODEM_CONTROL
    /* unlock here */
    spin_unlock_irqrestore(&port->lock, flags);
}

static const char *_mt5391_uart_type(struct uart_port *port)
{
    return "MT5391 Serial";
}

static void _mt5391_uart_release_port(struct uart_port *port)
{
    release_mem_region(port->mapbase, MT5391_UART_SIZE);
}

static int _mt5391_uart_request_port(struct uart_port *port)
{
    void *pv_region;

    pv_region = request_mem_region(port->mapbase, MT5391_UART_SIZE,
                                   "MT5391 Uart IO Mem");
    return pv_region != NULL ? 0 : -EBUSY;
}

static void _mt5391_uart_config_port(struct uart_port *port, int flags)
{
    if (flags & UART_CONFIG_TYPE)
    {
        port->type = PORT_MT5391;
        _mt5391_uart_request_port(port);
    }
}

static int _mt5391_uart_verify_port(struct uart_port *port,
                                    struct serial_struct *ser)
{
	int ret = 0;
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_MT5391)
	{
		ret = -EINVAL;
	}
	if (ser->irq != port->irq)
	{
		ret = -EINVAL;
	}
	if (ser->baud_base < 110)
	{
		ret = -EINVAL;
	}
	return ret;
}

static struct uart_ops _mt5391_uart_ops =
{
    .tx_empty       = _mt5391_uart_tx_empty,
    .set_mctrl      = _mt5391_uart_set_mctrl,
    .get_mctrl      = _mt5391_uart_get_mctrl,
    .stop_tx        = _mt5391_uart_stop_tx,
    .start_tx       = _mt5391_uart_start_tx,
    /* .send_xchar */
    .stop_rx        = _mt5391_uart_stop_rx,
    .enable_ms      = _mt5391_uart_enable_ms,
    .break_ctl      = _mt5391_uart_break_ctl,
    .startup        = _mt5391_uart_startup,
    .shutdown       = _mt5391_uart_shutdown,
    .set_termios    = _mt5391_uart_set_termios,
    /* .pm */
    /* .set_wake */
    .type           = _mt5391_uart_type,
    .release_port   = _mt5391_uart_release_port,
    .request_port   = _mt5391_uart_request_port,
    .config_port    = _mt5391_uart_config_port,
    .verify_port    = _mt5391_uart_verify_port,
};

static struct mt5391_uart_port _mt5391_uart_ports[] =
{
    {
        .port =
        {
            .membase        = (void*)MT5391_VA_UART,
            .mapbase        = MT5391_PA_UART,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_0,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt5391_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 0,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 0,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt5391_u0_read_allow,
        .fn_write_allow     = _mt5391_u0_write_allow,
        .fn_int_enable      = _mt5391_u0_int_enable,
        .fn_empty_int_enable    = _mt5391_u0_empty_int_enable,
        .fn_read_byte       = _mt5391_u0_read_byte,
        .fn_write_byte      = _mt5391_u0_write_byte,
        .fn_flush           = _mt5391_u0_flush,
        .fn_get_top_err     = _mt5391_u0_get_top_err,
    },
    {
        .port =
        {
            .membase        = (void*)MT5391_VA_UART,
            .mapbase        = MT5391_PA_UART,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_1,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt5391_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 1,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 1,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt5391_u1_read_allow,
        .fn_write_allow     = _mt5391_u1_write_allow,
        .fn_int_enable      = _mt5391_u1_int_enable,
        .fn_empty_int_enable    = _mt5391_u1_empty_int_enable,
        .fn_read_byte       = _mt5391_u1_read_byte,
        .fn_write_byte      = _mt5391_u1_write_byte,
        .fn_flush           = _mt5391_u1_flush,
        .fn_get_top_err     = _mt5391_u1_get_top_err,
    },
    {
        .port =
        {
            .membase        = (void*)MT5391_VA_UART2,
            .mapbase        = MT5391_PA_UART2,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_2,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt5391_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 2,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 2,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt5391_u2_read_allow,
        .fn_write_allow     = _mt5391_u2_write_allow,
        .fn_int_enable      = _mt5391_u2_int_enable,
        .fn_empty_int_enable= _mt5391_u2_empty_int_enable,
        .fn_read_byte       = _mt5391_u2_read_byte,
        .fn_write_byte      = _mt5391_u2_write_byte,
        .fn_flush           = _mt5391_u2_flush,
        .fn_get_top_err     = _mt5391_u2_get_top_err,
    },
};

static irqreturn_t _mt5391_uart0_interrupt(int irq, void *dev_id)
{
    unsigned int uart_int_ident;
    struct mt5391_uart_port *mport = (struct mt5391_uart_port *)dev_id;
    irqreturn_t ret = IRQ_NONE;

    uart_int_ident = UART_READ32(UART_INT_ID);
    uart_int_ident &= U0_INTMSK;

#if 0
    if (((uart_int_ident & UART_READ32(UART_INT_EN)) == 0) || (!mport))
    {
        return IRQ_NONE;
    }
#endif

    /* take care of SA_SHIRQ and return IRQ_NONE if possible [LDD/279] */

    /* ack u0 interrupt */
    UART_WRITE32(~uart_int_ident, UART_INT_ID);
    if (mport != &_mt5391_uart_ports[0])
    {
        while (1)
            ;
    }
    //_mt5391_handle_uart_interrupt(mport, regs, uart_int_ident & U0_TBUF);

    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U0_RBUF | U0_RERR)))
    {
        _mt5391_uart_rx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* tx mode */
    if (mport->fn_write_allow() || (uart_int_ident & U0_TBUF))
    {
        _mt5391_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt5391_uart_modem_state(mport);
    
    return ret;
}

static irqreturn_t _mt5391_uart1_interrupt(int irq, void *dev_id)
{
    unsigned int uart_int_ident;
    struct mt5391_uart_port *mport = (struct mt5391_uart_port *)dev_id;
    irqreturn_t ret = IRQ_NONE;
    
    uart_int_ident = UART_READ32(UART_INT_ID);
    uart_int_ident &= U1_INTMSK;

#if 0
    if (((uart_int_ident & UART_READ32(UART_INT_EN)) == 0) || (!mport))
    {
        return IRQ_NONE;
    }
#endif
    
    /* take care of SA_SHIRQ and return IRQ_NONE if possible [LDD/279] */

    /* ack u1 interrupt */
    UART_WRITE32(~uart_int_ident, UART_INT_ID);
    if (mport != &_mt5391_uart_ports[1])
    {
        while (1)
            ;
    }
    //_mt5391_handle_uart_interrupt(mport, regs, uart_int_ident & U1_TBUF);

    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U1_RBUF | U1_RERR)))
    {
        _mt5391_uart_rx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* tx mode */
    if (mport->fn_write_allow() || (uart_int_ident & U1_TBUF))
    {
        _mt5391_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt5391_uart_modem_state(mport);
    
    return ret;
}

static irqreturn_t _mt5391_uart2_interrupt(int irq, void *dev_id)
{
    unsigned int uart_int_ident;
    struct mt5391_uart_port *mport = (struct mt5391_uart_port *)dev_id;
    irqreturn_t ret = IRQ_NONE;

    uart_int_ident = UART_READ32(UART2_INT_ID);
    uart_int_ident &= U2_INTMSK;

#if 0
    if (((uart_int_ident & UART_READ32(UART2_INT_EN)) == 0) || (!mport))
    {
        return IRQ_NONE;
    }
#endif
    
    /* take care of SA_SHIRQ and return IRQ_NONE if possible [LDD/279] */

    /* ack u2 interrupt */
    UART_WRITE32(~uart_int_ident, UART2_INT_ID);
    if (mport != &_mt5391_uart_ports[2])
    {
        while (1)
            ;
    }
    //_mt5391_handle_uart_interrupt(mport, regs, uart_int_ident & U2_TBUF);

    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U2_RBUF | U2_RERR)))
    {
        _mt5391_uart_rx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* tx mode */
    if (mport->fn_write_allow() || (uart_int_ident & U2_TBUF))
    {
        _mt5391_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt5391_uart_modem_state(mport);

    return ret;
}

/*
 * console
 */

#ifdef CONFIG_SERIAL_MT5391_CONSOLE

static void _mt5391_uart_console_write(struct console *co, const char *s,
    unsigned int count)
{
    static int hw_init=0;
    int i;
    struct mt5391_uart_port *mport;

    if (((co->index == 0) && (!_mt5391_u0_trans_mode_on())) ||
        ((co->index == 2) && (!_mt5391_u2_trans_mode_on())))
    {
        return;
    }
    
    if (co->index >= UART_NR)
    {
        return;
    }

    if (!hw_init)
    {
        hw_init = 1;
        _mt5391_uart_hwinit(co->index);
    }
    
    mport = &_mt5391_uart_ports[co->index];
    for (i = 0; i < count; i++)
    {
        while (!mport->fn_write_allow())
        {
            barrier();
        }
        mport->fn_write_byte(s[i]);

        if (s[i] == '\n')
        {
            while (!mport->fn_write_allow())
            {
                barrier();
            }
            mport->fn_write_byte('\r');
        }
    }
}

static void __init _mt5391_uart_console_get_options(struct uart_port *port,
    int *pbaud, int *pparity, int *pbits)
{
    int baud=0, parity=0, stopbit, datalen;
    struct mt5391_uart_port *mport;
    unsigned int uCOMMCTRL;

    if (!port)
    {
        return;
    }

    mport = (struct mt5391_uart_port *)port;
    
    switch (mport->nport)
    {
    case 0:
        uCOMMCTRL = UART0_COMMCTRL;
        break;
    case 1:
        uCOMMCTRL = UART1_COMMCTRL;
        break;
    case 2:
        uCOMMCTRL = UART2_COMMCTRL;
        break;
    default:
        if (pbaud)
        {
            *pbaud = 115200;
        }
        if (pparity)
        {
            *pparity = 'n';
        }
        if (pbits)
        {
            *pbits = 8;
        }
        return;
    } 

    _mt5391_uart_get(uCOMMCTRL, &baud, &datalen, &stopbit, &parity);

    if (pbaud)
    {
        *pbaud = baud;
    }

    if (pparity)
    {
        switch (parity)
        {
        case 1:
            *pparity = 'o';
            break;
        case 2:
            *pparity = 'e';
            break;
        case 0:
        default:
            *pparity = 'n';
            break;
        }
    }

    if (pbits)
    {
        *pbits = datalen;
    }
}

static int __init _mt5391_uart_console_setup(struct console *co, char *options)
{
    struct uart_port *port;
    int baud    = 115200;
    int bits    = 8;
    int parity  = 'n';
    int flow    = 'n';
    int stopbit;
    int ret;

    printk(KERN_DEBUG "5391 console setup : co->index %d options:%s\n",
        co->index, options);

    if (co->index >= UART_NR)
    {
        co->index = 0;
    }
    
    port = (struct uart_port *)&_mt5391_uart_ports[co->index];

    if (options)
    {
        uart_parse_options(options, &baud, &parity, &bits, &flow);
    }
    else
    {
        _mt5391_uart_console_get_options(port, &baud, &parity, &stopbit);
    }

    ret = uart_set_options(port, co, baud, parity, bits, flow);
    printk(KERN_DEBUG "5391 console setup : uart_set_option port(%d) "
        "baud(%d) parity(%c) bits(%d) flow(%c) - ret(%d)\n",
        co->index, baud, parity, bits, flow, ret);
    return ret;
}


static struct uart_driver _mt5391_uart_reg;
static struct console _mt5391_uart_console =
{
    .name       = "ttyMT",
    .write      = _mt5391_uart_console_write,
    .device     = uart_console_device,
    .setup      = _mt5391_uart_console_setup,
    .flags      = CON_PRINTBUFFER,
    .index      = -1,
    .data       = &_mt5391_uart_reg,
};

static int __init _mt5391_uart_console_init(void)
{
    // set to transparent if boot loader doesn't do this
    //_mt5391_u0_set_trans_mode_on();
    register_console(&_mt5391_uart_console);
    return 0;
}

console_initcall(_mt5391_uart_console_init);

static int __init _mt5391_late_console_init(void)
{
    if (!(_mt5391_uart_console.flags & CON_ENABLED))
    {
        register_console(&_mt5391_uart_console);
    }
    return 0;
}

late_initcall(_mt5391_late_console_init);

#define MT5391_CONSOLE &_mt5391_uart_console
#else

#define MT5391_CONSOLE NULL

#endif /* CONFIG_SERIAL_MT5391_CONSOLE */


static struct uart_driver _mt5391_uart_reg =
{
    .owner          = THIS_MODULE,
    .driver_name    = "MT5391 serial",
    .dev_name       = "ttyMT",
    .major          = SERIAL_MT5391_MAJOR,
    .minor          = SERIAL_MT5391_MINOR,
    .nr             = UART_NR,
    .cons           = MT5391_CONSOLE,
};

/*
* probe? remove? suspend? resume? ids?
*/

static int _mt5391_uart_probe(struct device *_dev)
{
    struct platform_device *dev = to_platform_device(_dev);

    if (!dev)
    {
        return 0;
    }

    _mt5391_uart_ports[dev->id].port.dev = _dev;
    uart_add_one_port(&_mt5391_uart_reg, &_mt5391_uart_ports[dev->id].port);
    dev_set_drvdata(_dev, &_mt5391_uart_ports[dev->id]);
    return 0;
}

static int _mt5391_uart_remove(struct device *_dev)
{
    struct mt5391_uart_port *mport = dev_get_drvdata(_dev);

    dev_set_drvdata(_dev, NULL);

    if (mport)
    {
        uart_remove_one_port(&_mt5391_uart_reg, &mport->port);
    }
    return 0;
}

static int _mt5391_uart_suspend(struct device * _dev, pm_message_t state)
{
#if 0
    struct mt5391_uart_port *mport = dev_get_drvdata(_dev);

    if (mport && level == SUSPEND_DISABLE)
    {
        uart_suspend_port(&_mt5391_uart_reg, &mport->port);
    }
#endif
    return 0;
}

static int _mt5391_uart_resume(struct device *_dev)
{
#if 0
    struct mt5391_uart_port *mport = dev_get_drvdata(_dev);

    if (mport && (level == RESUME_ENABLE))
    {
        uart_resume_port(&_mt5391_uart_reg, &mport->port);
    }
#endif
    return 0;
}

static struct device_driver _mt5391_uart_driver =
{
    .name           = "MT5391 uart",
    .bus            = &platform_bus_type,
    .probe          = _mt5391_uart_probe,
    .remove         = _mt5391_uart_remove,
    .suspend        = _mt5391_uart_suspend,
    .resume         = _mt5391_uart_resume,
};


/*
* init, exit and module
*/

static int __init _mt5391_uart_init(void)
{
    int ret;
    int i;

    /* reset hardware */
    for (i = 0; i < UART_NR; i++)
    {
        _mt5391_uart_hwinit(i);  /* all port 115200, no int */
    }
    
    printk(KERN_INFO "Serial: MT5391 driver $Revision: #1 $\n");
    ret = uart_register_driver(&_mt5391_uart_reg);
    printk(KERN_DEBUG "Serial: uart_register_driver %d\n", ret);
    if (ret)
    {
        goto out;
    }

    for (i = 0; i < UART_NR; i++)
    {
        printk(KERN_DEBUG "Serial : add uart port %d\n", i);
        uart_add_one_port(&_mt5391_uart_reg, &_mt5391_uart_ports[i].port);
    }

    ret = driver_register(&_mt5391_uart_driver);
    printk(KERN_DEBUG "Serial: driver_register %d\n", ret);
    if (ret == 0)
    {
        goto out;
    }

//unreg:
//    uart_unregister_driver(&_mt5391_uart_reg);

out:
    return ret;
}

static void __exit _mt5391_uart_exit(void)
{
    uart_unregister_driver(&_mt5391_uart_reg);
}

module_init(_mt5391_uart_init);
module_exit(_mt5391_uart_exit);

MODULE_AUTHOR("MediaTek Inc.");
MODULE_DESCRIPTION("MT5391 serial port driver $Revision: #1 $");
MODULE_LICENSE("MKL");
