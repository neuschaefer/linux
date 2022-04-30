/* 
* linux/drivers/usb/gadget/spearplus_udc.c 
* 
*  Copyright (C) 2006   STMicroelectronics 
* 
* This program is free software; you can redistribute it and/or modify 
* it under the terms of the GNU General Public License as published by 
* the Free Software Foundation; either version 2 of the License ,or 
* ( at your option ) any later version. 
* 
* This program is distributed in the hope that it will be useful , 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
* GNU General Public License for more details. 
* 
* You should have received a copy of the GNU General Public License 
* along with this program; if not  , write to the Free Software 
* Foundation ,Inc. ,59 Temple Place ,Suite 330 ,Boston ,MA  02111-1307 USA 
* 
*  Author - Rajeev Kumar <rajeev-dlh.kumar@st.com>
*/


#include <linux/device.h>
#include <linux/dma-mapping.h>
#include <linux/dmapool.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/usb_ch9.h>
#include <linux/usb_gadget.h>
#include <linux/platform_device.h>

#include <asm/arch/hardware.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <asm/unaligned.h>
#include <asm/arch/irqs.h>
#include <asm/arch/spear.h>
#include "spr_udc_syn.h"


//#define printk(fmt...) 
//#define DESC_UPDATE
#ifdef DESC_UPDATE
volatile static struct spearhead_bulkd *curr_desc = NULL;
#endif

#define DEBUG

#define	DRIVER_VERSION	__DATE__
#define	DRIVER_DESC	"SpearPlus USB Device Controller driver"
static const char driver_name[] = "spear_udc";

static struct spearhead_dev *the_controller;
//static int ep0_disable (void);

#ifdef DESC_UPDATE
struct timer_list bytes512_timer;
static void    bytes512_timer_handler(void);
static u32 len=0;
#endif

/*
	static void
show_out_chain (struct spearhead_ep *ep)
{
	volatile struct spearhead_bulkd *tmp_desc;

	DBG (DBG_REGISTERS, "Out Chain for ep %s:\n", ep->ep.name);
	tmp_desc = ep->desc_out_ptr;
	while (tmp_desc != 0x0)
	{

		DBG (DBG_REGISTERS, "%08x %08x %08x %08x\n",
				tmp_desc->status, tmp_desc->reserved, tmp_desc->bufp,
				tmp_desc->nextd);

		if (tmp_desc->status & DmaUsb_LastDescr)
			break;
		tmp_desc =
			list_entry (tmp_desc->desc_list.next, struct spearhead_bulkd,
					desc_list);
	}
}
*/
	static void
show_global (struct spearhead_dev *dev)
{
	struct spearhead_glob_regs *glob;

	glob = dev->glob_base;

	DBG (DBG_REGISTERS, "Global registers:\n");
	DBG (DBG_REGISTERS, "Config: %08x Control: %08x Status: %08x\n",
			readl (&glob->dev_conf),
			readl (&glob->dev_control), readl (&glob->dev_status));
	DBG (DBG_REGISTERS,
			"Dev.Int: %08x mask = %08x Endp.Int: %08x mask = %08x\n",
			readl (&glob->dev_int), readl (&glob->dev_int_mask),
			readl (&glob->endp_int), readl (&glob->endp_int_mask));
}

	static void
show_registers (struct spearhead_ep *ep)
{
	struct spearhead_epin_regs *in_regs;
	struct spearhead_epout_regs *out_regs;

	in_regs = ep->in_regs;
	out_regs = ep->out_regs;

	DBG (DBG_REGISTERS, "Endp: %s, IN REGS\n", ep->ep.name);
	DBG (DBG_REGISTERS, "Control: %08x Status: %08x Bufsize: %08x\n",
			readl (&in_regs->control), readl (&in_regs->status),
			readl (&in_regs->bufsize));
	DBG (DBG_REGISTERS, "Max packet: %08x desc_ptr: %08x\n",
			readl (&in_regs->max_pack_size), readl (&in_regs->desc_ptr));
	DBG (DBG_REGISTERS, "          OUT REGS\n");
	DBG (DBG_REGISTERS, "Control: %08x Status: %08x Frame num: %08x\n",
			readl (&out_regs->control), readl (&out_regs->status),
			readl (&out_regs->frame_num));
	DBG (DBG_REGISTERS, "Bufsize: %08x setup_ptr %08x desc_ptr: %08x\n",
			readl (&out_regs->bufsize), readl (&out_regs->setup_ptr),
			readl (&out_regs->desc_ptr));
}


/* Alternative implementation of descriptors management */
	static int
desc_pool_init (struct spearhead_dev *dev)
{
	struct device *_dev = dev->dev;

	dev->desc_pool = dma_pool_create ("synudc desc pool",
			_dev,
			sizeof (struct spearhead_bulkd), 16, 0);
	if (!dev->desc_pool)
		return -ENOMEM;

	return 0;
}

	static void
desc_pool_remove (struct spearhead_dev *dev)
{
	if (dev->desc_pool)
		dma_pool_destroy (dev->desc_pool);
	dev->desc_pool = NULL;
}

	static inline void
desc_init (struct spearhead_bulkd *desc, dma_addr_t dma)
{
	memset (desc, 0, sizeof *desc);
	desc->dma_addr = dma;
	desc->status = cpu_to_le32 (DmaUsb_HostRdy);
	desc->bufp = cpu_to_le32 (0x0);
	desc->reserved = cpu_to_le32 (0xf0cacc1a);
	INIT_LIST_HEAD (&desc->desc_list);
}

	static struct spearhead_bulkd *
desc_alloc (struct spearhead_dev *dev, int flags)
{
	struct spearhead_bulkd *desc;
	dma_addr_t dma;

	desc = dma_pool_alloc (dev->desc_pool, flags, &dma);
	if (desc != NULL)
	{
		desc_init (desc, dma);
	}
	else
		printk("desc pool_alloc fail %s \n",__FUNCTION__); 
	return desc;

}

	static void
desc_free (struct spearhead_dev *dev, struct spearhead_bulkd *desc)
{
	dma_pool_free (dev->desc_pool, desc, desc->dma_addr);
}

/*
* Put DMA descriptors into freelist.
*/
	static void
USB_PutDescrIntoFreeList (struct spearhead_bulkd *desc)
{
	struct list_head *entry, *temp;
	struct spearhead_bulkd *tmp;

	DBG_ENTRY;
	DBG (DBG_ADDRESS, "desc = %p\n", desc);

	if (desc == NULL)
		return;

	list_for_each_safe (entry, temp, &desc->desc_list)
	{
		tmp = list_entry (entry, struct spearhead_bulkd, desc_list);
		list_del (entry);
		desc_free (the_controller, tmp);
	}

	list_del (&desc->desc_list);
	desc_free (the_controller, desc);

}

/*
* Returns a pointer to a list of 'num' free DMA descriptors.
* If the number of items in freelist is less than the number requested then
* NULL is returned.
*/
	static void *
USB_GetDescrFromFreeList (unsigned short num)
{
	struct spearhead_bulkd *desc;
	struct spearhead_bulkd *desc_prev;
	struct spearhead_bulkd *head;

	DBG (DBG_FUNC_ENTRY, "%s: num = %d\n", __FUNCTION__, num);
	/* 
	* LINUX_TBD: access to free list must be coordinated by semaphores
	*/
	//local_irq_save (flags);	// Disable UDC_AHB interrupts

	/* LINUX_TBD: initialize desc_prev */
	// head = desc_alloc (the_controller, GFP_KERNEL);
	head = desc_alloc (the_controller, GFP_ATOMIC); 
	if (unlikely (!head))
	{
		printk("desc_alloc fail %s\n",__FUNCTION__);
		return NULL;
	}

	desc = head;
	while (--num)
	{
		desc_prev = desc;
		//desc = desc_alloc (the_controller, GFP_KERNEL);
		desc = desc_alloc (the_controller, GFP_ATOMIC); 
		if (unlikely (!desc))
		{
			printk (KERN_INFO "Not enough memory for descriptors\n");
			goto cleanup;
		}
		desc_prev->nextd = cpu_to_le32 (desc->dma_addr);
		list_add_tail (&desc->desc_list, &head->desc_list);
	}

	DBG (DBG_FUNC_EXIT, "%s: head = %p\n", __FUNCTION__, head);
	return head;

cleanup:
	USB_PutDescrIntoFreeList (head);
	return NULL;
}

/**********************************************************************/
/* UDC_AHB register configuration routines */

/*
* Routine to program UDC NE registers
*
*   EndPtBuf[3:0]     = EndPoint number
*   EndPtBuf[4]       = EndPoint direction
*   EndPtBuf[6:5]     = EndPoint Type
*   EndPtBuf[10:7]    = Config number
*   EndPtBuf[14:11]   = Interface number
*   EndPtBuf[18:15]   = Alternate number
*   EndPtBuf[29:19]   = EndPoint MaxPktSize
*   EndPtBuf[31:30]   = Unused bits
*
* Note: 
* When the logic endpoint is bidirectional (inout) then two physical endpoints are assumed
* to exist in the core, so the following code configures both.
*/
	static void
UDCregs_set (struct spearhead_dev *dev, enum usb_device_speed speed) 
{
	u8 *regs = (dev->csr_base + 0x500);
	DBG_ENTRY;
	writel (0xffffffff, (u8 *) regs);
	/* LINUX_TBD: probably only EP0 must be initialized here */
	if (speed == USB_SPEED_HIGH)
	{
		writel (0x02000000, regs + 0x4); 
		writel (0x100001d1, regs + 0x8); //max-packet  
		writel (0x100001c2, regs + 0xC);
		writel (0x100001d3, regs + 0x10); 
		writel (0x100001c4, regs + 0x14);
		writel (0x020001f5, regs + 0x18); 	//intr-in
		writel (0x200089a6, regs + 0x1C); //iso-out
		writel (0x200091b7, regs + 0x20); //iso-in

	}
	else
	{
		writel (0x02000000, regs + 0x4);
		writel (0x020001d1, regs + 0x8); 
		writel (0x020001c2, regs + 0xC);
		writel (0x020001d3, regs + 0x10);
		writel (0x020001c4, regs + 0x14);
		writel (0x020001f5, regs + 0x18);	//intr-in  
		writel (0x200089a6, regs + 0x1C); //iso-out
		writel (0x200091b7, regs + 0x20); //iso-in

	}

#if 0

	if (speed == USB_SPEED_HIGH)
	{
		writel (0x02000000, regs + 0x4); 
		writel (0x100001d1, regs + 0x8); 
		writel (0x100001c2, regs + 0xC);
		writel (0x020001d3, regs + 0x10); 
		writel (0x100001c4, regs + 0x14);
	}
	else
	{
		writel (0x02000000, regs + 0x4);
		// writel (0x020001c1, regs + 0x8);
		writel (0x020001d1, regs + 0x8); 
		// writel (0x020000c2, regs + 0xC);
		writel (0x020001c2, regs + 0xC);
		// writel (0x020001d1, regs + 0x10);
		writel (0x020001d3, regs + 0x10);
		// writel (0x020000d2, regs + 0x14);
		writel (0x020001c4, regs + 0x14);
	}
#endif
}

/*
* Routine to program AHB wrapper registers
*/

/* LINUX_TBD: is dev necessary? */
	static void
