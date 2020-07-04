/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 *	Device handling code
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

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netpoll.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/list.h>
#include <linux/netfilter_bridge.h>

#include <asm/uaccess.h>
#include <linux/atphooks.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_WL) 
#include "linux/bcm_skb_defines.h"
#endif

#ifdef CONFIG_MLD_SNOOPING
#include "br_mld_snooping.h"
#endif

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#if defined(CONFIG_BCM_KF_WL)
#if defined(PKTC)
#include <linux_osl_dslcpe_pktc.h>
extern uint32_t (*wl_pktc_req_hook)(int req_id, uint32_t param0, uint32_t param1, uint32_t param2);
#endif /* PKTC */
#endif
#endif
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
#include "br_igmp.h"
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#endif

#ifdef CONFIG_IGMP_SNOOPING
extern int br_igmp_snooping_forward(struct sk_buff *skb, struct net_bridge *br,unsigned char *dest,int forward);
#endif

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
static struct net_device_stats *br_dev_get_stats(struct net_device *dev)
{
	//struct net_bridge *br = netdev_priv(dev);
	//return &br->statistics;
	return &dev->stats;
	
}

static BlogStats_t * br_dev_get_bstats(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	return &br->bstats;
}

static struct net_device_stats *br_dev_get_cstats(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	return &br->cstats;
}

static struct net_device_stats * br_dev_collect_stats(struct net_device *dev_p)
{
	BlogStats_t bStats;
	BlogStats_t * bStats_p;
	struct net_device_stats *dStats_p;
	struct net_device_stats *cStats_p;

	if ( dev_p == (struct net_device *)NULL )
		return (struct net_device_stats *)NULL;

	dStats_p = br_dev_get_stats(dev_p);
	cStats_p = br_dev_get_cstats(dev_p);
	bStats_p = br_dev_get_bstats(dev_p);

	memset(&bStats, 0, sizeof(BlogStats_t));

	blog_notify(FETCH_NETIF_STATS, (void*)dev_p,
				(uint32_t)&bStats, BLOG_PARAM2_NO_CLEAR);

	memcpy( cStats_p, dStats_p, sizeof(struct net_device_stats) );

#if defined(CONFIG_BCM_KF_EXTSTATS)    
	/* Handle packet count statistics */
	cStats_p->rx_packets += ( bStats.rx_packets + bStats_p->rx_packets );
	cStats_p->tx_packets += ( bStats.tx_packets + bStats_p->tx_packets );
	cStats_p->multicast  += ( bStats.multicast  + bStats_p->multicast );
	cStats_p->tx_multicast_packets += ( bStats.tx_multicast_packets + bStats_p->tx_multicast_packets );
	/* NOTE: There are no broadcast packets in BlogStats_t since the
       flowcache doesn't accelerate broadcast.  Thus, they aren't added here */

	/* set byte counts to 0 if the bstat packet counts are non 0 and the
		octet counts are 0 */
	/* Handle RX byte counts */
	if ( ((bStats.rx_bytes + bStats_p->rx_bytes) == 0) &&
		  ((bStats.rx_packets + bStats_p->rx_packets) > 0) )
	{
		cStats_p->rx_bytes = 0;
	}
	else
	{
		cStats_p->rx_bytes   += ( bStats.rx_bytes   + bStats_p->rx_bytes );
	}
	
	/* Handle TX byte counts */
	if ( ((bStats.tx_bytes + bStats_p->tx_bytes) == 0) &&
		  ((bStats.tx_packets + bStats_p->tx_packets) > 0) )
	{
		cStats_p->tx_bytes = 0;
	}
	else
	{
		cStats_p->tx_bytes   += ( bStats.tx_bytes   + bStats_p->tx_bytes );
	}

	/* Handle RX multicast byte counts */
	if ( ((bStats.rx_multicast_bytes + bStats_p->rx_multicast_bytes) == 0) &&
		 ((bStats.multicast + bStats_p->multicast) > 0) )
	{
		cStats_p->rx_multicast_bytes = 0;
	}
	else
	{
		cStats_p->rx_multicast_bytes   += ( bStats.rx_multicast_bytes   + bStats_p->rx_multicast_bytes );
	}

