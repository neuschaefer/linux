/*
 * linux/drivers/usb/gadget/pd12_udc.c
 * Taihu pd12-udc full speed USB device controllers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "pd12_udc.h"

/*#define DEBUG_PD12 printk*/
/*#define DEBUG_PD12_EP0 printk*/
/*#define DEBUG_PD12_SETUP printk*/

#ifndef DEBUG_PD12_EP0
# define DEBUG_PD12_EP0(fmt,args...)
#endif
#ifndef DEBUG_PD12_SETUP
# define DEBUG_PD12_SETUP(fmt,args...)
#endif
#ifndef DEBUG_PD12
# define NO_STATES
# define DEBUG_PD12(fmt,args...)
#endif

#define	DRIVER_DESC			"PD12 USB Device Controller"
#define	DRIVER_VERSION		__DATE__


static const char driver_name[] = "pd12_udc";
static const char driver_desc[] = DRIVER_DESC;
static const char ep0name[] = "ep0-control";
static void __iomem *th_pd12_virt;
static void __iomem *th_cpld_virt;
static u8 first_tran = 1;

#define DEV_CMD_ADDR  ((ulong)th_pd12_virt+1)
#define DEV_DATA_ADDR (th_pd12_virt)

#define CPLD_REG0_ADDR (th_cpld_virt)
#define CPLD_REG1_ADDR ((ulong)th_cpld_virt+1)
/*
  Local definintions.
*/

#ifndef NO_STATES
static char *state_names[] = {
	"WAIT_FOR_SETUP",
	"DATA_STATE_XMIT",
	"DATA_STATE_NEED_ZLP",
	"WAIT_FOR_OUT_STATUS",
	"DATA_STATE_RECV"
};
#endif

/*
  Local declarations.
*/
static int pd12_ep_enable(struct usb_ep *ep,
			     const struct usb_endpoint_descriptor *);
static int pd12_ep_disable(struct usb_ep *ep);
static struct usb_request *pd12_alloc_request(struct usb_ep *ep, unsigned);
static void pd12_free_request(struct usb_ep *ep, struct usb_request *);
static void *pd12_alloc_buffer(struct usb_ep *ep, unsigned, dma_addr_t *,
				  unsigned);
static void pd12_free_buffer(struct usb_ep *ep, void *, dma_addr_t,
				unsigned);
static int pd12_queue(struct usb_ep *ep, struct usb_request *, unsigned);
static int pd12_dequeue(struct usb_ep *ep, struct usb_request *);
static int pd12_set_halt(struct usb_ep *ep, int);
static void pd12_ep0_kick(struct pd12_udc *dev, struct pd12_ep *ep);
static void pd12_handle_ep0(struct pd12_udc *dev);

static void done(struct pd12_ep *ep, struct pd12_request *req,
		 int status);
static void stop_activity(struct pd12_udc *dev,
			  struct usb_gadget_driver *driver);
static void flush(struct pd12_ep *ep);
static void udc_enable(struct pd12_udc *dev);
static void udc_set_address(struct pd12_udc *dev, unsigned char address);

static struct usb_ep_ops pd12_ep_ops = {
	.enable = pd12_ep_enable,
	.disable = pd12_ep_disable,

	.alloc_request = pd12_alloc_request,
	.free_request = pd12_free_request,

	.alloc_buffer = pd12_alloc_buffer,
	.free_buffer = pd12_free_buffer,

	.queue = pd12_queue,
	.dequeue = pd12_dequeue,

	.set_halt = pd12_set_halt,
};


static __inline__ void read_data(volatile u8 *val)
{
	*val = *(volatile u8 *)DEV_DATA_ADDR;
	udelay(5);
}

static __inline__ void write_data(u8 val)
{
	*(volatile u8 *)DEV_DATA_ADDR = val;
	udelay(5);
}

static __inline__ void write_cmd(u8 val)
{
	*(volatile u8 *)DEV_CMD_ADDR = val;
	udelay(5);
}

static __inline__ void usb_set_index(u8 ep)
{
	if(ep != 0)
		ep += 1;
	write_cmd(ep);
}

static void pd12_set_ack(u8 index)
{
	
	write_cmd(index);
	write_cmd(PD12_ACK_SETUP);
	if(index  == 0)
		write_cmd(PD12_CLEAR_BUF);
		
}

static int write_fifo(struct pd12_ep *ep, struct pd12_request *req)
{
	u8 *buf;
	unsigned count;
	unsigned length;
	int is_last;
	u8 ep_sts;
	
	buf = req->req.buf + req->req.actual;
	prefetch(buf);
	
	count = ep->ep.maxpacket;	
	length = req->req.length - req->req.actual;
	length = min(length, count);
	req->req.actual += length;

	DEBUG_PD12("Write %d (max %d), fifo %p\n", length, count, buf);
	write_cmd(1);
	read_data(&ep_sts);
/*	write_cmd(PD12_ACK_SETUP); */
	write_cmd(PD12_WRITE_BUF);
	write_data(0x0);
	write_data(length);
	if(length == 0)
		write_data(0x0);
	while(length--)
		write_data(*buf++);
	write_cmd(PD12_VALIDATE_BUF);
	if(length != ep->ep.maxpacket)
		is_last = 1;
	else if(req->req.length == req->req.actual
			&& !req->req.zero)
		is_last = 1;
	else
		is_last = 0;

	if(is_last)
		done(ep,req,0);
	return is_last;

}
/*-------------------------------------------------------------------------*/

#ifdef CONFIG_USB_GADGET_DEBUG_PD12_FILES

static const char proc_node_name[] = "driver/udc";

static int
udc_proc_read(char *page, char **start, off_t off, int count,
	      int *eof, void *_dev)
{
	char *buf = page;
	struct pd12_udc *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int t;
	u8 ep_status[6];

	if (off != 0)
		return 0;

	local_irq_save(flags);

	/* basic device status */
	t = scnprintf(next, size,
		      DRIVER_DESC "\n"
		      "%s version: %s\n"
		      "Gadget driver: %s\n"
		      "Host: %s\n\n",
		      driver_name, DRIVER_VERSION,
		      dev->driver ? dev->driver->driver.name : "(none)");
	size -= t;
	next += t;

	for(i=0;i<PD12_MAX_ENDPOINTS;i++)
	{
		write_cmd(PD12_READ_LAST_STATUS+i);
		read_data(&ep_status[i]);
	}
	t = scnprintf(next, size,
		      "Endpoints last status:\n"
			  " ep0: 0x%x, ep1: 0x%x, ep2: 0x%x\n"
			  " ep3: 0x%x, ep4: 0x%x, ep5: 0x%x\n\n",
		      ep_status[0], ep_status[1], ep_status[2],
		      ep_status[3], ep_status[4], ep_status[5]
	    );
	size -= t;
	next += t;

	local_irq_restore(flags);
	*eof = 1;
	return count - size;
}

