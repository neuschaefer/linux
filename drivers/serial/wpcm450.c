/*
 * linux/drivers/serial/wpcm450.c
 *
 * Driver for onboard UARTs on the WinbondWPCM450
 *
 *
 * (c) 2006 Winbond Electronics
 *
 * Changelog:
 *
 * 18-Aug-2006  CLYU  Created file
*/

#include <linux/autoconf.h>

#if defined(CONFIG_SERIAL_WPCM450_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/sysrq.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/sched.h> 

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#include <asm/io.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/hardware.h>

#include <asm/arch/regs-serial.h>
#include <asm/arch/regs-gpio.h>

#include <asm/arch/regs-gcr.h>
#include <asm/arch/map.h>

#ifdef  __iDRAC__

#define ESC             0x1b
static struct tty_struct *idractty[4];  /* ttyS3-6 are idracttys */
static struct tty_struct **idracttyldr=idractty;
static struct tty_struct **preemptedldr=0;
static char idraclastch='\0';
static wait_queue_head_t idracttyldr_wait;
#define MODE1   1
#define MODE2A  3
#define MODE2B  4
#define MODE2C  2
static int idracmux=MODE2B;
static int idraclastmux=MODE2B;
static int idracmuxreq=MODE2B;
pid_t idracttypid[NR_PORTS];

#define WPCM450_GCR_INTCR2           WPCM450_GCR(0x60)
#define WPCM450_CLKSEL              (WPCM450_VA_GCR+0x200+(0x04))

static int snoopon=true;
static int idracdebug=false;
void wpcm450_setfilp(struct uart_port *port, struct file *filp);
void do_wpcm450_serial_bh(unsigned long);

DECLARE_TASKLET(bh_tasklet, do_wpcm450_serial_bh,0);


#ifdef SUPPORT_HISTBUF
#define HISTBUFMAX  (8*1024)
static int histbufmax=HISTBUFMAX;
static unsigned char histbuf[HISTBUFMAX];
static unsigned char *histinp=histbuf, *histoutp=histbuf, *myoutp;
struct tty_struct *flushtty;
struct timer_list flushtimer;
static int flushinghist=false;
static int histon=false;
#endif
#endif


unsigned long idrac_flags = 0;
static spinlock_t idrac_serial_spinlock = SPIN_LOCK_UNLOCKED;

/* platform ID */
//unsigned char kplatform_id = 0xFF; //initialized to undefined
static int read_platfrom_id_from_cpld(void);

#define MVP2000_ISR_PASS_LIMIT	256
#define CLEAR_RTS_THRESHOLD	    384 //This value is length of rx buffer in kernel minus 128

#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id);
#else
static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id, struct pt_regs *regs);
#endif
static void wpcm450_serial_set_mctrl(struct uart_port *port, unsigned int mctrl);
struct platform_device *wpcm450_uart_devs[NR_PORTS];
static struct wpcm450_uart_port wpcm450_serial_ports[];
static struct uart_driver wpcm450_uart_drv;

/* structures */

struct wpcm450_uart_info {
	char			*name;
	unsigned int		type;
	unsigned int		fifosize;

	/* uart controls */
	int (*reset_port)(struct uart_port *, struct wpcm450_uartcfg *);
};

struct wpcm450_uart_port {
	unsigned char			rx_claimed;
	unsigned char			tx_claimed;
	unsigned char           irq_flag; //this flag indicate whether the irq is requested or not
	struct wpcm450_uart_info	*info;
	struct wpcm450_uart_clksrc	*clksrc;
	struct clk			*clk;
	struct clk			*baudclk;
	struct uart_port	port;
	struct file         *filp;
};


/* configuration defines */
#if 1
#define dbg(x...) do {} while(0)
#else
#define dbg      printk
#endif    

/* UART name and device definitions */

#define WPCM450_SERIAL_NAME	"ttyS"
#define WPCM450_SERIAL_DEVFS    "tts/"
#define WPCM450_SERIAL_MAJOR	4
#define WPCM450_SERIAL_MINOR	64


/* conversion functions */

#define wpcm450_dev_to_port(__dev) (struct uart_port *)dev_get_drvdata(__dev)
#define wpcm450_dev_to_cfg(__dev) (struct wpcm450_uartcfg *)((__dev)->platform_data)

/* we can support 5 uarts, but not always use them */




/* port irq numbers */

#define TX_IRQ(port) ((port)->irq)
#define RX_IRQ(port) ((port)->irq)

/* register access controls */

#define portaddr(port, reg) ((port)->membase + (reg))

#define rd_regb(port, reg) (__raw_readb(portaddr(port, reg)))
#define rd_regl(port, reg) (__raw_readl(portaddr(port, reg)))

#define wr_regb(port, reg, val) \
  do { __raw_writeb(val, portaddr(port, reg)); } while(0)

#define wr_regl(port, reg, val) \
  do { __raw_writel(val, portaddr(port, reg)); } while(0)

/* macros to change one thing to another */

#define tx_enabled(port) ((port)->unused[0])
#define rx_enabled(port) ((port)->unused[1])

#define tx_disable(port) wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) & (~UART_IER_THRI))
#define tx_enable(port)	wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) | UART_IER_THRI)
#define rx_disable(port) wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) & ~UART_IER_RDI);	\
	wr_regb(port, WPCM450_COM_TOR, 0x00)
#define rx_enable(port)	wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) | UART_IER_RDI); \
	wr_regb(port, WPCM450_COM_TOR, 0x80 + 0x20)

/* flag to ignore all characters comming in */
#define RXSTAT_DUMMY_READ (0x10000000)


#ifdef __iDRAC__
#ifdef SUPPORT_HISTBUF
/*
 * These histbuf functions should be called with interrupts masked
 */
 
static inline void resethistbuf(void)
{
    histinp = histoutp = histbuf;
    flushinghist = false;
    del_timer(&flushtimer);
}


//increase index of histbuf with 1
static inline void wrapinc(unsigned char **p)
{
	if (++(*p) == histbuf+histbufmax)
		*p = histbuf;
}


//put character into histbuf
static inline void stuffhistbuf(unsigned char ch)
{
    *histinp = ch;
    
    wrapinc(&histinp);
    if (histinp == histoutp) {
        //when hist buffer is full, old data will be skipped.
        //    histbufoutp = histbufinp + 1
        if (flushinghist && myoutp == histoutp)
            wrapinc(&myoutp);
        wrapinc(&histoutp);
    }
}
    

static int read_platfrom_id_from_cpld(void)
{

#define SYSTEM_ID_ADDR      0xC4000003L
#define SYSTEM_ID_ADDR_2      	0xC4000022L

#define	SYSTEM_ID_MASK    0x0F
#define	SYSTEM_ID_BYTE2_MASK    0x04
#define SIZE_OF_SYSTEM_ID   1
#define DRIVER_NAME "wpcm450-uart"



	int	ret=0;
	struct resource		*tmp_mem;
	void __iomem 		*tmp_ioaddr;

	/** - Request the memory region for system ID */
	tmp_mem = request_mem_region(SYSTEM_ID_ADDR,SIZE_OF_SYSTEM_ID, DRIVER_NAME);
	if (!tmp_mem) {
		printk(KERN_ERR "%s: failed to request io memory region 0x%x.\n",
				__FUNCTION__, SYSTEM_ID_ADDR);
		return 0;
	}

	/* - Remap the memory region */
	tmp_ioaddr = ioremap_nocache(SYSTEM_ID_ADDR, SIZE_OF_SYSTEM_ID);
	if (0 == tmp_ioaddr) {
		release_mem_region(SYSTEM_ID_ADDR, SIZE_OF_SYSTEM_ID);
		printk(KERN_ERR "%s: failed to ioremap_nocache() io memory region.\n",
				__FUNCTION__);
		return 0;
	}

	//printk(KERN_CRIT "System ID= 0x%x\n", ioread8(tmp_ioaddr) & SYSTEM_ID_MASK);
	
        /** - Read system ID */
	ret = (ioread8(tmp_ioaddr) & SYSTEM_ID_MASK);
	iounmap(tmp_ioaddr);

   /*Read the second byte new implementation from Brutus onwards  */
   tmp_mem = request_mem_region(SYSTEM_ID_ADDR_2,SIZE_OF_SYSTEM_ID, DRIVER_NAME);
   if (!tmp_mem) {
	   printk(KERN_ERR "%s: failed to request io memory region 0x%x.\n",
			   __FUNCTION__, SYSTEM_ID_ADDR_2);
	   return 0;
   }
   
   /* - Remap the memory region */
   tmp_ioaddr = ioremap_nocache(SYSTEM_ID_ADDR_2, SIZE_OF_SYSTEM_ID);
   if (0 == tmp_ioaddr) {
	   release_mem_region(SYSTEM_ID_ADDR_2, SIZE_OF_SYSTEM_ID);
	   printk(KERN_ERR "%s: failed to ioremap_nocache() io memory region.\n",
			   __FUNCTION__);
	   return 0;
   }
   
   //printk(KERN_CRIT "System ID= 0x%x\n", ioread8(tmp_ioaddr) & SYSTEM_ID_MASK);
   
	   /** - Read system ID */
   ret |= ((ioread8(tmp_ioaddr) & SYSTEM_ID_BYTE2_MASK)<<2);


	/** - Unmap the memory region */
	iounmap(tmp_ioaddr);

	/** - Release the memory region */
	release_mem_region(SYSTEM_ID_ADDR, SIZE_OF_SYSTEM_ID);
	release_mem_region(SYSTEM_ID_ADDR_2, SIZE_OF_SYSTEM_ID);
	///printk(KERN_CRIT "Hari: System ID= 0x%x\n", ret);

	/** - Return the status */
	return ret;
}

