/*
<:copyright-BRCM:2013:DUAL/GPL:standard 

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
*/
/*
 ****************************************************************************
 * File Name  : bcm63xx_usb.c
 *
 * Description: This file contains the initilzation and registration routines
 * to enable USB controllers on bcm63xxx boards. 
 *
 *
 ***************************************************************************/

#if defined(CONFIG_USB) || defined(CONFIG_USB_MODULE)

#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/dma-mapping.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/bug.h>
#include <linux/usb/ehci_pdriver.h>
#include <linux/usb/ohci_pdriver.h>

#include <bcm_map_part.h>
#include <bcm_intr.h>
#include <pmc_usb.h>

extern void bcm_set_pinmux(unsigned int pin_num, unsigned int mux_num);


#define CAP_TYPE_EHCI       0x00
#define CAP_TYPE_OHCI       0x01
#define CAP_TYPE_XHCI       0x02

/*TODO double check the values for these 2 structures */
static struct usb_ehci_pdata bcm_ehci_pdata = {
    .caps_offset         = 0,
    .has_tt              = 0,
    .has_synopsys_hc_bug = 0,
    .port_power_off      = 0,
};

static struct usb_ohci_pdata bcm_ohci_pdata = {};

static struct platform_device *xhci_dev;
static struct platform_device *ehci_dev;
static struct platform_device *ohci_dev;


static __init struct platform_device *bcm_add_usb_host(int type, int id,
                        uint32_t mem_base, uint32_t mem_size, int irq,
                        const char *devname, void *private_data)
{
    struct resource res[2];
    struct platform_device *pdev;
    //static const u64 usb_dmamask = ~(u32)0;
    static const u64 usb_dmamask = 0xffffffff;

    memset(&res, 0, sizeof(res));
    res[0].start = mem_base;
    res[0].end   = mem_base + (mem_size -1);
    res[0].flags = IORESOURCE_MEM;

    res[1].flags = IORESOURCE_IRQ;
    res[1].start = res[1].end = irq;

    pdev = platform_device_alloc(devname, id);
    if(!pdev)
    {
        printk(KERN_ERR "Error Failed to allocate platform device for devname=%s id=%d\n",
                devname, id);
        return 0;
    }

    platform_device_add_resources(pdev, res, 2);

    pdev->dev.dma_mask = (u64 *)&usb_dmamask;
    pdev->dev.coherent_dma_mask = 0xffffffff;

    if(private_data)
    {
        pdev->dev.platform_data = private_data;
    }

    if(platform_device_add(pdev))
    {
        printk(KERN_ERR "Error Failed to add platform device for devname=%s id=%d\n",
                devname, id);
        return 0;
    }

    return pdev;
}

#if defined(CONFIG_BCM963138) && !defined(CONFIG_BRCM_CHIP_VER_A0)
static void bcm63138B0_manual_usb_ldo_start(void)
{
    USBH_CTRL->pll_ctl &= ~(1 << 30); /*pll_resetb=0*/
    USBH_CTRL->utmi_ctl_1 = 0; 
    USBH_CTRL->pll_ldo_ctl = 4; /*ldo_ctl=core_rdy */
    USBH_CTRL->pll_ctl |= ( 1 << 31); /*pll_iddq=1*/
    mdelay(10);
    USBH_CTRL->pll_ctl &= ~( 1 << 31); /*pll_iddq=0*/
    USBH_CTRL->pll_ldo_ctl |= 1; /*ldo_ctl.AFE_LDO_PWRDWNB=1*/
    USBH_CTRL->pll_ldo_ctl |= 2; /*ldo_ctl.AFE_BG_PWRDWNB=1*/
    mdelay(1);
    USBH_CTRL->utmi_ctl_1 = 0x00020002;/* utmi_resetb &ref_clk_sel=0; */ 
    USBH_CTRL->pll_ctl |= ( 1 << 30); /*pll_resetb=1*/
    mdelay(10);
}    


#define XHCI_ECIRA_BASE USB_XHCI_BASE + 0xf90

uint32_t xhci_ecira_read(uint32_t reg)
{
    volatile uint32_t *addr;
    uint32_t value;

    addr = (uint32_t *)(XHCI_ECIRA_BASE + 8);
    *addr =reg;

    addr = (uint32_t *)(XHCI_ECIRA_BASE + 0xc);
    value = *addr; 

    return value;
}

void xhci_ecira_write(uint32_t reg, uint32_t value)
{

    volatile uint32_t *addr;

    addr = (uint32_t *)(XHCI_ECIRA_BASE + 8);
    *addr =reg;

    addr = (uint32_t *)(XHCI_ECIRA_BASE + 0xc);
    *addr =value; 
}

static void bcm63138B0_usb3_erdy_nump_bypass(void)
{
    uint32_t value;

    value = xhci_ecira_read(0xa20c);
    value |= 0x10000;
    xhci_ecira_write(0xa20c, value);
}

#endif

#define MDIO_USB2   0
#define MDIO_USB3   (1 << 31)

static uint32_t usb_mdio_read(volatile uint32_t *mdio, uint32_t reg, int mode)
{
    uint32_t data;

    data = (reg << 16) | mode;
    mdio[0] = data;
    data |= (1 << 24);
    mdio[0] = data;
    mdelay(1);
    data &= ~(1 << 24);
    mdelay(1);

    return (mdio[1] & 0xffff);
}

