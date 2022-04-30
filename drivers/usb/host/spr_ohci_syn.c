/*
 * linux/drivers/usb/host/spr_ohci_syn.c
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
 * Glue Logic for SPEAr OHCI 
 * AUTHOR :  Ashwini Pahuja <ashwini.pahuja@st.com> */

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/mach-types.h>

#include <asm/arch/hardware.h>

#include <asm/arch/irqs.h>
#include <asm/arch/spear.h>
#include <linux/platform_device.h>

static void usb_release(struct device *dev)
{
    /* normally not freed */
}
static u64 usb_dmamask = ~0;

static struct resource ohci0_spear_resources[] = 
{
    [0] = {
#ifdef CONFIG_ARCH_SPEARPLUS
        .start = (VA_SPEARPLUS_START_USB_OHCI0),
        .end   = (VA_SPEARPLUS_START_USB_OHCI0 + SPEARPLUS_SIZE_USB_OHCI0 - 1),
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
        .start = (VA_SPEARBASIC_START_USB_OHCI0),
        .end   = (VA_SPEARBASIC_START_USB_OHCI0 + SPEARBASIC_SIZE_USB_OHCI0 - 1),
#endif
        .flags = IORESOURCE_MEM,
    },

    [1] = {
        .start        = IRQ_USB_H_OHCI_0,
        .end          = IRQ_USB_H_OHCI_0,
        .flags        = IORESOURCE_IRQ,
    },
};


static struct resource ohci1_spear_resources[] = {

    [0] = {
#ifdef CONFIG_ARCH_SPEARPLUS
        .start = (VA_SPEARPLUS_START_USB_OHCI1),
        .end   = (VA_SPEARPLUS_START_USB_OHCI1 + SPEARPLUS_SIZE_USB_OHCI1 - 1),
#endif
#ifdef CONFIG_ARCH_SPEARBASIC
        .start = (VA_SPEARBASIC_START_USB_OHCI1),
        .end   = (VA_SPEARBASIC_START_USB_OHCI1 + SPEARBASIC_SIZE_USB_OHCI1 - 1),
#endif

        .flags = IORESOURCE_MEM,
    },

    [1] = {
        .start        = IRQ_USB_H_OHCI_1,
        .end      	= IRQ_USB_H_OHCI_1,
        .flags        = IORESOURCE_IRQ,
    },
};

static struct platform_device spear_ohci0 =
{
    .name           = "ohci_hcd",
    .id             = 0,
    .dev = {
        .release                = usb_release,
        .dma_mask               = &usb_dmamask,
        .coherent_dma_mask      = 0xffffffff,
    },
    .num_resources  = ARRAY_SIZE(ohci0_spear_resources),
    .resource       = ohci0_spear_resources,
};
static struct platform_device spear_ohci1 =
{
    .name           = "ohci_hcd",
    .id             = 1,
    .dev = {
        .release                = usb_release,
        .dma_mask               = &usb_dmamask,
        .coherent_dma_mask      = 0xffffffff,
    },
    .num_resources  = ARRAY_SIZE(ohci1_spear_resources),
    .resource       = ohci1_spear_resources,
};

static struct platform_device *spear_usb_devs_ohci[] __initdata = {
    &spear_ohci0,&spear_ohci1
};



    static int __devinit
ohci_syn_start (struct usb_hcd *hcd)
{
    struct ohci_hcd	*ohci = hcd_to_ohci (hcd);
    int		ret;

    ohci->hcca = dma_alloc_coherent (hcd->self.controller,
            sizeof *ohci->hcca, &ohci->hcca_dma, 0);
    if (!ohci->hcca)
        return -ENOMEM;

    memset (ohci->hcca, 0, sizeof (struct ohci_hcca));
    if ((ret = ohci_mem_init (ohci)) < 0) {
        ohci_stop (hcd);
        return ret;
    }
    ohci->regs = hcd->regs;

    if ((ret = ohci_run (ohci)) < 0) {
        dev_err(hcd->self.controller, "can't start\n");
        ohci_stop (hcd);
        return ret;
    }

    create_debug_files (ohci);

#ifdef	DEBUG
    ohci_dump (ohci, 1);
#endif
    return 0;
}

static int syn_start_hc(struct ohci_hcd *ohci, struct platform_device *pdev)
{
    int ret;
    ret = ohci_init(ohci);
    dev_dbg(&pdev->dev, "starting USB Controller\n");
    return 0;
}

static void syn_stop_hc(struct platform_device *pdev)
{
    dev_dbg(&pdev->dev, "stopping USB Controller\n");
}

void ohci_syn_hcd_remove (struct usb_hcd *hcd, struct platform_device *pdev)
{
    usb_remove_hcd(hcd);
    syn_stop_hc(pdev);

    release_mem_region(hcd->rsrc_start, hcd->rsrc_len);
    usb_put_hcd(hcd);

    dev_info(&pdev->dev, "remove: state %x\n", hcd->state);
}

int ohci_syn_hcd_probe (const struct hc_driver *driver,
        struct platform_device *pdev)
{
    int retval;
    struct usb_hcd *hcd = 0;
    struct ohci_hcd *ohci;
    unsigned int *addr = NULL;

    printk("ENTERING OHCI SPEAR PROBE \n");

    if (pdev->num_resources != 2) {
        printk(KERN_ERR "hcd probe: invalid num_resources: %i\n",
                pdev->num_resources);
        return -ENODEV;
    }

    if (pdev->resource[0].flags != IORESOURCE_MEM
            || pdev->resource[1].flags != IORESOURCE_IRQ) {
        printk(KERN_ERR "hcd probe: invalid resource type\n");
        return -ENODEV;
    }