int check_external_mux_presence(void)
{
    static int system_ID =0xFF;
    system_ID = read_platfrom_id_from_cpld();
	if(((system_ID > 9) && (system_ID < 14)) || (0x15 == system_ID) || (0x16 == system_ID))//
	{
/*
            0x0A= McBean
 *          0x0B= Mayzie
 *          0x0C= Samiam
 *          0x0D= Horton
 *			0x15= Diamas
 *			0x16= Coaster	
*/
	    return 0;  // No external Serial MUX.
	}
	else
	{
		return 1;  // External Serial MUX preset
	}
}


 /* WARNING: flushhist() must be called with serial_lock held.
 */
//flush data in histbuf to ttyS6(read direction)
static void flushhist(unsigned long firsttime)
{
    int size;

    int i=0;

    spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);

    if (firsttime)
    {
        myoutp = histoutp;
    }
	if (myoutp == histinp)
	{
		flushinghist = false;
	}
	
    for ( ; flushinghist; ) 
    {
        if (myoutp < histinp)
            size = histinp - myoutp;
        else
            size = histbuf + histbufmax - myoutp;
       
         if(NULL!=flushtty->buf.tail)
         {
            if (size > (TTY_FLIPBUF_SIZE - flushtty->buf.tail->used))
                size = TTY_FLIPBUF_SIZE - flushtty->buf.tail->used;
         }       

        if (size == 0) {
            init_timer(&flushtimer);
	        flushtimer.expires = jiffies + (HZ>>1); 
	        flushtimer.data = (unsigned long)false;
	        flushtimer.function = &flushhist;
	        add_timer(&flushtimer);
            break;
        }

        for(i=0; i<size;i++)
        {
          tty_insert_flip_char(flushtty,myoutp[i],TTY_NORMAL);
        }


        myoutp += size;
        if (myoutp >= histbuf + histbufmax)
            myoutp = histbuf;
        if (myoutp == histinp)
            flushinghist = false;

	    tty_flip_buffer_push(flushtty);
    }
   spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
}

#endif //end of #ifdef SUPPORT_HISTBUF
char *invalid(int mux)
{
    static char str[30];

    sprintf(str, "invalid (%d)", mux);
    return(str);
}


char *modestr(int mux)
{
    return(mux == MODE1 ? "MODE1" :
        mux == MODE2A ? "MODE2a" :
        mux == MODE2B ? "MODE2b" :
        mux == MODE2C ? "MODE2c" :            
            invalid(mux));
}


 //It will be called by driver to inform idracttyd to change mux stae.
static void muxchange_sendsignal(pid_t ttyspid, unsigned long index)
{
    struct task_struct *p =NULL;

    
    #ifdef IDRAC_SERIALDRIVER_DEBUG
    printk("\n%s(), pid is %d\n",__FUNCTION__,ttyspid);
    #endif
    
    if (ttyspid == 0)
    {
        return;
    }
	read_lock(&tasklist_lock);
 	p = find_task_by_pid(ttyspid);
 	if(p != NULL)
 	{
 	    if (p->pid == ttyspid) 
 	    {
			if (IDRACTTY_PID==index)
			{
                #ifdef IDRAC_SERIALDRIVER_DEBUG
			    printk("request mux changed\n");
			    #endif

			    send_sig(SIGUSR2,p,1);
			    send_sig(SIGCONT,p,1);
			}else if(RACTTY_PID == index || IPMITTY_PID == index)
			{
                #ifdef IDRAC_SERIALDRIVER_DEBUG
			    printk("send hang up signal\n");
			    #endif
			    send_sig(SIGUNUSED,p,1);
			    send_sig(SIGCONT,p,1);
			}
		}
	}
	read_unlock(&tasklist_lock);
}

void do_wpcm450_serial_bh(unsigned long data)
{
  muxchange_sendsignal(idracttypid[IDRACTTY_PID],IDRACTTY_PID);
}

 //Change ttyS port when mux state is changed or ttyS is opened,.....
 //Question: When the previous leader was preempted, should it be blocked or hang up?
/*
 * This function should always be protected by serial_lock.
 */
static void newttyldr(void)
{
    struct tty_struct **oldldr;
    int i = 0 ;
    pid_t ttypid;
    
    /*
     * ttyS0 - reserved for debug port
     * ttyS1 - unused
     * ttyS2 - mux change channel
     * ttyS3 - RAC serial console (preempts IPMI serial console)
     * ttyS4 - IPMI serial console
     * ttyS5 - not used
     * ttyS6 - RAC/IPMI host console redirection (preempts all)
     */
    oldldr = idracttyldr;
    switch (idracmux) {
    case MODE1:     /* ttyS6 */
	if(check_external_mux_presence())
	{
        #ifdef IPMI_TERMINAL_IN_MODE1
        idracttyldr = idractty+3;
        #else
        
        /**Firt, set tty leadership to ttyS[4]*/
        idracttyldr = idractty+1;
        if(idractty[3])
        {
          /** ttyS6 is opened, set tty leadership to ttyS[6]*/
          idracttyldr = idractty+3;
        }else if(idractty[0])
        {
        	/** ttyS3 is opened, set tty leadership to ttyS[3]*/
        	idracttyldr = idractty;
        }

        #endif
	}	
	else
	{
	    idracttyldr = idractty+3;
	}
        break;
    case MODE2A:    /* ttyS3 or ttyS4, in that order */
    case MODE2B:
        idracttyldr = idractty[0] ? idractty : (idractty[1] ? idractty+1 : idractty);
        break;
    case MODE2C:    /* ttyS6 */
        idracttyldr = idractty+3;
        break;
    }
    /*
     * When leadership changes, hangup the old leader and wakeup the new guy.
     */
    if (oldldr != idracttyldr) {
        if (*oldldr) 
        {
            preemptedldr = oldldr;//"preemptedldr == 0" means a ttyS is closed. It will be cleared in idracttyclr.
            i=(oldldr-idractty)+START_VIRTUAL_SERIAL_PORT_OFFSET;
            if(i>=START_VIRTUAL_SERIAL_PORT_OFFSET && i<NR_PORTS)
            {
                ttypid = idracttypid[i];
                if(wpcm450_serial_ports[i].port.info)
                {
                  wpcm450_serial_ports[i].port.info->flags |= UIF_NORMAL_ACTIVE; 
                  tty_hangup(*oldldr);
                  muxchange_sendsignal(ttypid, i);
                }

            }      
              
        } else if (*idracttyldr) {
            wake_up_interruptible_all(&idracttyldr_wait);
        }
    }
}

//close a tty.
//set idractty[i] to 0 and 
static void idracttyclr(struct uart_port *port)
{
    struct tty_struct *tty = port->info->tty;

    int i;
    for (i = 0; i < IDRACDEVS+1; i++)
    {

        if (idractty[i] == tty)
            break;
    }
    if (i == IDRACDEVS+1)
    {
        return;
    }
    /*
     * When the idracttyldr quits, may need a mux change or have a new election.
     */
    spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
    
    idractty[i] = 0;

    if (idracttyldr-idractty == i) {
        if (idracmux == MODE2C) 
        {
            idracmuxreq = idraclastmux;
            muxchange_sendsignal(idracttypid[IDRACTTY_PID],IDRACTTY_PID);
        } else {
            newttyldr();
        }
    }
    if (preemptedldr != 0 && preemptedldr-idractty == i) {

        preemptedldr = 0;
        wake_up_interruptible_all(&idracttyldr_wait);
    }
    
    spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
}
//reset variables to initial value and call idracttyclr()
void idracttycleanup(struct uart_port *port, int hangup)
{



    int offset = port->line - START_VIRTUAL_SERIAL_PORT_OFFSET;

    /*
     * Early in rs_open, the async_struct is not yet linked to the tty.
     */
    if (!(offset >= 0 && offset < sizeof(idractty)/sizeof(struct tty_struct *)))
        return;
        
    if (port->line >1 && port->line < NR_PORTS)
        idracttypid[port->line] = 0;

        
    if (offset == SOL_VIRTUAL_SERIAL_OFFSET) 
    {
        #ifdef SUPPORT_HISTBUF
        histon = false;
        #endif
    }
    idracttyclr(port);

}

void wpcm450_setfilp(struct uart_port *port, struct file *filp)
{
    wpcm450_serial_ports[port->line].filp = filp;
}
#endif

static inline struct wpcm450_uart_port *to_ourport(struct uart_port *port)
{
	return container_of(port, struct wpcm450_uart_port, port);
}

/* translate a port to the device name */

static inline const char *wpcm450_serial_portname(struct uart_port *port)
{
	return to_platform_device(port->dev)->name;
}
#if 0
static int wpcm450_serial_txempty_nofifo(struct uart_port *port)
{
	return (rd_regb(port, WPCM450_COM_LSR) & UART_LSR_THRE);
}

