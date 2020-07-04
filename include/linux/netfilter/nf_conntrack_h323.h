/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
#ifndef _NF_CONNTRACK_H323_H
#define _NF_CONNTRACK_H323_H

#ifdef __KERNEL__

#include <linux/netfilter/nf_conntrack_h323_asn1.h>

#ifdef CONFIG_ATP_COMMON
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#endif

#define RAS_PORT 1719
#define Q931_PORT 1720
#define H323_RTP_CHANNEL_MAX 4	/* Audio, video, FAX and other */

/* This structure exists only once per master */
struct nf_ct_h323_master {


#if defined(CONFIG_BCM_KF_NETFILTER)
	enum{
		/* tpkt header and payload are wrapped in one packet */
		DIVTYPE_NORMAL = 0x00,
		/* tpkt header is in the first packet and payload is the
		 * next one
		 */
		DIVTYPE_TPKTHDR	= 0x01,
		/* tpkt packet (size maybe is more than several kbytes) is
		 * seperated into several parts by the tcp protocol. This
		 * dividing method is different from the second one.
		 */
		DIVTYPE_Q931 = 0x02,
	}div_type[IP_CT_DIR_MAX]; 
#endif

	/* Original and NATed Q.931 or H.245 signal ports */
	__be16 sig_port[IP_CT_DIR_MAX];

	/* Original and NATed RTP ports */
	__be16 rtp_port[H323_RTP_CHANNEL_MAX][IP_CT_DIR_MAX];

	union {
		/* RAS connection timeout */
		u_int32_t timeout;

		/* Next TPKT length (for separate TPKT header and data) */
		u_int16_t tpkt_len[IP_CT_DIR_MAX];
	};
};

struct nf_conn;

#ifdef CONFIG_ATP_COMMON
#define MEDIA_CONTROL_CHANNEL   1
#define MEDIA_CHANNEL           0


static inline int is_local_addr(union nf_inet_addr addr)
{
    struct net_device *brdev = NULL;
    struct in_device *indev = NULL;
    struct in_ifaddr *ifa = NULL;
    
    brdev = __dev_get_by_name(&init_net, "br0");
    if (brdev)
    {
        indev = __in_dev_get_rtnl(brdev);
        if (indev && (NULL != (ifa = indev->ifa_list)))
        {
            for (; ifa; ifa = ifa->ifa_next)
            {
                if ((ifa->ifa_local & ifa->ifa_mask) == (addr.ip & ifa->ifa_mask))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
#endif

#if defined(CONFIG_BCM_KF_NETFILTER)
extern int have_direct_route(union nf_inet_addr *src, union nf_inet_addr *dst,
			     int family);
#endif 

extern int get_h225_addr(struct nf_conn *ct, unsigned char *data,
			 TransportAddress *taddr,
			 union nf_inet_addr *addr, __be16 *port);
extern void nf_conntrack_h245_expect(struct nf_conn *new,
				     struct nf_conntrack_expect *this);
extern void nf_conntrack_q931_expect(struct nf_conn *new,
				     struct nf_conntrack_expect *this);
#if defined(CONFIG_BCM_KF_NETFILTER)
extern int (*set_addr_bf_hook)(struct sk_buff **pskb,
		       	       unsigned char **data, int datalen, int dataoff);
#endif
extern int (*set_h245_addr_hook) (struct sk_buff *skb,
				  unsigned char **data, int dataoff,
				  H245_TransportAddress *taddr,
				  union nf_inet_addr *addr,
				  __be16 port);
extern int (*set_h225_addr_hook) (struct sk_buff *skb,
				  unsigned char **data, int dataoff,
				  TransportAddress *taddr,
				  union nf_inet_addr *addr,
				  __be16 port);
extern int (*set_sig_addr_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned char **data,
				 TransportAddress *taddr, int count);
extern int (*set_ras_addr_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned char **data,
				 TransportAddress *taddr, int count);
#ifndef CONFIG_ATP_COMMON
extern int (*nat_rtp_rtcp_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned char **data, int dataoff,
				 H245_TransportAddress *taddr,
				 __be16 port, __be16 rtp_port,
				 struct nf_conntrack_expect *rtp_exp,
				 struct nf_conntrack_expect *rtcp_exp);
#else
extern int (*nat_rtp_rtcp_hook) (struct sk_buff *skb,
				 struct nf_conn *ct,
				 enum ip_conntrack_info ctinfo,
				 unsigned char **data, int dataoff,
				 H245_TransportAddress *taddr,
				 __be16 port, __be16 rtp_port,
				 struct nf_conntrack_expect *rtp_exp,
				 struct nf_conntrack_expect *rtcp_exp,
				 unsigned int mediatype);
#endif
extern int (*nat_t120_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned char **data, int dataoff,
			     H245_TransportAddress *taddr, __be16 port,
			     struct nf_conntrack_expect *exp);
extern int (*nat_h245_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned char **data, int dataoff,
			     TransportAddress *taddr, __be16 port,
			     struct nf_conntrack_expect *exp);
extern int (*nat_callforwarding_hook) (struct sk_buff *skb,
				       struct nf_conn *ct,
				       enum ip_conntrack_info ctinfo,
				       unsigned char **data, int dataoff,
				       TransportAddress *taddr,
				       __be16 port,
				       struct nf_conntrack_expect *exp);
extern int (*nat_q931_hook) (struct sk_buff *skb, struct nf_conn *ct,
			     enum ip_conntrack_info ctinfo,
			     unsigned char **data, TransportAddress *taddr,
			     int idx, __be16 port,
			     struct nf_conntrack_expect *exp);

#endif

#endif
