
#if defined(CONFIG_SERIAL_MT53XX_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#ifdef CONFIG_SERIAL_MT53XX_FACTORY_SUPPORT
#define CONFIG_TTYMT7_SUPPORT
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
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>

#include <asm/delay.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <mach/hardware.h>
#include <linux/version.h>

#include "mt53xx_uart.h"

#define DISABLE_MODEM_CONTROL

#define ENABLE_UDMA
//#define ENABLE_DMA_CHK
#define U0_U1_SHARE_IRQ_HANDLER

#ifdef ENABLE_UDMA
#define UDMA_BUFFER_SIZE 16384
#endif

#define MIN(x,y) (x<y?x:y)


/* support uart0(debug port), uart1 and uart 2*/
#ifdef CONFIG_MT53_FPGA
#define UART_NR                 1
#else
#ifdef CONFIG_TTYMT7_SUPPORT
#define UART_NR                 4
// Uart only handle visible character. 
// When receive character > UART_MIN_FACTORY_KEY and character = _u4MagicMaxInputForceFactory or _u4MagicMinInputForceFactory
// it will enter factory mode. Uart driver will notify MW by _pfMwFactoryInput() when receive new character.
// MW will receive data by calling  UART_Read().
#define UART_MIN_FACTORY_KEY    (0x20)
#define UART_MAX_FACTORY_KEY    (0x80)
#else
#define UART_NR                 3
#endif /* CONFIG_TTYMT7_SUPPORT */
#endif /* CONFIG_FPGA */

#define SERIAL_MT53xx_MAJOR     204
#define SERIAL_MT53xx_MINOR     16

#define MT53xx_ISR_PASS_LIMIT   256

/*
 * Port
 */
struct mt53xx_uart_port
{
    struct uart_port port;
    int nport;
    unsigned int old_status;
    unsigned int tx_stop;
    unsigned int rx_stop;
    unsigned int ms_enable;
    unsigned int baud, datalen, stopbit, parity;
    int hw_init;
#ifdef ENABLE_UDMA
    unsigned char *pu1DmaBuffer;
    unsigned char *pu1DrvBuffer;
#endif

    unsigned int    (*fn_read_allow)  (void);
    unsigned int    (*fn_write_allow) (void);
    void            (*fn_int_enable)  (int enable);
    void            (*fn_empty_int_enable) (int enable);
    unsigned int    (*fn_read_byte)   (void);
    void            (*fn_write_byte)  (unsigned int byte);
    void            (*fn_flush)       (void);
    void            (*fn_get_top_err) (int *p_parity, int *p_end, int *p_break);
};
static struct mt53xx_uart_port _mt53xx_uart_ports[];

#ifdef CONFIG_TTYMT7_SUPPORT
static int _enable_ttyMT7 = 0;
static unsigned int _u4MagicInputForceFactory = 0x60;
static unsigned int _u4NumMagicInputForceFactory = 0;
static unsigned int _au4OtherMagicInputForceFactory[4] = {0x60, 0x60, 0x60, 0x60};
void* uart_sem = NULL;
#endif

/*
 * Locals
 */
static void _mt53xx_uart_stop_tx(struct uart_port *port);

/*
 * HW functions of mt53xx uart
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
#ifdef ENABLE_UDMA
static unsigned int _mt5395_u1_HwGetRxDataCnt(void)
{
    return UART_READ32(UART1_DMAW_LEVEL);
}

static void _mt5395_u1_HwDmaWriteDram(unsigned int u4SPTR, unsigned int u4Size, unsigned int u4Threshold)
{
    unsigned int u4Reg = 0;

    u4Threshold &= 0xFFFFFF;
//    ASSERT(u4Threshold <= u4Size);
    
    UART_WRITE32(0, UART1_DMAW_EN);         

    u4Reg = UART_READ32(UART_INT_EN);
    u4Reg |= U1_WBUF_TOUT | U1_WBUF_FULL | U1_WBUF_OVER;
    UART_WRITE32(u4Reg, UART_INT_EN);

    UART_WRITE32(u4SPTR, UART1_DMAW_SPTR);
    UART_WRITE32((u4SPTR + u4Size), UART1_DMAW_EPTR);

    UART_WRITE32(1, UART1_DMAW_RST);
    UART_WRITE32((u4Threshold << 8) | 3, UART1_DMAW_EN);
}

static void _mt5395_u1_HwDmaReadDisable(void)
{
    UART_WRITE32(0, UART1_DMAR_EN);         
}

static unsigned int _mt5395_u1_GetRxDMABuf(unsigned char *pBuffer, unsigned int u4NumToRead)
{
    unsigned int u4WPTR;
    unsigned int u4RPTR;    
    unsigned int u4EPTR;    
    unsigned int u4SPTR;    
    unsigned int u4RxLength = 0;    
    unsigned int u4Length;    
    
    // only uart 1 support DMA buffer.
//    ASSERT(u4Port== UART_PORT_1);
//    ASSERT(pBuffer);

    u4Length = UART_READ32(UART1_DMAW_LEVEL);
    if (u4Length == 0)
    {
        return 0;
    }

    u4WPTR = UART_READ32(UART1_DMAW_WPTR);
    u4RPTR = UART_READ32(UART1_DMAW_RPTR);    
    u4SPTR = UART_READ32(UART1_DMAW_SPTR);            
    u4EPTR = UART_READ32(UART1_DMAW_EPTR);            

    if (u4WPTR > u4RPTR)
    {
        u4Length = MIN((u4WPTR - u4RPTR), u4NumToRead);
        memcpy((void *)pBuffer, phys_to_virt((const void *)u4RPTR), u4Length);     
        UART_WRITE32((u4RPTR + u4Length), UART1_DMAW_RPTR);        
        return u4Length;
    }
    else
    {
        u4Length = MIN((u4EPTR - u4RPTR), u4NumToRead);
        memcpy((void *)pBuffer, phys_to_virt((const void *)u4RPTR), u4Length);
        UART_WRITE32((u4RPTR + u4Length), UART1_DMAW_RPTR);
        if ((u4RPTR + u4Length) == u4EPTR)
        {
            UART_WRITE32(u4SPTR, UART1_DMAW_RPTR);
        }
        
        if (u4Length == u4NumToRead)
        {
            return u4Length;
        }
        u4RxLength += u4Length;
        pBuffer += u4Length;
        u4NumToRead -= u4Length;

        u4Length = MIN((u4WPTR - u4SPTR), u4NumToRead);
        u4RxLength += u4Length;
        memcpy((void *)pBuffer, phys_to_virt((const void *)u4SPTR), u4Length);
        UART_WRITE32((u4SPTR + u4Length), UART1_DMAW_RPTR);
        return u4RxLength;        
    }
}

static unsigned int _mt5395_u1_ReadRxFifo(struct mt53xx_uart_port *mport, unsigned char *pBuffer, unsigned int u4NumToRead)
{
//    BUFFER_INFO *prRxFIFO;
    unsigned int u4DataAvail;
    unsigned int i;
    
//    ASSERT(pBuffer);

    //HalFlushInvalidateDCache();
    dma_map_single(NULL, mport->pu1DmaBuffer, UDMA_BUFFER_SIZE, DMA_BIDIRECTIONAL);
    u4DataAvail = _mt5395_u1_GetRxDMABuf(pBuffer, u4NumToRead);

    return u4DataAvail;
}
#endif

static unsigned int _mt53xx_u0_trans_mode_on(void)
{
    return UART_READ32(UART0_STATUS_OLD) & U0_TRANSPARENT;
}

static unsigned int _mt53xx_u2_trans_mode_on(void)
{
    return UART_READ32(UART2_STATUS_OLD) & U2_TRANSPARENT;
}

static void _mt53xx_uart_flush(int port)
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

static void _mt53xx_u0_set_trans_mode(unsigned int on)
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

static void _mt53xx_u2_set_trans_mode(unsigned int on)
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

#ifdef CONFIG_TTYMT7_SUPPORT
#ifdef CONFIG_PREEMPT_RT
#define SEMAPHORE compat_semaphore
#else
#define SEMAPHORE semaphore
#endif //CONFIG_PREEMPT_RT

typedef struct {
    struct SEMAPHORE   rSem;     // OS semaphore
    int              i4Count;  // 0: locked, 1: unlocked
    spinlock_t         spinlock; // counter maintenance lock
} SEM_T;

static int _sema_create(void   **ppv_sema_id)
{
    SEM_T *sem;

    if(in_interrupt())
    {
        panic("create semaphore in interrupt!");
    }

    sem = kmalloc(sizeof(SEM_T), GFP_KERNEL);
    if(!sem)
    {
        panic("fail to kmalloc semaphore!");
    }

    if (!sem)
    {
        return -1;
    }

    spin_lock_init(&sem->spinlock);
    
    init_MUTEX_LOCKED(&(sem->rSem));
    sem->i4Count = 0;

    *ppv_sema_id = (void*)sem;

    return 0;
}


#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void process_sema_timeout(unsigned long __data)
{
    struct task_struct *task = (struct task_struct *)__data;
    set_tsk_sema_timeout(task);
    wake_up_process(task);
}
#endif

static int _sema_lock_timeout(void     *pv_sema_id,
                           unsigned int   ui4_time)
{
    int i4Ret;
    SEM_T *sem;
    unsigned long flags;

    if(in_interrupt())
    {
        panic("lock semaphore in interrupt!");
    }

    sem = (SEM_T *)pv_sema_id;
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,27)

    i4Ret=down_timeout(&(sem->rSem),msecs_to_jiffies(ui4_time));
    if(i4Ret==-ETIME)
//    	return OSR_DRV_TIMEOUT;
        return 0;

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);
    
    return 0;
#else
    struct timer_list timer;
    unsigned long expire;

    expire = msecs_to_jiffies(ui4_time) + jiffies;
    setup_timer(&timer, process_sema_timeout, (unsigned long)current);
    __mod_timer(&timer, expire);

    while (1)
    {
        /* Got semaphore before timeout */
        if (!down_interruptible_timeout(&(sem->rSem)))
        {
            i4Ret = 0;
            break;
        }
        
        if (sema_timeout(current))
        {
//            i4Ret = OSR_DRV_TIMEOUT;
            i4Ret = 0;
            break;
        }
    }
    del_singleshot_timer_sync(&timer);
    clear_tsk_sema_timeout(current);

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);

    return i4Ret;
