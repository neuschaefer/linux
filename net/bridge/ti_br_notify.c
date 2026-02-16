/*
 * ti_br_notify.c
 * Description:
 * TI unmanaged bridge implementation
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

#ifdef CONFIG_TI_UNMANAGED_BRIDGE

/**************************************************************************
 * FUNCTION NAME : ti_blackhole
 **************************************************************************
 * DESCRIPTION   :
 *  The function is the registered blackhole routine which swallows all
 *  packets received from the bridge management interface before they reach 
 *  the networking stacks.
 *
 * RETURNS       :
 *  Always return -1 i.e. never pass the packet up the stack
 ***************************************************************************/
int ti_blackhole(struct sk_buff *skb)
{       
	kfree_skb(skb);
    return -1;
}

/**************************************************************************
 * FUNCTION NAME : ti_unmanaged_bridge_handler
 **************************************************************************
 * DESCRIPTION   :
 *  This function is registered with the device notifier and its function
 *  is to detect if a bridged management interface has been bought up. If 
 *  one is detected it blackholes the management interface
 *
 * RETURNS       :
 *  0   - Bridged Management Interface; event has been processed.
 *  1   - Process the event normally as Open Source...
 ***************************************************************************/
int ti_unmanaged_bridge_handler (struct net_device *dev, unsigned long event)
{
    /* Check if the interface is a bridged management interface? */
    if ((dev->priv_flags & IFF_EBRIDGE) == 0)
        return 1;

    /* Check if the bridged management interface is being bought up? */
    if (event == NETDEV_UP)
    {
        ti_register_protocol_handler (dev, ti_blackhole);
        return 0;
    }

    /* All other events are handled as open source. */
    return 1;
}

#endif /* CONFIG_TI_UNMANAGED_BRIDGE */
