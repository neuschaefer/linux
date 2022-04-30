/******************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Shiraz Hashim                   */
/*   shiraz.hashim@st.com                                                     */
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
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                */
/******************************************************************************/




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
#include <linux/wait.h>
#include <linux/proc_fs.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>

#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>
#include <linux/string.h>
#include <linux/ctype.h>

#include <linux/usb_ch9.h>
#include <linux/usb_gadget.h>

#include "gadget_chips.h"
#include "spr_mux.h"




/* ---------------------------------------------------------------- */
/*		module prameters				    */
/* ---------------------------------------------------------------- */

static unsigned interfaces=MUX_NUM_INTF;
static unsigned short vendor=MUX_VENDOR_ID;
static unsigned short product=MUX_PRODUCT_ID;
static ushort __initdata bcdDevice;
static char *__initdata iManufacturer;
static char *__initdata iProduct;
static char *__initdata iSerialNum;

module_param (interfaces, uint, S_IRUGO);
module_param (vendor, ushort, S_IRUGO);
module_param (product, ushort, S_IRUGO);

module_param(bcdDevice, ushort, S_IRUGO);
MODULE_PARM_DESC(bcdDevice, "USB Device version (BCD)");

module_param(iManufacturer, charp, S_IRUGO);
MODULE_PARM_DESC(iManufacturer, "USB Manufacturer string");

module_param(iProduct, charp, S_IRUGO);
MODULE_PARM_DESC(iProduct, "USB Product string");

module_param(iSerialNum, charp, S_IRUGO);
MODULE_PARM_DESC(iSerialNum, "1");

/* ---------------------------------------------------------------- */

static char				manufacturer [50] = MUX_MANUFACTURER;
static char				product_desc [40] = MUX_PRODUCT_NAME;
static char				serial_num [40]   = MUX_SERIAL_NUMBER;



/* ---------------------------------------------------------------- */
/*		MUX specific definitions			    */
/* ---------------------------------------------------------------- */

/* defines for MUX device */

/* various string index */
#define MUX_MANUFACTURER_STR_ID 	1
#define MUX_PRODUCT_STR_ID		2
#define MUX_SERIAL_STR_ID		3


/* ---------------------------------------------------------------- */
/*		MUX specific structures				    */
/* ---------------------------------------------------------------- */

/* the device structure holds info for the USB device */
struct mux_dev {
	spinlock_t  	dev_lock;       /* lock for set/reset config */
	//raw_spinlock_t  	dev_lock;       /* lock for set/reset config */

	int         	dev_config;     /* configuration number */
	int 		intf_cntr;	/* to track regitered interfaces */
	int 		hs_desc_cntr;	/* to track high speed descriptor count */
	int	 	fs_desc_cntr;	/* to track full speed descriptor count */
	int 		suspended;

	struct 		usb_gadget       *dev_gadget;    /* gadget device pointer */
	struct 		usb_request      *dev_ctrl_req;  /* control request */
	struct 		tMUX_CompositeGadgetDriver *gadget_drivers[MUX_MAX_INTF];
};


/* ---------------------------------------------------------------- */




/* ---------------------------------------------------------------- */
/*		Global and static Variables		    	    */
/* ---------------------------------------------------------------- */


static struct mux_dev mux_device;

/* Device Descriptor */
static struct usb_device_descriptor mux_device_desc = {
	.bLength 			= USB_DT_DEVICE_SIZE,
	.bDescriptorType 		= USB_DT_DEVICE,
	.bcdUSB 			= __constant_cpu_to_le16 (0x0200),
	.bDeviceClass 			= USB_CLASS_PER_INTERFACE,
	.bDeviceSubClass	 	= 0,
	.bDeviceProtocol 		= 0,
	.idVendor 			= __constant_cpu_to_le16 (MUX_VENDOR_ID),
	.idProduct 			= __constant_cpu_to_le16 (MUX_PRODUCT_ID),
	.iManufacturer 			= MUX_MANUFACTURER_STR_ID,
	.iProduct 			= MUX_PRODUCT_STR_ID,
	.iSerialNumber 			= MUX_SERIAL_STR_ID,
	.bNumConfigurations 		= MUX_NUM_CONFIGS,
};


/* Config Descriptor */

