/*
 * linux/drivers/usb/gadget/musbhsfc_udc.c
 * Inventra MUSBHSFC USB device controller used on the AMCC PPC440EP(x)
 *
 * Copyright (C) 2004 Mikko Lahteenmaki, Nordic ID
 * Copyright (C) 2004 Bo Henriksen, Nordic ID
 * Copyright (C) 2004 IBM Corp.
 * Copyright (C) 2005 Montavista Software, Inc. <source@mvista.com>
 * Copyright (C) 2006 AMCC
 * Copyright (C) 2006 Stefan Roese <sr@denx.de>, DENX Software Engineering
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

#include <linux/platform_device.h>
#include "musbhsfc_udc.h"

//#define DEBUG printk
//#define DEBUG_EP0 printk
//#define DEBUG_SETUP printk

#ifndef DEBUG_EP0
# define DEBUG_EP0(fmt,args...)
#endif
#ifndef DEBUG_SETUP
# define DEBUG_SETUP(fmt,args...)
#endif
#ifndef DEBUG
# define NO_STATES
# define DEBUG(fmt,args...)
#endif

#define	DRIVER_DESC		"MUSBHSFC USB Device Controller"
#define	DRIVER_VERSION		__DATE__

struct musbhsfc_udc *the_controller;

static const char driver_name[] = "musbhsfc_udc";
static const char driver_desc[] = DRIVER_DESC;
static const char ep0name[] = "ep0-control";

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
static int musbhsfc_ep_enable(struct usb_ep *ep,
			      const struct usb_endpoint_descriptor *);
static int musbhsfc_ep_disable(struct usb_ep *ep);
static struct usb_request *musbhsfc_alloc_request(struct usb_ep *ep, unsigned);
static void musbhsfc_free_request(struct usb_ep *ep, struct usb_request *);
static void *musbhsfc_alloc_buffer(struct usb_ep *ep, unsigned, dma_addr_t *,
				   unsigned);
static void musbhsfc_free_buffer(struct usb_ep *ep, void *, dma_addr_t,
				 unsigned);
static int musbhsfc_queue(struct usb_ep *ep, struct usb_request *, unsigned);
static int musbhsfc_dequeue(struct usb_ep *ep, struct usb_request *);
static int musbhsfc_set_halt(struct usb_ep *ep, int);
static int musbhsfc_fifo_status(struct usb_ep *ep);
static int musbhsfc_fifo_status(struct usb_ep *ep);
static void musbhsfc_fifo_flush(struct usb_ep *ep);
static void musbhsfc_ep0_kick(struct musbhsfc_udc *dev, struct musbhsfc_ep *ep);
static void musbhsfc_handle_ep0(struct musbhsfc_udc *dev, u32 intr);

static void done(struct musbhsfc_ep *ep, struct musbhsfc_request *req,
		 int status);
static void pio_irq_enable(int bEndpointAddress);
static void pio_irq_disable(int bEndpointAddress);
static void stop_activity(struct musbhsfc_udc *dev,
			  struct usb_gadget_driver *driver);
static void flush(struct musbhsfc_ep *ep);
static void udc_enable(struct musbhsfc_udc *dev);
static void udc_set_address(struct musbhsfc_udc *dev, unsigned char address);

static struct usb_ep_ops musbhsfc_ep_ops = {
	.enable = musbhsfc_ep_enable,
	.disable = musbhsfc_ep_disable,

	.alloc_request = musbhsfc_alloc_request,
	.free_request = musbhsfc_free_request,

	.alloc_buffer = musbhsfc_alloc_buffer,
	.free_buffer = musbhsfc_free_buffer,

	.queue = musbhsfc_queue,
	.dequeue = musbhsfc_dequeue,

	.set_halt = musbhsfc_set_halt,
	.fifo_status = musbhsfc_fifo_status,
	.fifo_flush = musbhsfc_fifo_flush,
};

unsigned long phys_addr;
unsigned long base_addr;
unsigned long base_len;
unsigned int device_irq;

/* Inline code */

static __inline__ u8 usb_readb(unsigned long offset)
{
	return in_8((volatile u8 *) (base_addr + offset));
}

static __inline__ u16 usb_readw(unsigned long offset)
{
	return in_be16((volatile u16 *) (base_addr + offset));
}

static __inline__ u32 usb_read(unsigned long offset)
{
	return in_be32((volatile u32 *) (base_addr + offset));
}

static __inline__ void usb_writeb(u8 b, unsigned long offset)
{
	out_8((volatile u8 *)(base_addr + offset), b);
}

static __inline__ void usb_writew(u16 b, unsigned long offset)
{
	out_be16((volatile u16 *)(base_addr + offset), b);
}

static __inline__ void usb_set_index(u8 ep)
{
	usb_writeb(ep, USB_INDEX);
}

static __inline__ void usb_setb(u8 b, unsigned long offset)
{
	u8 after = usb_readb(offset) | b;
	usb_writeb(after, offset);
}

static __inline__ void usb_setw(u16 b, unsigned long offset)
{
	u16 after = usb_readw(offset) | b;
	usb_writew(after, offset);
}

static __inline__ void usb_clearb(u8 b, unsigned long offset)
{
	u8 after = usb_readb(offset) & ~b;
	usb_writeb(after, offset);
}

static __inline__ void usb_clearw(u16 b, unsigned long offset)
{
	u16 after = usb_readw(offset) & ~b;
	usb_writew(after, offset);
}

static __inline__ int write_packet(struct musbhsfc_ep *ep,
				   struct musbhsfc_request *req, int max)
{
	u8 *buf;
	int length, count;

	buf = req->req.buf + req->req.actual;
	prefetch(buf);

	length = req->req.length - req->req.actual;
	length = min(length, max);
	req->req.actual += length;

	DEBUG("Write %d (max %d)\n", length, max);

	count = length;
	while (count--) {
		usb_writeb(*buf, ep->fifo);
		buf++;
	}

	return length;
}

/*-------------------------------------------------------------------------*/

#ifdef CONFIG_USB_GADGET_DEBUG_FILES

static const char proc_node_name[] = "driver/udc";

static int
udc_proc_read(char *page, char **start, off_t off, int count,
	      int *eof, void *_dev)
{
	char *buf = page;
	struct musbhsfc_udc *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int t;

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
		      dev->driver ? dev->driver->driver.name : "(none)",
		      "full speed");
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

unsigned char musbhsfc_check_dir(struct musbhsfc_ep *ep,
				 unsigned char usb_direction)
{
	if ((usb_direction == USB_DIR_IN) &&
	    ((ep->desc->bEndpointAddress & USB_ENDPOINT_DIR_MASK)
	     == USB_DIR_IN))
		return 1;
	else if ((usb_direction == USB_DIR_OUT) &&
	    ((ep->desc->bEndpointAddress & USB_ENDPOINT_DIR_MASK)
	     == USB_DIR_OUT))
		return 1;
	return 0;
}

void musbhsfc_show_regs(void)
{
	printk(KERN_INFO "MUSBHSFC USB device Driver - Dump Registers\n");
	printk(KERN_INFO "USB_INTRIN    : 0x%x\n", usb_readw(USB_INTRIN));
	printk(KERN_INFO "USB_POWER     : 0x%x\n", usb_readb(USB_POWER));
	printk(KERN_INFO "USB_FADDR     : 0x%x\n", usb_readb(USB_FADDR));
	printk(KERN_INFO "USB_INTRINE   : 0x%x\n", usb_readw(USB_INTRINE));
	printk(KERN_INFO "USB_INTROUT   : 0x%x\n", usb_readw(USB_INTROUT));
	printk(KERN_INFO "USB_INTRUSBE  : 0x%x\n", usb_readb(USB_INTRUSBE));
	printk(KERN_INFO "USB_INTRUSB   : 0x%x\n", usb_readb(USB_INTRUSB ));
	printk(KERN_INFO "USB_INTROUTE  : 0x%x\n", usb_readw(USB_INTROUTE));
	printk(KERN_INFO "USB_TESTMODE  : 0x%x\n", usb_readb(USB_TESTMODE));
	printk(KERN_INFO "USB_INDEX     : 0x%x\n", usb_readb(USB_INDEX));
	printk(KERN_INFO "USB_FRAME     : 0x%x\n", usb_readw(USB_FRAME));
}

void musbhsfc_set_device(struct musbhsfc_udc *dev)
{
	if (SDR_READ(DCRN_SDR_USB0) & 0x2)
		dev->usb2_device = 0;
	else
		dev->usb2_device = 1;
}

