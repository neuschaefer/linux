/*
*    Copyright (c) 2012 Broadcom Corporation
*    All Rights Reserved
* 
<:label-BRCM:2012:DUAL/GPL:standard

Unless you and Broadcom execute a separate written software license
agreement governing use of this software, this software is licensed
to you under the terms of the GNU General Public License version 2
(the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
with the following added to such license:

   As a special exception, the copyright holders of this software give
   you permission to link this software with independent modules, and
   to copy and distribute the resulting executable under terms of your
   choice, provided that you also meet, for each linked independent
   module, the terms and conditions of the license of that module.
   An independent module is a module which is not derived from this
   software.  The special exception does not apply to any modifications
   of the software.

Not withstanding the above, under no circumstances may you combine
this software in any way with any other Broadcom software provided
under a license other than the GPL, without Broadcom's express prior
written consent.

:>
*/

#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <asm/atomic.h>
#include <linux/ip.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/if_vlan.h>
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif
#include <linux/rtnetlink.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
#include "br_igmp.h"
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
#include "br_mld.h"
#include <linux/module.h>
#endif
#include <linux/bcm_skb_defines.h>

#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))
static t_MCAST_CFG multiConfig = { -1, /* mcastPriQueue */
                                  0   /* thereIsAnUplink */
                                 };

void br_mcast_set_pri_queue(int val)
{
   multiConfig.mcastPriQueue = val;
}

int br_mcast_get_pri_queue(void)
{
   return multiConfig.mcastPriQueue;
}

void br_mcast_set_skb_mark_queue(struct sk_buff *skb)
{
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
   struct iphdr        *pipmcast = NULL;
   struct igmphdr      *pigmp = NULL;
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
   struct ipv6hdr      *pipv6mcast = NULL;
   struct icmp6hdr     *picmpv6 = NULL;
#endif
   int                  isMulticast = 0;
   const unsigned char *dest = eth_hdr(skb)->h_dest;

   if ( multiConfig.mcastPriQueue == -1 )
   {
      return;
   }

   if (unlikely(skb->pkt_type == PACKET_LOOPBACK) || 
       unlikely(skb->pkt_type == PACKET_HOST))
   {
      return;
   }

   if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
   {
      return;
   }

   /* look for L2 mcast packet */
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
   if((BR_MLD_MULTICAST_MAC_PREFIX == dest[0]) && 
      (BR_MLD_MULTICAST_MAC_PREFIX == dest[1])) {
      isMulticast = 1;
   }
   else
#endif
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
   if ( is_multicast_ether_addr(dest) && !is_broadcast_ether_addr(dest) )
   {
      isMulticast = 1;
   }
   else
#endif

   /* check for IP mcast header */
   if ( 0 == isMulticast )
   {
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
      br_mld_get_ip_icmp_hdrs(skb, &pipv6mcast, &picmpv6, NULL);
      if (pipv6mcast != NULL)
      {
         isMulticast = 1;
      }
      else
#endif
      {
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
         br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, NULL);
         if ( pipmcast != NULL )
         {
            isMulticast = 1;
         }
#endif
      }
   }
  
   if ( isMulticast )
   {
      skb->mark = SKBMARK_SET_Q(skb->mark, multiConfig.mcastPriQueue);
   }
}

void br_mcast_set_uplink_exists(int uplinkExists)
{
   multiConfig.thereIsAnUplink = uplinkExists;
}

int br_mcast_get_lan2lan_snooping(t_BR_MCAST_PROTO_TYPE proto, struct net_bridge *br)
{
   if (!multiConfig.thereIsAnUplink)
      {
      return BR_MC_LAN2LAN_STATUS_ENABLE;
      }
#if defined(CONFIG_BR_MLD_SNOOP)
   if ( BR_MCAST_PROTO_MLD == proto )
      {
         return br->mld_lan2lan_mc_enable;
      }
   else
#endif
   {
         return br->igmp_lan2lan_mc_enable;
      }
   }

static void br_mcast_mc_fdb_update_bydev(t_BR_MCAST_PROTO_TYPE proto, 
                                         struct net_bridge    *br,
                                         struct net_device    *dev,
                                         unsigned int          flushAll)
{
    if(!br || !dev)
        return;

    switch ( proto ) {
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
        case BR_MCAST_PROTO_IGMP:
            br_igmp_mc_fdb_update_bydev(br, dev, flushAll);
            br_igmp_process_device_removal (dev);
            break;
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
        case BR_MCAST_PROTO_MLD:
            br_mld_mc_fdb_update_bydev(br, dev, flushAll);
            //br_mld_process_device_removal (dev);
            break;
#endif
        default:
            break;
    }
    return;
}

