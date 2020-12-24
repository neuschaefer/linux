/*
 * Composite Gadget Driver for use with composite devices.
 * 
 * This driver was influenced by David Brownell's ideas from
 * http://www.mail-archive.com/linux-usb-devel@lists.sourceforge.net/msg50046.html
 */

/*
*
* 
* Copyright (C) 2006,2010 Avocent Corporation
*
* This file is subject to the terms and conditions of the GNU 
* General Public License Version 2. This program is distributed in the hope 
* that it will be useful, but WITHOUT ANY WARRANTY; without even 
* the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
* PURPOSE. See the GNU General Public License Version 2 for more details.
*
*/

#ifndef LINUX_VERSION_CODE
#  include <linux/version.h>
#endif

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
#include <linux/ctype.h>
#include <linux/dma-mapping.h>

#include <asm/byteorder.h>
#include <asm/system.h>
#include <asm/unaligned.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
   #include <linux/usb_ch9.h>
#else
   #include <linux/usb/ch9.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
   #include <linux/usb_gadget.h>
#else
   #include <linux/usb/gadget.h>
#endif
#include "gadget_chips.h"
#include "composite.h"

#undef DEBUG

#define DRIVER_DESC     "USB Composite Device"
#define DRIVER_NAME     "g_composite"
#define DRIVER_VERSION  "20120430"

static const char longname[]  = DRIVER_DESC;
static const char shortname[] = DRIVER_NAME;

#define DRIVER_VENDOR_ID        0x0624	// Avocent
#define DRIVER_PRODUCT_ID       0x0248

struct usb_composite_devices	the_cdevs;

/*-------------------------------------------------------------------------*/

static unsigned short	functions[MAX_NUM_CONTROLLERS] = {1, 1};
static unsigned short	products[MAX_NUM_CONTROLLERS] = {DRIVER_PRODUCT_ID,
							DRIVER_PRODUCT_ID + 1};

static struct {
	unsigned short	vendor;
	int             num_products;
	unsigned short	release;
	char            *description;
	char            *manufacturer;
	int	        num_functions;
	unsigned short	num_controllers;
} mod_data = {					// Default values
	.vendor			= DRIVER_VENDOR_ID,
	.num_products		= 1,
	.release		= __constant_cpu_to_le16(0x0000),
	.description            = DRIVER_DESC,
	.manufacturer           = "Avocent",
	.num_functions		= 1,
	.num_controllers	= 1,
	};


module_param_named(vendor, mod_data.vendor, ushort, S_IRUGO);
MODULE_PARM_DESC(vendor, "USB Vendor ID");

module_param_array(products, ushort, &mod_data.num_products, S_IRUGO);
MODULE_PARM_DESC(products, "USB Product ID's");

module_param_named(release, mod_data.release, ushort, S_IRUGO);
MODULE_PARM_DESC(release, "USB release number");

module_param_named(description, mod_data.description, charp, S_IRUGO);
MODULE_PARM_DESC(description, "USB Composite Description");

module_param_named(manufacturer, mod_data.manufacturer, charp, S_IRUGO);
MODULE_PARM_DESC(manufacturer, "USB Manufacturer");
	
module_param_named(controllers, mod_data.num_controllers, ushort, S_IRUGO);
MODULE_PARM_DESC(controllers, "Number of Controller Drivers allowed");

module_param_array(functions, ushort, &mod_data.num_functions, S_IRUGO);
MODULE_PARM_DESC(functions, "Number of Function Drivers allowed");

/*-------------------------------------------------------------------------*/		
	
/* big enough to hold our biggest descriptor */
#define USB_BUFSIZ     512


static inline int is_dualspeed(struct usb_gadget *g)
{
#ifdef CONFIG_USB_GADGET_DUALSPEED
       return g->is_dualspeed;
#else
       return 0;
#endif
}

static inline int is_otg(struct usb_gadget *g)
{
#ifdef CONFIG_USB_OTG
       return g->is_otg;
#else
       return 0;
#endif
}

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
static struct _usb_composite_dev* get_cdev(struct _usb_function *f)
{
	struct _usb_composite_dev *cdev;
	int i;
	
	for (i = 0; i < mod_data.num_controllers; i++) {
		cdev = &the_cdevs.cdevs[i];
		if (cdev && cdev->gadget) {
			if (strcmp(cdev->gadget->name, f->controller_name) == 0)
				return cdev;
		}
	}
	
	return NULL;
}
#endif

/*-------------------------------------------------------------------------*/

/*
 * DESCRIPTORS ... most are static, but strings and (full) configuration
 * descriptors are built on demand.  Also the (static) config and interface
 * descriptors are adjusted during fsg_bind().
 */
#define STRING_MANUFACTURER			1
#define STRING_PRODUCT				2
#define STRING_SERIAL				3
#define MAX_STRING                              3

/* There is only one configuration. */
#define	CONFIG_VALUE		1