static void wpcm450_serial_rx_enable(struct uart_port *port)
{
	unsigned long flags;
	unsigned int fcr;


	spin_lock_irqsave(&port->lock, flags);

	fcr = rd_regb(port, WPCM450_COM_FCR);
	fcr |= UART_FCR_RFR | UART_FCR_FME | UARTx_FCR_FIFO_LEVEL8;
	wr_regb(port, WPCM450_COM_FCR, fcr);
	
	rx_enable(port);
	rx_enabled(port) = 1;
	spin_unlock_irqrestore(&port->lock, flags);
}

static void wpcm450_serial_rx_disable(struct uart_port *port)
{
	unsigned long flags;

  dbg("wpcm450_serial_rx_disable\n");
	spin_lock_irqsave(&port->lock, flags);

	rx_disable(port);

	rx_enabled(port) = 0;
	spin_unlock_irqrestore(&port->lock, flags);
}
#endif
static void wpcm450_serial_stop_tx(struct uart_port *port)
{
	if (tx_enabled(port)) {
		tx_disable(port);
		tx_enabled(port) = 0;
	}
}

static void wpcm450_serial_start_tx(struct uart_port *port)
{
    #ifdef __iDRAC__
    if(idracmux != MODE2A || DEBUG_SERIAL_PORT==port->line)
    {
    #endif

	    if (!tx_enabled(port)) 
	    {
		    tx_enable(port);
		    tx_enabled(port) = 1;
	    }
    #ifdef __iDRAC__
	}else
	{
        wpcm450_serial_stop_tx(port);
	}
	#endif
}


static void wpcm450_serial_stop_rx(struct uart_port *port)
{
	if (rx_enabled(port)) {
		dbg("wpcm450_serial_stop_rx: port=%p\n", port);
		rx_disable(port);
		rx_enabled(port) = 0;
	}
}

static void wpcm450_serial_enable_ms(struct uart_port *port)
{
     wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) | UART_IER_MSI);
}

static inline struct wpcm450_uart_info *wpcm450_port_to_info(struct uart_port *port)
{
	return to_ourport(port)->info;
}

static inline struct wpcm450_uartcfg *wpcm450_port_to_cfg(struct uart_port *port)
{
	if (port->dev == NULL)
		return NULL;

	return (struct wpcm450_uartcfg *)port->dev->platform_data;
}
static void wpcm450uart_modem_status (struct uart_port* port)
{

	unsigned int status = rd_regb(port, WPCM450_COM_MSR); /* Clear modem status intr */


   	#ifdef __iDRAC__
   	if(status & UART_MSR_DDCD)
   	    uart_handle_dcd_change (port, status & UART_MSR_DCD);
    #endif
	uart_handle_cts_change (port, ((status&UART_MSR_CTS)>>0x4));
	wake_up_interruptible (&port->info->delta_msr_wait);

}
		
		
static irqreturn_t
#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
wpcm450_serial_rx_chars(int irq, void *dev_id)
#else
wpcm450_serial_rx_chars(int irq, void *dev_id, struct pt_regs *regs)
#endif
{
	struct wpcm450_uart_port *ourport = dev_id;
	struct uart_port *port = &ourport->port;
	struct tty_struct *tty = port->info->tty;
	unsigned int iir, ch, flag;
    unsigned long flags;

	irqreturn_t ret;
    unsigned int status, pass_counter = MVP2000_ISR_PASS_LIMIT;
    unsigned int  lsr, max_count = 16;
    unsigned char iir_status= 0;

	spin_lock_irqsave(&port->lock, flags);
		
	status = rd_regb(port, WPCM450_COM_LSR);

	do 
	{
	    if (status & UART_LSR_DR)
        {
            while ((status & UART_LSR_DR) && max_count--) 
            {
		
		        ch = rd_regb(port, WPCM450_COM_RX);
		        flag = TTY_NORMAL;
   	            port->icount.rx++;


#ifdef  __iDRAC__
                #ifdef IDRAC_SERIALDRIVER_DEBUG
                    printk("receive_chars: 0x%x\n", ch);
                #endif    
        /*
         * When iDRAC snooping, we ignore chars until the
         * "<ESC>(" sequence is received. Then we pass
         * through chars until the "<ESC>Q" sequence is
         * received, returning to the initial state.
         */
                if (tty == *idracttyldr) 
                {
                    switch (idracmux) 
                    {
                        case MODE1:
                            #ifdef SUPPORT_HISTBUF
                            stuffhistbuf(ch);
                            if (flushinghist)
                                goto ignore_char;
                                
                            break;
                            #endif
                        case MODE2A:

                            if (idracmuxreq != MODE2A)//It means that the new state is not MODE2A. 
                               goto ignore_char;   //idracmuxreq != idracmux means that set mux ioctl command is not finished.

                       
                            if (idraclastch == ESC && ch == '(') 
                            {
                                idraclastch = '\0';
                                idracmuxreq = MODE2B;
                                printk("Mode2A to Mode2B\n");
                                tasklet_schedule(&bh_tasklet);
                            }
                            idraclastch = ch;
                            goto ignore_char;
                        case MODE2B:
                            if (idracmuxreq != MODE2B)
                                goto ignore_char;
                            
                            if ((port->line != IPMI_VIRTUAL_SERIAL_PORT || snoopon) &&
                                 idraclastch == ESC && ch == 'Q') 
                            {
                                idraclastch = '\0';
                                /*
                                 * Echo back on 2B to 2A transition.
                                 */

                                wpcm450_uart_drv.tty_driver->put_char(tty,ESC);
                                wpcm450_uart_drv.tty_driver->put_char(tty,'*');
                                wpcm450_uart_drv.tty_driver->flush_chars(tty);
                                idracmuxreq = MODE2A;
                                printk("Mode2B to Mode2A\n");
                                tasklet_schedule(&bh_tasklet);
                                goto ignore_char;
                            }
                            idraclastch = ch;
                            break;
                        case MODE2C:

                        default:
                            break;
                    }
                }
#endif    
    
    
		        lsr = rd_regb(port, WPCM450_COM_LSR);
		        if (unlikely(lsr & WPCM450_LSRSTAT_ANY)) 
		        {
			        if (lsr & UART_LSR_BI) 
			        {
				        lsr &= ~(UART_LSR_FE | UART_LSR_PE);
				        port->icount.brk++;
				        if (uart_handle_break(port))
					        goto ignore_char;
			        } else if (lsr & UART_LSR_PE)
			        {
				        port->icount.parity++;
			        }
			        else if (lsr & UART_LSR_FE)
				        port->icount.frame++;
			        if (lsr & UART_LSR_OE)
				        port->icount.overrun++;

			        lsr &= port->read_status_mask;

			        if (lsr & UART_LSR_BI)
				       flag = TTY_BREAK;
			        else if (lsr & UART_LSR_PE)
				       flag = TTY_PARITY;
			        else if (lsr & UART_LSR_FE)
				       flag = TTY_FRAME;
		        }
 
         		if (uart_handle_sysrq_char(port, ch & 255))
		        	goto ignore_char;

	            if (port->flags & UPF_CONS_FLOW)
	            {
		            
		            if(NULL!=tty->buf.tail)
		            {
		               if ((TTY_FLIPBUF_SIZE) <= tty->buf.tail->used)
		                  tty_flip_buffer_push(tty);
		            
		               if (CLEAR_RTS_THRESHOLD < tty->buf.tail->used)
		                  wpcm450_serial_set_mctrl(port,~(TIOCM_RTS));
		            }

		        }
		        uart_insert_char(port, lsr, UART_LSR_OE, ch, flag);


ignore_char:
		      status = rd_regb(port, WPCM450_COM_LSR);
            }// while
	      tty_flip_buffer_push(tty);
        }
        #ifndef __iDRAC__
        if (port->flags & UPF_CONS_FLOW)
	    {
        
	        if (port->flags & UPF_CONS_FLOW)
	        {	
	    #endif        
	           iir_status = rd_regb(port,WPCM450_COM_IIR)&0xf;
        #ifndef __iDRAC__
	        }    
	    #endif    
            if (UART_IIR_MS==iir_status)
            {
              wpcm450uart_modem_status(port);
            }
        #ifndef __iDRAC__
        }
        #endif
		if (status & UART_LSR_THRE)
		{
			#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
			ret = wpcm450_serial_tx_chars(irq, dev_id);
			#else
			ret = wpcm450_serial_tx_chars(irq, dev_id, regs);
			#endif
            iir = rd_regb(port, WPCM450_COM_IIR);
	    }
		if (pass_counter-- == 0)
			break;

		status = rd_regb(port, WPCM450_COM_LSR);
	} while (status & (UART_LSR_THRE|UART_LSR_DR));
	spin_unlock_irqrestore(&port->lock,flags);

	return IRQ_HANDLED;
/*******************************************/
}


#if LINUX_VERSION_CODE  == KERNEL_VERSION(2,6,23)
static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id)
#else    
static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id, struct pt_regs *regs)
#endif
{
	struct wpcm450_uart_port *ourport = id;
	struct uart_port *port = &ourport->port;
	struct circ_buf *xmit = &port->info->xmit;
	struct wpcm450_uart_info *info = ourport->info;
	int count = info->fifosize;

	if (port->x_char) {
		wr_regb(port, WPCM450_COM_TX, port->x_char);
		port->icount.tx++;
		port->x_char = 0;
		goto out;
	}

	/* if there isnt anything more to transmit, or the uart is now
	 * stopped, disable the uart and exit
	*/

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		wpcm450_serial_stop_tx(port);
		goto out;
	}

	/* try and drain the buffer... */

	while (!uart_circ_empty(xmit) && count-- > 0) {
		wr_regb(port, WPCM450_COM_TX, xmit->buf[xmit->tail]);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
	}
  dbg("\nxmit->tail=%x, xmit->head=%x, xmit->buf = %x\n",xmit->tail,xmit->head,xmit->buf);
	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
	{
		wpcm450_serial_stop_tx(port);
	}	

 out:
	return IRQ_HANDLED;
}