#define create_proc_files() 	create_proc_read_entry(proc_node_name, 0, NULL, udc_proc_read, dev)
#define remove_proc_files() 	remove_proc_entry(proc_node_name, NULL)

#else	/* !CONFIG_USB_GADGET_DEBUG_FILES */

#define create_proc_files() do {} while (0)
#define remove_proc_files() do {} while (0)

#endif	/* CONFIG_USB_GADGET_DEBUG_FILES */

static void pd12_set_mode(u8 val)
{
	write_cmd(PD12_SET_MODE);
	write_data(val);
	write_data(0x3);/*maybe 0x43*/
}
static void pd12_read_int(u16* val)
{
	write_cmd(PD12_READ_INT);
	read_data((u8*)val);
}

static void pd12_read_lstatus(u8 index, u8* val)
{
	write_cmd(PD12_READ_LAST_STATUS + index);
	read_data(val);
}
/*
 * 	udc_disable - disable USB device controller
 */
static void udc_disable(struct pd12_udc *dev)
{
	DEBUG_PD12("%s, %p\n", __FUNCTION__, dev);

	udc_set_address(dev, 0);
	pd12_set_mode(0x06); /*disconnect soft connect pullup resior */
	
	dev->ep0state = WAIT_FOR_SETUP;
	dev->gadget.speed = USB_SPEED_UNKNOWN;
	dev->usb_address = 0;
}


/*
 * 	udc_reinit - initialize software state
 */
static void udc_reinit(struct pd12_udc *dev)
{
	u8 i;
	u16 tmp;
	
	DEBUG_PD12("%s, %p\n", __FUNCTION__, dev);

	udc_set_address(dev, 0);
	pd12_set_mode(0x06); /*disconnect soft connect pullup resior */
	mdelay(1500);
	pd12_set_mode(0x16); /*soft connnect*/
	pd12_read_int(&tmp);
	for(i= 0; i< 5; i++)
		pd12_read_lstatus(i,(u8 *)(&tmp));

	/* device/ep0 records init */
	INIT_LIST_HEAD(&dev->gadget.ep_list);
	INIT_LIST_HEAD(&dev->gadget.ep0->ep_list);
	dev->ep0state = WAIT_FOR_SETUP;
	dev->gadget.speed = USB_SPEED_UNKNOWN;
	dev->usb_address = 0;

	/* basic endpoint records init */
	for (i = 0; i < PD12_MAX_ENDPOINTS; i++) {
		struct pd12_ep *ep = &dev->ep[i];

		if (i != 0)
			list_add_tail(&ep->ep.ep_list, &dev->gadget.ep_list);

		ep->desc = 0;
		ep->stopped = 0;
		INIT_LIST_HEAD(&ep->queue);
	}

	/* the rest was statically initialized, and is read-only */
}

/* until it's enabled, this UDC should be completely invisible
 * to any USB host.
 */
static void udc_enable(struct pd12_udc *dev)
{

	u8 i;
	u16 tmp;
	
	DEBUG_PD12("%s, %p\n", __FUNCTION__, dev);

	pd12_set_mode(0x06); /*disconnect soft connect pullup resior */
	mdelay(1500);
	pd12_set_mode(0x16); /*soft connnect*/
	pd12_read_int(&tmp);
	for(i= 0; i< 5; i++)
		pd12_read_lstatus(i,(u8 *)(&tmp));
	dev->gadget.speed = USB_SPEED_FULL;

}

/*
  Register entry point for the peripheral controller driver.
*/
int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	struct pd12_udc *dev = the_controller;
	int retval;

	DEBUG_PD12("%s: %s\n", __FUNCTION__, driver->driver.name);
	if (!driver
	    || driver->speed != USB_SPEED_FULL
	    || !driver->bind
	    || !driver->unbind || !driver->disconnect || !driver->setup)
		return -EINVAL;
	if (!dev)
		return -ENODEV;
	if (dev->driver)
		return -EBUSY;
	/* first hook up the driver ... */
	dev->driver = driver;
	dev->gadget.dev.driver = &driver->driver;

	device_add(&dev->gadget.dev);
	retval = driver->bind(&dev->gadget);
	if (retval) {
		printk("%s: bind to driver %s --> error %d\n", dev->gadget.name,
		       driver->driver.name, retval);
		device_del(&dev->gadget.dev);

		dev->driver = 0;
		dev->gadget.dev.driver = 0;
		return retval;
	}

	/* ... then enable host detection and ep0; and we're ready
	 * for set_configuration as well as eventual disconnect.
	 * NOTE:  this shouldn't power up until later.
	 */
	printk("%s: registered gadget driver '%s'\n", dev->gadget.name,
	       driver->driver.name);

	udc_enable(dev);

	return 0;
}

EXPORT_SYMBOL(usb_gadget_register_driver);

/*
  Unregister entry point for the peripheral controller driver.
*/
int usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
{
	struct pd12_udc *dev = the_controller;
	unsigned long flags;

	if (!dev)
		return -ENODEV;
	if (!driver || driver != dev->driver)
		return -EINVAL;

	spin_lock_irqsave(&dev->lock, flags);
	dev->driver = 0;
	stop_activity(dev, driver);
	spin_unlock_irqrestore(&dev->lock, flags);

	driver->unbind(&dev->gadget);
	device_del(&dev->gadget.dev);

	udc_disable(dev);

	DEBUG_PD12("unregistered gadget driver '%s'\n", driver->driver.name);
	return 0;
}

EXPORT_SYMBOL(usb_gadget_unregister_driver);

/*-------------------------------------------------------------------------*/

/** Read to request from FIFO (max read == bytes in fifo)
 *  Return:  0 = still running, 1 = completed, negative = errno
 *  NOTE: INDEX register must be set for EP
 */
static int read_fifo(struct pd12_ep *ep, struct pd12_request *req)
{
	u8 count;
	u8 ep_sts;
	u8 *buf;
	unsigned bufferspace, is_short;

	/* make sure there's a packet in the FIFO. */
	usb_set_index(ep_index(ep));
	read_data(&ep_sts);
	if ((ep_sts & UDC_FIFO_UNREADABLE) == UDC_FIFO_UNREADABLE) {
		DEBUG_PD12("%s: Packet NOT ready!\n", __FUNCTION__);
		return -EINVAL;
	}

	buf = req->req.buf + req->req.actual;
	prefetchw(buf);
	bufferspace = req->req.length - req->req.actual;

	/* read all bytes from this packet */
	write_cmd(PD12_READ_BUF);
	read_data(&count);
	read_data(&count);
	req->req.actual += min((unsigned)count, bufferspace);

	is_short = (count < ep->ep.maxpacket);
	DEBUG_PD12("read %s %02x, %d bytes%s req %p %d/%d\n",
	      ep->ep.name, ep_sts, count,
	      is_short ? "/S" : "", req, req->req.actual, req->req.length);

	while (count-- != 0) {

		if (bufferspace == 0) {
			/* this happens when the driver's buffer
			 * is smaller than what the host sent.
			 * discard the extra data.
			 */
			if (req->req.status != -EOVERFLOW)
				printk("%s overflow %d\n", ep->ep.name, count);
			req->req.status = -EOVERFLOW;
		} else {
			read_data(buf++);
			bufferspace--;
		}
	}

	write_cmd(PD12_CLEAR_BUF);

	/* completion */
	if (is_short || req->req.actual == req->req.length) {
		done(ep, req, 0);

		return 1;
	}

	/* finished that packet.  the next one may be waiting... */
	return 0;
}