	/* Handle TX multicast byte counts */
	if ( ((bStats.tx_multicast_bytes + bStats_p->tx_multicast_bytes) == 0) &&
		 ((bStats.tx_multicast_packets + bStats_p->tx_multicast_packets) > 0) )
	{
		cStats_p->tx_multicast_bytes = 0;
	}
	else
	{
		cStats_p->tx_multicast_bytes   += ( bStats.tx_multicast_bytes   + bStats_p->tx_multicast_bytes );
	}  
	
#else
	cStats_p->rx_packets += ( bStats.rx_packets + bStats_p->rx_packets );
	cStats_p->tx_packets += ( bStats.tx_packets + bStats_p->tx_packets );

	/* set byte counts to 0 if the bstat packet counts are non 0 and the
		octet counts are 0 */
	if ( ((bStats.rx_bytes + bStats_p->rx_bytes) == 0) &&
		  ((bStats.rx_packets + bStats_p->rx_packets) > 0) )
	{
		cStats_p->rx_bytes = 0;
	}
	else
	{
		cStats_p->rx_bytes   += ( bStats.rx_bytes   + bStats_p->rx_bytes );
	}

	if ( ((bStats.tx_bytes + bStats_p->tx_bytes) == 0) &&
		  ((bStats.tx_packets + bStats_p->tx_packets) > 0) )
	{
		cStats_p->tx_bytes = 0;
	}
	else
	{
		cStats_p->tx_bytes   += ( bStats.tx_bytes   + bStats_p->tx_bytes );
	}
	cStats_p->multicast  += ( bStats.multicast  + bStats_p->multicast );
#endif
 
	return cStats_p;
}

static void br_dev_update_stats(struct net_device * dev_p, 
                                BlogStats_t * blogStats_p)
{
	BlogStats_t * bStats_p;

	if ( dev_p == (struct net_device *)NULL )
		return;

	bStats_p = br_dev_get_bstats(dev_p);

	bStats_p->rx_packets += blogStats_p->rx_packets;
	bStats_p->tx_packets += blogStats_p->tx_packets;
	bStats_p->rx_bytes   += blogStats_p->rx_bytes;
	bStats_p->tx_bytes   += blogStats_p->tx_bytes;
	bStats_p->multicast  += blogStats_p->multicast;

	return;
}

static void br_dev_clear_stats(struct net_device * dev_p)
{
	BlogStats_t * bStats_p;
	struct net_device_stats *dStats_p;
	struct net_device_stats *cStats_p;

	if ( dev_p == (struct net_device *)NULL )
		return;

	dStats_p = br_dev_get_stats(dev_p);
	cStats_p = br_dev_get_cstats(dev_p); 
	bStats_p = br_dev_get_bstats(dev_p);

	blog_notify(FETCH_NETIF_STATS, (void*)dev_p, 0, BLOG_PARAM2_DO_CLEAR);

	memset(bStats_p, 0, sizeof(BlogStats_t));
	memset(dStats_p, 0, sizeof(struct net_device_stats));
	memset(cStats_p, 0, sizeof(struct net_device_stats));

	return;
}
#endif /* CONFIG_BLOG */

/* net device transmit always called with BH disabled */
netdev_tx_t br_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	const unsigned char *dest = skb->data;
	struct net_bridge_fdb_entry *dst;
	struct net_bridge_mdb_entry *mdst;
	struct br_cpu_netstats *brstats = this_cpu_ptr(br->stats);
#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP))
	struct iphdr *pipmcast = NULL;
	struct igmphdr *pigmp = NULL;
#endif
#if (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))
	struct ipv6hdr *pipv6mcast = NULL;
	struct icmp6hdr *picmpv6 = NULL;
#endif

#ifdef CONFIG_BRIDGE_NETFILTER
	if (skb->nf_bridge && (skb->nf_bridge->mask & BRNF_BRIDGED_DNAT)) {
		br_nf_pre_routing_finish_bridge_slow(skb);
		return NETDEV_TX_OK;
	}
#endif

#if defined(CONFIG_BCM_KF_EXTSTATS) && defined(CONFIG_BLOG)
	blog_link(IF_DEVICE, blog_ptr(skb), (void*)dev, DIR_TX, skb->len);

	/* Gather general TX statistics */
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	/* Gather packet specific packet data using pkt_type calculations from the ethernet driver */
	switch (skb->pkt_type) {
	case PACKET_BROADCAST:
		dev->stats.tx_broadcast_packets++;
		break;

	case PACKET_MULTICAST:
		dev->stats.tx_multicast_packets++;
		dev->stats.tx_multicast_bytes += skb->len;
		break;
	}
