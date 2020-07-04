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

#ifndef _BR_IGMP_H
#define _BR_IGMP_H

#if defined(CONFIG_BCM_KF_IGMP)
#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/igmp.h>
#include <linux/in.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#endif
#include "br_mcast.h"

#if defined(CONFIG_BR_IGMP_SNOOP)

#define TIMER_CHECK_TIMEOUT (2*HZ)
#define BR_IGMP_MEMBERSHIP_TIMEOUT 260 /* RFC3376 */

struct net_bridge_mc_src_entry
{
    struct in_addr   src;
    unsigned long    tstamp;
    int              filt_mode;
};

struct net_bridge_mc_rep_entry
{
	struct in_addr      rep;
	unsigned long       tstamp;
	struct list_head    list;
};

struct net_bridge_mc_fdb_entry
{
	struct hlist_node               hlist;
	struct net_bridge_port         *dst;
	struct in_addr                  rxGrp;
	struct in_addr                  txGrp;
	struct list_head                rep_list;
	struct net_bridge_mc_src_entry  src_entry;
	uint16_t                        lan_tci; /* vlan id */
	uint32_t                        wan_tci; /* vlan id */
	int                             num_tags;
	char                            type;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	uint32_t                        blog_idx;
	char                            root;
#endif
	int                             lanppp;
	int                             excludePort;
	char                            enRtpSeqCheck;  
	struct net_device              *from_dev;
};

enum mcpd_packet_admitted
{
  MCPD_PACKET_ADMITTED_NO      = 0,
  MCPD_PACKET_ADMITTED_YES     = 1,
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
  MCPD_MLD_PACKET_ADMITTED_NO  = 2,
  MCPD_MLD_PACKET_ADMITTED_YES = 3,
#endif
};

typedef struct
{
    int                       bridgePointer;
    int                       skbPointer;
    enum mcpd_packet_admitted admitted;
} t_MCPD_ADMISSION;

int br_igmp_control_filter(const unsigned char *dest, __be32 dest_ip);

extern void mcpd_nl_send_igmp_purge_entry(struct net_bridge_mc_fdb_entry *igmp_entry);

int br_igmp_blog_rule_update(struct net_bridge_mc_fdb_entry *mc_fdb, int wan_ops);

extern int br_igmp_mc_forward(struct net_bridge *br, 
                              struct sk_buff *skb, 
                              int forward,
                              int is_routed);
void br_igmp_delbr_cleanup(struct net_bridge *br);

int br_igmp_mc_fdb_add(struct net_device *from_dev,
                       int wan_ops,
                       struct net_bridge *br, 
                       struct net_bridge_port *prt, 
                       struct in_addr *rxGrp, 
                       struct in_addr *txGrp, 
                       struct in_addr *rep, 
                       int mode, 
                       uint16_t tci, 
                       struct in_addr *src,
                       int lanppp,
                       int excludePort,
                       char enRtpSeqCheck);

extern void br_igmp_mc_fdb_cleanup(struct net_bridge *br);
extern int br_igmp_mc_fdb_remove(struct net_device *from_dev,
                                 struct net_bridge *br, 
                                 struct net_bridge_port *prt, 
                                 struct in_addr *rxGrp, 
                                 struct in_addr *txGrp, 
                                 struct in_addr *rep, 
                                 int mode, 
                                 struct in_addr *src);
int br_igmp_mc_fdb_update_bydev( struct net_bridge *br,
                                 struct net_device *dev,
                                 unsigned int       flushAll);
int __init br_igmp_snooping_init(void);
void br_igmp_snooping_fini(void);
void br_igmp_set_snooping(int val);
void br_igmp_handle_netdevice_events(struct net_device *ndev, unsigned long event);
void br_igmp_wipe_reporter_for_port (struct net_bridge *br,
                                     struct in_addr *rep, 
                                     u16 oldPort);
int br_igmp_process_if_change(struct net_bridge *br, struct net_device *ndev);
struct net_bridge_mc_fdb_entry *br_igmp_mc_fdb_copy(struct net_bridge *br, 
                                     const struct net_bridge_mc_fdb_entry *igmp_fdb);
void br_igmp_mc_fdb_del_entry(struct net_bridge *br, 
                              struct net_bridge_mc_fdb_entry *igmp_fdb,
                              struct in_addr *rep);
void br_igmp_set_timer( struct net_bridge *br );
void br_igmp_process_timer_check ( struct net_bridge *br );
void br_igmp_process_admission (t_MCPD_ADMISSION* admit);
void br_igmp_wipe_pending_skbs( void );
void br_igmp_process_device_removal(struct net_device* dev);
#endif /* CONFIG_BR_IGMP_SNOOP */

void br_igmp_get_ip_igmp_hdrs( const struct sk_buff *pskb, struct iphdr **ppipmcast, struct igmphdr **ppigmp, int *lanppp);
#endif /* CONFIG_BCM_KF_IGMP */

#endif /* _BR_IGMP_H */
