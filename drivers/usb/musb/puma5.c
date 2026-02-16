/*
 * Copyright (C) 2005-2006 by Texas Instruments
 *
 * This file is part of the Inventra Controller Driver for Linux.
 *
 * The Inventra Controller Driver for Linux is free software; you
 * can redistribute it and/or modify it under the terms of the GNU
 * General Public License version 2 as published by the Free Software
 * Foundation.
 *
 * The Inventra Controller Driver for Linux is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not,
 * write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA  02111-1307  USA
 *
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/usb/usb_phy_gen_xceiv.h>

#include <asm/system_info.h>
#include <asm/arch/hardware.h>
#include <asm/arch/memory.h>
#include <asm/mach-types.h>

#include "musb_core.h"
#include <asm-arm/arch-avalanche/puma5/puma5.h>
#define AVALANCHE_USBPHY_RESET_BIT              26

#include "puma5.h"
#include "cppi41_dma.h"
struct puma5_glue {
	struct device		*dev;
	struct platform_device	*musb;
};

//#define USBDRV_DEBUG
#ifdef USBDRV_DEBUG
#define dprintk(x,...) printk(x, ## __VA_ARGS__)
#else
#define dprintk(x,...)
#endif

static struct musb_hdrc_eps_bits musb_eps[] = {
       {       "ep1_tx", 10,   },
       {       "ep1_rx", 10,   },
       {       "ep2_tx", 9,    },
       {       "ep2_rx", 9,    },
       {       "ep3_tx", 3,    },
       {       "ep3_rx", 3,    },
       {       "ep4_tx", 3,    },
       {       "ep4_rx", 3,    },
       {       "ep5_tx", 3,    },
       {       "ep5_rx", 3,    },
       {       "ep6_tx", 3,    },
       {       "ep6_rx", 3,    },
       {       "ep7_tx", 3,    },
       {       "ep7_rx", 3,    },
       {       "ep8_tx", 2,    },
       {       "ep8_rx", 2,    },
       {       "ep9_tx", 2,    },
       {       "ep9_rx", 2,    },
       {       "ep10_tx", 2,   },
       {       "ep10_rx", 2,   },
       {       "ep11_tx", 2,   },
       {       "ep11_rx", 2,   },
       {       "ep12_tx", 2,   },
       {       "ep12_rx", 2,   },
       {       "ep13_tx", 2,   },
       {       "ep13_rx", 2,   },
       {       "ep14_tx", 2,   },
       {       "ep14_rx", 2,   },
       {       "ep15_tx", 2,   },
       {       "ep15_rx", 2,   },
};

static struct musb_hdrc_config musb_config = {
       .multipoint     = 1,
       .dyn_fifo       = 1,
       .soft_con       = 1,
       .dma            = 1,
       .num_eps        = 5,
       .dma_channels   = 7,
       .dma_req_chan   = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3),
       .ram_bits       = 10,
       .eps_bits       = musb_eps,
};
static struct musb_hdrc_platform_data usb_data = {
#if     defined(CONFIG_USB_MUSB_DUAL_ROLE)
        /* OTG requires a Mini-AB connector */
        .mode           = MUSB_OTG,
#elif   defined(CONFIG_USB_MUSB_GADGET)
        .mode           = MUSB_PERIPHERAL,
#elif   defined(CONFIG_USB_MUSB_HOST)
        .mode           = MUSB_HOST,
#endif
        /* irlml6401 switches 5V */
        .power          = 250,          /* sustains 3.0+ Amps (!) */
        .potpgt         = 4,            /* ~8 msec */
        .config         = &musb_config,

};

/* TODO IRQ and USBotg base address to be changed -
   refer 2.6.18.include/asm/arch-avalanche/puma5 */
static struct resource usb_resources [] = { {
        /* physical address */
        .start          = AVALANCHE_USB20_OTG_SLAVE_BASE,
        .end            = AVALANCHE_USB20_OTG_SLAVE_BASE + 0x5ff,
        .flags          = IORESOURCE_MEM,
}, {
        .start          = LNXINTNUM(AVALANCHE_SRTR_USB_INT),
        .flags          = IORESOURCE_IRQ,
        .name           = "mc",	
}, {
        .flags          = IORESOURCE_IRQ,
        .name           = "dma",
}
};