void br_mcast_handle_netdevice_events(struct net_device *ndev, unsigned long event)
{
    struct net_bridge *br = NULL;
    struct net_device *brDev = NULL;

    switch (event) {
        case NETDEV_DOWN:
        case NETDEV_GOING_DOWN:
        case NETDEV_CHANGE:
            rcu_read_lock();
            for_each_netdev_rcu(&init_net, brDev) {
                br = netdev_priv(brDev);
                if(brDev->priv_flags & IFF_EBRIDGE)
                {
                    /* snooping entries could be present even if snooping is
                       disabled, update existing entries */
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
                    br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_IGMP, br, ndev, 1);
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
                    br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_MLD, br, ndev, 1);
#endif
                }
            }
            rcu_read_unlock();
            break;
    }

    return;
}
#endif

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
inline void br_mcast_ipv4_to_eth(unsigned long ipv4_addr,
                                       unsigned char *mac_addr_p)
{
    unsigned char *ipv4_addr8_p = (unsigned char *)(&ipv4_addr);

    mac_addr_p[0] = 0x01;
    mac_addr_p[1] = 0x00;
    mac_addr_p[2] = 0x5E;
    mac_addr_p[3] = ipv4_addr8_p[1] & 0x7F;
    mac_addr_p[4] = ipv4_addr8_p[2];
    mac_addr_p[5] = ipv4_addr8_p[3];
}

inline void br_mcast_ipv6_to_eth(unsigned char *ipv6_addr,
                                       unsigned char *mac_addr_p)
{
    mac_addr_p[0] = 0x33;
    mac_addr_p[1] = 0x33;
    mac_addr_p[2] = ipv6_addr[12];
    mac_addr_p[3] = ipv6_addr[13];
    mac_addr_p[4] = ipv6_addr[14];
    mac_addr_p[5] = ipv6_addr[15];
}

void br_mcast_blog_release(t_BR_MCAST_PROTO_TYPE proto, void *mc_fdb)
{
	Blog_t *blog_p = BLOG_NULL;
	uint32_t blog_idx = BLOG_KEY_INVALID;
	BlogTraffic_t traffic;

#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
	if(proto == BR_MCAST_PROTO_IGMP)
	{
		blog_idx =  ((struct net_bridge_mc_fdb_entry *)mc_fdb)->blog_idx;
		((struct net_bridge_mc_fdb_entry *)mc_fdb)->blog_idx = 0;
		traffic = BlogTraffic_IPV4_MCAST;
	}
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	if(proto == BR_MCAST_PROTO_MLD)
	{
		blog_idx =  ((struct net_br_mld_mc_fdb_entry *)mc_fdb)->blog_idx;
		((struct net_br_mld_mc_fdb_entry *)mc_fdb)->blog_idx = 0;
		traffic = BlogTraffic_IPV6_MCAST;
	}
#endif

	if(BLOG_KEY_INVALID == blog_idx)
		return;

	blog_p = blog_deactivate(blog_idx, traffic, BlogClient_fcache);
#if defined(CONFIG_BCM_KF_RUNNER) && (defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE))
	/* When Runner is used, we always will use the Runner's IPTV table,
	 * and won't use software (flow-cache) acceleration */
		hw_accelerator_client_get();
#else /* CONFIG_BCM_KF_RUNNER && (CONFIG_BCM_RDPA) || (CONFIG_BCM_RDPA_MODULE) */
		sw_accelerator_client_get();
#endif /* CONFIG_BCM_KF_RUNNER && (CONFIG_BCM_RDPA) || (CONFIG_BCM_RDPA_MODULE) */
	if ( blog_p )
	{
		blog_rule_free_list(blog_p);
		blog_put(blog_p);
	}

	return;
}

static void br_mcast_blog_process_wan(blogRule_t *rule_p,
                                     void *mc_fdb,
                                     t_BR_MCAST_PROTO_TYPE proto,
                                     struct net_device **wan_dev_pp,
                                     struct net_device **wan_vlan_dev_pp)
{
	blogRuleAction_t ruleAction;
	struct net_device *dev_p = NULL;
	struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif 
	uint8_t                *dev_addr = NULL;
	uint32_t phyType;
	char wan_ops;