void musbhsfc_set_speed(struct musbhsfc_udc *dev)
{
	unsigned char high_speed = usb_readb(USB_POWER) & USB_POWER_HS_MODE;

	if (dev->usb2_device) {
		if (high_speed) {
			printk(KERN_INFO "USB Device 2.0 : High Speed Mode\n");
			dev->gadget.speed = USB_SPEED_HIGH;
			dev->gadget.is_dualspeed=1;
		} else {
			printk(KERN_INFO "USB Device 2.0 : Full Speed Mode\n");
			dev->gadget.speed = USB_SPEED_FULL;
			dev->gadget.is_dualspeed=1;
		}
	} else {
		printk(KERN_INFO "USB Device 1.1 : Full Speed Mode \n");
		dev->gadget.speed = USB_SPEED_FULL;
		dev->gadget.is_dualspeed=0;
	}
}

/*
 * 	udc_disable - disable USB device controller
 */
static void udc_disable(struct musbhsfc_udc *dev)
{
	DEBUG("%s, %p\n", __FUNCTION__, dev);

	udc_set_address(dev, 0);

	/* Disable interrupts */
	usb_writew(0, USB_INTRINE);
	usb_writew(0, USB_INTROUTE);
	usb_writeb(0, USB_INTRUSBE);

	/* Disable the USB */
	usb_writeb(0, USB_POWER);

	dev->ep0state = WAIT_FOR_SETUP;
	dev->gadget.speed = USB_SPEED_UNKNOWN;
	dev->usb_address = 0;

	dev->usb2_device = 0;
	dev->speed_set = 0;
}

/*
 * 	udc_reinit - initialize software state
 */
static void udc_reinit(struct musbhsfc_udc *dev)
{
	u32 i;

	DEBUG("%s, %p\n", __FUNCTION__, dev);

	/* device/ep0 records init */
	INIT_LIST_HEAD(&dev->gadget.ep_list);
	INIT_LIST_HEAD(&dev->gadget.ep0->ep_list);
	dev->ep0state = WAIT_FOR_SETUP;

	/* basic endpoint records init */
	for (i = 0; i < UDC_MAX_ENDPOINTS; i++) {
		struct musbhsfc_ep *ep = &dev->ep[i];

		if (i != 0)
			list_add_tail(&ep->ep.ep_list, &dev->gadget.ep_list);

		ep->desc = 0;
		ep->stopped = 0;
		INIT_LIST_HEAD(&ep->queue);
	}
	dev->remote_wakeup = 0;

	/* the rest was statically initialized, and is read-only */
}

#define BYTES2MAXP(x)	(x / 8)
#define MAXP2BYTES(x)	(x * 8)

/*
 * until it's enabled, this UDC should be completely invisible
 * to any USB host.
 */
static void udc_enable(struct musbhsfc_udc *dev)
{
	int ep=0;
	struct musbhsfc_ep *ep_reg = &dev->ep[ep];
	u8 csr;

	DEBUG("%s, %p\n", __FUNCTION__, dev);

	dev->usb2_device = 0;
	dev->speed_set = 0;

	dev->gadget.speed = USB_SPEED_UNKNOWN;

	usb_clearb(USB_POWER_FS_PHY_ENAB | USB_POWER_HS_ENAB, USB_POWER);

	usb_set_index(ep);

	usb_writeb(BYTES2MAXP(ep_maxpacket(ep_reg)),
				  USB_INMAXP);

	/* Read & Write CSR1, just in case */
	csr = usb_readb(ep_reg->csr1);
	usb_writeb(csr, ep_reg->csr1);

	flush(ep_reg);

	/* Clear spurious interrupts by reading these registers */
	usb_readw(USB_INTRIN);
	usb_readw(USB_INTROUT);
	usb_readb(USB_INTRUSB);

	/* Enable interrupts */
	usb_setw(USB_INTRIN_EP0, USB_INTRINE);
	usb_setb(USB_INTRUSBE_RESET | USB_INTRUSBE_RESUME |
		 USB_INTRUSBE_SUSPEND, USB_INTRUSBE);

	/* Enable SUSPEND */
	usb_setb(USB_POWER_ENABLE_SUSPEND, USB_POWER);

	musbhsfc_set_device(dev);
	if (!dev->usb2_device) {
		usb_setb(USB_POWER_FS_PHY_ENAB, USB_POWER);
		musbhsfc_set_speed(dev);
		dev->speed_set = 1;
	} else
		usb_setb(USB_POWER_HS_ENAB, USB_POWER);
}