/*
 *	done - retire a request; caller blocked irqs
 *  INDEX register is preserved to keep same
 */
static void done(struct pd12_ep *ep, struct pd12_request *req, int status)
{
	unsigned int stopped = ep->stopped;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, ep);
	list_del_init(&req->queue);

	if (req->req.status == -EINPROGRESS)
		req->req.status = status;
	else
		status = req->req.status;

	if (status && status != -ESHUTDOWN)
		DEBUG_PD12("complete %s req %p stat %d len %u/%u\n",
		      ep->ep.name, &req->req, status,
		      req->req.actual, req->req.length);

	/* don't modify queue heads during completion callback */
	ep->stopped = 1;

	spin_unlock(&ep->dev->lock);
	req->req.complete(&ep->ep, &req->req);
	spin_lock(&ep->dev->lock);
	ep->stopped = stopped;
}


/*
 * 	nuke - dequeue ALL requests
 */
void nuke(struct pd12_ep *ep, int status)
{
	struct pd12_request *req;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, ep);

	/* Flush FIFO */
	flush(ep);

	/* called with irqs blocked */
	while (!list_empty(&ep->queue)) {
		req = list_entry(ep->queue.next, struct pd12_request, queue);
		done(ep, req, status);
	}

}

/** Flush EP
 * NOTE: INDEX register must be set before this call
 */
static void flush(struct pd12_ep *ep)
{
}

/**
 *  handle IN interrupt
 */
static void pd12_in_epn(struct pd12_udc *dev, u8 ep_idx)
{
	u8 ep_sts;
	struct pd12_ep *ep = &dev->ep[ep_idx];
	struct pd12_request *req;

	usb_set_index(ep_idx);
	read_data(&ep_sts);
	DEBUG_PD12("%s: %d, status %x\n", __FUNCTION__, ep_idx, ep_sts);

	if (ep_sts & UDC_EP_STALL) {
		DEBUG_PD12("USB_EP_STALL\n");
		return;
	}

	if (!ep->desc) {
		DEBUG_PD12("%s: NO EP DESC\n", __FUNCTION__);
		return;
	}

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct pd12_request, queue);

	DEBUG_PD12("req: %p\n", req);

	if (!req)
		return;

	write_fifo(ep, req);
}

/*
* handle OUT interrupt(recv)
 */

static void pd12_out_epn(struct pd12_udc *dev, u8 ep_idx)
{
	u8 ep_sts;
	struct pd12_ep *ep = &dev->ep[ep_idx];
	struct pd12_request *req;

	DEBUG_PD12("%s: %d\n", __FUNCTION__, ep_idx);

	usb_set_index(ep_idx);
	read_data(&ep_sts);
	DEBUG_PD12("%s: %d, status %x\n", __FUNCTION__, ep_idx, ep_sts);

	if (ep_sts & UDC_EP_STALL) {
		DEBUG_PD12("USB_EP_STALL\n");
		flush(ep);
		return;
	}

	if (ep->desc) {

		if (list_empty(&ep->queue))
			req = 0;
		else
			req = list_entry(ep->queue.next,
					   struct pd12_request,
					   queue);

		if (!req) {
			printk("%s: NULL REQ %d\n",
				   __FUNCTION__, ep_idx);
			flush(ep);
		} else {
			read_fifo(ep, req);
		}

	} else {
		/* Throw packet away.. */
		printk("%s: No descriptor?!?\n", __FUNCTION__);
		flush(ep);
	}
}

static void stop_activity(struct pd12_udc *dev,
			  struct usb_gadget_driver *driver)
{
	int i;

	/* don't disconnect drivers more than once */
	if (dev->gadget.speed == USB_SPEED_UNKNOWN)
		driver = 0;
	dev->gadget.speed = USB_SPEED_UNKNOWN;

	/* prevent new request submissions, kill any outstanding requests  */
	for (i = 0; i < PD12_MAX_ENDPOINTS - 1; i++) {
		struct pd12_ep *ep = &dev->ep[i];
		ep->stopped = 1;

		usb_set_index(i);
		write_cmd(PD12_SET_STATUS);
		write_data(0x1);
		nuke(ep, -ESHUTDOWN);
	}
		
	write_cmd(1);
	write_cmd(PD12_SET_STATUS);
	write_data(0x1);

	/* report disconnect; the driver is already quiesced */
	if (driver) {
		spin_unlock(&dev->lock);
		driver->disconnect(&dev->gadget);
		spin_lock(&dev->lock);
	}

	/* re-init driver-visible data structures */
	udc_reinit(dev);
}

/** Handle USB RESET interrupt
 */
static void pd12_reset_intr(struct pd12_udc *dev)
{

	struct pd12_request *req;
	struct pd12_ep *ep = &dev->ep[0];

	DEBUG_PD12_EP0("%s: \n", __FUNCTION__);

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct pd12_request, queue);

	if (req){
		done(ep,req,0);
	} else {
		DEBUG_PD12_EP0("%s: NULL REQ\n", __FUNCTION__);
	}

	udc_set_address(dev, 0);
	pd12_set_ack(0);
	pd12_set_ack(1);
	dev->ep0state = WAIT_FOR_SETUP;
	first_tran = 1;
}

/*
 *	pd12 usb client interrupt handler.
 */
