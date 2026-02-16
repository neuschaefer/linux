/*
 * RNDIS MSG parser
 *
 * Version:     $Id: rndis.c,v 1.19 2004/03/25 21:33:46 robert Exp $
 *
 * Authors:	Benedikt Spranger, Pengutronix
 *		Robert Schwebel, Pengutronix
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              version 2, as published by the Free Software Foundation.
 *
 *		This software was originally developed in conformance with
 *		Microsoft's Remote NDIS Specification License Agreement.
 *
 * 03/12/2004 Kai-Uwe Bloem <linux-development@auerswald.de>
 *		Fixed message length bug in init_response
 *
 * 03/25/2004 Kai-Uwe Bloem <linux-development@auerswald.de>
 *		Fixed rndis_rm_hdr length bug.
 *
 * Copyright (C) 2004 by David Brownell
 *		updates to merge with Linux 2.6, better match RNDIS spec
 */
/*
Includes Intel Corporation's changes/modifications dated: 2014.
Changed/modified portions - Copyright © 2014, Intel Corporation.
*/

/* Copyright 2008, Texas Instruments Incorporated
 *
 * This program has been modified from its original operation by 
 * Texas Instruments to do the following:
 * 
 * Explanation of modification:
 *  fixes/changes from Puma5
 *  
 *
 * THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
 * "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS
 * OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 * PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR
 * DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS,
 * COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
 * See The GNU General Public License for more details.
 *
 * These changes are covered under version 2 of the GNU General Public License,
 * dated June 1991.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/seq_file.h>
#include <linux/netdevice.h>

#include <asm/io.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>


#undef	VERBOSE_DEBUG

#include "rndis.h"


/* The driver for your USB chip needs to support ep0 OUT to work with
 * RNDIS, plus all three CDC Ethernet endpoints (interrupt not optional).
 *
 * Windows hosts need an INF file like Documentation/usb/linux.inf
 * and will be happier if you provide the host_addr module parameter.
 */

#if 0
static int rndis_debug = 0;
module_param (rndis_debug, int, 0);
MODULE_PARM_DESC (rndis_debug, "enable debugging");
#else
#define rndis_debug		0
#endif

#define RNDIS_MAX_CONFIGS	1


static rndis_params rndis_per_dev_params[RNDIS_MAX_CONFIGS];

/* Driver Version */
static const __le32 rndis_driver_version = cpu_to_le32(1);

/* Function Prototypes */
static rndis_resp_t *rndis_add_response(int configNr, u32 length);


/* supported OIDs */
static const u32 oid_supported_list[] =
{
	/* the general stuff */
	RNDIS_OID_GEN_SUPPORTED_LIST,
	RNDIS_OID_GEN_HARDWARE_STATUS,
	RNDIS_OID_GEN_MEDIA_SUPPORTED,
	RNDIS_OID_GEN_MEDIA_IN_USE,
	RNDIS_OID_GEN_MAXIMUM_FRAME_SIZE,
	RNDIS_OID_GEN_LINK_SPEED,
	RNDIS_OID_GEN_TRANSMIT_BLOCK_SIZE,
	RNDIS_OID_GEN_RECEIVE_BLOCK_SIZE,
	RNDIS_OID_GEN_VENDOR_ID,
	RNDIS_OID_GEN_VENDOR_DESCRIPTION,
	RNDIS_OID_GEN_VENDOR_DRIVER_VERSION,
	RNDIS_OID_GEN_CURRENT_PACKET_FILTER,
	RNDIS_OID_GEN_MAXIMUM_TOTAL_SIZE,
	RNDIS_OID_GEN_MEDIA_CONNECT_STATUS,
	RNDIS_OID_GEN_PHYSICAL_MEDIUM,
#ifdef CONFIG_MACH_PUMA5
	RNDIS_OID_GEN_RNDIS_CONFIG_PARAMETER,
#endif

	/* the statistical stuff */
	RNDIS_OID_GEN_XMIT_OK,
	RNDIS_OID_GEN_RCV_OK,
	RNDIS_OID_GEN_XMIT_ERROR,
	RNDIS_OID_GEN_RCV_ERROR,
	RNDIS_OID_GEN_RCV_NO_BUFFER,
#ifdef	RNDIS_OPTIONAL_STATS
	RNDIS_OID_GEN_DIRECTED_BYTES_XMIT,
	RNDIS_OID_GEN_DIRECTED_FRAMES_XMIT,
	RNDIS_OID_GEN_MULTICAST_BYTES_XMIT,
	RNDIS_OID_GEN_MULTICAST_FRAMES_XMIT,
	RNDIS_OID_GEN_BROADCAST_BYTES_XMIT,
	RNDIS_OID_GEN_BROADCAST_FRAMES_XMIT,
	RNDIS_OID_GEN_DIRECTED_BYTES_RCV,
	RNDIS_OID_GEN_DIRECTED_FRAMES_RCV,
	RNDIS_OID_GEN_MULTICAST_BYTES_RCV,
	RNDIS_OID_GEN_MULTICAST_FRAMES_RCV,
	RNDIS_OID_GEN_BROADCAST_BYTES_RCV,
	RNDIS_OID_GEN_BROADCAST_FRAMES_RCV,
	RNDIS_OID_GEN_RCV_CRC_ERROR,
	RNDIS_OID_GEN_TRANSMIT_QUEUE_LENGTH,
#endif	/* RNDIS_OPTIONAL_STATS */

	/* mandatory 802.3 */
	/* the general stuff */
	RNDIS_OID_802_3_PERMANENT_ADDRESS,
	RNDIS_OID_802_3_CURRENT_ADDRESS,
	RNDIS_OID_802_3_MULTICAST_LIST,
	RNDIS_OID_802_3_MAC_OPTIONS,
	RNDIS_OID_802_3_MAXIMUM_LIST_SIZE,

	/* the statistical stuff */
	RNDIS_OID_802_3_RCV_ERROR_ALIGNMENT,
	RNDIS_OID_802_3_XMIT_ONE_COLLISION,
	RNDIS_OID_802_3_XMIT_MORE_COLLISIONS,
#ifdef	RNDIS_OPTIONAL_STATS
	RNDIS_OID_802_3_XMIT_DEFERRED,
	RNDIS_OID_802_3_XMIT_MAX_COLLISIONS,
	RNDIS_OID_802_3_RCV_OVERRUN,
	RNDIS_OID_802_3_XMIT_UNDERRUN,
	RNDIS_OID_802_3_XMIT_HEARTBEAT_FAILURE,
	RNDIS_OID_802_3_XMIT_TIMES_CRS_LOST,
	RNDIS_OID_802_3_XMIT_LATE_COLLISIONS,
#endif	/* RNDIS_OPTIONAL_STATS */

#ifdef	RNDIS_PM
	/* PM and wakeup are "mandatory" for USB, but the RNDIS specs
	 * don't say what they mean ... and the NDIS specs are often
	 * confusing and/or ambiguous in this context.  (That is, more
	 * so than their specs for the other OIDs.)
	 *
	 * FIXME someone who knows what these should do, please
	 * implement them!
	 */

	/* power management */
	OID_PNP_CAPABILITIES,
	OID_PNP_QUERY_POWER,
	OID_PNP_SET_POWER,

#ifdef	RNDIS_WAKEUP
	/* wake up host */
	OID_PNP_ENABLE_WAKE_UP,
	OID_PNP_ADD_WAKE_UP_PATTERN,
	OID_PNP_REMOVE_WAKE_UP_PATTERN,
#endif	/* RNDIS_WAKEUP */
#endif	/* RNDIS_PM */
};

