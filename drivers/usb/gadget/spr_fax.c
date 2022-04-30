/******************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Rajeev Kumar                    */
/*   rajeev-dlh.kumar@st.com						      */
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

#define DEBUG 		1
#define VERBOSE 	1

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
#include <linux/suspend.h>
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

//# define printk(fmt...)

#define MUX		// Comment this  for independent fax gadget driver without MUX layer
#define test_intr	//enable to include interrupt end point

#ifdef MUX
#include "spr_mux.h"
#endif

#define USB_REQ_LAST	1

#define DRIVER_DESC		"Fax Gadget"
#define DRIVER_VERSION		"0.1.1 03/17/2006"

static const char shortname[] = "fax";
static const char driver_desc[] = DRIVER_DESC;

static const int g_fax_devno = MKDEV(247, 4);
static const int g_fax_status_devno = MKDEV(247, 5);

/*------------------------------------------------------------------------*/

 /* the device structure holds info for the USB device */

struct fax_dev
{
  raw_spinlock_t      lock;			/*locking for device */
  raw_spinlock_t      lock_fax_io;	/*locking for read and write oprations */
  struct usb_gadget *gadget;	/*For taking services of UDC */
#ifndef MUX
  struct usb_request *req;		/* for control responses */
#endif
  u8              config;		/*configuration number to be used */
  s8              interface;	/*Interface number to be used */
  struct usb_ep  *in_ep, *out_ep;	/*BulkIn and BulkOut endpoint */
  struct usb_ep  *intr_ep;		/*Interrupt endpoint */
  const struct usb_endpoint_descriptor *in, *out, *intr;
  struct list_head rx_reqs;		/*linked list for received request */
  struct list_head rx_reqs_active;	/*linked list for received to be request */
  struct list_head rx_buffers;	/*linked list for received data */
  struct list_head tx_reqs;
  struct list_head tx_reqs_active;
#ifdef test_intr
  struct usb_request *intr_req;	/*current  received request */
#endif
  struct usb_request *current_rx_req;	/*current  received request */
  size_t          current_rx_bytes;	/* current receive bytes */
  u8             *current_rx_buf;	/* current  received buffer */
  u8              fax_status;
  u8              reset_fax;
  unsigned        suspended:1;	/*set this bit if the fax is in suspended mode */
  unsigned        zlp:1;
  struct cdev     fax_cdev;
  u8              fax_cdev_open;
  struct cdev     fax_status_cdev;
  u8              fax_status_cdev_open;

  volatile u8     tx_success;
  volatile u8     tx_status_busy;
  volatile u8     terminated;

#ifdef test_intr
  struct completion thread_notifier;
#endif

};

static struct fax_dev usb_fax_gadget;

#ifndef MUX
/*------------------------------------------------------------------------*/

#define FAX_VENDOR_NUM	0x1234	/* Epson */
#define FAX_PRODUCT_NUM	0x4321	/* Epson */

/* ----------------------------------------------------------------------- */
/*		module prameters					   */
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
#define USB_OUT_BUFSIZE			512 


#define USB_IN_BUFSIZE			512
#define USB_INTR_BUFSIZE		64

/* This device advertises one configuration. */
#define DEFAULT_CONFIG_VALUE		3

#define	FAX_INTERFACE		0
static          DECLARE_WAIT_QUEUE_HEAD(wait_read_fax);
static          DECLARE_WAIT_QUEUE_HEAD(wait_write_fax);
static          DECLARE_WAIT_QUEUE_HEAD(wait_intr_fax);

/*------------------------------------------------------------------------------*/
/*	usb_device_descriptor device_desc					*/
/*------------------------------------------------------------------------------*/

#ifndef MUX
static struct usb_device_descriptor device_desc = {
  .bLength = sizeof device_desc,
  .bDescriptorType = USB_DT_DEVICE,
  .bcdUSB = __constant_cpu_to_le16(0x0200),
  .bDeviceClass = USB_CLASS_PER_INTERFACE,
  .bDeviceSubClass = 0,
  .bDeviceProtocol = 0,
  .idVendor = __constant_cpu_to_le16(FAX_VENDOR_NUM),
  .idProduct = __constant_cpu_to_le16(FAX_PRODUCT_NUM),
  .iManufacturer = STRING_MANUFACTURER,
  .iProduct = STRING_PRODUCT,
  .iSerialNumber = STRING_SERIALNUM,
  .bNumConfigurations = 1
};

/*------------------------------------------------------------------------------*/
/*	usb_otg_descriptor otg_desc						*/
/*------------------------------------------------------------------------------*/

static struct usb_otg_descriptor otg_desc = {
  .bLength = sizeof otg_desc,
  .bDescriptorType = USB_DT_OTG,
  .bmAttributes = USB_OTG_SRP
};

/*------------------------------------------------------------------------------*/
/*	usb_config_descriptor config_desc 					*/
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
/*	 usb_interface_descriptor intf_desc					*/
/*------------------------------------------------------------------------------*/

#endif /*ndef MUX */

static struct usb_interface_descriptor intf_desc = {
  .bLength = sizeof intf_desc,
  .bDescriptorType = USB_DT_INTERFACE,
  .bInterfaceNumber = FAX_INTERFACE,
#ifdef test_intr
  .bNumEndpoints = 3,
#else
  .bNumEndpoints = 2,
#endif
  .bInterfaceClass = USB_CLASS_VENDOR_SPEC,
  .bInterfaceSubClass = 0,
  .bInterfaceProtocol = 0xff,
  .iInterface = 0
};

/*------------------------------------------------------------------------------*/
/*	usb_endpoint_descriptor fs_ep_in_desc					*/
/*------------------------------------------------------------------------------*/

static struct usb_endpoint_descriptor fs_ep_in_desc = {
  .bLength = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_IN,
  .bmAttributes = USB_ENDPOINT_XFER_BULK
};

/*------------------------------------------------------------------------------*/
/*	usb_endpoint_descriptor fs_ep_out_desc					*/
/*------------------------------------------------------------------------------*/

static struct usb_endpoint_descriptor fs_ep_out_desc = {
  .bLength = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_OUT,
  .bmAttributes = USB_ENDPOINT_XFER_BULK
};