	if(!mc_fdb)
		return;

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	if(BR_MCAST_PROTO_MLD == proto)
	{
		mld_fdb  = (struct net_br_mld_mc_fdb_entry *)mc_fdb;
		dev_p    = mld_fdb->from_dev;
		dev_addr = mld_fdb->dst->dev->dev_addr;
		wan_ops  = mld_fdb->type;
	}
	else
#endif
	{
		igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;
		dev_p    = igmp_fdb->from_dev;
		dev_addr = igmp_fdb->dst->dev->dev_addr;
		wan_ops  = igmp_fdb->type;
	}

	while(1)
	{
		if(netdev_path_is_root(dev_p))
		{
			*wan_dev_pp = dev_p;
			break;
		}

		if(dev_p->priv_flags & IFF_PPP)
		{
			rule_p->filter.hasPppoeHeader = 1;
			memset(&ruleAction, 0, sizeof(blogRuleAction_t));
			ruleAction.cmd = BLOG_RULE_CMD_POP_PPPOE_HDR;
			blog_rule_add_action(rule_p, &ruleAction);

			memset(&ruleAction, 0, sizeof(blogRuleAction_t));
			ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_DA;
			if(igmp_fdb)
				br_mcast_ipv4_to_eth(igmp_fdb->txGrp.s_addr, ruleAction.macAddr);
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
			else
				br_mcast_ipv6_to_eth(mld_fdb->grp.s6_addr, ruleAction.macAddr);
#endif
			blog_rule_add_action(rule_p, &ruleAction);
		}
		else if(*wan_vlan_dev_pp == NULL &&
		        dev_p->priv_flags & IFF_BCM_VLAN)
		{
			*wan_vlan_dev_pp = dev_p;
		}
		dev_p = netdev_path_next_dev(dev_p);
	}

	/* For IPoA */
	phyType = netdev_path_get_hw_port_type(*wan_dev_pp);
	phyType = BLOG_GET_HW_ACT(phyType);
	if((phyType == VC_MUX_IPOA) || (phyType == LLC_SNAP_ROUTE_IP))
	{
		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_DA;
		if(igmp_fdb)
			br_mcast_ipv4_to_eth(igmp_fdb->txGrp.s_addr, ruleAction.macAddr);
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
		else
			br_mcast_ipv6_to_eth(mld_fdb->grp.s6_addr, ruleAction.macAddr);
#endif
		blog_rule_add_action(rule_p, &ruleAction);
	}

	if(wan_ops == MCPD_IF_TYPE_ROUTED)
	{
		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_SET_MAC_SA;
		memcpy(ruleAction.macAddr, dev_addr, ETH_ALEN);
		blog_rule_add_action(rule_p, &ruleAction);

		memset(&ruleAction, 0, sizeof(blogRuleAction_t));
		ruleAction.cmd = BLOG_RULE_CMD_DECR_TTL;
		blog_rule_add_action(rule_p, &ruleAction);
	}
}

static void br_mcast_blog_process_lan(void *mc_fdb,
                                     t_BR_MCAST_PROTO_TYPE proto,
                                     struct net_device **lan_dev_pp,
                                     struct net_device **lan_vlan_dev_pp)
{
    struct net_device *dev_p = NULL;
    struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif

    if(!mc_fdb)
        return;

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    if (BR_MCAST_PROTO_MLD == proto )
    {
        mld_fdb = (struct net_br_mld_mc_fdb_entry *)mc_fdb;
        dev_p = mld_fdb->dst->dev;
    }
    else
#endif
    {
        igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;
        dev_p = igmp_fdb->dst->dev;
    }

    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            *lan_dev_pp = dev_p;
            break;
        }

        if(*lan_vlan_dev_pp == NULL &&
           dev_p->priv_flags & IFF_BCM_VLAN)
        {
            *lan_vlan_dev_pp = dev_p;
        }

        dev_p = netdev_path_next_dev(dev_p);
    }
}

void br_mcast_vlan_notify_for_blog_update(struct net_device *ndev,
                                          blogRuleVlanNotifyDirection_t direction,
                                          uint32_t nbrOfTags)
{
	struct net_bridge *br = NULL;
	struct net_device *dev = NULL;

#if defined(CONFIG_BCM_KF_WANDEV)
	if((ndev->priv_flags & IFF_WANDEV) && (direction == BLOG_RULE_VLAN_NOTIFY_DIR_TX))
		return;
#endif

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev) {
		br = netdev_priv(dev);
		if(dev->priv_flags & IFF_EBRIDGE)
		{
			/* snooping entries could be present even if snooping is
			   disabled, update existing entries */
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
			br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_IGMP, br, ndev, 0);
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
			br_mcast_mc_fdb_update_bydev(BR_MCAST_PROTO_MLD, br, ndev, 0);
#endif
		}
	}
	rcu_read_unlock();

	return;
}