#ifdef CONFIG_MACH_PUMA5
static void string_to_unicode (char *string, u16 *unicode_string, u16 *length);
static u8 unicode_to_macnibble (u16 value);
static int rndis_indicate_status_msg (int configNr, u32 status);
#endif

/* NDIS Functions */
static int gen_ndis_query_resp(int configNr, u32 OID, u8 *buf,
			       unsigned buf_len, rndis_resp_t *r)
{
	int retval = -ENOTSUPP;
	u32 length = 4;	/* usually */
	__le32 *outbuf;
	int i, count;
	rndis_query_cmplt_type *resp;
	struct net_device *net;
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats;

	if (!r) return -ENOMEM;
	resp = (rndis_query_cmplt_type *)r->buf;

	if (!resp) return -ENOMEM;

	if (buf_len && rndis_debug > 1) {
		pr_debug("query OID %08x value, len %d:\n", OID, buf_len);
		for (i = 0; i < buf_len; i += 16) {
			pr_debug("%03d: %08x %08x %08x %08x\n", i,
				get_unaligned_le32(&buf[i]),
				get_unaligned_le32(&buf[i + 4]),
				get_unaligned_le32(&buf[i + 8]),
				get_unaligned_le32(&buf[i + 12]));
		}
	}

	/* response goes here, right after the header */
	outbuf = (__le32 *)&resp[1];
	resp->InformationBufferOffset = cpu_to_le32(16);

	net = rndis_per_dev_params[configNr].dev;
	stats = dev_get_stats(net, &temp);

	switch (OID) {

	/* general oids (table 4-1) */

	/* mandatory */
	case RNDIS_OID_GEN_SUPPORTED_LIST:
		pr_debug("%s: RNDIS_OID_GEN_SUPPORTED_LIST\n", __func__);
		length = sizeof(oid_supported_list);
		count  = length / sizeof(u32);
		for (i = 0; i < count; i++)
			outbuf[i] = cpu_to_le32(oid_supported_list[i]);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_HARDWARE_STATUS:
		pr_debug("%s: RNDIS_OID_GEN_HARDWARE_STATUS\n", __func__);
		/* Bogus question!
		 * Hardware must be ready to receive high level protocols.
		 * BTW:
		 * reddite ergo quae sunt Caesaris Caesari
		 * et quae sunt Dei Deo!
		 */
		*outbuf = cpu_to_le32(0);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_MEDIA_SUPPORTED:
		pr_debug("%s: RNDIS_OID_GEN_MEDIA_SUPPORTED\n", __func__);
		*outbuf = cpu_to_le32(rndis_per_dev_params[configNr].medium);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_MEDIA_IN_USE:
		pr_debug("%s: RNDIS_OID_GEN_MEDIA_IN_USE\n", __func__);
		/* one medium, one transport... (maybe you do it better) */
		*outbuf = cpu_to_le32(rndis_per_dev_params[configNr].medium);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_MAXIMUM_FRAME_SIZE:
		pr_debug("%s: RNDIS_OID_GEN_MAXIMUM_FRAME_SIZE\n", __func__);
		if (rndis_per_dev_params[configNr].dev) {
			*outbuf = cpu_to_le32(
				rndis_per_dev_params[configNr].dev->mtu);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_LINK_SPEED:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_LINK_SPEED\n", __func__);
		if (rndis_per_dev_params[configNr].media_state
				== RNDIS_MEDIA_STATE_DISCONNECTED)
			*outbuf = cpu_to_le32(0);
		else
			*outbuf = cpu_to_le32(
				rndis_per_dev_params[configNr].speed);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_TRANSMIT_BLOCK_SIZE:
		pr_debug("%s: RNDIS_OID_GEN_TRANSMIT_BLOCK_SIZE\n", __func__);
		if (rndis_per_dev_params[configNr].dev) {
			*outbuf = cpu_to_le32(
				rndis_per_dev_params[configNr].dev->mtu);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_RECEIVE_BLOCK_SIZE:
		pr_debug("%s: RNDIS_OID_GEN_RECEIVE_BLOCK_SIZE\n", __func__);
		if (rndis_per_dev_params[configNr].dev) {
			*outbuf = cpu_to_le32(
				rndis_per_dev_params[configNr].dev->mtu);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_VENDOR_ID:
		pr_debug("%s: RNDIS_OID_GEN_VENDOR_ID\n", __func__);
		*outbuf = cpu_to_le32(
			rndis_per_dev_params[configNr].vendorID);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_VENDOR_DESCRIPTION:
		pr_debug("%s: RNDIS_OID_GEN_VENDOR_DESCRIPTION\n", __func__);
		if (rndis_per_dev_params[configNr].vendorDescr) {
			length = strlen(rndis_per_dev_params[configNr].
					vendorDescr);
			memcpy(outbuf,
				rndis_per_dev_params[configNr].vendorDescr,
				length);
		} else {
			outbuf[0] = 0;
		}
		retval = 0;
		break;

	case RNDIS_OID_GEN_VENDOR_DRIVER_VERSION:
		pr_debug("%s: RNDIS_OID_GEN_VENDOR_DRIVER_VERSION\n", __func__);
		/* Created as LE */
		*outbuf = rndis_driver_version;
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_CURRENT_PACKET_FILTER:
		pr_debug("%s: RNDIS_OID_GEN_CURRENT_PACKET_FILTER\n", __func__);
#ifdef CONFIG_MACH_PUMA5
		*outbuf = cpu_to_le32 (*(u16 *)rndis_per_dev_params[configNr].filter);
#else
		*outbuf = cpu_to_le32(*rndis_per_dev_params[configNr].filter);
#endif
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_MAXIMUM_TOTAL_SIZE:
		pr_debug("%s: RNDIS_OID_GEN_MAXIMUM_TOTAL_SIZE\n", __func__);
		*outbuf = cpu_to_le32(RNDIS_MAX_TOTAL_SIZE);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_GEN_MEDIA_CONNECT_STATUS:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_MEDIA_CONNECT_STATUS\n", __func__);
		*outbuf = cpu_to_le32(rndis_per_dev_params[configNr]
						.media_state);
		retval = 0;
		break;

	case RNDIS_OID_GEN_PHYSICAL_MEDIUM:
		pr_debug("%s: RNDIS_OID_GEN_PHYSICAL_MEDIUM\n", __func__);
#ifdef CONFIG_MACH_PUMA5
		*outbuf = cpu_to_le32 (2);
#else	
	*outbuf = cpu_to_le32(0);
#endif
		retval = 0;
		break;

	/* The RNDIS specification is incomplete/wrong.   Some versions
	 * of MS-Windows expect OIDs that aren't specified there.  Other
	 * versions emit undefined RNDIS messages. DOCUMENT ALL THESE!
	 */
	case RNDIS_OID_GEN_MAC_OPTIONS:		/* from WinME */
		pr_debug("%s: RNDIS_OID_GEN_MAC_OPTIONS\n", __func__);
		*outbuf = cpu_to_le32(
			  RNDIS_MAC_OPTION_RECEIVE_SERIALIZED
			| RNDIS_MAC_OPTION_FULL_DUPLEX);
		retval = 0;
		break;

	/* statistics OIDs (table 4-2) */

	/* mandatory */
	case RNDIS_OID_GEN_XMIT_OK:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_XMIT_OK\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->tx_packets
				- stats->tx_errors - stats->tx_dropped);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_RCV_OK:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_RCV_OK\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->rx_packets
				- stats->rx_errors - stats->rx_dropped);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_XMIT_ERROR:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_XMIT_ERROR\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->tx_errors);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_RCV_ERROR:
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_OID_GEN_RCV_ERROR\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->rx_errors);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_GEN_RCV_NO_BUFFER:
		pr_debug("%s: RNDIS_OID_GEN_RCV_NO_BUFFER\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->rx_dropped);
			retval = 0;
		}
		break;

#ifdef	RNDIS_OPTIONAL_STATS
	case OID_GEN_DIRECTED_BYTES_XMIT:
		pr_debug("%s: OID_GEN_DIRECTED_BYTES_XMIT\n", __func__);
		/*
		 * Aunt Tilly's size of shoes
		 * minus antarctica count of penguins
		 * divided by weight of Alpha Centauri
		 */
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (
				(rndis_per_dev_params [configNr]
					.stats->tx_packets -
				 rndis_per_dev_params [configNr]
					 .stats->tx_errors -
				 rndis_per_dev_params [configNr]
					 .stats->tx_dropped)
				* 123);
			retval = 0;
		}
		break;

	case OID_GEN_DIRECTED_FRAMES_XMIT:
		pr_debug("%s: OID_GEN_DIRECTED_FRAMES_XMIT\n", __func__);
		/* dito */
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (
				(rndis_per_dev_params [configNr]
					.stats->tx_packets -
				 rndis_per_dev_params [configNr]
					 .stats->tx_errors -
				 rndis_per_dev_params [configNr]
					 .stats->tx_dropped)
				/ 123);
			retval = 0;
		}
		break;

	case OID_GEN_MULTICAST_BYTES_XMIT:
		pr_debug("%s: OID_GEN_MULTICAST_BYTES_XMIT\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->multicast*1234);
			retval = 0;
		}
		break;

	case OID_GEN_MULTICAST_FRAMES_XMIT:
		pr_debug("%s: OID_GEN_MULTICAST_FRAMES_XMIT\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->multicast);
			retval = 0;
		}
		break;

	case OID_GEN_BROADCAST_BYTES_XMIT:
		pr_debug("%s: OID_GEN_BROADCAST_BYTES_XMIT\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->tx_packets/42*255);
			retval = 0;
		}
		break;

	case OID_GEN_BROADCAST_FRAMES_XMIT:
		pr_debug("%s: OID_GEN_BROADCAST_FRAMES_XMIT\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->tx_packets/42);
			retval = 0;
		}
		break;

	case OID_GEN_DIRECTED_BYTES_RCV:
		pr_debug("%s: OID_GEN_DIRECTED_BYTES_RCV\n", __func__);
		*outbuf = __constant_cpu_to_le32 (0);
		retval = 0;
		break;

	case OID_GEN_DIRECTED_FRAMES_RCV:
		pr_debug("%s: OID_GEN_DIRECTED_FRAMES_RCV\n", __func__);
		*outbuf = __constant_cpu_to_le32 (0);
		retval = 0;
		break;

	case OID_GEN_MULTICAST_BYTES_RCV:
		pr_debug("%s: OID_GEN_MULTICAST_BYTES_RCV\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->multicast * 1111);
			retval = 0;
		}
		break;

	case OID_GEN_MULTICAST_FRAMES_RCV:
		pr_debug("%s: OID_GEN_MULTICAST_FRAMES_RCV\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->multicast);
			retval = 0;
		}
		break;

	case OID_GEN_BROADCAST_BYTES_RCV:
		pr_debug("%s: OID_GEN_BROADCAST_BYTES_RCV\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->rx_packets/42*255);
			retval = 0;
		}
		break;

	case OID_GEN_BROADCAST_FRAMES_RCV:
		pr_debug("%s: OID_GEN_BROADCAST_FRAMES_RCV\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->rx_packets/42);
			retval = 0;
		}
		break;

	case OID_GEN_RCV_CRC_ERROR:
		pr_debug("%s: OID_GEN_RCV_CRC_ERROR\n", __func__);
		if (rndis_per_dev_params [configNr].stats) {
			*outbuf = cpu_to_le32 (rndis_per_dev_params [configNr]
					.stats->rx_crc_errors);
			retval = 0;
		}
		break;

	case OID_GEN_TRANSMIT_QUEUE_LENGTH:
		pr_debug("%s: OID_GEN_TRANSMIT_QUEUE_LENGTH\n", __func__);
		*outbuf = __constant_cpu_to_le32 (0);
		retval = 0;
		break;
