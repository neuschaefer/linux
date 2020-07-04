/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
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

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
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
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include <linux/list.h>
#include <linux/rtnetlink.h>
#include <linux/if_pppox.h>
#include <linux/ppp_defs.h>
#include <linux/module.h>
#include "br_private.h"
#include "br_mld.h"
#include <linux/if_vlan.h>
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif
#include "br_mcast.h"

void br_mld_get_ip_icmp_hdrs(const struct sk_buff *pskb, struct ipv6hdr **ppipv6mcast, struct icmp6hdr **ppicmpv6, int *lanppp)
{
	struct ipv6hdr *pipv6 = NULL;
	struct icmp6hdr *picmp = NULL;
	struct pppoe_hdr *pppoe = NULL;
	const unsigned char *dest = eth_hdr(pskb)->h_dest;

	if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_IPV6) )
	{
		pipv6 = (struct ipv6hdr *)skb_network_header(pskb);
	}
	else if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_PPP_SES) )
	{
		pppoe = (struct pppoe_hdr *)skb_network_header(pskb);
		if ( pppoe->tag[0].tag_type == htons(PPP_IPV6))
		{
			pipv6 = (struct ipv6hdr *)(skb_network_header(pskb) + PPPOE_SES_HLEN);
		}
	}
	else if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_8021Q) )
	{
		if ( vlan_eth_hdr(pskb)->h_vlan_encapsulated_proto == htons(ETH_P_IPV6) )
		{
			pipv6 = (struct ipv6hdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr));
		}
		else if ( vlan_eth_hdr(pskb)->h_vlan_encapsulated_proto == htons(ETH_P_PPP_SES) )
		{
			struct pppoe_hdr *pppoe = (struct pppoe_hdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr));
			if ( pppoe->tag[0].tag_type == PPP_IP)
			{
				pipv6 = (struct ipv6hdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr) + PPPOE_SES_HLEN);
			}
		}
	}

	*ppipv6mcast = NULL;
	*ppicmpv6 = NULL;
	if ( pipv6 != NULL )
	{
		if ( pppoe != NULL )
		{
			/* MAC will be unicast so check IP */
			if (pipv6 && (pipv6->daddr.s6_addr[0] == 0xFF))
			{
				u8 *nextHdr = (u8 *)((u8*)pipv6 + sizeof(struct ipv6hdr));
				if ( (pipv6->nexthdr == IPPROTO_HOPOPTS) &&
				     (*nextHdr == IPPROTO_ICMPV6) )
				{
					/* skip past hop by hop hdr */
					picmp =  (struct icmp6hdr *)(nextHdr + 8);
				}
				*ppipv6mcast = pipv6;
				*ppicmpv6 = picmp;
				if ( lanppp != NULL )
				{
					*lanppp = 1;
				}
			}
		}
		else
		{
			if ((BR_MLD_MULTICAST_MAC_PREFIX == dest[0]) && 
			    (BR_MLD_MULTICAST_MAC_PREFIX == dest[1]))
			{
				u8 *nextHdr = (u8 *)((u8*)pipv6 + sizeof(struct ipv6hdr));
				if ( (pipv6->nexthdr == IPPROTO_HOPOPTS) &&
				     (*nextHdr == IPPROTO_ICMPV6) )
				{
					/* skip past hop by hop hdr */
					picmp =  (struct icmp6hdr *)(nextHdr + 8);
				}

				*ppipv6mcast = pipv6;
				*ppicmpv6 = picmp;
				if ( lanppp != NULL )
				{
					*lanppp = 0;
				}
			}
		}
	}
	return;
}

int br_mld_snooping_enabled(struct net_device *dev) {
	struct net_bridge *br;
        struct net_bridge_port *port;

        port = br_port_get_rcu(dev);
	if (port) {
		br = port->br;
        	if (br->mld_snooping==SNOOPING_DISABLED_MODE) 
		return 0;
		else return 1;
	}
	return 0;
}

static struct kmem_cache *br_mld_mc_fdb_cache __read_mostly;
static struct kmem_cache *br_mld_mc_rep_cache __read_mostly;
static u32 br_mld_mc_fdb_salt __read_mostly;
static struct proc_dir_entry *br_mld_entry = NULL;

extern int mcpd_process_skb(struct net_bridge *br, struct sk_buff *skb,
                            unsigned short protocol);

static struct in6_addr all_dhcp_srvr_addr = { .in6_u.u6_addr32 = {0xFF050000, 
                                                                  0x00000000, 
                                                                  0x00000000, 
                                                                  0x00010003 } };