static struct usb_device_descriptor device_desc[MAX_NUM_CONTROLLERS] = {
{
	.bLength                = USB_DT_DEVICE_SIZE,
	.bDescriptorType        = USB_DT_DEVICE,
	.bcdUSB                 = __constant_cpu_to_le16(0x0110),
#if 1
	.bDeviceClass           = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass        = 0,
	.bDeviceProtocol        = 0,
#else
	// For use with Interface Association Class
	.bDeviceClass           = 0xEF,
	.bDeviceSubClass        = 0x02,
	.bDeviceProtocol        = 0x01,	
#endif
	/* The next three values can be overridden by module parameters */
	.idVendor               = __constant_cpu_to_le16(DRIVER_VENDOR_ID),
	.idProduct              = __constant_cpu_to_le16(DRIVER_PRODUCT_ID),
	.bcdDevice              = __constant_cpu_to_le16(0x0000),

	.iManufacturer          = STRING_MANUFACTURER,
	.iProduct               = STRING_PRODUCT,
	.iSerialNumber          = STRING_SERIAL,
	.bNumConfigurations     = 1,
},

{
	.bLength                = USB_DT_DEVICE_SIZE,
	.bDescriptorType        = USB_DT_DEVICE,
	.bcdUSB                 = __constant_cpu_to_le16(0x0110),
#if 1
	.bDeviceClass           = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass        = 0,
	.bDeviceProtocol        = 0,
#else
	// For use with Interface Association Class
	.bDeviceClass           = 0xEF,
	.bDeviceSubClass        = 0x02,
	.bDeviceProtocol        = 0x01,	
#endif
	/* The next three values can be overridden by module parameters */
	.idVendor               = __constant_cpu_to_le16(DRIVER_VENDOR_ID),
	.idProduct              = __constant_cpu_to_le16(DRIVER_PRODUCT_ID),
	.bcdDevice              = __constant_cpu_to_le16(0x0000),

	.iManufacturer          = STRING_MANUFACTURER,
	.iProduct               = STRING_PRODUCT,
	.iSerialNumber          = STRING_SERIAL,
	.bNumConfigurations     = 1,
}
};

static struct usb_config_descriptor config_desc[MAX_NUM_CONTROLLERS] = {
{
	.bLength                = sizeof config_desc,
	.bDescriptorType        = USB_DT_CONFIG,

	/* wTotalLength computed by usb_gadget_config_buf() */
	.bNumInterfaces         = 0,
	.bConfigurationValue    = CONFIG_VALUE,
	.iConfiguration         = 0,
	.bmAttributes           = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower              = 1,	// self-powered
},

{
	.bLength                = sizeof config_desc,
	.bDescriptorType        = USB_DT_CONFIG,

	/* wTotalLength computed by usb_gadget_config_buf() */
	.bNumInterfaces         = 0,
	.bConfigurationValue    = CONFIG_VALUE,
	.iConfiguration         = 0,
	.bmAttributes           = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower              = 1,	// self-powered
}
};

struct usb_descriptor_strings { 
	char                 manufacturer[64+1];
	char                 description[64+2+1];
	char                 serial[13+2+1];
};

static struct usb_descriptor_strings desc_strings[MAX_NUM_CONTROLLERS];

/* Static strings, in UTF-8 (for simplicity we use only ASCII characters) */
static struct usb_string                strings[] = {
	{STRING_MANUFACTURER,           desc_strings[0].manufacturer},
	{STRING_PRODUCT,                desc_strings[0].description},
	{STRING_SERIAL,                 desc_strings[0].serial},
	{}
};

static struct usb_gadget_strings        stringtab = {
	.language = 0x0409,		// en-us
	.strings  = strings,
};

static const struct usb_gadget_strings *stringTable[] = {
	(struct usb_gadget_strings *)&stringtab,
	NULL,
};

/* Static strings, in UTF-8 (for simplicity we use only ASCII characters) */
static struct usb_string                strings_1[] = {
	{STRING_MANUFACTURER,           desc_strings[1].manufacturer},
	{STRING_PRODUCT,                desc_strings[1].description},
	{STRING_SERIAL,                 desc_strings[1].serial},
	{}
};

static struct usb_gadget_strings        stringtab_1 = {
	.language = 0x0409,		// en-us
	.strings  = strings_1,
};

static const struct usb_gadget_strings *stringTable_1[] = {
	(struct usb_gadget_strings *)&stringtab_1,
	NULL,
};

/*-------------------------------------------------------------------------*/

/* To simplify, we expect to have only ONE real configuration, working the
 * same no matter what speed it connects with.  A given function may expose
 * multiple interfaces.  Each interface includes its descriptor, plus optional
 * class and endpoint descriptors (as usual).
 */

static int config_buf(struct _usb_composite_dev *cdev, void *buf, u8 type)
{
	struct usb_config_descriptor    *c = buf;
	void                            *next = buf + USB_DT_CONFIG_SIZE;
	int                             len = USB_BUFSIZ - USB_DT_CONFIG_SIZE;
	int                             hs;
	int				i;
	int                     	status;
	struct _usb_function             *f;
	const struct usb_descriptor_header	**functionTabPtr;

	if (is_dualspeed(cdev->gadget)) {
		hs = (cdev->gadget->speed == USB_SPEED_HIGH);
		if (type == USB_DT_OTHER_SPEED_CONFIG)
			hs = !hs;
	}
	else
		hs = 0;

	/* write a config descriptor */
	*c = *cdev->config;
	c->bLength = USB_DT_CONFIG_SIZE;
	c->bDescriptorType = type;
	c->bConfigurationValue = CONFIG_VALUE;

	/* REVISIT some configurations might need other descriptors,
	* independent of the interfaces they implement ... notably
	* OTG descriptors.
	*/
	
	/* add each function's descriptors */
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];		
		
		if (hs)
			functionTabPtr = f->hs_descriptors;
		else
			functionTabPtr = f->descriptors;
		
		if (!cdev->gadget->is_otg)
			functionTabPtr++;
		
		status = usb_descriptor_fillbuf(next, len, functionTabPtr);
		
		if (status < 0)
			return status;
		
		len -= status;
		next += status;
	}

	len = next - buf;
	c->wTotalLength = cpu_to_le16(len);
	

	
	return len;
}