#endif	/* RNDIS_OPTIONAL_STATS */

	/* ieee802.3 OIDs (table 4-3) */

	/* mandatory */
	case RNDIS_OID_802_3_PERMANENT_ADDRESS:
		pr_debug("%s: RNDIS_OID_802_3_PERMANENT_ADDRESS\n", __func__);
		if (rndis_per_dev_params[configNr].dev) {
			length = ETH_ALEN;
#ifdef CONFIG_MACH_PUMA5
			 memcpy (outbuf,
                                rndis_per_dev_params [configNr].perm_mac,
                                length);
#else
			memcpy(outbuf,
				rndis_per_dev_params[configNr].host_mac,
				length);
#endif
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_802_3_CURRENT_ADDRESS:
		pr_debug("%s: RNDIS_OID_802_3_CURRENT_ADDRESS\n", __func__);
		if (rndis_per_dev_params[configNr].dev) {
			length = ETH_ALEN;
			memcpy(outbuf,
				rndis_per_dev_params [configNr].host_mac,
				length);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_802_3_MULTICAST_LIST:
		pr_debug("%s: RNDIS_OID_802_3_MULTICAST_LIST\n", __func__);
		/* Multicast base address only */
		*outbuf = cpu_to_le32(0xE0000000);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_802_3_MAXIMUM_LIST_SIZE:
		pr_debug("%s: RNDIS_OID_802_3_MAXIMUM_LIST_SIZE\n", __func__);
		/* Multicast base address only */
#ifdef CONFIG_MACH_PUMA5
		*outbuf = cpu_to_le32(32); /* TAG0008 */
#else
		*outbuf = cpu_to_le32(1);
#endif
		retval = 0;
		break;

	case RNDIS_OID_802_3_MAC_OPTIONS:
		pr_debug("%s: RNDIS_OID_802_3_MAC_OPTIONS\n", __func__);
		*outbuf = cpu_to_le32(0);
		retval = 0;
		break;

	/* ieee802.3 statistics OIDs (table 4-4) */

	/* mandatory */
	case RNDIS_OID_802_3_RCV_ERROR_ALIGNMENT:
		pr_debug("%s: RNDIS_OID_802_3_RCV_ERROR_ALIGNMENT\n", __func__);
		if (stats) {
			*outbuf = cpu_to_le32(stats->rx_frame_errors);
			retval = 0;
		}
		break;

	/* mandatory */
	case RNDIS_OID_802_3_XMIT_ONE_COLLISION:
		pr_debug("%s: RNDIS_OID_802_3_XMIT_ONE_COLLISION\n", __func__);
		*outbuf = cpu_to_le32(0);
		retval = 0;
		break;

	/* mandatory */
	case RNDIS_OID_802_3_XMIT_MORE_COLLISIONS:
		pr_debug("%s: RNDIS_OID_802_3_XMIT_MORE_COLLISIONS\n", __func__);
		*outbuf = cpu_to_le32(0);
		retval = 0;
		break;

#ifdef	RNDIS_OPTIONAL_STATS
	case OID_802_3_XMIT_DEFERRED:
		pr_debug("%s: OID_802_3_XMIT_DEFERRED\n", __func__);
		/* TODO */
		break;

	case OID_802_3_XMIT_MAX_COLLISIONS:
		pr_debug("%s: OID_802_3_XMIT_MAX_COLLISIONS\n", __func__);
		/* TODO */
		break;

	case OID_802_3_RCV_OVERRUN:
		pr_debug("%s: OID_802_3_RCV_OVERRUN\n", __func__);
		/* TODO */
		break;

	case OID_802_3_XMIT_UNDERRUN:
		pr_debug("%s: OID_802_3_XMIT_UNDERRUN\n", __func__);
		/* TODO */
		break;

	case OID_802_3_XMIT_HEARTBEAT_FAILURE:
		pr_debug("%s: OID_802_3_XMIT_HEARTBEAT_FAILURE\n", __func__);
		/* TODO */
		break;

	case OID_802_3_XMIT_TIMES_CRS_LOST:
		pr_debug("%s: OID_802_3_XMIT_TIMES_CRS_LOST\n", __func__);
		/* TODO */
		break;

	case OID_802_3_XMIT_LATE_COLLISIONS:
		pr_debug("%s: OID_802_3_XMIT_LATE_COLLISIONS\n", __func__);
		/* TODO */
		break;
#endif	/* RNDIS_OPTIONAL_STATS */

#ifdef	RNDIS_PM
	/* power management OIDs (table 4-5) */
	case OID_PNP_CAPABILITIES:
		pr_debug("%s: OID_PNP_CAPABILITIES\n", __func__);

		/* for now, no wakeup capabilities */
		length = sizeof (struct NDIS_PNP_CAPABILITIES);
		memset(outbuf, 0, length);
		retval = 0;
		break;
	case OID_PNP_QUERY_POWER:
		pr_debug("%s: OID_PNP_QUERY_POWER D%d\n", __func__,
				le32_to_cpup((__le32 *) buf) - 1);
		/* only suspend is a real power state, and
		 * it can't be entered by OID_PNP_SET_POWER...
		 */
		length = 0;
		retval = 0;
		break;
#endif

	default:
		pr_warning("%s: query unknown OID 0x%08X\n",
			 __func__, OID);
	}
	if (retval < 0)
		length = 0;

	resp->InformationBufferLength = cpu_to_le32(length);
	r->length = length + sizeof(*resp);
	resp->MessageLength = cpu_to_le32(r->length);
	return retval;
}

