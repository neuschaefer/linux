/*
 *	Forwarding database
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
#include <linux/init.h>
#include <linux/rculist.h>
#include <linux/spinlock.h>
#include <linux/times.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/jhash.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <asm/unaligned.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include "br_igmp.h"
#include <linux/blog.h>
#endif
#if defined(CONFIG_BCM_KF_LOG)
#include <linux/bcm_log.h>
#endif

#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE)
#include "br_fp.h"
#include "br_fp_hooks.h"
#endif /* CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE */
#endif /* CONFIG_BCM_RDPA || CONFIG_BCM_RDPA_MODULE */
#endif /* CONFIG_BCM_KF_RUNNER */

#if defined(CONFIG_BCM_KF_WL)
#include <linux/module.h>
int (*fdb_check_expired_wl_hook)(unsigned char *addr) = NULL;
int (*fdb_check_expired_dhd_hook)(unsigned char *addr) = NULL;
#endif

static struct kmem_cache *br_fdb_cache __read_mostly;
static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		      const unsigned char *addr);
static void fdb_notify(struct net_bridge *br,
		       const struct net_bridge_fdb_entry *, int);

static u32 fdb_salt __read_mostly;

int __init br_fdb_init(void)
{
	br_fdb_cache = kmem_cache_create("bridge_fdb_cache",
					 sizeof(struct net_bridge_fdb_entry),
					 0,
					 SLAB_HWCACHE_ALIGN, NULL);
	if (!br_fdb_cache)
		return -ENOMEM;

	get_random_bytes(&fdb_salt, sizeof(fdb_salt));
	return 0;
}

void br_fdb_fini(void)
{
	kmem_cache_destroy(br_fdb_cache);
}

/* if topology_changing then use forward_delay (default 15 sec)
 * otherwise keep longer (default 5 minutes)
 */
static inline unsigned long hold_time(const struct net_bridge *br)
{
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	/* Seems one timer constant in bridge code can serve several different purposes. As we use forward_delay=0,
	if the code left unchanged, every entry in fdb will expire immidately after a topology change and every packet
	will flood the local ports for a period of bridge_max_age. This will result in low throughput after boot up. 
	So we decoulpe this timer from forward_delay. */
	return br->topology_change ? (15*HZ) : br->ageing_time;
#else
	return br->topology_change ? br->forward_delay : br->ageing_time;
#endif
}

static inline int has_expired(const struct net_bridge *br,
				  const struct net_bridge_fdb_entry *fdb)
{
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	if (fdb->fdb_key != BLOG_KEY_NONE)
		blog_query(QUERY_BRIDGEFDB, (void*)fdb, fdb->fdb_key, 0, 0);
#endif
	return !fdb->is_static &&
		time_before_eq(fdb->updated + hold_time(br), jiffies);
}

static inline int br_mac_hash(const unsigned char *mac)
{
	/* use 1 byte of OUI cnd 3 bytes of NIC */
	u32 key = get_unaligned((u32 *)(mac + 2));
	return jhash_1word(key, fdb_salt) & (BR_HASH_SIZE - 1);
}

static void fdb_rcu_free(struct rcu_head *head)
{
	struct net_bridge_fdb_entry *ent
		= container_of(head, struct net_bridge_fdb_entry, rcu);
	kmem_cache_free(br_fdb_cache, ent);
}

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
static int fdb_limit_port_check(struct net_bridge *br, struct net_bridge_port *port)
{
	if (port->max_port_fdb_entries != 0) {
		/* Check per port max limit */
		if ((port->num_port_fdb_entries+1) > port->max_port_fdb_entries)
			return -1;
	}
	return 0;    
}

static int fdb_limit_check(struct net_bridge *br, struct net_bridge_port *port)
{
	if (br->max_br_fdb_entries != 0) {
		/* Check per br limit */
		if ((br->used_br_fdb_entries+1) > br->max_br_fdb_entries)
			return -1;
	}

	return fdb_limit_port_check(br, port);
}

static void fdb_limit_update(struct net_bridge *br, struct net_bridge_port *port, int isAdd)
{
	if (isAdd) {
		port->num_port_fdb_entries++;
		if (port->num_port_fdb_entries > port->min_port_fdb_entries)
			br->used_br_fdb_entries++;
	}
	else {
		BUG_ON(!port->num_port_fdb_entries);
		port->num_port_fdb_entries--;
		if (port->num_port_fdb_entries >= port->min_port_fdb_entries)
			br->used_br_fdb_entries--;
	}        	
}
#endif

