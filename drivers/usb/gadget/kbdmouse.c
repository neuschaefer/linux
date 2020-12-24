/*
 * g_kbdmouse.c -- Gadget HIS keyboard and mouse driver
 *
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
#include <linux/uts.h>
#include <linux/version.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/dma-mapping.h>

#include <asm/byteorder.h>
//#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>

#include <asm/uaccess.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
   #include <linux/config.h>
   #include <linux/usb_ch9.h>
   #include "../input/hid.h"
#else
   #include <linux/usb/ch9.h>
   #include <linux/hid.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
   #include <linux/usb_gadget.h>
#else
   #include <linux/usb/gadget.h>
#endif

#include <linux/fs.h>
#include <linux/cdev.h>

#include "gadget_chips.h"

#include <linux/input.h>
#include "composite.h"

/*-------------------------------------------------------------------------*/
#define DRIVER_DESC		"Keyboard/Mouse Function"
#define DRIVER_NAME		"g_kbdmouse"
#define DRIVER_VERSION		"20120912"
#define CONTROLLER_NAME         "PCD"

static const char shortname [] = DRIVER_NAME;
static const char longname [] = DRIVER_DESC;

#define DRIVER_VENDOR_ID        0x0624	// Avocent
#define DRIVER_PRODUCT_ID       0x0249

/*-------------------------------------------------------------------------*/

static struct {
	char            *controller;
	unsigned short  idVendor;
	unsigned short  idProduct;
	unsigned short  bcdDevice;
} mod_data = {
	.controller		= CONTROLLER_NAME,
	.idVendor		= DRIVER_VENDOR_ID,
	.idProduct		= DRIVER_PRODUCT_ID,
	.bcdDevice		= 0x0000,
};

#define	CONTROLLER_SIZE		16
static char controller[CONTROLLER_SIZE+1];

module_param_named(controller, mod_data.controller, charp, S_IRUGO);
MODULE_PARM_DESC(controller, "Name of peripheral controller driver");

module_param_named(idVendor, mod_data.idVendor, ushort, S_IRUGO);
MODULE_PARM_DESC(idVendor, "idVendor field of Device Descriptor");

module_param_named(idProduct, mod_data.idProduct, ushort, S_IRUGO);
MODULE_PARM_DESC(dProduct, "idProduct field of Device Descriptor");

module_param_named(bcdDevice, mod_data.bcdDevice, ushort, S_IRUGO);
MODULE_PARM_DESC(bcdDevice, "bcdDevice field of Device Descriptor");


/*-------------------------------------------------------------------------*/
// IOCTL types
#define IOCTL_KBDMOUSE_GET_MOUSE_MODE _IOR('i', 190, unsigned int)
#define IOCTL_KBDMOUSE_SET_MOUSE_MODE _IOW('i', 191, unsigned int)
#define IOCTL_KBDMOUSE_STATUS         _IOWR('i', 192, struct kbdmse_status_data)
#define IOCTL_KBDMOUSE_CONNECT        _IOW('i' , 193, unsigned int)

struct kbdmse_status_data
{
	u16 flags;
	u8  kbd_status;
};

// Request types
#define KBDMOUSE_USBDATA_FLAG_KBD_STATUS        1

/*-------------------------------------------------------------------------*/

/*
 * driver assumes self-powered hardware, and
 * has no way for users to trigger remote wakeup.
 *
 * this version autoconfigures as much as possible,
 * which is reasonable for most "bulk-only" drivers.
 */
static const char *EP_KBD_NAME;
static const char *EP_MOUSE_NAME;

#ifdef CONFIG_USB_MOUSE_RELATIVE
static const char *EP_MOUSE_NAME_REL;
#endif

/*-------------------------------------------------------------------------*/

enum mouse_mode {
	MOUSE_MODE_BOOT,
	MOUSE_MODE_ABS,
	MOUSE_MODE_ABS_WACOM,
	MOUSE_MODE_PASS_THRU,
	MOUSE_MODE_REL_NONE,
	MOUSE_MODE_REL_LOW,
	MOUSE_MODE_REL_MED,
	MOUSE_MODE_REL_HIGH,
	MOUSE_MODE_REL_XF86,
	MOUSE_MODE_REL_NETWARE,
	MOUSE_MODE_MAX
};

/* big enough to hold our biggest descriptor */
#define USB_BUFSIZ	256

struct hid_param {
	unsigned short idle;
	unsigned short protocol;
};

struct hid_dev {
	spinlock_t		lock;
	struct usb_gadget	*gadget;
	struct usb_request	*req;		/* for control responses */
	u8                      ep0_recv; // indicates to receive data on ep0

	u8			config;
	struct usb_ep		*kbd_ep, *mouse_ep;
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	struct usb_ep		*mouse_ep_relative; 
#endif
	struct usb_request	*kbd_req, *mouse_req;
	
	enum mouse_mode         mouse_mode;
	enum mouse_mode		os_mouse_mode;

	/* autoresume timer */
	struct timer_list	resume;

	struct hid_param	param[3];
	
	wait_queue_head_t       status_wait_q;
	wait_queue_head_t       kbd_write_wait_q;
	wait_queue_head_t      	mouse_write_wait_q;
	u8                      write_aborted;
	
	struct kbdmse_status_data  status;
	
	dev_t                   chrdev;
	struct cdev             cdev;	
};

struct hid_chrdev {
	int		mode;
	unsigned char	buf[16];
};

#ifdef CONFIG_USB_COMPOSITE
int function_index = 0;
#endif

/*-------------------------------------------------------------------------*/
static struct hid_dev *g_kbdmouse;
static unsigned buflen = 16;//4096;
/*
 * if it's nonzero, autoresume says how many seconds to wait
 * before trying to wake up the host after suspend.
 */
static unsigned autoresume = 0;

/*-------------------------------------------------------------------------*/

/*
 * DESCRIPTORS ... most are static, but strings and (full)
 * configuration descriptors are built on demand.
 */

#define KBDMOUSE_MAJOR		245

#define STRING_KBD_INTERFACE	1
#define STRING_MOUSE_INTERFACE	2

#ifdef CONFIG_USB_MOUSE_RELATIVE
  #define STRING_MOUSE_INTERFACE_REL	3
#endif

#define	CONFIG_HID		1
#define	HID_INTF_KBD		0
#define	HID_INTF_MOUSE		1

#ifdef CONFIG_USB_MOUSE_RELATIVE
  #define	HID_INTF_MOUSE_REL	2
#endif

#ifndef CONFIG_USB_COMPOSITE
  #define STRING_MANUFACTURER			4
  #define STRING_PRODUCT			5
  #define STRING_SERIAL				6

static struct usb_device_descriptor device_desc = {
	.bLength                = sizeof device_desc,
	.bDescriptorType        = USB_DT_DEVICE,

	.bcdUSB                 = __constant_cpu_to_le16 (0x0200),
	.bDeviceClass           = USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass        = 0,
	.bDeviceProtocol        = 0,

	.idVendor               = __constant_cpu_to_le16(DRIVER_VENDOR_ID),
	.idProduct              = __constant_cpu_to_le16(DRIVER_PRODUCT_ID),
	.bcdDevice		= __constant_cpu_to_le16(0x0000),
	.iManufacturer          = STRING_MANUFACTURER,
	.iProduct               = STRING_PRODUCT,
	.iSerialNumber          = STRING_SERIAL,
	.bNumConfigurations     = 1,
};

static struct usb_config_descriptor hid_config = {
	.bLength                = sizeof hid_config,
	.bDescriptorType        = USB_DT_CONFIG,

	/* wTotalLength computed by usb_gadget_config_buf() */
#ifdef CONFIG_USB_MOUSE_RELATIVE
	.bNumInterfaces         = 3,
#else
	.bNumInterfaces         = 2,
#endif
	.bConfigurationValue    = CONFIG_HID,
	.iConfiguration         = 0,
	.bmAttributes           = USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER | USB_CONFIG_ATT_WAKEUP,
	.bMaxPower              = 0x00,	/* self-powered */
};

static struct usb_qualifier_descriptor dev_qualifier = {
	.bLength                = sizeof dev_qualifier,
	.bDescriptorType        = USB_DT_DEVICE_QUALIFIER,
	
	.bcdUSB                 = __constant_cpu_to_le16 (0x0200),	
	.bDeviceClass           = USB_CLASS_HID,
	.bDeviceProtocol        = 0,

	.bMaxPacketSize0        = 0,
	.bNumConfigurations     = 1,
};

#endif

static struct usb_otg_descriptor
otg_desc = {
	.bLength =		sizeof(otg_desc),
	.bDescriptorType =	USB_DT_OTG,

	.bmAttributes =		USB_OTG_SRP,
};

