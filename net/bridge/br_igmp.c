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
#if defined(CONFIG_BCM_KF_IGMP)
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <asm/atomic.h>
#include <linux/ip.h>
#include <linux/if_vlan.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/rtnetlink.h>
#include <linux/export.h>
#include <linux/igmp.h>
#include <linux/if_pppox.h>
#include <linux/ppp_defs.h>
#include "br_private.h"
#include "br_igmp.h"
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/if_vlan.h>
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif
#include "br_mcast.h"

#include "atp_interface.h"
#include "br_igmp_parse.h"


#define MCPD_NETLINK_SKB_TIMEOUT_MS 2000

void br_igmp_get_ip_igmp_hdrs( const struct sk_buff *pskb, struct iphdr **ppipmcast, struct igmphdr **ppigmp, int *lanppp)
{
	struct iphdr *pip = NULL;
	struct igmphdr *pigmp = NULL;
	struct pppoe_hdr *pppoe = NULL;
	const unsigned char *dest = eth_hdr(pskb)->h_dest;

	if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_IP) )
	{
		pip = (struct iphdr *)skb_network_header(pskb);
	}
	else if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_PPP_SES) )
	{
		pppoe = (struct pppoe_hdr *)skb_network_header(pskb);
		if ( pppoe->tag[0].tag_type == htons(PPP_IP))
		{
			pip = (struct iphdr *)(skb_network_header(pskb) + PPPOE_SES_HLEN);
		}
	}
	else if ( vlan_eth_hdr(pskb)->h_vlan_proto == htons(ETH_P_8021Q) )
	{
		if ( vlan_eth_hdr(pskb)->h_vlan_encapsulated_proto == htons(ETH_P_IP) )
		{
			pip = (struct iphdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr));
		}
		else if ( vlan_eth_hdr(pskb)->h_vlan_encapsulated_proto == htons(ETH_P_PPP_SES) )
		{
			struct pppoe_hdr *pppoe = (struct pppoe_hdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr));
			if ( pppoe->tag[0].tag_type == PPP_IP)
			{
				pip = (struct iphdr *)(skb_network_header(pskb) + sizeof(struct vlan_hdr) + PPPOE_SES_HLEN);
			}
		}
	}

	*ppipmcast = NULL;
	*ppigmp = NULL;
	if ( pip != NULL )
	{
		if ( pppoe != NULL )
		{
			/* MAC will be unicast so check IP */
			if ((pip->daddr & htonl(0xF0000000)) == htonl(0xE0000000))
			{
				if ( pip->protocol == IPPROTO_IGMP ) {
					pigmp = (struct igmphdr *)((char *)pip + (pip->ihl << 2));
				}
				*ppipmcast = pip;
				*ppigmp = pigmp;
				if ( lanppp != NULL )
				{
					*lanppp = 1;
				}
			}
		}
		else
		{
			if ( is_multicast_ether_addr(dest) && !is_broadcast_ether_addr(dest) )
			{
				if ( pip->protocol == IPPROTO_IGMP ) {
					pigmp = (struct igmphdr *)((char *)pip + (pip->ihl << 2));
				}
				*ppipmcast = pip;
				*ppigmp = pigmp;
				if ( lanppp != NULL )
				{
					*lanppp = 0;
				}
			}
		}
	}
	return;
}

#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)

static struct kmem_cache *br_igmp_mc_fdb_cache __read_mostly;
static struct kmem_cache *br_igmp_mc_rep_cache __read_mostly;
static u32 br_igmp_mc_fdb_salt __read_mostly;
static struct proc_dir_entry *br_igmp_entry = NULL;

#ifdef CONFIG_SUPPORT_ATP
extern int br_snooping_init(void);
#endif
extern int mcpd_process_skb(struct net_bridge *br, struct sk_buff *skb,
                            unsigned short protocol);
extern void mcpd_nl_process_timer_check ( void );
extern unsigned long mcpd_nl_get_next_timer_expiry ( int *found );

static struct in_addr ip_upnp_addr      = {htonl(0xEFFFFFFA)}; /* UPnP / SSDP */
static struct in_addr ip_ntfy_srvr_addr = {htonl(0xE000FF87)}; /* Notificatoin Server*/

static inline int br_igmp_mc_fdb_hash(const u32 grp)
{
	return jhash_1word(grp, br_igmp_mc_fdb_salt) & (BR_IGMP_HASH_SIZE - 1);
}

int br_igmp_control_filter(const unsigned char *dest, __be32 dest_ip)
{
    if(((dest) && is_broadcast_ether_addr(dest)) ||
       ((dest_ip & htonl(0xFFFFFF00)) == htonl(0xE0000000)) ||
       (dest_ip == ip_upnp_addr.s_addr) || /* UPnp/SSDP */
       (dest_ip == ip_ntfy_srvr_addr.s_addr))   /* Notification srvr */
    {
        return 0;
    }
    else
    {
        return 1;
    }
} /* br_igmp_control_filter */

