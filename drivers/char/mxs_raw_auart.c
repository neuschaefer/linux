/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
* Raw UART driver for HomeMatic CCU2 (Freescale MXS platform)
*
* Copyright (c) 2015 by eQ-3 Entwicklung GmbH
* Author: Lars Reemts, lars.reemts@finalbit.de
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
*/
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/sysrq.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/circ_buf.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <asm/termbits.h>
#include <asm/termios.h>
#include <asm/ioctls.h>
#include <mach/hi_hsan.h>
#include <asm/cacheflush.h>

/*
* Ioctl definitions
*/

/* Use 'u' as magic number */
#define MXS_AUART_IOC_MAGIC  'u'

/*
* S means "Set" through a ptr,
* G means "Get": reply by setting through a pointer
*/

/* Set and get the priority for the current channel */
#define MXS_AUART_IOCSPRIORITY _IOW(MXS_AUART_IOC_MAGIC,  1, unsigned long)
#define MXS_AUART_IOCGPRIORITY _IOR(MXS_AUART_IOC_MAGIC,  2, unsigned long)
#define MXS_AUART_IOC_MAXNR 2
#define MXS_AUART_PORTS 5
#define CIRCBUF_SIZE 1024
#define BAUD    115200
#define MXS_AUART_NUM 1
#define MODNAME "mxs-raw-auart"
/* enable debuging via proc filesystem */
#define PROC_DEBUG
//#define MXS_RX_TX_DATA_DEBUG

/* number of charaters to be written to the TX FIFO at once. */
#define TX_CHUNK_SIZE 8
#define BREAK_IND    (1 << 4)
#define PARITY_ERR   (1 << 2)
#define FRAME_ERR    (1 << 3)
#define OVERFLOW_IND (1 << 1)

struct per_connection_data;

/* Information about a single hardware UART */
struct mxs_auart_port {
	unsigned long mapbase;							//physical address of UART registers
	unsigned long irq;								//interrupt number
	void __iomem* membase;							//logical address of UART registers
	int open_count;									//number of open connections

	struct circ_buf rxbuf;							//RX buffer
	struct per_connection_data* tx_connection;		//connection which is currently sending
	spinlock_t lock_tx;								//TX lock for accessing tx_connection

	struct semaphore sem;							//semaphore for accessing this struct
	wait_queue_head_t readq;						//wait queue for read operations
	wait_queue_head_t writeq;						//wait queue for write operations
	
	struct clk *clk;								//System clock assigned to the UART device
	struct device *dev;								//System device
	
	dev_t devnode;									//Major/minor of /dev entry
	struct cdev cdev;								//character device structure
	struct class * class;							//driver class
	
	struct termios termios;							//dummy termios for emulating tty ioctls
	
	int count_tx;									//Statistic counter: Number of bytes transmitted
	int count_rx;									//Statistic counter: Number of bytes received
	int count_brk;									//Statistic counter: Number of break conditions received
	int count_parity;								//Statistic counter: Number of parity errors
	int count_frame;								//Statistic counter: Number of frame errors
	int count_overrun;								//Statistic counter: Number of RX overruns in hardware FIFO
	int count_buf_overrun;							//Statistic counter: Number of RX overruns in user space buffer
};

/* Information about a single connection from user space */
struct per_connection_data
{
	unsigned char txbuf[SZ_4K];
	size_t tx_buf_length;					//length of tx frame transmitted from userspace
	size_t tx_buf_index;					//index into txbuf
	
	struct mxs_auart_port* port;			//corresponding port
	
	unsigned long priority;					//priority of the corresponding channel

	struct semaphore sem;					//semaphore for accessing this struct. 
};

/* Array of port information for ports handled by this driver */
static struct mxs_auart_port *auart_port[MXS_AUART_PORTS];

static void mxs_raw_auart_start_tx(struct mxs_auart_port *port);
static struct mxs_auart_port* mxs_raw_auart_port_by_devnode( dev_t devnode );
static spinlock_t mxs_lock = SPIN_LOCK_UNLOCKED;