#endif

	u64_stats_update_begin(&brstats->syncp);
	brstats->tx_packets++;
	brstats->tx_bytes += skb->len;
	u64_stats_update_end(&brstats->syncp);

	BR_INPUT_SKB_CB(skb)->brdev = dev;

	skb_reset_mac_header(skb);
	skb_pull(skb, ETH_HLEN);

	rcu_read_lock();

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	br_mld_get_ip_icmp_hdrs(skb, &pipv6mcast, &picmpv6, NULL);
	if (pipv6mcast != NULL) {
		if (br_mld_mc_forward(br, skb, 0, 1)) {
			/* skb consumed so exit */
			goto out;
		}
	}
	else
#endif
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
	br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, NULL);
	if ( pipmcast != NULL )
	{
		if (br_igmp_mc_forward(br, skb, 0, 1)) {
			/* skb consumed so exit */
			goto out;
		}
	}
#endif

	if (is_broadcast_ether_addr(dest))
		br_flood_deliver(br, skb);
	else if (is_multicast_ether_addr(dest)) {
		if (unlikely(netpoll_tx_running(dev))) {
			br_flood_deliver(br, skb);
			goto out;
		}
// TODO: 新内核已经支持了IGMP/MLD snooping功能，该功能和平台的IGMP功能需要二选一
#ifndef CONFIG_ATP_COMMON
		if (br_multicast_rcv(br, NULL, skb)) {
			kfree_skb(skb);
			goto out;
		}

		mdst = br_mdb_get(br, skb);
		if (mdst || BR_INPUT_SKB_CB_MROUTERS_ONLY(skb))
			br_multicast_deliver(mdst, skb);
		else
			br_flood_deliver(br, skb);
#else
#ifdef CONFIG_ATP_MLD_SNOOPING
		if ((0x33 == dest[0]) && (0x33 == dest[1]))
		{
			if (!br_mld_snooping_forward(skb, br, (unsigned char *) dest, 0))
			{
				br_flood_deliver(br, skb);
			}
		}
		else
		{
			br_flood_deliver(br, skb);
		}
#elif defined(CONFIG_IGMP_SNOOPING)
		if (!br_igmp_snooping_forward(skb, br, (unsigned char *)dest, 0))
		{
			br_flood_deliver(br, skb);
		}
#else
		br_flood_deliver(br, skb);
#endif

#endif
	} else if ((dst = __br_fdb_get(br, dest)) != NULL)
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	{
		blog_link(BRIDGEFDB, blog_ptr(skb), (void*)dst, BLOG_PARAM1_DSTFDB, 0);
#if defined(CONFIG_BCM_KF_WL)
#if defined(PKTC)
		if (wl_pktc_req_hook && 
			(BLOG_GET_PHYTYPE(dst->dst->dev->path.hw_port_type) == BLOG_WLANPHY) && 
			wl_pktc_req_hook(GET_PKTC_TX_MODE, 0, 0, 0))
		{
			struct net_device *dst_dev_p = dst->dst->dev;
			uint32_t chainIdx = wl_pktc_req_hook(UPDATE_BRC_HOT, (uint32_t)&(dst->addr.addr[0]), (uint32_t)dst_dev_p, 0);
			if (chainIdx != INVALID_CHAIN_IDX)
			{
				// Update chainIdx in blog
				if (skb->blog_p != NULL)
				{
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM96838)
                    uint8_t wlan_queue = 0;

                    skb->blog_p->wfd_queue = ((chainIdx & WFD_IDX_UINT16_BIT_MASK) >> WFD_IDX_UINT16_BIT_POS);
                    wlan_queue = GET_WLAN_PRIORITY(skb->blog_p->mark);
                    chainIdx |= ((wlan_queue & 0x0f) << 8);
#endif
                    skb->blog_p->wl_metadata = chainIdx | WLAN_SKB_DRV_CHECK_MASK;
				    //printk("%s: Added ChainEntryIdx 0x%x Dev %s blogSrcAddr 0x%x blogDstAddr 0x%x DstMac %x:%x:%x:%x:%x:%x "
                    //       "wfd_q %d wl_metadata %d wl 0x%x\n", __FUNCTION__,
					//         chainIdx, dst->dst->dev->name, skb->blog_p->rx.tuple.saddr, skb->blog_p->rx.tuple.daddr, 
                    //         dst->addr.addr[0], dst->addr.addr[1], dst->addr.addr[2], dst->addr.addr[3], dst->addr.addr[4], 
                    //         dst->addr.addr[5], skb->blog_p->wfd_queue, skb->blog_p->wl_metadata, skb->blog_p->wl);
				}
			}
		}