static struct usb_endpoint_descriptor fs_ep_intr_desc = {
  .bLength = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_IN,
  .bmAttributes = USB_ENDPOINT_XFER_INT,
  //.bmAttributes = USB_ENDPOINT_XFER_BULK,
  .wMaxPacketSize = __constant_cpu_to_le16(64),
  .bInterval = 1 << 5,

};

static const struct usb_descriptor_header *fs_fax_function[11] = {
#ifndef MUX
  (struct usb_descriptor_header *) &otg_desc,
#endif
  (struct usb_descriptor_header *) &intf_desc,
  (struct usb_descriptor_header *) &fs_ep_in_desc,
  (struct usb_descriptor_header *) &fs_ep_out_desc,
#ifdef test_intr
 (struct usb_descriptor_header *) &fs_ep_intr_desc,
#endif
  NULL
};

#ifdef	CONFIG_USB_GADGET_DUALSPEED

/*----------------------------------------------------------------------*/
/* usb 2.0 devices need to expose both high speed and full speed	*/
/* descriptors, unless they only run at full speed.			*/
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

static struct usb_endpoint_descriptor hs_ep_intr_desc = {
  .bLength = USB_DT_ENDPOINT_SIZE,
  .bDescriptorType = USB_DT_ENDPOINT,
  .bEndpointAddress = USB_DIR_IN,
  .bmAttributes = USB_ENDPOINT_XFER_INT,
  .wMaxPacketSize = __constant_cpu_to_le16(64),
  .bInterval = 9,

};

#ifndef MUX

static struct usb_qualifier_descriptor dev_qualifier = {
  .bLength = sizeof dev_qualifier,
  .bDescriptorType = USB_DT_DEVICE_QUALIFIER,
  .bcdUSB = __constant_cpu_to_le16(0x0200),
  .bDeviceClass = USB_CLASS_VENDOR_SPEC,
  .bNumConfigurations = 1
};

#endif /*ndef MUX */

static const struct usb_descriptor_header *hs_fax_function[11] = {
#ifndef MUX
  (struct usb_descriptor_header *) &otg_desc,
#endif
  (struct usb_descriptor_header *) &intf_desc,
  (struct usb_descriptor_header *) &hs_ep_in_desc,
  (struct usb_descriptor_header *) &hs_ep_out_desc,
#ifdef test_intr
  (struct usb_descriptor_header *) &hs_ep_intr_desc,
#endif
  NULL
};

/* maxpacket and other transfer characteristics vary by speed. */
#define ep_desc(g,hs,fs) (((g)->speed==USB_SPEED_HIGH)?(hs):(fs))

#else

#define ep_desc(g,hs,fs) (((void)(g)), (fs))

#endif /* !CONFIG_USB_GADGET_DUALSPEED */

/* descriptors that are built on-demand */

#ifndef MUX

static char     manufacturer[50];
static char     product_desc[40] = DRIVER_DESC;
static char     serial_num[40] = "1";
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

#ifdef test_intr
static void
tx_status_complete(struct usb_ep *ep, struct usb_request *req)
{
  struct fax_dev *dev = ep->driver_data;

  DEBUG(dev, "tx_status_complete called status=%d  %c\n", req->status, *((char *) req->buf));

  switch (req->status)
  {
  default:
	VDEBUG(dev, "tx err %d\n", req->status);
	/* FALLTHROUGH */
  case -ECONNRESET:			// unlink
  case -ESHUTDOWN:				// disconnect etc
	break;
  case 0:
	break;
  }

  spin_lock(&dev->lock);

  dev->tx_status_busy = 0;
  wake_up_interruptible(&wait_intr_fax);

  spin_unlock(&dev->lock);
}
#endif

#ifdef test_intr

static int
sleep_thread(struct fax_dev *dev)
{
  int             rc;

  /* Wait until a we are woken up */
  rc = wait_event_interruptible(wait_intr_fax, (!dev->tx_status_busy || dev->terminated));
  if (current->flags & PF_FREEZE)
	refrigerator(PF_FREEZE);
  return (rc ? -EINTR : 0);
}

static int
fax_main_thread(void *_dev)
{
  struct fax_dev *dev = (struct fax_dev *) _dev;
  int             size = 1;
  unsigned long   flags;
  struct usb_request *req;

  daemonize("fax-gadget");
  set_fs(get_ds());

  wait_for_completion(&dev->thread_notifier);

  while (!dev->terminated)
  {
	if (dev->tx_status_busy)
	  sleep_thread(dev);

	if (dev->terminated)
	  break;

	spin_lock(&dev->lock_fax_io);
	spin_lock_irqsave(&dev->lock, flags);

	dev->tx_status_busy = 1;

	req = dev->intr_req;

	req->complete = tx_status_complete;
	req->length = size;

	req->zero = 1;

	/* Don't leave irqs off while doing memory copies */
	spin_unlock_irqrestore(&dev->lock, flags);

	memcpy(req->buf, &dev->fax_status, size);

	spin_lock_irqsave(&dev->lock, flags);

	if (usb_ep_queue(dev->intr_ep, req, GFP_ATOMIC))
	{
	  dev->tx_status_busy = 0;
	}

	spin_unlock_irqrestore(&dev->lock, flags);
	spin_unlock(&dev->lock_fax_io);

  }

  complete_and_exit(&dev->thread_notifier, 0);
}

/* thread over */

#endif

/*-------------------------------------------------------------------------*/

/****************************************************************************/
/* NAME:        SPEAr_Fax_ReqAlloc                                      */
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
SPEAr_Fax_ReqAlloc(struct usb_ep *ep, unsigned size, int gfp_flags)
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
/* NAME:     SPEAr_Fax_ReqFree                                          */
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
SPEAr_Fax_ReqFree(struct usb_ep *ep, struct usb_request *req)
{
  usb_ep_free_buffer(ep, req->buf, req->dma, req->length);
  usb_ep_free_request(ep, req);
}

