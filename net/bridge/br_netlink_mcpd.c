/*
* 2017.09.07 - change this file
* (C) Huawei Technologies Co., Ltd. < >
*/
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

#if (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)) || (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP))

#include <linux/socket.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/in.h>
#include <linux/if.h>
#include <linux/if_vlan.h>
#include "br_igmp.h"
#include "br_mld.h"

static struct sock *nl_sk = NULL;
static int mcpd_pid = 0;
static int mcpdControlsIgmpAdmission = 0;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
static int mcpdControlsMldAdmission = 0;
#endif

#define MCPD_SNOOP_IN_ADD    1
#define MCPD_SNOOP_IN_CLEAR  2
#define MCPD_SNOOP_EX_ADD    3
#define MCPD_SNOOP_EX_CLEAR  4

#define MAX_MULTICAST_APPS_REGISTERED 2
#define MULTICAST_APP_INVALID_ENTRY -1

static int snoop_registered_pid[MAX_MULTICAST_APPS_REGISTERED];

typedef enum
{
    SNOOP_REGISTRATION_TYPE_DEFAULT,
    SNOOP_REGISTRATION_TYPE_PRIMARY
}SNOOP_REGISTRATION_TYPE;

typedef struct mcpd_msg_hdr 
{
    __u16 type;
    __u16 len;
} t_MCPD_MSG_HDR;

typedef enum mcpd_msgtype 
{
    MCDP_MSG_BASE = 0,
    MCPD_MSG_REGISTER, /* usr - > krnl -> usr */
    MCPD_MSG_UNREGISTER, /* usr - > krnl -> usr */
    MCPD_MSG_IGMP_PKT, /* krnl -> usr */
    MCPD_MSG_IGMP_SNOOP_ENTRY,
    MCPD_MSG_MLD_PKT, /* krnl -> usr */
    MCPD_MSG_MLD_SNOOP_ENTRY,
    MCPD_MSG_IGMP_PURGE_ENTRY,
    MCPD_MSG_MLD_PURGE_ENTRY,
    MCPD_MSG_IF_CHANGE,
    MCPD_MSG_MC_FDB_CLEANUP, /* clean up for MIB RESET */
    MCPD_MSG_SET_PRI_QUEUE,
    MCPD_MSG_UPLINK_INDICATION,
    MCPD_MSG_IGMP_PURGE_REPORTER,
    MCPD_MSG_MLD_PURGE_REPORTER,
    MCPD_MSG_CONTROLS_ADMISSION,
    MCPD_MSG_ADMISSION_RESULT,
    MCPD_MSG_MAX
} t_MCPD_MSGTYPES;

typedef enum mcpd_ret_codes 
{
    MCPD_SUCCESS = 0,
    MCPD_GEN_ERR = 1,
    MCPD_RET_MEMERR = 2,
    MCPD_RET_ACCEPT = 3,
    MCPD_RET_DROP   = 4
} t_MCPD_RET_CODE;

typedef enum mcpd_proto_type
{
    MCPD_PROTO_IGMP = 0,
    MCPD_PROTO_MLD  = 1,
    MCPD_PROTO_MAX  = 2,
} t_MCPD_PROTO_TYPE;

typedef struct mcpd_register 
{
    int code;
    SNOOP_REGISTRATION_TYPE registration_type;
} t_MCPD_REGISTER;

typedef struct mcpd_if_change
{
   char              ifName[IFNAMSIZ];
   t_MCPD_PROTO_TYPE proto;
} t_MCPD_IF_CHANGE;

typedef struct mcpd_pkt_info
{
    char                      br_name[IFNAMSIZ];
    char                      port_name[IFNAMSIZ];
    unsigned short            port_no;
    int                       if_index;
    int                       data_len;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    unsigned char             srcMac[6];
#endif
    unsigned short            tci;/* vlan id */
    int                       lanppp;
    int                       packetIndex; /* kernel's skb pointer */
    int                       bridgeIndex; /* kernel's bridge pointer */
} t_MCPD_PKT_INFO;

typedef struct mcpd_igmp_purge_entry
{
    struct in_addr            grp;
    struct in_addr            src;
    struct in_addr            rep;
    t_MCPD_PKT_INFO           pkt_info;
} t_MCPD_IGMP_PURGE_ENTRY;

typedef struct mcpd_igmp_purge_reporter 
{
   char                      br_name[IFNAMSIZ];
   char                      port_no;
   struct                    in_addr grp;   
} t_MCPD_IGMP_PURGE_REPORTER;