static struct usb_config_descriptor mux_config_desc = {
	.bLength 			= USB_DT_CONFIG_SIZE,
	.bDescriptorType 		= USB_DT_CONFIG,
	/* .wTotalLength computed dynamically */
	.bNumInterfaces 		= MUX_NUM_INTF,
	.bConfigurationValue 		= MUX_CONFIG_ID,
	.iConfiguration 		= 0,
	.bmAttributes 			= USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER,
	.bMaxPower 			= 1,
};

static struct usb_qualifier_descriptor
dev_qualifier = {
	.bLength 		=      sizeof dev_qualifier,
	.bDescriptorType 	=      USB_DT_DEVICE_QUALIFIER,
	.bcdUSB 		=      __constant_cpu_to_le16 (0x0200),
	.bDeviceClass 		=      USB_CLASS_VENDOR_SPEC,
	.bNumConfigurations 	=      1,
};



static struct usb_string mux_strings[] = {
	  {MUX_MANUFACTURER_STR_ID, manufacturer},
	  {MUX_PRODUCT_STR_ID, product_desc},
	  {MUX_SERIAL_STR_ID, serial_num},
	  {}				/* end of list */
};

static struct usb_gadget_strings mux_string_table = {
	.language 	= 0x0409,		/* en-us */
	.strings 	= mux_strings,
};


static struct usb_otg_descriptor
otg_descriptor = {
	.bLength 		= sizeof otg_descriptor,
	.bDescriptorType 	= USB_DT_OTG,

	.bmAttributes 		= USB_OTG_SRP,
};


static const struct usb_descriptor_header *mux_fs_function[MUX_MAX_DESC] = {
	(struct usb_descriptor_header *) &otg_descriptor,NULL,
	/* rest added in registration */
};



#ifdef CONFIG_USB_GADGET_DUALSPEED
static const struct usb_descriptor_header *mux_hs_function[MUX_MAX_DESC] = {
	(struct usb_descriptor_header *) &otg_descriptor,NULL,
	/* rest added in registration */
};
#endif    
/* ---------------------------------------------------------------- */




/* ---------------------------------------------------------------- */
/*		Internal functions				    */
/* 		gadget driver framework APIs			    */
/* ---------------------------------------------------------------- */
static int 		SPEAr_MUX_Bind(struct usb_gadget *gadget);
static void 		SPEAr_MUX_Unbind(struct usb_gadget *gadget);
static int 		SPEAr_MUX_Setup(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl);
static int 		SPEAr_MUX_SetupStandard(struct usb_gadget *gadget, const struct usb_ctrlrequest *ctrl);
static void 		SPEAr_MUX_SetupComplete(struct usb_ep *ep, struct usb_request *req);
static void 		SPEAr_MUX_Disconnect(struct usb_gadget *gadget);
static int 		SPEAr_MUX_SetConfig(struct mux_dev *dev, unsigned config);
static void 		SPEAr_MUX_ResetConfig(struct mux_dev *dev);
static int 		SPEAr_MUX_BuildConfigBuf(u8 *buf, enum usb_device_speed speed, u8 type, unsigned int index, int is_otg);
static struct 		usb_request *SPEAr_MUX_AllocReq(struct usb_ep *ep, unsigned int len, int kmalloc_flags);
static void 		SPEAr_MUX_FreeReq(struct usb_ep *ep, struct usb_request *req);



/****************************************************************************/
/* NAME:   SPEAr_MUX_Bind						    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Called when MUX binds to UDC driver                      */
/*      							            */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           gadget: usb_gadget structure filled by UDC driver        */
/*                         with ops      				    */
/*                                          				    */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN: 	0 on success						    */
/*		err otherwise 					  	    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY: 								    */
/****************************************************************************/

