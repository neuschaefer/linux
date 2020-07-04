/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
/*
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

#ifndef _LINUX_IF_BRIDGE_H
#define _LINUX_IF_BRIDGE_H

#include <linux/types.h>

#define SYSFS_BRIDGE_ATTR	"bridge"
#define SYSFS_BRIDGE_FDB	"brforward"
#define SYSFS_BRIDGE_PORT_SUBDIR "brif"
#define SYSFS_BRIDGE_PORT_ATTR	"brport"
#define SYSFS_BRIDGE_PORT_LINK	"bridge"

#define BRCTL_VERSION 1

#define BRCTL_GET_VERSION 0
#define BRCTL_GET_BRIDGES 1
#define BRCTL_ADD_BRIDGE 2
#define BRCTL_DEL_BRIDGE 3
#define BRCTL_ADD_IF 4
#define BRCTL_DEL_IF 5
#define BRCTL_GET_BRIDGE_INFO 6
#define BRCTL_GET_PORT_LIST 7
#define BRCTL_SET_BRIDGE_FORWARD_DELAY 8
#define BRCTL_SET_BRIDGE_HELLO_TIME 9
#define BRCTL_SET_BRIDGE_MAX_AGE 10
#define BRCTL_SET_AGEING_TIME 11
#define BRCTL_SET_GC_INTERVAL 12
#define BRCTL_GET_PORT_INFO 13
#define BRCTL_SET_BRIDGE_STP_STATE 14
#define BRCTL_SET_BRIDGE_PRIORITY 15
#define BRCTL_SET_PORT_PRIORITY 16
#define BRCTL_SET_PATH_COST 17
#define BRCTL_GET_FDB_ENTRIES 18

#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_IGMP_RATE_LIMIT)
#define BRCTL_ENABLE_IGMP_RATE_LIMIT            21    /*not used by our brctl*/
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#define BRCTL_ADD_FDB_ENTRIES                   26
#define BRCTL_DEL_FDB_ENTRIES                   27
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_BRIDGE_DYNAMIC_FDB)
#define BRCTL_DEL_DYN_FDB_ENTRIES               28
#endif

#define BRCTL_SET_FLOWS              29

#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_UNI_UNI)
#define BRCTL_SET_UNI_UNI_CTRL                  30
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_IGMP)
#define BRCTL_ENABLE_IGMP_LAN2LAN_MC            31
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_MLD)
#define BRCTL_ENABLE_MLD_LAN2LAN_MC             32
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_IGMP)
#define BRCTL_GET_IGMP_LAN_TO_LAN_MCAST_ENABLED 33
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_MLD)
#define BRCTL_GET_MLD_LAN_TO_LAN_MCAST_ENABLED  34
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_BRIDGE_STATIC_FDB)
#define BRCTL_DEL_STATIC_FDB_ENTRIES            35
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || defined(CONFIG_BCM_KF_BRIDGE_DYNAMIC_FDB)
#define BRCTL_ADD_FDB_DYNAMIC_ENTRIES           36
#endif
#if !defined(CONFIG_BCM_IN_KERNEL) || (defined(CONFIG_BCM_KF_BRIDGE_MAC_FDB_LIMIT) && defined(CONFIG_BCM_BRIDGE_MAC_FDB_LIMIT))
#define BRCTL_GET_BR_FDB_LIMIT                  37
#define BRCTL_SET_BR_FDB_LIMIT                  38
#endif

#if defined (CONFIG_IGMP_SNOOPING) 
//atp 
#define BRCTL_SET_IGMP_SNOOPING 23
#define BRCTL_SHOW_IGMP_SNOOPING 24
#else
//bcm的组播加速才用
#define BRCTL_ENABLE_SNOOPING        22
#define BRCTL_ENABLE_PROXY_MODE      23
#endif

#if defined (CONFIG_MLD_SNOOPING)
//atp 
#define BRCTL_SET_MLD_SNOOPING  25
#define BRCTL_SHOW_MLD_SNOOPING 26
#else
//bcm的组播加速才用
#define BRCTL_MLD_ENABLE_SNOOPING    25
#define BRCTL_MLD_ENABLE_PROXY_MODE  26
#endif

