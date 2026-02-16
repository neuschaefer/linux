/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */
/*
         Includes Intel Corporation's changes/modifications dated: [Dec.2013].
         Changed/modified portions - Copyright © 2013, Intel Corporation
         1. WARN on NULL pointer in br_handle_frame()
 
         Includes Intel Corporation's changes/modifications dated: [May.2014].
         Changed/modified portions - Copyright © 2014, Intel Corporation
         2. Skip Link Local spacial proccess for L2VPN packets in br_handle_frame()
*/

#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include <linux/export.h>
#include <linux/rculist.h>
#include "br_private.h"

#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER
extern int ti_selective_packet_handler (struct sk_buff *skb);
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER */

#ifdef CONFIG_INTEL_L2VPN_L2CP_FORWARD
extern int intel_l2vpn_packet_handler (struct sk_buff *skb, int *l2vpnRelate);
#endif /* CONFIG_INTEL_L2VPN_L2CP_FORWARD */


/* Hook for brouter */
br_should_route_hook_t __rcu *br_should_route_hook __read_mostly;
EXPORT_SYMBOL(br_should_route_hook);

static int br_pass_frame_up(struct sk_buff *skb)
{
	struct net_device *indev, *brdev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(brdev);
	struct br_cpu_netstats *brstats = this_cpu_ptr(br->stats);

	u64_stats_update_begin(&brstats->syncp);
	brstats->rx_packets++;
	brstats->rx_bytes += skb->len;
	u64_stats_update_end(&brstats->syncp);

	/* Bridge is just like any other port.  Make sure the
	 * packet is allowed except in promisc modue when someone
	 * may be running packet capture.
	 */
	if (!(brdev->flags & IFF_PROMISC) &&
	    !br_allowed_egress(br, br_get_vlan_info(br), skb)) {
		kfree_skb(skb);
		return NET_RX_DROP;
	}

	skb = br_handle_vlan(br, br_get_vlan_info(br), skb);
	if (!skb)
		return NET_RX_DROP;

	indev = skb->dev;
	skb->dev = brdev;

	return NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
		       netif_receive_skb);
}

/* note: already called with rcu_read_lock */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	struct net_bridge *br;
	struct net_bridge_fdb_entry *dst;
	struct net_bridge_mdb_entry *mdst;
	struct sk_buff *skb2;
	bool unicast = true;
	u16 vid = 0;

	if (!p || p->state == BR_STATE_DISABLED)
		goto drop;

	if (!br_allowed_ingress(p->br, nbp_get_vlan_info(p), skb, &vid))
		goto drop;

	br = p->br;


#ifdef CONFIG_BRIDGE_NETFILTER
#ifdef CONFIG_BRIDGE_EBT_FORWARD
	/* Check for predetermined forwarding ebtables rule */
	if (skb->bridge_forward_port) {
		BR_INPUT_SKB_CB(skb)->brdev = br->dev;
		if(br->dev->flags & IFF_PROMISC) {
			/* Forward the frame and send a copy to the bridge interface */
			skb2=skb;
			br_forward(skb->bridge_forward_port, skb, skb2);
			if(skb2)
				return br_pass_frame_up(skb2);
		}
		else {
			/* Forward packet only to destination port */
			skb2=NULL;
			br_forward(skb->bridge_forward_port, skb, skb2);
			goto out;
		}
	}
#endif
#endif

	/* insert into forwarding database after filtering to avoid spoofing */
	if (p->flags & BR_LEARNING)
		br_fdb_update(br, p, eth_hdr(skb)->h_source, vid);

	if (!is_broadcast_ether_addr(dest) && is_multicast_ether_addr(dest) &&
	    br_multicast_rcv(br, p, skb))
		goto drop;

	if (p->state == BR_STATE_LEARNING)
		goto drop;

	BR_INPUT_SKB_CB(skb)->brdev = br->dev;

	/* The packet skb2 goes to the local host (NULL to skip). */
	skb2 = NULL;

	if (br->dev->flags & IFF_PROMISC)
		skb2 = skb;

	dst = NULL;

	if (is_broadcast_ether_addr(dest)) {
		skb2 = skb;
		unicast = false;
	} else if (is_multicast_ether_addr(dest)) {
#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER
        /* If return value is zero indicates fall back to default behavior */
        if (ti_selective_packet_handler(skb) != 0) {			
            goto out;
		}
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER */
		mdst = br_mdb_get(br, skb, vid);
		if ((mdst || BR_INPUT_SKB_CB_MROUTERS_ONLY(skb)) &&
		    br_multicast_querier_exists(br, eth_hdr(skb))) {
			if ((mdst && mdst->mglist) ||
			    br_multicast_is_router(br))
				skb2 = skb;
			br_multicast_forward(mdst, skb, skb2);
			skb = NULL;
			if (!skb2)
				goto out;
		} else
			skb2 = skb;

		unicast = false;
		br->dev->stats.multicast++;
	} else if ((dst = __br_fdb_get(br, dest, vid)) &&
			dst->is_local) {
		skb2 = skb;
		/* Do not forward the packet since it's local. */
		skb = NULL;
	}

	if (skb) {
		if (dst) {
			dst->used = jiffies;
			br_forward(dst->dst, skb, skb2);
		} else {
#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER
			/* If return value is zero indicates fall back to default behavior */
			if (ti_selective_packet_handler(skb) != 0)
				goto out;
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER */
			br_flood_forward(br, skb, skb2, unicast);
		}
	}

	if (skb2)
		return br_pass_frame_up(skb2);