// This is the same as a Dell USB Keyboard.
static unsigned char hid_report_kbd[] = {
	0x05, 0x01,	// Usage Page (Generic Desktop Control)
	0x09, 0x06,	// Usage (Keyboard)
	0xA1, 0x01,	// Collection (Application)
		0x05, 0x07, // Usage Page (Keyboard)
		0x19, 0xE0,	// Usage Minimum (224) (Left Control)
		0x29, 0xE7,	// Usage Maximum (231) (Right Control)
		0x15, 0x00,	// Logical Minimum (0)
		0x25, 0x01,	// Logical Maximum (1)
		0x75, 0x01,	// Report Size (1)
		0x95, 0x08,	// Report Count (8)
		0x81, 0x02, // Input (Data, Variable, Absolute, Bit Field)
		0x95, 0x01,	// Report Count (1)
		0x75, 0x08,	// Report Size (8)
		0x81, 0x01, // Input (Constant, Array, Absolute, Bit Field)
//		0x95, 0x03,	// Report Count (3)
		0x95, 0x05,	// Report Count (5)
		0x75, 0x01,	// Report Size (1)
		0x05, 0x08, // Usage Page (LEDs)
		0x19, 0x01,	// Usage Minimum (1) (Num Lock)
//		0x29, 0x03,	// Usage Maximum (3) (Scroll Lock)
		0x29, 0x05,	// Usage Maximum (5) (Kana)
		0x91, 0x02, // Output (Data, Value, Absolute, Non-volatile, Bit Field)
		0x95, 0x01,	// Report Count (1)
		0x75, 0x03,	// Report Size (3)
//		0x75, 0x05,	// Report Size (5)
		0x91, 0x01, // Output (Constant, Array, Absolute, Non-volatile, Bit Field)
		0x95, 0x06,	// Report Count (6)
		0x75, 0x08,	// Report Size (8)
		0x15, 0x00,	// Logical Minimum (0)
//		0x25, 0x65,	// Logical Maximum (101)
//		0x26, 0x8B,	// Logical Maximum (139)
		0x26, 0xFF, 0x00, 	// Logical Maximum (255)
		0x05, 0x07, // Usage Page (Keyboard)
		0x19, 0x00,	// Usage Minimum (0)
//		0x29, 0x65,	// Usage Maximum (101)
//		0x29, 0x8B,	// Usage Maximum (139)
		0x2A, 0xFF, 0x00,	// Usage Maximum (255)
		0x81, 0x00,	// Input (Data, Array, Absolute, Bit Field)
	0xC0		// End Collection
};

static unsigned char hid_report_mouse_absolute[] = {
	0x05, 0x01,	// Usage Page (Generic Desktop Control)
	0x09, 0x02,	// Usage (Mouse)
	0xA1, 0x01,	// Collection (Application)
		0x09, 0x01,	// Usage (Pointer)
		0xA1, 0x00,	// Collection (Physical)
			0x05, 0x09,			// Usage Page (Button)
			0x19, 0x01,			// Usage Minimum (1)
			0x29, 0x03,			// Usage Maximum (3)
			0x15, 0x00,			// Logical Minimum (0)
			0x25, 0x01,			// Logical Maximum (1)
			0x95, 0x03,			// Report Count (3)
			0x75, 0x01,			// Report Size (1)
			0x81, 0x02,			// Input (Data, Variable, Absolute)
			0x95, 0x01,			// Report Count (1)
			0x75, 0x05,			// Report Size (5)
			0x81, 0x03,			// Input (Constant)
			0x05, 0x01,			// Usage Page (Generic Desktop Control)
			0x09, 0x30,			// Usage (X)
			0x09, 0x31,			// Usage (Y)
			0x15, 0x00,			// Logical Minimum (0)
			0x26, 0xFF, 0x7F,	// Logical Maximum (32767)
			0x75, 0x10,			// Report Size (16)
			0x95, 0x02,			// Report Count (2)
			0x81, 0x02,			// Input (Data, Variable, Absolute)
			0x09, 0x38,			// Usage (Wheel)
			0x15, 0x81,			// Logical Minimum (-127)
			0x25, 0x7F,			// Logical Maximum (127)
			0x75, 0x08,			// Report Size (8)
			0x95, 0x01,			// Report Count (1)
			0x81, 0x06,			// Input (Data, Variable, Relative)
		0xC0,	// End Collection
	0xC0	// End Collection
};

static unsigned char hid_report_mouse_relative[] =
{
	0x05, 0x01,	// Usage Page (Generic Desktop Control)
	0x09, 0x02,	// Usage (Mouse)
	0xA1, 0x01,	// Collection (Application)
		0x09, 0x01,	// Usage (Pointer)
		0xA1, 0x00,	// Collection (Physical)
			0x05, 0x09,			// Usage Page (Button)
			0x19, 0x01,			// Usage Minimum (1)
			0x29, 0x03,			// Usage Maximum (3)
			0x15, 0x00,			// Logical Minimum (0)
			0x25, 0x01,			// Logical Maximum (1)
			0x95, 0x03,			// Report Count (3)
			0x75, 0x01,			// Report Size (1)
			0x81, 0x02,			// Input (Data, Variable, Absolute)
			0x95, 0x01,			// Report Count (1)
			0x75, 0x05,			// Report Size (5)
			0x81, 0x01,			// Input (Constant)
			0x05, 0x01,			// Usage Page (Generic Desktop Control)
			0x09, 0x30,			// Usage (X)
			0x09, 0x31,			// Usage (Y)
			0x09, 0x38,			// Usage (Wheel)
			0x15, 0x81,			// Logical Minimum (-127)
			0x25, 0x7F,			// Logical Maximum (127)
			0x75, 0x08,			// Report Size (8)
			0x95, 0x03,			// Report Count (3)
			0x81, 0x06,			// Input (Data, Variable, Relative)
		0xC0,		// End Collection
	0xC0		// End Collection
};

static struct hid_descriptor hid_desc[3] = {
{
	.bLength = 		sizeof (struct hid_descriptor),
	.bDescriptorType =	HID_DT_HID,

	.bcdHID =		__constant_cpu_to_le16 (0x0100),
	.bCountryCode =		0x00,
	.bNumDescriptors =	1,
	.desc[0] = {
		.bDescriptorType =	HID_DT_REPORT,
		.wDescriptorLength =	__constant_cpu_to_le16(
						sizeof hid_report_kbd),
	},
},
{
	.bLength = 		sizeof (struct hid_descriptor),
	.bDescriptorType =	HID_DT_HID,

	.bcdHID =		__constant_cpu_to_le16 (0x0100),
	.bCountryCode =		0x00,
	.bNumDescriptors =	1,
	.desc[0] = {
		.bDescriptorType =	HID_DT_REPORT,
		.wDescriptorLength =	__constant_cpu_to_le16(
						sizeof hid_report_mouse_absolute),
	},
},
{
	.bLength = 		sizeof (struct hid_descriptor),
	.bDescriptorType =	HID_DT_HID,

	.bcdHID =		__constant_cpu_to_le16 (0x0100),
	.bCountryCode =		0x00,
	.bNumDescriptors =	1,
	.desc[0] = {
		.bDescriptorType =	HID_DT_REPORT,
		.wDescriptorLength =	__constant_cpu_to_le16(
						sizeof hid_report_mouse_relative),
	},
},
};

/* one interface in each configuration */
static struct usb_interface_descriptor intf_desc[3] = {
{
	.bLength =		sizeof (struct usb_interface_descriptor),
	.bDescriptorType =	USB_DT_INTERFACE,

	.bInterfaceNumber =	HID_INTF_KBD,
	.bNumEndpoints =	1,
	.bInterfaceClass =	USB_CLASS_HID,
	.bInterfaceSubClass =	1,	/* boot interface subclass */
	.bInterfaceProtocol =	1,	/* keyboard */
	.iInterface =		STRING_KBD_INTERFACE,
},
{
	.bLength =		sizeof (struct usb_interface_descriptor),
	.bDescriptorType =	USB_DT_INTERFACE,

	.bInterfaceNumber =	HID_INTF_MOUSE,
	.bNumEndpoints =	1,
	.bInterfaceClass =	USB_CLASS_HID,
	.bInterfaceSubClass =	1,	/* boot interface subclass */
	.bInterfaceProtocol =	2,	/* mouse */
	.iInterface =		STRING_MOUSE_INTERFACE,
},
#ifdef CONFIG_USB_MOUSE_RELATIVE
{
	.bLength =		sizeof (struct usb_interface_descriptor),
	.bDescriptorType =	USB_DT_INTERFACE,

	.bInterfaceNumber =	HID_INTF_MOUSE_REL,
	.bNumEndpoints =	1,
	.bInterfaceClass =	USB_CLASS_HID,
	.bInterfaceSubClass =	1,	/* boot interface subclass */
	.bInterfaceProtocol =	2,	/* mouse */
	.iInterface =		STRING_MOUSE_INTERFACE_REL,
},
#endif
};