static int SPEAr_MUX_Bind(struct usb_gadget *gadget)
{
	int ret;
	struct mux_dev *dev;
	int i;


	

	dev = &mux_device;

	usb_ep_autoconfig_reset(gadget);

	for( i=0; dev->gadget_drivers[i] != NULL && i < interfaces; i++)
	  {
		MUX_DEBUG("\n===============driver:%d===============\n", dev->gadget_drivers[i]->interface_no);
		if(dev->gadget_drivers[i]->bind == NULL )
		  {
			printk(KERN_ERR "%s bind null\n",__FUNCTION__);
			goto autoconf_fail;
		  }
		if( (ret = dev->gadget_drivers[i]->bind(gadget)) != 0 ) //let each interface bind itself
		  {
			printk(KERN_WARNING "%s bind failed\n",__FUNCTION__);

			goto autoconf_fail;
		  }

		dev->gadget_drivers[i]->driver = get_gadget_data(gadget); //save individual i/f device data

	  }

	if (gadget_is_net2280(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0001);
	  } 
	else if (gadget_is_pxa(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0002);
	  } 
	else if (gadget_is_sh(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0003);
		/* sh doesn't support multiple interfaces or configs */
	  } 
	else if (gadget_is_sa1100(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0004);
		/* sa1100 doesn't support necessary endpoints */
	  } 
	else if (gadget_is_goku(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0005);
	  } 
	else if (gadget_is_mq11xx(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0006);
	  } 
	else if (gadget_is_omap(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0007);
	  } 
	else if (gadget_is_lh7a40x(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0008);
	  } 
	else if (gadget_is_n9604(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0009);
	  } 
	else if (gadget_is_pxa27x(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0011);
	  } 
	else if (gadget_is_spearplus(gadget)) 
	  {
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0012);
	  } 
	else 
	  {
		printk(KERN_WARNING "mux_bind: controller '%s' not recognized\n",
				gadget->name);
		/* unrecognized, but safe unless bulk is REALLY quirky */
		mux_device_desc.bcdDevice =
			__constant_cpu_to_le16(MUX_VERSION_NUM|0x0099);
	  }


	dev->dev_gadget = gadget;
	spin_lock_init(&dev->dev_lock);
	set_gadget_data(gadget, dev);  //assign mux'sown device data


	/* preallocate control response and buffer */
	dev->dev_ctrl_req = SPEAr_MUX_AllocReq(gadget->ep0, MUX_MAX_DESC_LEN, GFP_KERNEL);
	if (dev->dev_ctrl_req == NULL) 
	  {
		SPEAr_MUX_Unbind(gadget);
		return -ENOMEM;
	  }

	dev->dev_ctrl_req->complete = SPEAr_MUX_SetupComplete;
	mux_device_desc.bMaxPacketSize0 = gadget->ep0->maxpacket;

#ifdef CONFIG_USB_GADGET_DUALSPEED
	/* assume ep0 uses the same value for both speeds ... */
	dev_qualifier.bMaxPacketSize0 = mux_device_desc.bMaxPacketSize0;
#endif

	usb_gadget_set_selfpowered(gadget);

	gadget->ep0->driver_data = dev;

	printk(KERN_INFO "mux_bind: %s %s bound\n", MUX_LONG_NAME, MUX_VERSION_STR);

	return 0;

autoconf_fail:
	printk("mux_bind: cannot autoconfigure on %s\n", gadget->name);
	return -ENODEV;
}


/****************************************************************************/
/* NAME:         SPEAr_MUX_Unbind                                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when MUX unregisters from UDC driver                */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :         gadget structure filled by UDC driver                      */
/*                                                                          */
/*                                                                          */
/* OUT :        None                                                        */
/*                                                                          */
/* RETURN:      None                                                        */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static void SPEAr_MUX_Unbind(struct usb_gadget *gadget)
{
	struct mux_dev *dev = get_gadget_data(gadget);
	int i;

	
	/* Call each interface bind */
	for( i=0; dev->gadget_drivers[i] != NULL && i < interfaces; i++)
	  {
		if(dev->gadget_drivers[i]->unbind == NULL )
		  {
			/* Failure */
			printk(KERN_WARNING "%s unbind null\n",__FUNCTION__);
		  }
		else
		  {
			set_gadget_data(gadget, dev->gadget_drivers[i]->driver); 

			dev->gadget_drivers[i]->unbind(gadget); //let each interface unbind itself
		  }

	  }

	/* Restore MUX data */
	set_gadget_data(gadget, dev); 

	if (dev != NULL) 
	  {
		if (dev->dev_ctrl_req != NULL) 
		  {
			SPEAr_MUX_FreeReq(gadget->ep0, dev->dev_ctrl_req);
			dev->dev_ctrl_req = NULL;
		  }
		set_gadget_data(gadget, NULL);
	  }

	printk(KERN_INFO "mux_unbind: %s %s unbound\n", MUX_LONG_NAME, MUX_VERSION_STR);
}