static u64 usb_dmamask = DMA_BIT_MASK(32);

struct platform_device usb_device = {
        .name           = "musb-puma5",
        .id             = -1,
        .dev = {
                .platform_data          = &usb_data,
                .dma_mask               = &usb_dmamask,
                .coherent_dma_mask      = DMA_BIT_MASK(32),
        },
        .resource       = usb_resources,
        .num_resources  = ARRAY_SIZE(usb_resources),
};

static inline void phy_on(void)
{
      /* start the on-chip PHY and its PLL */
        /* Enable entire phy including the  charg pump reset D1 bit */
        /* start the on-chip PHY and its PLL */

        void    *__iomem tbase = (void *__iomem)AVALANCHE_DCL_BASE;
        u32 tmp;

        /* unlock writes to bootcfg area */
        musb_writel(tbase,BOOTCFG_REG_KICK0_OFFS,BOOTCFG_KICK0_UNLOCK_PATTERN);
        musb_writel(tbase,BOOTCFG_REG_KICK1_OFFS,BOOTCFG_KICK1_UNLOCK_PATTERN);

        /* clear PDWN and OTGPDWN bits in USBPHY */
        tmp = musb_readl(tbase, USBPHY_CTRL_OFFS);
        tmp &= ~(USBPHY_CTRL_PDWN | USBPHY_CTRL_OTGPDWN | USBPHY_CTRL_PLLON);

#define PHYREFCLK_FROM_ONCHIP_PLL
#ifdef PHYREFCLK_FROM_ONCHIP_PLL
        tmp &= ~(USBPHY_CTRL_OSCSEL);
#endif
#ifdef PHYREFCLK_FROM_ONBOARD_PLL
        tmp |= USBPHY_CTRL_OSCSEL;
#endif
        /* enable session end comparator */
        tmp |= USBPHY_CTRL_SESNDEN;

        /* enable vbus comparator enable */
        tmp |= USBPHY_CTRL_VBDTCTEN;
        musb_writel(tbase,USBPHY_CTRL_OFFS,tmp);

        mdelay(100);
}

static inline void phy_off(void)
{
         void    *__iomem tbase = (void *__iomem)AVALANCHE_DCL_BASE;
        u32 tmp;

        /* unlock writes to bootcfg area */
        musb_writel(tbase,BOOTCFG_REG_KICK0_OFFS,BOOTCFG_KICK0_UNLOCK_PATTERN);
        musb_writel(tbase,BOOTCFG_REG_KICK1_OFFS,BOOTCFG_KICK1_UNLOCK_PATTERN);

        /* powerdown the on-chip PHY and its oscillator */
        tmp = musb_readl(tbase, USBPHY_CTRL_OFFS);
        tmp |= (USBPHY_CTRL_PDWN | USBPHY_CTRL_OTGPDWN );
        musb_writel(tbase,USBPHY_CTRL_OFFS,tmp);
        mdelay(100);
}

static int dma_off = 1;

void puma5_musb_platform_enable(struct musb *musb)
{
	u32	tmp, old, val;

  	/* workaround:  setup irqs through both register sets */
        tmp = (musb->epmask & PUMA5_USB_TX_ENDPTS_MASK)
                        << PUMA5_USB_TXINT_SHIFT;
        musb_writel(musb->ctrl_base, PUMA5_USB_INT_MASK_SET_REG, tmp);
        old = tmp;
        tmp = (musb->epmask & (0xfffe & PUMA5_USB_RX_ENDPTS_MASK))
                        << PUMA5_USB_RXINT_SHIFT;
        musb_writel(musb->ctrl_base, PUMA5_USB_INT_MASK_SET_REG, tmp);
        tmp |= old;


		val = ~0x0;
	tmp |= ((val & 0x01ff) << PUMA5_USB_USBINT_SHIFT);
	musb_writel(musb->ctrl_base, PUMA5_USB_INT_MASK_SET_REG, tmp);

	if (is_dma_capable() && !dma_off)
		printk(KERN_WARNING "%s %s: dma not reactivated\n",
				__FILE__, __func__);
	else
		dma_off = 0;

	/* force a DRVVBUS irq so we can start polling for ID change */
	musb_writel(musb->ctrl_base, PUMA5_USB_INT_SET_REG,
    PUMA5_INTR_DRVVBUS << PUMA5_USB_USBINT_SHIFT);
}

