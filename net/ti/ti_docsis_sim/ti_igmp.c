/*
 * ti_igmp.c
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

#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/igmp.h>
#include <linux/list.h>
#include <linux/random.h>
#include "ti_igmp.h"

#ifdef CONFIG_TI_L2_IGMP
#undef CONFIG_TI_L2_IGMP_DEBUG

#define FALSE       0
#define TRUE        1
#define IGMP_INIT   1
#define IGMP_START  2
#define IGMP_STOP   3

#define DBR_CMCI_NET_DEV_TYPE       0x1  /* 0000 0001 */
#define DBR_CABLE_NET_DEV_TYPE      0x2  /* 0000 0010 */
#define DBR_ESAFE_NET_DEV_TYPE      0x4  /* 0000 0100 */
#define DBR_WAN_IP_NET_DEV_TYPE     0x8  /* 0000 1000 */
#define DBR_LAN_IP_NET_DEV_TYPE     0x10 /* 0001 0000 */

typedef struct TI_IGMP_MCB
{
    /* IGMP State machine - INIT, START, STOP */
    int state;

    /* Configuration received from DOCSIS */
    struct igmp_cfg     cfg;

    /* Statistics */
    struct igmp_stats   igmp_stats[NUM_OF_MULTICAST_INTERFACES];

    /* Total number of groups registered via JOIN */
    int                 number_of_groups;

    /* Time when last last was recived. Required to recompute MQI */
    unsigned long       last_MQ_time;

    /* 1 sec IGMP Timer */
    struct timer_list   igmp_timer;
}TI_IGMP_MCB;

TI_IGMP_MCB  ti_igmp_mcb;

LIST_HEAD (igmp_cache);

static void initialize_igmp_configuration(struct igmp_cfg *cfg)
{
    memcpy ((void *)&ti_igmp_mcb.cfg, (void *)cfg, sizeof (struct igmp_cfg));
  	ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryInterval = ti_igmp_mcb.cfg.igmp_mqi_timeout;
  	ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].version = 2;
  	ti_igmp_mcb.igmp_stats[IGMP_HFC_INT_INDEX].version = 2;
    return;
}

/**************************************************************************
 * FUNCTION NAME : mark_device_in_group
 **************************************************************************
 * DESCRIPTION   :
 *  Delete IGMP Cache entry
 *
 * RETURNS       :
 * None
 *
 **************************************************************************/
static void mark_device_in_group(struct igmp_cache_entry* group_info,
                            unsigned long dev, unsigned char flag)
{
    if (flag)
        group_info->input_dev |= ((unsigned long) 1 << dev);
    else
        group_info->input_dev &= (0xffffffff ^ ((unsigned long) 1 << dev));
    return;
}

/**************************************************************************
 * FUNCTION NAME : create_igmp_cache_entry
 **************************************************************************
 * DESCRIPTION   :
 *  Create IGMP Cache entry
 *
 * RETURNS       :
 *  NULL  -   Error
 *  Pointer to the cache entry
 *
 **************************************************************************/
static struct igmp_cache_entry* create_igmp_cache_entry(unsigned long ip_addr)
{
    struct igmp_cache_entry   *group_info;

    /* Allocate memory for Cache. */
    group_info = (struct igmp_cache_entry *) kmalloc(sizeof(struct igmp_cache_entry), GFP_KERNEL);
    if (group_info == NULL)
    {
        printk ("Error: Failed to allocate memory for IGMP cache\n");
        return NULL;
    }

    group_info->state = IDLE_MEMBER_STATE;
    group_info->blocked = FALSE;
    group_info->static_address = FALSE;
    group_info->M1ExpireTime = 0;
    group_info->M2ExpireTime = 0;
    group_info->skb = NULL;
    group_info->input_dev = 0;
    group_info->group_address = ip_addr;

    ti_igmp_mcb.number_of_groups++;

    /* Add to new entry to global IGMP Cache list */
    list_add_tail((struct list_head *)&group_info->links, &igmp_cache);
    return group_info;
}