static irqreturn_t pd12_udc_irq(int irq, void *_dev, struct pt_regs *r)
{
	struct pd12_udc *dev = _dev;
	volatile u8 int_status;
	unsigned long flags;

	spin_lock_irqsave(&dev->lock,flags);

	DEBUG_PD12("%s (on state %s)\n", __FUNCTION__,
		  state_names[dev->ep0state]);
	write_cmd(PD12_READ_INT);
	read_data(&int_status);
	if (int_status & (PD12_CNTL_IN | PD12_CNTL_OUT))
	{
		if((int_status & PD12_CNTL_OUT) == PD12_CNTL_OUT)
			dev->ep0state = WAIT_FOR_SETUP;
		int_status &= ~(PD12_CNTL_IN | PD12_CNTL_OUT);
		DEBUG_PD12("PD12_EP0 (control)\n");
		pd12_handle_ep0(dev);
		
	}
	if (int_status & PD12_SUSPEND_CHG)
	{
		u8 tmp;
		tmp = *(volatile char *)CPLD_REG0_ADDR;
		int_status &= ~PD12_SUSPEND_CHG;
		if(tmp & USB_SUSPEND)
		{
		/*	write_cmd(PD12_SND_RESUME); */
			if (dev->gadget.speed != USB_SPEED_UNKNOWN
				&& dev->driver
				&& dev->driver->resume)
				dev->driver->resume(&dev->gadget);
		}
		else
		{	
			if (dev->gadget.speed !=
				   USB_SPEED_FULL && dev->driver
				   && dev->driver->suspend)
				dev->driver->suspend(&dev->gadget);
		}
	}
	if (int_status & PD12_BUS_RST)
	{
		int_status &= ~PD12_BUS_RST;
		pd12_reset_intr(dev);
	}
	if (int_status & PD12_EP1_IN)
	{
		int_status &= ~PD12_EP1_IN;
		DEBUG_PD12("PD12_EP1_IN\n");
		pd12_in_epn(dev, 2);
	}
	if (int_status & PD12_MAIN_IN)
	{
		int_status &= ~PD12_MAIN_IN;
		DEBUG_PD12("PD12_MAIN_IN\n");
		pd12_in_epn(dev, 4);
	}
	if (int_status & PD12_EP1_OUT)
	{
		int_status &= ~PD12_EP1_OUT;
		DEBUG_PD12("PD12_EP1_OUT\n");
		pd12_out_epn(dev, 1);
	}
	if (int_status & PD12_MAIN_OUT)
	{
		int_status &= ~PD12_MAIN_OUT;
		DEBUG_PD12("PD12_MAIN_OUT\n");
		pd12_out_epn(dev, 3);
	}
			
	spin_unlock_irqrestore(&dev->lock,flags);
	return IRQ_HANDLED;
}

static int pd12_ep_enable(struct usb_ep *_ep,
			     const struct usb_endpoint_descriptor *desc)
{
	struct pd12_ep *ep;
	struct pd12_udc *dev;
	unsigned long flags;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct pd12_ep, ep);
	if (!_ep || !desc || ep->desc || _ep->name == ep0name
	    || desc->bDescriptorType != USB_DT_ENDPOINT
	    || ep->bEndpointAddress != desc->bEndpointAddress
	    || ep_maxpacket(ep) < le16_to_cpu(desc->wMaxPacketSize)) {
		DEBUG_PD12("%s, bad ep or descriptor\n", __FUNCTION__);
		return -EINVAL;
	}

	/* xfer types must match, except that interrupt ~= bulk */
	if (ep->bmAttributes != desc->bmAttributes ) {
		DEBUG_PD12("%s, %s type mismatch\n", __FUNCTION__, _ep->name);
		return -EINVAL;
	}

	/* hardware _could_ do smaller, but driver doesn't */
	if ((desc->bmAttributes == USB_ENDPOINT_XFER_BULK
	     && le16_to_cpu(desc->wMaxPacketSize) != ep_maxpacket(ep))
	    || !desc->wMaxPacketSize) {
		DEBUG_PD12("%s, bad %s maxpacket\n", __FUNCTION__, _ep->name);
		return -ERANGE;
	}

	dev = ep->dev;
	if (!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN) {
		DEBUG_PD12("%s, bogus device state\n", __FUNCTION__);
		return -ESHUTDOWN;
	}

	spin_lock_irqsave(&ep->dev->lock, flags);

	ep->stopped = 0;
	ep->desc = desc;
	ep->ep.maxpacket = le16_to_cpu(desc->wMaxPacketSize);

	/* Reset halt state (does flush) */
	pd12_set_halt(_ep, 0);

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	DEBUG_PD12("%s: enabled %s\n", __FUNCTION__, _ep->name);
	return 0;
}

/** Disable EP
 *  NOTE: Sets INDEX register
 */
static int pd12_ep_disable(struct usb_ep *_ep)
{
	struct pd12_ep *ep;
	unsigned long flags;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct pd12_ep, ep);
	if (!_ep || !ep->desc) {
		DEBUG_PD12("%s, %s not enabled\n", __FUNCTION__,
		      _ep ? ep->ep.name : NULL);
		return -EINVAL;
	}

	spin_lock_irqsave(&ep->dev->lock, flags);

	usb_set_index(ep_index(ep));

	/* Nuke all pending requests (does flush) */
	nuke(ep, -ESHUTDOWN);

	/* Disable ep  */
	write_cmd(PD12_SET_EP_EN);
	write_data(0x0);

	ep->desc = 0;
	ep->stopped = 1;

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	DEBUG_PD12("%s: disabled %s\n", __FUNCTION__, _ep->name);
	return 0;
}

static struct usb_request *pd12_alloc_request(struct usb_ep *ep,
						 unsigned gfp_flags)
{
	struct pd12_request *req;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, ep);

	req = kmalloc(sizeof *req, gfp_flags);
	if (!req)
		return 0;

	memset(req, 0, sizeof *req);
	INIT_LIST_HEAD(&req->queue);

	return &req->req;
}

static void pd12_free_request(struct usb_ep *ep, struct usb_request *_req)
{
	struct pd12_request *req;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, ep);

	req = container_of(_req, struct pd12_request, req);
	WARN_ON(!list_empty(&req->queue));
	kfree(req);
}

static void *pd12_alloc_buffer(struct usb_ep *ep, unsigned bytes,
				  dma_addr_t * dma, unsigned gfp_flags)
{
	char *retval;

	DEBUG_PD12("%s (%p, %d, %d)\n", __FUNCTION__, ep, bytes, gfp_flags);

	retval = kmalloc(bytes, gfp_flags & ~(__GFP_DMA | __GFP_HIGHMEM));
	if (retval)
		*dma = virt_to_bus(retval);
	return retval;
}

static void pd12_free_buffer(struct usb_ep *ep, void *buf, dma_addr_t dma,
				unsigned bytes)
{
	DEBUG_PD12("%s, %p\n", __FUNCTION__, ep);
	kfree(buf);
}

/** Queue one request
 *  Kickstart transfer if needed
 *  NOTE: Sets INDEX register
 */