#ifdef CONFIG_USB_COMPOSITE
static struct usb_interface_info	intf_info[3] = {
{
	.enabled		= 1,
	.intf_desc		= &intf_desc[0],
},
{
	.enabled		= 1,
	.intf_desc		= &intf_desc[1],
},
#ifdef CONFIG_USB_MOUSE_RELATIVE
{
	.enabled		= 1,
	.intf_desc		= &intf_desc[2],
}
#endif
};

static struct usb_interface_info *intf_info_table[] = {
	&intf_info[0],
	&intf_info[1],
#ifdef CONFIG_USB_MOUSE_RELATIVE
	&intf_info[2],
#else
	NULL,
#endif	
	NULL,
};
#endif

static struct usb_endpoint_descriptor fs_desc[3] = {
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (8),
	.bInterval =		8,
},
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (8),
	.bInterval =		8,
},
#ifdef CONFIG_USB_MOUSE_RELATIVE
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (8),
	.bInterval =		8,
},
#endif
};

static const struct usb_descriptor_header *fs_hid_function [] = {
	(struct usb_descriptor_header *) &otg_desc,
	(struct usb_descriptor_header *) &intf_desc[0],
	(struct usb_descriptor_header *) &hid_desc[0],
	(struct usb_descriptor_header *) &fs_desc[0],
	(struct usb_descriptor_header *) &intf_desc[1],
	(struct usb_descriptor_header *) &hid_desc[1],
	(struct usb_descriptor_header *) &fs_desc[1],
#ifdef CONFIG_USB_MOUSE_RELATIVE
	(struct usb_descriptor_header *) &intf_desc[2],
	(struct usb_descriptor_header *) &hid_desc[2],
	(struct usb_descriptor_header *) &fs_desc[2],
#endif	
	NULL,
};

#ifdef	CONFIG_USB_GADGET_DUALSPEED

/*
 * usb 2.0 devices need to expose both high speed and full speed
 * descriptors, unless they only run at full speed.
 *
 * that means alternate endpoint descriptors (bigger packets)
 * and a "device qualifier" ... plus more construction options
 * for the config descriptor.
 */

static struct usb_endpoint_descriptor hs_desc[3] = {
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (64),
	.bInterval =		4,
},
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (64),
	.bInterval =		4,
},
#ifdef CONFIG_USB_MOUSE_RELATIVE
{
	.bLength =		USB_DT_ENDPOINT_SIZE,
	.bDescriptorType =	USB_DT_ENDPOINT,

	.bEndpointAddress =	USB_DIR_IN,
	.bmAttributes =		USB_ENDPOINT_XFER_INT,
	.wMaxPacketSize =	__constant_cpu_to_le16 (64),
	.bInterval =		4,
},
#endif
};

static const struct usb_descriptor_header *hs_hid_function [] = {
	(struct usb_descriptor_header *) &otg_desc,
	(struct usb_descriptor_header *) &intf_desc[0],
	(struct usb_descriptor_header *) &hid_desc[0],
	(struct usb_descriptor_header *) &hs_desc[0],
	(struct usb_descriptor_header *) &intf_desc[1],
	(struct usb_descriptor_header *) &hid_desc[1],
	(struct usb_descriptor_header *) &hs_desc[1],
#ifdef CONFIG_USB_MOUSE_RELATIVE
	(struct usb_descriptor_header *) &intf_desc[2],
	(struct usb_descriptor_header *) &hid_desc[2],
	(struct usb_descriptor_header *) &hs_desc[2],
#endif	
	NULL,
};

/* maxpacket and other transfer characteristics vary by speed. */
// Force FULL speed descriptors because certain BIOS's don't like HS
#define ep_desc(g,hs,fs) fs
//#define ep_desc(g,hs,fs) (((g)->speed==USB_SPEED_HIGH)?(hs):(fs))

#else

/* if there's no high speed support, maxpacket doesn't change. */
#define ep_desc(g,hs,fs) fs

#endif	/* !CONFIG_USB_GADGET_DUALSPEED */

/* static strings, in iso 8859/1 */
static struct usb_string		strings [] = {
	{STRING_KBD_INTERFACE,		"Keyboard"},
	{STRING_MOUSE_INTERFACE,	"Mouse"},
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	{STRING_MOUSE_INTERFACE_REL,	"Mouse REL"},
#endif
#ifndef CONFIG_USB_COMPOSITE
	{ STRING_MANUFACTURER,		"Avocent",},
	{ STRING_PRODUCT,		longname,},
	{ STRING_SERIAL,		DRIVER_VERSION,},
#endif
	{  }			/* end of list */
};

static struct usb_gadget_strings	stringtab = {
	.language	= 0x0409,	/* en-us */
	.strings	= strings,
};

#ifdef CONFIG_USB_COMPOSITE
static const struct usb_gadget_strings *stringTable[] = {
	(struct usb_gadget_strings *) &stringtab,
	NULL,
};
#endif

static int
hid_device_buf (u8 *buf, u16 wIndex)
{
	int len = 0;

	if (wIndex == intf_desc[0].bInterfaceNumber) {
		len = sizeof(hid_desc[0]);
		memcpy(buf, &hid_desc[0], len);
	} else if (wIndex == intf_desc[1].bInterfaceNumber) {
#ifndef CONFIG_USB_MOUSE_RELATIVE	
		if (g_kbdmouse->os_mouse_mode == MOUSE_MODE_ABS) {
			len = sizeof(hid_desc[1]);
			memcpy(buf, &hid_desc[1], len);
		} else {
			len = sizeof(hid_desc[2]);
			memcpy(buf, &hid_desc[2], len);
		}
#else
		len = sizeof(hid_desc[1]);
		memcpy(buf, &hid_desc[1], len);
#endif

	}
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	else if (wIndex == intf_desc[2].bInterfaceNumber) {
		len = sizeof(hid_desc[2]);
		memcpy(buf, &hid_desc[2], len);
	}
#endif

	return len;
}

static int
hid_config_buf (u8 *buf, u16 wIndex)
{
	int len = 0;

	if (wIndex == intf_desc[0].bInterfaceNumber) {
		len = sizeof(hid_report_kbd);
		memcpy(buf, hid_report_kbd, len);
	} else if (wIndex == intf_desc[1].bInterfaceNumber) {
#ifndef CONFIG_USB_MOUSE_RELATIVE	
		if (g_kbdmouse->os_mouse_mode == MOUSE_MODE_ABS) {
			len = sizeof(hid_report_mouse_absolute);
			memcpy(buf, hid_report_mouse_absolute, len);
		} else {
			len = sizeof(hid_report_mouse_relative);
			memcpy(buf, hid_report_mouse_relative, len);
		}
#else
		len = sizeof(hid_report_mouse_absolute);
		memcpy(buf, hid_report_mouse_absolute, len);
#endif
		
		// Kludge for windows since it doesn't send us a
		// SET_PROTOCOL command
		g_kbdmouse->mouse_mode = g_kbdmouse->os_mouse_mode;
	}
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	else if (wIndex == intf_desc[2].bInterfaceNumber) {
		len = sizeof(hid_report_mouse_relative);
		memcpy(buf, hid_report_mouse_relative, len);
		
		// Kludge for windows since it doesn't send us a
		// SET_PROTOCOL command
		g_kbdmouse->mouse_mode = g_kbdmouse->os_mouse_mode;
	}
#endif

	return len;
}


#ifndef CONFIG_USB_COMPOSITE
/*
 * config descriptors are also handcrafted.  these must agree with code
 * that sets configurations, and with code managing interfaces and their
 * altsettings.  other complexity may come from:
 *
 *  - high speed support, including "other speed config" rules
 *  - multiple configurations
 *  - interfaces with alternate settings
 *  - embedded class or vendor-specific descriptors
 *
 * this handles high speed, and has a second config that could as easily
 * have been an alternate interface setting (on most hardware).
 *
 * NOTE:  to demonstrate (and test) more USB capabilities, this driver
 * should include an altsetting to test interrupt transfers, including
 * high bandwidth modes at high speed.  (Maybe work like Intel's test
 * device?)
 */
static int config_buf (struct usb_gadget *gadget,
		u8 *buf, u8 type, unsigned index)
{
	int				len;
	const struct usb_descriptor_header **function;
#ifdef CONFIG_USB_GADGET_DUALSPEED
	// Force FULL speed because certain BIOS's don't like HS
	int				hs = 0;
	//int				hs = (gadget->speed == USB_SPEED_HIGH);
#endif

	/* two configurations will always be index 0 and index 1 */
	if (index > 1)
		return -EINVAL;

#ifdef CONFIG_USB_GADGET_DUALSPEED
	if (type == USB_DT_OTHER_SPEED_CONFIG)
		hs = !hs;
	if (hs)
		function = hs_hid_function;
	else
#endif
		function = fs_hid_function;
		
	if (!gadget->is_otg)
		function++;

	len = usb_gadget_config_buf (&hid_config, buf, USB_BUFSIZ, function);
	if (len < 0)
		return len;
	((struct usb_config_descriptor *) buf)->bDescriptorType = type;
	return len;
}
#endif

