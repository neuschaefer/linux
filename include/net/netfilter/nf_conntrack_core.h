/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 * This header is used to share core functionality between the
 * standalone connection tracking module, and the compatibility layer's use
 * of connection tracking.
 *
 * 16 Dec 2003: Yasuyuki Kozakai @USAGI <yasuyuki.kozakai@toshiba.co.jp>
 *	- generalize L3 protocol dependent part.
 *
 * Derived from include/linux/netfiter_ipv4/ip_conntrack_core.h
 */

#ifndef _NF_CONNTRACK_CORE_H
#define _NF_CONNTRACK_CORE_H

#include <linux/netfilter.h>
#include <net/netfilter/nf_conntrack_l3proto.h>
#include <net/netfilter/nf_conntrack_l4proto.h>
#include <net/netfilter/nf_conntrack_ecache.h>

/* This header is used to share core functionality between the
   standalone connection tracking module, and the compatibility layer's use
   of connection tracking. */
#if defined(CONFIG_ATP_HYBRID_GREACCEL)
#define HI_NF_NORMAL_DEATH_HOOK   1
#define HI_NF_TIMEOUT_DEATH_HOOK  2    /*this is only used by GREACCEL*/
typedef unsigned int (*hi_nf_conntrack_death_hook)(void*, int);
extern unsigned int hi_nf_conntrack_register_death_hook(hi_nf_conntrack_death_hook hook);
extern unsigned int hi_nf_conntrack_unregister_death_hook(void);
#endif

extern unsigned int nf_conntrack_in(struct net *net,
				    u_int8_t pf,
				    unsigned int hooknum,
				    struct sk_buff *skb);

extern int nf_conntrack_init(struct net *net);
extern void nf_conntrack_cleanup(struct net *net);

extern int nf_conntrack_proto_init(void);
extern void nf_conntrack_proto_fini(void);

extern bool
nf_ct_get_tuple(const struct sk_buff *skb,
		unsigned int nhoff,
		unsigned int dataoff,
		u_int16_t l3num,
		u_int8_t protonum,
		struct nf_conntrack_tuple *tuple,
		const struct nf_conntrack_l3proto *l3proto,
		const struct nf_conntrack_l4proto *l4proto);

extern bool
nf_ct_invert_tuple(struct nf_conntrack_tuple *inverse,
		   const struct nf_conntrack_tuple *orig,
		   const struct nf_conntrack_l3proto *l3proto,
		   const struct nf_conntrack_l4proto *l4proto);

/* Find a connection corresponding to a tuple. */
extern struct nf_conntrack_tuple_hash *
nf_conntrack_find_get(struct net *net, u16 zone,
		      const struct nf_conntrack_tuple *tuple);

extern int __nf_conntrack_confirm(struct sk_buff *skb);

/* Confirm a connection: returns NF_DROP if packet must be dropped. */
static inline int nf_conntrack_confirm(struct sk_buff *skb)
{
	struct nf_conn *ct = (struct nf_conn *)skb->nfct;
	int ret = NF_ACCEPT;

	if (ct && !nf_ct_is_untracked(ct)) {
		if (!nf_ct_is_confirmed(ct))
			ret = __nf_conntrack_confirm(skb);
		if (likely(ret == NF_ACCEPT))
			nf_ct_deliver_cached_events(ct);
	}
	return ret;
}

int
print_tuple(struct seq_file *s, const struct nf_conntrack_tuple *tuple,
            const struct nf_conntrack_l3proto *l3proto,
            const struct nf_conntrack_l4proto *proto);

extern spinlock_t nf_conntrack_lock ;

#if (defined CONFIG_HSAN)
extern int  link_spin_lock_bh(void);
extern int  link_spin_unlock_bh(void);
extern int  res_link_refresh_ct (struct nf_conn * pst_ct);
extern int hi_nf_ct_get_packets(struct nf_conn *ct, u_int32_t mode, u_int32_t packets);
#endif
#endif /* _NF_CONNTRACK_CORE_H */