AHBregs_set (struct spearhead_dev *dev, struct spearhead_ep *ep)
{
	struct spearhead_epin_regs *in_regs;
	struct spearhead_epout_regs *out_regs;
	struct spearhead_glob_regs *glob;
	u32 tmp;

	DBG_ENTRY;
	/* 
	* Program UDC_AHB  Endpoint Registers
	*/

	in_regs = ep->in_regs;
	out_regs = ep->out_regs;
	glob = ep->dev->glob_base;

	switch (ep->attrib)
	{
		case USB_ENDPOINT_XFER_CONTROL:
			writel ((USB_ENDPOINT_XFER_CONTROL << 4) | Endp_Cntl_FLUSH,
					&in_regs->control);
			writel (ep->fifo_size, &in_regs->bufsize);
			writel (ep->ep.maxpacket, &in_regs->max_pack_size);

			writel (Endp_Cntl_Control, &out_regs->control);
			tmp = ep->ep.maxpacket | (ep->fifo_size << 16);
			writel (tmp, &out_regs->bufsize);
			break;
		case USB_ENDPOINT_XFER_BULK:

			if (is_ep_in (ep))
			{
				writel ((((USB_ENDPOINT_XFER_BULK << 4) | Endp_Cntl_CNAK | 
								Endp_Cntl_FLUSH)  & ~Endp_Cntl_SNAK) ,  &in_regs->control); //SNAK cleared
				writel (ep->fifo_size, &in_regs->bufsize);
				writel (ep->ep.maxpacket, &in_regs->max_pack_size);
			}
			else
			{
				writel ((USB_ENDPOINT_XFER_BULK << 4) | Endp_Cntl_CNAK , //RRDY to be cleared only after kick_dma
						&out_regs->control); 

				tmp = ep->ep.maxpacket | (ep->fifo_size << 16);
				writel (tmp, &out_regs->bufsize);
			}
			break;
		case USB_ENDPOINT_XFER_INT:
			if (is_ep_in (ep))
			{
				writel ((USB_ENDPOINT_XFER_INT << 4) | Endp_Cntl_CNAK |
						Endp_Cntl_FLUSH, &in_regs->control);
				writel (ep->fifo_size, &in_regs->bufsize);
				writel (ep->ep.maxpacket, &in_regs->max_pack_size);
			}
			break;

		case USB_ENDPOINT_XFER_ISOC:

			if (is_ep_in (ep))
			{
				writel ((USB_ENDPOINT_XFER_ISOC << 4) | Endp_Cntl_CNAK | 
						Endp_Cntl_FLUSH, &in_regs->control);
				writel (ep->fifo_size, &in_regs->bufsize);
				writel (ep->ep.maxpacket, &in_regs->max_pack_size);
			}
			else
			{
				writel ((USB_ENDPOINT_XFER_ISOC << 4) | Endp_Cntl_CNAK | Endp_Cntl_RRDY,
						&out_regs->control); 

				tmp = ep->ep.maxpacket | (ep->fifo_size << 16);
				writel (tmp, &out_regs->bufsize);
			}
			break;


		default:
			break;
	}
}

/*
* Routine to program DMA wrapper registers
*/
	static void
DMAregs_set (struct spearhead_ep *ep)
{
	unsigned int epn = ep->addr & ~USB_DIR_IN;
	u32 tmp, descp;
	struct spearhead_epin_regs *in_regs;
	struct spearhead_epout_regs *out_regs;
	struct spearhead_glob_regs *glob;
	struct spearhead_bulkd *desc;

	DBG_ENTRY;
	in_regs = ep->in_regs;
	out_regs = ep->out_regs;
	glob = ep->dev->glob_base;

	if (ep->attrib == USB_ENDPOINT_XFER_CONTROL)
	{
		DBG (DBG_GENERIC, "CONTROL ENDP!\n");

		/* logical Endpoint 0 ---------------------------------- */
		/* Init CSR registers that point to DMA descriptors */
		desc = USB_GetDescrFromFreeList (1);

		if(desc==NULL)
			return;

		descp = desc->dma_addr;
		DBG (DBG_ADDRESS, "setup pointer = %08x\n", descp);

		writel (descp, &(out_regs->setup_ptr));
		ep->setup_ptr = desc;

		desc = USB_GetDescrFromFreeList (1); 

		if(desc==NULL)
			return;

		descp = desc->dma_addr;
		// DBG (DBG_ADDRESS, "descriptor pointer = %08x\n", descp);
		writel (descp, &(out_regs->desc_ptr));
		ep->desc_out_ptr = desc;


		writel (0, &in_regs->desc_ptr);
		ep->desc_in_ptr = NULL;

		tmp = readl (&out_regs->control);
		tmp |= Endp_Cntl_RRDY; 
		writel (tmp, &out_regs->control);
	}
	else
	{
		DBG (DBG_GENERIC, "NON CONTROL ENDP!\n");
		if (ep->addr & USB_DIR_IN)
		{
			writel (0, &in_regs->desc_ptr);
			ep->desc_in_ptr = NULL;
			writel (0, &(out_regs->desc_ptr));
			ep->desc_out_ptr = NULL;
		}
		else
		{
			tmp = readl (&out_regs->control);
			writel (tmp, &out_regs->control);
		}
	}

	/* LINUX_TBD: check if both ints must be enabled */
	tmp = readl (&glob->endp_int_mask);
	tmp &= ~((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn));
	writel (tmp, &glob->endp_int_mask);

}

	static void
clear_nak (struct spearhead_ep *ep)
{
	u32 tmp;

	tmp = readl (&ep->in_regs->control);
	tmp |= Endp_Cntl_CNAK;
	tmp &= ~Endp_Cntl_SNAK;
	writel (tmp, &ep->in_regs->control);
	tmp = readl (&ep->out_regs->control);
	tmp |= Endp_Cntl_CNAK;
	tmp &= ~Endp_Cntl_SNAK;
	writel (tmp, &ep->out_regs->control);
}


	static void
stall_ep (struct spearhead_ep *ep)
{
	u32 tmp;

	/* STALL the endpoint */
	tmp = readl (&ep->out_regs->control);
	tmp |= Endp_Cntl_STALL;
	writel (tmp, &ep->out_regs->control);
	tmp = readl (&ep->in_regs->control);
	tmp |= Endp_Cntl_STALL;
	writel (tmp, &ep->in_regs->control);

}


/*
* endpoint enable/disable
*
*/
	static int
spearhead_ep_enable (struct usb_ep *_ep,
		const struct usb_endpoint_descriptor *desc)
{
	struct spearhead_ep *ep;
	struct spearhead_dev *dev;
	u16 max;
	int is_in;
	unsigned long flags;

	DBG_ENTRY;


	ep = container_of (_ep, struct spearhead_ep, ep);

	if (!_ep || !desc || ep->desc || desc->bDescriptorType != USB_DT_ENDPOINT) {
		printk ("invalid descriptor");
		return -EINVAL;
	}
	dev = ep->dev;
	if (ep == &dev->ep[0]){
		printk("invalid device");
		return -EINVAL;
	}
	if (!dev->driver || dev->gadget.speed == USB_SPEED_UNKNOWN)
	{
		printk ("unknown error");
		return -ESHUTDOWN;
	}

	switch (desc->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK)
	{
		case USB_ENDPOINT_XFER_BULK:
		case USB_ENDPOINT_XFER_INT: 
		case USB_ENDPOINT_XFER_ISOC:
			break;
		default:
			return -EINVAL;
	}

	spin_lock_irqsave (&dev->lock, flags);
	max = le16_to_cpu (get_unaligned (&desc->wMaxPacketSize));
	is_in = (USB_DIR_IN & desc->bEndpointAddress) != 0;

	/* LINUX_TBD: make checks and write _ep->maxpacket */
	ep->ep.maxpacket = max;
	ep->stopped = 0;
	ep->config_req = 0;
	ep->desc = desc;

	AHBregs_set (dev, ep);
	DMAregs_set (ep);

	DBG (DBG_ENDPOINT, "enable endpoint num.%d %s %s maxpacket %u\n",
			ep - dev->ep, ep->ep.name, is_in ? "IN" : "OUT", max);


	spin_unlock_irqrestore (&dev->lock, flags);
	return 0;
}


/*
*      done - retire a request;
*/
	static void
req_done (struct spearhead_ep *ep, struct spearhead_request *req, int status)
{
	unsigned stopped;
	struct spearhead_dev *dev;

	DBG_ENTRY;

	if (req == NULL)
		return;

	// delete the request from the endpoint queue
	list_del_init (&req->queue);

	// report the completion status
	if (likely (req->req.status == -EINPROGRESS))
		req->req.status = status;
	else
		status = req->req.status;

	dev = ep->dev;

	/* Invoke the completion callback
	* (don't modify queue heads during completion callback)
	*/
	stopped = ep->stopped;
	ep->stopped = 1;
	spin_unlock (&dev->lock);
	req->req.complete (&ep->ep, &req->req);

	DBG (DBG_REQUESTS, "complete %s req %p stat %d buf %p len %u/%u\n",
			ep->ep.name, req, status, req->req.buf,
			req->req.actual, req->req.length);


	spin_lock (&dev->lock);
	ep->stopped = stopped;

	DBG_EXIT;
}
#if 0
//code for disabling ep0
static int ep0_disable (void)
{
	struct spearhead_ep *ep;
	struct spearhead_dev *dev;
	struct spearhead_glob_regs *glob;
	struct spearhead_epin_regs *epregs;
	unsigned int epn;
	unsigned int tmp;

	DBG_ENTRY;

	ep = &the_controller->ep[0];
	dev  = ep->dev;
	glob = dev->glob_base;
	epregs = ep->in_regs;
	epn= ep->addr & ~USB_DIR_IN;


	USB_PutDescrIntoFreeList (ep->curr_chain_in);
	writel (0, &ep->in_regs->desc_ptr);
	ep->desc_in_ptr = NULL;
	ep->curr_chain_in = 0;


	tmp = readl (&glob->endp_int_mask);
	tmp |= ((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn));
	writel (tmp, &glob->endp_int_mask);

	/* clear intrs */
	writel((Endp0_IN_Int << epn)|(Endp0_OUT_Int << epn), &glob->endp_int);
	writel(Endp_Status_IN|Endp_Status_TDC ,&ep->in_regs->status);

	/* flush TX FIFO */
	tmp = readl (&ep->in_regs->control);
	tmp |= Endp_Cntl_FLUSH;
	writel ( tmp, &ep->in_regs->control);

	USB_PutDescrIntoFreeList (ep->desc_out_ptr);
	writel (0, &ep->out_regs->desc_ptr);
	ep->desc_out_ptr = NULL;
	ep->curr_chain_out = 0;

	tmp = readl (&glob->endp_int_mask);
	tmp |= ((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn));
	writel (tmp, &glob->endp_int_mask);

	/* clear intrs */
	writel((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn), &glob->endp_int);
	writel(Endp_Status_OUT_Data, &ep->out_regs->status);

	return 0;
}

#endif


	static void
cancel_all_req (struct spearhead_ep *ep, int status)
{
	struct spearhead_request *req;

	DBG_ENTRY;

	while (!list_empty (&ep->queue))
	{
		req = list_entry (ep->queue.next, struct spearhead_request, queue);

		req_done (ep, req, status);
	}


}

	static int
spearhead_ep_disable (struct usb_ep *_ep)
{
	struct spearhead_ep *ep;
	unsigned long flags;
	struct spearhead_dev *dev;
	struct spearhead_glob_regs *glob;
	struct spearhead_epin_regs *epregs;
	unsigned int epn;
	unsigned int tmp;


	DBG_ENTRY;

	ep = container_of (_ep, struct spearhead_ep, ep);
	dev  = ep->dev;
	glob = dev->glob_base;
	epregs = ep->in_regs;
	epn= ep->addr & ~USB_DIR_IN;


	if (!_ep || !ep->desc)
	{
		printk (KERN_ERR "%s, %s not enabled\n", __FUNCTION__,
				_ep ? ep->ep.name : NULL);
		return -EINVAL;
	}

	spin_lock_irqsave (&ep->dev->lock, flags);
	cancel_all_req (ep, -ESHUTDOWN);
	ep->desc = 0;
	ep->stopped = 1;
	ep->config_req = 0;

	if (is_ep_in (ep))
	{
		USB_PutDescrIntoFreeList (ep->curr_chain_in);
		writel (0, &ep->in_regs->desc_ptr);
		ep->desc_in_ptr = NULL;
		ep->curr_chain_in = 0;


		//clear and mask endpoint intrs when disabled
		tmp = readl (&glob->endp_int_mask);
		tmp |= ((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn));
		writel (tmp, &glob->endp_int_mask);
		/* clear intrs */
		writel((Endp0_IN_Int << epn)|(Endp0_OUT_Int << epn), &glob->endp_int);
		writel(Endp_Status_IN|Endp_Status_TDC ,&ep->in_regs->status);

		/* flush TX FIFO */
		tmp = readl (&ep->in_regs->control);
		tmp |= Endp_Cntl_FLUSH;
		writel ( tmp, &ep->in_regs->control);

	}
	else
	{
		USB_PutDescrIntoFreeList (ep->desc_out_ptr);
		writel (0, &ep->out_regs->desc_ptr);
		ep->desc_out_ptr = NULL;
		ep->curr_chain_out = 0;


		//disable RRDY when endpoint disabled

		tmp = readl (&ep->out_regs->control);
		tmp &= ~Endp_Cntl_RRDY;
		writel (tmp, &ep->out_regs->control);

		tmp = readl (&glob->endp_int_mask);
		tmp |= ((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn));
		writel (tmp, &glob->endp_int_mask);

		writel((Endp0_IN_Int << epn) | (Endp0_OUT_Int << epn), &glob->endp_int);
		writel(0xffffffff, &ep->out_regs->status);

	}

	spin_unlock_irqrestore (&ep->dev->lock, flags);

	/* LINUX_TBD: LS - flush fifo??? */
	printk (KERN_INFO "%s disabled\n", _ep->name);
	return 0;
}