static inline int br_mld_mc_fdb_hash(const struct in6_addr *grp)
{
	return jhash_1word((grp->s6_addr32[0] | grp->s6_addr32[3]), 
                                   br_mld_mc_fdb_salt) & (BR_MLD_HASH_SIZE - 1);
}

static int br_mld_control_filter(const unsigned char *dest, const struct in6_addr *ipv6)
{
    /* ignore any packets that are not multicast
       ignore scope0, node and link local addresses
       ignore IPv6 all DHCP servers address */
    if(((dest) && is_broadcast_ether_addr(dest)) || 
       (!BCM_IN6_IS_ADDR_MULTICAST(ipv6)) ||
       (BCM_IN6_IS_ADDR_MC_SCOPE0(ipv6)) ||
       (BCM_IN6_IS_ADDR_MC_NODELOCAL(ipv6)) ||
       (BCM_IN6_IS_ADDR_MC_LINKLOCAL(ipv6)) ||
       (0 == memcmp(ipv6, &all_dhcp_srvr_addr, sizeof(struct in6_addr))))
        return 0;
    else
        return 1;
}

/* This function requires that br->mld_mcl_lock is already held */
void br_mld_mc_fdb_del_entry(struct net_bridge *br, 
                             struct net_br_mld_mc_fdb_entry *mld_fdb,
                             struct in6_addr *rep)
{
	struct net_br_mld_mc_rep_entry *rep_entry = NULL;
	struct net_br_mld_mc_rep_entry *rep_entry_n = NULL;

	list_for_each_entry_safe(rep_entry, 
	                         rep_entry_n, &mld_fdb->rep_list, list) 
	{
		if((NULL == rep) ||
		   (BCM_IN6_ARE_ADDR_EQUAL(&rep_entry->rep, rep)))
		{
			list_del(&rep_entry->list);
			kmem_cache_free(br_mld_mc_rep_cache, rep_entry);
			if (rep)
{
				break;
			}
		}
	}
	if(list_empty(&mld_fdb->rep_list)) 
{
		hlist_del(&mld_fdb->hlist);
		br_mld_wl_del_entry(br, mld_fdb);
#if defined(CONFIG_BLOG) 
		br_mcast_blog_release(BR_MCAST_PROTO_MLD, (void *)mld_fdb);
#endif
#ifdef CONFIG_ATP_BRCM
		if (mld_fdb->from_dev)
		{
			dev_put(mld_fdb->from_dev);
		}
#endif
		kmem_cache_free(br_mld_mc_fdb_cache, mld_fdb);
}

	return;
}

void br_mld_set_timer( struct net_bridge *br )
{
	struct net_br_mld_mc_fdb_entry *mcast_group;
	int                             i;
	unsigned long                   tstamp;
	unsigned int                    found;

	if ( br->mld_snooping == 0 )
	{
		del_timer(&br->mld_timer);
		return;
	}

	/* the largest timeout is BR_MLD_MEMBERSHIP_TIMEOUT */
	tstamp = jiffies + (BR_MLD_MEMBERSHIP_TIMEOUT*HZ*2);
	found = 0;
	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h_group;
		hlist_for_each_entry(mcast_group, h_group, &br->mld_mc_hash[i], hlist) 
		{
			struct net_br_mld_mc_rep_entry *reporter;
			list_for_each_entry(reporter, &mcast_group->rep_list, list)
			{
				if ( time_after(tstamp, reporter->tstamp) )
				{
					tstamp = reporter->tstamp;
					found  = 1;
				}
			}
		}
	}

	if ( 0 == found )
	{
		del_timer(&br->mld_timer);
	}
	else
	{
		mod_timer(&br->mld_timer, (tstamp + TIMER_CHECK_TIMEOUT));
	}
}


static void br_mld_query_timeout(unsigned long ptr)
{
	struct net_br_mld_mc_fdb_entry *mcast_group;
	struct net_bridge *br;
	int i;
    
	br = (struct net_bridge *) ptr;

	spin_lock_bh(&br->mld_mcl_lock);
	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h_group, *n_group;
		hlist_for_each_entry_safe(mcast_group, h_group, n_group, &br->mld_mc_hash[i], hlist) 
		{
			struct net_br_mld_mc_rep_entry *reporter, *n_reporter;
			list_for_each_entry_safe(reporter, n_reporter, &mcast_group->rep_list, list)
			{
				if (time_after_eq(jiffies, reporter->tstamp))
				{
					br_mld_mc_fdb_del_entry(br, mcast_group, &reporter->rep);
				}
			}
		}
	}

	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);
}

