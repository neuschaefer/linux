/*
 * docsis_main.c
 *
 *  The file contains the DOCSIS Bridge Simulation Code.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/proc_fs.h>

#ifdef CONFIG_TI_L2_IGMP
#include <linux/ip.h>
#include <linux/in.h>
#include "ti_igmp.h"
#endif /* CONFIG_TI_L2_IGMP */

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/* Definition controls the printing of debug messages on the console. */
#undef DOCSIS_BRIDGE_DEBUG

/* Maximum number of entries in the ALT Table. */
#define MAX_ALT_SIZE    256

/* Maxmium number of interfaces attached to the DOCSIS Bridge. */
#define MAX_INTERFACES  4

/**************************************************************************
 *************************** Local Structures *****************************
 **************************************************************************/

/**************************************************************************
 * STRUCTURE NAME : DOCSIS_BRIDGE_DEVICE
 **************************************************************************
 * DESCRIPTION   :
 *  Structure defines the format of the network devices attached to the
 *  DOCSIS Bridge.
 **************************************************************************/
typedef struct DOCSIS_BRIDGE_DEVICE
{
    /* Linux specific device information. */
    struct net_device*  ptr_interface;

    /* The function is the registered function that is called to "push"
     * the packet onto the network interface. */
    int                 (*push)(struct sk_buff* skb);
}DOCSIS_BRIDGE_DEVICE;

/**************************************************************************
 * STRUCTURE NAME : DOCSIS_ALT_ENTRY
 **************************************************************************
 * DESCRIPTION   :
 *  Structure defines the format of the Address Lookup table entry.
 **************************************************************************/
typedef struct DOCSIS_ALT_ENTRY
{
    /* Indicates if the ALT entry is valid or not? */
    int                     status;

    /* MAC Address */
    unsigned char           mac_address[6];

    /* DOCSIS Bridge Device on which the MAC Address is present. */
    DOCSIS_BRIDGE_DEVICE*   ptr_docsis_device;
}DOCSIS_ALT_ENTRY;

/**************************************************************************
 * STRUCTURE NAME : DOCSIS_MCB
 **************************************************************************
 * DESCRIPTION   :
 *  Structure contains information about the DOCSIS bridge.
 **************************************************************************/
typedef struct DOCSIS_MCB
{
    /* List of all DOCSIS Bridge devices that is attached to the DOCSIS Bridge. */
    DOCSIS_BRIDGE_DEVICE   docsis_bridge_devices[MAX_INTERFACES];

    /* Address Lookup table; which contains all the MAC Address learnt */
    DOCSIS_ALT_ENTRY       alt_table[MAX_ALT_SIZE];
}DOCSIS_MCB;

/* Global Structure which keeps track of all the information for the DOCSIS Bridge. */
DOCSIS_MCB  docsis_mcb;

/**************************************************************************
 *************************** Extern Functions *****************************
 **************************************************************************/
extern struct net_device* __init ti_docsis_netdev_init(char* name);
extern struct net_device* __init ti_docsis_interconnect_init(void);
extern int ti_docsis_netif_rx(struct sk_buff *skb);
extern int ti_docsis_interconnect_receive(struct sk_buff *skb);

#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST
extern int ti_register_selective_packet_handler (char *br_name, int (*packet_handler)(struct sk_buff *skb), int priority);
extern int ti_unregister_selective_packet_handler (char *br_name, int (*packet_handler)(struct sk_buff *skb), int priority);
extern int ti_selective_forward(struct sk_buff *skb);
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST */

#ifdef CONFIG_TI_L2_IGMP
extern int ti_igmp_init (struct igmp_cfg *cfg);
extern int ti_igmp_start (void);
extern int ti_igmp_stop (void);
extern int ti_igmp_packet_handler(struct igmphdr *igmp_hdr, struct sk_buff *skb, unsigned int device_type);
extern unsigned long ti_get_igmp_devices(unsigned long ip_addr);
#endif /* CONFIG_TI_L2_IGMP */

#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST
/**************************************************************************
 * FUNCTION NAME : ti_selective_mark
 **************************************************************************
 * DESCRIPTION   :
 *  The function marks the bits corresponding to the device index on which the packet
 *  should be selectively forwarded
 *
 * RETURNS       :
 *  None
 *
 ***************************************************************************/