/*
 * Disable the HDRC and flush interrupts
 */
void puma5_musb_platform_disable(struct musb *musb)
{
	/* because we don't set CTRLR.UINT, "important" to:
	 *  - not read/write INTRUSB/INTRUSBE
	 *  - (except during initial setup, as workaround)
	 *  - use INTSETR/INTCLRR instead
	 */
	musb_writel(musb->ctrl_base, PUMA5_USB_INT_MASK_CLR_REG,
                          PUMA5_USB_USBINT_MASK
                        | PUMA5_USB_TXINT_MASK
                        | PUMA5_USB_RXINT_MASK);
        musb_writeb(musb->mregs, MUSB_DEVCTL, 0);
        musb_writel(musb->ctrl_base, PUMA5_USB_EOI_REG, 0);

	dma_off = 1;

	if (is_dma_capable() && !dma_off)
		WARNING("dma still active\n");
}


/* REVISIT it's not clear whether DaVinci can support full OTG.  */

static int vbus_state = -1;

#define	portstate(stmt)		stmt


/* VBUS SWITCHING IS BOARD-SPECIFIC */

static void evm_deferred_drvvbus(struct work_struct *_musb)
{
	//set puma5 vbus
}
DECLARE_WORK(evm_vbus_work, evm_deferred_drvvbus);

static void puma5_source_power(struct musb *musb, int is_on, int immediate)
{
	if (is_on)
		is_on = 1;

	if (vbus_state == is_on)
		return;
	vbus_state = is_on;		/* 0/1 vs "-1 == unknown/init" */

	/* this configuration is only of TNETC950N, where min-AB connect is provided
	   to test for host mode functionality */
        if( is_on ){
                PAL_sysGpioCtrl(14, GPIO_PIN, GPIO_OUTPUT_PIN);
                PAL_sysGpioOutBit(14,0);
                printk("VBUS on(%d)-GPIO14(LOW)\n",is_on);
        }else{
                PAL_sysGpioCtrl(14, GPIO_PIN, GPIO_OUTPUT_PIN);
                PAL_sysGpioOutBit(14,1);
                printk("VBUS off(%d)-GPIO14(HIGH)\n",is_on);
        }

        if (immediate) {
                vbus_state = is_on;
		//session(musb, is_on);
	}
//	schedule_work(&evm_vbus_work);
}

static void puma5_set_vbus(struct musb *musb, int is_on)
{
	WARN_ON(is_on && is_peripheral_active(musb));
	puma5_source_power(musb, is_on, 0);
}


#define	POLL_SECONDS	2

static struct timer_list otg_workaround;