/* read() function. */
static ssize_t mxs_raw_auart_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{
	struct per_connection_data *conn = filp->private_data;
	struct mxs_auart_port *port = conn->port;

	if (down_interruptible(&port->sem))
	return -ERESTARTSYS;
	
	while(!CIRC_CNT( port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE)) { /* nothing to read */
		up(&port->sem); /* release the lock */
		if (filp->f_flags & O_NONBLOCK)
		return -EAGAIN;
		if (wait_event_interruptible(port->readq, CIRC_CNT( port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE)))
		return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
		/* otherwise loop, but first reacquire the lock */
		if (down_interruptible(&port->sem))
		return -ERESTARTSYS;
	}
	/* ok, data is there, return something */
	count = min((int)count, CIRC_CNT_TO_END( port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE));
	if (copy_to_user(buf, port->rxbuf.buf+port->rxbuf.tail, count)) {
		up (&port->sem);
		return -EFAULT;
	}
	smp_mb();
	port->rxbuf.tail = (port->rxbuf.tail + count) & (CIRCBUF_SIZE - 1);
	up (&port->sem);

	return count;
}

/* try to become the current sender and start sending. Fails if a higher priority send is in progress */
static int acquire_sender( struct per_connection_data *conn )
{
	int retval = 0;
	struct mxs_auart_port *port = conn->port;
	unsigned long lock_flags;
	int sender_idle;
	spin_lock_irqsave(&port->lock_tx, lock_flags);
	sender_idle = port->tx_connection == NULL;
	if( (port->tx_connection == NULL) || (port->tx_connection->priority < conn->priority) )
	{
		port->tx_connection = conn;
		retval = 1;
		if( sender_idle )
		{
			mxs_raw_auart_start_tx( port );
		}else{
			wake_up_interruptible( &port->writeq );
		}
	}
	spin_unlock_irqrestore(&port->lock_tx, lock_flags);
	return retval;
}

/* check if sendig by the given connection was completed. Sendig was completed if the sending connection has changed */
static int send_completed( struct per_connection_data *conn )
{
	int retval = 0;
	struct mxs_auart_port *port = conn->port;
	unsigned long lock_flags;
	spin_lock_irqsave(&port->lock_tx, lock_flags);
	retval = port->tx_connection != conn;
	spin_unlock_irqrestore(&port->lock_tx, lock_flags);
	return retval;
}

/* write() function. Waits until count bytes have been transferred to the TX FIFO. May return less than count if send was interrupted by a higher priority write operation */
static ssize_t mxs_raw_auart_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
	struct per_connection_data *conn = filp->private_data;
	struct mxs_auart_port *port = conn->port;	
	int retval=0;
	int conn_sem_aquired = 0;/* keep track of aquiring the connection semaphore in order to properly release it */

	if (down_interruptible(&conn->sem))
	{
		retval = -ERESTARTSYS;
		goto exit;
	}
	conn_sem_aquired = 1;
	
	if( count > sizeof(conn->txbuf)  )
	{
		retval =  -EMSGSIZE;
		goto exit;
	}
	
	if(copy_from_user(conn->txbuf, buf, count)){
		retval=-EFAULT;
		goto exit;
	}
	
	conn->tx_buf_index = 0;
	conn->tx_buf_length = count;
	smp_wmb();
	
	if (wait_event_interruptible(port->writeq, acquire_sender( conn ) )){
		retval=-ERESTARTSYS;
		goto exit;
	}
	
	//wait for sending to complete
	if (wait_event_interruptible(port->writeq, send_completed( conn ) )){
		retval=-ERESTARTSYS;
		goto exit;
	}
	
	//return number of characters actually sent
	retval = conn->tx_buf_index;
	
exit:
	if(conn_sem_aquired)up(&conn->sem);
	return retval;    
}