/*-------------------------------------------------------------------------*/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,23)
void * _usb_ep_alloc_buffer(struct usb_gadget *gadget,
				   struct usb_ep *_ep,
                                   unsigned bytes,
                                   dma_addr_t *dma,
                                   gfp_t gfp_flags)
{
	void *buf;
	
#if defined (CONFIG_USB_SH7757_ENABLE_DMA_TRANSFER)
	buf = kmalloc(bytes, gfp_flags);
#else
	*dma = DMA_ADDR_INVALID;	
	buf = dma_alloc_coherent(gadget->dev.parent,
				bytes, dma, gfp_flags);
#endif

	return buf;	
}

void _usb_ep_free_buffer(struct usb_gadget *gadget,
				struct usb_ep *_ep,
                                void *buf,
                                dma_addr_t dma,
                                unsigned bytes)
{
#if defined (CONFIG_USB_SH7757_ENABLE_DMA_TRANSFER)
	if (buf)
		kfree(buf);
#else
	if (dma != DMA_ADDR_INVALID) {
		dma_free_coherent(gadget->dev.parent,
					bytes, buf, dma);
	}	
#endif
}
#endif

static struct usb_request *
alloc_ep_req (struct usb_ep *ep)
{
	struct usb_request	*req = NULL;

	req = usb_ep_alloc_request (ep, GFP_KERNEL);
	if (req) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
		req->buf = usb_ep_alloc_buffer (ep, buflen,
				&req->dma, GFP_KERNEL);
#else
		req->buf = _usb_ep_alloc_buffer (g_kbdmouse->gadget, ep, buflen,
				&req->dma, GFP_KERNEL);
#endif
		if (!req->buf) {
			usb_ep_free_request (ep, req);
			req = NULL;
		}
	}

	return req;
}

static void free_ep_req (struct usb_ep *ep, struct usb_request *req)
{
	if (req) {
		if (req->buf) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
			usb_ep_free_buffer (ep, req->buf, req->dma, buflen);
#else
			_usb_ep_free_buffer (g_kbdmouse->gadget, ep, req->buf,
							req->dma, buflen);
#endif
		}
		usb_ep_free_request (ep, req);
	}
}

