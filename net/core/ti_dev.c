/*
 * ti_dev.c
 * Description:
 * TI network device implementation
 *
 *  Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
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
 */

#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <net/sock.h>
#include <net/ip.h>
#include <net/dsfield.h>
#include <linux/udp.h>

#ifdef CONFIG_TI_DEVICE_PROTOCOL_HANDLING

/**************************************************************************
 * FUNCTION NAME : ti_protocol_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called before the packet is passed to the networking 
 *  stacks so that any custom protocol handling can be done. 
 *
 * RETURNS       :
 *  0   -   Packet can be passed up the NET stack
 *  <0  -   The packet should NOT be passed up the networking stack
 *
 * NOTES         :
 *  In the case of error; it is the responsibility of the registered packet
 *  handler to clean memory.
 ***************************************************************************/
int ti_protocol_handler (struct net_device* dev, struct sk_buff *skb)
{
    /* Check if there is a packet handler installed on the device or not? */
    if (dev->packet_handler == NULL)
        return 0;

    /* Pass the control to the packet handler. */
    return dev->packet_handler(skb);
}

/**************************************************************************
 * FUNCTION NAME : ti_register_protocol_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers a packet handler which passes all the received
 *  packets to the packet handler for the device.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_register_protocol_handler (struct net_device* dev, int (*packet_handler)(struct sk_buff *skb))
{
    if (dev->packet_handler != NULL)
    {
        printk ("Info: Device %s already has a packet handler 0x%p installed\n", dev->name, dev->packet_handler);
        return -1;
    }

    /* Register the packet handler. */
    dev->packet_handler = packet_handler;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_deregister_protocol_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters the packet handler
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_deregister_protocol_handler (struct net_device* dev)
{
    if (dev->packet_handler == NULL)
    {
        printk ("Info: Device %s has no packet handler installed\n", dev->name);
        return -1;
    }

    /* Register the packet handler. */
    dev->packet_handler = NULL;
    return 0;
}


EXPORT_SYMBOL(ti_register_protocol_handler); 
EXPORT_SYMBOL(ti_deregister_protocol_handler);

#endif /* CONFIG_TI_DEVICE_PROTOCOL_HANDLING */


#ifdef CONFIG_INTEL_NS_DEVICE_FILTER
/**************************************************************************
 * FUNCTION NAME : intel_ns_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function calls the ns_handler function of the device
 *
 * RETURNS       :
 *  0   -   drop
 *  1  -   accept
 ***************************************************************************/
 
int intel_ns_handler (struct net_device *dev,struct	in6_addr* dst_addr,unsigned char banned_flags)
{
    if (dev->ns_handler == NULL)
        return 1;

    return dev->ns_handler(dev,dst_addr,banned_flags);
}


/**************************************************************************
 * FUNCTION NAME : intel_register_ns_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers a neighbour solicitation handler which passes all the received
 *  packets to the ns handler for the device.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int intel_register_ns_handler (struct net_device* dev, int (*ns_handler)(struct net_device *dev,struct	in6_addr* dst_addr,unsigned char banned_flags))
{
    if (dev->ns_handler != NULL)
    {
        printk ("Info: Device %s already has a packet handler 0x%p installed\n", dev->name, dev->ns_handler);
        return -1;
    }

    /* Register the packet handler. */
    dev->ns_handler = ns_handler;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : intel_deregister_ns_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters the ns handler
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int intel_deregister_ns_handler (struct net_device* dev)
{
    if (dev->ns_handler == NULL)
    {
        printk ("KERN_INFO : Device %s has no packet handler installed\n", dev->name);
        return -1;
    }

    /* Register the packet handler. */
    dev->ns_handler = NULL;
    return 0;
}

EXPORT_SYMBOL(intel_register_ns_handler); 
EXPORT_SYMBOL(intel_deregister_ns_handler);
EXPORT_SYMBOL(intel_ns_handler);
#endif




#ifdef CONFIG_TI_DEVICE_INDEX_REUSE