static struct net_br_mld_mc_rep_entry *
                br_mld_rep_find(const struct net_br_mld_mc_fdb_entry *mc_fdb,
                                const struct in6_addr *rep)
{
	struct net_br_mld_mc_rep_entry *rep_entry;

	list_for_each_entry(rep_entry, &mc_fdb->rep_list, list)
   {
		if(BCM_IN6_ARE_ADDR_EQUAL(&rep_entry->rep, rep))
			return rep_entry;
    }

	return NULL;
}

/* In the case where a reporter has changed ports, this function
   will remove all records pointing to the old port */
void br_mld_wipe_reporter_for_port (struct net_bridge *br,
                                    struct in6_addr *rep, 
                                    u16 oldPort)
{
    int hashIndex = 0;
    struct hlist_node *h = NULL;
    struct hlist_node *n = NULL;
    struct hlist_head *head = NULL;
    struct net_br_mld_mc_fdb_entry *mc_fdb;

    spin_lock_bh(&br->mld_mcl_lock);
    for ( ; hashIndex < BR_MLD_HASH_SIZE ; hashIndex++)
    {
        head = &br->mld_mc_hash[hashIndex];
        hlist_for_each_entry_safe(mc_fdb, h, n, head, hlist)
        {
            if ((br_mld_rep_find(mc_fdb, rep)) &&
                (mc_fdb->dst->port_no == oldPort))
            {
                /* The reporter we're looking for has been found
                   in a record pointing to its old port */
                br_mld_mc_fdb_del_entry (br, mc_fdb, rep);
            }
        }
    }
    br_mld_set_timer(br);
    spin_unlock_bh(&br->mld_mcl_lock);
}

/* this is called during addition of a snooping entry and requires that 
   mld_mcl_lock is already held */
static int br_mld_mc_fdb_update(struct net_bridge *br, 
                                struct net_bridge_port *prt, 
                                struct in6_addr *grp, 
                                struct in6_addr *rep, 
                                int mode, 
                                struct in6_addr *src,
                                struct net_device *from_dev)
{
	struct net_br_mld_mc_fdb_entry *dst;
	struct net_br_mld_mc_rep_entry *rep_entry = NULL;
	int ret = 0;
	int filt_mode;
	struct hlist_head *head;
	struct hlist_node *h;

	if(mode == SNOOP_IN_ADD)
		filt_mode = MCAST_INCLUDE;
	else
		filt_mode = MCAST_EXCLUDE;
    
	head = &br->mld_mc_hash[br_mld_mc_fdb_hash(grp)];
	hlist_for_each_entry(dst, h, head, hlist) {
		if (BCM_IN6_ARE_ADDR_EQUAL(&dst->grp, grp))
		{
			if((BCM_IN6_ARE_ADDR_EQUAL(src, &dst->src_entry.src)) &&
			   (filt_mode == dst->src_entry.filt_mode) && 
			   (dst->from_dev == from_dev) &&
			   (dst->dst == prt) )
			{
				/* found entry - update TS */
				struct net_br_mld_mc_rep_entry *reporter = br_mld_rep_find(dst, rep);
				if(reporter == NULL)
				{
					rep_entry = kmem_cache_alloc(br_mld_mc_rep_cache, GFP_ATOMIC);
					if(rep_entry)
					{
						BCM_IN6_ASSIGN_ADDR(&rep_entry->rep, rep);
						rep_entry->tstamp = jiffies + BR_MLD_MEMBERSHIP_TIMEOUT*HZ;
						list_add_tail(&rep_entry->list, &dst->rep_list);
						br_mld_set_timer(br);
					}
				}
				else 
				{
					reporter->tstamp = jiffies + BR_MLD_MEMBERSHIP_TIMEOUT*HZ;
					br_mld_set_timer(br);
				}
				ret = 1;
			}
		}
	}
	return ret;
}

int br_mld_process_if_change(struct net_bridge *br, struct net_device *ndev)
{
	struct net_br_mld_mc_fdb_entry *dst;
	int i;

	spin_lock_bh(&br->mld_mcl_lock);
	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mld_mc_hash[i], hlist) 
		{
			if ((NULL == ndev) ||
			    (dst->dst->dev == ndev) ||
			    (dst->from_dev == ndev))
			{
				br_mld_mc_fdb_del_entry(br, dst, NULL);
			}
		}
	}
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);

	return 0;
}

