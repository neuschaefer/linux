/*
 *
 * puma5.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


#ifndef __MUSB_HDRDF_H__
#define __MUSB_HDRDF_H__

#define MUSB_C_DYNFIFO_DEF
#define MUSB_C_RAM_BITS 10

/*
 * Puma5-specific definitions
 */

/* Integrated highspeed/otg PHY */

//#define	USBPHY_CTL_PADDR	(DAVINCI_SYSTEM_MODULE_BASE + 0x34)
//#define	USBPHY_PHYCLKGD		(1 << 8)
//#define	USBPHY_SESNDEN		(1 << 7)	/* v(sess_end) comparator */
//#define	USBPHY_VBDTCTEN		(1 << 6)	/* v(bus) comparator */
//#define	USBPHY_PHYPLLON		(1 << 4)	/* override pll suspend */
//#define	USBPHY_CLK01SEL		(1 << 3)
//#define	USBPHY_OSCPDWN		(1 << 2)
//#define	USBPHY_PHYSPDWN		(1 << 0)

/* For now include usb OTG module registers here */
#define PUMA5_USB_VERSION_REG		0x00
#define PUMA5_USB_CTRL_REG		0x04
#define PUMA5_USB_STAT_REG		0x08
#define PUMA5_MODE_TGCR_REG		0x10 /* Mode reg Transparent,Gneric,CDC,RNDIS */
#define PUMA5_AUTOREQ_REG		0x14
#define	PUMA5_SRP_FIXTIME_REG		0x18 /* SRP Fixtime reg */
#define	PUMA5_TEARDOWN_REG		0x1C /* TearDown Register*/
#define PUMA5_USB_INT_SOURCE_REG	0x20
#define PUMA5_USB_INT_SET_REG		0x24
#define PUMA5_USB_INT_SRC_CLR_REG	0x28
#define PUMA5_USB_INT_MASK_REG		0x2c
#define PUMA5_USB_INT_MASK_SET_REG	0x30
#define PUMA5_USB_INT_MASK_CLR_REG	0x34
#define PUMA5_USB_INT_SRC_MASKED_REG	0x38
#define PUMA5_USB_EOI_REG		0x3c
#define PUMA5_USB_EOI_INTVEC		0x40

#define PUMA5_GRNDIS_EP1SIZE_REG	0x50
#define PUMA5_GRNDIS_EP2SIZE_REG	0x54
#define PUMA5_GRNDIS_EP3SIZE_REG	0x58
#define PUMA5_GRNDIS_EP4SIZE_REG	0x5C

#define PUMA5_USB_TX_ENDPTS_MASK	0x1f		/* ep0 + 4 tx */
#define PUMA5_USB_RX_ENDPTS_MASK	0x1e		/* 4 rx */

#define PUMA5_USB_USBINT_SHIFT		16
#define PUMA5_USB_TXINT_SHIFT		0
#define PUMA5_USB_RXINT_SHIFT		8

#define PUMA5_USB_USBINT_MASK		0x01ff0000	/* 8 Mentor, DRVVBUS */
#define PUMA5_USB_TXINT_MASK \
	(PUMA5_USB_TX_ENDPTS_MASK << PUMA5_USB_TXINT_SHIFT)
#define PUMA5_USB_RXINT_MASK \
	(PUMA5_USB_RX_ENDPTS_MASK << PUMA5_USB_RXINT_SHIFT)

#define PUMA5_BASE_OFFSET		0x400

#define RX_TEARDOWN_SHIFT_CNT		1
#define TX_TEARDOWN_SHIFT_CNT		17

#define PUMA5_INTR_DRVVBUS          0x0100

/* end point modes Transparent/RNDIS/CDC/Generic */
#define ENDPOINT_MODE_TRANSPARENT	0
#define	ENDPOINT_MODE_RNDIS		1
#define ENDPOINT_MODE_CDC		2
#define ENDPOINT_MODE_GENERIC		3

#define DEFAULT_TX_ENDPOINT_MODE	(ENDPOINT_MODE_GENERIC)
#define DEFAULT_RX_ENDPOINT_MODE        (ENDPOINT_MODE_GENERIC)


#define BOOTCFG_REG_KICK0_OFFS	0x38
#define BOOTCFG_REG_KICK1_OFFS	0x3C
#define BOOTCFG_KICK0_UNLOCK_PATTERN	0x83e70b13
#define BOOTCFG_KICK1_UNLOCK_PATTERN    0x95a4f1e0

#define USBPHY_CTRL_OFFS         0x0120
#define USBPHY_CTRL_PDWN        (1 << 0) /* RW : USB PHY power down ctrl, 0-powered, 1 - off */
#define USBPHY_CTRL_OTGPDWN     (1 << 1) /* USB OTG analog power down ctrl, 0-powered, 1-off */
#define USBPHY_CTRL_PLLON       (1 << 2) /* RW : USB PHY PLL suspend override, 0-normal PLL operation, 
					    1-override PLL suspend state */
#define USBPHY_CTRL_VBUSENS     (1 << 3) /* R  : OTG analog block VBUSSENSE output status 0-vbus not i
					    present, 1-present*/
#define USBPHY_CTRL_VBDTCTEN    (1 << 4) /* RW : vbus comparator enable 0-dis, 1-enb */
#define USBPHY_CTRL_SESNDEN     (1 << 5) /* RW : Session End comparator enable 0-dis,1-enb */
#define USBPHY_CTRL_PWRCLKGD	(1 << 6) /* R: USB PHY Power and Clock Good
						0 = Phy power not ramped or PLL not locked
						1 = Phy power is good and PLL is locked */
#define USBPHY_CTRL_OSCSEL	(1 << 16) /* RW : 0 . Feed USB PHY reference clock from on chip pll
						 1 . Feed USB PHY reference clock from on board oscillator */
 


#define USBPWRCLKGOOD	0x74    /* TODO to be changed using puma5 spec */ 
#define USBPHYDISABLE	0x70    

// bit map of UDB PHY DISABLE register
#define USBPHYDISABLE_USB_CP_DISABLE (1 << 0)
#define USBPHYDISABLE_USB_PHY_ALL_DISABLE (1 << 1)


// bit map of UDB POWER CLOCK GOOD register
#define USBPWRCLKGOOD_USB_POWERCLOCKGOOD (1 << 0)

#ifdef CONFIG_PM
extern void puma5_usb_power_down(struct musb *musb);
extern void puma5_usb_power_up(struct musb *musb);
#endif

//definition of dcl end
#endif	/* __MUSB_HDRDF_H__ */