/**************************************************************************
 * FUNCTION NAME : ti_dev_new_index
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called to allocate a new unique device index. The maximum 
 *  number of device index is 64
 *  stacks so that any custom protocol handling can be done. 
 *
 * RETURNS       :
 *  >0  -   Allocated device index
 *  -1  -   No free device Index. 
 *
 * NOTES         :
 *  In the case of error; The netdevice_register() function should 
 *          handle the error condition and return error
 ***************************************************************************/
int ti_dev_new_index(struct net *net)
{
	int ifindex = 0;

    while (ifindex++ < TI_MAX_DEVICE_INDEX)
    {
		if (!__dev_get_by_index(net,ifindex))
			return ifindex;
	}
    printk ("Error: Failed to allocate netdevice index\n");
    return -1;
}
#endif /* CONFIG_TI_DEVICE_INDEX_REUSE */


#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
/* Global array used to store the net device info. Max is 32 devices */
struct  net_device  *ti_netdevice[TI_MAX_DEVICE_INDEX];

/**************************************************************************
 * FUNCTION NAME : ti_save_netdevice_info
 **************************************************************************
 * DESCRIPTION   :
 *  The function saves the net_device pointer in the global array. Called by 
 *  register_netdevice() function
 *
 * RETURNS       :
 *  None
 *
 * NOTES         :
 *  The device index is 1 based and the array access is zero based
 ***************************************************************************/
void ti_save_netdevice_info(struct net_device *dev)
{
    if (dev != NULL)
        ti_netdevice[dev->ifindex - 1] = dev;
}

/**************************************************************************
 * FUNCTION NAME : ti_free_netdevice_info
 **************************************************************************
 * DESCRIPTION   :
 *  The function free the net_device pointer stored in the global array. 
 *  Called by unregister_netdevice() function
 *
 * RETURNS       :
 *  None
 *
 * NOTES         :
 *  The device index is 1 based and the array access is zero based
 ***************************************************************************/
void ti_free_netdevice_info(struct net_device *dev)
{
    if (dev != NULL)
        ti_netdevice[dev->ifindex - 1] = NULL;
}

#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER */


#ifdef CONFIG_TI_EGRESS_HOOK

/**************************************************************************
 * FUNCTION NAME : ti_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called before the packet is passed to the driver for
 *  transmission. 
 *
 * RETURNS       :
 *  0   -   Packet can be passed to the driver.
 *  <0  -   The packet should NOT be passed to the driver.
 *
 * NOTES         :
 *  If the hook does not want the packet to be passed to the driver it
 *  is the responsibility of the hook to clean the packet memory.
 ***************************************************************************/
int ti_egress_hook_handler (struct net_device* dev, struct sk_buff *skb)
{
    /* Check if there is an egress hook installed on the device or not? */
    if (dev->egress_hook == NULL)
        return 0;

    /* Pass the control to the egress hook */
    return dev->egress_hook(skb);
}

/**************************************************************************
 * FUNCTION NAME : ti_register_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers an egress hook which is attached to a networking
 *  device. 
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_register_egress_hook_handler (struct net_device* dev, int (*egress_hook)(struct sk_buff *skb))
{
    /* Check if an egress hook is already attached. */
    if (dev->egress_hook != NULL)
    {
        printk ("Info: Device %s already has an egress hook 0x%p installed\n", dev->name, dev->egress_hook);
        return -1;
    }

    /* Register the egress hook. */
    dev->egress_hook = egress_hook;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_deregister_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters the egress hook.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_deregister_egress_hook_handler (struct net_device* dev)
{
    if (dev->egress_hook == NULL)
    {
        printk ("Info: Device %s has no egress hook installed\n", dev->name);
        return -1;
    }

    /* De-register the egress hook. */
    dev->egress_hook = NULL;
    return 0;
}

EXPORT_SYMBOL(ti_register_egress_hook_handler); 
EXPORT_SYMBOL(ti_deregister_egress_hook_handler);
EXPORT_SYMBOL(ti_protocol_handler); 

#endif /* CONFIG_TI_EGRESS_HOOK */

#ifdef CONFIG_TI_DOCSIS_EGRESS_HOOK

