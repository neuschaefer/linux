/******************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Rajeev Kumar                    */
/*   rajeev-dlh kumar@st.com                                                  */
/*                                                                            */
/*   This program is free software; you can redistribute it and/or modify     */
/*   it under the terms of the GNU General Public License as published by     */
/*   the Free Software Foundation; either version 2 of the License, or        */
/*   (at your option) any later version.                                      */
/*                                                                            */
/*   This program is distributed in the hope that it will be useful,          */
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/*   GNU General Public License for more details.                             */
/*                                                                            */
/*   You should have received a copy of the GNU General Public License        */
/*   along with this program; if not, write to the                            */
/*   Free Software Foundation, Inc.,                                          */
/*   Foundation ,Inc. ,59 Temple Place ,Suite 330 ,Boston ,MA  02111-1307 USA */
/******************************************************************************/

#define DEBUG 1
#define VERBOSE 1

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/utsname.h>
#include <linux/device.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/cdev.h>

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/unaligned.h>
#include<linux/wait.h>

#include <linux/usb_ch9.h>
#include <linux/usb_gadget.h>

#include "gadget_chips.h"

//#define MUX						/* Comment this  for independent printer gadget driver without MUX layer */

#ifdef MUX
#include "spr_mux.h"
#endif

#define PRINTER_NOT_ERROR      0x08
#define PRINTER_SELECTED       0x10
#define PRINTER_PAPER_EMPTY    0x20

#define DRIVER_DESC		"Printer Gadget"
#define DRIVER_VERSION		"0.1.1 03/17/2006"

static const char shortname[] = "printer";
static const char driver_desc[] = DRIVER_DESC;

static const int g_printer_devno = MKDEV(247, 0);
static const int g_printer_status_devno = MKDEV(247, 1);


/*------------------------------------------------------------------------*/

 /* the device structure holds info for the USB device */

struct printer_dev
{
	spinlock_t      lock;		/*locking for device */
	spinlock_t      lock_printer_io;	/*locking for read and write oprations */
	struct usb_gadget *gadget;	/*For taking services of UDC */
#ifndef MUX
	struct usb_request *req;	/* for control responses */
#endif
	u8              config;		/*configuration number to be used */
	s8              interface;	/*Interface number to be used */
	struct usb_ep  *in_ep, *out_ep;	/*BulkIn and BulkOut endpoint */
	const struct usb_endpoint_descriptor *in, *out;
	struct list_head rx_reqs;	/*linked list for received request */
	struct list_head rx_reqs_active;	/*linked list for received to be request */
	struct list_head rx_buffers;	/*linked list for received data */
	struct usb_request *current_rx_req;	/*current  received request */
	size_t          current_rx_bytes;	/* current receive bytes */
	u8             *current_rx_buf;	/* current  received buffer */
	u8              printer_status;
	u8              reset_printer;
	unsigned        zlp:1;
	unsigned        suspended:1;	/*set this bit if the printer is in suspended mode */
	struct cdev     printer_cdev;
	u8              printer_cdev_open;
	struct cdev     printer_status_cdev;
	u8              printer_status_cdev_open;
};

static struct printer_dev usb_printer_gadget;

#ifndef MUX
/*------------------------------------------------------------------------*/

/* Thanks to NetChip Technologies for donating this product ID.
 */
#define PRINTER_VENDOR_NUM	0x0525	/* NetChip */
#define PRINTER_PRODUCT_NUM	0xa4a8	/* Linux-USB Printer Gadget */

/* ----------------------------------------------------------------------- */
/*		module prameters					   								*/
/* ------------------------------------------------------------------------*/

static ushort   idVendor;
module_param(idVendor, ushort, S_IRUGO);
MODULE_PARM_DESC(idVendor, "USB Vendor ID");

static ushort   idProduct;
module_param(idProduct, ushort, S_IRUGO);
MODULE_PARM_DESC(idProduct, "USB Product ID");

static ushort   bcdDevice;
module_param(bcdDevice, ushort, S_IRUGO);
MODULE_PARM_DESC(bcdDevice, "USB Device version (BCD)");

static char    *iManufacturer;
module_param(iManufacturer, charp, S_IRUGO);
MODULE_PARM_DESC(iManufacturer, "USB Manufacturer string");

static char    *iProduct;
module_param(iProduct, charp, S_IRUGO);
MODULE_PARM_DESC(iProduct, "USB Product string");

static char    *iSerialNum;
module_param(iSerialNum, charp, S_IRUGO);
MODULE_PARM_DESC(iSerialNum, "1");

#endif /*ndef MUX */
static char    *iPNPstring;
module_param(iPNPstring, charp, S_IRUGO);
MODULE_PARM_DESC(iPNPstring, "MFG:linux;MDL:g_printer;CLS:PRINTER;SN:1;");

/* Number of requests to allocate per endpoint, not used for ep0. */
static unsigned qlen = 10;
module_param(qlen, uint, S_IRUGO | S_IWUSR);

#define QLEN	qlen

#ifdef CONFIG_USB_GADGET_DUALSPEED
#define DEVSPEED	USB_SPEED_HIGH
#else /* full speed (low speed doesn't do bulk) */
#define DEVSPEED        USB_SPEED_FULL
#endif

/*-------------------------------------------------------------------------*/