static hi_void hi_uart_stop_tx(struct mxs_auart_port *pst_port)
{
    unsigned int ui_val = 0;

    HI_REG_READ(pst_port->membase + HI_UART_IER, ui_val);
    ui_val &= ~(HI_PTIME|HI_ETBEI);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, ui_val);
}

/* Transfer the next chunk of characters from the sending connection to the TX FIFO */
static inline void mxs_raw_auart_tx_chars(struct mxs_auart_port *port)
{
	int tx_count = 0;
    unsigned int ui_val   = 0;     
#ifdef MXS_RX_TX_DATA_DEBUG      
    unsigned char  buff[TX_CHUNK_SIZE] = {0};
#endif
    unsigned char   index = 0;
    
    do 
    {
        HI_REG_READ(port->membase + HI_UART_LSR, ui_val);    
    }while ( (ui_val&0x20) != 0x20 );
    
	while( (tx_count < TX_CHUNK_SIZE) && (port->tx_connection != NULL) && (port->tx_connection->tx_buf_index < port->tx_connection->tx_buf_length) )
	{
		writeb(port->tx_connection->txbuf[port->tx_connection->tx_buf_index], port->membase + HI_UART_THR);
  #ifdef MXS_RX_TX_DATA_DEBUG      
        buff[index++]=port->tx_connection->txbuf[port->tx_connection->tx_buf_index];
  #endif
		port->tx_connection->tx_buf_index++;        
        port->count_tx++;
		smp_wmb();
		tx_count++;
	}
	if( (port->tx_connection != NULL) && (port->tx_connection->tx_buf_index >= port->tx_connection->tx_buf_length) )
	{
		hi_uart_stop_tx(port);
		port->tx_connection = NULL;
		smp_wmb();
		wake_up_interruptible( &port->writeq );
	}

#ifdef MXS_RX_TX_DATA_DEBUG
    if (0 != tx_count)
    {        
        printk("func[%s]: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\r\n", __func__, buff[0],
            buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);
    }  
#endif
}

/* Read all available characters from the hardware RX FIFO and store them in the port's circular RX buffer. Notify userspace. */
static void mxs_raw_auart_rx_chars(struct mxs_auart_port *port)
{
	unsigned int stat = 0;
    unsigned int ui_max_count = 256;
    unsigned char c = 0;

    HI_REG_READ(port->membase + HI_UART_LSR, stat);
    if(!(stat & HI_DR))
    {
        HI_REG_READ(port->membase + HI_UART_RBR, c);
    }

    while ((stat & HI_DR) && ui_max_count--)
    {
        HI_REG_READ(port->membase + HI_UART_RBR, c);
        if (stat & BREAK_IND) {
            port->count_brk++;
            HI_REG_READ(port->membase + HI_UART_LSR, stat);
            continue;
        } else if (stat & PARITY_ERR) {
            port->count_parity++;
        } else if (stat & FRAME_ERR) {
            port->count_frame++;
        }
        
        if (stat & OVERFLOW_IND)
        port->count_overrun++;
        
        if( CIRC_SPACE( port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE ) )
        {
            port->rxbuf.buf[port->rxbuf.head] = c;
            smp_wmb();
            port->rxbuf.head = (port->rxbuf.head + 1) & (CIRCBUF_SIZE - 1);
        } 
        
#ifdef MXS_RX_TX_DATA_DEBUG
        printk("mxs_raw_auart_rx_char[0x%x] \r\n",c); 
#endif
        HI_REG_READ(port->membase + HI_UART_LSR, stat);
    }

	wake_up_interruptible( &port->readq );
}