/*
*      spearhead_ep_alloc_request - allocate a request data structure
*/
	static struct usb_request *
spearhead_ep_alloc_request (struct usb_ep *_ep, unsigned int gfp_flags)
{
	struct spearhead_request *req;

	DBG_ENTRY;
	if(!_ep) 
		return NULL; 

	req = kmalloc (sizeof *req, gfp_flags);
	if (!req)
		return 0;
	memset (req, 0, sizeof *req);
	INIT_LIST_HEAD (&req->queue);
	return &req->req;
}


/*
*      spearhead_ep_free_request - deallocate a request data structure
*/
	static void
spearhead_ep_free_request (struct usb_ep *_ep, struct usb_request *_req)
{
	struct spearhead_request *req;

	DBG_ENTRY;
	if(!_ep) 
		return; 

	req = container_of (_req, struct spearhead_request, req);

	WARN_ON (!list_empty (&req->queue));
	list_del_init (&req->queue); // delete the queued request also
	kfree (req);
}


/* LINUX_TBD: allocate buffers in a cache coherent manner */
	static void *
spearhead_ep_alloc_buffer (struct usb_ep *_ep, unsigned bytes,
		dma_addr_t * dma, unsigned int gfp_flags)
{
	char *retval;
	struct spearhead_ep *ep;

	DBG_ENTRY;
	if(!_ep) 
		return NULL; 

	ep = container_of (_ep, struct spearhead_ep, ep);

	retval = dma_alloc_coherent (ep->dev->dev, bytes, dma, gfp_flags);
	if(!retval)
	{
		printk("fail %s\n",__FUNCTION__);
	}

	return retval;
}

	static void
spearhead_ep_free_buffer (struct usb_ep *_ep, void *buf, dma_addr_t dma,
		unsigned bytes)
{
	struct spearhead_ep *ep;

	DBG_ENTRY;
	ep = container_of (_ep, struct spearhead_ep, ep);

	if(!_ep) 
		return; 

	dma_free_coherent (ep->dev->dev, bytes, buf, dma);
}

/*
* Start a DMA transfer (IN or OUT)
*/
	static int
kick_dma (struct spearhead_ep *ep, struct spearhead_request *req,
		unsigned char is_in)
{
	struct spearhead_bulkd *HeadBulkDescr;
	struct spearhead_bulkd  *BulkDescrPtr;
	unsigned int dma_descr_num = 0;
	unsigned int rest = 0, tmp;
	struct spearhead_dev *dev  ;
	//unsigned char *bufp;
	unsigned char *bufp = NULL;
	u32 descp;
	u32 tmp_sof, tmp_frame;

	DBG_ENTRY;

	dev = ep->dev;

	bufp = (unsigned char *) req->req.dma;
	if (bufp == NULL)
	{
		printk("bufp is zero");
		return -1;
	}

	DBG (DBG_ADDRESS, "bufp = %p\n", bufp);

	/* LINUX_TBD: eliminate magic numbers */
	dma_descr_num = ((req->req.length + 63) / ep->ep.maxpacket);
	if (dma_descr_num == 0)
		dma_descr_num = 1;

	rest = req->req.length;

	// Get a new DMA chain
	HeadBulkDescr = BulkDescrPtr =
		(struct spearhead_bulkd *) USB_GetDescrFromFreeList (dma_descr_num);
	if (BulkDescrPtr)
	{
		if (is_in)
		{			// IN request
			// Prepare the chain
			struct list_head *ptr;

			list_for_each (ptr, &HeadBulkDescr->desc_list)
			{
				BulkDescrPtr->status = DmaUsb_HostRdy | ep->ep.maxpacket;
				BulkDescrPtr->bufp = cpu_to_le32 (bufp);
				bufp += ep->ep.maxpacket;
				rest -= ep->ep.maxpacket;
				descp = le32_to_cpu (BulkDescrPtr->nextd);
				BulkDescrPtr =
					list_entry (ptr, struct spearhead_bulkd, desc_list);
			}

			// Last descriptor
			BulkDescrPtr->bufp = cpu_to_le32 (bufp);
			BulkDescrPtr->status = DmaUsb_LastDescr |
				(rest & DmaUsb_Len_Mask);

			ep->curr_chain_in = HeadBulkDescr;

			if( ep->attrib == USB_ENDPOINT_XFER_ISOC)
			{
				//tmp = ((*((volatile unsigned int *)(0xE1100408)) & 0x1ffc0000) >> 18);
				//tmp+=1;
				//HeadBulkDescr->status |= (tmp << 16 );
				
				tmp_sof = ((*((volatile unsigned int *)(0xE1100408)) & 0xffe00000) >> 21);
				tmp_frame = ((*((volatile unsigned int *)(0xE1100408)) & 0x001c0000) >> 18);
				if( tmp_sof < 2047 )
					tmp_sof += 1;
				else
					tmp_sof = 0;
				
				HeadBulkDescr->status |= (tmp_sof << 19) |(tmp_frame << 16 );

				descp = HeadBulkDescr->dma_addr;
				ep->desc_in_ptr = HeadBulkDescr;

				writel (descp, &(ep->in_regs->desc_ptr));

				tmp = readl (&(ep->in_regs->control));
				tmp &= ~Endp_Cntl_SNAK;
				tmp |= Endp_Cntl_POLL | Endp_Cntl_CNAK;
				writel (tmp, &(ep->in_regs->control));
			}

			//Clear NAK which was set in interrupt */
			//if(ep != &dev->ep[0])
			else
			{
				tmp = readl (&ep->in_regs->control);
				tmp |= Endp_Cntl_CNAK;
				tmp &= ~Endp_Cntl_SNAK;
				writel (tmp, &ep->in_regs->control);

				// POLL bit will be set only when IN packet is received from host
			}

		}

		else
		{			// OUT request 
			// Prepare the chain
			struct list_head *ptr;


			list_for_each (ptr, &HeadBulkDescr->desc_list)
			{
				BulkDescrPtr->status = DmaUsb_HostRdy;
				BulkDescrPtr->bufp = cpu_to_le32 (bufp);
				bufp += ep->ep.maxpacket;
				descp = le32_to_cpu (BulkDescrPtr->nextd);
				BulkDescrPtr =
					list_entry (ptr, struct spearhead_bulkd, desc_list);
			}

			BulkDescrPtr->bufp = cpu_to_le32 (bufp);
			BulkDescrPtr->status = DmaUsb_LastDescr | DmaUsb_HostRdy;

			// Use regs (LINUX_TBD) 
			descp = HeadBulkDescr->dma_addr;
			DBG (DBG_ADDRESS, "descriptor pointer = %08x\n", descp);
			writel (descp, &(ep->out_regs->desc_ptr));
			ep->desc_out_ptr = HeadBulkDescr;

#if 0
			tmp = readl (&(ep->out_regs->control));
			tmp |= Endp_Cntl_RRDY;
			writel (tmp, &(ep->out_regs->control));
#endif
#ifdef DESC_UPDATE
			curr_desc = HeadBulkDescr;
#endif /* DESC_UPDATE */
		}
	}
	else
	{
		return -ENOMEM;
	}
	return 0;
}

static void show_queues (struct spearhead_dev *dev);

	static int
spearhead_ep_queue (struct usb_ep *_ep, struct usb_request *_req,
		unsigned int gfp_flags)
{
	struct spearhead_request *req;
	struct spearhead_ep *ep;
	unsigned char is_in;
	u32 tmp;
	struct spearhead_dev *dev;
	unsigned long flags;
	struct spearhead_epin_regs *in_regs;
	struct spearhead_epout_regs *out_regs;
	int rcv_rdy_flag=0; //flag to chk whether kick dma called or not


	DBG_ENTRY;

	req = container_of (_req, struct spearhead_request, req);
	ep = container_of (_ep, struct spearhead_ep, ep);

	in_regs = ep->in_regs;
	out_regs = ep->out_regs;
	is_in = is_ep_in (ep);
	dev = ep->dev;

	spin_lock_irqsave (&dev->lock, flags);

	DBG (DBG_REQUESTS, "%s req %p: length %d buf %p\n",
			ep->ep.name, _req, _req->length, _req->buf);

	_req->status = -EINPROGRESS;
	_req->actual = 0;

	/* kickstart this i/o queue? */
	if (list_empty (&ep->queue) && !ep->stopped)
	{
		rcv_rdy_flag=1;

		if (ep->attrib == USB_ENDPOINT_XFER_CONTROL)
		{
			switch (dev->ep0state)
			{
				/* LINUX_TBD: Why only ep0 can be of type control? */
				case EP0_DATA_IN_STAGE:
					if (kick_dma (ep, req, 1) == 0)
					{
						DBG_PASS;
						clear_nak (ep);
					}
					break;
				case EP0_DATA_OUT_STAGE:
					if (kick_dma (ep, req, 0) == 0)
					{
						DBG_PASS;
						clear_nak (ep);
					}
					break;
				case EP0_CTRL_IDLE:
					DBG_PASS;
					if(req->req.length == 0)
					{
						req_done (ep, req, 0);
						req =0;
					}
					break;
				default:
					SYNUDC_ERR ("Queued request while in STATUS state %d\n",
							dev->ep0state);
					break;
			}
		}
		else
		{
			if (kick_dma (ep, req, is_in) == 0)
			{
				DBG_PASS;
				if (is_in == 0)
				{

					//				out_regs = 0xf0000240;
					tmp = readl (&out_regs->control);
					tmp |= Endp_Cntl_CNAK;
					tmp &= ~Endp_Cntl_SNAK;
					writel (tmp, &out_regs->control);
				}
				else
				{
					tmp = readl (&in_regs->control);
					tmp |= Endp_Cntl_CNAK;
					tmp &= ~Endp_Cntl_SNAK;
				}
				writel (tmp, &in_regs->control);
			}
		}
	}

	if (ep->stopped)
	{
		DBG (DBG_REQUESTS, "Request %p queued while ep %d stopped\n",
				req, ep - &dev->ep[0]);
	}

	/* dma irq handler advances the queue. */
	if (likely (req != 0))
	{
		list_add_tail (&req->queue, &ep->queue);
	}

	spin_unlock_irqrestore (&dev->lock, flags);

	if(rcv_rdy_flag)
	{
		//enable only when kick_dma caled
		tmp = readl (&(out_regs->control));
		tmp |= Endp_Cntl_RRDY;
		writel (tmp, &(out_regs->control));
	}

	show_queues (dev);
	return 0;
}


/* LINUX_TBD: implement these functions!!! */
	static int
spearhead_ep_dequeue (struct usb_ep *_ep, struct usb_request *_req)
{
	struct spearhead_ep *ep;
	unsigned long           flags;
	struct spearhead_request     *req;
	struct spearhead_request     *request;
	u32 tmp;


	if (!_ep )
		return -EINVAL;

	ep = container_of(_ep, struct spearhead_ep, ep);
	request = container_of(_req, struct spearhead_request, req);


	if (!_ep || ep->ep.name == driver_name)
		return -EINVAL;

	spin_lock_irqsave(&ep->dev->lock, flags);

	/* make sure it's actually queued on this endpoint */

	list_for_each_entry(req, &ep->queue, queue) {
		if (&(req->req) == _req)
			break;
	}
	if (&(req->req) != _req) {
		spin_unlock_irqrestore(&ep->dev->lock, flags);
		return -EINVAL;
	}

	req_done(ep, req, -ECONNRESET);

	if (is_ep_in (ep))
	{

		USB_PutDescrIntoFreeList (ep->curr_chain_in);
		writel (0, &ep->in_regs->desc_ptr);
		ep->desc_in_ptr = NULL;
		ep->curr_chain_in = 0;

		/* Flush transmit FIFO */
		tmp = readl (&ep->in_regs->control);
		tmp |= Endp_Cntl_FLUSH;
		writel ( tmp, &ep->in_regs->control);

	}
	else
	{
		USB_PutDescrIntoFreeList (ep->curr_chain_out);
		writel (0, &ep->out_regs->desc_ptr);
		ep->desc_out_ptr = NULL;
		ep->curr_chain_out = 0;
	}

	spin_unlock_irqrestore(&ep->dev->lock, flags);
	return 0;
}

	static int