#ifdef CONFIG_BR_MLD_SNOOP
typedef struct mcpd_mld_purge_reporter 
{
   char                      br_name[IFNAMSIZ];
   char                      port_no;
   struct                    in6_addr grp;
} t_MCPD_MLD_PURGE_REPORTER;
#endif /* CONFIG_BR_MLD_SNOOP */

extern void (*bcm_mcast_def_pri_queue_hook)(struct sk_buff *);

static void mcpd_dump_buf(char *buf, int len)
{
#if 0
    int i;
    printk("========================KRNEL BPEELA START===================================\n");
    for(i =0; i < len; i++) 
    {
     printk("%02x", (unsigned char)buf[i]);
     if(!((i+1)%2))
         printk(" ");
     if(!((i+1)%16))
       printk("\n");
    }
    printk("\n");
    printk("=======================KRNL BPEELA END====================================\n");
#endif
}

/* called with rcu read lock */
/* return values:  0 not handled *
 *                 1 handled     */
int mcpd_process_skb(struct net_bridge *br, struct sk_buff *skb, unsigned short protocol)
{
    struct nlmsghdr *nlh;
    char                   *ptr = NULL;
    struct sk_buff         *new_skb;
    t_MCPD_PKT_INFO        *pkt_info;
    int                     buf_size;
    char                   *br_name = br->dev->name;
    int                     if_index = br->dev->ifindex;
    struct net_bridge_port *port;
    int                     port_no;
    int                     len;
    u8                     *pData = NULL;
    short                   type;
    struct iphdr           *pipmcast = NULL;
    struct igmphdr         *pigmp = NULL;
    int                     lanppp;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    struct ipv6hdr         *pipv6mcast = NULL;
    struct icmp6hdr        *picmpv6 = NULL;
#endif    
    int                     placePending = 0;

    if(!mcpd_pid)
        return 0;

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    if ( protocol == ETH_P_IPV6 ) {
        br_mld_get_ip_icmp_hdrs(skb, &pipv6mcast, &picmpv6, &lanppp);
        if ( picmpv6 != NULL ) {
             pData = (u8 *)pipv6mcast;
             len   = skb->len - (pData - skb->data);
             type  = MCPD_MSG_MLD_PKT;
        }
    }
    else
#endif
    if (protocol == ETH_P_IP)
    {
        br_igmp_get_ip_igmp_hdrs(skb, &pipmcast, &pigmp, &lanppp);
        if ( pigmp != NULL ) {
             pData = (u8 *)pipmcast;
             len   = skb->len - (pData - skb->data);
             type  = MCPD_MSG_IGMP_PKT;
        }
    }

    if ( pData == NULL )
    {
        return 0;
    }

    port = br_port_get_rcu(skb->dev);
    port_no = port->port_no;
    buf_size = len + sizeof(t_MCPD_MSG_HDR) + sizeof(t_MCPD_PKT_INFO);
    new_skb  = alloc_skb(NLMSG_SPACE(buf_size), GFP_ATOMIC);

    if(!new_skb) {
        printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return 0;
    }

    nlh = (struct nlmsghdr *)new_skb->data;
    ptr = NLMSG_DATA(nlh);
    nlh->nlmsg_len = NLMSG_SPACE(buf_size);
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_flags = 0;
    skb_put(new_skb, NLMSG_SPACE(buf_size));
    ((t_MCPD_MSG_HDR *)ptr)->type = type;
    ((t_MCPD_MSG_HDR *)ptr)->len = sizeof(t_MCPD_PKT_INFO);
    ptr += sizeof(t_MCPD_MSG_HDR);

    pkt_info = (t_MCPD_PKT_INFO *)ptr;

    memcpy(pkt_info->br_name, br_name, IFNAMSIZ);
    memcpy(pkt_info->port_name, skb->dev->name, IFNAMSIZ);
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    memcpy(pkt_info->srcMac, skb_mac_header(skb)+6, 6);
#endif
    pkt_info->port_no = port_no;
    pkt_info->if_index = if_index;
    pkt_info->data_len = len;
    pkt_info->tci = 0; /* should be made big endian, but it's zero anyway */
    pkt_info->lanppp = lanppp;

#if defined(CONFIG_BCM_KF_VLAN) && (defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE))
    if(skb->vlan_count)
        pkt_info->tci = htonl(skb->vlan_header[0] >> 16);
#endif /* CONFIG_BCM_VLAN) */
    ptr += sizeof(t_MCPD_PKT_INFO);

    pkt_info->packetIndex = 0;
    pkt_info->bridgeIndex = 0;
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    if ( protocol == ETH_P_IPV6 ) {
      if (mcpdControlsMldAdmission) {
        pkt_info->bridgeIndex = (int)br;
        pkt_info->packetIndex = (int)skb;
        placePending = 1;
      }
    }
    else
#endif
    {
      if (mcpdControlsIgmpAdmission) {
        pkt_info->bridgeIndex = (int)br;
        pkt_info->packetIndex = (int)skb;
        placePending = 1;
      }
    }
    memcpy(ptr, pData, len);

    NETLINK_CB(new_skb).dst_group = 0;
    NETLINK_CB(new_skb).pid = mcpd_pid;
    mcpd_dump_buf((char *)nlh, 128);

    netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);