/* This function requires that br->mcl_lock is already held */
void br_igmp_mc_fdb_del_entry(struct net_bridge *br, 
                              struct net_bridge_mc_fdb_entry *igmp_fdb,
                              struct in_addr *rep)
{
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	struct net_bridge_mc_rep_entry *rep_entry_n = NULL;

	list_for_each_entry_safe(rep_entry, 
	                         rep_entry_n, &igmp_fdb->rep_list, list) 
{
		if((NULL == rep) ||
		   (rep_entry->rep.s_addr == rep->s_addr))
		{
			list_del(&rep_entry->list);
			kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
			if (rep)
			{
				break;
			}
		}
	}
	if(list_empty(&igmp_fdb->rep_list)) 
	{
		hlist_del(&igmp_fdb->hlist);
		if ( br->igmp_snooping )
{
			mcpd_nl_send_igmp_purge_entry(igmp_fdb);
		}
#if defined(CONFIG_BLOG) 
		br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)igmp_fdb);
#endif
#ifdef CONFIG_ATP_BRCM
		if (igmp_fdb->from_dev)
		{
			dev_put(igmp_fdb->from_dev);
		}
#endif
		kmem_cache_free(br_igmp_mc_fdb_cache, igmp_fdb);
}

	return;
}

void br_igmp_process_timer_check ( struct net_bridge *br )
{
  int pendingIndex = 0;

  for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
    if (( br->igmp_delayed_skb[pendingIndex].skb != NULL ) && 
        ( time_before (br->igmp_delayed_skb[pendingIndex].expiryTime, jiffies) ) ){
      kfree_skb(br->igmp_delayed_skb[pendingIndex].skb);
      br->igmp_delayed_skb[pendingIndex].skb = NULL;
      br->igmp_delayed_skb[pendingIndex].expiryTime = 0;
    }
  }    
}


unsigned long br_igmp_get_next_timer_expiry ( struct net_bridge *br, int *found )
{
  int pendingIndex = 0;
  unsigned long earliestTimeout = jiffies + (MCPD_NETLINK_SKB_TIMEOUT_MS*2);

  for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
    if (( br->igmp_delayed_skb[pendingIndex].skb != NULL) && 
        ( time_after (earliestTimeout, br->igmp_delayed_skb[pendingIndex].expiryTime)) ){
      earliestTimeout = br->igmp_delayed_skb[pendingIndex].expiryTime;
      *found = 1;
    }
  }    

  return earliestTimeout;
}


void br_igmp_set_timer( struct net_bridge *br )
{
	struct net_bridge_mc_fdb_entry *mcast_group;
	int                             i;
	/* the largest timeout is BR_IGMP_MEMBERSHIP_TIMEOUT */
	unsigned long                   tstamp = jiffies + (BR_IGMP_MEMBERSHIP_TIMEOUT*HZ*2);
	unsigned int                    found = 0;
	unsigned long                   pendTimeout = br_igmp_get_next_timer_expiry( br, &found );

	if (( br->igmp_snooping == 0 ) && ( found == 0 ))
	{
		del_timer(&br->igmp_timer);
		return;
	}

	if (found) 
	{
		if (time_after(tstamp, pendTimeout) ) {
			tstamp = pendTimeout;
		}
	}

	if ( br->igmp_snooping != 0 ) 
	{
		for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
		{
			struct hlist_node *h_group;
			hlist_for_each_entry(mcast_group, h_group, &br->mc_hash[i], hlist) 
			{
				struct net_bridge_mc_rep_entry *reporter_group;
				list_for_each_entry(reporter_group, &mcast_group->rep_list, list)
				{
					if ( time_after(tstamp, reporter_group->tstamp) )
					{
						tstamp = reporter_group->tstamp;
						found  = 1;
					}
				}
			}
		}
	}
  
	if ( 0 == found )
	{
		del_timer(&br->igmp_timer);
	}
	else
	{
		mod_timer(&br->igmp_timer, (tstamp + TIMER_CHECK_TIMEOUT));
	}

}


static void br_igmp_query_timeout(struct net_bridge *br)
{
	struct net_bridge_mc_fdb_entry *mcast_group;
	int i;
    
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h_group, *n_group;
		hlist_for_each_entry_safe(mcast_group, h_group, n_group, &br->mc_hash[i], hlist) 
		{
			struct net_bridge_mc_rep_entry *reporter_group, *n_reporter;
			list_for_each_entry_safe(reporter_group, n_reporter, &mcast_group->rep_list, list)
			{
				if (time_after_eq(jiffies, reporter_group->tstamp)) 
				{
					br_igmp_mc_fdb_del_entry(br, mcast_group, &reporter_group->rep);
				}            
			}
		}
	}

}