static unsigned int wpcm450_serial_tx_empty(struct uart_port *port)
{

	unsigned long lsrstat = rd_regb(port, WPCM450_COM_LSR);

    if(lsrstat&UART_LSR_THRE)
		return 1;
	else
		return 0;		
}

/* no modem control lines */
static unsigned int wpcm450_serial_get_mctrl(struct uart_port *port)
{
	unsigned int result = 0;
	unsigned int status = rd_regb(port,WPCM450_COM_MSR);

    
    #ifdef __iDRAC__	
	if (status & UART_MSR_DCD)
		result |= TIOCM_CAR;
	if (status & UART_MSR_DSR)
		result |= TIOCM_DSR;
	#endif	

    if (port->flags & UPF_CONS_FLOW)
    {
	    if (UART_MSR_CTS==(status & UART_MSR_CTS))
		    result |= TIOCM_CTS;
	}
	return result;
}

static void wpcm450_serial_set_mctrl(struct uart_port *port, unsigned int mctrl)
{

    if (mctrl & TIOCM_RTS)        
	{
        /*Set RTS */
	    wr_regb(port, WPCM450_COM_MCR, UART_MCR_RTS_SET);  
	}else
	{   
	    /*Clear RTS*/
	    wr_regb(port, WPCM450_COM_MCR, UART_MCR_RTS_CLEAR);
	}
}

static void wpcm450_serial_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;
	unsigned int ucon;

	spin_lock_irqsave(&port->lock, flags);

	ucon = rd_regb(port, WPCM450_COM_LCR);

	if (break_state)
		ucon |= UART_LCR_SBC;
	else
		ucon &= ~UART_LCR_SBC;

	wr_regb(port, WPCM450_COM_LCR, ucon);

	spin_unlock_irqrestore(&port->lock, flags);
}

static void wpcm450_serial_shutdown(struct uart_port *port)
{
	unsigned long flags;
	struct wpcm450_uart_port *ourport = to_ourport(port);


	
#ifndef __iDRAC__
	if (ourport->tx_claimed || ourport->rx_claimed) 
		free_irq(RX_IRQ(port), ourport);
	
	if (ourport->tx_claimed) {	
		tx_disable(port);
		tx_enabled(port) = 0;
		ourport->tx_claimed = 0;
	}

	if (ourport->rx_claimed) {
		dbg("wpcm450_serial_shutdown\n");
		rx_disable(port);
		ourport->rx_claimed = 0;
		rx_enabled(port) = 0;
	}
#else
    #ifdef IDRAC_SERIALDRIVER_DEBUG
    printk("%s():port->line %x\n",__FUNCTION__,port->line);
    #endif
    spin_lock_irqsave(&port->lock, flags);
    if(RAC_VIRTUAL_SERIAL_PORT==port->line || IPMI_VIRTUAL_SERIAL_PORT==port->line ||
        SOL_VIRTUAL_SERIAL_PORT==port->line||DEBUG_SERIAL_PORT==port->line)
    {
        if(1==ourport->irq_flag);
            free_irq(RX_IRQ(port), ourport);
        
	    ourport->irq_flag =0;

       
       if((0==wpcm450_serial_ports[RAC_VIRTUAL_SERIAL_PORT].irq_flag&&
          0==wpcm450_serial_ports[IPMI_VIRTUAL_SERIAL_PORT].irq_flag&&
          0==wpcm450_serial_ports[SOL_VIRTUAL_SERIAL_PORT].irq_flag)
          || DEBUG_SERIAL_PORT==port->line)
       {
          tx_disable(port);
          rx_disable(port);
          tx_enabled(port) = 0;
		  ourport->tx_claimed = 0;
		  ourport->rx_claimed = 0;
		  rx_enabled(port) = 0;

         
       }   
    }
    spin_unlock_irqrestore(&port->lock, flags);    
        #ifdef SUPPORT_HISTBUF 
        /*
         * idracttyd always has a socket open to ensure that the history buffer
         * is maintained in MODE1.
         */
        if (idracmux == MODE1 &&  SOL_VIRTUAL_SERIAL_PORT == port->line) 
        {
            if (flushinghist) {
                flushinghist = false;
                del_timer(&flushtimer);
            }
        }

        #endif
    
    idracttycleanup(port, false);
#endif    	
}


static int wpcm450_serial_startup(struct uart_port *port)
{
	struct wpcm450_uart_port *ourport = to_ourport(port);
	int ret = 0;
	
	

#ifdef __iDRAC__
    static int init=true;
    unsigned long   flags;
    int rc, idracredirect, offset;
    struct tty_struct *tty = port->info->tty;
    struct file* filp = NULL ;

    if(RAC_VIRTUAL_SERIAL_PORT== port->line || IPMI_VIRTUAL_SERIAL_PORT == port->line||
       SOL_VIRTUAL_SERIAL_PORT== port->line)
    {
        filp = wpcm450_serial_ports[port->line].filp;
  
    }
    
    
    if (init) {
        init = false;
        init_waitqueue_head(&idracttyldr_wait);
    }

    /*
     * Get the pid of idracttypid[] opening ttyS2, ttyS3, ttyS4, ttyS6.
     */
  
    if(port->line > 1 && port->line < NR_PORTS)
      idracttypid[port->line] = current->pid;

    /*
     * Perform iDRAC port mapping.
     */

    idracredirect = false;
    offset = port->line - START_VIRTUAL_SERIAL_PORT_OFFSET;
    if (offset >= 0 && offset < sizeof(idractty)/sizeof(struct tty_struct *)) 
    {


        if (RESERVED2_SERIAL_OFFSET == offset)
        {
            idracttypid[port->line] =0;
		    return -EINVAL;
        }
        idracredirect = true;

    }

    /*
     * With iDRAC redirection we may need to kick somebody off the port.
     */
    if (idracredirect) 
    {
        #ifdef SUPPORT_DCD
        if(filp)
        {
            if(filp->f_flags & O_NONBLOCK)
            {
               if (TIOCM_CAR != (wpcm450_serial_get_mctrl(port) & TIOCM_CAR))            
               {
                   idracttypid[port->line] = 0;
                   return -EAGAIN;
               }
            }
        }
        #endif

        #ifdef IDRAC_SERIALDRIVER_DEBUG
        if(port->line != 4)
           printk("open ttyS%d \n",port->line);
        #endif   
        spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
        if (idractty[offset] == 0) 
        {
            idractty[offset] = tty;
            if (offset == SOL_VIRTUAL_SERIAL_OFFSET && idracmux != MODE1&& idracmux != MODE2C) 
            {
                idracmuxreq = MODE2C;
                muxchange_sendsignal(idracttypid[IDRACTTY_PID],IDRACTTY_PID);
            } else 
            {
                newttyldr();
            }
        }else
        {
           /*idractty[offset] != 0 means previous tty is not cleaned.*/
           spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
           idracttypid[port->line] = 0;
           
           #ifdef IDRAC_SERIALDRIVER_DEBUG
           printk("open ttyS%d again. return EAGAIN error code\n",port->line);
           #endif
           return -EAGAIN;
        }    
        spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);

        /*
         * Only the idracttyldr gets to set state->info.
         */
        // 1. preemptedldr != 0 means that ttyS of previous leader is not closed completely.
        // 2. tty != *idracttyldr means that tty(idracttyldr) of current leader != current opened tty.
        //   =>when idracmuxreq == MODE2C, if idractty daemon does't set mux. Then tty!=*idracttyldr
        if (!(tty == *idracttyldr && preemptedldr == 0)) 
        {
            if(filp)
            {
                if (filp->f_flags & O_NONBLOCK) 
                {
                    idracttycleanup(port, false);
		            return (tty == *idracttyldr) ? -EAGAIN : -EBUSY;
		        }
		    }
            		        

            #ifdef IDRAC_SERIALDRIVER_DEBUG
            printk("startup(): wait_event_interuptible\n");
            #endif

            //preemptedldr ==0 means that  ttyS of previous leader was closed.
		    rc = wait_event_interruptible(idracttyldr_wait, tty == *idracttyldr && preemptedldr == 0);

		    if (rc != 0) 
		    {
                idracttycleanup(port, false);
		        return rc;
		    }
        }

        /*
         * ttyS3, ttyS4, and ttyS6 special case CRTSCTS and CLOCAL.
         */
        spin_lock_irqsave(&port->lock, flags);
        if (offset == RAC_VIRTUAL_SERIAL_OFFSET || offset == IPMI_VIRTUAL_SERIAL_OFFSET) 
        {
                //CTSRTS/DCD
                //enable modem status but H/W flow control is set by AP not always on

                /*Disable Modem Status*/
                tty->termios->c_cflag |= CLOCAL;
                tty->termios->c_cflag &= (~HUPCL);
                port->info->flags &= (~UIF_CHECK_CD);                
               #ifdef SUPPORT_DCD
               if (idracmux != MODE2A) 
               {
                 /*Enable Modem Status*/
                 tty->termios->c_cflag &= ~CLOCAL;
                 port->info->flags |= UIF_CHECK_CD;
                 tty->termios->c_cflag |= (HUPCL);
               }
               #endif
               //tty->termios->c_cflag |= (CRTSCTS);
               tty->termios->c_cc[VMIN] =  1;
               tty->termios->c_cc[VTIME] = 0;
                

        }
        spin_unlock_irqrestore(&port->lock, flags);
    }

  
      
    /*
     * ttyS6 optionally flushes the history in MODE1
     */
    #ifdef SUPPORT_HISTBUF
    if (idracmux == MODE1 && offset == SOL_VIRTUAL_SERIAL_OFFSET && histon) {
        histon = false;
        flushtty = tty;
        flushinghist = true;
        flushhist(true);
    }
    #endif