static int pd12_queue(struct usb_ep *_ep, struct usb_request *_req,
			 unsigned gfp_flags)
{
	struct pd12_request *req;
	struct pd12_ep *ep;
	struct pd12_udc *dev;
	unsigned long flags;
	u8 ep_status;

	DEBUG_PD12("\n\n\n%s, %p\n", __FUNCTION__, _ep);

	req = container_of(_req, struct pd12_request, req);
	if (!_req || !_req->complete || !_req->buf
	     || !list_empty(&req->queue)) {
		DEBUG_PD12("%s, bad params\n", __FUNCTION__);
		return -EINVAL;
	}

	ep = container_of(_ep, struct pd12_ep, ep);
	if (!_ep || (!ep->desc && (ep->ep.name != ep0name))) {
		DEBUG_PD12("%s, bad ep\n", __FUNCTION__);
		return -EINVAL;
	}

	dev = ep->dev;
	if (!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN) {
		DEBUG_PD12("%s, bogus device state %p\n", __FUNCTION__, dev->driver);
		return -ESHUTDOWN;
	}

	DEBUG_PD12("%s queue req %p, len %d buf %p\n", _ep->name, _req, _req->length,
	      _req->buf);

	spin_lock_irqsave(&dev->lock, flags);

	_req->status = -EINPROGRESS;
	_req->actual = 0;

	/* kickstart this i/o queue? */
	DEBUG_PD12("Add to %d Q %d %d\n", ep_index(ep), list_empty(&ep->queue),
	      ep->stopped);
	if (list_empty(&ep->queue) && !ep->stopped) {

		if (ep_index(ep) == 0) {
			/* EP0 */
			list_add_tail(&req->queue, &ep->queue);
			pd12_ep0_kick(dev, ep);
			req = 0;
		} else if (ep_is_in(ep)) {
			/* EP2 & EP4 */
			usb_set_index(ep_index(ep));
			read_data(&ep_status);
			if ((ep_status & 0x0) == 0x0) {
				if (write_fifo(ep, req) == 1)
					req = 0;
			}
		} else {
			/* EP1  & EP3 */
			usb_set_index(ep_index(ep));
			read_data(&ep_status);
			if ((ep_status & 0x01) == 0x01) {
				if (read_fifo(ep, req) == 1)
					req = 0;
			}
		}
	}

	/* pio or dma irq handler advances the queue. */
	if (req != 0)
		list_add_tail(&req->queue, &ep->queue);

	spin_unlock_irqrestore(&dev->lock, flags);

	return 0;
}

/* dequeue JUST ONE request */
static int pd12_dequeue(struct usb_ep *_ep, struct usb_request *_req)
{
	struct pd12_ep *ep;
	struct pd12_request *req;
	unsigned long flags;

	DEBUG_PD12("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct pd12_ep, ep);
	if (!_ep || ep->ep.name == ep0name )
		return -EINVAL;

	spin_lock_irqsave(&ep->dev->lock, flags);

	/* make sure it's actually queued on this endpoint */
	list_for_each_entry(req, &ep->queue, queue) {
		if (&req->req == _req)
			break;
	}
	if (&req->req != _req) {
		spin_unlock_irqrestore(&ep->dev->lock, flags);
		return -EINVAL;
	}

	done(ep, req, -ECONNRESET);

	spin_unlock_irqrestore(&ep->dev->lock, flags);
	return 0;
}

/** Halt specific EP
 *  Return 0 if success
 *  NOTE: Sets INDEX register to EP !
 */
static int pd12_set_halt(struct usb_ep *_ep, int value)
{
	struct pd12_ep *ep;
	unsigned long flags;
	u8 ep_status;

	ep = container_of(_ep, struct pd12_ep, ep);
	if (!_ep || (!ep->desc && ep->ep.name != ep0name)) {
		DEBUG_PD12("%s, bad ep\n", __FUNCTION__);
		return -EINVAL;
	}

	usb_set_index(ep_index(ep));

	DEBUG_PD12("%s, ep %d, val %d\n", __FUNCTION__, ep_index(ep), value);

	spin_lock_irqsave(&ep->dev->lock, flags);

	if (ep_index(ep) == 0) {
		/* EP0 */
		write_cmd(PD12_SET_STATUS | ep_index(ep));
		write_data(0x01);
	} else if (ep_is_in(ep)) {
		write_cmd(PD12_READ_EP_STATUS);
		read_data(&ep_status);
		if (value && ((ep_status & 0x60) /*buffer 0 or 1 full*/
			      || !list_empty(&ep->queue))) {
			/*
			 * Attempts to halt IN endpoints will fail (returning -EAGAIN)
			 * if any transfer requests are still queued, or if the controller
			 * FIFO still holds bytes that the host hasn't collected.
			 */
			spin_unlock_irqrestore(&ep->dev->lock, flags);
			DEBUG_PD12
			    ("Attempt to halt IN endpoint failed (returning -EAGAIN) %d %d\n",
			     (ep_status & 0x60),
			     !list_empty(&ep->queue));
			return -EAGAIN;
		}
		flush(ep);
		if (value)
		{
			write_cmd(PD12_SET_STATUS | ep_index(ep));
			write_data(0x01);
		} else {
			write_cmd(PD12_SET_STATUS | ep_index(ep));
			write_data(0x00);
		}

	} else {

		flush(ep);
		if (value)
		{
			write_cmd(PD12_SET_STATUS | ep_index(ep));
			write_data(0x01);
		} else {
			write_cmd(PD12_SET_STATUS | ep_index(ep));
			write_data(0x00);
		}
	}

	if (value) {
		ep->stopped = 1;
	} else {
		ep->stopped = 0;
	}

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	DEBUG_PD12("%s %s halted\n", _ep->name, value == 0 ? "NOT" : "IS");

	return 0;
}


/****************************************************************/
/* End Point 0 related functions                                */
/****************************************************************/

/* return:  0 = still running, 1 = completed, negative = errno */
static int write_fifo_ep0(struct pd12_ep *ep, struct pd12_request *req)
{
	u8 max;
	unsigned count;
	int is_last;
	unsigned length;
	u8* buf;
	u8 ep_sts;

	write_cmd(1); /* index 1*/
	read_data(&ep_sts);
/*	pd12_set_ack(1); */
	max = ep_maxpacket(ep);
	buf = req->req.buf + req->req.actual;
	prefetch(buf);

	DEBUG_PD12_EP0("%s\n", __FUNCTION__);

	length = req->req.length - req->req.actual;
	length = min(length, (unsigned)max);
	req->req.actual += length;

	DEBUG_PD12("Write %d (max %d), fifo %p\n", length, max, buf);

	count = length;
	write_cmd(PD12_WRITE_BUF);
	write_data(0x0);
	write_data(count);
	while (length--) {
		write_data(*buf++);
	}
	
	write_cmd(PD12_VALIDATE_BUF);
	/* last packet is usually short (or a zlp) */
	if (unlikely(count != max))
		is_last = 1;
	else {
		if (likely(req->req.length != req->req.actual) || req->req.zero)
			is_last = 0;
		else
			is_last = 1;
	}

	DEBUG_PD12_EP0("%s: wrote %s %d bytes%s %d left %p\n", __FUNCTION__,
		  ep->ep.name, count,
		  is_last ? "/L" : "", req->req.length - req->req.actual, req);

	/* requests complete when all IN data is in the FIFO */
	if (is_last) {
		done(ep, req, 0);
		return 1;
	}

	return 0;
}

static __inline__ void pd12_fifo_read(struct pd12_ep *ep,
					unsigned char *cp, u8 max)
{
	u8 count;

	usb_set_index(0);
	write_cmd(PD12_READ_BUF);
	read_data(&count);
	read_data(&count);
	if (count > max)
		count = max;
	while (count--){
		read_data(cp++);
	}
	write_cmd(PD12_CLEAR_BUF);
}

static __inline__ void pd12_fifo_write(struct pd12_ep *ep,
					  unsigned char *cp, u8 count)
{
	write_cmd(1); /* index 1 */
	write_cmd(PD12_WRITE_BUF);
	write_data(0x0);
	write_data(count);
	DEBUG_PD12_EP0("fifo_write: %d %d\n", ep_index(ep), count);
	while (count--)
		write_data(*cp++);
	write_cmd(PD12_VALIDATE_BUF);
}
static int read_fifo_ep0(struct pd12_ep *ep, struct pd12_request *req)
{
	u8 ep_status;
	u8 len;
	u8 *buf;
	unsigned bufferspace, count, is_short;

	DEBUG_PD12_EP0("%s\n", __FUNCTION__);

	usb_set_index(0); /* index 0*/
	read_data(&ep_status);
	if ((ep_status & UDC_FIFO_UNREADABLE) == UDC_FIFO_UNREADABLE)
		return 0;

	buf = req->req.buf + req->req.actual;
	prefetchw(buf);
	bufferspace = req->req.length - req->req.actual;

	write_cmd(PD12_READ_BUF);
	read_data(&len);
	read_data(&len);
	count = (unsigned)len;

	is_short = (count < ep->ep.maxpacket);
	DEBUG_PD12_EP0("read %s, %d bytes%s req %p %d/%d\n",
		  ep->ep.name, count,
		  is_short ? "/S" : "", req, req->req.actual, req->req.length);

	while (count--) {
		u8 byte;
		read_data(&byte);
		if (unlikely(bufferspace == 0)) {
			/* this happens when the driver's buffer
			 * is smaller than what the host sent.
			 * discard the extra data.
			 */
			if (req->req.status != -EOVERFLOW)
				DEBUG_PD12_EP0("%s overflow %d\n", ep->ep.name,
					  count);
			req->req.status = -EOVERFLOW;
		} else {
			*buf++ = byte;
			bufferspace--;
		}
	}

	/* completion */
	if (is_short || req->req.actual == req->req.length) {
		done(ep, req, 0);
		return 1;
	}

	/* finished that packet.  the next one may be waiting... */
	return 0;
}

/**
 * udc_set_address - set the USB address for this device
 * @address:
 *
 * Called from control endpoint function after it decodes a set address setup packet.
 */
static void udc_set_address(struct pd12_udc *dev, unsigned char address)
{
	DEBUG_PD12_EP0("%s: %d\n", __FUNCTION__, address);

	dev->usb_address = address;
	write_cmd(PD12_SET_ADDR_EN);
	write_data(0x80 | address);
}

/*
 * DATA_STATE_RECV (OUT_PKT_RDY)
 *      - if error
 *              set EP0_CLR_OUT | EP0_DATA_END | EP0_SEND_STALL bits
 *      - else
 *              set EP0_CLR_OUT bit
 				if last set EP0_DATA_END bit
 */
static void pd12_ep0_out(struct pd12_udc *dev)
{
	struct pd12_request *req;
	struct pd12_ep *ep = &dev->ep[0];
	int ret;

	DEBUG_PD12_EP0("%s: \n", __FUNCTION__);

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct pd12_request, queue);

	if (req) {

		if (req->req.length == 0) {
			DEBUG_PD12_EP0("ZERO LENGTH OUT!\n");
			dev->ep0state = WAIT_FOR_SETUP;
			return;
		}
		ret = read_fifo_ep0(ep, req);
		if (ret) {
			/* Done! */
			DEBUG_PD12_EP0("%s: finished, waiting for status\n",
				  __FUNCTION__);
			/* read last status here ?*/
			dev->ep0state = WAIT_FOR_SETUP;
		} else {
			/* Not done yet.. */
			DEBUG_PD12_EP0("%s: not finished\n", __FUNCTION__);
			/* usb_set(EP0_CLR_OUT, USB_EP0_CSR); */
		}
	} else {
		DEBUG_PD12_EP0("NO REQ??!\n");
	}
}