#endif
#endif
		br_deliver(dst->dst, skb);
	}        
#else
		br_deliver(dst->dst, skb);
#endif
	else
		br_flood_deliver(br, skb);

out:
	rcu_read_unlock();
	return NETDEV_TX_OK;
}

static int br_dev_init(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	br->stats = alloc_percpu(struct br_cpu_netstats);
	if (!br->stats)
		return -ENOMEM;

	return 0;
}

static int br_dev_open(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	netdev_update_features(dev);
	netif_start_queue(dev);
	br_stp_enable_bridge(br);
	br_multicast_open(br);

	return 0;
}

static void br_dev_set_multicast_list(struct net_device *dev)
{
}

static int br_dev_stop(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	br_stp_disable_bridge(br);
	br_multicast_stop(br);

	netif_stop_queue(dev);

	return 0;
}

#if !(defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG))
static struct rtnl_link_stats64 *br_get_stats64(struct net_device *dev,
						struct rtnl_link_stats64 *stats)
{
	struct net_bridge *br = netdev_priv(dev);
	struct br_cpu_netstats tmp, sum = { 0 };
	unsigned int cpu;

	for_each_possible_cpu(cpu) {
		unsigned int start;
		const struct br_cpu_netstats *bstats
			= per_cpu_ptr(br->stats, cpu);
		do {
			start = u64_stats_fetch_begin(&bstats->syncp);
			memcpy(&tmp, bstats, sizeof(tmp));
		} while (u64_stats_fetch_retry(&bstats->syncp, start));
		sum.tx_bytes   += tmp.tx_bytes;
		sum.tx_packets += tmp.tx_packets;
		sum.rx_bytes   += tmp.rx_bytes;
		sum.rx_packets += tmp.rx_packets;
	}

	stats->tx_bytes   = sum.tx_bytes;
	stats->tx_packets = sum.tx_packets;
	stats->rx_bytes   = sum.rx_bytes;
	stats->rx_packets = sum.rx_packets;

	return stats;
}
#endif

static int br_change_mtu(struct net_device *dev, int new_mtu)
{
	struct net_bridge *br = netdev_priv(dev);
	if (new_mtu < 68 || new_mtu > br_min_mtu(br))
		return -EINVAL;

	dev->mtu = new_mtu;

#ifdef CONFIG_BRIDGE_NETFILTER
	/* remember the MTU in the rtable for PMTU */
	dst_metric_set(&br->fake_rtable.dst, RTAX_MTU, new_mtu);
#endif

	return 0;
}

/* Allow setting mac address to any valid ethernet address. */
static int br_set_mac_address(struct net_device *dev, void *p)
{
	struct net_bridge *br = netdev_priv(dev);
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	spin_lock_bh(&br->lock);
	if (compare_ether_addr(dev->dev_addr, addr->sa_data)) {
		dev->addr_assign_type &= ~NET_ADDR_RANDOM;
		memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
		br_fdb_change_mac_address(br, addr->sa_data);
		br_stp_change_bridge_id(br, addr->sa_data);
	}
	br->flags |= BR_SET_MAC_ADDR;
	spin_unlock_bh(&br->lock);

	return 0;
}

static void br_getinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strcpy(info->driver, "bridge");
	strcpy(info->version, BR_VERSION);
	strcpy(info->fw_version, "N/A");
	strcpy(info->bus_info, "N/A");
}