/****************************************************************************/
/* NAME:     rx_complete  		                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from UDC when data is recevied                      */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      ep:  endpoint for which data is arrived			    */
/*            struct usb_request :adding active bufffer to                  */
/*	        req_list						    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    NONE			                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void
rx_complete(struct usb_ep *ep, struct usb_request *req)
{
  struct fax_dev *dev = ep->driver_data;
  int             status = req->status;
  unsigned long   flags;

  spin_lock_irqsave(&dev->lock, flags);

  list_del_init(&req->list);	/* Remove from Active List */

  DEBUG(dev, "%s:rx complete status=%d len=%d\n", __FUNCTION__, status, req->actual);

  switch (status)
  {

	/* normal completion */
  case 0:
	list_add_tail(&req->list, &dev->rx_buffers);
	DEBUG(dev, "G_FAX : rx length %d\n", req->actual);

	wake_up_interruptible(&wait_read_fax);
	break;

	/* software-driven interface shutdown */
  case -ECONNRESET:			// unlink
  case -ESHUTDOWN:				// disconnect etc
	VDEBUG(dev, "rx shutdown, code %d\n", status);

	list_add(&req->list, &dev->rx_reqs);
	break;

	/* for hardware automagic (such as pxa) */
  case -ECONNABORTED:			// endpoint reset
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
/* NAME:    SPEAr_Fax_Open   		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    */
/*             1 = FAILURE                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
SPEAr_Fax_Open(struct inode *inode, struct file *fd)
{
  struct fax_dev *dev;
  unsigned long   flags;
  int             ret = -EBUSY;

  dev = container_of(inode->i_cdev, struct fax_dev, fax_cdev);

  spin_lock_irqsave(&dev->lock, flags);

  if (!dev->fax_cdev_open)
  {
	dev->fax_cdev_open = 1;
	fd->private_data = dev;
	ret = 0;
  }

  spin_unlock_irqrestore(&dev->lock, flags);

  DEBUG(dev, "SPEAr_Fax_Open returned %x\n", ret);

  return ret;
}

/****************************************************************************/
/* NAME:    SPEAr_Fax_Close   		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    0 = SUCCESS		                                    */
/*            1 = FAILURE                                                   */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
SPEAr_Fax_Close(struct inode *inode, struct file *fd)
{
  struct fax_dev *dev;
  struct usb_request *req;
  unsigned long   flags;

  dev = container_of(inode->i_cdev, struct fax_dev, fax_cdev);

  spin_lock_irqsave(&dev->lock, flags);
  spin_lock(&dev->lock_fax_io);

  while (!(list_empty(&dev->tx_reqs_active)))
  {
	req = container_of(dev->tx_reqs_active.next, struct usb_request, list);
	if (usb_ep_dequeue(dev->in_ep, req))
	  DEBUG(dev, "Failed to dequeue in req %p\n", req);

	list_del_init(&req->list);
	list_add(&req->list, &dev->tx_reqs);
  }

  while (!(list_empty(&dev->rx_reqs_active)))
  {
	req = container_of(dev->rx_reqs_active.next, struct usb_request, list);
	if (usb_ep_dequeue(dev->out_ep, req))
	  DEBUG(dev, "Failed to dequeue in req %p\n", req);

	list_del_init(&req->list);
	list_add(&req->list, &dev->rx_reqs);
  }

  dev->fax_cdev_open = 0;

  spin_unlock(&dev->lock_fax_io);
  spin_unlock_irqrestore(&dev->lock, flags);

  DEBUG(dev, "SPEAr_Fax_Close\n");

  return 0;
}

/****************************************************************************/
/* NAME:    SPEAr_Fax_Read   		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    */
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes read		                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static          ssize_t
SPEAr_Fax_Read(struct file *fd, char __user * buf, size_t len, loff_t * ptr)
{
  struct fax_dev *dev = fd->private_data;
  unsigned long   flags;
  size_t          size;
  size_t          bytes_copied;
  struct usb_request *req;
  struct usb_request *current_rx_req;
  size_t          current_rx_bytes;
  u8             *current_rx_buf;

  if (len == 0)
	return 0;

  DEBUG(dev, "SPEAr_Fax_Read trying to read %d bytes\n", (int) len);

  spin_lock(&dev->lock_fax_io);
  spin_lock_irqsave(&dev->lock, flags);

  /* Check if a fax reset happens while we have interrupts on */
  dev->reset_fax = 0;

  while (!list_empty(&dev->rx_reqs))
  {
	int             error;

	req = container_of(dev->rx_reqs.next, struct usb_request, list);
	list_del_init(&req->list);

	req->length = USB_OUT_BUFSIZE;
	req->complete = rx_complete;

	error = usb_ep_queue(dev->out_ep, req, GFP_ATOMIC);

	/* We've disconnected or reset free the req and buffer */
	if (dev->reset_fax)
	{
	  if (!error)
	  {
		if (usb_ep_dequeue(dev->out_ep, req))
		{
		  DEBUG(dev, "Failed to dequeue " "in req %p\n", req);
		}
	  }
	  SPEAr_Fax_ReqFree(dev->out_ep, req);
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);
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
	spin_unlock_irqrestore(&dev->lock, flags);
	spin_unlock(&dev->lock_fax_io);

	wait_event_interruptible(wait_read_fax, !list_empty(&dev->rx_buffers));

	spin_lock(&dev->lock_fax_io);
	spin_lock_irqsave(&dev->lock, flags);
  }

  while ((current_rx_bytes || !list_empty(&dev->rx_buffers)) && len)
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
	if (dev->reset_fax)
	{
	  SPEAr_Fax_ReqFree(dev->out_ep, current_rx_req);
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);
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
  spin_unlock(&dev->lock_fax_io);

  DEBUG(dev, "SPEAr_Fax_Read returned %d bytes\n", (int) bytes_copied);

  return bytes_copied;
}

static void
tx_complete(struct usb_ep *ep, struct usb_request *req)
{
  struct fax_dev *dev = ep->driver_data;

  if( !req || !ep ) 
   return;

  DEBUG(dev, "%s: status=%d context=%d\n", __FUNCTION__, req->status, *((char *) req->context));
  switch (req->status)
  {
  default:
	VDEBUG(dev, "tx err %d\n", req->status);
	/* FALLTHROUGH */
  case -ECONNRESET:			// unlink
  case -ESHUTDOWN:			// disconnect etc
	break;
  case 0:
	break;
  }

  spin_lock(&dev->lock);
  /* Take the request struct off the active list and put it on the
   * free list.
   */

  if (*((char *) req->context) == USB_REQ_LAST)
	dev->tx_success = 1;

  list_del_init(&req->list);
  list_add(&req->list, &dev->tx_reqs);

  wake_up_interruptible(&wait_write_fax);

  spin_unlock(&dev->lock);
}

/****************************************************************************/
/* NAME:    SPEAr_Fax_Write      	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application to send data			    */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      								    */
/*            fd :file opearation pointer		                    */
/*            buff:data to write         		                    */
/*            len: data length           		                    */
/*            ptr :offset pointer       		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:      bytes wrote on SUCCESS		                            */
/*	       	   -1 on FAILURE					    */
/* REENTRANCY:                                                              */
/****************************************************************************/

static   ssize_t
SPEAr_Fax_Write(struct file *fd, const char __user * buf, size_t len, loff_t * ptr)
{
  struct fax_dev *dev = fd->private_data;
  unsigned long   flags;
  size_t          size;
  size_t          bytes_copied = 0;
  struct usb_request *req;
  unsigned int    i;
  static  char not_last_req = 0;
  static  char last_req = USB_REQ_LAST;

  DEBUG(dev, "SPEAr_Fax_Write trying to send %d bytes\n", (int) len);
  if(dev->interface < 0 ) 
      return 0;

  if (len == 0)
	return 0;

  spin_lock(&dev->lock_fax_io);
  spin_lock_irqsave(&dev->lock, flags);

  /* Check if a fax reset happens while we have interrupts on */
  dev->reset_fax = 0;

  while (len)
  {
	dev->tx_success = 0;
	if (list_empty(&dev->tx_reqs))
	{
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);

	  if (list_empty(&dev->tx_reqs))
	  {
		wait_event_interruptible(wait_write_fax, !list_empty(&dev->tx_reqs));
	  }
	  if (list_empty(&dev->tx_reqs))
	  {
	  	return -1; //error
	  }

	  spin_lock(&dev->lock_fax_io);
	  spin_lock_irqsave(&dev->lock, flags);
	}

	/* Some hardware doesn't like to write zlps. Hopefully the
	 * extra data byte won't cause any problems.
	 */
	if (len < USB_IN_BUFSIZE && !dev->zlp && (len % dev->in_ep->maxpacket) == 0)
	{
	  len++;
	}

	if (len > USB_IN_BUFSIZE)
	  size = USB_IN_BUFSIZE;
	else
	  size = len;

	req = container_of(dev->tx_reqs.next, struct usb_request, list);
	list_del_init(&req->list);

	req->complete = tx_complete;
	req->length = size;

	if (len > size)
	  req->zero = 0;
	else
	  req->zero = ((len % dev->in_ep->maxpacket) == 0);

	/* Don't leave irqs off while doing memory copies */
	spin_unlock_irqrestore(&dev->lock, flags);

	if (copy_from_user(req->buf, buf, size))
	{
	  list_add(&req->list, &dev->tx_reqs);
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);
	  return bytes_copied;
	}

	bytes_copied += size;
	len -= size;
	buf += size;

	spin_lock_irqsave(&dev->lock, flags);

	/* We've disconnected or reset free the req and buffer */
	if (dev->reset_fax)
	{
	  SPEAr_Fax_ReqFree(dev->in_ep, req);
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);
	  return 0;
	}

	if (len > 0)
	{
	  req->context = &not_last_req;
	}
	else
	{
	  req->context = &last_req;;
	}

	if (usb_ep_queue(dev->in_ep, req, GFP_ATOMIC))
	{
	  list_add(&req->list, &dev->tx_reqs);
	  spin_unlock_irqrestore(&dev->lock, flags);
	  spin_unlock(&dev->lock_fax_io);
	  return 0;
	}

	list_add(&req->list, &dev->tx_reqs_active);

  }

  spin_unlock_irqrestore(&dev->lock, flags);

  for (i = 0; i < 99999999; i++)
	for (i = 0; i < 99999999; i++)
	  for (i = 0; i < 99999999; i++)
	  {
		if (dev->tx_success)
		{
		  spin_unlock(&dev->lock_fax_io);
		  return bytes_copied;
		}
	  }

  spin_lock_irqsave(&dev->lock, flags);

  while (!(list_empty(&dev->tx_reqs_active)))
  {
	req = container_of(dev->tx_reqs_active.next, struct usb_request, list);
	if (usb_ep_dequeue(dev->in_ep, req))
	  DEBUG(dev, "Failed to dequeue in req %p\n", req);

	list_del_init(&req->list);
	list_add(&req->list, &dev->tx_reqs);
  }

  spin_unlock_irqrestore(&dev->lock, flags);
  spin_unlock(&dev->lock_fax_io);

  return -1;

  DEBUG(dev, "SPEAr_Fax_Write sent %d bytes\n", (int) bytes_copied);
}