int br_mld_mc_fdb_add(struct net_device *from_dev,
                        int wan_ops,
                        struct net_bridge *br, 
                        struct net_bridge_port *prt, 
                        struct in6_addr *grp, 
                        struct in6_addr *rep, 
                        int mode, 
                        uint16_t tci, 
                        struct in6_addr *src,
                        int lanppp)
{
	struct net_br_mld_mc_fdb_entry *mc_fdb;
	struct net_br_mld_mc_rep_entry *rep_entry = NULL;
	struct hlist_head *head = NULL;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
   int ret = 1;
#endif

	if(!br || !prt || !grp|| !rep || !from_dev)
		return 0;

        if(!(br_mld_control_filter(NULL, grp) ||
            BCM_IN6_IS_ADDR_L2_MCAST(grp)))
            return 0;

	if(!netdev_path_is_leaf(from_dev))
		return 0;

	if((SNOOP_IN_ADD != mode) && (SNOOP_EX_ADD != mode))
		return 0;

	/* allocate before getting the lock so that GFP_KERNEL can be used */
	mc_fdb = kmem_cache_alloc(br_mld_mc_fdb_cache, GFP_KERNEL);
	if (!mc_fdb)
	{
		return -ENOMEM;
	}
	rep_entry = kmem_cache_alloc(br_mld_mc_rep_cache, GFP_KERNEL);
	if ( !rep_entry )
	{
		kmem_cache_free(br_mld_mc_fdb_cache, mc_fdb);
		return -ENOMEM;
	}

	spin_lock_bh(&br->mld_mcl_lock);
	if (br_mld_mc_fdb_update(br, prt, grp, rep, mode, src, from_dev))
	{
		kmem_cache_free(br_mld_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_mld_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mld_mcl_lock);
		return 0;
	}
   
	BCM_IN6_ASSIGN_ADDR(&mc_fdb->grp, grp);
	BCM_IN6_ASSIGN_ADDR(&mc_fdb->src_entry, src);
	mc_fdb->src_entry.filt_mode = 
	             (mode == SNOOP_IN_ADD) ? MCAST_INCLUDE : MCAST_EXCLUDE;
	mc_fdb->dst = prt;
	mc_fdb->lan_tci = tci;
	mc_fdb->wan_tci = 0;
	mc_fdb->num_tags = 0;
	mc_fdb->from_dev = from_dev;
#ifdef CONFIG_ATP_BRCM
	dev_hold(mc_fdb->from_dev);
#endif
	mc_fdb->type = wan_ops;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	mc_fdb->root = 1;
	mc_fdb->blog_idx = BLOG_KEY_INVALID;
#endif
	mc_fdb->lanppp = lanppp;
	INIT_LIST_HEAD(&mc_fdb->rep_list);
	BCM_IN6_ASSIGN_ADDR(&rep_entry->rep, rep);
	rep_entry->tstamp = jiffies + (BR_MLD_MEMBERSHIP_TIMEOUT*HZ);
	list_add_tail(&rep_entry->list, &mc_fdb->rep_list);

	head = &br->mld_mc_hash[br_mld_mc_fdb_hash(grp)];
	hlist_add_head(&mc_fdb->hlist, head);

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	ret = br_mcast_blog_process(br, (void *)mc_fdb, BR_MCAST_PROTO_MLD);
	if(ret < 0)
	{
		hlist_del(&mc_fdb->hlist);
#ifdef CONFIG_ATP_BRCM
		if (mc_fdb->from_dev)
		{
			dev_put(mc_fdb->from_dev);
		}
#endif
		kmem_cache_free(br_mld_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_mld_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mld_mcl_lock);
		return ret;
	}
#endif
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);

	return 1;
}
EXPORT_SYMBOL(br_mld_mc_fdb_add);

void br_mld_mc_fdb_cleanup(struct net_bridge *br)
{
	struct net_br_mld_mc_fdb_entry *dst;
	int i;

	spin_lock_bh(&br->mld_mcl_lock);
	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mld_mc_hash[i], hlist) 
		{
			br_mld_mc_fdb_del_entry(br, dst, NULL);
		}
	}
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);
}

void br_mld_mc_fdb_remove_grp(struct net_bridge *br, 
                              struct net_bridge_port *prt, 
                              struct in6_addr *grp)
{
	struct net_br_mld_mc_fdb_entry *dst;
	struct hlist_head *head = NULL;
	struct hlist_node *h, *n;

	if(!br || !prt || !grp)
		return;

	spin_lock_bh(&br->mld_mcl_lock);
	head = &br->mld_mc_hash[br_mld_mc_fdb_hash(grp)];
	hlist_for_each_entry_safe(dst, h, n, head, hlist) {
		if ((BCM_IN6_ARE_ADDR_EQUAL(&dst->grp, grp)) && 
		    (dst->dst == prt))
		{
			br_mld_mc_fdb_del_entry(br, dst, NULL);
		}
	}
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);
}