static netdev_features_t br_fix_features(struct net_device *dev,
	netdev_features_t features)
{
	struct net_bridge *br = netdev_priv(dev);

	return br_features_recompute(br, features);
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void br_poll_controller(struct net_device *br_dev)
{
}

static void br_netpoll_cleanup(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *p, *n;

	list_for_each_entry_safe(p, n, &br->port_list, list) {
		br_netpoll_disable(p);
	}
}

static int br_netpoll_setup(struct net_device *dev, struct netpoll_info *ni)
{
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *p, *n;
	int err = 0;

	list_for_each_entry_safe(p, n, &br->port_list, list) {
		if (!p->dev)
			continue;

		err = br_netpoll_enable(p);
		if (err)
			goto fail;
	}

out:
	return err;

fail:
	br_netpoll_cleanup(dev);
	goto out;
}

int br_netpoll_enable(struct net_bridge_port *p)
{
	struct netpoll *np;
	int err = 0;

	np = kzalloc(sizeof(*p->np), GFP_KERNEL);
	err = -ENOMEM;
	if (!np)
		goto out;

	np->dev = p->dev;
	strlcpy(np->dev_name, p->dev->name, IFNAMSIZ);

	err = __netpoll_setup(np);
	if (err) {
		kfree(np);
		goto out;
	}

	p->np = np;

out:
	return err;
}

void br_netpoll_disable(struct net_bridge_port *p)
{
	struct netpoll *np = p->np;

	if (!np)
		return;

	p->np = NULL;

	/* Wait for transmitting packets to finish before freeing. */
	synchronize_rcu_bh();

	__netpoll_cleanup(np);
	kfree(np);
}

#endif

static int br_add_slave(struct net_device *dev, struct net_device *slave_dev)

{
	struct net_bridge *br = netdev_priv(dev);

	return br_add_if(br, slave_dev);
}

static int br_del_slave(struct net_device *dev, struct net_device *slave_dev)
{
	struct net_bridge *br = netdev_priv(dev);

	return br_del_if(br, slave_dev);
}

static const struct ethtool_ops br_ethtool_ops = {
	.get_drvinfo    = br_getinfo,
	.get_link	= ethtool_op_get_link,
};

static const struct net_device_ops br_netdev_ops = {
	.ndo_open		 = br_dev_open,
	.ndo_stop		 = br_dev_stop,
	.ndo_init		 = br_dev_init,
	.ndo_start_xmit		 = br_dev_xmit,
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	.ndo_get_stats		 = br_dev_collect_stats,
#else
	.ndo_get_stats64	 = br_get_stats64,
#endif
	.ndo_set_mac_address	 = br_set_mac_address,
	.ndo_set_rx_mode	 = br_dev_set_multicast_list,
	.ndo_change_mtu		 = br_change_mtu,
	.ndo_do_ioctl		 = br_dev_ioctl,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_netpoll_setup	 = br_netpoll_setup,
	.ndo_netpoll_cleanup	 = br_netpoll_cleanup,
	.ndo_poll_controller	 = br_poll_controller,
#endif
	.ndo_add_slave		 = br_add_slave,
	.ndo_del_slave		 = br_del_slave,
	.ndo_fix_features        = br_fix_features,
};

static void br_dev_free(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	free_percpu(br->stats);
	free_netdev(dev);
}

static struct device_type br_type = {
	.name	= "bridge",
};

void br_dev_setup(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	eth_hw_addr_random(dev);
	ether_setup(dev);

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	dev->put_stats = br_dev_update_stats;
	dev->clr_stats = br_dev_clear_stats;
#endif

	dev->netdev_ops = &br_netdev_ops;
	dev->destructor = br_dev_free;
	SET_ETHTOOL_OPS(dev, &br_ethtool_ops);
	SET_NETDEV_DEVTYPE(dev, &br_type);
	dev->tx_queue_len = 0;
	dev->priv_flags = IFF_EBRIDGE;

	dev->features = NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA |
			NETIF_F_GSO_MASK | NETIF_F_HW_CSUM | NETIF_F_LLTX |
			NETIF_F_NETNS_LOCAL | NETIF_F_HW_VLAN_TX;
	dev->hw_features = NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA |
			   NETIF_F_GSO_MASK | NETIF_F_HW_CSUM |
			   NETIF_F_HW_VLAN_TX;

	br->dev = dev;
	spin_lock_init(&br->lock);
	INIT_LIST_HEAD(&br->port_list);
	spin_lock_init(&br->hash_lock);

	br->bridge_id.prio[0] = 0x80;
	br->bridge_id.prio[1] = 0x00;

	memcpy(br->group_addr, br_group_address, ETH_ALEN);

	br->stp_enabled = BR_NO_STP;
	br->group_fwd_mask = BR_GROUPFWD_DEFAULT;

	br->designated_root = br->bridge_id;
	br->bridge_max_age = br->max_age = 20 * HZ;
	br->bridge_hello_time = br->hello_time = 2 * HZ;
	br->bridge_forward_delay = br->forward_delay = 15 * HZ;
	br->ageing_time = 300 * HZ;

	br_netfilter_rtable_init(br);
	br_stp_timer_init(br);
	br_multicast_init(br);

#if defined(CONFIG_BCM_KF_NETFILTER)
	br->num_fdb_entries = 0;
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
	br->max_br_fdb_entries = BR_MAX_FDB_ENTRIES;
	br->used_br_fdb_entries = 0;
#endif

#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
	br_igmp_snooping_br_init(br);
#endif

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	br_mld_snooping_br_init(br);
#endif
}