/**************************************************************************
 * FUNCTION NAME : ti_docsis_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called before the packet is passed to the driver for
 *  transmission. 
 *
 * RETURNS       :
 *  0   -   Packet can be passed to the driver.
 *  <0  -   The packet should NOT be passed to the driver.
 *
 * NOTES         :
 *  If the hook does not want the packet to be passed to the driver it
 *  is the responsibility of the hook to clean the packet memory.
 ***************************************************************************/
int ti_docsis_egress_hook_handler (struct net_device* dev, struct sk_buff *skb)
{
    /* Check if there is an egress hook installed on the device or not? */
    if (dev->docsis_egress_hook == NULL)
        return 0;

    /* Pass the control to the egress hook */
    return dev->docsis_egress_hook(skb);
}

/**************************************************************************
 * FUNCTION NAME : ti_register_docsis_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers an egress hook which is attached to a networking
 *  device. 
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_register_docsis_egress_hook_handler (struct net_device* dev, int (*docsis_egress_hook)(struct sk_buff *skb))
{
    /* Check if an egress hook is already attached. */
    if (dev->docsis_egress_hook != NULL)
    {
        printk ("Error: Device %s has an egress hook 0x%p installed\n", dev->name, dev->docsis_egress_hook);
        return -1;
    }

    /* Register the egress hook. */
    dev->docsis_egress_hook = docsis_egress_hook;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_deregister_docsis_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters the egress hook.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_deregister_docsis_egress_hook_handler (struct net_device* dev)
{
    if (dev->docsis_egress_hook == NULL)
    {
        printk ("Error: Device %s has no egress hook installed\n", dev->name);
        return -1;
    }

    /* De-register the egress hook. */
    dev->docsis_egress_hook = NULL;
    return 0;
}

EXPORT_SYMBOL(ti_register_docsis_egress_hook_handler); 
EXPORT_SYMBOL(ti_deregister_docsis_egress_hook_handler);

#endif /* CONFIG_TI_DOCSIS_EGRESS_HOOK */

#ifdef CONFIG_TI_GW_EGRESS_HOOK

/**************************************************************************
 * FUNCTION NAME : ti_gw_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called before the packet is passed to the driver for
 *  transmission. 
 *
 * RETURNS       :
 *  0   -   Packet can be passed to the driver.
 *  <0  -   The packet should NOT be passed to the driver.
 *
 * NOTES         :
 *  If the hook does not want the packet to be passed to the driver it
 *  is the responsibility of the hook to clean the packet memory.
 ***************************************************************************/
int ti_gw_egress_hook_handler (struct net_device* dev, struct sk_buff *skb)
{
    /* Check if there is an egress hook installed on the device or not? */
    if (dev->gw_egress_hook == NULL)
        return 0;

    /* Pass the control to the egress hook */
    return dev->gw_egress_hook(skb);
}

/**************************************************************************
 * FUNCTION NAME : ti_register_gw_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers an egress hook which is attached to a networking
 *  device. 
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_register_gw_egress_hook_handler (struct net_device* dev, int (*gw_egress_hook)(struct sk_buff *skb))
{
    /* Check if an egress hook is already attached. */
    if (dev->gw_egress_hook != NULL)
    {
        printk ("Error: Device %s has an egress hook 0x%p installed\n", dev->name, dev->gw_egress_hook);
        return -1;
    }
	
    /* Register the egress hook. */
    dev->gw_egress_hook = gw_egress_hook;
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_deregister_gw_egress_hook_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function deregisters the egress hook.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int ti_deregister_gw_egress_hook_handler (struct net_device* dev)
{
    if (dev->gw_egress_hook == NULL)
    {
        printk ("Error: Device %s has no egress hook installed\n", dev->name);
        return -1;
    }

    /* De-register the egress hook. */
    dev->gw_egress_hook = NULL;
    return 0;
}

EXPORT_SYMBOL(ti_register_gw_egress_hook_handler); 
EXPORT_SYMBOL(ti_deregister_gw_egress_hook_handler);

#endif /* CONFIG_TI_GW_EGRESS_HOOK */