static irqreturn_t hi_uart_intr(hi_int32 i_irq, hi_void *pv_dev_id)
{
    int i_handled       = 0;
    unsigned int ui_int_id       = 0;
    unsigned int ui_status_usr   = 0;
    unsigned int ui_status       = 0;
    unsigned int ui_pass_counter = HI_ISR_PASS_LMT;
    struct mxs_auart_port *pst_port        = (struct mxs_auart_port*)pv_dev_id;
   
    spin_lock(&mxs_lock);
    HI_REG_READ(pst_port->membase + HI_UART_IIR, ui_status);
    ui_int_id = 0x0F&ui_status;
    
    while (ui_int_id != HI_NOINTERRUPT)
    {
        if ((HI_RECEIVEAVA == ui_int_id) || (HI_RECTIMEOUT == ui_int_id) || (HI_RECEIVERR == ui_int_id))
        {
            mxs_raw_auart_rx_chars(pst_port);
        }else if (HI_MODEMSTA == ui_int_id){
            ;
        }
        else if(HI_TRANSEMP == ui_int_id)
        {
            mxs_raw_auart_tx_chars(pst_port);
        }
        else
        {
            HI_REG_READ(pst_port->membase + HI_UART_USR, ui_status_usr);
        }
        if (ui_pass_counter-- == 0)
        {
            break;
        }
        HI_REG_READ(pst_port->membase + HI_UART_IIR, ui_status);
        ui_int_id = 0x0F&ui_status;
        i_handled = 1;
    }   
    spin_unlock(&mxs_lock);
    return IRQ_RETVAL(i_handled);
}

/* Configure and enable the UART hardware */
static int mxs_raw_auart_startup(struct mxs_auart_port *pst_port)
{
    unsigned int  ui_lcr_h  = 0;
    unsigned int  ui_old_cr = 0;
    unsigned int  ui_quot   = 0;
    unsigned long ul_flags  = 0;     
    int  i_ret  = 0;
    unsigned int ui_val = 0;

    /* disable FIFO and interrupt */
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, 0 );
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0 );
    HI_REG_READ(pst_port->membase + HI_UART_USR, ui_val);
    
    /* Allocate the IRQ */
    i_ret = request_irq(pst_port->irq, hi_uart_intr, 0, dev_name(pst_port->dev), pst_port);
    if (0 != i_ret)
    {
        printk(KERN_INFO "mxs_raw_auart_startup: request_irq fail\n");
        return i_ret;
    }

    /* enable FIFO */
    spin_lock_irq(&mxs_lock);
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0xb1);
    HI_REG_READ(pst_port->membase + HI_UART_LSR, ui_val);    
    HI_REG_READ(pst_port->membase + HI_UART_RBR, ui_val); /* read to clr IIR */
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, HI_ERBFI|HI_ELSI);
    spin_unlock_irq(&mxs_lock);
    
    /* calculate the divisor */
    ui_quot = CONFIG_HSAN_BUSCLK / (16 * BAUD);    
    /* 8 data bits, no parity, 1bit stop */
    ui_lcr_h = HI_UART_8bit; 

    spin_lock_irqsave(&mxs_lock, ul_flags);    
    /* Enable DLL and DLH */
    HI_REG_READ(pst_port->membase + HI_UART_LCR, ui_old_cr);
    ui_old_cr |= HI_UART_DLAB;
    HI_REG_WRITE(pst_port->membase + HI_UART_LCR, ui_old_cr);

    /* Set baud rate */
    HI_REG_WRITE(pst_port->membase + HI_UART_DLH, ((ui_quot&0xFF00)>>8));
    HI_REG_WRITE(pst_port->membase + HI_UART_DLL, (ui_quot & 0xFF));

    ui_old_cr &= ~HI_UART_DLAB;
    HI_REG_WRITE(pst_port->membase + HI_UART_LCR, ui_old_cr);
    HI_REG_WRITE(pst_port->membase + HI_UART_LCR, ui_lcr_h);
    spin_unlock_irqrestore(&mxs_lock, ul_flags);

    printk(KERN_INFO "mxs_raw_auart_startup: ok\n");
    return i_ret;
}