#endif//end of #ifdef __iDRAC__

	  dbg("wpcm450_serial_startup: mapbase=%p membase=%x irq=%d\n",
	    port->mapbase, port->membase, RX_IRQ(port));
    spin_lock_irqsave(&port->lock, flags);
    #ifdef __iDRAC__
    if (DEBUG_SERIAL_PORT ==port->line)
    #endif
    {
       ourport->irq_flag = 1 ; 
	   ret = request_irq(RX_IRQ(port),
			  wpcm450_serial_rx_chars, IRQF_DISABLED|IRQF_SAMPLE_RANDOM,
			  wpcm450_serial_portname(port), ourport);
    }
    #ifdef __iDRAC__   
    
    if(RAC_VIRTUAL_SERIAL_PORT==port->line || IPMI_VIRTUAL_SERIAL_PORT==port->line
        || SOL_VIRTUAL_SERIAL_PORT==port->line)
    {
		ret = request_irq(RX_IRQ(port),
		wpcm450_serial_rx_chars, IRQF_SHARED|IRQF_SAMPLE_RANDOM,
		wpcm450_serial_portname(port), ourport);	  
		ourport->irq_flag = 1 ;  	  
    }


    #endif 			  
    spin_unlock_irqrestore(&port->lock, flags);
	if (ret != 0) {
		printk(KERN_ERR "cannot get irq %d\n", RX_IRQ(port));
		idracttypid[port->line] = 0;
		return ret;
	}

   #ifdef __iDRAC__   	
   if(DEBUG_SERIAL_PORT==port->line || RAC_VIRTUAL_SERIAL_PORT == port->line ||
     IPMI_VIRTUAL_SERIAL_PORT==port->line||SOL_VIRTUAL_SERIAL_PORT==port->line)
   #endif
   {
   
       dbg("rx enable\n");
	   rx_enable(port);
	   rx_enabled(port) = 1;
       ourport->rx_claimed = 1;

	   dbg("requesting tx irq...\n");

	   ourport->tx_claimed = 1;
	}
    if(DEBUG_SERIAL_PORT == port->line|| SOL_VIRTUAL_SERIAL_PORT ==port->line)
    {
    	tty->termios->c_cflag &= ~(HUPCL | CMSPAR);
    	tty->termios->c_cflag |= CLOCAL;
    }
	dbg("wpcm450_serial_startup ok\n");

	/* the port reset code should have done the correct
	 * register setup for the port controls */
	return ret;

}

/* power power management control */

static void wpcm450_serial_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
#if 0
	struct wpcm450_uart_port *ourport = to_ourport(port);
	switch (level) {
	case 3:
		if (!IS_ERR(ourport->baudclk) && ourport->baudclk != NULL)
			clk_disable(ourport->baudclk);

		clk_disable(ourport->clk);
		break;

	case 0:
		clk_enable(ourport->clk);

		if (!IS_ERR(ourport->baudclk) && ourport->baudclk != NULL)
			clk_enable(ourport->baudclk);

		break;
	default:
		printk(KERN_ERR "wpcm450_serial: unknown pm %d\n", level);
	}
#endif
}

/* baud rate calculation
 *
 * The UARTs on the WPCM450/S3C2440 can take their clocks from a number
 * of different sources, including the peripheral clock ("pclk") and an
 * external clock ("uclk"). The S3C2440 also adds the core clock ("fclk")
 * with a programmable extra divisor.
 *
 * The following code goes through the clock sources, and calculates the
 * baud clocks (and the resultant actual baud rates) and then tries to
 * pick the closest one and select that.
 *
*/


#define MAX_CLKS (8)

struct baud_calc {
	struct wpcm450_uart_clksrc	*clksrc;
	unsigned int			 calc;
	unsigned int			 quot;
	struct clk			*src;
};
#if 0
static int wpcm450_serial_calcbaud(struct baud_calc *calc,
				   struct uart_port *port,
				   struct wpcm450_uart_clksrc *clksrc,
				   unsigned int baud)
{
	unsigned long rate = port->uartclk;
	
	
	
	calc->quot = rate / (baud * 16);

	calc->quot -= 2;
	
	return 1;
}

static unsigned int wpcm450_serial_getclk(struct uart_port *port,
					  struct wpcm450_uart_clksrc **clksrc,
					  struct clk **clk,
					  unsigned int baud)
{
	struct wpcm450_uartcfg *cfg = wpcm450_port_to_cfg(port);
	struct wpcm450_uart_clksrc *clkp;
	struct baud_calc res;
	
	
	clkp = (wpcm450_uart_clksrc *)cfg->clocks;
	wpcm450_serial_calcbaud(&res, port, clkp, baud);
	

	/* ok, we now need to select the best clock we found */

	dbg(KERN_DEBUG "selected clock %p quot %d\n",
	       port->uartclk, res.quot);

	/* store results to pass back */

	*clksrc = res.clksrc;
	*clk    = res.src;

	return res.quot;
}
#endif
static void wpcm450_serial_set_termios(struct uart_port *port,
				       struct ktermios  *termios,
				       struct ktermios *old)
{


	

	unsigned long flags;
	unsigned int baud, quot;
	unsigned int ulcon;


    #ifdef __iDRAC__
    //Don't allow IDRACTTY_DAEMON_SERIAL_PORT, RESERVED1_SERIAL_PORT, RESERVED2_SERIAL_PORT 
    //to set termios. Otherwise termios may not correct, like baudrate.
    if(IDRACTTY_DAEMON_SERIAL_PORT == port->line || RESERVED1_SERIAL_PORT == port->line ||
    RESERVED2_SERIAL_PORT == port->line)
    return;
    #endif

	/*
	 * We don't support modem control lines.
	 */
	#ifndef __iDRAC__
	termios->c_cflag &= ~(HUPCL | CMSPAR);
	termios->c_cflag |= CLOCAL;
    #endif
    
  
    if(RAC_VIRTUAL_SERIAL_PORT == port->line || IPMI_VIRTUAL_SERIAL_PORT == port->line) 
    {
       /*Disable Modem Status*/
       termios->c_cflag |= CLOCAL;
       termios->c_cflag &= (~HUPCL);
       port->info->flags &= (~UIF_CHECK_CD);                

       #ifdef SUPPORT_DCD
       if (idracmux != MODE2A) 
       {
          /*Enable Modem Status*/
          termios->c_cflag &= ~CLOCAL;
          port->info->flags |= UIF_CHECK_CD;
          termios->c_cflag |= (HUPCL);
       }
       #endif
    }


