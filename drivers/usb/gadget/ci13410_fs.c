/*
 * Driver for the CHIPIDEA CI13410-A USB device controller.
 * Specs are available from <http://www.chipidea.com>.
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
#ifndef LINUX_VERSION_CODE
  #include <linux/version.h>
#endif
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp_lock.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/usb.h>
#include <linux/usb_gadget.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>
#if defined(CONFIG_MIPS_VSC452)
  #include <asm/mach-vsc452/vsc452int.h>
#endif
#include <linux/proc_fs.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,21)
  #include <linux/config.h>
  #include <linux/usb_ch9.h>
  #include "../input/hid.h"
#else
  #include <linux/usb/ch9.h>
  #include <linux/hid.h>
#endif

#include <linux/dma-mapping.h>

//#define CI13410_HIGH_SPEED_DRIVER
//#define STREAM_ENABLE
#include "ci13410.h"

static struct {
	int	full_speed;
} mod_data = {					// Default values
	.full_speed = 0,
	};

module_param_named(fullspeed, mod_data.full_speed, bool, S_IRUGO);
MODULE_PARM_DESC(fullspeed, "True to Force Full Speed operation");

static int force_full_speed(void)
{
	return (mod_data.full_speed);
}

static const char *ci13410_ep_name [] = {
	"ep0",
	"ep1out", "ep1in", "ep2out", "ep2in", "ep3out", "ep3in",
	"ep4out", "ep4in", "ep5out", "ep5in", "ep6out", "ep6in",
	"ep7out", "ep7in", "ep8out", "ep8in", "ep9out", "ep9in",
	"ep10out", "ep10in", "ep11out", "ep11in", "ep12out", "ep12in",
	"ep13out", "ep13in", "ep14out", "ep14in", "ep15out", "ep15in",
};

struct usb_ctrlrequest g_setupPacket;
	
/****************************************************************************/
static struct ci13410 *the_controller;
/*************************Memory Allocation**********************************/
static void * usb_memalloc(u32 n)
{
	u8      *aligned_ptr;
	u32     *temp;
	void    *unaligned_ptr = kmalloc(USB_CACHE_ALIGN(n) +
						PSP_CACHE_LINE_SIZE*2,
						GFP_KERNEL);
	if (!unaligned_ptr) {
		return NULL;
	}	
	
	aligned_ptr =  (u8*) USB_CACHE_ALIGN( (u32) unaligned_ptr);
	temp = (u32*) (aligned_ptr);
	(*temp) = (u32) unaligned_ptr;
		
	return (aligned_ptr + PSP_CACHE_LINE_SIZE);
}

static void  usb_memfree(void * aligned_ptr)
{
	u32* temp = (u32*) ((u8*)aligned_ptr - PSP_CACHE_LINE_SIZE);		
	void * unaligned_ptr = (void *) (*temp);
	
	if (unaligned_ptr) {
		kfree(unaligned_ptr);
	}
	return;
}

static u32 dTD_dma_to_virt(u32 virtBase, u32 dmaBase, u32 dma)
{
	u32 dmaOffset;
	u32 virtOffset;
	
	dmaOffset = dma - dmaBase;
	
	virtOffset = virtBase + dmaOffset;
	
	return virtOffset;
}
/*************************CPU register settings******************************/
#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
static void select_pll1_clk(void)
{
	ci13410_cpu_regs         __iomem *clk_reg_ptr;
	u32                      regVal;

	clk_reg_ptr = (ci13410_cpu_regs __iomem *)the_controller->
								clk_reg_ptr;
		
	regVal =
	ioread32(&clk_reg_ptr->registers.clk_regs.clksel);
	regVal &= (~(0x3<<6));
	regVal |= (0x1<<6);
	iowrite32(regVal,
		&clk_reg_ptr->registers.clk_regs.clksel);
}

static void set_usb_insertion_high(void)
{
	ci13410_cpu_regs         __iomem *gcr_reg_ptr;
	u32                      regVal;

	gcr_reg_ptr = (ci13410_cpu_regs __iomem *)the_controller->
								gcr_reg_ptr;
		
	regVal =
	ioread32(&gcr_reg_ptr->registers.gcr_regs.mfsel1);
	regVal |= (1<<23);
	iowrite32(regVal,
		&gcr_reg_ptr->registers.gcr_regs.mfsel1);
		
	regVal =
	ioread32(&gcr_reg_ptr->registers.gcr_regs.intcr);
	regVal |= (0x3<<13);
	iowrite32(regVal,
		&gcr_reg_ptr->registers.gcr_regs.intcr);
}

static void set_usb_insertion_float(void)
{
	ci13410_cpu_regs         __iomem *gcr_reg_ptr;
	u32                      regVal;

	gcr_reg_ptr = (ci13410_cpu_regs __iomem *)the_controller->
								gcr_reg_ptr;
		
	regVal =
	ioread32(&gcr_reg_ptr->registers.gcr_regs.mfsel1);
	regVal |= (1<<23);
	iowrite32(regVal,
		&gcr_reg_ptr->registers.gcr_regs.mfsel1);
		
	regVal =
	ioread32(&gcr_reg_ptr->registers.gcr_regs.intcr);
	regVal &= (~(0x3<<13));
	iowrite32(regVal,
		&gcr_reg_ptr->registers.gcr_regs.intcr);
}
#endif
/****************************************************************************/
/***********************Hardware Control Functions***************************/
static void _usb_device_free_XD(void *xd_ptr)
{
	u8              bDebug = FALSE;
	struct ci13410  *ci13410 = (struct ci13410*)
		(((struct ci13410_xd*)xd_ptr)->scratch_ptr->private);

	if (bDebug) printk(KERN_DEBUG "_usb_device_free_XD()- xd_ptr = 0x%08X\n",
				(int)xd_ptr);	

	/*
	** Add the XD to the free XD queue (linked via PRIVATE) and
	** increment the tail to the next descriptor
	*/
	USB_XD_QADD(ci13410->xd_head, ci13410->xd_tail,
			(struct ci13410_xd*)xd_ptr);
				
	ci13410->xd_entries++;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_free_XD(), SUCCESSFUL\n");
}

static void _usb_device_free_dTD(void *dTD_ptr)
{
	u8              bDebug = FALSE;
	struct ci13410  *ci13410 = (struct ci13410*)
		(((ci13410_ep_tr*)dTD_ptr)->scratch_ptr->private);

	if (bDebug) printk(KERN_DEBUG "_usb_device_free_dTD() - dTD_ptr = 0x%08X\n",
				(int)dTD_ptr);

	/*
	** Add the dTD to the free dTD queue (linked via PRIVATE) and
	** increment the tail to the next descriptor
	*/
	EHCI_DTD_QADD(ci13410->dtd_head, ci13410->dtd_tail,
			(ci13410_ep_tr*)dTD_ptr);
	ci13410->dtd_entries++;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_free_dTD(), SUCCESSFUL\n");
}

static void _usb_device_init(struct ci13410 *ci13410)
{
	u8                              bDebug = FALSE;
	u8                              temp;
	u32                             total_memory=0;
	void                            *driver_memory;
	dma_addr_t                      dma_driver_memory = DMA_ADDR_INVALID;

	if (bDebug) printk(KERN_DEBUG "_usb_device_init()\n");
		
	// Set state	
	ci13410->usb_state = USB_STATE_UNKNOWN;
   
	temp = (ci13410->max_endpoints * 2);
	
	/* Allocate MAX_XDS_FOR_TR_CALLS */
	ci13410->xd_base =
		(struct ci13410_xd*)usb_memalloc(sizeof(struct ci13410_xd)
			* MAX_XDS_FOR_TR_CALLS);
	
	if (ci13410->xd_base == NULL) {
		if (bDebug) printk(KERN_ERR "_usb_device_init() USB_memalloc"
					" failed for MAX_XDS_FOR_TR_CALLS\n");
		return;
	}

	memset(ci13410->xd_base, 0, sizeof(struct ci13410_xd)
		* MAX_XDS_FOR_TR_CALLS);

	/* Allocate memory for internal scratch structure */   
	ci13410->xd_scratch_struct_base = (struct ci13410_scratch_struct*)
		usb_memalloc(sizeof(struct ci13410_scratch_struct)
		* MAX_XDS_FOR_TR_CALLS);
   
	if (ci13410->xd_scratch_struct_base == NULL) {
		if (bDebug) printk(KERN_ERR "_usb_device_init() USB_memalloc failed"
					" for XD_SCRATCH_STRUCT_BASE\n");
		return;
	}

	ci13410->temp_xd_ptr =
		(struct ci13410_xd*)usb_memalloc(sizeof(struct ci13410_xd));
	
	if (ci13410->temp_xd_ptr == NULL) {
		if (bDebug) printk(KERN_ERR "_usb_device_init() USB_memalloc"
					" failed for TEMP_XD_PTR\n");
		return;		
	}
	
	memset(ci13410->temp_xd_ptr, 0, sizeof(struct ci13410_xd));

	
	/****************************************************************   
	Consolidated memory allocation    
	****************************************************************/   
	total_memory =
		((temp * sizeof(ci13410_ep_queue_head)) + 2048)
		+ ((MAX_EP_TR_DESCRS * sizeof(ci13410_ep_tr)) + 32)
		+ (sizeof(struct ci13410_scratch_struct) * MAX_EP_TR_DESCRS);

   	ci13410->total_memory = total_memory;
					
 	driver_memory = dma_alloc_coherent(the_controller->gadget.dev.parent,
				total_memory, &dma_driver_memory, GFP_KERNEL);
				
	if (driver_memory == NULL) {
		if (bDebug) printk(KERN_ERR "_usb_device_init()"
					" memalloc failed for driver_memory\n");
		return;
	}
	
	/****************************************************************   
	Zero out the memory allocated
	****************************************************************/   
	memset((void *) driver_memory, 0, total_memory );

	/****************************************************************   
	Keep a pointer to driver memory alloctaion
	****************************************************************/   
	ci13410->driver_memory = (u8*)driver_memory;
	ci13410->dma_driver_memory = dma_driver_memory;
                    
	/****************************************************************   
	Assign QH base
	****************************************************************/   
	ci13410->ep_queue_head_base =
		(ci13410_ep_queue_head*)driver_memory;
		
	ci13410->dma_ep_queue_head_base =
		(ci13410_ep_queue_head*)dma_driver_memory;		
	
	driver_memory += ((temp * sizeof(ci13410_ep_queue_head)) + 2048);
	dma_driver_memory += ((temp * sizeof(ci13410_ep_queue_head)) + 2048);

	/* Align the endpoint queue head to 2K boundary */   
	ci13410->ep_queue_head_ptr = (ci13410_ep_queue_head*)
		USB_MEM2048_ALIGN((u32)ci13410->ep_queue_head_base);
	ci13410->dma_ep_queue_head_ptr = (ci13410_ep_queue_head*)
		USB_MEM2048_ALIGN((u32)ci13410->dma_ep_queue_head_base);		
	
                                     
	/****************************************************************   
	Assign DTD base
	****************************************************************/   
	ci13410->dtd_base_ptr = (ci13410_ep_tr*) driver_memory;
	ci13410->dma_dtd_base_ptr = (ci13410_ep_tr*) dma_driver_memory;
   
	driver_memory +=
		((MAX_EP_TR_DESCRS * sizeof(ci13410_ep_tr)) + 32);
	dma_driver_memory +=
		((MAX_EP_TR_DESCRS * sizeof(ci13410_ep_tr)) + 32);		
    
	/* Align the dTD base to 32 byte boundary */   
	ci13410->dtd_aligned_base_ptr = (ci13410_ep_tr*)
			USB_MEM32_ALIGN((u32)ci13410->dtd_base_ptr);
	/* Align the dTD base to 32 byte boundary */   
	ci13410->dma_dtd_aligned_base_ptr = (ci13410_ep_tr*)
			USB_MEM32_ALIGN((u32)ci13410->dma_dtd_base_ptr);			
	
	/****************************************************************   
		Assign SCRATCH Structure base
	****************************************************************/   
	/* Allocate memory for internal scratch structure */   
	ci13410->scratch_struct_base =
		(struct ci13410_scratch_struct*) driver_memory;
		
   
	if (bDebug) printk(KERN_DEBUG "_usb_device_init() SUCCESSFUL\n");
		
	return;
}

static void _usb_device_chip_initialize(struct ci13410	*ci13410)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *id_reg_ptr;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_usb_regs                __iomem *cap_reg_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	struct ci13410_xd               *xd_ptr;
	ci13410_ep_tr                   *dTD_ptr;
	ci13410_ep_tr                   *dma_dTD_ptr;
	u32                             i, port_control;
	struct ci13410_scratch_struct   *temp_scratch_ptr;
	u32                             regVal;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_chip_initialize()\n");
	
	id_reg_ptr = (ci13410_usb_regs __iomem *)
		ci13410->id_reg_ptr;
	op_reg_ptr = (ci13410_usb_regs __iomem *)
		ci13410->op_dev_regs_ptr;
	cap_reg_ptr = (ci13410_usb_regs __iomem *)
		ci13410->cap_reg_ptr;
   
	/* Stop the controller */
	regVal =
	ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd);
	regVal &= ~EHCI_CMD_RUN_STOP;
	iowrite32(regVal,
		&op_reg_ptr->registers.op_dev_regs.usb_cmd);
		
#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	select_pll1_clk();
	set_usb_insertion_float();	
#endif
      
	/* Reset the controller to get default values */
	iowrite32(EHCI_CMD_CTRL_RESET,
		&op_reg_ptr->registers.op_dev_regs.usb_cmd);

	while (ioread32
		(&op_reg_ptr->registers.op_dev_regs.usb_cmd)
		& EHCI_CMD_CTRL_RESET) {
		/* Wait for the controller reset to complete */
	}
	
#ifdef TRIP_WIRE
	/* Program the controller to be the USB device controller */
	regVal = VUSBHS_MODE_CTRL_MODE_DEV | VUSBHS_MODE_SETUP_LOCK_DISABLE; 
#else
	/* Program the controller to be the USB device controller */ 
	regVal = VUSBHS_MODE_CTRL_MODE_DEV;
#endif
	
#if defined(CONFIG_MIPS_VSC452)
	regVal |= VUSBHS_MODE_BIG_ENDIAN;
#endif
	iowrite32(regVal,
		&op_reg_ptr->registers.op_dev_regs.usb_mode);

	/* Make sure the 16 MSBs of this register are 0s */
	iowrite32(0,
	&op_reg_ptr->registers.op_dev_regs.endpt_setup_stat);
   
	ep_queue_head_ptr = ci13410->ep_queue_head_ptr;

	/* Initialize all device queue heads */
	for (i = 0; i < (ci13410->max_endpoints * 2); i++) {
		/* Interrupt on Setup packet */
		(ep_queue_head_ptr + i)->max_pkt_length	= 
		 (((u32)USB_MAX_CTRL_PAYLOAD 
			<< VUSB_EP_QUEUE_HEAD_MAX_PKT_LEN_POS)
			| VUSB_EP_QUEUE_HEAD_IOS);
		
		(ep_queue_head_ptr + i)->next_dtd_ptr =
			VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;
	}   
   
	/* Configure the Endpoint List Address */
	iowrite32((u32)(ci13410->dma_ep_queue_head_ptr),
		&op_reg_ptr->registers.op_dev_regs.ep_list_addr);
   
	if (ioread32(&cap_reg_ptr->registers.cap_regs.hcs_params)
			& VUSB20_HCS_PARAMS_PORT_POWER_CONTROL_FLAG) {
		port_control =
		ioread32(&op_reg_ptr->registers.op_dev_regs.portscx[0]);
		port_control &=
		(~EHCI_PORTSCX_W1C_BITS | ~EHCI_PORTSCX_PORT_POWER);
		iowrite32(port_control,
			&op_reg_ptr->registers.op_dev_regs.portscx[0]);	
	}
	
	if (force_full_speed()) {
		printk(KERN_ALERT "_usb_device_chip_initialize()"
			" Forcing Full Speed Operation\n");
		port_control =
		ioread32(&op_reg_ptr->registers.op_dev_regs.portscx[0]);	
			port_control |= EHCI_PORTSCX_FORCE_FULL_SPEED_CONNECT;
			iowrite32(port_control,
			&op_reg_ptr->registers.op_dev_regs.portscx[0]);
	}
	
	ci13410->xd_head        = NULL;
	ci13410->xd_tail        = NULL;
	ci13410->xd_entries     = 0;
	
	xd_ptr                  = ci13410->xd_base;
	temp_scratch_ptr        = ci13410->xd_scratch_struct_base;
   
	/* Enqueue all the XDs */   
	for (i = 0;i < MAX_XDS_FOR_TR_CALLS; i++) {
		xd_ptr->scratch_ptr = temp_scratch_ptr;
		xd_ptr->scratch_ptr->free = _usb_device_free_XD;
		xd_ptr->scratch_ptr->private = (void*)ci13410;
		_usb_device_free_XD((void*)xd_ptr);
		xd_ptr++;
		temp_scratch_ptr++;
	}
	
	/* Initialize the internal dTD head and tail to NULL */   
	ci13410->dtd_head    = NULL;
	ci13410->dtd_tail    = NULL;
	ci13410->dtd_entries = 0;	
   
	dTD_ptr              = ci13410->dtd_aligned_base_ptr;
	dma_dTD_ptr          = ci13410->dma_dtd_aligned_base_ptr;

	temp_scratch_ptr     = ci13410->scratch_struct_base;

	/* Enqueue all the dTDs */   
	for (i = 0; i < MAX_EP_TR_DESCRS; i++) {
		dTD_ptr->scratch_ptr = temp_scratch_ptr;
		dTD_ptr->scratch_ptr->free = _usb_device_free_dTD;
		/* Set the dTD to be invalid */
		dTD_ptr->next_tr_elem_ptr = VUSBHS_TD_NEXT_TERMINATE;
		/* Set the Reserved fields to 0 */
		dTD_ptr->size_ioc_sts &= ~VUSBHS_TD_RESERVED_FIELDS;
		dTD_ptr->scratch_ptr->private = (void*)ci13410;
		dTD_ptr->scratch_ptr->dma = (void*)dma_dTD_ptr;
		_usb_device_free_dTD((void*)dTD_ptr);
		dTD_ptr++;
		dma_dTD_ptr++;
		temp_scratch_ptr++;
	}