static void otg_timer(unsigned long _musb)
{
	struct musb		*musb = (void *)_musb;
	void __iomem		*mregs = musb->mregs;
	u8			devctl;
	unsigned long		flags;

	/* We poll because DaVinci's won't expose several OTG-critical
	* status change events (from the transceiver) otherwise.
	 */
	devctl = musb_readb(mregs, MUSB_DEVCTL);
	dev_dbg(musb->controller, "poll devctl %02x (%s)\n", devctl, usb_otg_state_string(musb->xceiv->state));

	spin_lock_irqsave(&musb->lock, flags);
	switch (musb->xceiv->state) {
	case OTG_STATE_A_WAIT_VFALL:
		/* Wait till VBUS falls below SessionEnd (~0.2V); the 1.3 RTL
		 * seems to mis-handle session "start" otherwise (or in our
		 * case "recover"), in routine "VBUS was valid by the time
		 * VBUSERR got reported during enumeration" cases.
		 */
		if (devctl & MUSB_DEVCTL_VBUS) {
			mod_timer(&otg_workaround, jiffies + POLL_SECONDS * HZ);
			break;
		}
		musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
		musb_writel(musb->ctrl_base, PUMA5_USB_INT_SET_REG,
			MUSB_INTR_VBUSERROR << PUMA5_USB_USBINT_SHIFT);
		break;
	case OTG_STATE_B_IDLE:
		/* There's no ID-changed IRQ, so we have no good way to tell
		 * when to switch to the A-Default state machine (by setting
		 * the DEVCTL.SESSION flag).
		 *
		 * Workaround:  whenever we're in B_IDLE, try setting the
		 * session flag every few seconds.  If it works, ID was
		 * grounded and we're now in the A-Default state machine.
		 *
		 * NOTE setting the session flag is _supposed_ to trigger
		 * SRP, but clearly it doesn't.
		 */
		musb_writeb(mregs, MUSB_DEVCTL,
				devctl | MUSB_DEVCTL_SESSION);
		devctl = musb_readb(mregs, MUSB_DEVCTL);
		if (devctl & MUSB_DEVCTL_BDEVICE)
			mod_timer(&otg_workaround, jiffies + POLL_SECONDS * HZ);
		else
			musb->xceiv->state = OTG_STATE_A_IDLE;
		break;
	default:
		break;
	}
	spin_unlock_irqrestore(&musb->lock, flags);
}
#ifdef MUSB_USE_TASKLET
        static struct tasklet_struct musb_tasklet;
        static int musb_tasklet_scheduled = 0;
        static int taskInit = 0;
        void musb_tasklet_func(unsigned long);
#endif

DEFINE_SPINLOCK(musb_irq_lock);
#ifndef MUSB_USE_TASKLET
static irqreturn_t puma5_direct_interrupt(int irq, void *__hci);
#endif

static irqreturn_t puma5usb_interrupt(int irq, void *__hci)
{
        irqreturn_t ret;
#ifdef MUSB_USE_TASKLET
        unsigned long p;
        unsigned long flags;
        struct musb     *musb = __hci;
#endif

#ifndef MUSB_USE_TASKLET
        ret= puma5_direct_interrupt(irq,__hci);
#else
        /* use tasklet for processing here */
        spin_lock_irqsave(&musb_irq_lock, flags);
        if(musb_tasklet_scheduled)
        {
                ret = IRQ_HANDLED;
        }
        else
        {
                disable_irq(24); /* TODO */
                /* trigger tasklet action */
                p = (unsigned long) __hci;
                musb->int_regs = r;
                if(!taskInit)
                {
                        tasklet_init(&musb_tasklet,musb_tasklet_func,p);
                        taskInit = 1;
                }
                tasklet_schedule(&musb_tasklet);
                musb_tasklet_scheduled = 1;
                ret = IRQ_HANDLED;
        }/* end of else conditional block */

        spin_unlock_irqrestore(&musb_irq_lock, flags);
#endif

        return ret;
}