#ifdef CONFIG_BRIDGE_PORT_RELAY
#define BRCTL_SET_PORT_RELAY 34
#endif

#define BR_STATE_DISABLED 0
#define BR_STATE_LISTENING 1
#define BR_STATE_LEARNING 2
#define BR_STATE_FORWARDING 3
#define BR_STATE_BLOCKING 4

struct __bridge_info {
	__u64 designated_root;
	__u64 bridge_id;
	__u32 root_path_cost;
	__u32 max_age;
	__u32 hello_time;
	__u32 forward_delay;
	__u32 bridge_max_age;
	__u32 bridge_hello_time;
	__u32 bridge_forward_delay;
	__u8 topology_change;
	__u8 topology_change_detected;
	__u8 root_port;
	__u8 stp_enabled;
	__u32 ageing_time;
	__u32 gc_interval;
	__u32 hello_timer_value;
	__u32 tcn_timer_value;
	__u32 topology_change_timer_value;
	__u32 gc_timer_value;
};

struct __port_info {
	__u64 designated_root;
	__u64 designated_bridge;
	__u16 port_id;
	__u16 designated_port;
	__u32 path_cost;
	__u32 designated_cost;
	__u8 state;
	__u8 top_change_ack;
	__u8 config_pending;
	__u8 unused0;
	__u32 message_age_timer_value;
	__u32 forward_delay_timer_value;
	__u32 hold_timer_value;
};

struct __fdb_entry {
	__u8 mac_addr[6];
	__u8 port_no;
	__u8 is_local;
	__u32 ageing_timer_value;
	__u8 port_hi;
	__u8 pad0;
#if defined(CONFIG_BCM_KF_VLAN_AGGREGATION) && defined(CONFIG_BCM_VLAN_AGGREGATION)
	__u16 vid;
#else
	__u16 unused;
#endif
};

/*ATP新增*/
struct __fdb_lan
{
    __u8 mac_addr[6];
    __u8 port_no;
    char lanname[16];
};

#ifdef __KERNEL__

#include <linux/netdevice.h>

#if defined(CONFIG_BCM_KF_BRIDGE_PORT_ISOLATION) || defined(CONFIG_BCM_KF_BRIDGE_STP)
enum {
	BREVT_IF_CHANGED,
	BREVT_STP_STATE_CHANGED
};
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_PORT_ISOLATION)
extern struct net_device *bridge_get_next_port(char *brName, unsigned int *portNum);
extern int register_bridge_notifier(struct notifier_block *nb);
extern int unregister_bridge_notifier(struct notifier_block *nb);
#endif

#if defined(CONFIG_BCM_KF_BRIDGE_STP)
struct stpPortInfo {
	char portName[IFNAMSIZ];
	unsigned char stpState;
};
extern int register_bridge_stp_notifier(struct notifier_block *nb);
extern int unregister_bridge_stp_notifier(struct notifier_block *nb);
#endif

extern void brioctl_set(int (*ioctl_hook)(struct net *, unsigned int, void __user *));

typedef int br_should_route_hook_t(struct sk_buff *skb);
extern br_should_route_hook_t __rcu *br_should_route_hook;

#if defined(CONFIG_BCM_KF_FBOND) && (defined(CONFIG_BCM_FBOND) || defined(CONFIG_BCM_FBOND_MODULE))
typedef struct net_device *(* br_fb_process_hook_t)(struct sk_buff *skb_p, uint16_t h_proto, struct net_device *txDev );
extern void br_fb_bind(br_fb_process_hook_t brFbProcessHook);
#endif

#if (defined CONFIG_HSAN)
extern int hi_br_get_port(int ifindex, unsigned char *addr); 
extern int hi_br_find_mac(struct net_device *dev, unsigned char *addr);
extern struct net_bridge_fdb_entry * hi_br_find_fdb(struct net_device *dev, unsigned char *addr); 
#endif

#endif

#endif
