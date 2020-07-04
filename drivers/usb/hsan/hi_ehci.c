/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_ehci.c
  版 本 号   : 初稿
  生成日期   : D2014_03_28

******************************************************************************/


#include <linux/platform_device.h>
#include <mach/hi_hsan.h>

extern int usb_disabled(void);
extern hi_void hi_usb_start_hcd(hi_void);
extern hi_void hi_usb_stop_hcd(hi_void);

static struct resource g_st_ehci_res[] = 
{
        [0] = {
                .start = 0x10A40000,                // ehci reg base addr
                .end   = 0x10A40000 + 0x10000 - 1,  
                .flags = IORESOURCE_MEM,
        },
        [1] = {
                .start = 71,                       // ehci irq number 
                .end   = 71,
                .flags = IORESOURCE_IRQ,
        },
};

static u64 g_ul_usb_dmamask = DMA_BIT_MASK(32); 
static struct platform_device g_st_ehci_platdev = 
{
        .name   = "hi-ehci",
        .id     = 0,
        .dev    = {
                .platform_data     = NULL,
                .dma_mask          = &g_ul_usb_dmamask,
                .coherent_dma_mask = DMA_BIT_MASK(32),
        },
        .num_resources = ARRAY_SIZE(g_st_ehci_res),
        .resource      = g_st_ehci_res,
};

hi_int32 hi_ehci_init(hi_void)
{
    hi_int32 i_ret = 0; 
    
    if(HI_CHIP_ID_T_E == hi_get_chip_id())
    {
        return -ENODEV;
    }

    i_ret = platform_device_register(&g_st_ehci_platdev);
    if (0 != i_ret)
    {
        hi_printk("%s->%d, platform_device_register fail.\n", __FUNCTION__, __LINE__);
        return -ENODEV;
    } 

    return 0; 
}

hi_void hi_ehci_exit(hi_void)
{
    if(HI_CHIP_ID_T_E == hi_get_chip_id())
    {
        return ;
    }
    
    platform_device_unregister(&g_st_ehci_platdev);
}

static const struct hc_driver g_st_ehci_hc_driver = 
{
	.description		= hcd_name,
	.product_desc		= "hi-ehci",
	.hcd_priv_size		= sizeof(struct ehci_hcd),

	.irq			    = ehci_irq,
	.flags			    = HCD_MEMORY | HCD_USB2,

	.reset			    = ehci_init,
	.start			    = ehci_run,
	.stop			    = ehci_stop,
	.shutdown		    = ehci_shutdown,

	.urb_enqueue		= ehci_urb_enqueue,
	.urb_dequeue		= ehci_urb_dequeue,
	.endpoint_disable	= ehci_endpoint_disable,

	.get_frame_number	= ehci_get_frame,

	.hub_status_data	= ehci_hub_status_data,
	.hub_control		= ehci_hub_control,
	.bus_suspend		= ehci_bus_suspend,
	.bus_resume		    = ehci_bus_resume,
	.relinquish_port	= ehci_relinquish_port,
	.port_handed_over	= ehci_port_handed_over,
};

static hi_int32 hi_ehci_probe(struct platform_device *pst_dev)
{
	struct usb_hcd  *pst_hcd  = HI_NULL;
	struct ehci_hcd *pst_ehci = HI_NULL;
	struct resource *pst_res  = HI_NULL;
	hi_int32         i_ret    = 0;

	if (usb_disabled())
	{
		return -ENODEV;        
	}

	if (IORESOURCE_IRQ != pst_dev->resource[1].flags)
	{
		hi_printk("resource[1] is not IORESOURCE_IRQ");
		return -ENOMEM;
	}
    
	pst_hcd = usb_create_hcd(&g_st_ehci_hc_driver, &pst_dev->dev, "hi-ehci");
	if (HI_NULL == pst_hcd)
	{
		return -ENOMEM;        
	}

	pst_res = platform_get_resource(pst_dev, IORESOURCE_MEM, 0);
	pst_hcd->rsrc_start = pst_res->start;
	pst_hcd->rsrc_len   = resource_size(pst_res);
	if (HI_NULL == request_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len, hcd_name)) 
	{
		hi_printk("request_mem_region failed");
        usb_put_hcd(pst_hcd);
        return -EBUSY;
	}

	pst_hcd->regs = ioremap(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
	if (HI_NULL == pst_hcd->regs) 
	{
		hi_printk("ioremap failed");
        release_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
        usb_put_hcd(pst_hcd);
        return -ENOMEM;
	}

	hi_usb_start_hcd();
	pst_ehci = hcd_to_ehci(pst_hcd);
	pst_ehci->caps = pst_hcd->regs;
	pst_ehci->regs = pst_hcd->regs + HC_LENGTH(pst_ehci, readl(&pst_ehci->caps->hc_capbase));
	/* cache this readonly data; minimize chip reads */
	pst_ehci->hcs_params = readl(&pst_ehci->caps->hcs_params);
	i_ret = usb_add_hcd(pst_hcd, pst_dev->resource[1].start, IRQF_DISABLED | IRQF_SHARED);
	if (0 != i_ret) 
	{
        hi_usb_stop_hcd();
        iounmap(pst_hcd->regs);
        release_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
        usb_put_hcd(pst_hcd);
        return i_ret;
	}
    
    platform_set_drvdata(pst_dev, pst_hcd);
    return 0;
}

static int hi_ehci_remove(struct platform_device *pst_dev)
{
	struct usb_hcd *pst_hcd = platform_get_drvdata(pst_dev);

	usb_remove_hcd(pst_hcd); 
	iounmap(pst_hcd->regs); 
	release_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len); 
	usb_put_hcd(pst_hcd); 
	hi_usb_stop_hcd(); 
	platform_set_drvdata(pst_dev, HI_NULL); 
	return 0; 
}

static struct platform_driver g_st_ehci_driver =
{
	.probe		= hi_ehci_probe,
	.remove		= hi_ehci_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver     =
	{
		.name	= "hi-ehci",
		.owner	= THIS_MODULE,
		.pm	    = HI_NULL,
	}
};

MODULE_ALIAS("platform:hi-ehci");