static void *br_mcast_mc_fdb_copy(t_BR_MCAST_PROTO_TYPE proto,
								struct net_bridge *br, 
                                const void *mc_fdb)
{
    if(!mc_fdb)
        return NULL;

    if(BR_MCAST_PROTO_IGMP == proto)
        return br_igmp_mc_fdb_copy(br, (struct net_bridge_mc_fdb_entry *)mc_fdb);
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    else if(BR_MCAST_PROTO_MLD == proto)
        return br_mld_mc_fdb_copy(br, (struct net_br_mld_mc_fdb_entry *)mc_fdb);
#endif

	return NULL;
}

static void br_mcast_mc_fdb_del_entry(t_BR_MCAST_PROTO_TYPE proto, 
                               struct net_bridge *br, 
                               void *mc_fdb)
{

    if(!mc_fdb)
        return;

    if(BR_MCAST_PROTO_IGMP == proto)
        br_igmp_mc_fdb_del_entry(br, (struct net_bridge_mc_fdb_entry *)mc_fdb, NULL);
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    else if(BR_MCAST_PROTO_MLD == proto)
        br_mld_mc_fdb_del_entry(br, (struct net_br_mld_mc_fdb_entry *)mc_fdb, NULL);
#endif

    return;
} /* br_mcast_mc_fdb_del_entry */

static void br_mcast_blog_link_devices(Blog_t *blog_p, struct net_device *rxDev, 
                                       struct net_device *txDev, int wanType )
{
    struct net_device *dev_p;
    uint32_t delta;
    struct net_device *rxPath[MAX_VIRT_DEV];
    struct net_bridge_port *port;
    int rxPathIdx = 0;
    int i;

    /* save rx path required for reverse path traversal for delta calc */
    memset(&rxPath[0], 0, (MAX_VIRT_DEV * sizeof(struct net_device *)));
    dev_p = rxDev;
    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            break;
        }
        rxPath[rxPathIdx] = dev_p;
        rxPathIdx++;
        dev_p = netdev_path_next_dev(dev_p);
    }

    /* omit Ethernet header from virtual dev RX stats */
    delta = BLOG_ETH_HDR_LEN;

    for(i = (MAX_VIRT_DEV-1); i >= 0; i--)
    {
        if(NULL == rxPath[i])
        {
            continue;
        }

        if ( rxPath[i]->priv_flags & IFF_PPP )
        {
            delta += BLOG_PPPOE_HDR_LEN;
        }

        if ( rxPath[i]->priv_flags & IFF_802_1Q_VLAN )
        {
            delta += BLOG_VLAN_HDR_LEN;
        }

        if ( (rxPath[i]->priv_flags & IFF_BCM_VLAN) && 
             (blog_p->vtag_num > 0) )
        {
            delta += BLOG_VLAN_HDR_LEN;
        }

        blog_link(IF_DEVICE_MCAST, blog_p, rxPath[i], DIR_RX, delta);
        dev_p = netdev_path_next_dev(dev_p);
    }

    /* include Ethernet header in virtual TX stats */
    delta -= BLOG_ETH_HDR_LEN;

    if ( (wanType == MCPD_IF_TYPE_ROUTED) && br_port_get_rcu(txDev) )
    {
       /* routed packets will come through br_dev_xmit, link bridge
          device with blog */
       port = br_port_get_rcu(txDev);
       blog_link(IF_DEVICE_MCAST, blog_p, port->br->dev, 
                 DIR_TX, delta );
    }

    dev_p = txDev;
    while(1)
    {
        if(netdev_path_is_root(dev_p))
        {
            break;
        }

        if ( dev_p->priv_flags & IFF_802_1Q_VLAN )
        {
            delta -= BLOG_VLAN_HDR_LEN;
        }

        if ( dev_p->priv_flags & IFF_BCM_VLAN )
        {
            delta -= BLOG_VLAN_HDR_LEN;
        }

        blog_link(IF_DEVICE_MCAST, blog_p, dev_p, DIR_TX, delta);
        dev_p = netdev_path_next_dev(dev_p);
    }
}