/**************************************************************************
 * FUNCTION NAME : delete_igmp_cache_entry
 **************************************************************************
 * DESCRIPTION   :
 *  Delete IGMP Cache entry
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void delete_igmp_cache_entry(struct igmp_cache_entry* group_info)
{
    /* Remove the cache entry from the global IGMP cache list. */
    list_del((struct list_head *)&group_info->links);

    /* Free allocated memory */
    kfree (group_info);
}

/**************************************************************************
 * FUNCTION NAME : flush_igmp_cache_entry
 **************************************************************************
 * DESCRIPTION   :
 *  Flush IGMP Cache
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void flush_igmp_cache_entry(void)
{
    struct igmp_cache_entry *group_info;

	while (!list_empty(&igmp_cache))
    {
        group_info = (struct igmp_cache_entry *)list_entry(igmp_cache.next,
				 struct igmp_cache_entry, links);

        /* Free any skb stored */
        if (group_info->skb)
            kfree_skb(group_info->skb);

        /* Remove the cache entry from the global IGMP cache list. */
        list_del((struct list_head *)&group_info->links);

        /* Free allocated memory */
        kfree (group_info);
    }
    ti_igmp_mcb.number_of_groups = 0;
}

/**************************************************************************
 * FUNCTION NAME : get_igmp_cache_entry
 **************************************************************************
 * DESCRIPTION   :
 *  Get IGMP Cache entry
 *
 * RETURNS       :
 *  NULL  -   Error
 *  Pointer to the cache entry
 *
 *
 **************************************************************************/
static struct igmp_cache_entry* get_igmp_cache_entry(unsigned long ip_addr)
{
    struct list_head        *ptr_temp;
    struct igmp_cache_entry *group_info;

    list_for_each(ptr_temp, &igmp_cache)
    {
        /* Get the WAN Bridge Information and check for the name. */
        group_info = (struct igmp_cache_entry *)ptr_temp;
        if (group_info->group_address == ip_addr)
            return group_info;
    }
    return NULL;
}

/**************************************************************************
 * FUNCTION NAME : ti_get_igmp_devices
 **************************************************************************
 * DESCRIPTION   :
 *  Called by external entity to get devices on which IGMP JOIN was received
 *
 * RETURNS       :
 *  0   -   If no IP address match
 *  Device map - Indicate what netdevices IGMP JOIN was received on
 *
 **************************************************************************/
unsigned long ti_get_igmp_devices(unsigned long ip_addr)
{
    struct list_head        *ptr_temp;
    struct igmp_cache_entry *group_info;

    if (ti_igmp_mcb.state != IGMP_START)
        return 0;

    list_for_each(ptr_temp, &igmp_cache)
    {
        /* Get the WAN Bridge Information and check for the name. */
        group_info = (struct igmp_cache_entry *)ptr_temp;
        if (group_info->group_address == ip_addr)
        {
#ifdef CONFIG_TI_L2_IGMP_DEBUG
            printk ("DEBUG: Input dev is %lu\n", group_info->input_dev);
#endif
            return group_info->input_dev;
        }
    }
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : display_igmp_cache_entry
 **************************************************************************
 * DESCRIPTION   :
 *  Display IGMP Cache entries
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
void display_igmp_cache_entry(void)
{
    struct list_head        *ptr_temp;
    struct igmp_cache_entry *group_info;

    printk ("IGMP Cache                     \n");
    printk ("----------                     \n");
    printk ("IGMP Total Groups              %d\n", ti_igmp_mcb.number_of_groups);

    list_for_each(ptr_temp, &igmp_cache)
    {
        /* Get the WAN Bridge Information and check for the name. */
        group_info = (struct igmp_cache_entry *)ptr_temp;
        printk ("IGMP Group address         %4X \n", group_info->group_address);
        printk ("IGMP Device map            %4X \n", group_info->input_dev);
        printk ("IGMP SKB                   0x%p \n", group_info->skb);
        printk ("IGMP Group M1 Timer        %lu \n", group_info->M1ExpireTime);
        printk ("IGMP Group M2 Timer        %lu \n", group_info->M2ExpireTime);
        printk ("Source MAC Address         0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\n",
            group_info->mac_address[0], group_info->mac_address[1], group_info->mac_address[2],
            group_info->mac_address[3], group_info->mac_address[4], group_info->mac_address[5]);
        printk ("\n");
    }
    return;
}

/**************************************************************************
 * FUNCTION NAME : igmp_start_M1_timer
 **************************************************************************
 * DESCRIPTION   :
 *  Start M1 timer
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void igmp_start_M1_timer(struct igmp_cache_entry *group_info)
{
    unsigned long random_number;

    /* M1 timer is set to a random value between 0~3 seconds */

    get_random_bytes(&random_number, 4);

    group_info->M1ExpireTime = (random_number % (ti_igmp_mcb.cfg.igmp_m1_max_timeout + 1));

    if (group_info->M1ExpireTime == 0)
        group_info->M1ExpireTime++;
}