static void fdb_delete(struct net_bridge *br, struct net_bridge_fdb_entry *f)
{
#if defined(CONFIG_BCM_KF_NETFILTER)
	br->num_fdb_entries--;
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
	if (f->is_local == 0) {
		fdb_limit_update(br, f->dst, 0);
	}
#endif

#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE)
	if (!f->is_local) /* Do not remove local MAC to the Runner  */
		br_fp_hook(BR_FP_FDB_REMOVE, f, NULL);
#endif /* CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE */
#endif /* CONFIG_BCM_RDPA || CONFIG_BCM_RDPA_MODULE */
#endif /* CONFIG_BCM_KF_RUNNER */
	hlist_del_rcu(&f->hlist);
	fdb_notify(br, f, RTM_DELNEIGH);
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	if (f->fdb_key != BLOG_KEY_NONE)
		blog_notify(DESTROY_BRIDGEFDB, (void*)f, f->fdb_key, 0);
#endif
	call_rcu(&f->rcu, fdb_rcu_free);
}

void br_fdb_changeaddr(struct net_bridge_port *p, const unsigned char *newaddr)
{
	struct net_bridge *br = p->br;
	int i;
	
	spin_lock_bh(&br->hash_lock);

	/* Search all chains since old address/hash is unknown */
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h;
		hlist_for_each(h, &br->hash[i]) {
			struct net_bridge_fdb_entry *f;

			f = hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst == p && f->is_local) {
				/* maybe another port has same hw addr? */
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p && 
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto insert;
					}
				}

				/* delete old one */
				fdb_delete(br, f);
				goto insert;
			}
		}
	}
 insert:
	/* insert new address,  may fail if invalid address or dup. */
	fdb_insert(br, p, newaddr);

	spin_unlock_bh(&br->hash_lock);
}

void br_fdb_change_mac_address(struct net_bridge *br, const u8 *newaddr)
{
	struct net_bridge_fdb_entry *f;

	/* If old entry was unassociated with any port, then delete it. */
	f = __br_fdb_get(br, br->dev->dev_addr);
	if (f && f->is_local && !f->dst)
		fdb_delete(br, f);

	fdb_insert(br, NULL, newaddr);
}

void br_fdb_cleanup(unsigned long _data)
{
	struct net_bridge *br = (struct net_bridge *)_data;
	unsigned long delay = hold_time(br);
	unsigned long next_timer = jiffies + br->ageing_time;
	int i;

	spin_lock(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;

		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			unsigned long this_timer;
			if (f->is_static)
				continue;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
			if (f->fdb_key != BLOG_KEY_NONE)
				blog_query(QUERY_BRIDGEFDB, (void*)f, f->fdb_key, 0, 0);
#endif
			this_timer = f->updated + delay;
			if (time_before_eq(this_timer, jiffies))
#if defined(CONFIG_BCM_KF_RUNNER) || defined(CONFIG_BCM_KF_WL)
			{
				int flag = 0;

#if (defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)) && (defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE))
				br_fp_hook(BR_FP_FDB_CHECK_AGE, f, &flag);
#endif /* CONFIG_BCM_RDPA && CONFIG_BCM_RDPA_BRIDGE && CONFIG_BCM_RDPA_BRIDGE_MODULE */
				if (flag
#if defined(CONFIG_BCM_KF_WL)
				    || (fdb_check_expired_wl_hook && (fdb_check_expired_wl_hook(f->addr.addr) == 0))
				    || (fdb_check_expired_dhd_hook && (fdb_check_expired_dhd_hook(f->addr.addr) == 0))
#endif
				    )
				{
					f->updated = jiffies;
				}
				else
					fdb_delete(br, f);
			}
#else
				fdb_delete(br, f);
#endif
			else if (time_before(this_timer, next_timer))
				next_timer = this_timer;
		}
	}
	spin_unlock(&br->hash_lock);

	mod_timer(&br->gc_timer, round_jiffies_up(next_timer));
}

/* Completely flush all dynamic entries in forwarding database.*/
void br_fdb_flush(struct net_bridge *br)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct net_bridge_fdb_entry *f;
		struct hlist_node *h, *n;
		hlist_for_each_entry_safe(f, h, n, &br->hash[i], hlist) {
			if (!f->is_static)
#if defined(CONFIG_BCM_KF_RUNNER) && (defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)) && (defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE))
			{
				int flag = 0;

				br_fp_hook(BR_FP_FDB_CHECK_AGE, f, &flag);
				if (flag) {
					f->updated = jiffies;
				}
				else
				{
					fdb_delete(br, f);
				}
			}
#else /* CONFIG_BCM_KF_RUNNER && CONFIG_BCM_RUNNER && (CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE) */
				fdb_delete(br, f);
#endif /* CONFIG_BCM_KF_RUNNER && CONFIG_BCM_RUNNER && (CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE) */

		}
	}
	spin_unlock_bh(&br->hash_lock);
}