static int br_mcast_vlan_process(struct net_bridge     *br,
                           void                  *mc_fdb,
                           t_BR_MCAST_PROTO_TYPE  proto,
                           Blog_t                *blog_p)
{
    Blog_t           *new_blog_p;
    void             *new_mc_fdb = NULL;
    blogRule_t       *rule_p = NULL;
    int               firstRule = 1;
    uint32_t          vid = 0;
    blogRuleFilter_t *rule_filter = NULL;
    BlogTraffic_t     traffic;
    int               activates = 0;
    void             *rxDev;
    void             *txDev;
    int               wanType;
    uint32_t          blog_key;

    if(!mc_fdb || !blog_p || !br)
        return 0;

    if(!((BR_MCAST_PROTO_IGMP == proto) || (BR_MCAST_PROTO_MLD == proto)))
        return 0;

    firstRule = 1;
    rule_p = (blogRule_t *)blog_p->blogRule_p;
    while( rule_p )
    {
        blogRuleFilter_t *filter_p;

        filter_p = &rule_p->filter;
        /* if there is a rule that specifies a protocol filter that does not match
           blog key protocol skip it */
        if(blog_rule_filterInUse(filter_p->ipv4.mask.ip_proto))
        {
            if(filter_p->ipv4.mask.ip_proto & BLOG_RULE_IP_PROTO_MASK)
            {
                uint8_t proto;

                proto = filter_p->ipv4.value.ip_proto >> BLOG_RULE_IP_PROTO_SHIFT;
                if (proto != blog_p->key.protocol)
                {
                    /* skip this rule */
                    blog_p->blogRule_p = rule_p->next_p;
                    blog_rule_free(rule_p);
                    rule_p = blog_p->blogRule_p;
                    continue;
                }
            }
        }

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
        if(blog_rule_filterInUse(filter_p->ipv6.mask.nxtHdr))
        {
            if(filter_p->ipv6.mask.nxtHdr & BLOG_RULE_IP6_NXT_HDR_MASK)
            {
                uint8_t nxtHdr;

                nxtHdr = filter_p->ipv6.value.nxtHdr >> BLOG_RULE_IP6_NXT_HDR_SHIFT;
                if (nxtHdr != blog_p->key.protocol)
                {
                    /* skip this rule */
                    blog_p->blogRule_p = rule_p->next_p;
                    blog_rule_free(rule_p);
                    rule_p = blog_p->blogRule_p;
                    continue;
                }
            }
        }
#endif

        /* create new fdb entry unless this is the first rule. For the
           first rule use the fdb entry that was passed in */
        if ( 1 == firstRule )
        {
            firstRule  = 0;
            new_mc_fdb = mc_fdb;
        }
        else
        {
            new_mc_fdb = br_mcast_mc_fdb_copy(proto, br , mc_fdb);
            if(!new_mc_fdb)
            {
                printk(KERN_WARNING "%s new_mc_fdb allocation failed\n",__FUNCTION__);
                break;
            }
        }

        /* get a new blog and copy original blog */
        new_blog_p = blog_get();
        if (new_blog_p == BLOG_NULL) 
        {
            if (new_mc_fdb != mc_fdb) 
            {
                br_mcast_mc_fdb_del_entry(proto, br, new_mc_fdb);
            }
            break;
        }
        blog_copy(new_blog_p, blog_p);

        /* pop the rule off the original blog now that a new fdb and blog have been
           allocated. This is to ensure that all rules are freed in case of error */
        blog_p->blogRule_p = rule_p->next_p;
        rule_p->next_p = NULL;
            new_blog_p->blogRule_p = rule_p;

        rule_filter = &(((blogRule_t *)new_blog_p->blogRule_p)->filter);
        new_blog_p->vtag_num = rule_filter->nbrOfVlanTags;
        vid = ((rule_filter->vlan[0].value.h_vlan_TCI &
                rule_filter->vlan[0].mask.h_vlan_TCI) & 0xFFF);
        new_blog_p->vid = vid ? vid : 0xFFFF; 
        vid = ((rule_filter->vlan[1].value.h_vlan_TCI &
                rule_filter->vlan[1].mask.h_vlan_TCI) & 0xFFF);
        new_blog_p->vid |= vid ? (vid << 16) : 0xFFFF0000;

        blog_link(MCAST_FDB, new_blog_p, (void *)new_mc_fdb, 0, 0);

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
        if(BR_MCAST_PROTO_MLD == proto)
        {
            traffic = BlogTraffic_IPV6_MCAST;
            ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->wan_tci = new_blog_p->vid;
            ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->num_tags = new_blog_p->vtag_num;
            rxDev   = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->from_dev;
            txDev   = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->dst->dev;
            wanType = ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->type;
        }
        else//IGMP
#endif
        {
            traffic = BlogTraffic_IPV4_MCAST;
            ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->wan_tci = new_blog_p->vid;
            ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->num_tags = new_blog_p->vtag_num;
            rxDev   = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->from_dev;
            txDev   = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->dst->dev;
            wanType = ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->type;
        }

        br_mcast_blog_link_devices(new_blog_p, rxDev, txDev, wanType);

        blog_key = blog_activate(new_blog_p, traffic, BlogClient_fcache);
        if ( blog_key == BLOG_KEY_INVALID )
        {
            blog_rule_free_list(new_blog_p);
            blog_put(new_blog_p);
            if ( new_mc_fdb != mc_fdb )
            {
               br_mcast_mc_fdb_del_entry(proto, br, new_mc_fdb);
            }
        }
        else
        {
#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
            if(BR_MCAST_PROTO_MLD == proto)
            {
                ((struct net_br_mld_mc_fdb_entry *)new_mc_fdb)->blog_idx = blog_key;
            }
            else
#endif
            {
                ((struct net_bridge_mc_fdb_entry *)new_mc_fdb)->blog_idx = blog_key;
            }
#endif /* (CONFIG_BCM_RDPA) || (CONFIG_BCM_RDPA_MODULE) */
#endif /* CONFIG_BCM_KF_RUNNER */
            activates++;
        }

        /* advance to the next rule */
        rule_p = blog_p->blogRule_p;
    }

    /* Free blog. The blog will only have rules if there was an error */
    blog_rule_free_list(blog_p);
    blog_put(blog_p);

    return activates;
} /* br_mcast_vlan_process */