static int gen_ndis_set_resp(u8 configNr, u32 OID, u8 *buf, u32 buf_len,
			     rndis_resp_t *r)
{
	rndis_set_cmplt_type *resp;
	int i, retval = -ENOTSUPP;
	struct rndis_params *params;

	if (!r)
		return -ENOMEM;
	resp = (rndis_set_cmplt_type *)r->buf;
	if (!resp)
		return -ENOMEM;

	if (buf_len && rndis_debug > 1) {
		pr_debug("set OID %08x value, len %d:\n", OID, buf_len);
		for (i = 0; i < buf_len; i += 16) {
			pr_debug("%03d: %08x %08x %08x %08x\n", i,
				get_unaligned_le32(&buf[i]),
				get_unaligned_le32(&buf[i + 4]),
				get_unaligned_le32(&buf[i + 8]),
				get_unaligned_le32(&buf[i + 12]));
		}
	}

	params = &rndis_per_dev_params[configNr];
	switch (OID) {
	case RNDIS_OID_GEN_CURRENT_PACKET_FILTER:

		/* these NDIS_PACKET_TYPE_* bitflags are shared with
		 * cdc_filter; it's not RNDIS-specific
		 * NDIS_PACKET_TYPE_x == USB_CDC_PACKET_TYPE_x for x in:
		 *	PROMISCUOUS, DIRECTED,
		 *	MULTICAST, ALL_MULTICAST, BROADCAST
		 */
		*params->filter = (u16)get_unaligned_le32(buf);
		pr_debug("%s: RNDIS_OID_GEN_CURRENT_PACKET_FILTER %08x\n",
			__func__, *params->filter);

		/* this call has a significant side effect:  it's
		 * what makes the packet flow start and stop, like
		 * activating the CDC Ethernet altsetting.
		 */
#ifdef	RNDIS_PM
update_linkstate:
#endif
		retval = 0;
		if (*params->filter) {
			params->state = RNDIS_DATA_INITIALIZED;
			netif_carrier_on(params->dev);
			if (netif_running(params->dev))
				netif_wake_queue(params->dev);
		} else {
			params->state = RNDIS_INITIALIZED;
			netif_carrier_off(params->dev);
			netif_stop_queue(params->dev);
		}
		break;

	case RNDIS_OID_802_3_MULTICAST_LIST:
		/* I think we can ignore this */
		pr_debug("%s: RNDIS_OID_802_3_MULTICAST_LIST\n", __func__);
#ifdef CONFIG_MACH_PUMA5
		/* TAG0010*/
		memset (rndis_per_dev_params[configNr].mcast_addr, 0,
                        RNDIS_MAX_MULTICAST_SIZE * 6);
        memcpy (rndis_per_dev_params[configNr].mcast_addr,
                buf, buf_len);

		retval = 0;

#else 
		retval = 0;
#endif
		break;
#ifdef CONFIG_MACH_PUMA5
	case RNDIS_OID_GEN_RNDIS_CONFIG_PARAMETER:
		{
                struct rndis_config_parameter   *param;
                char   *strbuf;
                u16    param_name [80], parlen, index = 0;
                u8      first, second;


		param = (struct rndis_config_parameter *) buf;
                strbuf = buf + cpu_to_le32(param->ParameterNameOffset);

		string_to_unicode ("NetworkAddress", param_name, &parlen);
                if ((parlen == cpu_to_le32(param->ParameterNameLength)) &&
                        ((memcmp (param_name, strbuf, parlen) == 0)))
                {
                        parlen = cpu_to_le32(param->ParameterValueLength);
                        strbuf = buf + cpu_to_le32(param->ParameterValueOffset);
                        while (parlen)
                        {
                                first = unicode_to_macnibble (cpu_to_le16p((u16*)strbuf));
                                strbuf += 2;
                                second = unicode_to_macnibble (cpu_to_le16p((u16*)strbuf));
                                strbuf += 2;
                        if ((first != 0xff) && (second != 0xff))
                        {
                                rndis_per_dev_params[configNr].host_mac [index] = (first << 4) | second;
                                parlen -= 4;
                                index ++;
                        } else {
                                break;
                        }
                        }
                        retval = 0;
                }
                }

		break;
#endif

#ifdef	RNDIS_PM
	case OID_PNP_SET_POWER:
		/* The only real power state is USB suspend, and RNDIS requests
		 * can't enter it; this one isn't really about power.  After
		 * resuming, Windows forces a reset, and then SET_POWER D0.
		 * FIXME ... then things go batty; Windows wedges itself.
		 */
		i = le32_to_cpup((__force __le32 *)buf);
		pr_debug("%s: OID_PNP_SET_POWER D%d\n", __func__, i - 1);
		switch (i) {
		case NdisDeviceStateD0:
			*params->filter = params->saved_filter;
			goto update_linkstate;
		case NdisDeviceStateD3:
		case NdisDeviceStateD2:
		case NdisDeviceStateD1:
			params->saved_filter = *params->filter;
			retval = 0;
			break;
		}
		break;

#ifdef	RNDIS_WAKEUP
	// no wakeup support advertised, so wakeup OIDs always fail:
	//  - OID_PNP_ENABLE_WAKE_UP
	//  - OID_PNP_{ADD,REMOVE}_WAKE_UP_PATTERN
#endif

#endif	/* RNDIS_PM */

	default:
		pr_warning("%s: set unknown OID 0x%08X, size %d\n",
			 __func__, OID, buf_len);
	}

	return retval;
}