    if(termios->c_cflag & CRTSCTS)
    {
		if(!check_external_mux_presence())
		{
			wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) | UART_IER_MSI);
		}
        port->flags |= UPF_CONS_FLOW;
        wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) | UART_IER_MSI);
        wpcm450_serial_set_mctrl(port, UART_MCR_RTS_SET);
    }   
    else
    {    
    
        port->flags &= (~UPF_CONS_FLOW);
        wr_regb(port, WPCM450_COM_IER, rd_regb(port, WPCM450_COM_IER) & (~UART_IER_MSI));
        wpcm450_serial_set_mctrl(port, UART_MCR_RTS_CLEAR);
    }

	/*
	 * Ask the core to calculate the divisor for us.
	 */

	baud = uart_get_baud_rate(port, termios, old, 0, port->uartclk/16);
    dbg("wpcm450_serial_set_termios baud = %d\n",baud);

    #ifdef IDRAC_SERIALDRIVER_DEBUG
    if (9600==baud)
	   printk("KN:Serial=> 9600 bit/s\n");//quot = 154
	else if(19200==baud)
	  printk("KN:Serial=> 19200 bit/s\n");//quot = 76
    else if(38400==baud)
      printk("KN:Serial=> 38400 bit/s\n");//quot = 37
    else if(57600==baud)
      printk("KN:Serial=> 57600 bit/s\n");//quot = 24
    else
     printk("KN:Serial=> 115200 bit/s\n");//quot = 11
    #endif
 
    quot = (port->uartclk/(16 * baud)) -2;

 
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		dbg("config: 5bits/char\n");
		ulcon = UART_LCR_WLEN5;
		break;
	case CS6:
		dbg("config: 6bits/char\n");
		ulcon = UART_LCR_WLEN6;
		break;
	case CS7:
		dbg("config: 7bits/char\n");
		ulcon = UART_LCR_WLEN7;
		break;
	case CS8:
	default:
		dbg("config: 8bits/char\n");
		ulcon = UART_LCR_WLEN8;
		break;
	}

	if (termios->c_cflag & CSTOPB)
		ulcon |= UART_LCR_NSB;

	if (termios->c_cflag & PARENB) {
		ulcon |= UART_LCR_PARITY;
		if (termios->c_cflag & PARODD)
			ulcon |= UART_LCR_OPAR;
		else
			ulcon |= UART_LCR_EPAR;
	} else {
		ulcon |= UART_LCR_NPAR;
	}

	spin_lock_irqsave(&port->lock, flags);

	wr_regb (port, WPCM450_COM_LCR, 0x80); /* select divisor latch registers */    
    wr_regb (port, WPCM450_COM_LSB, quot&0x00FF);	  /* Baud rate = 9600   : 15,000,000/(16*(96+2)) */
    wr_regb (port, WPCM450_COM_MSB, ((quot&0xFF00)>>8));
	wr_regb(port, WPCM450_COM_LCR, ulcon);
	
	dbg("uart: quot = 0x%08x\n",   quot);

	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);

	/*
	 * Which character status flags are we interested in?
	 */
	port->read_status_mask = UART_LSR_OE;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= UART_LSR_FE | UART_LSR_PE;

	/*
	 * Which character status flags should we ignore?
	 */
	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UART_LSR_OE;
	if (termios->c_iflag & IGNBRK && termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UART_LSR_FE;

	/*
	 * Ignore all characters if CREAD is not set.
	 */
	if ((termios->c_cflag & CREAD) == 0)
		port->ignore_status_mask |= RXSTAT_DUMMY_READ;

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *wpcm450_serial_type(struct uart_port *port)
{
	switch (port->type) {
	case PORT_WPCM450:
		return "WPCM450";
	default:
		return NULL;
	}
}

#define MAP_SIZE (0x100)

static void wpcm450_serial_release_port(struct uart_port *port)
{
	release_mem_region(port->mapbase, MAP_SIZE);
}

static int wpcm450_serial_request_port(struct uart_port *port)
{
	const char *name = wpcm450_serial_portname(port);
	return request_mem_region(port->mapbase, MAP_SIZE, name) ? 0 : -EBUSY;
}

static void wpcm450_serial_config_port(struct uart_port *port, int flags)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

#ifndef __iDRAC__
	if (flags & UART_CONFIG_TYPE &&
	    wpcm450_serial_request_port(port) == 0)
		port->type = info->type;
#else
	   wpcm450_serial_request_port(port);
		port->type = info->type;

#endif		
}

/*
 * verify the new serial_struct (for TIOCSSERIAL).
 */
static int
wpcm450_serial_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

	if (ser->type != PORT_UNKNOWN && ser->type != info->type)
		return -EINVAL;

	return 0;
}
static int
wpcm450_serial_ioctl(struct uart_port *port, unsigned int cmd , unsigned long arg)
{

   unsigned char i = 0;
   unsigned long histbuflen = 0;
   pid_t ttypid;
    
    static int initialized=true;
    int muxnew;
    switch (cmd) 
    {
#ifdef  __iDRAC__
            
            
        case TIOCSERGMUX:
            /*
             * Supply the idracmuxreq value, so that a daemon can switch the mux.
             */
            if (copy_to_user((void *)arg, (void *)&idracmuxreq, sizeof(idracmuxreq)))
				return -EFAULT;
            break;

        case TIOCSERSMUX:
        if(!check_external_mux_presence())
		{     
			 /* The mux daemon is notifying here.
             */
            if (copy_from_user((void *)&muxnew, (void *)arg, sizeof(idracmux)))
		        return -EFAULT;

  
            spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
            if (!initialized)
            {
                initialized = true;
                unsigned int u32Data = (__raw_readl(WPCM450_CLKSEL) | (0x00000400));
                idracmux = idraclastmux = idracmuxreq = muxnew;
                __raw_writel (u32Data,WPCM450_CLKSEL);
            }
            else if (muxnew != idracmux) 
            {
            	/*
            	#define WPCM450_GCR_SPSWC  0xb0000038
            	#define WPCM450_GCR_INTCR2 0xb0000060
            	#define WPCM450_GCR_MFSEL1 0xb000000c
            	*/
                unsigned long u32SPSWC = (__raw_readl(WPCM450_GCR_SPSWC) & (~0x00000003));
                unsigned long u32INTCR2 = (__raw_readl(WPCM450_GCR_INTCR2));
                //unsigned long u32MFSEL1 = (__raw_readl(WPCM450_GCR_MFSEL1));
                switch(muxnew)
                {
            	/*
                	  Mayzie have only one com port; Therefore, we set MODE1 and
                	  MODE2A direct to host.
            	*/
                	case MODE1: 
                		//enable SDIOSEL(0xb000000c:bit 30)
                		//enable HSP2SEL(0xb000000c:bit 11)
                		//enable SP1_SI2(0xb0000060:bit 16)
                		//enable mode 3
                		u32SPSWC = u32SPSWC | 0x2; 
                		u32INTCR2 = u32INTCR2 | 0x00010000;                		
                		//u32MFSEL1 = (u32MFSEL1 | 0x40000800);
                	break;
                	case MODE2A:
                		//disable SP1_SI2(0xb0000060:bit 16)
                		//enable mode 1
                		u32SPSWC = u32SPSWC & (~0x00000003);
                		u32INTCR2 = u32INTCR2 & (~0x00010000);
                	break;
                	case MODE2B:
                		//disable SP1_SI2(0xb0000060:bit 16)
                		//enable mode 2
                		u32SPSWC = u32SPSWC | 0x1;
                		u32INTCR2 = u32INTCR2 & (~0x00010000);
                	break;
                	case MODE2C:
                		//disable SP1_SI2(0xb0000060:bit 16)
                		//enable mode 3
                		u32SPSWC = u32SPSWC | 0x2;
                		u32INTCR2 = u32INTCR2 & (~0x00010000);
                	break;

                }
                idraclastmux = idracmux;
                idracmux = idracmuxreq = muxnew;
                //follow description in INTCR2, SPEC p.88
                __raw_writel (u32SPSWC,WPCM450_GCR_SPSWC);
                //__raw_writel (u32MFSEL1,WPCM450_GCR_MFSEL1);
                __raw_writel (u32INTCR2,WPCM450_GCR_INTCR2);
            }
            spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
            break;
	}
	else
	{

            static int initialized=true;
            int muxnew;
            
            //Driver will initialize mux state as Mode2b
            /*
             * The mux daemon is notifying here.
             */
            if (copy_from_user((void *)&muxnew, (void *)arg, sizeof(idracmux)))
		        return -EFAULT;

  
            spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
            if (!initialized) {
                initialized = true;
                idracmux = idraclastmux = idracmuxreq = muxnew;
            } else if (muxnew != idracmux) 
            {
                #ifdef SUPPORT_HISTBUF
                resethistbuf();
                #endif
                if (muxnew == MODE1) {
                    idracmux = idraclastmux = idracmuxreq = MODE1;
                } else if (idraclastmux == MODE1) {
                    idraclastmux = MODE2B;
                    idracmux = idracmuxreq = muxnew;
                } else {
                    /*
                     * The reason idracmuxreq is set here is that the mux may have been changed
                     * by an external source rather than this driver. We react to such
                     * external mux changes as if they were made by this driver.
                     */
                    if(idracmux != muxnew)//If current mode == new mode, done't change last mode. Otherwise lastmode == current mode
                    {
                        idraclastmux = idracmux;
                    }
                    idracmux = idracmuxreq = muxnew;
                }
                newttyldr();
                /*
                 * For security reasons, we require a re-open on transitions between
                 * MODE2A and MODE2B. Additionally, MODE2A snooping cannot use CRTSCTS,
                 * but it is required in MODE2B.
                 */
                 #ifdef IDRAC_SERIALDRIVER_DEBUG
                 printk(" idraclastmux is %s\n",modestr(idraclastmux));
                 printk(" idracmuxreq is %s\n",modestr(idracmuxreq));
                 printk(" muxnew is %s\n",modestr(muxnew));
                 printk(" idracmux is %s\n",modestr(idracmux));
                 #endif
                 
                
                if (*idracttyldr &&
                        ((idracmux == MODE2A && idraclastmux == MODE2B) ||
                        (idracmux == MODE2B && idraclastmux == MODE2A)))
                 {       

                     i=(idracttyldr-idractty)+START_VIRTUAL_SERIAL_PORT_OFFSET;
                     if(i>=START_VIRTUAL_SERIAL_PORT_OFFSET && i<NR_PORTS)
                     {
                        ttypid = idracttypid[i];
                        

                        /*if AP is slept when opening ttyS, UIF_NORMAL_ACTIVE won't be set in uart_open()*/
                        /*When this ttyS is hung up, the shutdown() won't be called. *
                         *This is because of UIF_NORMAL_ACTIVE won't be set.
                         *If shutdown() is not called, the idracttycleanup() is also not called.*
                         *It will cause unpredictable situation*
                         *So set UIF_NORMAL_ACTIVE in info->flags then shutdown() will be called.*/

                        if(wpcm450_serial_ports[i].port.info)
                        {
                          wpcm450_serial_ports[i].port.info->flags |= UIF_NORMAL_ACTIVE; 
                          tty_hangup(*idracttyldr);
                          muxchange_sendsignal(ttypid,i);
                        }

                    }
                 }

            }
            spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
            break;
        }
		break; 
        case TIOCHISTON:
            #ifdef SUPPORT_HISTBUF
            histon = true;
            #endif
            break;
        case TIOCIDRACTTYBAUD:
        #if 0
        {
#define BAUDSTR(b)  (b == B9600 ? "9600" : b == B19200 ? "19200" : b == B38400 ? "38400" : \
                        b == B57600 ? "57600" : b == B115200 ? "115200" : "invalid")
#define RACSERIAL   0
#define IPMISERIAL  1
#define SOL         2
#define INVALID_NO  0xFF
            int i, idracbauds[3];
            int index = 0;
            pid_t ttypid;

            if (copy_from_user((void *)idracbauds, (void *)arg, sizeof(idracbauds)))
				return -EFAULT;
            printk("idracioctl: bauds ttyS3=%s ttyS4=%s ttyS6=%s\n",
                BAUDSTR(idracbauds[RACSERIAL]), BAUDSTR(idracbauds[IPMISERIAL]), BAUDSTR(idracbauds[SOL]));
            for (i = 0; i < sizeof(idracbauds)/sizeof(int); i++) {
                switch (idracbauds[i]) {
                case B9600:
                case B19200:
                case B38400:
                case B57600:
                case B115200:
                    break;
                default:
                    idracbauds[i] = B115200;
                }
            }
            /*
             * When changing the baud rate, we hangup the port, forcing everyone
             * to reopen at the new baud rate.
             */
            spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
            if(*idracttyldr)
            {   
                i = idracttyldr-idractty;   
                switch (i)
                {
                    case RAC_VIRTUAL_SERIAL_OFFSET:
                        index = RACSERIAL;
                        break;
                    case IPMI_VIRTUAL_SERIAL_OFFSET:
                        index = IPMISERIAL;
                        break;
                    case SOL_VIRTUAL_SERIAL_OFFSET:
                       index = SOL;
                        break;  
                    default:    
                        index = INVALID_NO;
                        break;
                } 
                if(INVALID_NO != index)
                {
                  if (((*idracttyldr)->termios->c_cflag&CBAUD) != idracbauds[index] && idracbauds[index]) 
                  {
                     (*idracttyldr)->termios->c_cflag &= ~CBAUD;
                     (*idracttyldr)->termios->c_cflag |= idracbauds[index];

                     i += START_VIRTUAL_SERIAL_PORT_OFFSET; 
                     ttypid = idracttypid[i];
                    
                     if(wpcm450_serial_ports[i].port.info)
                     {
                         wpcm450_serial_ports[i].port.info->flags |= UIF_NORMAL_ACTIVE; 
                         tty_hangup(*idracttyldr);
                         muxchange_sendsignal(ttypid,i);
                     }

                  }
                }
            }
            spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
            break;
        }
        #else
            return -ENOIOCTLCMD;
        #endif
        case TIOCSNOOPON:
            printk("idracioctl: snooping on\n");
            snoopon = true;
            break;
        case TIOCSNOOPOFF:
            /*snooping always on*/
            //printk("idracioctl: snooping off\n");
            //snoopon = FALSE;
            break;
        case TIOCSDEBUG:
            idracdebug = !idracdebug;
            printk("idracioctl: idracdebug %s\n", idracdebug ? "on" : "off");
            break;
        case TIOCHISTBUFMAX:
            #ifdef SUPPORT_HISTBUF
            
            if (copy_from_user((void *)&histbuflen, (void *)arg, sizeof(unsigned long)))
		        return -EFAULT;
		        
			if (histbuflen > HISTBUFMAX)
				histbuflen = HISTBUFMAX;
			if (histbuflen == 0)
				return -EINVAL;
			printk("idracioctl: histbufmax= %ld\n", histbuflen);
			spin_lock_irqsave(&idrac_serial_spinlock, idrac_flags);
			histbufmax = histbuflen;
            resethistbuf();
			spin_unlock_irqrestore(&idrac_serial_spinlock, idrac_flags);
			#endif
            break;
#endif          
        default:
          return -ENOIOCTLCMD;
          break;
    }
    


	return 0;
}