int br_mcast_blog_process(struct net_bridge *br,
                            void            *mc_fdb,
                            t_BR_MCAST_PROTO_TYPE proto)
{
	Blog_t *blog_p = BLOG_NULL;
	blogRule_t *rule_p = NULL;
	struct net_device *wan_vlan_dev_p = NULL;
	struct net_device *lan_vlan_dev_p = NULL;
	struct net_device *wan_dev_p = NULL;
	struct net_device *lan_dev_p = NULL;
	struct net_bridge_mc_fdb_entry *igmp_fdb = NULL;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	struct net_br_mld_mc_fdb_entry *mld_fdb = NULL;
#endif
	struct net_device *from_dev = NULL;
	uint32_t phyType;
   int numActivates;

	if(!mc_fdb)
		return -1;

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	if(BR_MCAST_PROTO_MLD == proto)
	{
		mld_fdb = (struct net_br_mld_mc_fdb_entry *)mc_fdb;
		from_dev = mld_fdb->from_dev;
	}
        else
#endif
	{
		igmp_fdb = (struct net_bridge_mc_fdb_entry *)mc_fdb;
		from_dev = igmp_fdb->from_dev;
	}

	/* allocate blog */
	blog_p = blog_get();
	if(blog_p == BLOG_NULL) {
		printk(KERN_WARNING "%s blog_p allocation failed\n",__FUNCTION__);
		return -1;
	}

	/* allocate blog rule */
	rule_p = blog_rule_alloc();
	if(rule_p == NULL)
	{
		printk(KERN_WARNING "%s blog_rule allocation failed\n",__FUNCTION__);
		blog_put(blog_p);
		return -1;
	}

	blog_rule_init(rule_p);
	blog_p->blogRule_p = (void *)rule_p;

	/* for LAN2LAN don't do anything */
	if(br->dev != from_dev) 
	{
		/* find WAN devices */
		br_mcast_blog_process_wan(rule_p, mc_fdb, proto,
		                          &wan_dev_p, &wan_vlan_dev_p);
	}

	/* find LAN devices */
	br_mcast_blog_process_lan(mc_fdb, proto, &lan_dev_p, &lan_vlan_dev_p);

	/* for LAN2LAN don't do anything */
	if(br->dev == from_dev) 
	{
		blog_p->rx.info.phyHdr = 0;
		blog_p->rx.info.channel = 0xFF; /* for lan2lan mcast */
		blog_p->rx.info.bmap.BCM_SWC = 1;
	}
	else
	{
		phyType = netdev_path_get_hw_port_type(wan_dev_p);
		blog_p->rx.info.phyHdrType = BLOG_GET_PHYTYPE(phyType);
		blog_p->rx.info.phyHdrLen = BLOG_GET_PHYLEN(phyType);
		phyType = BLOG_GET_HW_ACT(phyType);

		if(blog_p->rx.info.phyHdrType == BLOG_GPONPHY)
		{
			unsigned int hw_subport_mcast_idx;

			hw_subport_mcast_idx = netdev_path_get_hw_subport_mcast_idx(wan_dev_p);

			if(hw_subport_mcast_idx < CONFIG_BCM_MAX_GEM_PORTS)
			{
                blog_p->rx.info.channel = hw_subport_mcast_idx;
			}
			else
			{
				/* Not a GPON Multicast WAN device */
				blog_rule_free_list(blog_p);
				blog_put(blog_p);
				return -1;
			}
		}
		else /* Ethernet or DSL WAN device */
		{
			blog_p->rx.info.channel = netdev_path_get_hw_port(wan_dev_p);
		}

		if( (blog_p->rx.info.phyHdrType == BLOG_XTMPHY) &&
		    ((phyType == VC_MUX_PPPOA) ||
		     (phyType == VC_MUX_IPOA) ||
		     (phyType == LLC_SNAP_ROUTE_IP) ||
		     (phyType == LLC_ENCAPS_PPP)) )
		{
			blog_p->insert_eth = 1;
		}

		if( (blog_p->rx.info.phyHdrType == BLOG_XTMPHY) &&
		    ((phyType == VC_MUX_PPPOA) ||
		     (phyType == LLC_ENCAPS_PPP)) )
		{
			blog_p->pop_pppoa = 1;
		}

#if defined(CONFIG_BCM96816) || defined(CONFIG_BCM96818)
		blog_p->rx.info.bmap.BCM_SWC = 1;
#else
		if(blog_p->rx.info.phyHdrType == BLOG_ENETPHY)
		{
			blog_p->rx.info.bmap.BCM_SWC = 1;
		}
		else
		{
			blog_p->rx.info.bmap.BCM_XPHY = 1;
		}
#endif
	}

#if defined(CONFIG_MIPS_BRCM) && defined(CONFIG_BR_MLD_SNOOP)
	if ((mld_fdb) && (mld_fdb->lanppp)) {
		blog_p->has_pppoe = 1;
	}
	else
#endif
	if ((igmp_fdb) && (igmp_fdb->lanppp)) {
		blog_p->has_pppoe = 1;
	}

	blog_p->tx.info.bmap.BCM_SWC = 1;

	blog_p->key.l1_tuple.phy = blog_p->rx.info.phyHdr;
	blog_p->key.l1_tuple.channel = blog_p->rx.info.channel;
	blog_p->key.protocol = BLOG_IPPROTO_UDP;

	phyType = netdev_path_get_hw_port_type(lan_dev_p);
	blog_p->tx.info.phyHdrType = BLOG_GET_PHYTYPE(phyType);
	blog_p->tx.info.phyHdrLen = BLOG_GET_PHYLEN(phyType);
	blog_p->tx.info.channel = netdev_path_get_hw_port(lan_dev_p);

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
	if(BR_MCAST_PROTO_MLD == proto)
	{
		BCM_IN6_ASSIGN_ADDR(&blog_p->tupleV6.saddr, &mld_fdb->src_entry.src);
		BCM_IN6_ASSIGN_ADDR(&blog_p->tupleV6.daddr, &mld_fdb->grp);
		blog_p->rx.info.bmap.PLD_IPv6 = 1;
		blog_p->tx.info.bmap.PLD_IPv6 = 1;
	}
	else
#endif      
	{
		blog_p->rx.tuple.saddr = igmp_fdb->src_entry.src.s_addr;
		blog_p->rx.tuple.daddr = igmp_fdb->rxGrp.s_addr;
		blog_p->tx.tuple.saddr = igmp_fdb->src_entry.src.s_addr;
		blog_p->tx.tuple.daddr = igmp_fdb->txGrp.s_addr;
		blog_p->rx.tuple.port.dest = 0;
		if (igmp_fdb->excludePort != -1) {
			blog_p->rx.tuple.port.dest = igmp_fdb->excludePort;
		}
		blog_p->rtp_seq_chk = igmp_fdb->enRtpSeqCheck;
		blog_p->rx.info.bmap.PLD_IPv4 = 1;
		blog_p->tx.info.bmap.PLD_IPv4 = 1;
	}

	blog_p->rx.dev_p = wan_dev_p;
	blog_p->rx.multicast = 1;
	blog_p->tx.dev_p = lan_dev_p;

	if ( multiConfig.mcastPriQueue != -1 )
	{
		blog_p->mark = SKBMARK_SET_Q(blog_p->mark, multiConfig.mcastPriQueue);
	}

	/* add vlan blog rules, if any vlan interfaces were found */
	if(blogRuleVlanHook && (wan_vlan_dev_p || lan_vlan_dev_p)) {
		if(blogRuleVlanHook(blog_p, wan_vlan_dev_p, lan_vlan_dev_p) < 0) {
			printk(KERN_WARNING "Error while processing VLAN blog rules\n");
			blog_rule_free_list(blog_p);
			blog_put(blog_p);
			return -1;
		}
	}

	/* blog must have at least one rule */
	if (NULL == blog_p->blogRule_p)
	{
		/* blogRule_p == NULL may be valid if there are no 
		   VLAN rules and the default behavior for either interface is DROP */
//		printk(KERN_WARNING "Error while processing VLAN blog rules\n");
		blog_put(blog_p);
		return -1;
	}

	numActivates = br_mcast_vlan_process(br, mc_fdb, proto, blog_p);
	if ( 0 == numActivates )
	{
		return - 1;
	}

	return 0;
} /* br_mcast_blog_process */
#endif /* CONFIG_BLOG */

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
struct net_device *br_get_device_by_index(char *brname,char index) {
	struct net_bridge *br = NULL;
	struct net_bridge_port *br_port = NULL;
	struct net_device *dev = dev_get_by_name(&init_net,brname); 
	struct net_device *prtDev = NULL;