spearhead_ep_set_halt (struct usb_ep *_ep, int value)
{
	DBG_ENTRY;
	return 0;
}

	static int
spearhead_ep_fifo_status (struct usb_ep *_ep)
{
	DBG_ENTRY;
	printk("spearhead_ep_fifo_status\n");	
	return -EOPNOTSUPP;
}

/* LINUX_TBD: include rx case here? */
	static void
spearhead_ep_fifo_flush (struct usb_ep *_ep)
{
	struct spearhead_ep *ep;
	struct spearhead_epin_regs *regs;
	int epn;
	u32 tmp;
	struct spearhead_dev *dev;

	DBG_ENTRY;

	ep = container_of (_ep, struct spearhead_ep, ep);

	dev = ep->dev;
	regs = ep->in_regs;
	epn = ep->addr & ~USB_DIR_IN;

	if (is_ep_in (ep))
	{
		tmp = readl (&regs->control);
		tmp |= Endp_Cntl_FLUSH;
		writel (tmp, &regs->control);
	}
}

/**********************************************************************/
/* DEVICE SPECIFIC ACTIVITY */

/* LINUX_TBD: read all RX FIFO in slave mode until it is empty,
   then return to DMA mode */
#if 0
	static void
spearhead_RXFifo_flush (struct spearhead_dev *dev)
{
	unsigned long flags;
	u32 tmp;
	struct spearhead_glob_regs *glob = dev->glob_base;


	DBG_ENTRY;

	/* Disable UDC_AHB interrupts */
	local_irq_save (flags);

	tmp = readl (&glob->dev_control);
	tmp &= ~Dev_Cntl_DMAMode;
	writel (tmp, &glob->dev_control);

	tmp = readl (&glob->dev_status);
	while (!(tmp & Dev_Stat_RXFIFO_EMPTY))
	{
		// LINUX_TBD: more than one FIFO RX
		readb (dev->fifo_base);
		tmp = readl (&glob->dev_status);
	}

	tmp = readl (&glob->dev_control);
	tmp |= Dev_Cntl_DMAMode;
	writel (tmp, &glob->dev_control);

	/* Enable UDC_AHB interrupts */
	local_irq_restore (flags);
}
#endif

/*
*      spearhead_reinit - initialize software state
*/
	static void
spearhead_reinit (struct spearhead_dev *dev)
{
	u32 i;

	DBG_ENTRY;

	/* device/ep0 records init */
	INIT_LIST_HEAD (&dev->gadget.ep_list);
	INIT_LIST_HEAD (&dev->gadget.ep0->ep_list);
	dev->ep0state = EP0_CTRL_IDLE;
	DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
	dev->int_cmd = 0;

	/* basic endpoint records init */
	for (i = 0; i < SYNUDC_NUM_ENDPOINTS; i++)
	{

		struct spearhead_ep *ep = &dev->ep[i];

		if (i != 0)
			list_add_tail (&ep->ep.ep_list, &dev->gadget.ep_list);

		ep->curr_chain_in = NULL;
		ep->desc = 0; 
		ep->stopped = 0;
		INIT_LIST_HEAD (&ep->queue);


	}
}

/*
* Cancel all USB request still pending for every endpoint and
* disconnect the gadget driver.
*/
	static void
stop_activity (struct spearhead_dev *dev, struct usb_gadget_driver *driver)
{
	int i;

	DBG_ENTRY;
	/* don't disconnect drivers more than once */
	if (dev->gadget.speed == USB_SPEED_UNKNOWN)
		driver = NULL;
	dev->gadget.speed = USB_SPEED_UNKNOWN;

	/* prevent new request submissions, kill any outstanding requests  */
	for (i = 0; i < SYNUDC_NUM_ENDPOINTS; i++)
	{

		struct spearhead_ep *ep = &dev->ep[i];

		ep->stopped = 1;
		cancel_all_req (ep, -ESHUTDOWN);
	}

//	ep0_disable(); //disable ep0 also on disconnect

	if (driver)
	{
		spin_unlock (&dev->lock);
		driver->disconnect (&dev->gadget);
		spin_lock (&dev->lock);
	}


	/* re-init driver-visible data structures */
	spearhead_reinit (dev);
}


/*
*      spearhead_disable - disable USB device controller
*/
	static void
spearhead_disable (struct spearhead_dev *dev)
{
	DBG_ENTRY;
	dev->gadget.speed = USB_SPEED_UNKNOWN;	// init speed
}

/* 
* until it's enabled, this UDC should be completely invisible
* to any USB host.
*/
	static void
spearhead_enable (struct spearhead_dev *dev)
{
	u32 tmp, status;
	struct spearhead_plug_regs *plug = dev->plug_base;
	struct spearhead_glob_regs *glob = dev->glob_base;

	DBG_ENTRY;

	/* init speed */
	dev->gadget.speed =  USB_SPEED_UNKNOWN;      

	/* Clear any pending PLUG interrupt. */
	tmp = readl (&plug->pending);

	/* LINUX_TBD: maybe the part under #if 0 is the right one */
	status = readl (&plug->status);
	if (status & Plug_Status_attached)
	{
		status &= ~(Plug_Status_phy_reset | Plug_Status_phy_mode);
	}
	else
	{
		/* USB cable detached
		* Reset the PHY and switch the mode.
		*/
		status |= (Plug_Status_phy_reset | Plug_Status_phy_mode);
	}

	writel (status, &plug->status);

	status = readl (&plug->status);
	status |= Plug_Status_en;
	writel (status, &plug->status);

	/* Mask all interrupts */
	writel (~0, &glob->dev_int_mask);
	writel (~0, &glob->endp_int_mask);

	/* 
	* Program USB Global Registers
	*/
	tmp = Dev_Conf_HS_SPEED | Dev_Conf_REMWAKEUP | Dev_Conf_SELFPOW |
		Dev_Conf_PHYINT_16;

	writel (tmp, &glob->dev_conf);


#ifdef DESC_UPDATE
	tmp = Dev_Cntl_BurstEn |
		((7 * Dev_Cntl_BurstLenU) & Dev_Cntl_BurstLenMsk) |
		Dev_Cntl_DMAMode | Dev_Cntl_TxDMAEn | Dev_Cntl_DescrUpd;

#else
	tmp = Dev_Cntl_BurstEn |
		((7 * Dev_Cntl_BurstLenU) & Dev_Cntl_BurstLenMsk) |
		Dev_Cntl_DMAMode | Dev_Cntl_TxDMAEn ;
#endif /* DESC_UPDATE */

	writel (tmp, &glob->dev_control);

	/* LINUX_TBD: Here and before, set speed according to enumeration results */
	AHBregs_set (dev, &dev->ep[0]);

	status = readl (&glob->dev_status);
	/* Check Device speed */
	if ((status & Dev_Stat_ENUM) == Dev_Stat_ENUM_SPEED_HS)
	{
		// handling ep0 characteristic
		dev->gadget.speed = USB_SPEED_HIGH;
	}
	else
	{
		// handling ep0 characteristic
		dev->gadget.speed = USB_SPEED_FULL;
	}
	UDCregs_set (dev, dev->gadget.speed); 


	// Clear all interrupts pending
	writel (Dev_Int_Msk, &glob->dev_int);

	/* LINUX_TBD: unmask SOF ints when using iso endpoints */
	writel (Dev_Int_SOF, &glob->dev_int_mask);
}


/* when a driver is successfully registered, it will receive
* control requests including set_configuration(), which enables
* non-control requests.  then usb traffic follows until a
* disconnect is reported.  then a host may connect again, or
* the driver might get unbound.
*/
	int
usb_gadget_register_driver (struct usb_gadget_driver *driver)
{
	struct spearhead_dev *dev = the_controller;
	int retval;

	DBG_ENTRY;
	/* Paranoid */
	if (!driver ||
			/* driver->speed != USB_SPEED_FULL || */
			!driver->bind ||
			!driver->unbind || !driver->disconnect || !driver->setup)
		return -EINVAL;
	if (!dev)
		return -ENODEV;
	if (dev->driver)
		return -EBUSY;


	/* first hook up the driver ... */
	dev->driver = driver;
	dev->gadget.dev.driver = &driver->driver;

	retval = device_add (&dev->gadget.dev);
	retval = driver->bind (&dev->gadget);
	if (retval)
	{
		printk (KERN_ERR "bind to driver %s --> error %d\n",
				driver->driver.name, retval);
		device_del (&dev->gadget.dev);

		dev->driver = 0;
		dev->gadget.dev.driver = 0;
		return retval;
	}


	/* ... then enable host detection and ep0; and we're ready
	* for set_configuration as well as eventual disconnect.
	* NOTE:  this shouldn't power up until later.
	*/
	DBG (DBG_ADDRESS, "dev = %p\n", dev);
	spearhead_enable (dev);
	printk (KERN_INFO "registered gadget driver '%s'\n", driver->driver.name);

#ifdef DESC_UPDATE
	/*initializing timer for tasklet */
	init_timer(&bytes512_timer);
	bytes512_timer.function = &bytes512_timer_handler;
	bytes512_timer.expires = jiffies + HZ/10;
	add_timer(&bytes512_timer);

#endif


	return 0;
}

EXPORT_SYMBOL (usb_gadget_register_driver);

	int
usb_gadget_unregister_driver (struct usb_gadget_driver *driver)
{
	struct spearhead_dev *dev = the_controller;
	unsigned long flags;

	printk (KERN_INFO "%s\n", __FUNCTION__);

	if (!dev)
		return -ENODEV;
	if (!driver || driver != dev->driver)
		return -EINVAL;

	spin_lock_irqsave (&dev->lock, flags);
	spearhead_disable (dev);
	stop_activity (dev, driver);
	spin_unlock_irqrestore (&dev->lock, flags);

	driver->unbind (&dev->gadget);
	dev->driver = 0;

	device_del (&dev->gadget.dev);

	return 0;
}

EXPORT_SYMBOL (usb_gadget_unregister_driver);

/*
* USB Interrupt Handler
*/

	static void
handle_plug_irq (struct spearhead_dev *dev)
{
	struct spearhead_plug_regs *plug = dev->plug_base;
	unsigned int tmp;

	tmp = readl (&plug->pending);

	if (tmp & Plug_intpend)
	{
		DBG_ENTRY;
		tmp = readl (&plug->status);
		if (tmp & Plug_Status_attached)
		{
			/* USB cable attached
			* Turn off PHY reset bit (PLUG detect).
			* Switch PHY opmode to normal operation (PLUG detect).
			*/
			tmp &= ~(Plug_Status_phy_reset | Plug_Status_phy_mode);
		}
		else
		{
			/* USB cable detached
			* Reset the PHY and switch the mode.
			*/
			tmp |= (Plug_Status_phy_reset | Plug_Status_phy_mode);
		}
		writel (tmp, &plug->status);
	}
	return;
}

static void handle_ep0 (struct spearhead_dev *dev);

	static void
handle_internal_cmds (struct spearhead_dev *dev,
		struct usb_ctrlrequest u_ctrl_req)
{
	unsigned int stopped;
	struct spearhead_ep *ep0 = &(dev->ep[0]);


	/* 
	* Manage pending ints. We are sure that no more requests are issued
	* by the host, because the hw NAKs them
	*/
	dev->int_cmd = 1;

	handle_ep0 (dev);

	cancel_all_req (ep0, -EPROTO);

	/* The STATUS_IN transaction will be discarded. */
	dev->ep0state = EP0_CTRL_IDLE;
	DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);

	// call the gadget driver setup() routine.
	stopped = ep0->stopped;
	ep0->stopped = 1;
	if (dev->driver != NULL && dev->driver->setup != NULL)
	{
		spin_unlock (&dev->lock);
		/* LINUX_TBD: if setup() returns error, we have to stall ep0 */
		dev->driver->setup (&dev->gadget, &u_ctrl_req);
		spin_lock (&dev->lock);
	}
	ep0->stopped = stopped;

	/* "Eat" request queued by setup() while ep0 stopped */
	if (!list_empty (&ep0->queue))
	{
		struct spearhead_request *req;

		req = list_entry (ep0->queue.next, struct spearhead_request, queue);

		if (req->req.length == 0)
		{
			req_done (ep0, req, 0);
		}
		else
		{
			SYNUDC_ERR
				("Non zero-length packet queued after int.decoded command\n");
		}
	}
	dev->int_cmd = 0;

	{
		struct spearhead_glob_regs *glob = dev->glob_base;
		u32 tmp;

		tmp = readl (&glob->dev_control);
		tmp |= Dev_Cntl_RxDMAEn;
		writel (tmp, &glob->dev_control);
	}
}

	static void