/*-------------------------------------------------------------------------*/
static int composite_set_config(struct _usb_composite_dev *cdev, unsigned number)
{
	struct _usb_function     *f;
	int                     result = 0;
	int			i;
	struct usb_gadget       *gadget = cdev->gadget;
	struct usb_ctrlrequest  req;		
	char			*speed;

	memset(&req, 0, sizeof req);

	//composite_reset_config(cdev, &req);

	/*
	 * Some BIOSes seem to specify SetConfiguration with the wrong configuration
	 * value.  Allow for this.
	 */
        if ((number == 1) && (CONFIG_VALUE != 1)) {
                printk(KERN_ALERT "%s() (%s): force configuration number"
                			" from %d to %d\n",
                        		__FUNCTION__, gadget->name,
                        		number, CONFIG_VALUE);
                number = CONFIG_VALUE;
        }

	switch (number) {
	default:
		result = -EINVAL;
		usb_gadget_vbus_draw(gadget, is_otg(gadget) ? 8 : 100);
		break;
	case 0:
	case CONFIG_VALUE:
		req.bRequestType = USB_DIR_OUT | USB_TYPE_STANDARD
						| USB_RECIP_DEVICE;
		req.bRequest     = USB_REQ_SET_CONFIGURATION;		
		req.wValue       = le16_to_cpu(number);

		for (i = 0; i < cdev->next_function_index; i++) {
			f = cdev->function[i];
			
			if (f && f->setup) {
			
				req.wIndex = cpu_to_le16(i);
				result = f->setup(cdev, &req);
#if 0
				if (result < 0) {
					(void) composite_set_config(cdev, 0);
					return result;
				}
#endif					
			}
		}

		cdev->config->bConfigurationValue = number;
		usb_gadget_vbus_draw(gadget, 2 * cdev->config->bMaxPower);
		
		break;
	}
	
	switch (gadget->speed) {
	case USB_SPEED_LOW:     speed = "low"; break;
	case USB_SPEED_FULL:    speed = "full"; break;
	case USB_SPEED_HIGH:    speed = "high"; break;
	default:                speed = "?"; break;
	}

	printk(KERN_ALERT DRIVER_NAME " %s: %s speed config #%d \n",
						gadget->name, speed, number);

	return result;
}

/*-------------------------------------------------------------------------*/
static void composite_collect_langs(const struct usb_gadget_strings **sp,
					__le16 *buf)
{
	const struct usb_gadget_strings *s;
	u16                             language;
	__le16                          *tmp;

	while (*sp) {
		s = *sp;
		language = cpu_to_le16(s->language);
		
		for (tmp = buf; *tmp && tmp < &buf[126]; tmp++) {
			if (*tmp == language)
				goto repeat;
		}
		
		*tmp++ = language;
repeat:
		sp++;
	}
}

static int composite_check_string(const struct usb_gadget_strings **sp,
					void *buf, u16 language, int id)
{
	const struct usb_gadget_strings *s;
	int                             value;

	while (*sp) {
		s = *sp++;
		if (s->language != language)
			continue;
		value = usb_gadget_get_string((struct usb_gadget_strings*)s,
							id, buf);
		if (value > 0)
			return value;
	}
	return -EINVAL;
}


static int composite_lookup_string(struct _usb_composite_dev *cdev, 
					void *buf, u16 language, int id)
{
	struct _usb_function             *f;
	int                             len;
	int                             i;

	/* 0 == report all available language codes */
	if (id == 0) {
		struct usb_string_descriptor    *s = buf;
		const struct usb_gadget_strings **sp;

		memset(s, 0, 256);
		s->bDescriptorType = USB_DT_STRING;

		sp = cdev->strings;
		if (sp)
			composite_collect_langs(sp, s->wData);

		for (i = 0; i < cdev->next_function_index; i++) {
			f = cdev->function[i];
			sp = f->strings;
			if (sp)
				composite_collect_langs(sp, s->wData);
		}

		for (len = 0; s->wData[len] && len <= 126; len++)
			continue;
		if (!len)
			return -EINVAL;

		s->bLength = 2 * (len + 1);
		return s->bLength;
	}

	/* otherwise, look up and return a specified string */
	if (cdev->strings) {
		len = composite_check_string(cdev->strings, buf, language, id);
		if (len > 0)
			return len;
	}
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		if (!f->strings)
			continue;
		len = composite_check_string(f->strings, buf, language, id);
			if (len > 0)
		return len;
	}
	return -EINVAL;
}