#ifdef MUSB_USE_TASKLET
void musb_tasklet_func(unsigned long param)
{
        void * __hci = (void *)param;
        unsigned long   flags;
        struct musb     *musb = __hci;
        void            *__iomem tibase = musb->ctrl_base;
        u32             tmp,retval;

        spin_lock_irqsave(&musb->lock, flags);

        /* NOTE: DaVinci shadows the Mentor IRQs; don't manage them through
         * the mentor registers (except for setup), use the TI ones and EOI.
         */

        /* ack and handle non-CPPI interrupts */
        tmp = musb_readl(tibase, PUMA5_USB_INT_SRC_MASKED_REG);
        musb_writel(tibase, PUMA5_USB_INT_SRC_CLR_REG, tmp);
        musb->int_rx = (tmp & PUMA5_USB_RXINT_MASK)
                        >> PUMA5_USB_RXINT_SHIFT;
        musb->int_tx = (tmp & PUMA5_USB_TXINT_MASK)
                        >> PUMA5_USB_TXINT_SHIFT;
        musb->int_usb = (tmp & PUMA5_USB_USBINT_MASK)
                        >> PUMA5_USB_USBINT_SHIFT;

        if (tmp & (1 << (8 + PUMA5_USB_USBINT_SHIFT))){

              int     drvvbus = musb_readl(tibase, PUMA5_USB_STAT_REG);
                /* NOTE:  this must complete poweron within 100 msec */

                if (drvvbus) {
                        MUSB_HST_MODE(musb);
                        musb->xceiv->default_a = 1;
                        musb->xceiv->state = OTG_STATE_A_IDLE;
                } else {
                        MUSB_DEV_MODE(musb);
                        musb->xceiv->default_a = 0;
                        musb->xceiv->state = OTG_STATE_B_IDLE;
                }
                /* NOTE:  this must complete poweron within 100 msec */
                puma5_source_power(musb, drvvbus, 0);
                dprintk(" drrvbus status = %d,INTUSB=%x\n",drvvbus,musb->int_usb);
                retval = IRQ_HANDLED;
        }

        if ((musb->int_tx ) || musb->int_usb || musb->int_rx )
                musb_interrupt(musb);

        tmp = musb_readl(tibase, PUMA5_USB_INT_SRC_MASKED_REG);

        if(tmp)
                tasklet_schedule(&musb_tasklet);
        else
        {
                musb_tasklet_scheduled = 0;
                enable_irq(24); /* TODO */
                /* Endpoint/USB Status irq stays asserted until EOI is written */
                musb_writel(tibase, PUMA5_USB_EOI_REG, 0);
              }
        spin_unlock_irqrestore(&musb->lock, flags);
}
#endif
#ifndef MUSB_USE_TASKLET
static irqreturn_t puma5_direct_interrupt(int irq, void *__hci)
{
	unsigned long	flags;
	irqreturn_t	retval = IRQ_NONE;
	struct musb	*musb = __hci;
  struct usb_otg  *otg = musb->xceiv->otg;
	void __iomem	*tibase = musb->ctrl_base;
	u32		tmp;

	spin_lock_irqsave(&musb->lock, flags);

	/* NOTE: DaVinci shadows the Mentor IRQs.  Don't manage them through
	 * the Mentor registers (except for setup), use the TI ones and EOI.
	 *
	 * Docs describe irq "vector" registers associated with the CPPI and
	 * USB EOI registers.  These hold a bitmask corresponding to the
	 * current IRQ, not an irq handler address.  Would using those bits
	 * resolve some of the races observed in this dispatch code??
	 */

	/* CPPI interrupts share the same IRQ line, but have their own
	 * mask, state, "vector", and EOI registers.
	 */
	tmp = musb_readl(tibase, PUMA5_USB_INT_SRC_MASKED_REG);
	musb_writel(tibase, PUMA5_USB_INT_SRC_CLR_REG, tmp);
	dev_dbg(musb->controller, "IRQ %08x\n", tmp);

	musb->int_rx = (tmp & PUMA5_USB_RXINT_MASK)
			>> PUMA5_USB_RXINT_SHIFT;
	musb->int_tx = (tmp & PUMA5_USB_TXINT_MASK)
			>> PUMA5_USB_TXINT_SHIFT;
	musb->int_usb = (tmp & PUMA5_USB_USBINT_MASK)
			>> PUMA5_USB_USBINT_SHIFT;

	/* DRVVBUS irqs are the only proxy we have (a very poor one!) for
	 * DaVinci's missing ID change IRQ.  We need an ID change IRQ to
	 * switch appropriately between halves of the OTG state machine.
	 * Managing DEVCTL.SESSION per Mentor docs requires we know its
	 * value, but DEVCTL.BDEVICE is invalid without DEVCTL.SESSION set.
	 * Also, DRVVBUS pulses for SRP (but not at 5V) ...
	 */
	if (tmp & (PUMA5_INTR_DRVVBUS << PUMA5_USB_USBINT_SHIFT)) {
		int	drvvbus = musb_readl(tibase, PUMA5_USB_STAT_REG);
		void __iomem *mregs = musb->mregs;
		u8	devctl = musb_readb(mregs, MUSB_DEVCTL);
		int	err = musb->int_usb & MUSB_INTR_VBUSERROR;

        err = musb->int_usb & (MUSB_INTR_VBUSERROR|MUSB_INTR_BABBLE);
		if (err) {
			/* The Mentor core doesn't debounce VBUS as needed
			 * to cope with device connect current spikes. This
			 * means it's not uncommon for bus-powered devices
			 * to get VBUS errors during enumeration.
			 *
			 * This is a workaround, but newer RTL from Mentor
			 * seems to allow a better one: "re"starting sessions
			 * without waiting (on EVM, a **long** time) for VBUS
			 * to stop registering in devctl.
			 */
			musb->int_usb &= ~MUSB_INTR_VBUSERROR;
			musb->xceiv->state = OTG_STATE_A_WAIT_VFALL;
			mod_timer(&otg_workaround, jiffies + 1 * HZ);
			WARNING("VBUS error workaround (delay coming)\n");
		} else if (drvvbus) {
			musb->is_active = 1;
			MUSB_HST_MODE(musb);
			otg->default_a = 1;
			musb->xceiv->state = OTG_STATE_A_WAIT_VRISE;
			portstate(musb->port1_status |= USB_PORT_STAT_POWER);
			del_timer(&otg_workaround);
		} else {
			musb->is_active = 0;
			MUSB_DEV_MODE(musb);	
			otg->default_a = 0;
			musb->xceiv->state = OTG_STATE_B_IDLE;
			portstate(musb->port1_status &= ~USB_PORT_STAT_POWER);
		}

		/* NOTE:  this must complete poweron within 100 msec */
		puma5_source_power(musb, drvvbus, 0);
		dev_dbg(musb->controller, "VBUS %s (%s)%s, devctl %02x\n",
				drvvbus ? "on" : "off",
				usb_otg_state_string(musb->xceiv->state),
				err ? " ERROR" : "",
				devctl);
		retval = IRQ_HANDLED;
	}

	if (musb->int_tx || musb->int_rx || musb->int_usb)
		retval |= musb_interrupt(musb);

	/* irq stays asserted until EOI is written */
	musb_writel(tibase, PUMA5_USB_EOI_REG, 0);


	/* poll for ID change */
	if (musb->xceiv->state == OTG_STATE_B_IDLE)
		mod_timer(&otg_workaround, jiffies + POLL_SECONDS * HZ);

	spin_unlock_irqrestore(&musb->lock, flags);

	/* REVISIT we sometimes get unhandled IRQs
	 * (e.g. ep0).  not clear why...
	 */
	if (retval != IRQ_HANDLED)
		dev_dbg(musb->controller, "unhandled? %08x\n", tmp);
	return IRQ_HANDLED;
}
#endif
int puma5_musb_set_mode(struct musb *musb, u8 musb_mode)
{
	WARNING("FIXME: %p not implemented\n", __func__);
    return 0;
}

