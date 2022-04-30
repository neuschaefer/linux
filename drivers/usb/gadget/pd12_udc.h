/*
 * linux/drivers/usb/gadget/pd12_udc.h
 * Taihu pd12 full speed USB device controllers
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

#ifndef __PD12_UDC_H_
#define __PD12_UDC_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/version.h>
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
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <asm/byteorder.h>
#include <asm/dma.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#include <asm/unaligned.h>
#include <asm/ibm4xx.h>

#include <linux/usb_ch9.h>
#include <linux/usb_gadget.h>

#define TH_PD12_ADDR  0x50000000
#define TH_CPLD_ADDR  0x50100000

#define IRQ_USBINTR 29
#define USB_SUSPEND 0x20

#define UDC_FIFO_EMPTY 0x0
#define UDC_FIFO_FULL 0x01
#define UDC_EP_STALL 0x02
#define UDC_FIFO_UNWRITABLE (UDC_EP_STALL | UDC_FIFO_FULL)
#define UDC_FIFO_UNREADABLE (UDC_FIFO_EMPTY | UDC_EP_STALL)

/* pd12 udc command */
#define PD12_SET_ADDR_EN		0xd0
#define PD12_SET_EP_EN			0xd8
#define PD12_SET_MODE 			0xf3
#define PD12_SET_DMA 			0xfb
#define PD12_READ_INT			0xf4
#define PD12_READ_LAST_STATUS 	0x40
#define PD12_SET_STATUS			0x40
#define PD12_READ_EP_STATUS 	0x80
#define PD12_READ_BUF			0xf0
#define PD12_WRITE_BUF			0xf0
#define PD12_SET_EP_STATUS		0x40
#define PD12_ACK_SETUP			0xf1
#define PD12_CLEAR_BUF			0xf2
#define PD12_VALIDATE_BUF		0xfa
#define PD12_SND_RESUME			0xf6
#define PD12_RD_CUR_FRAME_NUM	0xf5


#define WAIT_FOR_SETUP          0
#define DATA_STATE_XMIT         1
#define WAIT_FOR_OUT_STATUS     2
#define DATA_STATE_RECV         3

#define PD12_SUSPEND_CHG 		0x80
#define PD12_BUS_RST 			0x40
#define PD12_MAIN_IN 			0x20
#define PD12_MAIN_OUT 			0x10
#define PD12_EP1_IN 			0x08
#define PD12_EP1_OUT 			0x04
#define PD12_CNTL_IN 			0x02
#define PD12_CNTL_OUT 			0x01


#define PD12_MAX_ENDPOINTS       6

/* ********************************************************************************************* */
/* IO
 */

struct pd12_ep {
	struct usb_ep ep;
	struct pd12_udc *dev;

	const struct usb_endpoint_descriptor *desc;
	struct list_head queue;
	unsigned long pio_irqs;
	unsigned long dma_irqs;
	short		  dma;

	u8 stopped;
	u8 bEndpointAddress;
	u8 bmAttributes;

};

struct pd12_request {
	struct usb_request req;
	struct list_head queue;
};

struct pd12_udc {
	struct usb_gadget gadget;
	struct usb_gadget_driver *driver;
	struct device *dev;
	spinlock_t lock;

	int ep0state;
	struct pd12_ep ep[PD12_MAX_ENDPOINTS];

	unsigned char usb_address;

	unsigned req_pending:1, req_std:1, req_config:1;
};

static struct pd12_udc *the_controller;

#define ep_is_in(EP) 		(((EP)->bEndpointAddress&USB_DIR_IN)==USB_DIR_IN)
#define ep_index(EP) 		((EP)->bEndpointAddress&0xF)
#define ep_maxpacket(EP) 	((EP)->ep.maxpacket)

#endif
