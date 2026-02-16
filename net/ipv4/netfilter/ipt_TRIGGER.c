/* Kernel module to match the port-ranges, trigger related port-ranges,
 * and alters the destination to a local IP address.
 *
 * Copyright (C) 2009 - 2010, CyberTAN Corporation
 *
 * All Rights Reserved.
 * You can redistribute it and/or modify it under the terms of the GPL v2
 *
 * THIS SOFTWARE IS OFFERED "AS IS", AND CYBERTAN GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. CYBERTAN
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NON INFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 *
 * Description:
 *   This is kernel module for port-triggering.
 *
 *   The module follows the Netfilter framework, called extended packet
 *   matching modules.
 ******************************************************************************************************
 Includes Intel Corporation's changes/modifications
 Changed/modified portions :
 * -  X-tables API adapted for kernel 2.6.39
 * -  Added IPT_TRIGGER_BOTH_PROTOCOLS (matches both TCP and UDP)
 * -  Added, improved Debug prints
 * -  Formatting improvements and general code cleanup
 *
 * Copyright © 2011-2016, Intel Corporation.
 ******************************************************************************************************
 */


#include <linux/types.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/protocol.h>
#include <net/checksum.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#ifdef CONFIG_NF_NAT_NEEDED
#include <net/netfilter/nf_nat.h>
#else
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#endif
#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* Skip cleaning up the list of existing trigger entries each time a rule is matched.
   Instead, let all the entries be deleted automatically when they time out. */
#define SKIP_TRIGGER_LIST_CLEANUP

/* Return pointer to first true entry, if any, or NULL.  A macro
   required to allow inlining of cmpfn. */