#endif
}

static int _sema_lock(void            *pv_sema_id)
{
    SEM_T *sem = (SEM_T *)pv_sema_id;
    unsigned long flags;

    if(in_interrupt())
    {
        panic("lock semaphore in interrupt!");
    }

    down(&(sem->rSem));

    spin_lock_irqsave(&sem->spinlock, flags);
    sem->i4Count = 0;
    spin_unlock_irqrestore(&sem->spinlock, flags);

    return 0;
}

static int _sema_unlock(void *pv_sema_id)
{
    SEM_T *sem = (SEM_T *)pv_sema_id;
    unsigned long flags;
    int to_wake_up = 0;

    // Lost wakeup:
    // Init as locked, ISR unlock, thread lock for waiting ISR => OK
    // Thread lock/unlock => OK
    // Thread lock/multiple ISR unlock => lost one unlock between thread got lock and not change i4Count to 0
    
    spin_lock_irqsave(&sem->spinlock, flags);
    if (likely(sem->i4Count == 0))
    {
        sem->i4Count = 1;
        to_wake_up = 1;
    }
    spin_unlock_irqrestore(&sem->spinlock, flags);

    if (to_wake_up)
    {
        up(sem);
    }

    return 0;
}

static int _UART_CheckMagicChar(int port, unsigned int data_byte)
{
    unsigned int i;
    unsigned int u4Num = _u4NumMagicInputForceFactory;
    if(u4Num == 0)
    {
        return 0;
    }
    // Check for magic char candidate 0
    if (((unsigned char)(_u4MagicInputForceFactory & 0xff) > UART_MAX_FACTORY_KEY || (unsigned char)(_u4MagicInputForceFactory & 0xff) < UART_MIN_FACTORY_KEY) &&
         ((unsigned char)data_byte == (unsigned char)(_u4MagicInputForceFactory & 0xff)) )             
    {
        // switch to factory mode.
        unsigned long flags;
        local_irq_save(flags);
        _enable_ttyMT7 = 1;       
        local_irq_restore(flags);
        _sema_unlock(uart_sem);
        return 1;
    }
    // Check for magic char candidate 1 ~ 4
    u4Num = (u4Num>5)?4:(u4Num-1);
    for(i = 0; i < u4Num; i++)
    {
        if (((unsigned char)(_au4OtherMagicInputForceFactory[i] & 0xff) > UART_MAX_FACTORY_KEY || (unsigned char)(_au4OtherMagicInputForceFactory[i] & 0xff) < UART_MIN_FACTORY_KEY) &&
         ((unsigned char)data_byte == (unsigned char)(_au4OtherMagicInputForceFactory[i] & 0xff)) )             
        {
            // switch to factory mode.
            unsigned long flags;
            local_irq_save(flags);
            _enable_ttyMT7 = 1;       
            local_irq_restore(flags);
            _sema_unlock(uart_sem);
            return 1;
        }
    }
    return 0;
}
#define DEFAULT_MAGIC_STR_LEN (6)
// Detect magic string "0.ft" receiving and switch UART0 to ttyMT3
static unsigned int _au4MagicStr[DEFAULT_MAGIC_STR_LEN] = {0x0d, 0x30,  0x2E,  0x66,  0x74, 0x0d};
static unsigned int _u4MagicStrMatchedPos = 0;
static unsigned int _u4MagicStrPrevMatch = 0;
static int _UART_CheckMagicString(int port, unsigned int data_byte)
{
    unsigned long flags;
    local_irq_save(flags);
    if(_u4MagicStrMatchedPos >= DEFAULT_MAGIC_STR_LEN)
    {
        local_irq_restore(flags);        
        return 0;
    }
    if((data_byte == _au4MagicStr[_u4MagicStrMatchedPos]) && (_u4MagicStrPrevMatch || (_u4MagicStrMatchedPos == 0)))    
    {
        _u4MagicStrPrevMatch = 1;    
        // String matched!
        if(_u4MagicStrMatchedPos == DEFAULT_MAGIC_STR_LEN - 1)       
        {
            _u4MagicStrMatchedPos = 0;        
            _enable_ttyMT7 = 1;            
            local_irq_restore(flags);              
            _sema_unlock(uart_sem);    
            return 1;
        }
        _u4MagicStrMatchedPos++;
    }
    else
    {
        _u4MagicStrPrevMatch = 0;
        _u4MagicStrMatchedPos = 0;        
    }
    local_irq_restore(flags);        
    return 0;
}
#endif

#ifdef ENABLE_UDMA
static void _mt5395_uart_dmainit(struct mt53xx_uart_port *mport)
{
    unsigned int z_size;
    unsigned int ui4_order;
    z_size = UDMA_BUFFER_SIZE;
    ui4_order = get_order(z_size);
    if(mport->pu1DmaBuffer == NULL)
    {
        mport->pu1DmaBuffer = (unsigned char *)__get_free_pages(GFP_KERNEL, ui4_order);    
    }
    if(mport->pu1DrvBuffer == NULL)
    {    
        mport->pu1DrvBuffer = (unsigned char *)__get_free_pages(GFP_KERNEL, ui4_order);     
    }
    if(mport->pu1DmaBuffer != NULL)
    {
        _mt5395_u1_HwDmaWriteDram(virt_to_phys(mport->pu1DmaBuffer), UDMA_BUFFER_SIZE, 1);
    }
}
#endif