#if 0
    mcpd_enqueue_packet(br, skb);
#endif

    return placePending;
} /* mcpd_process_skb */
EXPORT_SYMBOL(mcpd_process_skb);

static void mcpd_nl_process_set_admission_control(struct sk_buff* skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr = NLMSG_DATA(nlh);
    int changed = 0;

    ptr += sizeof(t_MCPD_MSG_HDR);

    // IGMP ADMISSION
    if ((*ptr == 0) || (*ptr == 1)) {
      if (mcpdControlsIgmpAdmission != *ptr) {
        changed = 1;
        mcpdControlsIgmpAdmission = *ptr;
        br_igmp_wipe_pending_skbs();
      }
    }
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
    ptr++;
    // MLD ADMISSION
    if ((*ptr == 0) || (*ptr == 1)) {
      if (mcpdControlsMldAdmission != *ptr) {
        changed = 1;
        mcpdControlsMldAdmission = *ptr;
        /* TBD  Insert MLD wipe code */
      }
    }
#endif
}

static void mcpd_nl_process_admission_result (struct sk_buff *skb)
{
    struct nlmsghdr*   nlh          = (struct nlmsghdr *)skb->data;
    char* ptr = NLMSG_DATA(nlh);
    t_MCPD_ADMISSION*  admit        = (t_MCPD_ADMISSION*)( ptr + sizeof(sizeof(t_MCPD_MSG_HDR)));

    if (admit == NULL) {
      return;
    }

    if ((admit->admitted == MCPD_PACKET_ADMITTED_NO) || (admit->admitted == MCPD_PACKET_ADMITTED_YES)) {
      br_igmp_process_admission (admit);
    }
    else {
      // TBD br_mld_process_admission (admit);
    }
}

int is_multicast_switching_mode_host_control(void)
{
    return (snoop_registered_pid[SNOOP_REGISTRATION_TYPE_PRIMARY] == mcpd_pid);
}

static void mcpd_nl_process_unregistration(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    struct sk_buff *new_skb = NULL;
    char *new_ptr = NULL;
    struct nlmsghdr *new_nlh = NULL;
    int buf_size;

    buf_size = NLMSG_SPACE((sizeof(t_MCPD_MSG_HDR) + sizeof(t_MCPD_REGISTER)));

    new_skb = alloc_skb(buf_size, GFP_ATOMIC);

    if(!new_skb) {
        printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return;
    }

    ptr = NLMSG_DATA(nlh);

    new_nlh = (struct nlmsghdr *)new_skb->data;
    new_ptr = NLMSG_DATA(new_nlh);
    new_nlh->nlmsg_len = buf_size;
    new_nlh->nlmsg_pid = 0;
    new_nlh->nlmsg_flags = 0;
    skb_put(new_skb, buf_size);
    ((t_MCPD_MSG_HDR *)new_ptr)->type = MCPD_MSG_UNREGISTER;
    ((t_MCPD_MSG_HDR *)new_ptr)->len = sizeof(t_MCPD_REGISTER);
    new_ptr += sizeof(t_MCPD_MSG_HDR);
    ((t_MCPD_REGISTER *)new_ptr)->code = MCPD_SUCCESS;

    NETLINK_CB(new_skb).dst_group = 0;
    NETLINK_CB(new_skb).pid = mcpd_pid;

    netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);
    mcpd_pid = snoop_registered_pid[SNOOP_REGISTRATION_TYPE_DEFAULT];

    return;
} /* mcpd_nl_process_unregistration */

