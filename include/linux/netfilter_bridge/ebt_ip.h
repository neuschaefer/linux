/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bart.de.schuymer@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */

#ifndef __LINUX_BRIDGE_EBT_IP_H
#define __LINUX_BRIDGE_EBT_IP_H

#include <linux/types.h>

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE
#define EBT_IP_SOURCE   0x0001
#define EBT_IP_DEST     0x0002
#define EBT_IP_TOS      0x0004
#define EBT_IP_PROTO    0x0008
#define EBT_IP_SPORT    0x0010
#define EBT_IP_DPORT    0x0020
#define EBT_IP_8021P    0x0040
#define EBT_IP_8021Q    0x0080
#define EBT_IP_SRANGE   0x0100
#define EBT_IP_DRANGE   0x0200
#else
#define EBT_IP_SOURCE   0x01
#define EBT_IP_DEST     0x02
#define EBT_IP_TOS      0x04
#define EBT_IP_PROTO    0x08
#define EBT_IP_SPORT    0x10
#define EBT_IP_DPORT    0x20
#define EBT_IP_8021P    0x40
#define EBT_IP_8021Q    0x80
#endif

/*博通DSCP暂未使用，先定义宏避免编译不过*/
#if defined(CONFIG_BCM_KF_NETFILTER) || !defined(CONFIG_BCM_IN_KERNEL)
#define EBT_IP_DSCP  0x0400
#endif

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_8021P | EBT_IP_8021Q | EBT_IP_SRANGE | EBT_IP_DRANGE)
#else
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_8021P | EBT_IP_8021Q)
#endif
#define EBT_IP_MATCH "ip"

#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE
struct ebt_iprange {
	/* Inclusive: network order. */
	u_int32_t min_ip, max_ip;
};
#endif

/* the same values are used for the invflags */
struct ebt_ip_info {
	__be32 saddr;
	__be32 daddr;
	__be32 smsk;
	__be32 dmsk;
	/* Start of modified  for ipp/tos problem 2012-03-07 */
	__be32 tos;
	/* End of modified  for ipp/tos problem 2012-03-07 */
#if defined(CONFIG_BCM_KF_NETFILTER) || !defined(CONFIG_BCM_IN_KERNEL)
	__u8  dscp;
#endif
	__u8  protocol;
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE    
	__u16 bitmask;
	__u16 invflags;
#else
    __u8 bitmask;
    __u8 invflags;
#endif
	__u16 sport[2];
	__u16 dport[2];
	__u16 vlan_8021p;
	__u16 vlan_8021q;
#ifdef CONFIG_BRIDGE_EBT_IP_IPRANGE        
    struct ebt_iprange src;
    struct ebt_iprange dst;
#endif    
};

#endif