static void _mt53xx_uart_hwinit(struct mt53xx_uart_port *mport, int port)
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

    // Clear interrupt status (should done before disable int??)
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
            _mt53xx_u0_set_trans_mode(0);
            UART_WRITE32(0, UART0_BUFFER_SIZE);
            _mt53xx_u0_set_trans_mode(1);
        }
    }

    if (port == 2)
    {
        if (UART_READ32(UART2_BUFFER_SIZE) != 0)
        {
            _mt53xx_u2_set_trans_mode(0);
            UART_WRITE32(0, UART2_BUFFER_SIZE);
            _mt53xx_u2_set_trans_mode(1);
        }

    }

    if (port == 0)
    {
        // Change to transparent mode if necessary
        if (!_mt53xx_u0_trans_mode_on())
        {
            _mt53xx_u0_set_trans_mode(1);
        }
    }

    if (port == 2)
    {
        if (!_mt53xx_u2_trans_mode_on())
        {
            _mt53xx_u2_set_trans_mode(1);
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
#ifndef ENABLE_UDMA
        UART_WRITE32(0, UART1_DMAW_CTRL);
#else
        // init DMA buffer
        _mt5395_uart_dmainit(mport);
#endif
        #if defined(ENABLE_DMA_CHK)
        // init dma check
        //UART_WRITE32(MODEM_HW_RTS | MODEM_HW_CTS | MODEM_RTS_TRIGGER_VALUE, UART1_MODEM);
        UART_WRITE32(0xfc, 0xd4); // todo.remove the magic number
        #endif
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
#if 0	//please set uart pinmux at related xx_pin_setup.h
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
#endif
    local_irq_restore(flags);
}

void _mt53xx_uart_set(unsigned int uCOMMCTRL, int baud, int datalen, int stop, int parity)
{
    if (uCOMMCTRL == UART2_COMMCTRL)
    {
        switch (baud)
        {
        case 115200:    	
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_115200), uCOMMCTRL);            
            break;
        case 57600:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_57600), uCOMMCTRL);
            break;
        case 38400:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_38400), uCOMMCTRL);
            break;
        case 19200:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_19200), uCOMMCTRL);
            break;
        case 9600:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_9600), uCOMMCTRL);
            break;
        case 2400:
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(U2_BAUD_2400), uCOMMCTRL);
            break;
        default:
            break;
        }
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
        case 3000000:
            UART_REG_BITCLR(CUSTBAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETCUSTBAUD(0x80), uCOMMCTRL);                    	
            UART_REG_BITCLR(BAUD_MASK, uCOMMCTRL);
            UART_REG_BITSET(SETBAUD(BAUD_CUST), uCOMMCTRL);                        
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

static void _mt53xx_uart_get(unsigned int uCOMMCTRL, int *p_baud, int *p_datalen, int *p_stop, int *p_parity)
{
    int baud, datalen, stop, parity;

    if (uCOMMCTRL == UART2_COMMCTRL)
    {
        switch (GETBAUD(UART_READ32(uCOMMCTRL)))
        {
        case U2_BAUD_X1:
            baud = 115200;
            break;
        case U2_BAUD_57600:
            baud = 57600;
            break;
        case U2_BAUD_38400:
            baud = 38400;
            break;
        case U2_BAUD_19200:
            baud = 19200;
            break;
        case U2_BAUD_9600:
            baud = 9600;
            break;
        case U2_BAUD_2400:
            baud = 2400;
            break;
        default:
            baud = 115200;
            break;
        }
    }
    else
    {
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
        case BAUD_CUST:
            baud = 3000000;
            break;        
        default:
            baud = 115200;
            break;
        }
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

    *p_baud = baud;
    *p_datalen = datalen;
    *p_stop = stop;
    *p_parity = parity;
}

/*
 * uart member functions
 */

static unsigned int _mt53xx_u0_read_allow(void)
{
    //return UART_READ32(UART0_STATUS_OLD) & U0_RD_ALLOW;
    if (_mt53xx_u0_trans_mode_on())
    {
        return GET_RX_DATA_SIZE(UART_READ32(UART0_STATUS));
    }
    else
    {
        //return UART_READ32(UART0_BUFFER_SIZE) + 1;
        return 1;
    }
}

static unsigned int _mt53xx_u0_write_allow(void)
{
    //return UART_READ32(UART0_STATUS_OLD) & U0_WR_ALLOW;
    if (_mt53xx_u0_trans_mode_on())
    {
        return GET_TX_EMPTY_SIZE(UART_READ32(UART0_STATUS));
    }
    else
    {
        //return 0;
        return 1;
    }
}

static void _mt53xx_u0_int_enable(int enable)
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

static void _mt53xx_u0_empty_int_enable(int enable)
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

static unsigned int _mt53xx_u0_read_byte(void)
{
    unsigned int i4Ret;

    i4Ret = UART_READ32(UART0_DATA_BYTE);

    return i4Ret;
}

static void _mt53xx_u0_write_byte(unsigned int byte)
{
    
    UART_WRITE32(byte, UART0_DATA_BYTE);
}

static void _mt53xx_u0_flush(void)
{
    _mt53xx_uart_flush(UART_PORT_0);
    _UART_HwWaitTxBufClear(UART_PORT_0);
}

static void _mt53xx_u0_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART0_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART0_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART0_STATUS) & ERR_BREAK)  ? 1 : 0;
}

static unsigned int _mt53xx_u1_read_allow(void)
{
    return GET_RX_DATA_SIZE(UART_READ32(UART1_STATUS));
}

static unsigned int _mt53xx_u1_write_allow(void)
{
    return GET_TX_EMPTY_SIZE(UART_READ32(UART1_STATUS));
}

static void _mt53xx_u1_int_enable(int enable)
{
    unsigned long flags;

    local_irq_save(flags);
#ifdef ENABLE_UDMA
    if (enable)
    {
        UART_REG_BITSET(U1_WBUF_FULL | U1_WBUF_TOUT | U1_WBUF_OVER, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U1_WBUF_FULL | U1_WBUF_TOUT | U1_WBUF_OVER, UART_INT_EN);
    }
#else
    if (enable)
    {
        UART_REG_BITSET(U1_INTALL, UART_INT_EN);
    }
    else
    {
        UART_REG_BITCLR(U1_INTALL, UART_INT_EN);
    }
#endif
    local_irq_restore(flags);
}

static void _mt53xx_u1_empty_int_enable(int enable)
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

static unsigned int _mt53xx_u1_read_byte(void)
{
    unsigned int i4Ret;
    i4Ret = UART_READ32(UART1_DATA_BYTE);

    return i4Ret;
}

static void _mt53xx_u1_write_byte(unsigned int byte)
{
    UART_WRITE32(byte, UART1_DATA_BYTE);
}

static void _mt53xx_u1_flush(void)
{
    _mt53xx_uart_flush(UART_PORT_1);
    _UART_HwWaitTxBufClear(UART_PORT_1);
}

static void _mt53xx_u1_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART1_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART1_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART1_STATUS) & ERR_BREAK)  ? 1 : 0;
}

static unsigned int _mt53xx_u2_read_allow(void)
{
    if (_mt53xx_u2_trans_mode_on())
    {
        return GET_RX_DATA_SIZE(UART_READ32(UART2_STATUS));
    }
    else
    {
        return 1;
    }
}

static unsigned int _mt53xx_u2_write_allow(void)
{
    if (_mt53xx_u2_trans_mode_on())
    {
        return GET_TX_EMPTY_SIZE(UART_READ32(UART2_STATUS));
    }
    else
    {
        return 1;
    }
}

static void _mt53xx_u2_int_enable(int enable)
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

static void _mt53xx_u2_empty_int_enable(int enable)
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

static unsigned int _mt53xx_u2_read_byte(void)
{
    unsigned int i4Ret;
    i4Ret = UART_READ32(UART2_DATA_BYTE);

    return i4Ret;
}

static void _mt53xx_u2_write_byte(unsigned int byte)
{
    UART_WRITE32(byte, UART2_DATA_BYTE);
}

static void _mt53xx_u2_flush(void)
{
    _mt53xx_uart_flush(UART_PORT_2);
    _UART_HwWaitTxBufClear(UART_PORT_2);
}

static void _mt53xx_u2_get_top_err(int *p_parity, int *p_end, int *p_break)
{
    *p_parity = (UART_READ32(UART2_STATUS) & ERR_PARITY) ? 1 : 0;
    *p_end    = (UART_READ32(UART2_STATUS) & ERR_END)    ? 1 : 0;
    *p_break  = (UART_READ32(UART2_STATUS) & ERR_BREAK)  ? 1 : 0;
}

/*
 * interrupt handling
 */