/* used after endpoint configuration */
static struct file_operations fax_io_operations = {
  .owner = THIS_MODULE,
  .open = SPEAr_Fax_Open,
  .read = SPEAr_Fax_Read,
  .write = SPEAr_Fax_Write,
  .release = SPEAr_Fax_Close
};

/****************************************************************************/
/* NAME:    SPEAr_Fax_StatusOpen 	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:     0 = SUCCESS		                                    */
/*	       1 = FAILURE						    */
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
SPEAr_Fax_StatusOpen(struct inode *inode, struct file *fd)
{
  struct fax_dev *dev;
  unsigned long   flags;
  int             ret = -EBUSY;

  dev = container_of(inode->i_cdev, struct fax_dev, fax_status_cdev);

  spin_lock_irqsave(&dev->lock, flags);

  if (!dev->fax_status_cdev_open)
  {
	dev->fax_status_cdev_open = 1;
	fd->private_data = dev;
	ret = 0;
  }

  spin_unlock_irqrestore(&dev->lock, flags);

  DEBUG(dev, "SPEAr_Fax_StatusOpen returned %x\n", ret);

  return ret;
}

/****************************************************************************/
/* NAME:  SPEAr_Fax_StatusClose   	                                    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  called from application for particular char device         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      inode:  for accessing file				    */
/*            fd :file opearation pointer		                    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    0 = SUCCESS			                            */
/*	      1 = FAILURE						    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int
SPEAr_Fax_StatusClose(struct inode *inode, struct file *fd)
{
  struct fax_dev *dev;
  unsigned long   flags;

  dev = container_of(inode->i_cdev, struct fax_dev, fax_status_cdev);

  spin_lock_irqsave(&dev->lock, flags);

  dev->fax_status_cdev_open = 0;

#ifdef test_intr
  if (dev->intr_ep && dev->intr_req)
	usb_ep_dequeue(dev->intr_ep, dev->intr_req);
#endif

  spin_unlock_irqrestore(&dev->lock, flags);

  DEBUG(dev, "SPEAr_Fax_StatusClose\n");

  return 0;
}

/****************************************************************************/
/* NAME:   SPEAr_Fax_StatusRead 		                             */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    */
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes read		                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static          ssize_t
SPEAr_Fax_StatusRead(struct file *fd, char __user * buf, size_t len, loff_t * ptr)
{
  struct fax_dev *dev;
  unsigned long   flags;
  int             ret;

  if (len <= 0)
	return 0;

  dev = fd->private_data;

  spin_lock_irqsave(&dev->lock, flags);

  ret = !(copy_to_user(buf, &dev->fax_status, 1));

  spin_unlock_irqrestore(&dev->lock, flags);

  DEBUG(dev, "SPEAr_Fax_StatusRead =0x%x bytes copied = %d\n", dev->fax_status, ret);

  return ret;
}

