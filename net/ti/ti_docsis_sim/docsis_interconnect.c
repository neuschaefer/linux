/*
 * docsis_interconnect.c
 *
 *  The file contains the DOCSIS Bridge to Local Bridge interconnect code
 *  The Linux Local Bridge interfaces only with "network interfaces".
 *  This implementation is thus a virtual networking device in which the
 *  "network interface" transmit function actually pushes packets to the
 *  DOCSIS Bridge.
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>

/**************************************************************************
 *************************** Local Structures *****************************
 **************************************************************************/

/**************************************************************************
 * STRUCTURE NAME : DOCSIS_INTERCONNECT_DEV
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines the DOCSIS Interconnect device which connects the
 *  Local and DOCSIS Bridge.
 **************************************************************************/
typedef struct DOCSIS_INTERCONNECT_DEV
{
    /* Pointer to the network device. */
    struct net_device*          ptr_device;

    /* Keep track of the statistics. */
    struct net_device_stats     stats;
}DOCSIS_INTERCONNECT_DEV;

/**************************************************************************
 *************************** Extern Functions *****************************
 **************************************************************************/

extern void ti_docsis_receive (struct sk_buff *skb);

/**************************************************************************
 * FUNCTION NAME : ti_docsis_interconnect_receive
 **************************************************************************
 * DESCRIPTION   :
 *  Call back function called from the DOCSIS Bridge to push the packet
 *  through the interconnect device back to the LOCAL Bridge.
 *
 * RETURNS       :
 *  Always returns 0
 **************************************************************************/
int ti_docsis_interconnect_receive(struct sk_buff *skb)
{
    DOCSIS_INTERCONNECT_DEV* ptr_docsis_net_dev;

    /* Get the DOCSIS Local Interconnect Information*/
    ptr_docsis_net_dev = (DOCSIS_INTERCONNECT_DEV *)skb->dev->priv;

	/* Increment the statistics. */
	ptr_docsis_net_dev->stats.rx_packets++;
	ptr_docsis_net_dev->stats.rx_bytes += skb->len;

    /* Pull the Ethernet MAC Address before passing it to the stack. */
    skb_pull(skb,ETH_HLEN);

    /* Push the packe to the networking stacks for the bridge to receive it */
    netif_rx (skb);
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_interconnect_xmit
 **************************************************************************
 * DESCRIPTION   :
 *  Registered Transmit function called when a packet is to be transmitted
 *  on to the interconnect device. This function is called by the Local
 *  Bridge to push the packet to the DOCSIS bridge.
 **************************************************************************/
static int ti_docsis_interconnect_xmit(struct sk_buff *skb, struct net_device *ptr_netdev)
{
    DOCSIS_INTERCONNECT_DEV* ptr_docsis_net_dev;

    /* Get the DOCSIS Local Interconnect Information*/
    ptr_docsis_net_dev = (DOCSIS_INTERCONNECT_DEV *)ptr_netdev->priv;

	/* Increment the statistics. */
	ptr_docsis_net_dev->stats.tx_packets++;
	ptr_docsis_net_dev->stats.tx_bytes += skb->len;

    /* Initialize the data pointers before passing the packet down to the
     * DOCSIS Bridge. */
    skb->mac.raw = skb->data;

    /* Push the packet to the DOCSIS Bridge. */
    ti_docsis_receive (skb);
	return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_interconnect_get_stats
 **************************************************************************
 * DESCRIPTION   :
 *  Get the network statistics
 *
 * RETURNS       :
 *  Pointer to the network statistics block
 **************************************************************************/
static struct net_device_stats* ti_docsis_interconnect_get_stats(struct net_device *ptr_netdev)
{
    DOCSIS_INTERCONNECT_DEV* ptr_docsis_net_dev;

    /* Get the DOCSIS Local Interconnect Information*/
    ptr_docsis_net_dev = (DOCSIS_INTERCONNECT_DEV *)ptr_netdev->priv;

    /* Return the network statistics. */
    return &ptr_docsis_net_dev->stats;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_interconnect_setup
 **************************************************************************
 * DESCRIPTION   :
 *  Initialize and override the network structure.
 **************************************************************************/
static void ti_docsis_interconnect_setup(struct net_device *ptr_netdev)
{
	DOCSIS_INTERCONNECT_DEV* ptr_docsis_net_dev = ptr_netdev->priv;

    /* Initialize the Ethernet device. */
	ether_setup(ptr_netdev);

    /* Setup the pointers */
	ptr_docsis_net_dev->ptr_device  = ptr_netdev;
	ptr_netdev->hard_start_xmit     = ti_docsis_interconnect_xmit;
	ptr_netdev->get_stats           = ti_docsis_interconnect_get_stats;

    /* Set the default MAC Address. */
    ptr_netdev->dev_addr[0] = 0x00;
    ptr_netdev->dev_addr[1] = 0xe0;
    ptr_netdev->dev_addr[2] = 0xa0;
    ptr_netdev->dev_addr[3] = 0x33;
    ptr_netdev->dev_addr[4] = 0x66;
    ptr_netdev->dev_addr[5] = 0x00;

    /* Work is completed. */
    return;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_interconnect_init
 **************************************************************************
 * DESCRIPTION   :
 *  Initialization code for the DOCSIS to Local Bridge connectivity
 *
 * ERROR         :
 *  Not NULL 	-   Success
 *  NULL  		-   Error
 **************************************************************************/
struct net_device* __init ti_docsis_interconnect_init(void)
{
    struct net_device* ptr_netdev;

    /* Allocate memory for the network device. */
	ptr_netdev = alloc_netdev(sizeof(DOCSIS_INTERCONNECT_DEV), "dbr0", ti_docsis_interconnect_setup);
    if(ptr_netdev == NULL)
    {
        printk ("Error: Unable to allocate memory for the DOCSIS to Local Bridge connectivity\n");
        return NULL;
    }

    /* Register the network device */
	if (register_netdev(ptr_netdev) < 0)
    {
        printk ("Error: Unable to register network device\n");
        return NULL;
    }

	/* Return the pointer to the network device. */
	return ptr_netdev;
}