static void usb_mdio_write(volatile uint32_t *mdio, uint32_t reg, uint32_t val, int mode)
{
    uint32_t data;
    data = (reg << 16) | val | mode;
    *mdio = data;
    data |= (1 << 25);
    *mdio = data;
    mdelay(1);
    data &= ~(1 << 25);
    *mdio = data;
}

static void usb2_eye_fix(void)
{
    /* Updating USB 2.0 PHY registers */
    usb_mdio_write((void *)&USBH_CTRL->mdio, 0x1f, 0x80a0, MDIO_USB2);
    usb_mdio_write((void *)&USBH_CTRL->mdio, 0x0a, 0xc6a0, MDIO_USB2);
}

static void usb3_ssc_enable(void)
{
    uint32 val;

    /* Enable USB 3.0 TX spread spectrum */
    usb_mdio_write((void *)&USBH_CTRL->mdio, 0x1f, 0x8040, MDIO_USB3);
    val = usb_mdio_read((void *)&USBH_CTRL->mdio, 0x01, MDIO_USB3) | 3;
    usb_mdio_write((void *)&USBH_CTRL->mdio, 0x01, val, MDIO_USB3);
}

static __init int bcm_add_usb_hosts(void)
{
    printk("++++ Powering up USB blocks\n");
    if(pmc_usb_power_up(PMC_USB_HOST_ALL))
    {
        printk(KERN_ERR "+++ Failed to Power Up USB Host\n");
        return -1;
    }
    mdelay(1);

    /*  default settings in chip are not correct for pwron/pwrflt, adjust these settings
     *  for IPP to work properly
     */
    bcm_set_pinmux(132, 1);/* usb0_pwrflt */
    bcm_set_pinmux(133, 1);/* usb0_pwron  */
    bcm_set_pinmux(134, 1);/* usb1_pwrflt */
    bcm_set_pinmux(135, 1);/* usb1_pwron  */

    mdelay(1);

    /*initialize XHCI settings*/
#if defined(CONFIG_BCM963138) && !defined(CONFIG_BRCM_CHIP_VER_A0)
    bcm63138B0_manual_usb_ldo_start();
    USBH_CTRL->usb_pm |= XHC_SOFT_RESETB;
    USBH_CTRL->usb30_ctl1 &= ~PHY3_PLL_SEQ_START;
#else
    USBH_CTRL->usb30_ctl1 |= USB3_IOC;
    USBH_CTRL->usb30_ctl1 |= XHC_SOFT_RESETB;
#endif

    USBH_CTRL->usb30_ctl1 |= PHY3_PLL_SEQ_START;

#if defined(CONFIG_BCM963138)
#if defined(CONFIG_BRCM_CHIP_VER_A0)
    /* work around to avoid USB3.0 issue of contoller being reset when UBUS is loaded */
    USBH_CTRL->bridge_ctl = (USBH_CTRL->bridge_ctl & 0xFFFF00FF) | (0x1000);
#else /*63138B0 */
     bcm63138B0_usb3_erdy_nump_bypass();
#endif
#endif

    /*adjust the default AFE settings for better eye diagrams */
     usb2_eye_fix();

    /*enable SSC for usb3.0 */
     usb3_ssc_enable();

    /*initialize EHCI & OHCI settings*/
    USBH_CTRL->bridge_ctl &= ~(EHCI_ENDIAN_SWAP | OHCI_ENDIAN_SWAP);
    USBH_CTRL->setup |= (USBH_IOC);
    USBH_CTRL->setup |= (USBH_IPP);

    xhci_dev = bcm_add_usb_host(CAP_TYPE_XHCI, 0, USB_XHCI_PHYS_BASE,
        0x1000, INTERRUPT_ID_USB_XHCI, "xhci-hcd", NULL);
    ehci_dev = bcm_add_usb_host(CAP_TYPE_EHCI, 0, USB_EHCI_PHYS_BASE,
        0x100, INTERRUPT_ID_USB_EHCI, "ehci-platform", &bcm_ehci_pdata);
    ohci_dev = bcm_add_usb_host(CAP_TYPE_OHCI, 0, USB_OHCI_PHYS_BASE,
        0x100, INTERRUPT_ID_USB_OHCI, "ohci-platform", &bcm_ohci_pdata);

    return 0;
}

#if defined CONFIG_USB_MODULE || defined CONFIG_USB_XHCI_HCD_MODULE
static void bcm_mod_cleanup(void)
{
    // we want to just disable usb interrupts and power down usb
    // we'll probably be restart later, re-add resources ok then?
    platform_device_del(xhci_dev);
    platform_device_del(ehci_dev);
    platform_device_del(ohci_dev);
    pmc_usb_power_down(PMC_USB_HOST_ALL);
    mdelay(1);
}

module_init(bcm_add_usb_hosts);
module_exit(bcm_mod_cleanup);

MODULE_LICENSE("GPL");
#else
arch_initcall(bcm_add_usb_hosts);
#endif

#endif /* defined(CONFIG_USB) || defined(CONFIG_USB_MODULE) */