/****************************************************************************/
/* NAME:   SPEAr_Fax_StatusWrite		                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  provide interface to to application for reading data.read  */
/* from the file associated with the open file descriptor		    */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      len: data length				  		    */
/*            fd :file discriptor associated with the file                  */
/* OUT :      buf: The buffer to filled with data                           */
/*                                                                          */
/* RETURN:    bytes copied		                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

#ifdef test_intr
static          ssize_t
SPEAr_Fax_StatusWrite(struct file *fd, const char __user * buf, size_t len, loff_t * ptr)
{
  struct fax_dev *dev;
  unsigned long   flags;
  int             ret;
  struct usb_request *req;

  if (len <= 0)
	return 0;

  dev = fd->private_data;

  spin_lock(&dev->lock_fax_io);

  ret = !(copy_from_user(&dev->fax_status, buf, sizeof(dev->fax_status)));

  spin_lock_irqsave(&dev->lock, flags);

  if (dev->tx_status_busy)
  {
	/* dequeue previous request */
	usb_ep_dequeue(dev->intr_ep, dev->intr_req);
	dev->tx_status_busy = 0;
  }

  req = dev->intr_req;

  req->complete = tx_status_complete;
  req->length = sizeof(dev->fax_status);


  req->zero = ((len % dev->intr_ep->maxpacket) == 0);

  /* Don't leave irqs off while doing memory copies */
  spin_unlock_irqrestore(&dev->lock, flags);

  memcpy(req->buf, &dev->fax_status, sizeof(dev->fax_status));

  spin_lock_irqsave(&dev->lock, flags);

  if (usb_ep_queue(dev->intr_ep, req, GFP_ATOMIC))
  {
	dev->tx_status_busy = 0;
  }

  dev->tx_status_busy = 1;

  spin_unlock_irqrestore(&dev->lock, flags);
  spin_unlock(&dev->lock_fax_io);

  DEBUG(dev, "SPEAr_Fax_StatusWrite wrote %x\n", dev->fax_status);

  return ret;
}
#endif

/* used after endpoint configuration */
static struct file_operations fax_status_operations = {
  .owner = THIS_MODULE,
  .open = SPEAr_Fax_StatusOpen,
  .read = SPEAr_Fax_StatusRead,
#ifdef test_intr
  .write = SPEAr_Fax_StatusWrite,
#endif
  .release = SPEAr_Fax_StatusClose
};

/****************************************************************************/
/* NAME:       SPEAr_Fax_EnableInterface				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      enable endps   					    */
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
SPEAr_Fax_EnableInterface(struct fax_dev *dev)
{
  int             result = 0;

  if( dev->interface >= 0 )
	  return 0;

  dev->in = ep_desc(dev->gadget, &hs_ep_in_desc, &fs_ep_in_desc);
  dev->in_ep->driver_data = dev;

  dev->out = ep_desc(dev->gadget, &hs_ep_out_desc, &fs_ep_out_desc);
  dev->out_ep->driver_data = dev;

#ifdef test_intr
  dev->intr = ep_desc(dev->gadget, &hs_ep_intr_desc, &fs_ep_intr_desc);
  dev->intr_ep->driver_data = dev;
#endif
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
#ifdef test_intr
  result = usb_ep_enable(dev->intr_ep, dev->intr);
  if (result != 0)
  {
	DEBUG(dev, "enable %s --> %d\n", dev->intr_ep->name, result);
	goto done;
  }
#endif

    dev->interface = 0;

done:
  /* on error, disable any endpoints  */
  if (result != 0)
  {
	(void) usb_ep_disable(dev->in_ep);
	(void) usb_ep_disable(dev->out_ep);
	(void) usb_ep_disable(dev->intr_ep);
	dev->in = NULL;
	dev->out = NULL;
#ifdef test_intr
	dev->intr = NULL;
#endif
  }

  /* caller is responsible for cleanup on error */
  return result;
}

/****************************************************************************/
/* NAME:       disable_ fax_interface				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      disable endps   					    */
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
SPEAr_Fax_DisableInterface(void *device)
{
  struct fax_dev *dev = (struct fax_dev *) device;

  if (dev->interface < 0)
	return;

  DEBUG(dev, "%s\n", __FUNCTION__);

  if (dev->in)
	usb_ep_disable(dev->in_ep);

  if (dev->out)
	usb_ep_disable(dev->out_ep);

#ifdef test_intr
  if (dev->intr)
	usb_ep_disable(dev->intr_ep);
#endif

  dev->interface = -1;
  dev->config =     0; 
}