/* Disable the UART hardware */
static void mxs_raw_auart_shutdown(struct mxs_auart_port *pst_port)
{
    unsigned int ui_val = 0;

    /* disable/clear all interrupts */
    spin_lock_irq(&mxs_lock);
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, 0 );
    HI_REG_READ(pst_port->membase + HI_UART_LSR, ui_val);
    spin_unlock_irq(&mxs_lock);

    /* Free the interrupt */
    free_irq(pst_port->irq, pst_port);
    
    /* disable break condition and fifos */
    do {
        HI_REG_READ(pst_port->membase + HI_UART_USR, ui_val);
    }while( ui_val&HI_UART_BUSY );
    
    HI_REG_READ(pst_port->membase + HI_UART_LCR, ui_val);    
    ui_val &= ~(HI_UART_BREAK);
    HI_REG_WRITE(pst_port->membase + HI_UART_LCR, ui_val);
    /* disable fifo*/
    HI_REG_WRITE(pst_port->membase + HI_UART_FCR, 0);
}

/* Start sending */
static void mxs_raw_auart_start_tx(struct mxs_auart_port *pst_port)
{
    unsigned int ui_val = 0;

    mxs_raw_auart_tx_chars(pst_port);

    HI_REG_READ(pst_port->membase + HI_UART_IER, ui_val);
    ui_val |= HI_ETBEI;
    HI_REG_WRITE(pst_port->membase + HI_UART_IER, ui_val);	
}

/* Get the port structure for a given device node (major/minor) */
static struct mxs_auart_port * mxs_raw_auart_port_by_devnode( dev_t devnode )
{
	int i;
	for( i = 0; i < MXS_AUART_PORTS; i++ )
	{
		if( auart_port[i] && auart_port[i]->devnode == devnode )
		{
			return auart_port[i];
		}
	}
	return NULL;
}


/* open(). Attach to UART port identified by devnode. If necessary, perform initialization and enable port for reception */
static int mxs_raw_auart_open(struct inode *inode, struct file *filp)
{
	int retval;
	struct mxs_auart_port *port = mxs_raw_auart_port_by_devnode(inode->i_rdev);
	struct per_connection_data* conn;
	
	if( !port )
	{
		return -ENODEV;
	}
	
	if (down_interruptible(&port->sem)){
		return -ERESTARTSYS;
	}
	
	if( !port->open_count ){
		port->rxbuf.head = port->rxbuf.tail = 0;
		retval = mxs_raw_auart_startup( port );
		if(retval){
			up(&port->sem);
			return retval;
		}
		init_waitqueue_head(&port->writeq);
		init_waitqueue_head(&port->readq);
	}
	port->open_count++;

	up(&port->sem);

	conn = kmalloc( sizeof( struct per_connection_data ), GFP_KERNEL );
	memset( conn, 0, sizeof( struct per_connection_data ) );
	conn->port = port;

	sema_init(&conn->sem, 1);

	filp->private_data = (void *)conn;
	
	return 0;
}

/* close(). Remove conection. If necessary, disable UART port */
static int mxs_raw_auart_close(struct inode *inode, struct file *filp)
{
	struct per_connection_data *conn = filp->private_data;
	struct mxs_auart_port *port = conn->port;

	if (down_interruptible(&conn->sem)){
		return -ERESTARTSYS;
	}

	kfree(conn);
	
	if (down_interruptible(&port->sem)){
		return -ERESTARTSYS;
	}
	
	port->open_count--;
	
	if(!port->open_count){
		mxs_raw_auart_shutdown( port);
	}
	
	up(&port->sem);
	
	return 0;
}

/* poll(). Called from userspace to request notification if port becomes readable or writeable. */
static unsigned int mxs_raw_auart_poll(struct file* filp, poll_table* wait)
{
	unsigned int mask=0;
	struct per_connection_data *conn = filp->private_data;
	struct mxs_auart_port *port = conn->port;
	unsigned long lock_flags;
	
	poll_wait(filp, &port->readq, wait);
	poll_wait(filp, &port->writeq, wait);
	
	spin_lock_irqsave(&port->lock_tx, lock_flags);
	if( (port->tx_connection == NULL) || (port->tx_connection->priority < conn->priority) )
	{
		mask |= POLLOUT | POLLWRNORM;
	}
	spin_unlock_irqrestore(&port->lock_tx, lock_flags);
	
	if( CIRC_CNT(port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE) > 0 ) mask |= POLLIN | POLLRDNORM;
	return mask;
}