#ifdef CONFIG_SERIAL_WPCM450_CONSOLE

static struct console wpcm450_serial_console;

#define WPCM450_SERIAL_CONSOLE &wpcm450_serial_console
#else
#define WPCM450_SERIAL_CONSOLE NULL
#endif

static struct uart_ops wpcm450_serial_ops = {
	.pm		= wpcm450_serial_pm,
	.tx_empty	= wpcm450_serial_tx_empty,
	.get_mctrl	= wpcm450_serial_get_mctrl,
	.set_mctrl	= wpcm450_serial_set_mctrl,
	.stop_tx	= wpcm450_serial_stop_tx,
	.start_tx	= wpcm450_serial_start_tx,
	.stop_rx	= wpcm450_serial_stop_rx,
	.enable_ms	= wpcm450_serial_enable_ms,
	.break_ctl	= wpcm450_serial_break_ctl,
	.startup	= wpcm450_serial_startup,
	.shutdown	= wpcm450_serial_shutdown,
	.set_termios	= wpcm450_serial_set_termios,
	.type		= wpcm450_serial_type,
	.release_port	= wpcm450_serial_release_port,
	.request_port	= wpcm450_serial_request_port,
	.config_port	= wpcm450_serial_config_port,
	.verify_port	= wpcm450_serial_verify_port,
#ifdef __iDRAC__	
	.ioctl      	= wpcm450_serial_ioctl,
#endif	
};


static struct uart_driver wpcm450_uart_drv = {
	.owner		= THIS_MODULE,
	.dev_name	= "wpcm450_serial",
	.nr		= NR_PORTS,
	.cons		= WPCM450_SERIAL_CONSOLE,
	.driver_name	= WPCM450_SERIAL_NAME,
	.major		= WPCM450_SERIAL_MAJOR,
	.minor		= WPCM450_SERIAL_MINOR,
};

static struct wpcm450_uart_port wpcm450_serial_ports[NR_PORTS] = {
	[0] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART0,
			.uartclk	= 24000000,
			.fifosize	= 14,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 0,
		}
	},
	[1] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
#ifndef __iDRAC__			
		    .irq		= IRQ_UART1,
			.uartclk	= 24000000,
			.fifosize	= 14,
#endif				
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 1,
		}
	},
	
#ifdef __iDRAC__
	[2] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 2,
		}
	},
	[3] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART1,
			.uartclk	= 24000000,
			.fifosize	= 14,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 3,
		}
	},
	[4] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART1,
			.uartclk	= 24000000,
			.fifosize	= 14,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 4,
		}
	},
	[5] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 5,
		}
	},
	[6] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.uartclk	= 24000000,
			.irq		= IRQ_UART1,
			.fifosize	= 14,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 6,
		}
	}
#endif
};

/* _wpcm450_serial_resetport
 *
 * wrapper to call the specific reset for this port (reset the fifos
 * and the settings)
*/

static inline int _wpcm450_serial_resetport(struct uart_port * port,
					   struct wpcm450_uartcfg *cfg)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

	return (info->reset_port)(port, cfg);
}

/* wpcm450_serial_init_port
 *
 * initialise a single serial port from the platform device given
 */

static int wpcm450_serial_init_port(struct wpcm450_uart_port *ourport,
				    struct wpcm450_uart_info *info,
				    struct platform_device *platdev)
{
	struct uart_port *port = &ourport->port;
	struct wpcm450_uartcfg *cfg;
	struct resource *res;
	

	dbg("wpcm450_serial_init_port: port=%p, platdev=%p\n", port, platdev);

	if (platdev == NULL)
		return -ENODEV;
		
	cfg = wpcm450_dev_to_cfg(&platdev->dev);

	if (port->mapbase != 0)
		return 0;

	/* setup info for port */
	port->dev	= &platdev->dev;
	ourport->info	= info;

	/* copy the info in from provided structure */
	ourport->port.fifosize = info->fifosize;

	dbg("wpcm450_serial_init_port: %p (hw %d)...\n", port, cfg->hwport);

	port->uartclk = 24000000;
	
	/* sort our the physical and virtual addresses for each UART */

	res = platform_get_resource(platdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		printk(KERN_ERR "failed to find memory resource for uart\n");
		return -EINVAL;
	}

	dbg("resource %p (%lx..%lx)\n", res, res->start, res->end);

	port->mapbase	= res->start;
	port->membase	= WPCM450_VA_UART + (res->start - WPCM450_PA_UART);
	port->irq	= platform_get_irq(platdev, 0);
	if (port->irq < 0)
		port->irq = 0;

	ourport->clk	= clk_get(&platdev->dev, "uart");

	dbg("port: map=%08x, mem=%08x, irq=%d, clock=%ld\n",
	    port->mapbase, port->membase, port->irq, port->uartclk);

	/* reset the fifos (and setup the uart) */
	_wpcm450_serial_resetport(port, cfg);

	return 0;
}

/* Device driver serial port probe */

static int probe_index = 0;

static int _wpcm450_serial_probe(struct platform_device *dev,
				struct wpcm450_uart_info *info)
{
	struct wpcm450_uart_port *ourport;
	int ret;

	dbg("_wpcm450_serial_probe(%p, %p) %d\n", dev, info, probe_index);

	ourport = &wpcm450_serial_ports[probe_index];
	probe_index++;