int br_mld_mc_fdb_remove(struct net_device *from_dev,
                         struct net_bridge *br, 
                         struct net_bridge_port *prt, 
                         struct in6_addr *grp, 
                         struct in6_addr *rep, 
                         int mode, 
                         struct in6_addr *src)
{
	struct net_br_mld_mc_fdb_entry *mc_fdb;
	int filt_mode;
	struct hlist_head *head = NULL;
	struct hlist_node *h, *n;
    
	if(!br || !prt || !grp|| !rep || !from_dev)
		return 0;

        if(!(br_mld_control_filter(NULL, grp) ||
            BCM_IN6_IS_ADDR_L2_MCAST(grp)))
            return 0;

	if(!netdev_path_is_leaf(from_dev))
		return 0;

	if((SNOOP_IN_CLEAR != mode) && (SNOOP_EX_CLEAR != mode))
		return 0;

	if(mode == SNOOP_IN_CLEAR)
		filt_mode = MCAST_INCLUDE;
	else
		filt_mode = MCAST_EXCLUDE;

	spin_lock_bh(&br->mld_mcl_lock);
	head = &br->mld_mc_hash[br_mld_mc_fdb_hash(grp)];
	hlist_for_each_entry_safe(mc_fdb, h, n, head, hlist) 
	{
		if ((BCM_IN6_ARE_ADDR_EQUAL(&mc_fdb->grp, grp)) && 
		    (filt_mode == mc_fdb->src_entry.filt_mode) &&
		    (BCM_IN6_ARE_ADDR_EQUAL(&mc_fdb->src_entry.src, src)) &&
		    (mc_fdb->from_dev == from_dev) &&
		    (mc_fdb->dst == prt))
		{
			br_mld_mc_fdb_del_entry(br, mc_fdb, rep);
		}
	}
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);

	return 0;
}
EXPORT_SYMBOL(br_mld_mc_fdb_remove);

int br_mld_mc_forward(struct net_bridge *br, 
                      struct sk_buff *skb, 
                      int forward, 
                      int is_routed)
{
	struct net_br_mld_mc_fdb_entry *dst;
	int status = 0;
	struct sk_buff *skb2;
	struct net_bridge_port *p, *p_n;
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct hlist_head *head = NULL;
	struct hlist_node *h;
	struct ipv6hdr *pipv6mcast = NULL;
	struct icmp6hdr *picmpv6 = NULL;
	int lanppp;

	br_mld_get_ip_icmp_hdrs(skb, &pipv6mcast, &picmpv6, &lanppp);
	if ( pipv6mcast == NULL )
	{
		return status;
	}
   
	if ( picmpv6 != NULL )
	{
		if((picmpv6->icmp6_type == ICMPV6_MGM_REPORT) ||
			(picmpv6->icmp6_type == ICMPV6_MGM_REDUCTION) || 
			(picmpv6->icmp6_type == ICMPV6_MLD2_REPORT)) 
		{
			rcu_read_lock();
                        if(skb->dev && (br_port_get_rcu(skb->dev)) &&
                            (br->mld_snooping || br->mld_proxy ||
                            is_multicast_switching_mode_host_control()))
			{
				/* for bridged WAN service, do not pass any MLD packets
				   coming from the WAN port to mcpd */
#if defined(CONFIG_BCM_KF_WANDEV)
				if ( skb->dev->priv_flags & IFF_WANDEV )
				{
					kfree_skb(skb);
					status = 1;
				}
				else
#endif
				{
				   mcpd_process_skb(br, skb, ETH_P_IPV6);
				}
			}
			rcu_read_unlock();
			return status;
		}
	}

	/* snooping could be disabled and still have entries */

	/* drop traffic by default when snooping is enabled
	   in blocking mode */
	if ((br->mld_snooping == SNOOPING_BLOCKING_MODE) &&
	     br_mld_control_filter(dest, &pipv6mcast->daddr))
	{
		status = 1;
	}

	spin_lock_bh(&br->mld_mcl_lock);
	head = &br->mld_mc_hash[br_mld_mc_fdb_hash(&pipv6mcast->daddr)];
	hlist_for_each_entry(dst, h, head, hlist) {
		if (BCM_IN6_ARE_ADDR_EQUAL(&dst->grp, &pipv6mcast->daddr)) {
			/* routed packet will have bridge as from dev - cannot match to mc_fdb */
			if ( is_routed ) {
				if ( dst->type != MCPD_IF_TYPE_ROUTED ) {
					continue;
				}
			}
			else {
				if ( dst->type != MCPD_IF_TYPE_BRIDGED ) {
					continue;
				}
#if defined(CONFIG_BCM_KF_WANDEV)
				if (skb->dev->priv_flags & IFF_WANDEV) {
					/* match exactly if skb device is a WAN device - otherwise continue */
					if (dst->from_dev != skb->dev)
						continue;
				}
				else 
#endif
				{
					/* if this is not an L2L mc_fdb entry continue */
					if (dst->from_dev != br->dev)
						continue;            
				}
			}
			if((dst->src_entry.filt_mode == MCAST_INCLUDE) && 
			   (BCM_IN6_ARE_ADDR_EQUAL(&pipv6mcast->saddr, &dst->src_entry.src))) {
				if (!dst->dst->dirty) {
					if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
					{
						spin_unlock_bh(&br->mld_mcl_lock);
						return 0;
					} 
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
					blog_clone(skb, blog_ptr(skb2));
#endif
					if(forward)
						br_forward(dst->dst, skb2, NULL);
					else
						br_deliver(dst->dst, skb2);
				}
				dst->dst->dirty = 1;
				status = 1;
			}
			else if(dst->src_entry.filt_mode == MCAST_EXCLUDE) {
				if((0 == dst->src_entry.src.s6_addr[0]) ||
				   (!BCM_IN6_ARE_ADDR_EQUAL(&pipv6mcast->saddr, &dst->src_entry.src))) {
					if (!dst->dst->dirty) {
						if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
						{
							spin_unlock_bh(&br->mld_mcl_lock);
							return 0;
						}
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
						blog_clone(skb, blog_ptr(skb2));
#endif
						if(forward)
							br_forward(dst->dst, skb2, NULL);
						else
							br_deliver(dst->dst, skb2);
					}
					dst->dst->dirty = 1;
					status = 1;
				}
				else if( BCM_IN6_ARE_ADDR_EQUAL(&pipv6mcast->saddr, &dst->src_entry.src)) {
					status = 1;
				}
			}
		}
	}
	spin_unlock_bh(&br->mld_mcl_lock);

	if (status) {
		list_for_each_entry_safe(p, p_n, &br->port_list, list) {
			p->dirty = 0;
		}
	}

	if(status)
		kfree_skb(skb);

	return status;
}