static void mcpd_nl_process_registration(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    struct sk_buff *new_skb = NULL;
    char *new_ptr = NULL;
    struct nlmsghdr *new_nlh = NULL;
    int buf_size;

    buf_size = NLMSG_SPACE((sizeof(t_MCPD_MSG_HDR) + sizeof(t_MCPD_REGISTER)));

    new_skb = alloc_skb(buf_size, GFP_ATOMIC);

    if(!new_skb) {
        printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return;
    }

    ptr = NLMSG_DATA(nlh);

    if(new_skb)
    {
        int registration_type;

        new_nlh = (struct nlmsghdr *)new_skb->data;
        new_ptr = NLMSG_DATA(new_nlh);
        new_nlh->nlmsg_len = buf_size;
        new_nlh->nlmsg_pid = 0;
        new_nlh->nlmsg_flags = 0;
        skb_put(new_skb, buf_size);
        ((t_MCPD_MSG_HDR *)new_ptr)->type = MCPD_MSG_REGISTER;
        ((t_MCPD_MSG_HDR *)new_ptr)->len = sizeof(t_MCPD_REGISTER);
        new_ptr += sizeof(t_MCPD_MSG_HDR);
        ptr += sizeof(t_MCPD_MSG_HDR);
        ((t_MCPD_REGISTER *)new_ptr)->code = MCPD_SUCCESS;
        registration_type = ((t_MCPD_REGISTER *)ptr)->registration_type; 
        if (!snoop_registered_pid[registration_type])
            snoop_registered_pid[registration_type] = nlh->nlmsg_pid;
        if ((mcpd_pid) && (mcpd_pid != snoop_registered_pid[registration_type]/*nlh->nlmsg_pid*/))
        {
            struct sk_buff *new_skb2 = alloc_skb(buf_size, GFP_ATOMIC);
            if(!new_skb2) {
                printk("br_netlink_mcpd.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
                return;
            }

            mcpd_nl_process_unregistration(new_skb2);
        }

        NETLINK_CB(new_skb).dst_group = 0;

        mcpd_pid = snoop_registered_pid[registration_type];
        NETLINK_CB(new_skb).pid = mcpd_pid;


        netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);
    }

    return;
} /* mcpd_nl_process_registration */


int mcpd_is_br_port(struct net_bridge *br,struct net_device *from_dev)
{
    struct net_bridge_port *p = NULL;
    int ret = 0;

    rcu_read_lock();
    list_for_each_entry_rcu(p, &br->port_list, list) {
        if ((p->dev) && (!memcmp(p->dev->name, from_dev->name, IFNAMSIZ)))
            ret = 1;
    }
    rcu_read_unlock();

    return ret;
} /* br_igmp_is_br_port */