#define xprintk(d,level,fmt,args...) \
	printk(level "%s: " fmt , DRIVER_DESC , ## args)

#ifdef DEBUG
#undef DEBUG
#define DEBUG(dev,fmt,args...) \
	xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define DEBUG(dev,fmt,args...) \
	do { } while (0)
#endif /* DEBUG */

#ifdef VERBOSE
#define VDEBUG(dev,fmt,args...) \
	xprintk(dev , KERN_DEBUG , fmt , ## args)
#else
#define VDEBUG(dev,fmt,args...) \
	xprintk(dev , KERN_DEBUG , fmt , ## args)
#endif /* DEBUG */

#define ERROR(dev,fmt,args...) \
	xprintk(dev , KERN_ERR , fmt , ## args)
#define WARN(dev,fmt,args...) \
	xprintk(dev , KERN_WARNING , fmt , ## args)
#define INFO(dev,fmt,args...) \
	xprintk(dev , KERN_INFO , fmt , ## args)

/*-------------------------------------------------------------------------*/

/* USB DRIVER HOOKUP (to the hardware driver, below us), mostly
 * ep0 implementation:  descriptors, config management, setup().
 * also optional class-specific notification interrupt transfer.
 */

/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.
 */

#ifndef MUX

#define STRING_MANUFACTURER		1
#define STRING_PRODUCT			2
#define STRING_SERIALNUM		3

#endif /*ndef MUX */

/* holds our biggest descriptor */
#define USB_DESC_BUFSIZE		256
#define USB_BUFSIZE			8192

/* This device advertises one configuration. */
#define DEFAULT_CONFIG_VALUE		3

#define	PRINTER_INTERFACE		0
static          DECLARE_WAIT_QUEUE_HEAD(wait_printer);

/*------------------------------------------------------------------------------*/
/*	usb_device_descriptor device_desc											*/
/*------------------------------------------------------------------------------*/

#ifndef MUX
static struct usb_device_descriptor device_desc = {
	.bLength = sizeof device_desc,
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = __constant_cpu_to_le16(0x0200),
	.bDeviceClass = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.idVendor = __constant_cpu_to_le16(PRINTER_VENDOR_NUM),
	.idProduct = __constant_cpu_to_le16(PRINTER_PRODUCT_NUM),
	.iManufacturer = STRING_MANUFACTURER,
	.iProduct = STRING_PRODUCT,
	.iSerialNumber = STRING_SERIALNUM,
	.bNumConfigurations = 1
};

/*------------------------------------------------------------------------------*/
/*	usb_otg_descriptor otg_desc													*/
/*------------------------------------------------------------------------------*/

static struct usb_otg_descriptor otg_desc = {
	.bLength = sizeof otg_desc,
	.bDescriptorType = USB_DT_OTG,
	.bmAttributes = USB_OTG_SRP
};

/*------------------------------------------------------------------------------*/
/*	usb_config_descriptor config_desc 											*/
/*------------------------------------------------------------------------------*/

static struct usb_config_descriptor config_desc = {
	.bLength = sizeof config_desc,
	.bDescriptorType = USB_DT_CONFIG,

	/* compute wTotalLength on the fly */
	.bNumInterfaces = 1,
	.bConfigurationValue = DEFAULT_CONFIG_VALUE,
	.iConfiguration = 0,
	.bmAttributes = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower = 1				/* Self-Powered */
};

/*------------------------------------------------------------------------------*/
/*	 usb_interface_descriptor intf_desc											*/
/*------------------------------------------------------------------------------*/

#endif /*ndef MUX */

static struct usb_interface_descriptor intf_desc = {
	.bLength = sizeof intf_desc,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = PRINTER_INTERFACE,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_PRINTER,
	.bInterfaceSubClass = 1,	/* Printer Sub-Class */
	.bInterfaceProtocol = 2,
	.iInterface = 0
};

/*------------------------------------------------------------------------------*/
/*	usb_endpoint_descriptor fs_ep_in_desc										*/
/*------------------------------------------------------------------------------*/

static struct usb_endpoint_descriptor fs_ep_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_IN,
	.bmAttributes = USB_ENDPOINT_XFER_BULK
};

/*------------------------------------------------------------------------------*/
/*	usb_endpoint_descriptor fs_ep_out_desc										*/
/*------------------------------------------------------------------------------*/

static struct usb_endpoint_descriptor fs_ep_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = USB_DIR_OUT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK
};

static const struct usb_descriptor_header *fs_printer_function[11] = {
#ifndef MUX
	(struct usb_descriptor_header *) &otg_desc,
#endif
	(struct usb_descriptor_header *) &intf_desc,
	(struct usb_descriptor_header *) &fs_ep_in_desc,
	(struct usb_descriptor_header *) &fs_ep_out_desc,
	NULL
};

#ifdef	CONFIG_USB_GADGET_DUALSPEED

/*----------------------------------------------------------------------*/
/* usb 2.0 devices need to expose both high speed and full speed		*/
/* descriptors, unless they only run at full speed.						*/
/*--------------------------------------------------------------------- */

static struct usb_endpoint_descriptor hs_ep_in_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __constant_cpu_to_le16(512)
};

static struct usb_endpoint_descriptor hs_ep_out_desc = {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bmAttributes = USB_ENDPOINT_XFER_BULK,
	.wMaxPacketSize = __constant_cpu_to_le16(512)
};

#ifndef MUX

static struct usb_qualifier_descriptor dev_qualifier = {
	.bLength = sizeof dev_qualifier,
	.bDescriptorType = USB_DT_DEVICE_QUALIFIER,
	.bcdUSB = __constant_cpu_to_le16(0x0200),
	.bDeviceClass = USB_CLASS_PRINTER,
	.bNumConfigurations = 1
};

#endif /*ndef MUX */

static const struct usb_descriptor_header *hs_printer_function[11] = {
#ifndef MUX
	(struct usb_descriptor_header *) &otg_desc,
#endif
	(struct usb_descriptor_header *) &intf_desc,
	(struct usb_descriptor_header *) &hs_ep_in_desc,
	(struct usb_descriptor_header *) &hs_ep_out_desc,
	NULL
};

/* maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,hs,fs) (((g)->speed==USB_SPEED_HIGH)?(hs):(fs))

#else

/* if there's no high speed support, maxpacket doesn't change. */
#define ep_desc(g,hs,fs) (((void)(g)), (fs))

#endif /* !CONFIG_USB_GADGET_DUALSPEED */

/*-------------------------------------------------------------------------*/

/* descriptors that are built on-demand */

#ifndef MUX

static char     manufacturer[50];
static char     product_desc[40] = DRIVER_DESC;
static char     serial_num[40] = "1";

#endif /*ndef MUX */

static char     pnp_string[1024] = "XXMFG:linux;MDL:g_printer;CLS:PRINTER;SN:1;";

#ifndef MUX

/* static strings, in UTF-8 */
static struct usb_string strings[] = {
	{STRING_MANUFACTURER, manufacturer,},
	{STRING_PRODUCT, product_desc,},
	{STRING_SERIALNUM, serial_num,},
	{}							/* end of list */
};

static struct usb_gadget_strings stringtab = {
	.language = 0x0409,			/* en-us */
	.strings = strings,
};

#endif /*ndef MUX */

/*-------------------------------------------------------------------------*/

/****************************************************************************/
/* NAME:        printer_req_alloc                                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Allocate memory for a usb request for an endpoint          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      ep:  endpoint for which usb request has to be allocated       */
/*            len: size of buf                                              */
/*            kmalloc_flags: flags to be passed while allocating memory     */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    allocated usb request                                         */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static struct usb_request *
printer_req_alloc(struct usb_ep *ep, unsigned size, int gfp_flags)
{
	struct usb_request *req;

	req = usb_ep_alloc_request(ep, gfp_flags);
	if (!req)
		return NULL;

	req->buf = usb_ep_alloc_buffer(ep, size, &req->dma, gfp_flags);
	if (!req->buf)
	{
		usb_ep_free_request(ep, req);
		req = NULL;
	}
	req->length = size;

	return req;
}

/****************************************************************************/
/* NAME:     printer_req_free                                               */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     Frees an allocated memory for USB request               */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :            ep:  endpoint for which req was assigned                */
/*                  req: request to be freed                                */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:          NONE                                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_req_free(struct usb_ep *ep, struct usb_request *req)
{
	usb_ep_free_buffer(ep, req->buf, req->dma, req->length);
	usb_ep_free_request(ep, req);
}

/****************************************************************************/
/* NAME:     rx_complete  		                                    		*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from UDC when data came	                            */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      ep:  endpoint for which data is arrived			    		*/
/*            struct usb_request :adding active bufffer to                  */
/*	        req_list						    							*/
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    NONE			                                    			*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
rx_complete(struct usb_ep *ep, struct usb_request *req)
{
	struct printer_dev *dev = ep->driver_data;
	int             status = req->status;
	unsigned long   flags;

	spin_lock_irqsave(&dev->lock, flags);

	list_del_init(&req->list);	/* Remove from Active List */

	switch (status)
	{

		/* normal completion */
	case 0:
		list_add_tail(&req->list, &dev->rx_buffers);
		DEBUG(dev, "G_Printer : rx length %d\n", req->actual);
		wake_up_interruptible(&wait_printer);
		break;

		/* software-driven interface shutdown */
	case -ECONNRESET:			// unlink
	case -ESHUTDOWN:			// disconnect etc
		VDEBUG(dev, "rx shutdown, code %d\n", status);

		list_add(&req->list, &dev->rx_reqs);
		break;

		/* for hardware automagic (such as pxa) */
	case -ECONNABORTED:		// endpoint reset
		DEBUG(dev, "rx %s reset\n", ep->name);
		list_add(&req->list, &dev->rx_reqs);
		break;

		/* data overrun */
	case -EOVERFLOW:
		// FALLTHROUGH

	default:
		DEBUG(dev, "rx status %d\n", status);
		list_add(&req->list, &dev->rx_reqs);
		break;
	}
	spin_unlock_irqrestore(&dev->lock, flags);
}

/****************************************************************************/
/* NAME:    printer_open   		                                    		*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    				*/
/*            fd :file opearation pointer		                    		*/
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    		*/
/*             1 = FAILURE                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_open(struct inode *inode, struct file *fd)
{
	struct printer_dev *dev;
	unsigned long   flags;
	int             ret = -EBUSY;

	dev = container_of(inode->i_cdev, struct printer_dev, printer_cdev);

	spin_lock_irqsave(&dev->lock, flags);

	if (!dev->printer_cdev_open)
	{
		dev->printer_cdev_open = 1;
		fd->private_data = dev;
		ret = 0;
	}

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_open returned %x\n", ret);

	return ret;
}

/****************************************************************************/
/* NAME:    printer_close   		                                    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    				*/
/*            fd :file opearation pointer		                    		*/
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    0 = SUCCESS		                                    		*/
/*            1 = FAILURE                                                   */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_close(struct inode *inode, struct file *fd)
{
	struct printer_dev *dev;
	unsigned long   flags;

	dev = container_of(inode->i_cdev, struct printer_dev, printer_cdev);

	spin_lock_irqsave(&dev->lock, flags);

	dev->printer_cdev_open = 0;

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_close\n");

	return 0;
}

/****************************************************************************/
/* NAME:    printer_read   		                                    		*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    		*/
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    					*/
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes read		                                    		*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static          ssize_t
printer_read(struct file *fd, char __user * buf, size_t len, loff_t * ptr)
{
	struct printer_dev *dev = fd->private_data;
	unsigned long   flags;
	size_t          size;
	size_t          bytes_copied;
	struct usb_request *req;
	struct usb_request *current_rx_req;
	size_t          current_rx_bytes;
	u8             *current_rx_buf;

	if (len == 0)
		return 0;

	DEBUG(dev, "printer_read trying to read %d bytes\n", (int) len);

	spin_lock(&dev->lock_printer_io);
	spin_lock_irqsave(&dev->lock, flags);

	/* Check if a printer reset happens while we have interrupts on */
	dev->reset_printer = 0;

	while (likely(!list_empty(&dev->rx_reqs)))
	{
		int             error;

		req = container_of(dev->rx_reqs.next, struct usb_request, list);
		list_del_init(&req->list);


		req->length = USB_BUFSIZE;
		req->complete = rx_complete;

		error = usb_ep_queue(dev->out_ep, req, GFP_ATOMIC);

		/* We've disconnected or reset free the req and buffer */
		if (dev->reset_printer)
		{
			if (!error)
			{
				if (usb_ep_dequeue(dev->out_ep, req))
				{
					DEBUG(dev, "Failed to dequeue " "in req %p\n", req);
				}
			}
			printer_req_free(dev->out_ep, req);
			spin_unlock_irqrestore(&dev->lock, flags);
			spin_unlock(&dev->lock_printer_io);
			return 0;
		}
	if (error)
		{
			DEBUG(dev, "rx submit --> %d\n", error);
			list_add(&req->list, &dev->rx_reqs);
			break;
		}
		else
		{
			list_add(&req->list, &dev->rx_reqs_active);
			break;
		}
	}

	bytes_copied = 0;
	current_rx_req = dev->current_rx_req;
	current_rx_bytes = dev->current_rx_bytes;
	current_rx_buf = dev->current_rx_buf;
	dev->current_rx_req = NULL;
	dev->current_rx_bytes = 0;
	dev->current_rx_buf = NULL;

	if (list_empty(&dev->rx_buffers) && !current_rx_bytes)
	{
		spin_unlock(&dev->lock_printer_io);
		spin_unlock_irqrestore(&dev->lock, flags);
		
		//interruptible_sleep_on(&wait_printer);
		wait_event_interruptible(wait_printer, !list_empty(&dev->rx_buffers));
		
		spin_lock(&dev->lock_printer_io);
		spin_lock_irqsave(&dev->lock, flags);
	}

	while ((current_rx_bytes || likely(!list_empty(&dev->rx_buffers))) && len)
	{
		if (current_rx_bytes == 0)
		{
			req = container_of(dev->rx_buffers.next, struct usb_request, list);
			list_del_init(&req->list);

			if (req->actual && req->buf)
			{
				current_rx_req = req;
				current_rx_bytes = req->actual;
				current_rx_buf = req->buf;
			}
			else
			{
				list_add(&req->list, &dev->rx_reqs);
				continue;
			}
		}

		/* Don't leave irqs off while doing memory copies */
		spin_unlock_irqrestore(&dev->lock, flags);

		if (len > current_rx_bytes)
			size = current_rx_bytes;
		else
			size = len;

		size -= copy_to_user(buf, current_rx_buf, size);
		bytes_copied += size;
		len -= size;
		buf += size;

		spin_lock_irqsave(&dev->lock, flags);

		/* We've disconnected or reset free the req and buffer */
		if (dev->reset_printer)
		{
			printer_req_free(dev->out_ep, current_rx_req);
			spin_unlock_irqrestore(&dev->lock, flags);
			spin_unlock(&dev->lock_printer_io);
			return 0;
		}

		if (size < current_rx_bytes)
		{
			current_rx_bytes -= size;
			current_rx_buf += size;
		}
		else
		{
			list_add(&current_rx_req->list, &dev->rx_reqs);
			current_rx_bytes = 0;
			current_rx_buf = NULL;
			current_rx_req = NULL;
		}
	}

	dev->current_rx_req = current_rx_req;
	dev->current_rx_bytes = current_rx_bytes;
	dev->current_rx_buf = current_rx_buf;

	spin_unlock_irqrestore(&dev->lock, flags);
	spin_unlock(&dev->lock_printer_io);

	DEBUG(dev, "printer_read returned %d bytes\n", (int) bytes_copied);

	return bytes_copied;
}

/* used after endpoint configuration */
static struct file_operations printer_io_operations = {
	.owner = THIS_MODULE,
	.open = printer_open,
	.read = printer_read,
	.release = printer_close
};

/****************************************************************************/
/* NAME:    printer_status_open 	                                    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    				*/
/*            fd :file opearation pointer		                    		*/
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    		*/
/*	       1 = FAILURE						    							*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_status_open(struct inode *inode, struct file *fd)
{
	struct printer_dev *dev;
	unsigned long   flags;
	int             ret = -EBUSY;

	dev = container_of(inode->i_cdev, struct printer_dev, printer_status_cdev);

	spin_lock_irqsave(&dev->lock, flags);

	if (!dev->printer_status_cdev_open)
	{
		dev->printer_status_cdev_open = 1;
		fd->private_data = dev;
		ret = 0;
	}

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_status_open returned %x\n", ret);

	return ret;
}

/****************************************************************************/
/* NAME:  printer_status_close   	                                    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    				*/
/*            fd :file opearation pointer		                    		*/
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    0 = SUCCESS			                            			*/
/*	      1 = FAILURE						    							*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_status_close(struct inode *inode, struct file *fd)
{
	struct printer_dev *dev;
	unsigned long   flags;

	dev = container_of(inode->i_cdev, struct printer_dev, printer_status_cdev);

	spin_lock_irqsave(&dev->lock, flags);

	dev->printer_status_cdev_open = 0;

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_status_close\n");

	return 0;
}

/****************************************************************************/
/* NAME:   printer_status_read 		                                    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    		*/
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    					*/
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes read		                                    		*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static          ssize_t
printer_status_read(struct file *fd, char __user * buf, size_t len, loff_t * ptr)
{
	struct printer_dev *dev;
	unsigned long   flags;
	int             ret;

	if (len <= 0)
		return 0;

	dev = fd->private_data;

	spin_lock_irqsave(&dev->lock, flags);

	ret = !(copy_to_user(buf, &dev->printer_status, 1));

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_status_read =0x%x bytes copied = %d\n", dev->printer_status, ret);

	return ret;
}

/****************************************************************************/
/* NAME:   printer_status_write		                                    	*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    		*/
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    					*/
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes copied		                                    		*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static          ssize_t
printer_status_write(struct file *fd, const char __user * buf, size_t len, loff_t * ptr)
{
	struct printer_dev *dev;
	unsigned long   flags;
	int             ret;

	if (len <= 0)
		return 0;

	dev = fd->private_data;

	spin_lock_irqsave(&dev->lock, flags);

	ret = !(copy_from_user(&dev->printer_status, buf, 1));

	spin_unlock_irqrestore(&dev->lock, flags);

	DEBUG(dev, "printer_status_write wrote %x\n", dev->printer_status);

	return ret;
}

/* used after endpoint configuration */
static struct file_operations printer_status_operations = {
	.owner = THIS_MODULE,
	.open = printer_status_open,
	.read = printer_status_read,
	.write = printer_status_write,
	.release = printer_status_close
};

/****************************************************************************/
/* NAME:       enable_ printer_interface				    				*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      enable endps   					    				*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:          int: 0 if success , error otherwise                     */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
enable_printer_interface(struct printer_dev *dev)
{
	int             result = 0;

	dev->in = ep_desc(dev->gadget, &hs_ep_in_desc, &fs_ep_in_desc);
	dev->in_ep->driver_data = dev;

	dev->out = ep_desc(dev->gadget, &hs_ep_out_desc, &fs_ep_out_desc);
	dev->out_ep->driver_data = dev;

	result = usb_ep_enable(dev->in_ep, dev->in);
	if (result != 0)
	{
		DEBUG(dev, "enable %s --> %d\n", dev->in_ep->name, result);
		goto done;
	}

	result = usb_ep_enable(dev->out_ep, dev->out);
	if (result != 0)
	{
		DEBUG(dev, "enable %s --> %d\n", dev->in_ep->name, result);
		goto done;
	}

  done:
	/* on error, disable any endpoints  */
	if (result != 0)
	{
		(void) usb_ep_disable(dev->in_ep);
		(void) usb_ep_disable(dev->out_ep);
		dev->in = NULL;
		dev->out = NULL;
	}

	/* caller is responsible for cleanup on error */
	return result;
}

/****************************************************************************/
/* NAME:       disable_ printer_interface				    				*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      disable endps   					    				*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:        NONE                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
disable_printer_interface(void *device)
{
	struct printer_dev *dev = (struct printer_dev *) device;

	if (dev->interface < 0)
		return;

	DEBUG(dev, "%s\n", __FUNCTION__);

	if (dev->in)
		usb_ep_disable(dev->in_ep);

	if (dev->out)
		usb_ep_disable(dev->out_ep);
	dev->interface = -1;
}

/****************************************************************************/
/* NAME:      printer_set_config					    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     set specific(config) configuration i.e. enable endps    */
/*                  as per config.change our operational config.	    	*/
/*	           must agree with the code that returns config descriptors 	*/
/*		  , and altsetting code.                                    		*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                 config: configuration number                             */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:       int: 0 if success , error otherwise                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_set_config(void *device, unsigned number)
{
	int             result = -EOPNOTSUPP;
	struct printer_dev *dev = (struct printer_dev *) device;
#ifndef MUX
	struct usb_gadget *gadget = dev->gadget;
#endif /*ndef MUX */

	/* First disable the current configuration */
	switch (dev->config)
	{
	case DEFAULT_CONFIG_VALUE:
		disable_printer_interface(dev);
		break;
	}

	/* Now enable the new configuration */
	switch (number)
	{
	case DEFAULT_CONFIG_VALUE:
		result = enable_printer_interface(dev);
		break;
	}

#ifndef MUX
	if (result)
	{
		usb_gadget_vbus_draw(dev->gadget, dev->gadget->is_otg ? 8 : 100);
	}
	else
#endif
	{
#ifndef MUX
		char           *speed;
		unsigned        power;

		power = 2 * config_desc.bMaxPower;
		usb_gadget_vbus_draw(dev->gadget, power);

		switch (gadget->speed)
		{
		case USB_SPEED_FULL:
			speed = "full";
			break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
		case USB_SPEED_HIGH:
			speed = "high";
			break;
#endif
		default:
			speed = "?";
			break;
		}
#endif /* !MUX */

		dev->config = number;
		while (likely(!list_empty(&dev->rx_reqs)))
		{
			int             error;
			struct usb_request *req;

			req = container_of(dev->rx_reqs.next, struct usb_request, list);
			list_del_init(&req->list);

			req->length = USB_BUFSIZE;
			req->complete = rx_complete;

			error = usb_ep_queue(dev->out_ep, req, GFP_ATOMIC);

			/* We've disconnected or reset free the req and buffer */
			if (dev->reset_printer)
			{
				if (!error)
				{
					if (usb_ep_dequeue(dev->out_ep, req))
					{
						DEBUG(dev, "Failed to dequeue " "in req %p\n", req);
					}
				}
				printer_req_free(dev->out_ep, req);
				return 0;
			}

			if (error)
			{
				DEBUG(dev, "rx submit --> %d\n", error);
				list_add(&req->list, &dev->rx_reqs);
				break;
			}
			else
			{
				list_add(&req->list, &dev->rx_reqs_active);
				break;
			}
		}

#ifndef MUX
		INFO(dev, "%s speed config #%d: %d mA, %s\n", speed, number, power, driver_desc);
#endif
	}
	return result;
}

/****************************************************************************/
/* NAME:			config_buf				    							*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    API to build configuration descriptor for printer        */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :          speed: high or low speed                                  */
/*                type:  descriptor type				    				*/
/*                index: configuration                                      */
/*                is_otg: whether otg                                       */
/* OUT :          buf:    buffer in which descriptor to be prepared         */
/*                                                                          */
/* RETURN:        length of buffer                                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

#ifndef MUX
static int
config_buf(enum usb_device_speed speed, u8 * buf, u8 type, unsigned index, int is_otg)
{
	int             len;
	const struct usb_descriptor_header **function;
#ifdef CONFIG_USB_GADGET_DUALSPEED
	int             hs = (speed == USB_SPEED_HIGH);

	if (type == USB_DT_OTHER_SPEED_CONFIG)
		hs = !hs;

	if (hs)
	{
		function = hs_printer_function;
	}
	else
	{
		function = fs_printer_function;
	}
#else
	function = fs_printer_function;
#endif

	if (index >= device_desc.bNumConfigurations)
		return -EINVAL;

	/* for now, don't advertise srp-only devices */
	if (!is_otg)
		function++;

	len = usb_gadget_config_buf(&config_desc, buf, USB_DESC_BUFSIZE, function);
	if (len < 0)
		return len;
	((struct usb_config_descriptor *) buf)->bDescriptorType = type;
	return len;
}

#endif /*ndef MUX */

/****************************************************************************/
/* NAME:        printer_set_alt_interface				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Change our operational Interface disable endps   	    */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                 number: configuration number                             */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:        0 on success otherwise error                              */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
printer_set_alt_interface(struct printer_dev *dev, unsigned number)
{
	int             result = -EINVAL;

	if (gadget_is_sa1100(dev->gadget) && dev->interface < 0)
	{
		/* tx fifo is full, but we can't clear it... */
		INFO(dev, "can't change interfaces\n");
		return -ESPIPE;
	}

	switch (number)
	{
	case PRINTER_INTERFACE:
		result = 0;
		break;
	}

	if (!result)
		INFO(dev, "Using printer alt interface %x\n", number);
	return result;
}

/****************************************************************************/
/* NAME:      printer_setup_complete                                        */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     called when setup completes                             */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :            ep: control endpoint                                    */
/*                  req: usb request which completed                        */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:   NONE                                                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

#ifndef MUX
static void
printer_setup_complete(struct usb_ep *ep, struct usb_request *req)
{
	if (req->status || req->actual != req->length)
		DEBUG((struct printer_dev *) ep->driver_data, "setup complete --> %d, %d/%d\n", req->status, req->actual, req->length);
}
#endif /*ndef MUX */

/****************************************************************************/
/* NAME:       printer_soft_reset               			    			*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Received printer reset request    			    		*/
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:        NONE                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_soft_reset(struct printer_dev *dev)
{
	struct usb_request *req;

	if (usb_ep_disable(dev->in_ep))
		DEBUG(dev, "Failed to disable USB in_ep\n");
	if (usb_ep_disable(dev->out_ep))
		DEBUG(dev, "Failed to disable USB out_ep\n");

	if (dev->current_rx_req != NULL)
	{
		list_add(&dev->current_rx_req->list, &dev->rx_reqs);
		dev->current_rx_req = NULL;
	}
	dev->current_rx_bytes = 0;
	dev->current_rx_buf = NULL;
	dev->reset_printer = 1;

	while (likely(!(list_empty(&dev->rx_buffers))))
	{
		req = container_of(dev->rx_buffers.next, struct usb_request, list);
		list_del_init(&req->list);
		list_add(&req->list, &dev->rx_reqs);
	}

	while (likely(!(list_empty(&dev->rx_reqs_active))))
	{
		req = container_of(dev->rx_buffers.next, struct usb_request, list);
		if (usb_ep_dequeue(dev->in_ep, req))
			DEBUG(dev, "Failed to dequeue in req %p\n", req);

		list_del_init(&req->list);
		list_add(&req->list, &dev->rx_reqs);
	}

	if (usb_ep_enable(dev->in_ep, dev->in))
		DEBUG(dev, "Failed to enable USB in_ep\n");
	if (usb_ep_enable(dev->out_ep, dev->out))
		DEBUG(dev, "Failed to enable USB out_ep\n");
}

/****************************************************************************/
/* NAME:      printer_setup                                                  */
/*---------------------------------------------------------------------------*/
/* DESCRIPTION:  Invoked when control packet received on endpoint 0          */
/*                                                                           */
/*                                                                           */
/* PARAMETERS:                                                               */
/* IN  :         gadget: gadget structure                                    */
/*               ctrl:   usb ctrl request                                    */
/*                                                                           */
/* OUT :                                                                     */
/*                                                                           */
/* RETURN:       int                                                         */
/*---------------------------------------------------------------------------*/
/* REENTRANCY:                                                               */
/****************************************************************************/
static int
#ifdef MUX
printer_setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl, struct usb_request *req)
#else
printer_setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
#endif
{
	struct printer_dev *dev = get_gadget_data(gadget);
#ifndef MUX
	struct usb_request *req = dev->req;
#endif
	int             value = -EOPNOTSUPP;
	u16             wIndex = le16_to_cpu(ctrl->wIndex);
	u16             wValue = le16_to_cpu(ctrl->wValue);
	u16             wLength = le16_to_cpu(ctrl->wLength);
#ifndef MUX
	req->complete = printer_setup_complete;
#endif

	switch (ctrl->bRequestType & USB_TYPE_MASK)
	{

	case USB_TYPE_STANDARD:
		switch (ctrl->bRequest)
		{

#ifndef MUX
		case USB_REQ_GET_DESCRIPTOR:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;
			switch (wValue >> 8)
			{

			case USB_DT_DEVICE:
				value = min(wLength, (u16) sizeof device_desc);
				memcpy(req->buf, &device_desc, value);
				break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
			case USB_DT_DEVICE_QUALIFIER:
				if (!gadget->is_dualspeed)
					break;
				value = min(wLength, (u16) sizeof dev_qualifier);
				memcpy(req->buf, &dev_qualifier, value);
				break;

			case USB_DT_OTHER_SPEED_CONFIG:
				if (!gadget->is_dualspeed)
					break;
				// FALLTHROUGH 
#endif //CONFIG_USB_GADGET_DUALSPEED
			case USB_DT_CONFIG:
				value = config_buf(gadget->speed, req->buf, wValue >> 8, wValue & 0xff, gadget->is_otg);
				if (value >= 0)
					value = min(wLength, (u16) value);
				break;

			case USB_DT_STRING:
				value = usb_gadget_get_string(&stringtab, wValue & 0xff, req->buf);
				if (value >= 0)
					value = min(wLength, (u16) value);
				break;
			}
			break;

		case USB_REQ_SET_CONFIGURATION:
			if (ctrl->bRequestType != 0)
				break;
			if (gadget->a_hnp_support)
				DEBUG(dev, "HNP available\n");
			else if (gadget->a_alt_hnp_support)
				DEBUG(dev, "HNP needs a different root port\n");

			spin_lock(&dev->lock);
			value = printer_set_config(dev, wValue);
			spin_unlock(&dev->lock);

			break;
		case USB_REQ_GET_CONFIGURATION:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;

			*(u8 *) req->buf = dev->config;
			value = min(wLength, (u16) 1);
			break;
#endif /*ndef MUX */

		case USB_REQ_SET_INTERFACE:
			if (ctrl->bRequestType != USB_RECIP_INTERFACE || !dev->config)
				break;

			value = printer_set_alt_interface(dev, PRINTER_INTERFACE);
			break;

		case USB_REQ_GET_INTERFACE:
			if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE) || !dev->config)
				break;

			*(u8 *) req->buf = dev->interface;
			value = min(wLength, (u16) 1);
			break;

		default:
			VDEBUG(dev,
				   "unknown ctrl req%02x.%02x v%04x i%04x l%d\n", ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);
		}
		break;

	case USB_TYPE_CLASS:
		switch (ctrl->bRequest)
		{
		case 0:				/* Get the IEEE-1284 PNP String */
#ifndef MUX
			/* Only one printer interface is supported. */
			if ((wIndex >> 8) != PRINTER_INTERFACE)
			{
				break;
			}
#endif

			value = (pnp_string[0] << 8) | pnp_string[1];
			memcpy(req->buf, pnp_string, value);

			DEBUG(dev, "1284 PNP String: %x %s\n", value, &pnp_string[2]);
			break;

		case 1:				/* Get Port Status */
#ifndef MUX
			/* Only one printer interface is supported. */
			if (wIndex != PRINTER_INTERFACE)
			{
				break;
			}
#endif

			*(u8 *) req->buf = dev->printer_status;
			value = min(wLength, (u16) 1);
			break;

		case 2:				/* Soft Reset */
#ifndef MUX
			/* Only one printer interface is supported. */
			if (wIndex != PRINTER_INTERFACE)
			{
				break;
			}
#endif

			printer_soft_reset(dev);

			value = 0;
			break;

		default:
			VDEBUG(dev,
				   "unknown ctrl req%02x.%02x v%04x i%04x l%d\n", ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);
		}
		break;

	default:
		VDEBUG(dev, "unknown ctrl req%02x.%02x v%04x i%04x l%d\n", ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);
		break;
	}