static void br_igmp_timeout (unsigned long ptr)
{
	struct net_bridge *br = (struct net_bridge *) ptr;

	
	/*组播抑制需同时解决lan侧设备report抑制后snooping被老化问题*/
#ifdef CONFIG_ATP_IGMP_DROP_LEAVE_LAN2LAN
	return;
#endif
	spin_lock_bh(&br->mcl_lock);

	br_igmp_query_timeout(br);
	br_igmp_process_timer_check(br);

	br_igmp_set_timer(br);
	spin_unlock_bh(&br->mcl_lock);
}

static struct net_bridge_mc_rep_entry *
                br_igmp_rep_find(const struct net_bridge_mc_fdb_entry *mc_fdb,
                                 const struct in_addr *rep)
{
	struct net_bridge_mc_rep_entry *rep_entry;

	list_for_each_entry(rep_entry, &mc_fdb->rep_list, list)
	{
	    if(rep_entry->rep.s_addr == rep->s_addr)
		return rep_entry;
	}

	return NULL;
}

/* In the case where a reporter has changed ports, this function
   will remove all records pointing to the old port */
void br_igmp_wipe_reporter_for_port (struct net_bridge *br,
                                     struct in_addr *rep, 
                                     u16 oldPort)
{
    int hashIndex = 0;
    struct hlist_node *h = NULL;
    struct hlist_node *n = NULL;
    struct hlist_head *head = NULL;
    struct net_bridge_mc_fdb_entry *mc_fdb;

    spin_lock_bh(&br->mcl_lock);
    for ( ; hashIndex < BR_IGMP_HASH_SIZE ; hashIndex++)
    {
        head = &br->mc_hash[hashIndex];
        hlist_for_each_entry_safe(mc_fdb, h, n, head, hlist)
        {
            if ((mc_fdb->dst->port_no == oldPort) &&
                (br_igmp_rep_find(mc_fdb, rep) != NULL))
            {
                /* The reporter we're looking for has been found
                   in a record pointing to its old port */
                br_igmp_mc_fdb_del_entry (br, mc_fdb, rep);
            }
        }
    }
    br_igmp_set_timer(br);
    spin_unlock_bh(&br->mcl_lock);
}

/* this is called during addition of a snooping entry and requires that 
   mcl_lock is already held */
static int br_mc_fdb_update(struct net_bridge *br, 
                            struct net_bridge_port *prt, 
                            struct in_addr *grp,
                            struct in_addr *rep, 
                            int mode, 
                            struct in_addr *src,
                            struct net_device *from_dev)
{
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	int ret = 0;
	int filt_mode;
	struct hlist_head *head;
	struct hlist_node *h;

	if(mode == SNOOP_IN_ADD)
		filt_mode = MCAST_INCLUDE;
	else
		filt_mode = MCAST_EXCLUDE;
    
	head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	hlist_for_each_entry(dst, h, head, hlist) {
		if (dst->grp.s_addr == grp->s_addr)
		{
			if((src->s_addr == dst->src_entry.src.s_addr) &&
			   (filt_mode == dst->src_entry.filt_mode) && 
			   (dst->from_dev == from_dev) &&
			   (dst->dst == prt))
			{
				/* found entry - update TS */
				struct net_bridge_mc_rep_entry *reporter = br_igmp_rep_find(dst, rep);
				if(reporter == NULL)
				{
					rep_entry = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_ATOMIC);
					if(rep_entry)
					{
						rep_entry->rep.s_addr = rep->s_addr;
						rep_entry->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT*HZ;
						list_add_tail(&rep_entry->list, &dst->rep_list);
						br_igmp_set_timer(br);
					}
				}
				else
				{
					reporter->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT*HZ;
					br_igmp_set_timer(br);
				}
				ret = 1;
			}
		}
	}

	return ret;
}

int br_igmp_process_if_change(struct net_bridge *br, struct net_device *ndev)
{
	struct net_bridge_mc_fdb_entry *dst;
	int i;

	spin_lock_bh(&br->mcl_lock);
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
		{
			if ((NULL == ndev) ||
			    (dst->dst->dev == ndev) ||
			    (dst->from_dev == ndev))
			{
				br_igmp_mc_fdb_del_entry(br, dst, NULL);
			}
		}
	}
	br_igmp_set_timer(br);
	spin_unlock_bh(&br->mcl_lock);

	return 0;
}