/* Flush all entries referring to a specific port.
 * if do_all is set also flush static entries
 */
void br_fdb_delete_by_port(struct net_bridge *br,
			   const struct net_bridge_port *p,
			   int do_all)
{
	int i;

	spin_lock_bh(&br->hash_lock);
	for (i = 0; i < BR_HASH_SIZE; i++) {
		struct hlist_node *h, *g;
		
		hlist_for_each_safe(h, g, &br->hash[i]) {
			struct net_bridge_fdb_entry *f
				= hlist_entry(h, struct net_bridge_fdb_entry, hlist);
			if (f->dst != p) 
				continue;

#if !defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
			if (f->is_static && !do_all)
				continue;
#else
			/* do_all - 0: only delete dynamic entries
						1: delete all entries
						2: only delete static entries */

			if (f->is_static && (do_all == 0))
				continue;
			else if (!f->is_static && (do_all == 2))
				continue;            
#endif
			
			/*
			 * if multiple ports all have the same device address
			 * then when one port is deleted, assign
			 * the local entry to other port
			 */
			if (f->is_local) {
				struct net_bridge_port *op;
				list_for_each_entry(op, &br->port_list, list) {
					if (op != p && 
					    !compare_ether_addr(op->dev->dev_addr,
								f->addr.addr)) {
						f->dst = op;
						goto skip_delete;
					}
				}
			}

			fdb_delete(br, f);
		skip_delete: ;
		}
	}
	spin_unlock_bh(&br->hash_lock);
}

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
int br_get_fdb_limit(struct net_bridge *br, 
						const struct net_bridge_port *p,
						int is_min)
{
	if((br == NULL) && (p == NULL))
		return -EINVAL;

	if(br != NULL) {
		if(is_min) 
			return br->used_br_fdb_entries;
		else
			return br->max_br_fdb_entries;
	}
	else { 
		if(is_min)
			return p->min_port_fdb_entries;
		else
			return p->max_port_fdb_entries;
	}
}
/* Set FDB limit
lmtType 0: Bridge limit
			1: Port limit
*/
int br_set_fdb_limit(struct net_bridge *br, 
						struct net_bridge_port *p,
						int lmt_type,
						int is_min,
						int fdb_limit)
{
	int new_used_fdb;
	
	if((br == NULL) || ((p == NULL) && lmt_type))
		return -EINVAL;

	if(fdb_limit == 0) {
		/* Disable limit */
		if(lmt_type == 0) {
			br->max_br_fdb_entries = 0;
		}
		else if(is_min) {
			if (p->num_port_fdb_entries < p->min_port_fdb_entries) {
				new_used_fdb = br->used_br_fdb_entries - p->min_port_fdb_entries;
				new_used_fdb += p->num_port_fdb_entries;
				br->used_br_fdb_entries = new_used_fdb;
			}
			p->min_port_fdb_entries = 0;
		}
		else {
			p->max_port_fdb_entries = 0;
		}            
	}
	else {        
		if(lmt_type == 0) {
			if(br->used_br_fdb_entries > fdb_limit) 
				return -EINVAL;
			br->max_br_fdb_entries = fdb_limit;
		}
		else if(is_min) {
			new_used_fdb = max(p->num_port_fdb_entries, p->min_port_fdb_entries);
			new_used_fdb = br->used_br_fdb_entries - new_used_fdb;
			new_used_fdb += max(p->num_port_fdb_entries, fdb_limit);
			if ( (br->max_br_fdb_entries != 0) &&
				(new_used_fdb > br->max_br_fdb_entries) )
				return -EINVAL;
				
			p->min_port_fdb_entries = fdb_limit;   
			br->used_br_fdb_entries = new_used_fdb;
		}
		else {
			if(p->num_port_fdb_entries > fdb_limit)
				return -EINVAL;
			p->max_port_fdb_entries = fdb_limit;
		}
	}
	return 0;
}
#endif

/* No locking or refcounting, assumes caller has rcu_read_lock */
struct net_bridge_fdb_entry *__br_fdb_get(struct net_bridge *br,
					  const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, &br->hash[br_mac_hash(addr)], hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr)) {
			if (unlikely(has_expired(br, fdb)))
				break;
			return fdb;
		}
	}

	return NULL;
}

#if IS_ENABLED(CONFIG_ATM_LANE)
/* Interface used by ATM LANE hook to test
 * if an addr is on some other bridge port */