// Workaround for Thidwick
#if defined(CONFIG_ARCH_WPCM450) && !defined (CI13410_HIGH_SPEED_DRIVER)
	/* Initialize the endpoint 0 properties */
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.endptctrlx[0]);
	regVal &= ~(EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL);
	iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.endptctrlx[0]);
#endif

	/* Enable Interrupts */
	iowrite32((EHCI_INTR_INT_EN
			| EHCI_INTR_PORT_CHANGE_DETECT_EN
			| EHCI_INTR_RESET_EN),
			&op_reg_ptr->registers.op_dev_regs.usb_intr);

	ci13410->usb_state = USB_STATE_UNKNOWN;
 
#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	set_usb_insertion_high();	
#endif

#if defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	// Set System Bus Configuration Register to 0x00000002
	iowrite32(0x00000002,
		&id_reg_ptr->registers.id_regs.sys_bus_config);
#endif

	/* Set the Run bit in the command register */
	iowrite32(EHCI_CMD_RUN_STOP,
		&op_reg_ptr->registers.op_dev_regs.usb_cmd);
   
	if (bDebug) printk(KERN_DEBUG "_usb_device_chip_initialize(),"
							" SUCCESSFUL\n");
}

static u8 _usb_device_cancel_transfer(struct ci13410 *ci13410, u8 ep_num,
							u8 direction)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_tr                   *dTD_ptr, *check_dTD_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	struct ci13410_xd               *xd_ptr;
	u32                             temp, bit_pos;

	if (bDebug) printk(KERN_DEBUG "_usb_device_cancel_transfer()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	bit_pos = (1 << (16 * direction + ep_num));
	temp = (2*ep_num + direction);

	ep_queue_head_ptr = ci13410->ep_queue_head_ptr + temp;		
   
	/* Unlink the dTD */
	dTD_ptr = ci13410->ep_dtd_heads[temp];
   
	if (dTD_ptr) {
		check_dTD_ptr =
			(ci13410_ep_tr*)((u32)dTD_ptr->next_tr_elem_ptr
							& VUSBHS_TD_ADDR_MASK);

		if (check_dTD_ptr)
			check_dTD_ptr =
				(ci13410_ep_tr*)((u32)
				dTD_dma_to_virt((u32)ci13410->dtd_aligned_base_ptr,
					    (u32)ci13410->dma_dtd_aligned_base_ptr,
					    dTD_ptr->next_tr_elem_ptr)
				& VUSBHS_TD_ADDR_MASK);
		
		if (dTD_ptr->size_ioc_sts & VUSBHS_TD_STATUS_ACTIVE) {
			/* Flushing will halt the pipe */
			/* Write 1 to the Flush register */
			iowrite32(bit_pos,
				&op_reg_ptr->registers.op_dev_regs.endptflush);

			/* Wait until flushing completed */
			while (ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptflush) & bit_pos) {
				/* ENDPTFLUSH bit should
				** be cleared to indicate this 
				** operation is complete 
				*/
			}
			
			while (ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptstatus) & bit_pos) {
				/* Write 1 to the Flush register */
				iowrite32(bit_pos,
					&op_reg_ptr->registers
					.op_dev_regs.endptflush);
         
				/* Wait until flushing completed */
				while (ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptflush) & bit_pos) {
					/* ENDPTFLUSH bit should 
					** be cleared to indicate this 
					** operation is complete 
					*/
				}
			}
		}
      
		/* Retire the current dTD */
		dTD_ptr->size_ioc_sts = 0;
		dTD_ptr->next_tr_elem_ptr = VUSBHS_TD_NEXT_TERMINATE;
		
		/* The transfer descriptor for this dTD */
		xd_ptr = (struct ci13410_xd*)
			(u32)dTD_ptr->scratch_ptr->xd_for_this_dtd;
		
		dTD_ptr->scratch_ptr->private = (void*)ci13410;
		/* Free the dTD */
		_usb_device_free_dTD((void*)dTD_ptr);
      
		/* Update the dTD head and tail
		** for specific endpoint/direction
		*/
		if (!check_dTD_ptr) {
			ci13410->ep_dtd_heads[temp] = NULL;
			ci13410->ep_dtd_tails[temp] = NULL;
			if (xd_ptr) {
				xd_ptr->scratch_ptr->private = (void*)ci13410;
				/* Free the transfer descriptor */
				_usb_device_free_XD((void*)xd_ptr);
			}
			/* No other transfers on the queue */
			ep_queue_head_ptr->next_dtd_ptr =
					VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;
			ep_queue_head_ptr->size_ioc_int_sts = 0;
		} else {
			ci13410->ep_dtd_heads[temp] = check_dTD_ptr;			
            
			if (xd_ptr) {
				if ((u32)(u32)check_dTD_ptr->scratch_ptr
					->xd_for_this_dtd != (u32)xd_ptr) {
					xd_ptr->scratch_ptr->private =
						(void*)ci13410;
					/* Free the transfer descriptor */
					_usb_device_free_XD((void*)xd_ptr);
				}
			}
         
			if (check_dTD_ptr->size_ioc_sts
				& VUSBHS_TD_STATUS_ACTIVE) {
				/* Start CR 1015 */
				/* Prime the Endpoint */
				iowrite32(bit_pos,
					&op_reg_ptr->registers
					.op_dev_regs.endptprime);

				if (!(ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptstatus) & bit_pos)) {
					while (ioread32(&op_reg_ptr->registers
						.op_dev_regs.endptprime)
						& bit_pos) {
						/* Wait for the ENDPTPRIME to
						** go to zero
						*/
					}

					if (ioread32(&op_reg_ptr->registers
						.op_dev_regs.endptstatus)
						& bit_pos) {
						/* The endpoint was not not
						** primed so no other transfers 
						** on the queue 
						*/
						goto done;
					}
				} else {
					goto done;
				}

				/* No other transfers on the queue */
				ep_queue_head_ptr->next_dtd_ptr =
					(u32)(check_dTD_ptr->scratch_ptr->dma);
				ep_queue_head_ptr->size_ioc_int_sts = 0;
   
				/* Prime the Endpoint */
				iowrite32(bit_pos,
					&op_reg_ptr->registers
					.op_dev_regs.endptprime);
			}
		}
	}
   
done:
	if (bDebug) printk(KERN_DEBUG "_usb_device_cancel_transfer(),"
							" SUCCESSFUL\n");
	return USB_OK;

}

static u8 _usb_device_init_endpoint(struct ci13410 *ci13410,
                             u8 ep_num,
                             u16 max_pkt_size,
                             u8 direction,
                             u8 type,
                             u8 flag)
{
	u8                              bDebug = FALSE;
	u8                              error  = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	struct ci13410_xd               *xd_ptr;
	u32                             bit_pos;
	u32                             regVal;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_init_endpoint()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	/* Initialize the transfer descriptor */
	ci13410->temp_xd_ptr->ep_num                   = ep_num;
	ci13410->temp_xd_ptr->bdirection               = direction;
	ci13410->temp_xd_ptr->wmaxpacketsize           = max_pkt_size;
	ci13410->temp_xd_ptr->ep_type                  = type;
	ci13410->temp_xd_ptr->dont_zero_terminate      = flag;
	ci13410->temp_xd_ptr->max_pkts_per_uframe      = ((flag
					& USB_MAX_PKTS_PER_UFRAME) >> 1);
	
	xd_ptr = ci13410->temp_xd_ptr;
	
	/* Get the endpoint queue head address */
	ep_queue_head_ptr =
		(ci13410_ep_queue_head*)
		ci13410->ep_queue_head_ptr + 2*xd_ptr->ep_num
		+ xd_ptr->bdirection;
      
	bit_pos = (1 << (16 * xd_ptr->bdirection + xd_ptr->ep_num));
   
	/* Check if the Endpoint is Primed */
	if ((!(ioread32(&op_reg_ptr->registers
		.op_dev_regs.endptprime) & bit_pos)) && 
		(!(ioread32(&op_reg_ptr->registers
		.op_dev_regs.endptstatus) & bit_pos))) { 
		/* Set the max packet length,
		** interrupt on Setup and Mult fields
		*/
		if (xd_ptr->ep_type == USB_ISOCHRONOUS_ENDPOINT) {
			/* Mult bit should be set for isochronous endpoints */
			ep_queue_head_ptr->max_pkt_length =
				((xd_ptr->wmaxpacketsize << 16)
				| ((xd_ptr->max_pkts_per_uframe ? 
				xd_ptr->max_pkts_per_uframe : 1)
				<< VUSB_EP_QUEUE_HEAD_MULT_POS));
		} else {
			if (xd_ptr->ep_type != USB_CONTROL_ENDPOINT) {
				ep_queue_head_ptr->max_pkt_length =
				((xd_ptr->wmaxpacketsize << 16)
				| (xd_ptr->dont_zero_terminate ?
				VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL : 0));
			} else {
				ep_queue_head_ptr->max_pkt_length =
				((xd_ptr->wmaxpacketsize << 16)
				| VUSB_EP_QUEUE_HEAD_IOS
				| (xd_ptr->dont_zero_terminate ?
				VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL : 0));
			}
		}
     	  
		/* Enable the endpoint for Rx and Tx 
		** and set the endpoint type
		*/
		regVal = ioread32(&op_reg_ptr->registers
				.op_dev_regs.endptctrlx[xd_ptr->ep_num]);
		if (xd_ptr->ep_num > 0) {
			regVal |= 
				((xd_ptr->bdirection ? (EHCI_EPCTRL_TX_ENABLE | 
				EHCI_EPCTRL_TX_DATA_TOGGLE_RST) : 
				(EHCI_EPCTRL_RX_ENABLE 
				| EHCI_EPCTRL_RX_DATA_TOGGLE_RST)) | 
				(xd_ptr->ep_type << (xd_ptr->bdirection ? 
				EHCI_EPCTRL_TX_EP_TYPE_SHIFT
				: EHCI_EPCTRL_RX_EP_TYPE_SHIFT)));
		} else {
			// No data toggle on ep0
			regVal |= 
				((xd_ptr->bdirection ? (EHCI_EPCTRL_TX_ENABLE) : 
				(EHCI_EPCTRL_RX_ENABLE)) | 
				(xd_ptr->ep_type << (xd_ptr->bdirection ? 
				EHCI_EPCTRL_TX_EP_TYPE_SHIFT
				: EHCI_EPCTRL_RX_EP_TYPE_SHIFT)));
		}

// Workaround for Thidwick machines
#if defined(CONFIG_ARCH_WPCM450) && defined (CI13410_HIGH_SPEED_DRIVER)		
		if (xd_ptr->ep_num > 0)
#endif
		{
			iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.
						endptctrlx[xd_ptr->ep_num]);
		}
	} else {
		if (bDebug) printk(KERN_ERR "_usb_device_init_endpoint()"
						" error ep init\n");
		
		return USBERR_EP_INIT_FAILED;
	}
    
	if (bDebug) printk(KERN_DEBUG "_usb_device_init_endpoint(),"
							" SUCCESSFUL\n");	

	return error;
}

static u8 _usb_device_deinit_endpoint(struct ci13410 *ci13410, u8 ep_num,
						u8 direction)
{
	u8                              bDebug	= FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	u32                             bit_pos;
	u32                             regVal;

	if (bDebug) printk(KERN_DEBUG "_usb_device_deinit_endpoint()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	/* Get the endpoint queue head address */
	ep_queue_head_ptr =
		(ci13410_ep_queue_head*)
		ci13410->ep_queue_head_ptr + 2*ep_num + direction;	
      
	bit_pos = (1 << (16 * direction + ep_num));
      
	/* Check if the Endpoint is Primed */ 
	if ((!(ioread32(&op_reg_ptr->registers
		.op_dev_regs.endptprime) & bit_pos)) && 
      		(!(ioread32(&op_reg_ptr->registers
		.op_dev_regs.endptstatus) & bit_pos)))	{ 
		/* Reset the max packet length and the interrupt on Setup */
		ep_queue_head_ptr->max_pkt_length = 0;
      
		/* Disable the endpoint for Rx
		** or Tx and reset the endpoint type
		*/
		regVal = ioread32(&op_reg_ptr->registers
				.op_dev_regs.endptctrlx[ep_num]);
		regVal &= ((direction ? ~EHCI_EPCTRL_TX_ENABLE
				: ~EHCI_EPCTRL_RX_ENABLE)
				| (direction ? ~EHCI_EPCTRL_TX_TYPE
				: ~EHCI_EPCTRL_RX_TYPE));		
			
		iowrite32(regVal, &op_reg_ptr->registers
				.op_dev_regs.endptctrlx[ep_num]);
	} else { 
		if (bDebug) printk(KERN_ERR "_usb_device_deinit_endpoint()"
					" : error deinit failed\n");
		return USBERR_EP_DEINIT_FAILED;
	}
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_deinit_endpoint(),"
							" SUCCESSFUL\n");	
      
	return USB_OK;
}

static void nuke(struct ci13410_ep *ep);
static void _usb_device_get_setup_data(struct ci13410 *ci13410,
                                u8 ep_num,
                                u8* buffer_ptr)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	int                             nLoop;
	u32                             ulSetup, ulBufLen;
	u32                             *pSetupBuffer;
	u8                              setupBuffer[8];
	u32                             regVal;
#ifdef TRIP_WIRE
	u8                              read_safe;
#endif
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_get_setup_data()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	/* Get the endpoint queue head */
	ep_queue_head_ptr = (ci13410_ep_queue_head*)
		ci13410->ep_queue_head_ptr + 2*ep_num + USB_RECV;

// Kludge to flush any data or status stage requests that are still pending		
	// Cancel any queued transfers on EP 0
	nuke(&ci13410->ep[0]);
	_usb_device_cancel_transfer(ci13410, 0, USB_RECV);
	_usb_device_cancel_transfer(ci13410, 0, USB_SEND);
	
	/********************************************************************
	CR 1219. Hardware versions 2.3+ have a implementation of tripwire 
	semaphore mechanism that requires that we read the contents of 
	QH safely by using the semaphore. Read the USBHS document to under
	stand how the code uses the semaphore mechanism. The following are
	the steps in brief
   
	1. USBCMD Write 1 to Setup Tripwire in register.
	2. Duplicate contents of dQH.StatusBuffer into local software byte
		array.
	3  Read Setup TripWire in register. (if set - continue; if
		cleared goto 1.)
	4. Write '0' to clear Setup Tripwire in register.
	5. Process setup packet using local software byte array copy and
		execute status/handshake phases.       
	********************************************************************/
#ifdef TRIP_WIRE  /* if semaphore mechanism is used
                  ** the following code is compiled in
		  */

	read_safe = FALSE;                      
	while(!read_safe) {
		/*********************************************************
		start with setting the semaphores
		*********************************************************/
		regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd);
		regVal |= EHCI_CMD_SETUP_TRIPWIRE_SET;
		iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.usb_cmd);

		/*********************************************************
		Duplicate the contents of SETUP buffer to our buffer
		*********************************************************/

		/* Copy the setup packet to private buffer */
		memcpy(setupBuffer, (u8*)ep_queue_head_ptr->setup_buffer, 8);
		
		pSetupBuffer = (u32*)setupBuffer;
		ulBufLen = 8;
		
		// Swap bytes in buffer
		for (nLoop = 0; nLoop < ulBufLen; nLoop += 4, pSetupBuffer++) {
			ulSetup = (*(u32*)(pSetupBuffer));
			buffer_ptr[nLoop + 0] = (ulSetup & 0x000000FF);
			
			if ((ulBufLen - nLoop) > 1)
				buffer_ptr[nLoop + 1] =
					((ulSetup & 0x0000FF00) >> 8);
	
			if ((ulBufLen - nLoop) > 2)
				buffer_ptr[nLoop + 2] =
					((ulSetup & 0x00FF0000) >> 16);
	
			if ((ulBufLen - nLoop) > 3)
				buffer_ptr[nLoop + 3] =
					((ulSetup & 0xFF000000) >> 24);		
		}		

		/*********************************************************
		If setup tripwire semaphore is cleared by hardware it means
		that we have a danger and we need to restart.
		else we can exit out of loop safely.
		*********************************************************/
		if(ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd)
				& EHCI_CMD_SETUP_TRIPWIRE_SET) {
			read_safe = TRUE; /* we can proceed exiting the loop*/
		}
	}
  
	/*********************************************************
	Clear the semaphore bit now
	*********************************************************/
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd);
	regVal &= EHCI_CMD_SETUP_TRIPWIRE_CLEAR;
	iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.usb_cmd);
	