/*
 * DATA_STATE_XMIT
 */
static int pd12_ep0_in(struct pd12_udc *dev)
{
	struct pd12_request *req;
	struct pd12_ep *ep = &dev->ep[0];
	int ret, need_zlp = 0;
	u8 val;
	
	DEBUG_PD12_EP0("%s: \n", __FUNCTION__);


	pd12_read_lstatus(1,&val);
	pd12_set_ack(1);
	if(((val & 0x1) != 0x1) && !first_tran){
	/*	printk("return from ep0_in\n"); */
		return 0;
	}
	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct pd12_request, queue);

	if (!req) {
		dev->ep0state = WAIT_FOR_SETUP;
		DEBUG_PD12_EP0("%s: NULL REQ\n", __FUNCTION__);
		return 0;
	}

	if (req->req.length == 0) {
		dev->ep0state = WAIT_FOR_SETUP;
		return 1;
	}

	ret = write_fifo_ep0(ep, req);
	first_tran = 0;
	if (ret == 1 && !need_zlp) {
		/* Last packet */
		DEBUG_PD12_EP0("%s: finished, waiting for status\n", __FUNCTION__);
		dev->ep0state = WAIT_FOR_SETUP;
	} else {
		DEBUG_PD12_EP0("%s: not finished\n", __FUNCTION__);
	}

	return 1;
}

static int pd12_handle_get_status(struct pd12_udc *dev,
				     struct usb_ctrlrequest *ctrl)
{
	struct pd12_ep *ep0 = &dev->ep[0];
	struct pd12_ep *qep;
	int reqtype = (ctrl->bRequestType & USB_RECIP_MASK);
	u16 val = 0;
	u8 ep_sts;

	if (reqtype == USB_RECIP_INTERFACE) {
		/* This is not supported.
		 * And according to the USB spec, this one does nothing..
		 * Just return 0
		 */
		DEBUG_PD12_SETUP("GET_STATUS: USB_RECIP_INTERFACE\n");
	} else if (reqtype == USB_RECIP_DEVICE) {
		DEBUG_PD12_SETUP("GET_STATUS: USB_RECIP_DEVICE\n");
		val |= (1 << 0);	/* Self powered */
		/*val |= (1<<1); */	/* Remote wakeup */
	} else if (reqtype == USB_RECIP_ENDPOINT) {
		int ep_num = (ctrl->wIndex & ~USB_DIR_IN);

		DEBUG_PD12_SETUP
		    ("GET_STATUS: USB_RECIP_ENDPOINT (%d), ctrl->wLength = %d\n",
		     ep_num, ctrl->wLength);

		if (ctrl->wLength > 2 || ep_num > 3) /* ep_num cannt abouve maxenpoint?*/
			return -EOPNOTSUPP;

		qep = &dev->ep[ep_num];
		if (ep_is_in(qep) != ((ctrl->wIndex & USB_DIR_IN) ? 1 : 0)
		    && ep_index(qep) != 0) {
			return -EOPNOTSUPP;
		}

		usb_set_index(ep_index(qep));
		read_data(&ep_sts);
		val = (u16)ep_sts;

		/* Back to EP0 index */
		usb_set_index(0);

		DEBUG_PD12_SETUP("GET_STATUS, ep: %d (%x), val = %d\n", ep_num,
			    ctrl->wIndex, val);
	} else {
		DEBUG_PD12_SETUP("Unknown REQ TYPE: %d\n", reqtype);
		return -EOPNOTSUPP;
	}