/****************************************************************************/
/* NAME:      SPEAr_Fax_SetConfig					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     set specific(config) configuration i.e. enable endps    */
/*                  as per config.change our operational config.	    */
/*	           must agree with the code that returns config descriptors */
/*		  , and altsetting code.                                    */
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
SPEAr_Fax_SetConfig(void *device, unsigned number)
{
  int             result = -EOPNOTSUPP;
  unsigned long flags;struct usb_request *req;
  struct fax_dev *dev = (struct fax_dev *) device;
#ifndef MUX
  struct usb_gadget *gadget = dev->gadget;
#endif /*ndef MUX */


  /* First disable the current configuration */
  switch (dev->config)
  {
  case DEFAULT_CONFIG_VALUE:
	SPEAr_Fax_DisableInterface(dev);
	break;
  }

  /* Now enable the new configuration */
  switch (number)
  {
  case DEFAULT_CONFIG_VALUE:
	result = SPEAr_Fax_EnableInterface(dev);
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

#ifdef test_intr
	/* queue fax status */
	
	
	spin_lock(&dev->lock_fax_io);
	spin_lock_irqsave(&dev->lock, flags);

	if (dev->tx_status_busy)
	{
	    /* dequeue previous request */
	    usb_ep_dequeue(dev->intr_ep, dev->intr_req);
	    dev->tx_status_busy = 0;
	}

	req = dev->intr_req;

	req->complete = tx_status_complete;
	req->length = sizeof(dev->fax_status);
	req->zero = 1;

	/* Don't leave irqs off while doing memory copies */
	spin_unlock_irqrestore(&dev->lock, flags);

	memcpy(req->buf, &dev->fax_status, sizeof(dev->fax_status));

	spin_lock_irqsave(&dev->lock, flags);

	if (usb_ep_queue(dev->intr_ep, req, GFP_ATOMIC))
	{
	    dev->tx_status_busy = 0;
	}

	dev->tx_status_busy = 1;

	spin_unlock_irqrestore(&dev->lock, flags);
	spin_unlock(&dev->lock_fax_io);
#endif

	dev->config = number;
	while (!list_empty(&dev->rx_reqs))
	{
	  int             error;
	  struct usb_request *req;

	  req = container_of(dev->rx_reqs.next, struct usb_request, list);
	  list_del_init(&req->list);

	  req->length = USB_OUT_BUFSIZE; 
	  req->complete = rx_complete;
	  error = usb_ep_queue(dev->out_ep, req, GFP_ATOMIC);  

	  /* We've disconnected or reset free the req and buffer */
	  if (dev->reset_fax)
	  {
		if (!error)
		{
		  if (usb_ep_dequeue(dev->out_ep, req))
		  {
			DEBUG(dev, "Failed to dequeue " "in req %p\n", req);
		  }
		}
		SPEAr_Fax_ReqFree(dev->out_ep, req);
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
/* NAME:			config_buf				    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    API to build configuration descriptor for fax        */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :          speed: high or low speed                                  */
/*                type:  descriptor type				    */
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
	function = hs_fax_function;
  }
  else
  {
	function = fs_fax_function;
  }
#else
  function = fs_fax_function;
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
/* NAME:        SPEAr_Fax_SetAltInterface				    */
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
SPEAr_Fax_SetAltInterface(struct fax_dev *dev, unsigned number)
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
  case FAX_INTERFACE:
	result = 0;
	break;
  }

  if (!result)
	INFO(dev, "Using fax alt interface %x\n", number);
  return result;
}

/****************************************************************************/
/* NAME:      fax_setup_complete                                        */
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
fax_setup_complete(struct usb_ep *ep, struct usb_request *req)
{
  if (req->status || req->actual != req->length)
	DEBUG((struct fax_dev *) ep->driver_data, "setup complete --> %d, %d/%d\n", req->status, req->actual, req->length);
}
#endif /*ndef MUX */

/****************************************************************************/
/* NAME:      SPEAr_Fax_Setup                                            */
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
SPEAr_Fax_Setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl, struct usb_request *req)
#else
SPEAr_Fax_Setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
#endif
{
  struct fax_dev *dev = get_gadget_data(gadget);
#ifndef MUX
  struct usb_request *req = dev->req;
#endif
  int             value = -EOPNOTSUPP;
  u16             wIndex = le16_to_cpu(ctrl->wIndex);
  u16             wValue = le16_to_cpu(ctrl->wValue);
  u16             wLength = le16_to_cpu(ctrl->wLength);

#ifndef MUX
  req->complete = fax_setup_complete;
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
	  value = SPEAr_Fax_SetConfig(dev, wValue);
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

	  value = SPEAr_Fax_SetAltInterface(dev, FAX_INTERFACE);
	  break;

	case USB_REQ_GET_INTERFACE:
	  if (ctrl->bRequestType != (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE) || !dev->config)
		break;

	  *(u8 *) req->buf = dev->interface;
	  value = min(wLength, (u16) 1);
	  break;

	default:
	  VDEBUG(dev, "unknown ctrl req%02x.%02x v%04x i%04x l%d\n", ctrl->bRequestType, ctrl->bRequest, wValue, wIndex, wLength);
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
	  fax_setup_complete(gadget->ep0, req);
	}
  }
#endif /* !MUX */

  /* host either stalls (value < 0) or reports success */
  return value;
}

/****************************************************************************/
/* NAME:      SPEAr_Fax_Disconnect                                      */
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
SPEAr_Fax_Disconnect(struct usb_gadget *gadget)
{
  struct fax_dev *dev = get_gadget_data(gadget);
  unsigned long   flags;

  DEBUG(dev, "%s\n", __FUNCTION__);

  spin_lock_irqsave(&dev->lock, flags);
  SPEAr_Fax_DisableInterface(dev);
  spin_unlock_irqrestore(&dev->lock, flags);
}