/*-------------------------------------------------------------------------*/
static void kbd_complete (struct usb_ep *ep, struct usb_request *req)
{
	struct hid_dev	*dev = ep->driver_data;
	
	if (req->status || req->actual != req->length)
		printk(KERN_DEBUG "%s: %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, req->length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	dev->kbd_req->complete = NULL;
	// wake up any user threads waiting on this condition. 
	wake_up_interruptible(&dev->kbd_write_wait_q);	
}

static struct usb_request *
kbd_start_ep (struct usb_ep *ep, void *buf, unsigned length, int gfp_flags)
{
	int			status;
	struct hid_dev	*dev = ep->driver_data;
	struct usb_request	*req = dev->kbd_req;
	
	req->complete = kbd_complete;
	memcpy(req->buf, buf, length);
	req->length = length;

	status = usb_ep_queue (ep, req, gfp_flags);
	if (status) {
		printk(KERN_ERR "%s: start %s --> %d\n", __FUNCTION__,
							ep->name, status);
	}

	return req;
}

static void mouse_complete (struct usb_ep *ep, struct usb_request *req)
{
	struct hid_dev	*dev = ep->driver_data;
	
	if (req->status || req->actual != req->length)
		printk(KERN_DEBUG "%s: %d, %u/%u\n", __FUNCTION__,
				req->status, req->actual, req->length);
	if (req->status == -ECONNRESET)		// Request was cancelled
		usb_ep_fifo_flush(ep);

	dev->mouse_req->complete = NULL;
	// wake up any user threads waiting on this condition. 
	wake_up_interruptible(&dev->mouse_write_wait_q);
}

static struct usb_request *
mouse_start_ep (struct usb_ep *ep, void *buf, unsigned length, int gfp_flags)
{
	int			status;
	struct hid_dev	*dev = ep->driver_data;
	struct usb_request	*req = dev->mouse_req;

	req->complete = mouse_complete;
	memcpy(req->buf, buf, length);
	req->length = length;

	status = usb_ep_queue (ep, req, gfp_flags);
	if (status) {
		printk(KERN_ERR "%s: start %s --> %d\n", __FUNCTION__,
							ep->name, status);
	}

	return req;
}


static int set_hid_config (struct hid_dev *dev, int gfp_flags)
{
	int			result = 0;
	struct usb_ep		*ep;
	struct usb_gadget	*gadget = dev->gadget;

	gadget_for_each_ep (ep, gadget) {
		const struct usb_endpoint_descriptor	*d;

		if (strcmp (ep->name, EP_KBD_NAME) == 0) {
			d = ep_desc (gadget, &hs_desc[0], &fs_desc[0]);
			result = usb_ep_enable (ep, d);
			if (result == 0) {
				ep->driver_data = dev;
				dev->kbd_ep = ep;
				dev->kbd_req->complete = NULL;
				continue;
			}

		} else if (strcmp (ep->name, EP_MOUSE_NAME) == 0) {
			d = ep_desc (gadget, &hs_desc[1],
						&fs_desc[1]);
			result = usb_ep_enable (ep, d);
			if (result == 0) {
				ep->driver_data = dev;
				dev->mouse_ep = ep;
				dev->mouse_req->complete = NULL;
				continue;
			}

#ifdef CONFIG_USB_MOUSE_RELATIVE			
		} else if (strcmp (ep->name, EP_MOUSE_NAME_REL) == 0) {
			d = ep_desc (gadget, &hs_desc[2],
						&fs_desc[2]);
			result = usb_ep_enable (ep, d);
			if (result == 0) {
				ep->driver_data = dev;
				dev->mouse_ep_relative = ep;
				dev->mouse_req->complete = NULL;
				continue;
			}
#endif
		/* ignore any other endpoints */
		} else
			continue;

		/* stop on error */
		printk(KERN_ERR "%s: can't start %s, result %d\n",
						__FUNCTION__, ep->name, result);
		break;
	}

	/* caller is responsible for cleanup on error */
	return result;
}

/*-------------------------------------------------------------------------*/

static void hid_reset_config (struct hid_dev *dev)
{
	if (dev->config == 0)
		return;

	//printk(KERN_DEBUG "%s: reset config\n", __FUNCTION__);

	/* just disable endpoints, forcing completion of pending i/o.
	 * all our completion handlers free their requests in this case.
	 */
	if (dev->kbd_ep) {
		usb_ep_disable (dev->kbd_ep);
	}
	if (dev->mouse_ep) {
		usb_ep_disable (dev->mouse_ep);
	}
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	if (dev->mouse_ep_relative) {
		usb_ep_disable (dev->mouse_ep_relative);
	}
#endif	
	dev->config = 0;
	dev->mouse_mode = MOUSE_MODE_BOOT;
	del_timer (&dev->resume);
}

/* change our operational config.  this code must agree with the code
 * that returns config descriptors, and altsetting code.
 *
 * it's also responsible for power management interactions. some
 * configurations might not work with our current power sources.
 *
 * note that some device controller hardware will constrain what this
 * code can do, perhaps by disallowing more than one configuration or
 * by limiting configuration choices (like the pxa2xx).
 */
static int hid_set_config (struct hid_dev *dev, unsigned number, int gfp_flags)
{
	int			result = 0;
	struct usb_gadget	*gadget = dev->gadget;

	if (number == dev->config)
		return 0;

	if (gadget_is_sa1100 (gadget) && dev->config) {
		/* tx fifo is full, but we can't clear it...*/
		printk(KERN_ERR "%s: can't change configurations\n",
								__FUNCTION__);
		return -ESPIPE;
	}
	hid_reset_config (dev);


	// Don't care what configuration is
	if (number > 0)
		result = set_hid_config (dev, gfp_flags);
	else if (number == 0)
		return result;
	else
		result = -EINVAL;

	if (!result && (!dev->kbd_ep || !dev->mouse_ep
#ifdef CONFIG_USB_MOUSE_RELATIVE	
		|| !dev->mouse_ep_relative
#endif		
		))
		result = -ENODEV;
	if (result)
		hid_reset_config (dev);
	else {
		char *speed;

		switch (gadget->speed) {
		case USB_SPEED_LOW:	speed = "low"; break;
		case USB_SPEED_FULL:	speed = "full"; break;
		case USB_SPEED_HIGH:	speed = "high"; break;
		default: 		speed = "?"; break;
		}

		dev->config = number;
		
		// Kludge for windows since it doesn't send us a
		// SET_PROTOCOL command
		dev->mouse_mode = dev->os_mouse_mode;		

		printk(KERN_ALERT DRIVER_NAME ": %s speed config #%d"
					" for %s\n", speed, number,
					"Keyboard/Mouse");		
	}
	return result;
}

/*-------------------------------------------------------------------------*/
static void hid_dequeue(struct hid_dev *dev)
{
	if (dev->kbd_ep && dev->kbd_req) {
		usb_ep_dequeue(dev->kbd_ep, dev->kbd_req);
	}
	if (dev->mouse_ep && dev->mouse_req) {
		usb_ep_dequeue(dev->mouse_ep, dev->mouse_req);
	}
#ifdef CONFIG_USB_MOUSE_RELATIVE	
	if (dev->mouse_ep_relative && dev->mouse_req) {
		usb_ep_dequeue(dev->mouse_ep_relative, dev->mouse_req);
	}
#endif	
}

#ifdef CONFIG_USB_COMPOSITE
static int hid_class_request (struct _usb_composite_dev *cdev, const struct usb_ctrlrequest *ctrl)
#else
static int hid_class_request (struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
#else
	struct hid_dev		*dev = get_gadget_data(gadget);
#endif
	struct usb_request	*req = dev->req;
	u16			w_index = le16_to_cpu(ctrl->wIndex);
	u16			w_value = le16_to_cpu(ctrl->wValue);
	int                     value = 0;

	switch (ctrl->bRequest) {
	case HID_REQ_GET_REPORT:
		memset(req->buf, 0, 8);
		if (w_index == intf_desc[0].bInterfaceNumber) {
				value = 8;
		} else if (w_index == intf_desc[1].bInterfaceNumber) {
			if (dev->mouse_mode == MOUSE_MODE_BOOT)
				value = 3;
			else if (dev->mouse_mode == MOUSE_MODE_ABS)
				value = 6;
			else
				value = 4;
		}
#ifdef CONFIG_USB_MOUSE_RELATIVE		
		else if (w_index == intf_desc[2].bInterfaceNumber) {
			if (dev->mouse_mode == MOUSE_MODE_BOOT)
				value = 3;
			else			
				value = 4;
		}
#endif
		else
			value = 0;
		break;
	case HID_REQ_GET_IDLE:
		memset(req->buf, dev->param[w_index&0x01].idle>>8, 1);
		value = 1;
		break;
	case HID_REQ_GET_PROTOCOL:
		memset(req->buf, dev->param[w_index&0x01].protocol, 1);
		value = 1;
		break;

	case HID_REQ_SET_REPORT:
		if (w_index == intf_desc[0].bInterfaceNumber) {
			dev->ep0_recv = 1;
			value = 1;
		}
		break;
	case HID_REQ_SET_IDLE:
		dev->param[w_index&0x01].idle = w_value;
		break;
	case HID_REQ_SET_PROTOCOL:
		dev->param[w_index&0x01].protocol = w_value;
		if (w_value == 1)
			dev->mouse_mode = dev->os_mouse_mode;
		else
			dev->mouse_mode = MOUSE_MODE_BOOT;		
		break;

	default:
		printk(KERN_ERR "%s: unexpected class request %x\n",
					__FUNCTION__, ctrl->bRequest);
		break;
	}

	return value;
}

static void hid_setup_complete (struct usb_ep *ep, struct usb_request *req)
{
	struct hid_dev    *dev = g_kbdmouse;
	u8  *buf_ptr;
	
	if (req->status || req->actual != req->length) {
		printk(KERN_ALERT "%s: %d, %d/%d\n", __FUNCTION__,
				req->status, req->actual, req->length);
	} else {
		if (dev->ep0_recv == 1) {
			buf_ptr = (u8*)req->buf;
			dev->status.flags = KBDMOUSE_USBDATA_FLAG_KBD_STATUS;
			dev->status.kbd_status = *buf_ptr;
			// Wake up pending thread
			wake_up_interruptible(&dev->status_wait_q);			
		}		
	}

	dev->ep0_recv = 0;
}

/*
 * The setup() callback implements all the ep0 functionality that's
 * not handled lower down, in hardware or the hardware driver (like
 * device and endpoint feature flags, and their status).  It's all
 * housekeeping for the gadget function we're implementing.  Most of
 * the work is in config-specific setup.
 */
#ifdef CONFIG_USB_COMPOSITE
static int hid_setup (struct _usb_composite_dev *cdev, const struct usb_ctrlrequest *ctrl)
#else
static int hid_setup (struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
	struct usb_gadget	*gadget = dev->gadget;
#else
	struct hid_dev		*dev = get_gadget_data(gadget);
#endif
	struct usb_request	*req = dev->req;
	int			value = -EOPNOTSUPP;
	u8			respond = 1;
	u16			w_index = le16_to_cpu(ctrl->wIndex);
	u16			w_value = le16_to_cpu(ctrl->wValue);
	u16			w_length = le16_to_cpu(ctrl->wLength);

	/* usually this stores reply data in the pre-allocated ep0 buffer,
	 * but config change events will reconfigure hardware.
	 */
	req->zero = 0;
	switch (ctrl->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		if (!(ctrl->bRequestType & USB_DIR_IN))
			goto unknown;
		switch (w_value >> 8) {
#ifndef CONFIG_USB_COMPOSITE
		case USB_DT_DEVICE:
			value = min (ctrl->wLength, (u16) sizeof device_desc);
			memcpy (req->buf, &device_desc, value);
			break;
#ifdef CONFIG_USB_GADGET_DUALSPEED
		case USB_DT_DEVICE_QUALIFIER:
			if (!gadget->is_dualspeed)
				break;
			value = min (ctrl->wLength, (u16) sizeof dev_qualifier);
			memcpy (req->buf, &dev_qualifier, value);
			break;

		case USB_DT_OTHER_SPEED_CONFIG:
			if (!gadget->is_dualspeed)
				break;
			// FALLTHROUGH
#endif /* CONFIG_USB_GADGET_DUALSPEED */
		case USB_DT_CONFIG:
			value = config_buf (gadget, req->buf,
					ctrl->wValue >> 8,
					ctrl->wValue & 0xff);
			if (value >= 0)
				value = min (ctrl->wLength, (u16) value);
			break;
#endif
		case HID_DT_HID:
			value = hid_device_buf(req->buf, w_index);
			if (value >= 0)
				value = min (w_length, (u16) value);
			break;
		case HID_DT_REPORT:
			value = hid_config_buf(req->buf, w_index);
			if (value >= 0)
				value = min (w_length, (u16) value);
			break;
#ifndef CONFIG_USB_COMPOSITE
		case USB_DT_STRING:
			/* wIndex == language code.
			 * this driver only handles one language, you can
			 * add others even if they don't use iso8859/1
			 */
			value = usb_gadget_get_string (&stringtab,
					ctrl->wValue & 0xff, req->buf);
			if (value >= 0)
				value = min (ctrl->wLength, (u16) value);
			break;
#endif
		default:
			break;
		}
		break;

	/* currently two configs, two speeds */
	case USB_REQ_SET_CONFIGURATION:
		if (ctrl->bRequestType != 0)
			goto unknown;
		spin_lock (&dev->lock);
		value = hid_set_config (dev, w_value, GFP_ATOMIC);
		spin_unlock (&dev->lock);
#ifdef CONFIG_USB_COMPOSITE
		respond = 0;  // composite layer will respond to this
		// Call composite driver to let it determine whether or not it can 
		// send the status stage yet.
		usb_composite_set_config_status_stage(cdev);
#endif
		break;
	case USB_REQ_GET_CONFIGURATION:
		if (ctrl->bRequestType != USB_DIR_IN)
			goto unknown;
		*(u8 *)req->buf = dev->config;
		value = min (w_length, (u16) 1);
		break;

	/* until we add altsetting support, or other interfaces,
	 * only 0/0 are possible.  pxa2xx only supports 0/0 (poorly)
	 * and already killed pending endpoint I/O.
	 */
	case USB_REQ_SET_INTERFACE:
	{
#ifdef CONFIG_USB_MOUSE_RELATIVE
		int bNumInterfaces         = 3;
#else
		int bNumInterfaces         = 2;
#endif
		if (ctrl->bRequestType != USB_RECIP_INTERFACE)
			goto unknown;
		spin_lock (&dev->lock);
		if (dev->config && w_index >= 0 && w_index <= bNumInterfaces && w_value == 0) {
			u8		config = dev->config;

			/* resets interface configuration, forgets about
			 * previous transaction state (queued bufs, etc)
			 * and re-inits endpoint state (toggle etc)
			 * no response queued, just zero status == success.
			 * if we had more than one interface we couldn't
			 * use this "reset the config" shortcut.
			 */
			hid_reset_config (dev);
			hid_set_config (dev, config, GFP_ATOMIC);
			value = 0;
		}
		spin_unlock (&dev->lock);
		break;
	}
	case USB_REQ_GET_INTERFACE:
	{
#ifdef CONFIG_USB_MOUSE_RELATIVE
		int bNumInterfaces         = 3;
#else
		int bNumInterfaces         = 2;
#endif
		if (ctrl->bRequestType != (USB_DIR_IN|USB_RECIP_INTERFACE))
			goto unknown;
		if (!dev->config)
			break;
		if (w_index < 0 || w_index >= bNumInterfaces) {
			value = -EDOM;
			break;
		}

		*(u8 *)req->buf = intf_desc[w_index].bAlternateSetting;
		value = min (w_length, (u16) 1);
		break;
	}
	default:
unknown:
		if ((ctrl->bRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS) {
#ifdef CONFIG_USB_COMPOSITE		
			value = hid_class_request(cdev, ctrl);
#else
			value = hid_class_request(gadget, ctrl);
#endif
		}
		else {
			printk(KERN_DEBUG
			"%s: unknown control req%02x.%02x v%04x i%04x l%d\n",
			__FUNCTION__, ctrl->bRequestType, ctrl->bRequest,
			w_value, w_index, w_length);
		}
	}
	
	/* respond with data transfer before status phase? */
	if (respond && value >= 0) {
		req->length = value;
		req->zero = value < w_length
				&& (value % gadget->ep0->maxpacket) == 0;
		value = usb_ep_queue (gadget->ep0, req, GFP_ATOMIC);
		if (value < 0) {
			printk(KERN_DEBUG "%s: ep_queue --> %d\n",
							__FUNCTION__, value);
			req->status = 0;
			hid_setup_complete (gadget->ep0, req);
		}
	}

	/* device either stalls (value < 0) or reports success */
	return value;
}

#ifdef CONFIG_USB_COMPOSITE
static void _hid_disconnect (struct _usb_composite_dev *cdev)
#else
static void _hid_disconnect (struct usb_gadget *gadget)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
#else
	struct hid_dev		*dev = get_gadget_data (gadget);
#endif
	unsigned long		flags;

	hid_dequeue(dev);
	
	/* abort pending read/write operations */
	dev->write_aborted = 1;
	dev->kbd_req->complete = NULL;
	dev->mouse_req->complete = NULL;
	wake_up_interruptible(&dev->kbd_write_wait_q);	
	wake_up_interruptible(&dev->mouse_write_wait_q);

	spin_lock_irqsave (&dev->lock, flags);
	hid_reset_config (dev);

	/* a more significant application might have some non-usb
	 * activities to quiesce here, saving resources like power
	 * or pushing the notification up a network stack.
	 */
	spin_unlock_irqrestore (&dev->lock, flags);

	/* next we may get setup() calls to enumerate new connections;
	 * or an unbind() during shutdown (including removing module).
	 */
}

static void hid_autoresume (unsigned long _dev)
{
	struct hid_dev	*dev = (struct hid_dev *) _dev;
	int		status;

	/* normally the host would be woken up for something
	 * more significant than just a timer firing...
	 */
	if (dev->gadget->speed != USB_SPEED_UNKNOWN) {
		status = usb_gadget_wakeup (dev->gadget);
		printk(KERN_DEBUG "%s: wakeup --> %d\n", __FUNCTION__, status);
	}
}


/*-------------------------------------------------------------------------*/
#ifdef CONFIG_USB_COMPOSITE
static void hid_unbind (struct _usb_composite_dev *cdev)
#else
static void hid_unbind (struct usb_gadget *gadget)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
	struct usb_gadget	*gadget;
#else
	struct hid_dev		*dev = get_gadget_data (gadget);
#endif

	//printk(KERN_DEBUG "%s: unbind\n", __FUNCTION__);

	if (!dev)
		return;

#ifdef CONFIG_USB_COMPOSITE
	gadget = dev->gadget;
#endif
	
	/* Free the request and buffer for endpoint 0 */
	if (dev->req) {
		if (dev->req->buf) {
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
			usb_ep_free_buffer(gadget->ep0, dev->req->buf,
						dev->req->dma, USB_BUFSIZ);
#else
			_usb_ep_free_buffer(gadget, gadget->ep0, dev->req->buf,
						dev->req->dma, USB_BUFSIZ);
#endif
		}
		usb_ep_free_request(gadget->ep0, dev->req);
	}
	
	// free the requests and buffers for kbd and mouse endpoints
	free_ep_req(dev->kbd_ep, dev->kbd_req);
	free_ep_req(dev->mouse_ep, dev->mouse_req);

	hid_dequeue(dev);

	hid_reset_config(dev);

	del_timer_sync (&dev->resume);

	// Unclaim the endpoints
	dev->kbd_ep->driver_data = NULL;
	dev->mouse_ep->driver_data = NULL;
#ifdef CONFIG_USB_MOUSE_RELATIVE
	dev->mouse_ep_relative->driver_data = NULL;
#endif
	
	if (dev)
		kfree (dev);

#ifdef CONFIG_USB_COMPOSITE
	set_composite_data(cdev, function_index, NULL);
#else
	set_gadget_data(gadget, NULL);
#endif
}