/****************************************************************************/
/* NAME:      SPEAr_MUX_Setup                                               */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Invoked when control packet received on endpoint 0         */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :         gadget: gadget structure                                   */
/*               ctrl:   usb ctrl request                                   */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:       int:                                                       */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static int SPEAr_MUX_Setup(struct usb_gadget *gadget,
		const struct usb_ctrlrequest *ctrl)
{
	int ret = -EOPNOTSUPP;
	struct mux_dev *dev = get_gadget_data(gadget);
	struct usb_request *req = dev->dev_ctrl_req;
	u16 wIndex;

	
	MUX_DEBUG("\n============================\n");
	MUX_DEBUG("mux_setup: request: btype=%02x, brequest=%02x, wvalue=%04x, windex=%04x, wlength=%d\n",
			ctrl->bRequestType, ctrl->bRequest, ctrl->wValue,
			ctrl->wIndex, ctrl->wLength);

	MUX_DEBUG("============================\n");


	switch (ctrl->bRequestType & USB_TYPE_MASK) 
	  {
		case USB_TYPE_STANDARD:
			ret = SPEAr_MUX_SetupStandard(gadget,ctrl);
			break;

		case USB_TYPE_CLASS:
			/*call appropriate interface's function */

			if( ctrl->wIndex >= MUX_MAX_INTF )
			  {
				wIndex = ctrl->wIndex >> 8;
			  }
			else
			  {
				wIndex = ctrl->wIndex ;
			  }
			MUX_DEBUG("Class spec req: modified windex= 0x%x\n", wIndex );


			if ( (ctrl->bRequestType & USB_RECIP_MASK) != USB_RECIP_INTERFACE
					|| wIndex >= MUX_MAX_INTF)
				break;

			if( dev->gadget_drivers[wIndex] == NULL || dev->gadget_drivers[wIndex]->setup == NULL )
				break;

			spin_lock(&dev->dev_lock);

			set_gadget_data(gadget, dev->gadget_drivers[wIndex]->driver);
			MUX_DEBUG("Calling interface=%d setup\n", wIndex);
			ret = dev->gadget_drivers[wIndex]->setup(gadget, ctrl, req);
			set_gadget_data(gadget, dev);

			spin_unlock(&dev->dev_lock);
			break;

			/***********
			  Add vendor specific requests here
case :
break;


			 **********/
		default:
			printk(KERN_ERR "mux_setup: unknown request, type=%02x, request=%02x, value=%04x, index=%04x, length=%d\n",
					ctrl->bRequestType, ctrl->bRequest, ctrl->wValue,
					ctrl->wIndex, ctrl->wLength);
			break;
	  }

	/* respond with data transfer before status phase? */
	if (ret >= 0) 
	  {
		req->length = ret;
		req->zero = ret < ctrl->wLength
			&& (ret % gadget->ep0->maxpacket) == 0;
		ret = usb_ep_queue(gadget->ep0, req, GFP_ATOMIC);
		if (ret < 0) 
		  {
			printk(KERN_ERR "mux_setup: cannot queue response, ret=%d\n",
					ret);
			req->status = 0;
			SPEAr_MUX_SetupComplete(gadget->ep0, req);
		  }
	  }

	/* device either stalls (ret < 0) or reports success */
	return ret;
}



