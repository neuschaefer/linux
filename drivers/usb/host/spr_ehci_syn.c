/*
 * linux/drivers/usb/host/spr_ehci_syn.c
 *
 *  Copyright (C)2006  ST MICROELECTRONICS
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
 * AUTHOR :  Ashwini Pahuja <ashwini.pahuja@st.com> */



/* may be called with controller, bus, and devices active */

/**
 * ehci_hcd_syn_drv_remove - shutdown processing for HCDs
 * @dev: USB Host Controller being removed
 * Context: !in_interrupt()
 *
 * Reverses the effect of ehci_hcd_syn_drv_probe(), first invoking
 * the HCD's stop() method.  It is always called from a thread
 * context, normally "rmmod", "apmd", or something similar.
 *
 */

#include <linux/platform_device.h>
#include <asm/arch/irqs.h>
#include <asm/arch/spear.h>


static void usb_release(struct device *dev)
{
    /* normally not freed */
}

static u64 usb_dmamask = ~0;

static struct resource ehci0_spear_resources[] = {
    [0] = {
#ifdef CONFIG_ARCH_SPEARPLUS
        .start = (VA_SPEARPLUS_START_USB_EHCI0),
        .end   = (VA_SPEARPLUS_START_USB_EHCI0 + SPEARPLUS_SIZE_USB_EHCI0 - 1),
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
        .start = (VA_SPEARBASIC_START_USB_EHCI0_1),
        .end   = (VA_SPEARBASIC_START_USB_EHCI0_1 + SPEARBASIC_SIZE_USB_EHCI0_1 - 1),
#endif
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start	= IRQ_USB_H_EHCI_0,
        .end 		= IRQ_USB_H_EHCI_0,
        .flags	= IORESOURCE_IRQ,
    },

};


#ifdef CONFIG_ARCH_SPEARPLUS
static struct resource ehci1_spearplus_resources[] = {
    [0] = {
        .start = (VA_SPEARPLUS_START_USB_EHCI1),
        .end   = (VA_SPEARPLUS_START_USB_EHCI1 + SPEARPLUS_SIZE_USB_EHCI1 - 1),
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start        = IRQ_USB_H_EHCI_1,
        .end          = IRQ_USB_H_EHCI_1,
        .flags        = IORESOURCE_IRQ,
    },
};
#endif

static struct platform_device spear_ehci0 =
{
    .name		= "ehci_hcd",
    .id		= 2,
    .dev = {
        .release		= usb_release,
        .dma_mask		= &usb_dmamask,
        .coherent_dma_mask	= 0xffffffff,
    },
    .num_resources	= ARRAY_SIZE(ehci0_spear_resources),
    .resource	= ehci0_spear_resources,
};

#ifdef CONFIG_ARCH_SPEARPLUS
static struct platform_device spear_ehci1 =
{
    .name           = "ehci_hcd",
    .id             = 3,
    .dev = {
        .release                = usb_release,
        .dma_mask               = &usb_dmamask,
        .coherent_dma_mask      = 0xffffffff,
    },
    .num_resources  = ARRAY_SIZE(ehci1_spearplus_resources),
    .resource       = ehci1_spearplus_resources,
};
#endif


static struct platform_device *spear_usb_devs_ehci[] __initdata = {
    &spear_ehci0
#ifdef CONFIG_ARCH_SPEARPLUS
        ,&spear_ehci1
#endif
};

/*
static int ehci_spear_setup(struct usb_hcd *hcd)
{

    struct ehci_hcd         *ehci = hcd_to_ehci(hcd);
    int  retval;

    printk("Entering ehci_spear_setup \n");
    retval = ehci_halt(ehci);

    if (retval)
    {
        printk("SPEAr EHCI Halt Failed \n");
        return retval;
    }

    retval = ehci_init(hcd);
    if (retval)
    {
        printk("SPEAr EHCI Init Failed \n");
        return retval;
    }

    if (ehci_is_TDI(ehci))
        ehci_reset(ehci);
}
*/

static const struct hc_driver ehci_hc_spear_driver = {
    .description =          hcd_name,
    .product_desc =         "SPEAr EHCI",
    .hcd_priv_size = sizeof(struct ehci_hcd),

    /*
     * generic hardware linkage
     */
    .irq =                  ehci_irq,
    .flags =                HCD_MEMORY | HCD_USB2,

    /*
     * basic lifecycle operations
     */
    .reset =                ehci_init,
    .start =                ehci_run,
    .stop =                 ehci_stop,

    /*
     * memory lifecycle (except per-request)
     */

    /*
     * managing i/o requests and associated device resources
     */
    .urb_enqueue =          ehci_urb_enqueue,
    .urb_dequeue =          ehci_urb_dequeue,
    .endpoint_disable =     ehci_endpoint_disable,

    /*
     * scheduling support
     */
    .get_frame_number =     ehci_get_frame,

    /*
     * root hub support
     */
    .hub_status_data =      ehci_hub_status_data,
    .hub_control =          ehci_hub_control,
};