int puma5_musb_platform_init(struct musb *musb)
{
	void __iomem	*tibase = musb->ctrl_base;
	u32		revision;
	int 		ret = -ENODEV;

	usb_nop_xceiv_register();
	musb->xceiv = usb_get_phy(USB_PHY_TYPE_USB2);
	if (IS_ERR_OR_NULL(musb->xceiv)) {
		ret = -EPROBE_DEFER;
		goto unregister;
	}
		
	musb->mregs += PUMA5_BASE_OFFSET;
#if 0
	musb->clock = clk_get(NULL, "USBCLK");
	if (IS_ERR(musb->clock))
		return PTR_ERR(musb->clock);

	status = clk_enable(musb->clock);
	if (status < 0)
		return -ENODEV;
#endif
	/* returns zero if e.g. not clocked */
	revision = musb_readl(tibase, PUMA5_USB_VERSION_REG);
	if (revision == 0)
		return -ENODEV;

	setup_timer(&otg_workaround, otg_timer, (unsigned long) musb);

	puma5_source_power(musb, 0, 1);

	/* reset the controller */
	musb_writel(tibase, PUMA5_USB_CTRL_REG, 0x1);

	/* start the on-chip PHY and its PLL */
	phy_on();

	msleep(5);
#if 0
	/* NOTE:  irqs are in mixed mode, not bypass to pure-musb */
	pr_debug("DaVinci OTG revision %08x phy %03x control %02x\n",
		revision, __raw_readl((void __force __iomem *)
				IO_ADDRESS(USBPHY_CTL_PADDR)),
		musb_readb(tibase, PUMA5_USB_CTRL_REG));
#endif
	musb->isr = puma5usb_interrupt;
	return 0;

fail:
	usb_put_phy(musb->xceiv);
unregister:
	usb_nop_xceiv_unregister();
	return ret;
}