handle_device_irq (struct spearhead_dev *dev)
{
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_plug_regs *plug = dev->plug_base;
	struct spearhead_epin_regs *ep0regs = dev->epin_base;
	u32 tmp, dev_int, status;


	dev_int = readl (&glob->dev_int);

	if (!dev_int)
		return;

	DBG_ENTRY;
	/* USB Reset detected on cable */
	if (dev_int & Dev_Int_USBRESET)
	{

		DBG (DBG_INTS, "USB RESET\n");

		/* ALL IRQ disabled */
		writel (~0x0, &glob->endp_int_mask);
		/* Disable RX DMA */
		tmp = readl (&glob->dev_control);
		tmp &= ~Dev_Cntl_RxDMAEn;
		writel (tmp, &glob->dev_control);

		// (LINUX_TBD) handling state??
		// Must shutdown (-ESHUTDOWN) all request for all endpoints
		stop_activity (dev, dev->driver);

		/* 
		* Turn off PHY reset bit (PLUG detect).
		* Switch PHY opmode to normal operation (PLUG detect).
		*/
		tmp = readl (&plug->status);
		if (tmp & Plug_Status_attached)
		{
			tmp &= ~(Plug_Status_phy_reset | Plug_Status_phy_mode);
		}
		else
		{
			/* USB cable detached
			* Reset the PHY and switch the mode.
			*/
			tmp |= (Plug_Status_phy_reset | Plug_Status_phy_mode);
		}

		writel (tmp, &plug->status);

		tmp = readl (&ep0regs->control);
		tmp |= Endp_Cntl_FLUSH;
		writel (tmp, &ep0regs->control);


		/* Clear interrupt */
		writel (Dev_Int_USBRESET, &glob->dev_int);
	}

	/* Device Enumeration completed */
	if (dev_int & Dev_Int_ENUM)
	{

		DBG (DBG_INTS, "USB Enumeration\n");

		status = readl (&glob->dev_status);
		/* Check Device speed */
		if ((status & Dev_Stat_ENUM) == Dev_Stat_ENUM_SPEED_HS)
		{
			// handling ep0 characteristic
			dev->gadget.speed = USB_SPEED_HIGH;
		}
		else
		{
			// handling ep0 characteristic
			dev->gadget.speed = USB_SPEED_FULL;
		}

		AHBregs_set (dev, &dev->ep[0]);
		UDCregs_set (dev, dev->gadget.speed);

		/* Clear interrupt */
		writel (Dev_Int_ENUM, &glob->dev_int);

		dev->ep0state = EP0_CTRL_IDLE;
		DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);

		/* program DMA buffer and interrupt registers */
		DMAregs_set (&dev->ep[0]);

		/* enable RX_DMA */
		tmp = readl (&glob->dev_control);
		tmp |= Dev_Cntl_RxDMAEn;
		writel (tmp, &glob->dev_control);
	}

	// The USB will be in SUSPEND in 3 ms
	if (dev_int & Dev_Int_INACTIVE)
	{
		/* Clear interrupt */
		writel (Dev_Int_INACTIVE, &glob->dev_int);
	}

	// SetConfiguration command received
	if (dev_int & Dev_Int_SETCFG)
	{

		struct usb_ctrlrequest u_ctrl_req;
		struct spearhead_ep *ep0 = &dev->ep[0];

		DBG (DBG_INTS, "Set Configuration\n");
		show_registers (ep0);

		/* Since the UDC_AHB controller filters SET_CONFIGURATION, we need
		   to send a fake one to the gadget driver above. We also make sure
		   the ep0 state is EP0_CTRL_IDLE, so that the STATUS_IN transaction
		   coming from above is discarded.  */

		// Prepare the command
		u_ctrl_req.bRequestType =
			USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_DEVICE;
		u_ctrl_req.bRequest = USB_REQ_SET_CONFIGURATION;
		u_ctrl_req.wValue = readl (&glob->dev_status) & Dev_Stat_CFG;
		u_ctrl_req.wIndex = 0;
		u_ctrl_req.wLength = 0;

		DBG (DBG_REQUESTS, "SETUP %02x.%02x v%04x i%04x l%04x\n",
				u_ctrl_req.bRequestType, u_ctrl_req.bRequest,
				u_ctrl_req.wValue, u_ctrl_req.wIndex, u_ctrl_req.wLength);

		handle_internal_cmds (dev, u_ctrl_req);

		writel (Dev_Int_SETCFG, &glob->dev_int);
		show_global (dev);
		show_registers (ep0);
	}

	/* LINUX_TBD: manage SET_INTERFACE command */
	// SetInterface command received
	if (dev_int & Dev_Int_SETINTF)
	{

		struct usb_ctrlrequest u_ctrl_req;
		struct spearhead_ep *ep0 = &dev->ep[0];

		DBG (DBG_INTS, "Set Interface\n");

		show_registers (ep0);
		show_registers (&dev->ep[1]);
		show_registers (&dev->ep[2]);
		show_global (dev);

		/* Since the UDC_AHB controller filters SET_CONFIGURATION, we need
		   to send a fake one to the gadget driver above. We also make sure
		   the ep0 state is EP0_CTRL_IDLE, so that the STATUS_IN transaction
		   coming from above is discarded.  */

		// Prepare the command
		u_ctrl_req.bRequestType = 
			USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE;
		u_ctrl_req.bRequest = USB_REQ_SET_INTERFACE;
		u_ctrl_req.wValue =
			(readl (&glob->dev_status) & Dev_Stat_INTF) >> 4;
		u_ctrl_req.wIndex = 0;
		u_ctrl_req.wLength = 0;

		DBG (DBG_REQUESTS, "SETUP %02x.%02x v%04x i%04x l%04x\n",
				u_ctrl_req.bRequestType, u_ctrl_req.bRequest,
				u_ctrl_req.wValue, u_ctrl_req.wIndex, u_ctrl_req.wLength);

		handle_internal_cmds (dev, u_ctrl_req);

		writel (Dev_Int_SETINTF, &glob->dev_int);
		show_global (dev);
		show_registers (ep0);
	}
	/* LINUX_TBD: if these interrupts are not managed, mask them */
	// USB Suspend detected on cable
	if (dev_int & Dev_Int_SUSPUSB)
	{
		writel (Dev_Int_SUSPUSB, &glob->dev_int);
	}

	// USB Start-Of-Frame detected on cable
	if (dev_int & Dev_Int_SOF)
	{
		writel (Dev_Int_SOF, &glob->dev_int);
	}
}

#ifdef DESC_UPDATE
static void bytes512_timer_handler(void)
{

	struct spearhead_ep *ep;
	struct spearhead_bulkd *tmp_desc;
	struct spearhead_epout_regs *epregs;
	struct spearhead_request *req;
	struct spearhead_dev *dev ;
	struct spearhead_glob_regs *glob ;
	unsigned char *bufp = NULL;
	u32 tmp;
	unsigned int stopped;
	int status;

	if (len>0)
	{
		ep = &the_controller->ep[2];
		dev = the_controller;

		glob = dev->glob_base;

		epregs = ep->out_regs;
		tmp_desc = ep->desc_out_ptr;


		tmp = readl (&epregs->control);
		tmp &= ~Endp_Cntl_RRDY;
		writel (tmp, &epregs->control);


		// For every non-control endpoints we must invoke req_done().
		if (ep->attrib != USB_ENDPOINT_XFER_CONTROL)
		{
			if (!list_empty (&ep->queue)) 
			{
				req =
					list_entry (ep->queue.next,
							struct spearhead_request, queue);
				req->req.actual = len;
				len = 0;
				spin_lock (&dev->lock);
				req_done (ep, req, 0);
				spin_unlock (&dev->lock);
			}

		}

		curr_desc->status= 0xc9000000;

		tmp = readl (&epregs->control);
		tmp |= Endp_Cntl_RRDY;
		writel (tmp, &epregs->control);
	}

	mod_timer(&bytes512_timer, jiffies + 1*HZ);
}

#endif


/*
* The following code handles an OUT interrupt for generic endpoint <epn>.
*/
	static void
spearhead_handle_epn_out_int (struct spearhead_ep *ep)
{
	int epn = ep->addr & ~USB_DIR_IN;
	unsigned int epn_bit = (1 << (epn + 16));
	volatile struct spearhead_bulkd *tmp_desc;
	struct spearhead_dev *dev = ep->dev;
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_epout_regs *epregs = ep->out_regs;
	struct spearhead_request *req;
	u32 tmp;
#ifdef DESC_UPDATE
	static status;
#endif

	// EDPT n  (OUT direction) -----------------------------------------------------

	while (readl (&glob->endp_int) & epn_bit)
	{
		writel (epn_bit, &glob->endp_int);

#ifdef DESC_UPDATE
		mod_timer(&bytes512_timer, jiffies + 1*HZ );
		status = readl (&epregs->status);


		if (status & Endp_Status_BUFFNAV)
		{

			writel (Endp_Status_BUFFNAV, &epregs->status);

			ep->curr_chain_out = ep->desc_out_ptr;
			writel (0, &epregs->desc_ptr);
			ep->desc_out_ptr = NULL;

			// For every non-control endpoints we must invoke req_done().
			if (ep->attrib != USB_ENDPOINT_XFER_CONTROL)
			{
				USB_PutDescrIntoFreeList (ep->curr_chain_out);
				ep->curr_chain_out = 0;

			}
			if (!list_empty (&ep->queue))
			{
				req =
					list_entry (ep->queue.next, struct spearhead_request,
							queue);
				if (kick_dma (ep, req, 0) == 0)
				{

					tmp = readl (&epregs->control);
					tmp |= Endp_Cntl_RRDY;
					writel (tmp, &epregs->control);
				}
			}
			return;

		}

		if(!( readl(&epregs->status) & Endp_Status_OUT_Data )  ||  (curr_desc == NULL))
		{
			printk(" returning sts=0x%x\n",  readl(&epregs->status));
			return;
		}

		status = curr_desc->status;

		if( status & DmaUsb_DmaDone)
		{
			len += (status & DmaUsb_Len_Mask);
		}

		status = curr_desc->status;

		if (status & DmaUsb_LastDescr)
		{
			goto ahead;
		}
		else
		{
			curr_desc =
				list_entry (curr_desc->desc_list.next,
						struct spearhead_bulkd, desc_list);

		}


		writel (Endp_Status_OUT_Data , &epregs->status);

		tmp = readl (&epregs->control);
		tmp |= Endp_Cntl_RRDY;
		writel (tmp, &epregs->control);
		return;
#endif /* DESC_UPDATE */

		/* search in the current chain the endpoint marked as LAST */
		tmp_desc = ep->desc_out_ptr;
		while (tmp_desc != 0x0)
		{
			if (tmp_desc->status & DmaUsb_LastDescr)
				break;
			tmp_desc =
				list_entry (tmp_desc->desc_list.next,
						struct spearhead_bulkd, desc_list);
		}


		// check if the last descriptor is marked as DONE               
#ifdef DESC_UPDATE
ahead: 
		tmp_desc = curr_desc;
#endif /* DESC_UPDATE */

		if (tmp_desc && (tmp_desc->status & DmaUsb_DmaDone))
		{
			ep->curr_chain_out = ep->desc_out_ptr;
			writel (0, &epregs->desc_ptr);
			ep->desc_out_ptr = NULL;

			// For every non-control endpoints we must invoke req_done().
			if (ep->attrib != USB_ENDPOINT_XFER_CONTROL)
			{
				if (!list_empty (&ep->queue)) 
				{
					req =
						list_entry (ep->queue.next,
								struct spearhead_request, queue);
#ifdef DESC_UPDATE
					req->req.actual = len;
					len = 0;
#else
					req->req.actual = tmp_desc->status & DmaUsb_Len_Mask;
#endif /* DESC_UPDATE */

					USB_PutDescrIntoFreeList (ep->curr_chain_out);
					ep->curr_chain_out = 0;

					req_done (ep, req, 0);
				}

			}
		}
		else
		{
			/* Move Descriptors into FREE list */
			USB_PutDescrIntoFreeList (ep->desc_out_ptr);
			writel (0, &epregs->desc_ptr);
			ep->desc_out_ptr = NULL;
		}

		writel (Endp_Status_OUT_Data , &epregs->status);     //clear endpoint sts bit

		/* restart i/o */
		if (!list_empty (&ep->queue))
		{
			req =
				list_entry (ep->queue.next, struct spearhead_request,
						queue);
			if (kick_dma (ep, req, 0) == 0)
			{
				tmp = readl (&epregs->control);
				tmp |= Endp_Cntl_RRDY;
				writel (tmp, &epregs->control);
			}
		}
	}

}