#else   /*when semaphore is not used */	
	
	/* Copy the setup packet to private buffer */
	memcpy(setupBuffer, (u8*)ep_queue_head_ptr->setup_buffer, 8);
	
	pSetupBuffer = (u32*)setupBuffer;
	ulBufLen = 8;
	
	// Swap bytes in buffer
	for (nLoop = 0; nLoop < ulBufLen; nLoop += 4, pSetupBuffer++) {
		ulSetup = (*(u32*)(pSetupBuffer));
		buffer_ptr[nLoop + 0] = (ulSetup & 0x000000FF);
		
		if ((ulBufLen - nLoop) > 1)
			buffer_ptr[nLoop + 1] = ((ulSetup & 0x0000FF00) >> 8);

		if ((ulBufLen - nLoop) > 2)
			buffer_ptr[nLoop + 2] = ((ulSetup & 0x00FF0000) >> 16);

		if ((ulBufLen - nLoop) > 3)
			buffer_ptr[nLoop + 3] = ((ulSetup & 0xFF000000) >> 24);		
	}
#endif		
	
	if (bDebug) {
		for (nLoop = 0; nLoop < 8; nLoop++) {
			printk("0x%02X ", buffer_ptr[nLoop]);
				if (((nLoop+1) % 8) == 0)
					printk("\n");
		}
		printk("\n");
	}
		
	/* Clear the bit in the ENDPTSETUPSTAT */
	//iowrite32((1 << ep_num),
	//	&op_reg_ptr->registers.op_dev_regs.endpt_setup_stat);
		
	while (ioread32(&op_reg_ptr->registers.op_dev_regs.endpt_setup_stat)) {
	}

	if (bDebug) printk(KERN_DEBUG "_usb_device_get_setup_data(),"
							" SUCCESSFUL\n");  
}

static void _usb_device_set_address(struct ci13410 *ci13410, u8 address)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;

	if (bDebug) printk(KERN_DEBUG "_usb_device_set_address()\n");

   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	iowrite32((u32)address << VUSBHS_ADDRESS_BIT_SHIFT,
		&op_reg_ptr->registers.op_dev_regs.device_addr);

	if (bDebug) printk(KERN_DEBUG "_usb_device_set_address(), DEVICE_ADDR"
		" = 0x%08X\n", (int)ioread32(&op_reg_ptr->registers
		.op_dev_regs.device_addr));
   
	ci13410->usb_state = USB_STATE_ADDRESS;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_set_address(), SUCCESSFUL\n");   
}

static void _usb_device_set_endpoint_status(struct ci13410 *ci13410, u8 ep,
						u8 stall)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	u8                              ep_num = ep & 0x0F;
	u32                             regVal;

	if (bDebug) printk(KERN_DEBUG "_usb_device_set_endpoint_status()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
		
	if (stall) {
		if (bDebug) printk(KERN_DEBUG "_usb_device_set_endpoint_status()"
			" set stall for ep %d\n", ep_num);
			
		regVal = ioread32(&op_reg_ptr->registers
						.op_dev_regs.endptctrlx[ep_num]);	
		if (ep & 0x80) {	// IN endpoint
			//regVal |= (EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL);
			regVal |= EHCI_EPCTRL_TX_EP_STALL;
			ci13410->ep[ep_num * 2].halted       = TRUE;
		} else {  // OUT endpoint
			regVal |= EHCI_EPCTRL_RX_EP_STALL;
			ci13410->ep[(ep_num * 2) - 1].halted = TRUE;
		}
		
		iowrite32(regVal,
			&op_reg_ptr->registers.op_dev_regs.endptctrlx[ep_num]);
		
	} else {
		if (bDebug) printk(KERN_DEBUG "_usb_device_set_endpoint_status()"
					" clear stall for ep %d\n", ep_num);

		regVal = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);

		if (ep & 0x80) {	// IN endpoint
			//regVal &= ~(EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL);
			regVal &= ~EHCI_EPCTRL_TX_EP_STALL;
			regVal |= EHCI_EPCTRL_TX_DATA_TOGGLE_RST;
			ci13410->ep[ep_num * 2].halted       = FALSE;
			
		} else {
			regVal &= ~EHCI_EPCTRL_RX_EP_STALL;
			regVal |= EHCI_EPCTRL_RX_DATA_TOGGLE_RST;
			ci13410->ep[(ep_num * 2) - 1].halted = FALSE;
		}
			
		iowrite32(regVal,
			&op_reg_ptr->registers.op_dev_regs.endptctrlx[ep_num]);
	}   

   if (bDebug) printk(KERN_DEBUG "_usb_device_set_endpoint_status(),"
   							" SUCCESSFUL\n"); 
}

static u8 _usb_device_set_status(struct ci13410 *ci13410, u8 component,
						u16 setting)
{
	u8 bDebug = FALSE;

	if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()\n");
   
	switch (component) {
	case USB_STATUS_DEVICE_STATE:
		ci13410->usb_state = setting;
		if (bDebug)
			printk(KERN_DEBUG "_usb_device_set_status()"
				" USB_STATE = %d\n", ci13410->usb_state);
		break;

	case USB_STATUS_DEVICE:
		ci13410->usb_device_state = setting;
		if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()"
			" USB_DEVICE_STATE = %d\n", ci13410->usb_device_state);
		break;

	case USB_STATUS_INTERFACE:
		break;

	case USB_STATUS_CURRENT_CONFIG:
		ci13410->usb_curr_config = setting;
		if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()"
			" USB_CURR_CONFIG = %d\n", ci13410->usb_curr_config);
		break;

	case USB_STATUS_SOF_COUNT:
		ci13410->usb_sof_count = setting;
		break;
	
	case USB_FORCE_FULL_SPEED:
		break;
	case USB_PHY_LOW_POWER_SUSPEND:
		break;

	case USB_STATUS_ADDRESS:
		ci13410->device_address = setting;
		if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()"
			" DEVICE_ADDRESS = %d\n", ci13410->device_address);
		_usb_device_set_address(ci13410, setting);
		break;

	case USB_STATUS_TEST_MODE:
		//_usb_dci_vusb20_set_test_mode(handle, setting);
		break;
 
	default:
		if (component & USB_STATUS_ENDPOINT) {
			if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()"
				" endpoint = 0x%02X\n",
				component & USB_STATUS_ENDPOINT_NUMBER_MASK);
			_usb_device_set_endpoint_status(ci13410,
			component & USB_STATUS_ENDPOINT_NUMBER_MASK, setting);

		} else {
			if (bDebug) printk(KERN_DEBUG "_usb_device_set_status()"
						" bad status\n");
			return USBERR_BAD_STATUS;
		}
		break;
	}

	if (bDebug) printk(KERN_DEBUG "_usb_device_set_status(), SUCCESSFUL\n");
	   
	return USB_OK;
}

static u8 _usb_device_get_endpoint_status(struct ci13410 *ci13410, u8 ep)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	u8                              ep_num = ep & 0x0F;
	u8                              ep_dir = ep & 0x80; 

	if (bDebug) printk(KERN_DEBUG "_usb_device_get_endpoint_status()\n");

	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;


	if (bDebug) printk(KERN_DEBUG "_usb_device_get_endpoint_status()"
				" SUCCESSFUL\n");

	return ((ioread32(&op_reg_ptr->registers
		.op_dev_regs.endptctrlx[ep_num])
		& (ep_dir ? EHCI_EPCTRL_TX_EP_STALL : EHCI_EPCTRL_RX_EP_STALL))
		? 1 : 0);
}

static u8 _usb_device_get_status(struct ci13410 *ci13410, u8 component,
							u16 *status)
{
	u8 bDebug = FALSE;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_get_status()\n");

	switch (component) {
	case USB_STATUS_DEVICE_STATE:
		*status = ci13410->usb_state;
		break;
	case USB_STATUS_DEVICE:
		*status = ci13410->usb_device_state;
		break;
	case USB_STATUS_INTERFACE:
		break;
	case USB_STATUS_ADDRESS:
		*status = ci13410->device_address;
		break;
	case USB_STATUS_CURRENT_CONFIG:
		*status = ci13410->usb_curr_config;
		break;
	case USB_STATUS_SOF_COUNT:
		*status = ci13410->usb_sof_count;
		break;
 
	default:
		if (component & USB_STATUS_ENDPOINT) {
			*status = _usb_device_get_endpoint_status(ci13410, 
				component & USB_STATUS_ENDPOINT_NUMBER_MASK);
		} else {
			if (bDebug) printk(KERN_ERR "_usb_device_get_status()"
						" bad status\n");
			return USBERR_BAD_STATUS;
		}
		break;
	}

	if (bDebug) printk(KERN_DEBUG "_usb_device_get_status(), SUCCESSFUL\n");
   
	return USB_OK;   
}

static u8 _usb_device_call_service(struct ci13410 *ci13410,
                            u8 type,
                            u8 setup,
                            u8 direction,
                            u8 *buffer_ptr,
                            u32 length,
                            u8 errors)
{		
	switch(type) {
	case  USB_SERVICE_EP0:
		service_ep0(ci13410, setup, direction,
				buffer_ptr, length, errors);
		break;
	case  USB_SERVICE_EP1:
	case  USB_SERVICE_EP2:
	case  USB_SERVICE_EP3:
	case  USB_SERVICE_EP4:
	case  USB_SERVICE_EP5:
	case  USB_SERVICE_EP6:
	case  USB_SERVICE_EP7:
	case  USB_SERVICE_EP8:
	case  USB_SERVICE_EP9:
	case  USB_SERVICE_EP10:
	case  USB_SERVICE_EP11:
	case  USB_SERVICE_EP12:
	case  USB_SERVICE_EP13:
	case  USB_SERVICE_EP14:
	case  USB_SERVICE_EP15:
		service_epn(ci13410, type, direction, buffer_ptr,
				length, errors);
		break;
	case  USB_SERVICE_BUS_RESET:
		reset_ep0(ci13410, setup, direction, buffer_ptr,
				length, errors);
		break;
	case  USB_SERVICE_SUSPEND:
		break;
	case  USB_SERVICE_SOF:
		break;
	case  USB_SERVICE_RESUME:
		break;
	case  USB_SERVICE_SLEEP:
		break;
	case  USB_SERVICE_SPEED_DETECTION:
		break;
	case  USB_SERVICE_ERROR:
		break;
	case  USB_SERVICE_STALL:
		break;
	}
	
	return USB_OK;  
}

static void _usb_device_stall_endpoint(struct ci13410 *ci13410,
                                u8 ep_num,
                                u8 direction)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	struct ci13410_ep               *ep;
	u32                             regVal;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_stall_endpoint()\n");

	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	/* Get the endpoint queue head address */
	ep_queue_head_ptr = (ci13410_ep_queue_head*)
		ci13410->ep_queue_head_ptr + 2*ep_num + direction;
   
	/* Stall the endpoint for Rx or Tx and set the endpoint type */
	if (ep_queue_head_ptr->max_pkt_length & VUSB_EP_QUEUE_HEAD_IOS) {
		/* This is a control endpoint so STALL both directions */
		regVal = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
		regVal |= (EHCI_EPCTRL_TX_EP_STALL | EHCI_EPCTRL_RX_EP_STALL);
		iowrite32(regVal, &op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
	} else {
		if (bDebug)
		printk(KERN_DEBUG "_usb_device_stall_endpoint() ep = %d,"
		" direction = %s\n", ep_num, direction?"USB_SEND":"USB_RECV");
		regVal = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
		regVal |= (direction ? EHCI_EPCTRL_TX_EP_STALL
				: EHCI_EPCTRL_RX_EP_STALL);
		iowrite32(regVal, &op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
	}
	
	if (ep_num == 0)
		ep = &ci13410->ep[0];
	else {
		if (direction == USB_RECV)  // OUT
			ep = &ci13410->ep[(ep_num * 2) - 1];
		else  // IN
			ep = &ci13410->ep[ep_num * 2];
	}
	
	ep->halted = TRUE;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_stall_endpoint(),"
							" SUCCESSFUL\n");
}

static void _usb_device_unstall_endpoint(struct ci13410 *ci13410,
                                  u8 ep_num,
                                  u8 direction)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	struct ci13410_ep               *ep;		
	u32                             regVal;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_unstall_endpoint()\n");
	
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	/* Enable the endpoint for Rx or Tx and set the endpoint type */
	regVal = ioread32(&op_reg_ptr->registers
				.op_dev_regs.endptctrlx[ep_num]);
	regVal &= (direction ? ~EHCI_EPCTRL_TX_EP_STALL
				: ~EHCI_EPCTRL_RX_EP_STALL);
	iowrite32(regVal, &op_reg_ptr->registers
				.op_dev_regs.endptctrlx[ep_num]);
	
	if (ep_num == 0)
		ep = &ci13410->ep[0];
	else {
		if (direction == USB_RECV)  // OUT
			ep = &ci13410->ep[(ep_num * 2) - 1];
		else  // IN
			ep = &ci13410->ep[ep_num * 2];
	}
	
	ep->halted = FALSE;	

	if (bDebug) printk(KERN_DEBUG "_usb_device_unstall_endpoint(),"
							" SUCCESSFUL\n");     
}

static void _usb_device_process_reset(struct ci13410 *ci13410)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	u8                              i;
	u32                             regVal;

	if (bDebug) printk(KERN_DEBUG "_usb_device_process_reset()\n");
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
   
	_usb_device_call_service(ci13410,
				USB_SERVICE_BUS_RESET, 0, 0, 0, 0, 0);

	/* The address bits are past bit 25-31. Set the address */
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.device_addr);
	regVal &= ~0xFE000000;
	iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.device_addr);
   
	/* Clear all the setup token semaphores */
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.endpt_setup_stat);
	iowrite32(0x000000ff, &op_reg_ptr->registers.op_dev_regs.endpt_setup_stat);
	

	/* Clear all the endpoint complete status bits */   
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.endptcomplete);
	iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.endptcomplete);
   
	while (ioread32(&op_reg_ptr->registers
				.op_dev_regs.endptprime) & 0xFFFFFFFF) {
		/* Wait until all ENDPTPRIME bits cleared */
	}
   
	/* Write 1s to the Flush register */
	iowrite32(0xFFFFFFFF, &op_reg_ptr->registers.op_dev_regs.endptflush);

	/* Unstall all endpoints */
	for (i = 0; i < ci13410->max_endpoints; i++) {
		_usb_device_unstall_endpoint(ci13410, i, 0);
		_usb_device_unstall_endpoint(ci13410, i, 1);
	}

	ci13410->bus_resetting	= TRUE;
	ci13410->usb_state		= USB_STATE_POWERED;	
   
	//_usb_device_call_service(ci13410,
	//			USB_SERVICE_BUS_RESET, 0, 0, 0, 0, 0);

	if (bDebug) printk(KERN_DEBUG "_usb_device_process_reset(),"
							" SUCCESSFUL\n");  
}