out:
	return 0;
drop:
	kfree_skb(skb);
	goto out;
}

/* note: already called with rcu_read_lock */
static int br_handle_local_finish(struct sk_buff *skb)
{
	struct net_bridge_port *p = br_port_get_rcu(skb->dev);
	u16 vid = 0;

	br_vlan_get_tag(skb, &vid);
	if (p->flags & BR_LEARNING)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source, vid);
	return 0;	 /* process further */
}

/*
 * Return NULL if skb is handled
 * note: already called with rcu_read_lock
 */
rx_handler_result_t br_handle_frame(struct sk_buff **pskb)
{
	struct net_bridge_port *p;
	struct sk_buff *skb = *pskb;
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	br_should_route_hook_t *rhook;

#ifdef CONFIG_INTEL_L2VPN_L2CP_FORWARD
        int l2vpnRelated = 0;
#endif /* CONFIG_INTEL_L2VPN_L2CP_FORWARD */


	if (unlikely(skb->pkt_type == PACKET_LOOPBACK))
		return RX_HANDLER_PASS;

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto drop;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (!skb)
		return RX_HANDLER_CONSUMED;

	/*this cond was added for skb->ti_selective_forward will receive later on the correct vlan device*/
#ifdef CONFIG_TI_DOCSIS_INPUT_DEV
	if(!skb->ti_docsis_input_dev) {
		skb->ti_docsis_input_dev = skb->dev;
	}
#endif

	p = br_port_get_rcu(skb->dev);

	if (WARN(p == NULL, "%s - %d: br_port_get_rcu(skb->dev) returned NULL, dropping packet (dev == %s)", 
	         __func__, __LINE__, skb->dev ? skb->dev->name : "NULL"))
	{
		/* Cannot continue since the following lines require p != NULL */
		/* What should we do with this packet???? */
		goto drop;
	}

	if (unlikely(is_link_local_ether_addr(dest))) {
#ifdef CONFIG_INTEL_L2VPN_L2CP_FORWARD
        /* Check if the packet is L2VPN related. If so, skip link local special proccess. */
        /* Upstream L2VPN packets will be handle Link Local packets in Docsis Bridge */
        if (!intel_l2vpn_packet_handler(skb, &l2vpnRelated)) 
        {
            if (l2vpnRelated)
            {
                goto forward;
            }
        }
#endif /* CONFIG_INTEL_L2VPN_L2CP_FORWARD */
		/*
		 * See IEEE 802.1D Table 7-10 Reserved addresses
		 *
		 * Assignment		 		Value
		 * Bridge Group Address		01-80-C2-00-00-00
		 * (MAC Control) 802.3		01-80-C2-00-00-01
		 * (Link Aggregation) 802.3	01-80-C2-00-00-02
		 * 802.1X PAE address		01-80-C2-00-00-03
		 *
		 * 802.1AB LLDP 		01-80-C2-00-00-0E
		 *
		 * Others reserved for future standardization
		 */
		switch (dest[5]) {
		case 0x00:	/* Bridge Group Address */
			/* If STP is turned off,
			   then must forward to keep loop detection */
			if (p->br->stp_enabled == BR_NO_STP)
				goto forward;
			break;

		case 0x01:	/* IEEE MAC (Pause) */
			goto drop;

		default:
			/* Allow selective forwarding for most other protocols */
			if (p->br->group_fwd_mask & (1u << dest[5]))
				goto forward;
		}

		/* Deliver packet to local host only */
		if (NF_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev,
			    NULL, br_handle_local_finish)) {
			return RX_HANDLER_CONSUMED; /* consumed by filter */
		} else {
			*pskb = skb;
			return RX_HANDLER_PASS;	/* continue processing */
		}
	}

forward:
	switch (p->state) {
	case BR_STATE_FORWARDING:
		rhook = rcu_dereference(br_should_route_hook);
		if (rhook) {
			if ((*rhook)(skb)) {
				*pskb = skb;
				return RX_HANDLER_PASS;
			}
			dest = eth_hdr(skb)->h_dest;
		}
		/* fall through */
	case BR_STATE_LEARNING:
		if (ether_addr_equal(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;
#ifdef CONFIG_BRIDGE_NETFILTER
#ifdef CONFIG_BRIDGE_EBT_FORWARD
		/* Initialize bridge bridge_forward_port so prerouting rule can populate it */
		skb->bridge_forward_port = NULL;	
#endif
#endif
		NF_HOOK(NFPROTO_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		break;
	default:
drop:
		kfree_skb(skb);
	}
	return RX_HANDLER_CONSUMED;
}