/****************************************************************************/
/* NAME:         SPEAr_Fax_UnBind                                       */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when fax unregisters from UDC driver            */
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
SPEAr_Fax_UnBind(struct usb_gadget *gadget)
{
  struct fax_dev *dev = get_gadget_data(gadget);
  struct usb_request *req;

  DEBUG(dev, "%s\n", __FUNCTION__);

  /* Remove Character Device */
  cdev_del(&dev->fax_cdev);
  cdev_del(&dev->fax_status_cdev);

  while (likely(!list_empty(&dev->tx_reqs)))
  {
	req = container_of(dev->tx_reqs.next, struct usb_request, list);
	list_del(&req->list);
	SPEAr_Fax_ReqFree(dev->in_ep, req);
  }

#ifdef test_intr
  SPEAr_Fax_ReqFree(dev->intr_ep, dev->intr_req);
#endif


  while (likely(!list_empty(&dev->rx_reqs)))
  {
	req = container_of(dev->rx_reqs.next, struct usb_request, list);
	list_del(&req->list);
	SPEAr_Fax_ReqFree(dev->out_ep, req);
  }

  while (likely(!list_empty(&dev->rx_buffers)))
  {
	req = container_of(dev->rx_buffers.next, struct usb_request, list);
	list_del(&req->list);
	SPEAr_Fax_ReqFree(dev->out_ep, req);
  }

#ifndef MUX
  if (dev->req)
  {
	usb_ep_free_buffer(gadget->ep0, dev->req->buf, dev->req->dma, dev->req->length);
	usb_ep_free_request(gadget->ep0, dev->req);
	dev->req = NULL;
  }
#endif

#ifdef test_intr
  dev->terminated = 1;
  wake_up_interruptible(&wait_intr_fax);
#endif

  set_gadget_data(gadget, NULL);
}

/***************************************************************************/
/* NAME:   SPEAr_Fax_Bind	    		                	   */
/*-------------------------------------------------------------------------*/
/* DESCRIPTION:    Called when fax binds to UDC driver                 */
/*      							           */
/*                                                                         */
/* PARAMETERS:                                                             */
/* IN  :           gadget: usb_gadget structure filled by UDC driver       */
/*                         with ops      				   */
/*                                          				   */
/* OUT :                                                                   */
/*                                                                         */
/* RETURN: 	0 on success						   */
/*		    err otherwise 					   */
/*-------------------------------------------------------------------------*/
/* REENTRANCY: 						                   */
/***************************************************************************/

static int
SPEAr_Fax_Bind(struct usb_gadget *gadget)
{
  struct fax_dev *dev;
  u8              zlp = 1;
  struct usb_ep  *in_ep, *out_ep;
#ifdef test_intr
  struct usb_ep   *intr_ep;
#endif
  int             status = -ENOMEM;
  int             gcnum;
  u32             i;
  struct usb_request *req;

  dev = &usb_fax_gadget;

  /* 
   * Register a character device as an interface to a user mode
   * program that handles the fax specific functionality.
   */

  cdev_init(&dev->fax_cdev, &fax_io_operations);
  dev->fax_cdev.owner = THIS_MODULE;
  status = cdev_add(&dev->fax_cdev, g_fax_devno, 1);
  if (status)
  {
	goto fail;
  }

  cdev_init(&dev->fax_status_cdev, &fax_status_operations);
  dev->fax_status_cdev.owner = THIS_MODULE;
  status = cdev_add(&dev->fax_status_cdev, g_fax_status_devno, 1);
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
  device_desc.idVendor = __constant_cpu_to_le16(FAX_VENDOR_NUM);
  device_desc.idProduct = __constant_cpu_to_le16(FAX_PRODUCT_NUM);

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

#ifdef test_intr
  intr_ep = usb_ep_autoconfig(gadget, &fs_ep_intr_desc);

  if (!intr_ep)
	goto autoconf_fail;
  intr_ep->driver_data = intr_ep;	/* claim */
#endif

#ifdef	CONFIG_USB_GADGET_DUALSPEED
  /* assumes ep0 uses the same value for both speeds ... */
#ifndef MUX
  dev_qualifier.bMaxPacketSize0 = device_desc.bMaxPacketSize0;
#endif

  /* and that all endpoints are dual-speed */
  hs_ep_in_desc.bEndpointAddress = fs_ep_in_desc.bEndpointAddress;
  hs_ep_out_desc.bEndpointAddress = fs_ep_out_desc.bEndpointAddress;
  hs_ep_intr_desc.bEndpointAddress = fs_ep_intr_desc.bEndpointAddress;
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
  spin_lock_init(&dev->lock_fax_io);

  INIT_LIST_HEAD(&dev->tx_reqs);
  INIT_LIST_HEAD(&dev->tx_reqs_active);
  INIT_LIST_HEAD(&dev->rx_reqs);
  INIT_LIST_HEAD(&dev->rx_reqs_active);
  INIT_LIST_HEAD(&dev->rx_buffers);

  /* Initialize Default values */

  dev->config = 0;
  dev->interface = -1;
  dev->fax_cdev_open = 0;
  dev->zlp = zlp;
  dev->fax_status_cdev_open = 0;
  dev->current_rx_req = NULL;
  dev->current_rx_bytes = 0;
  dev->current_rx_buf = NULL;

  dev->in_ep = in_ep;
  dev->out_ep = out_ep;
#ifdef test_intr
  dev->intr_ep = intr_ep;
  dev->fax_status = '0';
#endif

  dev->tx_success = 0;
  dev->tx_status_busy = 0;
  dev->terminated = 0;

#ifndef MUX
  /* preallocate control message data and buffer */
  dev->req = SPEAr_Fax_ReqAlloc(gadget->ep0, USB_DESC_BUFSIZE, GFP_KERNEL);

  if (!dev->req)
  {
	status = -ENOMEM;
	goto fail;
  }
#endif

  for (i = 0; i < QLEN; i++)
  {
	req = SPEAr_Fax_ReqAlloc(dev->in_ep, USB_IN_BUFSIZE, GFP_KERNEL);
	if (!req)
	{
	  while (likely(!list_empty(&dev->tx_reqs)))
	  {
		req = container_of(dev->tx_reqs.next, struct usb_request, list);
		list_del(&dev->tx_reqs);
		SPEAr_Fax_ReqFree(dev->in_ep, req);
	  }
	  return -ENOMEM;
	}
	list_add(&req->list, &dev->tx_reqs);
  }

#ifdef test_intr
  req = SPEAr_Fax_ReqAlloc(dev->intr_ep, USB_INTR_BUFSIZE, GFP_KERNEL);
  if (!req)
  {
	return -ENOMEM;
  }
  dev->intr_req = req;

#endif

  for (i = 0; i < QLEN; i++)
  {
	req = SPEAr_Fax_ReqAlloc(dev->out_ep, USB_OUT_BUFSIZE, GFP_KERNEL);
	if (!req)
	{
	  while (likely(!list_empty(&dev->rx_reqs)))
	  {
		req = container_of(dev->rx_reqs.next, struct usb_request, list);
		list_del(&dev->rx_reqs);
		SPEAr_Fax_ReqFree(dev->out_ep, req);
	  }
	  return -ENOMEM;
	}
	list_add(&req->list, &dev->rx_reqs);
  }

#ifndef MUX
  dev->req->complete = fax_setup_complete;
#endif

  /* finish hookup to lower layer ... */
  dev->gadget = gadget;
  set_gadget_data(gadget, dev);
#ifndef MUX
  gadget->ep0->driver_data = dev;
#endif

#ifdef test_intr
  /* thread */
  if (kernel_thread(fax_main_thread, dev, CLONE_KERNEL) < 0)
	goto fail;

  /* thread over */
#endif

  INFO(dev, "%s, version: " DRIVER_VERSION "\n", driver_desc);
#ifdef test_intr
  INFO(dev, "using %s, OUT %s IN %s INTR %s\n", gadget->name, out_ep->name, in_ep->name, intr_ep->name);
#else
  INFO(dev, "using %s, OUT %s IN %s \n", gadget->name, out_ep->name, in_ep->name);
#endif

  return 0;

fail:
  SPEAr_Fax_UnBind(gadget);
  return status;
}