static void _usb_device_process_port_change(struct ci13410 *ci13410)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;

	if (bDebug) printk(KERN_DEBUG "_usb_device_process_port_change()\n");  
  
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	if (ci13410->bus_resetting) {
		/* Bus reset operation complete */
		ci13410->bus_resetting = FALSE;
	}

	if (!(ioread32(&op_reg_ptr->registers
		.op_dev_regs.portscx[0]) & EHCI_PORTSCX_PORT_RESET)) {
		/* Get the speed */
		if (ioread32(&op_reg_ptr->registers
			.op_dev_regs.portscx[0])
			& EHCI_PORTSCX_PORT_HIGH_SPEED) {
			if (bDebug)
				printk(KERN_DEBUG
					"_usb_device_process_port_change()"
						" Speed changed to HIGH\n");
			ci13410->gadget.speed = USB_SPEED_HIGH;
		} else {
			if (bDebug) 
				printk(KERN_DEBUG
					"_usb_device_process_port_change()"
					" Speed changed to FULL\n");
			ci13410->gadget.speed = USB_SPEED_FULL;			
		}

		/* Inform the upper layers of the speed of operation */
		//_usb_device_call_service(usb_dev_ptr,
		//				USB_SERVICE_SPEED_DETECTION,
		//				0,
		//				0,
		//				0,
		//				usb_dev_ptr->SPEED,
		//				0);
   
	}

	if (ioread32(&op_reg_ptr->registers
		.op_dev_regs.portscx[0]) & EHCI_PORTSCX_PORT_SUSPEND) {
		ci13410->usb_dev_state_b4_suspend = ci13410->usb_state;
		ci13410->usb_state = USB_STATE_SUSPEND;

		/* Inform the upper layers */
		//_usb_device_call_service(usb_dev_ptr,
		//			USB_SERVICE_SUSPEND, 0, 0, 0, 0, 0);
	}

	if (!(ioread32(&op_reg_ptr->registers.op_dev_regs.portscx[0])
		& EHCI_PORTSCX_PORT_SUSPEND)
		&& (ci13410->usb_state == USB_STATE_SUSPEND)) {
		ci13410->usb_state = ci13410->usb_dev_state_b4_suspend;
		/* Inform the upper layers */
		//_usb_device_call_service(usb_dev_ptr,
		//			USB_SERVICE_RESUME, 0, 0, 0, 0, 0);
		
		if (bDebug) printk(KERN_DEBUG
					"_usb_device_process_port_change(),"
					" SUCCESSFUL, resumed\n");

		return;
	}

	ci13410->usb_state = USB_STATE_DEFAULT;
   
	if (bDebug) printk(KERN_DEBUG "_usb_device_process_port_change()"
					" SUCCESSFUL\n");
}

static void _usb_device_process_tr_complete(struct ci13410 *ci13410)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	ci13410_ep_tr                   *dTD_ptr; 
	ci13410_ep_tr                   *temp_dTD_ptr;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	u32                             temp, i, ep_num, direction, bit_pos;
	u32                             remaining_length = 0;
	u32                             actual_transfer_length = 0;
	u32                             errors = 0;
	struct ci13410_xd               *xd_ptr;
	struct ci13410_xd               *temp_xd_ptr = NULL;
	u8                              *buff_start_address = NULL;
	u8                              endpoint_detected = FALSE;
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	if (bDebug) printk(KERN_DEBUG "_usb_device_process_tr_complete()\n");

	/* Process non-setup transaction complete interrupts */   
	bit_pos = ioread32(&op_reg_ptr->registers.op_dev_regs.endptcomplete);

	if (bit_pos) {		
		/* Clear the bits in the register */
		iowrite32(bit_pos,
			&op_reg_ptr->registers.op_dev_regs.endptcomplete);
		
		/* Get the endpoint number and the direction of transfer */
		for (i = 0; i < USB_MAX_ENDPOINTS * 2; i++) {
			endpoint_detected = FALSE;
			if ((i < USB_MAX_ENDPOINTS) && (bit_pos & (1 << i))) {
				ep_num = i;
				direction = 0;
				endpoint_detected = TRUE;
			} else if((i >= USB_MAX_ENDPOINTS)
				&& (bit_pos
				& (1 << (i+16-USB_MAX_ENDPOINTS)))) {
				ep_num = (i - USB_MAX_ENDPOINTS);
				direction = 1;
				endpoint_detected = TRUE;
			}
         
			if(endpoint_detected) {
				temp = (2*ep_num + direction);

				/* Get the first dTD */      
				dTD_ptr = ci13410->ep_dtd_heads[temp];

				ep_queue_head_ptr = 
					(ci13410_ep_queue_head*)
					ci13410->ep_queue_head_ptr
					+ temp;				

				/* Process all the dTDs for respective 
				** transfers
				*/
				while (dTD_ptr) {
					if (dTD_ptr->size_ioc_sts
						& VUSBHS_TD_STATUS_ACTIVE) {
						/* No more dTDs to process.
						** Next one is owned by VUSB
						*/
						if (bDebug)
							printk(KERN_ERR "_usb_device_"
							" process_tr_complete()"
							" Transfer is active\n");
						break;
					}
               
					/* Get the correct internal transfer
					** descriptor
					*/
					xd_ptr = (struct ci13410_xd*)dTD_ptr
						->scratch_ptr->xd_for_this_dtd;
					if (xd_ptr) {
						buff_start_address = 
							xd_ptr->wstartaddress;
						actual_transfer_length =
							xd_ptr->wtotallength;
						temp_xd_ptr = xd_ptr;
					}
               
					/* Get the address of the next dTD */

					temp_dTD_ptr =
						(ci13410_ep_tr*)
						((u32)dTD_ptr->next_tr_elem_ptr
						& VUSBHS_TD_ADDR_MASK);
					
					if (temp_dTD_ptr)
						temp_dTD_ptr =
						(ci13410_ep_tr*)((u32)
						dTD_dma_to_virt((u32)ci13410->
						dtd_aligned_base_ptr,
						(u32)ci13410->
						dma_dtd_aligned_base_ptr,
						dTD_ptr->next_tr_elem_ptr)
						& VUSBHS_TD_ADDR_MASK);
					                  
					/* Read the errors */
					errors = (dTD_ptr->size_ioc_sts
							& VUSBHS_TD_ERROR_MASK);
                  
					if (!errors) {
						/* No errors
						** Get the length of transfer 
						** from the current dTD
						*/ 
						remaining_length +=
						((dTD_ptr->size_ioc_sts
						& VUSB_EP_TR_PACKET_SIZE)
						>> 16);
						
						actual_transfer_length -=
							remaining_length;
					} else {
						if (errors
						& VUSBHS_TD_STATUS_HALTED) {
							/* Clear the errors 
							** and Halt condition
							*/
							ep_queue_head_ptr
							->size_ioc_int_sts
							&= ~errors;
						}
					}
               
					/* Retire the processed dTD */
					_usb_device_cancel_transfer(ci13410,
							ep_num, direction);
					if (temp_dTD_ptr) {
						if ((u32)temp_dTD_ptr
							->scratch_ptr
							->xd_for_this_dtd
							!= (u32)temp_xd_ptr) {
							/* Transfer complete.
							** Call the register
							** service function
							** for the endpoint 
							*/
							_usb_device_call_service
							(ci13410,
							ep_num,
							FALSE,
							direction,
							buff_start_address,
							actual_transfer_length,
							errors);							
							
							remaining_length = 0;
						}
					} else {
						/* Transfer complete.
						** Call the register service
						** function for the endpoint 
						*/
						_usb_device_call_service
						(ci13410,
						ep_num,
						FALSE,
						direction,
						buff_start_address,
						actual_transfer_length,
						errors);
						
						remaining_length = 0;
					}
					
					dTD_ptr = temp_dTD_ptr;
					errors = 0;
				}
			}
		}
	}
	

	
	/* Process all Setup packet received interrupts */
	bit_pos = ioread32(&op_reg_ptr->registers
				.op_dev_regs.endpt_setup_stat);

	if (bit_pos) {
		/* Clear all the bits in the ENDPTSETUPSTAT */
		iowrite32(0x000000ff, &op_reg_ptr->registers
						.op_dev_regs.endpt_setup_stat);
		
		for (i = 0; i < USB_MAX_CONTROL_ENDPOINTS; i++) {
			if (bit_pos & (1 << i)) {
				_usb_device_call_service(ci13410,
							i, TRUE, 0, 0, 8, 0);
			}
		}
	}
   
	/* Don't wait for clearing the endpoint setup status register here.
	** It is checked when a setup packet is read out of the buffer 
	*/
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_process_tr_complete()"
				" SUCCESSFUL\n");
}

static u8 _usb_device_add_dTD(struct ci13410 *ci13410,
				struct ci13410_xd *xd_ptr)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs               __iomem *op_reg_ptr;
	ci13410_ep_tr       *dTD_ptr, *temp_dTD_ptr, *first_dTD_ptr = NULL;
	ci13410_ep_queue_head           *ep_queue_head_ptr;
	u32                             curr_pkt_len, remaining_len; 
	u32                             curr_offset, temp, bit_pos;
	u32                             regVal;
#ifdef TRIP_WIRE
	u8                              read_safe; /*boolean*/
	u32                             temp_ep_stat;
#endif	
	
	/*********************************************************************
	For a optimal implementation, we need to detect the fact that
	EP_LIST_ADDR
	we are adding DTD to an empty list. If list is empty, we can
	actually skip several programming steps esp. those for ensuring
   	hat there is no race condition.The following boolean will be useful
	in skipping some code here.
	*********************************************************************/
	u8 list_empty = FALSE;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_add_dTD()\n");
		
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	remaining_len = xd_ptr->wtotallength;
   
	curr_offset = 0;
	temp        = (2*xd_ptr->ep_num + xd_ptr->bdirection);
	bit_pos     = (1 << (16 * xd_ptr->bdirection + xd_ptr->ep_num));

	ep_queue_head_ptr =
		(ci13410_ep_queue_head*)
		ci13410->ep_queue_head_ptr + temp;
		
	
	/*********************************************************************
	This loops iterates through the length of the transfer and divides
	the data in to DTDs each handling the a max of 0x4000 bytes of data.
	The first DTD in the list is stored in a pointer called first_dTD_ptr.
	This pointer is later linked in to QH for processing by the hardware.   
	*********************************************************************/

	do {
		/* Check if we need to split the transfer into multiple dTDs */
		if (remaining_len > VUSB_EP_MAX_LENGTH_TRANSFER) {
			curr_pkt_len = VUSB_EP_MAX_LENGTH_TRANSFER;
		} else {
			curr_pkt_len = remaining_len;
		}
   
		remaining_len -= curr_pkt_len;
		

		/* Get a dTD from the queue */   
		EHCI_DTD_QGET(ci13410->dtd_head, ci13410->dtd_tail, dTD_ptr);
   
		if (!dTD_ptr) {
			if (bDebug) printk(KERN_DEBUG "_usb_device_add_dTD()"
						" Did not get dTD_ptr\n");
			return USBERR_TR_FAILED;
		}
		
		ci13410->dtd_entries--;

		if (curr_offset == 0) {
			first_dTD_ptr = dTD_ptr;
		}
		
		/* Zero the dTD. Leave the last 4 bytes as 
		** that is the scratch pointer
		*/
		memset((void *) dTD_ptr, 0, (sizeof(ci13410_ep_tr) - 4));

		/* Initialize the dTD */
		dTD_ptr->scratch_ptr->private = ci13410;
   
		/* Set the Terminate bit */
		dTD_ptr->next_tr_elem_ptr = VUSB_EP_QUEUE_HEAD_NEXT_TERMINATE;


		/*************************************************************
		FIX ME: For hig-speed and high-bandwidth ISO IN endpoints,
		we must initialize the multiplied field so that Host can issues
		multiple IN transactions on the endpoint. See the DTD data
		structure for MultiIO field.
      
		S Garg 11/06/2003
		*************************************************************/
      
		/* Fill in the transfer size */
		if (!remaining_len) {
			dTD_ptr->size_ioc_sts =
				((curr_pkt_len << VUSBHS_TD_LENGTH_BIT_POS)
				| (VUSBHS_TD_IOC)
				| (VUSBHS_TD_STATUS_ACTIVE));
		} else {
			dTD_ptr->size_ioc_sts =
				((curr_pkt_len << VUSBHS_TD_LENGTH_BIT_POS)
				| VUSBHS_TD_STATUS_ACTIVE);
		}
   
		/* Set the reserved field to 0 */
		dTD_ptr->size_ioc_sts &= ~VUSBHS_TD_RESERVED_FIELDS;

		dTD_ptr->buff_ptr0 = (u32)xd_ptr->wstartaddress + curr_offset;

		/* 4K apart buffer page pointers */
		dTD_ptr->buff_ptr1 = (dTD_ptr->buff_ptr0 + 4096);
		dTD_ptr->buff_ptr2 = (dTD_ptr->buff_ptr1 + 4096);
		dTD_ptr->buff_ptr3 = (dTD_ptr->buff_ptr2 + 4096);
		dTD_ptr->buff_ptr4 = (dTD_ptr->buff_ptr3 + 4096);
		
		curr_offset += curr_pkt_len;

		/* Maintain the first and last device transfer descriptor per 
		** endpoint and direction 
		*/
		if (!ci13410->ep_dtd_heads[temp]) {
			ci13410->ep_dtd_heads[temp] = dTD_ptr;
			/***********************************************
			If list does not have a head, it means that list
			is empty. An empty condition is detected.
			***********************************************/
			list_empty = TRUE;
		}
   
		/* Check if the transfer is to be
		** queued at the end or beginning
		*/
		temp_dTD_ptr = ci13410->ep_dtd_tails[temp];
      
		/* Remember which XD to use for this dTD */
		dTD_ptr->scratch_ptr->xd_for_this_dtd = (void*)xd_ptr;
      
		/* New tail */
		ci13410->ep_dtd_tails[temp] = dTD_ptr;
		if (temp_dTD_ptr) {
			/* Should not do |=.
			** The Terminate bit should be zero
			*/
			temp_dTD_ptr->next_tr_elem_ptr =
				(u32)(dTD_ptr->scratch_ptr->dma);
		}

	} while (remaining_len);

// Fix for zero length terminate on the control endpoint	
	if ((xd_ptr->ep_num == 0) && (xd_ptr->bdirection == USB_SEND)) {
		if ((xd_ptr->wtotallength > 0) &&
		   ((xd_ptr->wtotallength % 64) == 0) &&
		   (xd_ptr->wtotallength == le16_to_cpu(g_setupPacket.wLength))) {
			// Disable zero terminate
			ep_queue_head_ptr->max_pkt_length |= 
				VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL;
		} else {
			// Leave zero terminate enabled
			ep_queue_head_ptr->max_pkt_length &= 
				~VUSB_EP_QUEUE_HEAD_ZERO_LEN_TER_SEL;			
		}
	}
	
	/**************************************************************
	In the loop above DTD has already been added to the list
	However endpoint has not been primed yet.
	If list is not empty we need safter_usb_device_add_dTD
	ways to add DTD to the
	existing list. Else we just skip to adding DTD to QH safely.
	**************************************************************/
   
	if(!list_empty) {
#ifdef TRIP_WIRE
		/*********************************************************
		Hardware v3.2+ require the use of semaphore to ensure that
		QH is safely updated.
		*********************************************************/
               
		/*********************************************************
		Check the prime bit. If set goto done
		*********************************************************/
		if (ioread32(&op_reg_ptr->registers
				.op_dev_regs.endptprime) & bit_pos) {
			goto done;
		}

		read_safe = FALSE;                      
		while(!read_safe) {
			/******************************************************
			start with setting the semaphores
			******************************************************/
			regVal 	= ioread32(&op_reg_ptr->registers
						.op_dev_regs.usb_cmd);
			regVal	|= EHCI_CMD_ATDTW_TRIPWIRE_SET;
			iowrite32(regVal, &op_reg_ptr->registers
						.op_dev_regs.usb_cmd);
                      
			/******************************************************
			Read the endpoint status
			******************************************************/
			temp_ep_stat = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptstatus) & bit_pos; 

			/******************************************************
			Reread the ATDTW semaphore bit to check if it is
			cleared.
			When hardware see a hazard, it will clear the bit or
			else we remain set to 1 and we can proceed with priming
			of endpoint if not already primed.
			******************************************************/
			if(ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd)
					& EHCI_CMD_ATDTW_TRIPWIRE_SET) {
				read_safe = TRUE;
			}

		}/*end while loop */

		/*********************************************************
		Clear the semaphore
		*********************************************************/
		regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd);
		regVal &= EHCI_CMD_ATDTW_TRIPWIRE_CLEAR;
		iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.usb_cmd);

		/*********************************************************
		If endpoint is not active, we activate it now.               
		*********************************************************/
		if(!temp_ep_stat) {
			/* No other transfers on the queue */
			ep_queue_head_ptr->next_dtd_ptr =
					(u32)(first_dTD_ptr->scratch_ptr->dma);
			ep_queue_head_ptr->size_ioc_int_sts = 0;

			/* Prime the Endpoint */
			iowrite32(bit_pos, &op_reg_ptr->registers
						.op_dev_regs.endptprime);
		}

#else   /*workaround old method */		
		
		/* Start CR 1015 */
		/* Prime the Endpoint */
		iowrite32(bit_pos, &op_reg_ptr->registers
				.op_dev_regs.endptprime);

		if (!(ioread32(&op_reg_ptr->registers.op_dev_regs.endptstatus)
				& bit_pos)) {
			/* old workaround will be compiled */
			while (ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptprime) & bit_pos) {
				/* Wait for the ENDPTPRIME to go to zero */
			}

			if (ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptstatus) & bit_pos) {
				/* The endpoint was not not primed so no
				** other transfers on the queue 
				*/
				goto done;
			}
      
		} else {
			goto done;
		}

		/* No other transfers on the queue */
		ep_queue_head_ptr->next_dtd_ptr =
					(u32)(first_dTD_ptr->scratch_ptr->dma);
		ep_queue_head_ptr->size_ioc_int_sts = 0;
   
		/* Prime the Endpoint */
		iowrite32(bit_pos, &op_reg_ptr->registers
					.op_dev_regs.endptprime);
		