#ifndef MUX
	/* respond with data transfer before status phase? */
	if (value >= 0)
	{
		req->length = value;
		req->zero = value < wLength && (value % gadget->ep0->maxpacket) == 0;

		value = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);
		if (value < 0)
		{
			DEBUG(dev, "ep_queue --> %d\n", value);
			req->status = 0;
			printer_setup_complete(gadget->ep0, req);
		}
	}
#endif /* !MUX */

	/* host either stalls (value < 0) or reports success */
	return value;
}

/****************************************************************************/
/* NAME:      printer_disconnect                                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Invoked when host disconnected                         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :             gadget: gadget structure prepared by UDC driver        */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:  NONE                                                            */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_disconnect(struct usb_gadget *gadget)
{
	struct printer_dev *dev = get_gadget_data(gadget);
	unsigned long   flags;

	DEBUG(dev, "%s\n", __FUNCTION__);

	spin_lock_irqsave(&dev->lock, flags);
	disable_printer_interface(dev);
	spin_unlock_irqrestore(&dev->lock, flags);
}

/****************************************************************************/
/* NAME:         printer_unbind                                             */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when printer unregisters from UDC driver            */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :         gadget structure filled by UDC driver                      */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:      NONE                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_unbind(struct usb_gadget *gadget)
{
	struct printer_dev *dev = get_gadget_data(gadget);
	struct usb_request *req;

	DEBUG(dev, "%s\n", __FUNCTION__);
	printk("%s\n", __FUNCTION__);

	/* Remove Character Device */
	cdev_del(&dev->printer_cdev);
	cdev_del(&dev->printer_status_cdev);

	if (dev->current_rx_req)
	{
		printer_req_free(dev->out_ep, dev->current_rx_req);
	}

	while (likely(!list_empty(&dev->rx_reqs_active)))
	{
		req = container_of(dev->rx_reqs_active.next, struct usb_request, list);
		list_del(&req->list);
		if (usb_ep_dequeue(dev->out_ep, req))
		{
			DEBUG(dev, "Failed to dequeue in req %p\n", req);
		}
		printer_req_free(dev->out_ep, req);
	}

	while (likely(!list_empty(&dev->rx_reqs)))
	{
		req = container_of(dev->rx_reqs.next, struct usb_request, list);
		list_del(&req->list);
		printer_req_free(dev->out_ep, req);
	}

	while (likely(!list_empty(&dev->rx_buffers)))
	{
		req = container_of(dev->rx_buffers.next, struct usb_request, list);
		list_del(&req->list);
		printer_req_free(dev->out_ep, req);
	}

#ifndef MUX
	if (dev->req)
	{
		usb_ep_free_buffer(gadget->ep0, dev->req->buf, dev->req->dma, dev->req->length);
		usb_ep_free_request(gadget->ep0, dev->req);
		dev->req = NULL;
	}
#endif

	set_gadget_data(gadget, NULL);
}