static int ehci_hcd_spear_drv_remove (struct device *dev)
{
    struct platform_device *pdev = to_platform_device(dev);
    struct usb_hcd *hcd = platform_get_drvdata(pdev);

    if (!hcd)
        return 0;

    dev_info (hcd->self.controller, "remove, state %x\n", hcd->state);

    if (in_interrupt ())
        BUG ();

    usb_remove_hcd(hcd);
    iounmap(hcd->regs);
    release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
    usb_put_hcd(hcd);

    return 0;
}


int usb_ehci_spear_probe(const struct hc_driver *driver,
        struct usb_hcd **hcd_out, struct platform_device *dev)
{
    int retval;
    struct usb_hcd *hcd;
    struct ehci_hcd *ehci;
    volatile unsigned int *tmp;

    if (dev->num_resources != 2) 
    {
        printk(KERN_ERR "hcd probe: invalid num_resources: %i\n",
                dev->num_resources);
        return -ENODEV;
    }

    if (dev->resource[0].flags != IORESOURCE_MEM
            || dev->resource[1].flags != IORESOURCE_IRQ) {
        printk(KERN_ERR "hcd probe: invalid resource type\n");
        return -ENODEV;
    }

    hcd = usb_create_hcd(driver, &dev->dev, "SPEAr EHCI");
    if (!hcd)
        return -ENOMEM;
    hcd->rsrc_start = dev->resource[0].start;
    hcd->rsrc_len = dev->resource[0].end - dev->resource[0].start + 1;

    if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
        pr_debug("request_mem_region failed");
        retval = -EBUSY;
        goto err1;
    }

    hcd->regs = (void*)dev->resource[0].start;

    ehci = hcd_to_ehci(hcd);
    ehci->caps = hcd->regs;
    ehci->regs = hcd->regs + HC_LENGTH(readl(&ehci->caps->hc_capbase));
    /* cache this readonly data; minimize chip reads */
    ehci->hcs_params = readl(&ehci->caps->hcs_params);

    /* ehci_hcd_init(hcd_to_ehci(hcd)); */

    retval = usb_add_hcd(hcd, dev->resource[1].start, SA_INTERRUPT );

    // Wokr around 
    tmp = (unsigned int *)(((unsigned char *)(hcd->regs))+0x194);
    *(unsigned int *)(((unsigned char *)(hcd->regs))+0x194) = 0x00800080;

    tmp = (unsigned int *)(((unsigned char *)(hcd->regs))+0x1A0);
    *(unsigned int *)(((unsigned char *)(hcd->regs))+0x1A0) = 0x1;

    if (retval == 0)
        return retval;

    release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
err1:
    usb_put_hcd(hcd);
    return retval;
}


/* configure so an HC device and id are always provided */
/* always called with process context; sleeping is OK */

/** Allocates basic resources for this USB host controller, and
 * then invokes the start() method for the HCD associated with it
 * through the hotplug entry's driver_data.
 *
 */
static int ehci_hcd_spear_drv_probe (struct device *dev)
{
    struct platform_device *pdev = to_platform_device(dev);
    struct usb_hcd *hcd = NULL;
    int ret;

    printk (KERN_INFO "Enter ehci_hcd_spear_drv_probe\n");

    if (usb_disabled())
        return -ENODEV;

    ret = usb_ehci_spear_probe(&ehci_hc_spear_driver, &hcd, pdev);

    return ret;
}


static struct device_driver ehci_hcd_spear_driver = {
    .name		= (char *) hcd_name,
    .bus		= &platform_bus_type,
    .probe		= ehci_hcd_spear_drv_probe,
    .remove		= ehci_hcd_spear_drv_remove,

};


static int __init spear_ehci_init (void)
{
    int err;
    if (usb_disabled())
        return -ENODEV;

    platform_add_devices(spear_usb_devs_ehci, ARRAY_SIZE(spear_usb_devs_ehci));
    //platform_add_devices(spear_usb_devs_ohci, ARRAY_SIZE(spear_usb_devs_ehci));

    pr_debug ("%s: block sizes: qh %Zd qtd %Zd itd %Zd sitd %Zd\n",
            hcd_name,
            sizeof (struct ehci_qh), sizeof (struct ehci_qtd),
            sizeof (struct ehci_itd), sizeof (struct ehci_sitd));

#ifdef  CONFIG_PCI
    return pci_register_driver (&ehci_pci_driver);
#endif

    printk(KERN_INFO "Registering EHCI SPEAR driver\n");
    err = driver_register(&ehci_hcd_spear_driver);
    return 0;
}

static void __exit spear_ehci_cleanup (void)
{
    //	printk(KERN_INFO "Unloading EHCI SPEARlite driver\n");
    driver_unregister(&ehci_hcd_spear_driver);

}