#endif		
	} else {
		/* No other transfers on the queue */
		ep_queue_head_ptr->next_dtd_ptr =
					(u32)(first_dTD_ptr->scratch_ptr->dma);
		ep_queue_head_ptr->size_ioc_int_sts = 0;
					
// Workaround for winbond latency problem
#if defined(CONFIG_ARCH_WPCM450)
		{
			// Read something back from the queuehead
			u32 size_ioc_int_sts;
			size_ioc_int_sts = ep_queue_head_ptr->size_ioc_int_sts;	
		}
#endif

		/* Prime the Endpoint */
		iowrite32(bit_pos, &op_reg_ptr->registers
					.op_dev_regs.endptprime);					

	}	
done:
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_add_dTD(), SUCCESSFUL\n");

	return USB_OK;
}

static u8 _usb_device_send_data(struct ci13410	*ci13410,
                         u8 ep_num,
                         u8 *buff_ptr,
                         u32 size)
{
	u8                bDebug = FALSE;
	u8                error;
	struct ci13410_xd *xd_ptr;
	ci13410_usb_regs               __iomem *op_reg_ptr;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_send_data()\n");  
   
	if (!ci13410->xd_entries) {
		if (bDebug) printk(KERN_ERR "_usb_device_send_data()"
					" transfer in progress\n");
		return USB_STATUS_TRANSFER_IN_PROGRESS;
	}

	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

	if (0 == (ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd) & EHCI_CMD_RUN_STOP))
	{
		printk(KERN_ERR "_usb_device_send_data() - device is stopped\n");
		return USBERR_TR_FAILED;
	}

	/* Get a transfer descriptor */
	USB_XD_QGET(ci13410->xd_head, ci13410->xd_tail, xd_ptr);
	
	if (!xd_ptr) {
		if (bDebug) printk(KERN_ERR "_usb_device_send_data()"
					" Did not get xd_ptr\n");
		return USBERR_TR_FAILED;
	}
	
	ci13410->xd_entries--;

	/* Initialize the new transfer descriptor */      
	xd_ptr->ep_num          = ep_num;
	xd_ptr->bdirection      = USB_SEND;
	xd_ptr->wtotallength    = size;
	xd_ptr->wsofar          = 0;
	xd_ptr->wstartaddress   = buff_ptr;
   
	xd_ptr->bstatus = USB_STATUS_TRANSFER_ACCEPTED;

	error = _usb_device_add_dTD(ci13410, xd_ptr);
   
	if (error) {
		if (bDebug) printk(KERN_ERR "_usb_device_send_data()"
					" transfer failed\n");
		return USBERR_TX_FAILED;
	}

	if (bDebug) printk(KERN_DEBUG "_usb_device_send_data(), SUCCESSFUL\n");
		
	return error;
}

static u8 _usb_device_recv_data(struct ci13410	*ci13410,
                         u8 ep_num,
                         u8* buff_ptr,
                         u32 size)
{
	u8                bDebug = FALSE;
	u8                error = USB_OK;
	struct ci13410_xd *xd_ptr;
	ci13410_usb_regs               __iomem *op_reg_ptr;
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_recv_data()\n");

	if (!ci13410->xd_entries) {
		if (bDebug) printk(KERN_ERR "_usb_device_recv_data()"
					" transfer in progress\n");
		return USB_STATUS_TRANSFER_IN_PROGRESS;
	}

	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;

  if (0 == (ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd) & EHCI_CMD_RUN_STOP))
  {
		printk(KERN_ERR "_usb_device_recv_data() - device is stopped\n");
		return USBERR_RX_FAILED;
  }

	/* Get a transfer descriptor for the specified endpoint 
	** and direction 
	*/
	USB_XD_QGET(ci13410->xd_head, ci13410->xd_tail, xd_ptr);
   
	ci13410->xd_entries--;

	/* Initialize the new transfer descriptor */      
	xd_ptr->ep_num = ep_num;
	xd_ptr->bdirection = USB_RECV;
	xd_ptr->wtotallength = size;
	xd_ptr->wsofar = 0;
	xd_ptr->wstartaddress = buff_ptr;
   
	xd_ptr->bstatus = USB_STATUS_TRANSFER_ACCEPTED;

	error = _usb_device_add_dTD(ci13410, xd_ptr);

	if (error) {
		printk(KERN_ERR "_usb_device_recv_data(),  receive failed\n");
		return USBERR_RX_FAILED;
	}
	
	if (bDebug) printk(KERN_DEBUG "_usb_device_recv_data() SUCCESSFUL\n");
   
   return error;
}
/*****************************************************************************/


static void device_reset(struct ci13410 *ci13410)
{
	u8                              bDebug = FALSE;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	u32                             regVal;

	if (bDebug) printk(KERN_DEBUG "device_reset()\n");	
	
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
		
	/* Disable interrupts */
	regVal = ioread32(&op_reg_ptr->registers.op_dev_regs.usb_intr);
	regVal &= ~(EHCI_INTR_INT_EN | EHCI_INTR_ERR_INT_EN
			| EHCI_INTR_PORT_CHANGE_DETECT_EN
			| EHCI_INTR_SOF_UFRAME_EN
			| EHCI_INTR_DEVICE_SUSPEND| EHCI_INTR_RESET_EN);
	iowrite32(regVal, &op_reg_ptr->registers.op_dev_regs.usb_intr);		

	/* Stop the controller */
	regVal =
	ioread32(&op_reg_ptr->registers.op_dev_regs.usb_cmd);
	regVal &= ~EHCI_CMD_RUN_STOP;
	iowrite32(regVal,
		&op_reg_ptr->registers.op_dev_regs.usb_cmd);

#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	set_usb_insertion_float();	
#endif		
      
	/* Reset the controller to get default values */
	iowrite32(EHCI_CMD_CTRL_RESET,
		&op_reg_ptr->registers.op_dev_regs.usb_cmd);

	while (ioread32
		(&op_reg_ptr->registers.op_dev_regs.usb_cmd)
		& EHCI_CMD_CTRL_RESET) {
		/* Wait for the controller reset to complete */
	}
		
	if (bDebug) printk(KERN_DEBUG "device_reset(), SUCCESSFUL\n");  
}

static void ci13410_free_buffer(struct usb_ep *_ep,
                                void *buf,
                                dma_addr_t dma,
                                unsigned bytes);
                                
static void ci13410_free_request(struct usb_ep *_ep, struct usb_request *_req);

static void device_free_mem(struct ci13410 *ci13410)
{
	u8 bDebug = FALSE;

	if (bDebug) printk(KERN_DEBUG "device_free_mem()\n");	
	
	if(ci13410)
	{		
		// Free the driver memory		
		if(ci13410->driver_memory) {
			dma_free_coherent(the_controller->gadget.dev.parent,
					ci13410->total_memory,
					(void*)ci13410->driver_memory,
					ci13410->dma_driver_memory);			
		}
		
		// Free all internal transfer descriptors
		if(ci13410->xd_base)
			usb_memfree(ci13410->xd_base);
	   
		// Free all XD scratch memory
		if(ci13410->xd_scratch_struct_base)
			usb_memfree(ci13410->xd_scratch_struct_base);
	
		// Free the temp ep init XD
		if(ci13410->temp_xd_ptr)
			usb_memfree(ci13410->temp_xd_ptr);
		
		
		// Free the ep0 buffer and request
		if (ci13410->ep0_req) {
			if (ci13410->ep0_buf)
				ci13410_free_buffer(&ci13410->ep[0].ep,
							ci13410->ep0_buf,
							ci13410->ep0_req->dma,
							2);
					
			ci13410_free_request(&ci13410->ep[0].ep,
						ci13410->ep0_req);
		}
	
		// Release state struct memory
		usb_memfree(ci13410);
		ci13410 = NULL;
	}
	
	if (bDebug) printk(KERN_DEBUG "device_free_mem(), SUCCESSFUL\n");	
}	

/* dequeue ALL requests */
static void nuke(struct ci13410_ep *ep)
{
	struct ci13410_request  *req;
	u8                      count = 0;

	while (!list_empty(&ep->queue) && count < 20) {
		req = list_entry (ep->queue.next,
					struct ci13410_request, queue);
		list_del_init (&req->queue);
		req->req.status	= -ESHUTDOWN;
		if (likely(req->req.complete)) {
			spin_unlock (&ep->ci13410->lock);
			req->req.complete (&ep->ep, &req->req);
			spin_lock (&ep->ci13410->lock);
		}
		
		count++;  /* guard against endless loop */
	}
}

static void stop_activity(struct ci13410 *ci13410,
                          struct usb_gadget_driver *driver);

static void reset_ep0(struct ci13410 *ci13410,
               u8 setup,
               u8 direction,
               u8 *buffer_ptr,
               u32 length,
               u8 errors)
{
	u8                bDebug = FALSE;
	int               nEndpoint;
	struct ci13410_ep *ep;
		
	if (bDebug) printk(KERN_DEBUG "reset_ep0()\n");     

	stop_activity(ci13410, ci13410->driver);

	// Cancel any queued transfers
	/*on a reset always cancel all transfers all EP 0 */
	_usb_device_cancel_transfer(ci13410, 0, USB_RECV);
	_usb_device_cancel_transfer(ci13410, 0, USB_SEND);

	/* Initialize the endpoint 0 in both directions */
	_usb_device_init_endpoint(ci13410, 0, 64,
					USB_RECV, USB_CONTROL_ENDPOINT,
					0);
	_usb_device_init_endpoint(ci13410, 0, 64,
					USB_SEND, USB_CONTROL_ENDPOINT,
					0);

	// Cancel any queued transfers		
	for (nEndpoint = 1; nEndpoint <= ((ci13410->max_endpoints * 2) - 2);
		nEndpoint++) {
		ep = &ci13410->ep[nEndpoint];
		_usb_device_cancel_transfer(ci13410, ep->epNum, ep->direction);
	}

	if (bDebug) printk(KERN_DEBUG "reset_ep0(), SUCCESSFUL\n");
		
	return;
}

static u8 _usb_device_send_data(struct ci13410	*ci13410,
                         u8 ep_num,
                         u8 *buff_ptr,
                         u32 size);
	
static void setDeviceAddress(struct ci13410 *ci13410,
                      u8 setup,
                      struct usb_ctrlrequest *setup_ptr)
{
	static u8 new_address;

	if (setup) {
		new_address = le16_to_cpu(setup_ptr->wValue);
		/* ack */
		_usb_device_send_data(ci13410, 0, 0, 0);
	} else {
		_usb_device_set_status(ci13410, USB_STATUS_ADDRESS,
						new_address);

		_usb_device_set_status(ci13410, USB_STATUS_DEVICE_STATE,
					USB_STATE_ADDRESS);
	}
	return;
}

static void _usb_device_stall_endpoint(struct ci13410 *ci13410,
                                u8 ep_num,
                                u8 direction);
static int ci13410_queue (struct usb_ep *_ep,
                          struct usb_request *_req,
                          gfp_t gfp_flags);	

static void getStatusRequest(struct ci13410 *ci13410,
                      u8 setup,
                      struct usb_ctrlrequest *setup_ptr)
{
	u16 temp_status;
	u8  endpoint;
	
	if (setup) {
		switch (setup_ptr->bRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			// Device request
			_usb_device_get_status(ci13410, USB_STATUS_DEVICE,
						&temp_status);
			break;

		case USB_RECIP_INTERFACE:
			// Interface request
			temp_status = 0;
			break;

		case USB_RECIP_ENDPOINT:
			// Endpoint request
			endpoint = le16_to_cpu(setup_ptr->wIndex)
					& USB_STATUS_ENDPOINT_NUMBER_MASK;
			_usb_device_get_status(ci13410,
				USB_STATUS_ENDPOINT | endpoint, &temp_status);      
			break;
		default:
			// Unknown request
			_usb_device_stall_endpoint(ci13410, 0, 0);
			return;

		}
		
		*ci13410->ep0_buf               = temp_status;
		ci13410->ep0_req->buf           = ci13410->ep0_buf;
		ci13410->ep0_req->length        = 2;
		ci13410_queue(ci13410->gadget.ep0,
				ci13410->ep0_req, GFP_KERNEL);		
	}
	return;
}

static void process_next_request(struct ci13410_ep *ep);

static void clearFeatureRequest(struct ci13410 *ci13410,
                         u8 setup,
                         struct usb_ctrlrequest *setup_ptr)
{
	u8                              endpoint;
	u8                              ep_num;
	u16                             usb_status;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	struct ci13410_ep               *ep;
	//u32                             regVal;		
   
	op_reg_ptr =
		(ci13410_usb_regs __iomem*)ci13410->op_dev_regs_ptr;