#ifdef CONFIG_USB_COMPOSITE
static int set_string_ids(struct _usb_composite_dev *cdev)
{
	struct usb_string *s;
	int               string_id = -EINVAL;
	
	for (s = stringtab.strings; (s && (s->id > 0)); s++)
	{
		string_id = usb_composite_string_id(cdev);
		
		if (string_id < 0)
			break;
		
		switch (s->id)
		{
			case STRING_KBD_INTERFACE:
				s->id = string_id;
				intf_desc[0].iInterface = string_id;
				break;
			case STRING_MOUSE_INTERFACE:
				s->id = string_id;
				intf_desc[1].iInterface = string_id;
				break;
#ifdef CONFIG_USB_MOUSE_RELATIVE				
			case STRING_MOUSE_INTERFACE_REL:
				s->id = string_id;
				intf_desc[2].iInterface = string_id;
				break;
#endif
			default:
				return -EINVAL;
		}
	}
	
	return string_id;
}
#endif

static int kbdmouse_open(struct inode *inode, struct file *file)
{
	struct hid_chrdev *chrdev;

	chrdev = kmalloc(sizeof(struct hid_chrdev), GFP_KERNEL);
	if (chrdev) {
		chrdev->mode = MINOR(inode->i_rdev);
		file->private_data = chrdev;
	}

	return 0;
}

static int kbdmouse_release(struct inode *inode, struct file *file)
{
	if (file->private_data)
		kfree(file->private_data);

	return 0;
}

static ssize_t kbdmouse_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	ssize_t	len = count;
	struct hid_chrdev *chrdev = file->private_data;
	void *p = chrdev->buf;
	struct hid_dev	*dev;
	struct usb_ep	*mouse_ep;
	u32 rVal;
	
	if (g_kbdmouse->config == 0)
		return 0;		

	if (chrdev->mode == 0) {	/* keyboard */
		dev = g_kbdmouse->kbd_ep->driver_data;
		 
		dev->write_aborted = 0;
		 
		// pend until last transfer completes
		rVal = wait_event_interruptible(dev->kbd_write_wait_q,
						dev->kbd_req->complete == NULL);
		if (rVal != 0)
			return (-ERESTARTSYS);
						
		// Return an error if USB is resetting
		if (dev->write_aborted) {
			printk(KERN_ERR "%s: kbd write aborted for"
						" device reset.\n",
						__FUNCTION__);
			return (-1);
		}
			
		if (copy_from_user(p, buf, count)) {
			return (-EFAULT);
		}
		
		kbd_start_ep(g_kbdmouse->kbd_ep, p, count, GFP_KERNEL);
	} else if (chrdev->mode == 1) {	/* mouse */
#ifndef CONFIG_USB_MOUSE_RELATIVE
		dev = g_kbdmouse->mouse_ep->driver_data;
		mouse_ep = g_kbdmouse->mouse_ep;
#else
		if (g_kbdmouse->mouse_mode == MOUSE_MODE_ABS) {
			dev = g_kbdmouse->mouse_ep->driver_data;
			mouse_ep = g_kbdmouse->mouse_ep;
		} else {
			dev = g_kbdmouse->mouse_ep_relative->driver_data;
			mouse_ep = g_kbdmouse->mouse_ep_relative;
		}
#endif
		
		dev->write_aborted = 0;
		
		// pend until last transfer completes
		rVal = wait_event_interruptible(dev->mouse_write_wait_q,
					dev->mouse_req->complete == NULL);
		if (rVal != 0)
			return (-ERESTARTSYS);
												
		// Return an error if USB is resetting
		if (dev->write_aborted) {
			printk(KERN_ERR "%s: mouse write aborted for"
						" device reset.\n",
						__FUNCTION__);
			return (-1);
		}
								
		if (copy_from_user(p, buf, count)) {
			return (-EFAULT);
		}
		
		mouse_start_ep(mouse_ep, p, count, GFP_KERNEL);
	}

	return len;
}