/****************************************************************************/
/* NAME:   SPEAr_Fax_Suspend					    */
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
SPEAr_Fax_Suspend(struct usb_gadget *gadget)
{
  struct fax_dev *dev = get_gadget_data(gadget);

  DEBUG(dev, "suspend\n");
  dev->suspended = 1;
}

/****************************************************************************/
/* NAME:      SPEAr_Fax_Resume					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Called from UDC driver when  fax is resumed after    */
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
SPEAr_Fax_Resume(struct usb_gadget *gadget)
{
  struct fax_dev *dev = get_gadget_data(gadget);

  DEBUG(dev, "resume\n");
  dev->suspended = 0;
}

/*-------------------------------------------------------------------------*/

#ifndef MUX
static struct usb_gadget_driver fax_driver = {
  .speed = DEVSPEED,

  .function = (char *) driver_desc,
  .bind = SPEAr_Fax_Bind,
  .unbind = SPEAr_Fax_UnBind,

  .setup = SPEAr_Fax_Setup,
  .disconnect = SPEAr_Fax_Disconnect,

  .suspend = SPEAr_Fax_Suspend,
  .resume = SPEAr_Fax_Resume,

  .driver = {
			 .name = (char *) shortname,
			 .owner = THIS_MODULE,
			 },
};
#else
static struct tMUX_CompositeGadgetDriver fax_driver = {

  .function = (char *) driver_desc,
  .interface_no = 0,
  .bind = SPEAr_Fax_Bind,
  .unbind = SPEAr_Fax_UnBind,

  .setup = SPEAr_Fax_Setup,
  .disconnect = SPEAr_Fax_Disconnect,

  .suspend = SPEAr_Fax_Suspend,
  .resume = SPEAr_Fax_Resume,

  .set_config = SPEAr_Fax_SetConfig,
  .reset_config = SPEAr_Fax_DisableInterface,

  .hs_descriptor = hs_fax_function,
  .fs_descriptor = fs_fax_function,

};

#endif

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Rajeev/CPG");
MODULE_LICENSE("GPL");

/****************************************************************************/
/* NAME:      _init init						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Called when fax module is loaded                   */
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

static int      __init
init(void)
{
  int             status;

#ifdef test_intr
  init_completion(&usb_fax_gadget.thread_notifier);
#endif

  status = register_chrdev_region(g_fax_devno, 2, "USB fax (gadget)");
  if (status)
  {
	ERROR(dev, "register_chrdev_region %x\n", status);
	return status;
  }

#ifdef MUX
  status = SPEAr_MUX_UsbGadgetRegisterDriver(&fax_driver);
#else
  status = usb_gadget_register_driver(&fax_driver);
#endif
  if (status)
  {
	unregister_chrdev_region(g_fax_devno, 2);
	DEBUG(dev, "usb gadget register driver %x\n", status);
	return status;
  }

#ifdef test_intr
  /* thread */
  complete(&usb_fax_gadget.thread_notifier);
  /* thread over */
#endif

#ifdef MUX
  printk("FAX gadget driver with MUX support\n");
#else
  printk("FAX gadget driver without MUX support\n");
#endif /*ndef MUX */

  return status;
}

module_init(init);

/****************************************************************************/
/* NAME:      __exit cleanup (void)					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when fax module is unloaded                     */
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

static void     __exit
cleanup(void)
{
  int             status;

  spin_lock(&usb_fax_gadget.lock_fax_io);
  unregister_chrdev_region(g_fax_devno, 2);

#ifdef MUX
  status = SPEAr_MUX_UsbGadgetUnregisterDriver(&fax_driver);
#else
  status = usb_gadget_unregister_driver(&fax_driver);
#endif
  if (status)
  {
	ERROR(dev, "usb  unregister gadget driver %x\n", status);
  }

  spin_unlock(&usb_fax_gadget.lock_fax_io);

#ifdef test_intr
  wait_for_completion(&usb_fax_gadget.thread_notifier);
#endif
}
module_exit(cleanup);