int br_mld_mc_fdb_update_bydev( struct net_bridge *br,
                                struct net_device *dev,
                                unsigned int       flushAll)
{
	struct net_br_mld_mc_fdb_entry *mc_fdb;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	int ret;
#endif
	int i;

	if(!br || !dev)
		return 0;

	if(!netdev_path_is_leaf(dev))
		return 0;

	spin_lock_bh(&br->mld_mcl_lock);
	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(mc_fdb, h, n, &br->mld_mc_hash[i], hlist) 
		{
			if ((mc_fdb->dst->dev == dev) ||
			    (mc_fdb->from_dev == dev))
			{
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
				/* do not remove the root entry */
				if ((0 == mc_fdb->root) || (1 == flushAll))
				{
					br_mld_mc_fdb_del_entry(br, mc_fdb, NULL);
				}
				else
				{
					br_mcast_blog_release(BR_MCAST_PROTO_MLD, (void *)mc_fdb);
					mc_fdb->blog_idx = BLOG_KEY_INVALID;
				}
#else
				if (1 == flushAll)
				{
					br_mld_mc_fdb_del_entry(br, mc_fdb, NULL);
				}
#endif
			}
		}
	}

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	if ( 0 == flushAll )
	{
		for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
		{
			struct hlist_node *h, *n;
			hlist_for_each_entry_safe(mc_fdb, h, n, &br->mld_mc_hash[i], hlist) 
			{ 
				if ( (1 == mc_fdb->root) &&
				     ((mc_fdb->dst->dev == dev) ||
				      (mc_fdb->from_dev == dev)) )
				{
					mc_fdb->wan_tci  = 0;
					mc_fdb->num_tags = 0;
					ret = br_mcast_blog_process(br, (void*)mc_fdb, BR_MCAST_PROTO_MLD);
					if(ret < 0)
					{
						/* br_mcast_blog_process may return -1 if there are no blog rules
						 * which may be a valid scenario, in which case we delete the
						 * multicast entry.
						 */
						br_mld_mc_fdb_del_entry(br, mc_fdb, NULL);
						//printk(KERN_WARNING "%s: Failed to create the blog\n", __FUNCTION__);
					}
				}
			}
		}
	}
#endif   
	br_mld_set_timer(br);
	spin_unlock_bh(&br->mld_mcl_lock);

	return 0;
}

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
/* This is a support function for vlan/blog processing that requires that 
   br->mld_mcl_lock is already held */