/*
 * Response Functions
 */

static int rndis_init_response(int configNr, rndis_init_msg_type *buf)
{
	rndis_init_cmplt_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	if (!params->dev)
		return -ENOTSUPP;

	r = rndis_add_response(configNr, sizeof(rndis_init_cmplt_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_init_cmplt_type *)r->buf;

#ifdef CONFIG_MACH_PUMA5
	if (!resp) return -ENOMEM;
#endif

	resp->MessageType = cpu_to_le32(RNDIS_MSG_INIT_C);
	resp->MessageLength = cpu_to_le32(52);
	resp->RequestID = buf->RequestID; /* Still LE in msg buffer */
	resp->Status = cpu_to_le32(RNDIS_STATUS_SUCCESS);
	resp->MajorVersion = cpu_to_le32(RNDIS_MAJOR_VERSION);
	resp->MinorVersion = cpu_to_le32(RNDIS_MINOR_VERSION);
	resp->DeviceFlags = cpu_to_le32(RNDIS_DF_CONNECTIONLESS);
	resp->Medium = cpu_to_le32(RNDIS_MEDIUM_802_3);
	resp->MaxPacketsPerTransfer = cpu_to_le32(1);
	resp->MaxTransferSize = cpu_to_le32(
		  params->dev->mtu
		+ sizeof(struct ethhdr)
		+ sizeof(struct rndis_packet_msg_type)
		+ 22);
	resp->PacketAlignmentFactor = cpu_to_le32(0);
	resp->AFListOffset = cpu_to_le32(0);
	resp->AFListSize = cpu_to_le32(0);

	if (params->ack)
		params->ack (params->dev);
#ifdef CONFIG_MACH_PUMA5
	/* TAG0002 */
/*
	if (params->media_state == NDIS_MEDIA_STATE_CONNECTED)
                rndis_indicate_status_msg (configNr,
                                          RNDIS_STATUS_MEDIA_CONNECT);
*/
#endif
	return 0;
}

static int rndis_query_response(int configNr, rndis_query_msg_type *buf)
{
	rndis_query_cmplt_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	/* pr_debug("%s: OID = %08X\n", __func__, cpu_to_le32(buf->OID)); */
	if (!params->dev)
		return -ENOTSUPP;

	/*
	 * we need more memory:
	 * gen_ndis_query_resp expects enough space for
	 * rndis_query_cmplt_type followed by data.
	 * oid_supported_list is the largest data reply
	 */
	r = rndis_add_response(configNr,
		sizeof(oid_supported_list) + sizeof(rndis_query_cmplt_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_query_cmplt_type *)r->buf;

	resp->MessageType = cpu_to_le32(RNDIS_MSG_QUERY_C);
	resp->RequestID = buf->RequestID; /* Still LE in msg buffer */

	if (gen_ndis_query_resp(configNr, le32_to_cpu(buf->OID),
			le32_to_cpu(buf->InformationBufferOffset)
					+ 8 + (u8 *)buf,
			le32_to_cpu(buf->InformationBufferLength),
			r)) {
		/* OID not supported */
		resp->Status = cpu_to_le32(RNDIS_STATUS_NOT_SUPPORTED);
		resp->MessageLength = cpu_to_le32(sizeof *resp);
		resp->InformationBufferLength = cpu_to_le32(0);
		resp->InformationBufferOffset = cpu_to_le32(0);
	} else
		resp->Status = cpu_to_le32(RNDIS_STATUS_SUCCESS);

	if (params->ack)
		params->ack (params->dev);

	return 0;
}

static int rndis_set_response(int configNr, rndis_set_msg_type *buf)
{
	u32 BufLength, BufOffset;
	rndis_set_cmplt_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	r = rndis_add_response(configNr, sizeof(rndis_set_cmplt_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_set_cmplt_type *)r->buf;

	BufLength = le32_to_cpu(buf->InformationBufferLength);
	BufOffset = le32_to_cpu(buf->InformationBufferOffset);

#ifdef	VERBOSE_DEBUG
	pr_debug("%s: Length: %d\n", __func__, BufLength);
	pr_debug("%s: Offset: %d\n", __func__, BufOffset);
	pr_debug("%s: InfoBuffer: ", __func__);

	for (i = 0; i < BufLength; i++) {
		pr_debug("%02x ", *(((u8 *) buf) + i + 8 + BufOffset));
	}

	pr_debug("\n");
#endif

	resp->MessageType = cpu_to_le32(RNDIS_MSG_SET_C);
	resp->MessageLength = cpu_to_le32(16);
	resp->RequestID = buf->RequestID; /* Still LE in msg buffer */
	if (gen_ndis_set_resp(configNr, le32_to_cpu(buf->OID),
			((u8 *)buf) + 8 + BufOffset, BufLength, r))
		resp->Status = cpu_to_le32(RNDIS_STATUS_NOT_SUPPORTED);
	else
		resp->Status = cpu_to_le32(RNDIS_STATUS_SUCCESS);

	if (params->ack)
		params->ack (params->dev);

	return 0;
}

static int rndis_reset_response(int configNr, rndis_reset_msg_type *buf)
{
	rndis_reset_cmplt_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	r = rndis_add_response(configNr, sizeof(rndis_reset_cmplt_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_reset_cmplt_type *)r->buf;

	resp->MessageType = cpu_to_le32(RNDIS_MSG_RESET_C);
	resp->MessageLength = cpu_to_le32(16);
	resp->Status = cpu_to_le32(RNDIS_STATUS_SUCCESS);
	/* resent information */
	resp->AddressingReset = cpu_to_le32(1);

	if (params->ack)
		params->ack (params->dev);

	return 0;
}

static int rndis_keepalive_response(int configNr,
				    rndis_keepalive_msg_type *buf)
{
	rndis_keepalive_cmplt_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	/* host "should" check only in RNDIS_DATA_INITIALIZED state */

	r = rndis_add_response(configNr, sizeof(rndis_keepalive_cmplt_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_keepalive_cmplt_type *)r->buf;

	resp->MessageType = cpu_to_le32(RNDIS_MSG_KEEPALIVE_C);
	resp->MessageLength = cpu_to_le32(16);
	resp->RequestID = buf->RequestID; /* Still LE in msg buffer */
	resp->Status = cpu_to_le32(RNDIS_STATUS_SUCCESS);

	if (params->ack)
		params->ack (params->dev);

	return 0;
}


/*
 * Device to Host Comunication
 */
static int rndis_indicate_status_msg(int configNr, u32 status)
{
	rndis_indicate_status_msg_type *resp;
	rndis_resp_t *r;
	struct rndis_params *params = rndis_per_dev_params + configNr;

	if (params->state == RNDIS_UNINITIALIZED)
		return -ENOTSUPP;

	r = rndis_add_response(configNr,
				sizeof(rndis_indicate_status_msg_type));
	if (!r)
		return -ENOMEM;
	resp = (rndis_indicate_status_msg_type *)r->buf;

	resp->MessageType = cpu_to_le32(RNDIS_MSG_INDICATE);
	resp->MessageLength = cpu_to_le32(20);
	resp->Status = cpu_to_le32(status);
	resp->StatusBufferLength = cpu_to_le32(0);
	resp->StatusBufferOffset = cpu_to_le32(0);

	if (params->ack)
		params->ack (params->dev);

	return 0;
}

int rndis_signal_connect(int configNr)
{
	rndis_per_dev_params[configNr].media_state
			= RNDIS_MEDIA_STATE_CONNECTED;
	return rndis_indicate_status_msg(configNr,
					  RNDIS_STATUS_MEDIA_CONNECT);
}
EXPORT_SYMBOL(rndis_signal_connect);
#ifdef CONFIG_MACH_PUMA5
int rndis_signal_disconnect (int configNr, u8 logical)
#else
int rndis_signal_disconnect(int configNr)
#endif
{
#ifdef CONFIG_MACH_PUMA5
        u8      *buf;
        u32     temp;
#endif

	rndis_per_dev_params[configNr].media_state
			= RNDIS_MEDIA_STATE_DISCONNECTED;
#ifdef CONFIG_MACH_PUMA5
        if (!logical)
        {

                rndis_per_dev_params [configNr].state = RNDIS_UNINITIALIZED;
                while ((buf = rndis_get_next_response (configNr, &temp)))
                        rndis_free_response (configNr, buf);
                return 0;
        } else
#endif
	return rndis_indicate_status_msg(configNr,
					  RNDIS_STATUS_MEDIA_DISCONNECT);
}
EXPORT_SYMBOL(rndis_signal_disconnect);

void rndis_uninit(int configNr)
{
	u8 *buf;
	u32 length;

	if (configNr >= RNDIS_MAX_CONFIGS)
		return;
	rndis_per_dev_params [configNr].used = 0;
	rndis_per_dev_params[configNr].state = RNDIS_UNINITIALIZED;

	/* drain the response queue */
	while ((buf = rndis_get_next_response(configNr, &length)))
		rndis_free_response(configNr, buf);
}
EXPORT_SYMBOL(rndis_uninit);

void rndis_set_host_mac(int configNr, const u8 *addr)
{
	rndis_per_dev_params[configNr].host_mac = (u8*)addr;
#ifdef CONFIG_MACH_PUMA5
	memcpy ((void *)rndis_per_dev_params[configNr].perm_mac, addr, 6);
#endif
}
EXPORT_SYMBOL(rndis_set_host_mac);

/*
 * Message Parser
 */
int rndis_msg_parser(u8 configNr, u8 *buf)
{
	u32 MsgType, MsgLength;
	__le32 *tmp;
	struct rndis_params *params;

	if (!buf)
		return -ENOMEM;

	tmp = (__le32 *)buf;
	MsgType   = get_unaligned_le32(tmp++);
	MsgLength = get_unaligned_le32(tmp++);

	if (configNr >= RNDIS_MAX_CONFIGS)
		return -ENOTSUPP;
	params = &rndis_per_dev_params[configNr];

	/* NOTE: RNDIS is *EXTREMELY* chatty ... Windows constantly polls for
	 * rx/tx statistics and link status, in addition to KEEPALIVE traffic
	 * and normal HC level polling to see if there's any IN traffic.
	 */

	/* For USB: responses may take up to 10 seconds */
	switch (MsgType) {
	case RNDIS_MSG_INIT:
		pr_debug("%s: RNDIS_MSG_INIT\n",
			__func__);
		params->state = RNDIS_INITIALIZED;
		return rndis_init_response(configNr,
					(rndis_init_msg_type *)buf);

	case RNDIS_MSG_HALT:
		pr_debug("%s: RNDIS_MSG_HALT\n",
			__func__);
		params->state = RNDIS_UNINITIALIZED;
		if (params->dev) {
#ifdef CONFIG_MACH_PUMA5
			 memcpy (
                (void *)rndis_per_dev_params[configNr].host_mac,
                (void *)rndis_per_dev_params[configNr].perm_mac, 6); 
#endif
			netif_carrier_off(params->dev);
			netif_stop_queue(params->dev);
		}
		return 0;

	case RNDIS_MSG_QUERY:
		return rndis_query_response(configNr,
					(rndis_query_msg_type *)buf);

	case RNDIS_MSG_SET:
		return rndis_set_response(configNr,
					(rndis_set_msg_type *)buf);

	case RNDIS_MSG_RESET:
		pr_debug("%s: RNDIS_MSG_RESET\n",
			__func__);
		return rndis_reset_response(configNr,
					(rndis_reset_msg_type *)buf);

	case RNDIS_MSG_KEEPALIVE:
		/* For USB: host does this every 5 seconds */
		if (rndis_debug > 1)
			pr_debug("%s: RNDIS_MSG_KEEPALIVE\n",
				__func__);
		return rndis_keepalive_response(configNr,
						 (rndis_keepalive_msg_type *)
						 buf);

	default:
		/* At least Windows XP emits some undefined RNDIS messages.
		 * In one case those messages seemed to relate to the host
		 * suspending itself.
		 */
		pr_warning("%s: unknown RNDIS message 0x%08X len %d\n",
			__func__, MsgType, MsgLength);
		print_hex_dump_bytes(__func__, DUMP_PREFIX_OFFSET,
				     buf, MsgLength);
		break;
	}

	return -ENOTSUPP;
}
EXPORT_SYMBOL(rndis_msg_parser);

int rndis_register(int (* rndis_control_ack) (struct net_device *))
{
	u8 i;

	if (!rndis_control_ack)
		return -EINVAL;

	for (i = 0; i < RNDIS_MAX_CONFIGS; i++) {
		if (!rndis_per_dev_params[i].used) {
			rndis_per_dev_params[i].used = 1;
			rndis_per_dev_params[i].ack = rndis_control_ack;
			pr_debug("%s: configNr = %d\n", __func__, i);
			return i;
		}
	}
	pr_debug("failed\n");

	return -ENODEV;
}
EXPORT_SYMBOL(rndis_register);

void rndis_deregister(int configNr)
{
	pr_debug("%s:\n", __func__);

	if (configNr >= RNDIS_MAX_CONFIGS) return;
	rndis_per_dev_params[configNr].used = 0;

	return;
}
EXPORT_SYMBOL(rndis_deregister);

int rndis_set_param_dev(u8 configNr, struct net_device *dev, 
			 struct net_device_stats *stats, 
			 u16 *cdc_filter)
{
	pr_debug("%s:\n", __func__);
	if (!dev)
		return -EINVAL;
	if (configNr >= RNDIS_MAX_CONFIGS) return -1;

	rndis_per_dev_params[configNr].dev = dev;
	rndis_per_dev_params [configNr].stats = stats;
	rndis_per_dev_params[configNr].filter = cdc_filter;

	return 0;
}
EXPORT_SYMBOL(rndis_set_param_dev);

int rndis_set_param_vendor(u8 configNr, u32 vendorID, const char *vendorDescr)
{
	pr_debug("%s:\n", __func__);
	if (!vendorDescr) return -1;
	if (configNr >= RNDIS_MAX_CONFIGS) return -1;

	rndis_per_dev_params[configNr].vendorID = vendorID;
	rndis_per_dev_params[configNr].vendorDescr = vendorDescr;

	return 0;
}
EXPORT_SYMBOL(rndis_set_param_vendor);

int rndis_set_param_medium(u8 configNr, u32 medium, u32 speed)
{
	pr_debug("%s: %u %u\n", __func__, medium, speed);
	if (configNr >= RNDIS_MAX_CONFIGS) return -1;

	rndis_per_dev_params[configNr].medium = medium;
	rndis_per_dev_params[configNr].speed = speed;

	return 0;
}
EXPORT_SYMBOL(rndis_set_param_medium);

#ifdef CONFIG_MACH_PUMA5
u32  rndis_get_param_filter (u8 configNr)
{
        return (u32)(*rndis_per_dev_params [configNr].filter);
}
#endif

void rndis_add_hdr(struct sk_buff *skb)
{
	struct rndis_packet_msg_type *header;

	if (!skb)
		return;
	header = (void *)skb_push(skb, sizeof(*header));
	memset(header, 0, sizeof *header);
	header->MessageType = cpu_to_le32(RNDIS_MSG_PACKET);
	header->MessageLength = cpu_to_le32(skb->len);
	header->DataOffset = cpu_to_le32(36);
	header->DataLength = cpu_to_le32(skb->len - sizeof(*header));
}
EXPORT_SYMBOL(rndis_add_hdr);

void rndis_free_response(int configNr, u8 *buf)
{
	rndis_resp_t *r;
	struct list_head *act, *tmp;

	list_for_each_safe(act, tmp,
			&(rndis_per_dev_params[configNr].resp_queue))
	{
		r = list_entry(act, rndis_resp_t, list);
		if (r && r->buf == buf) {
			list_del(&r->list);
			kfree(r);
		}
	}
}
EXPORT_SYMBOL(rndis_free_response);

u8 *rndis_get_next_response(int configNr, u32 *length)
{
	rndis_resp_t *r;
	struct list_head *act, *tmp;

	if (!length) return NULL;

	list_for_each_safe(act, tmp,
			&(rndis_per_dev_params[configNr].resp_queue))
	{
		r = list_entry(act, rndis_resp_t, list);
		if (!r->send) {
			r->send = 1;
			*length = r->length;
			return r->buf;
		}
	}

	return NULL;
}
EXPORT_SYMBOL(rndis_get_next_response);

static rndis_resp_t *rndis_add_response(int configNr, u32 length)
{
	rndis_resp_t *r;

	/* NOTE: this gets copied into ether.c USB_BUFSIZ bytes ... */
	r = kmalloc(sizeof(rndis_resp_t) + length, GFP_ATOMIC);
	if (!r) return NULL;

	r->buf = (u8 *)(r + 1);
	r->length = length;
	r->send = 0;

	list_add_tail(&r->list,
		&(rndis_per_dev_params[configNr].resp_queue));
	return r;
}

int rndis_rm_hdr(struct sk_buff *skb)
{
	/* tmp points to a struct rndis_packet_msg_type */
	__le32 *tmp = (void *)skb->data;

	/* MessageType, MessageLength */
	if (cpu_to_le32(RNDIS_MSG_PACKET)
			!= get_unaligned(tmp++)) {
		dev_kfree_skb_any(skb);
		return -EINVAL;
	}
	tmp++;

	/* DataOffset, DataLength */
	if (!skb_pull(skb, get_unaligned_le32(tmp++) + 8)) {
		dev_kfree_skb_any(skb);
		return -EOVERFLOW;
	}
	skb_trim(skb, get_unaligned_le32(tmp++));

	return 0;
}
EXPORT_SYMBOL(rndis_rm_hdr);

#ifdef CONFIG_USB_GADGET_DEBUG_FILES

static int rndis_proc_show(struct seq_file *m, void *v)
{
	rndis_params *param = m->private;

	seq_printf(m,
			 "Config Nr. %d\n"
			 "used      : %s\n"
			 "state     : %s\n"
			 "medium    : 0x%08X\n"
			 "speed     : %d\n"
			 "cable     : %s\n"
			 "vendor ID : 0x%08X\n"
			 "vendor    : %s\n",
			 param->confignr, (param->used) ? "y" : "n",
			 ({ char *s = "?";
			 switch (param->state) {
			 case RNDIS_UNINITIALIZED:
				s = "RNDIS_UNINITIALIZED"; break;
			 case RNDIS_INITIALIZED:
				s = "RNDIS_INITIALIZED"; break;
			 case RNDIS_DATA_INITIALIZED:
				s = "RNDIS_DATA_INITIALIZED"; break;
			}; s; }),
			 param->medium,
			 (param->media_state) ? 0 : param->speed*100,
			 (param->media_state) ? "disconnected" : "connected",
			 param->vendorID, param->vendorDescr);
	return 0;
}

static ssize_t rndis_proc_write(struct file *file, const char __user *buffer,
				size_t count, loff_t *ppos)
{
	rndis_params *p = PDE_DATA(file_inode(file));
	u32 speed = 0;
	int i, fl_speed = 0;

	for (i = 0; i < count; i++) {
		char c;
		if (get_user(c, buffer))
			return -EFAULT;
		switch (c) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			fl_speed = 1;
			speed = speed * 10 + c - '0';
			break;
		case 'C':
		case 'c':
			rndis_signal_connect(p->confignr);
			break;
		case 'D':
		case 'd':
#ifdef CONFIG_MACH_PUMA5
			rndis_signal_disconnect(p->confignr,0);
#else
			rndis_signal_disconnect(p->confignr);
#endif
			break;
		default:
			if (fl_speed) p->speed = speed;
			else pr_debug("%c is not valid\n", c);
			break;
		}

		buffer++;
	}

	return count;
}

static int rndis_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, rndis_proc_show, PDE_DATA(inode));
}

static const struct file_operations rndis_proc_fops = {
	.owner		= THIS_MODULE,
	.open		= rndis_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
	.write		= rndis_proc_write,
};

#define	NAME_TEMPLATE "driver/rndis-%03d"

static struct proc_dir_entry *rndis_connect_state [RNDIS_MAX_CONFIGS];

#endif /* CONFIG_USB_GADGET_DEBUG_FILES */


int rndis_init(void)
{
	u8 i;

	for (i = 0; i < RNDIS_MAX_CONFIGS; i++) {
#ifdef	CONFIG_USB_GADGET_DEBUG_FILES
		char name [20];

		sprintf(name, NAME_TEMPLATE, i);
		rndis_connect_state[i] = proc_create_data(name, 0660, NULL,
					&rndis_proc_fops,
					(void *)(rndis_per_dev_params + i));
		if (!rndis_connect_state[i]) {
			pr_debug("%s: remove entries", __func__);
			while (i) {
				sprintf(name, NAME_TEMPLATE, --i);
				remove_proc_entry(name, NULL);
			}
			pr_debug("\n");
			return -EIO;
		}
		rndis_connect_state [i]->nlink = 1;
		rndis_connect_state [i]->write_proc = rndis_proc_write;
		rndis_connect_state [i]->read_proc = rndis_proc_read;
		rndis_connect_state [i]->data = (void *)
				(rndis_per_dev_params + i);
#endif
		rndis_per_dev_params[i].confignr = i;
		rndis_per_dev_params[i].used = 0;
		rndis_per_dev_params[i].state = RNDIS_UNINITIALIZED;
		rndis_per_dev_params[i].media_state
				= RNDIS_MEDIA_STATE_DISCONNECTED;
		INIT_LIST_HEAD(&(rndis_per_dev_params[i].resp_queue));
	}

	return 0;
}

void rndis_exit(void)
{
#ifdef CONFIG_USB_GADGET_DEBUG_FILES
	u8 i;
	char name[20];

	for (i = 0; i < RNDIS_MAX_CONFIGS; i++) {
		sprintf(name, NAME_TEMPLATE, i);
		remove_proc_entry(name, NULL);
	}
#endif
}
#ifdef CONFIG_MACH_PUMA5
char  rndis_get_multicast_status (int cfg, const u8 *addr)
{
        u32     index = 0;
        char    status = 0;

        while (index < RNDIS_MAX_MULTICAST_SIZE)
        {
                if ((rndis_per_dev_params[cfg].mcast_addr[index][0] == addr[0]) &&
                (rndis_per_dev_params[cfg].mcast_addr[index][1] == addr[1]) &&
                (rndis_per_dev_params[cfg].mcast_addr[index][2] == addr[2]) &&
                (rndis_per_dev_params[cfg].mcast_addr[index][3] == addr[3]) &&
                (rndis_per_dev_params[cfg].mcast_addr[index][4] == addr[4]) &&
                (rndis_per_dev_params[cfg].mcast_addr[index][5] == addr[5]))
                {
                        status = 1;
                        break;
                }
                index++;
        }

        return status;
}
/* TAG0009 */
static void string_to_unicode (char *string, u16 *unicode_string, u16 *length)
{
        u32     index;

        /* The UNICODE string will be twice the length of the ANSI string. */
        *length = 2 * strlen (string);

        /* Convert the ANSI String to UNICODE format. */
        for (index = 0; index < strlen (string); index++)
                unicode_string[index] = cpu_to_le16((u16)(string[index]));

}

static u8 unicode_to_macnibble (u16 value)
{
        u8 retval  = 0xff;

        if ( (value >= '0') && (value <= '9') )
        {
                retval = value - '0';
        } else {
                if ( (value >= 'A') && (value <= 'F') )
                {
                        retval = value - '7';
                } else {
                        if ( (value >= 'a') && (value <= 'f') )
                        {
                                retval = value - 'W';
                        }
                }
        }

        return retval;
}
int is_rndis_configured(int configNr)
{

        if (rndis_per_dev_params [configNr].state == RNDIS_UNINITIALIZED)
                return 0;
        else
	if (rndis_per_dev_params [configNr].state == RNDIS_INITIALIZED)
                return 1;
	else
		return -1;
}
#endif