/****************************************************************************/
/* NAME:   printer_bind	    		                	           			*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Called when printer binds to UDC driver                 	*/
/*      							           								*/
/*                                                                         	*/
/* PARAMETERS:                                                             	*/
/* IN  :           gadget: usb_gadget structure filled by UDC driver       	*/
/*                         with ops      				   					*/
/*                                          				   				*/
/* OUT :                                                                	*/
/*                                                                         	*/
/* RETURN: 	0 on success						   							*/
/*		    err otherwise 					   								*/
/*--------------------------------------------------------------------------*/
/* REENTRANCY: 						                   						*/
/****************************************************************************/

static int
printer_bind(struct usb_gadget *gadget)
{
	struct printer_dev *dev;
	u8              zlp = 1;
	struct usb_ep  *in_ep, *out_ep;
	int             status = -ENOMEM;
	int             gcnum;
	size_t          len;
	u32             i;
	struct usb_request *req;

	dev = &usb_printer_gadget;

	/* 
	 * Register a character device as an interface to a user mode
	 * program that handles the printer specific functionality.
	 */

	cdev_init(&dev->printer_cdev, &printer_io_operations);
	dev->printer_cdev.owner = THIS_MODULE;
	status = cdev_add(&dev->printer_cdev, g_printer_devno, 1);
	if (status)
	{
		goto fail;
	}

	cdev_init(&dev->printer_status_cdev, &printer_status_operations);
	dev->printer_status_cdev.owner = THIS_MODULE;
	status = cdev_add(&dev->printer_status_cdev, g_printer_status_devno, 1);
	if (status)
	{
		ERROR(dev, "Failed to open char device\n");
		goto fail;
	}

	if (gadget_is_sa1100(gadget))
	{
		/* hardware can't write zlps */
		zlp = 0;
	}
	gcnum = 0x0012;

	if (gcnum >= 0)
	{
#ifndef MUX
		device_desc.bcdDevice = cpu_to_le16(0x0200 + gcnum);
#endif /*ndef MUX */
	}
	else
	{
		dev_warn(&gadget->dev, "controller '%s' not recognized\n", gadget->name);
		/* unrecognized, but safe unless bulk is REALLY quirky */
#ifndef MUX
		device_desc.bcdDevice = __constant_cpu_to_le16(0xFFFF);
#endif /*ndef MUX */
	}

#ifndef MUX
	device_desc.bcdDevice = __constant_cpu_to_le16(0x0200 | 0x0012);
	device_desc.idVendor = __constant_cpu_to_le16(PRINTER_VENDOR_NUM);
	device_desc.idProduct = __constant_cpu_to_le16(PRINTER_PRODUCT_NUM);

	/* support optional vendor/distro customization */
	if (idVendor)
	{
		if (!idProduct)
		{
			dev_err(&gadget->dev, "idVendor needs idProduct!\n");
			return -ENODEV;
		}
		device_desc.idVendor = cpu_to_le16(idVendor);
		device_desc.idProduct = cpu_to_le16(idProduct);
		if (bcdDevice)
			device_desc.bcdDevice = cpu_to_le16(bcdDevice);
	}
#endif /*ndef MUX */

	len = strlen(pnp_string);
	pnp_string[0] = (len >> 8) & 0xFF;
	pnp_string[1] = len & 0xFF;

	/* all we really need is bulk IN/OUT */
#ifndef MUX
	usb_ep_autoconfig_reset(gadget);
#endif
	in_ep = usb_ep_autoconfig(gadget, &fs_ep_in_desc);

	if (!in_ep)
	{
	  autoconf_fail:
		dev_err(&gadget->dev, "can't autoconfigure on %s\n", gadget->name);
		return -ENODEV;
	}
	in_ep->driver_data = in_ep;	/* claim */

	out_ep = usb_ep_autoconfig(gadget, &fs_ep_out_desc);

	if (!out_ep)
		goto autoconf_fail;
	out_ep->driver_data = out_ep;	/* claim */

#ifdef	CONFIG_USB_GADGET_DUALSPEED
	/* assumes ep0 uses the same value for both speeds ... */
#ifndef MUX
	dev_qualifier.bMaxPacketSize0 = device_desc.bMaxPacketSize0;
#endif

	/* and that all endpoints are dual-speed */
	hs_ep_in_desc.bEndpointAddress = fs_ep_in_desc.bEndpointAddress;
	hs_ep_out_desc.bEndpointAddress = fs_ep_out_desc.bEndpointAddress;
#endif /* DUALSPEED */

#ifndef MUX
	device_desc.bMaxPacketSize0 = gadget->ep0->maxpacket;
	usb_gadget_set_selfpowered(gadget);

	if (gadget->is_otg)
	{
		otg_desc.bmAttributes |= USB_OTG_HNP, config_desc.bmAttributes |= USB_CONFIG_ATT_WAKEUP;
		config_desc.bMaxPower = 4;
	}
#endif

	spin_lock_init(&dev->lock);
	spin_lock_init(&dev->lock_printer_io);

	INIT_LIST_HEAD(&dev->rx_reqs);
	INIT_LIST_HEAD(&dev->rx_reqs_active);
	INIT_LIST_HEAD(&dev->rx_buffers);
	dev->config = 0;
	dev->interface = -1;
	dev->printer_cdev_open = 0;
	dev->printer_status_cdev_open = 0;
	dev->zlp = zlp;
	dev->printer_status = PRINTER_NOT_ERROR | PRINTER_SELECTED;
	dev->current_rx_req = NULL;
	dev->current_rx_bytes = 0;
	dev->current_rx_buf = NULL;

	dev->in_ep = in_ep;
	dev->out_ep = out_ep;

#ifndef MUX
	/* preallocate control message data and buffer */
	dev->req = printer_req_alloc(gadget->ep0, USB_DESC_BUFSIZE, GFP_KERNEL);

	if (!dev->req)
	{
		status = -ENOMEM;
		goto fail;
	}
#endif

	for (i = 0; i < QLEN; i++)
	{
		req = printer_req_alloc(dev->out_ep, USB_BUFSIZE, GFP_KERNEL);
		if (!req)
		{
			while (likely(!list_empty(&dev->rx_reqs)))
			{
				req = container_of(dev->rx_reqs.next, struct usb_request, list);
				list_del(&dev->rx_reqs);
				printer_req_free(dev->out_ep, req);
			}
			return -ENOMEM;
		}
		list_add(&req->list, &dev->rx_reqs);
	}

#ifndef MUX
	dev->req->complete = printer_setup_complete;
#endif

	/* finish hookup to lower layer ... */
	dev->gadget = gadget;
	set_gadget_data(gadget, dev);
#ifndef MUX
	gadget->ep0->driver_data = dev;
#endif

	INFO(dev, "%s, version: " DRIVER_VERSION "\n", driver_desc);
	INFO(dev, "using %s, OUT %s IN %s\n", gadget->name, out_ep->name, in_ep->name);

	return 0;

  fail:
	printer_unbind(gadget);
	return status;
}