static void mcpd_nl_process_igmp_snoop_entry(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    struct net_bridge_port *prt;
    t_MCPD_IGMP_SNOOP_ENTRY *snoop_entry;
    unsigned char *ptr;
    struct net_device *from_dev= NULL;
    int idx = 0;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    snoop_entry = (t_MCPD_IGMP_SNOOP_ENTRY *)ptr;

    dev = dev_get_by_name(&init_net, snoop_entry->br_name);
    if(NULL == dev)
        return;

    if ((0 == (dev->priv_flags & IFF_EBRIDGE)) ||
        (0 == (dev->flags & IFF_UP)))
    {
        printk("%s: invalid bridge %s for snooping entry\n", 
               __FUNCTION__, snoop_entry->br_name);
        dev_put(dev);
        return;
    }
    br = netdev_priv(dev);

    for(idx = 0; idx < MCPD_MAX_IFS; idx++)
    {
        if(snoop_entry->wan_info[idx].if_ops)
        {
            from_dev = dev_get_by_name(&init_net, 
                                       snoop_entry->wan_info[idx].if_name);
            if (NULL == from_dev)
               continue;

            rcu_read_lock();
            prt = br_get_port(br, snoop_entry->port_no);
            if ( NULL == prt )
            {
               printk("%s: port %d could not be found in br %s\n", 
                      __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
               rcu_read_unlock();
               dev_put(from_dev);
               dev_put(dev);
               return;
            }

            if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
               (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
            {
                br_igmp_mc_fdb_remove(from_dev,
                                  br, 
                                  prt, 
                                  &snoop_entry->destGrp, 
                                  &snoop_entry->rep, 
                                  snoop_entry->mode, 
                                  &snoop_entry->src);
            }
            else
            {
                if((snoop_entry->wan_info[idx].if_ops == MCPD_IF_TYPE_BRIDGED) && 
                   (!mcpd_is_br_port(br, from_dev)))
                {
                   rcu_read_unlock();
                   dev_put(from_dev);
                   continue;
                }

                if (0 == (prt->dev->flags & IFF_UP)) {
                   printk("%s: port %d is not up %s\n", 
                          __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
                   rcu_read_unlock();
                   dev_put(from_dev);
                   dev_put(dev);
                   return;
                }
    
                if (0 == (from_dev->flags & IFF_UP)) {
                   printk("%s: source device %s is not up\n", 
                          __FUNCTION__, from_dev->name);
                   rcu_read_unlock();
                   dev_put(from_dev);
                   continue;
                }

                br_igmp_mc_fdb_add(from_dev,
                               snoop_entry->wan_info[idx].if_ops,
                               br, 
                               prt, 
                               &snoop_entry->grp,
                               &snoop_entry->destGrp,
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci,
                               &snoop_entry->src,
                               snoop_entry->lanppp);
            }
            rcu_read_unlock();
            dev_put(from_dev);
        }
        else
        {
            break;
        }
    }

    /* if LAN-2-LAN snooping enabled make an entry                         *
     * unless multicast DNAT is being used (grp and destGrp are different) */
    if (br_mcast_get_lan2lan_snooping(BR_MCAST_PROTO_IGMP, br) &&
        (snoop_entry->grp.s_addr == snoop_entry->destGrp.s_addr) ) 
    {
        rcu_read_lock();
        prt = br_get_port(br, snoop_entry->port_no);
        if ( NULL == prt )
        {
           printk("%s: port %d could not be found in br %s\n", 
                  __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
           rcu_read_unlock();
           dev_put(dev);
           return;
        }

        if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
            (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
        {
            br_igmp_mc_fdb_remove(dev,
                                    br, 
                                    prt, 
                                    &snoop_entry->grp, 
                                    &snoop_entry->rep, 
                                    snoop_entry->mode, 
                                    &snoop_entry->src);
        }
        else
        {
            if (0 == (prt->dev->flags & IFF_UP)) {
               printk("%s: port %d is not up %s\n", 
                      __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
               rcu_read_unlock();
               dev_put(dev);
               return;
            }

            br_igmp_mc_fdb_add(dev,
                               MCPD_IF_TYPE_BRIDGED,
                               br, 
                               prt, 
                               &snoop_entry->grp, 
                               &snoop_entry->grp, 
                               &snoop_entry->rep, 
                               snoop_entry->mode, 
                               snoop_entry->tci,
                               &snoop_entry->src,
                               snoop_entry->lanppp);
        }
        rcu_read_unlock();
    }
        dev_put(dev);

    return;
} /* mcpd_nl_process_igmp_snoop_entry*/

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
static void mcpd_nl_process_mld_snoop_entry(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    struct net_bridge_port *prt;
    t_MCPD_MLD_SNOOP_ENTRY *snoop_entry;
    unsigned char *ptr;
    struct net_device *from_dev= NULL;
    int idx = 0;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    snoop_entry = (t_MCPD_MLD_SNOOP_ENTRY *)ptr;

    dev = dev_get_by_name(&init_net, snoop_entry->br_name);
    if(NULL == dev)
        return;

    if ((0 == (dev->priv_flags & IFF_EBRIDGE)) ||
        (0 == (dev->flags & IFF_UP)))
    {
        printk("%s: invalid bridge %s for snooping entry\n", 
               __FUNCTION__, snoop_entry->br_name);
        dev_put(dev);
        return;
    }
    br = netdev_priv(dev);

    for(idx = 0; idx < MCPD_MAX_IFS; idx++)
    {
        if(snoop_entry->wan_info[idx].if_ops)
        {
            from_dev = dev_get_by_name(&init_net, 
                                       snoop_entry->wan_info[idx].if_name);
            if(NULL == from_dev)
               continue;

            rcu_read_lock();
            prt = br_get_port(br, snoop_entry->port_no);
            if ( NULL == prt )
            {
               printk("%s: port %d could not be found in br %s\n", 
                      __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
               rcu_read_unlock();
               dev_put(from_dev);
               dev_put(dev);
               return;
            }

            if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
                (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
            {
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
                mcast_snooping_call_chain(SNOOPING_DEL_ENTRY,snoop_entry);
#endif
                br_mld_mc_fdb_remove(from_dev,
                                    br, 
                                    prt, 
                                    &snoop_entry->grp,
                                    &snoop_entry->rep, 
                                    snoop_entry->mode, 
                                    &snoop_entry->src);
            }
            else
            {
                if((snoop_entry->wan_info[idx].if_ops == MCPD_IF_TYPE_BRIDGED) && 
                   (!mcpd_is_br_port(br, from_dev)))
                {
                   rcu_read_unlock();
                   dev_put(from_dev);
                   continue;
                }

                if (0 == (prt->dev->flags & IFF_UP)) {
                   printk("%s: port %d is not up %s\n", 
                          __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
                   rcu_read_unlock();
                   dev_put(from_dev);
                   dev_put(dev);
                   return;
                }
    
                if (0 == (from_dev->flags & IFF_UP)) {
                   printk("%s: source device %s is not up\n", 
                          __FUNCTION__, from_dev->name);
                   rcu_read_unlock();
                   dev_put(from_dev);
                   continue;
                }

                br_mld_mc_fdb_add(from_dev,
                                snoop_entry->wan_info[idx].if_ops,
                                br, 
                                prt, 
                                &snoop_entry->grp, 
                                &snoop_entry->rep, 
                                snoop_entry->mode, 
                                snoop_entry->tci, 
                                &snoop_entry->src,
                                snoop_entry->lanppp);
                mcast_snooping_call_chain(SNOOPING_ADD_ENTRY,snoop_entry);
            }
            rcu_read_unlock();
            dev_put(from_dev);
        }
        else
        {
            break;
        }
    }

    /* if LAN-2-LAN snooping enabled make an entry */
    if(br_mcast_get_lan2lan_snooping(BR_MCAST_PROTO_MLD, br))
    {
        rcu_read_lock();
        prt = br_get_port(br, snoop_entry->port_no);
        if ( NULL == prt )
        {
           printk("%s: port %d could not be found in br %s\n", 
                  __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
           rcu_read_unlock();
           dev_put(dev);
           return;
        }

        if((snoop_entry->mode == MCPD_SNOOP_IN_CLEAR) ||
            (snoop_entry->mode == MCPD_SNOOP_EX_CLEAR)) 
        {
            mcast_snooping_call_chain(SNOOPING_DEL_ENTRY,snoop_entry);
            br_mld_mc_fdb_remove(dev,
                                 br, 
                                 prt, 
                                 &snoop_entry->grp, 
                                 &snoop_entry->rep, 
                                 snoop_entry->mode, 
                                 &snoop_entry->src);
        }
        else
        {
            if (0 == (prt->dev->flags & IFF_UP)) {
               printk("%s: port %d is not up %s\n", 
                      __FUNCTION__, snoop_entry->port_no, snoop_entry->br_name);
               rcu_read_unlock();
               dev_put(dev);
               return;
            }

            br_mld_mc_fdb_add(dev,
                              MCPD_IF_TYPE_BRIDGED,
                              br, 
                              prt, 
                              &snoop_entry->grp, 
                              &snoop_entry->rep, 
                              snoop_entry->mode, 
                              snoop_entry->tci, 
                              &snoop_entry->src,
                              snoop_entry->lanppp);
            mcast_snooping_call_chain(SNOOPING_ADD_ENTRY,snoop_entry);
        }
        rcu_read_unlock();
    }
        dev_put(dev);

    return;
} /* mcpd_nl_process_mld_snoop_entry*/
#endif

static void mcpd_nl_process_uplink_indication(struct sk_buff const *skb)
{
    struct nlmsghdr const *nlh = (struct nlmsghdr *)skb->data;
    unsigned char *ptr;
    int uplinkIndicator;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    uplinkIndicator = (int)*(int *)ptr;

    br_mcast_set_uplink_exists(uplinkIndicator);

    return;
}

static void mcpd_nl_process_igmp_purge_reporter (struct sk_buff *skb)
{
   struct nlmsghdr const *nlh = (struct nlmsghdr *)skb->data;
   t_MCPD_IGMP_PURGE_REPORTER* purge_data = NULL;
   unsigned char *ptr;
   struct net_device *dev = NULL;
   struct net_bridge *br = NULL;

   ptr = NLMSG_DATA(nlh);
   ptr += sizeof(t_MCPD_MSG_HDR);

   purge_data = (t_MCPD_IGMP_PURGE_REPORTER*) ptr;
   
   dev = dev_get_by_name (&init_net, purge_data->br_name);
   if (dev == NULL) {
      return;
   }

   br = (struct net_bridge *)netdev_priv(dev);
   if (br == NULL) {
      dev_put(dev);
      return;
   }

   br_igmp_wipe_reporter_for_port(br, &purge_data->grp, (u16)purge_data->port_no);
   dev_put(dev);
}

#ifdef CONFIG_BR_MLD_SNOOP
static void mcpd_nl_process_mld_purge_reporter (struct sk_buff *skb)
{
   struct nlmsghdr const *nlh = (struct nlmsghdr *)skb->data;
   t_MCPD_MLD_PURGE_REPORTER* purge_data = NULL;
   unsigned char *ptr;
   struct net_device *dev = NULL;
   struct net_bridge *br = NULL;

   ptr = NLMSG_DATA(nlh);
   ptr += sizeof(t_MCPD_MSG_HDR);

   purge_data = (t_MCPD_MLD_PURGE_REPORTER*) ptr;

   if (purge_data == NULL) {
      return;
   }

   dev = dev_get_by_name (&init_net, purge_data->br_name);
   if (dev == NULL) {
      return;
   }

   br = netdev_priv(dev);
   if (br == NULL) {
      dev_put(dev);
      return;
   }

   br_mld_wipe_reporter_for_port(br, &purge_data->grp, (u16)purge_data->port_no);
   dev_put(dev);
}
#endif
   
static void mcpd_nl_process_mcast_pri_queue (struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    unsigned char *ptr;
    int val;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);

    val = (int)*(int *)ptr;

    br_mcast_set_pri_queue(val);

    return;
}

static void mcpd_nl_process_if_change(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    struct net_device *ndev = NULL;
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;
    unsigned char *ptr;
    t_MCPD_IF_CHANGE *ifChgMsg;

    ptr = NLMSG_DATA(nlh);
    ptr += sizeof(t_MCPD_MSG_HDR);
    ifChgMsg = (t_MCPD_IF_CHANGE *)ptr;
    ndev = dev_get_by_name(&init_net, &ifChgMsg->ifName[0]);
    if(!ndev)
        return;

    if (ndev->priv_flags & IFF_EBRIDGE)
    {
        br = netdev_priv(ndev);
        if ( NULL == br )
        {
            dev_put(ndev);
            return;
        }

        /* update is for a bridge interface so flush all entries */
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
        if ( MCPD_PROTO_MLD != ifChgMsg->proto )
        {
            br_igmp_process_if_change(br, NULL);
            br_igmp_process_device_removal(ndev);
        }
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
        if ( MCPD_PROTO_IGMP != ifChgMsg->proto )
        {
            br_mld_process_if_change(br, NULL);
        }
#endif
    }
    else
    {
        rcu_read_lock();
        for_each_netdev_rcu(&init_net, dev)
        {
            if(dev->priv_flags & IFF_EBRIDGE)
            {
                br = netdev_priv(dev);
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
                if ( MCPD_PROTO_MLD != ifChgMsg->proto )
                {
                    br_igmp_process_if_change(br, ndev);
                }
#endif
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
                if ( MCPD_PROTO_IGMP != ifChgMsg->proto )
                {
                    br_mld_process_if_change(br, ndev);
                }
#endif
            }
        }
        rcu_read_unlock();
    }

    dev_put(ndev);

    return;
} /* mcpd_nl_process_if_change */

static void mcpd_nl_process_mc_fdb_cleanup(void)
{
    struct net_device *dev = NULL;
    struct net_bridge *br = NULL;

    rcu_read_lock();
    for_each_netdev_rcu(&init_net, dev)
    {
        br = netdev_priv(dev);
        if((dev->priv_flags & IFF_EBRIDGE) && (br))
        {
#if defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)
            if(br->igmp_snooping) {
                br_igmp_mc_fdb_cleanup(br);
                br_igmp_process_device_removal(dev);
            }
#endif            

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
            if(br->mld_snooping) {
                br_mld_mc_fdb_cleanup(br);
            }
#endif
        }
    }
    rcu_read_unlock();
    return;
}

void mcpd_nl_send_igmp_purge_entry(struct net_bridge_mc_fdb_entry *igmp_entry)
{
    t_MCPD_IGMP_PURGE_ENTRY *purge_entry;
    int buf_size = 0;
    struct sk_buff *new_skb;
    struct nlmsghdr *nlh;
    char *ptr = NULL;
    struct net_bridge_mc_rep_entry *rep = NULL;

    if(!igmp_entry)
        return;

    rep = list_first_entry(&igmp_entry->rep_list, struct net_bridge_mc_rep_entry, list);

    if(!rep)
        return;

    if(!mcpd_pid)
        return;

    buf_size = sizeof(t_MCPD_IGMP_PURGE_ENTRY) + sizeof(t_MCPD_MSG_HDR);
    new_skb = alloc_skb(NLMSG_SPACE(buf_size), GFP_ATOMIC);

    if(!new_skb) 
    {
        return;
    }

    nlh = (struct nlmsghdr *)new_skb->data;
    ptr = NLMSG_DATA(nlh);
    nlh->nlmsg_len = NLMSG_SPACE(buf_size);
    nlh->nlmsg_pid = 0;
    nlh->nlmsg_flags = 0;
    skb_put(new_skb, NLMSG_SPACE(buf_size));
    ((t_MCPD_MSG_HDR *)ptr)->type = MCPD_MSG_IGMP_PURGE_ENTRY;
    ((t_MCPD_MSG_HDR *)ptr)->len = sizeof(t_MCPD_IGMP_PURGE_ENTRY);
    ptr += sizeof(t_MCPD_MSG_HDR);

    purge_entry = (t_MCPD_IGMP_PURGE_ENTRY *)ptr;

    purge_entry->grp.s_addr = igmp_entry->grp.s_addr;
    purge_entry->src.s_addr = igmp_entry->src_entry.src.s_addr;
    purge_entry->rep.s_addr = rep->rep.s_addr;

    purge_entry->pkt_info.br_name[0] = '\0';
    memcpy(purge_entry->pkt_info.port_name, igmp_entry->dst->dev->name, IFNAMSIZ);
    purge_entry->pkt_info.port_no = igmp_entry->dst->port_no;
    purge_entry->pkt_info.if_index = 0;
    purge_entry->pkt_info.data_len = 0;

#if defined(CONFIG_BCM_KF_VLAN) && (defined(CONFIG_BCM_VLAN) || defined(CONFIG_BCM_VLAN_MODULE))
    purge_entry->pkt_info.tci = igmp_entry->lan_tci;
#endif /* CONFIG_BCM_VLAN */

    NETLINK_CB(new_skb).dst_group = 0;
    NETLINK_CB(new_skb).pid = mcpd_pid;
    mcpd_dump_buf((char *)nlh, 128);

    netlink_unicast(nl_sk, new_skb, mcpd_pid, MSG_DONTWAIT);

    return;
} /* mcpd_nl_send_igmp_purge_entry */

static inline void mcpd_nl_rcv_skb(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    unsigned short msg_type;

    if (skb->len >= NLMSG_SPACE(0)) 
    {
        if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
            return;

        ptr = NLMSG_DATA(nlh);

        msg_type = *(unsigned short *)ptr;
        switch(msg_type)
        {
            case MCPD_MSG_REGISTER:
                mcpd_nl_process_registration(skb);
                break;

            case MCPD_MSG_UNREGISTER:
                mcpd_nl_process_unregistration(skb);
                break;

            case MCPD_MSG_IGMP_SNOOP_ENTRY:
                mcpd_nl_process_igmp_snoop_entry(skb);
                break;
                
#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
            case MCPD_MSG_MLD_SNOOP_ENTRY:
                mcpd_nl_process_mld_snoop_entry(skb);
                break;
#endif

            case MCPD_MSG_IF_CHANGE:
                mcpd_nl_process_if_change(skb);
                break;

            case MCPD_MSG_MC_FDB_CLEANUP:
                mcpd_nl_process_mc_fdb_cleanup();
                break;

            case MCPD_MSG_SET_PRI_QUEUE:
                mcpd_nl_process_mcast_pri_queue(skb);
                break;

            case MCPD_MSG_UPLINK_INDICATION:
                mcpd_nl_process_uplink_indication(skb);
                break;

            case MCPD_MSG_IGMP_PURGE_REPORTER:
                mcpd_nl_process_igmp_purge_reporter(skb);
                break;
                
#ifdef CONFIG_BR_MLD_SNOOP
            case MCPD_MSG_MLD_PURGE_REPORTER:
                mcpd_nl_process_mld_purge_reporter(skb);
                break;
#endif
            case MCPD_MSG_CONTROLS_ADMISSION:
                mcpd_nl_process_set_admission_control(skb);
                break;

            case MCPD_MSG_ADMISSION_RESULT:
                mcpd_nl_process_admission_result(skb);
                break;
                
            default:
                printk("MCPD Unknown usr->krnl msg type -%d- \n", msg_type);
        }
    }

    return;
} /* mcpd_nl_rcv_skb */

#if 0
static void mcpd_nl_data_ready(struct sock *sk, int len)
{
    struct sk_buff *skb = NULL;
    unsigned int qlen = skb_queue_len(&sk->sk_receive_queue);

    while (qlen-- && (skb = skb_dequeue(&sk->sk_receive_queue))) 
    {
        mcpd_nl_rcv_skb(skb);
        kfree_skb(skb);
    }
} /* mcpd_nl_data_ready */
#endif

static int __init mcpd_module_init(void)
{
    printk(KERN_INFO "Initializing MCPD Module\n");

    nl_sk = netlink_kernel_create(&init_net, NETLINK_MCPD, 0, 
                                mcpd_nl_rcv_skb, NULL, THIS_MODULE);

    if(nl_sk == NULL) 
    {
        printk("MCPD: failure to create kernel netlink socket\n");
        return -ENOMEM;
    }

    bcm_mcast_def_pri_queue_hook = br_mcast_set_skb_mark_queue;    

    return 0;
} /* mcpd_module_init */

static void __exit mcpd_module_exit(void)
{
    sock_release(nl_sk->sk_socket); 
    printk(KERN_INFO "Removed MCPD\n");
} /* mcpd_module_exit */

module_init(mcpd_module_init);
module_exit(mcpd_module_exit);

#endif /* defined(CONFIG_BCM_KF_MLD) || defined(CONFIG_BCM_KF_IGMP) */