struct net_br_mld_mc_fdb_entry *br_mld_mc_fdb_copy(struct net_bridge *br, 
                                     const struct net_br_mld_mc_fdb_entry *mld_fdb)
{
	struct net_br_mld_mc_fdb_entry *new_mld_fdb = NULL;
	struct net_br_mld_mc_rep_entry *rep_entry = NULL;
	struct net_br_mld_mc_rep_entry *rep_entry_n = NULL;
	int success = 1;
	struct hlist_head *head = NULL;

	new_mld_fdb = kmem_cache_alloc(br_mld_mc_fdb_cache, GFP_ATOMIC);
	if (new_mld_fdb)
	{
		memcpy(new_mld_fdb, mld_fdb, sizeof(struct net_br_mld_mc_fdb_entry));
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
		new_mld_fdb->blog_idx = BLOG_KEY_INVALID;
#endif
		new_mld_fdb->root = 0;
		INIT_LIST_HEAD(&new_mld_fdb->rep_list);

		list_for_each_entry(rep_entry, &mld_fdb->rep_list, list) {
			rep_entry_n = kmem_cache_alloc(br_mld_mc_rep_cache, GFP_ATOMIC);
			if(rep_entry_n)
			{
				memcpy(rep_entry_n, 
				       rep_entry, 
				       sizeof(struct net_br_mld_mc_rep_entry));
				list_add_tail(&rep_entry_n->list, &new_mld_fdb->rep_list);
			}
			else 
			{
				success = 0;
				break;
			}
		}

		if(success)
		{
			head = &br->mld_mc_hash[br_mld_mc_fdb_hash(&mld_fdb->grp)];
			hlist_add_head(&new_mld_fdb->hlist, head);
#ifdef CONFIG_ATP_BRCM
			dev_hold(new_mld_fdb->from_dev);
#endif
	}
		else
		{
			list_for_each_entry_safe(rep_entry, 
			                         rep_entry_n, &new_mld_fdb->rep_list, list) {
				list_del(&rep_entry->list);
				kmem_cache_free(br_mld_mc_rep_cache, rep_entry);
			}
			kmem_cache_free(br_mld_mc_fdb_cache, new_mld_fdb);
			new_mld_fdb = NULL;
		}
	}

	return new_mld_fdb;
} /* br_mld_mc_fdb_copy */
#endif

static void *snoop_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct net_device *dev;
	loff_t offs = 0;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev) {
		if ((dev->priv_flags & IFF_EBRIDGE) && (*pos == offs)) {
			return dev;
		}
	}
	++offs;
	return NULL;
}

static void *snoop_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net_device *dev = v;

	++*pos;
	for(dev = next_net_device_rcu(dev); dev; dev = next_net_device_rcu(dev)) {
		if(dev->priv_flags & IFF_EBRIDGE) {
			return dev;
		}
	}
	return NULL;
}