/*-------------------------------------------------------------------------*/

static void composite_setup_complete(struct usb_ep *ep, struct usb_request *req)
{	
	if (req->status || req->actual != req->length)
		printk(KERN_ALERT "%s(): %d, %d/%d\n", __FUNCTION__, 
			req->status, req->actual, req->length);
}


static struct _usb_function* get_function_by_interface(
					struct _usb_composite_dev *cdev,
					u16	w_index)
{
	struct _usb_function		*f;
	struct usb_interface_info	**intf_info_table;
	struct usb_interface_info	*intf_info;
	struct usb_interface_descriptor *intf_desc;
	int				i;
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		intf_info_table = f->intf_info_table; 
		while (*intf_info_table) {
			intf_info = *intf_info_table;
			intf_desc = intf_info->intf_desc;
			if ((intf_info->enabled) &&
				(intf_desc->bInterfaceNumber == w_index))
				return f;
			intf_info_table++;
		}
	}
	
	printk(KERN_ALERT "%s(): could not find function for interface %d\n",
				__FUNCTION__, w_index);
	return NULL;
}
/*
 * The setup() callback implements all the ep0 functionality that's
 * not handled lower down, in hardware or the hardware driver(like
 * device and endpoint feature flags, and their status).  It's all
 * housekeeping for the gadget function we're implementing.  Most of
 * the work is in config-specific setup.
 */
static int composite_setup(struct usb_gadget *gadget,
			const struct usb_ctrlrequest *ctrl)
{
	struct _usb_composite_dev        *cdev = get_gadget_data(gadget);
	struct usb_request              *req = cdev->req;
	struct _usb_function		*f;
	int				result = 0;
	int                             value = -EOPNOTSUPP;
	u8				respond = 0;
	u16                             w_index = le16_to_cpu(ctrl->wIndex);
	u16                             w_value = le16_to_cpu(ctrl->wValue);
	u16                             w_length = le16_to_cpu(ctrl->wLength);
	
	req->zero = 0;
	
	if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS) {
		// forward to the function driver
		//f = cdev->interface[w_index];
		f = get_function_by_interface(cdev, w_index);
	
		result = -EOPNOTSUPP;	
		if (f && f->setup)
		{
			spin_lock(&cdev->lock);
			result = f->setup(cdev, ctrl);
			spin_unlock(&cdev->lock);
			if (result < 0)
			{
				printk(KERN_ERR "%s() (%s):"
					" failed forwarding"
					" class request: t = 0x%02X,"
					" r = 0x%02X, v = 0x%04X,"
					" i = 0x%04X, l = 0x%04X\n",
					__FUNCTION__, gadget->name,
					ctrl->bRequestType, ctrl->bRequest, 
					w_value, w_index, w_length);
			}
		}
		return result;
	}
	
	switch (ctrl->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		if (!(ctrl->bRequestType & USB_DIR_IN))
			goto unknown;
		switch (w_value >> 8) {
		case USB_DT_DEVICE:
			value = min(w_length, (u16) sizeof *cdev->dev_desc);
			memcpy(req->buf, cdev->dev_desc, value);
			break;
		
		case USB_DT_DEVICE_QUALIFIER:
			if (!is_dualspeed(gadget))
				break;
			value = min(w_length, (u16) sizeof cdev->qual);
			memcpy(req->buf, &cdev->qual, value);
			break;

		case USB_DT_OTHER_SPEED_CONFIG:
			if (!is_dualspeed(gadget))
				break;
				// FALLTHROUGH

		case USB_DT_CONFIG:
			/* one config ... so it must always be index 0 */
			if (w_value & 0xff)
				break;

			value = config_buf(cdev, req->buf, w_value >> 8);
			if (value >= 0)
				value = min(w_length, (u16) value);
			break;
		case USB_DT_STRING:
			value = composite_lookup_string(cdev, req->buf, w_index,
								w_value & 0xff);
			if (value >= 0)
				value = min(w_length, (u16) value);
			break;
		default:
			// forward to the function driver
			f = get_function_by_interface(cdev, w_index);
			result = -EOPNOTSUPP;	
			if (f && f->setup)
			{
				spin_lock(&cdev->lock);
				result = f->setup(cdev, ctrl);
				spin_unlock(&cdev->lock);
				if (result < 0)
				{
					printk(KERN_ALERT "%s() (%s): failed"
						" forwarding desc request for"
						" interface 0x%04X,\n",
						__FUNCTION__, gadget->name,
						w_index);
				}
			}
			return result;
		}
		
		respond = 1;
		break;

	/* currently one config, two speeds */
	case USB_REQ_SET_CONFIGURATION:
		if (ctrl->bRequestType != 0)
			goto unknown;
		spin_lock(&cdev->lock);
		value = composite_set_config(cdev, w_value);
		spin_unlock(&cdev->lock);
		break;
		
	case USB_REQ_GET_CONFIGURATION:
		if (ctrl->bRequestType != USB_DIR_IN)
			goto unknown;
		*(u8 *)req->buf = cdev->config->bConfigurationValue;
		value = min(w_length, (u16) 1);
		respond = 1;
		break;

	/* function drivers must handle get/set altsetting */
	case USB_REQ_SET_INTERFACE:
		if (ctrl->bRequestType != USB_RECIP_INTERFACE)
			goto unknown;
		f = get_function_by_interface(cdev, w_index);
		if (!cdev->config->bConfigurationValue
				|| w_index >= MAX_COMPOSITE_INTERFACES
				//|| !cdev->interface[w_index])
				|| f == NULL)
			break;
		spin_lock(&cdev->lock);
		value = f->setup(cdev, ctrl);
		spin_unlock(&cdev->lock);
		break;
		
	case USB_REQ_GET_INTERFACE:
		if (ctrl->bRequestType !=(USB_DIR_IN|USB_RECIP_INTERFACE))
			goto unknown;
		f = get_function_by_interface(cdev, w_index);
		if (!cdev->config->bConfigurationValue
				|| w_index >= MAX_COMPOSITE_INTERFACES
				//|| !cdev->interface[w_index])
				|| f == NULL)
			break;
		spin_lock(&cdev->lock);
		/* function must set cdev->req->buf[0] */
		value = f->setup(cdev, ctrl);
		spin_unlock(&cdev->lock);
		value = min(w_length, (u16) 1);
		break;
	default:
unknown:
		printk(KERN_DEBUG "%s() (%s):"
			" unknown control req%02x.%02x v%04x i%04x l%d\n",
			__FUNCTION__, gadget->name,
			ctrl->bRequestType, ctrl->bRequest, w_value,
			w_index, w_length);
	}

	/* respond with data transfer before status phase? */
	if (respond && value >= 0) {
		req->length = value;
		req->zero = value < w_length;
		value = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);
		
		if (value < 0) {
			printk(KERN_DEBUG "%s() (%s): ep_queue --> %d\n", 
				__FUNCTION__, gadget->name, value);
			req->status = 0;
			composite_setup_complete(gadget->ep0, req);
		}
	}

	/* device either stalls (value < 0) or reports success */
	return value;
}

