/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @file u_net_util.h
 * $RCSfile: $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * @par Description:
 *      network utilities
 */
/*----------------------------------------------------------------------------*/
/*!
* @addtogroup  groupMW_INET
* @{
*/

#ifndef U_NET_UTILITY_H
#define U_NET_UTILITY_H

#include "u_net_common.h"

/**
 * Frame packet type
 */
#define X_IP_PACKET           0x0800        /**< ip packet */
#define X_ARP_PACKET          0x0806        /**< arp packet */
#define X_RARP_PACKET         0x8035        /**< rarp packet */

/**
 * http://www.iana.org/assignments/protocol-numbers
 */
#define X_PROTOCOL_ICMP       1             /**< internet protocol icmp */
#define X_PROTOCOL_IGMP       2             /**< internet protocol igmp */
#define X_PROTOCOL_TCP        6             /**< internet protocol tcp */
#define X_PROTOCOL_EGP        8             /**< internet protocol egp */
#define X_PROTOCOL_IGP        9             /**< internet protocol igp */
#define X_PROTOCOL_UDP        17            /**< internet protocol udp */
#define X_PROTOCOL_IPv6       41            /**< internet protocol ipv6 */
#define X_PROTOCOL_IPv6_ICMP  58            /**< internet protocol ipv6 icmp */
#define X_PROTOCOL_L2TP       115           /**< internet protocol l2tp */

/**
 * http://www.iana.org/assignments/port-numbers
 */
#define X_PORT_DNS               53         /**< internet port dns */
#define X_PORT_BOOTPS          67           /**< internet port bootp server */
#define X_PORT_BOOTPC         68            /**< internet port bootp client */
#define X_PORT_TFTP              69         /**< internet port tftp */
#define X_PORT_HTTP              80         /**< internet port http */

/**
 * Header length
 */
#define X_ETHERNET_HEADER_LENGTH       14                               /**< ethernet header length */
#define X_UDP_HEADER_LENGTH                 8                           /**< udp header length */
#define X_TCP_HEADER_LENGTH                 20 /**< tcp header normal length (not include option) */

/**
 * Frame type
 */
#define X_MAC_FRAME_TYPE_ETHERNET_II  1     /**< ethernet II frame */
#define X_MAC_FRAME_TYPE_802_2_LLC    2     /**< 802.2 LLC frame */
#define X_MAC_FRAME_TYPE_802_2_SNAP   3     /**< 802.2 snap frame */

/**
 * IP header offset
 */
#define X_IP_TOS_OFFSET              1      /**< IP tos offset */
#define X_IP_TOTAL_LENGTH_OFFSET     2      /**< IP total length offset */
#define X_IP_IDENTIFICATION_OFFSET   4      /**< IP identification offset j*/
#define X_IP_FLAG_OFFSET             6      /**< IP flag offset */
#define X_IP_TTL_OFFSET              8      /**< IP ttl offset */
#define X_IP_PROTOCOL_OFFSET         9      /**< IP protocol offset */
#define X_IP_HDR_CHKSUM_OFFSET       10     /**< IP header checksum offset */
#define X_IP_SRC_IP_OFFSET           12     /**< IP source IP offset */
#define X_IP_DST_IP_OFFSET           16     /**< IP destination IP offset */

/**
 * TCP header offset
 */
#define X_TCP_DST_PORT_OFFSET        2      /**< TCP destination port offset */
#define X_TCP_SEQUENCE_ID_OFFSET     4      /**< TCP sequence id offset */
#define X_TCP_ACK_ID_OFFSET          8      /**< TCP ACK id offset */
#define X_TCP_HDR_LEN_OFFSET         12     /**< TCP header length offset */
#define X_TCP_FLAG_OFFSET            13     /**< TCP flag offset */
#define X_TCP_WINDOW_SIZE_OFFSET     14     /**< TCP win size offset */
#define X_TCP_CHKSUM_OFFSET          16     /**< TCP checksum offset */

/**
 * UDP header offset
 */
#define X_UDP_DST_PORT_OFFSET        2      /**< UDP destination port offset */
#define X_UDP_LEN_OFFSET             4      /**< UDP length offset */
#define X_UDP_CHKSUM_OFFSET          6      /**< UDP checksum offset */

/**
 * ARP header offset
 */
#define X_ARP_PROT_TYPE_OFFSET       2      /**< ARP port type offset */
#define X_ARP_HARD_SIZE_OFFSET       4      /**< ARP header size offset */
#define X_ARP_PROT_SIZE_OFFSET       5      /**< ARP port size offset */
#define X_ARP_OP_OFFSET              6      /**< ARP op offset */
#define X_ARP_SENDER_MAC_OFFSET      8      /**< ARP sender MAC offset */
#define X_ARP_SENDER_IP_OFFSET      14      /**< ARP sender IP offset */
#define X_ARP_TGT_ETH_ADDR_OFFSET   18      /**< ARP target ethernet address offset */
#define X_ARP_TGT_IP_OFFSET         24      /**< ARP target IP offset */