#define LIST_FIND(head, cmpfn, type, args...)           \
   ({                                                      \
           const struct list_head *__i, *__j = NULL;       \
                                    \
           list_for_each(__i, (head))                      \
           if (cmpfn((const type)__i , ## args)) \
           { \
                  __j = __i;                     \
                  break;                         \
           }                                     \
           (type)__j;                            \
   })

/* Uncomment the next line to enable verbose debug prints */
//#define TRIGGER_DEBUG
#ifndef TRIGGER_DEBUG
        #define KERN_LEVL
        #define DEBUGP( format, args...)
        #define TRIGGER_TIMEOUT 120  /* 120 secs to wait for backward connection, when timeout - delete trigger */
#else
#define DEBUGP printk
        #define KERN_LEVL KERN_CRIT
        #define DEBUG_BIG_TIMEOUT 600 /* 10 min */
        #define TRIGGER_TIMEOUT DEBUG_BIG_TIMEOUT
#endif

MODULE_LICENSE("GPL");
struct ipt_trigger {
    struct list_head list;             /* Trigger list */
    struct timer_list timeout;         /* Timer for list destroying */
    __be32 srcip;                      /* Outgoing source address */
    __be32 dstip;                      /* Outgoing destination address */
    u_int16_t trig_proto;              /* Trigger protocol */
    u_int16_t rproto;                  /* Related protocol */
    struct ipt_trigger_ports ports;    /* Trigger and related ports */
    u_int8_t reply;                    /* Confirm a reply connection */
};

static LIST_HEAD(trigger_list);

static void trigger_refresh(struct ipt_trigger *trig, unsigned long extra_jiffies)
{
    DEBUGP( KERN_LEVL "%s: \n", __FUNCTION__);
    NF_CT_ASSERT(trig);
    spin_lock_bh(&nf_conntrack_lock);

    /* Need del_timer for race avoidance (may already be dying). */
    if (del_timer(&trig->timeout)) {
        trig->timeout.expires = jiffies + extra_jiffies;
        add_timer(&trig->timeout);
    }

    spin_unlock_bh(&nf_conntrack_lock);
}

static void __del_trigger(struct ipt_trigger *trig)
{
    DEBUGP( KERN_LEVL "%s: \n", __FUNCTION__);
    NF_CT_ASSERT(trig);

    /* delete from 'trigger_list' */
    list_del(&trig->list);
    kfree(trig);
}

static void trigger_timeout(unsigned long ul_trig)
{
    struct ipt_trigger *trig= (void *) ul_trig;

    DEBUGP( KERN_LEVL "trigger list %p timed out\n", trig);
    spin_lock_bh(&nf_conntrack_lock);
    __del_trigger(trig);
    spin_unlock_bh(&nf_conntrack_lock);
}

static unsigned int
add_new_trigger(struct ipt_trigger *trig)
{
    struct ipt_trigger *new;

    DEBUGP( KERN_LEVL "%s: \n", __FUNCTION__);
    spin_lock_bh(&nf_conntrack_lock);
    new = (struct ipt_trigger *)
    kmalloc(sizeof(struct ipt_trigger), GFP_ATOMIC);

    if (!new) {
        spin_unlock_bh(&nf_conntrack_lock);
        DEBUGP( KERN_LEVL "%s: OOM allocating trigger list\n", __FUNCTION__);
        return -ENOMEM;
    }

    memset(new, 0, sizeof(*trig));
    memcpy(new, trig, sizeof(*trig));
    INIT_LIST_HEAD(&new->list);

    /* add to global table of trigger */
    list_add_tail(&new->list, &trigger_list);

    /* add and start timer if required */
    init_timer(&new->timeout);
    new->timeout.data = (unsigned long)new;
    new->timeout.function = trigger_timeout;
    new->timeout.expires = jiffies + (TRIGGER_TIMEOUT * HZ);
    add_timer(&new->timeout);

    spin_unlock_bh(&nf_conntrack_lock);

    return 0;
}

/* Find a match for trigger-type=out, i.e match on trigger private ports */
static inline int trigger_out_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport)
{
    u_int16_t tproto = i->trig_proto;

    DEBUGP( KERN_LEVL "%s: Packet proto= %d, dport=%d.\n", __FUNCTION__,  proto, dport);
    DEBUGP( KERN_LEVL "%s: Trigger address =%p, trigger_protocol= %d, SRC_IP=%pI4 DST_IP=%pI4 mport[0..1]=%d, %d.\n", __FUNCTION__,
	    i, i->trig_proto, &i->srcip, &i->dstip, i->ports.mport[0], i->ports.mport[1]);

    /* Return OK if packet port belongs to trigger related range (public) and protocol
       is indeed one of specified protocols (either TCP, UDP, or BOTH) */
    if  (tproto != IPT_TRIGGER_BOTH_PROTOCOLS)
    {
        /* Test exact protocol */
        if  (tproto != proto)
        {
             return 0; /* not matched */
        }
    }
    if ( (i->ports.mport[0] <= dport)  && (i->ports.mport[1] >= dport))
    {
        DEBUGP( KERN_LEVL "%s: MATCHED trigger, Proto= %d, rport[0..1]=%d, %d.\n", __FUNCTION__,
            i->trig_proto, i->ports.mport[0], i->ports.mport[1]);
        return 1; /* port is within specified trigger range, match found */
    }
    else
    {
        return 0; /* not matched */
    }
}

/* This function creates trigger when packet traverse from 
   internal LAN to WAN destination 
   the packet is NATed, so nat hash is created */
static unsigned int
trigger_out(struct sk_buff *skb, const void *targinfo) 
{

    const struct ipt_trigger_info *info = targinfo;
    struct ipt_trigger trig, *found;
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;    /* Might be TCP, UDP */

    DEBUGP( KERN_LEVL "############# %s ############\n", __FUNCTION__);
    /* Check if the trigger range has already existed in 'trigger_list'. */
    found = LIST_FIND(&trigger_list, trigger_out_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    if (found) {
        /* Yeah, it exists. We need to update(delay) the destroying timer. */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        /* In order to allow multiple hosts use the same port range, we update
           the 'saddr' after previous trigger has a reply connection. */
        if (found->reply)
            found->srcip = iph->saddr;
    }
    else {
        /* Create new trigger */
        memset(&trig, 0, sizeof(trig));
        trig.srcip = iph->saddr;  /* Stores originating IP */
        trig.dstip = iph->daddr;  /* Stores destination IP */
        trig.rproto = iph->protocol;
        trig.trig_proto = info->proto;
        DEBUGP ( KERN_LEVL "TRIGGER OUT -protocol %d\n", info->proto);
        memcpy(&trig.ports, &info->ports, sizeof(struct ipt_trigger_ports));
            if (trig.ports.rport[1] == 0)
                trig.ports.rport[1] = trig.ports.rport[0];
            if (trig.ports.mport[1] == 0)
                trig.ports.mport[1] = trig.ports.mport[0];
        add_new_trigger(&trig); /* Add the new 'trig' to list 'trigger_list'. */
    }

    return XT_CONTINUE;     /* Proceed to  other checks */
}

/* Find a match for trigger-type=in and dnat, i.e match on trigger public ports */
static inline int trigger_in_matched(const struct ipt_trigger *i,
    const u_int16_t proto, const u_int16_t dport)
{
    u_int16_t tproto = i->trig_proto;

    DEBUGP( KERN_LEVL "%s: Packet proto= %d, dport=%d.\n", __FUNCTION__,  proto, dport);
    DEBUGP( KERN_LEVL "%s: Trigger address =%p, trigger_protocol= %d, mport[0..1]=%d, %d.\n", __FUNCTION__, 
	    i, i->trig_proto, i->ports.mport[0], i->ports.mport[1]); 


    /* Return OK if packet port belongs to trigger related range (public) and protocol
       is indeed one of specified protocols (either TCP, UDP, or BOTH) */
    if  (tproto != IPT_TRIGGER_BOTH_PROTOCOLS)
    {
        /* Test exact protocol */
        if  (tproto != proto)
        {
            return 0; /* not matched */
        }
    }
    if ( (i->ports.rport[0] <= dport)  && (i->ports.rport[1] >= dport))
    {
        DEBUGP( KERN_LEVL "%s: MATCHED trigger, Proto= %d, rport[0..1]=%d, %d.\n", __FUNCTION__,
        i->trig_proto, i->ports.rport[0], i->ports.rport[1]);
        return 1; /* port is within specified trigger range, match found */
    }
    else
    {
        return 0; /* not matched */
    }
}

/* This function processes packets which where changed by dnat function and allows them in forward chain */
static unsigned int
trigger_in(struct sk_buff *skb)
{
    struct ipt_trigger *found;
    const struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;    /* Might be TCP, UDP */

    /* Check if the trigger for this range was already created and inserted into the 'trigger_list'. */
    DEBUGP( KERN_LEVL "TRIGGER_IN - CHECK range: packet src->%pI4"
        "  packet dst->%pI4" "\n" ,
        &iph->saddr,
        &iph->daddr
        );

    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    if (found)
    {
        DEBUGP( KERN_LEVL "TRIGGER_IN: TRIGGER FOUND packet src->%pI4"
            " packet dst->%pI4 \n" ,
            &iph->saddr,
            &iph->daddr
            );

        /* Yeah, it exists. We need to update(delay) the destroying timer. */
        trigger_refresh(found, TRIGGER_TIMEOUT * HZ);
        DEBUGP( KERN_LEVL KERN_INFO "TRIGGER_IN: refresh trigger \n "  );

        return NF_ACCEPT;    /* Accept it, or the imcoming packet could be
                                dropped in the FORWARD chain */
    }

    return XT_CONTINUE;	/* the match was done but no trigger exist  */
}

/* Creates a nf_nat_range structure from a nf_nat_ipv4_range */
static void xt_nat_convert_range(struct nf_nat_range *dst,
				 const struct nf_nat_ipv4_range *src)
{
	memset(&dst->min_addr, 0, sizeof(dst->min_addr));
	memset(&dst->max_addr, 0, sizeof(dst->max_addr));

	dst->flags	     = src->flags;
	dst->min_addr.ip = src->min_ip;
	dst->max_addr.ip = src->max_ip;
	dst->min_proto	 = src->min;
	dst->max_proto	 = src->max;
}

/* This function translates a received from WAN packet's dest.IP to IP of the trigger invoker
   (trigger type 'out' )
   It needs to have conntrack info in order to associate incoming packet with previously made
   connection from LAN to WAN */
static unsigned int
trigger_dnat(struct sk_buff *skb, int hooknum)
{
    struct ipt_trigger *found;
    struct iphdr *iph = ip_hdr(skb);
    struct tcphdr *tcph = (void *)iph + iph->ihl*4;     /* Might be TCP or UDP */
    struct nf_conn *ct;
    enum ip_conntrack_info ctinfo;
    struct nf_nat_ipv4_multi_range_compat newrange;
    struct nf_nat_range range;

    DEBUGP( KERN_LEVL "####Starting  %s ############\n", __FUNCTION__);
    DEBUGP( KERN_LEVL "TRIGGER_DNAT: Protocol ->%s  SRC_IP->%pI4"
        " DST IP->%pI4 \n" ,
        (iph->protocol == IPPROTO_TCP) ? "TCP" : "UDP",
        &iph->saddr,
        &iph->daddr
        );
    NF_CT_ASSERT(hooknum == NF_INET_PRE_ROUTING);
    /* Check if the trigger-ed range has already existed in 'trigger_list'. */
    found = LIST_FIND(&trigger_list, trigger_in_matched,
        struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

    if (!found || !found->srcip)
        return XT_CONTINUE;    /* We don't block any packet, continue to scan rules table. */

    DEBUGP( KERN_LEVL "##### FOUND ### %s ############\n", __FUNCTION__);
    found->reply = 1;   /* Indicate that this match means a packet from related connection. */

    ct = nf_ct_get(skb, &ctinfo);
    NF_CT_ASSERT(ct && (ctinfo == IP_CT_NEW));

    DEBUGP( KERN_LEVL "%s: Got ready to dnat -foud proper trigger and doing address replacement ", __FUNCTION__);
    nf_ct_dump_tuple(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);

    /* Alter the destination of imcoming packet. */
    newrange.rangesize = 1;
    newrange.range[0].flags = NF_NAT_RANGE_MAP_IPS;
    newrange.range[0].min_ip = found->srcip;
    newrange.range[0].max_ip = found->srcip;
    newrange.range[0].min.all = 0;
    newrange.range[0].min.all = 0;

    xt_nat_convert_range(&range, &newrange.range[0]);
    /* Create the nat processor that will replace packet source address with that of the port forwarder requestor.
       The nat table is slightly different from the `filter' table,
       in that only the first packet of a new connection will traverse the table:
       the result of this traversal is then applied to all future packets in the same connection.
       http://www.netfilter.org/documentation/HOWTO//netfilter-hacking-HOWTO-3.html#ss3.3
       In test on aborted udp client, repeated udp connection was not established due to
       Conntrack Error : Protocol Error Detected for packet
       evidently  caused by UDP server not sending a single reply, so protocol state is not defined */

    /* Alter the destination of incoming packet. */
    return nf_nat_setup_info(ct, &range, NF_NAT_MANIP_DST);
}

static unsigned int
trigger_target(struct sk_buff *skb, const struct xt_action_param *par)
{
    unsigned int hooknum = par->hooknum;
    const struct ipt_trigger_info *info = par->targinfo ;
    const struct iphdr *iph = ip_hdr(skb);

    DEBUGP( KERN_LEVL "%s: type = %s\n", __FUNCTION__,
        (info->type == IPT_TRIGGER_DNAT) ? "dnat" :
        (info->type == IPT_TRIGGER_IN) ? "in" : "out");

    /* The Port-trigger only supports TCP and UDP. */
    if ((iph->protocol != IPPROTO_TCP) && (iph->protocol != IPPROTO_UDP))
        return XT_CONTINUE;

    if (info->type == IPT_TRIGGER_OUT)
        return trigger_out(skb, par->targinfo);
    else if (info->type == IPT_TRIGGER_IN)
        return trigger_in(skb);
    else if (info->type == IPT_TRIGGER_DNAT)
        return trigger_dnat(skb, hooknum);

    DEBUGP( KERN_LEVL "%s: Warning: Unknown trigger type %d\n", __FUNCTION__, info->type);
    return XT_CONTINUE;
}

static int trigger_check(const struct xt_tgchk_param *par )
{
    const struct ipt_trigger_info *info = par->targinfo;
    unsigned int hook_mask = par->hook_mask;
	
    /* Original version didn't support these rules in the mangle table, but this version does */

    if (hook_mask & ~((1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_FORWARD)))
    {
        printk("trigger_check: bad hooks %x.\n", hook_mask);
        return -EINVAL;
    }

    if (info->type == IPT_TRIGGER_OUT)  /* only trigger-type out specifies protocol and other parameters */
    {
        switch (info->proto)
        {
            case IPPROTO_TCP :
            case IPPROTO_UDP :
            case IPT_TRIGGER_BOTH_PROTOCOLS :
            {
                DEBUGP( KERN_LEVL "trigger_check: valid protocol %d.\n", info->proto);
                break;
            }
            default :
            {
                printk("trigger_check: ERROR:BAD protocol %d.\n", info->proto);
                return -EINVAL;
            }
        }

        if (!info->ports.mport[0] || !info->ports.rport[0]) {
            printk("trigger_check: Try 'iptables -j TRIGGER -h' for help.\n");
            return -EINVAL;
        }
    }

#ifndef SKIP_TRIGGER_LIST_CLEANUP
    /* Empty the 'trigger_list' */
    list_for_each_safe(cur_item, tmp_item, &trigger_list)
    {
        struct ipt_trigger *trig = (void *)cur_item;

        DEBUGP( KERN_LEVL "%s: list_for_each_safe(): %p.\n", __FUNCTION__, trig);
        del_timer(&trig->timeout);
        __del_trigger(trig);
    }
#endif

    return 0;
}

static struct xt_target redirect_reg __read_mostly = {
    .name           = "TRIGGER",
    .family         = NFPROTO_IPV4,
    .target         = trigger_target,
    .targetsize     = sizeof(struct ipt_trigger_info),
    .hooks          = (1 << NF_INET_PRE_ROUTING) | (1 << NF_INET_FORWARD),
    .checkentry     = trigger_check,
    .me             = THIS_MODULE,
};

static int __init porttrigger_init(void)
{
		return xt_register_target(&redirect_reg);
}

static void __exit porttrigger_exit(void)
{
		xt_unregister_target(&redirect_reg);
}

module_init(porttrigger_init);
module_exit(porttrigger_exit);
