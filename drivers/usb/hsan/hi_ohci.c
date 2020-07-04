/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/******************************************************************************

                  版权所有 (C), 2008-2018, 海思半导体有限公司

 ******************************************************************************
  文 件 名   : hi_ohci.c
  版 本 号   : 初稿
  生成日期   : D2014_03_28

******************************************************************************/


#include <linux/platform_device.h>
#include <mach/hi_hsan.h>

extern hi_int32 usb_disabled(hi_void);
extern hi_void hi_usb_start_hcd(hi_void);
extern hi_void hi_usb_stop_hcd(hi_void);

static struct resource g_st_ohci_res[] = 
{
    {
        .start = 0x10A50000,  // ohci reg base addr 
        .end   = 0x10A50000 + 0x10000 - 1,
        .flags = IORESOURCE_MEM,
    },
    {
        .start = 70,         // ohci irq number
        .end   = 70,
        .flags = IORESOURCE_IRQ,
    },
};

static u64 g_ul_usb_dmamask = DMA_BIT_MASK(32);
static struct platform_device g_st_ohci_platdev =
{
        .name   = "hi-ohci",
        .id     = 0,
        .dev    = 
        {
            .platform_data     = HI_NULL,
            .dma_mask          = &g_ul_usb_dmamask,
            .coherent_dma_mask = DMA_BIT_MASK(32),
            .release           = HI_NULL,
        },
        .num_resources = ARRAY_SIZE(g_st_ohci_res),
        .resource       = g_st_ohci_res,
};


hi_int32 hi_ohci_init(hi_void)
{
    hi_int32 i_ret = 0; 
    
	if(HI_CHIP_ID_T_E == hi_get_chip_id())
	{
	    return -ENODEV;
    }

    i_ret = platform_device_register(&g_st_ohci_platdev);
    if (0 != i_ret)
    {
        hi_printk("%s->%d, platform_device_register fail.\n", __FUNCTION__, __LINE__);
        return -ENODEV;        
    }

    return 0; 
}

hi_void hi_ohci_exit(hi_void)
{
	if(HI_CHIP_ID_T_E == hi_get_chip_id())
	{
	    return;
    }
    platform_device_unregister(&g_st_ohci_platdev);    
}

static hi_int32 hi_ohci_start(struct usb_hcd *pst_hcd)
{
	struct ohci_hcd	*pst_ohci = hcd_to_ohci(pst_hcd);
	hi_int32         i_ret    = 0;

    i_ret = ohci_init(pst_ohci); 
    if(0 != i_ret)
    {
		return i_ret;
    }

    i_ret = ohci_run(pst_ohci); 
    if (0 != i_ret)
    {
		ohci_stop(pst_hcd);
		return i_ret;
    }

	return 0;
}

static const struct hc_driver g_st_ohci_hc_driver = 
{
	.description      = hcd_name,
	.product_desc     = "hi-ohci",
	.hcd_priv_size    = sizeof(struct ohci_hcd),

	.irq              = ohci_irq,
	.flags            = HCD_USB11 | HCD_MEMORY,
	
	.start            = hi_ohci_start,
	.stop             =	ohci_stop,
	.shutdown         =	ohci_shutdown,

	.urb_enqueue      =	ohci_urb_enqueue,
	.urb_dequeue      =	ohci_urb_dequeue,
	.endpoint_disable =	ohci_endpoint_disable,

	.get_frame_number =	ohci_get_frame,

	.hub_status_data  =	ohci_hub_status_data,
	.hub_control      =	ohci_hub_control,
	.start_port_reset =	ohci_start_port_reset,
}; 

static hi_int32 hi_ohci_hcd_drv_probe(struct platform_device *pst_dev)
{
	hi_int32        i_ret   = 0;
	struct usb_hcd *pst_hcd = HI_NULL;

	if (usb_disabled())
	{
		return -ENODEV;        
	}

	if (IORESOURCE_IRQ != pst_dev->resource[1].flags) 
	{
		return -ENOMEM;
	}

	pst_hcd = usb_create_hcd(&g_st_ohci_hc_driver, &pst_dev->dev, "hi-ohci");
	if (HI_NULL == pst_hcd)
	{
		return -ENOMEM;
	}
    
	pst_hcd->rsrc_start = pst_dev->resource[0].start;
	pst_hcd->rsrc_len   = pst_dev->resource[0].end - pst_dev->resource[0].start + 1;

	if (HI_NULL == request_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len, hcd_name)) 
	{
		hi_printk("request_mem_region failed\n");
		i_ret = -EBUSY;
        usb_put_hcd(pst_hcd);
        return -EBUSY; 
	}

	pst_hcd->regs = ioremap(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
	if (HI_NULL == pst_hcd->regs) 
	{
		hi_printk("ioremap failed\n");
        release_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
        usb_put_hcd(pst_hcd);
        return -ENOMEM;
	}

	hi_usb_start_hcd();
	ohci_hcd_init(hcd_to_ohci(pst_hcd));

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

static int hi_ohci_hcd_drv_remove(struct platform_device *pst_dev)
{
	struct usb_hcd *pst_hcd = platform_get_drvdata(pst_dev);

	usb_remove_hcd(pst_hcd);
	hi_usb_stop_hcd();
	iounmap(pst_hcd->regs);
	release_mem_region(pst_hcd->rsrc_start, pst_hcd->rsrc_len);
	usb_put_hcd(pst_hcd);
	platform_set_drvdata(pst_dev, HI_NULL); 
	return 0;
}


static struct platform_driver g_st_ohci_driver = 
{
	.probe		= hi_ohci_hcd_drv_probe,
	.remove		= hi_ohci_hcd_drv_remove,
	.shutdown	= usb_hcd_platform_shutdown,
	.driver		= {
		.name	= "hi-ohci",
		.owner	= THIS_MODULE,
		.pm	= HI_NULL,
	},
};

MODULE_ALIAS("platform:hi-ohci");