/*
  Register entry point for the peripheral controller driver.
*/
int usb_gadget_register_driver(struct usb_gadget_driver *driver)
{
	struct musbhsfc_udc *dev = the_controller;
	int retval;

	DEBUG("%s: %s\n", __FUNCTION__, driver->driver.name);

	if (!driver
	    || (driver->speed < USB_SPEED_FULL)
	    || (driver->speed > USB_SPEED_HIGH)
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

		dev->driver = NULL;
		dev->gadget.dev.driver = NULL;
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
	struct musbhsfc_udc *dev = the_controller;
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

	DEBUG("unregistered gadget driver '%s'\n", driver->driver.name);
	return 0;
}

EXPORT_SYMBOL(usb_gadget_unregister_driver);

/*-------------------------------------------------------------------------*/

/** Write request to FIFO (max write == maxp size)
 *  Return:  0 = still running, 1 = completed, negative = errno
 *  NOTE: INDEX register must be set for EP
 */
static int write_fifo(struct musbhsfc_ep *ep, struct musbhsfc_request *req)
{
	u32 max;
	u8 csr;
	unsigned count;
	int is_last, is_short;

	max = __constant_le16_to_cpu(ep->desc->wMaxPacketSize);

	csr = usb_readb(ep->csr1);
	DEBUG("CSR: %x %d\n", csr, csr & USB_INCSR_FIFONEMPTY);


	count = write_packet(ep, req, max);
	usb_setb(USB_INCSR_INPKTRDY, ep->csr1);

	/* last packet is usually short (or a zlp) */
	if (unlikely(count != max))
		is_last = is_short = 1;
	else {
		if (likely(req->req.length != req->req.actual)
		    || req->req.zero)
			is_last = 0;
		else
			is_last = 1;
		/* interrupt/iso maxpacket may not fill the fifo */
		is_short = unlikely(max < ep_maxpacket(ep));
	}

	DEBUG("%s: wrote %s %d bytes%s%s %d left %p\n", __FUNCTION__,
	      ep->ep.name, count,
	      is_last ? "/L" : "", is_short ? "/S" : "",
	      req->req.length - req->req.actual, req);

	/* requests complete when all IN data is in the FIFO */
	if (is_last) {
		done(ep, req, 0);
		if (list_empty(&ep->queue)) {
			pio_irq_disable(ep_index(ep));
		}
		return 1;
	}

	return 0;
}

/** Read to request from FIFO (max read == bytes in fifo)
 *  Return:  0 = still running, 1 = completed, negative = errno
 *  NOTE: INDEX register must be set for EP
 */
static int read_fifo(struct musbhsfc_ep *ep, struct musbhsfc_request *req)
{
	u8 csr;
	u8 *buf;
	unsigned bufferspace, count, is_short;

	/* make sure there's a packet in the FIFO. */
	csr = usb_readb(ep->csr1);
	if (!(csr & USB_OUTCSR_OUTPKTRDY)) {
		DEBUG("%s: Packet NOT ready!\n", __FUNCTION__);
		return -EINVAL;
	}

	buf = req->req.buf + req->req.actual;
	prefetchw(buf);
	bufferspace = req->req.length - req->req.actual;

	/* read all bytes from this packet */
	count = usb_readw(USB_OUTCOUNT);
	req->req.actual += min(count, bufferspace);

	is_short = (count < ep->ep.maxpacket);
	DEBUG("read %s %02x, %d bytes%s req %p %d/%d\n",
	      ep->ep.name, csr, count,
	      is_short ? "/S" : "", req, req->req.actual, req->req.length);


	while (likely(count-- != 0)) {
		u8 byte = (u8) (usb_readb(ep->fifo) & 0xff);

		if (unlikely(bufferspace == 0)) {
			 /* this happens when the driver's buffer
			 * is smaller than what the host sent.
			 * discard the extra data.
			 */
			if (req->req.status != -EOVERFLOW)
				printk("%s overflow %d\n", ep->ep.name, count);
			req->req.status = -EOVERFLOW;
		} else {
			*buf++ = byte;
			bufferspace--;
		}
	}

	usb_clearb(USB_OUTCSR_OUTPKTRDY, ep->csr1);

	/* completion */
	if (is_short || req->req.actual == req->req.length) {
		done(ep, req, 0);
#if 0
		/*
		 * This code is excluded, but I don't want to remove it
		 * completely for now!
		 * sr, 2006-12-18
		 */
		/* double buffering requires to flush 2 packets */
		usb_setb(USB_OUTCSR_FLUSHFIFO, ep->csr1);
		/* outpktrdy bit must be cleared before flushing the 2nd packet */
		usb_clearb(USB_OUTCSR_OUTPKTRDY, ep->csr1);
		usb_setb(USB_OUTCSR_FLUSHFIFO, ep->csr1);
#endif

		if (list_empty(&ep->queue))
			pio_irq_disable(ep_index(ep));
		return 1;
	}

	/* finished that packet.  the next one may be waiting... */
	return 0;
}

/*
 *	done - retire a request; caller blocked irqs
 *  INDEX register is preserved to keep same
 */
static void done(struct musbhsfc_ep *ep, struct musbhsfc_request *req, int status)
{
	unsigned int stopped = ep->stopped;
	u32 index;

	DEBUG("%s, %p\n", __FUNCTION__, ep);
	list_del_init(&req->queue);

	if (likely(req->req.status == -EINPROGRESS))
		req->req.status = status;
	else
		status = req->req.status;

	if (status && status != -ESHUTDOWN)
		DEBUG("complete %s req %p stat %d len %u/%u\n",
		      ep->ep.name, &req->req, status,
		      req->req.actual, req->req.length);

	/* don't modify queue heads during completion callback */
	ep->stopped = 1;
	/* Read current index (completion may modify it) */
	index = usb_readb(USB_INDEX);

	spin_unlock(&ep->dev->lock);
	req->req.complete(&ep->ep, &req->req);
	spin_lock(&ep->dev->lock);

	/* Restore index */
	usb_set_index(index);
	ep->stopped = stopped;
}

/** Enable EP interrupt */
static void pio_irq_enable(int ep)
{
	DEBUG("%s: %d\n", __FUNCTION__, ep);

	switch (ep) {
	case 1:
		usb_setw(USB_INTRIN_EP1, USB_INTRINE);
		break;
	case 2:
		usb_setw(USB_INTROUT_EP2, USB_INTROUTE);
		break;
	case 3:
		usb_setw(USB_INTRIN_EP3, USB_INTRINE);
		break;
	default:
		DEBUG("Unknown endpoint: %d\n", ep);
		break;
	}
}

/** Disable EP interrupt */
static void pio_irq_disable(int ep)
{
	DEBUG("%s: %d\n", __FUNCTION__, ep);

	switch (ep) {
	case 1:
		usb_clearw(USB_INTRIN_EP1, USB_INTRINE);
		break;
	case 2:
		usb_clearw(USB_INTROUT_EP2, USB_INTROUTE);
		break;
	case 3:
		usb_clearw(USB_INTRIN_EP3, USB_INTRINE);
		break;
	default:
		DEBUG("Unknown endpoint: %d\n", ep);
		break;
	}
}

/*
 * 	nuke - dequeue ALL requests
 */
void nuke(struct musbhsfc_ep *ep, int status)
{
	struct musbhsfc_request *req;

	DEBUG("%s, %p\n", __FUNCTION__, ep);

	/* Flush FIFO */
	flush(ep);

	/* called with irqs blocked */
	while (!list_empty(&ep->queue)) {
		req = list_entry(ep->queue.next, struct musbhsfc_request, queue);
		done(ep, req, status);
	}

	/* Disable IRQ if EP is enabled (has descriptor) */
	if (ep->desc)
		pio_irq_disable(ep_index(ep));
}

/** Flush EP
 * NOTE: INDEX register must be set before this call
 */
static void flush(struct musbhsfc_ep *ep)
{
	DEBUG("%s, %p\n", __FUNCTION__, ep);

	switch (ep->ep_type) {
	case ep_control:
		break;

	case ep_bulk_in:
	case ep_interrupt:
	case ep_iso_in:
		/* double buffering requires to flush 2 packets */
		usb_setb(USB_INCSR_FLUSHFIFO, ep->csr1);
		/* inpktrdy bit must be set before flushing the 2nd packet */
		usb_setb(USB_INCSR_INPKTRDY, ep->csr1);
		usb_setb(USB_INCSR_FLUSHFIFO, ep->csr1);
		break;

	case ep_bulk_out:
	case ep_iso_out:
		/* double buffering requires to flush 2 packets */
		usb_setb(USB_OUTCSR_FLUSHFIFO, ep->csr1);
		/* outpktrdy bit must be cleared before flushing the 2nd packet */
		usb_clearb(USB_OUTCSR_OUTPKTRDY, ep->csr1);
		usb_setb(USB_OUTCSR_FLUSHFIFO, ep->csr1);
		break;
	}
}

/**
 * musbhsfc_in_epn - handle IN interrupt
 */
static void musbhsfc_in_epn(struct musbhsfc_udc *dev, u32 ep_idx, u32 intr)
{
	u8 csr;
	struct musbhsfc_ep *ep = &dev->ep[ep_idx];
	struct musbhsfc_request *req;

	usb_set_index(ep_idx);

	csr = usb_readb(ep->csr1);
	DEBUG("%s: %d, csr %x\n", __FUNCTION__, ep_idx, csr);

	if (csr & USB_INCSR_SENTSTALL) {
		DEBUG("USB_INCSR_SENTSTALL\n");
		usb_clearb(USB_INCSR_SENTSTALL,
			ep->csr1);
		return;
	}

	if (!ep->desc) {
		DEBUG("%s: NO EP DESC\n", __FUNCTION__);
		return;
	}

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct musbhsfc_request, queue);

	DEBUG("req: %p\n", req);

	if (!req)
		return;

	write_fifo(ep, req);
}

/* ********************************************************************************************* */
/* Bulk OUT (recv)
 */

static void musbhsfc_out_epn(struct musbhsfc_udc *dev, u32 ep_idx, u32 intr)
{
	struct musbhsfc_ep *ep = &dev->ep[ep_idx];
	struct musbhsfc_request *req;

	DEBUG("%s: ep_idx: %d\n", __FUNCTION__, ep_idx);

	usb_set_index(ep_idx);

	if (ep->desc) {
		u8 csr;
		csr = usb_readb(ep->csr1);

		while ((csr = usb_readb(ep-> csr1))
				      & (USB_OUTCSR_OUTPKTRDY
				       | USB_OUTCSR_SENTSTALL)) {

			DEBUG("%s: csr: %x \n", __FUNCTION__, csr);

			if (csr & USB_OUTCSR_SENTSTALL) {
				DEBUG("%s: stall sent, flush fifo\n",
				      __FUNCTION__);
				flush(ep);
				usb_clearb(USB_OUTCSR_SENTSTALL, ep->csr1);
			} else if (csr & USB_OUTCSR_OUTPKTRDY) {
				if (list_empty(&ep->queue))
					req = 0;
				else
					req =
					    list_entry(ep->queue.next,
						       struct musbhsfc_request,
						       queue);

				if (!req) {
					DEBUG("%s: NULL REQ %d\n",
					       __FUNCTION__, ep_idx);
					flush(ep);
					break;
				} else {
					read_fifo(ep, req);
				}
			}

		}

	} else {
		/* Throw packet away.. */
		printk("%s: No descriptor?!?\n", __FUNCTION__);
		flush(ep);
	}
}

static void stop_activity(struct musbhsfc_udc *dev,
			  struct usb_gadget_driver *driver)
{
	int i;

	/* don't disconnect drivers more than once */
	if (dev->gadget.speed == USB_SPEED_UNKNOWN)
		driver = 0;
	dev->gadget.speed = USB_SPEED_UNKNOWN;

	/* prevent new request submissions, kill any outstanding requests  */
	for (i = 0; i < UDC_MAX_ENDPOINTS; i++) {
		struct musbhsfc_ep *ep = &dev->ep[i];
		ep->stopped = 1;

		usb_set_index(i);
		nuke(ep, -ESHUTDOWN);
	}

	/* report disconnect; the driver is already quiesced */
	if (driver) {
		spin_unlock(&dev->lock);
		driver->disconnect(&dev->gadget);
		spin_lock(&dev->lock);
	}

	/* re-init driver-visible data structures */
	udc_reinit(dev);
}

/*
 * Handle USB RESET interrupt
 */
static void musbhsfc_reset_intr(struct musbhsfc_udc *dev)
{
	musbhsfc_set_speed(dev);
	dev->ep0state = WAIT_FOR_SETUP;
	udc_set_address(dev, 0);
	dev->remote_wakeup=0;
}

/*
 *	musbhsfc usb client interrupt handler.
 */