static void ti_selective_mark(struct sk_buff* skb, struct iphdr *iph)
{
    skb->ti_selective_fwd_dev_info = ti_get_igmp_devices(iph->daddr);

#ifdef DOCSIS_BRIDGE_DEBUG
        printk ("DEBUG DOCSIS Bridge: Fowarding Multicast packet from (%s) index (%d) mark (0x%llX)\n",
                    skb->dev->name, skb->dev->ifindex, skb->ti_selective_fwd_dev_info);
#endif /* DOCSIS_BRIDGE_DEBUG */

    return;
}

/**************************************************************************
 * FUNCTION NAME : docsis_read_info
 **************************************************************************
 * DESCRIPTION   :
 *  The function is the read handler for the DOCSIS Bridge proc entry. This
 *  will be called by the system, when the user does an "cat" to the proc
 *  entry.
 *
 * RETURNS       :
 *      > = 0 - Length of output string.
 *
 ***************************************************************************/
static int docsis_read_info(char* buf, char**start, off_t offset, int count, int *eof, void *data)
{
    int len = 0;

    len += sprintf (buf+len,"This is a WRITE ONLY PROC ENTRY.\n");

    /* Return the length. */
    return len;
}

/**************************************************************************
 * FUNCTION NAME : docsis_write_info
 **************************************************************************
 * DESCRIPTION   :
 *  The function is the write handler for the DOCSIS Bridge proc entry. This
 *  will be called by the system, when the user does an "echo" to the proc
 *  entry.
 *
 * RETURNS       :
 *      count - Success.
 *      < 0 -   Error.
 ***************************************************************************/
static int docsis_write_info
(
    struct file*    file,
    const char*     buffer,
    unsigned long   count,
    void*           data
)
{
	char    cmd[10];

    /* Initialize the buffer before using it. */
    memset ((void *)&cmd[0], 0, sizeof(cmd));

	/* Copy from user space. */
	if (copy_from_user (cmd, buffer, count))
		return -EFAULT;

	cmd[count-1] = '\0';

    if (strcmp(&cmd[0], "1") == 0)
    {
        if ((ti_register_selective_packet_handler ("dbr0", ti_selective_forward, 1)) == 0)
        {
            printk ("DEBUG: Handler to selectively forward multicast/broadcast packets installed\n");
            return count;
        }
        else
        {
            printk ("DEBUG: Error installing handler to selectively forward multicast/broadcast packets\n");
    		return -EFAULT;
        }
    }
    else if (strcmp(&cmd[0], "0") == 0)
    {
        if ((ti_unregister_selective_packet_handler ("dbr0", ti_selective_forward, 1)) == 0)
        {
            printk ("DEBUG: Handler to selectively forward multicast/broadcast packets uninstalled\n");
            return count;
        }
        else
        {
            printk ("DEBUG: Error uninstalling handler to selectively forward multicast/broadcast packets\n");
            return -EFAULT;
        }
    }
    else if (strcmp(&cmd[0], "2") == 0)
        display_igmp_cache_entry();
    else if (strcmp(&cmd[0], "3") == 0)
        ti_igmp_stop();
    else if (strcmp(&cmd[0], "4") == 0)
        ti_igmp_start();
    else
        printk ("Error - Unknown command\n");

    return count;
}
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST */

/**************************************************************************
 * FUNCTION NAME : ti_docsis_get_docsis_device
 **************************************************************************
 * DESCRIPTION   :
 *  The function retreives the DOCSIS network device for the corresponding
 *  network interface.
 *
 * RETURNS       :
 *  Pointer to the DOCSIS device -   Success
 *  NULL                         -   If no entry is found.
 **************************************************************************/