static void _mt53xx_uart_rx_chars(struct mt53xx_uart_port *mport)
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

    if (((mport->nport == 0) && !(_mt53xx_u0_trans_mode_on())) ||
        ((mport->nport == 2) && !(_mt53xx_u2_trans_mode_on())))
    {
        max_count = 1;
    }

    while (max_count > 0)
    {
        max_count--;

        /* check status */
        if (mport->fn_read_allow())
        {
            /* in mt53xx, process error before read byte */
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

/*
 * interrupt handling
 */
#ifdef CONFIG_TTYMT7_SUPPORT
static void _mt53xx_uart_rx_chars_2(struct mt53xx_uart_port *mport, 
    struct mt53xx_uart_port *mport_factory)
{
    struct tty_struct *tty, *tty_factory = NULL;
    int max_count;
    unsigned int data_byte;
    unsigned int flag;
    int err_parity, err_end, err_break;
    int ttyMT0_has_char = 0;
    int ttyMT7_has_char = 0;    
#if 0
    int err_overrun, err_status;
#endif    

    if (!mport || !(&mport->port) || !(mport->port.state) || !mport_factory)
    {
        return;
    }

    tty = mport->port.state->port.tty;
    if(mport_factory && (&mport_factory->port) && (mport_factory->port.state))
    {
        tty_factory = mport_factory->port.state->port.tty;
    }
    
    if (!tty)
    {
        return;
    }

    max_count = RX_BUF_SIZE;

    if (((mport->nport == 0) && !(_mt53xx_u0_trans_mode_on())) ||
        ((mport->nport == 2) && !(_mt53xx_u2_trans_mode_on())))
    {
        max_count = 1;
    }

    while (max_count > 0)
    {
        max_count--;

        /* check status */
        if(!_enable_ttyMT7)
        {
            ttyMT0_has_char = 1;
            if (mport->fn_read_allow())
            {
                /* in mt5395, process error before read byte */
                mport->fn_get_top_err(&err_parity, &err_end, &err_break);
    
                /* read the byte */
                data_byte = mport->fn_read_byte();
                mport->port.icount.rx++;            
                flag = TTY_NORMAL;

                if(_UART_CheckMagicChar(mport->nport, data_byte))
                {
                    ttyMT7_has_char = 1;  
                    mport_factory->port.icount.rx++;                               
                    uart_insert_char(&mport_factory->port, 0, 0, data_byte, flag);                    
                }
                else if(_UART_CheckMagicString(mport->nport, data_byte))
                {
                    // Do nothing
                }
                
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
        else
        {     
            if (mport_factory && mport_factory->fn_read_allow())
            {
                ttyMT7_has_char = 1;               
                /* in mt5395, process error before read byte */
                mport_factory->fn_get_top_err(&err_parity, &err_end, &err_break);
    
                /* read the byte */
                data_byte = mport_factory->fn_read_byte();
                mport_factory->port.icount.rx++;            
                flag = TTY_NORMAL;                    
                
                /* error handling routine */
                if (err_break)
                {
                    mport_factory->port.icount.brk++;
                    if (uart_handle_break(&mport_factory->port))
                    {
                        continue;
                    }
                    flag = TTY_BREAK;
                }
                else if (err_parity)
                {
                    mport_factory->port.icount.parity++;
                    flag = TTY_PARITY;
                }
                else if (err_end)
                {
                    mport_factory->port.icount.frame++;
                    flag = TTY_FRAME;
                }
            }
            else
            {
                break;
            }

#if 0
            if (uart_handle_sysrq_char(&mport_factory->port, data_byte))
            {
                continue;
            }
#endif

            //2.6.18. no overrun support, set status and mask to 0
            if(tty_factory)
            {
                uart_insert_char(&mport_factory->port, 0, 0, data_byte, flag);
            }
    
            //2.6.10
            //tty_insert_flip_char(tty, data_byte, flag);
        }
    }

    if(ttyMT0_has_char)
    {
        tty_flip_buffer_push(tty);
    }
    if(ttyMT7_has_char && tty_factory)
    {
        tty_flip_buffer_push(tty_factory);    
    }
    //tty_schedule_flip(tty);
}
#endif

static void _mt53xx_uart_tx_chars(struct mt53xx_uart_port *mport)
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
        _mt53xx_uart_stop_tx(port);
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
        _mt53xx_uart_stop_tx(port);
    }
}

// Fake
#ifdef CONFIG_TTYMT7_SUPPORT
static void _mt53xx_uart_tx_chars_2(struct mt53xx_uart_port *mport)
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
        port->icount.tx++;
        port->x_char = 0;
        return;
    }

    /* stop tx if circular buffer is empty or this port is stopped */
    if (uart_circ_empty(xmit) || uart_tx_stopped(port))
    {
        _mt53xx_uart_stop_tx(port);
        return;
    }

    count = TX_BUF_SIZE;

    while ((count > 0) &&
           !(uart_circ_empty(xmit)))
    {
        count--;
        xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
        port->icount.tx++;
    }
    
    if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
    {
        uart_write_wakeup(port);
    }

    if (uart_circ_empty(xmit))
    {
        _mt53xx_uart_stop_tx(port);
    }
}
#endif

static void _mt53xx_uart_modem_state(struct mt53xx_uart_port *mport)
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
static unsigned int _mt53xx_uart_tx_empty(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned int count;
    unsigned long flags;

    if (!port)
    {
        return 0;
    }

    mport = (struct mt53xx_uart_port *)port;

    spin_lock_irqsave(&port->lock, flags);

#ifdef CONFIG_TTYMT7_SUPPORT    
    if((_enable_ttyMT7 && mport->nport == 0) || (!_enable_ttyMT7 && mport->nport == 3))
    {
    	count = UART_FIFO_SIZE;           
    }
    else
#endif    
    {
        count = mport->fn_write_allow();
    }

    spin_unlock_irqrestore(&port->lock, flags);
    
    return ((count == UART_FIFO_SIZE) ? TIOCSER_TEMT : 0);
}

static void _mt53xx_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
    struct mt53xx_uart_port *mport;
    unsigned int regval;
    unsigned int uMS;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;
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

    #if !defined(ENABLE_DMA_CHK)
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
    #endif
}