	if(!dev)
		return NULL;

	if (0 == (dev->priv_flags & IFF_EBRIDGE))
	{
		printk("%s: invalid bridge name specified %s\n", 
		         __FUNCTION__, brname);
		dev_put(dev);
		return NULL;
	}
	br = netdev_priv(dev);

	rcu_read_lock();
	br_port = br_get_port(br, index);
	if ( br_port )
	{
		prtDev = br_port->dev;
	}
	rcu_read_unlock();
	dev_put(dev);
	return prtDev;
}

static RAW_NOTIFIER_HEAD(mcast_snooping_chain);

int register_mcast_snooping_notifier(struct notifier_block *nb) {
	return raw_notifier_chain_register(&mcast_snooping_chain,nb);
}

int unregister_mcast_snooping_notifier(struct notifier_block *nb) {
	return raw_notifier_chain_unregister(&mcast_snooping_chain,nb);
}

int mcast_snooping_call_chain(unsigned long val,void *v) 
{
	t_MCPD_MLD_SNOOP_ENTRY *snoopEntry=(t_MCPD_MLD_SNOOP_ENTRY *)v;
	struct net_device *device=br_get_device_by_index(snoopEntry->br_name,snoopEntry->port_no);
	if(!strncmp(device->name,"wl",2))  
		return raw_notifier_call_chain(&mcast_snooping_chain,val,v);
	else 
		return 1;
}