/****************************************************************************/
/* NAME:             SPEAr_MUX_SetupStandard                                */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      handles standard USB requests                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :             gadget: gadget structure                               */
/*                   ctrl:   usb control request                            */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:          int:                                                    */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static int SPEAr_MUX_SetupStandard(struct usb_gadget *gadget,
		const struct usb_ctrlrequest *ctrl)
{
	int ret = -EOPNOTSUPP;
	struct mux_dev *dev = get_gadget_data(gadget);
	struct usb_request *req = dev->dev_ctrl_req;

	
	switch (ctrl->bRequest) 
	  {
		case USB_REQ_GET_DESCRIPTOR:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;

			switch (ctrl->wValue >> 8) 
			  {
				case USB_DT_DEVICE:
					ret = min(ctrl->wLength,
							(u16)sizeof(struct usb_device_descriptor));
					memcpy(req->buf, &mux_device_desc, ret);
					break;

#ifdef CONFIG_USB_GADGET_DUALSPEED
				case USB_DT_DEVICE_QUALIFIER:
					if (!gadget->is_dualspeed)
						break;
					ret= min (ctrl->wLength, (u16) sizeof dev_qualifier);
					memcpy (req->buf, &dev_qualifier, ret);
					break;

				case USB_DT_OTHER_SPEED_CONFIG:
					if (!gadget->is_dualspeed)
						break;
					// FALLTHROUGH
#endif /* CONFIG_USB_GADGET_DUALSPEED */


				case USB_DT_CONFIG:
					ret = SPEAr_MUX_BuildConfigBuf(req->buf, gadget->speed,
							ctrl->wValue >> 8, ctrl->wValue & 0xff,
							gadget->is_otg);
					if (ret >= 0)
						ret = min(ctrl->wLength, (u16)ret);
					break;

				case USB_DT_STRING:
					/* wIndex == language code. */

					ret = usb_gadget_get_string(&mux_string_table,
							ctrl->wValue & 0xff, req->buf);
					if (ret >= 0)
						ret = min(ctrl->wLength, (u16)ret);

					break;
			  }
			break;

		case USB_REQ_SET_CONFIGURATION:
			if (ctrl->bRequestType != 0)
				break;
			ret = SPEAr_MUX_SetConfig(dev, ctrl->wValue);
			break;

		case USB_REQ_GET_CONFIGURATION:
			if (ctrl->bRequestType != USB_DIR_IN)
				break;
			*(u8 *)req->buf = dev->dev_config;
			ret = min(ctrl->wLength, (u16)1);
			break;

		case USB_REQ_SET_INTERFACE:
			if (ctrl->bRequestType != USB_RECIP_INTERFACE)
				break;

			if( dev->gadget_drivers[ctrl->wIndex] == NULL || dev->gadget_drivers[ctrl->wIndex]->setup == NULL )
				break;

			/* resets interface configuration, forgets about
			 * previous transaction state (queued bufs, etc)
			 * and re-inits endpoint state (toggle etc)
			 * no response queued, just zero status == success.
			 * if we had more than one interface we couldn't
			 * use this "reset the config" shortcut.
			 */
			spin_lock(&dev->dev_lock);

			set_gadget_data(gadget, dev->gadget_drivers[ctrl->wIndex]->driver);
			ret = dev->gadget_drivers[ctrl->wIndex]->setup(gadget, ctrl, req);
			set_gadget_data(gadget, dev);

			spin_unlock(&dev->dev_lock);

			break;


		case USB_REQ_GET_INTERFACE:
			/* no alternate interface settings */
			spin_lock(&dev->dev_lock);

			if( dev->gadget_drivers[ctrl->wIndex] == NULL || dev->gadget_drivers[ctrl->wIndex]->setup == NULL )
				break;
			set_gadget_data(gadget, dev->gadget_drivers[ctrl->wIndex]->driver);
			ret = dev->gadget_drivers[ctrl->wIndex]->setup(gadget, ctrl, req);
			set_gadget_data(gadget, dev);

			spin_unlock(&dev->dev_lock);

			break;

		default:
			printk(KERN_ERR "mux_setup: unknown standard request, type=%02x, request=%02x, value=%04x, index=%04x, length=%d\n",
					ctrl->bRequestType, ctrl->bRequest, ctrl->wValue,
					ctrl->wIndex, ctrl->wLength);
			break;
	  }

	return ret;
}

/****************************************************************************/
/* NAME:      SPEAr_MUX_SetupComplete                                       */
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
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static void SPEAr_MUX_SetupComplete(struct usb_ep *ep, struct usb_request *req)
{
	
	if (req->status || req->actual != req->length) 
	  {
		MUX_DEBUG("mux_setup_complete: status error, status=%d, actual=%d, length=%d\n",
				req->status, req->actual, req->length);
	  }
}


/****************************************************************************/
/* NAME:      SPEAr_MUX_Disconnect                                          */
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
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static void SPEAr_MUX_Disconnect(struct usb_gadget *gadget)
{
	struct mux_dev *dev = get_gadget_data(gadget);
	

	SPEAr_MUX_ResetConfig(dev);

	printk(KERN_INFO "mux_disconnect: %s disconnected\n", MUX_LONG_NAME);
}



/****************************************************************************/
/* NAME:      SPEAr_MUX_SetConfig                                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     set specific(config) configuration i.e. enable endps    */
/*                  as per config                                           */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: device structure                                    */
/*                 config: configuration number                             */
/*                                                                          */
/* OUT :                                       				    */
/*                                                                          */
/* RETURN:          int: 0 if success , error otherwise                     */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static int SPEAr_MUX_SetConfig(struct mux_dev *dev, unsigned config)
{
	int i;
	int result;
	/* Call set_config of each registered interface */
	

	if (config == dev->dev_config)
		return 0;

	SPEAr_MUX_ResetConfig (dev);

	spin_lock(&dev->dev_lock);

	for( i=0; dev->gadget_drivers[i] != NULL && i < interfaces; i++)
	  {
		if(dev->gadget_drivers[i]->set_config == NULL )
		  {
			/* Failure */
			printk(KERN_WARNING "%s setconfig null\n",__FUNCTION__);
		  }
		else
		  {

			//let each interface call  its config
			result &= dev->gadget_drivers[i]->set_config
				(dev->gadget_drivers[i]->driver, config); 
		  }
	  }

	if (!result )
		result = -ENODEV;

	dev->dev_config = config;

	spin_unlock(&dev->dev_lock);

	return 0;


}