int br_igmp_mc_fdb_add(struct net_device *from_dev,
                       int wan_ops,
                       struct net_bridge *br, 
                       struct net_bridge_port *prt, 
                       struct in_addr *grp, 
                       struct in_addr *destGrp, 
                       struct in_addr *rep, 
                       int mode, 
                       uint16_t tci, 
                       struct in_addr *src,
                       int lanppp)
{
	struct net_bridge_mc_fdb_entry *mc_fdb = NULL;
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	struct hlist_head *head = NULL;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	int ret = 1;
#endif

	if(!br || !prt || !grp || !destGrp || !rep || !from_dev)
		return 0;

	if( !br_igmp_control_filter(NULL, destGrp->s_addr) || !br_igmp_control_filter(NULL, grp->s_addr) )
		return 0;

	if(!netdev_path_is_leaf(from_dev))
		return 0;

	if((SNOOP_IN_ADD != mode) && (SNOOP_EX_ADD != mode))
		return 0;
#ifdef CONFIG_SUPPORT_ATP
	mc_fdb = kmem_cache_alloc(br_igmp_mc_fdb_cache, GFP_ATOMIC);
	if ( !mc_fdb )
	{
		return -ENOMEM;
	}
	rep_entry = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_ATOMIC);
	if ( !rep_entry )
	{
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		return -ENOMEM;
	}
#else
	mc_fdb = kmem_cache_alloc(br_igmp_mc_fdb_cache, GFP_KERNEL);
	if ( !mc_fdb )
	{
		return -ENOMEM;
	}
	rep_entry = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_KERNEL);
	if ( !rep_entry )
	{
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		return -ENOMEM;
	}
#endif	

	spin_lock_bh(&br->mcl_lock);
	if (br_mc_fdb_update(br, prt, destGrp, rep, mode, src, from_dev))
	{
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mcl_lock);
		return 0;
	}

	mc_fdb->grp.s_addr = grp->s_addr;
	mc_fdb->destGrp.s_addr = destGrp->s_addr;
	memcpy(&mc_fdb->src_entry, src, sizeof(struct in_addr));
	mc_fdb->src_entry.filt_mode = (mode == SNOOP_IN_ADD) ? MCAST_INCLUDE : MCAST_EXCLUDE;
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
	rep_entry->rep.s_addr = rep->s_addr;
	rep_entry->tstamp = jiffies + BR_IGMP_MEMBERSHIP_TIMEOUT * HZ;
	list_add_tail(&rep_entry->list, &mc_fdb->rep_list);

	head = &br->mc_hash[br_igmp_mc_fdb_hash(destGrp->s_addr)];
	hlist_add_head(&mc_fdb->hlist, head);

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	ret = br_mcast_blog_process(br, (void*)mc_fdb, BR_MCAST_PROTO_IGMP);
	if(ret < 0)
	{
		hlist_del(&mc_fdb->hlist);
#ifdef CONFIG_ATP_BRCM
		if (mc_fdb->from_dev)
		{
			dev_put(mc_fdb->from_dev);
		}
#endif
		kmem_cache_free(br_igmp_mc_fdb_cache, mc_fdb);
		kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
		spin_unlock_bh(&br->mcl_lock);
		return ret;
	}
#endif
	br_igmp_set_timer(br);

	spin_unlock_bh(&br->mcl_lock);

	return 1;
}
EXPORT_SYMBOL(br_igmp_mc_fdb_add);

void br_igmp_mc_fdb_cleanup(struct net_bridge *br)
{
	struct net_bridge_mc_fdb_entry *dst;
	int i;
    
	spin_lock_bh(&br->mcl_lock);
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(dst, h, n, &br->mc_hash[i], hlist) 
		{
			br_igmp_mc_fdb_del_entry(br, dst, NULL);
		}
	}
	br_igmp_set_timer(br);
	spin_unlock_bh(&br->mcl_lock);
}

int br_igmp_mc_fdb_remove(struct net_device *from_dev,
                          struct net_bridge *br, 
                          struct net_bridge_port *prt, 
                          struct in_addr *grp, 
                          struct in_addr *rep, 
                          int mode, 
                          struct in_addr *src)
{
	struct net_bridge_mc_fdb_entry *mc_fdb;
	int filt_mode;
	struct hlist_head *head = NULL;
	struct hlist_node *h, *n;

	//printk("--- remove mc entry ---\n");
	
	if(!br || !prt || !grp|| !rep || !from_dev)
	    return 0;

	if(!br_igmp_control_filter(NULL, grp->s_addr))
		return 0;

	if(!netdev_path_is_leaf(from_dev))
	    return 0;

	if((SNOOP_IN_CLEAR != mode) && (SNOOP_EX_CLEAR != mode))
	   return 0;

	if(mode == SNOOP_IN_CLEAR)
		filt_mode = MCAST_INCLUDE;
	else
		filt_mode = MCAST_EXCLUDE;