static irqreturn_t musbhsfc_udc_irq(int irq, void *_dev)
{
	struct musbhsfc_udc *dev = _dev;

	DEBUG("\n\n");

	spin_lock(&dev->lock);

	for (;;) {

		u32 intr_in = usb_readw(USB_INTRIN);
		u32 intr_out = usb_readw(USB_INTROUT);
		u32 intr_int = usb_readb(USB_INTRUSB);

		u32 in_en = usb_readw(USB_INTRINE);
		u32 out_en = usb_readw(USB_INTROUTE);


		if (!intr_out && !intr_in && !intr_int)
			break;

		DEBUG("%s (on state %s)\n", __FUNCTION__,
		      state_names[dev->ep0state]);
		DEBUG("intr_out = %x\n", intr_out);
		DEBUG("intr_in  = %x\n", intr_in);
		DEBUG("intr_int = %x\n", intr_int);


		if (intr_int & USB_INTRUSB_RESUME) {
			DEBUG("USB resume\n");
			if (dev->gadget.speed != USB_SPEED_UNKNOWN
			    && dev->driver
			    && dev->driver->resume) {
				dev->driver->resume(&dev->gadget);
			}
		}
		if (intr_int & USB_INTRUSB_RESET) {
			DEBUG("USB reset\n");
			musbhsfc_reset_intr(dev);
			dev->speed_set = 0;
		}
		if (intr_in & USB_INTRIN_EP0) {
			DEBUG("USB_INTRIN_EP0 (control)\n");
			if (dev->usb2_device &&
			    (dev->speed_set == 0)) {
				musbhsfc_set_speed(dev);
				dev->speed_set = 1;
			}
			musbhsfc_handle_ep0(dev, intr_in);
		}
		if (intr_out & USB_INTROUT_EP0) {
			DEBUG("USB_INTROUT_EP0 (control) \n");
			if (dev->usb2_device &&
			    (dev->speed_set == 0)) {
				musbhsfc_set_speed(dev);
				dev->speed_set = 1;
			}
			musbhsfc_handle_ep0(dev, intr_out);
		}
		if ((intr_in & USB_INTRIN_EP1)
		    && (in_en & USB_INTRIN_EP1)) {
			DEBUG("USB_INTRIN_EP1\n");
			musbhsfc_in_epn(dev, 1, intr_in);
		}
		if ((intr_in & USB_INTRIN_EP2)
		    && (in_en & USB_INTRIN_EP2)) {
			DEBUG("USB_INTRIN_EP2\n");
			musbhsfc_in_epn(dev, 2, intr_int);
		}
		if ((intr_in & USB_INTRIN_EP3)
		    && (in_en & USB_INTRIN_EP3)) {
			DEBUG("USB_INTRIN_EP3\n");
			musbhsfc_in_epn(dev, 3, intr_in);
		}
		if ((intr_out & USB_INTROUT_EP1)
		    && (out_en & USB_INTROUT_EP1)) {
			DEBUG("USB_INTROUT_EP1\n");
			musbhsfc_out_epn(dev, 1, intr_out);
		}
		if ((intr_out & USB_INTROUT_EP2)
		    && (out_en & USB_INTROUT_EP2)) {
			DEBUG("USB_INTROUT_EP2\n");
			musbhsfc_out_epn(dev, 2, intr_out);
		}
		if ((intr_out & USB_INTROUT_EP3)
		    && (out_en & USB_INTROUT_EP3)) {
			DEBUG("USB_INTROUT_EP3\n");
			musbhsfc_out_epn(dev, 3, intr_out);
		}
		if (intr_int & USB_INTRUSB_SUSPEND) {
			DEBUG("USB suspend\n");
			if (dev->gadget.speed !=
			    USB_SPEED_UNKNOWN && dev->driver
			    && dev->driver->suspend && dev->remote_wakeup) {
				dev->driver->suspend(&dev->gadget);
			}
		}
	}

	spin_unlock(&dev->lock);
	DEBUG("%s: IRQ_HANDLED\n", __FUNCTION__);

	return IRQ_HANDLED;
}

static int musbhsfc_ep_enable(struct usb_ep *_ep,
			     const struct usb_endpoint_descriptor *desc)
{
	struct musbhsfc_ep *ep;
	struct musbhsfc_udc *dev;
	unsigned long flags;
	u16 max_packet_size = __constant_le16_to_cpu(desc->wMaxPacketSize);

	DEBUG("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (!_ep || !desc || ep->desc || _ep->name == ep0name
	    || desc->bDescriptorType != USB_DT_ENDPOINT
	    || ep->bEndpointAddress != desc->bEndpointAddress
	    || MAX_FIFO_SIZE < max_packet_size
	    || !max_packet_size)
	{
		DEBUG("%s, bad ep or descriptor\n", __FUNCTION__);
		DEBUG("desc:%p\n",desc);
		DEBUG("ep->desc:%p\n",ep->desc);
		DEBUG("_ep->name:%s\n",_ep->name);
		DEBUG("desc->bDescriptorType:%x (must be 5)\n",desc->bDescriptorType);
		DEBUG("ep->bEndpointAddress:%x (must be equal to) desc->bEndpointAddress:%x\n",
			ep->bEndpointAddress,desc->bEndpointAddress);
		DEBUG("MAX_FIFO_SIZE:%x (must be greater or equal to)  max_packet_size:%x\n",
		       MAX_FIFO_SIZE,max_packet_size);
		return -EINVAL;
	}


	dev = ep->dev;
	if (!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN) {
		DEBUG("%s, bogus device state\n", __FUNCTION__);
		return -ESHUTDOWN;
	}

	spin_lock_irqsave(&ep->dev->lock, flags);

	ep->stopped = 0;
	dev->remote_wakeup = 0;
	ep->desc = desc;
	ep->ep.maxpacket = max_packet_size;
	usb_set_index(ep_index(ep));

	if (musbhsfc_check_dir(ep, USB_DIR_OUT)) {
		if (ep != 0) {
			ep->csr1 = USB_OUTCSR;
			ep->csr2 = USB_OUTCSRH;
		}
		switch (ep->desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) {
		case USB_ENDPOINT_XFER_BULK:
			usb_clearb(USB_OUTCSRH_DMAENA |
				  USB_OUTCSRH_AUTOCLR, ep->csr2);
			ep->ep_type = ep_bulk_out;
			break;
		case USB_ENDPOINT_XFER_INT:
			usb_clearb(USB_OUTCSRH_DMAENA |
				  USB_OUTCSRH_AUTOCLR, ep->csr2);
			usb_setb(USB_OUTCSRH_DISNYET, ep->csr2);

			break;
		case USB_ENDPOINT_XFER_ISOC:
			usb_setb(USB_OUTCSRH_ISO, ep->csr2);
			ep->ep_type = ep_iso_out;
			break;
		default:
			DEBUG("Invalid bmAttributes (OUT):%x\n",
		    		     ep->desc->bmAttributes);
			return -EINVAL;
		}
		usb_writew(max_packet_size, USB_OUTMAXP);
	}

	if (musbhsfc_check_dir(ep, USB_DIR_IN)) {
		if (ep != 0) {
			ep->csr1 = USB_INCSR;
			ep->csr2 = USB_INCSRH;
		}
		switch (ep->desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) {
		case USB_ENDPOINT_XFER_BULK:
			usb_clearb(USB_INCSRH_DMAENA | USB_INCSRH_AUTOSET,
				  ep->csr2);
			usb_setb(USB_INCSRH_MODE, ep->csr2);
			ep->ep_type = ep_bulk_in;

			break;
		case USB_ENDPOINT_XFER_INT:
			usb_clearb(USB_INCSRH_DMAENA | USB_INCSRH_AUTOSET,
				  ep->csr2);
			usb_setb(USB_INCSRH_MODE, ep->csr2);
			ep->ep_type = ep_interrupt;

			break;
		case USB_ENDPOINT_XFER_ISOC:
			usb_setb(USB_INCSRH_MODE | USB_INCSRH_ISO, ep->csr2);
			ep->ep_type = ep_iso_in;
			break;
		default:
			DEBUG("Invalid bmAttributes (IN):%x\n",
		    		     ep->desc->bmAttributes);
			return -EINVAL;
		}
		usb_writew(max_packet_size, USB_INMAXP);
	}

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	/* Reset halt state (does flush) */
	musbhsfc_set_halt(_ep, 0);

	DEBUG("%s: enabled %s\n", __FUNCTION__, _ep->name);
	return 0;
}

/** Disable EP
 *  NOTE: Sets INDEX register
 */
static int musbhsfc_ep_disable(struct usb_ep *_ep)
{
	struct musbhsfc_ep *ep;
	unsigned long flags;

	DEBUG("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (!_ep || !ep->desc) {
		DEBUG("%s, %s not enabled\n", __FUNCTION__,
		      _ep ? ep->ep.name : NULL);
		return -EINVAL;
	}

	spin_lock_irqsave(&ep->dev->lock, flags);

	usb_set_index(ep_index(ep));

	/* Nuke all pending requests (does flush) */
	nuke(ep, -ESHUTDOWN);

	/* Disable ep IRQ */
	pio_irq_disable(ep_index(ep));

	ep->desc = 0;
	ep->stopped = 1;

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	DEBUG("%s: disabled %s\n", __FUNCTION__, _ep->name);
	return 0;
}

static struct usb_request *musbhsfc_alloc_request(struct usb_ep *ep,
						 unsigned gfp_flags)
{
	struct musbhsfc_request *req;

	DEBUG("%s, %p\n", __FUNCTION__, ep);

	req = kzalloc(sizeof(*req), gfp_flags);
	if (!req)
		return 0;
	INIT_LIST_HEAD(&req->queue);

	return &req->req;
}