/****************************************************************************/
/* NAME:      SPEAr_MUX_ResetConfig                                         */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Reset configuration i.e. disable all allocated endpoints */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           dev: Device structure                                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void SPEAr_MUX_ResetConfig(struct mux_dev *dev)
{
	int i;
	/* call disconnect of each interface */

	
	spin_lock(&dev->dev_lock);
	for( i=0; dev->gadget_drivers[i] != NULL  && i < interfaces; i++)
	  {
		if(dev->gadget_drivers[i]->reset_config == NULL )
		  {
			/* Failure */
			printk(KERN_WARNING "%s reset_config null\n",__FUNCTION__);
			goto fail;
		  }
		//let each interface reset  its config
		dev->gadget_drivers[i]->reset_config(dev->gadget_drivers[i]->driver); 
	  }

fail:
	dev->dev_config = 0;

	spin_unlock(&dev->dev_lock);
}



/****************************************************************************/
/* NAME:      SPEAr_MUX_BuildConfigBuf                                      */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    API to build configuration descriptor for MUX            */
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

static int SPEAr_MUX_BuildConfigBuf(u8 *buf, enum usb_device_speed speed,
		u8 type, unsigned int index, int is_otg)
{
	int len;
	int hs;
	struct usb_config_descriptor *config_desc;
	const struct usb_descriptor_header **function;

	
	if (index >= mux_device_desc.bNumConfigurations)
		return -EINVAL;


	config_desc = &mux_config_desc;


	//chose a function
#ifdef CONFIG_USB_GADGET_DUALSPEED
	hs = (speed == USB_SPEED_HIGH);

	if (type == USB_DT_OTHER_SPEED_CONFIG)
		hs = !hs;

	if (hs) 
	  {
		function = mux_hs_function;
	  } 
	else 
	  {
		function = mux_fs_function;
	  }
#else
	function = mux_fs_function;

#endif

	if (!is_otg)
	  {
		function++;
	  }

	len = usb_gadget_config_buf(config_desc, buf, MUX_MAX_DESC_LEN, function);
	if (len < 0)
		return len;

	((struct usb_config_descriptor *)buf)->bDescriptorType = type;

	return len;
}



/****************************************************************************/
/* NAME:      SPEAr_MUX_AllocReq                                            */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Allocate memory for usb request for an endpoint            */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :      ep:  endpoint for which usb request has to be allocated       */
/*            len: size of buf                                              */
/*            kmalloc_flags: flags to be passed while allocating memory     */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:    alloctaed usb request                                         */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static struct usb_request *SPEAr_MUX_AllocReq(struct usb_ep *ep, unsigned int len, int kmalloc_flags)
{
	struct usb_request *req;
	

	if (ep == NULL)
		return NULL;

	req = usb_ep_alloc_request(ep, kmalloc_flags);

	if (req != NULL) 
	  {
		req->length = len;
		req->buf = usb_ep_alloc_buffer(ep,len,&req->dma, kmalloc_flags); 

		if (req->buf == NULL) 
		  {
			usb_ep_free_request(ep, req);
			return NULL;
		  }
	  }

	return req;
}


/****************************************************************************/
/* NAME:      SPEAr_MUX_FreeReq                                             */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     Frees an allocated USB request                          */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :            ep:  endpoint for which req was assigned                */
/*                  req: request to be freed                                */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

static void SPEAr_MUX_FreeReq(struct usb_ep *ep, struct usb_request *req)
{
	
	if (ep != NULL && req != NULL) 
	  {
		if(req->buf != NULL)  
			usb_ep_free_buffer ( ep,req->buf,req->dma,req->length ); 

		usb_ep_free_request(ep, req);
	  }
}



/****************************************************************************/
/* NAME:      SPEAr_MUX_Suspend                                             */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:   Invoked by UDC driver on USB suspend                      */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           gadget structure filled by UDC driver                   */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:                                                                  */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

	static void