/**************************************************************************
 * FUNCTION NAME : igmp_start_M2_timer
 **************************************************************************
 * DESCRIPTION   :
 *  Start M2 timer
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void igmp_start_M2_timer(struct igmp_cache_entry *group_info, unsigned char group_flag)
{
    unsigned long queryMaxResTime = ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryMaxResTime;
    unsigned long queryInterval = ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryInterval;

    /* Handle IGMPv1 query */
    if (queryMaxResTime == 0 )
        queryMaxResTime = ti_igmp_mcb.cfg.igmp_qri_timeout;

    if (group_flag)
    {
        /* If group specific query, restart the M2 timer */
        group_info->M2ExpireTime = (queryMaxResTime / 10) * ti_igmp_mcb.cfg.igmp_docsis_robustness;
    }
    else
    {
        /* M2 timer = 2 * MQI + MRI .  */
        group_info->M2ExpireTime = (queryInterval * ti_igmp_mcb.cfg.igmp_docsis_robustness)
                                        + (queryMaxResTime / 10);
    }
    if (group_info->M2ExpireTime == 0)
        group_info->M2ExpireTime++;
}

/**************************************************************************
 * FUNCTION NAME : igmp_handle_M1_timeout
 **************************************************************************
 * DESCRIPTION   :
 *  Handles M1 timer timeout
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void igmp_handle_M1_timeout(struct igmp_cache_entry *group_info)
{
    if (group_info == NULL)
    {
        printk ("Error: M1 Timeout on non-existent group\n");
        return;
    }
    else
    {
        if(group_info->state == JOIN_IN_PROCESS_STATE)
        {
            /* Change state to JOINED */
            group_info->state = JOINED_MEMBER_STATE;

            /* Send IGMP MR to Cable Interface */
            ti_igmp_mcb.cfg.ti_igmp_send_packet(group_info->skb, group_info->input_dev,
                DBR_CABLE_NET_DEV_TYPE);

            group_info->skb = NULL;
            /* Last MR was sent by us */
            group_info->MRselfSentFlag = TRUE;
        }
    }
}

/**************************************************************************
 * FUNCTION NAME : igmp_handle_M2_timeout
 **************************************************************************
 * DESCRIPTION   :
 *  Handles M2 timer timeout
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void igmp_handle_M2_timeout(struct igmp_cache_entry *group_info)
{
    if (group_info == NULL)
    {
        printk ("Error: M2 Timeout on non-existent group\n");
        return;
    }
    else
    {
        /* Notify external entity a LEAVE message or timeout was processed */
        ti_igmp_mcb.cfg.ti_igmp_message_processed(group_info->mac_address, LEAVE_STATE);

        /* Free any skb stored */
        if (group_info->skb)
            kfree_skb(group_info->skb);
        delete_igmp_cache_entry(group_info);

        ti_igmp_mcb.number_of_groups--;
    }
    return;
}

/**************************************************************************
 * FUNCTION NAME : ti_igmp_timer_expired
 **************************************************************************
 * DESCRIPTION   :
 *  TI Layer2 IGMP timer function
 *
 * RETURNS       :
 *  None
 *
 **************************************************************************/