int puma5_musb_platform_exit(struct musb *musb)
{
  del_timer_sync(&otg_workaround);

	puma5_source_power(musb, 0 /*off*/, 1);

	/* delay, to avoid problems with module reload */
	if (musb->xceiv->otg->default_a) {
		int	maxdelay = 30;
		u8	devctl, warn = 0;

		/* if there's no peripheral connected, this can take a
		 * long time to fall, especially on EVM with huge C133.
		 */
		do {
			devctl = musb_readb(musb->mregs, MUSB_DEVCTL);
			if (!(devctl & MUSB_DEVCTL_VBUS))
				break;
			if ((devctl & MUSB_DEVCTL_VBUS) != warn) {
				warn = devctl & MUSB_DEVCTL_VBUS;
				dev_dbg(musb->controller, "VBUS %d\n",
					warn >> MUSB_DEVCTL_VBUS_SHIFT);
			}
			msleep(1000);
			maxdelay--;
		} while (maxdelay > 0);

		/* in OTG mode, another host might be connected */
		if (devctl & MUSB_DEVCTL_VBUS)
			dev_dbg(musb->controller, "VBUS off timeout (devctl %02x)\n", devctl);
	}

	phy_off();

	usb_put_phy(musb->xceiv);
	usb_nop_xceiv_unregister();

	return 0;
}

#ifdef CONFIG_PM
#define USB_PLL_REG_OFFSET              0x8
#define USB_PLL_DISABLED                (1 << 0)
#define USB_PLL_POWERDOWN               (1 << 1)
#define USB_PLL_BYPASS_N                (1 << 2)
#define USB_PLL_RESET_N                 (1 << 3)

void musb_pullup(struct musb *musb, int is_on)
{
        u8 power;

        power = musb_readb(musb->mregs, MUSB_DEVCTL);
        if (is_on)
                power |= MUSB_POWER_SOFTCONN;
        else
                power &= ~MUSB_POWER_SOFTCONN;


        musb_writeb(musb->mregs, MUSB_DEVCTL, power);
}

void puma5_usb_power_up(struct musb *musb)
{
        u32 *usb_pll ;
        u32 val;

#if defined(CONFIG_PUMA5_USBPHY_BYPASS_POR)
        /* bypass the usb phy POR - only for PG2 devices */
        if (system_rev == AVALANCHE_PG_REV2){
            *(u32 *)USB_PHY_POR_ADDR = 0x80;
            mdelay(10);
        }
#endif
         /* set USB clock speed to 24 Mhz */
        if(PAL_sysClkcSetFreq(PAL_SYS_CLKC_USB, AVALANCHE_USB_CLOCK_SPEED) < 0)
                dev_dbg(musb->controller,"%s:USB clock set failed\n", __FUNCTION__);

        /* power up the USB_PLL */
        usb_pll = (u32 *)(AVALANCHE_USB_PLL_BASE + USB_PLL_REG_OFFSET);
        val = *usb_pll;
        val |= (USB_PLL_RESET_N | USB_PLL_BYPASS_N);
        val &= ~(USB_PLL_POWERDOWN |USB_PLL_DISABLED);
        *usb_pll = val;
        mdelay(10);

        /* enable the usb clock */
        PAL_sysPowerCtrl((INT32)PSC_SR_CLK5, POWER_CTRL_POWER_UP);
        PAL_sysPowerCtrl((INT32)PSC_USB_PHY, POWER_CTRL_POWER_UP);
        mdelay(10);

        phy_on();
        //musb_pullup(musb, 1);

}
EXPORT_SYMBOL(puma5_usb_power_up);
void puma5_usb_power_down(struct musb *musb)
{

        u32 *usb_pll;
        u32 val;

        //musb_pullup(musb, 0);

#ifdef CONFIG_USB_MUSB_GADGET
#if 0
        if (musb->pGadgetDriver && musb->pGadgetDriver->disconnect) {
                spin_unlock(&musb->lock);
                musb->pGadgetDriver->disconnect(&musb->g);
                spin_lock(&musb->lock);
        }
#endif
#endif

         /* power down usb phy */
        phy_off();
        udelay(1000);

        //* disable the usb pll clock */
        PAL_sysPowerCtrl((INT32)PSC_SR_CLK5, POWER_CTRL_POWER_DOWN);
        PAL_sysPowerCtrl((INT32)PSC_USB_PHY, POWER_CTRL_POWER_DOWN);

        mdelay(10);

        /* power down the USB_PLL */
        usb_pll = (u32 *)(AVALANCHE_USB_PLL_BASE + USB_PLL_REG_OFFSET);
        val = *usb_pll;
        val |= (USB_PLL_POWERDOWN |USB_PLL_DISABLED);
        val &= ~(USB_PLL_RESET_N | USB_PLL_BYPASS_N);
        *usb_pll = val;
}
EXPORT_SYMBOL(puma5_usb_power_down);
#endif