SPEAr_MUX_Suspend (struct usb_gadget *gadget)
{
	int i;
	struct mux_dev *dev = get_gadget_data(gadget);

	
	spin_lock(&dev->dev_lock);

	for( i=0; dev->gadget_drivers[i] != NULL && i < interfaces; i++)
	  {
		if(dev->gadget_drivers[i]->suspend != NULL )
		  {
			set_gadget_data(gadget, dev->gadget_drivers[i]->driver);
			dev->gadget_drivers[i]->suspend(gadget);	
		  }

	  }
	set_gadget_data(gadget, dev);
	dev->suspended = 1;

	spin_unlock(&dev->dev_lock);
}


/****************************************************************************/
/* NAME:      SPEAr_MUX_Resume                                              */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:    Called from UDC driver when  MUX is resumed after        */
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
SPEAr_MUX_Resume (struct usb_gadget *gadget)
{
	int i;
	struct mux_dev *dev = get_gadget_data(gadget);

	
	spin_lock(&dev->dev_lock);
	for( i=0; dev->gadget_drivers[i] != NULL  && i < interfaces; i++)
	  {
		if(dev->gadget_drivers[i]->resume != NULL )
		  {
			set_gadget_data(gadget, dev->gadget_drivers[i]->driver);

			dev->gadget_drivers[i]->resume(gadget); 
		  }

	  }
	set_gadget_data(gadget, dev);
	dev->suspended = 0;

	spin_unlock(&dev->dev_lock);
}

/* gadget driver struct */

static struct usb_gadget_driver mux_gadget_driver = {
#ifdef CONFIG_USB_GADGET_DUALSPEED
	.speed =                USB_SPEED_HIGH,
#else
	.speed =                USB_SPEED_FULL,
#endif /* CONFIG_USB_GADGET_DUALSPEED */

	.function =             MUX_LONG_NAME,

	.bind =                 SPEAr_MUX_Bind,
	.unbind =               SPEAr_MUX_Unbind,

	.setup =                SPEAr_MUX_Setup,
	.disconnect =           SPEAr_MUX_Disconnect,

	.suspend	= 	    SPEAr_MUX_Suspend,
	.resume	=	    SPEAr_MUX_Resume,

	.driver = {
		.name =         MUX_SHORT_NAME,
		.owner = 	THIS_MODULE,
		/* .shutdown = ... */
		/* .suspend = ...  */
		/* .resume = ...   */
	},
};



static int __init SPEAr_MUX_ModuleInit(void);
static void __exit SPEAr_MUX_ModuleExit(void);

module_init(SPEAr_MUX_ModuleInit);
module_exit(SPEAr_MUX_ModuleExit);
MODULE_AUTHOR ("Shiraz Hashim/CPG");
MODULE_LICENSE ("Dual BSD/GPL");

/****************************************************************************/
/* NAME:      SPEAr_MUX_ModuleInit                                          */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:      Called when MUX module is loaded                       */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :                                                                    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:           0 on success error otherwise                           */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/


static int __init SPEAr_MUX_ModuleInit(void)
{
	

	mux_device.intf_cntr = 0;
	mux_device.hs_desc_cntr = 1;  //0th desc is otg
	mux_device.fs_desc_cntr = 1;  //0th desc is otg

	mux_config_desc.bNumInterfaces = interfaces;
	mux_device_desc.idVendor       = __constant_cpu_to_le16 (vendor);
	mux_device_desc.idProduct      = __constant_cpu_to_le16 (product);
	mux_device_desc.bcdDevice      = cpu_to_le16(bcdDevice);
	if(iManufacturer)
	    strlcpy (manufacturer, iManufacturer, sizeof manufacturer);
	if (iProduct)
	    strlcpy (product_desc, iProduct, sizeof product_desc);
	if(iSerialNum)
	    strlcpy (serial_num, iSerialNum, sizeof serial_num);

	spin_lock_init(&mux_device.dev_lock);

	return 0;
}

/****************************************************************************/
/* NAME:      SPEAr_MUX_ModuleExit					    */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:  Called when MUX module is unloaded                         */
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

static void __exit SPEAr_MUX_ModuleExit(void)
{


    printk(KERN_INFO "mux_module_exit: %s %s unloaded\n", MUX_LONG_NAME, MUX_VERSION_STR);
}