static void ti_igmp_timer_expired(unsigned long __data)
{
    struct timer_list       *timer;
    struct igmp_cache_entry *group_info;
    unsigned long           value = 1, counter = ti_igmp_mcb.number_of_groups;
    struct list_head        *ptr_temp = igmp_cache.next;

    if (ti_igmp_mcb.state != IGMP_START)
        return;

    while (counter-- > 0)
    {
		group_info = (struct igmp_cache_entry *) list_entry(ptr_temp,
				 struct igmp_cache_entry, links);

        ptr_temp = ptr_temp->next;

        if (group_info->M2ExpireTime)
        {
            if (--group_info->M2ExpireTime == 0)
            {
                igmp_handle_M2_timeout(group_info);
                continue;
            }
        }

        if (group_info->M1ExpireTime)
        {
            if (--group_info->M1ExpireTime == 0)
            {
                igmp_handle_M1_timeout(group_info);
                    continue;
            }
        }
    }

    /* Reinitialize IGMP timer. Expiration time 1 second */
    timer = &ti_igmp_mcb.igmp_timer;
    init_timer(timer);
    timer->data = (unsigned long) &value;
	timer->function = ti_igmp_timer_expired;
    timer->expires = jiffies + 1 * HZ;
    add_timer(timer);

    return;
}

/**************************************************************************
 * FUNCTION NAME : process_igmp_membership_report
 **************************************************************************
 * DESCRIPTION   :
 *  Process IGMP membership Report
 *
 * RETURNS       :
 *  None
 *
 * NOTES         :
 *  In the case of error; the packet memory is freed.
 **************************************************************************/
static void process_igmp_membership_report(struct igmphdr *igmp_hdr,
                struct sk_buff *skb, unsigned int device_type)
{
    struct igmp_cache_entry *group_info = NULL;
    int                     dev_index = 0;
    struct ethhdr           *eth_header;

    /* Get the Ethernet header */
    eth_header = (struct ethhdr *)skb->mac.raw;
    if (eth_header == NULL)
    {
        /* No Ethernet header. Drop the packet */
        printk ("Error: No Ethernet header. Dropping packet\n");
        kfree_skb(skb);
        return;
    }

#ifdef CONFIG_TI_L2_IGMP_DEBUG
    printk ("DEBUG: Processing IGMP MR for address (%4X)......\n", igmp_hdr->group);
#endif

    if (skb->input_dev != NULL)
        dev_index = skb->input_dev->ifindex;
    else if (skb->dev != NULL)
        dev_index = skb->dev->ifindex;
    else
    {
        printk ("DEBUG: IGMP MR for group address(%4X) doesnot have input_dev or dev set. Dropping packet\n",
                igmp_hdr->group);
        kfree_skb(skb);
        return;
    }

    group_info = get_igmp_cache_entry(igmp_hdr->group);

