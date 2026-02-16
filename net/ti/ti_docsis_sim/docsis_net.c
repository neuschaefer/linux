/*
 * docsis_net.c
 *
 *  The file contains the DOCSIS Bridge connectivity to the IP stack
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
 * STRUCTURE NAME : DOCSIS_NET_DEV
 **************************************************************************
 * DESCRIPTION   :
 *  The structure defines a DOCSIS Network device.
 **************************************************************************/
typedef struct DOCSIS_NET_DEV
{
    /* Pointer to the network device. */
    struct net_device*          ptr_device;

    /* Keep track of the statistics. */
    struct net_device_stats     stats;
}DOCSIS_NET_DEV;

/**************************************************************************
 *************************** Extern Functions ******************************
 **************************************************************************/
extern void ti_docsis_receive (struct sk_buff *skb);

/**************************************************************************
 * FUNCTION NAME : ti_docsis_netif_rx
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called by the DOCSIS Bridge to push a packet through
 *  the NET devices.
 *
 * RETURNS       :
 *  Always returns 0
 **************************************************************************/
int ti_docsis_netif_rx(struct sk_buff *skb)
{
    DOCSIS_NET_DEV* ptr_docsis_netdev;
    struct ethhdr*  ptr_ethhdr;

    /* Get the DOCSIS network device */
    ptr_docsis_netdev = skb->dev->priv;

    /* Increment the statistics. */
    ptr_docsis_netdev->stats.rx_packets++;
    ptr_docsis_netdev->stats.rx_bytes += skb->len;

    /* Set the flag if the packet was meant for us; this is called from the DOCSIS
     * Bridge and we know for sure that the skb->mac.raw points to the start of the
     * Ethernet frame. So we check the Destination MAC address of the packet with
     * the MAC Address of the network device and set the flag. This is required else
     * the packet will be dropped in the IP stack. This is the same that is done in the
     * eth_type_trans code. The reason why I did not call the function again is because
     * I want to preserve the original "input_dev" and not have it overwritten. */
    if (memcmp(ptr_docsis_netdev->ptr_device->dev_addr, skb->mac.raw, ETH_ALEN) == 0)
	    skb->pkt_type = PACKET_HOST;

    /* Initialize the protocol. */
    ptr_ethhdr   = (struct ethhdr *)skb->mac.raw;
    skb->protocol = ptr_ethhdr->h_proto;

    /* Pull the Ethernet MAC Address before passing it to the stack. */
    skb_pull(skb,ETH_HLEN);

    /* Push the packet to the IP stack. */
    netif_rx (skb);
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_netdev_xmit
 **************************************************************************
 * DESCRIPTION   :
 *  Registered Transmit function called when a packet is to be transmitted
 *  on to the interconnect device. This function is called by the Local
 *  Bridge to push the packet to the DOCSIS bridge.
 *
 * RETURNS       :
 *  Always returns 0.
 **************************************************************************/
static int ti_docsis_netdev_xmit(struct sk_buff *skb, struct net_device *ptr_netdev)
{
    DOCSIS_NET_DEV* ptr_docsis_netdev;

    /* Get the DOCSIS network device */
    ptr_docsis_netdev = ptr_netdev->priv;

    /* Increment the statistics. */
    ptr_docsis_netdev->stats.tx_packets++;
    ptr_docsis_netdev->stats.tx_bytes += skb->len;

    /* Initialize the data pointers before passing the packet down to the
     * DOCSIS Bridge. */
    skb->mac.raw = skb->data;

    /* Push the packet to the DOCSIS Bridge. */
    ti_docsis_receive (skb);
	return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_netdev_get_stats
 **************************************************************************
 * DESCRIPTION   :
 *  Get the network statistics
 *
 * RETURNS       :
 *  Pointer to the network statistics block
 **************************************************************************/
static struct net_device_stats* ti_docsis_netdev_get_stats(struct net_device *ptr_netdev)
{
    DOCSIS_NET_DEV* ptr_docsis_net_dev;

    /* Get the DOCSIS Local Interconnect Information*/
    ptr_docsis_net_dev = ptr_netdev->priv;

    /* Return the network statistics. */
    return &ptr_docsis_net_dev->stats;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_netdev_setup
 **************************************************************************
 * DESCRIPTION   :
 *  Initialize and override the network structure.
 **************************************************************************/
static void ti_docsis_netdev_setup(struct net_device *ptr_netdev)
{
	DOCSIS_NET_DEV* ptr_docsis_net_dev = ptr_netdev->priv;

    /* Initialize the Ethernet device. */
	ether_setup(ptr_netdev);

    /* Setup the pointers */
	ptr_docsis_net_dev->ptr_device  = ptr_netdev;
	ptr_netdev->hard_start_xmit     = ti_docsis_netdev_xmit;
	ptr_netdev->get_stats           = ti_docsis_netdev_get_stats;

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
 * FUNCTION NAME : ti_docsis_netdev_init
 **************************************************************************
 * DESCRIPTION   :
 *  Initialization of the DOCSIS to the Network stack connectivity.
 *
 * ERROR         :
 *  Not NULL 	-   Success
 *  NULL  		-   Error
 **************************************************************************/
struct net_device* __init ti_docsis_netdev_init(char* name)
{
    struct net_device*       ptr_netdev;

    /* Allocate memory for the network device. */
	ptr_netdev = alloc_netdev(sizeof(DOCSIS_NET_DEV), name, ti_docsis_netdev_setup);
    if(ptr_netdev == NULL)
    {
        printk ("Error: Unable to allocate memory for the DOCSIS to Network stack connectivity\n");
        return NULL;
    }

    /* Register the network device */
	if (register_netdev(ptr_netdev) < 0)
    {
        printk ("Error: Unable to register the DOCSIS to Network stack connectivity\n");
		return NULL;
	}

	/* Return the pointer to the network device. */
	return ptr_netdev;
}