int kbdmouse_ioctl(struct inode *inode,
                struct file *filp,
                unsigned int cmd,
                unsigned long arg)
{
	struct kbdmse_status_data status_data[1];
	u32 rVal;
	
	switch (cmd) {
	case IOCTL_KBDMOUSE_GET_MOUSE_MODE:
		if (copy_to_user((void __user *) arg,
			&g_kbdmouse->mouse_mode, sizeof(g_kbdmouse->mouse_mode))) {
			return (-EFAULT);
		}
		break;
	case IOCTL_KBDMOUSE_SET_MOUSE_MODE:
#ifndef CONFIG_USB_MOUSE_RELATIVE
		if (g_kbdmouse->os_mouse_mode != arg) {
			printk(KERN_ALERT "%s: Switching mouse mode to %d\n",
					__FUNCTION__, (int)arg);
			g_kbdmouse->os_mouse_mode = arg;
			
			if (g_kbdmouse->os_mouse_mode == MOUSE_MODE_ABS) {
				fs_hid_function[5] =
					(struct usb_descriptor_header *) &hid_desc[1]; // ABS
				hs_hid_function[5] =
					(struct usb_descriptor_header *) &hid_desc[1]; // ABS
			} else {
				fs_hid_function[5] =
					(struct usb_descriptor_header *) &hid_desc[2]; // REL
				hs_hid_function[5] =
					(struct usb_descriptor_header *) &hid_desc[2]; // REL
			}
		
			// Reset USB
			usb_gadget_disconnect(g_kbdmouse->gadget);
			msleep(500);
			usb_gadget_connect(g_kbdmouse->gadget);
		}
#else 
		g_kbdmouse->os_mouse_mode = arg;

		if (g_kbdmouse->mouse_mode != MOUSE_MODE_BOOT)
			g_kbdmouse->mouse_mode    = arg;
#endif	
		break;
	case IOCTL_KBDMOUSE_STATUS:
		if (!access_ok(VERIFY_WRITE, (void *)arg, sizeof(struct kbdmse_status_data))) {
			return (-EFAULT);
		}

		if(g_kbdmouse->status.flags == 0) {
			// pend on status event
			rVal = wait_event_interruptible(g_kbdmouse->status_wait_q,
					g_kbdmouse->status.flags);
			if (rVal != 0)
				return (-ERESTARTSYS);
		}
	
		status_data->flags       = g_kbdmouse->status.flags;
		g_kbdmouse->status.flags = 0;
		status_data->kbd_status  = g_kbdmouse->status.kbd_status;

		if (copy_to_user((void*)arg, (void*)status_data, sizeof(struct kbdmse_status_data))) {
			return (-EFAULT);
		}

		break;

	case IOCTL_KBDMOUSE_CONNECT:
		 /* 
			arg = 1 -> Connect
			arg = 0 -> Disconnect
		*/
		printk(KERN_DEBUG "%s - IOCTL_KBDMOUSE_CONNECT -> arg=%ld n",__FUNCTION__,arg);
		if ( arg == 0 || arg == 1) 
			usb_gadget_disconnect(g_kbdmouse->gadget);
		if ( arg == 1 ) {
			msleep(500);
			usb_gadget_connect(g_kbdmouse->gadget);
		}

		break;
		
	default:
		printk(KERN_ERR "%s: Bad Command 0x%x\n", __FUNCTION__, cmd);
		return (-ENOTTY);		
	}
	
	return (0);
}

static struct file_operations kbdmouse_ops = {
	.owner =	THIS_MODULE,
	.open =		kbdmouse_open,
	.release =	kbdmouse_release,
	.write =	kbdmouse_write,
	.ioctl = 	kbdmouse_ioctl,
};

#ifdef CONFIG_USB_COMPOSITE
static int __init hid_bind (struct _usb_composite_dev *cdev)
#else
static int __init hid_bind (struct usb_gadget *gadget)
#endif
{
	struct hid_dev		*dev;
	struct usb_ep		*ep;
	int			rc = -EINVAL;
#ifdef CONFIG_USB_COMPOSITE
	struct usb_gadget	*gadget = cdev->gadget;
	int			interface_id;
#endif
	
	dev = kmalloc (sizeof *dev, GFP_KERNEL);
	if (!dev)
		return -ENOMEM;
	memset (dev, 0, sizeof *dev);
	
	dev->gadget = gadget;
	g_kbdmouse = dev;

	dev->ep0_recv = 0;
	
	/* Bulk-only drivers like this one SHOULD be able to
	 * autoconfigure on any sane usb controller driver,
	 * but there may also be important quirks to address.
	 */
#ifndef CONFIG_USB_COMPOSITE
	usb_ep_autoconfig_reset(gadget);
#endif

	ep = usb_ep_autoconfig (gadget, &fs_desc[0]);
	if (!ep) {
		printk (KERN_ERR "%s: can't autoconfigure KBD %s\n",
				__FUNCTION__, gadget->name);
autoconf_fail:
		printk (KERN_ERR "%s: can't autoconfigure on %s\n",
				__FUNCTION__, gadget->name);
		return -ENODEV;
	}
	EP_KBD_NAME = ep->name;
	ep->driver_data = ep;	/* claim */
	dev->kbd_ep = ep;
	
	// allocate keyboard request and buffer
	dev->kbd_req = alloc_ep_req (ep);
	if (!dev->kbd_req) {
		rc = -ENOMEM;
		goto out;
	}
	dev->kbd_req->complete = NULL;

	ep = usb_ep_autoconfig (gadget, &fs_desc[1]);
	if (!ep) {
		printk (KERN_ERR "%s: can't autoconfigure ABS %s\n",
				__FUNCTION__, gadget->name);
		goto autoconf_fail;
	}
	EP_MOUSE_NAME = ep->name;
	ep->driver_data = ep;	/* claim */
	dev->mouse_ep = ep;
	
	// allocate absolute mouse request and buffer
	dev->mouse_req = alloc_ep_req (ep);
	if (!dev->mouse_req) {
		rc = -ENOMEM;
		goto out;
	}
	dev->mouse_req->complete = NULL;

#ifdef CONFIG_USB_MOUSE_RELATIVE	
	ep = usb_ep_autoconfig (gadget, &fs_desc[2]);
	if (!ep) {
		printk (KERN_ERR "%s: can't autoconfigure REL %s\n",
				__FUNCTION__, gadget->name);
		goto autoconf_fail;
	}
	EP_MOUSE_NAME_REL = ep->name;
	ep->driver_data = ep;	/* claim */
	dev->mouse_ep_relative = ep;
	
	// relative mouse uses the same request and buffer that
	// absolute uses	
#endif

	/* ok, we made sense of the hardware ... */
	spin_lock_init (&dev->lock);
	
	// init wait queues
	init_waitqueue_head(&dev->status_wait_q);
	init_waitqueue_head(&dev->kbd_write_wait_q);
	init_waitqueue_head(&dev->mouse_write_wait_q);

#ifdef CONFIG_USB_COMPOSITE
	// Get function index
	function_index = usb_composite_function_index(cdev);
	if (function_index < 0) {
		printk(KERN_ERR "%s: failed getting function index\n", 
								__FUNCTION__);
		rc = -EINVAL;
		goto out;		
	}	
	set_composite_data(cdev, function_index, dev);
#else
	set_gadget_data (gadget, dev);
#endif

	/* preallocate control response and buffer */
	dev->req = usb_ep_alloc_request (gadget->ep0, GFP_KERNEL);
	if (!dev->req) {
		rc = -ENOMEM;
		goto out;
	}
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	dev->req->buf = usb_ep_alloc_buffer (gadget->ep0, USB_BUFSIZ,
				&dev->req->dma, GFP_KERNEL);
#else
	dev->req->buf = _usb_ep_alloc_buffer (gadget, gadget->ep0, USB_BUFSIZ,
				&dev->req->dma, GFP_KERNEL);
#endif
	if (!dev->req->buf) {
		rc = -ENOMEM;
		goto out;
	}

	dev->req->complete = hid_setup_complete;
	
#ifdef CONFIG_USB_GADGET_DUALSPEED
	/* and that all endpoints are dual-speed */
	hs_desc[1].bEndpointAddress =
				fs_desc[1].bEndpointAddress;
#ifdef CONFIG_USB_MOUSE_RELATIVE				
	hs_desc[2].bEndpointAddress =
				fs_desc[2].bEndpointAddress;
#endif				
	hs_desc[0].bEndpointAddress = fs_desc[0].bEndpointAddress;
#endif

#ifdef CONFIG_USB_COMPOSITE
	// Set String IDs
	if (set_string_ids(cdev) < 0) {
		printk(KERN_ERR "%s: failed setting string IDs\n", __FUNCTION__);
		rc = -EINVAL;
		goto out;		
	}
	
	// Set Interface Number for Keyboard
	interface_id = usb_composite_interface_id(cdev, &intf_info[0]);
	if (interface_id >= 0)
		intf_desc[0].bInterfaceNumber = interface_id;
	else {
		printk(KERN_ERR "%s: invalid KBD interface number %d\n",
						__FUNCTION__, interface_id);
		rc = -EINVAL;
		goto out;
	}
	
	// Set Interface Number for Mouse
	interface_id = usb_composite_interface_id(cdev, &intf_info[1]);
	if (interface_id >= 0)
		intf_desc[1].bInterfaceNumber = interface_id;
	else {
		printk(KERN_ERR "%s: invalid Mouse"
			" interface number %d\n", __FUNCTION__, interface_id);
		rc = -EINVAL;
		goto out;
	}

#ifdef CONFIG_USB_MOUSE_RELATIVE	
	// Set Interface Number for Relative Mouse
	interface_id = usb_composite_interface_id(cdev, &intf_info[2]);
	if (interface_id >= 0)
		intf_desc[2].bInterfaceNumber = interface_id;
	else {
		printk(KERN_ERR "%s: invalid REL Mouse"
			" interface number %d\n", __FUNCTION__, interface_id);
		rc = -EINVAL;
		goto out;
	}
#endif
#else
	// Set device descriptor idVendor, idProduct, bcdDevice
	device_desc.idVendor  = cpu_to_le16(mod_data.idVendor);
	device_desc.idProduct = cpu_to_le16(mod_data.idProduct);
	device_desc.bcdDevice = cpu_to_le16(mod_data.bcdDevice);

	// Set ep0 max packet size
	device_desc.bMaxPacketSize0 = gadget->ep0->maxpacket;

#ifdef CONFIG_USB_GADGET_DUALSPEED
	/* assume ep0 uses the same value for both speeds ... */
	dev_qualifier.bMaxPacketSize0 = device_desc.bMaxPacketSize0;
#endif
#endif

	// initialize mouse mode
	dev->os_mouse_mode = MOUSE_MODE_ABS;
	dev->mouse_mode = MOUSE_MODE_BOOT; 
	
	// Get major number
	rc = alloc_chrdev_region(&dev->chrdev, 0, 3, DRIVER_NAME); 
    	if (rc < 0) {
		printk(KERN_ERR "%s: Failed to obtain major number\n", 
							__FUNCTION__);
		goto out;
	}
	
	printk(KERN_ALERT "%s: Using Major Number %d\n", __FUNCTION__,
							MAJOR(dev->chrdev));
	
	// Register char device					
	cdev_init(&dev->cdev, &kbdmouse_ops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops   = &kbdmouse_ops;
	rc = cdev_add(&dev->cdev, dev->chrdev, 3); 
	if (rc < 0) {
		printk(KERN_ERR "%s: Failed to register char device\n", 
								__FUNCTION__);
		goto out;
	}		
	
	init_timer (&dev->resume);
	dev->resume.function = hid_autoresume;
	dev->resume.data = (unsigned long) dev;

	gadget->ep0->driver_data = dev;

	printk(KERN_ALERT "%s: using %s, Keyboard %s Mouse %s\n",
			__FUNCTION__, gadget->name, EP_KBD_NAME, EP_MOUSE_NAME);
		
#ifdef CONFIG_USB_MOUSE_RELATIVE
	printk(KERN_ALERT "%s: Mouse REL %s\n", __FUNCTION__, EP_MOUSE_NAME_REL);
#endif

#ifndef CONFIG_USB_COMPOSITE
	usb_gadget_connect(gadget);
#endif
		
	return 0;

out:
#ifdef CONFIG_USB_COMPOSITE
	hid_unbind (cdev);
#else
	hid_unbind (gadget);
#endif
	return rc;
}

/*-------------------------------------------------------------------------*/

#ifdef CONFIG_USB_COMPOSITE
static void hid_suspend (struct _usb_composite_dev *cdev)
#else
static void hid_suspend (struct usb_gadget *gadget)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
	struct usb_gadget	*gadget = cdev->gadget;
#else
	struct hid_dev		*dev = get_gadget_data (gadget);
#endif

	if (gadget->speed == USB_SPEED_UNKNOWN)
		return;

	if (autoresume) {
		mod_timer (&dev->resume, jiffies + (HZ * autoresume));
		printk(KERN_DEBUG "%s: suspend, wakeup in %d seconds\n",
						__FUNCTION__, autoresume);
	} else
		printk(KERN_DEBUG "%s: suspend\n", __FUNCTION__);
}