/*-------------------------------------------------------------------------*/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
void * _usb_ep_alloc_buffer(struct usb_gadget *gadget,
				   struct usb_ep *_ep,
                                   unsigned bytes,
                                   dma_addr_t *dma,
                                   gfp_t gfp_flags)
{
	void *buf;
	
	*dma = DMA_ADDR_INVALID;	
	buf = dma_alloc_coherent(gadget->dev.parent,
				bytes, dma, gfp_flags);

	return buf;	
}
EXPORT_SYMBOL(_usb_ep_alloc_buffer);

void _usb_ep_free_buffer(struct usb_gadget *gadget,
				struct usb_ep *_ep,
                                void *buf,
                                dma_addr_t dma,
                                unsigned bytes)
{	
	if (dma != DMA_ADDR_INVALID) {
		dma_free_coherent(gadget->dev.parent,
					bytes, buf, dma);
	}	
}
EXPORT_SYMBOL(_usb_ep_free_buffer);
#endif

static void /* __init_or_exit */
composite_unbind(struct usb_gadget *gadget)
{
	struct _usb_composite_dev        *cdev = get_gadget_data(gadget);
	struct _usb_function             *f;
	int                             i;
	
	if (cdev) {
		// unbind all function drivers
		for (i = 0; i < cdev->next_function_index; i++) {
			f = cdev->function[i];
			
			if (f && f->unbind)
				f->unbind(cdev);
			
			f = NULL;
		}

		the_cdevs.next_cdev_index = cdev->cdev_index; 
		
		if (cdev->req) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
			usb_ep_free_buffer(gadget->ep0, cdev->req->buf,
						cdev->req->dma, USB_BUFSIZ);
#else
			_usb_ep_free_buffer(gadget, gadget->ep0, cdev->req->buf,
						cdev->req->dma, USB_BUFSIZ);
#endif
			usb_ep_free_request(gadget->ep0, cdev->req);
		}
		
		the_cdevs.num_cdevs--;
	}
	
	set_gadget_data(gadget, NULL);
}

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
static int composite_bind(struct usb_gadget *gadget)
#else
static int __init composite_bind(struct usb_gadget *gadget)
#endif
{
	struct _usb_composite_dev *cdev =
				&the_cdevs.cdevs[the_cdevs.next_cdev_index];	
	int                      status = -ENOMEM;
	int                      i;

	// Initialize
	cdev->cdev_index          = the_cdevs.next_cdev_index;	
	cdev->next_string_id      = MAX_STRING + 1;
	cdev->next_interface_id   = 0;
	cdev->next_function_index = 0;
	cdev->num_functions       = functions[cdev->cdev_index];
	cdev->set_config_count    = 0;
	
	for (i = 0; i < MAX_NUM_FUNCTIONS; i++)
		cdev->function[i] = NULL;
	
	//for (i = 0; i < MAX_COMPOSITE_INTERFACES; i++)
	//	cdev->interface[i] = NULL;
		
	the_cdevs.num_cdevs++;
		
	spin_lock_init(&cdev->lock);
	cdev->gadget = gadget;
	set_gadget_data(gadget, cdev);

	/* preallocate control response and buffer */
	cdev->req = usb_ep_alloc_request(gadget->ep0, GFP_KERNEL);
	if (!cdev->req)
		goto fail;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	cdev->req->buf = usb_ep_alloc_buffer(gadget->ep0, USB_BUFSIZ,
						&cdev->req->dma, GFP_KERNEL);
#else
	cdev->req->buf = _usb_ep_alloc_buffer(gadget, gadget->ep0, USB_BUFSIZ,
						&cdev->req->dma, GFP_KERNEL);
#endif
	if (!cdev->req->buf)
		goto fail;

	cdev->req->complete		= composite_setup_complete;
	gadget->ep0->driver_data	= cdev;
	
	cdev->dev_desc	= &device_desc[cdev->cdev_index];
	cdev->config	= &config_desc[cdev->cdev_index];
	if (cdev->cdev_index == 0) {

		cdev->strings	= stringTable;
	} else {
		cdev->strings	= stringTable_1;		
	}
	
	cdev->dev_desc->bMaxPacketSize0 = gadget->ep0->maxpacket;

//FIX ME:: usb_ep_autoconfig_reset() with CONFIG_USB_GADGET_DUAL_CONTROLLERS
// for some reason	
	// make sure no endpoints are claimed
#if !defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
	usb_ep_autoconfig_reset(gadget);
#endif

	usb_gadget_set_selfpowered(gadget);

	// Set the manufacturer string
	strlcpy(desc_strings[cdev->cdev_index].manufacturer,
			mod_data.manufacturer,
			min(sizeof(desc_strings[cdev->cdev_index].manufacturer),
			strlen(mod_data.manufacturer)+1));
			
	// Set the product string	
	strlcpy(desc_strings[cdev->cdev_index].description,
			mod_data.description,
			min(sizeof(desc_strings[cdev->cdev_index].description)-2,
			strlen(mod_data.description)+1));

	sprintf(desc_strings[cdev->cdev_index].description,
			"%s-%d", desc_strings[cdev->cdev_index].description,
			cdev->cdev_index);
			
	// fix up the descriptors
	cdev->dev_desc->bNumConfigurations	= 1;
	cdev->dev_desc->idVendor  = cpu_to_le16(mod_data.vendor);
	cdev->dev_desc->idProduct = cpu_to_le16(products[cdev->cdev_index]);
	cdev->dev_desc->bcdDevice = cpu_to_le16(mod_data.release);

	if (is_dualspeed(gadget)) {
		cdev->dev_desc->bcdUSB          = __constant_cpu_to_le16(0x0200);
		
		cdev->qual.bLength              = sizeof cdev->qual;
		cdev->qual.bDescriptorType      = USB_DT_DEVICE_QUALIFIER;
	
		cdev->qual.bcdUSB               = cdev->dev_desc->bcdUSB;
		cdev->qual.bDeviceClass         = cdev->dev_desc->bDeviceClass;
		cdev->qual.bDeviceProtocol = cdev->dev_desc->bDeviceProtocol;
	
		/* assume ep0 uses the same value for both speeds ... */
		cdev->qual.bMaxPacketSize0 = cdev->dev_desc->bMaxPacketSize0;
	
		cdev->qual.bNumConfigurations =
					cdev->dev_desc->bNumConfigurations;
	}
	
	return 0;
	
fail:
	composite_unbind(gadget);
	return status;
}