static void musbhsfc_free_request(struct usb_ep *ep, struct usb_request *_req)
{
	struct musbhsfc_request *req;

	DEBUG("%s, %p\n", __FUNCTION__, ep);

	req = container_of(_req, struct musbhsfc_request, req);
	WARN_ON(!list_empty(&req->queue));
	kfree(req);
}

static void *musbhsfc_alloc_buffer(struct usb_ep *_ep, unsigned bytes,
				  dma_addr_t * dma, unsigned gfp_flags)
{
	struct musbhsfc_ep *ep;
	char * retval;

	ep = container_of(_ep, struct musbhsfc_ep, ep);

	DEBUG("%s (%p, %d, %d)\n", __FUNCTION__, ep, bytes, gfp_flags);

	retval = kmalloc(bytes, gfp_flags & ~(__GFP_DMA | __GFP_HIGHMEM));
	if (retval)
		*dma = virt_to_phys(retval);
	return retval;
}

static void musbhsfc_free_buffer(struct usb_ep *_ep, void *buf, dma_addr_t dma,
				unsigned bytes)
{
	struct musbhsfc_ep *ep;

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	DEBUG("%s, %p\n", __FUNCTION__, _ep);

	kfree(buf);

}

/** Queue one request
 *  Kickstart transfer if needed
 *  NOTE: Sets INDEX register
 */
static int musbhsfc_queue(struct usb_ep *_ep, struct usb_request *_req,
			 unsigned gfp_flags)
{
	struct musbhsfc_request *req;
	struct musbhsfc_ep *ep;
	struct musbhsfc_udc *dev;
	unsigned long flags;

	DEBUG("\n\n\n%s, %p\n", __FUNCTION__, _ep);

	req = container_of(_req, struct musbhsfc_request, req);
	if (unlikely
	    (!_req || !_req->complete || !_req->buf
	     || !list_empty(&req->queue))) {
		DEBUG("%s, bad params\n", __FUNCTION__);
		return -EINVAL;
	}

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (unlikely(!_ep || (!ep->desc && ep->ep.name != ep0name))) {
		DEBUG("%s, bad ep\n", __FUNCTION__);
		return -EINVAL;
	}

	dev = ep->dev;
	if (unlikely(!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN)) {
		DEBUG("%s, bogus device state %p\n", __FUNCTION__, dev->driver);
		return -ESHUTDOWN;
	}

	DEBUG("%s queue req %p, len %d buf %p\n", _ep->name, _req, _req->length,
	      _req->buf);

	spin_lock_irqsave(&dev->lock, flags);

	_req->status = -EINPROGRESS;
	_req->actual = 0;

	/* kickstart this i/o queue? */
	DEBUG("Add to %d Q %d %d\n", ep_index(ep), list_empty(&ep->queue),
	      ep->stopped);
	if (list_empty(&ep->queue) && likely(!ep->stopped)) {
		u8 csr;

		if (ep_index(ep) == 0) {
			/* EP0 */
			list_add_tail(&req->queue, &ep->queue);
			musbhsfc_ep0_kick(dev, ep);
			req = 0;
		} else if (musbhsfc_check_dir(ep, USB_DIR_IN)) {
			usb_set_index(ep_index(ep));
			csr = usb_readb(ep->csr1);
			pio_irq_enable(ep_index(ep));
			if ((csr & USB_INCSR_FIFONEMPTY) == 0) {
				if (write_fifo(ep, req) == 1)
					req = 0;
			}
		} else if (musbhsfc_check_dir(ep, USB_DIR_OUT)){
			usb_set_index(ep_index(ep));
			csr = usb_readb(ep->csr1);
			pio_irq_enable(ep_index(ep));

			if (csr & USB_OUTCSR_OUTPKTRDY) {
				if (read_fifo(ep, req) == 1)
					req = 0;
			}
		}
	}

	/* pio or dma irq handler advances the queue. */
	if (likely(req != 0))
		list_add_tail(&req->queue, &ep->queue);

	spin_unlock_irqrestore(&dev->lock, flags);

	return 0;
}

/* dequeue JUST ONE request */
static int musbhsfc_dequeue(struct usb_ep *_ep, struct usb_request *_req)
{
	struct musbhsfc_ep *ep;
	struct musbhsfc_request *req;
	unsigned long flags;

	DEBUG("%s, %p\n", __FUNCTION__, _ep);

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (!_ep || ep->ep.name == ep0name)
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

	spin_unlock_irqrestore(&ep->dev->lock, flags);
	return 0;
}

/** Halt specific EP
 *  Return 0 if success
 *  NOTE: Sets INDEX register to EP !
 */
static int musbhsfc_set_halt(struct usb_ep *_ep, int value)
{
	struct musbhsfc_ep *ep;
	unsigned long flags;

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (unlikely(!_ep || (!ep->desc && ep->ep.name != ep0name))) {
		DEBUG("%s, bad ep\n", __FUNCTION__);
		return -EINVAL;
	}

	usb_set_index(ep_index(ep));

	DEBUG("%s, ep %d, val %d\n", __FUNCTION__, ep_index(ep), value);

	spin_lock_irqsave(&ep->dev->lock, flags);

	if (ep_index(ep) == 0) {
		/* EP0 */
		usb_setb(USB_CSR0_SENDSTALL, ep->csr1);
	} else if (musbhsfc_check_dir(ep, USB_DIR_IN)) {
		u8 csr = usb_readb(ep->csr1);

		if (value && ((csr & USB_INCSR_FIFONEMPTY)
			      || !list_empty(&ep->queue))) {
			/*
			 * Attempts to halt IN endpoints will fail (returning -EAGAIN)
			 * if any transfer requests are still queued, or if the controller
			 * FIFO still holds bytes that the host hasn't collected.
			 */
			spin_unlock_irqrestore(&ep->dev->lock, flags);
			DEBUG
			    ("Attempt to halt IN endpoint failed (returning -EAGAIN) %d %d\n",
			     (csr & USB_INCSR_FIFONEMPTY),
			     !list_empty(&ep->queue));
			return -EAGAIN;
		}

		flush(ep);
		if (value) {
			usb_setb(USB_INCSR_SENDSTALL, ep->csr1);
		}
		else {
			usb_clearb(USB_INCSR_SENDSTALL, ep->csr1);
			usb_setb(USB_INCSR_CLRDATATOG, ep->csr1);
		}

	} else if (musbhsfc_check_dir(ep, USB_DIR_OUT)) {

		flush(ep);
		if (value) {

			usb_setb(USB_OUTCSR_SENDSTALL, ep->csr1);
		}
		else {
			usb_clearb(USB_OUTCSR_SENDSTALL, ep->csr1);
			usb_setb(USB_OUTCSR_CLRDATATOG, ep->csr1);
		}
	}

	if (value) {
		ep->stopped = 1;
	} else {
		ep->stopped = 0;
	}

	spin_unlock_irqrestore(&ep->dev->lock, flags);

	DEBUG("%s %s halted\n", _ep->name, value == 0 ? "NOT" : "IS");

	return 0;
}

/** Return bytes in EP FIFO
 *  NOTE: Sets INDEX register to EP
 */
static int musbhsfc_fifo_status(struct usb_ep *_ep)
{
	u8 csr;
	int count = 0;
	struct musbhsfc_ep *ep;

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (!_ep) {
		DEBUG("%s, bad ep\n", __FUNCTION__);
		return -ENODEV;
	}

	DEBUG("%s, %d\n", __FUNCTION__, ep_index(ep));

	/* LPD can't report unclaimed bytes from IN fifos */
	if (musbhsfc_check_dir(ep, USB_DIR_IN))
		return -EOPNOTSUPP;

	usb_set_index(ep_index(ep));

	csr = usb_readb(ep->csr1);
	if (ep->dev->gadget.speed != USB_SPEED_UNKNOWN ||
	    csr & USB_OUTCSR_OUTPKTRDY) {
		count = usb_readw(USB_OUTCOUNT);
	}

	return count;
}

/** Flush EP FIFO
 *  NOTE: Sets INDEX register to EP
 */
static void musbhsfc_fifo_flush(struct usb_ep *_ep)
{
	struct musbhsfc_ep *ep;

	ep = container_of(_ep, struct musbhsfc_ep, ep);
	if (unlikely(!_ep || (!ep->desc && ep->ep.name != ep0name))) {
		DEBUG("%s, bad ep\n", __FUNCTION__);
		return;
	}

	usb_set_index(ep_index(ep));
	flush(ep);
}

/****************************************************************/
/* End Point 0 related functions                                */
/****************************************************************/

