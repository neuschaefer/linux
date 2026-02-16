/*
 * ti_igmp.h
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _TI_IGMP_H
#define _TI_IGMP_H

/* group states */
#define IDLE_MEMBER_STATE               0        /* Non-Member     */
#define JOINED_MEMBER_STATE             1        /* Joined-Member  */
#define JOIN_IN_PROCESS_STATE           2        /* Wait-to-Join   */
#define LEAVE_STATE                     0
#define JOIN_STATE                      1

#define IGMP_HFC_INT_INDEX              0
#define IGMP_CMCI_INT_INDEX             1

#define NUM_OF_MULTICAST_INTERFACES     2

struct igmp_cfg
{
    int enabled;
    int igmp_mqi_timeout;
    int igmp_qri_timeout;
    int igmp_m1_max_timeout;
    int igmp_docsis_robustness;
    /* Callback function to send IGMP packet via DOCSIS bridge. Interface specfies
     * whether packet has to be sent to CMCI or CABLE interface
     */
    int (*ti_igmp_send_packet)(struct sk_buff *skb, unsigned long device_map, unsigned long device_type);
    /* Callback function to notify DOCSIS of IGMP JOIN/LEAVE messages.
     * MAC address is source mac of JOIN/LEAVE message.
     * Message type specifies JOIN(1)/LEAVE(0).
     */
    int (*ti_igmp_message_processed)(char *mac_address, int msg_type);
};

struct igmp_stats
{
   unsigned long  queryInterval;
   unsigned long  version;
   unsigned long  querier;
   unsigned long  queryMaxResTime;
   unsigned long  version1QuerierTimer;
   unsigned long  wrongVerQueries;
   unsigned long  joins;
   unsigned long  groups;
   unsigned long  robustness;
   unsigned long  lastMemQueryIntrvl;
   unsigned long  proxyIfIndex;
   unsigned long  querierUpTime;
   unsigned long  querierExpiryTime;
};

struct igmp_cache_entry
{
    /* Links to the next and prev cache entry */
    struct list_head    links;
    unsigned char       state;
    unsigned char       blocked;
    unsigned char       static_address;
    unsigned long       group_address;
    unsigned char       MRselfSentFlag;
    unsigned long       M1ExpireTime;
    unsigned long       M2ExpireTime;
    /* Store the packet */
    struct sk_buff      *skb;
    /* Devices map from where JOIN request was received */
    unsigned long       input_dev;
	unsigned char	    mac_address[ETH_ALEN];	/* source MAC address */
};

extern int ti_igmp_init (struct igmp_cfg *cfg);
extern int ti_igmp_start (void);
extern int ti_igmp_stop (void);
extern int ti_igmp_packet_handler(struct igmphdr *igmp_hdr, struct sk_buff *skb, unsigned int device_type);
extern unsigned long ti_get_igmp_devices(unsigned long ip_addr);
extern void display_igmp_cache_entry(void);
#endif /* _TI_IGMP_H */