/****************************************************************************/
/* NAME:   printer_suspend					            					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:   Invoked by UDC driver on USB suspend                      */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           gadget structure filled by UDC driver                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:        NONE                                                      */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_suspend(struct usb_gadget *gadget)
{
	struct printer_dev *dev = get_gadget_data(gadget);

	DEBUG(dev, "suspend\n");
	dev->suspended = 1;
}

/****************************************************************************/
/* NAME:      printer_resume						    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Called from UDC driver when  printer is resumed after    */
/*                 suspending                                               */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           gadget structure filled by UDC driver                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
printer_resume(struct usb_gadget *gadget)
{
	struct printer_dev *dev = get_gadget_data(gadget);

	DEBUG(dev, "resume\n");
	dev->suspended = 0;
}

/*-------------------------------------------------------------------------*/

#ifndef MUX
static struct usb_gadget_driver printer_driver = {
	.speed = DEVSPEED,

	.function = (char *) driver_desc,
	.bind = printer_bind,
	.unbind = printer_unbind,

	.setup = printer_setup,
	.disconnect = printer_disconnect,

	.suspend = printer_suspend,
	.resume = printer_resume,

	.driver = {
			   .name = (char *) shortname,
			   .owner = THIS_MODULE,
			   },
};
#else
static struct tMUX_CompositeGadgetDriver printer_driver = {