void br_mcast_wl_flush(struct net_bridge *br) {
	t_MCPD_MLD_SNOOP_ENTRY snoopEntry;
	struct net_bridge_port *p;

	rcu_read_lock();   
	list_for_each_entry_rcu(p, &br->port_list, list) {
		if(!strncmp(p->dev->name,"wl",2)){
			snoopEntry.port_no= p->port_no;
			memcpy(snoopEntry.br_name,br->dev->name,IFNAMSIZ);
			mcast_snooping_call_chain(SNOOPING_FLUSH_ENTRY_ALL,(void *)&snoopEntry);
		}
	}
	rcu_read_unlock();
}

void br_mld_wl_del_entry(struct net_bridge *br,struct net_br_mld_mc_fdb_entry *dst) {
	if(dst && (!strncmp(dst->dst->dev->name,"wl",2))) { 
		t_MCPD_MLD_SNOOP_ENTRY snoopEntry;
		snoopEntry.port_no=dst->dst->port_no;
		memcpy(snoopEntry.br_name,br->dev->name,IFNAMSIZ);
		memcpy(&snoopEntry.grp,&dst->grp,sizeof(struct in6_addr));
		mcast_snooping_call_chain(SNOOPING_FLUSH_ENTRY,(void *)&snoopEntry);
	} 

}
EXPORT_SYMBOL(unregister_mcast_snooping_notifier);
EXPORT_SYMBOL(register_mcast_snooping_notifier);
EXPORT_SYMBOL(br_get_device_by_index);

#endif /* defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP) */

#endif /* defined(CONFIG_BCM_KF_IGMP) || defined(CONFIG_BCM_KF_MLD) */