/* return:  0 = still running, 1 = completed, negative = errno */
static int write_fifo_ep0(struct musbhsfc_ep *ep, struct musbhsfc_request *req)
{
	u32 max;
	unsigned count;
	int is_last;

	max = ep_maxpacket(ep);

	DEBUG_EP0("%s\n", __FUNCTION__);

	count = write_packet(ep, req, max);

	/* last packet is usually short (or a zlp) */
	if (unlikely(count != max))
		is_last = 1;
	else {
		if (likely(req->req.length != req->req.actual) || req->req.zero)
			is_last = 0;
		else
			is_last = 1;
	}

	DEBUG_EP0("%s: wrote %s %d bytes%s %d left %p\n", __FUNCTION__,
		  ep->ep.name, count,
		  is_last ? "/L" : "", req->req.length - req->req.actual, req);

	/* requests complete when all IN data is in the FIFO */
	if (is_last) {
		done(ep, req, 0);
		return 1;
	}

	return 0;
}

static __inline__ int musbhsfc_fifo_read(struct musbhsfc_ep *ep,
					unsigned char *cp, int max)
{
	int bytes;
	int count = usb_readw(USB_OUTCOUNT);

	if (count > max)
		count = max;
	bytes = count;
	while (count--)
		*cp++ = usb_readb(ep->fifo) & 0xFF;
	return bytes;
}

static __inline__ void musbhsfc_fifo_write(struct musbhsfc_ep *ep,
					  unsigned char *cp, int count)
{
	DEBUG_EP0("fifo_write: %d %d\n", ep_index(ep), count);
	while (count--) {
		usb_writeb(*cp, ep->fifo);
		cp++;
	}
}