	/* Put status to FIFO */
	pd12_fifo_write(ep0, (u8 *) & val, sizeof(val));

	return 0;
}

/*
 * WAIT_FOR_SETUP
 *      - read data packet from EP0 FIFO
 *      - decode command
 *      - if error
 *              set EP0_CLR_OUT | EP0_DATA_END | EP0_SEND_STALL bits
 *      - else
 *              set EP0_CLR_OUT | EP0_DATA_END bits
 */
static void pd12_ep0_setup(struct pd12_udc *dev)
{
	struct pd12_ep *ep = &dev->ep[0];
	struct usb_ctrlrequest ctrl;
	int i;
	u8 stat;
	u8 inbuf[16];
	
	DEBUG_PD12_SETUP("%s: \n", __FUNCTION__);

	/* Nuke all previous transfers */
	nuke(ep, -EPROTO);
	pd12_read_lstatus(0,&stat);
	pd12_set_ack(0);
	pd12_set_ack(1);
	if((stat & 0x01) != 0x01){
		return ;
	}
	pd12_fifo_read(ep, (u8 *)&ctrl, 8);

	DEBUG_PD12_SETUP("CTRL.bRequestType = 0x%x (is_in 0x%x)\n", ctrl.bRequestType,
		    ctrl.bRequestType == USB_DIR_IN);
	DEBUG_PD12_SETUP("CTRL.bRequest = 0x%x\n", ctrl.bRequest);
	DEBUG_PD12_SETUP("CTRL.wLength = 0x%x\n", ctrl.wLength);
	DEBUG_PD12_SETUP("CTRL.wValue = 0x%x (%d)\n", ctrl.wValue, ctrl.wValue >> 8);
	DEBUG_PD12_SETUP("CTRL.wIndex = 0x%x\n", ctrl.wIndex);

	/* Set direction of EP0 */
	if (ctrl.bRequestType & USB_DIR_IN) {
		ep->bEndpointAddress |= USB_DIR_IN;
	} else {
		ep->bEndpointAddress &= ~USB_DIR_IN;
	}


	/* Handle some SETUP packets ourselves */
	switch (ctrl.bRequest) {
	case USB_REQ_SET_ADDRESS:
		if (ctrl.bRequestType != (USB_TYPE_STANDARD | USB_RECIP_DEVICE))
			break;

		DEBUG_PD12_SETUP("USB_REQ_SET_ADDRESS (%d)\n", ctrl.wValue);
		udc_set_address(dev, le16_to_cpu(ctrl.wValue));
		pd12_fifo_write(ep,inbuf,0);
		return;

	case USB_REQ_GET_STATUS:{
			if (pd12_handle_get_status(dev, &ctrl) == 0)
				return;

	case USB_REQ_CLEAR_FEATURE:
	case USB_REQ_SET_FEATURE:
			if (ctrl.bRequestType == USB_RECIP_ENDPOINT) {
				struct pd12_ep *qep;
				int ep_num = (ctrl.wIndex & 0x0f);

				/* Support only HALT feature */
				if (ctrl.wValue != 0 || ctrl.wLength != 0
				    || ep_num > 4 || ep_num < 1)
					break;

				qep = &dev->ep[ep_num];
				if (ctrl.bRequest == USB_REQ_SET_FEATURE) {
					DEBUG_PD12_SETUP("SET_FEATURE (%d)\n",
						    ep_num);
					pd12_set_halt(&qep->ep, 1);
				} else {
					DEBUG_PD12_SETUP("CLR_FEATURE (%d)\n",
						    ep_num);
					pd12_set_halt(&qep->ep, 0);
				}
				usb_set_index(0);

				return;
			}
			break;
		}

	default:
		break;
	}

	if (dev->driver) {
		/* device-2-host (IN) or no data setup command, process immediately */
		spin_unlock(&dev->lock);
		i = dev->driver->setup(&dev->gadget, &ctrl);
		spin_lock(&dev->lock);

		if (i < 0) {
			pd12_ep0_in(dev);
			/* setup processing failed, force stall */
			DEBUG_PD12_SETUP
			    ("  --> ERROR: gadget setup FAILED (stalling), setup returned %d\n",
			     i);
		/*	usb_set_index(0); */
			write_cmd(PD12_SET_STATUS);
			write_data(0x1);
			write_cmd(PD12_SET_STATUS + 0x1);
			write_data(0x1);
			/* ep->stopped = 1; */
			dev->ep0state = WAIT_FOR_SETUP;
		}
	}
}

/*
 * handle ep0 in interrupt
 */
static void pd12_handle_ep0(struct pd12_udc *dev)
{
	struct pd12_ep *ep = &dev->ep[0];
	u8 ep0in_sts,ep0out_sts/*,int_sts*/;
	
	/* Set index 0 */
	write_cmd(0x0);
	read_data(&ep0out_sts);
	
	write_cmd(0x1);
	read_data(&ep0in_sts);
	

	/*
	 * if STALL is set, clear STALL bit
	 */
	if (ep0out_sts & UDC_EP_STALL ) {
/*		DEBUG_PD12_EP0("%s: EP0_SENT_STALL is set: %x\n", __FUNCTION__, status); */
		write_cmd(PD12_SET_STATUS);
		write_data(0x0);
		nuke(ep, -ECONNABORTED);
		dev->ep0state = WAIT_FOR_SETUP;
		return;
	}

	if (ep0in_sts & UDC_EP_STALL ) {
/*		DEBUG_PD12_EP0("%s: EP0_SENT_STALL is set: %x\n", __FUNCTION__, status); */
		write_cmd(PD12_SET_STATUS + 0x1);
		write_data(0x0);
		nuke(ep, -ECONNABORTED);
		dev->ep0state = WAIT_FOR_SETUP;
		return;
	}

	switch (dev->ep0state) {
		case WAIT_FOR_SETUP:
			DEBUG_PD12_EP0("WAIT_FOR_SETUP\n");
			pd12_ep0_setup(dev);
			break;
		case DATA_STATE_RECV:
			DEBUG_PD12_EP0("DATA_STATE_RECV\n");
			pd12_ep0_out(dev);
			break;
		case DATA_STATE_XMIT:
			DEBUG_PD12_EP0("continue with DATA_STATE_XMIT\n");
			pd12_ep0_in(dev);
			return;
		default:
			/* Stall? */
			DEBUG_PD12_EP0("Odd state!! state = %s\n",
				  state_names[dev->ep0state]);
			dev->ep0state = WAIT_FOR_SETUP;
			/* nuke(ep, 0); */
			/* usb_set(EP0_SEND_STALL, ep->csr1); */
			break;
	}

}