/*
* The following code handles an IN interrupt for generic endpoint <epn>.
*/
	static void
spearhead_handle_epn_in_int (struct spearhead_ep *ep)
{
	int epn = ep->addr & ~USB_DIR_IN;
	unsigned int epn_bit = (1 << epn);
	struct spearhead_bulkd *BulkINDescrPtr, *HEADBlkinDPtr;
	struct spearhead_request *req;
	struct spearhead_dev *dev = ep->dev;
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_epin_regs *epregs = ep->in_regs;
	u32 tmp, status, descp;


	tmp = readl (&glob->endp_int);
	if (tmp & epn_bit)
	{

		DBG_ENTRY;
		writel (epn_bit, &glob->endp_int); // clear endpoint interrupt 

		status = readl (&epregs->status);
		/* If DMA completed transfer to TxFIFO */
		if (status & Endp_Status_TDC)
		{

			HEADBlkinDPtr = ep->desc_in_ptr;
			/* the DMA completed the current descriptor; free it !! */
			USB_PutDescrIntoFreeList (HEADBlkinDPtr);
			writel (0, &epregs->desc_ptr);
			ep->desc_in_ptr = 0;
			HEADBlkinDPtr = NULL; 	

			ep->curr_chain_in = 0;
			if (ep->attrib != USB_ENDPOINT_XFER_CONTROL)
			{
				DBG_PASS;
				if (!list_empty (&ep->queue))
				{

					req =
						list_entry (ep->queue.next,
								struct spearhead_request, queue);
					req_done (ep, req, 0);
				}
			}
			/* restart i/o */
			if (!list_empty (&ep->queue))
			{
				req =
					list_entry (ep->queue.next,
							struct spearhead_request, queue);
				kick_dma (ep, req, 1);
			}

			/* Interrupt ack */
			writel (Endp_Status_TDC,&epregs->status); 
		}

		/* The current IN endpoint received a USB IN request. */
		else if (status & Endp_Status_IN) 
		{

			BulkINDescrPtr = ep->curr_chain_in;

			// Program DMA with next IN chain and start it (set POLL bit).
			if (BulkINDescrPtr != 0x0 && ep->attrib != USB_ENDPOINT_XFER_ISOC)
			{
				descp = BulkINDescrPtr->dma_addr;
				DBG (DBG_ADDRESS, "descriptor pointer = %08x\n", descp);
				writel (descp, &(epregs->desc_ptr));
				ep->desc_in_ptr = BulkINDescrPtr;

				tmp = readl (&(epregs->control));
				tmp |= Endp_Cntl_POLL;  
				writel (tmp, &(epregs->control));

				DBG (DBG_ADDRESS, "BulkINDescrPtr->bufp = %08x\n",
						le32_to_cpu (BulkINDescrPtr->bufp));
				DBG_PASS;
			}
			else
			{
				/* Set NAK * which should be cleared in kick_dma */
				tmp = readl (&epregs->control);
				tmp |= Endp_Cntl_SNAK;
				writel (tmp, &epregs->control);

			}
			writel (Endp_Status_IN , &epregs->status);   
		}

		// clear status IN bit and endpoint interrupt
		show_global (dev); 
	}
}

	static void
new_handle_ep0_in (struct spearhead_ep *ep)
{
	struct spearhead_bulkd *BulkINDescrPtr, *HEADBlkinDPtr;
	struct spearhead_request *req;

	struct spearhead_dev *dev = ep->dev;
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_epin_regs *epregs = ep->in_regs;
	u32 tmp, status, descp;

	DBG_ENTRY;

	status = readl (&epregs->status);

	DBG (DBG_REGISTERS, "epn_bit = %d endp status = %08x\n", epn_bit, status);
	/* Clear interrupt */
	show_global (dev);
	writel (Endp0_IN_Int, &glob->endp_int);

	/* If DMA completed transfer to TxFIFO */
	if (status & Endp_Status_TDC)
	{

		writel (Endp_Status_TDC, &epregs->status);

		HEADBlkinDPtr = ep->desc_in_ptr; 
		/* the DMA completed the current descriptor; free it !! */
		USB_PutDescrIntoFreeList (HEADBlkinDPtr);  

		writel (0, &epregs->desc_ptr);
		ep->desc_in_ptr = 0;
		HEADBlkinDPtr = NULL; 		//@@ Deepak
		ep->curr_chain_in = 0;		//@@ Deepak
		/* Transfer ack */
		/* LINUX_TBD: here we assume everything goes well: 
		* probably errors should be managed
		*/
		switch (dev->ep0state)
		{
			case EP0_DATA_IN_STAGE:
				dev->ep0state = EP0_STATUS_OUT_STAGE;
				DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
				break;
			case EP0_STATUS_IN_STAGE:
				dev->ep0state = EP0_CTRL_IDLE;
				DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
				break;
			default:
				SYNUDC_ERR ("Data tx from ep0 in state %d\n", dev->ep0state);
				stall_ep (ep); 
				break;
		}
	}

	/* The current IN endpoint received a USB IN request. */
	status = readl (&epregs->status);  
	if (status & Endp_Status_IN)
	{

		BulkINDescrPtr = ep->curr_chain_in;
		ep->curr_chain_in = 0;

		// Program DMA with next IN chain and start it (set POLL bit).
		if (BulkINDescrPtr != 0x0)
		{
			descp = BulkINDescrPtr->dma_addr;
			DBG (DBG_ADDRESS, "descriptor pointer = %08x\n", descp);
			writel (descp, &(epregs->desc_ptr));
			ep->desc_in_ptr = BulkINDescrPtr;

			tmp = readl (&(epregs->control));
			tmp |= Endp_Cntl_POLL;
			writel (tmp, &(epregs->control));

			DBG (DBG_ADDRESS, "BulkINDescrPtr->bufp = %08x\n",
					le32_to_cpu (BulkINDescrPtr->bufp));
			DBG_PASS;

			/* restart i/o */
			if (!list_empty (&ep->queue))
			{
				req =
					list_entry (ep->queue.next,
							struct spearhead_request, queue);
				kick_dma (ep, req, 1);
			}
		}
	}

	/* clear status IN bit */
	writel (Endp_Status_IN, &epregs->status);

}

	static void
new_handle_ep0_out_data (struct spearhead_ep *ep)
{
	struct spearhead_dev *dev = ep->dev;
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_epout_regs *ep0_oregs = ep->out_regs;
	struct spearhead_ep *ep0 = ep;
	struct spearhead_request *req;
	u32 tmp;
	struct spearhead_bulkd *descp;

	DBG_ENTRY;

	/*
	* Not a setup packet. Remember that here we are only managing the 
	* ep0 OUT interrupt case.
	*/
	if (list_empty (&ep->queue))
	{
		SYNUDC_ERR ("OUT request received while list empty for ep %d\n",
				ep - &dev->ep[0]); 

		return;
	}
	req = list_entry (ep0->queue.next, struct spearhead_request, queue);

	// We are in a CTRL-IN transfer and this is the STATUS OUT stage.
	if (dev->ep0state == EP0_STATUS_OUT_STAGE)
	{
		/* LINUX_TBD - we should get STATUS from HOST before free it ...  */
		req_done (ep0, req, 0);

		// Move Descriptor into FreeList list
		tmp = readl (&(ep0_oregs->desc_ptr));
		descp = ep0->desc_out_ptr;
		USB_PutDescrIntoFreeList (descp);
		dev->ep0state = EP0_CTRL_IDLE;
		DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
	}
	else if (dev->ep0state == EP0_DATA_OUT_STAGE)
	{
		/* We are in a CTRL-OUT transfer and this is the DATA OUT stage. */

		dev->ep0state =EP0_CTRL_IDLE;

		descp = ep0->desc_out_ptr;
		while (descp != 0x0)
		{
			if (descp->status & DmaUsb_LastDescr)
				break;
			descp =
				list_entry (descp->desc_list.next,
						struct spearhead_bulkd, desc_list);
		}
		
		if (descp && (descp->status & DmaUsb_DmaDone))
		{
			req->req.actual = descp->status & DmaUsb_Len_Mask;
			req_done (ep0, req, 0);
		}


		// Move Descriptor into FreeList list
		tmp = readl (&(ep0_oregs->desc_ptr));
		descp = ep0->desc_out_ptr;
		USB_PutDescrIntoFreeList (descp);
		DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);

	}
	else
	{
		req_done (ep0, req, 0);
	}

	/*
	* Set new descriptor chain (and re-enable the DMA if the case)
	*/
	if ((descp = USB_GetDescrFromFreeList (1)) != NULL)
	{
		DBG_PASS;
		DBG (DBG_ADDRESS, "descriptor pointer = %p\n", descp);
		writel (descp->dma_addr, &(ep0_oregs->desc_ptr));
		ep0->desc_out_ptr = descp;

		tmp = readl (&ep0_oregs->control);
		tmp |= Endp_Cntl_RRDY;
		writel (tmp, &ep0_oregs->control);

		tmp = readl (&glob->dev_control);
		tmp |= Dev_Cntl_RxDMAEn;
		writel (tmp, &glob->dev_control);
	}
	else
	{
		DBG_PASS;
		writel (0, &ep0_oregs->desc_ptr);
		ep0->desc_out_ptr = NULL;
	}

}

	static void
new_handle_ep0_setup (struct spearhead_ep *ep)
{
	struct spearhead_dev *dev = ep->dev;
	struct spearhead_glob_regs *glob = dev->glob_base;
	struct spearhead_setupd *SetupDescrPtr;
	unsigned char *SetupPtr;
	struct usb_ctrlrequest u_ctrl_req;
	int ret = 0;
	struct spearhead_ep *ep0 = ep;
	struct spearhead_epout_regs *ep0_oregs = ep0->out_regs;
	u32 tmp, status;

	DBG_ENTRY;

	SetupDescrPtr = (struct spearhead_setupd *) (ep0->setup_ptr);
	SetupPtr = (unsigned char *) (&(SetupDescrPtr->data1));

	status = le32_to_cpu (SetupDescrPtr->status);
	if ((status & DmaUsb_BS_Mask) == DmaUsb_DmaDone)
	{

		/* LINUX_TBD: why don't we check the error? */
		memcpy (&u_ctrl_req, SetupPtr, 8);
		/* swap some fields in a big_endian architecture */
		le16_to_cpus (&u_ctrl_req.wValue);
		le16_to_cpus (&u_ctrl_req.wIndex);
		le16_to_cpus (&u_ctrl_req.wLength);

		DBG (DBG_REQUESTS, "SETUP %02x.%02x v%04x i%04x l%04x\n",
				u_ctrl_req.bRequestType, u_ctrl_req.bRequest,
				u_ctrl_req.wValue, u_ctrl_req.wIndex, u_ctrl_req.wLength);

		/* If ep0 was not idle, stall it */
		if (dev->ep0state != EP0_CTRL_IDLE)
		{
			/* STALL the endpoint */
			SYNUDC_ERR
				("Setup packet received while not idle: stall ep0\n");
			stall_ep (ep0);
			return;
		}

		if (u_ctrl_req.bRequestType & USB_DIR_IN)
		{
			/*testing for windows starts*/	
			if ((u_ctrl_req.bRequestType == 0xa1) && (u_ctrl_req.bRequest==0xfe))
			{
				stall_ep (ep0);
				tmp = readl (&ep0_oregs->control);
				tmp |= Endp_Cntl_RRDY;
				writel (tmp, &ep0_oregs->control);
				dev->ep0state = EP0_CTRL_IDLE;	//Status IN is handled internall
				return ;
			}
			/*testing windows end*/
			dev->ep0state = EP0_DATA_IN_STAGE;
			DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
		}
		else
		{
			dev->ep0state = EP0_DATA_OUT_STAGE;
			DBG (DBG_EP0STATE, "ep0state = %d\n", dev->ep0state);
		}
		if (u_ctrl_req.wLength == 0)
		{
			dev->ep0state = EP0_CTRL_IDLE;	//Status IN is handled internally
		}


		/* Delegate request fulfillment to gadget driver */
		if (dev->driver != NULL && dev->driver->setup != NULL)
		{
			/* call the gadget driver setup() routine */
			spin_unlock (&dev->lock);
			ret = dev->driver->setup (&dev->gadget, &u_ctrl_req);
			spin_lock (&dev->lock);
			if (ret < 0)
			{
				SYNUDC_ERR ("Gadget setup error: stall ep0\n");
				stall_ep (ep0);
				tmp = readl (&ep0_oregs->control);
				tmp |= Endp_Cntl_RRDY;
				writel (tmp, &ep0_oregs->control);
				dev->ep0state = EP0_CTRL_IDLE;	//Status IN is handled internall

				return;
			}
		}

		/* Set Setup Buffer status to 2'b00 (host ready) */
		SetupDescrPtr->status = cpu_to_le32 (0);

		/* 
		* LINUX_TBD: if there are not multiple receive FIFO controllers,
		* the RRDY bit is reserved. Check!!!
		*/
		tmp = readl (&ep0_oregs->control);
		tmp |= Endp_Cntl_RRDY;
		writel (tmp, &ep0_oregs->control);
		/* LINUX_TBD (nella ep_queue ???) */
		clear_nak (ep0);

		/* The DMA was automatically disabled
		* at the end of the transfer */
		tmp = readl (&glob->dev_control);
		tmp |= Dev_Cntl_RxDMAEn;
		writel (tmp, &glob->dev_control);
	}
}

	static void