static int read_fifo_ep0(struct musbhsfc_ep *ep, struct musbhsfc_request *req)
{
	u8 csr;
	u8 *buf;
	unsigned bufferspace, count, is_short;

	DEBUG_EP0("%s\n", __FUNCTION__);

	csr = usb_readb(USB_CSR0);
	if (!(csr & USB_OUTCSR_OUTPKTRDY))
		return 0;

	buf = req->req.buf + req->req.actual;
	prefetchw(buf);
	bufferspace = req->req.length - req->req.actual;

	/* read all bytes from this packet */
	if (likely(csr & USB_CSR0_OUTPKTRDY)) {
		count = usb_readw(USB_OUTCOUNT);
		req->req.actual += min(count, bufferspace);
	} else			/* zlp */
		count = 0;

	is_short = (count < ep->ep.maxpacket);
	DEBUG_EP0("read %s %02x, %d bytes%s req %p %d/%d\n",
		  ep->ep.name, csr, count,
		  is_short ? "/S" : "", req, req->req.actual, req->req.length);

	while (likely(count-- != 0)) {
		u8 byte = (u8) (usb_readb(ep->fifo) & 0xff);

		if (unlikely(bufferspace == 0)) {
			/* this happens when the driver's buffer
			 * is smaller than what the host sent.
			 * discard the extra data.
			 */
			if (req->req.status != -EOVERFLOW)
				DEBUG_EP0("%s overflow %d\n", ep->ep.name,
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
static void udc_set_address(struct musbhsfc_udc *dev, unsigned char address)
{
	DEBUG_EP0("%s: %d\n", __FUNCTION__, address);
	dev->usb_address = address;
	usb_setb((address & USB_FADDR_ADDR_MASK), USB_FADDR);
	usb_setb(USB_FADDR_UPDATE | (address & USB_FADDR_ADDR_MASK), USB_FADDR);
}

/*
 * DATA_STATE_RECV (OUTPKTRDY)
 *      - if error
 *      	clear USB_CSR0_OUTPKTRDY
 *              set USB_CSR0_DATAEND | USB_CSR0_SENDSTALL bits
 *      - else
 *              clear USB_CSR0_OUTPKTRDY bit
 				if last set USB_CSR0_DATAEND bit
 */
static void musbhsfc_ep0_out(struct musbhsfc_udc *dev, u8 csr)
{
	struct musbhsfc_request *req;
	struct musbhsfc_ep *ep = &dev->ep[0];
	int ret;

	DEBUG_EP0("%s: %x\n", __FUNCTION__, csr);

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct musbhsfc_request, queue);

	if (req) {

		if (req->req.length == 0) {
			DEBUG_EP0("ZERO LENGTH OUT!\n");
			usb_setb(USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND,
				 USB_CSR0);
			dev->ep0state = WAIT_FOR_SETUP;
			return;
		}
		ret = read_fifo_ep0(ep, req);
		if (ret) {
			/* Done! */
			DEBUG_EP0("%s: finished, waiting for status\n",
				  __FUNCTION__);

			usb_setb(USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND,
				 USB_CSR0);
			dev->ep0state = WAIT_FOR_SETUP;
		} else {
			/* Not done yet.. */
			DEBUG_EP0("%s: not finished\n", __FUNCTION__);
			usb_setb(USB_CSR0_SVDOUTPKTRDY, USB_CSR0);
		}
	} else {
		DEBUG_EP0("NO REQ??!\n");
	}
}

/*
 * DATA_STATE_XMIT
 */
static int musbhsfc_ep0_in(struct musbhsfc_udc *dev, u8 csr)
{
	struct musbhsfc_request *req;
	struct musbhsfc_ep *ep = &dev->ep[0];
	int ret, need_zlp = 0;

	DEBUG_EP0("%s: %x\n", __FUNCTION__, csr);

	if (list_empty(&ep->queue))
		req = 0;
	else
		req = list_entry(ep->queue.next, struct musbhsfc_request, queue);

	if (!req) {
		DEBUG_EP0("%s: NULL REQ\n", __FUNCTION__);
		return 0;
	}

	if (req->req.length == 0) {

		usb_setb((USB_CSR0_INPKTRDY | USB_CSR0_DATAEND), USB_CSR0);
		dev->ep0state = WAIT_FOR_SETUP;
		return 1;
	}

	if (req->req.length - req->req.actual == EP0_PACKETSIZE) {
		/* Next write will end with the packet size, */
		/* so we need Zero-length-packet */
		need_zlp = 1;
	}

	ret = write_fifo_ep0(ep, req);

	if (ret == 1 && !need_zlp) {
		/* Last packet */
		DEBUG_EP0("%s: finished, waiting for status\n", __FUNCTION__);

		usb_setb((USB_CSR0_INPKTRDY | USB_CSR0_DATAEND), USB_CSR0);
		dev->ep0state = WAIT_FOR_SETUP;
	} else {
		DEBUG_EP0("%s: not finished\n", __FUNCTION__);
		usb_setb(USB_CSR0_INPKTRDY, USB_CSR0);
	}

	if (need_zlp) {
		DEBUG_EP0("%s: Need ZLP!\n", __FUNCTION__);
		usb_setb(USB_CSR0_INPKTRDY, USB_CSR0);
		dev->ep0state = DATA_STATE_NEED_ZLP;
	}

	return 1;
}

static int musbhsfc_handle_get_status(struct musbhsfc_udc *dev,
				     struct usb_ctrlrequest *ctrl)
{
	struct musbhsfc_ep *ep0 = &dev->ep[0];
	struct musbhsfc_ep *qep;
	int reqtype = (ctrl->bRequestType & USB_RECIP_MASK);
	u16 val = 0;

	int ep_num = ((__le16_to_cpu(ctrl->wIndex)) & ~USB_DIR_IN);

	if (reqtype == USB_RECIP_INTERFACE) {
		/* This is not supported.
		 * And according to the USB spec, this one does nothing..
		 * Just return 0
		 */
		DEBUG_SETUP("GET_STATUS: USB_RECIP_INTERFACE\n");
	} else if (reqtype == USB_RECIP_DEVICE) {
		DEBUG_SETUP("GET_STATUS: USB_RECIP_DEVICE\n");
		val |= (1 << 0);	           /* Self powered */
		val |= ((dev->remote_wakeup)<< 1); /* Remote wakeup */
		DEBUG("Remote wakeup status: %d\n",dev->remote_wakeup);
	} else if (reqtype == USB_RECIP_ENDPOINT) {

		DEBUG_SETUP
		    ("GET_STATUS: USB_RECIP_ENDPOINT (%d), __le16_to_cpu(ctrl->wLength) = %d\n",
		     ep_num, __le16_to_cpu(ctrl->wLength));

		if (__le16_to_cpu(ctrl->wLength) > 2 || ep_num > (UDC_MAX_ENDPOINTS-1))
			return -EOPNOTSUPP;

		qep = &dev->ep[ep_num];
		if ( ep_index(qep) != 0  && musbhsfc_check_dir(qep, USB_DIR_IN)
		    != ((__le16_to_cpu(ctrl->wIndex) & USB_DIR_IN) ? 1 : 0)
		    ) {
			return -EOPNOTSUPP;
		}

		usb_set_index(ep_index(qep));

		/* Return status on next IN token */
		switch (qep->ep_type) {
		case ep_control:
			val =
			    (usb_readb(qep->csr1) & USB_CSR0_SENDSTALL) ==
			    USB_CSR0_SENDSTALL;
			break;
		case ep_bulk_in:
		case ep_interrupt:
		case ep_iso_in:
			val =
			    (usb_readb(qep->csr1) & USB_INCSR_SENDSTALL) ==
			    USB_INCSR_SENDSTALL;
			break;
		case ep_bulk_out:
		case ep_iso_out:
			val =
			    (usb_readb(qep->csr1) & USB_OUTCSR_SENDSTALL) ==
			    USB_OUTCSR_SENDSTALL;
			break;
		}

		/* Back to EP0 index */
		usb_set_index(0);

	} else {
		DEBUG_SETUP("Unknown REQ TYPE: %d\n", reqtype);
		return -EOPNOTSUPP;
	}

	DEBUG_SETUP("GET_STATUS, ep: %d (%x), val = %d\n", ep_num,
			    __le16_to_cpu(ctrl->wIndex), val);

	val=__le16_to_cpu(val);

	/* Clear "out packet ready" */
	usb_setb(USB_CSR0_SVDOUTPKTRDY, USB_CSR0);
	/* Put status to FIFO */
	musbhsfc_fifo_write(ep0, (u8 *) & val, sizeof(val));
	/* Issue "In packet ready" */
	usb_setb((USB_CSR0_INPKTRDY | USB_CSR0_DATAEND), USB_CSR0);

	return 0;
}

/*
 * WAIT_FOR_SETUP (OUTPKTRDY)
 *      - read data packet from EP0 FIFO
 *      - decode command
 *      - if error
 *              clear USB_CSR0_OUTPKTRDY
 *              set USB_CSR0_DATAEND | USB_CSR0_SENDSTALL bits
 *      - else
 *              clear USB_CSR0_OUTPKTRDY
 *              set USB_CSR0_DATAEND bits
 */
static void musbhsfc_ep0_setup(struct musbhsfc_udc *dev, u8 csr)
{
	struct musbhsfc_ep *ep = &dev->ep[0];
	struct usb_ctrlrequest ctrl;
	int i, bytes, is_in;
	int ep_num;

	DEBUG_SETUP("%s: %x\n", __FUNCTION__, csr);

	/* Nuke all previous transfers */
	nuke(ep, -EPROTO);

	/* read control req from fifo (8 bytes) */
	bytes = musbhsfc_fifo_read(ep, (unsigned char *)&ctrl, 8);

	ep_num = (__le16_to_cpu(ctrl.wIndex) & 0x0f);

	DEBUG_SETUP("Read CTRL REQ %d bytes\n", bytes);
	DEBUG_SETUP("CTRL.bRequestType = %d (is_in %d)\n", ctrl.bRequestType,
		    ctrl.bRequestType == USB_DIR_IN);
	DEBUG_SETUP("CTRL.bRequest = %d\n", ctrl.bRequest);
	DEBUG_SETUP("CTRL.wLength = %d\n", __le16_to_cpu(ctrl.wLength));
	DEBUG_SETUP("CTRL.wValue = %d (%d)\n", __le16_to_cpu(ctrl.wValue),
		     __le16_to_cpu(ctrl.wValue) >> 8);
	DEBUG_SETUP("CTRL.wIndex = %d\n", __le16_to_cpu(ctrl.wIndex));

	/* Set direction of EP0 */
	if (likely(ctrl.bRequestType & USB_DIR_IN)) {
		ep->bEndpointAddress |= USB_DIR_IN;
		is_in = 1;
	} else {
		ep->bEndpointAddress &= ~USB_DIR_IN;
		is_in = 0;
	}

	/* Handle some SETUP packets ourselves */
	switch (ctrl.bRequest) {
	case USB_REQ_SET_ADDRESS:
		if (ctrl.bRequestType != (USB_TYPE_STANDARD | USB_RECIP_DEVICE))
			break;

		DEBUG_SETUP("USB_REQ_SET_ADDRESS (%d)\n",
			    __le16_to_cpu(ctrl.wValue));
		udc_set_address(dev, __le16_to_cpu(ctrl.wValue));
		usb_setb(USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND,
			 USB_CSR0);
		return;

	case USB_REQ_GET_STATUS:{
			if (musbhsfc_handle_get_status(dev, &ctrl) == 0)
				return;

	case USB_REQ_CLEAR_FEATURE:
	case USB_REQ_SET_FEATURE:
			if (ctrl.bRequestType == USB_RECIP_ENDPOINT) {
				struct musbhsfc_ep *qep;

				/* Support only HALT feature */
				if (__le16_to_cpu(ctrl.wValue) != 0
				    || __le16_to_cpu(ctrl.wLength) != 0
				    || ep_num > 3 || ep_num < 1)
					break;

				qep = &dev->ep[ep_num];
				spin_unlock(&dev->lock);
				if (ctrl.bRequest == USB_REQ_SET_FEATURE) {
					DEBUG_SETUP("SET_FEATURE ENDPOINT_HALT(%d)\n",
						    ep_num);
					musbhsfc_set_halt(&qep->ep, 1);
				} else {
					DEBUG_SETUP("CLR_FEATURE ENDPOINT_HALT(%d)\n",
						    ep_num);
					musbhsfc_set_halt(&qep->ep, 0);
				}
				spin_lock(&dev->lock);
				usb_set_index(0);

				/* Reply with a ZLP on next IN token */
				usb_setb(USB_CSR0_SVDOUTPKTRDY |
					 USB_CSR0_DATAEND, USB_CSR0);
				return;
			}

			if (ctrl.bRequestType == USB_RECIP_DEVICE) {
				if (ctrl.bRequest == USB_REQ_SET_FEATURE) {
					DEBUG_SETUP("SET_FEATURE DEVICE_REMOTE_WAKEUP(%d)\n",
						    ep_num);
					dev->remote_wakeup=1;

				} else {
					DEBUG_SETUP("CLR_FEATURE DEVICE_REMOTE_WAKEUP(%d)\n",
						    ep_num);
					dev->remote_wakeup=0;
				}
				usb_set_index(0);

				/* Reply with a ZLP on next IN token */
				usb_setb(USB_CSR0_SVDOUTPKTRDY |
					 USB_CSR0_DATAEND, USB_CSR0);
				return;

			}
			break;
		}

	default:
		break;
	}

	if (likely(dev->driver)) {
		/* device-2-host (IN) or no data setup command, process immediately */
		spin_unlock(&dev->lock);
		i = dev->driver->setup(&dev->gadget, &ctrl);
		spin_lock(&dev->lock);

		if (i < 0) {
			/* setup processing failed, force stall */
			DEBUG_SETUP
			    ("  --> ERROR: gadget setup FAILED (stalling), setup returned %d\n",
			     i);
			usb_set_index(0);
			usb_setb((USB_CSR0_SVDOUTPKTRDY | USB_CSR0_DATAEND |
				  USB_CSR0_SENDSTALL), USB_CSR0);

			/* ep->stopped = 1; */
			dev->ep0state = WAIT_FOR_SETUP;
		}
	}
}

/*
 * DATA_STATE_NEED_ZLP
 */
static void musbhsfc_ep0_in_zlp(struct musbhsfc_udc *dev, u8 csr)
{
	DEBUG_EP0("%s: %x\n", __FUNCTION__, csr);

	usb_setb((USB_CSR0_INPKTRDY | USB_CSR0_DATAEND), USB_CSR0);
	dev->ep0state = WAIT_FOR_SETUP;
}

/*
 * handle ep0 interrupt
 */
static void musbhsfc_handle_ep0(struct musbhsfc_udc *dev, u32 intr)
{
	struct musbhsfc_ep *ep = &dev->ep[0];
	u8 csr;

	/* Set index 0 */
	usb_set_index(0);
 	csr = usb_readb(USB_CSR0);

	DEBUG_EP0("%s: csr = %x\n", __FUNCTION__, csr);

	/*
	 * if SENTSTALL is set
	 *      - clear the SENTSTALL bit
	 */
	if (csr & USB_CSR0_SENTSTALL) {
		DEBUG_EP0("%s: USB_CSR0_SENTSTALL is set: %x\n", __FUNCTION__, csr);
		usb_clearb((USB_CSR0_SENTSTALL | USB_CSR0_SENDSTALL),
			  USB_CSR0);
		nuke(ep, -ECONNABORTED);
		dev->ep0state = WAIT_FOR_SETUP;
		return;
	}

	/*
	 * if SETUPEND is set
	 *      - abort the last transfer
	 *      - set SVDSETUPEND
	 */
	if (csr & USB_CSR0_SETUPEND) {
		DEBUG_EP0("%s: USB_CSR0_SETUPEND is set: %x\n", __FUNCTION__, csr);

		usb_setb(USB_CSR0_SVDSETUPEND, USB_CSR0);

		nuke(ep, 0);
		dev->ep0state = WAIT_FOR_SETUP;
	}



	/*
	 * if a transfer is in progress && INPKTRDY and OUTPKTRDY are clear
	 *      - fill EP0 FIFO
	 *      - if last packet
	 *      -       set INPKTRDY | DATAEND
	 *      - else
	 *              set INPKTRDY
	 */
	if (!(csr & (USB_CSR0_INPKTRDY | USB_CSR0_OUTPKTRDY))) {
		DEBUG_EP0("%s: INPKTRDY and OUTPKTRDY are clear\n",
			  __FUNCTION__);

		switch (dev->ep0state) {
		case DATA_STATE_XMIT:
			DEBUG_EP0("continue with DATA_STATE_XMIT\n");
			musbhsfc_ep0_in(dev, csr);
			return;
		case DATA_STATE_NEED_ZLP:
			DEBUG_EP0("continue with DATA_STATE_NEED_ZLP\n");
			musbhsfc_ep0_in_zlp(dev, csr);
			return;
		default:
			/* Stall? */
			DEBUG_EP0("Odd state!! state = %s\n",
				  state_names[dev->ep0state]);
			dev->ep0state = WAIT_FOR_SETUP;
			break;
		}
	}

	/*
	 * if USB_CSR0_OUTPKTRDY is set
	 *      - read data packet from EP0 FIFO
	 *      - decode command
	 *      - if error
	 *              set SVDOUTPKTRDY | DATAEND bits | SENDSTALL
	 *      - else
	 *              set SVDOUTPKTRDY | DATAEND bits
	 */
	if (csr & USB_CSR0_OUTPKTRDY) {

		DEBUG_EP0("%s: USB_CSR0_OUTPKTRDY is set: %x\n", __FUNCTION__,
			  csr);

		switch (dev->ep0state) {
		case WAIT_FOR_SETUP:
			DEBUG_EP0("WAIT_FOR_SETUP\n");
			musbhsfc_ep0_setup(dev, csr);
			break;

		case DATA_STATE_RECV:
			DEBUG_EP0("DATA_STATE_RECV\n");
			musbhsfc_ep0_out(dev, csr);
			break;

		default:
			/* send stall? */
			DEBUG_EP0("strange state!! 2. send stall? state = %d\n",
				  dev->ep0state);
			break;
		}
	}
}

static void musbhsfc_ep0_kick(struct musbhsfc_udc *dev, struct musbhsfc_ep *ep)
{
	u8 csr;

	usb_set_index(0);
	csr = usb_readb(USB_CSR0);

	DEBUG_EP0("%s: %x\n", __FUNCTION__, csr);

	/* Clear "out packet ready" */
	usb_setb(USB_CSR0_SVDOUTPKTRDY, USB_CSR0);

	if (ep_is_in(ep)) {
		dev->ep0state = DATA_STATE_XMIT;
		musbhsfc_ep0_in(dev, csr);
	} else {
		dev->ep0state = DATA_STATE_RECV;
		musbhsfc_ep0_out(dev, csr);
	}
}

/* ---------------------------------------------------------------------------
 * 	device-scoped parts of the api to the usb controller hardware
 * ---------------------------------------------------------------------------
 */

static int musbhsfc_udc_get_frame(struct usb_gadget *_gadget)
{
	return usb_readw(USB_FRAME);
}


static int musbhsfc_udc_wakeup(struct usb_gadget *_gadget)
{
	usb_setb(USB_POWER_RESUME, USB_POWER);
	mdelay(1);
	usb_clearb(USB_POWER_RESUME, USB_POWER);

	return 0;

}


static const struct usb_gadget_ops musbhsfc_udc_ops = {
	.get_frame = musbhsfc_udc_get_frame,
	.wakeup = musbhsfc_udc_wakeup,
	/* current versions must always be self-powered */
};

static void nop_release(struct device *dev)
{
	DEBUG("%s %s\n", __FUNCTION__, dev->bus_id);
}

static struct musbhsfc_udc udc = {
	.usb_address = 0,

	.gadget = {
		   .ops = &musbhsfc_udc_ops,
		   .ep0 = &udc.ep[0].ep,
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
			 .ops = &musbhsfc_ep_ops,
			 .maxpacket = EP0_PACKETSIZE,
			 },
		  .dev = &udc,

		  .bEndpointAddress = 0,
		  .bmAttributes = 0,

		  .ep_type = ep_control,
		  .fifo = USB_FIFO_EP0,
		  .csr1 = USB_CSR0,
		  .csr2 = USB_CSR0,
		  },

	/* first group of endpoints */
	.ep[1] = {
		  .ep = {
			 .name = "ep1",
			 .ops = &musbhsfc_ep_ops,
			 .maxpacket = BULK_FIFO_SIZE,
			 },
		  .dev = &udc,

		  .bEndpointAddress =  USB_DIR_IN | 1,
		  .bmAttributes = USB_ENDPOINT_XFER_BULK,

		  .ep_type = ep_bulk_in,
		  .fifo = USB_FIFO_EP0 + 4,
		  .csr1 = USB_INCSR,
		  .csr2 = USB_INCSRH,
		  },

	.ep[2] = {
		  .ep = {
			 .name = "ep2",
			 .ops = &musbhsfc_ep_ops,
			 .maxpacket = BULK_FIFO_SIZE,
			 },
		  .dev = &udc,

		  .bEndpointAddress = 2,
		  .bmAttributes = USB_ENDPOINT_XFER_BULK,

		  .ep_type = ep_bulk_out,
		  .fifo = USB_FIFO_EP0 + 8,
		  .csr1 = USB_OUTCSR,
		  .csr2 = USB_OUTCSRH,
		  },

	.ep[3] = {
		  .ep = {
			 .name = "ep3",
			 .ops = &musbhsfc_ep_ops,
			 .maxpacket = INT_FIFO_SIZE,
			 },
		  .dev = &udc,

		  .bEndpointAddress = USB_DIR_IN | 3,
		  .bmAttributes = USB_ENDPOINT_XFER_INT,

		  .ep_type = ep_interrupt,
		  .fifo = USB_FIFO_EP0 + 12,
		  .csr1 = USB_INCSR,
		  .csr2 = USB_INCSRH,
		  },
};

/*
 * 	probe - binds to the platform device
 */
static int musbhsfc_udc_probe(struct platform_device *pdev)
{
	struct musbhsfc_udc *dev = &udc;
	int retval;
	struct resource *res;

	DEBUG("%s: %p\n", __FUNCTION__, _dev);

	device_irq = platform_get_irq_byname(pdev, "usb_device_irq");
	if (!device_irq) {
		DEBUG("%s: no device_irq\n", __FUNCTION__);
		return -ENODEV;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		DEBUG("%s: no reg addr\n", __FUNCTION__);
		return -ENODEV;
	}
	phys_addr = res->start;
	base_len = res->end - res->start + 1;

	if (!request_mem_region(phys_addr, base_len, driver_name)) {
		DEBUG("%s: request_mem_region failed\n", __FUNCTION__);
		return -EBUSY;
        }

	base_addr = (unsigned long)ioremap_nocache(phys_addr, base_len);
	if (base_addr == 0)
		return -EFAULT;

	spin_lock_init(&dev->lock);
	dev->dev = &pdev->dev;

	device_initialize(&dev->gadget.dev);
	dev->gadget.dev.parent = &pdev->dev;

	the_controller = dev;
	platform_set_drvdata(pdev, dev);

	udc_disable(dev);
	udc_reinit(dev);

	/* irq setup after old hardware state is cleaned up */
	retval = request_irq(device_irq, musbhsfc_udc_irq, IRQF_DISABLED,
			     driver_name, dev);
	if (retval != 0) {
		DEBUG(KERN_ERR "%s: can't get irq %i, err %d\n", driver_name,
		      device_irq, retval);
		return -EBUSY;
	}

	create_proc_files();

	return retval;
}

static int musbhsfc_udc_remove(struct platform_device *pdev)
{
	struct musbhsfc_udc *dev = platform_get_drvdata(pdev);

	DEBUG("%s: %p\n", __FUNCTION__, dev);

	udc_disable(dev);
	remove_proc_files();
	usb_gadget_unregister_driver(dev->driver);

	free_irq(device_irq, dev);

	platform_set_drvdata(pdev, 0);

	release_mem_region(phys_addr, base_len);
	iounmap ((void *)base_addr);

	the_controller = 0;

	return 0;
}

/*-------------------------------------------------------------------------*/

static struct platform_driver udc_driver = {
	.probe = musbhsfc_udc_probe,
	.remove = musbhsfc_udc_remove,
	    /* FIXME power management support */
	    /* .suspend = ... disable UDC */
	    /* .resume = ... re-enable UDC */
	.driver	= {
		.name = (char *)driver_name,
		.owner = THIS_MODULE,
	},
};

static int __init udc_init(void)
{
	DEBUG("%s: %s version %s\n", __FUNCTION__, driver_name, DRIVER_VERSION);
	return platform_driver_register(&udc_driver);
}

static void __exit udc_exit(void)
{
	platform_driver_unregister(&udc_driver);
}

module_init(udc_init);
module_exit(udc_exit);

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("source@mvista.com");
MODULE_LICENSE("GPL");