/* ioctl(). Called from userspace to set the connection priority. Emulates a subset of TTY ioctls used by some userspace tools. */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,11))
static int mxs_raw_auart_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
#else
static long mxs_raw_auart_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
#endif
{
	struct per_connection_data *conn = filp->private_data;
	struct mxs_auart_port *port = conn->port;
	int err = 0;
	int ret = 0;
	unsigned long temp;
	
	if (_IOC_TYPE(cmd) == MXS_AUART_IOC_MAGIC)
	{
		/*
		* extract the type and number bitfields, and don't decode
		* wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
		*/
		if (_IOC_NR(cmd) > MXS_AUART_IOC_MAXNR) return -ENOTTY;

		/*
		* the direction is a bitmask, and VERIFY_WRITE catches R/W
		* transfers. `Type' is user-oriented, while
		* access_ok is kernel-oriented, so the concept of "read" and
		* "write" is reversed
		*/
		if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE, (void *)arg, _IOC_SIZE(cmd));
		else if (_IOC_DIR(cmd) & _IOC_WRITE)
		err =  !access_ok(VERIFY_READ, (void *)arg, _IOC_SIZE(cmd));
		if (err) return -EFAULT;
	}

	if (down_interruptible(&conn->sem))
		return -ERESTARTSYS;
	
	switch(cmd) {

	/* Set connection priority */
	case MXS_AUART_IOCSPRIORITY: /* Set: arg points to the value */
		ret = __get_user(temp, (unsigned long *)arg);
		if(ret)break;
		conn->priority = temp;
		break;

	/* Get connection priority */
	case MXS_AUART_IOCGPRIORITY: /* Get: arg is pointer to result */
		ret = __put_user(conn->priority, (unsigned long *)arg);
		break;
		
	/* Emulated TTY ioctl: Get termios struct */
	case TCGETS:
		if( access_ok(VERIFY_READ, (void *)arg, sizeof(struct termios) ) )
		{
			if (down_interruptible(&port->sem))
			{
				ret = -ERESTARTSYS;
			}else{
				ret = copy_to_user( (void*)arg, &port->termios, sizeof(struct termios) );
				up(&port->sem);
			}
		} else {
			ret = -EFAULT;
		}
	break;
	
	/* Emulated TTY ioctl: Set termios struct */
	case TCSETS:
		if( access_ok(VERIFY_WRITE, (void *)arg, sizeof(struct termios) ) )
		{
			if (down_interruptible(&port->sem))
			{
				ret = -ERESTARTSYS;
			}else{
				ret = copy_from_user( &port->termios, (void*)arg, sizeof(struct termios) );
				up(&port->sem);
			}
		} else {
			ret = -EFAULT;
		}
		break;
		
	/* Emulated TTY ioctl: Get receive queue size */
	case TIOCINQ:
		if (down_interruptible(&port->sem))
		{
			ret = -ERESTARTSYS;
		}else{
			temp = CIRC_CNT( port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE);
			up(&port->sem);
			ret = __put_user( temp, (int*)arg );
		}
		break;
		
	/* Emulated TTY ioctl: Get send queue size */
	case TIOCOUTQ:
		temp = 0;
		ret = __put_user( temp, (int*)arg );
		break;
		
	/* Emulated TTY ioctl: Exclusive use */
	case TIOCEXCL:
		break;
		
	/* Emulated TTY ioctl: Flush */
	case TCFLSH:
		break;
		
	/* Emulated TTY ioctl: Get states of modem control lines */
	case TIOCMGET:
		temp = TIOCM_DSR | TIOCM_CD | TIOCM_CTS;
		ret = __put_user( temp, (int*)arg );
		break;
		
	/* Emulated TTY ioctl: Set states of modem control lines */
	case TIOCMSET:
		break;

	default:
		ret = -ENOTTY;
	}
	
	up(&conn->sem);
	return ret;

}