static unsigned int _mt53xx_uart_get_mctrl(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned int modem_status;
    unsigned int result = 0;
    unsigned int uMS;

    if (!port)
    {
        return 0;
    }

    mport = (struct mt53xx_uart_port *)port;

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

static void _mt53xx_uart_stop_tx(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;
    
    //mport->fn_empty_int_enable(0);
    local_irq_save(flags);
    mport->tx_stop = 1;
    local_irq_restore(flags);
}

static void _mt53xx_uart_start_tx(struct uart_port *port)
{
    //struct circ_buf *xmit = &port->info->xmit;
    struct mt53xx_uart_port *mport;
    unsigned long flags;

    //mport->fn_empty_int_enable(1);

    if (!port)
    {
        return;
    }
    
    mport = (struct mt53xx_uart_port *)port;

    local_irq_save(flags);
#ifdef CONFIG_TTYMT7_SUPPORT    
    if((!_enable_ttyMT7 && mport->nport == 3) || (_enable_ttyMT7 && mport->nport == 0))
    {
        _mt53xx_uart_tx_chars_2(mport);    
        mport->tx_stop = 0;   
        local_irq_restore(flags);        
        return;
    }
#endif
    
    if (mport->fn_write_allow())
    {
        _mt53xx_uart_tx_chars(mport);
    }
    mport->tx_stop = 0;

    local_irq_restore(flags);
}


static void _mt53xx_uart_stop_rx(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned long flags;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;

    local_irq_save(flags);
    mport->rx_stop = 1;
    local_irq_restore(flags);
}

static void _mt53xx_uart_enable_ms(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;

#ifdef DISABLE_MODEM_CONTROL
    mport->ms_enable = 0;
#endif // DISABLE_MODEM_CONTROL
}

static void _mt53xx_uart_break_ctl(struct uart_port *port, int break_state)
{
    struct mt53xx_uart_port *mport;
    unsigned int regval;
    unsigned long flags;
    unsigned int uCOMMCTRL;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;
    
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
static irqreturn_t _mt53xx_uart0_interrupt(int irq, void *dev_id);
static irqreturn_t _mt53xx_uart1_interrupt(int irq, void *dev_id);
static irqreturn_t _mt53xx_uart2_interrupt(int irq, void *dev_id);

static int _mt53xx_uart_startup(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned long flags;
    int retval;
    int u0irq, u1irq, u2irq;
    
    if (!port)
    {
        return -1;
    }

    mport = (struct mt53xx_uart_port *)port;
    // ignore if ttyMT3
#ifdef CONFIG_TTYMT7_SUPPORT    
    if(mport->nport == 3)
    {
        return 0;
    }
#endif
  
    /* FIXME: hw init? */
    _mt53xx_uart_hwinit(mport, mport->nport);

    mport->hw_init = 1;
    
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
        retval = request_irq(port->irq, _mt53xx_uart0_interrupt, IRQF_SHARED,
                             "MT53xx Serial 0", port);
        if (retval)
        {
            _u0_irq_allocated --;
            return retval;
        }
    }

    if ((mport->nport == 1) && (u1irq == 1))
    {
#ifndef U0_U1_SHARE_IRQ_HANDLER  
        /* allocate irq, u0/u1 share the same interrupt number */
        retval = request_irq(port->irq, _mt53xx_uart1_interrupt, IRQF_SHARED,
                             "MT53xx Serial 1", port);
#endif        
        if (retval)
        {
            _u1_irq_allocated --;
            return retval;
        }
    }

    if ((mport->nport == 2) && (u2irq == 1))
    {
        /* allocate irq, u0/u1 share the same interrupt number */
        retval = request_irq(port->irq, _mt53xx_uart2_interrupt, IRQF_SHARED,
                             "MT53xx Serial 2", port);
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

static void _mt53xx_uart_shutdown(struct uart_port *port)
{
    struct mt53xx_uart_port *mport;
    unsigned long flags;
    int u0irq, u1irq, u2irq;
        
    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;

    // ignore if ttyMT3
#ifdef CONFIG_TTYMT7_SUPPORT    
    if(mport->nport == 3)
    {
        return 0;
    }
#endif

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
    else if(mport->nport == 2)
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
#ifndef U0_U1_SHARE_IRQ_HANDLER        
        free_irq(port->irq, port);
#endif
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
    _mt53xx_uart_hwinit(mport, mport->nport);

    mport->hw_init = 0;
    
}

static unsigned char _mt53xx_uart_cal_rxd_timeout(unsigned int baud)
{
    unsigned int wait_time;

    wait_time = 1000000 / (baud / 8);

    if (wait_time < (64 * 128 / 30))
    {
        if(wait_time * 30 / 128 == 0)
        {
            return 0x1;
        }
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

static void _mt53xx_uart_set_termios(struct uart_port *port,
    struct ktermios *termios, struct ktermios *old)
{
    struct mt53xx_uart_port *mport;
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

    mport = (struct mt53xx_uart_port *)port;
            
    switch (mport->nport)
    {
    case 0:
#ifdef CONFIG_TTYMT7_SUPPORT    	
    case 3:
#endif    	
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

    //_mt53xx_uart_flush(mport->nport);
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
    
#ifdef CONFIG_TTYMT7_SUPPORT    	
    // Skip applying UART setting because control right belongs to the other
    if((mport->nport == 0 && _enable_ttyMT7) ||
    	(mport->nport == 3 && !_enable_ttyMT7))
    {
        /* unlock here */
        spin_unlock_irqrestore(&port->lock, flags);
        return;        
    }
#endif   

    _mt53xx_uart_set(uCOMMCTRL, baud, datalen, stopbit, parity);
    
    /* change rxd timeout period */
    switch (mport->nport)
    {
    case 0:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART0_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt53xx_uart_cal_rxd_timeout(baud)) , UART0_BUFCTRL);
        break;
    case 1:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART1_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt53xx_uart_cal_rxd_timeout(baud)) , UART1_BUFCTRL);
        break;
    case 2:
        UART_REG_BITCLR(RX_TOUT_CYCLE(0xFF) , UART2_BUFCTRL);
        UART_REG_BITSET(RX_TOUT_CYCLE(_mt53xx_uart_cal_rxd_timeout(baud)) , UART2_BUFCTRL);
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

static const char *_mt53xx_uart_type(struct uart_port *port)
{
    return "MT53xx Serial";
}

static void _mt53xx_uart_release_port(struct uart_port *port)
{
    release_mem_region(port->mapbase, MT53xx_UART_SIZE);
}

static int _mt53xx_uart_request_port(struct uart_port *port)
{
    void *pv_region;

    pv_region = request_mem_region(port->mapbase, MT53xx_UART_SIZE,
                                   "MT53xx Uart IO Mem");
    return pv_region != NULL ? 0 : -EBUSY;
}

static void _mt53xx_uart_config_port(struct uart_port *port, int flags)
{
    if (flags & UART_CONFIG_TYPE)
    {
        port->type = PORT_MT53xx;
        _mt53xx_uart_request_port(port);
    }
}

static int _mt53xx_uart_verify_port(struct uart_port *port,
                                    struct serial_struct *ser)
{
	int ret = 0;
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_MT53xx)
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

#ifdef CONFIG_TTYMT7_SUPPORT
static int _mt53xx_uart_ioctl(struct uart_port *port, unsigned int cmd,
                  unsigned long arg)
{
    unsigned long flags;
    int ret = 0;  
    if(cmd == 0xffff)
    {
        if(arg == 0)
        {
            /* lock from here */
            spin_lock_irqsave(&port->lock, flags);          
            _enable_ttyMT7 = 0;
            /* unlock here */
            spin_unlock_irqrestore(&port->lock, flags);    
        }
        else if(arg == 7)
        {            
            // switch to factory mode.
            /* lock from here */
            spin_lock_irqsave(&port->lock, flags);                      
            _enable_ttyMT7 = 1;       
            /* unlock here */
            spin_unlock_irqrestore(&port->lock, flags);          
            _sema_unlock(uart_sem);                              
        }
        else
        {
            ret = -EINVAL;
        }
    }        
    else if(cmd == 0xfffe)
    {
        /* lock from here */
        spin_lock_irqsave(&port->lock, flags);           
        _u4MagicInputForceFactory =  (unsigned int)arg;
        _u4NumMagicInputForceFactory = 1;        
        /* unlock here */
        spin_unlock_irqrestore(&port->lock, flags);   
    }
    else if(cmd == 0xfffc)
    {
        unsigned int  i;
        unsigned int au4Magic[6]; // num of magic + 5 magic
        if(copy_from_user(au4Magic, (void*)arg, (sizeof(unsigned int) * 6)))
        {
            printk(KERN_CRIT "_mt5365_uart_ioctl - copy_from_user() fail\n");
            ret = -EFAULT;            
        }    
        // num of magic == 0
        if(au4Magic[0] == 0)
        {
            return 0;
        }
        /* lock from here */
        spin_lock_irqsave(&port->lock, flags);           
        _u4MagicInputForceFactory =  (unsigned int)au4Magic[1];
        // Storing magic char 1~4 to array[0] ~ array[3]
        for(i = 1; i < (au4Magic[0]>5?5:au4Magic[0]); i++)
        {
            _au4OtherMagicInputForceFactory[i-1] = au4Magic[i+1];
        }
        _u4NumMagicInputForceFactory = au4Magic[0]>5?5:au4Magic[0];        
        /* unlock here */
        spin_unlock_irqrestore(&port->lock, flags);   
    }        
#if 0    
    else if(cmd == 0xfffd)
    {           
        unsigned int timeout = 500;
        if(copy_from_user(&timeout, (void*)arg, sizeof(int)))
        {
            printk(KERN_CRIT "_mt5365_uart_ioctl - copy_from_user() fail\n");
            ret = -EFAULT;            
        }    
        _sema_lock_timeout(uart_sem, ((timeout >500)?500:timeout));
        if(copy_to_user((void*)arg, &_enable_ttyMT7, sizeof(int)))
        {
            printk(KERN_CRIT "_mt5365_uart_ioctl - copy_to_user() fail\n");
            ret = -EFAULT;            
        }
    }  
#endif    
    else
    {
        ret = -ENOIOCTLCMD;
    } 
    return ret;
}
#endif

static struct uart_ops _mt53xx_uart_ops =
{
    .tx_empty       = _mt53xx_uart_tx_empty,
    .set_mctrl      = _mt53xx_uart_set_mctrl,
    .get_mctrl      = _mt53xx_uart_get_mctrl,
    .stop_tx        = _mt53xx_uart_stop_tx,
    .start_tx       = _mt53xx_uart_start_tx,
    /* .send_xchar */
    .stop_rx        = _mt53xx_uart_stop_rx,
    .enable_ms      = _mt53xx_uart_enable_ms,
    .break_ctl      = _mt53xx_uart_break_ctl,
    .startup        = _mt53xx_uart_startup,
    .shutdown       = _mt53xx_uart_shutdown,
    .set_termios    = _mt53xx_uart_set_termios,
    /* .pm */
    /* .set_wake */
    .type           = _mt53xx_uart_type,
    .release_port   = _mt53xx_uart_release_port,
    .request_port   = _mt53xx_uart_request_port,
    .config_port    = _mt53xx_uart_config_port,
    .verify_port    = _mt53xx_uart_verify_port,
#ifdef CONFIG_TTYMT7_SUPPORT
    .ioctl = _mt53xx_uart_ioctl,
#endif
};

static struct mt53xx_uart_port _mt53xx_uart_ports[] =
{
    {
        .port =
        {
            .membase        = (void*)MT53xx_VA_UART,
            .mapbase        = MT53xx_PA_UART,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_0,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt53xx_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 0,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 0,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt53xx_u0_read_allow,
        .fn_write_allow     = _mt53xx_u0_write_allow,
        .fn_int_enable      = _mt53xx_u0_int_enable,
        .fn_empty_int_enable    = _mt53xx_u0_empty_int_enable,
        .fn_read_byte       = _mt53xx_u0_read_byte,
        .fn_write_byte      = _mt53xx_u0_write_byte,
        .fn_flush           = _mt53xx_u0_flush,
        .fn_get_top_err     = _mt53xx_u0_get_top_err,
    },
    {
        .port =
        {
            .membase        = (void*)MT53xx_VA_UART,
            .mapbase        = MT53xx_PA_UART,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_1,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt53xx_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 1,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 1,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt53xx_u1_read_allow,
        .fn_write_allow     = _mt53xx_u1_write_allow,
        .fn_int_enable      = _mt53xx_u1_int_enable,
        .fn_empty_int_enable    = _mt53xx_u1_empty_int_enable,
        .fn_read_byte       = _mt53xx_u1_read_byte,
        .fn_write_byte      = _mt53xx_u1_write_byte,
        .fn_flush           = _mt53xx_u1_flush,
        .fn_get_top_err     = _mt53xx_u1_get_top_err,
    },
    {
        .port =
        {
            .membase        = (void*)MT53xx_VA_UART2,
            .mapbase        = MT53xx_PA_UART2,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_2,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt53xx_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 2,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 2,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt53xx_u2_read_allow,
        .fn_write_allow     = _mt53xx_u2_write_allow,
        .fn_int_enable      = _mt53xx_u2_int_enable,
        .fn_empty_int_enable= _mt53xx_u2_empty_int_enable,
        .fn_read_byte       = _mt53xx_u2_read_byte,
        .fn_write_byte      = _mt53xx_u2_write_byte,
        .fn_flush           = _mt53xx_u2_flush,
        .fn_get_top_err     = _mt53xx_u2_get_top_err,
    },
#ifdef CONFIG_TTYMT7_SUPPORT
    {
        .port =
        {
            .membase        = (void*)MT53xx_VA_UART,
            .mapbase        = MT53xx_PA_UART,
            .iotype         = SERIAL_IO_MEM,
            .irq            = VECTOR_RS232_0,
            .uartclk        = 921600,
            .fifosize       = TX_BUF_SIZE,
            .ops            = &_mt53xx_uart_ops,
            .flags          = ASYNC_BOOT_AUTOCONF,
            .line           = 3,
            .lock           = SPIN_LOCK_UNLOCKED,
        },
        .nport              = 3,
        .old_status         = 0,
        .tx_stop            = 0,
        .rx_stop            = 0,
        .ms_enable          = 0,
        .fn_read_allow      = _mt53xx_u0_read_allow,
        .fn_write_allow     = _mt53xx_u0_write_allow,
        .fn_int_enable      = _mt53xx_u0_int_enable,
        .fn_empty_int_enable= _mt53xx_u0_empty_int_enable,
        .fn_read_byte       = _mt53xx_u0_read_byte,
        .fn_write_byte      = _mt53xx_u0_write_byte,
        .fn_flush           = _mt53xx_u0_flush,
        .fn_get_top_err     = _mt53xx_u0_get_top_err,
    },  
#endif     
};

static irqreturn_t _mt53xx_uart0_interrupt(int irq, void *dev_id)
{
    unsigned int uart_int_ident;
    struct mt53xx_uart_port *mport = (struct mt53xx_uart_port *)dev_id;
#ifdef CONFIG_TTYMT7_SUPPORT
    struct mt53xx_uart_port *mport_factory;
#endif
#ifdef U0_U1_SHARE_IRQ_HANDLER
    // Special
    struct mt53xx_uart_port *mport1 = &_mt53xx_uart_ports[1];
#endif
    irqreturn_t ret = IRQ_NONE;

#ifdef CONFIG_TTYMT7_SUPPORT
    mport = &_mt53xx_uart_ports[0];
    mport_factory = &_mt53xx_uart_ports[3];   
#endif    
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
    if (mport != &_mt53xx_uart_ports[0])
    {
        while (1)
            ;
    }
    //_mt53xx_handle_uart_interrupt(mport, regs, uart_int_ident & U0_TBUF);

    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U0_RBUF | U0_RERR)))
    {
#ifdef CONFIG_TTYMT7_SUPPORT    
        _mt53xx_uart_rx_chars_2(mport, mport_factory);
#else
        _mt53xx_uart_rx_chars(mport);
#endif
        ret = IRQ_HANDLED;
    }

    /* tx mode */
#ifdef CONFIG_TTYMT7_SUPPORT      
    if(!_enable_ttyMT7)
    {
        if (mport->fn_write_allow() || (uart_int_ident & U0_TBUF))
        {
            _mt53xx_uart_tx_chars(mport);
            ret = IRQ_HANDLED;
        }
    }
    else
    {
        if (mport_factory->fn_write_allow() || (uart_int_ident & U0_TBUF))
        {
            _mt53xx_uart_tx_chars(mport_factory);
            ret = IRQ_HANDLED;
        }
   
    }
#else
    if (mport->fn_write_allow() || (uart_int_ident & U0_TBUF))
    {
        _mt53xx_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }
#endif

    /* modem mode */
    _mt53xx_uart_modem_state(mport);

#ifdef U0_U1_SHARE_IRQ_HANDLER
    // Special    
    uart_int_ident = UART_READ32(UART_INT_ID);
    uart_int_ident &= U1_INTMSK;
    /* ack u1 interrupt */
    UART_WRITE32(~uart_int_ident, UART_INT_ID);    

    if(mport1 == NULL || mport1->port.state == NULL || mport1->port.state->port.tty == NULL)
    {
        return ret;
    }
    
    if (uart_int_ident & (U1_WBUF_TOUT | U1_WBUF_FULL | U1_WBUF_OVER))
    {
        unsigned int u4RxCnt;
        u4RxCnt = _mt5395_u1_HwGetRxDataCnt();
        if (u4RxCnt > 0)
        {
            struct tty_struct *tty;        
            unsigned int i;
            tty = mport1->port.state->port.tty;
            u4RxCnt = _mt5395_u1_ReadRxFifo(mport1, mport1->pu1DrvBuffer, u4RxCnt);
            /* read the byte */
            for(i = 0; i < u4RxCnt; i++)
            {
                uart_insert_char(&mport1->port, 0, 0, *(mport1->pu1DrvBuffer + i), TTY_NORMAL);            
                mport1->port.icount.rx++;
            }        
            tty_flip_buffer_push(tty);            
        }     
        ret = IRQ_HANDLED;        
    }
    /* tx mode */
    if (mport1->fn_write_allow() || (uart_int_ident & U1_TBUF))
    {
        _mt53xx_uart_tx_chars(mport1);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt53xx_uart_modem_state(mport1);
#endif
    
    return ret;
}

static irqreturn_t _mt53xx_uart1_interrupt(int irq, void *dev_id)
{
#ifndef U0_U1_SHARE_IRQ_HANDLER
    unsigned int uart_int_ident;
    struct mt53xx_uart_port *mport = (struct mt53xx_uart_port *)dev_id;
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
    if (mport != &_mt53xx_uart_ports[1])
    {
        while (1)
            ;
    }
    //_mt53xx_handle_uart_interrupt(mport, regs, uart_int_ident & U1_TBUF);

    // dmx rx mode
#ifdef ENABLE_UDMA
    if (uart_int_ident & (U1_WBUF_TOUT | U1_WBUF_FULL | U1_WBUF_OVER))
    {
        unsigned int u4RxCnt;
        u4RxCnt = _mt5395_u1_HwGetRxDataCnt();
        if (u4RxCnt > 0)
        {
            struct tty_struct *tty;        
            unsigned int i;
            tty = mport->port.state->port.tty;
            u4RxCnt = _mt5395_u1_ReadRxFifo(mport, mport->pu1DrvBuffer, u4RxCnt);
            /* read the byte */
            for(i = 0; i < u4RxCnt; i++)
            {
                uart_insert_char(&mport->port, 0, 0, *(mport->pu1DrvBuffer + i), TTY_NORMAL);            
                mport->port.icount.rx++;
            }        
            tty_flip_buffer_push(tty);            
        }     
        ret = IRQ_HANDLED;        
    }
#else
    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U1_RBUF | U1_RERR)))
    {
        _mt53xx_uart_rx_chars(mport);
        ret = IRQ_HANDLED;
    }
#endif

    /* tx mode */
    if (mport->fn_write_allow() || (uart_int_ident & U1_TBUF))
    {
        _mt53xx_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt53xx_uart_modem_state(mport);
    
    return ret;
#else
    return IRQ_HANDLED;   
#endif
}

static irqreturn_t _mt53xx_uart2_interrupt(int irq, void *dev_id)
{
    unsigned int uart_int_ident;
    struct mt53xx_uart_port *mport = (struct mt53xx_uart_port *)dev_id;
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
    if (mport != &_mt53xx_uart_ports[2])
    {
        while (1)
            ;
    }
    //_mt53xx_handle_uart_interrupt(mport, regs, uart_int_ident & U2_TBUF);

    /* rx mode */
    if (mport->fn_read_allow() || (uart_int_ident & (U2_RBUF | U2_RERR)))
    {
        _mt53xx_uart_rx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* tx mode */
    if (mport->fn_write_allow() || (uart_int_ident & U2_TBUF))
    {
        _mt53xx_uart_tx_chars(mport);
        ret = IRQ_HANDLED;
    }

    /* modem mode */
    _mt53xx_uart_modem_state(mport);

    return ret;
}

/*
 * console
 */

#ifdef CONFIG_SERIAL_MT53XX_CONSOLE

static void _mt53xx_uart_console_write(struct console *co, const char *s,
    unsigned int count)
{
//    static int hw_init=0;
    int i;
    struct mt53xx_uart_port *mport;

#ifdef CONFIG_TTYMT7_SUPPORT
    if(_enable_ttyMT7)
    {
#if 0    
        mport = &_mt5395_uart_ports[co->index];
        for (i = 0; i < count; i++)
        {
            mport->fn_write_byte(s[i]);    
            if (s[i] == '\n')
            {
                mport->fn_write_byte('\r');
            }
        }    
#endif        
        return;
    }
#endif
    if (((co->index == 0) && (!_mt53xx_u0_trans_mode_on())) ||
        ((co->index == 2) && (!_mt53xx_u2_trans_mode_on())))
    {
        return;
    }
    
    if (co->index >= UART_NR)
    {
        return;
    }

/*    
    if (!hw_init)
    {
        hw_init = 1;
        _mt53xx_uart_hwinit(co->index);
    }
*/

    mport = &_mt53xx_uart_ports[co->index];
    if (!mport->hw_init)
    {
        unsigned long flags;      
        mport->hw_init = 1;
        local_irq_save(flags);         
        _mt53xx_uart_hwinit(mport, co->index);
        if (co->index == 0)
        {
            /* enable uart 0 interrupt */
            UART_WRITE32(U0_INTALL | U0_TBUF, UART_INT_EN);
        }    
        if (co->index == 1)
        {
            /* enable uart 1 interrupt */        
            UART_WRITE32(U1_INTALL | U1_TBUF, UART_INT_EN);
        }    
        if (co->index == 2)
        {
            /* enable uart2 interrupt */
            UART_WRITE32(U2_INTALL | U2_TBUF, UART2_INT_EN);
        }            
        local_irq_restore(flags);        
    }

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

static void __init _mt53xx_uart_console_get_options(struct uart_port *port,
    int *pbaud, int *pparity, int *pbits)
{
    int baud=0, parity=0, stopbit, datalen;
    struct mt53xx_uart_port *mport;
    unsigned int uCOMMCTRL;

    if (!port)
    {
        return;
    }

    mport = (struct mt53xx_uart_port *)port;
    
    switch (mport->nport)
    {
    case 0:
#ifdef CONFIG_TTYMT7_SUPPORT
//    case 3: // virtual UART 
#endif 
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

    _mt53xx_uart_get(uCOMMCTRL, &baud, &datalen, &stopbit, &parity);

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

static int __init _mt53xx_uart_console_setup(struct console *co, char *options)
{
    struct uart_port *port;
    int baud    = 115200;
    int bits    = 8;
    int parity  = 'n';
    int flow    = 'n';
    int stopbit;
    int ret;

    printk(KERN_DEBUG "53xx console setup : co->index %d options:%s\n",
        co->index, options);

    if (co->index >= UART_NR)
    {
        co->index = 0;
    }
    
    port = (struct uart_port *)&_mt53xx_uart_ports[co->index];

    if (options)
    {
        uart_parse_options(options, &baud, &parity, &bits, &flow);
    }
    else
    {
        _mt53xx_uart_console_get_options(port, &baud, &parity, &stopbit);
    }

    ret = uart_set_options(port, co, baud, parity, bits, flow);
    printk(KERN_DEBUG "53xx console setup : uart_set_option port(%d) "
        "baud(%d) parity(%c) bits(%d) flow(%c) - ret(%d)\n",
        co->index, baud, parity, bits, flow, ret);
    return ret;
}


static struct uart_driver _mt53xx_uart_reg;
static struct console _mt53xx_uart_console =
{
    .name       = "ttyMT",
    .write      = _mt53xx_uart_console_write,
    .device     = uart_console_device,
    .setup      = _mt53xx_uart_console_setup,
    .flags      = CON_PRINTBUFFER,
    .index      = -1,
    .data       = &_mt53xx_uart_reg,
};

static int __init _mt53xx_uart_console_init(void)
{
    // set to transparent if boot loader doesn't do this
    //_mt53xx_u0_set_trans_mode_on();
    register_console(&_mt53xx_uart_console);
    return 0;
}

console_initcall(_mt53xx_uart_console_init);

static int __init _mt53xx_late_console_init(void)
{
    if (!(_mt53xx_uart_console.flags & CON_ENABLED))
    {
        register_console(&_mt53xx_uart_console);
    }
    return 0;
}

late_initcall(_mt53xx_late_console_init);

#define MT53XX_CONSOLE &_mt53xx_uart_console
#else

#define MT53XX_CONSOLE NULL

#endif /* CONFIG_SERIAL_MT53XX_CONSOLE */


static struct uart_driver _mt53xx_uart_reg =
{
    .owner          = THIS_MODULE,
    .driver_name    = "MT53xx serial",
    .dev_name       = "ttyMT",
    .major          = SERIAL_MT53xx_MAJOR,
    .minor          = SERIAL_MT53xx_MINOR,
    .nr             = UART_NR,
    .cons           = MT53XX_CONSOLE,
};

/*
* probe? remove? suspend? resume? ids?
*/

#if 0 // LH
static int _mt53xx_uart_probe(struct device *_dev)
{
    struct platform_device *dev = to_platform_device(_dev);

    if (!dev)
    {
        return 0;
    }

    _mt53xx_uart_ports[dev->id].port.dev = _dev;
    uart_add_one_port(&_mt53xx_uart_reg, &_mt53xx_uart_ports[dev->id].port);
    dev_set_drvdata(_dev, &_mt53xx_uart_ports[dev->id]);
    return 0;
}
#else
static int __devinit _mt53xx_uart_probe(struct platform_device *dev)
{
       _mt53xx_uart_hwinit(&_mt53xx_uart_ports[dev->id], dev->id);  /* all port 115200, no int */
        printk(KERN_DEBUG "Serial : add uart port %d\n", dev->id);
        uart_add_one_port(&_mt53xx_uart_reg, &_mt53xx_uart_ports[dev->id].port);
        
    platform_set_drvdata(dev, &_mt53xx_uart_ports[dev->id]);
    return 0;
}
#endif

#if 0 // LH
static int _mt53xx_uart_remove(struct device *_dev)
{
    struct mt53xx_uart_port *mport = dev_get_drvdata(_dev);

    dev_set_drvdata(_dev, NULL);

    if (mport)
    {
        uart_remove_one_port(&_mt53xx_uart_reg, &mport->port);
    }
    return 0;
}
#else
static int __devexit _mt53xx_uart_remove(struct platform_device *pdev)
{
        struct mt53xx_uart_port *mport = platform_get_drvdata(pdev);

        platform_set_drvdata(pdev, NULL);

        if (mport)
                uart_remove_one_port(&_mt53xx_uart_reg, &mport->port);

        return 0;       
}
#endif

#if 0 // LH
static int _mt53xx_uart_suspend(struct device * _dev, pm_message_t state)
{
#if 0
    struct mt53xx_uart_port *mport = dev_get_drvdata(_dev);

    if (mport && level == SUSPEND_DISABLE)
    {
        uart_suspend_port(&_mt53xx_uart_reg, &mport->port);
    }
#endif
    return 0;
}
#else
static int _mt53xx_uart_suspend(struct platform_device * _dev, pm_message_t state)
{
#if 0
    struct mt5391_uart_port *mport = platform_get_drvdata(_dev);

    if (mport)
    {
        uart_suspend_port(&_mt5391_uart_reg, &mport->port);
    }
#endif
    return 0;
}
#endif

#if 0 // LH
static int _mt53xx_uart_resume(struct device *_dev)
{
#if 0
    struct mt53xx_uart_port *mport = dev_get_drvdata(_dev);

    if (mport && (level == RESUME_ENABLE))
    {
        uart_resume_port(&_mt53xx_uart_reg, &mport->port);
    }
#endif
#else
static int _mt53xx_uart_resume(struct platform_device *dev)
{
    ((struct mt53xx_uart_port*)dev->dev.platform_data)->fn_empty_int_enable(1);
    ((struct mt53xx_uart_port*)dev->dev.platform_data)->fn_int_enable(1);
    if(dev->id==0)
    {
            _mt53xx_u0_int_enable(_u0_irq_allocated);
    }
    else if (dev->id==1)
    {
            _mt53xx_u1_int_enable(_u1_irq_allocated);
    }       
    else if (dev->id==2)
    {
            _mt53xx_u2_int_enable(_u2_irq_allocated);
    }
        
    if (_u0_irq_allocated||_u1_irq_allocated||_u2_irq_allocated)
    {
        __raw_writel(__raw_readl(BIM_VIRT + REG_RW_IRQEN) | (1 << VECTOR_RS232_0),
                     BIM_VIRT + REG_RW_IRQEN);
    }   
             
    return 0;
}
#endif

#if 0 // LH
static struct device_driver _mt53xx_uart_driver =
#else
static struct platform_driver _mt53xx_uart_driver =
#endif
{
    #if 0 // LH
    .name           = "MT53xx uart",
    .bus            = &platform_bus_type,
    #endif
    .probe          = _mt53xx_uart_probe,
    .remove         = _mt53xx_uart_remove,
    .suspend        = _mt53xx_uart_suspend,
    .resume         = _mt53xx_uart_resume,
    #if 1 // LH
    .driver         = {
        .name   = "MT53xx-uart",
        .owner  = THIS_MODULE,
    },
    #endif
};

#if 1 // LH
static struct platform_device _mt53xx_uart0_device = {
        .name                   = "MT53xx-uart",
        .id                     = 0,
        .dev                    = {
                .platform_data  = &_mt53xx_uart_ports[0],
        }
};

static struct platform_device _mt53xx_uart1_device = {
        .name                   = "MT53xx-uart",
        .id                     = 1,
        .dev                    = {
                .platform_data  = &_mt53xx_uart_ports[1],
        }
};
static struct platform_device _mt53xx_uart2_device = {
        .name                   = "MT53xx-uart",
        .id                     = 2,
        .dev                    = {
                .platform_data  = &_mt53xx_uart_ports[2],
        }
};
static struct platform_device _mt53xx_uart3_device = {
        .name                   = "MT53xx-uart",
        .id                     = 3,
        .dev                    = {
                .platform_data  = &_mt53xx_uart_ports[3],
        }
};
#endif


/*
* init, exit and module
*/

static int __init _mt53xx_uart_init(void)
{
    int ret;
    int i;

#ifdef CONFIG_TTYMT7_SUPPORT
    _sema_create(&uart_sem);
#endif

    /* reset hardware */
    for (i = 0; i < UART_NR; i++)
    {
#ifdef ENABLE_UDMA    
        _mt53xx_uart_ports[i].pu1DmaBuffer = NULL;
        _mt53xx_uart_ports[i].pu1DrvBuffer = NULL;        
#endif        
        _mt53xx_uart_ports[i].hw_init = 0;        
        _mt53xx_uart_hwinit(&_mt53xx_uart_ports[i], i);  /* all port 115200, no int */
    }
    
    printk(KERN_INFO "Serial: MT53xx driver $Revision: #1 $\n");
    ret = uart_register_driver(&_mt53xx_uart_reg);
    printk(KERN_DEBUG "Serial: uart_register_driver %d\n", ret);
    if (ret)
    {
        goto out;
    }

    #if 1 // LH
    platform_device_register(&_mt53xx_uart0_device);
    platform_device_register(&_mt53xx_uart1_device);
    platform_device_register(&_mt53xx_uart2_device);
    platform_device_register(&_mt53xx_uart3_device);
    #endif

    #if 0
    for (i = 0; i < UART_NR; i++)
    {
        printk(KERN_DEBUG "Serial : add uart port %d\n", i);
        uart_add_one_port(&_mt53xx_uart_reg, &_mt53xx_uart_ports[i].port);
    }

    ret = driver_register(&_mt53xx_uart_driver);
    printk(KERN_DEBUG "Serial: driver_register %d\n", ret);
    #else // LH
    ret = platform_driver_register(&_mt53xx_uart_driver);
    printk(KERN_DEBUG "Serial:  platform_driver_register %d\n", ret);
    #endif
    if (ret == 0)
    {
        goto out;
    }

//unreg:
//    uart_unregister_driver(&_mt53xx_uart_reg);

out:
    return ret;
}

static void __exit _mt53xx_uart_exit(void)
{
    uart_unregister_driver(&_mt53xx_uart_reg);
}

#ifdef CONFIG_TTYMT7_SUPPORT
int mt53_uart_wait_magic_char(unsigned long arg)
{
        unsigned int timeout = 500;
        int ret = 0;
        if(copy_from_user(&timeout, (void*)arg, sizeof(int)))
        {
            printk(KERN_CRIT "_mt53xx_uart_ioctl - copy_from_user() fail\n");
            ret = -EFAULT;            
        }    
//        _sema_lock_timeout(uart_sem, ((timeout >500)?500:timeout));
        _sema_lock(uart_sem);
        if(copy_to_user((void*)arg, &_enable_ttyMT7, sizeof(int)))
        {
            printk(KERN_CRIT "_mt53xx_uart_ioctl - copy_to_user() fail\n");
            ret = -EFAULT;            
        }
        return ret;
}
#endif

module_init(_mt53xx_uart_init);
module_exit(_mt53xx_uart_exit);

MODULE_AUTHOR("MediaTek Inc.");
MODULE_DESCRIPTION("MT53xx serial port driver $Revision: #1 $");
MODULE_LICENSE("MKL");