int br_fdb_test_addr(struct net_device *dev, unsigned char *addr)
{
	struct net_bridge_fdb_entry *fdb;
	struct net_bridge_port *port;
	int ret;

	rcu_read_lock();
	port = br_port_get_rcu(dev);
	if (!port)
		ret = 0;
	else {
		fdb = __br_fdb_get(port->br, addr);
		ret = fdb && fdb->dst && fdb->dst->dev != dev &&
			fdb->dst->state == BR_STATE_FORWARDING;
	}
	rcu_read_unlock();

	return ret;
}
#endif /* CONFIG_ATM_LANE */

/*
 * Fill buffer with forwarding table records in 
 * the API format.
 */
int br_fdb_fillbuf(struct net_bridge *br, void *buf,
		   unsigned long maxnum, unsigned long skip)
{
	struct __fdb_entry *fe = buf;
	int i, num = 0;
	struct hlist_node *h;
	struct net_bridge_fdb_entry *f;

	memset(buf, 0, maxnum*sizeof(struct __fdb_entry));

	rcu_read_lock();
	for (i = 0; i < BR_HASH_SIZE; i++) {
		hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist) {
			if (num >= maxnum)
				goto out;

			if (has_expired(br, f)) 
				continue;

			/* ignore pseudo entry for local MAC address */
			if (!f->dst)
				continue;

			if (skip) {
				--skip;
				continue;
			}

			/* convert from internal format to API */
			memcpy(fe->mac_addr, f->addr.addr, ETH_ALEN);

			/* due to ABI compat need to split into hi/lo */
			fe->port_no = f->dst->port_no;
			fe->port_hi = f->dst->port_no >> 8;

			fe->is_local = f->is_local;
			if (!f->is_static)
				fe->ageing_timer_value = jiffies_to_clock_t(jiffies - f->updated);
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
			fe->vid = f->vid;
#endif
			++fe;
			++num;
		}
	}

 out:
	rcu_read_unlock();

	return num;
}

static struct net_bridge_fdb_entry *fdb_find(struct hlist_head *head,
					     const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry(fdb, h, head, hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr))
			return fdb;
	}
	return NULL;
}

static struct net_bridge_fdb_entry *fdb_find_rcu(struct hlist_head *head,
						    const unsigned char *addr)
{
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;

	hlist_for_each_entry_rcu(fdb, h, head, hlist) {
		if (!compare_ether_addr(fdb->addr.addr, addr))
			return fdb;
	}
	return NULL;
}

#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
static struct net_bridge_fdb_entry *fdb_create(struct net_bridge *br, 
					       struct hlist_head *head,
					       struct net_bridge_port *source,
					       const unsigned char *addr,
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
					       unsigned int vid,
#endif
					       int is_local,
					       int is_static)
#else
static struct net_bridge_fdb_entry *fdb_create(struct hlist_head *head,
					       struct net_bridge_port *source,
					       const unsigned char *addr)
#endif /* CONFIG_BCM_KF_BRIDGE_STATIC_FDB */
{
	struct net_bridge_fdb_entry *fdb;

#if defined(CONFIG_BCM_KF_NETFILTER)
	if (br->num_fdb_entries >= BR_MAX_FDB_ENTRIES)
		return NULL;

	/* some users want to always flood. */
	if (hold_time(br) == 0 && !is_static)
		return NULL;
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
	if (!is_local && (fdb_limit_check(br, source) != 0))
		return NULL;
#endif

	fdb = kmem_cache_alloc(br_fdb_cache, GFP_ATOMIC);
	if (fdb) {
		memcpy(fdb->addr.addr, addr, ETH_ALEN);
		fdb->dst = source;
#if !defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
		fdb->is_local = 0;
		fdb->is_static = 0;
#else
		fdb->is_static = is_static;
		fdb->is_local = is_local;
#endif
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
		fdb->fdb_key = BLOG_KEY_NONE;
#endif
#if defined (CONFIG_BCM_KF_NETFILTER)
		br->num_fdb_entries++;
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
		if (!is_local) {
			fdb_limit_update(br, source, 1);
		}
#endif
		fdb->updated = fdb->used = jiffies;
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
		fdb->vid = vid;
#endif

#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE)
		if (!is_local) /* Do not add local MAC to the Runner  */
			br_fp_hook(BR_FP_FDB_ADD, fdb, NULL);
#endif /* CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE */
#endif /* CONFIG_BCM_RUNNER */
#endif /* CONFIG_BCM_KF_RUNNER */
		hlist_add_head_rcu(&fdb->hlist, head);
	}
	return fdb;
}