/*-------------------------------------------------------------------------*/
static void composite_disconnect(struct usb_gadget *gadget)
{
	struct _usb_composite_dev        *cdev = get_gadget_data(gadget);
	unsigned long                   flags;
	struct _usb_function             *f;
	int                             i;		

	//printk(KERN_DEBUG "%s() (%s):\n", __FUNCTION__, gadget->name);

	spin_lock_irqsave(&cdev->lock, flags);
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		if(f && f->disconnect)
			f->disconnect(cdev);
	}
	
	cdev->config->bConfigurationValue = 0;
	
	spin_unlock_irqrestore(&cdev->lock, flags);
}

static void composite_suspend(struct usb_gadget *gadget)
{
	struct _usb_composite_dev        *cdev = get_gadget_data(gadget);
	struct _usb_function             *f;
	int                             i;

	//printk(KERN_DEBUG "%s() (%s):\n", __FUNCTION__, gadget->name);
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		if (f && f->suspend)
			f->suspend(cdev);
	}
}

static void composite_resume(struct usb_gadget *gadget)
{
	struct _usb_composite_dev        *cdev = get_gadget_data(gadget);
	struct _usb_function             *f;
	int                             i;		

	//printk(KERN_DEBUG "%s() (%s):\n", __FUNCTION__, gadget->name);
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		if (f && f->resume)
			f->resume(cdev);
	}
}