	dbg("%s: initialising port %p...\n", __FUNCTION__, ourport);

	ret = wpcm450_serial_init_port(ourport, info, dev);
	if (ret < 0)
		goto probe_err;

	dbg("%s: adding port\n", __FUNCTION__);
	uart_add_one_port(&wpcm450_uart_drv, &ourport->port);
	platform_set_drvdata(dev, &ourport->port);
    ourport->irq_flag = 0;
	return 0;

 probe_err:
	return ret;
}

static int wpcm450_serial_remove(struct platform_device *dev)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);

	if (port)
		uart_remove_one_port(&wpcm450_uart_drv, port);

	return 0;
}

/* UART power management code */

#ifdef CONFIG_PM

static int wpcm450_serial_suspend(struct platform_device *dev, pm_message_t state)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);

	if (port)
		uart_suspend_port(&wpcm450_uart_drv, port);

	return 0;
}

static int wpcm450_serial_resume(struct platform_device *dev)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);
	struct wpcm450_uart_port *ourport = to_ourport(port);

	if (port) {
		clk_enable(ourport->clk);
		_wpcm450_serial_resetport(port, wpcm450_port_to_cfg(port));
		clk_disable(ourport->clk);

		uart_resume_port(&wpcm450_uart_drv, port);
	}

	return 0;
}

#else
#define wpcm450_serial_suspend NULL
#define wpcm450_serial_resume  NULL
#endif

static int _wpcm450_serial_init(struct platform_driver *drv,
			       struct wpcm450_uart_info *info)
{
	dbg("_wpcm450_serial_init(%p,%p)\n", drv, info);

	return platform_driver_register(drv);
}


/* now comes the code to initialise either the wpcm450 serial
 * port information
*/

static int wpcm450_serial_resetport(struct uart_port *port,
				    struct wpcm450_uartcfg *cfg)
{
	dbg("wpcm450_serial_resetport: port=%p (%08lx), cfg=%p\n",
	    port, port->mapbase, cfg);
	
	/* reset both fifos */
	if((port->line == 0) ||(port->line == 1))
		wr_regb (port, WPCM450_COM_FCR, 0x0F | UARTx_FCR_FIFO_LEVEL8); /* reset Tx and Rx FIFO */

	return 0;
}

static struct wpcm450_uart_info wpcm450_uart_inf = {
	.name		= "Winbond WPCM450 UART",
	.type		= PORT_WPCM450,
	.fifosize	= 16,
	.reset_port	= wpcm450_serial_resetport,
};

/* device management */

static int wpcm450_serial_probe(struct platform_device *dev)
{
	return _wpcm450_serial_probe(dev, &wpcm450_uart_inf);
}

static struct platform_driver wpcm450_serial_drv = {
	.probe		= wpcm450_serial_probe,
	.remove		= wpcm450_serial_remove,
	.suspend	= wpcm450_serial_suspend,
	.resume		= wpcm450_serial_resume,
	.driver		= {
		.name	= "wpcm450-uart",
		.owner	= THIS_MODULE,
	},
};

static inline int wpcm450_serial_init(void)
{
	return _wpcm450_serial_init(&wpcm450_serial_drv, &wpcm450_uart_inf);
}

static inline void wpcm450_serial_exit(void)
{
	platform_driver_unregister(&wpcm450_serial_drv);
}

#define wpcm450_uart_inf_at &wpcm450_uart_inf


/* module initialisation code */

static int __init wpcm450_serial_modinit(void)
{
	int ret;
	ret = uart_register_driver(&wpcm450_uart_drv);
	if (ret < 0) {
		printk(KERN_ERR "failed to register UART driver\n");
		return -1;
	}

	wpcm450_serial_init();

	return 0;
}

static void __exit wpcm450_serial_modexit(void)
{
	wpcm450_serial_exit();

	uart_unregister_driver(&wpcm450_uart_drv);
}


module_init(wpcm450_serial_modinit);
module_exit(wpcm450_serial_modexit);

/* Console code */

#ifdef CONFIG_SERIAL_WPCM450_CONSOLE

static struct uart_port *cons_uart;

static int
wpcm450_serial_console_txrdy(struct uart_port *port, unsigned int ufcon)
{
	unsigned long lsrstat;

	lsrstat = rd_regb(port, WPCM450_COM_LSR);
	return (lsrstat & UART_LSR_THRE) ? 1 : 0;
}

static void
wpcm450_serial_console_putchar(struct uart_port *port, int ch)
{
#if 1//clyu for emulate
	while (!wpcm450_serial_console_txrdy(port, 0))
		barrier();
#endif
	wr_regb(cons_uart, WPCM450_COM_TX, ch);
}

static void
wpcm450_serial_console_write(struct console *co, const char *s,
			     unsigned int count)
{
	uart_console_write(cons_uart, s, count, wpcm450_serial_console_putchar);
}

static void __init
wpcm450_serial_get_options(struct uart_port *port, int *baud,
			   int *parity, int *bits)
{
	

	unsigned int lcrcon,div;
	unsigned long rate;


	lcrcon = rd_regb(port, WPCM450_COM_LCR);
 	/* consider the serial port configured if the tx/rx mode set */

		switch (lcrcon & UART_LCR_CSMASK) {
		case UART_LCR_WLEN5:
			*bits = 5;
			break;
		case UART_LCR_WLEN6:
			*bits = 6;
			break;
		case UART_LCR_WLEN7:
			*bits = 7;
			break;
		default:
		case UART_LCR_WLEN8:
			*bits = 8;
			break;
		}
		
		if(lcrcon & UART_LCR_PARITY)
		{
			switch (lcrcon & UART_LCR_PMMASK) {
			case UART_LCR_EPAR:
				*parity = 'e';
				break;
			case UART_LCR_OPAR:
				*parity = 'o';
				break;
			//case UART_LCR_NPAR:
			default:
				*parity = 'n';
			}
		}
		else
			*parity = 'n';

		/* now calculate the baud rate */
		
		wr_regb (port, WPCM450_COM_LCR, 0x80); /* select divisor latch registers */    
	    div = ((rd_regb(port, WPCM450_COM_MSB)<<8)&0xff00) | (rd_regb(port, WPCM450_COM_LSB)&0xff);
		rate = port->uartclk;
		*baud = rate / ( 16 * (div + 2));
	    wr_regb(port, WPCM450_COM_LCR, lcrcon);
		
		


		dbg("calculated baud %d\n", *baud);
	

}

/* wpcm450_serial_init_ports
 *
 * initialise the serial ports from the machine provided initialisation
 * data.
*/

static int wpcm450_serial_init_ports(struct wpcm450_uart_info *info)
{
	struct wpcm450_uart_port *ptr = wpcm450_serial_ports;
	struct platform_device **platdev_ptr;
	int i;

	dbg("wpcm450_serial_init_ports: initialising ports...\n");

	platdev_ptr = wpcm450_uart_devs;

	for (i = 0; i < NR_PORTS; i++, ptr++, platdev_ptr++) {
		wpcm450_serial_init_port(ptr, info, *platdev_ptr);
	}

	return 0;
}

static int __init
wpcm450_serial_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 9600;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	dbg("wpcm450_serial_console_setup: co=%p (%d), %s\n",
	    co, co->index, options);

	/* is this a valid port */

	if (co->index == -1 || co->index >= NR_PORTS)
		co->index = 0;

	port = &wpcm450_serial_ports[co->index].port;

	/* is the port configured? */

	if (port->mapbase == 0x0) {
		co->index = 0;
		port = &wpcm450_serial_ports[co->index].port;
	}

	cons_uart = port;

	dbg("wpcm450_serial_console_setup: port=%p (%d)\n", port, co->index);

	/*
	 * Check whether an invalid uart number has been specified, and
	 * if so, search for the first available port that does have
	 * console support.
	 */
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	else
		wpcm450_serial_get_options(port, &baud, &parity, &bits);

	dbg("wpcm450_serial_console_setup: baud %d\n", baud);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

/* wpcm450_serial_initconsole
 *
 * initialise the console from one of the uart drivers
*/

static struct console wpcm450_serial_console =
{
	.name		= WPCM450_SERIAL_NAME,
	.device		= uart_console_device,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.write		= wpcm450_serial_console_write,
	.setup		= wpcm450_serial_console_setup
};

static int wpcm450_serial_initconsole(void)
{
	struct wpcm450_uart_info *info;
	struct platform_device *dev = wpcm450_uart_devs[0];

	dbg("wpcm450_serial_initconsole\n");

	/* select driver based on the cpu */

	if (dev == NULL) {
		printk(KERN_ERR "wpcm450: no devices for console init\n");
		return 0;
	}

	if (strcmp(dev->name, "wpcm450-uart") == 0) {
		info = wpcm450_uart_inf_at;
	} else {
		printk(KERN_ERR "wpcm450: no driver for %s\n", dev->name);
		return 0;
	}

	if (info == NULL) {
		printk(KERN_ERR "wpcm450: no driver for console\n");
		return 0;
	}

	wpcm450_serial_console.data = &wpcm450_uart_drv;
	wpcm450_serial_init_ports(info);

	register_console(&wpcm450_serial_console);
	return 0;
}

console_initcall(wpcm450_serial_initconsole);

#endif /* CONFIG_SERIAL_WPCM450_CONSOLE */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("Samsung WPCM450/S3C2440 Serial port driver");