#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
/* called with RTNL */
static int fdb_adddel_static(struct net_bridge *br,
                             struct net_bridge_port *source,
                             const unsigned char *addr, 
                             int addEntry)
{
	struct hlist_head *head;
	struct net_bridge_fdb_entry *fdb;

	if (!is_valid_ether_addr(addr))
		return -EINVAL;

	head = &br->hash[br_mac_hash(addr)];
	fdb = fdb_find(head, addr);
	if (fdb)
	{
		/* if the entry exists and it is not static then we will delete it
		   and then add it back as static. If we are not adding an entry
		   then just delete it */
		if ( (0 == addEntry) || (0 == fdb->is_static) )
		{
			fdb_delete(br, fdb);
		}
		else
		{ /* add same static mac, do nothing */
			return 0;
		}
	}
   
	if ( 1 == addEntry )
	{
		struct net_bridge_fdb_entry * fdb;
#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
		fdb = fdb_create(br, head, source, addr, VLAN_N_VID, 0, 1);
#else
		fdb = fdb_create(br, head, source, addr, 0, 1);
#endif
#else
		fdb = fdb_create(head, source, addr);
#endif
		if (!fdb)
			return -ENOMEM;
	}

	return 0;
}
#endif



static int fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	if (!is_valid_ether_addr(addr))
		return -EINVAL;

	fdb = fdb_find(head, addr);
	if (fdb) {
		/* it is okay to have multiple ports with same
		 * address, just use the first one.
				 */
		if (fdb->is_local)
					return 0;
		br_warn(br, "adding interface %s with same address "
				       "as a received packet\n",
				       source->dev->name);
		fdb_delete(br, fdb);
			}

#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
	fdb = fdb_create(br, head, source, addr, VLAN_N_VID, 1, 1);
#else
	fdb = fdb_create(br, head, source, addr, 1, 1);
#endif
#else
	fdb = fdb_create(head, source, addr);
#endif
	if (!fdb)
		return -ENOMEM;

	fdb->is_local = fdb->is_static = 1;
	fdb_notify(br, fdb, RTM_NEWNEIGH);
	return 0;
}

/* Add entry for local address of interface */
int br_fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr)
{
	int ret;

	spin_lock_bh(&br->hash_lock);
	ret = fdb_insert(br, source, addr);
	spin_unlock_bh(&br->hash_lock);
	return ret;
}
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr, const unsigned int vid)
#else
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr)
#endif
{
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	/* some users want to always flood. */
	if (hold_time(br) == 0)
		return;

	/* ignore packets unless we are using this port */
	if (!(source->state == BR_STATE_LEARNING ||
	      source->state == BR_STATE_FORWARDING))
		return;

	fdb = fdb_find_rcu(head, addr);
	if (likely(fdb)) {
		/* attempt to update an entry for a local interface */
		if (unlikely(fdb->is_local)) {
			if (net_ratelimit())
				br_warn(br, "received packet on %s with "
				       " own address as source address\n",
				       source->dev->name);
		} else {
#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE)
			struct net_bridge_port *fdb_dst = fdb->dst;
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
			unsigned int fdb_vid = fdb->vid;
#endif /* CONFIG_BCM_KF_VLAN_AGGREGATION && CONFIG_BCM_VLAN_AGGREGATION */
#endif /* CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE */
#endif /* CONFIG_BCM_RUNNER */
#endif /* CONFIG_BCM_KF_RUNNER */

#if defined (CONFIG_BCM_KF_NETFILTER) || (defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT))
			/* In case of MAC move - let ethernet driver clear switch ARL */
			if (fdb->dst && fdb->dst->port_no != source->port_no) {
#if defined (CONFIG_BCM_KF_NETFILTER)
				bcmFun_t *ethswClearArlFun;
#endif
#if defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT)
				/* Check can be learned on new port */
				if (fdb_limit_port_check(br, source) != 0)
					return;
				/* Modify both of old and new port counter */
				fdb_limit_update(br, fdb->dst, 0);
				fdb_limit_update(br, source, 1);
#endif /* CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT && CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT */
#if defined (CONFIG_BCM_KF_NETFILTER)
				/* Get the switch clear ARL function pointer */
				ethswClearArlFun =  bcmFun_get(BCM_FUN_IN_ENET_CLEAR_ARL_ENTRY);
				if ( ethswClearArlFun ) {
					ethswClearArlFun((void*)addr);
				}
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
				/* Also flush the associated entries in accelerators */
				if (fdb->fdb_key != BLOG_KEY_NONE)
					blog_notify(DESTROY_BRIDGEFDB, (void*)fdb, fdb->fdb_key, 0);
#endif
#endif
			}
#endif /* CONFIG_BCM_KF_NETFILTER || (CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT && CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT */
			/* fastpath: update of existing entry */
			fdb->dst = source;
			fdb->updated = jiffies;
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
			fdb->vid = vid;
#endif
#if defined(CONFIG_BCM_KF_RUNNER)
#if defined(CONFIG_BCM_RDPA) || defined(CONFIG_BCM_RDPA_MODULE)
#if defined(CONFIG_BCM_RDPA_BRIDGE) || defined(CONFIG_BCM_RDPA_BRIDGE_MODULE)
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
			/*  Do not update FastPath if the the source still == dst and vid is same */
			if (fdb_dst != source || fdb_vid != vid)
				br_fp_hook(BR_FP_FDB_MODIFY, fdb, NULL);
#else
			/*  Do not update FastPath if the the source still == dst */
			if (fdb_dst != source)
				br_fp_hook(BR_FP_FDB_MODIFY, fdb, NULL);
#endif /* CONFIG_BCM_KF_VLAN_AGGREGATION && CONFIG_BCM_VLAN_AGGREGATION */
#endif /* CONFIG_BCM_RDPA_BRIDGE || CONFIG_BCM_RDPA_BRIDGE_MODULE */
#endif /* CONFIG_BCM_RUNNER */
#endif /* CONFIG_BCM_KF_RUNNER */
		}
	} else {
		spin_lock(&br->hash_lock);
		if (likely(!fdb_find(head, addr))) {
#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
			fdb = fdb_create(br, head, source, addr, vid, 0, 0);
#else
			fdb = fdb_create(br, head, source, addr, 0, 0);
#endif
#else
			fdb = fdb_create(head, source, addr);
#endif
			if (fdb)
				fdb_notify(br, fdb, RTM_NEWNEIGH);
		}
		/* else  we lose race and someone else inserts
		 * it first, don't bother updating
		 */
		spin_unlock(&br->hash_lock);
	}
}