int usb_composite_reconfigure_interface_ids(struct _usb_composite_dev *cdev)
{
	struct _usb_function		*f;
	struct usb_interface_info	**intf_info_table;
	struct usb_interface_info	*intf_info;
	struct usb_interface_descriptor *intf_desc;
	int				i;
	
	cdev->next_interface_id = 0;
	
	for (i = 0; i < cdev->next_function_index; i++) {
		f = cdev->function[i];
		intf_info_table = f->intf_info_table;
		
		while (*intf_info_table) {
			intf_info = *intf_info_table;
			intf_desc = intf_info->intf_desc;
			if (intf_info->enabled) {
				intf_desc->bInterfaceNumber = 
							cdev->next_interface_id;
				cdev->next_interface_id++;
			} else {
				intf_desc->bInterfaceNumber =
						MAX_COMPOSITE_INTERFACES;
			}
			intf_info_table++;
		}
	}
	
	cdev->config->bNumInterfaces = cdev->next_interface_id;

#if 1
	// Set the serial number
	strlcpy(desc_strings[cdev->cdev_index].serial, DRIVER_VERSION,
		min(sizeof(desc_strings[cdev->cdev_index].serial)-2,
		strlen(DRIVER_VERSION)+1));
			
		if (cdev->next_interface_id == 1) {	
			sprintf(desc_strings[cdev->cdev_index].serial, "%s-1",
				desc_strings[cdev->cdev_index].serial);
		} 
#endif
	
	return cdev->next_interface_id;
}
EXPORT_SYMBOL(usb_composite_reconfigure_interface_ids);

int usb_composite_string_id(struct _usb_composite_dev *cdev)
{
	if ((cdev->current_bind) && (cdev->next_string_id < 255))
		return cdev->next_string_id++;
	return -ENODEV;
}
EXPORT_SYMBOL(usb_composite_string_id);

int usb_composite_interface_id(struct _usb_composite_dev *cdev,
				struct usb_interface_info *intf_info)
{
	if ((cdev->next_interface_id < MAX_COMPOSITE_INTERFACES)
		&& (cdev->current_bind)) {
			if (intf_info->enabled) {
				//cdev->interface[cdev->next_interface_id] = cdev->current_bind;
				return cdev->next_interface_id++;
			} else {
				return MAX_COMPOSITE_INTERFACES;
			}
	}

	return -ENODEV;
}
EXPORT_SYMBOL(usb_composite_interface_id);

int usb_composite_function_index(struct _usb_composite_dev *cdev)
{
	if ((cdev->next_function_index < MAX_NUM_FUNCTIONS)
		&& (cdev->current_bind)) {
		return cdev->next_function_index;
	}
	return -ENODEV;
}
EXPORT_SYMBOL(usb_composite_function_index);

void set_composite_data(struct _usb_composite_dev *cdev, u8 index, void *data)
{
	cdev->function_data[index] = data;
}
EXPORT_SYMBOL(set_composite_data);

void *get_composite_data(struct _usb_composite_dev *cdev, u8 index)
{
	return cdev->function_data[index];
}
EXPORT_SYMBOL(get_composite_data);


void usb_composite_set_config_status_stage(struct _usb_composite_dev *cdev)
{
	struct usb_gadget       *gadget = cdev->gadget;
	struct usb_request      *req = cdev->req;
	int                     value = -EOPNOTSUPP;
	
	cdev->set_config_count++;

	if (cdev->set_config_count == cdev->num_functions) {
		req->length = 0;
		req->zero = 0;

		printk(KERN_ALERT "%s() SET_CONFIGURATION sending status stage\n", __FUNCTION__);
		value = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);
		
		if (value < 0) {
			printk(KERN_DEBUG "%s() (%s): ep_queue --> %d\n", 
				__FUNCTION__, gadget->name, value);
			req->status = 0;
			composite_setup_complete(gadget->ep0, req);
		}

		cdev->set_config_count = 0;
	}
}
EXPORT_SYMBOL(usb_composite_set_config_status_stage);


/*-------------------------------------------------------------------------*/

static struct usb_gadget_driver composite_gadget_driver = {
	.speed          = USB_SPEED_HIGH,
	.function	= DRIVER_NAME,
	.bind           = composite_bind,
	.unbind         = __exit_p(composite_unbind),

	.setup          = composite_setup,
	.disconnect     = composite_disconnect,

	.suspend        = composite_suspend,
	.resume         = composite_resume,

	.driver = {
		.name      	= DRIVER_NAME,
		.owner          = THIS_MODULE,
	},
};

int _usb_composite_register(struct _usb_function *f)
{
	struct _usb_composite_dev *cdev;
	int status;
	
	printk(KERN_DEBUG "%s()\n", __FUNCTION__);

	
	if (!f	|| !f->bind
		|| !f->unbind
		|| !f->setup
		|| !f->descriptors
		|| !f->controller_name)
		return -EINVAL;

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)		
	// Find the correct controller driver to use for this function
	cdev = get_cdev(f);
#else
	cdev = &the_cdevs.cdevs[0];
#endif
	
	if (!cdev) {
		printk(KERN_ERR "%s(): Unable to find %s"
			" controller for %s\n", __FUNCTION__, 
					f->controller_name, f->name);
		return -EINVAL;
	}
	
	if ((is_dualspeed(cdev->gadget) && !f->hs_descriptors)
		|| cdev->next_function_index == cdev->num_functions)
		return -EINVAL;
	
	cdev->current_bind = f;
	
	// bind function
	status = f->bind(cdev);
	if (status < 0) {
		printk(KERN_ERR "%s(): bind to driver"
				" '%s' ERROR\n", __FUNCTION__, f->name);
		return status;
	}
	
	cdev->current_bind = NULL;

	// Add function to the list
	cdev->function[cdev->next_function_index] = f;
	cdev->next_function_index++;
	cdev->config->bNumInterfaces = cdev->next_interface_id;
	
	printk(KERN_ALERT "%s(): bind to driver '%s' SUCCESSFUL\n",
						__FUNCTION__, f->name);
	
	if ((cdev->next_function_index == cdev->num_functions) &&
						(cdev->next_interface_id > 0)) {
#if 1
	// Set the serial number
	strlcpy(desc_strings[cdev->cdev_index].serial, DRIVER_VERSION,
		min(sizeof(desc_strings[cdev->cdev_index].serial)-2,
		strlen(DRIVER_VERSION)+1));
			
		if (cdev->next_interface_id == 1) {	
			sprintf(desc_strings[cdev->cdev_index].serial, "%s-1",
				desc_strings[cdev->cdev_index].serial);
		} 
#endif
				
		return usb_gadget_connect(cdev->gadget);
	} else
		return 0;
 
}
EXPORT_SYMBOL(_usb_composite_register);