#define ENABLE_RAW_DUMP  ((UINT32) 1 << 31)     /**< enable dump raw packets */
#define ENABLE_ARP_DUMP  ((UINT32) 1 << 30)     /**< enable dump arp packets */
#define ENABLE_IP_DUMP   ((UINT32) 1 << 29)     /**< enable dump ip packets */
#define ENABLE_UDP_DUMP  ((UINT32) 1 << 28)     /**< enable dump udp packets */
#define ENABLE_TCP_DUMP  ((UINT32) 1 << 27)     /**< enable dump tcp packets */
#define ENABLE_ICMP_DUMP  ((UINT32) 1 << 26)    /**< enable dump icmp packets */
#define ENABLE_DNS_DUMP  ((UINT32) 1 << 25)     /**< enable dump dns packets */
#define ENABLE_DHCP_DUMP  ((UINT32) 1 << 24)    /**< enable dump dhcp packets */
#define ENABLE_TFTP_DUMP  ((UINT32) 1 << 23)    /**< enable dump tftp packets */
#define ENABLE_HTTP_DUMP  ((UINT32) 1 << 22)    /**< enable dump http packets */

/**
 * dump packet
 */
/* Filter tx transfer in my mac (DA = my mac) */
#define FILTER_MAC_MYIN  ((UINT32) 1 << 0)              /**< filter tx transfer in my mac (DA = my mac) */
/* Filter tx transfer out from my mac (SA = my mac) */
#define FILTER_MAC_MYOUT ((UINT32) 1 << 1)              /**< filter tx transfer out from my mac (SA = my mac) */
/* Filter tx transfer in (or out from) my mac (DA = my mac or SA = my mac) */
#define FILTER_MAC_MYIO  (FILTER_MAC_MYIN | FILTER_MAC_MYOUT)   /**< filter tx transfer in (or out from) my mac (DA = my mac or SA = my mac) */

#define FILTER_NI_RX ((UINT32) 1 << 2)                  /**< network interface rx (receive)*/
#define FILTER_NI_TX ((UINT32) 1 << 3)                  /**< network interface tx (transfer) */
#define FILTER_NI_RTX  (FILTER_NI_RX | FILTER_NI_TX)    /**< network interface rx + tx */

#define FILTER_NI_ETH0 ((UINT32) 1 << 4)                    /**< network interface eth0 */
#define FILTER_NI_IP0 ((UINT32) 1 << 5)                     /**< network interface ip0 */
#define FILTER_NI_BOTH  (FILTER_NI_ETH0 | FILTER_NI_IP0)    /**< both network interface eth0 and ip0 */

#define ENABLE_ALL_DUMP (ENABLE_ARP_DUMP | ENABLE_IP_DUMP | ENABLE_UDP_DUMP | ENABLE_TCP_DUMP | \
                         ENABLE_ICMP_DUMP | ENABLE_DNS_DUMP | ENABLE_DHCP_DUMP | ENABLE_TFTP_DUMP | \
                         ENABLE_HTTP_DUMP)      /**< all dump enable */

#define DISABLE_ALL_DUMP ((UINT32) 0)       /**< all dump disable */

/*!
 * @struct FrameInfo_T
 * @brief frame information
 * @code
 * typedef struct FrameInfo_s
 * {
 *     INT16   i2_frame_type;
 *     INT16   i2_frame_len;
 *     UCHAR   *pDaMacAddress;
 *     UCHAR   *pSaMacAddress;
 *     INT16   i2_pkt_type;
 *     INT16   i2_pkt_len;
 *     INT16   i2_pkt_protocol;
 *     UCHAR   *pPktHdrAddr;
 *     UCHAR   *pProtocolHdrAddr;
 *     UCHAR   *pAppHdrAddr;     
 * } FrameInfo_T;
 * @endcode
 *
 * @li@c i2_frame_type      - frame type
 * @li@c i2_frame_len       - frame length
 * @li@c pDaMacAddress      - DA MAC address
 * @li@c pSaMacAddress      - SA MAC address
 * @li@c i2_pkt_type        - packet type
 * @li@c i2_pkt_len         - packet length
 * @li@c i2_pkt_protocol    - packet protocol
 * @li@c pPktHdrAddr        - packet header address
 * @li@c pProtocolHdrAddr   - protocol header address
 * @li@c pAppHdrAddr        - app header address
 */
typedef struct FrameInfo_s
{
    /**
     * Frame info
     */
    INT16   i2_frame_type;
    INT16   i2_frame_len;
    UCHAR   *pDaMacAddress;
    UCHAR   *pSaMacAddress;
    /**
     * Packet info
     */
    INT16   i2_pkt_type;
    INT16   i2_pkt_len;
    INT16   i2_pkt_protocol;
    /**
     * IP/ARP/RARP hdr start
     */
    UCHAR   *pPktHdrAddr;
    /**
     * TCP/UDP/ICMP hdr start
     */
    UCHAR   *pProtocolHdrAddr;
    /**
     * TFTP/DNS/DHCP/HTTP hdr start
     */    
    UCHAR   *pAppHdrAddr;     
} FrameInfo_T;

#endif

/*! @} */