	spin_lock_bh(&br->mcl_lock);
	head = &br->mc_hash[br_igmp_mc_fdb_hash(grp->s_addr)];
	 hlist_for_each_entry_safe(mc_fdb, h, n, head, hlist)
	{
		if ((mc_fdb->destGrp.s_addr == grp->s_addr) && 
		    (filt_mode == mc_fdb->src_entry.filt_mode) && 
		    (mc_fdb->src_entry.src.s_addr == src->s_addr) &&
		    (mc_fdb->from_dev == from_dev) &&
		    (mc_fdb->dst == prt)) 
		{
			br_igmp_mc_fdb_del_entry(br, mc_fdb, rep);
		}
	}
	br_igmp_set_timer(br);
	spin_unlock_bh(&br->mcl_lock);
	
	return 0;
}
EXPORT_SYMBOL(br_igmp_mc_fdb_remove);


int br_igmp_mc_forward(struct net_bridge *br, 
                       struct sk_buff *skb, 
                       int forward,
                       int is_routed)
{
	struct net_bridge_mc_fdb_entry *dst;
	int status = 0;
	struct sk_buff *skb2;
	struct net_bridge_port *p, *p_n;
	struct iphdr *pipmcast = NULL;
	struct igmphdr *pigmp = NULL;
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct hlist_head *head = NULL;
	struct hlist_node *h;
	int lanppp;

	br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, &lanppp);
	if ( pipmcast == NULL ) {
		return status;
	}

	if ((pigmp != NULL) &&
	    (br->igmp_proxy || br->igmp_snooping || 
            is_multicast_switching_mode_host_control()))
	{
		/* for bridged WAN service, do not pass any IGMP packets
		   coming from the WAN port to mcpd. Queries can be passed 
		   through for forwarding, other types should be dropped */
		if (skb->dev)
		{
			/*当前博通驱动未设置IFF_WANDEV，使用ATP的WAN判断条件*/
#if defined(CONFIG_BCM_KF_WANDEV)
#ifdef CONFIG_SUPPORT_ATP
			if (( skb->dev->priv_flags & IFF_WANDEV ) || IS_WAN_DEVICE(skb->dev->name))
#else
			if ( skb->dev->priv_flags & IFF_WANDEV )
#endif
			{
				if ( pigmp->type != IGMP_HOST_MEMBERSHIP_QUERY )
				{
					kfree_skb(skb);
					status = 1;
				}
			}
			else
#endif
			{
				rcu_read_lock();
				if(br_port_get_rcu(skb->dev))
				{ 
#ifdef CONFIG_SUPPORT_ATP
					/*codenomicon igmp测试挂死修改:博通snooping处理之前进行必要的合法性校验*/
					if (is_igmp_need_process(skb))
					{
					    br_igmp_process(br, skb);
					}
#else
                  	mcpd_process_skb(br, skb, ETH_P_IP);				  
#endif
				}
				rcu_read_unlock();
			}
		}
		return status;
	}

	if (br_igmp_control_filter(dest, pipmcast->daddr) == 0) {
		return status;
	}

	/* snooping could be disabled and still have manual entries */

	/* drop traffic by default when snooping is enabled
	   in blocking mode */
	if (br->igmp_snooping == SNOOPING_BLOCKING_MODE)
	{
		status = 1;
	}

	spin_lock_bh(&br->mcl_lock);
	head = &br->mc_hash[br_igmp_mc_fdb_hash(pipmcast->daddr)];
	hlist_for_each_entry(dst, h, head, hlist) {
		if (dst->destGrp.s_addr == pipmcast->daddr) {
			/* routed packet will have bridge as dev - cannot match to mc_fdb */
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
					if (dst->from_dev != skb->dev) {
						continue;
					}
				}
				else {
					/* if this is not an L2L mc_fdb entry continue */
					if (dst->from_dev != br->dev) {
						continue;            
					}
				}
#endif
			}

			if((dst->src_entry.filt_mode == MCAST_INCLUDE) && 
				(pipmcast->saddr == dst->src_entry.src.s_addr)) {
				if (!dst->dst->dirty) {
					if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
					{
						spin_unlock_bh(&br->mcl_lock);
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
				if((0 == dst->src_entry.src.s_addr) ||
					(pipmcast->saddr != dst->src_entry.src.s_addr)) {
					if (!dst->dst->dirty) {
						if((skb2 = skb_clone(skb, GFP_ATOMIC)) == NULL)
						{
							spin_unlock_bh(&br->mcl_lock);
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
				else if(pipmcast->saddr == dst->src_entry.src.s_addr) {
					status = 1;
				}
			}
		}
	}
	spin_unlock_bh(&br->mcl_lock);

	if (status) {
		list_for_each_entry_safe(p, p_n, &br->port_list, list) {
			p->dirty = 0;
		}
	}

	if(status)
		kfree_skb(skb);

	return status;
}

void br_igmp_process_admission (t_MCPD_ADMISSION* admit)
{
    int pendingIndex = 0;
    struct net_bridge *br = (struct net_bridge *)admit->bridgePointer;
    struct sk_buff *skb = (struct sk_buff *)admit->skbPointer;

    if ((br == NULL) || (skb == NULL)) {
      printk ("Error %p %p\n", br, skb);
    }

    spin_lock_bh(&br->mcl_lock);

    for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
      printk("Comparing [%d] = %p\n", pendingIndex, br->igmp_delayed_skb[pendingIndex].skb);
      if ( br->igmp_delayed_skb[pendingIndex].skb == skb ) {
        break;
      }
    }

    if (pendingIndex == MCPD_MAX_DELAYED_SKB_COUNT) {
      /* Did not find that skb, may have timed out */
      printk("br_igmp_process_admission no match\n");
      return;
    }

    if (admit->admitted == MCPD_PACKET_ADMITTED_YES) {
      /* send the packet on */
      rcu_read_lock();
      br_flood_forward(br, br->igmp_delayed_skb[pendingIndex].skb, NULL);
      rcu_read_unlock();
    }
    else {
      /* packet was not admitted, free it up */
      kfree_skb(br->igmp_delayed_skb[pendingIndex].skb);
    }
    br->igmp_delayed_skb[pendingIndex].skb = NULL;

    spin_unlock_bh(&br->mcl_lock);
}

void br_igmp_wipe_pending_skbs( void )
{
  struct net_device *dev;

  rcu_read_lock();
  for_each_netdev_rcu(&init_net, dev) {
    if (dev->priv_flags & IFF_EBRIDGE) {
      int pendingIndex = 0;
      struct net_bridge* br = netdev_priv(dev);
      spin_lock_bh(&br->mcl_lock);
      for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
        if ( br->igmp_delayed_skb[pendingIndex].skb != NULL) {
          kfree_skb(br->igmp_delayed_skb[pendingIndex].skb);
          br->igmp_delayed_skb[pendingIndex].skb = NULL;
          br->igmp_delayed_skb[pendingIndex].expiryTime = 0;
        }
      }
      spin_unlock_bh(&br->mcl_lock);
    }
  }
  rcu_read_unlock();
}

