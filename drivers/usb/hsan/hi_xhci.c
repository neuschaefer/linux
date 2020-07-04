/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/



/*****************************************************************************/


static struct resource hiusb_xhci_res[] = {
        [0] = {
                .start = HI_REG_IO_BASE_XHCI,
                .end   = HI_REG_IO_BASE_XHCI + HI_REG_IO_SIZE_XHCI - 1,
                .flags = IORESOURCE_MEM,
        },
        [1] = {
                .start = HI_REG_IRQ_NUM_XHCI,
                .end   = HI_REG_IRQ_NUM_XHCI,
                .flags = IORESOURCE_IRQ,
        },
};


static u64 usb_dmamask = DMA_BIT_MASK(32);

extern hi_void hi_usb_start_hcd(hi_void);
extern hi_void hi_usb_stop_hcd(hi_void);

static struct hiusb_plat_data  hiusb_data = {
	.start_hcd = hi_usb_start_hcd,
	.stop_hcd = hi_usb_stop_hcd,
};

static struct platform_device hiusb_xhci_platdev= {
        .acp    = 0x0,
        .name   = "xhci-hcd",
        .id     = -1,
        .dev    = {
		        .init_name = "hiusb3.0",
                .platform_data  = &hiusb_data,
                .dma_mask = &usb_dmamask,
                .coherent_dma_mask = DMA_BIT_MASK(32),
        },
        .num_resources = ARRAY_SIZE(hiusb_xhci_res),
        .resource       = hiusb_xhci_res,
};