new_handle_ep0_out (struct spearhead_ep *ep)
{
	struct spearhead_dev *dev = ep->dev;
	struct spearhead_epout_regs *regs = ep->out_regs;
	struct spearhead_glob_regs *glob = dev->glob_base;
	u32 status, condition;

	DBG_ENTRY;

	/* Check if a setup packet was received */
	status = readl (&regs->status);
	condition = status & Endp_Status_OUTMsk;

	/*
	* Setup packets can still be received after an internally
	* decoded command, so we manage them later.
	*/
	if (dev->int_cmd && condition == Endp_Status_OUT_Setup)
		return;

	/* Clear interrupt */
	show_global (dev);
	writel (Endp0_OUT_Int, &glob->endp_int);

	switch (condition)
	{
		case Endp_Status_OUT_none:
		case Endp_Status_OUT_Data:
			new_handle_ep0_out_data (ep);
			break;
		case Endp_Status_OUT_Setup:
			new_handle_ep0_setup (ep);
			break;
		default:
			SYNUDC_ERR ("Out Int received, but OUT field of status\n"
					"register was neither data nor setup, but %d\n",
					condition);
			break;
	}

	show_global (dev);
	show_registers (ep);
	writel (condition, &regs->status);
	show_global (dev);
	show_registers (ep);

}

	static void
handle_ep0 (struct spearhead_dev *dev)
{
	struct spearhead_ep *ep0 = &dev->ep[0];
	struct spearhead_glob_regs *glob = dev->glob_base;
	u32 tmp;

	/* 
	* It is necessary to serve in or out ep0 interrupt first,
	* depending on ep0state, in order to make the ep0 state 
	* machine evolve the right way.
	*/

	tmp = readl (&glob->endp_int);

	switch (dev->ep0state)
	{
		case EP0_DATA_IN_STAGE:
		case EP0_STATUS_IN_STAGE:
			if (tmp & Endp0_IN_Int)
			{
				new_handle_ep0_in (ep0);
			}
			if (tmp & Endp0_OUT_Int)
			{
				new_handle_ep0_out (ep0);
			}
			break;
		case EP0_DATA_OUT_STAGE:
		case EP0_STATUS_OUT_STAGE:
		case EP0_CTRL_IDLE:
		default:
			if (tmp & Endp0_OUT_Int)
			{
				new_handle_ep0_out (ep0);
			}
			if (tmp & Endp0_IN_Int)
			{
				new_handle_ep0_in (ep0);
			}
			break;
	}
}


	static void
handle_ep_irq (struct spearhead_dev *dev)
{
	u32 i;

	DBG_ENTRY;

	/* ep0 is handled separately */
	handle_ep0 (dev);

	show_global (dev);
	/* Let's handle all the other cases */
	for (i = 1; i < SYNUDC_NUM_ENDPOINTS; i++)
	{

		struct spearhead_ep *ep = &(dev->ep[i]);

		if (is_ep_in (ep))
		{
			spearhead_handle_epn_in_int (ep);
		}
		else
		{
			spearhead_handle_epn_out_int (ep);
		}
	}
}

	static irqreturn_t
spearhead_irq (int irq, void *_dev/*, struct pt_regs *r*/)
{
	struct spearhead_dev *dev = (struct spearhead_dev *) _dev;
	u32 dev_int, endp_int;

	DBG_ENTRY;

	dev_int = readl (&dev->glob_base->dev_int);
	endp_int = readl (&dev->glob_base->endp_int);
	DBG (DBG_INTS, "devint = %08x, endpint = %08x\n", dev_int, endp_int);

	spin_lock (&dev->lock);

	/* Plug detect interrupt (high priority) */
	handle_plug_irq (dev);
	/* Device interrupts (high priority) */
	handle_device_irq (dev);
	/* Endpoint interrupts */
	handle_ep_irq (dev);

	spin_unlock (&dev->lock);

	DBG_EXIT;

	return IRQ_HANDLED;
}

static struct usb_ep_ops spearhead_ep_ops = {
	.enable = spearhead_ep_enable,

	.disable = spearhead_ep_disable,

	.alloc_request= spearhead_ep_alloc_request,
	.free_request = spearhead_ep_free_request,

	.alloc_buffer = spearhead_ep_alloc_buffer,
	.free_buffer = spearhead_ep_free_buffer,

	.queue = spearhead_ep_queue,
	.dequeue = spearhead_ep_dequeue,

	.set_halt = spearhead_ep_set_halt,
	.fifo_status = spearhead_ep_fifo_status,
	.fifo_flush = spearhead_ep_fifo_flush,
};

	static int
spearhead_dev_get_frame (struct usb_gadget *_gadget)
{
	struct spearhead_dev *dev;
	struct spearhead_glob_regs *glob;
	unsigned long flags;
	unsigned int tmp;

	DBG_ENTRY;

	dev = container_of (_gadget, struct spearhead_dev, gadget);

	glob = dev->glob_base;
	spin_lock_irqsave (&dev->lock, flags);
	tmp = readl (&(glob->dev_status));
	spin_unlock_irqrestore (&dev->lock, flags);

	return ((tmp >> 18) & 0x3fff);
}

	static int
spearhead_dev_wakeup (struct usb_gadget *_gadget)
{
	struct spearhead_dev *dev;
	struct spearhead_glob_regs *glob;
	unsigned long flags;
	unsigned int tmp;

	DBG_ENTRY;

	dev = container_of (_gadget, struct spearhead_dev, gadget);

	glob = dev->glob_base;
	spin_lock_irqsave (&dev->lock, flags);
	tmp = readl (&glob->dev_control);
	tmp |= Dev_Cntl_RESUME;
	writel (tmp, &glob->dev_control);
	spin_unlock_irqrestore (&dev->lock, flags);

	return 0;
}

static const struct usb_gadget_ops spearhead_dev_ops = {
	.get_frame = spearhead_dev_get_frame,
	.wakeup = spearhead_dev_wakeup,
};

	static void
nop_release (struct device *dev)
{
	printk (KERN_DEBUG "%s %s\n", __FUNCTION__, dev->bus_id);
}

