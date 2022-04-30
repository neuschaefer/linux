/********************************************************************************/
/*   Copyright (C) STMicroelectronics 2006 by Shiraz HAshim 					*/
/*   shiraz.hashim@st.com														*/
/*																				*/
/*   This program is free software; you can redistribute it and/or modify		*/
/*   it under the terms of the GNU General Public License as published by		*/
/*   the Free Software Foundation; either version 2 of the License, or			*/
/*   (at your option) any later version.										*/
/*																				*/
/*   This program is distributed in the hope that it will be useful,			*/
/*   but WITHOUT ANY WARRANTY; without even the implied warranty of 			*/
/*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				*/
/*   GNU General Public License for more details.								*/
/*																				*/
/*   You should have received a copy of the GNU General Public License			*/
/*   along with this program; if not, write to the								*/
/*   Free Software Foundation, Inc.,											*/
/*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.					*/
/********************************************************************************/

#ifndef _SPEAR_MUX_H_
#define _SPEAR_MUX_H_

/*--------------------------------------------------------------------------*/
/*     MUX related defines                 				    				*/
/*--------------------------------------------------------------------------*/
#define MUX_LONG_NAME		"MUX"
#define MUX_SHORT_NAME		"MUX"
#define MUX_MAX_DESC_LEN  	256
#define MUX_VERSION_STR		"1.00"
#define MUX_VERSION_NUM		0x0200
#define MUX_SERIAL_NUMBER	"0"
#define MUX_MANUFACTURER	"ST/CPG"
#define MUX_PRODUCT_NAME	"Mux Composite Device"

/* device related info */
#define MUX_CONFIG_ID			3

#define MUX_VENDOR_ID			0x1234
#define MUX_PRODUCT_ID			0x4321

#ifdef MUX_DBG_FLG
#define MUX_DEBUG( x... )	do{\
			    printk(x);\
    			    }while(0);
#else
#define MUX_DEBUG( x... )
#endif

/* max no of enps, interfaces etc. */
#define MUX_MAX_ENDP		16
#define MUX_MAX_INTF		4
#define MUX_MAX_DESC		12

/* no of config, interfaces etc. */
#define MUX_NUM_CONFIGS			1
#define MUX_NUM_INTF			1

/*----------------------------------------------------------------------*/
/*     Structure representing gadget drivers							*/
/*     To be supplied by each gadget drivers							*/
/*     while Registration and Unregistration							*/
/*----------------------------------------------------------------------*/

struct tMUX_CompositeGadgetDriver
{
	char           *function;
	int             interface_no;

	int             (*bind) (struct usb_gadget *);
	void            (*unbind) (struct usb_gadget *);
	int             (*setup) (struct usb_gadget *, const struct usb_ctrlrequest *, struct usb_request * req);
	void            (*disconnect) (struct usb_gadget *);
	void            (*suspend) (struct usb_gadget *);
	void            (*resume) (struct usb_gadget *);
	int             (*set_config) (void *dev, unsigned config);
	void            (*reset_config) (void *dev);

	// individual interface/device related data
	void           *driver;
	const struct usb_descriptor_header **hs_descriptor;
	const struct usb_descriptor_header **fs_descriptor;
};

/*--------------------------------------------------------------------------*/
/*     Registration and Unregistration APIs				    				*/
/*--------------------------------------------------------------------------*/

extern int      SPEAr_MUX_UsbGadgetRegisterDriver(struct tMUX_CompositeGadgetDriver *);
extern int      SPEAr_MUX_UsbGadgetUnregisterDriver(struct tMUX_CompositeGadgetDriver *);

#endif /* _SPEAR_MUX_H_ */
