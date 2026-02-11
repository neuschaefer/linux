/*****************************************************************************
* Copyright 2006 - 2008 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/

#ifndef _NET_PLATFORM_H
#define _NET_PLATFORM_H

/*
 * Board dependent configuration for the Ethernet driver
 */
struct island_net_hw_cfg
{
   /*
    * The PHYs are external to the chip, so the connections are dependent on
    * the hardware layout.  The following module arguments can be used to
    * define:
    * -The MDIO PHY address
    * -GPIO pin connected to reset input of the PHY
    * -PHY interrupt pins
    * -PHY power enable
    */
   int addrPhy0;	/* Phy0 address */
   int addrPhy1;	/* Phy1 address */

   int gpioPhy0;	/* Phy0 reset */
   int gpioPhy1;	/* Phy1 reset */

   int gpioPhy0_int;	/* Phy0 interrupt pin */
   int gpioPhy1_int;	/* Phy1 interrupt pin */
   int gpioPhyPwrEn;	/* Phy power gate */

   char *phy_regulator_name; /* name of the PHY LDO */
};

#endif /* _NET_PLATFORM_H */