static DOCSIS_BRIDGE_DEVICE* ti_docsis_get_docsis_device (struct net_device* ptr_interface)
{
    int index;

    /* Search the DOCSIS Network device table. */
    for (index = 0; index <MAX_INTERFACES; index++)
    {
        /* Compare the network interface for a match. */
        if (ptr_interface == docsis_mcb.docsis_bridge_devices[index].ptr_interface)
            return &docsis_mcb.docsis_bridge_devices[index];
    }

    /* No match found. */
    return NULL;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_check_alt_table
 **************************************************************************
 * DESCRIPTION   :
 *  The function checks the ALT table for a matching entry.
 *
 * RETURNS       :
 *  >=0  -   Index of matched entry if one is found
 *  <0   -   If no entry is found.
 **************************************************************************/
static int ti_docsis_check_alt_table (unsigned char* mac_address)
{
    int index;

    /* Cycle through all the ALT Entries. */
    for (index = 0; index < MAX_ALT_SIZE; index++)
    {
        /* Is the ALT entry valid? */
        if (docsis_mcb.alt_table[index].status == 1)
        {
            /* YES. Compare the MAC Address and check for a duplicate */
            if (memcmp ((void *)mac_address, (void *)&docsis_mcb.alt_table[index].mac_address[0], 6) == 0)
                return index;
        }
    }
    /* No matching entry found... */
    return -1;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_add_alt_table
 **************************************************************************
 * DESCRIPTION   :
 *  Adds an entry to the ALT Table.
 *
 * RETURNS       :
 *  Index of the ALT Entry on success   -   Success
 *  <0                                  -   If no space is present
 **************************************************************************/
static int ti_docsis_add_alt_table (unsigned char* mac_address, DOCSIS_BRIDGE_DEVICE* ptr_docsis_device)
{
    int index;

    /* Check for duplicate MAC Address. */
    index = ti_docsis_check_alt_table (mac_address);
    if (index >= 0)
        return index;

    /* No Entry was found; so search through all the entries in the ALT Table. */
    for (index = 0; index < MAX_ALT_SIZE; index++)
    {
        /* Is the ALT entry free?*/
        if (docsis_mcb.alt_table[index].status == 0)
        {
            /* YES. Add the ALT Entry into the Table. */
            memcpy ((void *)&docsis_mcb.alt_table[index].mac_address[0], (void *)mac_address, 6);
            docsis_mcb.alt_table[index].ptr_docsis_device = ptr_docsis_device;
            docsis_mcb.alt_table[index].status = 1;
            return index;
        }
    }

    /* There was no space in the DOCSIS ALT Table. */
    return -1;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_flood_packet
 **************************************************************************
 * DESCRIPTION   :
 *  Floods the packet on all interfaces excluding the interface on which the
 *  packet was received.
 *
 * RETURNS       :
 *  0   -   Success
 *  <0  -   Error
 **************************************************************************/
static void ti_docsis_flood_packet (struct sk_buff* skb)
{
    struct sk_buff* skbclone;
    int             index = 0;

    /* Cycle through all the entries. */
    for (index = 0; index < MAX_INTERFACES; index++)
    {
        /* Basic Sanity Check: Dont proceed if there is valid pointer */
        if (docsis_mcb.docsis_bridge_devices[index].ptr_interface == NULL)
        {
            printk ("Error: Index %d no DOCSIS Bridge Device defined\n", index);
            continue;
        }

        /* Do we send the packet on this interface? */
        if (skb->dev == docsis_mcb.docsis_bridge_devices[index].ptr_interface)
            continue;

        /* Clone the packet. */
        skbclone = skb_clone(skb, GFP_ATOMIC);
        if (skbclone == NULL)
        {
            /* Memory Problems: Clean the original SKB before leaving. */
            printk ("Error: Cloning Failed for Device %s\n", docsis_mcb.docsis_bridge_devices[index].ptr_interface->name);
            kfree_skb(skb);
            return;
        }

        /* Initialize the packet structure to indicate the device on which the packet will be transmitted */
        skbclone->dev = docsis_mcb.docsis_bridge_devices[index].ptr_interface;

        /* Push the packet via the DOCSIS Device */
        docsis_mcb.docsis_bridge_devices[index].push(skbclone);

        /* Debug Message. */
#ifdef DOCSIS_BRIDGE_DEBUG
        printk ("Flooding: Packet 0x%p --> %s\n", skbclone, docsis_mcb.docsis_bridge_devices[index].ptr_interface->name);
#endif /* DOCSIS_BRIDGE_DEBUG */
    }

    /* Cleanup the original packet. */
    kfree_skb (skb);
    return;
}

/**************************************************************************
 * FUNCTION NAME : ti_docsis_receive
 **************************************************************************
 * DESCRIPTION   :
 *  The function is the entry point by which packets are pushed into the
 *  DOCSIS Bridge.
 *
 * NOTES         :
 *  Before calling this function ensure that the MAC RAW Pointer in the
 *  SKB is valid and points to the start of the MAC header.
 **************************************************************************/
void ti_docsis_receive (struct sk_buff *skb)
{
    struct ethhdr*          ptr_ethhdr;
    int                     index;
    DOCSIS_BRIDGE_DEVICE*   ptr_docsis_dev;

    /* Get the Ethernet header. */
    ptr_ethhdr = (struct ethhdr *)skb->mac.raw;
    if (ptr_ethhdr == NULL)
    {
        /* No Ethernet header; drop the packet and clean the memory. */
        printk ("No Ethernet header; dropping packet\n");
        kfree_skb (skb);
        return;
    }

    /* Get the DOCSIS Device information. */
    ptr_docsis_dev = ti_docsis_get_docsis_device (skb->dev);
    if (ptr_docsis_dev == NULL)
    {
        /* Packet was passed from a device not attached to the DOCSIS Bridge. */
        printk ("No DOCSIS Device; packet from %s\n", skb->dev->name);
        kfree_skb (skb);
        return;
    }

    /* Debug Message */
#ifdef DOCSIS_BRIDGE_DEBUG
    printk ("DEBUG: Packet 0x%p from %s 0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\n",
            skb, ptr_docsis_dev->ptr_interface->name,
            ptr_ethhdr->h_source[0], ptr_ethhdr->h_source[1], ptr_ethhdr->h_source[2],
            ptr_ethhdr->h_source[3], ptr_ethhdr->h_source[4], ptr_ethhdr->h_source[5]);
#endif /* DOCSIS_BRIDGE_DEBUG*/

    /* Add the Source MAC address of the packet to the ALT Table. */
    ti_docsis_add_alt_table (ptr_ethhdr->h_source, ptr_docsis_dev);

    /* Check if the packet is a Broadcast/Multicast packet. */
    if (ptr_ethhdr->h_dest[0] & 0x1)
    {
#ifdef CONFIG_TI_L2_IGMP
        /* Multicast packet. Selectively forward */
        if (!ptr_ethhdr->h_dest[1] && (ptr_ethhdr->h_dest[2] & 0x5e))
        {
            struct iphdr    *iph = (struct iphdr *) (((char *)skb->mac.raw) + ETH_HLEN);
            unsigned long   device_type;
            if (iph != NULL)
            {
                if (iph->protocol == IPPROTO_IGMP)
                {
                    if (strcmp(skb->input_dev->name, "eth1") == 0)
                        device_type = 2;
                    else device_type = 1;
                    ti_igmp_packet_handler ((struct igmphdr *)(((char *)iph) + (iph->ihl << 2)),
                        skb, device_type);
                    return;
                }
                else
#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST
                    ti_selective_mark(skb, iph);
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST */
            }
        }
#endif /* CONFIG_TI_L2_IGMP */

        /* Flood the packet on all interfaces. */
        ti_docsis_flood_packet (skb);
        return;
    }

    /* Unicast Packets. Check the ALT Table for a match on the destination MAC Address */
    index = ti_docsis_check_alt_table (ptr_ethhdr->h_dest);
    if (index < 0)
    {
        /* No Match found; flood the packet. */
        ti_docsis_flood_packet (skb);
        return;
    }
    else
    {
        /* Match Found: Send the packet only on that interface */
        skb->dev = docsis_mcb.alt_table[index].ptr_docsis_device->ptr_interface;
        docsis_mcb.alt_table[index].ptr_docsis_device->push(skb);
#ifdef DOCSIS_BRIDGE_DEBUG
        printk ("Packet 0x%p --> %s\n", skb, skb->dev->name);
#endif /* DOCSIS_BRIDGE_DEBUG */
    }

    /* Work has been completed. */
    return;
}

#ifdef CONFIG_TI_L2_IGMP
int ti_docsis_igmp_send_packet(struct sk_buff *skb, unsigned long device_map, unsigned long device_type)
{
    printk ("IGMP send function called for device map (%lu) device type (%lu)\n",
        device_map, device_type);
    return 0;
}

int ti_docsis_igmp_message_processed(char *mac_address, int msg_type)
{
    printk ("IGMP message (%s) processsed for Source MAC 0x%02x-0x%02x-0x%02x-0x%02x-0x%02x-0x%02x\n",
            msg_type ? "JOIN":"LEAVE", mac_address[0], mac_address[1], mac_address[2], mac_address[3],
            mac_address[4], mac_address[5]);
    return 0;
}
#endif /* CONFIG_TI_L2_IGMP */

/**************************************************************************
 * FUNCTION NAME : ti_docsis_init
 **************************************************************************
 * DESCRIPTION   :
 *  Initialization code for the DOCSIS Bridge
 **************************************************************************/
void ti_docsis_init (void)
{
    printk ("DEBUG: DOCSIS Bridge Simulator Initializing\n");

    /* Initialize the Global MCB for the DOCSIS. */
    memset ((void *)&docsis_mcb, 0, sizeof(DOCSIS_MCB));

    /************* Register the DOCSIS Interface ***************************/
    docsis_mcb.docsis_bridge_devices[0].ptr_interface = dev_get_by_name ("eth0");
    if (docsis_mcb.docsis_bridge_devices[0].ptr_interface == NULL)
    {
        printk ("Error: Unable to detect the DOCSIS Upstream Interface\n");
        return;
    }

    /* The PUSH Function in this case will send the packet to the DOCSIS Interface. */
    docsis_mcb.docsis_bridge_devices[0].push = dev_queue_xmit;

    /************* Register the DOCSIS Local Interconnect *******************/
    docsis_mcb.docsis_bridge_devices[1].ptr_interface = ti_docsis_interconnect_init();
    if (docsis_mcb.docsis_bridge_devices[1].ptr_interface == NULL)
        return;

    /* The PUSH Function in this case will send the packet via the "Interconnect" device
     * back to the LOCAL Bridge. */
    docsis_mcb.docsis_bridge_devices[1].push = ti_docsis_interconnect_receive;

    /************* Register the DOCSIS NET IP Connectivity *******************/
    docsis_mcb.docsis_bridge_devices[2].ptr_interface = ti_docsis_netdev_init("dbr1");
    if (docsis_mcb.docsis_bridge_devices[2].ptr_interface == NULL)
        return;

    /* The PUSH Function in this case will send the packet via the NET device to the
     * IP Stack */
    docsis_mcb.docsis_bridge_devices[2].push = ti_docsis_netif_rx;

    docsis_mcb.docsis_bridge_devices[3].ptr_interface = ti_docsis_netdev_init("dbr2");
    if (docsis_mcb.docsis_bridge_devices[3].ptr_interface == NULL)
        return;

    /* The PUSH Function in this case will send the packet via the NET device to the
     * IP Stack */
    docsis_mcb.docsis_bridge_devices[3].push = ti_docsis_netif_rx;

#ifdef CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST
{
    struct proc_dir_entry*  ptr_dir_entry;

    /* Create the proc entry for configuration. */
    ptr_dir_entry = create_proc_entry("avalanche/docsis_bridge", 0644, NULL);
    if (ptr_dir_entry == NULL)
    {
        printk ("ERROR: Unable to create the proc entry for docsis bridge\n");
        return;
    }
    ptr_dir_entry->data         = NULL;
    ptr_dir_entry->read_proc    = docsis_read_info;
    ptr_dir_entry->write_proc   = docsis_write_info;
    ptr_dir_entry->owner        = NULL;
}
#endif /* CONFIG_TI_L2_SELECTIVE_FORWARDER_TEST */

#ifdef CONFIG_TI_L2_IGMP
{
    struct igmp_cfg cfg;

    cfg.enabled = 1;
    cfg.igmp_mqi_timeout = 125;
    cfg.igmp_qri_timeout = 100;
    cfg.igmp_m1_max_timeout = 3;
    cfg.igmp_docsis_robustness = 2;
    cfg.ti_igmp_send_packet = ti_docsis_igmp_send_packet;
    cfg.ti_igmp_message_processed = ti_docsis_igmp_message_processed;
    ti_igmp_init(&cfg);
    ti_igmp_start();
}
#endif /* CONFIG_TI_L2_IGMP */

    printk ("DEBUG: DOCSIS Bridge Simulator Initialized succesfully\n");
    return;
}