int _usb_composite_unregister(struct _usb_function *f)
{	
	struct _usb_composite_dev *cdev;
	
#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)		
	// Find the correct controller driver to use for this function
	cdev = get_cdev(f);
#else
	cdev = &the_cdevs.cdevs[0];
#endif
	
	if (!cdev) {
		printk(KERN_ERR "%s(): Unable to find %s"
			" controller for %s\n", __FUNCTION__,
						f->controller_name, f->name);
		return -EINVAL;
	}	
	
	cdev->gadget_unregister(&composite_gadget_driver);

	return 0;

}
EXPORT_SYMBOL(_usb_composite_unregister);

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
int usb_gadget_controller_register(
		int (*gadget_register) (struct usb_gadget_driver *driver),
		int (*gadget_unregister) (struct usb_gadget_driver *driver))
{
	struct _usb_composite_dev *cdev =
				&the_cdevs.cdevs[the_cdevs.next_cdev_index];
	int rc;
	
	if (the_cdevs.num_cdevs >= mod_data.num_controllers) {
		printk(KERN_ERR "%s(): Can not"
			" register another controller driver.\n", __FUNCTION__);
		return -EINVAL;
	}
	
	cdev->gadget_register   = gadget_register;
	cdev->gadget_unregister = gadget_unregister;
	
	rc = cdev->gadget_register(&composite_gadget_driver);
	
	the_cdevs.next_cdev_index++;
	
	return rc;
}
EXPORT_SYMBOL(usb_gadget_controller_register);

int usb_gadget_controller_unregister(struct usb_gadget *gadget)
{
	struct _usb_composite_dev *cdev;
	int i;
	
	for (i = 0; i < mod_data.num_controllers; i++) {
		cdev = &the_cdevs.cdevs[i];
		if (cdev && (cdev->gadget == gadget)) {
			cdev->gadget_unregister(&composite_gadget_driver);
			break;
		}
	}
	return 0;	
}
EXPORT_SYMBOL(usb_gadget_controller_unregister);
#endif


static int __init composite_init(void)
{
	struct _usb_composite_dev	*cdev;
	int i;	
	
	printk(KERN_ALERT "%s() Desc: %s\n", __FUNCTION__, DRIVER_DESC);
	printk(KERN_ALERT "%s() Name: %s\n", __FUNCTION__, DRIVER_NAME);
	printk(KERN_ALERT "%s() Version: %s\n", __FUNCTION__, DRIVER_VERSION);

	if (mod_data.num_controllers > MAX_NUM_CONTROLLERS) {
		printk(KERN_ERR "%s(): Invalid number of"
					" Controller Drivers %d, \n",
						__FUNCTION__, 
						mod_data.num_controllers);
		return -EINVAL;
	}

	for (i = 0; i < mod_data.num_controllers; i++ ) {
		if ((functions[i] <= 0)
			|| (functions[i] > MAX_NUM_FUNCTIONS)) {
			printk(KERN_ERR "%s(): Invalid number of"
					" Function Drivers %d\n", 
					__FUNCTION__, functions[i]);
			 return -EINVAL;
		}
	}		
		
	// Allocate the composite device structures
	the_cdevs.cdevs = kzalloc(mod_data.num_controllers *
				sizeof(struct _usb_composite_dev), GFP_KERNEL);
	if (!the_cdevs.cdevs)
		return -ENOMEM;

	for (i = 0; i < mod_data.num_controllers; i++) { 
		cdev = &the_cdevs.cdevs[i];
#if !defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
		cdev->gadget_register = usb_gadget_register_driver;
		cdev->gadget_unregister = usb_gadget_unregister_driver;
#endif
	}
		
	the_cdevs.next_cdev_index = 0;
	the_cdevs.num_cdevs = 0;

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
	return 0;
#else
	cdev = &the_cdevs.cdevs[0];
	return cdev->gadget_register(&composite_gadget_driver);
#endif
	
}
module_init(composite_init);

static void __exit composite_cleanup(void)
{
#if !defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
	struct _usb_composite_dev	*cdev;

	cdev = &the_cdevs.cdevs[0];
	cdev->gadget_unregister(&composite_gadget_driver);
#endif
	if (the_cdevs.cdevs)
		kfree(the_cdevs.cdevs);
}
module_exit(composite_cleanup);


MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_AUTHOR("Tony Scragg");
MODULE_LICENSE("GPL v2");
