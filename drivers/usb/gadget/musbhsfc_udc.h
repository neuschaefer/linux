/*
 * linux/drivers/usb/gadget/musbhsfc_udc.h
 * Inventra MUSBHSFC USB device controller
 *
 * Copyright (C) 2004 Mikko Lahteenmaki, Nordic ID
 * Copyright (C) 2004 Bo Henriksen, Nordic ID
 * Copyright (C) 2004 IBM Corp.
 * Copyright (C) 2005 Montavista Software, Inc. <source@mvista.com>
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

#ifndef __MUSBHSFC_H_
#define __MUSBHSFC_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/delay.h>

#include <asm/byteorder.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>

#include <linux/usb_ch9.h>
#include <linux/usb_gadget.h>

/*
 * Memory map
 */

#define USB_INTRIN	0x00	/* IN interrupt register */
#define USB_POWER	0x02	/* Power management register */
#define USB_FADDR	0x03	/* Function address register */
#define USB_INTRINE	0x04	/* IN interrupt enable reg */
#define USB_INTROUT	0x06	/* OUT interrupt register */
#define USB_INTRUSBE    0x08	/* IntrUSB interrupt en reg */
#define USB_INTRUSB	0x09	/* Interrupt register */
#define USB_INTROUTE	0x0A	/* OUT interrupt enable reg */
#define USB_TESTMODE	0x0C	/* Enables the USB 2.0 test modes */
#define USB_INDEX	0x0D	/* Index register */
#define USB_FRAME	0x0E	/* Frame number */
#define USB_CSR0        0x11	/* Control Status reg for EP0 */
#define USB_INCSRH      0x10	/* Control Status reg for IN */
#define USB_INCSR       0x11	/* Control Status reg for IN */
#define USB_INMAXP      0x12	/* Max packet size for IN ept */
#define USB_OUTCSRH     0x14	/* Control Status reg for OUT */
#define USB_OUTCSR      0x15	/* Control Status reg for OUT */
#define USB_OUTMAXP     0x16	/* Max packet size for OUT ep */
#define USB_OUTCOUNT    0x1A	/* Num of received bytes in */
				/* EP0 FIFO or OUT EP FIFO */
#define USB_FIFO_EP0    0x20	/* FIFOs for Endpoint 0 */
				/* next FIFO is at + 4bytes */

/* Interrupt IN register bit masks */
#define USB_INTRIN_EP0			1<<0
#define USB_INTRIN_EP1			1<<1
#define USB_INTRIN_EP2			1<<2
#define USB_INTRIN_EP3			1<<3

/* Power register bit masks */
#define USB_POWER_ISO_UPDATE		0x80
#define USB_POWER_FS_PHY_ENAB		0x40
#define USB_POWER_HS_ENAB		0x20
#define USB_POWER_HS_MODE		0x10
#define USB_POWER_RESET			0x08
#define USB_POWER_RESUME		0x04
#define USB_POWER_SUSPEND_MODE		0x02
#define USB_POWER_ENABLE_SUSPEND	0x01

/* FADDR bits definitions */
#define USB_FADDR_ADDR_MASK		0x7f  /* address */
#define USB_FADDR_UPDATE		0x80  /* update */

/* Interrupt OUT register bit masks */
#define USB_INTROUT_EP0                  1<<0
#define USB_INTROUT_EP1                  1<<1
#define USB_INTROUT_EP2                  1<<2
#define USB_INTROUT_EP3                  1<<3

/* Interrupt USB register bit masks */
#define USB_INTRUSB_SOF			0x08
#define USB_INTRUSB_RESET		0x04
#define USB_INTRUSB_RESUME		0x02
#define USB_INTRUSB_SUSPEND		0x01

/* Interrupt USB Enable register bit masks */
#define USB_INTRUSBE_SOF		0x08
#define USB_INTRUSBE_RESET		0x04
#define USB_INTRUSBE_RESUME		0x02
#define USB_INTRUSBE_SUSPEND		0x01

/* Testmode register bits */
#define USB_TEST_SE0NAK			0x01
#define USB_TEST_J			0x02
#define USB_TEST_K			0x04
#define USB_TEST_PACKET			0x08

