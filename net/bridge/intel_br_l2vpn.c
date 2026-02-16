/* 
  GPL LICENSE SUMMARY

  Copyright(c) 2014 Intel Corporation.

  This program is free software; you can redistribute it and/or modify 
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.

  Contact Information:
    Intel Corporation
    2200 Mission College Blvd.
    Santa Clara, CA  97052
*/

/*
 * intel_br_l2vpn.c
 * Description:
 * Intel L2VPN implementation
 */

#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
//#include <net/net_namespace.h>
#include "br_private.h"


/**************************************************************************
 * FUNCTION NAME : intel_l2vpn_packet_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function is called from the bridge to verify if the packet is l2vpn l2vpn related.
 *  If so, bridge will skip the special handling of link-local packets, and the handling 
 *  will be done in docsis bridge
 *    
 * RETURNS       :
 *  0   -   No packet handler. invoke default behavior 
 *  ret -   Returns the return code passed by the registered functions
 ***************************************************************************/
int intel_l2vpn_packet_handler (struct sk_buff *skb, int *l2vpnRelate)
{

	struct net_bridge       *br;
	struct net_bridge_port  *p;
    struct l2_sph       *handle;
 
    int                 ret = 0;
	/* Get bridge port. */ 
    if ((p = br_port_get_rcu(skb->dev)) == NULL) 
    {       
        return -1;
    }
    
    /* Get  bridge is valid. */ 
    if ((br = p->br) == NULL) 
    {        
        return -1;
    }
    if (br->l2vpn_packet_handler  == NULL)
    {
        return -1;

    }
    ret = br->l2vpn_packet_handler(skb, l2vpnRelate); 
    
}

/**************************************************************************
 * FUNCTION NAME : intel_register_l2vpn_packet_handler
 **************************************************************************
 * DESCRIPTION   :
 *  The function registers/Unregisters a l2vpn packet handler for a bridge.
 *  
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 ***************************************************************************/
int intel_register_l2vpn_packet_handler (char *br_name, int (*packet_handler)(struct sk_buff *skb, int *sfIndex, int *phsIndex, int *l2vpnRelate))
{
    struct net_bridge       *br;
	struct net_device       *dev;
	struct net_bridge_port  *p;

    /* Check if device is valid. */ 
    if ((dev = dev_get_by_name (&init_net, br_name)) == NULL)
    {
        printk ("Error: Device %s does not exist\n", br_name);
        return -1;
    }

    /* Check if bridge port. */ 
    if ((p = br_port_get_rcu(dev)) == NULL) 
    {
        printk ("Error: Bridge port %s does not exist\n", br_name);
        return -1;
    }
    
    /* Check if bridge is valid. */     
    if ((br = p->br) == NULL) 
    {
        printk ("Error: Bridge %s does not exist\n", br_name);
        return -1;
    }

    br->l2vpn_packet_handler = packet_handler; 

    if (packet_handler)
    {
        printk ("DEBUG: Register l2vpn packet handler successful\n"); 
    }
    else
    {
        printk ("DEBUG: Unregister l2vpn packet handler successful\n"); 
    }


    return 0;
}

EXPORT_SYMBOL(intel_register_l2vpn_packet_handler); 