    if (device_type == DBR_CABLE_NET_DEV_TYPE)
    {
        /* Membership request received from RF */
        if (group_info != NULL)
        {
            if (group_info->state == JOIN_IN_PROCESS_STATE)
            {
                /* Cancel M1 timer */
                group_info->M1ExpireTime = 0;
                /* Not sending previous MR received from CPE to RF. Free the skb */
                kfree_skb(group_info->skb);
                group_info->skb = NULL;
            }
            /* Clear flag to notify that the last membership report was not sent by us. */
            group_info->MRselfSentFlag = FALSE;

            /* Forward the packet to LAN interface */
            ti_igmp_mcb.cfg.ti_igmp_send_packet(skb, group_info->input_dev,
                (unsigned long)(DBR_CMCI_NET_DEV_TYPE & DBR_ESAFE_NET_DEV_TYPE));

            /* Change state to Joined */
            group_info->state = JOINED_MEMBER_STATE;
        }
        else
        {
#ifdef CONFIG_TI_L2_IGMP_DEBUG
            printk ("DEBUG: IGMP MR received from RFI i/f for non-existent group address(%4X). Dropping packet\n",
                igmp_hdr->group);
#endif
            kfree_skb (skb);
        }
    }
    else
    {
        /* Membership request received from LAN */
        if( group_info == NULL)
        {
            /* New membership join */
            if ((group_info = create_igmp_cache_entry(igmp_hdr->group)) != NULL)
            {
                group_info->state = JOIN_IN_PROCESS_STATE;
                group_info->skb = skb;
#ifdef CONFIG_TI_L2_IGMP_DEBUG
                printk ("DEBUG: IGMP Cache Entry created (0x%p) Source MAC 0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\n",
                    group_info, eth_header->h_source[0], eth_header->h_source[1],
                    eth_header->h_source[2], eth_header->h_source[3],
                    eth_header->h_source[4], eth_header->h_source[5]);
#endif
                memcpy ((void *)&group_info->mac_address[0], (void *)&eth_header->h_source[0], ETH_ALEN);

                /* Start M1 and M2 timer */
                igmp_start_M1_timer(group_info);
                igmp_start_M2_timer(group_info, FALSE);

                /* Set the input device map to store incoming interface */
                if (dev_index > 0)
                    mark_device_in_group(group_info, dev_index - 1, TRUE);
                else
                    printk ("DEBUG: IGMP Member JOIN received for device index (%d) not found\n",
                       dev_index);

                /* Notify external entity a JOIN message was processed */
                ti_igmp_mcb.cfg.ti_igmp_message_processed(group_info->mac_address, JOIN_STATE);
            }
            else
            {
                printk ("Error: Adding group address (%4X) failed. Dropping packet\n", igmp_hdr->group);
                kfree_skb(skb);
            }
        }
        else
        {
            /* Existing member. Process only if Group address is not static */
            if (group_info->static_address == FALSE)
            {
                if (group_info->state == JOINED_MEMBER_STATE)
                {
                    /* Store the skb to send a report to RF after random delay */
                    group_info->state = JOIN_IN_PROCESS_STATE;
                    igmp_start_M1_timer(group_info);
                    if (group_info->skb == NULL)
                        group_info->skb = skb;
                    else
                        printk ("Why am I here??");

                }
                else
                {
#ifdef CONFIG_TI_L2_IGMP_DEBUG
                   printk ("DEBUG: IGMP MR received in Joining state from (%4X). Dropping packet\n",
                        igmp_hdr->group);
#endif
                    kfree_skb(skb);
                    return;
                }
                group_info->M2ExpireTime = 0;
                igmp_start_M2_timer(group_info, FALSE);
                if (dev_index > 0)
                    mark_device_in_group(group_info, dev_index - 1, TRUE);
                else
                    printk ("DEBUG: IGMP Member JOIN received for device index (%d) not found\n",
                        dev_index);
            }
            else
            {
                printk ("DEBUG: IGMP MR received for static MAC group address(%4X). Dropping packet\n",
                    igmp_hdr->group);
                kfree_skb (skb);
            }
        }
    }
}

/**************************************************************************
 * FUNCTION NAME : process_igmp_leave_group
 **************************************************************************
 * DESCRIPTION   :
 *  Process IGMP Leave message
 *
 * RETURNS       :
 *  None
 *
 * NOTES         :
 *  In the case of error; the packet memory is freed.
 **************************************************************************/
static void process_igmp_leave_group(struct igmphdr *igmp_hdr,
            struct sk_buff *skb, unsigned int device_type)
{
    struct igmp_cache_entry *group_info = NULL;
    int                     dev_index = 0;

    if (skb->input_dev != NULL)
        dev_index = skb->input_dev->ifindex;
    else if (skb->dev != NULL)
        dev_index = skb->dev->ifindex;
    else
    {
        printk ("DEBUG: IGMP MR for group address(%4X) doesnot have input_dev or dev set. Dropping packet\n",
                igmp_hdr->group);
        kfree_skb(skb);
        return;
    }
#ifdef CONFIG_TI_L2_IGMP_DEBUG
    printk ("DEBUG: Processing IGMP Leave for address (%4X)......\n", igmp_hdr->group);
#endif