void br_igmp_process_device_removal(struct net_device* dev)
{
  if (NULL == dev) {
    return;
  }

  if (dev->priv_flags & IFF_EBRIDGE) {
    /* the removed device is a bridge, clear all pending */
    int pendingIndex = 0;
    struct net_bridge* br = netdev_priv(dev);
    spin_lock_bh(&br->mcl_lock);
    for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
      if (br->igmp_delayed_skb[pendingIndex].skb != NULL) {
        kfree_skb(br->igmp_delayed_skb[pendingIndex].skb);
        br->igmp_delayed_skb[pendingIndex].skb = NULL;
        br->igmp_delayed_skb[pendingIndex].expiryTime = 0;
      }
    }  
    spin_unlock_bh(&br->mcl_lock);
  }
  else {
    /* this is a non bridge device.  We must clear it from all bridges. */
    struct net_device *maybeBridge = NULL;

    rcu_read_lock();
    for_each_netdev_rcu(&init_net, maybeBridge) {
      if (maybeBridge->priv_flags & IFF_EBRIDGE) {
        int pendingIndex = 0;
        struct net_bridge* br = netdev_priv(maybeBridge);
        spin_lock_bh(&br->mcl_lock);
        for ( ; pendingIndex < MCPD_MAX_DELAYED_SKB_COUNT; pendingIndex ++) {
          if ((br->igmp_delayed_skb[pendingIndex].skb != NULL) && ( br->igmp_delayed_skb[pendingIndex].skb->dev == dev )){
            kfree_skb(br->igmp_delayed_skb[pendingIndex].skb);
            br->igmp_delayed_skb[pendingIndex].skb = NULL;
            br->igmp_delayed_skb[pendingIndex].expiryTime = 0;
          }
        }  
        spin_unlock_bh(&br->mcl_lock);
      }
    }
    rcu_read_unlock();
  }
}