#if defined(CONFIG_BCM_KF_NETFILTER)
extern void br_fdb_refresh( struct net_bridge_fdb_entry *fdb );
void br_fdb_refresh( struct net_bridge_fdb_entry *fdb )
{
	fdb->updated = jiffies;
	return;
}
#endif


#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
int br_fdb_adddel_static(struct net_bridge *br, struct net_bridge_port *source,
                         const unsigned char *addr, int bInsert)
{
	int ret = 0;

	spin_lock_bh(&br->hash_lock);

	ret = fdb_adddel_static(br, source, addr, bInsert);

	spin_unlock_bh(&br->hash_lock);
   
	return ret;
}
#endif


static int fdb_to_nud(const struct net_bridge_fdb_entry *fdb)
{
	if (fdb->is_local)
		return NUD_PERMANENT;
	else if (fdb->is_static)
		return NUD_NOARP;
	else if (has_expired(fdb->dst->br, fdb))
		return NUD_STALE;
	else
		return NUD_REACHABLE;
}

static int fdb_fill_info(struct sk_buff *skb, const struct net_bridge *br,
			 const struct net_bridge_fdb_entry *fdb,
			 u32 pid, u32 seq, int type, unsigned int flags)
{
	unsigned long now = jiffies;
	struct nda_cacheinfo ci;
	struct nlmsghdr *nlh;
	struct ndmsg *ndm;

	nlh = nlmsg_put(skb, pid, seq, type, sizeof(*ndm), flags);
	if (nlh == NULL)
		return -EMSGSIZE;

	ndm = nlmsg_data(nlh);
	ndm->ndm_family	 = AF_BRIDGE;
	ndm->ndm_pad1    = 0;
	ndm->ndm_pad2    = 0;
	ndm->ndm_flags	 = 0;
	ndm->ndm_type	 = 0;
	ndm->ndm_ifindex = fdb->dst ? fdb->dst->dev->ifindex : br->dev->ifindex;
	ndm->ndm_state   = fdb_to_nud(fdb);

	NLA_PUT(skb, NDA_LLADDR, ETH_ALEN, &fdb->addr);

	ci.ndm_used	 = jiffies_to_clock_t(now - fdb->used);
	ci.ndm_confirmed = 0;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	if (fdb->fdb_key != BLOG_KEY_NONE)
		blog_query(QUERY_BRIDGEFDB, (void*)fdb, fdb->fdb_key, 0, 0);
#endif
	ci.ndm_updated	 = jiffies_to_clock_t(now - fdb->updated);
	ci.ndm_refcnt	 = 0;
	NLA_PUT(skb, NDA_CACHEINFO, sizeof(ci), &ci);

	return nlmsg_end(skb, nlh);

nla_put_failure:
	nlmsg_cancel(skb, nlh);
	return -EMSGSIZE;
}