static struct file_operations mxs_raw_auart_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.read		= mxs_raw_auart_read,
	.write		= mxs_raw_auart_write,
	.open		= mxs_raw_auart_open,
	.release	= mxs_raw_auart_close,
	.poll       = mxs_raw_auart_poll,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,11))
	.ioctl      = mxs_raw_auart_ioctl,
#else
	.unlocked_ioctl = mxs_raw_auart_ioctl,
#endif
};

#ifdef PROC_DEBUG
/* Provides values of relevant hardware registers and statistical information via /proc filesystem. Useful for debugging */
static int mxs_raw_auart_read_procmem(char* buf, char** start, off_t offset, int count, int* eof, void* data)
{
	struct mxs_auart_port *port = data;
	int len = 0;
	len += snprintf(buf+len, count, "HI_UART_IER    =%08X\n", readl(port->membase + HI_UART_IER));
	len += snprintf(buf+len, count, "HI_UART_USR    =%08X\n", readl(port->membase + HI_UART_USR));
	len += snprintf(buf+len, count, "HI_UART_LCR =%08X\n", readl(port->membase + HI_UART_LCR));
	len += snprintf(buf+len, count, "HI_UART_IIR     =%08X\n", readl(port->membase + HI_UART_IIR));
	len += snprintf(buf+len, count, "HI_UART_LSR     =%08X\n", readl(port->membase + HI_UART_LSR));

	len += snprintf( buf+len, count, "open_count=%d\n", port->open_count);
	len += snprintf( buf+len, count, "count_tx=%d\n", port->count_tx);
	len += snprintf( buf+len, count, "count_rx=%d\n", port->count_rx);
	len += snprintf( buf+len, count, "count_brk=%d\n", port->count_brk);
	len += snprintf( buf+len, count, "count_parity=%d\n", port->count_parity);
	len += snprintf( buf+len, count, "count_frame=%d\n", port->count_frame);
	len += snprintf( buf+len, count, "count_overrun=%d\n", port->count_overrun);
	len += snprintf( buf+len, count, "rxbuf_size=%d\n", CIRC_CNT(port->rxbuf.head, port->rxbuf.tail, CIRCBUF_SIZE));
	len += snprintf( buf+len, count, "rxbuf_head=%d\n", port->rxbuf.head);
	len += snprintf( buf+len, count, "rxbuf_tail=%d\n", port->rxbuf.tail);
	
	*eof = 1;
	return len;
}
#endif

/* Exit function called once for every UART port handled by this driver. */
static int __devexit mxs_raw_auart_remove(struct platform_device *pdev)
{
	struct mxs_auart_port *port = platform_get_drvdata(pdev);
	
#ifdef PROC_DEBUG
	{
		char proc_name[32];
		snprintf(proc_name, sizeof(proc_name), "mxs_auart_raw.%d", pdev->id );
		remove_proc_entry(proc_name, NULL);
	}
#endif

    if (NULL == port)
    {
        printk("platform_get_drvdata is fail.\n");
        return -1;
    }
    
	device_destroy(port->class, MKDEV(MAJOR(port->devnode), MINOR(port->devnode)));
	class_destroy(port->class);
	cdev_del(&port->cdev);
	unregister_chrdev_region(port->devnode, 1);
	

	auart_port[pdev->id] = NULL;
	kfree(port->rxbuf.buf);	
	kfree(port);

	return 0;
}