int br_igmp_mc_fdb_update_bydev( struct net_bridge *br,
                                 struct net_device *dev,
                                 unsigned int       flushAll)
{
	struct net_bridge_mc_fdb_entry *mc_fdb;
	int i;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	int ret;
#endif

	if(!br || !dev)
		return 0;

	if(!netdev_path_is_leaf(dev))
		return 0;

	spin_lock_bh(&br->mcl_lock);
	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(mc_fdb, h, n, &br->mc_hash[i], hlist) 
		{
			if ((mc_fdb->dst->dev == dev) ||
			    (mc_fdb->from_dev == dev))
			{
				/* do note remove the root entry */
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
				if ((0 == mc_fdb->root) || (1 == flushAll))
				{
					br_igmp_mc_fdb_del_entry(br, mc_fdb, NULL);
				}
				else
				{
					br_mcast_blog_release(BR_MCAST_PROTO_IGMP, (void *)mc_fdb);
					mc_fdb->blog_idx = BLOG_KEY_INVALID;
				}
#else
				if (1 == flushAll)
				{
					br_igmp_mc_fdb_del_entry(br, mc_fdb, NULL);
				}
#endif
			}
		}
	}

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	if (0 == flushAll)
	{
		for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
		{
			struct hlist_node *h, *n;
			hlist_for_each_entry_safe(mc_fdb, h, n, &br->mc_hash[i], hlist) 
			{ 
				if ( (1 == mc_fdb->root) && 
				     ((mc_fdb->dst->dev == dev) ||
				      (mc_fdb->from_dev == dev)) )
				{
					mc_fdb->wan_tci  = 0;
					mc_fdb->num_tags = 0; 
					ret = br_mcast_blog_process(br, (void*)mc_fdb, BR_MCAST_PROTO_IGMP);
					if(ret < 0)
					{
						/* br_mcast_blog_process may return -1 if there are no blog rules
						 * which may be a valid scenario, in which case we delete the
						 * multicast entry.
						 */
						br_igmp_mc_fdb_del_entry(br, mc_fdb, NULL);
						//printk(KERN_DEBUG "%s: Failed to create the blog\n", __FUNCTION__);
					}
				}
			}
		}
	}
#endif   
	br_igmp_set_timer(br);
	spin_unlock_bh(&br->mcl_lock);

	return 0;
}

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
/* This is a support function for vlan/blog processing that requires that 
   br->mcl_lock is already held */
struct net_bridge_mc_fdb_entry *br_igmp_mc_fdb_copy(
                       struct net_bridge *br, 
                       const struct net_bridge_mc_fdb_entry *igmp_fdb)
{
	struct net_bridge_mc_fdb_entry *new_igmp_fdb = NULL;
	struct net_bridge_mc_rep_entry *rep_entry = NULL;
	struct net_bridge_mc_rep_entry *rep_entry_n = NULL;
	int success = 1;
	struct hlist_head *head = NULL;

	new_igmp_fdb = kmem_cache_alloc(br_igmp_mc_fdb_cache, GFP_ATOMIC);
	if (new_igmp_fdb)
	{
		memcpy(new_igmp_fdb, igmp_fdb, sizeof(struct net_bridge_mc_fdb_entry));
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
		new_igmp_fdb->blog_idx = BLOG_KEY_INVALID;
#endif
		new_igmp_fdb->root = 0;
		INIT_LIST_HEAD(&new_igmp_fdb->rep_list);

		list_for_each_entry(rep_entry, &igmp_fdb->rep_list, list) {
			rep_entry_n = kmem_cache_alloc(br_igmp_mc_rep_cache, GFP_ATOMIC);
			if(rep_entry_n)
			{
				memcpy(rep_entry_n, 
				       rep_entry, 
				       sizeof(struct net_bridge_mc_rep_entry));
				list_add_tail(&rep_entry_n->list, &new_igmp_fdb->rep_list);
			}
			else 
			{
				success = 0;
				break;
			}
		}

		if(success)
		{
			head = &br->mc_hash[br_igmp_mc_fdb_hash(igmp_fdb->destGrp.s_addr)];
			hlist_add_head(&new_igmp_fdb->hlist, head);
#ifdef CONFIG_ATP_BRCM
			dev_hold(new_igmp_fdb->from_dev);
#endif
		}
		else
		{
			list_for_each_entry_safe(rep_entry, 
			                         rep_entry_n, &new_igmp_fdb->rep_list, list) {
				list_del(&rep_entry->list);
				kmem_cache_free(br_igmp_mc_rep_cache, rep_entry);
			}
			kmem_cache_free(br_igmp_mc_fdb_cache, new_igmp_fdb);
			new_igmp_fdb = NULL;
		}
	}

	return new_igmp_fdb;
} /* br_igmp_mc_fdb_copy */
#endif