    if ((group_info = get_igmp_cache_entry(igmp_hdr->group)) != NULL)
    {
        /* Send IGMP MR to Cable Interface */
        ti_igmp_mcb.cfg.ti_igmp_send_packet(group_info->skb, group_info->input_dev,
            DBR_CABLE_NET_DEV_TYPE);

        /* skb is sent for forwarding. Do not free */
        group_info->skb = NULL;

        /* Reset the input device map to store incoming interface */
        if (dev_index > 0)
            mark_device_in_group(group_info, dev_index - 1, FALSE);
        else
            printk ("DEBUG: IGMP Leave received for device index (%d) not found\n", dev_index);

        /* If last member in group - Delete timers, group information from cache */
        if (group_info->input_dev == 0)
            igmp_handle_M2_timeout(group_info);
    }
    else
    {
#ifdef CONFIG_TI_L2_IGMP_DEBUG
        printk ("DEBUG: IGMP Leave message received for non-existent group address(%4X). Dropping packet\n",
            igmp_hdr->group);
#endif
        kfree_skb (skb);
    }
}

/**************************************************************************
 * FUNCTION NAME : process_igmp_membership_query
 **************************************************************************
 * DESCRIPTION   :
 *  Process IGMP membership Query
 *
 * RETURNS       :
 *  None
 *
 * NOTES         :
 *  In the case of error; the packet memory is freed.
 **************************************************************************/
static void process_igmp_membership_query(struct igmphdr *igmp_hdr,
                struct sk_buff *skb, unsigned int device_type)
{
    struct igmp_cache_entry *group_info = NULL;
    unsigned long           query_delta, curr_time;

#ifdef CONFIG_TI_L2_IGMP_DEBUG
    printk ("DEBUG: Processing IGMP Query......\n");
#endif

    if (device_type == DBR_CABLE_NET_DEV_TYPE)
    {
        if (igmp_hdr->group == 0)
        {
            /* All host Membership Query received */
            /* Get time difference from previous query */

            curr_time = (jiffies_to_msecs(jiffies) / 1000);
            query_delta = (unsigned long)((long)curr_time - (long)ti_igmp_mcb.last_MQ_time);

            /* Update last MQ time to current time */
            ti_igmp_mcb.last_MQ_time = curr_time;

            ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryMaxResTime = igmp_hdr->code;
            ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryInterval = (unsigned long)
                max ((unsigned long)ti_igmp_mcb.cfg.igmp_mqi_timeout, (unsigned long) query_delta);

            /* IGMP v1 query received */
            if (igmp_hdr->code == 0)
                ti_igmp_mcb.igmp_stats[IGMP_HFC_INT_INDEX].wrongVerQueries++;
        }
        else
        {
            /* Group Specific Query received */
            if ((group_info = get_igmp_cache_entry(igmp_hdr->group)) != NULL)
            {
                group_info->M2ExpireTime = 0;
                igmp_start_M2_timer(group_info, TRUE);
            	ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].lastMemQueryIntrvl = igmp_hdr->code;
            	ti_igmp_mcb.igmp_stats[IGMP_CMCI_INT_INDEX].queryMaxResTime = igmp_hdr->code;
            }
            else
                printk ("DEBUG: IGMP MQ received for non-existing group. Dropping packet\n");
        }

        /* Forward the MQ packet to LAN interface */
        ti_igmp_mcb.cfg.ti_igmp_send_packet(skb, group_info->input_dev,
            (unsigned long)(DBR_CMCI_NET_DEV_TYPE & DBR_ESAFE_NET_DEV_TYPE));
    }
    else
    {
#ifdef CONFIG_TI_L2_IGMP_DEBUG
        printk ("DEBUG: IGMP MQ received from non-RF interface. Dropping packet\n");
#endif
        kfree_skb(skb);
    }
}

/**************************************************************************
 * FUNCTION NAME : ti_igmp_packet_handler
 **************************************************************************
 * DESCRIPTION   :
 *  TI IGMP Management Packet handler function
 *
 * RETURNS       :
 *  <0  -   Error
 *   0  -   Success
 *
 * NOTES         :
 *  The packet owned by IGMP. It will be storedfor future use or
 *  freed in case of error.
 **************************************************************************/