	if (setup) {
		_usb_device_get_status(ci13410, USB_STATUS_DEVICE_STATE,
					&usb_status);
	
		if ((usb_status != USB_STATE_CONFIG)
			&& (usb_status != USB_STATE_ADDRESS)) {
			_usb_device_stall_endpoint(ci13410, 0, 0);
			return;
		}
		
		switch (setup_ptr->bRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			// DEVICE
			if (le16_to_cpu(setup_ptr->wValue) == 1) {
				// clear remote wakeup
				_usb_device_get_status(ci13410,
						USB_STATUS_DEVICE, &usb_status);
				usb_status &= ~USB_REMOTE_WAKEUP;
				_usb_device_set_status(ci13410,
						USB_STATUS_DEVICE, usb_status);
			} else {
				_usb_device_stall_endpoint(ci13410, 0, 0);
				return;
			}
			break;
 
		case USB_RECIP_ENDPOINT:
			// ENDPOINT
			if (le16_to_cpu(setup_ptr->wValue) != 0) {
				_usb_device_stall_endpoint(ci13410, 0, 0);
				return;
			}
			
			endpoint = le16_to_cpu(setup_ptr->wIndex)
					& USB_STATUS_ENDPOINT_NUMBER_MASK;

			ep_num   = endpoint & 0x0F;
			
			_usb_device_get_status(ci13410, USB_STATUS_ENDPOINT
						| endpoint, &usb_status);
			
			// clear stall
			_usb_device_set_status(ci13410, USB_STATUS_ENDPOINT
						| endpoint, 0);
			
			// reset the data toggle on the endpoint.
			if (endpoint & 0x80) {	// IN endpoint
#if 0
				regVal = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
				regVal |= EHCI_EPCTRL_TX_DATA_TOGGLE_RST;
				iowrite32(regVal, &op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
#endif			
				ep = &ci13410->ep[ep_num * 2];
			} else {  // OUT endpoint
#if 0
				regVal = ioread32(&op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
				regVal |= EHCI_EPCTRL_RX_DATA_TOGGLE_RST;
				iowrite32(regVal, &op_reg_ptr->registers
					.op_dev_regs.endptctrlx[ep_num]);
#endif			
				ep = &ci13410->ep[(ep_num * 2) - 1];
			}
			
			// status phase
			_usb_device_send_data(ci13410, 0, 0, 0);
			
			// Process next request on the endpoint's request queue
			// if the endpoint was previously stalled otherwise don't
			if (usb_status)
				process_next_request(ep);
			return;
    
		default:
			_usb_device_stall_endpoint(ci13410, 0, 0);
			return;
		}
		
		// status phase
		_usb_device_send_data(ci13410, 0, 0, 0);		
	}
	return;
}

static void setFeatureRequest(struct ci13410 *ci13410,
                       u8 setup,
                       struct usb_ctrlrequest *setup_ptr)
{
	u16 usb_status;
	u8  endpoint;

	if (setup) {
		switch (setup_ptr->bRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			// DEVICE
			if (le16_to_cpu(setup_ptr->wValue) == 1) {
				// set remote wakeup
				_usb_device_get_status(ci13410,
					USB_STATUS_DEVICE, &usb_status);
				usb_status |= USB_REMOTE_WAKEUP;
				_usb_device_set_status(ci13410,
					USB_STATUS_DEVICE, usb_status);
			} else {
				_usb_device_stall_endpoint(ci13410, 0, 0);
				return;
			}
			break;
    
		case USB_RECIP_ENDPOINT:
			// ENDPOINT
			if (le16_to_cpu(setup_ptr->wValue) != 0) {
				_usb_device_stall_endpoint(ci13410, 0, 0);
				return;
			}
			endpoint = le16_to_cpu(setup_ptr->wIndex)
				& USB_STATUS_ENDPOINT_NUMBER_MASK;
			_usb_device_get_status(ci13410, USB_STATUS_ENDPOINT
						| endpoint, &usb_status);
			
			// set stall
			_usb_device_set_status(ci13410, USB_STATUS_ENDPOINT
						| endpoint, 1);
			break;

		default:
			_usb_device_stall_endpoint(ci13410, 0, 0);
			return;

		}
      
		// status phase
		_usb_device_send_data(ci13410, 0, 0, 0);
	}
	return;
}

static void setConfigValue(struct ci13410 *ci13410,
                    u8 setup,
                    struct usb_ctrlrequest *setup_ptr)
{
	u16 usb_state;
   
	if (setup) {
		if ((le16_to_cpu(setup_ptr->wValue) & 0x00FF) > 2) {
			// generate stall
			//_usb_device_stall_endpoint(ci13410, 0, 0);
			return;
		}
		// 0 indicates return to unconfigured state
		if ((le16_to_cpu(setup_ptr->wValue) & 0x00FF) == 0) {
			_usb_device_get_status(ci13410,
					USB_STATUS_DEVICE_STATE, &usb_state);
			if ((usb_state == USB_STATE_CONFIG)
				|| (usb_state == USB_STATE_ADDRESS)) {
				// clear the currently selected config value
				_usb_device_set_status(ci13410,
					USB_STATUS_CURRENT_CONFIG, 0);
				_usb_device_set_status(ci13410,
					USB_STATUS_DEVICE_STATE,
					USB_STATE_ADDRESS);
				// status phase
				//_usb_device_send_data(ci13410, 0, 0, 0);
			} else {
				//_usb_device_stall_endpoint(ci13410, 0, 0);
			}
			return;
		}

		/*
		** If the configuration value (setup_ptr->VALUE & 0x00FF)
		** differs
		** from the current configuration value, then endpoints must be
		** reconfigured to match the new device configuration
		*/
		_usb_device_get_status(ci13410, USB_STATUS_CURRENT_CONFIG,
					&usb_state);
		if (usb_state != (le16_to_cpu(setup_ptr->wValue) & 0x00FF)) {
			// Reconfigure endpoints here
			switch (le16_to_cpu(setup_ptr->wValue) & 0x00FF) {
				default:
				break;
			}
			_usb_device_set_status(ci13410,
						USB_STATUS_CURRENT_CONFIG,
						le16_to_cpu(setup_ptr->wValue)
						& 0x00FF);
			_usb_device_set_status(ci13410,
						USB_STATUS_DEVICE_STATE,
						USB_STATE_CONFIG);      
			// status phase
			//_usb_device_send_data(ci13410, 0, 0, 0);
			return;
		}
		
		_usb_device_set_status(ci13410, USB_STATUS_DEVICE_STATE,
					USB_STATE_CONFIG);
		// status phase
		//_usb_device_send_data(ci13410, 0, 0, 0);
	}
	return;
}


static u8 _usb_device_recv_data(struct ci13410	*ci13410,
                         u8 ep_num,
                         u8* buff_ptr,
                         u32 size);
	
static void process_next_request(struct ci13410_ep *ep)
{
	u8                     bDebug = FALSE;
	struct ci13410_request *req = NULL;
	
	// process next request on the endpoint's request queue
	if (!list_empty(&ep->queue)) {
		req = list_entry(ep->queue.next, struct ci13410_request, queue);
			
		if (ep->epNum != 0) {
			if (!ep->halted) {		
				if (ep->direction == USB_SEND) {
					if (bDebug)
					printk(KERN_DEBUG "process_next_request()"
					"Write %d bytes to %s\n",
					req->req.length, ep->ep.name);
					
					_usb_device_send_data(ep->ci13410,
							ep->epNum,
							(u8*)req->req.dma,
							req->req.length);
				} else {				
					if (bDebug)
					printk(KERN_DEBUG "process_next_request()"
					" Read %d bytes from %s\n",
					req->req.length, ep->ep.name);
					_usb_device_recv_data(ep->ci13410, 
							ep->epNum,
							(u8*)req->req.dma,
							req->req.length);
				}
			}
		} else {
			printk(KERN_DEBUG "process_next_request()"
				" Unexpected request on %s request queue,"
				" length = %d\n", ep->ep.name, req->req.length);
		}
	}
}
	
static void transfer_complete(struct ci13410 *ci13410,
                              struct ci13410_ep *ep,
                              u32 length)
{
	struct ci13410_request *req;
	u8                     processNextReq = 1;
	
	if (!list_empty(&ep->queue)) {
		req = list_entry(ep->queue.next, struct ci13410_request, queue);		

		// delete entry from queue	
		list_del_init(&req->queue);
		
		if(list_empty(&ep->queue))
			processNextReq = 0;
	
		req->req.actual = length;
		req->req.status = 0;
		
		// Just return if this is an internal request on ep0
		if (req->internal)
			return;
					
		if (likely(req->req.complete)) {
			// Call gadget driver's complete callback
			spin_unlock(&ci13410->lock);
			req->req.complete(&ep->ep, &req->req);
			spin_lock(&ci13410->lock);
		}

		if (processNextReq)
			process_next_request(ep);
	}
}

static void service_ep0(struct ci13410 *ci13410,
                 u8 setup,
                 u8 direction,
                 u8 *buffer_ptr,
                 u32 length,
                 u8 errors)
{
	struct ci13410_ep *ep;
	int               rc;
	u8		  send_to_gadget_driver = 1;
	
	if (setup) {
		ep = &ci13410->ep[0];
		
		_usb_device_get_setup_data(ci13410, 0, (u8*)&g_setupPacket);
		
		if (!(g_setupPacket.bRequestType & 0x80))
			// Setup with data OUT Host-to-Device
			ci13410->ep[0].direction = USB_RECV;
		else
			// Setup with data IN Device-to-Host
			ci13410->ep[0].direction = USB_SEND;
	}

	// Check request and handle these standard requests
	if ((g_setupPacket.bRequestType & USB_TYPE_MASK)
		== USB_TYPE_STANDARD) {
		switch (g_setupPacket.bRequest) {
		case USB_REQ_GET_STATUS:
			getStatusRequest(ci13410, setup, &g_setupPacket);
			send_to_gadget_driver = 0;
			break;
		case USB_REQ_CLEAR_FEATURE:
			clearFeatureRequest(ci13410, setup, &g_setupPacket);
			send_to_gadget_driver = 0;
			break;
		case USB_REQ_SET_FEATURE:
			setFeatureRequest(ci13410, setup, &g_setupPacket);
			send_to_gadget_driver = 0;
			break;
		case USB_REQ_SET_ADDRESS:
			setDeviceAddress(ci13410, setup, &g_setupPacket);
			send_to_gadget_driver = 0;
			break;
		case USB_REQ_SET_CONFIGURATION:
			setConfigValue(ci13410, setup, &g_setupPacket);
			send_to_gadget_driver = 1;
			break;
		default:
			break;
		}
	}
	
	if ((g_setupPacket.bRequestType & USB_TYPE_MASK)
		== USB_TYPE_CLASS) {
		switch (g_setupPacket.bRequest) {
		case HID_REQ_SET_REPORT:
			ci13410->ep[0].direction = USB_RECV;
			send_to_gadget_driver = 1;
			break;
		default:
			break;
		}
	}
	
	if (setup && send_to_gadget_driver) {
		// Call gadget driver's setup callback
		spin_unlock (&ci13410->lock);		
		rc = ci13410->driver->setup(&ci13410->gadget, &g_setupPacket);
		spin_lock (&ci13410->lock);
		
		if (rc < 0) {
			printk(KERN_ALERT "service_ep0()"
				" driver->setup returned %d, stalling ep\n",rc);
			printk(KERN_ALERT "setup.bRequestType = 0x%02X\n",
						g_setupPacket.bRequestType);
			printk(KERN_ALERT "setup.bRequest = 0x%02X\n",
						g_setupPacket.bRequest);
			printk(KERN_ALERT "setup.wValue = 0x%04X\n",
					le16_to_cpu(g_setupPacket.wValue));
			printk(KERN_ALERT "setup.wIndex = 0x%04X\n",
					le16_to_cpu(g_setupPacket.wIndex));
			printk(KERN_ALERT "setup.wLength = 0x%04X\n",
					le16_to_cpu(g_setupPacket.wLength));
						 
			_usb_device_stall_endpoint(ci13410, 0, 0);
			return;
		}
		
	}		
	
	if(buffer_ptr) {
		ep = &ci13410->ep[0];
		transfer_complete(ci13410, ep, length);
	}	
		
	return;
}

static void service_epn(struct ci13410 *ci13410,
                 u8 ep_num,
                 u8 direction,
                 u8 *buffer_ptr,
                 u32 length,
                 u8 errors)
{
	struct ci13410_ep *ep;
		
	if (buffer_ptr) {
		if (direction == USB_RECV)  // OUT
			ep = &ci13410->ep[(ep_num * 2) - 1];
		else  // IN
			ep = &ci13410->ep[ep_num * 2];
		
		transfer_complete(ci13410, ep, length);
	}	
}

/*********************Interrupt Handler***************************************/
//static struct ci13410 *the_controller;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
static irqreturn_t ci13410_irq (int irq, void *_ci13410, struct pt_regs *regs)
#else
static irqreturn_t ci13410_irq (int irq, void *_ci13410)
#endif
{
	u8                              bDebug = FALSE;
	struct ci13410                  *ci13410 = _ci13410;
	ci13410_usb_regs                __iomem *op_reg_ptr;
	u32                             status = 0x12345678;

	//if (bDebug) printk(KERN_DEBUG "ci13410_irq() called...\n");
		
#if defined(CONFIG_MIPS_VSC452)
	{
		u64 ior = read_ior();
		
		// if the hw interrupt is not for USB
		//if (!(ior & IOR_USBIS1))
		if (!(ior))
			/*printk(KERN_DEBUG "\n\AvctIrqHandler()"
					" ior = %Lx (%Lx)\n",ior,IOR_USBIS1); */
			return(IRQ_NONE);       /*
						**just ack the kernel that it is
						** not handled here and return
						*/
	}
#endif
	
	spin_lock(&ci13410->lock);
	
	op_reg_ptr =
		(ci13410_usb_regs __iomem *)ci13410->op_dev_regs_ptr;
	
	if (!ci13410 || !op_reg_ptr)
		return(IRQ_NONE);
	
	for(;;) {
		status = ioread32(&op_reg_ptr->registers.op_dev_regs.usb_sts);
		
		/* Clear all the interrupts occured */
		iowrite32(status, &op_reg_ptr->registers.op_dev_regs.usb_sts);

		if (!(status & ioread32(&op_reg_ptr->registers
						.op_dev_regs.usb_intr))) {
			break;
		}
				
		if (status & EHCI_STS_RESET) {
			if (bDebug) printk(KERN_DEBUG "ci13410_irq()"
					" - Reset interrupt occurred.\n");
			_usb_device_process_reset(ci13410);
		}

		if (status & EHCI_STS_PORT_CHANGE) {
			if (bDebug) printk(KERN_DEBUG "ci13410_irq()"
					" - Port change interrupt occurred.\n");
			_usb_device_process_port_change(ci13410);
		}
	
		if (status & EHCI_STS_ERR) {
			/*if (bDebug) printk(KERN_DEBUG "ci13410_irq() - Error"
						" interrupt occurred.\n"); */
		}
		
		if (status & EHCI_STS_SOF) {
			/* if (bDebug) printk(KERN_DEBUG "ci13410_irq() - SOF"
						" interrupt occurred.\n"); */
		}
		
		if (status & EHCI_STS_INT) {
			if (bDebug) printk(KERN_DEBUG "ci13410_irq()"
					" - TR complete interrupt occurred.\n");
			_usb_device_process_tr_complete(ci13410);
		}
	
		if (status & EHCI_STS_SUSPEND) {
			/* if (bDebug) printk(KERN_DEBUG "ci13410_irq()"
		 			" - Suspend interrupt occurred.\n"); */
		}
	}
    
	spin_unlock(&ci13410->lock);	
	return(IRQ_HANDLED);
}
/*****************************************************************************/

/*************************Gadget Endpoint API*********************************/
static int ci13410_enable(struct usb_ep *_ep,
                          const struct usb_endpoint_descriptor *desc)
{
	struct ci13410_ep *ep;
	unsigned long     flags;
	int               ret = 0;

	ep = container_of(_ep, struct ci13410_ep, ep);
	
	ep->desc         = desc;
	ep->ep.maxpacket = desc->wMaxPacketSize;
	
	spin_lock_irqsave(&ep->ci13410->lock, flags);	
	
	printk(KERN_DEBUG "ci13410_enable() %s maxpacket = 0x%04X\n", 
				ep->ep.name, le16_to_cpu(ep->ep.maxpacket));
		
	_usb_device_init_endpoint(ep->ci13410,
				ep->epNum,
				le16_to_cpu(ep->ep.maxpacket),
				ep->direction,
				(desc->bmAttributes
				& USB_ENDPOINT_XFERTYPE_MASK),
				USB_DEVICE_DONT_ZERO_TERMINATE);

	spin_unlock_irqrestore(&ep->ci13410->lock, flags);	
	
	return ret;	
}

static int ci13410_disable(struct usb_ep *_ep)
{
	struct ci13410_ep *ep;
	unsigned long     flags;
	int               ret = 0;

	ep = container_of(_ep, struct ci13410_ep, ep);
		
	if(ep->epNum == 0)
		return ret;

	spin_lock_irqsave(&ep->ci13410->lock, flags);	
	ep->halted       = 0;
	ep->ep.maxpacket = 512;

	//printk(KERN_DEBUG "ci13410_disable() %s\n", ep->ep.name);		
	nuke(ep);	

	_usb_device_deinit_endpoint(ep->ci13410, ep->epNum, ep->direction);
	
	spin_unlock_irqrestore(&ep->ci13410->lock, flags);
	
	return ret;
}

static struct usb_request * ci13410_alloc_request(struct usb_ep *_ep,
                                                  gfp_t gfp_flags)
{
	struct ci13410_request *req;
	
	req = usb_memalloc(sizeof(struct ci13410_request));
	if (!req)
		return NULL;
		
	memset(req, 0, sizeof(struct ci13410_request));
	INIT_LIST_HEAD(&req->queue);

	return &req->req;	
}

static void ci13410_free_request(struct usb_ep *_ep, struct usb_request *_req)
{
	struct ci13410_request *req;

	req = container_of(_req, struct ci13410_request, req);
	if (req) {
		usb_memfree(req);
	}	
}

static void * ci13410_alloc_buffer(struct usb_ep *_ep,
                                   unsigned bytes,
                                   dma_addr_t *dma,
                                   gfp_t gfp_flags)
{
	void *buf;
	
	*dma = DMA_ADDR_INVALID;	
	buf = dma_alloc_coherent(the_controller->gadget.dev.parent,
				bytes, dma, gfp_flags);

	return buf;	
}

static void ci13410_free_buffer(struct usb_ep *_ep,
                                void *buf,
                                dma_addr_t dma,
                                unsigned bytes)
{	
	if (dma != DMA_ADDR_INVALID) {
		dma_free_coherent(the_controller->gadget.dev.parent,
					bytes, buf, dma);
	}	
}

static int ci13410_queue(struct usb_ep *_ep,
                         struct usb_request *_req,
                         gfp_t gfp_flags)
{
	u8                     bDebug = FALSE;	
	struct ci13410_ep      *ep;
	struct ci13410_request *req;
	unsigned long          flags;
	int                    request = 0;
	u8                     add_to_q = 1;
	
	ep  = container_of(_ep, struct ci13410_ep, ep);
	req = container_of(_req, struct ci13410_request, req);
	
	if (req->req.length == 0) {
		if (bDebug) printk(KERN_DEBUG "ci13410_queue() Recieved %d length"
				" request for endpoint %s, buf = 0x%08X\n",
				req->req.length, ep->ep.name,
				(u32)req->req.buf);
				
		if ((ep->epNum != 0) ||
		    ((ep->epNum == 0) && (ep->direction == USB_SEND)))
			return -EOPNOTSUPP;
			
		add_to_q = 0;
	}
	
	if (ep->epNum == 0 && !list_empty(&ep->queue)) {
		// if ep0 queue is not empty don't process this 
		// request
		printk(KERN_ERR "ci13410_queue() ep0 %d bytes"
					" request already in progress!!!\n",
					req->req.length);
		return -EINPROGRESS;
	}	
	
	spin_lock_irqsave(&ep->ci13410->lock, flags);
	
	if (list_empty(&ep->queue))
		request = 1;	

	if (bDebug) printk(KERN_DEBUG "ci13410_queue() %s\n", ep->ep.name);
	
	if (add_to_q) {
		list_add_tail(&req->queue, &ep->queue);
		req->req.status	= -EINPROGRESS;
		req->req.actual = 0;
	}
	
	if (ep->epNum == 0) { // control endpoint
		if (ep->direction == USB_SEND) {
			if (bDebug) printk(KERN_DEBUG "ci13410_queue()"
					" Write %d bytes"
					" to endpoint %s, buf = 0x%08X\n",
					req->req.length, ep->ep.name,
					(u32)req->req.buf);
			
			_usb_device_send_data(ep->ci13410, ep->epNum,
						(u8*)req->req.dma, req->req.length);
			
			// Status phase
			_usb_device_recv_data(ep->ci13410, 0, 0, 0);
		} else {
			if (bDebug) printk(KERN_DEBUG "ci13410_queue()"
					" Read %d bytes"
					" from endpoint %s, buf = 0x%08X\n",
					req->req.length, ep->ep.name,
					(u32)req->req.dma);
			
			if (req->req.length > 0)
				_usb_device_recv_data(ep->ci13410, ep->epNum,
						(u8*)req->req.dma, req->req.length);
			
			// Status phase
			_usb_device_send_data(ep->ci13410, 0, 0, 0);			
		}
	} else {// INT or BULK endpoint
		if (request && !ep->halted) {
			if (ep->direction == USB_SEND) {
				if (bDebug) printk(KERN_DEBUG "ci13410_queue()"
						" Write %d"
						" bytes to endpoint %s\n",
						req->req.length, ep->ep.name);
				
				_usb_device_send_data(ep->ci13410, ep->epNum,
						(u8*)req->req.dma, req->req.length);
			} else {				
				if (bDebug) printk(KERN_DEBUG "ci13410_queue()"
						" Read %d"
						" bytes from endpoint %s\n",
						req->req.length, ep->ep.name);
				
				_usb_device_recv_data(ep->ci13410, ep->epNum,
						(u8*)req->req.dma, req->req.length);		
			}
		} else {
			if (bDebug) printk(KERN_DEBUG "ci13410_queue()"
					" Queue %d bytes"
					" for endpoint %s\n", req->req.length,
					ep->ep.name);
		}
	}
	
	spin_unlock_irqrestore(&ep->ci13410->lock, flags);

	return 0;	
}


static int ci13410_dequeue (struct usb_ep *_ep, struct usb_request *_req)
{
	struct ci13410_ep      *ep;
	struct ci13410_request *req;
	unsigned long          flags;

	ep  = container_of(_ep, struct ci13410_ep, ep);
	req = container_of(_req, struct ci13410_request, req);
	
	if (ep) {
		spin_lock_irqsave(&ep->ci13410->lock, flags);	
		if (!list_empty(&ep->queue)) {
			if (req) {
				req->req.status	= -ESHUTDOWN;
				list_del_init (&req->queue);
				if (likely(req->req.complete)) {
					req->req.complete (&ep->ep, &req->req);
				}
			}
		}
		spin_unlock_irqrestore(&ep->ci13410->lock, flags);
	}
	
	return 0;	
}

static int ci13410_set_halt (struct usb_ep *_ep, int value)
{
	struct ci13410_ep      *ep;
	struct ci13410_request *req;		
	unsigned long          flags;
	int                    ret = 0;
	
	ep  = container_of(_ep, struct ci13410_ep, ep);
	req = list_entry(ep->queue.next, struct ci13410_request, queue);	
		
	spin_lock_irqsave(&ep->ci13410->lock, flags);
	if (!list_empty(&ep->queue)) {
		ret = -EAGAIN;
		goto out;
	}	
	
	if (value) {
		// printk(KERN_DEBUG "ci13410_set_halt() set halt for %s\n",
		//		ep->ep.name);
		_usb_device_stall_endpoint(ep->ci13410, ep->epNum,
						ep->direction);
	} else {
		//printk(KERN_DEBUG "ci13410_set_halt() clear halt for %s\n",
		//		ep->ep.name);
		_usb_device_unstall_endpoint(ep->ci13410, ep->epNum,
						ep->direction);
	}

out:	
	spin_unlock_irqrestore(&ep->ci13410->lock, flags);
	return ret;	
}

static int ci13410_fifo_status (struct usb_ep *_ep)
{
	//printk(KERN_DEBUG "ci13410_fifo_status()\n");
	return -EOPNOTSUPP;
}

static void ci13410_fifo_flush (struct usb_ep *_ep)
{
	struct ci13410_ep *ep;
	unsigned long     flags;

	ep = container_of(_ep, struct ci13410_ep, ep);

	spin_lock_irqsave(&ep->ci13410->lock, flags);	
	if (list_empty(&ep->queue) && !ep->halted) {
		//printk(KERN_DEBUG "ci13410_fifo_flush() flush %s\n", ep->ep.name);
		_usb_device_cancel_transfer(ep->ci13410, ep->epNum,
						ep->direction);
	}
	spin_unlock_irqrestore(&ep->ci13410->lock, flags);
}

static struct usb_ep_ops ci13410_ep_ops = {
	.enable                 = ci13410_enable,
	.disable                = ci13410_disable,

	.alloc_request          = ci13410_alloc_request,
	.free_request           = ci13410_free_request,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	.alloc_buffer           = ci13410_alloc_buffer,
	.free_buffer            = ci13410_free_buffer,
#endif

	.queue                  = ci13410_queue,
	.dequeue                = ci13410_dequeue,

	.set_halt               = ci13410_set_halt,
	.fifo_status            = ci13410_fifo_status,
	.fifo_flush             = ci13410_fifo_flush,
};
/*****************************************************************************/

/**************************Gadget API*****************************************/
static int ci13410_get_frame (struct usb_gadget *_gadget)
{
	//printk(KERN_DEBUG "ci13410_get_frame()\n");	
	return 0;
}

static int ci13410_wakeup (struct usb_gadget *_gadget)
{
	//printk(KERN_DEBUG "ci13410_wakeup()\n");	
	return 0;
}

static int ci13410_set_selfpowered (struct usb_gadget *_gadget, int value)
{
	//printk(KERN_DEBUG "ci13410_set_selfpowered()\n");
	return 0;
}

static int ci13410_pullup(struct usb_gadget *_gadget, int is_on)
{
	struct ci13410 *ci13410 = the_controller;
	unsigned long  flags;

	spin_lock_irqsave(&ci13410->lock, flags);
	//printk(KERN_DEBUG "ci13410_pullup() is_on = %d\n", is_on);
	if (is_on)
		_usb_device_chip_initialize(ci13410);
	else {
		// Cancel all transfers
		reset_ep0(ci13410, 0, 0, 0, 0, 0);		
		// Stop the chip
		device_reset(ci13410);
	}
	spin_unlock_irqrestore(&ci13410->lock, flags);

	return 0;
}

static const struct usb_gadget_ops ci13410_gadget_ops = {
	.get_frame              = ci13410_get_frame,
	.wakeup                 = ci13410_wakeup,
	.set_selfpowered        = ci13410_set_selfpowered,
	.pullup                 = ci13410_pullup,
};
/*****************************************************************************/

static void stop_activity(struct ci13410 *ci13410,
                          struct usb_gadget_driver *driver)
{
	unsigned i;

	if (ci13410->gadget.speed == USB_SPEED_UNKNOWN)
		driver = NULL;

	if (driver) {
		spin_unlock(&ci13410->lock);
		//call gadget driver's disconnect callback
		driver->disconnect(&ci13410->gadget);
		spin_lock(&ci13410->lock);
	}

	// Cancel queued up requests
	for (i = 0; i < ((ci13410->max_endpoints * 2) - 1); i++)
		nuke(&ci13410->ep[i]);
}

/*****************Gadget Driver Register/Unregister***************************/

int ci13410_proc(char *buf, char **start, off_t offset, int count, int *eof,
	     void *data)
{
	int nLen = 0;
	int i;

	nLen +=
	    sprintf((buf + nLen),
		    "**** USB Driver (Version %s) for %s ****\n",
		    DRIVER_VERSION, CI13410_CORE_TYPE);
		    
	if (the_controller) {
		nLen += sprintf((buf + nLen), "ID_REGISTERS:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "USB Core ID:                    0x%08X\n",
			    ioread32(&the_controller->id_reg_ptr->registers.
			    			id_regs.id));
#if defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
		nLen +=
		    sprintf((buf + nLen),
			    "SYS_BUS_CONFIG:                 0x%08X\n",
			    ioread32(&the_controller->id_reg_ptr->registers.
			    			id_regs.sys_bus_config));
#endif
			    			
		nLen += sprintf((buf + nLen), "\n");

		nLen += sprintf((buf + nLen), "CAPABILITY_REGISTERS:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "CAPLENGTH_HCIVER:               0x%08X\n",
			    ioread32(&the_controller->cap_reg_ptr->registers.
			    			cap_regs.caplength_hciver));
		nLen +=
		    sprintf((buf + nLen),
			    "HCS_PARAMS:                     0x%08X\n",
			    ioread32(&the_controller->cap_reg_ptr->registers.
			    				cap_regs.hcs_params));
		nLen +=
		    sprintf((buf + nLen),
			    "HCC_PARAMS:                     0x%08X\n",
			    ioread32(&the_controller->cap_reg_ptr->registers.
			    				cap_regs.hcc_params));
		nLen +=
		    sprintf((buf + nLen),
			    "DCI_VERSION:                    0x%08X\n",
			    ioread32(&the_controller->cap_reg_ptr->registers.
			    				cap_regs.dci_version));
		nLen +=
		    sprintf((buf + nLen),
			    "DCC_PARAMS:                     0x%08X\n",
			    ioread32(&the_controller->cap_reg_ptr->registers.
			    				cap_regs.dcc_params));
		nLen += sprintf((buf + nLen), "\n");
	
		nLen += sprintf((buf + nLen), "OPERATIONAL_DEVICE_REGISTERS:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "USB_CMD:                        0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->
			    			registers.op_dev_regs.usb_cmd));
		nLen +=
		    sprintf((buf + nLen),
			    "USB_STS:                        0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    				op_dev_regs.usb_sts));
		nLen +=
		    sprintf((buf + nLen),
			    "USB_INTR:                       0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    				op_dev_regs.usb_intr));
		nLen +=
		    sprintf((buf + nLen),
			    "USB_FRINDEX:                    0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.usb_frindex));
		nLen +=
		    sprintf((buf + nLen),
			    "CTRLDSSEGMENT:                  0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.ctrldssegment));
		nLen +=
		    sprintf((buf + nLen),
			    "DEVICE_ADDR:                    0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.device_addr));
		nLen +=
		    sprintf((buf + nLen),
			    "EP_LIST_ADDR:                   0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.ep_list_addr));			    			
		nLen +=
		    sprintf((buf + nLen),
			    "EP_NAK:                         0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptnak));
			    			
		nLen +=
		    sprintf((buf + nLen),
			    "EP_NAKEN:                       0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptnaken));
		nLen +=
		    sprintf((buf + nLen),
			    "CONFIG_FLAG:                    0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.config_flag));
			    			
		for (i = 0; i < 1; /*VUSBHS_MAX_PORTS; */ i++) {
			nLen +=
			    sprintf((buf + nLen),
				    "PORTSCX[%d]:                     0x%08X\n", i,
				    ioread32(&the_controller->op_dev_regs_ptr->
				    	registers.op_dev_regs.portscx[i]));
		}
		nLen +=
		    sprintf((buf + nLen),
			    "OTGSC:                          0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    				op_dev_regs.otgsc));
		nLen +=
		    sprintf((buf + nLen),
			    "USB_MODE:                       0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    				op_dev_regs.usb_mode));
		nLen +=
		    sprintf((buf + nLen),
			    "ENDPT_SETUP_STAT:               0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endpt_setup_stat));
		nLen +=
		    sprintf((buf + nLen),
			    "ENDPTPRIME:                     0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptprime));
		nLen +=
		    sprintf((buf + nLen),
			    "ENDPTFLUSH:                     0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptflush));
		nLen +=
		    sprintf((buf + nLen),
			    "ENDPTSTATUS:                    0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptstatus));
		nLen +=
		    sprintf((buf + nLen),
			    "ENDPTCOMPLETE:                  0x%08X\n",
			    ioread32(&the_controller->op_dev_regs_ptr->registers.
			    			op_dev_regs.endptcomplete));
#if 1
		for (i = 0; i < USB_MAX_ENDPOINTS; i++) {
			nLen +=
			    sprintf((buf + nLen),
				    "ENDPTCTRLX[%d]:                  0x%08X\n", i,
				    ioread32(&the_controller->op_dev_regs_ptr->
				    	registers.op_dev_regs.endptctrlx[i]));
		}
#endif
		nLen += sprintf((buf + nLen), "\n");
		
#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
		nLen += sprintf((buf + nLen), "GCR_REGISTERS:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "MFSEL1:                         0x%08X\n",
			    ioread32(&the_controller->gcr_reg_ptr->
			    			registers.gcr_regs.mfsel1));
		nLen +=
		    sprintf((buf + nLen),
			    "INTCR:                          0x%08X\n",
			    ioread32(&the_controller->gcr_reg_ptr->registers.
			    				gcr_regs.intcr));
			    				
		nLen += sprintf((buf + nLen), "\n");
		
		nLen += sprintf((buf + nLen), "CLK_REGISTERS:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "CLKSEL:                         0x%08X\n",
			    ioread32(&the_controller->clk_reg_ptr->
			    			registers.clk_regs.clksel));
			    				
		nLen += sprintf((buf + nLen), "\n");			    				
#endif		

#if 1
		nLen += sprintf((buf + nLen), "USB_DEV_STATE_STRUCT:\n");
		nLen +=
		    sprintf((buf + nLen),
			    "the_controller:                 0x%08X\n",
			    (int) the_controller);
		nLen +=
		    sprintf((buf + nLen),
			    "BUS_RESETTING:                  %s\n",
			    the_controller->
			    bus_resetting ? "TRUE" : "FALSE");
		nLen +=
		    sprintf((buf + nLen),
			    "TRANSFER_PENDING:               %s\n",
			    the_controller->
			    transfer_pending ? "TRUE" : "FALSE");
		nLen +=
		    sprintf((buf + nLen),
			    "CAP_PTR:                        0x%08X\n",
			    (u32) the_controller->cap_reg_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "OP_DEV_PTR:                     0x%08X\n",
			    (u32) the_controller->op_dev_regs_ptr);
#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
		nLen +=
		    sprintf((buf + nLen),
			    "GCR_PTR:                        0x%08X\n",
			    (u32) the_controller->gcr_reg_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "CLK_PTR:                        0x%08X\n",
			    (u32) the_controller->clk_reg_ptr);
#endif
		nLen +=
		    sprintf((buf + nLen),
			    "TEMP_XD_PTR:                    0x%08X\n",
			    (u32) the_controller->temp_xd_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "XD_BASE:                        0x%08X\n",
			    (u32) the_controller->xd_base);
		nLen +=
		    sprintf((buf + nLen),
			    "XD_HEAD:                        0x%08X\n",
			    (u32) the_controller->xd_head);
		nLen +=
		    sprintf((buf + nLen),
			    "XD_TAIL:                        0x%08X\n",
			    (u32) the_controller->xd_tail);
		nLen +=
		    sprintf((buf + nLen),
			    "PENDING_XD_PTR:                 0x%08X\n",
			    (u32) the_controller->pending_xd_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "XD_ENTRIES:                     %d\n",
			    (int) the_controller->xd_entries);
		nLen +=
		    sprintf((buf + nLen),
			    "EP_QUEUE_HEAD_PTR:              0x%08X\n",
			    (u32) the_controller->ep_queue_head_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "DMA_EP_QUEUE_HEAD_PTR:          0x%08X\n",
			    (u32) the_controller->dma_ep_queue_head_ptr);			    
		nLen +=
		    sprintf((buf + nLen),
			    "DRIVER_MEMORY:                  0x%08X\n",
			    (u32) the_controller->driver_memory);			    
		nLen +=
		    sprintf((buf + nLen),
			    "EP_QUEUE_HEAD_BASE:             0x%08X\n",
			    (u32) the_controller->ep_queue_head_base);
		nLen +=
		    sprintf((buf + nLen),
			    "DMA_EP_QUEUE_HEAD_BASE:         0x%08X\n",
			    (u32) the_controller->dma_ep_queue_head_base);			    
		nLen +=
		    sprintf((buf + nLen),
			    "DTD_BASE_PTR:                   0x%08X\n",
			    (u32) the_controller->dtd_base_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "DMA_DTD_BASE_PTR:               0x%08X\n",
			    (u32) the_controller->dma_dtd_base_ptr);			    
		nLen +=
		    sprintf((buf + nLen),
			    "DTD_ALIGNED_BASE_PTR:           0x%08X\n",
			    (u32) the_controller->dtd_aligned_base_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "DMA_DTD_ALIGNED_BASE_PTR:       0x%08X\n",
			    (u32) the_controller->dma_dtd_aligned_base_ptr);
		nLen +=
		    sprintf((buf + nLen),
			    "DTD_HEAD:                       0x%08X\n",
			    (u32) the_controller->dtd_head);
		nLen +=
		    sprintf((buf + nLen),
			    "DTD_TAIL:                       0x%08X\n",
			    (u32) the_controller->dtd_tail);
#endif
#if 1
		for (i = 0; i <= 1; /*(USB_MAX_ENDPOINTS * 2) */ i++) {
			nLen +=
			    sprintf((buf + nLen),
				    "EP_DTD_HEADS[%d]:                0x%08X\n",
				    i,
				    (u32) the_controller->ep_dtd_heads[i]);
			nLen +=
			    sprintf((buf + nLen),
				    "EP_DTD_TAILS[%d]:                0x%08X\n",
				    i,
				    (u32) the_controller->ep_dtd_tails[i]);
		}
#endif
#if 1
		nLen +=
		    sprintf((buf + nLen),
			    "XD_SCRATCH_STRUCT_BASE:         0x%08X\n",
			    (u32) the_controller->xd_scratch_struct_base);
		nLen +=
		    sprintf((buf + nLen),
			    "SCRATCH_STRUCT_BASE:            0x%08X\n",
			    (u32) the_controller->scratch_struct_base);
		nLen +=
		    sprintf((buf + nLen),
			    "USB_STATE:                      %d\n",
			    the_controller->usb_state);
		nLen +=
		    sprintf((buf + nLen),
			    "USB_DEVICE_STATE:               %d\n",
			    the_controller->usb_device_state);
		nLen +=
		    sprintf((buf + nLen),
			    "USB_SOF_COUNT:                  %d\n",
			    the_controller->usb_sof_count);
		nLen +=
		    sprintf((buf + nLen),
			    "DTD_ENTRIES:                    %d\n",
			    the_controller->dtd_entries);
		nLen +=
		    sprintf((buf + nLen),
			    "USB_DEV_STATE_B4_SUSPEND:       %d\n",
			    the_controller->usb_dev_state_b4_suspend);
		nLen +=
		    sprintf((buf + nLen),
			    "MAX_ENDPOINTS:                  %d\n",
			    (int) the_controller->max_endpoints);
		nLen +=
		    sprintf((buf + nLen),
			    "USB_CURR_CONFIG:                %d\n",
			    (int) the_controller->usb_curr_config);
		nLen +=
		    sprintf((buf + nLen),
			    "DEVICE_ADDRESS:                 %d\n",
			    (int) the_controller->device_address);
#endif
	}
	// *** All done ***
	*eof = 1;

	return (nLen);
}

	
/* when a driver is successfully registered, it will receive
 * control requests including set_configuration(), which enables
 * non-control requests.  then usb traffic follows until a
 * disconnect is reported.  then a host may connect again, or
 * the driver might get unbound.
 */
#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
static int _usb_gadget_register_driver(struct usb_gadget_driver *driver)
#else
int usb_gadget_register_driver(struct usb_gadget_driver *driver)
#endif
{
	struct ci13410 *ci13410 = the_controller;
	int            retval;

	//printk(KERN_DEBUG "usb_gadget_register_driver()\n");

	if (!driver
		|| !driver->bind
		|| !driver->unbind
		|| !driver->setup)
		return -EINVAL;
	if (!ci13410)
		return -ENODEV;
	if (ci13410->driver)
		return -EBUSY;

	/* hook up the driver */
	driver->driver.bus = NULL;
	ci13410->driver = driver;
	ci13410->gadget.dev.driver = &driver->driver;

	retval = device_add(&ci13410->gadget.dev);
	retval = driver->bind(&ci13410->gadget);
	if (retval) {
		//err("bind to driver error");
		printk(KERN_ERR "usb_gadget_register_driver()"
				" bind to driver '%s'"
				" ERROR\n", ci13410->driver->function );
		ci13410->driver = NULL;
		ci13410->gadget.dev.driver = NULL;
		return retval;
	}
	
	printk(KERN_DEBUG "usb_gadget_register_driver() bind to driver '%s'"
			" SUCCESSFUL\n", ci13410->driver->function );

	/* Initialize and start the chip */
	//_usb_device_chip_initialize(ci13410);

	return 0;	
}

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
static int _usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
#else
int usb_gadget_unregister_driver(struct usb_gadget_driver *driver)
#endif
{
	struct ci13410 *ci13410 = the_controller;
	unsigned long  flags;	

	//printk(KERN_DEBUG "usb_gadget_unregister_driver()\n");
	
	if (!ci13410)
		return -ENODEV;
	if (!driver || driver != ci13410->driver)
		return -EINVAL;	
	
	spin_lock_irqsave(&ci13410->lock, flags);
	ci13410->driver = NULL;
	/* Reset the chip */
	device_reset(ci13410);
	stop_activity(ci13410, driver);
	spin_unlock_irqrestore(&ci13410->lock, flags);

	driver->unbind(&ci13410->gadget);
	
	//printk(KERN_DEBUG "usb_gadget_unregister_driver() SUCCESSFUL\n");
	
	return 0;
}

#if defined(CONFIG_USB_GADGET_DUAL_CONTROLLERS)
//EXPORT_SYMBOL(_usb_gadget_register_driver);
//EXPORT_SYMBOL(_usb_gadget_unregister_driver);
#else
EXPORT_SYMBOL(usb_gadget_register_driver);
EXPORT_SYMBOL(usb_gadget_unregister_driver);
#endif

/*****************************************************************************/

#if defined (CONFIG_USB_GADGET_DUAL_CONTROLLERS)
extern int usb_gadget_controller_register(
		int (*gadget_register) (struct usb_gadget_driver *driver),
		int (*gadget_unregister) (struct usb_gadget_driver *driver));

extern int usb_gadget_controller_unregister(struct usb_gadget *gadget);
#endif

/************Gadget(Peripheral Controller Driver) probe/remove****************/
#define resource_len(r) (((r)->end - (r)->start) + 1)
//static int __init ci13410_probe(struct device *dev)
static int ci13410_probe(struct device *dev)
{
	struct platform_device         *pdev;
	struct resource                *addr = NULL;
	int                            irq = -1;
	void                   __iomem *reg = NULL;
	struct ci13410                 *ci13410 = NULL;
	int                            ret = 0;
	int                            i;
	struct ci13410_request         *req;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	u32                            irqFlags = SA_INTERRUPT;
#else
	u32                            irqFlags = IRQF_DISABLED;
#endif

	//printk(KERN_DEBUG "ci13410_probe()\n");


	pdev = container_of(dev, struct platform_device, dev);
	if (pdev->num_resources < 2)
		return -ENODEV;
	if (strcmp(pdev->name, DRIVER_NAME) != 0) {
		printk(KERN_ERR "ci13410_probe() resource not found\n");
		return -ENODEV;
	}

	addr = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	
	irq = platform_get_irq(pdev, 0);
	
	if (!addr || (irq < 0))
		return -ENODEV;	

	// Map USB registers
	reg = ioremap(addr->start, resource_len(addr));
	if (reg == NULL) {
		ret = -ENOMEM;
		printk(KERN_ERR "ci13410_probe() USB ioremap error.\n");
		goto clean_up;
	}

	/* initialize device state struct */
	ci13410 = usb_memalloc(sizeof(struct ci13410));
	if (ci13410 == NULL) {
		ret = -ENOMEM;
		printk(KERN_ERR "ci13410_probe() USB_memalloc error\n");
		goto clean_up;
	}
	
	memset(ci13410, 0, sizeof(struct ci13410));
	spin_lock_init(&ci13410->lock);
	dev_set_drvdata(dev, ci13410);

	ci13410->gadget.ops = &ci13410_gadget_ops;
	device_initialize(&ci13410->gadget.dev);
	strcpy(ci13410->gadget.dev.bus_id, "gadget");

#ifdef CI13410_HIGH_SPEED_DRIVER
	ci13410->gadget.is_dualspeed    = 1;
#else
	ci13410->gadget.is_dualspeed    = 0;
#endif
	ci13410->gadget.dev.parent      = dev;
	ci13410->gadget.dev.dma_mask    = pdev->dev.dma_mask;
	ci13410->gadget.dev.release     = pdev->dev.release;
	ci13410->gadget.name            = DRIVER_NAME;
	
	ci13410->reg                    = reg;
	ci13410->id_reg_ptr             = (ci13410_usb_regs __iomem *) (u32)reg;
	ci13410->cap_reg_ptr            = (ci13410_usb_regs __iomem *)
						((u32)reg + 0x0100);
	ci13410->op_dev_regs_ptr = (ci13410_usb_regs __iomem *)
					((u32)ci13410->cap_reg_ptr +
					(ioread32(&ci13410->cap_reg_ptr
					->registers.cap_regs.caplength_hciver)
					& EHCI_CAP_LEN_MASK));

#if 0
	printk(KERN_DEBUG "ci13410_probe() Mapped USB Registers at:\n");
	printk(KERN_DEBUG "ci13410_probe() reg base = 0x%08X\n",
						(u32)ci13410->reg);	
	printk(KERN_DEBUG "ci13410_probe() id_reg_ptr = 0x%08X\n",
						(u32)ci13410->id_reg_ptr);
	printk(KERN_DEBUG "ci13410_probe() cap_reg_ptr = 0x%08X\n",
						(u32)ci13410->cap_reg_ptr);	
	printk(KERN_DEBUG "ci13410_probe() op_dev_regs_ptr = 0x%08X\n",
						(u32)ci13410->op_dev_regs_ptr);	
#endif

#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	// Map CPU registers
	ci13410->cpuReg = ioremap(CI13410_GCR_BASE, CI13410_USB_REG_SIZE);
	if (ci13410->cpuReg == NULL) {
		ret = -ENOMEM;
		printk(KERN_DEBUG "ci13410_probe() cpu ioremap error.\n");
		goto clean_up;
	}
	
	ci13410->gcr_reg_ptr  = (ci13410_cpu_regs __iomem *)
						((u32)ci13410->cpuReg + 0x000C);
	ci13410->clk_reg_ptr  = (ci13410_cpu_regs __iomem *)
						((u32)ci13410->cpuReg + 0x0200);

#if 0						
	printk(KERN_DEBUG "ci13410_probe() Mapped CPU Registers at:\n");
	printk(KERN_DEBUG "ci13410_probe() CPU reg base = 0x%08X\n",
						(u32)ci13410->cpuReg);		
	printk(KERN_DEBUG "ci13410_probe() gcr_reg_ptr = 0x%08X\n",
						(u32)ci13410->gcr_reg_ptr);
	printk(KERN_DEBUG "ci13410_probe() clk_reg_ptr = 0x%08X\n",
						(u32)ci13410->clk_reg_ptr);
#endif							
#endif

#if defined (CONFIG_MIPS_VSC452)
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
	irqFlags = SA_SHIRQ;
#else
	irqFlags = IRQF_SHARED;
#endif
#endif

	if ((ret = request_irq(irq, ci13410_irq, irqFlags,
					DRIVER_NAME, ci13410)) < 0) {
		printk(KERN_DEBUG "ci13410_probe() request_irq error (%d)\n",
									ret) ;
		goto clean_up;
	}
	
	//printk(KERN_DEBUG "ci13410_probe() using irq = %d\n", irq);
	
	INIT_LIST_HEAD(&ci13410->gadget.ep_list);
	ci13410->gadget.ep0 = &ci13410->ep[0].ep;
	INIT_LIST_HEAD(&ci13410->gadget.ep0->ep_list);
	
	/* Get the maximum number of endpoints
	** supported by this USB controller
	*/
	ci13410->max_endpoints =
		(ioread32(&ci13410->cap_reg_ptr->registers.cap_regs.dcc_params)
		& VUSB20_MAX_ENDPTS_SUPPORTED);

	for (i = 0; i < (ci13410->max_endpoints * 2) - 1; i++) {
		struct ci13410_ep *ep = &ci13410->ep[i];

		if (i != 0) {
			INIT_LIST_HEAD(&ep->ep.ep_list);
			list_add_tail(&ep->ep.ep_list,
					&ci13410->gadget.ep_list);
		}

		INIT_LIST_HEAD(&ep->queue);
		ep->ci13410            = ci13410;
		ep->ep.name            = ci13410_ep_name[i];
		ep->ep.ops             = &ci13410_ep_ops;

		if (i == 0)
			ep->epNum      = i;
		else {
			ep->epNum      = i / 2 + i % 2;
			ep->direction  = (i % 2)?USB_RECV:USB_SEND;
		}
		
		ep->index              = i;
		ep->halted             = FALSE;
				
		ep->ep.maxpacket       = 512;
	}

	ci13410->ep[0].ep.maxpacket = 64;
	ci13410->ep[0].direction    = USB_SEND;
	
	the_controller = ci13410;
	
	_usb_device_init(ci13410);

	ci13410->ep0_req =
		ci13410_alloc_request(&ci13410->ep[0].ep, GFP_KERNEL);
	if (ci13410->ep0_req == NULL) {
		ret = -ENOMEM;
		goto clean_up;
	}

	ci13410->ep0_buf =
		ci13410_alloc_buffer(&ci13410->ep[0].ep, 2,
			&ci13410->ep0_req->dma, GFP_KERNEL);
	if (ci13410->ep0_buf == NULL) {
		ret = -ENOMEM;
		goto clean_up;
	}

	req = container_of(ci13410->ep0_req, struct ci13410_request, req);
	req->internal = TRUE;
	
#if defined (CONFIG_USB_GADGET_DUAL_CONTROLLERS)
	// Register with the composite layer
	ret = usb_gadget_controller_register(_usb_gadget_register_driver,
					_usb_gadget_unregister_driver);
	
	if (ret < 0)
		goto clean_up;
#endif	
	
	// Register /proc entry
	create_proc_read_entry(CI13410_PROC_NAME, 0, NULL, ci13410_proc, NULL);
	
	return 0;
	
clean_up:
	if (ci13410) {
		if (ci13410->ep0_req) {
			if (ci13410->ep0_buf)
				ci13410_free_buffer(&ci13410->ep[0].ep,
							ci13410->ep0_buf,
							ci13410->ep0_req->dma,
							2);			
			
			ci13410_free_request(&ci13410->ep[0].ep,
						ci13410->ep0_req);
		}

		usb_memfree(ci13410);
	}
	if (reg)
		iounmap(reg);

	return ret;
}

static int __init_or_module ci13410_remove(struct device *dev)
{
	struct ci13410                  *ci13410 = dev_get_drvdata(dev);
	struct platform_device          *pdev;

	// Free IRQ	
	pdev = container_of(dev, struct platform_device, dev);
	free_irq(platform_get_irq(pdev, 0), ci13410);
	
	/* shutdown the chip */
	device_reset(ci13410);
	
	iounmap(ci13410->reg);

#if !defined(CI13410_HIGH_SPEED_DRIVER) && defined(CONFIG_ARCH_WPCM450)
	iounmap(ci13410->cpuReg);
#endif
	
	/* free device memory */
	device_free_mem(ci13410);
	
	// Remove entry from /proc filesystem
	remove_proc_entry(CI13410_PROC_NAME, NULL);	
	
	return 0;
}
/*****************************************************************************/

/**********Gadget(Peripheral Controller Driver) init/cleanup******************/
static void ci13410_release_platform(struct device *dev)
{
	/* This function is required. It is because
	   "device_release()"(driver/base/core.c) uses it. */
}

static struct resource ci13410_usb_gadget_resources[] = {
	[0] = {
		.start  = CI13410_USB_REG_PBASE,
		.end    = CI13410_USB_REG_PBASE + CI13410_USB_REG_SIZE,
		.flags  = IORESOURCE_MEM,
	},
	[1] = {
		.start  = CI13410_USB_IRQ,		/* irq number */
		.end    = CI13410_USB_IRQ,
		.flags  = IORESOURCE_IRQ,
	},
};

static struct platform_device ci13410_usb_gadget_device = {
	.name           = (char *) DRIVER_NAME,
	.id             = 0,
	.dev = {
		.dma_mask          = NULL,		/* not use dma */
		.coherent_dma_mask = 0xffffffff,
		.release           = ci13410_release_platform,
	},
	.num_resources = ARRAY_SIZE(ci13410_usb_gadget_resources),
	.resource      = ci13410_usb_gadget_resources,
};

static struct device_driver ci13410_driver = {
	.name   =	(char *) DRIVER_NAME,
	.bus    = &platform_bus_type,
	
	.probe  = ci13410_probe,
	.remove = ci13410_remove,
};

static int __init ci13410_init (void)
{
	printk(KERN_ALERT "ci13410_init() Desc: %s\n", DRIVER_DESC);
	printk(KERN_ALERT "ci13410_init() Name: %s\n", DRIVER_NAME);
	printk(KERN_ALERT "ci13410_init() Version: %s\n", DRIVER_VERSION);
	platform_device_register(&ci13410_usb_gadget_device);
	return driver_register(&ci13410_driver);
}
module_init (ci13410_init);

static void __exit ci13410_cleanup (void)
{
	//printk(KERN_ALERT "ci13410_cleanup()\n");
#if defined (CONFIG_USB_GADGET_DUAL_CONTROLLERS)
	usb_gadget_controller_unregister(&the_controller->gadget);
#endif	
	platform_device_unregister(&ci13410_usb_gadget_device);
	driver_unregister(&ci13410_driver);
}
module_exit (ci13410_cleanup);

MODULE_DESCRIPTION (DRIVER_DESC);
MODULE_AUTHOR ("Tony Scragg");
MODULE_LICENSE ("GPL");