#ifdef CONFIG_USB_COMPOSITE
static void hid_resume (struct _usb_composite_dev *cdev)
#else
static void hid_resume (struct usb_gadget *gadget)
#endif
{
#ifdef CONFIG_USB_COMPOSITE
	struct hid_dev		*dev = get_composite_data(cdev, function_index);
#else
	struct hid_dev		*dev = get_gadget_data (gadget);
#endif

	printk(KERN_DEBUG "%s: resume\n", __FUNCTION__);
	del_timer (&dev->resume);
}


/*-------------------------------------------------------------------------*/
#ifdef CONFIG_USB_COMPOSITE
static struct _usb_function hid_driver = {
	.name             = (char *) shortname,
	.controller_name  = controller,
	.strings          = stringTable,
	.descriptors      = fs_hid_function,
#ifdef CONFIG_USB_GADGET_DUALSPEED	
	.hs_descriptors   = hs_hid_function,
#else
	.hs_descriptors   = NULL,
#endif
	.intf_info_table  = intf_info_table,
	.bind             = hid_bind,
	.unbind           = hid_unbind,
	.disconnect       = _hid_disconnect,
	.setup            = hid_setup,
	.suspend          = hid_suspend,
	.resume           = hid_resume,
	
	.driver           = {
		.name      = (char *) shortname,
		.owner     = THIS_MODULE,
		// .release = ...
		// .suspend = ...
		// .resume = ...
	},	
};
#else
static struct usb_gadget_driver hid_driver = {
#ifdef CONFIG_USB_GADGET_DUALSPEED
	.speed		= USB_SPEED_HIGH,
#else
	.speed		= USB_SPEED_FULL,
#endif
	.function	= (char *) shortname,
	.bind		= hid_bind,
	.unbind		= hid_unbind,
	.disconnect	= _hid_disconnect,
	.setup		= hid_setup,
	.suspend	= hid_suspend,
	.resume		= hid_resume,

	.driver 	= {
		.name		= (char *) shortname,
		.owner     = THIS_MODULE,
		// .release = ...
		// .suspend = ...
		// .resume = ...
	},
};
#endif

MODULE_AUTHOR ("Avocent Corp.");
MODULE_LICENSE ("GPL v2");

#ifdef CONFIG_USB_GADGET_LS_CONTROLLER
extern int _usb_gadget_register_ls_driver(struct usb_gadget_driver *driver);
extern int _usb_gadget_unregister_ls_driver(struct usb_gadget_driver *driver);
#endif

static int __init hid_init (void)
{
	/* a real value would likely come through some id prom
	 * or module option.  this one takes at least two packets.
	 */
	printk(KERN_ALERT "%s: Desc: %s\n", __FUNCTION__, DRIVER_DESC);
	printk(KERN_ALERT "%s: Name: %s\n", __FUNCTION__, DRIVER_NAME);
	printk(KERN_ALERT "%s: Version: %s\n", __FUNCTION__, DRIVER_VERSION);
	
	strncpy(controller, mod_data.controller, CONTROLLER_SIZE);	

#ifdef CONFIG_USB_COMPOSITE  	
	return _usb_composite_register(&hid_driver);
#else
  #ifndef CONFIG_USB_GADGET_DUAL_CONTROLLERS
    #ifdef CONFIG_USB_GADGET_LS_CONTROLLER
		return _usb_gadget_register_ls_driver(&hid_driver);
	#else
		return usb_gadget_register_driver(&hid_driver);
	#endif
  #endif
#endif 
}
module_init (hid_init);

static void __exit hid_cleanup (void)
{
	cdev_del(&g_kbdmouse->cdev);
	unregister_chrdev_region(g_kbdmouse->chrdev, 3);
	
#ifdef CONFIG_USB_COMPOSITE
	_usb_composite_unregister(&hid_driver);
#else
  #ifndef CONFIG_USB_GADGET_DUAL_CONTROLLERS
	usb_gadget_disconnect(g_kbdmouse->gadget);
    #ifdef CONFIG_USB_GADGET_LS_CONTROLLER
		_usb_gadget_unregister_ls_driver(&hid_driver);
	#else
		usb_gadget_unregister_driver(&hid_driver);
	#endif
  #endif
#endif	
}
module_exit (hid_cleanup);

