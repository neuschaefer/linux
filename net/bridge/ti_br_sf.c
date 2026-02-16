/*
 * ti_br_sf.c
 * Description:
 * TI Selective forwarding implementation
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
#include "br_private.h"

#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER

#undef CONFIG_TI_L2_SELECTIVE_FORWARDER_DEBUG

/**************************************************************************
 *************************** Extern Data Structures ***********************
 **************************************************************************/
extern struct net_device *ti_netdevice[TI_MAX_DEVICE_INDEX];

/**************************************************************************
 * FUNCTION NAME : get_enabled_bit_count
 **************************************************************************
 * DESCRIPTION   :
 *  The function counts the number of bits enabled. 
 *
 * RETURNS       :
 *  Number of enabled bits 0 to 32 (max)
 ***************************************************************************/
static int get_enabled_bit_count(unsigned long long value)
{
    int count = 0;
    while (value != 0)
    {
        value &= value - 1;
        count ++;
    }
    return count;
}

/**************************************************************************
 * FUNCTION NAME : ti_selective_forward
 **************************************************************************
 * DESCRIPTION   :
 *  The function selectively forwards the packets on the interfaces that are 
 *  marked. If the mark is zero, packet is flooded.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 *
 ***************************************************************************/
int ti_selective_forward(struct sk_buff *skb)
{
    if (!skb->ti_selective_fwd_dev_info)
    {
        /* Mark is zero. Return zero to flood the packet */
        return 0;
    }
    else
    {
        unsigned long long  interface_list = skb->ti_selective_fwd_dev_info; 
        unsigned int        dev_index = 0, count;
        struct sk_buff      *skb2;
        struct net_device   *dev = skb->dev;
       
        /* Get number of bits that are marked */ 
        count = get_enabled_bit_count(interface_list);
        
#ifdef CONFIG_TI_PACKET_PROCESSOR
    /* Check if the packet is about to be forwarded to single egress interface ONLY.
       For all the cases of cloning make sure the BYPASS flag is set. */
        if (1 != count)
        {
            TI_HIL_ADD_PP_INFO_FLAGS(skb, TI_HIL_PACKET_FLAG_PP_SESSION_BYPASS);
        }
#endif

        /* Iterate and forward the cloned packet on all interfaces that are marked */
        while (interface_list)
        {
            if (interface_list & 0x1)
            {
                if (((dev = ti_netdevice[dev_index]) != NULL) && (br_port_get_rcu(dev) != NULL))
                {
                    if (count == 1)
                        break;

                    if ((skb2 = skb_clone(skb, GFP_ATOMIC)) != NULL)
                    {
#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_DEBUG
                        printk ("DEBUG Local Bridge: Forwarding Cloned multicast packet from (%s) to (%s) index (%d)\n", skb2->dev->name, dev->br_port->dev->name, dev_index+1);
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_DEBUG */
                        br_forward(br_port_get_rcu(dev), skb2,NULL);
                    }
                    else
                    {
                        printk ( KERN_DEBUG "Local Bridge: SKB Clone failed. Freeing original packet\n");
                        dev_kfree_skb_any(skb); 
                        return -1;
                    }
                }
                else
                {    
                    printk ( KERN_DEBUG "Local Bridge: Device info not found for device index (%d), info = 0x%llx\n", dev_index+1, skb->ti_selective_fwd_dev_info);
                }
                count--;
            }
            dev_index++;
            interface_list >>= 1;
        }
        if ((dev != NULL) && (br_port_get_rcu(dev)!= NULL))
        {
#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_DEBUG
            printk (KERN_DEBUG "Local Bridge: Forwarding Original multicast packet from (%s) to (%s) index (%d)\n", skb->dev->name, dev->br_port->dev->name, dev_index+1);
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_DEBUG */

            /* Send the original packet on the last interface */
            br_forward(br_port_get_rcu(dev), skb,NULL);
        }
        else
        {
            dev_kfree_skb_any(skb);
        }
        return 1;
    }
}

EXPORT_SYMBOL(ti_selective_forward);

#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER */

