/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#if defined(CONFIG_BCM_KF_NETFILTER)
/*
 * RTSP extension for NAT alteration.
 *
 * Copyright (c) 2008 Broadcom Corporation.
 *
 * <:label-BRCM:2011:DUAL/GPL:standard
 * 
 * Unless you and Broadcom execute a separate written software license 
 * agreement governing use of this software, this software is licensed 
 * to you under the terms of the GNU General Public License version 2 
 * (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php, 
 * with the following added to such license:
 * 
 *    As a special exception, the copyright holders of this software give 
 *    you permission to link this software with independent modules, and 
 *    to copy and distribute the resulting executable under terms of your 
 *    choice, provided that you also meet, for each linked independent 
 *    module, the terms and conditions of the license of that module. 
 *    An independent module is a module which is not derived from this
 *    software.  The special exception does not apply to any modifications 
 *    of the software.  
 * 
 * Not withstanding the above, under no circumstances may you combine 
 * this software in any way with any other Broadcom software provided 
 * under a license other than the GPL, without Broadcom's express prior 
 * written consent. 
 * 
 * :>
 *
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/tcp.h>
#include <net/tcp.h>

#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_helper.h>
#include <net/netfilter/nf_nat_rule.h>
#include <linux/netfilter/nf_conntrack_rtsp.h>
#define NF_NEED_STRNCASECMP
#define NF_NEED_STRTOU16
#include <linux/netfilter_helpers.h>
/*declare static variable to avoid allocating memory during processing packets*/
/*This static variable will store the mangled content*/
static char *nat_rtsp_buffer;
static uint nat_rtsp_len;
static bool nat_need_mangle;
static DEFINE_SPINLOCK(nat_rtsp_buffer_lock);

#define SKIP_WSPACE(ptr,len,off) while(off < len && isspace(*(ptr+off))) { off++; }

/*** helper functions ***/
static void
get_skb_tcpdata(struct sk_buff* skb, char** pptcpdata, uint* ptcpdatalen)
{
    struct iphdr*   iph  = ip_hdr(skb);
    struct tcphdr*  tcph = (struct tcphdr*)((char*)iph + iph->ihl*4);

    *pptcpdata = (char*)tcph +  tcph->doff*4;
    *ptcpdatalen = (char*)skb->tail - *pptcpdata;
}
/****************************************************************************/
/*RTSP mangle the nat_rtsp_buffer*/
static int rtsp_mangle_tcp_buffer(unsigned int match_offset,
							    unsigned int match_len,
								const char *rep_buffer,
								unsigned int rep_len)
{
	char * psttcp = nat_rtsp_buffer;	
    
    if ((!psttcp) || (0 == nat_rtsp_len))
    {
    	return -1;
	}
    
    memmove(psttcp+match_offset+rep_len, psttcp+match_offset+match_len, 
				nat_rtsp_len-match_offset-match_len);
    memcpy(psttcp+match_offset, rep_buffer, rep_len);
    nat_rtsp_len = nat_rtsp_len + rep_len - match_len;
    nat_need_mangle = true;
    
    return 0;
}
/****************************************************************************/
/*RTSP mangle the skb must be called once after all the changes are done to the 
 *nat_rtsp_buffer*/
static int rtsp_mangle_tcp_content(struct sk_buff *skb, struct nf_conn *ct,
								enum ip_conntrack_info ctinfo)
{
	char * psttcp = NULL;
    uint tcplen = 0;
    
    if ((!nat_rtsp_buffer) || (0 == nat_rtsp_len) || (false == nat_need_mangle))
    {
        spin_unlock_bh(&nat_rtsp_buffer_lock);
        return -1;
    }

    get_skb_tcpdata(skb, &psttcp, &tcplen);
    
	if (!nf_nat_mangle_tcp_packet(skb, ct, ctinfo, 0, tcplen,
				      nat_rtsp_buffer, nat_rtsp_len)) {
		spin_unlock_bh(&nat_rtsp_buffer_lock);
		if (net_ratelimit())
			printk("nf_nat_rtsp: nf_nat_mangle_tcp_packet error\n");
		return -1;
	}
    
    spin_unlock_bh(&nat_rtsp_buffer_lock);
    return 0;
}
/****************************************************************************/
/*RTSP init the skb, must be called once before mangle*/
static int rtsp_init_tcp_buffer(const char * tcpbuf, uint tcplen)
{
	spin_lock_bh(&nat_rtsp_buffer_lock);
    
    memset(nat_rtsp_buffer, 0, NF_ALG_BUFFER_SIZE);
    nat_rtsp_len = 0;
    
	if ((!tcpbuf) || (NF_ALG_BUFFER_SIZE < tcplen))
    {
    	return -1;
    }
    
    memcpy(nat_rtsp_buffer, tcpbuf, tcplen);
    nat_rtsp_len = tcplen;
    
    return 0;
}
/****************************************************************************/
static int modify_ports(struct sk_buff *skb, struct nf_conn *ct,
			enum ip_conntrack_info ctinfo,
			int matchoff, int matchlen,
			u_int16_t rtpport, u_int16_t rtcpport,
			char dash)
{
	char buf[sizeof("65535-65535")];
	int len;

	if (dash)
#ifdef CONFIG_ATP_COMMON
		len = snprintf(buf, sizeof(buf), "%hu%c%hu", rtpport, dash, rtcpport);
#else
		len = sprintf(buf, "%hu%c%hu", rtpport, dash, rtcpport);
#endif
	else
#ifdef CONFIG_ATP_COMMON
		len = snprintf(buf, sizeof(buf), "%hu", rtpport);
#else
		len = sprintf(buf, "%hu", rtpport);
#endif
    if (0 > rtsp_mangle_tcp_buffer(matchoff, matchlen, buf, len))
    {
    	if (net_ratelimit())
			printk("nf_nat_rtsp: rtsp_mangle_tcp_buffer error\n");
		return -1;
    }
    
	return 0;
}

/* Setup NAT on this expected conntrack so it follows master but expect the src ip. */
/* If we fail to get a free NAT slot, we'll get dropped on confirm */
static void nf_nat_follow_master_nosrc(struct nf_conn *ct,
                          struct nf_conntrack_expect *exp)
{
	struct nf_nat_ipv4_range range;

	/* This must be a fresh one. */
	BUG_ON(ct->status & IPS_NAT_DONE_MASK);

	/* For DST manip, map port here to where it's expected. */
	range.flags = (NF_NAT_RANGE_MAP_IPS | NF_NAT_RANGE_PROTO_SPECIFIED);
	range.min = range.max = exp->saved_proto;
	range.min_ip = range.max_ip
		= ct->master->tuplehash[!exp->dir].tuple.src.u3.ip;
	nf_nat_setup_info(ct, &range, NF_NAT_MANIP_DST);
}

/****************************************************************************/
/* One data channel */
static int nat_rtsp_channel (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned int matchoff, unsigned int matchlen,
			     struct nf_conntrack_expect *rtp_exp)
{
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_expect *exp;
	int dir = CTINFO2DIR(ctinfo);
	u_int16_t nated_port = 0;
	struct hlist_node *n;
	int exp_exist = 0;

	/* Set expectations for NAT */
	rtp_exp->saved_proto.udp.port = rtp_exp->tuple.dst.u.udp.port;
	rtp_exp->expectfn = nf_nat_follow_master_nosrc;
	rtp_exp->dir = !dir;

	/* Lookup existing expects */
	spin_lock_bh(&nf_conntrack_lock);
	hlist_for_each_entry(exp, n, &help->expectations, lnode) {
		if (exp->saved_proto.udp.port == rtp_exp->saved_proto.udp.port){
			/* Expectation already exists */ 
			rtp_exp->tuple.dst.u.udp.port = 
				exp->tuple.dst.u.udp.port;
			nated_port = ntohs(exp->tuple.dst.u.udp.port);
			exp_exist = 1;
			break;
		}
	}
	spin_unlock_bh(&nf_conntrack_lock);

	if (exp_exist) {
		nf_ct_expect_related(rtp_exp);
		goto modify_message;
	}

	/* Try to get a port. */
	for (nated_port = ntohs(rtp_exp->tuple.dst.u.udp.port);
	     nated_port != 0; nated_port++) {
		rtp_exp->tuple.dst.u.udp.port = htons(nated_port);
		if (nf_ct_expect_related(rtp_exp) == 0)
			break;
	}

	if (nated_port == 0) {	/* No port available */
		if (net_ratelimit())
			printk("nf_nat_rtsp: out of UDP ports\n");
		return 0;
	}

modify_message:
	/* Modify message */
	if (modify_ports(skb, ct, ctinfo, matchoff, matchlen,
			 nated_port, 0, 0) < 0) {
		nf_ct_unexpect_related(rtp_exp);
		return -1;
	}

	/* Success */
	pr_debug("nf_nat_rtsp: expect RTP ");
	nf_ct_dump_tuple(&rtp_exp->tuple);

	return 0;
}

/****************************************************************************/
/* A pair of data channels (RTP/RTCP) */
static int nat_rtsp_channel2 (struct sk_buff *skb, struct nf_conn *ct,
			      enum ip_conntrack_info ctinfo,
			      unsigned int matchoff, unsigned int matchlen,
			      struct nf_conntrack_expect *rtp_exp,
			      struct nf_conntrack_expect *rtcp_exp,
			      char dash)
{
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_expect *exp;
	int dir = CTINFO2DIR(ctinfo);
	u_int16_t nated_port = 0;
	struct hlist_node *n;
	int exp_exist = 0;

	/* Set expectations for NAT */
	rtp_exp->saved_proto.udp.port = rtp_exp->tuple.dst.u.udp.port;
	rtp_exp->expectfn = nf_nat_follow_master_nosrc;
	rtp_exp->dir = !dir;
	rtcp_exp->saved_proto.udp.port = rtcp_exp->tuple.dst.u.udp.port;
	rtcp_exp->expectfn = nf_nat_follow_master_nosrc;
	rtcp_exp->dir = !dir;

	/* Lookup existing expects */
	spin_lock_bh(&nf_conntrack_lock);
	hlist_for_each_entry(exp, n, &help->expectations, lnode) {
		if (exp->saved_proto.udp.port == rtp_exp->saved_proto.udp.port){
			/* Expectation already exists */ 
			rtp_exp->tuple.dst.u.udp.port = 
				exp->tuple.dst.u.udp.port;
			rtcp_exp->tuple.dst.u.udp.port = 
				htons(ntohs(exp->tuple.dst.u.udp.port) + 1);
			nated_port = ntohs(exp->tuple.dst.u.udp.port);
			exp_exist = 1;
			break;
		}
	}
	spin_unlock_bh(&nf_conntrack_lock);

	if (exp_exist) {
		nf_ct_expect_related(rtp_exp);
		nf_ct_expect_related(rtcp_exp);
		goto modify_message;
	}

	/* Try to get a pair of ports. */
	for (nated_port = ntohs(rtp_exp->tuple.dst.u.udp.port) & (~1);
	     nated_port != 0; nated_port += 2) {
		rtp_exp->tuple.dst.u.udp.port = htons(nated_port);
		if (nf_ct_expect_related(rtp_exp) == 0) {
			rtcp_exp->tuple.dst.u.udp.port =
			    htons(nated_port + 1);
			if (nf_ct_expect_related(rtcp_exp) == 0)
				break;
			nf_ct_unexpect_related(rtp_exp);
		}
	}

	if (nated_port == 0) {	/* No port available */
		if (net_ratelimit())
			printk("nf_nat_rtsp: out of RTP/RTCP ports\n");
		return 0;
	}

modify_message:
	/* Modify message */
	if (modify_ports(skb, ct, ctinfo, matchoff, matchlen,
			 nated_port, nated_port + 1, dash) < 0) {
		nf_ct_unexpect_related(rtp_exp);
		nf_ct_unexpect_related(rtcp_exp);
		return -1;
	}

	/* Success */
	pr_debug("nf_nat_rtsp: expect RTP ");
	nf_ct_dump_tuple(&rtp_exp->tuple);
	pr_debug("nf_nat_rtsp: expect RTCP ");
	nf_ct_dump_tuple(&rtcp_exp->tuple);

	return 0;
}

/****************************************************************************/
static __be16 lookup_mapping_port(struct nf_conn *ct,
				  enum ip_conntrack_info ctinfo,
				  __be16 port)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	struct nf_conn_help *help = nfct_help(ct);
	struct nf_conntrack_expect *exp;
	struct nf_conn *child;
	struct hlist_node *n;

	/* Lookup existing expects */
	pr_debug("nf_nat_rtsp: looking up existing expectations...\n");
	hlist_for_each_entry(exp, n, &help->expectations, lnode) {
		if (exp->tuple.dst.u.udp.port == port) {
			pr_debug("nf_nat_rtsp: found port %hu mapped from "
				 "%hu\n",
			       	 ntohs(exp->tuple.dst.u.udp.port),
			       	 ntohs(exp->saved_proto.all));
			return exp->saved_proto.all;
		}
	}

	/* Lookup existing connections */
	pr_debug("nf_nat_rtsp: looking up existing connections...\n");
	list_for_each_entry(child, &ct->derived_connections, derived_list) {
		if (child->tuplehash[dir].tuple.dst.u.udp.port == port) {
			pr_debug("nf_nat_rtsp: found port %hu mapped from "
				 "%hu\n",
			       	 ntohs(child->tuplehash[dir].
			       	 tuple.dst.u.udp.port),
			       	 ntohs(child->tuplehash[!dir].
			       	 tuple.src.u.udp.port));
			return child->tuplehash[!dir].tuple.src.u.udp.port;
		}
	}

	return htons(0);
}

/****************************************************************************/
static int nat_rtsp_modify_port (struct sk_buff *skb, struct nf_conn *ct,
			      	 enum ip_conntrack_info ctinfo,
				 unsigned int matchoff, unsigned int matchlen,
			      	 __be16 rtpport)
{
	__be16 orig_port;

	orig_port = lookup_mapping_port(ct, ctinfo, rtpport);
	if (orig_port == htons(0)) {
		return 0;
	}
	if (modify_ports(skb, ct, ctinfo, matchoff, matchlen,
			 ntohs(orig_port), 0, 0) < 0)
		return -1;
	pr_debug("nf_nat_rtsp: Modified client_port from %hu to %hu\n",
	       	 ntohs(rtpport), ntohs(orig_port));
	return 0;
}

/****************************************************************************/
static int nat_rtsp_modify_port2 (struct sk_buff *skb, struct nf_conn *ct,
			       	  enum ip_conntrack_info ctinfo,
				  unsigned int matchoff, unsigned int matchlen,
			       	  __be16 rtpport, __be16 rtcpport,
				  char dash)
{
	__be16 orig_port;

	orig_port = lookup_mapping_port(ct, ctinfo, rtpport);
	if (orig_port == htons(0)) {
		return 0;
	}
	if (modify_ports(skb, ct, ctinfo, matchoff, matchlen,
			 ntohs(orig_port), ntohs(orig_port)+1, dash) < 0)
		return -1;
	pr_debug("nf_nat_rtsp: Modified client_port from %hu to %hu\n",
	       	 ntohs(rtpport), ntohs(orig_port));
	return 0;
}

/****************************************************************************/
static int nat_rtsp_modify_addr(struct sk_buff *skb, struct nf_conn *ct,
				enum ip_conntrack_info ctinfo,
				int matchoff, int matchlen)
{
	char buf[sizeof("255.255.255.255")];
	int dir = CTINFO2DIR(ctinfo);
	int len;

	/* Change the destination address to FW's WAN IP address */

#ifdef CONFIG_ATP_COMMON
	len = snprintf(buf, sizeof(buf), "%pI4",
		       &ct->tuplehash[!dir].tuple.dst.u3.ip);
#else
	len = sprintf(buf, "%pI4",
		       &ct->tuplehash[!dir].tuple.dst.u3.ip);
#endif
    if (0 > rtsp_mangle_tcp_buffer(matchoff, matchlen, buf, len))
    {
		if (net_ratelimit())
			printk("nf_nat_rtsp: rtsp_mangle_tcp_buffer error\n");
		return -1;
    } 
	return 0;
}

/****************************************************************************/
static int
rtsp_mangle_xnataddr(struct sk_buff *skb, struct nf_conn *ct,
        			       enum ip_conntrack_info ctinfo, 
        			       unsigned int protoff, uint xnatoff, uint xnatlen, char *nat_buffer)
{
    char*       ptcp;
    uint        tcplen;
    char*       pxnataddr;
    u_int32_t   newip;
    u_int16_t   dport;
    uint        off = 0;
    char        extaddr[16];
    const char* ptr;

    struct tcphdr _tcph, *th;
    unsigned int tcpdataoff;

    /* Get TCP header */
    th = skb_header_pointer(skb, protoff, sizeof(_tcph), &_tcph);
    if (th == NULL) {
    	return 0;
    }

    /* Get TCP payload offset */
    tcpdataoff = protoff + th->doff * 4;
    if (tcpdataoff >= skb->len) { /* No data? */
    	return 0;
    }

    /* Get TCP payload length */
    tcplen = skb->len - tcpdataoff;

    /* Get TCP payload pointer */
    ptcp = skb_header_pointer(skb, tcpdataoff, tcplen, nat_buffer);


    pr_debug("rtsp_mangle_xnat: xnatoff = %u, xnatlen = %u\n", xnatoff, xnatlen);

    pxnataddr = ptcp + xnatoff;

    if ((xnatoff + xnatlen) > tcplen || xnatlen < 14 || 
        !iseol(pxnataddr[xnatlen-1]) ||
        nf_strncasecmp(pxnataddr, "x-NAT_Address:", 14) != 0)
    {
        pr_debug("rtsp_mangle_xnat: sanity check failed\n");
        return 0;
    }
    
    off += 14;
    
    SKIP_WSPACE(ptcp + xnatoff, xnatlen, off);

    newip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
    dport = ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all);
    
    pr_debug("rtsp_mangle_xnat: new x-NAT_Address = %u.%u.%u.%u:%u\n", NIPQUAD(newip), dport);

    snprintf(extaddr, 16, "%u.%u.%u.%u", NIPQUAD(newip));

    /* x-NAT_Address: xx.xx.xx.xx:xx */
    if (off < xnatlen)
    {
        int   origoff = 0;
        int   origlen = 0;
        char  rbuf[32];
        uint  rbuflen = 0;
        u_int16_t   port;
        uint        numlen;
    
        /* 需要替换的内容和长度 */
        rbuflen = snprintf(rbuf, 32, "%s:%u", extaddr, dport);
        
        pr_debug("rtsp_mangle_xnat: rbuf=%s rbufalen:%d\n", rbuf, rbuflen);

        origoff = (pxnataddr-ptcp)+off;

        ptr = memchr(pxnataddr+off, ':', xnatlen-off);
        if ( NULL == ptr )
        {
            return 0;
        }
        
        numlen = ptr - pxnataddr - off;
        
        off += numlen;
        origlen += numlen;

        if (pxnataddr[off] == ':')
        {
            off++;
            origlen++;
            numlen = nf_strtou16(pxnataddr+off, &port);
            off += numlen;
            origlen += numlen;
        }
        
        pr_debug("rtsp_mangle_xnat: origoff = %d origlen = %d\n", origoff, origlen);

        if (0 > rtsp_mangle_tcp_buffer(origoff, origlen, rbuf, rbuflen))
        {
        	return 0;
        }     
    }

    pr_debug("rtsp_mangle_xnat: done\n");
       
    return 1;
}

static int __init init(void)
{
	nat_rtsp_buffer = kmalloc(NF_ALG_BUFFER_SIZE, GFP_KERNEL);
    if (!nat_rtsp_buffer) 
        return -ENOMEM;
    nat_rtsp_len = 0;
    nat_need_mangle = false;
	BUG_ON(rcu_dereference(nat_rtsp_channel_hook) != NULL);
	BUG_ON(rcu_dereference(nat_rtsp_channel2_hook) != NULL);
	BUG_ON(rcu_dereference(nat_rtsp_modify_port_hook) != NULL);
	BUG_ON(rcu_dereference(nat_rtsp_modify_port2_hook) != NULL);
	BUG_ON(rcu_dereference(nat_rtsp_modify_addr_hook) != NULL);
    BUG_ON(rcu_dereference(rtsp_mangle_xnataddr_hook) != NULL);
    BUG_ON(rcu_dereference(rtsp_init_tcp_buffer_hook) != NULL);
    BUG_ON(rcu_dereference(rtsp_mangle_tcp_content_hook) != NULL);
	rcu_assign_pointer(nat_rtsp_channel_hook, nat_rtsp_channel);
	rcu_assign_pointer(nat_rtsp_channel2_hook, nat_rtsp_channel2);
	rcu_assign_pointer(nat_rtsp_modify_port_hook, nat_rtsp_modify_port);
	rcu_assign_pointer(nat_rtsp_modify_port2_hook, nat_rtsp_modify_port2);
	rcu_assign_pointer(nat_rtsp_modify_addr_hook, nat_rtsp_modify_addr);
    rcu_assign_pointer(rtsp_mangle_xnataddr_hook, rtsp_mangle_xnataddr);
	rcu_assign_pointer(rtsp_init_tcp_buffer_hook, rtsp_init_tcp_buffer);
    rcu_assign_pointer(rtsp_mangle_tcp_content_hook, rtsp_mangle_tcp_content);

	pr_debug("nf_nat_rtsp: init success\n");
	return 0;
}

/****************************************************************************/
static void __exit fini(void)
{
	rcu_assign_pointer(nat_rtsp_channel_hook, NULL);
	rcu_assign_pointer(nat_rtsp_channel2_hook, NULL);
	rcu_assign_pointer(nat_rtsp_modify_port_hook, NULL);
	rcu_assign_pointer(nat_rtsp_modify_port2_hook, NULL);
	rcu_assign_pointer(nat_rtsp_modify_addr_hook, NULL);
    rcu_assign_pointer(rtsp_mangle_xnataddr_hook, NULL);
    rcu_assign_pointer(rtsp_init_tcp_buffer_hook, NULL);
    rcu_assign_pointer(rtsp_mangle_tcp_content_hook, NULL);
    kfree(nat_rtsp_buffer);
    nat_rtsp_len = 0;
    nat_need_mangle = false;
	synchronize_rcu();
}

/****************************************************************************/
module_init(init);
module_exit(fini);

MODULE_AUTHOR("Broadcom Corporation");
MODULE_DESCRIPTION("RTSP NAT helper");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ip_nat_rtsp");

#endif // defined(CONFIG_BCM_KF_NETFILTER)