static void pd12_ep0_kick(struct pd12_udc *dev, struct pd12_ep *ep)
{

	if (ep_is_in(ep)) {
		dev->ep0state = DATA_STATE_XMIT;
		pd12_ep0_in(dev);
	} else {
		dev->ep0state = DATA_STATE_RECV;
		pd12_ep0_out(dev);
	}
}

/* ---------------------------------------------------------------------------
 * 	device-scoped parts of the api to the usb controller hardware
 * ---------------------------------------------------------------------------
 */

static int pd12_udc_get_frame(struct usb_gadget *_gadget)
{
	u16 frame1,frame2;
	write_cmd(PD12_RD_CUR_FRAME_NUM);
	read_data((u8 *)&frame1);/* Least significant 8 bits */
	read_data((u8 *)&frame2);/* Most significant 3 bits */
	DEBUG_PD12("%s, %p\n", __FUNCTION__, _gadget);
	return ((frame2 & 0x07) << 8) | (frame1 & 0xff);
}

static int pd12_udc_wakeup(struct usb_gadget *_gadget)
{
	/* host may not have enabled remote wakeup */
	/*if ((UDCCS0 & UDCCS0_DRWF) == 0)
	   return -EHOSTUNREACH;
	   udc_set_mask_UDCCR(UDCCR_RSM); */
	return -ENOTSUPP;
}

static const struct usb_gadget_ops pd12_udc_ops = {
	.get_frame = pd12_udc_get_frame,
	.wakeup = pd12_udc_wakeup,
	/* current versions must always be self-powered */
};

static void nop_release(struct device *dev)
{
	DEBUG_PD12("%s %s\n", __FUNCTION__, dev->bus_id);
}

static struct pd12_udc usb_memory = {
	.usb_address = 0,

	.gadget = {
		   .ops = &pd12_udc_ops,
		   .ep0 = &usb_memory.ep[0].ep,
		   .name = driver_name,
		   .dev = {
			   .bus_id = "gadget",
			   .release = nop_release,
			   },
		   },

	/* control endpoint */
	.ep[0] = {
		  .ep = {
			 .name = ep0name,
			 .ops = &pd12_ep_ops,
			 .maxpacket = 16,
			 },
		  .dev = &usb_memory,

		  .bEndpointAddress = 0,
		  .bmAttributes = USB_ENDPOINT_XFER_CONTROL,
		  },

	/* first group of endpoints */
	.ep[1] = {
		  .ep = {
			 .name = "ep1out-bulk",
			 .ops = &pd12_ep_ops,
			 .maxpacket = 16,
			 },
		  .dev = &usb_memory,

		  .bEndpointAddress = 1,
		  .bmAttributes = USB_ENDPOINT_XFER_BULK,

		  },

	.ep[2] = {
		  .ep = {
			 .name = "ep2in-bulk",
			 .ops = &pd12_ep_ops,
			 .maxpacket = 16,
			 },
		  .dev = &usb_memory,

		  .bEndpointAddress = 2 | USB_DIR_IN,
		  .bmAttributes = USB_ENDPOINT_XFER_BULK,

		  },
	.ep[3] = {
		  .ep = {
			 .name = "ep3out-int",
			 .ops = &pd12_ep_ops,
			 .maxpacket = 64,
			 },
		  .dev = &usb_memory,

		  .bEndpointAddress = 3,
		  .bmAttributes = USB_ENDPOINT_XFER_INT,

		  },
	.ep[4] = {
		  .ep = {
			 .name = "ep3in-int",
			 .ops = &pd12_ep_ops,
			 .maxpacket = 64,
			 },
		  .dev = &usb_memory,

		  .bEndpointAddress = 4 | USB_DIR_IN,
		  .bmAttributes = USB_ENDPOINT_XFER_INT,

		  },
};

/*
 * 	probe - binds to the platform device
 */
static int __devinit pd12_udc_probe(struct device *_dev)
{
	struct pd12_udc *dev = &usb_memory;
	int retval;

	DEBUG_PD12("%s: %p\n", __FUNCTION__, _dev);
	spin_lock_init(&dev->lock);
	dev->dev = _dev;
	
	device_initialize(&dev->gadget.dev);
	dev->gadget.dev.parent = _dev;

	the_controller = dev;
	dev_set_drvdata(_dev, dev);

	udc_reinit(dev);

	dev->gadget.speed = USB_SPEED_FULL;
	/* irq setup after old hardware state is cleaned up */
	retval =
	    request_irq(IRQ_USBINTR, pd12_udc_irq, /*SA_INTERRUPT*/SA_SAMPLE_RANDOM, driver_name,
			dev);
	if (retval != 0) {
		DEBUG_PD12(KERN_ERR "%s: can't get irq %i, err %d\n", driver_name,
		      IRQ_USBINTR, retval);
		return -EBUSY;
	}

	create_proc_files();

	return retval;
}

static int __devexit pd12_udc_remove(struct device *_dev)
{
	struct pd12_udc *dev = _dev->driver_data;

	DEBUG_PD12("%s: %p\n", __FUNCTION__, dev);

	udc_disable(dev);
	remove_proc_files();
	usb_gadget_unregister_driver(dev->driver);

	free_irq(IRQ_USBINTR, dev);

	dev_set_drvdata(_dev, 0);

	the_controller = 0;

	return 0;
}
static struct platform_device	pd12_pdev = {
	.name	= (char *) driver_name,
	.id		= -1,
};

/*-------------------------------------------------------------------------*/

static struct device_driver udc_driver = {
	.name = (char *)driver_name,
	.bus = &platform_bus_type,
	.probe = pd12_udc_probe,
	.remove = pd12_udc_remove
	    /* FIXME power management support */
	    /* .suspend = ... disable UDC */
	    /* .resume = ... re-enable UDC */
};

static int __init udc_init(void)
{
	int retval;
	
	DEBUG_PD12("%s: %s version %s\n", __FUNCTION__, driver_name, DRIVER_VERSION);
	th_pd12_virt = ioremap((ulong)TH_PD12_ADDR,0x10);
	if(!th_pd12_virt){
		printk("%s: ioremap fail\n",__FUNCTION__);
		return -EIO;
	}
	th_cpld_virt = ioremap((ulong)TH_CPLD_ADDR,0x10);
	if(!th_cpld_virt){
		printk("%s: ioremap fail\n",__FUNCTION__);
		return -EIO;
	}
	retval = platform_device_register (&pd12_pdev);
	if (retval < 0){
		platform_device_unregister (&pd12_pdev);
		return retval;
	}
	return driver_register(&udc_driver);
}

static void __exit udc_exit(void)
{
	if(th_pd12_virt){
		iounmap((void*)th_pd12_virt);
		th_pd12_virt = NULL;
	}
	if(th_cpld_virt){
		iounmap((void*)th_cpld_virt);
		th_cpld_virt = NULL;
	}
	driver_unregister(&udc_driver);
}

module_init(udc_init);
module_exit(udc_exit);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