/* Probe function called once for every UART port handled by this driver */
static int __devinit mxs_raw_auart_probe(struct platform_device *pdev)
{
	struct mxs_auart_port *port;
	int ret = 0;
	char dev_name[32];

	port = kzalloc(sizeof(struct mxs_auart_port), GFP_KERNEL);
	if (!port) {
		ret = -ENOMEM;
		goto out;
	}
	snprintf(dev_name, sizeof(dev_name), "mxs_auart_raw.%d", pdev->id);
	
	ret = alloc_chrdev_region(&port->devnode, 0, 1, dev_name);
	if( ret )
	{
		printk(KERN_ERR "mxs_auart_raw: Unable to get device number region\n");
		goto out_free;
	}
	cdev_init(&port->cdev, &mxs_raw_auart_fops);
	port->cdev.owner=THIS_MODULE;
	port->cdev.ops=&mxs_raw_auart_fops;
	ret=cdev_add(&port->cdev, port->devnode, 1);
	if(ret){
		printk(KERN_ERR "mxs_auart_raw: Unable to add driver\n");
		goto out_unregister_chrdev_region;
	}
	port->class=class_create(THIS_MODULE, dev_name);
	if(IS_ERR(port->class)){
		ret = -EIO;
		printk(KERN_ERR "mxs_auart_raw: Unable to register driver class\n");
		goto out_cdev_del;
	}
	
	device_create(port->class, NULL, MKDEV(MAJOR(port->devnode), MINOR(port->devnode)), "%s", dev_name);
	
    port->mapbase = HI_REG_BASE_UART1;
    port->membase = (hi_void*)hi_io_address(HI_REG_BASE_UART1);
	port->dev = get_device(&pdev->dev);
    port->irq = HI_INTNR_UART1;
    
	sema_init(&port->sem, 1);
	spin_lock_init(&port->lock_tx);
	init_waitqueue_head(&port->readq);
	init_waitqueue_head(&port->writeq);
	
	port->rxbuf.buf = kmalloc(CIRCBUF_SIZE, GFP_KERNEL);	
	platform_set_drvdata(pdev, port);
	auart_port[pdev->id] = port;
#ifdef PROC_DEBUG
	{
		create_proc_read_entry(dev_name, 0, NULL, mxs_raw_auart_read_procmem, port);
	}
#endif
    printk(KERN_INFO "mxs_raw_auart_probe: ok\n");
	return 0;	

out_cdev_del:
	cdev_del(&port->cdev);
out_unregister_chrdev_region:
	unregister_chrdev_region(port->devnode, 1);
out_free:
	kfree(port);
out:
	return ret;
}

static struct platform_driver mxs_auart_driver = {
	.probe = mxs_raw_auart_probe,
	.remove = __devexit_p(mxs_raw_auart_remove),
	.driver = {
		.name = MODNAME,
		.owner = THIS_MODULE,
	},
};

static struct platform_device mxs_auart_device = {
    .name = MODNAME,
    .id = 0,
};

static struct platform_device *mxs_auart_devices[MXS_AUART_NUM] = {
    &mxs_auart_device,
};

/* Init funtion for this driver. */
static int __init mxs_raw_auart_init(void)
{
	int r;
	
	memset( auart_port, 0, sizeof(auart_port) );
    
	r = platform_driver_register(&mxs_auart_driver);
	if (r)
	{
        printk(KERN_ERR "register platform driver fail\n");
        goto out;
    }

    r = platform_add_devices(mxs_auart_devices, MXS_AUART_NUM);
    if(r)
    {
        printk(KERN_ERR "register platform device fail\n");
        goto out;
    }

	return 0;

out:
	return r;
}

/* Exit function for this driver */
static void __exit mxs_raw_auart_exit(void)
{
	platform_driver_unregister(&mxs_auart_driver);
    platform_device_unregister(&mxs_auart_device);
}

module_init(mxs_raw_auart_init);
module_exit(mxs_raw_auart_exit);
MODULE_DESCRIPTION("eQ-3 raw MXS application uart driver");
MODULE_LICENSE("GPL");