static const struct musb_platform_ops puma5_ops = {
	.init		= puma5_musb_platform_init,
	.exit		= puma5_musb_platform_exit,

	.enable		= puma5_musb_platform_enable,
	.disable	= puma5_musb_platform_disable,

	.set_mode	= puma5_musb_set_mode,

	.set_vbus	= puma5_set_vbus,
};

static u64 puma5_dmamask = DMA_BIT_MASK(32);

static int puma5_probe(struct platform_device *pdev)
{
	struct musb_hdrc_platform_data	*pdata = pdev->dev.platform_data;
	struct platform_device		*musb;
	struct puma5_glue		*glue;

	int				ret = -ENOMEM;

	glue = kzalloc(sizeof(*glue), GFP_KERNEL);
	if (!glue) {
		dev_err(&pdev->dev, "failed to allocate glue context\n");
		goto err0;
	}

	musb = platform_device_alloc("musb-hdrc", PLATFORM_DEVID_AUTO);
	if (!musb) {
		dev_err(&pdev->dev, "failed to allocate musb device\n");
		goto err1;
	}

	musb->dev.parent		= &pdev->dev;
	musb->dev.dma_mask		= &puma5_dmamask;
	musb->dev.coherent_dma_mask	= puma5_dmamask;

	glue->dev			= &pdev->dev;
	glue->musb			= musb;

	pdata->platform_ops		= &puma5_ops;

	platform_set_drvdata(pdev, glue);

	ret = platform_device_add_resources(musb, pdev->resource,
			pdev->num_resources);
	if (ret) {
		dev_err(&pdev->dev, "failed to add resources\n");
		goto err2;
	}

	ret = platform_device_add_data(musb, pdata, sizeof(*pdata));
	if (ret) {
		dev_err(&pdev->dev, "failed to add platform_data\n");
		goto err2;
	}

	ret = platform_device_add(musb);
	if (ret) {
		dev_err(&pdev->dev, "failed to register musb device\n");
		goto err2;
	}

	return 0;


err2:
	platform_device_put(musb);

err1:
	kfree(glue);

err0:
	return ret;
}

static int puma5_remove(struct platform_device *pdev)
{
	struct puma5_glue		*glue = platform_get_drvdata(pdev);

	platform_device_unregister(glue->musb);
	kfree(glue);

	return 0;
}

static struct platform_driver puma5_driver = {
	.probe    = puma5_probe,
	.remove		= puma5_remove,
	.driver		= {
		.name	= "musb-puma5",	
	},
};

static void musb_release (struct device *dev)
{}
            
static int __init puma5_init(void)
{
	printk("puma5 usb init\n");
  	platform_device_register(&usb_device);
  	usb_device.dev.release = musb_release;
  	return platform_driver_probe(&puma5_driver, puma5_probe);
}
subsys_initcall(puma5_init);

static void __exit puma5_exit(void)
{
	platform_driver_unregister(&puma5_driver);
}
module_exit(puma5_exit);

MODULE_DESCRIPTION("Puma5 MUSB Glue Layer");
MODULE_AUTHOR("TODO?????");
MODULE_LICENSE("GPL v2");

/* 
TODO:
   We can use module_platform_driver (like in davinchi) only in case 
   when move platform_device_register from puma5_init to mach-avalance

module_platform_driver(puma5_driver);
*/