static inline size_t fdb_nlmsg_size(void)
{
	return NLMSG_ALIGN(sizeof(struct ndmsg))
		+ nla_total_size(ETH_ALEN) /* NDA_LLADDR */
		+ nla_total_size(sizeof(struct nda_cacheinfo));
}

static void fdb_notify(struct net_bridge *br,
		       const struct net_bridge_fdb_entry *fdb, int type)
{
	struct net *net = dev_net(br->dev);
	struct sk_buff *skb;
	int err = -ENOBUFS;

	skb = nlmsg_new(fdb_nlmsg_size(), GFP_ATOMIC);
	if (skb == NULL)
		goto errout;

	err = fdb_fill_info(skb, br, fdb, 0, 0, type, 0);
	if (err < 0) {
		/* -EMSGSIZE implies BUG in fdb_nlmsg_size() */
		WARN_ON(err == -EMSGSIZE);
		kfree_skb(skb);
		goto errout;
	}
	rtnl_notify(skb, net, 0, RTNLGRP_NEIGH, NULL, GFP_ATOMIC);
	return;
errout:
	if (err < 0)
		rtnl_set_sk_err(net, RTNLGRP_NEIGH, err);
}

/* Dump information about entries, in response to GETNEIGH */
int br_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb)
{
	struct net *net = sock_net(skb->sk);
	struct net_device *dev;
	int idx = 0;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		struct net_bridge *br = netdev_priv(dev);
		int i;

		if (!(dev->priv_flags & IFF_EBRIDGE))
			continue;

		for (i = 0; i < BR_HASH_SIZE; i++) {
			struct hlist_node *h;
			struct net_bridge_fdb_entry *f;

			hlist_for_each_entry_rcu(f, h, &br->hash[i], hlist) {
				if (idx < cb->args[0])
					goto skip;

				if (fdb_fill_info(skb, br, f,
						  NETLINK_CB(cb->skb).pid,
						  cb->nlh->nlmsg_seq,
						  RTM_NEWNEIGH,
						  NLM_F_MULTI) < 0)
					break;
skip:
				++idx;
			}
		}
	}
	rcu_read_unlock();

	cb->args[0] = idx;

	return skb->len;
}

/* Update (create or replace) forwarding database entry */
static int fdb_add_entry(struct net_bridge_port *source, const __u8 *addr,
			 __u16 state, __u16 flags)
{
	struct net_bridge *br = source->br;
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	fdb = fdb_find(head, addr);
	if (fdb == NULL) {
		if (!(flags & NLM_F_CREATE))
			return -ENOENT;

#if defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
		fdb = fdb_create(br, head, source, addr, VLAN_N_VID, 0, 0);
#else
		fdb = fdb_create(br, head, source, addr, 0, 0);
#endif
#else
		fdb = fdb_create(head, source, addr);
#endif
		if (!fdb)
			return -ENOMEM;
		fdb_notify(br, fdb, RTM_NEWNEIGH);
	} else {
		if (flags & NLM_F_EXCL)
			return -EEXIST;
	}

	if (fdb_to_nud(fdb) != state) {
	if (state & NUD_PERMANENT)
		fdb->is_local = fdb->is_static = 1;
		else if (state & NUD_NOARP) {
			fdb->is_local = 0;
		fdb->is_static = 1;
		} else
			fdb->is_local = fdb->is_static = 0;

		fdb->updated = fdb->used = jiffies;
		fdb_notify(br, fdb, RTM_NEWNEIGH);
	}

	return 0;
}