/* CSR0 bit masks */
#define USB_CSR0_SVDSETUPEND		0x80
#define USB_CSR0_SVDOUTPKTRDY		0x40
#define USB_CSR0_SENDSTALL		0x20
#define USB_CSR0_SETUPEND		0x10
#define USB_CSR0_DATAEND		0x08
#define USB_CSR0_SENTSTALL		0x04
#define USB_CSR0_INPKTRDY		0x02
#define USB_CSR0_OUTPKTRDY		0x01

/* Endpoint CSR register bits */
/* IN CSR */
#define USB_INCSRH_AUTOSET		0x80
#define USB_INCSRH_ISO			0x40
#define USB_INCSRH_MODE			0x20
#define USB_INCSRH_DMAENA		0x10
#define USB_INCSRH_FRCDATATOG		0x08
#define USB_INCSR_INCOMPTX		0x80
#define USB_INCSR_CLRDATATOG		0x40
#define USB_INCSR_SENTSTALL		0x20
#define USB_INCSR_SENDSTALL		0x10
#define USB_INCSR_FLUSHFIFO		0x08
#define USB_INCSR_UNDERRUN		0x04
#define USB_INCSR_FIFONEMPTY		0x02
#define USB_INCSR_INPKTRDY		0x01
/* OUT CSR */
#define USB_OUTCSRH_AUTOCLR		0x80
#define USB_OUTCSRH_ISO			0x40
#define USB_OUTCSRH_DMAENA		0x20
#define USB_OUTCSRH_DISNYET		0x10
#define USB_OUTCSRH_DMAMODE		0x08
#define USB_OUTCSRH_INCOMPRX		0x01
#define USB_OUTCSR_CLRDATATOG		0x80
#define USB_OUTCSR_SENTSTALL		0x40
#define USB_OUTCSR_SENDSTALL		0x20
#define USB_OUTCSR_FLUSHFIFO		0x10
#define USB_OUTCSR_DATA_ERROR		0x08
#define USB_OUTCSR_OVERRUN		0x04
#define USB_OUTCSR_FIFOFULL		0x02
#define USB_OUTCSR_OUTPKTRDY		0x01

// Max packet size
#define EP0_PACKETSIZE          64
#define EP0_MAXPACKETSIZE       64

#define BULK_FIFO_SIZE	((unsigned)512)
#ifdef CONFIG_440EP
#define ISO_FIFO_SIZE	((unsigned)1024)
#endif
#ifdef CONFIG_440EPX
/* biggest MAX Packet size */
#define MAX_FIFO_SIZE	((unsigned)3072)
#endif
#ifdef CONFIG_440EP
#define INT_FIFO_SIZE	((unsigned)64)
#endif
#ifdef CONFIG_440EPX
#define INT_FIFO_SIZE	((unsigned)1024)
#endif

#define USB_MAX_BUFFER_SIZE	4096

#define UDC_MAX_ENDPOINTS	4

#define WAIT_FOR_SETUP		0
#define DATA_STATE_XMIT		1
#define DATA_STATE_NEED_ZLP	2
#define WAIT_FOR_OUT_STATUS	3
#define DATA_STATE_RECV		4

/* ********************************************************************************************* */
/* IO
 */

typedef enum ep_type {
	ep_control, ep_bulk_in, ep_bulk_out, ep_interrupt, ep_iso_in, ep_iso_out
} ep_type_t;

struct musbhsfc_ep {
	struct usb_ep ep;
	struct musbhsfc_udc *dev;

	const struct usb_endpoint_descriptor *desc;
	struct list_head queue;

	u8 stopped;
	u8 bEndpointAddress;
	u8 bmAttributes;

	ep_type_t ep_type;
	u32 fifo;
	u32 csr1;
	u32 csr2;
};

struct musbhsfc_request {
	struct usb_request req;
	struct list_head queue;
};

struct musbhsfc_udc {
	struct usb_gadget gadget;
	struct usb_gadget_driver *driver;
	struct device *dev;
	spinlock_t lock;
	u8 remote_wakeup;
	int ep0state;
	struct musbhsfc_ep ep[UDC_MAX_ENDPOINTS];

	unsigned char usb_address;
	unsigned usb2_device:1;
	unsigned speed_set:1;
};

extern struct musbhsfc_udc *the_controller;

#define ep_is_in(EP) 		(((EP)->bEndpointAddress&USB_DIR_IN)==USB_DIR_IN)
#define ep_index(EP) 		((EP)->bEndpointAddress&0xF)
#define ep_maxpacket(EP) 	((EP)->ep.maxpacket)

#endif
