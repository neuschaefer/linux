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
#ifndef _BR_MCAST_H
#define _BR_MCAST_H

#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))

#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/igmp.h>
#include <linux/in.h>
#include "br_private.h"
#include <linux/if_vlan.h>
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif

#define MCPD_IF_TYPE_UNKWN      0
#define MCPD_IF_TYPE_BRIDGED    1
#define MCPD_IF_TYPE_ROUTED     2

typedef enum br_mcast_proto_type {
    BR_MCAST_PROTO_NONE,
    BR_MCAST_PROTO_IGMP,
    BR_MCAST_PROTO_MLD
} t_BR_MCAST_PROTO_TYPE;

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
void br_mcast_blog_release(t_BR_MCAST_PROTO_TYPE proto, void *mc_fdb);
void br_mcast_vlan_notify_for_blog_update(struct net_device *ndev,
                                   blogRuleVlanNotifyDirection_t direction,
                                   uint32_t nbrOfTags);
int br_mcast_blog_process(struct net_bridge *br,
                            void *mc_fdb,
                            t_BR_MCAST_PROTO_TYPE proto);
#endif
void br_mcast_handle_netdevice_events(struct net_device *ndev, unsigned long event);

#define PROXY_DISABLED_MODE 0
#define PROXY_ENABLED_MODE 1

#define SNOOPING_DISABLED_MODE 0
#define SNOOPING_ENABLED_MODE 1
#define SNOOPING_BLOCKING_MODE 2

typedef enum br_mcast_l2l_snoop_mode {
    BR_MCAST_L2L_SNOOP_DISABLED,
    BR_MCAST_L2L_SNOOP_ENABLED,
    BR_MCAST_L2L_SNOOP_ENABLED_LAN
} t_BR_MCAST_L2L_SNOOP_MODE;

#define MCPD_MAX_IFS            10
typedef struct mcpd_wan_info
{
	char                      if_name[IFNAMSIZ];
	int                       if_ops;
} t_MCPD_WAN_INFO;

typedef struct mcpd_igmp_snoop_entry
{
	char                      br_name[IFNAMSIZ];
	/* Internal, ignore endianness */
	unsigned short            port_no;
	unsigned int              mode;
	unsigned int              code;
	unsigned short            tci;/* vlan id */
	t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];
	int                       lanppp;
	int                       excludePort;  
	char                      enRtpSeqCheck;
	/* Standard, use big endian */
	struct                    in_addr rxGrp;
	struct                    in_addr txGrp;
	struct                    in_addr src;
	struct                    in_addr rep;
} t_MCPD_IGMP_SNOOP_ENTRY;

typedef struct mcastCfg {
	int          mcastPriQueue;
	int          thereIsAnUplink;
} t_MCAST_CFG;

void br_mcast_set_pri_queue(int val);
int  br_mcast_get_pri_queue(void);
void br_mcast_set_skb_mark_queue(struct sk_buff *skb);
void br_mcast_set_uplink_exists(int uplinkExists);
int  br_mcast_get_lan2lan_snooping(t_BR_MCAST_PROTO_TYPE proto, struct net_bridge *br);

#if defined(CONFIG_BR_MLD_SNOOP) && defined(CONFIG_BCM_KF_MLD)
#define SNOOPING_ADD_ENTRY 0
#define SNOOPING_DEL_ENTRY 1
#define SNOOPING_FLUSH_ENTRY 2
#define SNOOPING_FLUSH_ENTRY_ALL 3
int mcast_snooping_call_chain(unsigned long val,void *v);
void br_mcast_wl_flush(struct net_bridge *br) ;

typedef struct mcpd_mld_snoop_entry
{
	char                      br_name[IFNAMSIZ];
	/* Internal, ignore endianness */
	unsigned short            port_no;
	unsigned int              mode;
	unsigned int              code;
	unsigned short            tci;
	t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];
	int                       lanppp;
	/* External, use big endian */
	struct                    in6_addr grp;
 	struct                    in6_addr src;
	struct                    in6_addr rep;
	unsigned char             srcMac[6];
} t_MCPD_MLD_SNOOP_ENTRY;
#endif

#endif /* (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)) */

#endif /* _BR_MCAST_H */