/* ---------------------------------------------------------------- */
/*		Exported Kernel APIS		    		    */
/* ---------------------------------------------------------------- */



/****************************************************************************/
/* NAME:      SPEAr_MUX_UsbGadgetRegisterDriver                             */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION:     API for gadget driver to register to MUX                */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :           driver: tMUX_CompositeGadgetDriver structure to be       */
/*                         supplied by gadget  drivers                      */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:         int: 0 on success err otherwise                          */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

    int
SPEAr_MUX_UsbGadgetRegisterDriver (struct tMUX_CompositeGadgetDriver *driver)
{
    struct mux_dev *dev= &mux_device;
    int retval, i;




    if (!driver ||  !driver->bind || !driver->unbind || !driver->disconnect || !driver->setup
	    || !driver->set_config || !driver->reset_config || !driver->fs_descriptor || !driver->hs_descriptor)
    {
	printk("Erro invalid driver\n");
	return -EINVAL;
    }	

    if (!dev)
    {
	printk("Error no dev\n");
	return -ENODEV;
    }	

    spin_lock(&dev->dev_lock);


    driver->interface_no = dev->intf_cntr;
    dev->gadget_drivers[dev->intf_cntr] = driver;

    i=0;
    while( driver->fs_descriptor[i] != NULL )
    {
	mux_fs_function[dev->fs_desc_cntr] = driver->fs_descriptor[i];
	if(mux_fs_function[dev->fs_desc_cntr]->bDescriptorType == USB_DT_INTERFACE )
	{
	    ((struct usb_interface_descriptor *)(mux_fs_function[dev->fs_desc_cntr]))->bInterfaceNumber += dev->intf_cntr ;
	}

	dev->fs_desc_cntr++;
	i++;


    }

    i=0;
    while( driver->hs_descriptor[i] != NULL )
    {
	mux_hs_function[dev->hs_desc_cntr++] = driver->hs_descriptor[i++];

    }

    dev->intf_cntr++;

    spin_unlock(&dev->dev_lock);


    MUX_DEBUG("%s\tinterfaces=%d\n",__FUNCTION__,interfaces );
    if(dev->intf_cntr == interfaces)
    {
	mux_fs_function[dev->fs_desc_cntr] = NULL;
	mux_hs_function[dev->hs_desc_cntr] = NULL;

	retval = usb_gadget_register_driver(&mux_gadget_driver);
	if (retval) {
	    printk(KERN_ERR "mux_module_init: cannot register gadget driver, ret=%d\n", retval);
	    return retval;
	}
    }

    return 0;
}



/****************************************************************************/
/* NAME:      SPEAr_MUX_UsbGadgetUnregisterDriver                           */
/*--------------------------------------------------------------------------*/
/* DESCRIPTION: unregistration from MUX API for gadget drivers              */
/*                                                                          */
/*                                                                          */
/* PARAMETERS:                                                              */
/* IN  :        struct tMUX_CompositeGadgetDriver *driver 	     	    */
/*                                                                          */
/*                                                                          */
/* OUT :                                                                    */
/*                                                                          */
/* RETURN:      int                                                         */
/*--------------------------------------------------------------------------*/
/* REENTRANCY:                                                              */
/****************************************************************************/

int  SPEAr_MUX_UsbGadgetUnregisterDriver (struct tMUX_CompositeGadgetDriver *driver)
{

    int status;
    int i;
    int interface;
    struct mux_dev *dev= &mux_device;



    spin_lock(&dev->dev_lock);

    if( dev->intf_cntr <= 0 )
	return 0;

    interface = driver->interface_no;

    status = usb_gadget_unregister_driver(&mux_gadget_driver);

    while(dev->hs_desc_cntr > 0)
    {
	mux_hs_function[dev->hs_desc_cntr--] = NULL;
    }

    while(dev->fs_desc_cntr > 0)
    {
	mux_fs_function[dev->fs_desc_cntr--] = NULL;
    }

    for(i=0; i<dev->intf_cntr ;i++)
    {
	dev->gadget_drivers[i]=NULL;
    }

    dev->intf_cntr = 0;
    dev->fs_desc_cntr=1;
    dev->hs_desc_cntr=1;

    spin_unlock(&dev->dev_lock);

    return status;


}

EXPORT_SYMBOL(SPEAr_MUX_UsbGadgetUnregisterDriver);
EXPORT_SYMBOL(SPEAr_MUX_UsbGadgetRegisterDriver);
