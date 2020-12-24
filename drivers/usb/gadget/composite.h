/*
 * Header file to define Function Drivers for use with Composite Gadget
 * Drivers
 * 
 * This driver was influenced by David Brownell's ideas from
 * http://www.mail-archive.com/linux-usb-devel@lists.sourceforge.net/msg50046.html
 */

/*
 * Copyright (C) 2006 Avocent Corp.
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
 */

#ifndef        __LINUX_USB_COMPOSITE_H
#define        __LINUX_USB_COMPOSITE_H

#ifdef __KERNEL__

#define	DMA_ADDR_INVALID	(~(dma_addr_t)0)

/* Support for composite gadgets, built from distinct function drivers.
 * Example:  one offering both network and mass storage functionality.
 */

struct usb_composite_dev;
	

/**
 * struct usb_function - describes one function of a composite device
 * @name: for diagnostics, identifies the function
 * @strings: tables of strings, keyed by identifiers assigned during bind()
 *     and language IDs provided in control requests
 * @descriptors: table of low/full speed descriptors, using interface and
 *     string identifiers assigned during bind()
 * @hs_descriptors: table of high speed descriptors, using interface and
 *     string identifiers assigned during bind(); or null
 * @num_interfaces: the number of interfaces in this function
 * @function_index: index into usb_composite_dev.function_data[]
 * @bind: before the gadget can register, all of its functions bind() to the
 *     available resources including identifiers for strings, interfaces,
 *     and endpoints
 * @unbind: before the gadget can unregister, all of its functions unbind()
 *     the resources they have allocated.
 * @setup: used for SET_INTERFACE, SET_CONFIGURATION, and interface-specific
 *     control requests
 * @disconnect: used to notify functions when the host has disconnected
 * @suspend: notifies functions when the host stops sending USB traffic
 * @disconnect: notifies functions when the host reactivates the USB link
 *
 * A single USB function uses one or more interfaces, and supports dual speed
 * operation on appropriate hardware.
 */
 
struct usb_interface_info {	
	u8					enabled;
	struct usb_interface_descriptor		*intf_desc;
};

struct usb_function {
	const char                              *name;
	const char                              *controller_name;
	const struct usb_gadget_strings         **strings;
	const struct usb_descriptor_header      **descriptors;
	const struct usb_descriptor_header      **hs_descriptors;
	struct usb_interface_info		**intf_info_table;

	int    (*bind)(struct usb_composite_dev *);
	void   (*unbind)(struct usb_composite_dev *);
	int    (*setup)(struct usb_composite_dev *,
				const struct usb_ctrlrequest *);
	void   (*disconnect)(struct usb_composite_dev *);
	void   (*suspend)(struct usb_composite_dev *);
	void   (*resume)(struct usb_composite_dev *);
		
	struct device_driver driver;
};


#define	MAX_COMPOSITE_INTERFACES        8       /* max 16 */
#define	MAX_NUM_FUNCTIONS               4
#define MAX_NUM_CONTROLLERS             2

/**
 * struct usb_composite_device - represents one composite usb gadget
 * @gadget: read-only, abstracts the gadget's usb peripheral controller
 * @req: used for control responses; buffer is pre-allocated
 * @dev_desc: device descriptor
 * @config: configuration descriptor; if the device is not configured,
 *     its bConfigurationValue is zero and other fields are ignored
 * @strings: table of strings
 */
struct usb_composite_dev {
	spinlock_t                              lock;
	struct usb_gadget                       *gadget;
	int 	(*gadget_register) (struct usb_gadget_driver *driver);
	int 	(*gadget_unregister) (struct usb_gadget_driver *driver);	
	struct usb_request                      *req;
	struct usb_device_descriptor            *dev_desc;
	struct usb_config_descriptor            *config;
	const struct usb_gadget_strings         **strings;		

	/* INTERNALS -- not for function drivers */
	u8                   next_string_id;	
	u8                   next_interface_id;
	u8                   num_functions;
	u8                   next_function_index;
	u8                   cdev_index;
	struct usb_function  *current_bind;
	struct usb_function  *function[MAX_NUM_FUNCTIONS];
	void                 *function_data[MAX_NUM_FUNCTIONS];
	struct usb_qualifier_descriptor qual;
	/* The CBI specification limits the serial string to 12 uppercase
	 * hexadecimal characters. */
};

struct usb_composite_devices {
	u8                       next_cdev_index;
	u8                       num_cdevs;
	struct usb_composite_dev *cdevs;
};

int usb_composite_register(struct usb_function *);
int usb_composite_unregister(struct usb_function *);

void set_composite_data(struct usb_composite_dev *cdev, u8 index, void *data);
void *get_composite_data(struct usb_composite_dev *cdev, u8 index);	

int usb_composite_reconfigure_interface_ids(struct usb_composite_dev *cdev);
int usb_composite_string_id(struct usb_composite_dev *cdev);
int usb_composite_interface_id(struct usb_composite_dev *cdev,
				struct usb_interface_info *intf);
int usb_composite_function_index(struct usb_composite_dev *cdev);

void * _usb_ep_alloc_buffer(struct usb_gadget *gadget,
				   struct usb_ep *_ep,
                                   unsigned bytes,
                                   dma_addr_t *dma,
                                   gfp_t gfp_flags);
                                   
void _usb_ep_free_buffer(struct usb_gadget *gadget,
				struct usb_ep *_ep,
                                void *buf,
                                dma_addr_t dma,
                                unsigned bytes);

#endif  /* __KERNEL__ */

#endif /* __LINUX_USB_COMPOSITE_H */