    addr = ioremap_nocache(pdev->resource[0].start,
            pdev->resource[0].end
            - pdev->resource[0].start + 1);

    if (!addr) {
        pr_debug("ioremap failed");
        retval = -ENOMEM;
        goto err1;
    }

    hcd = usb_create_hcd (driver, &pdev->dev, pdev->dev.bus_id);
    if (!hcd) {
        retval = -ENOMEM;
        goto err0;
    }
    
    hcd->rsrc_start = pdev->resource[0].start;
    hcd->rsrc_len = pdev->resource[0].end - pdev->resource[0].start + 1;

    if (!request_mem_region(hcd->rsrc_start, hcd->rsrc_len, hcd_name)) {
        dev_dbg(&pdev->dev, "request_mem_region failed\n");
        retval = -EBUSY;
        goto err1;
    }

    hcd->regs = (void*)(int)hcd->rsrc_start;

    ohci = hcd_to_ohci(hcd);
    ohci_hcd_init(ohci);

    retval = syn_start_hc(ohci, pdev);
    if (retval < 0)
        goto err2;

    retval = usb_add_hcd(hcd, platform_get_irq(pdev, 0), SA_INTERRUPT);
    if (retval == 0) 
        return retval;

    syn_stop_hc(pdev);


err0:
err2:
    usb_put_hcd(hcd);

err1:
    release_mem_region(pdev->resource[0].start,
            pdev->resource[0].end - pdev->resource[0].start + 1);

    return retval;
}


/*-------------------------------------------------------------------------*/

static const struct hc_driver ohci_syn_hc_driver = {
    .description =		hcd_name,
    .product_desc =		"SPEAr OHCI",
    .hcd_priv_size =	sizeof(struct ohci_hcd),

    /*
     * generic hardware linkage
     */
    .irq =			ohci_irq,
    .flags =		HCD_USB11 | HCD_MEMORY,

    /*
     * basic lifecycle operations
     */
    .start =		ohci_syn_start,
    .stop =			ohci_stop,
#ifdef	CONFIG_PM
    /* suspend:		ohci_syn_suspend,  -- tbd */
    /* resume:		ohci_syn_resume,   -- tbd */
#endif

    /*
     * managing i/o requests and associated device resources
     */
    .urb_enqueue =		ohci_urb_enqueue,
    .urb_dequeue =		ohci_urb_dequeue,
    .endpoint_disable =	ohci_endpoint_disable,

    /*
     * scheduling support
     */
    .get_frame_number =	ohci_get_frame,

    /*
     * root hub support
     */
    .hub_status_data =	ohci_hub_status_data,
    .hub_control =		ohci_hub_control,
#ifdef	CONFIG_USB_SUSPEND
    .hub_suspend =		ohci_hub_suspend,
    .hub_resume =		ohci_hub_resume,
#endif
    //Ashwini- adding following LOC
    .start_port_reset =     ohci_start_port_reset,

};

static int ohci_hcd_syn_drv_probe(struct platform_device *dev)
{
    printk (KERN_INFO "Enter ohci_hcd_syn_drv_probe\n");

    return ohci_syn_hcd_probe(&ohci_syn_hc_driver,dev);
    return 0;
}

static int ohci_hcd_syn_drv_remove(struct platform_device *dev)
{
    struct usb_hcd *hcd = platform_get_drvdata(dev);
	ohci_syn_hcd_remove(hcd, dev);
	platform_set_drvdata(dev, NULL);
	return 0;
}

#if	defined(CONFIG_USB_SUSPEND) || defined(CONFIG_PM)
static int ohci_hcd_syn_drv_suspend(struct platform_device *dev, pm_message_t message)
{
    struct ohci_hcd	*ohci = hcd_to_ohci(platform_get_drvdata(dev));

    if (time_before(jiffies, ohci->next_statechange))
        msleep(5);
    ohci->next_statechange = jiffies;

    ohci_to_hcd(ohci)->state = HC_STATE_SUSPENDED;

    return 0;
}

static int ohci_hcd_syn_drv_resume(struct platform_device *dev)
{
    struct ohci_hcd	*ohci = hcd_to_ohci(platform_get_drvdata(dev));

    if (time_before(jiffies, ohci->next_statechange))
        msleep(5);
    ohci->next_statechange = jiffies;

    usb_hcd_resume_root_hub(dev_get_drvdata(dev));
    return 0;
}
#endif

/*
 * Driver definition to register with the platform bus
 */
static struct platform_driver ohci_hcd_syn_driver = {
    .probe		= ohci_hcd_syn_drv_probe,
    .remove		= ohci_hcd_syn_drv_remove,
#if	 defined(CONFIG_PM)
    .suspend	= ohci_hcd_syn_drv_suspend,
    .resume		= ohci_hcd_syn_drv_resume,
#endif
    .driver		= {
        .owner	= THIS_MODULE,
        .name	= "ohci_hcd",
    },
};

static int __init ohci_hcd_syn_init(void)
{

    int ret;
    ret =  platform_add_devices(spear_usb_devs_ohci, ARRAY_SIZE(spear_usb_devs_ohci));

    printk(KERN_INFO "Loading OHCI SPEAR driver\n");
    if (usb_disabled()) {
        printk (KERN_DEBUG "Error: USB is disabled\n");
        return -ENODEV;
    } 

    ret = platform_driver_register(&ohci_hcd_syn_driver);
    return ret;
}

static void __exit ohci_hcd_syn_cleanup (void)
{
    printk(KERN_INFO "Unloading OHCI SPEARhead driver\n");
    platform_driver_unregister(&ohci_hcd_syn_driver);
}

module_init(ohci_hcd_syn_init);
module_exit(ohci_hcd_syn_cleanup);