static void *snoop_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct net_device *dev;
	loff_t offs = 0;

	rcu_read_lock();
	for_each_netdev_rcu(&init_net, dev) {
		if((dev->priv_flags & IFF_EBRIDGE) && (*pos == offs)) {
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
	struct net_bridge_mc_fdb_entry *dst;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_mc_rep_entry *rep_entry;
	int first;
	int i;
	int tstamp;

	seq_printf(seq, "igmp snooping %d  proxy %d  lan2lan-snooping %d/%d, rate-limit %dpps, priority %d\n",
	           br->igmp_snooping, 
	           br->igmp_proxy,
	           br->igmp_lan2lan_mc_enable,
	           br_mcast_get_lan2lan_snooping(BR_MCAST_PROTO_IGMP, br),
	           br->igmp_rate_limit,
	           br_mcast_get_pri_queue());
	seq_printf(seq, "bridge device src-dev #tags lan-tci  wan-tci");
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	seq_printf(seq, "    group      mode DNATgroup  source     reporter   timeout Index\n");
#else
	seq_printf(seq, "    group      mode DNATgroup  source     reporter   timeout\n");
#endif

	for (i = 0; i < BR_IGMP_HASH_SIZE; i++) 
	{
		struct hlist_node *h;
		hlist_for_each(h, &br->mc_hash[i]) 
		{
			dst = hlist_entry(h, struct net_bridge_mc_fdb_entry, hlist);
			if(dst)
	{
		seq_printf(seq, "%-6s %-6s %-7s %02d    0x%04x   0x%04x%04x", 
		           br->dev->name, 
		           dst->dst->dev->name, 
		           dst->from_dev->name, 
		           dst->num_tags,
		           ntohs(dst->lan_tci),
		           ((dst->wan_tci >> 16) & 0xFFFF),
		           (dst->wan_tci & 0xFFFF));

		seq_printf(seq, " 0x%08x", htonl(dst->grp.s_addr));

		seq_printf(seq, " %-4s 0x%08x 0x%08x", 
			           (dst->src_entry.filt_mode == MCAST_EXCLUDE) ? 
			           "EX" : "IN",  htonl(dst->destGrp.s_addr), htonl(dst->src_entry.src.s_addr) );

		first = 1;
		list_for_each_entry(rep_entry, &dst->rep_list, list)
		{ 

			if ( 0 == br->igmp_snooping )
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
				seq_printf(seq, " 0x%08x %-7d 0x%08x\n", htonl(rep_entry->rep.s_addr), tstamp, dst->blog_idx);
#else
				seq_printf(seq, " 0x%08x %-7d\n", htonl(rep_entry->rep.s_addr), tstamp);
#endif
				first = 0;
			}
			else
			{
				seq_printf(seq, "%76s 0x%08x %-7d\n", " ", htonl(rep_entry->rep.s_addr), tstamp);
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

static struct file_operations br_igmp_snoop_proc_fops = {
	.owner = THIS_MODULE,
	.open  = snoop_seq_open,
	.read  = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

void br_igmp_snooping_br_init( struct net_bridge *br )
{
	spin_lock_init(&br->mcl_lock);
	br->igmp_lan2lan_mc_enable = BR_MC_LAN2LAN_STATUS_DEFAULT;
	setup_timer(&br->igmp_timer, br_igmp_timeout, (unsigned long)br);   
}

void br_igmp_snooping_br_fini( struct net_bridge *br )
{
	del_timer_sync(&br->igmp_timer);
}

int __init br_igmp_snooping_init(void)
{
	br_igmp_entry = proc_create("igmp_snooping", 0, init_net.proc_net,
			   &br_igmp_snoop_proc_fops);

	if(!br_igmp_entry) {
		printk("error while creating igmp_snooping proc\n");
        return -ENOMEM;
	}

	br_igmp_mc_fdb_cache = kmem_cache_create("bridge_igmp_mc_fdb_cache",
                            sizeof(struct net_bridge_mc_fdb_entry),
                            0,
                            SLAB_HWCACHE_ALIGN, NULL);
    if (!br_igmp_mc_fdb_cache)
		return -ENOMEM;

    br_igmp_mc_rep_cache = kmem_cache_create("bridge_igmp_mc_rep_cache",
                            sizeof(struct net_bridge_mc_rep_entry),
                            0,
                            SLAB_HWCACHE_ALIGN, NULL);
    if (!br_igmp_mc_rep_cache)
    {
        kmem_cache_destroy(br_igmp_mc_fdb_cache);
		return -ENOMEM;
    }

	get_random_bytes(&br_igmp_mc_fdb_salt, sizeof(br_igmp_mc_fdb_salt));

#ifdef CONFIG_SUPPORT_ATP
    br_snooping_init();
#endif
    return 0;
}

void br_igmp_snooping_fini(void)
{
	kmem_cache_destroy(br_igmp_mc_fdb_cache);
	kmem_cache_destroy(br_igmp_mc_rep_cache);

    return;
}
#endif /* CONFIG_BR_IGMP_SNOOP */
#endif /* CONFIG_BCM_KF_IGMP */