static struct spearhead_dev memory = {
	.gadget = {
		.ops = &spearhead_dev_ops,
		.ep0 = &memory.ep[0].ep,
		.name = driver_name,
		.dev = {
			.bus_id = "gadget",
			.release = nop_release,
		},
	},
	/* control endpoint */
	.ep[0] = {
		.ep = {
			/* LINUX_TBD: endpoints'names are significant for autoconfiguration: check them */
			.name = "ep0-ctrl",
			.ops = &spearhead_ep_ops,
			.maxpacket = EP0_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP0_FIFO_SIZE,
		.addr = USB_DIR_IN | 0, 
		.attrib = USB_ENDPOINT_XFER_CONTROL,
	},
	/* first group of endpoints */
	.ep[1] = {
		.ep = {
			.name = "ep1in-bulk",
			.ops = &spearhead_ep_ops,
			.maxpacket = EP1_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP1_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 1,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[2] = {
		.ep = {
			.name = "ep2out-bulk",
			.ops = &spearhead_ep_ops,
			.maxpacket = EP2_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP2_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 0x02,   
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[3] = {
		.ep = {
			.name = "ep3in-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP3_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP3_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 3,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[4] = {
		.ep = {
			.name = "ep4out-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP4_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP4_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 4, 
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[5] = {
		.ep = {
			.name = "ep5in-int", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP5_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP5_IN_FIFO_SIZE,
		.addr = USB_DIR_IN  | 5, 
		.attrib = USB_ENDPOINT_XFER_INT,
	},
	.ep[6] = {
		.ep = {
			.name = "ep6out-iso", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP6_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP6_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 6,
		.attrib = USB_ENDPOINT_XFER_ISOC,
	},
	.ep[7] = {
		.ep = {
			.name = "ep7in-iso", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP7_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP7_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 7,
		.attrib = USB_ENDPOINT_XFER_ISOC,
	},
	.ep[8] = {
		.ep = {
			.name = "ep8out-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP8_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP8_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 8, 
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[9] = {
		.ep = {
			.name = "ep9in-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP9_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP9_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 9,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[10] = {
		.ep = {
			.name = "ep10out-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP10_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP10_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 10, 
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[11] = {
		.ep = {
			.name = "ep11in-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP11_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP11_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 11,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[12] = {
		.ep = {
			.name = "ep12out-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP12_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP12_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 12, 
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[13] = {
		.ep = {
			.name = "ep13in-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP13_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP13_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 13,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[14] = {
		.ep = {
			.name = "ep14out-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP14_OUT_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP14_OUT_FIFO_SIZE,
		.addr = USB_DIR_OUT | 14, 
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
	.ep[15] = {
		.ep = {
			.name = "ep15in-bulk", 
			.ops = &spearhead_ep_ops,
			.maxpacket = EP15_IN_MAXPCK_SIZE,
		},
		.dev = &memory,
		.fifo_size = EP15_IN_FIFO_SIZE,
		.addr = USB_DIR_IN | 15,
		.attrib = USB_ENDPOINT_XFER_BULK,
	},
};

static const char proc_node_name[] = "driver/synudc";

	static int
spearhead_proc_read (char *page, char **start, off_t off, int count,
		int *eof, void *_dev)
{
	char *buf = page;
	struct spearhead_dev *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int t;

	if (off != 0)
		return 0;

	local_irq_save (flags);

	/* basic device status */
	t = scnprintf (next, size, DRIVER_DESC "\n"
			"%s version: %s\nGadget driver: %s\n\n",
			driver_name, DRIVER_VERSION,
			dev->driver ? dev->driver->driver.name : "(none)");
	size -= t;
	next += t;


	local_irq_restore (flags);
	*eof = 1;
	return count - size;
}

static const char proc_node_regs_name[] = "driver/spearhead_regs";

	static int
spearhead_proc_regs_read (char *page, char **start, off_t off, int count,
		int *eof, void *_dev)
{
	char *buf = page;
	struct spearhead_dev *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int t;
	u32 *p;
	u8 *startp;

	if (off != 0)
		return 0;

	local_irq_save (flags);

	startp = (dev->csr_base + 0x500);
	p = (u32 *) startp;
	/* UDC regs show */
	while ((int)p < (int)startp + 0x40 && size > 0)
	{
		t = scnprintf (next, size, "%p: %08x %08x %08x %08x\n",
				p, p[0], p[1], p[2], p[3]);
		p += 4;
		size -= t;
		next += t;
	}

	local_irq_restore (flags);
	*eof = 1;
	return count - size;
}

static const char proc_node_endp_name[] = "driver/spearhead_endp";

	static int
spearhead_proc_endp_read (char *page, char **start, off_t off, int count,
		int *eof, void *_dev)
{
	char *buf = page;
	struct spearhead_dev *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int i, t;
	struct spearhead_glob_regs *glob = dev->glob_base;

	if (off != 0)
		return 0;

	local_irq_save (flags);


	t = scnprintf (next, size, "Config: %08x Control: %08x Status: %08x\n",
			readl (&glob->dev_conf),
			readl (&glob->dev_control), readl (&glob->dev_status));

	size -= t;
	next += t;

	t = scnprintf (next, size, "Dev.int = %08x Endp.int. = %08x\n",
			readl (&(dev->glob_base->dev_int)),
			readl (&(dev->glob_base->endp_int)));
	size -= t;
	next += t;


	for (i = 0; i < SYNUDC_NUM_ENDPOINTS; i++)
	{

		struct spearhead_ep *ep = &dev->ep[i];
		struct spearhead_epin_regs *in_regs;
		struct spearhead_epout_regs *out_regs;

		in_regs = ep->in_regs;
		out_regs = ep->out_regs;

		t = scnprintf (next, size, "Endp: %s\n\tIN REGS\n", ep->ep.name);
		size -= t;
		next += t;
		t = scnprintf (next, size,
				"\tControl: %08x Status: %08x Bufsize: %08x\n",
				readl (&in_regs->control), readl (&in_regs->status),
				readl (&in_regs->bufsize));
		size -= t;
		next += t;
		t = scnprintf (next, size, "\tMax packet: %08x desc_ptr: %08x\n",
				readl (&in_regs->max_pack_size),
				readl (&in_regs->desc_ptr));
		size -= t;
		next += t;
		t = scnprintf (next, size, "\tOUT REGS\n");
		size -= t;
		next += t;
		t = scnprintf (next, size,
				"\tControl: %08x Status: %08x Frame num: %08x\n",
				readl (&out_regs->control),
				readl (&out_regs->status),
				readl (&out_regs->frame_num));
		size -= t;
		next += t;
		t = scnprintf (next, size,
				"\tBufsize: %08x setup_ptr %08x desc_ptr: %08x\n\n",
				readl (&out_regs->bufsize),
				readl (&out_regs->setup_ptr),
				readl (&out_regs->desc_ptr));
		size -= t;
		next += t;
	}

	local_irq_restore (flags);
	*eof = 1;

	return count - size;
}

static const char proc_node_out_chain_name[] = "driver/spearhead_out_chain";

	static int
spearhead_proc_out_chain (char *page, char **start, off_t off, int count,
		int *eof, void *_dev)
{
	volatile struct spearhead_bulkd *tmp_desc;
	char *buf = page;
	struct spearhead_dev *dev = _dev;
	char *next = buf;
	unsigned size = count;
	unsigned long flags;
	int t;
	struct spearhead_ep *ep = &dev->ep[2];


	if (off != 0)
		return 0;

	local_irq_save (flags);

	t = scnprintf (next, size, "Out Chain for ep %s:\n", ep->ep.name);

	size -= t;
	next += t;

	tmp_desc = ep->desc_out_ptr;
	while (tmp_desc != 0x0)
	{

		t = scnprintf (next, size, "%08x %08x %08x %08x\n",
				tmp_desc->status, tmp_desc->reserved,
				tmp_desc->bufp, tmp_desc->nextd);

		size -= t;
		next += t;

		if (tmp_desc->status & DmaUsb_LastDescr)
			break;
		tmp_desc =
			list_entry (tmp_desc->desc_list.next, struct spearhead_bulkd,
					desc_list);
	}

	local_irq_restore (flags);
	*eof = 1;

	return count - size;
}

	static void
show_queues (struct spearhead_dev *dev)
{
	unsigned size;
	unsigned long flags;
	int i;

	size = PAGE_SIZE;
	spin_lock_irqsave (&dev->lock, flags);

	for (i = 0; i < 3; i++)
	{

		struct spearhead_ep *ep = &dev->ep[i];
		struct spearhead_request *req;
		int t;

		t = ep->addr;

		DBG (DBG_QUEUES, "%s (ep%d %s - %s)\n",
				ep->ep.name, t & USB_ENDPOINT_NUMBER_MASK,
				(t & USB_DIR_IN) ? "in" : "out", (
												  {
												  char *val;
												  switch (ep->
													  attrib &
													  0x03)
												  {
												  case USB_ENDPOINT_XFER_BULK:
												  val = "bulk"; break; case USB_ENDPOINT_XFER_INT:
												  val = "intr"; break; case USB_ENDPOINT_XFER_CONTROL:
												  val = "ctrl"; break; default:
												  val = "iso";
												  break;};
												  val;}
												 ));


		if (list_empty (&ep->queue))
		{
			DBG (DBG_QUEUES, "\t(nothing queued)\n");
			continue;
		}
		list_for_each_entry (req, &ep->queue, queue)
		{
			DBG (DBG_QUEUES,
					"\treq %p len %d/%d buf %p\n",
					&req->req, req->req.actual, req->req.length, req->req.buf);

		}

	}
	spin_unlock_irqrestore (&dev->lock, flags);
	return;
}

/*
* 	probe - binds to the platform device
*/
	static int __init
spearhead_probe (struct device *_dev)
{
	struct spearhead_dev *dev = &memory;
	int status = 0;
	SYNUDC_INFO ("Probing Device Synopsys UDC\n");
	spin_lock_init (&dev->lock);

	/* other non-static parts of init */

#ifdef CONFIG_ARCH_SPEARPLUS
	dev->csr_base = (__iomem u8 *) IO_ADDRESS (SPEARPLUS_START_USBD_CFG);
	dev->plug_base =
		(struct spearhead_plug_regs __iomem *) IO_ADDRESS (SPEARPLUS_START_USBD_PLUG);
	dev->fifo_base =
		(struct spearhead_fifo_regs __iomem *) IO_ADDRESS (SPEARPLUS_START_USBD_FIFO);

#endif

#ifdef CONFIG_ARCH_SPEARBASIC
	dev->csr_base = (__iomem u8 *) IO_ADDRESS (SPEARBASIC_START_USBD_CFG);
	dev->plug_base =
		(struct spearhead_plug_regs __iomem *) IO_ADDRESS (SPEARBASIC_START_USBD_PLUG);
	dev->fifo_base =
		(struct spearhead_fifo_regs __iomem *) IO_ADDRESS (SPEARBASIC_START_USBD_FIFO);

#endif

	dev->epin_base = (struct spearhead_epin_regs __iomem *) (dev->csr_base);
	dev->epout_base =
		(struct spearhead_epout_regs __iomem *) (dev->csr_base + 0x200);
	dev->glob_base =
		(struct spearhead_glob_regs __iomem *) (dev->csr_base + 0x400);
	SYNUDC_INFO ("csr %p: plug %p, fifo %p\n", dev->csr_base, dev->plug_base,
			dev->fifo_base);

	dev->dev = _dev;
	device_initialize (&dev->gadget.dev);
	dev->gadget.dev.parent = _dev;
	dev->gadget.dev.dma_mask = _dev->dma_mask;
	dev->gadget.is_dualspeed = 1;
	dev->int_cmd = 0;

	/* LINUX_TBD: Eliminate the_controller from the driver */
	the_controller = dev;
	dev_set_drvdata (_dev, dev);

	/* Restore a known hardware state */
	spearhead_disable (dev);
	spearhead_reinit (dev);

	status =
		request_irq (IRQ_USB_DEV, spearhead_irq, 0, driver_name, dev);
	if (status != 0)
	{
		SYNUDC_ERR ("%s: can't get irq %d, err %d\n",
				driver_name, IRQ_USB_DEV, status); 
		status = -EBUSY;
		return status;
	}
	/* LINUX_TBD: check if got_irq is really necessary */
	dev->got_irq = 1;

	{
		struct spearhead_ep *ep = &dev->ep[0];

		ep->in_regs = &dev->epin_base[0];
		ep->out_regs = &dev->epout_base[0];
	}


	{
		struct spearhead_ep *ep = &dev->ep[1];

		ep->in_regs = &dev->epin_base[1];
		ep->out_regs = &dev->epout_base[1];
	}

	{
		struct spearhead_ep *ep = &dev->ep[2];

		ep->in_regs = &dev->epin_base[2];  
		ep->out_regs = &dev->epout_base[2];
	}

	{
		struct spearhead_ep *ep = &dev->ep[3];

		ep->in_regs = &dev->epin_base[3];  
		ep->out_regs = &dev->epout_base[3];
	}


	{
		struct spearhead_ep *ep = &dev->ep[4];

		ep->in_regs = &dev->epin_base[4];  
		ep->out_regs = &dev->epout_base[4];
	}

	{
		struct spearhead_ep *ep = &dev->ep[5];

		ep->in_regs = &dev->epin_base[5];  
		ep->out_regs = &dev->epout_base[5];
	}
	{
		struct spearhead_ep *ep = &dev->ep[6];

		ep->in_regs = &dev->epin_base[6];  
		ep->out_regs = &dev->epout_base[6];
	}
	{
		struct spearhead_ep *ep = &dev->ep[7];

		ep->in_regs = &dev->epin_base[7];  
		ep->out_regs = &dev->epout_base[7];
	}
	{
		struct spearhead_ep *ep = &dev->ep[8];

		ep->in_regs = &dev->epin_base[8];  
		ep->out_regs = &dev->epout_base[8];
	}
	{
		struct spearhead_ep *ep = &dev->ep[9];

		ep->in_regs = &dev->epin_base[9];  
		ep->out_regs = &dev->epout_base[9];
	}
	{
		struct spearhead_ep *ep = &dev->ep[10];

		ep->in_regs = &dev->epin_base[10];  
		ep->out_regs = &dev->epout_base[10];
	}
	{
		struct spearhead_ep *ep = &dev->ep[11];

		ep->in_regs = &dev->epin_base[11];  
		ep->out_regs = &dev->epout_base[11];
	}
	{
		struct spearhead_ep *ep = &dev->ep[12];

		ep->in_regs = &dev->epin_base[12];  
		ep->out_regs = &dev->epout_base[12];
	}
	{
		struct spearhead_ep *ep = &dev->ep[13];

		ep->in_regs = &dev->epin_base[13];  
		ep->out_regs = &dev->epout_base[13];
	}
	{
		struct spearhead_ep *ep = &dev->ep[14];

		ep->in_regs = &dev->epin_base[14];  
		ep->out_regs = &dev->epout_base[14];
	}
	{
		struct spearhead_ep *ep = &dev->ep[15];

		ep->in_regs = &dev->epin_base[15];  
		ep->out_regs = &dev->epout_base[15];
	}


	status = desc_pool_init (dev);
	if (status != 0)
	{
		desc_pool_remove (dev);
		return -1;
	}

	create_proc_files ();

	SYNUDC_INFO ("Device Synopsys UDC successfully probed\n");
	return 0;

}


	static int __exit
spearhead_remove (struct device *_dev)
{
	struct spearhead_dev *dev = _dev->driver_data;

	DBG_ENTRY;

	spearhead_disable (dev);
	desc_pool_remove (dev);

	remove_proc_files ();
	usb_gadget_unregister_driver (dev->driver);

	if (dev->got_irq)
	{
		free_irq (IRQ_USB_DEV, dev);
		dev->got_irq = 0;
	}

	dev_set_drvdata (_dev, 0);
	the_controller = 0;

	return 0;
}


static struct device_driver spearhead_driver = {
	.name = (char *) driver_name,
	.bus = &platform_bus_type,
	.probe = spearhead_probe,
	.remove = __exit_p (spearhead_remove),
};

	static int __init
spearhead_init (void)
{
	int status;

	printk ("Loading %s: version %s\n", driver_name, DRIVER_VERSION);
	status = driver_register (&spearhead_driver);
	return status;
}

module_init (spearhead_init);

	static void __exit
spearhead_exit (void)
{
	printk ("Removing %s: version %s\n", driver_name, DRIVER_VERSION);
	driver_unregister (&spearhead_driver);
}

module_exit (spearhead_exit);

MODULE_DESCRIPTION (DRIVER_DESC);
MODULE_AUTHOR ("Rajeev Kumar");
MODULE_LICENSE ("GPL");