int ti_igmp_packet_handler(struct igmphdr *igmp_hdr, struct sk_buff *skb, unsigned int device_type)
{
    if (ti_igmp_mcb.state != IGMP_START)
    {
        kfree_skb(skb);
        return 0;
    }

#ifdef CONFIG_TI_L2_IGMP_DEBUG
    printk ("Debug: IGMP Type 0x%X - Group = %4X\n", igmp_hdr->type, igmp_hdr->group);
#endif

    switch (igmp_hdr->type)
	{
        case IGMP_HOST_MEMBERSHIP_REPORT:
		case IGMPV2_HOST_MEMBERSHIP_REPORT:
            process_igmp_membership_report(igmp_hdr, skb, device_type);

        break;
        case IGMP_HOST_LEAVE_MESSAGE:
            process_igmp_leave_group(igmp_hdr, skb, device_type);
        break;
		case IGMP_HOST_MEMBERSHIP_QUERY:
            process_igmp_membership_query(igmp_hdr, skb, device_type);
            break;
	    default:
		    break;
    }
    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_igmp_start
 **************************************************************************
 * DESCRIPTION   :
 *  TI Layer2 IGMP start function
 *
 * RETURNS       :
 *  <0  -   Error
 *   0  -   Success
 *
 **************************************************************************/
int ti_igmp_start (void)
{
	struct timer_list   *timer;
    unsigned long       value = 1;

    if (ti_igmp_mcb.state == IGMP_START)
        return 0;

    printk ("DEBUG: TI Layer 2 IGMP Starting\n");

    /* Initialize IGMP timer. Expiration time 1 second */
    timer = &ti_igmp_mcb.igmp_timer;
    init_timer(timer);
    timer->data = (unsigned long) &value;
	timer->function = ti_igmp_timer_expired;
    timer->expires = jiffies + 1 * HZ;
    add_timer(timer);
    ti_igmp_mcb.state = IGMP_START;

    return 0;
}

/**************************************************************************
 * FUNCTION NAME : ti_igmp_stop
 **************************************************************************
 * DESCRIPTION   :
 *  TI Layer2 IGMP stop function
 *
 * RETURNS       :
 *  <0  -   Error
 *   0  -   Success
 *
 **************************************************************************/
int ti_igmp_stop (void)
{
	struct timer_list   *timer;

    if (ti_igmp_mcb.state != IGMP_START)
        return 0;

    printk ("DEBUG: TI Layer 2 IGMP Stoping\n");

    ti_igmp_mcb.state = IGMP_STOP;
    timer = &ti_igmp_mcb.igmp_timer;
    del_timer (timer);

    /* Flush IGMP cache */
    flush_igmp_cache_entry();
    return 0;
}


/**************************************************************************
 * FUNCTION NAME : ti_igmp_init
 **************************************************************************
 * DESCRIPTION   :
 *  TI Layer2 IGMP init function
 *
 * RETURNS       :
 *  <0  -   Error
 *   0  -   Success
 *
 **************************************************************************/
int ti_igmp_init (struct igmp_cfg *cfg)
{
    printk ("DEBUG: TI Layer 2 IGMP Initializing\n");

    /* Initialize the Global IGMP Master Control Block */
    memset ((void *)&ti_igmp_mcb, 0, sizeof(ti_igmp_mcb));

    /* Initialize IGMP configuration */
    initialize_igmp_configuration(cfg);
    ti_igmp_mcb.state = IGMP_INIT;

    return 0;
}
EXPORT_SYMBOL(ti_igmp_init);
EXPORT_SYMBOL(ti_igmp_start);
EXPORT_SYMBOL(ti_igmp_stop);
EXPORT_SYMBOL(ti_igmp_packet_handler);
EXPORT_SYMBOL(ti_get_igmp_devices);
#endif /* CONFIG_TI_L2_IGMP */