static int snoop_seq_show(struct seq_file *seq, void *v)
{
	struct net_device *dev = v;
	struct net_br_mld_mc_fdb_entry *dst;
	struct net_bridge *br = netdev_priv(dev);
	struct net_br_mld_mc_rep_entry *rep_entry;
	int first;
	int i;
	int tstamp;

	seq_printf(seq, "mld snooping %d  proxy %d  lan2lan-snooping %d/%d, priority %d\n",
              br->mld_snooping,
              br->mld_proxy,
	      br->mld_lan2lan_mc_enable,
	      br_mcast_get_lan2lan_snooping(BR_MCAST_PROTO_MLD, br),
              br_mcast_get_pri_queue());
	seq_printf(seq, "bridge device src-dev #tags lan-tci  wan-tci");
	seq_printf(seq, "    group                               mode source");
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	seq_printf(seq, "                              timeout reporter");
	seq_printf(seq, "                              Index\n");
#else
	seq_printf(seq, "                              timeout reporter\n");
#endif

	for (i = 0; i < BR_MLD_HASH_SIZE; i++) 
	{
		struct hlist_node *h;
		hlist_for_each(h, &br->mld_mc_hash[i]) 
		{
			dst = hlist_entry(h, struct net_br_mld_mc_fdb_entry, hlist);
			if( dst )
	{
		seq_printf(seq, "%-6s %-6s %-7s %02d    0x%04x   0x%04x%04x", 
		           br->dev->name, 
		           dst->dst->dev->name, 
		           dst->from_dev->name, 
		           dst->num_tags,
		           ntohs(dst->lan_tci),
		           ((dst->wan_tci >> 16) & 0xFFFF),
		           (dst->wan_tci & 0xFFFF));

		seq_printf(seq, " %08x:%08x:%08x:%08x",
		           htonl(dst->grp.s6_addr32[0]),
		           htonl(dst->grp.s6_addr32[1]),
		           htonl(dst->grp.s6_addr32[2]),
		           htonl(dst->grp.s6_addr32[3]));

		seq_printf(seq, " %-4s %08x:%08x:%08x:%08x", 
		           (dst->src_entry.filt_mode == MCAST_EXCLUDE) ? 
		            "EX" : "IN",
		           htonl(dst->src_entry.src.s6_addr32[0]), 
		           htonl(dst->src_entry.src.s6_addr32[1]), 
		           htonl(dst->src_entry.src.s6_addr32[2]), 
		           htonl(dst->src_entry.src.s6_addr32[3]));

		      first = 1;
				list_for_each_entry(rep_entry, &dst->rep_list, list)
				{ 

					if ( 0 == br->mld_snooping )
					{
						tstamp = 0;
					}
					else
					{
						tstamp = (int)(rep_entry->tstamp - jiffies) / HZ;
					}

					if(first)
					{
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
		seq_printf(seq, " %-7d %08x:%08x:%08x:%08x 0x%08x\n", 
						           tstamp,
						           htonl(rep_entry->rep.s6_addr32[0]),
						           htonl(rep_entry->rep.s6_addr32[1]),
						           htonl(rep_entry->rep.s6_addr32[2]),
						           htonl(rep_entry->rep.s6_addr32[3]), dst->blog_idx);
#else
		seq_printf(seq, " %-7d %08x:%08x:%08x:%08x\n", 
						           tstamp,
						           htonl(rep_entry->rep.s6_addr32[0]),
						           htonl(rep_entry->rep.s6_addr32[1]),
						           htonl(rep_entry->rep.s6_addr32[2]),
						           htonl(rep_entry->rep.s6_addr32[3]));
#endif
						first = 0;
					}
					else 
					{
						seq_printf(seq, "%126s %-7d %08x:%08x:%08x:%08x\n", " ", 
						           tstamp,
						           htonl(rep_entry->rep.s6_addr32[0]),
						           htonl(rep_entry->rep.s6_addr32[1]),
						           htonl(rep_entry->rep.s6_addr32[2]),
						           htonl(rep_entry->rep.s6_addr32[3]));
					}
				}
			}
		}
	}

	return 0;
}

static void snoop_seq_stop(struct seq_file *seq, void *v)
{
	rcu_read_unlock();
}

static struct seq_operations snoop_seq_ops = {
	.start = snoop_seq_start,
	.next  = snoop_seq_next,
	.stop  = snoop_seq_stop,
	.show  = snoop_seq_show,
};

static int snoop_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &snoop_seq_ops);
}

static struct file_operations br_mld_snoop_proc_fops = {
	.owner = THIS_MODULE,
	.open  = snoop_seq_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

void br_mld_snooping_br_init( struct net_bridge *br )
{
	spin_lock_init(&br->mld_mcl_lock);
	br->mld_lan2lan_mc_enable = BR_MC_LAN2LAN_STATUS_DEFAULT;
	setup_timer(&br->mld_timer, br_mld_query_timeout, (unsigned long)br);  
}

void br_mld_snooping_br_fini( struct net_bridge *br )
{
	del_timer_sync(&br->mld_timer);
}

int __init br_mld_snooping_init(void)
{
	br_mld_entry = proc_create("mld_snooping", 0, init_net.proc_net,
			   &br_mld_snoop_proc_fops);

	if(!br_mld_entry) {
		printk("error while creating mld_snooping proc\n");
        return -ENOMEM;
	}

	br_mld_mc_fdb_cache = kmem_cache_create("bridge_mld_mc_fdb_cache",
                            sizeof(struct net_br_mld_mc_fdb_entry),
                            0,
                            SLAB_HWCACHE_ALIGN, NULL);
    if (!br_mld_mc_fdb_cache)
		return -ENOMEM;

    br_mld_mc_rep_cache = kmem_cache_create("br_mld_mc_rep_cache",
                                             sizeof(struct net_br_mld_mc_rep_entry),
                                             0,
                                             SLAB_HWCACHE_ALIGN, NULL);
    if (!br_mld_mc_rep_cache)
    {
       kmem_cache_destroy(br_mld_mc_fdb_cache);
		 return -ENOMEM;
    }

	 get_random_bytes(&br_mld_mc_fdb_salt, sizeof(br_mld_mc_fdb_salt));

    return 0;
}

void br_mld_snooping_fini(void)
{
	kmem_cache_destroy(br_mld_mc_fdb_cache);
	kmem_cache_destroy(br_mld_mc_rep_cache);

    return;
}

EXPORT_SYMBOL(br_mld_control_filter);
EXPORT_SYMBOL(br_mld_snooping_enabled);
#endif /* defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP) */