	.function = (char *) driver_desc,
	.interface_no = 0,
	.bind = printer_bind,
	.unbind = printer_unbind,

	.setup = printer_setup,
	.disconnect = printer_disconnect,

	.suspend = printer_suspend,
	.resume = printer_resume,

	.set_config = printer_set_config,
	.reset_config = disable_printer_interface,

	.hs_descriptor = hs_printer_function,
	.fs_descriptor = fs_printer_function,

};

#endif

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Rajeev Kumar");
MODULE_LICENSE("GPL");

/****************************************************************************/
/* NAME:      _init init						    						*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Called when printer module is loaded                   */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           NONE                                                     */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:           0 on success error otherwise                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int __init
init(void)
{
	int      status;
	status = register_chrdev_region(g_printer_devno, 2, "USB printer (gadget)");
	if (status)
	{
		ERROR(dev, "register_chrdev_region %x\n", status);
		return status;
	}

#ifdef MUX
	status = SPEAr_MUX_UsbGadgetRegisterDriver(&printer_driver);
#else
	status = usb_gadget_register_driver(&printer_driver);
#endif
	if (status)
	{
		unregister_chrdev_region(g_printer_devno, 2);
		DEBUG(dev, "usb_gadget_register_driver %x\n", status);
	}

#ifdef MUX
	printk("Printer gadget driver with MUX support\n");
#else
	printk("Printer gadget driver without MUX support\n");
#endif /*ndef MUX */

	return status;
}

module_init(init);

/****************************************************************************/
/* NAME:      __exit cleanup (void)					    					*/
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when printer module is unloaded                     */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void __exit
cleanup(void)
{
	int             status;

	spin_lock(&usb_printer_gadget.lock_printer_io);
	unregister_chrdev_region(g_printer_devno, 2);

#ifdef MUX
	status = SPEAr_MUX_UsbGadgetUnregisterDriver(&printer_driver);
#else
	status = usb_gadget_unregister_driver(&printer_driver);
#endif
	if (status)
	{
		ERROR(dev, "usb_gadget_unregister_driver %x\n", status);
	}
	spin_unlock(&usb_printer_gadget.lock_printer_io);
}

module_exit(cleanup);
