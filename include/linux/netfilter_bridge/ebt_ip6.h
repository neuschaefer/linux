/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 *  ebt_ip6
 *
 *	Authors:
 * Kuo-Lang Tseng <kuo-lang.tseng@intel.com>
 * Manohar Castelino <manohar.r.castelino@intel.com>
 *
 *  Jan 11, 2008
 *
 */

#ifndef __LINUX_BRIDGE_EBT_IP6_H
#define __LINUX_BRIDGE_EBT_IP6_H

#include <linux/types.h>

#define EBT_IP6_SOURCE 0x01
#define EBT_IP6_DEST 0x02
#define EBT_IP6_TCLASS 0x04
#define EBT_IP6_PROTO 0x08
#define EBT_IP6_SPORT 0x10
#define EBT_IP6_DPORT 0x20
#define EBT_IP6_FLOWLBL 0x40  // IPv6 Flow Label
#define EBT_IP6_ICMP6 0x80

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE
#define EBT_IP6_SRANGE 0x0100
#define EBT_IP6_DRANGE 0x0200
#endif

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE 
#define EBT_IP6_MASK (EBT_IP6_SOURCE | EBT_IP6_DEST | EBT_IP6_TCLASS |\
		      EBT_IP6_PROTO | EBT_IP6_SPORT | EBT_IP6_DPORT | EBT_IP6_FLOWLBL |\
		      EBT_IP6_ICMP6 | EBT_IP6_SRANGE | EBT_IP6_DRANGE)
#else
#define EBT_IP6_MASK (EBT_IP6_SOURCE | EBT_IP6_DEST | EBT_IP6_TCLASS |\
		      EBT_IP6_ICMP6 | EBT_IP6_PROTO | EBT_IP6_SPORT | EBT_IP6_DPORT | EBT_IP6_FLOWLBL)
#endif

#define EBT_IP6_MATCH "ip6"

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE  
struct ebt_ip6range
{
    struct in6_addr min_ip6;
    struct in6_addr max_ip6;
};
#endif

/* the same values are used for the invflags */
struct ebt_ip6_info {
	struct in6_addr saddr;
	struct in6_addr daddr;
	struct in6_addr smsk;
	struct in6_addr dmsk;
	__u8  tclass;
	__u8  protocol;
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE  
	__u16  bitmask;
	__u16  invflags;
#else
	__u8  bitmask;
	__u8  invflags;
#endif

	union {
		__u16 sport[2];
		__u8 icmpv6_type[2];
	};
	union {
		__u16 dport[2];
		__u8 icmpv6_code[2];
	};
    __u32 flowlbl;  // IPv6 Flow Label
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE  
    struct ebt_ip6range ip6range_src;
    struct ebt_ip6range ip6range_dst; 
#endif    
};

#endif