/* Add new permanent fdb entry with RTM_NEWNEIGH */
int br_fdb_add(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
{
	struct net *net = sock_net(skb->sk);
	struct ndmsg *ndm;
	struct nlattr *tb[NDA_MAX+1];
	struct net_device *dev;
	struct net_bridge_port *p;
	const __u8 *addr;
	int err;

	ASSERT_RTNL();
	err = nlmsg_parse(nlh, sizeof(*ndm), tb, NDA_MAX, NULL);
	if (err < 0)
		return err;

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_ifindex == 0) {
		pr_info("bridge: RTM_NEWNEIGH with invalid ifindex\n");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, ndm->ndm_ifindex);
	if (dev == NULL) {
		pr_info("bridge: RTM_NEWNEIGH with unknown ifindex\n");
		return -ENODEV;
	}

	if (!tb[NDA_LLADDR] || nla_len(tb[NDA_LLADDR]) != ETH_ALEN) {
		pr_info("bridge: RTM_NEWNEIGH with invalid address\n");
		return -EINVAL;
	}

	addr = nla_data(tb[NDA_LLADDR]);
	if (!is_valid_ether_addr(addr)) {
		pr_info("bridge: RTM_NEWNEIGH with invalid ether address\n");
		return -EINVAL;
	}

	if (!(ndm->ndm_state & (NUD_PERMANENT|NUD_NOARP|NUD_REACHABLE))) {
		pr_info("bridge: RTM_NEWNEIGH with invalid state %#x\n", ndm->ndm_state);
		return -EINVAL;
	}

	p = br_port_get_rtnl(dev);
	if (p == NULL) {
		pr_info("bridge: RTM_NEWNEIGH %s not a bridge port\n",
			dev->name);
		return -EINVAL;
	}

	if (ndm->ndm_flags & NTF_USE) {
		rcu_read_lock();
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
		br_fdb_update(p->br, p, addr, VLAN_N_VID);
#else
		br_fdb_update(p->br, p, addr);
#endif
		rcu_read_unlock();
	} else {
	spin_lock_bh(&p->br->hash_lock);
	err = fdb_add_entry(p, addr, ndm->ndm_state, nlh->nlmsg_flags);
	spin_unlock_bh(&p->br->hash_lock);
	}

	return err;
}

static int fdb_delete_by_addr(struct net_bridge_port *p, const u8 *addr)
{
	struct net_bridge *br = p->br;
	struct hlist_head *head = &br->hash[br_mac_hash(addr)];
	struct net_bridge_fdb_entry *fdb;

	fdb = fdb_find(head, addr);
	if (!fdb)
		return -ENOENT;

	fdb_delete(p->br, fdb);
	return 0;
}

/* Remove neighbor entry with RTM_DELNEIGH */
int br_fdb_delete(struct sk_buff *skb, struct nlmsghdr *nlh, void *arg)
{
	struct net *net = sock_net(skb->sk);
	struct ndmsg *ndm;
	struct net_bridge_port *p;
	struct nlattr *llattr;
	const __u8 *addr;
	struct net_device *dev;
	int err;

	ASSERT_RTNL();
	if (nlmsg_len(nlh) < sizeof(*ndm))
		return -EINVAL;

	ndm = nlmsg_data(nlh);
	if (ndm->ndm_ifindex == 0) {
		pr_info("bridge: RTM_DELNEIGH with invalid ifindex\n");
		return -EINVAL;
	}

	dev = __dev_get_by_index(net, ndm->ndm_ifindex);
	if (dev == NULL) {
		pr_info("bridge: RTM_DELNEIGH with unknown ifindex\n");
		return -ENODEV;
	}

	llattr = nlmsg_find_attr(nlh, sizeof(*ndm), NDA_LLADDR);
	if (llattr == NULL || nla_len(llattr) != ETH_ALEN) {
		pr_info("bridge: RTM_DELNEIGH with invalid address\n");
		return -EINVAL;
	}

	addr = nla_data(llattr);

	p = br_port_get_rtnl(dev);
	if (p == NULL) {
		pr_info("bridge: RTM_DELNEIGH %s not a bridge port\n",
			dev->name);
		return -EINVAL;
	}

	spin_lock_bh(&p->br->hash_lock);
	err = fdb_delete_by_addr(p, addr);
	spin_unlock_bh(&p->br->hash_lock);

	return err;
}

#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
int br_fdb_get_vid(const unsigned char *addr)
{
	struct net_bridge *br = NULL;
	struct hlist_node *h;
	struct net_bridge_fdb_entry *fdb;
	struct net_device *br_dev;
	int addr_hash = br_mac_hash(addr);
	int vid = -1;

	rcu_read_lock();

	for_each_netdev(&init_net, br_dev){
		if (br_dev->priv_flags & IFF_EBRIDGE) {
			br = netdev_priv(br_dev);
			hlist_for_each_entry_rcu(fdb, h, &br->hash[addr_hash], hlist) {
				if (!compare_ether_addr(fdb->addr.addr, addr)) {
					if (unlikely(!has_expired(br, fdb)))
						vid = (int)fdb->vid;
					break;
				}
			}
		}          
	}
	
	rcu_read_unlock();
	return vid;
}
EXPORT_SYMBOL(br_fdb_get_vid);
#endif //defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)


#if defined(CONFIG_BCM_KF_WL)
EXPORT_SYMBOL(fdb_check_expired_wl_hook);
EXPORT_SYMBOL(fdb_check_expired_dhd_hook);
#endif
